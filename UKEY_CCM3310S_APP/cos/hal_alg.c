#include "ccm3310s.h"
#include "hal_alg.h"
#include "cos.h"
#include "algorithm.h"
#include "sha.h"

void TOOL_BcdToAsc(unsigned char *Dest,unsigned char *Src,unsigned int Len)
{
    unsigned int i;
	for(i=0;i<Len;i++)
	{
	    //高Nibble转换
	    if(((*(Src + i) & 0xF0) >> 4) <= 9)
	    {
            *(Dest + 2*i) = ((*(Src + i) & 0xF0) >> 4) + 0x30;
        }
        else
        {
            *(Dest + 2*i)  = ((*(Src + i) & 0xF0) >> 4) + 0x37;   //大写A~F
        }    
        //低Nibble转换
        if((*(Src + i) & 0x0F) <= 9)
        {
            *(Dest + 2*i + 1) = (*(Src + i) & 0x0F) + 0x30;
        }
        else
        {
            *(Dest + 2*i + 1) = (*(Src + i) & 0x0F) + 0x37;   //大写A~F
        }    
    }
}




//填充数据
void hal_alg_filldata(unsigned char *pbin,unsigned char *pbout,unsigned char per_len,unsigned short *len)
{
	unsigned short tmp_len=*len;
	unsigned char len1;
	if(pbin!=pbout)
		memmove(pbout,pbin,tmp_len);
	pbout[tmp_len++]=0x80;
	len1=tmp_len%per_len;
	if(len1)
	{
		memset(pbout+tmp_len,0x00,per_len-len1);
	}
	tmp_len+=per_len-len1;
	*len=tmp_len;
}

#if 0
//去填充数据,两个字节的长度+数据+填充
unsigned char hal_alg_defilldata(unsigned char *pbin,unsigned short *len)
{
	unsigned short len1=0;
	unsigned char temp_buffer[1024]={0x00};
	unsigned short temp_len=0;
	unsigned int i=0;
	len1=(pbin[0]<<8)+pbin[1];
	while(temp_len>0)
	{
		if(pbin[temp_len--]==0x80)
			break;
	}
	temp_len--;//减去80一字节长度
	
	if(len1==temp_len)
	{
		*len  =len1;	
		memcpy(temp_buffer,pbin+2,len1);	
		memcpy(pbin,temp_buffer,len1);
		return 0;
	}
	else
	{
		return 1;
	}



}
#else
//去填充数据,两个字节的长度+数据+填充
unsigned char hal_alg_defilldata(unsigned char *pbin,unsigned short *len)
{
	unsigned short len1,tmp_len=*len;
#if ALG_DEBUG
	UART_printf("tmp_len \r\n");
	HAL_UART_SEND_ASC((U8*)&tmp_len,2);	
#endif
	len1=(pbin[0]<<8)+pbin[1];
	while(tmp_len>0)
	{
		if(pbin[tmp_len--]==0x80)
			break;
	}
	tmp_len--;//减去80一字节长度
#if ALG_DEBUG
	UART_printf("tmp_len \r\n");
	HAL_UART_SEND_ASC((U8*)&tmp_len,2);
	UART_printf("len1 \r\n");
	HAL_UART_SEND_ASC((U8*)&len1,2);	
#endif
	
	if(len1==tmp_len)
	{
		*len=tmp_len;
		memmove(pbin,pbin+2,len1);
		return 0;
	}
	else 
		return 1;
}



#endif

void hal_alg_genrand(unsigned char *buf, unsigned short len)
{
	Genarate_Random(buf,  len);
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

void DES_CRYPT(U8 *in, U32 bytes, U8 *key, U8 key_len, U8 *iv, U8 decrypt, U8 cbc, U8 *out)
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

// ---------------------------------------------------------------------  
// Func : SM1_CRYPT()
// Description : Cryption for SM1
// ---------------------------------------------------------------------  
// Input
// in: 	 	Input data buffer. Plaintext to encrypt or crypto to decrypt.
// bytes: 	Length of input data in byte. Must be multiple of 16.
// key:	Key in byte format with the length of 16 bytes.
// new_key:	Flag to load key
// iv:		Initial value for CBC mode in byte format.
// decrypt:	Operation flag. 0 for encrypt, 1 for decrypt.
// cbc:		Operation flag. 0 for ECB, 1 for CBC,
//----------------------------------------------------------------------
// Output
// out: 	Output data buffer.

void SM1_CRYPT(U8 *in, U32 bytes, U8 *key, U8 *iv, U8 decrypt, U8 cbc, U8 *out)
{
	if(cbc)
	{
			if(decrypt)
			{
				sm1_decrypt_cbc(key,iv,in,out,bytes);	
			}
			else
			{
				sm1_encrypt_cbc(key,iv,in,out,bytes);
			}

	}
	else
	{
			if(decrypt)
			{
				sm1_decrypt_ecb(key,in,out,bytes);	
	
			}
			else
			{
				sm1_encrypt_ecb(key,in,out,bytes);

			}
	}

}
void	SSF33_CRYPT(U8 *in, U32 bytes, U8 *key, U8 *iv, U8 decrypt, U8 cbc, U8 *out)
{
	if(cbc)
	{
			if(decrypt)
			{
				ssf33_decrypt_cbc(key,iv,in,out,bytes);	
			}
			else
			{
				ssf33_encrypt_cbc(key,iv,in,out,bytes);
			}

	}
	else
	{
			if(decrypt)
			{
				ssf33_decrypt_ecb(key,in,out,bytes);	
	
			}
			else
			{
				ssf33_encrypt_ecb(key,in,out,bytes);

			}
	}

}

void	SM4_CRYPT(U8 *in, U32 bytes, U8 *key, U8 *iv, U8 decrypt, U8 cbc, U8 *out)
{

	if(cbc)
	{
			if(decrypt)
			{
				sms4_decrypt_cbc(key,iv,in,out,bytes);	
			}
			else
			{
				sms4_encrypt_cbc(key,iv,in,out,bytes);
			}

	}
	else
	{
			if(decrypt)
			{	
						
				sms4_decrypt_ecb(key,in,out,bytes);
	
			}
			else
			{
				sms4_encrypt_ecb(key,in,out,bytes);
			}
	}

}


unsigned char hal_3des_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char type,
								unsigned char *pbout)
{
	unsigned char cbc,encrypt;

	unsigned int *piIn = 0, *piOut=0;
	unsigned int piIv[2],piKey[6],pTmp[16];
	
	piIn = piOut = pTmp;

	
	if((SYMALG_MODEIS & type) == SYMALG_ECB)
	{
		cbc = 0;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC)
	{
		cbc = 1;
	}
	if((SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		encrypt = 0;
	}
	else if((SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		encrypt = 1;
	}

	
	DES_CRYPT(pbin, 16, pkey, 16, piv, encrypt, cbc, pbout);
	return 0;
}

unsigned char hal_sm1_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char type,
								unsigned char *pbout)
{
	unsigned char cbc,encrypt;
	if((SYMALG_MODEIS & type) == SYMALG_ECB)
	{
		cbc = 0;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC)
	{
		cbc = 1;
	}
	if((SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		encrypt = 0;
	}
	else if((SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		encrypt = 1;
	}
	
	SM1_CRYPT(pbin, len, pkey, piv, encrypt, cbc, pbout);	

	return 0;
}
unsigned char hal_ssf33_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char type,
								unsigned char *pbout)
{

	unsigned char cbc,encrypt;
	if((SYMALG_MODEIS & type) == SYMALG_ECB)
	{
		cbc = 0;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC)
	{
		cbc = 1;
	}
	if((SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		encrypt = 0;
	}
	else if((SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		encrypt = 1;
	}

	SSF33_CRYPT(pbin, len,pkey, piv, encrypt, cbc, pbout);
	return 0;
}
unsigned char hal_sm4_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char type,
								unsigned char *pbout)
{
	
	unsigned char cbc,encrypt;
	unsigned char tmp_buff[1024];
	unsigned char tmp_iv[16];
	unsigned char tmp_data[16];
	unsigned char plain[16];
	unsigned int i,j;

	if(len==0 || len%16!=0)
	{
		return 1;	
	}
	
	if((SYMALG_MODEIS & type) == SYMALG_ECB)
	{
		cbc = 0;
	}
	else if((SYMALG_MODEIS & type) == SYMALG_CBC)
	{
		if (piv == NULL)
			return 2;
		cbc = 1;
	}
	if((SYMALG_ENDECIS & type) == SYMALG_SENC)
	{
		encrypt = 0;
	}
	else if((SYMALG_ENDECIS & type) == SYMALG_SDEC)
	{
		encrypt = 1;
	}


	
	SM4_CRYPT(pbin, len,pkey, piv, encrypt, cbc, pbout);	

	return 0;
}

unsigned char hal_alg_bcae(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char mode,
								unsigned char *pbout,
								unsigned char algtype)
{
	if(algtype==ALG_SM4)
	{
		hal_sm4_oper(pbin,len,pkey,piv,mode,pbout);		
	}
	else if(algtype==ALG_SSF33)
	{
		hal_ssf33_oper(pbin,len,pkey,piv,mode,pbout);	
	}
	else if(algtype==ALG_SM1)
	{
		hal_sm1_oper(pbin,len,pkey,piv,mode,pbout);	
	}
	else 
		return 1;

	return 0;
	
}
unsigned char hal_alg_calmac(unsigned char *pbin,unsigned short len,unsigned char *pkey,unsigned char *piv,unsigned char *pmac,unsigned char algtype)
{
	unsigned short tmplen=len;
	int i=0;
	hal_alg_filldata(pbin,pbin,16,&tmplen);

	if(algtype==ALG_SM4)
	{
		hal_sm4_oper(pbin,tmplen,pkey,piv,SYMALG_CBC|SYMALG_SENC,pbin);		
	}
	else if(algtype==ALG_SSF33)
	{
		hal_ssf33_oper(pbin,tmplen,pkey,piv,SYMALG_CBC|SYMALG_SENC,pbin);	
	}
	else if(algtype==ALG_SM1)
	{
		hal_sm1_oper(pbin,tmplen,pkey,piv,SYMALG_CBC|SYMALG_SENC,pbin);	
	}
	else 
		return 1;
	memcpy(pmac,pbin+tmplen-16,4);
	return 0;
	
}
void Char2IntReverse(unsigned char* in, unsigned int* out,unsigned int len)
{

	unsigned int temp[64],templen;
		
	templen=len/4;
	
	memcpy((unsigned char*)temp,in,len);
	for(; templen != 0;)
	{
	  *(out++) = temp[--templen];
	}
}

void Int2CharReverse(unsigned int* in, unsigned char* out,unsigned int len)
{
	unsigned int temp[64],templen;
	templen=len;
	for(;len != 0;)
	{
		temp[--len] = *(in++);
	}
	memcpy(out,(unsigned char*)temp,templen*4);

}

void endiantrans(unsigned char *pbin,unsigned short len)
{
	unsigned char tmp;
	unsigned short i;

	for(i=0;i<len/2;i++)
	{
		tmp=pbin[i];
		pbin[i]=pbin[len-i-1];
		pbin[len-i-1]=tmp;
		
	}
}

unsigned char *G_ALG_BUF;
RSA_CRT_Key g_RsaKey;
RSA_PUBLICKEY g_rsapubkey;
RSA_CRT_PRIVATEKEY g_rsaprikey;
U32 *G_RSA_DATA_BUF;
unsigned char SM2_Init_flag;
SM2_KEYEXCHANGE *p_sm2_keyexchange;
SM2_KEYEXCHANGE1  *p_sm2_keyA;

void hal_RSAinit()
	{

	
		U32 *p_buff=(U32*)G_ALG_BUF=g_algorithm_buf1;
	
		
		g_RsaKey.pkp= p_buff;
		g_RsaKey.pkq =	p_buff+32*1;
		g_RsaKey.pkdp=	p_buff+32*2;
		g_RsaKey.pkdq=	p_buff+32*3;
		g_RsaKey.pku=  p_buff+32*4;
	
		g_RsaKey.pke = p_buff+32*5;
		g_RsaKey.pkn= p_buff+32*7;
	
		g_rsapubkey.pE= p_buff+32*5;
		g_rsapubkey.pN= p_buff+32*7;
		g_rsapubkey.pE[0] = 0x10001;
	
		g_rsaprikey.pP= p_buff;
		g_rsaprikey.pQ =  p_buff+32*1;
		g_rsaprikey.pDP=  p_buff+32*2;
		g_rsaprikey.pDQ=  p_buff+32*3;
		g_rsaprikey.pU=  p_buff+32*4;
		g_rsaprikey.pN = p_buff+32*7;
		G_RSA_DATA_BUF=(U32 *)(G_ALG_BUF+1536);
		
		return;
	}



//对待计算数据使用PKCS1_V1_5方式填充
unsigned char hal_alg_Rsapadding(		
							unsigned char* 		pInBuf,
							unsigned int		InLen,
							unsigned char* 		puchOutData,
							unsigned int		ulOutDataLen,
							unsigned char		type
							)
{
	unsigned int i, j;
	unsigned char *ptr;	
	
	if((type & ASYALG_ENDECIS) == ASYALG_ASIGN)//私钥填充
	{
			//PKCS1_V1_5方式填充私钥计算数据
		if (InLen > (ulOutDataLen - 11))
		{
			return NOK;
		}	
		ptr = puchOutData;
	
		*(ptr++) = 0;
		*(ptr++) = 1; /* Private Key BT (Block Type) */
	
		/* pad out with 0xff data */
		j = ulOutDataLen - 3 - InLen;
		memset(ptr, 0xff, j);
		ptr += j;
		*(ptr++) = '\0';
		memcpy(ptr, pInBuf, InLen);
	}
	else		//PKCS1_V1_5方式填充公钥计算数据
	{
		if (InLen > (ulOutDataLen - 11))
		{
			return NOK;
		}
	
		ptr = puchOutData;
	
		*(ptr++) = 0;
		*(ptr++) = 2; /* Public Key BT (Block Type) */
	
		/* pad out with non-zero random data */
		j = ulOutDataLen - 3 - InLen;	
		
		
		hal_alg_genrand(ptr,j);	//产生随机数	
		
		
		
		//检查随机数中是否有0，若有则重新生成随机数	
		for(i=0;i<j;i++)
		{
			if(*ptr == '\0')
			{
				do
				{
					hal_alg_genrand(ptr,1);
				}while(*ptr == '\0');
			}
			ptr++;			
		}	

		*(ptr++) = '\0';	
		memcpy(ptr, pInBuf, InLen);
	}

	return OK;
}
							
//从解密后数据中恢复原始数据
//ulFlags definition: bit0:0-rsa1024,1-rsa2048; bit1:reserved;bit2:0-PKCS1_V1_5方式填充,1-其它方式
unsigned char hal_alg_RsaOffPadding(		
							unsigned char* 		pInBuf,
							unsigned int		InLen,
							unsigned char* 		puchOutData,
							unsigned int*		pulOutDataLen)
{
	unsigned int i,j;
	unsigned char* ptr;
	ptr = pInBuf;

	if(*ptr)		//第一个字节必须是0
		return NOK;
	ptr++;	
	if(*ptr == 1)		//BT=1
	{
		ptr++;	//指向PS数据段
		j = InLen-2;
		for(i=0;i<j;i++)
		{
			if(*ptr != 0xff)	//检查PS段结束标志
			{
				if(*ptr == 0)
				{
					ptr++;
					break;
				}
				else
					return NOK;
			}
			ptr++;								
		}
		if(i == j)	//如果全是0xff，则错误
			return NOK;
		if(i < 8)	//如果PS段长度小于8字节，则错误
			return NOK;
		i++;
		*pulOutDataLen = j-i;	//有效数据长度
		memcpy(puchOutData, ptr, *pulOutDataLen);
	}
	else if(*ptr == 2)	//BT=2
	{
		j = InLen-2;
		for(i=0;i<j;i++)	//查找PS段结束标志'\0'
		{
			if(*(++ptr) == 0)
				break;				
		}
		if(i == j)	//如果全部是PS字段，则错误
			return NOK;
		if(i < 8)	//如果PS字段长度小于8，则错误
			return NOK;
		i++;
		*pulOutDataLen = j-i;
		ptr++;
		memcpy(puchOutData, ptr, *pulOutDataLen);
	}
	else
		return NOK;
	return OK;
}	



void hal_genRSAkey(RSA_CRT_Key *pRSAkey,unsigned char flag)
{
	unsigned short len,len1,rsaBits;
	U8 fixKey = 0x01;
	U8 rsaCrt = 0x01;

	if(flag==ASYALG_RSA1024)
	{
		rsaBits=1024;
		len1=128;
	}
	else if(flag==ASYALG_RSA2048)
	{
		rsaBits=2048;
		len1=256;
	}
	rsa_genarate_keypair(rsaBits, fixKey, rsaCrt);
	//转换成大端

//	HAL_UART_SendData("mypublicKey.exponent\r\n", sizeof("mypublicKey.exponent\r\n"));
//	HAL_UART_SEND_ASC( (UINT8*)mypublicKey.exponent,rsaBits/8);
//	HAL_UART_SendData("\r\n",2);
	wordswap_u8((U8*)pRSAkey->pkn, (UINT8*)mypublicKey.modulus, rsaBits/8);		//N
	wordswap_u8((U8*)pRSAkey->pke, (UINT8*)mypublicKey.exponent, rsaBits/8);		//E

//	HAL_UART_SendData("pRSAkey->pke\r\n", sizeof("pRSAkey->pke\r\n"));
//	HAL_UART_SEND_ASC( (UINT8*)pRSAkey->pke,rsaBits/8);
//	HAL_UART_SendData("\r\n",2);
	
	wordswap_u8((U8*)pRSAkey->pkd, (UINT8*)myprivateKey.exponent, rsaBits/8);		//D
	wordswap_u8((U8*)pRSAkey->pkp, (UINT8*)myprivateKey.prime[0], (rsaBits/8)/2);		//P
	wordswap_u8((U8*)pRSAkey->pkq, (UINT8*)myprivateKey.prime[1], (rsaBits/8)/2);		//Q
	wordswap_u8((U8*)pRSAkey->pkdp, (UINT8*)myprivateKey.primeExponent[0], (rsaBits/8)/2);		//DP
	wordswap_u8((U8*)pRSAkey->pkdq, (UINT8*)myprivateKey.primeExponent[1], (rsaBits/8)/2);		//DQ
	wordswap_u8((U8*)pRSAkey->pku, (UINT8*)myprivateKey.coefficient, (rsaBits/8)/2);		//Qinv

	return;
}



void hal_alg_RSApublic(RSA_PUBLICKEY *pubkey,unsigned char *pIn,unsigned char *pOut,unsigned char flag)
{
	unsigned int rsaBits,len2;
	unsigned int outLen;
//	memset((U8*)pubkey->pE,0,256);
//	pubkey->pE[0]=0x10001;

	if(flag==ASYALG_RSA1024)
	{
		rsaBits=1024;
		len2=128;
	}
	else if(flag==ASYALG_RSA2048)
	{
		rsaBits=2048;
		len2=256;
	}

	//pubkey	
	mypublicKey.bits=rsaBits;
	mypublicKey.exponent[0]=0x10001;	
	wordswap_u8( (UINT8*)mypublicKey.modulus, (U8*)pubkey->pN,rsaBits/8);		//N 大端转小端

	//data_in
	memcpy((UINT8*)G_RSA_DATA_BUF, pIn, rsaBits/8);
	wordswap(G_RSA_DATA_BUF, G_RSA_DATA_BUF, rsaBits/8/4);		//大端转小端

	RsaPubKey(G_RSA_DATA_BUF,&outLen,G_RSA_DATA_BUF,rsaBits/8,&mypublicKey);

	//data_out
	wordswap(G_RSA_DATA_BUF, G_RSA_DATA_BUF, rsaBits/8/4);		//小端转大端
	memcpy(pOut, (UINT8*)G_RSA_DATA_BUF, rsaBits/8);
}

void hal_alg_RSAprivate(RSA_CRT_PRIVATEKEY *privatekey,unsigned char *pIn,unsigned char *pOut,unsigned char flag)
{
	unsigned int len,rsaBits;
	unsigned int outLen;
	if(flag==ASYALG_RSA1024)
	{
		len=128;
		rsaBits=1024;
	}
	else if(flag==ASYALG_RSA2048)
	{
		len=256;
		rsaBits=2048;
	}

	//prikey
	myprivateKey.bits=rsaBits;
	myprivateKey.publicExponent[0]=0x10001;

	wordswap_u8( (U8*)myprivateKey.modulus, (U8*)privatekey->pN,rsaBits/8);		//N 大端转小端
	wordswap_u8((U8*)myprivateKey.prime[0],(U8*)privatekey->pP,(rsaBits/8)/2);
	wordswap_u8((U8*)myprivateKey.prime[1],(U8*)privatekey->pQ,(rsaBits/8)/2);
	wordswap_u8((U8*)myprivateKey.primeExponent[0],(U8*)privatekey->pDP,(rsaBits/8)/2);
	wordswap_u8((U8*)myprivateKey.primeExponent[1],(U8*)privatekey->pDQ,(rsaBits/8)/2);
	wordswap_u8((U8*)myprivateKey.coefficient,(U8*)privatekey->pU,(rsaBits/8)/2);	

	//data_in
	memcpy((UINT8*)G_RSA_DATA_BUF, pIn, rsaBits/8);
	wordswap(G_RSA_DATA_BUF, G_RSA_DATA_BUF, rsaBits/8/4);		//大端转小端	

#if 0

	HAL_UART_SendData("myprivateKey.p\r\n", sizeof("myprivateKey.p\r\n"));
	HAL_UART_SEND_ASC( (UINT8*)myprivateKey.prime[0],rsaBits/16);
	HAL_UART_SendData("\r\n",2);

	HAL_UART_SendData("myprivateKey.q\r\n", sizeof("myprivateKey.q\r\n"));
	HAL_UART_SEND_ASC( (UINT8*)myprivateKey.prime[1],rsaBits/16);
	HAL_UART_SendData("\r\n",2);

	HAL_UART_SendData("myprivateKey.dp\r\n", sizeof("myprivateKey.dp\r\n"));
	HAL_UART_SEND_ASC( (UINT8*)myprivateKey.primeExponent[0],rsaBits/16);
	HAL_UART_SendData("\r\n",2);

	HAL_UART_SendData("myprivateKey.dq\r\n", sizeof("myprivateKey.dq\r\n"));
	HAL_UART_SEND_ASC( (UINT8*)myprivateKey.primeExponent[1],rsaBits/16);
	HAL_UART_SendData("\r\n",2);	

	HAL_UART_SendData("myprivateKey.u\r\n", sizeof("myprivateKey.u\r\n"));
	HAL_UART_SEND_ASC( (UINT8*)myprivateKey.coefficient,rsaBits/16);
	HAL_UART_SendData("\r\n",2);		
#endif	

	RsaPrivKeyCRT(G_RSA_DATA_BUF, &outLen, G_RSA_DATA_BUF, rsaBits/8, &myprivateKey);

	//data_out
	wordswap(G_RSA_DATA_BUF, G_RSA_DATA_BUF, rsaBits/8/4);		//小端转大端
	memcpy(pOut, (UINT8*)G_RSA_DATA_BUF, rsaBits/8);

}


unsigned char hal_alg_SM2producekeys(U8  *pPubkey, U8 *pPrikey)
{
	UINT32 sm2keybuf[8];

	sm2_genarate_keypair();
	//公钥
	wordswap(sm2keybuf, stuPubKey.stuQx.auValue, SM2_256_LENGTH);	//转换成大端
	memcpy(pPubkey, (UINT8 *)sm2keybuf, SM2_256_LENGTH*4);

	wordswap(sm2keybuf, stuPubKey.stuQy.auValue, SM2_256_LENGTH);	//转换成大端
	memcpy(pPubkey+SM2_256_LENGTH*4, (UINT8 *)sm2keybuf, SM2_256_LENGTH*4);
	//私钥
	wordswap(sm2keybuf, stuPrivKey.auValue, SM2_256_LENGTH);	//转换成大端
	memcpy(pPrikey, (UINT8 *)sm2keybuf, SM2_256_LENGTH*4);

#if ALG_DEBUG
	UART_printf("SM2 pPubkey \r\n");
	HAL_UART_SEND_ASC((unsigned char*)pPubkey,64);

	UART_printf("SM2 pPrikey \r\n");
	HAL_UART_SEND_ASC((unsigned char*)pPrikey,32);
#endif		
	return 0;	
}
//flag 0: C1 +C2+C3 flag:1 C1+C3+C2
unsigned char hal_alg_SM2encrypt(U8 *Pubkey,U8 *pIn,unsigned int inlen,U8 *pOut,U8 flag)
{

	UINT32 sm2keybuf[16];
	//public key
	memcpy(sm2keybuf,Pubkey,64);
	wordswap( stuPubKey.stuQx.auValue, sm2keybuf,SM2_256_LENGTH);	//转换成小端
	wordswap( stuPubKey.stuQy.auValue,sm2keybuf+8, SM2_256_LENGTH);	//转换成小端

	sm2_encrypt_data_v1(pIn, inlen*8, pOut);

	memmove(pOut,pOut+1,inlen+96);
	return 0;
		
}

unsigned char hal_alg_SM2decrypt(U8 *Prikey,unsigned char *pIn,unsigned int inlen,unsigned char *pOut)
{
	U8 cipher[256];
	UINT32 sm2keybuf[16];
	//private key
	memcpy(sm2keybuf,Prikey,32);
	wordswap( stuPrivKey.auValue, sm2keybuf,SM2_256_LENGTH);	//转换成小端

	memcpy(cipher+1,pIn,inlen);
	sm2_decrypt_data_v1(cipher,( inlen-96)*8, pOut);
	return 0;
}


unsigned char hal_alg_SM2sign(U8 *Prikey, U8 *pHash, U8 *pSign)
{
	UINT32 id_Z[8];
	UINT32 sm2keybuf[16];

	//private key
	memcpy(sm2keybuf,Prikey,32);
	wordswap( stuPrivKey.auValue, sm2keybuf,SM2_256_LENGTH);	//转换成小端

	memset((UINT8*)stuSignatureR.auValue, 0x00, 32);
	memset((UINT8*)stuSignatureS.auValue, 0x00, 32);

	SM2Signature(pHash,SM2_256_LENGTH*4,&stuEccPrams,&stuPrivKey,&stuSignatureR,&stuSignatureS);
	
	//签名数据为小端，需转换成大端
	wordswap(id_Z, stuSignatureR.auValue, SM2_256_LENGTH);		//id_Z临时缓冲区
	memcpy(pSign, (UINT8*)id_Z, SM2_256_LENGTH*4);
	wordswap(id_Z, stuSignatureS.auValue, SM2_256_LENGTH);		//id_Z临时缓冲区
	memcpy(pSign+32, (UINT8*)id_Z, SM2_256_LENGTH*4);
	return 0;
}

unsigned char hal_alg_SM2verify(U8 *Pubkey,  U8*pHash,U8 *pSign)
{
	BOOLEAN result;
	UINT32 id_Z[8];
	UINT32 sm2keybuf[16];
	UINT32 sm2Signbuf[16];
	
	//public key
	memcpy(sm2keybuf,Pubkey,64);
	wordswap( stuPubKey.stuQx.auValue, sm2keybuf,SM2_256_LENGTH);	//转换成小端
	wordswap( stuPubKey.stuQy.auValue,sm2keybuf+8, SM2_256_LENGTH);	//转换成小端

	//sign data
	memcpy(sm2Signbuf,pSign,64);
	
	//签名数据为小端，需做转换
	memcpy((UINT8*)id_Z, sm2Signbuf, SM2_256_LENGTH*4);	//id_Z临时缓冲区
	wordswap(stuSignatureR.auValue, id_Z, SM2_256_LENGTH);
	memcpy((UINT8*)id_Z, sm2Signbuf+8, SM2_256_LENGTH*4);	//id_Z临时缓冲区
	wordswap(stuSignatureS.auValue, id_Z, SM2_256_LENGTH);

	result = SM2Verification((UINT8*)pHash,SM2_256_LENGTH*4,&stuEccPrams,&stuPubKey,&stuSignatureR,&stuSignatureS);
	if(1==result)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

unsigned char hal_alg_SM2genexchangekey(SM2_KEYEXCHANGE *p_sm2_keyex,unsigned char *pout,unsigned char mode)
{
	U8 key[32],ret;
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
	UINT32 temp[16]={0};
	
	//pu keya 
	memcpy(temp,p_sm2_keyex->selfpub,64);
 	wordswap(stuPubKey_A.stuQx, temp, 8);
 	wordswap(stuPubKey_A.stuQy, temp+8, 8);

	//pri keya
	memcpy(temp,p_sm2_keyex->selfpri,32);
 	wordswap(stuPrivKey_A.auValue, temp, 8);

	//pub temp a
	memcpy(temp,p_sm2_keyex->selftmppub,64);
 	wordswap(stuPubKey_TempA.stuQx, temp, 8);
 	wordswap(stuPubKey_TempA.stuQy, temp+8, 8);

	//pri  temp a
	memcpy(temp,p_sm2_keyex->selftmppri,32);
 	wordswap(stuPrivKey_TempA.auValue, temp, 8);


	//pub key b
	memcpy(temp,p_sm2_keyex->otherpub,64);
 	wordswap(stuPubKey_B.stuQx, temp, 8);
 	wordswap(stuPubKey_B.stuQy, temp+8, 8);


	//pub temp b
	memcpy(temp,p_sm2_keyex->othertmppub,64);
 	wordswap(stuPubKey_TempB.stuQx, temp, 8);
 	wordswap(stuPubKey_TempB.stuQy, temp+8, 8);


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

	memcpy(pout, (UINT8*)keyA, 32);
	memcpy(pout+32, (UINT8*)keyB, 32);

	return 0;
}
//----------------------hash运算------------------
Hash_tx hash_tx;

void hal_alg_hashinit(unsigned type)
{
	if(type==HASH_SM3)
		type =HASH_SM3_1;
	else if(type ==HASH_SHA1)
		type = HASH_SHA1_1;
	else if(type ==HASH_SHA224)
		type = HASH_SHA224_1;
	else if(type ==HASH_SHA256)
		type = HASH_SHA256_1;
	SHA_Init(&hash_tx,type);
}

void  hal_alg_hashupdate(unsigned char *pIn,unsigned int len)
{
	SHA_Update(&hash_tx, pIn, len);
}
void  hal_alg_hashfinal(unsigned char *pOut)
{
	UINT32 u32Dest[8] = {0};
	SHA_Final(&hash_tx, u32Dest);		//输出为大端格式
	memcpy(pOut, (UINT8 *)u32Dest, 32);

}
//----------------------hash运算END------------------

#if 0
//----------------------hash运算------------------
void hal_sha1_oper(unsigned char *pbin,
								unsigned int len,
								unsigned char *pbout)
{
	HASH_CTX sha1_ctx;
	Hash_Initialize(&sha1_ctx, 1);
	Hash_Update(&sha1_ctx,pbin, len);	
	Hash_Finalize(&sha1_ctx);
	memcpy(pbout, sha1_ctx.ABCDEFGH, 20);
}
HASH_CTX pHash_Ctx;
void hal_alg_hashinit(unsigned type)
{
	Hash_Initialize(&pHash_Ctx, type);
}

void  hal_alg_hashupdate(unsigned char *pIn,unsigned int len)
{
	Hash_Update(&pHash_Ctx,pIn,len);
}
void  hal_alg_hashfinal(unsigned char *pOut)
{
	Hash_Finalize(&pHash_Ctx);
	memcpy(pOut,(unsigned char*)pHash_Ctx.ABCDEFGH,32);
}
//----------------------hash运算END------------------





#endif // debug

