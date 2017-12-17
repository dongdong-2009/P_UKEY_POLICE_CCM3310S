#ifdef  LIB_SYMBOL
#define	LIB_EXT
#else	
#define	LIB_EXT extern
#endif

#define LOW_FORM 							0x0F
#define HIGH_FORM 							0xF0
#define HIGH_OF_16BIT						0xFF00
LIB_EXT int strlen(const char *StrDest);  
LIB_EXT  void memcpy(unsigned char *pDst,const unsigned char *pSrc, UINT16 cnt);
LIB_EXT  void *memset( void *dst, const char val, register unsigned len);
LIB_EXT  UINT8 memcmp(const void *s1, const void *s2, register unsigned len);
void * memmove(void * dest,const void *src,register unsigned count);

LIB_EXT void memcpy_special(UINT8 * pDst, const UINT8 * pSrc, UINT16 cnt);
LIB_EXT UINT8 Calculate_LRC(UINT8 *src, UINT16 len);

LIB_EXT void XorArrays(UINT8 *dest, UINT8* pbySrc1, UINT8* pbySrc2, UINT8 byLen);
LIB_EXT void wordswap(UINT32 *dest, UINT32 *src, UINT32 wordLen);
LIB_EXT BOOLEAN wordswap_u8(UINT8 *dest, UINT8 *src, UINT32 byteLen);



