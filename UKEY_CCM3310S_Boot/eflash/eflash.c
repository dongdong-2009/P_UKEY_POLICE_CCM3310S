/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:eflash.c                           // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
#define  EFLASH_SYMBOL
#include "includes.h"

/**********************************************************************
*
*  时钟配置
*
**********************************************************************/
void set_clkd(UINT32 f_sys_khz)
{
	UINT8 ctmp = 0;
	
	if(f_sys_khz > 64000)
	{
		ctmp = (unsigned char)((f_sys_khz / 8) / 1000);
		ctmp |= EFLASH_CLKD_PRDIV8;
	}
	else
	{
		ctmp = (unsigned char)( f_sys_khz / 1000);
	}
	
	EFLASHCLKD = 0x4C;
	//write EFMTIM
	EFLASHTIM = 0x040805E8;
}
///////////////////////////////////////////////////////////////////////


/**********************************************************************
*
*  EFLASH 操作示例
*
**********************************************************************/
void EFLASHDemo()
{
	UINT32 i;
	UINT32 data;
	UINT32 addr = 0x00000000; //master mode,eflash baseAddr
	

	//Enter High Power Mode
	set_HighPowerMode(0,10);  

	for(i=0x00030000;i<CHIP_BYTE_SIZE;i+=PAGE_BYTE_SIZE)
	{
		EFLASH_page_erase(addr+i);
	}

	for(i=0x00030000;i<CHIP_BYTE_SIZE;i += 4)
	{
		EFLASH_program(addr+i,i*0x12345677);
	}

	for(i=0x00030000;i<CHIP_BYTE_SIZE;i += 4)
	{
		data = *(unsigned int *)(addr + i);
		if(data != i*0x12345677)
		{
			asm{bkpt} //编程校验出错
			break;
		}
	}

	//Exit High Power Mode
	set_HighPowerMode(1,10);  // 

}

