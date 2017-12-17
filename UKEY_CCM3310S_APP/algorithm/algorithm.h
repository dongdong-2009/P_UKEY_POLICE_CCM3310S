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

#ifdef  ALGORITHM_SYMBOL
#define	ALGORITHM_EXT
#else	
#define	ALGORITHM_EXT extern
#endif

#include "sm2.h"
#include "rsa.h"

#define DES_MODE_EN	0x00		//加密
#define DES_MODE_DE	0x01		//解密

#define DES_MODE_ECB	0x00		//ECB
#define DES_MODE_CBC	0x01		//CBC


#define DES_BIT64	8		//key长度
#define DES_BIT56	7		//key长度
#define DES_BIT128	16		//key长度
#define DES_BIT192	24		//key长度


#define AES_MODE_EN	0x00		//加密
#define AES_MODE_DE	0x01		//解密

#define AES_MODE_ECB	0x00		//ECB
#define AES_MODE_CTR	0x01		//CTR

#define AES_KEYMODE_128	0x00		//key长度: 128 bits
#define AES_KEYMODE_192	0x01		//key长度: 192 bits
#define AES_KEYMODE_256	0x02		//key长度: 256 bits


#define SM1_MODE_EN		0x00
#define SM1_MODE_DE		0x01

#define SM1_MODE_ECB	0x00
#define SM1_MODE_CBC	0x01

#define SM1_SK_INTERNAL		0x00
#define SM1_SK_EXTERNAL		0x01


#define SMS4_MODE_EN		0x00
#define SMS4_MODE_DE		0x01

#define SMS4_MODE_ECB		0x00
#define SMS4_MODE_CBC		0x01


#define SSF33_MODE_ENCRYPT	0x00		//加密
#define SSF33_MODE_DECRYPT	0x01		//解密
#define SSF33_MODE_CBC	0x01		//CBC
#define SSF33_MODE_ECB	0x00		//ECB
#define SSF33_MODE_CBC	0x01		//CBC
#define SSF33_SK_INTERNAL	0x00	//内部SK
#define SSF33_SK_EXTERNAL	0x01	//外部SK




#define SM2_192_LENGTH		6
#define SM2_256_LENGTH		8


//算法用临时缓冲区
ALGORITHM_EXT UINT32 g_algorithm_buf1[512];	

ALGORITHM_EXT UINT8 g_random[16];

//SM1密钥
ALGORITHM_EXT UINT32 g_sm1SK[4];
ALGORITHM_EXT UINT32 g_sm1AK[4];
ALGORITHM_EXT UINT32 g_sm1EK[4];
ALGORITHM_EXT UINT32 g_sm1IV[4];

//MAC密钥
ALGORITHM_EXT UINT32 g_MACKey[4];
ALGORITHM_EXT UINT32 g_MACIV[4];

//用户ID
ALGORITHM_EXT UINT8 g_useridLen;
ALGORITHM_EXT UINT32 g_userid[16];

//SM2变量
ALGORITHM_EXT UINT16 g_sm2Bits;

ALGORITHM_EXT SM2_STU_PRAMS stuEccPrams;
ALGORITHM_EXT SM2_STU_PUBKEY stuPubKey;
ALGORITHM_EXT ECC_STU_BIGINT32 stuPrivKey;

ALGORITHM_EXT ECC_STU_BIGINT32 stuSignatureR;
ALGORITHM_EXT ECC_STU_BIGINT32 stuSignatureS;

ALGORITHM_EXT UINT32 g_sm2Za[8];
ALGORITHM_EXT UINT32 g_sm2E[8];

//RSA变量
//#define		RSA_TEST_BITS	512
//#define		RSA_TEST_BITS	1024
//#define		RSA_TEST_BITS	2048

ALGORITHM_EXT STU_RSA_PUBKEY  mypublicKey;
ALGORITHM_EXT STU_RSA_PRIVKEY myprivateKey;

//DES密钥
ALGORITHM_EXT UINT32 g_desKey[6];
ALGORITHM_EXT UINT32 g_desIV[2];

//AES密钥
ALGORITHM_EXT UINT32 g_aesKey[8];
ALGORITHM_EXT UINT32 g_aesIV[4];

//SMS4密钥
ALGORITHM_EXT UINT32 g_sms4Key[4];
ALGORITHM_EXT UINT32 g_sms4IV[4];

//SSF33密钥
ALGORITHM_EXT UINT32 g_ssf33MK[4];
ALGORITHM_EXT UINT32 g_ssf33SK[4];
ALGORITHM_EXT UINT32 g_ssf33IV[4];

ALGORITHM_EXT void Genarate_Random(UINT8* random, UINT32 len);

ALGORITHM_EXT BOOLEAN sm1_encrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN sm1_decrypt_ecb(UINT8 *key, UINT8 *cipher, UINT8 *plain, UINT32 length);
ALGORITHM_EXT BOOLEAN sm1_encrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN sm1_decrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN sm1_encrypt_ecb_3key(UINT8 *Ekey, UINT8 *Akey, UINT8 *Skey, UINT8 sksel, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN sm1_decrypt_ecb_3key(UINT8 *Ekey, UINT8 *Akey, UINT8 *Skey, UINT8 sksel, UINT8 *cipher, UINT8 *plain, UINT32 length);
ALGORITHM_EXT BOOLEAN sm1_encrypt_cbc_3key(UINT8 *Ekey, UINT8 *Akey, UINT8 *Skey, UINT8 *iv, UINT8 sksel, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN sm1_decrypt_cbc_3key(UINT8 *Ekey, UINT8 *Akey, UINT8 *Skey, UINT8 *iv, UINT8 sksel, UINT8 *cipher, UINT8 *plain, UINT32 length);

ALGORITHM_EXT BOOLEAN sm3_hash_data(UINT8 *src, UINT32 bitLen, UINT8 *dest);

ALGORITHM_EXT void sm2_genarate_keypair(void);
ALGORITHM_EXT void sm2_encrypt_data_v1(UINT8 * pPlain, UINT32 kLen, UINT8 *pCipher);
ALGORITHM_EXT void sm2_encrypt_data_v2(UINT8 * pPlain, UINT32 kLen, UINT8 *pCipher);
ALGORITHM_EXT BOOLEAN sm2_decrypt_data_v1(UINT8 * pCipher, UINT32 kLen, UINT8 *pPlain);
ALGORITHM_EXT BOOLEAN sm2_decrypt_data_v2(UINT8 * pCipher, UINT32 kLen, UINT8 *pPlain);
ALGORITHM_EXT void sm2_sign_data(UINT8 * sm2_message, UINT32 bitLen, UINT8 * Signature0, UINT8 * Signature1);
ALGORITHM_EXT BOOLEAN sm2_verify_data(UINT8 * sm2_message, UINT32 bitLen, UINT8 * Signature0, UINT8 * Signature1);

ALGORITHM_EXT void rsa_genarate_keypair(UINT16 rsaBits, UINT8 fixKey, UINT8 rsaCrt);
ALGORITHM_EXT BOOLEAN rsa_encrypt_data(UINT16 rsaBits, UINT8 * pPlain, UINT32 kLen, UINT8 *pCipher);
ALGORITHM_EXT BOOLEAN rsa_decrypt_data(UINT16 rsaBits, UINT8 rsaCrt, UINT8 * pCipher, UINT32 kLen, UINT8 *pPlain);
ALGORITHM_EXT BOOLEAN rsa_sign_data(UINT16 rsaBits, UINT8 rsaCrt, UINT8 * message, UINT32 kLen, UINT8 * Signature);
ALGORITHM_EXT BOOLEAN rsa_verify_data(UINT16 rsaBits, UINT8 * Signature, UINT32 kLen, UINT8 * message);

ALGORITHM_EXT BOOLEAN des_encrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN des_decrypt_ecb(UINT8 *key, UINT8 *cipher, UINT8 *plain, UINT32 length);
ALGORITHM_EXT BOOLEAN des_encrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN des_decrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *cipher, UINT8 *plain, UINT32 length);
ALGORITHM_EXT BOOLEAN tdes_encrypt_ecb_2key(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN tdes_decrypt_ecb_2key(UINT8 *key, UINT8 *cipher, UINT8 *plain, UINT32 length);
ALGORITHM_EXT BOOLEAN tdes_encrypt_cbc_2key(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN tdes_decrypt_cbc_2key(UINT8 *key, UINT8 *iv, UINT8 *cipher, UINT8 *plain, UINT32 length);
ALGORITHM_EXT BOOLEAN tdes_encrypt_ecb_3key(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN tdes_decrypt_ecb_3key(UINT8 *key, UINT8 *cipher, UINT8 *plain, UINT32 length);
ALGORITHM_EXT BOOLEAN tdes_encrypt_cbc_3key(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN tdes_decrypt_cbc_3key(UINT8 *key, UINT8 *iv, UINT8 *cipher, UINT8 *plain, UINT32 length);

ALGORITHM_EXT BOOLEAN aes_encrypt_ecb(UINT8 *key, UINT8 keyMode, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN aes_decrypt_ecb(UINT8 *key, UINT8 keyMode, UINT8 *cipher, UINT8 *plain, UINT32 length);
ALGORITHM_EXT BOOLEAN aes_encrypt_ctr(UINT8 *key, UINT8 *iv, UINT8 keyMode, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN aes_decrypt_ctr(UINT8 *key, UINT8 *iv, UINT8 keyMode, UINT8 *cipher, UINT8 *plain, UINT32 length);

ALGORITHM_EXT BOOLEAN sms4_encrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN sms4_decrypt_ecb(UINT8 *key, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN sms4_encrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length);
ALGORITHM_EXT BOOLEAN sms4_decrypt_cbc(UINT8 *key, UINT8 *iv, UINT8 *plain, UINT8 *cipher, UINT32 length);


