							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usbdev_class.c
Author:        Version:        Date: 20121230
Description:  usb设备类实现
Others: 
History:
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define  USBDEV_CLASS_SYMBOL
#include "includes.h"


extern unsigned char g_reboot;


/*
Description: Send packet to host,don't use DMA.(BULK-IN to host)
flag--0,normal,flag--1,CSW packet
*/
void USBDev_Packet_BulkIn(unsigned char *pDataBuf,unsigned short uiLength,unsigned char flag)
{
	unsigned char ucReg;
	UINT32 fifoAddr;
	UINT8 epIndex = INDEX_EP1;

	gUSBC_ComReg->EINDEX = epIndex;
	
	if(flag)
	{
		gpram_bulkPhase =0xFF;
		gpram_subCmd = 0xFF;
	}
	
	//write data into TX FIFO and wait for send
	switch(epIndex)
	{
		case INDEX_EP1:
		{
			fifoAddr = USB_FIFO_ENDPOINT_1;
			break;
		}
		case INDEX_EP2:
		{
			fifoAddr = USB_FIFO_ENDPOINT_2;
			break;
		}
		case INDEX_EP3:
		{
			fifoAddr = USB_FIFO_ENDPOINT_3;
			break;
		}
	}
	
#ifdef USB_AUTOSET_TEST
	usb_SetTxSize(uiLength);
#endif

	write_ep_buf(fifoAddr,pDataBuf,uiLength);

#ifndef USB_AUTOSET_TEST
	usb_SetTxSize(uiLength);
	usb_SetTxFlag(epIndex);		//set TxPktRdy=1
#endif
}

/*
Description:Inquiry
*/
void USBDev_Inquiry(void)
{
	unsigned char *ptrCIB=(unsigned char *)gpram_databuf;
	unsigned char len=36;
	
	//response inquiry command data
	unsigned char SCSI_Inquiry[36]=
	{
	    0x00,          /* Direct Access Device */
	    0x00,          /* RMB = 1: Removable Medium */
	    0x00,          /* Version: No conformance claim to standard */
	    0x00,

	    0x00,          /* Additional Length */
	    0x00,          /* SCCS = 1: Storage Controller Component */
	    0x00,
	    0x00,
	    /* Vendor Identification */
	    'G', 'A', 'S', 'E', 'C', ' ', ' ',' ', 
	    /* Product Identification */
	    'G', 'A', 'U', 'K', 'E', 'Y', ' ', ' ', ' ',
	    ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	    /* Product Revision Level */
	    '1', '.', '0', '0'
	  };
	//Set Suspend enable flag
	//gpram_bSuspendSupport = 1;

	//response inquiry command data packet  to host
	USBDev_Packet_BulkIn((unsigned char *)SCSI_Inquiry,len,0);
}

/*
Description:Command 1a
*/
void USBDev_ModeSense(void)
{
	unsigned char ucRegLow, ucRegHigh;
	
	unsigned char SCSIDATA_Mode_Sense_1a[4]={0x03,0x00,0x00,0x00};
			
	//Send Mode Sense data packet to host
	USBDev_Packet_BulkIn((unsigned char *)SCSIDATA_Mode_Sense_1a,4,0);
}

/*
Description:SCSICMD Request Sense
*/
void USBDev_RequestSense(void)
{
	//scsi command request sense data,18B
	unsigned char SCSIDATA_RequestSense[18]=
	{
	        0x70,
	        0x00,
	        0x02,

	        0x00,		        
	        0x00,
	        0x00,
	        0x00,
	        0x06,
	        0x00,
	        0x00,
	        0x00,
	        0x00,

	        0x3a,
	        0x00,		        
	        0x00,
	        0x00,
	        0x00,
	        0x00,
	} ;

	if((gpram_Eject_Flag == RIGHT_BTN_EJECT)||(gpram_Eject_Flag == LUN_READ_PROTECT)){
		//20080605	
		//If user eject U-Disk in windows by right button
		//sense key:no media
		//20081117
		//If user set Lun in windows by tools as Read Protect
		//sense key:no media
		SCSIDATA_RequestSense[2] = 0x02;
		//ASC(Additional Sense Code)
		SCSIDATA_RequestSense[12] = 0x3a;		
	}else if(gpram_Abnormal_Flag == UNSUPPORT_SCSI_CMD){
		//sense key:INVALID FILED IN CDB
		SCSIDATA_RequestSense[2] = 0x05;
		//ASC
		SCSIDATA_RequestSense[12] = 0x24;		
		//reset Abnormal_Flag
		gpram_Abnormal_Flag = NORMAL_SCSI_STATUS;
	}else if(gpram_CDFlag == 0){
		if(gpram_ISOFlag == PHASE_REPORT_NO_MEDIA){
			//sense key:no media
			SCSIDATA_RequestSense[2] = 0x02;
			//ASC
			SCSIDATA_RequestSense[12] = 0x3a;		
			gpram_ISOFlag = PHASE_REPORT_MEDIA_CHANGE;
		}else if(gpram_ISOFlag == PHASE_REPORT_MEDIA_CHANGE){
			//sense key:media changed
			SCSIDATA_RequestSense[2] = 0x06;
			//ASC
			SCSIDATA_RequestSense[12] = 0x28;		
			gpram_ISOFlag = PHASE_NORMAL;
		}
	}	
	//response request sense command data packet to host
	USBDev_Packet_BulkIn(SCSIDATA_RequestSense,18,0);
}

void USBDev_ReadCapacity()
{	
	//format capacity size is 12B,capacity size is 8B
	unsigned char size=8;//format capacity size is 12B
	unsigned char *ptrArr;
	
	//response read capacities command data 
	unsigned char SCSIDATA_ReadCapacities[]=
	{
		 0x00,//reserved
		 0x00,//reserved
		 0x00,//reserved
		 0x08,//Capacity List Length
		 
		 0x00, //number of block   MSB
		 0x01, //number of block    |
		 0xec, //number of block    |
		 0xff, //number of block   LSB // 125MBYTE
		 
		 0x00, //no cartridge in drive//02
		 0x00,  // length of block
		 0x02, 
		 0x00
	};
	
	//capacity default value
	if(gpram_bulkPhase == SCSICMD_READ_CAPACITY)
	{
		ptrArr=SCSIDATA_ReadCapacities+4;
		ptrArr[2]=0xED;
		ptrArr[3]=0x0;
		size=8;
	}
	else
	{
		ptrArr=SCSIDATA_ReadCapacities;
		size=12;
	}

	//response read capacity command data packet to host
	USBDev_Packet_BulkIn(ptrArr,size,0);
}

/*
Description:Read data from FLASH.
*/
void USBDev_ReadData()
{
	unsigned long m_readDataAddr;
	unsigned long sourceAddr;
	unsigned char sectorOffset =0;
	unsigned char tmp;
	
	
	if(gpram_wrLength)
	{		
		gpram_msgflags1 &=0xF7;

		m_readDataAddr = 0x00800000;
		if(gpram_usbVer == 1)
		{
			//usb2.0
			//SetFifo(0x00800000, 0);
			gpram_wrLength -= USB_MAX_PACKET_SIZE;	
	    		usb_SetTxFlag(INDEX_EP1);
		}
		else
		{
			//usb1.1,had sent 8 64B packets to host
			
			//SetFifo(0x00800000, 0);
	    		usb_SetTxFlag(INDEX_EP1);
			gpram_wrLength -= USB_MAX_PACKET_SIZE_V11;
			//Usb1.1,final packet of 8 packet,or Usb2.0
			if(gpram_TxRxCount == 7)
			{	
				gpram_TxRxCount=0;
			}
			else
			{
				gpram_TxRxCount++;
				return;
			}
		}
			
	}
	else
	{		
		//it is last packet,send CSW response
		gpram_msgflags1 &=0xF7;
		USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);
	}
}

/*
Description:SCSICMD Read CD=>Only Support MAC OS,return Dummy data
*/
void USBDev_ReadCDData(void)
{
	unsigned short USB_Packet_Size;
	

	if(gpram_wrLength)
	{
		if(gpram_usbVer == 1)
		{
			//USB2.0
			USB_Packet_Size = USB_MAX_PACKET_SIZE;
		}
		else
		{
			//USB1.1
			USB_Packet_Size = USB_MAX_PACKET_SIZE_V11;
		}

		if(gpram_wrLength>=USB_Packet_Size){
			usb_SetFifoAddr(INDEX_EP1, gpram_databuf, 0);
	    	usb_SetTxFlag(INDEX_EP1);
			gpram_wrLength -= USB_Packet_Size;
		}else{
			usb_SetTxSize(gpram_wrLength);
			usb_SetFifoAddr(INDEX_EP1, gpram_databuf, 0);
	    	usb_SetTxFlag(INDEX_EP1);
			gpram_wrLength = 0;			
		}		
	}
	else
	{
		if(gpram_Abnormal_Flag != DUMMY_RW_STATUS){		//return Dummy CSW to host when all data sent
			gpram_Abnormal_Flag = UNSUPPORT_SCSI_CMD;
			gpram_cswPacket[MUSB_CSW_SCSI_CSW_STATUS] = REPORT_ABNORMAL_STATUS;
		}
		//it is last packet,send CSW response
		gpram_msgflags1 &=0xF7;
		USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);	
	}
}

void USBDev_ISO(void)
{
	unsigned char len;
	unsigned char pos;
	
	switch(gpram_bulkPhase)
	{
		case SCSICMD_READ_TOC:
			pos = 0;
			if(gpram_cbwPacket[MUSB_CBW_SCSI_TOC_DATA_LENGTH]==0){
				len=12;
			}
			else{
				len=20;
			}
			break;
		case SCSICMD_MODE_SENSE_10:
			pos = 64;
			len = gpram_cbwPacket[MUSB_CBW_SCSI_MODESENSE_DATA_LENGTH];
			break;
	}

 	
	USBDev_Packet_BulkIn((unsigned char *)(gpram_databuf+pos), len, 0);
}

/*
Description:SCSICMD Read
*/
void USBDev_Read(void)
{
	usb_SetTxSize(0);
	
#ifdef READ_CD
	if(gpram_Abnormal_Flag == DUMMY_RW_STATUS)
	{
		gpram_bulkPhase = SCSICMD_READ_CD;
		USBDev_ReadCDData();	
	}else
#endif	
	{
		//gpram_TxRxCount=0;
		gpram_msgflags1 |= 0x8;

		USBDev_ReadData();
	}
}

/*
Description:Receive data from host.(BULK-OUT to host)
*/
void USBDev_Data_BulkOut(void)
{
	unsigned char ucReg=1;
	unsigned char ucPara_1;
	unsigned short uiRxCount;
	unsigned long i,tmp_ff = 0;
	unsigned long *ulPara = &gpram_cbwPacket_l[4];
	unsigned long cbw_signature = 0;	//from CBW
	unsigned long cbw_signature_ok = 0x55534243;
	unsigned long fifoAddr;
	unsigned int len;

    
	uiRxCount = gUSBC_IdxReg->RXCount_H;
	uiRxCount <<= 8;
	uiRxCount += gUSBC_IdxReg->RXCount_L; 
	   
	if(uiRxCount==0)
		return;
 
	if(uiRxCount==31)
	{
		read_ep_buf(gpram_cbwPacket,USB_FIFO_ENDPOINT_1,31);
		
		//Read the cbw signature
		cbw_signature += (gpram_cbwPacket[0]<<24);
		cbw_signature += (gpram_cbwPacket[1]<<16);
		cbw_signature += (gpram_cbwPacket[2]<<8);
		cbw_signature += gpram_cbwPacket[3];
		// constructe a csw packet
		//dCSWTag,got from CBW
		gpram_cswPacket[4]=gpram_cbwPacket[4];
		gpram_cswPacket[5]=gpram_cbwPacket[5];
		gpram_cswPacket[6]=gpram_cbwPacket[6];
		gpram_cswPacket[7]=gpram_cbwPacket[7];
		//dCSWDataResidue
		gpram_cswPacket[8]=gpram_cbwPacket[8];
		gpram_cswPacket[9]=gpram_cbwPacket[9];
		gpram_cswPacket[10]=gpram_cbwPacket[10];
		gpram_cswPacket[11]=gpram_cbwPacket[11];
		//bCSWStatus
		gpram_cswPacket[12]=0;
		//Command and Sub Command
		gpram_bulkPhase = gpram_cbwPacket[15];
		gpram_subCmd = gpram_cbwPacket[28];
		ucPara_1 = gpram_cbwPacket[29];
		
		//接收数据包的长度	，只针对write(10)，write(12)的长度从偏移21开始的4字节
		len = (gpram_cbwPacket[20] <<24)|(gpram_cbwPacket[21] <<16)|(gpram_cbwPacket[22] <<8)|(gpram_cbwPacket[23]);
		gpram_wrLength = (len & 0xffff)<<9;		//块数*512字节
		
		//Check the cbw signature
		if(cbw_signature != cbw_signature_ok)
		{
			usb_SendRxStall(INDEX_EP1);
			usb_SendTxStall(INDEX_EP1);
			return;	
		}
	
		//process command
		switch(gpram_bulkPhase)
		{
			case SCSICMD_TEST_UNIT_READY:
			case  SCSICMD_VERIFY_10:  //operation code 0x2F
				 USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);//Send csw packet to host 				 
			break;
					//operation code 0x1b
			case	SCSICMD_START_STOP_UNIT:			
				USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);//Send csw packet to host
				if((gpram_cbwPacket[19] & 0xF3)==0x02)	//20080605	
				{
					gpram_Eject_Flag = RIGHT_BTN_EJECT;
					//write_port_byte(0x00C80000,0x00);	//close LED
					asm{doze}
				}
			break;
			case SCSICMD_PREVENT_ALLOW_MEDIUM_REMOVAL:			
				//Send csw packet to host
				USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);								
			break;
			case SCSICMD_INQUIRY:
				USBDev_Inquiry();
			break;
			case SCSICMD_READ_FORMAT_CAPACITIES:
			case SCSICMD_READ_CAPACITY:
				USBDev_ReadCapacity();
			break;
			case SCSICMD_MODE_SENSE_06:
				USBDev_ModeSense();
			break;
			case SCSICMD_REQUEST_SENSE:	
				USBDev_RequestSense();
			break;
			case	0xD4:
			case	SCSICMD_WRITE_12:
				gpram_bulkPhase = SCSICMD_WRITE_10;
				//继续往下执行;
			case	SCSICMD_WRITE_10:
		
				//fifoAddr = gpram_databuf;
				//SetFifo(fifoAddr, 1);
			break;
			
			case	0xD3:
			case	SCSICMD_READ_12:		
					gpram_bulkPhase = SCSICMD_READ_10;
					//继续往下执行;
			case SCSICMD_READ_10:	
				USBDev_Read();	
			break;
			
			case SCSICMD_MODE_SENSE_10:
			case SCSICMD_READ_TOC:
				USBDev_ISO();
			break;



///****************添加用户程序分支*****************************************//				
			case 0xFE:  								 //用户定义程序 写命令
				gpram_bulkPhase = 0xFE;
				//packetCount = 0;
				//解析csw包  
				/*
				if(gpram_usbVer == 0)
				{
					packetSize = USB_MAX_PACKET_SIZE_V11;
				}
				else
				{
					packetSize = USB_MAX_PACKET_SIZE;
				}
				*/
				//数据总长度
				//parse CBW & get the sum of data，直接是字节长度
				gpram_wrLength  = (*(gpram_cbwPacket+8));
				gpram_wrLength |= (*(gpram_cbwPacket+9))<<8;
				gpram_wrLength |= (*(gpram_cbwPacket+10))<<16;
				gpram_wrLength |= (*(gpram_cbwPacket+11))<<24;

				g_APDU_LEN = gpram_wrLength;
				//设置接收数据的缓存
				sramAddr = (UINT32)g_APDU_BUF;
				usb_SetFifoAddr(INDEX_EP1, sramAddr, 1);
				//usb_ClearRx(INDEX_EP1);
			break;
			
			case 0xFD:									 //用户定义程序 读命令
#if 0
				gpram_bulkPhase = 0xFD;
				packetCount = 0;
				gusb_data_finish_flag =0;	
				//解析csw包  
				if(gpram_usbVer == 0)
				{
					packetSize = USB_MAX_PACKET_SIZE_V11;
				}
				else
				{
					packetSize = USB_MAX_PACKET_SIZE;
				}

				sramAddr = (UINT32)apdu_response;
				while(len_response>USB_MAX_PACKET_SIZE)
				{	
					usb_SetTxSize(USB_MAX_PACKET_SIZE);
					usb_SetFifoAddr(INDEX_EP1, sramAddr, 0);
					usb_SetTxFlag(INDEX_EP1);
					len_response-=USB_MAX_PACKET_SIZE;
					sramAddr+=	USB_MAX_PACKET_SIZE;
				}

				gpram_wrLength  = (*(gpram_cbwPacket+8));
				gpram_wrLength |= (*(gpram_cbwPacket+9))<<8;
				gpram_wrLength |= (*(gpram_cbwPacket+10))<<16;
				gpram_wrLength |= (*(gpram_cbwPacket+11))<<24;

				usb_SetTxSize(len_response);
				usb_SetFifoAddr(INDEX_EP1, sramAddr, 0);
				usb_SetTxFlag(INDEX_EP1);
				len_response=0;
				gusb_data_finish_flag=1;	
#else
			gpram_bulkPhase = 0xFD;
			packetCount = 0;
			//解析csw包  
			if(gpram_usbVer == 0)
			{
				packetSize = USB_MAX_PACKET_SIZE_V11;
			}
			else
			{
				packetSize = USB_MAX_PACKET_SIZE;
			}

			if(len_response>packetSize)	
				usb_SetTxSize(packetSize);
			else
				usb_SetTxSize(len_response);

			gpram_wrLength	= (*(gpram_cbwPacket+8));
			gpram_wrLength |= (*(gpram_cbwPacket+9))<<8;
			gpram_wrLength |= (*(gpram_cbwPacket+10))<<16;
			gpram_wrLength |= (*(gpram_cbwPacket+11))<<24;
			
			sramAddr = (UINT32)apdu_response;
			usb_SetFifoAddr(INDEX_EP1, sramAddr, 0);
			usb_SetTxFlag(INDEX_EP1);

			if(len_response>packetSize)	
				len_response-=packetSize;
			else
				len_response=0;	

#endif
			break;
///////////////////////////////////////////////////////////////////////////////////	


				
///****************添加用户程序分支*****************************************//				
			case 0xC0:  								 //用户定义程序 写命令
				gpram_bulkPhase = 0xC0;
				//packetCount = 0;
				//解析csw包  
				/*
				if(gpram_usbVer == 0)
				{
					packetSize = USB_MAX_PACKET_SIZE_V11;
				}
				else
				{
					packetSize = USB_MAX_PACKET_SIZE;
				}
				*/
				//数据总长度
				//parse CBW & get the sum of data，直接是字节长度
				gpram_wrLength  = (*(gpram_cbwPacket+8));
				gpram_wrLength |= (*(gpram_cbwPacket+9))<<8;
				gpram_wrLength |= (*(gpram_cbwPacket+10))<<16;
				gpram_wrLength |= (*(gpram_cbwPacket+11))<<24;

				g_APDU_LEN = gpram_wrLength;
				//设置接收数据的缓存
				sramAddr = (UINT32)g_APDU_BUF;
				usb_SetFifoAddr(INDEX_EP1, sramAddr, 1);
				//usb_ClearRx(INDEX_EP1);
			break;
			
			case 0xC1:									 //用户定义程序 读命令
				gpram_bulkPhase = 0xC1;
				packetCount = 0;
				//解析csw包  
				if(gpram_usbVer == 0)
				{
					packetSize = USB_MAX_PACKET_SIZE_V11;
				}
				else
				{
					packetSize = USB_MAX_PACKET_SIZE;
				}
				usb_SetTxSize(packetSize);
				
				gpram_wrLength  = (*(gpram_cbwPacket+8));
				gpram_wrLength |= (*(gpram_cbwPacket+9))<<8;
				gpram_wrLength |= (*(gpram_cbwPacket+10))<<16;
				gpram_wrLength |= (*(gpram_cbwPacket+11))<<24;
				
				sramAddr = (UINT32)apdu_response;
				usb_SetFifoAddr(INDEX_EP1, sramAddr, 0);
				usb_SetTxFlag(INDEX_EP1);
				
			break;
///////////////////////////////////////////////////////////////////////////////////				
		
			default:
				//unsupport SCSI cmd,return INVAILD CDB FIELD in Request Sense
				gpram_Abnormal_Flag = UNSUPPORT_SCSI_CMD;
				//avoid no data send cause TIMEOUT,not fixed
				USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,8,1);//Send csw packet to host				
			break;
		}
		
		//clear RxPktRdy bit
		usb_ClearRxFlag(INDEX_EP1);
	}
	else
	{	
		gpram_counter=0;
		//enter in busy mode	
		switch(gpram_bulkPhase)
		{
			case SCSICMD_WRITE_10:
			{
				//the USB disk is not formatted by mass production tool
				if(gpram_usbVer == 1)
				{
					gpram_wrLength -= USB_MAX_PACKET_SIZE;
				}
				else
				{
					gpram_wrLength -= USB_MAX_PACKET_SIZE_V11;
				}
				//clear Rx
				usb_ClearRxFlag(INDEX_EP1);
				
				if(gpram_wrLength <= 0)
				{
					//write csw response
					USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);		
				}
				break;
			}


			case 0xFE:    //用户定义程序 写
				//packetCount++;
				//gpram_wrLength -= packetSize;
				gpram_wrLength -= uiRxCount;
				
				if(gpram_wrLength == 0)
				{
					usb_SetFifoAddr(INDEX_EP1, USB_FIFO_OFFSET, 1);		//USB FIFO改成USB协议的BUF，否则下一包数据会放到APDUBUF里
					usb_ClearRxFlag(INDEX_EP1);				
					USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);	//write csw response	
					Delay(2000);
					//数据接收完毕，置接收完毕标志
					//命令处理
					APDUprocess1(g_APDU_BUF, g_APDU_LEN, apdu_response, &len_response);
					//HAL_UART_SendData("len_response",sizeof("len_response"));
					//HAL_UART_SEND_ASC((unsigned char*)&len_response,2);
				}
				else
				{	
					//sramAddr = (UINT32)g_APDU_BUF+packetCount*512;
					sramAddr += uiRxCount;
					usb_SetFifoAddr(INDEX_EP1, sramAddr, 1);
					usb_ClearRxFlag(INDEX_EP1);	
				}
			break;



		
			case 0xC0:    //用户定义程序 写
				//packetCount++;
				//gpram_wrLength -= packetSize;
				gpram_wrLength -= uiRxCount;
				
				if(gpram_wrLength == 0)
				{
					usb_SetFifoAddr(INDEX_EP1, USB_FIFO_OFFSET, 1);		//USB FIFO改成USB协议的BUF，否则下一包数据会放到APDUBUF里
					usb_ClearRxFlag(INDEX_EP1);
					USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);	//write csw response	

					//数据接收完毕，置接收完毕标志
					//命令处理
					APDUprocess(g_APDU_BUF, g_APDU_LEN, apdu_response, &len_response);
				}
				else
				{	
					//sramAddr = (UINT32)g_APDU_BUF+packetCount*512;
					sramAddr += uiRxCount;
					usb_SetFifoAddr(INDEX_EP1, sramAddr, 1);
					usb_ClearRxFlag(INDEX_EP1);	
				}
			break;
		}
 	}
}

/*
Description:Device sends data to host(BULK-IN to host)
*/
void USBDev_Data_BulkIn(void)
{
	unsigned char ucReg;
	unsigned long csw_residue=0;
	
	switch(gpram_bulkPhase)
	{
#ifdef READ_CD	
		case SCSICMD_READ_CD:
		{
			USBDev_ReadCDData();
			return;		
		}
#endif		
		case SCSICMD_READ_10:
			USBDev_ReadData();
			break;	

	
		case 0xFD: //vendor read
#if 0		
			if((0==len_response))
			{
				gpram_msgflags1 &=0xF7;
				csw_residue += gpram_cswPacket[8];
				csw_residue += (gpram_cswPacket[9]<<8);
				csw_residue += (gpram_cswPacket[10]<<16);
				csw_residue += (gpram_cswPacket[11]<<24);
				csw_residue -= (len_response);
	
				gpram_cswPacket[8] =	csw_residue&0xff;
				gpram_cswPacket[9] =	(csw_residue>>8)&0xff;
				gpram_cswPacket[10] =	(csw_residue>>16)&0xff;
				gpram_cswPacket[11] =	(csw_residue>>24)&0xff;
				
				USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);	
			}
				
#else	

			if(g_reboot==1)
					g_reboot=2;
				if(g_reboot==3)
					g_reboot=4;


			packetCount++;
			if(len_response ==0)
			{
				gpram_msgflags1 &=0xF7;
				csw_residue += gpram_cswPacket[8];
				csw_residue += (gpram_cswPacket[9]<<8);
				csw_residue += (gpram_cswPacket[10]<<16);
				csw_residue += (gpram_cswPacket[11]<<24);
				csw_residue -= (len_response);
	
				gpram_cswPacket[8] =	csw_residue&0xff;
				gpram_cswPacket[9] =	(csw_residue>>8)&0xff;
				gpram_cswPacket[10] =	(csw_residue>>16)&0xff;
				gpram_cswPacket[11] =	(csw_residue>>24)&0xff;
				USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);
			}
			else
			{
				if(len_response>packetSize)
				{
					sramAddr = (UINT32)apdu_response+packetCount*512;
					usb_SetFifoAddr(INDEX_EP1, sramAddr, 0);
					usb_SetTxFlag(INDEX_EP1);
					len_response-= packetSize;
				}
				else 
				{
					usb_SetTxSize(len_response);
					sramAddr = (UINT32)apdu_response+packetCount*512;
					usb_SetFifoAddr(INDEX_EP1, sramAddr, 0);
					usb_SetTxFlag(INDEX_EP1);
					len_response =0;
				}
			}

		
#endif
		break;

					
		case 0xC1: //vendor read
			packetCount++;
			gpram_wrLength -= packetSize;
			
			if(gpram_wrLength == 0)
			{
				gpram_msgflags1 &=0xF7;
				USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);
			
			}
			else 
			{
				sramAddr = (UINT32)apdu_response+packetCount*512;
				usb_SetFifoAddr(INDEX_EP1, sramAddr, 0);
				usb_SetTxFlag(INDEX_EP1);
			}
		break;
			
		default:
			/*
			当收到不支持的SCSI命令时，先在bulkout中发送8字节数据，
			然后在此处发送CSW
			*/
			if(gpram_Abnormal_Flag == UNSUPPORT_SCSI_CMD)	
			{
				//Unsupport SCSI cmd,csw is abnormal
				gpram_cswPacket[MUSB_CSW_SCSI_CSW_STATUS] = REPORT_ABNORMAL_STATUS;	
			}
			USBDev_Packet_BulkIn((unsigned char *)gpram_cswPacket,13,1);		
						
		break;			
	}
}
