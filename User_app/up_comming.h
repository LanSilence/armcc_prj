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
    Type_None,        //无任务
    Type_Poweron,     //开机上告
    Type_Ontime,      //定时上告
    Type_UpTemp,      //温度上告
    Type_Realtime,    //即时上告
    Type_Alarm,       //告警上告
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
    uint8_t client_id[20];   //ascii 类型的设备id
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


//上告流程的挂起
typedef enum
{
    RUN_STA = 0,
    STOP_STA = 1,
    KEEP_STA,
}UP_PROCESS_STA;

typedef struct
{
    uint8_t Reg_state;    //注册状态 0 未注册，1 收到ok 2 收到 register success 3 收到 observe 
    
}M2MCLIENT;
#endif
/*上行协议结构体定义*/
typedef  struct
{
    uint8_t           IncidentPend;                           //事件挂起标志 
    MessageId_Type    MsgId;                                  //消息ID
    uint8_t           TxBuffer[uart_tx_size];                 //发送缓冲区
    uint16_t          TxLength;                               //发送长度
    uint8_t           TimeoutTimes;                           //超时次数
    uint8_t           UpTimes;                                //上告次数
    uint8_t           UpState;
#if TRAN_PRO == MQTT_PRO    
    MQTTINFO          Mqtt_Info;
#elif TRAN_PRO == LWM2M_PRO    
    M2MCLIENT         Lwm2m_Info;
#endif
    uint8_t           Up_Task;
    uint8_t           CurrentTask;                            //目前执行的任务
    uint8_t           ModIsReady;                             //连接完成可以发送消息
    uint8_t           NetState;
    uint8_t           Multiframe;                             //是否有后续帧
}Up_Struct;

#ifdef FTP_UPDATA
typedef struct
{
    uint8_t         ip[4];
    uint16_t        port;
    uint8_t         username[20];   //账号
    uint8_t         password[20];   //密码
    uint32_t        FileSize;       //文件大小
    uint8_t         FileDir[20];    //文件路径    
    uint32_t        RecvSize;       //收到大小
    uint8_t         FtpRunning;      //FTP是否在运行
}FTP_STRUCT;
extern FTP_STRUCT FtpInfo;
#else

typedef struct
{
    uint8_t         http_addr[40];  // 服务器地址
    uint8_t         FileDir[40];    // 文件路径 
    uint32_t        FileSize;       // 文件大小
    uint32_t        RecvSize;       // 收到大小
    uint8_t         HttpRunning;    // FTP是否在运行
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


