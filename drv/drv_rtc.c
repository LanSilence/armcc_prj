#include "drv_rtc.h"

// RTC读写时间和秒中断示例
uint8_t TIME_DEFAULT[6] = {0x20,0x08, 0x31, 0x16, 0x17};
void RTC_IRQHandler(void)
{
	if(ENABLE == LL_RTC_IsEnabledIT_Second(RTC) &&
		 SET == LL_RTC_IsActiveFlag_Second(RTC))//查询秒中断标志是否置起
	{
		LL_RTC_ClearFlag_Second(RTC);		//清除秒中断标志
        task_1000Mstimer();
	}
    
    if(ENABLE == LL_RTC_IsEnabledIT_Minute(RTC) &&
		 SET == LL_RTC_IsActiveFlag_Minute(RTC))//查询秒中断标志是否置起
	{		
        
        RTC_minCheck();
        LL_RTC_ClearFlag_Minute(RTC);		//清除秒中断标志
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
	LL_RTC_ClearFlag_Second(RTC);		//清除秒中断标志
	LL_RTC_EnableIT_Second(RTC);
    
    
    LL_RTC_ClearFlag_Minute(RTC);
    LL_RTC_EnableIT_Minute(RTC);
    
	NVIC_DisableIRQ(RTC_IRQn);				//NVIC中断控制器配置
	NVIC_SetPriority(RTC_IRQn,2);
	NVIC_EnableIRQ(RTC_IRQn);	
}


/******************************
*获取RTC时间 BCD码，
入口参数unsigned char 类型的指针
*******************************/
void RTC_GetTime(RTC_Struct *Rtc)
{

    /* 配置秒 */
    Rtc->Second = (uint8_t )LL_RTC_GetSecondReg(RTC);
    /* 配置分钟 */
    Rtc->Minute  =(uint8_t )LL_RTC_GetMinutesReg(RTC);
    /* 配置小时 */
    Rtc->Hour = (uint8_t )LL_RTC_GetHoursReg(RTC);
    /* 配置日期 */
    Rtc->Day = (uint8_t )LL_RTC_GetDaysReg(RTC);
    /* 配置周 */
    Rtc->Week = (uint8_t )LL_RTC_GetWeeksReg(RTC);
    /* 配置月 */
    Rtc->Month = (uint8_t )LL_RTC_GetMonthReg(RTC);
    /* 配置年 */
    Rtc->Year = (uint8_t )LL_RTC_GetYearsReg(RTC);
}



/******************************
*设置RTC时间 BCD码，
入口参数unsigned char 类型的指针
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

        RTC_GetTime(&TempTime1);	//读取确认设置结果
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

