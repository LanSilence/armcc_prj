#ifndef __DRV_RTC
#define __DRV_RTC

#include "main.h"



typedef struct
{
    /*! �� */    
    uint8_t Year;
    /*! �� */       
    uint8_t Month;        
    /*! �� */   
    uint8_t Day;   
    /*! ʱ */   
    uint8_t Hour;
    /*! �� */   
    uint8_t Minute;    
    /*! �� */   
    uint8_t Second;   
    /*! �� */   
    uint8_t Week;  
                                                         
}RTC_Struct;

extern uint8_t TIME_DEFAULT[6];


void RTC_Init(void);
int RTC_SetTime(RTC_Struct *Rtc);
void RTC_GetTime(RTC_Struct *Rtc);
    
#endif
