#ifndef __USB_CCID_H
#define __USB_CCID_H


#define PC_to_RDR_IccPowerOn        0x62         
#define PC_to_RDR_IccPowerOff       0x63         
#define PC_to_RDR_GetSlotStatus     0x65         
#define PC_to_RDR_XfrBlock          0x6F         
#define PC_to_RDR_GetParameters     0x6C         
#define PC_to_RDR_ResetParameters   0x6D       
#define PC_to_RDR_SetParameters     0x61         
#define PC_to_RDR_Escape            0x6B         
#define PC_to_RDR_IccClock          0x6E         
#define PC_to_RDR_T0APDU            0x6A         
#define PC_to_RDR_Secure            0x69         
#define PC_to_RDR_Mechanical        0x71         
#define PC_to_RDR_Abort             0x72         
#define PC_to_RDR_SetDataRateAndClockFrequency 0x73

#define RDR_to_PC_DataBlock     0x80
#define RDR_to_PC_SlotStatus    0x81
#define RDR_to_PC_Parameters    0x82
#define RDR_to_PC_Escape        0x83           
#define RDR_to_PC_DataRateAndClockFrequency 0x84


typedef struct _ccid_cmd{
	unsigned char type;
	unsigned char length[4];
	unsigned char slot;
	unsigned char seq;
	unsigned char param[3];
	unsigned char *pdata;
}CCID_CMD, *PCCID_CMD;

typedef struct _apdu{
	unsigned char *pCommBuff;
	unsigned char *pInData;		
	unsigned char *pOutData;
	unsigned int LC;
	unsigned int LE;
	unsigned int  RLE;	
	unsigned char CLA;
	unsigned char INS;
	unsigned char PP1;
	unsigned char PP2;
	unsigned char PP3;
	unsigned char LCH;
	unsigned char LCL;
	unsigned char SW1;
	unsigned char SW2;
}APDU;


#define BULK_BUF_MAXLEN     0x40
#define NULL 0
#endif __USB_CCID_H
