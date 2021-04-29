#include "main.h"  
#include "uart.h"
#include "task_manage.h"
#include "drv_nbmodule.h"


#if  ADDR_USERPARAME_END > ALL_FLASH_SIZE

    #error "==> FLASH �ռ�������� <=="

#endif

void NMI_Handler(void)
{
	PMU->WKFR |= PMU_WKFR_DBGWKF_Msk;
}

ErrorStatus IWDT_Init(void)
{
	LL_IWDT_InitTypeDef IWDT_InitStruct;
	/* Ĭ�ϲ�ʹ�ô��� */
	IWDT_InitStruct.IwdtWindows    = 0;
	/*����ʱ��*/
	IWDT_InitStruct.OverflowPeriod = LL_IWDT_IWDT_OVERFLOW_PERIOD_8000MS;
	return LL_IWDT_Init(IWDT, &IWDT_InitStruct);
}

void IWDT_Clr(void)
{
  LL_IWDG_ReloadCounter(IWDT);
}
void running()
{    //AllPin_Output_Low(); 
    //LPUART_Init(LPUART1);
    //IRDA_Init();
    task_stop(running);
}

extern void Write_Freeze_data(DIRECT Dir);
int main (void)
{	
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    /* SHOULD BE KEPT!!! */
    MF_Clock_Init();
    
    /* Configure the system clock */
    /* SHOULD BE KEPT!!! */
    MF_SystemClock_Config();
    
    /* Initialize all configured peripherals */
    /* SHOULD BE KEPT!!! */
    MF_Config_Init();
    
	IWDT_Init();//���Ź�
    UserInit();
    Init_SVD();
	Flash_Read();
	
	
	__enable_irq();	//��ȫ���ж�ʹ��
	/********************************************************************/
	print("  APP Start!\r\n");
#ifdef TOUCHUAN
    touchuang_test();
#else
    StartUp(Type_Ontime);
#endif    
    task_start(typeMs, 30000, running);     

	for( ; ; )
    {
		IWDT_Clr();             //��ϵͳ���Ź�	
        task_schedule();
#ifndef __DEBUG
       EnterSleep();
#endif
    }
}
