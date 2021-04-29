#ifndef	__CRC_H__
#define __CRC_H__

#include "define_all.h"  

extern void Init_CRC_CRC16CCITT(void);
extern unsigned int CalCRC16_CCITT( uint16 Init,__IO uint08 *DataIn, unsigned long Len );
extern unsigned int BitChange(unsigned int us_DataIn);
#endif
