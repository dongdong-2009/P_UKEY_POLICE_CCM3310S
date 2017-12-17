#include "ccm3310s.h"
#include "HAL_Flash.h"
#include "HAL_ALG.h"
#include "algorithm_lib.h"





BOOLEAN des_encrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length)
{
	UINT8 ret;
	UINT32 desKey[2];
	UINT32 desIV[2];
	UINT32 g_algorithm_buf1[512];	
	
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
	UINT32 g_algorithm_buf1[512];	
	
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
	UINT32 g_algorithm_buf1[512];	
	
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
	UINT32 g_algorithm_buf1[512];	
	
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




// ---------------------------------------------------------------------  
// Func : DES_CRYPT()
// Description : Cryption for DES
// ---------------------------------------------------------------------  
// Input
// in: 	 	Input data buffer. Plaintext to encrypt or crypto to decrypt.
// bytes: 	Length of input data in byte. Must be multiple of 16.
// key:	Key in byte format
// key_len:	The length of the key in bit. 56/64 for DES, 112/128 for 3DES
// iv:		Initial value for CBC mode in byte format.
// decrypt:	Operation flag. 0 for encrypt, 1 for decrypt.
// cbc:		Operation flag. 0 for ECB, 1 for CBC,
//----------------------------------------------------------------------
// Output
// out: 	Output data buffer.

void DES_CRYPT(UINT8 *in, UINT32 bytes, UINT8 *key, UINT8 key_len, UINT8 *iv, UINT8 decrypt, UINT8 cbc, UINT8 *out)
{


	if(key_len==16)
	{
		if(cbc)
		{
			if(decrypt)
			{
				des_decrypt_cbc(key,  iv,in, out,bytes);
			}
			else
			{
				des_encrypt_cbc(key,  iv,in, out,bytes);
			}
		}
		else
		{
			if(decrypt)
			{
				des_decrypt_ecb(key,in, out,bytes);
			}
			else
			{
				des_encrypt_ecb(key,in, out,bytes);
			}
		}
	}
	else if(key_len==32)
	{
		

	}
}

unsigned char HAL_ALG_DESFix(unsigned char * pInBuf, 
								unsigned int  length,
								unsigned char *pKey, 
								unsigned char *pIV, 
								unsigned char type,
								unsigned char * pOutBuf)
{

	unsigned int i=0,j=0;
	unsigned short keylen;
	unsigned char encrypt, cbc;
	
	unsigned int *piIn = 0, *piOut=0;
	unsigned int piIv[2],piKey[6],pTmp[64];
	
	piIn = piOut = pTmp;

	if(length==0 || length%8!=0)
	{
		return NOK;	
	}
	
	if(pKey == NULL)
		return NOK;

	if((SYMALG_MODEIS & type) == SYMALG_CBC)
	{
		if (pIV == NULL)
			return NOK;
	}
	switch (type)
	{
		//DES ECB ENC
		case SYMALG_DES64 | SYMALG_ECB | SYMALG_SENC:
		{
			keylen = 8;
			encrypt = 0;
			cbc = 0;
		}
		break;
		//DES CBC ENC
		case SYMALG_DES64 | SYMALG_CBC | SYMALG_SENC:
		{
			keylen = 8;
			encrypt = 0;
			cbc = 1;
		}
		break;
		//DES ECB DEC
		case SYMALG_DES64 | SYMALG_ECB | SYMALG_SDEC:
		{
			keylen = 8;
			encrypt = 1;
			cbc = 0;
		}
		break;
		//DES CBC DEC
		case SYMALG_DES64 | SYMALG_CBC | SYMALG_SDEC:
		{
			keylen = 8;
			encrypt = 1;
			cbc = 1;
		}
		break;
		// 3DES 128 ECB ENC 
		case SYMALG_DES128 | SYMALG_ECB | SYMALG_SENC:
		{
			keylen = 16;
			encrypt = 0;
			cbc = 0;
		}
		break;
		// 3DES 128 ECB DEC
		case SYMALG_DES128 | SYMALG_ECB | SYMALG_SDEC:
		{
			keylen = 16;	
			encrypt = 1;
			cbc = 0;
		}
		break;
		// 3DES 128 CBC ENC
		case SYMALG_DES128 | SYMALG_CBC | SYMALG_SENC:
		{
			keylen = 16;	
			encrypt = 0;
			cbc = 1;
		}
		break;
		// 3DES 128 CBC DEC
		case SYMALG_DES128 | SYMALG_CBC | SYMALG_SDEC:
		{
			keylen = 16;		
			encrypt = 1;
			cbc = 1;
		}
		break;
		// 3DES 192 ECB ENC
		case SYMALG_DES192 | SYMALG_ECB | SYMALG_SENC:
		{
			keylen = 24;
			encrypt = 0;
			cbc = 0;
		}
		break;
		// 3DES 192 ECB DEC
		case SYMALG_DES192 | SYMALG_ECB | SYMALG_SDEC:
		{
			keylen = 24;	
			encrypt = 1;
			cbc = 0;
		}
		break;
		// 3DES 192 CBC ENC
		case SYMALG_DES192 | SYMALG_CBC | SYMALG_SENC:
		{
			keylen = 24;	
			encrypt = 0;
			cbc = 1;
		}
		break;
		// 3DES 192 CBC DEC
		case SYMALG_DES192 | SYMALG_CBC | SYMALG_SDEC:
		{
			keylen = 24;	
			encrypt = 1;
			cbc = 1;
		}
		break;
		default:
			return NOK;
		break;

	}
	
	if(cbc)
		memcpy((unsigned char*)piIv, pIV, 8);
		
	memcpy((unsigned char*)piIn, pInBuf, length);
	memcpy((unsigned char*)piKey, pKey, keylen);

	DES_EnDecrypt(encrypt,cbc,piKey,keylen,piIv,piIn,piOut,length,0);
	//DES_CRYPT((unsigned char*)piIn, length, (unsigned char*)piKey, keylen, (unsigned char*)piIv, encrypt, cbc, (unsigned char*)piOut);
	
	memcpy(pOutBuf, piOut, length);
	

	return OK;
}

