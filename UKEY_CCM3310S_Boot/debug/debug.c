/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:debug.c                            // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
#define  DEBUG_SYMBOL
#include "includes.h"

void ExitProgram (void)
{    
	while(1) 
	{}
}

void SendString(unsigned char * p_u8String)
{
	unsigned char * p_u8PresentChar;
	p_u8PresentChar  = p_u8String;
	while ( *p_u8PresentChar != '\0')
	{
		 SendOneByte(*p_u8PresentChar);
		 p_u8PresentChar++;
	}
}




void AssertReport( unsigned char * p_u8FileName,unsigned char * p_u8LineNumber )
{
    unsigned char a_u8ReportString[] = "[ASSERT]Error Report:\n";
    SendString(a_u8ReportString);
    SendString(p_u8FileName);
    SendOneByte(',');
    SendString(p_u8LineNumber);	
    SendOneByte('!');
    SendOneByte('\n');
    ExitProgram();
}

void HAL_UART_SendData(unsigned char *data, unsigned short len)
{
	unsigned int i;
	unsigned short rv=0;
	
	for(i=0; i<len; i++)
	{		
		 SendOneByte(*(data+i));			
	}
	
}

void UART_printf(char *fmt,...)
{
//	unsigned char temp[256];
//	va_list arg_ptr;
////	unsigned char *string=pUARTData;
//	unsigned char *string=temp;	
//	va_start(arg_ptr, fmt);  
//	vsprintf((char *)string,fmt,arg_ptr); 
//    	HAL_UART_SendData(string,strlen((char *)string));
//	va_end(arg_ptr);
}


void HAL_UART_SEND_ASC(unsigned char* pInData,unsigned short len)
{	
	unsigned char temp[4];
	unsigned char m;
	unsigned short i,len1;
	
	len1=2*len;
	for(i=0;i<len;i++)
	{
		m=(pInData[i]>>4)&0x0f;
		if(m>=0&&m<=9)
			m+=0x30;
		else
			m+=55;
		temp[0]=m;
		m=pInData[i]&0x0f;
		if(m>=0&&m<=9)
			m+=0x30;
		else
			m+=55;
		temp[1]=m;
		temp[2]=' ';
		HAL_UART_SendData(temp,3);
	}
	temp[0]=0x0a;
	HAL_UART_SendData(temp,1);
}




