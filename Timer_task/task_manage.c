#include "task_manage.h"




/***************
*
*�ļ���task_manage.c
*���ڹ��������������ִ�е�ÿ������
*
*
*
***************/
unsigned long System_Task[System_arry_num] = {0};   //ÿ��������Ҫ����regist_taskע�ᵽSystem_Task
__TASK_ARRY Task_Arry[MAXTASK_NUMBER+1];            //�������

void task_1000Mstimer(void)
{
    unsigned int task_num;
    for(task_num = 1; task_num <= MAXTASK_NUMBER; task_num++)    
    {
      if(Task_Arry[task_num].timetype == type1000Ms && Task_Arry[task_num].taskswitch == task_on)
        Task_Arry[task_num].count++;
    }
}

void task_Mstimer(void)
{
    unsigned int task_num;
    for(task_num = 1; task_num <= MAXTASK_NUMBER; task_num++)    
    {
        if(Task_Arry[task_num].timetype == typeMs && Task_Arry[task_num].taskswitch == task_on)
           Task_Arry[task_num].count++;
    }
}


/**************
*����:  ������ע������
*����1����������typeMs ��������  type1000Ms ������
*����2������ʱ��
*����3������ص�����(����ִ�к���)
*���أ� 0ʧ�� ����0�ɹ�
***************/
int task_start(__Task_Type Type, unsigned long Cycle_time, TASK_FUNC_PTR task_func)
{
    unsigned int task_num;
    for(task_num = 1; task_num <= MAXTASK_NUMBER; task_num++)    
    {

        if(Task_Arry[task_num].taskswitch  == task_off)
        {
            Task_Arry[task_num].count       = 0;
            Task_Arry[task_num].tasknum     = task_num;
            Task_Arry[task_num].taskswitch  = task_on;
            Task_Arry[task_num].timeLen     = Cycle_time;
            Task_Arry[task_num].timetype    = Type;
            Task_Arry[task_num].Task_Fun_CallBack = task_func;
            return task_num;
        }
    }
    return -1;
}

/***********
����ɾ�� 0�ɹ� -1ʧ��
***********/
int task_stop(TASK_FUNC_PTR fun_ptr)
{
    unsigned int task_num;
    for(task_num= 0;task_num <= MAXTASK_NUMBER;task_num++)
    {
        if(Task_Arry[task_num].Task_Fun_CallBack == fun_ptr && Task_Arry[task_num].taskswitch  == task_on)
        {
            Task_Arry[task_num].count       = 0;
            Task_Arry[task_num].tasknum     = task_num;
            Task_Arry[task_num].taskswitch  = task_off;
            Task_Arry[task_num].timeLen     = 0;
            Task_Arry[task_num].timetype    = type_null;
            Task_Arry[task_num].Task_Fun_CallBack = 0; 
            return 0;
        }
    }
    return -1;
}

void task_init(void)
{
    unsigned char task_num;
    for(task_num = 1;task_num <= MAXTASK_NUMBER; task_num++)
    {
        Task_Arry[task_num].count       = 0;
        Task_Arry[task_num].tasknum     = task_num;
        Task_Arry[task_num].taskswitch  = task_off;
        Task_Arry[task_num].timeLen     = 0;
        Task_Arry[task_num].timetype    = type_null;
        Task_Arry[task_num].Task_Fun_CallBack = 0;       
    }
}

void task_schedule(void)
{
    unsigned char task_num;
    for (task_num = 1; task_num <= MAXTASK_NUMBER; task_num++)
    {
        if (Task_Arry[task_num].taskswitch == task_on)
        {
            if (Task_Arry[task_num].count >= Task_Arry[task_num].timeLen)
            {
                Task_Arry[task_num].count = 0;
                Task_Arry[task_num].Task_Fun_CallBack();
            }
        }

    }
}
//��������ʱ�������
int task_reset(__Task_Type Type, unsigned long Cycle_time, TASK_FUNC_PTR task_func)
{
    unsigned int task_num;
    for(task_num= 0;task_num <= MAXTASK_NUMBER;task_num++)
    {
        if(Task_Arry[task_num].Task_Fun_CallBack == task_func && Task_Arry[task_num].taskswitch  == task_on)
        {
            Task_Arry[task_num].count       = 0;
            Task_Arry[task_num].tasknum     = task_num;
            Task_Arry[task_num].timeLen     = Cycle_time;
            Task_Arry[task_num].timetype    = Type;
            return 0;
        }
    }
    if(task_num >= MAXTASK_NUMBER)
    {
        task_start(Type,Cycle_time,task_func);   //�����ڸ��������Զ�����
    }
    return -1;
}


unsigned char IsTaskRunning(void)
{
    unsigned char task_num;
    for (task_num = 1; task_num <= MAXTASK_NUMBER; task_num++)
    {
        if(Task_Arry[task_num].timetype == typeMs && Task_Arry[task_num].taskswitch == task_on)
            return 1;
    }
    
    return 0;
}


