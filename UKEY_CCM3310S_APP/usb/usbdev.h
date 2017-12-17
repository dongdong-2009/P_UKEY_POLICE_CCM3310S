							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usb.h
Author:        Version:        Date: 20121230
Description:  usb模块寄存器定义
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#ifdef  USB_SYMBOL
#define	USB_EXT
#else	
#define	USB_EXT extern
#endif

typedef struct _sUSBCCommonReg{
	volatile UINT8 FADDRR;
	volatile UINT8 UCSR;

	volatile UINT8 INTRTX_L; 
	volatile UINT8 INTRTX_H;

	volatile UINT8 INTRRX_L;                                
	volatile UINT8 INTRRX_H;

	volatile UINT8 INTRTXE_L;                      
	volatile UINT8 INTRTXE_H;                            

	volatile UINT8 INTRRXE_L;
	volatile UINT8 INTRRXE_H;

	volatile UINT8 INTRUSB;                                
	volatile UINT8 INTRUSBE;

	volatile UINT8 FNUMR_L;                               
	volatile UINT8 FNUMR_H;

	volatile UINT8 EINDEX;                               
	volatile UINT8 TESTMODE;
}sUSBCCommonReg;


typedef struct _sUSBCINdexedReg{
	volatile UINT8 TXPSZR_L;              //10  
	volatile UINT8 TXPSZR_H;              //11  
	union                                         //12
	{
		volatile UINT8 E0CSR_L;
		volatile UINT8 TXCSR_L;
	};
	union                                        //13
	{
		volatile UINT8 E0CSR_H;
		volatile UINT8 TXCSR_H;
	};   

	volatile UINT8 RXPSZR_L;            //14
	volatile UINT8 RXPSZR_H;            //15            
                                    
	volatile UINT8 RXCSR_L;              //16
	volatile UINT8 RXCSR_H;              //17        
                                
	union
	{
		volatile UINT8 E0COUNTR_L;       //18
		volatile UINT8 RXCount_L;
	};
	union
	{
		volatile UINT8 E0COUNTR_H;        // 19     手册上没提
		volatile UINT8 RXCount_H;
	}; 
}sUSBCIndexedReg;


typedef  struct _sUSBCFIFO_Reg{
	volatile UINT8 TX_fifosz;
	volatile UINT8 RX_fifosz;
	volatile UINT8 TX_fifoadd_L;
	volatile UINT8 TX_fifoadd_H;
	volatile UINT8 RX_fifoadd_L;
	volatile UINT8 RX_fifoadd_H;
}sUSBCFIFO_Reg;


typedef  struct  _device_request
{
	UINT8  bmRequestType;
	UINT8  bRequest;
	UINT16 wValue;
	UINT16 wIndex;
	UINT16 wCount;
}DEVICE_REQUEST;


#define USB_FIFO_ENDPOINT_0  USBC_BASE + 0x20                                   
#define USB_FIFO_ENDPOINT_1  USBC_BASE + 0x24                                       
#define USB_FIFO_ENDPOINT_2  USBC_BASE + 0x28                                
#define USB_FIFO_ENDPOINT_3  USBC_BASE + 0x2C                                      
#define USB_FIFO_ENDPOINT_4  USBC_BASE + 0x30                                   
#define USB_FIFO_ENDPOINT_5  USBC_BASE + 0x34                                    
#define USB_FIFO_ENDPOINT_6  USBC_BASE + 0x38                                         
#define USB_FIFO_ENDPOINT_7  USBC_BASE + 0x3C                                         

                                                        
#define 	CONTROL_EP                                     0
#define 	DATA_IN_EP                                     1
#define 	DATA_OUT_EP                                    1
#define		INDEX_EP1                                      1	
#define 	INDEX_EP2                                      2	
#define 	INDEX_EP3                                      3	
#define 	INDEX_EP4                                      4	
#define 	INDEX_EP5                                      5	
#define 	INDEX_EP6                                      6	
#define 	INDEX_EP7                                      7	
                                                        
                                                        
                                                        
#define 	USB_POWER_ENAB_SUSP                            0x01
#define 	USB_POWER_SUSP_MODE                            0x02 	
#define  	USB_POWER_RESUME                               0x04	
#define 	USB_POWER_RESET                                0x08 	
#define 	USB_POWER_HS_MODE                              0x10 	
#define 	USB_POWER_HS_ENAB                              0x20		
#define 	USB_POWER_SOFT_CONN                            0x40 	
#define  	USB_POWER_ISO_UPDATE                           0x80	
                                                        
                                                        
                                                        
#define 	USB_INTERRUPT_SUSPEND                          0x01
#define 	USB_INTERRUPT_RESUME                           0x02
#define 	USB_INTERRUPT_RESET                            0x04
#define 	USB_INTERRUPT_SOF                              0x08
#define 	USB_INTERRUPT_CONNECT                          0x10
#define 	USB_INTERRUPT_DISCON                           0x20
#define 	USB_INTERRUPT_SESSREQ                          0x40
#define 	USB_INTERRUPT_VBUSERR                          0x80
                                                        
#define 	USB_MAX_PACKET_SIZE_EP0                     64                                                        
#define 	USB_MAX_PACKET_SIZE                           512
#define 	USB_MAX_PACKET_SIZE_LOW                       0x00
#define 	USB_MAX_PACKET_SIZE_HIGH                      0x02
#define 	USB_MAX_PACKET_SIZE_V11                       64
#define 	USB_MAX_PACKET_SIZE_LOW_V11                   0x40
#define 	USB_MAX_PACKET_SIZE_HIGH_V11                  0x00
                                                        
                                                        
                                                        
#define 	USB_INTERRUPT_EP0                             0x01  
#define 	USB_TX_INTERRUPT_EP1                          0x02  
#define 	USB_TX_INTERRUPT_EP2                          0x04	
#define		USB_TX_INTERRUPT_EP3                          0x08  
#define 	USB_TX_INTERRUPT_EP4                          0x10  
#define 	USB_TX_INTERRUPT_EP5                          0x20	
#define 	USB_TX_INTERRUPT_EP6                          0x40  
#define 	USB_TX_INTERRUPT_EP7                          0x80	
                                                        
#define 	USB_RX_INTERRUPT_EP0                          0x01  
#define 	USB_RX_INTERRUPT_EP1                          0x02  
#define 	USB_RX_INTERRUPT_EP2                          0x04	
#define		USB_RX_INTERRUPT_EP3                          0x08  
#define 	USB_RX_INTERRUPT_EP4                          0x10  
#define 	USB_RX_INTERRUPT_EP5                          0x20	
#define 	USB_RX_INTERRUPT_EP6                          0x40  
#define 	USB_RX_INTERRUPT_EP7                          0x80	
                                                        
#define 	DEV_CSR0_RXPKTRDY                             0x01  
#define 	DEV_CSR0_TXPKTRDY                             0x02  
#define 	DEV_CSR0_SENTSTALL                            0x04	
#define		DEV_CSR0_DATAEND                              0x08  
#define 	DEV_CSR0_SETUPEND                             0x10  
#define 	DEV_CSR0_SENDSTALL                            0x20	
#define 	DEV_CSR0_SERVICE_RXPKTRDY                     0x40  
#define 	DEV_CSR0_SERVICE_SETUPEND                     0x80	
                                                        
                                                        
#define 	DEV_TXCSR_TXPKTRDY                            0x01
#define 	DEV_TXCSR_FIFO_NOT_EMPTY                      0x02
#define 	DEV_TXCSR_UNDER_RUN                           0x04
#define		DEV_TXCSR_FLUSH_FIFO                          0x08
#define 	DEV_TXCSR_SEND_STALL                          0x10
#define 	DEV_TXCSR_SENT_STALL                          0x20
#define 	DEV_TXCSR_CLR_DATA_TOG                        0x40
#define 	DEV_TXCSR_INCOMP_TX                           0x80
                                                        
                                                        
#define 	DEV_TXCSR_FRC_DATA_TOG                        0x08
#define 	DEV_TXCSR_DMA_ENAB                            0x10
#define 	DEV_TXCSR_MODE                                0x20
#define		DEV_TXCSR_ISO                                 0x40
#define		DEV_TXCSR_AUTO_SET                            0x80
                                                        
                                                        
#define 	DEV_RXCSR_RXPKTRDY                            0x01 
#define 	DEV_RXCSR_FIFOFULL                            0x02 
#define 	DEV_RXCSR_OVERRUN                             0x04 
#define		DEV_RXCSR_DATA_ERROR                          0x08 
#define 	DEV_RXCSR_FLUSH_FIFO                          0x10 
#define 	DEV_RXCSR_SENDSTALL                           0x20 
#define 	DEV_RXCSR_SENTSTALL                           0x40 
#define 	DEV_RXCSR_CLR_DATA_TOG                        0x80 
                                                        
                                                        
#define 	DEV_RXCSR_INCOMP_RX                           0x01 
#define 	DEV_RXCSR_DMAMODE                             0x08 
#define 	DEV_RXCSR_DISNYET                             0x10 
#define		DEV_RXCSR_DMA_ENAB                            0x20 
#define 	DEV_RXCSR_ISO                                 0x40  
#define 	DEV_RXCSR_AUTOCLEAR                           0x80 

             		
                                                          		
#define 	DEVICE_TYPE                                   		0x1
#define 	CONFIG_TYPE                                   		0x2
#define 	STRING_TYPE                                   		0x3
#define 	INTERFACE_TYPE                                		0x4
#define 	ENDPOINT_TYPE                                 		0x5
#define		DEVICE_QUALIFIER                              		0x6
                                                          		
#define 	OTHER_SPEED                                   		0x7
#define 	INTERFACE_POWER                               		0x8
#define 	REPORT_TYPE                                   		0x22

#define BIT0   0x0001
#define BIT1   0x0002
#define BIT2   0x0004
#define BIT3   0x0008
#define BIT4   0x0010
#define BIT5   0x0020
#define BIT6   0x0040
#define BIT7   0x0080
#define BIT8   0x0100
#define BIT9   0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000

USB_EXT   sUSBCFIFO_Reg*    gUSBC_fifoReg;
USB_EXT   sUSBCCommonReg*   gUSBC_ComReg;
USB_EXT   sUSBCIndexedReg*  gUSBC_IdxReg;

USB_EXT   UINT8 gpram_bSuspendSupport;
USB_EXT   UINT8 gpram_usbVer;

USB_EXT   volatile UINT8 gpram_msgflags;		//BIT0~BIT3代表USB的reset、suspend等中断
USB_EXT   volatile UINT8 gpram_msgflags1;		//BIT0表示EP0中断，BIT1~BIT7表示发送中断

USB_EXT   UINT8  g_uchUSBRcvStatus;   //为1代表来了接收中断
USB_EXT   UINT8  g_uchUSBSendStatus;   //为1代表来了发送中断

USB_EXT  UINT32 g_uchUSBFifoAddr;	//发送FIFO地址
                                          
USB_EXT   void USBDev_Suspend(void);
USB_EXT   void USBDev_BusReset(UINT8 flag);
USB_EXT   void USBDev_Init(void);
USB_EXT   void USB_Init(void);


                                   
