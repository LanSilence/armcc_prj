#ifndef	__DEFINEALL_H__
#define __DEFINEALL_H__

//定义常量, 常数

//系统时钟默认使用RCHF
#define RCHFCLKCFG	8	//8, 16, 24MHz

//define_all.h中RCHFCLKCFG控制系统时钟
#if( RCHFCLKCFG == 8 )//8.0MHz
#define clkmode   1
#define SYSCLKdef RCC_RCHF_CR_FSEL_8MHZ//RCHF中心频率8MHz
#elif( RCHFCLKCFG == 16 )//16.0MHz
#define clkmode   2
#define SYSCLKdef RCC_RCHF_CR_FSEL_16MHZ//RCHF中心频率16MHz
#elif( RCHFCLKCFG == 24 )//24.0MHz
#define clkmode   3
#define SYSCLKdef RCC_RCHF_CR_FSEL_24MHZ//RCHF中心频率24MHz
#endif


//定义常量, 常数
#define WAITTIME 250//2500//	//X * 4MS 上电后等待进入boot的时间，超时后跳转到主程序运行

#define APPLICATION_ADDRESS_OFFSET		(0x4000)

//地址范围:PROGRAMSTA1~PROGRAMEND1~PROGRAMSTA2~PROGRAMEND2
//#define PROGRAMSTA1	0x000000	//程序空间1起始地址
//#ifdef __A02
#define PROGRAMEND1	0x01FFFF	//程序空间1结束地址
#define PROGRAMPROZ	0x02F800	//程序保护区起始地址
//#endif
//#ifdef __A03
//#define PROGRAMEND1	0x02FFFF	//程序空间1结束地址
//#define PROGRAMPROZ	0x02F800	//程序保护区起始地址
//#endif
//#ifdef __A04
//#define PROGRAMEND1	0x03FFFF	//程序空间1结束地址
//#define PROGRAMPROZ	0x03F800	//程序保护区起始地址
//#endif
//#ifdef __A05
//#define PROGRAMEND1	0x05FFFF	//程序空间1结束地址
//#define PROGRAMPROZ	0x05F800	//程序保护区起始地址
//#endif
//#ifdef __A06
//#define PROGRAMEND1	0x07FFFF	//程序空间1结束地址
//#define PROGRAMPROZ	0x07F800	//程序保护区起始地址
//#endif

//uart 缓冲区顺序定义
#define CHHW	0
#define CH485	1
#define CHZB	2

/*变量类型定义*/
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

/*功能IO宏定义*/
#define LED0_GPIO		GPIOC
#define LED0_PIN		LL_GPIO_PIN_0

#define LED1_GPIO		GPIOC
#define LED1_PIN		LL_GPIO_PIN_1

/*操作宏定义*/
#define LED0_ON			LL_GPIO_ResetOutputPin(LED0_GPIO, LED0_PIN)
#define LED0_OFF		LL_GPIO_SetOutputPin(LED0_GPIO, LED0_PIN)
#define LED0_TOG		LL_GPIO_ToggleOutputPin(LED0_GPIO, LED0_PIN)

#define LED1_ON			LL_GPIO_ResetOutputPin(LED1_GPIO, LED1_PIN)
#define LED1_OFF		LL_GPIO_SetOutputPin(LED1_GPIO, LED1_PIN)
#define LED1_TOG		LL_GPIO_ToggleOutputPin(LED1_GPIO, LED1_PIN)

#define EA_OFF	((__get_PRIMASK()&0x00000001) == 0x00000001)
#define EA_ON	((__get_PRIMASK()&0x00000001) == 0x00000000)


/* GPIO配置函数参数宏定义 */
//IO输入口配置 
//type 0 = 普通 
//type 1 = 上拉
#define IN_NORMAL	0
#define IN_PULLUP	1

//IO输出口配置 
//type 0 = 普通 
//type 1 = OD
#define OUT_PUSHPULL	0
#define OUT_OPENDRAIN	1

//IO数字特殊功能口 
//type 0 = 普通 
//type 1 = OD (OD功能仅部分特殊功能支持)
//type 2 = 普通+上拉 
//type 3 = OD+上拉
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



