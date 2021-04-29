#ifndef __USER_INIT_H__
#define __USER_INIT_H__

#include "main.h"

// 8M: LL_RCC_RCHF_FREQUENCY_8MHZ
// 16M: LL_RCC_RCHF_FREQUENCY_16MHZ
// 24M: LL_RCC_RCHF_FREQUENCY_24MHZ
#define RCHF_CLOCK  LL_RCC_RCHF_FREQUENCY_8MHZ

extern uint32_t systemClock;

void UserInit(void);

void DelayUs(uint32_t count);
void DelayMs(uint32_t count);
void IWDT_Clr(void);

#endif
