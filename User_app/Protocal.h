#ifndef DEF_PROTOCAL
#define DEF_PROTOCAL

#include "main.h"

#define PROTO_HEAD_SIZE 32



#define DATA_0A01       0x0A01      //������¼�����ϸ�
#define TEMP_0A02       0x0A02      //�¶�����
#define ALARM_0A09      0x0A09      //�澯
#define CMD_0A0A        0x0A0A      //�����޸�
#define UPGRADE_0A0B    0x0A0B      //��������
#define UPGRADE_0A0C    0x0A0C      //�������ݰ�
#define CMD_0AED        0x0AED      //����֡


/*********������ָ��***********/
#define TIME_AAF1       0xAAF1      //Уʱ   
#define FACTORY_AAF2    0xAAF2      //�ָ�����



typedef struct
{
    uint8_t UpFlag[2];      //UP������־
    uint8_t Version[2];     //�汾��
    uint32_t Size;          //��С
    uint8_t Type[4];        //FULL ȫ����  DIFF��ְ�
}UPGARDE_PARAMER;    



extern UPGARDE_PARAMER Upgrade_Paramer;
void Get_FrameData(uint16_t cmd);
uint16_t Get_Temprature_Freeze(uint8_t *Pin, uint8_t *Multiframe);
void Protocal_Analyze(uint8_t *Pin,uint16_t CMD,uint16_t Len,uint8_t ComType);
void Process_Cmd(uint8_t *Pin, uint8_t ComType);

#endif


