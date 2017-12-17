/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:main.c                             // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
#define  MAIN_SYMBOL
#include "includes.h"

/*
定义通信缓冲区: 
g_APDU_BUF指向APDU_DATA_ADDR地址
apdu_response指向RESPONSE_DATA_ADDR地址
目前APDU和response缓冲区大小各定义成2K，如果修改缓冲区大小，需要修改Linkfile文件的DATA段起始地址
*/
#define APDU_DATA_ADDR			(0x00800200)		//USB FIFO地址需为8字节整数倍
#define RESPONSE_DATA_ADDR		(0x00800A00)		//USB FIFO地址需为8字节整数倍
unsigned char g_reboot;

void InitGlobalVariable(void)
{
	g_APDU_BUF = (UINT8*)APDU_DATA_ADDR;
	apdu_response = (UINT8*)RESPONSE_DATA_ADDR;
	g_APDU_LEN = 0;
	g_reboot=0;
	

	//使能EMDAC、DMAC和SHA模块时钟
	*(volatile unsigned int *)(0x00c30004) &= ~0x300200;  //enable
	InitTrng();
	SM2_Enable();
	RsaModuleEnable();
	SM1_Enable();
	DES_Enable();
	AES_Enable();
	SMS4_Enable();
	SSF33_Enable();

	//读取SM1密钥
	F_Read(EFLASH_SM1_EK_ADDR, (UINT8*)g_sm1EK, EFLASH_SM1_EK_SIZE);
	F_Read(EFLASH_SM1_AK_ADDR, (UINT8*)g_sm1AK, EFLASH_SM1_AK_SIZE);
	F_Read(EFLASH_SM1_SK_ADDR, (UINT8*)g_sm1SK, EFLASH_SM1_SK_SIZE);
	F_Read(EFLASH_SM1_IV_ADDR, (UINT8*)g_sm1IV, EFLASH_SM1_IV_SIZE);

	//读取SM2签名用户ID
	g_useridLen = F_Read_u8(EFLASH_SM2_USERID_LENGTH_ADDR);
	F_Read(EFLASH_SM2_USERID_ADDR, (UINT8*)g_userid, EFLASH_SM2_USERID_SIZE);

	
	//读取DES密钥
	F_Read(EFLASH_DES_KEY_ADDR, (UINT8*)g_desKey, EFLASH_DES_KEY_SIZE);
	F_Read(EFLASH_DES_IV_ADDR, (UINT8*)g_desIV, EFLASH_DES_IV_SIZE);

	//读取AES密钥
	F_Read(EFLASH_AES_KEY_ADDR, (UINT8*)g_aesKey, EFLASH_AES_KEY_SIZE);
	F_Read(EFLASH_AES_IV_ADDR, (UINT8*)g_aesIV, EFLASH_AES_IV_SIZE);
	
	//读取SMS4密钥
	F_Read(EFLASH_SMS4_KEY_ADDR, (UINT8*)g_sms4Key, EFLASH_SMS4_KEY_SIZE);
	F_Read(EFLASH_SMS4_IV_ADDR, (UINT8*)g_sms4IV, EFLASH_SMS4_IV_SIZE);

	//读取SSF33密钥
	F_Read(EFLASH_SSF33_MK_ADDR, (UINT8*)g_ssf33MK, EFLASH_SSF33_MK_SIZE);
	F_Read(EFLASH_SSF33_SK_ADDR, (UINT8*)g_ssf33SK, EFLASH_SSF33_SK_SIZE);
	F_Read(EFLASH_SSF33_IV_ADDR, (UINT8*)g_ssf33IV, EFLASH_SSF33_IV_SIZE);
}


void interrupt_config(void)
{
	//配置中断
	Disable_Interrupts;	
	NIER_reg      = 0x0;               //禁能所有的中断
	ICR_reg       = 0;                 //采用向量中断，因默认采用自动向量中断，所以需要把中断模式调整为向量中断
	PLSR25_reg  = PRIORITY_LEVEL_4;    //USB中断，优先级设为4
	NIER_reg   |= 0x0010;              //使能优先级为4的中断使能	
	Enable_Interrupts;	
}


void Chip_Reset(unsigned char flag)
{
	unsigned int i;

	unsigned char buf[16];

	if(flag==1)
	{
		buf[0] = 0xff;
		F_Write( 0x00003803,buf, 1);		
		USB_DisConnect();
	}
		
	//for(i=0x50000;i>0;i--){};
	//
	asm
	{
		lrw r1, 0x200;
		jmp r1;	
	}	

	return;
}


void main()
{
	UINT8 ucISR;	
	UINT16 recvLen, sendLen;
	
	ModuleClk_On(MODULE_CLK_USBC|MODULE_CLK_CRYPTO|MODULE_CLK_TRNG|MODULE_CLK_SM1|MODULE_CLK_SHA1|MODULE_CLK_SPI1|MODULE_CLK_SPI2);	//打开CRYPTO/TRNG模块的时钟
    
	SetClk();

	//EnBootMode();

	
	set_clkd(g_sys_clk);

	InitGlobalVariable();

	//hal_RSAinit
	hal_RSAinit();	

	//sm2 init
	g_sm2Bits = 256;
	SM2Init(g_sm2Bits);
	
	//usb init		
	USBDev_Init();

	//interrupt init
	interrupt_config();

	//uart init
	ModuleClk_On(MODULE_CLK_SCI);	//打开SCI模块的时钟
//	UARTInit(g_sys_clk*1000,9600,TEN,ParityDIS,EVE);

	//flash interface init
	HAL_Flash_Interface_Init();

	//cos init
	cos_init(0);

//	HAL_UART_SendData("in main\r\n",sizeof("in main\r\n"));
	while(1)
	{
#ifndef USB_USEINT		//查询方式	
		gpram_msgflags1 |= gUSBC_ComReg->INTRTX_L;                 //取走USB的发送中断寄存器中的值
		ucISR = gUSBC_ComReg->INTRRX_L;                        //取走USB的接收中断寄存器中的值


		if(g_reboot==2)
		{
			Chip_Reset(1);
		}
		if(g_reboot==4)
		{
			Chip_Reset(0);
		}



		//EP1~EP7接收中断
		g_uchUSBRcvStatus = (ucISR&0xFE); 
		
		//EP0中断
		if(gpram_msgflags1 & 0x1)
		{
			if (gpram_USBAddrFlag == 1)                        //如果地址改写了
			{
				gUSBC_ComReg->FADDRR = gpram_USBNewAddr;   //把新地址放进去进行操作
				gpram_USBAddrFlag  = 0;
			}
			gpram_msgflags1 &= 0xFE;                           //清EP0中断标志		
			USBDev_EP0_ISR();
		}

		//Clear Last Completed Command		
		if(gpram_bulkPhase == 0xFF)
		{
			gpram_msgflags1 &= (~(1<<INDEX_EP1));
		}	

		//EP1~EP7发送中断
		if (gpram_msgflags1 & (1<<INDEX_EP1) )  
		{
			gpram_msgflags1 &= ( ~(1<<INDEX_EP1) );   //清EP1发送中断标志
			g_uchUSBSendStatus |= (1<<INDEX_EP1);					//连续发送多个包时，用来判断发送结束
		}
		
#endif		//#ifndef USB_USEINT	

		if (g_uchUSBRcvStatus & (1<<INDEX_EP1))     
		{
			g_uchUSBRcvStatus &= ~(1<<INDEX_EP1); 
			USBDev_EP1_RX_ISR();
		}

		if (g_uchUSBSendStatus & (1<<INDEX_EP1))
		{
			g_uchUSBSendStatus &= (~(1<<INDEX_EP1));
			USBDev_EP1_TX_ISR();	
		}
	}
}
						
				
						
		
