#ifndef __HAL_ALG_H__
#define __HAL_ALG_H__

#include "Global_Var.h"

#define SYMALG_ALGIS				0x0F
#define	SYMALG_SM1					0x01
#define	SYMALG_SSF33				0x02
#define	SYMALG_SMS4					0x03
#define	SYMALG_DES64				0x04
#define	SYMALG_DES128				0x05
#define	SYMALG_DES192				0x06

#define SYMALG_MODEIS				0x30
#define SYMALG_ECB					0x00
#define SYMALG_CBC					0x10
#define SYMALG_CFB					0x20
#define SYMALG_OFB					0x30

#define SYMALG_ENDECIS				0x40
#define SYMALG_SENC					0x00
#define SYMALG_SDEC					0x40

#define SYMALG_MACIS				0x80
#define SYMALG_MAC					0x80

#define ASYALG						0xcf
#define ASYALG_SM2					0xc1
#define ASYALG_RSA1024				0xc2
#define ASYALG_RSA2048				0xc3

#define ASYALG_ENDECIS				0x30
#define ASYALG_ASIGN				0x00
#define ASYALG_ADEC					0x10
#define ASYALG_AEXCH				0x20

#define HASHALG						0xff
#define HASHALG_SM3					0xf1
#define HASHALG_SHA1				0xf2
#define HASHALG_SHA256				0xf3
#define HASHALG_MD5					0xf4

#define SHA1_RESULTLEN  (160/8)


#define DES_MODE_EN	0x00		//加密
#define DES_MODE_DE	0x01		//解密

#define DES_MODE_ECB	0x00		//ECB
#define DES_MODE_CBC	0x01		//CBC


#define DES_BIT64	8		//key长度
#define DES_BIT56	7		//key长度
#define DES_BIT128	16		//key长度
#define DES_BIT192	24		//key长度


unsigned char HAL_ALG_DESFix(	unsigned char *pInBuf, 
								unsigned int  length, 
								unsigned char *pKey, 
								unsigned char *pIV, 
								unsigned char type,
								unsigned char *pOutBuf);





#endif




