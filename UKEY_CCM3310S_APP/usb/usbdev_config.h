							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usbdev_control.h
Author:        Version:        Date: 20121230
Description:  
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/
#ifdef  USBDEV_CONFIG_SYMBOL
#define	USBDEV_CONFIG_EXT
#else	
#define	USBDEV_CONFIG_EXT extern
#endif


extern const unsigned char USB_Device_Descriptor[18];

extern const unsigned char USB_Device_Qualifier_Descriptor[10];

extern const unsigned char USB_Configuration_Descriptor [];

extern const unsigned char USB_String_Descriptor[];
	
extern const unsigned char USB_Manufacturer_Descriptor[];

extern const unsigned char srialnumString[22+20];
	
extern const UINT8 UsbLanguageID[4];	

extern const unsigned char DBR[512];

extern const unsigned char MBR[512];

