#include "main.h"  

#define BOOTLOAD_CONFIG_START_ADDRESS  			(0x1E000) 	//����������Ϣ��ʼ��ַ128K flash��


/************************************************************
 �������ƣ�BOOTLOAD_write_cfg_to_flash
 ����˵������bootload��cfgд��flash
 ���������
 �����������
 ���ز�������ȷ���� 0 ,���󷵻� 1
************************************************************/
unsigned char BOOTLOAD_write_cfg_to_flash(void)
{
unsigned long flag = 0x12345678;	
	if(Flash_Erase_Sector(BOOTLOAD_CONFIG_START_ADDRESS/512)!=0)//��cfg����
			return 1;
	//д��flash	
	if(Flash_Write_String(BOOTLOAD_CONFIG_START_ADDRESS,(uint32_t *)&flag, 1)!=0)//��cfgд��flash
			return 2;
  return 0;	
}


