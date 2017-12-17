							/*All rights reserved */
/*********************************************************************
Copyright (C), 2010-2013, C*Core Technology Co., Ltd
File name: algorithm.c
Author:        Version:        Date: 20121230
Description:  应用命令的处理
Others: 
History:                 
  1. Date:
      Author:
      Modification:
  2. ...
**********************************************************************/

#define  ALGORITHM_SYMBOL
#include "includes.h"

/**************************SM2变量定义*******************************/
//椭圆曲线参数
//192bits
const UINT8 charGx_192[] = "4AD5F7048DE709AD51236DE65E4D4B482C836DC6E4106640";
const UINT8 charGy_192[] = "02BB3A02D4AAADACAE24817A4CA3A1B014B5270432DB27D2";
const UINT8 charPrimeN_192[] = "BDB6F4FE3E8B1D9E0DA8C0D40FC962195DFAE76F56564677";
const UINT8 charPrimeP_192[] = "BDB6F4FE3E8B1D9E0DA8C0D46F4C318CEFE4AFE3B6B8551F";
const UINT8 charCoefficientA_192[] = "BB8E5E8FBC115E139FE6A814FE48AAA6F0ADA1AA5DF91985";
const UINT8 charCoefficientB_192[] = "1854BEBDC31B21B7AEFC80AB0ECD10D5B1B3308E6DBF11C1";

//256bits
const UINT8 charGx_256[]           = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
const UINT8 charGy_256[]           = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";
const UINT8 charPrimeN_256[]       = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";
const UINT8 charPrimeP_256[]       = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
const UINT8 charCoefficientA_256[] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
const UINT8 charCoefficientB_256[] = "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";


/***********************************************************************/

void Genarate_Random(UINT8* random, UINT32 len)
{
	UINT32 i;
	UINT32 result;
	UINT8 temp[4];

	for (i=0; i<len/4; i++)
	{
		result = GetRandomWord();	
		*(random+i*4) = (UINT8)(result>>24);
		*(random+i*4+1) = (UINT8)(result>>16);
		*(random+i*4+2) = (UINT8)(result>>8);
		*(random+i*4+3) = (UINT8)(result);
	}

	if ((len%4) != 0)
	{
		result = GetRandomWord();	
		*(temp) = (UINT8)(result>>24);
		*(temp+1) = (UINT8)(result>>16);
		*(temp+2) = (UINT8)(result>>8);
		*(temp+3) = (UINT8)(result);
		memcpy(random+i*4, temp, (len%4));
	}
}

BOOLEAN sm1_encrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 sm1EK[4];
	UINT32 sm1AK[4];
	UINT32 sm1SK[4];
	UINT32 sm1IV[4];
	
	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sm1EK, key, 16);
	memset((UINT8*)sm1AK, 0x00, 16);
	memset((UINT8*)sm1SK, 0x00, 16);
	memset((UINT8*)sm1IV, 0x00, 16);

	memcpy((UINT8*)g_algorithm_buf1, plain, length);
	SM1_EnDecrypt(SM1_MODE_EN,SM1_MODE_ECB, 0,sm1EK,sm1AK,sm1SK,
				sm1IV,g_algorithm_buf1,  g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

BOOLEAN sm1_decrypt_ecb(UINT8 *key, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 sm1EK[4];
	UINT32 sm1AK[4];
	UINT32 sm1SK[4];
	UINT32 sm1IV[4];
	
	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sm1EK, key, 16);
	memset((UINT8*)sm1AK, 0x00, 16);
	memset((UINT8*)sm1SK, 0x00, 16);
	memset((UINT8*)sm1IV, 0x00, 16);

	memcpy((UINT8*)g_algorithm_buf1, cipher, length);
	SM1_EnDecrypt(SM1_MODE_DE,SM1_MODE_ECB, 0,sm1EK,sm1AK,sm1SK,
				sm1IV,g_algorithm_buf1,  g_algorithm_buf1, length);
	memcpy(plain, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

BOOLEAN sm1_encrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 sm1EK[4];
	UINT32 sm1AK[4];
	UINT32 sm1SK[4];
	UINT32 sm1IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sm1EK, key, 16);
	memcpy((UINT8 *)sm1IV, iv, 16);
	memset((UINT8*)sm1AK, 0x00, 16);
	memset((UINT8*)sm1SK, 0x00, 16);

	memcpy((UINT8*)g_algorithm_buf1, plain, length);
	SM1_EnDecrypt(SM1_MODE_EN,SM1_MODE_CBC, 0,sm1EK,sm1AK,sm1SK,
				sm1IV,g_algorithm_buf1,  g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

BOOLEAN sm1_decrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 sm1EK[4];
	UINT32 sm1AK[4];
	UINT32 sm1SK[4];
	UINT32 sm1IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sm1EK, key, 16);
	memcpy((UINT8 *)sm1IV, iv, 16);
	memset((UINT8*)sm1AK, 0x00, 16);
	memset((UINT8*)sm1SK, 0x00, 16);

	memcpy((UINT8*)g_algorithm_buf1, cipher, length);
	SM1_EnDecrypt(SM1_MODE_DE,SM1_MODE_CBC, 0,sm1EK,sm1AK,sm1SK,
				sm1IV,g_algorithm_buf1,  g_algorithm_buf1, length);
	memcpy(plain, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

BOOLEAN sm1_encrypt_ecb_3key(UINT8 *Ekey, UINT8 *Akey, UINT8 *Skey, UINT8 sksel, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 sm1EK[4];
	UINT32 sm1AK[4];
	UINT32 sm1SK[4];
	UINT32 sm1IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sm1EK, Ekey, 16);
	memcpy((UINT8 *)sm1AK, Akey, 16);
	if (sksel == 1)	//SK使用外部输入
	{
		memcpy((UINT8 *)sm1SK, Skey, 16);
	}
	else
	{
		memset((UINT8*)sm1SK, 0x00, 16);
	}
	memset((UINT8*)sm1IV, 0x00, 16);
	
	memcpy((UINT8*)g_algorithm_buf1, plain, length);	
	SM1_EnDecrypt(SM1_MODE_EN,SM1_MODE_ECB, sksel, sm1EK,sm1AK,sm1SK,
				sm1IV,g_algorithm_buf1,  g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

BOOLEAN sm1_decrypt_ecb_3key(UINT8 *Ekey, UINT8 *Akey, UINT8 *Skey, UINT8 sksel, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 sm1EK[4];
	UINT32 sm1AK[4];
	UINT32 sm1SK[4];
	UINT32 sm1IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sm1EK, Ekey, 16);
	memcpy((UINT8 *)sm1AK, Akey, 16);
	if (sksel == 1)	//SK使用外部输入
	{
		memcpy((UINT8 *)sm1SK, Skey, 16);
	}
	else
	{
		memset((UINT8*)sm1SK, 0x00, 16);
	}
	memset((UINT8*)sm1IV, 0x00, 16);

	memcpy((UINT8*)g_algorithm_buf1, cipher, length);
	SM1_EnDecrypt(SM1_MODE_DE,SM1_MODE_ECB, sksel, sm1EK,sm1AK,sm1SK,
				sm1IV,g_algorithm_buf1,  g_algorithm_buf1, length);
	memcpy(plain, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

BOOLEAN sm1_encrypt_cbc_3key(UINT8 *Ekey, UINT8 *Akey, UINT8 *Skey, UINT8 *iv, UINT8 sksel, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 sm1EK[4];
	UINT32 sm1AK[4];
	UINT32 sm1SK[4];
	UINT32 sm1IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sm1EK, Ekey, 16);
	memcpy((UINT8 *)sm1IV, iv, 16);
	memcpy((UINT8 *)sm1AK, Akey, 16);
	if (sksel == 1)	//SK使用外部输入
	{
		memcpy((UINT8 *)sm1SK, Skey, 16);
	}
	else
	{
		memset((UINT8*)sm1SK, 0x00, 16);
	}

	memcpy((UINT8*)g_algorithm_buf1, plain, length);
	SM1_EnDecrypt(SM1_MODE_EN,SM1_MODE_CBC, sksel, sm1EK,sm1AK,sm1SK,
				sm1IV,g_algorithm_buf1,  g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

BOOLEAN sm1_decrypt_cbc_3key(UINT8 *Ekey, UINT8 *Akey, UINT8 *Skey, UINT8 *iv, UINT8 sksel, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 sm1EK[4];
	UINT32 sm1AK[4];
	UINT32 sm1SK[4];
	UINT32 sm1IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sm1EK, Ekey, 16);
	memcpy((UINT8 *)sm1IV, iv, 16);
	memcpy((UINT8 *)sm1AK, Akey, 16);
	if (sksel == 1)	//SK使用外部输入
	{
		memcpy((UINT8 *)sm1SK, Skey, 16);
	}
	else
	{
		memset((UINT8*)sm1SK, 0x00, 16);
	}

	memcpy((UINT8*)g_algorithm_buf1, cipher, length);
	SM1_EnDecrypt(SM1_MODE_DE,SM1_MODE_CBC, sksel, sm1EK,sm1AK,sm1SK,
				sm1IV,g_algorithm_buf1,  g_algorithm_buf1, length);
	memcpy(plain, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

BOOLEAN sm3_hash_data(UINT8 *src, UINT32 bitLen, UINT8 *dest)
{
	UINT8 i;	
	UINT32 data_buf[8] = {0};
	UINT32 u32Dest[8] = {0};
	UINT8 bit_remain;
	UINT8 lastBlockLen;
	UINT8 blockLen;
	Hash_tx hash_tx;
	
	bit_remain = bitLen&0x07;
	if(bit_remain)
	{
		return FALSE;
	}

	SHA_Init(&hash_tx, HASH_SM3_1);
	blockLen = bitLen>>9;		//512位(即64字节)一组
	for(i=0;i<blockLen;i++)
	{
		SHA_Update(&hash_tx, src+i*64, 64);
	}
	if (bitLen%512 != 0)
	{
		if (bit_remain == 0)
		{
			lastBlockLen = (bitLen%512)/8;
		}
		else	//TODO:不是整数字节的情况，暂时处理有问题，应用中是否有此情况?
		{
			lastBlockLen = ((bitLen%512)/8)+1;
		}

		SHA_Update(&hash_tx, src+blockLen*64, lastBlockLen);
	}
	
	SHA_Final(&hash_tx, u32Dest);		//输出为大端格式
	memcpy(dest, (UINT8 *)u32Dest, 32);

	return TRUE;	
}

BOOLEAN sha_hash_data(UINT8 type, UINT8 *src, UINT32 bitLen, UINT8 *dest)
{
	UINT8 i;	
	UINT32 data_buf[8] = {0};
	UINT32 u32Dest[8] = {0};
	UINT8 bit_remain;
	UINT8 lastBlockLen;
	UINT8 blockLen;
	Hash_tx hash_tx;
	
	bit_remain = bitLen&0x07;
	if(bit_remain)
	{
		return FALSE;
	}

	if ((type<HASH_SHA0_1) || (type>HASH_SHA256_1))
	{
		return FALSE;
	}
	
	SHA_Init(&hash_tx,type);
	blockLen = bitLen>>9;		//512位(即64字节)一组
	for(i=0;i<blockLen;i++)
	{
		SHA_Update(&hash_tx, src+i*64, 64);
	}
	if (bitLen%512 != 0)
	{
		if (bit_remain == 0)
		{
			lastBlockLen = (bitLen%512)/8;
		}
		else	//TODO:不是整数字节的情况，暂时处理有问题，应用中是否有此情况?
		{
			lastBlockLen = ((bitLen%512)/8)+1;
		}

		SHA_Update(&hash_tx, src+blockLen*64, lastBlockLen);
	}
	
	SHA_Final(&hash_tx, u32Dest);		//输出为大端格式
	memcpy(dest, (UINT8 *)u32Dest, 32);

	return TRUE;	
}


void SM2Init(unsigned short testbits)
{
	UINT8 databuf[64];
	
	stuEccPrams.uBits = testbits;
	
	//初始化ECC参数
	EccBig32Initial(&(stuEccPrams.stuPrimeP));
	EccBig32Initial(&(stuEccPrams.stuCoefficientA));
	EccBig32Initial(&(stuEccPrams.stuCoefficientB));
	EccBig32Initial(&(stuEccPrams.stuGx));
	EccBig32Initial(&(stuEccPrams.stuGy));
	EccBig32Initial(&(stuEccPrams.stuPrimeN));
	EccBig32Initial(&(stuPrivKey));
	EccBig32Initial(&(stuPubKey.stuQx));
	EccBig32Initial(&(stuPubKey.stuQy));
	EccBig32Initial(&stuSignatureR);
	EccBig32Initial(&stuSignatureS);

	if (stuEccPrams.uBits == 192)
	{
		EccCharToHex(charGx_192,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuGx));
		
		EccCharToHex(charGy_192,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuGy));
		
		EccCharToHex(charPrimeN_192,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuPrimeN));

		EccCharToHex(charPrimeP_192,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuPrimeP));
		
		EccCharToHex(charCoefficientA_192,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuCoefficientA));

		EccCharToHex(charCoefficientB_192,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuCoefficientB));	

		stuSignatureR.uLen = SM2_192_LENGTH;
		stuSignatureS.uLen = SM2_192_LENGTH;
		stuPubKey.stuQx.uLen = SM2_192_LENGTH;
		stuPubKey.stuQy.uLen = SM2_192_LENGTH;
		stuPrivKey.uLen = SM2_192_LENGTH;
	}	
	else if (stuEccPrams.uBits == 256)
	{
		EccCharToHex(charGx_256,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuGx));
		
		EccCharToHex(charGy_256,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuGy));
		
		EccCharToHex(charPrimeN_256,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuPrimeN));

		EccCharToHex(charPrimeP_256,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuPrimeP));
		
		EccCharToHex(charCoefficientA_256,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuCoefficientA));

		EccCharToHex(charCoefficientB_256,databuf,stuEccPrams.uBits/4);
		EccByBufToBigInt32(databuf,stuEccPrams.uBits/8,&(stuEccPrams.stuCoefficientB));	
		
		stuSignatureR.uLen = SM2_256_LENGTH;
		stuSignatureS.uLen = SM2_256_LENGTH;
		stuPubKey.stuQx.uLen = SM2_256_LENGTH;
		stuPubKey.stuQy.uLen = SM2_256_LENGTH;
		stuPrivKey.uLen = SM2_256_LENGTH;
	}
}	


//密钥都为小端结构
void sm2_genarate_keypair(void)
{
	SM2GenerateKeyPair(&stuEccPrams,&stuPrivKey,&stuPubKey);
}

void sm2_encrypt_data_v1(UINT8 * pPlain, UINT32 kLen, UINT8 *pCipher)
{
	UINT32 byteLen;

	if ((kLen%8) != 0)
	{
		byteLen = (kLen/8)+1;
	}
	else
	{
		byteLen = kLen/8;
	}

	//明文和密文不使用同一个缓冲区，则SM2加解密的明文长度最大支持到1.7K字节
	//memcpy(apdu_response, pPlain, byteLen);
	SM2Encrypt_V1((UINT8*)pPlain, byteLen, &stuPubKey,&stuEccPrams,(UINT8*)g_algorithm_buf1);
	*pCipher = 0x04;		//97字节模式
	if (g_sm2Bits == 192)
	{
		memcpy(pCipher+1, (UINT8*)g_algorithm_buf1, 48);		//C1
		memcpy(pCipher+1+48, (UINT8*)g_algorithm_buf1+48+byteLen, 32);		//C3
		memcpy(pCipher+1+48+32, (UINT8*)g_algorithm_buf1+48, byteLen);		//C2
	}
	else		//256bits
	{
		memcpy(pCipher+1, (UINT8*)g_algorithm_buf1, 64);		//C1
		memcpy(pCipher+1+64, (UINT8*)g_algorithm_buf1+64+byteLen, 32);		//C3
		memcpy(pCipher+1+64+32, (UINT8*)g_algorithm_buf1+64, byteLen);		//C2
	}
}

void sm2_encrypt_data_v2(UINT8 * pPlain, UINT32 kLen, UINT8 *pCipher)
{
	UINT32 byteLen;

	if ((kLen%8) != 0)
	{
		byteLen = (kLen/8)+1;
	}
	else
	{
		byteLen = kLen/8;
	}

	//明文和密文不使用同一个缓冲区，则SM2加解密的明文长度最大支持到1.7K字节
	memcpy((UINT8*)apdu_response, pPlain, byteLen);
	SM2Encrypt_V2((UINT8*)apdu_response, byteLen, &stuPubKey,&stuEccPrams,(UINT8*)g_algorithm_buf1);
	*pCipher = 0x04;		//97字节模式
	if (g_sm2Bits == 192)
	{
		memcpy(pCipher+1, (UINT8*)g_algorithm_buf1, 48+32+byteLen);		//C1/C3/C2
	}
	else		//256bits
	{
		memcpy(pCipher+1, (UINT8*)g_algorithm_buf1, 64+32+byteLen);		//C1/C3/C2
	}
}

//kLen是明文的位长度
//Return         : 1:decrypt successs; 0:fail
BOOLEAN sm2_decrypt_data_v1(UINT8 * pCipher, UINT32 kLen, UINT8 *pPlain)
{
	UINT8 result;
	UINT32 byteLen;

	if ((kLen%8) != 0)
	{
		byteLen = (kLen/8)+1;
	}
	else
	{
		byteLen = kLen/8;
	}

	//明文和密文不使用同一个缓冲区，则SM2加解密的明文长度最大支持到1.7K字节
	//04字节模式
	if (g_sm2Bits == 192)
	{
		memcpy((UINT8*)apdu_response, pCipher+1, 48);		//C1
		memcpy((UINT8*)apdu_response+48+byteLen, pCipher+1+48, 32);	//C3
		memcpy((UINT8*)apdu_response+48, pCipher+1+48+32, byteLen);	//C2
	}
	else
	{
		memcpy((UINT8*)apdu_response, pCipher+1, 64);		//C1
		memcpy((UINT8*)apdu_response+64+byteLen, pCipher+1+64, 32);	//C3
		memcpy((UINT8*)apdu_response+64, pCipher+1+64+32, byteLen);	//C2
	}
		
	result = SM2Decrypt_V1((UINT8*)apdu_response,byteLen,&stuPrivKey,&stuEccPrams,(UINT8*)g_algorithm_buf1);
	memset(apdu_response, 0x00, 2048);
	if (result == 1)		//成功
	{
		memcpy(pPlain, (UINT8*)g_algorithm_buf1, byteLen);
	}
	return result;
}


//kLen是明文的位长度
//Return         : 1:decrypt successs; 0:fail
BOOLEAN sm2_decrypt_data_v2(UINT8 * pCipher, UINT32 kLen, UINT8 *pPlain)
{
	UINT8 result;
	UINT32 byteLen;

	if ((kLen%8) != 0)
	{
		byteLen = (kLen/8)+1;
	}
	else
	{
		byteLen = kLen/8;
	}

	//明文和密文不使用同一个缓冲区，则SM2加解密的明文长度最大支持到1.7K字节
	//04字节模式
	if (g_sm2Bits == 192)
	{
		memcpy((UINT8*)apdu_response, pCipher+1, 48+32+byteLen);		//C1/C3/C2
	}
	else
	{
		memcpy((UINT8*)apdu_response, pCipher+1, 64+32+byteLen);		//C1/C3/C2
	}
		
	result = SM2Decrypt_V2((UINT8*)apdu_response,byteLen,&stuPrivKey,&stuEccPrams,(UINT8*)g_algorithm_buf1);
	memset(apdu_response, 0x00, 2048);
	if (result == 1)		//成功
	{
		memcpy(pPlain, (UINT8*)g_algorithm_buf1, byteLen);
	}
	
	return result;
}

/**************************************************************************************************
* Function Name  : GetZ
* Description    : Computes  id_Z = sm3(id_bit_length||id||ECC_a||ECC_b||ECC_BaseX||ECC_BaseY||PubX||PubY)
* Input          : - *id           : input id data 
				 : - id_bit_length : the bit length of id data (id_bit_length%8=0)
				 : - * PubX        : public key x coordinate
				 : - * PubY        : public key y coordinate
* Output		 : - * id_Z        : output the hash data Z 
* Return         : carry
**************************************************************************************************/

UINT8 GetZ(UINT8* id,UINT16 id_bit_length,UINT32* PubX,UINT32* PubY,UINT32* id_Z)
{
	UINT8 data_buf[256];
	UINT8 byte_cnt;
	UINT8 bit_remain;
	UINT32 temp[8];
	int i;
	UINT8 CurveLength;

	if (g_sm2Bits == 192)
	{
		CurveLength = SM2_192_LENGTH;
	}
	else
	{
		CurveLength = SM2_256_LENGTH;
	}
	
	for(i=0;i<256;i++)
	{
		data_buf[i] = 0;
	}
	
	byte_cnt = id_bit_length >> 3;
	bit_remain = id_bit_length & 0x07;
	if(bit_remain)
	{
		return 0;
	}	
	data_buf[0] = (UINT8)(id_bit_length>>8);
	data_buf[1] = (UINT8)id_bit_length;
	//copy id after id_bit_length
	memcpy((data_buf+2),id,byte_cnt);
	//copy ECC_a after id	
	memcpy((UINT8 *)temp, (UINT8 *)stuEccPrams.stuCoefficientA.auValue, CurveLength*4);
	wordswap(temp,temp,CurveLength);
	memcpy((data_buf+2+byte_cnt),(UINT8*)temp,CurveLength<<2);
		
	//copy ECC_b after ECC_a
	memcpy((UINT8 *)temp, (UINT8 *)stuEccPrams.stuCoefficientB.auValue, CurveLength*4);
	wordswap(temp,temp,CurveLength);
	memcpy((data_buf+2+byte_cnt+CurveLength*4),(UINT8*)temp,CurveLength<<2);	
	
	//copy ECC_BaseX after ECC_b	
	memcpy((UINT8 *)temp, (UINT8 *)stuEccPrams.stuGx.auValue, CurveLength*4);
	wordswap(temp,temp,CurveLength);
	memcpy((data_buf+2+byte_cnt+2*CurveLength*4),(UINT8*)temp,CurveLength<<2);
	
	//copy ECC_BaseY after ECC_BaseX
	memcpy((UINT8 *)temp, (UINT8 *)stuEccPrams.stuGy.auValue, CurveLength*4);
	wordswap(temp,temp,CurveLength);
	memcpy((data_buf+2+byte_cnt+3*CurveLength*4),(UINT8*)temp,CurveLength<<2);
	
	//copy PubX after ECC_BaseY	
	memcpy((UINT8 *)temp, (UINT8 *)PubX, CurveLength*4);
	wordswap(temp,temp,CurveLength);
	memcpy((data_buf+2+byte_cnt+4*CurveLength*4),(UINT8*)temp,CurveLength<<2);
	
	//copy PubY after PubX
	memcpy((UINT8 *)temp, (UINT8 *)PubY, CurveLength*4);
	wordswap(temp,temp,CurveLength);
	memcpy((data_buf+2+byte_cnt+5*CurveLength*4),(UINT8*)temp,CurveLength<<2);

	sm3_hash_data(data_buf,((2+byte_cnt+24*CurveLength)<<3),(UINT8 *)id_Z);
	memcpy((UINT8*)g_sm2Za, (UINT8 *)id_Z, 32);
	
	return 1;
}

UINT8 GetE(UINT8 *pMessage,UINT32 kLen,UINT32* id_Z,UINT32 *pDigest)
{
	UINT32 byteLen;

	if ((kLen%8) != 0)
	{
		byteLen = (kLen/8)+1;
	}
	else
	{
		byteLen = kLen/8;
	}

	memcpy((UINT8*)g_algorithm_buf1, (UINT8*)id_Z, 32);
	memcpy((UINT8*)g_algorithm_buf1+32, (UINT8*)pMessage, byteLen);
	sm3_hash_data((UINT8*)g_algorithm_buf1, kLen+32*8, (UINT8*)pDigest);
	memcpy((UINT8*)g_sm2E, (UINT8 *)pDigest, 32);

	return 1;	
}

void sm2_sign_data_IDA(UINT8 * sm2_message, UINT32 bitLen, UINT8 * Signature0, UINT8 * Signature1)
{
	UINT32 u32Signature[8];
	UINT32 byteLen;

	if ((bitLen%8) != 0)
	{
		byteLen = (bitLen/8)+1;
	}
	else
	{
		byteLen = bitLen/8;
	}
	
	memcpy((UINT8 *)g_algorithm_buf1, sm2_message, byteLen);
	
	SM2SignatureWithIDA(g_algorithm_buf1,byteLen,g_userid,g_useridLen,&stuEccPrams,&stuPubKey,&stuPrivKey,&stuSignatureR,&stuSignatureS);

	if (g_sm2Bits == 192)
	{
		wordswap(u32Signature, stuSignatureR.auValue, SM2_192_LENGTH);
		memcpy(Signature0, (UINT8*)u32Signature, SM2_192_LENGTH*4);
		wordswap(u32Signature, stuSignatureS.auValue, SM2_192_LENGTH);
		memcpy(Signature1, (UINT8*)u32Signature, SM2_192_LENGTH*4);
	}
	else
	{
		wordswap(u32Signature, stuSignatureR.auValue, SM2_256_LENGTH);
		memcpy(Signature0, (UINT8*)u32Signature, SM2_256_LENGTH*4);
		wordswap(u32Signature, stuSignatureS.auValue, SM2_256_LENGTH);
		memcpy(Signature1, (UINT8*)u32Signature, SM2_256_LENGTH*4);
	}
}

//函数参数都是大端结构
void sm2_sign_data(UINT8 * sm2_message, UINT32 bitLen, UINT8 * Signature0, UINT8 * Signature1)
{
	UINT32 id_Z[8];
	UINT32 digest[8];
	
	GetZ((UINT8*)g_userid,g_useridLen*8,stuPubKey.stuQx.auValue, stuPubKey.stuQy.auValue, id_Z);
	GetE(sm2_message,bitLen,id_Z,digest);	//digest是大端
	
	memset((UINT8*)stuSignatureR.auValue, 0x00, 32);
	memset((UINT8*)stuSignatureS.auValue, 0x00, 32);
	if (g_sm2Bits == 192)
	{
		SM2Signature((UINT8*)(digest+2),SM2_192_LENGTH*4,&stuEccPrams,&stuPrivKey,&stuSignatureR,&stuSignatureS);
	}
	else
	{
		SM2Signature((UINT8*)digest,SM2_256_LENGTH*4,&stuEccPrams,&stuPrivKey,&stuSignatureR,&stuSignatureS);
	}
	
	//签名数据为小端，需转换成大端
	if (g_sm2Bits == 192)
	{
		wordswap(id_Z, stuSignatureR.auValue, SM2_192_LENGTH);		//id_Z临时缓冲区
		memcpy(Signature0, (UINT8*)id_Z, SM2_192_LENGTH*4);
		wordswap(id_Z, stuSignatureS.auValue, SM2_192_LENGTH);		//id_Z临时缓冲区
		memcpy(Signature1, (UINT8*)id_Z, SM2_192_LENGTH*4);
	}
	else
	{
		wordswap(id_Z, stuSignatureR.auValue, SM2_256_LENGTH);		//id_Z临时缓冲区
		memcpy(Signature0, (UINT8*)id_Z, SM2_256_LENGTH*4);
		wordswap(id_Z, stuSignatureS.auValue, SM2_256_LENGTH);		//id_Z临时缓冲区
		memcpy(Signature1, (UINT8*)id_Z, SM2_256_LENGTH*4);
	}
}


// 1:verify success; 0:fail
BOOLEAN sm2_verify_data_IDA(UINT8 * sm2_message, UINT32 bitLen, UINT8 * Signature0, UINT8 * Signature1)
{
	BOOLEAN result;
	UINT32 u32Signature[8];
	UINT32 byteLen;

	if ((bitLen%8) != 0)
	{
		byteLen = (bitLen/8)+1;
	}
	else
	{
		byteLen = bitLen/8;
	}

	memcpy((UINT8 *)g_algorithm_buf1, sm2_message, byteLen);

	if (g_sm2Bits == 192)
	{
		memcpy((UINT8*)u32Signature, Signature0, SM2_192_LENGTH*4);
		wordswap(stuSignatureR.auValue, u32Signature, SM2_192_LENGTH);
		memcpy((UINT8*)u32Signature, Signature1, SM2_192_LENGTH*4);
		wordswap(stuSignatureS.auValue, u32Signature, SM2_192_LENGTH);
	}
	else
	{
		memcpy((UINT8*)u32Signature, Signature0, SM2_256_LENGTH*4);
		wordswap(stuSignatureR.auValue, u32Signature, SM2_256_LENGTH);
		memcpy((UINT8*)u32Signature, Signature1, SM2_256_LENGTH*4);
		wordswap(stuSignatureS.auValue, u32Signature, SM2_256_LENGTH);
	}
		
	result = SM2VerificationWithIDA(g_algorithm_buf1,byteLen,g_userid,g_useridLen,&stuEccPrams,&stuPubKey,&stuSignatureR,&stuSignatureS);		
	
	return result;
}

// 1:verify success; 0:fail
BOOLEAN sm2_verify_data(UINT8 * sm2_message, UINT32 bitLen, UINT8 * Signature0, UINT8 * Signature1)
{
	BOOLEAN result;
	UINT32 id_Z[8];
	UINT32 digest[8];

	GetZ((UINT8*)g_userid,g_useridLen*8,stuPubKey.stuQx.auValue, stuPubKey.stuQy.auValue, id_Z);
	GetE(sm2_message,bitLen,id_Z,digest);	//digest是大端

	//签名数据为小端，需做转换
	if (g_sm2Bits == 192)
	{
		memcpy((UINT8*)id_Z, Signature0, SM2_192_LENGTH*4);	//id_Z临时缓冲区
		wordswap(stuSignatureR.auValue, id_Z, SM2_192_LENGTH);
		memcpy((UINT8*)id_Z, Signature1, SM2_192_LENGTH*4);	//id_Z临时缓冲区
		wordswap(stuSignatureS.auValue, id_Z, SM2_192_LENGTH);
	}
	else
	{
		memcpy((UINT8*)id_Z, Signature0, SM2_256_LENGTH*4);	//id_Z临时缓冲区
		wordswap(stuSignatureR.auValue, id_Z, SM2_256_LENGTH);
		memcpy((UINT8*)id_Z, Signature1, SM2_256_LENGTH*4);	//id_Z临时缓冲区
		wordswap(stuSignatureS.auValue, id_Z, SM2_256_LENGTH);
	}

	if (g_sm2Bits == 192)
	{
		result = SM2Verification((UINT8*)(digest+2),SM2_192_LENGTH*4,&stuEccPrams,&stuPubKey,&stuSignatureR,&stuSignatureS);
	}
	else
	{
		result = SM2Verification((UINT8*)digest,SM2_256_LENGTH*4,&stuEccPrams,&stuPubKey,&stuSignatureR,&stuSignatureS);
	}
	
	return result;
}

BOOLEAN S_Check(unsigned int *s1, unsigned int * s2,SM2_STU_PRAMS *pstuEccPrams)
{
	UINT8 i;
	
	if(pstuEccPrams->uBits == 192)
	{
		for(i=0;i<6;i++)
		{
			if(s1[i] != s2[i])
			{
				return FALSE;
			}
		}
	}
	
	if(pstuEccPrams->uBits == 256)
	{
		for(i=0;i<8;i++)
		{
			if(s1[i] != s2[i])
			{
				return FALSE;
			}
		}
	}	
			
	return TRUE;
}

BOOLEAN sm2_Exchange_Key(UINT8* response, UINT16 *respLen)
{
	SM2_STU_PUBKEY stuPubKey_A;
	ECC_STU_BIGINT32 stuPrivKey_A;
	SM2_STU_PUBKEY stuPubKey_TempA;
	ECC_STU_BIGINT32 stuPrivKey_TempA;
	SM2_STU_PUBKEY stuPubKey_B;
	ECC_STU_BIGINT32 stuPrivKey_B;
	SM2_STU_PUBKEY stuPubKey_TempB;
	ECC_STU_BIGINT32 stuPrivKey_TempB;
	UINT32 ZA[8];
	UINT32 ZB[8];
	UINT32 mode;
	UINT32 keyA[8];
	UINT32 keyB[8];
	UINT32 klen;
	UINT32 SA[8];
	UINT32 S1[8];
	UINT32 SB[8];
	UINT32 S2[8];	
	UINT32 result;
	UINT8 userid[16] = {0};
	UINT8 useridLen = 16;

	SM2GenerateKeyPair(&stuEccPrams,&stuPrivKey_A,&stuPubKey_A);
	SM2GenerateKeyPair(&stuEccPrams,&stuPrivKey_TempA,&stuPubKey_TempA);
	SM2GenerateKeyPair(&stuEccPrams,&stuPrivKey_B,&stuPubKey_B);
	SM2GenerateKeyPair(&stuEccPrams,&stuPrivKey_TempB,&stuPubKey_TempB);

	GetZ((UINT8*)userid,useridLen*8,stuPubKey_A.stuQx.auValue, stuPubKey_A.stuQy.auValue, ZA);
	GetZ((UINT8*)userid,useridLen*8,stuPubKey_B.stuQx.auValue, stuPubKey_B.stuQy.auValue, ZB);
	
	mode = 0;		//己方为A方
	klen = 128;
	 
	result = SM2KeyExchange(&stuEccPrams,&stuPrivKey_A,&stuPubKey_TempA,&stuPrivKey_TempA,&stuPubKey_B,&stuPubKey_TempB,ZA,ZB,keyA,klen,SA,S1,mode);
	//计算完成后把S1发送给对方，并比较SA和S2
	if (result == FALSE)
		return FALSE;
	
   	mode = 1;		//己方为B方
   	klen = 128;
             
	result = SM2KeyExchange(&stuEccPrams,&stuPrivKey_B,&stuPubKey_TempB,&stuPrivKey_TempB,&stuPubKey_A,&stuPubKey_TempA,ZB,ZA,keyB,klen,S2,SB,mode);
	//计算完成后把S2发送给对方，并比较SB和S1
	if (result == FALSE)
		return FALSE;
	
	////////////////////A_B Check ////////////////////////////////////////////////////
	if (S_Check(SA, S2, &stuEccPrams) == FALSE)
		return FALSE;

	if (S_Check(SB, S1, &stuEccPrams) == FALSE)
		return FALSE;

	memcpy(response, (UINT8*)keyA, 32);
	memcpy(response+32, (UINT8*)keyB, 32);

	*respLen = 64;
	
	return TRUE;  	
}

void rsa_genarate_keypair(UINT16 rsaBits, UINT8 fixKey, UINT8 rsaCrt)
{	
	RsaGenerateKeyPair(rsaBits, &mypublicKey, &myprivateKey,fixKey, rsaCrt);
}

BOOLEAN rsa_encrypt_data(UINT16 rsaBits, UINT8 * pPlain, UINT32 kLen, UINT8 *pCipher)
{
	UINT16 byteLen;
	UINT32 outLen;
	int result;

	if ((kLen > rsaBits) || ((kLen%8) != 0))
	{
		return FALSE;
	}
	byteLen = (kLen/8);

	memset((UINT8*)g_algorithm_buf1, 0x00, rsaBits/8);
	memcpy((UINT8*)g_algorithm_buf1+rsaBits/8-byteLen, pPlain, byteLen);
	wordswap(g_algorithm_buf1, g_algorithm_buf1, rsaBits/8/4);		//大端转小端
	result = RsaPubKey(g_algorithm_buf1,&outLen,g_algorithm_buf1,rsaBits/8,&mypublicKey);
	if (result !=0)
		return FALSE;
	
	wordswap(g_algorithm_buf1, g_algorithm_buf1, rsaBits/8/4);		//小端转大端
	memcpy(pCipher, (UINT8*)g_algorithm_buf1, rsaBits/8);
	
	return TRUE;
}


BOOLEAN rsa_decrypt_data(UINT16 rsaBits, UINT8 rsaCrt, UINT8 * pCipher, UINT32 kLen, UINT8 *pPlain)
{
	UINT16 byteLen;
	UINT32 outLen;
	int result;

	if ((kLen > rsaBits) || ((kLen%8) != 0))
	{
		return FALSE;
	}
	byteLen = (kLen/8);
	
	memset((UINT8*)g_algorithm_buf1, 0x00, rsaBits/8);
	memcpy((UINT8*)g_algorithm_buf1+rsaBits/8-byteLen, pCipher, byteLen);
	wordswap(g_algorithm_buf1, g_algorithm_buf1, rsaBits/8/4);		//大端转小端
	if(rsaCrt)
	{
		result = RsaPrivKeyCRT(g_algorithm_buf1, &outLen, g_algorithm_buf1, rsaBits/8, &myprivateKey);
	}
	else
	{
		result = RsaPrivKey(g_algorithm_buf1, &outLen, g_algorithm_buf1, rsaBits/8, &myprivateKey);
	}
	if (result != 0)
		return FALSE;
	
	wordswap(g_algorithm_buf1, g_algorithm_buf1, rsaBits/8/4);		//小端转大端
	memcpy(pPlain, (UINT8*)g_algorithm_buf1, rsaBits/8);

	return TRUE;	
}

BOOLEAN rsa_sign_data(UINT16 rsaBits, UINT8 rsaCrt, UINT8 * message, UINT32 kLen, UINT8 * Signature)
{
	UINT16 byteLen;
	UINT32 outLen;
	int result;

	if ((kLen > rsaBits) || ((kLen%8) != 0))
	{
		return FALSE;
	}
	byteLen = (kLen/8);
	
	memset((UINT8*)g_algorithm_buf1, 0x00, rsaBits/8);
	memcpy((UINT8*)g_algorithm_buf1+rsaBits/8-byteLen, message, byteLen);
	wordswap(g_algorithm_buf1, g_algorithm_buf1, rsaBits/8/4);		//大端转小端
	if(rsaCrt)
	{
		result = RsaPrivKeyCRT(g_algorithm_buf1, &outLen, g_algorithm_buf1, rsaBits/8, &myprivateKey);
	}
	else
	{
		result = RsaPrivKey(g_algorithm_buf1, &outLen, g_algorithm_buf1, rsaBits/8, &myprivateKey);
	}
	if (result != 0)
		return FALSE;
	
	wordswap(g_algorithm_buf1, g_algorithm_buf1, rsaBits/8/4);		//小端转大端
	memcpy(Signature, (UINT8*)g_algorithm_buf1, rsaBits/8);

	return TRUE;	
}


// 1:verify success; 0:fail
BOOLEAN rsa_verify_data(UINT16 rsaBits, UINT8 * Signature, UINT32 kLen, UINT8 * message)
{
	UINT16 byteLen;
	UINT32 outLen;
	int result;

	if ((kLen > rsaBits) || ((kLen%8) != 0))
	{
		return FALSE;
	}
	byteLen = (kLen/8);

	memset((UINT8*)g_algorithm_buf1, 0x00, rsaBits/8);
	memcpy((UINT8*)g_algorithm_buf1+rsaBits/8-byteLen, Signature, byteLen);
	wordswap(g_algorithm_buf1, g_algorithm_buf1, rsaBits/8/4);		//大端转小端
	result = RsaPubKey(g_algorithm_buf1,&outLen,g_algorithm_buf1,rsaBits/8,&mypublicKey);
	if (result != 0)
		return FALSE;
	
	wordswap(g_algorithm_buf1, g_algorithm_buf1, rsaBits/8/4);		//小端转大端
	memcpy(message, (UINT8*)g_algorithm_buf1, rsaBits/8);

	return TRUE;	
}

BOOLEAN des_encrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[2];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT64);
	memset((UINT8*)desIV, 0x00, 8);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	ret = DES_EnDecrypt(DES_MODE_EN, DES_MODE_ECB, desKey, DES_BIT64, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}

BOOLEAN des_decrypt_ecb(UINT8 *key, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[2];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT64);
	memset((UINT8*)desIV, 0x00, 8);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	ret = DES_EnDecrypt(DES_MODE_DE, DES_MODE_ECB, desKey, DES_BIT64, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(plain, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}


BOOLEAN des_encrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[2];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT64);
	memcpy((UINT8 *)desIV, iv, 8);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	ret = DES_EnDecrypt(DES_MODE_EN, DES_MODE_CBC, desKey, DES_BIT64, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}

BOOLEAN des_decrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[2];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT64);
	memcpy((UINT8 *)desIV, iv, 8);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	ret = DES_EnDecrypt(DES_MODE_DE, DES_MODE_CBC, desKey, DES_BIT64, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(plain, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}


BOOLEAN tdes_encrypt_ecb_2key(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[6];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT128);
	memset((UINT8*)desIV, 0x00, 8);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	ret = DES_EnDecrypt(DES_MODE_EN, DES_MODE_ECB, desKey, DES_BIT128, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}

BOOLEAN tdes_decrypt_ecb_2key(UINT8 *key, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[6];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT128);
	memset((UINT8*)desIV, 0x00, 8);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	ret = DES_EnDecrypt(DES_MODE_DE, DES_MODE_ECB, desKey, DES_BIT128, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(plain, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}


BOOLEAN tdes_encrypt_cbc_2key(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[6];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT128);
	memcpy((UINT8 *)desIV, iv, 8);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	ret = DES_EnDecrypt(DES_MODE_EN, DES_MODE_CBC, desKey, DES_BIT128, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}

BOOLEAN tdes_decrypt_cbc_2key(UINT8 *key, UINT8 *iv, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[6];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT128);
	memcpy((UINT8 *)desIV, iv, 8);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	ret = DES_EnDecrypt(DES_MODE_DE, DES_MODE_CBC, desKey, DES_BIT128, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(plain, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}


BOOLEAN tdes_encrypt_ecb_3key(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[6];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT192);
	memset((UINT8*)desIV, 0x00, 8);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	ret = DES_EnDecrypt(DES_MODE_EN, DES_MODE_ECB, desKey, DES_BIT192, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}

BOOLEAN tdes_decrypt_ecb_3key(UINT8 *key, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[6];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT192);
	memset((UINT8*)desIV, 0x00, 8);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	ret = DES_EnDecrypt(DES_MODE_DE, DES_MODE_ECB, desKey, DES_BIT192, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(plain, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}


BOOLEAN tdes_encrypt_cbc_3key(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[6];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT192);
	memcpy((UINT8 *)desIV, iv, 8);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	ret = DES_EnDecrypt(DES_MODE_EN, DES_MODE_CBC, desKey, DES_BIT192, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}

BOOLEAN tdes_decrypt_cbc_3key(UINT8 *key, UINT8 *iv, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[6];
	UINT32 desIV[2];
	
	if (length%8 != 0)
		return FALSE;
	
	memcpy((UINT8 *)desKey, key, DES_BIT192);
	memcpy((UINT8 *)desIV, iv, 8);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	ret = DES_EnDecrypt(DES_MODE_DE, DES_MODE_CBC, desKey, DES_BIT192, desIV, g_algorithm_buf1, g_algorithm_buf1, length, 0);
	if (ret != 0)
	{
		return FALSE;
	}
	else
	{
		memcpy(plain, (UINT8*)g_algorithm_buf1, length);
		return TRUE;
	}
}

BOOLEAN aes_encrypt_ecb(UINT8 *key, UINT8 keyMode, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 aesKey[8];
	UINT32 aesIV[4];
	
	if ((length%16 != 0) || (keyMode>2))
		return FALSE;
	
	memcpy((UINT8 *)aesKey, key, 32);
	memset((UINT8*)aesIV, 0x00, 16);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	AES_EnDecrypt(AES_MODE_EN, AES_MODE_ECB, aesKey, aesIV, keyMode, g_algorithm_buf1, g_algorithm_buf1, length);

	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}

BOOLEAN aes_decrypt_ecb(UINT8 *key, UINT8 keyMode, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 aesKey[8];
	UINT32 aesIV[4];
	
	if ((length%16 != 0) || (keyMode>2))
		return FALSE;
	
	memcpy((UINT8 *)aesKey, key, 32);
	memset((UINT8*)aesIV, 0x00, 16);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	AES_EnDecrypt(AES_MODE_DE, AES_MODE_ECB, aesKey, aesIV, keyMode, g_algorithm_buf1, g_algorithm_buf1, length);

	memcpy(plain, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}


BOOLEAN aes_encrypt_cbc(UINT8 *key, UINT8 *IV, UINT8 keyMode, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 aesKey[8];
	UINT32 aesIV[4];
	UINT32 i, j;
	UINT32 crypto[4];		//CBC模式临时缓冲区
	UINT32 data_in[4];		//CBC模式临时缓冲区
	
	if ((length%16 != 0) || (keyMode>2))
		return FALSE;
	
	memcpy((UINT8 *)aesKey, key, 32);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);
	memcpy((UINT8*)crypto, IV, 16);

	for(i=0;i<length/4;i+=4)
	{
		for(j=0;j<4;j++) 
		{
			data_in[j] = g_algorithm_buf1[i+j] ^ crypto[j];
		}
		AES_EnDecrypt(AES_MODE_EN, AES_MODE_ECB, aesKey, aesIV, keyMode, data_in, crypto, 16);
		for (j=0;j<4;j++)
		{
			g_algorithm_buf1[i+j] = crypto[j];
		}
	} 

	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}

BOOLEAN aes_decrypt_cbc(UINT8 *key, UINT8 *IV, UINT8 keyMode, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 aesKey[8];
	UINT32 aesIV[4];
	UINT32 i, j;
	UINT32 crypto[4];		//CBC模式临时缓冲区
	UINT32 data_in[4];		//CBC模式临时缓冲区
	UINT32 temp[4];
	
	if ((length%16 != 0) || (keyMode>2))
		return FALSE;
	
	memcpy((UINT8 *)aesKey, key, 32);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);
	memcpy((UINT8*)crypto, IV, 16);

	for(i=0;i<length/4;i+=4)
	{
		//ECB模式加密：加解密、ECB、key、IV、密钥长度、源数据、目标数据、长度
		AES_EnDecrypt(AES_MODE_DE, AES_MODE_ECB, aesKey, aesIV, keyMode, &g_algorithm_buf1[i], data_in, 16);
		for(j=0;j<4;j++) 
		{
			temp[j] = data_in[j] ^ crypto[j];
		}
		for (j=0;j<4;j++)
		{
			crypto[j] = g_algorithm_buf1[i+j];
		}
		for (j=0;j<4;j++)
		{
			g_algorithm_buf1[i+j] = temp[j];
		}
	}			

	memcpy(plain, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}


BOOLEAN aes_encrypt_ctr(UINT8 *key, UINT8 *iv, UINT8 keyMode, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 aesKey[8];
	UINT32 aesIV[4];
	
	if ((length%16 != 0) || (keyMode>2))
		return FALSE;
	
	memcpy((UINT8 *)aesKey, key, 32);
	memcpy((UINT8 *)aesIV, iv, 16);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	AES_EnDecrypt(AES_MODE_EN, AES_MODE_CTR, aesKey, aesIV, keyMode, g_algorithm_buf1, g_algorithm_buf1, length);

	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}

BOOLEAN aes_decrypt_ctr(UINT8 *key, UINT8 *iv, UINT8 keyMode, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 aesKey[8];
	UINT32 aesIV[4];
	
	if ((length%16 != 0) || (keyMode>2))
		return FALSE;
	
	memcpy((UINT8 *)aesKey, key, 32);
	memcpy((UINT8 *)aesIV, iv, 16);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	AES_EnDecrypt(AES_MODE_DE, AES_MODE_CTR, aesKey, aesIV, keyMode, g_algorithm_buf1, g_algorithm_buf1, length);

	memcpy(plain, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}


BOOLEAN sms4_encrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 sms4Key[4];
	UINT32 sms4IV[4];
	
	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sms4Key, key, 16);
	memset((UINT8*)sms4IV, 0x00, 8);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	SMS4_EnDecrypt(SMS4_MODE_EN, SMS4_MODE_ECB, sms4Key, sms4IV, g_algorithm_buf1, g_algorithm_buf1, length);
	
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}


BOOLEAN sms4_decrypt_ecb(UINT8 *key, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 sms4Key[4];
	UINT32 sms4IV[4];
	
	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sms4Key, key, 16);
	memset((UINT8*)sms4IV, 0x00, 8);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	SMS4_EnDecrypt(SMS4_MODE_DE, SMS4_MODE_ECB, sms4Key, sms4IV, g_algorithm_buf1, g_algorithm_buf1, length);
	
	memcpy(plain, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}


BOOLEAN sms4_encrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 sms4Key[4];
	UINT32 sms4IV[4];
	
	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sms4Key, key, 16);
	memcpy((UINT8*)sms4IV, iv, 16);
	memcpy((UINT8*)g_algorithm_buf1, plain, length);

	SMS4_EnDecrypt(SMS4_MODE_EN, SMS4_MODE_CBC, sms4Key, sms4IV, g_algorithm_buf1, g_algorithm_buf1, length);
	
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}

BOOLEAN sms4_decrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *cipher, UINT8 *plain, UINT32 length)
{
	UINT32 sms4Key[4];
	UINT32 sms4IV[4];
	
	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)sms4Key, key, 16);
	memcpy((UINT8*)sms4IV, iv, 16);
	memcpy((UINT8*)g_algorithm_buf1, cipher, length);

	SMS4_EnDecrypt(SMS4_MODE_DE, SMS4_MODE_CBC, sms4Key, sms4IV, g_algorithm_buf1, g_algorithm_buf1, length);
	
	memcpy(plain, (UINT8*)g_algorithm_buf1, length);
	return TRUE;
}

BOOLEAN ssf33_encrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 ssf33MK[4];
	UINT32 ssf33SK[4];
	UINT32 ssf33IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)ssf33MK, key, 16);
	memset((UINT8*)ssf33SK, 0x00, 16);
	memset((UINT8*)ssf33IV, 0x00, 16);

	memcpy((UINT8*)g_algorithm_buf1, plain, length);	
	SSF33_EnDecrypt(SSF33_MODE_ENCRYPT, SSF33_MODE_ECB, 0, ssf33MK, ssf33SK, ssf33IV, g_algorithm_buf1, g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}


BOOLEAN ssf33_decrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 ssf33MK[4];
	UINT32 ssf33SK[4];
	UINT32 ssf33IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)ssf33MK, key, 16);
	memset((UINT8*)ssf33SK, 0x00, 16);
	memset((UINT8*)ssf33IV, 0x00, 16);

	memcpy((UINT8*)g_algorithm_buf1, plain, length);	
	SSF33_EnDecrypt(SSF33_MODE_DECRYPT, SM1_MODE_ECB, 0, ssf33MK, ssf33SK, ssf33IV, g_algorithm_buf1, g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}


BOOLEAN ssf33_encrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 ssf33MK[4];
	UINT32 ssf33SK[4];
	UINT32 ssf33IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)ssf33MK, key, 16);
	memset((UINT8*)ssf33SK, 0x00, 16);
	memcpy((UINT8 *)ssf33IV, iv, 16);

	memcpy((UINT8*)g_algorithm_buf1, plain, length);	
	SSF33_EnDecrypt(SSF33_MODE_ENCRYPT, SM1_MODE_CBC, 0, ssf33MK, ssf33SK, ssf33IV, g_algorithm_buf1, g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}


BOOLEAN ssf33_decrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 ssf33MK[4];
	UINT32 ssf33SK[4];
	UINT32 ssf33IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)ssf33MK, key, 16);
	memset((UINT8*)ssf33SK, 0x00, 16);
	memcpy((UINT8 *)ssf33IV, iv, 16);
	
	memcpy((UINT8*)g_algorithm_buf1, plain, length);	

	SSF33_EnDecrypt(SSF33_MODE_DECRYPT, SM1_MODE_CBC, 0, ssf33MK, ssf33SK, ssf33IV, g_algorithm_buf1, g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

BOOLEAN ssf33_encrypt_ecb_2key(UINT8 *Mkey, UINT8 *Skey, UINT8 sksel, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 ssf33MK[4];
	UINT32 ssf33SK[4];
	UINT32 ssf33IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)ssf33MK, Mkey, 16);
	if (sksel == 1)	//SK使用外部输入
	{
		memcpy((UINT8 *)ssf33SK, Skey, 16);
	}
	else
	{
		memset((UINT8*)ssf33SK, 0x00, 16);
	}
	memset((UINT8*)ssf33IV, 0x00, 16);

	memcpy((UINT8*)g_algorithm_buf1, plain, length);	
	SSF33_EnDecrypt(SSF33_MODE_ENCRYPT, SSF33_MODE_ECB, sksel, ssf33MK, ssf33SK, ssf33IV, g_algorithm_buf1, g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}


BOOLEAN ssf33_decrypt_ecb_2key(UINT8 *Mkey, UINT8 *Skey, UINT8 sksel, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 ssf33MK[4];
	UINT32 ssf33SK[4];
	UINT32 ssf33IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)ssf33MK, Mkey, 16);
	if (sksel == 1)	//SK使用外部输入
	{
		memcpy((UINT8 *)ssf33SK, Skey, 16);
	}
	else
	{
		memset((UINT8*)ssf33SK, 0x00, 16);
	}
	memset((UINT8*)ssf33IV, 0x00, 16);

	memcpy((UINT8*)g_algorithm_buf1, plain, length);	
	SSF33_EnDecrypt(SSF33_MODE_DECRYPT, SM1_MODE_ECB, sksel, ssf33MK, ssf33SK, ssf33IV, g_algorithm_buf1, g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}


BOOLEAN ssf33_encrypt_cbc_2key(UINT8 *Mkey, UINT8 *Skey, UINT8 *iv, UINT8 sksel, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 ssf33MK[4];
	UINT32 ssf33SK[4];
	UINT32 ssf33IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)ssf33MK, Mkey, 16);
	if (sksel == 1)	//SK使用外部输入
	{
		memcpy((UINT8 *)ssf33SK, Skey, 16);
	}
	else
	{
		memset((UINT8*)ssf33SK, 0x00, 16);
	}
	memcpy((UINT8 *)ssf33IV, iv, 16);

	memcpy((UINT8*)g_algorithm_buf1, plain, length);	
	SSF33_EnDecrypt(SSF33_MODE_ENCRYPT, SM1_MODE_CBC, sksel, ssf33MK, ssf33SK, ssf33IV, g_algorithm_buf1, g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}


BOOLEAN ssf33_decrypt_cbc_2key(UINT8 *Mkey, UINT8 *Skey, UINT8 *iv, UINT8 sksel, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT32 ssf33MK[4];
	UINT32 ssf33SK[4];
	UINT32 ssf33IV[4];

	if (length%16 != 0)
		return FALSE;
	
	memcpy((UINT8 *)ssf33MK, Mkey, 16);
	if (sksel == 1)	//SK使用外部输入
	{
		memcpy((UINT8 *)ssf33SK, Skey, 16);
	}
	else
	{
		memset((UINT8*)ssf33SK, 0x00, 16);
	}
	memcpy((UINT8 *)ssf33IV, iv, 16);
	
	memcpy((UINT8*)g_algorithm_buf1, plain, length);	

	SSF33_EnDecrypt(SSF33_MODE_DECRYPT, SM1_MODE_CBC, sksel, ssf33MK, ssf33SK, ssf33IV, g_algorithm_buf1, g_algorithm_buf1, length);
	memcpy(cipher, (UINT8*)g_algorithm_buf1, length);

	return TRUE;
}

