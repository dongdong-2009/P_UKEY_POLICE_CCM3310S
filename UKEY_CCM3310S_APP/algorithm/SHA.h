#ifndef	_SHA_H_
#define	_SHA_H_

//*******Hash算法类型编号********//
#define		HASH_SM3_1		0
#define		HASH_SHA0_1		1
#define		HASH_SHA1_1		2
#define		HASH_SHA224_1		3
#define 	HASH_SHA256_1		4


//*******Hash算法数据结构********//
typedef	struct Hash_tx
{
	unsigned int  hash_len;	  //本次传送数据长度；	
	unsigned int  DataBuf[16]; //本次运算剩余未传到Register的数据
	unsigned char HASHMode;	  //Hash算法类型编号
	
}Hash_tx;

#endif