/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_opa.c
  * @author  FMSH Application Team
  * @brief   Src file of OPA LL Module
  *******************************************************************************************************
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
  *******************************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "fm33lc0xx_ll_rcc.h"
#include "fm33lc0xx_ll_opa.h"
#include "fm33_assert.h"
/** @addtogroup FM33LC0xx_LL_Driver_OPA
  * @{
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup LPUART_LL_Private_Macros
  * @{
  */


#define         IS_OPA_ALL_INSTANCE(INTENCE)               (((INTENCE) == OPA1)||\
                                                            ((INTENCE) == OPA2))
                                                            
#define         IS_LL_OPA_NIP_CHANNAL(__VALUE__)           (((__VALUE__) == LL_OPA_POSITIVE_INPUT_CHANNEL_OPA_INP1)||\
                                                            ((__VALUE__) == LL_OPA_POSITIVE_INPUT_CHANNEL_OPA_INP2))


#define         IS_LL_OPA_NIN_CHANNAL(__VALUE__)           (((__VALUE__) == LL_OPA_NEGTIVE_INPUT_CHANNEL_OPA_INN1)||\
                                                             ((__VALUE__) == LL_OPA_NEGTIVE_INPUT_CHANNEL_OPA_INN2)||\
                                                             ((__VALUE__) == LL_OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF)||\
                                                             ((__VALUE__) == LL_OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_THREE_QUARTERS)||\
                                                             ((__VALUE__) == LL_OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_HALF)||\
                                                             ((__VALUE__) == LL_OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_ONE_QUARTER)||\
                                                             ((__VALUE__) == LL_OPA_NEGTIVE_INPUT_CHANNEL_OPA_VREF_ONE_EIGHTH))
                                                             
#define         IS_LL_OPA_MODE(__VALUE__)                  (((__VALUE__) == LL_OPA_MODE_STANDLONE)||\
                                                            ((__VALUE__) == LL_OPA_MODE_COMPARISON)||\
                                                            ((__VALUE__) == LL_OPA_MODE_PGA)||\
                                                            ((__VALUE__) == LL_OPA_MODE_BUFF))
                  
                
#define         IS_LL_OPA_DIGITALFILTER(__VALUE__)         (((__VALUE__) == DISABLE)||\
                                                             ((__VALUE__) == ENABLE))
                                                             
#define         IS_LL_OPA_NEGTIVE_TO_PIN(__VALUE__)        (((__VALUE__) == DISABLE)||\
                                                             ((__VALUE__) == ENABLE)) 

#define         IS_LL_OPA_LOW_POWER_MODE(__VALUE__)        (((__VALUE__) == DISABLE)||\
                                                             ((__VALUE__) == ENABLE))

#define         IS_LL_OPA_GAIN(__VALUE__)                  (((__VALUE__) == LL_OPA_GAIN_MUL_2)||\
                                                            ((__VALUE__) == LL_OPA_GAIN_MUL_4)||\
                                                            ((__VALUE__) == LL_OPA_GAIN_MUL_8)||\
                                                            ((__VALUE__) == LL_OPA_GAIN_MUL_16)) 
/**
  * @}
  */
/**
  * @brief	??????OPA ??????????????????????????????
  * @param	??????????????????
  * @retval	?????????????????????????????????
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */
ErrorStatus LL_OPA_DeInit(OPA_Type *OPAx)
{
    ErrorStatus status = PASS;
    /* ??????????????????????????? */
    assert_param(IS_OPA_ALL_INSTANCE(OPAx));
	/* ?????????????????? */
    OPAx->CR = 0x80000E00;
    OPAx->CALR = 0x00000000;
    OPAx->IER = 0x00000000;
    return (status);
}

/**
  * @brief  ?????? OPA_InitStruct.
  * @param  OPAx 
  * @param  OPA_InitStruct ???????????? @ref LL_OPA_InitTypeDef ?????????
  *         ??????????????????????????????????????????.
  * @retval ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS LPUART????????????  
  */
ErrorStatus LL_OPA_Init(OPA_Type *OPAx, LL_OPA_InitTypeDef *OPA_InitStruct)
{
    ErrorStatus status = PASS;

    /* ?????????????????? */
    assert_param(IS_OPA_ALL_INSTANCE(OPAx));
    assert_param(IS_LL_OPA_NIP_CHANNAL(OPA_InitStruct->INP));
    assert_param(IS_LL_OPA_NIN_CHANNAL(OPA_InitStruct->INN));
    assert_param(IS_LL_OPA_MODE(OPA_InitStruct->Mode));
    assert_param(IS_LL_OPA_DIGITALFILTER(OPA_InitStruct->DigitalFilter));
    assert_param(IS_LL_OPA_NEGTIVE_TO_PIN(OPA_InitStruct->NegtiveToPin));
    assert_param(IS_LL_OPA_LOW_POWER_MODE(OPA_InitStruct->LowPowermode));
    assert_param(IS_LL_OPA_GAIN(OPA_InitStruct->Gain));
    /*????????????*/
    LL_OPA_SetOPAMode(OPAx,OPA_InitStruct->Mode);
    /*??????????????????*/
    LL_OPA_SetNegtiveInputChannel(OPAx,OPA_InitStruct->INN);
    /*??????????????????*/
    LL_OPA_SetPositiveInputChannel(OPAx,OPA_InitStruct->INP);
    /*?????????????????????*/
    if(OPA_InitStruct->LowPowermode == ENABLE)
    {
       LL_OPA_EnableLowpowerMode(OPAx); 
    }
    /*???????????????????????????????????????*/
    if(OPA_InitStruct->Mode == LL_OPA_MODE_COMPARISON && OPA_InitStruct->DigitalFilter == ENABLE)
    {
        LL_OPA_EnableComparModeDigitalFilter(OPAx);
    }
    else if(OPA_InitStruct->Mode == LL_OPA_MODE_PGA)
    {
        /*??????PGA??????*/
        LL_OPA_SetGain(OPAx,OPA_InitStruct->Gain);
        if(OPA_InitStruct->NegtiveToPin == ENABLE)
        {
            /*??????PGA??????????????????????????????PIN*/
            LL_OPA_EnableNegtiveConnetToPIN(OPAx);
        }
    }
    /*OPA2?????????PGA??????*/
    if(OPAx == OPA2 && OPA_InitStruct->Mode == LL_OPA_MODE_PGA)
    {
        status = FAIL;
    }
    return status;
}
/**
  * @brief	LL_OPA_InitTypeDef ???????????????
  * @param 	LL_OPA_InitTypeDef ??????????????????????????????????????????????????? @ref LL_OPA_InitTypeDef structure 
  *         ?????????
  * @retval	None
  */

void LL_OPA_StructInit(LL_OPA_InitTypeDef *OPA_InitStruct)
{
   OPA_InitStruct->INP              = LL_OPA_POSITIVE_INPUT_CHANNEL_OPA_INP1;           
    
   OPA_InitStruct->INN              = LL_OPA_NEGTIVE_INPUT_CHANNEL_OPA_INN1; 
    
   OPA_InitStruct->Mode             = LL_OPA_MODE_STANDLONE; 
                  
   OPA_InitStruct->DigitalFilter    = DISABLE; 
    
   OPA_InitStruct->NegtiveToPin     = DISABLE;  
    
   OPA_InitStruct->Gain             = LL_OPA_GAIN_MUL_2; 
   
   OPA_InitStruct->LowPowermode     = DISABLE; 
}
/** 
  *@} 
  */
/** 
  *@} 
  */
/*************************************************************END OF FILE************************************************************/

