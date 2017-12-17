/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:uart.c                             // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
#define  UART_SYMBOL
#include "includes.h"


//**********************************************************************/
void UARTInit(unsigned long u32Fsys,unsigned long u32BaudRate,unsigned char u8FrameLen,unsigned char u8ParityEnable,unsigned char u8ParityType)
{
    
    //4800,9600,19200,38400,115200 are baudrate typical values
    
    rcSCIBD = (unsigned short)(u32Fsys/(16*u32BaudRate));
    
    rcSCICR1  = 0x00;
    rcSCICR1 |= u8FrameLen;   	
    rcSCICR1 |= u8ParityEnable;	      //parity enable
    rcSCICR1 |= u8ParityType;	      //odd parity
    rcSCICR2  = 0; //普通 			  	
    //rcSCICR2 = SCICR2_RIE_MASK|SCICR2_TIE_MASK|SCICR2_TCIE_MASK|SCICR2_ILIE_MASK;	
          
}
///////////////////////////////////////////////////////////////////////


/**********************************************************************
*
* 
*
**********************************************************************/
void SendOneByte(unsigned char u8OutByte)
{	
    SCI_ENABLE_TRANSMITTER;
     	
    while((rcSCISR1 & SCISR1_TDRE_MASK)==0);	
    rcSCIDRL = u8OutByte;
    while((rcSCISR1 & SCISR1_TC_MASK)==0);
}
///////////////////////////////////////////////////////////////////////

