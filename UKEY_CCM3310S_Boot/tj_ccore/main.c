/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:main.c                             // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
#define  MAIN_SYMBOL
#include "includes.h"





void main()
{
	UINT8 i;
    //unsigned char a_u8FileNameBuffer[]   = "main.c";
    //unsigned char a_u8LineNumberBuffer[] = "0000";
    //unsigned char a_u8ReportPass[] = "SysClk = 40MHz";
	unsigned char buffer[512];
	ModuleClk_On(MODULE_CLK_SCI|MODULE_CLK_USBC);	//打开SCI、USB模块的时钟
	SetClk();

	//return boot
	//EnBootMode();

	//eflash clock
    set_clkd(g_sys_clk);
	
	//uart init
//	UARTInit(g_sys_clk*1000,9600,TEN,ParityDIS,EVE);


	//des enable
	*(volatile unsigned int *)(0x00c30004) &= ~0x300200;  //enable
	DES_Enable();

	//usb init	
	USBInit();
    
	//var init
	MID_GVAR_Init();

	//boot init		
	BOOT_INIT();
	
	Disable_Interrupts;
	NIER_reg=0x0;
	ICR_reg=0;
	PLSR25_reg=PRIORITY_LEVEL_4;
	NIER_reg |= 0x10;
	Enable_Interrupts;

	while(1)
	{
		HAL_USB_BulkOut(1,g_b_Comm,&g_BulkOutlen);
		if(0!=g_BulkOutlen)
		{
			ccid_data_process();					
		}	
	}
						
}			



void init_cos()
{
	unsigned char uchFlag = 0;
	unsigned int	AppCode = 0;
	
	
	uchFlag = (*(volatile unsigned char*)BOOTLOADER_FLAG_ADDR);
	AppCode = (*(volatile unsigned int*)APP_FLASH_ADDR);

	if((uchFlag == 0x55) && (AppCode != 0xffffffff))
	{
		asm
		{
			lrw r1, APP_START;
			jmp r1;	
		}	
	}
	else
	{
		asm
		{
			bsr   main
		}
	}	
}
	
