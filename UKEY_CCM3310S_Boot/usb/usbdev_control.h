#ifdef  USBDEV_CONTROL_SYMBOL
#define	USBDEV_CONTROL_EXT
#else	
#define	USBDEV_CONTROL_EXT extern
#endif



USBDEV_CONTROL_EXT   sUSBCFIFO_Reg*    gUSBC_fifoReg;
USBDEV_CONTROL_EXT   sUSBCCommonReg*   gUSBC_ComReg;
USBDEV_CONTROL_EXT   sUSBCIndexedReg*  gUSBC_IdxReg;
                     
USBDEV_CONTROL_EXT   DEVICE_REQUEST  dev_std_req;
                     
USBDEV_CONTROL_EXT   unsigned int    VendorReadFlag;
USBDEV_CONTROL_EXT   unsigned int    ReadDataAddr;
USBDEV_CONTROL_EXT   unsigned int    ReadDataLength;
USBDEV_CONTROL_EXT   unsigned int    InitData;
                     
USBDEV_CONTROL_EXT   UINT8           setreport_flag;
                     
                     
                     
USBDEV_CONTROL_EXT   volatile unsigned char gpram_USBAddrFlag;
USBDEV_CONTROL_EXT   volatile unsigned char gpram_USBNewAddr;	
                     
USBDEV_CONTROL_EXT   unsigned long gpram_Doze_delay;
USBDEV_CONTROL_EXT   unsigned char gpram_bDozeSupport;
                     
USBDEV_CONTROL_EXT   volatile unsigned char gpram_msgflags;
USBDEV_CONTROL_EXT   volatile unsigned char gpram_msgflags1;
                     
USBDEV_CONTROL_EXT   unsigned char gpram_configFlag;
USBDEV_CONTROL_EXT   unsigned long g_WakeupFeature;
USBDEV_CONTROL_EXT   unsigned char gpram_usbVer;
                     
USBDEV_CONTROL_EXT   volatile unsigned char gpram_TxRxCount;
USBDEV_CONTROL_EXT   volatile unsigned long gpram_wrLength;
USBDEV_CONTROL_EXT   unsigned char gpram_bSuspendSupport;
                     
                     
USBDEV_CONTROL_EXT   unsigned char gpram_subCmd;	
USBDEV_CONTROL_EXT   unsigned char gpram_ClearRxFlag;
                     
USBDEV_CONTROL_EXT   unsigned long gpram_databuf;
                     
                     
USBDEV_CONTROL_EXT   unsigned char  g_uchUSBStatus;   //BIT0为1代表来了接收中断
                                          //BIT4为1代表来了发送中断
USBDEV_CONTROL_EXT unsigned char  control_buffer[64];                   
USBDEV_CONTROL_EXT unsigned char  report_buffer[64];                     
                     
                     
                     
                     
                     
USBDEV_CONTROL_EXT   void read_ep_buf(unsigned char *out_buf,unsigned long ep,unsigned short uiLens);
USBDEV_CONTROL_EXT   void write_ep_buf(unsigned long ep,unsigned char *out_buf,unsigned short uiLens);
USBDEV_CONTROL_EXT   void USBDev_EP0_SendStall(void);
USBDEV_CONTROL_EXT   void write_ep0_buf(unsigned char *in_buf, unsigned short uiLen);
USBDEV_CONTROL_EXT   void USB_Init();
USBDEV_CONTROL_EXT   void Delay_Time_f(unsigned long time);
USBDEV_CONTROL_EXT   void USBDev_Suspend(void);
USBDEV_CONTROL_EXT   void USBDev_BusReset(unsigned char flag);
USBDEV_CONTROL_EXT   void USBDev_getreport(void);
USBDEV_CONTROL_EXT   void USBDev_setreport(void); 
USBDEV_CONTROL_EXT   void USBDev_Ep0Handler(void);
USBDEV_CONTROL_EXT   void USBDev_GetDescriptor(void);
USBDEV_CONTROL_EXT   void ClearRx();
USBDEV_CONTROL_EXT   void SetTx();
USBDEV_CONTROL_EXT   void SetFifo(unsigned long fifoAddr,unsigned char flag);
USBDEV_CONTROL_EXT   UINT16 usb_receive(unsigned char *buf);
USBDEV_CONTROL_EXT   void usb_send(unsigned char *buf,unsigned char len);
USBDEV_CONTROL_EXT 	 void usb_get_status(void);

