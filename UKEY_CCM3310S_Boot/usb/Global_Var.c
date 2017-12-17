#include "Global_Var.h"
#include "stdio.h"

APDU		g_APDU;
PCCID_CMD	ccid_cmd;

unsigned char g_b_Comm[COMMUNICATION_BUF_LEN];
unsigned short g_BulkOutlen;
unsigned char g_AtrOk;
unsigned char g_IV[8];
unsigned char g_b_Buf[0x200];


void MID_GVAR_Init(void)
{
	memset(&g_APDU, 0, sizeof(APDU));	
	memset(g_IV, 0, 8);
	ccid_cmd = (PCCID_CMD)g_b_Comm;
	g_APDU.pCommBuff = g_b_Comm+10;
	g_APDU.pInData = g_b_Comm +17;
	g_APDU.pOutData = g_b_Comm + 10;

	
}

