/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:uart.h                             // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
#ifdef  UART_SYMBOL
#define	UART_EXT
#else	
#define	UART_EXT extern
#endif

UART_EXT void UARTInit(unsigned long u32Fsys,unsigned long u32BaudRate,unsigned char u8FrameLen,unsigned char u8ParityEnable,unsigned char u8ParityType);
UART_EXT void SendOneByte(unsigned char u8OutByte);



