#ifndef __HAL_ALG_H_
#define __HAL_ALG_H_


#define ALG_DEBUG 0

#ifndef NULL
#define NULL 0
#endif

typedef enum{				
	ALG_SM4=0,	
	ALG_SSF33,
	ALG_SM1
}ALG_TYPE;

typedef enum{				
	HASH_SM3=0,	
	HASH_SHA1,
	HASH_SHA224,
	HASH_SHA256
}HASH_TYPE;

#define NOK						0xFF
#define OK						0
//---------------------------------BCAE----------------
#define SYMALG_MODEIS				0x30
#define SYMALG_ECB					0x00
#define SYMALG_CBC					0x10

#define SYMALG_ENDECIS				0x40
#define SYMALG_SENC				0x00
#define SYMALG_SDEC				0x40

//#define HASH_SHA1					160
//#define HASH_SHA224				224
//#define HASH_SHA256				255

#define HASH_NEW					1
#define HASH_CONTINUE				0

//---------------------------------SSF33----------------


#define SSF33_ECB_ENCRYPT		0x00000000
#define SSF33_ECB_DECRYPT		0x00000002
#define SSF33_CBC_ENCRYPT		0x00000004
#define SSF33_CBC_DECRYPT		0x00000006

//----------------------------------END---------





#define ASYALG						0xcf
#define ASYALG_SM2					0xc1
#define ASYALG_RSA1024				0xc2
#define ASYALG_RSA2048				0xc3

#define RSA1024_LENGTH				128
#define RSA2048_LENGTH				256
#define RSA_LENGTH					256

#define ASYALG_ENDECIS				0x30
#define ASYALG_ASIGN				0x00
#define ASYALG_ADEC				0x10
#define ASYALG_AEXCH				0x20


typedef struct _RSA_CRT_Key
{
	unsigned int *pkn;
	unsigned int *pke;
	unsigned int *pku;
	unsigned int *pkdp;
	unsigned int *pkdq; 
	unsigned int *pkp; 
	unsigned int *pkq;
	unsigned int *pkd;
	unsigned int nLen; 	//unit:byte
	unsigned int eLen; 	//unit:byte
}RSA_CRT_Key;

typedef struct
{
	unsigned int *pE;
	unsigned int *pN;
	
}RSA_PUBLICKEY;


typedef struct
{
	unsigned int *pP;
	unsigned int *pQ;
	unsigned int *pDP;
	unsigned int *pDQ;
	unsigned int *pU;	
	unsigned int *pN;
}RSA_CRT_PRIVATEKEY;

typedef struct
{
	unsigned char selfpub[64];
	unsigned char selfpri[32];
	unsigned char selftmppub[64];
	unsigned char selftmppri[32];
	unsigned char otherpub[64];
	unsigned char othertmppub[32];
	unsigned char selfid[64];
	unsigned char otherid[64];
	unsigned char selfidlen;
	unsigned char otheridlen;
}SM2_KEYEXCHANGE;

typedef struct
{
	unsigned int selfpub[16];
	unsigned int selfpri[8];
	unsigned int selftmppub[16];
	unsigned int selftmppri[8];
	unsigned int otherpub[16];
	unsigned int othertmppub[16];
	unsigned int selfid[8];
	unsigned int selfidlen;
	unsigned int otherid[8];
	unsigned int otheridlen;
}SM2_KEYEXCHANGE1;

extern RSA_CRT_Key g_RsaKey;
extern RSA_PUBLICKEY g_rsapubkey;
extern RSA_CRT_PRIVATEKEY g_rsaprikey;
extern unsigned int *G_RSA_DATA_BUF;
extern SM2_KEYEXCHANGE *p_sm2_keyexchange;


unsigned char hal_alg_calmac(unsigned char *pbin,unsigned short len,unsigned char *pkey,unsigned char *piv,unsigned char *pmac,unsigned char algtype);

void hal_alg_genrand(unsigned char *buf, unsigned short len);

unsigned char hal_alg_bcae(unsigned char *pbin,
								unsigned int len,
								unsigned char *pkey,
								unsigned char *piv,
								unsigned char mode,
								unsigned char *pbout,
								unsigned char algtype);

void hal_alg_init(void);

#endif

