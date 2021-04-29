#include "IRDA_Device.h"

IRDA_DEVICE_STRUCT IRDA_Struct = {0};
uint32_t Get122VSample;
TRIG_STRUCT Trigger;

void IRDA_ADC_Init()
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_ADC_CommonInitTypeDef    ADC_CommonInitStruct = {0};
    
    /*****GPIOA输出低*****/
    //LL_GPIO_ResetOutputPin(GPIO, GPIO_InitStruct.Pin);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG; //模拟
    GPIO_InitStruct.Pull = DISABLE;
    GPIO_InitStruct.RemapPin = DISABLE;   
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    

    
    
    //ADC 时钟设置
	ADC_CommonInitStruct.AdcClockSource    = LL_RCC_ADC_OPERATION_CLOCK_PRESCALLER_RCHF; //RCHF
	ADC_CommonInitStruct.AdcClockPrescaler = LL_RCC_ADC_OPERATION_CLOCK_PRESCALER_DIV8; //16分频
	LL_ADC_CommonInit(&ADC_CommonInitStruct);
    
    
    ADC_InitStruct.ADC_ContinuousConvMode  = LL_ADC_CONV_SINGLE;//单次模式
    ADC_InitStruct.ADC_AutoMode            = LL_ADC_SINGLE_CONV_MODE_AUTO;//全自动
    ADC_InitStruct.ADC_ScanDirection       = LL_ADC_SEQ_SCAN_DIR_FORWARD;//通道正序扫描
    ADC_InitStruct.ADC_ExternalTrigConv    = LL_ADC_EXT_TRIGGER_NONE;//禁止触发信号
    ADC_InitStruct.ADC_OverrunMode 		   	= LL_ADC_OVR_DATA_PRESERVED;//覆盖上次数据
    ADC_InitStruct.ADC_WaitMode 		       = LL_ADC_WAIT_MODE_WAIT;//等待 
    ADC_InitStruct.ADC_SamplingStartControl = LL_ADC_SAMPLING_START_CONTROL_BY_REG;//由START寄存器启动ADC采样
    ADC_InitStruct.ADC_SamplingTimeControl = LL_ADC_SAMPLING_TIME_CONTROL_BY_REG;//由寄存器控制ADC采样时间
    ADC_InitStruct.ADC_Channel_Swap_Wiat   = LL_ADC_SAMPLEING_INTERVAL_11_CYCLES;//通道切换等待时间
    ADC_InitStruct.ADC_Channel_Fast_Time   = LL_ADC_FAST_CH_SAMPLING_TIME_4_ADCCLK;//快速通道采样时间
    ADC_InitStruct.ADC_Channel_Slow_Time   = LL_ADC_SLOW_CH_SAMPLING_TIME_192_ADCCLK;//慢速通道采样时间
    ADC_InitStruct.ADC_Oversampling        = DISABLE;//过采样关闭
    ADC_InitStruct.ADC_OverSampingRatio    = LL_ADC_OVERSAMPLING_8X;//8倍过采样
    ADC_InitStruct.ADC_OversamplingShift   = LL_ADC_OVERSAMPLING_RESULT_DIV8;//数据右移, /8	
    LL_ADC_Init(ADC, &ADC_InitStruct);
    
    LL_RCC_SetADCPrescaler(LL_RCC_ADC_OPERATION_CLOCK_PRESCALER_DIV1);
	
    
    Get122VSample = GetVref1p22Sample();
    LL_ADC_EnalbleSequencerChannel(ADC, LL_ADC_EXTERNAL_CH_1);//通道选择ADC_1
	LL_ADC_EnalbleSequencerChannel(ADC, LL_ADC_EXTERNAL_CH_0);//通道选择ADC_0
    
    //LL_ADC_EnableIT_EOC(ADC);
//    NVIC_DisableIRQ(ADC_IRQn);				//NVIC中断控制器配置
//	NVIC_SetPriority(ADC_IRQn,3);
//	NVIC_EnableIRQ(ADC_IRQn);
    //IO_IRDA_A_OUTPUT;
    //IO_IRDA_A_LOW;
    LL_ADC_EnableDMATransfer(ADC);

}

//温度传感器初始化
void Temp_Init()
{
    
}

float Get_Temprature()
{
    uint32_t ADC_CHER;   //临时存储寄存器值
    uint32_t i = 0x1ff;
    uint32_t TempData = 0;
    uint64_t VPTAT;
    uint32_t VPTAT_30;
    float Temprature = 0;
    
    LL_VREF_EnableVPTATBuffer(VREF);
    LL_RCC_SetADCPrescaler(LL_RCC_ADC_OPERATION_CLOCK_PRESCALER_DIV8);
    ADC_CHER = ADC->CHER;
    ADC->CHER = 0x01<<16; //关闭其他  选择温度传感
    //LL_ADC_EnalbleSequencerChannel(ADC, LL_ADC_INTERNAL_CH_TEMPSENSOR)
    LL_ADC_Enable(ADC);
    LL_ADC_StartConversion(ADC);  // 开始转换
    
    while(LL_ADC_IsActiveFlag_EOC(ADC) == 0 && i != 0)
    {
        i--;  //防止超时
    }
    LL_ADC_ClearFlag_EOC(ADC);//清标志
    //VPTAT = (GetTSample *3000*(ADC_VREF))/(Get122VSample*4095); 
    
    TempData = LL_ADC_ReadConversionData12(ADC);
    VPTAT = (uint64_t)((uint64_t)TempData *3000*(ADC_VREF))/(Get122VSample*4095); 
	VPTAT_30 = ((TS_CAL>>16) *3000)/(4095); 
	Temprature =  (float)(VPTAT*1.0 - VPTAT_30*1.0)/(2.97)+30-0.5;	
    
    print("temp = %.2f\r\n",Temprature);
    
    LL_ADC_Disable(ADC);
    LL_VREF_DisableVPTATBuffer(VREF);//关闭PTAT BUFFER
    ADC->CHER = ADC_CHER;//将原寄存器值放回
    return Temprature;
        
}

void ADC_IRQHandler(void)
{
	if(LL_ADC_IsEnabledIT_EOC(ADC) && LL_ADC_IsActiveFlag_EOC(ADC))
	{
		LL_ADC_ClearFlag_EOC(ADC);//清标志	
    
	}
    
    if(LL_ADC_IsEnabledIT_EOS(ADC) && LL_ADC_IsActiveFlag_EOS(ADC))
    {
        LL_ADC_ClearFlag_EOS(ADC);//清标志	
    }

}


//根据例程需要获取VREF的电压值  用于计算后面ADC采样的电压
uint32_t GetVref1p22Sample(void)
{
    uint16_t i = 0x1fff;
	uint16_t ADCRdresult;
	LL_RCC_SetADCPrescaler(LL_RCC_ADC_OPERATION_CLOCK_PRESCALER_DIV8);
    LL_VREF_EnableVREFBuffer(VREF);//使能VREF BUFFER
	LL_ADC_EnalbleSequencerChannel(ADC, LL_ADC_INTERNAL_CH_VREF);//通道选择VREF
	LL_ADC_DisableSequencerChannel(ADC,LL_ADC_EXTERNAL_CH_1);
    LL_ADC_DisableSequencerChannel(ADC,LL_ADC_EXTERNAL_CH_0);  //关闭其他通道
    
    LL_DMA_Disable_DMA(DMA);        //  DMA关闭
	LL_ADC_ClearFlag_EOC(ADC);//清标志			
    LL_ADC_Enable(ADC);   	 // 启动ADC
	LL_ADC_StartConversion(ADC);  //开始转换
    // 等待转换完成
     LL_ADC_IsActiveFlag_EOC(ADC);
    while ( LL_ADC_IsActiveFlag_EOC(ADC) == RESET)
    {
        i--;
        if(i==0)
        {
           break;    //超时退出
        }  
    }
    LL_ADC_ClearFlag_EOC(ADC);//清标志
	ADCRdresult =LL_ADC_ReadConversionData12(ADC);//获取采样值

    LL_ADC_Disable(ADC);    // 关闭ADC
	LL_ADC_DisableSequencerChannel(ADC, LL_ADC_INTERNAL_CH_VREF);//通道关闭VREF	
    LL_VREF_DisableVREFBuffer(VREF);//关闭VREF BUFFER	
    LL_DMA_Enable_DMA(DMA);    
    LL_ADC_EnalbleSequencerChannel(ADC, LL_ADC_EXTERNAL_CH_1);//通道选择ADC_1
	LL_ADC_EnalbleSequencerChannel(ADC, LL_ADC_EXTERNAL_CH_0);//通道选择ADC_0
    // 转换结果 
    if(i==0)
        return 0x043F;   //超时则默认电源为4.5V的值
    return ADCRdresult;
}

void ADC_DMA(uint16_t *buffer, uint32_t length)
{
    LL_DMA_InitTypeDef DMA_InitStruct={0};
		
		DMA_InitStruct.PeriphAddress = LL_DMA_PERIPHERAL_FUNCTION1;	
		DMA_InitStruct.MemoryAddress = (uint32_t)buffer;		
		DMA_InitStruct.Direction = LL_DMA_DIR_PERIPHERAL_TO_RAM;	
		DMA_InitStruct.MemoryAddressIncMode = LL_DMA_INCREMENTAL_INCREASE;	
		DMA_InitStruct.DataSize = LL_DMA_BAND_WIDTH_HALF_WORD;
		DMA_InitStruct.NbData = length - 1;	
		DMA_InitStruct.Priority = LL_DMA_CHANNEL_PRIORITY_HIGH;		
		DMA_InitStruct.CircMode = DISABLE;
		LL_DMA_Init(DMA, &DMA_InitStruct, LL_DMA_CHANNEL_4);		
				
		LL_DMA_Enable_DMA(DMA);     
		LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_4);
		LL_DMA_Enable_Channel(DMA, LL_DMA_CHANNEL_4);    
}

void IRDA_Init()
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*****GPIOC4配置*****/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = ENABLE;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /********************/
    RCC->OPCCR1 |= LL_RCC_OPERATION1_CLOCK_EXTI;
    LL_GPIO_SetExitLine9(GPIO_COMMON, LL_GPIO_EXTI_LINE_9_PC4);    //选择PC4
    LL_EXTI_DisableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_9);//数字滤波
    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_9, LL_GPIO_EXTI_TRIGGER_BOTHEDGE);  //上升沿触发
    //LL_RCC_Group1_EnableBusClock(LL_RCC_OPERATION1_CLOCK_EXTI);
    
    
    
    
    GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = ENABLE;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /********************/
    LL_GPIO_SetExitLine6(GPIO_COMMON, LL_GPIO_EXTI_LINE_6_PB8);    //选择PB8
    LL_EXTI_DisableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_6);//数字滤波
    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_6, LL_GPIO_EXTI_TRIGGER_BOTHEDGE);  //上升沿触发
    

    IRDA_ADC_Init();
    
    
    
    NVIC_DisableIRQ(GPIO_IRQn);				//NVIC中断控制器配置
	NVIC_SetPriority(GPIO_IRQn,2);
	NVIC_EnableIRQ(GPIO_IRQn);
    
}

uint16_t ADCdata[200];   //存储转换完成的数据

//不停检测ADC是否转换完成
void Detect_ADC()
{
    
	LL_ADC_ClearFlag_EOC(ADC);//清标志			
   // 等待转换完成

	
    if(IRDA_Struct.IsADCStart == 1)
    {
           
        if(LL_DMA_IsActiveFlag_Finished(DMA, LL_DMA_CHANNEL_4))
        {
            
            LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_4);
            ADC_DMA(ADCdata + IRDA_Struct.IRDA_VotageCount,2);
            LL_ADC_Enable(ADC);
            LL_ADC_StartConversion(ADC);  // 开始转换

            if(IRDA_Struct.IRDA_VotageCount < 198)
               IRDA_Struct.IRDA_VotageCount +=2;

        }
     
    }
        
}

void Deal_IRDA1(void)
{
    if(Trigger.count<MAX_TRIG_TIMES)
        Trigger.TriggerBuf[Trigger.count++] ='A';
    if(IRDA_Struct.IsADCStart == 0)
    {
        IRDA_Struct.IsADCStart = 1;
        ADC_DMA(ADCdata + IRDA_Struct.IRDA_VotageCount,2);
        LL_ADC_Enable(ADC);   	        // 启动ADC
        LL_ADC_StartConversion(ADC);    // 开始转换        	
        task_reset(typeMs,50,Detect_ADC);
        //IRDA_Struct.Direct[0] = A2B_DIR;
    }
    task_reset(typeMs,1000,Judge_Direction);   //延迟1200
}

void Deal_IRDA2(void)
{
    if(Trigger.count<MAX_TRIG_TIMES)
        Trigger.TriggerBuf[Trigger.count++] ='B';
    if(IRDA_Struct.IsADCStart == 0)
    {
        IRDA_Struct.IsADCStart = 1;
        ADC_DMA(ADCdata + IRDA_Struct.IRDA_VotageCount,2);
        LL_ADC_Enable(ADC);   	                // 启动ADC
        LL_ADC_StartConversion(ADC);            // 开始转换        	
        task_reset(typeMs,50,Detect_ADC);
        //IRDA_Struct.Direct[0] = B2A_DIR;
    }
    task_reset(typeMs,1000,Judge_Direction);
}


//将电压值经过求和后进行处理
/*
uint8_t Prossce_Votage(void)
{
    uint8_t step = 0,i;
    uint8_t result = 0;
    uint64_t sumresult = 0;
    
    for(i = 0;i<IRDA_Struct.IRDA_VotageCount;i+=2)
    {
        IRDA_Struct.ADCRdresult_B = ( (uint64_t)ADCdata[i] *3000*(ADC_VREF))/(Get122VSample*4095);
        IRDA_Struct.ADCRdresult_A = ( (uint64_t)ADCdata[i+1] *3000*(ADC_VREF))/(Get122VSample*4095);
        sumresult = IRDA_Struct.ADCRdresult_B + IRDA_Struct.ADCRdresult_A;
        if(step == 0)
        {    
            #if 0        
            if(IRDA_Struct.ADCRdresult_A > 2900 && IRDA_Struct.ADCRdresult_B < 2200)   //先判断谁先被触发
            {
                IRDA_Struct.Direct[result] = A2B_DIR;
            }
            else if(IRDA_Struct.ADCRdresult_B > 2900 && IRDA_Struct.ADCRdresult_A < 2200)
            {
                IRDA_Struct.Direct[result] = B2A_DIR;
            }
            if(sumresult>5500 && result<1)//同时进出的情况
            {
                IRDA_Struct.Direct[result] = BOTH_DIR;
                step++;   
            }
            if(IRDA_Struct.Direct[result]!=ABNONE && sumresult < 100 && result <= Support_Num - 1)  //同时低电平
            {
                result++;   //获取到一个信号
                step++;    //下一步
            }
            
        }
        if(step == 1)
        {
            if(sumresult>3000 && IRDA_Struct.Direct[result] != BOTH_DIR)
                step = 0;
            
            if(IRDA_Struct.Direct[result] == BOTH_DIR && sumresult < 100 && result <= Support_Num - 1)
            {
                step ++;
                //result++;
            }
        }
        if(step == 2)
        {
            if(IRDA_Struct.Direct[result] == BOTH_DIR && sumresult >= 4000 && result <= Support_Num - 1)
            {
                step=0;
                result++;
            }
        }
        #else
            if(IRDA_Struct.ADCRdresult_A < 100 && IRDA_Struct.ADCRdresult_B > 1300)   //先判断谁先被触发
            {
                IRDA_Struct.Direct[result] = A2B_DIR;
            }
            else if(IRDA_Struct.ADCRdresult_B < 100 && IRDA_Struct.ADCRdresult_A > 1300)
            {
                IRDA_Struct.Direct[result] = B2A_DIR;
            }
            
            if(sumresult <200 && result<1)//同时进出的情况
            {
                IRDA_Struct.Direct[result] = BOTH_DIR;
                step++;   
            }
            else if(sumresult <1000 && result>=1)   //第二个接着进入的情况
            {
                if(IRDA_Struct.ADCRdresult_A < IRDA_Struct.ADCRdresult_B && IRDA_Struct.ADCRdresult_A < 100)
                {
                    IRDA_Struct.Direct[result] = A2B_DIR;
                }
                else if(IRDA_Struct.ADCRdresult_A > IRDA_Struct.ADCRdresult_B && IRDA_Struct.ADCRdresult_B < 100)
                {
                    IRDA_Struct.Direct[result] = B2A_DIR;
                }
            }
            
            if(IRDA_Struct.Direct[result]!=ABNONE && sumresult > 5500 && result <= Support_Num - 1)  //同时高电平
            {
                result++;   //获取到一个信号
                step++;    //下一步
            }
            
        }
        if(step == 1)
        {
            if(sumresult < 2000 && IRDA_Struct.Direct[result] != BOTH_DIR)
                step = 0;
            
            if(IRDA_Struct.Direct[result] == BOTH_DIR && sumresult > 5500 && result <= Support_Num - 1)
            {
                step ++;
                //result++;
            }
        }
        if(step == 2)
        {
            if(IRDA_Struct.Direct[result] == BOTH_DIR && sumresult < 2000 && result <= Support_Num - 1)
            {
                step=0;
                result++;
            }
        }    
        #endif
        print("电压B：%lld\n",IRDA_Struct.ADCRdresult_B);
        
        print("电压A：%lld\n",IRDA_Struct.ADCRdresult_A);
        
    }Uart1SendData(Trigger.TriggerBuf,Trigger.count);
        print("\n");
    return result;
}
  
*/
//分别获取AB两个电压的低点和高点进行判断
uint8_t Prossce_Votage(void)
{
    uint8_t step = 0,i;
    uint8_t result = 0;
    uint8_t ABVolt[10] = {0};
    uint8_t flaga=0xff,flagb=0xff;
    for(i = 0;i<IRDA_Struct.IRDA_VotageCount;i+=2)
    {
        IRDA_Struct.ADCRdresult_B = ( (uint64_t)ADCdata[i] *3000*(ADC_VREF))/(Get122VSample*4095);
        IRDA_Struct.ADCRdresult_A = ( (uint64_t)ADCdata[i+1] *3000*(ADC_VREF))/(Get122VSample*4095);
        if(step == 0)
        {
            if(IRDA_Struct.ADCRdresult_A<Low_level)
                flaga = 1;
            else if(IRDA_Struct.ADCRdresult_A>High_Level)
                flaga = 0;
            
            if(IRDA_Struct.ADCRdresult_B<Low_level)
                flagb = 1;
            else if(IRDA_Struct.ADCRdresult_B>High_Level)
                flagb = 0;
            step++;
            continue;
        }
        if(step == 1 && result<10)
        {
            if(IRDA_Struct.ADCRdresult_A<Low_level && flaga == 1)
            {
                flaga = 0;
                ABVolt[result++] = 'a';
            }
            else if(IRDA_Struct.ADCRdresult_A>High_Level && flaga == 0)
            {
                flaga = 1;
                ABVolt[result++] = 'A';
            }
            else if(flaga == 0xff)
            {
                if(IRDA_Struct.ADCRdresult_A<Low_level)
                {
                    flaga = 0;
                    ABVolt[result++] = 'a';
                }
                else if(IRDA_Struct.ADCRdresult_A>High_Level)
                {
                    flaga = 1;
                    ABVolt[result++] = 'A';
                }
            }
            
            if(IRDA_Struct.ADCRdresult_B<Low_level && flagb==1)
            {
                flagb = 0;
                ABVolt[result++] = 'b';
            }
            else if(IRDA_Struct.ADCRdresult_B>High_Level && flagb==0)
            {
                flagb = 1;
                ABVolt[result++] = 'B';
            }
            else if(flagb == 0xff)
            {
                if(IRDA_Struct.ADCRdresult_B<Low_level)
                {
                    flagb = 0;
                    ABVolt[result++] = 'b';
                }
                else if(IRDA_Struct.ADCRdresult_B>High_Level)
                {
                    flagb = 1;
                    ABVolt[result++] = 'B';
                }
            }
        }
        else if(result>=10)
            break;
        
        //print("电压B：%lld\n",IRDA_Struct.ADCRdresult_B);
        
        //print("电压A：%lld\n",IRDA_Struct.ADCRdresult_A);
    }
    if(ABVolt[0] == 0)
        return 0;
    
    uint8_t ABTotalNum = result;
    result = 0;
    Uart1SendData(ABVolt,ABTotalNum);
        print("\n");
        Uart1SendData(Trigger.TriggerBuf,Trigger.count);
        print("\n");
    if(((ABVolt[0] == 'a' && ABVolt[1] == 'b') || (ABVolt[0] == 'b' && ABVolt[1] == 'a')) && ABTotalNum>3)
    {
            IRDA_Struct.Direct[result++] = BOTH_DIR;
            return result;
    }
    
   
        
    step = 0;
    if(ABTotalNum<2 && memcmp(Trigger.TriggerBuf,"AAAAAAAAAAAA",Trigger.count) && memcmp(Trigger.TriggerBuf,"BBBBBBBBBBB",Trigger.count))  //只有当ABTotalNum小于7的时候才用AB触发判断
    {
        if (Trigger.TriggerBuf[0] == 'A')
            IRDA_Struct.Direct[result++] = A2B_DIR; 
        else if(Trigger.TriggerBuf[0] == 'B')
            IRDA_Struct.Direct[result++] = B2A_DIR; 
          return result;  
    }
    #if 0
    for(i=0;i<Trigger.count;i++)
    {
        
        
        if(step == 0)
        {
            if(Trigger.count <= 3)
            {
                if(memcmp(ABVolt,"aBb",3)==0)
                {
                    IRDA_Struct.Direct[result++] = A2B_DIR;    //速度太快也许只触发三次 ，具体要看电压变化
                    break;
                }
            }
            if(Trigger.TriggerBuf[i] == 'A' )
            {
                //IRDA_Struct.Direct[result++] = A2B_DIR;
                temp = A2B_DIR;   //第一个是A可以确定是A到B
            }
            else if(Trigger.TriggerBuf[i] == 'B')
            {
                //IRDA_Struct.Direct[result++] = B2A_DIR;
                temp = B2A_DIR;   //第一个是B可以确定是B到A
            }
            step++;
            continue;
        }
        else if(step == 1)
        {
            if(temp == A2B_DIR && Trigger.TriggerBuf[i] == 'B')  //如果是A到B 中间必须要有一个B
            {
                //IRDA_Struct.Direct[result++] = A2B_DIR;
                step = 2;  
            }
            else if(temp == B2A_DIR && Trigger.TriggerBuf[i] == 'A')
            {
                //IRDA_Struct.Direct[result++] = B2A_DIR;
                step = 2;
            }
        }
        else if(step == 2)
        {
            if(temp == A2B_DIR && Trigger.TriggerBuf[i] == 'A')  //再次有A则结束一轮寻找
            {
                IRDA_Struct.Direct[result++] = A2B_DIR;
                step = 0;
            }
            else if(temp == B2A_DIR && Trigger.TriggerBuf[i] == 'B')
            {
                IRDA_Struct.Direct[result++] = B2A_DIR;
                step = 0;
            }
            if(result !=0 && Trigger.count <=6)
                break;
        }
        
        #if 0
        if(ABVolt[i]=='a' && i<ABTotalNum-2)
        {
            if(i<ABTotalNum-3 && ABVolt[i+1] == 'A' && ABVolt[i+2] == 'B' )
            {
                IRDA_Struct.Direct[result++] = A2B_DIR;
                i+=2;
            }
            else if(ABVolt[i+1] == 'B')
            {
                i+=1;
                IRDA_Struct.Direct[result++] = A2B_DIR;
            }
            else if(ABVolt[i]=='a' && i<ABTotalNum-2)
            {
                if(ABVolt[i+1]=='b' && ABVolt[i+2]=='B' &&  ABVolt[i+3]=='A')
                {
                    i+=3;
                    IRDA_Struct.Direct[result++] = BOTH_DIR; 
                }
                else if(ABVolt[i+1]=='b' && ABVolt[i+2]=='A' &&  ABVolt[i+3]=='B')
                {
                    i+=3;
                    IRDA_Struct.Direct[result++] = BOTH_DIR; 
                }
            }
        }
        else if(ABVolt[i]=='b')
        {
            if(i<ABTotalNum-2 && ABVolt[i+1] == 'A' && ABVolt[i+2] == 'B' )
            {
                IRDA_Struct.Direct[result++] = B2A_DIR;
                i+=2;
            }
            else if(ABVolt[i+1] == 'A')
            {
                i+=1;
                IRDA_Struct.Direct[result++] = B2A_DIR;
            }
            else if(ABVolt[i]=='b' && i<ABTotalNum-3)
            {
                if(ABVolt[i+1]=='a' && ABVolt[i+2]=='B' &&  ABVolt[i+3]=='A')
                {
                    i+=3;
                    IRDA_Struct.Direct[result++] = BOTH_DIR;
                }
                else if(ABVolt[i+1]=='a' && ABVolt[i+2]=='A' &&  ABVolt[i+3]=='B')
                {
                   i+=3;
                   IRDA_Struct.Direct[result++] = BOTH_DIR; 
                }
            }
        }
        #endif
        
        #if 0 
        if(ABVolt[0] == 'a' && ABVolt[1] != 'b')
        {
            if(ABVolt[i] == 'a')
            {
               IRDA_Struct.Direct[result++] = A2B_DIR; 
            }
        }
        else if(ABVolt[0] == 'b' && ABVolt[1] != 'a')
        {
            if(ABVolt[i] == 'b')
            {
               IRDA_Struct.Direct[result++] = B2A_DIR; 
            }
        }
        else if(ABTotalNum >= 3 && memcmp(ABVolt,"AbB",3) == 0)
        {
            IRDA_Struct.Direct[result++] = B2A_DIR;
            break;
        }
        else if((ABVolt[0] == 'a' && ABVolt[1] == 'b') || (ABVolt[0] == 'b' && ABVolt[1] == 'a'))
        {
            IRDA_Struct.Direct[result++] = BOTH_DIR;
            break;
        }
        else if(ABTotalNum <= 3 && memcmp(ABVolt,"Bb",2) == 0)
        {
            IRDA_Struct.Direct[result++] = A2B_DIR; 
            break;
        }
        else if(ABVolt[0] == 'A')
        {
            if(memcmp(ABVolt,"Aa",2) == 0)
            {
                IRDA_Struct.Direct[result++] = B2A_DIR;
                break;
            }
        }
        #endif
  
    }
    #endif
    if( ABTotalNum>=4)  //大于7或者大于4 并且没有检测到进出 情况则使用电压变化判断
    {
        for(i=0;i<ABTotalNum-1;i++)    //最后一个出现a或者b都不算
        {
            
            if(memcmp(&ABVolt[i],"aAB",3)==0 || memcmp(&ABVolt[i],"aBA",3)==0)
            {
                IRDA_Struct.Direct[result++] = A2B_DIR; 
                i+=3;
            }
            else if(memcmp(&ABVolt[i],"bBA",3) == 0 || memcmp(&ABVolt[i],"bAB",3)==0)
            {
                IRDA_Struct.Direct[result++] = B2A_DIR; 
                i+=3;          
            }
            else
            {
            if(ABVolt[i] == 'A'&& ABVolt[i+1] == 'B')
            {
                IRDA_Struct.Direct[result++] = A2B_DIR;
                i+=2;
            }
            else if(ABVolt[i] == 'B'&& ABVolt[i+1] == 'A')
            {
                IRDA_Struct.Direct[result++] = B2A_DIR;
                i+=2;
            }
            else if(memcmp(&ABVolt[i],"bBa",3)==0 && i<=ABTotalNum-3)
            {
                IRDA_Struct.Direct[result++] = B2A_DIR;
                i+=3;
            }
            else if(memcmp(&ABVolt[i],"bAa",3)==0 && i<=ABTotalNum-3)
            {
                IRDA_Struct.Direct[result++] = B2A_DIR;
                i+=3;
            }
            else if(memcmp(&ABVolt[i],"Aab",3)==0 && i<=ABTotalNum-3)
            {
                IRDA_Struct.Direct[result++] = A2B_DIR;
                i+=3;
            }
            else if(memcmp(&ABVolt[i],"BbA",3)==0 && i<=ABTotalNum-3)
            {
                IRDA_Struct.Direct[result++] = B2A_DIR;
                i+=3;
            }
            else if(memcmp(&ABVolt[i],"AaB",3)==0 && i<=ABTotalNum-3)
            {
                IRDA_Struct.Direct[result++] = A2B_DIR;
                i+=3;
            }
            else if(memcmp(&ABVolt[i],"aAb",3)==0 && i<=ABTotalNum-3)
            {
                IRDA_Struct.Direct[result++] = A2B_DIR;
                i+=2;
            }
            else if(memcmp(&ABVolt[i],"aBb",3)==0 && i<=ABTotalNum-3)
            {
                IRDA_Struct.Direct[result++] = A2B_DIR;
                i+=3;
            }
            else if(memcmp(&ABVolt[i],"Bab",3)==0 && i<=ABTotalNum-3)
            {
                IRDA_Struct.Direct[result++] = B2A_DIR;
                i+=3;
            }
            }
        }
    }
    return result;
}


/*   //简单通过判断电压的变化判定
uint8_t Prossce_Votage(void)
{
    uint8_t step = 0,i;
    uint8_t result = 0;
    
    for(i = 0;i<IRDA_Struct.IRDA_VotageCount;i+=2)
    {
        IRDA_Struct.ADCRdresult_B = ( (uint64_t)ADCdata[i] *3000*(ADC_VREF))/(Get122VSample*4095);
        IRDA_Struct.ADCRdresult_A = ( (uint64_t)ADCdata[i+1] *3000*(ADC_VREF))/(Get122VSample*4095);
        if(i <= 50 && step == 0)
        {
            if(IRDA_Struct.ADCRdresult_A > 2900 && IRDA_Struct.ADCRdresult_B < 2000)
            {
                IRDA_Struct.Direct = A2B_DIR;
            }
            else if(IRDA_Struct.ADCRdresult_B > 2900 && IRDA_Struct.ADCRdresult_A < 2000)
            {
                IRDA_Struct.Direct = B2A_DIR;
            }
            
            if(IRDA_Struct.Direct == A2B_DIR && IRDA_Struct.ADCRdresult_A < 80)
            {
                step++;
            }
            else if(IRDA_Struct.Direct == B2A_DIR && IRDA_Struct.ADCRdresult_B < 80)
            {
                step++;
            }
        }
        
        if(step == 1 && i < 130)
        {
            if(IRDA_Struct.ADCRdresult_B > 2900 && IRDA_Struct.ADCRdresult_A < 80 && IRDA_Struct.Direct == A2B_DIR)
            {
                result = 1;  //结果正确
                step++;
            }
            else if(IRDA_Struct.ADCRdresult_A > 2900 && IRDA_Struct.ADCRdresult_B < 80 && IRDA_Struct.Direct == B2A_DIR)
            {
                result = 1;  //结果正确
                step++;
            }
        }
            print("电压B：%lld\n",IRDA_Struct.ADCRdresult_B);
        
            print("电压A：%lld\n",IRDA_Struct.ADCRdresult_A);
    }
    
    return result;
    
}
*/

void Clear_IRDA()
{
    memset(IRDA_Struct.Direct,ABNONE,sizeof(IRDA_Struct.Direct));
    memset(ADCdata,0,sizeof(ADCdata));
    LL_ADC_Disable(ADC);
    IRDA_Struct.IRDA_VotageCount = 0;
    IRDA_Struct.IsADCStart = 0;
    ShieldCount = 0;                    
    DeviceParam.staus.shield = 0;
    Trigger.count = 0;
    task_stop(Judge_Direction);
    task_stop(Detect_ADC);
}

extern void Write_Freeze_data(DIRECT Dir);
void Judge_Direction(void)
{
    uint8_t i = 0, result = 0;
    
    result = Prossce_Votage();
    if(result)
    {
        for(i = 0;i<result;i++)
        {
            if(IRDA_Struct.Direct[i] == A2B_DIR)
            {
                print("A->B\r\n");
                Write_Freeze_data(A2B_DIR);     //进出事件写入Flash
                DeviceParam.TotalNumber++;
            }    
            else if (IRDA_Struct.Direct[i] == B2A_DIR)
            {
                print("B->A\r\n");
                Write_Freeze_data(B2A_DIR);
                if(DeviceParam.TotalNumber>0)
                    DeviceParam.TotalNumber--;
            }
            else if(IRDA_Struct.Direct[i] == BOTH_DIR)
            {
                Write_Freeze_data(A2B_DIR);
                Write_Freeze_data(B2A_DIR);
                print("A->B\r\n");
                print("B->A\r\n");
            }
        }
       print("人数：%hu\r\n",DeviceParam.TotalNumber); 
       if(DeviceParam.uptime.uptype ==0 && DeviceParam.uptime.interval == 0)   //即时上告
            SendDataToSevice(Type_Realtime);
    }

        
    Clear_IRDA();
}
