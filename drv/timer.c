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

//lptimer, 4ms定时器中断
void Init_lptim(void)
{
	LL_LPTIM_TimeInitTypeDef  LPTIM_InitStruct={0};
	LPTIM_InitStruct.PrescalerClockSource = LL_LPTIM_PRESCALER_INPUT_LPTIM_CLK;   //时钟输入选择CLKSEL的时钟
	LPTIM_InitStruct.ClockSource = LL_RCC_LPTIM_OPERATION_CLOCK_SOURCE_LSCLK;     //CLKSEL时钟源选择LSCLK
	LPTIM_InitStruct.Prescaler = LL_LPTIM_CLOCKDIVISION_DIV1;                     //计数时钟不分频
	LPTIM_InitStruct.Mode = LL_LPTIM_OPERATION_MODE_NORMAL;                       //普通定时器模式
	LPTIM_InitStruct.OneState = LL_LPTIM_ONE_STATE_TIMER_CONTINUE;                //连续计数模式
	LL_LPTIM_TimeModeInit(LPTIM,&LPTIM_InitStruct);

	LL_LPTIM_SetAutoReload(LPTIM,16128);    //设置目标值
	LL_LPTIM_ClearFlag_CounterOver(LPTIM);   //清除溢出标志

	//LL_LPTIM_EnableIT_CounterOver(LPTIM);   //溢出中断使能
	//NVIC_DisableIRQ(LPTIM_IRQn);
	//NVIC_SetPriority(LPTIM_IRQn,2);//设置中断优先级
	//NVIC_EnableIRQ(LPTIM_IRQn);    //使能NVIC

	//LL_LPTIM_Enable(LPTIM);   //使能LPTIM	
}


//基本定时器，用于ms级别任务的计时
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



