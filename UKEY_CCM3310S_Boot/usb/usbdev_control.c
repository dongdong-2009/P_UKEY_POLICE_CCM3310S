#define  USBDEV_CONTROL_SYMBOL	
#include "includes.h"


const UINT8 USB_Device_Descriptor[18] =   //USB设备描述符
{
    0x12,                       //bLength: 0x12 字节
    0x01,                       //bDescriptorType: 设备
    0x10, 0x01,                 //bcdUSB: version2.0
    0x00,                       //bDeviceClass: 自定义接口
    0x00,                       //bDeviceSubClass: 子类0
    0x00,                       //bDeviceProtocol: 协议0
    0x40,                       //bMaxPacketSize0: 64字节（端点0的最大包长）
    0xc2, 0x22,                 //idVendor:  厂商 ID	        
    0x32, 0x23,                 //idProduct: 产品 ID 
    0x11, 0x20,                 //bcdDevice: 设备版本
    0x01,                       //iManufacturer: 字符串索引
    0x02,                       //iProduct: 字符串索引
    0x00,                       //iSerialNumber: 字符串索引	        
    0x01,                       //bNumConfigurations:支持的配置数1	        
};



const UINT8 USB_Device_Qualifier_Descriptor[10] =  //设备更改描述符，只有同时支持全速和高速的设备才需要。
{                             
    0x0A,                     
    0x06,                     
    0x00, 0x02,             
    0x00,                     
    0x00,                    
    0x00,                     
    0x40,                     
    0x01,                     
    0x00,
    };                   

const UINT8 USB_Configuration_Descriptor[] =  //USB配置描述符
{                             //USB配置描述符
    0x09,                         //bLength  配置描述符长度  
    0x02,                         //bDescriptor   配置描述符
    0x56, 0x00,                   //wTotalLength  该集合的总长度  
    0x01,                         //bNumInterface  该配置的接口数为1
    0x01,                         //bConfiguration Value   该配置的值为1
    0x00,                         //iConfiguration    该配置没有字符串索引
    0x80,                         //bmAttributes
    0x32,                         //bMaxPower   设备最大电流100mA
                              //USB接口描述符
    0x09,                         //bLength:    接口描述符长度
    0x04,                         //bDescriptorType   接口描述符
    0x00,                         //bInterfaceNumber   接口编号
    0x00,                         //bAlternateSetting   接口备用编号
    0x02,                         //bNumEndpoints     非0端点数
    0x0b,                         //bInterfaceClass:   
    0x00,                         //bInterfaceSubClass   自定义设备
    0x00,                         //bInterfaceProtocol
    0x00,                         //iInterface       字符串索引	        

							//CCID描述符
    0x36,         			/* bLength*/
    0x21, 					/* bDescriptorType: CCID*/
	0x10,0x01,  			/* bcdCCID*/
	0x00,		   			/* Max Slot Index (only one)*/
	0x01,          			/* Voltage Support : 0x01=5.0V*/
   	0x03,0x00,0x00,0x00, 	/* Protocols : 0x0001=T0,0x0003=T1*/	
    0xfc,0x0d,0x00,0x00,  	/* Default Clock*/
    0xfc,0x0d,0x00,0x00,  	/* Max Clock*/
    0x00,           		/* Num Clock Supported*/
    0x80,0x25,0x00,0x00,   	/* Data Rate : 0x2580=9600bps*/
    0x80,0x25,0x00,0x00, 	/* Max Data Rate : 0x01c200=115200bps*/	//0x0001c200
   	0x00,                   /* bNumDataRatesSupported：Number of Data Rates*/ 
    0x13,0x04,0x00,0x00,    /* Max IFSD*/
    0x00,0x00,0x00,0x00,    /* Synch Protocols*/
    0x00,0x00,0x00,0x00,    /* dwMechanical :No special characteristics*/
    0x42,0x00,0x04,0x00,    /* Features :40=Automatic parameters negotiation made by the CCID,02=Automatic parameter configuration based on ATR data*///0x00040042
    0x13,0x04,0x00,0x00,    /* Max CCID Message Length : 0x400=1k,0x010f=260+10*/ //0x13,0x04,0x00,0x00,
    0xFF,                   /* Class Get Response :echoes the APDU class*/
    0xFF,                   /* Class Envelope ：echoes the APDU class*/
    0x00,0x00,      	    /* LCD Layout */
    0x00,                   /* Pin Support */
    0x01,                   /* Max Busy Slots */
    
                                   //USB接口描述符端点描述符 
    0x07,                          //bLength:             
    0x05,                          //bDescriptorType:    端点描述符   
    0x01,       //EndpointAddress     输入端点为端点1
    0x02,                          //bmAttributes        中断传输
    0x40,                          //wMaxPacketSize      512字节
    0x00,                          
    0x00,                          //bInterval           1mS查询一次
      
    0x07,                          //bLength: 
    0x05,                          //bDescriptorType: 
    0x81,            //EndpointAddress
    0x02,                          //bmAttributes
    0x40,                          //wMaxPacketSize                        
    0x00,                          
    0x00,                          //bInterval			
};
 
 
 
const UINT8 USB_String_Descriptor[] =   //USB字符串描述符
{
    0x1a,                      //bLength: 0x1A 字节
    0x03,                      //bDescriptorType: 字符串
     
    0x30,
    0x30,
    
    0x30,
    0x00,    
    0x30,
    0x00,
      
    0x30,
    0x00,    
    0x30,
    0x00,
        
    0x30,
    0x00,    
    0x30,
    0x00,    
        
    0x30,
    0x00,    
    0x30,
    0x00,
    
    0x38,
    0x00,    
    0x36,
    0x00,    
        
    0x34,
    0x00, 
};

 const unsigned char StringLangID[4] =
   {
	 0x04,
	 0x03,
	 0x09,
	 0x04
   }; /* LangID = 0x0409: U.S. English */
 
 const unsigned char StringVendor[2] =
   {
	 2, 3
   };
 
 const unsigned char StringProduct[32] =
   {
	 6+8+18, 3, 
	 'C', 0, 
	 'U', 0,
	 'P', 0, 
	 '_', 0,
	 'R', 0,
	 '2', 0,
	 ' ', 0, 
	 'B', 0,
	 'O', 0, 
	 'O', 0, 
	 ' T', 0,
	 ' ', 0,
	 '0', 0, 
	 '0', 0, 
	 '1', 0
   };
 const unsigned char StringSerial[18] =
   {
	 8+10,3,
	 '0',0,
	 '0',0,
	 '0',0,
	 '0',0,
	 '0',0,
	 '0',0,
	 '0',0,
	 '1',0
   };








 const UINT8 USB_report_Descriptor_hid[] =  //USB报告描述符
{                            
    0x05, 0x01,
    0x09, 0x00,   //自定义数据               
    0xA1, 0x01,    
              
    0x15, 0x00,               
    0x25, 0xFF,     
             
    0x19, 0x01,              
    0x29, 0x08,         
            
            
    0x95, 0x08,              
    0x75, 0x08,
    
    0x81, 0x02,   //input     
       
    0x19, 0x01,                      
    0x29, 0x08, 
                         
    0x91,0x02,   //output
                 
    0xC0                     
};



/* const UINT8 USB_report_Descriptor2[] =  //USB报告描述符  8个字节
{                            
    0x05, 0x01,
    0x09, 0x00,   //自定义数据               
    0xA1, 0x01,    
              
    0x15, 0x00,               
    0x25, 0xFF,     
             
    0x19, 0x01,              
    0x29, 0x08,         
            
            
    0x95, 0x08,              
    0x75, 0x08,
    
    0x81, 0x02,   //input     
       
    0x19, 0x01,                      
    0x29, 0x08, 
                     
    0x91,0x02,    //output
                  
    0xC0                     
};
*/




const UINT8 USB_report_Descriptor2[] =  //USB报告描述符 52个字节
{                            
    0x06, 0xA0,0xFF,		//全局，将用途页选择为?
    0x09, 0x01, 			//局部，将用途选择为01
    0xA1, 0x01,			//主条目，COLLECTION开集合，为应用集合
    0x09, 0x02,               	//局部，将用途选择为02
    0xA1, 0x00,  			//主条目，COLLECTION开集合，为?
    0x06, 0xA1,0xFF, 		//全局，将用途页选择为?
       
             
    0x09, 0x03,			//局部，将用途选择为03
    0x09, 0x04,			//局部，将用途选择为04
    0x15, 0x80, 			//全局，逻辑值最小为80
    0x25, 0x7F,			//全局，逻辑值最大为7F
    0x35, 0x00,              	//全局，物理值最小为00
    0x45, 0xFF,			//全局，物理值最大为FF
    0x75, 0x08,			//全局，每个数据域长度为8位
    
    0x95, 0x40,              	//全局，数据域的数量为64个
    0x81, 0x02,			//主条目，数据位输入
    
    0x09, 0x05,              	//局部，将用途选择为05
    0x09, 0x06, 
    0x15, 0x80,              
    0x25, 0x7F,
    0x35, 0x00,              
    0x45, 0xFF,
    0x75, 0x08,			
    
    0x95, 0x40,			//全局，数据域的数量为64个
    0x91, 0x02,              	//主条目，数据位输出
   
    0xC0,                    		//END COLLECTION
    0xC0                     		//END COLLECTION
};






void read_ep_buf(unsigned char *out_buf,unsigned long ep,unsigned short uiLens)
{
	unsigned short i=0;
	for (i=0;i<uiLens;i++)
	{
		out_buf[i] = read_port_byte(ep);
	}
}


void write_ep_buf(unsigned long ep,unsigned char *out_buf,unsigned short uiLens)
{
	unsigned short i;
	for (i=0;i<uiLens;i++)
	{
		write_port_byte(ep,out_buf[i]);
	}
}


void  USBDev_EP0_SendStall(void)
{
	unsigned char  ucReg = gUSBC_IdxReg->E0CSR_L;
	ucReg |= DEV_CSR0_SENDSTALL;
	gUSBC_IdxReg->E0CSR_L   = ucReg;
}

void write_ep0_buf(unsigned char *in_buf, unsigned short uiLen)
{
	unsigned char  ucReg  = gUSBC_IdxReg->E0CSR_L;
	unsigned char  package=0;


	if(uiLen>64)
	{
		while(uiLen>64)
		{
			write_ep_buf(USB_FIFO_ENDPOINT_0,in_buf+package*64,64);	    
			ucReg  |=  DEV_CSR0_TXPKTRDY;    // buffer填满后，告诉USB开始发送
			gUSBC_IdxReg->E0CSR_L  =  ucReg;

			uiLen-=64;
			package++;
			Delay(20);
		}	

		write_ep_buf(USB_FIFO_ENDPOINT_0,in_buf+package*64,uiLen); 	
		
		ucReg  |=  DEV_CSR0_DATAEND | DEV_CSR0_TXPKTRDY;	// buffer填满后，告诉USB开始发送
		gUSBC_IdxReg->E0CSR_L  =  ucReg;	
	}
	else
	{
		write_ep_buf(USB_FIFO_ENDPOINT_0,in_buf+package*64,uiLen);		
		ucReg  |=  DEV_CSR0_DATAEND | DEV_CSR0_TXPKTRDY;	// buffer填满后，告诉USB开始发送
		gUSBC_IdxReg->E0CSR_L  =  ucReg;	

	}
}


void USB_Init()
{
	PHYPA   |=  USB_WORDSWITCH;                                  //配置usb PHY
	gUSBC_fifoReg  = (sUSBCFIFO_Reg*)(USBC_BASE+ 0x1A);
	gUSBC_ComReg   = (sUSBCCommonReg*)USBC_BASE;
	gUSBC_IdxReg   = (sUSBCIndexedReg*)(USBC_BASE + 0x10);
	
	gpram_bSuspendSupport    = 0;                               //禁能Suspend功能
//	gUSBC_ComReg->INTRUSBE   =   USB_INTERRUPT_RESET            //使能USB中断
//	                            |USB_INTERRUPT_CONNECT
//	                            |USB_INTERRUPT_DISCON
//	                            |USB_INTERRUPT_SUSPEND
//	                            |USB_INTERRUPT_RESUME;

	gUSBC_ComReg->INTRTXE_L   =  USB_INTERRUPT_EP0              //使能EP0中断和EP1发送中断             
	                            |(1<<USB_ENDPOINT_INDEX);
	 
	gUSBC_ComReg->INTRRXE_L   =  (1<<USB_ENDPOINT_INDEX);       //使能EP1接收中断                         
	                            
	gUSBC_ComReg->EINDEX   =  CONTROL_EP;                       //把EP0设为当前要操作的端点	
	
	if(gpram_usbVer == 1)
		gUSBC_ComReg->UCSR   = USB_POWER_SOFT_CONN | USB_POWER_HS_ENAB;    //打开软连接和高速模式
	else
		gUSBC_ComReg->UCSR  = USB_POWER_SOFT_CONN;
			
			
	gpram_USBAddrFlag   = 0;
	gpram_USBNewAddr    = 0;
}


void USBDev_SetTxSize(unsigned short Size)
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

void Delay_Time_f(unsigned long time)
{
	unsigned long i=0;
	while (((volatile unsigned long)(i))!= time)
	{i++;}	
}



void USBDev_BusReset(unsigned char flag)
{
	unsigned char ucMode;
	unsigned char tempL,tempH;
	unsigned long i=0;
	if (flag == 0)
	{
		while (i!= 0x3FFFF)
		{
		i++;			
		}
		gUSBC_ComReg->FADDRR   = 0 ;    //复位后usb设备地址为0
	}

	gUSBC_ComReg->EINDEX  = USB_ENDPOINT_INDEX;   //把EP1设为当前要操作的端点 
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
}
	
	
void USBDev_getreport(void)
{
	UINT8 ucReg;
	ucReg  = dev_std_req.wCount>>8;
	gUSBC_IdxReg->E0CSR_L  = DEV_CSR0_SERVICE_RXPKTRDY;
	write_ep0_buf(report_buffer,ucReg);
}
	
		
void USBDev_setreport(void)                           //设置报告
{
	gUSBC_ComReg->EINDEX = CONTROL_EP;
	gUSBC_IdxReg->E0CSR_L = DEV_CSR0_SERVICE_RXPKTRDY;
	setreport_flag=1;
}
	
void USBDev_Ep0Handler(void)
{
	unsigned char  ucReq  = 0;
	volatile unsigned short   wValue = 0;
	read_ep_buf((unsigned char*)&dev_std_req,USB_FIFO_ENDPOINT_0,8);
	ucReq  = dev_std_req.bRequest;
	

	switch(ucReq)
	{

		case 0:
		{
			usb_get_status();
			break;
		}
	

		case 5:                               //USB协议，set address
		{
			ucReq = dev_std_req.wValue>>8;
			gUSBC_IdxReg->E0CSR_L  = DEV_CSR0_SERVICE_RXPKTRDY|DEV_CSR0_DATAEND;
			gpram_USBNewAddr  =  ucReq;
			gpram_USBAddrFlag = 1;
			break;	
		}
		case 6:                               //USB协议，获取描述符
		{
			USBDev_GetDescriptor();
			break;
		}
		case 8:                               //USB协议，获取配置，返回当前配置
			ucReq = gpram_configFlag;
			gUSBC_IdxReg->E0CSR_L  = DEV_CSR0_SERVICE_RXPKTRDY|DEV_CSR0_DATAEND;
			write_ep0_buf(&ucReq,1); 
			break;
		case 9:	
		{
			if(dev_std_req.bmRequestType == 0x21)                    //因为bmRequestTyped的D5~D6不为0，所以不是USB标准请求。
				                                                      //是HID协议中规定的set report，则按照set report来操作
			{
				USBDev_setreport();								
			}
			else                               //USB协议，设置配置
			{
				gpram_configFlag  = dev_std_req.wValue & 0x0F;    //对于复合设备，把接口号保存下来,当作当前配置
				gUSBC_IdxReg->E0CSR_L  = DEV_CSR0_SERVICE_RXPKTRDY|DEV_CSR0_DATAEND;
			}
			break;
		}
		case 10:    
		{
			if(dev_std_req.bmRequestType == 0x21)         //当dev_std_req.bmRequestType == 0x21时，是HID协议中规定的set idle，则会出现问题          
			{
				 USBDev_EP0_SendStall();
	    			 break;							
			}
			else                                         //USB协议，GetInterface
			{
				ucReq =0 ;	
				gUSBC_IdxReg->E0CSR_L  = DEV_CSR0_SERVICE_RXPKTRDY;
				write_ep0_buf(&ucReq,1);
			}
			break;			
		}
		case 11:           			//USB协议，set interface
		{
			gUSBC_IdxReg->E0CSR_L  = DEV_CSR0_SERVICE_RXPKTRDY|DEV_CSR0_DATAEND;
			break;
		}
		default:
		{
			USBDev_EP0_SendStall();
			break;
		}
	}
}


void USBDev_GetDescriptor(void)
{
	unsigned int  i;
	unsigned char USBDEV_Descriptor[18];
	unsigned char USBDEV_Configuration_Descriptor[86];
	unsigned char USBDEV_String_Descriptor[26];
	unsigned char ucReg;
	unsigned char reqIndex;
	unsigned char ucIndex;
	
	ucReg = dev_std_req.wCount>>8;		//长度
	reqIndex = dev_std_req.wValue & 0xFF;    //描述符的索引值
	ucIndex  = dev_std_req.wValue>>8;         //描述符的类型
	gUSBC_IdxReg->E0CSR_L  = DEV_CSR0_SERVICE_RXPKTRDY;   //清空接收FIFO
	for (i=0;i<18;i++)
	{
		USBDEV_Descriptor[i] = USB_Device_Descriptor[i];	
	}
	for (i=0;i<86;i++)
	{
		USBDEV_Configuration_Descriptor[i] = USB_Configuration_Descriptor[i];	
	}
	for (i=0;i<26;i++)
	{
		USBDEV_String_Descriptor[i] = USB_String_Descriptor[i];	
	}
	switch(reqIndex)
	{
		case  DEVICE_TYPE:                          //设备描述符
		{
			if(ucReg<19)
			{
				write_ep0_buf((unsigned char*)USBDEV_Descriptor,ucReg);
			}
			else	//返回8字节
			{
				write_ep0_buf((unsigned char*)USBDEV_Descriptor,8);
			}
			break;
		}
		case  CONFIG_TYPE:                         //配置描述符
		{
			if(ucReg>USB_Configuration_Descriptor[2])
			{
				ucReg = USB_Configuration_Descriptor[2];
			}
	
			write_ep0_buf((unsigned char*)USBDEV_Configuration_Descriptor,ucReg);
			break;
		}
		case STRING_TYPE:                          //字符串描述符
		{
			switch(ucIndex)	
			{
				case  0:
					if(ucReg > StringLangID[0])
							ucReg = StringLangID[0];
					write_ep0_buf((unsigned char*)StringLangID,ucReg);
					
					break;

				case  1:
					if(ucReg > StringVendor[0])
							ucReg = StringVendor[0];
					write_ep0_buf((unsigned char*)StringVendor,ucReg);
					break;					

				case  2:
					if(ucReg > StringProduct[0])
							ucReg = StringProduct[0];
					write_ep0_buf((unsigned char*)StringProduct,ucReg);
					break;	

				case  3:
					if(ucReg > StringSerial[0])
							ucReg = StringSerial[0];
					write_ep0_buf((unsigned char*)StringSerial,ucReg);
					break;
				default:
					USBDev_EP0_SendStall();
					break;
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
			if(ucReg < 19 )
			{
				write_ep0_buf((unsigned char*)USB_Device_Qualifier_Descriptor,ucReg);
			}
			else
			{
				write_ep0_buf((unsigned char*)USB_Device_Qualifier_Descriptor,8);
			}
			break;
		}
		case OTHER_SPEED:
		{
			if(ucReg<10)
			{
				write_ep0_buf((unsigned char*)USB_Configuration_Descriptor,ucReg);
			}
			else
			{
				USBDEV_Configuration_Descriptor[22] = 0x40;
				USBDEV_Configuration_Descriptor[23] = 0x00;
				USBDEV_Configuration_Descriptor[29] = 0x40;
				USBDEV_Configuration_Descriptor[30] = 0x00;
				write_ep0_buf(USBDEV_Configuration_Descriptor,0x20);
			}
			break;
		}
		case INTERFACE_POWER:
		{
			USBDev_EP0_SendStall();
			break;
		}
		case REPORT_TYPE:                 //HID协议里规定的报告描述符
		{
			ucReg = 0x34;
			write_ep0_buf((unsigned char*)USB_report_Descriptor2,ucReg);
			break;
		}
		default:
		{
			USBDev_EP0_SendStall();
			break;
		}
	}
}


void usb_get_status(void)
{
	unsigned char status[2] = {0x00, 0x00};
	switch(dev_std_req.wIndex)
	{
	case 0:
		write_ep0_buf(status,2);
		break;
		
	case 0x0001:
		status[0] = 0x00;
		write_ep0_buf(status,2);
		break;
		
	case 0x0082:
		status[0] = 0x00;
		write_ep0_buf(status,2);
		break;
		
	default:
		USBDev_EP0_SendStall();
		break;
	}
}



void SetTx()
{
	unsigned char ucReg = 0;
	ucReg  = 0x01;
	gUSBC_IdxReg->TXCSR_L = ucReg;
}


void SetFifo(unsigned long fifoAddr,unsigned char flag)
{
	if(flag == 0)               
	{
		gUSBC_fifoReg->TX_fifoadd_L    = ((fifoAddr>>3)&0x00FF);
		gUSBC_fifoReg->TX_fifoadd_H    = ((fifoAddr>>11)&0x00FF);
	}
	else if(flag == 1)
	{
		gUSBC_fifoReg->RX_fifoadd_L    = ((fifoAddr>>3)&0x00FF);
		gUSBC_fifoReg->RX_fifoadd_H    = ((fifoAddr>>11)&0x00FF);
	}
	else
	{
		gUSBC_fifoReg->TX_fifoadd_L    = ((fifoAddr>>3)&0x00FF);	   
   		gUSBC_fifoReg->TX_fifoadd_H    = ((fifoAddr>>11)&0x00FF);	
   		gUSBC_fifoReg->RX_fifoadd_L    = ((fifoAddr>>3)&0x00FF);	
   		gUSBC_fifoReg->RX_fifoadd_H    = ((fifoAddr>>11)&0x00FF);
   	}
}



UINT16 usb_receive(unsigned char *buf)
{
	UINT16   uiRxCount;
	UINT8    ucReg;
	uiRxCount   =  gUSBC_IdxReg->RXCount_H;
	uiRxCount   <<= 8;
	uiRxCount += gUSBC_IdxReg->RXCount_L;
	
	if(uiRxCount == 0)
		return;
	
	if(  uiRxCount > 0  )
	{
		read_ep_buf((UINT8 *)buf,USB_ENDPOINT_FIFO_ADDR,uiRxCount);
		ucReg  = gUSBC_IdxReg->RXCSR_L;
		ucReg  &= 0xFE;                 //告诉USB接收完成
		gUSBC_IdxReg->RXCSR_L  = ucReg;
	}
	return  uiRxCount;
}
	

void usb_send(unsigned char *buf,unsigned char len)
{
	unsigned char i;
	unsigned char  ucReg;
	SetFifo(USB_FIFO_OFFSET,2);
	write_ep_buf(USB_ENDPOINT_FIFO_ADDR,buf,len);
	USBDev_SetTxSize(len);                                      //发送的时候包的大小要设一下
	SetTx();
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
