/**
  ****************************************************************************************************
  * @file    fm33lc0xx_ll_uart.c
  * @author  FMSH Application Team
  * @brief   Src file of UART LL Module
  ****************************************************************************************************
  * @attention
  *
  * Copyright (c) [2019] [Fudan Microelectronics]
  * THIS SOFTWARE is licensed under the Mulan PSL v1.
  * can use this software according to the terms and conditions of the Mulan PSL v1.
  * You may obtain a copy of Mulan PSL v1 at:
  * http://license.coscl.org.cn/MulanPSL
  * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
  * PURPOSE.
  * See the Mulan PSL v1 for more details.
  *
  ****************************************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "fm33lc0xx_ll_uart.h"
#include "fm33lc0xx_ll_rcc.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0xx_LL_Driver_UART
  * @{
  */
  
/* Private macros ------------------------------------------------------------*/
/** @addtogroup UART_LL_Private_Macros
  * @{
  */

                                                          
#define         IS_UART_INSTANCE(INSTANCE)              (((INSTANCE) == UART0)||\
                                                         ((INSTANCE) == UART1)||\
                                                         ((INSTANCE) == UART4)||\
                                                         ((INSTANCE) == UART5))
 
#define         IS_LL_UART_CLKSRC(__VALUE__)            (((__VALUE__) == LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1)||\
                                                         ((__VALUE__) == LL_RCC_UART_OPERATION_CLOCK_SOURCE_RCHF)||\
                                                         ((__VALUE__) == LL_RCC_UART_OPERATION_CLOCK_SOURCE_SYSCLK)||\
                                                         ((__VALUE__) == LL_RCC_UART_OPERATION_CLOCK_SOURCE_RC4M_PSC))                                                             
                                                             
#define         IS_LL_UART_BAUDRATE(__VALUE__)            (((__VALUE__) <= 460800))
                                                             
#define         IS_LL_UART_DATAWIDTH(__VALUE__)           (((__VALUE__) == LL_UART_DATAWIDTH_6B)||\
                                                           ((__VALUE__) == LL_UART_DATAWIDTH_7B)||\
                                                           ((__VALUE__) == LL_UART_DATAWIDTH_8B)||\
                                                           ((__VALUE__) == LL_UART_DATAWIDTH_9B))
                                                      
#define         IS_LL_UART_STOPBITS(__VALUE__)            (((__VALUE__) == LL_UART_STOPBITS_1)||\
                                                           ((__VALUE__) == LL_UART_STOPBITS_2))
                                                             
#define         IS_LL_UART_PARITY(__VALUE__)              (((__VALUE__) == LL_UART_PARITY_NONE)||\
                                                           ((__VALUE__) == LL_UART_PARITY_EVEN)||\
                                                           ((__VALUE__) == LL_UART_PARITY_ODD))
                                                             
#define         IS_LL_UART_DIRECTION(__VALUE__)           (((__VALUE__) == LL_UART_DIRECTION_NONE)||\
                                                           ((__VALUE__) == LL_UART_DIRECTION_RX)||\
                                                           ((__VALUE__) == LL_UART_DIRECTION_TX)||\
                                                           ((__VALUE__) == LL_UART_DIRECTION_TX_RX)) 
                                                             
#define         IS_LL_UART_INFRA_MODULATION(__VALUE__)    (((__VALUE__) == DISABLE)||\
                                                           ((__VALUE__) == ENABLE))


#define         IS_LL_UART_INFRARED_POLARITY(__VALUE__)        (((__VALUE__) == LL_UART_INFRARED_POLARITY_POSITIVE)||\
                                                                ((__VALUE__) == LL_UART_INFRARED_POLARITY_NEGATIVE))
                                                             
#define         IS_LL_UART_INFRARED_MODULATION_DUTY(__VALUE__) (((__VALUE__) <= 16))
                                                                
                                                             
#define         IS_LL_UART_INFRARED_MODULATION_FREQ(__VALUE__) (((__VALUE__) > 0))                                                           
/**
  * @}
  */
  
/** @addtogroup UART_LL_EF_Init
  * @{
  */

/**
  * @brief	??????UART ??????????????????????????????
  * @param	??????????????????
  * @retval	?????????????????????????????????
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */
ErrorStatus LL_UART_DeInit(UART_Type *UARTx)
{
    ErrorStatus status = PASS;

    /* ??????????????????????????? */
    assert_param(IS_UART_INSTANCE(UARTx));
	/* ?????????????????? */
	LL_RCC_EnablePeripheralReset();
    if (UARTx == UART0)
    {
		/*??????UART*/
        LL_RCC_EnableResetAPB1Peripheral(LL_RCC_APB2_PERIPHERAL_RST_UART0);
        LL_RCC_DisableResetAPB1Peripheral(LL_RCC_APB2_PERIPHERAL_RST_UART0);
        /* ???????????????????????? */
        LL_RCC_Group3_DisableBusClock(LL_RCC_BUS3_CLOCK_UART0);
        /* ???????????????????????? */
        LL_RCC_Group1_DisableOperationClock(LL_RCC_OPERATION1_CLOCK_UART0);  
    }
    else if (UARTx == UART1)
    {
		/*??????UART*/
        LL_RCC_EnableResetAPB2Peripheral(LL_RCC_APB2_PERIPHERAL_RST_UART1);
        LL_RCC_DisableResetAPB2Peripheral(LL_RCC_APB2_PERIPHERAL_RST_UART1);
		/* ???????????????????????? */
        LL_RCC_Group3_DisableBusClock(LL_RCC_BUS3_CLOCK_UART1);
		/* ???????????????????????? */
        LL_RCC_Group1_DisableOperationClock(LL_RCC_OPERATION1_CLOCK_UART1);
    }
    else if (UARTx == UART4)
    {
		/*??????UART*/
        LL_RCC_EnableResetAPB2Peripheral(LL_RCC_APB1_PERIPHERAL_RST_UART4);
        LL_RCC_DisableResetAPB2Peripheral(LL_RCC_APB1_PERIPHERAL_RST_UART4);
		/* ???????????????????????? */
        LL_RCC_Group3_DisableBusClock(LL_RCC_BUS3_CLOCK_UART4);
    }
    else if (UARTx == UART5)
    {
		/*??????UART*/
        LL_RCC_EnableResetAPB2Peripheral(LL_RCC_APB1_PERIPHERAL_RST_UART5);
        LL_RCC_DisableResetAPB2Peripheral(LL_RCC_APB1_PERIPHERAL_RST_UART5);
		/* ???????????????????????? */
        LL_RCC_Group3_DisableBusClock(LL_RCC_BUS3_CLOCK_UART5);
    }
    else
    {
        status = FAIL;
    }
    /* ???????????????????????? */
    LL_RCC_DisablePeripheralReset();
    return (status);
}

/**
  * @brief	????????????????????????UART????????? 
  *               
  * @param	UARTx  ??????????????????
  * @param 	UART_InitStruct????????????LL_UART_InitTypeDef????????????????????????????????????UART?????????????????????	  
  *         
  * @retval	ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS UART????????????        
  */
ErrorStatus LL_UART_Init(UART_Type *UARTx, LL_UART_InitTypeDef *UART_InitStruct)
{
    ErrorStatus status = FAIL;
    uint32_t Fclk,BaudRate;

	/* ????????????????????? */
    assert_param(IS_UART_INSTANCE(UARTx));
    assert_param(IS_LL_UART_CLKSRC(UART_InitStruct->ClockSrc));
    assert_param(IS_LL_UART_DATAWIDTH(UART_InitStruct->DataWidth));
    assert_param(IS_LL_UART_BAUDRATE(UART_InitStruct->BaudRate));
    assert_param(IS_LL_UART_PARITY(UART_InitStruct->Parity));
    assert_param(IS_LL_UART_STOPBITS(UART_InitStruct->StopBits));
    assert_param(IS_LL_UART_DIRECTION(UART_InitStruct->TransferDirection));
    assert_param(IS_LL_UART_INFRA_MODULATION(UART_InitStruct->InfraredModulation));

    if(UARTx == UART0 || UARTx == UART1)
    {
        /*???????????????*/
        LL_RCC_SetUARTClockSource(UARTx,UART_InitStruct->ClockSrc);
        /* ??????????????????????????????baudrate ???????????????????????? */
        switch(UART_InitStruct->ClockSrc)
        {
            case LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1:
                Fclk = LL_RCC_GetAPB1ClockFreq(LL_RCC_GetAHBClockFreq(LL_RCC_GetSystemClockFreq()));
                break;    
            case LL_RCC_UART_OPERATION_CLOCK_SOURCE_RCHF:
                Fclk = LL_RCC_GetRCHFClockFreq();
                break;
            case LL_RCC_UART_OPERATION_CLOCK_SOURCE_SYSCLK:
                Fclk = LL_RCC_GetSystemClockFreq();
                break;
            case LL_RCC_UART_OPERATION_CLOCK_SOURCE_RC4M_PSC:
                Fclk = LL_RCC_GetRC4MClockFreq();
                break;
        }
        BaudRate = Fclk/UART_InitStruct->BaudRate - 1;
    }
    if(UARTx == UART0)
    {
        /*??????????????????*/
        LL_RCC_Group3_EnableBusClock(LL_RCC_BUS3_CLOCK_UART0);
		/*??????????????????*/
        LL_RCC_Group1_EnableOperationClock(LL_RCC_OPERATION1_CLOCK_UART0);  
    }
    else if(UARTx == UART1)
    {
        /*??????????????????*/
        LL_RCC_Group3_EnableBusClock(LL_RCC_BUS3_CLOCK_UART1);
		/*??????????????????*/
        LL_RCC_Group1_EnableOperationClock(LL_RCC_OPERATION1_CLOCK_UART1);
    }
    else if(UARTx == UART4)
    {
        /*??????????????????*/
        LL_RCC_Group3_EnableBusClock(LL_RCC_BUS3_CLOCK_UART4);
        Fclk = LL_RCC_GetAPB2ClockFreq(LL_RCC_GetAHBClockFreq(LL_RCC_GetSystemClockFreq()));
        BaudRate = Fclk/UART_InitStruct->BaudRate - 1;
        
    }
    else if(UARTx == UART5)
    {
        /*??????????????????*/
        LL_RCC_Group3_EnableBusClock(LL_RCC_BUS3_CLOCK_UART5);
        Fclk = LL_RCC_GetAPB2ClockFreq(LL_RCC_GetAHBClockFreq(LL_RCC_GetSystemClockFreq()));
        BaudRate = Fclk/UART_InitStruct->BaudRate - 1;
    }
	/*??????????????????*/
    if(UART_InitStruct->TransferDirection&LL_UART_DIRECTION_TX)
    {
        LL_UART_EnableDirectionTx(UARTx);
    }
    if(UART_InitStruct->TransferDirection&LL_UART_DIRECTION_RX)
    {
        LL_UART_EnableDirectionRx(UARTx);
    }
    if(UART_InitStruct->InfraredModulation == ENABLE)
    {
        /*??????????????????*/
        LL_UART_Enable_InfraredModulation(UARTx);
    }
    /*???????????????*/
    LL_UART_SetBaudRate(UARTx,BaudRate);
	/*???????????????*/
    LL_UART_SetStopBitsLength(UARTx,UART_InitStruct->StopBits);
	/*??????????????????*/
    LL_UART_SetDataWidth(UARTx,UART_InitStruct->DataWidth);
	/*???????????????*/
    LL_UART_SetParity(UARTx,UART_InitStruct->Parity);
	
    status = PASS;
    return status;
}
/**
  * @brief	????????????????????????????????????????????? 
  *               
  * @param	UARTx  ??????????????????
  *
  * @param 	UART_InitStruct????????????LL_UART_InitTypeDef????????????????????????????????????UART?????????????????????	  
  *         
  * @retval	ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS UART????????????        
  */
ErrorStatus LL_UART_InfraRed_Init(UART_Type *UARTx, LL_UART_InfraRed_InitTypeDef *InfraRed_InitStruct)
{
    ErrorStatus status = FAIL;
    uint32_t  tempTZBRG,tempTH;

	/* ????????????????????? */
    assert_param(IS_UART_INSTANCE(UARTx));
    assert_param(IS_LL_UART_INFRARED_POLARITY(InfraRed_InitStruct->Polarity));
    
    LL_RCC_Group3_EnableBusClock(LL_RCC_BUS3_CLOCK_UARTIR);
    /*??????????????????*/
    LL_UART_SetInfraredPolarity(UART_COMMON,InfraRed_InitStruct->Polarity);
    /*??????????????????*/
    tempTZBRG = (uint32_t)((LL_RCC_GetAPB1ClockFreq(LL_RCC_GetAHBClockFreq(LL_RCC_GetSystemClockFreq()))*1.0)/InfraRed_InitStruct->ModulationFrequency -1);
    /* ??????????????? */
    if((tempTZBRG>>4) != 0)
		{
			
			tempTH = (uint32_t)(((float)InfraRed_InitStruct->ModulationDuty/100.0)*((float)(tempTZBRG + 1)/(float)(tempTZBRG>>4)) + 0.5);
		}
		else
		{
			tempTH = (uint32_t)(((float)InfraRed_InitStruct->ModulationDuty/100.0)*(float)(tempTZBRG + 1) + 0.5);
			
		}
    /* ?????????????????????????????? */    
    LL_UART_SetInfraredModulationDuty(UART_COMMON,tempTH);
    LL_UART_SetInfraredModulationFrequency(UART_COMMON,tempTZBRG);
    
    status = PASS;
    return status;
}

/**
  * @brief	UART_InitStruct ???????????????
  * @param 	UART_InitStruct ??????????????????????????????????????????????????? @ref LL_UART_InitTypeDef structure 
  *         ?????????
  * @retval	None
  */
void LL_UART_InfraRed_StructInit(LL_UART_InfraRed_InitTypeDef *InfraRed_InitStruct)
{
	InfraRed_InitStruct->Polarity                  = LL_UART_INFRARED_POLARITY_NEGATIVE;//LL_UART_INFRARED_POLARITY_POSITIVE;
	InfraRed_InitStruct->ModulationDuty            = 50;
	InfraRed_InitStruct->ModulationFrequency       = 38000;
}
/**
  * @brief	UART_InitStruct ???????????????
  * @param 	UART_InitStruct ??????????????????????????????????????????????????? @ref LL_UART_InitTypeDef structure 
  *         ?????????
  * @retval	None
  */
void LL_UART_StructInit(LL_UART_InitTypeDef *UART_InitStruct)
{
	UART_InitStruct->BaudRate            = 9600;
	UART_InitStruct->DataWidth           = LL_UART_DATAWIDTH_8B;
	UART_InitStruct->StopBits            = LL_UART_STOPBITS_1;
	UART_InitStruct->Parity              = LL_UART_PARITY_NONE ;
	UART_InitStruct->TransferDirection   = LL_UART_DIRECTION_TX_RX;
	UART_InitStruct->InfraredModulation  = DISABLE;
    UART_InitStruct->ClockSrc            = LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;
}
/**
  * @}
  */

/**
  * @}
  */

/*************************************************************END OF FILE************************************************************/

