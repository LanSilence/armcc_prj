#include "main.h"




uint8_t power_change;


void Init_SVD(void)
{
	SVD_InitTypeDef   SVD_InitStruct={0};
	
	SVD_InitStruct.Mode            = LL_SVD_WORK_MODE_ALWAYS;
	SVD_InitStruct.Interval        = LL_SVD_INTERVEL_ENABLE_PERIOD_62MS;
	SVD_InitStruct.Threshold       = LL_SVD_WARNING_THRESHOLD_LEVEL_STANDARD_GTOUP12;
	SVD_InitStruct.SVSChannel      = DISABLE;
	SVD_InitStruct.VrefSelect      = LL_SVD_REFERENCE_VOLTAGE_1_2_VOLT;
	SVD_InitStruct.DigitalFilter   = DISABLE;
    
	LL_SVD_Init(SVD,&SVD_InitStruct);
    
	LL_PMU_ClearFlag_PowerRiseFlag(SVD);
	LL_PMU_ClearFlag_PowerFallFlag(SVD);
	//LL_SVD_EnableITPowerRise(SVD);
	//LL_SVD_EnableITPowerFall(SVD);
    //不需要中断，按分钟或者秒去查询
	//NVIC_DisableIRQ(SVD_IRQn);			//NVIC中断控制器配置
	//NVIC_SetPriority(SVD_IRQn,2);
	//NVIC_EnableIRQ(SVD_IRQn);	
    
	LL_SVD_EnableSVD(SVD); 
	
}


void delay_process_power()
{
    if(DeviceParam.PowerState.PreState != DeviceParam.PowerState.CurrentState)
    {
        DeviceParam.PowerState.PreState = DeviceParam.PowerState.CurrentState;
        if(DeviceParam.PowerState.CurrentState == Power_Unnormal)
        {
            power_change = 1;
        }
    }
    task_stop(delay_process_power);
}


void power_check_overtime()
{
    //LL_SVD_DisableSVD(SVD); 
    task_stop(power_check_overtime);
}

void Power_Check()
{

    //LL_SVD_EnableSVD(SVD);
    //Init_SVD();
//    task_start(typeMs,500,power_check_overtime);
    if(SVD->ISR & (1U << 8))
    {
        DeviceParam.PowerState.CurrentState = Power_Normal;
        
    }
    else
    {
        DeviceParam.PowerState.CurrentState = Power_Unnormal;
    }
    if(Up_process.UpState != S_Halt)
    { 
       return;
    }
    
    if(DeviceParam.PowerState.PreState != DeviceParam.PowerState.CurrentState)  //状态不同
    {
       DeviceParam.PowerState.PreState = DeviceParam.PowerState.CurrentState;
        print("电压变化: %u\n异常1，正常0\n",DeviceParam.PowerState.CurrentState);
       if(DeviceParam.PowerState.CurrentState == Power_Unnormal)
       {
           StartUp(Type_Alarm);
       }
    }

}
