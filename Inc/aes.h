
#ifndef __AES_H
#define __AES_H
#ifdef __cplusplus
extern "C" {
#endif
	
/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33xx.h"
	
ErrorStatus Test_AES_ECB(void);
ErrorStatus Test_AES_CTR(void);
ErrorStatus Test_AES_CBC(void);
ErrorStatus Test_AES_MULTH(void);	
ErrorStatus Test_AES_GCM(void);


uint16_t AES_Encryption(uint8_t *AESKey, uint8_t *DataIn, uint16_t DataLen, uint8_t *DataOut);
uint16_t AES_Dencryption(uint8_t *AESKey, uint8_t *DataIn, uint16_t DataLen, uint8_t *DataOut);
#ifdef __cplusplus
}
#endif

#endif /* __AES_H */

/************************ (C) COPYRIGHT FMSH *****END OF FILE****/
