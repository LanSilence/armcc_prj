#include "up_comming.h"


Up_Struct Up_process = 
{
    .IncidentPend  = 1,           
    .MsgId         = Message_None,                                   
    .TimeoutTimes  = 5, 
    .UpTimes       = 0,
    .UpState       = 0,

#if TRAN_PRO == MQTT_PRO    
    .Mqtt_Info = 
    {
        .id = 0,
        .client_id = {"150013042"},
        .user_id   = {"langchengping"},
        .password  = {"ZNJMKjfxdNBz7a3rwZgqU50cAs9bjitbv29ZJMKGhGw"},
    },
#elif TRAN_PRO == LWM2M_PRO 
    .Lwm2m_Info = 
    {
       .Reg_state = 0,      
    },
#endif
};
#ifdef HTTPS_UP
//http://124.70.213.41:8080/zhongkongFiles/Tester.bin
HTTP_STRUCT HttpInfo = 
{
    .http_addr      ={"http://124.70.213.41:8080"}, 
    .FileDir        ={"/zhongkongFiles/Tester.bin"},
    .FileSize       = 0, 
    .RecvSize       = 0,
    .HttpRunning    = 0,
};     
#else
FTP_STRUCT FtpInfo = 
{
    .ip         = {192,168,1,1},
    .port       = 1234,
    .username   = {"username\0"},
    .password   = {"12345\0"},
    .FileSize   = 0,   
    .FtpRunning = 0,
};
#endif
void StartUp(UPDATATYPE uptyped)
{
    if(Up_process.Up_Task)
        return ;
    if(uptyped == Type_Ontime || uptyped == Type_Poweron)
    {
        //Up_process.Up_Task |= 1U << Type_UpTemp;
        if(Check_Temprature_Data(NULL)>0)
            Up_process.Up_Task |= 1U << Type_UpTemp;   //������
    }
    Up_process.Up_Task |= 1U << uptyped;
    Up_process.IncidentPend = 0;
    Up_process.UpState = S_Start;
    task_start(typeMs,50,DeviceUpComing_Process);
}

void SendDataToSevice(UPDATATYPE upType)   //��ʱ�ϸ治����
{
    if(Up_process.Up_Task)
        return ;
    if(Up_process.UpState == S_Halt)
        StartUp(upType);
#if TRAN_PRO == MQTT_PRO     
    if(NBDevice.Process == M_NONE && Up_process.Mqtt_Info.connet == 1)
#elif TRAN_PRO == LWM2M_PRO  
    if(NBDevice.Process == M_NONE && Up_process.Lwm2m_Info.Reg_state == OBSSUC)
#endif    
    {
        Up_process.Up_Task |= 1<<upType;
        Up_process.UpState =S_Connet;
        Up_process.IncidentPend = 0;
    }
    else if(Up_process.Lwm2m_Info.Reg_state == 0)
    {
#if TRAN_PRO == MQTT_PRO                    
        NBDevice.Process = M_EMQNEW;//��������
#elif TRAN_PRO == LWM2M_PRO                
        NBDevice.Process = M_M2MCLINEW;
#endif     
        Up_process.Up_Task |= 1<<upType;
        NBDevice.SendCmd_Pend = 0; 
    }
    
}
void UpComing_Process_Overtime(void)
{
    //Up_process.IncidentPend = 0;
    switch(Up_process.UpState)
    {
    case S_Halt:
        
        break;
    case S_Start:
        
        if(Up_process.ModIsReady == 0)
        {
            Up_process.UpState = S_Start;
            Up_process.TimeoutTimes++;
            print("*������ʱ*\r\n");
            if(Up_process.TimeoutTimes > 2)
            {
                Up_process.TimeoutTimes = 0;
                Up_process.UpState = S_ModOff;
            }
        }
        break;
    case S_Connet:
        
        break;
    case S_DataRead:
        
        break;
    case S_DataSend:
        
        break;
    case S_WaitTask:
        Set_UpProcess_UpState(S_UpSucceed,RUN_STA);
        print("�ȴ�����ʱ\n");
        break;
    case S_TaskEnd:
        
        break;
    case S_UpSucceed:
        Set_UpProcess_UpState(S_ModOff,RUN_STA);
        break;
    default:
        break;
    }
    
    task_stop(UpComing_Process_Overtime);
}

void Get_Report_Message()
{

    if(Up_process.Up_Task & (1U << Type_Poweron))
    {
        //Up_process.Up_Task &= ~(1U << Type_Poweron);
        Up_process.CurrentTask = Type_Poweron;
        Get_FrameData(DATA_0A01);
    }
    else if(Up_process.Up_Task & (1U << Type_Ontime))
    {
        //Up_process.Up_Task &= ~(1U << Type_Ontime);
        Up_process.CurrentTask = Type_Ontime;
        Get_FrameData(DATA_0A01);
        
    }
    else if(Up_process.Up_Task & (1U << Type_Realtime))
    {
        Up_process.CurrentTask = Type_Realtime;
        Get_FrameData(DATA_0A01);
    }
    else if(Up_process.Up_Task & (1U << Type_UpTemp))
    {
        //Up_process.Up_Task &= ~(1U << Type_UpTemp);
        Up_process.CurrentTask = Type_UpTemp;
        Get_FrameData(TEMP_0A02);
    }   
    else if(Up_process.Up_Task & (1U << Type_Alarm))
    {
        Up_process.CurrentTask = Type_Alarm;
        Get_FrameData(ALARM_0A09);
    }
}


/************
*����DeviceUpComing_Process������״̬
*������  UpState  Up_process.UpState��״̬  run_stop ���л��߹���
**************/
void Set_UpProcess_UpState(UPState UpState, UP_PROCESS_STA run_stop)
{
    if(run_stop < KEEP_STA)  //����״̬���仯
        Up_process.IncidentPend = run_stop;
    
    Up_process.UpState = UpState;
}

void DelayUp()
{
    StartUp(Type_Ontime);
    task_stop(DelayUp);    //��������
}

void DeviceUpComing_Process()
{
    if(Up_process.IncidentPend == 0)
    {
        switch(Up_process.UpState)
        {
        case S_Halt:
            
            break;
        case S_Start:
            memset(Up_process.TxBuffer,0,sizeof(Up_process.TxBuffer));
            Up_process.ModIsReady = 0;
            task_start(type1000Ms,600,NBDeviceOverTime);    //10���ӳ�ʱ
            task_reset(typeMs,100,Module_Power_manage);
            task_start(type1000Ms,30,UpComing_Process_Overtime);   //30�볬ʱ
            Device_Power.power_state = NB_IDLE;             //����ģ�鿪��
            NBDevice.SendCmd_Pend = 0 ;
            Up_process.UpState = S_Wait_Power;
            break;
        case S_Wait_Power:
            if(Device_Power.power_state == NB_IS_ON)  //���ģ���Ƿ񿪻��ɹ�
            {
                task_reset(type1000Ms,30,UpComing_Process_Overtime);
                Up_process.UpState = S_Connet;   
                Up_process.IncidentPend = 1;
            }
            break;
        case S_Connet:
#if TRAN_PRO == MQTT_PRO     
            if(Up_process.NetState == 1 && Up_process.Mqtt_Info.connet == 1)     // 0 :δ���� 1�����ӳɹ� 2������ʧ��
#elif TRAN_PRO == LWM2M_PRO  
            if(Up_process.NetState == 1 && Up_process.Lwm2m_Info.Reg_state == OBSSUC)
#endif  
            {
                task_reset(type1000Ms,30,UpComing_Process_Overtime);
                Set_UpProcess_UpState(S_DataRead,KEEP_STA);
                NBDevice.SendCmd_Pend = 1;     //��ȡ����
            }
            break;
        case S_DataRead:
            Up_process.IncidentPend = 1;   //����,�ȴ��������
            if(Up_process.TxLength == 0)  //�������տ��Ի�ȡ��Ϣ
                Get_Report_Message();
            Up_process.UpState = S_WaitTask;
#if TRAN_PRO == MQTT_PRO                    
                NBDevice.Process = M_EMQPUB;
#elif TRAN_PRO == LWM2M_PRO                
                NBDevice.Process = M_M2MCLISEND;
#endif            
            NBDevice.SendCmd_Pend = 0;    //ȥ����ATָ��
            task_reset(type1000Ms,30,UpComing_Process_Overtime);
            break;
        case S_DataSend:     //���ͳɹ��ȴ�����
            Set_UpProcess_UpState(S_WaitTask,STOP_STA);
            task_reset(type1000Ms,10,UpComing_Process_Overtime);   //ʮ��û���յ������ϸ����
            break;
        case S_WaitTask:
            Up_process.UpState = S_TaskEnd;
            task_reset(type1000Ms,30,UpComing_Process_Overtime);
            break;
        case S_TaskEnd:   //���ʱ����������Ҫ�ϱ�
            if(Up_process.Up_Task & (1<<Type_Ontime) || Up_process.Multiframe == 1)
            {
                Up_process.UpState = S_DataRead; 
                Up_process.Up_Task |= 1<<Type_Ontime;
                Up_process.CurrentTask = Type_Ontime;
                
            }
            else if(Up_process.Up_Task &(1<<Type_UpTemp)|| Up_process.Multiframe == 2)
            {
                Up_process.UpState = S_DataRead; 
                Up_process.Up_Task |= 1<<Type_UpTemp;
                Up_process.CurrentTask = Type_UpTemp;
            }
            else if(DeviceParam.uptime.interval != 0 || DeviceParam.uptime.uptype != 0 )
            {
                
                Set_UpProcess_UpState(S_DataSend,RUN_STA);   //������ɵȴ� ��������
                
            }
            else if(DeviceParam.uptime.interval == 0 && DeviceParam.uptime.uptype == 0)  //��ʱ�ϸ�
            {
                Up_process.IncidentPend = 1;       //ȫ������ȴ�
                NBDevice.SendCmd_Pend = 1;
                task_stop(NBDeviceOverTime);       //һֱ���ߣ��رճ�ʱ����
                task_stop(UpComing_Process_Overtime);
            }
            break;
        case S_UpFail:     //�ϱ�ʧ�ܴ���
            if(Up_process.NetState == 0)
            {
                print("*��������ʧ��*\r\n");
            }
            //if(Up_process.TimeoutTimes>3)    //��ʹ����������ж�
            if(Up_process.UpTimes < 2 && DeviceParam.uptime.uptype == 3)
            {
                //�����ϸ棬�����ظ��ϸ�
               Up_process.UpTimes ++;
               switch(Up_process.UpTimes)
               {
               case 1:
                   task_reset(type1000Ms,600,DelayUp);
                   print("*�ϸ�ʧ�� 10���Ӻ��%hhu���ظ��ϸ�*",Up_process.UpTimes);
                   break;
               case 2:
                   task_reset(type1000Ms,3600,DelayUp);
                   print("*�ϸ�ʧ�� 60���Ӻ��%hhu���ظ��ϸ�*",Up_process.UpTimes);
                   break;
               default :
                   break;
               }
               
            }
            if(DeviceParam.uptime.interval != 0)
            {
                NBDevice.SendCmd_Pend = 1; 
                Set_UpProcess_UpState(S_UpSucceed,RUN_STA); 
            }
        case S_UpSucceed:    //�ϱ��ɹ�����
            
            if( Up_process.Lwm2m_Info.Reg_state == OBSSUC)
            {
                NBDevice.SendCmd_Pend = 0;
#if TRAN_PRO == MQTT_PRO                    
                NBDevice.Process = M_EMQDISCON;
#elif TRAN_PRO == LWM2M_PRO                
                NBDevice.Process = M_M2MCLIDEL;
#endif
            }
            Up_process.UpTimes = 0;
            task_stop(DelayUp);
            Set_UpProcess_UpState(S_UpSucceed,STOP_STA);
            task_reset(type1000Ms,3,UpComing_Process_Overtime);
            break;
        case S_ModOff:     //ģ��ػ�����
            Up_process.IncidentPend = 1;
            NBDevice.RebootTimes = 0;
            Up_process.UpState = S_Halt;
            Device_Power.power_state = NB_POWER_OFF;  
            Up_process.Up_Task = 0;
            task_stop(NBDeviceOverTime);
            task_stop(DeviceUpComing_Process);
            task_stop(ATCmd_Process);
            task_stop(UpComing_Process_Overtime);
            print("*���̽���*\r\n");
            CheckCodeToUp();
            break;
        default:
            break;
        }
    }
}



UPTIME      UpPeak_time;
void calculate_uptime(UPTIME *uptime)
{
    memcpy(uptime,&DeviceParam.uptime,sizeof(UPTIME));
    uptime->timepoint[2] = ((DeviceParam.id[15]-0x30)*2 + uptime->timepoint[2])/60;
}
//����Ƿ�ʱ���ϸ�
void Check_UpTime()
{
    RTC_Struct Rtc;
    RTC_GetTime(&Rtc);
    static uint16_t Int_count = 0;
    static uint16 UpCount = 0;    //ͳ����һ���ϸ��ʱ��
    
    if(UpCount<=5)
        UpCount++;
    if(DeviceParam.UpPeak == 1)
    {
        calculate_uptime(&UpPeak_time);
    }
    else
        memcpy(&UpPeak_time,&DeviceParam.uptime,sizeof(UPTIME));
    if(DeviceParam.uptime.uptype == 3 && DeviceParam.uptime.interval!=0 && UpCount >= 5)   //����,������һ���ϸ�Ҫ����5����
    {
        UpCount = 0;
        if(Rtc.Hour == DeviceParam.uptime.timepoint[1] &&
            Rtc.Minute  == DeviceParam.uptime.timepoint[2] )
        {
            Int_count++;
            if(Int_count >= DeviceParam.uptime.interval)
                StartUp(Type_Ontime);
        }
            
    }
    else if(DeviceParam.uptime.uptype == 1 && DeviceParam.uptime.interval!=0)  //����
    {
        Int_count++;
        if(Int_count >= DeviceParam.uptime.interval)
        {
            Int_count = 0;
            StartUp(Type_Ontime);
        }
    }
    else if(DeviceParam.uptime.uptype == 2 && DeviceParam.uptime.interval!=0)  //��Сʱ
    {
        Int_count++;
        if(Int_count >= DeviceParam.uptime.interval*60 )
        {
            Int_count = 0;
            StartUp(Type_Ontime);
        }
    }
}


