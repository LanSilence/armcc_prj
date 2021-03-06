/**
  *******************************************************************************************************
  * @file    fm33lc0xx_ll_lcd.c
  * @author  FMSH Application Team
  * @brief   Src file of LCD LL Module
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
#include "fm33lc0xx_ll_rmu.h"
#include "fm33lc0xx_ll_lcd.h"
#include "fm33_assert.h"
/** @addtogroup FM33LC0xx_LL_Driver_LCD
  * @{
  */
  
/** @addtogroup Private_Macros
  * @{
  */

#define         IS_LL_LCD_INSTANCE(INTENCE)             ((INTENCE) == LCD)

#define         IS_LL_LCD_ICCTRL(__VALUE__)             (((__VALUE__) == (LL_LCD_DRV_IC_CTRL_LARGETS))||\
                                                        ((__VALUE__) == (LL_LCD_DRV_IC_CTRL_LARGE))||\
                                                        ((__VALUE__) == (LL_LCD_DRV_IC_CTRL_SMALL))||\
                                                        ((__VALUE__) == (LL_LCD_DRV_IC_CTRL_MININUM)))
                                                        
#define         IS_LL_LCD_ENMODE(__VALUE__)             ((__VALUE__) == (LL_LCD_DRV_ENMODE_INNERRESISTER))

#define         IS_LL_LCD_BIAS(__VALUE__)               (((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_0))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_1))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_2))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_3))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_4))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_5))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_6))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_7))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_8))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_9))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_10))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_11))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_12))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_13))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_14))||\
                                                        ((__VALUE__) == (LL_LCD_GRAY_SCALE_CTRL_LEVEL_15)))

#define         IS_LL_LCD_BIASMD(__VALUE__)             (((__VALUE__) == (LL_LCD_SET_BIASMD_DIV4BIAS))||\
                                                        ((__VALUE__) == (LL_LCD_SET_BIASMD_DIV3BIAS)))
                                                        
#define         IS_LL_LCD_BWFT(__VALUE__)               (((__VALUE__) == (LL_LCD_WAVEFORM_FORMAT_TYPE_A))||\
                                                        ((__VALUE__) == (LL_LCD_WAVEFORM_FORMAT_TYPE_B)))
                                                        
#define         IS_LL_LCD_LMUX(__VALUE__)               (((__VALUE__) == (LL_LCD_SEG_LINE_SELECT_COM_4))||\
                                                        ((__VALUE__) == (LL_LCD_SEG_LINE_SELECT_COM_6))||\
                                                        ((__VALUE__) == (LL_LCD_SEG_LINE_SELECT_COM_8)))


/**
  * @}
  */
/** @addtogroup LCD_LL_EF_Init
  * @{
  */ 

/**
  * @brief  ????????????LCD?????????.
  * @param  LCDx 
  * @retval ErrorStatus?????????:
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */  
static uint32_t LL_LCD_DisplayFreq(uint32_t freq)
{
    uint32_t displayFreq;
    uint8_t DivWFT;
	
	switch (LL_LCD_GetWaveform(LCD))
    {
		case LL_LCD_WAVEFORM_FORMAT_TYPE_A: DivWFT = 2; break;
        case LL_LCD_WAVEFORM_FORMAT_TYPE_B: DivWFT = 4; break;
    }
	
	if ((freq > 0) && (freq <= 100))
	{
        switch (LL_LCD_GetCOMxNum(LCD))
        {
            case LL_LCD_SEG_LINE_SELECT_COM_4:
                displayFreq = (uint32_t)(32768.0 / (float)(4 * freq * DivWFT) + 0.5);
                break;
            
            case LL_LCD_SEG_LINE_SELECT_COM_6:
                displayFreq = (uint32_t)(32768.0 / (float)(6 * freq * DivWFT) + 0.5);
                break;
            
            case LL_LCD_SEG_LINE_SELECT_COM_8:
                displayFreq = (uint32_t)(32768.0 / (float)(8 * freq * DivWFT) + 0.5);
                break;
        }
	}
	else
	{
		displayFreq = 32;
	}
    
    return displayFreq & 0xff;
}

/**
  * @brief  ????????????LCD?????????.
  * @param  LCDx 
  * @retval ErrorStatus?????????:
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */  
static uint32_t LL_LCD_FlickTime(uint32_t time)
{
    float stepTime;
   
    switch (LL_LCD_GetCOMxNum(LCD))
    {
        case LL_LCD_SEG_LINE_SELECT_COM_4:
            stepTime = (float)(4 * LL_LCD_GetDisplayFrequencyPsc(LCD) * 2 * 16 * 1000) / 32768.0;
            break;
        
        case LL_LCD_SEG_LINE_SELECT_COM_6:
            stepTime = (float)(6 * LL_LCD_GetDisplayFrequencyPsc(LCD) * 2 * 16 * 1000) / 32768.0;
            break;
        
        case LL_LCD_SEG_LINE_SELECT_COM_8:
            stepTime = (float)(8 * LL_LCD_GetDisplayFrequencyPsc(LCD) * 2 * 16 * 1000) / 32768.0;
            break;
    }
    
    return (uint32_t)(time / stepTime + 0.5);
}

/**
  * @brief  ????????????LCD?????????.

  * @param  LCDx 
  * @retval ErrorStatus?????????:
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */  
ErrorStatus LL_LCD_DeInit(LCD_Type *LCDx)
{
	assert_param(IS_LL_LCD_INSTANCE(LCDx));
    
    /* ?????????????????? */
    LL_RCC_EnablePeripheralReset();
    
    /* ??????LCD??????????????? */
    LL_RCC_EnableResetAPB2Peripheral(LL_RCC_APB1_PERIPHERAL_RST_LCD);
    LL_RCC_DisableResetAPB2Peripheral(LL_RCC_APB1_PERIPHERAL_RST_LCD);
    /* ?????????????????????????????????????????? */
    LL_RCC_Group2_DisableBusClock(LL_RCC_BUS2_CLOCK_LCD);
    
    /* ?????????????????? */
    LL_RCC_DisablePeripheralReset();
    return PASS;
}

/**
  * @brief  ?????? LCD_InitStruct ???????????????????????????????????????????????????????????????.
  * @param  LCDx LCDx 
  * @param  LCD_InitStruct ???????????? @ref LL_LCD_InitTypeDef ?????????
  *         ??????????????????????????????????????????.
  * @retval ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS LPUART????????????  
  */
ErrorStatus LL_LCD_Init(LCD_Type *LCDx,LL_LCD_InitTypeDef *LCD_InitStruct)
{
	assert_param(IS_LL_LCD_INSTANCE(LCDx));   
    assert_param(IS_LL_LCD_ICCTRL(LCD_InitStruct->ICCtrl));
    assert_param(IS_LL_LCD_ENMODE(LCD_InitStruct->EnableMode));
    assert_param(IS_LL_LCD_BIAS(LCD_InitStruct->BiasVoltage));
    assert_param(IS_LL_LCD_BIASMD(LCD_InitStruct->BiasMode));
    assert_param(IS_LL_LCD_BWFT(LCD_InitStruct->Waveform));
    assert_param(IS_LL_LCD_LMUX(LCD_InitStruct->COMxNum)); 
    
    /* ????????????????????? */
    LL_RCC_Group2_EnableBusClock(LL_RCC_BUS2_CLOCK_LCD);
    
    /* ????????????????????? */
    LL_LCD_SetICCtrl(LCD, LCD_InitStruct->ICCtrl);
    /* LCD???????????? */
    LL_LCD_SetEnableMode(LCD, LCD_InitStruct->EnableMode);
    /* ?????????????????? */
    LL_LCD_SetBiasVoltage(LCD, LCD_InitStruct->BiasVoltage);
    /* ?????????????????? */
    LL_LCD_SetBiasMode(LCD, LCD_InitStruct->BiasMode);
    /* ?????????????????? */
    LL_LCD_SetWaveform(LCD, LCD_InitStruct->Waveform);
    /* COMx????????? */
    LL_LCD_SetCOMxNum(LCD, LCD_InitStruct->COMxNum);
    /* ?????????????????? */
    LL_LCD_SetDisplayFrequencyPsc(LCD, LL_LCD_DisplayFreq(LCD_InitStruct->FreqPsc));
    /* ?????????????????? */
    LL_LCD_SetDispalyOnTime(LCD, LL_LCD_FlickTime(LCD_InitStruct->FlickOnTime));
    LL_LCD_SetDisplayOffTime(LCD, LL_LCD_FlickTime(LCD_InitStruct->FlickOffTime));
    /* ???????????? */
    LL_LCD_Enable(LCD);
    return PASS;
}

/**
  * @brief	?????? LCD_InitStruct ???????????????
  * @param 	LCD_InitStruct ??????????????????????????????????????????????????? @ref LL_LCD_InitTypeDef ?????????  
  *         
  * @retval	None
  */
void LL_LCD_StructInit(LL_LCD_InitTypeDef *LCD_InitStruct)
{
	LCD_InitStruct->ICCtrl              =   LL_LCD_DRV_IC_CTRL_LARGETS;
    LCD_InitStruct->EnableMode          =   LL_LCD_DRV_ENMODE_INNERRESISTER;
    LCD_InitStruct->BiasVoltage         =   LL_LCD_GRAY_SCALE_CTRL_LEVEL_10;
    LCD_InitStruct->BiasMode            =   LL_LCD_SET_BIASMD_DIV4BIAS;
    LCD_InitStruct->Waveform            =   LL_LCD_WAVEFORM_FORMAT_TYPE_A;
    LCD_InitStruct->COMxNum             =   LL_LCD_SEG_LINE_SELECT_COM_6;
    LCD_InitStruct->FreqPsc             =   42;
    LCD_InitStruct->FlickOnTime         =   0;
    LCD_InitStruct->FlickOffTime        =   0;
}



/**
  * @}
  */
/**
  * @}
  */
/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-10-17**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
