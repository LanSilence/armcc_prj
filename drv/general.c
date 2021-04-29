#include "main.h"


//
unsigned char BcdToHex(unsigned char bcd)
{
    return ((bcd >> 4) & 0x0f) * 10 + (bcd & 0x0f);
}

unsigned char HexToBcd(unsigned char hex)
{
    return (((hex / 10) << 4) & 0xf0) | ((hex % 10) & 0x0f);
}

unsigned char isLeapYear(unsigned int year)
{
    if (0 == year%400)
        return 1;
    if ((0 == year%4 ) && (0 != year%100))//闰年的条件,能同时被和整除的或者能被整除的年份就是闰年
        return 1;
    return 0;
}

unsigned long mktime (struct Time_tt *p)
{
    unsigned long mon = p->tm_mon, year = p->tm_year; 
    
    if (0 >= (int) (mon -= 2)) 
    {    /* 1..12 -> 11,12,1..10 */
         mon += 12;      /* Puts Feb last since it has leap day */
         year -= 1;
    }

    return (((
             (unsigned long) (year/4 - year/100 + year/400 + 367 * mon/12 + p->tm_mday) +
             year*365 - 719499
          )*24 + p->tm_hour /* now have hours */
       )*60 + p->tm_min /* now have minutes */
    )*60 + p->tm_sec; /* finally seconds */
}

void gmtime(unsigned long p, RTC_Struct *pTime)
{
    unsigned short year;
    unsigned long  tmp;
    unsigned long  totalDays    = 365;
    unsigned long  days         = p/86400UL + 1;       //总天数, 一天是 86400秒 = 24*60*60
    unsigned char  dayCount[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    year = 1970;   //从1970-1-1起比较时间
    
    while (1)
    {
        if (isLeapYear(year))
            totalDays = 366;
        else
            totalDays = 365;
        if (days <= totalDays )
            break;

        days -= totalDays;
        year++;    //计算年
    }
    
    if (isLeapYear(year)) 
    {
        dayCount[1]++;  //闰年2月是29天
    }
    pTime->Month = 1;
    for (unsigned char i = 0; i<12; i++)
    {
        if (days <= dayCount[i])
        {
            pTime->Day = days;
            break;
        }
        else
        {
            pTime->Month++;
            days -= dayCount[i];
        }
 
    }
    pTime->Year = HexToBcd(year - 2000);
    pTime->Month  = HexToBcd(pTime->Month);
    pTime->Day  = HexToBcd(pTime->Day);
    tmp = p%86400UL;
    pTime->Hour = HexToBcd((unsigned char)(tmp/3600));
    pTime->Minute  = HexToBcd((unsigned char)((tmp%3600)/60));
    pTime->Second  = HexToBcd((unsigned char)(tmp%60));
}

void Get_UTC_8Time(RTC_Struct *Time)
{
    struct Time_tt tt;
    
    tt.tm_year = BcdToHex(Time->Year) +2000;
    tt.tm_mon  = BcdToHex(Time->Month);
    tt.tm_mday = BcdToHex(Time->Day);
    tt.tm_hour = BcdToHex(Time->Hour);
    tt.tm_min  = BcdToHex(Time->Minute);
    tt.tm_sec  = BcdToHex(Time->Second);
    
    
    gmtime(mktime(&tt)+8*3600,Time);
       
}

unsigned long mktime_BCD (RTC_Struct *Time)
{
    struct Time_tt tt;
    
    tt.tm_year = BcdToHex(Time->Year) +2000;
    tt.tm_mon  = BcdToHex(Time->Month);
    tt.tm_mday = BcdToHex(Time->Day);
    tt.tm_hour = BcdToHex(Time->Hour);
    tt.tm_min  = BcdToHex(Time->Minute);
    tt.tm_sec  = BcdToHex(Time->Second);
    
    return mktime(&tt);
}


