#ifndef	__DEFINEALL_H__
#define __DEFINEALL_H__

//���峣��, ����

//ϵͳʱ��Ĭ��ʹ��RCHF
#define RCHFCLKCFG	8	//8, 16, 24MHz

//define_all.h��RCHFCLKCFG����ϵͳʱ��
#if( RCHFCLKCFG == 8 )//8.0MHz
#define clkmode   1
#define SYSCLKdef RCC_RCHF_CR_FSEL_8MHZ//RCHF����Ƶ��8MHz
#elif( RCHFCLKCFG == 16 )//16.0MHz
#define clkmode   2
#define SYSCLKdef RCC_RCHF_CR_FSEL_16MHZ//RCHF����Ƶ��16MHz
#elif( RCHFCLKCFG == 24 )//24.0MHz
#define clkmode   3
#define SYSCLKdef RCC_RCHF_CR_FSEL_24MHZ//RCHF����Ƶ��24MHz
#endif


//���峣��, ����
#define WAITTIME 250//2500//	//X * 4MS �ϵ��ȴ�����boot��ʱ�䣬��ʱ����ת������������

#define APPLICATION_ADDRESS_OFFSET		(0x4000)

//��ַ��Χ:PROGRAMSTA1~PROGRAMEND1~PROGRAMSTA2~PROGRAMEND2
//#define PROGRAMSTA1	0x000000	//����ռ�1��ʼ��ַ
//#ifdef __A02
#define PROGRAMEND1	0x01FFFF	//����ռ�1������ַ
#define PROGRAMPROZ	0x02F800	//���򱣻�����ʼ��ַ
//#endif
//#ifdef __A03
//#define PROGRAMEND1	0x02FFFF	//����ռ�1������ַ
//#define PROGRAMPROZ	0x02F800	//���򱣻�����ʼ��ַ
//#endif
//#ifdef __A04
//#define PROGRAMEND1	0x03FFFF	//����ռ�1������ַ
//#define PROGRAMPROZ	0x03F800	//���򱣻�����ʼ��ַ
//#endif
//#ifdef __A05
//#define PROGRAMEND1	0x05FFFF	//����ռ�1������ַ
//#define PROGRAMPROZ	0x05F800	//���򱣻�����ʼ��ַ
//#endif
//#ifdef __A06
//#define PROGRAMEND1	0x07FFFF	//����ռ�1������ַ
//#define PROGRAMPROZ	0x07F800	//���򱣻�����ʼ��ַ
//#endif

//uart ������˳����
#define CHHW	0
#define CH485	1
#define CHZB	2

/*�������Ͷ���*/
typedef union
{
  unsigned char B08;
  struct
  {
    unsigned char bit0:1;
    unsigned char bit1:1;
    unsigned char bit2:1;
    unsigned char bit3:1;
    unsigned char bit4:1;
    unsigned char bit5:1;
    unsigned char bit6:1;
    unsigned char bit7:1;
  }Bit;
}B08_Bit;
#define	uint08 uint8_t
#define	uint16 uint16_t
#define uint32 uint32_t
#define	int08 int8_t		
#define	int16 int16_t
#define	int32 int32_t

/*����IO�궨��*/
#define LED0_GPIO		GPIOC
#define LED0_PIN		LL_GPIO_PIN_0

#define LED1_GPIO		GPIOC
#define LED1_PIN		LL_GPIO_PIN_1

/*�����궨��*/
#define LED0_ON			LL_GPIO_ResetOutputPin(LED0_GPIO, LED0_PIN)
#define LED0_OFF		LL_GPIO_SetOutputPin(LED0_GPIO, LED0_PIN)
#define LED0_TOG		LL_GPIO_ToggleOutputPin(LED0_GPIO, LED0_PIN)

#define LED1_ON			LL_GPIO_ResetOutputPin(LED1_GPIO, LED1_PIN)
#define LED1_OFF		LL_GPIO_SetOutputPin(LED1_GPIO, LED1_PIN)
#define LED1_TOG		LL_GPIO_ToggleOutputPin(LED1_GPIO, LED1_PIN)

#define EA_OFF	((__get_PRIMASK()&0x00000001) == 0x00000001)
#define EA_ON	((__get_PRIMASK()&0x00000001) == 0x00000000)


/* GPIO���ú��������궨�� */
//IO��������� 
//type 0 = ��ͨ 
//type 1 = ����
#define IN_NORMAL	0
#define IN_PULLUP	1

//IO��������� 
//type 0 = ��ͨ 
//type 1 = OD
#define OUT_PUSHPULL	0
#define OUT_OPENDRAIN	1

//IO�������⹦�ܿ� 
//type 0 = ��ͨ 
//type 1 = OD (OD���ܽ��������⹦��֧��)
//type 2 = ��ͨ+���� 
//type 3 = OD+����
#define ALTFUN_NORMAL			0
#define ALTFUN_OPENDRAIN		1
#define ALTFUN_PULLUP			2
#define ALTFUN_OPENDRAIN_PULLUP	3


/*include*/
#include "FM33LC0XX.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "bintohex.h"
#include "main.h"
#include "user_init.h"
#include "uart.h"
#include "interrupt.h"
#include "flash.h"
#include "crc.h"
#include "file_bootload.h"

#endif



