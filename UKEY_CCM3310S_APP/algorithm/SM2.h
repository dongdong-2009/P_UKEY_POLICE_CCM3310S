/*(c) Copyright 2003-2009，C*Core rights reserved */

//*****************************************************************************
//*\文件名称ecc.h
//*\摘要：    ecc相关的数据结构声明及函数声明
//*\当前版本：Ver0.1

//******************************************************************************
#ifndef	__ECC_H__
#define	__ECC_H__

#ifndef bool
#define bool int
#endif

/*(c) Copyright 2003-2009，C*Core rights reserved */

//*****************************************************************************
//*\文件名称bigInt.h
//*\摘要：    32位大数相关的数据结构声明及函数声明
//*\当前版本：Ver0.1
//******************************************************************************
#ifndef	__BIGINT_H__
#define	__BIGINT_H__


//大数运算相关的数据结构定义与运算函数
#define ECC_BIGINT32_MAXLEN 	8

#define write_port_byte(port,adata) (*(volatile unsigned char *)(port))=((unsigned char)(adata))
#define read_port_byte(port)        (*(volatile unsigned char *)(port))

#define write_port_halfword(port,adata) (*(volatile unsigned short *)(port))=((unsigned short)(adata))
#define read_port_halfword(port)		(*(volatile unsigned short *)(port))

#define write_port_word(port,adata) (*(volatile unsigned long *)(port))=((unsigned long)(adata))
#define read_port_word(port)		(*(volatile unsigned long *)(port))

typedef unsigned char   UINT8;
typedef signed   char   SINT8;
typedef unsigned short  UINT16;
typedef signed   short  SINT16;
typedef unsigned int    UINT32;
typedef signed 	 int    SINT32;

typedef UINT32 BIG_INT;
typedef UINT16 BIG_HALF_DIGIT;

//定义大数的32位数组结构
//auValue数组的低32位地址存放大数的低32位有效数，高32位地址存放大数的高32位有效数  
typedef struct 
{
 	UINT16   uLen;                         //32位数组的实际长度
	UINT32   auValue[ECC_BIGINT32_MAXLEN]; //32位数组的值
}ECC_STU_BIGINT32;



/* Length of digit in bits */
#define BIG_LEN 32
#define BIG_HALF_DIGIT_BITS 16
/* Length of digit in bytes */
#define BIG_DIGIT_LEN (BIG_LEN / 8)

/* Maximum length in digits */
#define MAX_BIG_LEN MAX_RSA_MODULUS_LEN

/* Maximum digits */
#define MAX_BIG_DIGIT 0xffffffff
#define MAX_BIG_HALF_DIGIT 0xffff

#define BIG_LT   -1
#define BIG_EQ   0
#define BIG_GT 1

#define LOW_HALF(x) ((x) & MAX_BIG_HALF_DIGIT)
#define HIGH_HALF(x) (((x) >> BIG_HALF_DIGIT_BITS) & MAX_BIG_HALF_DIGIT)
#define TO_HIGH_HALF(x) (((BIG_INT)(x)) << BIG_HALF_DIGIT_BITS)
#define DIGIT_MSB(x) (unsigned int)(((x) >> (BIG_LEN - 1)) & 1)
#define DIGIT_2MSB(x) (unsigned int)(((x) >> (BIG_LEN - 2)) & 3)

//*----------------------------------------------------------------------------
//* \函数名称：void EccBig32Initial(ECC_STU_BIGINT *pstuA)
//* \功能简介：大数的初始化。数组值全赋值为0，数组长度记为1
//* \输入参数：无
//* \输出参数：pstuA：指向结构体ECC_STU_BIGINT的指针，初始化后的pstuA
//* \返回值  ：无

//*----------------------------------------------------------------------------
void EccBig32Initial(ECC_STU_BIGINT32 *pstuA);

//*----------------------------------------------------------------------------
//* \函数名称：void EccByBufToBigInt32(UINT8 *pbyInBuf,UINT16 uInLen, ECC_STU_BIGINT32 *pstuOutBigInt)
//* \功能简介：输入无符号8位整型数组转换为结构体ECC_STU_BIGINT32的大数格式          
//* \          例如 长度为8的数据 pbyOutBuf[0]=0x55;
//* \							  pbyOutBuf[1]=0x66;
//* \							  pbyOutBuf[2]=0x77;
//* \							  pbyOutBuf[3]=0x88;
//* \                             pbyOutBuf[4]=0x11;
//* \							  pbyOutBuf[5]=0x22;
//* \							  pbyOutBuf[6]=0x33;
//* \							  pbyOutBuf[7]=0x44;
//* \        转化为 长度为2的大数 pstuInBigInt->auValue[0]=0x11223344;
//* \   				          pstuInBigInt->auValue[1]=0x55667788;
//* \输入参数：pbyInBuf：指向无符号8位整型数组首地址的指针
//* \          uInLen：无符号8位整型数组的长度
//* \输出参数：pstuOutBigInt：指向结构体ECC_STU_BIGINT32的指针，输出转换后的对应大数
//* \返回值  ：无

//*----------------------------------------------------------------------------
void EccByBufToBigInt32(UINT8 *pbyInBuf,UINT16 uInLen, ECC_STU_BIGINT32 *pstuOutBigInt);



//*----------------------------------------------------------------------------
//* \函数名称：void EccBig32IntToByBuf(ECC_STU_BIGINT32 *pstuInBigInt, UINT8 *pbyOutBuf, UINT16 *puOutLen)
//* \功能简介：输入结构体ECC_STU_BIGINT32的大数格式转换为无符号8位整型数组  
//* \          例如 长度为2的大数 pstuInBigInt->auValue[0]=0x11223344;
//* \   				          pstuInBigInt->auValue[1]=0x55667788;
//* \          转化为             pbyOutBuf[0]=0x55;
//* \							  pbyOutBuf[1]=0x66;
//* \							  pbyOutBuf[2]=0x77;
//* \							  pbyOutBuf[3]=0x88;
//* \                             pbyOutBuf[4]=0x11;
//* \							  pbyOutBuf[5]=0x22;
//* \							  pbyOutBuf[6]=0x33;
//* \							  pbyOutBuf[7]=0x44;
//* \输入参数：pstuInBigInt：指向结构体ECC_STU_BIGINT32的指针
//* \输出参数：pbyOutBuf：指向无符号8位整型数组首地址的指针，输出转换后的对应数组
//* \          puOutLen：无符号8位整型数组的长度
//* \返回值  ：无

//*----------------------------------------------------------------------------
void EccBig32IntToByBuf(ECC_STU_BIGINT32 *pstuInBigInt, UINT8 *pbyOutBuf, UINT16 *puOutLen);

#endif  //__BIGINT_H__


//------------------------------------------------------------

//定义ECC参数的格式
typedef struct 
{
    UINT16 uBits;                               // 模数P的位数
	ECC_STU_BIGINT32  stuPrimeP;				// 模数P
	ECC_STU_BIGINT32  stuCoefficientA;			// 椭圆系数A	
	ECC_STU_BIGINT32  stuCoefficientB;			// 椭圆系数B		
	ECC_STU_BIGINT32  stuGx;				    // 椭圆基点坐标Gx
	ECC_STU_BIGINT32  stuGy;				    // 椭圆基点坐标Gy
	ECC_STU_BIGINT32  stuPrimeN;				// 椭圆基点坐标G的阶	
} SM2_STU_PRAMS;


//定义ECC公钥的格式
typedef struct 
{
	ECC_STU_BIGINT32  stuQx;			        // 公钥坐标Qx	
	ECC_STU_BIGINT32  stuQy;				    // 公钥坐标Qy
} SM2_STU_PUBKEY;

#endif

