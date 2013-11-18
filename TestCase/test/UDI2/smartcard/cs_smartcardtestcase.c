/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_smartcardtestcase.h"
#include "udi2_smc.h"
#include "udi2_os.h"
#include "udiplus_debug.h"
#include "udiplus_os.h"
#include "cs_testkit.h"

#define T1                    (1)
#define T14                   (14)

#define MAX_TEST_CMD_NUM      (4)
#define MAX_TEST_EXCCMD_LEN   (160) /*用于测试数据交互的命令长度*/
#define MAX_TEST_CAPCMD_LEN   (140) /*用于测试性能的命令长度*/
#define MAX_TEST_EXCHANGE_TIME (200) /*交互指令所需的最长时间，经验值*/

#define ONE_HOUR_MILLISECONDS (3600000)


typedef enum
{
    E_QSTART=0,
    E_QEND,
    E_QCHECK,
    E_QCLEAR,
    E_QSET,
    E_CNT,
    E_CNTCHECK,
    E_QMAX
}E_QTYPE;

typedef enum 
{
	EM_TEST_CONAX,
	EM_TEST_VIACCESS,
	EM_TEST_TF,
	EM_TEST_DVN,
	EM_TEST_CryptoWorks,
	EM_TEST_Mediaguard,
	EM_TEST_SUMAVISION,
	EM_TEST_SUANTONG,
	EM_TEST_NAGRAVISION,
	EM_TEST_IRDETO,
	EM_TEST_IRDET14,
	EM_TEST_COMVENIENT,
	EM_TEST_MAX	
}TestCaType_E;


typedef struct
{
	unsigned char aaCommand[2*MAX_TEST_CMD_NUM][MAX_TEST_EXCCMD_LEN];
}CommandExchange_S;

/*DVN 命令结构体*/
typedef struct
{
	unsigned char aaDVNCommand[2][MAX_TEST_EXCCMD_LEN];
}DVNCommand_S;

typedef struct
{
	unsigned char	dwLen[2];
	unsigned char	acCommand[256];
}CaProtocalTestCommand_S;

/*各个卡支持的参数*/
static CSUDISMCParam_S g_sCardPara[EM_TEST_MAX]=
{
	/*CONAX*/
	{372, 12096,4500000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T0, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_EVEN_PARITY},
	/*VIACCESS*/
	{372, 12096,4500000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T0, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_ODD_PARITY},
	/*TF*/
	{372, 12096,4500000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T0, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_EVEN_PARITY},
	/*DVN*/
	{372, 12096,4500000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T0, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_EVEN_PARITY},
	/*CryptoWorks*/
	{372, 9600,	3579000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T0, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_EVEN_PARITY},
	/*Mediaguard*/
	{372, 9600, 3579000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T0, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_EVEN_PARITY},
	/*SUMAVISION*/
	{372, 12096,	4500000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T0, EM_UDISMC_STOP_2, EM_UDISMC_8BITS_EVEN_PARITY},
	/*SUANTONG*/
	{372, 12096,4500000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T1, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_EVEN_PARITY},
	/*NAGRAVISION*/
	{372, 12096,4500000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T1, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_EVEN_PARITY},
	/*IRDETO*/
	{372, 12096,4500000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T0, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_NO_PARITY},
	/*IRDET14*/
	{625, 9600,	6000000, EM_UDISMC_STANDARD_IRDETO, EM_UDISMC_PROTOCOL_T14, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_NO_PARITY},
	/*comvenient*/
	{372, 12096,4500000, EM_UDISMC_STANDARD_ISO, EM_UDISMC_PROTOCOL_T1, EM_UDISMC_STOP_1, EM_UDISMC_8BITS_NO_PARITY},	
	
};


/*******************************************************************
 NOTIFY:
		g_acSmc_Test_cmd[even][]:---cmd1 send
		g_acSmc_Test_cmd[odd][]:---cmd1 reply		
 NOTIFY:
		g_acSmc_Test_cmd[][0~1]: cmd lenth 
 ***********************************************************************/
/*各个卡用于测试数据交互的命令,四条*/
static CommandExchange_S g_sExchangeCommand[EM_TEST_MAX] = 
{	
	{/*CONAX*/
		{
			{0x00, 0x08, 0xdd, 0x26, 0x00, 0x00, 0x03, 0x10, 0x01, 0x40},
			{0x00, 0x02, 0x98, 0x11},
			{0x00, 0x05, 0xdd ,0xca ,0x00 ,0x00 ,0x11},
			{0x00, 0x13, 0x20,  0x01,  0x40,  0x28,  0x02,  0x0b,  0x00,  0x2f,  0x02,  0x00,  0x2c,  0x30,  0x01,  0x08,  0x23,  0x01,  0x03,  0x90,  0x00},
			{0x00, 0x07, 0xdd ,0xc2 ,0x00 ,0x00 ,0x02 ,0x66 ,0x00},
			{0x00, 0x02, 0x98, 0x06},
			{0x00, 0x05, 0xdd ,0xca ,0x00 ,0x00 ,0x06},
			{0x00, 0x08, 0x74 ,0x04 ,0x71 ,0x3f ,0xb3 ,0x00,0x90, 0x00}
		}
	},	
	{/*VIACCESS*/
		{
			{0x00,0x04,0xca, 0xac, 0xa4, 0x00},//test the 4 chars apdu!
			{0x00,0x02,0x90, 0x00},
			{0x00,0x05,0xca, 0xb8, 0x00, 0x00, 0x07},
			{0x00,0x09,0xa4, 0x05, 0x00, 0x00, 0x00, 0x02, 0xcb,0x90, 0x08},
			{0x00,0x04,0xca, 0xac, 0x08, 0x00},
			{0x00,0x02,0x90, 0x00},
			{0x00,0x05,0xca, 0xb8, 0x00, 0x00, 0x03},
			{0x00,0x05,0x08, 0x01, 0x00, 0x90, 0x08}
		}
	},	
	/*TF*/
	{
		{
			{0x00,0x09,0x80, 0x46, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x08},
			{0x00,0x02,0x61, 0x08},
			{0x00,0x05,0x00, 0xc0, 0x00, 0x00, 0x08},
			{0x00,0x0a,0x90, 0x56, 0xaf, 0xd1, 0x14, 0x38, 0x1b, 0x8f, 0x90, 0x00},
			{0x00,0x0a,0x00, 0xa4, 0x04, 0x00, 0x05, 0xf9, 0x5a, 0x54, 0x00, 0x06},
			{0x00,0x02,0x90, 0x00},
			{0x00,0x09,0x80, 0x46, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x14},
			{0x00,0x02,0x61, 0x14}
		}
	},	
	/*DVN没有用到*/
	{
		{
			{0x00,0x05,0x84,  0xaa,  0x00,  0x00,  0x30},
			{0x00,0x01,0xaa},
			{0x00,0x32,0x21, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
				 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
				 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
				 0x90, 0x00},
			{0x00,0x37,0x84,  0xaa,  0x00,  0x00,  0x30, 0xe1, 0x20, 0x00, 0x00, 0x01, 0x03, 0x53, 0x44, 0x48, 0x16, 0x28, 0x30, 0x00, 0x00, 0x00, 0x00, 
				 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			 	 0x00, 0x00, 0x00, 0x00, 0x44, 0xef, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,0x90,  0x00},
			{0x00,0x05,0x84,  0xaa,  0x00,  0x00,  0x10},
			{0x00,0x01,0xaa},
			{0x00,0x12,0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x74, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
			 	 0x90, 0x00},
			{0x00,0x17,0x84,  0x45,  0x00,  0x00,  0x10,  0xd8,  0x27,  0x9d,  0xd9,  0x5b,  0x8a,  0xc2,  0x43,  0x25,  0x54,  0x7b,  0x09,  
			     0xe6,  0x44,  0xaa,  0x95,  0x90,  0x00}
		}
	},	
	/*CryptoWorks*/
	{
		{
			//{0x00,0x05,0xa4, 0xc0, 0x00, 0x00, 0x11},
			//{0x00,0x13,0xdf, 0x0f, 0x05, 0x04, 0x00, 0x09, 0x3f, 0x00, 0x01, 0x00, 0xf0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x90, 0x00},
			{0x00,0x07,0xa4, 0xa4, 0x00, 0x00, 0x02, 0x3f, 0x00},
			{0x00,0x02,0x9f, 0x11},
			{0x00,0x07,0xa4, 0xa4, 0x00, 0x00, 0x02, 0x2f, 0x01},
			{0x00,0x02,0x9f, 0x11},
			{0x00,0x06,0xa4, 0xa2, 0x00, 0x00, 0x01, 0xd1},
			{0x00,0x02,0x9f, 0x04},
			{0x00,0x05,0xa4, 0xb2, 0x00, 0x00, 0x04},
			{0x00,0x06,0xd1, 0x02, 0x0d, 0x00,0x90, 0x00}
		}
	},
	/*Mediaguard*/
	{
		{
			{0x00,0x05,0xc1, 0x0e, 0x00, 0x00, 0x08},
			{0x00,0x0a,0x00, 0x00, 0x00, 0x00, 0x09, 0x97, 0xd1, 0xfe, 0x90, 0x00},
			{0x00,0x15,0xc1, 0x30, 0x00, 0x00, 0x10, 0x01, 0x01, 0x01, 0x02, 0x03, 0x07, 0x17, 0xa4, 0x01, 0x01, 0x01,0x02, 0x03, 0x07, 0x17, 0xa4},
			{0x00,0x02,0x97, 0x01},
			{0x00,0x05, 0xc1, 0x16, 0x00, 0x00, 0x06},
			{0x00,0x08,0x00, 0x00, 0x00, 0x3f, 0x09, 0x10,0x90, 0x00},
			{0x00,0x05,0xc1, 0x12, 0x05, 0x00, 0x19},
			{0x00,0x1b,0x00, 0x90, 0x4f, 0x50, 0x2d, 0x54, 0x45, 0x53, 0x54, 0x2d, 0x35, 0x20, 0x20, 0x20, 0x20, 0x20,0x20, 0x20, 0xac, 0xeb, 0xaf, 0xfe, 0x19, 0x9f, 0x00, 0x90, 0x00}
		}
	},	
	/*SUMAVISION*/
	{
		{
			{0x00,0x07,0x00, 0xa4, 0x04, 0x00, 0x02, 0x3f, 0x00},
			{0x00,0x02,0x90, 0x00},
			{0x00,0x07,0x00, 0xa4, 0x04, 0x00, 0x02, 0x4a, 0x00},
			{0x00,0x02,0x90, 0x00},
			{0x00,0x0b,0x00, 0xb2, 0x00, 0x05, 0x06, 0x00, 0x01, 0xff, 0x00, 0x01, 0xff},
			{0x00,0x02,0x61, 0x67},
			{0x00,0x05,0x00, 0xc0, 0x00, 0x00, 0x67},
			{0x00,0x69,0x00,  0x01,  0x64,  0x00,  0x06,  0xf7,  0x09,  0x45,  0x64,  0x57,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
				 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
				 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
				 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  
				 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x44,  0x56,  0x54,  0x00,  0xfd,  0xfd,  0xfd,  0xfd,  0xdd,  0xdd,  
				 0xdd,  0xdd,  0xdd,  0xdd,  0xdd,  0xdd,  0xdd,  0xdd,  0xdd,  0xdd,  0x00,  0x00,  0x00,  0x02,  0x00,  0x00,  
				 0x00,  0x03,  0x57,  0xa8,  0x40,  0x5c,  0x90,  0x00}
		}
	},
	/*SUANTONG*/
	{
		{
			{0x00,0x06,0x00, 0x32, 0x10, 0x01, 0x01, 0x01},
			{0x00,0x08,0x00, 0x00, 0x02, 0x90, 0x00, 0x92, 0x00, 0x00},
			{0x00,0x05,0x00, 0x84, 0x00, 0x00, 0x10},
			{0x00,0x18,0x00, 0x00, 0x12, 0xd0, 0x9d, 0x75, 0xb4, 0xcf, 0x06, 0xd4, 0xa9, 0x5c, 0x02, 0x5f, 0x56, 0xb1, 0xa9, 0xf1, 0xf3, 0x90, 0x00, 0xf7,0x00, 0x00},
			{0x00,0x0e,0x00, 0x0c, 0x00, 0x00, 0x10, 0xd0, 0x9d, 0x75, 0xb4, 0xcf, 0x06, 0xd4, 0xa9, 0x00},
			{0x00,0x10,0x00, 0x00, 0x0a, 0xda, 0x6e, 0xa8, 0xb3, 0x49, 0x7e, 0xd4, 0x64, 0x90, 0x00, 0xb2,0x00, 0x00},						
			{0x00,0x0e,0x00, 0x0b, 0x00, 0x00, 0x10, 0xda, 0x6e, 0xa8, 0xb3, 0x49, 0x7e, 0xd4, 0x64, 0x00},
			{0x00,0x08,0x00, 0x00, 0x02, 0x90, 0x00, 0x92,0x00,0x00}
		}
	},
	/*NAGRAVISION*/
	{
		{
			{0x00,0x0c,0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87},
			{0x00,0x0e,0x12 ,0x00 ,0x08 ,0xb0 ,0x04 ,0x00 ,0xd3 ,0x00 ,0x20 ,0x90 ,0x00 ,0xcd, 0x00,0x00},
			{0x00,0x05,0x21 ,0xc1 ,0x01 ,0xfe ,0x1f},
			{0x00,0x07,0x12 ,0xe1 ,0x01 ,0xfe ,0x0c,0x00,0x00},
			{0x00,0x0c,0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x12 ,0x00 ,0x06 ,0x15},
			{0x00,0x0e,0x12 ,0x40 ,0x08 ,0x92 ,0x04 ,0x05 ,0xe1 ,0x00 ,0x4c ,0x90 ,0x00 ,0xf4,0x00,0x00},
			{0x00,0x0c,0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x15 ,0x00 ,0x08 ,0x5c},
			{0x00,0x10,0x12 ,0x00 ,0x0a ,0x95 ,0x06 ,0x17 ,0x20 ,0x20 ,0x01 ,0x01 ,0x01 ,0x90 ,0x00 ,0x0d,0x00,0x00}
		}
	},
	/*IRDETO*/
	{
		{
			{0x00,0x06,0xd2, 0x42, 0x00, 0x00, 0x01, 0x1d},
			{0x00,0x02,0x90, 0x19},
			{0x00,0x05,0xd2, 0xfe, 0x00, 0x00, 0x19},
			{0x00,0x1b,0x01,  0x02,  0x00,  0x00,  0x21,  0x00,  0x00,  0x10,  0x02,  0x1a,  0x00,  0xff,  0xff,  0x00,  0x00,  0x03,  0x00, 0x5a,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x4c,  0x90,  0x00},
			{0x00,0x06,0xd2, 0x3c, 0x00, 0x00, 0x01, 0x22},
			{0x00,0x02,0x90, 0x15},
			{0x00,0x05,0xd2, 0xfe, 0x00, 0x00, 0x15},
			{0x00,0x17,0x01, 0x02, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x0c, 0x54, 0x39,0x01, 0x93, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8,0x90, 0x00}
		}
	},
	/*IRDET14*/
	{
		{
			{0x00,0x07,0x01,  0x02,  0x02,  0x03,  0x00,  0x00,  0x3d},
			{0x00,0x1b,0x01,  0x02,  0x00,  0x00,  0x02,  0x03,  0x00,  0x10,  0x05,  0x05,  0x12,  0x06,  0x01,  0x06,  0x02,  0x06,  0x03, 0x06,  0x04,  0x00,  0x00,  0x4e,  0x30,  0x31,  0x74,  0x00,  0x00},
			{0x00,0x07,0x01,  0x02,  0x00,  0x03,  0x00,  0x00,  0x3f},
			{0x00,0x1f,0x01,  0x02,  0x00,  0x00,  0x00,  0x03,  0x00,  0x14,  0x34,  0x30,  0x30,  0x32,  0x35,  0x34,  0x39,  0x34,  0x38, 0x36,  0x54,  0x33,  0x33,  0x32,  0x30,  0x32,  0x41,  0x00,  0x00,  0x00,  0x0a,  0x00,  0x00},
			{0x00,0x07,0x01,  0x02,  0x01,  0x03,  0x00,  0x00,  0x3e},
			{0x00,0x1b,0x01,  0x02,  0x00,  0x00,  0x01,  0x03,  0x00,  0x10,  0xff,  0xff,  0xff,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x04,  0x07,  0x84,  0xf1,  0xd3,  0x18,  0x6c,  0x00,  0x00},
			{0x00,0x07,0x01,  0x02,  0x03,  0x03,  0x00,  0x00,  0x3c},
			{0x00,0x23,0x01,  0x02,  0x00,  0x00,  0x03,  0x03,  0x00,  0x18,  0x00,  0x04,  0x04,  0x7f,  0x00,  0x00,  0x00,  0x00,  0x00, 0x00,  0x01,  0xcb,  0x0c,  0x01,  0x2d,  0x2d,  0x2d,  0x82,  0x62,  0x00,  0x00,  0x00,  0x00,  0x00,  0x51,  0x00,  0x00}
		}
	},
	/*comvenient*/
	{
		{
			{0x00,0x34,0xd1, 0x28, 0x31, 0x13, 0x28, 0x43, 0x29, 0x20, 0x20, 0x63, 0x6f, 0x6d, 0x76, 0x65,
			0x6e, 0x69,0x65, 0x6e, 0x74, 0x20, 0x2c, 0x20, 0x62, 0x65, 0x74, 0x61, 0x63, 0x72, 0x79, 0x70,
			0x74, 0x20,0x62, 0x79, 0x20, 0x20, 0x63, 0x6f, 0x6d, 0x76, 0x65, 0x6e, 0x69, 0x65, 0x6e, 0x74,
			0x20, 0x20,0x32, 0x30, 0x30, 0x33},			
			{0x00,0x7b,0xd1, 0x28, 0x0b, 0x10, 0x50, 0x38, 0x57, 0x45, 0x35, 0x30, 0x31, 0x37, 0x2d, 0x31,
			0x11, 0x11,0x42, 0x65, 0x74, 0x61, 0x43, 0x72, 0x79, 0x70, 0x74, 0x20, 0x49, 0x49, 0x2d, 0x31,
			0x2e, 0x32,0x0b, 0x12, 0x36, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x36, 0x35, 0x31, 0x13,
			0x28, 0x43,0x29, 0x20, 0x42, 0x65, 0x74, 0x61, 0x52, 0x65, 0x73, 0x65, 0x61, 0x72, 0x63, 0x68,
			0x2c, 0x20,0x62, 0x65, 0x74, 0x61, 0x63, 0x72, 0x79, 0x70, 0x74, 0x20, 0x62, 0x79, 0x20, 0x42,
			0x65, 0x74,0x61, 0x52, 0x65, 0x73, 0x65, 0x61, 0x72, 0x63, 0x68, 0x20, 0x32, 0x30, 0x30, 0x30,
			0x1a, 0x0f,0x31, 0x2e, 0x32, 0x2e, 0x30, 0x2e, 0x39, 0x38, 0x20, 0x32, 0x30, 0x30, 0x32, 0x2d,
			0x30, 0x32,0x2d, 0x32, 0x31, 0x20, 0x31, 0x37, 0x3a, 0x33, 0x38, 0x00, 0x00},
 
			{0x00,0x34,0xd1, 0x28, 0x31, 0x13, 0x28, 0x43, 0x29, 0x20, 0x42, 0x65, 0x74, 0x61, 0x52, 0x65,
			0x73, 0x65,0x61, 0x72, 0x63, 0x68, 0x2c, 0x20, 0x62, 0x65, 0x74, 0x61, 0x63, 0x72, 0x79, 0x70,
			0x74, 0x20,0x62, 0x79, 0x20, 0x42, 0x65, 0x74, 0x61, 0x52, 0x65, 0x73, 0x65, 0x61, 0x72, 0x63,
			0x68, 0x20,0x32, 0x30, 0x30, 0x32},
			{0x00,0x7b,0xd1, 0x28, 0x0b, 0x10, 0x50, 0x38, 0x57, 0x45, 0x35, 0x30, 0x31, 0x37, 0x2d, 0x31,
			0x11, 0x11,0x42, 0x65, 0x74, 0x61, 0x43, 0x72, 0x79, 0x70, 0x74, 0x20, 0x49, 0x49, 0x2d, 0x31,
			0x2e, 0x32,0x0b, 0x12, 0x36, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x36, 0x35, 0x31, 0x13,
			0x28, 0x43,0x29, 0x20, 0x42, 0x65, 0x74, 0x61, 0x52, 0x65, 0x73, 0x65, 0x61, 0x72, 0x63, 0x68,
			0x2c, 0x20,0x62, 0x65, 0x74, 0x61, 0x63, 0x72, 0x79, 0x70, 0x74, 0x20, 0x62, 0x79, 0x20, 0x42,
			0x65, 0x74,0x61, 0x52, 0x65, 0x73, 0x65, 0x61, 0x72, 0x63, 0x68, 0x20, 0x32, 0x30, 0x30, 0x30,
			0x1a, 0x0f,0x31, 0x2e, 0x32, 0x2e, 0x30, 0x2e, 0x39, 0x38, 0x20, 0x32, 0x30, 0x30, 0x32, 0x2d,
			0x30, 0x32,0x2d, 0x32, 0x31, 0x20, 0x31, 0x37, 0x3a, 0x33, 0x38, 0x00, 0x00},
			
			{0x00,0x02,0xd1, 0x10},
			{0x00,0x06,0xd1, 0x10, 0x17, 0x24, 0x00, 0x00},
			
			{0x00,0x4f,0xd1, 0x0e, 0x00, 0xf9, 0x6c, 0x26, 0x67, 0x4f, 0xe6, 0x37, 0xa9, 0x81, 0x9a, 0xcb,
			0x63, 0x3a,0x4e, 0x4b, 0xf1, 0x2d, 0x83, 0x8b, 0x74, 0x0d, 0x50, 0x7d, 0xb7, 0x6c, 0x47, 0xb7,
			0xed, 0x26,0xf8, 0xc2, 0x6e, 0x63, 0xba, 0x98, 0xfd, 0xd9, 0x1c, 0x30, 0xf5, 0x95, 0xee, 0xc2,
			0x82, 0x9d,0xc8, 0xb8, 0x29, 0x83, 0xf0, 0x48, 0xbd, 0xde, 0x0d, 0xb0, 0x82, 0x33, 0x9d, 0xd4,
			0xc7, 0x47,0xe2, 0x9c, 0xf2, 0x67, 0xfd, 0xd6, 0x2d, 0xc5, 0x8c, 0x1b, 0x61, 0x0a, 0x79, 0xa6,
			0xe9},
			{0x00,0x20,0xd1, 0x0e, 0x77, 0x4f, 0x97, 0xc2, 0x3e, 0x2e, 0x43, 0x2e, 0xfe, 0x59, 0x8f, 0x75,
			0x63, 0x0e,0x55, 0xe6, 0x69, 0xb2, 0x36, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x33, 0x36, 0x35,
			0x00, 0x00}
		}
	},	
};

/*各个卡用于测试性能的命令，一条*/
static unsigned char g_TestCapCmd[EM_TEST_MAX][MAX_TEST_CAPCMD_LEN]=
{	
	/*CONAX*/
	{
	 0x00,0x75,
	 0xdd ,0x84 ,0x00 ,0x00 ,0x70 ,0x12 ,0x6e ,0x82 ,0x70 ,0x6b ,0x00 ,0x00 ,0x00 ,0x71 ,0x3f ,0xb6 ,
	 0xe8 ,0x70 ,0x62 ,0x64 ,0x00 ,0x29 ,0x36 ,0x28 ,0xaf ,0x07 ,0x8b ,0x72 ,0xb6 ,0xac ,0x8e ,0x22 ,
	 0x1b ,0x64 ,0xf2 ,0x2c ,0x0c ,0xaf ,0xd9 ,0x79 ,0x54 ,0x6d ,0x4f ,0x92 ,0xd1 ,0xd8 ,0x11 ,0xe9 ,
	 0xbd ,0x22 ,0xda ,0xa2 ,0x08 ,0x76 ,0x43 ,0x64 ,0xec ,0xde ,0x31 ,0x0a ,0xca ,0xf3 ,0x7d ,0xd5 ,
	 0x3c ,0x84 ,0xdd ,0x13 ,0x49 ,0xe1 ,0x87 ,0x5d ,0x6d ,0xa1 ,0x61 ,0xdf ,0x21 ,0xdc ,0xd2 ,0xba ,
	 0x16 ,0xef ,0x3a ,0xe5 ,0xf4 ,0x49 ,0xd8 ,0xa9 ,0xb2 ,0xab ,0xf8 ,0xcb ,0x1d ,0x18 ,0x3b ,0x63 ,
	 0x66 ,0x69 ,0x73 ,0xc1 ,0x04 ,0xf8 ,0xc0 ,0x61 ,0xcb ,0x66 ,0x1b ,0x2f ,0x54 ,0x92 ,0x32 ,0x62 ,
	 0x2c ,0x53 ,0x9b ,0x2f ,0xfc
	 },
	/*VIACCESS*/
	{
	 0x00,0x04,
	 0xca, 0xac, 0xa4, 0x00
	 },
	/*TF*/
	{
	 0x00, 0x09,
	 0x80, 0x46, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x08	 
	},
	/*DVN没有用到，DVN不测性能*/
	{
	 0x00, 0x42,
	 0xe7, 0x0b, 0x18, 0xd2, 0x89, 0x65, 0x4d, 0x98, 0xab, 0xbc, 0x96, 0xaa, 0x4b, 0xc8, 0x6a, 0x99, 
	 0x81, 0x28, 0x08, 0x6e, 0x39, 0x9d, 0x85, 0x41, 0x0a, 0xe8, 0xef, 0x4e, 0x9f, 0x8b, 0x08, 0xd7, 
 	 0x30, 0x91, 0xc0, 0x70, 0xd7, 0xc3, 0x18, 0xa9, 0x25, 0x9d, 0x1a, 0x11, 0x86, 0x6b, 0x81, 0x69, 
 	 0xe5, 0x77, 0x20, 0x09, 0xa4, 0x9f, 0xf3, 0xdf, 0x37, 0x2f, 0x58, 0x01, 0xd6, 0x41, 0xeb, 0x05, 
 	 0x90, 0x00
 	},
	/*CryptoWorks*/
	{
	 0x00, 0x05,
	 0xa4, 0xc0, 0x00, 0x00, 0x11,
	},	
	/*Mediaguard*/
	{0x00, 0x05,
	 0xc1, 0x12, 0x05, 0x00, 0x19},
	/*SUMAVISION*/
	{
         0x00,0x07,
         0x00, 0xa4, 0x04, 0x00, 0x02, 0x3f, 0x00
	},
	/*SUANTONG*/
	{
        0x00,0x4e,
        0x80, 0xeb, 0x80, 0x00, 0x49, 0x00, 0x00, 0x00, 0x00, 0x01, 0xd9, 0x00, 0x38, 0x82, 0x3f, 0x90, 
        0x03, 0x00, 0x00, 0x10, 0x86, 0xfd, 0x71, 0x4b, 0xe2, 0xbc, 0x40, 0xf2, 0xd4, 0xbc, 0xe1, 0xc4, 
        0xa1, 0x70, 0xde, 0x54, 0xd6, 0xa8, 0x12, 0x21, 0xfd, 0x5e, 0x8c, 0x75, 0xf5, 0x1a, 0xd2, 0x8c, 
        0x17, 0x93, 0x98, 0xd7, 0xd7, 0xf9, 0xa3, 0xa2, 0x68, 0x67, 0xaa, 0xd2, 0x6b, 0x6b, 0x56, 0xd5, 
        0x38, 0x88, 0x25, 0x5c, 0x62, 0xc8, 0xf6, 0xa3, 0x3d, 0xcc, 0x73, 0x9d, 0xbb, 0x1e
    },
	/*NAGRAVISION*/
	{
	 0x00, 0x4c,
	 0x21 ,0x40 ,0x48 ,0xa0 ,0xca ,0x00 ,0x00 ,0x42 ,0x2b ,0x40 ,0x57 ,0xaa ,0x1e ,0x9e ,0x80 ,0x9b,
	 0x75 ,0x22 ,0x08 ,0xb8 ,0x49 ,0x1c ,0x1c ,0xfb ,0x44 ,0xc8 ,0x63 ,0x49 ,0x4f ,0x7f ,0x4f ,0x6d,
	 0xbd ,0x16 ,0xe0 ,0xf7 ,0xbb ,0x08 ,0x10 ,0x36 ,0x3d ,0xb8 ,0x2f ,0x5c ,0xb1 ,0xb6 ,0x79 ,0xb0,
	 0x80 ,0x75 ,0x6d ,0xe3 ,0x60 ,0x2b ,0x7c ,0xcc ,0x10 ,0xce ,0x01 ,0x98 ,0x42 ,0x4a ,0x5a ,0x95,
	 0x48 ,0xe5 ,0x72 ,0xc3 ,0x8e ,0x44 ,0x75 ,0xe6 ,0x9f ,0x85 ,0x02 ,0xa1
	},	
	/*IRDETO*/
	{
	 0x00,0x06,
	 0xd2, 0x42, 0x00, 0x00, 0x01, 0x1d
	},
	/*IRDET14*/
	{
	 0x00, 0x07,
	 0x01,  0x02,  0x08,  0x03,  0x00,  0x00,  0x37
	},
	/*COMVENIENT*/
	{
	0x00,0x34,
	0xd1, 0x28, 0x31, 0x13, 0x28, 0x43, 0x29, 0x20, 0x20, 0x63, 0x6f, 0x6d, 0x76, 0x65,
	0x6e, 0x69,0x65, 0x6e, 0x74, 0x20, 0x2c, 0x20, 0x62, 0x65, 0x74, 0x61, 0x63, 0x72, 0x79, 0x70,
	0x74, 0x20,0x62, 0x79, 0x20, 0x20, 0x63, 0x6f, 0x6d, 0x76, 0x65, 0x6e, 0x69, 0x65, 0x6e, 0x74,
	0x20, 0x20,0x32, 0x30, 0x30, 0x33
	},	
};


/*DVN命令，只保存发出的命令*/
static DVNCommand_S g_DVNExchangeCmd[MAX_TEST_CMD_NUM]=
{
	{
		{
			{0x00, 0x05, 0x84,  0xaa,  0x00,  0x00,  0x30},
			{0x00, 0x32, 0x21, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
			 0x90, 0x00}
		}
	}	,

	{
		{
			{0x00, 0x05, 0x84,  0xaa,  0x00,  0x00,  0x10},
			{0x00, 0x12, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x74, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	 		 0x90, 0x00}
		}
	},

	{
		{
			{0x00, 0x05, 0x84,  0xaa,  0x00,  0x00,  0x40},
			{0x00, 0x42, 0x59, 0x32, 0x00, 0x00, 0xf7, 0x8b, 0xfc, 0xbd, 0xf8, 0xf7, 0x4c, 0x6c, 0xf6, 0x88, 0xaf, 0xf9, 
			 0xb0, 0xe1, 0x64, 0x5c, 0xf7, 0x8b, 0xea, 0x9e, 0x92, 0x7d, 0xb9, 0xae, 0x79, 0xe2, 0xe8, 0xee, 
			 0x37, 0x99, 0x5e, 0xcd, 0xd9, 0x4e, 0x18, 0x45, 0x6c, 0xe7, 0x4f, 0x66, 0x2f, 0x4d, 0x28, 0xe8, 
			 0x10, 0xbd, 0xfa, 0x4e, 0xfe, 0x26, 0xbd, 0xc5, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
			 0x90, 0x00}
		}
	},

	{
		{
			{0x00, 0x05, 0x84,  0xaa,  0x00,  0x00,  0x10},
			{0x00, 0x12, 0x58, 0x02, 0x00, 0x00, 0xf7, 0x8b, 0x8f, 0x72, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
			 0x90, 0x00}	
		}
	}
};

static CaProtocalTestCommand_S gsT0Command_TF[]=
{
	{{0 ,  9},{ 0x80, 0x46, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x08 }},
	{{0 ,  5},{ 0x00, 0xc0, 0x00, 0x00, 0x08 }},
	{{0 , 10},{ 0x00, 0xa4, 0x04, 0x00, 0x05, 0xf9, 0x5a, 0x54, 0x00, 0x06 }},
	{{0 ,  9},{ 0x80, 0x46, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x14 }},
	{{0 ,  5},{ 0x00, 0xc0, 0x00, 0x00, 0x14}},
	{{0 ,  9},{ 0x80, 0x46, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0x09}},
};

static CaProtocalTestCommand_S gsT1Command[]=
{
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,5}, {0x21 ,0xc1 ,0x01 ,0xfe ,0x1f }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x12 ,0x00 ,0x06 ,0x15 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x15 ,0x00 ,0x08 ,0x5c }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0xc2 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x00 ,0x1c ,0x3e }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0xc0 ,0x2e ,0x8c }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x80 ,0x1c ,0xbe }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x2a ,0x00 ,0x42 ,0x29 }},
	{{0,76},{0x21 ,0x40 ,0x48 ,0xa0 ,0xca ,0x00 ,0x00 ,0x42 ,0x2b ,0x40 ,0x95 ,0x69 ,0xe6 ,0xc9 ,0xa6 ,0x40 ,
	      0xe7 ,0xdd ,0xa9 ,0x38 ,0xc2 ,0xa4 ,0xdb ,0x6d ,0x55 ,0xc1 ,0x69 ,0x7b ,0xad ,0x64 ,0xa4 ,0x51 , 
		  0x76 ,0x56 ,0x77 ,0xa2 ,0x90 ,0x19 ,0x29 ,0x67 ,0x8e ,0x54 ,0x83 ,0x23 ,0x69 ,0x09 ,0x79 ,0x00 ,
		  0x1a ,0x44 ,0x0f ,0xd4 ,0x72 ,0x68 ,0xb1 ,0xf3 ,0x94 ,0x3a ,0x62 ,0x63 ,0xee ,0xcb ,0x19 ,0x99 ,
		  0x2d ,0x7a ,0x83 ,0xa9 ,0xbf ,0xbb ,0xed ,0x04 ,0x28 ,0x4f ,0x02 ,0xed }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0x82 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x08 ,0x03 ,0x69 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0xc2 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x01 ,0x0e ,0x2d }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x81 ,0x0e ,0xed }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0xc2 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x07 ,0x20 ,0x05 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0x82 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x04 ,0x18 ,0x7e }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x84 ,0x18 ,0xbe }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0xc4 ,0x44 ,0xe2 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x84 ,0x18 ,0xbe }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0x82 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x05 ,0x57 ,0x30 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x85 ,0x57 ,0xf0 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x85 ,0x57 ,0xb0 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x85 ,0x57 ,0xf0 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x85 ,0x57 ,0xb0 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x85 ,0x57 ,0xf0 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x85 ,0x57 ,0xb0 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0xc2 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x03 ,0x1a ,0x3b }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0x82 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x02 ,0x57 ,0x37 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0xc2 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x06 ,0x16 ,0x32 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x86 ,0x16 ,0xf2 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0xc2 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x09 ,0x03 ,0x28 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0x82 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x0a ,0x03 ,0x6b }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0xc2 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x0b ,0x03 ,0x2a }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0x82 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x0c ,0x03 ,0x6d }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0xc2 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x0d ,0x03 ,0x2c }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0x82 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,13},{0x21 ,0x00 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x0e ,0x03 ,0x6f }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0xc2 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,13},{0x21 ,0x40 ,0x09 ,0xa0 ,0xca ,0x00 ,0x00 ,0x03 ,0x22 ,0x01 ,0x0f ,0x03 ,0x2e }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0x82 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc7 ,0x00 ,0x04 ,0x82 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x77 ,0xf0 ,0x21 ,0xe6 ,0x15 ,0x98 
		 ,0x4b ,0x75 ,0xb9 ,0x4f ,0xe6 ,0x74 ,0xc7 ,0xf9 ,0x0e ,0x27 ,0xc8 ,0x43 ,0xd4 ,0xd1 ,0x25 ,0xca 
	  	 ,0xee ,0x15 ,0xa2 ,0x18 ,0x8a ,0x04 ,0xbd ,0x91 ,0xdc ,0x9a ,0xdb ,0xa4 ,0xf8 ,0xc6 ,0x2f ,0xd7 
	  	 ,0xee ,0xd9 ,0x33 ,0x18 ,0xf8 ,0xc9 ,0xa6 ,0xfd ,0x0d ,0x2d ,0x26 ,0x89 ,0x5e ,0x45 ,0x49 ,0xe3 
	  	 ,0x49 ,0xdf ,0xe9 ,0x22 ,0x03 ,0x88 ,0x3d ,0x2d ,0x4e ,0x85 ,0x07 ,0x1e ,0xc1 ,0xe6 ,0x1e ,0x04 
	  	 ,0x1b ,0x99 ,0x12 ,0x7d ,0x54 ,0xd0 ,0xfe ,0x03 ,0x88 ,0x95 ,0xe5 ,0xdd ,0xc6 ,0x38 ,0xb9 ,0xa6 
	  	 ,0xe0 ,0x40 ,0x00 ,0xbf ,0x19 ,0xde ,0x81 ,0x13 ,0x14 ,0xad ,0x19 ,0xf6 ,0xd1 ,0x88 ,0x14 ,0xa8 
	  	 ,0xd3 ,0x3b ,0xff ,0x61 ,0xe3 ,0xc8 ,0x3a ,0x87 ,0x62 ,0x59 ,0x44 ,0xe5 ,0x6f ,0x8f ,0xdd ,0x1d 
	  	 ,0x5e ,0x01 ,0x3e ,0x80 ,0xec ,0x1a ,0xd0 ,0xf5 ,0x74 ,0x94 ,0xad ,0x39 ,0xe0 ,0x68 ,0x41 ,0x1b 
	  	 ,0x32 ,0x6b ,0x1c ,0xf7 ,0xc8 ,0x22 ,0x22 ,0x02 ,0x58 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xff ,0x54 ,0x2f ,0xbc ,0x56 ,0xc5 
		 ,0xb2 ,0xcf ,0xca ,0x80 ,0xe3 ,0xcc ,0x9e ,0x8b ,0xcf ,0xf2 ,0x13 ,0xb3 ,0x18 ,0xe2 ,0xb4 ,0x7d 
		 ,0xfd ,0x50 ,0x2a ,0x0a ,0x13 ,0xc1 ,0x98 ,0x41 ,0xfa ,0x42 ,0x1e ,0x7a ,0x38 ,0x90 ,0x09 ,0x53 
		 ,0x4a ,0x48 ,0x99 ,0x25 ,0xc8 ,0x18 ,0x43 ,0x9f ,0xbf ,0xa6 ,0xfc ,0x0f ,0x3c ,0x81 ,0x14 ,0x33 
		 ,0x8b ,0x06 ,0xa9 ,0xeb ,0xe4 ,0xf6 ,0x2c ,0xb9 ,0xad ,0x42 ,0x19 ,0xe9 ,0x20 ,0xe1 ,0xb7 ,0xea 
		 ,0x3a ,0x17 ,0xdb ,0x90 ,0x98 ,0x81 ,0xb3 ,0x2f ,0x6e ,0x91 ,0xa5 ,0xe6 ,0x02 ,0xf5 ,0x63 ,0xd3 
		 ,0xf5 ,0x59 ,0xab ,0x2f ,0xf3 ,0x4f ,0x88 ,0x5d ,0xf3 ,0x65 ,0x28 ,0x5d ,0xaa ,0x35 ,0x02 ,0xe1 
		 ,0x4d ,0x82 ,0xb2 ,0xc5 ,0xa5 ,0xbe ,0x4d ,0xa8 ,0x9a ,0xb1 ,0x9d ,0x36 ,0x1a ,0x37 ,0xa5 ,0x67 
		 ,0x73 ,0xdf ,0x9c ,0xb1 ,0xf1 ,0xba ,0x27 ,0xd0 ,0x77 ,0xbc ,0x6b ,0x50 ,0x6d ,0xa7 ,0x15 ,0x9f 
		 ,0x00 ,0xf2 ,0xbf ,0x88 ,0x1e ,0xb5 ,0x2f ,0x02 ,0x13 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x2e ,0xd9 ,0x2c ,0xfe ,0xbb ,0xa0 
			,0x79 ,0xa5 ,0x17 ,0xf5 ,0x7a ,0x21 ,0xde ,0xcb ,0xa9 ,0x44 ,0xd1 ,0xab ,0xb6 ,0x9d ,0x63 ,0x4d 
			,0x1d ,0xc9 ,0xd7 ,0xbc ,0x4b ,0xbc ,0xfc ,0xcb ,0xd5 ,0xa4 ,0xc4 ,0x78 ,0x0a ,0xa2 ,0x33 ,0x51 
			,0x93 ,0xff ,0x83 ,0xb1 ,0x50 ,0x82 ,0x46 ,0x10 ,0x75 ,0xd1 ,0x3d ,0x0b ,0x47 ,0x1f ,0x25 ,0x8b 
			,0x33 ,0x7b ,0xf4 ,0xf8 ,0xb2 ,0x89 ,0x0b ,0x37 ,0x22 ,0xd3 ,0x6e ,0xd7 ,0xf6 ,0x4e ,0xdc ,0x8c 
			,0x49 ,0x4e ,0xfe ,0x27 ,0xdc ,0xc8 ,0x13 ,0x72 ,0xea ,0x28 ,0x15 ,0x22 ,0xcd ,0x50 ,0x36 ,0x29 
			,0xf2 ,0xd9 ,0x13 ,0xd9 ,0xd9 ,0xbf ,0xce ,0xbc ,0x35 ,0x33 ,0x69 ,0xce ,0x18 ,0x60 ,0xa7 ,0xd9 
			,0x07 ,0xf8 ,0x58 ,0x35 ,0xc7 ,0x45 ,0x7f ,0xa8 ,0xd2 ,0xdc ,0xcd ,0x07 ,0x77 ,0x02 ,0x19 ,0xf9 
			,0x2c ,0xe5 ,0xa3 ,0xec ,0xc0 ,0x7a ,0xf3 ,0x69 ,0x7a ,0x49 ,0xf8 ,0xbf ,0x53 ,0x71 ,0xe7 ,0x80 
			,0x44 ,0xc9 ,0x84 ,0xe1 ,0x6c ,0x5c ,0xa1 ,0x02 ,0xd6 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x18 ,0x76 ,0x24 ,0xd5 ,0x2c ,0xaa 
			,0x72 ,0x4f ,0x02 ,0x16 ,0xfe ,0x9d ,0x07 ,0x8a ,0xf1 ,0xb1 ,0x84 ,0xdf ,0x23 ,0x17 ,0x35 ,0x8a 
			,0xb3 ,0x26 ,0x19 ,0x54 ,0x44 ,0x71 ,0xbf ,0x40 ,0xc1 ,0x48 ,0xc3 ,0xb6 ,0x8b ,0x94 ,0x1a ,0xbc 
			,0x2e ,0xf1 ,0xac ,0xfa ,0xbf ,0x8d ,0xcb ,0x8b ,0xc4 ,0x7b ,0xcf ,0xcc ,0x91 ,0x5b ,0x88 ,0xa0 
			,0x4e ,0x9a ,0xf7 ,0x5f ,0x2c ,0xe2 ,0xb9 ,0xd5 ,0x51 ,0x0b ,0x9a ,0x2c ,0xdd ,0x75 ,0xad ,0x96 
			,0xdc ,0x1e ,0x0c ,0xb5 ,0xee ,0xad ,0x48 ,0xdd ,0xf2 ,0xe9 ,0x48 ,0x9b ,0x22 ,0xc0 ,0x32 ,0xec 
			,0xf5 ,0xf7 ,0x28 ,0xf2 ,0x91 ,0xb0 ,0xa8 ,0x5a ,0x43 ,0x6f ,0x40 ,0xbc ,0x20 ,0xed ,0x05 ,0x03 
			,0x2e ,0xd9 ,0xe8 ,0xee ,0x65 ,0xbb ,0x87 ,0x0a ,0x03 ,0x89 ,0xef ,0x47 ,0x7c ,0x32 ,0x59 ,0xea 
			,0x82 ,0x7a ,0x17 ,0x91 ,0xb3 ,0xf6 ,0x13 ,0x81 ,0x97 ,0x0c ,0x69 ,0x6f ,0x78 ,0x9a ,0xa0 ,0xaf 
			,0xff ,0xe1 ,0xde ,0x02 ,0x08 ,0x61 ,0x12 ,0x02 ,0xd0 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xf4 ,0x2c ,0x21 ,0x34 ,0x94 ,0x43 
			,0xfa ,0xd8 ,0xb7 ,0xf3 ,0xdd ,0xe4 ,0x84 ,0x11 ,0x84 ,0xe9 ,0x34 ,0x40 ,0x84 ,0x3b ,0x54 ,0xeb 
			,0x52 ,0xce ,0x6d ,0xe6 ,0xf4 ,0xf8 ,0x65 ,0x33 ,0x95 ,0x83 ,0xb0 ,0xfb ,0xf2 ,0xdf ,0x2c ,0xcc 
			,0x75 ,0x4b ,0x76 ,0xcd ,0xe6 ,0xf1 ,0x48 ,0x37 ,0x97 ,0x99 ,0xae ,0x9f ,0xbd ,0xd8 ,0xdf ,0x52 
			,0x2b ,0x84 ,0x39 ,0x7c ,0x66 ,0x82 ,0x61 ,0x26 ,0x05 ,0x52 ,0xd4 ,0x17 ,0xa7 ,0xe9 ,0x4a ,0x7c 
			,0x67 ,0x2c ,0xa7 ,0x2f ,0x69 ,0x43 ,0x67 ,0x55 ,0x56 ,0x9b ,0xd6 ,0x92 ,0x6a ,0xec ,0x16 ,0x43 
			,0xa9 ,0xf8 ,0xfb ,0x98 ,0xb5 ,0x11 ,0xc1 ,0x9f ,0xaf ,0xfb ,0x26 ,0x08 ,0xd9 ,0x94 ,0x2b ,0x22 
			,0x54 ,0xc3 ,0x6c ,0xe9 ,0x1b ,0x99 ,0xfa ,0x6e ,0x5a ,0xe7 ,0xc3 ,0x9d ,0xb5 ,0x73 ,0xe5 ,0x36 
			,0x89 ,0xe5 ,0x78 ,0x29 ,0xee ,0xdc ,0xab ,0xdf ,0x0a ,0x08 ,0x67 ,0x68 ,0xcb ,0xf9 ,0xcc ,0xbe 
			,0xdd ,0x8c ,0x94 ,0x5f ,0x0b ,0x02 ,0x62 ,0x02 ,0x10 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,155},{0x21 ,0x40 ,0x97 ,0xa0 ,0xca ,0x00 ,0x00 ,0x91 ,0x04 ,0x8f ,0x01 ,0x01 ,0x2e ,0x48 ,0x2f ,0xd9 
			,0xca ,0xc3 ,0x11 ,0x07 ,0x2b ,0x46 ,0x3c ,0x87 ,0x44 ,0xea ,0x50 ,0xc1 ,0x6c ,0xc2 ,0x1e ,0x5f 
			,0x34 ,0x7b ,0x27 ,0x89 ,0xce ,0x2d ,0x7f ,0xe4 ,0x59 ,0xb1 ,0xc8 ,0xef ,0x51 ,0x08 ,0xd6 ,0x17 
			,0x07 ,0x42 ,0xe7 ,0x6f ,0x90 ,0xbb ,0x20 ,0xb0 ,0x6d ,0xc0 ,0xde ,0x09 ,0x0c ,0xfb ,0x58 ,0x6c 
			,0x0c ,0x5c ,0xf7 ,0x3a ,0xe8 ,0xa0 ,0x4d ,0x28 ,0x4b ,0xe7 ,0xb2 ,0x4b ,0xff ,0x75 ,0x3b ,0xb0 
			,0x91 ,0x98 ,0xde ,0x46 ,0x23 ,0x7a ,0xbd ,0xe8 ,0x01 ,0x74 ,0xa6 ,0xd0 ,0x41 ,0x22 ,0x74 ,0x75 
			,0xd4 ,0x99 ,0x21 ,0xba ,0xba ,0x3e ,0x4a ,0x09 ,0x3a ,0x81 ,0x8d ,0x8c ,0xcc ,0x2e ,0xa0 ,0x24 
			,0x54 ,0x3d ,0xd2 ,0x15 ,0x58 ,0xb4 ,0x9f ,0x50 ,0xf4 ,0x59 ,0x1e ,0xa7 ,0x0d ,0xda ,0x36 ,0x26 
			,0x1a ,0x3a ,0x01 ,0x52 ,0xe3 ,0x5d ,0x02 ,0xc2 ,0x8e ,0xe2 ,0x09 ,0xb5 ,0xe8 ,0x24 ,0x0c ,0x69 
			,0x63 ,0x87 ,0x8b ,0x3f ,0xaa ,0x6d ,0x63 ,0xc5 ,0x26 ,0x02 ,0xe4 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc8 ,0x00 ,0x06 ,0xcf }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xf2 ,0x61 ,0x25 ,0xf9 ,0x48 ,0x9e 
			,0x57 ,0x60 ,0x27 ,0x0a ,0x49 ,0xcb ,0x00 ,0x7e ,0x5e ,0x6f ,0xdc ,0x42 ,0xb3 ,0x25 ,0x5a ,0x39 
			,0x72 ,0xf8 ,0xcd ,0x32 ,0xea ,0x69 ,0xe5 ,0xed ,0xde ,0x48 ,0x19 ,0x2e ,0xb9 ,0xd0 ,0x21 ,0xce 
			,0xb0 ,0x91 ,0xee ,0x57 ,0x1c ,0x53 ,0x18 ,0x0d ,0x17 ,0x72 ,0xbc ,0x20 ,0x73 ,0xac ,0x1c ,0xa3 
			,0x57 ,0xdb ,0xa9 ,0x9d ,0x71 ,0x7e ,0x41 ,0xd9 ,0xae ,0x7b ,0xfb ,0xff ,0x95 ,0xba ,0xe2 ,0x2f 
			,0x94 ,0xd4 ,0x8a ,0xaa ,0x96 ,0xbe ,0x59 ,0x0b ,0x87 ,0xd3 ,0x62 ,0x47 ,0xcd ,0x3e ,0x2c ,0x2d 
			,0xb8 ,0x3a ,0xed ,0xcb ,0x33 ,0xb2 ,0x4c ,0x91 ,0x94 ,0x71 ,0xf0 ,0x8b ,0xe2 ,0x38 ,0xfc ,0x0b 
			,0x38 ,0xdb ,0x9a ,0x6f ,0x17 ,0xa6 ,0x4b ,0x71 ,0x2b ,0x95 ,0xc8 ,0xb6 ,0x73 ,0x9a ,0x5d ,0xe3 
			,0x54 ,0x0a ,0x7e ,0x98 ,0xe4 ,0x71 ,0xfd ,0xbb ,0xaf ,0x95 ,0x9e ,0x71 ,0xff ,0xf6 ,0x79 ,0x1e 
			,0xdb ,0x08 ,0x3f ,0x04 ,0xef ,0x78 ,0x29 ,0x02 ,0x3c }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,155},{0x21 ,0x40 ,0x97 ,0xa0 ,0xca ,0x00 ,0x00 ,0x91 ,0x04 ,0x8f ,0x01 ,0x01 ,0x71 ,0xb6 ,0x2b ,0x85 
			,0x50 ,0xe4 ,0xa2 ,0x61 ,0x89 ,0x76 ,0x7a ,0x95 ,0x9b ,0x20 ,0xa0 ,0xed ,0xae ,0x06 ,0xf9 ,0xb2 
			,0x60 ,0xfa ,0x1d ,0x9b ,0x5b ,0x2d ,0x3c ,0x8a ,0x7c ,0x38 ,0x34 ,0x8e ,0xcf ,0x89 ,0xce ,0x6b 
			,0xc0 ,0x25 ,0xa8 ,0x40 ,0x92 ,0x88 ,0xad ,0x92 ,0xde ,0xdd ,0x2f ,0x7c ,0xac ,0xda ,0xf3 ,0x26 
			,0xf6 ,0xc1 ,0x32 ,0xb7 ,0x88 ,0x4e ,0x53 ,0xfc ,0x8b ,0x2c ,0x34 ,0xfb ,0xfe ,0x27 ,0x24 ,0x0e 
			,0xb6 ,0x9a ,0x33 ,0xd7 ,0xc9 ,0x0e ,0x6c ,0xcc ,0x78 ,0x2b ,0x92 ,0x7a ,0x88 ,0x82 ,0x8f ,0x7d 
			,0x11 ,0x2e ,0xff ,0xa2 ,0x9c ,0x2d ,0x57 ,0xc1 ,0x46 ,0xb8 ,0xb0 ,0xe1 ,0x81 ,0x6a ,0x5d ,0x51 
			,0x88 ,0xe0 ,0xb6 ,0xcd ,0xdb ,0x2c ,0x8d ,0x2a ,0x22 ,0xfc ,0x21 ,0xff ,0x04 ,0x8a ,0x03 ,0xe7 
			,0x3b ,0xd1 ,0x2f ,0x9a ,0xa3 ,0xdc ,0x34 ,0xa8 ,0xc1 ,0x0b ,0x1c ,0x3c ,0xf8 ,0x75 ,0x47 ,0x69 
			,0xbe ,0x6b ,0xa7 ,0xd5 ,0xeb ,0x48 ,0x8f ,0xf5 ,0x11 ,0x02 ,0x97 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc8 ,0x00 ,0x06 ,0xcf }},
	{{0,155},{0x21 ,0x00 ,0x97 ,0xa0 ,0xca ,0x00 ,0x00 ,0x91 ,0x04 ,0x8f ,0x01 ,0x01 ,0x54 ,0x40 ,0x2e ,0x0b 
			,0x8f ,0x5f ,0x12 ,0xb2 ,0x55 ,0x82 ,0x6b ,0x89 ,0x3e ,0x34 ,0x1b ,0xda ,0xf9 ,0xa3 ,0x8f ,0xd6 
			,0x90 ,0xd5 ,0x69 ,0xf3 ,0x51 ,0x97 ,0x49 ,0x4c ,0x1b ,0x93 ,0x38 ,0x61 ,0x86 ,0x7d ,0x13 ,0xfc 
			,0x1d ,0x5b ,0x0a ,0x78 ,0xe5 ,0x46 ,0xb3 ,0xff ,0x81 ,0xad ,0x0d ,0xfa ,0xa9 ,0x90 ,0x78 ,0x3d 
			,0x0c ,0x0b ,0xf4 ,0xc3 ,0xc7 ,0x96 ,0xf2 ,0xe6 ,0xa0 ,0x45 ,0x7a ,0x96 ,0x83 ,0xdb ,0x4e ,0xc2 
			,0x75 ,0x19 ,0x66 ,0xa5 ,0x64 ,0xda ,0x4f ,0x20 ,0xd2 ,0x4a ,0x62 ,0xf5 ,0xc7 ,0xb6 ,0xa2 ,0x17 
			,0x7b ,0xbf ,0xc5 ,0xed ,0xe2 ,0x7f ,0xf6 ,0x7f ,0x09 ,0x39 ,0x7f ,0x15 ,0xe6 ,0x0d ,0x23 ,0xd5 
			,0x42 ,0x01 ,0x0b ,0xee ,0x46 ,0x4d ,0xd5 ,0x2e ,0x7e ,0xa7 ,0x1b ,0x1b ,0xcb ,0xe1 ,0x30 ,0x90 
			,0x26 ,0x76 ,0x61 ,0x42 ,0x5d ,0xb3 ,0x8a ,0x09 ,0xa7 ,0x19 ,0xad ,0x2f ,0xe4 ,0x75 ,0xc4 ,0x26 
			,0x64 ,0x4f ,0x97 ,0xca ,0x24 ,0xef ,0xf7 ,0x78 ,0xa5 ,0x02 ,0xbc }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc8 ,0x00 ,0x06 ,0x8f }},
	{{0,155},{0x21 ,0x40 ,0x97 ,0xa0 ,0xca ,0x00 ,0x00 ,0x91 ,0x04 ,0x8f ,0x01 ,0x01 ,0x75 ,0x28 ,0x2c ,0x93 
			,0x76 ,0x81 ,0x1f ,0x83 ,0x36 ,0x7b ,0x81 ,0xc4 ,0xd6 ,0x82 ,0x54 ,0x6e ,0x22 ,0xbf ,0xbf ,0x01 
			,0xf9 ,0x48 ,0x1b ,0x23 ,0xfd ,0x28 ,0x69 ,0xbb ,0xc2 ,0xc0 ,0xbf ,0xb6 ,0xd2 ,0xa5 ,0x4d ,0xf0 
			,0xd9 ,0x58 ,0x12 ,0x0d ,0x16 ,0x5a ,0x05 ,0x6d ,0x3d ,0xc4 ,0x35 ,0x65 ,0x39 ,0x94 ,0x4b ,0xc9 
			,0xee ,0x99 ,0xbb ,0xbe ,0x12 ,0x0d ,0xc3 ,0x48 ,0xe4 ,0xcc ,0x13 ,0xa5 ,0x51 ,0x65 ,0x0f ,0x63 
			,0x36 ,0x59 ,0x00 ,0x85 ,0x96 ,0x42 ,0x62 ,0x2b ,0x51 ,0x64 ,0xbd ,0x60 ,0xbe ,0x9b ,0x70 ,0xc9 
			,0xaf ,0x61 ,0x92 ,0xa4 ,0xe9 ,0x67 ,0x8f ,0x0b ,0xc1 ,0xef ,0x60 ,0x30 ,0xa3 ,0x30 ,0x42 ,0x00 
			,0x5a ,0x04 ,0x40 ,0xbb ,0x2b ,0xfd ,0x56 ,0xae ,0x7f ,0xae ,0x3f ,0xe9 ,0x7d ,0x10 ,0xe9 ,0x72 
			,0x0e ,0xcd ,0x90 ,0x98 ,0x3d ,0xd6 ,0x1a ,0x0a ,0x74 ,0x63 ,0xf4 ,0xfc ,0xbb ,0xd5 ,0xe2 ,0x36 
			,0xba ,0xaf ,0xd9 ,0x1c ,0x82 ,0xd7 ,0xa1 ,0x2a ,0xa5 ,0x02 ,0x01 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc8 ,0x00 ,0x06 ,0xcf }},
	{{0,155},{0x21 ,0x00 ,0x97 ,0xa0 ,0xca ,0x00 ,0x00 ,0x91 ,0x04 ,0x8f ,0x01 ,0x01 ,0xc9 ,0xde ,0x25 ,0x5f 
			,0x66 ,0x96 ,0xcd ,0x6d ,0xf4 ,0xf5 ,0xa2 ,0xb9 ,0x3e ,0x39 ,0xae ,0x76 ,0xd1 ,0x19 ,0x16 ,0x3b 
			,0x1d ,0x5b ,0x94 ,0x41 ,0x81 ,0x64 ,0xe9 ,0x63 ,0x87 ,0x19 ,0x89 ,0xe2 ,0xf2 ,0x9d ,0x83 ,0x22 
			,0xa8 ,0x1c ,0x9c ,0x53 ,0x99 ,0x46 ,0x82 ,0xe2 ,0xb1 ,0xc4 ,0xad ,0x8b ,0x79 ,0xd7 ,0x88 ,0x7d 
			,0x0a ,0x6f ,0x28 ,0xaf ,0x9b ,0xbe ,0x5e ,0x7b ,0x68 ,0x80 ,0xff ,0x55 ,0xc1 ,0x3c ,0x18 ,0xb1 
			,0x0e ,0xc1 ,0x39 ,0x4a ,0xcf ,0x61 ,0x0e ,0x2a ,0x0c ,0xc8 ,0xab ,0xdf ,0x71 ,0x9e ,0x44 ,0x29 
			,0x60 ,0x99 ,0x83 ,0x64 ,0x34 ,0xc6 ,0xba ,0x02 ,0x12 ,0x1c ,0x4a ,0x50 ,0x96 ,0x0a ,0xc8 ,0x80 
			,0xce ,0xe8 ,0x08 ,0xf3 ,0xb1 ,0x31 ,0xaf ,0x4f ,0xc2 ,0x5a ,0x02 ,0xa8 ,0x8b ,0x18 ,0xa5 ,0x81 
			,0x0c ,0x19 ,0x52 ,0x42 ,0xc6 ,0x67 ,0xf5 ,0xf5 ,0x51 ,0x7e ,0x19 ,0xe2 ,0x11 ,0x74 ,0x93 ,0x5c 
			,0xba ,0x7f ,0x09 ,0x64 ,0x5b ,0xa3 ,0xfe ,0x46 ,0x8c ,0x02 ,0x30 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc8 ,0x00 ,0x06 ,0x8f }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x7d ,0x04 ,0x2e ,0x4b ,0xf0 ,0x08 
			,0x33 ,0x9a ,0x0c ,0xc9 ,0x39 ,0x2e ,0x8a ,0x3e ,0x09 ,0x58 ,0x9d ,0x24 ,0x8f ,0xe1 ,0x24 ,0xae 
			,0xb2 ,0x46 ,0xe3 ,0xf4 ,0x37 ,0xb2 ,0xed ,0xcf ,0x2c ,0xba ,0xce ,0x27 ,0x26 ,0x86 ,0x25 ,0x5d 
			,0xff ,0xf4 ,0xfa ,0xf7 ,0x0d ,0xd7 ,0x14 ,0xc6 ,0xcd ,0x79 ,0x06 ,0x16 ,0xac ,0x8d ,0xdd ,0xe9 
			,0x4a ,0xb3 ,0xd9 ,0x9d ,0x72 ,0x64 ,0x7e ,0x6d ,0x65 ,0x97 ,0x8a ,0xc8 ,0x68 ,0xee ,0x04 ,0x4a 
			,0x84 ,0x17 ,0x11 ,0x37 ,0x76 ,0x2f ,0x40 ,0x78 ,0xd3 ,0x62 ,0x80 ,0x00 ,0xb6 ,0xdd ,0x08 ,0x49 
			,0x6e ,0xe6 ,0xcb ,0x99 ,0x0f ,0x5c ,0xcc ,0x93 ,0x65 ,0x2a ,0x48 ,0x4e ,0x8e ,0x58 ,0xb6 ,0x32 
			,0xc7 ,0x43 ,0x36 ,0xb1 ,0x3c ,0xc3 ,0x01 ,0xe0 ,0x33 ,0xed ,0x18 ,0xd3 ,0x4a ,0x2f ,0x81 ,0x94 
			,0x3a ,0x5a ,0x37 ,0x78 ,0x27 ,0x3e ,0x46 ,0xc3 ,0xf3 ,0xd3 ,0x31 ,0x8f ,0x1b ,0x0f ,0xf6 ,0x0e 
			,0xef ,0xe7 ,0x15 ,0x49 ,0x97 ,0xf0 ,0x71 ,0x02 ,0xdb }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,155},{0x21 ,0x00 ,0x97 ,0xa0 ,0xca ,0x00 ,0x00 ,0x91 ,0x04 ,0x8f ,0x01 ,0x01 ,0x7d ,0x8b ,0x2f ,0xe3 
			,0x8d ,0x27 ,0x17 ,0x26 ,0x35 ,0x5b ,0x85 ,0xc6 ,0xbd ,0xe8 ,0xd6 ,0x53 ,0x9d ,0x00 ,0x8e ,0x99 
			,0xea ,0xff ,0xc3 ,0x9f ,0x15 ,0x4a ,0xf1 ,0xa2 ,0x92 ,0x2d ,0xf8 ,0x70 ,0x82 ,0xa6 ,0x0c ,0x46 
			,0x07 ,0xe0 ,0xc6 ,0xd2 ,0x43 ,0xc7 ,0x58 ,0xa2 ,0x95 ,0x38 ,0x98 ,0xd4 ,0x37 ,0x74 ,0x16 ,0x2c 
			,0x1b ,0x20 ,0x6e ,0xe4 ,0x9a ,0xb7 ,0x1b ,0xdd ,0xf2 ,0x74 ,0xbb ,0xba ,0xb5 ,0x48 ,0xbf ,0x0a 
			,0xd8 ,0x67 ,0x53 ,0xb8 ,0xc6 ,0x19 ,0x1f ,0xbb ,0x2a ,0x11 ,0x14 ,0xb2 ,0x8f ,0x43 ,0x4d ,0x58 
			,0x59 ,0x24 ,0xea ,0xa3 ,0xb5 ,0xbd ,0x61 ,0x7d ,0xf0 ,0x53 ,0x11 ,0x73 ,0x1f ,0x30 ,0x4a ,0x4f 
			,0xf1 ,0x0c ,0xac ,0xd0 ,0xef ,0x47 ,0x0d ,0x0b ,0x8f ,0xf6 ,0xa5 ,0x51 ,0x14 ,0xa5 ,0xaf ,0xdb 
			,0x55 ,0x7e ,0x18 ,0xf2 ,0x7e ,0xd2 ,0x55 ,0xd9 ,0xc3 ,0x95 ,0x4e ,0x8a ,0x6f ,0x70 ,0xb4 ,0xa6 
			,0x1f ,0xbe ,0x4c ,0x2b ,0xc8 ,0xe5 ,0xba ,0x68 ,0x68 ,0x02 ,0x66 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc8 ,0x00 ,0x06 ,0x8f }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x55 ,0xb1 ,0x27 ,0x8e ,0xe9 ,0x29 
			,0x69 ,0x49 ,0x5a ,0x0d ,0x43 ,0xa3 ,0xe1 ,0xed ,0x27 ,0x1e ,0x1c ,0x4e ,0xc4 ,0x87 ,0x64 ,0xf4 
			,0x10 ,0xe1 ,0x95 ,0x29 ,0x6b ,0x75 ,0xdc ,0x1d ,0xdc ,0x9f ,0x81 ,0x95 ,0xc5 ,0x3a ,0xec ,0x2b 
			,0xe0 ,0xc3 ,0xec ,0xb2 ,0x93 ,0xfd ,0xb3 ,0x18 ,0x29 ,0xbb ,0x10 ,0x6a ,0x17 ,0x50 ,0x1b ,0x97 
			,0xfd ,0x11 ,0xd9 ,0xef ,0xdb ,0x6c ,0x84 ,0xbd ,0xe2 ,0x74 ,0x13 ,0x8d ,0x73 ,0xd6 ,0xe8 ,0x96 
			,0xbf ,0x15 ,0x78 ,0xa1 ,0x63 ,0xb6 ,0xbf ,0x7a ,0xaf ,0x98 ,0x18 ,0x99 ,0x94 ,0xe7 ,0x23 ,0x2f 
			,0xe7 ,0x05 ,0xdb ,0xb8 ,0x70 ,0xe9 ,0x20 ,0x71 ,0xc5 ,0x76 ,0x37 ,0x8e ,0x5b ,0xb2 ,0x86 ,0x0e 
			,0x50 ,0x85 ,0xf9 ,0xe0 ,0x76 ,0x14 ,0x5d ,0xfc ,0x10 ,0x60 ,0xbb ,0xdc ,0x7f ,0x00 ,0xe4 ,0xf1 
			,0xa2 ,0x88 ,0x62 ,0xc8 ,0xb3 ,0x6f ,0x5d ,0x26 ,0x47 ,0x14 ,0x79 ,0xde ,0xc2 ,0xbb ,0xcd ,0x1d 
			,0xc9 ,0x46 ,0x0b ,0x1d ,0xe2 ,0x2e ,0x10 ,0x02 ,0x13 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xd2 ,0xa2 ,0x24 ,0x28 ,0x38 ,0x0c 
			,0xd0 ,0xab ,0xb4 ,0xdc ,0x81 ,0xfc ,0x38 ,0x07 ,0xe7 ,0x4f ,0xeb ,0xcd ,0x2d ,0xa9 ,0x37 ,0x0e 
			,0x96 ,0xd1 ,0x58 ,0x8f ,0x11 ,0x4b ,0x64 ,0x77 ,0x65 ,0xd3 ,0xcf ,0x9f ,0x59 ,0xda ,0x21 ,0x24 
			,0xb0 ,0x3f ,0xf4 ,0x92 ,0x58 ,0x1b ,0xd1 ,0x90 ,0x25 ,0x4a ,0x5f ,0x3e ,0x09 ,0x4f ,0x0e ,0xd5 
			,0x0c ,0x65 ,0x17 ,0xc7 ,0xc8 ,0xc5 ,0x13 ,0x19 ,0xb4 ,0xee ,0x4c ,0x94 ,0x2d ,0xdd ,0x12 ,0x55 
			,0xba ,0xd8 ,0x2a ,0xbd ,0x6f ,0xfa ,0x0f ,0x8d ,0x8a ,0x3f ,0x7c ,0xe4 ,0x65 ,0x87 ,0xa8 ,0x9e 
			,0x59 ,0x8d ,0xf5 ,0x4d ,0xe1 ,0xd6 ,0x08 ,0xa2 ,0xca ,0x9c ,0xd1 ,0x3e ,0x7a ,0x2b ,0x48 ,0x60 
			,0x75 ,0x32 ,0x7d ,0x1d ,0x4e ,0x81 ,0xd2 ,0x82 ,0x0d ,0xbb ,0xf7 ,0x28 ,0xf1 ,0x86 ,0x7d ,0x37 
			,0x54 ,0xec ,0x9b ,0xea ,0xf0 ,0x9d ,0x5d ,0x34 ,0xbe ,0x43 ,0x45 ,0x31 ,0x37 ,0x17 ,0x4f ,0x2f 
			,0x56 ,0xbc ,0x1b ,0xf1 ,0xcd ,0x75 ,0x6c ,0x02 ,0xaf }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x2f ,0x98 ,0x22 ,0x68 ,0x49 ,0xee 
			,0x03 ,0x39 ,0x24 ,0xdb ,0x42 ,0xac ,0x98 ,0x86 ,0x8e ,0x9a ,0x5c ,0x70 ,0x5a ,0x7c ,0x86 ,0x7a 
			,0x73 ,0x38 ,0x4d ,0x52 ,0x33 ,0xb0 ,0xfa ,0x92 ,0x50 ,0x2f ,0x3d ,0xba ,0x74 ,0xfc ,0x62 ,0xfd 
			,0x76 ,0x71 ,0x4a ,0x45 ,0x6d ,0xb9 ,0xfd ,0x79 ,0x43 ,0x52 ,0x0c ,0xd1 ,0x69 ,0x5a ,0x07 ,0x88 
			,0xb1 ,0xf9 ,0x7f ,0x38 ,0x58 ,0x91 ,0x05 ,0x98 ,0x94 ,0xc3 ,0x6e ,0xd9 ,0xd0 ,0xa6 ,0x77 ,0xbc 
			,0x88 ,0x29 ,0x51 ,0x6f ,0x17 ,0x4a ,0x65 ,0x44 ,0xe8 ,0x17 ,0xb9 ,0x2c ,0x67 ,0x84 ,0xbf ,0x79 
			,0xe1 ,0xfa ,0x81 ,0x0a ,0xcf ,0xce ,0xf6 ,0x47 ,0x02 ,0x2a ,0x65 ,0x7b ,0x08 ,0x9b ,0x13 ,0xeb 
			,0x3e ,0x26 ,0x52 ,0x68 ,0x6a ,0x9f ,0xc2 ,0xf2 ,0xf4 ,0x5d ,0x0e ,0xf8 ,0xce ,0xff ,0x0a ,0x20 
			,0xed ,0x5c ,0xde ,0x8c ,0x11 ,0xc8 ,0x7b ,0xb3 ,0x24 ,0x72 ,0xa4 ,0x1f ,0x39 ,0xa9 ,0x71 ,0xaf 
			,0x06 ,0x51 ,0xb0 ,0xdf ,0x3b ,0x26 ,0x6e ,0x02 ,0x02 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x9a ,0xc2 ,0x25 ,0x44 ,0xcc ,0x18 
			,0x3e ,0xb7 ,0x35 ,0x4e ,0x84 ,0x8a ,0xe3 ,0x78 ,0xa6 ,0x17 ,0x0e ,0xde ,0x49 ,0x59 ,0x30 ,0x8e 
			,0xed ,0x95 ,0xe2 ,0xf9 ,0xa7 ,0x77 ,0xe0 ,0xd7 ,0xce ,0x01 ,0x31 ,0xe9 ,0x72 ,0x65 ,0x6a ,0x8f 
			,0x16 ,0xd9 ,0xdb ,0xa0 ,0xac ,0x1a ,0x3c ,0x7a ,0x01 ,0x46 ,0x09 ,0x3e ,0x44 ,0xce ,0x62 ,0x82 
			,0xff ,0x06 ,0xe4 ,0xef ,0xb4 ,0xac ,0x6a ,0xa5 ,0xce ,0x28 ,0xec ,0x53 ,0x94 ,0xd2 ,0xaa ,0x0e 
			,0x24 ,0x6c ,0x00 ,0xc6 ,0x61 ,0xb7 ,0x78 ,0xa3 ,0x85 ,0xa3 ,0xbd ,0xa9 ,0x06 ,0x3c ,0xce ,0xc7 
			,0x62 ,0x96 ,0x33 ,0xe4 ,0x9e ,0xff ,0xe9 ,0x46 ,0x0e ,0xd6 ,0x1e ,0x6c ,0x24 ,0x9e ,0x20 ,0xb3 
			,0x17 ,0xb1 ,0x69 ,0x0e ,0x20 ,0xe3 ,0x2e ,0xa6 ,0x24 ,0xff ,0x1f ,0xb8 ,0x29 ,0x81 ,0x06 ,0xa0 
			,0xb3 ,0x62 ,0xe3 ,0x4d ,0x28 ,0xd8 ,0x5a ,0x52 ,0xde ,0x22 ,0x56 ,0x52 ,0xb0 ,0xb9 ,0x79 ,0xbe 
			,0xb5 ,0x29 ,0x16 ,0x2b ,0x47 ,0x1e ,0x53 ,0x02 ,0x33 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xb1 ,0x6c ,0x27 ,0x83 ,0x7e ,0x09 
			,0xbb ,0x56 ,0x13 ,0xa3 ,0x48 ,0x4c ,0xc2 ,0x61 ,0xb9 ,0x0f ,0xc7 ,0xb0 ,0x4a ,0xcb ,0x45 ,0xd0 
			,0x7d ,0xc1 ,0xc7 ,0xf6 ,0xe8 ,0xc8 ,0xe0 ,0xc3 ,0xa5 ,0x2f ,0x68 ,0xed ,0x23 ,0x16 ,0x04 ,0x4c 
			,0xd0 ,0xcb ,0xc9 ,0x7f ,0x49 ,0x23 ,0x88 ,0x8e ,0x10 ,0x1e ,0xc1 ,0x47 ,0xb7 ,0x87 ,0xc9 ,0xcf 
			,0x7a ,0xe0 ,0x30 ,0xcc ,0xda ,0xaa ,0x76 ,0x84 ,0x41 ,0xec ,0x8b ,0xd1 ,0x4a ,0x4b ,0xe3 ,0xf3 
			,0x9a ,0x34 ,0xcb ,0xc7 ,0x68 ,0xa0 ,0x55 ,0x01 ,0xd3 ,0xe4 ,0x41 ,0xc0 ,0x74 ,0x0d ,0xce ,0x6d 
			,0x56 ,0xfb ,0x07 ,0x51 ,0xf4 ,0x5d ,0x48 ,0xbf ,0x30 ,0xad ,0x17 ,0x3b ,0x76 ,0xdc ,0x71 ,0xbd 
			,0x68 ,0x00 ,0x99 ,0xe2 ,0x8f ,0xc1 ,0x8a ,0xc1 ,0x24 ,0x08 ,0x85 ,0xa9 ,0x9d ,0x7c ,0x63 ,0xb0 
			,0x80 ,0x4f ,0x78 ,0x10 ,0xd6 ,0xbd ,0xb4 ,0xb3 ,0xe4 ,0xc3 ,0x31 ,0xe4 ,0x85 ,0x1c ,0xfd ,0xa6 
			,0xe4 ,0x92 ,0xf7 ,0x2a ,0xd5 ,0x85 ,0x85 ,0x02 ,0xb8 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xa4 ,0xa6 ,0x29 ,0x11 ,0x1b ,0xe0 
			,0x87 ,0x4f ,0xc0 ,0x9b ,0x95 ,0x28 ,0x6f ,0xe5 ,0xaa ,0xa8 ,0xe1 ,0xfb ,0x3c ,0x1c ,0xd0 ,0x96 
			,0x07 ,0x1b ,0x93 ,0x56 ,0xe9 ,0x93 ,0x19 ,0x49 ,0x24 ,0x3e ,0x0c ,0x07 ,0x8e ,0x53 ,0xa0 ,0x91 
			,0x69 ,0x99 ,0x82 ,0x00 ,0x58 ,0x54 ,0xe5 ,0x63 ,0x42 ,0xb6 ,0x22 ,0xb9 ,0xcc ,0x61 ,0xa6 ,0x4d 
			,0xea ,0x30 ,0x02 ,0x09 ,0xe3 ,0xa6 ,0x9e ,0xb7 ,0xe9 ,0xb8 ,0xa8 ,0x13 ,0x31 ,0x23 ,0xd9 ,0xf6 
			,0xa3 ,0xa3 ,0x47 ,0x99 ,0xbd ,0x34 ,0x1e ,0x2b ,0x5e ,0xa4 ,0x56 ,0x6a ,0x85 ,0xfd ,0xc6 ,0xde 
			,0x1d ,0x8c ,0x88 ,0x2a ,0xa1 ,0x29 ,0x55 ,0x61 ,0xc9 ,0x65 ,0xe7 ,0x46 ,0x86 ,0xd6 ,0x15 ,0x3b 
			,0x08 ,0xda ,0x1f ,0xde ,0x48 ,0x57 ,0xe3 ,0xfe ,0x0b ,0xcb ,0xf2 ,0xf1 ,0x01 ,0xd0 ,0xf1 ,0xca 
			,0x50 ,0xa1 ,0xdd ,0x5c ,0x2c ,0xb4 ,0x89 ,0xbf ,0x16 ,0xce ,0x3b ,0xf7 ,0xfb ,0xa6 ,0x7f ,0x01 
			,0x87 ,0x50 ,0xc2 ,0xc6 ,0x4e ,0x03 ,0x66 ,0x02 ,0x66 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xf6 ,0xc7 ,0x2d ,0xcd ,0x55 ,0x54 
			,0x75 ,0x26 ,0x3b ,0x79 ,0x86 ,0x9b ,0x19 ,0x15 ,0x0a ,0x5b ,0x79 ,0xd8 ,0xd9 ,0xc7 ,0x7f ,0xdb 
			,0x1e ,0x3f ,0xc1 ,0x31 ,0x4f ,0xec ,0x36 ,0x49 ,0x84 ,0x45 ,0x1f ,0x2b ,0xdc ,0x24 ,0xc6 ,0xd4 
			,0x4f ,0x21 ,0xda ,0x84 ,0xd0 ,0xf5 ,0x61 ,0x0c ,0x2f ,0x99 ,0x79 ,0x54 ,0x5f ,0x69 ,0xbe ,0xcd 
			,0x9b ,0x2e ,0x4e ,0x00 ,0x9c ,0xfa ,0x56 ,0x86 ,0x7c ,0xaa ,0xdf ,0x18 ,0x6a ,0x74 ,0xd7 ,0xfe 
			,0x53 ,0x3a ,0xfb ,0x1e ,0xea ,0x9d ,0xa7 ,0x31 ,0x69 ,0x3b ,0xef ,0x2b ,0xc1 ,0xe1 ,0xca ,0x16 
			,0x0d ,0x79 ,0xa9 ,0xf5 ,0x08 ,0x91 ,0xb8 ,0x6d ,0x89 ,0x27 ,0xcb ,0xdd ,0x9b ,0x84 ,0x5e ,0x10 
			,0xe4 ,0xbb ,0x41 ,0xc0 ,0xa2 ,0x93 ,0xdc ,0x34 ,0xd0 ,0xa1 ,0x47 ,0x3a ,0xdd ,0xcc ,0x03 ,0x6d 
			,0xa1 ,0x69 ,0x9b ,0x34 ,0xcc ,0xd6 ,0x33 ,0xd4 ,0x9c ,0xe9 ,0xe9 ,0xc4 ,0xbe ,0xfd ,0x42 ,0x59 
			,0xa4 ,0x06 ,0xa4 ,0x24 ,0x6a ,0xdd ,0x77 ,0x02 ,0x5a }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xb3 ,0x0a ,0x21 ,0x0c ,0x52 ,0x38 
			,0x72 ,0x30 ,0x98 ,0x78 ,0x11 ,0x82 ,0x4a ,0xee ,0xbe ,0x6d ,0x95 ,0xc4 ,0x86 ,0x09 ,0xc6 ,0xb9 
			,0x89 ,0x7e ,0x13 ,0xbd ,0x1f ,0x63 ,0x7b ,0x2a ,0x70 ,0xbc ,0x85 ,0xf0 ,0x55 ,0xb8 ,0xf2 ,0x56 
			,0xa7 ,0xd2 ,0xe7 ,0xde ,0xe9 ,0x8d ,0x22 ,0xc8 ,0xce ,0x45 ,0xa8 ,0x38 ,0xc4 ,0x01 ,0xbb ,0xa2 
			,0x88 ,0x0e ,0x71 ,0xd7 ,0x76 ,0xe7 ,0xed ,0xb3 ,0x17 ,0x00 ,0x98 ,0x23 ,0xa6 ,0x24 ,0x70 ,0x80 
			,0x16 ,0xf1 ,0x31 ,0xb3 ,0x29 ,0xa7 ,0x93 ,0xe7 ,0x07 ,0xe6 ,0xd5 ,0x55 ,0x1c ,0xb0 ,0x70 ,0x45 
			,0x7c ,0x4e ,0xf1 ,0xe4 ,0x6d ,0x94 ,0xe7 ,0x2b ,0x08 ,0xb7 ,0xa5 ,0x66 ,0xc7 ,0x63 ,0x94 ,0x0b 
			,0xc2 ,0x34 ,0x22 ,0xb8 ,0x65 ,0xfc ,0x59 ,0x43 ,0x7f ,0xe0 ,0x6d ,0x62 ,0xf0 ,0x61 ,0xd1 ,0xd5 
			,0x91 ,0x6a ,0x59 ,0x31 ,0x56 ,0xe9 ,0x39 ,0x04 ,0x09 ,0x2a ,0x05 ,0x7d ,0xda ,0x9c ,0x2f ,0x29 
			,0xc2 ,0xda ,0x2a ,0x6d ,0x7f ,0xc6 ,0x58 ,0x02 ,0x1c }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x6d ,0x63 ,0x2e ,0xb4 ,0x47 ,0x64 
			,0x32 ,0xdc ,0x37 ,0x91 ,0x5a ,0xa1 ,0x83 ,0x0c ,0xef ,0x1e ,0x20 ,0xbb ,0x1a ,0x24 ,0xd1 ,0x2f 
			,0x11 ,0x81 ,0xb4 ,0xe0 ,0xaf ,0xc5 ,0xee ,0x7a ,0x1f ,0x83 ,0xf2 ,0x2e ,0x01 ,0xea ,0x84 ,0xb5 
			,0x24 ,0xea ,0xa9 ,0x1e ,0xf2 ,0x64 ,0xfa ,0x23 ,0xb0 ,0x48 ,0x96 ,0xa8 ,0x92 ,0x17 ,0x01 ,0xfe 
			,0x0f ,0x20 ,0x8c ,0x5b ,0xba ,0x1c ,0x71 ,0x82 ,0x0f ,0x81 ,0x9d ,0xf2 ,0x92 ,0x15 ,0x7e ,0x48 
			,0x3a ,0xaa ,0x4b ,0x3e ,0xc1 ,0x4b ,0xce ,0x05 ,0x04 ,0x6f ,0xcf ,0x26 ,0x1b ,0x55 ,0x07 ,0xff 
			,0x11 ,0xde ,0x32 ,0x48 ,0xe7 ,0x57 ,0x55 ,0x5d ,0xbd ,0x73 ,0xff ,0x90 ,0xcb ,0x5d ,0xa0 ,0x5a 
			,0x51 ,0x9d ,0xe6 ,0x52 ,0x3a ,0xa5 ,0x0f ,0xad ,0xd5 ,0xbe ,0x89 ,0x49 ,0xdd ,0xb7 ,0x51 ,0x35 
			,0x3b ,0x5a ,0x78 ,0xdc ,0x59 ,0x96 ,0x53 ,0xf0 ,0x05 ,0xb7 ,0x1f ,0x14 ,0x9e ,0x1f ,0xe5 ,0xd5 
			,0x99 ,0x51 ,0x31 ,0x8d ,0x46 ,0xb4 ,0x3c ,0x02 ,0xc4 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x1d ,0xfb ,0x21 ,0x0d ,0x95 ,0x10 
			,0xe1 ,0x2e ,0xae ,0xb7 ,0x80 ,0xca ,0xe8 ,0x1f ,0x4a ,0xe4 ,0x52 ,0x06 ,0xbe ,0xeb ,0x40 ,0x59 
			,0x70 ,0xae ,0x45 ,0x40 ,0xf0 ,0x95 ,0x87 ,0x7f ,0x98 ,0xd7 ,0x9a ,0x42 ,0x1f ,0x8d ,0x2f ,0x78 
			,0xdc ,0x29 ,0x05 ,0xbd ,0x1d ,0x90 ,0x37 ,0xcb ,0x1a ,0x10 ,0x7a ,0xc8 ,0x12 ,0x93 ,0x68 ,0xa1 
			,0xbe ,0xef ,0xcd ,0x6f ,0xb9 ,0xba ,0xa5 ,0xd7 ,0xcb ,0xe7 ,0x1b ,0xd9 ,0xfc ,0xc0 ,0x83 ,0x1e 
			,0x5a ,0x91 ,0x32 ,0x3a ,0x39 ,0xd6 ,0x29 ,0xce ,0x4c ,0xfe ,0x9c ,0x75 ,0x13 ,0xbe ,0x54 ,0xfa 
			,0x3d ,0xc1 ,0xdc ,0x8b ,0x5f ,0x68 ,0xc9 ,0xf7 ,0xab ,0x44 ,0x0d ,0xf0 ,0x12 ,0xfe ,0xca ,0x52 
			,0xb4 ,0xc9 ,0xf6 ,0xcf ,0xda ,0xd9 ,0x6a ,0xb9 ,0xe8 ,0x15 ,0xdb ,0x31 ,0xff ,0x4c ,0x25 ,0xd3 
			,0xcb ,0xd5 ,0x32 ,0x00 ,0x6b ,0x04 ,0x56 ,0x68 ,0x17 ,0x30 ,0xfc ,0x7a ,0xc4 ,0xf3 ,0xf4 ,0x36 
			,0x0c ,0x52 ,0x13 ,0x85 ,0xae ,0x44 ,0x59 ,0x02 ,0x9d }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xd8 ,0x3c ,0x2c ,0x53 ,0x16 ,0x36 
			,0x6f ,0xba ,0xaa ,0xc6 ,0x77 ,0xa9 ,0x0d ,0xf5 ,0xf0 ,0xbb ,0x93 ,0x72 ,0xc0 ,0x02 ,0xb9 ,0x53 
			,0xdb ,0xd4 ,0x90 ,0x64 ,0xb6 ,0xfe ,0xcb ,0x01 ,0xb6 ,0xa2 ,0xaa ,0xcf ,0xd1 ,0xc2 ,0xb5 ,0x4e 
			,0xef ,0xcb ,0x4b ,0xe6 ,0x36 ,0xc8 ,0xa8 ,0x73 ,0x88 ,0xda ,0xbe ,0x75 ,0xa9 ,0x9b ,0x2b ,0xfd 
			,0xb9 ,0x7e ,0xfc ,0x88 ,0x57 ,0xfe ,0x2c ,0x41 ,0x71 ,0x24 ,0x15 ,0x20 ,0x94 ,0x74 ,0xbd ,0x76 
			,0x2e ,0xeb ,0x00 ,0xc7 ,0x61 ,0x6f ,0x32 ,0x50 ,0xd8 ,0x16 ,0x9a ,0x2f ,0xc6 ,0x09 ,0xba ,0x95 
			,0x4b ,0xd3 ,0x54 ,0xb6 ,0x8c ,0x88 ,0x6d ,0x2e ,0x02 ,0x86 ,0xfa ,0x55 ,0xe4 ,0xe9 ,0x09 ,0x18 
			,0x7d ,0x2d ,0x7e ,0xbb ,0xcb ,0x8b ,0x16 ,0xe9 ,0xdf ,0x96 ,0xde ,0x02 ,0x33 ,0x63 ,0x4a ,0x65 
			,0xd1 ,0x83 ,0x71 ,0x9f ,0x9e ,0xe6 ,0xd9 ,0x02 ,0x1f ,0xbf ,0xc4 ,0x19 ,0x6b ,0x2d ,0x42 ,0x97 
			,0x13 ,0x21 ,0xf6 ,0x79 ,0xf8 ,0x44 ,0x9d ,0x02 ,0x60 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x73 ,0x10 ,0x21 ,0xc9 ,0x95 ,0x71 
			,0x2c ,0x83 ,0x50 ,0x2c ,0x7b ,0x53 ,0x50 ,0xbc ,0x7a ,0x9f ,0x8b ,0x36 ,0x34 ,0x9c ,0x2d ,0xf2 
			,0x74 ,0xa8 ,0x38 ,0x98 ,0x7d ,0xde ,0x1f ,0x4c ,0xd1 ,0xc0 ,0x41 ,0x86 ,0xa5 ,0x34 ,0x4b ,0x57 
			,0x21 ,0x93 ,0x6a ,0x82 ,0x50 ,0xff ,0xf2 ,0x52 ,0xc4 ,0x83 ,0x28 ,0x9c ,0x22 ,0xa6 ,0x26 ,0x49 
			,0x72 ,0xc9 ,0x03 ,0xb8 ,0x89 ,0x36 ,0x90 ,0x0f ,0x88 ,0xe4 ,0xdd ,0x8e ,0x0f ,0x6e ,0xc3 ,0x0c 
			,0xf5 ,0xf9 ,0x4c ,0xc0 ,0xad ,0xbc ,0x74 ,0xb5 ,0x82 ,0x10 ,0x4b ,0xaa ,0x18 ,0x7f ,0x52 ,0x19 
			,0x9f ,0xb0 ,0x8b ,0xd9 ,0xb3 ,0xfc ,0x81 ,0xc1 ,0x05 ,0x96 ,0xfd ,0xa3 ,0x11 ,0xb3 ,0x26 ,0x9a 
			,0x0f ,0xf7 ,0xdf ,0x03 ,0xb0 ,0x46 ,0x74 ,0xdf ,0x2f ,0x73 ,0x01 ,0x61 ,0x86 ,0x18 ,0xc7 ,0xb4 
			,0xbd ,0x44 ,0x0b ,0xe9 ,0x1e ,0xbd ,0x25 ,0xd3 ,0x1f ,0x93 ,0xcb ,0xd2 ,0x9e ,0x1f ,0x1d ,0x41 
			,0xb2 ,0x10 ,0xbc ,0x8a ,0x9a ,0xde ,0x21 ,0x02 ,0x54 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x56 ,0xb0 ,0x28 ,0xe4 ,0x12 ,0x09 
			,0x0c ,0xc0 ,0x8e ,0x6f ,0xfa ,0x10 ,0x45 ,0x36 ,0xf8 ,0x93 ,0x28 ,0xc3 ,0xd6 ,0x7f ,0xb7 ,0x9f 
			,0x27 ,0x67 ,0x74 ,0x51 ,0xfb ,0x5a ,0x64 ,0x48 ,0x0e ,0x63 ,0x0b ,0x79 ,0xcd ,0x12 ,0xfc ,0xdd 
			,0xf1 ,0xee ,0xca ,0xf3 ,0xe6 ,0x86 ,0x20 ,0xc6 ,0xb7 ,0xa0 ,0x8e ,0x20 ,0x6e ,0xf7 ,0x67 ,0x62 
			,0x1d ,0x2c ,0xe8 ,0x68 ,0xdf ,0x4e ,0x42 ,0x3d ,0x27 ,0xd8 ,0xd8 ,0xf3 ,0xd0 ,0x4a ,0xd5 ,0xdb 
			,0x66 ,0x31 ,0x75 ,0x59 ,0xdd ,0xd0 ,0xb5 ,0x50 ,0x00 ,0x00 ,0x8d ,0x48 ,0x8e ,0x5e ,0x8a ,0x20 
			,0xe8 ,0xd9 ,0xca ,0x60 ,0xf5 ,0xe3 ,0x4e ,0xcb ,0x64 ,0xf1 ,0xf9 ,0xca ,0x70 ,0xc4 ,0x6a ,0x92 
			,0x90 ,0x1f ,0x7b ,0x76 ,0xb9 ,0xfc ,0x17 ,0x04 ,0x6e ,0x3c ,0x39 ,0xf0 ,0x59 ,0x0d ,0x40 ,0x29 
			,0x28 ,0x7c ,0xc0 ,0xc5 ,0x97 ,0x5e ,0x28 ,0x6d ,0x2b ,0x56 ,0xa1 ,0xfc ,0x68 ,0xef ,0x76 ,0x02 
			,0xbf ,0x72 ,0xcd ,0x2d ,0xdf ,0xcc ,0x17 ,0x02 ,0x15 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x8a ,0xc7 ,0x25 ,0x45 ,0x54 ,0xee 
			,0x8a ,0x02 ,0x70 ,0x08 ,0xc7 ,0xc8 ,0x76 ,0x2f ,0x94 ,0x63 ,0x4e ,0x6e ,0xe2 ,0x33 ,0x4e ,0xef 
			,0x10 ,0x3c ,0x1e ,0x3d ,0x71 ,0x62 ,0x6d ,0x5f ,0x66 ,0x0b ,0x8d ,0x6f ,0x11 ,0xf3 ,0xf2 ,0x05 
			,0x07 ,0xf5 ,0x54 ,0x85 ,0x27 ,0x63 ,0x7d ,0xe0 ,0xff ,0x72 ,0x28 ,0x02 ,0xce ,0xec ,0x68 ,0x77 
			,0x6c ,0x56 ,0xcb ,0x80 ,0xc6 ,0x47 ,0xa5 ,0xcd ,0x48 ,0x75 ,0x2c ,0x9f ,0x59 ,0x86 ,0xf7 ,0xa8 
			,0xc2 ,0x07 ,0x28 ,0x59 ,0x41 ,0x9e ,0x10 ,0x83 ,0xcf ,0xb1 ,0xd9 ,0x0c ,0x6f ,0xcf ,0x25 ,0x24 
			,0x25 ,0x28 ,0x4a ,0x4e ,0xe7 ,0x5d ,0x02 ,0xc5 ,0x67 ,0x36 ,0xd8 ,0x37 ,0xec ,0x94 ,0x74 ,0xb5 
			,0x95 ,0xb3 ,0xe1 ,0xc4 ,0x42 ,0x11 ,0x9a ,0x36 ,0x59 ,0x98 ,0xce ,0xed ,0xb2 ,0x63 ,0x16 ,0x20 
			,0x02 ,0xb2 ,0xd5 ,0x64 ,0xe2 ,0xe8 ,0x43 ,0xae ,0xec ,0x93 ,0x37 ,0x36 ,0xee ,0xac ,0x76 ,0xc7 
			,0xde ,0xa7 ,0x99 ,0x0e ,0x10 ,0x5b ,0x4c ,0x02 ,0xf7 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0xca ,0xd2 ,0x28 ,0x31 ,0xa8 ,0x89 
			,0x6b ,0x8d ,0x7e ,0xea ,0x50 ,0x3a ,0x58 ,0x79 ,0xd6 ,0x7d ,0x51 ,0x2c ,0x2a ,0xac ,0x87 ,0x94 
			,0x72 ,0x27 ,0xf5 ,0x33 ,0x89 ,0x37 ,0x12 ,0xf9 ,0x8d ,0x95 ,0x43 ,0x94 ,0x53 ,0x40 ,0x24 ,0xf9 
			,0x64 ,0xd2 ,0x13 ,0xc3 ,0x84 ,0x56 ,0xc1 ,0xf3 ,0xec ,0xbd ,0x2a ,0xf1 ,0xd6 ,0x84 ,0x3b ,0x35 
			,0x3c ,0x5e ,0xc9 ,0xc7 ,0x53 ,0xcb ,0xda ,0xa1 ,0x38 ,0x36 ,0x46 ,0x82 ,0x46 ,0xf0 ,0x77 ,0xc1 
			,0x8d ,0xe6 ,0x1e ,0xef ,0x6b ,0x86 ,0x17 ,0xd4 ,0xce ,0x49 ,0xd2 ,0xa3 ,0xd8 ,0xb7 ,0x70 ,0xf8 
			,0xec ,0xf0 ,0x69 ,0x17 ,0x1b ,0x72 ,0xdf ,0xdf ,0x09 ,0xf8 ,0x1c ,0xfa ,0x67 ,0x36 ,0x36 ,0x1f 
			,0x05 ,0x05 ,0xfa ,0x35 ,0xa1 ,0x12 ,0x0a ,0x59 ,0xeb ,0xbd ,0x5d ,0x7b ,0x7c ,0x90 ,0x4d ,0x7f 
			,0xcb ,0xd3 ,0x8e ,0x2b ,0xad ,0x2c ,0xf4 ,0x9a ,0x65 ,0x18 ,0x12 ,0x8e ,0x2f ,0x93 ,0xf4 ,0x07 
			,0xa7 ,0x46 ,0x4b ,0x57 ,0xce ,0x43 ,0x85 ,0x02 ,0x58 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x7c ,0xa6 ,0x23 ,0xac ,0x3c ,0x92 
			,0x15 ,0x2e ,0x6d ,0x4b ,0xff ,0xa5 ,0x86 ,0xe7 ,0x5f ,0xdd ,0xa7 ,0xf7 ,0x76 ,0x5c ,0x05 ,0x3a 
			,0x39 ,0xc8 ,0x26 ,0x10 ,0xee ,0x05 ,0x26 ,0x35 ,0xd5 ,0xd2 ,0xa8 ,0xfc ,0xed ,0x45 ,0x3f ,0x69 
			,0x71 ,0x73 ,0x27 ,0x49 ,0x92 ,0x53 ,0x16 ,0x8a ,0x67 ,0x81 ,0x55 ,0x25 ,0xa7 ,0xed ,0x48 ,0x68 
			,0xe3 ,0xcd ,0xc0 ,0x21 ,0xaf ,0x6d ,0x1c ,0x75 ,0x81 ,0xb9 ,0x1f ,0x2a ,0x20 ,0xbd ,0x78 ,0x11 
			,0x9d ,0x3d ,0x5f ,0x22 ,0x09 ,0xb0 ,0xdf ,0xa3 ,0xe4 ,0x78 ,0x38 ,0xbc ,0x9b ,0x6b ,0x9c ,0x40 
			,0xd1 ,0xeb ,0x37 ,0x1b ,0x78 ,0x0b ,0x8e ,0x1a ,0xf6 ,0xc3 ,0x12 ,0x15 ,0xb4 ,0xd1 ,0x38 ,0x3a 
			,0xc7 ,0xe9 ,0xe2 ,0xb4 ,0x3d ,0xab ,0x98 ,0x70 ,0x95 ,0x6d ,0x40 ,0x80 ,0x1b ,0xd1 ,0x6e ,0x36 
			,0x48 ,0x6b ,0x65 ,0x5f ,0x77 ,0xa8 ,0xf3 ,0xcf ,0xde ,0x88 ,0xe3 ,0xc1 ,0x28 ,0x96 ,0xaa ,0x5a 
			,0x41 ,0x89 ,0x90 ,0xaa ,0x64 ,0x63 ,0x71 ,0x02 ,0x61 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }},
	{{0,153},{0x21 ,0x40 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x0a ,0xc1 ,0x21 ,0x2e ,0xd6 ,0xaf 
			,0xab ,0xb6 ,0xc3 ,0x0e ,0xf7 ,0x3c ,0x3b ,0xbd ,0xba ,0x43 ,0xaf ,0xa4 ,0x58 ,0x30 ,0x30 ,0x33 
			,0x5b ,0x57 ,0x7e ,0xd5 ,0xa9 ,0xf2 ,0xfb ,0x40 ,0xac ,0x9a ,0xb7 ,0x01 ,0xdd ,0x1d ,0x3c ,0xc0 
			,0x31 ,0xed ,0x25 ,0xa0 ,0x48 ,0x1b ,0x8b ,0x11 ,0xb4 ,0xca ,0xff ,0xfa ,0xc3 ,0xc5 ,0xb5 ,0x4c 
			,0x67 ,0x43 ,0xa4 ,0x88 ,0xa0 ,0x78 ,0x1e ,0xad ,0xf5 ,0x1a ,0xfd ,0xb7 ,0xfe ,0x12 ,0xa1 ,0x80 
			,0xef ,0x86 ,0xa3 ,0x61 ,0xb3 ,0x1d ,0x7c ,0x96 ,0x00 ,0x46 ,0xb5 ,0xc4 ,0xe3 ,0x30 ,0x04 ,0x41 
			,0xf1 ,0xbc ,0x72 ,0x10 ,0x8e ,0x91 ,0xff ,0xda ,0xac ,0xce ,0x4e ,0xee ,0x56 ,0x57 ,0x53 ,0x98 
			,0x67 ,0x7d ,0x79 ,0x6c ,0x79 ,0x91 ,0x77 ,0x41 ,0x1f ,0x73 ,0xa1 ,0xe9 ,0x71 ,0xa3 ,0x92 ,0x61 
			,0xc1 ,0xbf ,0x09 ,0x10 ,0x74 ,0xb3 ,0xdd ,0xa4 ,0xbd ,0x02 ,0x2d ,0x49 ,0x6d ,0x3d ,0x95 ,0x36 
			,0x18 ,0xda ,0x63 ,0x65 ,0x81 ,0x7a ,0x28 ,0x02 ,0xa5 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0x87 }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x2b }},
	{{0,153},{0x21 ,0x00 ,0x95 ,0xa0 ,0xca ,0x00 ,0x00 ,0x8f ,0x07 ,0x8d ,0x68 ,0x32 ,0x2a ,0x16 ,0x49 ,0x4f 
			,0x3b ,0x98 ,0x77 ,0x1a ,0x79 ,0xb8 ,0xa6 ,0xce ,0x60 ,0xd7 ,0x42 ,0xc1 ,0x58 ,0xf9 ,0xb0 ,0x07 
			,0x8c ,0x55 ,0xb4 ,0xd6 ,0xf6 ,0x3f ,0x5d ,0xd0 ,0xea ,0xb8 ,0x84 ,0x06 ,0x4d ,0xe2 ,0x6d ,0xc0 
			,0x5d ,0xd8 ,0x39 ,0x6e ,0xd4 ,0x55 ,0x2d ,0xa3 ,0x88 ,0xeb ,0x9d ,0xc7 ,0xa4 ,0xb6 ,0xa8 ,0x22 
			,0x39 ,0x5e ,0x51 ,0x03 ,0x61 ,0xdd ,0x77 ,0xb3 ,0x4a ,0x61 ,0x17 ,0xbd ,0x17 ,0xcb ,0x0e ,0x9d 
			,0xe4 ,0x49 ,0x48 ,0x47 ,0x5f ,0x48 ,0x70 ,0x09 ,0x23 ,0x4d ,0x87 ,0x92 ,0x94 ,0xc0 ,0x13 ,0x2f 
			,0x6e ,0x8a ,0x7a ,0x7b ,0x40 ,0xe5 ,0xc4 ,0xcb ,0xef ,0xc6 ,0x05 ,0x7c ,0x29 ,0xb3 ,0x94 ,0x98 
			,0x98 ,0x15 ,0x07 ,0x19 ,0xe5 ,0x27 ,0x8b ,0x69 ,0x8c ,0x50 ,0xb8 ,0xdb ,0xdf ,0xbc ,0xf6 ,0x9f 
			,0x0e ,0x67 ,0x10 ,0xe0 ,0x63 ,0x62 ,0x54 ,0x0c ,0xb6 ,0xf7 ,0x55 ,0xe8 ,0x09 ,0x78 ,0x10 ,0xc4 
			,0xd1 ,0xb8 ,0x74 ,0xe4 ,0x7f ,0x42 ,0x4e ,0x02 ,0x6d }},
	{{0,12},{0x21 ,0x40 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0xc0 ,0x00 ,0x06 ,0xc7 }},
	{{0,12},{0x21 ,0x00 ,0x08 ,0xa0 ,0xca ,0x00 ,0x00 ,0x02 ,0x1c ,0x00 ,0x36 ,0x6b }}
};

static CaProtocalTestCommand_S gsT14Command[]=
{	
	{{0x00,0x07},{0x01 ,0x02 ,0x21 ,0x00 ,0x00 ,0x00 ,0x1d }},
	{{0x00,0x17},{0x01 ,0x02 ,0x20 ,0x00 ,0x00 ,0x10 ,0x03 ,0x06 ,0x17 ,0x7c ,0xbf ,0xe2 ,0x50 ,0x00 ,0xff ,0x00 
		         ,0x67 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xf7 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1e ,0x00 ,0x00 ,0x00 ,0x22 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1f ,0x00 ,0x00 ,0x00 ,0x23 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x00 ,0x00 ,0x00 ,0x00 ,0x3c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x01 ,0x00 ,0x00 ,0x00 ,0x3d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x02 ,0x00 ,0x00 ,0x00 ,0x3e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x08 ,0x00 ,0x00 ,0x00 ,0x34 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x00 ,0x00 ,0x3f }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x00 ,0x00 ,0x20 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x00 ,0x00 ,0x3b }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x01 ,0x00 ,0x3e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x01 ,0x00 ,0x21 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x01 ,0x00 ,0x3a }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x02 ,0x00 ,0x3d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x02 ,0x00 ,0x22 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x02 ,0x00 ,0x39 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x03 ,0x00 ,0x3c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x03 ,0x00 ,0x23 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x03 ,0x00 ,0x38 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1b ,0x04 ,0x00 ,0x00 ,0x23 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x15 ,0x00 ,0x00 ,0x00 ,0x29 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0e ,0x02 ,0x00 ,0x00 ,0x30 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0e ,0x03 ,0x00 ,0x00 ,0x31 }},
	{{0x00,0x47},{0x01 ,0x02 ,0x28 ,0x00 ,0x00 ,0x40 ,0x49 ,0x5e ,0xc4 ,0xe3 ,0xd1 ,0xba ,0xec ,0x32 ,0xa6 ,0x19 
				 ,0x97 ,0xad ,0xfa ,0x27 ,0xec ,0x9a ,0xce ,0x3b ,0xc8 ,0xa8 ,0xad ,0x3b ,0x05 ,0xbb ,0x70 ,0x21 
				 ,0xe5 ,0x52 ,0xf2 ,0x0b ,0xc9 ,0x31 ,0xc2 ,0x45 ,0xc8 ,0x5c ,0x14 ,0xba ,0x01 ,0xbf ,0x93 ,0x39 
				 ,0xb3 ,0xca ,0xcf ,0xbe ,0xee ,0x28 ,0xb3 ,0xf1 ,0x85 ,0x4d ,0x8b ,0x76 ,0x08 ,0x96 ,0x2b ,0x4a 
				 ,0x79 ,0x1b ,0x33 ,0x5a ,0x59 ,0x9c ,0x84 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0c ,0x00 ,0x00 ,0x00 ,0x30 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x26 ,0x00 ,0x00 ,0x01 ,0x00 ,0x1b }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0c ,0x00 ,0x01 ,0x00 ,0x31 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0c ,0x00 ,0x02 ,0x00 ,0x32 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0c ,0x00 ,0x03 ,0x00 ,0x33 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x2a ,0x01 ,0x00 ,0x00 ,0x17 }},
	{{0x00,0x07},{0x01 ,0x05 ,0x00 ,0x80 ,0x00 ,0x00 ,0xbb }},
	/*{{0x00,0x07},{0x01 ,0x01 ,0x00 ,0x80 ,0x00 ,0x00 ,0xbf }},*/
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x00 ,0x00 ,0x3f }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x00 ,0x00 ,0x20 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x00 ,0x00 ,0x3b }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x00 ,0x00 ,0x3f }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x00 ,0x00 ,0x20 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x00 ,0x00 ,0x3b }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x00 ,0x00 ,0x3f }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x00 ,0x00 ,0x20 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x00 ,0x00 ,0x3b }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0c ,0x00 ,0x00 ,0x00 ,0x30 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x01 ,0x00 ,0x3e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x01 ,0x00 ,0x21 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x01 ,0x00 ,0x3a }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x01 ,0x00 ,0x3e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x01 ,0x00 ,0x21 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x01 ,0x00 ,0x3a }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x01 ,0x00 ,0x3e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x01 ,0x00 ,0x21 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x01 ,0x00 ,0x3a }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0c ,0x00 ,0x01 ,0x00 ,0x31 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x02 ,0x00 ,0x3d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x02 ,0x00 ,0x22 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x02 ,0x00 ,0x39 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x02 ,0x00 ,0x3d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x02 ,0x00 ,0x22 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x02 ,0x00 ,0x39 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x02 ,0x00 ,0x3d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x02 ,0x00 ,0x22 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x02 ,0x00 ,0x39 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0c ,0x00 ,0x02 ,0x00 ,0x32 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x03 ,0x00 ,0x3c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x03 ,0x00 ,0x23 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x03 ,0x00 ,0x38 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x03 ,0x00 ,0x3c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x03 ,0x00 ,0x23 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x03 ,0x00 ,0x38 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x03 ,0x00 ,0x3c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x03 ,0x00 ,0x23 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x03 ,0x00 ,0x38 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x0c ,0x00 ,0x03 ,0x00 ,0x33 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x08 ,0x00 ,0x00 ,0x00 ,0x34 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x08 ,0x00 ,0x00 ,0x00 ,0x34 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x00 ,0x00 ,0x3f }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x00 ,0x00 ,0x20 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x00 ,0x00 ,0x3b }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x01 ,0x00 ,0x3e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x01 ,0x00 ,0x21 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x01 ,0x00 ,0x3a }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x02 ,0x00 ,0x3d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x02 ,0x00 ,0x22 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x02 ,0x00 ,0x39 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x03 ,0x00 ,0x3c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x03 ,0x00 ,0x23 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x03 ,0x00 ,0x38 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x23 ,0x00 ,0x00 ,0x00 ,0x1f }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x00 ,0x00 ,0x3f }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x00 ,0x00 ,0x20 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x00 ,0x00 ,0x3b }},
	{{0x00,0x07},{0x01 ,0x02 ,0x23 ,0x00 ,0x01 ,0x00 ,0x1e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x01 ,0x00 ,0x3e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x01 ,0x00 ,0x21 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x01 ,0x00 ,0x3a }},
	{{0x00,0x07},{0x01 ,0x02 ,0x23 ,0x00 ,0x02 ,0x00 ,0x1d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x02 ,0x00 ,0x3d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x02 ,0x00 ,0x22 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x02 ,0x00 ,0x39 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x23 ,0x00 ,0x03 ,0x00 ,0x1c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x03 ,0x00 ,0x3c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x03 ,0x00 ,0x23 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x03 ,0x00 ,0x38 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x04 ,0x04 ,0x00 ,0x00 ,0x3c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x04 ,0x04 ,0x01 ,0x00 ,0x3d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x04 ,0x04 ,0x02 ,0x00 ,0x3e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x04 ,0x04 ,0x03 ,0x00 ,0x3f }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x00 ,0x00 ,0x3f }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x00 ,0x00 ,0x20 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x00 ,0x00 ,0x3b }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x00 ,0x39 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x01 ,0x38 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x02 ,0x3b }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x03 ,0x3a }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x04 ,0x3d }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x05 ,0x3c }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x06 ,0x3f }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x07 ,0x3e }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x08 ,0x31 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x09 ,0x30 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x0a ,0x33 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x0b ,0x32 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x00 ,0x01 ,0x0c ,0x35 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x01 ,0x00 ,0x3e }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x01 ,0x00 ,0x21 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x01 ,0x00 ,0x3a }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x00 ,0x38 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x01 ,0x39 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x02 ,0x3a }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x03 ,0x3b }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x04 ,0x3c }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x05 ,0x3d }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x06 ,0x3e }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x07 ,0x3f }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x08 ,0x30 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x09 ,0x31 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x0a ,0x32 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x0b ,0x33 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x01 ,0x01 ,0x0c ,0x34 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x02 ,0x00 ,0x3d }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x02 ,0x00 ,0x22 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x02 ,0x00 ,0x39 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x00 ,0x3b }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x01 ,0x3a }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x02 ,0x39 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x03 ,0x38 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x04 ,0x3f }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x05 ,0x3e }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x06 ,0x3d }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x07 ,0x3c }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x08 ,0x33 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x09 ,0x32 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x0a ,0x31 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x0b ,0x30 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x02 ,0x01 ,0x0c ,0x37 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x00 ,0x03 ,0x00 ,0x3c }},
	{{0x00,0x07},{0x01 ,0x02 ,0x1c ,0x00 ,0x03 ,0x00 ,0x23 }},
	{{0x00,0x07},{0x01 ,0x02 ,0x03 ,0x04 ,0x03 ,0x00 ,0x38 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x00 ,0x3a }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x01 ,0x3b }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x02 ,0x38 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x03 ,0x39 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x04 ,0x3e }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x05 ,0x3f }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x06 ,0x3c }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x07 ,0x3d }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x08 ,0x32 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x09 ,0x33 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x0a ,0x30 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x0b ,0x31 }},
	{{0x00,0x08},{0x01 ,0x02 ,0x04 ,0x00 ,0x03 ,0x01 ,0x0c ,0x36 }}
};
static unsigned char acPPSDI[16][2] = {{0,0}, {0,7}, {0,10}, {0,11}, {0,12}, {0,13}, {0,14}, {0,15}, {1,1}, {2,2},{4,3}, {8,4}, {12,8},{16,5},{20,9},{32,6}};
static int acPPSFI[16][2] = {{0,7}, {0,8}, {0,14}, {0,15}, {372, 0}, {372, 1}, {512,9}, {558,2}, {744,3},{768, 10}, {1024, 11},{1116,4}, {1488,5}, {1536,12},{1860,6},{2048,13}};

static int SmcGetDi(int nDI)
{
	int ii = 0;

	for( ii = 0; ii < 16; ii++)
	{
		if( acPPSDI[ii][1] == nDI)
	    {
			return acPPSDI[ii][0];
        }
	}

	CSTCPrint("[%s.%d] did not find di:%d\n", __FUNCTION__, __LINE__, nDI);

	return -1;
}

static int SmcGetFi(int nFI)
{
	int ii = 0;
	for( ii = 0; ii < 16; ii++)
	{
		if( acPPSFI[ii][1] == nFI)
	    {
			return acPPSFI[ii][0];
        }
	}

	CSTCPrint("[%s.%d] did not find fi:%d\n", __FUNCTION__, __LINE__, nFI);

	return -1;
}

//nStatus:0 开始,1结束,2,返回结果,3清除结果
static int CSSMCQuickInOutCheck(int nTime,E_QTYPE nStatus)
{
	static int g_nTimeBefore = 0;
	static int g_nTimeCost   = 0;
	static int g_nMaxLimit   = 0;
	static int g_nCount      = 0;

	switch(nStatus)
	{
		case E_QSTART:
		{
			g_nTimeBefore = nTime;
			break;
		}
		case E_QEND:
		{
			g_nTimeCost = nTime - g_nTimeBefore;
			CSTCPrint("g_nTimeCost:%d\n", g_nTimeCost);
			break;
		}
		case E_QCHECK:
		{
			if(g_nTimeCost > g_nMaxLimit)
			{
				CSTCPrint("g_nTimeCost:%d, maxlimit:%d\n", g_nTimeCost, g_nMaxLimit);
				return CSUDI_FAILURE;
			}
			break;
		}
		case E_QCLEAR:
		{
			g_nTimeCost = 0;
			break;
		}
		case E_QSET:
		{
			g_nMaxLimit = nTime;
			g_nCount = 0;
			break;
		}
		case E_CNT:
		{
			g_nCount++;
			break;
		}
		case E_CNTCHECK:
		{
			if(g_nCount < 10)
			{
				return CSUDI_FAILURE;
			}
			break;
		}
		default:
		{
			break;        
		}
	}    

	return CSUDI_SUCCESS;    
}

static void CSSMCCallBackQuick(int nCardIndex, CSUDI_Error_Code eStatus)
{
	switch(eStatus)
	{
		case CSUDISMC_ERROR_IN:
		{
			CSTCPrint("[CS-SMC-TEST]: CS_SMC_IN\n");			
			break;
		}
		case CSUDISMC_ERROR_OUT:
		{
			CSTCPrint("[CS-SMC-TEST]: CS_SMC_OUT\n");
			CSSMCQuickInOutCheck(CSUDIPlusOSGetTickCount(), E_QEND);
			CSSMCQuickInOutCheck(0, E_CNT);
			break;
		}
		case CSUDISMC_ERROR_READY:
		{
			CSTCPrint("[CS-SMC-TEST]: CS_SMC_READY\n");			
			break;
		}
		default:
		{
			CSTCPrint("[CS-SMC-TEST]: CS_SMC_UNKNOW_ERROR:%d\n", eStatus);
			break;
		}
	}
}

//----------------------read file data------------------------------------------
#define MAX_SMC_DATA 	3072	//表示3072个字符，"0xff, "表示一个数据需要6个字符，6*512
#define MAX_CMDNAME_LEN 16
#define GETARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
typedef struct Cell_SmartCard
{
	unsigned char *m_data;
	int	 m_len;
}sCellSMC;

typedef struct Info_SmartCard
{
	sCellSMC m_send;
	sCellSMC m_receive;
	sCellSMC m_status;
}sInfoSMC;

typedef struct _InfoLinkSMC
{
	sInfoSMC 		*m_node;
	int      		m_num;	
	struct _InfoLinkSMC *next;
}sInfoLinkSMC;

static sInfoLinkSMC *psRootNode = NULL;
static sInfoLinkSMC *psLastNode = NULL;
static unsigned char*s_pcAtr = NULL;
static int s_Atrlen = 0; 
static unsigned char s_ReData[MAX_SMC_DATA+1];

PVOID CSCAMalloc( unsigned int nMemorySize )
{
	return CSUDIOSMalloc(nMemorySize);
}

BOOL CSCAFree( PVOID pvMemoryAddr )
{
	return CSUDIOSFree(pvMemoryAddr);
}

//add one communicate data in link.
int CSCAFsAddOneCmd(sInfoSMC *pSInfo)
{
	sInfoSMC *psNode = NULL;
	sInfoLinkSMC *psLink = NULL;

	if(!pSInfo)
	{
		CSTCPrint("[%s.%d] pSInfo is NULL\n", __FUNCTION__, __LINE__);
		return FAILURE;
	}

	psNode =(sInfoSMC*)CSCAMalloc(sizeof(sInfoSMC));
	if(!psNode)
	{
		CSTCPrint("[%s.%d] CSCAMalloc failed\n", __FUNCTION__, __LINE__);
		return FAILURE;
	}

	memcpy(&psNode->m_send,&pSInfo->m_send,sizeof(sCellSMC));
	memcpy(&psNode->m_receive,&pSInfo->m_receive,sizeof(sCellSMC));
	memcpy(&psNode->m_status,&pSInfo->m_status,sizeof(sCellSMC));

	psLink = (sInfoLinkSMC*)CSCAMalloc(sizeof(sInfoLinkSMC));
	if(!psLink)
	{
		CSTCPrint("[%s.%d] CSCAMalloc failed\n", __FUNCTION__, __LINE__);
		CSCAFree(psNode);
		return FAILURE;
	}
	
	if(!psRootNode)
	{
		psRootNode = psLink;
		psLink->m_node = psNode;
		psLink->m_num = 0;
		psLink->next = NULL;
		psLastNode = psRootNode;
	}
	else
	{
		psLastNode->next = psLink;		
		psLink->m_num = psLastNode->m_num+1;
		psLink->m_node = psNode;
		psLink->next = NULL;
		psLastNode = psLink;
	}
	return SUCCESS;
}

//根据索引取命令
int CSCAFsGetOneCmd(sInfoSMC **pSInfo,int nIndex)
{
	if(!psLastNode)
	{
		//没有命令
		CSTCPrint("[%s.%d] pSInfo is NULL\n", __FUNCTION__, __LINE__);
		return FAILURE;
	}
	
	if(nIndex <= psLastNode->m_num)
	{
		sInfoLinkSMC *psTmpNode = psRootNode;
		
		while(psTmpNode->m_num != nIndex)
		{
			psTmpNode = psTmpNode->next;
		}
		*pSInfo = psTmpNode->m_node;
		return SUCCESS;
	}
	else
	{
		CSTCPrint("[%s.%d] nIndex(%d) <= psLastNode->m_num(%d)\n",
			__FUNCTION__, __LINE__, nIndex, psLastNode->m_num);
		return FAILURE;
	}
}

int CSCAFsDestoryCmd()
{
	if(s_pcAtr)
	{
		CSCAFree(s_pcAtr);
		s_pcAtr = NULL;
	}

	if(psRootNode)
	{
		sInfoLinkSMC *psTmpNode = psRootNode;
		sInfoLinkSMC *psTmpNodeDes;
		while(psTmpNode)
		{
			psTmpNodeDes = psTmpNode;
			CSCAFree(psTmpNodeDes->m_node->m_send.m_data);
			CSCAFree(psTmpNodeDes->m_node->m_receive.m_data);
			CSCAFree(psTmpNodeDes->m_node->m_status.m_data);
			CSCAFree(psTmpNodeDes->m_node);
			CSCAFree(psTmpNodeDes);			
			psTmpNode = psTmpNode->next;
		}
		psRootNode = NULL;
		psLastNode = NULL;		
	}
	
	return CSUDI_SUCCESS;
}

//送入原始字符，转换为数组，并把数据长度反馈出来
CSTKCfgError_E CSCAFsAnalyData(unsigned char *pcRaw,unsigned char **pcData,int *pnLen)
{
	int nIndex0,nIndex1;
	unsigned char *cIn;

	if(!pcRaw || !pcData ||!pnLen)
	{
		CSTCPrint("[%s.%d] arguments illegal\n", __FUNCTION__, __LINE__);
		return CS_TK_CONFIG_FAILURE;
	}

	cIn = pcRaw;
	nIndex0 = 0;
	nIndex1 = 0;
	while(*cIn)
	{
		switch(*cIn)
		{
			case 'x':
			case 'X':
				{
					nIndex0++;
				}
				break;

			case ',':
				{
					nIndex1++;
				}
				break;
			case '\n':
			case '\r':
				{
					*(cIn+1)= 0;
				}
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
				break;
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case ' ':
			case '\t':
				break;
	
			default:
				{
					CSTCPrint("input error:%c\n",*cIn);
					goto DATABAD;
				}
				break;
		}
		cIn++;
	}
	
	if(nIndex0 == nIndex1 || nIndex0 == nIndex1+1)
	{
		unsigned char cText[3];
		int cIndex = 0;
		int bRegion = 0;
		unsigned char *cTemp;

		/*CSTCPrint("nIndex0=%d,nIndex1=%d\n",nIndex0,nIndex1);*/
		*pcData = (unsigned char*)CSCAMalloc(1+sizeof(char)*nIndex0);
		if(!*pcData)
		{
			goto MEMBAD;
		}
		cTemp = *pcData;
		*pnLen = nIndex0;	
		cIn = pcRaw;
		while(*cIn)
		{
			switch(*cIn)
			{
				case 'x':
				case 'X':
					{
						if(0 == bRegion)
						{
							bRegion = 1;
							cIndex = 0;							
						}
						else
						{
							CSTCPrint("bad 0\n");
							goto MEMBAD;
						}
					}
					break;

				case ',':
					{
						if(1 == bRegion)
						{
							bRegion = 0;
							cText[cIndex] = 0;						
							*cTemp = (unsigned char)CSTKGetIntFromStr((char*)cText,16);
							cTemp++;
						}
						else
						{
							CSTCPrint("bad 1\n");
							goto MEMBAD;
						}
					}
					break;
				case '\n':
				case '\r':
					{
						*(cIn+1)= 0;
					}
					break;
				/*case '0'...'9':
				case 'a'...'f':
				case 'A'...'F':*/
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					{
						if(1 == bRegion)
						{
							cText[cIndex++] = *cIn;
							if(cIndex > 2)
							{
								CSTCPrint("bad 2\n");
								goto MEMBAD;
							}
						}
					}
					break;
				case ' ':
				case '\t':
					break;
		
				default:
					{
						CSTCPrint("input error:%c\n",*cIn);
						goto MEMBAD;
					}
					break;
			}
			cIn++;	
		}

		if(1 == bRegion)//the last data without ','
		{
			bRegion = 0;
			cText[cIndex] = 0;							
			*cTemp = (unsigned char)CSTKGetIntFromStr((char*)cText,16);
			cTemp++;
		}
	}
	else
	{
		//数据格式不对，0x 和 逗号 相等，或者只差一个。否则认为是数据错误
		goto DATABAD;
	}	
	return CS_TK_CONFIG_SUCCESS;

DATABAD:
	//before malloc 
	CSTCPrint("%s:DATABAD\n",__FUNCTION__);
	return CS_TK_CONFIG_FAILURE;
MEMBAD:
	CSTCPrint("%s:MEMBAD\n",__FUNCTION__);
	CSCAFree(*pcData);
	*pcData = NULL;
	return CS_TK_CONFIG_FAILURE;
}

CSTKCfgError_E CSTCGetConfigAtr(void)
{
	char *pcSection = "SMARTCARD";
	int nAtrLen;
	char *pckeyAtr= "ATR";	
	
	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo(pcSection,pckeyAtr,(char*)s_ReData,MAX_SMC_DATA))
	{
		//CSTCPrint("get atr fail!\n");
		goto FAIL;
	}
	else
	{	
		/*CSTCPrint("read from file:%s\n",s_ReData);*/
		if(CS_TK_CONFIG_SUCCESS != CSCAFsAnalyData(s_ReData,&s_pcAtr,&nAtrLen))
		{
			//CSTCPrint("atr data error!\n");
			goto FAIL;
		}
		else
		{			
			/*int nTemp;
			CSTCPrint("analysized. Atr(len=%d):",nAtrLen);
			for(nTemp = 0;nTemp < nAtrLen; nTemp++)
			{
				CSTCPrint("0x%02x ",*(s_pcAtr+nTemp));
			}*/

			s_Atrlen = nAtrLen;
		}
	}
	return CS_TK_CONFIG_SUCCESS;
FAIL:
	CSTCPrint("[%s.%d] failed\n", __FUNCTION__, __LINE__);
	return CS_TK_CONFIG_FAILURE;
}
	
CSTKCfgError_E CSTCGetConfigCmd(void)
{
	char *pcSection = "SMARTCARD";

	char *pckey[3] = 
	{
		"SEND",
		"RECEIVE",
		"STATUS"
	};

	unsigned char *pcData;
	int nDataLen;
	int nNum = 0;
	int nIndex;
	char cNumStr[4];
	char cKeyStr[16];

	sCellSMC sTmpInfoSend={0,0};
	sCellSMC sTmpInfoReceive={0,0};
	sCellSMC sTmpInfoStatus={0,0};
	sInfoSMC sTmpInfo;
	
	do{
		nNum++;	
		sprintf(cNumStr,"%d",nNum);		 
		for(nIndex = 0; nIndex < GETARRAYSIZE(pckey);nIndex++)
		{		
			strcpy(cKeyStr,pckey[nIndex]);
			strcat(cKeyStr,cNumStr);
			/*CSTCPrint("cmd:%s\n",cKeyStr);*/
			if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo(pcSection,cKeyStr,(char*)s_ReData,MAX_SMC_DATA))
			{
				if(nIndex == 0 && nNum > 1)//至少有一组正确的命令，才报错
				{
					goto SUC_OUT;
				}
				else
				{
					//CSTCPrint("%s get fail!\n",cKeyStr);
					goto FAIL_OUT;
				}
			}
			if(CS_TK_CONFIG_SUCCESS != CSCAFsAnalyData(s_ReData,&pcData,&nDataLen))
			{
				//CSTCPrint("%s data error!\n",cKeyStr);
				goto FAIL_OUT;
			}
			else
			{
				//do nothing
			}
			switch(nIndex)
			{
				case 0:
					{
						sTmpInfoSend.m_data = pcData;
						sTmpInfoSend.m_len = nDataLen;
					}
					break;
				case 1:
					{
						sTmpInfoReceive.m_data = pcData;
						sTmpInfoReceive.m_len = nDataLen;
					}
					break;
				case 2:
					{
						sTmpInfoStatus.m_data = pcData;
						sTmpInfoStatus.m_len = nDataLen;
					}
					break;
				default:
					break;
			}			
		}
		sTmpInfo.m_send 	= sTmpInfoSend;
		sTmpInfo.m_receive 	= sTmpInfoReceive;
		sTmpInfo.m_status 	= sTmpInfoStatus;

		CSCAFsAddOneCmd(&sTmpInfo);
		
	}while(1);

SUC_OUT:
	return CS_TK_CONFIG_SUCCESS;
FAIL_OUT:
	CSTCPrint("[%s.%d] failed\n", __FUNCTION__, __LINE__);
	return CS_TK_CONFIG_FAILURE;
}

static int s_FI[16]			={372,372,558,744,1116,1488,1860,-1,-1,512,768,1024,1536,2048,-1,-1};
static float s_FreMax[16]	={4,  5,   6,  8,  12,  16, 20,  -1,-1,5,  7.5,10,  15,  20,  -1,-1};
static int s_DI[16]			={1,  1,   2,  4,  8,   16, 32,  -1,12,20, -1, -1,  -1,  -1,  -1,-1};
static int CSAtrDecode(unsigned char *pcAtr,int nAtrlen)
{
	unsigned char cVal;
	unsigned char cHis;
	unsigned char cInx=0;
	int nEtu = 372;
		
	if(!pcAtr || 0 == nAtrlen)
	{
		CSTCPrint("input param error!\n");
		return CSUDI_FAILURE;
	}

	cVal = *pcAtr++;

	if(0x3f == cVal)
	{
		CSTCPrint("convention: inversion\n");
	}
	else if(0x3b == cVal)
	{
		CSTCPrint("convention: Direct\n");
	}
	else
	{
		CSTCPrint("[%s.%d] pcAtr(0x%%x) not 0x3f or 0x3b\n", __FUNCTION__, __LINE__, cVal);
		return CSUDI_FAILURE;
	}

	cVal = *pcAtr++;	
	cHis = cVal & 0x0f;
	while (cVal!= 0)
	{
		cInx++;

		if (cVal & 0x10)
		{
			unsigned char cValue = *pcAtr++;
			
			if (cInx == 1)
			{
				int fi = (cValue & 0xF0)>>4;
				int di = cValue & 0x0F;

				if(fi < 16 && di < 16)
				{
					nEtu = (s_FI[fi]/s_DI[di]);
					CSTCPrint("FI=%d,DI=%d,MaxFrequency=%f(MHz)\n",s_FI[fi],s_DI[di],s_FreMax[fi]);					
					CSTCPrint("ETU=%d\n",s_FI[fi]/s_DI[di]);
				}
				else
				{
					CSTCPrint("atr not follow iso7816 !\n");
					return CSUDI_FAILURE;
				}
				
			}
			if (cInx == 2)
			{
				CSTCPrint("SpecificMode:Yes!\n");
				CSTCPrint("SpecificType:T%d\n",cValue&0x0f);
				CSTCPrint("SpecificUse:%s\n",(cValue&0x10)?"postpond":"now");
				CSTCPrint("SpecificTypeChangable:%s\n",(cValue&0x80)? "FALSE":"TRUE");
			}
			if (cInx > 2)
			{
				CSTCPrint("The Max Card BlockSize(T1 only):%d\n",cValue);
			}
		}

		if (cVal & 0x20)
		{
			unsigned char cValue = *pcAtr++;
			
			if (cInx == 1)
			{
				int nPI = (cValue & 0x60) >> 5;
				int nPV = cValue & 0x1F;

				CSTCPrint("Max Program Current:%d(mA)\n",(nPI < 2)? 25*(nPI+1) : 0);
				if(nPV == 0)
				{
					CSTCPrint("Max Program Voltage:default\n");
				}
				else if(nPV >= 5 && nPV <= 25)
				{
					CSTCPrint("Max Program Voltage:%d(V)\n",nPV);
				}
				else if(nPV >=50 && nPV <= 250)
				{
					CSTCPrint("Max Program Voltage:%d(V)\n",nPV/10);
				}
				else
				{
					CSTCPrint("Max Program Voltage:error\n");
				}
			}
			if (cInx == 2)
			{
				int nPI = (cValue & 0x60) >> 5;
				CSTCPrint("Max Program Current New:%d(mA)\n",(nPI > 2)? 25*(nPI+1) : 0);
			}
			if (cInx > 2)
			{
				int CWI = cValue&0x0F;
				int BWI = (cValue & 0xF0) >> 4;

				CSTCPrint("CWT = %d(etu)(cwi:%d)\n",(1<<CWI)+11,CWI);
				CSTCPrint("BWT = %d(etu)(bwi:%d)\n",(1<<BWI)*960*372/nEtu + 11,BWI);
			}
		}

		if (cVal & 0x40)
		{
			unsigned char cValue = *pcAtr++;
			if (cInx == 1)
			{
				if (cValue == 0xFF)
				{
					CSTCPrint("Extra Guardtime(default):2(etu)\n");
				}
				else
				{
					if(cValue < 2)
						cValue = 2;
					
					CSTCPrint("Extra Guardtime:%d\n",cValue);
				}
			}
			if (cInx == 2)
			{
				CSTCPrint("WI=%d\n",cValue);
				CSTCPrint("WWT=960*WI*FI/f (etu)\n");
			}
			if (cInx > 2)
			{
				CSTCPrint("Check:%s\n",(cValue&0x01)?"CRC":"LRC");
			}
		}

		if (cVal & 0x80)
		{
			cVal = *pcAtr++;
			
			if(0 == (cVal & 0xF0))
				break;
		}
		else
		{
			break;
		}
	}

	CSTCPrint("history character(len=%d):",cHis);
	for(cVal = 0; cVal < cHis; cVal++)
	{
		CSTCPrint("%c ",*pcAtr++);
	}

	return CSUDI_SUCCESS;
}


//------------------------common config ------------------
int CSSMCTestConfigCommon(int nInputCardSlotNum,int nInputCardNum)
{
	CSUDISMCParam_S sCardParamSet;

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	return CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet);		
}
//------------------------common config end---------------
/*********************************************************************************************
 *Function      : 针对suantong T1 协议，对数据进行补全
 *Function Name : CSDataFixForSuantong
 *Paramater     :
 *	pucData [IN]: 输入数据
 *	nDataLen[IN]: 输入数据
 *	nMaxLen [IN]: 输入数据的buffer总长度
 *Return       :  SUCCESS:成功   FAILURE:失败
 *Remark        :
 *	对于NXP平台，输入指令需要补全，卡输出数据不需要补全
 *  补全规则:  [0][0][len][...data...][lrc]
 ********************************************************************************************/
INT CSDataFixForSuantong(unsigned char * pucData, int nDataLen, int nMaxLen)
{
	unsigned char chLRC;
	int nTemp;
	if(!pucData || nDataLen == 0 || nMaxLen == 0)
	{
		CSTCPrint("[%s.%d] arguments illegal\n", __FUNCTION__, __LINE__);
		return CSUDI_FAILURE;
	}

	if(nDataLen +4 >= nMaxLen)
	{
		CSTCPrint("[suantong: not enough supply,need[%d] / supply[%d]]\n",nDataLen+4,nMaxLen);
		return CSUDI_FAILURE;
	}
	for(nTemp = nDataLen-1; nTemp >= 0; nTemp--)
	{
		pucData[nTemp+3]= pucData[nTemp];
	}	

	pucData[0]=0;
	pucData[1]=0;
	pucData[2]=nDataLen;
	chLRC=0;

	for (nTemp = 0; nTemp <  3+(INT)nDataLen; nTemp++)
	{
		chLRC^=pucData[nTemp];
	}

	pucData[nTemp]=chLRC;
	
	return CSUDI_SUCCESS;
}
/*********************************************************************************************
 *Function      : 计算真实ATR长度及传输协议(T0/T1/T14)
 *Function Name : CSCaculatAtrLen
 *Paramater     :
 *	pucAtr[IN]    : ATR数据
 *	dwAtrLen[IN]  : ATR长度
 *	pdwRLen[OUT]  : 通过ATR数据计算出真实的ATR长度，用于与dwAtrLen进行比较
 *	pcTransfT[OUT]: 计算出智能卡传输协议
 *Return        :
 *	SUCCESS:计算成功
 *	FAILURE:计算失败
 *Remark        :
 *	对于T0     : 没有校验字节
 *	对于T1/T14 : 将多出一个校验字节
 ********************************************************************************************/
int CSCaculatAtrLen(unsigned char *pucAtr, unsigned int dwAtrLen, unsigned int* pdwRLen, unsigned char *pcTransfT)
{
	int  nRet = CSUDI_FAILURE;
	unsigned char *pucTmp = pucAtr;
	unsigned char cHistoryLen;
	unsigned char cTChar;
	unsigned char cTCharLen = 0;
	unsigned char cCount = 0;
	
	CSASSERT(NULL != pucAtr);
	CSASSERT(0 < dwAtrLen);

	if((NULL != pucAtr) && (0 < dwAtrLen))
	{
		CSASSERT((pucAtr[0] == 0x3B) || (pucAtr[0] == 0x3F));

		if((pucAtr[0] == 0x3B) || (pucAtr[0] == 0x3F))
		{
			cHistoryLen = pucAtr[1] & 0x0F;//历史字节的长度
			cTChar = (pucAtr[1] & 0xF0) >> 4;//第二个unsigned char的值

			pucTmp += 2;
			*pcTransfT = 0;//t0协议

			while(1)
			{
				//TA(i)存在
				if(cTChar & 0x01)
				{
					cTCharLen++;
				}

				//TB(i)存在
				if(cTChar & 0x02)
				{
					cTCharLen++;
				}

				//Tc(i)存在
				if(cTChar & 0x04)
				{
					cTCharLen++;
				}

				//TD(i)存在
				if(cTChar & 0x08)
				{
					/*pucTmp += cTCharLen;*/
					cTChar  = (pucTmp[cTCharLen] & 0xF0) >> 4;
					if(cCount == 0)
					{
						*pcTransfT = pucTmp[cTCharLen] & 0x0F;
					}

					/*cTCharLen = 0;*/
					cTCharLen++;
					cCount++;
				}
				else
				{
					break;
				}
			}

			//pucTmp += cTCharLen;
			*pdwRLen = cTCharLen + cHistoryLen + 2;//其中加1是第1个字节0x3B/0x3F

			if((*pcTransfT == T1) || (*pcTransfT == T14))
			{
				*pdwRLen += 1;
			}
                     CSTCPrint("Cal Len:%d,Res Len:%d",*pdwRLen,dwAtrLen);
			if(*pdwRLen == dwAtrLen)
			{
			    nRet = CSUDI_SUCCESS;
			}
                    else
                    {
                          CSASSERT(*pdwRLen == dwAtrLen) ;
                    }
		}
	}
    else
    {
        CSASSERT(NULL != pucAtr) ;
        CSASSERT (0 < dwAtrLen);
    }

	return nRet;
}

/****************************************************************************************
 *Function      :校验T14传输是否正确
 *Function Name : CSCheckT14TranfIsRight
 *Parameter     :
 *pcSendData[IN]     :组装的往卡中写的数据
 *pcResponseData[IN] :卡中回应的数据
 *nResponseLen[IN]   :卡中返回的数据长度
 *Return Value :
 *	CSUDI_SUCCESS  : 接收回应的数据正确
 *	CSUDI_FAILURE    : 接收回应的数据错误
 ****************************************************************************************/
static int CSCheckT14TranfIsRight(unsigned char* pcSendData, unsigned char* pcResponseData, int nResponseLen)
{
	int nRet = CSUDI_FAILURE;
	int  ii;
	int  nResponseParseLen = 0;
	unsigned char cCheckSum = 0x3E;
	
	if((NULL != pcSendData) && (NULL != pcResponseData))
	{
		if((pcSendData[0] == pcResponseData[0]) && (pcSendData[1] == pcResponseData[1])
			&& (pcSendData[2] == pcResponseData[4]) && (pcSendData[3] == pcResponseData[5])
			&& (pcSendData[4] == pcResponseData[6]))
		{
			nResponseParseLen = pcResponseData[7] + 8 + 1;//其中1是校验字节，8是前回应数据的前8个字节
			if(nResponseParseLen == nResponseLen)
			{
				for(ii = 1; ii < (nResponseParseLen - 1); ii++)
				{
					cCheckSum ^= pcResponseData[ii]; //从Irdeto文档中找到的
				}
				
				if(cCheckSum == pcResponseData[nResponseParseLen - 1])
				{
					nRet = CSUDI_SUCCESS;
				}
			}
		}
	}
	else
	{
		CSASSERT(NULL != pcSendData);
		CSASSERT(NULL != pcResponseData);
	}
	
	return nRet;
}

/****************************************************************************************
 *Function      :校验T1传输是否正确
 *Function Name : CSCheckT1TranfIsRight
 *Parameter     :
 *pcResponseData[IN] :卡中回应的数据
 *nResponseLen[IN]   :卡中返回的数据长度
 *Return Value :
 *	CSUDI_SUCCESS  : 接收回应的数据正确
 *	CSUDI_FAILURE    : 接收回应的数据错误
 ****************************************************************************************/
static int CSCheckT1TranfIsRight(unsigned char * pcResponseData, int nResponseLen)
{
	int nRet = CSUDI_FAILURE;
	int  ii;
	unsigned char cCheckSum = 0;

	if(NULL != pcResponseData)
	{
		if(pcResponseData[2] + 4 == nResponseLen)/*4:NAD、PCB、LEN、EDC */
		{			
			for(ii = 0; ii < (nResponseLen - 1); ii++)/*LRC校验*/
			{
				cCheckSum ^= pcResponseData[ii]; 
			}

			if(cCheckSum == pcResponseData[nResponseLen - 1])
			{
				nRet = CSUDI_SUCCESS;
			}			
		}
	}
	else
	{
		CSASSERT(NULL != pcResponseData);
	}

	return nRet;
}

/****************************************************************************************
 *Function      :校验T0传输是否正确
 *Function Name : CSCheckT0TranfIsRight
 *Parameter     :
 *pcSendData[IN]     :组装的往卡中写的数据
 *nSendLen[IN]       :往卡中写入数据的长度
 *pucStatusWord[IN]  :卡中回应的状态字
 *nResponseLen[IN]   :卡中返回的数据长度
 *Return Value :
 *	CSUDI_SUCCESS  : 接收回应的数据正确
 *	CSUDI_FAILURE   : 接收回应的数据错误
 ****************************************************************************************/
static int CSCheckT0TranfIsRight(unsigned char * pcSendData, int nSendLen, int nResponseLen, unsigned char * pucStatusWord)
{
	int nRet = CSUDI_FAILURE;	
	
	if((NULL != pcSendData) && (NULL != pucStatusWord))
	{
		if(nSendLen > 5)//写指令
		{
			if((pucStatusWord[0] & 0x60) || (pucStatusWord[0] & 0x90))
			{
				nRet = CSUDI_SUCCESS;
			}			
		}
		else//读指令
		{
			if(pcSendData[4] == nResponseLen)
			{
				nRet = CSUDI_SUCCESS;
			}	
		}
	}
	else
	{
		CSASSERT(NULL != pcSendData);
		CSASSERT(NULL != pucStatusWord);
	}	
    
	return nRet;
}

/****************************************************************************************
 *Function      :校验数据传输是否正确
 *Function Name : CSCheckTranfIsRight
 *Parameter     :
 *eProtocalUsed[IN]  :交互协议
 *pcSendData[IN]     :组装的往卡中写的数据
 *nSendLen[IN]       :往卡中写入数据的长度
 *pcResponseData[IN] :卡中返回的数据
 *pucStatusWord[IN]  :卡中回应的状态字
 *nResponseLen[IN]   :卡中返回的数据长度
 *Return Value :
 *	CSUDI_SUCCESS  : 接收回应的数据正确
 *	CSUDI_FAILURE   : 接收回应的数据错误
 ****************************************************************************************/
static int CSCheckTranfIsRight(CSUDISMCProtocol_E eProtocalUsed, unsigned char* pcSendData, int nSendLen, unsigned char * pcResponseData, int nResponseLen, unsigned char * pucStatusWord)
{
	int  nRet = CSUDI_FAILURE;

	if(eProtocalUsed >= EM_UDISMC_PROTOCOL_MAX)
	{
		CSASSERT(eProtocalUsed < EM_UDISMC_PROTOCOL_MAX);
		return CSUDI_FAILURE;
	}

	switch(eProtocalUsed)
	{
		case EM_UDISMC_PROTOCOL_T0:
			if((NULL != pcSendData) && (NULL != pucStatusWord))
			{
				nRet = CSCheckT0TranfIsRight(pcSendData, nSendLen, nResponseLen, pucStatusWord);
			}
			else
			{
				CSASSERT(NULL != pcSendData);
				CSASSERT(NULL != pucStatusWord);
			}
			break;

		case EM_UDISMC_PROTOCOL_T1:
			if(NULL != pcResponseData)
			{
				nRet = CSCheckT1TranfIsRight(pcResponseData, nResponseLen);				
			}
			else
			{
				CSASSERT(NULL != pcResponseData);
			}
			break;

		case EM_UDISMC_PROTOCOL_T14:
			if((NULL != pcSendData) && (NULL != pcResponseData))
			{
				nRet = CSCheckT14TranfIsRight(pcSendData, pcResponseData, nResponseLen);
			}
			else
			{
				CSASSERT(NULL != pcSendData);
				CSASSERT(NULL != pcResponseData);
			}
			break;
		default:
			break;
	}

	return nRet;
}

static int CSCheckTranfIsRightEx(CSUDISMCProtocol_E eProtocalUsed,
									unsigned char* pcSendData,
									int nSendLen,
									unsigned char* pcRightReData,
									int nReLen,
									unsigned char * pcResponseData,
									int nResponseLen,
									unsigned char * pucRightSTWord,
									unsigned char * pucStatusWord)
{
	int  nRet = CSUDI_FAILURE;

	if(eProtocalUsed >= EM_UDISMC_PROTOCOL_MAX)
	{
		CSASSERT(eProtocalUsed < EM_UDISMC_PROTOCOL_MAX);
		return CSUDI_FAILURE;
	}
	
	/*if(nReLen >0 )
	{
		if(nReLen == nResponseLen &&
			0 == memcmp(pcRightReData,pcResponseData,nReLen))
		{
			nRet = 	CSUDI_SUCCESS;
		}
		else
		{
			CSFailInfoSaveFile("expect Data:",pcRightReData,nReLen);
			CSFailInfoSaveFile("actual Data:",pcResponseData,nResponseLen);
			return CSUDI_FAILURE;
		}
	}*/
	
	switch(eProtocalUsed)
	{
		case EM_UDISMC_PROTOCOL_T0:
			if(NULL != pucStatusWord)
			{
				nRet = CSCheckT0TranfIsRight(pcSendData, nSendLen, nResponseLen,pucStatusWord);
			}
			else
			{
				CSASSERT(NULL != pucStatusWord);
			}
			break;

		case EM_UDISMC_PROTOCOL_T1:
			if(NULL != pcResponseData)
			{
				nRet = CSCheckT1TranfIsRight(pcResponseData, nResponseLen);				
			}
			else
			{
				CSASSERT(NULL != pcResponseData);
			}
			break;

		case EM_UDISMC_PROTOCOL_T14:
			if(NULL != pcResponseData)
			{
				nRet = CSCheckT14TranfIsRight(pcSendData, pcResponseData, nResponseLen);
			}
			else
			{
				CSASSERT(NULL != pcSendData);
				CSASSERT(NULL != pcResponseData);
			}
			break;
		default:
			break;
	}

	return nRet;
}

CSUDI_Error_Code g_eSmcStatus = CSUDI_FAILURE;

/*打开时的回调函数*/
static void CSSMCCallBack(int nCardIndex, CSUDI_Error_Code eStatus)
{
	g_eSmcStatus = eStatus;
	switch(eStatus)
	{
		case CSUDISMC_ERROR_IN:
		{
			CSTCPrint("[smartcard callback]: CSUDISMC_ERROR_IN\n");
			break;
		}
		case CSUDISMC_ERROR_OUT:
		{
			CSTCPrint("[smartcard callback]: CSUDISMC_ERROR_OUT\n");
			break;
		}
		case CSUDISMC_ERROR_READY:
		{
			CSTCPrint("[smartcard callback]: CSUDISMC_ERROR_READY\n");
			break;
		}
		default:
		{
			CSTCPrint("[smartcard callback]: smartcard other status: 0x%x\n", eStatus);
			break;
		}
	}
}

BOOL CSTC_SMARTCARD_Init(void)
{
	//在本测试用例集执行前调用
	CSTCPrint("本模块测试用例默认将测试卡正确插入测试卡槽\n");

	return CSUDI_TRUE;
}

BOOL CSTC_SMARTCARD_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetCount
//@DESCRIPTION:测试pnSMCCount为空的情况
//@PRECONDITION:无
//@INPUT:pnSMCCount = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_GetCount_0001(void)
{ 
	CSTCPrint( "\n[CSTC_SMARTCARD_IT_GetCount_0001]测试参数为空的情况下获取卡槽的个数开始\n");

	CSTCPrint( "\n[CSTC_SMARTCARD_IT_GetCount_0001]返回值是:%d  \n",CSUDISMCGetCount(NULL));

       CSTCPrint("Before Test!!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCGetCount(NULL), "步骤1失败");
       CSTCPrint("After Test!!!\n");       
	CSTK_FATAL_POINT;
	CSTCPrint( "\n[CSTC_SMARTCARD_IT_GetCount_0001]测试参数为空 的情况下获取卡槽的个数结束\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetCount
//@DESCRIPTION:测试成功查询卡槽个数的情况
//@PRECONDITION:无
//@INPUT:int nCardNum = 0xff;
//@INPUT:pnSMCCount = &nCardNum;
//@EXPECTATION:接口返回CSUDI_TRUE，查询到的卡槽个数与实际个数相同
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询有效卡槽个数
//@EXECUTIONFLOW:2、查询config文件中的卡槽个数
//@EXECUTIONFLOW:3、判断两者是否相同，相同测试成功，否则测试失败
BOOL CSTC_SMARTCARD_IT_GetCount_0002(void)
{
	int nCardNum = 0xff;
	char *pcSection = "SMARTCARD";
	char *pckey =  "CARD_SLOT_NUM" ;
	char cResult[16];
	int  nResultSize = 16;
	CSUDI_BOOL bRet = CSUDI_FALSE;

	CSTCPrint("[CSTC_SMARTCARD_IT_GetCount_0002]测试正确获取卡槽的个数start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCGetCount(&nCardNum) , "步骤1失败");
	
	CSTCPrint("[CSTC_SMARTCARD_IT_GetCount_0002]获得的卡槽个数:%d\n",nCardNum);

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey,cResult,nResultSize), "步骤2失败\n");		
	CSTK_ASSERT_TRUE_FATAL(nCardNum == CSTKGetIntFromStr(cResult,10),"步骤3失败\n");	
	bRet = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_GetCount_0002]测试正确获取卡槽的个数end\n");

	return bRet;
}

//@CASEGROUP:CSUDISMCDetect
//@DESCRIPTION: 测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION: 无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用接口CSUDISMCDetect，传入参数-1，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Detect_0003(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0003]测试卡槽索引无效时(等于-1)检测卡状态start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCDetect(-1), "步骤1失败");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0003]测试卡槽索引无效时(等于-1)检测卡状态end\n");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDISMCDetect
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用接口CSUDISMCDetect，传入参数nCardNum，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Detect_0004(void)
{
	int nCardNum = 0xff;

	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0004]测试卡槽索引无效(等于最大有效索引号+1)时检测卡状态start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDetect(nCardNum), "步骤2失败");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0004]测试卡槽索引无效(等于最大有效索引号+1)时检测卡状态end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetParams，设置参数nCardIndex = -1，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0005(void)
{
	CSUDISMCParam_S sSmcParm;
	
	sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_NO_PARITY;
	sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
	sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
	sSmcParm.m_eStopBit = EM_UDISMC_STOP_0;
	sSmcParm.m_nBaudRate = 115200;
	sSmcParm.m_nETU = 372;
	sSmcParm.m_nFrequency = 3570000;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0005]测试卡槽索引号无效(等于-1)时执行CSUDISMCSetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(-1, &sSmcParm), "步骤1失败");

	CSTK_FATAL_POINT;	
       
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0005]测试卡槽索引号无效(等于-1)时执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用CSUDISMCSetParams，设置参数nCardIndex = nCardNum，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0006(void)
{
	int nCardNum = 0xff;	
	CSUDISMCParam_S sSmcParm;

	sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_NO_PARITY;
	sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
	sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
	sSmcParm.m_eStopBit = EM_UDISMC_STOP_0;
	sSmcParm.m_nBaudRate = 115200;
	sSmcParm.m_nETU = 372;
	sSmcParm.m_nFrequency = 3570;	

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0006]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCSetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(nCardNum, &sSmcParm), "步骤2失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0006]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试psSMCParams指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:nCardIndex = 0;
//@INPUT:psSMCParams = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0007(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0007]测试psSMCParams指针为空时执行CSUDISMCSetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==   CSUDISMCSetParams(0, NULL), "步骤1失败");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0007]测试psSMCParams指针为空时执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试psSMCParams的m_eCheckBit不合法(EM_UDISMC_PARITY_MAX+1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:CSUDISMCParam_S sSmcParm; 
//@INPUT:sSmcParm.m_eCheckBit = EM_UDISMC_PARITY_MAX+1;
//@INPUT:sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
//@INPUT:sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
//@INPUT:sSmcParm.m_eStopBit = EM_UDISMC_STOP_0;
//@INPUT:sSmcParm.m_nBaudRate = 115200;
//@INPUT:sSmcParm.m_nETU = 372;
//@INPUT:sSmcParm.m_nFrequency = 3570;
//@INPUT:nCardIndex = 0;
//@INPUT:psSMCParams  = &sSmcParm;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0008(void)
{
	CSUDISMCParam_S sSmcParm;
	
	sSmcParm.m_eCheckBit = EM_UDISMC_PARITY_MAX+1;
	sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
	sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
	sSmcParm.m_eStopBit = EM_UDISMC_STOP_0;
	sSmcParm.m_nBaudRate = 115200;
	sSmcParm.m_nETU = 372;
	sSmcParm.m_nFrequency = 3570;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0008]测试参数psSMCParams的m_eCheckBit不合法(EM_UDISMC_PARITY_MAX)的情况执行CSUDISMCSetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "步骤1失败");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0008]]测试参数psSMCParams的m_eCheckBit不合法(EM_UDISMC_PARITY_MAX)的情况执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试psSMCParams的m_eProtocol不合法(EM_UDISMC_PROTOCOL_MAX+1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:CSUDISMCParam_S sSmcParm; 	
//@INPUT:sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
//@INPUT:sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_MAX+1;
//@INPUT:sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
//@INPUT:sSmcParm.m_eStopBit = EM_UDISMC_STOP_0;
//@INPUT:sSmcParm.m_nBaudRate = 115200;
//@INPUT:sSmcParm.m_nETU = 372;
//@INPUT:sSmcParm.m_nFrequency = 3570;
//@INPUT:nCardIndex = 0;
//@INPUT:psSMCParams  = &sSmcParm;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0009(void)
{
	CSUDISMCParam_S sSmcParm;
	
	sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
	sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_MAX+1;
	sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
	sSmcParm.m_eStopBit = EM_UDISMC_STOP_0;
	sSmcParm.m_nBaudRate = 115200;
	sSmcParm.m_nETU = 372;
	sSmcParm.m_nFrequency = 3570;		

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0009]测试参数psSMCParams的m_eProtocol不合法(EM_UDISMC_PROTOCOL_MAX)的情况执行CSUDISMCSetParams start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "步骤1失败");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0009]测试参数psSMCParams的m_eProtocol不合法(EM_UDISMC_PROTOCOL_MAX)的情况执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试psSMCParams的m_eStandard不合法(EM_UDISMC_STANDARD_MAX+1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:CSUDISMCParam_S sSmcParm; 	
//@INPUT:sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
//@INPUT:sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
//@INPUT:sSmcParm.m_eStandard = EM_UDISMC_STANDARD_MAX+1;
//@INPUT:sSmcParm.m_eStopBit = EM_UDISMC_STOP_0;
//@INPUT:sSmcParm.m_nBaudRate = 115200;
//@INPUT:sSmcParm.m_nETU = 372;
//@INPUT:sSmcParm.m_nFrequency = 3570;
//@INPUT:nCardIndex = 0;
//@INPUT:psSMCParams  = &sSmcParm;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0010(void)
{
	CSUDISMCParam_S sSmcParm;
	
	sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
	sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
	sSmcParm.m_eStandard = EM_UDISMC_STANDARD_MAX+1;
	sSmcParm.m_eStopBit = EM_UDISMC_STOP_0;
	sSmcParm.m_nBaudRate = 115200;
	sSmcParm.m_nETU = 372;
	sSmcParm.m_nFrequency = 3570;	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0010]测试参数psSMCParams的m_eStandard不合法(EM_UDISMC_STANDARD_MAX)的情况执行CSUDISMCSetParams start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "步骤1失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0010]]测试参数psSMCParams的m_eStandard不合法(EM_UDISMC_STANDARD_MAX)的情况执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试psSMCParams的m_eStopBit不合法(EM_UDISMC_STOP_MAX+1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:CSUDISMCParam_S sSmcParm; 	
//@INPUT:sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
//@INPUT:sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
//@INPUT:sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
//@INPUT:sSmcParm.m_eStopBit = EM_UDISMC_STOP_MAX+1;
//@INPUT:sSmcParm.m_nBaudRate = 115200;
//@INPUT:sSmcParm.m_nETU = 372;
//@INPUT:sSmcParm.m_nFrequency = 3570;
//@INPUT:nCardIndex = 0;
//@INPUT:psSMCParams  = &sSmcParm;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0011(void)
{
	CSUDISMCParam_S sSmcParm;
	
	sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
	sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
	sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
	sSmcParm.m_eStopBit = EM_UDISMC_STOP_MAX+1;
	sSmcParm.m_nBaudRate = 115200;
	sSmcParm.m_nETU = 372;
	sSmcParm.m_nFrequency = 3570;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0011]测试参数psSMCParams的m_eStopBit不合法(EM_UDISMC_STOP_MAX)的情况执行CSUDISMCSetParams start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "步骤1失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0011]]测试参数psSMCParams的m_eStopBit不合法(EM_UDISMC_STOP_MAX)的情况执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试psSMCParams的m_nBaudRate不合法(等于-1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:CSUDISMCParam_S sSmcParm; 	
//@INPUT:sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
//@INPUT:sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
//@INPUT:sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
//@INPUT:sSmcParm.m_eStopBit = EM_UDISMC_STOP_3;
//@INPUT:sSmcParm.m_nBaudRate = -1;
//@INPUT:sSmcParm.m_nETU = 372;
//@INPUT:sSmcParm.m_nFrequency = 3570;
//@INPUT:nCardIndex = 0;
//@INPUT:psSMCParams  = &sSmcParm;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0012(void)
{
	CSUDISMCParam_S sSmcParm;
	
	sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
	sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
	sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
	sSmcParm.m_eStopBit = EM_UDISMC_STOP_3 ;
	sSmcParm.m_nBaudRate = -1;
	sSmcParm.m_nETU = 372;
	sSmcParm.m_nFrequency = 3570;	

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0012]测试参数psSMCParams的m_nBaudRate不合法(等于-1)的情况执行CSUDISMCSetParams start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "步骤1失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0012]测试参数psSMCParams的m_nBaudRate不合法(等于-1)的情况执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试psSMCParams的m_nETU不合法(等于-1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:CSUDISMCParam_S sSmcParm; 	
//@INPUT:sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
//@INPUT:sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
//@INPUT:sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
//@INPUT:sSmcParm.m_eStopBit = EM_UDISMC_STOP_3;
//@INPUT:sSmcParm.m_nBaudRate = 115200;
//@INPUT:sSmcParm.m_nETU = -1;
//@INPUT:sSmcParm.m_nFrequency = 3570;
//@INPUT:nCardIndex = 0;	
//@INPUT:psSMCParams  = &sSmcParm;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0013(void)
{
	CSUDISMCParam_S sSmcParm;
	
	sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
	sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
	sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
	sSmcParm.m_eStopBit = EM_UDISMC_STOP_3 ;
	sSmcParm.m_nBaudRate = 115200;
	sSmcParm.m_nETU = -1;
	sSmcParm.m_nFrequency = 3570;	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0013]测试参数psSMCParams的m_nETU不合法(等于-1)的情况执行CSUDISMCSetParams start\n");
	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "步骤1失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0013]]测试参数psSMCParams的m_nETU不合法(等于-1)的情况执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:测试psSMCParams的m_nFrequency不合法(等于-1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:CSUDISMCParam_S sSmcParm; 	
//@INPUT:sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
//@INPUT:sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
//@INPUT:sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
//@INPUT:sSmcParm.m_eStopBit = EM_UDISMC_STOP_3;
//@INPUT:sSmcParm.m_nBaudRate = 115200;
//@INPUT:sSmcParm.m_nETU = 372;
//@INPUT:sSmcParm.m_nFrequency = -1;
//@INPUT:nCardIndex = 0;	
//@INPUT:psSMCParams  = &sSmcParm;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0014(void)
{
	CSUDISMCParam_S sSmcParm;
	
	sSmcParm.m_eCheckBit = EM_UDISMC_8BITS_EVEN_PARITY;
	sSmcParm.m_eProtocol = EM_UDISMC_PROTOCOL_T0;
	sSmcParm.m_eStandard = EM_UDISMC_STANDARD_ISO;
	sSmcParm.m_eStopBit = EM_UDISMC_STOP_3 ;
	sSmcParm.m_nBaudRate = 115200;
	sSmcParm.m_nETU = 372;
	sSmcParm.m_nFrequency = -1;	

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0014]测试参数psSMCParams的m_nFrequency不合法(等于-1)的情况执行CSUDISMCSetParams start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "步骤1失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0014]]测试参数psSMCParams的m_nFrequency不合法(等于-1)的情况执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetParams
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_GetParam_0015(void)
{
	CSUDISMCParam_S sSmcParm;

	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0015]测试卡槽索引号无效(等于-1)时执行CSUDISMCGetParams start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCGetParams(-1, &sSmcParm), "步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0015]]测试卡槽索引号无效(等于-1)时执行CSUDISMCGetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetParams
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用CSUDISMCGetParams，传入参数nCardNum，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_GetParam_0016(void)
{
	int nCardNum = 0xff;	
	CSUDISMCParam_S sSmcParm;		

	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0016]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCGetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCGetParams(nCardNum, &sSmcParm), "步骤2失败");
	
	CSTK_FATAL_POINT;	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0016]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCGetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetParams
//@DESCRIPTION:测试psSMCParams指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:nCardIndex = 0;
//@INPUT:psSMCParams = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetParams，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_GetParam_0017(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0017]测试psSMCParams指针为空时执行CSUDISMCGetParams start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCGetParams(0, NULL), "步骤1失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0017]测试psSMCParams指针为空时执行CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCOpen，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Open_0018(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0018]测试卡槽索引号无效(等于-1)的情况下执行CSUDISMCOpen  start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCOpen(-1, CSSMCCallBack), "步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0018]测试卡槽索引号无效(等于-1)的情况下执行CSUDISMCOpen  end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用CSUDISMCOpen，传入参数nCardNum，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Open_0019(void)
{
	int nCardNum = 0xff;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0019]测试卡槽索引号无效(等于最大有效索引号+1)的情况下执行CSUDISMCOpen  start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCOpen(nCardNum, CSSMCCallBack), "步骤2失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0019]测试卡槽索引号无效(等于最大有效索引号+1)的情况下执行CSUDISMCOpen  end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:测试fnSMCCallback指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:nCardIndex = 0;
//@INPUT:fnSMCCallback = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCOpen，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Open_0020(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0020]测试fnSMCCallback指针为空时执行CSUDISMCOpen start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCOpen(0, NULL), "步骤1失败");
	
	CSTK_FATAL_POINT;		
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0020]测试fnSMCCallback指针为空时执行CSUDISMCOpen end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCClose
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCClose，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Close_0021(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0021]测试卡槽索引号无效(等于-1)时执行CSUDISMCClose start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCClose(-1), "步骤1失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0021]]测试卡槽索引无效(等于-1)时执行CSUDISMCClose end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCClose
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用CSUDISMCClose，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Close_0022(void)
{
	int nCardNum;

	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0022]测试卡槽索引号无效(等于最大有效索引号+1)执行CSUDISMCClose start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCClose(nCardNum), "步骤2失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0022]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCClose end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCReset，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Close_0023(void)
{
	unsigned char aucAtr[64] = {0};
	int nAtrLen;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0023]测试卡槽索引号无效(等于-1)时执行CSUDISMCReset start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCReset(-1, aucAtr, &nAtrLen, CSUDI_TRUE), "步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0023]]测试卡槽索引号无效(等于-1)时执行CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用CSUDISMCReset，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Reset_0024(void)
{
	unsigned char aucAtr[64] = {0};
	int nAtrLen;
	int nCardNum;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0024]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCReset start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCReset(nCardNum, aucAtr, &nAtrLen, CSUDI_TRUE), "步骤2失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0024]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:测试pucAtr指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:nCardIndex = 0;
//@INPUT:pucAtr = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCReset，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Reset_0025(void)
{
	int nAtrLen;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0025]测试puAtr指针为空时执行CSUDISMCReset start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCReset(0, NULL, &nAtrLen, CSUDI_TRUE), "步骤1失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0025]测试puAtr指针为空时执行CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:测试pnAtrLength指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucAtr指针不为空
//@INPUT:unsigned char aucAtr[64] = {0};	
//@INPUT:nCardIndex = 0; 
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength =NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCReset，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Reset_0026(void)
{
	unsigned char aucAtr[64] = {0};	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0026]测试pnAtrLength指针为空时执行CSUDISMCReset start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCReset(0, aucAtr, NULL, CSUDI_TRUE), "步骤1失败");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0026]测试pnAtrLength指针为空时执行CSUDISMCReset end\n");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCDataExchange，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0027(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0027]测试卡槽索引号无效(等于-1)时执行CSUDISMCDataExchange start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(-1, aucDataToWrite, 5, aucResData, &nNumRead, pucStatusWord), "步骤1失败");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0027]]测试卡槽索引号无效(等于-1)时执行CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用CSUDISMCDataExchange，判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0028(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	int nNumRead;
	int nCardNum;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0028]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCDataExchange start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(nCardNum, aucDataToWrite, 5, aucResData, &nNumRead, pucStatusWord), "步骤2失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0028]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试pucWriteData指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCDataExchange,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0029(void)
{
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0029]测试pucWriteData指针为空时执行CSUDISMCDataExchange start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, NULL, 5, aucResData, &nNumRead, pucStatusWord), "步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0029]测试pucWriteData指针为空时执行CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试nNumberToWrite不合法(等于-1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucWriteData不为空
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWrite = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCDataExchange,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0030(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0030]测试nNumberToWrite不合法(等于-1)时执行CSUDISMCDataExchange start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, aucDataToWrite, -1, aucResData, &nNumRead, pucStatusWord), "步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0030]测试nNumberToWrite不合法(等于-1)时执行CSUDISMCDataExchange end\n");


	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试pcResponseData指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucWriteData不为空
//@PRECONDITION:3.nNumberToWrite有效
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pcResponseData = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCDataExchange,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0031(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char pucStatusWord[2];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0031]测试pcResponseData指针为空时执行CSUDISMCDataExchange start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, aucDataToWrite, 5, NULL, &nNumRead, pucStatusWord), "步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0031]测试pcResponseData指针为空时执行CSUDISMCDataExchange end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试pnNumberRead指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucWriteData不为空
//@PRECONDITION:3.nNumberToWrite有效
//@PRECONDITION:4.pcResponseData不为空
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:unsigned char aucResData[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCDataExchange,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0032(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0032]测试pnNumberRead指针为空时执行CSUDISMCDataExchange start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, aucDataToWrite, 5, aucResData, NULL, pucStatusWord), "步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0032]测试pnNumberRead指针为空时执行CSUDISMCDataExchange end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试pucStatusWord指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucWriteData指针不为空
//@PRECONDITION:3.nNumberToWrite有效
//@PRECONDITION:4.pcResponseData指针不为空
//@PRECONDITION:5.pnNumberRead指针不为空
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCDataExchange,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0033(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0033]测试pucStatusWord指针为空时执行CSUDISMCDataExchange start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, aucDataToWrite, 5, aucResData, &nNumRead, NULL), "步骤1失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0033]测试pucStatusWord指针为空时执行CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSend,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0034(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumWrite;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0034]测试卡槽索引号无效(等于-1)时执行CSUDISMCSend start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(-1, aucDataToWrite, 5, &nNumWrite, 1000), "步骤1失败");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0034]测试卡槽索引号无效(等于-1)时执行CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用CSUDISMCSend,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0035(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumToWrite=5;
	int nNumWrite;
	int nTimeout=1000;
	int nCardNum;

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0035]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCSend start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(nCardNum, aucDataToWrite, nNumToWrite, &nNumWrite, nTimeout), "步骤2失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0035]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:测试pucWriteBuf指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = NULL;
//@EXPECTATION::接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSend,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0036(void)
{
	int nNumToWrite=5;
	int nNumWrite;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0036]测试pucWriteBuf指针为空时执行CSUDISMCSend start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(0, NULL, nNumToWrite, &nNumWrite, 1000), "步骤1失败");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0036]]测试卡槽索引号无效(等于-1)时执行CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:测试nNumberToWrite不合法(等于-1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucWriteBuf不为空
//@INPUT:ubsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSend,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0037(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumWrite;

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0037]]测试nNumberToWrite不合法(等于-1)时执行CSUDISMCSend end\n");
				
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(0, aucDataToWrite, -1, &nNumWrite, 1000), "步骤1失败");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0037]]测试nNumberToWrite不合法(等于-1)时执行CSUDISMCSend end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:测试pnNumberWrite指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucWriteBuf不为空
//@PRECONDITION:3.nNumberToWrite有效
//@INPUT:ubsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pnNumberWrite = NULL;
//@EXPECTATION::接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSend,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0038(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumToWrite=5;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0038]测试pnNumberWrite指针为空时执行CSUDISMCSend start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(0, aucDataToWrite, nNumToWrite, NULL, 1000), "步骤1失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0038]测试pnNumberWrite指针为空时执行CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:测试nTimeout不合法(等于-1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucWriteBuf不为空
//@PRECONDITION:3.nNumberToWrite有效
//@PRECONDITION:4.pnNumberWrite 不为空;
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:int nNumWrite;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pnNumberWrite = &nNumWrite;
//@INPUT:nTimeout = -1;  
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSend,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0039(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumToWrite=5;
	int nNumWrite;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0039]测试nTimeout不合法(等于-1)时执行CSUDISMCSend start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(0, aucDataToWrite, nNumToWrite,&nNumWrite, -1), "步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0039]测试nTimeout不合法(等于-1)时执行CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCReceive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0040(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=5;
	int nNumRead;
	int nTimeout=1000;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0040]测试卡槽索引号无效(等于-1)时执行CSUDISMCReceive start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(-1, aucDataToRead, nNumToRead, &nNumRead, nTimeout),"步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0040]]测试卡槽索引号无效(等于-1)时执行CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION::接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用CSUDISMCReceive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0041(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=5;
	int nNumRead;
	int nTimeout=1000;
	int nCardNum;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0041]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCReceive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(nCardNum, aucDataToRead, nNumToRead, &nNumRead, nTimeout), "步骤2失败");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0041]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCReceive end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:测试pucReadBuf指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:nCardIndex = 0;
//@INPUT:pucReadBuf = NULL;
//@EXPECTATION::接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCReceive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0042(void)
{
	int nNumToRead=5;
	int nNumRead;
	int nTimeout=1000;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0042]测试pucReadBuf指针为空时执行CSUDISMCReceive start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(0, NULL, nNumToRead, &nNumRead, nTimeout), "步骤1失败");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0042]测试pucReadBuf指针为空时执行CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:测试nNumberToRead不合法(等于-1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucReadBuf不为空
//@INPUT:unsigned char aucDataRead[64]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucReadBuf = aucDataRead;
//@INPUT:nNumberToRead = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCReceive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0043(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=-1;
	int nNumRead;
	int nTimeout=1000;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0043]测试nNumberToRead不合法(等于-1)时执行CSUDISMCReceive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(0, aucDataToRead, nNumToRead, &nNumRead, nTimeout), "步骤1失败");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0043]测试nNumberToRead不合法(等于-1)时执行CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:测试pnNumberRead指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucReadBuf不为空
//@PRECONDITION:3.nNumberToRead有效
//@INPUT:unsigned char aucDataRead[64]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucReadBuf = aucDataRead;
//@INPUT:nNumberToRead = 4;
//@INPUT:pnNumberRead = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCReceive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0044(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=5;
	int nTimeout=1000;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0044]测试pnNumberRead指针为空时执行CSUDISMCReceive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(0, aucDataToRead, nNumToRead, NULL, nTimeout), "步骤1失败");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0044]测试pnNumberRead指针为空时执行CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:测试nTimeout不合法(等于-1)的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucReadBuf不为空
//@PRECONDITION:3.nNumberToRead有效
//@PRECONDITION:4.pnNumberRead不为空
//@INPUT:unsigned char aucDataRead[64]={0};
//@INPUT:int nNumRead;
//@INPUT:nCardIndex = 0;
//@INPUT:pucReadBuf = aucDataRead;
//@INPUT:nNumberToRead = 4;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:nTimeout = -1；
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCReceive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0045(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=5;
	int nNumRead;
	int nTimeout=-1;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0045]测试nTimeout不合法(等于-1)时执行CSUDISMCReceive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(0, aucDataToRead, nNumToRead, &nNumRead, nTimeout), "步骤1失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0045]测试nTimeout不合法(等于-1)时执行CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetPPS,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0046(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucDataToRead[5]={0};
	int nNumToWrite=5;
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0046]测试卡槽索引号无效(等于-1)时执行CSUDISMCSetPPS start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetPPS(-1, aucDataToWrite, nNumToWrite, aucDataToRead, &nNumRead), "步骤1失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0046]]测试卡槽索引号无效(等于-1)时执行CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:2、调用CSUDISMCSetPPS,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0047(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucDataToRead[5]={0};
	int nNumToWrite=5;
	int nNumToRead;
	int nCardNum;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0047]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCSetPPS start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCSetPPS(nCardNum, aucDataToWrite, nNumToWrite, aucDataToRead, &nNumToRead), "步骤2失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0047]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:测试pucWriteData指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetPPS,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0048(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToWrite=5;
	int nNumToRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0048]测试pucWriteData指针为空时执行CSUDISMCSetPPS start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCSetPPS(0, NULL, nNumToWrite, aucDataToRead, &nNumToRead), "步骤1失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0048]测试pucWriteData指针为空时执行CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:测试pucResponseData指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucWriteData不为空
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWriteLen = 5;
//@INPUT:pucResponseData = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetPPS,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0049(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumToWrite=5;
	int nNumToRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0049]测试pucResponseData指针为空时执行CSUDISMCSetPPS start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCSetPPS(0, aucDataToWrite, nNumToWrite, NULL, &nNumToRead), "步骤1失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0049]测试pucResponseData指针为空时执行CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:测试pnResponseDataLen指针为空的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucWriteData不为空
//@PRECONDITION:3.pucResponseData不为空
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:unsigned char aucResData[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWriteLen = 5;
//@INPUT:pucResponseData = aucResData;
//@INPUT:pnResponseDataLen = NULL;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCSetPPS,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0050(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucDataToRead[5]={0};
	int nNumToWrite=5;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0050]测试pnResponseDataLen指针为空时执行CSUDISMCSetPPS start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCSetPPS(0, aucDataToWrite, nNumToWrite, aucDataToRead, NULL), "步骤1失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0050]测试pnResponseDataLen指针为空时执行CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCActive
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCActive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Active_0051(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Active_0051]测试卡槽索引号无效(等于-1)时执行CSUDISMCActive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCActive(-1), "步骤1失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Active_0051]]测试卡槽索引号无效(等于-1)时执行CSUDISMCActive end\n");	

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCActive
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:1、调用CSUDISMCActive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Active_0052(void)
{
	int nCardNum;

	CSTCPrint("[CSTC_SMARTCARD_IT_Active_0052]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCActive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==CSUDISMCActive(nCardNum), "步骤2失败");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Active_0052]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCActive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDeactive
//@DESCRIPTION:测试卡槽索引号无效(等于-1)的情况
//@PRECONDITION:无
//@INPUT:nCardIndex = -1;
//@EXPECTATION::接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCDeactive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Deactive_0053(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Deactive_0053]测试卡槽索引号无效(等于-1)时执行CSUDISMCDeactive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCDeactive(-1), "步骤1失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Deactive_0053]测试卡槽索引号无效(等于-1)时执行CSUDISMCDeactive end\n");	

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDeactive
//@DESCRIPTION:测试卡槽索引号无效(等于最大有效索引号+1)的情况
//@PRECONDITION:1.正确查询系统中的卡槽个数nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:接口返回CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDISMCGetCount查询系统中有效卡槽的个数nCardNum
//@EXECUTIONFLOW:1、调用CSUDISMCDeactive,判断返回值是否等于CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Deactive_0054(void)
{
	int nCardNum;

	CSTCPrint("[CSTC_SMARTCARD_IT_Deactive_0054]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCActive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCDeactive(nCardNum), "步骤2失败");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Deactive_0054]测试卡槽索引号无效(等于最大有效索引号+1)时执行CSUDISMCDeactive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDetect
//@DESCRIPTION:测试成功检测卡状态的情况
//@PRECONDITION:1.欲检测的卡槽索引号有效
//@INPUT:nCardIndex = 用户输入的卡槽索引;
//@EXPECTATION:正确查询卡的状态
//@EXECUTIONFLOW:1、从配置文件中查询测试卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数值
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、提示并等待用户插入智能卡
//@EXECUTIONFLOW:6、调用接口CSUDISMCDetect
//@EXECUTIONFLOW:7、判断CSUDISMCDetect返回值是否等于CSUDISMC_ERROR_IN
//@EXECUTIONFLOW:8、提示并等待用户拔掉智能卡
//@EXECUTIONFLOW:9、调用接口CSUDISMCDetect
//@EXECUTIONFLOW:10、判断CSUDISMCDetect返回值是否等于CSUDISMC_ERROR_OUT
//@EXECUTIONFLOW:11、调用CSUDISMCReset
//@EXECUTIONFLOW:12、调用接口CSUDISMCDetect
//@EXECUTIONFLOW:13、判断CSUDISMCDetect返回值是否等于CSUDISMC_ERROR_READY
//@EXECUTIONFLOW:14、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Detect_0055(void)
{
	int nInputCardSlotNum = 0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParam;
	unsigned char aucAtr[64] = {0};
	int nAtrLen;
	BOOL bOpened = CSUDI_FALSE;
	BOOL bRemoved = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0055]测试正确检测卡状态start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
		
 	sCardParam.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParam.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParam.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParam.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParam.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParam.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParam.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParam)," 步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	bOpened = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_IN == CSUDISMCDetect(nInputCardSlotNum),"步骤7失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤8失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_READY == CSUDISMCDetect(nInputCardSlotNum),"步骤9失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0055]请将测试卡从测试卡槽拔出(按任意键继续)\r\n");
	CSTKWaitAnyKey();
	bRemoved = CSUDI_TRUE;
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_OUT == CSUDISMCDetect(nInputCardSlotNum),"步骤10失败");
	
	CSTK_FATAL_POINT;
		
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"步骤14，销毁资源失败");
	}
	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("请将测试卡正确插入测试卡槽\n");
		CSTKWaitAnyKey();
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0055]测试正确检测卡状态start\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams/CSUDISMCGetParams
//@DESCRIPTION:测试成功设置、查询参数的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.psSMCParams指针不为空且各成员都有效
//@INPUT:nCardIndex = 用户输入的卡槽索引;
//@INPUT:psSMCParams  = 用户输入的参数;
//@EXPECTATION:查询到的各参数和设置的一致，使用示波器测量波特率和频率与设置的值一致
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据用户的输入获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、判断返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDISMCOpen，回调函数是预先定义好的回调函数
//@EXECUTIONFLOW:6、调用CSUDISMCReset，判断返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDISMCGetParams，比较查询到的参数与设置的是否相同
//@EXECUTIONFLOW:8、使用示波器测量频率与设置的值是否一致
//@EXECUTIONFLOW:9、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_SetParamGetParam_0056(void)
{	
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	CSUDISMCParam_S sCardParamGet;
	BOOL bOpened = CSUDI_FALSE;
	unsigned char aucAtr[64] = {0};
	int nAtrLen;

	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParamGetParam_0056]测试成功设置、查询参数 start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," 步骤4失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤5失败");

	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤6失败");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISMCGetParams(nInputCardSlotNum, &sCardParamGet)),"步骤7失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParamGetParam_0056] 查询的checkbit是%d,standard是%d,stopbit是%d,frequency是%d,\
		protocal是%d,baudrate是%d,etu是%d",sCardParamGet.m_eCheckBit,sCardParamGet.m_eStandard,sCardParamGet.m_eStopBit,sCardParamGet.m_nFrequency,
		sCardParamGet.m_eProtocol,sCardParamGet.m_nBaudRate,sCardParamGet.m_nETU);

	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_eCheckBit == sCardParamSet.m_eCheckBit),"步骤7.1失败"); 
	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_eStandard == sCardParamSet.m_eStandard) ,"步骤7.2失败");
	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_eStopBit == sCardParamSet.m_eStopBit),"步骤7.3失败");
	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_eProtocol == sCardParamSet.m_eProtocol),"步骤7.4失败"); 
	
	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_nFrequency/sCardParamGet.m_nBaudRate +1 == sCardParamGet.m_nETU 
	||sCardParamGet.m_nFrequency/sCardParamGet.m_nBaudRate == sCardParamGet.m_nETU 
	||sCardParamGet.m_nFrequency/sCardParamGet.m_nBaudRate -1 == sCardParamGet.m_nETU),"步骤7.5失败");
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParamGetParam_0056] 设置的频率是:%d,请使用示波器测量是否与设置的一致(是按Y,否按N)\n",sCardParamSet.m_nFrequency);

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"步骤8失败");
	
	CSTK_FATAL_POINT;
		
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"步骤9，销毁资源失败");
	}
		
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParamGetParam_0056]测试测试成功设置、查询参数 end\n");	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:测试对同一个卡槽成功打开后再次打开的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.fnSMCCallback不为空
//@PRECONDITION:3.卡槽之前被成功打开
//@INPUT:static void CSSMCCallBack(int nCardIndex, CSUDI_Error_Code eStatus);
//@INPUT:nCardIndex = 0;
//@INPUT:fnSMCCallback = CSSMCCallBack;
//@EXPECTATION:接口返回CSUDISMC_ERROR_FAILURE
//@EXECUTIONFLOW:1、从配置文件中查询测试卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号
//@EXECUTIONFLOW:3、根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen，判断返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDISMCOpen，判断返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDISMCClose，判断返回值是否等于CSUDI_SUCCESS
BOOL CSTC_SMARTCARD_IT_Open_0057(void)
{	
	int nInputCardSlotNum=0;
	BOOL bOpened = CSUDI_FALSE;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	char *pcSection = "SMARTCARD";
	char *pckey1 = "TEST_CARD_SLOT_INDEX";
	char *pckey2 = "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int nResultSize = 16;

	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0057]测试对同一个卡槽成功打开后再次打开 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," 步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	bOpened = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤5失败");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0057]测试对同一个卡槽成功打开后再次打开 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:测试成功打开、关闭卡槽的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.fnSMCCallback不为空
//@PRECONDITION:3.卡槽之前没有被成功打开
//@PRECONDITION:4.连好PC和机顶盒串口，打开串口工具
//@INPUT:nCardIndex = 0;
//@INPUT:fnSMCCallback = CSSMCCallBack;
//@EXPECTATION:成功打开后，插拔智能卡，在串口打印中能够看到回调函数检测到卡In和卡out时输出的信息；成功关闭后，插拔卡时在串口打印中看不到回调函数检测到卡In和卡Out时输出的信息
//@EXECUTIONFLOW:1、从配置文件中查询测试卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据卡索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数，调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:3、调用CSUDISMCOpen，回调函数是预先定义好的回调函数
//@EXECUTIONFLOW:4、提示用户拔出智能卡，判断是否有回调上来
//@EXECUTIONFLOW:5、提示用户插如智能卡，判断是否有回调上来
//@EXECUTIONFLOW:6、调用CSUDISMCClose
//@EXECUTIONFLOW:7、提示用户拔出智能卡，判断是否有回调上来
//@EXECUTIONFLOW:8、提示用户插如智能卡，判断是否有回调上来
BOOL CSTC_SMARTCARD_IT_Open_0058(void)
{	
	int nInputCardSlotNum=0;
	BOOL bOpened = CSUDI_FALSE;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	BOOL bRemoved = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]测试成功打开、关闭卡槽 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "步骤1失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);	
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," 步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤3失败");
	bOpened = CSUDI_TRUE;	
	
	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]请拔出智能卡\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_OUT, "步骤4失败");

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]请插入智能卡\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_IN, "步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "步骤6失败");	
	
	bOpened = CSUDI_FALSE;	

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]请拔出智能卡\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDI_FAILURE, "步骤7失败");
	bRemoved = CSUDI_TRUE;

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]请插入智能卡\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDI_FAILURE, "步骤8失败");
	bRemoved = CSUDI_FALSE;

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "销毁资源失败");
	}
	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("请将测试卡正确插入测试卡槽\n");
		CSTKWaitAnyKey();
	}
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]测试成功打开、关闭卡槽 end\n");	

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCClose
//@DESCRIPTION:测试关闭之前卡槽没有打开的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.卡槽之前没有被成功打开
//@INPUT:nCardIndex = 0;
//@EXPECTATION:接口返回CSUDISMC_ERROR_FAILURE
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、调用CSUDISMCClose，判断返回值是否等于CSUDISMC_ERROR_FAILURE
BOOL CSTC_SMARTCARD_IT_Close_0059(void)
{	
	int nInputCardSlotNum=0;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char cResult1[16];
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0059]测试关闭之前卡槽没有打开 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCClose(nInputCardSlotNum), "步骤2失败");		

	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0059]测试关闭之前卡槽没有打开 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:测试卡槽中没有插入智能卡时Reset的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucAtr指针不为空
//@PRECONDITION:3.pnAtrLength指针不为空
//@PRECONDITION:4.卡槽中没有插入智能卡
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = 用户输入的索引号;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@INPUT:bColdRst = TRUE
//@EXPECTATION:接口返回CSUDISMC_ERROR_FAILURE
//@EXECUTIONFLOW:1、从配置文件中查询测试卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen，回调函数是预先定义好的回调函数
//@EXECUTIONFLOW:5、判断CSUDISMCOpen的返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDISMCReset,判断返回值是否等于CSUDISMC_ERROR_FAILURE
BOOL CSTC_SMARTCARD_IT_Reset_0060(void)
{
	int nInputCardSlotNum=0;
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	CSUDISMCParam_S sCardParamSet;	
	int nInputCardNum;
	BOOL bOpened = CSUDI_FALSE;

	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0060]测试卡槽中没有插入智能卡时执行CSUDISMCReset start\n");
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0060]请将测试卡从测试卡槽中拔出，按任意键继续\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," 步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤5失败");
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_FALSE) ,"步骤6失败");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}
	CSTCPrint("请将测试卡正确插入测试卡槽\n");
	CSTKWaitAnyKey();
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0060]测试卡槽中没有插入智能卡时执行CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:测试反插卡时复位失败的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucAtr指针不为空
//@PRECONDITION:3.pnAtrLength指针不为空
//@PRECONDITION:4.卡槽中反插入智能卡
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = 用户输入的索引号;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@INPUT:bColdRst = TRUE
//@EXPECTATION:接口返回CSUDISMC_ERROR_ERROR_ANSWER；
//@EXECUTIONFLOW:1、从配置文件中查询测试卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen，回调函数是预先定义好的回调函数
//@EXECUTIONFLOW:5、判断CSUDISMCOpen的返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDISMCReset,判断CSUDISMCReset的返回值是否等于CSUDISMC_ERROR_MUTE
BOOL CSTC_SMARTCARD_IT_Reset_0061(void)
{	
	int nInputCardSlotNum=0;
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	BOOL bOpened = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0061]测试卡槽中反插卡时执行CSUDISMCReset start\n");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0061]请将测试卡反插入测试卡槽(无磁条接触)，按任意键继续\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," 步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤5失败");
	bOpened = CSUDI_TRUE;	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_MUTE == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤6失败");
	
	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}	
	CSTCPrint("请将测试卡正确插入测试卡槽\n");
	CSTKWaitAnyKey();

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0061]测试卡槽中反插卡时执行CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:测试设置错误的频率时复位失败的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucAtr指针不为空
//@PRECONDITION:3.pnAtrLength指针不为空
//@PRECONDITION:4.卡槽中插入智能卡
//@PRECONDITION:5.为卡槽设置错误的频率
//@PRECONDITION:6.卡槽之前被成功打开
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = 用户输入的索引号;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@INPUT:bColdRst = TRUE
//@EXPECTATION:接口返回CSUDISMC_ERROR_ERROR_ANSWER；
//@EXECUTIONFLOW:1、从配置文件中查询测试卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡索引号
//@EXECUTIONFLOW:3、根据测试卡索引号获得卡支持的频率范围(保存在全局的数组中)
//@EXECUTIONFLOW:4、调用CSUDISMCSetParams，传入不在测试卡支持范围内的频率参数
//@EXECUTIONFLOW:5、调用CSUDISMCOpen
//@EXECUTIONFLOW:6、调用CSUDISMCReset
//@EXECUTIONFLOW:7、判断CSUDISMCReset的返回值是否等于CSUDISMC_ERROR_FAILURE
//@EXECUTIONFLOW:8、CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Reset_0062(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	BOOL bOpened = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0062]测试设置错误的频率时复位失败 start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol ;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = 20000000;//g_sCardPara[nInputCardNum].m_nFrequency + 1000;//0-5M都会成功

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤5失败");

	bOpened = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤7失败");
	
	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0062]测试设置错误的频率时复位失败 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:测试成功复位的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucAtr指针不为空
//@PRECONDITION:3.pnAtrLength指针不为空
//@PRECONDITION:4.卡槽中插入智能卡
//@PRECONDITION:5.为卡槽设置正确的参数
//@PRECONDITION:6.卡槽之前被成功打开
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = 用户输入的索引号;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@EXPECTATION:接口返回CSUDI_SUCCESS；
//@EXPECTATION:Atr的第一个字节是0x3b或0x3f；
//@EXPECTATION:返回的Atr长度与实际返回的Atr数据长度相同；
//@EXPECTATION:使用示波器测量复位波形、CLK、VCC、RST引脚电平符合ISO7816_123标准；
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用CSUDISMCReset,传入参数bColdRst = TRUE
//@EXECUTIONFLOW:6、判断CSUDISMCReset的返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:7、判断Atr的第一个字节是否是0x3b或0x3f；
//@EXECUTIONFLOW:8、根据用户输入的协议计算实际返回的Atr数据的长度，判断与返回的pnAtrLength是否相同
//@EXECUTIONFLOW:9、提示用户使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123冷复位标准
//@EXECUTIONFLOW:10、调用CSUDISMCReset,传入参数bColdRst = FALSE
//@EXECUTIONFLOW:11、判断CSUDISMCReset的返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:12、判断Atr的第一个字节是否是0x3b或0x3f；
//@EXECUTIONFLOW:13、根据用户输入的协议计算实际返回的Atr数据的长度，判断与返回的pnAtrLength是否相同
//@EXECUTIONFLOW:14、使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123冷复位标准
//@EXECUTIONFLOW:15、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Reset_0063(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	unsigned int dwRLen;
	unsigned char  ucTransfT;
	BOOL bOpened = CSUDI_FALSE;

	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]测试成功复位 start\n");	
		
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");			
	
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]请将示波器探头连接好智能卡CLK、VCC、RST引脚，准备抓取各引脚波形(按任意键继续):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"步骤8计算实际Atr长度失败");
		
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"步骤8失败,实际长度与计算长度不相等");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]请使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123冷复位标准(是按Y,否按N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]请将示波器探头连接好智能卡CLK、VCC、RST引脚，准备再次抓取各引脚波形(按任意键继续):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_FALSE),"步骤11失败");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"步骤12失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"步骤13计算实际Atr长度失败");
	
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"步骤13失败,实际长度与计算长度不相等");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]请使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123热复位标准(是按Y,否按N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤14失败");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]测试成功复位 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:测试成功热复位后再次热复位的情况
//@PRECONDITION:1.卡槽索引号有效
//@PRECONDITION:2.pucAtr指针不为空
//@PRECONDITION:3.pnAtrLength指针不为空
//@PRECONDITION:4.卡槽中插入智能卡
//@PRECONDITION:5.为卡槽设置正确的参数
//@PRECONDITION:6.卡槽之前被成功打开
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = 用户输入的索引号;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@EXPECTATION:再次热复位接口返回CSUDI_SUCCESS；
//@EXPECTATION:再次热复位后Atr的第一个字节是0x3b或0x3f；
//@EXPECTATION:再次热复位后返回的Atr长度与实际返回的Atr数据长度相同；
//@EXPECTATION:再次热复位后使用示波器测量复位波形、CLK、VCC、RST引脚电平符合ISO7816_123标准；
//@EXECUTIONFLOW:1、从配置文件查询测试卡槽索引号
//@EXECUTIONFLOW:2、从配置文件查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用CSUDISMCReset,传入参数bColdRst = TRUE
//@EXECUTIONFLOW:6、判断CSUDISMCReset的返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:7、判断Atr的第一个字节是否是0x3b或0x3f；
//@EXECUTIONFLOW:8、根据测试卡协议计算实际返回的Atr数据的长度，判断与返回的pnAtrLength是否相同
//@EXECUTIONFLOW:9、使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123冷复位标准；
//@EXECUTIONFLOW:10、再次调用CSUDISMCReset，传入参数bColdRst = FALSE
//@EXECUTIONFLOW:11、判断CSUDISMCReset的返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:12、判断Atr的第一个字节是否是0x3b或0x3f；
//@EXECUTIONFLOW:13、根据测试卡协议计算实际返回的Atr数据的长度，判断与返回的pnAtrLength是否相同
//@EXECUTIONFLOW:14、使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123热复位标准
//@EXECUTIONFLOW:15、再次调用CSUDISMCReset，传入参数bColdRst = FALSE
//@EXECUTIONFLOW:16、判断CSUDISMCReset的返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:17、判断Atr的第一个字节是否是0x3b或0x3f；
//@EXECUTIONFLOW:18、根据测试卡协议计算实际返回的Atr数据的长度，判断与返回的pnAtrLength是否相同
//@EXECUTIONFLOW:19、使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123热复位标准
//@EXECUTIONFLOW:20、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Reset_0064(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	unsigned int dwRLen;
	unsigned char  ucTransfT;
	BOOL bOpened = CSUDI_FALSE;

	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]测试成功热复位后再次热复位 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]请将示波器探头连接好智能卡CLK、VCC、RST引脚，准备抓取各引脚波形(按任意键继续):\n");

	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"步骤7失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"步骤8计算实际Atr长度失败");
		
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"步骤8失败,实际长度与计算长度不相等");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]请使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123冷复位标准(是按Y,否按N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]请将示波器探头连接好智能卡CLK、VCC、RST引脚，准备再次抓取各引脚波形(按任意键继续):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_FALSE),"步骤11失败");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"步骤12失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"步骤13计算实际Atr长度失败");
		
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"步骤13失败,实际长度与计算长度不相等");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]请使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123热复位标准(是按Y,否按N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤14失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]请将示波器探头连接好智能卡CLK、VCC、RST引脚，准备再次抓取各引脚波形(按任意键继续):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_FALSE),"步骤16失败");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"步骤17失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"步骤18计算实际Atr长度失败");
		
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"步骤18失败,实际长度与计算长度不相等");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]请使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123热复位标准(是按Y,否按N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤19失败");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]测试成功热复位后再次热复位 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试卡槽中没有插入智能卡和卡槽没有复位的情况(不针对DVN CA)
//@PRECONDITION:1.接口各参数有效
//@PRECONDITION:2.卡槽中没有插入智能卡
//@PRECONDITION:3.卡槽之前没有被成功reset
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned char auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:接口分别返回CSUDISMC_ERROR_OUT和CSUDISMC_ERROR_NOT_RESET
//@EXECUTIONFLOW:1、从配置文件查询测试卡槽索引号
//@EXECUTIONFLOW:2、从配置文件查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用接口CSUDISMCDataExchange，判断返回值是否等于CSUDISMC_ERROR_OUT
//@EXECUTIONFLOW:6、提示并等待用户插入智能卡
//@EXECUTIONFLOW:7、调用接口CSUDISMCDataExchange，判断返回值是否等于CSUDISMC_ERROR_NOT_RESET
//@EXECUTIONFLOW:8、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Exchange_0065(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	int nNumRead;
	unsigned char auStatus[2];	
	BOOL bOpened = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	BOOL bRemoved = CSUDI_FALSE;

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0065]测试卡槽中没有插入智能卡和卡槽没有复位时调用CSUDISMCDataExchange start\n");
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0065]请将测试卡从测试卡槽拔出，按任意键继续\n");
	CSTKWaitAnyKey();
	bRemoved = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "步骤1失败");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "步骤2失败");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "该测试用例不针对DVN CA");

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack), "步骤4失败");
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_OUT == CSUDISMCDataExchange(nInputCardSlotNum, aucDataToWrite, 5, aucResData, &nNumRead, auStatus), "步骤5失败");
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0065]请将测试卡插入测试卡槽(按任意键继续)\n");
	CSTKWaitAnyKey();
	bRemoved = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_NOT_RESET == CSUDISMCDataExchange(nInputCardSlotNum, aucDataToWrite, 5, aucResData, &nNumRead, auStatus), "步骤7失败");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "销毁资源失败");
	}

	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("请将测试卡正确插入测试卡槽\n");
		CSTKWaitAnyKey();
	}
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0065]测试卡槽中没有插入智能卡和卡槽没有复位时调用CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试成功交互命令的情况(不针对DVN CA)
//@PRECONDITION:1.接口各参数有效
//@PRECONDITION:2.卡槽之前成功reset
//@PRECONDITION:3.交互使用的命令与测试的卡相匹配
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = 与卡匹配的命令;
//@INPUT:nNumberToWrite = 由实际命令长度决定;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:接口返回CSUDI_SUCCESS,根据协议计算命令回复信息正确
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、判断是否DVN CA,若是测试结束，否则继续。调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用CSUDISMCReset
//@EXECUTIONFLOW:6、根据测试卡的索引号，获得与卡匹配的一组用于测试交互的命令(保存在全局数组中)，对该组命令调用CSUDISMCDataExchange
//@EXECUTIONFLOW:7、判断每次CSUDISMCDataExchange返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:8、判断根据协议计算每次命令回复的信息是否正确
//@EXECUTIONFLOW:9、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Exchange_0066(void)
{
 	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucResData[512];
	int nNumRead;
	unsigned char auStatus[2];	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;		
	int nSendLen;
	BOOL bOpened = CSUDI_FALSE;
	int i;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0066]测试成功交互命令 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "该测试用例不针对DVN CA");

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;	
 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤5失败");
	
    if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
    {
		int nIndex;
		sInfoLinkSMC *psCurLink = psRootNode;
		unsigned char auStatus[2];
		int nNumRead = 0;
		
		for(nIndex=0; nIndex<= psLastNode->m_num; nIndex++)
		{	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, \
				psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
				&nNumRead, auStatus),"步骤7失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
				psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
				psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
				s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"步骤8失败");

			psCurLink = psCurLink->next;	
		}			
    }
	else
	{
		for(i=0; i<MAX_TEST_CMD_NUM; i++)
		{
			memset(aucResData, 0, sizeof(aucResData));

			nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[i*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[i*2][1];

			if(nInputCardNum == EM_TEST_SUANTONG)
			{
				 unsigned char command[512];					 
			
				 memcpy(command,&(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]),nSendLen);
 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤6失败:suantong datafix failure");
					 //nSendLen += 4;
				
				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"步骤7失败");			 

				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7.5失败:suantong datafix failure");
				 nNumRead +=4;
			}
			else if(nInputCardNum == EM_TEST_COMVENIENT)
			{
                 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤7失败");
                 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7.5失败:COMVENIENT datafix failure");
				 nNumRead +=4;
			}
			else
			{	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤7失败");
			}
					
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,  &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead,  auStatus),"步骤8失败");
		}
	}
	
	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	
	if(CSUDI_TRUE == bOpened)
	{	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");			
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0066]测试成功交互命令  end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:性能测试(测试交互命令的时间)(不针对DVN CA)
//@PRECONDITION:1.接口各参数有效
//@PRECONDITION:2.卡槽之前成功reset
//@PRECONDITION:3.使用预先定义的各个卡用于性能测试的命令
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = 与卡匹配的命令;
//@INPUT:nNumberToWrite = 由实际命令长度决定;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:接口返回CSUDI_SUCCESS,命令回复信息的内容和长度与命令预期相同，交互使用的时间不大于预期时间
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、判断是否DVN CA,若是测试结束，否则继续。调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用CSUDISMCReset
//@EXECUTIONFLOW:6、根据测试卡的索引号，获得与卡匹配的一条用于性能测试的命令及预期交互时间(保存在全局数组中)
//@EXECUTIONFLOW:7、调用CSUDIPlusOSGetTickCount，查询当前时间
//@EXECUTIONFLOW:8、调用CSUDISMCDataExchange
//@EXECUTIONFLOW:9、调用CSUDIPlusOSGetTickCount，查询当前时间,比较前后时间，得到交互使用的时间
//@EXECUTIONFLOW:10、判断返回值CSUDISMCDataExchange是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:11、判断根据协议计算命令回复的信息是否正确
//@EXECUTIONFLOW:12、判断命令交互使用的时间是否小于预期交互时间，若不小于则测试失败，打印出预期和实际交互时间
//@EXECUTIONFLOW:13、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Exchange_0067(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucResData[512];
	int nNumRead;
	unsigned char auStatus[2];	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;		
	unsigned int nTickBeforeExchange;
	unsigned int nTickAfterExchange;
	unsigned int nTickCost;
	int nSendLen;
	BOOL bOpened = CSUDI_FALSE;
	BOOL bSptFile = CSUDI_FALSE;
	int i;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0067]性能测试(测试交互命令的时间) start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "该测试用例不针对DVN CA");

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;	

	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤5失败");

	nTickBeforeExchange = CSUDIPlusOSGetTickCount();

	if(bSptFile)
	{
		sInfoLinkSMC *psCurLink = psLastNode;
		{
			for(i=0; i<MAX_TEST_CMD_NUM; i++)
			{	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, 
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
					&nNumRead, auStatus),"步骤8命令交互失败");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"步骤14命令返回值失败");
			}			
		}
	}
	else	
	{
		for(i=0; i<MAX_TEST_CMD_NUM; i++)
		{
			memset(aucResData, 0, sizeof(aucResData));

			nSendLen = g_TestCapCmd[nInputCardNum][0]+g_TestCapCmd[nInputCardNum][1];

			if(nInputCardNum == EM_TEST_SUANTONG)
			{
				 unsigned char command[512];

				 memcpy(command,&g_TestCapCmd[nInputCardNum][2],nSendLen);
					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤6失败:suantong datafix failure");
					 //nSendLen += 4;

				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"步骤10失败");			 

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤6.5失败:suantong datafix failure");
				 nNumRead +=4;

			}
			else if(nInputCardNum == EM_TEST_COMVENIENT)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"步骤6命令交互失败");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤6.5失败:COMVENIENT datafix failure");
				nNumRead +=4;
			}				
			else
			{			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"步骤6失败");
			}				

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead,  auStatus),"步骤7失败");
		}
	}
	nTickAfterExchange = CSUDIPlusOSGetTickCount();

	nTickCost =(nTickAfterExchange - nTickBeforeExchange)/MAX_TEST_CMD_NUM;

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0067]交互命令使用的时间是:%d ms\n", nTickCost);
	
	CSTK_ASSERT_TRUE_FATAL(nTickCost <= MAX_TEST_EXCHANGE_TIME, "步骤12失败");

	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0067]性能测试(测试交互命令的时间) end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:老化测试1个小时数据交互的情况以及老化测试后性能测试(不针对DVN CA)
//@PRECONDITION:1.接口各参数有效
//@PRECONDITION:2.卡槽之前成功reset
//@PRECONDITION:3.交互使用的命令与测试的卡相匹配
//@PRECONDITION:4.连接好PC与机顶盒串口
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = 与卡匹配的命令;
//@INPUT:nNumberToWrite = 由实际命令长度决定;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:1.每次交互接口返回CSUDI_SUCCESS,命令回复信息的内容和长度与命令预期相同，老化前后交互同一条命令的时间相等
//@EXPECTATION:2.测试完之后三次拔插卡在串口打印中能够看到回调函数检测到卡Out和卡In时的打印信息
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、判断是否DVN CA,若是测试结束，否则继续。调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用CSUDISMCReset
//@EXECUTIONFLOW:6、根据用户输入的测试卡类型，获得与卡匹配的用于性能测试的命令(一条，保存在全局数组中)
//@EXECUTIONFLOW:7、调用CSUDIPlusOSGetTickCount，查询当前时间
//@EXECUTIONFLOW:8、四次调用CSUDISMCDataExchange
//@EXECUTIONFLOW:9、调用CSUDIPlusOSGetTickCount，查询当前时间,比较前后时间，求交互一次平均值，得到老化测试之前交互命令使用的时间
//@EXECUTIONFLOW:10、根据测试卡的索引号，获得与卡匹配的一组用于交互测试的命令(保存在全局数组中)，在1个小时之内循环调用CSUDISMCDataExchange
//@EXECUTIONFLOW:11、判断每次调用CSUDISMCDataExchange的返回值是否等于CSUDI_SUCCESS，若不相同则退出循环，测试失败
//@EXECUTIONFLOW:12、根据协议计算每次调用CSUDISMCDataExchange的命令回复信息是否正确，若不正确则退出循环，测试失败
//@EXECUTIONFLOW:13、调用CSUDIPlusOSGetTickCount，查询当前时间
//@EXECUTIONFLOW:14、四次调用CSUDISMCDataExchange，交互同一条用于性能测试的命令
//@EXECUTIONFLOW:15、调用CSUDIPlusOSGetTickCount，查询当前时间,比较前后时间，求交互一次平均值，得到老化测试之后交互命令使用的时间
//@EXECUTIONFLOW:16、打印出两次的时间，提示用户判断性能是否下降，根据用户判断决定是否成功
//@EXECUTIONFLOW:17、提示用户拔插卡三次，每次是否有相应的回调函数消息，若有，则成功，否则失败
//@EXECUTIONFLOW:18、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Exchange_0068(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucResData[512];
	unsigned char auStatus[2];	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;
	int nNumRead = 0;
	int nNumToRead;
	int nNumWrite;
	int nTimeOut = 1000;		
	int nTickBeforeExchange;
	int nTickAfterExchange;
	int nTickCostBefore = 0;
	int nTickCostAfter;
	unsigned int dwStartTime;
	unsigned int dwTestTime = 0;
	int i=0,j,m;
	int nSendLen;
	BOOL bOpened = CSUDI_FALSE;
	BOOL bSptFile = CSUDI_FALSE;

	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]老化测试1个小时数据交互的情况以及老化测试后性能测试 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	

	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "该测试用例不针对DVN CA");
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;	

	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}

	CSTCGetConfigAtr();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤5失败");

	if(nInputCardNum != EM_TEST_DVN)
	{
		nTickBeforeExchange = CSUDIPlusOSGetTickCount();
		if(bSptFile)
		{
			sInfoLinkSMC *psCurLink = psLastNode;
			for(i=0; i<MAX_TEST_CMD_NUM; i++)
			{	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, 
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
					&nNumRead, auStatus),"步骤6命令交互失败");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"步骤7命令返回值失败");
			}			
		}
		else
		{
			for(i=0; i<MAX_TEST_CMD_NUM; i++)
			{
				memset(aucResData, 0, sizeof(aucResData));

				nSendLen = g_TestCapCmd[nInputCardNum][0]+g_TestCapCmd[nInputCardNum][1];				
				if(nInputCardNum == EM_TEST_SUANTONG)
				{
					 unsigned char command[512];

					 memcpy(command,&g_TestCapCmd[nInputCardNum][2],nSendLen);
					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤6失败:suantong datafix failure");
					 //nSendLen += 4;

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"步骤6命令交互失败");
					 
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7.5失败:suantong datafix failure");
					 nNumRead +=4;
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"步骤6命令返回值失败");
				}
				else if(nInputCardNum == EM_TEST_CONAX || nInputCardNum == EM_TEST_VIACCESS || nInputCardNum == EM_TEST_CryptoWorks)
				{
				    nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[i*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[i*2][1];
				    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤6命令交互失败");
				    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead, auStatus),"步骤6命令返回值失败");
				}
				else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"步骤6命令交互失败");
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤6.5失败:COMVENIENT datafix failure");
					nNumRead +=4;
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"步骤6命令返回值失败");				    
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"步骤6命令交互失败");					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"步骤6命令返回值失败");
				}
			}				
		}
		nTickAfterExchange = CSUDIPlusOSGetTickCount();
	
		nTickCostBefore =(nTickAfterExchange - nTickBeforeExchange)/MAX_TEST_CMD_NUM;
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]老化测试1个小时数据交互，请等待!\n");
	dwStartTime = CSUDIPlusOSGetTickCount();
	
	while(dwTestTime <= ONE_HOUR_MILLISECONDS)
	{
		if(bSptFile)
	    {
			int nIndex;
			sInfoLinkSMC *psCurLink = psRootNode;
			unsigned char auStatus[2];
			int nNumRead = 0;

			if(nInputCardNum != EM_TEST_DVN)
			{
				for(nIndex=0; nIndex<= psLastNode->m_num; nIndex++)
				{	
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, \
						psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
						&nNumRead, auStatus),"步骤11数据交互失败");
					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
						psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
						psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
						s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"步骤11数据交互返回值失败");

					psCurLink = psCurLink->next;	
				}
			}
			else
			{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
						psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
						&nNumRead,nTimeOut),"步骤11第一次发送数据失败");				
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
						s_ReData, 1,&nNumRead,nTimeOut),\
						"步骤11第一次接收数据返回值失败");
					CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
						(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"步骤11第一次接收数据失败");
					psCurLink = psCurLink->next;	
						
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
						psCurLink->m_node->m_send.m_data, 5,\
						&nNumRead,nTimeOut),"步骤11第二次发送数据失败");					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
						s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
						"步骤11第二次接收数据返回值失败");
					CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"步骤11第二次接收数据失败");
					psCurLink = psCurLink->next;	

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
						s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
						"步骤11第三次接收数据返回值失败");
					CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"步骤11第三次接收数据失败");
					psCurLink = psCurLink->next;
					
					/*接收状态字*/
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
						s_ReData, 2,&nNumRead,nTimeOut),\
						"步骤11第四次接收数据返回值失败");
					CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"步骤11第四次接收数据失败");					
	    	}
		}
		else
		{
			for(j=0; j<MAX_TEST_CMD_NUM; j++)
			{
				memset(aucResData, 0, sizeof(aucResData));

				if(nInputCardNum != EM_TEST_DVN)
				{	
					nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[j*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[j*2][1];
					if(nInputCardNum == EM_TEST_SUANTONG)
					{
						 unsigned char command[512];					 
					
						 memcpy(command,&(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]),nSendLen);
	 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤11失败:suantong datafix failure");
						 //nSendLen += 4;
						
						 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"步骤11数据交互失败");

						 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤11.2失败:suantong datafix failure");
					    nNumRead +=4;
					}
					else if(nInputCardNum == EM_TEST_COMVENIENT)
					{
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤7失败");
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7.5失败:COMVENIENT datafix failure");
					 nNumRead +=4;					    
					}
					else
					{			
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤11数据交互失败");
					}


			 		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, nNumRead, auStatus),"步骤11数据交互返回值失败");
				}
				else/*DVN*/
				{
					nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[0][0] + g_DVNExchangeCmd[j].aaDVNCommand[0][1];
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"步骤11第一次发送数据失败");
					/*接收1个字节*/
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"步骤11第一次接收数据返回值失败");

					CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[j].aaDVNCommand[0][3]),"步骤11第一次接收数据失败");

					nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[1][0] + g_DVNExchangeCmd[j].aaDVNCommand[1][1];

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"步骤11第二次发送数据失败");
					/*接收5个字节*/
					memset(aucResData, 0, sizeof(aucResData));
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"步骤第二次接收数据返回值失败");

					CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"步骤11第二次接收数据失败");

					nNumToRead = aucResData[4];
					memset(aucResData, 0, sizeof(aucResData));
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"步骤第三次接收数据返回值失败");

					CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"步骤11第三次接收数据失败");
					/*接收状态字*/
					memset(aucResData, 0, sizeof(aucResData));
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"步骤11第四次接收数据返回值失败");

					CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"步骤11第四次接收数据失败");				
				}
			}	
		}
		dwTestTime = CSUDIPlusOSGetTickCount() - dwStartTime;
	}
	
	if(nInputCardNum != EM_TEST_DVN)
	{	
		nTickBeforeExchange = CSUDIPlusOSGetTickCount();
		if(bSptFile)
		{
			sInfoLinkSMC *psCurLink = psLastNode;
			for(i=0; i<MAX_TEST_CMD_NUM; i++)
			{	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, 
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
					&nNumRead, auStatus),"步骤14命令交互失败");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"步骤14命令返回值失败");
			}			
		}
		else
		{
			for(m=0; m<MAX_TEST_CMD_NUM; m++)
			{
				memset(aucResData, 0, sizeof(aucResData));

				nSendLen = g_TestCapCmd[nInputCardNum][0]+g_TestCapCmd[nInputCardNum][1];

				if(nInputCardNum == EM_TEST_SUANTONG)
				{
					 unsigned char command[512];

					 memcpy(command,&g_TestCapCmd[nInputCardNum][2],nSendLen);
					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤14失败:suantong datafix failure");
					 //nSendLen += 4;

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"步骤14命令交互失败");	

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤14.2失败:suantong datafix failure");
					 nNumRead +=4;
					 
	 				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"步骤14命令返回值失败");
				}
				else if(nInputCardNum == EM_TEST_CONAX || nInputCardNum == EM_TEST_VIACCESS || nInputCardNum == EM_TEST_CryptoWorks)
				{
				    nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[m*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[m*2][1];
				    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[m*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤14命令交互失败");
				    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[m*2][2]), nSendLen, aucResData, nNumRead, auStatus),"步骤14命令返回值失败");
				}
				else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"步骤6命令交互失败");
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤6.5失败:COMVENIENT datafix failure");
					nNumRead +=4;
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"步骤6命令返回值失败");				    
				}				
				else
				{			
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"步骤14命令交互失败");
    				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"步骤14命令返回值失败");
				}
			}
		}
		nTickAfterExchange = CSUDIPlusOSGetTickCount();

		nTickCostAfter =(nTickAfterExchange - nTickBeforeExchange)/MAX_TEST_CMD_NUM;
		
		CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]老化前后交互命令使用的时间分别是%d ms,%d ms\n",nTickCostBefore,nTickCostAfter);

		CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]请判断老化后测试性能是否下降(没有下降按Y,下降按N)\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤16失败");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]请插拔智能卡三次(分别插拔一下算一次)，并查看在串口打印中是否能够看到回调函数检测到卡In和卡out时输出的信息(是按Y,否按N)\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤17失败");
	
	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]老化测试1个小时数据交互的情况以及老化测试后性能测试 end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:测试卡槽中没有插入智能卡和卡槽没有复位的情况(只针对DVN CA)
//@PRECONDITION:1.各参数有效
//@PRECONDITION:2.卡槽中没有插入智能卡
//@PRECONDITION:3.卡槽之前没有被成功reset
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:int nNumWrite;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pnNumberWrite = &nNumWrite;
//@INPUT:nTimeout = 0; 
//@EXPECTATION:没插卡时接口返回CSUDISMC_ERROR_OUT，插卡后接口返回CSUDISMC_ERROR_NOT_RESET
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数值
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用接口CSUDISMCSend
//@EXECUTIONFLOW:6、判断第五步返回值是否等于CSUDISMC_ERROR_OUT
//@EXECUTIONFLOW:7、提示用户插入智能卡
//@EXECUTIONFLOW:8、调用接口CSUDISMCSend
//@EXECUTIONFLOW:9、判断第八步返回值是否等于CSUDISMC_ERROR_NOT_RESET
//@EXECUTIONFLOW:10、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Send_0069(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucDataToWrite[5]={0};
	int nNumWrite;
	int nTimeOut = 1000;
	BOOL bOpened = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	BOOL bRemoved = CSUDI_FALSE;
		
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0069]测试卡槽中没有插入智能卡和卡槽没有复位时调用CSUDISMCSend(读写分离测试:适用于天柏CA) start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤2失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);		
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum == EM_TEST_DVN, "该测试用例只针对DVN CA");

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0069]请将测试卡从测试卡槽拔出，按任意键继续\n");
	CSTKWaitAnyKey();
	bRemoved = CSUDI_TRUE;

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSend(nInputCardSlotNum, aucDataToWrite, 5, &nNumWrite, nTimeOut),"步骤5失败");
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0069]请将测试卡插入测试卡槽(按任意键继续)\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSend(nInputCardSlotNum, aucDataToWrite, 5, &nNumWrite, nTimeOut),"步骤9失败");	
	bRemoved = CSUDI_FALSE;

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}
	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("请将测试卡正确插入测试卡槽\n");
		CSTKWaitAnyKey();
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0069]测试卡槽中没有插入智能卡和卡槽没有复位时调用CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:测试发送命令超时的情况(只针对DVN CA)
//@PRECONDITION:1.各参数有效
//@PRECONDITION:2.卡槽之前成功reset
//@INPUT:unsigned char aucDataToWrite[]={与卡匹配的命令};
//@INPUT:int nNumWrite;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = 由实际的命令决定;
//@INPUT:pnNumberWrite = &nNumWrite;
//@INPUT:nTimeout的值等于10ms; 
//@EXPECTATION:接口返回CSUDISMC_ERROR_TIMEOUT
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用CSUDISMCReset
//@EXECUTIONFLOW:6、根据用测试卡的索引号，获得与卡匹配的用于性能测试的命令(100Bytes左右，保存在全局数组中)，调用CSUDISMCSend，传入参数nTimeout的值为20ms
//@EXECUTIONFLOW:7、判断CSUDISMCSend的返回值是否是CSUDISMC_ERROR_TIMEOUT
//@EXECUTIONFLOW:8、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_Send_0070(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	int nNumWrite;
	int nTimeOut = 10;
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;
	int nSendLen;
	BOOL bOpened = CSUDI_FALSE;
	BOOL bSptFile =CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0070]测试发送命令超时(读写分离测试:适用于天柏CA) start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤2失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum == EM_TEST_DVN, "该测试用例只针对DVN CA");

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;	
	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤5失败");

	if(bSptFile)
	{
		sInfoLinkSMC *psCurLink = psLastNode;
		if(psCurLink->m_num == 4)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSend(nInputCardSlotNum,\
				psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, &nNumWrite, nTimeOut),\
				"步骤7失败");	
		}
	}
	else
	{
		nSendLen = g_TestCapCmd[nInputCardNum][0]+g_TestCapCmd[nInputCardNum][1];

		CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSend(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, &nNumWrite, nTimeOut),"步骤7失败");
	}
	
	CSTK_FATAL_POINT;
	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");	
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0070]测试发送命令超时 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend、CSUDISMCReceive
//@DESCRIPTION:测试成功发送、接收命令的情况(只针对DVN CA)
//@PRECONDITION:1.各参数有效
//@PRECONDITION:2.卡槽之前成功reset
//@INPUT:unsigned char aucDataToWrite[]={与卡匹配的命令};
//@INPUT:int nNumWrite;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = 由实际的命令决定;
//@INPUT:pnNumberWrite = &nNumWrite;
//@INPUT:nTimeout = 1000;  
//@EXPECTATION:接口返回CSUDI_SUCCESS，之后调用CSUDISMCReceive接口，根据协议计算命令回复的信息正确
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、判断是否DVN CA,若不是测试结束，否则继续。调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用CSUDISMCReset
//@EXECUTIONFLOW:6、调用CSUDISMCSend
//@EXECUTIONFLOW:7、判断CSUDISMCSend的返回值是否是CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDISMCReceive接口，根据协议计算命令回复的信息是否正确
//@EXECUTIONFLOW:9、调用CSUDISMCClose
BOOL CSTC_SMARTCARD_IT_SendReceice_0071(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucResData[512];
	int nNumRead,nNumWrite;
	int nNumToRead;
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	int nTimeOut = 1000;
	int nSendLen;
	int i;
	BOOL bOpened = CSUDI_FALSE;
	BOOL bSptFile =CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;

	CSTCPrint("[CSTC_SMARTCARD_IT_SendReceice_0071]测试成功发送、接收命令(读写分离测试:适用于天柏CA) start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤2失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);		
 	CSTK_ASSERT_TRUE_FATAL(nInputCardNum == EM_TEST_DVN, "该测试用例只针对DVN CA");

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;	
	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤5失败");

	CSAtrDecode(aucAtr,nAtrLen);
	
	if(bSptFile)
	{
		sInfoLinkSMC *psCurLink = psRootNode;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
				psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
				&nNumRead,nTimeOut),"步骤8第一次发送数据失败");				
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
				s_ReData, 1,&nNumRead,nTimeOut),\
				"步骤8第一次接收数据返回值失败");
			CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
				(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"步骤8第一次接收数据失败");
			psCurLink = psCurLink->next;	
				
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
				psCurLink->m_node->m_send.m_data, 5,\
				&nNumRead,nTimeOut),"步骤8第二次发送数据失败");					
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
				s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
				"步骤8第二次接收数据返回值失败");
			CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"步骤8第二次接收数据失败");
			psCurLink = psCurLink->next;	

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
				s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
				"步骤8第三次接收数据返回值失败");
			CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"步骤8第三次接收数据失败");
			psCurLink = psCurLink->next;
			
			/*接收状态字*/
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
				s_ReData, 2,&nNumRead,nTimeOut),\
				"步骤8第四次接收数据返回值失败");
			CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"步骤8第四次接收数据失败");	
	}
	else
	{
		for(i=0; i<MAX_TEST_CMD_NUM; i++)
		{
			memset(aucResData, 0, sizeof(aucResData));

			nSendLen = g_DVNExchangeCmd[i].aaDVNCommand[0][0] + g_DVNExchangeCmd[i].aaDVNCommand[0][1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[i].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"步骤7第一次发送数据失败");
			/*接收1个字节*/
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"步骤8第一次接收数据返回值失败");

			CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[i].aaDVNCommand[0][3]),"步骤8第一次接收数据失败");

			nSendLen = g_DVNExchangeCmd[i].aaDVNCommand[1][0] + g_DVNExchangeCmd[i].aaDVNCommand[1][1];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[i].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"步骤7第二次发送数据失败");
			/*接收5个字节*/
			memset(aucResData, 0, sizeof(aucResData));
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"步骤8第二次接收数据返回值失败");

			CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"步骤8第二次接收数据失败");

			nNumToRead = aucResData[4];
			memset(aucResData, 0, sizeof(aucResData));
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"步骤8第三次接收数据返回值失败");

			CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"步骤8第三次接收数据失败");
			/*接收状态字*/
			memset(aucResData, 0, sizeof(aucResData));
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"步骤8第四次接收数据返回值失败");

			CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"步骤8第四次接收数据失败");		
		}	
	}

	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");	
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SendReceice_0071]测试成功发送、接收命令 end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:测试Atr的TA1存在时，传入的FI/DI的值大于TA1的情况
//@PRECONDITION:1.卡槽复位成功
//@INPUT:unsigned char aucDataToWrite[4]={根据TA1的值决定};
//@INPUT:unsigned char aucResData[4];
//@INPUT:int nNumRead;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWriteLen = 5;
//@INPUT:pucResponseData = aucResData;
//@INPUT:pnResponseDataLen = &nNumRead;
//@EXPECTATION:接口返回CSUDISMC_ERROR_OTHER
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen打开卡槽；
//@EXECUTIONFLOW:5、调用CSUDISMCReset复位，判断TA1是否存在，如果不存在返回测试成功
//@EXECUTIONFLOW:6、如果TA1存在，则调用CSUDISMCSetPPS，传入TA1+1的值作为的FI/DI值
//@EXECUTIONFLOW:7、判断CSUDISMCSetPPS的返回值是否是CSUDISMC_ERROR_FAILURE，是则测试成功，不是则测试失败
//@EXECUTIONFLOW:8、调用CSUDISMCClose关闭卡槽；
BOOL CSTC_SMARTCARD_IT_SetPPS_0072(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char ucWriteData[5] = {0};
	int nNumRead;
	unsigned char auResData[5] = {0};	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	int nNumToWrite;
	BOOL bOpened = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0072]测试Atr的TA1存在时，调用CSUDISMCSetPPS，传入的FI/DI的值大于TA1 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);	
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack), "步骤4失败");
	
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "步骤5失败");

	if((aucAtr[1]& 0x10) != 0)
	{
		ucWriteData[0] = 0x10;
		ucWriteData[1] = aucAtr[2] + 0x01;
		nNumToWrite = 2;
		CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSetPPS(nInputCardSlotNum, ucWriteData, nNumToWrite, auResData, &nNumRead), "步骤7失败");
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "销毁资源失败");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0072]测试成功发送、接收命令 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:测试PPS交互成功的情况
//@PRECONDITION:1.各参数有效
//@PRECONDITION:2.卡槽成功reset
//@INPUT:unsigned char aucDataToWrite[4]={0x10,配置文件中的起始FI/DI值};
//@INPUT:unsigned char aucResData[4];
//@INPUT:int nNumRead;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWriteLen = 5;
//@INPUT:pucResponseData = aucResData;
//@INPUT:pnResponseDataLen = &nNumRead;
//@EXPECTATION:接口返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、从配置文件中查询PPS交互的起始FI/DI的值
//@EXECUTIONFLOW:4、调用CSUDISMCSetParams为卡槽设置正确的参数；
//@EXECUTIONFLOW:5、调用CSUDISMCOpen打开卡槽；
//@EXECUTIONFLOW:6、调用CSUDISMCReset复位
//@EXECUTIONFLOW:7、循环调用CSUDISMCSetPPS，FI/DI的值每次减0x01,直到返回CSUDI_SUCCESS为止，如果减到0x11还没有成功，则测试失败
//@EXECUTIONFLOW:8、调用CSUDISMCGetParams，判断返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:9、测试成功后，提示用户使用示波器测量频率
//@EXECUTIONFLOW:10、根据测试成功时的FI/DI的值获得他们分别对应的Fi、Di值,判断(Fi/Di)*波特率=频率的等式是否成立，成立测试成功，不成立测试失败
//@EXECUTIONFLOW:11、调用CSUDISMCClose关闭卡槽；
BOOL CSTC_SMARTCARD_IT_SetPPS_0073(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char ucWriteData[3] = {0};
	int nNumRead;
	unsigned char auResData[5] = {0};	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	int nNumToWrite;
	unsigned char ucInputFIDI;
	unsigned char ucFIDIStart;
	int nATRDi,nATRFi;	
	BOOL bOpened = CSUDI_FALSE;
	CSUDISMCParam_S sCardParamGet;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char *pckey3 =  "TEST_FIDI_START";
	char cResult1[16];
	char cResult2[16];	
	char cResult3[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0073]测试PPS交互成功的情况 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);		

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey3, cResult3, nResultSize), "步骤3失败\n");		
	ucInputFIDI = CSTKGetIntFromStr(cResult3, 10);		
	 
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack), "步骤5失败");
	
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "步骤6失败");
	
	memset(ucWriteData, 0, sizeof(ucWriteData));
	ucFIDIStart = 0x11 + ucInputFIDI;
	ucWriteData[0] = 0x10;
	nNumToWrite = 3;

	while(ucFIDIStart >= 0x11)
	{
		ucWriteData[1] = ucFIDIStart;
		if(CSUDI_SUCCESS == CSUDISMCSetPPS(nInputCardSlotNum, ucWriteData, nNumToWrite, auResData, &nNumRead))
		{
			break;
		}
		ucFIDIStart = ucFIDIStart - 0x01;
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0073]ucFIDIStart:0x%xn", ucFIDIStart);

	CSTK_ASSERT_TRUE_FATAL(ucFIDIStart >= 0x11, "步骤7失败");

	nATRDi = SmcGetDi(ucFIDIStart&0x0f);
	nATRFi = SmcGetFi((ucFIDIStart&0xf0) >> 4);

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0073]PPS交互成功时的Fi和Di分别是:%d,%0d!\n", nATRFi, nATRDi);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISMCGetParams(nInputCardSlotNum, &sCardParamGet)), "步骤8失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0073]当前智能卡的波特率是%d，请使用示波器测量频率，判断(Fi/Di)*波特率=频率的等式是否成立(是按Y,否按N)\n", sCardParamGet.m_nBaudRate);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤10失败");
	
	CSTK_FATAL_POINT;		

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "销毁资源失败");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0073]测试PPS交互成功的情况 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCActive、CSUDISMCDeactive
//@DESCRIPTION:测试卡槽中没有插入智能卡的情况下激活、去激活
//@PRECONDITION:1.卡槽索引号有效；
//@PRECONDITION:2.卡槽中没有插入智能卡
//@INPUT:nCardIndex = 用户输入的卡索引
//@EXPECTATION:调用CSUDISMCActive、CSUDISMCDeactive接口正确返回各异常情况
//@EXECUTIONFLOW:1、从配置文件中查询智能卡的卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams为卡槽设置正确的参数；
//@EXECUTIONFLOW:4、调用CSUDISMCOpen打开卡槽；
//@EXECUTIONFLOW:5、调用CSUDISMCActive
//@EXECUTIONFLOW:6、判断CSUDISMCActive的返回值是否等于CSUDISMC_ERROR_OUT
//@EXECUTIONFLOW:7、调用CSUDISMCDeactive
//@EXECUTIONFLOW:8、判断CSUDISMCDeactive的返回值是否等于CSUDISMC_ERROR_OUT
BOOL CSTC_SMARTCARD_IT_ActiveDeactive_0074(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	BOOL bOpened = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0074]测试卡槽中没有插入智能卡的情况下激活、去激活 start\n");
	
	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0074]请将测试卡从测试卡槽拔出，按任意键继续\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);		
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");

	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_OUT == CSUDISMCActive(nInputCardSlotNum),"步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_OUT == CSUDISMCDeactive(nInputCardSlotNum),"步骤8失败");

	CSTK_FATAL_POINT;	

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}

	CSTCPrint("请将测试卡正确插入测试卡槽\n");
	CSTKWaitAnyKey();
	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0074]测试卡槽中没有插入智能卡的情况下激活、去激活 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCActive、CSUDISMCDeactive
//@DESCRIPTION:测试成功激活、去激活的情况
//@PRECONDITION:1.接口各参数有效
//@INPUT:nCardIndex = 用户输入的卡索引;
//@EXPECTATION:接口返回值正确，各引脚状态正确
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams为卡槽设置正确的参数；
//@EXECUTIONFLOW:4、调用CSUDISMCOpen打开卡槽；
//@EXECUTIONFLOW:5、调用CSUDISMCActive，判断CSUDISMCActive的返回值是否等于CSUDI_SUCCESS；
//@EXECUTIONFLOW:6、提示用户测量并判断RST、CLK、VPP、I/O、VCC各引脚的状态是否正确(RST:L,VCC:powered,I/O:reception,VPP:idle,CLK:波形稳定)
//@EXECUTIONFLOW:7、调用CSUDISMCReset冷复位,判断CSUDISMCReset的返回值时候等于CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDISMCActive，判断CSUDISMCActive的返回值是否等于CSUDI_SUCCESS；
//@EXECUTIONFLOW:9、提示用户测量并判断CLK、VPP、VCC各引脚的状态是否正确(VCC:powered,VPP:idle,CLK:波形稳定)；
//@EXECUTIONFLOW:10、调用CSUDISMCDeactive，判断CSUDISMCDeactive的返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:11、提示用户测量并判断RST、CLK、VPP、I/O、VCC各引脚的状态是否正确(RST:L,VCC:inactive,I/O:state A,VPP:inactive,CLK:L)
//@EXECUTIONFLOW:12、调用CSUDISMCClose关闭卡槽；
BOOL CSTC_SMARTCARD_IT_ActiveDeactive_0075(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	BOOL bOpened = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]测试成功激活、去激活的情况,请正确插入智能卡 start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);		
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;		

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]请将示波器探头连接好智能卡CLK、VCC、RST、VPP、I/O引脚，准备抓取各引脚波形(按任意键继续):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCActive(nInputCardSlotNum),"步骤5失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]请测量并判断RST、CLK、VPP、I/O、VCC各引脚的状态是否正确(RST:L,VCC:powered,I/O:reception,VPP:idle,CLK:波形稳定)，是按Y，否按N");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤7失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]请将示波器探头连接好智能卡CLK、VCC、RST、VPP、I/O引脚，准备再次抓取各引脚波形(按任意键继续):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCActive(nInputCardSlotNum),"步骤8失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]请测量并判断CLK、VPP、VCC各引脚的状态是否正确(VCC:powered,VPP:idle,CLK:波形稳定)，是按Y，否按N");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤9失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]请将示波器探头连接好智能卡CLK、VCC、RST、VPP、I/O引脚，准备再次抓取各引脚波形(按任意键继续):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDeactive(nInputCardSlotNum),"步骤10失败");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]请测量并判断RST、CLK、VPP、I/O、VCC各引脚的状态是否正确(RST:L,VCC:inactive,I/O:state A,VPP:inactive,CLK:L)，是按Y，否按N");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤11失败");

	CSTK_FATAL_POINT;		

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]测试成功激活、去激活的情况 end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:测试反插卡后系统的稳定性
//@PRECONDITION:1.欲测试的卡槽中插入智能卡
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = 与卡匹配的命令;
//@INPUT:nNumberToWrite = 由实际命令长度决定;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:用例每一步的操作返回预期结果
//@EXECUTIONFLOW:1、从配置文件中查询智能卡的卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams为卡槽设置正确的参数；
//@EXECUTIONFLOW:4、调用CSUDISMCOpen打开卡槽；
//@EXECUTIONFLOW:5、调用CSUDISMCReset复位；
//@EXECUTIONFLOW:6、使用正确的命令调用CSUDISMCDataExchange进行数据交互；
//@EXECUTIONFLOW:7、拔掉智能卡，检测是否有回调上来；
//@EXECUTIONFLOW:8、提示用户将卡反插入卡槽并检测是否有回调上来；
//@EXECUTIONFLOW:9、调用CSUDISMCReset复位，返回CSUDISMC_ERROR_MUTE；
//@EXECUTIONFLOW:10、拔出智能卡，检测是否有回调上来；
//@EXECUTIONFLOW:11、将卡正确插入卡槽，检测是否有回调上来，
//@EXECUTIONFLOW:12、调用CSUDISMCReset复位卡槽，成功复位；
//@EXECUTIONFLOW:13、再次调用CSUDISMCDataExchange进行数据交互，交互成功；
//@EXECUTIONFLOW:14、调用CSUDISMCClose关闭卡槽。
BOOL CSTC_SMARTCARD_IT_0076(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	int nNumRead;
	int nNumToRead;
	int nNumWrite;
	int nTimeOut = 1000;
	unsigned char aucResData[512] = {0};	
	unsigned char auStatus[2] = {0};
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	int nSendLen;
	int i=0,j;
	BOOL bOpened = CSUDI_FALSE;
	BOOL bSptFile = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;
	BOOL bRemoved = CSUDI_FALSE;

	CSTCPrint("[CSTC_SMARTCARD_IT_0076]测试反插卡后系统的稳定性 start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;
	
	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤5失败");

	if(bSptFile)
    {
		int nIndex;
		sInfoLinkSMC *psCurLink = psRootNode;
		unsigned char auStatus[2];
		int nNumRead = 0;

		if(nInputCardNum != EM_TEST_DVN)
		{
			for(nIndex=0; nIndex<= psLastNode->m_num; nIndex++)
			{	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
					&nNumRead, auStatus),"步骤6数据交互失败");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"步骤6数据交互返回值失败");

				psCurLink = psCurLink->next;	
			}
		}
		else
		{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
					&nNumRead,nTimeOut),"步骤6第一次发送数据失败");				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 1,&nNumRead,nTimeOut),\
					"步骤6第一次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
					(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"步骤6第一次接收数据失败");
				psCurLink = psCurLink->next;	
					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, 5,\
					&nNumRead,nTimeOut),"步骤6第二次发送数据失败");					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"步骤6第二次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"步骤6第二次接收数据失败");
				psCurLink = psCurLink->next;	

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"步骤6第三次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"步骤6第三次接收数据失败");
				psCurLink = psCurLink->next;
				
				/*接收状态字*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 2,&nNumRead,nTimeOut),\
					"步骤6第四次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"步骤6第四次接收数据失败");					
    	}
	}
	else
	{
		for(i=0; i<MAX_TEST_CMD_NUM; i++)
		{
			memset(aucResData, 0, sizeof(aucResData));

			if(nInputCardNum != EM_TEST_DVN)
			{
				nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[i*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[i*2][1];
				if(nInputCardNum == EM_TEST_SUANTONG)
				{
					 unsigned char command[512];					 
				
					 memcpy(command,&(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]),nSendLen);
 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤6失败:suantong datafix failure");
					 //nSendLen += 4;
					
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"步骤6数据交返回值互失败");			 

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7.5失败:suantong datafix failure");
					 nNumRead +=4;
				}
                else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"步骤6命令交互失败");
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤6.5失败:COMVENIENT datafix failure");
					nNumRead +=4;
				}				
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤6数据交返回值互失败");
				}
				
		 		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead, auStatus),"步骤6数据交互失败");
			}
			else
			{
				CSTCPrint("[CSTC_SMARTCARD_IT_0076]nInputCardNum == EM_TEST_DVN!\n");

				nSendLen = g_DVNExchangeCmd[i].aaDVNCommand[0][0] + g_DVNExchangeCmd[i].aaDVNCommand[0][1];

				CSTCPrint("[CSTC_SMARTCARD_IT_0076]CSUDISMCSend before!\n");

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[i].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"步骤6第一次发送数据失败");

				CSTCPrint("[CSTC_SMARTCARD_IT_0076]CSUDISMCSend after!\n");

				/*接收1个字节*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"步骤6第一次接收数据返回值失败");

				CSTCPrint("[CSTC_SMARTCARD_IT_0076]CSUDISMCReceive after!\n");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[i].aaDVNCommand[0][3]),"步骤6第一次接收数据失败");

				nSendLen = g_DVNExchangeCmd[i].aaDVNCommand[1][0] + g_DVNExchangeCmd[i].aaDVNCommand[1][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[i].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"步骤6第二次发送数据失败");
				/*接收5个字节*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"步骤6第二次接收数据返回值失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"步骤6第二次接收数据失败");

				nNumToRead = aucResData[4];
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"步骤6第三次接收数据返回值失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"步骤6第三次接收数据失败");
				/*接收状态字*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"步骤6第四次接收数据返回值失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"步骤6第四次接收数据失败");		
			}
		}
	}

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_0076]请拔出智能卡\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_OUT, "步骤7失败");

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_0076]请将测试智能卡反插入测试卡槽\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_IN, "步骤8失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_MUTE == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "步骤9失败");

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_0076]请拔出智能卡\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_OUT, "步骤10失败");
	bRemoved = CSUDI_TRUE;

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_0076]请将测试智能卡正确插入测试卡槽\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_IN, "步骤11失败");
	bRemoved = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤12失败");

	if(bSptFile)
    {
		int nIndex;
		sInfoLinkSMC *psCurLink = psRootNode;
		unsigned char auStatus[2];
		int nNumRead = 0;

		if(nInputCardNum != EM_TEST_DVN)
		{
			for(nIndex=0; nIndex<= psLastNode->m_num; nIndex++)
			{	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
					&nNumRead, auStatus),"步骤13数据交互失败");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"步骤13数据交互返回值失败");

				psCurLink = psCurLink->next;	
			}
		}
		else
		{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
					&nNumRead,nTimeOut),"步骤6第一次发送数据失败");				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 1,&nNumRead,nTimeOut),\
					"步骤6第一次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
					(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"步骤6第一次接收数据失败");
				psCurLink = psCurLink->next;	
					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, 5,\
					&nNumRead,nTimeOut),"步骤6第二次发送数据失败");					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"步骤6第二次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"步骤6第二次接收数据失败");
				psCurLink = psCurLink->next;	

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"步骤6第三次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"步骤6第三次接收数据失败");
				psCurLink = psCurLink->next;
				
				/*接收状态字*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 2,&nNumRead,nTimeOut),\
					"步骤6第四次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"步骤11第四次接收数据失败");					
    	}
	}
	else
	{
		for(j=0; j<MAX_TEST_CMD_NUM; j++)
		{
			memset(aucResData, 0, sizeof(aucResData));

			if(nInputCardNum != EM_TEST_DVN)
			{	
				nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[j*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[j*2][1];
				if(nInputCardNum == EM_TEST_SUANTONG)
				{
					 unsigned char command[512];					 
				
					 memcpy(command,&(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]),nSendLen);
 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤13失败:suantong datafix failure");
					 //nSendLen += 4;
					
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"步骤13数据交互失败");			 

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7.5失败:suantong datafix failure");
					 nNumRead +=4;
				}
				else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤13失败");
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7.5失败:COMVENIENT datafix failure");
					 nNumRead +=4;
				}				
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤13数据交互失败");
				}

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, nNumRead, auStatus),"步骤13数据交互返回值失败");
			}
			else
			{	
				nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[0][0] + g_DVNExchangeCmd[j].aaDVNCommand[0][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"步骤6发送数据失败");
				/*接收1个字节*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"步骤6接收数据失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[j].aaDVNCommand[0][3]),"步骤6接收数据失败");

				nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[1][0] + g_DVNExchangeCmd[j].aaDVNCommand[1][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"步骤6发送数据失败");
				/*接收5个字节*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"步骤6接收数据失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"步骤6接收数据失败");

				nNumToRead = aucResData[4];
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"步骤6接收数据失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"步骤6接收数据失败");
				/*接收状态字*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"步骤6接收数据失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"步骤6接收数据失败");		
			}
		}
	}
	CSTK_FATAL_POINT;		

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}
	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("请将测试卡正确插入测试卡槽\n");
		CSTKWaitAnyKey();
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_0076]测试反插卡后系统的稳定性 end\n");

	return CSUDI_TRUE;
}


static CSUDI_BOOL g_bThreadExitIT_0077 = CSUDI_FALSE;
static void SMCTestThreadEntry(void * pvParam)
{
	CSUDI_Error_Code err = CSUDI_SUCCESS;
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;

	while(g_bThreadExitIT_0077)
	{
		if(CSUDISMC_ERROR_IN == CSUDISMCDetect(*(int*)pvParam))
		{
			err = CSUDISMCReset(*(int*)pvParam, aucAtr, &nAtrLen, CSUDI_TRUE);	
			if(err != CSUDI_SUCCESS)
			{
				CSTCPrint("Smartcard Reset Fail!\n");
			}
			else
			{
				CSTCPrint("Smartcard Reset Success!\n");
			}
		}
 	    CSUDIOSThreadSleep(500);
	}
}

//@CASEGROUP:CSUDISMCReset
//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:插拔卡强度测试
//@PRECONDITION:无
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = 与卡匹配的命令;
//@INPUT:nNumberToWrite = 由实际命令长度决定;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:强度测试之后，复位成功，交互命令成功
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的索引号，根据索引号获得保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、调用CSUDISMCSetParams为卡槽设置正确的参数；
//@EXECUTIONFLOW:4、调用CSUDISMCOpen打开卡槽；
//@EXECUTIONFLOW:5、创建任务，不停检测卡状态，当检测到卡插入时调用CSUDISMCReset热复位
//@EXECUTIONFLOW:6、提示并等于用户插拔卡大于50次(速率1秒钟三次,连续拔掉再插入算一次)
//@EXECUTIONFLOW:7、插拔50次之后调用CSUDISMCReset，判断复位是否成功，不成功则测试失败
//@EXECUTIONFLOW:8、根据用户的输入获得测试卡用于测试命令交互的一组命令，调用CSUDISMCDataExchange或CSUDISMCSend/CSUDISMCReceive，判断每条命令是否交互成功，若不成功则测试失败
//@EXECUTIONFLOW:9、调用CSUDISMCClose关闭卡槽；
BOOL CSTC_SMARTCARD_IT_0077(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	int nNumRead = 0;
	int nNumToRead;
	int nNumWrite;
	int nTimeOut = 1000;
	unsigned char aucResData[512] = {0};	
	unsigned char auStatus[2] = {0};
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	CSUDI_HANDLE hThreadhandle;

	unsigned int dwRLen;
	unsigned char  ucTransfT;
	CSUDI_Error_Code eRet = CSUDI_FALSE;
	int nSendLen;
	int j;
	BOOL bOpened = CSUDI_FALSE;
	BOOL bSptFile =CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]测试反复插拔卡后系统的稳定性 start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;	
	
	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}

	g_bThreadExitIT_0077 = CSUDI_TRUE;
	eRet = CSUDIOSThreadCreate("SMCTestThread", 7 , 1024, (CSUDIOSThreadEntry_F)SMCTestThreadEntry, &nInputCardSlotNum, &hThreadhandle);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == eRet) && (hThreadhandle != CSUDI_NULL),"步骤6，创建成功的任务输出的任务句柄不能为空");

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]请插拔卡大于50次(连续拔掉再插入算一次),之后按任意键继续\n");
	CSTKWaitAnyKey();
	g_bThreadExitIT_0077 = CSUDI_FALSE;
	CSUDIOSThreadSleep(1000);
	CSUDIOSThreadDestroy(hThreadhandle);

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]请将示波器探头连接好智能卡CLK、VCC、RST引脚，准备抓取各引脚波形(按Y继续):\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤7复位失败");
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"步骤7ATR错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"步骤7计算实际Atr长度失败");
	
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"步骤7失败,实际长度与计算长度不相等");

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]请使用示波器测量复位波形、CLK、VCC、RST引脚电平等是否符合ISO7816_123冷复位标准(是按Y,否按N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7复位后波形错误");

	if(bSptFile)
    {
		int nIndex;
		sInfoLinkSMC *psCurLink = psRootNode;
		unsigned char auStatus[2];
		int nNumRead = 0;

		if(nInputCardNum != EM_TEST_DVN)
		{
			for(nIndex=0; nIndex<= psLastNode->m_num; nIndex++)
			{	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
					&nNumRead, auStatus),"步骤8数据交互失败");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"步骤8数据交互返回值失败");

				psCurLink = psCurLink->next;	
			}
		}
		else
		{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
					&nNumRead,nTimeOut),"步骤8第一次发送数据失败");				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 1,&nNumRead,nTimeOut),\
					"步骤8第一次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
					(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"步骤8第一次接收数据失败");
				psCurLink = psCurLink->next;	
					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, 5,\
					&nNumRead,nTimeOut),"步骤8第二次发送数据失败");					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"步骤8第二次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"步骤8第二次接收数据失败");
				psCurLink = psCurLink->next;	

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"步骤8第三次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"步骤8第三次接收数据失败");
				psCurLink = psCurLink->next;
				
				/*接收状态字*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 2,&nNumRead,nTimeOut),\
					"步骤8第四次接收数据返回值失败");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"步骤8第四次接收数据失败");					
    	}
	}
	else
	{
		for(j=0; j<MAX_TEST_CMD_NUM; j++)
		{
			memset(aucResData, 0, sizeof(aucResData));

			if(nInputCardNum != EM_TEST_DVN)
			{
				nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[j*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[j*2][1];
				if(nInputCardNum == EM_TEST_SUANTONG)
				{
					 unsigned char command[512];					 
				
					 memcpy(command,&(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]),nSendLen);
	 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤8失败:suantong datafix failure");
						 //nSendLen += 4;
					
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"步骤8数据交互失败");

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤8.5失败:suantong datafix failure");
    				 nNumRead +=4;						 
				}
				else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤8失败");
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7.5失败:COMVENIENT datafix failure");
					 nNumRead +=4;
				}					
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤8数据交互失败");
				}
		 		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,&(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, nNumRead, auStatus),"步骤8数据交互返回值失败");
			}
			else
			{
				nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[0][0] + g_DVNExchangeCmd[j].aaDVNCommand[0][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"步骤8第一次发送数据失败");
				/*接收1个字节*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"步骤8第一次接收数据返回值失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[j].aaDVNCommand[0][3]),"步骤8第一次接收数据失败");

				nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[1][0] + g_DVNExchangeCmd[j].aaDVNCommand[1][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"步骤8第二次发送数据失败");
				/*接收5个字节*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"步骤8第二次接收数据返回值失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"步骤8接收数据失败");

				nNumToRead = aucResData[4];
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"步骤8第三次接收数据返回值失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"步骤8第三次接收数据失败");
				/*接收状态字*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"步骤8第四次接收数据返回值失败");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"步骤8第四次接收数据失败");		
			}
		}
	}
	
	CSTK_FATAL_POINT;		

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]测试反复插拔卡后系统的稳定性  end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:数据交互12小时强度测试(不针对DVN CA)
//@PRECONDITION:无
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = 与卡匹配的命令;
//@INPUT:nNumberToWrite = 由实际命令长度决定;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:强度测试之后，复位成功，交互命令成功
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的协议号，根据协议号获得测试卡索引号及保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、判断是否DVN CA,若是测试结束，否则继续。调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用CSUDISMCReset
//@EXECUTIONFLOW:6、根据测试卡的协议号，获得与卡匹配的一组用于测试交互的命令(保存在全局数组中)，在24小时内对该组命令循环调用CSUDISMCDataExchange
//@EXECUTIONFLOW:7、判断每次CSUDISMCDataExchange返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:8、判断根据协议计算每次命令回复的信息是否正确
//@EXECUTIONFLOW:9、调用CSUDISMCClose
//@EXECUTIONFLOW:10、调用CSUDISMCClose关闭卡槽；
BOOL CSTC_SMARTCARD_IT_0078(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum = 0;
	int nCardProtocalIndex;
	CSUDISMCParam_S sCardParamSet;	
	int nNumRead;
	unsigned char aucResData[512] = {0};	
	unsigned char auStatus[2] = {0};
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;	
	int nSendLen;
	int j;
	unsigned int dwStartTime;
	unsigned int dwTestTime = 0;
	BOOL bOpened = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_PROTOCAL_INDEX";	
	char *pckey3 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	char cResult3[16];	
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_0078]数据交互12小时强度测试 start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "步骤1失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nCardProtocalIndex = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey3,cResult3,nResultSize), "步骤2.5失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult3,10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "该测试用例不针对DVN CA");

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"步骤4失败");
	
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"步骤5失败");

	CSAtrDecode(aucAtr,nAtrLen);
	
	dwStartTime = CSUDIPlusOSGetTickCount();
	
	while(dwTestTime <= (12*ONE_HOUR_MILLISECONDS))/*we have to test for 12 hours*/
	{
		if(nCardProtocalIndex == 0)//T0协议
		{
			if(nInputCardNum != EM_TEST_TF)
			{
				for(j=0; j<MAX_TEST_CMD_NUM; j++)
				{
					memset(aucResData, 0, sizeof(aucResData));

					nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[j*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[j*2][1];
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤6-1数据交互失败");								
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,  &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, nNumRead,  auStatus),"步骤7-1数据交互返回值失败");
				}
			}
			else
			{
				for(j=0; j< sizeof(gsT0Command_TF)/sizeof(CaProtocalTestCommand_S); j++)
				{
					memset(aucResData, 0, sizeof(aucResData));					
					nSendLen = gsT0Command_TF[j].dwLen[0] + gsT0Command_TF[j].dwLen[1];
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, gsT0Command_TF[j].acCommand, nSendLen, aucResData, &nNumRead, auStatus),"步骤6-2数据交互失败");
			 		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, gsT0Command_TF[j].acCommand, nSendLen, aucResData, nNumRead, auStatus),"步骤7-2数据交互返回值失败");
				}
			}				
		}
		else if(nCardProtocalIndex ==1)//T1协议
		{
			if(nInputCardNum == EM_TEST_SUANTONG)
			{
				int i;
				unsigned char command[512];
				for(i=0; i<MAX_TEST_CMD_NUM; i++)
				{
					memset(aucResData, 0, sizeof(aucResData));

					nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[i*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[i*2][1];
					memcpy(command,&(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]),nSendLen);
						 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤11失败:suantong datafix failure");
						//nSendLen += 4;

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"步骤6-3数据交互失败");			 

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7-4失败:suantong datafix failure");
					nNumRead +=4;
							
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,  &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead,  auStatus),"步骤7-5数据交互返回值失败");
				}
			}
			else if(nInputCardNum == EM_TEST_COMVENIENT)
			{
   				int i;
				for(i=0; i<MAX_TEST_CMD_NUM; i++)
				{
    				 nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[i*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[i*2][1];
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"步骤6-4失败");
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"步骤7-6失败:COMVENIENT datafix failure");
					 nNumRead +=4;
 	 				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,  &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead,  auStatus),"步骤7-7失败");
	 				}
			}				
			else
			{
				for(j=0; j< sizeof(gsT1Command)/sizeof(CaProtocalTestCommand_S); j++)
				{
					memset(aucResData, 0, sizeof(aucResData));					
					nSendLen = gsT1Command[j].dwLen[0] + gsT1Command[j].dwLen[1];
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, gsT1Command[j].acCommand, nSendLen, aucResData, &nNumRead, auStatus),"步骤6-5数据交互失败");
				 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, gsT1Command[j].acCommand, nSendLen, aucResData, nNumRead, auStatus),"步骤7-8数据交互返回值失败");
				}
			}
		}
		else if(nCardProtocalIndex ==2)//T14协议
		{
			for(j=0; j< sizeof(gsT14Command)/sizeof(CaProtocalTestCommand_S); j++)
			{
				memset(aucResData, 0, sizeof(aucResData));					
				nSendLen = gsT14Command[j].dwLen[0] + gsT14Command[j].dwLen[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, gsT14Command[j].acCommand, nSendLen, aucResData, &nNumRead, auStatus),"步骤6-6数据交互失败");
			 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, gsT14Command[j].acCommand, nSendLen, aucResData, nNumRead, auStatus),"步骤7-9数据交互返回值失败");
			}
		}
		else
		{
		}
		dwTestTime = CSUDIPlusOSGetTickCount() - dwStartTime;
	}

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_0078]数据交互12小时强度测试 end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSTC_SMARTCARD_IT_Exchange_0079
//@DESCRIPTION:卡特殊指令交互
//@PRECONDITION:无
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = 与卡匹配的命令;
//@INPUT:nNumberToWrite = 由实际命令长度决定;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:交互命令成功
//@EXECUTIONFLOW:1、从配置文件中查询卡槽索引号
//@EXECUTIONFLOW:2、从配置文件中查询测试卡的协议号，根据协议号获得测试卡索引号及保存在全局变量中的卡的ETU、波特率、频率、标准、协议、停止位、校验位等参数
//@EXECUTIONFLOW:3、判断是否TF CA或SUMAVISION CA,若不是测试结束，否则继续。调用CSUDISMCSetParams，传入测试卡的参数
//@EXECUTIONFLOW:4、调用CSUDISMCOpen
//@EXECUTIONFLOW:5、调用CSUDISMCReset
//@EXECUTIONFLOW:6、根据测试卡的协议号，获得与卡匹配的一组特殊交互的命令
//@EXECUTIONFLOW:7、判断每次CSUDISMCDataExchange返回值是否等于CSUDI_SUCCESS
//@EXECUTIONFLOW:8、判断每次命令回复的信息是否正确
//@EXECUTIONFLOW:9、调用CSUDISMCClose
//@EXECUTIONFLOW:10、调用CSUDISMCClose关闭卡槽；
BOOL CSTC_SMARTCARD_IT_Exchange_0079(void)
{
	/*-------for example:tf3.0--------------------
[=>: 0x80, 0x4e, 0x00, 0x03, 0x11, ]//读到的数据为空，只返回了状态字
[<=: 0x94, 0x92, ]
[CA-SMC]SW1 SW2 0x94, 0x92
--------------------------------------------*/
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucResData[512];
	unsigned char auStatus[2];	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;
	int nNumRead;
	BOOL bOpened = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;	
	unsigned int dwStartTime = 0;
	unsigned int dwEndTime = 0;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0079]特殊指令测试 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "步骤2失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL((nInputCardNum == EM_TEST_TF) || (nInputCardNum == EM_TEST_SUMAVISION),
		"该测试用例只针对TF CA和SUMAVISION CA");	

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack), "步骤4失败");
	
	bOpened = CSUDI_TRUE;	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "步骤5失败");

	dwStartTime = CSUDIPlusOSGetTickCount();
	
	if(nInputCardNum == EM_TEST_TF)//TF card
	{
		static unsigned char ucSendData1[]={0x80, 0x4e, 0x00, 0x04, 0x11};
		static unsigned char ucSendData2[]={0x80, 0x4e, 0x00, 0x03, 0x11};		

		memset(aucResData, 0, sizeof(aucResData));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, ucSendData1, sizeof(ucSendData1)/sizeof(ucSendData1[0]), aucResData, &nNumRead, auStatus), "步骤6数据交互失败");
		CSTK_ASSERT_TRUE_FATAL(nNumRead == 0, "步骤6数据处理失败");
		
		memset(aucResData, 0, sizeof(aucResData));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, ucSendData2, sizeof(ucSendData2)/sizeof(ucSendData2[0]), aucResData, &nNumRead, auStatus), "步骤7数据交互失败");
		CSTK_ASSERT_TRUE_FATAL(nNumRead == 0, "步骤7数据处理失败");
	}
	else if(nInputCardNum == EM_TEST_SUMAVISION)//shumashixun card
	{
		static unsigned char ucSendData1[]={0x00, 0x20, 0x04, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		memset(aucResData, 0, sizeof(aucResData));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, ucSendData1, sizeof(ucSendData1)/sizeof(ucSendData1[0]), aucResData, &nNumRead, auStatus), "步骤6数据交互失败");
		CSTCPrint("nNumRead=%d,readdata[0]=0x%02x,status=%02x %02x\n", nNumRead, aucResData[0], auStatus[0], auStatus[1]);
		CSTK_ASSERT_TRUE_FATAL(nNumRead == 0, "步骤7数据处理失败");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "该测试用例只针对TF CA和SUMAVISION CA");
	}

	dwEndTime = CSUDIPlusOSGetTickCount();
	CSTK_ASSERT_TRUE_FATAL(dwEndTime - dwStartTime < 100, "步骤9交互时间过长");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "销毁资源失败");
	}	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0079]特殊指令测试 end\n");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSTC_SMARTCARD_IT_Exchange_0080
//@DESCRIPTION:卡指令交互中快速拔卡,应快速响应(不针对DVN CA)
//@PRECONDITION:无
//@EXPECTATION:交互命令成功每条指令失败的时候,在100ms内返回没插卡的错误quick insert/remove card, the callback function: response time: < 100ms CSSMCQuickInOutCheck(100,E_QSET);第一个参数设置拔卡最大响应时间(ms)
BOOL CSTC_SMARTCARD_IT_Exchange_0080(void)
{
	int nInputCardSlotNum=0;
	int nInputCardNum;
	CSUDISMCParam_S sCardParamSet;	
	unsigned char aucResData[512];
	unsigned char auStatus[2];	
	unsigned char aucAtr[64] = {0};	
	int nAtrLen;
	int nNumRead;
	int nTickBeforeExchange;
	int nTickAfterExchange;
	int nTickCost = 0;
	int i=0;
	int nSendLen;
	BOOL bOpened = CSUDI_FALSE;
	BOOL bSptFile = CSUDI_FALSE;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char *pckey2 =  "TEST_CARD_INDEX";
	char cResult1[16];
	char cResult2[16];	
	int  nResultSize = 16;	
	CSUDI_Error_Code emCardStatus;
	BOOL bNeedReset = FALSE;
   
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0080]快速拔卡,应快速响应,响应时间不超过500ms\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "步骤2失败\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "步骤2失败\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "该测试用例不针对DVN CA");	

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBackQuick), "步骤4失败");

	bOpened = CSUDI_TRUE;	

	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}

	CSTCGetConfigAtr();

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0080]请插卡后等待复位成功(打印出<TKC> CSUDISMCReset OK!),再拔卡. 30S内反复插拔卡,至少10次.按任意键开始操作\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "步骤5失败");

	nTickBeforeExchange = CSUDIPlusOSGetTickCount();
	CSSMCQuickInOutCheck(0,E_QCLEAR);
	CSSMCQuickInOutCheck(100+MAX_TEST_EXCHANGE_TIME, E_QSET);// 要加上卡交互指令的时间

	do
	{
		if(bSptFile)
		{
			sInfoLinkSMC *psCurLink = psLastNode;
			for(i=0; i<MAX_TEST_CMD_NUM; i++)
			{	
				CSUDISMCDataExchange(nInputCardSlotNum, 
				psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
				&nNumRead, auStatus);    					
			}			
		}
		else
		{
			emCardStatus = CSUDISMCDetect(nInputCardSlotNum);
			if((emCardStatus == CSUDISMC_ERROR_IN) || emCardStatus == CSUDISMC_ERROR_READY)
			{
				if(bNeedReset)
				{
					if(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE))
					{
						CSTCPrint("CSUDISMCReset OK!\n");
						bNeedReset = FALSE;
					}
				}    		       
			}
			else if (emCardStatus == CSUDISMC_ERROR_OUT)
			{
				bNeedReset = TRUE;
			}

			if(bNeedReset == FALSE)
			{
				for(i=0; i<MAX_TEST_CMD_NUM; i++)
				{
					memset(aucResData, 0, sizeof(aucResData));
					nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[i*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[i*2][1];

					if(nInputCardNum == EM_TEST_SUANTONG)
					{
						unsigned char command[512];

						nSendLen = g_TestCapCmd[nInputCardNum][0]+g_TestCapCmd[nInputCardNum][1];
						memcpy(command,&g_TestCapCmd[nInputCardNum][2], nSendLen);
        					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"步骤6失败:suantong datafix failure");
        					 //nSendLen += 4;

						CSSMCQuickInOutCheck(CSUDIPlusOSGetTickCount(), E_QSTART);
						if(CSUDI_SUCCESS != CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus))
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "快速拔卡,响应时间过长,步骤7失败");        					    
						}
						else
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData, nNumRead, 512), "步骤6.5失败:suantong datafix failure");
							nNumRead +=4;

							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus), "步骤6命令返回值失败");
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "快速拔卡,响应时间过长,步骤7失败");
						}
					}
					else if(nInputCardNum == EM_TEST_COMVENIENT)
					{
						CSSMCQuickInOutCheck(CSUDIPlusOSGetTickCount(), E_QSTART);
						if(CSUDI_SUCCESS != CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus))
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "快速拔卡,响应时间过长,步骤7失败");
						}
						else
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData, nNumRead, 512), "步骤7.5失败:COMVENIENT datafix failure");
							nNumRead +=4;                            
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead, auStatus), "步骤6命令返回值失败");
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "快速拔卡,响应时间过长,步骤7失败");
						}
					}        				
					else
					{	
						CSSMCQuickInOutCheck(CSUDIPlusOSGetTickCount(), E_QSTART);
						if(CSUDI_SUCCESS != CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus))    					
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "快速拔卡,响应时间过长,步骤7失败");
						}
						else
						{
							//拔卡后,这个函数会返回成功,其实最后1个或2个字节已经是错误的,所以无法检查数据有效性
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead, auStatus), "步骤6命令返回值失败");
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "快速拔卡,响应时间过长,步骤7失败");
						}
					}
				}
			}
		}
		nTickAfterExchange = CSUDIPlusOSGetTickCount();	
		nTickCost = nTickAfterExchange - nTickBeforeExchange;
	}while(nTickCost < 30000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_CNTCHECK), "快速插拔次数小于10次, 步骤8失败");

	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"销毁资源失败");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0080]快速拔卡测试end\n");
	return CSUDI_TRUE;
}


