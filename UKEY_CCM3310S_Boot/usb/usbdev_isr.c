
#define	USBDEV_ISR_SYMBOL
#include "includes.h"


/**************************************************************************************/
void USBDev_ISR(void)
{
	unsigned char  ucISR;
	unsigned char  sectorOffset;
	
	gpram_msgflags |= gUSBC_ComReg->INTRUSB;                   //取走USB中断寄存器中的值
	
	gpram_msgflags1 |= gUSBC_ComReg->INTRTX_L;                 //取走USB的发送中断寄存器中的值
	ucISR     = gUSBC_ComReg->INTRRX_L;                        //取走USB的接收中断寄存器中的值
		
		
	if(ucISR & (1<<USB_ENDPOINT_INDEX))                        //如果是EP1的接收中断置位，那么占用gpram_msgflags中的BIT4位进行处理
	{
		gpram_msgflags  |= 0x10;
	}
	
	if(gpram_msgflags1 & 0x1)                                  //如果是EP0中断
	{
		if (gpram_USBAddrFlag == 1)                        //如果地址改写了
		{
			gUSBC_ComReg->FADDRR = gpram_USBNewAddr;   //把新地址放进去进行操作
			gpram_USBAddrFlag  = 0;
		}
		gpram_msgflags1 &= 0xFE;                           //清EP0中断标志			
		
		USBDev_EP0_ISR();
	}
	if (gpram_msgflags & 0x10)                                 //如果是EP1的接收中断置位
	{

		gpram_msgflags &= 0xEF;                            //清EP1接收中断标志
		g_uchUSBStatus |= 0x01;		
	}
	if (gpram_msgflags1 & (1<<USB_ENDPOINT_INDEX) )            //如果是EP1的发送中断置位
	{
		gpram_msgflags1 &= ( ~(1<<USB_ENDPOINT_INDEX) );   //清EP1发送中断标志
		g_uchUSBStatus |= 0x10;	
		
	}
			
	if (gpram_msgflags & 0x04)                                 //如果是bus reset
	{
		gpram_msgflags &= 0xFB;    
		USBDev_BusReset(0);

	}
		
	if(gpram_msgflags & 0x1)                                   //如果是suspend
	{
		gpram_msgflags &= 0xFE;		

	}		
}
/////////////////////////////////////////////////////////////////////////////////////////





/*************************************************************************************/
void USBDev_EP0_ISR(void)
{
	//unsigned char ucReqType=0,ucCount=0;
	unsigned char ucReg=0;

	gUSBC_ComReg->EINDEX  = CONTROL_EP;     
	ucReg= gUSBC_IdxReg->E0CSR_L;
	
	//clear SentStall bit
	if(ucReg &0x04)      //如果SendStall发送完成，则清标志
	{
		ucReg &=0xFB;		
	}

	if(ucReg & 0x10)     //	如果setup end发生，那么清setup end标志位
	{
		ucReg |=0x80;
	}
  
	gUSBC_IdxReg->E0CSR_L  = ucReg;


	if(ucReg & 0x01)    //如果接收到数据包
	{
		USBDev_Ep0Handler();
	}    

}
/////////////////////////////////////////////////////////////////////////////////////////
unsigned char USBDev_EP1_RX_ISR(unsigned char* pData)
{
	unsigned char  ucRegLow;
	unsigned char  number;
	number = 0;
	gUSBC_ComReg->EINDEX = USB_ENDPOINT_INDEX;
	ucRegLow  = gUSBC_IdxReg->RXCSR_L;
	if(ucRegLow & DEV_RXCSR_SENTSTALL)                            //如果检测到sendstall完成
		{
			ucRegLow  &= 0xBF;                                    //清除该标志位
			ucRegLow  |= DEV_RXCSR_CLR_DATA_TOG;                  //复位该端点的数据
			gUSBC_IdxReg->RXCSR_L  = ucRegLow;                    
		}
	if(ucRegLow & DEV_RXCSR_RXPKTRDY  )
  		{
  			number = usb_receive(pData);  		
  		}
  		return (number);
}

void USBDev_EP1_TX_ISR(unsigned char* pData,unsigned short len)
{
	    gUSBC_ComReg->EINDEX  = USB_ENDPOINT_INDEX;
  		usb_send(pData,len); 
}


