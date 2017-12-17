							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usb.c
Author:        Version:        Date: 20121230
Description:  usb模块操作
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define  USB_SYMBOL
#include "includes.h"


void USB_Init(void)
{
	PHYPA   |=  USB_WORDSWITCH;                                  //配置usb PHY
	gUSBC_fifoReg  = (sUSBCFIFO_Reg*)(USBC_BASE + 0x1A);
	gUSBC_ComReg   = (sUSBCCommonReg*)USBC_BASE;
	gUSBC_IdxReg   = (sUSBCIndexedReg*)(USBC_BASE + 0x10);
	
	gpram_bSuspendSupport    = 0;                               //禁能Suspend功能
	gUSBC_ComReg->INTRUSBE   =   USB_INTERRUPT_RESET            //使能USB中断
	                            |USB_INTERRUPT_CONNECT
	                            |USB_INTERRUPT_DISCON
	                            |USB_INTERRUPT_SUSPEND
	                            |USB_INTERRUPT_RESUME;

#ifdef USB_USEINT
	gUSBC_ComReg->INTRTXE_L   =  USB_INTERRUPT_EP0              //使能EP0中断和EP1发送中断             
	                            |(1<<INDEX_EP1);
	gUSBC_ComReg->INTRRXE_L   =  (1<<INDEX_EP1);       //使能EP1接收中断                         
#else		//#ifdef USB_USEINT
	gUSBC_ComReg->INTRTXE_L = 0;
	gUSBC_ComReg->INTRRXE_L = 0;
#endif		//#ifdef USB_USEINT
	gUSBC_ComReg->EINDEX   =  CONTROL_EP;                       //把EP0设为当前要操作的端点	
	
	if(gpram_usbVer == 1)
		gUSBC_ComReg->UCSR   = USB_POWER_SOFT_CONN | USB_POWER_HS_ENAB;    //打开软连接和高速模式
	else
		gUSBC_ComReg->UCSR  = USB_POWER_SOFT_CONN;
			
	gpram_USBAddrFlag   = 0;
	gpram_USBNewAddr    = 0;

	gpram_cbwPacket=(unsigned char*)gpram_cbwPacket_l;
	gpram_cswPacket[0]=0x55;
	gpram_cswPacket[1]=0x53;
	gpram_cswPacket[2]=0x42;
	gpram_cswPacket[3]=0x53;
}

void USB_DisConnect(void)
{
	gUSBC_ComReg->UCSR  &= ~USB_POWER_SOFT_CONN;
}

void USB_Connect(void)
{
	gUSBC_ComReg->UCSR  |= USB_POWER_SOFT_CONN;
}


void USBDev_Init(void)
{
	gpram_usbVer   = 1;		//高速USB
	//gpram_usbVer   = 0;		//全速USB
	USB_Init();
}


void USBDev_BusReset(UINT8 flag)
{
	UINT8 ucMode;
	UINT8 tempL,tempH;
	UINT32 i=0;
	UINT16 EPxBufAddr;
	
	if (flag == 0)
	{
		while (i!= 0x3FFFF)
		{
			i++;			
		}
		gUSBC_ComReg->FADDRR   = 0 ;    //复位后usb设备地址为0
	}
#ifndef USB_USEINT			//在USB_Init中已经关中断了，为什么后面又打开了?这里重新关中断
	gUSBC_ComReg->INTRTXE_L = 0;
	gUSBC_ComReg->INTRRXE_L = 0;
#endif

	gUSBC_ComReg->EINDEX  = INDEX_EP1;   //把EP1设为当前要操作的端点 
	if( gpram_usbVer == 1 )
	{
		gUSBC_fifoReg->TX_fifosz   = 0x06;     //512 Bytes
		gUSBC_fifoReg->RX_fifosz   = 0x06;
	}
	else
	{
		gUSBC_fifoReg->TX_fifosz   = 0x03;     //64Bytes
		gUSBC_fifoReg->RX_fifosz   = 0x03;
	}

	gUSBC_fifoReg->TX_fifoadd_L    = ((USB_FIFO_OFFSET>>3)&0x00FF);	   
	gUSBC_fifoReg->TX_fifoadd_H    = ((USB_FIFO_OFFSET>>11)&0x00FF);	
	gUSBC_fifoReg->RX_fifoadd_L    = ((USB_FIFO_OFFSET>>3)&0x00FF);	
	gUSBC_fifoReg->RX_fifoadd_H    = ((USB_FIFO_OFFSET>>11)&0x00FF);

	if(gpram_usbVer  == 1)
	{
		tempL  = USB_MAX_PACKET_SIZE_LOW;       
		tempH  = USB_MAX_PACKET_SIZE_HIGH;      //512
	}
	else
	{
		tempL  = USB_MAX_PACKET_SIZE_LOW_V11;	//64				 
		tempH  = USB_MAX_PACKET_SIZE_HIGH_V11;
	}
	gUSBC_IdxReg->TXCSR_L   = 0x40;                //从DATA0开始发数  

	
	#ifdef  USB_AUTOSET_TEST
	gUSBC_IdxReg->TXCSR_H  = 0x80;                 //自动发送，当放到FIFO中的值和包大小一致时，自动会生成TxPktRdy,并发送	                                        
	#endif
	
	gUSBC_IdxReg->TXPSZR_L   = tempL;              //发送包的大小设置
	gUSBC_IdxReg->TXPSZR_H   = tempH;
	
	gUSBC_IdxReg->RXCSR_L   = 0x80;                //从DATA0开始收数
	gUSBC_IdxReg->RXCSR_H   = 0x0;
	  
	gUSBC_IdxReg->RXPSZR_L   = tempL;              //接收包的大小设置
	gUSBC_IdxReg->RXPSZR_H   = tempH;
	  
	gUSBC_IdxReg->TXCSR_L  = DEV_TXCSR_FLUSH_FIFO;    //清FIFO
	gUSBC_IdxReg->RXCSR_L  = DEV_RXCSR_FLUSH_FIFO;

	gpram_bulkPhase = 0;	//Clear SCSI Command

}
	
	
