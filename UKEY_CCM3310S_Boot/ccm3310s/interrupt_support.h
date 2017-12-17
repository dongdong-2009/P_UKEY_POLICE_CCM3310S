/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:interrupt_support.h                // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
extern unsigned long Read_PSR(void);
extern void Write_PSR(unsigned long);

extern unsigned long Read_VBR(void);
extern void Write_VBR(unsigned long);

extern asm void Disable_Ints();
extern asm void Enable_Ints();

#define ALLOW_AUTOVECTORS		0x8000		
#define ALLOW_VECTORS			0x0000		

#define VECTOR_OFFSET	32

#define PRIORITY_LEVEL_31		31		
#define PRIORITY_LEVEL_30		30
#define PRIORITY_LEVEL_29		29
#define PRIORITY_LEVEL_28		28	
#define PRIORITY_LEVEL_27		27		
#define PRIORITY_LEVEL_26		26
#define PRIORITY_LEVEL_25		25
#define PRIORITY_LEVEL_24		24
#define PRIORITY_LEVEL_23		23		
#define PRIORITY_LEVEL_22		22
#define PRIORITY_LEVEL_21		21
#define PRIORITY_LEVEL_20		20
#define PRIORITY_LEVEL_19		19		
#define PRIORITY_LEVEL_18		18
#define PRIORITY_LEVEL_17		17
#define PRIORITY_LEVEL_16		16
#define PRIORITY_LEVEL_15		15		
#define PRIORITY_LEVEL_14		14
#define PRIORITY_LEVEL_13		13
#define PRIORITY_LEVEL_12		12
#define PRIORITY_LEVEL_11		11		
#define PRIORITY_LEVEL_10		10
#define PRIORITY_LEVEL_09		9
#define PRIORITY_LEVEL_08		8
#define PRIORITY_LEVEL_07		7		
#define PRIORITY_LEVEL_06		6
#define PRIORITY_LEVEL_05		5
#define PRIORITY_LEVEL_04		4
#define PRIORITY_LEVEL_03		3		
#define PRIORITY_LEVEL_02		2
#define PRIORITY_LEVEL_01		1
#define PRIORITY_LEVEL_00		0	


#define PRIORITY_LEVEL_31_INTS 	0x80000000
#define PRIORITY_LEVEL_30_INTS 	0x40000000
#define PRIORITY_LEVEL_29_INTS 	0x20000000
#define PRIORITY_LEVEL_28_INTS 	0x10000000
#define PRIORITY_LEVEL_27_INTS 	0x08000000
#define PRIORITY_LEVEL_26_INTS 	0x04000000
#define PRIORITY_LEVEL_25_INTS 	0x02000000
#define PRIORITY_LEVEL_24_INTS 	0x01000000
#define PRIORITY_LEVEL_23_INTS 	0x00800000
#define PRIORITY_LEVEL_22_INTS 	0x00400000
#define PRIORITY_LEVEL_21_INTS 	0x00200000
#define PRIORITY_LEVEL_20_INTS 	0x00100000
#define PRIORITY_LEVEL_19_INTS 	0x00080000
#define PRIORITY_LEVEL_18_INTS 	0x00040000
#define PRIORITY_LEVEL_17_INTS 	0x00020000
#define PRIORITY_LEVEL_16_INTS 	0x00010000
#define PRIORITY_LEVEL_15_INTS 	0x00008000
#define PRIORITY_LEVEL_14_INTS 	0x00004000
#define PRIORITY_LEVEL_13_INTS 	0x00002000
#define PRIORITY_LEVEL_12_INTS 	0x00001000
#define PRIORITY_LEVEL_11_INTS 	0x00000800
#define PRIORITY_LEVEL_10_INTS 	0x00000400
#define PRIORITY_LEVEL_09_INTS 	0x00000200
#define PRIORITY_LEVEL_08_INTS 	0x00000100
#define PRIORITY_LEVEL_07_INTS 	0x00000080
#define PRIORITY_LEVEL_06_INTS 	0x00000040
#define PRIORITY_LEVEL_05_INTS 	0x00000020
#define PRIORITY_LEVEL_04_INTS 	0x00000010
#define PRIORITY_LEVEL_03_INTS 	0x00000008
#define PRIORITY_LEVEL_02_INTS 	0x00000004
#define PRIORITY_LEVEL_01_INTS 	0x00000002
#define PRIORITY_LEVEL_00_INTS 	0x00000001
