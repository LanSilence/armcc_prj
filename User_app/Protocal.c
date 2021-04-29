#include "Protocal.h"

UPGARDE_PARAMER Upgrade_Paramer;
uint16_t Pack_Sum = 0;     //升级包总数
uint16_t Recv_PackSum = 0; //接收的总数

unsigned short CRC16_0xA001(unsigned char *pszBuf, unsigned short unLength)
{
    unsigned short i, j;
    unsigned short CrcReg = 0xFFFF;

    for (i = 0; i < unLength; i++)
    {
        CrcReg = CrcReg ^ pszBuf[i];

        for (j = 0; j < 8; j++)
        {
            if ((CrcReg & 0x0001) == 0x0001)
            {
                CrcReg >>= 1;
                CrcReg ^= 0xA001;
            }
            else
            {
                CrcReg >>= 1;
            }
        }
    }
    return (unsigned short)CrcReg;
}

uint16_t Get_FreezeData(uint8_t *Pout, uint8_t *Multiframe)
{
    int p_read = 0;
    int p_write = 0;
    uint8_t  datanum = 0;
    uint16_t datalen = 0;
    uint32_t dataread = 0;
    //short result;
    
    bin_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_END, &p_read,  0x11);  //按照4个字节查找 flash对齐 p_write偏移量
    order_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_END, &p_write,  0x55);  //按照4个字节查找 flash对齐 p_write偏移量
    
    dataread = *(uint32_t *)(ADDR_P_DATA_FREEZE+p_read);
    if(dataread == 0x11)   
    {
        datanum = (p_write - p_read)/4;
        p_read += 4;   //读取的地址往前移动一个单位
    }
    else if(dataread == 0x55)  //判断零地址上被读取过没有
        datanum = (p_write - p_read)/4+1;
    if(p_read>p_write)   //数据错误擦除
    {
        Flash_Erase_Sector(ADDR_P_DATA_FREEZE/512);
        Flash_Erase_Sector(ADDR_DATA_FREEZE/512);
        return 0;
    }
    if(datanum>15 )
    {
        memcpy(Pout,(uint8_t *)(ADDR_DATA_FREEZE+FREEZE_DATA_SIZE*p_read/4),15*FREEZE_DATA_SIZE);
        datalen = 15*FREEZE_DATA_SIZE;
        *Multiframe = 1;
    }
    else if(datanum != 0)
    {
        memcpy(Pout,(uint8_t *)(ADDR_DATA_FREEZE+FREEZE_DATA_SIZE*p_read/4),datanum*FREEZE_DATA_SIZE);
        datalen = datanum*FREEZE_DATA_SIZE;
        *Multiframe = 0;
    }
    return datalen;
}

extern unsigned long mktime_BCD (RTC_Struct *Time);
extern void gmtime(unsigned long p, RTC_Struct *pTime);
uint16_t Get_Temprature_Freeze(uint8_t *Pin, uint8_t *Multiframe)
{
    RTC_Struct time;
    int p_read = -4;
    short result = 0;
    uint16_t hour_num = 0;
    
    uint16_t datalen = 0;
    unsigned long time_start = 0;
    
    *Multiframe = 0;
    hour_num = Check_Temprature_Data(&time_start);
    if(hour_num == 0)     
        return 0;
    
    else if(hour_num > 25)
    {
        hour_num = 25;
        *Multiframe = 2;   //还有数据
    }
    else if(hour_num <= 25)
    {
        *Multiframe = 0;
    }
    
    result = bin_search(ADDR_TEMP_DATA, ADDR_TEMP_END, &p_read,  0x00);
    
    Pin[datalen++] = hour_num;    //数据条数
    Pin[datalen++] = 0;
    gmtime(time_start,&time);
    
    memcpy(&Pin[datalen],&time,4);
    datalen += 4;
    if(result != -1)
    {        
       memcpy(Pin+datalen,(uint32_t *)(ADDR_TEMP_DATA+p_read +4),hour_num*4);          
    }
    else
    {
        memcpy(Pin+datalen,(uint32_t *)(ADDR_TEMP_DATA),hour_num*4);
    }
    datalen += hour_num * 4;
    return datalen;
   
}

uint16_t Get_Alarm_Fram(uint8_t *Pin,uint8_t *Multiframe)
{
    uint16_t datalen = 0;
    
    memcpy(Pin,&DeviceParam.staus,2);
    *Multiframe = 0;
    datalen += 2;
    return datalen;
}
void Get_FrameData(uint16_t cmd)
{
    uint8_t data[190] = {0}; //7B226D7367223A22
    uint16_t datalen = 0;
    uint16_t GetLen = 0;
    uint16_t AES_Len = 0;
    uint16_t crc16 = 0;
    RTC_Struct rtc;
    
    //memcpy(Up_process.TxBuffer,"{\"msg\":\"",8);
    switch(cmd)
    {
    case DATA_0A01:
        GetLen = Get_FreezeData(data+1, &Up_process.Multiframe);    //前面一位留给数据条数
        
        //数据条数
        data[0] = GetLen/FREEZE_DATA_SIZE;
        GetLen++;        
        break;
    case TEMP_0A02:
        GetLen = Get_Temprature_Freeze(data, &Up_process.Multiframe);
        break;
    case ALARM_0A09:
        GetLen = Get_Alarm_Fram(data, &Up_process.Multiframe);
        break;
    }
    AES_Len = GetLen;
#ifdef AES_128
    uint8_t DataOut[(GetLen/16+1)*16]; //输出为16的倍数
    memset(DataOut,0xFF,sizeof(DataOut));
    AES_Len = AES_Encryption(AESKey,data,GetLen,DataOut);
#else
 
#endif    
    /***填充帧头**/
    
    
    Up_process.TxBuffer[datalen++] = 0x68;   //起始
    /******imei******/
    memcpy(&Up_process.TxBuffer[datalen],DeviceParam.id,15);
    datalen += 15;
    
    //版本号
    Up_process.TxBuffer[datalen++] = VER_H;
    Up_process.TxBuffer[datalen++] = VER_L;
    
    //控制码
    Up_process.TxBuffer[datalen++] = 0;
    if(Up_process.Multiframe || (Up_process.Up_Task & (~Up_process.CurrentTask)))   //还有其他任务
        Up_process.TxBuffer[datalen] |= 1 <<4;
    
    //CSQ
    Up_process.TxBuffer[datalen++] = NBDevice.CSQ;
    
    //时间
    RTC_GetTime((RTC_Struct *)&rtc);
    memcpy(&Up_process.TxBuffer[datalen],&rtc,6);
    datalen += 6;
    
    Up_process.TxBuffer[datalen++] = (uint16_t)cmd>>8;
    Up_process.TxBuffer[datalen++] = (uint16_t)cmd;
    
    //数据长度
    Up_process.TxBuffer[datalen++] = (uint8_t)AES_Len;
    Up_process.TxBuffer[datalen++] = (uint8_t)(AES_Len>>8);
    

    /************/
#ifdef AES_128    
    memcpy(&Up_process.TxBuffer[datalen],DataOut,AES_Len);
#else
    memcpy(&Up_process.TxBuffer[datalen],data,GetLen);
#endif
    
    datalen += AES_Len;
    
    //校验
    crc16 = CRC16_0xA001(Up_process.TxBuffer,datalen);   
    Up_process.TxBuffer[datalen++] = (uint8_t)crc16;
    Up_process.TxBuffer[datalen++] = (uint8_t)(crc16>>8);
    
    //结束符
    Up_process.TxBuffer[datalen++] = 0x16;
    
    
    Up_process.TxLength = datalen;    
}

uint8_t mamual_timing(uint8_t *Pin)
{
    RTC_Struct time;
    time.Year       = Pin[0];
    time.Month      = Pin[1];
    time.Day        = Pin[2];
    time.Hour       = Pin[3];
    time.Minute     = Pin[4];
    time.Second     = Pin[5];
    
    return RTC_SetTime(&time);    
}

void CMDRsult_Send(uint8_t* data,uint16_t len)
{
    int i = 0;
    if(len>uart_tx_size)
        return;
    for(i =0;i<= len ;i++)
        print("%02X",*(data+i));
    print("\r\n");
}

uint8_t SetParam(uint8_t *Pin)
{
    uint8_t result = 0;
    if(Pin[0] == 1)    //修改上告参数
    {
        if(Pin[1] <= 3)
            DeviceParam.uptime.uptype = Pin[1];
        else
            return 1;
        DeviceParam.uptime.interval = Pin[2];
        if(Pin[3]<0x24 && Pin[4] < 0x60)
        {
            DeviceParam.uptime.timepoint[1] = Pin[3];
            DeviceParam.uptime.timepoint[2] = Pin[4];
        }
        else 
            return 1;
    }
    
    if(Pin[5] == 1)  //修改温度告警阈值
    {
        float temp = 0;
        memcpy(&temp,&Pin[6],4);
        if(temp<=80 && temp >= -45)
            DeviceParam.Temp_threshold = temp;
        else
            return 1;
    }
    
    if(Pin[10] == 1)  //修改遮挡告警天数
    {
        memcpy(&DeviceParam.ShieldTime,&Pin[11],2);
    }
    if(Pin[13] == 1)   //修改ip端口
    {
      memcpy(DeviceParam.ip,&Pin[14],20);
      memcpy(&DeviceParam.port,&Pin[34],2);
    }
    
    Save_Parame();
    return result;
}

uint16_t Set_Upgrade_Paramer(uint8_t *Pin, uint8_t *Pout)
{
    uint8_t i = 0;
    Pout[0] = VER_H;
    Pout[1] = VER_L;
    if(Pin[2] == VER_H && Pin[3] == VER_L)
    {
        Pout[3] = 1;
        Pout[2] = 0;
        return 4;
    }   
    else 
    {
        Pout[2] = 0;
        Pout[3] = 0;
    }
    Recv_PackSum = 0;
    memcpy(&Upgrade_Paramer,Pin,sizeof(Upgrade_Paramer));
    
    for(i = 0;i<(ADDR_UP_END-ADDR_UPDATA)/512;i++)
    {
        print("正在擦除扇区%d\n",ADDR_UPDATA/512+i);
        Flash_Erase_Sector(ADDR_UPDATA/512+i);
    }
    print("擦除完成\n");
    return 4;
}

uint16_t CheckAllUpgradeCode(uint8_t *Pout)
{
    uint16 i = 0;
    uint8_t tempbuf[16];
    uint8_t j = 0;
    uint16_t ErrorNum = 0;
    for(i=0;i<((ADDR_UP_END-ADDR_UPDATA)/256);i++)
    {
        memcpy(tempbuf,(uint8_t *)(ADDR_UPDATA+i*256),16);
        for(j = 0;j<16;j++)
        {
            if(tempbuf[j] != 0xff)
            {
                break;
            }
        }
        if(j==16)
        {
            memcpy(&Pout[4+ErrorNum*2],&i,2);
            ErrorNum++;
            Pack_Sum--;
            if(ErrorNum>40)
                break;
        }
            
    }
    if(ErrorNum != 0 )  //有错误
    {
        Pout[0] = 0;
        Pout[1] = 1;
        
        memcpy(Pout+2,&ErrorNum,2);
        return 4+2*ErrorNum;
    }
    else
    {
        Pout[0] = 0;
        Pout[1] = 0;
    }
    return 2;
    
}


//检查升级包是否有错误
uint16_t Get_Upgrade_Pack(uint8_t *Pin, uint8_t *Pout)
{
    uint16_t datalen = 0;
    
    memcpy(&Pack_Sum,Pin,2);  //升级包总数
    if(Flash_Write_String((uint32_t)ADDR_UPDATA,(uint32_t *)&Pin[4],256/4))
    {
        //接收失败
        Pout[datalen++] = 0; 
        Pout[datalen++] = 1; 
        
        //失败包数
        Pout[datalen++] = 0;
        Pout[datalen++] = 1;
        
        //序号
        memcpy(&Pout[datalen],&Pin[2],2);
        datalen += 2;
    }
    else
    {
        //接收成功
        Pout[datalen++] = 0; 
        Pout[datalen++] = 0; 
        Recv_PackSum++;
        
        if(Recv_PackSum == Pack_Sum && Recv_PackSum != 0)  //全部接收完成
        {
            datalen = CheckAllUpgradeCode(Pout);  //检查数据是否完整
        }
    }
    
    return datalen;
    
}

uint16_t GetParam(uint8_t *Pout)
{
    uint16_t datalen = 0;
    //上告参数
    Pout[datalen++] = DeviceParam.uptime.uptype;
    Pout[datalen++] = DeviceParam.uptime.interval;
    Pout[datalen++] = DeviceParam.uptime.timepoint[1];
    Pout[datalen++] = DeviceParam.uptime.timepoint[2];
    
    //温度阈值
    memcpy(&Pout[datalen],&DeviceParam.Temp_threshold,4);
    datalen += 4;
    //遮挡天数
    memcpy(&Pout[datalen],&DeviceParam.ShieldTime,2);
    datalen +=2;
    //ip
    
    memcpy(&Pout[datalen],DeviceParam.ip,20);
    datalen += 20;
    memcpy(&Pout[datalen],&DeviceParam.port,2);
    datalen += 2;
    return datalen;
    
    
    
}
void Protocal_Analyze(uint8_t *Pin,uint16_t CMD,uint16_t Len,uint8_t ComType)
{
    uint8_t data[190] = {0};
    uint16_t datalen = 0;    
    uint16_t GetLen = 0;
    uint16_t AES_Len = 0;
    uint16_t crc16 = 0;
    RTC_Struct rtc;
    
    
    switch(CMD)
    {
    case DATA_0A01: 
        StartUp(Type_Poweron);
        break;
    case TEMP_0A02: 
    case ALARM_0A09:
        break;
    case TIME_AAF1:
        data[0] = mamual_timing(Pin);
        GetLen ++;
        break;
    case CMD_0A0A:
        print("收到-参数设置\n");
        SetParam(Pin);
        GetLen = GetParam(data);
        break;
    case UPGRADE_0A0B:
        GetLen = Set_Upgrade_Paramer(Pin,data);
        break;
    case UPGRADE_0A0C:
        GetLen = Get_Upgrade_Pack(Pin,data);//获取升级数据
        break;
    case FACTORY_AAF2:
    {
        for(uint8_t i=0;i<(ALL_FLASH_SIZE-ADDR_TEMP_START)/512;i++)
        {
            Flash_Erase_Sector(ADDR_TEMP_START/512+i);
        }
        memcpy(&DeviceParam,&DefaultParam,sizeof(DefaultParam));
        Save_Parame();
        data[0] = 1;
        GetLen ++;
        print("*恢复出厂*\r\n");
        RCC->SOFTRST = (uint32_t)0x5C5CAABB;   //复位
    }
    case CMD_0AED:  //结束
        Set_UpProcess_UpState(S_UpSucceed,RUN_STA);
        return;
    default:
        break;
    } 
        AES_Len = GetLen;
#ifdef AES_128
    uint8_t DataOut[(GetLen/16+1)*16]; //输出为16的倍数
    memset(DataOut,0xFF,sizeof(DataOut));
    AES_Len = AES_Encryption(AESKey,data,GetLen,DataOut);
#else
 
#endif    
    /***填充帧头**/
    
    
    Up_process.TxBuffer[datalen++] = 0x68;   //起始
    /******imei******/
    memcpy(&Up_process.TxBuffer[datalen],DeviceParam.id,15);
    datalen += 15;
    
    //版本号
    Up_process.TxBuffer[datalen++] = VER_H;
    Up_process.TxBuffer[datalen++] = VER_L;
    
    //控制码
    Up_process.TxBuffer[datalen++] = 0;
    if(Up_process.Multiframe)
        Up_process.TxBuffer[datalen] |= 1 <<4;
    
    //CSQ
    Up_process.TxBuffer[datalen++] = NBDevice.CSQ;
    
    //时间
    RTC_GetTime((RTC_Struct *)&rtc);
    memcpy(&Up_process.TxBuffer[datalen],&rtc,6);
    datalen += 6;
    
    Up_process.TxBuffer[datalen++] = (uint16_t)CMD>>8;
    Up_process.TxBuffer[datalen++] = (uint16_t)CMD;
    
    //数据长度
    Up_process.TxBuffer[datalen++] = (uint8_t)AES_Len;
    Up_process.TxBuffer[datalen++] = (uint8_t)(AES_Len>>8);
    

    /************/
#ifdef AES_128    
    memcpy(&Up_process.TxBuffer[datalen],DataOut,AES_Len);
#else
    memcpy(&Up_process.TxBuffer[datalen],data,GetLen);
#endif
    
    datalen += AES_Len;
    
    //校验
    crc16 = CRC16_0xA001(Up_process.TxBuffer,datalen);   
    Up_process.TxBuffer[datalen++] = (uint8_t)crc16;
    Up_process.TxBuffer[datalen++] = (uint8_t)(crc16>>8);
    
    //结束符
    Up_process.TxBuffer[datalen++] = 0x16;
    
    
    Up_process.TxLength = datalen;  
    
    if(ComType == NBMODE_COM)
    {
        CMDRsult_Send(Up_process.TxBuffer,Up_process.TxLength);
        NBDevice.SendCmd_Pend = 0;
        NBDevice.Process = M_M2MCLISEND;
    }
    else if(ComType == DEBUG_COM)
    {
        Up_process.TxLength = 0;
        Uart1SendData(Up_process.TxBuffer,Up_process.TxLength);
    }
   
}



void Process_Cmd(uint8_t *Pin, uint8_t ComType)
{
    uint16_t CRC_Code = 0;
    uint16_t datalen = 0;
    uint16_t CMD = 0;
    
    memcpy(&datalen,&Pin[28],2);
    datalen += 33;     //数据总长度
    if(datalen >300)    //数据超
        return;
    memcpy(&CRC_Code,&Pin[datalen-3],2);
    
    if(CRC_Code != CRC16_0xA001(Pin,datalen-3))
    {
        print("CRC检验错误！！\r\n");
        return;
    }
    if(memcmp(&Pin[1],DeviceParam.id,15) && ComType == NBMODE_COM)
    {
        print("设备IMEI不匹配！！\r\n");
        return;
    }
    if(Pin[18] & (0x10))  //有后续帧
    {
        Up_process.UpState = S_DataSend;   //等待任务
        Up_process.IncidentPend = 0;
    }
    else
    {
        if(ComType == NBMODE_COM)
            Up_process.UpState = S_TaskEnd;   //等待任务
        Up_process.IncidentPend = 0; 
    }
    
    CMD = Pin[26];
    CMD = (CMD<<8) + Pin[27];
    Protocal_Analyze(Pin+30,CMD,datalen-33,ComType);
    
}

