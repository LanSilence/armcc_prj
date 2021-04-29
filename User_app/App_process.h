#ifndef APP_H
#define APP_H

#include "drv_rtc.h"
#include "main.h"
#include "power_manage.h"
#include "IRDA_Device.h"



#define FREEZE_DATA_SIZE 8
#define MAX_WRITE_DATA   123*4    //���д�붳�����ݵ�����  123��
#define MA_READ_DATA     127*4    //�ܶ�ȡ��������        127��  ��������Ϊ�˱�֤���һ��д���ʱ����д��ɹ�
#define MAX_TEMP_DATA    168
#pragma anon_unions
typedef union
{
    uint16_t  value;
    struct
    {
        uint16_t remove          :1;     //��ж
        uint16_t low_voltage     :1;     //��ص�ѹ
//        uint16_t is_up           :1;     //�Ƿ����ϸ�
        uint16_t shield          :1;     //�Ƿ��ڵ�
        uint16_t temprature      :1;
        uint16_t nc1             :4;
        uint16_t nc2             :8;
    };

       
}Satus;

typedef struct 
{
    uint8_t uptype;         //3:�����ϸ棬����timepoint��ʱ�� 2:Сʱ�ϸ�  1:�����ϸ�  0:��ʱ�ϸ�
    uint8_t interval;
    uint8_t timepoint[3];   //��ʱ��
}UPTIME;
struct USER_PARAM
{
    unsigned char id[20];
    Satus       staus;   
    uint32_t    Voltage;           //��Դ��ѹֵ
    uint8_t     ip[20];
    uint16_t    port;
    uint8_t     updata_ip[4];
    uint16_t    updata_port;
    Power_Struct     PowerState;
    UPTIME      uptime;
    uint16_t    TotalNumber;       //���������
    uint16_t    ShieldTime;        //���ڵ�����������澯
    float       Temp_threshold;    //�¶ȸ澯��ֵ
    uint8_t     UpPeak;            //���
    uint8_t     UpFlag;            //������־
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

