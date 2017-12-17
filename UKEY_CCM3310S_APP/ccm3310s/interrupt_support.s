/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:interrupt_support.s                // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/
    .file   "interrupt_support.s"
    .text
    
######################################################################
# Control register read routines
######################################################################
    .export Read_PSR, Read_VBR

Read_PSR:
    mfcr    r2,cr0
    rts

Read_VBR:
    mfcr    r2,cr1
    rts

######################################################################
# Control register write routines
######################################################################
    .export Write_PSR, Write_VBR

Write_PSR:
    mtcr    r2,cr0
    rts

Write_VBR:
    mtcr    r2,cr1
    rts

##########################################################################
# Disable_Ints function (disables interrupts by clearing PSR.IE and PSR.FE
##########################################################################
    .export Disable_Ints
Disable_Ints:
    mfcr    r1, cr0		# get PSR
    bclri   r1, 0x4		# disable fast interrupts
    bclri   r1, 0x6		# disable normal interrupts
    mtcr    r1, cr0		# make it so by stashing it back into PSR
    jmp     r15

##########################################################################
# Enable_Ints function (enables interrupts by setting PSR.IE and PSR.FE
##########################################################################
    .export Enable_Ints
Enable_Ints:
    mfcr    r1, cr0		# get PSR	
    bseti   r1, 0x4		# enable fast interrupts
    bseti   r1, 0x6		# enable normal interrupts
    mtcr    r1, cr0		# put changes back into PSR
    jmp     r15

