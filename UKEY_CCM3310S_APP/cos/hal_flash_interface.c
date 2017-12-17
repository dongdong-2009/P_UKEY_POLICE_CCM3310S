#include "hal_flash_data.h"
#include "hal_flash_interface.h"
#include "Eflash_api.h"

struct hw_flash_device flash_device;

unsigned char _hal_flash_init(void)
{
	//InitFlash();
}
//更新数据,不擦除flash,写之前必须保证数据区域是0xff
unsigned char _hal_flash_updatedata(unsigned int addr,unsigned char *pin,unsigned short len)
{
	if(addr<DATASTART||addr>(DATAEND-len))
		return FLASH_ERR_ADDR;
	
	F_Write(addr,pin,len);
	return 0;
}
unsigned char _hal_flash_writedata(unsigned int addr,unsigned char *pin,unsigned short len)
{
	if(addr<DATASTART||addr>(DATAEND-len))
		return FLASH_ERR_ADDR;
	F_Write(addr,pin,len);
		return 0;
}
unsigned char _hal_flash_readdata(unsigned int addr,unsigned char *pout,unsigned short len)
{
	F_Read(addr,pout,len);
}
unsigned char _hal_erasepage(unsigned int addr)
{
	unsigned int i=0;
	if(addr<DATASTART||addr>DATAEND)
		return FLASH_ERR_ADDR;
	for(i=0;i<4;i++)		
	EFLASH_Erase(addr+i*0x200);
	return 0;	
}
unsigned char _hal_updateflag(unsigned int addr,unsigned char status)
{
	unsigned char temp;
	temp=*(unsigned char *)addr;
	temp&=status;
	if(temp==status)//无需擦除		
		F_Write_u8(addr,status);
	else
		F_Write(addr,&status,1);
}

void HAL_Flash_Interface_Init(void)
{
	struct hw_flash_device  *p_flash_device;

	p_flash_device = &flash_device;
	p_flash_device->init=_hal_flash_init;
	p_flash_device->writedata=_hal_flash_writedata;
	p_flash_device->readdata=_hal_flash_readdata;
	p_flash_device->erasepage=_hal_erasepage;
	p_flash_device->updateflag=_hal_updateflag;
	p_flash_device->updatedata=_hal_flash_updatedata;

	p_flash_device->init();
}

