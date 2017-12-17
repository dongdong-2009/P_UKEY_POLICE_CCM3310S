
#ifdef  USBDEV_ISR_SYMBOL
#define	USBDEV_ISR_EXT
#else	
#define	USBDEV_ISR_EXT extern
#endif


USBDEV_ISR_EXT   void USBDev_ISR(void);
USBDEV_ISR_EXT   void USBDev_EP0_ISR(void);
USBDEV_ISR_EXT   unsigned char USBDev_EP1_RX_ISR(unsigned char* pData);
USBDEV_ISR_EXT   void USBDev_EP1_TX_ISR(unsigned char* pData,unsigned short len);

