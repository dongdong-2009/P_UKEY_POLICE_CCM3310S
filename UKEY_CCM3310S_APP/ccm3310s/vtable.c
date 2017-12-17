/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:vtable.c                           // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
extern  asm void _start(void);
extern  void ExceptionProc(void);
extern asm void USB_int_Service(void);

//定义了一个指向函数的指针数组
/* Vector Table Initialization ************************************************/

void (*const __vector_table[64])(void) = {
  &_start,				    //	0 0x000  
  &ExceptionProc,			//	1 0x004  
  &ExceptionProc,			//	2 0x008  
  &ExceptionProc,			//	3 0x00C  
  &ExceptionProc,			//	4 0x010  
  &ExceptionProc,			//	5 0x014  
  &ExceptionProc,			//	6 0x018  
  &ExceptionProc,			//	7 0x01C  
  &ExceptionProc,			//	8 0x020  
  &ExceptionProc,			//	9 0x024  
  &ExceptionProc,			//	10 0x028  
  &ExceptionProc,			//	11 0x02C  
  &ExceptionProc,			//	12 0x030  
  &ExceptionProc,			//	13 0x034  
  &ExceptionProc,			//	14 0x038  
  &ExceptionProc,			//	15 0x03C  
  &ExceptionProc,			//	16 0x040  
  &ExceptionProc,			//	17 0x044  
  &ExceptionProc,			//	18 0x048  
  &ExceptionProc,			//	19 0x04C  
  &ExceptionProc,			//	20 0x050  
  &ExceptionProc,			//	21 0x054  
  &ExceptionProc,			//	22 0x058  
  &ExceptionProc,			//	23 0x05C  
  &ExceptionProc,			//	24 0x060  
  &ExceptionProc,			//	25 0x064  
  &ExceptionProc,			//	26 0x068  
  &ExceptionProc,			//	27 0x06C  
  &ExceptionProc,			//	28 0x070  
  &ExceptionProc,			//	29 0x074  
  &ExceptionProc,			//	30 0x078  
  &ExceptionProc,			//	31 0x07C  
  /////////////////////////////////
  
  &ExceptionProc,			//	32 0x080
  &ExceptionProc,			//	33 0x084  
  &ExceptionProc,			//	34 0x088  
  &ExceptionProc,			//	35 0x08C 
  
  &USB_int_Service,			//	36 0x090 
  
  &ExceptionProc,			//	37 0x094  
  &ExceptionProc,			//	38 0x098  
  &ExceptionProc,			//	39 0x09C  
  &ExceptionProc,			//	40 0x0A0  
  &ExceptionProc,			//	41 0x0A4  
  &ExceptionProc,			//	42 0x0A8  
  &ExceptionProc,			//	43 0x0AC  
  &ExceptionProc,			//	44 0x0B0  
  &ExceptionProc,			//	45 0x0B4  
  &ExceptionProc,			//	46 0x0B8  
  &ExceptionProc,			//	47 0x0BC  
  &ExceptionProc,			//	48 0x0C0  
  &ExceptionProc,			//	49 0x0C4  
  &ExceptionProc,			//	50 0x0C8  
  &ExceptionProc,			//	51 0x0CC  
  &ExceptionProc,			//	52 0x0D0  
  &ExceptionProc,			//	53 0x0D4  
  &ExceptionProc,			//	54 0x0D8  
  &ExceptionProc,			//	55 0x0DC  
  &ExceptionProc,			//	56 0x0E0  
  &ExceptionProc,			//	57 0x0E4  
  &ExceptionProc,			//	58 0x0E8
  &ExceptionProc,			//	59 0x0EC 
  &ExceptionProc,			//	60 0x0F0  
  &ExceptionProc,			//	61 0x0F4  
  &ExceptionProc,			//	62 0x0F8
  &ExceptionProc,			//	63 0x0FC  
 
};



#pragma interrupt on
void ExceptionProc(void)
{

}
#pragma interrupt off


