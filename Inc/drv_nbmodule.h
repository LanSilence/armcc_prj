#ifndef __DRV_NB_H
#define __DRV_NB_H

#include "stdint.h"
//#include "main.h"
#include "system_config.h"

#define AT_BUFF_MAX 60  //atָ���

typedef enum
{
    AT_Type,
    PROT_Type,
    Err_Type,
}CmdType;

typedef enum 
{
    POWEROFF,
    POWERON,
}POWERONOFF;

typedef enum 
{
    NB_IDLE,        //����״̬
    NB_OPEN_VBAT,   //�ϵ�
    NB_WAIT_VBAT,   //�ȴ��ϵ�
    NB_POWER_ON,    //����
    NB_WAIT_ON,     //�ȴ�����
    NB_DETECT_ON,   //��⿪��
    NB_CHECK_ON,    //��֤����״̬
    NB_IS_ON,       //����
    NB_VBAT_DOWN,
    NB_POWER_OFF,
    NB_WAIT_OFF,
    
}POWER_STATE;

typedef enum    //M:mask
{
    M_NONE,
    M_AT,
    M_ATE0,
    M_ATI,    
    M_CTZU,         //�Զ���ȡʱ��
    M_CGN,          //IMEI
    M_CIMI,         //IMSI
    M_MICCID,       //ccid
    M_UN_COPS,      //ע������
    M_COPS,
    M_EGACT,        //pdp������
    M_CSQ,   
    M_CESQ,         //
    M_CEREG,
    M_CCLK,         //AT+CCLK?    
#if TRAN_PRO == MQTT_PRO    
    M_EMQNEW,       //����mqtt������
    M_EMQCON,       //�����豸
    M_EMQSUB,       //����
    M_EMQPUB,       //����
    M_EMQUNSUB,     //ȡ������
    M_EMQDISCON,    //�Ͽ�����
#elif TRAN_PRO == LWM2M_PRO  
    M_M2MCLINEW,    //ע��
    M_M2MCLIDEL,    //ȥע��
    M_M2MCLISEND,   //���ݷ���
    M_M2MCLICFG,    //������������
    M_M2MCLI,       //�����ϱ�
    M_M2MCLIREADEN, //���ý���ģʽ    0 ֱ�Ӵ������� 1 ���뻺�棬�ȴ���ȡ
    M_M2MCLIDATA,   //���ճ����ϱ�
    M_M2MCLIREAD,   //��ȡ��������
#endif    
    M_ZFTPOPEN,     //FTP����
    M_ZFTPSIZE,     //��ȡ�ļ���С
    M_ZFTPGET,      //�����ļ�
    M_ZFTPCLOSE,    //�ر�FTP
#ifdef HTTPS_UP
    M_EHTTPCREATE,  //����http�ͻ���              AT+EHTTPCREATE=0,42,42,""https://219.144.130.26:61614/",,,0,,0,,0,"
    M_EHTTPCON,     //����http����
    M_EHTTPSEND,    //��������
    M_EHTTPDISCON,  //�ر�����
    M_EHTTPDESTROY, //�ͷ�����
#endif
    M_MIPLCREATE,   
    M_MIPLADDOBJ,
    M_MIPLOPEN,
    M_MIPLNOTIFY,
    M_MIPLWRITERSP,
    M_MIPCLOSE,
    M_ZRST,
}ATCmdType;
typedef struct 
{
    POWER_STATE power_state;
    unsigned long Tim_delay;
    unsigned char PowerOnTimes;
}DEVICE_POWER;

typedef struct 
{
    unsigned char state;    
}NBSTATE;
extern NBSTATE NBState;
extern DEVICE_POWER Device_Power;

struct NBMODULE_Str // �ܽṹ��
{
    ATCmdType Process;                            //���ӽ���
    uint8_t SendCmd_Pend;                         //������ָ��
//  uint8_t TxBuffer[SEND_BUFF_SIZE];           //���ջ�����
//  uint16_t TxLength;                          //���ͳ���
    uint8_t CCID[20];                             //CCID  
    uint8_t IMEI[15];                             //IMEI        
    uint8_t IMSI[15];                             //IMSI
    uint8_t CSQ;                                  //�ź�ǿ��
    int16_t RSRP;                                 //�źŽ��չ���
    int16_t SNR;                                  //�����
    uint8_t ECL;                                  //��ǿ���ǵȼ�
    uint32_t CELLID;                              //С��ID
    uint32_t EARFCN;                              //Ƶ��    Range 0-262143
    uint16_t PCI;                                 //����С����ʶ Range 0-503
    uint8_t ErrorStep;                            //���ϲ���
    uint16_t ErrorCode;                           //���ϴ���
    uint8_t TimeoutNum;                           //��ʱ����
    uint8_t ReconnectTimes;                       //��������
    uint8_t SendTimes;                            //ָ��ʹ��� 
    uint8_t SendTimesMax;                         //����ط�����
    uint8_t RebootTimes;                          //ģ����������
    uint8_t EGACT_Count;                          //�յ�EGACT�Ĵ���
};
extern struct NBMODULE_Str NBDevice;

void NB_Module_VBAT(POWERONOFF power);
void touchuang_test(void);
unsigned char Get_ATCMDFrom(ATCmdType AT_CMD,  char *CMD_Buf);
void Module_Power_manage(void);
void NBDeviceOverTime(void);
void AT_Proce_Overtime(void);
void ATCmd_Process(void);
void Ftp_FileRecv(void);

#endif
