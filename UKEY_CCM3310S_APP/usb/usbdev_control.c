							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usbdev_control.c
Author:        Version:        Date: 20121230
Description:  usb设备枚举过程的实现
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define  USBDEV_CONTROL_SYMBOL	
#include "includes.h"

/*************************************************
Function: read_ep_buf
Description: 从FIFO中读数据
Calls:  
Called By:  
Input: 
    INT8U* out_buf:读出数据的起始地址
    INT32U ep:FIFO地址
    INT16U uiLens:读出的数据长度
Output: 无
Return: 无
Others: 无
*************************************************/
void read_ep_buf(UINT8 *out_buf,UINT32 ep,UINT16 uiLens)
{
	UINT16 i=0;
	for (i=0;i<uiLens;i++)
	{
		out_buf[i] = read_port_byte(ep);
	}
}

/*************************************************
Function: write_ep_buf
Description: 向FIFO中写数据
Calls:  
Called By:  
Input: 
    INT32U ep:FIFO地址
    INT8U* out_buf:要写入数据的起始地址
    INT16U uiLens:写入的数据长度
Output: 无
Return: 无
Others: 无
*************************************************/
void write_ep_buf(UINT32 ep,UINT8 *out_buf,UINT16 uiLens)
{
	UINT16 i;
	for (i=0;i<uiLens;i++)
	{
		write_port_byte(ep,out_buf[i]);
	}
}

/*************************************************
Function: write_ep0_buf
Description: 通过端点0发送数据，最后一包
Calls:  
Called By:  
Input: 
    INT8U* in_buf:要发送的数据的起始地址
    INT16U uiLens:要发送的数据长度
Output: 无
Return: 无
Others: 无
*************************************************/
void write_ep0_buf(UINT8 *in_buf, UINT16 uiLen)
{
	UINT8  ucReg  = gUSBC_IdxReg->E0CSR_L;
	write_ep_buf(USB_FIFO_ENDPOINT_0,in_buf,uiLen);	    
	ucReg  |=  DEV_CSR0_DATAEND | DEV_CSR0_TXPKTRDY;    // buffer填满后，告诉USB开始发送
	gUSBC_IdxReg->E0CSR_L  =  ucReg;	
}

/*************************************************
Function: write_ep0_buf_notEnd
Description: 通过端点0发送数据，不是最后一包
Calls:  
Called By:  
Input: 
    INT8U* in_buf:要发送的数据的起始地址
    INT16U uiLens:要发送的数据长度
Output: 无
Return: 无
Others: 无
*************************************************/
void write_ep0_buf_notEnd(UINT8 *in_buf, UINT16 uiLen)
{
	UINT8  ucReg  = gUSBC_IdxReg->E0CSR_L;
	write_ep_buf(USB_FIFO_ENDPOINT_0,in_buf,uiLen);	    
	ucReg  |=  DEV_CSR0_TXPKTRDY;    // buffer填满后，告诉USB开始发送，不是最后一包
	gUSBC_IdxReg->E0CSR_L  =  ucReg;	
}

void write_ep0_buf_long(UINT16 requstLen, UINT8 *databuf, UINT16 dataLen)
{
	UINT16 sentLen = 0;

	if (dataLen > USB_MAX_PACKET_SIZE_EP0)		//描述符大小超过一个包大小，需分包发送
	{
		if (requstLen > USB_MAX_PACKET_SIZE_EP0)
		{
			//TODO:使用中断方式时CDC类返回配置描述符有问题
#ifdef USB_USEINT		
			gUSBC_ComReg->INTRTXE_L &=0xfe;		//关闭EP0中断
#endif
			while ((requstLen-sentLen)>USB_MAX_PACKET_SIZE_EP0)
			{
				write_ep0_buf_notEnd(databuf+sentLen,USB_MAX_PACKET_SIZE_EP0);
				while((gUSBC_ComReg->INTRTX_L &0x01)==0x00);
				sentLen += USB_MAX_PACKET_SIZE_EP0;
			}
			write_ep0_buf(databuf+sentLen,requstLen-sentLen);
#ifdef USB_USEINT			
			gUSBC_ComReg->INTRTXE_L |=0x01;		//打开EP0中断
#endif
		}
		else
		{
			write_ep0_buf(databuf,requstLen);
		}
	}
	else	//一包就可发送完成
	{
		requstLen = (requstLen>dataLen)?dataLen:requstLen;		//host可能会发送长度字节为FF的请求
		write_ep0_buf(databuf,requstLen);
	}
}
			
void  USBDev_EP0_SendStall(void)
{
	UINT8  ucReg = gUSBC_IdxReg->E0CSR_L;
	ucReg |= DEV_CSR0_SENDSTALL;
	gUSBC_IdxReg->E0CSR_L   = ucReg;
}


/*************************************************
Function: USBDev_Ep0Handler
Description: 端点0的数据处理
Calls:  
Called By:  
Input: 无
Output: 无
Return: 无
Others: 无
*************************************************/
void USBDev_Ep0Handler(void)
{
	UINT8  ucReq  = 0;
	UINT16   wValue = 0;

#ifdef EP0_SETGET_REPORT
	if (setreport_flag == 1)		//已收到report，非setup包，而是out包
	{
		read_ep_buf((unsigned char*)report_buffer,USB_FIFO_ENDPOINT_0,64);
		USBDev_ClearRx0();		//芯片自动返回状态过程包
		setreport_flag = 0;		
		return;
	}
#endif	

#ifdef USB_CDC_CLASS
	if(gUSBC_IdxReg->E0COUNTR_L != 8)		//设置串口属性，非setup包，而是out包
	{
		read_ep_buf(LineCoding,USB_FIFO_ENDPOINT_0,7);
		USBDev_ClearRx0();	//|DEV_CSR0_DATAEND;		//芯片自动返回状态过程包
		return;
	}
#endif	
	
	read_ep_buf((UINT8*)&dev_std_req,USB_FIFO_ENDPOINT_0,8);		//TODO:EP0的接收FIFO大小默认为8字节
	USBDev_ClearRx0();
	//printf_debug((INT8U*)"EP0:", 4, 1, 0);		//TODO:不能加太多打印，否则通信有问题，要求返回响应时间不能太长?
	//printf_debug((INT8U*)&dev_std_req, 8, 0, 1);
	
	ucReq  = dev_std_req.bRequest;
	
	if(1 == ((dev_std_req.bmRequestType>>5) & 0x3))   //类请求
	{
		switch(ucReq)
		{
			case 0xFE://0xa1:GetMaxLUN
			{
				ucReq=gpram_maxLun & 0x7F;
				if(ucReq>4)
					ucReq=0;
				write_ep0_buf(&ucReq,1);	
				USBDev_BusReset(1);		
				break;	
			}
			case 0xFF://0x21:ClassReset
			{
				USBDev_BusReset(1);
				break;
			}
			default:
			{
				USBDev_EP0_SendStall();
				break;	
			}
		}
	}
	else		//标准请求
	{
		switch(ucReq)
		{
			case 0://GET_STATUS
			{
				USBDev_GetStatus();
				break;
			}
			case 1://CLEAR_FEATURE
			{				
				if(dev_std_req.bmRequestType == 0x2)//clear endpoint_halt, dev_std_req->bmRequestType			
					gpram_WakeupFeature = 0;
				break;
			}
			case 3://SET_FEATURE
			{
				if(dev_std_req.bmRequestType == 0x2)//halt endpoint,
					gpram_WakeupFeature = 1;
				if(dev_std_req.bmRequestType == 0x0)	//test mode
					USBDev_TestMode();
				break;
			}
			case 5:                               //USB协议，set address
			{	
				ucReq = dev_std_req.wValue>>8;
				gpram_USBNewAddr  =  ucReq;
				gpram_USBAddrFlag = 1;
				write_ep0_buf(&ucReq,0);		//只设置了寄存器标志，此句不要也行
				break;	
			}
			case 6://GET_DESCRIPTOR
			{
				USBDev_GetDescriptor();
				break;
			}	
			case 8://GET_CONFIGURATION
			{				
				ucReq=gpram_configFlag;	
				write_ep0_buf(&ucReq,1);			
				break;
			}
			case 9://SET_CONFIGURATION
			{				
				gpram_configFlag = dev_std_req.wValue & 0x0F;	
				write_ep0_buf(&ucReq,0);		//只设置了寄存器标志，此句不要也行
				break;
			}
			case 10://GET_INTERFACE
			{				
				ucReq = 0;		//Return Alternate Setting value		
				write_ep0_buf(&ucReq,1);	//set 1 data packet		
				break;	
			}
			case 11://SET_INTERFACE
			{
				write_ep0_buf(&ucReq,0);		//只设置了寄存器标志，此句不要也行
				break;	
			}
			default:			
			{
				USBDev_EP0_SendStall();
				break;
			}
		}
	}
}


/*************************************************
Function: USBDev_GetDescriptor
Description: USB描述符的处理
Calls:  
Called By:  
Input: 无
Output: 无
Return: 无
Others: 无
*************************************************/
void USBDev_GetDescriptor(void)
{
	UINT16 ucRegLen;
	UINT8 ucReg=((UINT8*)&dev_std_req)[6];//dev_std_req->wCount>>8;
	UINT8 ucReg2=((UINT8*)&dev_std_req)[7];//dev_std_req->wCount
	UINT8 reqIndex=((UINT8*)&dev_std_req)[3]; //=dev_std_req->wValue & 0x0F;
	UINT8 ucIndex=((UINT8*)&dev_std_req)[2];//=dev_std_req->wValue>>8;
	UINT16 i;
	UINT16 Configuration_Descriptor_Len;

	unsigned char ucReg_h;
	ucReg_h = dev_std_req.wCount &0xff;
	
	ucRegLen = ((UINT16)ucReg2<<8)+ucReg;
	Configuration_Descriptor_Len = (USB_Configuration_Descriptor[3]<<8)+USB_Configuration_Descriptor[2];

	switch(reqIndex)
	{
		case DEVICE_TYPE:		 //设备描述符
		{
			if(ucReg<=USB_Device_Descriptor[0])
			{
				write_ep0_buf((UINT8 *)USB_Device_Descriptor,ucReg);
			}
			else
			{
				write_ep0_buf((UINT8 *)USB_Device_Descriptor,8);
			}
			break;
		}
		
		case CONFIG_TYPE:		 //配置描述符
		{
			write_ep0_buf_long(ucRegLen, (UINT8*)USB_Configuration_Descriptor, Configuration_Descriptor_Len);
			break;
		}

		case STRING_TYPE:		 //字符串描述符
		{
			if(ucIndex==0)
			{
				write_ep0_buf((UINT8*)UsbLanguageID,4);
			}
			else if(ucIndex==2)
			{
				ucReg = (ucReg>srialnumString[0])?srialnumString[0]:ucReg;
				write_ep0_buf((UINT8*)srialnumString,ucReg);
			}
			else if(ucIndex==3)
			{
				ucReg = (ucReg>USB_String_Descriptor[0])?USB_String_Descriptor[0]:ucReg;
				write_ep0_buf((UINT8*)USB_String_Descriptor,ucReg);
			}
			else if(ucIndex==4)
			{	//serial code
				ucReg = (ucReg>USB_Manufacturer_Descriptor[0])?USB_Manufacturer_Descriptor[0]:ucReg;
				write_ep0_buf((UINT8*)USB_Manufacturer_Descriptor,ucReg);
			}

			break;
		}
		case INTERFACE_TYPE:                    //接口描述符
		{
			USBDev_EP0_SendStall();
			break;
		}
		case ENDPOINT_TYPE:                    //端点描述符
		{
			USBDev_EP0_SendStall();
			break;
		}
		case  DEVICE_QUALIFIER:
		{
			if(ucReg<=USB_Device_Qualifier_Descriptor[0])
			{
				write_ep0_buf((UINT8*)USB_Device_Qualifier_Descriptor,ucReg);
			}
			else
			{
				write_ep0_buf((UINT8*)USB_Device_Qualifier_Descriptor,8);		//如果超过设备描述符的实际长度，就只返回8字节
			}
			break;
		}
		
		case OTHER_SPEED:
		{
			if(ucReg<10)
			{
				write_ep0_buf((UINT8*)USB_Configuration_Descriptor,ucReg);
			}
			else
			{/*
				USBDEV_Configuration_Descriptor[22] = 0x40;
				USBDEV_Configuration_Descriptor[23] = 0x00;
				USBDEV_Configuration_Descriptor[29] = 0x40;
				USBDEV_Configuration_Descriptor[30] = 0x00;
				write_ep0_buf(USBDEV_Configuration_Descriptor,0x20);*/
			}
			break;
		}
		case INTERFACE_POWER:
		{
			USBDev_EP0_SendStall();
			break;
		}
#ifdef USB_HID_CLASS
		case REPORT_TYPE:                 //HID协议里规定的报告描述符
		{
#ifdef HID_512			
			ucReg = 0x36;
#else
			ucReg = 0x34;
#endif
			write_ep0_buf((unsigned char*)USB_report_Descriptor,ucReg);
			break;
		}
#endif
		default:
		{
			USBDev_EP0_SendStall();
			break;	
		}
	}
}

void USBDev_ClearRx0()
{
	//set ServiceRxPktRdy to clear RxPktRdy
	write_port_byte(0x01000012,DEV_CSR0_SERVICE_RXPKTRDY);
}

/*
	Description:USB standard command GetStatus
*/
void USBDev_GetStatus(void)
{
	UINT8 txdat[2]={0,0};

	switch(dev_std_req.bmRequestType)//dev_std_req->bmRequestType
	{
		case REQUEST_ENDPOINT_STATUS:
			if(gpram_WakeupFeature == 1)
			txdat[0]=1;	
		case REQUEST_DEVICE_STATUS:
		case REQUEST_INTERFACE_STATUS:
			write_ep0_buf(txdat,2);
			break;
		default:
			USBDev_EP0_SendStall();
			break;
	}	
}
	
	
/*
	Description:support usb high speed test mode
*/
void USBDev_TestMode(void)
{
	UINT8 test_packet[53] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 
		0xAA, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 
		0xEE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF, 
		0xEF, 0xF7, 0xFB, 0xFD, 0xFC, 0x7E, 0xBF, 0xDF, 
		0xEF, 0xF7, 0xFB, 0xFD, 0x7E};
	
	switch(((UINT8 *)&dev_std_req)[5])
	{
		case 1:		//Test_J
			write_port_byte( 0x0100000f,0x02);
			//asm{wait}
			break;
		case 2:		//Test_K
			write_port_byte( 0x0100000f,0x04);
			//asm{wait}
			break;
		case 3:		//Test_SE0_NAK
			write_port_byte( 0x0100000f,0x01);
			//asm{wait}
			break;
		case 4:		//Test_Packet
			write_ep_buf(USB_FIFO_ENDPOINT_0,test_packet,53);
			write_port_byte(0x01000012,0x02);
			write_port_byte(0x0100000f,0x08);
			//asm{wait}
			break;
		default:
			break;			
	}
}
	
	

/*************************************************
Function: usb_receive
Description: USB端点1接收一包数据
Calls:  
Called By:  
Input: 
    INT8U *buf:接收数据的起始地址
Output: 无
Return: 接收数据的长度
Others: 无
*************************************************/
UINT16 usb_receive(UINT8 *buf)
{
	UINT16   uiRxCount;
	UINT8    ucReg;
	uiRxCount   =  gUSBC_IdxReg->RXCount_H;
	uiRxCount   <<= 8;
	uiRxCount += gUSBC_IdxReg->RXCount_L;
	
	if(uiRxCount == 0)
		return;
	
	if(uiRxCount > 0)		//一包数据应小于512字节
	{
		read_ep_buf((UINT8 *)buf,g_uchUSBFifoAddr,uiRxCount);
		ucReg  = gUSBC_IdxReg->RXCSR_L;
		ucReg  &= 0xFE;                 //清除FIFO，允许下一个包传输
		gUSBC_IdxReg->RXCSR_L  = ucReg;
	}
	return  uiRxCount;
}

/*************************************************
Function: usb_send
Description: USB端点1发送一包数据
Calls:  
Called By:  
Input: 
    INT8U *buf:发送数据的起始地址
    INT16U len:发送数据的长度
Output: 无
Return: 无
Others: 无
*************************************************/
void usb_send(UINT8 *buf, UINT16 len)
{
	UINT8  ucReg;

	write_ep_buf(g_uchUSBFifoAddr,buf,len);
	usb_SetTxSize(len);                                      //发送的时候包的大小要设一下

	//set TxPktRdy=1
	//if(uiLength!=0x40)	//when length = 0x40,TxpktRdy autoset,  TODO:没有打开自动设置功能，且不能是固定的0x40
	{
		ucReg =	read_port_byte( 0x01000012);
		ucReg |= DEV_TXCSR_TXPKTRDY;
		write_port_byte( 0x01000012,ucReg);
#ifndef USB_USEINT		
		while((read_port_byte( 0x01000012)&DEV_TXCSR_TXPKTRDY) == DEV_TXCSR_TXPKTRDY);
#endif		
	}
}

void usb_SetTxSize(UINT16 Size)
{
	if(Size)
	{
		gUSBC_IdxReg->TXPSZR_L  = Size;
		gUSBC_IdxReg->TXPSZR_H  = (Size>>8);
	}
	else
	{
		if(gpram_usbVer  == 1)
		{
			gUSBC_IdxReg->TXPSZR_L  = USB_MAX_PACKET_SIZE_LOW;
			gUSBC_IdxReg->TXPSZR_H  = USB_MAX_PACKET_SIZE_HIGH;
		}
		else
		{
			gUSBC_IdxReg->TXPSZR_L  = USB_MAX_PACKET_SIZE_LOW_V11;
			gUSBC_IdxReg->TXPSZR_H  = USB_MAX_PACKET_SIZE_HIGH_V11;
		}
	}
}


void usb_ClearRxFlag(UINT8 EPindex)
{
    unsigned char ucReg = 0;

	gUSBC_ComReg->EINDEX = EPindex;
	//clear Rx
	ucReg = gUSBC_IdxReg->RXCSR_L;
	ucReg &=0xFE;	//Clear RxPktRdy
	gUSBC_IdxReg->RXCSR_L = ucReg;	
}

void usb_SetTxFlag(UINT8 EPindex)
{
	unsigned char ucReg = 0;
	
	gUSBC_ComReg->EINDEX = EPindex;
	
	ucReg = gUSBC_IdxReg->TXCSR_L;
	ucReg |= 0x01;	//Set TxPktRdy
	gUSBC_IdxReg->TXCSR_L = ucReg;	
}

//USB协议和应用BUF不同
void usb_SetFifoAddr(UINT8 EPindex, unsigned long fifoAddr, unsigned char flag)
{
	if(flag == 0)  //read
	{
		gUSBC_ComReg->EINDEX = EPindex;
		gUSBC_fifoReg->TX_fifoadd_L = ((fifoAddr>>3)&0xFF);
		gUSBC_fifoReg->TX_fifoadd_H = ((fifoAddr>>11)&0xFF);
	}
	else if(flag == 1)//write
	{
		gUSBC_ComReg->EINDEX = EPindex;
		gUSBC_fifoReg->RX_fifoadd_L = ((fifoAddr>>3)&0xFF);
		gUSBC_fifoReg->RX_fifoadd_H = ((fifoAddr>>11)&0xFF); 
	}
}


/*
Description:Send stall signal to host to ruined current transport
*/
void usb_SendRxStall(UINT8 EPindex)
{
	unsigned char ucReg;
	
	gUSBC_ComReg->EINDEX = EPindex;
	//read RXCSR register
	ucReg = gUSBC_IdxReg->RXCSR_L;
	//set SendStall(D5) =1 to genrerate a stall signal
	ucReg |= DEV_RXCSR_SENDSTALL;
	gUSBC_IdxReg->RXCSR_L = ucReg;
	//Set Endpoint1 is Halt in GetStatus
	gpram_WakeupFeature |= 0x01;
}

/*
Description:Send stall signal to host to ruined current transport
*/
void usb_SendTxStall(UINT8 EPindex)
{
	unsigned char ucReg;
	
	gUSBC_ComReg->EINDEX = EPindex;
	//read RXCSR register
	ucReg = gUSBC_IdxReg->TXCSR_L;
	//set SendStall(D5) =1 to genrerate a stall signal
	ucReg |= DEV_TXCSR_SEND_STALL;
	gUSBC_IdxReg->TXCSR_L = ucReg;
	//Set Endpoint1 is Halt in GetStatus
	gpram_WakeupFeature |= 0x01;
}

	
