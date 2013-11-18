/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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
#define MAX_TEST_EXCCMD_LEN   (160) /*���ڲ������ݽ����������*/
#define MAX_TEST_CAPCMD_LEN   (140) /*���ڲ������ܵ������*/
#define MAX_TEST_EXCHANGE_TIME (200) /*����ָ��������ʱ�䣬����ֵ*/

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

/*DVN ����ṹ��*/
typedef struct
{
	unsigned char aaDVNCommand[2][MAX_TEST_EXCCMD_LEN];
}DVNCommand_S;

typedef struct
{
	unsigned char	dwLen[2];
	unsigned char	acCommand[256];
}CaProtocalTestCommand_S;

/*������֧�ֵĲ���*/
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
/*���������ڲ������ݽ���������,����*/
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
	/*DVNû���õ�*/
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

/*���������ڲ������ܵ����һ��*/
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
	/*DVNû���õ���DVN��������*/
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


/*DVN���ֻ���淢��������*/
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

//nStatus:0 ��ʼ,1����,2,���ؽ��,3������
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
#define MAX_SMC_DATA 	3072	//��ʾ3072���ַ���"0xff, "��ʾһ��������Ҫ6���ַ���6*512
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

//��������ȡ����
int CSCAFsGetOneCmd(sInfoSMC **pSInfo,int nIndex)
{
	if(!psLastNode)
	{
		//û������
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

//����ԭʼ�ַ���ת��Ϊ���飬�������ݳ��ȷ�������
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
		//���ݸ�ʽ���ԣ�0x �� ���� ��ȣ�����ֻ��һ����������Ϊ�����ݴ���
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
				if(nIndex == 0 && nNum > 1)//������һ����ȷ������ű���
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
 *Function      : ���suantong T1 Э�飬�����ݽ��в�ȫ
 *Function Name : CSDataFixForSuantong
 *Paramater     :
 *	pucData [IN]: ��������
 *	nDataLen[IN]: ��������
 *	nMaxLen [IN]: �������ݵ�buffer�ܳ���
 *Return       :  SUCCESS:�ɹ�   FAILURE:ʧ��
 *Remark        :
 *	����NXPƽ̨������ָ����Ҫ��ȫ����������ݲ���Ҫ��ȫ
 *  ��ȫ����:  [0][0][len][...data...][lrc]
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
 *Function      : ������ʵATR���ȼ�����Э��(T0/T1/T14)
 *Function Name : CSCaculatAtrLen
 *Paramater     :
 *	pucAtr[IN]    : ATR����
 *	dwAtrLen[IN]  : ATR����
 *	pdwRLen[OUT]  : ͨ��ATR���ݼ������ʵ��ATR���ȣ�������dwAtrLen���бȽ�
 *	pcTransfT[OUT]: ��������ܿ�����Э��
 *Return        :
 *	SUCCESS:����ɹ�
 *	FAILURE:����ʧ��
 *Remark        :
 *	����T0     : û��У���ֽ�
 *	����T1/T14 : �����һ��У���ֽ�
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
			cHistoryLen = pucAtr[1] & 0x0F;//��ʷ�ֽڵĳ���
			cTChar = (pucAtr[1] & 0xF0) >> 4;//�ڶ���unsigned char��ֵ

			pucTmp += 2;
			*pcTransfT = 0;//t0Э��

			while(1)
			{
				//TA(i)����
				if(cTChar & 0x01)
				{
					cTCharLen++;
				}

				//TB(i)����
				if(cTChar & 0x02)
				{
					cTCharLen++;
				}

				//Tc(i)����
				if(cTChar & 0x04)
				{
					cTCharLen++;
				}

				//TD(i)����
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
			*pdwRLen = cTCharLen + cHistoryLen + 2;//���м�1�ǵ�1���ֽ�0x3B/0x3F

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
 *Function      :У��T14�����Ƿ���ȷ
 *Function Name : CSCheckT14TranfIsRight
 *Parameter     :
 *pcSendData[IN]     :��װ��������д������
 *pcResponseData[IN] :���л�Ӧ������
 *nResponseLen[IN]   :���з��ص����ݳ���
 *Return Value :
 *	CSUDI_SUCCESS  : ���ջ�Ӧ��������ȷ
 *	CSUDI_FAILURE    : ���ջ�Ӧ�����ݴ���
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
			nResponseParseLen = pcResponseData[7] + 8 + 1;//����1��У���ֽڣ�8��ǰ��Ӧ���ݵ�ǰ8���ֽ�
			if(nResponseParseLen == nResponseLen)
			{
				for(ii = 1; ii < (nResponseParseLen - 1); ii++)
				{
					cCheckSum ^= pcResponseData[ii]; //��Irdeto�ĵ����ҵ���
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
 *Function      :У��T1�����Ƿ���ȷ
 *Function Name : CSCheckT1TranfIsRight
 *Parameter     :
 *pcResponseData[IN] :���л�Ӧ������
 *nResponseLen[IN]   :���з��ص����ݳ���
 *Return Value :
 *	CSUDI_SUCCESS  : ���ջ�Ӧ��������ȷ
 *	CSUDI_FAILURE    : ���ջ�Ӧ�����ݴ���
 ****************************************************************************************/
static int CSCheckT1TranfIsRight(unsigned char * pcResponseData, int nResponseLen)
{
	int nRet = CSUDI_FAILURE;
	int  ii;
	unsigned char cCheckSum = 0;

	if(NULL != pcResponseData)
	{
		if(pcResponseData[2] + 4 == nResponseLen)/*4:NAD��PCB��LEN��EDC */
		{			
			for(ii = 0; ii < (nResponseLen - 1); ii++)/*LRCУ��*/
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
 *Function      :У��T0�����Ƿ���ȷ
 *Function Name : CSCheckT0TranfIsRight
 *Parameter     :
 *pcSendData[IN]     :��װ��������д������
 *nSendLen[IN]       :������д�����ݵĳ���
 *pucStatusWord[IN]  :���л�Ӧ��״̬��
 *nResponseLen[IN]   :���з��ص����ݳ���
 *Return Value :
 *	CSUDI_SUCCESS  : ���ջ�Ӧ��������ȷ
 *	CSUDI_FAILURE   : ���ջ�Ӧ�����ݴ���
 ****************************************************************************************/
static int CSCheckT0TranfIsRight(unsigned char * pcSendData, int nSendLen, int nResponseLen, unsigned char * pucStatusWord)
{
	int nRet = CSUDI_FAILURE;	
	
	if((NULL != pcSendData) && (NULL != pucStatusWord))
	{
		if(nSendLen > 5)//дָ��
		{
			if((pucStatusWord[0] & 0x60) || (pucStatusWord[0] & 0x90))
			{
				nRet = CSUDI_SUCCESS;
			}			
		}
		else//��ָ��
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
 *Function      :У�����ݴ����Ƿ���ȷ
 *Function Name : CSCheckTranfIsRight
 *Parameter     :
 *eProtocalUsed[IN]  :����Э��
 *pcSendData[IN]     :��װ��������д������
 *nSendLen[IN]       :������д�����ݵĳ���
 *pcResponseData[IN] :���з��ص�����
 *pucStatusWord[IN]  :���л�Ӧ��״̬��
 *nResponseLen[IN]   :���з��ص����ݳ���
 *Return Value :
 *	CSUDI_SUCCESS  : ���ջ�Ӧ��������ȷ
 *	CSUDI_FAILURE   : ���ջ�Ӧ�����ݴ���
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

/*��ʱ�Ļص�����*/
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
	//�ڱ�����������ִ��ǰ����
	CSTCPrint("��ģ���������Ĭ�Ͻ����Կ���ȷ������Կ���\n");

	return CSUDI_TRUE;
}

BOOL CSTC_SMARTCARD_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetCount
//@DESCRIPTION:����pnSMCCountΪ�յ����
//@PRECONDITION:��
//@INPUT:pnSMCCount = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_GetCount_0001(void)
{ 
	CSTCPrint( "\n[CSTC_SMARTCARD_IT_GetCount_0001]���Բ���Ϊ�յ�����»�ȡ���۵ĸ�����ʼ\n");

	CSTCPrint( "\n[CSTC_SMARTCARD_IT_GetCount_0001]����ֵ��:%d  \n",CSUDISMCGetCount(NULL));

       CSTCPrint("Before Test!!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCGetCount(NULL), "����1ʧ��");
       CSTCPrint("After Test!!!\n");       
	CSTK_FATAL_POINT;
	CSTCPrint( "\n[CSTC_SMARTCARD_IT_GetCount_0001]���Բ���Ϊ�� ������»�ȡ���۵ĸ�������\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetCount
//@DESCRIPTION:���Գɹ���ѯ���۸��������
//@PRECONDITION:��
//@INPUT:int nCardNum = 0xff;
//@INPUT:pnSMCCount = &nCardNum;
//@EXPECTATION:�ӿڷ���CSUDI_TRUE����ѯ���Ŀ��۸�����ʵ�ʸ�����ͬ
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯ��Ч���۸���
//@EXECUTIONFLOW:2����ѯconfig�ļ��еĿ��۸���
//@EXECUTIONFLOW:3���ж������Ƿ���ͬ����ͬ���Գɹ����������ʧ��
BOOL CSTC_SMARTCARD_IT_GetCount_0002(void)
{
	int nCardNum = 0xff;
	char *pcSection = "SMARTCARD";
	char *pckey =  "CARD_SLOT_NUM" ;
	char cResult[16];
	int  nResultSize = 16;
	CSUDI_BOOL bRet = CSUDI_FALSE;

	CSTCPrint("[CSTC_SMARTCARD_IT_GetCount_0002]������ȷ��ȡ���۵ĸ���start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCGetCount(&nCardNum) , "����1ʧ��");
	
	CSTCPrint("[CSTC_SMARTCARD_IT_GetCount_0002]��õĿ��۸���:%d\n",nCardNum);

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey,cResult,nResultSize), "����2ʧ��\n");		
	CSTK_ASSERT_TRUE_FATAL(nCardNum == CSTKGetIntFromStr(cResult,10),"����3ʧ��\n");	
	bRet = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_GetCount_0002]������ȷ��ȡ���۵ĸ���end\n");

	return bRet;
}

//@CASEGROUP:CSUDISMCDetect
//@DESCRIPTION: ���Կ�����������Ч(����-1)�����
//@PRECONDITION: ��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1�����ýӿ�CSUDISMCDetect���������-1���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Detect_0003(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0003]���Կ���������Чʱ(����-1)��⿨״̬start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCDetect(-1), "����1ʧ��");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0003]���Կ���������Чʱ(����-1)��⿨״̬end\n");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDISMCDetect
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2�����ýӿ�CSUDISMCDetect���������nCardNum���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Detect_0004(void)
{
	int nCardNum = 0xff;

	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0004]���Կ���������Ч(���������Ч������+1)ʱ��⿨״̬start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDetect(nCardNum), "����2ʧ��");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0004]���Կ���������Ч(���������Ч������+1)ʱ��⿨״̬end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetParams�����ò���nCardIndex = -1���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0005]���Կ�����������Ч(����-1)ʱִ��CSUDISMCSetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(-1, &sSmcParm), "����1ʧ��");

	CSTK_FATAL_POINT;	
       
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0005]���Կ�����������Ч(����-1)ʱִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2������CSUDISMCSetParams�����ò���nCardIndex = nCardNum���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
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

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0006]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCSetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(nCardNum, &sSmcParm), "����2ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0006]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:����psSMCParamsָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@INPUT:nCardIndex = 0;
//@INPUT:psSMCParams = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetParam_0007(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0007]����psSMCParamsָ��Ϊ��ʱִ��CSUDISMCSetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==   CSUDISMCSetParams(0, NULL), "����1ʧ��");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0007]����psSMCParamsָ��Ϊ��ʱִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:����psSMCParams��m_eCheckBit���Ϸ�(EM_UDISMC_PARITY_MAX+1)�����
//@PRECONDITION:1.������������Ч
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
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0008]���Բ���psSMCParams��m_eCheckBit���Ϸ�(EM_UDISMC_PARITY_MAX)�����ִ��CSUDISMCSetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "����1ʧ��");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0008]]���Բ���psSMCParams��m_eCheckBit���Ϸ�(EM_UDISMC_PARITY_MAX)�����ִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:����psSMCParams��m_eProtocol���Ϸ�(EM_UDISMC_PROTOCOL_MAX+1)�����
//@PRECONDITION:1.������������Ч
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
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
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

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0009]���Բ���psSMCParams��m_eProtocol���Ϸ�(EM_UDISMC_PROTOCOL_MAX)�����ִ��CSUDISMCSetParams start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "����1ʧ��");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0009]���Բ���psSMCParams��m_eProtocol���Ϸ�(EM_UDISMC_PROTOCOL_MAX)�����ִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:����psSMCParams��m_eStandard���Ϸ�(EM_UDISMC_STANDARD_MAX+1)�����
//@PRECONDITION:1.������������Ч
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
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0010]���Բ���psSMCParams��m_eStandard���Ϸ�(EM_UDISMC_STANDARD_MAX)�����ִ��CSUDISMCSetParams start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "����1ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0010]]���Բ���psSMCParams��m_eStandard���Ϸ�(EM_UDISMC_STANDARD_MAX)�����ִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:����psSMCParams��m_eStopBit���Ϸ�(EM_UDISMC_STOP_MAX+1)�����
//@PRECONDITION:1.������������Ч
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
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
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

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0011]���Բ���psSMCParams��m_eStopBit���Ϸ�(EM_UDISMC_STOP_MAX)�����ִ��CSUDISMCSetParams start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "����1ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0011]]���Բ���psSMCParams��m_eStopBit���Ϸ�(EM_UDISMC_STOP_MAX)�����ִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:����psSMCParams��m_nBaudRate���Ϸ�(����-1)�����
//@PRECONDITION:1.������������Ч
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
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
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

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0012]���Բ���psSMCParams��m_nBaudRate���Ϸ�(����-1)�����ִ��CSUDISMCSetParams start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "����1ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0012]���Բ���psSMCParams��m_nBaudRate���Ϸ�(����-1)�����ִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:����psSMCParams��m_nETU���Ϸ�(����-1)�����
//@PRECONDITION:1.������������Ч
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
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0013]���Բ���psSMCParams��m_nETU���Ϸ�(����-1)�����ִ��CSUDISMCSetParams start\n");
	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "����1ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0013]]���Բ���psSMCParams��m_nETU���Ϸ�(����-1)�����ִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams
//@DESCRIPTION:����psSMCParams��m_nFrequency���Ϸ�(����-1)�����
//@PRECONDITION:1.������������Ч
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
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
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

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0014]���Բ���psSMCParams��m_nFrequency���Ϸ�(����-1)�����ִ��CSUDISMCSetParams start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetParams(0, &sSmcParm), "����1ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParam_0014]]���Բ���psSMCParams��m_nFrequency���Ϸ�(����-1)�����ִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetParams
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_GetParam_0015(void)
{
	CSUDISMCParam_S sSmcParm;

	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0015]���Կ�����������Ч(����-1)ʱִ��CSUDISMCGetParams start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCGetParams(-1, &sSmcParm), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0015]]���Կ�����������Ч(����-1)ʱִ��CSUDISMCGetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetParams
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2������CSUDISMCGetParams���������nCardNum���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_GetParam_0016(void)
{
	int nCardNum = 0xff;	
	CSUDISMCParam_S sSmcParm;		

	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0016]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCGetParams start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCGetParams(nCardNum, &sSmcParm), "����2ʧ��");
	
	CSTK_FATAL_POINT;	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0016]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCGetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCGetParams
//@DESCRIPTION:����psSMCParamsָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@INPUT:nCardIndex = 0;
//@INPUT:psSMCParams = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetParams���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_GetParam_0017(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0017]����psSMCParamsָ��Ϊ��ʱִ��CSUDISMCGetParams start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCGetParams(0, NULL), "����1ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_GetParam_0017]����psSMCParamsָ��Ϊ��ʱִ��CSUDISMCSetParams end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCOpen���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Open_0018(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0018]���Կ�����������Ч(����-1)�������ִ��CSUDISMCOpen  start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCOpen(-1, CSSMCCallBack), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0018]���Կ�����������Ч(����-1)�������ִ��CSUDISMCOpen  end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2������CSUDISMCOpen���������nCardNum���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Open_0019(void)
{
	int nCardNum = 0xff;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0019]���Կ�����������Ч(���������Ч������+1)�������ִ��CSUDISMCOpen  start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCOpen(nCardNum, CSSMCCallBack), "����2ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0019]���Կ�����������Ч(���������Ч������+1)�������ִ��CSUDISMCOpen  end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:����fnSMCCallbackָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@INPUT:nCardIndex = 0;
//@INPUT:fnSMCCallback = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCOpen���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Open_0020(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0020]����fnSMCCallbackָ��Ϊ��ʱִ��CSUDISMCOpen start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCOpen(0, NULL), "����1ʧ��");
	
	CSTK_FATAL_POINT;		
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0020]����fnSMCCallbackָ��Ϊ��ʱִ��CSUDISMCOpen end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCClose
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCClose���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Close_0021(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0021]���Կ�����������Ч(����-1)ʱִ��CSUDISMCClose start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCClose(-1), "����1ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0021]]���Կ���������Ч(����-1)ʱִ��CSUDISMCClose end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCClose
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2������CSUDISMCClose���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Close_0022(void)
{
	int nCardNum;

	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0022]���Կ�����������Ч(���������Ч������+1)ִ��CSUDISMCClose start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCClose(nCardNum), "����2ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0022]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCClose end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCReset���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Close_0023(void)
{
	unsigned char aucAtr[64] = {0};
	int nAtrLen;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0023]���Կ�����������Ч(����-1)ʱִ��CSUDISMCReset start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCReset(-1, aucAtr, &nAtrLen, CSUDI_TRUE), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0023]]���Կ�����������Ч(����-1)ʱִ��CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2������CSUDISMCReset���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Reset_0024(void)
{
	unsigned char aucAtr[64] = {0};
	int nAtrLen;
	int nCardNum;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0024]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCReset start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCReset(nCardNum, aucAtr, &nAtrLen, CSUDI_TRUE), "����2ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0024]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:����pucAtrָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@INPUT:nCardIndex = 0;
//@INPUT:pucAtr = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCReset���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Reset_0025(void)
{
	int nAtrLen;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0025]����puAtrָ��Ϊ��ʱִ��CSUDISMCReset start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCReset(0, NULL, &nAtrLen, CSUDI_TRUE), "����1ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0025]����puAtrָ��Ϊ��ʱִ��CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:����pnAtrLengthָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucAtrָ�벻Ϊ��
//@INPUT:unsigned char aucAtr[64] = {0};	
//@INPUT:nCardIndex = 0; 
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength =NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCReset���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Reset_0026(void)
{
	unsigned char aucAtr[64] = {0};	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0026]����pnAtrLengthָ��Ϊ��ʱִ��CSUDISMCReset start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCReset(0, aucAtr, NULL, CSUDI_TRUE), "����1ʧ��");
	
	CSTK_FATAL_POINT;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0026]����pnAtrLengthָ��Ϊ��ʱִ��CSUDISMCReset end\n");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCDataExchange���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0027(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0027]���Կ�����������Ч(����-1)ʱִ��CSUDISMCDataExchange start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(-1, aucDataToWrite, 5, aucResData, &nNumRead, pucStatusWord), "����1ʧ��");
	
	CSTK_FATAL_POINT;	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0027]]���Կ�����������Ч(����-1)ʱִ��CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2������CSUDISMCDataExchange���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0028(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	int nNumRead;
	int nCardNum;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0028]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCDataExchange start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(nCardNum, aucDataToWrite, 5, aucResData, &nNumRead, pucStatusWord), "����2ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0028]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:����pucWriteDataָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCDataExchange,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0029(void)
{
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0029]����pucWriteDataָ��Ϊ��ʱִ��CSUDISMCDataExchange start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, NULL, 5, aucResData, &nNumRead, pucStatusWord), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0029]����pucWriteDataָ��Ϊ��ʱִ��CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:����nNumberToWrite���Ϸ�(����-1)�����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucWriteData��Ϊ��
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWrite = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCDataExchange,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0030(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0030]����nNumberToWrite���Ϸ�(����-1)ʱִ��CSUDISMCDataExchange start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, aucDataToWrite, -1, aucResData, &nNumRead, pucStatusWord), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0030]����nNumberToWrite���Ϸ�(����-1)ʱִ��CSUDISMCDataExchange end\n");


	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:����pcResponseDataָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucWriteData��Ϊ��
//@PRECONDITION:3.nNumberToWrite��Ч
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pcResponseData = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCDataExchange,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0031(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char pucStatusWord[2];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0031]����pcResponseDataָ��Ϊ��ʱִ��CSUDISMCDataExchange start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, aucDataToWrite, 5, NULL, &nNumRead, pucStatusWord), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0031]����pcResponseDataָ��Ϊ��ʱִ��CSUDISMCDataExchange end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:����pnNumberReadָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucWriteData��Ϊ��
//@PRECONDITION:3.nNumberToWrite��Ч
//@PRECONDITION:4.pcResponseData��Ϊ��
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:unsigned char aucResData[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCDataExchange,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0032(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	unsigned char pucStatusWord[2];
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0032]����pnNumberReadָ��Ϊ��ʱִ��CSUDISMCDataExchange start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, aucDataToWrite, 5, aucResData, NULL, pucStatusWord), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0032]����pnNumberReadָ��Ϊ��ʱִ��CSUDISMCDataExchange end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:����pucStatusWordָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucWriteDataָ�벻Ϊ��
//@PRECONDITION:3.nNumberToWrite��Ч
//@PRECONDITION:4.pcResponseDataָ�벻Ϊ��
//@PRECONDITION:5.pnNumberReadָ�벻Ϊ��
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCDataExchange,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Exchange_0033(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucResData[512];
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0033]����pucStatusWordָ��Ϊ��ʱִ��CSUDISMCDataExchange start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCDataExchange(0, aucDataToWrite, 5, aucResData, &nNumRead, NULL), "����1ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0033]����pucStatusWordָ��Ϊ��ʱִ��CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSend,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0034(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumWrite;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0034]���Կ�����������Ч(����-1)ʱִ��CSUDISMCSend start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(-1, aucDataToWrite, 5, &nNumWrite, 1000), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0034]���Կ�����������Ч(����-1)ʱִ��CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2������CSUDISMCSend,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0035(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumToWrite=5;
	int nNumWrite;
	int nTimeout=1000;
	int nCardNum;

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0035]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCSend start\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(nCardNum, aucDataToWrite, nNumToWrite, &nNumWrite, nTimeout), "����2ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0035]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:����pucWriteBufָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = NULL;
//@EXPECTATION::�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSend,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0036(void)
{
	int nNumToWrite=5;
	int nNumWrite;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0036]����pucWriteBufָ��Ϊ��ʱִ��CSUDISMCSend start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(0, NULL, nNumToWrite, &nNumWrite, 1000), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0036]]���Կ�����������Ч(����-1)ʱִ��CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:����nNumberToWrite���Ϸ�(����-1)�����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucWriteBuf��Ϊ��
//@INPUT:ubsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSend,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0037(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumWrite;

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0037]]����nNumberToWrite���Ϸ�(����-1)ʱִ��CSUDISMCSend end\n");
				
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(0, aucDataToWrite, -1, &nNumWrite, 1000), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0037]]����nNumberToWrite���Ϸ�(����-1)ʱִ��CSUDISMCSend end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:����pnNumberWriteָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucWriteBuf��Ϊ��
//@PRECONDITION:3.nNumberToWrite��Ч
//@INPUT:ubsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pnNumberWrite = NULL;
//@EXPECTATION::�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSend,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0038(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumToWrite=5;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0038]����pnNumberWriteָ��Ϊ��ʱִ��CSUDISMCSend start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(0, aucDataToWrite, nNumToWrite, NULL, 1000), "����1ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0038]����pnNumberWriteָ��Ϊ��ʱִ��CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:����nTimeout���Ϸ�(����-1)�����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucWriteBuf��Ϊ��
//@PRECONDITION:3.nNumberToWrite��Ч
//@PRECONDITION:4.pnNumberWrite ��Ϊ��;
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:int nNumWrite;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pnNumberWrite = &nNumWrite;
//@INPUT:nTimeout = -1;  
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSend,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Send_0039(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumToWrite=5;
	int nNumWrite;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0039]����nTimeout���Ϸ�(����-1)ʱִ��CSUDISMCSend start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSend(0, aucDataToWrite, nNumToWrite,&nNumWrite, -1), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0039]����nTimeout���Ϸ�(����-1)ʱִ��CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCReceive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0040(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=5;
	int nNumRead;
	int nTimeout=1000;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0040]���Կ�����������Ч(����-1)ʱִ��CSUDISMCReceive start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(-1, aucDataToRead, nNumToRead, &nNumRead, nTimeout),"����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0040]]���Կ�����������Ч(����-1)ʱִ��CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION::�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2������CSUDISMCReceive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0041(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=5;
	int nNumRead;
	int nTimeout=1000;
	int nCardNum;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0041]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCReceive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(nCardNum, aucDataToRead, nNumToRead, &nNumRead, nTimeout), "����2ʧ��");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0041]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCReceive end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:����pucReadBufָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@INPUT:nCardIndex = 0;
//@INPUT:pucReadBuf = NULL;
//@EXPECTATION::�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCReceive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0042(void)
{
	int nNumToRead=5;
	int nNumRead;
	int nTimeout=1000;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0042]����pucReadBufָ��Ϊ��ʱִ��CSUDISMCReceive start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(0, NULL, nNumToRead, &nNumRead, nTimeout), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0042]����pucReadBufָ��Ϊ��ʱִ��CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:����nNumberToRead���Ϸ�(����-1)�����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucReadBuf��Ϊ��
//@INPUT:unsigned char aucDataRead[64]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucReadBuf = aucDataRead;
//@INPUT:nNumberToRead = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCReceive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0043(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=-1;
	int nNumRead;
	int nTimeout=1000;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0043]����nNumberToRead���Ϸ�(����-1)ʱִ��CSUDISMCReceive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(0, aucDataToRead, nNumToRead, &nNumRead, nTimeout), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0043]����nNumberToRead���Ϸ�(����-1)ʱִ��CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:����pnNumberReadָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucReadBuf��Ϊ��
//@PRECONDITION:3.nNumberToRead��Ч
//@INPUT:unsigned char aucDataRead[64]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucReadBuf = aucDataRead;
//@INPUT:nNumberToRead = 4;
//@INPUT:pnNumberRead = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCReceive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0044(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=5;
	int nTimeout=1000;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0044]����pnNumberReadָ��Ϊ��ʱִ��CSUDISMCReceive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(0, aucDataToRead, nNumToRead, NULL, nTimeout), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0044]����pnNumberReadָ��Ϊ��ʱִ��CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReceive
//@DESCRIPTION:����nTimeout���Ϸ�(����-1)�����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucReadBuf��Ϊ��
//@PRECONDITION:3.nNumberToRead��Ч
//@PRECONDITION:4.pnNumberRead��Ϊ��
//@INPUT:unsigned char aucDataRead[64]={0};
//@INPUT:int nNumRead;
//@INPUT:nCardIndex = 0;
//@INPUT:pucReadBuf = aucDataRead;
//@INPUT:nNumberToRead = 4;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:nTimeout = -1��
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCReceive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Receive_0045(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToRead=5;
	int nNumRead;
	int nTimeout=-1;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0045]����nTimeout���Ϸ�(����-1)ʱִ��CSUDISMCReceive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCReceive(0, aucDataToRead, nNumToRead, &nNumRead, nTimeout), "����1ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Receive_0045]����nTimeout���Ϸ�(����-1)ʱִ��CSUDISMCReceive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetPPS,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0046(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucDataToRead[5]={0};
	int nNumToWrite=5;
	int nNumRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0046]���Կ�����������Ч(����-1)ʱִ��CSUDISMCSetPPS start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==  CSUDISMCSetPPS(-1, aucDataToWrite, nNumToWrite, aucDataToRead, &nNumRead), "����1ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0046]]���Կ�����������Ч(����-1)ʱִ��CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:2������CSUDISMCSetPPS,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0047(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucDataToRead[5]={0};
	int nNumToWrite=5;
	int nNumToRead;
	int nCardNum;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0047]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCSetPPS start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCSetPPS(nCardNum, aucDataToWrite, nNumToWrite, aucDataToRead, &nNumToRead), "����2ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0047]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:����pucWriteDataָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetPPS,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0048(void)
{
	unsigned char aucDataToRead[5]={0};
	int nNumToWrite=5;
	int nNumToRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0048]����pucWriteDataָ��Ϊ��ʱִ��CSUDISMCSetPPS start\n");
			
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCSetPPS(0, NULL, nNumToWrite, aucDataToRead, &nNumToRead), "����1ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0048]����pucWriteDataָ��Ϊ��ʱִ��CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:����pucResponseDataָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucWriteData��Ϊ��
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWriteLen = 5;
//@INPUT:pucResponseData = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetPPS,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0049(void)
{
	unsigned char aucDataToWrite[5]={0};
	int nNumToWrite=5;
	int nNumToRead;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0049]����pucResponseDataָ��Ϊ��ʱִ��CSUDISMCSetPPS start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCSetPPS(0, aucDataToWrite, nNumToWrite, NULL, &nNumToRead), "����1ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0049]����pucResponseDataָ��Ϊ��ʱִ��CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:����pnResponseDataLenָ��Ϊ�յ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucWriteData��Ϊ��
//@PRECONDITION:3.pucResponseData��Ϊ��
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:unsigned char aucResData[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWriteLen = 5;
//@INPUT:pucResponseData = aucResData;
//@INPUT:pnResponseDataLen = NULL;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCSetPPS,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_SetPPS_0050(void)
{
	unsigned char aucDataToWrite[5]={0};
	unsigned char aucDataToRead[5]={0};
	int nNumToWrite=5;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0050]����pnResponseDataLenָ��Ϊ��ʱִ��CSUDISMCSetPPS start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCSetPPS(0, aucDataToWrite, nNumToWrite, aucDataToRead, NULL), "����1ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0050]����pnResponseDataLenָ��Ϊ��ʱִ��CSUDISMCSetPPS end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCActive
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCActive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Active_0051(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Active_0051]���Կ�����������Ч(����-1)ʱִ��CSUDISMCActive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCActive(-1), "����1ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Active_0051]]���Կ�����������Ч(����-1)ʱִ��CSUDISMCActive end\n");	

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCActive
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:1������CSUDISMCActive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Active_0052(void)
{
	int nCardNum;

	CSTCPrint("[CSTC_SMARTCARD_IT_Active_0052]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCActive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER ==CSUDISMCActive(nCardNum), "����2ʧ��");
	
	CSTK_FATAL_POINT;

	CSTCPrint("[CSTC_SMARTCARD_IT_Active_0052]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCActive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDeactive
//@DESCRIPTION:���Կ�����������Ч(����-1)�����
//@PRECONDITION:��
//@INPUT:nCardIndex = -1;
//@EXPECTATION::�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCDeactive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Deactive_0053(void)
{
	CSTCPrint("[CSTC_SMARTCARD_IT_Deactive_0053]���Կ�����������Ч(����-1)ʱִ��CSUDISMCDeactive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCDeactive(-1), "����1ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Deactive_0053]���Կ�����������Ч(����-1)ʱִ��CSUDISMCDeactive end\n");	

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDeactive
//@DESCRIPTION:���Կ�����������Ч(���������Ч������+1)�����
//@PRECONDITION:1.��ȷ��ѯϵͳ�еĿ��۸���nCardNum
//@INPUT:nCardIndex = nCardNum;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDISMCGetCount��ѯϵͳ����Ч���۵ĸ���nCardNum
//@EXECUTIONFLOW:1������CSUDISMCDeactive,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_BAD_PARAMETER
BOOL CSTC_SMARTCARD_IT_Deactive_0054(void)
{
	int nCardNum;

	CSTCPrint("[CSTC_SMARTCARD_IT_Deactive_0054]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCActive start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDISMCGetCount(&nCardNum), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_BAD_PARAMETER == CSUDISMCDeactive(nCardNum), "����2ʧ��");
	
	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Deactive_0054]���Կ�����������Ч(���������Ч������+1)ʱִ��CSUDISMCDeactive end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDetect
//@DESCRIPTION:���Գɹ���⿨״̬�����
//@PRECONDITION:1.�����Ŀ�����������Ч
//@INPUT:nCardIndex = �û�����Ŀ�������;
//@EXPECTATION:��ȷ��ѯ����״̬
//@EXECUTIONFLOW:1���������ļ��в�ѯ���Կ���������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ���ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���ֵ
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5����ʾ���ȴ��û��������ܿ�
//@EXECUTIONFLOW:6�����ýӿ�CSUDISMCDetect
//@EXECUTIONFLOW:7���ж�CSUDISMCDetect����ֵ�Ƿ����CSUDISMC_ERROR_IN
//@EXECUTIONFLOW:8����ʾ���ȴ��û��ε����ܿ�
//@EXECUTIONFLOW:9�����ýӿ�CSUDISMCDetect
//@EXECUTIONFLOW:10���ж�CSUDISMCDetect����ֵ�Ƿ����CSUDISMC_ERROR_OUT
//@EXECUTIONFLOW:11������CSUDISMCReset
//@EXECUTIONFLOW:12�����ýӿ�CSUDISMCDetect
//@EXECUTIONFLOW:13���ж�CSUDISMCDetect����ֵ�Ƿ����CSUDISMC_ERROR_READY
//@EXECUTIONFLOW:14������CSUDISMCClose
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0055]������ȷ��⿨״̬start\n");
		
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
		
 	sCardParam.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParam.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParam.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParam.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParam.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParam.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParam.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParam)," ����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	bOpened = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_IN == CSUDISMCDetect(nInputCardSlotNum),"����7ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����8ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_READY == CSUDISMCDetect(nInputCardSlotNum),"����9ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0055]�뽫���Կ��Ӳ��Կ��۰γ�(�����������)\r\n");
	CSTKWaitAnyKey();
	bRemoved = CSUDI_TRUE;
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_OUT == CSUDISMCDetect(nInputCardSlotNum),"����10ʧ��");
	
	CSTK_FATAL_POINT;
		
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"����14��������Դʧ��");
	}
	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("�뽫���Կ���ȷ������Կ���\n");
		CSTKWaitAnyKey();
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Detect_0055]������ȷ��⿨״̬start\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetParams/CSUDISMCGetParams
//@DESCRIPTION:���Գɹ����á���ѯ���������
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.psSMCParamsָ�벻Ϊ���Ҹ���Ա����Ч
//@INPUT:nCardIndex = �û�����Ŀ�������;
//@INPUT:psSMCParams  = �û�����Ĳ���;
//@EXPECTATION:��ѯ���ĸ����������õ�һ�£�ʹ��ʾ�������������ʺ�Ƶ�������õ�ֵһ��
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ������û��������ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4���жϷ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDISMCOpen���ص�������Ԥ�ȶ���õĻص�����
//@EXECUTIONFLOW:6������CSUDISMCReset���жϷ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDISMCGetParams���Ƚϲ�ѯ���Ĳ��������õ��Ƿ���ͬ
//@EXECUTIONFLOW:8��ʹ��ʾ��������Ƶ�������õ�ֵ�Ƿ�һ��
//@EXECUTIONFLOW:9������CSUDISMCClose
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParamGetParam_0056]���Գɹ����á���ѯ���� start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," ����4ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����5ʧ��");

	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISMCGetParams(nInputCardSlotNum, &sCardParamGet)),"����7ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_SetParamGetParam_0056] ��ѯ��checkbit��%d,standard��%d,stopbit��%d,frequency��%d,\
		protocal��%d,baudrate��%d,etu��%d",sCardParamGet.m_eCheckBit,sCardParamGet.m_eStandard,sCardParamGet.m_eStopBit,sCardParamGet.m_nFrequency,
		sCardParamGet.m_eProtocol,sCardParamGet.m_nBaudRate,sCardParamGet.m_nETU);

	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_eCheckBit == sCardParamSet.m_eCheckBit),"����7.1ʧ��"); 
	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_eStandard == sCardParamSet.m_eStandard) ,"����7.2ʧ��");
	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_eStopBit == sCardParamSet.m_eStopBit),"����7.3ʧ��");
	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_eProtocol == sCardParamSet.m_eProtocol),"����7.4ʧ��"); 
	
	CSTK_ASSERT_TRUE_FATAL((sCardParamGet.m_nFrequency/sCardParamGet.m_nBaudRate +1 == sCardParamGet.m_nETU 
	||sCardParamGet.m_nFrequency/sCardParamGet.m_nBaudRate == sCardParamGet.m_nETU 
	||sCardParamGet.m_nFrequency/sCardParamGet.m_nBaudRate -1 == sCardParamGet.m_nETU),"����7.5ʧ��");
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParamGetParam_0056] ���õ�Ƶ����:%d,��ʹ��ʾ���������Ƿ������õ�һ��(�ǰ�Y,��N)\n",sCardParamSet.m_nFrequency);

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"����8ʧ��");
	
	CSTK_FATAL_POINT;
		
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"����9��������Դʧ��");
	}
		
	CSTCPrint("[CSTC_SMARTCARD_IT_SetParamGetParam_0056]���Բ��Գɹ����á���ѯ���� end\n");	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:���Զ�ͬһ�����۳ɹ��򿪺��ٴδ򿪵����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.fnSMCCallback��Ϊ��
//@PRECONDITION:3.����֮ǰ���ɹ���
//@INPUT:static void CSSMCCallBack(int nCardIndex, CSUDI_Error_Code eStatus);
//@INPUT:nCardIndex = 0;
//@INPUT:fnSMCCallback = CSSMCCallBack;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_FAILURE
//@EXECUTIONFLOW:1���������ļ��в�ѯ���Կ���������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ���������
//@EXECUTIONFLOW:3�����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ�������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen���жϷ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDISMCOpen���жϷ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDISMCClose���жϷ���ֵ�Ƿ����CSUDI_SUCCESS
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

	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0057]���Զ�ͬһ�����۳ɹ��򿪺��ٴδ� start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," ����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	bOpened = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����5ʧ��");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0057]���Զ�ͬһ�����۳ɹ��򿪺��ٴδ� end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCOpen
//@DESCRIPTION:���Գɹ��򿪡��رտ��۵����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.fnSMCCallback��Ϊ��
//@PRECONDITION:3.����֮ǰû�б��ɹ���
//@PRECONDITION:4.����PC�ͻ����д��ڣ��򿪴��ڹ���
//@INPUT:nCardIndex = 0;
//@INPUT:fnSMCCallback = CSSMCCallBack;
//@EXPECTATION:�ɹ��򿪺󣬲�����ܿ����ڴ��ڴ�ӡ���ܹ������ص�������⵽��In�Ϳ�outʱ�������Ϣ���ɹ��رպ󣬲�ο�ʱ�ڴ��ڴ�ӡ�п������ص�������⵽��In�Ϳ�Outʱ�������Ϣ
//@EXECUTIONFLOW:1���������ļ��в�ѯ���Կ���������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����ݿ������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:3������CSUDISMCOpen���ص�������Ԥ�ȶ���õĻص�����
//@EXECUTIONFLOW:4����ʾ�û��γ����ܿ����ж��Ƿ��лص�����
//@EXECUTIONFLOW:5����ʾ�û��������ܿ����ж��Ƿ��лص�����
//@EXECUTIONFLOW:6������CSUDISMCClose
//@EXECUTIONFLOW:7����ʾ�û��γ����ܿ����ж��Ƿ��лص�����
//@EXECUTIONFLOW:8����ʾ�û��������ܿ����ж��Ƿ��лص�����
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]���Գɹ��򿪡��رտ��� start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "����1ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);	
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," ����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����3ʧ��");
	bOpened = CSUDI_TRUE;	
	
	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]��γ����ܿ�\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_OUT, "����4ʧ��");

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]��������ܿ�\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_IN, "����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "����6ʧ��");	
	
	bOpened = CSUDI_FALSE;	

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]��γ����ܿ�\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDI_FAILURE, "����7ʧ��");
	bRemoved = CSUDI_TRUE;

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]��������ܿ�\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDI_FAILURE, "����8ʧ��");
	bRemoved = CSUDI_FALSE;

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "������Դʧ��");
	}
	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("�뽫���Կ���ȷ������Կ���\n");
		CSTKWaitAnyKey();
	}
	CSTCPrint("[CSTC_SMARTCARD_IT_Open_0058]���Գɹ��򿪡��رտ��� end\n");	

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCClose
//@DESCRIPTION:���Թر�֮ǰ����û�д򿪵����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.����֮ǰû�б��ɹ���
//@INPUT:nCardIndex = 0;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_FAILURE
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2������CSUDISMCClose���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_FAILURE
BOOL CSTC_SMARTCARD_IT_Close_0059(void)
{	
	int nInputCardSlotNum=0;
	char *pcSection = "SMARTCARD";
	char *pckey1 =  "TEST_CARD_SLOT_INDEX";
	char cResult1[16];
	int  nResultSize = 16;
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0059]���Թر�֮ǰ����û�д� start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCClose(nInputCardSlotNum), "����2ʧ��");		

	CSTK_FATAL_POINT;
	CSTCPrint("[CSTC_SMARTCARD_IT_Close_0059]���Թر�֮ǰ����û�д� end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:���Կ�����û�в������ܿ�ʱReset�����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucAtrָ�벻Ϊ��
//@PRECONDITION:3.pnAtrLengthָ�벻Ϊ��
//@PRECONDITION:4.������û�в������ܿ�
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = �û������������;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@INPUT:bColdRst = TRUE
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_FAILURE
//@EXECUTIONFLOW:1���������ļ��в�ѯ���Կ���������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen���ص�������Ԥ�ȶ���õĻص�����
//@EXECUTIONFLOW:5���ж�CSUDISMCOpen�ķ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDISMCReset,�жϷ���ֵ�Ƿ����CSUDISMC_ERROR_FAILURE
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0060]���Կ�����û�в������ܿ�ʱִ��CSUDISMCReset start\n");
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0060]�뽫���Կ��Ӳ��Կ����аγ��������������\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," ����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����5ʧ��");
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_FALSE) ,"����6ʧ��");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}
	CSTCPrint("�뽫���Կ���ȷ������Կ���\n");
	CSTKWaitAnyKey();
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0060]���Կ�����û�в������ܿ�ʱִ��CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:���Է��忨ʱ��λʧ�ܵ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucAtrָ�벻Ϊ��
//@PRECONDITION:3.pnAtrLengthָ�벻Ϊ��
//@PRECONDITION:4.�����з��������ܿ�
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = �û������������;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@INPUT:bColdRst = TRUE
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_ERROR_ANSWER��
//@EXECUTIONFLOW:1���������ļ��в�ѯ���Կ���������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen���ص�������Ԥ�ȶ���õĻص�����
//@EXECUTIONFLOW:5���ж�CSUDISMCOpen�ķ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDISMCReset,�ж�CSUDISMCReset�ķ���ֵ�Ƿ����CSUDISMC_ERROR_MUTE
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0061]���Կ����з��忨ʱִ��CSUDISMCReset start\n");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0061]�뽫���Կ���������Կ���(�޴����Ӵ�)�������������\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet)," ����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����5ʧ��");
	bOpened = CSUDI_TRUE;	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_MUTE == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����6ʧ��");
	
	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}	
	CSTCPrint("�뽫���Կ���ȷ������Կ���\n");
	CSTKWaitAnyKey();

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0061]���Կ����з��忨ʱִ��CSUDISMCReset end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:�������ô����Ƶ��ʱ��λʧ�ܵ����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucAtrָ�벻Ϊ��
//@PRECONDITION:3.pnAtrLengthָ�벻Ϊ��
//@PRECONDITION:4.�����в������ܿ�
//@PRECONDITION:5.Ϊ�������ô����Ƶ��
//@PRECONDITION:6.����֮ǰ���ɹ���
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = �û������������;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@INPUT:bColdRst = TRUE
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_ERROR_ANSWER��
//@EXECUTIONFLOW:1���������ļ��в�ѯ���Կ���������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ�������
//@EXECUTIONFLOW:3�����ݲ��Կ������Ż�ÿ�֧�ֵ�Ƶ�ʷ�Χ(������ȫ�ֵ�������)
//@EXECUTIONFLOW:4������CSUDISMCSetParams�����벻�ڲ��Կ�֧�ַ�Χ�ڵ�Ƶ�ʲ���
//@EXECUTIONFLOW:5������CSUDISMCOpen
//@EXECUTIONFLOW:6������CSUDISMCReset
//@EXECUTIONFLOW:7���ж�CSUDISMCReset�ķ���ֵ�Ƿ����CSUDISMC_ERROR_FAILURE
//@EXECUTIONFLOW:8��CSUDISMCClose
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0062]�������ô����Ƶ��ʱ��λʧ�� start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol ;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = 20000000;//g_sCardPara[nInputCardNum].m_nFrequency + 1000;//0-5M����ɹ�

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����5ʧ��");

	bOpened = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����7ʧ��");
	
	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0062]�������ô����Ƶ��ʱ��λʧ�� end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:���Գɹ���λ�����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucAtrָ�벻Ϊ��
//@PRECONDITION:3.pnAtrLengthָ�벻Ϊ��
//@PRECONDITION:4.�����в������ܿ�
//@PRECONDITION:5.Ϊ����������ȷ�Ĳ���
//@PRECONDITION:6.����֮ǰ���ɹ���
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = �û������������;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@EXPECTATION:�ӿڷ���CSUDI_SUCCESS��
//@EXPECTATION:Atr�ĵ�һ���ֽ���0x3b��0x3f��
//@EXPECTATION:���ص�Atr������ʵ�ʷ��ص�Atr���ݳ�����ͬ��
//@EXPECTATION:ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ����ISO7816_123��׼��
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5������CSUDISMCReset,�������bColdRst = TRUE
//@EXECUTIONFLOW:6���ж�CSUDISMCReset�ķ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:7���ж�Atr�ĵ�һ���ֽ��Ƿ���0x3b��0x3f��
//@EXECUTIONFLOW:8�������û������Э�����ʵ�ʷ��ص�Atr���ݵĳ��ȣ��ж��뷵�ص�pnAtrLength�Ƿ���ͬ
//@EXECUTIONFLOW:9����ʾ�û�ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�临λ��׼
//@EXECUTIONFLOW:10������CSUDISMCReset,�������bColdRst = FALSE
//@EXECUTIONFLOW:11���ж�CSUDISMCReset�ķ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:12���ж�Atr�ĵ�һ���ֽ��Ƿ���0x3b��0x3f��
//@EXECUTIONFLOW:13�������û������Э�����ʵ�ʷ��ص�Atr���ݵĳ��ȣ��ж��뷵�ص�pnAtrLength�Ƿ���ͬ
//@EXECUTIONFLOW:14��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�临λ��׼
//@EXECUTIONFLOW:15������CSUDISMCClose
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]���Գɹ���λ start\n");	
		
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");			
	
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]�뽫ʾ����̽ͷ���Ӻ����ܿ�CLK��VCC��RST���ţ�׼��ץȡ�����Ų���(�����������):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"����8����ʵ��Atr����ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"����8ʧ��,ʵ�ʳ�������㳤�Ȳ����");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�临λ��׼(�ǰ�Y,��N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]�뽫ʾ����̽ͷ���Ӻ����ܿ�CLK��VCC��RST���ţ�׼���ٴ�ץȡ�����Ų���(�����������):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_FALSE),"����11ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"����12ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"����13����ʵ��Atr����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"����13ʧ��,ʵ�ʳ�������㳤�Ȳ����");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�ȸ�λ��׼(�ǰ�Y,��N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ��");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0063]���Գɹ���λ end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCReset
//@DESCRIPTION:���Գɹ��ȸ�λ���ٴ��ȸ�λ�����
//@PRECONDITION:1.������������Ч
//@PRECONDITION:2.pucAtrָ�벻Ϊ��
//@PRECONDITION:3.pnAtrLengthָ�벻Ϊ��
//@PRECONDITION:4.�����в������ܿ�
//@PRECONDITION:5.Ϊ����������ȷ�Ĳ���
//@PRECONDITION:6.����֮ǰ���ɹ���
//@INPUT:unsigned char aucAtr[64] = {0};
//@INPUT:int nAtrLen;	
//@INPUT:nCardIndex = �û������������;
//@INPUT:pucAtr = aucAtr; 
//@INPUT:pnAtrLength = &nAtrLen;
//@EXPECTATION:�ٴ��ȸ�λ�ӿڷ���CSUDI_SUCCESS��
//@EXPECTATION:�ٴ��ȸ�λ��Atr�ĵ�һ���ֽ���0x3b��0x3f��
//@EXPECTATION:�ٴ��ȸ�λ�󷵻ص�Atr������ʵ�ʷ��ص�Atr���ݳ�����ͬ��
//@EXPECTATION:�ٴ��ȸ�λ��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ����ISO7816_123��׼��
//@EXECUTIONFLOW:1���������ļ���ѯ���Կ���������
//@EXECUTIONFLOW:2���������ļ���ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5������CSUDISMCReset,�������bColdRst = TRUE
//@EXECUTIONFLOW:6���ж�CSUDISMCReset�ķ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:7���ж�Atr�ĵ�һ���ֽ��Ƿ���0x3b��0x3f��
//@EXECUTIONFLOW:8�����ݲ��Կ�Э�����ʵ�ʷ��ص�Atr���ݵĳ��ȣ��ж��뷵�ص�pnAtrLength�Ƿ���ͬ
//@EXECUTIONFLOW:9��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�临λ��׼��
//@EXECUTIONFLOW:10���ٴε���CSUDISMCReset���������bColdRst = FALSE
//@EXECUTIONFLOW:11���ж�CSUDISMCReset�ķ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:12���ж�Atr�ĵ�һ���ֽ��Ƿ���0x3b��0x3f��
//@EXECUTIONFLOW:13�����ݲ��Կ�Э�����ʵ�ʷ��ص�Atr���ݵĳ��ȣ��ж��뷵�ص�pnAtrLength�Ƿ���ͬ
//@EXECUTIONFLOW:14��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�ȸ�λ��׼
//@EXECUTIONFLOW:15���ٴε���CSUDISMCReset���������bColdRst = FALSE
//@EXECUTIONFLOW:16���ж�CSUDISMCReset�ķ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:17���ж�Atr�ĵ�һ���ֽ��Ƿ���0x3b��0x3f��
//@EXECUTIONFLOW:18�����ݲ��Կ�Э�����ʵ�ʷ��ص�Atr���ݵĳ��ȣ��ж��뷵�ص�pnAtrLength�Ƿ���ͬ
//@EXECUTIONFLOW:19��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�ȸ�λ��׼
//@EXECUTIONFLOW:20������CSUDISMCClose
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]���Գɹ��ȸ�λ���ٴ��ȸ�λ start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;	

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]�뽫ʾ����̽ͷ���Ӻ����ܿ�CLK��VCC��RST���ţ�׼��ץȡ�����Ų���(�����������):\n");

	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"����8����ʵ��Atr����ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"����8ʧ��,ʵ�ʳ�������㳤�Ȳ����");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�临λ��׼(�ǰ�Y,��N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]�뽫ʾ����̽ͷ���Ӻ����ܿ�CLK��VCC��RST���ţ�׼���ٴ�ץȡ�����Ų���(�����������):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_FALSE),"����11ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"����12ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"����13����ʵ��Atr����ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"����13ʧ��,ʵ�ʳ�������㳤�Ȳ����");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�ȸ�λ��׼(�ǰ�Y,��N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]�뽫ʾ����̽ͷ���Ӻ����ܿ�CLK��VCC��RST���ţ�׼���ٴ�ץȡ�����Ų���(�����������):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_FALSE),"����16ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"����17ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"����18����ʵ��Atr����ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"����18ʧ��,ʵ�ʳ�������㳤�Ȳ����");

	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�ȸ�λ��׼(�ǰ�Y,��N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����19ʧ��");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Reset_0064]���Գɹ��ȸ�λ���ٴ��ȸ�λ end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:���Կ�����û�в������ܿ��Ϳ���û�и�λ�����(�����DVN CA)
//@PRECONDITION:1.�ӿڸ�������Ч
//@PRECONDITION:2.������û�в������ܿ�
//@PRECONDITION:3.����֮ǰû�б��ɹ�reset
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
//@EXPECTATION:�ӿڷֱ𷵻�CSUDISMC_ERROR_OUT��CSUDISMC_ERROR_NOT_RESET
//@EXECUTIONFLOW:1���������ļ���ѯ���Կ���������
//@EXECUTIONFLOW:2���������ļ���ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5�����ýӿ�CSUDISMCDataExchange���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_OUT
//@EXECUTIONFLOW:6����ʾ���ȴ��û��������ܿ�
//@EXECUTIONFLOW:7�����ýӿ�CSUDISMCDataExchange���жϷ���ֵ�Ƿ����CSUDISMC_ERROR_NOT_RESET
//@EXECUTIONFLOW:8������CSUDISMCClose
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

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0065]���Կ�����û�в������ܿ��Ϳ���û�и�λʱ����CSUDISMCDataExchange start\n");
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0065]�뽫���Կ��Ӳ��Կ��۰γ��������������\n");
	CSTKWaitAnyKey();
	bRemoved = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "����1ʧ��");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "����2ʧ��");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "�ò������������DVN CA");

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack), "����4ʧ��");
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_OUT == CSUDISMCDataExchange(nInputCardSlotNum, aucDataToWrite, 5, aucResData, &nNumRead, auStatus), "����5ʧ��");
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0065]�뽫���Կ�������Կ���(�����������)\n");
	CSTKWaitAnyKey();
	bRemoved = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_NOT_RESET == CSUDISMCDataExchange(nInputCardSlotNum, aucDataToWrite, 5, aucResData, &nNumRead, auStatus), "����7ʧ��");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "������Դʧ��");
	}

	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("�뽫���Կ���ȷ������Կ���\n");
		CSTKWaitAnyKey();
	}
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0065]���Կ�����û�в������ܿ��Ϳ���û�и�λʱ����CSUDISMCDataExchange end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:���Գɹ�������������(�����DVN CA)
//@PRECONDITION:1.�ӿڸ�������Ч
//@PRECONDITION:2.����֮ǰ�ɹ�reset
//@PRECONDITION:3.����ʹ�õ���������ԵĿ���ƥ��
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = �뿨ƥ�������;
//@INPUT:nNumberToWrite = ��ʵ������Ⱦ���;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:�ӿڷ���CSUDI_SUCCESS,����Э���������ظ���Ϣ��ȷ
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3���ж��Ƿ�DVN CA,���ǲ��Խ������������������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5������CSUDISMCReset
//@EXECUTIONFLOW:6�����ݲ��Կ��������ţ�����뿨ƥ���һ�����ڲ��Խ���������(������ȫ��������)���Ը����������CSUDISMCDataExchange
//@EXECUTIONFLOW:7���ж�ÿ��CSUDISMCDataExchange����ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:8���жϸ���Э�����ÿ������ظ�����Ϣ�Ƿ���ȷ
//@EXECUTIONFLOW:9������CSUDISMCClose
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

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0066]���Գɹ��������� start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "�ò������������DVN CA");

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;	
 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����5ʧ��");
	
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
				&nNumRead, auStatus),"����7ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
				psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
				psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
				s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"����8ʧ��");

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
 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����6ʧ��:suantong datafix failure");
					 //nSendLen += 4;
				
				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"����7ʧ��");			 

				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7.5ʧ��:suantong datafix failure");
				 nNumRead +=4;
			}
			else if(nInputCardNum == EM_TEST_COMVENIENT)
			{
                 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����7ʧ��");
                 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7.5ʧ��:COMVENIENT datafix failure");
				 nNumRead +=4;
			}
			else
			{	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����7ʧ��");
			}
					
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,  &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead,  auStatus),"����8ʧ��");
		}
	}
	
	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	
	if(CSUDI_TRUE == bOpened)
	{	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");			
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0066]���Գɹ���������  end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:���ܲ���(���Խ��������ʱ��)(�����DVN CA)
//@PRECONDITION:1.�ӿڸ�������Ч
//@PRECONDITION:2.����֮ǰ�ɹ�reset
//@PRECONDITION:3.ʹ��Ԥ�ȶ���ĸ������������ܲ��Ե�����
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = �뿨ƥ�������;
//@INPUT:nNumberToWrite = ��ʵ������Ⱦ���;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:�ӿڷ���CSUDI_SUCCESS,����ظ���Ϣ�����ݺͳ���������Ԥ����ͬ������ʹ�õ�ʱ�䲻����Ԥ��ʱ��
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3���ж��Ƿ�DVN CA,���ǲ��Խ������������������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5������CSUDISMCReset
//@EXECUTIONFLOW:6�����ݲ��Կ��������ţ�����뿨ƥ���һ���������ܲ��Ե����Ԥ�ڽ���ʱ��(������ȫ��������)
//@EXECUTIONFLOW:7������CSUDIPlusOSGetTickCount����ѯ��ǰʱ��
//@EXECUTIONFLOW:8������CSUDISMCDataExchange
//@EXECUTIONFLOW:9������CSUDIPlusOSGetTickCount����ѯ��ǰʱ��,�Ƚ�ǰ��ʱ�䣬�õ�����ʹ�õ�ʱ��
//@EXECUTIONFLOW:10���жϷ���ֵCSUDISMCDataExchange�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:11���жϸ���Э���������ظ�����Ϣ�Ƿ���ȷ
//@EXECUTIONFLOW:12���ж������ʹ�õ�ʱ���Ƿ�С��Ԥ�ڽ���ʱ�䣬����С�������ʧ�ܣ���ӡ��Ԥ�ں�ʵ�ʽ���ʱ��
//@EXECUTIONFLOW:13������CSUDISMCClose
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

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0067]���ܲ���(���Խ��������ʱ��) start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "�ò������������DVN CA");

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;	

	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����5ʧ��");

	nTickBeforeExchange = CSUDIPlusOSGetTickCount();

	if(bSptFile)
	{
		sInfoLinkSMC *psCurLink = psLastNode;
		{
			for(i=0; i<MAX_TEST_CMD_NUM; i++)
			{	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, 
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, s_ReData,\
					&nNumRead, auStatus),"����8�����ʧ��");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"����14�����ֵʧ��");
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
					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����6ʧ��:suantong datafix failure");
					 //nSendLen += 4;

				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"����10ʧ��");			 

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����6.5ʧ��:suantong datafix failure");
				 nNumRead +=4;

			}
			else if(nInputCardNum == EM_TEST_COMVENIENT)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"����6�����ʧ��");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����6.5ʧ��:COMVENIENT datafix failure");
				nNumRead +=4;
			}				
			else
			{			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"����6ʧ��");
			}				

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead,  auStatus),"����7ʧ��");
		}
	}
	nTickAfterExchange = CSUDIPlusOSGetTickCount();

	nTickCost =(nTickAfterExchange - nTickBeforeExchange)/MAX_TEST_CMD_NUM;

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0067]��������ʹ�õ�ʱ����:%d ms\n", nTickCost);
	
	CSTK_ASSERT_TRUE_FATAL(nTickCost <= MAX_TEST_EXCHANGE_TIME, "����12ʧ��");

	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0067]���ܲ���(���Խ��������ʱ��) end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:�ϻ�����1��Сʱ���ݽ���������Լ��ϻ����Ժ����ܲ���(�����DVN CA)
//@PRECONDITION:1.�ӿڸ�������Ч
//@PRECONDITION:2.����֮ǰ�ɹ�reset
//@PRECONDITION:3.����ʹ�õ���������ԵĿ���ƥ��
//@PRECONDITION:4.���Ӻ�PC������д���
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = �뿨ƥ�������;
//@INPUT:nNumberToWrite = ��ʵ������Ⱦ���;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:1.ÿ�ν����ӿڷ���CSUDI_SUCCESS,����ظ���Ϣ�����ݺͳ���������Ԥ����ͬ���ϻ�ǰ�󽻻�ͬһ�������ʱ�����
//@EXPECTATION:2.������֮�����ΰβ忨�ڴ��ڴ�ӡ���ܹ������ص�������⵽��Out�Ϳ�Inʱ�Ĵ�ӡ��Ϣ
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3���ж��Ƿ�DVN CA,���ǲ��Խ������������������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5������CSUDISMCReset
//@EXECUTIONFLOW:6�������û�����Ĳ��Կ����ͣ�����뿨ƥ����������ܲ��Ե�����(һ����������ȫ��������)
//@EXECUTIONFLOW:7������CSUDIPlusOSGetTickCount����ѯ��ǰʱ��
//@EXECUTIONFLOW:8���Ĵε���CSUDISMCDataExchange
//@EXECUTIONFLOW:9������CSUDIPlusOSGetTickCount����ѯ��ǰʱ��,�Ƚ�ǰ��ʱ�䣬�󽻻�һ��ƽ��ֵ���õ��ϻ�����֮ǰ��������ʹ�õ�ʱ��
//@EXECUTIONFLOW:10�����ݲ��Կ��������ţ�����뿨ƥ���һ�����ڽ������Ե�����(������ȫ��������)����1��Сʱ֮��ѭ������CSUDISMCDataExchange
//@EXECUTIONFLOW:11���ж�ÿ�ε���CSUDISMCDataExchange�ķ���ֵ�Ƿ����CSUDI_SUCCESS��������ͬ���˳�ѭ��������ʧ��
//@EXECUTIONFLOW:12������Э�����ÿ�ε���CSUDISMCDataExchange������ظ���Ϣ�Ƿ���ȷ��������ȷ���˳�ѭ��������ʧ��
//@EXECUTIONFLOW:13������CSUDIPlusOSGetTickCount����ѯ��ǰʱ��
//@EXECUTIONFLOW:14���Ĵε���CSUDISMCDataExchange������ͬһ���������ܲ��Ե�����
//@EXECUTIONFLOW:15������CSUDIPlusOSGetTickCount����ѯ��ǰʱ��,�Ƚ�ǰ��ʱ�䣬�󽻻�һ��ƽ��ֵ���õ��ϻ�����֮�󽻻�����ʹ�õ�ʱ��
//@EXECUTIONFLOW:16����ӡ�����ε�ʱ�䣬��ʾ�û��ж������Ƿ��½��������û��жϾ����Ƿ�ɹ�
//@EXECUTIONFLOW:17����ʾ�û��β忨���Σ�ÿ���Ƿ�����Ӧ�Ļص�������Ϣ�����У���ɹ�������ʧ��
//@EXECUTIONFLOW:18������CSUDISMCClose
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]�ϻ�����1��Сʱ���ݽ���������Լ��ϻ����Ժ����ܲ��� start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	

	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "�ò������������DVN CA");
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;	

	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}

	CSTCGetConfigAtr();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����5ʧ��");

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
					&nNumRead, auStatus),"����6�����ʧ��");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"����7�����ֵʧ��");
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
					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����6ʧ��:suantong datafix failure");
					 //nSendLen += 4;

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"����6�����ʧ��");
					 
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7.5ʧ��:suantong datafix failure");
					 nNumRead +=4;
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"����6�����ֵʧ��");
				}
				else if(nInputCardNum == EM_TEST_CONAX || nInputCardNum == EM_TEST_VIACCESS || nInputCardNum == EM_TEST_CryptoWorks)
				{
				    nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[i*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[i*2][1];
				    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����6�����ʧ��");
				    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead, auStatus),"����6�����ֵʧ��");
				}
				else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"����6�����ʧ��");
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����6.5ʧ��:COMVENIENT datafix failure");
					nNumRead +=4;
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"����6�����ֵʧ��");				    
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"����6�����ʧ��");					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"����6�����ֵʧ��");
				}
			}				
		}
		nTickAfterExchange = CSUDIPlusOSGetTickCount();
	
		nTickCostBefore =(nTickAfterExchange - nTickBeforeExchange)/MAX_TEST_CMD_NUM;
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]�ϻ�����1��Сʱ���ݽ�������ȴ�!\n");
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
						&nNumRead, auStatus),"����11���ݽ���ʧ��");
					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
						psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
						psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
						s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"����11���ݽ�������ֵʧ��");

					psCurLink = psCurLink->next;	
				}
			}
			else
			{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
						psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
						&nNumRead,nTimeOut),"����11��һ�η�������ʧ��");				
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
						s_ReData, 1,&nNumRead,nTimeOut),\
						"����11��һ�ν������ݷ���ֵʧ��");
					CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
						(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"����11��һ�ν�������ʧ��");
					psCurLink = psCurLink->next;	
						
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
						psCurLink->m_node->m_send.m_data, 5,\
						&nNumRead,nTimeOut),"����11�ڶ��η�������ʧ��");					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
						s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
						"����11�ڶ��ν������ݷ���ֵʧ��");
					CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"����11�ڶ��ν�������ʧ��");
					psCurLink = psCurLink->next;	

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
						s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
						"����11�����ν������ݷ���ֵʧ��");
					CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"����11�����ν�������ʧ��");
					psCurLink = psCurLink->next;
					
					/*����״̬��*/
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
						s_ReData, 2,&nNumRead,nTimeOut),\
						"����11���Ĵν������ݷ���ֵʧ��");
					CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"����11���Ĵν�������ʧ��");					
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
	 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����11ʧ��:suantong datafix failure");
						 //nSendLen += 4;
						
						 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"����11���ݽ���ʧ��");

						 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����11.2ʧ��:suantong datafix failure");
					    nNumRead +=4;
					}
					else if(nInputCardNum == EM_TEST_COMVENIENT)
					{
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����7ʧ��");
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7.5ʧ��:COMVENIENT datafix failure");
					 nNumRead +=4;					    
					}
					else
					{			
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����11���ݽ���ʧ��");
					}


			 		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, nNumRead, auStatus),"����11���ݽ�������ֵʧ��");
				}
				else/*DVN*/
				{
					nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[0][0] + g_DVNExchangeCmd[j].aaDVNCommand[0][1];
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"����11��һ�η�������ʧ��");
					/*����1���ֽ�*/
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"����11��һ�ν������ݷ���ֵʧ��");

					CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[j].aaDVNCommand[0][3]),"����11��һ�ν�������ʧ��");

					nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[1][0] + g_DVNExchangeCmd[j].aaDVNCommand[1][1];

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"����11�ڶ��η�������ʧ��");
					/*����5���ֽ�*/
					memset(aucResData, 0, sizeof(aucResData));
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"����ڶ��ν������ݷ���ֵʧ��");

					CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"����11�ڶ��ν�������ʧ��");

					nNumToRead = aucResData[4];
					memset(aucResData, 0, sizeof(aucResData));
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"��������ν������ݷ���ֵʧ��");

					CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"����11�����ν�������ʧ��");
					/*����״̬��*/
					memset(aucResData, 0, sizeof(aucResData));
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"����11���Ĵν������ݷ���ֵʧ��");

					CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"����11���Ĵν�������ʧ��");				
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
					&nNumRead, auStatus),"����14�����ʧ��");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"����14�����ֵʧ��");
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
					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����14ʧ��:suantong datafix failure");
					 //nSendLen += 4;

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"����14�����ʧ��");	

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����14.2ʧ��:suantong datafix failure");
					 nNumRead +=4;
					 
	 				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"����14�����ֵʧ��");
				}
				else if(nInputCardNum == EM_TEST_CONAX || nInputCardNum == EM_TEST_VIACCESS || nInputCardNum == EM_TEST_CryptoWorks)
				{
				    nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[m*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[m*2][1];
				    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[m*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����14�����ʧ��");
				    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[m*2][2]), nSendLen, aucResData, nNumRead, auStatus),"����14�����ֵʧ��");
				}
				else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"����6�����ʧ��");
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����6.5ʧ��:COMVENIENT datafix failure");
					nNumRead +=4;
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"����6�����ֵʧ��");				    
				}				
				else
				{			
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"����14�����ʧ��");
    				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus),"����14�����ֵʧ��");
				}
			}
		}
		nTickAfterExchange = CSUDIPlusOSGetTickCount();

		nTickCostAfter =(nTickAfterExchange - nTickBeforeExchange)/MAX_TEST_CMD_NUM;
		
		CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]�ϻ�ǰ�󽻻�����ʹ�õ�ʱ��ֱ���%d ms,%d ms\n",nTickCostBefore,nTickCostAfter);

		CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]���ж��ϻ�����������Ƿ��½�(û���½���Y,�½���N)\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����16ʧ��");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]�������ܿ�����(�ֱ���һ����һ��)�����鿴�ڴ��ڴ�ӡ���Ƿ��ܹ������ص�������⵽��In�Ϳ�outʱ�������Ϣ(�ǰ�Y,��N)\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����17ʧ��");
	
	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0068]�ϻ�����1��Сʱ���ݽ���������Լ��ϻ����Ժ����ܲ��� end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:���Կ�����û�в������ܿ��Ϳ���û�и�λ�����(ֻ���DVN CA)
//@PRECONDITION:1.��������Ч
//@PRECONDITION:2.������û�в������ܿ�
//@PRECONDITION:3.����֮ǰû�б��ɹ�reset
//@INPUT:unsigned char aucDataToWrite[5]={0};
//@INPUT:int nNumWrite;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = 5;
//@INPUT:pnNumberWrite = &nNumWrite;
//@INPUT:nTimeout = 0; 
//@EXPECTATION:û�忨ʱ�ӿڷ���CSUDISMC_ERROR_OUT���忨��ӿڷ���CSUDISMC_ERROR_NOT_RESET
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���ֵ
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5�����ýӿ�CSUDISMCSend
//@EXECUTIONFLOW:6���жϵ��岽����ֵ�Ƿ����CSUDISMC_ERROR_OUT
//@EXECUTIONFLOW:7����ʾ�û��������ܿ�
//@EXECUTIONFLOW:8�����ýӿ�CSUDISMCSend
//@EXECUTIONFLOW:9���жϵڰ˲�����ֵ�Ƿ����CSUDISMC_ERROR_NOT_RESET
//@EXECUTIONFLOW:10������CSUDISMCClose
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
		
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0069]���Կ�����û�в������ܿ��Ϳ���û�и�λʱ����CSUDISMCSend(��д�������:���������CA) start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����2ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);		
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum == EM_TEST_DVN, "�ò�������ֻ���DVN CA");

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0069]�뽫���Կ��Ӳ��Կ��۰γ��������������\n");
	CSTKWaitAnyKey();
	bRemoved = CSUDI_TRUE;

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSend(nInputCardSlotNum, aucDataToWrite, 5, &nNumWrite, nTimeOut),"����5ʧ��");
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0069]�뽫���Կ�������Կ���(�����������)\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSend(nInputCardSlotNum, aucDataToWrite, 5, &nNumWrite, nTimeOut),"����9ʧ��");	
	bRemoved = CSUDI_FALSE;

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}
	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("�뽫���Կ���ȷ������Կ���\n");
		CSTKWaitAnyKey();
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0069]���Կ�����û�в������ܿ��Ϳ���û�и�λʱ����CSUDISMCSend end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend
//@DESCRIPTION:���Է������ʱ�����(ֻ���DVN CA)
//@PRECONDITION:1.��������Ч
//@PRECONDITION:2.����֮ǰ�ɹ�reset
//@INPUT:unsigned char aucDataToWrite[]={�뿨ƥ�������};
//@INPUT:int nNumWrite;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = ��ʵ�ʵ��������;
//@INPUT:pnNumberWrite = &nNumWrite;
//@INPUT:nTimeout��ֵ����10ms; 
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_TIMEOUT
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5������CSUDISMCReset
//@EXECUTIONFLOW:6�������ò��Կ��������ţ�����뿨ƥ����������ܲ��Ե�����(100Bytes���ң�������ȫ��������)������CSUDISMCSend���������nTimeout��ֵΪ20ms
//@EXECUTIONFLOW:7���ж�CSUDISMCSend�ķ���ֵ�Ƿ���CSUDISMC_ERROR_TIMEOUT
//@EXECUTIONFLOW:8������CSUDISMCClose
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0070]���Է������ʱ(��д�������:���������CA) start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����2ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum == EM_TEST_DVN, "�ò�������ֻ���DVN CA");

 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;	
	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����5ʧ��");

	if(bSptFile)
	{
		sInfoLinkSMC *psCurLink = psLastNode;
		if(psCurLink->m_num == 4)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSend(nInputCardSlotNum,\
				psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len, &nNumWrite, nTimeOut),\
				"����7ʧ��");	
		}
	}
	else
	{
		nSendLen = g_TestCapCmd[nInputCardNum][0]+g_TestCapCmd[nInputCardNum][1];

		CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSend(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, &nNumWrite, nTimeOut),"����7ʧ��");
	}
	
	CSTK_FATAL_POINT;
	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");	
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_Send_0070]���Է������ʱ end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSend��CSUDISMCReceive
//@DESCRIPTION:���Գɹ����͡�������������(ֻ���DVN CA)
//@PRECONDITION:1.��������Ч
//@PRECONDITION:2.����֮ǰ�ɹ�reset
//@INPUT:unsigned char aucDataToWrite[]={�뿨ƥ�������};
//@INPUT:int nNumWrite;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteBuf = aucDataToWrite;
//@INPUT:nNumberToWrite = ��ʵ�ʵ��������;
//@INPUT:pnNumberWrite = &nNumWrite;
//@INPUT:nTimeout = 1000;  
//@EXPECTATION:�ӿڷ���CSUDI_SUCCESS��֮�����CSUDISMCReceive�ӿڣ�����Э���������ظ�����Ϣ��ȷ
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3���ж��Ƿ�DVN CA,�����ǲ��Խ������������������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5������CSUDISMCReset
//@EXECUTIONFLOW:6������CSUDISMCSend
//@EXECUTIONFLOW:7���ж�CSUDISMCSend�ķ���ֵ�Ƿ���CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDISMCReceive�ӿڣ�����Э���������ظ�����Ϣ�Ƿ���ȷ
//@EXECUTIONFLOW:9������CSUDISMCClose
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

	CSTCPrint("[CSTC_SMARTCARD_IT_SendReceice_0071]���Գɹ����͡���������(��д�������:���������CA) start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����2ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);		
 	CSTK_ASSERT_TRUE_FATAL(nInputCardNum == EM_TEST_DVN, "�ò�������ֻ���DVN CA");

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;	
	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����5ʧ��");

	CSAtrDecode(aucAtr,nAtrLen);
	
	if(bSptFile)
	{
		sInfoLinkSMC *psCurLink = psRootNode;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
				psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
				&nNumRead,nTimeOut),"����8��һ�η�������ʧ��");				
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
				s_ReData, 1,&nNumRead,nTimeOut),\
				"����8��һ�ν������ݷ���ֵʧ��");
			CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
				(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"����8��һ�ν�������ʧ��");
			psCurLink = psCurLink->next;	
				
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
				psCurLink->m_node->m_send.m_data, 5,\
				&nNumRead,nTimeOut),"����8�ڶ��η�������ʧ��");					
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
				s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
				"����8�ڶ��ν������ݷ���ֵʧ��");
			CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"����8�ڶ��ν�������ʧ��");
			psCurLink = psCurLink->next;	

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
				s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
				"����8�����ν������ݷ���ֵʧ��");
			CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"����8�����ν�������ʧ��");
			psCurLink = psCurLink->next;
			
			/*����״̬��*/
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
				s_ReData, 2,&nNumRead,nTimeOut),\
				"����8���Ĵν������ݷ���ֵʧ��");
			CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"����8���Ĵν�������ʧ��");	
	}
	else
	{
		for(i=0; i<MAX_TEST_CMD_NUM; i++)
		{
			memset(aucResData, 0, sizeof(aucResData));

			nSendLen = g_DVNExchangeCmd[i].aaDVNCommand[0][0] + g_DVNExchangeCmd[i].aaDVNCommand[0][1];
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[i].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"����7��һ�η�������ʧ��");
			/*����1���ֽ�*/
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"����8��һ�ν������ݷ���ֵʧ��");

			CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[i].aaDVNCommand[0][3]),"����8��һ�ν�������ʧ��");

			nSendLen = g_DVNExchangeCmd[i].aaDVNCommand[1][0] + g_DVNExchangeCmd[i].aaDVNCommand[1][1];

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[i].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"����7�ڶ��η�������ʧ��");
			/*����5���ֽ�*/
			memset(aucResData, 0, sizeof(aucResData));
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"����8�ڶ��ν������ݷ���ֵʧ��");

			CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"����8�ڶ��ν�������ʧ��");

			nNumToRead = aucResData[4];
			memset(aucResData, 0, sizeof(aucResData));
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"����8�����ν������ݷ���ֵʧ��");

			CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"����8�����ν�������ʧ��");
			/*����״̬��*/
			memset(aucResData, 0, sizeof(aucResData));
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"����8���Ĵν������ݷ���ֵʧ��");

			CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"����8���Ĵν�������ʧ��");		
		}	
	}

	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");	
	}
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SendReceice_0071]���Գɹ����͡��������� end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:����Atr��TA1����ʱ�������FI/DI��ֵ����TA1�����
//@PRECONDITION:1.���۸�λ�ɹ�
//@INPUT:unsigned char aucDataToWrite[4]={����TA1��ֵ����};
//@INPUT:unsigned char aucResData[4];
//@INPUT:int nNumRead;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWriteLen = 5;
//@INPUT:pucResponseData = aucResData;
//@INPUT:pnResponseDataLen = &nNumRead;
//@EXPECTATION:�ӿڷ���CSUDISMC_ERROR_OTHER
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen�򿪿��ۣ�
//@EXECUTIONFLOW:5������CSUDISMCReset��λ���ж�TA1�Ƿ���ڣ���������ڷ��ز��Գɹ�
//@EXECUTIONFLOW:6�����TA1���ڣ������CSUDISMCSetPPS������TA1+1��ֵ��Ϊ��FI/DIֵ
//@EXECUTIONFLOW:7���ж�CSUDISMCSetPPS�ķ���ֵ�Ƿ���CSUDISMC_ERROR_FAILURE��������Գɹ������������ʧ��
//@EXECUTIONFLOW:8������CSUDISMCClose�رտ��ۣ�
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0072]����Atr��TA1����ʱ������CSUDISMCSetPPS�������FI/DI��ֵ����TA1 start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);	
		
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack), "����4ʧ��");
	
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "����5ʧ��");

	if((aucAtr[1]& 0x10) != 0)
	{
		ucWriteData[0] = 0x10;
		ucWriteData[1] = aucAtr[2] + 0x01;
		nNumToWrite = 2;
		CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_FAILURE == CSUDISMCSetPPS(nInputCardSlotNum, ucWriteData, nNumToWrite, auResData, &nNumRead), "����7ʧ��");
	}
	
	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "������Դʧ��");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0072]���Գɹ����͡��������� end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCSetPPS
//@DESCRIPTION:����PPS�����ɹ������
//@PRECONDITION:1.��������Ч
//@PRECONDITION:2.���۳ɹ�reset
//@INPUT:unsigned char aucDataToWrite[4]={0x10,�����ļ��е���ʼFI/DIֵ};
//@INPUT:unsigned char aucResData[4];
//@INPUT:int nNumRead;
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = aucDataToWrite;
//@INPUT:nNumberToWriteLen = 5;
//@INPUT:pucResponseData = aucResData;
//@INPUT:pnResponseDataLen = &nNumRead;
//@EXPECTATION:�ӿڷ���CSUDI_SUCCESS
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3���������ļ��в�ѯPPS��������ʼFI/DI��ֵ
//@EXECUTIONFLOW:4������CSUDISMCSetParamsΪ����������ȷ�Ĳ�����
//@EXECUTIONFLOW:5������CSUDISMCOpen�򿪿��ۣ�
//@EXECUTIONFLOW:6������CSUDISMCReset��λ
//@EXECUTIONFLOW:7��ѭ������CSUDISMCSetPPS��FI/DI��ֵÿ�μ�0x01,ֱ������CSUDI_SUCCESSΪֹ���������0x11��û�гɹ��������ʧ��
//@EXECUTIONFLOW:8������CSUDISMCGetParams���жϷ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:9�����Գɹ�����ʾ�û�ʹ��ʾ��������Ƶ��
//@EXECUTIONFLOW:10�����ݲ��Գɹ�ʱ��FI/DI��ֵ������Ƿֱ��Ӧ��Fi��Diֵ,�ж�(Fi/Di)*������=Ƶ�ʵĵ�ʽ�Ƿ�������������Գɹ�������������ʧ��
//@EXECUTIONFLOW:11������CSUDISMCClose�رտ��ۣ�
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0073]����PPS�����ɹ������ start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);		

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey3, cResult3, nResultSize), "����3ʧ��\n");		
	ucInputFIDI = CSTKGetIntFromStr(cResult3, 10);		
	 
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack), "����5ʧ��");
	
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "����6ʧ��");
	
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

	CSTK_ASSERT_TRUE_FATAL(ucFIDIStart >= 0x11, "����7ʧ��");

	nATRDi = SmcGetDi(ucFIDIStart&0x0f);
	nATRFi = SmcGetFi((ucFIDIStart&0xf0) >> 4);

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0073]PPS�����ɹ�ʱ��Fi��Di�ֱ���:%d,%0d!\n", nATRFi, nATRDi);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDISMCGetParams(nInputCardSlotNum, &sCardParamGet)), "����8ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0073]��ǰ���ܿ��Ĳ�������%d����ʹ��ʾ��������Ƶ�ʣ��ж�(Fi/Di)*������=Ƶ�ʵĵ�ʽ�Ƿ����(�ǰ�Y,��N)\n", sCardParamGet.m_nBaudRate);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����10ʧ��");
	
	CSTK_FATAL_POINT;		

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "������Դʧ��");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_SetPPS_0073]����PPS�����ɹ������ end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCActive��CSUDISMCDeactive
//@DESCRIPTION:���Կ�����û�в������ܿ�������¼��ȥ����
//@PRECONDITION:1.������������Ч��
//@PRECONDITION:2.������û�в������ܿ�
//@INPUT:nCardIndex = �û�����Ŀ�����
//@EXPECTATION:����CSUDISMCActive��CSUDISMCDeactive�ӿ���ȷ���ظ��쳣���
//@EXECUTIONFLOW:1���������ļ��в�ѯ���ܿ��Ŀ���������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParamsΪ����������ȷ�Ĳ�����
//@EXECUTIONFLOW:4������CSUDISMCOpen�򿪿��ۣ�
//@EXECUTIONFLOW:5������CSUDISMCActive
//@EXECUTIONFLOW:6���ж�CSUDISMCActive�ķ���ֵ�Ƿ����CSUDISMC_ERROR_OUT
//@EXECUTIONFLOW:7������CSUDISMCDeactive
//@EXECUTIONFLOW:8���ж�CSUDISMCDeactive�ķ���ֵ�Ƿ����CSUDISMC_ERROR_OUT
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
	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0074]���Կ�����û�в������ܿ�������¼��ȥ���� start\n");
	
	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0074]�뽫���Կ��Ӳ��Կ��۰γ��������������\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);		
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");

	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_OUT == CSUDISMCActive(nInputCardSlotNum),"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_OUT == CSUDISMCDeactive(nInputCardSlotNum),"����8ʧ��");

	CSTK_FATAL_POINT;	

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}

	CSTCPrint("�뽫���Կ���ȷ������Կ���\n");
	CSTKWaitAnyKey();
	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0074]���Կ�����û�в������ܿ�������¼��ȥ���� end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCActive��CSUDISMCDeactive
//@DESCRIPTION:���Գɹ����ȥ��������
//@PRECONDITION:1.�ӿڸ�������Ч
//@INPUT:nCardIndex = �û�����Ŀ�����;
//@EXPECTATION:�ӿڷ���ֵ��ȷ��������״̬��ȷ
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParamsΪ����������ȷ�Ĳ�����
//@EXECUTIONFLOW:4������CSUDISMCOpen�򿪿��ۣ�
//@EXECUTIONFLOW:5������CSUDISMCActive���ж�CSUDISMCActive�ķ���ֵ�Ƿ����CSUDI_SUCCESS��
//@EXECUTIONFLOW:6����ʾ�û��������ж�RST��CLK��VPP��I/O��VCC�����ŵ�״̬�Ƿ���ȷ(RST:L,VCC:powered,I/O:reception,VPP:idle,CLK:�����ȶ�)
//@EXECUTIONFLOW:7������CSUDISMCReset�临λ,�ж�CSUDISMCReset�ķ���ֵʱ�����CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDISMCActive���ж�CSUDISMCActive�ķ���ֵ�Ƿ����CSUDI_SUCCESS��
//@EXECUTIONFLOW:9����ʾ�û��������ж�CLK��VPP��VCC�����ŵ�״̬�Ƿ���ȷ(VCC:powered,VPP:idle,CLK:�����ȶ�)��
//@EXECUTIONFLOW:10������CSUDISMCDeactive���ж�CSUDISMCDeactive�ķ���ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:11����ʾ�û��������ж�RST��CLK��VPP��I/O��VCC�����ŵ�״̬�Ƿ���ȷ(RST:L,VCC:inactive,I/O:state A,VPP:inactive,CLK:L)
//@EXECUTIONFLOW:12������CSUDISMCClose�رտ��ۣ�
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
	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]���Գɹ����ȥ��������,����ȷ�������ܿ� start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);		
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;		

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]�뽫ʾ����̽ͷ���Ӻ����ܿ�CLK��VCC��RST��VPP��I/O���ţ�׼��ץȡ�����Ų���(�����������):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCActive(nInputCardSlotNum),"����5ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]��������ж�RST��CLK��VPP��I/O��VCC�����ŵ�״̬�Ƿ���ȷ(RST:L,VCC:powered,I/O:reception,VPP:idle,CLK:�����ȶ�)���ǰ�Y����N");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����7ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]�뽫ʾ����̽ͷ���Ӻ����ܿ�CLK��VCC��RST��VPP��I/O���ţ�׼���ٴ�ץȡ�����Ų���(�����������):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCActive(nInputCardSlotNum),"����8ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]��������ж�CLK��VPP��VCC�����ŵ�״̬�Ƿ���ȷ(VCC:powered,VPP:idle,CLK:�����ȶ�)���ǰ�Y����N");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����9ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]�뽫ʾ����̽ͷ���Ӻ����ܿ�CLK��VCC��RST��VPP��I/O���ţ�׼���ٴ�ץȡ�����Ų���(�����������):\n");

	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDeactive(nInputCardSlotNum),"����10ʧ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]��������ж�RST��CLK��VPP��I/O��VCC�����ŵ�״̬�Ƿ���ȷ(RST:L,VCC:inactive,I/O:state A,VPP:inactive,CLK:L)���ǰ�Y����N");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����11ʧ��");

	CSTK_FATAL_POINT;		

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_ActiveDeactive_0075]���Գɹ����ȥ�������� end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:���Է��忨��ϵͳ���ȶ���
//@PRECONDITION:1.�����ԵĿ����в������ܿ�
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = �뿨ƥ�������;
//@INPUT:nNumberToWrite = ��ʵ������Ⱦ���;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:����ÿһ���Ĳ�������Ԥ�ڽ��
//@EXECUTIONFLOW:1���������ļ��в�ѯ���ܿ��Ŀ���������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParamsΪ����������ȷ�Ĳ�����
//@EXECUTIONFLOW:4������CSUDISMCOpen�򿪿��ۣ�
//@EXECUTIONFLOW:5������CSUDISMCReset��λ��
//@EXECUTIONFLOW:6��ʹ����ȷ���������CSUDISMCDataExchange�������ݽ�����
//@EXECUTIONFLOW:7���ε����ܿ�������Ƿ��лص�������
//@EXECUTIONFLOW:8����ʾ�û����������뿨�۲�����Ƿ��лص�������
//@EXECUTIONFLOW:9������CSUDISMCReset��λ������CSUDISMC_ERROR_MUTE��
//@EXECUTIONFLOW:10���γ����ܿ�������Ƿ��лص�������
//@EXECUTIONFLOW:11��������ȷ���뿨�ۣ�����Ƿ��лص�������
//@EXECUTIONFLOW:12������CSUDISMCReset��λ���ۣ��ɹ���λ��
//@EXECUTIONFLOW:13���ٴε���CSUDISMCDataExchange�������ݽ����������ɹ���
//@EXECUTIONFLOW:14������CSUDISMCClose�رտ��ۡ�
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

	CSTCPrint("[CSTC_SMARTCARD_IT_0076]���Է��忨��ϵͳ���ȶ��� start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;
	
	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����5ʧ��");

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
					&nNumRead, auStatus),"����6���ݽ���ʧ��");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"����6���ݽ�������ֵʧ��");

				psCurLink = psCurLink->next;	
			}
		}
		else
		{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
					&nNumRead,nTimeOut),"����6��һ�η�������ʧ��");				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 1,&nNumRead,nTimeOut),\
					"����6��һ�ν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
					(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"����6��һ�ν�������ʧ��");
				psCurLink = psCurLink->next;	
					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, 5,\
					&nNumRead,nTimeOut),"����6�ڶ��η�������ʧ��");					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"����6�ڶ��ν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"����6�ڶ��ν�������ʧ��");
				psCurLink = psCurLink->next;	

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"����6�����ν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"����6�����ν�������ʧ��");
				psCurLink = psCurLink->next;
				
				/*����״̬��*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 2,&nNumRead,nTimeOut),\
					"����6���Ĵν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"����6���Ĵν�������ʧ��");					
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
 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����6ʧ��:suantong datafix failure");
					 //nSendLen += 4;
					
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"����6���ݽ�����ֵ��ʧ��");			 

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7.5ʧ��:suantong datafix failure");
					 nNumRead +=4;
				}
                else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, &nNumRead, auStatus),"����6�����ʧ��");
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����6.5ʧ��:COMVENIENT datafix failure");
					nNumRead +=4;
				}				
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����6���ݽ�����ֵ��ʧ��");
				}
				
		 		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead, auStatus),"����6���ݽ���ʧ��");
			}
			else
			{
				CSTCPrint("[CSTC_SMARTCARD_IT_0076]nInputCardNum == EM_TEST_DVN!\n");

				nSendLen = g_DVNExchangeCmd[i].aaDVNCommand[0][0] + g_DVNExchangeCmd[i].aaDVNCommand[0][1];

				CSTCPrint("[CSTC_SMARTCARD_IT_0076]CSUDISMCSend before!\n");

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[i].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"����6��һ�η�������ʧ��");

				CSTCPrint("[CSTC_SMARTCARD_IT_0076]CSUDISMCSend after!\n");

				/*����1���ֽ�*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"����6��һ�ν������ݷ���ֵʧ��");

				CSTCPrint("[CSTC_SMARTCARD_IT_0076]CSUDISMCReceive after!\n");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[i].aaDVNCommand[0][3]),"����6��һ�ν�������ʧ��");

				nSendLen = g_DVNExchangeCmd[i].aaDVNCommand[1][0] + g_DVNExchangeCmd[i].aaDVNCommand[1][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[i].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"����6�ڶ��η�������ʧ��");
				/*����5���ֽ�*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"����6�ڶ��ν������ݷ���ֵʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"����6�ڶ��ν�������ʧ��");

				nNumToRead = aucResData[4];
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"����6�����ν������ݷ���ֵʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"����6�����ν�������ʧ��");
				/*����״̬��*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"����6���Ĵν������ݷ���ֵʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"����6���Ĵν�������ʧ��");		
			}
		}
	}

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_0076]��γ����ܿ�\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_OUT, "����7ʧ��");

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_0076]�뽫�������ܿ���������Կ���\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_IN, "����8ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDISMC_ERROR_MUTE == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "����9ʧ��");

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_0076]��γ����ܿ�\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_OUT, "����10ʧ��");
	bRemoved = CSUDI_TRUE;

	g_eSmcStatus = CSUDI_FAILURE;
	CSTCPrint("[CSTC_SMARTCARD_IT_0076]�뽫�������ܿ���ȷ������Կ���\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(g_eSmcStatus == CSUDISMC_ERROR_IN, "����11ʧ��");
	bRemoved = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����12ʧ��");

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
					&nNumRead, auStatus),"����13���ݽ���ʧ��");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"����13���ݽ�������ֵʧ��");

				psCurLink = psCurLink->next;	
			}
		}
		else
		{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
					&nNumRead,nTimeOut),"����6��һ�η�������ʧ��");				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 1,&nNumRead,nTimeOut),\
					"����6��һ�ν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
					(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"����6��һ�ν�������ʧ��");
				psCurLink = psCurLink->next;	
					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, 5,\
					&nNumRead,nTimeOut),"����6�ڶ��η�������ʧ��");					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"����6�ڶ��ν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"����6�ڶ��ν�������ʧ��");
				psCurLink = psCurLink->next;	

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"����6�����ν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"����6�����ν�������ʧ��");
				psCurLink = psCurLink->next;
				
				/*����״̬��*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 2,&nNumRead,nTimeOut),\
					"����6���Ĵν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"����11���Ĵν�������ʧ��");					
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
 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����13ʧ��:suantong datafix failure");
					 //nSendLen += 4;
					
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"����13���ݽ���ʧ��");			 

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7.5ʧ��:suantong datafix failure");
					 nNumRead +=4;
				}
				else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����13ʧ��");
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7.5ʧ��:COMVENIENT datafix failure");
					 nNumRead +=4;
				}				
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����13���ݽ���ʧ��");
				}

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, nNumRead, auStatus),"����13���ݽ�������ֵʧ��");
			}
			else
			{	
				nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[0][0] + g_DVNExchangeCmd[j].aaDVNCommand[0][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"����6��������ʧ��");
				/*����1���ֽ�*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"����6��������ʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[j].aaDVNCommand[0][3]),"����6��������ʧ��");

				nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[1][0] + g_DVNExchangeCmd[j].aaDVNCommand[1][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"����6��������ʧ��");
				/*����5���ֽ�*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"����6��������ʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"����6��������ʧ��");

				nNumToRead = aucResData[4];
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"����6��������ʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"����6��������ʧ��");
				/*����״̬��*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"����6��������ʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"����6��������ʧ��");		
			}
		}
	}
	CSTK_FATAL_POINT;		

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}
	if(CSUDI_TRUE == bRemoved)
	{
		CSTCPrint("�뽫���Կ���ȷ������Կ���\n");
		CSTKWaitAnyKey();
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_0076]���Է��忨��ϵͳ���ȶ��� end\n");

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
//@DESCRIPTION:��ο�ǿ�Ȳ���
//@PRECONDITION:��
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = �뿨ƥ�������;
//@INPUT:nNumberToWrite = ��ʵ������Ⱦ���;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:ǿ�Ȳ���֮�󣬸�λ�ɹ�����������ɹ�
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ��������ţ����������Ż�ñ�����ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3������CSUDISMCSetParamsΪ����������ȷ�Ĳ�����
//@EXECUTIONFLOW:4������CSUDISMCOpen�򿪿��ۣ�
//@EXECUTIONFLOW:5���������񣬲�ͣ��⿨״̬������⵽������ʱ����CSUDISMCReset�ȸ�λ
//@EXECUTIONFLOW:6����ʾ�������û���ο�����50��(����1��������,�����ε��ٲ�����һ��)
//@EXECUTIONFLOW:7�����50��֮�����CSUDISMCReset���жϸ�λ�Ƿ�ɹ������ɹ������ʧ��
//@EXECUTIONFLOW:8�������û��������ò��Կ����ڲ����������һ���������CSUDISMCDataExchange��CSUDISMCSend/CSUDISMCReceive���ж�ÿ�������Ƿ񽻻��ɹ��������ɹ������ʧ��
//@EXECUTIONFLOW:9������CSUDISMCClose�رտ��ۣ�
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

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]���Է�����ο���ϵͳ���ȶ��� start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	
 	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;	
	
	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}

	g_bThreadExitIT_0077 = CSUDI_TRUE;
	eRet = CSUDIOSThreadCreate("SMCTestThread", 7 , 1024, (CSUDIOSThreadEntry_F)SMCTestThreadEntry, &nInputCardSlotNum, &hThreadhandle);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == eRet) && (hThreadhandle != CSUDI_NULL),"����6�������ɹ����������������������Ϊ��");

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]���ο�����50��(�����ε��ٲ�����һ��),֮�����������\n");
	CSTKWaitAnyKey();
	g_bThreadExitIT_0077 = CSUDI_FALSE;
	CSUDIOSThreadSleep(1000);
	CSUDIOSThreadDestroy(hThreadhandle);

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]�뽫ʾ����̽ͷ���Ӻ����ܿ�CLK��VCC��RST���ţ�׼��ץȡ�����Ų���(��Y����):\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����7��λʧ��");
	CSTK_ASSERT_TRUE_FATAL((aucAtr[0]== 0x3B) || (aucAtr[0] == 0x3F),"����7ATR����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCaculatAtrLen(aucAtr, nAtrLen, &dwRLen, &ucTransfT),"����7����ʵ��Atr����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(dwRLen == nAtrLen,"����7ʧ��,ʵ�ʳ�������㳤�Ȳ����");

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]��ʹ��ʾ����������λ���Ρ�CLK��VCC��RST���ŵ�ƽ���Ƿ����ISO7816_123�临λ��׼(�ǰ�Y,��N):\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7��λ���δ���");

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
					&nNumRead, auStatus),"����8���ݽ���ʧ��");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRightEx(sCardParamSet.m_eProtocol,\
					psCurLink->m_node->m_send.m_data,psCurLink->m_node->m_send.m_len,\
					psCurLink->m_node->m_receive.m_data,psCurLink->m_node->m_receive.m_len,\
					s_ReData,nNumRead,psCurLink->m_node->m_status.m_data,auStatus),"����8���ݽ�������ֵʧ��");

				psCurLink = psCurLink->next;	
			}
		}
		else
		{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, psCurLink->m_node->m_send.m_len,\
					&nNumRead,nTimeOut),"����8��һ�η�������ʧ��");				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 1,&nNumRead,nTimeOut),\
					"����8��һ�ν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&\
					(s_ReData[0] == psCurLink->m_node->m_receive.m_data[0]),"����8��һ�ν�������ʧ��");
				psCurLink = psCurLink->next;	
					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, \
					psCurLink->m_node->m_send.m_data, 5,\
					&nNumRead,nTimeOut),"����8�ڶ��η�������ʧ��");					
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"����8�ڶ��ν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 5,"����8�ڶ��ν�������ʧ��");
				psCurLink = psCurLink->next;	

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, psCurLink->m_node->m_receive.m_len,&nNumRead,nTimeOut),\
					"����8�����ν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == psCurLink->m_node->m_receive.m_len,"����8�����ν�������ʧ��");
				psCurLink = psCurLink->next;
				
				/*����״̬��*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, \
					s_ReData, 2,&nNumRead,nTimeOut),\
					"����8���Ĵν������ݷ���ֵʧ��");
				CSTK_ASSERT_TRUE_FATAL(nNumRead == 2,"����8���Ĵν�������ʧ��");					
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
	 					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����8ʧ��:suantong datafix failure");
						 //nSendLen += 4;
					
					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"����8���ݽ���ʧ��");

					 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����8.5ʧ��:suantong datafix failure");
    				 nNumRead +=4;						 
				}
				else if(nInputCardNum == EM_TEST_COMVENIENT)
				{
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����8ʧ��");
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7.5ʧ��:COMVENIENT datafix failure");
					 nNumRead +=4;
				}					
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����8���ݽ���ʧ��");
				}
		 		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,&(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, nNumRead, auStatus),"����8���ݽ�������ֵʧ��");
			}
			else
			{
				nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[0][0] + g_DVNExchangeCmd[j].aaDVNCommand[0][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[0][2]), nSendLen, &nNumWrite, nTimeOut),"����8��һ�η�������ʧ��");
				/*����1���ֽ�*/
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 1, &nNumRead, nTimeOut),"����8��һ�ν������ݷ���ֵʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 1) &&(aucResData[0] == g_DVNExchangeCmd[j].aaDVNCommand[0][3]),"����8��һ�ν�������ʧ��");

				nSendLen = g_DVNExchangeCmd[j].aaDVNCommand[1][0] + g_DVNExchangeCmd[j].aaDVNCommand[1][1];

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSend(nInputCardSlotNum, &(g_DVNExchangeCmd[j].aaDVNCommand[1][2]), nSendLen, &nNumWrite, nTimeOut),"����8�ڶ��η�������ʧ��");
				/*����5���ֽ�*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 5, &nNumRead, nTimeOut),"����8�ڶ��ν������ݷ���ֵʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 5),"����8��������ʧ��");

				nNumToRead = aucResData[4];
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, nNumToRead, &nNumRead, nTimeOut),"����8�����ν������ݷ���ֵʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == nNumToRead),"����8�����ν�������ʧ��");
				/*����״̬��*/
				memset(aucResData, 0, sizeof(aucResData));
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReceive(nInputCardSlotNum, aucResData, 2, &nNumRead, nTimeOut),"����8���Ĵν������ݷ���ֵʧ��");

				CSTK_ASSERT_TRUE_FATAL((nNumRead == 2),"����8���Ĵν�������ʧ��");		
			}
		}
	}
	
	CSTK_FATAL_POINT;		

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_0077]���Է�����ο���ϵͳ���ȶ���  end\n");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDISMCDataExchange
//@DESCRIPTION:���ݽ���12Сʱǿ�Ȳ���(�����DVN CA)
//@PRECONDITION:��
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = �뿨ƥ�������;
//@INPUT:nNumberToWrite = ��ʵ������Ⱦ���;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:ǿ�Ȳ���֮�󣬸�λ�ɹ�����������ɹ�
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ���Э��ţ�����Э��Ż�ò��Կ������ż�������ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3���ж��Ƿ�DVN CA,���ǲ��Խ������������������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5������CSUDISMCReset
//@EXECUTIONFLOW:6�����ݲ��Կ���Э��ţ�����뿨ƥ���һ�����ڲ��Խ���������(������ȫ��������)����24Сʱ�ڶԸ�������ѭ������CSUDISMCDataExchange
//@EXECUTIONFLOW:7���ж�ÿ��CSUDISMCDataExchange����ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:8���жϸ���Э�����ÿ������ظ�����Ϣ�Ƿ���ȷ
//@EXECUTIONFLOW:9������CSUDISMCClose
//@EXECUTIONFLOW:10������CSUDISMCClose�رտ��ۣ�
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_0078]���ݽ���12Сʱǿ�Ȳ��� start\n");
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey1,cResult1,nResultSize), "����1ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1,10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nCardProtocalIndex = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey3,cResult3,nResultSize), "����2.5ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult3,10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "�ò������������DVN CA");

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack),"����4ʧ��");
	
	bOpened = CSUDI_TRUE;	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE),"����5ʧ��");

	CSAtrDecode(aucAtr,nAtrLen);
	
	dwStartTime = CSUDIPlusOSGetTickCount();
	
	while(dwTestTime <= (12*ONE_HOUR_MILLISECONDS))/*we have to test for 12 hours*/
	{
		if(nCardProtocalIndex == 0)//T0Э��
		{
			if(nInputCardNum != EM_TEST_TF)
			{
				for(j=0; j<MAX_TEST_CMD_NUM; j++)
				{
					memset(aucResData, 0, sizeof(aucResData));

					nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[j*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[j*2][1];
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����6-1���ݽ���ʧ��");								
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,  &(g_sExchangeCommand[nInputCardNum].aaCommand[j*2][2]), nSendLen, aucResData, nNumRead,  auStatus),"����7-1���ݽ�������ֵʧ��");
				}
			}
			else
			{
				for(j=0; j< sizeof(gsT0Command_TF)/sizeof(CaProtocalTestCommand_S); j++)
				{
					memset(aucResData, 0, sizeof(aucResData));					
					nSendLen = gsT0Command_TF[j].dwLen[0] + gsT0Command_TF[j].dwLen[1];
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, gsT0Command_TF[j].acCommand, nSendLen, aucResData, &nNumRead, auStatus),"����6-2���ݽ���ʧ��");
			 		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, gsT0Command_TF[j].acCommand, nSendLen, aucResData, nNumRead, auStatus),"����7-2���ݽ�������ֵʧ��");
				}
			}				
		}
		else if(nCardProtocalIndex ==1)//T1Э��
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
						 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����11ʧ��:suantong datafix failure");
						//nSendLen += 4;

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus),"����6-3���ݽ���ʧ��");			 

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7-4ʧ��:suantong datafix failure");
					nNumRead +=4;
							
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,  &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead,  auStatus),"����7-5���ݽ�������ֵʧ��");
				}
			}
			else if(nInputCardNum == EM_TEST_COMVENIENT)
			{
   				int i;
				for(i=0; i<MAX_TEST_CMD_NUM; i++)
				{
    				 nSendLen = g_sExchangeCommand[nInputCardNum].aaCommand[i*2][0]+g_sExchangeCommand[nInputCardNum].aaCommand[i*2][1];
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus),"����6-4ʧ��");
                     CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData,nNumRead,512),"����7-6ʧ��:COMVENIENT datafix failure");
					 nNumRead +=4;
 	 				 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol,  &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead,  auStatus),"����7-7ʧ��");
	 				}
			}				
			else
			{
				for(j=0; j< sizeof(gsT1Command)/sizeof(CaProtocalTestCommand_S); j++)
				{
					memset(aucResData, 0, sizeof(aucResData));					
					nSendLen = gsT1Command[j].dwLen[0] + gsT1Command[j].dwLen[1];
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, gsT1Command[j].acCommand, nSendLen, aucResData, &nNumRead, auStatus),"����6-5���ݽ���ʧ��");
				 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, gsT1Command[j].acCommand, nSendLen, aucResData, nNumRead, auStatus),"����7-8���ݽ�������ֵʧ��");
				}
			}
		}
		else if(nCardProtocalIndex ==2)//T14Э��
		{
			for(j=0; j< sizeof(gsT14Command)/sizeof(CaProtocalTestCommand_S); j++)
			{
				memset(aucResData, 0, sizeof(aucResData));					
				nSendLen = gsT14Command[j].dwLen[0] + gsT14Command[j].dwLen[1];
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, gsT14Command[j].acCommand, nSendLen, aucResData, &nNumRead, auStatus),"����6-6���ݽ���ʧ��");
			 	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, gsT14Command[j].acCommand, nSendLen, aucResData, nNumRead, auStatus),"����7-9���ݽ�������ֵʧ��");
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}	
	
	CSTCPrint("[CSTC_SMARTCARD_IT_0078]���ݽ���12Сʱǿ�Ȳ��� end\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSTC_SMARTCARD_IT_Exchange_0079
//@DESCRIPTION:������ָ���
//@PRECONDITION:��
//@INPUT:unsigned char aucResData[64];
//@INPUT:int nNumRead;
//@INPUT:unsigned int auStatus[64];
//@INPUT:nCardIndex = 0;
//@INPUT:pucWriteData = �뿨ƥ�������;
//@INPUT:nNumberToWrite = ��ʵ������Ⱦ���;
//@INPUT:pcResponseData = aucResData;
//@INPUT:pnNumberRead = &nNumRead;
//@INPUT:pucStatusWord = auStatus;
//@EXPECTATION:��������ɹ�
//@EXECUTIONFLOW:1���������ļ��в�ѯ����������
//@EXECUTIONFLOW:2���������ļ��в�ѯ���Կ���Э��ţ�����Э��Ż�ò��Կ������ż�������ȫ�ֱ����еĿ���ETU�������ʡ�Ƶ�ʡ���׼��Э�顢ֹͣλ��У��λ�Ȳ���
//@EXECUTIONFLOW:3���ж��Ƿ�TF CA��SUMAVISION CA,�����ǲ��Խ������������������CSUDISMCSetParams��������Կ��Ĳ���
//@EXECUTIONFLOW:4������CSUDISMCOpen
//@EXECUTIONFLOW:5������CSUDISMCReset
//@EXECUTIONFLOW:6�����ݲ��Կ���Э��ţ�����뿨ƥ���һ�����⽻��������
//@EXECUTIONFLOW:7���ж�ÿ��CSUDISMCDataExchange����ֵ�Ƿ����CSUDI_SUCCESS
//@EXECUTIONFLOW:8���ж�ÿ������ظ�����Ϣ�Ƿ���ȷ
//@EXECUTIONFLOW:9������CSUDISMCClose
//@EXECUTIONFLOW:10������CSUDISMCClose�رտ��ۣ�
BOOL CSTC_SMARTCARD_IT_Exchange_0079(void)
{
	/*-------for example:tf3.0--------------------
[=>: 0x80, 0x4e, 0x00, 0x03, 0x11, ]//����������Ϊ�գ�ֻ������״̬��
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
	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0079]����ָ����� start\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "����2ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection,pckey2,cResult2,nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2,10);	
	CSTK_ASSERT_TRUE_FATAL((nInputCardNum == EM_TEST_TF) || (nInputCardNum == EM_TEST_SUMAVISION),
		"�ò�������ֻ���TF CA��SUMAVISION CA");	

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBack), "����4ʧ��");
	
	bOpened = CSUDI_TRUE;	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "����5ʧ��");

	dwStartTime = CSUDIPlusOSGetTickCount();
	
	if(nInputCardNum == EM_TEST_TF)//TF card
	{
		static unsigned char ucSendData1[]={0x80, 0x4e, 0x00, 0x04, 0x11};
		static unsigned char ucSendData2[]={0x80, 0x4e, 0x00, 0x03, 0x11};		

		memset(aucResData, 0, sizeof(aucResData));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, ucSendData1, sizeof(ucSendData1)/sizeof(ucSendData1[0]), aucResData, &nNumRead, auStatus), "����6���ݽ���ʧ��");
		CSTK_ASSERT_TRUE_FATAL(nNumRead == 0, "����6���ݴ���ʧ��");
		
		memset(aucResData, 0, sizeof(aucResData));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, ucSendData2, sizeof(ucSendData2)/sizeof(ucSendData2[0]), aucResData, &nNumRead, auStatus), "����7���ݽ���ʧ��");
		CSTK_ASSERT_TRUE_FATAL(nNumRead == 0, "����7���ݴ���ʧ��");
	}
	else if(nInputCardNum == EM_TEST_SUMAVISION)//shumashixun card
	{
		static unsigned char ucSendData1[]={0x00, 0x20, 0x04, 0x02, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		memset(aucResData, 0, sizeof(aucResData));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCDataExchange(nInputCardSlotNum, ucSendData1, sizeof(ucSendData1)/sizeof(ucSendData1[0]), aucResData, &nNumRead, auStatus), "����6���ݽ���ʧ��");
		CSTCPrint("nNumRead=%d,readdata[0]=0x%02x,status=%02x %02x\n", nNumRead, aucResData[0], auStatus[0], auStatus[1]);
		CSTK_ASSERT_TRUE_FATAL(nNumRead == 0, "����7���ݴ���ʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "�ò�������ֻ���TF CA��SUMAVISION CA");
	}

	dwEndTime = CSUDIPlusOSGetTickCount();
	CSTK_ASSERT_TRUE_FATAL(dwEndTime - dwStartTime < 100, "����9����ʱ�����");

	CSTK_FATAL_POINT;

	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum), "������Դʧ��");
	}	
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0079]����ָ����� end\n");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSTC_SMARTCARD_IT_Exchange_0080
//@DESCRIPTION:��ָ����п��ٰο�,Ӧ������Ӧ(�����DVN CA)
//@PRECONDITION:��
//@EXPECTATION:��������ɹ�ÿ��ָ��ʧ�ܵ�ʱ��,��100ms�ڷ���û�忨�Ĵ���quick insert/remove card, the callback function: response time: < 100ms CSSMCQuickInOutCheck(100,E_QSET);��һ���������ðο������Ӧʱ��(ms)
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
   
	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0080]���ٰο�,Ӧ������Ӧ,��Ӧʱ�䲻����500ms\n");

	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey1, cResult1, nResultSize), "����2ʧ��\n");		
	nInputCardSlotNum = CSTKGetIntFromStr(cResult1, 10);	
	
	CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(pcSection, pckey2, cResult2, nResultSize), "����2ʧ��\n");		
	nInputCardNum = CSTKGetIntFromStr(cResult2, 10);	
	CSTK_ASSERT_TRUE_FATAL(nInputCardNum != EM_TEST_DVN, "�ò������������DVN CA");	

	sCardParamSet.m_eCheckBit = g_sCardPara[nInputCardNum].m_eCheckBit;
	sCardParamSet.m_eProtocol = g_sCardPara[nInputCardNum].m_eProtocol;
	sCardParamSet.m_eStandard = g_sCardPara[nInputCardNum].m_eStandard;
	sCardParamSet.m_eStopBit = g_sCardPara[nInputCardNum].m_eStopBit;
	sCardParamSet.m_nBaudRate = g_sCardPara[nInputCardNum].m_nBaudRate;
	sCardParamSet.m_nETU = g_sCardPara[nInputCardNum].m_nETU;
	sCardParamSet.m_nFrequency = g_sCardPara[nInputCardNum].m_nFrequency;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCSetParams(nInputCardSlotNum, &sCardParamSet), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCOpen(nInputCardSlotNum, CSSMCCallBackQuick), "����4ʧ��");

	bOpened = CSUDI_TRUE;	

	if(CS_TK_CONFIG_SUCCESS == CSTCGetConfigCmd())
	{
		bSptFile = CSUDI_TRUE;
	}

	CSTCGetConfigAtr();

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0080]��忨��ȴ���λ�ɹ�(��ӡ��<TKC> CSUDISMCReset OK!),�ٰο�. 30S�ڷ�����ο�,����10��.���������ʼ����\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCReset(nInputCardSlotNum, aucAtr, &nAtrLen, CSUDI_TRUE), "����5ʧ��");

	nTickBeforeExchange = CSUDIPlusOSGetTickCount();
	CSSMCQuickInOutCheck(0,E_QCLEAR);
	CSSMCQuickInOutCheck(100+MAX_TEST_EXCHANGE_TIME, E_QSET);// Ҫ���Ͽ�����ָ���ʱ��

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
        					 //CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS == CSDataFixForSuantong(command,nSendLen,512),"����6ʧ��:suantong datafix failure");
        					 //nSendLen += 4;

						CSSMCQuickInOutCheck(CSUDIPlusOSGetTickCount(), E_QSTART);
						if(CSUDI_SUCCESS != CSUDISMCDataExchange(nInputCardSlotNum, command, nSendLen, aucResData, &nNumRead, auStatus))
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "���ٰο�,��Ӧʱ�����,����7ʧ��");        					    
						}
						else
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData, nNumRead, 512), "����6.5ʧ��:suantong datafix failure");
							nNumRead +=4;

							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &g_TestCapCmd[nInputCardNum][2], nSendLen, aucResData, nNumRead, auStatus), "����6�����ֵʧ��");
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "���ٰο�,��Ӧʱ�����,����7ʧ��");
						}
					}
					else if(nInputCardNum == EM_TEST_COMVENIENT)
					{
						CSSMCQuickInOutCheck(CSUDIPlusOSGetTickCount(), E_QSTART);
						if(CSUDI_SUCCESS != CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus))
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "���ٰο�,��Ӧʱ�����,����7ʧ��");
						}
						else
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSDataFixForSuantong(aucResData, nNumRead, 512), "����7.5ʧ��:COMVENIENT datafix failure");
							nNumRead +=4;                            
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead, auStatus), "����6�����ֵʧ��");
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "���ٰο�,��Ӧʱ�����,����7ʧ��");
						}
					}        				
					else
					{	
						CSSMCQuickInOutCheck(CSUDIPlusOSGetTickCount(), E_QSTART);
						if(CSUDI_SUCCESS != CSUDISMCDataExchange(nInputCardSlotNum, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, &nNumRead, auStatus))    					
						{
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "���ٰο�,��Ӧʱ�����,����7ʧ��");
						}
						else
						{
							//�ο���,��������᷵�سɹ�,��ʵ���1����2���ֽ��Ѿ��Ǵ����,�����޷����������Ч��
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSCheckTranfIsRight(sCardParamSet.m_eProtocol, &(g_sExchangeCommand[nInputCardNum].aaCommand[i*2][2]), nSendLen, aucResData, nNumRead, auStatus), "����6�����ֵʧ��");
							CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_QCHECK), "���ٰο�,��Ӧʱ�����,����7ʧ��");
						}
					}
				}
			}
		}
		nTickAfterExchange = CSUDIPlusOSGetTickCount();	
		nTickCost = nTickAfterExchange - nTickBeforeExchange;
	}while(nTickCost < 30000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSSMCQuickInOutCheck(0, E_CNTCHECK), "���ٲ�δ���С��10��, ����8ʧ��");

	CSTK_FATAL_POINT;

	CSCAFsDestoryCmd();
	if(CSUDI_TRUE == bOpened)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISMCClose(nInputCardSlotNum),"������Դʧ��");
	}

	CSTCPrint("[CSTC_SMARTCARD_IT_Exchange_0080]���ٰο�����end\n");
	return CSUDI_TRUE;
}


