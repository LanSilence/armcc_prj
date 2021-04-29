#include "bsp.h"
#include "main.h"


void All_Output()
{

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10 | LL_GPIO_PIN_13| LL_GPIO_PIN_14;
    /*****GPIOA输出低*****/
    LL_GPIO_ResetOutputPin(GPIOA, GPIO_InitStruct.Pin);
    
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = DISABLE;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /********************/
    
    /*****GPIOB输出低*****/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3 | LL_GPIO_PIN_8 | \
                          LL_GPIO_PIN_9 | LL_GPIO_PIN_11 | LL_GPIO_PIN_10 | LL_GPIO_PIN_13| LL_GPIO_PIN_14;
    LL_GPIO_ResetOutputPin(GPIOB, GPIO_InitStruct.Pin);
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /*****GPIOC输出低*****/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2| LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 |  LL_GPIO_PIN_8 | \
                          LL_GPIO_PIN_9 | LL_GPIO_PIN_10 ;
    LL_GPIO_ResetOutputPin(GPIOC, GPIO_InitStruct.Pin);
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    /*****GPIOD输出低*****/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0| LL_GPIO_PIN_1 | LL_GPIO_PIN_11;
    LL_GPIO_ResetOutputPin(GPIOD, GPIO_InitStruct.Pin);
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
}

//模拟开关
void Allpin_ANEN(FunState en)
{
    if(en == DISABLE)
        GPIOA->ANEN = en;
        
}

void AllPin_Output_Low()
{
    All_Output();
    Allpin_ANEN(DISABLE);
    GPIO_COMMON->EXTIDF = 0;
    GPIO_COMMON->EXTIEDS = ~(uint32_t)0;
}



