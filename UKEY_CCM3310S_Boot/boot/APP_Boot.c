#include "APP_Boot.h"
#include "HAL_Flash.h"
#include "Global_Var.h"
#include "HAL_ALG.h"
#include "ccm3310s.h"

unsigned int APP_POS_CMD_ReadTerminalInfo(void)
{
	unsigned char revFlag = 0;
	unsigned int  back = 0;
	unsigned char DevInfo[0x25];
	unsigned char offset =0;
	unsigned char CardNoLen;
	unsigned char CardNo[20];
	
	if(g_APDU.PP2 == 0)
	{
		*(g_APDU.pOutData) = 0x1f;
		g_APDU.RLE = 1;
	}
	else if(g_APDU.PP2 == 1)
	{

		HAL_Flash_Read(DEV_INFO,0x25,DevInfo);

		//终端序列号
		memcpy(g_APDU.pOutData, DevInfo, 25);
		offset += 25;
		//终端固件版本号
		g_APDU.pOutData[offset] = 0x02;
		offset += 1;
		g_APDU.pOutData[offset] = strlen(COS_VER);
		offset += 1;
		memcpy((g_APDU.pOutData+offset), COS_VER, strlen(COS_VER));
		offset += strlen(COS_VER);
		memcpy((g_APDU.pOutData+offset), DEV_INFO+33, 4);
		offset +=4;
	

		CardNoLen= 16;
		memset(CardNo, 0x30, 16);

		*(g_APDU.pOutData+offset) = 4;	//IC卡卡号TAG
		offset++;
		*(g_APDU.pOutData+offset) =  CardNoLen;
		offset++;
		//revFlag  =  *(g_APDU.pOutData+53);
		memcpy(g_APDU.pOutData+offset,CardNo,CardNoLen);	
		offset += CardNoLen;
		*(g_APDU.pOutData + offset) = 5;
		offset++;
		*(g_APDU.pOutData + offset) = 1;
		offset++;
		//HAL_Flash_Read(REVERSAL_FLAG,1,&revFlag);
		revFlag = 0;
		*(g_APDU.pOutData + offset) = revFlag;
		offset++;
			
		*(g_APDU.pOutData+offset) = 6;
		offset++;
		*(g_APDU.pOutData+offset) = strlen(DEV_TYPE);
		offset++;
		memcpy((g_APDU.pOutData+offset), DEV_TYPE, strlen(DEV_TYPE));
		offset += strlen(DEV_TYPE);
						
		g_APDU.RLE = offset;
	
	}
	else
	{
		g_APDU.RLE = 0;
		return SW_P1P2_ERROR;
	}

	return SW_OPERATION_SUCCESS;		
}



void BOOT_INIT(void)
{
	unsigned short ver =0;
	unsigned char tmp[2];
							 
	HAL_Flash_Read(BOOTLOADER_INITFLAG_ADDR, BOOTLOADER_INITFLAG_ADDR_LEN, tmp);//读取初始化标识
	if(tmp[0]!= 0xaa)
	{
		HAL_Flash_Erase(BOOTLOADER_CONFG_ADDR, 0);
	 	//写bootloader 版本号
	 	ver = BOOTLOADER_VERSION;
		 tmp[0] = ver >>  8;
		 tmp[1] = ver;
		 HAL_Flash_Write(BOOTLOADER_VERSION_ADDR, BOOTLOADER_VERSION_ADDR_LEN, tmp);

		//写初始化表示
		tmp[0] = 0xaa;
		HAL_Flash_Write(BOOTLOADER_INITFLAG_ADDR, BOOTLOADER_INITFLAG_ADDR_LEN, tmp);
	}	
}

//读写flash最大长度为1024字节，1页
unsigned int CMD_BOOT_UPLOAD_DATA(void)
{
	unsigned int len =0;
	unsigned int flashaddr = 0;
	unsigned char rv;

	//addr is operated
	flashaddr += g_APDU.PP1  << 16;
	flashaddr +=  g_APDU.PP2<< 8;
	flashaddr +=  g_APDU.PP3;
	flashaddr += APP_FLASH_ADDR;//flash绝对地址0x0004000			
	//data length
	len =  g_APDU.LCH<< 8;
	len += g_APDU.LCL;	
	
	rv = HAL_Flash_Read(flashaddr, len, g_APDU.pOutData);

	if(!rv)
	{
		g_APDU.RLE = 0;
		return SW_MEMORY_ERROR;
	}
	else
	{
		g_APDU.RLE = len; 
		return SW_OPERATION_SUCCESS;
	}

	
}
// 2字节命令
// 3字节flash地址
// 2字节数据长度
unsigned int CMD_BOOT_DNLOAD_DATA(void)
{
	unsigned int len = 0;
	unsigned int flashaddr = 0;
	unsigned char rv;
	//addr is operated
	flashaddr += g_APDU.PP1  << 16;
	flashaddr +=  g_APDU.PP2<< 8;
	flashaddr +=  g_APDU.PP3;
	flashaddr += APP_FLASH_ADDR;//flash绝对地址0x8004000			
	//data length
	len =  g_APDU.LCH<< 8;
	len += g_APDU.LCL;	
	

	rv =HAL_Flash_Write(flashaddr, len, g_APDU.pInData);

	g_APDU.RLE = 0;

	if(!rv)
	{
		return SW_MEMORY_ERROR;
	}
	else
	{
		return SW_OPERATION_SUCCESS;
	}

}

unsigned int CMD_BOOT_DNLOAD_DATA_DES3(void)
{
	unsigned int len = 0;
	unsigned int flashaddr = 0;
	unsigned int i=0;
	unsigned char des3key[16];
	unsigned char rv;
	unsigned char tmp[512];
		
	//addr is operated
	flashaddr += g_APDU.PP1  << 16;
	flashaddr +=  g_APDU.PP2<< 8;
	flashaddr +=  g_APDU.PP3;
	flashaddr += APP_FLASH_ADDR;		
	//data length
	len =  g_APDU.LCH<< 8;
	len += g_APDU.LCL;	

	if(len%8)
	{
		return SW_P1P2_ERROR;
	}
	// DES 解密数据
	
	//读取密钥
	HAL_Flash_Read(BOOTLOADER_KEY_ADDR, BOOTLOADER_KEY_ADDR_LEN, des3key);

	rv = HAL_ALG_DESFix(g_APDU.pInData, len, des3key, NULL,SYMALG_DES128 | SYMALG_ECB | SYMALG_SDEC , g_APDU.pInData);
	rv=OK;

	if(rv != OK)
	{
		return SW_MEMORY_ERROR;
	}
	
	//计算MAC
	rv = HAL_ALG_DESFix(g_APDU.pInData, len, des3key, g_IV, SYMALG_DES64 | SYMALG_CBC | SYMALG_SENC , g_b_Buf);
	if(rv != OK)
	{
		return SW_MEMORY_ERROR;
	}
	
	memcpy(g_IV, g_b_Buf+len-8, 8);
	//  写入
	rv =HAL_Flash_Write(flashaddr, len, g_APDU.pInData);
	g_APDU.RLE = 0;
	if(!rv)
	{
		return SW_MEMORY_ERROR;
	}
	else
	{
		return SW_OPERATION_SUCCESS;
	}
}

unsigned int CMD_BOOT_ERASE_SECTOR(void)
{
	unsigned int len;
	unsigned int flashaddr = 0;
	unsigned char rv;
	//addr is operated
	flashaddr += g_APDU.PP1  << 16;
	flashaddr +=  g_APDU.PP2<< 8;
	flashaddr +=  g_APDU.PP3;
	flashaddr += APP_FLASH_ADDR;//flash绝对地址			
	//data length
	len =  g_APDU.LCL;

	rv = HAL_Flash_Erase(flashaddr, len); 

   	g_APDU.RLE =0;

	if(!rv)
	{
		return SW_MEMORY_ERROR;
	}
	else
	{
		return SW_OPERATION_SUCCESS;
	}
}

//可连续crc	
void boot_flashCRC(unsigned char *pbData, unsigned int len, unsigned int *crc)
{
   unsigned int b,s_s,i;
   unsigned int j;
   for(j=0;j<len;j++)
   {
	  s_s=(unsigned)pbData[j];
	  for(i=0;i<8;i++)
	  {
		 b=*crc&1;
		 if(s_s&1)
			b^=1;
		 if(b)
			  *crc^=0x4002;
		 *crc>>=1;
		 if(b)
			  *crc|=0x8000;
		 s_s>>=1;
	  }
   }
} 

//命令：2 bytes
//校验地址：3 bytes
//校验长度: 3bytes
//外CRC值 2 bytes
//返回值0x9000 crc校验相等，否则返回2字节CRC值
unsigned int CMD_BOOT_CRC_DATA(void)
{
	unsigned int crc =0, pccrc = 0;
	unsigned int len =0;
	unsigned int flashaddr = 0;
	unsigned int ret =0; 
	unsigned char TmpBuf[0x800];
	//addr is operated
	flashaddr += g_APDU.PP1  << 16;
	flashaddr +=  g_APDU.PP2<< 8;
	flashaddr +=  g_APDU.PP3;
	flashaddr += APP_FLASH_ADDR;//flash绝对地址			
	//data length
	len =  g_APDU.LCH<< 8;
	len += g_APDU.LCL;	

	
	pccrc = g_APDU.pInData[0] << 8;				//data
	pccrc += g_APDU.pInData[1] ;				//data

	HAL_Flash_Read(flashaddr, len, TmpBuf);
	boot_flashCRC(TmpBuf, len, &crc);
	
	if(crc != pccrc)
	{ 
		crc = 0x12fb; 
		*(g_APDU.pOutData) = (crc &0x0000ff00) >> 8; 
		*(g_APDU.pOutData+1) = crc;
	   	g_APDU.RLE = 2;		   	
		ret = SW_VERIFY_CRC_ERROR;	   			
	}
	else
	{ 
		g_APDU.RLE = 0;
	   	ret = SW_OPERATION_SUCCESS;	   
	} 
	return ret;
}


void Chip_Reset()
{
	RCR = 0x8000;
	return;
}


unsigned int CMD_BOOT_FLAG_DATA(void)
{
	unsigned int ret = 0;
	unsigned char uchFlag;
	
	switch(g_APDU.PP1)
	{
	case 00:								 
		HAL_Flash_Read(BOOTLOADER_FLAG_ADDR, BOOTLOADER_FLAG_ADDR_LEN, g_APDU.pOutData);
		g_APDU.RLE = BOOTLOADER_FLAG_ADDR_LEN;
		ret = SW_OPERATION_SUCCESS;
	break;
   	case 01:
   		//比较MAC值，写入
		if(memcmp(g_APDU.pInData, g_IV, 8))
		{
			g_APDU.RLE = 0;
			ret = SW_MAC_ERROR;
		}
   		else
   		{
		   	uchFlag = 0x55;
			HAL_Flash_Write(BOOTLOADER_FLAG_ADDR, BOOTLOADER_FLAG_ADDR_LEN, &uchFlag);
			g_APDU.RLE = 0;	
			ret = SW_OPERATION_SUCCESS;
		}
	break;
   	case 02:
	   	uchFlag = 0xFF;
		HAL_Flash_Write(BOOTLOADER_FLAG_ADDR, BOOTLOADER_FLAG_ADDR_LEN, &uchFlag);
		g_APDU.RLE = 0;
		ret = SW_OPERATION_SUCCESS;
	break;
	default:
		ret = SW_P1P2_ERROR;//------返回P1P2参数错误
		g_APDU.RLE = 0;
	}
	
	memset(g_IV, 0, 8);
			 
	return ret;
}

unsigned int CMD_BOOT_BOOT_DATA(void)
{
unsigned int ret = 0;
	
	switch(g_APDU.PP1)
	{
	case 00:								 
		HAL_Flash_Read(BOOTLOADER_VERSION_ADDR, BOOTLOADER_VERSION_ADDR_LEN, g_APDU.pOutData);
		g_APDU.RLE = BOOTLOADER_VERSION_ADDR_LEN;
		
		ret = SW_OPERATION_SUCCESS;
	break;
	default:
		ret = SW_P1P2_ERROR;//------返回P1P2参数错误
		g_APDU.RLE = 0;
	}
			 
	return ret;
}

unsigned int CMD_BOOT_KEY_DATA(void)
{
	unsigned int ret = 0;
	unsigned char uchFlag = 0;
	
	if((g_APDU.LCH != 0x00) || (g_APDU.LCL != 0x10))
	{
		g_APDU.RLE = 0;
		
		return SW_P1P2_ERROR;
	}
	
	switch(g_APDU.PP1)
	{
//	case 00:								 
//		HAL_Flash_Read(BOOTLOADER_KEY_ADDR, BOOTLOADER_KEY_ADDR_LEN, g_APDU.pOutData);
//		g_APDU.RLE = BOOTLOADER_KEY_ADDR_LEN;
//		ret = SW_OPERATION_SUCCESS;
//	break;
   	case 01:
   		HAL_Flash_Read(BOOTLOADER_KEY_FLAG_ADDR, BOOTLOADER_KEY_FLAG_ADDR_LEN, &uchFlag);
   		
   		if(uchFlag == 0xFF)
   		{
			HAL_Flash_Write(BOOTLOADER_KEY_ADDR, BOOTLOADER_KEY_ADDR_LEN, g_APDU.pInData);
			
			uchFlag = 0x55;			
			HAL_Flash_Write(BOOTLOADER_KEY_FLAG_ADDR, BOOTLOADER_KEY_FLAG_ADDR_LEN, &uchFlag);
			g_APDU.RLE = 0;
			ret = SW_OPERATION_SUCCESS;
		}
		else
		{
			g_APDU.RLE = 0;
			ret = SW_MAC_ERROR;
		}
		
	break;
	default:
		ret = SW_P1P2_ERROR;//------返回P1P2参数错误
		g_APDU.RLE = 0;
	}
			 
	return ret;
}


