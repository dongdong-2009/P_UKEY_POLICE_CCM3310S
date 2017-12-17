#include "hal_alg.h"
#include "cos.h"
#include "string.h"
//#include "readsn.h"

//g_sector_info,分配空间时,第一个字节是要使用的扇区数量,记录将使用的扇区编号
SECTOR_INFO g_sector_info;//

SECTOR_INFO *p_sector_info;
FILE_HEAD_INFO *p_g_EF_head,*p_g_DF_head;//全局，初始化指向MF
SECURE_STATE g_secure_state;

#define EF_FILE_TYPE 		p_g_EF_head->type
#define EF_FILE_SIZE 		((p_g_EF_head->len[0]<<8)+p_g_EF_head->len[1])
#define EF_FILE_STATE		 p_g_EF_head->rfu[F_FLAG_STATE]
#define EF_FILE_RSATYPE	 	p_g_EF_head->rfu[F_FLAG_RSATYPE]

#define  DEBUG 0

#define  UPDATE_SECTOR_TYPE 0

const MANUFACTURER_INFO init_manufactor={
		{0x12,0x34,0x56,0x78},
			"01",//硬件版本
			"01",//指纹算法版本
			"01",//驱动代号 01 UMS 
			"01",//芯片代号 01 HS32U2 01 HS32K1
			"北京百汇安",//制造厂商名称
			"公安三所",//发行商名称
			0x01,			//用途
			""
};
const SERIAL_NUMBER_INFO init_serial={
			0x02,
			"",//rfu
			"1234567812345678"//序列号
};

void cos_version(U8 ver[2])
{
	memcpy(ver,"\x0\x1",2);
}

void printfsector(U16 *sector_list)
{
	U16 num;
	HAL_UART_SendData("malloc sector info\r\n",sizeof("malloc sector info\r\n"));
	for(num=0;num<sector_list[0]+1;num++)
	{		
		HAL_UART_SEND_ASC(&sector_list[num],1);
		HAL_UART_SendData("\r\n",sizeof("\r\n"));
		
	}
}
//格式化出厂信息
U16 cos_dev_format(U8 *pinfo)
{
	MANUFACTURER_INFO *p=(MANUFACTURER_INFO*)DEV_VER_INFO;
	U8 flag[4]={0x12,0x34,0x56,0x78};
	
//	if(memcmp(p->init_flag,flag,4)!=0)
	{
		flash_device.erasepage(DEV_VER_INFO);
		flash_device.erasepage(DEV_COS_INIT);		
		flash_device.updatedata(DEV_VER_INFO,flag,4);
		flash_device.updatedata(DEV_VER_INFO+4,pinfo,96);
		return SW_SUCCESS;
	}
//	else
//		SW_COMMAND_FORBBIDON;
}

U16 cos_set_devicenum(U8 *pbin)
{
	U8 *p;
	p=(U8 *)DEV_VER_INFO;
	p+=sizeof(MANUFACTURER_INFO)+16;

	flash_device.writedata((U32)p,pbin,16);
	
	return SW_SUCCESS;
}
U16 cos_get_devicenum(U8 *pbout)
{
	unsigned char buffer[16];
	ReadSn(buffer);
	TOOL_BcdToAsc(pbout,buffer,8);	
	return SW_SUCCESS;

}
//设置介质编号
U16 cos_set_keynum(U8 flag,U8 *pbin)
{
	U8 *p,status;
	p=(U8 *)DEV_VER_INFO;
	p+=sizeof(MANUFACTURER_INFO)+sizeof(SERIAL_NUMBER_INFO);

	status=0xaa;
	if(flag==0)
	{
		if(*p==0xaa)
			return SW_FUNCTION_NOTSURPORT;//只能设置一次	
		flash_device.updatedata((U32)p,&status,1);
		p+=1;
		flash_device.updatedata((U32)p,pbin,32);
	}
	else if(flag==1)
	{
		p+=33;
		if(*p==0xaa)
			return SW_FUNCTION_NOTSURPORT;//只能设置一次		
		flash_device.updatedata((U32)p,&status,1);
		p+=1;
		flash_device.updatedata((U32)p,pbin,128);
	}
	return SW_SUCCESS;
}

//设置介质编号
U16 cos_get_keynum(U8 flag,U8 *pbout,U32 *poutlen)
{
	U8 *p;
	p=(U8 *)DEV_VER_INFO;
	p+=sizeof(MANUFACTURER_INFO)+sizeof(SERIAL_NUMBER_INFO);

	*poutlen=0;
	if(flag==0)
	{
		if(*p!=0xaa)
			return SW_INFO_NOINIT;
		p+=1;
		memcpy(pbout,p,32);
		*poutlen=32;
	}
	else if(flag==1)
	{
		p+=33;
		if(*p!=0xaa)
			return SW_INFO_NOINIT;
		p+=1;
		memcpy(pbout,p,128);
		*poutlen=128;
	}
	return SW_SUCCESS;
}

void cos_get_pri_sectorinfo()
{
	memcpy((U8*)&g_sector_info.init_flag,(U8*)&p_sector_info->init_flag,sizeof(SECTOR_INFO));
}

void cos_init(unsigned char flag)
{
	U16 i;
	U8 init_flag[4]={0x12,0x34,0x56,0x78};

	
	U8 *p_cos_init_flag=(U8*)DEV_COS_INIT;
	p_sector_info=(SECTOR_INFO*)PRI_SECTOR_INFO;
	p_g_DF_head=(FILE_HEAD_INFO*)MFSTART;
	p_g_EF_head=(FILE_HEAD_INFO*)FILE_PT_NULL;
	g_secure_state=SECURE_NULL;

	if(memcmp(init_flag,p_cos_init_flag,4)!=0)
	{
		for(i=1;i<TOTAL_DATA_PAGE;i++)//清除所有数据区数据
		{
			flash_device.erasepage(DATASTART+i*FLASH_PAGE_SIZE);
		}
		flash_device.updatedata(DEV_COS_INIT,init_flag,4);
		flash_device.updateflag((U32)&(p_sector_info->init_flag),0xaa);
	}
	else
	{
		if(flag==1)//强制格式化
		{
			for(i=2;i<TOTAL_DATA_PAGE;i++)//清除所有数据区数据
			{
				flash_device.erasepage(DATASTART+i*FLASH_PAGE_SIZE);
			}
			flash_device.updateflag((U32)&(p_sector_info->init_flag),0xaa);
		}

//		else if(p_sector_info->init_flag!=0xaa)
//		{
//			for(i=1;i<TOTAL_DATA_PAGE;i++)//清除所有数据区数据
//			{
//				flash_device.erasepage(DATASTART+i*FLASH_PAGE_SIZE);
//			}
//			flash_device.updateflag((U32)&(p_sector_info->init_flag),0xaa);
//		}
		cos_checksectorlist();
	}
	
	cos_get_pri_sectorinfo();
	cos_creat_initMF();
}
//返回COS文件系统大小信息
void cos_getsizeinfo(U32 *total,U32 *rest)
{
	U16 i,j,num,free_num=0;

	*total=MFEND-MFSTART;
	*rest=0;

#if  UPDATE_SECTOR_TYPE
	for(i=0;i<SECTOR_NUM;i++)
	{
		if(g_sector_info.sector_list[i]==S_FREE)
		{
			free_num++;
		}		
	}
	*rest=free_num*SECTOR_SIZE;

#else
	
	for(i=0;i<SECTOR_NUM;i++)
	{
		if(p_sector_info->sector_list[i]==S_FREE)
		{
			free_num++;
		}		
	}
	*rest=free_num*SECTOR_SIZE;

#endif
}

U8 cos_getsecurestate(void)
{
	return g_secure_state;
}
U8 cos_get_usage(void)
{
	U8 *p;
	int i=0;
	p=(U8 *)DEV_VER_INFO;
	p+=60;
	return *p;
}
U16 cos_clearsecurestate(void)
{
	g_secure_state=SECURE_NULL;
	return SW_SUCCESS;
}
U32 cos_sectortoaddr(U16 sector_num)
{
	return MFSTART+sector_num*SECTOR_SIZE;
}
unsigned short cos_addrtosector(unsigned int addr)
{
	return (addr-MFSTART)/SECTOR_SIZE;
}
//扇区表对应的状态更新
void cos_updatesectorstate(U16 sector_num,U8 status)
{
	flash_device.updateflag((U32)&p_sector_info->sector_list[sector_num],status);
}
U16 cos_check_space(U16 size,U16 *sector_list)
{
	U16 i,j,num,free_num=0;

#if UPDATE_SECTOR_TYPE
	if(0==size%(SECTOR_SIZE-4))
	{
		num=size/(SECTOR_SIZE-4);
	}
	else
	{
		num=size/(SECTOR_SIZE-4)+1;
	}
	j=1;
	for(i=0;i<SECTOR_NUM;i++)
	{
		if(g_sector_info.sector_list[i]==S_FREE)
		{
			free_num++;
			sector_list[j++]=i;
			if(free_num==num)
			{	
				sector_list[0]=free_num;
				return SW_SUCCESS;
			}
		}		
	}
	return SW_SPACE_NOTENOUGH;
#else
	if(0==size%(SECTOR_SIZE-4))
	{
		num=size/(SECTOR_SIZE-4);
	}
	else
	{
		num=size/(SECTOR_SIZE-4)+1;
	}
	j=1;
	for(i=0;i<SECTOR_NUM;i++)
	{
		if(p_sector_info->sector_list[i]==S_FREE)
		{
			free_num++;
			sector_list[j++]=i;
			if(free_num==num)
			{	
				sector_list[0]=free_num;
				return SW_SUCCESS;
			}
		}		
	}
	return SW_SPACE_NOTENOUGH;

#endif

	
}
//返回分配空间的首地址和逻辑块之间的关系
U32 cos_malloc_space(U16 *sector_list)
{
	U16 num;
	U32 addr,addr1;
	for(num=1;num<sector_list[0];num++)
	{
		addr=cos_sectortoaddr(sector_list[num]);//获取第一块扇区地址
		addr+=SECTOR_SIZE-4;
		addr1=cos_sectortoaddr(sector_list[num+1]);
		flash_device.writedata(addr,(U8 *)&addr1,4);
	}
	return cos_sectortoaddr(sector_list[1]);
}
void cos_updatesectorlist(U16 *sec_list,U8 status)
{
	U16 num;


#if UPDATE_SECTOR_TYPE
	SECTOR_INFO *p_sec_sector;
	p_sec_sector=(SECTOR_INFO*)SEC_SECTOR_INFO;

	for(num=0;num<sec_list[0];num++)
	{	
		g_sector_info.sector_list[sec_list[num+1]]=status;
	}
	//flash_device.erasepage(SEC_SECTOR_INFO);
	//首先写到备份扇区
	flash_device.writedata(SEC_SECTOR_INFO,(U8*)g_sector_info.init_flag,sizeof(SECTOR_INFO));
	//写主扇区信息
	flash_device.writedata(PRI_SECTOR_INFO,(U8*)g_sector_info.init_flag,sizeof(SECTOR_INFO));

	flash_device.updateflag((U32)&p_sector_info->rfu[0],0xAA);
	

#else
	for(num=0;num<sec_list[0];num++)
	{		
		flash_device.updateflag((U32)&p_sector_info->sector_list[sec_list[num+1]],status);
	}
	
#endif
	
}
//检查上次扇区表是否有错
void cos_checksectorlist(void)
{
	U16 num;

#if UPDATE_SECTOR_TYPE
	SECTOR_INFO *p_sec_sector;
	p_sec_sector=(SECTOR_INFO*)SEC_SECTOR_INFO;

	if(p_sec_sector->rfu[0]==0xFF)
		return;

	else//恢复主扇区
	{
		flash_device.erasepage(PRI_SECTOR_INFO);
		flash_device.updatedata(PRI_SECTOR_INFO+4,(U8*)(SEC_SECTOR_INFO+4),SECTOR_NUM);
		flash_device.updateflag(PRI_SECTOR_INFO,0xaa);
		flash_device.updateflag((U32)&p_sec_sector->rfu[0],0xFF);
	}
		
#endif
	
}

//将逻辑地址和偏移量映射为物理地址
U32 cos_dataaddr(U32 startaddr, U32 offset)
{
	if(offset<SECTOR_SIZE-(startaddr&SECTOR_ADDR_LOW)-4)	//偏移量未超过一个扇区
		startaddr+=offset;  										//确定flash地址
	else  															//偏移量超过起始地址所在扇区
	{
		offset-=SECTOR_SIZE-(startaddr&SECTOR_ADDR_LOW)-4;//超出起始地址所在扇区的那部分偏移量
		startaddr=*(U32 *)((startaddr&SECTOR_ADDR_HI)+SECTOR_SIZE-4);//获取下一扇区地址
		
		while(offset>=SECTOR_SIZE-4)  
		{
			startaddr=*(U32 *)((startaddr&SECTOR_ADDR_HI)+SECTOR_SIZE-4);//获取下一扇区地址
			offset-=SECTOR_SIZE-4;			
		}		
		startaddr+=offset;   										//确定flash的地址
	}
	return startaddr;
}
//COS读FLASH
U16 cos_readdata(U32 addr,U8 *buff,U16 len)
{
	U32 tmp_addr;
	U16 ret;
	
	if(len<=SECTOR_SIZE-4-(addr&SECTOR_ADDR_LOW))
	{
		#if	DEBUG
			UART_printf("read one sector\r\n");
		#endif
		return flash_device.readdata(addr,buff,len);
	}
	else//跨扇区
	{
		#if	DEBUG
			UART_printf("read cross sector");
		#endif
		//首先写完第一个扇区
		flash_device.readdata(addr,buff,SECTOR_SIZE-4-(addr&SECTOR_ADDR_LOW));
		len-=SECTOR_SIZE-4-(addr&SECTOR_ADDR_LOW);
		buff+=SECTOR_SIZE-4-(addr&SECTOR_ADDR_LOW);
		tmp_addr=*(U32 *)((addr&SECTOR_ADDR_HI)+SECTOR_SIZE-4);//获取下一扇区地址
		#if	DEBUG
			UART_printf("sector addr\r\n");
			HAL_UART_SEND_ASC((U8 *)&tmp_addr,4);
		#endif
	}
	while(len>SECTOR_SIZE-4)
	{
		flash_device.readdata(tmp_addr,buff,SECTOR_SIZE-4);
		len-=SECTOR_SIZE-4;
		buff+=SECTOR_SIZE-4;
		tmp_addr=*(U32 *)(tmp_addr+SECTOR_SIZE-4);
	}
	if(len>0)
	{
		flash_device.readdata(tmp_addr,buff,len);
	}
	return 0;
}
//COS写FLASH
U16 cos_writedata(U32 addr,U8 *buff,U16 len)
{
	U32 tmp_addr;
	U16 ret;
	
	if(len<=SECTOR_SIZE-4-(addr&SECTOR_ADDR_LOW))
	{
		#if	DEBUG
			UART_printf("write one sector\r\n");
		#endif
		return flash_device.writedata(addr,buff,len);
	}
	else//跨扇区
	{
		#if	DEBUG
			UART_printf("write cross sector");
		#endif
		//首先写完第一个扇区
		flash_device.writedata(addr,buff,SECTOR_SIZE-4-(addr&SECTOR_ADDR_LOW));
		len-=SECTOR_SIZE-4-(addr&SECTOR_ADDR_LOW);
		buff+=SECTOR_SIZE-4-(addr&SECTOR_ADDR_LOW);
		tmp_addr=*(U32 *)((addr&SECTOR_ADDR_HI)+SECTOR_SIZE-4);//获取下一扇区地址
		#if	DEBUG
			UART_printf("sector addr\r\n");
			HAL_UART_SEND_ASC((U8 *)&tmp_addr,4);
		#endif
	}
	while(len>SECTOR_SIZE-4)
	{
		flash_device.writedata(tmp_addr,buff,SECTOR_SIZE-4);
		len-=SECTOR_SIZE-4;
		buff+=SECTOR_SIZE-4;
		tmp_addr=*(U32 *)(tmp_addr+SECTOR_SIZE-4);
	}
	if(len>0)
	{
		flash_device.writedata(tmp_addr,buff,len);
	}
	return 0;
}

//初始化设备认证密钥
U16 cos_init_dev_authkey(U8 *pbin)
{
	DEV_AUTHKEY_INFO *p=(DEV_AUTHKEY_INFO *)DEV_AUTH_KEY;
	U8 buf[19];
	
	if(p->state==0xaa)//只准初始化一次
		return SW_FUNCTION_NOTSURPORT;
	
	buf[0]=0xbb;
	memcpy(buf+1,pbin,17);
	buf[18]=buf[17];
	
	flash_device.writedata(DEV_AUTH_KEY,buf,19);
	return SW_SUCCESS;
}

U16 cos_end_dev_authkey(void)
{
	DEV_AUTHKEY_INFO *p=(DEV_AUTHKEY_INFO *)DEV_AUTH_KEY;
	U8 buf[19];
	buf[0]=0xaa;
	flash_device.writedata(DEV_AUTH_KEY,buf,1);
	return SW_SUCCESS;
}

//设备认证
U16 cos_dev_auth(U8 *pbin,U8 *pbrand,U8 algtype)
{
	DEV_AUTHKEY_INFO *p=(DEV_AUTHKEY_INFO *)DEV_AUTH_KEY;
	U8 iv[16],buf[16],key[16],remain_counter;
	U16 len,status=0x6300;
	int i=0;
	//if(p->state!=0xaa)//未初始认证密钥
		//return SW_REFERENCEDATA_INVALID;

	remain_counter=p->retry_counter;

	if(remain_counter==0)//重试次数为0
		return SW_AUTH_LOCKED;

	memset(iv,0,16);
	memcpy(key,p->authkey,16);
	memcpy(buf,pbin,16);
	
	if(algtype==ALG_SM4)
	{
		hal_sm4_oper(buf,16,key,iv,SYMALG_ECB|SYMALG_SDEC,buf);		
	}
	else if(algtype==ALG_SSF33)
	{
		hal_ssf33_oper(buf,16,key,iv,SYMALG_ECB|SYMALG_SDEC,buf);
	}
	else if(algtype==ALG_SM1)
	{
		hal_sm1_oper(buf,16,key,iv,SYMALG_ECB|SYMALG_SDEC,buf);
	}
	else
		return SW_CONDITION_DISSATISFY;

	len=16;	
	if(hal_alg_defilldata(buf,&len)==0)//数据格式正确
	{	
		if(memcmp(buf,pbrand,len)==0)//
		{
			g_secure_state=SECURE_DEV;
			if(p->max_counter!=p->retry_counter)
			{
				flash_device.writedata((U32)&(p->retry_counter),(U8 *)&(p->max_counter),1);				
			}
			return SW_SUCCESS;
		}
	}
		
	//认证失败
	remain_counter-=1;
	flash_device.writedata((U32)&(p->retry_counter),(U8 *)&remain_counter,1);

	status|=remain_counter;
	
	return status;
}
//修改认证密钥
U16 cos_changedevkey(U8 *pbin,U8 *piv,U8 algtype)
{
	DEV_AUTHKEY_INFO *p=(DEV_AUTHKEY_INFO *)DEV_AUTH_KEY;
	U8 mac[4],buf[64],iv[16],key[16],remain_counter;
	U16 status=0x6300;

	U16 len=0x26;
	int i=0;
	remain_counter=p->retry_counter;

	if(remain_counter==0)//重试次数为0
		return SW_AUTH_LOCKED;
	
	memcpy(key,p->authkey,16);
	memcpy(buf,pbin,len);
	UART_printf("key\r\n");
	for(i=0;i<16;i++)
	{
		UART_printf("%2x",key[i]);
	}
	if(hal_alg_calmac(buf, len,key, piv, mac, algtype))
		return SW_CONDITION_DISSATISFY;


	if(memcmp(pbin+len,mac,4)==0)
	{
		memset(iv,0,16);
		memcpy(buf,pbin+6,0x20);

		
		hal_alg_bcae(buf,0x20,key,iv,SYMALG_ECB|SYMALG_SDEC,buf,algtype);//
		len=0x20;


		
		hal_alg_defilldata(buf,&len);


		
		{
			flash_device.writedata((U32)(p->authkey),buf,16);				
			if(p->max_counter!=p->retry_counter)
			{
				flash_device.writedata((U32)&(p->retry_counter),(U8 *)&(p->max_counter),1);				
			}
			return SW_SUCCESS;
		}
	}

	//mac比对失败
	remain_counter-=1;
	flash_device.writedata((U32)&(p->retry_counter),(U8 *)&remain_counter,1);

	status|=remain_counter;
	
	return status;
}
U16 cos_getpininfo(U8 *pbout,U8 pintype)
{
	EF_PIN_STRUCT *p_pin;

	U32 pin_addr;
	
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	pbout[0]=p_pin->pin_record[pintype].error_counter;
	pbout[1]=p_pin->pin_record[pintype].retry_counter;
	pbout[2]=0x01;
	return SW_SUCCESS;
}
U16 cos_blockuserpin()
{
	EF_PIN_STRUCT *p_pin;

	U32 pin_addr;
	U8 authority[2]={AUTH_FOBIDDN};
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	flash_device.writedata((U32)&(p_pin->pin_record[USER_PIN].auth_use),authority,1);	
	g_secure_state=SECURE_NULL;
	
	return SW_SUCCESS;
}

U16 cos_unblock_userpin(U8 *pbin,U8 *piv,U8 pintype,U8 algtype)
{
	EF_PIN_STRUCT *p_pin;

	U32 pin_addr;
	U8 mac[4],in[32],iv[16],buf[80],key[16],remain_counter,authority;
	U16 status=0x6300;
	U16 len=0x28;
	int i=0;
	authority =AUTH_FREE;
	
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	remain_counter=p_pin->pin_record[ADMIN_PIN].retry_counter;
	
	if(remain_counter==0)//重试次数为0
		return SW_AUTH_LOCKED;

	memcpy(key,p_pin->pin_record[ADMIN_PIN].pin,16);

	if(*(pbin+2)==0x00)
	{
		len = 0x28;
	}
	else if(*(pbin+2)==0x10)
	{
		len = 0x08;
	}
	memcpy(buf,pbin,len);


#if 0
	UART_printf("buf\r\n");	
	for(i=0;i<len;i++)
	{
		UART_printf("%2x",buf[i]);
	}

	UART_printf("\r\nkey\r\n");	
	for(i=0;i<16;i++)
	{
		UART_printf("%2x",key[i]);
	}
#endif


	if(hal_alg_calmac(buf, len,key, piv, mac, algtype))
		return SW_CONDITION_DISSATISFY;	
#if 0
	UART_printf("\r\nmac\r\n");	
	for(i=0;i<4;i++)
	{
		UART_printf("%2x",mac[i]);
	}
#endif


	if(memcmp(pbin+len,mac,4)==0)
	{

		if(*(pbin+2)==0x00)
		{
						memset(iv,0,16);
						memcpy(buf,pbin+8,0x20);
						
						hal_alg_bcae(buf,0x20,key,iv,SYMALG_ECB|SYMALG_SDEC,buf,algtype);//
						len=0x20;

						hal_alg_defilldata(buf,&len);
#if 0
							UART_printf("buf2\r\n");	
							for(i=0;i<len;i++)
							{
								UART_printf("%2x",buf[i]);
							}
#endif			
							flash_device.writedata((U32)(p_pin->pin_record[pintype].pin),buf,16);
		}
		else if(*(pbin+2)==0x10)
		{
		}

	

			flash_device.writedata((U32)&(p_pin->pin_record[pintype].auth_use),&authority,1);
			if(p_pin->pin_record[pintype].error_counter!=p_pin->pin_record[pintype].retry_counter)
			{
				flash_device.writedata((U32)&(p_pin->pin_record[pintype].retry_counter),
										(U8 *)&(p_pin->pin_record[pintype].error_counter),1);				
			}
			return SW_SUCCESS;
		}

	//mac比对失败
	remain_counter-=1;
	flash_device.writedata((U32)&(p_pin->pin_record[pintype].retry_counter),(U8 *)&remain_counter,1);

	status|=remain_counter;

	g_secure_state=SECURE_NULL;
	
	return status;
}

U16 cos_veryfypin(U8 *pbin,U8 *pbrand,U8 pintype,U8 algtype)
{
	EF_PIN_STRUCT *p_pin;

	U32 pin_addr;
	U8 in[16],iv[16],buf[16],key[16],remain_counter,authority;
	U16 len,status=0x6300;
	
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	remain_counter=p_pin->pin_record[pintype].retry_counter;
	authority = p_pin->pin_record[pintype].auth_use;
	if(remain_counter==0)//重试次数为0
		return SW_AUTH_LOCKED;
	if(authority==AUTH_FOBIDDN)
		return SW_AUTH_LOCKED;

	memset(iv,0,16);
	memcpy(key,p_pin->pin_record[pintype].pin,16);
	memcpy(in,pbin,16);

	if(algtype==ALG_SM4)
	{
		hal_sm4_oper(in,16,key,iv,SYMALG_ECB|SYMALG_SDEC,buf);		
	}
	else if(algtype==ALG_SSF33)
	{
		hal_ssf33_oper(in,16,key,iv,SYMALG_ECB|SYMALG_SDEC,buf);
	}
	else if(algtype==ALG_SM1)
	{
		hal_sm1_oper(in,16,key,iv,SYMALG_ECB|SYMALG_SDEC,buf);
	}
	else
		return SW_CONDITION_DISSATISFY;

	len=16;
	if(hal_alg_defilldata(buf,&len)==0)//数据格式正确
	{	
		if(memcmp(buf,pbrand,len)==0)//
		{			
			if(p_pin->pin_record[pintype].error_counter!=p_pin->pin_record[pintype].retry_counter)
			{
				flash_device.writedata((U32)&(p_pin->pin_record[pintype].retry_counter),(U8 *)&(p_pin->pin_record[pintype].error_counter),1);				
			}
			if(pintype==USER_PIN)
				g_secure_state=SECURE_USER;
			else if(pintype==ADMIN_PIN)
				g_secure_state=SECURE_ADMIN;
			return SW_SUCCESS;
		}
	}
		
	//认证失败
	remain_counter-=1;
	flash_device.writedata((U32)&(p_pin->pin_record[pintype].retry_counter),(U8 *)&remain_counter,1);

	status|=remain_counter;
	
	g_secure_state=SECURE_NULL;
	
	return status;
}
U16 cos_changepin(U8 *pbin,U8 *piv,U8 pintype,U8 algtype)
{
	EF_PIN_STRUCT *p_pin;

	U32 pin_addr;
	U8 mac[4],in[32],iv[16],buf[80],key[16],remain_counter;
	U16 status=0x6300;

	U16 len=0x28;
	int i=0;
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	remain_counter=p_pin->pin_record[pintype].retry_counter;
	
	if(remain_counter==0)//重试次数为0
		return SW_AUTH_LOCKED;

	memcpy(key,p_pin->pin_record[pintype].pin,16);

	memcpy(buf,pbin,len);

#if 1
	UART_printf("buf\r\n");	
	for(i=0;i<len;i++)
	{
		UART_printf("%2x",buf[i]);
	}
#endif	
#if 1
	UART_printf("\r\nkey\r\n");	
	for(i=0;i<16;i++)
	{
		UART_printf("%2x",key[i]);
	}
#endif


	if(hal_alg_calmac(buf, len,key, piv, mac, algtype))
		return SW_CONDITION_DISSATISFY;	
#if 1
	UART_printf("\r\nmac\r\n");	
	for(i=0;i<4;i++)
	{
		UART_printf("%2x",mac[i]);
	}
#endif


	if(memcmp(pbin+len,mac,4)==0)
	{
		memset(iv,0,16);
		memcpy(buf,pbin+8,0x20);
		
		hal_alg_bcae(buf,0x20,key,iv,SYMALG_ECB|SYMALG_SDEC,buf,algtype);//
		len=0x20;

		hal_alg_defilldata(buf,&len);
		{
#if 1
	UART_printf("buf2\r\n");	
	for(i=0;i<len;i++)
	{
		UART_printf("%2x",buf[i]);
	}
#endif			
			flash_device.writedata((U32)(p_pin->pin_record[pintype].pin),buf,16);				
			if(p_pin->pin_record[pintype].error_counter!=p_pin->pin_record[pintype].retry_counter)
			{
				flash_device.writedata((U32)&(p_pin->pin_record[pintype].retry_counter),
										(U8 *)&(p_pin->pin_record[pintype].error_counter),1);				
			}
			return SW_SUCCESS;
		}
	}

	//mac比对失败
	remain_counter-=1;
	flash_device.writedata((U32)&(p_pin->pin_record[pintype].retry_counter),(U8 *)&remain_counter,1);

	status|=remain_counter;

	g_secure_state=SECURE_NULL;
	
	return status;
}

U8 cos_updatefilerfu(U8 filetype,U8 flag,U8 state)
{
	U32 file_addr,addr;
	FILE_HEAD_INFO *pfile;

	if(flag>8)
		return 1;
	
	if(filetype==F_TPYE_DF||filetype==F_TPYE_MF)
		pfile=p_g_DF_head;
	else if(F_TYPE_EF==(filetype&0xf0))
		pfile=p_g_EF_head;

	addr=cos_dataaddr((U32)pfile,FIND(FILE_HEAD_INFO,rfu));
	addr+=flag;
	flash_device.updateflag(addr,state);
	return 0;
}

U8 cos_getfilerfu(U8 filetype,U8 flag,U8 *state)
{
	U32 file_addr,addr;
	FILE_HEAD_INFO *pfile;

	if(flag>8)
		return 1;
	
	if(filetype==F_TPYE_DF||filetype==F_TPYE_MF)
		pfile=p_g_DF_head;
	else if(F_TYPE_EF==(filetype&0xf0))
		pfile=p_g_EF_head;

	addr=cos_dataaddr((U32)pfile,FIND(FILE_HEAD_INFO,rfu));
	addr+=flag;
	*state=*(U8 *)addr;
	return 0;
}
U16 cos_check_MF(void)
{
	FILE_HEAD_INFO *p_file;
	DEV_AUTHKEY_INFO *p=(DEV_AUTHKEY_INFO *)DEV_AUTH_KEY;
	p_file=(FILE_HEAD_INFO*)MFSTART;
	
	if(p->retry_counter==0)//重试次数为0
		return SW_AUTH_LOCKED;
	
	if(p_file->fid[0]!=0x3F||p_file->fid[1]!=0x00)
		return SW_MF_NOTEXIST;
	else
		return SW_SUCCESS;
}
//
U16 cos_check_auth(U8 filetyp,U8 flag)
{
	 FILE_HEAD_INFO *pfile;	 
	U8 auth_request;
	
	if(filetyp==F_TPYE_DF||filetyp==F_TPYE_MF)
		pfile=p_g_DF_head;
	else if(F_TYPE_EF==(filetyp&0xf0))
		pfile=p_g_EF_head;

	if(AUTH_READ_LOC==flag)
		auth_request= pfile->auth_read;
	else if(AUTH_WRITE_LOC==flag)
		auth_request= pfile->auth_write;
	else if(AUTH_DELETE_LOC==flag)
		auth_request=pfile->auth_delete;

	if(AUTH_FOBIDDN==auth_request)//不准删除
		return  SW_SECURITY_DISSATISFY;
	else if(AUTH_FREE==auth_request)//不需要权限
		return  SW_SUCCESS;
	
	else if(g_secure_state!=auth_request)
		return SW_SECURITY_DISSATISFY;
	else 
		return SW_SUCCESS;
}

U16 cos_check_FIDok(U8 uchfid[2],U8 ftype)
{
	FILE_HEAD_INFO *p_file;

	if(F_TPYE_DF==ftype)
	{
		p_file=(FILE_HEAD_INFO*)MFSTART;
		while(p_file->pnext!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(p_file->fid[0]==uchfid[0]&&p_file->fid[1]==uchfid[1])
				return SW_DATA_PARAERR;
			p_file=p_file->pnext;
		}
		return SW_SUCCESS;
	}
	else if(F_TYPE_EF==(ftype&0xf0))
	{
		p_file=p_g_DF_head->pchild;
		while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(p_file->fid[0]==uchfid[0]&&p_file->fid[1]==uchfid[1])
				return SW_DATA_PARAERR;
			p_file=p_file->pnext;
		}
		return SW_SUCCESS;
	}
}

//创建文件,并更新链表信息,只有文件头信息，EF文件会分配空间但不会写入数据
U16 cos_instantiation_file(U8 *pbfile,U16 file_size)
{
	U32 file_addr,addr;
	U16 ret,sector_list[SECTOR_NUM+1];//
	FILE_HEAD_INFO *p_f_head=(FILE_HEAD_INFO*)pbfile;

	FILE_HEAD_INFO *phead,*ptemp;
	
	ret=cos_check_space(file_size,sector_list);
	if(SW_SUCCESS!=ret)
		return ret;
	//printfsector(sector_list);
	file_addr=cos_malloc_space(sector_list);

	if(F_TPYE_MF==p_f_head->type)//创建的是MF
	{
		cos_writedata(file_addr, pbfile, file_size);
	}
	else if(F_TPYE_DF==p_f_head->type)//创建的是DF
	{
		ptemp=p_g_DF_head;
		while((FILE_HEAD_INFO*)(FILE_PT_NULL)!=ptemp->pnext)//在最后位置插入
		{
			ptemp=ptemp->pnext;
		}
		p_f_head->pprevious=ptemp;//新应用的前指向
		cos_writedata(file_addr, pbfile, sizeof(FILE_HEAD_INFO));//创建应用
		//更行前指针的地址			
		addr=cos_dataaddr((U32)ptemp,FIND(FILE_HEAD_INFO,pnext));
		cos_writedata(addr, (U8 *)&file_addr, 4);
		p_g_DF_head=(FILE_HEAD_INFO*)file_addr;
	}
	else //创建的是EF
	{		
		p_f_head->pparent=ptemp=p_g_DF_head;
		if((FILE_HEAD_INFO*)(FILE_PT_NULL)==ptemp->pchild)//当前DF下第一个EF文件
		{
			//更行DF子文件指针为当前EF的地址	
			addr=cos_dataaddr((U32)ptemp,FIND(FILE_HEAD_INFO,pchild));
			cos_writedata(addr, (U8 *)&file_addr, 4);

			//写文件信息
			cos_writedata(file_addr, pbfile, sizeof(FILE_HEAD_INFO));			
		}
		else
		{
			ptemp=ptemp->pchild;
			while((FILE_HEAD_INFO*)(FILE_PT_NULL)!=ptemp->pnext)//在最后位置插入
			{
				ptemp=ptemp->pnext;
			}
			p_f_head->pprevious=ptemp;//新应用的前指向
			cos_writedata(file_addr, pbfile, sizeof(FILE_HEAD_INFO));
			//更行前指针的地址			
			addr=cos_dataaddr((U32)ptemp,FIND(FILE_HEAD_INFO,pnext));
			cos_writedata(addr, (U8 *)&file_addr, 4);
		}	
		p_g_EF_head=(FILE_HEAD_INFO*)file_addr;
		UART_printf("create file\r\n");
		HAL_UART_SEND_ASC((U8*)p_g_EF_head,sizeof(FILE_HEAD_INFO));
	
	}
	
	cos_updatesectorlist(sector_list,S_INUSE);
	return SW_SUCCESS;
}
//
void cos_free_space(unsigned int addr)
{
	unsigned short num;
	unsigned short i=1;

	U16 sector_list[SECTOR_NUM+1];
		
	while(addr!=FILE_PT_NULL)
	{

		num=cos_addrtosector(addr);
		sector_list[i++]=num;
		addr=*(unsigned int *)(addr+SECTOR_SIZE-4);
	}
	sector_list[0]=i-1;	
//	printfsector(sector_list);
	cos_updatesectorlist(sector_list,S_FREE);
}
unsigned short cos_drop_file(unsigned char filetype)
{
	FILE_HEAD_INFO *pfile,*pfather,*pchild,*pnext,*ppre;
	unsigned int addr,addr1=0xffffffff;

	if(F_TPYE_DF==filetype)
	{
		pfile=p_g_DF_head;
		pnext=pfile->pnext;
		ppre=pfile->pprevious;
		pchild=pfile->pchild;
		
		addr=cos_dataaddr((unsigned int)ppre,FIND(FILE_HEAD_INFO,pnext));
		cos_writedata(addr, (unsigned char *)&(pfile->pnext), 4);
		if(pnext!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			addr=cos_dataaddr((unsigned int)pnext,FIND(FILE_HEAD_INFO,pprevious));
			cos_writedata(addr, (unsigned char *)&(pfile->pprevious), 4);
		}
		while(pchild!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			cos_free_space((U32)pchild);
			pchild=pchild->pnext;
		}
		cos_free_space((U32)pfile);
		p_g_DF_head=(FILE_HEAD_INFO*)MFSTART;
		
	}
	
	else if(F_TYPE_EF==(filetype&0xf0))	//删除的是EF	
	 {
	 	pfile=p_g_EF_head;

		pfather=pfile->pparent;
		pnext=pfile->pnext;
		ppre=pfile->pprevious;
		
		if(pfather->pchild==pfile)//当前EF是ADF下的一个文件
		{
			//更新DF的pchild指向		
			addr=cos_dataaddr((unsigned int)pfather,FIND(FILE_HEAD_INFO,pchild));
			cos_writedata(addr, (unsigned char *)&(pfile->pnext), 4);

			//更新NEXT的前指向	
			if(pnext!=(FILE_HEAD_INFO*)FILE_PT_NULL)
			{
				addr=cos_dataaddr((unsigned int)pnext,FIND(FILE_HEAD_INFO,pprevious));
				cos_writedata(addr, (unsigned char *)&addr1, 4);
			}
			
		}
		else//更新前后链表
		{
			addr=cos_dataaddr((unsigned int)ppre,FIND(FILE_HEAD_INFO,pnext));
			cos_writedata(addr, (unsigned char *)&(pfile->pnext), 4);
			if(pnext!=(FILE_HEAD_INFO*)FILE_PT_NULL)
			{
				addr=cos_dataaddr((unsigned int)pnext,FIND(FILE_HEAD_INFO,pprevious));
				cos_writedata(addr, (unsigned char *)&(pfile->pprevious), 4);
			}
		}

		p_g_EF_head=(FILE_HEAD_INFO*)FILE_PT_NULL;
		cos_free_space((U32)pfile);
	 }	 
	return SW_SUCCESS;
}
//读文件数据
U16 cos_read_filedata(U8 filetype,U16 offset,U8 *pbdata,U16 file_size)
{
	U32 file_addr,addr;
	FILE_HEAD_INFO *pfile;
	
	if(filetype==F_TPYE_DF||filetype==F_TPYE_MF)
		pfile=p_g_DF_head;
	else if(F_TYPE_EF==(filetype&0xf0))
		pfile=p_g_EF_head;

	addr=cos_dataaddr((U32)pfile,sizeof(FILE_HEAD_INFO)+offset);
	cos_readdata(addr, pbdata, file_size);
	return SW_SUCCESS;
}
//写文件数据
U16 cos_write_filedata(U8 filetype,U16 offset,U8 *pbdata,U16 file_size)
{
	U32 file_addr,addr;
	FILE_HEAD_INFO *pfile;
	
	if(filetype==F_TPYE_DF||filetype==F_TPYE_MF)
		pfile=p_g_DF_head;
	else if(F_TYPE_EF==(filetype&0xf0))
		pfile=p_g_EF_head;

	addr=cos_dataaddr((U32)pfile,sizeof(FILE_HEAD_INFO)+offset);
	cos_writedata(addr, pbdata, file_size);
	return SW_SUCCESS;
}
U16 cos_creat_initMF(void)
{
	FILE_HEAD_INFO df_head,pf_head;
	U16 df_size,pin_size,ret;

	ret=cos_check_MF();
	if(SW_MF_NOTEXIST!=ret)
	{	
		return SW_APP_EXIST;
	}

	// 1.初始化DF信息并分配空间
	memset(df_head.fid,0xff,sizeof(FILE_HEAD_INFO));
	
	df_head.fid[0]=0x3f;
	df_head.fid[1]=0x00;
	
	df_head.type=F_TPYE_MF;
	
	df_head.auth_read=AUTH_FREE;
	df_head.auth_write=AUTH_DEV;
	df_head.auth_delete=AUTH_FOBIDDN;
	memset(df_head.name,0x00,sizeof(df_head.name));

	df_size=sizeof(FILE_HEAD_INFO);	
	
	cos_instantiation_file(df_head.fid,df_size);

	return SW_SUCCESS;

		
}
//创建应用的同时需要创建PIN文件，PIN文件记录用户PIN和管理员PIN
U16 cos_creat_app(U8 fid[2],U8 auth[3],U8 name[32],U8 *pbpininfo)
{
	FILE_HEAD_INFO df_head,pf_head;
	U16 df_size,pin_size,ret;
	EF_PIN_STRUCT pin_file;
	U8 hash[20];

	// 1.初始化DF信息并分配空间
	memset(df_head.fid,0xff,sizeof(FILE_HEAD_INFO));
	
	df_head.fid[0]=fid[0];
	df_head.fid[1]=fid[1];
//	if(fid[0]==0x3f&&fid[1]==0x00)
//	{
//		df_head.type=F_TPYE_MF;
//	}		
//	else
	{
		if(p_g_DF_head->type!=F_TPYE_MF)//只能在MF下创建DF
			return SW_FUNCTION_NOTSURPORT;
		df_head.type=F_TPYE_DF;
	}
	
	df_head.auth_read=auth[0];
	df_head.auth_write=auth[1];
	df_head.auth_delete=auth[2];
	//memset(df_head.name,0x00,sizeof(df_head.name));
	memcpy(df_head.name,name,32);

	df_size=sizeof(FILE_HEAD_INFO);	

	//应用PIN信息
	pin_size=sizeof(EF_PIN_STRUCT);
	memset((U8 *)pin_file.pin_record,0xff,pin_size);
	
	pin_file.pin_record[0].type=ADMIN_PIN;
	//hal_sha1_oper(pbpininfo,16,hash);
	//memcpy(pin_file.pin_record[0].pin,hash,16);//管理员口令
	memcpy(pin_file.pin_record[0].pin,pbpininfo,16);
	pbpininfo+=16;

	pin_file.pin_record[0].auth_use=AUTH_FREE;
	pin_file.pin_record[0].auth_change=AUTH_FREE;
	pin_file.pin_record[0].retry_counter=pin_file.pin_record[0].error_counter=*pbpininfo;
	pbpininfo++;

	
	pin_file.pin_record[1].type=USER_PIN;
	//hal_sha1_oper(pbpininfo,16,hash);
	//memcpy(pin_file.pin_record[1].pin,hash,16);//用户口令
	memcpy(pin_file.pin_record[1].pin,pbpininfo,16);
	pbpininfo+=16;

	pin_file.pin_record[1].auth_use=AUTH_FREE;
	pin_file.pin_record[1].auth_change=AUTH_FREE;
	pin_file.pin_record[1].retry_counter=pin_file.pin_record[1].error_counter=*pbpininfo;
	pbpininfo++;

//	pin_file.finger_maxnum=*pbpininfo;
//	pbpininfo++;
//	pin_file.finger_rest_counter=pin_file.finger_max_counter=*pbpininfo;
//	pin_file.finger_current_num=0;
	
	ret=cos_instantiation_file(df_head.fid,df_size+pin_size);//创建应用
	if(SW_SUCCESS!=ret)
		return ret;
	return cos_write_filedata(F_TPYE_DF,0,(U8*)pin_file.pin_record,pin_size);

	
	// 1.初始化PIN信息并分配空间
#if 0
	if(1)
	{
		EF_PIN_STRUCT pin_file;
		U8 hash[20];
		pin_size=sizeof(EF_PIN_STRUCT);
			
		memset(pf_head.fid,0xff,sizeof(FILE_HEAD_INFO));
		memset((U8 *)&pin_file.max_record,0xff,sizeof(EF_PIN_STRUCT));
		
		pf_head.fid[0]=0x00;
		pf_head.fid[1]=0x00;
		pf_head.type=F_TPYE_PIN;
		pf_head.auth_read=AUTH_FREE;
		pf_head.auth_write=0xFF;
		pf_head.auth_delete=0xFF;

		pf_head.len[0]=(pin_size>>8)&0xff;
		pf_head.len[1]=pin_size&0xff;
		memset(pf_head.name,0x00,sizeof(pf_head.name));

		cos_creat_file(pf_head.fid,sizeof(FILE_HEAD_INFO)+pin_size);
		
		pin_file.pin_record[0].type=ADMIN_PIN;
		hal_sha1_oper(pbpininfo,16,hash);
		memcpy(pin_file.pin_record[0].pin,hash,16);//管理员口令
		pbpininfo+=16;
		if(*pbpininfo>0x0f)
		{
			return SW_DATA_ERR;
		}
		pin_file.pin_record[0].auth_use=0x0f;
		pin_file.pin_record[0].auth_change=0xff;
		pin_file.pin_record[0].state=0x0f;
		pin_file.pin_record[0].error_counter=*pbpininfo<<4;
		pbpininfo++;

		
		pin_file.pin_record[1].type=USER_PIN;
		hal_sha1_oper(pbpininfo,16,hash);
		memcpy(pin_file.pin_record[0].pin,hash,16);//用户口令
		pbpininfo+=16;
		if(*pbpininfo>0x0f)
		{
			return SW_DATA_ERR;
		}
		pin_file.pin_record[1].auth_use=0x0f;
		pin_file.pin_record[1].auth_change=0xff;
		pin_file.pin_record[1].state=0x0f;
		pin_file.pin_record[1].error_counter=*pbpininfo<<4;
		pbpininfo++;

		pin_file.print_counter=*pbpininfo;
		pbpininfo++;
		pin_file.print_err_counter=*pbpininfo;

		
		cos_write_file((U8*)&pin_file.max_record,0,pin_size);
	}	
#endif
}

U16 cos_open_app(U8 *pbin,U8 type)
{
	FILE_HEAD_INFO *p_file;
	p_file=(FILE_HEAD_INFO*)MFSTART;
	
	if(type==0x00)//以ID的方式打开应用
	{
		if(memcmp(pbin,p_g_DF_head->fid,2)==0)
		{
			return SW_SUCCESS;
		}	
		
		while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(p_file->fid[0]==pbin[0]&&p_file->fid[1]==pbin[1])
			{
				p_g_DF_head=p_file;
				return SW_SUCCESS;
			}
			p_file=p_file->pnext;
		}
		return SW_FILE_NOTEXIST;
	}
	else// if(type==0x01)//以应用名称打开
	{
		while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(memcmp(pbin,p_file->name,32)==0)
			{
				p_g_DF_head=p_file;
				return SW_SUCCESS;
			}
			p_file=p_file->pnext;
		}
		return SW_FILE_NOTEXIST;
	}
	return SW_SUCCESS;
}
U16 cos_close_app(U8 *pbin,U8 type)
{
	p_g_DF_head=(FILE_HEAD_INFO*)MFSTART;
	g_secure_state=SECURE_NULL;
	return SW_SUCCESS;
}

U16 cos_delete_app()
{
	cos_drop_file(F_TPYE_DF);
	return SW_SUCCESS;
}
U16 cos_enum_app(U8 *pbout,U32 *poutlen)
{
	FILE_HEAD_INFO *p_file;
	U32 len=0;
	p_file=(FILE_HEAD_INFO*)MFSTART;	
	
	*poutlen=0;
	p_file=p_file->pnext;
	while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
	{
		memcpy(pbout+len,p_file->fid,2);
		len+=2;
		memcpy(pbout+len,p_file->name,32);
		len+=32;
		p_file=p_file->pnext;
	}

	*poutlen=len;

	return SW_SUCCESS;
}

U16 cos_creat_file(CREATE_FILE *p_create_file)
{
	FILE_HEAD_INFO pf_head;
	U16 file_size,datasize,ret;

	memset(pf_head.fid,0xff,sizeof(FILE_HEAD_INFO));
	
	pf_head.fid[0]=p_create_file->fid[0];
	pf_head.fid[1]=p_create_file->fid[1];
	pf_head.type=p_create_file->ftype;
	pf_head.len[0]=p_create_file->len[0];
	pf_head.len[1]=p_create_file->len[1];
	pf_head.auth_read=p_create_file->auth_read;
	pf_head.auth_write=p_create_file->auth_modify;
	pf_head.auth_delete=p_create_file->auth_delete;

	//memset(pf_head.name,0x00,sizeof(pf_head.name));
	memcpy(pf_head.name,p_create_file->name,32);

	datasize=(pf_head.len[0]<<8)+pf_head.len[1];
	file_size=sizeof(FILE_HEAD_INFO);	
	file_size+=datasize;//
	
	return cos_instantiation_file(pf_head.fid,file_size);//创建文件
}

U16 cos_open_file(U8 *pbin,U8 type)
{
	FILE_HEAD_INFO *p_file;
	
	p_file=p_g_DF_head->pchild;
	
	if(type==0x00)//以ID的方式打开应用
	{
		if(p_g_EF_head!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(memcmp(pbin,p_g_EF_head->fid,2)==0)
			{
				return SW_SUCCESS;
			}
		}
			
		while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(p_file->fid[0]==pbin[0]&&p_file->fid[1]==pbin[1])
			{
				p_g_EF_head=p_file;
				UART_printf("open file\r\n");
				HAL_UART_SEND_ASC((U8*)p_g_EF_head,sizeof(FILE_HEAD_INFO));
				return SW_SUCCESS;
			}
			p_file=p_file->pnext;
		}
		return SW_FILE_NOTEXIST;
	}
	else// if(type==0x01)//以应用名称打开
	{
		while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(memcmp(pbin,p_file->name,strlen((char *)pbin))==0)
			{
				p_g_EF_head=p_file;
				return SW_SUCCESS;
			}
			p_file=p_file->pnext;
		}
		return SW_FILE_NOTEXIST;
	}

	return SW_SUCCESS;
}
U16 cos_enum_file(U8 *pbout,U32 *poutlen,U8 type,U8 filetype)
{
	FILE_HEAD_INFO *p_file;
	U32 len=0;
	unsigned short file_num=0;

	p_file=p_g_DF_head->pchild;	
	
	*poutlen=0;

	if(type==0x00)//获取文件列表
	{			
		while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(filetype==0xff)//获取所有文件
			{
				memcpy(pbout+len,p_file->fid,2);
				len+=2;
				memcpy(pbout+len,p_file->name,32);
				len+=32;
			}
			else
			{
				if(p_file->type==filetype)
				{
					memcpy(pbout+len,p_file->fid,2);
					len+=2;
					memcpy(pbout+len,p_file->name,32);
					len+=32;					
				}			
			}
			p_file=p_file->pnext;
		}
		*poutlen=len;
	}
	else  if(type==0x01)///获取文件个数
	{
		file_num=0;
		while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(filetype==0xff)//获取所有文件
			{
				file_num++;
			}
			else
			{
				if(p_file->type==filetype)
				{
					file_num++;				
				}			
			}
			p_file=p_file->pnext;
		
		}
		*pbout=((file_num>>8)&0xff);
		*(pbout+1)= (file_num&0xff);
		*poutlen=2;
	}
	return SW_SUCCESS;
}

U16 cos_enum_file1(U8 *pbout,U32 *poutlen,U8 type,U8 filetype)
{
	FILE_HEAD_INFO *p_file;
	U32 len=0;

	p_file=p_g_DF_head->pchild;	
	
	*poutlen=0;

	if(type==0x00)//获取文件列表
	{			
		while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(filetype==0xff)//获取所有文件
			{
				memcpy(pbout+len,p_file->fid,3);
				len+=3;
				memcpy(pbout+len,p_file->name,32);
				len+=32;
			}
			else
			{
				if(p_file->type==filetype)
				{
					memcpy(pbout+len,p_file->fid,2);
					len+=2;
					memcpy(pbout+len,p_file->name,32);
					len+=32;					
				}			
			}
			p_file=p_file->pnext;
		}
		*poutlen=len;
	}
	else  if(type==0x01)///获取文件个数
	{
		pbout[0]=0;
		while(p_file!=(FILE_HEAD_INFO*)FILE_PT_NULL)
		{
			if(filetype==0xff)//获取所有文件
			{
				pbout[0]++;
			}
			else
			{
				if(p_file->type==filetype)
				{
					pbout[0]++;				
				}			
			}
			pbout[0]++;
		}
		*poutlen=1;
	}
	return SW_SUCCESS;
}

U16 cos_write_file(U16 offset,U8 *pbindata,U16 filelen)
{
	U32 file_addr,addr;
	FILE_HEAD_INFO *pfile;
	U16 file_size;

	pfile=p_g_EF_head;

	file_size=(pfile->len[0]<<8)+pfile->len[1];

	if(offset+filelen>file_size)
		return SW_SPACE_NOTENOUGH;
	
	return cos_write_filedata(F_TYPE_EF,offset,pbindata,filelen);
}

U16 cos_delete_file(void)
{
	cos_drop_file(F_TYPE_EF);
	return SW_SUCCESS;
}

U16 cos_get_fileinfo(U8 *pboutdata,U32 *poutlen)
{
	U32 file_addr,addr;
	FILE_HEAD_INFO *pfile;
	U32 len=0;

	*poutlen=0;
	pfile=p_g_EF_head;

	pboutdata[len++]=p_g_EF_head->fid[0];
	pboutdata[len++]=p_g_EF_head->fid[1];

	memcpy(pboutdata+len,pfile->name,32);
	len+=32;

	pboutdata[len++]=pfile->len[0];
	pboutdata[len++]=pfile->len[1];
	pboutdata[len++]=pfile->type;
	pboutdata[len++]=pfile->auth_read;
	pboutdata[len++]=pfile->auth_write;
	pboutdata[len++]=pfile->auth_delete;

	*poutlen=len;
	
	return SW_SUCCESS;
}

U16 cos_read_file(U8 *pboutdata,U16 offset,U16 filelen)
{
	U32 file_addr,addr;
	FILE_HEAD_INFO *pfile;
	U16 file_size;
	
	pfile=p_g_EF_head;

	file_size=(pfile->len[0]<<8)+pfile->len[1];

	if(offset>file_size)
		return SW_OFFSET_ERR;

	if(offset+filelen>file_size)
		return SW_SPACE_NOTENOUGH;

	return cos_read_filedata(F_TYPE_EF,offset,pboutdata,filelen);
	
}

//------------------------秘钥相关--------------
U16 cos_rsa_getpubkey(U8 *pkey,U8 flag)
{
	FILE_HEAD_INFO *pfile;
	U32 offset=0;
	pfile=p_g_EF_head;

	//公钥
	if(flag==ASYALG_RSA1024)
	{
		return cos_read_filedata(F_TYPE_EF,offset,pkey,RSA1024_LENGTH);
	}
	else if(flag==ASYALG_RSA2048)
	{
		return cos_read_filedata(F_TYPE_EF,offset,pkey,RSA2048_LENGTH);
	}
}

U16 cos_rsa_getprikey(RSA_CRT_PRIVATEKEY *privatekey,U8 flag)
{
	FILE_HEAD_INFO *pfile;
	U32 len,offset=0;
	pfile=p_g_EF_head;

	//公钥
	if(flag==ASYALG_RSA1024)
	{
		len=RSA1024_LENGTH/2;
		
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pP,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pQ,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pDP,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pDQ,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pU,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pN,len*2);	
	}
	else if(flag==ASYALG_RSA2048)
	{
		len=RSA2048_LENGTH/2;
		
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pP,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pQ,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pDP,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pDQ,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pU,len);
		offset+=len;
		cos_read_filedata(F_TYPE_EF,offset,(U8*)privatekey->pN,len*2);		
	}
	return SW_SUCCESS;
}
U16 cos_genrsakey(U8 pri_id[2],U8 pub_id[2],U8 *pout,U8 flag)
{
	U16 ret;
	U16 len,len1,len2,offset=0;

	if(flag==ASYALG_RSA1024)
	{
		len=RSA1024_LENGTH;
		len1=sizeof(RSA1024_PRI_STRUCT);//文件大小
		len2=sizeof(RSA1024_PUB_STRUCT);
	}
	else if(flag==ASYALG_RSA2048)
	{
		len=RSA2048_LENGTH;
		len1=sizeof(RSA2048_PRI_STRUCT);//文件大小
		len2=sizeof(RSA2048_PUB_STRUCT);
	}
	else
		return SW_FUNCTION_NOTSURPORT;

	//检测私钥文件
	ret=cos_open_file(pri_id,0);
	if(SW_SUCCESS!=ret)
		return ret;	

	ret=cos_check_auth(F_TYPE_EF,AUTH_WRITE_LOC);
	if(SW_SUCCESS!=ret)
		return ret;	

	if(EF_FILE_TYPE!=F_TPYE_RSAPRI)//检查文件类型
		return  SW_FILE_TYPEERR;
		//UART_printf("len1\r\n");
		//UART_printf("%2x",len1);

		//UART_printf("\r\nEF_FILE_SIZE\r\n");
		//UART_printf("%2x",EF_FILE_SIZE);
	if(EF_FILE_SIZE<len1)//检查文件大小
		return  SW_SPACE_NOTENOUGH;

	if(pub_id[0]!=0||pub_id[1]!=0)
	{
		//检测公钥文件
		ret=cos_open_file(pub_id,0);
		if(SW_SUCCESS!=ret)
			return ret;	

		ret=cos_check_auth(F_TYPE_EF,AUTH_WRITE_LOC);
		if(SW_SUCCESS!=ret)
			return ret;	

		if(EF_FILE_TYPE!=F_TPYE_RSAPUB)//检查文件类型
			return  SW_FILE_TYPEERR;


		if(EF_FILE_SIZE<len2)//检查文件大小
			return  SW_SPACE_NOTENOUGH;
	}
	

	hal_genRSAkey(&g_RsaKey,flag);

	cos_open_file(pri_id,0);

	cos_write_filedata(F_TYPE_EF,offset,(U8*)g_RsaKey.pkp,len/2);
	offset+=len/2;
	
	cos_write_filedata(F_TYPE_EF,offset,(U8*)g_RsaKey.pkq,len/2);
	offset+=len/2;

	cos_write_filedata(F_TYPE_EF,offset,(U8*)g_RsaKey.pkdp,len/2);
	offset+=len/2;
	
	cos_write_filedata(F_TYPE_EF,offset,(U8*)g_RsaKey.pkdq,len/2);
	offset+=len/2;

	cos_write_filedata(F_TYPE_EF,offset,(U8*)g_RsaKey.pku,len/2);
	offset+=len/2;

	cos_write_filedata(F_TYPE_EF,offset,(U8*)g_RsaKey.pkn,len);

	
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_RSATYPE,flag);
	if(pub_id[0]!=0||pub_id[1]!=0)
	{
		cos_open_file(pub_id,0);
		offset=0;
		cos_write_filedata(F_TYPE_EF,offset,(U8*)g_RsaKey.pkn,len);

		cos_updatefilerfu(F_TYPE_EF,F_FLAG_RSATYPE,flag);
		cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	}
	memcpy(pout,(U8 *)g_RsaKey.pkn,len);
	endiantrans(pout,len);
	return SW_SUCCESS;
}

U16 cos_rsa1024_encrypt_verify(U8 *pkey,U8 *pin,U8 *pout,U8 keyflag)
{
	U16 ret;

	if(keyflag==0)//使用内部公钥文件加密
	{
		ret=cos_open_file(pkey,0);
		if(SW_SUCCESS!=ret)
			return ret;			

		if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
			return  SW_INFO_NOINIT;
		if(EF_FILE_TYPE!=F_TPYE_RSAPUB)//检查文件类型
			return  SW_FILE_TYPEERR;
		if(EF_FILE_RSATYPE!=ASYALG_RSA1024)//检查文件类型
			return  SW_FILE_TYPEERR;
		
		cos_rsa_getpubkey((U8*)g_rsapubkey.pN,ASYALG_RSA1024);
	}
	else
	{
		endiantrans(pkey,128);			
		memcpy((U8*)g_rsapubkey.pN,pkey,128);	
	}
	
	endiantrans(pin,128);	
	hal_alg_RSApublic(&g_rsapubkey,pin,pout,ASYALG_RSA1024);
	endiantrans(pout,128);
	
	return SW_SUCCESS;
}
U16 cos_rsa2048_encrypt_verify(U8 *pkey,U8 *pin,U8 *pout,U8 keyflag)
{
	U16 ret;

	if(keyflag==0)//使用内部公钥文件加密
	{
		ret=cos_open_file(pkey,0);
		if(SW_SUCCESS!=ret)
			return ret;			

		if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
			return  SW_INFO_NOINIT;
		if(EF_FILE_TYPE!=F_TPYE_RSAPUB)//检查文件类型
			return  SW_FILE_TYPEERR;
		if(EF_FILE_RSATYPE!=ASYALG_RSA2048)//检查文件类型
			return  SW_FILE_TYPEERR;
		
		cos_rsa_getpubkey((U8*)g_rsapubkey.pN,ASYALG_RSA2048);
	}
	else
	{
		endiantrans(pkey,256);	
		memcpy((U8*)g_rsapubkey.pN,pkey,256);	
	}
	endiantrans(pin,256);
	hal_alg_RSApublic(&g_rsapubkey,pin,pout,ASYALG_RSA2048);
	endiantrans(pout,256);

	return SW_SUCCESS;
}
U16 cos_rsa1024_decrypt_sign(U8 *file_id,U8 *pin,U8 *pout)
{
	U16 ret;
	
	ret=cos_open_file(file_id,0);
	if(SW_SUCCESS!=ret)
		return ret;			

	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return  SW_INFO_NOINIT;
	if(EF_FILE_TYPE!=F_TPYE_RSAPRI)//检查文件类型
		return  SW_FILE_TYPEERR;
	if(EF_FILE_RSATYPE!=ASYALG_RSA1024)//检查文件类型
		return  SW_FILE_TYPEERR;
	
	cos_rsa_getprikey(&g_rsaprikey,ASYALG_RSA1024);	

	endiantrans(pin,128);
	hal_alg_RSAprivate(&g_rsaprikey,pin,pout,ASYALG_RSA1024);
	endiantrans(pout,128);
	
	return SW_SUCCESS;
}
U16 cos_rsa2048_decrypt_sign(U8 *file_id,U8 *pin,U8 *pout)
{
	U16 ret;
	
	ret=cos_open_file(file_id,0);
	if(SW_SUCCESS!=ret)
		return ret;			

	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return  SW_INFO_NOINIT;
	if(EF_FILE_TYPE!=F_TPYE_RSAPRI)//检查文件类型
		return  SW_FILE_TYPEERR;
	if(EF_FILE_RSATYPE!=ASYALG_RSA2048)//检查文件类型
		return  SW_FILE_TYPEERR;
	
	cos_rsa_getprikey(&g_rsaprikey,ASYALG_RSA2048);	

	endiantrans(pin,256);
	hal_alg_RSAprivate(&g_rsaprikey,pin,pout,ASYALG_RSA2048);
	endiantrans(pout,256);

	return SW_SUCCESS;
}

U16 cos_rsa_exportpubkey(U8 *pid,U8 *pout,U32 *poulen)
{
	U16 ret,len;

	*poulen=0;

	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;	
#if 0
	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return  SW_INFO_NOINIT;
	if(EF_FILE_TYPE!=F_TPYE_RSAPUB)//检查文件类型
		return  SW_FILE_TYPEERR;
	
	if(EF_FILE_RSATYPE==ASYALG_RSA1024)
	{
		len=0x80;
		pout[0]=0x04;
		pout[1]=0x00;
	}
	else if(EF_FILE_RSATYPE==ASYALG_RSA2048)
	{
		len=0x100;
		pout[0]=0x08;
		pout[1]=0x00;
	}

	cos_rsa_getpubkey(pout+2,EF_FILE_RSATYPE);
	endiantrans(pout+2,len);

	*poulen=len+2;
#else
	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return	SW_INFO_NOINIT;
	if(EF_FILE_TYPE!=F_TPYE_RSAPUB)//检查文件类型
		return	SW_FILE_TYPEERR;

	if(EF_FILE_RSATYPE==ASYALG_RSA1024)
	{
		len=0x80;
	}
	else if(EF_FILE_RSATYPE==ASYALG_RSA2048)
	{
		len=0x100;
	}

	cos_rsa_getpubkey(pout,EF_FILE_RSATYPE);
	endiantrans(pout,len);

	*poulen=len;



#endif
	return SW_SUCCESS;
}

U16 cos_rsa_importpubkey(U8 *pid,U8 *pin,U8 rsatype)
{
	U16 ret,len,len1;
	U8 rsa_type;
	
	if(rsatype==ASYALG_RSA1024)
	{
		len=RSA1024_LENGTH;
		len1=sizeof(RSA1024_PUB_STRUCT);
	}
	else if(rsatype==ASYALG_RSA2048)
	{
		len=RSA2048_LENGTH;
		len1=sizeof(RSA2048_PUB_STRUCT);
	}
	else
		return SW_FUNCTION_NOTSURPORT;

	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;	

	ret=cos_check_auth(F_TYPE_EF,AUTH_WRITE_LOC);
	if(SW_SUCCESS!=ret)
		return ret;

	if(EF_FILE_TYPE!=F_TPYE_RSAPUB)//检查文件类型
		return  SW_FILE_TYPEERR;
	
	if(EF_FILE_SIZE<len1)//检查文件大小
		return  SW_SPACE_NOTENOUGH;

	endiantrans(pin,len1);
	
	cos_write_filedata(F_TYPE_EF,0,pin,len1);
	
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_RSATYPE,rsatype);
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	
	return SW_SUCCESS;
}
U16 cos_rsa_import_enc_prikey(U8 *pid,U8 *pkey,U8 algtype,U8 *pin,U16 inlen,U8 rsatype)
{
	U16 ret,len,len1,offset,tmp_len;
	U8  buf[1024],key[16],iv[16],alg_type=0;
	U8* ptr = buf;

	if(rsatype==ASYALG_RSA1024)
	{
		len=RSA1024_LENGTH;
		len1=sizeof(RSA1024_PRI_STRUCT);
	}
	else if(rsatype==ASYALG_RSA2048)
	{
		len=RSA2048_LENGTH;
		len1=sizeof(RSA2048_PRI_STRUCT);
	}
	else
		return SW_FUNCTION_NOTSURPORT;

	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;	

	ret=cos_check_auth(F_TYPE_EF,AUTH_WRITE_LOC);
	if(SW_SUCCESS!=ret)
		return ret;

	if(EF_FILE_TYPE!=F_TPYE_RSAPRI)//检查文件类型
		return  SW_FILE_TYPEERR;
	
	if(EF_FILE_SIZE<len1)//检查文件大小
		return  SW_SPACE_NOTENOUGH;

	memset(iv,0,16);
	memcpy(buf,pin,inlen);
	memcpy(key,pkey,16);
	alg_type = algtype;
//	UART_printf("buf \r\n");
//	HAL_UART_SEND_ASC((U8*)buf,inlen);
//	UART_printf("key \r\n");
//	HAL_UART_SEND_ASC((U8*)key,16);
//	UART_printf("iv \r\n");
//	HAL_UART_SEND_ASC((U8*)iv,16);
//	UART_printf("alg_type \r\n");
//	HAL_UART_SEND_ASC((U8*)&alg_type,1);
	hal_alg_bcae(buf,inlen,key,iv,SYMALG_SDEC|SYMALG_ECB,buf,alg_type);
	//UART_printf("inlen \r\n");
	//HAL_UART_SEND_ASC((U8*)&inlen,2);
	if(hal_alg_defilldata(buf,&inlen)==1)
	{
		return SW_ALG_UNKNOWNERR;
	}
	offset=0;
	endiantrans(ptr,len/2);
	cos_write_filedata(F_TYPE_EF,offset,ptr,len/2);//P
	offset+=len/2;
	ptr+=len/2;
	
	endiantrans(ptr,len/2);
	cos_write_filedata(F_TYPE_EF,offset,ptr,len/2);//Q
	offset+=len/2;
	ptr+=len/2;

	endiantrans(ptr,len/2);
	cos_write_filedata(F_TYPE_EF,offset,ptr,len/2);//DP
	offset+=len/2;
	ptr+=len/2;

	endiantrans(ptr,len/2);
	cos_write_filedata(F_TYPE_EF,offset,ptr,len/2);//DQ
	offset+=len/2;
	ptr+=len/2;

	endiantrans(ptr,len/2);
	cos_write_filedata(F_TYPE_EF,offset,ptr,len/2);//U

	cos_updatefilerfu(F_TYPE_EF,F_FLAG_RSATYPE,rsatype);
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	
	return SW_SUCCESS;
}
U16 cos_rsa_importprikey(U8 *pid,U8 *pin,U8 rsatype)
{
	U16 ret,len,len1,offset;
	U8 rsa_type;

	if(rsatype==ASYALG_RSA1024)
	{
		len=RSA1024_LENGTH;
		len1=sizeof(RSA1024_PRI_STRUCT);
	}
	else if(rsatype==ASYALG_RSA2048)
	{
		len=RSA2048_LENGTH;
		len1=sizeof(RSA2048_PRI_STRUCT);
	}
	else
		return SW_FUNCTION_NOTSURPORT;

	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;	

	ret=cos_check_auth(F_TYPE_EF,AUTH_WRITE_LOC);
	if(SW_SUCCESS!=ret)
		return ret;

	if(EF_FILE_TYPE!=F_TPYE_RSAPRI)//检查文件类型
		return  SW_FILE_TYPEERR;
	
	if(EF_FILE_SIZE<len1)//检查文件大小
		return  SW_SPACE_NOTENOUGH;
	
	offset=0;
	endiantrans(pin,len/2);
	cos_write_filedata(F_TYPE_EF,offset,pin,len/2);//P
	offset+=len/2;
	pin+=len/2;

	endiantrans(pin,len/2);
	cos_write_filedata(F_TYPE_EF,offset,pin,len/2);//Q
	offset+=len/2;
	pin+=len/2;

	endiantrans(pin,len/2);
	cos_write_filedata(F_TYPE_EF,offset,pin,len/2);//DP
	offset+=len/2;
	pin+=len/2;

	endiantrans(pin,len/2);
	cos_write_filedata(F_TYPE_EF,offset,pin,len/2);//DQ
	offset+=len/2;
	pin+=len/2;

	endiantrans(pin,len/2);
	cos_write_filedata(F_TYPE_EF,offset,pin,len/2);//U

	cos_updatefilerfu(F_TYPE_EF,F_FLAG_RSATYPE,rsatype);
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	
	return SW_SUCCESS;
}

U16 cos_rsa_exportkey(U8 *pubkey,U8* pin,U8 rsatype,U8 *pout,U32 *poulen)
{
	U16 ret,len;

	*poulen=0;
	
	if(rsatype==ASYALG_RSA1024)
	{
		len=RSA1024_LENGTH;
	}
	else if(rsatype==ASYALG_RSA2048)
	{
		len=RSA2048_LENGTH;
	}
	
	memcpy((U8*)g_rsapubkey.pN,pubkey,len);

	hal_alg_Rsapadding(pin,16,(U8 *)G_RSA_DATA_BUF,len,ASYALG_ADEC);
	
	hal_alg_RSApublic(&g_rsapubkey,(U8 *)G_RSA_DATA_BUF,pout,rsatype);
	endiantrans(pout,len);
	*poulen=len;
	return SW_SUCCESS;
}

//------------------------国密部分--------------

U16 cos_getsm2key(U8 *pkey,U8 flag)
{
	FILE_HEAD_INFO *pfile;
	U32 offset=0;
	pfile=p_g_EF_head;

	if(flag==0)
		return cos_read_filedata(F_TYPE_EF,offset,pkey,64);
	else
		return cos_read_filedata(F_TYPE_EF,offset,pkey,32);
}
U16 cos_genSM2key(U8 pri_id[2],U8 pub_id[2],U8 *pout,U32 *poulen)
{
	U16 ret;
	U16 offset=0;
	U8 pubkey[64],prikey[32];
	*poulen=0;

	//检测私钥文件
	ret=cos_open_file(pri_id,0);
	if(SW_SUCCESS!=ret)
		return ret;	

	ret=cos_check_auth(F_TYPE_EF,AUTH_WRITE_LOC);
	if(SW_SUCCESS!=ret)
		return ret;	

	if(EF_FILE_TYPE!=F_TPYE_SM2PRI)//检查文件类型
		return  SW_FILE_TYPEERR;

	if(EF_FILE_SIZE<32)//检查文件大小
		return  SW_SPACE_NOTENOUGH;

	if(pub_id[0]!=0||pub_id[1]!=0)
	{
		//检测私钥文件
		ret=cos_open_file(pub_id,0);
		if(SW_SUCCESS!=ret)
			return ret;	

		ret=cos_check_auth(F_TYPE_EF,AUTH_WRITE_LOC);
		if(SW_SUCCESS!=ret)
			return ret;	

		if(EF_FILE_TYPE!=F_TPYE_SM2PUB)//检查文件类型
			return  SW_FILE_TYPEERR;

		if(EF_FILE_SIZE<64)//检查文件大小
			return  SW_SPACE_NOTENOUGH;
	}
	

	hal_alg_SM2producekeys(pubkey,prikey);

	cos_open_file(pri_id,0);
	cos_write_filedata(F_TYPE_EF,offset,prikey,32);
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	if(pub_id[0]!=0||pub_id[1]!=0)
	{
		cos_open_file(pub_id,0);
		offset=0;
		cos_write_filedata(F_TYPE_EF,offset,pubkey,64);
		cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	}
	memcpy(pout,pubkey,64);
	*poulen=64;
	return SW_SUCCESS;
}

U16 cos_SM2encrypt(U8 *pkey,U8 *pin,U32 len,U8 *pout,U8 flag)
{
	U16 ret;

	U8 pubkey[64];

	if(flag==0)//使用内部公钥文件加密
	{
		ret=cos_open_file(pkey,0);
		if(SW_SUCCESS!=ret)
			return ret;			

		if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
			return  SW_INFO_NOINIT;
		if(EF_FILE_TYPE!=F_TPYE_SM2PUB)//检查文件类型
			return  SW_FILE_TYPEERR;
		
		cos_getsm2key(pubkey,0);
	}
	else
		memcpy(pubkey,pkey,64);
#if ALG_DEBUG
	UART_printf("SM2 pPubkey  read\r\n");
	HAL_UART_SEND_ASC((unsigned char*)pubkey,64);
#endif
	ret = hal_alg_SM2encrypt(pubkey,pin,len,pout,1);
	return SW_SUCCESS;
}

U16 cos_SM2decrypt(U8 *pid,U8 *pin,U32 len,U8 *pout)
{
	U16 ret;
	U8 prikey[32];
	
	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;			

	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return  SW_INFO_NOINIT;
	if(EF_FILE_TYPE!=F_TPYE_SM2PRI)//检查文件类型
		return  SW_FILE_TYPEERR;

	cos_getsm2key(prikey,1);	
	hal_alg_SM2decrypt(prikey,pin,len,pout);
	
	return SW_SUCCESS;
}
U16 cos_SM2sign(U8 *pid,U8 *phash,U8 *psign)
{
	U16 ret;
	U8 prikey[32];
	
	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;			

	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return  SW_INFO_NOINIT;
	if(EF_FILE_TYPE!=F_TPYE_SM2PRI)//检查文件类型
		return  SW_FILE_TYPEERR;	

	cos_getsm2key(prikey,1);	
	hal_alg_SM2sign(prikey,phash,psign);
	
	return SW_SUCCESS;
}

U16 cos_SM2verify(U8 *pkey,U8 *phash,U8 *psign,U8 flag)
{
	U16 ret;
	U16 offset=4;
	U8 pubkey[64];

	if(flag==0)//使用内部公钥文件加密
	{
		ret=cos_open_file(pkey,0);
		if(SW_SUCCESS!=ret)
			return ret;			

		if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
			return  SW_INFO_NOINIT;
		if(EF_FILE_TYPE!=F_TPYE_SM2PUB)//检查文件类型
			return  SW_FILE_TYPEERR;
		
		cos_getsm2key(pubkey,0);
	}
	else
		memcpy(pubkey,pkey,64);

	if(hal_alg_SM2verify(pubkey,phash,psign))
		return SW_VERIFY_FAIL;
	
	return SW_SUCCESS;
}
U16 cos_SM2_exportkeyA(U8 *pid,U8 *pout,U32 *poulen)
{
	U16 ret,len;
	U8 rsatype;

	*poulen=0;

	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;	

	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return  SW_INFO_NOINIT;
	if(EF_FILE_TYPE!=F_TPYE_SM2PUB)//检查文件类型
		return  SW_FILE_TYPEERR;

	cos_getsm2key(pout,0);
	*poulen=64;
	return SW_SUCCESS;
}

U16 cos_SM2_importpubkey(U8 *pid,U8 *pin)
{
	U16 ret,len;
	U8 rsatype;

	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;	
	
	if(EF_FILE_TYPE!=F_TPYE_SM2PUB)//检查文件类型
		return  SW_FILE_TYPEERR;

	if(EF_FILE_SIZE<64)//检查文件大小
		return  SW_SPACE_NOTENOUGH;

	cos_write_filedata(F_TYPE_EF,0,pin,64);
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	
	return SW_SUCCESS;
}
U16 cos_SM2_importprikey(U8 *pid,U8 *pin)
{
	U16 ret,len;
	U8 rsatype;

	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;	
	
	if(EF_FILE_TYPE!=F_TPYE_SM2PRI)//检查文件类型
		return  SW_FILE_TYPEERR;

	if(EF_FILE_SIZE<32)//检查文件大小
		return  SW_SPACE_NOTENOUGH;

	cos_write_filedata(F_TYPE_EF,0,pin,32);
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	
	return SW_SUCCESS;
}
U16 cos_SM2_import_enc_prikey(U8 *pid,U8 *pkey,U8 algtype,U8 *pin,unsigned int inlen)
{
	U16 ret,len;
	U8 rsatype,iv[16],buf[128];

	ret=cos_open_file(pid,0);
	if(SW_SUCCESS!=ret)
		return ret;	
	
	if(EF_FILE_TYPE!=F_TPYE_SM2PRI)//检查文件类型
		return  SW_FILE_TYPEERR;

	if(EF_FILE_SIZE<32)//检查文件大小
		return  SW_SPACE_NOTENOUGH;

	memset(iv,0,16);
	memcpy(buf,pin,inlen);
	hal_alg_bcae(buf,inlen,pkey,iv,SYMALG_ECB|SYMALG_SDEC,buf,algtype);

//	if(hal_alg_defilldata(buf,&inlen)==1)
//		return SW_ALG_UNKNOWNERR;
	
	cos_write_filedata(F_TYPE_EF,0,buf,32);
	cos_updatefilerfu(F_TYPE_EF,F_FLAG_STATE,F_STATE_OK);
	
	return SW_SUCCESS;
}
U16 cos_SM2_exportkey(U8 *pubkey,U8* pin,U8 *pout,U32 *poulen)
{
	U16 ret;

	*poulen=0;
	hal_alg_SM2encrypt(pubkey,pin,16,pout,1);
	*poulen=16+96;
	return SW_SUCCESS;
}

U16 cos_SM2genZ(U8 *pkey,U8 idlen,U8 *pid,U8 *pout,U8 flag)
{
	U16 ret;
	//unsigned char pubkey[64];

	if(flag==0)//使用内部公钥文件加密
	{
		ret=cos_open_file(pkey,0);
		if(SW_SUCCESS!=ret)
			return ret;			

		if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
			return  SW_INFO_NOINIT;
		if(EF_FILE_TYPE!=F_TPYE_SM2PUB)//检查文件类型
			return  SW_FILE_TYPEERR;
		
		cos_getsm2key(p_sm2_keyexchange->selfpub,0);
	}
	else
		memcpy(p_sm2_keyexchange->selfpub,pkey,64);

	memcpy(p_sm2_keyexchange->selfid,pid,idlen);
	p_sm2_keyexchange->selfidlen=idlen;

	hal_alg_SM2producekeys(p_sm2_keyexchange->selftmppub,p_sm2_keyexchange->selftmppri);

	memcpy(pout,p_sm2_keyexchange->selfpub,64);
	memcpy(pout+64,p_sm2_keyexchange->selftmppub,64);
	
	return SW_SUCCESS;
}

U16 cos_SM2genkeya(U8 *pspub_id,U8* pspri_id,
						U8 sid_len,U8* psid,
						U8 *popub_key,U8* popub_tmpkey,
						U8 oid_len,U8* poid,
						U8 *pout,U8 *pkey,U8 mode)						
{
	U16 ret;
	//unsigned char pubkey[64];

	ret=cos_open_file(pspub_id,0);
	if(SW_SUCCESS!=ret)
		return ret;			

	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return  SW_INFO_NOINIT;
	if(EF_FILE_TYPE!=F_TPYE_SM2PUB)//检查文件类型
		return  SW_FILE_TYPEERR;
	
	cos_getsm2key(p_sm2_keyexchange->selfpub,0);

	ret=cos_open_file(pspri_id,0);
	if(SW_SUCCESS!=ret)
		return ret;			

	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return  SW_INFO_NOINIT;
	if(EF_FILE_TYPE!=F_TPYE_SM2PRI)//检查文件类型
		return  SW_FILE_TYPEERR;
	
	cos_getsm2key(p_sm2_keyexchange->selfpri,1);

	hal_alg_SM2producekeys(p_sm2_keyexchange->selftmppub,p_sm2_keyexchange->selftmppri);//临时公私钥

	memcpy(p_sm2_keyexchange->selfid,psid,sid_len);//己方ID
	p_sm2_keyexchange->selfidlen=sid_len;

	memcpy(p_sm2_keyexchange->otherpub,popub_key,64);	
	memcpy(p_sm2_keyexchange->othertmppub,popub_tmpkey,64);	

	memcpy(p_sm2_keyexchange->otherid,poid,oid_len);//
	p_sm2_keyexchange->otheridlen=oid_len;

	if(hal_alg_SM2genexchangekey(p_sm2_keyexchange,pkey,mode)==0)
	{
		memcpy(pout,p_sm2_keyexchange->selftmppub,64);
		return SW_SUCCESS;
	}
	return SW_ALG_UNKNOWNERR;
	
}

U16 cos_importkey(U8 *file_id,U8* pin,U32 inlen,U32 *pout)
{
	U16 ret;
	unsigned char algtype,rsatype,outlen,data_buf[256];
	ret=cos_open_file(file_id,0);
	if(SW_SUCCESS!=ret)
		return ret;			

	if(EF_FILE_STATE!=F_STATE_OK)//检查文件状态
		return  SW_INFO_NOINIT;
	if(EF_FILE_TYPE==F_TPYE_SM2PRI)//检查文件类型
	{
		algtype=ASYALG_SM2;
	}
	else if(EF_FILE_TYPE==F_TPYE_RSAPRI)//检查文件类型
	{
		algtype=EF_FILE_RSATYPE;
	}
	else
		return  SW_FILE_TYPEERR;

	if(algtype==ASYALG_SM2)
	{
		if(inlen!=16+96)
			return SW_LCLE_ERROR;

		cos_getsm2key(data_buf,1);	
		hal_alg_SM2decrypt(data_buf,pin,inlen,pout);		
	}
	else if(algtype==ASYALG_RSA1024)
	{
		if(inlen!=RSA1024_LENGTH)
			return SW_LCLE_ERROR;
		cos_rsa_getprikey(&g_rsaprikey,ASYALG_RSA1024);		
		hal_alg_RSAprivate(&g_rsaprikey,pin,data_buf,ASYALG_RSA1024);
		if(hal_alg_RsaOffPadding(data_buf, RSA1024_LENGTH,data_buf , &outlen)==0)
		{
			memcpy(pout,data_buf,16);
		}
			
	}
	else if(algtype==ASYALG_RSA2048)
	{
		if(inlen!=RSA2048_LENGTH)
			return SW_LCLE_ERROR;
		cos_rsa_getprikey(&g_rsaprikey,ASYALG_RSA2048);		
		hal_alg_RSAprivate(&g_rsaprikey,pin,data_buf,ASYALG_RSA2048);
		if(hal_alg_RsaOffPadding(data_buf, RSA2048_LENGTH,data_buf , &outlen)==0)
		{
			memcpy(pout,data_buf,16);
		}			
	}
	return SW_SUCCESS;
}

//------------------------秘钥相关END--------------

#if 0

//-----------------------------COS指纹部分------------
U16 cos_getfingerinfo(U8 *pbout,U32 *poutlen)
{
	EF_PIN_STRUCT *p_pin;
	U32 pin_addr,len=0;
	U8 i;
	*poutlen=0;

#if 0	
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	pbout[len++]=p_pin->finger_maxnum;
	//pbout[len++]=xFA_ReadAllFingerCount();
	pbout[len++]=p_pin->finger_current_num;
	pbout[len++]=p_pin->finger_max_counter;
	pbout[len++]=p_pin->finger_rest_counter;

	for(i=0;i<p_pin->finger_maxnum;i++)
	{
		if(p_pin->fingerID[i]!=0xff)//已存在相同ID
		{
			pbout[len++]=p_pin->fingerID[i]+1;
		}
	}

	*poutlen=len;
	return SW_SUCCESS;

#else
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	pbout[len++]=p_pin->finger_maxnum;
	pbout[len++]=xFA_ReadAllFingerCount();
	//pbout[len++]=p_pin->finger_current_num;
	pbout[len++]=p_pin->finger_max_counter;
	pbout[len++]=p_pin->finger_rest_counter;

	for(i=0;i<10;i++)
	{
		if(xFA_CheckAFinger(i)==1)//已存在相同ID
		{
			pbout[len++]=i+1;
		}
	}

	*poutlen=len;
	return SW_SUCCESS;

#endif
}

U16 cos_checkfingerstaue()
{
	EF_PIN_STRUCT *p_pin;
	U32 pin_addr;
#if 0
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	if(p_pin->finger_rest_counter==0)
		return SW_AUTH_LOCKED;
	
	if(p_pin->finger_current_num==0)
		return SW_FUNCTION_NOTSURPORT;

	if(p_pin->finger_current_num==p_pin->finger_maxnum)
		return SW_FINGER_SPACEFULL;
	
	return SW_SUCCESS;
#else
	U8 finger_current_num;

	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	finger_current_num=xFA_ReadAllFingerCount();

	if(p_pin->finger_rest_counter==0)
		return SW_AUTH_LOCKED;
	
	if(finger_current_num==0)
		return SW_FUNCTION_NOTSURPORT;

	if(finger_current_num==p_pin->finger_maxnum)
		return SW_FINGER_SPACEFULL;
	
	return SW_SUCCESS;
#endif
}
U16 cos_appendfinger(U8 id)
{
	EF_PIN_STRUCT *p_pin;
	U32 pin_addr;
	U8 counter,i,j,flag1,flag;
#if 0
	flag=1;
	
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	counter=p_pin->finger_current_num;
	j=0;
	flag1=1;
	for(i=0;i<p_pin->finger_maxnum;i++)
	{
		if(p_pin->fingerID[i]==id)//已存在相同ID
		{
			flag=0;
			break;
		}
		if(p_pin->fingerID[i]==0xff&&flag1==1)
		{
			j=i;
			flag1=0;
		}
	}
	if(flag)
	{
		flash_device.writedata((U32)&(p_pin->fingerID[j]),(U8 *)&id,1);	
		counter++;
		flash_device.writedata((U32)&(p_pin->finger_current_num),(U8 *)&counter,1);			
	}
	return SW_SUCCESS;
#else
	
	return SW_SUCCESS;

#endif
}

U16 cos_deletefinger(U8 id)
{

#if 0
	EF_PIN_STRUCT *p_pin;
	U32 pin_addr;
	U8 counter,i,flag;

	flag=1;
	
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;

	counter=p_pin->finger_current_num;
	for(i=0;i<p_pin->finger_maxnum;i++)
	{
		if(p_pin->fingerID[i]==id)//已存在相同ID
		{
			flag=0;
			break;
		}
	}
	if(flag)
	{
		return SW_FINGER_IDUNMATCH	;
	}
	else
	{
		id=0xff;
		flash_device.writedata((U32)&(p_pin->fingerID[i]),(U8 *)&id,1);	
		counter--;
		flash_device.writedata((U32)&(p_pin->finger_current_num),(U8 *)&counter,1);	
	}
#endif
	return SW_SUCCESS;
}
//FLAG指纹验证结果
U16 cos_matchfinger(U8 flag)
{
	EF_PIN_STRUCT *p_pin;
	U32 pin_addr,len=0;
	U8 counter;
	
	pin_addr=cos_dataaddr((U32)p_g_DF_head,sizeof(FILE_HEAD_INFO));
	p_pin=(EF_PIN_STRUCT*)pin_addr;
	counter=p_pin->finger_rest_counter;
	
	if(flag==0)//验证成功
	{
		g_secure_state=SECURE_USER;
		if(counter!=p_pin->finger_max_counter)
			flash_device.writedata((U32)&(p_pin->finger_rest_counter),(U8 *)&p_pin->finger_max_counter,1);	
			
	}
	else
	{
		counter--;
		flash_device.writedata((U32)&(p_pin->finger_rest_counter),(U8 *)&counter,1);
	}
	return SW_SUCCESS;
}
//-----------------------------COS指纹部分END------------

void cos_test()
{
	unsigned char buf[1024];
	unsigned int addr,i,len,addr1=0xffffffff;

	FILE_HEAD_INFO *ptmp;

	p_g_DF_head=p_g_DF_head->pnext;

#if 0
	cos_updatesectorstate(3,S_FREE);
	cos_updatesectorstate(4,S_FREE);

	cos_updatesectorstate(5,S_FREE);
	cos_updatesectorstate(6,S_FREE);
	cos_updatesectorstate(7,S_FREE);
	cos_updatesectorstate(8,S_FREE);

	HAL_UART_SEND_ASC(p_sector_info->sector_list,20);
	
	ptmp=p_g_DF_head->pchild;

	addr=cos_dataaddr((U32)ptmp,FIND(FILE_HEAD_INFO,pnext));
	cos_writedata(addr, (unsigned char *)&addr1, 4);
#endif	
	UART_printf("COS FILE\r\n");
	cos_enum_file1(buf,&len,0,0xff);
	i=0;
	while(len>0)
	{
		HAL_UART_SEND_ASC(buf+i,35);
		i+=35;
		len-=35;
	}	

#if 0
	UART_printf("COS FILE\r\n");
	HAL_UART_SEND_ASC((U8*)0x1ec00,126);
	HAL_UART_SEND_ASC((U8*)0x1f000,126);

#endif
	
	p_g_DF_head=(FILE_HEAD_INFO*)MFSTART;
}



#endif //debug endif

