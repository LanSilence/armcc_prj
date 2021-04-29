#include "main.h"

typedef struct
{
	uint8_t *ByteStr; //数据首地址 
	uint32_t Len; 	 //长度
} ByteStr_TypeDef;


typedef struct
{
	ByteStr_TypeDef	Plaintext;//明文
	ByteStr_TypeDef AAD;//附加信息
	ByteStr_TypeDef	Ciphertext;//密文
	ByteStr_TypeDef Tag;//认证标签, 固定IV为128bit（16字节）
	ByteStr_TypeDef IV;//初始向量，固定IV为96bit（12字节）
} GcmStruct_TypeDef;


void ByteExChange(uint32_t* pWord, uint32_t Length)
{
	int32_t i = 0;
	int32_t value = 0;
	uint32_t* pRam = pWord;
	for(i=0; i<Length; i++)
	{
		value = (*pRam << 24) + (((*pRam & 0xFF00) >> 8)<< 16) + (((*pRam & 0xFF0000) >> 16) << 8) + (*pRam >> 24) ;
		*pRam++= value;
	}
}


/**
  * @brief	设置加解密密钥(key0存放密钥最低32bit)
  * @param 	密钥	16/24/32字节长度的密钥  
  * @param 	长度	16/24/32          
  * @retval	None
  */
void AES_KEY_WriteEx(uint8_t *KeyIn, uint8_t Len)
{
	uint8_t i;
	__IO uint32_t Temp32;
	
	if( (Len != 16) && (Len != 24) && (Len != 32) ) return;
	
	for(i=0; i<Len; i=i+4)
	{
		Temp32 = (KeyIn[0+i]<<24)|(KeyIn[1+i]<<16)|(KeyIn[2+i]<<8)|(KeyIn[3+i]<<0);
		LL_AES_SetKeys(AES,(Len-i)/4-1,Temp32);
	}
	
}

/**
  * @brief	读取加解密密钥，第一个字存放密钥最低32bit
  * @param 	密钥	16/24/32字节长度的密钥  
  * @param 	长度	16/24/32          
  * @retval	None
  */
void AES_KEY_ReadEx(uint8_t *KeyOut, uint8_t Len)
{
	uint8_t i;
	__IO uint32_t Temp32;
	
	if( (Len != 16) && (Len != 24) && (Len != 32) ) return;
	
	for(i=0; i<Len; i=i+4)
	{
		Temp32 = LL_AES_GetKeys(AES , (Len-i)/4-1);
		KeyOut[0+i] = Temp32>>24;
		KeyOut[1+i] = Temp32>>16;
		KeyOut[2+i] = Temp32>>8;
		KeyOut[3+i] = Temp32>>0;
	}
}

/**
  * @brief	设置初始向量
  * @param 	初始向量        
  * @retval	None
  */
void AES_IVR_WriteEx(uint8_t *IVRIn)
{
	uint8_t i;
	__IO uint32_t Temp32;
	
	for(i=0; i<4*4; i=i+4)
	{
		Temp32 = (IVRIn[0+i]<<24)|(IVRIn[1+i]<<16)|(IVRIn[2+i]<<8)|(IVRIn[3+i]<<0);
		LL_AES_WriteDataToIVR(AES,(16-i)/4-1,Temp32);
	}
}

/**
  * @brief	读取初始向量
  * @param 	初始向量        
  * @retval	None
  */

void AES_IVR_ReadEx(uint8_t *IVROut)
{
	uint8_t i;
	__IO uint32_t Temp32;
	
	for(i=0; i<4*4; i=i+4)
	{
		Temp32 = LL_AES_ReadDataFromIVR(AES,(4*4-i)/4-1);
		IVROut[0+i] = Temp32>>24;
		IVROut[1+i] = Temp32>>16;
		IVROut[2+i] = Temp32>>8;
		IVROut[3+i] = Temp32>>0;	
	}
}

/**
  * @brief	设置加解密数据，16字节（128bit）分组数据输入
  * @param 	初始向量        
  * @retval	None
  */

void AES_DIN_GroupWrite_128BIT(uint8_t *DataIn)
{
	uint8_t i;
	__IO uint32_t Temp32;

	for(i=0; i<16; i=i+4)
	{
		Temp32 = (DataIn[0+i]<<24)|(DataIn[1+i]<<16)|(DataIn[2+i]<<8)|(DataIn[3+i]<<0);
		LL_AES_WriteDataToDIR(AES,Temp32);
	}
}

/**
  * @brief	加解密数据输出，16字节（128bit）分组数据输出        
  * @param 	初始向量        
  * @retval	None
  */

void AES_DOUT_GroupRead_128BIT(uint8_t *DataOut)
{
	uint8_t i;
	__IO uint32_t Temp32;
	
	for(i=0; i<16; i=i+4)
	{
		Temp32 = LL_AES_ReadDataFromDOR(AES);
		DataOut[0+i] = Temp32>>24;
		DataOut[1+i] = Temp32>>16;
		DataOut[2+i] = Temp32>>8;
		DataOut[3+i] = Temp32>>0;
	}
}

/**
  * @brief	先设置16字节（128bit）加密数据，然后再读取16字节（128bit）解密数据       
  * @param 	加密数据   
  * @param 	解密数据  
  * @retval	None
  */
ErrorStatus AES_GroupWriteAndRead_128BIT(uint8_t *DataIn, uint8_t *DataOut)
{	
	uint16_t i;
	
	/* 输入加解密数据 */
	AES_DIN_GroupWrite_128BIT(DataIn);
	
	/* 等待CCF标志置起,完成计算大约需要100个时钟周期 */
	for(i=0;i<500;i++)
	{
		if(SET == LL_AES_IsActiveFlag_Complete(AES) ) break;
	}
	/* 清CCF标志 */
	/* 读取前必须清除 */ 
	LL_AES_ClearFlag_Complete(AES);
	
	/* 读取结果 */
	AES_DOUT_GroupRead_128BIT(DataOut);
	
	if(i==500) return FAIL;
	else return PASS;
}


/**
  * @brief	先设置16字节（128bit）加密数据，然后再读取16字节（128bit）解密数据       
  * @param 	加密数据   
  * @param 	解密数据  
  * @retval	None
  */
ErrorStatus AES_GroupWriteAndRead_128BIT_IVR(uint8_t *DataIn, uint8_t *DataOut)
{	
	uint16_t i;
	
	/* 输入加解密数据 */
	AES_DIN_GroupWrite_128BIT(DataIn);
	
	/* 等待CCF标志置起,完成计算大约需要100个时钟周期 */
	for(i=0;i<500;i++)
	{
		if(SET == LL_AES_IsActiveFlag_Complete(AES) ) break;
	}
	/* 清CCF标志，读取前必须清除 */
	LL_AES_ClearFlag_Complete(AES);
	
	/* 读取结果 */
	AES_IVR_ReadEx(DataOut);
	
	if(i==500) return FAIL;
	else return PASS;
}








/**
  * @brief	AES初始化函数
	* @param  KeyLenth This parameter can be one of the following values:
  *           @arg @ref LL_AES_KEY_LENTH_IS_128BITS
  *           @arg @ref LL_AES_KEY_LENTH_IS_192BITS
  *           @arg @ref LL_AES_KEY_LENTH_IS_256BITS
	* @param	CipherMode This parameter can be one of the following values:
  *           @arg @ref LL_AES_CIPHER_ECB_MODE
  *           @arg @ref LL_AES_CIPHER_CBC_MODE
  *           @arg @ref LL_AES_CIPHER_CTR_MODE
  *           @arg @ref LL_AES_CIPHER_MULT_MODE			
	* @param	OperationMode This parameter can be one of the following values:
  *           @arg @ref LL_AES_OPERATION_MODE_ENCRYPTION
  *           @arg @ref LL_AES_OPERATION_MODE_KEY_EXTENSION
  *           @arg @ref LL_AES_OPERATION_MODE_DECRYPT
  *           @arg @ref LL_AES_OPERATION_MODE_KEY_EXTENSION_AND_DECRYPT	
	* @param	DataType This parameter can be one of the following values:
  *           @arg @ref LL_AES_DATA_SWAP_RULE_NOT_EXCHANGED
  *           @arg @ref LL_AES_DATA_SWAP_RULE_HAIFWORD_EXCHANGE
  *           @arg @ref LL_AES_DATA_SWAP_RULE_BYTE_EXCHANGED
  *           @arg @ref LL_AES_DATA_SWAP_RULE_BIT_EXCHANGED	
  * @retval	返回错误状态，可能值：
  *       	-PASS 测试成功
  *       	-FAIL 测试失败
  */
ErrorStatus AES_Init(uint32_t KeyLenth, uint32_t CipherMode, uint32_t OperationMode, uint32_t DataType)
{
	LL_AES_InitTypeDef AES_InitStructer;
	/********************加密过程********************/
	/* 重置AES模块 */
	LL_AES_DeInit();
	/* 设置 AES_InitStruct 为默认配置 */
	LL_AES_StructInit(&AES_InitStructer);
	AES_InitStructer.KeyLenth      = KeyLenth;
  AES_InitStructer.CipherMode    = CipherMode;
  AES_InitStructer.OperationMode = OperationMode;
  AES_InitStructer.DataType      = DataType; 
	/* AES_InitStructer初始化对应外设入口地址的寄存器值 */
	return LL_AES_Init(AES,&AES_InitStructer);
}

/**
  * @brief	AES加解密处理流程
	* @param  KeyIn 密钥指针
	* @param  KeyLen 密钥长度
	* @param	IVRIn 向量指针	
	* @param	DataIn 加密输入数据指针
	* @param	DataInLen 加密数据输入长度
	* @param	DataOut 解密输出数据指针
	* @param	DataOutLen 解密输出长度
  * @retval	返回错误状态，可能值：
  *       	-PASS 测试成功
  *       	-FAIL 测试失败
  */

ErrorStatus AES_EncryptionAndDecryption(uint8_t *KeyIn, uint8_t KeyLen, uint8_t *IVRIn, uint8_t *DataIn, uint16_t DataInLen, uint8_t *DataOut, uint16_t* DataOutLen)
{
	uint32_t i;
	uint32_t result = 0;
	uint8_t TempBuf[16];
	/* 设置密钥 */
	AES_KEY_WriteEx(KeyIn,KeyLen);	
	/* CTR和CBC模式下写AES_IVRx寄存器 */
//	if((*IVRIn != NULL) && (LL_AES_GetCipherMode(AES) != LL_AES_CIPHER_ECB_MODE))
//	{
//		AES_IVR_WriteEx(IVRIn);	
//	}
	/* 启动AES */
	LL_AES_Enable(AES);

	for(i=0; i<DataInLen/16; i++)
	{
		if(LL_AES_GetCipherMode(AES) == LL_AES_CIPHER_MULT_MODE)
		{
			result = AES_GroupWriteAndRead_128BIT_IVR(DataIn + 16*i, DataOut + 16*i);
			if(result == FAIL)
				return FAIL;
		}
		else
		{
			result = AES_GroupWriteAndRead_128BIT(DataIn + 16*i, DataOut + 16*i);
			if(result == FAIL)
				return FAIL;
		}
	}
	if(DataInLen % 16)
	{
		memset(TempBuf , 0, sizeof(TempBuf));
		memcpy(TempBuf, DataIn + i*16,DataInLen - i*16);
		if(LL_AES_GetCipherMode(AES) == LL_AES_CIPHER_MULT_MODE)
		{
			result = AES_GroupWriteAndRead_128BIT_IVR(TempBuf, DataOut + i*16);
			if(result == FAIL)
				return FAIL;
		}
		else
		{
			result = AES_GroupWriteAndRead_128BIT(TempBuf, DataOut + i*16);
			if(result == FAIL)
				return FAIL;
		}
	}
	LL_AES_Disable(AES);
	return PASS;
}






uint16_t AES_Encryption(uint8_t *AESKey, uint8_t *DataIn, uint16_t DataLen, uint8_t *DataOut)
{
    uint16_t OutLen = 0;
    ErrorStatus result;
    /* 加密 */
	AES_Init(LL_AES_KEY_LENTH_IS_128BITS,LL_AES_CIPHER_ECB_MODE,LL_AES_OPERATION_MODE_ENCRYPTION,LL_AES_DATA_SWAP_RULE_NOT_EXCHANGED);
	result = AES_EncryptionAndDecryption(AESKey,16,0,DataIn,DataLen,DataOut,&OutLen);
    if(result == PASS)
        return OutLen;
    return FAIL;
}

//解密
uint16_t AES_Dencryption(uint8_t *AESKey, uint8_t *DataIn, uint16_t DataLen, uint8_t *DataOut)
{
    uint16_t OutLen = 0;
    ErrorStatus result;
    /* 加密 */
	AES_Init(LL_AES_KEY_LENTH_IS_128BITS,LL_AES_CIPHER_ECB_MODE,LL_AES_OPERATION_MODE_DECRYPT,LL_AES_DATA_SWAP_RULE_NOT_EXCHANGED);
	result = AES_EncryptionAndDecryption(AESKey,16,0,DataIn,DataLen,DataOut,&OutLen);
    if(result == PASS)
        return OutLen;
    return FAIL;
}


