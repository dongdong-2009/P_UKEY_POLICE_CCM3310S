#ifndef __HAL_FLASH_H
#define __HAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif 

void HAL_Flash_Init(void);
unsigned char HAL_Flash_Read(unsigned int StartAddr, unsigned int ReadLen, unsigned char * puchDataBuff);
unsigned char HAL_Flash_Write(unsigned int StartAddr, unsigned int WriteLen, unsigned char * puchDataBuff);
unsigned char HAL_Flash_Erase(unsigned int StartAddr, unsigned int EraseLen);

#ifdef __cplusplus
}
#endif
#endif //__HAL_FLASH_H