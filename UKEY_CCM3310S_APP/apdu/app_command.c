							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: app_command.c
Author:        Version:        Date: 20121230
Description:  应用命令的处理
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define  APP_COMMAND_SYMBOL
#include "includes.h"

unsigned char g_rand[8],g_randstate=0;
SESSION_KEY g_session_key;
const unsigned char app_ver[]="20171205";
extern unsigned char g_reboot;




UINT16 CMD_Read_SN(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *p_u16LeLength)
{
	ReadSn(p_u8Response);
	*p_u16LeLength = 16;
	
	return 0x9000;
}

UINT16 EnableBootMode(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *p_u16LeLength)
{
	EnBootMode();
	return 0x9000;
}

UINT16 CMD_GetRandom(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	Genarate_Random(p_u8Response, g_u16APDULe);

	len_response = g_u16APDULe;
	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

UINT16 CMD_SM1_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	if (g_u16APDULc != 64)
	{
		return SW_LENGTH_ERROR;
	}
	
	//按顺序分别为EK、AK、SK、IV
	offset = 0;
	memcpy((UINT8*)g_sm1EK, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_SM1_EK_ADDR, (UINT8*)g_sm1EK, EFLASH_SM1_EK_SIZE);
	
	offset += 16;
	memcpy((UINT8*)g_sm1AK, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_SM1_AK_ADDR, (UINT8*)g_sm1AK, EFLASH_SM1_AK_SIZE);
	
	offset += 16;
	memcpy((UINT8*)g_sm1SK, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_SM1_SK_ADDR, (UINT8*)g_sm1SK, EFLASH_SM1_SK_SIZE);
	
	offset += 16;
	memcpy((UINT8*)g_sm1IV, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_SM1_IV_ADDR, (UINT8*)g_sm1IV, EFLASH_SM1_IV_SIZE);

	return g_u16APDUSW;
}

UINT16 CMD_SM1_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	//按顺序分别为EK、AK、SK、IV
	offset = 0;
	F_Read(EFLASH_SM1_EK_ADDR, (UINT8*)g_sm1EK, EFLASH_SM1_EK_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_sm1EK, 16);
	
	offset += 16;
	F_Read(EFLASH_SM1_AK_ADDR, (UINT8*)g_sm1AK, EFLASH_SM1_AK_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_sm1AK, 16);
	
	offset += 16;
	F_Read(EFLASH_SM1_SK_ADDR, (UINT8*)g_sm1SK, EFLASH_SM1_SK_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_sm1SK, 16);
	
	offset += 16;
	F_Read(EFLASH_SM1_IV_ADDR, (UINT8*)g_sm1IV, EFLASH_SM1_IV_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_sm1IV, 16);

	offset += 16;

	len_response = offset;

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;
	
	return g_u16APDUSW;
}

UINT16 CMD_SM1_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 sksel;
	UINT8 mode;

	if ((g_u16APDULc%16) != 0)
	{
		return SW_LENGTH_ERROR;
	}
	
	if (g_u8APDUP2 == 0)		//SK使用内部密钥
	{
		sksel = 0;
	}
	else		//SK使用外部密钥
	{
		sksel = 1;
	}
	
	if (g_u8APDUP1 == 0)		//ecb模式
	{
		sm1_encrypt_ecb_3key((UINT8*)g_sm1EK, (UINT8*)g_sm1AK, (UINT8*)g_sm1SK, sksel, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else		//cbc模式
	{
		sm1_encrypt_cbc_3key((UINT8*)g_sm1EK, (UINT8*)g_sm1AK, (UINT8*)g_sm1SK, (UINT8*)g_sm1IV, sksel, gp_u8APDUData, apdu_response, g_u16APDULc);
	}

	len_response = g_u16APDULc;	
	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
}

UINT16 CMD_SM1_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 sksel;
	UINT8 mode;

	if ((g_u16APDULc%16) != 0)
	{
		return SW_LENGTH_ERROR;
	}
	
	if (g_u8APDUP2 == 0)		//SK使用内部密钥
	{
		sksel = 0;
	}
	else		//SK使用外部密钥
	{
		sksel = 1;
	}

	if (g_u8APDUP1 == 0)		//ecb模式
	{
		sm1_decrypt_ecb_3key((UINT8*)g_sm1EK, (UINT8*)g_sm1AK, (UINT8*)g_sm1SK, sksel, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else		//cbc模式
	{
		sm1_decrypt_cbc_3key((UINT8*)g_sm1EK, (UINT8*)g_sm1AK, (UINT8*)g_sm1SK, (UINT8*)g_sm1IV, sksel, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	
	len_response = g_u16APDULc;	


	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
	
}

UINT16 CMD_SM2_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT32 sm2keybuf[8];
	UINT8 offset;

	if (g_u8APDUP2 == 0)		// 192bits
	{
		if (g_u16APDULc != SM2_192_LENGTH*12)
		{
			return SW_LENGTH_ERROR;
		}
		g_sm2Bits = 192;
		SM2Init(g_sm2Bits);
		
		//公钥
		offset = 0;
		memcpy((UINT8 *)sm2keybuf, gp_u8APDUData+offset, SM2_192_LENGTH*4);
		wordswap(stuPubKey.stuQx.auValue, sm2keybuf, SM2_192_LENGTH);	//转换成小端
		offset += SM2_192_LENGTH*4;
		memcpy((UINT8 *)sm2keybuf, gp_u8APDUData+offset, SM2_192_LENGTH*4);
		wordswap(stuPubKey.stuQy.auValue, sm2keybuf, SM2_192_LENGTH);	//转换成小端
		
		F_Write(EFLASH_SM2_PUBKEY_192_ADDR, (UINT8*)stuPubKey.stuQx.auValue, SM2_192_LENGTH*4);
		F_Write(EFLASH_SM2_PUBKEY_192_ADDR+32, (UINT8*)stuPubKey.stuQy.auValue, SM2_192_LENGTH*4);
		
		//私钥
		offset += SM2_192_LENGTH*4;
		memcpy((UINT8 *)sm2keybuf, gp_u8APDUData+offset, SM2_192_LENGTH*4);
		wordswap(stuPrivKey.auValue, sm2keybuf, SM2_192_LENGTH);	//转换成小端	

		F_Write(EFLASH_SM2_PRIVKEY_192_ADDR, (UINT8*)stuPrivKey.auValue, SM2_192_LENGTH*4);	
	}
	else		// 256bits
	{
		if (g_u16APDULc != SM2_256_LENGTH*12)
		{
			return SW_LENGTH_ERROR;
		}
		g_sm2Bits = 256;
		SM2Init(g_sm2Bits);
		
		//公钥
		offset = 0;
		memcpy((UINT8 *)sm2keybuf, gp_u8APDUData+offset, SM2_256_LENGTH*4);
		wordswap(stuPubKey.stuQx.auValue, sm2keybuf, SM2_256_LENGTH);	//转换成小端
		offset += SM2_256_LENGTH*4;
		memcpy((UINT8 *)sm2keybuf, gp_u8APDUData+offset, SM2_256_LENGTH*4);
		wordswap(stuPubKey.stuQy.auValue, sm2keybuf, SM2_256_LENGTH);	//转换成小端

		F_Write(EFLASH_SM2_PUBKEY_256_ADDR, (UINT8*)stuPubKey.stuQx.auValue, SM2_256_LENGTH*4);
		F_Write(EFLASH_SM2_PUBKEY_256_ADDR+32, (UINT8*)stuPubKey.stuQy.auValue, SM2_256_LENGTH*4);
		
		//私钥
		offset += SM2_256_LENGTH*4;
		memcpy((UINT8 *)sm2keybuf, gp_u8APDUData+offset, SM2_256_LENGTH*4);
		wordswap(stuPrivKey.auValue, sm2keybuf, SM2_256_LENGTH);	//转换成小端	

		F_Write(EFLASH_SM2_PRIVKEY_256_ADDR, (UINT8*)stuPrivKey.auValue, SM2_256_LENGTH*4);	
	}
	
	return g_u16APDUSW;
}

UINT16 CMD_SM2_GenerateKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT32 sm2keybuf[8];
	UINT8 offset;

	if (g_u8APDUP2 == 0)		//192bits
	{
		g_sm2Bits = 192;
		SM2Init(g_sm2Bits);
	}
	else		//256bits
	{
		g_sm2Bits = 256;
		SM2Init(g_sm2Bits);
	}
	
	sm2_genarate_keypair();

	if (g_u8APDUP2 == 0)		//192bits
	{
		F_Write(EFLASH_SM2_PUBKEY_192_ADDR, (UINT8*)stuPubKey.stuQx.auValue, 24);
		F_Write(EFLASH_SM2_PUBKEY_192_ADDR+32, (UINT8*)stuPubKey.stuQy.auValue, 24);
		F_Write(EFLASH_SM2_PRIVKEY_192_ADDR, (UINT8*)stuPrivKey.auValue, 24);
		//公钥
		offset = 0;
		wordswap(sm2keybuf, stuPubKey.stuQx.auValue, SM2_192_LENGTH);	//转换成大端
		memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_192_LENGTH*4);
		offset += SM2_192_LENGTH*4;
		wordswap(sm2keybuf, stuPubKey.stuQy.auValue, SM2_192_LENGTH);	//转换成大端
		memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_192_LENGTH*4);
		//私钥
		offset += SM2_192_LENGTH*4;
		wordswap(sm2keybuf, stuPrivKey.auValue, SM2_192_LENGTH);	//转换成大端
		memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_192_LENGTH*4);
		offset += SM2_192_LENGTH*4;

		len_response = offset;
	}
	else
	{
		F_Write(EFLASH_SM2_PUBKEY_256_ADDR, (UINT8*)stuPubKey.stuQx.auValue, SM2_256_LENGTH*4);
		F_Write(EFLASH_SM2_PUBKEY_256_ADDR+32, (UINT8*)stuPubKey.stuQy.auValue, SM2_256_LENGTH*4);
		F_Write(EFLASH_SM2_PRIVKEY_256_ADDR, (UINT8*)stuPrivKey.auValue, SM2_256_LENGTH*4);
		//公钥
		offset = 0;
		wordswap(sm2keybuf, stuPubKey.stuQx.auValue, SM2_256_LENGTH);	//转换成大端
		memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_256_LENGTH*4);
		offset += SM2_256_LENGTH*4;
		wordswap(sm2keybuf, stuPubKey.stuQy.auValue, SM2_256_LENGTH);	//转换成大端
		memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_256_LENGTH*4);
		//私钥
		offset += SM2_256_LENGTH*4;
		wordswap(sm2keybuf, stuPrivKey.auValue, SM2_256_LENGTH);	//转换成大端
		memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_256_LENGTH*4);
		offset += SM2_256_LENGTH*4;
		
		len_response = offset;
	}
	
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;
	return g_u16APDUSW;
}

BOOLEAN Read_SM2_Key(UINT8 sm2BitFlg, UINT16 *sm2Bits)
{
	UINT32 eflash_pub_addr;
	UINT32 eflash_priv_addr;
	
	if (sm2BitFlg == 0)		//192位
	{
		*sm2Bits = 192;
		SM2Init(*sm2Bits);
		eflash_pub_addr = EFLASH_SM2_PUBKEY_192_ADDR;
		eflash_priv_addr = EFLASH_SM2_PRIVKEY_192_ADDR;
		
		stuPubKey.stuQx.uLen = SM2_192_LENGTH;
		stuPubKey.stuQy.uLen = SM2_192_LENGTH;
		stuPrivKey.uLen = SM2_192_LENGTH;
		stuSignatureR.uLen = SM2_192_LENGTH;
		stuSignatureS.uLen = SM2_192_LENGTH;	
	}
	else if (sm2BitFlg == 1)		//256位
	{
		*sm2Bits = 256;
		SM2Init(*sm2Bits);
		eflash_pub_addr = EFLASH_SM2_PUBKEY_256_ADDR;
		eflash_priv_addr = EFLASH_SM2_PRIVKEY_256_ADDR;

		stuPubKey.stuQx.uLen = SM2_256_LENGTH;
		stuPubKey.stuQy.uLen = SM2_256_LENGTH;
		stuPrivKey.uLen = SM2_256_LENGTH;
		stuSignatureR.uLen = SM2_256_LENGTH;
		stuSignatureS.uLen = SM2_256_LENGTH;		
	}
	else
	{
		return FALSE;
	}

	F_Read(eflash_pub_addr, (UINT8*)stuPubKey.stuQx.auValue, 32);
	F_Read(eflash_pub_addr+32, (UINT8*)stuPubKey.stuQy.auValue, 32);
	F_Read(eflash_priv_addr, (UINT8*)stuPrivKey.auValue, 32);

	return TRUE;
}

UINT16 CMD_SM2_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT32 sm2keybuf[8];
	UINT8 offset;

	if (Read_SM2_Key(g_u8APDUP2, &g_sm2Bits) == FALSE)
	{
		return SW_P1P2_ERROR;
	}
	
	if (g_u8APDUP1 == 0)		//读取公钥
	{
		if (g_sm2Bits == 192)		//192bits
		{
			offset = 0;
			wordswap(sm2keybuf, stuPubKey.stuQx.auValue, SM2_192_LENGTH);	//转换成大端
			memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_192_LENGTH*4);
			offset += SM2_192_LENGTH*4;
			wordswap(sm2keybuf, stuPubKey.stuQy.auValue, SM2_192_LENGTH);	//转换成大端
			memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_192_LENGTH*4);
			offset += SM2_192_LENGTH*4;
		}
		else		//256bits
		{
			offset = 0;
			wordswap(sm2keybuf, stuPubKey.stuQx.auValue, SM2_256_LENGTH);	//转换成大端
			memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_256_LENGTH*4);
			offset += SM2_256_LENGTH*4;
			wordswap(sm2keybuf, stuPubKey.stuQy.auValue, SM2_256_LENGTH);	//转换成大端
			memcpy(apdu_response+offset, (UINT8 *)sm2keybuf, SM2_256_LENGTH*4);
			offset += SM2_256_LENGTH*4;
		}
	}
	else		//读取私钥
	{
		if (g_sm2Bits == 192)		//192bits
		{
			wordswap(sm2keybuf, stuPrivKey.auValue, SM2_192_LENGTH);	//转换成大端
			memcpy(apdu_response, (UINT8 *)sm2keybuf, SM2_192_LENGTH*4);
			offset = SM2_192_LENGTH*4;
		}
		else		//256bits
		{
			wordswap(sm2keybuf, stuPrivKey.auValue, SM2_256_LENGTH);	//转换成大端
			memcpy(apdu_response, (UINT8 *)sm2keybuf, SM2_256_LENGTH*4);
			offset = SM2_256_LENGTH*4;
		}
	}
	
	len_response = offset;
	
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

//加解密签名验签之前需先执行产生密钥对、设置密钥对或读取密钥对操作
UINT16 CMD_SM2_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	if (g_u8APDUP1 == 0)
	{
		sm2_encrypt_data_v1(gp_u8APDUData, g_u16APDULc*8, apdu_response);
	}
	else if (g_u8APDUP1 == 1)
	{
		sm2_encrypt_data_v2(gp_u8APDUData, g_u16APDULc*8, apdu_response);
	}
	
	if (g_sm2Bits == 192)		//192bits
	{
		len_response = g_u16APDULc+81;
	}
	else		//256bits
	{
		len_response = g_u16APDULc+97;
	}

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

UINT16 CMD_SM2_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN result;

	if (g_sm2Bits == 192)		//192bits
	{
		if (g_u8APDUP1 == 0)
		{
			result = sm2_decrypt_data_v1(gp_u8APDUData, (g_u16APDULc-81)*8, apdu_response);
		}
		else if (g_u8APDUP1 == 1)
		{
			result = sm2_decrypt_data_v2(gp_u8APDUData, (g_u16APDULc-81)*8, apdu_response);
		}
		len_response = g_u16APDULc-81;
	}
	else
	{
		if (g_u8APDUP1 == 0)
		{
			result = sm2_decrypt_data_v1(gp_u8APDUData, (g_u16APDULc-97)*8, apdu_response);
		}
		else if (g_u8APDUP1 == 1)
		{
			result = sm2_decrypt_data_v2(gp_u8APDUData, (g_u16APDULc-97)*8, apdu_response);
		}
		len_response = g_u16APDULc-97;
	}
		
	if (result == 1)		//成功
	{
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	}
	else 	//失败
	{
		g_u16APDUSW = SW_ALGORITHM_ERROR;
	}
	
	return g_u16APDUSW;
}

UINT16 CMD_SM2_SetUserID(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	if (g_u16APDULc > EFLASH_SM2_USERID_SIZE)
	{
		return SW_LENGTH_ERROR;
	}

	g_useridLen = (UINT8)g_u16APDULc;
	memcpy((UINT8 *)g_userid, gp_u8APDUData, g_useridLen);
	F_Write_u8(EFLASH_SM2_USERID_LENGTH_ADDR, g_useridLen);
	F_Write(EFLASH_SM2_USERID_ADDR, (UINT8*)g_userid, EFLASH_SM2_USERID_SIZE);

	return g_u16APDUSW;
}

UINT16 CMD_SM2_ReadUserID(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	g_useridLen = F_Read_u8(EFLASH_SM2_USERID_LENGTH_ADDR);
	F_Read(EFLASH_SM2_USERID_ADDR, (UINT8*)g_userid, g_useridLen);
	memcpy(apdu_response, (UINT8 *)g_userid, EFLASH_SM2_USERID_SIZE);

	len_response = g_useridLen;
	
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

UINT16 CMD_SM2_SignIDA(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	if (g_sm2Bits == 192)		//192bits
	{
		offset = 24;
	}
	else
	{
		offset = 32;
	}
	
	if (g_u8APDUP1 == 0)
	{
		sm2_sign_data(gp_u8APDUData, g_u16APDULc*8, apdu_response, apdu_response+offset);
	}
	else
	{
		sm2_sign_data_IDA(gp_u8APDUData, g_u16APDULc*8, apdu_response, apdu_response+offset);
	}
	
	len_response = offset*2;

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

//原文+签名
UINT16 CMD_SM2_VerifyIDA(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN result;
	UINT8 offset;

	if (g_sm2Bits == 192)		//192bits
	{
		offset = 24;
	}
	else
	{
		offset = 32;
	}
	
	if (g_u8APDUP1 == 0)
	{
		result = sm2_verify_data(gp_u8APDUData, (g_u16APDULc-offset*2)*8, gp_u8APDUData+g_u16APDULc-offset*2, gp_u8APDUData+g_u16APDULc-offset);
	}
	else
	{
		result = sm2_verify_data_IDA(gp_u8APDUData, (g_u16APDULc-offset*2)*8, gp_u8APDUData+g_u16APDULc-offset*2, gp_u8APDUData+g_u16APDULc-offset);
	}
	
	if (result == 0)		//fail
	{
		return 0x9804;	
	}
	else
	{
		return 0x9000;
	}
}

UINT16 CMD_SM2_GetZ(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	memcpy(apdu_response, (UINT8*)g_sm2Za, 32);
	len_response = 32;

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

UINT16 CMD_SM2_GetE(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	memcpy(apdu_response, (UINT8*)g_sm2E, 32);
	len_response = 32;

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

UINT16 CMD_SM2_ExchangeKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN result;

	if (g_u8APDUP2 == 0)		//192bits
	{
		g_sm2Bits = 192;
		SM2Init(g_sm2Bits);
	}
	else		//256bits
	{
		g_sm2Bits = 256;
		SM2Init(g_sm2Bits);
	}
	
	result = sm2_Exchange_Key(apdu_response, &len_response);

	if (result == TRUE)
	{
		memcpy(p_u8Response, apdu_response, len_response);
		*u16ResponseLen = len_response;
		return SW_SUCCESS;
	}
	else
	{
		return SW_ALGORITHM_ERROR;
	}
}

UINT16 CMD_SM3_Hash(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	sm3_hash_data(gp_u8APDUData, g_u16APDULc*8, apdu_response);
	len_response = 32;

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}


UINT16 CMD_SHA_Hash(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 type;
	BOOLEAN result;

	type = g_u8APDUP1;
	
	result = sha_hash_data(type, gp_u8APDUData, g_u16APDULc*8, apdu_response);
	if (result == FALSE)
	{
		return SW_ALGORITHM_ERROR;
	}

	if (type == HASH_SHA0_1)
	{
		len_response = 20;
	}
	else if (type == HASH_SHA1_1)
	{
		len_response = 20;
	}
	else if (type == HASH_SHA224_1)
	{
		len_response = 28;
	}
	else if (type == HASH_SHA256_1)
	{
		len_response = 32;
	}
	else
	{
		return SW_P1P2_ERROR;
	}
	
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

UINT16 CMD_RSA_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 crt;
	UINT8 fixKey;
	UINT8 rsaBits;
	UINT32 eflash_pub_addr;
	UINT32 eflash_priv_addr;

	if (g_u8APDUP2 == 0)		//512位
	{
		eflash_pub_addr = EFLASH_RSA_PUBKEY_512_ADDR;
		eflash_priv_addr = EFLASH_RSA_PRIVKEY_512_ADDR;
	}
	else if (g_u8APDUP2 == 1)		//1024位
	{
		eflash_pub_addr = EFLASH_RSA_PUBKEY_1024_ADDR;
		eflash_priv_addr = EFLASH_RSA_PRIVKEY_1024_ADDR;
	}
	else if (g_u8APDUP2 == 3)		//2048位
	{
		eflash_pub_addr = EFLASH_RSA_PUBKEY_2048_ADDR;
		eflash_priv_addr = EFLASH_RSA_PRIVKEY_2048_ADDR;
	}
	else
	{
		return SW_P1P2_ERROR;
	}
	
	switch(g_u8APDUP1)		//参数类型
	{
		case 0x00:		//N
		{
			if (g_u16APDULc != (g_u8APDUP2*512+512)/8)
			{
				return SW_LENGTH_ERROR;
			}	
			wordswap_u8((UINT8*)mypublicKey.modulus, gp_u8APDUData, g_u16APDULc);	//私钥运算时需设置N值，CRT时N只做判断不做运算，需大于输入值;STD时需要N值
			wordswap_u8((UINT8*)myprivateKey.modulus, gp_u8APDUData, g_u16APDULc);
			//memset((UINT8*)myprivateKey.modulus, 0xFF, g_u16APDULc);		
			break;
		}
		case 0x01:		//E
		{
			if (g_u16APDULc != (g_u8APDUP2*512+512)/8)
			{
				return SW_LENGTH_ERROR;
			}	
			wordswap_u8((UINT8*)mypublicKey.exponent, gp_u8APDUData, g_u16APDULc);
			//wordswap_u8(gp_u8APDUData, (UINT8*)myprivateKey.publicExponent, g_u16APDULc);		
			memset((UINT8*)myprivateKey.publicExponent, 0x00, g_u16APDULc);		//私钥运算时不需要E值
			break;
		}
		case 0x02:		//D
		{
			if (g_u16APDULc != (g_u8APDUP2*512+512)/8)
			{
				return SW_LENGTH_ERROR;
			}	
			wordswap_u8((UINT8*)myprivateKey.exponent, gp_u8APDUData, g_u16APDULc);
			break;
		}
		case 0x03:		//P
		{
			if (g_u16APDULc != ((g_u8APDUP2*512+512)/8)/2)
			{
				return SW_LENGTH_ERROR;
			}	
			wordswap_u8((UINT8*)myprivateKey.prime[0], gp_u8APDUData, g_u16APDULc);
			break;
		}
		case 0x04:		//Q
		{
			if (g_u16APDULc != ((g_u8APDUP2*512+512)/8)/2)
			{
				return SW_LENGTH_ERROR;
			}	
			wordswap_u8((UINT8*)myprivateKey.prime[1], gp_u8APDUData, g_u16APDULc);
			break;
		}
		case 0x05:		//DP
		{
			if (g_u16APDULc != ((g_u8APDUP2*512+512)/8)/2)
			{
				return SW_LENGTH_ERROR;
			}	
			wordswap_u8((UINT8*)myprivateKey.primeExponent[0], gp_u8APDUData, g_u16APDULc);
			break;
		}
		case 0x06:		//DQ
		{
			if (g_u16APDULc != ((g_u8APDUP2*512+512)/8)/2)
			{
				return SW_LENGTH_ERROR;
			}	
			wordswap_u8((UINT8*)myprivateKey.primeExponent[1], gp_u8APDUData, g_u16APDULc);
			break;
		}
		case 0x07:		//Qinv
		{
			if (g_u16APDULc != ((g_u8APDUP2*512+512)/8)/2)
			{
				return SW_LENGTH_ERROR;
			}	
			wordswap_u8((UINT8*)myprivateKey.coefficient, gp_u8APDUData, g_u16APDULc);
			break;
		}
		default:
		{
			return SW_P1P2_ERROR;
		}
	}

	F_Write(eflash_pub_addr, (UINT8*)&mypublicKey, sizeof(STU_RSA_PUBKEY));
	F_Write(eflash_priv_addr, (UINT8*)&myprivateKey, sizeof(STU_RSA_PRIVKEY));			
			
	return g_u16APDUSW;
}

UINT16 CMD_RSA_GenerateKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 rsaCrt;
	UINT8 fixKey;
	UINT8 rsaBitFlg;
	UINT16 rsaBits;
	UINT32 eflash_pub_addr;
	UINT32 eflash_priv_addr;
	UINT16 offset;

	fixKey = g_u8APDUP1;
	if (fixKey>2)
	{
		return SW_P1P2_ERROR;
	}

	rsaCrt = (g_u8APDUP2&0x0F);
	if (rsaCrt > 1)
	{
		return SW_P1P2_ERROR;
	}
	
	rsaBitFlg = (g_u8APDUP2>>4);
	if (rsaBitFlg == 0)
	{
		rsaBits = 512;
		eflash_pub_addr = EFLASH_RSA_PUBKEY_512_ADDR;
		eflash_priv_addr = EFLASH_RSA_PRIVKEY_512_ADDR;
	}
	else if (rsaBitFlg == 1)
	{
		rsaBits = 1024;
		eflash_pub_addr = EFLASH_RSA_PUBKEY_1024_ADDR;
		eflash_priv_addr = EFLASH_RSA_PRIVKEY_1024_ADDR;
	}
	else if (rsaBitFlg == 3)
	{
		rsaBits = 2048;
		eflash_pub_addr = EFLASH_RSA_PUBKEY_2048_ADDR;
		eflash_priv_addr = EFLASH_RSA_PRIVKEY_2048_ADDR;
	}
	else
	{
		return SW_P1P2_ERROR;
	}

	rsa_genarate_keypair(rsaBits, fixKey, rsaCrt);
	F_Write(eflash_pub_addr, (UINT8*)&mypublicKey, sizeof(STU_RSA_PUBKEY));
	F_Write(eflash_priv_addr, (UINT8*)&myprivateKey, sizeof(STU_RSA_PRIVKEY));

	//转换成大端
	offset = 0;
	wordswap_u8(apdu_response+offset, (UINT8*)myprivateKey.modulus, rsaBits/8);		//N
	offset += rsaBits/8;
	wordswap_u8(apdu_response+offset, (UINT8*)myprivateKey.publicExponent, rsaBits/8);		//E
	offset += rsaBits/8;
	wordswap_u8(apdu_response+offset, (UINT8*)myprivateKey.exponent, rsaBits/8);		//D
	offset += rsaBits/8;
	wordswap_u8(apdu_response+offset, (UINT8*)myprivateKey.prime[0], (rsaBits/8)/2);		//P
	offset += (rsaBits/8)/2;
	wordswap_u8(apdu_response+offset, (UINT8*)myprivateKey.prime[1], (rsaBits/8)/2);		//Q
	offset += (rsaBits/8)/2;
	wordswap_u8(apdu_response+offset, (UINT8*)myprivateKey.primeExponent[0], (rsaBits/8)/2);		//DP
	offset += (rsaBits/8)/2;
	wordswap_u8(apdu_response+offset, (UINT8*)myprivateKey.primeExponent[1], (rsaBits/8)/2);		//DQ
	offset += (rsaBits/8)/2;
	wordswap_u8(apdu_response+offset, (UINT8*)myprivateKey.coefficient, (rsaBits/8)/2);		//Qinv
	offset += (rsaBits/8)/2;
	
	len_response = offset;
	
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

BOOLEAN Read_Rsa_Key(UINT8 rsaBitFlg, UINT16 *rsaBits)
{
	UINT32 eflash_pub_addr;
	UINT32 eflash_priv_addr;
	
	if (rsaBitFlg == 0)		//512位
	{
		*rsaBits = 512;
		eflash_pub_addr = EFLASH_RSA_PUBKEY_512_ADDR;
		eflash_priv_addr = EFLASH_RSA_PRIVKEY_512_ADDR;
	}
	else if (rsaBitFlg == 1)		//1024位
	{
		*rsaBits = 1024;
		eflash_pub_addr = EFLASH_RSA_PUBKEY_1024_ADDR;
		eflash_priv_addr = EFLASH_RSA_PRIVKEY_1024_ADDR;
	}
	else if (rsaBitFlg == 3)		//2048位
	{
		*rsaBits = 2048;
		eflash_pub_addr = EFLASH_RSA_PUBKEY_2048_ADDR;
		eflash_priv_addr = EFLASH_RSA_PRIVKEY_2048_ADDR;
	}
	else
	{
		return FALSE;
	}

	F_Read(eflash_pub_addr, (UINT8*)&mypublicKey, sizeof(STU_RSA_PUBKEY));
	F_Read(eflash_priv_addr, (UINT8*)&myprivateKey, sizeof(STU_RSA_PRIVKEY));
	mypublicKey.bits = *rsaBits;
	myprivateKey.bits = *rsaBits;

	return TRUE;
}

UINT16 CMD_RSA_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 crt;
	UINT8 fixKey;
	UINT16 rsaBits;

	if (Read_Rsa_Key(g_u8APDUP2, &rsaBits) == FALSE)
	{
		return SW_P1P2_ERROR;
	}
	
	switch(g_u8APDUP1)		//参数类型
	{
		case 0x00:		//N
		{
			wordswap_u8(apdu_response, (UINT8*)myprivateKey.modulus, rsaBits/8);
			len_response = rsaBits/8;
			break;
		}
		case 0x01:		//E
		{
			wordswap_u8(apdu_response, (UINT8*)mypublicKey.exponent, rsaBits/8);
			len_response = rsaBits/8;
			break;
		}
		case 0x02:		//D
		{
			wordswap_u8(apdu_response, (UINT8*)myprivateKey.exponent, rsaBits/8);
			len_response = rsaBits/8;
			break;
		}
		case 0x03:		//P
		{
			wordswap_u8(apdu_response, (UINT8*)myprivateKey.prime[0], (rsaBits/8)/2);
			len_response = (rsaBits/8)/2;
			break;
		}
		case 0x04:		//Q
		{
			wordswap_u8(apdu_response, (UINT8*)myprivateKey.prime[1], (rsaBits/8)/2);
			len_response = (rsaBits/8)/2;
			break;
		}
		case 0x05:		//DP
		{
			wordswap_u8(apdu_response, (UINT8*)myprivateKey.primeExponent[0], (rsaBits/8)/2);
			len_response = (rsaBits/8)/2;
			break;
		}
		case 0x06:		//DQ
		{
			wordswap_u8(apdu_response, (UINT8*)myprivateKey.primeExponent[1], (rsaBits/8)/2);
			len_response = (rsaBits/8)/2;
			break;
		}
		case 0x07:		//Qinv
		{
			wordswap_u8(apdu_response, (UINT8*)myprivateKey.coefficient, (rsaBits/8)/2);
			len_response = (rsaBits/8)/2;
			break;
		}
		default:
		{
			return SW_P1P2_ERROR;
		}
	}

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}


UINT16 CMD_RSA_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN result;
	UINT16 rsaBits;

	if (Read_Rsa_Key(g_u8APDUP2, &rsaBits) == FALSE)
	{
		return SW_P1P2_ERROR;
	}

	//外部输入为大端格式，内部转换成小端
	result = rsa_encrypt_data(rsaBits, gp_u8APDUData, g_u16APDULc*8, apdu_response);
	if (result == TRUE)		//成功
	{
		len_response = rsaBits/8;

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	}
	else 	//失败
	{
		g_u16APDUSW = SW_ALGORITHM_ERROR;
	}
	
	return g_u16APDUSW;
}

UINT16 CMD_RSA_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN result;
	UINT16 rsaBits;
	UINT8 rsaCrt;

	if (Read_Rsa_Key(g_u8APDUP2, &rsaBits) == FALSE)
	{
		return SW_P1P2_ERROR;
	}

	rsaCrt = g_u8APDUP1;
	
	result = rsa_decrypt_data(rsaBits, rsaCrt, gp_u8APDUData, g_u16APDULc*8, apdu_response);
	if (result == TRUE)		//成功
	{
		len_response = rsaBits/8;

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	}
	else 	//失败
	{
		g_u16APDUSW = SW_ALGORITHM_ERROR;
	}
	
	return g_u16APDUSW;
}

UINT16 CMD_RSA_Sign(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN result;
	UINT16 rsaBits;
	UINT8 rsaCrt;

	if (Read_Rsa_Key(g_u8APDUP2, &rsaBits) == FALSE)
	{
		return SW_P1P2_ERROR;
	}

	rsaCrt = g_u8APDUP1;
	
	result = rsa_sign_data(rsaBits, rsaCrt, gp_u8APDUData, g_u16APDULc*8, apdu_response);

	if (result == TRUE)		//成功
	{
		len_response = rsaBits/8;

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	}
	else 	//失败
	{
		g_u16APDUSW = SW_ALGORITHM_ERROR;
	}

	return g_u16APDUSW;
}

UINT16 CMD_RSA_Verify(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN result;
	UINT16 rsaBits;

	if (Read_Rsa_Key(g_u8APDUP2, &rsaBits) == FALSE)
	{
		return SW_P1P2_ERROR;
	}
	
	result = rsa_verify_data(rsaBits, gp_u8APDUData, g_u16APDULc*8, apdu_response);

	if (result == TRUE)		//成功
	{
		len_response = rsaBits/8;

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	}
	else 	//失败
	{
		g_u16APDUSW = SW_ALGORITHM_ERROR;
	}

	return g_u16APDUSW;
}


UINT16 CMD_DES_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	if (g_u16APDULc != 32)
	{
		return SW_LENGTH_ERROR;
	}
	
	//按顺序分别为KEY(24 byte)、IV(8 byte)
	offset = 0;
	memcpy((UINT8*)g_desKey, gp_u8APDUData+offset, 24);
	F_Write(EFLASH_DES_KEY_ADDR, (UINT8*)g_desKey, EFLASH_DES_KEY_SIZE);

	offset += 24;
	memcpy((UINT8*)g_desIV, gp_u8APDUData+offset, 8);
	F_Write(EFLASH_DES_IV_ADDR, (UINT8*)g_desIV, EFLASH_DES_IV_SIZE);

	return g_u16APDUSW;
}

UINT16 CMD_DES_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	//按顺序分别为KEY(24 byte)、IV(8 byte)
	offset = 0;
	F_Read(EFLASH_DES_KEY_ADDR, (UINT8*)g_desKey, EFLASH_DES_KEY_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_desKey, 24);

	offset += 24;
	F_Read(EFLASH_DES_IV_ADDR, (UINT8*)g_desIV, EFLASH_DES_IV_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_desIV, 8);

	offset += 8;

	len_response = offset;
	
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;
	
	return g_u16APDUSW;
}

UINT16 CMD_DES_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN ret;

	if ((g_u16APDULc%8) != 0)
	{
		return SW_LENGTH_ERROR;
	}
	
	if (g_u8APDUP1 == 0)		//ecb模式
	{
		if (g_u8APDUP2 == 0)		//des
		{
			ret = des_encrypt_ecb((UINT8*)g_desKey, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
		else if (g_u8APDUP2 == 1)		// 3des-2key
		{
			ret = tdes_encrypt_ecb_2key((UINT8*)g_desKey, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
		else			// 3des-3key
		{
			ret = tdes_encrypt_ecb_3key((UINT8*)g_desKey, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
	}
	else			//cbc模式
	{
		if (g_u8APDUP2 == 0)		//des
		{
			ret = des_encrypt_cbc((UINT8*)g_desKey, (UINT8*)g_desIV, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
		else if (g_u8APDUP2 == 1)		// 3des-2key
		{
			ret = tdes_encrypt_cbc_2key((UINT8*)g_desKey, (UINT8*)g_desIV, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
		else			// 3des-3key
		{
			ret = tdes_encrypt_cbc_3key((UINT8*)g_desKey, (UINT8*)g_desIV, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
	}

	if (ret == FALSE)		//error
	{
		return SW_ALGORITHM_ERROR;
	}

	len_response = g_u16APDULc;	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
}

UINT16 CMD_DES_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN ret;

	if ((g_u16APDULc%8) != 0)
	{
		return SW_LENGTH_ERROR;
	}
	
	if (g_u8APDUP1 == 0)		//ecb模式
	{
		if (g_u8APDUP2 == 0)		//des
		{
			ret = des_decrypt_ecb((UINT8*)g_desKey, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
		else if (g_u8APDUP2 == 1)		// 3des-2key
		{
			ret = tdes_decrypt_ecb_2key((UINT8*)g_desKey, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
		else			// 3des-3key
		{
			ret = tdes_decrypt_ecb_3key((UINT8*)g_desKey, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
	}
	else		//cbc模式
	{
		if (g_u8APDUP2 == 0)		//des
		{
			ret = des_decrypt_cbc((UINT8*)g_desKey, (UINT8*)g_desIV, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
		else if (g_u8APDUP2 == 1)		// 3des-2key
		{
			ret = tdes_decrypt_cbc_2key((UINT8*)g_desKey, (UINT8*)g_desIV, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
		else			// 3des-3key
		{
			ret = tdes_decrypt_cbc_3key((UINT8*)g_desKey, (UINT8*)g_desIV, gp_u8APDUData, apdu_response, g_u16APDULc);
		}
	}

	if (ret == FALSE)		//error
	{
		return SW_ALGORITHM_ERROR;
	}

	len_response = g_u16APDULc;	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
}


UINT16 CMD_AES_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	if (g_u16APDULc != 48)
	{
		return SW_LENGTH_ERROR;
	}
	
	//按顺序分别为KEY(32 byte)、IV(16 byte)
	offset = 0;
	memcpy((UINT8*)g_aesKey, gp_u8APDUData+offset, 32);
	F_Write(EFLASH_AES_KEY_ADDR, (UINT8*)g_aesKey, EFLASH_AES_KEY_SIZE);

	offset += 32;
	memcpy((UINT8*)g_aesIV, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_AES_IV_ADDR, (UINT8*)g_aesIV, EFLASH_AES_IV_SIZE);

	return g_u16APDUSW;
}

UINT16 CMD_AES_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	//按顺序分别为KEY(32 byte)、IV(16 byte)
	offset = 0;
	F_Read(EFLASH_AES_KEY_ADDR, (UINT8*)g_aesKey, EFLASH_AES_KEY_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_aesKey, 32);

	offset += 32;
	F_Read(EFLASH_AES_IV_ADDR, (UINT8*)g_aesIV, EFLASH_AES_IV_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_aesIV, 16);

	offset += 16;

	len_response = offset;
	
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}

UINT16 CMD_AES_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 keyMode;
	BOOLEAN ret;

	if ((g_u16APDULc%16) != 0)
	{
		return SW_LENGTH_ERROR;
	}

	keyMode = g_u8APDUP2;		//0为128bits、1为192bits、2为256bits
	if (keyMode > 2)
	{
		return SW_P1P2_ERROR;
	}
	
	if (g_u8APDUP1 == 0)		//ecb模式
	{
		ret = aes_encrypt_ecb((UINT8*)g_aesKey, keyMode, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else if (g_u8APDUP1 == 1)			//ctr模式
	{
		ret = aes_encrypt_ctr((UINT8*)g_aesKey, (UINT8*)g_aesIV, keyMode, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else if (g_u8APDUP1 == 2)		//cbc模式
	{
		ret = aes_encrypt_cbc((UINT8*)g_aesKey, (UINT8*)g_aesIV, keyMode, gp_u8APDUData, apdu_response, g_u16APDULc);
	}

	if (ret == FALSE)		//error
	{
		return SW_ALGORITHM_ERROR;
	}

	len_response = g_u16APDULc;	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
}

UINT16 CMD_AES_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 keyMode;
	BOOLEAN ret;

	if ((g_u16APDULc%8) != 0)
	{
		return SW_LENGTH_ERROR;
	}

	keyMode = g_u8APDUP2;
	
	if (g_u8APDUP1 == 0)		//ecb模式
	{
		ret = aes_decrypt_ecb((UINT8*)g_aesKey, keyMode, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else if (g_u8APDUP1 == 1)		//ctr模式
	{
		ret = aes_decrypt_ctr((UINT8*)g_aesKey, (UINT8*)g_aesIV, keyMode, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else if (g_u8APDUP1 == 2)		//cbc模式
	{
		ret = aes_decrypt_cbc((UINT8*)g_aesKey, (UINT8*)g_aesIV, keyMode, gp_u8APDUData, apdu_response, g_u16APDULc);
	}

	if (ret == FALSE)		//error
	{
		return SW_ALGORITHM_ERROR;
	}
	
	len_response = g_u16APDULc;	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
}


UINT16 CMD_SMS4_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	if (g_u16APDULc != 32)
	{
		return SW_LENGTH_ERROR;
	}
	
	//按顺序分别为KEY(16 byte)、IV(16 byte)
	offset = 0;
	memcpy((UINT8*)g_sms4Key, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_SMS4_KEY_ADDR, (UINT8*)g_sms4Key, EFLASH_SMS4_KEY_SIZE);

	offset += 16;
	memcpy((UINT8*)g_sms4IV, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_SMS4_IV_ADDR, (UINT8*)g_sms4IV, EFLASH_SMS4_IV_SIZE);

	return g_u16APDUSW;
}


UINT16 CMD_SMS4_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	//按顺序分别为KEY(16 byte)、IV(16 byte)
	offset = 0;
	F_Read(EFLASH_SMS4_KEY_ADDR, (UINT8*)g_sms4Key, EFLASH_SMS4_KEY_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_sms4Key, 16);

	offset += 16;
	F_Read(EFLASH_SMS4_IV_ADDR, (UINT8*)g_sms4IV, EFLASH_SMS4_IV_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_sms4IV, 16);

	offset += 16;

	len_response = offset;
	
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;
}


UINT16 CMD_SMS4_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN ret;

	if ((g_u16APDULc%16) != 0)
	{
		return SW_LENGTH_ERROR;
	}
	
	if (g_u8APDUP1 == 0)		//ecb模式
	{
		ret = sms4_encrypt_ecb((UINT8*)g_sms4Key, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else		//cbc模式
	{
		ret = sms4_encrypt_cbc((UINT8*)g_sms4Key, (UINT8*)g_sms4IV, gp_u8APDUData, apdu_response, g_u16APDULc);
	}

	if (ret == FALSE)		//error
	{
		return SW_ALGORITHM_ERROR;
	}

	len_response = g_u16APDULc;	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
}


UINT16 CMD_SMS4_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	BOOLEAN ret;

	if ((g_u16APDULc%16) != 0)
	{
		return SW_LENGTH_ERROR;
	}
	
	if (g_u8APDUP1 == 0)		//ecb模式
	{
		ret = sms4_decrypt_ecb((UINT8*)g_sms4Key, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else		//cbc模式
	{
		ret = sms4_decrypt_cbc((UINT8*)g_sms4Key, (UINT8*)g_sms4IV, gp_u8APDUData, apdu_response, g_u16APDULc);
	}

	if (ret == FALSE)		//error
	{
		return SW_ALGORITHM_ERROR;
	}

	len_response = g_u16APDULc;	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
}


UINT16 CMD_SSF33_SetKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	if (g_u16APDULc != 48)
	{
		return SW_LENGTH_ERROR;
	}
	
	//按顺序分别为MK、SK、IV
	offset = 0;
	memcpy((UINT8*)g_ssf33MK, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_SSF33_MK_ADDR, (UINT8*)g_ssf33MK, EFLASH_SSF33_MK_SIZE);
	
	offset += 16;
	memcpy((UINT8*)g_ssf33SK, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_SSF33_SK_ADDR, (UINT8*)g_ssf33SK, EFLASH_SSF33_SK_SIZE);
	
	offset += 16;
	memcpy((UINT8*)g_ssf33IV, gp_u8APDUData+offset, 16);
	F_Write(EFLASH_SSF33_IV_ADDR, (UINT8*)g_ssf33IV, EFLASH_SSF33_IV_SIZE);

	return g_u16APDUSW;
}

UINT16 CMD_SSF33_ReadKey(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 offset;

	//按顺序分别为MK、SK、IV
	offset = 0;
	F_Read(EFLASH_SSF33_MK_ADDR, (UINT8*)g_ssf33MK, EFLASH_SSF33_MK_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_ssf33MK, 16);
	
	offset += 16;
	F_Read(EFLASH_SSF33_SK_ADDR, (UINT8*)g_ssf33SK, EFLASH_SSF33_SK_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_ssf33SK, 16);
	
	offset += 16;
	F_Read(EFLASH_SSF33_IV_ADDR, (UINT8*)g_ssf33IV, EFLASH_SSF33_IV_SIZE);
	memcpy(apdu_response+offset, (UINT8*)g_ssf33IV, 16);

	offset += 16;

	len_response = offset;
	
	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;
	
	return g_u16APDUSW;
}


UINT16 CMD_SSF33_Encrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 sksel;
	UINT8 mode;

	if ((g_u16APDULc%16) != 0)
	{
		return SW_LENGTH_ERROR;
	}
	
	if (g_u8APDUP2 == 0)		//SK使用内部密钥
	{
		sksel = 0;
	}
	else		//SK使用外部密钥
	{
		sksel = 1;
	}
	
	if (g_u8APDUP1 == 0)		//ecb模式
	{
		ssf33_encrypt_ecb_2key((UINT8*)g_ssf33MK, (UINT8*)g_ssf33SK, sksel, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else		//cbc模式
	{
		ssf33_encrypt_cbc_2key((UINT8*)g_ssf33MK, (UINT8*)g_ssf33SK, (UINT8*)g_ssf33IV, sksel, gp_u8APDUData, apdu_response, g_u16APDULc);
	}

	len_response = g_u16APDULc;	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
}


UINT16 CMD_SSF33_Decrypt(UINT8 *p_u8APDU, UINT8 *p_u8Response, UINT16 *u16ResponseLen)
{
	UINT8 sksel;
	UINT8 mode;

	if ((g_u16APDULc%16) != 0)
	{
		return SW_LENGTH_ERROR;
	}
	
	if (g_u8APDUP2 == 0)		//SK使用内部密钥
	{
		sksel = 0;
	}
	else		//SK使用外部密钥
	{
		sksel = 1;
	}

	if (g_u8APDUP1 == 0)		//ecb模式
	{
		ssf33_decrypt_ecb_2key((UINT8*)g_ssf33MK, (UINT8*)g_ssf33SK, sksel, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	else		//cbc模式
	{
		ssf33_decrypt_cbc_2key((UINT8*)g_ssf33MK, (UINT8*)g_ssf33SK, (UINT8*)g_ssf33IV, sksel, gp_u8APDUData, apdu_response, g_u16APDULc);
	}
	
	len_response = g_u16APDULc;	

	memcpy(p_u8Response, apdu_response, len_response);

	*u16ResponseLen = len_response;

	return g_u16APDUSW;	
	
}



//初始化设备认证密钥
unsigned short APP_CMD_KeyInit(void)
{
	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.LC!=0x11)
		return SW_LCLE_ERROR;
	
	return cos_init_dev_authkey(g_APDU.pInData);	 
}
//结束初始化
unsigned short APP_CMD_EndInit(void)
{
	g_APDU.RLE=0;
		
	return cos_end_dev_authkey();	 
}
//设置介质标号
unsigned short APP_CMD_SetKeyNum(void)
{
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1)
		return SW_P1P2_ERROR;

	if(g_APDU.PP2==0)
	{
		if(g_APDU.LC!=0x20)
			return SW_LCLE_ERROR;
		return cos_set_keynum(0,g_APDU.pInData);
	}

	else if(g_APDU.PP2==1)
	{
		if(g_APDU.LC!=0x80)
			return SW_LCLE_ERROR;
		return cos_set_keynum(1,g_APDU.pInData);
	}
}


//设备认证
unsigned short APP_CMD_DevAuth(void)
{	
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1&&g_APDU.PP2!=2)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x10)
		return SW_LCLE_ERROR;
	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;
	g_randstate=0;
	
	return cos_dev_auth(g_APDU.pInData,g_rand,g_APDU.PP2);
}

//修改认证密钥
unsigned short APP_CMD_ChangeDevPin(void)
{	
	unsigned char rand[16]={0};
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1&&g_APDU.PP2!=2)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x24)
		return SW_LCLE_ERROR;
	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;
	g_randstate=0;

	memcpy(rand,g_rand,8);
	
	return cos_changedevkey((unsigned char *)&g_APDU.CLA,rand,g_APDU.PP2);
}

//获取PIN信息
unsigned short APP_CMD_GetPinInfo(void)
{	
	unsigned short ret;
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)
	{
		 g_APDU.RLE=3;
		return cos_getpininfo(g_APDU.pOutData,g_APDU.PP2);
	}

	return ret;
}

unsigned short APP_CMD_BlockPin(void)
{	
	unsigned short ret;
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)
	{
		return cos_blockuserpin();
	}
	return ret;
}
unsigned short APP_CMD_ClearSecureState(void)
{	
	unsigned short ret;
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)
	{
		return cos_clearsecurestate();
	}
	return ret;
}
unsigned short APP_CMD_VeryfyPin(void)
{
	unsigned char fid[2];
	unsigned short ret;
	 g_APDU.RLE=0;

	if(g_APDU.PP1!=0&&g_APDU.PP1!=1)
		return SW_P1P2_ERROR;
	if(g_APDU.LC!=0x12)
		return SW_LCLE_ERROR;

	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;	
	g_randstate=0;

	ret=cos_open_app(g_APDU.pInData,0);
	
	if(ret==SW_SUCCESS)		
	{
		return cos_veryfypin(g_APDU.pInData+2,g_rand,g_APDU.PP1,g_APDU.PP2);
	}	
	return ret;
}

unsigned short APP_CMD_ChangePin(void)
{	
	unsigned short ret;
	unsigned char rand[16]={0};
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0&&g_APDU.PP1!=1)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1&&g_APDU.PP2!=2)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x26)
		return SW_LCLE_ERROR;
	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;
	g_randstate=0;

	memcpy(rand,g_rand,8);

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)
		return ret;
	
	return cos_changepin((unsigned char *)&g_APDU.CLA,rand,g_APDU.PP1,g_APDU.PP2);
}

unsigned short APP_CMD_UnlockPin(void)
{	
	unsigned short ret;
	unsigned char rand[16]={0};
	 g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0&&g_APDU.PP1!=0x10)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1&&g_APDU.PP2!=2)
		return SW_P1P2_ERROR;
	
	if(g_APDU.LC!=0x26&&g_APDU.LC!=0x06)
		return SW_LCLE_ERROR;
	if(g_randstate==0)
		return SW_REFERENCEDATA_INVALID;
	g_randstate=0;

	memcpy(rand,g_rand,8);

	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)
		return ret;
	
	return cos_unblock_userpin((unsigned char *)&g_APDU.CLA,rand,USER_PIN,g_APDU.PP2);
}


unsigned short APP_CMD_GenRand(void)
{
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;

	if(g_APDU.LC==0)
	{
		g_APDU.LC=243;
	}

	
	#if ALG_DEBUG
		memcpy(g_APDU.pOutData,"\x01\x01\x01\x01\x01\x01\x01\x01",8);
	#else
	hal_alg_genrand(g_APDU.pOutData,g_APDU.LC);
	#endif
	memcpy(g_rand,g_APDU.pOutData,8);
	g_randstate=1;
	
	g_APDU.RLE=g_APDU.LC;
	
	return SW_SUCCESS;	 
}



unsigned short APP_CMD_ReadTerminalInfo(void)
{
	unsigned short len=0;
	unsigned char *pout;
	MANUFACTURER_INFO *p_info;
	DEV_AUTHKEY_INFO  *p_authkey;
	unsigned int total_size,rest_size;
	unsigned char buffer[16];

	g_APDU.RLE=0;

	pout=g_APDU.pOutData;
	p_info=(MANUFACTURER_INFO*)DEV_VER_INFO;
	p_authkey=(DEV_AUTHKEY_INFO*)DEV_AUTH_KEY;
	
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;

	if(g_APDU.PP2==0)
	{
		if(g_APDU.LC!=0x7b)
			return SW_LCLE_ERROR;
		
		memcpy(pout,p_info->hardware_ver,96);//厂商信息+序列号
		pout+=96-16;//设备序列号由芯片序列号转换


		cos_get_devicenum(pout);
		pout+=16;

		
	#if ALG_DEBUG
		memcpy(pout,"\xb9\xf3\x83\x57\x48\xbb\x08\x42\x48\xf2\x45\xe8\x46\x01\x6f\x87",16);
		//memcpy(pout,"\x20\x17\x04\x07\x48\xf2\x4c\x39\x04\x24\x86\x88\x31\x35\x31\x00",16);
	#else
		ReadSn(buffer);
		memcpy(pout,buffer,16);//芯片序列号

	#endif
		pout+=16;

		cos_getsizeinfo(&total_size,&rest_size);
		memcpy(pout,(unsigned char*)&total_size,4);//COS总空间
		pout+=4;
		
		memcpy(pout,(unsigned char*)&rest_size,4);//COS总空间
		pout+=4;

		*pout++=p_authkey->retry_counter;
		*pout++=0X04;
		*pout++=0X00;
		
		g_APDU.RLE=0x7b;
	}
	else if(g_APDU.PP2==1)//获取介质标号
	{
		if(g_APDU.LC!=0x20)
			return SW_LCLE_ERROR;
		return cos_get_keynum(0,g_APDU.pOutData,&g_APDU.RLE);
		
	}
	else if(g_APDU.PP2==2)
	{
		if(g_APDU.LC!=0x80)
			return SW_LCLE_ERROR;
		return cos_get_keynum(1,g_APDU.pOutData,&g_APDU.RLE);
	}
	else if(g_APDU.PP2==3)
	{
		if(g_APDU.LC!=0x02)
			return SW_LCLE_ERROR;
		cos_version(g_APDU.pOutData);
		g_APDU.RLE=2;
	}
	else if(g_APDU.PP2==4)
	{
		if(g_APDU.LC!=0x01)
			return SW_LCLE_ERROR;
		g_APDU.pOutData[0]=cos_getsecurestate();
		g_APDU.RLE=1;
	}
	else if(g_APDU.PP2==5)
	{
		if(g_APDU.LC!=0x01)
			return SW_LCLE_ERROR;
		g_APDU.pOutData[0]=cos_get_usage();
		g_APDU.RLE=1;
	}
	else if(g_APDU.PP2==0xAA)//获取介质标号
	{	
		if(g_APDU.LC!=0x40)
			return SW_LCLE_ERROR;
		
		memcpy(g_APDU.pOutData,p_info->hardware_ver,64);//厂商信息+序列号		
		g_APDU.RLE=64;
	}
	else 
		return SW_P1P2_ERROR;
	
	return SW_SUCCESS;	 
}



//创建应用的同时创建PIN文件
unsigned short APP_CMD_CreatAPP(void)
{
	unsigned short file_id,ret,file_size;
	CREATE_APP *p_create_app=(CREATE_APP*)g_APDU.pInData;
	unsigned char auth[3];

	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.LC!=0x45)
		return SW_LCLE_ERROR;

	file_id=p_create_app->fid[0]<<8+p_create_app->fid[1];

	//配置应用权限
	auth[0]=AUTH_FREE;//读权限自由
	auth[1]=p_create_app->auth;//应用下创建文件权限
	auth[2]=AUTH_DEV;//删除应用需要设备权限
#if 0	
	if(FID_MF==file_id)
	{
		ret=cos_check_MF();
		if(SW_MF_NOTEXIST==ret)
		{
			ret=cos_creat_app(p_create_app->fid,0x0f,p_create_app->auth,p_create_app->name,p_create_app->admin_pin);
			UART_printf("file data\r\n");
			HAL_UART_SEND_ASC((unsigned char *)MFSTART,512);			
		}
		return ret;
	}
	else
#endif
	{
		ret=cos_check_MF();
		if(SW_SUCCESS==ret)
		{
			ret=cos_check_auth(F_TPYE_MF,AUTH_WRITE_LOC);
			if(SW_SUCCESS==ret)
			{
				ret=cos_check_FIDok(p_create_app->fid,F_TPYE_DF);
				if(SW_SUCCESS==ret)
				{
					ret=cos_creat_app(p_create_app->fid,auth,p_create_app->name,p_create_app->admin_pin);
				}			
			}
		}

		return ret;
	}
}        

unsigned short APP_CMD_OpenAPP(void)
{
	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;

	if(g_APDU.PP2==0&&g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;
	else if(g_APDU.PP2==1&&g_APDU.LC!=0x20)
		return SW_LCLE_ERROR;
	return cos_open_app(g_APDU.pInData,g_APDU.PP2);
}

unsigned short APP_CMD_CloseAPP(void)
{
	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;

	if(g_APDU.PP2==0&&g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;
	else if(g_APDU.PP2==1&&g_APDU.LC!=0x20)
		return SW_LCLE_ERROR;
	return cos_close_app(g_APDU.pInData,g_APDU.PP2);
}

unsigned short APP_CMD_DeleteAPP(void)
{
	unsigned short ret,file_size;
	unsigned char app_id[2];
	CREATE_FILE *p_create_file=(CREATE_FILE*)g_APDU.pInData;

	g_APDU.RLE=0;
		
	if(g_APDU.LC!=0x2&&g_APDU.LC!=0X20)
		return SW_LCLE_ERROR;

	ret=cos_open_app(g_APDU.pInData,g_APDU.PP2);
	if(ret==SW_SUCCESS)		
	{		

		if(cos_getsecurestate()==SECURE_DEV)
		{
			ret=cos_delete_app();
		}
		else 
			return SW_SECURITY_DISSATISFY;
			
		
	}	
	return ret;
	
}

unsigned short APP_CMD_EnumAPP(void)
{
	g_APDU.RLE=0;
		
	if(g_APDU.PP1!=0||g_APDU.PP2!=0x00)
		return SW_P1P2_ERROR;
	if(g_APDU.LC!=0)
		return SW_LCLE_ERROR;
	
	return cos_enum_app(g_APDU.pOutData,&g_APDU.RLE);
}

unsigned short APP_CMD_CreatFile(void)
{
	unsigned short ret,file_size;
	unsigned char app_id[2];
	CREATE_FILE *p_create_file=(CREATE_FILE*)g_APDU.pInData;

	g_APDU.RLE=0;
		
	if(g_APDU.LC!=0x28)
		return SW_LCLE_ERROR;

	app_id[0]=g_APDU.PP1;
	app_id[1]=g_APDU.PP2;
	ret=cos_open_app(app_id,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_check_auth(F_TPYE_DF,AUTH_WRITE_LOC);
		if(SW_SUCCESS==ret)
		{
			ret=cos_check_FIDok(p_create_file->fid,F_TYPE_EF);
			if(SW_SUCCESS==ret){
				return cos_creat_file(p_create_file);
			}
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_DeleteFile(void)
{
	unsigned short ret,file_size;
	unsigned char app_id[2];
	CREATE_FILE *p_create_file=(CREATE_FILE*)g_APDU.pInData;

	g_APDU.RLE=0;
		
	if(g_APDU.LC!=0x2)
		return SW_LCLE_ERROR;

	app_id[0]=g_APDU.PP1;
	app_id[1]=g_APDU.PP2;
	ret=cos_open_app(app_id,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_open_file(g_APDU.pInData,0);
		if(SW_SUCCESS==ret)
		{
			ret=cos_check_auth(F_TYPE_EF,AUTH_DELETE_LOC);
			if(SW_SUCCESS==ret)
				ret=cos_delete_file();
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_WriteFile(void)
{
	unsigned short ret,file_len,offset;
	unsigned char app_id[2],fid[2];
	CREATE_FILE *p_create_file=(CREATE_FILE*)g_APDU.pInData;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;
		
	if(g_APDU.LC<0x08)
		return SW_LCLE_ERROR;
	
	offset=(g_APDU.pInData[4]<<8)+g_APDU.pInData[5];	
	file_len=(g_APDU.pInData[6]<<8)+g_APDU.pInData[7];	

	if(g_APDU.LC!=0x08+file_len)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_open_file(g_APDU.pInData+2,0);
		if(SW_SUCCESS==ret)
		{
			ret=cos_check_auth(F_TYPE_EF,AUTH_WRITE_LOC);
			if(SW_SUCCESS==ret)
				return cos_write_file(offset,g_APDU.pInData+8,file_len);
		}
	}	
	return ret;
	
}
unsigned short APP_CMD_GetFileInfo(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2!=0&&g_APDU.PP2!=1)
		return SW_P1P2_ERROR;	
	
	ret=cos_open_app(g_APDU.pInData,g_APDU.PP2);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_open_file(g_APDU.pInData+2,g_APDU.PP2);
		if(SW_SUCCESS==ret)
		{	
			//ret=cos_check_auth(F_TYPE_EF,AUTH_READ_LOC);
			//if(SW_SUCCESS==ret)
			{
				ret=cos_get_fileinfo(g_APDU.pOutData,&g_APDU.RLE);
			}
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_ReadFile(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x08)
		return SW_LCLE_ERROR;

	offset=(g_APDU.pInData[4]<<8)+g_APDU.pInData[5];	
	file_len=(g_APDU.pInData[6]<<8)+g_APDU.pInData[7];	
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_open_file(g_APDU.pInData+2,g_APDU.PP2);
		if(SW_SUCCESS==ret)
		{	
			ret=cos_check_auth(F_TYPE_EF,AUTH_READ_LOC);
			if(SW_SUCCESS==ret)
			{
				ret=cos_read_file(g_APDU.pOutData,offset,file_len);
				g_APDU.RLE=file_len;
			}
		}
	}	
	return ret;
	
}
unsigned short APP_CMD_EnumFile(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0&&g_APDU.PP1!=1)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x02)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{
		ret=cos_enum_file(g_APDU.pOutData,&g_APDU.RLE,g_APDU.PP1,g_APDU.PP2);
	}	
	return ret;
	
}
//-----------------------秘钥运算指令-----------------
unsigned short APP_CMD_Gen1024RSA(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x06)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_genrsakey(g_APDU.pInData+2,g_APDU.pInData+4,g_APDU.pOutData,ASYALG_RSA1024);	
		if(ret==SW_SUCCESS)
		{
			g_APDU.RLE=0x80;
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_Gen2048RSA(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x06)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_genrsakey(g_APDU.pInData+2,g_APDU.pInData+4,g_APDU.pOutData,ASYALG_RSA2048);	
		if(ret==SW_SUCCESS)
		{
			g_APDU.RLE=0x100;
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_1024RSAEncrypt_Verify(void)
{
	unsigned short ret,file_len,offset;
	unsigned char flag,fid[2];
	g_APDU.RLE=0;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;
		if(g_APDU.LC!=0x102)
			return SW_LCLE_ERROR;
		
	}
	else
	{
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
		flag=0;
		if(g_APDU.LC!=0x82)
			return SW_LCLE_ERROR;
	}
		
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		if(flag==1)//使用外部公钥
		{
			ret=cos_rsa1024_encrypt_verify(g_APDU.pInData+2+0x80,g_APDU.pInData+2,g_APDU.pOutData,flag);	
		}
		else if(flag==0)//使用内部文件
		{
			ret=cos_rsa1024_encrypt_verify(fid,g_APDU.pInData+2,g_APDU.pOutData,flag);	
		}	
		if(ret==SW_SUCCESS)
			g_APDU.RLE=0x80;
	}	
	return ret;
	
}
unsigned short APP_CMD_2048RSAEncrypt_Verify(void)
{
	unsigned short ret,file_len,offset;
	unsigned char flag,fid[2];
	g_APDU.RLE=0;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;
		if(g_APDU.LC!=0x202)
			return SW_LCLE_ERROR;
		
	}
	else
	{
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
		flag=0;
		if(g_APDU.LC!=0x102)
			return SW_LCLE_ERROR;
	}
		
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		if(flag==1)//使用外部公钥
		{
			ret=cos_rsa2048_encrypt_verify(g_APDU.pInData+2+0x100,g_APDU.pInData+2,g_APDU.pOutData,flag);	
		}
		else if(flag==0)//使用内部文件
		{
			ret=cos_rsa2048_encrypt_verify(fid,g_APDU.pInData+2,g_APDU.pOutData,flag);	
		}	
		if(ret==SW_SUCCESS)
			g_APDU.RLE=0x100;
	}	
	return ret;
	
}

unsigned short APP_CMD_1024RSADecrypt_Sign(void)
{
	unsigned short ret,file_len,offset;
	unsigned char flag,fid[2];
	g_APDU.RLE=0;

	if(g_APDU.LC!=0x82)
		return SW_LCLE_ERROR;
	
	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;	
		
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		ret=cos_rsa1024_decrypt_sign(fid,g_APDU.pInData+2,g_APDU.pOutData);
		if(ret==SW_SUCCESS)
			g_APDU.RLE=0x80;
	}	
	return ret;
	
}
unsigned short APP_CMD_2048RSADecrypt_Sign(void)
{
	unsigned short ret,file_len,offset;
	unsigned char flag,fid[2];
	g_APDU.RLE=0;

	if(g_APDU.LC!=0x102)
		return SW_LCLE_ERROR;
	
	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;	
		
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		ret=cos_rsa2048_decrypt_sign(fid,g_APDU.pInData+2,g_APDU.pOutData);
		if(ret==SW_SUCCESS)
			g_APDU.RLE=0x100;
	}	
	return ret;
	
}
unsigned short APP_CMD_RSAExportKey(void)
{
	unsigned short ret,len;
	unsigned char rsatype;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x87&&g_APDU.LC!=0x107)
		return SW_LCLE_ERROR;

	len=(g_APDU.pInData[5]<<8)+g_APDU.pInData[6];//模长
	
	if(len==0x80)
	{
		rsatype=ASYALG_RSA1024;
	}
	else if(len==0x100)
	{
		rsatype=ASYALG_RSA2048;
	}
	else
		return SW_REFERENCEDATA_INVALID;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	

	g_session_key.state=0XAA;

	g_session_key.keyid[0]=g_APDU.pInData[2];
	g_session_key.keyid[1]=g_APDU.pInData[3];

	g_session_key.type=g_APDU.pInData[4];

	hal_alg_genrand(g_session_key.key,16);
#if ALG_DEBUG
	memcpy(g_session_key.key,"\xD3\xB3\x31\xFD\x0E\x43\xF9\x55\x40\xA5\x00\x03\xA6\x1F\xC0\x57",16);
#endif
	return cos_rsa_exportkey(g_APDU.pInData+7,g_session_key.key,rsatype,g_APDU.pOutData,&g_APDU.RLE);	
	
}

unsigned short APP_CMD_ExportRSAkeyA(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x04)
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_rsa_exportpubkey(g_APDU.pInData+2,g_APDU.pOutData,&g_APDU.RLE);	
	
}
unsigned short APP_CMD_ImportRSAkeyA(void)
{
	unsigned short ret;
	unsigned char flag;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC==0x86)
	{
		flag=ASYALG_RSA1024;
	}
	else if(g_APDU.LC==0x106)
	{
		flag=ASYALG_RSA2048;
	}
	else
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_rsa_importpubkey(g_APDU.pInData+2,g_APDU.pInData+6,flag);	
	
}
unsigned short APP_CMD_ImportRSAkeyB(void)
{
	unsigned short ret;
	unsigned char flag;
	unsigned short len;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;

	len=(g_APDU.pInData[6]<<8)+g_APDU.pInData[7];//模长
	
	if(len==0x400)
	{
		flag=ASYALG_RSA1024;
	}
	else if(len==0x800)
	{
		flag=ASYALG_RSA2048;
	}
	else
		return SW_REFERENCEDATA_INVALID;

	if(g_session_key.state!=0xaa)
		return SW_SECURITY_DISSATISFY;

	len=g_APDU.LC-8;
		
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_rsa_import_enc_prikey(g_APDU.pInData+2,g_session_key.key,
						g_session_key.type,g_APDU.pInData+8,len,flag);	
	
}
unsigned short APP_CMD_ImportRSAkeyB1(void)
{
	unsigned short ret;
	unsigned char flag;
	unsigned short len;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;

	len=(g_APDU.pInData[4]<<8)+g_APDU.pInData[5];//模长
	
	if(len==0x400)
	{
		flag=ASYALG_RSA1024;
	}
	else if(len==0x800)
	{
		flag=ASYALG_RSA2048;
	}
	else
		return SW_REFERENCEDATA_INVALID;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_rsa_importprikey(g_APDU.pInData+2,g_APDU.pInData+6,flag);	
	
}

unsigned short APP_CMD_GenSM2Key(void)
{
	unsigned short ret,file_len,offset;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x06)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_genSM2key(g_APDU.pInData+2,g_APDU.pInData+4,g_APDU.pOutData,&g_APDU.RLE);				
	}	
	return ret;
	
}
unsigned short APP_CMD_SM2Encrypt(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;
	unsigned int len;

	g_APDU.RLE=0;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;
		len=g_APDU.LC-66;	
	}
	else
	{
		len=g_APDU.LC-2;
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
	}
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		if(flag==1)
		{
			ret=cos_SM2encrypt(g_APDU.pInData+2+len,g_APDU.pInData+2,len,g_APDU.pOutData,flag);	
		}
		else if(flag==0)//使用内部文件
		{
			ret=cos_SM2encrypt(fid,g_APDU.pInData+2,len,g_APDU.pOutData,flag);	
		}
		if(ret==SW_SUCCESS)
			g_APDU.RLE=len+96;
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2Decrypt(void)
{
	unsigned short ret;
	unsigned char fid[2];
	unsigned int len;

	g_APDU.RLE=0;

	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;	

	len=g_APDU.LC-2;
		
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		ret=cos_SM2decrypt(fid,g_APDU.pInData+2,len,g_APDU.pOutData);	
		if(ret==SW_SUCCESS)
			g_APDU.RLE=len-96;
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2Sign(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;

	g_APDU.RLE=0;

	if(g_APDU.LC!=0x22)
		return SW_LCLE_ERROR;

	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;	
		
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{	
		ret=cos_SM2sign(fid,g_APDU.pInData+2,g_APDU.pOutData);
		if(ret==SW_SUCCESS)
			g_APDU.RLE=64;
	}	
	return ret;	
}

unsigned short APP_CMD_SM2Verify(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;
	unsigned int len;

	g_APDU.RLE=0;

	if(g_APDU.LC!=0x62&&g_APDU.LC!=0xA2)
		return SW_LCLE_ERROR;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;		
	}
	else
	{
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
	}
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		if(flag==1)
		{
			ret=cos_SM2verify(g_APDU.pInData+2,g_APDU.pInData+2+64,g_APDU.pInData+2+64+32,flag);	
		}
		else if(flag==0)//使用内部文件
		{
			ret=cos_SM2verify(fid,g_APDU.pInData+2,g_APDU.pInData+2+32,flag);	
		}			
	}	
	return ret;
	
}

unsigned short APP_CMD_ExportSM2keyA(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x04)
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_SM2_exportkeyA(g_APDU.pInData+2,g_APDU.pOutData,&g_APDU.RLE);	
	
}
unsigned short APP_CMD_ImportSM2KeyA(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x44)
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_SM2_importpubkey(g_APDU.pInData+2,g_APDU.pInData+4);	
	
}

unsigned short APP_CMD_ImportSM2KeyB(void)
{
	unsigned short ret,len;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	

//	if(g_session_key.state!=0xaa)
//		return SW_SECURITY_DISSATISFY;

	//len=(g_APDU.pInData[6]<<8)+g_APDU.pInData[7];

	//len=len/8;
	len=g_APDU.LC-8;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_SM2_import_enc_prikey(g_APDU.pInData+2,g_session_key.key,g_session_key.type,g_APDU.pInData+8,len);	
	
}

unsigned short APP_CMD_ImportSM2KeyB1(void)
{
	unsigned short ret;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x26)
		return SW_LCLE_ERROR;

	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	
	
	return cos_SM2_importprikey(g_APDU.pInData+2,g_APDU.pInData+6);	
	
}

unsigned short APP_CMD_SM2GenZ(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;
	unsigned int len;

	g_APDU.RLE=0;

	if(g_APDU.PP1==0&&g_APDU.PP2==0)//使用外部公钥
	{
		flag=1;		
	}
	else
	{
		fid[0]=g_APDU.PP1;
		fid[1]=g_APDU.PP2;
	}
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret==SW_SUCCESS)		
	{		
		if(flag==1)//使用外部公钥
		{
			cos_SM2genZ(g_APDU.pInData+2+1+g_APDU.pInData[2],g_APDU.pInData[2],g_APDU.pInData+3,g_APDU.pOutData,flag);
		}
		else if(flag==0)//使用内部文件
		{
			cos_SM2genZ(fid,g_APDU.pInData[2],g_APDU.pInData+3,g_APDU.pOutData,flag);
		}			
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2GenKeyA(void)
{
	unsigned short ret;
	unsigned char fid[2], pubid[2],priid[2];
	unsigned char idlen,oidlen,*pid,*poid,*pother_pub,*pother_tmppub,*pin;

	g_APDU.RLE=0;

	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;

	pin=g_APDU.pInData;

	memcpy(pubid,pin,2);//己方公钥
	pin+=2;
	memcpy(priid,pin,2);//己方私钥
	pin+=2;

	memcpy(g_session_key.keyid,pin,2);//会话密钥ID
	pin+=2;
	g_session_key.type=*pin;//会话密钥类型
	pin+=1;

	pother_pub=pin;//对方公钥
	pin+=64;
	
	pother_tmppub=pin;//对方临时公钥
	pin+=64;

	oidlen=*pin;
	pin+=1;

	poid=pin;
	pin+=oidlen;

	idlen=*pin;
	pin+=1;

	pid=pin;
	pin+=idlen;
	
	ret=cos_open_app(fid,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_SM2genkeya(pubid,priid,idlen,pid,pother_pub,pother_tmppub,oidlen,poid,g_APDU.pOutData,g_session_key.key,0);
		if(ret==SW_SUCCESS)
		{
			g_session_key.state=0xaa;
			g_APDU.RLE=64;
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2GenKeyB(void)
{
	unsigned short ret;
	unsigned char fid[2], pubid[2],priid[2];
	unsigned char idlen,oidlen,*pid,*poid,*pother_pub,*pother_tmppub,*pin;

	g_APDU.RLE=0;

	fid[0]=g_APDU.PP1;
	fid[1]=g_APDU.PP2;

	pin=g_APDU.pInData;

	memcpy(pubid,pin,2);//己方公钥
	pin+=2;
	memcpy(priid,pin,2);//己方私钥
	pin+=2;

	memcpy(g_session_key.keyid,pin,2);//会话密钥ID
	pin+=2;
	g_session_key.type=*pin;//会话密钥类型
	pin+=1;

	pother_pub=pin;//对方公钥
	pin+=64;
	
	pother_tmppub=pin;//对方临时公钥
	pin+=64;

	oidlen=*pin;
	pin+=1;

	poid=pin;
	pin+=oidlen;

	idlen=*pin;
	pin+=1;

	pid=pin;
	pin+=idlen;
	
	ret=cos_open_app(fid,0);
	if(ret==SW_SUCCESS)		
	{		
		ret=cos_SM2genkeya(pubid,priid,idlen,pid,pother_pub,pother_tmppub,oidlen,poid,g_APDU.pOutData,g_session_key.key,1);
		if(ret==SW_SUCCESS)
		{
			g_session_key.state=0xaa;
			g_APDU.RLE=64;
		}
	}	
	return ret;
	
}

unsigned short APP_CMD_SM2ExportKey(void)
{
	unsigned short ret,len;
	unsigned char rsatype;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x44)
		return SW_LCLE_ERROR;
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	

	if(g_session_key.state!=0XAA)
		return SW_DATA_NOTFOUND;
	if(g_session_key.keyid[0]!=g_APDU.pInData[2]||g_session_key.keyid[1]!=g_APDU.pInData[3])
		return SW_DATA_NOTFOUND;;
	
	return cos_SM2_exportkey(g_APDU.pInData+4,g_session_key.key,g_APDU.pOutData,&g_APDU.RLE);	
	
}

unsigned short APP_CMD_ImportKey(void)
{
	unsigned short ret,len;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	

	len=g_APDU.LC-7;

	memcpy(g_session_key.keyid,g_APDU.pInData+4,2);//会话密钥ID
	g_session_key.type=g_APDU.pInData[6];//会话密钥类型
	
	return cos_importkey(g_APDU.pInData+2,g_APDU.pInData+7,len,g_session_key.key);	
	
}

unsigned short APP_CMD_DestoryKey(void)
{
	unsigned short ret;
	unsigned char keyid[2];

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0||g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	
	ret=cos_open_app(g_APDU.pInData,0);
	if(ret!=SW_SUCCESS)		
		return ret;	

	memset((unsigned char *)g_session_key.state,0,sizeof(SESSION_KEY));

	return SW_SUCCESS;
}
//---------------------------------------------------------------------------
unsigned short APP_CMD_Hash(void)
{
	unsigned short ret;
	unsigned char type,len;

	g_APDU.RLE=0;

	if(g_APDU.PP2==HASH_SM3||g_APDU.PP2==HASH_SHA256)
	{
		len=32;
	}
	else if(g_APDU.PP2==HASH_SHA1)
	{
		len=20;
	}
	else if(g_APDU.PP2==HASH_SHA224)
	{
		len=28;
	}
	else
		return SW_CONDITION_DISSATISFY;
	
	type=g_APDU.PP2;

	if(g_APDU.PP1==0)
	{
		hal_alg_hashinit(type);
		hal_alg_hashupdate(g_APDU.pInData,g_APDU.LC);
		hal_alg_hashfinal(g_APDU.pOutData);
	}
	else if(g_APDU.PP1==1)
	{
		hal_alg_hashinit(type);
		hal_alg_hashupdate(g_APDU.pInData,g_APDU.LC);
		len=0;
	}
	else if(g_APDU.PP1==2)
	{
		hal_alg_hashupdate(g_APDU.pInData,g_APDU.LC);
		len=0;
	}
	else if(g_APDU.PP1==3)
	{
		hal_alg_hashfinal(g_APDU.pOutData);
	}

	else
		SW_P1P2_ERROR;
	
	g_APDU.RLE=len;
	
	return SW_SUCCESS;
	
}



//---------------------------------------------------------------------------
CRYPT_DATA g_crypt;

unsigned short APP_CMD_CryptInit(void)
{
	unsigned short ret;
	unsigned char fid[2], flag=0;
	unsigned int len;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x10)
		return SW_LCLE_ERROR;

	if(g_APDU.PP2==ALG_SM4||g_APDU.PP2==ALG_SSF33||g_APDU.PP2==ALG_SM1)
	{
		g_crypt.type=g_APDU.PP2;
		memcpy(g_crypt.key,g_APDU.pInData,16);
		g_crypt.key_sta=0xaa;
	}
	else if(g_APDU.PP2==0x10)
	{
		memcpy(g_crypt.iv,g_APDU.pInData,16);
		g_crypt.iv_sta=0xaa;
	}
	else
		return SW_P1P2_ERROR;
	
	return SW_SUCCESS;
	
}

unsigned short APP_CMD_Encrypt(void)
{
	unsigned short ret;
	unsigned char  cbc_flag;
	unsigned char buf_in[1024],buf_out[1024],key[16],iv[16],alg_type=0;
	g_APDU.RLE=0;

	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2==0)
	{
		cbc_flag=SYMALG_ECB;
	}
	else if(g_APDU.PP2==1)
	{
		cbc_flag=SYMALG_CBC;
		if(g_crypt.iv_sta!=0xaa)
		return SW_CONDITION_DISSATISFY;	
	}
	else
		SW_P1P2_ERROR;

	if(g_APDU.LC%16)
		return SW_LCLE_ERROR;

	if(g_crypt.key_sta!=0xaa)
		return SW_DATA_NOTFOUND;
	
	memcpy(buf_in,g_APDU.pInData,g_APDU.LC);
	memcpy(key,g_crypt.key,16);
	memcpy(iv,g_crypt.iv,16);
	alg_type=g_crypt.type;
	hal_alg_bcae(buf_in,
				g_APDU.LC, 
				key, 
				iv, 
				SYMALG_SENC|cbc_flag, 
				buf_out,
				alg_type);
	memcpy(g_APDU.pOutData,buf_out,g_APDU.LC);
	g_APDU.RLE=g_APDU.LC;
	
	return SW_SUCCESS;
	
}

unsigned short APP_CMD_Decrypt(void)
{
	unsigned short ret;
	unsigned char cbc_flag;
	unsigned char buf[1024],key[16],iv[16],alg_type=0;

	g_APDU.RLE=0;

	if(g_APDU.PP1!=0)
		return SW_P1P2_ERROR;
	if(g_APDU.PP2==0)
	{
			cbc_flag=SYMALG_ECB;

	}
	else if(g_APDU.PP2==1)
	{
		cbc_flag=SYMALG_CBC;
		if(g_crypt.iv_sta!=0xaa)
		return SW_CONDITION_DISSATISFY;
	}
	else
		SW_P1P2_ERROR;

	if(g_APDU.LC%16)
		return SW_LCLE_ERROR;

	if(g_crypt.key_sta!=0xaa)
		return SW_DATA_NOTFOUND;

	memcpy(buf,g_APDU.pInData,g_APDU.LC);
	memcpy(key,g_crypt.key,16);
	memcpy(iv,g_crypt.iv,16);
	alg_type=g_crypt.type;
	hal_alg_bcae(buf,
				g_APDU.LC, 
				key, 
				iv, 
				SYMALG_SDEC|cbc_flag, 
				buf,
				alg_type);
	memcpy(g_APDU.pOutData,buf,g_APDU.LC);
	g_APDU.RLE=g_APDU.LC;
	
	return SW_SUCCESS;
	
}
//---------------------------------------------------------------------------

unsigned short APP_CMD_DevFormat(void)
{
	g_APDU.RLE=0;

	if(g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	
	if(g_APDU.PP1==0)
	{
		if(g_APDU.LC!=0x60)
			return SW_LCLE_ERROR;
		
		return cos_dev_format(g_APDU.pInData);
	}
	else if(g_APDU.PP1==1)//读设备序列号
	{
		cos_get_devicenum(g_APDU.pOutData);
		g_APDU.RLE=16;
		return SW_SUCCESS;
	}
	else if(g_APDU.PP1==2)//读版本号
	{
		memcpy((unsigned char*)g_APDU.pOutData,(unsigned char*)app_ver,strlen((char *)app_ver));
		g_APDU.RLE=strlen((char *)app_ver);
		return SW_SUCCESS;
	}	
	else
		return SW_P1P2_ERROR;	
}


unsigned short APP_CMD_FactoryCMD(void)
{
	unsigned char buf[16],buf1[16],iv[16];
	unsigned char key[16]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	//unsigned char bufdata[]={0x8b,0x65,0x1a,0x89,0xa7,0x44,0x24,0x41,0x0d,0x0c,0x23,0xe3,0xea,0x4d,0x16,0xec};
	g_APDU.RLE=0;

	if(g_APDU.PP2!=0)
		return SW_P1P2_ERROR;	
	if(g_APDU.LC!=0x10)
		return SW_LCLE_ERROR;

	memset(iv,0,16);

	hal_3des_oper(g_APDU.pInData,16,key,iv,SYMALG_ECB|SYMALG_SDEC,buf);
	
	cos_get_devicenum(buf1);
	if(memcmp(buf,buf1,16))
		return SW_REFERENCEDATA_INVALID;
	
	if(g_APDU.PP1==0)
	{
		g_reboot=1;
	}
	else if(g_APDU.PP1==1)
	{	
		flash_device.erasepage(DEV_VER_INFO);
		flash_device.erasepage(DEV_COS_INIT);
		g_reboot=3;
	}		
	else
		return SW_P1P2_ERROR;	

	return SW_SUCCESS;
}



//unsigned short APP_CMD_SerialNO(void)
//{
//	g_APDU.RLE=0;

//	if(g_APDU.PP2!=0)
//		return SW_P1P2_ERROR;	
//	if(g_APDU.LC!=0x10)
//		return SW_LCLE_ERROR;

//	if(g_APDU.PP1==0)
//	{
//		return cos_set_devicenum(g_APDU.pInData);
//	}
//	else if(g_APDU.PP1==1)
//	{
//		g_APDU.RLE=0x10;
//		return cos_get_devicenum(g_APDU.pOutData);
//	}
//		
//	else
//		return SW_P1P2_ERROR;	
//}


unsigned short APP_CMD_ReturnBoot(void)
{	
	unsigned char password[6];


	memset(password,0x31,6);
	if(memcmp(g_APDU.pInData, password, 6)==0)	
	{
		//return boot
		EnBootMode();
		return SW_SUCCESS;

	}
}
unsigned short APP_CMD_Erase_key_number(void)
{
	unsigned char password[6];
	U8 *p,status,pbin[33]={0xaa},pbout[33];
	U8 i=0;
	memset(password,0x31,6);
	if(memcmp(g_APDU.pInData, password, 6)==0)	
	{
		//erase KeyNum	
		p=(U8 *)DEV_VER_INFO;
		p+=sizeof(MANUFACTURER_INFO)+sizeof(SERIAL_NUMBER_INFO);
		flash_device.erasepage((U32)p);

		p=(U8 *)DEV_VER_INFO;
		p+=sizeof(MANUFACTURER_INFO)+sizeof(SERIAL_NUMBER_INFO);
		flash_device.readdata((U32)p,pbout,33);
		HAL_UART_SEND_ASC(pbout,33);

		//erase cos
		for(i=1;i<TOTAL_DATA_PAGE;i++)//清除所有数据区数据
				{
					flash_device.erasepage(DATASTART+i*FLASH_PAGE_SIZE);
				}
		//初始化主扇区表
		flash_device.erasepage(PRI_SECTOR_INFO);

	}
	return SW_SUCCESS;

}
unsigned short APP_CMD_get_data(void)
{	
	unsigned char buffer[16]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0x00,0x11,0x22,0x33,0x44,0x55,0x66};	


	F_Write(0x00003805,buffer,16);
	flash_device.readdata(0x00003805,buffer,16);

	memcpy(g_APDU.pOutData,buffer,16);
	g_APDU.RLE=0x10;

	//Chip_Reset(1);
		
	return SW_SUCCESS;
}


//---------------------------------------------------------------------------
CMD_TABLE key_cmd[]={
	{0x0050,APP_CMD_DevAuth},
	{0x0051,APP_CMD_VeryfyPin},
	{0x0084,APP_CMD_GenRand},
	{0x8011,APP_CMD_EndInit},
	{0x8020,APP_CMD_CreatAPP},
	{0x8022,APP_CMD_EnumAPP},
	{0x8026,APP_CMD_OpenAPP},
	{0x8027,APP_CMD_CloseAPP},
	{0x8028,APP_CMD_DeleteAPP},
	{0x8031,APP_CMD_CryptInit},
	{0x8032,APP_CMD_Encrypt},
	{0x8033,APP_CMD_Decrypt},
	{0x8046,APP_CMD_Gen1024RSA},
	{0x8047,APP_CMD_Gen2048RSA},
	{0x8052,APP_CMD_GetPinInfo},
	{0x8056,APP_CMD_BlockPin},
	{0x805C,APP_CMD_ClearSecureState},
	{0x8068,APP_CMD_SetKeyNum},
	{0x8086,APP_CMD_1024RSADecrypt_Sign},
	{0x8087,APP_CMD_2048RSADecrypt_Sign},
	{0x808A,APP_CMD_1024RSAEncrypt_Verify},
	{0x808B,APP_CMD_2048RSAEncrypt_Verify},
	{0x80A0,APP_CMD_ImportKey},
	{0x80A1,APP_CMD_RSAExportKey},
	{0x80A2,APP_CMD_SM2ExportKey},
	{0x80A3,APP_CMD_DestoryKey},
	{0x80C1,APP_CMD_ExportSM2keyA},
	{0x80CD,APP_CMD_ImportSM2KeyA},
	{0x80CB,APP_CMD_ImportSM2KeyB1},
	{0x80CA,APP_CMD_ReadTerminalInfo},
	{0x80E1,APP_CMD_CreatFile},
	{0x80E2,APP_CMD_DeleteFile},
	{0x80E3,APP_CMD_EnumFile},
	{0x80E4,APP_CMD_GetFileInfo},
	{0x80B0,APP_CMD_ReadFile},
	{0x80B6,APP_CMD_WriteFile},

	{0x80F0,APP_CMD_ExportRSAkeyA},
	{0x80F2,APP_CMD_ImportRSAkeyA},
	{0x80F3,APP_CMD_ImportRSAkeyB},	
	{0x80F4,APP_CMD_ImportRSAkeyB1},
	{0x845D,APP_CMD_ChangeDevPin},
	{0x845E,APP_CMD_ChangePin},
	{0x845F,APP_CMD_UnlockPin},
	{0x88CE,APP_CMD_GenSM2Key},
	{0x88C1,APP_CMD_ExportSM2keyA},
	{0x88C2,APP_CMD_SM2GenKeyA},
	{0x88C3,APP_CMD_SM2Sign},
	{0x88C4,APP_CMD_SM2Verify},
	{0x88C5,APP_CMD_SM2Decrypt},
	{0x88C6,APP_CMD_SM2Encrypt},
	{0x88C8,APP_CMD_SM2GenKeyB},
	{0x88CF,APP_CMD_ImportSM2KeyB},
	{0x88CB,APP_CMD_ImportSM2KeyB1},
	{0x80CC,APP_CMD_Hash},
	{0x88CD,APP_CMD_ImportSM2KeyA},
	{0x9010,APP_CMD_KeyInit},

	//自定义指令
	{0x5A00,APP_CMD_DevFormat},
	{0x5A01,APP_CMD_FactoryCMD},
	{0x5A03,APP_CMD_ReturnBoot},
	{0x5A04,APP_CMD_Erase_key_number},
	{0x5A05,APP_CMD_get_data},
};




void Command()
{
	unsigned short  cmd,i,ret;
	unsigned char flag=1;
	
	g_APDU.RLE=0;
	
	cmd=(g_APDU.CLA<<8)+g_APDU.INS;
	 for(i = 0; i < sizeof(key_cmd)/sizeof(key_cmd[0]); i++)
	{
		if(cmd== key_cmd[i].cmd)
		{
			ret= key_cmd[i].function();
			
			flag=0;		
			break;
		}
	}
	 if(flag)
	 	ret=0x6d00;
	
	g_APDU.SW1 = ret>>8;
	g_APDU.SW2 = ret&0x00ff;
	return;		//2011.12.20	 		
}


