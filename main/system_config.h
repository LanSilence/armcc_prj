#ifndef __SYS_CONFIG
#define __SYS_CONFIG


#define VER_H 0x01  //版本号高位
#define VER_L 0x00  //版本号低位

#define MAXTASK_NUMBER    32  //最大任务数量
#define ENABLE_LPUART1     1  //使用低功耗串口
//#define AES_128
//#define TOUCHUAN              //开启透传

//#define FTP_UPDATA


//使能串口信息打印
#define PRINT_INFO_EN

#ifndef FTP_UPDATA

#define HTTPS_UP
#endif

#define MQTT_PRO  1
#define LWM2M_PRO 2

#define TRAN_PRO  LWM2M_PRO


#endif

