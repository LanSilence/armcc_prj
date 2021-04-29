#include "lpuart.h"
#include "main.h"
#include "user_init.h"



void LPUART0_IRQHandler(void)
{	
	//�����жϴ���
	if((ENABLE == LL_LPUART_IsEnabledIT_RXBF(LPUART0))
		&&(SET == LL_LPUART_IsActiveFlag_RXBF(LPUART0)))//��������ƥ���ж�
	{
  
	}
	
	//�����жϴ���
	if((ENABLE == LL_LPUART_IsEnabledIT_TXSE(LPUART0))
		&&(SET == LL_LPUART_IsActiveFlag_TXSE(LPUART0)))
	{
		//��������жϱ�־
        LL_LPUART_ClearFlag_TXSE(LPUART0);    
	}
}

void LPUART1_IRQHandler(void)
{
	uint8_t temp;
	
	//�����жϴ���
	if((ENABLE == LL_LPUART_IsEnabledIT_RXBF(LPUART1))
		&&(SET == LL_LPUART_IsActiveFlag_RXBF(LPUART1)))//��������ƥ���ж�
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
	
	//�����жϴ���
	if((ENABLE == LL_LPUART_IsEnabledIT_TXSE(LPUART1))
		&&(SET == LL_LPUART_IsActiveFlag_TXSE(LPUART1)))
	{
		//����ָ�����ȵ�����

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
        GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;              //���ֹ���
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;     //�������
        GPIO_InitStruct.Pull = ENABLE;                            //����ʹ������
        GPIO_InitStruct.RemapPin = ENABLE;                        //ʹ�ܶ������ֹ���
        LL_GPIO_Init(GPIOA,&GPIO_InitStruct);                     //PA13
        
        GPIO_InitStruct.Pin = LL_GPIO_PIN_14;                     
        GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;               //���ֹ���
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;      //�������
        GPIO_InitStruct.Pull = DISABLE;                            //���Ͳ�����
        GPIO_InitStruct.RemapPin = ENABLE;                         //ʹ�ܶ������ֹ���    
        LL_GPIO_Init(GPIOA,&GPIO_InitStruct);                      //PA14
    }
    if(LPUARTx==LPUART1)
    {
        GPIO_InitStruct.Pin = LL_GPIO_PIN_13;                           
        GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;                //���ֹ���
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;       //�������
        GPIO_InitStruct.Pull = ENABLE;                              //����ʹ������
        GPIO_InitStruct.RemapPin = ENABLE;                          //ʹ�ܶ������ֹ���  
        LL_GPIO_Init(GPIOB,&GPIO_InitStruct);                       //PB13
      
        GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
        GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;                //���ֹ���
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;       //�������
        GPIO_InitStruct.Pull = DISABLE;                             //���Ͳ�����
        GPIO_InitStruct.RemapPin = ENABLE;                          //ʹ�ܶ������ֹ���  
        LL_GPIO_Init(GPIOB,&GPIO_InitStruct);                       //PB14
    }
    InitStructer.ClockSrc 			= LL_RCC_LPUART_OPERTION_CLOCK_SOURCE_LSCLK;  //ʱ��ԴLSCLK
    InitStructer.BaudRate            = LL_LPUART_BAUDRATE_9600;  //������9600
	  InitStructer.DataWidth           = LL_LPUART_DATAWIDTH_8B;   //8λ����λ
	  InitStructer.StopBits            = LL_LPUART_STOP_1BITS;     //1λֹͣλ
	  InitStructer.Parity              = LL_LPUART_PARITY_NONE ;   //żУ��
	  InitStructer.TransferDirection   = LL_LPUART_DIRECTION_TX_RX;   //ʹ�ܷ��ͺͽ���
    LPUARTx->CSR |= LPUART_CSR_RXEV;                                //���ջ���
    LL_LPUART_Init(LPUARTx,&InitStructer);     
    LL_LPUART_EnabledIT_RXBF(LPUARTx);    //�򿪽����ж�
    if(LPUARTx==LPUART1)
    {
        NVIC_DisableIRQ(LPUART1_IRQn);
        NVIC_SetPriority(LPUART1_IRQn,2);//�ж����ȼ�����
        NVIC_EnableIRQ(LPUART1_IRQn);     //ʹ��NVIC
    }
    if(LPUARTx==LPUART0)
    {
        NVIC_DisableIRQ(LPUART0_IRQn);
        NVIC_SetPriority(LPUART0_IRQn,2);//�ж����ȼ�����
        NVIC_EnableIRQ(LPUART0_IRQn);     //ʹ��NVIC
    }
}

