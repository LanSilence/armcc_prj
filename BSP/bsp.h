#ifndef __BSP_H
#define __BSP_H

#define IO_NBMODE_POWER_PORT    (GPIOA)
#define IO_NBMODE_POWER_PIN     LL_GPIO_PIN_9
#define IO_NBMODE_POWER_OUT     LL_GPIO_DisablePinOpendrain(IO_NBMODE_POWER_PORT,IO_NBMODE_POWER_PIN);LL_GPIO_SetPinMode(GPIOA,IO_NBMODE_POWER_PIN,LL_GPIO_MODE_OUTPUT);LL_GPIO_DisablePinPullup(GPIOA,IO_NBMODE_POWER_PIN)
#define IO_NBMODE_POWER(a)      (a)?(IO_NBMODE_POWER_PORT->DSET |= IO_NBMODE_POWER_PIN):(IO_NBMODE_POWER_PORT->DRST |= IO_NBMODE_POWER_PIN)
#define IO_NBMODE_POWER_OPENDRAIN  LL_GPIO_EnablePinOpendrain(IO_NBMODE_POWER_PORT,IO_NBMODE_POWER_PIN)


#define IO_NBMODE_VBAT_PORT      (GPIOA)
#define IO_NBMODE_VBAT_PIN       LL_GPIO_PIN_8
#define IO_NBMODE_VBAT_OUT       LL_GPIO_DisablePinOpendrain(IO_NBMODE_VBAT_PORT,IO_NBMODE_VBAT_PIN);LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8,LL_GPIO_MODE_OUTPUT);LL_GPIO_EnablePinPullup(GPIOA,LL_GPIO_PIN_8)
#define IO_NBMODE_VBAT(a)        (a)?(IO_NBMODE_VBAT_PORT->DSET |= IO_NBMODE_VBAT_PIN):(IO_NBMODE_VBAT_PORT->DRST |= IO_NBMODE_VBAT_PIN)
#define IO_NBMODE_VBAT_OPENDRAIN       LL_GPIO_EnablePinOpendrain(IO_NBMODE_VBAT_PORT,IO_NBMODE_VBAT_PIN)

#define IO_REMOVE_PORT      (GPIOD)
#define IO_REMOVE_PIN       LL_GPIO_PIN_1
#define IO_REMOVE_OUT       LL_GPIO_DisablePinOpendrain(IO_REMOVE_PORT,IO_REMOVE_PIN);LL_GPIO_SetPinMode(IO_REMOVE_PORT,IO_REMOVE_PIN,LL_GPIO_MODE_OUTPUT);LL_GPIO_EnablePinPullup(IO_REMOVE_PORT,IO_REMOVE_PIN)
#define IO_REMOVE_IN        LL_GPIO_EnablePinInput(IO_REMOVE_PORT,IO_REMOVE_PIN);LL_GPIO_EnablePinOpendrain(IO_REMOVE_PORT,IO_REMOVE_PIN);LL_GPIO_SetPinMode(IO_REMOVE_PORT,IO_REMOVE_PIN,LL_GPIO_MODE_INPUT);LL_GPIO_DisablePinPullup(IO_REMOVE_PORT,IO_REMOVE_PIN)
#define IO_REMOVE(a)        (a)?(IO_REMOVE_PORT->DSET |= IO_REMOVE_PIN):(IO_REMOVE_PORT->DRST |= IO_REMOVE_PIN)
#define IO_REMOVE_READ      (IO_REMOVE_PORT->DIN & IO_REMOVE_PIN) >> 1
//#define IO_NBMODE_POWER_PORT    (GPIOA)

#define IO_IRDA1_PORT      (GPIOC)
#define IO_IRDA1_PIN       LL_GPIO_PIN_4
#define IO_IRDA1_OUT       LL_GPIO_DisablePinOpendrain(IO_IRDA1_PORT,IO_IRDA1_PIN);LL_GPIO_SetPinMode(IO_IRDA1_PORT,IO_IRDA1_PIN,LL_GPIO_MODE_OUTPUT);LL_GPIO_EnablePinPullup(IO_IRDA1_PORT,IO_IRDA1_PIN)
#define IO_IRDA1_IN        LL_GPIO_EnablePinInput(IO_IRDA1_PORT,IO_IRDA1_PIN);LL_GPIO_EnablePinOpendrain(IO_IRDA1_PORT,IO_IRDA1_PIN);LL_GPIO_SetPinMode(IO_IRDA1_PORT,IO_IRDA1_PIN,LL_GPIO_MODE_INPUT);LL_GPIO_DisablePinPullup(IO_IRDA1_PORT,IO_IRDA1_PIN)
#define IO_IRDA1(a)        (a)?(IO_IRDA1_PORT->DSET |= LL_GPIO_PIN_4):(IO_IRDA1_PORT->DRST |= LL_GPIO_PIN_4)
#define IO_IRDA1_READ      (IO_IRDA1_PORT->DIN & IO_IRDA1_PIN) >> 4

#define IO_IRDA_A_PORT      (GPIOC)
#define IO_IRDA_A_PIN       LL_GPIO_PIN_9|LL_GPIO_PIN_10  
#define IO_IRDA_A_OUTPUT    IO_IRDA_A_PORT->PUEN &= ~(IO_IRDA_A_PIN);IO_IRDA_A_PORT->INEN &= (IO_IRDA_A_PIN);IO_IRDA_A_PORT->FCR &= ~(0xf<<18); IO_IRDA_A_PORT->FCR |= ~(0x5<<18);IO_IRDA_A_PORT->ODEN &= ~(IO_IRDA_A_PIN)
#define IO_IRDA_A_LOW       IO_IRDA1_PORT->DRST |= IO_IRDA_A_PIN;IO_IRDA_A_PORT->FCR &=(0xf<<18)

#define IO_IRDA_A_INPUT     IO_IRDA_A_PORT->INEN |= (IO_IRDA_A_PIN);IO_IRDA_A_PORT->PUEN &= ~(IO_IRDA_A_PIN)

void AllPin_Output_Low(void);
#endif
