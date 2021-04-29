#include "main.h"  

#define BOOTLOAD_CONFIG_START_ADDRESS  			(0x1E000) 	//升级配置信息起始地址128K flash处


/************************************************************
 函数名称：BOOTLOAD_write_cfg_to_flash
 功能说明：将bootload的cfg写入flash
 输入参数：
 输出参数：无
 返回参数：正确返回 0 ,错误返回 1
************************************************************/
unsigned char BOOTLOAD_write_cfg_to_flash(void)
{
unsigned long flag = 0x12345678;	
	if(Flash_Erase_Sector(BOOTLOAD_CONFIG_START_ADDRESS/512)!=0)//将cfg擦除
			return 1;
	//写入flash	
	if(Flash_Write_String(BOOTLOAD_CONFIG_START_ADDRESS,(uint32_t *)&flag, 1)!=0)//将cfg写入flash
			return 2;
  return 0;	
}


