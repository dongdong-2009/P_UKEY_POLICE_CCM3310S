							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usbdev_isr.c
Author:        Version:        Date: 20121230
Description:  usb设备中断处理
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define	USBDEV_ISR_SYMBOL
#include "includes.h"


/*************************************************
Function: USBDev_ISR
Description: USB中断处理
Calls:  
Called By:  
Input: 无
Output: 无
Return: 无
Others: 无
*************************************************/
void USBDev_ISR(void)
{
	UINT8  ucISR, index;
	
	index = gUSBC_ComReg->EINDEX;	//解决USB中断模式的问题。先保存端点，执行完后恢复端点。
	
	gpram_msgflags |= gUSBC_ComReg->INTRUSB;                   //取走USB中断寄存器中的值

#ifdef USB_USEINT
	gpram_msgflags1 |= gUSBC_ComReg->INTRTX_L;                 //取走USB的发送中断寄存器中的值
	ucISR = gUSBC_ComReg->INTRRX_L;                        //取走USB的接收中断寄存器中的值

	//EP1~EP7接收中断
	g_uchUSBRcvStatus = (ucISR&0xFE); 
	
	//EP0中断
	if(gpram_msgflags1 & 0x1)
	{
		if (gpram_USBAddrFlag == 1)                        //如果地址改写了
		{
			//设置地址后会产生SetupEnd中断?
			gUSBC_ComReg->FADDRR = gpram_USBNewAddr;   //把新地址放进去进行操作
			gpram_USBAddrFlag  = 0;
		}
		gpram_msgflags1 &= 0xFE;                           //清EP0中断标志		
		USBDev_EP0_ISR();
	}
	
	//Clear Last Completed Command		
	if(gpram_bulkPhase == 0xFF)
	{
		gpram_msgflags1 &= (~(1<<INDEX_EP1));
	}
	//EP1~EP7发送中断
	if (gpram_msgflags1 & (1<<INDEX_EP1) )  
	{
		gpram_msgflags1 &= ( ~(1<<INDEX_EP1) );   //清EP1发送中断标志
		g_uchUSBSendStatus |= (1<<INDEX_EP1);					//连续发送多个包时，用来判断发送结束
	}
	
#endif

	//bus reset
	if (gpram_msgflags & 0x04)  
	{
		gpram_msgflags &= 0xFB; 		
		//SendOneByte(0x01);		                      
		USBDev_BusReset(0);	    
	}

	//suspend
	if(gpram_msgflags & 0x1)     
	{
		gpram_msgflags &= 0xFE;  		
		//SendOneByte(0x02);		                           
	}

	//sof
	/*
	if(((gpram_msgflags & 0x08) == 0x08) && (gpram_msgflags1 == 0 ))    //如果是帧开始且发送中断都没有置位(考虑到SOF太经常有了，建议去掉此分支)
	{
	    gpram_msgflags &= 0xF7;  
		
		SendOneByte(0x03);
		
		if(gpram_bDozeSupport)
		{}
	}
	 */		
	gUSBC_ComReg->EINDEX = index; 	//恢复端点
}

/*************************************************
Function: USBDev_EP0_ISR
Description: USB端点0的中断处理
Calls:  
Called By:  
Input: 无
Output: 无
Return: 无
Others: 无
*************************************************/
void USBDev_EP0_ISR(void)
{
	//INT8U ucReqType=0,ucCount=0;
	UINT8 ucReg=0;

	gUSBC_ComReg->EINDEX  = CONTROL_EP;     
	ucReg= gUSBC_IdxReg->E0CSR_L;
	
	//clear SentStall bit
	if(ucReg &0x04)      //如果SendStall发送完成，则清标志
	{
		ucReg &=0xFB;	
		//printf_debug((INT8U*)"SendStall_Finish!", 17, 1, 1);
	}

	if(ucReg & 0x10)     //	如果setup end发生，那么清setup end标志位
	{
		ucReg |=0x80;
		//printf_debug((INT8U*)"SetupEnd", 8, 1, 1);
	}
  
	gUSBC_IdxReg->E0CSR_L  = ucReg;

	if(ucReg & DEV_CSR0_RXPKTRDY)    //如果接收到数据包
	{
		USBDev_Ep0Handler();
	} 
}

/*
Description:Receive data sent from host.(BULK-OUT)
*/
void USBDev_EP1_RX_ISR(void)
{
	unsigned char ucRegLow;
	
	//access DATA_OUT_EP register map
	gUSBC_ComReg->EINDEX = INDEX_EP1;
	ucRegLow = gUSBC_IdxReg->RXCSR_L;
	//Clear sentstall and restart data toggle.
	if(ucRegLow & DEV_RXCSR_SENTSTALL)
	{
		ucRegLow &=0xBF;//clear SendStall bit
		ucRegLow |=DEV_RXCSR_CLR_DATA_TOG;//set ClrDataTog
		gUSBC_IdxReg->RXCSR_L = ucRegLow;
	}

	//Start receive data packet
	if(ucRegLow & DEV_RXCSR_RXPKTRDY)	 //RxPktRdy=1
	{
		USBDev_Data_BulkOut();
	}
	
}


/*
Description:Send data to host.(BULK-IN)
*/
void USBDev_EP1_TX_ISR(void)
{
	//access DATA_OUT_EP register map
	gUSBC_ComReg->EINDEX = INDEX_EP1;
	USBDev_Data_BulkIn();
}

