/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:eflash.h                           // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
#ifdef  EFLASH_SYMBOL
#define	EFLASH_EXT
#else	
#define	EFLASH_EXT extern
#endif

EFLASH_EXT void set_clkd(UINT32 f_sys_khz);


extern unsigned char (*EFLASH_page_erase)(unsigned int);
extern void (*set_HighPowerMode)(unsigned char, unsigned char);
extern unsigned char (*EFLASH_program)(unsigned int, unsigned int);

