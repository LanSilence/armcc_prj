#include "lpuart.h"
#include "main.h"
#include "user_init.h"



void LPUART0_IRQHandler(void)
{	
	//接收中断处理
	if((ENABLE == LL_LPUART_IsEnabledIT_RXBF(LPUART0))
		&&(SET == LL_LPUART_IsActiveFlag_RXBF(LPUART0)))//接收数据匹配中断
	{
  
	}
	
	//发送中断处理
	if((ENABLE == LL_LPUART_IsEnabledIT_TXSE(LPUART0))
		&&(SET == LL_LPUART_IsActiveFlag_TXSE(LPUART0)))
	{
		//清除发送中断标志
        LL_LPUART_ClearFlag_TXSE(LPUART0);    
	}
}

void LPUART1_IRQHandler(void)
{
	uint8_t temp;
	
	//接收中断处理
	if((ENABLE == LL_LPUART_IsEnabledIT_RXBF(LPUART1))
		&&(SET == LL_LPUART_IsActiveFlag_RXBF(LPUART1)))//接收数据匹配中断
	{
        temp = LL_LPUART_ReceiveData(LPUART1);
        task_reset(typeMs, 50, Uart0Overtime);
        CMD_Recv_Buff.Recvbuff[CMD_Recv_Buff.p_write++] = temp;
        if(CMD_Recv_Buff.p_write>=USART_BUFF_SIZE)
            CMD_Recv_Buff.p_write = 0;
#ifdef TOUCHUAN       
		
        UART0->TXBUF = temp;
#endif
	}
	
	//发送中断处理
	if((ENABLE == LL_LPUART_IsEnabledIT_TXSE(LPUART1))
		&&(SET == LL_LPUART_IsActiveFlag_TXSE(LPUART1)))
	{
		//发送指定长度的数据

	}
}

void LPUART_Init(LPUART_Type* LPUARTx)
{
    LL_LPUART_InitTypeDef  InitStructer = {0};
    LL_GPIO_InitTypeDef  GPIO_InitStruct = {0};
    volatile uint8_t tmp08;
      
    if(LPUARTx==LPUART0)
    {
        GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;              //数字功能
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;     //推挽输出
        GPIO_InitStruct.Pull = ENABLE;                            //接收使能上拉
        GPIO_InitStruct.RemapPin = ENABLE;                        //使能额外数字功能
        LL_GPIO_Init(GPIOA,&GPIO_InitStruct);                     //PA13
        
        GPIO_InitStruct.Pin = LL_GPIO_PIN_14;                     
        GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;               //数字功能
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;      //推挽输出
        GPIO_InitStruct.Pull = DISABLE;                            //发送不上拉
        GPIO_InitStruct.RemapPin = ENABLE;                         //使能额外数字功能    
        LL_GPIO_Init(GPIOA,&GPIO_InitStruct);                      //PA14
    }
    if(LPUARTx==LPUART1)
    {
        GPIO_InitStruct.Pin = LL_GPIO_PIN_13;                           
        GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;                //数字功能
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;       //推挽输出
        GPIO_InitStruct.Pull = ENABLE;                              //接收使能上拉
        GPIO_InitStruct.RemapPin = ENABLE;                          //使能额外数字功能  
        LL_GPIO_Init(GPIOB,&GPIO_InitStruct);                       //PB13
      
        GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;                //数字功能
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;       //推挽输出
        GPIO_InitStruct.Pull = DISABLE;                             //发送不上拉
        GPIO_InitStruct.RemapPin = ENABLE;                          //使能额外数字功能  
        LL_GPIO_Init(GPIOB,&GPIO_InitStruct);                       //PB14
    }
    InitStructer.ClockSrc 			= LL_RCC_LPUART_OPERTION_CLOCK_SOURCE_LSCLK;  //时钟源LSCLK
    InitStructer.BaudRate            = LL_LPUART_BAUDRATE_9600;  //波特率9600
	  InitStructer.DataWidth           = LL_LPUART_DATAWIDTH_8B;   //8位数据位
	  InitStructer.StopBits            = LL_LPUART_STOP_1BITS;     //1位停止位
	  InitStructer.Parity              = LL_LPUART_PARITY_NONE ;   //偶校验
	  InitStructer.TransferDirection   = LL_LPUART_DIRECTION_TX_RX;   //使能发送和接收
    LPUARTx->CSR |= LPUART_CSR_RXEV;                                //接收唤醒
    LL_LPUART_Init(LPUARTx,&InitStructer);     
    LL_LPUART_EnabledIT_RXBF(LPUARTx);    //打开接收中断
    if(LPUARTx==LPUART1)
    {
        NVIC_DisableIRQ(LPUART1_IRQn);
        NVIC_SetPriority(LPUART1_IRQn,2);//中断优先级配置
        NVIC_EnableIRQ(LPUART1_IRQn);     //使能NVIC
    }
    if(LPUARTx==LPUART0)
    {
        NVIC_DisableIRQ(LPUART0_IRQn);
        NVIC_SetPriority(LPUART0_IRQn,2);//中断优先级配置
        NVIC_EnableIRQ(LPUART0_IRQn);     //使能NVIC
    }
}

