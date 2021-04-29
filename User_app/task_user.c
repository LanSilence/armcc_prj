#include "task_user.h"




//��ʱ����Ƿ��ڵ�
//ԭ��ͨ����ʱ����ShieldCount����������ĳ��ֵʱ��ʾ���ڵ����������˳���
void Check_Shiled(void)
{
    ShieldCount++;
    if(ShieldCount>DeviceParam.ShieldTime*24*60 && DeviceParam.ShieldTime!=0)    //DeviceParam.ShieldTime��λ��������*24*60ת��Ϊ����
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
        print("*�豸�ƶ�: %hu*\r\n",staus);
        if(staus == 0)
            StartUp(Type_Alarm);
            
    }
    
}

//ÿʮ����һ���¶�
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
    if(staus!=DeviceParam.staus.temprature && staus==1)   //�쳣
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
    Up_process.UpTimes = 0;   //����ظ��ϸ����
    task_stop(DelayUp);
}

extern void Save_Temprature(RTC_Struct *time);
void RTC_minCheck(void)
{
    RTC_Struct Rtc;
    Check_Shiled();     //����ڵ�
    Check_UpTime();
    RTC_GetTime(&Rtc);
    print("%02X-%02X-%02X-%02X-%02X-%02X\n",Rtc.Year,Rtc.Month,Rtc.Day,Rtc.Hour,Rtc.Minute,Rtc.Second);
    if((Up_process.UpState == S_Halt && IRDA_Struct.IsADCStart == 0) || \
       (DeviceParam.uptime.interval == 0 && DeviceParam.uptime.uptype == 0))  //�ϸ��ʱ���ѹ��仯�Ͳ�ȥ��ȡ��׼��ѹ
    {
        Get122VSample = GetVref1p22Sample();  
        //print("��׼ADֵ��%d\n",Get122VSample);        
    }
    if(Rtc.Minute == 0 && Rtc.Second == 0)   //Сʱʱ��
        Save_Temprature(&Rtc);   
    if(Rtc.Hour == 0 && Rtc.Minute == 0 && Rtc.Second == 0)
    {//�������
        MidNightTask();
    }
}



//��ʱɨ��
void Scanning(void)
{
    Check_Remove();
    Power_Check();
    Check_Temprature();
}
