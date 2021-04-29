#ifndef	__FILE_BOOTLOAD_H__
#define __FILE_BOOTLOAD_H__

#include "define_all.h"  


/************************************************************
 函数名称：BOOTLOAD_write_cfg_to_flash
 功能说明：将bootload的cfg写入flash
 输入参数：
 输出参数：无
 返回参数：正确返回 0 ,错误返回 1
************************************************************/
extern unsigned char BOOTLOAD_write_cfg_to_flash(void);

#endif
