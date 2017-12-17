#ifndef	__HAL_FLASH_DATA_H__
#define	__HAL_FLASH_DATA_H__

#define FLASH_ERR_ADDR		1	//地址参数错误
#define FLASH_ERR_LEN		2	//长度参数错误
/*
根据使用的芯片的不同,定义芯片的FLASH配置
*/
#define  	FLASH_PAGE_SIZE			0x0800

#define  	FLASH_PRINT_ADDR			0x02F000//指纹算法空间
#define  	FLASH_END_ADDR			0x040000

#define  	SECTOR_SIZE				0x400//0x0400
#define  	SECTOR_ADDR_LOW			(SECTOR_SIZE-1)
#define  	SECTOR_ADDR_HI			(0xffffffff-SECTOR_ADDR_LOW)
//COS空间配置信息
#define 	DATASTART				0x1C000				//片内存储空间起始地址

#define DEV_VER_INFO 			DATASTART
#define DEV_COS_INIT			(DEV_VER_INFO+FLASH_PAGE_SIZE)
#define DEV_AUTH_KEY 			(DEV_COS_INIT+FLASH_PAGE_SIZE)
#define PRI_SECTOR_INFO			(DEV_AUTH_KEY+FLASH_PAGE_SIZE)				//主扇区信息位置
#define SEC_SECTOR_INFO		(PRI_SECTOR_INFO+FLASH_PAGE_SIZE)	//备份扇区位置
// 1字节状态+16字节认证秘钥+1字节重试次数+1字节当前剩余次数


//#define	
#define 	MFSTART 				0x1E800				//MF起始，即COS计时位置
#define 	MFEND					0x2E800				//COS结束位置
#define	COS_SIZE				(MFEND-MFSTART)//		//COS大小64K

#define  	SECTOR_NUM			(COS_SIZE / SECTOR_SIZE)
//#define 	SECTOU_LIST_SIZE		((SECTOR_NUM+7)/8)
#define 	DATAEND				0x2E800				//片内存储空间结束地址
#define	TOTAL_DATA_PAGE		(DATAEND-DATASTART)/FLASH_PAGE_SIZE
#endif

