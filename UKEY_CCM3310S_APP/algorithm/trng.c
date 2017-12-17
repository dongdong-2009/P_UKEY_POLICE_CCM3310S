/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:trng.c                             // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
#include "includes.h" 

#define   inp_w(addr)               *(volatile UINT32*)(addr)
#define   inp_h(addr)               *(volatile UINT16*)(addr)
#define   inp_b(addr)               *(volatile UINT8*)(addr)

#define   outp_w(addr, dat)         *(volatile UINT32*)(addr)=(dat)
#define   outp_h(addr, dat)         *(volatile UINT16*)(addr)=(dat)
#define   outp_b(addr, dat)         *(volatile UINT8*)(addr)=(dat)

//*-------------------------------------------------------------------------------------
//* 函数名称：void RSA_InitTrng()
//* 功能简介：随机数模块初始化
//* 输入参数：无
//* 输出参数：无
//* 返回值：  无

//*-------------------------------------------------------------------------------------
void InitTrng(void)
{
	//*(unsigned long *)(0xc30000+8) = 0;
	
    outp_w(TRNG_CSR, 0x00000504);             		//随机数产生使能
}

//*-------------------------------------------------------------------------------------
//* 函数名称：UINT32 GetRandomWord(void)
//* 功能简介：从随机数寄存器里取32 bit已经产生好的随机数
//* 输入参数：无
//* 输出参数：无
//* 返回值：  取的随机数值
//*-------------------------------------------------------------------------------------

UINT32 GetRandomWord(void)             
{    
    
     UINT32 ulRandomValue=0,ulRandomTmpOld,ulRandomTmpNew,i,j;

	ulRandomTmpNew = inp_w(TRNG_CSR);
	while((ulRandomTmpNew&0x00000800) == 0){
		ulRandomTmpNew = inp_w(TRNG_CSR);
	}
	ulRandomValue = inp_w(TRNG_DR);
	*(unsigned long *)TRNG_CSR |= 0x00000400;
	
    return ulRandomValue;  //返回取出的随机数
}

