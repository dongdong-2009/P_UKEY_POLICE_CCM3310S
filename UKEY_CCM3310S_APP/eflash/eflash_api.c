							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: eflash_operate.c
Author:        Version:        Date: 20121230
Description:  eflash的接口
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define  EFLASH_API_SYMBOL
#include "includes.h"


UINT32  eflash_buffer[PAGE_BYTE_SIZE/4];

BOOLEAN EFLASH_Erase(UINT32 flash_addr)
{
	UINT8 status;

	//Enter High Power Mode
	set_HighPowerMode(0,10);  
	
	status = EFLASH_page_erase(flash_addr);
	
	//Exit High Power Mode
	set_HighPowerMode(1,10);  // 

	return status;
}


BOOLEAN FLASH_checkAddr(UINT32 flash_addr,UINT32 len)
{
	return TRUE;
}


BOOLEAN EFLASH_Write(UINT32 flash_addr, const UINT8* ram_addr, UINT32 len)
{
	UINT16 offset;
	UINT16 pageleft;
	UINT16 pageright;
	UINT32 dest = flash_addr;
	UINT8* src  = (UINT8*)ram_addr;
	UINT32 i;

	if (FLASH_checkAddr(flash_addr,len) == FALSE)
	{
		return FALSE;
	}

	//Enter High Power Mode
	set_HighPowerMode(0,10);  	

	offset  = ((UINT32)dest & PAGE_MASK);
	pageleft  = PAGE_BYTE_SIZE - offset;
	pageright = ((UINT16)(dest + len) & PAGE_MASK);
	
	/*如果页没有对齐，则先对第一页操作*/
	if (offset != 0) 
	{
		dest -= offset;/*dest指针跳转到当前页头*/
		memcpy((UINT8*)eflash_buffer, (UINT8*)dest, PAGE_BYTE_SIZE);
		if (len>pageleft) 
		{
			memcpy((UINT8*)eflash_buffer + offset, src, pageleft);
			len  -= pageleft;
			src  += pageleft;
		} 
		else 
		{
			memcpy((UINT8*)eflash_buffer + offset, src, len);
			len=0;
		}
		EFLASH_page_erase(dest);
		for(i=0;i<PAGE_BYTE_SIZE/4;i++)
		{
			EFLASH_program((dest+i*0x04),eflash_buffer[i]);
		}
		dest += PAGE_BYTE_SIZE;
		//offset = 0;
	}
	while (len > 0) 
	{
		/*如果存在中间页*/
		if (len >= PAGE_BYTE_SIZE) 
		{
			memcpy((UINT8*)eflash_buffer, src, PAGE_BYTE_SIZE);
			EFLASH_page_erase(dest);
			for(i=0;i<PAGE_BYTE_SIZE/4;i++)
			{
				EFLASH_program((dest+i*0x04),eflash_buffer[i]);
			}

			dest += PAGE_BYTE_SIZE;
			src  += PAGE_BYTE_SIZE;
			len  -= PAGE_BYTE_SIZE;
		} 
		else 
		{   /*做最后页操作*/
			memcpy((UINT8*)eflash_buffer, (UINT8*)dest, PAGE_BYTE_SIZE);
			memcpy((UINT8*)eflash_buffer, src, len);
			EFLASH_page_erase(dest);
			for(i=0;i<PAGE_BYTE_SIZE/4;i++)
			{
				EFLASH_program((dest+i*0x04),eflash_buffer[i]);
			}
			len = 0;                
		}
	}

	//Exit High Power Mode
	set_HighPowerMode(1,10);  // 
	   
	return TRUE;
}

BOOLEAN F_Write(UINT32 flash_addr, const UINT8 *ram_addr, UINT32 len) 
{
	return EFLASH_Write(flash_addr, ram_addr, len);
}

BOOLEAN F_Read(UINT32 flash_addr, UINT8* ram_addr, UINT32 len)
{	
	memcpy(ram_addr, (UINT8*)flash_addr, len);

	return TRUE;
}

BOOLEAN F_Write_u8(UINT32 flash_addr, UINT8 u8Data)
{
	return F_Write(flash_addr, &u8Data, 1);
}

BOOLEAN F_Write_u16(UINT32 flash_addr, UINT16 u16Data)
{
	return F_Write(flash_addr, (UINT8 *)&u16Data, 2);
}

BOOLEAN F_Write_u32(UINT32 flash_addr, UINT32 u32Data)
{
	return F_Write(flash_addr, (UINT8*)&u32Data, 4);
}

UINT8 F_Read_u8(UINT32 flash_addr)
{
	UINT8 u8Data;
	F_Read(flash_addr, &u8Data, 1);
	return u8Data;
}

UINT16 F_Read_u16(UINT32 flash_addr)
{
	UINT16 u16Data;
	F_Read(flash_addr, (UINT8*)&u16Data, 2);
	return u16Data;
}

UINT32 F_Read_u32(UINT32 flash_addr)
{
	UINT32 u32Data;
	F_Read(flash_addr, (UINT8*)&u32Data, 4);
	return u32Data;
}


