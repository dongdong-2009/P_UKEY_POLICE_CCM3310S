#ifndef __GLOBAL_VAR_H
#define __GLOBAL_VAR_H

#include "USB_CCID.h"

#define 	NOK							0
#define 	OK							1
#define		COMMUNICATION_BUF_LEN		0x800




extern APDU 		g_APDU;
extern PCCID_CMD	ccid_cmd;

extern unsigned char 	g_b_Comm[];
extern unsigned short 	g_BulkOutlen;

extern unsigned char g_IV[8];
extern unsigned char g_b_Buf[0x200];



void MID_GVAR_Init(void);


#endif //__GLOBAL_VAR_H
