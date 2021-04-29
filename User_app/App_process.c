#include "App_process.h"

uint16_t ShieldCount = 0;

struct USER_PARAM DefaultParam = 
{
    .id          =  {0},
    .staus.value =   0,
#if TRAN_PRO == MQTT_PRO    
    .ip = {"mqtt.ctwing.cn"},
    .port = 1883,
#elif TRAN_PRO == LWM2M_PRO    
    .ip = {"221.229.214.202"},
    .port = 5683,
#endif    
    .TotalNumber = 0,
    .Temp_threshold = 55,
    .uptime = {
        .uptype = 1,
        .interval = 5,
        .timepoint = {0,0x08,0},
    },
    .UpPeak = 1,                        //  默认开启错峰
    
};

struct USER_PARAM DeviceParam = 
{
    .id          =  {0},
    .staus.value =   0,
    .ip = {183,230,40,39},
    .port = 6002,
    .TotalNumber = 0,
};

uint8_t AESKey[16] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0};

uint8_t check_flash()
{
    return 0;
}

void Load_Param()
{
    if(check_flash())
    {
        
    }
    else
    {}
}


//DeepSleep
void DeepSleep(void)
{
    LL_PMU_SleepInitTypeDef LPM_InitStruct;
    LL_RCC_SetSleepModeRCLPWorkMode(LL_RCC_RCLP_UNDER_SLEEP_CLOSE);//休眠下关闭RCLP
    LL_RMU_EnablePowerDownReset(RMU);//打开PDR
    LL_RMU_DisableBORPowerDownReset(RMU);//关闭BOR 2uA
    
    LL_ADC_Disable(ADC);//关闭ADC
    //LL_VREF_DisableVREF(VREF);//关闭VREF1p2
    WRITE_REG(VREF->BUFCR, 0);//关闭全部VREFbuf
    
    LPM_InitStruct.DeepSleep = LL_PMU_SLEEP_MODE_DEEP;
    LPM_InitStruct.PowerMode = LL_PMU_POWER_MODE_SLEEP_AND_DEEPSLEEP;
    LPM_InitStruct.WakeupFrequency = LL_PMU_SLEEP_WAKEUP_FREQ_RCHF_8MHZ;
    LPM_InitStruct.WakeupDelay = LL_PMU_WAKEUP_DELAY_TIME_2US;
    LPM_InitStruct.CoreVoltageScaling = DISABLE; 
    LL_PMU_Sleep_Init(PMU, &LPM_InitStruct);
}


void EnterSleep(void)
{
    if(IsTaskRunning() )    //电源检测不进入休眠
    {
        //print("%0X\n",PMU->WKFR);
        return;
    }        
    else
    {
        IWDT_Clr();
        DeepSleep();
        IWDT_Clr();
        //print(".");
    }
}

void Save_Event(DIRECT Direct)
{
    
}

void Flash_Read(void)
{
    uint8_t i;
    for(i = 0 ;i<4;i++)
    {
        memcpy(&DeviceParam,(uint32_t *)ADDR_USERPARAME,sizeof(DeviceParam));
        if(memcmp(DeviceParam.id,"86",2) == 0)
        {
            //DeviceParam.UpFlag = 0xFF;
            print("*参数获取成功*\n");
            break;
        }
    }
    if(i>=4)
    {
        memcpy(&DeviceParam,&DefaultParam,sizeof(DeviceParam));
        //DeviceParam.UpFlag =0xFF;//测试升级
    }
    //Save_Parame();
    
}



void CheckCodeToUp()
{
    uint16_t CRCRD;
    uint16_t IapCRC;
    unsigned long FileSize = 0;
    uint8_t data[8] = {0};
    uint8_t i=0;
    struct UPGARDE_PARAMER *param;
        
    
    if(Upgrade_Paramer.UpFlag[0] != 'U' || Upgrade_Paramer.UpFlag[1] != 'P')
        return ;
    Init_CRC_CRC16CCITT();
    
    memcpy(data,(uint32_t *)(ADDR_UPDATA),8);     //提取升级文件中的校验码和文件长度  读8个字节是为了对齐读取
    
    IapCRC = data[0] + (data[1] << 8);
    FileSize = data[2] + (data[3] <<8) +(data[4] <<16) +(data[5] <<24);
    //CRCRD = CalCRC16_CCITT(0xFFFF,crcdata,8 );
    if(FileSize != Upgrade_Paramer.Size)
    {
        print("文件长度错误！！！\r\n");
        return;
    }
    CRCRD = CalCRC16_CCITT(0xFFFF,(__IO uint8_t *)(ADDR_UPDATA+2),FileSize -2);  //前两位为校验，从第二个开始
    print("chec__crc-%04X file__crc-%04X\r\n",CRCRD,IapCRC);
    if(IapCRC == CRCRD)
    {
        
        

        for(i=0;i<3;i++)
        {
            Flash_Erase_Sector((ADDR_UPGRADE_PAMARE)/512);
            Flash_Write_String(ADDR_UPGRADE_PAMARE,(uint32_t *)&Upgrade_Paramer,sizeof(Upgrade_Paramer));  //写标志
            param = (struct UPGARDE_PARAMER *)(ADDR_UPGRADE_PAMARE);
            if(memcmp(&Upgrade_Paramer,param,sizeof(Upgrade_Paramer)) == 0)
                break;                
        }
        if(i>=3)
        {
            print("写升级标志失败\r\n");
            return;
        }
        print("Check OK! Go to BootLoad!\r\n");
        RCC->SOFTRST = (uint32_t)0x5C5CAABB;   //复位

    }
    else
    {
        print("校验错误\r\n");
    }
}


void Save_Parame()
{
    uint8_t i;
    struct USER_PARAM parame; 
    for(i = 0;i<4;i++)
    {
        Flash_Erase_Sector(ADDR_USERPARAME/512);   
        Flash_Write_String(ADDR_USERPARAME,(uint32_t *)&DeviceParam,sizeof(DeviceParam)/4); 
        memcpy(&parame,(uint32_t *)ADDR_USERPARAME,sizeof(parame));
        if(memcmp(&parame,&DeviceParam,sizeof(parame)) == 0)           
        {
            print("*参数保存成功*\n");
            break;
        }                     
    }
        
}


void Write_Freeze_data(DIRECT Dir)
{
    int p_write = -4,p_read = -4;
    uint32_t pdata = 0x55;
    uint8_t data[FREEZE_DATA_SIZE+2] = {0};
    short result = 0;
    RTC_Struct rtc;
    
    RTC_GetTime((RTC_Struct *)&rtc);
    memcpy(data+2,&rtc,6);
    if(Dir == A2B_DIR)
    {
        data[0] = 0;
        data[1] = 0;
    }
    else if(Dir == B2A_DIR)
    {
        data[0] = 0;
        data[1] = 1;
    }
    else if(Dir == B2A_DIR)
    {
        data[0] = 0;
        data[1] = 2;
    }
    //memcpy(&data[8],&Temprature,4);
    result = order_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_END, &p_write,  0x55);  //按照4个字节查找 flash对齐 p_write偏移量
    bin_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_END, &p_read,  0x11);
    if(p_write >= p_read && result!=-1 && p_write < MA_READ_DATA)
    {
        if(Flash_Write_String((ADDR_DATA_FREEZE+p_write*FREEZE_DATA_SIZE/4+FREEZE_DATA_SIZE),(uint32_t *)data,FREEZE_DATA_SIZE/4) == 0)
            Flash_Write_String(ADDR_P_DATA_FREEZE + p_write + 4,&pdata,1);
        else   //写失败，擦除重写
        {
            Flash_Erase_Sector(ADDR_P_DATA_FREEZE/512);
            for(int i=0;i<ADDR_DATA_FREEZE_SIZE/512;i++)
                Flash_Erase_Sector(ADDR_DATA_FREEZE/512+i);
            Flash_Write_String((ADDR_DATA_FREEZE),(uint32_t *)data,FREEZE_DATA_SIZE/4);
            Flash_Write_String(ADDR_P_DATA_FREEZE,&pdata,1);
        }
        
        if(p_write >= MAX_WRITE_DATA  && p_write < MA_READ_DATA)  //差最后四条数据写满一个扇区   
        {
            if(DeviceParam.uptime.uptype != 0 )
                StartUp(Type_Ontime);
        }
    }
    else if(p_write == -4 && result == -1 && p_read<MAX_WRITE_DATA)
    {
        if(Flash_Write_String((ADDR_DATA_FREEZE+p_read*FREEZE_DATA_SIZE/4+FREEZE_DATA_SIZE),(uint32_t *)data,FREEZE_DATA_SIZE/4) == 0)
            Flash_Write_String(ADDR_P_DATA_FREEZE + p_read + 4,&pdata,1);
        else   //写失败，擦除重写
        {
            Flash_Erase_Sector(ADDR_P_DATA_FREEZE/512);
            for(int i=0;i<ADDR_DATA_FREEZE_SIZE/512;i++)
                Flash_Erase_Sector(ADDR_DATA_FREEZE/512+i);
            Flash_Write_String((ADDR_DATA_FREEZE),(uint32_t *)data,FREEZE_DATA_SIZE/4);
            Flash_Write_String(ADDR_P_DATA_FREEZE,&pdata,1);
        }
    }
    else if(p_write >= MA_READ_DATA)
    {
        if(DeviceParam.uptime.uptype != 0 )
            StartUp(Type_Ontime);   //上次没告成功，数据不能再写了
        return;
    }
    result = order_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_FREEZE+0x200, &p_write,  0x55);  //按照4个字节查找 flash对齐 p_write偏移量
    bin_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_FREEZE+0x200, &p_read,  0x11);
    print("p_read = %d  p_write = %d\r\n",p_read,p_write);
        
    
}


//写标志位
//0x55数据写标志   0x11数据读标志
void Write_Freeze_Pdata(uint32_t pdata)
{
    int p_write = -4;
 
    
    bin_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_FREEZE+0x200, &p_write,  pdata);
    if(p_write >= MA_READ_DATA && pdata == 0x55)  //写满一个扇区
    {
        return;
    }
    else
        Flash_Write_String(ADDR_P_DATA_FREEZE + p_write + 4,&pdata,1);
    
}


//偏移读指针位置 pdata 偏移多少个
void Set_Freeze_Pread(uint32_t pdata)   //置指针位置
{
    uint8_t i = 0;
    int P_read = -4;
    int P_write = -4;
    uint32_t data;
    short result = 0;
    
    
    bin_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_FREEZE+0x200, &P_read,  0x11);
    order_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_FREEZE+0x200, &P_write, 0x55);
    
    if(P_write - P_read <pdata)  //超过小于pdata个
    {
        pdata = P_write - P_read;
        if(pdata>MA_READ_DATA+20)
            return;
    }
    for(i = 1;i<=pdata;i++)
    {
        data =*(uint32_t *)(P_read+ADDR_P_DATA_FREEZE+4*i);    //获取值
        if(data != 0x55)
            break;
        else if(data == 0x55) 
            Write_Freeze_Pdata(0x11);
    }
    bin_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_FREEZE+0x200, &P_read,  0x11);         //再获取一遍确认是全部读取完毕
    result = order_search(ADDR_P_DATA_FREEZE, ADDR_P_DATA_FREEZE+0x200, &P_write, 0x55);
    if(P_read>=MAX_WRITE_DATA && result== -1)   //
    {   //全部读取完毕,清空
        Flash_Erase_Sector(ADDR_P_DATA_FREEZE/512);
        for(i=0;i<ADDR_DATA_FREEZE_SIZE/512;i++)
            Flash_Erase_Sector(ADDR_DATA_FREEZE/512+i);
    }
    
}

extern unsigned long mktime_BCD (RTC_Struct *Time);
short Get_Temprature_Pwrite(uint16_t *P_write,RTC_Struct *time)
{
    uint8_t DateStart[8] = {0};
    
    unsigned long time_now = 0;
    unsigned long time_start = 0;
    
    memcpy(DateStart,(uint32_t *)ADDR_TEMP_START,4);
    if(DateStart[0] == 0xff ||\
       DateStart[0] == 0xff ||\
       DateStart[0] == 0xff ||\
       DateStart[0] == 0xff    )
    {
        return -1;
    }
    
    time->Minute = 0;
    time->Second = 0;
    time_now = mktime_BCD(time); 
    time_start = mktime_BCD((RTC_Struct *)DateStart);
    time_now += 3600;     //往前移动一个小时，为了获取当前时间的数据
    
    if(time_now<time_start)
        return -1;
    *P_write = (time_now-time_start)/3600;
    if(*P_write>MAX_TEMP_DATA)
        *P_write = MAX_TEMP_DATA;   //超过七天没有读取 取可存储最大数
    return 0;
}



/*******************
*
*获取未读温度数据的条数
*参数 start  温度数据有效的起始时间
*******************/
uint16_t Check_Temprature_Data(unsigned long *start)
{
    RTC_Struct time;
    uint8_t DateStart[8] = {0};
    int p_read = -4;
    uint16_t hour_num = 0;
    
    unsigned long time_now = 0;
    unsigned long time_start = 0;
    
    memcpy(DateStart,(uint32_t *)ADDR_TEMP_START,4);
    if(DateStart[0] == 0xff ||\
       DateStart[0] == 0xff ||\
       DateStart[0] == 0xff ||\
       DateStart[0] == 0xff    )
    {
        return 0;
    }
    
    bin_search(ADDR_TEMP_DATA, ADDR_TEMP_END, &p_read,  0x00);
    RTC_GetTime(&time);
    
    time.Minute = 0;
    time.Second = 0;
    time_now = mktime_BCD(&time);    
    time_now += 3600;     //往前移动一个小时，为了获取当前时间的数据
    
    time_start = mktime_BCD((RTC_Struct *)DateStart);
    
    if(p_read>=0)
        time_start += (p_read+4)/4*3600;      //加上已读的
    if(p_read>MAX_TEMP_DATA*4)
    {        
        return 0;    
    }
    
    hour_num = (time_now - time_start)/3600;
    if(hour_num == 0 || time_now<time_start)     
        return 0;
    if(start != NULL)
        *start = time_start;
    return hour_num;
}


//偏移读指针位置 pdata 偏移多少个
/******************
功能：将已读的数据内容直接清零
参数：pdata清空的个数
******************/
void Set_Temprature_Pread(uint32_t pdata)   //置指针位置
{
    RTC_Struct time;
    uint8_t DateStart[8] = {0},i;
    int p_read = -4;
    uint16_t hour_num = 0;
    
    uint32_t dataread = 0xFF;
    
    unsigned long time_now = 0;
    unsigned long time_start = 0;
    
    memcpy(DateStart,(uint32_t *)ADDR_TEMP_START,4);
    if(DateStart[0] == 0xff ||\
       DateStart[0] == 0xff ||\
       DateStart[0] == 0xff ||\
       DateStart[0] == 0xff    )
    {
        return;
    }
    
    bin_search(ADDR_TEMP_DATA, ADDR_TEMP_END, &p_read,  0x00);
    RTC_GetTime(&time);
    
    time.Minute = 0;
    time.Second = 0;
    time_now = mktime_BCD(&time);     
    time_now += 3600;     //往前移动一个小时，为了清空当前时间的数据
    
    time_start = mktime_BCD((RTC_Struct *)DateStart);
    if(p_read>=0)
        time_start += (p_read+4)/4*3600;      //加上已读的
    
    
    hour_num = (time_now - time_start)/3600;   //未读还有多少  当前时间 - （开始时间+读取的时间）
    if( hour_num == 0)     
        return;   //错误
    if(hour_num>pdata)
        hour_num = pdata;
    for(i = 0;i<hour_num;i++)
    {
        if(Flash_Write_String(ADDR_TEMP_DATA+p_read +4+i*4,(uint32_t *)"\0\0\0\0",1))
        {
            Flash_Erase_Sector(ADDR_TEMP_DATE/512);
            Flash_Erase_Sector(ADDR_TEMP_DATE/512+1);
            return;
        }
    }
    
    bin_search(ADDR_TEMP_DATA, ADDR_TEMP_END, &p_read,  0x00);
    Get_Temprature_Pwrite(&hour_num,&time);
    memcpy(&dataread,(uint32_t *)(ADDR_TEMP_DATE+hour_num*4),4);
    if(p_read > (MAX_TEMP_DATA-4)*4 && (dataread == 0 || dataread == 0xffffffff))     //数据存满，且读完
    {
        Flash_Erase_Sector(ADDR_TEMP_DATE/512);
        Flash_Erase_Sector(ADDR_TEMP_DATE/512+1);
    }
        
}


void Save_Temprature(RTC_Struct *time)
{
    float Temprature;
    uint8_t tempbuf[4];
    uint16_t hour_num = 1;  
    uint8_t date[7] = {0};
    
    memcpy(date,(uint32_t *)(ADDR_TEMP_DATE),4);   //获取起始日期  记录格式为  20-09-25 14     
    if(Get_Temprature_Pwrite(&hour_num,time) == -1)      //时间不对数据全部擦除
    {
        Flash_Erase_Sector(ADDR_TEMP_DATE/512);
        Flash_Erase_Sector(ADDR_TEMP_DATE/512+1);   //两个扇区
        Flash_Write_String(ADDR_TEMP_DATE,(uint32_t *)time,1);   //写入日期
    }
     
    if(hour_num>MAX_TEMP_DATA)
        return;
    else if(hour_num<=MAX_TEMP_DATA && hour_num >=MAX_TEMP_DATA-4)
    {
        if(Up_process.UpState != S_Halt)
            SendDataToSevice(Type_UpTemp);
        else
            StartUp(Type_UpTemp);        
    }

    Temprature = Get_Temprature();
    memcpy(tempbuf,&Temprature,4);
    Flash_Write_String(ADDR_TEMP_DATA+(hour_num-1)*4,(uint32_t *)(tempbuf),1);
#ifdef __DEBUG
    Get_Temprature_Pwrite(&hour_num,time);
    print("温度%hu条\r\n",hour_num);
#endif


    
}





