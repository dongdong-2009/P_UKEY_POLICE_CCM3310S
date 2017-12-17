/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:startup.c                          // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
extern void (*const __vector_table[64])(void);
extern void __init_user(void);
extern void (*const __vector_table[64])(void);
extern void main();


extern int *__bss_begin;                //  start of .bss section
extern int *__bss_end;                  //  end of .bss section
extern int *__stack_begin;              //  start of stack section
extern int *__stack_end;                //  end of stack section




asm void _start(void)
{

// initialize the Vector Base Register (VBR)
	lrw     r3, __vector_table                     //把__vector_table放入r3
	mtcr    r3, cr1                                //把__vector_table放入vbr寄存器
 
//Close watchdog
	lrw	    r3, 0x0e
	lrw     r2, 0x00c60000
	st.w    r3, (r2,0)

//Close TC
	lrw   r3,0x00
	lrw   r2,0x00d30000
	st.w  r3,(r2,0)
  
// initialize stack pointer
	lrw     r10, __stack_begin
	mov     r0, r10                       
	
	
	
// zero out .bss section
	lrw     r5, __bss_begin
	lrw     r6, __bss_end
	
	cmphs   r5, r6                         //如果r5>=r6,C位置1
	bt      _end_zero_bss                  //如果C为1，则跳转到_end_zero_bss
	xor     r4, r4                         //r4清0

_zero_bss:

	subi    r6, 1                          //r6的值减1
	st.b    r4, (r6,0)                     //把0放入r6中给出的地址  （从最后的地址向前减地址）
	cmplt   r5, r6                         //如果r5<r6,C位置1
	bt      _zero_bss                      //如果C为1，则跳转到_zero_bss
	
_end_zero_bss:

// copy ROM to RAM
	bsr     __init_user                    //跳转到__init_user
	
//  program entry point
	bsr   main                             //跳转到main

}



