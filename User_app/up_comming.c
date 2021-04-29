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
            Up_process.Up_Task |= 1U << Type_UpTemp;   //有数据
    }
    Up_process.Up_Task |= 1U << uptyped;
    Up_process.IncidentPend = 0;
    Up_process.UpState = S_Start;
    task_start(typeMs,50,DeviceUpComing_Process);
}

void SendDataToSevice(UPDATATYPE upType)   //即时上告不休眠
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
        NBDevice.Process = M_EMQNEW;//重新连接
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
            print("*开机超时*\r\n");
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
        print("等待任务超时\n");
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
*设置DeviceUpComing_Process的运行状态
*参数：  UpState  Up_process.UpState的状态  run_stop 运行或者挂起
**************/
void Set_UpProcess_UpState(UPState UpState, UP_PROCESS_STA run_stop)
{
    if(run_stop < KEEP_STA)  //运行状态不变化
        Up_process.IncidentPend = run_stop;
    
    Up_process.UpState = UpState;
}

void DelayUp()
{
    StartUp(Type_Ontime);
    task_stop(DelayUp);    //单次运行
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
            task_start(type1000Ms,600,NBDeviceOverTime);    //10分钟超时
            task_reset(typeMs,100,Module_Power_manage);
            task_start(type1000Ms,30,UpComing_Process_Overtime);   //30秒超时
            Device_Power.power_state = NB_IDLE;             //控制模块开机
            NBDevice.SendCmd_Pend = 0 ;
            Up_process.UpState = S_Wait_Power;
            break;
        case S_Wait_Power:
            if(Device_Power.power_state == NB_IS_ON)  //检测模块是否开机成功
            {
                task_reset(type1000Ms,30,UpComing_Process_Overtime);
                Up_process.UpState = S_Connet;   
                Up_process.IncidentPend = 1;
            }
            break;
        case S_Connet:
#if TRAN_PRO == MQTT_PRO     
            if(Up_process.NetState == 1 && Up_process.Mqtt_Info.connet == 1)     // 0 :未连接 1：连接成功 2：连接失败
#elif TRAN_PRO == LWM2M_PRO  
            if(Up_process.NetState == 1 && Up_process.Lwm2m_Info.Reg_state == OBSSUC)
#endif  
            {
                task_reset(type1000Ms,30,UpComing_Process_Overtime);
                Set_UpProcess_UpState(S_DataRead,KEEP_STA);
                NBDevice.SendCmd_Pend = 1;     //先取数据
            }
            break;
        case S_DataRead:
            Up_process.IncidentPend = 1;   //挂起,等待发送完成
            if(Up_process.TxLength == 0)  //缓冲区空可以获取消息
                Get_Report_Message();
            Up_process.UpState = S_WaitTask;
#if TRAN_PRO == MQTT_PRO                    
                NBDevice.Process = M_EMQPUB;
#elif TRAN_PRO == LWM2M_PRO                
                NBDevice.Process = M_M2MCLISEND;
#endif            
            NBDevice.SendCmd_Pend = 0;    //去处理AT指令
            task_reset(type1000Ms,30,UpComing_Process_Overtime);
            break;
        case S_DataSend:     //发送成功等待任务
            Set_UpProcess_UpState(S_WaitTask,STOP_STA);
            task_reset(type1000Ms,10,UpComing_Process_Overtime);   //十秒没有收到任务上告结束
            break;
        case S_WaitTask:
            Up_process.UpState = S_TaskEnd;
            task_reset(type1000Ms,30,UpComing_Process_Overtime);
            break;
        case S_TaskEnd:   //检查时候还有数据需要上报
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
                
                Set_UpProcess_UpState(S_DataSend,RUN_STA);   //发送完成等待 结束命令
                
            }
            else if(DeviceParam.uptime.interval == 0 && DeviceParam.uptime.uptype == 0)  //即时上告
            {
                Up_process.IncidentPend = 1;       //全部挂起等待
                NBDevice.SendCmd_Pend = 1;
                task_stop(NBDeviceOverTime);       //一直在线，关闭超时任务
                task_stop(UpComing_Process_Overtime);
            }
            break;
        case S_UpFail:     //上报失败处理
            if(Up_process.NetState == 0)
            {
                print("*网络连接失败*\r\n");
            }
            //if(Up_process.TimeoutTimes>3)    //不使用这个变量判断
            if(Up_process.UpTimes < 2 && DeviceParam.uptime.uptype == 3)
            {
                //按天上告，开启重复上告
               Up_process.UpTimes ++;
               switch(Up_process.UpTimes)
               {
               case 1:
                   task_reset(type1000Ms,600,DelayUp);
                   print("*上告失败 10分钟后第%hhu次重复上告*",Up_process.UpTimes);
                   break;
               case 2:
                   task_reset(type1000Ms,3600,DelayUp);
                   print("*上告失败 60分钟后第%hhu次重复上告*",Up_process.UpTimes);
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
        case S_UpSucceed:    //上报成功处理
            
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
        case S_ModOff:     //模块关机处理
            Up_process.IncidentPend = 1;
            NBDevice.RebootTimes = 0;
            Up_process.UpState = S_Halt;
            Device_Power.power_state = NB_POWER_OFF;  
            Up_process.Up_Task = 0;
            task_stop(NBDeviceOverTime);
            task_stop(DeviceUpComing_Process);
            task_stop(ATCmd_Process);
            task_stop(UpComing_Process_Overtime);
            print("*流程结束*\r\n");
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
//检测是否到时间上告
void Check_UpTime()
{
    RTC_Struct Rtc;
    RTC_GetTime(&Rtc);
    static uint16_t Int_count = 0;
    static uint16 UpCount = 0;    //统计上一次上告的时间
    
    if(UpCount<=5)
        UpCount++;
    if(DeviceParam.UpPeak == 1)
    {
        calculate_uptime(&UpPeak_time);
    }
    else
        memcpy(&UpPeak_time,&DeviceParam.uptime,sizeof(UPTIME));
    if(DeviceParam.uptime.uptype == 3 && DeviceParam.uptime.interval!=0 && UpCount >= 5)   //按天,距离上一次上告要大于5分钟
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
    else if(DeviceParam.uptime.uptype == 1 && DeviceParam.uptime.interval!=0)  //按分
    {
        Int_count++;
        if(Int_count >= DeviceParam.uptime.interval)
        {
            Int_count = 0;
            StartUp(Type_Ontime);
        }
    }
    else if(DeviceParam.uptime.uptype == 2 && DeviceParam.uptime.interval!=0)  //按小时
    {
        Int_count++;
        if(Int_count >= DeviceParam.uptime.interval*60 )
        {
            Int_count = 0;
            StartUp(Type_Ontime);
        }
    }
}


