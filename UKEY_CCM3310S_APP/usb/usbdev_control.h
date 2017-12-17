							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: usbdev_control.h
Author:        Version:        Date: 20121230
Description:  usb设备枚举过程的实现
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#ifdef  USBDEV_CONTROL_SYMBOL
#define	USBDEV_CONTROL_EXT
#else	
#define	USBDEV_CONTROL_EXT extern
#endif

#define 	REQUEST_DEVICE_STATUS                         		0x80
#define 	REQUEST_INTERFACE_STATUS                      		0x81
#define 	REQUEST_ENDPOINT_STATUS                       		0x82

USBDEV_CONTROL_EXT UINT8  report_buffer[64];

USBDEV_CONTROL_EXT UINT8 gpram_WakeupFeature;	//1--endpoint halt feature is enable,0--disable
                     
USBDEV_CONTROL_EXT   DEVICE_REQUEST  dev_std_req;
                     
USBDEV_CONTROL_EXT   UINT8           setreport_flag;
                     
                     
                     
USBDEV_CONTROL_EXT   UINT8 gpram_USBAddrFlag;
USBDEV_CONTROL_EXT   UINT8 gpram_USBNewAddr;	
                     
USBDEV_CONTROL_EXT   UINT32 gpram_Doze_delay;
USBDEV_CONTROL_EXT   UINT8 gpram_bDozeSupport;
                     
USBDEV_CONTROL_EXT   UINT8 gpram_configFlag;


#define 	SCSICMD_TEST_UNIT_READY        			         	0x00
#define 	SCSICMD_REZERO_UNIT               			     	0x01
#define 	SCSICMD_REQUEST_SENSE                   			0x03
#define 	SCSICMD_FORMAT_UNIT        							0x04
#define 	SCSICMD_INQUIRY                         			0x12
#define 	SCSICMD_MODE_SENSE_06                   			0x1a
#define 	SCSICMD_START_STOP_UNIT                 			0x1b
#define 	SCSICMD_SEND_DIAGNOSTIC                 			0x1d
#define 	SCSICMD_PREVENT_ALLOW_MEDIUM_REMOVAL    			0x1e
#define 	SCSICMD_READ_FORMAT_CAPACITIES          			0x23
#define 	SCSICMD_READ_CAPACITY                   			0x25
#define 	SCSICMD_READ_10                         			0x28
#define 	SCSICMD_READ_12                         			0xa8
#define 	SCSICMD_WRITE_10                        			0x2a
#define 	SCSICMD_WRITE_12                        			0xaa
#define 	SCSICMD_SEEK_10                         			0x2b
#define 	SCSICMD_WRITE_AND_VERIFY_10             			0x2e
#define 	SCSICMD_VERIFY_10                       			0x2f
#define		SCSICMD_READ_TOC									0x43
#define 	SCSICMD_MODE_SELECT_10                  			0x55
#define 	SCSICMD_MODE_SENSE_10                  				0x5a
#define 	SCSICMD_GET_CONFIG                         			0x46
#define 	SCSICMD_EVENT_STATUS                        		0x4a
#define 	SCSICMD_READ_DISC                        			0x51
#define 	SCSICMD_GET_PERFORMANCE                       		0xac
#define 	SCSICMD_READ_TRK_INF             			        0x52
#define 	SCSICMD_SET_CD_SPEED                       			0xbb
#define		SCSICMD_READ_CD									    0xbe
#define 	SCSICMD_READ_DVD_STRU                  			  	0xad
#define 	SCSICMD_DUMMY_WRITE_10                  			0xdd

           				
#define		PHASE_READ_ISO                                		3
#define 	PHASE_REPORT_NO_MEDIA                         		2
#define 	PHASE_REPORT_MEDIA_CHANGE                     		1
#define 	PHASE_NORMAL                                  		0
                                                          		
                                                              		
#define 	NORMAL_SCSI_STATUS                            		0
#define 	REPORT_ABNORMAL_STATUS                        		1
#define 	UNSUPPORT_SCSI_CMD                            		2
#define 	DUMMY_RW_STATUS                               		3
#define 	DUMMY_RW_STATUS2                              		4
                                                          		
                                                          		
                                                          		
#define 	MUSB_CBW_SCSI_LUN_BYTE                        		13
#define 	MUSB_CBW_SCSI_CMD_BYTE                        		15      
#define  	MUSB_CBW_SCSI_LBA_LSB                         		20      	
#define 	MUSB_CBW_SCSI_TOC_DATA_LENGTH                 		22      
#define 	MUSB_CBW_SCSI_MODESENSE_DATA_LENGTH           		23     
#define 	MUSB_CBW_SCSI_CSW_STATUS                      		12 
#define		MUSB_CSW_SCSI_CSW_STATUS				12

 #define 	RIGHT_BTN_EJECT                               		1
#define 	LUN_READ_PROTECT                              		2


USBDEV_CONTROL_EXT unsigned char gpram_CRCEnable;

USBDEV_CONTROL_EXT volatile unsigned char gpram_tmpVPBEnable;//0 -- disable;1 -- enable;
USBDEV_CONTROL_EXT volatile unsigned char gpram_lastTmpVPBEnable;
USBDEV_CONTROL_EXT unsigned char gpram_speedupEnable;
//Eject_Test,indicate Host eject u-disk by right-button
USBDEV_CONTROL_EXT unsigned char gpram_Eject_Flag;

USBDEV_CONTROL_EXT unsigned char gpram_Abnormal_Flag;

USBDEV_CONTROL_EXT volatile unsigned char gpram_bulkPhase;	

USBDEV_CONTROL_EXT unsigned char gpram_WakeupFeature;	

// ISO_Test CD flag: 0: current LUN is CD, 1: current LUN is not CD
USBDEV_CONTROL_EXT unsigned char gpram_CDFlag;	// Add by wangtingping 20080507

//ISO_Test,remove ISO when first time read
USBDEV_CONTROL_EXT unsigned char gpram_ISOFlag;

//5..0,current LUN,(0xFF--private parititon)
USBDEV_CONTROL_EXT unsigned char gpram_currentLUN;	

//7--private partition flag,5..0--LUNs
USBDEV_CONTROL_EXT unsigned char gpram_maxLun;

USBDEV_CONTROL_EXT unsigned char gpram_ledIdleState;

USBDEV_CONTROL_EXT unsigned char gpram_ledWorkState;

USBDEV_CONTROL_EXT unsigned char gpram_eraseFlag;

USBDEV_CONTROL_EXT unsigned long gpram_cbwPacket_l[8];
USBDEV_CONTROL_EXT unsigned char* gpram_cbwPacket;

USBDEV_CONTROL_EXT unsigned char gpram_cswPacket[13];

USBDEV_CONTROL_EXT unsigned char gpram_dev_std_req[8];

USBDEV_CONTROL_EXT unsigned long gpram_Led_delay;
USBDEV_CONTROL_EXT unsigned long gpram_counter;

USBDEV_CONTROL_EXT unsigned long gpram_formatcapacity[5];

USBDEV_CONTROL_EXT   unsigned char gpram_subCmd;	
USBDEV_CONTROL_EXT   volatile unsigned long gpram_wrLength;

USBDEV_CONTROL_EXT   unsigned char gusb_receive_finish_flag;


USBDEV_CONTROL_EXT volatile unsigned int packetCount;
USBDEV_CONTROL_EXT unsigned int sramAddr;   //FIFO addr in the sram
USBDEV_CONTROL_EXT volatile unsigned int packetSize;
USBDEV_CONTROL_EXT volatile unsigned int dataSize;

USBDEV_CONTROL_EXT unsigned int performStatus; 
USBDEV_CONTROL_EXT unsigned int dataLen;
USBDEV_CONTROL_EXT   unsigned long gpram_databuf;
USBDEV_CONTROL_EXT   volatile unsigned char gpram_TxRxCount;


USBDEV_CONTROL_EXT   void read_ep_buf(UINT8 *out_buf,UINT32 ep,UINT16 uiLens);
USBDEV_CONTROL_EXT   void write_ep_buf(UINT32 ep,UINT8 *out_buf,UINT16 uiLens);
USBDEV_CONTROL_EXT   void USBDev_EP0_SendStall(void);
USBDEV_CONTROL_EXT   void write_ep0_buf(UINT8 *in_buf, UINT16 uiLen);
USBDEV_CONTROL_EXT   void USBDev_getreport(void);
USBDEV_CONTROL_EXT   void USBDev_setreport(void); 
USBDEV_CONTROL_EXT   void USBDev_Ep0Handler(void);
USBDEV_CONTROL_EXT   void USBDev_GetDescriptor(void);
USBDEV_CONTROL_EXT   void usb_SetTxSize(UINT16 Size);
USBDEV_CONTROL_EXT   UINT16 usb_receive(UINT8 *buf);
USBDEV_CONTROL_EXT   void usb_send(UINT8 *buf, UINT16 len);
USBDEV_CONTROL_EXT   void USBDev_ClearRx0();
USBDEV_CONTROL_EXT   void USBDev_GetStatus(void);
USBDEV_CONTROL_EXT   void USBDev_TestMode(void);

