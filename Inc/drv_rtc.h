#ifndef __DRV_RTC
#define __DRV_RTC

#include "main.h"



typedef struct
{
    /*! 年 */    
    uint8_t Year;
    /*! 月 */       
    uint8_t Month;        
    /*! 日 */   
    uint8_t Day;   
    /*! 时 */   
    uint8_t Hour;
    /*! 分 */   
    uint8_t Minute;    
    /*! 秒 */   
    uint8_t Second;   
    /*! 周 */   
    uint8_t Week;  
                                                         
}RTC_Struct;

extern uint8_t TIME_DEFAULT[6];


void RTC_Init(void);
int RTC_SetTime(RTC_Struct *Rtc);
void RTC_GetTime(RTC_Struct *Rtc);
    
#endif
