#include "main.h"

//��ʱʱ��4000us,
void LPTIM_IRQHandler(void)
{
	//unsigned char Ch;

	LL_LPTIM_ClearFlag_CounterOver(LPTIM);       //�����־     

	
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
	//�����жϴ���
	if((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART1))
		&&(SET == LL_UART_IsActiveFlag_RXBF(UART1)))
	{
		//�ж�ת�����յ�������
		Temp08 = LL_UART_ReceiveData(UART1);//�����жϱ�־����ͨ����ȡrxreg�Ĵ������
		//Judge_RX_Frame( CH485, Temp08 );

        UART0->TXBUF = Temp08;

	}
	//�����жϴ���	
	if( (ENABLE == LL_UART_IsEnbaledIT_ShiftBuffEmpty(UART1))
		&&(SET == LL_UART_IsActiveFlag_TXSE(UART1)) ) //Transmit
	{
		LL_UART_ClearFlag_TXSE(UART1);//��������жϱ�־
	}
	
	ClearClosedIF(UART1);//�����־
}


void UART0_IRQHandler(void)
{
	uint08 Temp;
	//�����жϴ���
	if((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART0))
		&&(SET == LL_UART_IsActiveFlag_RXBF(UART0)))
	{
		//�ж�ת�����յ�������
		Temp = LL_UART_ReceiveData(UART0);//�����жϱ�־����ͨ����ȡrxreg�Ĵ������
		//Judge_RX_Frame( CHZB, Temp );
        NB_Recv_Buff.Recvbuff[NB_Recv_Buff.p_write++] = Temp;
        if(NB_Recv_Buff.p_write >= USART_BUFF_SIZE)
            NB_Recv_Buff.p_write = 0;
#ifdef TOUCHUAN
        UART1->TXBUF = Temp;
#endif        
        
        //task_reset(typeMs,50000,Uart0Overtime);
	}

	//�����жϴ���
	if((ENABLE == LL_UART_IsEnbaledIT_ShiftBuffEmpty(UART0))
		&&(SET == LL_UART_IsActiveFlag_TXSE(UART0)))
	{
		LL_UART_ClearFlag_TXSE(UART0);	//��������жϱ�־
	}
	
	ClearClosedIF(UART0);//�����־
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


//SVD�ж�
void SVD_IRQHandler(void)
{
    
	if(SET == LL_SVD_IsActiveFlag_PowerFallFlag(SVD) )//Ƿѹ�ж�
	{
		LL_PMU_ClearFlag_PowerFallFlag(SVD);
        power_change = 1;
        
        print("��ѹ�仯 Power_Unnormal\n");
	}
	else if(SET == LL_SVD_IsActiveFlag_PowerRiseFlag(SVD))//��ѹ�ж�
	{
		LL_PMU_ClearFlag_PowerRiseFlag(SVD);
        //SVD->ISR |= SVD_ISR_PFF;
        power_change = 1;
        DeviceParam.PowerState.CurrentState = Power_Normal;
        print("��ѹ�仯 Power_Normal\n");

	}
}


void LFDET_IRQHandler(void)
{
    while(1);
}
