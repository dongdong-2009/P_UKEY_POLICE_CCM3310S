							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: eflash_operate.h
Author:        Version:        Date: 20121230
Description:  eflash的接口
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/
#include "ccm3310s.h"
#ifdef  EFLASH_API_SYMBOL
#define	EFLASH_API_EXT
#else	
#define	EFLASH_API_EXT extern
#endif

#define PAGE_MASK                   (PAGE_BYTE_SIZE-1)
#define D_PAGE_SIZE		PAGE_BYTE_SIZE		//Double flash页大小

#define EFLASH_START_ADDRESS		0x20000		//保存数据的起始地址，用户可灵活定义
#define EFLASH_END_ADDRESS		0x40000


EFLASH_API_EXT BOOLEAN F_Write(UINT32 flash_addr, const UINT8 *ram_addr, UINT32 len) ;
EFLASH_API_EXT BOOLEAN F_Read(UINT32 flash_addr, UINT8* ram_addr, UINT32 len);
EFLASH_API_EXT BOOLEAN F_Write_u8(UINT32 flash_addr, UINT8 u8Data);
EFLASH_API_EXT BOOLEAN F_Write_u16(UINT32 flash_addr, UINT16 u16Data);
EFLASH_API_EXT BOOLEAN F_Write_u32(UINT32 flash_addr, UINT32 u32Data);
EFLASH_API_EXT UINT8 F_Read_u8(UINT32 flash_addr);
EFLASH_API_EXT UINT16 F_Read_u16(UINT32 flash_addr);
EFLASH_API_EXT UINT32 F_Read_u32(UINT32 flash_addr);

