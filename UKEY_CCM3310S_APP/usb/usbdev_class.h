							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usbdev_class.h
Author:        Version:        Date: 20121230
Description:  usb设备类实现
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#ifdef  USBDEV_CLASS_SYMBOL
#define	USBDEV_CLASS_EXT
#else	
#define	USBDEV_CLASS_EXT extern
#endif
                                  	  	
void USBDev_Data_BulkOut(void);
void USBDev_Data_BulkIn(void);


