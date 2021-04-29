#ifndef __DRV_NB_H
#define __DRV_NB_H

#include "stdint.h"
//#include "main.h"
#include "system_config.h"

#define AT_BUFF_MAX 60  //at指令长度

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
    NB_IDLE,        //空闲状态
    NB_OPEN_VBAT,   //上电
    NB_WAIT_VBAT,   //等待上电
    NB_POWER_ON,    //开机
    NB_WAIT_ON,     //等待开机
    NB_DETECT_ON,   //检测开机
    NB_CHECK_ON,    //验证开机状态
    NB_IS_ON,       //开机
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
    M_CTZU,         //自动获取时间
    M_CGN,          //IMEI
    M_CIMI,         //IMSI
    M_MICCID,       //ccid
    M_UN_COPS,      //注销入网
    M_COPS,
    M_EGACT,        //pdp上下文
    M_CSQ,   
    M_CESQ,         //
    M_CEREG,
    M_CCLK,         //AT+CCLK?    
#if TRAN_PRO == MQTT_PRO    
    M_EMQNEW,       //连接mqtt服务器
    M_EMQCON,       //链接设备
    M_EMQSUB,       //订阅
    M_EMQPUB,       //发布
    M_EMQUNSUB,     //取消订阅
    M_EMQDISCON,    //断开连接
#elif TRAN_PRO == LWM2M_PRO  
    M_M2MCLINEW,    //注册
    M_M2MCLIDEL,    //去注册
    M_M2MCLISEND,   //数据发送
    M_M2MCLICFG,    //设置数据类型
    M_M2MCLI,       //主动上报
    M_M2MCLIREADEN, //设置接收模式    0 直接传至串口 1 放入缓存，等待读取
    M_M2MCLIDATA,   //接收长度上报
    M_M2MCLIREAD,   //读取缓存数据
#endif    
    M_ZFTPOPEN,     //FTP连接
    M_ZFTPSIZE,     //获取文件大小
    M_ZFTPGET,      //下载文件
    M_ZFTPCLOSE,    //关闭FTP
#ifdef HTTPS_UP
    M_EHTTPCREATE,  //建立http客户端              AT+EHTTPCREATE=0,42,42,""https://219.144.130.26:61614/",,,0,,0,,0,"
    M_EHTTPCON,     //建立http连接
    M_EHTTPSEND,    //发送请求
    M_EHTTPDISCON,  //关闭连接
    M_EHTTPDESTROY, //释放连接
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

struct NBMODULE_Str // 总结构体
{
    ATCmdType Process;                            //连接进程
    uint8_t SendCmd_Pend;                         //挂起发送指令
//  uint8_t TxBuffer[SEND_BUFF_SIZE];           //接收缓冲区
//  uint16_t TxLength;                          //发送长度
    uint8_t CCID[20];                             //CCID  
    uint8_t IMEI[15];                             //IMEI        
    uint8_t IMSI[15];                             //IMSI
    uint8_t CSQ;                                  //信号强度
    int16_t RSRP;                                 //信号接收功率
    int16_t SNR;                                  //信噪比
    uint8_t ECL;                                  //增强覆盖等级
    uint32_t CELLID;                              //小区ID
    uint32_t EARFCN;                              //频点    Range 0-262143
    uint16_t PCI;                                 //物理小区标识 Range 0-503
    uint8_t ErrorStep;                            //故障步骤
    uint16_t ErrorCode;                           //故障代码
    uint8_t TimeoutNum;                           //超时计数
    uint8_t ReconnectTimes;                       //重连次数
    uint8_t SendTimes;                            //指令发送次数 
    uint8_t SendTimesMax;                         //最大重发次数
    uint8_t RebootTimes;                          //模块重启次数
    uint8_t EGACT_Count;                          //收到EGACT的次数
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
