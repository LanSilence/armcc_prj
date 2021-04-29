#ifndef __POWER_MANA_H
#define __POWER_MANA_H

//#include "main.h"

typedef enum
{
    Power_Normal,
    Power_Unnormal,
    
}Power_State;


typedef struct
{
    Power_State CurrentState;
    Power_State PreState;
}Power_Struct;

extern unsigned char power_change;
void Init_SVD(void);
void Power_Check(void);
void power_check_overtime(void);
    
#endif



