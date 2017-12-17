

typedef struct _sUSBCCommonReg{
	 volatile unsigned char FADDRR;
	 volatile unsigned char UCSR;

   volatile unsigned char INTRTX_L; 
   volatile unsigned char INTRTX_H;
  
   volatile unsigned char INTRRX_L;                                
   volatile unsigned char INTRRX_H;
   
   volatile unsigned char INTRTXE_L;                      
   volatile unsigned char INTRTXE_H;                            
   
   volatile unsigned char INTRRXE_L;
   volatile unsigned char INTRRXE_H;
  
   volatile unsigned char INTRUSB;                                
   volatile unsigned char INTRUSBE;
   
   volatile unsigned char FNUMR_L;                               
   volatile unsigned char FNUMR_H;
   
   volatile unsigned char EINDEX;                               
   volatile unsigned char TESTMODE;
}sUSBCCommonReg;


typedef struct _sUSBCINdexedReg{
	volatile unsigned char TXPSZR_L;              //10  
	volatile unsigned char TXPSZR_H;              //11  
	union                                         //12
		{
		volatile unsigned char E0CSR_L;
		volatile unsigned char TXCSR_L;
		};
	union                                        //13
		{
		volatile unsigned char E0CSR_H;
		volatile unsigned char TXCSR_H;
		};   

	volatile unsigned char RXPSZR_L;            //14
	volatile unsigned char RXPSZR_H;            //15            
                                    
	volatile unsigned char RXCSR_L;              //16
	volatile unsigned char RXCSR_H;              //17        
                                
	union
		{
		volatile unsigned char E0COUNTR_L;       //18
		volatile unsigned char RXCount_L;
		};
	union
		{
		volatile unsigned char E0COUNTR_H;        // 19     
		volatile unsigned char RXCount_H;
		}; 
}sUSBCIndexedReg;


typedef  struct _sUSBCFIFO_Reg{
	 volatile unsigned char TX_fifosz;
	 volatile unsigned char RX_fifosz;
	 volatile unsigned char TX_fifoadd_L;
	 volatile unsigned char TX_fifoadd_H;
	 volatile unsigned char RX_fifoadd_L;
	 volatile unsigned char RX_fifoadd_H;
}sUSBCFIFO_Reg;

//#define USB_BUFFER_ADDR     (0x00800000 + USB_FIFO_OFFSET + 0x200)


#define USB_FIFO_ENDPOINT_0  USBC_BASE + 0x20                                   
#define USB_FIFO_ENDPOINT_1  USBC_BASE + 0x24                                       
#define USB_FIFO_ENDPOINT_2  USBC_BASE + 0x28                                
#define USB_FIFO_ENDPOINT_3  USBC_BASE + 0x2C                                      
#define USB_FIFO_ENDPOINT_4  USBC_BASE + 0x30                                   
#define USB_FIFO_ENDPOINT_5  USBC_BASE + 0x34                                    
#define USB_FIFO_ENDPOINT_6  USBC_BASE + 0x38                                         
#define USB_FIFO_ENDPOINT_7  USBC_BASE + 0x3C                                         



#define USB_EP1_TEST

#ifdef USB_EP1_TEST
#define USB_ENDPOINT_FIFO_ADDR                          USB_FIFO_ENDPOINT_1
#define USB_ENDPOINT_INDEX                              INDEX_EP1
#endif                                                  
                                                        
#ifdef USB_EP2_TEST                                     
#define USB_ENDPOINT_FIFO_ADDR                          USB_FIFO_ENDPOINT_2
#define USB_ENDPOINT_INDEX                              INDEX_EP2
#endif                                                  
                                                        
#ifdef USB_EP3_TEST                                     
#define USB_ENDPOINT_FIFO_ADDR                          USB_FIFO_ENDPOINT_3
#define USB_ENDPOINT_INDEX                              INDEX_EP3
#endif                                                  
                                                        
#ifdef USB_EP4_TEST                                     
#define USB_ENDPOINT_FIFO_ADDR                          USB_FIFO_ENDPOINT_4
#define USB_ENDPOINT_INDEX                              INDEX_EP4
#endif                                                  
                                                        
                                                        
#ifdef USB_EP5_TEST                                     
#define USB_ENDPOINT_FIFO_ADDR                          USB_FIFO_ENDPOINT_5
#define USB_ENDPOINT_INDEX                              INDEX_EP5
#endif                                                  
                                                        
                                                        
#ifdef USB_EP6_TEST                                     
#define USB_ENDPOINT_FIFO_ADDR                          USB_FIFO_ENDPOINT_6
#define USB_ENDPOINT_INDEX                              INDEX_EP6
#endif                                                  
                                                        
                                                        
#ifdef USB_EP7_TEST                                     
#define USB_ENDPOINT_FIFO_ADDR                          USB_FIFO_ENDPOINT_7
#define USB_ENDPOINT_INDEX                              INDEX_EP7
#endif                                                  
                                                        
                                                        
                                                        
#define 	CONTROL_EP                                     0
#define 	DATA_IN_EP                                     1
#define 	DATA_OUT_EP                                    1
#define     INDEX_EP1                                      1	
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
                                                        
                                                        
#define 	USB_MAX_PACKET_SIZE                           512
#define 	USB_MAX_PACKET_SIZE_LOW                       0x00
#define 	USB_MAX_PACKET_SIZE_HIGH                      0x02
#define 	USB_MAX_PACKET_SIZE_V11                       64
#define 	USB_MAX_PACKET_SIZE_LOW_V11                   0x40
#define 	USB_MAX_PACKET_SIZE_HIGH_V11                  0x00
                                                        
                                                        
                                                        
#define 	USB_INTERRUPT_EP0                             0x01  
#define 	USB_TX_INTERRUPT_EP1                          0x02  
#define 	USB_TX_INTERRUPT_EP2                          0x04	
#define     USB_TX_INTERRUPT_EP3                          0x08  
#define 	USB_TX_INTERRUPT_EP4                          0x10  
#define 	USB_TX_INTERRUPT_EP5                          0x20	
#define 	USB_TX_INTERRUPT_EP6                          0x40  
#define 	USB_TX_INTERRUPT_EP7                          0x80	
                                                        
#define 	USB_RX_INTERRUPT_EP0                          0x01  
#define 	USB_RX_INTERRUPT_EP1                          0x02  
#define 	USB_RX_INTERRUPT_EP2                          0x04	
#define     USB_RX_INTERRUPT_EP3                          0x08  
#define 	USB_RX_INTERRUPT_EP4                          0x10  
#define 	USB_RX_INTERRUPT_EP5                          0x20	
#define 	USB_RX_INTERRUPT_EP6                          0x40  
#define 	USB_RX_INTERRUPT_EP7                          0x80	
                                                        
#define 	DEV_CSR0_RXPKTRDY                             0x01  
#define 	DEV_CSR0_TXPKTRDY                             0x02  
#define 	DEV_CSR0_SENTSTALL                            0x04	
#define     DEV_CSR0_DATAEND                              0x08  
#define 	DEV_CSR0_SETUPEND                             0x10  
#define 	DEV_CSR0_SENDSTALL                            0x20	
#define 	DEV_CSR0_SERVICE_RXPKTRDY                     0x40  
#define 	DEV_CSR0_SERVICE_SETUPEND                     0x80	
                                                        
                                                        
#define 	DEV_TXCSR_TXPKTRDY                            0x01
#define 	DEV_TXCSR_FIFO_NOT_EMPTY                      0x02
#define 	DEV_TXCSR_UNDER_RUN                           0x04
#define     DEV_TXCSR_FLUSH_FIFO                          0x08
#define 	DEV_TXCSR_SEND_STALL                          0x10
#define 	DEV_TXCSR_SENT_STALL                          0x20
#define 	DEV_TXCSR_CLR_DATA_TOG                        0x40
#define 	DEV_TXCSR_INCOMP_TX                           0x80
                                                        
                                                        
#define 	DEV_TXCSR_FRC_DATA_TOG                        0x08
#define 	DEV_TXCSR_DMA_ENAB                            0x10
#define 	DEV_TXCSR_MODE                                0x20
#define     DEV_TXCSR_ISO                                 0x40
#define     DEV_TXCSR_AUTO_SET                            0x80
                                                        
                                                        
#define 	DEV_RXCSR_RXPKTRDY                            0x01 
#define 	DEV_RXCSR_FIFOFULL                            0x02 
#define 	DEV_RXCSR_OVERRUN                             0x04 
#define     DEV_RXCSR_DATA_ERROR                          0x08 
#define 	DEV_RXCSR_FLUSH_FIFO                          0x10 
#define 	DEV_RXCSR_SENDSTALL                           0x20 
#define 	DEV_RXCSR_SENTSTALL                           0x40 
#define 	DEV_RXCSR_CLR_DATA_TOG                        0x80 
                                                        
                                                        
#define 	DEV_RXCSR_INCOMP_RX                           0x01 
#define 	DEV_RXCSR_DMAMODE                             0x08 
#define 	DEV_RXCSR_DISNYET                             0x10 
#define     DEV_RXCSR_DMA_ENAB                            0x20 
#define 	DEV_RXCSR_ISO                                 0x40  
#define 	DEV_RXCSR_AUTOCLEAR                           0x80 



#define TX                                          1
#define RX                                          0



typedef  struct  _device_request
		{
			UINT8  bmRequestType;
			UINT8  bRequest;
			UINT16 wValue;
			UINT16 wIndex;
			UINT16 wCount;
			}DEVICE_REQUEST;









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


#define   VENDOR_IDENTIFY_CMD                             		0xA0
#define   VENDOR_IDENTIFY_SUBCMD                          		0x01
                                                          		
                                                          		
#define   VENDOR_R_W_CMD                                  		0xB0
#define   VENDOR_READ_SUBCMD                              		0x80
#define   VENDOR_WRITE_SUBCMD                             		0x00
                                                          		
                                                          		
#define   VENDOR_CMD                                      		0xD1
#define   VENDOR_WRITEDATA                                		0xD2
                                                          		
                                                          		
#define   VENDOR_WRITECODE_SRAM                           		1 
#define   VENDOR_WRITEDATA_SRAM                           		2
#define   VENDOR_WRITE_FLASH                              		0 
                                                          		
                                                     	  		
#define 	WRITEINT                                      		0
#define 	READINT                                       		1
                                                          		
#define 	SAMSUNGTWOPLANE                               		0
#define 	MICRONTWOPLANE                                		1
#define 	TOSHIBATWOPLANE                               		2
                                                     	  		
#define 	DOUBLESECTORSIZE                              		1024
#define 	SECTORSIZE                                    		512
#define 	HALFSECTORSIZE                                		256
#define 	MISIZE                                        		8
#define 	MITABSIZE                                     		18
                                                     	  		
                                                         		
#define 	PAGE128                                       		0
#define 	PAGE256                                       		1          
#define 	PAGE512                                       		2
                                                     	  		
                                                     	  		
                                                     	  		
                                                     	  		
#define 	REQUEST_DEVICE_STATUS                         		0x80
#define 	REQUEST_INTERFACE_STATUS                      		0x81
#define 	REQUEST_ENDPOINT_STATUS                       		0x82
                                                     	  		
                                                     				
                                                     				
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
                                                          		
                                                          		
#define 	RIGHT_BTN_EJECT                               		1
#define 	LUN_READ_PROTECT                              		2
                                                          		
                                                          		
#define 	DEVICE_TYPE                                   		0x1
#define 	CONFIG_TYPE                                   		0x2
#define 	STRING_TYPE                                   		0x3
#define 	INTERFACE_TYPE                                		0x4
#define 	ENDPOINT_TYPE                                 		0x5
#define		DEVICE_QUALIFIER                              		0x6
                                                          		
#define 	OTHER_SPEED                                   		0x7
#define 	INTERFACE_POWER                               		0x8
#define 	REPORT_TYPE                                   		0x22






 void USBInit(void);
                                   
                                    
                                    
                                    
