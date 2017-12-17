/*All rights reserved */
/*************************************************
  	  Copyright (C), 2010-2013, C*Core Technology Co., Ltd
  File name:ccm3310s.h                         // 文件名
  Author:       Version:1.0     Date:          // 作者、版本及完成日期
  Description:                                 // 用于详细说明此程序文件完成的主要功能
  Others:                                      // 其它内容的说明
  History:                                     // 修改历史记录列表，每条修改记录应包括修改日期、修改
                                               // 者及修改内容简述  
    1. Date:         2013-03-13
       Author:       徐凯
       Modification: 按照《天津国芯科技有限公司产品应用部C语言编程规范》V1.0标准化
/*************************************************/

/*********************************************************************************************************************/
#define write_port_byte(port,adata)	                 (*(volatile unsigned char *)(port))=((unsigned char)(adata))
#define read_port_byte(port)		                 (*(volatile unsigned char *)(port))

#define write_port_halfword(port,adata)	             (*(volatile unsigned short *)(port))=((unsigned short)(adata))
#define read_port_halfword(port)	                 (*(volatile unsigned short *)(port))

#define write_port_word(port,adata)	                 (*(volatile unsigned long *)(port))=((unsigned long)(adata))
#define read_port_word(port)		                 (*(volatile unsigned long *)(port))
/*********************************************************************************************************************/


/*********************************************************************************************************************/
typedef unsigned char  			BOOLEAN;
typedef unsigned char           UINT8;
typedef signed char             INT8;
typedef unsigned short int      UINT16;
typedef signed short int        INT16;
typedef unsigned int            UINT32;
typedef signed int              INT32;
typedef float          		    FP32;
typedef double         		    FP64;


#define TRUE                    0x01
#define FALSE                   0x00
/*********************************************************************************************************************/


/*********************************************************************************************************************/
#define bit31  0x80000000
#define bit30  0x40000000
#define bit29  0x20000000
#define bit28  0x10000000
#define bit27  0x08000000
#define bit26  0x04000000
#define bit25  0x02000000
#define bit24  0x01000000
#define bit23  0x00800000
#define bit22  0x00400000
#define bit21  0x00200000
#define bit20  0x00100000
#define bit19  0x00080000
#define bit18  0x00040000
#define bit17  0x00020000
#define bit16  0x00010000
#define bit15  0x00008000
#define bit14  0x00004000
#define bit13  0x00002000
#define bit12  0x00001000
#define bit11  0x00000800
#define bit10  0x00000400
#define bit09  0x00000200
#define bit08  0x00000100
#define bit07  0x00000080
#define bit06  0x00000040
#define bit05  0x00000020
#define bit04  0x00000010
#define bit03  0x00000008
#define bit02  0x00000004
#define bit01  0x00000002
#define bit00  0x00000001

#define BIT_SET(value,bit) (value |= (1 << bit))
#define BIT_CLR(value,bit) (value &= ~(1 << bit))
/*********************************************************************************************************************/


/*********************************************************************************************************************/
#define		MIM_BASE		0x00c00000
#define		CCM_BASE		0x00c10000
#define		RESET_BASE		0x00c20000
#define		CLOCK_BASE		0x00c30000
#define		INTC_BASE		0x00c40000
#define		PIT1_BASE		0x00c50000
#define		WDT_BASE		0x00c60000
#define		SCB2_BASE		0x00c70000
#define		CRYPTO_BASE		0x00c80000
#define		DES_BASE		0x00c90000
#define		EDMAC_BASE		0x00ca0000
//#define		SPI_BASE		0x00cb0000
#define		SPI_BASE		0x00dc0000
#define		USI1_BASE		0x00cc0000
#define		EPORT_BASE		0x00cd0000
#define		USI2_BASE		0x00ce0000
#define		AES_BASE		0x00cf0000
#define		SSF33_BASE		0x00d00000
#define		SCI_BASE		0x00d10000
#define		TRNG_BASE		0x00d20000
#define		TC_BASE             0x00d30000
#define		PIT2_BASE		0x00d40000
#define		EFLASH_BASE		0x00d50000
#define		I2C_BASE		0x00d60000
#define		USI3_BASE		0x00d70000
#define		DMAC_BASE		0x00d80000
#define		SHA1_BASE		0x00db0000
#define		SPI2_BASE		0x00dc0000
#define		SMS4_BASE		0x00de0000
#define		USBC_BASE		0x01000000	

/*********************************************************************************************************************/
  
 
/*********************************************************************************************************************/
/* Chip Configuration Module (CCM)*/   
                                      
#define     CCR                           (*(volatile unsigned short *)(CCM_BASE + 0x0000))
#define     CCR_HIGH                      (*(volatile unsigned short *)(CCM_BASE + 0x0000))
#define     PHYPA                         (*(volatile unsigned short *)(CCM_BASE + 0x0004))
#define     PCFG12                        (*(volatile unsigned short *)(CCM_BASE + 0x0012))
                                          
#define     USB_LITEND                    0x0800
#define     INTSWAPD15_8                  0x0040          
#define     ISODAT1SWAPD8                 0x0020
                                          
#define     USB_WORDSWITCH                0x0002
#define     CLKMODE                       0x0080
                                          
/*********************************************************************************************************************/      
    
 
/*********************************************************************************************************************/
/*Reset Controller Module*/

#define     RCR                           (*(volatile unsigned int *)(RESET_BASE + 0x0000))
#define     RCR_1                           (*(volatile unsigned char *)(RESET_BASE + 4))

#define     CRE                           0x0080
#define     CRWE                          0x0040
/*********************************************************************************************************************/   
    
    
/*********************************************************************************************************************/
/*Clock Module*/
                                        
#define     UCR                           (*(volatile unsigned long *)(CLOCK_BASE + 0x0000))
#define     CCVR_H                        (*(volatile unsigned short *)(CLOCK_BASE + 0x000C))
#define     CCVR_L                        (*(volatile unsigned short *)(CLOCK_BASE + 0x000E))
#define     PVCR_H                        (*(volatile unsigned short *)(CLOCK_BASE + 0x0010))

#define     PLLBI_REG_PLLEN               0x0020
#define     UPDATE_CARD_CFG               0x00000002
#define     UPDATE_PLL_CFG                0x00000004                           
                                         
#define     MODULE_CLK_USI1               0x00000001	//默认打开
#define     MODULE_CLK_SPI1               0x00000002	//默认打开
#define     MODULE_CLK_USBC               0x00000004	//默认打开
#define     MODULE_CLK_PIT1               0x00000008	//默认打开
#define     MODULE_CLK_WDT                0x00000010	//默认打开
#define     MODULE_CLK_EPORT              0x00000020	//默认打开
#define     MODULE_CLK_CRYPTO             0x00000040	//默认关闭
#define     MODULE_CLK_DES                0x00000080	//默认关闭
#define     MODULE_CLK_SM1                0x00000100	//默认关闭
#define     MODULE_CLK_TRNG               0x00000400	//默认关闭
#define     MODULE_CLK_PIT2               0x00000800	//默认关闭
#define     MODULE_CLK_SCI                0x00002000	//默认关闭
#define     MODULE_CLK_USI2               0x00004000	//默认打开
#define     MODULE_CLK_USI3               0x00008000	//默认打开
#define     MODULE_CLK_I2C                0x00010000	//默认打开
#define     MODULE_CLK_SSF33              0x00020000	//默认关闭
#define     MODULE_CLK_TC                 0x00040000	//默认打开
#define     MODULE_CLK_AES                0x00080000	//默认关闭
#define     MODULE_CLK_SHA1               0x00100000	//默认关闭
#define     MODULE_CLK_SPI2               0x01000000	//默认打开
#define     MODULE_CLK_SMS4               0x08000000	//默认打开

/*********************************************************************************************************************/
 
 
/*********************************************************************************************************************/                                  
/*EFlash Module*/  
     
#define     EFLASHCLKD                    (*(volatile unsigned char *)(EFLASH_BASE + 0x02))
#define     EFLASHTIM                     (*(volatile unsigned int *)(EFLASH_BASE + 0x14))

#define     EFLASH_CLKD_PRDIV8            (1 << 6)

#define     PAGE_BYTE_SIZE                (0x200)		// 512bytes, 128 words
#define     CHIP_BYTE_SIZE                (0x40000)		// 256kbytes, 128k words

#define     EFLASH_SYS_CLK_32M            32000
#define     EFLASH_SYS_CLK_40M            40000
#define     EFLASH_SYS_CLK_60M            60000
#define     EFLASH_SYS_CLK_80M            80000

/*********************************************************************************************************************/ 

   
   
/*********************************************************************************************************************/
/*Interrupt Controller Module*/

#define RSR_S    bit31
#define PSR_SP   bit29 + bit28

#define PSR_VEC bit22  + bit21 + bit20 + bit19 + bit18 + bit17 + bit16
#define TM      bit15  + bit14      
#define TP      bit13
#define TCTL    bit12
#define SC      bit10
#define MM      bit09
#define EE      bit08
#define IC      bit07
#define IE      bit06
#define FE      bit04
#define AF      bit01
#define C       bit00

#define VECTOR_OFFSET	32

#define Enable_Interrupts		Write_PSR(Read_PSR() | EE | IE | FE)	  /* 开中断 */
#define Disable_Interrupts 	    Write_PSR(Read_PSR() & ~EE & ~IE & ~FE) /* 关中断 */

#define Clear_VBR               Write_VBR((Read_VBR() & 0) | 0x00801000)
#define Install_Vector(isr,address)        (*(void **)(address) = (isr))

#define PRIORITY_LEVEL_7		0x07		
#define PRIORITY_LEVEL_6		0x06
#define PRIORITY_LEVEL_5		0x05
#define PRIORITY_LEVEL_4		0x04
#define PRIORITY_LEVEL_3		0x03		
#define PRIORITY_LEVEL_2		0x02
#define PRIORITY_LEVEL_1		0x01
#define PRIORITY_LEVEL_0		0x00	

#define PRIORITY_LEVEL_7_INTS 	0x00000080
#define PRIORITY_LEVEL_6_INTS 	0x00000040
#define PRIORITY_LEVEL_5_INTS 	0x00000020
#define PRIORITY_LEVEL_4_INTS 	0x00000010
#define PRIORITY_LEVEL_3_INTS 	0x00000008
#define PRIORITY_LEVEL_2_INTS 	0x00000004
#define PRIORITY_LEVEL_1_INTS 	0x00000002
#define PRIORITY_LEVEL_0_INTS 	0x00000001

#define ICR_reg		(*(volatile unsigned short*)(INTC_BASE+0x0000))
#define NIER_reg	(*(volatile unsigned long*)(INTC_BASE+0x0010))  

//#define INTC_BASE    0x00C40000
#define PLSR00_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0040))	
#define PLSR01_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0041))	
#define PLSR02_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0042))	
#define PLSR03_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0043))	
#define PLSR04_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0044))	
#define PLSR05_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0045))	
#define PLSR06_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0046))	
#define PLSR07_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0047))
#define PLSR08_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0048))	
#define PLSR09_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0049))	
#define PLSR10_reg  (*(volatile unsigned char *)(INTC_BASE + 0x004A))	
#define PLSR11_reg  (*(volatile unsigned char *)(INTC_BASE + 0x004B))
#define PLSR12_reg  (*(volatile unsigned char *)(INTC_BASE + 0x004C))	
#define PLSR13_reg  (*(volatile unsigned char *)(INTC_BASE + 0x004D))	
#define PLSR14_reg  (*(volatile unsigned char *)(INTC_BASE + 0x004E))	
#define PLSR15_reg  (*(volatile unsigned char *)(INTC_BASE + 0x004F))
#define PLSR16_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0050))	
#define PLSR17_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0051))	
#define PLSR18_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0052))	
#define PLSR19_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0053))
#define PLSR20_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0054))	
#define PLSR21_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0055))	
#define PLSR22_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0056))	
#define PLSR23_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0057))
#define PLSR24_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0058))	
#define PLSR25_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0059))	
#define PLSR26_reg  (*(volatile unsigned char *)(INTC_BASE + 0x005A))	
#define PLSR27_reg  (*(volatile unsigned char *)(INTC_BASE + 0x005B))
#define PLSR28_reg  (*(volatile unsigned char *)(INTC_BASE + 0x005C))	
#define PLSR29_reg  (*(volatile unsigned char *)(INTC_BASE + 0x005D))	
#define PLSR30_reg  (*(volatile unsigned char *)(INTC_BASE + 0x005E))	
#define PLSR31_reg  (*(volatile unsigned char *)(INTC_BASE + 0x005F))
#define PLSR32_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0060))	
#define PLSR33_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0061))	
#define PLSR34_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0062))	
#define PLSR35_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0063))
#define PLSR36_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0064))	
#define PLSR37_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0065))	
#define PLSR38_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0066))	
#define PLSR39_reg  (*(volatile unsigned char *)(INTC_BASE + 0x0067))
/*********************************************************************************************************************/


/*********************************************************************************************************************/
/*Serial Communications Interface Module (SCI)*/


//#define SCI_BASE         0x00D10000
////////////////////////////////////////////
#define rcSCIBD			(*(volatile unsigned short*)(SCI_BASE + 0x0000))	// SCI2 baud register
#define rcSCIBDH		(*(volatile unsigned char *)(SCI_BASE + 0x0000))	// SCI2 baud register high
#define rcSCIBDL		(*(volatile unsigned char *)(SCI_BASE + 0x0001))	// SCI2 baud register low
#define rcSCICR1		(*(volatile unsigned char *)(SCI_BASE + 0x0002))	// SCI2 control register1
#define rcSCICR2		(*(volatile unsigned char *)(SCI_BASE + 0x0003))	// SCI2 control register2
#define rcSCISR1		(*(volatile unsigned char *)(SCI_BASE + 0x0004))	// SCI2 status register1
#define rcSCISR2		(*(volatile unsigned char *)(SCI_BASE + 0x0005))	// SCI2 status register2
#define rcSCIDRH		(*(volatile unsigned char *)(SCI_BASE + 0x0006))	// SCI2 data register high
#define rcSCIDRL		(*(volatile unsigned char *)(SCI_BASE + 0x0007))	// SCI2 data register low
#define rcSCIPURD		(*(volatile unsigned char *)(SCI_BASE + 0x0008))	// SCI2 pullup and reduced drive register
#define rcSCIPORT		(*(volatile unsigned char *)(SCI_BASE + 0x0009))	// SCI2 port data register
#define rcSCIDDR		(*(volatile unsigned char *)(SCI_BASE + 0x000a))	// SCI2 data direction register

////////////////////////////////////////////
#define SCICR1_LOOPS_MASK		0x80
#define SCICR1_WOMS_MASK		0x40
#define SCICR1_RSRC_MASK		0x20
#define SCICR1_M_MASK			0x10
#define SCICR1_WAKE_MASK		0x08
#define SCICR1_ILT_MASK			0x04
#define SCICR1_PE_MASK			0x02
#define SCICR1_PT_MASK 			0x01

////////////////////////////////////////////
#define SCICR2_TIE_MASK			0x80
#define SCICR2_TCIE_MASK		0x40
#define SCICR2_RIE_MASK			0x20
#define SCICR2_ILIE_MASK		0x10
#define SCICR2_TE_MASK			0x08
#define SCICR2_RE_MASK			0x04
#define SCICR2_RWU_MASK			0x02
#define SCICR2_SBK_MASK			0x01

////////////////////////////////////////////
#define SCISR1_TDRE_MASK		0x80
#define SCISR1_TC_MASK		    0x40
#define SCISR1_RDRF_MASK		0x20
#define SCISR1_IDLE_MASK		0x10
#define SCISR1_OR_MASK			0x08
#define SCISR1_NF_MASK			0x04
#define SCISR1_FE_MASK			0x02
#define SCISR1_PF_MASK			0x01

////////////////////////////////////////////
#define SCISR2_RAF_MASK			0x01

////////////////////////////////////////////
#define SCIDRH_R8_MASK		    0x80
#define SCIDRH_T8_MASK		    0x40

////////////////////////////////////////////
#define SCIPURD_SCISDOZ_MASK	0x80
#define SCIPURD_RDPSCI_MASK		0x10
#define SCIPURD_PUPSCI_MASK		0x01

////////////////////////////////////////////
#define SCIPORT_PORTSC1_MASK	0x02
#define SCIPORT_PORTSC0_MASK	0x01

////////////////////////////////////////////
#define SCIDDR_DDRSC1_MASK		0x02
#define SCIDDR_DDRSC0_MASK		0x01

////////////////////////////////////////////

#define SCI_ENABLE_TRANSMITTER  (rcSCICR2 |= SCICR2_TE_MASK)
#define SCI_DISABLE_TRANSMITTER (rcSCICR2 &= ~SCICR2_TE_MASK)

#define SCI_ENABLE_RECEIVER  (rcSCICR2 |= SCICR2_RE_MASK)
#define SCI_DISABLE_RECEIVER (rcSCICR2 &= ~SCICR2_RE_MASK)

/////////////////////////////////////////////
#define		TEN		0				    //10bit frame
#define		ELEVEN	SCICR1_M_MASK	    //11bit frame

#define		ParityDIS	0				//Parity Disable
#define		ParityEN	SCICR1_PE_MASK	//Parity Enable

#define		EVE		0				    //Even Parity
#define		ODD		SCICR1_PT_MASK	    //Odd Parity
/*********************************************************************************************************************/


/*********************************************************************************************************************/
/*Edge Port Module*/

#define EPORT_EPPAR   (*(volatile unsigned short *)(EPORT_BASE+0x00))
#define EPORT_EPDDR   (*(volatile unsigned char *)(EPORT_BASE+0x02))
#define EPORT_EPIER   (*(volatile unsigned char *)(EPORT_BASE+0x03))
#define EPORT_EPDR    (*(volatile unsigned char *)(EPORT_BASE+0x04))
#define EPORT_EPPDR   (*(volatile unsigned char *)(EPORT_BASE+0x05))
#define EPORT_EPFR    (*(volatile unsigned char *)(EPORT_BASE+0x06))
#define EPORT_EPPUE   (*(volatile unsigned char *)(EPORT_BASE+0x07))
#define EPORT_EPLPR   (*(volatile unsigned char *)(EPORT_BASE+0x08))
#define EPORT_EPODE   (*(volatile unsigned char *)(EPORT_BASE+0x09))
/*********************************************************************************************************************/
   
                                        
/*********************************************************************************************************************/                                  
/*Universal Serial Interface (USI)*/

#define     USI1_BA                       USI1_BASE
#define     USI2_BA                       USI2_BASE
#define     USI3_BA                       USI3_BASE

#define     USI_BA                        USI1_BA
/*********************************************************************************************************************/          


/*********************************************************************************************************************/                                  
/*Clock*/
#define CLK_UCR				(*(volatile UINT32 *)(CLOCK_BASE+0x00))
#define CLK_MSCR			(*(volatile UINT32 *)(CLOCK_BASE+0x04))
#define CLK_SYNSR			(*(volatile UINT16 *)(CLOCK_BASE+0x08))
#define CLK_SYNTR			(*(volatile UINT16 *)(CLOCK_BASE+0x0a))
#define CLK_CCVR			(*(volatile UINT32 *)(CLOCK_BASE+0x0c))
#define CLK_PCVR			(*(volatile UINT32 *)(CLOCK_BASE+0x10))
#define CLK_G0CR			(*(volatile UINT32 *)(CLOCK_BASE+0x14))
#define CLK_G1CR			(*(volatile UINT32 *)(CLOCK_BASE+0x18))
#define CLK_G2CR			(*(volatile UINT32 *)(CLOCK_BASE+0x1c))

#define CLEAR_PHY_BIT_DIR	*(unsigned short *)(CCM_BASE+0x04) &= ~0x0004;
/*********************************************************************************************************************/          


/*********************************************************************************************************************/                                  
/*I2C*/
#define I2C_SAR   (*(volatile unsigned char *)(I2C_BASE))
#define I2C_CR    (*(volatile unsigned char *)(I2C_BASE + 1))
#define I2C_CPR   (*(volatile unsigned char *)(I2C_BASE + 2))
#define I2C_CSR   (*(volatile unsigned char *)(I2C_BASE + 3))
#define I2C_CDR   (*(volatile unsigned char *)(I2C_BASE + 4))
#define I2C_PCR   (*(volatile unsigned char *)(I2C_BASE + 7))
#define I2C_PDR   (*(volatile unsigned char *)(I2C_BASE + 8))
#define I2C_DDR   (*(volatile unsigned char *)(I2C_BASE + 9))

#define CR_EN     (0x01)
#define CR_IEN    (0x02)
#define CR_MSMOD  (0x04)
#define CR_ACKEN  (0x08)
#define CR_REPSTA (0x10)

#define SR_TF     (0x01)
#define SR_RC     (0x02)
#define SR_AASLV  (0x04)
#define SR_BBUSY  (0x08)
#define SR_ARBL   (0x10)
#define SR_RXTX   (0x20)
#define SR_DACK   (0x40)
#define SR_AACK   (0x80)

#define I2C_SLAVE_ADDR  (0x60)		//I2c eeprom
/*********************************************************************************************************************/  


/*********************************************************************************************************************/                                  
/*PIT*/
#define PIT_BASE		(0x00c50000)

#define rPIT_PCSR		*(volatile unsigned short *)(PIT_BASE)
#define rPIT_PMR		*(volatile unsigned short *)(PIT_BASE+2)
#define rPIT_PCNTR		*(volatile unsigned short *)(PIT_BASE+4)


#define PIT_PCSR		(PIT_BASE)
#define PIT_PMR			(PIT_BASE+2)
#define PIT_PCNTR		(PIT_BASE+4)
        
#define PIT_EN 			(1<<0)
#define PIT_RLD 			(1<<1)
#define PIT_PIF			(1<<2)
#define PIT_PIE			(1<<3)
#define PIT_OVM			(1<<4)
#define PIT_PDBG			(1<<5)
#define PIT_PDOZE			(1<<6)
		
#define PIT_PRE32			(5<<8) //32分频
/*********************************************************************************************************************/          
/*********************************************************************************************************************/                                  
/*SPI*/
#define	rSPICR1		(*(volatile unsigned char *)(SPI_BASE ))	
#define	rSPICR2		(*(volatile unsigned char *)(SPI_BASE + 0x01))	
#define	rSPIBR		(*(volatile unsigned char *)(SPI_BASE + 0x02))	
#define	rSPISR		(*(volatile unsigned char *)(SPI_BASE + 0x03))	
#define	rSPIDR		(*(volatile unsigned char *)(SPI_BASE + 0x05))	
#define	rSPIPURD	(*(volatile unsigned char *)(SPI_BASE + 0x06))	
#define	rSPIPORT	(*(volatile unsigned char *)(SPI_BASE + 0x07))	
#define	rSPIDDR		(*(volatile unsigned char *)(SPI_BASE + 0x08))	


#define SPI_SPIF_MASK        (0x80)
#define SPI_WCOL_MASK        (0x40)
/*********************************************************************************************************************/        
        

/*********************************************************************************************************************/                                  
/*TC*/
//#define TC_BASE		(0x00d30000)

#define rTCCR		*(volatile unsigned short *)(TC_BASE)
#define rTCMR		*(volatile unsigned short *)(TC_BASE+2)
#define rTCNTR		*(volatile unsigned short *)(TC_BASE+4)
#define rTCSR		*(volatile unsigned short *)(TC_BASE+6)

#define TCCR		(TC_BASE)
#define TCMR		(TC_BASE+2)
#define TCNTR		(TC_BASE+4)
#define TCSR		(TC_BASE+6)

#define TC_WAIT				(1<<11)
#define TC_DOZE				(1<<10)
#define TC_STOP		    	(1<<9)
#define TC_DBG				(1<<8)
#define TC_IS				(1<<7)
#define TC_IF				(1<<3)
#define TC_IE				(1<<2)
#define TC_CU				(1<<1)
#define TC_RN				(1<<0)
        
        
        
#define TC_WDP_64			(0<<4) //64ms
#define TC_WDP_32			(1<<4) //32ms
#define TC_WDP_16			(2<<4) //16ms
#define TC_WDP_8			(3<<4) //8ms
#define TC_WDP_4			(4<<4) //4ms
#define TC_WDP_2			(5<<4) //2ms
#define TC_WDP_1			(6<<4) //1ms
#define TC_WDP_0_5			(7<<4) //0.5ms
/*********************************************************************************************************************/             
        
        
        
/*********************************************************************************************************************/                                  
/*TRNG*/
//#define		TRNG_BASE		0x00d20000
#define rTRNG_CSR		*(volatile unsigned int *)(TRNG_BASE)
#define rTRNG_DR		*(volatile unsigned int *)(TRNG_BASE+4)
        
        
#define TRNG_CSR		(TRNG_BASE)
#define TRNG_DR		    (TRNG_BASE+4)
        
#define TRNGEN 			(1<<8)
#define TRNGIE 			(1<<9)
#define TRNGCLR			(1<<10)
#define TRNG_INT		(1<<11)
#define TRNG_SHIFT		(1<<12)

		
#define DIVCR(x)		(x)
/*********************************************************************************************************************/             
        
/*********************************************************************************************************************/                                  
/*USB*/
//#define USBC_BASE			0x01000000
//  EP0 端点Fifo地址固定在 0x00800000不可修改
//  只可以修改IN ，OUT 端点的地址
#define USB_FIFO_OFFSET			0x0 
/*********************************************************************************************************************/             
        
        

/*********************************************************************************************************************/                                  
/*USI*/
#define USI_BASE USI1_BASE

#define USIBDR                        (*(volatile UINT8 *)(USI_BASE+0x00))
#define USICR1                        (*(volatile UINT8 *)(USI_BASE+0x01))
#define USICR2                        (*(volatile UINT8 *)(USI_BASE+0x02))
#define USISR                         (*(volatile UINT8 *)(USI_BASE+0x03))
#define USIIER                        (*(volatile UINT8 *)(USI_BASE+0x04))
#define USIRDR                        (*(volatile UINT8 *)(USI_BASE+0x05))
#define USITDR                        (*(volatile UINT8 *)(USI_BASE+0x05))
#define USIWTRH                       (*(volatile UINT8 *)(USI_BASE+0x06))
#define USIWTRM	                      (*(volatile UINT8 *)(USI_BASE+0x07))
#define USIWTRL                       (*(volatile UINT8 *)(USI_BASE+0x08))
#define USIGTRH                       (*(volatile UINT8 *)(USI_BASE+0x09))
#define USIGTRL                       (*(volatile UINT8 *)(USI_BASE+0x0a))
#define USICSR                        (*(volatile UINT8 *)(USI_BASE+0x0b))
#define USIPCR                        (*(volatile UINT8 *)(USI_BASE+0x0c))
#define USIPDR                        (*(volatile UINT8 *)(USI_BASE+0x0d))
#define USIDDR                        (*(volatile UINT8 *)(USI_BASE+0x0e))
#define USIFIFOINTCON                 (*(volatile UINT8 *)(USI_BASE+0x0f))
#define USICRCH                       (*(volatile UINT8 *)(USI_BASE+0x10))
#define USICRCL                       (*(volatile UINT8 *)(USI_BASE+0x11))
        

#define USISR_TDRE_MASK       0x80
#define USISR_TC_MASK         0x40
#define USISR_RDRF_MASK       0x20
#define USISR_OR_MASK         0x10
#define USISR_ERROR_MASK      0x08
#define USISR_ATR_MASK        0x04
#define USISR_WTO_MASK        0x02
#define USISR_SB_MASK         0x01
/*********************************************************************************************************************/                                  
/*D0~D31*/
#define		GPIO_BA		MIM_BASE

#define rcIOCR          (*(volatile UINT16 *)(GPIO_BA+0x10))
#define rcGPIODO        (*(volatile UINT32 *)(GPIO_BA+0x14))
#define rcGPIODIR       (*(volatile UINT32 *)(GPIO_BA+0x18))
#define rcGPIODI        (*(volatile UINT32 *)(GPIO_BA+0x1C))































