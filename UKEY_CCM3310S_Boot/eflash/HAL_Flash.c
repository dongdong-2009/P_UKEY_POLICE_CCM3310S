#include "HAL_Flash.h"
#include "eflash_api.h"
#include "Global_Var.h"

void HAL_Flash_Init()
{
	//InitFlash();
}

unsigned char HAL_Flash_Read(unsigned int StartAddr, unsigned int ReadLen, unsigned char * puchDataBuff)
{		
	F_Read(StartAddr,puchDataBuff,ReadLen);
	return OK;
}
 
unsigned char HAL_Flash_Write(unsigned int StartAddr, unsigned int WriteLen, unsigned char * puchDataBuff)
{        

	F_Write(StartAddr,puchDataBuff,WriteLen);
	return OK; 
}

unsigned char HAL_Flash_Erase(unsigned int StartAddr, unsigned int EraseLen)
{
	unsigned int i=0;

	if(StartAddr%0x800)
	{
		return NOK;
	}
	
	
	for(i=0;i<4;i++)		
	EFLASH_Erase(StartAddr+i*0x200);
	
	return OK;
}
	

