#ifndef __SYS_CONFIG
#define __SYS_CONFIG


#define VER_H 0x01  //�汾�Ÿ�λ
#define VER_L 0x00  //�汾�ŵ�λ

#define MAXTASK_NUMBER    32  //�����������
#define ENABLE_LPUART1     1  //ʹ�õ͹��Ĵ���
//#define AES_128
//#define TOUCHUAN              //����͸��

//#define FTP_UPDATA


//ʹ�ܴ�����Ϣ��ӡ
#define PRINT_INFO_EN

#ifndef FTP_UPDATA

#define HTTPS_UP
#endif

#define MQTT_PRO  1
#define LWM2M_PRO 2

#define TRAN_PRO  LWM2M_PRO


#endif

