							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: app_command.h
Author:        Version:        Date: 20121230
Description:  应用命令的处理
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#ifdef  APP_COMMAND_SYMBOL
#define	APP_COMMAND_EXT
#else	
#define	APP_COMMAND_EXT extern
#endif

#define  	COMMUNICATION_BUF_LEN	0x410

typedef struct
{
	unsigned short cmd;
	unsigned short (*function)();
}CMD_TABLE;


typedef struct
{
	unsigned char type;
	unsigned char key_sta;
	unsigned char iv_sta;
	unsigned char rfu;
	unsigned char key[16];
	unsigned char iv[16];
}CRYPT_DATA;

typedef struct
{	
	unsigned char state;
	unsigned char type;
	unsigned char keyid[2];//会话秘钥ID	
	unsigned char key[16];
}SESSION_KEY;
APP_COMMAND_EXT UINT32 g_eflash_addr;

//APP_COMMAND_EXT UINT8 g_APDU_BUF[2048];
APP_COMMAND_EXT UINT8 *g_APDU_BUF;
APP_COMMAND_EXT UINT16 g_APDU_LEN;

//APP_COMMAND_EXT UINT8 apdu_response[2048];	
APP_COMMAND_EXT UINT8 *apdu_response;
APP_COMMAND_EXT UINT8 *papdu_response;
APP_COMMAND_EXT UINT16 len_response;

APP_COMMAND_EXT UINT16 CMD_Read_SN(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *p_u16LeLength);
APP_COMMAND_EXT UINT16 EnableBootMode(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *p_u16LeLength);

APP_COMMAND_EXT UINT16 CMD_GetRandom(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);

APP_COMMAND_EXT UINT16 CMD_SM1_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM1_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM1_Encrypt(UINT8 *p_u8APDU,  UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM1_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);

APP_COMMAND_EXT UINT16 CMD_SM2_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_GenerateKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_SetUserID(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_ReadUserID(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_SignIDA(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_VerifyIDA(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_GetZ(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_GetE(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SM2_ExchangeKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);

APP_COMMAND_EXT UINT16 CMD_SM3_Hash(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SHA_Hash(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);

APP_COMMAND_EXT UINT16 CMD_RSA_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_RSA_GenerateKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_RSA_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_RSA_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_RSA_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_RSA_Sign(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_RSA_Verify(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);

APP_COMMAND_EXT UINT16 CMD_DES_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_DES_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_DES_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_DES_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);

APP_COMMAND_EXT UINT16 CMD_AES_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_AES_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_AES_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_AES_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);

APP_COMMAND_EXT UINT16 CMD_SMS4_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SMS4_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SMS4_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SMS4_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);

APP_COMMAND_EXT UINT16 CMD_SSF33_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SSF33_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SSF33_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);
APP_COMMAND_EXT UINT16 CMD_SSF33_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen);


