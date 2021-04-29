#ifndef	__FLASH_H__
#define __FLASH_H__

#include "define_all.h"  

#define ALL_FLASH_SIZE          0x10000
#define ADDR_APPSTART           0x4000                               //APP��ʼ��ַ

#define ADDR_UPGRADE_PAMARE     (ADDR_APPSTART+0x7800)
#define ADDR_UPGARDE_SIZE       (0x200)

#define ADDR_UPDATA             (ADDR_UPGRADE_PAMARE+ADDR_UPGARDE_SIZE)                  //����APP�ĵ�ַ
#define ADDR_UP_END             (ADDR_UPDATA + 0xA000) 

#define ADDR_TEMP_SIZE          (0x400)
#define ADDR_TEMP_START         (ALL_FLASH_SIZE-0xC00)                  //������ʼ
#define ADDR_TEMP_DATE          (ADDR_TEMP_START)                       //�¶ȼ�¼��ʼ����
#define ADDR_TEMP_DATA          (ADDR_TEMP_START+4)                     //�¶����ݴ洢λ��
#define ADDR_TEMP_END           (ADDR_TEMP_START+ADDR_TEMP_SIZE)

#define ADDR_P_DATA_SIZE        (0x200)                                 //��Ա����ָ��
#define ADDR_P_DATA_FREEZE      (ALL_FLASH_SIZE-0x800)
#define ADDR_P_DATA_END         (ADDR_P_DATA_FREEZE+ADDR_P_DATA_SIZE)

#define ADDR_DATA_FREEZE_SIZE   (0x400)
#define ADDR_DATA_FREEZE        (ADDR_P_DATA_END)                        //��Ա��������
#define ADDR_DATA_FREEZE_END    (ADDR_DATA_FREEZE + ADDR_DATA_FREEZE_SIZE)  

#define ADDR_USERPARAME_SIZE    (0x200)
#define ADDR_USERPARAME         (ADDR_DATA_FREEZE_END)                       //���512�ֽڴ洢�û����ò���
#define ADDR_USERPARAME_END     (ADDR_DATA_FREEZE_END+ADDR_USERPARAME_SIZE)

#define ADDR_UPFLAG             (ADDR_USERPARAME+40U)                   //������־




extern void FlashInit(void);
extern uint8_t Flash_Erase_Sector( uint16_t SectorNum);
extern uint8_t Flash_Write_String( uint32 prog_addr,uint32_t* prog_data, uint16_t Len);
short bin_search( uint32_t start_addr, uint32_t end_addr, int *pFindData, uint32_t SearchByte);
short order_search( uint32_t start_addr, uint32_t end_addr, int *pFindData, uint32_t SearchByte);
#endif
