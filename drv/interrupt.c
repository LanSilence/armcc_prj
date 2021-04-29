#include "main.h"

//定时时间4000us,
void LPTIM_IRQHandler(void)
{
	//unsigned char Ch;

	LL_LPTIM_ClearFlag_CounterOver(LPTIM);       //清除标志     

	
}

void ClearClosedIF(UART_Type* UARTx)
{
	if( (DISABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UARTx))
		&&(SET == LL_UART_IsActiveFlag_RXBF(UARTx)) )
	{	
		LL_UART_ReceiveData(UARTx);
	}
	if( (DISABLE == LL_UART_IsEnbaledIT_ShiftBuffEmpty(UARTx))
		&&(SET == LL_UART_IsActiveFlag_TXSE(UARTx)) )
	{
		LL_UART_ClearFlag_TXSE(UARTx);
	}
}


//485
void UART1_IRQHandler(void)
{
	uint08 Temp08;

	//CH485
	//接收中断处理
	if((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART1))
		&&(SET == LL_UART_IsActiveFlag_RXBF(UART1)))
	{
		//中断转发接收到的数据
		Temp08 = LL_UART_ReceiveData(UART1);//接收中断标志仅可通过读取rxreg寄存器清除
		//Judge_RX_Frame( CH485, Temp08 );

        UART0->TXBUF = Temp08;

	}
	//发送中断处理	
	if( (ENABLE == LL_UART_IsEnbaledIT_ShiftBuffEmpty(UART1))
		&&(SET == LL_UART_IsActiveFlag_TXSE(UART1)) ) //Transmit
	{
		LL_UART_ClearFlag_TXSE(UART1);//清除发送中断标志
	}
	
	ClearClosedIF(UART1);//清除标志
}


void UART0_IRQHandler(void)
{
	uint08 Temp;
	//接收中断处理
	if((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART0))
		&&(SET == LL_UART_IsActiveFlag_RXBF(UART0)))
	{
		//中断转发接收到的数据
		Temp = LL_UART_ReceiveData(UART0);//接收中断标志仅可通过读取rxreg寄存器清除
		//Judge_RX_Frame( CHZB, Temp );
        NB_Recv_Buff.Recvbuff[NB_Recv_Buff.p_write++] = Temp;
        if(NB_Recv_Buff.p_write >= USART_BUFF_SIZE)
            NB_Recv_Buff.p_write = 0;
#ifdef TOUCHUAN
        UART1->TXBUF = Temp;
#endif        
        
        //task_reset(typeMs,50000,Uart0Overtime);
	}

	//发送中断处理
	if((ENABLE == LL_UART_IsEnbaledIT_ShiftBuffEmpty(UART0))
		&&(SET == LL_UART_IsActiveFlag_TXSE(UART0)))
	{
		LL_UART_ClearFlag_TXSE(UART0);	//清除发送中断标志
	}
	
	ClearClosedIF(UART0);//清除标志
}

void GPIO_IRQHandler(void)
{
    if((LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_9) == SET) &&\
       (LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_6) == SET))
    {
        LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_9);
        LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_6);
        return;
    }
    else if(LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_9) == SET)
    {
        //print("A\n");
        LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_9);
        Deal_IRDA1();
    }   
    else if(LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_6) == SET)
    {
        //print("B\n");
        LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_6);
        Deal_IRDA2();
    }
}


//SVD中断
void SVD_IRQHandler(void)
{
    
	if(SET == LL_SVD_IsActiveFlag_PowerFallFlag(SVD) )//欠压中断
	{
		LL_PMU_ClearFlag_PowerFallFlag(SVD);
        power_change = 1;
        
        print("电压变化 Power_Unnormal\n");
	}
	else if(SET == LL_SVD_IsActiveFlag_PowerRiseFlag(SVD))//过压中断
	{
		LL_PMU_ClearFlag_PowerRiseFlag(SVD);
        //SVD->ISR |= SVD_ISR_PFF;
        power_change = 1;
        DeviceParam.PowerState.CurrentState = Power_Normal;
        print("电压变化 Power_Normal\n");

	}
}


void LFDET_IRQHandler(void)
{
    while(1);
}
