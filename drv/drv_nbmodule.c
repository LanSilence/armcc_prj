#include "drv_nbmodule.h"
#include "bsp.h"
#include "uart.h"
#include "system_config.h"
#include "stdio.h"
#include "main.h"


DEVICE_POWER Device_Power;
NBSTATE NBState;
struct NBMODULE_Str NBDevice;
uint8_t ATsendBuf[AT_BUFF_MAX];
#ifdef TOUCHUAN

void touchuang_test(void)
{

//    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    print("ģ���ϵ�\n");
    Uartx_open(UART0);
    NB_Module_VBAT(POWERON);
    IO_NBMODE_POWER(0);
    IO_NBMODE_POWER_OUT;
      
   
//    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_9);
//    
//    GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
//    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
//    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//    GPIO_InitStruct.Pull = DISABLE;
//    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_9);
    DelayMs(1000);
    IO_NBMODE_POWER(1);
    print("ģ�鿪�����\n");
}

#endif

void NB_Module_VBAT(POWERONOFF power)
{
    IO_NBMODE_VBAT_OUT;
    IO_NBMODE_VBAT(power);
}

unsigned char Get_ATCMDFrom(ATCmdType AT_CMD,  char *CMD_Buf)
{
    unsigned char len = 0;
    switch(AT_CMD)
    {
    case  M_NONE:
        break;
    case  M_AT:
        len = sprintf(CMD_Buf,"AT\r\n");     
        break;
    case  M_ATE0:
        len = sprintf(CMD_Buf,"ATE0\r\n");
        break;
    case  M_ATI:    
        len = sprintf(CMD_Buf,"ATI\r\n");
        break;
    case M_CTZU:
        len = sprintf(CMD_Buf,"AT+CTZU=1\r\n");
        break;
    case  M_CGN:       //IMEI
        len = sprintf(CMD_Buf,"AT+GSN\r\n");
        break;
    case  M_CIMI:      //IMSI
        len = sprintf(CMD_Buf,"AT+CIMI\r\n");
        break;
    case  M_MICCID:    //ccid
        len = sprintf(CMD_Buf,"AT*MICCID\r\n");
        break;
    case  M_UN_COPS:   //ע������
        len = sprintf(CMD_Buf,"AT\r\n");
        break;
    case  M_COPS:
        len = sprintf(CMD_Buf,"AT+COPS=4,2,46011,9\r\n");
        break;
    case  M_EGACT:    //pdp������
        len = sprintf(CMD_Buf,"AT+EGACT=1,1,\"CTNB\"\r\n");
        break;
    case  M_CSQ:    
        len = sprintf(CMD_Buf,"AT+CSQ\r\n");
        break;
    case M_CESQ:
        len = sprintf(CMD_Buf,"AT+CESQ\r\n");
        break;
    case  M_CEREG:
        len = sprintf(CMD_Buf,"AT+CEREG?\r\n");
        break;
    case  M_CCLK:
        len = sprintf(CMD_Buf,"AT+CCLK?\r\n");
        break;
#if TRAN_PRO == MQTT_PRO    
    case M_EMQNEW:          //����mqtt������
        //len = sprintf(CMD_Buf,"AT+EMQNEW=\"%hhu.%hhu.%hhu.%hhu\",\"%hu\",1000,%u\r\n",DeviceParam.ip[0],DeviceParam.ip[1],\
                        DeviceParam.ip[2],DeviceParam.ip[3],DeviceParam.port,USART_BUFF_SIZE);
        len = sprintf(CMD_Buf,"AT+EMQNEW=\"%s\",\"%hu\",1000,%u\r\n",DeviceParam.ip,DeviceParam.port,USART_BUFF_SIZE*2);
        break;
    case M_EMQCON:          //�����豸
        len = sprintf(CMD_Buf,"AT+EMQCON=%hhu,4,\"%s\",1000,1,0,\"%s\",\"%s\"\r\n",Up_process.Mqtt_Info.id,Up_process.Mqtt_Info.client_id,Up_process.Mqtt_Info.user_id,Up_process.Mqtt_Info.password);
        break;
    case M_EMQSUB:          //����
        len = sprintf(CMD_Buf,"AT+EMQSUB=0,\"device_control\",1\r\n");
        break;
    case M_EMQPUB:          //����
        len = sprintf(CMD_Buf,"AT+EMQPUB=0,\"device_control\",1,0,0,%d,",Up_process.TxLength*2+10);   //�������Ϣ�����ٷ���ATָ�����ϣ���Сָ�����
        break;
    case M_EMQUNSUB:        //ȡ������
        len = sprintf(CMD_Buf,"AT+EMQUNSUB=0,\"mytopic\"");
        break;
    case M_EMQDISCON:       //�Ͽ�����
        len = sprintf(CMD_Buf,"AT+EMQDISCON=0\r\n");
        break;
#elif TRAN_PRO == LWM2M_PRO            
    case M_M2MCLINEW:    //ע��
        len = sprintf(CMD_Buf,"AT+M2MCLINEW=%s,%hu,\"\",300\r\n",DeviceParam.ip,DeviceParam.port);
        break;
    case M_M2MCLIDEL:    //ȥע��
        len = sprintf(CMD_Buf,"AT+M2MCLIDEL\r\n");
        break;
    case M_M2MCLISEND:   //���ݷ���
        len = sprintf(CMD_Buf,"AT+M2MCLISEND=");
        break;
    case M_M2MCLICFG:    //������������
        len = sprintf(CMD_Buf,"AT+M2MCLICFG=0,0\r\n");
        break;
    case M_M2MCLIREADEN: //���ý���ģʽ    0 ֱ�Ӵ������� 1 ���뻺�棬�ȴ���ȡ
        len = sprintf(CMD_Buf,"AT+M2MCLIREADEN=0\r\n");
        break;
    case M_M2MCLIREAD:   //��ȡ��������
        break;
#endif    
#ifdef HTTPS_UP                                      //AT+EHTTPCREATE=0,41,41,""http://219.144.130.26:61613/",,,0,,0,,0,"
    case M_EHTTPCREATE:  //����http�ͻ���              AT+EHTTPCREATE=0,39,39,""http://124.70.213.41:8080/",,,0,,0,,0,"
        len = sprintf(CMD_Buf,"AT+EHTTPCREATE=0,%d,%d,\"\"%s\",,,0,,0,,0,\"\r\n",strlen((char *)HttpInfo.http_addr), \
                      strlen((char *)HttpInfo.http_addr),HttpInfo.http_addr);            //http://124.70.213.41:8080/zhongkongFiles/Tester.bin
        break;
    case M_EHTTPCON:     //����http����
        len = sprintf(CMD_Buf,"AT+EHTTPCON=%hhu\r\n",HttpInfo.HttpClient);
        break;
    case M_EHTTPSEND:    //��������  AT+EHTTPSEND=0,56,56,"0,0,37,"/zhongkongFiles/Tester.bin",0,,0,,0,,"
        {
            unsigned short sendlen = 0;
            sendlen = strlen((char *)HttpInfo.FileDir);
            if(sendlen>=10)
                sendlen += 19;
            else
                sendlen += 18;
            len = sprintf(CMD_Buf,"AT+EHTTPSEND=0,%hu,%hu,\"0,0,%d,\"%s\",0,,0,,0,,\"\r\n",sendlen,sendlen, \
                          strlen((char *)HttpInfo.FileDir),HttpInfo.FileDir);
        }
        break;
    case M_EHTTPDISCON:  //�ر�����
        len = sprintf(CMD_Buf,"AT+EHTTPDISCON=%hhu",HttpInfo.HttpClient);
        break;
    case M_EHTTPDESTROY: //�ͷ�����
        len = sprintf(CMD_Buf,"AT+EHTTPDESTROY=%hhu",HttpInfo.HttpClient);
        break;
#endif  
    
#ifdef FTP_UPDATA    
    case M_ZFTPOPEN:        //FTP���� AT+ZFTPOPEN="219.144.130.27",21,"test","test",0
        len = sprintf(CMD_Buf,"AT+ZFTPOPEN=\"%hhu.%hhu.%hhu.%hhu\",%hu,\"%s\",\"%s\",0\r\n", \
                      FtpInfo.ip[0],FtpInfo.ip[1],FtpInfo.ip[2],FtpInfo.ip[3],FtpInfo.port,FtpInfo.username,FtpInfo.password);
        break;
    case M_ZFTPSIZE:        //��ȡ�ļ���С  AT+ZFTPSIZE="/zwlog/123.txt"
        len = sprintf(CMD_Buf,"AT+ZFTPSIZE=\"%s\"\r\n",(char *)FtpInfo.FileDir);
        break;
    case M_ZFTPGET:         //�����ļ�  AT+ZFTPGET="zk.txt",0,20
        len = sprintf(CMD_Buf,"AT+ZFTPGET=\"%s\",%u,200\r\n",FtpInfo.FileDir,FtpInfo.RecvSize);
        break;
    case M_ZFTPCLOSE:       //�ر�FTP
        len = sprintf(CMD_Buf,"AT+ZFTPCLOSE\r\n");
        break;
#endif    
    case  M_MIPLCREATE:     //�ݲ�֧��ͨ��NBiotЭ������onenet  ����δ��
    case  M_MIPLADDOBJ:
    case  M_MIPLOPEN:
    case  M_MIPLNOTIFY:
    case  M_MIPLWRITERSP:
    case  M_MIPCLOSE:
        break;
    case  M_ZRST:
        len = sprintf(CMD_Buf,"AT+ZRST\r\n");
        break;
    default:
        break;
    }
    return len;
}
void Module_Power_manage()
{
    unsigned char len = 0;
        char CMD[8];
    static unsigned char ATSendTimes = 0;
    
    if(Device_Power.Tim_delay > 0)
        Device_Power.Tim_delay--;
    
    switch(Device_Power.power_state)
    {
    case NB_IDLE:   
           Device_Power.power_state =  NB_OPEN_VBAT;
        break;
    case NB_OPEN_VBAT:
        Uartx_open(UART0);   //���ڿ���Ӧ�÷������ϸ��������ĵط� ���������
        NB_Module_VBAT(POWERON);
        Device_Power.PowerOnTimes ++;
        print("*ģ���ϵ�*\n");
        Device_Power.power_state = NB_WAIT_VBAT;
        Device_Power.Tim_delay = 20;//�����ϵ�
        break;
    case NB_WAIT_VBAT:
        if(Device_Power.Tim_delay == 0)
        {
            print("*ģ�����ϵ�*\n");
            Device_Power.power_state = NB_WAIT_ON;
            IO_NBMODE_POWER(0);
            IO_NBMODE_POWER_OUT;
            IO_NBMODE_POWER(1);
            Device_Power.Tim_delay = 10;   //�ȴ�1�뿪�� 
            Device_Power.PowerOnTimes++;         
        }
        break;
    case NB_POWER_ON:
        break;
    case NB_WAIT_ON:
        if(Device_Power.Tim_delay == 0)
        {
            IO_NBMODE_POWER(0);
            Device_Power.power_state = NB_DETECT_ON;
        }
        break;
    case NB_DETECT_ON:
        //����ָ��AT
        if(ATSendTimes < 5)
        {            
            len = Get_ATCMDFrom(M_AT,CMD);
            ATSendTimes++;
            Uart0SendData((unsigned char *)CMD,len);
            Device_Power.power_state = NB_CHECK_ON;
            Device_Power.Tim_delay = 20;
        }
        else
        {
            
            if(Device_Power.PowerOnTimes >= 5)
            {
                Up_process.IncidentPend = 0;   //�ص��ϸ����̴���ػ�
                Up_process.UpState = S_UpFail;
                NBState.state = 3;   //0 ���� ,1���ڿ���, 2�����ɹ���3����ʧ��
                print("**������������---�ػ�**\n");
            }
            else
            {
                NB_Module_VBAT(POWEROFF);   //�رյ�Դ
                Device_Power.power_state = NB_OPEN_VBAT;
                print("**ʧ��**\r\n**���¿���***\r\n");
            }
        }
        break;
    case NB_CHECK_ON:
        if(Device_Power.Tim_delay)
        {
            //����Ƿ��յ�OK
             if(RecvDataFromBuff(&NB_Recv_Buff,UartBuff) == AT_Type)
             {
                 if (memcmp(UartBuff,"OK",2) == 0)
                 {
                     print("*ģ�鿪��*\r\n");
                     Device_Power.power_state = NB_IS_ON;
                     NBDevice.Process = M_ATE0;
                     task_start(typeMs,100,ATCmd_Process);
                 }
             }
        }
        else
        {
            Device_Power.power_state = NB_OPEN_VBAT;
        }
        break;
    case NB_IS_ON:
        
        break;     
    case NB_POWER_OFF:
        IO_NBMODE_POWER(1);
        Device_Power.Tim_delay = 50; //5���ػ����
        Device_Power.power_state = NB_VBAT_DOWN;
        print("*���ڹػ�*\r\n");
        break;
    case NB_VBAT_DOWN:
        if(Device_Power.Tim_delay == 0)
        {
            IO_NBMODE_POWER(0);
  
            Device_Power.Tim_delay = 10; //1����µ�
            Device_Power.power_state = NB_WAIT_OFF;
        }
        break;
    case NB_WAIT_OFF:         
        if(Device_Power.Tim_delay == 0)
        {   
            print("*ģ��ػ�*\r\n");
            Device_Power.power_state = NB_IDLE;
            NB_Module_VBAT(POWEROFF);
            task_stop(Module_Power_manage);
            Device_Power.PowerOnTimes = 0;
            ATSendTimes = 0;
            Uartx_close(UART0);
        }
        break;
    default:
        break;
    }
}

unsigned char Get_CSQ(unsigned char *Pin)
{
    int csq = 0;
    csq = Pin[6] -0x30;
    if(Pin[7] == ',')
        return csq;
    else
        return (unsigned char)(csq*10+Pin[7]-0x30);
}

//��ʱ֮���������״̬
void clear_allstate(void)
{
    NBDevice.CSQ = 0;
    NBDevice.Process = M_NONE;
#if TRAN_PRO == MQTT_PRO     
    Up_process.Mqtt_Info.connet = 0;
#elif TRAN_PRO == LWM2M_PRO  
    Up_process.Lwm2m_Info.Reg_state = 0;
#endif
    
    Up_process.UpState = S_Halt;
    Up_process.IncidentPend = 1;
}
//�ϸ�ʱ�䳬ʱ  10����  
void NBDeviceOverTime() 
{
    Device_Power.power_state = NB_POWER_OFF;
    clear_allstate();
    task_stop(NBDeviceOverTime);
    task_stop(ATCmd_Process);
    task_stop(DeviceUpComing_Process);
}    


//ATָ�����̴���
void ATCmd_Process(void)
{
    unsigned char len;
    if(NBDevice.SendCmd_Pend == 0)
    {
        len = Get_ATCMDFrom(NBDevice.Process, (char *)ATsendBuf);
        if(len == 0)
            NBDevice.SendCmd_Pend = 1;
        Uart1SendData(ATsendBuf,len);
        Uart0SendData(ATsendBuf,len);
        switch(NBDevice.Process)
        {
        case M_NONE:
            break;
        case M_AT:
        case M_ATE0:
        case M_ATI: 
        case M_CTZU:
        case M_CCLK:
        case M_CGN:       //IMEI��
        case M_CIMI:      //IMSI  
        case M_MICCID:            
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,3,AT_Proce_Overtime);
            break;
        case M_UN_COPS:   //ע������
            break;
        case M_COPS:
        case M_EGACT:    //pdp������
        case M_CESQ:
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,3,AT_Proce_Overtime);
            break;
        case M_CSQ:    
        case M_CEREG:
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 60;
            task_reset(type1000Ms,3,AT_Proce_Overtime);
            break;
#if TRAN_PRO == MQTT_PRO
        case M_EMQNEW:     //����mqtt������ 
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 1;
            task_reset(type1000Ms,60,AT_Proce_Overtime);
            break;
        case M_EMQCON:       //�����豸
        case M_EMQSUB:       //����
        case M_EMQUNSUB:     //ȡ������
        case M_EMQDISCON:    //�Ͽ�����
            
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,5,AT_Proce_Overtime);
            break;
        case M_EMQPUB:       //����
            if(Up_process.TxLength)  //��������Ϊ��
            {

                Uart1SendData((unsigned char *)"\"",1);
                Uart1SendData((unsigned char *)"7B226D7367223A22",16);   //json��Ϣǰ׺{"msg":"
                
                
                HexToAssic_Uart1Send(Up_process.TxBuffer,Up_process.TxLength);
                Uart1SendData((unsigned char *)"227D",4);               //json��Ϣ��׺"}
                Uart1SendData((unsigned char *)"\"\r\n",3);
                
                
                Uart0SendData((unsigned char *)"\"",1);
                Uart0SendData((unsigned char *)"7B226D7367223A22",16);
                HexToAssic_Uart0Send(Up_process.TxBuffer,Up_process.TxLength);
                Uart0SendData((unsigned char *)"227D",4);
                Uart0SendData((unsigned char *)"\"\r\n",3);
                
            }
            //Up_process.IncidentPend = 0;  //�ϸ�������һ���ȴ���Ϣ
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,3,AT_Proce_Overtime);
            break;
#elif TRAN_PRO == LWM2M_PRO            
        
        case M_M2MCLISEND:   //���ݷ���
            if(Up_process.TxLength)  //��������Ϊ��
            {
                HexToAssic_Uart1Send(Up_process.TxBuffer,Up_process.TxLength);
                Uart1SendData((unsigned char *)",0",2);
                Uart1SendData((unsigned char *)"\r\n",2);
                HexToAssic_Uart0Send(Up_process.TxBuffer,Up_process.TxLength);   
                Uart0SendData((unsigned char *)",0",2);                
                Uart0SendData((unsigned char *)"\r\n",2);
            }
            //Up_process.IncidentPend = 0;  //�ϸ�������һ���ȴ���Ϣ
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,10,AT_Proce_Overtime);
            break;
        case M_M2MCLINEW:    //ע��
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,10,AT_Proce_Overtime);
            break;
        case M_M2MCLIDEL:    //ȥע��
        case M_M2MCLICFG:    //������������
        case M_M2MCLI:       //�����ϱ�
        case M_M2MCLIREADEN: //���ý���ģʽ    0 ֱ�Ӵ������� 1 ���뻺�棬�ȴ���ȡ
        case M_M2MCLIDATA:   //���ճ����ϱ�
        case M_M2MCLIREAD:   //��ȡ��������
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,5,AT_Proce_Overtime);
           break;
#endif
#ifdef HTTPS_UP
        case M_EHTTPCREATE:  //����http�ͻ���              AT+EHTTPCREATE=0,42,42,""https://219.144.130.26:61614/",,,0,,0,,0,"
            break;
        case M_EHTTPCON:     //����http����
            break;
        case M_EHTTPSEND:    //��������
            break;
        case M_EHTTPDISCON:  //�ر�����
            break;
        case M_EHTTPDESTROY: //�ͷ�����
            break;
#else 
        case M_ZFTPOPEN:     //FTP����
        case M_ZFTPSIZE:     //��ȡ�ļ���С
        case M_ZFTPCLOSE:    //�ر�FTP
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,3,AT_Proce_Overtime);
            break;
        case M_ZFTPGET:      //�����ļ�
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,3,AT_Proce_Overtime);
            break;
#endif        
        case M_ZRST:
            NBDevice.SendCmd_Pend = 1;  //������ָ������
            NBDevice.SendTimes ++;
            NBDevice.SendTimesMax = 5;
            task_reset(type1000Ms,5,AT_Proce_Overtime);
            break;
        default:
            break;
        }
    }
    
   if(RecvDataFromBuff(&NB_Recv_Buff,UartBuff) == AT_Type)
   {
       if(memcmp(UartBuff,"+M2MCLIRECV:",12) == 0)
       {
           //��ȡ����
           uint8_t cmd[400] = {0};
           int i;
           uint8_t temp[2];
           sscanf((char *)&UartBuff[12],"%s",cmd);
           for(i=0;i<=strlen((char *)UartBuff);i++)
           {
               memcpy(temp,cmd+i*2,2);
               sscanf((char *)temp,"%02hhX",cmd+i);
           }
           Process_Cmd(cmd,NBMODE_COM);     //Э����� 
           Up_process.IncidentPend = 0;   //�ص��ϸ����̴�������
           Up_process.UpState = S_Connet;
       }
       switch(NBDevice.Process)
       {
#if TRAN_PRO == MQTT_PRO     
           case M_NONE:
           if(memcmp(UartBuff,"+EMQPUB:",8) == 0)
           {
               uint8_t cmd[200] = {0};
               sscanf((char *)UartBuff,"%*s %*s %*s %*s %*s %*s %*s \"%[^\"]",cmd);
               
               //Protocal_Analyze(cmd);
               //Up_process.IncidentPend = 0;   //�ص��ϸ����̴�������
               //Protocol_Analayse(cmd);  //Э�����
               //Up_process.UpState = S_Connet;
           }
           if(memcmp(UartBuff,"ERROR",5) == 0)
           {
               //Up_process.IncidentPend = 0;
           }
           if(memcmp(UartBuff,"+EMQDISCON: 0",13) == 0)
           {
               Up_process.Mqtt_Info.connet = 0;
           }
           break;
#elif TRAN_PRO == LWM2M_PRO  
    
#endif
       
       case M_AT:
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_ATE0;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_ATE0:
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_ATI;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_ATI:  
           if (memcmp(UartBuff,"OK",2) == 0)
           {
                NBDevice.Process = M_CTZU;  
                NBDevice.SendCmd_Pend = 0;    
                NBDevice.SendTimes = 0;       
                task_stop(AT_Proce_Overtime);  
           }               
           break;
       case M_CTZU:
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_CGN;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
       case M_CGN:       //IMEI
           if(memcmp(UartBuff,"86",2) == 0)
           {
               memcpy(NBDevice.IMEI,UartBuff,15);
               //memcpy(DeviceParam.id,NBDevice.IMEI,15);
           }
           if (memcmp(UartBuff,"OK",2) == 0 && memcmp(NBDevice.IMEI,"86",2) == 0)
           {
               NBDevice.Process = M_CIMI;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               if(memcmp(DeviceParam.id,NBDevice.IMEI,15) && (memcmp(NBDevice.IMEI,"86",2) ==0))
               {
                   //�豸��δ���ã�IMEI��ȷ  ���Ʋ��洢
                   memcpy(DeviceParam.id,NBDevice.IMEI,15);
                   Save_Parame();
               }
               
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_CIMI:      //IMSI
           if(memcmp(UartBuff,"46",2) == 0)
           {
               memcpy(NBDevice.IMSI,UartBuff,15);
           }
           if (memcmp(UartBuff,"OK",2) == 0 && memcmp(NBDevice.IMSI,"46",2) == 0)
           {
               NBDevice.Process = M_MICCID;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_MICCID:
           if(memcmp(UartBuff,"*MICCID",7) == 0)
           {
               memcpy(NBDevice.CCID,&UartBuff[9],20);
           }
           if (memcmp(UartBuff,"OK",2) == 0 && memcmp(NBDevice.CCID,"898",3) == 0)
           {
               NBDevice.Process = M_CSQ;
               NBDevice.SendTimes = 0;
               NBDevice.SendCmd_Pend = 0;
           }
           break;
       case M_UN_COPS:   //ע������
           
           break;
       case M_CSQ:   
           if(memcmp(UartBuff,"+CSQ",4) == 0)  
           {
               NBDevice.CSQ = Get_CSQ(UartBuff);              
           }
            if(memcmp(UartBuff,"OK",2) == 0)
            {
                if(NBDevice.CSQ > 0 && NBDevice.CSQ <=31)
               {
                   NBDevice.Process = M_CESQ;
                   NBDevice.SendCmd_Pend = 0;
                   NBDevice.SendTimes = 0;
                   task_stop(AT_Proce_Overtime);
               }
            }
           break;
       case M_CESQ:
           if(memcmp(UartBuff,"+CESQ:",6) == 0)
           {
               NBDevice.SendCmd_Pend = 1;
           }
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_CEREG;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_CEREG:   //+CEREG: 0,1
           if(memcmp(UartBuff,"+CEREG:",7) == 0)
           {
               Up_process.NetState = UartBuff[10] -0x30;
           }
           
           if ((Up_process.NetState == 1)&&(memcmp(UartBuff,"OK",2) == 0))
           {
               NBDevice.Process = M_EGACT;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_COPS:
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_EGACT;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_EGACT:    //pdp������
           if(memcmp(UartBuff,"+EGACT:",7)==0)
           {
               NBDevice.EGACT_Count++;
               if(NBDevice.EGACT_Count>=2)
               {
                    NBDevice.Process = M_CCLK;
                    NBDevice.SendCmd_Pend = 0;
                    NBDevice.SendTimes = 0;
                    NBDevice.EGACT_Count = 0;
                    task_stop(AT_Proce_Overtime);
               }
           }
           break;
       case M_CCLK:
           if (memcmp(UartBuff,"+CCLK",5) == 0)
           {
               extern void Get_UTC_8Time(RTC_Struct *Time);
               uint8_t time[10];
               sscanf((char *)&UartBuff[8],"%02hhx/%02hhx/%02hhx,%02hhx:%02hhx:%02hhx",&time[0],&time[1],&time[2],&time[3],&time[4],&time[5]);
               Get_UTC_8Time((RTC_Struct*)time);//��ȡ����ʱ��
               if(RTC_SetTime((RTC_Struct*)time))
               {
#if TRAN_PRO == MQTT_PRO                    
                NBDevice.Process = M_EMQNEW;
#elif TRAN_PRO == LWM2M_PRO                
                NBDevice.Process = M_M2MCLINEW;
#endif                   
                NBDevice.SendCmd_Pend = 0;
                NBDevice.SendTimes = 0;
                task_stop(AT_Proce_Overtime);
               }
           }
           break;
#if TRAN_PRO == MQTT_PRO           
       case M_EMQNEW:    //����mqtt������
           if(memcmp(UartBuff,"+EMQNEW",7) == 0)
           {
               Up_process.Mqtt_Info.id = UartBuff[9]-0x30;
               Up_process.Mqtt_Info.connet = 1;
           }
           
           if (memcmp(UartBuff,"OK",2) == 0 && Up_process.Mqtt_Info.connet == 1)
           {
               NBDevice.Process = M_EMQCON;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_EMQCON:       //�����豸
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               Up_process.Mqtt_Info.connet = 1;
               NBDevice.Process = M_EMQSUB;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;               
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_EMQSUB:       //����
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               //NBDevice.Process = M_NONE;
               NBDevice.SendCmd_Pend = 1;    //���𣬵ȴ�������Ϣ��ʱ������
               NBDevice.SendTimes = 0;
               Up_process.TxLength = 0;
               task_reset(type1000Ms,5, AT_Proce_Overtime);   //5���û����Ӧ��ֱ�ӷ�����Ϣ
               
           }
           if(memcmp(UartBuff,"+EMQPUB:",8) == 0)
           {
               uint8_t cmd[400] = {0};
               int i;
               uint8_t temp[2];
               sscanf((char *)UartBuff,"%*s %*s %*s %*s %*s %*s %*s \"%[^\"]",cmd);
               for(i=0;i<=200;i++)
               {
                   memcpy(temp,cmd+i*2,2);
                   sscanf((char *)temp,"%02hhX",cmd+i);
               }
               for(i=0;i<=100;i++)
               {
                   memcpy(temp,cmd+8+i*2,2);
                   sscanf((char *)temp,"%02hhX",cmd+i);
               }
               Process_Cmd(cmd,NBMODE_COM);     //Э����� 
               Up_process.IncidentPend = 0;   //�ص��ϸ����̴�������
               Up_process.UpState = S_Connet;
           }
           break;
       case M_EMQPUB:       //����
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_NONE;
               NBDevice.SendCmd_Pend = 1;    //����
               NBDevice.SendTimes = 0;
               Up_process.IncidentPend = 0;  //�ϸ����̼�������
               Up_process.TxLength = 0;      //���ݷ�����ɻ��������
               switch(Up_process.CurrentTask)
               {
               case Type_Poweron:
                   Up_process.Up_Task &= ~(1U << Type_Poweron);
                   Set_Freeze_Pread(15);         //�������ƫ�ƶ�ָ��λ��
                   break;
               case Type_Ontime:
                   Up_process.Up_Task &= ~(1U << Type_Ontime);
                   Set_Freeze_Pread(15); 
                   break;
               case Type_UpTemp:
                   Up_process.Up_Task &= ~(1U <<Type_UpTemp);
                   Set_Temprature_Pread(25);
                   break;
               case Type_Realtime:
                   Up_process.Up_Task &= ~(1U << Type_Realtime);
                   Set_Freeze_Pread(15); 
                   break;
               case Type_Alarm:
                   Up_process.Up_Task &= ~(1U << Type_Alarm);
                   break;
               }
                Up_process.CurrentTask = 0;               
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_EMQUNSUB:    //ȡ������
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_EMQDISCON;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_EMQDISCON:    //�Ͽ�����
           if (memcmp(UartBuff,"OK",2) == 0 || memcmp(UartBuff,"ERROR",5) == 0)
           {
               NBDevice.Process = M_NONE;
               //NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               Up_process.IncidentPend = 0;
               Up_process.Mqtt_Info.connet = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
#elif TRAN_PRO == LWM2M_PRO  
       case M_M2MCLINEW:    //ע��
           if(memcmp(UartBuff,"OK",2) == 0)
           {
               Up_process.Lwm2m_Info.Reg_state = RECVOK;
           }
           if(memcmp(UartBuff,"+M2MCLI:register success",24) == 0 && Up_process.Lwm2m_Info.Reg_state == RECVOK)
           {
               
               Up_process.Lwm2m_Info.Reg_state = RESSUC;
           }
           if(memcmp(UartBuff,"+M2MCLI:observe success",23) == 0 && Up_process.Lwm2m_Info.Reg_state == RESSUC)
           {
               Up_process.Lwm2m_Info.Reg_state = OBSSUC;
               NBDevice.Process = M_M2MCLICFG;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_M2MCLIDEL:    //ȥע��
           if(memcmp(UartBuff,"+M2MCLI:deregister success",26) == 0)
           {
               Up_process.Lwm2m_Info.Reg_state = 0;
               //Up_process.IncidentPend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_M2MCLICFG:    //������������
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_NONE;
               NBDevice.SendCmd_Pend = 1;    //����
               NBDevice.SendTimes = 0;
               Up_process.IncidentPend = 0;  //�ϸ����̼�������
           }
           break;
       case M_M2MCLISEND:   //���ݷ���
           if(memcmp(UartBuff,"+M2MCLI:notify success",22) == 0)
           {
               NBDevice.Process = M_NONE;
               NBDevice.SendCmd_Pend = 1;    //����
               NBDevice.SendTimes = 0;
               Up_process.IncidentPend = 0;  //�ϸ����̼�������
               Up_process.TxLength = 0;      //���ݷ�����ɻ��������
               switch(Up_process.CurrentTask)
               {
               case Type_Poweron:
                   Up_process.Up_Task &= ~(1U << Type_Poweron);
                   Set_Freeze_Pread(15);         //�������ƫ�ƶ�ָ��λ��
                   break;
               case Type_Ontime:
                   Up_process.Up_Task &= ~(1U << Type_Ontime);
                   Set_Freeze_Pread(15); 
                   break;
               case Type_UpTemp:
                   Up_process.Up_Task &= ~(1U <<Type_UpTemp);
                   Set_Temprature_Pread(25);
                   break;
               case Type_Realtime:
                   Up_process.Up_Task &= ~(1U << Type_Realtime);
                   Set_Freeze_Pread(15); 
                   break;
               case Type_Alarm:
                   Up_process.Up_Task &= ~(1U << Type_Alarm);
                   break;
               }
                Up_process.CurrentTask = 0;               
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_M2MCLIREADEN: //���ý���ģʽ    0 ֱ�Ӵ������� 1 ���뻺�棬�ȴ���ȡ
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_NONE;
               NBDevice.SendCmd_Pend = 1;    //����
               NBDevice.SendTimes = 0;
               Up_process.IncidentPend = 0;  //�ϸ����̼�������
           }
           break;
       case M_M2MCLIREAD:   //��ȡ��������
           if(memcmp(UartBuff,"+M2MCLIRECV:",12) == 0)
           {
               //��ȡ����
               uint8_t cmd[400] = {0};
               int i;
               uint8_t temp[2];
               sscanf((char *)&UartBuff[12],"%s",cmd);
               for(i=0;i<=200;i++)
               {
                   memcpy(temp,cmd+i*2,2);
                   sscanf((char *)temp,"%02hhX",cmd+i);
               }
               Process_Cmd(cmd,NBMODE_COM);     //Э����� 
               Up_process.IncidentPend = 0;   //�ص��ϸ����̴�������
               Up_process.UpState = S_Connet;
           }
           break;
       
       /*
       case M_M2MCLINEW:    //ע��
       case M_M2MCLIDEL:    //ȥע��
       case M_M2MCLISEND:   //���ݷ���
       case M_M2MCLICFG:    //������������
       case M_M2MCLI:       //�����ϱ�
       case M_M2MCLIREADEN: //���ý���ģʽ    0 ֱ�Ӵ������� 1 ���뻺�棬�ȴ���ȡ
       case M_M2MCLIDATA:   //���ճ����ϱ�
       case M_M2MCLIREAD:   //��ȡ��������
           break;
       */
#endif
#ifdef HTTPS_UP
        case M_EHTTPCREATE:  //����http�ͻ���              AT+EHTTPCREATE=0,42,42,""https://219.144.130.26:61614/",,,0,,0,,0,"
            if (memcmp(UartBuff,"+EHTTPCREAT:",12) == 0)
            {
                HttpInfo.HttpClient = UartBuff[12]-0x30;
            }
            if(HttpInfo.HttpClient != 0xff && memcmp(UartBuff,"OK",2) == 0)
            {
               NBDevice.Process = M_EHTTPCON;
               NBDevice.SendCmd_Pend = 1;    //����
               NBDevice.SendTimes = 0;
               Up_process.TxLength = 0;
               task_stop(AT_Proce_Overtime);
            }
            break;
        case M_EHTTPCON:     //����http����
            if(memcmp(UartBuff,"OK",2) == 0)
            {
               NBDevice.Process = M_EHTTPSEND;
               NBDevice.SendCmd_Pend = 1;    //����
               NBDevice.SendTimes = 0;
               Up_process.TxLength = 0;
               task_stop(AT_Proce_Overtime);
            }
            break;
        case M_EHTTPSEND:    //��������
            if(memcmp(UartBuff,"Content-Length:",15))
            {
                sscanf((char *)UartBuff,"%*s:%u",&HttpInfo.FileSize);
                
            }
            break;
        case M_EHTTPDISCON:  //�ر�����
            if(memcmp(UartBuff,"OK",2) == 0)
            {
               NBDevice.Process = M_EHTTPDESTROY;
               NBDevice.SendCmd_Pend = 1;    //����
               NBDevice.SendTimes = 0;
               Up_process.TxLength = 0;
               task_stop(AT_Proce_Overtime);
            }
            break;
        case M_EHTTPDESTROY: //�ͷ�����
            if(memcmp(UartBuff,"OK",2) == 0)
            {
               NBDevice.Process = M_NONE;
               NBDevice.SendCmd_Pend = 1;    //����
               NBDevice.SendTimes = 0;
               Up_process.TxLength = 0;
               task_stop(AT_Proce_Overtime);
            }
            break;
#endif 
#ifdef FTP_UPDATA        
       case M_ZFTPOPEN:     //FTP����
           if (memcmp(UartBuff,"OK",2) == 0 )
           {
               NBDevice.Process = M_ZFTPSIZE;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           break;
       case M_ZFTPSIZE:     //��ȡ�ļ���С
           if(UartBuff[0]>=0x30&&UartBuff[0]<=0x39)
           {
               sscanf((char *)UartBuff,"%u",&FtpInfo.FileSize);
           }
           if(memcmp(UartBuff,"OK",2) == 0 && FtpInfo.FileSize>1000)
           {
               NBDevice.Process = M_ZFTPGET;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
       case M_ZFTPGET:      //�����ļ�
            if(memcmp(UartBuff,"CONNECT",7) == 0)
            {
                FtpInfo.FtpRunning = 1;
            }            
            task_reset(type1000Ms,300,AT_Proce_Overtime);
            break;
       case M_ZFTPCLOSE:    //�ر�FTP
           break;
#endif           
       case M_MIPLCREATE:
           break;
       case M_MIPLADDOBJ:
           break;
       case M_MIPLOPEN:
           break;
       case M_MIPLNOTIFY:
           break;
       case M_MIPLWRITERSP:
           break;
       case M_MIPCLOSE:
           break;
       case M_ZRST:
           if (memcmp(UartBuff,"OK",2) == 0)
           {
               NBDevice.Process = M_AT;
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
           else if (memcmp(UartBuff,"ERROR",5) == 0)
           {
#if TRAN_PRO == MQTT_PRO                    
               NBDevice.Process = M_EMQDISCON;
#elif TRAN_PRO == LWM2M_PRO                
               NBDevice.Process = M_M2MCLIDEL;
#endif  
               NBDevice.SendCmd_Pend = 0;
               NBDevice.SendTimes = 0;
               task_stop(AT_Proce_Overtime);
           }
       default:
           break;
           
       }
       
   }
#ifdef FTP_UPDATA   
   if(FtpInfo.FtpRunning)
   {
       Ftp_FileRecv();//�����ļ�
   }
#endif
}

void AT_Proce_Overtime(void)
{
    NBDevice.SendCmd_Pend =0; 
    if(NBDevice.Process ==  M_ZRST)  //��λָ�ʱ��ɹ�   ��λָ��ظ�OK���治��0d0a�޷���ȷȡ��ok
    {
        NBDevice.Process = M_AT;
        NBDevice.SendTimes = 0;
    }
    if(NBDevice.SendTimes >= NBDevice.SendTimesMax)
    {
        
        if(NBDevice.RebootTimes > 1)
        {
            //task_stop(ATCmd_Process);            
            Up_process.UpState = S_UpFail;
            Up_process.IncidentPend = 0;
            NBDevice.SendCmd_Pend =1; 
        }
        else
        {
            switch(NBDevice.Process)
            {
            case M_ATE0:
            case M_ATI:   
            case M_CTZU:         //�Զ���ȡʱ��
            case M_CGN:          //IMEI
            case M_CIMI:         //IMSI
            case M_MICCID:       //ccid
            case M_UN_COPS:      //ע������
            case M_COPS:
            case M_EGACT:        //pdp������
            case M_CSQ:  
            case M_CEREG:
            case M_CCLK:        //AT+CCLK? 
#if TRAN_PRO == MQTT_PRO                 
            case M_EMQNEW:       //����mqtt������
            case M_EMQCON:       //�����豸
            case M_EMQSUB:       //����
            case M_EMQPUB:       //����
            case M_EMQUNSUB:     //ȡ������
            case M_EMQDISCON:   //�Ͽ�����
#elif TRAN_PRO == LWM2M_PRO

#endif            
            case M_ZFTPOPEN:     //FTP����
            case M_ZFTPSIZE:     //��ȡ�ļ���С
            case M_ZFTPGET:     //�����ļ�
            case M_ZFTPCLOSE:    //�ر�FTP
            case M_ZRST:
     //           break;
            default:
                Up_process.UpState = S_Connet;
#if TRAN_PRO == MQTT_PRO     
            Up_process.Mqtt_Info.connet = 0;    // 0 :δ���� 1�����ӳɹ� 2������ʧ��
#elif TRAN_PRO == LWM2M_PRO  
            Up_process.Lwm2m_Info.Reg_state = 0;
#endif              
                Up_process.NetState = 0;
                NBDevice.Process = M_ZRST;
                NBDevice.RebootTimes++;
                NBDevice.SendTimes = 0;    
                break;
                    
            }
        }
    }
#if TRAN_PRO == MQTT_PRO 
    else
    {                   
        if(NBDevice.Process == M_EMQSUB)
        {
            NBDevice.Process = M_EMQPUB;
            Up_process.UpState = S_Connet;
            Up_process.IncidentPend = 0;
            NBDevice.SendCmd_Pend = 1;
        }
    }
#endif    
      
        
    task_stop(AT_Proce_Overtime);
    
}


void Ftp_FileRecv(void)
{
    uint8_t dat;
    static uint16_t datalen = 0;
    while(NB_Recv_Buff.p_read != NB_Recv_Buff.p_write)
    {
        dat = NB_Recv_Buff.Recvbuff[NB_Recv_Buff.p_read++];
        UartBuff[datalen] = dat;
        if(NB_Recv_Buff.p_read >= USART_BUFF_SIZE) 
            NB_Recv_Buff.p_write = 0;
        if(datalen >= 256)
        {
            datalen = 0;
            print(".");
#ifdef FTP_UPDATA            
            Flash_Write_String(ADDR_APPSTART+FtpInfo.RecvSize,(uint32_t *)UartBuff,256);
            FtpInfo.RecvSize += 256;
#endif
        }        
        
        if(datalen >=2 && UartBuff[datalen-2] == 0x0D && UartBuff[datalen-1] == 0x0A && \
                          UartBuff[datalen-4] == 'O'  && UartBuff[datalen-3] == 'K')    //��ȡ��ν��յķ�ʽ
          
        {
            datalen = 0;
            NBDevice.Process = M_ZFTPGET;
            NBDevice.SendCmd_Pend = 0;
            NBDevice.SendTimes = 0;
            task_stop(AT_Proce_Overtime);
        }
    }
#ifdef FTP_UPDATA    
    if(FtpInfo.RecvSize >= FtpInfo.FileSize)
    {
        
        
        FtpInfo.FileSize = 0;
        FtpInfo.RecvSize = 0;
        FtpInfo.FtpRunning = 0;
#else
    if(HttpInfo.RecvSize >= HttpInfo.FileSize)
    {

        HttpInfo.FileSize = 0;
        HttpInfo.RecvSize = 0;
        HttpInfo.HttpRunning = 0;
#endif 
        datalen = 0;
        NBDevice.SendTimes = 0;
        CheckCodeToUp();
        //���У��ɹ�����Ĳ�������
        Up_process.IncidentPend = 0;
        NBDevice.Process = M_NONE;
        Up_process.UpState = S_ModOff;
    }

}


