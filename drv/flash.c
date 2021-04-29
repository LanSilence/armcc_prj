#include "main.h"  

ErrorStatus LL_FLASH_Program_String(FLASH_Type* FLASHx, uint32_t Address, uint32_t *data,uint32_t Length)
{
  uint32_t i;
	ErrorStatus ret;
	for(i=0; i<Length; i++)
	{
		ret = LL_FLASH_Program_Word(FLASH, Address + i * 4, data[i]);
		if(ret == FAIL)
			return FAIL;
	}
  return PASS;
}

//扇区擦函数(每个扇区512字节) 
//适用于bootloader，禁止擦除boot区
//输入参数：SectorNum   需要擦除扇区的扇区号
uint8_t Flash_Erase_Sector( uint16_t SectorNum )
{
	uint16_t i;
	uint8_t Result = 0;
	uint32_t *PFlash;
	
	if(SectorNum < (APPLICATION_ADDRESS_OFFSET/512)) return 2;//禁止擦除bootloader区
	PFlash = (uint32_t *)(uint32)(SectorNum * 512);
	LL_FLASH_PageErase(FLASH,SectorNum * 512);
	for (i = 0; i < 128; i++ )
	{
		if (PFlash[i] != 0xFFFFFFFF ) 
		{
			Result = 1;
			break;
		}
	}
	return Result;
}

//连续写flash函数
//输入参数：prog_addr  被写flash首地址
//			prog_data  待写数据缓冲区
//			Len        写入长度(一次不要写入太多，以免看门狗溢出)
uint8_t Flash_Write_String( uint32_t prog_addr,uint32_t* prog_data, uint16_t Len)
{
	uint16_t i;
	uint8_t Result = 0;
	uint32_t *PFlash;
	
	if(prog_addr < APPLICATION_ADDRESS_OFFSET) return 2;//禁止写bootloader区
  	LL_FLASH_Program_String(FLASH,prog_addr, prog_data, Len);
	
	PFlash = (uint32_t*)prog_addr;
	for (i = 0; i < Len; i++)
	{
		if (PFlash[i] != prog_data[i]) 
		{
			Result = 1;
			break;
		}
	}
	return Result;
}


short order_search( uint32_t start_addr, uint32_t end_addr, int *pFindData, uint32_t SearchByte)
{
    unsigned short  end     = end_addr - start_addr; 
    uint32_t data ,i;
    
    for(i = 0;i<=end/4;i++)
    {
        data = *(uint32_t *)(start_addr+end-i*4);
        if(data == SearchByte)
        {
           *pFindData = end-i*4;
           return 0;
        }
    }
    return -1;
}

short bin_search( uint32_t start_addr, uint32_t end_addr, int *pFindData, uint32_t SearchByte)
{    
    unsigned short  start   = 0;
    unsigned short  end     = end_addr - start_addr;   //直接用直接地址，调试时出过错误，改用偏移地址
    unsigned short  mid;
    unsigned char    i;
    uint32_t data;

    if(*(uint32_t *)(start_addr) == 0xFFFFFFFF)
        return -1;    //flash无数据
    for(i = 0; i < 20; i++)    
    {
        mid = ( start + end ) / 2;
        if(mid&0x0003)
            mid &= ~(uint32_t)0x3;  //地址对齐
        data = *(uint32_t *)(start_addr + mid);
        
        if ( data != SearchByte )  
        {
            end = mid;
        }
        else if (data == SearchByte)
        {
            start = mid;    
        }

        if ( ( start + 4 ) == end )
        {
            data = *(uint32_t *)(start_addr + end);
            if (data == SearchByte)
            {
                *pFindData =end;
                return 1;
            }
            data = *(uint32_t *)(start_addr + start);
            if (data == SearchByte)
            {
                *pFindData =start;
                return 1;
            }
            else if ( data != SearchByte )
            {
                return -1;
            }
        }
    }
    return -1;
}
