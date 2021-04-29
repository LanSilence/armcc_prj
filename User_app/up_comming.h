#ifndef __UP_COM_H
#define __UP_COM_H

#include "uart.h"
#include "main.h"

#define uart_tx_size 256

typedef enum
{
    Message_None = 0,
}MessageId_Type;   

typedef enum
{
    Type_None,        //������
    Type_Poweron,     //�����ϸ�
    Type_Ontime,      //��ʱ�ϸ�
    Type_UpTemp,      //�¶��ϸ�
    Type_Realtime,    //��ʱ�ϸ�
    Type_Alarm,       //�澯�ϸ�
}UPDATATYPE;


typedef enum
{
    S_Halt,
    S_Start,
    S_Wait_Power,
    S_Connet,
    S_DataSend,
    S_DataRead,
    S_WaitTask,
    S_TaskEnd,
    S_UpSucceed,
    S_UpFail,
    S_Disconnect,
    S_FtpStep,
    S_ModOff,
    
}UPState;

#if TRAN_PRO == MQTT_PRO
typedef struct
{
    uint8_t id;
    uint8_t client_id[20];   //ascii ���͵��豸id
    uint8_t connet;
    uint8_t user_id[20];
    uint8_t password[50];
}MQTTINFO;
#elif TRAN_PRO == LWM2M_PRO

typedef enum
{
    UNREG,
    RECVOK,
    RESSUC,
    OBSSUC,
}RER_STATE;


//�ϸ����̵Ĺ���
typedef enum
{
    RUN_STA = 0,
    STOP_STA = 1,
    KEEP_STA,
}UP_PROCESS_STA;

typedef struct
{
    uint8_t Reg_state;    //ע��״̬ 0 δע�ᣬ1 �յ�ok 2 �յ� register success 3 �յ� observe 
    
}M2MCLIENT;
#endif
/*����Э��ṹ�嶨��*/
typedef  struct
{
    uint8_t           IncidentPend;                           //�¼������־ 
    MessageId_Type    MsgId;                                  //��ϢID
    uint8_t           TxBuffer[uart_tx_size];                 //���ͻ�����
    uint16_t          TxLength;                               //���ͳ���
    uint8_t           TimeoutTimes;                           //��ʱ����
    uint8_t           UpTimes;                                //�ϸ����
    uint8_t           UpState;
#if TRAN_PRO == MQTT_PRO    
    MQTTINFO          Mqtt_Info;
#elif TRAN_PRO == LWM2M_PRO    
    M2MCLIENT         Lwm2m_Info;
#endif
    uint8_t           Up_Task;
    uint8_t           CurrentTask;                            //Ŀǰִ�е�����
    uint8_t           ModIsReady;                             //������ɿ��Է�����Ϣ
    uint8_t           NetState;
    uint8_t           Multiframe;                             //�Ƿ��к���֡
}Up_Struct;

#ifdef FTP_UPDATA
typedef struct
{
    uint8_t         ip[4];
    uint16_t        port;
    uint8_t         username[20];   //�˺�
    uint8_t         password[20];   //����
    uint32_t        FileSize;       //�ļ���С
    uint8_t         FileDir[20];    //�ļ�·��    
    uint32_t        RecvSize;       //�յ���С
    uint8_t         FtpRunning;      //FTP�Ƿ�������
}FTP_STRUCT;
extern FTP_STRUCT FtpInfo;
#else

typedef struct
{
    uint8_t         http_addr[40];  // ��������ַ
    uint8_t         FileDir[40];    // �ļ�·�� 
    uint32_t        FileSize;       // �ļ���С
    uint32_t        RecvSize;       // �յ���С
    uint8_t         HttpRunning;    // FTP�Ƿ�������
    uint8_t         HttpClient;
}HTTP_STRUCT;

extern HTTP_STRUCT HttpInfo;
#endif
extern Up_Struct Up_process;


void DeviceUpComing_Process(void);
void StartUp(UPDATATYPE uptyped);
void Check_UpTime(void);
void SendDataToSevice(UPDATATYPE upType);
void DelayUp(void);
void Set_UpProcess_UpState(UPState UpState, UP_PROCESS_STA run_stop);
#endif


