#ifndef	__UART_H__
#define __UART_H__

#include "define_all.h"  
#include "drv_nbmodule.h"
//����ͨ�ų�ʱʱ�䳣��
#define txtimeout_max	125				//500ms�������ַ��������ʱ
#define framedelay_mx	15 				//60ms
//ͨ�Ż�������󳤶ȶ���
#define	urx_buf_size	50				//ͨ�Ż�������󳤶�
#define USART_BUFF_SIZE 300
#define uart_rx_size    USART_BUFF_SIZE + AT_BUFF_MAX   //���ջ�������С

#define DEBUG_COM       0           //���Դ���
#define NBMODE_COM      1           //ģ�鴮��


typedef  struct
{
    unsigned short p_write;
    unsigned short p_read;
    unsigned char Recvbuff[USART_BUFF_SIZE];
    CmdType Type;
} UART_RECV_BUFF;
extern UART_RECV_BUFF NB_Recv_Buff;
extern UART_RECV_BUFF CMD_Recv_Buff;
extern unsigned char UartBuff[uart_rx_size];
extern unsigned char CMDBuff[USART_BUFF_SIZE];

extern void Uart_Proc( uint08 Ch );
extern void Veri_Etmr3(void);
extern uint08 Judge_RX_Frame( uint08 Ch, uint08 RcvData );
extern void Tx_Frame( uint08 Ch );
extern void UART485_Send_String( uint08 *buffer);
extern void Uartx_Init(UART_Type* UARTx);
void print(const char fmt[], ...);
void Uartx_close(UART_Type *UARTx);
void Uartx_open(UART_Type *UARTx);
int Uart0SendData(unsigned char *pSendBuf, unsigned int Len);
int Uart1SendData(unsigned char *pSendBuf, unsigned int Len);
CmdType RecvDataFromBuff(UART_RECV_BUFF *CMD,unsigned char *buf);
void Uart0Overtime(void);
void HexToAssic_Uart0Send(unsigned char *pSendBuf, unsigned int Len);
void HexToAssic_Uart1Send(unsigned char *pSendBuf, unsigned int Len);

#endif
