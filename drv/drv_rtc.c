#include "drv_rtc.h"

// RTC��дʱ������ж�ʾ��
uint8_t TIME_DEFAULT[6] = {0x20,0x08, 0x31, 0x16, 0x17};
void RTC_IRQHandler(void)
{
	if(ENABLE == LL_RTC_IsEnabledIT_Second(RTC) &&
		 SET == LL_RTC_IsActiveFlag_Second(RTC))//��ѯ���жϱ�־�Ƿ�����
	{
		LL_RTC_ClearFlag_Second(RTC);		//������жϱ�־
        task_1000Mstimer();
	}
    
    if(ENABLE == LL_RTC_IsEnabledIT_Minute(RTC) &&
		 SET == LL_RTC_IsActiveFlag_Minute(RTC))//��ѯ���жϱ�־�Ƿ�����
	{		
        
        RTC_minCheck();
        LL_RTC_ClearFlag_Minute(RTC);		//������жϱ�־
	}
}

void RTC_Init(void)
{	

	LL_RTC_InitTypeDef   InitStructer;
    
	InitStructer.Year   = 0x20;
	InitStructer.Month  = 0x02;
	InitStructer.Day    = 0x21;	
	InitStructer.Week   = 0x04;	
	InitStructer.Hour   = 0x15;
	InitStructer.Minute = 0x33;
	InitStructer.Second = 0x00;	
	LL_RTC_Init(RTC,&InitStructer);
    
    RTC->ADJUST = 0xFF;
    RTC->ADSIGN = 0;
    RTC->IER = 0;
	LL_RTC_ClearFlag_Second(RTC);		//������жϱ�־
	LL_RTC_EnableIT_Second(RTC);
    
    
    LL_RTC_ClearFlag_Minute(RTC);
    LL_RTC_EnableIT_Minute(RTC);
    
	NVIC_DisableIRQ(RTC_IRQn);				//NVIC�жϿ���������
	NVIC_SetPriority(RTC_IRQn,2);
	NVIC_EnableIRQ(RTC_IRQn);	
}


/******************************
*��ȡRTCʱ�� BCD�룬
��ڲ���unsigned char ���͵�ָ��
*******************************/
void RTC_GetTime(RTC_Struct *Rtc)
{

    /* ������ */
    Rtc->Second = (uint8_t )LL_RTC_GetSecondReg(RTC);
    /* ���÷��� */
    Rtc->Minute  =(uint8_t )LL_RTC_GetMinutesReg(RTC);
    /* ����Сʱ */
    Rtc->Hour = (uint8_t )LL_RTC_GetHoursReg(RTC);
    /* �������� */
    Rtc->Day = (uint8_t )LL_RTC_GetDaysReg(RTC);
    /* ������ */
    Rtc->Week = (uint8_t )LL_RTC_GetWeeksReg(RTC);
    /* ������ */
    Rtc->Month = (uint8_t )LL_RTC_GetMonthReg(RTC);
    /* ������ */
    Rtc->Year = (uint8_t )LL_RTC_GetYearsReg(RTC);
}



/******************************
*����RTCʱ�� BCD�룬
��ڲ���unsigned char ���͵�ָ��
*******************************/
int RTC_SetTime(RTC_Struct *Rtc)
{
    uint8_t n;
    uint8_t Result = 0;
    RTC_Struct TempTime1;
    LL_RTC_InitTypeDef TempTime2;
    
    TempTime2.Day       = Rtc->Day;
    TempTime2.Hour      = Rtc->Hour;
    TempTime2.Minute    = Rtc->Minute;
    TempTime2.Month     = Rtc->Month;
    TempTime2.Second    = Rtc->Second;
    TempTime2.Year      = Rtc->Year;
    
    
    for(n = 0 ; n < 3; n++)
    {
        LL_RTC_ConfigTime(RTC,&TempTime2);

        RTC_GetTime(&TempTime1);	//��ȡȷ�����ý��
        if( (uint8_t )(TempTime1.Year) == (uint8_t )(Rtc->Year)     &&  
            (uint8_t )(TempTime1.Month) == (uint8_t )(Rtc->Month)   && 
            (uint8_t )(TempTime1.Day) == (uint8_t )(Rtc->Day)       && 
            (uint8_t )(TempTime1.Hour) == (uint8_t )(Rtc->Hour)     && 
            (uint8_t )(TempTime1.Minute) == (uint8_t )(Rtc->Minute) &&    
            (uint8_t )(TempTime1.Second) == (uint8_t )(Rtc->Second)              
           )
        {
            Result = 1;
                break;
        }
    }
    return Result;
}

