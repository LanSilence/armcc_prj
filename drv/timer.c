#include "timer.h"
#include "main.h"
#include "task_manage.h"




void BSTIM_IRQHandler(void)
{
    if (( LL_BSTIM_IsEnabledIT_UpdataEvent(BSTIM) == SET ) &&(LL_BSTIM_IsActiveFlag_UpdataEvent(BSTIM)==SET))
    {	 
        LL_BSTIM_ClearFlag_UpdataEvent(BSTIM);    
        task_Mstimer();
    }			
}

//lptimer, 4ms��ʱ���ж�
void Init_lptim(void)
{
	LL_LPTIM_TimeInitTypeDef  LPTIM_InitStruct={0};
	LPTIM_InitStruct.PrescalerClockSource = LL_LPTIM_PRESCALER_INPUT_LPTIM_CLK;   //ʱ������ѡ��CLKSEL��ʱ��
	LPTIM_InitStruct.ClockSource = LL_RCC_LPTIM_OPERATION_CLOCK_SOURCE_LSCLK;     //CLKSELʱ��Դѡ��LSCLK
	LPTIM_InitStruct.Prescaler = LL_LPTIM_CLOCKDIVISION_DIV1;                     //����ʱ�Ӳ���Ƶ
	LPTIM_InitStruct.Mode = LL_LPTIM_OPERATION_MODE_NORMAL;                       //��ͨ��ʱ��ģʽ
	LPTIM_InitStruct.OneState = LL_LPTIM_ONE_STATE_TIMER_CONTINUE;                //��������ģʽ
	LL_LPTIM_TimeModeInit(LPTIM,&LPTIM_InitStruct);

	LL_LPTIM_SetAutoReload(LPTIM,16128);    //����Ŀ��ֵ
	LL_LPTIM_ClearFlag_CounterOver(LPTIM);   //��������־

	//LL_LPTIM_EnableIT_CounterOver(LPTIM);   //����ж�ʹ��
	//NVIC_DisableIRQ(LPTIM_IRQn);
	//NVIC_SetPriority(LPTIM_IRQn,2);//�����ж����ȼ�
	//NVIC_EnableIRQ(LPTIM_IRQn);    //ʹ��NVIC

	//LL_LPTIM_Enable(LPTIM);   //ʹ��LPTIM	
}


//������ʱ��������ms��������ļ�ʱ
void Init_basetim()
{
    LL_BSTIM_InitTypeDef   InitStructer;
    
    InitStructer.Prescaler         = 79;
    InitStructer.Autoreload        = 99;    
    InitStructer.AutoreloadState   = ENABLE;
    InitStructer.ClockSource       = LL_RCC_BSTIM_OPERATION_CLK_SOURCE_APBCLK2;
  
    LL_BSTIM_Init(BSTIM,&InitStructer);
    
    
  
    NVIC_DisableIRQ(BSTIM_IRQn);
    NVIC_SetPriority(BSTIM_IRQn, 2);
    NVIC_EnableIRQ(BSTIM_IRQn);
    
    LL_BSTIM_ClearFlag_UpdataEvent(BSTIM);
    LL_BSTIM_EnabledIT_UpdataEvent(BSTIM);
  
    LL_BSTIM_EnableCounter(BSTIM);
}



