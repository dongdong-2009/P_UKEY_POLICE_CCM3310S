/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:debug.h                            // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
#ifdef  DEBUG_SYMBOL
#define	DEBUG_EXT
#else	
#define	DEBUG_EXT extern
#endif




#define    ASSERT_ENABLE       // 若使用断言测试
#define    NOP     asm {mov r0,r0}


DEBUG_EXT   unsigned char * p_u8CurrentFileName;
DEBUG_EXT 	unsigned char * p_u8CurrentLineNumber;

DEBUG_EXT 	void ExitProgram (void);
DEBUG_EXT 	void SendString(unsigned char * p_u8String);
DEBUG_EXT 	void AssertReport( unsigned char * p_u8FileName,unsigned char * p_u8LineNumber );
DEBUG_EXT    void UART_printf(char *fmt,...);
DEBUG_EXT    void HAL_UART_SEND_ASC(unsigned char* pInData,unsigned short len);




#ifdef ASSERT_ENABLE       // 若使用断言测试	
	#define  ASSERT_REPORT       AssertReport( p_u8CurrentFileName,p_u8CurrentLineNumber )
#else // 若不使用断言测试
	#define ASSERT_REPORT        NOP
#endif /* end of ASSERT */

