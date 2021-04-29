#ifndef __TASK_MANAGE
#define __TASK_MANAGE

#include "system_config.h"

typedef enum
{
    task_off = 0,
    task_on  = !task_off,
}__Task_Switch;

typedef enum
{
    type_null,
    typeMs,
    type1000Ms,    
}__Task_Type;

typedef void (*TASK_FUNC_PTR)(void);
typedef unsigned int Taskhandle;

extern Taskhandle show_running;
extern Taskhandle module_manage;
extern Taskhandle module_overtime;
extern Taskhandle AT_Overtime;
extern Taskhandle Cmd_Process;
#define System_arry_num  MAXTASK_NUMBER%32?(MAXTASK_NUMBER/32+1):MAXTASK_NUMBER/32
extern unsigned long System_Task[System_arry_num];

#pragma anon_unions
#pragma pack (1)


typedef struct{
    __Task_Type   timetype;             //任务时间类型
    unsigned char tasknum;      		//任务号
    __Task_Switch taskswitch;    		//任务开关
    unsigned long timeLen;       		//任务的周期
    unsigned long count;         		//任务时间计数
    TASK_FUNC_PTR Task_Fun_CallBack;
}__TASK_ARRY;
#pragma pack()

#define long_size sizeof(unsigned long)
#define regist_task(tasknum)   System_Task[tasknum/(long_size*8)]|=(1<<tasknum%(long_size*8))      //任务注册
#define unregist_task(tasknum)  System_Task[tasknum/(long_size*8)]&= ~(1<<tasknum%(long_size*8))    //任务注销

void task_schedule(void);
void task_init(void);
int task_start(__Task_Type Type, unsigned long Cycle_time, TASK_FUNC_PTR task_func);
int task_stop(TASK_FUNC_PTR fun_ptr);
void task_1000Mstimer(void);
void task_Mstimer(void);
int task_reset(__Task_Type Type, unsigned long Cycle_time, TASK_FUNC_PTR task_func);
unsigned char IsTaskRunning(void);

#endif

