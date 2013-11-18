/**
 *@version 1.0.1 2009/11/06  �ع�ԭʼ�汾���޸ĸ������߽ӿڣ��Ż�����
 *@version 1.0.2 2009/11/05  �ع�ԭʼ�汾���޸����ò���������Ϊͳһ�ӿ�
 *@version 1.0.1 2009/11/01  �ع�ԭʼ�汾���޸���Ƶ��ʽΪͳһ�ӿ�
*/
/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_livetestcase.h"
#include "udi2_player.h"
#include "udi2_demux.h"
#include "udi2_audio.h"
#include "udi2_video.h"
#include "udi2_tuner.h"
#include "udi2_os.h"
#include <stdlib.h>
#include "../../cs_udi2testcase.h"
#include "../cs_playertest.h"
#include "udiplus_os.h"

#define NUSERDATA (1234)
#define NUSERDATATEMP (5678)
#define ERRORDATA (2009)
#define SLEEPTIME (1000)
#define UDI2_TUNER0_ID 0
#define PLAYER_WAIT_TIMES (10*1000)
#define INVALIDE_TYPE  (0x7fffffff)
#define  REPEAT_TIMES 10

typedef enum 
{
    EM_LIVE_MPEG2_MULTI_LAN1,          /*������AC3MP2*/
	EM_LIVE_MPEG2_AV_SYN,              /*��������ͬ��*/
	EM_LIVE_MPEG2_MULTI_LAN2,	         /*��������������*/
	EM_LIVE_MPEG2_ABILITY_TEST1,       /*ָ����Խ�Ŀ*/
	EM_LIVE_MPEG2_ABILITY_TEST2,      /*PCR���Ϊ0.01S*/
	EM_LIVE_MPEG2_ABILITY_TEST3,      /*PCR���Ϊ0.1S*/
	EM_LIVE_MPEG1_AV,					/*MPEG1��Ƶ���Խ�Ŀ2*/
	EM_LIVE_H264_SD,    				 /*H264�����Ŀ*/
	EM_LIVE_H264_HD_720P,                  /*H264����720P��Ŀ*/
	EM_LIVE_H264_HD_1080i,                  /*H264����1080I��Ŀ*/
	EM_LIVE_MPEG2_PAL,                /*�¼Ӳ���������̨ PAL*/
	EM_LIVE_MPEG2_NTSC,				/*�¼Ӳ���������̨ NTSC*/
	EM_LIVE_MPEG2_AC3,				/*DolbyAC3��׼����*/
	EM_LIVE_MPEG2_TELETEX,			 /*����teltext������*/
	EM_LIVE_AC3PLUS,	       			 /*����AC3PLUS��Ƶ*/
	EM_LIVE_AAC,						/*����AAC��Ƶ*/
	EM_LIVE_AACPLUS,					/*����AACPLUS��Ƶ*/
	EM_LIVE_VC1,						/*����VC1��Ƶ*/
	EM_LIVE_MPEG2_HD,					/*����mpeg2������Ƶ*/
	EM_LIVE_MPEG2_AV_SYN_ERROR_PCR,  /*PCRΪ0*/
	EM_LIVE_COORDINATE_SHIFT,              /*��������ƫ����֤����*/
	EM_LIVE_AUDIO_SOURCE_CHANGE, 	/*������Ƶ���͸ı���֤����*/
	EM_LIVE_MP3,						/*����MP3(MPEG1/2,layer 3)��Ƶ*/
	EM_LIVE_MAX
	
}PlayerTestServiceIndex_E;

/**
* �����������õ��Ľ�Ŀ��ص�������Ϣ
*/
typedef struct 
{	
	const char*          m_pcTsFilename;  	  //�����ļ�����
	const char* 		 m_pcServiceDescript; 	//��������
	int                  m_nVideoPid;           ///< ��������PID��-1��ʾ������
	CSUDIVIDStreamType_E m_eVidStreamType;      ///��Ƶ����
	int                  m_nAudioPid;           ///< ��������PID ��-1��ʾ������
	CSUDIAUDStreamType_E m_eAudStreamType;      ///<��Ƶ����
	int 				 m_nPcrPid;				///< PCR����PID��-1��ʾ������
	int 				 m_nSubPid;				///<SUBTITLE���͵�PID��-1��ʾ������
	int 				 m_nTelPid;				///<TELETEXT���͵�PID��-1��ʾ������
}PlayerTestServiceInfo_S;

#define SERVICE_INVALID  (-1)
#define STREAM_TYPE_INVALID  (0xff)

static PlayerTestServiceInfo_S g_sPlayerSeviceInfo[] ={
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"������AC3MP2",
			60,
			EM_UDI_VID_STREAM_MPEG2,
			62,
			EM_UDI_AUD_STREAM_MPEG2,
			60,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_MULTI_LAN1*/
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"��������ͬ��",
			100,
			EM_UDI_VID_STREAM_MPEG2,
			101,
			EM_UDI_AUD_STREAM_MPEG2,
			100,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_AV_SYN*/
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"��������������",
			1140,
			EM_UDI_VID_STREAM_MPEG2,
			1141,
			EM_UDI_AUD_STREAM_MPEG2,
			SERVICE_INVALID,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_MULTI_LAN2*/
		{
			"Irdeto.PCR.CCITT.ts",
			"ָ����Խ�Ŀ",
			100,
			EM_UDI_VID_STREAM_MPEG2,
			101,
			EM_UDI_AUD_STREAM_MPEG2,
			100,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_ABILITY_TEST1*/
		{
			"Irdeto.PCR.CCITT.ts",
			"PCR���Ϊ0.01S",
			2049,
			EM_UDI_VID_STREAM_MPEG2,
			2050,
			EM_UDI_AUD_STREAM_MPEG2,
			2049,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_ABILITY_TEST2*/
		{
			"Irdeto.PCR.CCITT.ts",
			"PCR���Ϊ0.1S",
			2065,
			EM_UDI_VID_STREAM_MPEG2,
			2066,
			EM_UDI_AUD_STREAM_MPEG2,
			2065,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_ABILITY_TEST3*/
		{
			"H.264.ts",
			"MPEG1��Ƶ���Խ�Ŀ2",
			100,
			EM_UDI_VID_STREAM_MPEG1,
			101,
			EM_UDI_AUD_STREAM_MPEG1,
			100,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG1_AV*/
		{
			"mpeg2hd+h264sd.ts",
			"EM_LIVE_H264_SD",
			0x300,
			EM_UDI_VID_STREAM_H264,
			0x301,
			EM_UDI_AUD_STREAM_MPEG1,
			0x300,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_H264_SD*/
		{
			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
			"H264����720P����Ƶ",
			0x328,
			EM_UDI_VID_STREAM_H264,
			0x329,
			EM_UDI_AUD_STREAM_MPEG1,
			0x328,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_H264_HD_720P*/
		{
			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
			"H264����1080I����Ƶ",
			0x33c,
			EM_UDI_VID_STREAM_H264,
			0x33d,
			EM_UDI_AUD_STREAM_MPEG1,
			0x33c,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_H264_HD_1080i*/	
		{
			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
			"�¼Ӳ���������̨ PAL",
			0x300,
			EM_UDI_VID_STREAM_MPEG2,
			0x301,
			EM_UDI_AUD_STREAM_MPEG2,
			0x200,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_PAL*/	
		{
			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
			"�¼Ӳ���������̨ NTSC",
			0x314,
			EM_UDI_VID_STREAM_MPEG2,
			0x315,
			EM_UDI_AUD_STREAM_MPEG2,
			0x201,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_NTSC*/
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"DolbyAC3��׼����",
			512,
			EM_UDI_VID_STREAM_MPEG2,
			650,
			EM_UDI_AUD_STREAM_AC3,
			128,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_AC3*/
		{
			"Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts",
			"����VBI��ʽ��TELTEXT",
			2695,
			EM_UDI_VID_STREAM_MPEG2,
			2690,
			EM_UDI_AUD_STREAM_MPEG2,
			2695,
			SERVICE_INVALID,
			2694
		}, /*EM_LIVE_MPEG2_TELETEX*/
		{
			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
			"AC3PLUS��Ƶ��Ŀ",
			0x350,
			EM_UDI_VID_STREAM_H264,
			0x351,
			EM_UDI_AUD_STREAM_AC3_PLUS,
			0x350,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_AC3PLUS*/
		{
			"H.264.ts",
			"AAC��Ƶ��Ŀ",
			0x1001,
			EM_UDI_VID_STREAM_H264,
			0x1002,
			EM_UDI_AUD_STREAM_MPEG_AAC,
			0x1001,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_AAC*/
		{
			"EM_LIVE_AACPLUS.ts", //���޴�����Ƶ���͵�����
			"EM_LIVE_AACPLUS",
			0x20,
			EM_UDI_VID_STREAM_H264,
			0x21,
			EM_UDI_AUD_STREAM_MPEG_AAC,
			SERVICE_INVALID,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_AACPLUS*/
		{
			"VC1_Apollo13_1080p.ts",
			"EM_LIVE_VC1",
			0x21,
			EM_UDI_VID_STREAM_VC1,
			SERVICE_INVALID,
			EM_UDI_AUD_STREAM_MPEG2,
			SERVICE_INVALID,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_VC1*/
		{
			"mpeg2hd+h264sd.ts",
			"EM_LIVE_MPEG2_HD",
			0x328,
			EM_UDI_VID_STREAM_MPEG2,
			0x329,
			EM_UDI_AUD_STREAM_MPEG1,
			SERVICE_INVALID,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_MPEG2_HD*/
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"��������ͬ����ĿPCRΪ0������ֵ",
			100,
			EM_UDI_VID_STREAM_MPEG2,
			101,
			EM_UDI_AUD_STREAM_MPEG2,
			SERVICE_INVALID,
			SERVICE_INVALID,
			SERVICE_INVALID
		} ,/*EM_LIVE_MPEG2_AV_SYN_ERROR_PCR*/
		{
			"Irdeto.PCR.CCITT.ts",
			"������Ƶ����ƫ��",
			0x64,
			EM_UDI_VID_STREAM_MPEG2,
			0x65,
			EM_UDI_AUD_STREAM_MPEG2,
			SERVICE_INVALID,
			SERVICE_INVALID,
			SERVICE_INVALID
		},/*EM_LIVE_COORDINATE_SHIFT */
		{
			"Stream_type_switch_2_25.h264_7.trp",
			"������Ƶ���͸ı��¼��ص�",
			0x31,
			EM_UDI_VID_STREAM_H264,
			0x30,
			EM_UDI_AUD_STREAM_AC3_PLUS,
			0x31,
			SERVICE_INVALID,
			SERVICE_INVALID
		},/*EM_LIVE_AUDIO_SOURCE_CHANGE */
		{
			"H.264.ts",
			"MP3��׼����(MPEG1/2,layer 3)",
			0x1801,
			EM_UDI_VID_STREAM_H264,
			0x1802,
			EM_UDI_AUD_STREAM_MP3,
			0x1801,
			SERVICE_INVALID,
			SERVICE_INVALID
		}/*EM_LIVE_MP3 */
};

static CSUDI_BOOL s_abPlayerCBType[EM_UDIPLAYER_MAXEVENTTYPE] = {CSUDI_TRUE, CSUDI_FALSE};
static CSUDI_BOOL s_bPlayerSupportHD = CSUDI_TRUE;
static CSUDI_BOOL s_bPlayerSupportTTxt = CSUDI_TRUE;
static char * s_apcPlayerKey[] = {
	"CALLBACK_SUPPORT_TYPE_VIDEO_FRAME_COMING",
	"CALLBACK_SUPPORT_TYPE_AUDIO_FRAME_COMING",
	"CALLBACK_SUPPORT_TYPE_VIDEO_UNDERFLOW",
	"CALLBACK_SUPPORT_TYPE_AUDIO_UNDERFLOW",
	"CALLBACK_SUPPORT_TYPE_VIDEO_OVERFLOW",
	"CALLBACK_SUPPORT_TYPE_AUDIO_OVERFLOW",
	"CALLBACK_SUPPORT_TYPE_VIDEO_SOURCE_CHANGE",
	"CALLBACK_SUPPORT_TYPE_AUDIO_SOURCE_CHANGE",
	"CALLBACK_SUPPORT_TYPE_END_OF_STREAM",
	"CALLBACK_SUPPORT_TYPE_BEGIN_OF_STREAM",
	"PLAYER_SUPPORT_HD",
	"PLAYER_SUPPORT_STREAM_TELETEXT"
};

/**
@brief ��ȡ�����ļ���Ϣ,��ʼ��player�������

@return ��ȡ�ɹ�����CS_TK_CONFIG_SUCCESS,ʧ�ܷ���CS_TK_CONFIG_FAILURE
*/
int LIVEPLAY_iInitCfg()
{
	char cBuf[32] = {0};
	int nCfgInfoIndex = 0;
	int nResult = 0;

	for(; nCfgInfoIndex < EM_UDIPLAYER_MAXEVENTTYPE; nCfgInfoIndex++)
	{
		//�����Ϣ
		memset(cBuf, '\0', sizeof(cBuf));
		nResult =  CSTKGetConfigInfo("PLAYER", s_apcPlayerKey[nCfgInfoIndex], cBuf, sizeof(cBuf));
		if(CS_TK_CONFIG_SUCCESS != nResult)
		{
			CSTCPrint("[UDI2PLAYERTEST]:not support %s! \n", s_apcPlayerKey[nCfgInfoIndex]);
			continue;
		}

		if (cBuf[0] == '1')
		{
			s_abPlayerCBType[nCfgInfoIndex] = CSUDI_TRUE;
		}
	}

	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("PLAYER", s_apcPlayerKey[nCfgInfoIndex++], cBuf, sizeof(cBuf));
	if (cBuf[0] == '0')
	{
		s_bPlayerSupportHD = CSUDI_FALSE;
	}
	else if (cBuf[0] == '1')
	{
		s_bPlayerSupportHD = CSUDI_TRUE;
	}


	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("PLAYER", s_apcPlayerKey[nCfgInfoIndex++], cBuf, sizeof(cBuf));
	if (cBuf[0] == '0')
	{
		s_bPlayerSupportTTxt = CSUDI_FALSE;
	}
	else if (cBuf[0] == '1')
	{
		s_bPlayerSupportTTxt = CSUDI_TRUE;
	}
	
	return CS_TK_CONFIG_SUCCESS;
}

CSUDI_BOOL CSTC_LIVE_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	//iTest();
	CSUDIAUDIOSetVolume(0, 31);
		CSUDIVIDEOShow(0, CSUDI_TRUE);
		CSUDIVIDEOSetAspectRatio(0,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_AUTO);
		CSUDIVIDEOSetMatchMethod(0,EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_MATCH_METHOD_IGNORE);
	
	return (CS_TK_CONFIG_SUCCESS == LIVEPLAY_iInitCfg());
}

CSUDI_BOOL CSTC_LIVE_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}
static CSUDI_BOOL PLAYER_iSetStream(CSUDI_BOOL bIsSet,CSUDI_HANDLE hPlayer,PlayerTestServiceIndex_E eSerIndex,unsigned int dwStreamType);

static CSUDI_BOOL PLAYER_iLockTuner(PlayerTestServiceIndex_E eSerIndex)
{
	return CSTC_UDI2PortingLock(UDI2_TUNER0_ID,g_sPlayerSeviceInfo[eSerIndex].m_pcTsFilename);
}

//@CASEGROUP:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback������ִ��Ч��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���������������Ҫ��
//@EXPECTATION:��CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback����Ҫ����ο�UDI��ֲָ��
//@EXECUTIONFLOW:1����һ��ֱ��������
//@EXECUTIONFLOW:2��hPlayerΪCSUDI_NULLʱ������CSUDIPLAYERAddPlayerCallback����������CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:3��fnPlayerCallbackΪCSUDI_NULL ʱ������CSUDIPLAYERAddPlayerCallback���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��eEvent ΪEM_UDIPLAYER_MAXEVENTTYPE ʱ������CSUDIPLAYERAddPlayerCallback���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��eEvent ΪEM_UDIPLAYER_MAXEVENTTYPE+100 ʱ������CSUDIPLAYERAddPlayerCallback���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��hPlayerΪCSUDI_NULLʱ������CSUDIPLAYERDelPlayerCallback����������CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:7��fnPlayerCallbackΪCSUDI_NULLʱ������CSUDIPLAYERDelPlayerCallback���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8��eEventΪEM_UDIPLAYER_MAXEVENTTYPEʱ������CSUDIPLAYERDelPlayerCallback���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��eEventΪEM_UDIPLAYER_MAXEVENTTYPE+100ʱ������CSUDIPLAYERDelPlayerCallback���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10��pvUserDataΪCSUDI_NULLʱ������CSUDIPLAYERDelPlayerCallback����������CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
//@EXECUTIONFLOW:11��pvUserDataΪ0x1234ʱ������CSUDIPLAYERDelPlayerCallback����������CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
//@EXECUTIONFLOW:12��pvUserDataΪ�Ϸ�ָ��ʱ������CSUDIPLAYERDelPlayerCallback����������CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
//@EXECUTIONFLOW:13���ظ�����2-����12ָ������
//@EXECUTIONFLOW:14������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0001(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDIPlayerEventType_E  eEvent=EM_UDIPLAYER_VIDEO_FRAME_COMING;
	
	int  nUserData=NUSERDATA;

	int i=0;
	
//��������������
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;
	
//����������
	hPlayer = PLAYER_openPlayer(uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1 ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//testing CSUDIPLAYERAddPlayerCallback 
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERAddPlayerCallback(CSUDI_NULL,fnPlayerCallback,eEvent,&nUserData), "����2 ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERAddPlayerCallback(hPlayer,CSUDI_NULL,eEvent,&nUserData), "����3 ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback,EM_UDIPLAYER_MAXEVENTTYPE,&nUserData), "����4 ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback,EM_UDIPLAYER_MAXEVENTTYPE+100,&nUserData), "����5 ʧ��");
		
		//testing CSUDIPLAYERDelPlayerCallback 
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERDelPlayerCallback (CSUDI_NULL,fnPlayerCallback,eEvent,&nUserData), "����6 ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERDelPlayerCallback(hPlayer,CSUDI_NULL,eEvent,&nUserData), "����7 ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,EM_UDIPLAYER_MAXEVENTTYPE,&nUserData), "����8 ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,EM_UDIPLAYER_MAXEVENTTYPE+100,&nUserData), "����9 ʧ��");
		

		//testing CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST	
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST == CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, CSUDI_NULL), "����10 ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST == CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, (void *)0x1234), "����11 ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST == CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, &nUserData), "����12 ʧ��");
		
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����14 ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback
//@DESCRIPTION:�������в�������Ӧ�ó��������Ͻӿ����Ҫ��CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback������ִ��Ч��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1�����в��������Ͻӿ����Ҫ��
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback����Ҫ����ο�UDI��ֲָ��
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERAddPlayerCallback��ӻص�������������ͬ����(pnUserDataΪCSUDI_NULL,0x1234�Լ�������Ϸ���ֵַ)����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERDelPlayerCallbackɾ������2����ӵ������ص�������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2-����3ָ������
//@EXECUTIONFLOW:5������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDIPlayerEventType_E  eEvent=EM_UDIPLAYER_VIDEO_FRAME_COMING;
	
	int  nUserData=NUSERDATA;

	int i=0;
	
//��������������
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;
	
//����������
	hPlayer = PLAYER_openPlayer(uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1 ʧ��");

	for (i=0;i < REPEAT_TIMES; i++)
	{
		for (eEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING; eEvent<EM_UDIPLAYER_MAXEVENTTYPE;eEvent++)
		{
			if (!s_abPlayerCBType[eEvent])
			{
				continue;
			}
			//testing CSUDIPLAYERAddPlayerCallback
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,&nUserData), "����2-1 ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,CSUDI_NULL), "����2-2 ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,(void *)0x1234), "����2-3 ʧ��");

			//testing CSUDIPLAYERDelPlayerCallback
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, &nUserData), "����3-1 ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, CSUDI_NULL), "����3-2 ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, (void *)0x1234), "����3-3 ʧ��");
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����5 ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback
//@DESCRIPTION:��������CSUDIPLAYERAddPlayerCallback �����ܳɹ����32���ص�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1�����в��������Ͻӿ����Ҫ��
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback����Ҫ����ο�UDI��ֲָ��
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERAddPlayerCallback���32���ص���������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERDelPlayerCallbackɾ������2����ӵ�32���ص���������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2-����3ָ������
//@EXECUTIONFLOW:5������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDIPlayerEventType_E  eEvent=EM_UDIPLAYER_VIDEO_FRAME_COMING;
	
	int i=0, j=0;
	int  anUserData[33] = {0};
	
//��������������
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;
	
//����������
	hPlayer = PLAYER_openPlayer(uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1 ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//���32���ص�	
		for (j=0; j<32; j++)
		{
			anUserData[j] = j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback,eEvent,&anUserData[j]), "�����ʧ��");
		}
		
		//ɾ��32���ص�		
		for (j=0; j<32; j++)
		{
			anUserData[j] = j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback (hPlayer,fnPlayerCallback,eEvent,&anUserData[j]), "����ʮʧ��");
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����5 ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback
//@DESCRIPTION:�������в�������Ӧ�ó��������Ͻӿ����Ҫ��CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback������ִ��Ч��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1�����в��������Ͻӿ����Ҫ��
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback����Ҫ����ο�UDI��ֲָ��
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2��fnPlayerCallbackΪfnPlayerCallback2������CSUDIPLAYERAddPlayerCallback��ӻص�������������ͬ����(pnUserDataΪCSUDI_NULL,0x1234�Լ�������Ϸ���ֵַ)����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��fnPlayerCallbackΪfnPlayerCallback2���ٴε���CSUDIPLAYERAddPlayerCallback����ͼ�ظ����ͬ����ʵ������������CSUDIPLAYER_ERROR_CALLBACK_EXIST
//@EXECUTIONFLOW:4��fnPlayerCallbackΪfnPlayerCallback������CSUDIPLAYERDelPlayerCallback��ͼɾ��û��ע��Ļص�����ʵ������������CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
//@EXECUTIONFLOW:5��fnPlayerCallbackΪfnPlayerCallback2������CSUDIPLAYERDelPlayerCallback��ͼɾ������2����ӵ������ص�������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����2-����5ָ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0004(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDIPlayerEventType_E  eEvent=EM_UDIPLAYER_VIDEO_FRAME_COMING;
	
	int  nUserData=NUSERDATA;

	int i=0;

	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	
//��������������
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;
	
//����������
	hPlayer = PLAYER_openPlayer(uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1 ʧ��");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		for (eEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING; eEvent<EM_UDIPLAYER_MAXEVENTTYPE;eEvent++)
		{
			if (!s_abPlayerCBType[eEvent])
			{
				continue;
			}

			//testing CSUDIPLAYERAddPlayerCallback
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,&nUserData), "����2-1 ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,CSUDI_NULL), "����2-2 ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,(void *)0x1234), "����2-3 ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_EXIST == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,&nUserData), "����3-1 ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_EXIST == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,CSUDI_NULL), "����3-2 ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_EXIST == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,(void *)0x1234), "����3-3 ʧ��");

			//testing CSUDIPLAYERDelPlayerCallback
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, &nUserData), "����4-1 ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, CSUDI_NULL), "����4-2 ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, (void *)0x1234), "����4-3 ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, (void *)0x4321), "����4-4 ʧ��");

			nResult = CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent+1, (void *)0x1234);
			if((EM_UDIPLAYER_MAXEVENTTYPE -1) == eEvent)
			{
				CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_BAD_PARAMETER == nResult), "����4-5-1 ʧ��");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST == nResult), "����4-5-2 ʧ��");
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, &nUserData), "����5-1 ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, CSUDI_NULL), "����5-2 ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, (void *)0x1234), "����5-3 ʧ��");
			
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����6 ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ��CSUDIPLAYEROpen&CSUDIPLAYERClose������ִ��Ч��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������֧��ֱ����Demux, Audio, Video�豸ID
//@EXECUTIONFLOW:2��psPlayerChnlΪCSUDI_NULL������CSUDIPLAYEROpen���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��psPlayerChnl->m_nDemuxΪ-1������CSUDIPLAYEROpen����������CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:4��psPlayerChnl->m_nDemuxΪ-2������CSUDIPLAYEROpen����������CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:5��psPlayerChnl->m_nDemuxΪһ����֧��ֱ����Demux ID������CSUDIPLAYEROpen����������CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:6��psPlayerChnl->m_nVideoDecoder Ϊ-2������CSUDIPLAYEROpen����������CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:7��psPlayerChnl->m_nAudioDecoder Ϊ-2������CSUDIPLAYEROpen����������CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:8��ePlayerType Ϊ��Чֵ(��:EM_UDIPLAYER_MAXPLAYERTYPE)������CSUDIPLAYEROpen���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��phPlayer ΪCSUDI_NULL������CSUDIPLAYEROpen���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10��hPlayer ΪCSUDI_NULL������CSUDIPLAYERClose���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:11���ظ�����2-����10ָ������
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0001(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;

	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int nDemuxID,nAudioID,nVideoID;
	
	CSUDIPlayerChnl_S  sPlayerChnl;

	int i = 0;
	
	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);

	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_MPEG2);
	
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&((-1 != nAudioID)||(-1 != nVideoID))),"����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (CSUDI_NULL,ePlayerType,&hPlayer), "����2ʧ��");

		sPlayerChnl.m_nDemux = -1;
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");

		sPlayerChnl.m_nDemux = -2;
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����4ʧ��");
		
		sPlayerChnl.m_nDemux =	PLAYER_searchDemuxID_N(EM_UDI_DEMUX_PLAY);
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����5ʧ��");
		sPlayerChnl.m_nDemux = nDemuxID;
		
		sPlayerChnl.m_nVideoDecoder = -2;
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����6ʧ��");
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		
		sPlayerChnl.m_nAudioDecoder = -2;
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_MAXPLAYERTYPE,&hPlayer), "����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,CSUDI_NULL), "����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClose (CSUDI_NULL), "����10ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:������������£�CSUDIPLAYEROpen&CSUDIPLAYERClose��ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���ܰ�ֱ����ʽ�����򿪲�����
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������֧��ֱ����Demux, Audio, Video�豸ID
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����ֱ����ʽ�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERClose���رղ���2�򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2-����3ָ������
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0002(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;

	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int nDemuxID,nAudioID,nVideoID;
	
	CSUDIPlayerChnl_S  sPlayerChnl;

	int i = 0;
	
	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);

	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_MPEG2);
	
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&((-1 != nAudioID)||(-1 != nVideoID))),"����1ʧ��");

	sPlayerChnl.m_nAudioDecoder = nAudioID;
	sPlayerChnl.m_nVideoDecoder = nVideoID;
	sPlayerChnl.m_nDemux = nDemuxID;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����3ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:����������������£�������ε���CSUDIPLAYEROpen&CSUDIPLAYERClose�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���ܰ�ֱ����ʽ�����򿪲�����
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������֧��ֱ����Demux, Audio, Video�豸ID
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����ֱ����ʽ�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3���������䣬�ٴε���CSUDIPLAYEROpen����ֱ����ʽ�򿪲��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERClose���رղ���2�򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5���������䣬�ٴε���CSUDIPLAYERClose���رղ���2�򿪵Ĳ��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����2-����5ָ������
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0003(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;

	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int nDemuxID,nAudioID,nVideoID;
	
	CSUDIPlayerChnl_S  sPlayerChnl;

	int i = 0;
	
	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);

	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_MPEG2);
	
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&((-1 != nAudioID)||(-1 != nVideoID))),"����1ʧ��");

	sPlayerChnl.m_nAudioDecoder = nAudioID;
	sPlayerChnl.m_nVideoDecoder = nVideoID;
	sPlayerChnl.m_nDemux = nDemuxID;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClose (hPlayer), "����5ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:������Ƶ������ͨ���Ĵ���رա��ر��ǹ����豸ʱ��CSUDIPLAYEROpen&CSUDIPLAYERClose��ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ����������Ƶ������
//@INPUT:1��
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1���ֱ����֧��ֱ����Demux,  ֧��EM_UDI_AUD_STREAM_MPEG2���͵�Audio, ֧��EM_UDI_VID_STREAM_MPEG2����Video���豸ID
//@EXECUTIONFLOW:2���ֱ���ҵ�2��֧��ֱ����Demux,  ֧��EM_UDI_AUD_STREAM_MPEG2���͵�Audio, ֧��EM_UDI_VID_STREAM_MPEG2����Video���豸ID
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen����ֱ����ʽ�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���벽��3�򿪵Ĳ���������Audio decoder��Demux������CSUDIPLAYEROpen�򿪲���������������CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:5���벽��3�򿪵Ĳ���������Audio decoder������CSUDIPLAYEROpen�򿪲���������������CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:6���벽��3�򿪵Ĳ���������Demux������CSUDIPLAYEROpen�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERClose�ر���һ���򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���벽��3�򿪵Ĳ������������κ��豸������CSUDIPLAYEROpen�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERClose�ر���һ���򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIPLAYERClose�رղ���3���򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:11���ظ�����3-����10ָ������
//@EXECUTIONFLOW:12���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0004(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;

	CSUDI_HANDLE   hPlayer = CSUDI_NULL, hPlayer1= CSUDI_NULL;
	
	int nDemuxID,nAudioID,nVideoID;
	
	int nDemuxID1,nAudioID1,nVideoID1;
	
	CSUDIPlayerChnl_S  sPlayerChnl;

	int i = 0;
	
	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);

	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_MPEG2);
	
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nAudioID)),"����1ʧ��");

	nDemuxID1=PLAYER_searchDemuxID_WithIndex(EM_UDI_DEMUX_PLAY,2);

	nAudioID1=PLAYER_searchAudioID_WithIndex(EM_UDI_AUD_STREAM_MPEG2,2);
	
	nVideoID1=PLAYER_searchVideoID_WithIndex(EM_UDI_VID_STREAM_MPEG2,2);
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = -1;
		sPlayerChnl.m_nDemux = nDemuxID;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");

		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;  //������Audio decoder��Demux
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����4ʧ��");

		if (nDemuxID1 != -1)
		{
			sPlayerChnl.m_nAudioDecoder = nAudioID;
			sPlayerChnl.m_nVideoDecoder = -1;
			sPlayerChnl.m_nDemux = nDemuxID1;   //����Audio decoder
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����5ʧ��");
		}
		
		if (nAudioID1 != -1)
		{
			sPlayerChnl.m_nDemux = nDemuxID;
			sPlayerChnl.m_nAudioDecoder = nAudioID1;
			sPlayerChnl.m_nVideoDecoder = -1;  //����demux 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����6ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "����7ʧ��");
			hPlayer1 = CSUDI_NULL;
		}

		if (nAudioID1 != -1 && nDemuxID1 != -1)
		{
			sPlayerChnl.m_nAudioDecoder = nAudioID1;
			sPlayerChnl.m_nVideoDecoder = -1;
			sPlayerChnl.m_nDemux = nDemuxID1;   //û�й����豸
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����8ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "����9ʧ��");

			hPlayer1 = CSUDI_NULL;
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����10ʧ��");

		hPlayer = CSUDI_NULL;
	
	}
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����12-1ʧ��");		
	}
	
	if (hPlayer1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "����12-2ʧ��");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:������Ƶ������ͨ���Ĵ���رա��ر��ǹ����豸ʱ��CSUDIPLAYEROpen&CSUDIPLAYERClose��ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ����������Ƶ������
//@INPUT:1��
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1���ֱ����֧��ֱ����Demux,  ֧��EM_UDI_AUD_STREAM_MPEG2���͵�Audio, ֧��EM_UDI_VID_STREAM_MPEG2����Video���豸ID
//@EXECUTIONFLOW:2���ֱ���ҵ�2��֧��ֱ����Demux,  ֧��EM_UDI_AUD_STREAM_MPEG2���͵�Audio, ֧��EM_UDI_VID_STREAM_MPEG2����Video���豸ID
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen����ֱ����ʽ�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���벽��3�򿪵Ĳ���������Video  decoder��Demux������CSUDIPLAYEROpen�򿪲���������������CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:5���벽��3�򿪵Ĳ���������Video decoder������CSUDIPLAYEROpen�򿪲���������������CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:6���벽��3�򿪵Ĳ���������Demux������CSUDIPLAYEROpen�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERClose�ر���һ���򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���벽��3�򿪵Ĳ������������κ��豸������CSUDIPLAYEROpen�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERClose�ر���һ���򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIPLAYERClose�رղ���3���򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:11���ظ�����3-����10ָ������
//@EXECUTIONFLOW:12���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0005(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;

	CSUDI_HANDLE   hPlayer = CSUDI_NULL, hPlayer1= CSUDI_NULL;
	
	int nDemuxID,nAudioID,nVideoID;
	
	int nDemuxID1,nAudioID1,nVideoID1;
	
	CSUDIPlayerChnl_S  sPlayerChnl;

	int i = 0;
	
	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);

	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_MPEG2);
	
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nVideoID)),"����1ʧ��");

	nDemuxID1=PLAYER_searchDemuxID_WithIndex(EM_UDI_DEMUX_PLAY,2);

	nAudioID1=PLAYER_searchAudioID_WithIndex(EM_UDI_AUD_STREAM_MPEG2,2);
	
	nVideoID1=PLAYER_searchVideoID_WithIndex(EM_UDI_VID_STREAM_MPEG2,2);
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = -1;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");

		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;  //������Video decoder��Demux
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����4ʧ��");

		if (nDemuxID1 != -1)
		{
			sPlayerChnl.m_nAudioDecoder = -1;
			sPlayerChnl.m_nVideoDecoder = nVideoID;
			sPlayerChnl.m_nDemux = nDemuxID1;   //����Video decoder
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����5ʧ��");
		}
		
		if (nVideoID1 != -1)
		{
			sPlayerChnl.m_nDemux = nDemuxID;
			sPlayerChnl.m_nAudioDecoder = -1;
			sPlayerChnl.m_nVideoDecoder = nVideoID1;  //����demux 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����6ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "����7ʧ��");
			hPlayer1 = CSUDI_NULL;
		}


		if (nVideoID1 != -1 && nDemuxID1 != -1)
		{
			sPlayerChnl.m_nAudioDecoder = -1;
			sPlayerChnl.m_nVideoDecoder = nVideoID1;
			sPlayerChnl.m_nDemux = nDemuxID1;   //û�й����豸
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����8ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "����9ʧ��");

			hPlayer1 = CSUDI_NULL;
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����10ʧ��");

		hPlayer = CSUDI_NULL;
		
	}
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����12-1ʧ��");		
	}
	
	if (hPlayer1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "����12-2ʧ��");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:������Ƶ/��Ƶ������ͨ���Ĵ���رա��ر��ǹ����豸ʱ��CSUDIPLAYEROpen&CSUDIPLAYERClose��ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ����������Ƶ������
//@INPUT:1��
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1���ֱ����֧��ֱ����Demux,  ֧��EM_UDI_AUD_STREAM_MPEG2���͵�Audio, ֧��EM_UDI_VID_STREAM_MPEG2����Video���豸ID
//@EXECUTIONFLOW:2���ֱ���ҵ�2��֧��ֱ����Demux,  ֧��EM_UDI_AUD_STREAM_MPEG2���͵�Audio, ֧��EM_UDI_VID_STREAM_MPEG2����Video���豸ID
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen����ֱ����ʽ�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���벽��3�򿪵Ĳ���������Video  decoder��Demux������CSUDIPLAYEROpen�򿪲���������������CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:5���벽��3�򿪵Ĳ���������Audio  decoder��Demux������CSUDIPLAYEROpen�򿪲���������������CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:6���벽��3�򿪵Ĳ���������Video decoder������CSUDIPLAYEROpen�򿪲���������������CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:7���벽��3�򿪵Ĳ���������Audio decoder������CSUDIPLAYEROpen�򿪲���������������CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:8���벽��3�򿪵Ĳ���������Demux������CSUDIPLAYEROpen�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERClose�ر���һ���򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:10���벽��3�򿪵Ĳ������������κ��豸������CSUDIPLAYEROpen�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:11������CSUDIPLAYERClose�ر���һ���򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose�رղ���3���򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13���ظ�����3-����12ָ������
//@EXECUTIONFLOW:14���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0006(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;

	CSUDI_HANDLE   hPlayer = CSUDI_NULL, hPlayer1= CSUDI_NULL;
	
	int nDemuxID,nAudioID,nVideoID;
	
	int nDemuxID1,nAudioID1,nVideoID1;
	
	CSUDIPlayerChnl_S  sPlayerChnl;

	int i = 0;
	
	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);

	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_MPEG2);
	
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nAudioID)&&(-1 != nVideoID)),"����1ʧ��");

	nDemuxID1=PLAYER_searchDemuxID_WithIndex(EM_UDI_DEMUX_PLAY,2);

	nAudioID1=PLAYER_searchAudioID_WithIndex(EM_UDI_AUD_STREAM_MPEG2,2);
	
	nVideoID1=PLAYER_searchVideoID_WithIndex(EM_UDI_VID_STREAM_MPEG2,2);
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");

		 //������Video decoder��Demux
		sPlayerChnl.m_nDemux = nDemuxID;
		 if(-1 != nAudioID1)
		 {
		 	sPlayerChnl.m_nAudioDecoder = nAudioID1;
		 }
		 else
		 {
		 	sPlayerChnl.m_nAudioDecoder = nAudioID;
		 }
		sPlayerChnl.m_nVideoDecoder = nVideoID; 
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����4ʧ��");

		//������Audio decoder��Demux
		sPlayerChnl.m_nDemux = nDemuxID;
		sPlayerChnl.m_nAudioDecoder = nAudioID; 
		if(-1 != nVideoID1)
		{
			sPlayerChnl.m_nVideoDecoder = nVideoID1; 
		}
		else
		{
			sPlayerChnl.m_nVideoDecoder = nVideoID;
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����5ʧ��");

		if (nDemuxID1 != -1)
		{
			if (nAudioID1 != -1)
			{
				//����Video decoder
				sPlayerChnl.m_nAudioDecoder = nAudioID1;
				sPlayerChnl.m_nVideoDecoder = nVideoID; 
				sPlayerChnl.m_nDemux = nDemuxID1;  
				CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����6ʧ��");
			}

			if (nVideoID1 != -1)
			{
				//����Audio decoder
				sPlayerChnl.m_nAudioDecoder = nAudioID;
				sPlayerChnl.m_nVideoDecoder = nVideoID1;
				sPlayerChnl.m_nDemux = nDemuxID1;   
				CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����7ʧ��");
			}
		}
		
		if ((nVideoID1 != -1)&&(nAudioID1 != -1))
		{
			//����demux 
			sPlayerChnl.m_nDemux = nDemuxID;
			sPlayerChnl.m_nAudioDecoder = nAudioID1;
			sPlayerChnl.m_nVideoDecoder = nVideoID1;  
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����8ʧ��");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "����9ʧ��");
			hPlayer1 = CSUDI_NULL;
		}
		

		if (((nVideoID1 != -1)&&(nAudioID1 != -1)) && (nDemuxID1 != -1))
		{
			//û�й����豸
			sPlayerChnl.m_nAudioDecoder = nAudioID1;
			sPlayerChnl.m_nVideoDecoder = nVideoID1;
			sPlayerChnl.m_nDemux = nDemuxID1;   
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "����10ʧ��");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "����11ʧ��");
			hPlayer1 = CSUDI_NULL;
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����12ʧ��");

		hPlayer = CSUDI_NULL;
		
	}
	
	CSTK_FATAL_POINT;

	//�ָ��ֳ�
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����14-1ʧ��");		
	}
	if (hPlayer1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "����14-2ʧ��");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:�����Ƿ�֧��H264������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���ܰ�ֱ����ʽ�����򿪲�����
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������֧��ֱ����Demux, ֧�� EM_UDI_VID_STREAM_H264 ���͵���Ƶ�����豸nVideoID
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����ֱ����ʽ�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERClose���رղ���2�򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2-����3ָ������
//@EXECUTIONFLOW:5���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0007(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;

	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int nDemuxID,nAudioID,nVideoID;
	
	CSUDIPlayerChnl_S  sPlayerChnl;

	int i = 0;

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("ϵͳ֧��H264����\r\n");
		CSTCPrint("The chip surpports decode of H264format\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��H264����\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	
	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);

	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_MPEG2);
	
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264);

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nVideoID)),"����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = -1;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����2-1ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����3-1ʧ��");

		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����2-2ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����3-2ʧ��");
		hPlayer = CSUDI_NULL;
		
	}
	
	CSTK_FATAL_POINT;
	
	//�ָ��ֳ�
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����5ʧ��");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:�����Ƿ�֧��EM_UDI_AUD_STREAM_AC3������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���ܰ�ֱ����ʽ�����򿪲�����
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������֧��ֱ����Demux, ֧�� EM_UDI_AUD_STREAM_AC3 ���͵���Ƶ�����豸nVideoID
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����ֱ����ʽ�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERClose���رղ���2�򿪵Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2-����3ָ������
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0008(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;

	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int nDemuxID,nAudioID,nVideoID;
	
	CSUDIPlayerChnl_S  sPlayerChnl;

	int i = 0;
	
	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);

	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_AC3);
	
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264);

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nAudioID)),"����1ʧ��");


	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = -1;
		sPlayerChnl.m_nDemux = nDemuxID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����2-1ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����3-1ʧ��");

		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����2-2ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����3-2ʧ��");

		hPlayer = CSUDI_NULL;
				
	}
	
	CSTK_FATAL_POINT;
	
	//�ָ��ֳ�
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����14-1ʧ��");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:���Բ�����Ӧ�ó����ڲ����Ͻӿ����Ҫ�������£�����CSUDIPLAYERSetStream���������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ��������
//@EXECUTIONFLOW:2������hPlayerΪCSUDI_NULL������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������psStreamInfoΪCSUDI_NULL������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������psStreamInfoΪCSUDI_NULL����nStreamCntΪ0������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5������nStreamCntΪ0������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������nStreamCntΪ-1������£�����CSUDIPLAYERSetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������hPlayerΪCSUDI_NULL������£�����CSUDIPLAYERGetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������psStreamInfoΪCSUDI_NULL������£�����CSUDIPLAYERGetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9������pnStreamCntΪCSUDI_NULL������£�����CSUDIPLAYERGetStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����2-����9 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0001(void)
{
	int nStreamCnt = 1;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERSetStream (CSUDI_NULL,asStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,CSUDI_NULL,nStreamCnt,CSUDI_NULL), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,CSUDI_NULL,0,CSUDI_NULL), "����4ʧ��");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,0,CSUDI_NULL), "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,-1,CSUDI_NULL), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetStream (CSUDI_NULL,asStreamInfo,&nStreamCnt), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetStream (hPlayer,CSUDI_NULL,&nStreamCnt), "����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetStream (hPlayer,asStreamInfo,CSUDI_NULL), "����9ʧ��");

	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

typedef struct _Testcase_StreamInfo
{
	int testID;
	CSUDIStreamInfo_S 	m_sStreamInfo;
	char 				*m_pcDesc;
}Testcase_StreamInfo_T;


static Testcase_StreamInfo_T s_SetGetStream_0002_streamInfo[]=
{
	{2,{0,		EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}}, 	"ֱ��״̬�£�PID����Ϊ0"},
	{3,{-1,		EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}}, 	"ֱ��״̬�£�PID����Ϊ-1"},
	{4,{0x1FFF,EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}}, 	"ֱ��״̬�£�PID����Ϊ0x1FFF"},
	{5,{0x100,	EM_UDI_CONTENT_PCR+1,		{EM_UDI_VID_STREAM_MPEG2}}, 	"ֱ��״̬�£�CSUDIContentType_E ���ͷǷ�"},
	{6,{0x100,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAMTYPE_NUM}},	"ֱ��״̬�£���Ƶ���ͷǷ�"},
	{7,{0x100,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_UNKNOWN}},	"ֱ��״̬�£���Ƶ���ͷǷ�"},
	{8,{0x101,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAMTYPE_NUM}},	"ֱ��״̬�£���Ƶ���ͷǷ�"},
	{9,{0x101,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_UNKNOWN}},	"ֱ��״̬�£���Ƶ���ͷǷ�"},	
};

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:��������Ϣ����(CSUDIStreamInfo_S)���������ϲ����Ͻӿ����Ҫ�������£�����CSUDIPLAYERSetStream���������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ��������
//@EXECUTIONFLOW:2��������ֱ��״̬�£�����PIDΪ0�����
//@EXECUTIONFLOW:3��������ֱ��״̬�£�����PIDΪ-1�����
//@EXECUTIONFLOW:4��������ֱ��״̬�£�����PIDΪ0x1FFF�����
//@EXECUTIONFLOW:5��������ֱ��״̬�£�����CSUDIContentType_E ���ͷǷ������
//@EXECUTIONFLOW:6��������ֱ��״̬�£���Ƶ����ΪEM_UDI_VID_STREAMTYPE_NUM�����
//@EXECUTIONFLOW:7��������ֱ��״̬�£���Ƶ����ΪEM_UDI_VID_STREAM_UNKNOWN�����
//@EXECUTIONFLOW:8��������ֱ��״̬�£���Ƶ����ΪEM_UDI_AUD_STREAMTYPE_NUM�����
//@EXECUTIONFLOW:9��������ֱ��״̬�£���Ƶ����ΪEM_UDI_AUD_STREAM_UNKNOWN�����
//@EXECUTIONFLOW:10���ظ�����2-����9  ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0,j=0, count = 0;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	Testcase_StreamInfo_T *pstreaminfo = CSUDI_NULL;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));

	count = sizeof(s_SetGetStream_0002_streamInfo)/sizeof(Testcase_StreamInfo_T);
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		for (j=0;j<count;j++)
		{
			pstreaminfo = &s_SetGetStream_0002_streamInfo[j];				
			asStreamInfo[0] = pstreaminfo->m_sStreamInfo;

			//pls check porting code.if nStreamCnt == 1, it returns true without check psStreamInfo 
			udiRe  = CSUDIPLAYERSetStream (hPlayer,asStreamInfo,1,CSUDI_NULL);

			if (udiRe == CSUDI_SUCCESS)
			{
				CSTCPrint("��������CSTC_LIVE_TEST_IT_SetGetStream_0002 ��%d�� \"%s\" ����ʧ��",pstreaminfo->testID, pstreaminfo->m_pcDesc);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=udiRe, "�Բ��������Ҫ��Ĳ�������CSUDIPLAYERSetStream���������ط�CSUDI_SUCCESS");	
		}
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

static Testcase_StreamInfo_T s_SetGetStream_0003_streamInfo[]=
{
	{1,{0x101,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 CSUDI_NULL},
	{1,{0x102,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 "һ��������ͬʱ������������ͬ����ƵPID������ʧ�ܣ����ط�CSUDI_SUCCESS"},
	{2,{0x101,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 CSUDI_NULL},
	{2,{0x103,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 "һ��������ͬʱ������������ͬ����ƵPID������ʧ�ܣ����ط�CSUDI_SUCCESS"},
	{3,{0x101,	EM_UDI_CONTENT_PCR,		{0}},	 CSUDI_NULL},
	{3,{0x104,	EM_UDI_CONTENT_PCR,		{0}},	 "һ��������ͬʱ������������ͬ��PCR PID������ʧ�ܣ����ط�CSUDI_SUCCESS"},
	{4,{0x102,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 CSUDI_NULL},
	{4,{0x102,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 "һ��������ͬʱ������������ͬ����ƵPID������ʧ�ܣ����ط�CSUDI_SUCCESS"},
	{5,{0x103,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 CSUDI_NULL},
	{5,{0x103,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 "һ��������ͬʱ������������ͬ����ƵPID������ʧ�ܣ����ط�CSUDI_SUCCESS"},
	{6,{0x104,	EM_UDI_CONTENT_PCR,		{0}},	 CSUDI_NULL},
	{6,{0x104,	EM_UDI_CONTENT_PCR,		{0}},	 "һ��������ͬʱ������������ͬ��PCR PID������ʧ�ܣ����ط�CSUDI_SUCCESS"},
	{7,{0x105,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 CSUDI_NULL},
	{7,{0x105,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 "һ��������ͬʱ������������ͬPID������Ϣ������ʧ�ܣ����ط�CSUDI_SUCCESS"},

};

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:����ͬʱ����������Ƶ����Ƶ��PCR����Ϣʱ������CSUDIPLAYERSetStream���������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ��������
//@EXECUTIONFLOW:2��ͬʱ������������ͬ����ƵPID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��ͬʱ������������ͬ����ƵPID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��ͬʱ������������ͬ��PCR PID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͬʱ������������ͬ����ƵPID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��ͬʱ������������ͬ����ƵPID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7��ͬʱ������������ͬ��PCR PID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8��ͬʱ������������ͬ��PID����Ƶ����Ƶ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����2-����8 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0,j=0, count = 0;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	Testcase_StreamInfo_T *pstreaminfo = CSUDI_NULL;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));

	count = sizeof(s_SetGetStream_0003_streamInfo)/(2*sizeof(Testcase_StreamInfo_T));

	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		for (j=0;j<count;j++)
		{
			pstreaminfo = &s_SetGetStream_0003_streamInfo[2*j];
			asStreamInfo[0] = pstreaminfo->m_sStreamInfo;
			
			pstreaminfo = &s_SetGetStream_0003_streamInfo[2*j+1];
			asStreamInfo[1] = pstreaminfo->m_sStreamInfo;
			
			CSTCPrint("2*j=%d\n",2*j);
			
			udiRe  = CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL);
			CSTCPrint("2*j+1=%d\n",2*j+1);
			
			if ((udiRe == CSUDI_SUCCESS) && (pstreaminfo->m_pcDesc != CSUDI_NULL))
			{
				CSTCPrint("��������CSTC_LIVE_TEST_IT_SetGetStream_0003 ��%d�� \"%s\" ����ʧ��",pstreaminfo->testID, pstreaminfo->m_pcDesc);
			}
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=udiRe, "�Բ��������Ҫ��Ĳ�������CSUDIPLAYERSetStream���������ط�CSUDI_SUCCESS");	
		}
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:��һ����Ƶ����������ͨ��CSUDIPLAYERGetStream��ȡ��Ψһ����Ƶ����Ϣ��ͨ��CSUDIPLAYERSetStream������Ƶ����Ϣʱ���������ط�CSUDI_SUCCESS
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ������ƵDVBֱ��������
//@EXECUTIONFLOW:2������CSUDIPLAYERGetStream����ȡ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3���ж���һ����ȡ������Ϣ�в���ֻ��һ����Ƶ����Ϣ
//@EXECUTIONFLOW:4�����ڲ���1�򿪵Ĳ�������������Ƶ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����2 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0004(void)
{
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERGetStream (hPlayer,asStreamInfo,&nStreamCnt), "����2ʧ��");

	CSTCPrint("asStreamInfo[0].m_eContentType=%d\n",asStreamInfo[0].m_eContentType);
	CSTCPrint("nStreamCnt=%d\n",nStreamCnt);
	
	CSTK_ASSERT_TRUE_FATAL((nStreamCnt <= 1),"����3ʧ��");

	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_nPid  = 0x101;
	asStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 0x102;
	asStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;		

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "����4-1ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[1],1,CSUDI_NULL), "����4-2ʧ��");
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:��һ����Ƶ����������ͨ��CSUDIPLAYERGetStream��ȡ��Ψһ����Ƶ����Ϣ��ͨ��CSUDIPLAYERSetStream������Ƶ����Ϣʱ���������ط�CSUDI_SUCCESS
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ������ƵDVBֱ��������
//@EXECUTIONFLOW:2������CSUDIPLAYERGetStream����ȡ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3���ж���һ����ȡ������Ϣ�в���ֻ��һ����Ƶ����Ϣ
//@EXECUTIONFLOW:4�����ڲ���1�򿪵Ĳ�������������Ƶ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����2 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0005(void)
{
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERGetStream (hPlayer,asStreamInfo,&nStreamCnt), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL((nStreamCnt <= 1),"����3ʧ��");

	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_nPid  = 0x101;
	asStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 0x102;
	asStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;		

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "����4-1ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,1,CSUDI_NULL), "����4-2ʧ��");
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION::���ò�������֧�ֵ���Ƶ������ʱ���������ط�CSUDI_SUCCESS
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ����֧��ĳ����Ƶ��������DVBֱ��������
//@EXECUTIONFLOW:2�����ڲ���1�򿪵Ĳ�������������Ƶ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3���ظ�����2 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0006(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
	CSUDIPlayerChnl_S  psPlayerChnl;
	int nAudioID, nVideoID, nDemuxID = -1;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_Y(eAudStreamType);
	nVideoID=PLAYER_searchVideoID_N((unsigned int *)&eVidStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nDemuxID, "û��֧��ֱ����demux");
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioID, "û��֧��MPEG2���͵���Ƶ������");
	if(nVideoID == -1)
	{
		CSTCPrint("�Ҳ�����֧�ֵ�����,ֱ�Ӳ��Գɹ�\n");
		return CSUDI_TRUE;
	}

//��������������
	psPlayerChnl.m_nDemux=nDemuxID;
	psPlayerChnl.m_nAudioDecoder=nAudioID;
	psPlayerChnl.m_nVideoDecoder=nVideoID;

//���ý�Ŀ��Ϣ����
	asStreamInfo[0].m_nPid=128;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=eVidStreamType;

	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 129;
	asStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;		

	CSTCPrint("use video index:%d type:%d\n",nVideoID,eVidStreamType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYEROpen (&psPlayerChnl,ePlayerType,&hPlayer), "����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "����2-1ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,1,CSUDI_NULL), "����2-2ʧ��");

	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION::���ò�������֧�ֵ���Ƶ�����ͣ��������ط�CSUDI_SUCCESS
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ����֧��ĳ����Ƶ��������DVBֱ��������
//@EXECUTIONFLOW:2�����ڲ���1�򿪵Ĳ�������������Ƶ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3���ظ�����2 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0007(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
	CSUDIPlayerChnl_S  psPlayerChnl;
	int nAudioID, nVideoID, nDemuxID = -1;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_N((unsigned int *)&eAudStreamType);
	nVideoID=PLAYER_searchVideoID_Y(eVidStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nDemuxID, "û��֧��ֱ����demux");
	CSTK_ASSERT_TRUE_FATAL(-1 != nVideoID, "û��֧��MPEG2���͵���Ƶ������");
	if(nAudioID == -1)
	{
		CSTCPrint("�Ҳ�����֧�ֵ�����,ֱ�Ӳ��Գɹ�\n");
		return CSUDI_TRUE;
	}

//��������������
	psPlayerChnl.m_nDemux=nDemuxID;
	psPlayerChnl.m_nAudioDecoder=nAudioID;
	psPlayerChnl.m_nVideoDecoder=nVideoID;

//���ý�Ŀ��Ϣ����
	asStreamInfo[0].m_nPid=128;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 129;
	asStreamInfo[1].m_uStreamType.m_eVideoType = eAudStreamType;		

	CSTCPrint("use video index:%d type:%d\n",nVideoID,eVidStreamType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYEROpen (&psPlayerChnl,ePlayerType,&hPlayer), "����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],2,CSUDI_NULL), "����2-1ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[1],1,CSUDI_NULL), "����2-2ʧ��");

	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

static CSUDI_BOOL verify_streaminfo(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S asStreamInfo[],int nStreamCnt)
{
	int nCnt = 10;
	int i = 0,j =0;
	
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_BOOL bRe = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERGetStream (hPlayer,sStreamInfo,&nCnt), "��ȡ����Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL((nStreamCnt == nCnt), "����Ϣ��������");

	for (i=0;i<nCnt;i++)
	{
		bRe = CSUDI_FALSE;
		
		for (j=0;j<nCnt;j++)
		{
			if (sStreamInfo[j].m_eContentType == asStreamInfo[i].m_eContentType)
			{
				if (!memcmp(&sStreamInfo[j] ,&asStreamInfo[i],sizeof(CSUDIStreamInfo_S)))
				{
					bRe = CSUDI_TRUE;
				}
			}
		}
		if (!bRe)
		{
			break;
		}
	}

	CSTK_FATAL_POINT;
	
	return bRe;
	
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:�����ڴ���Ƶ/��Ƶ������start/stop֮�����CSUDIPLAYERSetStream&CSUDIPLAYERGetStream�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@INPUT:2��psStreamInfo->m_nPid= 352,
//@INPUT:	  psStreamInfo->m_eContentTpye=EM_UDI_CONTENT_VIDEO  ,
//@INPUT:	  psStreamInfo->m_uStreamType->m_eVideoType=EM_UDI_VID_STREAM_MPEG2 
//@INPUT:3��nStreamCnt=1
//@INPUT:4��psPlaybackParam=CSUDI_NULL
//@EXPECTATION:start֮��stop֮ǰ���ط� CSUDI_SUCCESS��stop֮��start֮ǰ���ط� CSUDI_SUCCESS,Open֮��Close֮ǰ�����Ե���CSUDIPLAYERGetStream
//@REMARK: SetStreamֻ�����ڲ������ձ�������ʱ���벥������ֹ֮ͣ��
//@EXECUTIONFLOW:1����һ������Ƶ��DVBֱ��������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream��������Ƶ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��ͨ��CSUDIPLAYERGetStream���жϻ�ȡ������Ϣ��������õ�����Ϣ�Ƿ�һ��
//@EXECUTIONFLOW:4������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERSetStream��������Ƶ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��ͨ��CSUDIPLAYERGetStream���жϻ�ȡ������Ϣ��������õ�����Ϣ�Ƿ�һ��
//@EXECUTIONFLOW:7������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8��ͨ��CSUDIPLAYERGetStream���жϻ�ȡ������Ϣ��������õ�����Ϣ�Ƿ�һ��
//@EXECUTIONFLOW:9���ظ�����2-����8 ָ������
//@EXECUTIONFLOW:10���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0008(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_nPid  = 352;
	asStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERSetStream (hPlayer,asStreamInfo,1,CSUDI_NULL), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,1), "����3ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStart (hPlayer), "����4ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,1,CSUDI_NULL), "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,1), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStop(hPlayer), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,1), "����8ʧ��");
		
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:�����ڲ�����start/stop֮�����CSUDIPLAYERSetStream&CSUDIPLAYERGetStream�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@INPUT:2��psStreamInfo������Ƶ����Ϣ
//@INPUT:3��nStreamCnt=2
//@INPUT:4��psPlaybackParam=CSUDI_NULL
//@EXPECTATION:start֮��stop֮ǰ���ط� CSUDI_SUCCESS��stop֮��start֮ǰ���ط� CSUDI_SUCCESS,Open֮��Close֮ǰ�����Ե���CSUDIPLAYERGetStream
//@REMARK: SetStreamֻ�����ڲ������ձ�������ʱ���벥������ֹ֮ͣ��
//@EXECUTIONFLOW:1����һ����������Ƶ��DVBֱ��������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream��������Ƶ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��ͨ��CSUDIPLAYERGetStream���жϻ�ȡ������Ϣ��������õ�����Ϣ�Ƿ�һ��
//@EXECUTIONFLOW:4������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERSetStream��������Ƶ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��ͨ��CSUDIPLAYERGetStream���жϻ�ȡ������Ϣ��������õ�����Ϣ�Ƿ�һ��
//@EXECUTIONFLOW:7������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8��ͨ��CSUDIPLAYERGetStream���жϻ�ȡ������Ϣ��������õ�����Ϣ�Ƿ�һ��
//@EXECUTIONFLOW:9���ظ�����2-����8 ָ������
//@EXECUTIONFLOW:10���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0009(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_nPid  = 352;
	asStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 353;
	asStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,2), "����3ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStart (hPlayer), "����4ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,2), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStop(hPlayer), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,2), "����8ʧ��");
		
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPlaybackParam    
//@DESCRIPTION:����ֱ��ģʽ�£�CSUDIPLAYERGetPlaybackParam������ִ�н��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��
//@INPUT:2��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ�����������õ����������
//@EXECUTIONFLOW:2��hPlayerΪCSUDI_NULL������CSUDIPLAYERGetPlaybackParam���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��psPlaybackParamΪCSUDI_NULL������CSUDIPLAYERGetPlaybackParam���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��hPlayerΪ����1�õ��ľ����psStreamInfoΪ��Ч��ַ������CSUDIPLAYERGetPlaybackParam,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����2-����4ָ������
//@EXECUTIONFLOW:6������CSUDIPLAYERClose �رղ�����
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPlaybackParam_0001(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIPlaybackParam_S  sPlaybackParam;
	int i=0;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;
	
//����������
	hPlayer= PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetPlaybackParam (CSUDI_NULL,&sPlaybackParam), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetPlaybackParam (hPlayer,CSUDI_NULL), "����3ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetPlaybackParam (hPlayer,&sPlaybackParam), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����6ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream   
//@DESCRIPTION:�����ڲ����Ƿ�������£��ӿں���CSUDIPLAYERModifyStream��ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:�����Ƿ�������£����ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ��������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart��������ֱ������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��hPlayerΪCSUDI_NULL������£�����CSUDIPLAYERModifyStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��psStreamInfoΪCSUDI_NULL������£�����CSUDIPLAYERModifyStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��nStreamCntΪ0������£�����CSUDIPLAYERModifyStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7��nStreamCntΪ-1������£�����CSUDIPLAYERModifyStream��������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8���ظ�����4-����7 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0001(void)
{
  	CSUDIStreamInfo_S  sStreamInfo[10];
	int nStreamCnt;
	
	CSUDI_HANDLE   hPlayer= CSUDI_NULL;
	
	int i=0;
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;

//���ý�Ŀ��Ϣ
	sStreamInfo[0].m_nPid=352;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	sStreamInfo[1].m_nPid=353;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;	

	nStreamCnt=2;
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo[0],nStreamCnt,CSUDI_NULL), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart (hPlayer), "����3ʧ��");
	
	sStreamInfo[0].m_nPid=354;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	sStreamInfo[1].m_nPid=356;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;	
	
	for (i=0;i<REPEAT_TIMES;i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (CSUDI_NULL,&sStreamInfo[0],nStreamCnt), "����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,CSUDI_NULL,2), "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,&sStreamInfo[0],0), "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,&sStreamInfo[0],-1), "����7ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if (CSUDI_NULL  != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream
//@DESCRIPTION:��������Ϣ����(CSUDIStreamInfo_S)���������ϲ����Ͻӿ����Ҫ�������£�����CSUDIPLAYERModifyStream���������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ��������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream��������Ϣ
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������
//@EXECUTIONFLOW:4��������ֱ��״̬�£�����CSUDIPLAYERModifyStream�޸�PIDΪ0�����
//@EXECUTIONFLOW:5��������ֱ��״̬�£�����CSUDIPLAYERModifyStream�޸�PIDΪ-1�����
//@EXECUTIONFLOW:6��������ֱ��״̬�£�����CSUDIPLAYERModifyStream�޸�PIDΪ0x1FFF�����
//@EXECUTIONFLOW:7��������ֱ��״̬�£�����CSUDIPLAYERModifyStream�޸�CSUDIContentType_E ���ͷǷ������
//@EXECUTIONFLOW:8��������ֱ��״̬�£�����CSUDIPLAYERModifyStream�޸���Ƶ����ΪEM_UDI_VID_STREAMTYPE_NUM�����
//@EXECUTIONFLOW:9��������ֱ��״̬�£�����CSUDIPLAYERModifyStream�޸���Ƶ����ΪEM_UDI_VID_STREAM_UNKNOWN�����
//@EXECUTIONFLOW:10��������ֱ��״̬�£�����CSUDIPLAYERModifyStream�޸���Ƶ����ΪEM_UDI_AUD_STREAMTYPE_NUM�����
//@EXECUTIONFLOW:11��������ֱ��״̬�£�����CSUDIPLAYERModifyStream�޸���Ƶ����ΪEM_UDI_AUD_STREAM_UNKNOWN�����
//@EXECUTIONFLOW:12���ظ�����4-����11  ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0002(void)
{
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0,j=0, count = 0;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	Testcase_StreamInfo_T *pstreaminfo = CSUDI_NULL;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;
	
	memset(asStreamInfo,0,sizeof(asStreamInfo));

	asStreamInfo[0].m_nPid=0x100;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_nPid=0x101;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;	

	asStreamInfo[2].m_nPid=0x100;
	asStreamInfo[2].m_eContentType=EM_UDI_CONTENT_PCR;

	nStreamCnt=3;

	count = sizeof(s_SetGetStream_0002_streamInfo)/sizeof(Testcase_StreamInfo_T);
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],nStreamCnt,CSUDI_NULL), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hPlayer), "����3ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		for (j=0;j<count;j++)
		{
			pstreaminfo = &s_SetGetStream_0002_streamInfo[j];				
			asStreamInfo[0] = pstreaminfo->m_sStreamInfo;
			
			udiRe  = CSUDIPLAYERModifyStream (hPlayer,asStreamInfo,1);

			if (udiRe == CSUDI_SUCCESS)
			{
				CSTCPrint("��������%s ��%d�� \"%s\" ����ʧ��",__FUNCTION__,pstreaminfo->testID+3, pstreaminfo->m_pcDesc);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=udiRe, "�Բ��������Ҫ��Ĳ�������CSUDIPLAYERModifyStream���������ط�CSUDI_SUCCESS");	
		}
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop( hPlayer), "ֹͣ������ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�رղ�����ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream
//@DESCRIPTION:����ͬʱ����������Ƶ����Ƶ��PCR����Ϣʱ������CSUDIPLAYERModifyStream���������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��������ϲ��������Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ��������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream��������Ϣ
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������
//@EXECUTIONFLOW:4��ͬʱ������������ͬ����ƵPID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͬʱ������������ͬ����ƵPID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��ͬʱ������������ͬ��PCR PID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7��ͬʱ������������ͬ����ƵPID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8��ͬʱ������������ͬ����ƵPID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��ͬʱ������������ͬ��PCR PID���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10��ͬʱ������������ͬ��PID����Ƶ����Ƶ����Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:11���ظ�����4-����10 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0003(void)
{
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0,j=0, count = 0;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	Testcase_StreamInfo_T *pstreaminfo = CSUDI_NULL;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));

	asStreamInfo[0].m_nPid=0x100;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_nPid=0x101;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;	

	asStreamInfo[2].m_nPid=0x100;
	asStreamInfo[2].m_eContentType=EM_UDI_CONTENT_PCR;

	nStreamCnt=3;

	count = sizeof(s_SetGetStream_0003_streamInfo)/(2*sizeof(Testcase_StreamInfo_T));
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],nStreamCnt,CSUDI_NULL), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hPlayer), "����3ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		for (j=0;j<count;j++)
		{
			pstreaminfo = &s_SetGetStream_0003_streamInfo[2*j];
			asStreamInfo[0] = pstreaminfo->m_sStreamInfo;
			
			pstreaminfo = &s_SetGetStream_0003_streamInfo[2*j+1];
			asStreamInfo[1] = pstreaminfo->m_sStreamInfo;
			
			udiRe  = CSUDIPLAYERModifyStream (hPlayer,asStreamInfo,2);
			
			if ((udiRe == CSUDI_SUCCESS) && (pstreaminfo->m_pcDesc != CSUDI_NULL))
			{
				CSTCPrint("��������%s ��%d�� \"%s\" ����ʧ��",__FUNCTION__,pstreaminfo->testID+3, pstreaminfo->m_pcDesc);
			}
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=udiRe, "�Բ��������Ҫ��Ĳ�������CSUDIPLAYERModifyStream���������ط�CSUDI_SUCCESS");	
		}
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop( hPlayer), "ֹͣ������ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�رղ�����ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream   
//@DESCRIPTION:�����ڲ����Ϸ���Ӧ�ó������������Ҫ�������£��ӿں���CSUDIPLAYERModifyStream��ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:�����Ƿ�������£����ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ��������
//@EXECUTIONFLOW:2������������֮ǰ������CSUDIPLAYERModifyStream��������Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������������֮ǰ������CSUDIPLAYERSetStream��������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4����������Ϣ֮������������֮ǰ������CSUDIPLAYERModifyStream��������Ϣ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERStart��������ֱ������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������������֮�󣬵���CSUDIPLAYERModifyStream��������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERStop��ֹͣ��ֱ������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���ظ�����2-����7 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0004(void)
{
  	CSUDIStreamInfo_S  sStreamInfo[10];
	int nStreamCnt;
	
	CSUDI_HANDLE   hPlayer= CSUDI_NULL;
	
	int i=0;
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;

//���ý�Ŀ��Ϣ
	sStreamInfo[0].m_nPid=352;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	sStreamInfo[1].m_nPid=353;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;	

	nStreamCnt=2;
	
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,sStreamInfo,nStreamCnt), "����2ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo[0],nStreamCnt,CSUDI_NULL), "����3ʧ��");
		
		sStreamInfo[0].m_nPid=354;
		sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
		sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;		
		sStreamInfo[1].m_nPid=356;
		sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
		sStreamInfo[1].m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,sStreamInfo,nStreamCnt), "����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart (hPlayer), "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERModifyStream (hPlayer,sStreamInfo,nStreamCnt), "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����7ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if (CSUDI_NULL  != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream
//@DESCRIPTION:ͨ��CSUDIPLAYERModifyStream�޸Ĳ���������֧�ֵ���Ƶ�����ͣ��������ط�CSUDI_SUCCESS
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ��������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������Ƶ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart��������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERModifyStream,�޸���Ƶ����Ϣ����������֧�ֵ������ͣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERStopֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����2-����5 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0005(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
	CSUDIPlayerChnl_S  psPlayerChnl;
	int nAudioID, nVideoID, nDemuxID = -1;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_N((unsigned int *)(&eAudStreamType));
	nVideoID=PLAYER_searchVideoID_Y(eVidStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nDemuxID, "û��֧��ֱ����demux");
	CSTK_ASSERT_TRUE_FATAL(-1 != nVideoID, "û��֧��MPEG2���͵���Ƶ������");
	if(nAudioID == -1)
	{
		CSTCPrint("�Ҳ�����֧�ֵ�����,ֱ�Ӳ��Գɹ�\n");
		return CSUDI_TRUE;
	}

//��������������
	psPlayerChnl.m_nDemux=nDemuxID;
	psPlayerChnl.m_nAudioDecoder=nAudioID;
	psPlayerChnl.m_nVideoDecoder=nVideoID;

//���ý�Ŀ��Ϣ����
	asStreamInfo[0].m_nPid=128;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 129;
	asStreamInfo[1].m_uStreamType.m_eVideoType = eAudStreamType;		

	CSTCPrint("use video index:%d type:%d\n",nVideoID,eVidStreamType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYEROpen (&psPlayerChnl,ePlayerType,&hPlayer), "����1ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],1,CSUDI_NULL), "����2ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStart( hPlayer), "����3ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERModifyStream (hPlayer,&asStreamInfo[0],2), "����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop( hPlayer), "����5ʧ��");		
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�رղ�����ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream
//@DESCRIPTION:ͨ��CSUDIPLAYERModifyStream�޸Ĳ���������֧�ֵ���Ƶ�����ͣ��������ط�CSUDI_SUCCESS
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��DVBֱ��������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������Ƶ����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart��������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERModifyStream,�޸���Ƶ����Ϣ����������֧�ֵ������ͣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERStopֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����2-����5 ָ������
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0006(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
	CSUDIPlayerChnl_S  psPlayerChnl;
	int nAudioID, nVideoID, nDemuxID = -1;
	
//���ò�������Ϣ	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //���Ҿ��в��Ź��ܵ�demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_Y(eAudStreamType);
	nVideoID=PLAYER_searchVideoID_N((unsigned int *)(&eVidStreamType));
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nDemuxID, "û��֧��ֱ����demux");
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioID, "û��֧��MPEG2���͵���Ƶ������");
	if(nVideoID == -1)
	{
		CSTCPrint("�Ҳ�����֧�ֵ�����,ֱ�Ӳ��Գɹ�\n");
		return CSUDI_TRUE;
	}

//��������������
	psPlayerChnl.m_nDemux=nDemuxID;
	psPlayerChnl.m_nAudioDecoder=nAudioID;
	psPlayerChnl.m_nVideoDecoder=nVideoID;

//���ý�Ŀ��Ϣ����

	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_nPid  = 129;
	asStreamInfo[0].m_uStreamType.m_eVideoType = eAudStreamType;		
	
	asStreamInfo[1].m_nPid=128;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType=eVidStreamType;

	CSTCPrint("use video index:%d type:%d\n",nVideoID,eVidStreamType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYEROpen (&psPlayerChnl,ePlayerType,&hPlayer), "����1ʧ��");


	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],1,CSUDI_NULL), "����2ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStart( hPlayer), "����3ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERModifyStream (hPlayer,&asStreamInfo[0],2), "����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop( hPlayer), "����5ʧ��");		
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�رղ�����ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   & CSUDIPLAYERStop
//@DESCRIPTION:���Բ�����ָ��Ƿ������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer=CSUDI_NULL
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYERStart��ʹhPlayer=CSUDI_NULL����������CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:2������CSUDIPLAYERStop��ʹhPlayer=CSUDI_NULL����������CSUDIPLAYER_ERROR_INVALID_HANDLE
CSUDI_BOOL CSTC_LIVE_TEST_IT_StartStop_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERStart(CSUDI_NULL), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERStop(CSUDI_NULL), "����2ʧ��");
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   & CSUDIPLAYERStop
//@DESCRIPTION:������������/ֹͣһ�������������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream,������������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����3-����4ָ������
//@EXECUTIONFLOW:6���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_StartStop_0002(void)
{
	CSUDIStreamInfo_S  asStreamInfo[10];
	int nStreamCnt,i;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

//���ý�Ŀ��Ϣ
	memset(asStreamInfo,0,sizeof(asStreamInfo));

	asStreamInfo[0].m_nPid=0x100;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_nPid=0x101;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;	

	asStreamInfo[2].m_nPid=0x100;
	asStreamInfo[2].m_eContentType=EM_UDI_CONTENT_PCR;

	nStreamCnt=3;

//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   
//@DESCRIPTION:��Ӧ�ó������������Ҫ�������£�����CSUDIPLAYERStart/ CSUDIPLAYERStop,�������ط�CSUDI_SUCCESS
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_STATUS 
//@REMARK:CSUDIPLAYERStartֻ������������֮����ֹͣ������֮����ã�CSUDIPLAYERStopֻ��������������֮��ֹͣ������֮ǰ���ܵ���
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2��CSUDIPLAYEROpen֮����������CSUDIPLAYERStop��ֹͣ���������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��CSUDIPLAYEROpen֮����������CSUDIPLAYERStart���������������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��CSUDIPLAYEROpen֮�����CSUDIPLAYERSetStream,������������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��CSUDIPLAYERSetStream֮��CSUDIPLAYERStart֮ǰ������CSUDIPLAYERStop��ֹͣ���������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���ٴε���CSUDIPLAYERStart���������������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ٴε���CSUDIPLAYERStop��ֹͣ���������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����5-����9ָ������
//@EXECUTIONFLOW:11���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_StartStop_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int nStreamCnt = 0, i=0;
	
	CSUDIStreamInfo_S  asStreamInfo[10];
//��������������
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;
	
	memset(asStreamInfo,0,sizeof(asStreamInfo));

	asStreamInfo[0].m_nPid=0x100;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_nPid=0x101;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;	

	asStreamInfo[2].m_nPid=0x100;
	asStreamInfo[2].m_eContentType=EM_UDI_CONTENT_PCR;

	nStreamCnt=3;
//����������
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],nStreamCnt,CSUDI_NULL), "����4ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����8ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "����9ʧ��");
		
	}
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:���Բ��������ΪCSUDI_NULLʱ��CSUDIPLAYERPause & CSUDIPLAYERResume�ĵ������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer=CSUDI_NULL
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_HANDLE 
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIPLAYERPause��ʹhPlayer=CSUDI_NULL����������CSUDIPLAYER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:2������CSUDIPLAYERResume��ʹhPlayer=CSUDI_NULL����������CSUDIPLAYER_ERROR_INVALID_HANDLE 
CSUDI_BOOL CSTC_LIVE_TEST_IT_PauseResume_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERPause(CSUDI_NULL), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERResume(CSUDI_NULL), "����2ʧ��");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:����Ӧ�ó������������Ҫ��ʱ��CSUDIPLAYERPause & CSUDIPLAYERResume������ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:Ӧ�ó������������Ҫ��ʱӦ�÷��ط�CSUDI_SUCCESS
//@REMARK:ֱ��������Ҫ��֧��CSUDIPLAYERPause & CSUDIPLAYERResume
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2��Open����������������CSUDIPLAYERPause���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��Open����������������CSUDIPLAYERResume���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��Open�����������CSUDIPLAYERSetStream������������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��Start����֮ǰ������CSUDIPLAYERPause���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��Start����֮ǰ������CSUDIPLAYERResume���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERStart��Start����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8��Start֮��Pause����֮ǰ������CSUDIPLAYERResume���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERPause��ͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIPLAYERStart���������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:11������CSUDIPLAYERSetStream���������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:12������CSUDIPLAYERModifyStream���������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:13���ٴε���CSUDIPLAYERPause���������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:14������CSUDIPLAYERResume����������CSUDI_SUCCESS 
//@EXECUTIONFLOW:15���ٴε���CSUDIPLAYERResume���������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:16������CSUDIPLAYERPause����������CSUDI_SUCCESS 
//@EXECUTIONFLOW:17������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18���ظ�����5-����17ָ������
//@EXECUTIONFLOW:19���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_PauseResume_0002(void)
{
	CSUDIStreamInfo_S  asStreamInfo[10];
	
	int nStreamCnt,i;
	
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

//��������������

	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;
//���ý�Ŀ��Ϣ
	memset(asStreamInfo,0,sizeof(asStreamInfo));

	asStreamInfo[0].m_nPid=0x100;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_nPid=0x101;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;	

	asStreamInfo[2].m_nPid=0x100;
	asStreamInfo[2].m_eContentType=EM_UDI_CONTENT_PCR;

	nStreamCnt=3;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause (hPlayer), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "����4ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause (hPlayer), "����5ʧ��");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����7ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "����8ʧ��");

		udiRe = CSUDIPLAYERPause(hPlayer);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == udiRe),"����9ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERStart(hPlayer), "����10ʧ��");
					
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERModifyStream (hPlayer,asStreamInfo,nStreamCnt), "����12ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause(hPlayer), "����13ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����14ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "����15ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����16ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����17ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetSpeed       
//@DESCRIPTION:����ֱ������£�CSUDIPLAYERSetSpeed������ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��hPlayerΪCSUDI_NULL������ CSUDIPLAYERSetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��eSpeedΪ��Чֵ������ CSUDIPLAYERSetSpeed���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��������ȷ������£�Start������֮ǰ����CSUDIPLAYERSetSpeed����������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:6������CSUDIPLAYERStart��Start����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7��������ȷ������£�Start֮�����CSUDIPLAYERSetSpeed����������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����3-����8ָ������
//@EXECUTIONFLOW:10���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetSpeed_0001(void)
{
	CSUDIPlayerSpeed_E  eSpeed=EM_UDIPLAYER_SPEED_NORMAL;
	CSUDIStreamInfo_S  sStreamInfo;
	int nStreamCnt, i;
	
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code errorCode = CSUDI_SUCCESS;
	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE ;

//���ý�Ŀ��Ϣ
	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream(hPlayer,&sStreamInfo,1,CSUDI_NULL), "����2ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERSetSpeed(CSUDI_NULL,eSpeed), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSetSpeed(CSUDI_NULL,EM_UDIPLAYER_SPEED_FASTFORWARD_32+10), "����4ʧ��");

		for (eSpeed = EM_UDIPLAYER_SPEED_NORMAL;eSpeed <=EM_UDIPLAYER_SPEED_FASTFORWARD_32;eSpeed++)
		{
			errorCode = CSUDIPLAYERSetSpeed(hPlayer,eSpeed);

			//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
			CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "����5ʧ��");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����6ʧ��");

		for (eSpeed = EM_UDIPLAYER_SPEED_NORMAL;eSpeed <=EM_UDIPLAYER_SPEED_FASTFORWARD_32;eSpeed++)
		{
			errorCode = CSUDIPLAYERSetSpeed(hPlayer, eSpeed);

			//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
			CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "����7ʧ��");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����8ʧ��");

	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSeek        
//@DESCRIPTION:����ֱ������£��ӿں���CSUDIPLAYERSeek��ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1����Ч��hPlayer  
//@INPUT:2����Ч��nPosInSec
//@INPUT:3��ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_CURRENT 
//@EXPECTATION:ֱ��ģʽ�²�֧��  CSUDIPLAYERSeek
//@REMARK:
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��hPlayerΪCSUDI_NULL������ CSUDIPLAYERSeek���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������Ϸ��������� CSUDIPLAYERSeek���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����3-����4ָ������
//@EXECUTIONFLOW:6���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_Seek_0001(void)
{
	CSUDIPlayPosition_E   ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_CURRENT;
	
	CSUDIStreamInfo_S  sStreamInfo;
	int nStreamCnt, i;
 
	CSUDIPLAYERType_E  ePlayerType  =EM_UDIPLAYER_LIVE;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;

//���ý�Ŀ��Ϣ
	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSeek(CSUDI_NULL,1,ePlayPosFlag), "����3ʧ��");
		
		for (ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_HEAD;ePlayPosFlag<=EM_UDIPLAYER_POSITION_FROM_END;ePlayPosFlag++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSeek(hPlayer,0,ePlayPosFlag), "����4ʧ��");
		}
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPosInSec           
//@DESCRIPTION:����ֱ������£��ӿں���CSUDIPLAYERGetCurPosInSec��ִ�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:ֱ��״̬�²�֧��  CSUDIPLAYERGetCurPosInSec
//@REMARK:
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3��hPlayerΪCSUDI_NULL������ CSUDIPLAYERGetCurPosInSec���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��pnPosInSecΪCSUDI_NULL������ CSUDIPLAYERGetCurPosInSec���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���������Ϸ��������� CSUDIPLAYERGetCurPosInSec���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����3-����5ָ������
//@EXECUTIONFLOW:7���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPosInSec_0001(void)
{
	int  nPosInSec;
	
//���ý�Ŀ��Ϣ����
	CSUDIStreamInfo_S  sStreamInfo;              
	CSUDIPlaybackParam_S 	sPlaybackParam;
	int nStreamCnt,i;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
		
//���ò���������
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;
	char * szName="ts";

//���ý�Ŀ��Ϣ
	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	sPlaybackParam.m_eSpeed=EM_UDIPLAYER_SPEED_NORMAL ;
	sPlaybackParam.m_nSecondPos=0;
	strcpy(sPlaybackParam.m_szFileName, szName);

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(CSUDI_NULL,&nPosInSec), "����3ʧ��");

		//check porting code pls,it have not check pnPosInSec
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(hPlayer,CSUDI_NULL), "����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(hPlayer,&nPosInSec), "����5ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPTS          
//@DESCRIPTION:����CSUDIPLAYERGetCurPTS�ڲ�������������ִ�н��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��hPlayerΪCSUDI_NULL������ CSUDIPLAYERGetCurPTS���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��eContentTypeΪ�Ƿ�ֵ������ CSUDIPLAYERGetCurPTS���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��psPTSΪCSUDI_NULL������ CSUDIPLAYERGetCurPTS���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7���ظ�����3-����6ָ������
//@EXECUTIONFLOW:8���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0001(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	CSUDI_UINT64  sPTS;
	int i;

//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(CSUDI_NULL,eContentType,&sPTS), "����4ʧ��");

		//pls check porting code,it's have not check eContentType
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,  ERRORDATA, &sPTS), "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,CSUDI_NULL), "����6ʧ��");
	}
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPTS
//@DESCRIPTION:����CSUDIPLAYERGetCurPTS��Ӧ�ó���������Ҫ�������µ�ִ�н��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERGetCurPTS��start֮��stop֮ǰ������Ч������ʱ����Ч
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2���򿪲���������������CSUDIPLAYERGetCurPTS���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4�����ò�������������Ϣ����������CSUDIPLAYERGetCurPTS���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6�������������󣬵��� CSUDIPLAYERGetCurPTS����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERPause��ͣ������������ ����CSUDI_SUCCESS
//@EXECUTIONFLOW:8������ CSUDIPLAYERGetCurPTS����������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:9������CSUDIPLAYERResume�ָ���ͣ������ ����CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ָ���ͣ�󣬵��� CSUDIPLAYERGetCurPTS����������CSUDI_SUCCESS
//@EXECUTIONFLOW:11������CSUDIPLAYERStopֹͣ������������ ����CSUDI_SUCCESS
//@EXECUTIONFLOW:12��ֹͣ�������󣬵��� CSUDIPLAYERGetCurPTS���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:13���ظ�����4-����12ָ������
//@EXECUTIONFLOW:14���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0002(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	CSUDI_UINT64  sPTS;
	int i;

	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  asStreamInfo[10];
	int nStreamCnt;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	asStreamInfo[0].m_nPid=352;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_nPid=353;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;
	
	asStreamInfo[2].m_nPid=352;
	asStreamInfo[2].m_eContentType=EM_UDI_CONTENT_PCR;

	nStreamCnt=3;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "����3ʧ��");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		for (eContentType=EM_UDI_CONTENT_VIDEO;eContentType<=EM_UDI_CONTENT_AUDIO;eContentType++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "����4ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����5ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "����6ʧ��");

			udiRe =  CSUDIPLAYERPause(hPlayer);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == udiRe), "����7ʧ��");

			udiRe =  CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS);
			
			//pls check poritng code ,it is only work in START mode, when in PAUSE,it's return CSUDIPLAYER_ERROR_INVALID_STATUS 
			CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS  == udiRe)||(CSUDIPLAYER_ERROR_INVALID_STATUS==udiRe)), "����8ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), "����9ʧ��");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "����10ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "����12ʧ��");
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPTS          
//@DESCRIPTION:���Զ�����Ƶ��������ȡ��ƵPTS��Ϣ
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlaye
//@INPUT:2��eContentType= EM_UDI_CONTENT_AUDIO   
//@INPUT:3�� psPTS�ǿ�
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERGetCurPTS��start֮��stop֮ǰ������Ч������ʱ����Ч
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERGetCurPTS���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����4ָ������
//@EXECUTIONFLOW:6���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0003(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_AUDIO;
	CSUDI_UINT64  sPTS;
	int i=0;
	
//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPTS          
//@DESCRIPTION:���Զ�����Ƶ��������ȡ��ƵPTS��Ϣ
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlaye
//@INPUT:2��eContentType= EM_UDI_CONTENT_VIDEO   
//@INPUT:3�� psPTS�ǿ�
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERGetCurPTS��start֮��stop֮ǰ������Ч������ʱ����Ч
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERGetCurPTS���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����4ָ������
//@EXECUTIONFLOW:6���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0004(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	CSUDI_UINT64  sPTS;
	int i=0;
	
//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount          
//@DESCRIPTION:����CSUDIPLAYERGetPacketCount�ڲ�������������ִ�н��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��hPlayerΪCSUDI_NULL������ CSUDIPLAYERGetPacketCount���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��eContentTypeΪ�Ƿ�ֵ������ CSUDIPLAYERGetPacketCount���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��puPacketCntΪCSUDI_NULL������ CSUDIPLAYERGetPacketCount���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7���ظ�����3-����6ָ������
//@EXECUTIONFLOW:8���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0001(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	unsigned int   uPacketCnt;
	int i;

//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(NULL,eContentType,&uPacketCnt), "����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,  ERRORDATA, &uPacketCnt), "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,CSUDI_NULL), "����6ʧ��");
	}
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount
//@DESCRIPTION:����CSUDIPLAYERGetPacketCount��Ӧ�ó���������Ҫ�������µ�ִ�н��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERGetPacketCount��start֮��stop֮ǰ������Ч������ʱ����Ч
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2���򿪲���������������CSUDIPLAYERGetCurPTS���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4�����ò�������������Ϣ����������CSUDIPLAYERGetPacketCount���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:6�������������󣬵��� CSUDIPLAYERGetPacketCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERPause��ͣ������������ ����CSUDI_SUCCESS
//@EXECUTIONFLOW:8������ CSUDIPLAYERGetPacketCount����������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:9�������ͣ�������ɹ�������CSUDIPLAYERResume�ָ���ͣ������ ����CSUDI_SUCCESS
//@EXECUTIONFLOW:10�������ͣ�������ɹ����һָ���ͣ�ɹ��󣬵��� CSUDIPLAYERGetPacketCount����������CSUDI_SUCCESS
//@EXECUTIONFLOW:11������CSUDIPLAYERStopֹͣ������������ ����CSUDI_SUCCESS
//@EXECUTIONFLOW:12��ֹͣ�������󣬵��� CSUDIPLAYERGetPacketCount���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:13���ظ�����4-����12ָ������
//@EXECUTIONFLOW:14���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0002(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	unsigned   int uPacketCnt;
	int i;

	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  asStreamInfo[10];
	int nStreamCnt;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	asStreamInfo[0].m_nPid=352;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_nPid=353;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;
	
	asStreamInfo[2].m_nPid=352;
	asStreamInfo[2].m_eContentType=EM_UDI_CONTENT_PCR;

	nStreamCnt=3;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "����4ʧ��");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		for (eContentType=EM_UDI_CONTENT_VIDEO;eContentType<=EM_UDI_CONTENT_AUDIO;eContentType++)
		{
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����5ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "����6ʧ��");

			udiRe =  CSUDIPLAYERPause(hPlayer);

			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == udiRe), "����7ʧ��");

			udiRe =  CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt);
			
			//pls check poritng code ,it is only work in START mode, when in PAUSE,it's return CSUDIPLAYER_ERROR_INVALID_STATUS 
			CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS  == udiRe) || (CSUDIPLAYER_ERROR_INVALID_STATUS == udiRe)), "����8ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), "����9ʧ��");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "����10ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "����12ʧ��");
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount          
//@DESCRIPTION:���Զ�����Ƶ��������ȡ��ƵPacketCount
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlayer
//@INPUT:2��eContentType= EM_UDI_CONTENT_AUDIO   
//@INPUT:3�� puPacketCnt�ǿ�
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:����Ƶ��������ȡ��ƵPacketCount�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERGetPacketCount���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����4ָ������
//@EXECUTIONFLOW:6���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0003(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_AUDIO;

	unsigned int  puPacketCnt;
	
	int i=0;
	
//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&puPacketCnt), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount          
//@DESCRIPTION:���Զ�����Ƶ��������ȡ��ƵPacket Count
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlaye
//@INPUT:2��eContentType= EM_UDI_CONTENT_AUDIO   
//@INPUT:3�� puPacketCnt�ǿ�
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:����Ƶ��������ȡ��ƵPacketCount�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERGetPacketCount���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����4ָ������
//@EXECUTIONFLOW:6���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0004(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	unsigned int  puPacketCnt;
	int i=0;
	
//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&puPacketCnt), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�����");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount			
//@DESCRIPTION:���Ե���������Ƶ�ܹ�׼ȷ��ȡ��Ƶpacketcount
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlaye
//@INPUT:2��eContentType= EM_UDI_CONTENT_AUDIO	 
//@INPUT:3�� puPacketCnt�ǿ�
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:�ܹ�׼ȷ��ȡGetPacketCount
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts�����������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5������CSUDIPLAYERGetPacketCount��ȡ��Ƶ����packet��������10��֮�������������ϸı�
//@EXECUTIONFLOW:6����ʾ������Ա�ε��ź��ߣ�����5��֮����ƵPacketCountû�иı�
//@EXECUTIONFLOW:7����ʾ������Ա���²����ź��ߣ�������������ƵPacketCount�����ı�
//@EXECUTIONFLOW:8������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:9������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0005(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	unsigned int uPacketCnt = 0;
	unsigned int uLastPacketCnt = 0;
	int i;

	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");

	//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_AUDIO), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSTCPrint("��Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTCPrint("Is the audio output normal? \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ�ܣ���Ƶ���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "����5ʧ��");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ%d��\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "����5ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt!=uLastPacketCnt),"����5ʧ�ܣ���ƵPacketCntû�иı�");
		uLastPacketCnt = uPacketCnt;
	}

	CSTCPrint("��ε��ź���\n");
	CSTCPrint("Pls disattach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����6ʧ�ܣ�û�аε��ź���");

	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ5��\n");
	CSUDIOSThreadSleep(5000);//�ȴ�5���ټ������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "����6ʧ��");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ%d��\n", REPEAT_TIMES/2);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt),"����6ʧ��");
		CSTCPrint("@%d lastPacketCnt=%d , thisPacketCnt=%d \n", i, uLastPacketCnt, uPacketCnt);
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt==uLastPacketCnt),"����6ʧ�ܣ���ƵPacketCnt�仯��");
		uLastPacketCnt = uPacketCnt;
		
	}

	CSTCPrint("�����²����ź���\n");
	CSTCPrint("Pls attach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ�û�в����ź���");

	CSUDIOSThreadSleep(500);//�ȴ�
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "����6ʧ��");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ%d��\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "����6ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt!=uLastPacketCnt),"����7ʧ�ܣ���ƵPacketCntû�иı�");
		uLastPacketCnt = uPacketCnt;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����8ʧ��");
	CSTK_FATAL_POINT;

	CSTCPrint("��ȷ�������²����ź���\n");
	CSTKWaitAnyKey();

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����9ʧ��");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}
//@CASEGROUP:CSUDIPLAYERGetPacketCount			
//@DESCRIPTION:���Ե���������Ƶ�ܹ�׼ȷ��ȡ��Ƶ��packetcount
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlaye
//@INPUT:2��eContentType= EM_UDI_CONTENT_VIDEO	 
//@INPUT:3�� puPacketCnt�ǿ�
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:�ܹ�׼ȷ��ȡGetPacketCount
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts�����������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5������CSUDIPLAYERGetPacketCount��ȡ��Ƶ����packet�����������������ϸı�
//@EXECUTIONFLOW:6����ʾ������Ա�ε��ź��ߣ�����5��֮����ƵPacketCountû�иı�
//@EXECUTIONFLOW:7����ʾ������Ա���²����ź��ߣ�������������ƵPacketCount�����ı�
//@EXECUTIONFLOW:8������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:9������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0006(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	unsigned int uPacketCnt = 0;
	unsigned int uLastPacketCnt = 0;
	int i;
	
	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");
	
	//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSTCPrint("��Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTCPrint("Is the video output normal? \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ�ܣ���Ƶ���Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "����5ʧ��");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ%d��\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "����5ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt!=uLastPacketCnt),"����5ʧ�ܣ���ƵPacketCntû�иı�");
		uLastPacketCnt = uPacketCnt;
	}
	
	CSTCPrint("��ε��ź���\n");
	CSTCPrint("Pls disattach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����6ʧ�ܣ�û�аε��ź���");
	
	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ5��\n");
	CSUDIOSThreadSleep(5000);//�ȴ�5���ټ������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "����6ʧ��");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ%d��\n", REPEAT_TIMES/2);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt),"����6ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt==uLastPacketCnt),"����6ʧ�ܣ���ƵPacketCnt�仯��");
		uLastPacketCnt = uPacketCnt;
		
	}
	
	CSTCPrint("�����²����ź���\n");
	CSTCPrint("Pls attach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ�û�в����ź���");
	
	CSUDIOSThreadSleep(500);//�ȴ�
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "����6ʧ��");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ%d��\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "����6ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt!=uLastPacketCnt),"����7ʧ�ܣ���ƵPacketCntû�иı�");
		uLastPacketCnt = uPacketCnt;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����8ʧ��");
	CSTK_FATAL_POINT;

	CSTCPrint("��ȷ�������²����ź���\n");
	CSTKWaitAnyKey();
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����9ʧ��");
		hPlayer=CSUDI_NULL;
	}

	return CSUDI_TRUE;

}
//@CASEGROUP:CSUDIPLAYERGetPacketCount			
//@DESCRIPTION:���Բ�������Ƶ�ܹ�׼ȷ��ȡ��Ƶ��packetcount
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlaye
//@INPUT:2��eContentType= EM_UDI_CONTENT_AUDIO|EM_UDI_CONTENT_VIDEO	 
//@INPUT:3�� puPacketCnt�ǿ�
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:�ܹ�׼ȷ��ȡGetPacketCount
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts�����������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5������CSUDIPLAYERGetPacketCount��ȡ����Ƶ����packet�����������������ϸı�
//@EXECUTIONFLOW:6����ʾ������Ա�ε��ź��ߣ�����5��֮������ƵPacketCountû�иı�
//@EXECUTIONFLOW:7������CSUDIPLAYERClear�����������ݣ������ٻ�ȡ��������ƵPacketCount��Ϊ0�������κα仯
//@EXECUTIONFLOW:8����ʾ������Ա���²����ź��ߣ���������������ƵPacketCount�����ı�
//@EXECUTIONFLOW:9������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:10������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0007(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	unsigned int uAudioPacketCnt = 0;
	unsigned int uLastAudioPacketCnt = 0;
	unsigned int uVideoPacketCnt = 0;
	unsigned int uLastVideoPacketCnt = 0;

	int i;
	
	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");
	
	//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	
	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
    CSTCPrint("Is A/V output normal ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ�ܣ�����Ƶ���Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "����5ʧ��");
	uLastAudioPacketCnt = uAudioPacketCnt;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "����5ʧ��");
	uLastVideoPacketCnt = uVideoPacketCnt;

	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ%d��\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "����5ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uAudioPacketCnt!=uLastAudioPacketCnt),"����5ʧ�ܣ���ƵPacketCntû�иı�");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "����5ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uVideoPacketCnt!=uLastVideoPacketCnt),"����5ʧ�ܣ���ƵPacketCntû�иı�");
		uLastVideoPacketCnt = uVideoPacketCnt;
		uLastAudioPacketCnt = uAudioPacketCnt;
	}
	
	CSTCPrint("��ε��ź���\n");
	CSTCPrint("Pls disattach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����6ʧ�ܣ�û�аε��ź���");
	
	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ5��\n");
	CSUDIOSThreadSleep(5000);//�ȴ�5���ټ������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "����6ʧ��");
	uLastAudioPacketCnt = uAudioPacketCnt;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "����6ʧ��");
	uLastVideoPacketCnt = uVideoPacketCnt;

	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ%d��\n", REPEAT_TIMES/2);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "����6ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uAudioPacketCnt==uLastAudioPacketCnt),"����6ʧ�ܣ���ƵPacketCnt�仯��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "����6ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uVideoPacketCnt==uLastVideoPacketCnt),"����6ʧ��: ��ƵPacketCntPacketCnt�仯��");
		uLastVideoPacketCnt = uVideoPacketCnt;
		uLastAudioPacketCnt = uAudioPacketCnt;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClear(hPlayer), "����������л��������ʧ��");

	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ2��\n");
	CSUDIOSThreadSleep(2000);//�ȴ�2���ټ������

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL((uAudioPacketCnt == 0 || uLastAudioPacketCnt == uAudioPacketCnt), "����7ʧ�ܣ�CSUDIPLAYERClear֮����ƵPacketCnt����");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL((uVideoPacketCnt == 0 || uLastVideoPacketCnt == uVideoPacketCnt), "����7ʧ�ܣ�CSUDIPLAYERClear֮����ƵPacketCnt����");
	
	CSTCPrint("�����²����ź���\n");
	CSTCPrint("Pls attach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ�û�в����ź���");
	
	CSUDIOSThreadSleep(500);//�ȴ�
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "����7ʧ��");
	uLastAudioPacketCnt = uAudioPacketCnt;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "����7ʧ��");
	uLastVideoPacketCnt = uVideoPacketCnt;

	CSTCPrint("����CSUDIPLAYERGetPacketCount����ȴ�Լ%d��\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "����7ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uAudioPacketCnt!=uLastAudioPacketCnt),"����7ʧ�ܣ���ƵPacketCntû�иı�");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "����7ʧ��");
		CSTK_ASSERT_TRUE_FATAL((uVideoPacketCnt!=uLastVideoPacketCnt),"����7ʧ�ܣ���ƵPacketCntû�иı�");
		uLastVideoPacketCnt = uVideoPacketCnt;
		uLastAudioPacketCnt = uAudioPacketCnt;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����8ʧ��");
	CSTK_FATAL_POINT;

	CSTCPrint("��ȷ�������²����ź���\n");
	CSTKWaitAnyKey();
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����9ʧ��");
		hPlayer=CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:����CSUDIPLAYERTrickMode�ڲ�������������ִ�н��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:�Ƿ��Ĳ��ž��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��hPlayerΪCSUDI_NULL,  bTrickModeΪCSUDI_TRUE������ CSUDIPLAYERTrickMode���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��hPlayerΪCSUDI_NULL,  bTrickModeΪCSUDI_FALSE������ CSUDIPLAYERTrickMode���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0001(void)
{
//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1�򿪲�����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, &sStreamInfo, nStreamCnt, CSUDI_NULL), "����2��������Ϣʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3��ʼ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERTrickMode(CSUDI_NULL, CSUDI_TRUE), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERTrickMode(CSUDI_NULL, CSUDI_FALSE), "����5ʧ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop (hPlayer), "�ָ��ֳ�����");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "�ָ��ֳ�����");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:�����ظ�������˳�TrickMode�����
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��
//@EXPECTATION:����CSUDIPLAYER_ERROR_INVALID_STATUS
//@REMARK:
//@EXECUTIONFLOW:1����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:2������CSUDIPLAYERSetStream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYERStart����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERTrickMode,�˳�TrickModeģʽ��������TrickModeΪ�٣���������CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR
//@EXECUTIONFLOW:5������CSUDIPLAYERTrickMode,����TrickModeģʽ��������TrickModeΪ�棬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ٴε���CSUDIPLAYERTrickMode,����TrickModeģʽ��������TrickModeΪ�棬��������CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR
//@EXECUTIONFLOW:7������CSUDIPLAYERTrickMode,�˳�TrickModeģʽ��������TrickModeΪ�٣���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���ٴε���CSUDIPLAYERTrickMode,�˳�TrickModeģʽ��������TrickModeΪ�٣���������CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR
//@EXECUTIONFLOW:9���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0002(void)
{
//���ò�������Ϣ		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // ��ȡ���в��Ź��ܵĲ�����
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//���ý�Ŀ��Ϣ
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//����������
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����1�򿪲�����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, &sStreamInfo, nStreamCnt, CSUDI_NULL), "����2��������Ϣʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����3��ʼ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE), "����5ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE), "����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "����7ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "����8ʧ��");		

	CSTK_FATAL_POINT;

	if(hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop (hPlayer), "����9-1�ָ��ֳ�����");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����9-2�ָ��ֳ�����");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:�����ڱ��������£�����TrickMOdeģʽ��ִ�н��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlayer
//@EXPECTATION:���������ܹ����ٲ���,�޻�֡,���ǵ��쳣
//@REMARK:
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts�����������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:6������CSUDIPLAYERTrickMode,����TrickModeģʽ������trickModeΪ��
//@EXECUTIONFLOW:7����ʾ������Ա�۲죬�����������ٲ��ţ���ֻ��I֡�Ĳ��Ų����޻�֡,���ǵ��쳣
//@EXECUTIONFLOW:8������CSUDIPLAYERTrickMode���˳�TrickMOde
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�����ص��������ʲ���
//@EXECUTIONFLOW:10������CSUDIPLAYERStopֹͣ����
//@EXECUTIONFLOW:11������CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0003(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	CSTCPrint("CSTC_LIVE_TEST_IT_GetTrickMode_0003: ����GetTrickMode�Ƿ���Ч(Audio&Video Test_27Mbps_20070524.ts)\n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1��Ƶʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2�򿪲�����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3��������Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4��ʼ����ʧ��");

	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	udiRe = CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"����6ʧ�ܣ����Ų�����");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("��Ƶ�Ƿ���ٲ���\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ����ٲ��Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "����8ʧ��");
		CSTCPrint("����Ƶ�Ƿ�ָ�������������\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ�ܣ���ʼ���Ų�����");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����11ʧ��");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:�����ڸ��������£�����TrickMOdeģʽ��ִ�н��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlayer
//@EXPECTATION:���������ܹ����ٲ���
//@REMARK:
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts�����������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:6������CSUDIPLAYERTrickMode,����TrickModeģʽ������trickModeΪ��
//@EXECUTIONFLOW:7����ʾ������Ա�۲죬�����������ٲ��ţ���ֻ��I֡�Ĳ���
//@EXECUTIONFLOW:8������CSUDIPLAYERTrickMode���˳�TrickMOde
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬������������
//@EXECUTIONFLOW:10������CSUDIPLAYERStopֹͣ����
//@EXECUTIONFLOW:11������CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0004(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	if (-1 == PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("��֧��H264��Ƶ��ʽ\n");
		CSTCPrint("Not support STREAM_H264\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_GetTrickMode_0004: ����GetTrickMode�Ƿ���Ч(CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg)\n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "����1ʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG1, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer, EM_LIVE_H264_HD_720P, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	udiRe = CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"����6ʧ�ܣ����Ų�����");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("��Ƶ�Ƿ���ٲ���\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ����ٲ��Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "����8ʧ��");
		CSTCPrint("����Ƶ�Ƿ�ָ�������������\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ�ܣ���ʼ���Ų�����");

	}

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����11ʧ��");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:�����ڱ��������£�����TrickMOdeģʽ֮��ִ̨�еĽ��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlayer
//@EXPECTATION:������̨֮���ܹ��������Ž�Ŀ,�ܹ���������TrickMode
//@REMARK:
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts�����������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:6������CSUDIPLAYERTrickMode,����TrickModeģʽ������trickModeΪ��
//@EXECUTIONFLOW:7����ʾ������Ա�۲죬�����������ٲ��ţ���ֻ��I֡�Ĳ���
//@EXECUTIONFLOW:8������CSUDIPLAYERTrickMode,�˳�TrickModeģʽ��
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬������������
//@EXECUTIONFLOW:10������CSUDIPLAYERStopֹͣ����
//@EXECUTIONFLOW:11������CSUDIPLAYERSetStream����������Ϣ������̨
//@EXECUTIONFLOW:12������CSUDIPLAYERStart��ʼ����
//@EXECUTIONFLOW:13����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:14������CSUDIPLAYERTrickMode,����TrickModeģʽ������trickModeΪ��
//@EXECUTIONFLOW:15����ʾ������Ա�۲죬�����������ٲ��ţ���ֻ��I֡�Ĳ���
//@EXECUTIONFLOW:16������CSUDIPLAYERTrickMode,�˳�TrickModeģʽ��
//@EXECUTIONFLOW:17����ʾ������Ա�۲죬������������
//@EXECUTIONFLOW:18������CSUDIPLAYERStopֹͣ����
//@EXECUTIONFLOW:19������CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0005(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	int nStreamCnt = 2;
	CSUDIStreamInfo_S sStreamInfo[3] = {{0}};
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	sStreamInfo[0].m_nPid = 1140;

	sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_MPEG2;
	sStreamInfo[1].m_nPid = 1141;

	CSTCPrint("CSTC_LIVE_TEST_IT_GetTrickMode_0005: ����GetTrickMode�Ƿ���Ч(Audio&Video Test_27Mbps_20070524.ts)\n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1��Ƶʧ��\n");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2����������ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3����������Ϣʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4��ʼ����ʧ��\n");

	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����\n");

	udiRe = CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"����6ʧ�ܣ����Ų�����");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("��Ƶ�Ƿ���ٲ���\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ����ٲ��Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "����8ʧ�ܣ��˳�TrickMode����ʧ��");
		CSTCPrint("����Ƶ�Ƿ��ٴ�����������\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ�ܣ���ʼ���Ų�����");

	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��:ֹͣ����ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream(hPlayer, &sStreamInfo[0], nStreamCnt, NULL), "����11:�豸������Ϣʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����12:��ʼ����ʧ��");
	CSTCPrint("����Ƶ��̨֮���Ƿ��ٴ�����������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ�ܣ���ʼ���Ų�����");

	udiRe = CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"����14ʧ�ܣ����Ų�����");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("��Ƶ�Ƿ���ٲ���\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����15ʧ�ܣ����ٲ��Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "����16:�˳�TrcikModeʧ��");
		CSTCPrint("����Ƶ�Ƿ��ٴ�����������\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����17ʧ�ܣ���������ʧ��");

	}
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����18ֹͣ����ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����19�رղ�����ʧ��");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:�����ڸ��������£�����TrickMOdeģʽ֮��ִ̨�еĽ��
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ϸ���hPlayer
//@EXPECTATION:�����ܹ���̨�ɹ�
//@REMARK:
//@EXECUTIONFLOW:1�����������ļ���ǰ����CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3������CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg�����������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:6������CSUDIPLAYERTrickMode,����TrickModeģʽ������trickModeΪ��
//@EXECUTIONFLOW:7����ʾ������Ա�۲죬�����������ٲ��ţ���ֻ��I֡�Ĳ���
//@EXECUTIONFLOW:8������CSUDIPLAYERTrickMode���˳�TrickMOde
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬������������
//@EXECUTIONFLOW:10������CSUDIPLAYERStop��ֹͣ��ǰ�����Ĳ���
//@EXECUTIONFLOW:11�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:12������Audio&Video Test_27Mbps_20070524.ts��AC3��׼���Խ�Ŀ�������������ô˲�����������
//@EXECUTIONFLOW:13������CSUDIPLAYERStart����ʼ����
//@EXECUTIONFLOW:14����ʾ������Ա�۲죬�����ܹ�����CCTV1����ֱ��
//@EXECUTIONFLOW:15������CSUDIPLAYERTrickMode,����TrickModeģʽ������trickModeΪ��
//@EXECUTIONFLOW:16����ʾ������Ա�۲죬�����������ٲ��ţ���ֻ��I֡�Ĳ���
//@EXECUTIONFLOW:17������CSUDIPLAYERTrickMode���˳�TrickMOde
//@EXECUTIONFLOW:18����ʾ������Ա�۲죬������������
//@EXECUTIONFLOW:19������CSUDIPLAYERStopֹͣ����
//@EXECUTIONFLOW:20������CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0006(void)
{

	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	
	if (-1 == PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("��֧��H264��Ƶ��ʽ\n");
		CSTCPrint("Not support STREAM_H264\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_GetTrickMode_0006: ����GetTrickMode�Ƿ���Ч(CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg)\n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "����1��Ƶʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG1, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2����������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer, EM_LIVE_H264_HD_720P, \
																EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3����������Ϣʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4��ʼ����ʧ��");
	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE), "����6����TrickModeʧ��");
	CSTCPrint("����Ƶ�Ƿ������ٲ��ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ����ٲ��Ų�����");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "����8�˳�TrickModeʧ��");
	CSTCPrint("����Ƶ�Ƿ�����������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ�ܣ���������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ֹͣ����ʧ��");
												
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AC3), "����11��Ƶʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE,hPlayer, EM_LIVE_MPEG2_AC3, \
																EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����12����������Ϣʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����13��ʼ����ʧ��");
	CSTCPrint("����Ƶ��̨֮���Ƿ��ܹ��������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE), "����15����TrickModeʧ��");
	CSTCPrint("����Ƶ�Ƿ������ٲ��ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����16ʧ�ܣ����ٲ��Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "����17�˳�TrickModeʧ��");
	CSTCPrint("����Ƶ�Ƿ�����������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����18ʧ�ܣ��������Ų�����");
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����19ֹͣ����ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����20�رղ�����ʧ��");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERClear          
//@DESCRIPTION:���Դ�������Ƿ����Ϸ������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1���Ƿ���hPlayer���Ϸ���hPlayer
//@EXPECTATION:����Ƿ��������ط�CSUDI_SUCCESS������Ϸ���������CSUDI_SUCCESS
//@REMARK:����CSUDI_NULL��Ȼ����ô˽ӿڣ������ط�CSUDI_SUCCESS������Ϸ������򷵻�CSUDI_SUCCESS�����ƽ̨��֧�ִ˽ӿ��򷵻�CSUDI_ERROR_PLAYER_NOTSUPPORT
//@EXECUTIONFLOW:1������Ƿ����ž��CSUDI_NULL������CSUDIPLAYERClear���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:2������Ϸ����ž��������CSUDIPLAYERClear����������CSUDI_SUCCESS�����ƽ̨��֧�ִ˽ӿ��򷵻�CSUDI_ERROR_PLAYER_NOTSUPPORT
CSUDI_BOOL CSTC_LIVE_TEST_IT_Clear_0001(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPLAYERType_E ePlayerType;
	CSUDI_Error_Code uRetStatus;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClear(hPlayer), "����1ʧ��");

	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nAudioDecoder = 0;
	sPlayerChnl.m_nVideoDecoder = 0;
	ePlayerType = EM_UDIPLAYER_LIVE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer), "����2ʧ�ܣ�û�гɹ�����һ��Liva������");

	uRetStatus = CSUDIPLAYERClear(hPlayer);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == uRetStatus) || (CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == uRetStatus), "����2ʧ��");
	
	CSTK_FATAL_POINT
	{
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "���ٲ�����ʧ��")
		}
	}

	return CSUDI_TRUE;
}

/**
* param[in]: bIsSet ���ΪCSUDI_TRUE��ʾ����CSUDIPLAYERSetStream���ı䣬��֮����CSUDIPLAYERModifyStream
* param[in]: eSerIndex ��ӦPlayerTestServiceIndex_Eö�������еĶ���
* param[in]: dwStreamType ΪPlayerTestStreamType_Eö�����͵����,����:EM_STREAM_VIDEO|EM_STREAM_AUDIO
*/
static CSUDI_BOOL PLAYER_iSetStream(CSUDI_BOOL bIsSet,CSUDI_HANDLE hPlayer,PlayerTestServiceIndex_E eSerIndex,unsigned int dwStreamType)
{
	CSUDIStreamInfo_S  sStreamInfo[10];  //���ᳬ��10��
	int nStreamCnt = 0;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	int i = 0;
	
	if(eSerIndex >= sizeof(g_sPlayerSeviceInfo)/sizeof(PlayerTestServiceInfo_S))  return CSUDI_FALSE;
	
	switch(dwStreamType)
	{
		case (EM_STREAM_VIDEO):
		{
			sStreamInfo[0].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nVideoPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			sStreamInfo[0].m_uStreamType.m_eVideoType = g_sPlayerSeviceInfo[eSerIndex].m_eVidStreamType;
			nStreamCnt = 1;
			
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sPlayerSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		case (EM_STREAM_AUDIO):
		{
			sStreamInfo[0].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nAudioPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
			sStreamInfo[0].m_uStreamType.m_eAudioType = g_sPlayerSeviceInfo[eSerIndex].m_eAudStreamType;
			nStreamCnt = 1;
			
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sPlayerSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		case (EM_STREAM_VIDEO|EM_STREAM_AUDIO):
		{
			sStreamInfo[0].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nVideoPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			sStreamInfo[0].m_uStreamType.m_eVideoType = g_sPlayerSeviceInfo[eSerIndex].m_eVidStreamType;
			
			sStreamInfo[1].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nAudioPid;
			sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
			sStreamInfo[1].m_uStreamType.m_eAudioType = g_sPlayerSeviceInfo[eSerIndex].m_eAudStreamType;
			nStreamCnt = 2;
			
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sPlayerSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		case (EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR):
		{
			sStreamInfo[0].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nVideoPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			sStreamInfo[0].m_uStreamType.m_eVideoType = g_sPlayerSeviceInfo[eSerIndex].m_eVidStreamType;
			
			sStreamInfo[1].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nAudioPid;
			sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
			sStreamInfo[1].m_uStreamType.m_eAudioType = g_sPlayerSeviceInfo[eSerIndex].m_eAudStreamType;

			if (g_sPlayerSeviceInfo[eSerIndex].m_nPcrPid <= 0)
			{
				nStreamCnt = 2;
			}
			else
			{
				sStreamInfo[2].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nPcrPid;
				sStreamInfo[2].m_eContentType = EM_UDI_CONTENT_PCR;
				nStreamCnt = 3;
			}
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sPlayerSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		case (EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR|EM_STREAM_TELETEXT):
		{
			sStreamInfo[0].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nVideoPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			sStreamInfo[0].m_uStreamType.m_eVideoType = g_sPlayerSeviceInfo[eSerIndex].m_eVidStreamType;
			
			sStreamInfo[1].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nAudioPid;
			sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
			sStreamInfo[1].m_uStreamType.m_eAudioType = g_sPlayerSeviceInfo[eSerIndex].m_eAudStreamType;

			sStreamInfo[2].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nPcrPid;
			sStreamInfo[2].m_eContentType = EM_UDI_CONTENT_PCR;

			sStreamInfo[3].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nTelPid;
			sStreamInfo[3].m_eContentType = EM_UDI_CONTENT_TELTEXT;
			nStreamCnt = 4;
			
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sPlayerSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		case (EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR|EM_STREAM_SUBTITLE):
		{
			sStreamInfo[0].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nVideoPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			sStreamInfo[0].m_uStreamType.m_eVideoType = g_sPlayerSeviceInfo[eSerIndex].m_eVidStreamType;
			
			sStreamInfo[1].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nAudioPid;
			sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
			sStreamInfo[1].m_uStreamType.m_eAudioType = g_sPlayerSeviceInfo[eSerIndex].m_eAudStreamType;

			sStreamInfo[2].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nPcrPid;
			sStreamInfo[2].m_eContentType = EM_UDI_CONTENT_PCR;

			sStreamInfo[3].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nSubPid;
			sStreamInfo[3].m_eContentType = EM_UDI_CONTENT_SUBTITLE;
			nStreamCnt = 4;
			
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sPlayerSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		case (EM_STREAM_AUDIO|EM_STREAM_PCR|EM_STREAM_TELETEXT):
		{	
			sStreamInfo[0].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nAudioPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
			sStreamInfo[0].m_uStreamType.m_eAudioType = g_sPlayerSeviceInfo[eSerIndex].m_eAudStreamType;

			sStreamInfo[1].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nPcrPid;
			sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_PCR;

			sStreamInfo[2].m_nPid = g_sPlayerSeviceInfo[eSerIndex].m_nTelPid;
			sStreamInfo[2].m_eContentType = EM_UDI_CONTENT_TELTEXT;
			nStreamCnt = 3;
			
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sPlayerSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		default:
			CSTCPrint("Not Such Stream Type !!!!!!!\n");
	}

	CSTCPrint("���ڲ��� %s...... ��Ŀ\r\n",g_sPlayerSeviceInfo[eSerIndex].m_pcServiceDescript);
	
	if(bIsSet)
	{
		nRet = CSUDIPLAYERSetStream(hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL);
	}
	else
	{
		nRet = CSUDIPLAYERModifyStream(hPlayer,sStreamInfo,nStreamCnt);
	}

	if(!PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AC3))
	{
		for(i = 0; i < nStreamCnt; i ++)
		{
			if((EM_UDI_CONTENT_AUDIO == sStreamInfo[i].m_eContentType) && (EM_UDI_AUD_STREAM_AC3 == sStreamInfo[i].m_uStreamType.m_eAudioType))
			{
				return (CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == nRet);
			}
		}
	}
	
	return (nRet == CSUDI_SUCCESS);
}
//@CASEGROUP:PLAYER  
//@DESCRIPTION:�����ǲ����������õ�������Ƶ��Ϣ
//@REMARK:�������������е����������������Ϣ��ö��ֵ�����
//@DESCRIPTION:�ṹ����ϢΪ:
//@INPUT:typedef struct 
//@INPUT:{	
//@INPUT:	const char*          m_pcTsFilename;  	  //�����ļ�����
//@INPUT:	const char* 		 m_pcServiceDescript; 	//��������
//@INPUT:	int                  m_nVideoPid;           ///< ��������PID��-1��ʾ������
//@INPUT:	CSUDIVIDStreamType_E m_eVidStreamType;      ///��Ƶ����
//@INPUT:	int                  m_nAudioPid;           ///< ��������PID ��-1��ʾ������
//@INPUT:	CSUDIAUDStreamType_E m_eAudStreamType;      ///<��Ƶ����
//@INPUT:	int 				 m_nPcrPid;				///< PCR����PID��-1��ʾ������
//@INPUT:	int 				 m_nSubPid;				///<SUBTITLE���͵�PID��-1��ʾ������
//@INPUT:	int 				 m_nTelPid;				///<TELETEXT���͵�PID��-1��ʾ������
//@INPUT:}PlayerTestServiceInfo_S;
//@INPUT:		{
//@INPUT:			"Audio&Video Test_27Mbps_20070524.ts",
//@INPUT:			"������AC3MP2",
//@INPUT:			60,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			62,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			60,
//@INPUT:		}, /*EM_LIVE_MPEG2_MULTI_LAN1*/
//@INPUT:		{
//@INPUT:			"Audio&Video Test_27Mbps_20070524.ts",
//@INPUT:			"��������ͬ��",
//@INPUT:			100,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			101,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			100,
//@INPUT:		}, /*EM_LIVE_MPEG2_AV_SYN*/
//@INPUT:		{
//@INPUT:			"Audio&Video Test_27Mbps_20070524.ts",
//@INPUT:			"��������������",
//@INPUT:			1140,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			1142,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:		}, /*EM_LIVE_MPEG2_MULTI_LAN2*/
//@INPUT:		{
//@INPUT:			"Irdeto.PCR.CCITT.ts",
//@INPUT:			"ָ����Խ�Ŀ",
//@INPUT:			100,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			101,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			100,
//@INPUT:		}, /*EM_LIVE_MPEG2_ABILITY_TEST1*/
//@INPUT:		{
//@INPUT:			"Irdeto.PCR.CCITT.ts",
//@INPUT:			"PCR���Ϊ0.01S",
//@INPUT:			2049,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			2052,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			2049,
//@INPUT:		}, /*EM_LIVE_MPEG2_ABILITY_TEST2*/
//@INPUT:		{
//@INPUT:			"Irdeto.PCR.CCITT.ts",
//@INPUT:			"PCR���Ϊ0.1S",
//@INPUT:			2065,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			2066,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			2065,
//@INPUT:		}, /*EM_LIVE_MPEG2_ABILITY_TEST3*/
//@INPUT:		{
//@INPUT:			"H.264.ts",
//@INPUT:			"MPEG1��Ƶ���Խ�Ŀ2",
//@INPUT:			100,
//@INPUT:			EM_UDI_VID_STREAM_MPEG1,
//@INPUT:			101,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG1,
//@INPUT:			100,
//@INPUT:		}, /*EM_LIVE_MPEG1_AV*/
//@INPUT:		{
//@INPUT:			"Player_H264_SD.mpg",
//@INPUT:			"EM_LIVE_H264_SD",
//@INPUT:			100,
//@INPUT:			EM_UDI_VID_STREAM_H264,
//@INPUT:			101,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG1,
//@INPUT:			100,
//@INPUT:		}, /*EM_LIVE_H264_SD*/
//@INPUT:		{
//@INPUT:			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
//@INPUT:			"H264����720P����Ƶ",
//@INPUT:			0x1e1,
//@INPUT:			EM_UDI_VID_STREAM_H264,
//@INPUT:			0x1e2,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG1,
//@INPUT:		}, /*EM_LIVE_H264_HD_720P*/
//@INPUT:		{
//@INPUT:			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
//@INPUT:			"H264����1080I����Ƶ",
//@INPUT:			0x33c,
//@INPUT:			EM_UDI_VID_STREAM_H264,
//@INPUT:			0x33d,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG1,
//@INPUT:			0x33c,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			SERVICE_INVALID
//@INPUT:		}, /*EM_LIVE_H264_HD_1080i*/	
//@INPUT:		{
//@INPUT:			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
//@INPUT:			"�¼Ӳ���������̨ PAL",
//@INPUT:			0x300,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			0x301,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			0x200
//@INPUT:		}, /*EM_LIVE_MPEG2_PAL*/	
//@INPUT:		{
//@INPUT:			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
//@INPUT:			"�¼Ӳ���������̨ NTSC",
//@INPUT:			0x315,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			0x316,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			0x201
//@INPUT:		}, /*EM_LIVE_MPEG2_NTSC*/
//@INPUT:		{
//@INPUT:			"Audio&Video Test_27Mbps_20070524.ts",
//@INPUT:			"DolbyAC3��׼����",
//@INPUT:			512,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			650,
//@INPUT:			EM_UDI_AUD_STREAM_AC3,
//@INPUT:			128,
//@INPUT:		}, /*EM_LIVE_MPEG2_AC3*/
//@INPUT:		{
//@INPUT:			"Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts",
//@INPUT:			"����VBI��ʽ��TELTEXT",
//@INPUT:			0x8a7,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			0x8a2,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			0x8a7,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			0x8a4
//@INPUT:		}, /*EM_LIVE_MPEG2_TELETEX*/
//@INPUT:		{
//@INPUT:			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
//@INPUT:			"AC3PLUS��Ƶ��Ŀ",
//@INPUT:			0x350,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			0x351
//@INPUT:			EM_UDI_AUD_STREAM_AC3_PLUS,
//@INPUT:			0x350,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			SERVICE_INVALID
//@INPUT:		}, /*EM_LIVE_AC3PLUS*/
//@INPUT:		{
//@INPUT:			"MPEG2_AAC",
//@INPUT:			"AAC��Ƶ��Ŀ",
//@INPUT:			0x20,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			0x21,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG_AAC,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			SERVICE_INVALID
//@INPUT:		}, /*EM_LIVE_AAC*/
//@INPUT:		{
//@INPUT:			"EM_LIVE_AAC",
//@INPUT:			"EM_LIVE_AAC",
//@INPUT:			SERVICE_INVALID,
//@INPUT:			EM_UDI_VID_STREAMTYPE_NUM,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			EM_UDI_AUD_STREAMTYPE_NUM,
//@INPUT:		}, /*EM_LIVE_AAC*/
//@INPUT:		{
//@INPUT:			"EM_LIVE_AACPLUS",
//@INPUT:			"EM_LIVE_AACPLUS",
//@INPUT:			SERVICE_INVALID,
//@INPUT:			EM_UDI_VID_STREAMTYPE_NUM,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			EM_UDI_AUD_STREAMTYPE_NUM,
//@INPUT:		}, /*EM_LIVE_AACPLUS*/
//@INPUT:		{
//@INPUT:			"EM_LIVE_VC1",
//@INPUT:			"EM_LIVE_VC1",
//@INPUT:			SERVICE_INVALID,
//@INPUT:			EM_UDI_VID_STREAMTYPE_NUM,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			EM_UDI_AUD_STREAMTYPE_NUM,
//@INPUT:		}, /*EM_LIVE_VC1*/
//@INPUT:		{
//@INPUT:			"EM_LIVE_MPEG2_HD",
//@INPUT:			"EM_LIVE_MPEG2_HD",
//@INPUT:			SERVICE_INVALID,
//@INPUT:			EM_UDI_VID_STREAMTYPE_NUM,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			EM_UDI_AUD_STREAMTYPE_NUM,
//@INPUT:		}, /*EM_LIVE_MPEG2_HD*/
//@INPUT:		{
//@INPUT:			"Audio&Video Test_27Mbps_20070524.ts",
//@INPUT:			"��������ͬ��",
//@INPUT:			100,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			101,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			0,
//@INPUT:			SERVICE_INVALID,
//@INPUT:			SERVICE_INVALID
//@INPUT:		} /*EM_LIVE_MPEG2_AV_SYN_ERROR_PCR*/
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0000(void)
{
	return CSUDI_FALSE;
}
//@CASEGROUP:LivePLAYER
//@DESCRIPTION:���ԶԲ��������п��ƴӶ�ʵ������Ƶ�Ĳ���
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: �Բ������ܹ��������ƣ�������������ͣ��������ֹͣ��
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts�����������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERPause,��ͣ����������������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:7���������6����CSUDI_SUCCESS������ʾ������Ա�ж�����Ƶ�Ƿ���ͣ��ֹͣ?
//@EXECUTIONFLOW:8���������6����CSUDI_SUCCESS�������CSUDIPLAYERResume����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���������6����CSUDI_SUCCESS������ʾ������Ա�ж�����Ƶ�Ƿ�ָ�����?
//@EXECUTIONFLOW:10������CSUDIPLAYERStop,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:11����ʾ������Ա�۲죬����Ƶ�Ƿ�ֹͣ����
//@EXECUTIONFLOW:12������CSUDIPLAYERStart ��������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13����ʾ������Ա�۲죬����Ƶ�Ƿ����¿�ʼ����
//@EXECUTIONFLOW:14������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:15������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0001(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0001: ���ԶԲ��������п��ƴӶ�ʵ������Ƶ�Ĳ���(Audio&Video Test_27Mbps_20070524.ts)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0001: Test the control ability of the player \n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
    CSTCPrint("Is A/V output normal ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	udiRe = CSUDIPLAYERPause(hPlayer);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"����6ʧ�ܣ����Ų�����");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("����Ƶ�Ƿ��ٴ���ͣ������\n");
        CSTCPrint("Does the player pause ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ���ͣ���Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����8ʧ��");
		CSTCPrint("����Ƶ�Ƿ��ٴο�ʼ������\n");
        CSTCPrint("Does the player start again and the A/V has normal output ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ�ܣ���ʼ���Ų�����");

	}
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");
    CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
    CSTCPrint("Does the player stop ?\n");
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ�ܣ�ֹͣ���Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����12ʧ��");
	
	CSTCPrint("����Ƶ�Ƿ��ٴο�ʼ������\n");
    CSTCPrint("Does the player start again and the A/V has normal output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ�ܣ���ʼ���Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����14ʧ��");

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����15ʧ��");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Ե���������ƵMPEG2�������� 
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION:�������ܹ������������е�֧�ֵ���Ƶ
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts������Ƶ�������ͣ�����Ƶֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts������Ƶ�������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�ֻ����Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0002:MPEG2 ������Ƶ����(Audio&Video Test_27Mbps_20070524.ts����)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0002: Test the ability of playing SD program \n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==  PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("��ȷ���Ƿ���\"��Ƶ�����������ţ�������Ƶ���ܲ��ų���\"\n");
    CSTCPrint("Do we get normal video output and get no audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Ե���������ƵMPEG2�������� 
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:  EM_LIVE_MPEG2_HD
//@EXPECTATION:�������ܹ������������е�֧�ֵ���Ƶ
//@EXECUTIONFLOW:1�����������ļ���ǰ����Player_Mpeg_HD.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Player_Mpeg_HD.ts������Ƶ�������ͣ�����Ƶֱ��������
//@EXECUTIONFLOW:3������Player_Mpeg_HD.ts������Ƶ�������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�ֻ����Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	if (!s_bPlayerSupportHD)
	{
		CSTCPrint("��֧�ָ�����Ƶ\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0003:MPEG2 ������Ƶ����\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0003:Test the ability of playing MPEG2 HD program\n");
	
//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_HD), "����1ʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_HD,EM_STREAM_VIDEO), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	CSUDIOSThreadSleep(SLEEPTIME);
	
	CSTCPrint("��ȷ���Ƿ���\"��Ƶ�����������ţ�������Ƶ���ܲ��ų���\"\n");
	CSTCPrint("Do we get normal video output and get no audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Ե���������ƵMPEG1�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG1_AV
//@EXPECTATION: �������ܹ������������е�֧�ֵ���Ƶ
//@EXECUTIONFLOW:1�����������ļ���ǰ����H.264.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������H.264.ts������Ƶ��������(MPEG1��Ƶ)������Ƶֱ��������
//@EXECUTIONFLOW:3������H.264.ts������Ƶ����(MPEG1��Ƶ)���ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ���ֻ����Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0004(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0004:MPEG1 ������Ƶ����(H.264.ts����)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0004:Test the ability of playing MPEG1 SD video\n");

	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG1_AV), "����1ʧ��");

	//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_MPEG1, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG1_AV,EM_STREAM_VIDEO), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	
	CSTCPrint("��ȷ���Ƿ���\"��Ƶ�����������ţ�������Ƶ���ܲ��ų���\"\n");
	CSTCPrint("Do we get normal video output and get no audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");


	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Ե���������ƵH.264��������
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_H264_SD
//@EXPECTATION: ���ϵͳ֧��H264�������������ܹ������������е�֧�ֵ���Ƶ
//@EXECUTIONFLOW:1�����������ļ���ǰ����H.264.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������H.264.ts������Ƶ��������(H264��Ƶ)������Ƶֱ��������
//@EXECUTIONFLOW:3������H.264.ts������Ƶ����(H264��Ƶ)���ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ���ֻ����Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0005(void)
{
	unsigned int enStream = EM_LIVE_H264_SD;
	if(-1 != PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSUDI_HANDLE  hPlayer = CSUDI_NULL;

		CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0005:H264 ������Ƶ���Ų���\n");
        CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0005:Test the ability of playing H.264 SD video\n");

		//���������ļ��ϵ���Ϣ������Ƶ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(enStream), "����1ʧ��");

		//����������
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,enStream,EM_STREAM_VIDEO), "����3ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

		CSUDIOSThreadSleep(SLEEPTIME);

		CSTCPrint("��Ƶ�����������ţ�������Ƶ���ܲ��ų���?\n");
        CSTCPrint("Do we get normal video output and get no audio output\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

		CSTK_FATAL_POINT;
		
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
			hPlayer = CSUDI_NULL;
		}
		return CSUDI_TRUE;
		}
	else
	{
		CSTCPrint("��֧��H264��Ƶ��ʽ\r \n");
		CSTCPrint("Not supported STREAM_H264\r \n");
		return CSUDI_FALSE;
	}
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Ե���������ƵH.264��������
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_H264_HD_720P
//@EXPECTATION: ���ϵͳ֧��H264�������������ܹ������������е�֧�ֵ���Ƶ
//@EXECUTIONFLOW:1�����������ļ���ǰ��������������Ӧ��Ƶ��
//@EXECUTIONFLOW:2�����ݲ���������Ƶ��������(H264��Ƶ)������Ƶֱ��������
//@EXECUTIONFLOW:3�����ݲ���������Ƶ����(H264��Ƶ)���ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ���ֻ����Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0006(void)
{
	unsigned int enStream = EM_LIVE_H264_HD_720P;
	if(-1 != PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSUDI_HANDLE   hPlayer = CSUDI_NULL;

		CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0006:H264 ������Ƶ���Ų���\n");
        CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0006:Test the ability of playing H.264 HD video\n");

		//���������ļ��ϵ���Ϣ������Ƶ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(enStream), "����1ʧ��");

		//����������
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,enStream,EM_STREAM_VIDEO), "����3ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

		CSUDIOSThreadSleep(SLEEPTIME);

		CSTCPrint("��Ƶ�����������ţ�������Ƶ���ܲ��ų���?\n");
        CSTCPrint("Do we get normal video output and get no audio output\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

		CSTK_FATAL_POINT;
		
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
			hPlayer = CSUDI_NULL;
		}

		return CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("��֧��H264��Ƶ��ʽ\r \n");
		CSTCPrint("Not supported STREAM_H264\r \n");
		return CSUDI_FALSE;
	}
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:����PAL-NTSC��ʽ�л�ʱ�Ĳ������
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ�� 
//@INPUT:1��EM_LIVE_MPEG2_PAL
//@INPUT:2��EM_LIVE_MPEG2_NTSC
//@EXPECTATION: �����������ܹ��������ţ��������쳣����
//@EXECUTIONFLOW:1�����������ļ���ǰ����SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts��������Ƶ�������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3�����EM_UDIPLAYER_VIDEO_SOURCE_CHANGE�¼��Ļص�����
//@EXECUTIONFLOW:4������SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts��������Ƶ�������ô˲���������������
//@EXECUTIONFLOW:5������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬PAL��Ŀ�������Ƿ񲥷ų���?
//@EXECUTIONFLOW:7��PAL/NTSC��Ŀ�л���ÿ5���л�һ�Σ����л�5�Σ�����ʾ�û�ע��۲���Ƶ�仯
//@EXECUTIONFLOW:8���ж��Ƿ���EM_UDIPLAYER_VIDEO_SOURCE_CHANGE�¼��ص�����?
//@EXECUTIONFLOW:9����ʾ������Աȷ��PAL/NTSC��Ŀ�л��������Ƿ�����(����Ƶ�ж��������ܲ���)?
//@EXECUTIONFLOW:10��ɾ����ע��Ļص�����
//@EXECUTIONFLOW:11������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:12������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0007(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int nLoop = 0;
	PlayerTestServiceIndex_E eIndex = EM_LIVE_MPEG2_PAL;

    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0007:PAL-NTSC��ʽ�л�����(SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts����)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0007:Test the ability of switching between PAL and NTSC format\n");

	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "����1ʧ��");

	//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback1,EM_UDIPLAYER_VIDEO_SOURCE_CHANGE,CSUDI_NULL),	"����3ʧ��");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����4ʧ��");
	CSUDIVIDEOShow(0,TRUE);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����5ʧ��");
	
	CSUDIOSThreadSleep(SLEEPTIME);
	
	CSTCPrint("��ȷ��: �Ƿ�����ȷ���ų�PAL�ƽ�Ŀ������Ƶ?\n");
	CSTCPrint("Do we get normal PAL format A/V output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����6ʧ��");
	
	CSTCPrint("��ע��۲���Ƶ�仯�����潫���н�ĿԴPAL/NTSC�л���ÿ�γ���5�룬��5�� \n");
	CSTCPrint("Pls pay attention to the change of video output,We will switch between PAL and NTSC five times in every 5 sec\n");    
	for(nLoop = 0;nLoop < 5;nLoop++)
	{
		if((nLoop%2) == 0) 
		{
			eIndex = EM_LIVE_MPEG2_NTSC;
		}
		else
		{
			eIndex = EM_LIVE_MPEG2_PAL;
		}
		
		PLAYER_iInitCallbackPara();
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,eIndex,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iIsSourceEventCallback(SLEEPTIME*5),"����8ʧ��");
	}
	
	CSTCPrint("��ȷ��: PAL/NTSC��Ŀ�л��Ƿ�����(����Ƶ�ж��������ܲ��ų���)\n");
	CSTCPrint("Is the switch between PAL and NTSC normal?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback1,EM_UDIPLAYER_VIDEO_SOURCE_CHANGE,CSUDI_NULL),"����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop (hPlayer), "����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����12ʧ��");
		
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Ե�������MPEG1��Ƶ�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG1_AV
//@EXPECTATION: �����������ܹ��������ų�����
//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Ե���������ƵMPEG1�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG1_AV
//@EXPECTATION: �������ܹ������������е�֧�ֵ���Ƶ
//@EXECUTIONFLOW:1�����������ļ���ǰ����H.264.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������H.264.ts��MPEG1��Ŀ����Ƶ��������(MPEG1��Ƶ)������Ƶֱ��������
//@EXECUTIONFLOW:3������H.264.ts��MPEG1��Ŀ����Ƶ����(MPEG1��Ƶ)���ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ���ֻ����Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0008(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

 	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0008:MPEG1��Ƶ����\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0008:Test the ability of playing MPEG1 audio\n");

//���������ļ��ϵ���Ϣ������Ƶ

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG1_AV), "����1ʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG1, INVALIDE_TYPE, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG1_AV,EM_STREAM_AUDIO), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	
	CSTCPrint("��Ƶ��������������\n");
    CSTCPrint("Is the audio output normal ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Ե�������MPEG2��Ƶ�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: �����������ܹ��������ų�����
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��MPEG2��Ŀ����Ƶ��������(MPEG2��Ƶ)������Ƶֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��MPEG2��Ŀ����Ƶ����(MPEG2��Ƶ)���ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ���ֻ����Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0009(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

 	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0009:MPEG2�ı�����Ƶ����\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0009:Test the ability of playing MPEG2 audio\n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, INVALIDE_TYPE, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_AUDIO), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME*5);

	CSTCPrint("��Ƶ��������������\n");
    CSTCPrint("Is the audio output normal ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");


	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Ե�������AC3��Ƶ�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_AC3
//@EXPECTATION: �����������ܹ��������ų�����
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��AC3��Ŀ����Ƶ��������(AC3��Ƶ)������Ƶֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��AC3��Ŀ����Ƶ����(AC3��Ƶ)���ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ���ֻ����Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0010(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AC3))
	{
		CSTCPrint("ϵͳ֧��AC3���룬�������кϷ��豸����������� \r\n");
        CSTCPrint("The chip surpports decode of AC3 format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��AC3���룬����RCA�豸��������� \r\n");
		CSTCPrint("The chip doesn't surpport decode of AC3 format, we expect RCA having no audio output.\r\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0010:AC3��Ƶ����(Audio&Video Test_27Mbps_20070524.ts����)\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0010:Test the ability of playing AC3 audio\n");
	
//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AC3), "����1ʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_AC3, EM_UDIPLAYER_LIVE, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_AC3,EM_STREAM_AUDIO), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);

	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Does the audio output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ���MPEG2��Ƶ+MPEG2��Ƶ 
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT:1��EM_LIVE_MPEG2_AV_SYN
//@INPUT:2��EM_LIVE_MPEG2_MULTI_LAN2
//@INPUT:3��EM_LIVE_MPEG2_ABILITY_TEST1
//@INPUT:4��EM_LIVE_MPEG2_ABILITY_TEST2
//@INPUT:5��EM_LIVE_MPEG2_ABILITY_TEST3
//@EXPECTATION:�������ܹ������������е�֧�ֵ�����Ƶ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts���ڰ׳���Ŀ���������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts���ڰ׳���Ŀ������Ƶ�������ô˲��������������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾȷ��"��Ƶ���������ʱ�������Ƿ�ͬʱ����"
//@EXECUTIONFLOW:6������CSUDIPLAYERModify(),�޸������ԣ�ʹ�䲥��ͬһ�����е�����������Ŀ
//@EXECUTIONFLOW:7����ʾȷ��"����Ƶ�Ƿ�����������,�����������ֱ�Ϊ���Ӻ͹㲥������?"
//@EXECUTIONFLOW:8�����������ļ���ǰ����Irdeto.PCR.CCITT.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:9������CSUDIPLAYERModify(),�޸������ԣ�ʹ�䲥��"ָ�����"��Ŀ
//@EXECUTIONFLOW:10����ʾȷ��"����Ƶ�Ƿ�����������?"
//@EXECUTIONFLOW:11������CSUDIPLAYERModify(),�޸������ԣ�ʹ�䲥��"PCR���Ϊ0.01S"��Ŀ
//@EXECUTIONFLOW:12����ʾȷ��"����Ƶ�Ƿ�����������?"
//@EXECUTIONFLOW:13������CSUDIPLAYERModify(),�޸������ԣ�ʹ�䲥��"PCR���Ϊ0.1S"��Ŀ
//@EXECUTIONFLOW:14����ʾȷ��"����Ƶ�Ƿ�����������?"
//@EXECUTIONFLOW:15������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:16������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0011(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

 	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0011:MPEG2������Ƶ����\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0011:Test the ability of playing MPEG2 grogram \n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AV_SYN), "����1ʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_AV_SYN,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("��Ƶ���������ʱ�������Ƿ�ͬʱ����? \r\n");
    CSTCPrint("Is the audio synchronized with the video ?");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN2,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����6ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("����Ƶ�Ƿ�����������,����������Ϊ���Ӻ͹㲥������?\r\n");
    CSTCPrint("Is A/V output normal,together with TV sound  in the RCA's left output and broadcast sound in the RCA's rigth output?");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_ABILITY_TEST1), "����8ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_ABILITY_TEST1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����9ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("����Ƶ�Ƿ�����������?\r\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_ABILITY_TEST2,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����11ʧ��");
	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("����Ƶ�Ƿ�����������?\r\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_ABILITY_TEST3,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����13ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("����Ƶ�Ƿ�����������?\r\n");
    CSTCPrint("Does the audio output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����15ʧ��");
	
 	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����16ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ���MPEG2��Ƶ+AC3��Ƶ 
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: 1��EM_LIVE_MPEG2_AC3
//@EXPECTATION:�������ܹ������������е�֧�ֵ�����Ƶ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��DolbyAC3��׼���Խ�Ŀ���������ͣ�����Ƶֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��DolbyAC3��׼���Խ�Ŀ�������������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0012(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AC3))
	{
		CSTCPrint("ϵͳ֧��AC3���룬������Ƶ�����������ţ����кϷ��豸����������� \r\n");
		CSTCPrint("The chip surpports decode of AC3 format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��AC3���룬����RCA�豸��������� \r\n");
		CSTCPrint("The chip doesn't surpport decode of AC3 format, we expect RCA having no audio output.\r\n");
		return CSUDI_FALSE;
	}
	
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0012:MPEG2����Ƶ��AC3����Ƶ����(Audio&Video Test_27Mbps_20070524.ts����)\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0012:Test the ability of playing MPEG2 video and AC3 audio at the same time \n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AC3), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_AC3, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_AC3,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");



	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Does the A/V output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	
 	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ�������Ƶʱ��PCRΪ0�Ͳ�����PCR�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_AV_SYN_ERROR_PCR
//@EXPECTATION:�������ܹ������������е�֧�ֵ�����Ƶ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��DolbyAC3��׼���Խ�Ŀ���������ͣ�����Ƶֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��DolbyAC3��׼���Խ�Ŀ�������������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERModify(),�޸������ԣ�ʹ��PCR PIDΪһ������ֵ
//@EXECUTIONFLOW:7����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:8������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:9������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0013(void)
{
	CSUDIStreamInfo_S  sStreamInfo[3];
	int nStreamCnt;
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0013:���Բ�������Ƶʱ��PCRΪ0�Ͳ�����PCR�����\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0013:Test the ability of playing a program with PCR pid set to zero or to one nonexistence in the stream\n");
	
//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AV_SYN_ERROR_PCR), "����1��Ƶʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_AV_SYN_ERROR_PCR,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");
	
	sStreamInfo[0].m_nPid = g_sPlayerSeviceInfo[EM_LIVE_MPEG2_AV_SYN_ERROR_PCR].m_nVideoPid;
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType = g_sPlayerSeviceInfo[EM_LIVE_MPEG2_AV_SYN_ERROR_PCR].m_eVidStreamType;
	
	sStreamInfo[1].m_nPid = g_sPlayerSeviceInfo[EM_LIVE_MPEG2_AV_SYN_ERROR_PCR].m_nAudioPid;
	sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType = g_sPlayerSeviceInfo[EM_LIVE_MPEG2_AV_SYN_ERROR_PCR].m_eAudStreamType;

	sStreamInfo[2].m_nPid = 0xff;
	sStreamInfo[2].m_eContentType = EM_UDI_CONTENT_PCR;
	nStreamCnt = 3;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERModifyStream(hPlayer,sStreamInfo,nStreamCnt), "����6ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����8ʧ��");
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����9ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ���MPEG2��Ƶ+MPEG2��Ƶ��ʱ��ͬʱ����teltext�����
//@PRECONDITION: PLAYERģ�飬����Ƶ��������������ʼ��
//@PRECONDITION: ׼�����Բ���VBI��ʽ�ĵ��ӻ�
//@INPUT: EM_LIVE_MPEG2_TELETEX
//@EXPECTATION: �������ܹ������������е�֧�ֵ�����Ƶ�������ܹ���������TELETEXͼ�ĵ���
//@EXECUTIONFLOW:1�����������ļ���ǰ����Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts������teltext��Ŀ���������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3������Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts������teltext��Ŀ�������������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų����������ܹ���������TELETEXͼ����Ļ?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0014(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;

	if (!s_bPlayerSupportTTxt)
	{
		CSTCPrint("��֧��ͼ�ĵ���\r \n");
		CSTCPrint("Not supported teltext\r \n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0014:MPEG1����Ƶ��MPEG2����Ƶ,ͬʱ����teltext\n");
	CSTCPrint("��׼���ÿ��Բ���VBI��ʽ�ĵ��ӻ�\r\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0014:Test the ability of playing a program with Teletext context.\n");
		
//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_TELETEX), "����1��Ƶʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_TELETEX,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR|EM_STREAM_TELETEXT),
			"����3ʧ��"
		);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("������������Ƶ�������ܹ���������TELETEXͼ�ĵ��ӣ�\n");
    CSTCPrint("Is A/V output normal together with Teletext ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}
  
//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ��϶�һ����Ŀ����ֹͣ�������Ķ���
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_PAL
//@EXPECTATION:������������ʱ�򲥷���������������
//@EXECUTIONFLOW:1�����������ļ���ǰ����SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts���¼Ӳ���������̨ PAL��Ŀ���������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3������SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts���¼Ӳ���������̨ PAL��Ŀ�������������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5������CSUDIOSThreadSleep��sleep1��10��ʱ��
//@EXECUTIONFLOW:6������CSUDIPLAYERStop���رղ�����
//@EXECUTIONFLOW:7���ظ�����4-����6,��100������
//@EXECUTIONFLOW:8������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:10������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:11������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0015(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int i;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0015:���϶�һ����Ŀ����ֹͣ�������Ķ����Ĳ��� \n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0015:Test continuously start and stop playing a program\n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "����1ʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO),
			"����3ʧ��"
		);
	
	for ( i=0 ;i < 100; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
		CSUDIOSThreadSleep(SLEEPTIME);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
		CSTCPrint("ѹ���������%d ��,��100 �Ρ�\n", i);
        CSTCPrint("Total times 100. Still has %d times left.",(100-i-1));
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����11ʧ��");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ��϶�һ����Ŀ���д�������������������Ƶ���ͣ�������ֹͣ���رյĶ���
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_PAL 
//@EXPECTATION:��һ���Ĺ���ǿ�����ܹ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ����SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts���¼Ӳ���������̨ PAL��Ŀ���������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3������SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts���¼Ӳ���������̨ PAL��Ŀ�������������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5������CSUDIOSThreadSleep��sleep1��10��ʱ��
//@EXECUTIONFLOW:6������CSUDIPLAYERStop���رղ�����
//@EXECUTIONFLOW:7������CSUDIPLAYERClose��ɾ�����������ͷ���Դ
//@EXECUTIONFLOW:8���ظ�����2-����7,��100������
//@EXECUTIONFLOW:9������SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts���¼Ӳ���������̨ PAL��Ŀ���������ͣ��ٴδ�����Ƶֱ��������
//@EXECUTIONFLOW:10������SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts���¼Ӳ���������̨ PAL��Ŀ�������������ô˲����������ԣ�
//@EXECUTIONFLOW:11������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:12����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:13������CSUDIPLAYERStop���رղ�����
//@EXECUTIONFLOW:14����ʾ������Ա�۲죬����Ƶ�����Ƿ���ֹͣ?
//@EXECUTIONFLOW:15������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:16����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:17������CSUDIPLAYERPause,��ͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18����ʾ������Ա�ж�����Ƶ�Ƿ���ͣ��ֹͣ?
//@EXECUTIONFLOW:19������CSUDIPLAYERResume����������CSUDI_SUCCESS
//@EXECUTIONFLOW:20����ʾ������Ա�ж�����Ƶ�Ƿ�ָ�����?
//@EXECUTIONFLOW:21������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:22������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0016(void)
{	
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	int i;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	void *pData1= CSUDI_NULL,*pData2 = CSUDI_NULL;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0016:����Ƶ����״̬֮�䷴���л��Ĳ���\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0016:Test continuously open,setstream,start and stop a player\n");

	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "����1ʧ��");

	pData1 = CSUDIOSMalloc(0x10000);  //�����ڴ���Դ�����������
	
	//����������
	for ( i=0 ;i < 100; i++ )
	{	
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
		CSTK_ASSERT_TRUE_FATAL(
				CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO),
				"����3ʧ��"
			);
		pData2 = CSUDIOSMalloc(0x10000);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

		CSUDIOSFree(pData1);
		pData1 = CSUDI_NULL;
		
		CSUDIOSThreadSleep(SLEEPTIME);

		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

		pData1 = CSUDIOSMalloc(0x10000);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
		
		CSUDIOSFree(pData2);
		pData2 = CSUDI_NULL;
	}
	
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����9������������ʧ��");
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO),
			"����10ʧ��"
		);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����11��ʼ��ʧ��");
	CSTCPrint("����Ƶ��ʼ��������\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ�ܣ���ʼ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����13ʧ��");
	CSTCPrint("����Ƶֹͣ��������\n");
    CSTCPrint("Is A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ�ܣ�ֹͣ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����15��ʼ��ʧ��");
	CSTCPrint("����Ƶ���¿�ʼ��������\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����16ʧ�ܣ���ʼ����ʧ��");

	udiRe = CSUDIPLAYERPause(hPlayer);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== udiRe),"����17ʧ�ܣ����Ų�����");

	CSTCPrint("����Ƶ�Ƿ��ٴ�ֹͣ������\n");
	CSTCPrint("Does A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����18ʧ�ܣ���ͣ���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����19ʧ��");
	CSTCPrint("����Ƶ�Ƿ��ٴο�ʼ������\n");
	CSTCPrint("Does A/V start playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����20ʧ�ܣ���ʼ���Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����21ʧ��");
	
	CSTK_FATAL_POINT;
	
	if (pData1 != CSUDI_NULL)
	{
		CSUDIOSFree(pData1);
	}

	if (pData2 != CSUDI_NULL)
	{
		CSUDIOSFree(pData2);
	}
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����22ʧ��");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:������һ����Ŀ�������ŵ�ʱ����ͬһ�����ٴδ��������������
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION:�ٴε���ʱ�����豸æ���ҽ�Ŀ����Ӱ���������
//@REMARK: ��Ҫ�ǲ���һ���������������ŵ�ʱ����������������õ�Ӱ��
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ�������������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ��ٴδ�����Ƶֱ������������������CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:6������CSUDIOSThreadSleep��sleep 50msʱ��
//@EXECUTIONFLOW:7���ظ�����5-����6,��100������
//@EXECUTIONFLOW:8����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:9������CSUDIPLAYERStop���رղ�����
//@EXECUTIONFLOW:10����ʾ������Ա�۲죬����Ƶ�����Ƿ���ֹͣ?
//@EXECUTIONFLOW:11������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:12����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:13������CSUDIPLAYERPause,��ͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14����ʾ������Ա�ж�����Ƶ�Ƿ���ͣ��ֹͣ?
//@EXECUTIONFLOW:15������CSUDIPLAYERResume����������CSUDI_SUCCESS
//@EXECUTIONFLOW:16����ʾ������Ա�ж�����Ƶ�Ƿ�ָ�����?
//@EXECUTIONFLOW:17������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:18������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0017(void)
{	
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hTmpPlayer = CSUDI_NULL;
	CSUDI_Error_Code  udiRe;
	int i;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0017:����Ƶ����ʱ����ͬһ��������һ���������Ĳ���\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0017:Test open a player to play a program and open another player with the same parameters at the same time.\n");
	
	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");

	//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR),
			"����3ʧ��"
		);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	
	for ( i=0; i< REPEAT_TIMES; i++)
	{
		udiRe = PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hTmpPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY  == udiRe && hTmpPlayer == CSUDI_NULL, "����5ʧ��");
		CSUDIOSThreadSleep(50);
	}	

	CSTCPrint("����Ƶ������������\n");
    CSTCPrint("Does A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����8 ʧ�ܣ���ʼ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����9ʧ��");
	CSTCPrint("����Ƶֹͣ��������\n");
    CSTCPrint("Does A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ�ܣ�ֹͣ����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����11ʧ��");
	
	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("����Ƶ��ʼ��������\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ��");

	udiRe = CSUDIPLAYERPause(hPlayer);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== udiRe),"����13ʧ�ܣ����Ų�����");

	CSTCPrint("����Ƶ�Ƿ��ٴ���ͣ������\n");
	CSTCPrint("Does A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ�ܣ���ͣ���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����15ʧ��");
	
	CSTCPrint("����Ƶ�Ƿ��ٴο�ʼ������\n");
	CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����16ʧ�ܣ���ʼ���Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����17ʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����18ʧ��");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:������һ����Ŀ�������ŵ�ʱ��Խӿڽ��ж�ε��õ����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN2
//@EXPECTATION: ���ط� CSUDI_SUCCESS����Ŀ�Ĳ���״̬����Ӱ��
//@REMARK: ��Ҫ�ǲ���һ���������������ŵ�ʱ����������������õ�Ӱ��
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ�������������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����CSUDIPLAYERStart����������100�Σ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����CSUDIPLAYERResume����������100�Σ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:8������CSUDIPLAYERPause,��ͣ����������������CSUDI_SUCCESS��CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED��������سɹ����ִ�в���9-����12
//@EXECUTIONFLOW:9���ظ�����CSUDIPLAYERPause����������100�Σ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10����ʾ������Ա�ж�����Ƶ�Ƿ���ͣ��ֹͣ?
//@EXECUTIONFLOW:11������CSUDIPLAYERResume����������CSUDI_SUCCESS
//@EXECUTIONFLOW:12���ظ�����CSUDIPLAYERResume����������100�Σ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:13����ʾ������Ա�۲죬����Ƶ�Ƿ��ڲ�����?
//@EXECUTIONFLOW:14������CSUDIPLAYERStop���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15���ظ�����CSUDIPLAYERStop����������100�Σ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:16����ʾ������Ա�۲죬����Ƶ�����Ƿ���ֹͣ?
//@EXECUTIONFLOW:17������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:18����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:19������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:20������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0018(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	int i;	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0018:������һ����Ŀ�������ŵ�ʱ��Խӿڽ��ж�ε��õ����\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0018:Test the situation of invoking the player apis many times while playing a program.\n");
	
//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN2), "����1ʧ��");
	
//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN2,EM_STREAM_VIDEO|EM_STREAM_AUDIO),
			"����3ʧ��"
		);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	for ( i=0 ; i < REPEAT_TIMES; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIPLAYERStart(hPlayer), "����5ʧ��");
						
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS !=  CSUDIPLAYERResume(hPlayer)), "����6ʧ��");

	}
	
	CSTCPrint("����Ƶ��ʼ��������\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ��");

	udiRe = CSUDIPLAYERPause(hPlayer);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== udiRe),"����8ʧ��");

	for ( i=0 ; i < REPEAT_TIMES; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERPause(hPlayer), "����9ʧ��");
	}
	
	CSTCPrint("����Ƶ��ͣ��������?\n");
	CSTCPrint("Does A/V pause playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����11ʧ��");

	for ( i=0 ; i < REPEAT_TIMES; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERResume(hPlayer), "����12ʧ��");
	}

	CSTCPrint("��ȷ������Ƶ�Ƿ��ڲ�����?\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����14ʧ��");

	for ( i=0 ; i < REPEAT_TIMES; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "����15ʧ��");
	}
	CSTCPrint("����Ƶֹͣ��������\n");
    CSTCPrint("Does A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����16ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����17ʧ��");

	CSTCPrint("����Ƶ��ʼ��������\n");
    CSTCPrint("Does A/V start playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����18ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����19ʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����20ʧ��");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ���H.264�����Ŀ��MPEG2��Ŀ�л����� 
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ����ǰ��ϵͳ��Ҫ��ͬʱ������������Ƶ��
//@INPUT:1��EM_LIVE_H264_HD
//@INPUT:2��EM_LIVE_MPEG2_PAL
//@EXPECTATION:�������ܹ������������е�֧�ֵ�����Ƶ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ��������������Ӧ��Ƶ��
//@EXECUTIONFLOW:2�����ݲ�����,H264�����Ŀ���������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3�����ݲ�����,H264�����Ŀ�������������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:8�������¼Ӳ���������̨ PAL��Ŀ���������Ը��´˲����������ԣ�
//@EXECUTIONFLOW:9��sleep 5s
//@EXECUTIONFLOW:11������H264�����Ŀ���������Ը��´˲����������ԣ�
//@EXECUTIONFLOW:12���ظ�����8-����12,����2������
//@EXECUTIONFLOW:13����ʾ������Ա�۲죬�Ƿ���������Ƶ���ų���?
//@EXECUTIONFLOW:14������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:15������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0019(void)
{
	if(-1 != PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSUDI_HANDLE  hPlayer = CSUDI_NULL;
		int nLoop = 0;
		
		CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0019:����H.264�����Ŀ��MPEG2��Ŀ���л����� \n");
        	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0019:Test the situation of swithing between H.264 HD program and MPEG2 SD.\n");
		
		//���������ļ��ϵ���Ϣ������Ƶ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "����1ʧ��");

		//����������
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_HD_720P,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

		CSTCPrint("H264�����Ŀ������Ƶ��������?\n");
        CSTCPrint("Is H264 HD program playing normally ?\r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ��");
		
		for(nLoop = 0;nLoop < 5;nLoop++)
		{
			if((nLoop%2) == 0) 
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����13ʧ��");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_H264_HD_720P,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����15ʧ��");
			}
			
			CSUDIOSThreadSleep(SLEEPTIME*5);
		}
		
		CSTCPrint("��Ŀ�л�����������Ƶ�Ƿ�����������?\r\n");
        CSTCPrint("Is the player still playing normally after the swith?\r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����14ʧ�ܣ����Ų�����");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����15ʧ��");
		
		CSTK_FATAL_POINT;
		
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����16ʧ��");
			hPlayer = CSUDI_NULL;
		}

		return CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("��֧��H264��Ƶ��ʽ\n");
		CSTCPrint("Not support STREAM_H264\n");
		return CSUDI_FALSE;
	}
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION: ���Բ���AC3plus��Ƶ
//@PRECONDITION: PLAYERģ�飬����Ƶ��������������ʼ��
//@PRECONDITION: ƽ̨����֧��AC3plus��Ƶ������
//@INPUT: EM_LIVE_AC3PLUS
//@EXPECTATION: �������ܹ������������е�֧�ֵ�����Ƶ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ��������������Ӧ��Ƶ��
//@EXECUTIONFLOW:2�����ݲ�����AC3PLUS��Ŀ������Ƶ�������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3�����ݲ�����AC3PLUS��Ŀ������Ƶ�������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬��ָ��ͨ���Ƿ�����Ƶ�������
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0020(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;

	if(!PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AC3_PLUS))
	{
		CSTCPrint("ϵͳ��֧��AC3plus���� \r\n");
		CSTCPrint("The chip doesn't surpport decode of AC3plus format.\r\n");
		return CSUDI_FALSE;
	}
	
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0020:���Բ���H264��Ƶ + AC3plus��Ƶ \n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0020:Test the ability of playing MPEG2 video and AC3_PLUS audio at the same time \n");

	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AC3PLUS), "����1ʧ��");

	//����������
	nRet = PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_AC3_PLUS,EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);//, EM_UDI_VID_STREAM_MPEG2

	if(nRet == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_AC3PLUS,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
		
		if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AC3_PLUS))
		{
			CSTCPrint("ϵͳ֧��AC3plus���룬������Ƶ�����������ţ����кϷ��豸����������� \r\n");
	        CSTCPrint("The chip surpports decode of AC3_PLUS format,we expect all audio device having normal output.\r\n");
		}
		else
		{
			CSTCPrint("ϵͳ��֧��AC3plus���룬������Ƶ�����������ţ����кϷ��豸��������� \r\n");
	        CSTCPrint("The chip doesn't surpport decode of AC3plus format, we expect RCA having no audio output.\r\n");
		}
		CSTCPrint("�Ƿ�ΪԤ�ڽ��?\n ");
	    CSTCPrint("Do we get the expectation ?\n ");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��AC3plus���� \r\n");
        CSTCPrint("The chip doesn't surpports decode of AC3_PLUS format \r\n");
	}
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}		
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ���AAC��Ƶ
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_AAC
//@EXPECTATION: �������ܹ������������е�֧�ֵ�����Ƶ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ����H.264.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������H.264.ts������AAC��Ƶ��Ŀ������Ƶ�������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3������H.264.ts������AAC��Ƶ��Ŀ������Ƶ�������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬��ָ��ͨ���Ƿ�����Ƶ�������
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0021(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;

	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_MPEG_AAC))	
	{
		CSTCPrint("ϵͳ֧��MPEG_AAC���룬������Ƶ�����������ţ����кϷ��豸����������� \r\n");
		CSTCPrint("The chip surpports decode of AAC format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��MPEG_AAC����\r\n");
		CSTCPrint("The chip doesn't surpport decode of AAC format.\r\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0021:���Բ���H.264��Ƶ + AAC��Ƶ \n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0021:Test the ability of playing H.264 video and AAC audio \n");
	
	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AAC), "����1ʧ��");

	//����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY,EM_UDI_AUD_STREAM_MPEG_AAC,EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);//qluo:2011-8-21:EM_UDI_AUD_STREAM_MPEG_AAC,EM_UDI_VID_STREAM_MPEG2
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_AAC,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");


	CSTCPrint("�Ƿ�ΪԤ�ڽ��?\n ");
    CSTCPrint("Do we get the expectation ? \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����6ʧ��");
		hPlayer = CSUDI_NULL;
	}	
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ���AACplus��Ƶ
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_AACPLUS
//@EXPECTATION: �������ܹ������������е�֧�ֵ�����Ƶ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ����MPEG2_AAC.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������EM_LIVE_AACPLUS.ts������AACplus��Ƶ��Ŀ������Ƶ�������ͣ�������Ƶֱ��������
//@EXECUTIONFLOW:3������EM_LIVE_AACPLUS.ts������AACplus��Ƶ��Ŀ������Ƶ�������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬��ָ��ͨ���Ƿ�����Ƶ�������
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0022(void)
{
#if 0
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;

	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AAC_PLUS_ADTS))	
	{
		CSTCPrint("ϵͳ֧��AACPLUS���룬������Ƶ�����������ţ����кϷ��豸����������� \r\n");
		CSTCPrint("The chip surpports decode of AACPLUS format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��AACPLUS����\r\n");
		CSTCPrint("The chip doesn't surpport decode of AACPLUS format.\r\n");
		return CSUDI_FALSE;
	}
	
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0022:���Բ���AACplus��Ƶ \n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0022:Test the ability of playing AACplus audio \n");

	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AACPLUS), "����1ʧ��");

	//����������
	nRet = PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_AAC_PLUS_ADTS, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);//��������ʵ�����Editedby Qluo2011-8-22,old value:EM_UDI_VID_STREAM_MPEG2

	if(nRet == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_AACPLUS,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
		
		if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AAC_PLUS_ADTS))
		{
			CSTCPrint("ϵͳ֧��AAC_PLUS���룬�������кϷ��豸����������� \r\n");
			CSTCPrint("The chip surpports decode of AAC_PLUS format,we expect all audio device having normal output.\r\n");
		}
		else
		{
			CSTCPrint("ϵͳ��֧��AAC_PLUS���룬����RCA�豸��������� \r\n");
			CSTCPrint("The chip doesn't surpport decode of AAC_PLUS format, we expect RCA having no audio output.\r\n");
		}
		CSTCPrint("�Ƿ�ΪԤ�ڽ��?\n ");
		CSTCPrint("Do we get the expectation ? \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");
	    
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��AAC_PLUS���� \r\n");
		CSTCPrint("The chip doesn't surpports decode of AAC_PLUS format \r\n");
	}

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����6ʧ��");
		hPlayer = CSUDI_NULL;
	}	
#endif
	CSTCPrint("��ʱû�п��õ�ACCPlus��Ƶ���� \r\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ���VC1��Ƶ
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_VC1
//@EXPECTATION: �������ܹ������������е�֧�ֵ�����Ƶ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ����VC1_Apollo13_1080p.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������VC1_Apollo13_1080p.ts������Ƶ�������ͣ�����Ƶֱ��������
//@EXECUTIONFLOW:3������VC1_Apollo13_1080p.ts������Ƶ�������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�ֻ����Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0023(void)
{
	if(-1 != PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_VC1))
	{
		CSUDI_HANDLE  hPlayer = CSUDI_NULL;

		CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0023:���Բ���VC1��Ƶ\n");
        CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0023:Test the ability of playing AACplus audio \n");
	
		//���������ļ��ϵ���Ϣ������Ƶ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_VC1), "����1ʧ��");

		//����������
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_VC1, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_VC1,EM_STREAM_VIDEO), "����3ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

		CSTCPrint("������Ƶ��������\r\n");
        CSTCPrint("Is it playing normally ? \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
		
		CSTK_FATAL_POINT;
		
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
			hPlayer = CSUDI_NULL;
		}	
		return CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("��֧��VC1��Ƶ��ʽ \r\n");
		CSTCPrint("Not supported STREAM_VC1 \r\n");
		return CSUDI_FALSE;
	}
}

static void PlayerEventCallback(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void * pvUserData)
{
	UserData_S * psUserData = CSUDI_NULL;
	CSUDI_HANDLE hSem = CSUDI_NULL;
	int          nRet = 0;
	CSTCPrint("[%s] enter !  eEvent =  %d  \r\n",__FUNCTION__,eEvent);
	
	if(CSUDI_NULL == pvUserData)
	{
		CSTCPrint("pvUserData is null \r\n");
		return;
	}
	psUserData = (UserData_S *)pvUserData;

	if(CSUDI_NULL != psUserData->phSem)
	{
		hSem = *(psUserData->phSem);
	}
	else
	{
		psUserData->bTestPass = CSUDI_FALSE;
		CSTCPrint("[%s]  CSUDI_NULL == psUserData->phSem   \r\n",__FUNCTION__);
		return ;
	}

	if(1 != psUserData->nId)
	{
		psUserData->bTestPass = CSUDI_FALSE;
		CSTCPrint("[%s]  1 != psUserData->nId   \r\n",__FUNCTION__);
		return ;
	}
	
	switch(eEvent)
	{
		case EM_UDIPLAYER_VIDEO_FRAME_COMING:
			psUserData->bTestPass = CSUDI_TRUE;
			if((nRet = CSUDIPLAYERDelPlayerCallback(hPlayer,PlayerEventCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING , pvUserData)) != CSUDI_SUCCESS)
			{
				psUserData->bTestPass = CSUDI_FALSE;
				CSTCPrint("[%s] CSUDIPLAYERDelPlayerCallback fail! return  0X%x   \r\n",__FUNCTION__,nRet);
			}
			break;
		default:
			psUserData->bTestPass = CSUDI_FALSE;
			CSTCPrint("[%s] wrong eEvent   \r\n",__FUNCTION__);
			break;
	}

	if((nRet = CSUDIOSSemRelease(hSem)) != CSUDI_SUCCESS)
	{
		psUserData->bTestPass = CSUDI_FALSE;
		CSTCPrint("[%s] CSUDIOSSemRelease fail! return  0X%x   \r\n",__FUNCTION__,nRet);
	}

	CSTCPrint("[%s] end !  psUserData->bTestPass =  %d  \r\n",__FUNCTION__,psUserData->bTestPass);
	return ;
}

static void AVCtrlCallback(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void * pvUserData)
{
	UserData_S * psUserData = CSUDI_NULL;
	CSUDI_HANDLE hSem = CSUDI_NULL;
	int          nRet = 0;
	
	CSTCPrint("[%s] enter !  eEvent =  %d  \r\n",__FUNCTION__,eEvent);
	if(CSUDI_NULL == pvUserData)
	{
		CSTCPrint("pvUserData is null \r\n");
		return;
	}
	psUserData = (UserData_S *)pvUserData;

	if(CSUDI_NULL != psUserData->phSem)
	{
		hSem = *(psUserData->phSem);
	}
	else
	{
		psUserData->bTestPass = CSUDI_FALSE;
		CSTCPrint("[%s]  CSUDI_NULL == psUserData->phSem   \r\n",__FUNCTION__);
		return ;
	}

	if(2 != psUserData->nId)
	{
		psUserData->bTestPass = CSUDI_FALSE;
		CSTCPrint("[%s]  2 != psUserData->nId   \r\n",__FUNCTION__);
		return ;
	}
	
	switch(eEvent)
	{
		case EM_UDIPLAYER_VIDEO_FRAME_COMING:
			psUserData->bTestPass = CSUDI_TRUE;
			if((nRet = CSUDIPLAYERDelPlayerCallback(hPlayer,AVCtrlCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING , pvUserData)) != CSUDI_SUCCESS)
			{
				psUserData->bTestPass = CSUDI_FALSE;
				CSTCPrint("[%s] CSUDIPLAYERDelPlayerCallback fail! return  0X%x   \r\n",__FUNCTION__,nRet);
			}
			break;
		default:
			psUserData->bTestPass = CSUDI_FALSE;
			CSTCPrint("[%s] wrong eEvent   \r\n",__FUNCTION__);
			break;
	}

	if((nRet = CSUDIOSSemRelease(hSem)) != CSUDI_SUCCESS)
	{
		psUserData->bTestPass = CSUDI_FALSE;
		CSTCPrint("[%s] CSUDIOSSemRelease fail! return  0X%x   \r\n",__FUNCTION__,nRet);
	}

	CSTCPrint("[%s] end !  psUserData->bTestPass =  %d  \r\n",__FUNCTION__,psUserData->bTestPass);	
	return ;
}	
//@CASEGROUP:LivePLAYER
//@DESCRIPTION:��������Ƶ�Ĳ���ʱ������֡�¼�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��,�ź��������Ѿ�ͨ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: ע�����֡�ص������ܼ�������֡�¼��������ڻص��гɹ��Ƴ�����
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3����playerģ��ע����֡�ص�����
//@EXECUTIONFLOW:4������Audio&Video Test_27Mbps_20070524.ts�����������ô˲����������ԣ�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart������������ 
//@EXECUTIONFLOW:6�������֡�¼���2�볬ʱ����֡�ص�����������յ���֡�¼����ڻص��������Ƴ��������򷵻�FALSE 
//@EXECUTIONFLOW:7������CSUDIPLAYERStopֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERClose�ͷŲ�������Դ
//@EXECUTIONFLOW:9������CSUDIOSSemDestroy�ͷ��ź�����Դ
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0024(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hSem = CSUDI_NULL;
	UserData_S     sUserData = {CSUDI_NULL,CSUDI_FALSE,1};

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0024: ���ԶԲ��������п��ƴӶ�ʵ������Ƶ�Ĳ���(Audio&Video Test_27Mbps_20070524.ts)\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0024: Test the new frame event of the player \n");

	//1���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");
	
	//2����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	//3ע����֡�ص�
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("newframe",0 , 1, &hSem), "����3ʧ��");
	sUserData.phSem = &hSem;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,PlayerEventCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &sUserData),"����3ʧ��");

	//4���ò���������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����4ʧ��");

	//5����������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����5ʧ��");

	//6�������֡�ص�
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(hSem, 2000),"����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == sUserData.bTestPass,"����6ʧ��");

	//7ֹͣ����
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����7ʧ��");

	CSTK_FATAL_POINT;

	//8�ͷ�player��Դ
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����8ʧ��");
		hPlayer=CSUDI_NULL;
	}

	//9�ͷ��ź�����Դ
	if(CSUDI_NULL != hSem)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemDestroy(hSem),"����9ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:ģ��ʵ�ʲ������̣���������Ƶ����ʱע�������ص�����������֡�¼������ظ�������һ���ص��ظ�����ע���Ƴ�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ�����ź��������Ѿ�ͨ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: ע���������֡�ص��������ܼ�������֡�¼��������ڻص��гɹ��Ƴ�����
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3����playerģ��ע��������֡�ص�����
//@EXECUTIONFLOW:4������Audio&Video Test_27Mbps_20070524.ts�����������ô˲�����������
//@EXECUTIONFLOW:5������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:6�������֡�ص���������֡�ص�����������յ���֡�¼������ڻص��������Ƴ��������򷵻�FALSE
//@EXECUTIONFLOW:7���ظ���playerģ��ע����֡�ص�AVCtrlCallback 5�Σ�ÿ�ζ��ȴ���֡�������Ƴ�����
//@EXECUTIONFLOW:8��ÿ��ע��AVCtrlCallbackӦ�þ��ܹ��յ���֡�ص���Ϣ�Ҷ��ܹ��ɹ��Ƴ��������򷵻�FALSE
//@EXECUTIONFLOW:9������CSUDIPLAYERStopֹͣ������
//@EXECUTIONFLOW:10������CSUDIPLAYERClose�ͷŲ����������Դ
//@EXECUTIONFLOW:11������CSUDIOSSemDestroy�ͷ��ź�����Դ
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0025(void)
{

	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   ahSem[2] = {0};
	static UserData_S     asUserData[2];
	int				i = 0;
	
	memset(asUserData,0,sizeof(asUserData));

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0025: ���ԶԲ��������п��ƴӶ�ʵ������Ƶ�Ĳ���(Audio&Video Test_27Mbps_20070524.ts)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0025: Test the new frame event of the player \n");

	//1���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");
	
	//2����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��")

	//3��playerģ��ע��������֡�ص�����
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("newframe1",0 , 1, &ahSem[0]), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("newframe2",0 , 1, &ahSem[1]), "����3ʧ��");
	asUserData[0].phSem = &ahSem[0];
	asUserData[1].phSem = &ahSem[1];
	asUserData[0].nId = 1;
	asUserData[1].nId = 2;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,PlayerEventCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &asUserData[0]),"����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,AVCtrlCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &asUserData[1]),"����3ʧ��");

	//4���ò���������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����4ʧ��");

	//5����player
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����5ʧ��");

	//6�����֡�ص�
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(ahSem[0], 2000),"����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == asUserData[0].bTestPass,"����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(ahSem[1], 2000),"����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == asUserData[1].bTestPass,"����6ʧ��");

	//7�ظ�ע����֡�ص��Ĳ���
	asUserData[1].bTestPass = CSUDI_FALSE;
	for(i = 0;i < 5;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,AVCtrlCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &asUserData[1]),"����7ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(ahSem[1], 2000),"����8ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == asUserData[1].bTestPass,"����8ʧ��");

		CSUDIOSThreadSleep(100);
		
		if(CSUDI_TRUE  != asUserData[1].bTestPass)
		{
			break;
		}
	}

	//9ֹͣ������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����9ʧ��");

	CSTK_FATAL_POINT;

	//10�ͷŲ����������Դ
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����10ʧ��");
		hPlayer=CSUDI_NULL;
	}

	//11�ͷ��ź��������Դ
	if(CSUDI_NULL != ahSem[0])
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemDestroy(ahSem[0]),"����11ʧ��");
	}

	if(CSUDI_NULL != ahSem[1])
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemDestroy(ahSem[1]),"����11ʧ��");
	}
	
	
	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:��ⲥ�Ź�������Ƶ����������Ƿ������仯
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ�����ź��������Ѿ�ͨ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: ��ȡ���Ľ�����ĸ�������Ƶ�����仯
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts�����������ͣ���ֱ��������
//@EXECUTIONFLOW:3����playerģ��ע����֡�ص�����
//@EXECUTIONFLOW:4������Audio&Video Test_27Mbps_20070524.ts�����������ô˲����������ԣ�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:6�������֡�¼�
//@EXECUTIONFLOW:7��ÿ��1s��ȥ��ȡ��Ƶ������Ƶ�������������һ�ν��бȽϣ����Ƚ�5��,ÿһ������Ƶ��������Ӧ�÷����仯,���򷵻�FALSE
//@EXECUTIONFLOW:8��ֹͣ������
//@EXECUTIONFLOW:9���ͷŲ����������Դ
//@EXECUTIONFLOW:10���ͷ��ź��������Դ
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0026(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	DWORD			   nVideoCount = 0;
	DWORD			   nAudioCount = 0;
	DWORD  		   nPreVideoCount = 0;
	DWORD 		   nPreAudioCount = 0;
	int				i = 0;
	CSUDI_HANDLE   hSem = CSUDI_NULL;
	UserData_S     sUserData = {CSUDI_NULL,CSUDI_FALSE,1};

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0026: ���ԶԲ��������п��ƴӶ�ʵ������Ƶ�Ĳ���(Audio&Video Test_27Mbps_20070524.ts)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0026: Test the PacketCount of the player \n");

	//1���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");
	
	//2����������
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	//3ע����֡�ص�
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("newframe",0 , 1, &hSem), "����3ʧ��");
	sUserData.phSem = &hSem;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,PlayerEventCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &sUserData),"����3ʧ��");

	//4���ò���������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����4ʧ��");

	//5����player
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����5ʧ��");
	CSUDIOSThreadSleep(2000);//��2s�ٿ�ʼ��⣬��Ϊ���ŵ����ųɹ�������Ҫһ��ʱ�䣬���ʱ����ڽ�����������ܲ���	


	//6�������֡�ص�
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(hSem, 2000),"����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == sUserData.bTestPass,"����6ʧ��");
	
	//7�������Ƶ���ĸ����Ƿ����仯
	while(i < 5)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&nVideoCount), "����7ʧ��")
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&nAudioCount), "����7ʧ��")
		CSTK_ASSERT_TRUE_FATAL(nPreVideoCount != nVideoCount || nPreAudioCount != nAudioCount,"����7ʧ��");

		nPreAudioCount = nVideoCount;
		nPreVideoCount = nAudioCount;
		i++;

		CSUDIOSThreadSleep(1000);
	}

	//8ֹͣ������
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����8ʧ��");

	CSTK_FATAL_POINT;

	//9�ͷŲ����������Դ
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����9ʧ��");
		hPlayer=CSUDI_NULL;
	}

	//10�ͷ��ź�������Դ
	if(CSUDI_NULL != hSem)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemDestroy(hSem),"����10ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:���Բ���H264��Ƶ+MP3��Ƶ 
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MP3
//@EXPECTATION: �������ܹ������������е�֧�ֵ�����Ƶ���������
//@EXECUTIONFLOW:1�����������ļ���ǰ����H.264.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������H.264.ts��MP3��Ŀ����Ƶ��������(MP3��Ƶ)��������Ƶֱ��������
//@EXECUTIONFLOW:3������H.264.ts��MP3��Ŀ����Ƶ����(MP3��Ƶ)���ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0027(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_MP3))
	{
		CSTCPrint("ϵͳ֧��EM_UDI_AUD_STREAM_MP3���룬�������кϷ��豸����������� \r\n");
        CSTCPrint("The chip surpports decode of EM_UDI_AUD_STREAM_MP3 format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��EM_UDI_AUD_STREAM_MP3���� \r\n");
		CSTCPrint("The chip doesn't surpport decode of EM_UDI_AUD_STREAM_MP3 format.\r\n");
		return CSUDI_FALSE;
	}

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("ϵͳ֧��H264����\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��H264����\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0027:EM_UDI_AUD_STREAM_MP3��Ƶ����(H.264.ts����)\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0027:Test the ability of playing MP3 audio\n");
	
//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MP3), "����1ʧ��");
	
//����������
	
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MP3, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MP3,EM_STREAM_AUDIO|EM_STREAM_VIDEO), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");

	CSUDIOSThreadSleep(SLEEPTIME);

	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Is A/V output normal ? ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����7ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}


static CSUDI_HANDLE  g_switch_event = CSUDI_NULL;

static CSUDI_Error_Code wait_for_siwtch_event(unsigned int uTimeout )
{
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	if ( g_switch_event== CSUDI_NULL)
	{
		 if (CSUDIOSEventCreate("siwtch", CSUDIOS_EVENT_AUTO_RESET, &g_switch_event) == CSUDI_SUCCESS)
		 {
		 	udiRe = CSUDIOSEventReset(g_switch_event);
		 }
		 else
		 {
		 	CSTCPrint("�����¼�ʧ��\n");
			
		 }
	}
	if (g_switch_event  != CSUDI_NULL)
	{
		CSUDIOSEventReset(g_switch_event);
		
		udiRe = CSUDIOSEventWait(g_switch_event,uTimeout);
	}
	else
	{
		udiRe = CSUDIOS_ERROR_UNKNOWN_ERROR;
	}
	
	return udiRe;
	
}

static CSUDI_Error_Code notify_for_siwtch_event(void)
{
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	if ( g_switch_event== CSUDI_NULL)
	{
		 if (CSUDIOSEventCreate("siwtch", CSUDIOS_EVENT_AUTO_RESET, &g_switch_event) == CSUDI_SUCCESS)
		 {
		 	udiRe = CSUDIOSEventReset(g_switch_event);
		 }
		 else
		 {
		 	CSTCPrint("�����¼�ʧ��\n");
			
		 }
	}
	if (g_switch_event  != CSUDI_NULL)
	{
		udiRe = CSUDIOSEventSet(g_switch_event);
	}
	else
	{
		udiRe = CSUDIOS_ERROR_UNKNOWN_ERROR;
	}
	
	return udiRe;
	
}

#define NEW_FRAME_AT_SWITCHING  (void *)0x1234

typedef enum 
{
	EM_TIMING_BEFORE_SWITCH,
	EM_TIMING_NEW_FRAME_COMING,
	EM_TIMING_MAX_COUNT
}EM_TIMING_AT_SWITCH_T;

static unsigned int g_system_time[EM_TIMING_MAX_COUNT];

static void CS_iSetTiming(EM_TIMING_AT_SWITCH_T ePoint)
{
	if (ePoint < EM_TIMING_MAX_COUNT && ePoint >= EM_TIMING_BEFORE_SWITCH )
	{
		g_system_time[ePoint] = CSUDIPlusOSGetTickCount();
	}
	else
	{
		CSTCPrint("��������\n");
	}
}

static void CS_iResetTiming(EM_TIMING_AT_SWITCH_T ePoint)
{
	if (ePoint < EM_TIMING_MAX_COUNT && ePoint >= EM_TIMING_BEFORE_SWITCH )
	{
		g_system_time[ePoint] = 0;
	}
	else
	{
		CSTCPrint("��������\n");
	}
}


static int CS_iGetTiming(EM_TIMING_AT_SWITCH_T eBegin,EM_TIMING_AT_SWITCH_T eEnd)
{
	if ((eBegin < EM_TIMING_MAX_COUNT) && (eBegin >= EM_TIMING_BEFORE_SWITCH) && (eEnd < EM_TIMING_MAX_COUNT) && (eEnd >= EM_TIMING_BEFORE_SWITCH ))
	{
		
		return (g_system_time[eEnd] -  g_system_time[eBegin]);
	}
	else
	{
		CSTCPrint("��������\n");
		return 0;
	}
}


//ָ���¼����͵Ļص�����
static void fnPlayerNewFrameEvent(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{

	if( (eEvent==EM_UDIPLAYER_VIDEO_FRAME_COMING) && (pvUserData == NEW_FRAME_AT_SWITCHING))
	{
		CS_iSetTiming(EM_TIMING_NEW_FRAME_COMING);

		if (notify_for_siwtch_event() != CSUDI_SUCCESS)
		{
			CSTCPrint("��һ֡�¼�֪ͨʧ��%s, %d\n",__FILE__,__LINE__);
		}
	}
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:����ͬƵ��ͬ���ͽ�Ŀ(MPEG2��Ƶ)֮���л���ʱ��
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: ����MPEG2֮����л�ƽ��ʱ����1.2�����ڽ�Ϊ���ʣ�����1.2����Ҫ�Ż�
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERAddPlayerCallbackע����֡�ص�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream����"������AC3MP2"��"��������������"��Ŀ֮���л�
//@EXECUTIONFLOW:9������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:10���ȴ���һ֡�����¼�����һ֡��������㲢��ʾ��Ŀ�л�ʱ���ƽ����Ŀ�л�ʱ�䣬����ƽ����̨ʱ����1.2����
//@EXECUTIONFLOW:11��������֡�¼��ܹ���500ms���������������ʧ��
//@EXECUTIONFLOW:12���ظ�����5-����11,10������
//@EXECUTIONFLOW:13����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:14������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:15������CSUDIPLAYERDelPlayerCallbackɾ���ص�
//@EXECUTIONFLOW:16������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0001(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe= CSUDI_SUCCESS;
	
	int i=0,switchtime,totalTime=0;

 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0001:����ͬƵ��ͬ���ͽ�Ŀ(MPEG2������Ƶ)֮���л���ʱ��\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0001:Test the time of switching programs in the same frequence .\n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����3ʧ��");

	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����5ʧ��");
	
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����6ʧ�ܣ����Ų�����");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����7ʧ��");
				
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����8-1ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_NTSC, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����8-2ʧ��");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
			CSTCPrint("��%d ���л�ʱ��Ϊ%d����:,ƽ���л�ʱ��Ϊ:%d����\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
		}
		else
		{
			totalTime += 5000;
			CSTCPrint("��%d ���л���5000������û���յ���֡�¼�\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "��̨��5000ms��δ�յ���֡�¼�");
		}
	}

	CSTCPrint("%d ��ƽ���л�ʱ��Ϊ:%d����(ͬƵ��ͬ����(MPEG2)��Ŀ�л���Ҫ�󲻳���1200����)\n", i, totalTime/i);
    CSTCPrint("The average switching time is: %d(MS) with %d times\n",totalTime/i, i);
	CSTK_ASSERT_TRUE_FATAL((totalTime/i <= 1200), "ƽ����̨ʱ�䳬��1200���룬��̨�ٶ���Ҫ�Ż�");
	
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����14ʧ��");	

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����16ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:���Կ�Ƶ�㣬ͬ���ͽ�Ŀ(MPEg2)֮���л���ʱ��
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_MPEG2_PAL
//@EXPECTATION: ����MPEG2֮���Ƶ���л�ƽ��ʱ����1.5�����ڽ�Ϊ���ʣ�����1.5����Ҫ�Ż�
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERAddPlayerCallbackע����֡�ص�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:7������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:8�����������ļ���ǰ����Player_PAL_NTSC������Ӧ��Ƶ��
//@EXECUTIONFLOW:9������Player_PAL_NTSC��,"�¼Ӳ���������̨ PAL"��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:10������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:11����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:12������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:13��ǰ����Ƶ(Ƶ���ӦAudio&Video Test_27Mbps_20070524.ts/SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts����)
//@EXECUTIONFLOW:14������������Ƶ��ͽ�Ŀ��Ϣ�����ò�������������
//@EXECUTIONFLOW:15������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:16���ȴ���һ֡�����¼�����һ֡��������㲢��ʾ��Ŀ�л�ʱ���ƽ����Ŀ�л�ʱ��
//@EXECUTIONFLOW:17��������֡�¼��ܹ���500ms���������������ʧ��
//@EXECUTIONFLOW:18���ظ�����12-����17,10������
//@EXECUTIONFLOW:19������ƽ����̨ʱ����1.5������
//@EXECUTIONFLOW:20����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:21������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:22������CSUDIPLAYERDelPlayerCallbackɾ���ص�
//@EXECUTIONFLOW:23������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe= CSUDI_SUCCESS;
	
	int i=0,switchtime,totalTime=0;
	int nFreqShare = -1;
	char acValue[32] = {0};
	
 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0002:���Կ�Ƶ��ͬ���ͽ�Ŀ(MPEg2)֮���л���ʱ��\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0002:Test the time of switching programs at different frequences .\n");
	CSTCPrint("�����������������Ϊ %s �� %s \n", g_sPlayerSeviceInfo[EM_LIVE_MPEG2_MULTI_LAN1].m_pcTsFilename, g_sPlayerSeviceInfo[EM_LIVE_MPEG2_PAL].m_pcTsFilename);

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_DEV_SHARE", acValue, sizeof(acValue)))
	{
		nFreqShare = CSTKGetIntFromStr(acValue,10);
	}

	CSTK_ASSERT_TRUE_FATAL(nFreqShare == 0, "ִ�б������轫�����ļ���FREQ_DEV_SHARE����Ϊ0(�����豸������)������������������ڲ�ͬƵ�㲥��");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");

	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����5ʧ��");
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����6ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����7ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "����8ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����9ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����10ʧ��");	
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ�ܣ����Ų�����");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");
				
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����13-1ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����14-1ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "����13-2ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����14-2ʧ��");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����15ʧ��");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
			CSTCPrint("��%d���л�ʱ��Ϊ%d����:,ƽ���л�ʱ��Ϊ:%d����\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
		}
		else
		{
			totalTime += 5000;
			CSTCPrint("��%d ���л���5000������û���յ���֡�¼�\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "��̨��5000ms��δ�յ���֡�¼�");
		}
	}

	CSTCPrint("%d ��ƽ���л�ʱ��Ϊ:%d����(��Ƶ��ͬ����(MPEG2)��Ŀ�л���Ҫ�󲻳���1500����)\n", i, totalTime/i);
    CSTCPrint("The average switching time is: %d(MS) with %d times\n",totalTime/i, i);
	CSTK_ASSERT_TRUE_FATAL((totalTime/i <= 1500), "ƽ����̨ʱ�䳬��1500���룬��̨�ٶ���Ҫ�Ż�");
	
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����20ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����21ʧ��");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����23ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:����ͬƵ��H.264��Ƶ���ͽ�Ŀ֮���л���ʱ��
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_H264_HD_720P
//@EXPECTATION: ����H264����֮����л�ƽ��ʱ����2.2�����ڽ�Ϊ���ʣ�����2.2����Ҫ�Ż�
//@EXECUTIONFLOW:1�����������ļ���ǰ����SD_PAL_NTSC_and_HD_H.264_1080i_720P_merge.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2��������������H264����720P��Ŀ���������ͣ���ֱ��������
//@EXECUTIONFLOW:3��������������H264����720P��Ŀ�Ĳ��Ų�����
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream����H264 ����720P�͸���1080i��Ŀ֮���л�
//@EXECUTIONFLOW:8������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:9���ȴ���һ֡�����¼�����һ֡��������㲢��ʾ��Ŀ�л�ʱ���ƽ����Ŀ�л�ʱ��
//@EXECUTIONFLOW:10��������֡�¼��ܹ���500ms���������������ʧ��
//@EXECUTIONFLOW:11���ظ�����6-����10,10������
//@EXECUTIONFLOW:12������ƽ����̨ʱ����2.2������
//@EXECUTIONFLOW:13����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:14������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:15������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe= CSUDI_SUCCESS;
	
	int i=0,switchtime,totalTime=0;

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("ϵͳ֧��H264����\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��H264����\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	

	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0003:����ͬƵ��H.264��Ƶ���ͽ�Ŀ֮���л���ʱ��\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0003:Test the time of switching H.264 programs in the same frequence .\n");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_HD_720P, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����3ʧ��");

	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
				
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_HD_720P,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����7-1ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_HD_1080i, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����7-2ʧ��");

		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
			CSTCPrint("��%d���л�ʱ��Ϊ%d����:,ƽ���л�ʱ��Ϊ:%d����\n", i+1, switchtime,(totalTime/(i+1)));
			CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
        }
        else
        {
			totalTime += 5000;
			CSTCPrint("��%d���л���5000������û���յ���֡�¼�\n", i+1);
			CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "��̨��5000ms��δ�յ���֡�¼�");
        }
		
	}

	CSTCPrint("%d ��ƽ���л�ʱ��Ϊ:%d����(ͬƵ��ͬ����(H.264)��Ŀ�л���Ҫ�󲻳���2200����)\n", i, totalTime/i);
    CSTCPrint("The average switching time is: %d(MS) with %d times\n",totalTime/i, i);
	CSTK_ASSERT_TRUE_FATAL((totalTime/i <= 2200), "ƽ����̨ʱ�䳬��2200���룬��̨�ٶ���Ҫ�Ż�");
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����14ʧ��");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����15ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:���Կ�Ƶ�㣬��ͬ���ͽ�Ŀ(H264,MPEG2)֮���л���ʱ��
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_H264_SD
//@EXPECTATION: ����MPEG2/H264֮���Ƶ���л�ƽ��ʱ����2.0�����ڽ�Ϊ���ʣ�����2.0����Ҫ�Ż�
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7�����������ļ���ǰ����mpeg2hd+h264sd.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:8������mpeg2hd+h264sd.ts��,H264�����Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:9������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:10����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:11������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:12��ǰ����Ƶ(Ƶ���ӦAudio&Video Test_27Mbps_20070524.ts/mpeg2+h264_8M.trp����)
//@EXECUTIONFLOW:13������������Ƶ��ͽ�Ŀ��Ϣ�����ò�������������
//@EXECUTIONFLOW:14������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:15���ȴ���һ֡�����¼�����һ֡��������㲢��ʾ��Ŀ�л�ʱ���ƽ����Ŀ�л�ʱ��
//@EXECUTIONFLOW:16��������֡�¼��ܹ���500ms���������������ʧ��
//@EXECUTIONFLOW:17���ظ�����11-����16,10������
//@EXECUTIONFLOW:18������ƽ����̨ʱ����2.0������
//@EXECUTIONFLOW:19����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:20������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:21������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0004(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe= CSUDI_SUCCESS;
	int i=0,switchtime,totalTime=0;
	int nFreqShare = -1;
	char acValue[32] = {0};

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("ϵͳ֧��H264����\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��H264����\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}

 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0004:���Կ�Ƶ�㲻ͬ���ͽ�Ŀ(H264,MPEG2)֮���л���ʱ��\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0004:Test the time of switching different type(MPEG2&H.264) of programs at different frequences .\n");
	CSTCPrint("�����������������Ϊ %s �� %s \n", g_sPlayerSeviceInfo[EM_LIVE_MPEG2_MULTI_LAN1].m_pcTsFilename, g_sPlayerSeviceInfo[EM_LIVE_H264_SD].m_pcTsFilename);

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_DEV_SHARE", acValue, sizeof(acValue)))
	{
		nFreqShare = CSTKGetIntFromStr(acValue,10);
	}

	CSTK_ASSERT_TRUE_FATAL(nFreqShare == 0, "ִ�б������轫�����ļ���FREQ_DEV_SHARE����Ϊ0(�����豸������)������������������ڲ�ͬƵ�㲥��");
 	
//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");
	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "����7ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����8ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");	
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ�ܣ����Ų�����");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");
		
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����12-1ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����13-1ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "����12-2ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����13-2ʧ��");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����14ʧ��");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
			CSTCPrint("��%d���л�ʱ��Ϊ%d����:,ƽ���л�ʱ��Ϊ:%d����\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
        }
        else
        {
            totalTime += 5000;
            CSTCPrint("��%d���л���5000������û���յ���֡�¼�\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "��̨��5000ms��δ�յ���֡�¼�");
        }

	}

	CSTCPrint("%d ��ƽ���л�ʱ��Ϊ:%d����(��Ƶ�㲻ͬ����(H264,MPEG2)��Ŀ�л���Ҫ�󲻳���2000����)\n", i, totalTime/i);
    CSTCPrint("The average switching time is: %d(MS) with %d times\n",totalTime/i, i);
	CSTK_ASSERT_TRUE_FATAL((totalTime/i <= 2000), "ƽ����̨ʱ�䳬��2000���룬��̨�ٶ���Ҫ�Ż�");
	
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����19ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����20ʧ��");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����21ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

#define PLAYER_STRESS_TESTTIME (10000)

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:��Ƶ�㣬ͬ���ͽ�Ŀ(MPEg2)֮�������л����ϻ�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_MPEG2_PAL
//@EXPECTATION: ����MPEG2֮���Ƶ���л�ƽ��ʱ����1.5�����ڽ�Ϊ���ʣ�����1.5����Ҫ�Ż�
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7�����������ļ���ǰ����Player_PAL_NTSC������Ӧ��Ƶ��
//@EXECUTIONFLOW:8������Player_PAL_NTSC��,"�¼Ӳ���������̨ PAL"��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:9������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:10����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:11������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:12��ǰ����Ƶ(Ƶ���ӦAudio&Video Test_27Mbps_20070524.ts/SD_PAL_NTSC_and_HD_H.264_1080i_720P_merge.ts����)
//@EXECUTIONFLOW:13������������Ƶ��ͽ�Ŀ��Ϣ�����ò�������������
//@EXECUTIONFLOW:14������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:15���ȴ���һ֡�����¼�����һ֡��������㲢��ʾ��Ŀ�л�ʱ���ƽ����Ŀ�л�ʱ��
//@EXECUTIONFLOW:16���ظ�����11-����15,10000������
//@EXECUTIONFLOW:17���ж�ƽ���л���Ŀ��ʱ�䣬������1.5s����
//@EXECUTIONFLOW:18����ʾ������Ա�۲죬����л�������Ƶ�Ƿ����������ų���?
//@EXECUTIONFLOW:19������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:20������CSUDIPLAYERCloseɾ��������
//@REMARK:�������������Ҫ�ܳ�ʱ�����ִ����ɣ����ʺ��ڶ�ʱ�����
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0005(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	int i=0,switchtime,totalTime=0;
	int nTimeoutNum = 0;
	int nFreqShare = -1;
	char acValue[32] = {0};

 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0005:��Ƶ��ͬ���ͽ�Ŀ(MPEg2)֮�������л����ϻ����ԣ������������ִ����ɽ���Ҫ�ܳ�ʱ��(6��Сʱ����)\n");	
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0005:Test the time of switching (MPEG2) programs at different frequences .\n");
    CSTCPrint("NOTE: The testcase will take about six hours!!!!\r\n");
	CSTCPrint("�����������������Ϊ %s �� %s \n", g_sPlayerSeviceInfo[EM_LIVE_MPEG2_MULTI_LAN1].m_pcTsFilename, g_sPlayerSeviceInfo[EM_LIVE_MPEG2_PAL].m_pcTsFilename);

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_DEV_SHARE", acValue, sizeof(acValue)))
	{
		nFreqShare = CSTKGetIntFromStr(acValue,10);
	}

	CSTK_ASSERT_TRUE_FATAL(nFreqShare == 0, "ִ�б������轫�����ļ���FREQ_DEV_SHARE����Ϊ0(�����豸������)������������������ڲ�ͬƵ�㲥��");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"δ׼���ÿ�ʼ����");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");
	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "����7ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����8ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");	
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ�ܣ����Ų�����");

	for (i=0;i<PLAYER_STRESS_TESTTIME;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");
		
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����12-1ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����13-1ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "����12-2ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����13-2ʧ��");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����14ʧ��");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
            CSTCPrint("��%d���л�ʱ��Ϊ%d����:,ƽ���л�ʱ��Ϊ:%d����\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
        }
        else
        {
            totalTime += 5000;
            CSTCPrint("��%d���л���5000������û���յ���֡�¼�\n", i+1);
            CSTCPrint("We don't get video output within 5000(MS) at the %dth time\n", i+1);
			
			nTimeoutNum++;
			CSTK_ASSERT_TRUE_FATAL((nTimeoutNum < 10), "��γ���5000������û���յ���֡�¼������");
        }

	}

	CSTCPrint("ƽ���л�ʱ��Ϊ:%d����(��Ƶ��ͬ����(MPEg2)��Ŀ�л���Ҫ�󲻳���1500����)\n", (totalTime/PLAYER_STRESS_TESTTIME));
	CSTK_ASSERT_TRUE_FATAL(((totalTime/PLAYER_STRESS_TESTTIME) <= 1500), "ƽ����̨ʱ�䳬��1500���룬��̨�ٶ���Ҫ�Ż�");
	
	CSTCPrint("����л�������Ƶ����������������(û�г��ֻ�������������ס������)��\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����18ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����19ʧ��");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����20ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:��Ƶ�㣬��ͬ���ͽ�Ŀ(H264,MPEG2)֮���л����ϻ�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_H264_SD
//@EXPECTATION: ����MPEG2/H264֮���Ƶ���л�ƽ��ʱ����2.0�����ڽ�Ϊ���ʣ�����2.0����Ҫ�Ż�
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7�����������ļ���ǰ����mpeg2+h264_8M.trp������Ӧ��Ƶ��
//@EXECUTIONFLOW:8������mpeg2hd+h264sd.ts��,"�¼Ӳ���������̨ PAL"��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:9������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:10����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:11������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:12��ǰ����Ƶ(Ƶ���ӦAudio&Video Test_27Mbps_20070524.ts/mpeg2hd+h264sd.ts����)
//@EXECUTIONFLOW:13������������Ƶ��ͽ�Ŀ��Ϣ�����ò�������������
//@EXECUTIONFLOW:14������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:15���ȴ���һ֡�����¼�����һ֡��������㲢��ʾ��Ŀ�л�ʱ���ƽ����Ŀ�л�ʱ��
//@EXECUTIONFLOW:16���ظ�����11-����15,10000������
//@EXECUTIONFLOW:17���ж�ƽ���л���Ŀ��ʱ�䣬������2.0s����
//@EXECUTIONFLOW:18����ʾ������Ա�۲죬����л�������Ƶ�Ƿ����������ų���?
//@EXECUTIONFLOW:19������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:20������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0006(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe= CSUDI_SUCCESS;
	int i=0,switchtime,totalTime=0;
	int nTimeoutNum = 0;
	int nFreqShare = -1;
	char acValue[32] = {0};

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("ϵͳ֧��H264����\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��H264����\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	
 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0006:��Ƶ�㲻ͬ���ͽ�Ŀ(H264,MPEG2)֮���л����ϻ����ԣ������������ִ����ɽ���Ҫ�ܳ�ʱ��(6��Сʱ����)\n");	
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0006:Test the time of switching different type(MPEG2&H.264) of programs at different frequences .\n");
    CSTCPrint("NOTE: This a Stress testcase!It will take about six hours!!!!\r\n");
	CSTCPrint("�����������������Ϊ %s �� %s \n", g_sPlayerSeviceInfo[EM_LIVE_MPEG2_MULTI_LAN1].m_pcTsFilename, g_sPlayerSeviceInfo[EM_LIVE_H264_SD].m_pcTsFilename);

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_DEV_SHARE", acValue, sizeof(acValue)))
	{
		nFreqShare = CSTKGetIntFromStr(acValue,10);
	}

	CSTK_ASSERT_TRUE_FATAL(nFreqShare == 0, "ִ�б������轫�����ļ���FREQ_DEV_SHARE����Ϊ0(�����豸������)������������������ڲ�ͬƵ�㲥��");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"δ׼���ÿ�ʼ����");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");
	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "����7ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����8ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");	
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ�ܣ����Ų�����");

	for (i=0;i<PLAYER_STRESS_TESTTIME;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");
		
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����12-1ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����13-1ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "����12-2ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����13-2ʧ��");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����14ʧ��");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
            CSTCPrint("��%d���л�ʱ��Ϊ%d����:,ƽ���л�ʱ��Ϊ:%d����\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
        }
        else
        {
            totalTime += 5000;
            CSTCPrint("��%d���л���5000������û���յ���֡�¼�\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);

			nTimeoutNum++;
			CSTK_ASSERT_TRUE_FATAL((nTimeoutNum < 10), "��γ���5000������û���յ���֡�¼������");
        }

	}
	
	CSTCPrint("ƽ���л�ʱ��Ϊ:%d����(��Ƶ�㲻ͬ����(H264,MPEG2)��Ŀ�л���Ҫ�󲻳���2000����)\n", (totalTime/PLAYER_STRESS_TESTTIME));
	CSTK_ASSERT_TRUE_FATAL(((totalTime/PLAYER_STRESS_TESTTIME) <= 2000), "ƽ����̨ʱ�䳬��2000���룬��̨�ٶ���Ҫ�Ż�");
	
	CSTCPrint("����л�������Ƶ����������������(û�г��ֻ�������������ס������)��\n");
    CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����18ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����19ʧ��");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����20ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:ͬƵ�㣬ͬ���ͽ�Ŀ(MPEg2)֮�������л����ϻ�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_MPEG2_PAL
//@EXPECTATION: ����MPEG2֮��ͬƵ���л�ƽ��ʱ����1�����ڽ�Ϊ���ʣ�����1����Ҫ�Ż�
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7������Audio&Video Test_27Mbps_20070524.ts��,����������������Ŀ���������ͣ���ֱ��������
//@EXECUTIONFLOW:8������Audio&Video Test_27Mbps_20070524.ts��,����������������Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:9������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:10����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:11������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:12��ǰ����Ƶ(Ƶ���ӦAudio&Video Test_27Mbps_20070524.ts����)
//@EXECUTIONFLOW:13������������Ƶ��ͽ�Ŀ��Ϣ�����ò�������������
//@EXECUTIONFLOW:14������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:15���ȴ���һ֡�����¼�����һ֡��������㲢��ʾ��Ŀ�л�ʱ���ƽ����Ŀ�л�ʱ��
//@EXECUTIONFLOW:16���ظ�����11-����15,10000������
//@EXECUTIONFLOW:17���ж�ƽ���л���Ŀ��ʱ�䣬������1.0s����
//@EXECUTIONFLOW:18����ʾ������Ա�۲죬����л�������Ƶ�Ƿ����������ų���?
//@EXECUTIONFLOW:19������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:20������CSUDIPLAYERCloseɾ��������
//@REMARK:�������������Ҫ�ܳ�ʱ�����ִ����ɣ����ʺ��ڶ�ʱ�����
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0007(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	int i=0,switchtime,totalTime=0;
	int nTimeoutNum = 0;

 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0007:ͬƵ��ͬ���ͽ�Ŀ(MPEg2)֮�������л����ϻ����ԣ������������ִ����ɽ���Ҫ�ܳ�ʱ��(6��Сʱ����)\n");	
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0007:Test the time of switching (MPEG2) programs at different frequences .\n");
    CSTCPrint("NOTE: The testcase will take about six hours!!!!\r\n");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"δ׼���ÿ�ʼ����");

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE, &hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");
	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN2), "����7ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN2, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����8ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����9ʧ��");	
	CSTCPrint("����Ƶ��������������\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ�ܣ����Ų�����");

	for (i=0;i<PLAYER_STRESS_TESTTIME;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");

		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����12-1ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����13-1ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����12-1ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_MPEG2_MULTI_LAN2, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����13-2ʧ��");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����14ʧ��");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH, EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
            CSTCPrint("��%d���л�ʱ��Ϊ%d����:,ƽ���л�ʱ��Ϊ:%d����\n", i+1, switchtime, (totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n", i+1, switchtime, (totalTime/(i+1)));
        }
        else
        {
            totalTime += 5000;
            CSTCPrint("��%d���л���5000������û���յ���֡�¼�\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			
			nTimeoutNum++;
			CSTK_ASSERT_TRUE_FATAL((nTimeoutNum < 10), "��γ���5000������û���յ���֡�¼������");
        }

	}

	CSTCPrint("ƽ���л�ʱ��Ϊ:%d����(ͬƵ��ͬ����(MPEG2)��Ŀ�л���Ҫ�󲻳���1000����)\n", (totalTime/PLAYER_STRESS_TESTTIME));
	CSTK_ASSERT_TRUE_FATAL(((totalTime/PLAYER_STRESS_TESTTIME) < 1000), "ƽ����̨ʱ�䳬��1000���룬��̨�ٶ���Ҫ�Ż�");
	
	CSTCPrint("����л�������Ƶ����������������(û�г��ֻ�������������ס������)��\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����18ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����19ʧ��");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����20ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:��Ƶ�㣬��ͬ���ͽ�Ŀ(H264,MPEG2)֮������л����ϻ�����
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_H264_SD
//@EXPECTATION: ����MPEG2/H264֮���Ƶ���л�ƽ��ʱ����2.0�����ڽ�Ϊ���ʣ�����2.0����Ҫ�Ż�
//@EXECUTIONFLOW:1�����������ļ���ǰ����Audio&Video Test_27Mbps_20070524.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ͣ���ֱ��������
//@EXECUTIONFLOW:3������Audio&Video Test_27Mbps_20070524.ts��,������AC3MP2��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:5����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:6������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:7�����������ļ���ǰ����mpeg2+h264_8M.trp������Ӧ��Ƶ��
//@EXECUTIONFLOW:8������mpeg2hd+h264sd.ts��,"�¼Ӳ���������̨ PAL"��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:9������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:10����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:11������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:12��ǰ����Ƶ(Ƶ���ӦAudio&Video Test_27Mbps_20070524.ts/mpeg2hd+h264sd.ts����)
//@EXECUTIONFLOW:13������������Ƶ��ͽ�Ŀ��Ϣ�����ò�������������
//@EXECUTIONFLOW:14������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:15��sleep 80����
//@EXECUTIONFLOW:16���ظ�����11-����15,10000������
//@EXECUTIONFLOW:17����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:18������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:19������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0008(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int i=0;

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("ϵͳ֧��H264����\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��H264����\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	
 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0008:��Ƶ�㲻ͬ���ͽ�Ŀ(H264,MPEG2)֮������л����ϻ����ԣ������������ִ����ɽ���Ҫ�ܳ�ʱ��(6��Сʱ���ϣ���ע��һ��Сʱ��۲�����Ƶ�ı仯)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0008:Test quick switching between different type(MPEG2&H.264) of programs at different frequences .\n");
    CSTCPrint("NOTE: This a Stress testcase!It will take about six hours!!!!\r\n");

	CSTKWaitAnyKey();

//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����1ʧ��");

//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	CSTCPrint("����Ƶ��������������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "����7ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����8ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");	
	CSTCPrint("����Ƶ��������������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ�ܣ����Ų�����");

	for (i=0;i<PLAYER_STRESS_TESTTIME;i++)
	{
		CSTCPrint("��������%d���л�\n",i+1);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "����12-1ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����13-1ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "����12-2ʧ��");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "����13-2ʧ��");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����14ʧ��");

		CSUDIOSThreadSleep(80);

	}
	
	CSTCPrint("����Ƶ��������������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����17ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����18ʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����19ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:����ͬƵ�����h.264�����mpeg2֮����̨��ʱ��
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: EM_UDI_VID_STREAM_H264,EM_LIVE_MPEG2_PAL
//@EXPECTATION: 1.����MPEG2�л�������h.264��ʱ��С�ڵ���1.5s,��������㽨���Ż�
//@EXPECTATION: 1.����h.264�л�������MPEG2��ʱ��С�ڵ���500ms,��������㽨���Ż�
//@EXECUTIONFLOW:1�����������ļ���ǰ����SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts������Ӧ��Ƶ��
//@EXECUTIONFLOW:2����֧��H.264��ֱ��������,�������سɹ�
//@EXECUTIONFLOW:3����������VES2��Ŀ���������ô˲����������ԣ�
//@EXECUTIONFLOW:4������CSUDIPLAYERAddPlayerCallbackע����֡�ص�
//@EXECUTIONFLOW:5������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:6����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:7����¼��ʼʱ��
//@EXECUTIONFLOW:8������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:9������CSUDIPLAYERSetStream����"VES2"��"CNAI PAL"��Ŀ֮���л�
//@EXECUTIONFLOW:10������CSUDIPLAYERStart������������
//@EXECUTIONFLOW:11���ȴ���һ֡�����¼�����һ֡��������㲢��ʾ��Ŀ�л�ʱ���ƽ����Ŀ�л�ʱ�䣬����ƽ����̨ʱ����1.2����
//@EXECUTIONFLOW:12��������֡�¼��ܹ���500ms���������������ʧ��
//@EXECUTIONFLOW:13���ظ�����7-����12,20��
//@EXECUTIONFLOW:14����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���?
//@EXECUTIONFLOW:15������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:16������CSUDIPLAYERDelPlayerCallbackɾ���ص�
//@EXECUTIONFLOW:17������CSUDIPLAYERCloseɾ��������
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0009(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	int i = 0;
	int switchtime;
	int totalTime_HD2SD = 0, totalTime_SD2HD = 0;
	int nWaitTimeSD2HD = 1500, nWaitTimeHD2SD = 500;
	int nAvgTimeSD2HD, nAvgTimeHD2SD;

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("ϵͳ֧��H264����\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧��H264����\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	

	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0003:����ͬƵ��H.264��Ƶ���ͽ�Ŀ֮���л���ʱ��\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0003:Test the time of switching H.264 programs in the same frequence .\n");

	//���������ļ��ϵ���Ϣ������Ƶ
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "����1ʧ��");

	//����������
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE, &hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_H264_HD_720P, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����3ʧ��");

	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����4ʧ��");
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ�ܣ����Ų�����");

	for (i=0; i<REPEAT_TIMES*2; i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����6ʧ��");
				
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_H264_HD_720P, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����7-1ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "����7-2ʧ��");

		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

		udiRe = wait_for_siwtch_event((i%2)?nWaitTimeSD2HD:nWaitTimeHD2SD);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			if (i%2)
			{
				totalTime_SD2HD += switchtime;
			}
			else
			{
				totalTime_HD2SD += switchtime;
			}
			
			CSTCPrint("��%d��%s �л�ʱ��Ϊ%d����\n", i+1,(i%2)?"����->����":"����->����",switchtime);
			CSTCPrint("The %dth time of switching takes %d(MS) from %s\n",i+1, switchtime,(i%2)?"SD->HD":"HD->SD");
	        }
	        else
	        {
	        		if (i%2)
				{
					totalTime_SD2HD += nWaitTimeSD2HD;
					CSTCPrint("��%d �α���->�����л���%d������û���յ���֡�¼�\n", i+1,nWaitTimeSD2HD);
					CSTCPrint("We don't get new frame coming event within %d(MS) at the %dth time from SD->HD\n", nWaitTimeSD2HD,i+1);
				}
				else
				{
					totalTime_HD2SD += nWaitTimeHD2SD;
					CSTCPrint("��%d �θ���->�����л���%d������û���յ���֡�¼�\n", i+1,nWaitTimeHD2SD);
					CSTCPrint("We don't get new frame coming event within %d(MS) at the %dth time from HD->SD\n", nWaitTimeHD2SD,i+1);
				}
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "��̨�ڹ涨ʱ����δ�յ���֡�¼�");
	        }
	}

	nAvgTimeSD2HD = totalTime_SD2HD/(i/2);
	nAvgTimeHD2SD = totalTime_HD2SD/(i/2);

	CSTCPrint("%d �θ���->����ƽ���л�ʱ��Ϊ:%d����\n", i/2, nAvgTimeHD2SD);
    	CSTCPrint("The average switching time (HD->SD)is: %d(MS) with %d times\n",nAvgTimeHD2SD, i/2);

	CSTCPrint("%d �α���->����ƽ���л�ʱ��Ϊ:%d����\n", i/2, nAvgTimeSD2HD);
    	CSTCPrint("The average switching time (SD->HD)is: %d(MS) with %d times\n",nAvgTimeSD2HD, i/2);
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ�ܣ����Ų�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����14ʧ��");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "����15ʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}


static int   s_nLiveTestdata = 0;
static void  fnPlayerCallback3(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{
	CSTCPrint("����ص�����!!");
	if( eEvent==EM_UDIPLAYER_VIDEO_FRAME_COMING)
	{ 
		CSTCPrint("��֡����,������Ϣ!!\n");
		s_nLiveTestdata = 1001;
	}
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:���Խ�Ŀ����ʱ�¼�ΪEM_UDIPLAYER_VIDEO_FRAME_COMING �Ļص��������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@EXPECTATION:CSUDI_SUCCESS
//@EXECUTIONFLOW:1������PLAYER_iLockTuner����ָ����Ƶ��
//@EXECUTIONFLOW:2��������Ƶ�����������
//@EXECUTIONFLOW:3����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:4������PLAYER_iSetStream,������������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERAddPlayerCallback��ӻص��������¼�ΪEM_UDIPLAYER_VIDEO_FRAME_COMING��������Ϣs_nLiveTestdata
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���жϻص���Ϣ�Ƿ���ȷ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�2~8
//@EXECUTIONFLOW:10���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_Program_0001(void)
{
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDI_HANDLE   hAVPlayer;
	CSUDIPlayerEventType_E  eEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING;
	int nUserData = NUSERDATA;
	int i = 0;
	int j = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "����1����Ƶʧ��!");
	for(i = 0;i < 2; i++)
	{
		s_nLiveTestdata = 0;
	  	sPlayerChnl.m_nDemux = 0;
		sPlayerChnl.m_nAudioDecoder = 0;
		sPlayerChnl.m_nVideoDecoder = 0;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, FALSE),"����2���ر���Ƶʧ��!");
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE, &hAVPlayer)) && hAVPlayer != CSUDI_NULL,
			"����3���򿪲�����ʧ��! "
			);
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE, hAVPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_AUDIO|EM_STREAM_VIDEO),
			"����4�����ý�Ŀ��ʧ��! "
			);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hAVPlayer,fnPlayerCallback3,eEvent,&nUserData), "����5��ע��ص�ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hAVPlayer), "����6����ʼ���Ž�Ŀʧ��!");
		for(j = 0;j < 10 ;j ++)
		{
			if(s_nLiveTestdata ==1001)
			break;
	    		CSUDIOSThreadSleep(2000);
		}
		CSTK_ASSERT_TRUE_FATAL(s_nLiveTestdata == 1001, "����7���ص���Ϣ����!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "����8��ֹͣ���Ž�Ŀʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hAVPlayer,fnPlayerCallback3,eEvent, &nUserData), "ɾ���ص�ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hAVPlayer), "�رղ�����ʧ�� !");
		
	}
	CSTK_FATAL_POINT
	if (hAVPlayer != CSUDI_NULL)
	{
	    CSUDIPLAYERClose (hAVPlayer);
	     hAVPlayer = CSUDI_NULL;
	}
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, TRUE),"����Ƶʧ��!");
	return CSUDI_TRUE;
 
}

static void  fnPlayerCallback4(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{
	CSTCPrint("����ص�����!!");
	if( eEvent==EM_UDIPLAYER_AUDIO_SOURCE_CHANGE)
	{ 
		CSTCPrint("��ƵԴ�����ı�,������Ϣ!!\n");
		s_nLiveTestdata = 1001;
	}
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:���Խ�Ŀ����ʱ�¼�ΪEM_LIVE_AUDIO_SOURCE_CHANGE �Ļص��������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@EXPECTATION:CSUDI_SUCCESS
//@EXECUTIONFLOW:1������PLAYER_iLockTuner����ָ����Ƶ��
//@EXECUTIONFLOW:2��������Ƶ�����������
//@EXECUTIONFLOW:3����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:4������PLAYER_iSetStream,������������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERAddPlayerCallback��ӻص��������¼�ΪEM_LIVE_AUDIO_SOURCE_CHANGE��������Ϣs_nLiveTestdata
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���жϻص���Ϣ�Ƿ���ȷ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�2~8
//@EXECUTIONFLOW:10���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_Program_0002(void)
{
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDI_HANDLE   hAVPlayer;
	CSUDIPlayerEventType_E  eEvent = EM_UDIPLAYER_AUDIO_SOURCE_CHANGE;
	int nUserData = NUSERDATA;
	int i = 0;
	int j = 0;

	if(PLAYER_iIsVideoSupportDecodeType(g_sPlayerSeviceInfo[EM_LIVE_AUDIO_SOURCE_CHANGE].m_eVidStreamType))
	{
		CSTCPrint("ϵͳ֧�ֽ���\r\n");
		CSTCPrint("The chip surpports decode format\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧�ֽ���\r\n");
		CSTCPrint("The chip doesn't surpport decode format\r\n");
		return CSUDI_FALSE;
	}
	
	if(!s_abPlayerCBType[EM_UDIPLAYER_AUDIO_SOURCE_CHANGE])
	{
		CSTCPrint("��֧����ƵԴ�ı��¼�\r\n");
		CSTCPrint("Not surpport the event of audio source change.\r\n");
		return CSUDI_FALSE;
	}
	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AUDIO_SOURCE_CHANGE), "����1����Ƶʧ��!");
	for(i = 0;i < 2; i++)
	{
		s_nLiveTestdata = 0;
	  	sPlayerChnl.m_nDemux = 0;
		sPlayerChnl.m_nAudioDecoder = 0;
		sPlayerChnl.m_nVideoDecoder = 0;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, FALSE),"����2���ر���Ƶʧ��!");
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE, &hAVPlayer)) && hAVPlayer != CSUDI_NULL,
			"����3���򿪲�����ʧ��! "
			);
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE, hAVPlayer,EM_LIVE_AUDIO_SOURCE_CHANGE, EM_STREAM_AUDIO|EM_STREAM_VIDEO|EM_STREAM_PCR),
			"����4�����ý�Ŀ��ʧ��! "
			);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hAVPlayer,fnPlayerCallback4,eEvent,&nUserData), "����5��ע��ص�ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hAVPlayer), "����6����ʼ���Ž�Ŀʧ��!");
		for(j = 0;j < 10 ;j ++)
		{
			if(s_nLiveTestdata ==1001)
			break;
	    		CSUDIOSThreadSleep(2000);
		}
		CSTK_ASSERT_TRUE_FATAL(s_nLiveTestdata == 1001, "����7���ص���Ϣ����!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "����8��ֹͣ���Ž�Ŀʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hAVPlayer,fnPlayerCallback4,eEvent, &nUserData), "ɾ���ص�ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hAVPlayer), "�رղ�����ʧ�� !");
		
	}
	CSTK_FATAL_POINT
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer = CSUDI_NULL;
	}
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, TRUE),"����Ƶʧ��!");
	
	return CSUDI_TRUE;
 
}

#define AUDIO_DECODER 0
static void  fnPlayerCallback5(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{
	CSUDIAUDIOStatus_S stStatus;
	const char * pAC3 = "EM_UDI_AUD_STREAM_AC3";
	const char * pAC3Plus = "EM_UDI_AUD_STREAM_AC3_PLUS";
	const char * pCurrentType = pAC3;
	
	CSTCPrint("����ص�����!!");
	if( eEvent==EM_UDIPLAYER_AUDIO_SOURCE_CHANGE)
	{ 
		CSTCPrint("��ƵԴ�����ı�,������Ϣ!!\n");
        CSTCPrint("The audio source changes !!\n");
		CSUDIAUDIOGetStatus(AUDIO_DECODER, &stStatus);
		printf("[chenyajie] type=%d(%d, %d, %d, %d)\n", stStatus.m_eStreamType, stStatus.m_nSampleRate, stStatus.m_nBitWidth, stStatus.m_nChannels, stStatus.m_uFrameCount);
		pCurrentType = (stStatus.m_eStreamType == EM_UDI_AUD_STREAM_AC3?pAC3:pAC3Plus);
		CSTCPrint("��Ƶ���͸ı�Ϊ%s!!\n",pCurrentType);
        CSTCPrint("The auido type changed to %s !!\n",pCurrentType);
	}
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:���Խ�Ŀ����ʱ�¼�ΪEM_LIVE_AUDIO_SOURCE_CHANGE �Ļص��������
//@PRECONDITION:PLAYERģ�鱻������ʼ��
//@INPUT:1��hPlayer��Ч
//@EXPECTATION:CSUDI_SUCCESS
//@EXECUTIONFLOW:1������PLAYER_iLockTuner����ָ����Ƶ��
//@EXECUTIONFLOW:2��������Ƶ�����������
//@EXECUTIONFLOW:3����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:4������PLAYER_iSetStream,������������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERAddPlayerCallback��ӻص��������¼�ΪEM_LIVE_AUDIO_SOURCE_CHANGE��������Ϣs_nLiveTestdata
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���жϻص���Ϣ�Ƿ���ȷ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ָ��ֳ�
CSUDI_BOOL CSTC_LIVE_TEST_IT_Program_0003(void)
{
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDI_HANDLE   hAVPlayer;
	CSUDIPlayerEventType_E  eEvent = EM_UDIPLAYER_AUDIO_SOURCE_CHANGE;
	int nUserData = NUSERDATA;

	if(PLAYER_iIsVideoSupportDecodeType(g_sPlayerSeviceInfo[EM_LIVE_AUDIO_SOURCE_CHANGE].m_eVidStreamType))
	{
		CSTCPrint("ϵͳ֧�ֽ���\r\n");
		CSTCPrint("The chip surpports decode format\r\n");
	}
	else
	{
		CSTCPrint("ϵͳ��֧�ֽ���\r\n");
		CSTCPrint("The chip doesn't surpport decode format\r\n");
		return CSUDI_FALSE;
	}

	if(!s_abPlayerCBType[EM_UDIPLAYER_AUDIO_SOURCE_CHANGE])
	{
		CSTCPrint("��֧����ƵԴ�ı��¼�\r\n");
		CSTCPrint("Not surpport the event of audio source change.\r\n");
		return CSUDI_FALSE;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AUDIO_SOURCE_CHANGE), "����1����Ƶʧ��!");

  	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nAudioDecoder = AUDIO_DECODER;
	sPlayerChnl.m_nVideoDecoder = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE, &hAVPlayer)) && hAVPlayer != CSUDI_NULL,
		"����3���򿪲�����ʧ��! "
		);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE, hAVPlayer,EM_LIVE_AUDIO_SOURCE_CHANGE, EM_STREAM_AUDIO|EM_STREAM_VIDEO|EM_STREAM_PCR),
		"����4�����ý�Ŀ��ʧ��! "
		);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hAVPlayer,fnPlayerCallback5,eEvent,&nUserData), "����5��ע��ص�ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hAVPlayer), "����6����ʼ���Ž�Ŀʧ��!");

	CSTCPrint("��ȷ�ϲ��ŵ���Ƶ���������仯��ʱ����'��ƵԴ�����ı�'�Ļص���Ϣ��ӡ�����Ҵ�ӡ����Ƶ��������Ƶ����ʾ��һ��\n");
	CSTCPrint("Make sure that we get the audio source change event! And make sure  the notified log is consistent with the live playing!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7���ص���Ϣ����!");
	//CSTK_ASSERT_TRUE_FATAL(s_nLiveTestdata == 1001, "�ص���Ϣ����!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "����8��ֹͣ���Ž�Ŀʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hAVPlayer,fnPlayerCallback5,eEvent, &nUserData), "ɾ���ص�ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hAVPlayer), "�رղ�����ʧ�� !");
		
	CSTK_FATAL_POINT
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
 
}



