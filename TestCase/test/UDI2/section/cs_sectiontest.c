#include <stdlib.h>
#include "cs_sectiontest.h"
#include "cs_testkit.h"
#include "udiplus_debug.h"
#include "udiplus_typedef.h"

typedef struct
{
	unsigned char* 	pucTestData;
	int 			nDataLen;
}DUMPTESTData_S;

//�����ݴ�EPG_0606_121458.ts�����з�������
static unsigned char s_TestPATDATA[128]=
{
	0x00, 0xB0, 0x39, 0x00, 0x0B, 0xD7, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x10, 0x00, 0x70, 0xE4, 0x65,\
	0x02, 0x2C, 0xF5, 0xBD, 0x02, 0x2B, 0xF5, 0xB3, 0x00, 0x2D, 0xE1, 0xC7, 0x00, 0xEC, 0xE9, 0x3D,\
	0x00, 0x2A, 0xE1, 0xA9, 0x00, 0x31, 0xE1, 0xEF, 0x00, 0x72, 0xE4, 0x79, 0x01, 0xCE, 0xF2, 0x11,\
	0x01, 0xCD, 0xF2, 0x07, 0x00, 0x71, 0xE4, 0x6F, 0xC9, 0xC4, 0x6E, 0xD5
};
//�����ݴ�EPG_0606_121458.ts�����з�������
static unsigned char s_TestTDTDATA[] =
{
	0x70 ,0x70 ,0x05 ,0xd3 ,0xf1 ,0x04
};
//�����ݴ�EPG_0606_121458.ts�����з�������
static unsigned char s_TestCATDATA[]=
{
	0x01 ,0xb0 ,0x2c ,0xff ,0xff ,0xc5 ,0x00 ,0x00 ,
	0x09 ,0x21 ,0x01 ,0x00 ,0xe0 ,0xdc ,0x07 ,0x80 ,
	0xe2 ,0x01 ,0x2d ,0x80 ,0xdd ,0x00 ,0x93 ,0x80 ,
	0xe3 ,0x01 ,0x2e ,0x80 ,0xe4 ,0x01 ,0x2f ,0x80 ,
	0xe5 ,0x01 ,0x30 ,0x80 ,0xe6 ,0x01 ,0x31 ,0x80 ,
	0xe7 ,0x01 ,0x32 ,0xfa ,0x9f ,0xc7 ,0x14 
};

//�����ݴ�EPG_0606_121458.ts�����з�������
static unsigned char s_TestEIT_SECTION0DATA[]=
{
	0x4f ,0xf0 ,0x72 ,0x00 ,0x66 ,0xc1 ,0x00 ,0x01 ,
	0x00 ,0x01 ,0x40 ,0x80 ,0x01 ,0x4f ,0x00 ,0x28 ,
	0xd3 ,0xb8 ,0x10 ,0x12 ,0x00 ,0x00 ,0x28 ,0x00 ,
	0x80 ,0x57 ,0x4d ,0x51 ,0x63 ,0x68 ,0x69 ,0x16
};
//�����ݴ�EPG_0606_121458.ts�����з�������
static unsigned char s_TestEIT_SECTION1DATA[]=
{
	0x4f ,0xf0 ,0x73 ,0x00 ,0x66 ,0xc1 ,0x01 ,0x01 ,
	0x00 ,0x01 ,0x40 ,0x80 ,0x01 ,0x4f ,0x00 ,0x29 ,
	0xd3 ,0xb8 ,0x10 ,0x40 ,0x00 ,0x00 ,0x28 ,0x00 ,
	0x80 ,0x58 ,0x4d ,0x52 ,0x63 ,0x68 ,0x69 ,0x08
};
/*{{{--------------Coship 2011-5-6 the data attached by houwenrui--------------------*/
//�����ݴ�Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts�����з�������
static unsigned char s_TestEIT_SECTION2DATA[PSISI_LENGTH][DATA_LEN]=
{
	{0x4E,0xF0,0x8C,0x07,0xF0,0xC7,0x01,0x01,0x00,0x67,0x00,0x64,0x01,0x4E,0x85,0xC3},
	{0x4E,0xF0,0x94,0x07,0xF0,0xC7,0x00,0x01,0x00,0x67,0x00,0x64,0x01,0x4E,0x85,0xC2},
	{0x4E,0xB1,0x22,0x07,0xEF,0xC3,0x00,0x00,0x00,0x67,0x00,0x64,0x00,0x4E,0x03,0xC4}
};
//�����ݴ�Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts�����з�������
static unsigned char s_TestSUBTITLE1DATA[PES_LENGTH][DATA_LEN]=
{
	{0x00,0x00,0x01,0xBD,0x0F,0x61,0x84,0x80,0x05,0x27,0xA9,0x15,0xD0,0x25,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x0C,0x99,0x84,0x80,0x05,0x27,0xA9,0x3B,0x37,0x15,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x00,0x19,0x84,0x80,0x05,0x27,0xA9,0x47,0x5E,0x15,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x15,0xC3,0x84,0x80,0x05,0x27,0xA9,0x4D,0x0F,0x25,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x10,0x55,0x84,0x80,0x05,0x27,0xA9,0x57,0xD3,0xB5,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x16,0x49,0x84,0x80,0x05,0x27,0xA9,0x65,0x32,0xF5,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x16,0xCF,0x84,0x80,0x05,0x27,0xA9,0x75,0xE0,0xD5,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x00,0x19,0x84,0x80,0x05,0x27,0xA9,0x89,0xA7,0xF5,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x0E,0xFD,0x84,0x80,0x05,0x27,0xA9,0x8D,0xF6,0x95,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x0E,0x09,0x84,0x80,0x05,0x27,0xA9,0x95,0xEB,0x15,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x0F,0xEB,0x84,0x80,0x05,0x27,0xA9,0xA1,0xD9,0xD5,0x20,0x00},
	{0x00,0x00,0x01,0xBD,0x1A,0x07,0x84,0x80,0x05,0x27,0xA9,0xAF,0x71,0x55,0x20,0x00}
};
/*--------------Coship 2011-5-6 the data attached by houwenrui--------------------}}}*/
//�����ݴ�Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts�����з�������
static unsigned char s_TestSUBTITLEDATA[]=
{
	0x00,0x00,0x01,0xbd/*packet_start_code+stream_id*/ 
};
//�����ݴ�Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts�����з�������
static unsigned char s_TestTELETEXTDATA[]=
{
	0x00,0x00,0x01,0xbd,0x05,0x02,/*packet_start_code+stream_id*/ 
};

//�����ݴ�EPG_0606_121458.ts�����з�������,��program_numberΪ112��PMT����
static unsigned char s_TestPMTProg1DATA[]=
{
	0x02, 0xB0,0x1d,0x00,0x70,0xC7,0x00,0x00,0xE4,0x60,
};

//�����ݴ�EPG_0606_121458.ts�����з�������,��program_numberΪ556��PMT����
static unsigned char s_TestPMTProg2DATA[]=
{
	0x02, 0xB0,0x1d,0x02,0x2C,0xC1,0x00,0x00,0xff,0xff,
};

//�����ݴ�EPG_0606_121458.ts�����з�������,��program_numberΪ555��PMT����
static unsigned char s_TestPMTProg3DATA[]=
{
	0x02, 0xB0,0x33,0x02,0x2B,0xf7,0x00,0x00,0xff,0xff,0xf0,0x00,0x02,0xf5,0xae,0xf0,0x00,
};


//�����ݴ������з�������
CSUDIFILTERMatchMask_S g_sMMFilter[EM_SECTION_FILTER_MAX] = { \
	{
		{0x00},
		{0xff},
		{0x00}
	},  /*PAT_FILTER001*/	 
	{
		{0xff},
		{0xff},
		{0x01}
	}, /*PAT_FILTER002*/ 
	{
		{0x00},
		{0xff},
		{0x01}
	}, /*PAT_FILTER003*/	 
	{
		{0x00,0x00,0x0b},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*PAT_FILTER004*/	
	{
		{0x00,0x00,0x0b,0xd7},
		{0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00}
	},/*PAT_FILTER005*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00},
		{0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x00}
	},/*PAT_FILTER006*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00},
		{0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x01}
	},/*PAT_FILTER007*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00},
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
	},/*PAT_FILTER008*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00},
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01}
	},/*PAT_FILTER009*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10},
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
	},/*PAT_FILTER010*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10},
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01}
	},/*PAT_FILTER011*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70},
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
	},/*PAT_FILTER012*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70},
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01}
	},/*PAT_FILTER013*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70,0xe4,0x65},
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
	},/*PAT_FILTER014*/	
	{
		{0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70,0xe4,0x65},
		{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01}
	},/*PAT_FILTER015*/	
	{
		{0},
		{0},
		{0}
	},/*PAT_FILTER_MAX*/
	{
		{0x70},
		{0xff},
		{0x00}
	},/*TDT_FILTER_001*/	
	{
		{0x01},
		{0xff},
		{0x00}
	},/*CAT_FILTER001*/
	{
		{0x4f,0x00,0x66},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*EIT_FILTER001*/
	{
		{0x4f,0x00,0x66,0xc1,0x00},
		{0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00}
	},/*EIT_FILTER002*/
	{
		{0x4f,0x00,0x66,0xc1,0x01},
		{0xff,0xff,0xff,0xff,0xff},
		{0x00,0x00,0x00,0x00,0x00}
	},/*EIT_FILTER003*/	
	{
		{0x60},
		{0xf0},
		{0x00}
	},/*EIT_FILTER004*/	 
	{
		{0x0},
		{0x0},
		{0x0}
	},/*EIT_FILTER_MAX*/
	{
		{0x3c,0x00,0x01},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*OC_FILTER001*/
	{
		{0x3c,0x00,0x02},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*OC_FILTER002*/
	{
		{0x3c,0x00,0x03},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*OC_FILTER003*/
	{
		{0x3c,0x00,0x04},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*OC_FILTER004*/
		{
		{0x3c,0x00,0x05},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*OC_FILTER005*/
		{
		{0x3c,0x00,0x06},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*OC_FILTER006*/
		{
		{0x3c,0x00,0x07},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*OC_FILTER007*/
		{
		{0x3c,0x00,0x08},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*OC_FILTER008*/
		{
		{0x3c,0x00,0x09},
		{0xff,0xff,0xff},
		{0x00,0x00,0x00}
	},/*OC_FILTER009*/
	{
		{0x0},
		{0x0},
		{0x0}
	},/*OC_FILTER_MAX*/
	{
		{0x0},
		{0x0},
		{0x0}
	},/*EM_SECTION_PES_VIDEO_001*/
	{
		{0x0},
		{0x0},
		{0x0}
	},/*EM_SECTION_PES_VIDEO_002*/
	{
		{0x0},
		{0x0},
		{0x0}
	},/*EM_SECTION_PES_AUDIO_001*/
	{
		{0x0},
		{0x0},
		{0x0}
	},/*EM_SECTION_PES_AUDIO_002*/
	{
		{0x0},
		{0x0},
		{0x0}
	},/*EM_SECTION_PES_SUBTITLE_001*/

	{
		{0x0},
		{0x0},
		{0x0}
	},/*EM_SECTION_PES_SUBTITLE_002*/
	{
		{0x0},
		{0x0},
		{0x0}
	},/*EM_SECTION_PES_TELETEXT_001*/
	{
		{0x0},
		{0x0},
		{0x0}
	},/*EM_SECTION_PES_TELETEXT_002*/
	{
		{0x0},
		{0x0},
		{0x0}
	},/*EM_SECTION_PES_MAX*/
	{
		{0x02, 0x00,0x70},
		{0xff, 0xff,0xff},
		{0x00, 0x00,0x00}
	},/*EM_SECTION_PMT_FILTER_001*///��Խ�Ŀ1
	{
		{0x02, 0x02,0x2C},
		{0xff, 0xff,0xff},
		{0x00, 0x00,0x00}
	},/*EM_SECTION_PMT_FILTER_002*///��Խ�Ŀ2
	{
		{0x02, 0x02,0x2B},//2B
		{0xff, 0xff,0xff},
		{0x00, 0x00,0x00}
	},/*EM_SECTION_PMT_FILTER_003*///��Խ�Ŀ3
	{
		{0x02, 0x02,0x70,0x00/*version*/,0x00,0x00,0xE4,0x70},
		{0xff, 0xff,0xff,0x3e,0xff,0xff,0xff,0xff},
		{0x00, 0x01,0x00,0x01,0x00,0x00,0x00,0x01}
	},/*EM_SECTION_PMT_FILTER_004*///��Խ�Ŀ1�����������˵����
	{
		{0x02, 0x00,0x2B,0xf7,0x02,0x02,0x04,0x70},
		{0xff, 0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00, 0x01,0x01,0x01,0x01,0x01,0x01,0x01}
	},/*EM_SECTION_PMT_FILTER_005*///��Խ�Ŀ2
	{
		{0x02, 0x00,0xC0,0x00,0x00,0x00,0xff, 0xff,0xf0,0x00,0x02,0xf5,0xB8,0xf0,0x00},
		{0xff, 0xff,0xf0,0xff,0xff,0xff,0xff, 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
		{0x00, 0x01,0x01,0x01,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00}
	}/*EM_SECTION_PMT_FILTER_006*///��Խ�Ŀ3(555)
};

//���ڴ洢ĳ������ʹ�õ�FilterIndex��userdata��Ϊ��������±�
//��ֱ����userdata����Ϊ���ܴ���һ��DMXͬʱ�����ڼ���������
static int			s_nDMXFilterIndex[EM_SECTION_FILTER_MAX];

static CSUDI_BOOL	s_bSectionCallbackSuc[EM_SECTION_FILTER_MAX];//���뵽���ݲ���ͨ����֤

static CSUDI_BOOL	s_bSectionCallbackRuq[EM_SECTION_FILTER_MAX];//ֻ�����뵽����

static DUMPTESTData_S	s_sTestData[EM_SECTION_FILTER_MAX]; //ͨ����������������ȡ�������filter�ص����������жϵ�����

static int  		s_nTaskP01Rst = 0;//0-��ʼֵ 1-�ɹ� ����-ʧ��
static int  		s_nTaskP02Rst = 0;
static int  		s_nTaskP03Rst = 0;

static int  		s_nTaskR01Rst     = 0;//TASK R01����ֵ  0-��ʼֵ 1-�ɹ� ����-ʧ��
static int  		s_nTaskR02Rst     = 0;
static int  		s_nTaskR03Rst     = 0;
static int  		s_nTaskR04Rst     = 0;

static CSUDI_BOOL s_bPSISI_PES_Flag_Init = CSUDI_FALSE;

void SECTION_iInitPowerCasePar()
{
	s_nTaskP01Rst = 0;
	s_nTaskP02Rst = 0;
	s_nTaskP03Rst = 0;
}
void SECTION_iInitRequestCasePar()
{
	s_nTaskR01Rst	   = 0;
	s_nTaskR02Rst      = 0;
	s_nTaskR03Rst      = 0;
	s_nTaskR04Rst      = 0;
}
void SECTION_iTestInit()
{
	//PAT_FILTER001 data
	s_sTestData[EM_SECTION_PAT_FILTER_001].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_001].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_002].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_002].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_003].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_003].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_004].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_004].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_005].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_005].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_006].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_006].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_007].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_007].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_008].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_008].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_009].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_009].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_010].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_010].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_011].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_011].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_012].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_012].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_013].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_013].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_014].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_014].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
	
	s_sTestData[EM_SECTION_PAT_FILTER_015].pucTestData = s_TestPATDATA;
	s_sTestData[EM_SECTION_PAT_FILTER_015].nDataLen = sizeof(s_TestPATDATA)/sizeof(char);
    
	//TDT_FILTER001 data
	s_sTestData[EM_SECTION_TDT_FILTER_001].pucTestData = s_TestTDTDATA;
	s_sTestData[EM_SECTION_TDT_FILTER_001].nDataLen = sizeof(s_TestTDTDATA)/sizeof(char);	

	//CAT_FILTER001 data
	s_sTestData[EM_SECTION_CAT_FILTER_001].pucTestData = s_TestCATDATA;
	s_sTestData[EM_SECTION_CAT_FILTER_001].nDataLen = sizeof(s_TestCATDATA)/sizeof(char);

	//EIT_FILTER001 data ��filter�õ�����section�����ݣ��ֱ�Ϊs_TestEIT_SECTION0DATA ��s_TestEIT_SECTION1DATA

	//EIT_FILTER002 data
	s_sTestData[EM_SECTION_EIT_FILTER_002].pucTestData = s_TestEIT_SECTION0DATA;
	s_sTestData[EM_SECTION_EIT_FILTER_002].nDataLen = sizeof(s_TestEIT_SECTION0DATA)/sizeof(char);  
	
	//EIT_FILTER003 data
	s_sTestData[EM_SECTION_EIT_FILTER_003].pucTestData = s_TestEIT_SECTION1DATA;
	s_sTestData[EM_SECTION_EIT_FILTER_003].nDataLen = sizeof(s_TestEIT_SECTION1DATA)/sizeof(char); 
	
	//SUBTITLE_001 data
	s_sTestData[EM_SECTION_PES_SUBTITLE_001].pucTestData = s_TestSUBTITLEDATA;
	s_sTestData[EM_SECTION_PES_SUBTITLE_001].nDataLen = sizeof(s_TestSUBTITLEDATA)/sizeof(char);

	//TELETEXT_001 data
	s_sTestData[EM_SECTION_PES_TELETEXT_001].pucTestData = s_TestTELETEXTDATA;
	s_sTestData[EM_SECTION_PES_TELETEXT_001].nDataLen = sizeof(s_TestTELETEXTDATA)/sizeof(char);

	//PMT_FILTER_001 data
	s_sTestData[EM_SECTION_PMT_FILTER_001].pucTestData = s_TestPMTProg1DATA;
	s_sTestData[EM_SECTION_PMT_FILTER_001].nDataLen = sizeof(s_TestPMTProg1DATA)/sizeof(char);

	//PMT_FILTER_002 data
	s_sTestData[EM_SECTION_PMT_FILTER_002].pucTestData = s_TestPMTProg2DATA;
	s_sTestData[EM_SECTION_PMT_FILTER_002].nDataLen = sizeof(s_TestPMTProg2DATA)/sizeof(char);

	//PMT_FILTER_003 data
	s_sTestData[EM_SECTION_PMT_FILTER_003].pucTestData = s_TestPMTProg3DATA;
	s_sTestData[EM_SECTION_PMT_FILTER_003].nDataLen = sizeof(s_TestPMTProg3DATA)/sizeof(char);

	//PMT_FILTER_004 data
	s_sTestData[EM_SECTION_PMT_FILTER_004].pucTestData = s_TestPMTProg1DATA;
	s_sTestData[EM_SECTION_PMT_FILTER_004].nDataLen = sizeof(s_TestPMTProg1DATA)/sizeof(char);

	//PMT_FILTER_005 data
	s_sTestData[EM_SECTION_PMT_FILTER_005].pucTestData = s_TestPMTProg2DATA;
	s_sTestData[EM_SECTION_PMT_FILTER_005].nDataLen = sizeof(s_TestPMTProg2DATA)/sizeof(char);

	//PMT_FILTER_006 data
	s_sTestData[EM_SECTION_PMT_FILTER_006].pucTestData = s_TestPMTProg3DATA;
	s_sTestData[EM_SECTION_PMT_FILTER_006].nDataLen = sizeof(s_TestPMTProg3DATA)/sizeof(char);

	//EM_SECTION_PSISI_PES_FILTER_PSISI data
	s_sTestData[EM_SECTION_PSISI_PES_FILTER_PSISI].pucTestData = (unsigned char*)s_TestEIT_SECTION2DATA;
	s_sTestData[EM_SECTION_PSISI_PES_FILTER_PSISI].nDataLen = sizeof(s_TestEIT_SECTION2DATA)/sizeof(char);

	//EM_SECTION_PSISI_PES_FILTER_PES data
	s_sTestData[EM_SECTION_PSISI_PES_FILTER_PES].pucTestData = (unsigned char*)s_TestSUBTITLE1DATA;
	s_sTestData[EM_SECTION_PSISI_PES_FILTER_PES].nDataLen = sizeof(s_TestSUBTITLE1DATA)/sizeof(char);

}

CSUDI_BOOL SECTION_iSetMatchMask(CSUDIFILTERMatchMask_S * psMatch, int nFilterIndex)
{
	if((nFilterIndex >= EM_SECTION_FILTER_MAX) || (psMatch == CSUDI_NULL))
	{
		return CSUDI_FALSE;
	}
	memset(psMatch, 0, sizeof(CSUDIFILTERMatchMask_S));
	memcpy(psMatch,&g_sMMFilter[nFilterIndex],sizeof(CSUDIFILTERMatchMask_S));	
	s_nDMXFilterIndex[nFilterIndex] = nFilterIndex;
	return CSUDI_TRUE;
}

/*
* �������� section���ݵ�filter�����g_sMMFilter����
* �����TDT����CRCCheck����ΪCSUDI_TRUE���������CRCCheck����ΪCSUDI_TRUE
* ����psReqInfoΪCSUDI_NULL����ʱֻ������s_nDMXFilterIndex��ֵ
*/
CSUDI_BOOL SECTION_iSetSectionFilter(CSUDISECTIONRequestInfo_S * psReqInfo,int nFilterIndex ,int nDemuxIndex,int nPid)
{
	if(nFilterIndex >= EM_SECTION_FILTER_MAX) 
	{
		return CSUDI_FALSE;
	}
	
	if(psReqInfo != CSUDI_NULL)
	{
		memset(psReqInfo,0,sizeof(CSUDISECTIONRequestInfo_S));
		
		psReqInfo->m_eReqType = EM_UDI_REQ_SECTION_DATA;
		psReqInfo->m_nDemuxID = nDemuxIndex;
		psReqInfo->m_nPID = nPid;
		
		memcpy(&psReqInfo->m_sMMFilter,&g_sMMFilter[nFilterIndex],sizeof(psReqInfo->m_sMMFilter));
		
		if(EM_SECTION_TDT_FILTER_001 == nFilterIndex)
		{
			psReqInfo->m_bCRCCheck = CSUDI_FALSE;
		}
		else
		{
			psReqInfo->m_bCRCCheck = CSUDI_TRUE;
		}
	}
	
	s_nDMXFilterIndex[nFilterIndex] = nFilterIndex;
	
	return CSUDI_TRUE; 
}

CSUDI_BOOL SECTION_iSetPESReq(CSUDISECTIONRequestInfo_S * psReqInfo,int nFilterIndex ,int nDemuxIndex,int nPid)
{
	if(psReqInfo != CSUDI_NULL)
	{
		memset(psReqInfo,0,sizeof(CSUDISECTIONRequestInfo_S));
		psReqInfo->m_eReqType = EM_UDI_REQ_PES_DATA;
		psReqInfo->m_nDemuxID = nDemuxIndex;
		psReqInfo->m_nPID = nPid;	
		psReqInfo->m_bCRCCheck = CSUDI_TRUE;
	}
	s_nDMXFilterIndex[nFilterIndex] = nFilterIndex;
	return CSUDI_TRUE; 
}


/*
*filter�Ļص��������˺������ж����ݵ���ȷ��
*/
void SECTION_iCallback_1( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	int nFilterIndex = 0xff;
	int i;
	
   	if(pSection == CSUDI_NULL || (int)pvUserData >= EM_SECTION_FILTER_MAX)
	{
		return ;
	}
	
	nLength  = pSection->m_nDataLen;
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	
	if(s_bSectionCallbackRuq[nFilterIndex]) //����Ѿ����������򷵻أ�����ȥ�ظ�У��
	{
		return;
	}
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
			{
				CSTCPrint( "��ȡ��SECTION_FILTER%03d ��Section���� data callback length = %d*****\n",nFilterIndex+1,nLength);
				CSTCPrint( "SECTION_FILTER%03d receives section data callback length = %d*****\n",nFilterIndex+1,nLength);
				
				SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_TRUE);  //���ûص��������ݵ���ʱû��У��

				nLength = (nLength < s_sTestData[nFilterIndex].nDataLen) ? nLength : s_sTestData[nFilterIndex].nDataLen;
				
				nLength = (nLength < 32) ? nLength : 32;

				for( i = 0; i < nLength; ++ i)
				{
					if( pSection->m_pucData[i] != s_sTestData[nFilterIndex].pucTestData[i])
					{
						CSTCPrint( "\n�����ڵ�%d ���ֽڱȽ�ʱ��ͨ��! ��������Ϊ%02x,��ȷ����Ϊ%02x\n",\
									i+1,pSection->m_pucData[i],s_sTestData[nFilterIndex].pucTestData[i]);
						CSTCPrint( "\nThe %dth byte of the section is not correct! Received data: %02x,Expected data: %02x\n",\
									i+1,pSection->m_pucData[i],s_sTestData[nFilterIndex].pucTestData[i]);
						return ;
					}
				}
				CSTCPrint( "\nFILTER%03d ������֤ͨ��***********************************\n",nFilterIndex+1);
				CSTCPrint( "\nFILTER%03d passes the checking***********************************\n",nFilterIndex+1);
				SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_TRUE,CSUDI_TRUE ); //���ûص�����������У��ɹ�
			}
			break;
	case EM_UDI_REQ_PES_DATA:
		{
			CSTCPrint( "��ȡ��PES_FILTER%03d ��pes����[%2x %2x %2x] data callback length = %d*****\n",
				nFilterIndex+1,pSection->m_pucData[0],pSection->m_pucData[1],pSection->m_pucData[2],
				nLength);
			CSTCPrint( "PES_FILTER%03d receives pes data [%2x %2x %2x] data callback length = %d*****\n",
				nFilterIndex+1,pSection->m_pucData[0],pSection->m_pucData[1],pSection->m_pucData[2],
				nLength);
			
			SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_TRUE);  //���ûص��������ݵ���ʱû��У��
			if(nFilterIndex >= EM_SECTION_PES_SUBTITLE_001 && nFilterIndex < EM_SECTION_PES_MAX)
			{
				///����pes(subtitle,teletext data)
				nLength = (nLength < s_sTestData[nFilterIndex].nDataLen) ? nLength : s_sTestData[nFilterIndex].nDataLen;
				
				nLength = (nLength < 32) ? nLength : 32;

				for( i = 0; i < nLength; ++ i)
				{
					if( pSection->m_pucData[i] != s_sTestData[nFilterIndex].pucTestData[i])
					{
						CSTCPrint( "\n�����ڵ�%d ���ֽڱȽ�ʱ��ͨ��! ��������Ϊ%02x,��ȷ����Ϊ%02x\n",\
									i+1,pSection->m_pucData[i],s_sTestData[nFilterIndex].pucTestData[i]);
						CSTCPrint( "\nThe %dth byte of the section is not correct! Received data: %02x,Expected data: %02x\n",\
									i+1,pSection->m_pucData[i],s_sTestData[nFilterIndex].pucTestData[i]);
						return ;
					}
				}
				SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_TRUE,CSUDI_TRUE ); //���ûص�����������У��ɹ�
			}
		}
		break;
		
	default:
		break;
	}
	return;
}
void SECTION_iCallback_2( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	int nFilterIndex = 0xff;
	int i;
	
   	if(pSection == CSUDI_NULL || (int)pvUserData >= EM_SECTION_FILTER_MAX)
	{
		return ;
	}
	
	nLength  = pSection->m_nDataLen;
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	
	if(s_bSectionCallbackRuq[nFilterIndex]) //����Ѿ����������򷵻أ�����ȥ�ظ�У��
	{
		return;
	}
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
			
			CSTCPrint( " FILTER%03d  data callback length = %d*****\n", nFilterIndex+1,nLength);
			
			SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_TRUE);  //���ûص��������ݵ���ʱû��У��

			nLength = (nLength < s_sTestData[nFilterIndex].nDataLen) ? nLength : s_sTestData[nFilterIndex].nDataLen;
			
			nLength = (nLength < 32) ? nLength : 32;

			for( i = 0; i < nLength; ++ i)
			{
				if( pSection->m_pucData[i] != s_sTestData[nFilterIndex].pucTestData[i])
				{
					CSTCPrint( "\n�����ڵ�%d ���ֽڱȽ�ʱ��ͨ��! ��������Ϊ%02x,��ȷ����Ϊ%02x\n",\
								i+1,pSection->m_pucData[i],s_sTestData[nFilterIndex].pucTestData[i]);
					CSTCPrint( "\nThe %dth byte of the section is not correct! Received data: %02x,Expected data: %02x\n",\
								i+1,pSection->m_pucData[i],s_sTestData[nFilterIndex].pucTestData[i]);
					return ;
				}
			}
			CSTCPrint( "\nFILTER%03d ������֤ͨ��***********************************\n",nFilterIndex+1);
			CSTCPrint( "\nFILTER%03d passes the checking***********************************\n",nFilterIndex+1);
			SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_TRUE,CSUDI_TRUE ); //���ûص�����������У��ɹ�
			break;
	case EM_UDI_REQ_PES_DATA:
		break;
		
	default:
		break;

	}
	return;
}

void SECTION_iCallback_3( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{	
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	int nFilterIndex = 0xff;
	int i;
    extern unsigned char g_CSTC_SECTION_cVersion;
   	if(pSection == CSUDI_NULL || (int)pvUserData >= EM_SECTION_FILTER_MAX)
	{	
		return ;
	}
	
	nLength  = pSection->m_nDataLen;
	g_CSTC_SECTION_cVersion = (pSection->m_pucData[5]>>1)&0x1f;
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	
	if(s_bSectionCallbackRuq[nFilterIndex]) //����Ѿ����������򷵻أ�����ȥ�ظ�У��
	{	
		return;
	}
	switch(eType)
	{	
		case EM_UDI_REQ_SECTION_DATA:
			
			CSTCPrint( " FILTER%03d  data callback length = %d*****\n", nFilterIndex+1,nLength);
			
			SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_TRUE);  //���ûص��������ݵ���ʱû��У��

			nLength = (nLength < s_sTestData[nFilterIndex].nDataLen) ? nLength : s_sTestData[nFilterIndex].nDataLen;
			
			nLength = (nLength < 32) ? nLength : 32;

			for( i = 0; i < nLength; ++ i)
			{
				if( pSection->m_pucData[i] != s_sTestData[nFilterIndex].pucTestData[i])
				{
					CSTCPrint( "\n�����ڵ�%d ���ֽڱȽ�ʱ��ͨ��! ��������Ϊ%02x,��ȷ����Ϊ%02x\n",\
								i+1,pSection->m_pucData[i],s_sTestData[nFilterIndex].pucTestData[i]);
					return ;
				}
			}
			CSTCPrint( "\nFILTER%03d ������֤ͨ��***********************************\n",nFilterIndex+1);
			SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_TRUE,CSUDI_TRUE ); //���ûص�����������У��ɹ�
			break;
	case EM_UDI_REQ_PES_DATA:
		break;
		
	default:
		break;

	}
	return;
}

/*
*filter�Ļص��������˺������ж����ݵ���ȷ��,��Ҫ��������CSTC_SECTION_IT_Allocate_0003����
*/
CSUDI_BOOL g_bUserDataIsNUll = CSUDI_TRUE;
void SECTION_iCallback_UserDataNULL( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	int nFilterIndex = EM_SECTION_PAT_FILTER_001; //Ĭ����PAT_FILTER_001
	int i;

	if(s_bSectionCallbackRuq[nFilterIndex]) //����Ѿ����������򷵻أ�����ȥ�ظ�У��
	{
		return;
	}
	
   	if(pSection == CSUDI_NULL || pvUserData != CSUDI_NULL) //�û�����Ӧ��ΪCSUDI_NULL
	{
		if(pvUserData != CSUDI_NULL)
		{
			g_bUserDataIsNUll = CSUDI_FALSE;
			CSTCPrint( "�û�����Ӧ��ΪCSUDI_NULL\n");
		}
		return ;
	}
	
	nLength  = pSection->m_nDataLen;
	
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
			
			CSTCPrint( " FILTER%03d  data callback length = %d*****\n", nFilterIndex+1,nLength);
			
			SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_TRUE);  //���ûص��������ݵ���ʱû��У��

			nLength = (nLength < s_sTestData[nFilterIndex].nDataLen) ? nLength : s_sTestData[nFilterIndex].nDataLen;
			
			nLength = (nLength < 32) ? nLength : 32;

			for( i = 0; i < nLength; ++ i)
			{
				if( pSection->m_pucData[i] != s_sTestData[nFilterIndex].pucTestData[i])
				{
					CSTCPrint( "\n�����ڵ�%d ���ֽڱȽ�ʱ��ͨ��! ��������Ϊ%02x,��ȷ����Ϊ%02x\n",\
								i+1,pSection->m_pucData[i],s_sTestData[nFilterIndex].pucTestData[i]);
					CSTCPrint( "\nThe %dth byte of the section is not correct! Received data: %02x,Expected data: %02x\n",\
								i+1,pSection->m_pucData[i],s_sTestData[nFilterIndex].pucTestData[i]);
					return ;
				}
			}
			CSTCPrint( "\nFILTER%03d ������֤ͨ��***********************************\n",nFilterIndex+1);
			CSTCPrint( "\nFILTER%03d passes the checking***********************************\n",nFilterIndex+1);
			SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_TRUE,CSUDI_TRUE ); //���ûص�����������У��ɹ�
			break;
	case EM_UDI_REQ_PES_DATA:
		break;
		
	default:
		break;

	}
	return;
}
//��Ҫ��������CSTC_SECTION_IT_Allocate_0003����
CSUDI_BOOL SECTION_iGetCallbackStatus_UserDataNULL( CSUDI_BOOL bCheck, const CSUDISECTIONRequestInfo_S * psRequestInfo, const void* pvUserData, CSUDI_BOOL bDebug)
{
	int i = 0;
	int nWaitTime = 2; // 2s
	CSUDI_BOOL bRet = CSUDI_FALSE;
    int nFilterIndex = EM_SECTION_PAT_FILTER_001;

   	if(psRequestInfo == CSUDI_NULL || pvUserData != CSUDI_NULL)
	{
		return CSUDI_FALSE;
	}

	if((nFilterIndex >= EM_SECTION_EIT_FILTER_001) && (nFilterIndex < EM_SECTION_EIT_FILTER_MAX))
	{
		nWaitTime = 5; //��EIT������Ҫ5s����
	}
	
	for(i=0;i < nWaitTime;i++) //�ȴ�2s��ȡPAT��ȡ
	{
	    CSUDIOSThreadSleep(1000); 
        
        if( bCheck ) //��ҪУ������
		{	  
			if( s_bSectionCallbackSuc[nFilterIndex])
			{
				bRet = CSUDI_TRUE;
			}
		}
		else   //����ҪУ������
		{
			if( s_bSectionCallbackRuq[nFilterIndex])
			{
				bRet = CSUDI_TRUE;
			}
		}

		if(bRet)
		{
			return bRet;
		}
	}
	
	if(!bDebug) 
	{
		return CSUDI_FALSE;
	}

	if(bCheck == CSUDI_TRUE && s_bSectionCallbackRuq[nFilterIndex] == CSUDI_TRUE)
	{
		CSTCPrint("[SECTION]FILTER%03d���뵽����,��δͨ��������֤������������ļ��Ƿ���ȷ\r\n", nFilterIndex+1);
		CSTCPrint("[SECTION] ��ǰFILTER����:\r\n");
		CSTCPrint("[SECTION]FILTER%03d Receives data,but checking failure!\r\n", nFilterIndex+1);
		CSTCPrint("[SECTION] Current FILTER context:\r\n");
		CSTCPrint("[SECTION] bCRCCheck %d\r\n",psRequestInfo->m_bCRCCheck);
		CSTCPrint("[SECTION] dwDemuxID %d\r\n",psRequestInfo->m_nDemuxID);
		CSTCPrint("[SECTION] dwPID 0x%x\r\n",psRequestInfo->m_nPID);
		
		CSTCPrint("[SECTION] match: ");
		for(i=0;i<CSUDISECTION_FILTER_LEN;i++)
		{
			CSTCPrint("%02x ",psRequestInfo->m_sMMFilter.match[i]);
		}
		
		CSTCPrint("\r\n[SECTION]mask:  ");
		for(i=0;i<CSUDISECTION_FILTER_LEN;i++)
		{
			CSTCPrint("%02x ",psRequestInfo->m_sMMFilter.mask[i]);
		}
		
		CSTCPrint("\r\n[SECTION]negate:");
		for(i=0;i<CSUDISECTION_FILTER_LEN;i++)
		{
			CSTCPrint("%02x ",psRequestInfo->m_sMMFilter.negate[i]);
		}
	}

	if( s_bSectionCallbackRuq[nFilterIndex] == CSUDI_FALSE)
	{
		CSTCPrint("\r\n[SECTION]FILTER%03dδ���뵽����\r\n",nFilterIndex+1);
		CSTCPrint("\r\n[SECTION]FILTER%03d not Receive data\r\n",nFilterIndex+1);
	}
	return CSUDI_FALSE;	
}
/*
***��Ҫ���CSTC_SECTION_IT_0040��������
*/
void SECTION_iCallback_4(CSUDISECTIONRequestType_E eType,void * pvParam ,void * pvUserData)
{
	int i = 0;
	static unsigned char bPES_ReceiveFlag[PES_LENGTH]= {0};
	static unsigned char bPSISI_ReceiveFlag[PSISI_LENGTH]= {0};
	CSUDI_BOOL  bPESAllReceive = CSUDI_TRUE;
	CSUDI_BOOL	bPSISIAllReceive = CSUDI_TRUE;
    int PES_LEN = sizeof(s_TestSUBTITLE1DATA)/sizeof(s_TestSUBTITLE1DATA[0]);
    int  PSISI_LEN = sizeof(s_TestEIT_SECTION2DATA)/sizeof(s_TestEIT_SECTION2DATA[0]);
	CSUDISECTIONCallbackData_S* pData = (CSUDISECTIONCallbackData_S*)pvParam;
		
    if(!s_bPSISI_PES_Flag_Init)
    {
        s_bPSISI_PES_Flag_Init = CSUDI_TRUE;
        memset(bPES_ReceiveFlag,  0,sizeof(bPES_ReceiveFlag));
        memset(bPSISI_ReceiveFlag,0,sizeof(bPSISI_ReceiveFlag));
    }
    
	if(CSUDI_NULL == pvParam)
	{
		return ;
	}

	if(CSUDI_NULL == pData || CSUDI_NULL == pData->m_pucData)
	{
		return ;
	}

	if(EM_UDI_REQ_PES_DATA == eType)
	{
		for(i=0; i<PES_LEN; i++)
		{
			if(0== memcmp(pData->m_pucData,&s_TestSUBTITLE1DATA[i],DATA_LEN))
			{
				bPES_ReceiveFlag[i]=1;
				{
					int j=0;
					printf("bPES_ReceiveFlag  :");
					for(j=0; j<PES_LEN; j++)
					{
						printf("%d ",bPES_ReceiveFlag[j]);
					}
					printf("\n");
				}
			}
		}

		for(i =0; i<sizeof(bPES_ReceiveFlag)/sizeof(bPES_ReceiveFlag[0]); i++)
		{
			if(bPES_ReceiveFlag[i] != 1)
			{
				bPESAllReceive = CSUDI_FALSE;
				break;
			}
		}
		if(bPESAllReceive)
		{
            SECTION_iSetCallbackStatus(EM_SECTION_PSISI_PES_FILTER_PES,CSUDI_TRUE,CSUDI_TRUE);
		}
	}
	else if(EM_UDI_REQ_SECTION_DATA == eType)
	{
		for(i=0; i<PSISI_LEN; i++)
		{
			if(0== memcmp(pData->m_pucData,&s_TestEIT_SECTION2DATA[i],DATA_LEN))
			{
				bPSISI_ReceiveFlag[i]=1;
				{
					int j=0;
					printf("bPSISI_ReceiveFlag :");
					for(j=0; j<PSISI_LEN; j++)
					{
						printf("%d ",bPSISI_ReceiveFlag[j]);
					}
					printf("\n");
				}
			}
		}

		for(i =0; i<sizeof(bPSISI_ReceiveFlag)/sizeof(bPSISI_ReceiveFlag[0]); i++)
		{
			if(bPSISI_ReceiveFlag[i] != 1)
			{
				bPSISIAllReceive = CSUDI_FALSE;
				break;
			}
		}
	    if(bPSISIAllReceive)
		{
            SECTION_iSetCallbackStatus(EM_SECTION_PSISI_PES_FILTER_PSISI,CSUDI_TRUE,CSUDI_TRUE);
		}	
    }
    else
    {
        /*date type error*/
    }
}

/*
* ��Ҫ���CSTC_SECTION_IT_0016��������
*/
static int s_nEitSectionCnt = 0;
void SECTION_iEitCallback( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	int nFilterIndex = 0xff;
	int i;
	int nSecNum = 0;
	int nTestDataIndex = 0;//EM_SECTION_EIT_FILTER_001�Ĺ����������Ի�ȡ����section������
	
   	if(pSection == CSUDI_NULL || (int)pvUserData >= EM_SECTION_FILTER_MAX)
	{
		return ;
	}
	
	nSecNum  = pSection->m_pucData[6];
	nLength  = pSection->m_nDataLen;
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];

	nTestDataIndex = (nSecNum == 0) ? EM_SECTION_EIT_FILTER_002 : EM_SECTION_EIT_FILTER_003;
	
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
			
			CSTCPrint( " FILTER%03d  data callback length = %d*****\n", nFilterIndex+1,nLength);
			
			SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_TRUE);  //���ûص��������ݵ���ʱû��У��

			nLength = (nLength < s_sTestData[nTestDataIndex].nDataLen) ? nLength : s_sTestData[nTestDataIndex].nDataLen;
			
			nLength = (nLength < 32) ? nLength : 32;

			for( i = 0; i < nLength; ++ i)
			{
				if( pSection->m_pucData[i] != s_sTestData[nTestDataIndex].pucTestData[i])
				{
					CSTCPrint( "\n�����ڵ�%d ���ֽڱȽ�ʱ��ͨ��! ��������Ϊ%02x,��ȷ����Ϊ%02x\n",\
								i+1,pSection->m_pucData[i],s_sTestData[nTestDataIndex].pucTestData[i]);
					CSTCPrint( "\nThe %dth byte of the section is not correct! Received data: %02x,Expected data: %02x\n",\
								i+1,pSection->m_pucData[i],s_sTestData[nTestDataIndex].pucTestData[i]);
					return ;
				}
			}
			CSTCPrint( "\nEIT������֤ͨ��***********************************\n");
			CSTCPrint( "\nEIT data passes the checking***********************************\n");
			if((EM_SECTION_EIT_FILTER_001 == nFilterIndex) && (++s_nEitSectionCnt == 2))//�յ�����section�����������
			{
				SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_TRUE,CSUDI_TRUE ); //���ûص�����������У��ɹ�
				s_nEitSectionCnt = 0;
			}
			else
			{
				SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_TRUE,CSUDI_TRUE ); //���ûص�����������У��ɹ�
			}
			break;
	case EM_UDI_REQ_PES_DATA:
		break;
		
	default:
		break;

	}
	return;
}
 /**
@brief ��ȡ���ݻص�������״̬�������2s�ڻ�ȡ���������򷵻�ʧ�ܣ�bDebugΪCSUDI_TRUE,���ӡ�������ֵ

@param[in] bCheck �������ݺ��Ƿ��������У��
@param[in] psRequestInfo  �������filterֵ
@param[in] pvUserData ����filter���û����ݣ���μ�SECTION_Filter_Eö������
@param[in] bDebug ������󲻵����ݣ�ΪCSUDI_TRUE���ӡ�����filter����Ϣ��ΪCSUDI_FALSE�򲻴�ӡ�����filter��Ϣ
@return 
	-bCheckΪCSUDI_TRUE,�ܹ��ɹ��������ݻ�������У��ͨ���ŷ���CSUDI_TRUE
	-bCheckΪCSUDI_FALSE���ܹ��ɹ����������򷵻�CSUDI_TRUE
*/
CSUDI_BOOL SECTION_iGetCallbackStatus( CSUDI_BOOL bCheck, const CSUDISECTIONRequestInfo_S * psRequestInfo, const void* pvUserData, CSUDI_BOOL bDebug)
{
	int i = 0;
	int nWaitTime = 2; // 2s
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int nFilterIndex;

   	if(psRequestInfo == CSUDI_NULL || (int)pvUserData >= EM_SECTION_FILTER_MAX)
	{
		return CSUDI_FALSE;
	}
	
    nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
    
	if(nFilterIndex >= EM_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserDataӦ�ò�����EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE ;
	}

	if((nFilterIndex >= EM_SECTION_EIT_FILTER_001) && (nFilterIndex < EM_SECTION_EIT_FILTER_MAX))
	{
		nWaitTime = 5; //��EIT������Ҫ5s����
	}
	else if(EM_SECTION_TDT_FILTER_001 == nFilterIndex )
	{
		nWaitTime = 30;//TDT��һ������30s��һ��
	}
	else if(EM_SECTION_PSISI_PES_FILTER_PES == nFilterIndex)
	{
		nWaitTime = 110;// TELETEXT_SUBTITLE_25MBPS_071220_TS����Ĭ��������25M��Ҫ��֤����������һȦ��110s����
	}
	else if(EM_SECTION_PSISI_PES_FILTER_PSISI == nFilterIndex)
	{
		nWaitTime = 10;
	}
	else if(nFilterIndex >= EM_SECTION_PES_VIDEO_001 && nFilterIndex < EM_SECTION_PES_MAX)
	{
		nWaitTime = 10;//pes��һ������30s��һ��
	}
    else
    {
		/*nWaitTime Unkown*/
    }
	
	for(i=0;i < nWaitTime;i++) //�ȴ�2s��ȡPAT��ȡ
	{
	    CSUDIOSThreadSleep(1000); 
        
        if( bCheck ) //��ҪУ������
		{	  
			if( s_bSectionCallbackSuc[nFilterIndex])
			{
				bRet = CSUDI_TRUE;
			}
		}
		else   //����ҪУ������
		{
			if( s_bSectionCallbackRuq[nFilterIndex])
			{
				bRet = CSUDI_TRUE;
			}
		}

		if(bRet)
		{
			return bRet;
		}
	}
	
	if(!bDebug) 
	{
		return CSUDI_FALSE;
	}
	if(bCheck == CSUDI_TRUE && s_bSectionCallbackRuq[nFilterIndex] == CSUDI_TRUE)
	{
		CSTCPrint("[SECTION]FILTER%03d���뵽����,��δͨ��������֤������������ļ��Ƿ���ȷ\r\n", nFilterIndex+1);
		CSTCPrint("[SECTION] ��ǰFILTER����:\r\n");
		CSTCPrint("[SECTION]FILTER%03d Receives data,but checking failure!\r\n", nFilterIndex+1);
		CSTCPrint("[SECTION] Current FILTER context:\r\n");
		CSTCPrint("[SECTION] bCRCCheck %d\r\n",psRequestInfo->m_bCRCCheck);
		CSTCPrint("[SECTION] dwDemuxID %d\r\n",psRequestInfo->m_nDemuxID);
		CSTCPrint("[SECTION] dwPID 0x%x\r\n",psRequestInfo->m_nPID);
		
		CSTCPrint("[SECTION] match: ");
		for(i=0;i<CSUDISECTION_FILTER_LEN;i++)
		{
			CSTCPrint("%02x ",psRequestInfo->m_sMMFilter.match[i]);
		}
		
		CSTCPrint("\r\n[SECTION]mask:  ");
		for(i=0;i<CSUDISECTION_FILTER_LEN;i++)
		{
			CSTCPrint("%02x ",psRequestInfo->m_sMMFilter.mask[i]);
		}
		
		CSTCPrint("\r\n[SECTION]negate:");
		for(i=0;i<CSUDISECTION_FILTER_LEN;i++)
		{
			CSTCPrint("%02x ",psRequestInfo->m_sMMFilter.negate[i]);
		}
	}

	if( s_bSectionCallbackRuq[nFilterIndex] == CSUDI_FALSE && bCheck == CSUDI_TRUE)
	{
		CSTCPrint("\r\n[SECTION]Filter %03d δ���뵽����\r\n",nFilterIndex+1);
		CSTCPrint("\r\n[SECTION]Filter %03d not Receive data\r\n",nFilterIndex+1);
	}
	return CSUDI_FALSE;	
}
/*
*���ûص�����״ֵ̬����Ҫ������filter֮ǰ����
*/
void SECTION_iSetCallbackStatus( int nFilterIndex, CSUDI_BOOL bSuc ,CSUDI_BOOL bReq )
{

	if(nFilterIndex >= EM_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserData < EM_SECTION_FILTER_MAX\n");
		 return ;
	}

	s_bSectionCallbackSuc[nFilterIndex] = bSuc;
	s_bSectionCallbackRuq[nFilterIndex] = bReq;
	
	return;
}

/*
* ����һ��filter�������ûص�����״ֵ̬
* ��Ҫ���ڲ��Թ�עfilter��ȡ������ֵ���������������CSTC_SECTION_IT_Allocate_0003
*/
CSUDI_BOOL SECTION_iAllocFilter(const CSUDISECTIONRequestInfo_S * psRequestInfo, const void* pvUserData,CSUDI_HANDLE *phFilter)
{
	int nFilterIndex = 0xff;
	
	if((int)pvUserData >= EM_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserDataӦ�ò�����EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE;
	}
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	
	SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);

	///�˴������ҪΪ�˲�������CSTC_SECTION_IT_0012����CSTC_SECTION_IT_0012��CSTC_SECTION_IT_0013\14
	if(EM_SECTION_PAT_FILTER_001 == nFilterIndex)
	{
		return (CSUDI_SUCCESS == CSUDIFILTERAllocate(psRequestInfo,SECTION_iCallback_1,pvUserData,phFilter));
	}
	else if(EM_SECTION_PAT_FILTER_002 == nFilterIndex || EM_SECTION_PAT_FILTER_004 == nFilterIndex)
	{
		return (CSUDI_SUCCESS == CSUDIFILTERAllocate(psRequestInfo,SECTION_iCallback_2,pvUserData,phFilter));
	}
	/*{{{--------������CSTC_SECTION_IT_0040��������--------*/
	else if(nFilterIndex == EM_SECTION_PSISI_PES_FILTER_PSISI || nFilterIndex == EM_SECTION_PSISI_PES_FILTER_PES)
	{
        s_bPSISI_PES_Flag_Init = CSUDI_FALSE;
		return (CSUDI_SUCCESS == CSUDIFILTERAllocate(psRequestInfo,SECTION_iCallback_4,pvUserData,phFilter));
	}
	/*}}}--------������CSTC_SECTION_IT_0040��������--------*/
	else if((nFilterIndex >= EM_SECTION_EIT_FILTER_001) && (nFilterIndex < EM_SECTION_EIT_FILTER_MAX))
	{
		s_nEitSectionCnt = 0; //Initial the EitSection count to 0
		return (CSUDI_SUCCESS == CSUDIFILTERAllocate(psRequestInfo,SECTION_iEitCallback,pvUserData,phFilter));
	}
	else
	{
		return (CSUDI_SUCCESS == CSUDIFILTERAllocate(psRequestInfo,SECTION_iCallback_1,pvUserData,phFilter));
	}

	return CSUDI_FALSE;
}

CSUDI_BOOL SECTION_iStartFilter(CSUDI_HANDLE hFilter,const void * pvUserData)
{
	int nFilterIndex = 0xff;
	CSUDI_Error_Code nRet = CSUDI_FALSE;
	if((int)pvUserData >= EM_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserData < EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE;
	}
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	
	SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);	
	
	nRet = CSUDIFILTERStart(hFilter);
	CSTCPrint("SECTION_iStartFilter %03d.....\n",nFilterIndex+1);
	
	if(CSUDI_SUCCESS != nRet)
	{
		CSTCPrint("SECTION_iStartFilter Failure !!\n");
	}
	return (CSUDI_SUCCESS == nRet);
}
CSUDI_BOOL SECTION_iStopFilter(CSUDI_HANDLE hFilter,const void* pvUserData)
{
	int nFilterIndex = 0xff;
	CSUDI_Error_Code nRet = CSUDI_FALSE;
	
	if((int)pvUserData >= EM_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserData < EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE;
	}
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	
	CSTCPrint("SECTION_iStopFilter %03d.....\n",nFilterIndex+1);
	
	nRet = CSUDIFILTERStop(hFilter);
	
	CSUDIOSThreadSleep(1000); 
	
	SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);
	return (CSUDI_SUCCESS == nRet);
}
CSUDI_BOOL SECTION_iModifyFilter(CSUDI_HANDLE hFilter,const CSUDIFILTERMatchMask_S * psMatchFilter,CSUDI_BOOL bCRCCheck,const void* pvUserData)
{
	int nFilterIndex = 0xff;
	CSUDI_Error_Code nRet = CSUDI_FALSE;
	if((int)pvUserData >= EM_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserData < EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE;
	}
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	CSTCPrint("SECTION_iModifyFilter %03d .....\n",nFilterIndex+1);
	SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);	
	
	nRet = CSUDIFILTERModify(hFilter,psMatchFilter,bCRCCheck);
	
	return (CSUDI_SUCCESS == nRet);
}
CSUDI_BOOL SECTION_iFreeFilter(CSUDI_HANDLE hFilter,const void* pvUserData)
{
	int nFilterIndex = 0xff;
	CSUDI_Error_Code nRet = CSUDI_FALSE;
	
	if((int)pvUserData >= EM_SECTION_FILTER_MAX)
	{
		 CSTCPrint("(int)pvUserDataӦ�ò�����EM_SECTION_FILTER_MAX\n");
		 return CSUDI_FALSE;
	}
	
	nFilterIndex = s_nDMXFilterIndex[(int)pvUserData];
	CSTCPrint("SECTION_iFreeFilter %03d.....\n",nFilterIndex+1);
	nRet = CSUDIFILTERFree(hFilter);
	
	CSUDIOSThreadSleep(500); 
	
	SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);
	
	return (CSUDI_SUCCESS == nRet);
}


CSUDI_BOOL SECTION_iGetPowerCallBackStatus()
{
	int nTimes = 0;
	while(nTimes < TESTTIMES*10)//��������Լ��Ҫ2��30�����
	{
		CSUDIOSThreadSleep(1000);
		
		if(s_nTaskP01Rst != 0 && s_nTaskP02Rst != 0 && s_nTaskP03Rst != 0)
		{
			if(s_nTaskP01Rst == 1 && s_nTaskP02Rst == 1 && s_nTaskP03Rst == 1)
			{
				return CSUDI_TRUE;
			}
			else
			{
				CSTCPrint("s_nTaskP01Rst = %d.....\n",s_nTaskP01Rst);
				CSTCPrint("s_nTaskP02Rst = %d.....\n",s_nTaskP02Rst);
				CSTCPrint("s_nTaskP03Rst = %d.....\n",s_nTaskP03Rst);
				return CSUDI_FALSE;
			}
		}
		nTimes++;
	}
	return CSUDI_FALSE;
}
CSUDI_BOOL SECTION_iGetRequestCallBackStatus()
{
	int nTimes = 0;
	while(nTimes < TESTTIMES*30)//��������Լ��Ҫ4��30�����
	{
		CSUDIOSThreadSleep(1000);
		if( s_nTaskR01Rst != 0 && s_nTaskR02Rst != 0 && s_nTaskR03Rst != 0 && s_nTaskR04Rst != 0)
		{
			if( s_nTaskR01Rst == 1 && s_nTaskR02Rst == 1 && s_nTaskR03Rst == 1 && s_nTaskR04Rst == 1)
			{
				return CSUDI_TRUE;
			}
			else
			{
				CSTCPrint("s_nTaskR01Rst = %d.....\n",s_nTaskR01Rst);
				CSTCPrint("s_nTaskR02Rst = %d.....\n",s_nTaskR02Rst);
				CSTCPrint("s_nTaskR03Rst = %d.....\n",s_nTaskR03Rst);
				CSTCPrint("s_nTaskR04Rst = %d.....\n",s_nTaskR04Rst);
				return CSUDI_FALSE;
			}
		}
		nTimes++;
	}
	return CSUDI_FALSE;
}

//�ο�CSTC_SECTION_IT_0016����˵��
void SECTION_IT_0016_Filter_Thread1(void * pvArg)
{
	int nLoop = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIFILTERMatchMask_S sMatch;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;

	for(nLoop = 0; nLoop < TESTTIMES; nLoop++)
	{
		CSTCPrint("\rFilter_Thread1 begin ..........\r\n");
		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter1);
		SECTION_GOTO(bRet,EXISTLABEL,"Create Filter1 failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001);
		SECTION_GOTO(bRet,EXISTLABEL,"Start Filter1 failure !!!");
		
		bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001, CSUDI_TRUE);
		SECTION_GOTO(bRet,EXISTLABEL,"Expected Data at filter1 failure  !!!");
		
		bRet = SECTION_iStopFilter(hFilter1, (void*)EM_SECTION_PAT_FILTER_001);
		SECTION_GOTO(bRet,EXISTLABEL,"Stop filter1 failure  !!!");

		bRet = SECTION_iSetMatchMask(&sMatch,EM_SECTION_PAT_FILTER_002);
		SECTION_GOTO(bRet,EXISTLABEL,"Set Match2 failure  !!!");
		
		
		bRet = SECTION_iModifyFilter(hFilter1,&sMatch,CSUDI_TRUE,(void*)EM_SECTION_PAT_FILTER_002);
		SECTION_GOTO(bRet,EXISTLABEL,"Modify filter2 Failure  !!!");
		
		bRet = SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_002);
		SECTION_GOTO(bRet,EXISTLABEL,"Start Filter1 failure !!!");		
		//��Ϊ�û����ݻ���EM_SECTION_PAT_FILTER_001,��������ʹ��EM_SECTION_PAT_FILTER_001
		bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001, CSUDI_TRUE);
		SECTION_GOTO(bRet,EXISTLABEL,"Expected Data at filter1 failure  !!!");
		
		bRet = SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_002);
		SECTION_GOTO(bRet,EXISTLABEL,"Free Filter1 failure !!!");	
		hFilter1 = CSUDI_NULL;
		
		CSTCPrint("[SECTION]Filter_Thread1����%d��,��ѭ�����ɽ���,�����ĵȴ�\r\n",TESTTIMES-nLoop-1);
		CSTCPrint("[SECTION]Pls wait! Filter_Thread1 will exit the loop after %d times\r\n",TESTTIMES-nLoop-1);
		CSUDIOSThreadSleep(1000);
	}
EXISTLABEL:
		
	s_nTaskP01Rst = (bRet ? 1 : 2);
	if(CSUDI_NULL != hFilter1)
	{
		SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_002);
	}
	CSTCPrint("\rFilter_Thread1 end ..........\r\n");
	return;
}

//�ο�CSTC_SECTION_IT_0016����˵��
void SECTION_IT_0016_Filter_Thread2(void * pvArg)
{
	int nLoop = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIFILTERMatchMask_S sMatch;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;

	for(nLoop = 0; nLoop < TESTTIMES; nLoop++)
	{
		CSTCPrint("\rFilter_Thread2 begin ..........\r\n");
		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_003,DEMUX_0,EM_PAT_PID);
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,&hFilter2);
		SECTION_GOTO(bRet,EXISTLABLE,"Create Filter2 failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_003);
		SECTION_GOTO(bRet,EXISTLABLE,"Start Filter2 failure !!!");
		
		bRet = SECTION_iGetCallbackStatus(CSUDI_FALSE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003, CSUDI_FALSE);
		SECTION_GOTO(!bRet,EXISTLABLE,"Expected No Data at filter2 failure  !!!");

		bRet = SECTION_iStopFilter(hFilter2, (void*)EM_SECTION_PAT_FILTER_003);
		SECTION_GOTO(bRet,EXISTLABLE,"Stop filter2 failure  !!!");

		bRet = SECTION_iSetMatchMask(&sMatch,EM_SECTION_PAT_FILTER_004);
		SECTION_GOTO(bRet,EXISTLABLE,"Set Match2 failure  !!!");
		
		bRet = SECTION_iModifyFilter(hFilter2,&sMatch,CSUDI_TRUE,(void*)EM_SECTION_PAT_FILTER_004);
		SECTION_GOTO(bRet,EXISTLABLE,"Modify filter2 Failure  !!!");
		
		bRet = SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004);
		SECTION_GOTO(bRet,EXISTLABLE,"Start Filter2 failure !!!");		
		//��Ϊ�û����ݻ���EM_SECTION_PAT_FILTER_003,��������ʹ��EM_SECTION_PAT_FILTER_003
		bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003, CSUDI_TRUE);
		SECTION_GOTO(bRet,EXISTLABLE,"Expected Data at filter2 failure  !!!");
		
		bRet = SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004);
		SECTION_GOTO(bRet,EXISTLABLE,"Free Filter2 failure !!!");	
		hFilter2 = CSUDI_NULL;
		
		CSTCPrint("[SECTION]Filter_Thread2����%d��,��ѭ�����ɽ���,�����ĵȴ�\r\n",TESTTIMES-nLoop-1);
		CSTCPrint("[SECTION]Pls wait! Filter_Thread1 will exit the loop after %d times\r\n",TESTTIMES-nLoop-1);
		CSUDIOSThreadSleep(1000);
	}
EXISTLABLE:
		
	s_nTaskP02Rst = (bRet ? 1 : 2);
	
	if(CSUDI_NULL != hFilter2)
	{
		SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004);
	}
	CSTCPrint("\rFilter_Thread2 end ..........\r\n");
	return;

}

//�ο�CSTC_SECTION_IT_0016����˵��
void SECTION_IT_0016_Filter_Thread3(void * pvArg)
{
	int nLoop = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIFILTERMatchMask_S sMatch;
	CSUDI_HANDLE hFilter3 = CSUDI_NULL;

	for(nLoop = 0; nLoop < TESTTIMES; nLoop++)
	{
		CSTCPrint("\rFilter_Thread3 begin ..........\r\n");
		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_005,DEMUX_0,EM_PAT_PID);
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_005,&hFilter3);
		SECTION_GOTO(bRet,EXISTLABLE,"Create Filter3 failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter3,(void*)EM_SECTION_PAT_FILTER_005);
		SECTION_GOTO(bRet,EXISTLABLE,"Start Filter3 failure !!!");
		
		bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_005, CSUDI_TRUE);
		SECTION_GOTO(bRet,EXISTLABLE,"Expected Data at filter3 failure  !!!");

		bRet = SECTION_iStopFilter(hFilter3, (void*)EM_SECTION_PAT_FILTER_005);
		SECTION_GOTO(bRet,EXISTLABLE,"Stop filter3 failure  !!!");

		bRet = SECTION_iSetMatchMask(&sMatch,EM_SECTION_PAT_FILTER_006);
		SECTION_GOTO(bRet,EXISTLABLE,"Set Match3 failure  !!!");
		
		bRet = SECTION_iModifyFilter(hFilter3,&sMatch,CSUDI_TRUE,(void*)EM_SECTION_PAT_FILTER_006);
		SECTION_GOTO(bRet,EXISTLABLE,"Modify filter3 Failure  !!!");
		
		bRet = SECTION_iStartFilter(hFilter3,(void*)EM_SECTION_PAT_FILTER_006);
		SECTION_GOTO(bRet,EXISTLABLE,"Start Filter3 failure !!!");		
		//��Ϊ�û����ݻ���EM_SECTION_PAT_FILTER_005,��������ʹ��EM_SECTION_PAT_FILTER_005
		bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_005, CSUDI_TRUE);
		SECTION_GOTO(bRet,EXISTLABLE,"Expected Data at filter3 failure  !!!");
		
		bRet = SECTION_iFreeFilter(hFilter3,(void*)EM_SECTION_PAT_FILTER_006);
		SECTION_GOTO(bRet,EXISTLABLE,"Free Filter3 failure !!!");	
		hFilter3 = CSUDI_NULL;
		
		CSTCPrint("[SECTION]Filter_Thread3����%d��,��ѭ�����ɽ���,�����ĵȴ�\r\n",TESTTIMES-nLoop-1);
		CSTCPrint("[SECTION]Pls wait! Filter_Thread1 will exit the loop after %d times\r\n",TESTTIMES-nLoop-1);
		CSUDIOSThreadSleep(1000);
	}
	
EXISTLABLE:
		
	s_nTaskP03Rst = (bRet ? 1 : 2);
	
	if(CSUDI_NULL != hFilter3)
	{
		SECTION_iFreeFilter(hFilter3,(void*)EM_SECTION_PAT_FILTER_005);
	}
	CSTCPrint("\rFilter_Thread3 end ..........\r\n");
	return;
}

//�ο�CSTC_SECTION_IT_0022����˵��
void SECTION_IT_0022_Filter_Thread1(void * pvArg)
{
	int nLoop = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;

	for(nLoop = 0; nLoop < TESTTIMES; nLoop++)
	{
		CSTCPrint("\rFilter_Thread1 begin ..........\r\n");
		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_007,DEMUX_0,EM_PAT_PID);
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_007,&hFilter1);
		SECTION_GOTO(bRet,EXISTLABEL,"Create Filter1 failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_007);
		SECTION_GOTO(bRet,EXISTLABEL,"Start Filter1 failure !!!");
		
		bRet = SECTION_iGetCallbackStatus(CSUDI_FALSE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_007, CSUDI_FALSE);
		SECTION_GOTO(!bRet,EXISTLABEL,"Expected No Data at filter1 failure  !!!");
		
		bRet = SECTION_iFreeFilter(hFilter1, (void*)EM_SECTION_PAT_FILTER_007);
		SECTION_GOTO(bRet,EXISTLABEL,"Free Filter1 failure !!!");

		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_008,DEMUX_0,EM_PAT_PID);
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_008,&hFilter1);
		SECTION_GOTO(bRet,EXISTLABEL,"Create Filter1 failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_008);
		SECTION_GOTO(bRet,EXISTLABEL,"Start Filter1 failure !!!");		
		bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_008, CSUDI_TRUE);
		SECTION_GOTO(bRet,EXISTLABEL,"Expected Data at filter1 failure  !!!");
		
		bRet = SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_008);
		SECTION_GOTO(bRet,EXISTLABEL,"Free Filter1 failure !!!");	
		hFilter1 = CSUDI_NULL;
		
		CSTCPrint("[SECTION]Filter_Thread1����%d��,��ѭ�����ɽ���,�����ĵȴ�\r\n",TESTTIMES-nLoop-1);
		CSTCPrint("[SECTION]Pls wait! Filter_Thread1 will exit the loop after %d times\r\n",TESTTIMES-nLoop-1);
		CSUDIOSThreadSleep(1000);
	}
EXISTLABEL:
		
	s_nTaskP01Rst = (bRet ? 1 : 2);
	if(CSUDI_NULL != hFilter1)
	{
		SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_002);
	}
	CSTCPrint("\rFilter_Thread1 end ..........\r\n");
	return;
}

//�ο�CSTC_SECTION_IT_0022����˵��
void SECTION_IT_0022_Filter_Thread2(void * pvArg)
{
	int nLoop = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;

	for(nLoop = 0; nLoop < TESTTIMES; nLoop++)
	{
		CSTCPrint("\rFilter_Thread2 begin ..........\r\n");
		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_003,DEMUX_0,EM_PAT_PID);
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,&hFilter2);
		SECTION_GOTO(bRet,EXISTLABLE,"Create Filter2 failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_003);
		SECTION_GOTO(bRet,EXISTLABLE,"Start Filter2 failure !!!");
		
		bRet = SECTION_iGetCallbackStatus(CSUDI_FALSE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003, CSUDI_FALSE);
		SECTION_GOTO(!bRet,EXISTLABLE,"Expected No Data at filter2 failure  !!!");

		bRet = SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_003);
		SECTION_GOTO(bRet,EXISTLABLE,"Free Filter2 failure !!!");	
		
		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_004,DEMUX_0,EM_PAT_PID);
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_004,&hFilter2);
		SECTION_GOTO(bRet,EXISTLABLE,"Create Filter2 failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004);
		bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_004, CSUDI_TRUE);
		SECTION_GOTO(bRet,EXISTLABLE,"Expected Data at filter2 failure  !!!");
		
		bRet = SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004);
		SECTION_GOTO(bRet,EXISTLABLE,"Free Filter2 failure !!!");	
		hFilter2 = CSUDI_NULL;
		
		CSTCPrint("[SECTION]Filter_Thread2����%d��,��ѭ�����ɽ���,�����ĵȴ�\r\n",TESTTIMES-nLoop-1);
		CSTCPrint("[SECTION]Pls wait! Filter_Thread2 will exit the loop after %d times\r\n",TESTTIMES-nLoop-1);
		CSUDIOSThreadSleep(1000);
	}
EXISTLABLE:
		
	s_nTaskP02Rst = (bRet ? 1 : 2);
	
	if(CSUDI_NULL != hFilter2)
	{
		SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004);
	}
	CSTCPrint("\rFilter_Thread2 end ..........\r\n");
	return;

}

//�ο�CSTC_SECTION_IT_0022����˵��
void SECTION_IT_0022_Filter_Thread3(void * pvArg)
{
	int nLoop = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter3 = CSUDI_NULL;

	for(nLoop = 0; nLoop < TESTTIMES; nLoop++)
	{
		CSTCPrint("\rFilter_Thread3 begin ..........\r\n");
		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_005,DEMUX_0,EM_PAT_PID);
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_005,&hFilter3);
		SECTION_GOTO(bRet,EXISTLABLE,"Create Filter3 failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter3,(void*)EM_SECTION_PAT_FILTER_005);
		SECTION_GOTO(bRet,EXISTLABLE,"Start Filter3 failure !!!");
		
		bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_005, CSUDI_TRUE);
		SECTION_GOTO(bRet,EXISTLABLE,"Expected Data at filter3 failure  !!!");

		bRet = SECTION_iStopFilter(hFilter3, (void*)EM_SECTION_PAT_FILTER_005);
		SECTION_GOTO(bRet,EXISTLABLE,"Stop filter3 failure  !!!");

		bRet = SECTION_iFreeFilter(hFilter3,(void*)EM_SECTION_PAT_FILTER_005);
		SECTION_GOTO(bRet,EXISTLABLE,"Free Filter3 failure !!!");	

		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_006,DEMUX_0,EM_PAT_PID);
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_006,&hFilter3);
		SECTION_GOTO(bRet,EXISTLABLE,"Create Filter3 failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter3,(void*)EM_SECTION_PAT_FILTER_006);
		SECTION_GOTO(bRet,EXISTLABLE,"Start Filter3 failure !!!");		
		bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_006, CSUDI_TRUE);
		SECTION_GOTO(bRet,EXISTLABLE,"Expected Data at filter3 failure  !!!");
		
		bRet = SECTION_iFreeFilter(hFilter3,(void*)EM_SECTION_PAT_FILTER_006);
		SECTION_GOTO(bRet,EXISTLABLE,"Free Filter3 failure !!!");	
		hFilter3 = CSUDI_NULL;
		
		CSTCPrint("[SECTION]Filter_Thread3����%d��,��ѭ�����ɽ���,�����ĵȴ�\r\n",TESTTIMES-nLoop-1);
		CSTCPrint("[SECTION]Pls wait! Filter_Thread3 will exit the loop after %d times\r\n",TESTTIMES-nLoop-1);
		CSUDIOSThreadSleep(1000);
	}
	
EXISTLABLE:
		
	s_nTaskP03Rst = (bRet ? 1 : 2);
	
	if(CSUDI_NULL != hFilter3)
	{
		SECTION_iFreeFilter(hFilter3,(void*)EM_SECTION_PAT_FILTER_005);
	}
	CSTCPrint("\rFilter_Thread3 end ..........\r\n");
	return;
}

//�ο�CSTC_SECTION_IT_0017����˵��
void ThreadWithFilterNum(void * pvArg)
{
	int nLoop = 0;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	int nPid = 0xffff;
	int nFilterIndex = (int)pvArg;
	switch(nFilterIndex)
	{
		case EM_SECTION_PAT_FILTER_001:
			nPid = EM_PAT_PID;
			break;
		case EM_SECTION_CAT_FILTER_001:
			nPid = EM_CAT_PID;
			break;
		case EM_SECTION_EIT_FILTER_001:
			nPid = EM_EIT_PID;
			break;
		case EM_SECTION_TDT_FILTER_001:
			nPid = EM_TDT_PID;
			break;
		case EM_SECTION_PES_SUBTITLE_001:
			nPid = EM_SUBTITLE_001_PID;
			break;
		case EM_SECTION_PES_TELETEXT_001:
			nPid = EM_TELETEXT_001_PID;
			break;
		case EM_SECTION_PES_AUDIO_001:
			nPid = EM_AUDIO_001_PID;
			break;
		case EM_SECTION_PES_AUDIO_002:
			nPid = EM_AUDIO_002_PID;
			break;
		default:
			break;
	}
	if(nFilterIndex >= EM_SECTION_PES_VIDEO_001 && nFilterIndex < EM_SECTION_PES_MAX)
	{
		SECTION_iSetPESReq(&sTmpReqInfo,nFilterIndex,DEMUX_0,nPid);
	}
	else
	{
		SECTION_iSetSectionFilter(&sTmpReqInfo,nFilterIndex,DEMUX_0,nPid);
	}
	
	for(nLoop = 0; nLoop < TESTTIMES; nLoop++)
	{
		bRet = SECTION_iAllocFilter(&sTmpReqInfo,(void*)nFilterIndex,&hFilter);
		SECTION_GOTO(bRet,EXISTLABLE,"Create Filter failure !!!");
		
		bRet = SECTION_iStartFilter(hFilter,(void*)nFilterIndex);
		SECTION_GOTO(bRet,EXISTLABLE,"Start Filter failure !!!");
		if(EM_SECTION_PES_VIDEO_001 <= nFilterIndex && nFilterIndex <= EM_SECTION_PES_MAX)
		{
			bRet = SECTION_iGetCallbackStatus(CSUDI_FALSE, &sTmpReqInfo,(void*)nFilterIndex, CSUDI_TRUE);
		}
		else
		{
			bRet = SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo,(void*)nFilterIndex, CSUDI_TRUE);
		}
		SECTION_GOTO(bRet,EXISTLABLE,"Expected Data at filter failure  !!!");
		
		bRet = SECTION_iFreeFilter(hFilter,(void*)nFilterIndex);
		SECTION_GOTO(bRet,EXISTLABLE,"Free Filter failure !!!");	
		hFilter = CSUDI_NULL;
		
		CSTCPrint("[SECTION]Filter_Thread%02d ����%d��,��ѭ�����ɽ���,�����ĵȴ�\r\n",nFilterIndex,TESTTIMES-nLoop-1);
		CSTCPrint("[SECTION]Pls wait! Filter_Thread%02d will exit the loop after %d times\r\n",nFilterIndex,TESTTIMES-nLoop-1);
		CSUDIOSThreadSleep(1000);
	}
	
EXISTLABLE:
	switch(nFilterIndex)
	{
		case EM_SECTION_PAT_FILTER_001:
		case EM_SECTION_PES_SUBTITLE_001:
			s_nTaskR01Rst = (bRet ? 1 : 2);
			break;
		case EM_SECTION_CAT_FILTER_001:
		case EM_SECTION_PES_TELETEXT_001:
			s_nTaskR02Rst = (bRet ? 1 : 2);
			break;
		case EM_SECTION_EIT_FILTER_001:
		case EM_SECTION_PES_AUDIO_001:
			s_nTaskR03Rst = (bRet ? 1 : 2);
			break;
		case EM_SECTION_TDT_FILTER_001:
		case EM_SECTION_PES_AUDIO_002:
			s_nTaskR04Rst = (bRet ? 1 : 2);
			break;
		default:
			break;
	}	
	
	if(CSUDI_NULL != hFilter)
	{
		SECTION_iFreeFilter(hFilter,(void*)nFilterIndex);
	}
	hFilter = CSUDI_NULL;
	return;
}
#if 1
#define SECTION_BUFF_SIZE   (1024*1024+1024*128)  

HCSHANDLE SECTION_iUSPSectionInit(void)
{
	HCSHANDLE hSection ;
	TaskInitPar     tempTaskInitPar;
	SectionQueuePar tempQueuePar[3];
	unsigned char * pucSectionBuf = CSUDI_NULL;
	
	
	hSection = Section2CGetSectionSourceHandle();

	if(hSection == 0)
	{
		pucSectionBuf = (unsigned char*)CSUDIOSMalloc(SECTION_BUFF_SIZE);
		SECTION_GOTO((pucSectionBuf != CSUDI_NULL),EXISTLABLE, "Create Section buffer failure!!");

		tempQueuePar[0].m_nQueueSize = 1024*1024; // 1M
		tempQueuePar[0].m_nPriorityIndex = SECTION_PRIORITY_HIGH;
		
		tempQueuePar[1].m_nQueueSize = 0;
		tempQueuePar[1].m_nPriorityIndex = SECTION_PRIORITY_MIDDLE;

		tempQueuePar[2].m_nQueueSize = 0;
		tempQueuePar[2].m_nPriorityIndex = SECTION_PRIORITY_LOW;
		
		tempTaskInitPar.m_nOneBufSize           = 0;
		tempTaskInitPar.m_nSectionQueueParCount = 3;
		tempTaskInitPar.m_nStackSize            = 32*1024; //Section task stack size
		tempTaskInitPar.m_nTaskPriority         = 6*17;    //Section task priority
		tempTaskInitPar.m_nTaskPriorityIndex    = 0;
		tempTaskInitPar.m_pSectionQueuePar      = tempQueuePar;

		hSection = Section2CCreateSectionSourceTimeOutTaskEx( \
			pucSectionBuf, SECTION_BUFF_SIZE, &tempTaskInitPar, 1, 2*17, 8*1024);
		SECTION_GOTO((hSection != 0),EXISTLABLE, "Create Section source failure!!");
	}
EXISTLABLE:
	if(hSection == 0)
	{
		CSUDIOSFree(pucSectionBuf);
	}

	return hSection;
}
#endif
typedef enum {
	SectionStop =0,
	SectionStart=1,
	SectionFinish,
	SectionFree
}SectionStatus_E;

typedef struct 
{
 	int nLastNum;
	CSUDI_HANDLE hRequest;
	int nTraceIndex; 
	unsigned char* pSectionTrace;
	CSUDI_BOOL bStatus;
	CSUDI_BOOL bDropPacket;
}SectionInfo_S;

#define TEST_FILTER_NUM (16)	//16

static SectionInfo_S 	g_sSecInfo[TEST_FILTER_NUM];
static CSUDI_HANDLE		g_hSectInfoSem = CSUDI_NULL;
static CSUDI_BOOL	    g_bTestFinish = CSUDI_FALSE; //���Խ���
static CSUDI_BOOL		g_bFinish = CSUDI_FALSE;  

CSUDI_BOOL SECTION_iSetOCReqFilter( CSUDISECTIONRequestInfo_S * psReqInfo ,int nFilterIndex,int nDemuxIndex)
{
	if(nFilterIndex >= EM_SECTION_OC_FILTER_MAX || nFilterIndex < EM_SECTION_OC_FILTER001) 
	{
		return CSUDI_FALSE;
	}
	
	if(psReqInfo != CSUDI_NULL)
	{
		psReqInfo->m_eReqType = EM_UDI_REQ_SECTION_DATA;
		psReqInfo->m_nDemuxID = nDemuxIndex;
		psReqInfo->m_nPID = 0x5de;
		
		memset(&psReqInfo->m_sMMFilter, 0, sizeof(psReqInfo->m_sMMFilter));
		memcpy(&psReqInfo->m_sMMFilter,&g_sMMFilter[nFilterIndex],sizeof(psReqInfo->m_sMMFilter));
		
		psReqInfo->m_bCRCCheck = CSUDI_TRUE;
	}
	return CSUDI_TRUE;   
}

static void SectionMonitorThread(void *pvArg)
{	
	int 		i = 0;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	int 		nFinishNum = 0;
	
	while(!g_bFinish)
	{
		CSUDIOSSemWait(g_hSectInfoSem,500);
		
		nFinishNum = 0;
		
		for(i=0; i<(TEST_FILTER_NUM-1); i++)
		{
			if (g_sSecInfo[i].bStatus == SectionFinish)
			{
				nRet = CSUDIFILTERFree(g_sSecInfo[i].hRequest);
				if(nRet != CSUDI_SUCCESS)
				{
					CSTCPrint("ȡ����%d ����������ʧ��\n",i);
					break;
				}
				g_sSecInfo[i].bStatus = SectionFree;
			}

			if (g_sSecInfo[i].bStatus == SectionFree)
			{
				nFinishNum++;
			}
		}
		
		if (nFinishNum == (TEST_FILTER_NUM-1))
		{
			break;
		}
	}
	
	/* tell SECTION_iTest_IT_20 if we return by error*/
	g_bFinish = (nRet == CSUDI_SUCCESS) && (nFinishNum == (TEST_FILTER_NUM-1)); 
	
	g_bTestFinish = CSUDI_TRUE;
}
static void SECTION_iOCTestCallback(CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	int nIdx;
	int nLastNum;
	int nSecNum;
	int nIndex;
	int i;
	CSUDISECTIONCallbackData_S * pSection;
	if(eType != EM_UDI_REQ_SECTION_DATA) 
		return;
	
	pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	nIdx = (int)pvUserData;
	nLastNum = pSection->m_pucData[7];
	nSecNum  = pSection->m_pucData[6];
	
	if(g_sSecInfo[nIdx].nLastNum == -1)
	{
		g_sSecInfo[nIdx].nLastNum = nLastNum;
	}
	if(g_sSecInfo[nIdx].bStatus == SectionFinish)
	{
		return;
	}

	if (g_sSecInfo[nIdx].pSectionTrace == CSUDI_NULL)
	{
		/*����3*nLastNum�Ŀռ䣬������ǰһ��nLastNum�ռ䰴�򱣴棬�Լ���Ƿ��Ѿ����꣬
		 ���������������������������ֶ�δ�������ټ�����*/
		g_sSecInfo[nIdx].pSectionTrace = CSUDIOSMalloc(3*(g_sSecInfo[nIdx].nLastNum+1));
		if (CSUDI_NULL == g_sSecInfo[nIdx].pSectionTrace)
		{
			CSTCPrint("�����%d ������������section �Ÿ����ڴ�ʧ��\n",nIdx);
			g_sSecInfo[nIdx].bStatus = SectionFinish;
			CSUDIOSSemRelease(g_hSectInfoSem);
		}
		else
		{
			memset(g_sSecInfo[nIdx].pSectionTrace,0xff,3*(g_sSecInfo[nIdx].nLastNum+1));
		}
	}

	if (g_sSecInfo[nIdx].pSectionTrace != CSUDI_NULL)
	{
		nIndex = g_sSecInfo[nIdx].nTraceIndex + g_sSecInfo[nIdx].nLastNum + 1;
		/*������ֶ���û�����꣬���ٽ���*/
		if (g_sSecInfo[nIdx].nTraceIndex >= 2*(g_sSecInfo[nIdx].nLastNum+1) )
		{
			g_sSecInfo[nIdx].bStatus = SectionFinish;
			CSUDIOSSemRelease(g_hSectInfoSem);
		}
		else
		{
			g_sSecInfo[nIdx].pSectionTrace[nIndex] = nSecNum;  //���հ���˳���¼
			
			if (g_sSecInfo[nIdx].nTraceIndex != 0)
			{
				if ((g_sSecInfo[nIdx].pSectionTrace[nIndex] != (1 + g_sSecInfo[nIdx].pSectionTrace[nIndex-1]))
					&& g_sSecInfo[nIdx].pSectionTrace[nIndex])//�����������գ�����
				{
					g_sSecInfo[nIdx].bDropPacket = CSUDI_TRUE;
				}
			}
			
			g_sSecInfo[nIdx].nTraceIndex++;
			
			if(nSecNum <= g_sSecInfo[nIdx].nLastNum)
			{
				g_sSecInfo[nIdx].pSectionTrace[nSecNum] = nSecNum; ///��˳���¼���Ľ���
				
				for(i=0;i <= g_sSecInfo[nIdx].nLastNum;i++)
				{
					if (g_sSecInfo[nIdx].pSectionTrace[i] != i)
						break;
				}
				
				if (i > g_sSecInfo[nIdx].nLastNum)
				{
					g_sSecInfo[nIdx].bStatus = SectionFinish;
					CSUDIOSSemRelease(g_hSectInfoSem);
				}
			}
			else
			{
				CSTCPrint("��%d�����������յ��쳣����ȡ��!(nSecNum,nLastNum)(%d,%d)\n",
					nIdx,nSecNum,g_sSecInfo[nIdx].nLastNum);
				
				g_sSecInfo[nIdx].bStatus = SectionFinish;
				
				CSUDIOSSemRelease(g_hSectInfoSem);
			}
		}
	}

	
}

static void SECTION_iOCTestPrintCallback(CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	int nIdx;
	int nLastNum;
	int nSecNum;
	int nIndex;
	int i;
	int j;
	CSUDISECTIONCallbackData_S * pSection;
	if(eType != EM_UDI_REQ_SECTION_DATA) 
		return;
	
	pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	nIdx = (int)pvUserData;
	nLastNum = pSection->m_pucData[7];
	nSecNum  = pSection->m_pucData[6];
	
	if(g_sSecInfo[nIdx].nLastNum == -1)
	{
		g_sSecInfo[nIdx].nLastNum = nLastNum;
	}
	if(g_sSecInfo[nIdx].bStatus == SectionFinish)
	{
		return;
	}

	CSTCPrint("\r\nnIdx=0x%02x,secNum=%02d,lastNum=%02d,tableExt=0x%02x%02x\r\n",
		nIdx,
		pSection->m_pucData[6],
		pSection->m_pucData[7],
		pSection->m_pucData[3],
		pSection->m_pucData[4]
		);

	for( j = 0; j < 10; j++)
	{
		CSTCPrint("������section�ص��мӴ�ӡ���%d���Ƿ񶪰�\n", j);
	}
	
	if (g_sSecInfo[nIdx].pSectionTrace == CSUDI_NULL)
	{
		/*����3*nLastNum�Ŀռ䣬������ǰһ��nLastNum�ռ䰴�򱣴棬�Լ���Ƿ��Ѿ����꣬
		 ���������������������������ֶ�δ�������ټ�����*/
		g_sSecInfo[nIdx].pSectionTrace = CSUDIOSMalloc(3*(g_sSecInfo[nIdx].nLastNum+1));
		if (CSUDI_NULL == g_sSecInfo[nIdx].pSectionTrace)
		{
			CSTCPrint("�����%d ������������section �Ÿ����ڴ�ʧ��\n",nIdx);
			g_sSecInfo[nIdx].bStatus = SectionFinish;
			CSUDIOSSemRelease(g_hSectInfoSem);
		}
		else
		{
			memset(g_sSecInfo[nIdx].pSectionTrace,0xff,3*(g_sSecInfo[nIdx].nLastNum+1));
		}
	}

	if (g_sSecInfo[nIdx].pSectionTrace != CSUDI_NULL)
	{
		nIndex = g_sSecInfo[nIdx].nTraceIndex + g_sSecInfo[nIdx].nLastNum + 1;
		/*������ֶ���û�����꣬���ٽ���*/
		if (g_sSecInfo[nIdx].nTraceIndex >= 2*(g_sSecInfo[nIdx].nLastNum+1) )
		{
			g_sSecInfo[nIdx].bStatus = SectionFinish;
			CSUDIOSSemRelease(g_hSectInfoSem);
		}
		else
		{
			g_sSecInfo[nIdx].pSectionTrace[nIndex] = nSecNum;  //���հ���˳���¼
			
			if (g_sSecInfo[nIdx].nTraceIndex != 0)
			{
				if ((g_sSecInfo[nIdx].pSectionTrace[nIndex] != (1 + g_sSecInfo[nIdx].pSectionTrace[nIndex-1]))
					&& g_sSecInfo[nIdx].pSectionTrace[nIndex])//�����������գ�����
				{
					g_sSecInfo[nIdx].bDropPacket = CSUDI_TRUE;
				}
			}
			
			g_sSecInfo[nIdx].nTraceIndex++;
			
			if(nSecNum <= g_sSecInfo[nIdx].nLastNum)
			{
				g_sSecInfo[nIdx].pSectionTrace[nSecNum] = nSecNum; ///��˳���¼���Ľ���
				
				for(i=0;i <= g_sSecInfo[nIdx].nLastNum;i++)
				{
					if (g_sSecInfo[nIdx].pSectionTrace[i] != i)
						break;
				}
				
				if (i > g_sSecInfo[nIdx].nLastNum)
				{
					g_sSecInfo[nIdx].bStatus = SectionFinish;
					CSUDIOSSemRelease(g_hSectInfoSem);
				}
			}
			else
			{
				CSTCPrint("��%d�����������յ��쳣����ȡ��!(nSecNum,nLastNum)(%d,%d)\n",
					nIdx,nSecNum,g_sSecInfo[nIdx].nLastNum);
				
				g_sSecInfo[nIdx].bStatus = SectionFinish;
				
				CSUDIOSSemRelease(g_hSectInfoSem);
			}
		}
	}

	
}

CSUDI_BOOL SECTION_iTest_IT_20(void)
{
	CSUDI_HANDLE  hMonitorThread = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	unsigned int nStartTime =0;
	unsigned int nEndTime  =0;
	int i = 0;
	int nIndex = 0;
	int nFilterIndex = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	
	memset(g_sSecInfo,0,sizeof(g_sSecInfo));
	
	SECTION_GOTO(
		(CSUDI_SUCCESS == CSUDIOSSemCreate("Semfor_IT_20", 0, 1, &g_hSectInfoSem)),
		EXISTLABLE,
		"create semphore failure !!!"
		);
	SECTION_GOTO(
		(CSUDI_SUCCESS == CSUDIOSThreadCreate("SectionMonitorThread", 7*17, 1024*16, SectionMonitorThread,CSUDI_NULL,&hMonitorThread)),
		EXISTLABLE,
		"create SectionMonitorThread failure !!!"
		);
	
	nStartTime = CSUDIPlusOSGetTickCount();
	
	for(i=1; i<TEST_FILTER_NUM; i++)
	{
		nIndex = i % 4;
		if(i%4 == 0)
		{
			nIndex = 4;
		}
		switch(nIndex)
		{
			case 1:
				nFilterIndex = EM_SECTION_OC_FILTER001;
				break;
			case 2:
				nFilterIndex = EM_SECTION_OC_FILTER002;
				break;
			case 3:
				nFilterIndex = EM_SECTION_OC_FILTER003;
				break;
		 	case 4:
				nFilterIndex = EM_SECTION_OC_FILTER004;
				break;
		}
		
		bRet = SECTION_iSetOCReqFilter(&sTmpReqInfo,nFilterIndex,DEMUX_0);
		
		if(bRet != CSUDI_TRUE)
		{
			CSTCPrint("Set FilterRequest %d Failuer !!!\n",nIndex);
				SECTION_GOTO(bRet == CSUDI_TRUE,EXISTLABLE,"Set FilterRequest !!!");
		}
		
		g_sSecInfo[i-1].nLastNum = -1;
		g_sSecInfo[i-1].nTraceIndex = 0;
		
		nRet = CSUDIFILTERAllocate(&sTmpReqInfo, SECTION_iOCTestCallback, (void*)(i-1), &g_sSecInfo[i-1].hRequest);
		CSTCPrint("Allocate Filter %d Success.......... !!!\n",(i-1));
		if(nRet != CSUDI_SUCCESS)
		{
			CSTCPrint("Allocate Filter %d Failuer !!!\n",(i-1));
			SECTION_GOTO((CSUDI_SUCCESS == nRet),EXISTLABLE,"Allocate Filter Failuer !!!");
		}

		nRet = CSUDIFILTERSetBufSize(g_sSecInfo[i-1].hRequest, 1024*256);
		if(nRet != CSUDI_SUCCESS)
		{
			CSTCPrint("Set Filter %d buffersize Failuer !!!\n",(i-1));
		}
		
		nRet = CSUDIFILTERStart(g_sSecInfo[i-1].hRequest);
		if(nRet != CSUDI_SUCCESS)
		{
			CSTCPrint("Start Filter %d  Failuer !!!\n",(i-1));
			SECTION_GOTO((CSUDI_SUCCESS == nRet),EXISTLABLE,"Start Filter Failuer !!!");
		}
		g_sSecInfo[i-1].bStatus = SectionStart;
	}
	
	CSTCPrint("Waiting !! about(10s).........\n");
	i = 50;
	while((i-- >= 0) && (!g_bTestFinish))
	{
		CSUDIOSThreadSleep(200); /*�ȴ�10s,���ʱ�䵽��û���������˳�*/
	}
	
	nEndTime = CSUDIPlusOSGetTickCount();
	
EXISTLABLE:
	{
		g_bFinish = CSUDI_TRUE;
		
		i = 2;
		
		while((!g_bTestFinish)&&(i-- > 0)) CSUDIOSThreadSleep(500);
		
		bRet = CSUDI_TRUE;
		
		for(i = 0; i < TEST_FILTER_NUM-1; i++)
		{
			/*���û���κΰ��յ���monitor�̲߳����ܵ�,������Ҫ�����ｫ����filterȡ�����Ը�λ�ֳ�*/
			if(g_sSecInfo[i].bStatus != SectionFree && g_sSecInfo[i].bStatus != SectionStop)
			{
				nRet = CSUDIFILTERFree(g_sSecInfo[i].hRequest);
				if(nRet != CSUDI_SUCCESS)
				{
					CSTCPrint("Free Filter %d failure \n",i);
				}
				g_sSecInfo[i].bStatus = SectionFree;
			}
			if(g_sSecInfo[i].bDropPacket && g_sSecInfo[i].pSectionTrace != CSUDI_NULL)
			{
				CSTCPrint("The %d Filter got dropped packets, the sequence is (lastNum:%d):\n",i,g_sSecInfo[i].nLastNum);
				for(nIndex=0;nIndex < g_sSecInfo[i].nTraceIndex; nIndex++)
				{
					CSTCPrint("%d ",g_sSecInfo[i].pSectionTrace[nIndex + g_sSecInfo[i].nLastNum + 1 ]);
				}
				CSTCPrint("\n");
				bRet = CSUDI_FALSE;
			}
			if(!g_sSecInfo[i].bDropPacket && g_sSecInfo[i].nLastNum > 0)
			{
				for(nIndex = 0;nIndex <= g_sSecInfo[i].nLastNum;nIndex++)
				{
					if (g_sSecInfo[i].pSectionTrace[nIndex] != nIndex)  break;
				}
				
				if (nIndex > g_sSecInfo[i].nLastNum)
				{
					//CSTCPrint("��%d����������û�ж���\n",i);
					CSTCPrint("the %dth filter is OK!\n",i);
				}
				else
				{
					//CSTCPrint("��%d������������û����ȫ\n",i);
					CSTCPrint("the %dth filter can't recieve all section:\n",i);
					for(nIndex = 0;nIndex <(g_sSecInfo[i].nLastNum+1)*3;nIndex++)
					{
						CSTCPrint("%x ",g_sSecInfo[i].pSectionTrace[nIndex]);
					}
					CSTCPrint("\n");
					bRet = CSUDI_FALSE;
				}
			}
			if (g_sSecInfo[i].nLastNum < 0)
			{
				CSTCPrint("��%d����������û���յ��κΰ�,��������������Ƿ�Ϊoc_test.ts\n",i);
				bRet = CSUDI_FALSE;
			}
			if (g_sSecInfo[i].bStatus == SectionStop)
			{
				CSTCPrint("��%d�����������Ѿ�ֹͣ\n",i);
				bRet = CSUDI_FALSE;
			}
			if (g_sSecInfo[i].pSectionTrace != CSUDI_NULL)
			{
				CSUDIOSFree(g_sSecInfo[i].pSectionTrace);
				g_sSecInfo[i].pSectionTrace = CSUDI_NULL;
			}
		}
		
		if(g_hSectInfoSem != CSUDI_NULL)
		{
			CSUDIOSSemDestroy(g_hSectInfoSem);
			g_hSectInfoSem = CSUDI_NULL;
		}
		if(hMonitorThread != CSUDI_NULL)
		{
			CSUDIOSThreadDestroy(hMonitorThread);
			hMonitorThread = CSUDI_NULL;
		}
		g_bFinish = CSUDI_FALSE;
		g_bTestFinish = CSUDI_FALSE;
		
		if(bRet)
		{
			CSTCPrint("�հ�����ʱ��:%d ms\n",nEndTime-nStartTime);
		}

		
		CSTCPrint("�������\n");
	}
	return bRet;
}

CSUDI_BOOL SECTION_iTest_IT_21(int nFilterIndex)
{
	CSUDI_HANDLE  hMonitorThread = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	unsigned int nStartTime =0;
	unsigned int nEndTime  =0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	int i = 0;
	int j = 0;
	int nIndex = 0;

	if( nFilterIndex >= EM_SECTION_OC_FILTER_MAX || nFilterIndex < EM_SECTION_OC_FILTER001)
	{
		CSTCPrint("�Ƿ���nFilterIndex.......... !!!\n");
		return CSUDI_FALSE;
	}

	memset(g_sSecInfo,0,sizeof(g_sSecInfo));

	if(CSUDI_SUCCESS != CSUDIOSSemCreate("Semfor_IT_20", 0, 1, &g_hSectInfoSem))
	{
		CSTCPrint("Create OSSem Fail.......... !!!\n");
		return CSUDI_FALSE;
	}

	if(CSUDI_SUCCESS != CSUDIOSThreadCreate("SectionMonitorThread", 7*17, 1024*16, SectionMonitorThread,CSUDI_NULL,&hMonitorThread))
	{
		CSTCPrint("Create Thread Fail.......... !!!\n");
		return CSUDI_FALSE;
	}
	
	if(nFilterIndex == EM_SECTION_OC_FILTER002)
	{
		i = 1;
	}

	if(nFilterIndex == EM_SECTION_OC_FILTER003)
	{
		i = 2;
	}

	if(nFilterIndex == EM_SECTION_OC_FILTER004)
	{
		i = 3;
	}
	
	bRet = SECTION_iSetOCReqFilter(&sTmpReqInfo,nFilterIndex,DEMUX_0);
	if(!bRet)
	{
		CSTCPrint("Filter��ʼ��ʧ��");
		return CSUDI_FALSE;
	}

	g_sSecInfo[i].nLastNum = -1;
	g_sSecInfo[i].nTraceIndex = 0;

	nRet = CSUDIFILTERAllocate(&sTmpReqInfo, SECTION_iOCTestCallback, (void*)i, &g_sSecInfo[i].hRequest);
	if(nRet == CSUDI_SUCCESS)
	{
		CSTCPrint("Allocate Filter %d Success.......... !!!\n",i);
	}
	else
	{
		CSTCPrint("Allocate Filter %d Failuer !!!\n",i);
		CSTK_ASSERT_TRUE_FATAL(nRet, "Allocate Filter Failuer !!!");
	}

	nRet = CSUDIFILTERSetBufSize(g_sSecInfo[i].hRequest, 1024*256);
	if(nRet != CSUDI_SUCCESS)
	{
		CSTCPrint("Set Filter %d buffersize Failuer !!!\n",i);
	}
	
	nStartTime = CSUDIPlusOSGetTickCount();
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIFILTERStart(g_sSecInfo[i].hRequest)), "Start Filter Failuer !!!");
	g_sSecInfo[i].bStatus = SectionStart;
	
	CSTCPrint("Waiting !! about(10s).........\n");
	j = 50;
	while((j-- >= 0) && (!g_bTestFinish))
	{
		CSUDIOSThreadSleep(200); /*�ȴ�10s,���ʱ�䵽��û���������˳�*/
	}
	
	nEndTime = CSUDIPlusOSGetTickCount();

	CSTK_FATAL_POINT

	g_bFinish = CSUDI_TRUE;
		
	j = 2;
		
	while((!g_bTestFinish)&&(j-- > 0)) CSUDIOSThreadSleep(500);
		
	bRet = CSUDI_TRUE;

	if(g_sSecInfo[i].bStatus != SectionFree && g_sSecInfo[i].bStatus != SectionStop)
	{
		nRet = CSUDIFILTERFree(g_sSecInfo[i].hRequest);
		if(nRet != CSUDI_SUCCESS)
		{
			CSTCPrint("Free Filter %d failure \n",i);
		}
		g_sSecInfo[i].bStatus = SectionFree;
	}

	if(!g_sSecInfo[i].bDropPacket && g_sSecInfo[i].nLastNum > 0)
	{
		for(nIndex = 0;nIndex <= g_sSecInfo[i].nLastNum;nIndex++)
		{
			if (g_sSecInfo[i].pSectionTrace[nIndex] != nIndex)  break;
		}
		
		if (nIndex > g_sSecInfo[i].nLastNum)
		{
			CSTCPrint("the %dth filter is OK!\n",i);
		}
		else
		{
			CSTCPrint("the %dth filter can't recieve all section:\n",i);
			for(nIndex = 0;nIndex <(g_sSecInfo[i].nLastNum+1)*3;nIndex++)
			{
				CSTCPrint("%x ",g_sSecInfo[i].pSectionTrace[nIndex]);
			}
			CSTCPrint("\n");
			bRet = CSUDI_FALSE;
		}
	}
	if (g_sSecInfo[i].nLastNum < 0)
	{
		CSTCPrint("��%d����������û���յ��κΰ�,��������������Ƿ�Ϊoc_test.ts\n",i);
		bRet = CSUDI_FALSE;
	}
	if (g_sSecInfo[i].bStatus == SectionStop)
	{
		CSTCPrint("��%d�����������Ѿ�ֹͣ\n",i);
		bRet = CSUDI_FALSE;
	}
	if (g_sSecInfo[i].pSectionTrace != CSUDI_NULL)
	{
		CSUDIOSFree(g_sSecInfo[i].pSectionTrace);
		g_sSecInfo[i].pSectionTrace = CSUDI_NULL;
	}

	if(g_hSectInfoSem != CSUDI_NULL)
	{
		CSUDIOSSemDestroy(g_hSectInfoSem);
		g_hSectInfoSem = CSUDI_NULL;
	}
	if(hMonitorThread != CSUDI_NULL)
	{
		CSUDIOSThreadDestroy(hMonitorThread);
		hMonitorThread = CSUDI_NULL;
	}

	g_bFinish = CSUDI_FALSE;
	g_bTestFinish = CSUDI_FALSE;
	
	if(bRet)
	{
		CSTCPrint("ͨ��filter %d�հ�����ʱ��Ϊ%d ms.\n",i,nEndTime-nStartTime);
	}

	CSTCPrint("�������\n");
	
	return bRet;
}

CSUDI_BOOL SECTION_iTest_IT_22(int nFilterNum)
{
	CSUDI_HANDLE  hMonitorThread = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	unsigned int nStartTime =0;
	unsigned int nEndTime  =0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	int i = 0;
	int j = 0;
	int nIndex = 0;

	if( nFilterNum >= EM_SECTION_OC_FILTER_MAX || nFilterNum < EM_SECTION_OC_FILTER001)
	{
		CSTCPrint("�Ƿ���nFilterNum.......... !!!\n");
		return CSUDI_FALSE;
	}

	memset(g_sSecInfo,0,sizeof(g_sSecInfo));

	if(CSUDI_SUCCESS != CSUDIOSSemCreate("Semfor_IT_20", 0, 1, &g_hSectInfoSem))
	{
		CSTCPrint("Create OSSem Fail.......... !!!\n");
		return CSUDI_FALSE;
	}

	if(CSUDI_SUCCESS != CSUDIOSThreadCreate("SectionMonitorThread", 7*17, 1024*16, SectionMonitorThread,CSUDI_NULL,&hMonitorThread))
	{
		CSTCPrint("Create Thread Fail.......... !!!\n");
		return CSUDI_FALSE;
	}

	for( i = 0; i <= (nFilterNum-EM_SECTION_OC_FILTER001); i++)
	{
		bRet = SECTION_iSetOCReqFilter(&sTmpReqInfo,(EM_SECTION_OC_FILTER001+i),DEMUX_0);
		if(!bRet)
		{
			CSTCPrint("Filter��ʼ��ʧ��");
			return CSUDI_FALSE;
		}

		g_sSecInfo[i].nLastNum = -1;
		g_sSecInfo[i].nTraceIndex = 0;

		nRet = CSUDIFILTERAllocate(&sTmpReqInfo, SECTION_iOCTestPrintCallback, (void*)i, &g_sSecInfo[i].hRequest);
		if(nRet == CSUDI_SUCCESS)
		{
			CSTCPrint("Allocate Filter %d Success.......... !!!\n",i);
		}
		else
		{
			CSTCPrint("Allocate Filter %d Failuer !!!\n",i);
			CSTK_ASSERT_TRUE_FATAL(nRet, "Allocate Filter Failuer !!!");
		}

		nRet = CSUDIFILTERSetBufSize(g_sSecInfo[i].hRequest, 1024*256);
		if(nRet != CSUDI_SUCCESS)
		{
			CSTCPrint("Set Filter %d buffersize Failuer !!!\n",i);
		}

		nStartTime = CSUDIPlusOSGetTickCount();

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIFILTERStart(g_sSecInfo[i].hRequest)), "Start Filter Failuer !!!");
		g_sSecInfo[i].bStatus = SectionStart;
	}

	CSTCPrint("Waiting !! about(10s).........\n");
	j = 50;
	while((j-- >= 0) && (!g_bTestFinish))
	{
		CSUDIOSThreadSleep(200); /*�ȴ�10s,���ʱ�䵽��û���������˳�*/
	}

	nEndTime = CSUDIPlusOSGetTickCount();
	
	CSTK_FATAL_POINT

	g_bFinish = CSUDI_TRUE;
		
	j = 2;
		
	while((!g_bTestFinish)&&(j-- > 0)) CSUDIOSThreadSleep(500);
		
	bRet = CSUDI_TRUE;

	for( i = 0; i <= (nFilterNum -EM_SECTION_OC_FILTER001); i++)
	{
		if(g_sSecInfo[i].bStatus != SectionFree && g_sSecInfo[i].bStatus != SectionStop)
		{
			nRet = CSUDIFILTERFree(g_sSecInfo[i].hRequest);
			if(nRet != CSUDI_SUCCESS)
			{
				CSTCPrint("Free Filter %d failure \n",i);
			}
			g_sSecInfo[i].bStatus = SectionFree;
		}

		if(!g_sSecInfo[i].bDropPacket && g_sSecInfo[i].nLastNum > 0)
		{
			for(nIndex = 0;nIndex <= g_sSecInfo[i].nLastNum;nIndex++)
			{
				if (g_sSecInfo[i].pSectionTrace[nIndex] != nIndex)  break;
			}
			
			if (nIndex > g_sSecInfo[i].nLastNum)
			{
				CSTCPrint("the %dth filter is OK!\n",i);
			}
			else
			{
				CSTCPrint("the %dth filter can't recieve all section:\n",i);
				for(nIndex = 0;nIndex <(g_sSecInfo[i].nLastNum+1)*3;nIndex++)
				{
					CSTCPrint("%x ",g_sSecInfo[i].pSectionTrace[nIndex]);
				}
				CSTCPrint("\n");
				bRet = CSUDI_FALSE;
			}
		}
		if (g_sSecInfo[i].nLastNum < 0)
		{
			CSTCPrint("��%d����������û���յ��κΰ�,��������������Ƿ�Ϊoc_test.ts\n",i);
			bRet = CSUDI_FALSE;
		}
		if (g_sSecInfo[i].bStatus == SectionStop)
		{
			CSTCPrint("��%d�����������Ѿ�ֹͣ\n",i);
			bRet = CSUDI_FALSE;
		}
		if (g_sSecInfo[i].pSectionTrace != CSUDI_NULL)
		{
			CSUDIOSFree(g_sSecInfo[i].pSectionTrace);
			g_sSecInfo[i].pSectionTrace = CSUDI_NULL;
		}
	}
	if(g_hSectInfoSem != CSUDI_NULL)
	{
		CSUDIOSSemDestroy(g_hSectInfoSem);
		g_hSectInfoSem = CSUDI_NULL;
	}
	if(hMonitorThread != CSUDI_NULL)
	{
		CSUDIOSThreadDestroy(hMonitorThread);
		hMonitorThread = CSUDI_NULL;
	}

	g_bFinish = CSUDI_FALSE;
	g_bTestFinish = CSUDI_FALSE;
	
	if(bRet)
	{
		CSTCPrint("ͨ��filter %d�հ�����ʱ��Ϊ%d ms.\n",i,nEndTime-nStartTime);
	}

	CSTCPrint("�������\n");
	
	return bRet;
}

//���CSTC_SECTION_IT_0039ע��
CSUDI_BOOL SECTION_iTest_IT_39(void)
{
	CSUDI_HANDLE  hMonitorThread = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_BOOL bRet = CSUDI_TRUE;
	unsigned int nStartTime =0;
	unsigned int nEndTime  =0;
	int i = 0;
	int nIndex = 0;
	int nFilterIndex = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	
	memset(g_sSecInfo,0,sizeof(g_sSecInfo));
	
	SECTION_GOTO(
		(CSUDI_SUCCESS == CSUDIOSSemCreate("Semfor_IT_39", 0, 1, &g_hSectInfoSem)),
		EXISTLABLE,
		"create semphore failure !!!"
		);
	SECTION_GOTO(
		(CSUDI_SUCCESS == CSUDIOSThreadCreate("SectionMonitorThread", 7*17, 1024*16, SectionMonitorThread,CSUDI_NULL,&hMonitorThread)),
		EXISTLABLE,
		"create SectionMonitorThread failure !!!"
		);
	
	nStartTime = CSUDIPlusOSGetTickCount();
	
	for(nIndex=1; nIndex<TEST_FILTER_NUM; nIndex++)
	{
		switch(nIndex)
		{
			case 1:
				nFilterIndex = EM_SECTION_OC_FILTER001;
				break;
			case 2:
				nFilterIndex = EM_SECTION_OC_FILTER002;
				break;
			case 3:
				nFilterIndex = EM_SECTION_OC_FILTER003;
				break;
		 	case 4:
				nFilterIndex = EM_SECTION_OC_FILTER004;
				break;
			case 5:
				nFilterIndex = EM_SECTION_OC_FILTER005;
				break;
			case 6:
				nFilterIndex = EM_SECTION_OC_FILTER006;
				break;
			case 7:
				nFilterIndex = EM_SECTION_OC_FILTER007;
				break;
		 	case 8:
				nFilterIndex = EM_SECTION_OC_FILTER008;
				break;
			case 9:
				nFilterIndex = EM_SECTION_OC_FILTER009;
				break;
			case 10:
				nFilterIndex = EM_SECTION_OC_FILTER003;
				break;
			case 11:
				nFilterIndex = EM_SECTION_OC_FILTER004;
				break;
			case 12:
				nFilterIndex = EM_SECTION_OC_FILTER003;
				break;
		 	case 13:
				nFilterIndex = EM_SECTION_OC_FILTER004;
				break;
			case 14:
				nFilterIndex = EM_SECTION_OC_FILTER003;
				break;
		 	case 15:
				nFilterIndex = EM_SECTION_OC_FILTER004;
				break;	
		}
		i = nIndex;
		
		bRet = SECTION_iSetOCReqFilter(&sTmpReqInfo,nFilterIndex,DEMUX_0);
		
		if(bRet != CSUDI_TRUE)
		{
			CSTCPrint("Set FilterRequest %d Failuer !!!\n",nIndex);
				SECTION_GOTO(bRet == CSUDI_TRUE,EXISTLABLE,"Set FilterRequest !!!");
		}
		
		g_sSecInfo[i-1].nLastNum = -1;
		g_sSecInfo[i-1].nTraceIndex = 0;
		
		nRet = CSUDIFILTERAllocate(&sTmpReqInfo, SECTION_iOCTestCallback, (void*)(i-1), &g_sSecInfo[i-1].hRequest);
		CSTCPrint("Allocate Filter %d Success.......... !!!\n",(i-1));
		if(nRet != CSUDI_SUCCESS)
		{
			CSTCPrint("Allocate Filter %d Failuer !!!\n",(i-1));
			SECTION_GOTO((CSUDI_SUCCESS == nRet),EXISTLABLE,"Allocate Filter Failuer !!!");
		}

		nRet = CSUDIFILTERSetBufSize(g_sSecInfo[i-1].hRequest, 1024*512);
		if(nRet != CSUDI_SUCCESS)
		{
			CSTCPrint("Set Filter %d buffersize Failuer !!!\n",(i-1));
		}
		
		nRet = CSUDIFILTERStart(g_sSecInfo[i-1].hRequest);
		if(nRet != CSUDI_SUCCESS)
		{
			CSTCPrint("Start Filter %d  Failuer !!!\n",(i-1));
			SECTION_GOTO((CSUDI_SUCCESS == nRet),EXISTLABLE,"Start Filter Failuer !!!");
		}
		g_sSecInfo[i-1].bStatus = SectionStart;
	}
	
	CSTCPrint("Waiting !! about(20s).........\n");
	i = 100;
	while((i-- >= 0) && (!g_bTestFinish))
	{
		CSUDIOSThreadSleep(200); /*�ȴ�20s,���ʱ�䵽��û���������˳�*/
	}
	
	nEndTime = CSUDIPlusOSGetTickCount();
	
EXISTLABLE:
	{
		g_bFinish = CSUDI_TRUE;
		
		i = 40;
		
		while((!g_bTestFinish)&&(i-- > 0)) CSUDIOSThreadSleep(500);
		
		bRet = CSUDI_TRUE;
		
		for(i = 0; i < TEST_FILTER_NUM-1; i++)
		{
			/*���û���κΰ��յ���monitor�̲߳����ܵ�,������Ҫ�����ｫ����filterȡ�����Ը�λ�ֳ�*/
			if(g_sSecInfo[i].bStatus != SectionFree && g_sSecInfo[i].bStatus != SectionStop)
			{
				CSTCPrint("g_sSecInfo[%d].bStatus = %d \n",i,g_sSecInfo[i].bStatus);
				nRet = CSUDIFILTERFree(g_sSecInfo[i].hRequest);
				if(nRet != CSUDI_SUCCESS)
				{
					CSTCPrint("Free Filter %d failure \n",i);
				}
				g_sSecInfo[i].bStatus = SectionFree;
			}
			if(g_sSecInfo[i].bDropPacket && g_sSecInfo[i].pSectionTrace != CSUDI_NULL)
			{
				CSTCPrint("The %d Filter got dropped packets, the sequence is (lastNum:%d):\n",i,g_sSecInfo[i].nLastNum);
				for(nIndex=0;nIndex < g_sSecInfo[i].nTraceIndex; nIndex++)
				{
					CSTCPrint("%d ",g_sSecInfo[i].pSectionTrace[nIndex + g_sSecInfo[i].nLastNum + 1 ]);
				}
				CSTCPrint("\n");
				bRet = CSUDI_FALSE;
			}
			
			if(!g_sSecInfo[i].bDropPacket && g_sSecInfo[i].nLastNum >= 0)
			{
				for(nIndex = 0;nIndex <= g_sSecInfo[i].nLastNum;nIndex++)
				{
					if (g_sSecInfo[i].pSectionTrace[nIndex] != nIndex)  break;
				}
				
				if (nIndex > g_sSecInfo[i].nLastNum)
				{
					CSTCPrint("the %dth filter is OK!\n",i);
				}
				else
				{
					CSTCPrint("the %dth filter can't recieve all section:\n",i);
					for(nIndex = 0;nIndex <(g_sSecInfo[i].nLastNum+1)*3;nIndex++)
					{
						CSTCPrint("%x ",g_sSecInfo[i].pSectionTrace[nIndex]);
					}
					CSTCPrint("\n");
					bRet = CSUDI_FALSE;
				}
			}
			if (g_sSecInfo[i].nLastNum < 0)
			{
				CSTCPrint("��%d����������û���յ��κΰ�,��������������Ƿ�Ϊoc_test.ts\n",i);
				bRet = CSUDI_FALSE;
			}
			if (g_sSecInfo[i].bStatus == SectionStop)
			{
				CSTCPrint("��%d�����������Ѿ�ֹͣ\n",i);
				bRet = CSUDI_FALSE;
			}
			if (g_sSecInfo[i].pSectionTrace != CSUDI_NULL)
			{
				CSUDIOSFree(g_sSecInfo[i].pSectionTrace);
				g_sSecInfo[i].pSectionTrace = CSUDI_NULL;
			}
		}
		
		if(g_hSectInfoSem != CSUDI_NULL)
		{
			CSUDIOSSemDestroy(g_hSectInfoSem);
			g_hSectInfoSem = CSUDI_NULL;
		}
		if(hMonitorThread != CSUDI_NULL)
		{
			CSUDIOSThreadDestroy(hMonitorThread);
			hMonitorThread = CSUDI_NULL;
		}
		g_bFinish = CSUDI_FALSE;
		g_bTestFinish = CSUDI_FALSE;
		
		if(bRet)
		{
			CSTCPrint("�հ�����ʱ��:%d ms\n",nEndTime-nStartTime);
		}

		
		CSTCPrint("�������\n");
	}
	return bRet;
}

