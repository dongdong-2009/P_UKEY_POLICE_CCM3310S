#ifndef __COS_H_
#define __COS_H_

#include "hal_flash_data.h"
#include "hal_flash_interface.h"
/*
COS文件结构的定义
*/
#pragma pack(1)/*指定按1字节对齐*/

// ---------------------------------------------------------------------  
// Type Definitions 
// ---------------------------------------------------------------------  

typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;



//-------------------------设备信息----
typedef struct  {
	unsigned char init_flag[4];
	unsigned char hardware_ver[2];		//硬件版本
	unsigned char printalg_ver[2];			//算法版本
	unsigned char driver_code[2];			//驱动代号
	unsigned char chip_code[2];			//芯片型号
	unsigned char manu_name[24];			//制造商名称
	unsigned char publisher_name[24];		//发行商名称
	unsigned char usage;				//用途
	unsigned char rfu[7];
} MANUFACTURER_INFO;
typedef struct  {
	unsigned char manu_code;				//厂商标识
	unsigned char rfu[15];					//
	unsigned char serial_number[16];			//序列号
} SERIAL_NUMBER_INFO;

typedef struct  {
	unsigned char state;
	unsigned char keynum[32];				//介质编号
	unsigned char state1;
	unsigned char signature[128];				//介质编号
} KEY_NUMBER_INFO;

//-------------------------设备认证密钥信息---------------------------
typedef struct  {
	unsigned char state;				//初始化状态
	unsigned char authkey[16];					//
	unsigned char max_counter;			//重试次数
	unsigned char retry_counter;			//剩余次数
	unsigned char rfu[5];
} DEV_AUTHKEY_INFO;

//-------------------------常量定义------------------------------------------
typedef enum{				
	S_FREE=0xFF,	//空闲状态		
	S_INUSE=0xFE,	//已使用		
	S_OUTUSE=0xFC,	//无效数据		
	S_DELETE=0xF8,	//废弃
	S_BAD=0xF0,		//坏块
}SECTOR_STATE;

typedef struct {
	SECTOR_STATE f_state;
	unsigned char rfu[7];
	unsigned char f_data[SECTOR_SIZE-1];
} Sector_DataStruct;

typedef struct sector_info {
	unsigned char init_flag;		//cos初始化标记
	unsigned char rfu[3];			//预留
	unsigned char sector_list[SECTOR_NUM];//扇区信息
} SECTOR_INFO;


//------------------------结构体定义-------------------------------------------
#define FILE_HEAD_LEN 64

#define FIND(struc,e) (unsigned int)(&((struc*)0)->e)    //获取结构体中某一元素的偏移量
#define FILE_PT_NULL 0xffffffff

typedef enum{				
	F_STATE_INIT=0XFF,	//文件已建立,但为写入数据
	F_STATE_OK=0xFC	//文件已写入数据，正常使用状态
}FILE_STATE;

typedef enum{				
	F_TPYE_MF=0XC0,	
	F_TPYE_DF=0X80,
	F_TPYE_RSAPUB=0X0A,
	F_TPYE_RSAPRI=0X0B,//对称密钥
	F_TPYE_SM2PUB=0X0D,//非对称
	F_TPYE_SM2PRI=0X0E,//非对称
	F_TPYE_CERT=0X0C,//证书文件
	F_TPYE_PRINT=0X0F,//指纹文件
	F_TYPE_EF=0X00	
}FILE_TYPE;

typedef enum{				
	F_FLAG_STATE=0,	//文件写过标记位置
	F_FLAG_RSATYPE=3		//RSA标记
}FILE_FLAG_LOC;

//记录块头信息
typedef struct file_head_info
{
	unsigned char fid[2];//文件ID		MF:3F00,应用下的pin文件fid 0x0000
	unsigned char type;//文件类型
	unsigned char len[2];//文件长度,DF可以忽略
	unsigned char auth_read;//DF创建权限,EF,KEY读使用权限
	unsigned char auth_write;//DF修改权限,EF,KEY写和修改权限
	unsigned char auth_delete;//DF删除权限,EF,KEY删除权限
	unsigned char name[32];//文件名字
	struct  file_head_info *pparent;//EF指向DF
	struct  file_head_info *pchild;//DF下指向第一个EF
	struct  file_head_info *pprevious;//同一级文件指向
	struct  file_head_info *pnext;//同一级文件指向
	unsigned char rfu[8];//rfu[0]  文件已经写入标志
						//rfu[1]  
						//rfu[2]  
						//rfu[3]  RSA 算法标志
}FILE_HEAD_INFO;

//记录块头信息
typedef struct 
{
	unsigned char fid[2];//文件ID		MF:3F00,应用下的pin文件fid 0x0000
	unsigned char type;//文件类型
	unsigned char len[2];//文件长度,DF可以忽略
	unsigned char auth_read;//DF创建权限,EF,KEY读使用权限
	unsigned char auth_write;//DF修改权限,EF,KEY写和修改权限
	unsigned char auth_delete;//DF删除权限,EF,KEY删除权限
	struct  file_head_info *pparent;//EF指向DF
	struct  file_head_info *pchild;//DF下指向第一个EF
	struct  file_head_info *pprevious;//同一级文件指向
	struct  file_head_info *pnext;//同一级文件指向
	unsigned char rfu[4];
}FILE_HEAD1_INFO;

typedef enum{				
	DEBIT_KEY=0,	
	CREDIT_KEY
}KEY_TYPE;



//KEY结构,对称密钥
typedef struct
{
	unsigned char type;//类型
	unsigned char kid;//密钥标识符
	unsigned char version;//
	unsigned char alg_type;//算法标识符
	unsigned char key[16];//
	unsigned char auth_use;//使用权限
	unsigned char auth_change;//修改权限
	unsigned char rfu[10];
}KEY_STRUCT;

typedef enum{	
	ADMIN_PIN=0,
	USER_PIN,
	PRINT_PIN
}PIN_TYPE;

//PIN记录结构
typedef struct
{
	unsigned char type;//类型
	unsigned char kid;//密钥标识符
	unsigned char pin[16];//
	unsigned char auth_use;//使用权限
	unsigned char auth_change;//修改权限
//	unsigned char minlen;//PIN最小长度
	unsigned char error_counter;//最大尝试次数
	unsigned char retry_counter;//剩余次数
	unsigned char rfu[6];
}PIN_STRUCT;


//PIN文件结构
typedef struct
{
//	unsigned char max_record;
//	unsigned char record_num;
//	unsigned char rfu[2];
	PIN_STRUCT pin_record[2];
	unsigned char finger_maxnum;//最多指纹个数
	unsigned char finger_max_counter;
	unsigned char finger_rest_counter;
	unsigned char finger_current_num;
	unsigned char fingerID[10];
}EF_PIN_STRUCT;

typedef struct
{
	unsigned char N[128];//
}RSA1024_PUB_STRUCT;

typedef struct
{
	unsigned char P[64];//
	unsigned char Q[64];//
	unsigned char dP[64];//
	unsigned char dQ[64];//
	unsigned char U[64];//
}RSA1024_PRI_STRUCT;

typedef struct
{
	unsigned char N[256];//
}RSA2048_PUB_STRUCT;
typedef struct
{
	unsigned char P[128];//
	unsigned char Q[128];//
	unsigned char dP[128];//
	unsigned char dQ[128];//
	unsigned char U[128];//
}RSA2048_PRI_STRUCT;

typedef struct
{
	unsigned char prikey[32];
}SM2_PRI_STRUCT;

typedef enum{				
	SECURE_NULL=0,//无权限
	SECURE_DEV=0x80,//设备认证
	SECURE_USER=0x10,//用户权限
	SECURE_ADMIN=0x01,//管理员权限
	SECURE_DEFAULT=0xff//管理员权限
}SECURE_STATE;

typedef enum{				
	AUTH_FOBIDDN=0,//不允许
	AUTH_USER=0x10,//用户权限
	AUTH_ADMIN=0x01,//管理员权限
	AUTH_DEV=0x80,//设备
	AUTH_FREE=0xff//自由
}AUTHORITY;
//文件权限
typedef enum{				
	AUTH_READ_LOC=0,//不允许
	AUTH_WRITE_LOC=0x01,//用户权限
	AUTH_DELETE_LOC=0x02,//管理员权限
}AUTH_LOC;

typedef struct
{
	unsigned char state;//备份区状态
	unsigned char rfu[3];//保留
	unsigned char len[2];//备份数据长度，高字节在前
	unsigned char *paddr;//备份源数据地址
	unsigned char *pdata;
}BACKUP_DATA_STRUCT;




//-------------------------cos指令层数据定义---------------


typedef struct 
{
	unsigned char fid[2];//文件ID		MF:3F00
	unsigned char name[32];//
	unsigned char auth;//DF创建权限,EF,KEY读使用权限
	unsigned char admin_pin[16];//
	unsigned char try_counter1;
	unsigned char user_pin[16];//
	unsigned char try_counter2;
	unsigned char print_num;//
	unsigned char try_counter3;
}CREATE_APP;

typedef struct 
{
	unsigned char fid[2];//文件ID		MF:3F00
	unsigned char name[32];//
	unsigned char len[2];
	unsigned char ftype;//文件类型
	unsigned char auth_read;//
	unsigned char auth_modify;
	unsigned char auth_delete;
}CREATE_FILE;


#pragma pack () /*取消指定对齐，恢复缺省对齐*/

#define FID_MF		0x3f00


#define SW_SUCCESS 						0x9000
#define SW_MF_NOTEXIST					0x6500
#define SW_VERIFY_FAIL				0x6283//验签失败
#define SW_FLASH_WRITEFAIL				0x6581//写入FLASH失败
#define SW_LCLE_ERROR					0X6700//长度错误
#define SW_INFO_NOINIT					0x6901//信息未设置
#define SW_FILE_TYPEERR					0x6981//文件类型不支持
#define SW_SECURITY_DISSATISFY			0x6982//安全状态不满足
#define SW_AUTH_LOCKED				0x6983//认证方法被锁定
#define SW_REFERENCEDATA_INVALID 		0x6984//已用数据无效
#define SW_CONDITION_DISSATISFY		0x6985//使用条件不满足
#define SW_COMMAND_FORBBIDON		0X6986//命令不允许
#define SW_SMOBJ_ERROR				0X6988//安全报文数据对象不正确
#define SW_DISSELCET_APP				0X698A//未选择应用
#define SW_DATA_PARAERR				0X6A80//数据字段不正确
#define SW_FUNCTION_NOTSURPORT		0X6A81//功能不被支持
#define SW_FILE_NOTEXIST				0X6A82//应用或文件不存在
#define SW_SPACE_NOTENOUGH			0X6A84//空间不够
#define SW_P1P2_ERROR					0x6a86//P1P2不正确
#define SW_DATA_NOTFOUND				0x6A88//应用的数据未找到
#define SW_APP_EXIST					0x6A89//应用已经存在
#define SW_APP_SELECTED				0X6A8A//应用已打开
#define SW_APP_NOTEXIST				0X6A8B//应用不存在
#define SW_KEY_NOTEXIST				0X6A8C//引用的对称密钥不存在
#define SW_DATA_ERR					0X6A8D//数据错误
#define SW_OFFSET_ERR					0X6B00//数据错误
#define SW_ALG_UNKNOWNERR					0X6F00//数据错误

#define SW_FINGER_SPACEFULL					0X9301//指纹个数已满

#define SW_FINGER_CONNETERR					0X9201//指纹采集出错
#define SW_FINGER_TIMEOUT						0X9206//指纹采集超时
#define SW_FINGER_EIGENVALUEERR				0X9202//特征值出错
#define SW_FINGER_ALG_UNLOAD					0X9203//指纹算法引擎未加载
#define SW_FINGER_IDUNMATCH					0X9207//指纹算法错误
#define SW_FINGER_ALG_ERR						0X9208//指纹算法错误
#define SW_FINGER_EIGENVALUEINVALID			0X9012//表明模板非法 或 特征值非法，注册模板失败


void cos_version(unsigned char ver[2]);
U16 cos_dev_format(U8 *pinfo);
unsigned short cos_set_keynum(unsigned char flag,unsigned char *pbin);
unsigned short cos_get_keynum(unsigned char flag,unsigned char *pbout,unsigned int *poutlen);
void cos_getsizeinfo(unsigned int *total,unsigned int *rest);
unsigned char cos_getsecurestate(void);
unsigned int cos_sectortoaddr(unsigned short sector_num);
void cos_updatesectorstate(unsigned short sector_num,unsigned char status);
unsigned short cos_check_space(unsigned short size,unsigned short *sector_list);
unsigned int cos_malloc_space(unsigned short *sector_list);
void cos_updatesectorlist(unsigned short *sector_list,unsigned char status);
unsigned int cos_dataaddr(unsigned int startaddr, unsigned int offset);
void cos_checksectorlist(void);

void cos_init(unsigned char flag);

unsigned short cos_writedata(unsigned int addr,unsigned char *buff,unsigned short len);
unsigned short cos_init_dev_authkey(unsigned char *pbin);
unsigned short cos_dev_auth(unsigned char *pbin,unsigned char *pbrand,unsigned char algtype);
unsigned short cos_veryfypin(unsigned char *pbin,unsigned char *pbrand,unsigned char pintype,unsigned char algtype);
unsigned short cos_creat_initMF(void);
unsigned short cos_check_MF(void);
unsigned short cos_check_auth(unsigned char filetyp,unsigned char flag);
unsigned short cos_check_FIDok(unsigned char uchfid[2],unsigned char ftype);
unsigned short cos_instantiation_file(unsigned char *pbfile,unsigned short file_size);
unsigned short cos_write_filedata(unsigned char filetype,unsigned short offset,unsigned char *pbdata,unsigned short file_size);
unsigned short cos_creat_app(unsigned char fid[2],unsigned char auth[3],unsigned char name[32],unsigned char *pbpininfo);
unsigned short cos_open_app(unsigned char *pbin,unsigned char type);
unsigned short cos_creat_file(CREATE_FILE *p_create_file);
unsigned short cos_open_file(unsigned char *pbin,unsigned char type);
unsigned short cos_write_file(unsigned short offset,unsigned char *pbindata,unsigned short filelen);

unsigned short cos_SM2encrypt(unsigned char *pkey,unsigned char *pin,unsigned int len,unsigned char *pout,unsigned char flag);

void printfsector(unsigned short *sector_list);
#endif

