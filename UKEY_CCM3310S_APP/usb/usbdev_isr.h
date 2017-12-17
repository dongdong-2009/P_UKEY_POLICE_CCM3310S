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

#ifdef  USBDEV_ISR_SYMBOL
#define	USBDEV_ISR_EXT
#else	
#define	USBDEV_ISR_EXT extern
#endif


USBDEV_ISR_EXT   void USBDev_ISR(void);
USBDEV_ISR_EXT   void USBDev_EP0_ISR(void);
USBDEV_ISR_EXT   UINT16 USBDev_EPx_RX_ISR(UINT8* pData);
USBDEV_ISR_EXT   void USBDev_EPx_TX_ISR(UINT8* pData, UINT16 len);


