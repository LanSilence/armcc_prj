#include "main.h" 

//3. CRC16-CCITT
void Init_CRC_CRC16CCITT(void)
{
    LL_CRC_InitTypeDef CRC_InitStruct;
    
    CRC_InitStruct.CalculatMode = LL_CRC_CALCULA_MODE_SERIAL;
    CRC_InitStruct.DataWidth = LL_CRC_CRC_DR_WIDTH_BYTE;
    CRC_InitStruct.InitVel = 0x0000;
    CRC_InitStruct.Polynomial = 0x1021;
    CRC_InitStruct.PolynomialWidth = LL_CRC_POLYNOMIAL_WIDTH_16BIT;
    CRC_InitStruct.ReflectIn = LL_CRC_INPUT_REFLECTE_MODE_NOMAL;
    CRC_InitStruct.ReflectOut = LL_CRC_OUPUT_REFLECTE_MODE_NOMAL;
    CRC_InitStruct.XorReg = 0x0000;
    CRC_InitStruct.XorRegState = DISABLE;
    LL_CRC_Init(CRC, &CRC_InitStruct);
}


//CRC-16/CCITT x16+x12+x5+1 0x1021
//Init = 0x0000
//RefIn，RefOut
//计算结果高低字节倒序输出
unsigned int CalCRC16_CCITT( uint16 Init, __IO uint08 *DataIn, unsigned long Len )
{
	uint16 i, CRC16;

	LL_CRC_SetLinearShiftReg(CRC,Init);	//初值寄存器
	
	for( i=0; i<Len; i++ )
	{
		LL_CRC_SetDataReg(CRC,DataIn[i]);//输入输出寄存器
		Do_DelayStart();
		{
			if(RESET == LL_CRC_IsActiveFlag_Busy(CRC))
			{
				break;
			}
		}While_DelayUsEnd(200);
	}

	CRC16 = LL_CRC_GetDataReg(CRC) & 0xffff;
	return CRC16;
}

unsigned int BitChange(unsigned int us_DataIn)
{
unsigned int us_Data = us_DataIn;
us_Data = ((us_Data & 0xFF00) >> 8) | ((us_Data & 0x00FF) << 8);
us_Data = ((us_Data & 0xF0F0) >> 4) | ((us_Data & 0x0F0F) << 4);
us_Data = ((us_Data & 0xCCCC) >> 2) | ((us_Data & 0x3333) << 2);
us_Data = ((us_Data & 0xAAAA) >> 1) | ((us_Data & 0x5555) << 1);

return (us_Data);
}
