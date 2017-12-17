
#ifndef _RSA_H_
#define _RSA_H_


#define LESS_THAN 1
#define GREAT_THAN 2
#define EQUAL_THAN 0
/* RSA key lengths. */
#define MIN_RSA_MODULUS_BITS 508

#define MAX_RSA_MODULUS_BITS 2048
//#define MAX_RSA_MODULUS_BITS 1024
//#define MAX_RSA_MODULUS_BITS 512
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 31) / 32)
#define MAX_RSA_PRIME_BITS	((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN	((MAX_RSA_PRIME_BITS   + 31) / 32)

/* Maximum lengths of encoded and encrypted content, as a function of
	 content length len. Also, inverse functions. */

#define ENCODED_CONTENT_LEN(len) (4*(len)/3 + 3)
#define ENCRYPTED_CONTENT_LEN(len) ENCODED_CONTENT_LEN ((len)+8)
#define DECODED_CONTENT_LEN(len) (3*(len)/4 + 1)
#define DECRYPTED_CONTENT_LEN(len) (DECODED_CONTENT_LEN (len) - 1)


/* Maximum length of Diffie-Hellman parameters. */

#define DH_PRIME_LEN(bits) (((bits) + 7) / 8)

/* Error codes. */

#define RE_CONTENT_ENCODING 0x0400
#define RE_DATA 0x0401
#define RE_DIGEST_ALGORITHM 0x0402
#define RE_ENCODING 0x0403
#define RE_KEY 0x0404
#define RE_KEY_ENCODING 0x0405
#define RE_LEN 0x0406
#define RE_MODULUS_LEN 0x0407
#define RE_NEED_RANDOM 0x0408
#define RE_PRIVATE_KEY 0x0409
#define RE_PUBLIC_KEY 0x040a
#define RE_SIGNATURE 0x040b
#define RE_SIGNATURE_ENCODING 0x040c
#define RE_ENCRYPTION_ALGORITHM 0x040d
#define RE_FILE 0x040e



/* Internal Error Codes */


#define ID_OK    0
#define ID_ERROR 1

/* Internal defs. */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif



/* Random structure. */

typedef struct {
  unsigned int bytesNeeded;                    /* seed bytes required */
  unsigned char state[16];                     /* state of object */
  unsigned int outputAvailable;                /* number byte available */
  unsigned char output[16];                    /* output bytes */
} R_RANDOM_STRUCT;


//=================================================================================================
//大数存储方式
//Addr |  0  |  1  |  2 |  3|  4  |  5  |  6  |  7  |  9  |  10 | 11  |  12 |.........
//Word |        0           |         1             |          2            |.........   
//Bit  |31-24|23-16|15-8|7-0|63-56|55-48|47-40|39-32|95-88|87-80|79-72|71-64|.........


/* RSA public and private key. */

// 公钥
typedef struct {  
  unsigned int bits;                     /* length in bits of modulus */
  unsigned int modulus[MAX_RSA_MODULUS_LEN];  /* modulus */
  unsigned int exponent[MAX_RSA_MODULUS_LEN]; /* public exponent */
} STU_RSA_PUBKEY;

// 私钥
typedef struct {
  unsigned int bits;                     /* length in bits of modulus */
  unsigned int modulus[MAX_RSA_MODULUS_LEN];  /* modulus */
  unsigned int publicExponent[MAX_RSA_MODULUS_LEN];     /* public exponent */
  unsigned int exponent[MAX_RSA_MODULUS_LEN]; /* private exponent */
  unsigned int prime[2][MAX_RSA_PRIME_LEN];   /* prime factors *///pq
  unsigned int primeExponent[2][MAX_RSA_PRIME_LEN];     /* exponents for CRT */
  unsigned int coefficient[MAX_RSA_PRIME_LEN];//inv          /* CRT coefficient */
} STU_RSA_PRIVKEY;

/* RSA prototype key. */

typedef struct {
  unsigned int bits;                           /* length in bits of modulus */
  int useFermat4;                              /* public exponent (1 = F4, 0 = 3) */
} STU_RSA_KEY;


//=================================================================================================
// RSA 密钥对生产
//unsigned char mode;        /* 0 -- 1024bits;1 -- 2048bits */
//STU_RSA_PUBKEY * pub_key,  /* 公钥 */
//STU_RSA_PRIVKEY * prv_key, /* 私钥 */
//unsigned char fixkey,      /* 0 -- 公钥随机生产, 1 -- 公钥固定为0x10001 */    
//unsigned char crt,         /* 0 -- 密钥格式为普通格式 1 -- 密钥格式为CRT格式 */
//unsigned char RsaGenerateKeyPair( unsigned char mode, STU_RSA_PUBKEY * pub_key, STU_RSA_PRIVKEY * prv_key,unsigned char fixkey,unsigned char crt  );
unsigned char RsaGenerateKeyPair( unsigned int mode, STU_RSA_PUBKEY * pub_key, STU_RSA_PRIVKEY * prv_key,unsigned char fixkey,unsigned char crt  );

//=================================================================================================
// RSA 模块使能
void RsaModuleEnable();


//=================================================================================================
//RSA 模块关闭
void RsaModuleDisable();

//=================================================================================================
//RSA 公钥运算
//unsigned int *output;          /* 输出数据 */
//unsigned int *outputLen;        /* 输出数据长度 */
//unsigned int *input;           /* 输入数据 */
//unsigned int inputLen;          /* 输入数据长度 */
//STU_RSA_PUBKEY *publicKey;      /* 公钥 */
int RsaPubKey(unsigned int *output, unsigned int *outputLen,
				  unsigned int *input,unsigned int  inputLen, STU_RSA_PUBKEY *publicKey);



//=================================================================================================
//RSA 普通模式私钥运算 
//unsigned int *output;          /* 输出数据 */
//unsigned int *outputLen;        /* 输出数据长度 */
//unsigned int *input;           /* 输入数据 */
//unsigned int inputLen;           /* 输入数据长度 */
//STU_RSA_PRIVKEY *privateKey;     /* 私钥 */
int RsaPrivKey(unsigned int *output, unsigned int *outputLen, 
				unsigned int *input, unsigned int inputLen, 
				STU_RSA_PRIVKEY * privateKey);


//=================================================================================================
//RSA CRT模式私钥运算 
//unsigned int *output;          /* 输出数据 */
//unsigned int *outputLen;        /* 输出数据长度 */
//unsigned int *input;           /* 输入数据 */
//unsigned int inputLen;           /* 输入数据长度 */
//STU_RSA_PRIVKEY *privateKey;     /* 私钥 */
int RsaPrivKeyCRT(unsigned int *output, unsigned int *outputLen, 
				unsigned int *input, unsigned int inputLen, 
				STU_RSA_PRIVKEY * pprivateKey);
#endif /* _RSA_H_ */
