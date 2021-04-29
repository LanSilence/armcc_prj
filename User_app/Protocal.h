#ifndef DEF_PROTOCAL
#define DEF_PROTOCAL

#include "main.h"

#define PROTO_HEAD_SIZE 32



#define DATA_0A01       0x0A01      //人流记录数据上告
#define TEMP_0A02       0x0A02      //温度数据
#define ALARM_0A09      0x0A09      //告警
#define CMD_0A0A        0x0A0A      //参数修改
#define UPGRADE_0A0B    0x0A0B      //升级参数
#define UPGRADE_0A0C    0x0A0C      //升级数据包
#define CMD_0AED        0x0AED      //结束帧


/*********测试用指令***********/
#define TIME_AAF1       0xAAF1      //校时   
#define FACTORY_AAF2    0xAAF2      //恢复出厂



typedef struct
{
    uint8_t UpFlag[2];      //UP升级标志
    uint8_t Version[2];     //版本号
    uint32_t Size;          //大小
    uint8_t Type[4];        //FULL 全量包  DIFF差分包
}UPGARDE_PARAMER;    



extern UPGARDE_PARAMER Upgrade_Paramer;
void Get_FrameData(uint16_t cmd);
uint16_t Get_Temprature_Freeze(uint8_t *Pin, uint8_t *Multiframe);
void Protocal_Analyze(uint8_t *Pin,uint16_t CMD,uint16_t Len,uint8_t ComType);
void Process_Cmd(uint8_t *Pin, uint8_t ComType);

#endif


