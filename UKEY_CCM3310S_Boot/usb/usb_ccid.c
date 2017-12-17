
#include "includes.h"
void HAL_USB_BulkIn(unsigned char Endpoint, unsigned char *pBuf, unsigned short len)
{
		unsigned char EpIndex_reg;
		unsigned int i=0;

		
		while (len > BULK_BUF_MAXLEN)
		{
			USBDev_EP1_TX_ISR(pBuf+i,BULK_BUF_MAXLEN);	//发送一包数据
			while ( ( g_uchUSBStatus & 0x10) != 0x10 );
			g_uchUSBStatus &= ~0x10;
			

				
			len -= BULK_BUF_MAXLEN;
			i += BULK_BUF_MAXLEN;
		}
		
		//发送剩余数据到主机
		if(len>0)
		{
			USBDev_EP1_TX_ISR(pBuf+i,len);	//发送一包数据
			while ( ( g_uchUSBStatus & 0x10) != 0x10 );			
			g_uchUSBStatus &= ~0x10;
	
		}
		
		//FIXME 
		if (len == BULK_BUF_MAXLEN)//如果一次通信中的数据是64的整数倍
		{									//则需要发送0长包来结束此次通信

			USBDev_EP1_TX_ISR(NULL,0);	//发送一包数据
			while ( ( g_uchUSBStatus & 0x10) != 0x10 );
			g_uchUSBStatus &= ~0x10;

		}
}



void HAL_USB_BulkOut(unsigned char Endpoint, unsigned char *pBuf, unsigned short *plen)
{	
	unsigned short offset=0;
	unsigned short len=0;
	unsigned short temp_len=0;
	//receive the first pack
	if( (g_uchUSBStatus & 0x01) == 0x01 )	  
	{
		temp_len = USBDev_EP1_RX_ISR(pBuf); //接收到一包数据
		offset = temp_len;		
		ccid_cmd = (PCCID_CMD)pBuf;
		len = ccid_cmd->length[0] + (ccid_cmd->length[1] << 8) ;		
		*plen = offset;
		g_uchUSBStatus &= ~0x01; 
	}


	//receive  remain data
	if(len > 54)
	{
		len -= 54;		
		while(len)
		{
			while(!(g_uchUSBStatus& 0x01));		
			if( (g_uchUSBStatus & 0x01) == 0x01 )	  
			{
				temp_len = USBDev_EP1_RX_ISR(pBuf+offset); //接收到一包数据
				len    -= temp_len;
				offset += temp_len;				
				g_uchUSBStatus &= ~0x01; 
			}	
		}
		*plen = offset;
		
	}
			
	
	return;	
}

void Command(void)
{
#if 1
	unsigned int ret = 0;

	switch(g_APDU.INS)
	{	
/*		case CMD_UPLOAD_DATA: //读FLASH
			ret = CMD_BOOT_UPLOAD_DATA();
			if(ret != SW_OPERATION_SUCCESS)
				g_APDU.RLE = 0;	
			g_APDU.SW1 = 0x90;
			g_APDU.SW2 = 0x00;
			break;	
			
		case CMD_DNLOAD_DATA_A: //写FLASH,非加密				
			ret = CMD_BOOT_DNLOAD_DATA();
			if(ret != SW_OPERATION_SUCCESS)
				g_APDU.RLE = 0;	
			g_APDU.SW1 = ret>>8;
			g_APDU.SW2 = ret&0x00ff;
		break;
*/		
		case READ_TERMINAL_INFO:
			{
				ret = APP_POS_CMD_ReadTerminalInfo();
				
				g_APDU.SW1 = ret>>8;
				g_APDU.SW2 = ret&0x00ff;

			}
				break;
		case CMD_DNLOAD_DATA_B: //写FLASH,3DES加密方式				
			ret = CMD_BOOT_DNLOAD_DATA_DES3();
			if(ret != SW_OPERATION_SUCCESS)
				g_APDU.RLE = 0;	
			g_APDU.SW1 = ret>>8;
			g_APDU.SW2 = ret&0x00ff;
			break;	
/*			
		case CMD_ERASE_SECTOR: //擦FLASH	
			ret = CMD_BOOT_ERASE_SECTOR();
			if(ret != SW_OPERATION_SUCCESS)
				g_APDU.RLE = 0;	
			g_APDU.SW1 = ret>>8;
			g_APDU.SW2 = ret&0x00ff;
			break;	
			
		case CMD_BOOT_CRC: //flash data CRC	
			ret = CMD_BOOT_CRC_DATA();
		//	if(ret != SW_OPERATION_SUCCESS)
			//	g_APDU.RLE = 0;	
			g_APDU.SW1 = ret>>8;
			g_APDU.SW2 = ret&0x00ff;
			break;
*/			
		case CMD_BOOT_FLAG: //boot flag	
			ret = CMD_BOOT_FLAG_DATA();
	
			g_APDU.SW1 = ret>>8;
			g_APDU.SW2 = ret&0x00ff;
			break;	
			
		case CMD_BOOT_INFO: //获取boot信息	

			ret = CMD_BOOT_BOOT_DATA();
			g_APDU.SW1 = ret>>8;
			g_APDU.SW2 = ret&0x00ff;
			break;	
			
		case CMD_BOOT_DNLOAD_KEY: //下载或读取密钥	
			ret = CMD_BOOT_KEY_DATA();
			g_APDU.SW1 = ret>>8;
			g_APDU.SW2 = ret&0x00ff;
			break;	
		
		case 0xFF:
			EnBootMode();			
			ret=0x9000;
			break;

		default:
			g_APDU.RLE = 0;
			ret = 0x6D00;
			break;
	}
	g_APDU.SW1 = ret>>8;
	g_APDU.SW2 = ret&0x00ff;
							


	return;		//2011.12.20	

#endif	
}




void ccid_data_process(void)
{

	unsigned char err_flag= 0;
	unsigned short BlukInlen= 0;
	const unsigned char ATR[] = 
	{
		0x3b, 0xb5, 0x11, 0x00, 0x81, 0x31, 0x46, 
		0x15, 0x56, 0x20, 0x31, 0x2e, 0x30, 0x1e
	};

	err_flag = 0;
	

	
	switch(ccid_cmd->type)
	{
	case PC_to_RDR_IccPowerOn:
		{
			
			ccid_cmd->type = RDR_to_PC_DataBlock;
			ccid_cmd->length[0] = 14; 
			ccid_cmd->length[1] = 0;
			ccid_cmd->param[0] = ccid_cmd->param[1] = 0;
	
			memcpy(g_b_Comm+10, ATR, 14);
		}
	
		break;
	case PC_to_RDR_IccPowerOff:
		{
			ccid_cmd->type = RDR_to_PC_SlotStatus;
			ccid_cmd->length[0] = ccid_cmd->length[1] = 0;
			ccid_cmd->param[0] = 0x01;	
			
		}
		break;
	case PC_to_RDR_XfrBlock:
		{ 

				g_APDU.CLA = g_b_Comm[10];
				g_APDU.INS = g_b_Comm[11];
				g_APDU.PP1 = g_b_Comm[12];
				g_APDU.PP2 = g_b_Comm[13];
				g_APDU.PP3 = g_b_Comm[14];
				g_APDU.LCH = g_b_Comm[15];
				g_APDU.LCL = g_b_Comm[16];

				g_APDU.LC = (ccid_cmd->length[0] | ccid_cmd->length[1] << 8) - 5;
				
				g_APDU.pInData = g_b_Comm+17;
				g_APDU.pOutData =g_b_Comm + 10;
				
				Command();
	
				g_APDU.pOutData[g_APDU.RLE] = g_APDU.SW1;
				g_APDU.pOutData[1+g_APDU.RLE] = g_APDU.SW2;
	
				g_APDU.RLE += 2;
	
				
				ccid_cmd->type = RDR_to_PC_DataBlock;
						
				ccid_cmd->length[0] =(g_APDU.RLE & 0xff);
				ccid_cmd->length[1] =((g_APDU.RLE >> 8) & 0xff);
				
				ccid_cmd->length[2] = 0;
				ccid_cmd->length[3] = 0;
					
				ccid_cmd->param[0] = ccid_cmd->param[1] = ccid_cmd->param[2] = 0;
			
			
		} 
		break;
	case  PC_to_RDR_SetParameters:
		{
			ccid_cmd->type = RDR_to_PC_Parameters;
			ccid_cmd->length[0] = ccid_cmd->length[1] = 0;
			ccid_cmd->param[0] = 0x00;
			ccid_cmd->param[1] = 0x00;
			ccid_cmd->param[2] = 0x00;
		}
		break;
	case PC_to_RDR_GetParameters:
		{
			ccid_cmd->type = RDR_to_PC_Parameters;
			ccid_cmd->length[0] = ccid_cmd->length[1] = 0;
			ccid_cmd->param[0] = 0x40;
			ccid_cmd->param[1] = 0x85;
			ccid_cmd->param[2] = 0x00;
		}
		break;
		
	default:
		{
			err_flag =1;
		}
		break;
	}
	
	if(!err_flag)
	{						
		BlukInlen = ccid_cmd->length[0]+(ccid_cmd->length[1]<<8)+10;
		HAL_USB_BulkIn(1, g_b_Comm, BlukInlen);
		g_BulkOutlen = 0;
	}
	

}


