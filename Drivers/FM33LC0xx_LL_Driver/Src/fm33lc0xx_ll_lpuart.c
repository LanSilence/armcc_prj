/**
  ****************************************************************************************************
  * @file    fm33lC0xx_ll_lpuart.c
  * @author  FMSH Application Team
  * @brief   Src file of LPUART LL Module
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
#include "fm33lc0xx_ll_lpuart.h"
#include "fm33lc0xx_ll_rcc.h"
#include "fm33_assert.h"
/** @addtogroup FM33LC0xx_LL_Driver_LPUART
  * @{
  */
  
/* Private macros ------------------------------------------------------------*/
/** @addtogroup LPUART_LL_Private_Macros
  * @{
  */
#define         IS_LPUART_INSTANCE(INSTANCE)               (((INSTANCE) == LPUART0)||\
                                                             ((INSTANCE) == LPUART1))
                                                             
#define         IS_LL_LPUART_CLKSRC(__VALUE__)             (((__VALUE__) == LL_RCC_LPUART_OPERTION_CLOCK_SOURCE_LSCLK)||\
                                                            ((__VALUE__) == LL_RCC_LPUART_OPERTION_CLOCK_SOURCE_RCHF)||\
                                                            ((__VALUE__) == LL_RCC_LPUART_OPERTION_CLOCK_SOURCE_RC4M))                                                            
                                                             
#define         IS_LL_LPUART_BAUDRATE(__VALUE__)            (((__VALUE__) == LL_LPUART_BAUDRATE_300)||\
                                                             ((__VALUE__) == LL_LPUART_BAUDRATE_600)||\
                                                             ((__VALUE__) == LL_LPUART_BAUDRATE_1200)||\
                                                             ((__VALUE__) == LL_LPUART_BAUDRATE_2400)||\
                                                             ((__VALUE__) == LL_LPUART_BAUDRATE_4800)||\
                                                             ((__VALUE__) == LL_LPUART_BAUDRATE_9600))
                                                             
#define         IS_LL_LPUART_DATAWIDTH(__VALUE__)           (((__VALUE__) == LL_LPUART_DATAWIDTH_6B)||\
                                                             ((__VALUE__) == LL_LPUART_DATAWIDTH_7B)||\
                                                             ((__VALUE__) == LL_LPUART_DATAWIDTH_8B)||\
                                                             ((__VALUE__) == LL_LPUART_DATAWIDTH_9B))
                                                      
#define         IS_LL_LPUART_STOPBITS(__VALUE__)            (((__VALUE__) == LL_LPUART_STOP_1BITS)||\
                                                             ((__VALUE__) == LL_LPUART_STOP_2BITS))
                                                             
#define         IS_LL_LPUART_PARITY(__VALUE__)              (((__VALUE__) == LL_LPUART_PARITY_NONE)||\
                                                             ((__VALUE__) == LL_LPUART_PARITY_EVEN)||\
                                                             ((__VALUE__) == LL_LPUART_PARITY_ODD))
                                                             
#define         IS_LL_LPUART_DIRECTION(__VALUE__)           (((__VALUE__) == LL_LPUART_DIRECTION_NONE)||\
                                                             ((__VALUE__) == LL_LPUART_DIRECTION_RX)||\
                                                             ((__VALUE__) == LL_LPUART_DIRECTION_TX)||\
                                                             ((__VALUE__) == LL_LPUART_DIRECTION_TX_RX)) 
                                                             
/**
  * @}
  */
  
/** @addtogroup LPUART_LL_EF_Init
  * @{
  */

/**
  * @brief	??????LPUART ??????
  * @param	??????????????????
  * @retval	?????????????????????????????????
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */
ErrorStatus LL_LPUART_DeInit(LPUART_Type *LPUARTx)
{
    ErrorStatus status = PASS;

    /* ??????????????????????????? */
    assert_param(IS_LPUART_INSTANCE(LPUARTx));
	/* ?????????????????? */
	LL_RCC_EnablePeripheralReset();
    if (LPUARTx == LPUART0)
    {
		/*??????LPUART*/
        LL_RCC_EnableResetAPB1Peripheral(LL_RCC_APB1_PERIPHERAL_RST_LPUART0);
        LL_RCC_DisableResetAPB1Peripheral(LL_RCC_APB1_PERIPHERAL_RST_LPUART0);
        /* ???????????????????????? */
        LL_RCC_Group3_DisableBusClock(LL_RCC_BUS3_CLOCK_LPUART0);
        /* ???????????????????????? */
        LL_RCC_Group1_DisableOperationClock(LL_RCC_OPERATION1_CLOCK_LPUART0);  
    }
    else if (LPUARTx == LPUART1)
    {
		/*??????LPUART*/
        LL_RCC_EnableResetAPB2Peripheral(LL_RCC_APB2_PERIPHERAL_RST_LPUART1);
        LL_RCC_DisableResetAPB2Peripheral(LL_RCC_APB2_PERIPHERAL_RST_LPUART1);
		/* ???????????????????????? */
        LL_RCC_Group3_DisableBusClock(LL_RCC_BUS3_CLOCK_LPUART1);
		/* ???????????????????????? */
        LL_RCC_Group1_DisableOperationClock(LL_RCC_OPERATION1_CLOCK_LPUART1);
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
  * @brief	????????????????????????LPUART????????? 
  *         
  * @note 	??????????????????????????????MCTL??????????????????????????????32768Hz?????????????????????????????????????????????????????????????????????  
  *      	???????????????????????????????????????????????????????????????????????????????????????      
  * @param	LPUARTx  ??????????????????
  * @param 	LPUART_InitStruct???????????? @ref LL_LPUART_InitTypeDef????????????????????????????????????LPUART?????????????????????	  
  *         
  * @retval	ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS LPUART????????????        
  */
ErrorStatus LL_LPUART_Init(LPUART_Type *LPUARTx, LL_LPUART_InitTypeDef *LPUART_InitStruct)
{
    ErrorStatus status = FAIL;
    uint16_t  MCTLVel;

	/* ????????????????????? */
    assert_param(IS_LPUART_INSTANCE(LPUARTx));
    assert_param(IS_LL_LPUART_CLKSRC(LPUART_InitStruct->ClockSrc));
    assert_param(IS_LL_LPUART_BAUDRATE(LPUART_InitStruct->BaudRate));
    assert_param(IS_LL_LPUART_DATAWIDTH(LPUART_InitStruct->DataWidth));
    assert_param(IS_LL_LPUART_STOPBITS(LPUART_InitStruct->StopBits));
    assert_param(IS_LL_LPUART_PARITY(LPUART_InitStruct->Parity));
    assert_param(IS_LL_LPUART_DIRECTION(LPUART_InitStruct->TransferDirection));
    if(LPUARTx == LPUART0)
    {
        /*??????????????????*/
        LL_RCC_Group3_EnableBusClock(LL_RCC_BUS3_CLOCK_LPUART0);
		/*??????????????????*/
        LL_RCC_Group1_EnableOperationClock(LL_RCC_OPERATION1_CLOCK_LPUART0);
    }
    else
    {
        /*??????????????????*/
        LL_RCC_Group3_EnableBusClock(LL_RCC_BUS3_CLOCK_LPUART1);
		/*??????????????????*/
        LL_RCC_Group1_EnableOperationClock(LL_RCC_OPERATION1_CLOCK_LPUART1);
    }
    /*???????????????*/
    LL_RCC_SetLPUARTClockSource(LPUARTx,LPUART_InitStruct->ClockSrc);
	/*??????????????????*/
    if(LPUART_InitStruct->TransferDirection&LL_LPUART_DIRECTION_TX)
    {
        do
        {
          LL_LPUART_EnabledTx(LPUARTx);
        }while(LL_LPUART_IsEnabledTx(LPUARTx)!= SET);
    }
    if(LPUART_InitStruct->TransferDirection&LL_LPUART_DIRECTION_RX)
    {
      do{
        LL_LPUART_EnabledRx(LPUARTx);
      }while(LL_LPUART_IsEnabledRx(LPUARTx)!= SET);
    }
    /*???????????????*/
    LL_LPUART_SetBaudRate(LPUARTx,LPUART_InitStruct->BaudRate);
	/*???????????????*/
    LL_LPUART_SetStopBitsLength(LPUARTx,LPUART_InitStruct->StopBits);
	/*??????????????????*/
    LL_LPUART_SetDataWidth(LPUARTx,LPUART_InitStruct->DataWidth);
	/*???????????????*/
    LL_LPUART_SetParity(LPUARTx,LPUART_InitStruct->Parity);
	/*?????????????????????MCTL???*/
    switch(LPUART_InitStruct->BaudRate)
    {
        case LL_LPUART_BAUDRATE_9600:MCTLVel = 0x0552;break;
        case LL_LPUART_BAUDRATE_4800:MCTLVel = 0x1EFB;break;
        case LL_LPUART_BAUDRATE_2400:MCTLVel = 0x16DB;break;
        case LL_LPUART_BAUDRATE_1200:MCTLVel = 0x0492;break;
        case LL_LPUART_BAUDRATE_600:MCTLVel = 0x16D6;break;
        case LL_LPUART_BAUDRATE_300:MCTLVel = 0x0842;break;
    }
    LL_LPUART_SetMCTLVel(LPUARTx,MCTLVel);
    status = PASS;
    return status;
}

/**
  * @brief	LPUART_InitStruct ???????????????
  * @param 	LPUART_InitStruct ??????????????????????????????????????????????????? @ref LL_LPUART_InitTypeDef structure 
  *         ?????????
  * @retval	None
  */

void LL_LPUART_StructInit(LL_LPUART_InitTypeDef *LPUART_InitStruct)
{
	LPUART_InitStruct->BaudRate            = LL_LPUART_BAUDRATE_9600;
	LPUART_InitStruct->DataWidth           = LL_LPUART_DATAWIDTH_8B;
	LPUART_InitStruct->StopBits            = LL_LPUART_STOP_1BITS;
	LPUART_InitStruct->Parity              = LL_LPUART_PARITY_NONE ;
	LPUART_InitStruct->TransferDirection   = LL_LPUART_DIRECTION_TX_RX;
    LPUART_InitStruct->ClockSrc            = LL_RCC_LPUART_OPERTION_CLOCK_SOURCE_LSCLK;

}
/**
  * @}
  */

/**
  * @}
  */

/*************************************************************END OF FILE************************************************************/

