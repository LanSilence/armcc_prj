#include "task_user.h"




//定时检测是否被遮挡
//原理：通过定时增加ShieldCount变量，超过某个值时表示被遮挡，或者无人出入
void Check_Shiled(void)
{
    ShieldCount++;
    if(ShieldCount>DeviceParam.ShieldTime*24*60 && DeviceParam.ShieldTime!=0)    //DeviceParam.ShieldTime单位是天所以*24*60转化为分钟
    {
        ShieldCount = 0;
        DeviceParam.staus.shield = 1;
        StartUp(Type_Alarm);
    }
}

void Check_Remove()
{
    static char timeSec = 0;
    static char staus = 0;  
    IO_REMOVE_IN;
    
    if(IO_REMOVE_READ)
    {
        timeSec ++;
        if(timeSec > 4)
        {
            timeSec = 0;   
            staus = IO_REMOVE_READ;            
        }
    }        
    else
    {
        timeSec = 0;
        staus = IO_REMOVE_READ;
    }
        
    if(staus!=DeviceParam.staus.remove)
    {
        DeviceParam.staus.remove = staus;
        print("*设备移动: %hu*\r\n",staus);
        if(staus == 0)
            StartUp(Type_Alarm);
            
    }
    
}

//每十秒检测一次温度
void Check_Temprature()
{
    static uint8_t count = 0;
    static uint8_t errorcount = 0;
    float Temprature = 0;
    static char staus = 0;
    
    
    if(Up_process.UpState != S_Halt || IRDA_Struct.IsADCStart == 1)
        return;
   
    count++;
    if(count>=5)
    {
        count = 0;
        Temprature = Get_Temprature(); 
        if(Temprature>85 || Temprature <-15)
        {
            Get122VSample = GetVref1p22Sample();
            return ;
        }
            
        if(Temprature >= 55)
        {
            errorcount++;
        }
        else
        {
            staus = 0;
        }
            
    }
    if(errorcount>6)
    {
        staus = 1;
    }
    if(staus!=DeviceParam.staus.temprature && staus==1)   //异常
    {
        staus = DeviceParam.staus.temprature;
        if((DeviceParam.uptime.interval == 0 && DeviceParam.uptime.uptype == 0))
            SendDataToSevice(Type_Alarm);
        else
            StartUp(Type_Alarm);
    }
    
}

void MidNightTask()
{
    Up_process.UpTimes = 0;   //清除重复上告次数
    task_stop(DelayUp);
}

extern void Save_Temprature(RTC_Struct *time);
void RTC_minCheck(void)
{
    RTC_Struct Rtc;
    Check_Shiled();     //检测遮挡
    Check_UpTime();
    RTC_GetTime(&Rtc);
    print("%02X-%02X-%02X-%02X-%02X-%02X\n",Rtc.Year,Rtc.Month,Rtc.Day,Rtc.Hour,Rtc.Minute,Rtc.Second);
    if((Up_process.UpState == S_Halt && IRDA_Struct.IsADCStart == 0) || \
       (DeviceParam.uptime.interval == 0 && DeviceParam.uptime.uptype == 0))  //上告的时候电压会变化就不去获取基准电压
    {
        Get122VSample = GetVref1p22Sample();  
        //print("基准AD值：%d\n",Get122VSample);        
    }
    if(Rtc.Minute == 0 && Rtc.Second == 0)   //小时时间
        Save_Temprature(&Rtc);   
    if(Rtc.Hour == 0 && Rtc.Minute == 0 && Rtc.Second == 0)
    {//零点任务
        MidNightTask();
    }
}



//定时扫描
void Scanning(void)
{
    Check_Remove();
    Power_Check();
    Check_Temprature();
}
