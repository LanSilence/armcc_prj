#ifndef DEF_GENERAL
#define DEF_GENERAL

#include "main.h"
#include "drv_rtc.h"

struct Time_tt
{
    unsigned long tm_sec;
    unsigned long tm_min;
    unsigned long tm_hour;
    unsigned long tm_mday;
    unsigned long tm_mon;
    unsigned long tm_year;
};

//void Get_UTC_8Time(RTC_Struct *Time);
unsigned char BcdToHex(unsigned char bcd);
//unsigned long mktime_BCD (RTC_Struct *Time);


#endif


