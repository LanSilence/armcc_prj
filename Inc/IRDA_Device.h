#ifndef __IRDA_H
#define __IRDA_H
#include "main.h"

#define Support_Num 4
#define ADC_VREF    (*((uint16_t *)(0x1FFFFB08)))   // 30℃ vref1.22采样值
#define TS_CAL      (*((uint32_t *)(0x1FFFFA90)))   // 30℃使用 ADC 采样并转换温度传感器输出电压


#define High_Level 2800
#define Low_level  300
#define MAX_TRIG_TIMES 20
typedef enum
{
    ABNONE,
    A2B_DIR,
    B2A_DIR,
    BOTH_DIR,
}DIRECT;

typedef struct
{
    uint8_t TriggerBuf[MAX_TRIG_TIMES];
    uint8_t count;
}TRIG_STRUCT;

typedef struct  
{
    uint8_t IRDA_VotageCount;
//    uint8_t IRDA2_Flag;
    DIRECT Direct[Support_Num];
    uint8_t IsADCStart;
    uint64_t ADCRdresult_A;
    uint64_t ADCRdresult_B;
}IRDA_DEVICE_STRUCT;


extern TRIG_STRUCT Trigger;
extern IRDA_DEVICE_STRUCT IRDA_Struct;
extern uint32_t Get122VSample;
void IRDA_Init(void);
void Deal_IRDA1(void);
void Deal_IRDA2(void);
void Judge_Direction(void);
uint32_t GetVref1p22Sample(void);
extern float Get_Temprature(void);
void Clear_IRDA(void);
#endif



