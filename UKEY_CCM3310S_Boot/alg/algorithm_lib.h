
//打开宏，使用ROM里面的算法库，关闭宏，则使用源码库
//#define ROM_CODE_LIB

#ifdef ROM_CODE_LIB

#define EFLASH_program 		(*(unsigned char (*)(unsigned int, unsigned int))(((unsigned int *)(0x00403CD0))))
#define EFLASH_page_erase	(*(unsigned char (*)(unsigned int)) (((unsigned int *)(0x00403DC8))))
#define EFLASH_mase_erase	(*(void (*)(unsigned int)) (((unsigned int *)(0x00403E84))))

#define EDMAC_TransData	(*(void (*)(unsigned int , unsigned int , unsigned int , unsigned int ))(((unsigned int *)(0x00401C50))))

#define SHA_Init	(*(void (*)(Hash_tx *,unsigned char ))(((unsigned int *)(0x00401D80))))
#define SHA_Update	(*(void (*)(Hash_tx *,unsigned char *, unsigned int ))(((unsigned int *)(0x00401DA8))))
#define SHA_Final	(*(unsigned int (*)(Hash_tx *,unsigned int *))(((unsigned int *)(0x00401EFC))))
#define HashCalc	(*(unsigned int (*)(unsigned char , unsigned int *, unsigned int , unsigned int *))(((unsigned int *)(0x00401FB4))))

#define SM2GenerateKeyPair	(*(unsigned char (*)(SM2_STU_PRAMS *,ECC_STU_BIGINT32 *,SM2_STU_PUBKEY *))(((unsigned int *)(0x0040308C))))
#define SM2Encrypt_V1			(*(void (*)(unsigned char *,unsigned short ,SM2_STU_PUBKEY *,SM2_STU_PRAMS *,unsigned char *))(((unsigned int *)(0x00403778))))
#define SM2Decrypt_V1			(*(unsigned char (*)(unsigned char *,unsigned short ,ECC_STU_BIGINT32 *,SM2_STU_PRAMS *,unsigned char *))(((unsigned int *)(0x0040379C))))
#define SM2Encrypt_V2			(*(void (*)(unsigned char *,unsigned short ,SM2_STU_PUBKEY *,SM2_STU_PRAMS *,unsigned char *))(((unsigned int *)(0x004037C8))))
#define SM2Decrypt_V2			(*(unsigned char (*)(unsigned char *,unsigned short ,ECC_STU_BIGINT32 *,SM2_STU_PRAMS *,unsigned char *))(((unsigned int *)(0x004037EC))))
#define SM2Signature 		(*(void (*)(unsigned char *,unsigned short ,SM2_STU_PRAMS *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *))(((unsigned int *)(0x00403818))))
#define SM2Verification	    (*(unsigned char (*)(unsigned char *,unsigned short ,SM2_STU_PRAMS *,SM2_STU_PUBKEY *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *))(((unsigned int *)(0x00403AE8))))

//ZA = hash(ENTLA || IDA || a || b || xg || yg || xa || ya) 
#define sign	    (*(void (*)(unsigned char *,unsigned short ,SM2_STU_PUBKEY *,SM2_STU_PRAMS *,unsigned int *))(((unsigned int *)(0x0040203C))))

/*运算法则： pstuC = (pstuA + pstuE) mod pstuN */
#define EccBigModAddBig	(*(void (*)(unsigned int *,unsigned int *,unsigned int *,unsigned int *,unsigned char ))(((unsigned int *)(0x004023BC))))
/*运算法则： pstuC = (pstuA - pstuE) mod pstuN */
#define EccBigModSubBig	(*(void (*)(unsigned int *,unsigned int *,unsigned int *,unsigned int *,unsigned char ))(((unsigned int *)(0x00402494))))
/*运算法则： pstuC = (pstuA * pstuE) mod pstuN */
#define EccBigModMulBig	(*(void (*)(unsigned int *,unsigned int *,unsigned int *,unsigned int *,unsigned char ))(((unsigned int *)(0x0040256C))))
/*运算法则： pstuC = (1/pstuA) mod pstuN */
#define EccBigExtEucBig	(*(void (*)(unsigned int *,unsigned int *,unsigned int *,unsigned char ))(((unsigned int *)(0x00402628))))
/*运算法则： pointC = (pointA + pointB) */
#define EccPointAdd		(*(void (*)(ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,SM2_STU_PRAMS *))(((unsigned int *)(0x004027A4))))
/*运算法则： pointB = (K * pointA) */
#define EccPointMul		(*(void (*)(ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,ECC_STU_BIGINT32 *,SM2_STU_PRAMS *))(((unsigned int *)(0x00402AF8))))

#define GetRandomWord		(*(unsigned int (*)(void))(((unsigned int *)(0x00403CAC))))

#else

#define			CRYPTO_BASE		0x00c80000

#define CRYPT0_CACR  (CRYPTO_BASE + 0x000)
#define CRYPT0_CASR  (CRYPTO_BASE + 0x004)
#define CRYPT0_CAAR  (CRYPTO_BASE + 0x008)
#define CRYPT0_CAER  (CRYPTO_BASE + 0x108)
#define CRYPT0_CANR  (CRYPTO_BASE + 0x208)

#define CRYPT0_CASR_DONE	0x01

#define CRYPT0_WRITE_CMD(cmd)		((*(UINT32 *)CRYPT0_CACR) = (cmd))

#define CACR  *(volatile unsigned int *)(CRYPTO_BASE + 0x000)
#define CASR  *(volatile unsigned int *)(CRYPTO_BASE + 0x004)
#define CAAR  *(volatile unsigned int *)(CRYPTO_BASE + 0x008)
#define CAER  *(volatile unsigned int *)(CRYPTO_BASE + 0x108)
#define CANR  *(volatile unsigned int *)(CRYPTO_BASE + 0x208)

#define BAR0     	(*(volatile UINT32 *)(0x00d80000)) 
#define BAR1     	(*(volatile UINT32 *)(0x00d80004)) 
#define PAR0     	(*(volatile UINT32 *)(0x00d80008)) 
#define PAR1     	(*(volatile UINT32 *)(0x00d8000c)) 
#define GCR			(*(volatile UINT16 *)(0x00d80010))
#define DSR     	(*(volatile UINT16 *)(0x00d80012)) 
#define BCR0     	(*(volatile UINT32 *)(0x00d80014)) 
#define BCR1     	(*(volatile UINT32 *)(0x00d80018)) 
#define TCR0     	(*(volatile UINT32 *)(0x00d8001c)) 
#define TCR1     	(*(volatile UINT32 *)(0x00d80020)) 
#define DCR0     	(*(volatile UINT32 *)(0x00d80024)) 
#define DCR1     	(*(volatile UINT32 *)(0x00d80028)) 

#define DMACEN    0x0001

#endif

/*****************************库函数声明****************************************************************
**SM1:
	void InitSM1(unsigned char EncDe_Mode, unsigned char mode, unsigned char sksel);
	void SM1KeyInput(unsigned int *EK,unsigned int *AK,unsigned int *SK, unsigned char sksel);
	void SM1IVInput(unsigned int *IV);
	void SM1_Crypt(unsigned int *src, unsigned int *dst, unsigned int datalen);
**SM2:
	void SM2Init(unsigned short bits);
	UINT8 SM2GenerateKeyPair (SM2_STU_PRAMS *pstuSM2Prams,SM2_STU_BIGINT32 *pstuPrivKey,SM2_STU_PUBKEY *pstuPbuKey);
	void SM2Encrypt_V1(unsigned char *mes,unsigned short klen,SM2_STU_PUBKEY *pstuPubKey,SM2_STU_PRAMS *pstuEccPrams,unsigned char *cipher);
	UINT8 SM2Decrypt_V1(unsigned char *cipher,unsigned short klen,ECC_STU_BIGINT32 *stuPrivKey,SM2_STU_PRAMS *pstuEccPrams,unsigned char *plain);
	void SM2Encrypt_V2(unsigned char *mes,unsigned short klen,SM2_STU_PUBKEY *pstuPubKey,SM2_STU_PRAMS *pstuEccPrams,unsigned char *cipher);
	UINT8 SM2Decrypt_V2(unsigned char *cipher,unsigned short klen,ECC_STU_BIGINT32 *stuPrivKey,SM2_STU_PRAMS *pstuEccPrams,unsigned char *plain);
	void SM2Signature(unsigned char *mes,unsigned short klen,SM2_STU_PRAMS *pstuEccPrams,ECC_STU_BIGINT32 *pstuPrivKey,ECC_STU_BIGINT32 *pSignatureR,ECC_STU_BIGINT32 *pSignatureS);
	UINT8 SM2Verification(unsigned char *mes,unsigned short klen,SM2_STU_PRAMS *pstuEccPrams,SM2_STU_PUBKEY *pstuPubKey,ECC_STU_BIGINT32 *pSignatureR,ECC_STU_BIGINT32 *pSignatureS);
	void SM2SignatureWithIDA(unsigned char *mes,unsigned short klen,unsigned char *IDA,unsigned short entla,SM2_STU_PRAMS *pstuEccPrams,SM2_STU_PUBKEY *pstuPubKey,ECC_STU_BIGINT32 *pstuPrivKey,ECC_STU_BIGINT32 *pSignatureR,ECC_STU_BIGINT32 *pSignatureS);
	UINT8 SM2VerificationWithIDA(unsigned char *mes,unsigned short klen,unsigned char *IDA,unsigned short entla,SM2_STU_PRAMS *pstuEccPrams,SM2_STU_PUBKEY *pstuPubKey,ECC_STU_BIGINT32 *pSignatureR,ECC_STU_BIGINT32 *pSignatureS);
	void sign(unsigned char *IDA,unsigned short entla,SM2_STU_PUBKEY *pstuPubKey,SM2_STU_PRAMS *pstuEccPrams,unsigned int *za);
	void EccBigModAddBig(unsigned int *pstuA,unsigned int *pstuE,unsigned int *pstuN,unsigned int *pstuC,UINT8 index);  
	void EccBigModSubBig(unsigned int *pstuA,unsigned int *pstuE,unsigned int *pstuN,unsigned int *pstuC,UINT8 index);  
	void EccBigModMulBig(unsigned int *pstuA,unsigned int *pstuE,unsigned int *pstuN,unsigned int *pstuC,UINT8 index);  
	void EccBigExtEucBig(unsigned int *pstuA,unsigned int *pstuN,unsigned int *pstuC,unsigned char index);  
	void EccPointAdd(ECC_STU_BIGINT32 *pPointAx,ECC_STU_BIGINT32 *pPointAy,ECC_STU_BIGINT32 *pPointBx,ECC_STU_BIGINT32 *pPointBy,ECC_STU_BIGINT32 *pPointCx,ECC_STU_BIGINT32 *pPointCy,SM2_STU_PRAMS *pstuEccPrams);
	void EccPointMul(ECC_STU_BIGINT32 *pStuK,ECC_STU_BIGINT32 *pPointAx,ECC_STU_BIGINT32 *pPointAy,ECC_STU_BIGINT32 *pPointBx,ECC_STU_BIGINT32 *pPointBy,SM2_STU_PRAMS *pstuEccPrams);
**SM3:
	void SHA_Init(Hash_tx *phash_tx,unsigned char HashModeNum);
	void SHA_Update(Hash_tx *phash_tx,unsigned char *pMessageBlock, unsigned int DataLen);
	unsigned int SHA_Final(Hash_tx *phash_tx,unsigned int *pRes);
	unsigned int HashCalc(unsigned char HashMode, unsigned int *pData, unsigned int len, unsigned int *pRes);
**SM4:
	void InitSMS4(unsigned char cmode, unsigned char cbc_ecb_mode);
	void SMS4KeyInput(unsigned int *KEY);
	void SMS4IVInput(unsigned int *IV);
	void SMS4_Crypt(unsigned int *src, unsigned int *dst, unsigned int datalen);
**SSF33:
	void InitSSF33(unsigned char EncDe_Mode, unsigned char mode, unsigned char sksel);
	void SSF33KeyInput(unsigned int *MK, unsigned int *SK, unsigned char flag);
	void SSF33IVInput(unsigned int *IV);
	void SSF33_Crypt(unsigned int *src, unsigned int *dst, unsigned int datalen);
**DES:
	U8 DES_KeyInit(U8 EncDe_Mode, U8 ECB_CBC_Mode, U32 *Key, U32 KeyLen, U32 *ivbuffer, U8 check_parity);
	void DES_Crypt(U32 *srcbuffer, U32 *Output, U32 datalen);
**AES:
	void AES_KeyInit(U8 EN_DE_MODE, U8 CTR_ECB_MODE, U32 *key, U32 *cntIV, U32 key_mode);
	void AES_Crypt(U32 *srcbuffer, U32 *Output, U32 datalen);
*************************************************************************************************************/
