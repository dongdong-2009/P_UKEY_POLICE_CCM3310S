#define  APDU_INSTRUCTION_SYMBOL
#include "includes.h"

APDU g_APDU;
const INS_Attribute Command_APDU_INS_ROM[]={
#if 0
	//注: INS不能为0x00
	//随机数
	{0x00, 0x10, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_GetRandom},
	
	//RSA
	{0x00, 0x11, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_RSA_SetKey},
	{0x00, 0x12, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_RSA_GenerateKey},
	{0x00, 0x13, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_RSA_ReadKey},
	{0x00, 0x14, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_RSA_Encrypt},
	{0x00, 0x15, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_RSA_Decrypt},
	{0x00, 0x16, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_RSA_Sign},
	{0x00, 0x17, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_RSA_Verify},

	//DES
	{0x00, 0x20, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_DES_SetKey},
	{0x00, 0x21, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_DES_ReadKey},
	{0x00, 0x22, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_DES_Encrypt},
	{0x00, 0x23, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_DES_Decrypt},

	//AES
	{0x00, 0x30, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_AES_SetKey},
	{0x00, 0x31, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_AES_ReadKey},
	{0x00, 0x32, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_AES_Encrypt},
	{0x00, 0x33, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_AES_Decrypt},
	
	//SM1
	{0x00, 0x60, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM1_SetKey},
	{0x00, 0x61, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM1_ReadKey},
	{0x00, 0x62, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM1_Encrypt},
	{0x00, 0x63, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM1_Decrypt},
	
	//SM2
	{0x00, 0x80, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_SetKey},
	{0x00, 0x81, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_GenerateKey},
	{0x00, 0x82, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_ReadKey},
	{0x00, 0x83, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_Encrypt},
	{0x00, 0x84, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_Decrypt},
	{0x00, 0x85, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_SetUserID},
	{0x00, 0x86, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_ReadUserID},
	{0x00, 0x87, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_SignIDA},
	{0x00, 0x88, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_VerifyIDA},
	{0x00, 0x89, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_GetZ},
	{0x00, 0x8A, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_GetE},
	{0x00, 0x8D, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM2_ExchangeKey},
	
	//SM3
	{0x00, 0x90, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SM3_Hash},
	{0x00, 0x91, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SHA_Hash},

	//SMS4
	{0x00, 0xB0, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SMS4_SetKey},
	{0x00, 0xB1, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SMS4_ReadKey},
	{0x00, 0xB2, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SMS4_Encrypt},
	{0x00, 0xB3, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SMS4_Decrypt},

	//SSF33
	{0x00, 0xD0, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SSF33_SetKey},
	{0x00, 0xD1, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SSF33_ReadKey},
	{0x00, 0xD2, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SSF33_Encrypt},
	{0x00, 0xD3, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_SSF33_Decrypt},

	//Read SN
	{0x00, 0xF0, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, CMD_Read_SN},		//TODO:不能使用F0，why?
	//go boot
	{0x00, 0xFF, LIFE_CYCLE_CONTROL_FF, AUTH_CONTROL_FF, EnableBootMode},

#endif
	{0x00, 0x00, 0x00, 0x00, 0x00},			// 该行请勿删除 !!! 该行请勿删除 !!! 该行请勿删除 !!!
};
	
BOOLEAN Find_Instruction(UINT8 *p_u8APDU, UINT16 *p_Index)
{
	INS_Attribute *byAPDU_Describe;
	
	byAPDU_Describe=(INS_Attribute *)(&Command_APDU_INS_ROM[0]);

	*p_Index=0;
	while ((*byAPDU_Describe).u8INS)
	{
		if ( ((*byAPDU_Describe).u8CLA == p_u8APDU[0x00]) && ( (*byAPDU_Describe).u8INS == p_u8APDU[0x01]) )
		{
			return TRUE;
		}

		(*p_Index)++;
		byAPDU_Describe++;
	}
	
	return FALSE;
}


UINT16 Exec_Instruction(UINT16 u16Index, UINT8 *p_u8APDU, UINT16 u16APDULen, UINT8 *p_u8Response, UINT16 *p_u16LeLength)
{
	g_u16APDULen = u16APDULen;
	g_u16APDUSW = SW_SUCCESS;
	g_u8APDUP1 = p_u8APDU[2];
	g_u8APDUP2 = p_u8APDU[3];
	g_u16APDULc = 0;
	g_u16APDULe = 0;

	if (g_u16APDULen == 4)			//case 1
	{
		g_u16APDUCase = 1;
	}
	else if (g_u16APDULen == 5)		//case 2S
	{
		g_u16APDULe = p_u8APDU[4];
		g_u16APDUCase = 2;
	}
	else if (g_u16APDULen == (5+p_u8APDU[4]))		//case 3S
	{
		g_u16APDULc = p_u8APDU[4];
		gp_u8APDUData = p_u8APDU+5;
		g_u16APDUCase = 3;
	}
	else if (g_u16APDULen == (6+p_u8APDU[4]))		//case 4S
	{
		g_u16APDULc = p_u8APDU[4];
		g_u16APDULe = p_u8APDU[g_u16APDULen-1];
		gp_u8APDUData = p_u8APDU+5;
		g_u16APDUCase = 4;
	}
	else if ((g_u16APDULen == 7) && (p_u8APDU[4] == 0))		//case 2E
	{
		g_u16APDULe = (p_u8APDU[5]<<8)+p_u8APDU[6];
		g_u16APDUCase = 2;
	}
	else if ((g_u16APDULen == (7+(p_u8APDU[5]<<8)+p_u8APDU[6])) && (p_u8APDU[4] == 0))		//case 3E
	{
		g_u16APDULc = (p_u8APDU[5]<<8)+p_u8APDU[6];	
		gp_u8APDUData = p_u8APDU+7;
		g_u16APDUCase = 3;
	}
	else if ((g_u16APDULen == (9+(p_u8APDU[5]<<8)+p_u8APDU[6])) && (p_u8APDU[4] == 0))		//case 4E
	{
		g_u16APDULc = (p_u8APDU[5]<<8)+p_u8APDU[6];
		g_u16APDULe = (p_u8APDU[g_u16APDULen-1]<<8)+p_u8APDU[g_u16APDULen-2];
		gp_u8APDUData = p_u8APDU+7;
		g_u16APDUCase = 4;
	}
	else
	{
		return SW_LENGTH_ERROR;
	}	

	if ((p_u8APDU[0x00]&0x10) == 0x10)		//命令链中非最后的一条指令
	{
		g_u16APDUChain = 1;
	}
	else		//单条指令或此命令为命令链的最后一条指令
	{
		g_u16APDUChain = 0;
	}

#ifndef USB_CCID_CLASS		//CCID协议需要通过GetResponse命令取数据，所以不能清0
	memset(apdu_response, 0x00, 2048);
#endif

	return (Command_APDU_INS_ROM[u16Index].INS_Address)(p_u8APDU, p_u8Response, p_u16LeLength);
}

UINT8 GetCardLifeCycle(void)
{
	return g_u8LifeCycle;
}

BOOLEAN Security(UINT16 unIndex)
{
	UINT8 u8LifeCycle;	
	UINT8 u8InsLifeCycleStatus;
	UINT8 u8AC;
	/*
	u8InsLifeCycleStatus = Command_APDU_INS_ROM[unIndex].u8Life_Period;
	if(u8InsLifeCycleStatus != LIFE_CYCLE_CONTROL_FF)		
	{
		u8LifeCycle = GetCardLifeCycle();
		if (u8LifeCycle != u8InsLifeCycleStatus)		//当前生命周期与命令的生命周期不符，直接返回
		{
			return FALSE;
		}
	}
	
	u8AC = Command_APDU_INS_ROM[unIndex].u8AuthFlag;
	if (u8AC != AUTH_CONTROL_FF)
	{
		if ((g_u8AuthFlg&u8AC) != u8AC)					//当前认证状态与命令的认证状态不符，直接返回
		{
			return FALSE;
		}
	}
	*/
	return TRUE;
}



UINT8 APDUprocess(UINT8 *p_u8APDU, UINT16 u16APDULen, UINT8 *p_u8Response, UINT16 *p_u16RespLen)
{
	UINT16 responseLen;
	UINT16 u16Index;
	UINT16 u16SW1SW2;

	u16SW1SW2 = SW_SUCCESS;
	responseLen = 0;
	
	if (Find_Instruction(p_u8APDU, &u16Index))
	{
		if (Security(u16Index))
		{
			u16SW1SW2=Exec_Instruction(u16Index, p_u8APDU, u16APDULen, p_u8Response, &responseLen);
		}
		else
		{
			u16SW1SW2 = 0x6985;
		}
	}
	else
	{
		u16SW1SW2 = 0x6D00;
	}

	p_u8Response[responseLen++] = (UINT8)(u16SW1SW2>>8);
	p_u8Response[responseLen++] = (UINT8)u16SW1SW2;
	*p_u16RespLen = responseLen;

	g_APDU_LEN = 0;
}


UINT8 APDUprocess1(UINT8 *p_u8APDU, UINT16 u16APDULen, UINT8 *p_u8Response, UINT16 *p_u16RespLen)
{
		unsigned short slength = 0;
		slength = 0;
		slength =  (gpram_cbwPacket[23] +(gpram_cbwPacket[22] << 8)) ;
	
		g_APDU.CLA = g_APDU_BUF[0];
		g_APDU.INS = g_APDU_BUF[1];
		g_APDU.PP1 = g_APDU_BUF[2];
		g_APDU.PP2 = g_APDU_BUF[3];
		g_APDU.LEN[0] = g_APDU_BUF[4];
		g_APDU.LEN[1] = g_APDU_BUF[5];

		if(slength==4)
		{
			g_APDU.LC =0;
		}
		else if(slength==5)
		{
			g_APDU.LC =0;

		}
		else if(slength==6)
		{
			g_APDU.LC=(g_APDU_BUF[4]<<8)+g_APDU_BUF[5];
		}
		else 
		{
			g_APDU.LC = slength - 6;	
			memcpy(g_APDU.pInData, g_APDU_BUF+6, g_APDU.LC);
		}
				
		Command();


		memcpy(p_u8Response,g_APDU.pOutData, g_APDU.RLE );
		*(p_u8Response+g_APDU.RLE) = g_APDU.SW1;
		*(p_u8Response+g_APDU.RLE+1) = g_APDU.SW2;
	
		
		*p_u16RespLen = g_APDU.RLE+2;


		memset(g_APDU_BUF, 0, COMMUNICATION_BUF_LEN);		
		g_APDU_LEN = 0;


}


