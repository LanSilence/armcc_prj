#ifndef APP_H
#define APP_H

#include "drv_rtc.h"
#include "main.h"
#include "power_manage.h"
#include "IRDA_Device.h"



#define FREEZE_DATA_SIZE 8
#define MAX_WRITE_DATA   123*4    //最大写入冻结数据的条数  123条
#define MA_READ_DATA     127*4    //能读取最大的条数        127条  差四条是为了保证最后一次写入的时候能写入成功
#define MAX_TEMP_DATA    168
#pragma anon_unions
typedef union
{
    uint16_t  value;
    struct
    {
        uint16_t remove          :1;     //拆卸
        uint16_t low_voltage     :1;     //电池低压
//        uint16_t is_up           :1;     //是否在上告
        uint16_t shield          :1;     //是否被遮挡
        uint16_t temprature      :1;
        uint16_t nc1             :4;
        uint16_t nc2             :8;
    };

       
}Satus;

typedef struct 
{
    uint8_t uptype;         //3:按天上告，采用timepoint的时间 2:小时上告  1:分钟上告  0:即时上告
    uint8_t interval;
    uint8_t timepoint[3];   //天时分
}UPTIME;
struct USER_PARAM
{
    unsigned char id[20];
    Satus       staus;   
    uint32_t    Voltage;           //电源电压值
    uint8_t     ip[20];
    uint16_t    port;
    uint8_t     updata_ip[4];
    uint16_t    updata_port;
    Power_Struct     PowerState;
    UPTIME      uptime;
    uint16_t    TotalNumber;       //进入的人数
    uint16_t    ShieldTime;        //被遮挡多少天产生告警
    float       Temp_threshold;    //温度告警阈值
    uint8_t     UpPeak;            //错峰
    uint8_t     UpFlag;            //升级标志
};
extern uint8_t AESKey[16];
extern struct USER_PARAM DeviceParam;
extern struct USER_PARAM DefaultParam;
extern uint16_t ShieldCount;
void EnterSleep(void);
void Flash_Read(void);
void CheckCodeToUp(void);
void Save_Parame(void);
void Set_Freeze_Pread(uint32_t pdata);
void Set_Temprature_Pread(uint32_t pdata);
uint16_t Check_Temprature_Data(unsigned long *start);


#endif

