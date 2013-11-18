/**
 *@version 1.0.1 2009/11/06  重构原始版本，修改各个工具接口，优化流程
 *@version 1.0.2 2009/11/05  重构原始版本，修改设置播放流类型为统一接口
 *@version 1.0.1 2009/11/01  重构原始版本，修改锁频方式为统一接口
*/
/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
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
    EM_LIVE_MPEG2_MULTI_LAN1,          /*多语言AC3MP2*/
	EM_LIVE_MPEG2_AV_SYN,              /*测试视音同步*/
	EM_LIVE_MPEG2_MULTI_LAN2,	         /*多语言左右声道*/
	EM_LIVE_MPEG2_ABILITY_TEST1,       /*指标测试节目*/
	EM_LIVE_MPEG2_ABILITY_TEST2,      /*PCR间隔为0.01S*/
	EM_LIVE_MPEG2_ABILITY_TEST3,      /*PCR间隔为0.1S*/
	EM_LIVE_MPEG1_AV,					/*MPEG1视频测试节目2*/
	EM_LIVE_H264_SD,    				 /*H264标清节目*/
	EM_LIVE_H264_HD_720P,                  /*H264高清720P节目*/
	EM_LIVE_H264_HD_1080i,                  /*H264高清1080I节目*/
	EM_LIVE_MPEG2_PAL,                /*新加波亚洲新闻台 PAL*/
	EM_LIVE_MPEG2_NTSC,				/*新加波亚洲新闻台 NTSC*/
	EM_LIVE_MPEG2_AC3,				/*DolbyAC3标准测试*/
	EM_LIVE_MPEG2_TELETEX,			 /*包含teltext的码流*/
	EM_LIVE_AC3PLUS,	       			 /*包含AC3PLUS音频*/
	EM_LIVE_AAC,						/*包含AAC音频*/
	EM_LIVE_AACPLUS,					/*包含AACPLUS音频*/
	EM_LIVE_VC1,						/*包含VC1视频*/
	EM_LIVE_MPEG2_HD,					/*包含mpeg2高清视频*/
	EM_LIVE_MPEG2_AV_SYN_ERROR_PCR,  /*PCR为0*/
	EM_LIVE_COORDINATE_SHIFT,              /*包含坐标偏移验证码流*/
	EM_LIVE_AUDIO_SOURCE_CHANGE, 	/*包含音频类型改变验证码流*/
	EM_LIVE_MP3,						/*包含MP3(MPEG1/2,layer 3)音频*/
	EM_LIVE_MAX
	
}PlayerTestServiceIndex_E;

/**
* 测试用例中用到的节目相关的码流信息
*/
typedef struct 
{	
	const char*          m_pcTsFilename;  	  //码流文件名称
	const char* 		 m_pcServiceDescript; 	//码流描述
	int                  m_nVideoPid;           ///< 数据所在PID，-1表示不存在
	CSUDIVIDStreamType_E m_eVidStreamType;      ///视频类型
	int                  m_nAudioPid;           ///< 数据所在PID ，-1表示不存在
	CSUDIAUDStreamType_E m_eAudStreamType;      ///<音频类型
	int 				 m_nPcrPid;				///< PCR类型PID，-1表示不存在
	int 				 m_nSubPid;				///<SUBTITLE类型的PID，-1表示不存在
	int 				 m_nTelPid;				///<TELETEXT类型的PID，-1表示不存在
}PlayerTestServiceInfo_S;

#define SERVICE_INVALID  (-1)
#define STREAM_TYPE_INVALID  (0xff)

static PlayerTestServiceInfo_S g_sPlayerSeviceInfo[] ={
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"多语言AC3MP2",
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
			"测试视音同步",
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
			"多语言左右声道",
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
			"指标测试节目",
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
			"PCR间隔为0.01S",
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
			"PCR间隔为0.1S",
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
			"MPEG1视频测试节目2",
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
			"H264高清720P音视频",
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
			"H264高清1080I音视频",
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
			"新加波亚洲新闻台 PAL",
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
			"新加波亚洲新闻台 NTSC",
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
			"DolbyAC3标准测试",
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
			"测试VBI方式的TELTEXT",
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
			"AC3PLUS音频节目",
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
			"AAC音频节目",
			0x1001,
			EM_UDI_VID_STREAM_H264,
			0x1002,
			EM_UDI_AUD_STREAM_MPEG_AAC,
			0x1001,
			SERVICE_INVALID,
			SERVICE_INVALID
		}, /*EM_LIVE_AAC*/
		{
			"EM_LIVE_AACPLUS.ts", //暂无此种音频类型的码流
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
			"测试视音同步节目PCR为0和其它值",
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
			"测试视频坐标偏移",
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
			"测试音频类型改变事件回调",
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
			"MP3标准测试(MPEG1/2,layer 3)",
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
@brief 读取配置文件信息,初始化player相关配置

@return 读取成功返回CS_TK_CONFIG_SUCCESS,失败返回CS_TK_CONFIG_FAILURE
*/
int LIVEPLAY_iInitCfg()
{
	char cBuf[32] = {0};
	int nCfgInfoIndex = 0;
	int nResult = 0;

	for(; nCfgInfoIndex < EM_UDIPLAYER_MAXEVENTTYPE; nCfgInfoIndex++)
	{
		//获得信息
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
	//在本测试用例集执行前调用
	//iTest();
	CSUDIAUDIOSetVolume(0, 31);
		CSUDIVIDEOShow(0, CSUDI_TRUE);
		CSUDIVIDEOSetAspectRatio(0,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_AUTO);
		CSUDIVIDEOSetMatchMethod(0,EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_MATCH_METHOD_IGNORE);
	
	return (CS_TK_CONFIG_SUCCESS == LIVEPLAY_iInitCfg());
}

CSUDI_BOOL CSTC_LIVE_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}
static CSUDI_BOOL PLAYER_iSetStream(CSUDI_BOOL bIsSet,CSUDI_HANDLE hPlayer,PlayerTestServiceIndex_E eSerIndex,unsigned int dwStreamType);

static CSUDI_BOOL PLAYER_iLockTuner(PlayerTestServiceIndex_E eSerIndex)
{
	return CSTC_UDI2PortingLock(UDI2_TUNER0_ID,g_sPlayerSeviceInfo[eSerIndex].m_pcTsFilename);
}

//@CASEGROUP:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback函数的执行效果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数不符合设计要求
//@EXPECTATION:非CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback参数要求请参考UDI移植指南
//@EXECUTIONFLOW:1、打开一个直播播放器
//@EXECUTIONFLOW:2、hPlayer为CSUDI_NULL时，调用CSUDIPLAYERAddPlayerCallback，期望返回CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:3、fnPlayerCallback为CSUDI_NULL 时，调用CSUDIPLAYERAddPlayerCallback，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、eEvent 为EM_UDIPLAYER_MAXEVENTTYPE 时，调用CSUDIPLAYERAddPlayerCallback，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、eEvent 为EM_UDIPLAYER_MAXEVENTTYPE+100 时，调用CSUDIPLAYERAddPlayerCallback，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、hPlayer为CSUDI_NULL时，调用CSUDIPLAYERDelPlayerCallback，期望返回CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:7、fnPlayerCallback为CSUDI_NULL时，调用CSUDIPLAYERDelPlayerCallback，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、eEvent为EM_UDIPLAYER_MAXEVENTTYPE时，调用CSUDIPLAYERDelPlayerCallback，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、eEvent为EM_UDIPLAYER_MAXEVENTTYPE+100时，调用CSUDIPLAYERDelPlayerCallback，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、pvUserData为CSUDI_NULL时，调用CSUDIPLAYERDelPlayerCallback，期望返回CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
//@EXECUTIONFLOW:11、pvUserData为0x1234时，调用CSUDIPLAYERDelPlayerCallback，期望返回CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
//@EXECUTIONFLOW:12、pvUserData为合法指针时，调用CSUDIPLAYERDelPlayerCallback，期望返回CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
//@EXECUTIONFLOW:13、重复步骤2-步骤12指定次数
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0001(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDIPlayerEventType_E  eEvent=EM_UDIPLAYER_VIDEO_FRAME_COMING;
	
	int  nUserData=NUSERDATA;

	int i=0;
	
//创建播放器参数
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;
	
//创建播放器
	hPlayer = PLAYER_openPlayer(uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1 失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//testing CSUDIPLAYERAddPlayerCallback 
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERAddPlayerCallback(CSUDI_NULL,fnPlayerCallback,eEvent,&nUserData), "步骤2 失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERAddPlayerCallback(hPlayer,CSUDI_NULL,eEvent,&nUserData), "步骤3 失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback,EM_UDIPLAYER_MAXEVENTTYPE,&nUserData), "步骤4 失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback,EM_UDIPLAYER_MAXEVENTTYPE+100,&nUserData), "步骤5 失败");
		
		//testing CSUDIPLAYERDelPlayerCallback 
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERDelPlayerCallback (CSUDI_NULL,fnPlayerCallback,eEvent,&nUserData), "步骤6 失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERDelPlayerCallback(hPlayer,CSUDI_NULL,eEvent,&nUserData), "步骤7 失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,EM_UDIPLAYER_MAXEVENTTYPE,&nUserData), "步骤8 失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,EM_UDIPLAYER_MAXEVENTTYPE+100,&nUserData), "步骤9 失败");
		

		//testing CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST	
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST == CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, CSUDI_NULL), "步骤10 失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST == CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, (void *)0x1234), "步骤11 失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST == CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, &nUserData), "步骤12 失败");
		
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤14 失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback
//@DESCRIPTION:测试所有参数并且应用场景均符合接口设计要求，CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback函数的执行效果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、所有参数均符合接口设计要求
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback参数要求请参考UDI移植指南
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERAddPlayerCallback添加回调函数的三个不同事例(pnUserData为CSUDI_NULL,0x1234以及合任意合法地址值)，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERDelPlayerCallback删除步骤2中添加的三个回调函数事例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复步骤2-步骤3指定次数
//@EXECUTIONFLOW:5、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDIPlayerEventType_E  eEvent=EM_UDIPLAYER_VIDEO_FRAME_COMING;
	
	int  nUserData=NUSERDATA;

	int i=0;
	
//创建播放器参数
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;
	
//创建播放器
	hPlayer = PLAYER_openPlayer(uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1 失败");

	for (i=0;i < REPEAT_TIMES; i++)
	{
		for (eEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING; eEvent<EM_UDIPLAYER_MAXEVENTTYPE;eEvent++)
		{
			if (!s_abPlayerCBType[eEvent])
			{
				continue;
			}
			//testing CSUDIPLAYERAddPlayerCallback
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,&nUserData), "步骤2-1 失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,CSUDI_NULL), "步骤2-2 失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,(void *)0x1234), "步骤2-3 失败");

			//testing CSUDIPLAYERDelPlayerCallback
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, &nUserData), "步骤3-1 失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, CSUDI_NULL), "步骤3-2 失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, (void *)0x1234), "步骤3-3 失败");
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤5 失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback
//@DESCRIPTION:测试利用CSUDIPLAYERAddPlayerCallback 最少能成功添加32个回调函数
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、所有参数均符合接口设计要求
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback参数要求请参考UDI移植指南
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERAddPlayerCallback添加32个回调函数事例，期望均返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERDelPlayerCallback删除步骤2中添加的32个回调函数事例，期望均返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复步骤2-步骤3指定次数
//@EXECUTIONFLOW:5、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDIPlayerEventType_E  eEvent=EM_UDIPLAYER_VIDEO_FRAME_COMING;
	
	int i=0, j=0;
	int  anUserData[33] = {0};
	
//创建播放器参数
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;
	
//创建播放器
	hPlayer = PLAYER_openPlayer(uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1 失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//添加32个回调	
		for (j=0; j<32; j++)
		{
			anUserData[j] = j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback,eEvent,&anUserData[j]), "步骤九失败");
		}
		
		//删除32个回调		
		for (j=0; j<32; j++)
		{
			anUserData[j] = j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback (hPlayer,fnPlayerCallback,eEvent,&anUserData[j]), "步骤十失败");
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤5 失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback
//@DESCRIPTION:测试所有参数并且应用场景均符合接口设计要求，CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback函数的执行效果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、所有参数均符合接口设计要求
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERAddPlayerCallback & CSUDIPLAYERDelPlayerCallback参数要求请参考UDI移植指南
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、fnPlayerCallback为fnPlayerCallback2，调用CSUDIPLAYERAddPlayerCallback添加回调函数的三个不同事例(pnUserData为CSUDI_NULL,0x1234以及合任意合法地址值)，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、fnPlayerCallback为fnPlayerCallback2，再次调用CSUDIPLAYERAddPlayerCallback，试图重复添加同样的实例，期望返回CSUDIPLAYER_ERROR_CALLBACK_EXIST
//@EXECUTIONFLOW:4、fnPlayerCallback为fnPlayerCallback，调用CSUDIPLAYERDelPlayerCallback试图删除没有注册的回调函数实例，期望返回CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
//@EXECUTIONFLOW:5、fnPlayerCallback为fnPlayerCallback2，调用CSUDIPLAYERDelPlayerCallback试图删除步骤2中添加的三个回调函数事例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、重复步骤2-步骤5指定次数
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0004(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDIPlayerEventType_E  eEvent=EM_UDIPLAYER_VIDEO_FRAME_COMING;
	
	int  nUserData=NUSERDATA;

	int i=0;

	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	
//创建播放器参数
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;
	
//创建播放器
	hPlayer = PLAYER_openPlayer(uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1 失败");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		for (eEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING; eEvent<EM_UDIPLAYER_MAXEVENTTYPE;eEvent++)
		{
			if (!s_abPlayerCBType[eEvent])
			{
				continue;
			}

			//testing CSUDIPLAYERAddPlayerCallback
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,&nUserData), "步骤2-1 失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,CSUDI_NULL), "步骤2-2 失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,(void *)0x1234), "步骤2-3 失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_EXIST == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,&nUserData), "步骤3-1 失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_EXIST == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,CSUDI_NULL), "步骤3-2 失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_EXIST == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback2,eEvent,(void *)0x1234), "步骤3-3 失败");

			//testing CSUDIPLAYERDelPlayerCallback
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, &nUserData), "步骤4-1 失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, CSUDI_NULL), "步骤4-2 失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback,eEvent, (void *)0x1234), "步骤4-3 失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, (void *)0x4321), "步骤4-4 失败");

			nResult = CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent+1, (void *)0x1234);
			if((EM_UDIPLAYER_MAXEVENTTYPE -1) == eEvent)
			{
				CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_BAD_PARAMETER == nResult), "步骤4-5-1 失败");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST == nResult), "步骤4-5-2 失败");
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, &nUserData), "步骤5-1 失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, CSUDI_NULL), "步骤5-2 失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  ==   CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback2,eEvent, (void *)0x1234), "步骤5-3 失败");
			
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤6 失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求，CSUDIPLAYEROpen&CSUDIPLAYERClose函数的执行效果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、查找支持直播的Demux, Audio, Video设备ID
//@EXECUTIONFLOW:2、psPlayerChnl为CSUDI_NULL，调用CSUDIPLAYEROpen，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、psPlayerChnl->m_nDemux为-1，调用CSUDIPLAYEROpen，期望返回CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:4、psPlayerChnl->m_nDemux为-2，调用CSUDIPLAYEROpen，期望返回CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:5、psPlayerChnl->m_nDemux为一个不支持直播的Demux ID，调用CSUDIPLAYEROpen，期望返回CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:6、psPlayerChnl->m_nVideoDecoder 为-2，调用CSUDIPLAYEROpen，期望返回CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:7、psPlayerChnl->m_nAudioDecoder 为-2，调用CSUDIPLAYEROpen，期望返回CSUDIPLAYER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:8、ePlayerType 为无效值(如:EM_UDIPLAYER_MAXPLAYERTYPE)，调用CSUDIPLAYEROpen，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、phPlayer 为CSUDI_NULL，调用CSUDIPLAYEROpen，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、hPlayer 为CSUDI_NULL，调用CSUDIPLAYERClose，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:11、重复步骤2-步骤10指定次数
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

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&((-1 != nAudioID)||(-1 != nVideoID))),"步骤1失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (CSUDI_NULL,ePlayerType,&hPlayer), "步骤2失败");

		sPlayerChnl.m_nDemux = -1;
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");

		sPlayerChnl.m_nDemux = -2;
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤4失败");
		
		sPlayerChnl.m_nDemux =	PLAYER_searchDemuxID_N(EM_UDI_DEMUX_PLAY);
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤5失败");
		sPlayerChnl.m_nDemux = nDemuxID;
		
		sPlayerChnl.m_nVideoDecoder = -2;
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤6失败");
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		
		sPlayerChnl.m_nAudioDecoder = -2;
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_DEVICE_ID == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_MAXPLAYERTYPE,&hPlayer), "步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,CSUDI_NULL), "步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClose (CSUDI_NULL), "步骤10失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:测试正常情况下，CSUDIPLAYEROpen&CSUDIPLAYERClose的执行情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、能按直播方式正常打开播放器
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、查找支持直播的Demux, Audio, Video设备ID
//@EXECUTIONFLOW:2、调用CSUDIPLAYEROpen，以直播方式打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERClose，关闭步骤2打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复步骤2-步骤3指定次数
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

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&((-1 != nAudioID)||(-1 != nVideoID))),"步骤1失败");

	sPlayerChnl.m_nAudioDecoder = nAudioID;
	sPlayerChnl.m_nVideoDecoder = nVideoID;
	sPlayerChnl.m_nDemux = nDemuxID;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤3失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:测试正常参数情况下，连续多次调用CSUDIPLAYEROpen&CSUDIPLAYERClose的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、能按直播方式正常打开播放器
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、查找支持直播的Demux, Audio, Video设备ID
//@EXECUTIONFLOW:2、调用CSUDIPLAYEROpen，以直播方式打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、参数不变，再次调用CSUDIPLAYEROpen，以直播方式打开播放器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERClose，关闭步骤2打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、参数不变，再次调用CSUDIPLAYERClose，关闭步骤2打开的播放器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤2-步骤5指定次数
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

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&((-1 != nAudioID)||(-1 != nVideoID))),"步骤1失败");

	sPlayerChnl.m_nAudioDecoder = nAudioID;
	sPlayerChnl.m_nVideoDecoder = nVideoID;
	sPlayerChnl.m_nDemux = nDemuxID;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClose (hPlayer), "步骤5失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:测试音频播放器通道的打开与关闭。特别是共用设备时，CSUDIPLAYEROpen&CSUDIPLAYERClose的执行情况
//@PRECONDITION:PLAYER模块被正常初始化，存在音频播放器
//@INPUT:1、
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、分别查找支持直播的Demux,  支持EM_UDI_AUD_STREAM_MPEG2类型的Audio, 支持EM_UDI_VID_STREAM_MPEG2类型Video等设备ID
//@EXECUTIONFLOW:2、分别查找第2个支持直播的Demux,  支持EM_UDI_AUD_STREAM_MPEG2类型的Audio, 支持EM_UDI_VID_STREAM_MPEG2类型Video等设备ID
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen，以直播方式打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、与步骤3打开的播放器共用Audio decoder和Demux。调用CSUDIPLAYEROpen打开播放器，期望返回CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:5、与步骤3打开的播放器共用Audio decoder，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:6、与步骤3打开的播放器共用Demux，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose关闭上一步打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、与步骤3打开的播放器不共用任何设备，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERClose关闭上一步打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIPLAYERClose关闭步骤3所打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、重复步骤3-步骤10指定次数
//@EXECUTIONFLOW:12、恢复现场
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

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nAudioID)),"步骤1失败");

	nDemuxID1=PLAYER_searchDemuxID_WithIndex(EM_UDI_DEMUX_PLAY,2);

	nAudioID1=PLAYER_searchAudioID_WithIndex(EM_UDI_AUD_STREAM_MPEG2,2);
	
	nVideoID1=PLAYER_searchVideoID_WithIndex(EM_UDI_VID_STREAM_MPEG2,2);
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = -1;
		sPlayerChnl.m_nDemux = nDemuxID;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");

		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;  //共用了Audio decoder和Demux
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤4失败");

		if (nDemuxID1 != -1)
		{
			sPlayerChnl.m_nAudioDecoder = nAudioID;
			sPlayerChnl.m_nVideoDecoder = -1;
			sPlayerChnl.m_nDemux = nDemuxID1;   //共用Audio decoder
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤5失败");
		}
		
		if (nAudioID1 != -1)
		{
			sPlayerChnl.m_nDemux = nDemuxID;
			sPlayerChnl.m_nAudioDecoder = nAudioID1;
			sPlayerChnl.m_nVideoDecoder = -1;  //共用demux 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤6失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "步骤7失败");
			hPlayer1 = CSUDI_NULL;
		}

		if (nAudioID1 != -1 && nDemuxID1 != -1)
		{
			sPlayerChnl.m_nAudioDecoder = nAudioID1;
			sPlayerChnl.m_nVideoDecoder = -1;
			sPlayerChnl.m_nDemux = nDemuxID1;   //没有共用设备
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤8失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "步骤9失败");

			hPlayer1 = CSUDI_NULL;
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤10失败");

		hPlayer = CSUDI_NULL;
	
	}
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤12-1失败");		
	}
	
	if (hPlayer1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "步骤12-2失败");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:测试视频播放器通道的打开与关闭。特别是共用设备时，CSUDIPLAYEROpen&CSUDIPLAYERClose的执行情况
//@PRECONDITION:PLAYER模块被正常初始化，存在音频播放器
//@INPUT:1、
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、分别查找支持直播的Demux,  支持EM_UDI_AUD_STREAM_MPEG2类型的Audio, 支持EM_UDI_VID_STREAM_MPEG2类型Video等设备ID
//@EXECUTIONFLOW:2、分别查找第2个支持直播的Demux,  支持EM_UDI_AUD_STREAM_MPEG2类型的Audio, 支持EM_UDI_VID_STREAM_MPEG2类型Video等设备ID
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen，以直播方式打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、与步骤3打开的播放器共用Video  decoder和Demux。调用CSUDIPLAYEROpen打开播放器，期望返回CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:5、与步骤3打开的播放器共用Video decoder，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:6、与步骤3打开的播放器共用Demux，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose关闭上一步打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、与步骤3打开的播放器不共用任何设备，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERClose关闭上一步打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIPLAYERClose关闭步骤3所打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、重复步骤3-步骤10指定次数
//@EXECUTIONFLOW:12、恢复现场
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

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nVideoID)),"步骤1失败");

	nDemuxID1=PLAYER_searchDemuxID_WithIndex(EM_UDI_DEMUX_PLAY,2);

	nAudioID1=PLAYER_searchAudioID_WithIndex(EM_UDI_AUD_STREAM_MPEG2,2);
	
	nVideoID1=PLAYER_searchVideoID_WithIndex(EM_UDI_VID_STREAM_MPEG2,2);
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = -1;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");

		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;  //共用了Video decoder和Demux
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤4失败");

		if (nDemuxID1 != -1)
		{
			sPlayerChnl.m_nAudioDecoder = -1;
			sPlayerChnl.m_nVideoDecoder = nVideoID;
			sPlayerChnl.m_nDemux = nDemuxID1;   //共用Video decoder
			CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤5失败");
		}
		
		if (nVideoID1 != -1)
		{
			sPlayerChnl.m_nDemux = nDemuxID;
			sPlayerChnl.m_nAudioDecoder = -1;
			sPlayerChnl.m_nVideoDecoder = nVideoID1;  //共用demux 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤6失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "步骤7失败");
			hPlayer1 = CSUDI_NULL;
		}


		if (nVideoID1 != -1 && nDemuxID1 != -1)
		{
			sPlayerChnl.m_nAudioDecoder = -1;
			sPlayerChnl.m_nVideoDecoder = nVideoID1;
			sPlayerChnl.m_nDemux = nDemuxID1;   //没有共用设备
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤8失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "步骤9失败");

			hPlayer1 = CSUDI_NULL;
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤10失败");

		hPlayer = CSUDI_NULL;
		
	}
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤12-1失败");		
	}
	
	if (hPlayer1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "步骤12-2失败");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:测试音频/视频播放器通道的打开与关闭。特别是共用设备时，CSUDIPLAYEROpen&CSUDIPLAYERClose的执行情况
//@PRECONDITION:PLAYER模块被正常初始化，存在音频播放器
//@INPUT:1、
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、分别查找支持直播的Demux,  支持EM_UDI_AUD_STREAM_MPEG2类型的Audio, 支持EM_UDI_VID_STREAM_MPEG2类型Video等设备ID
//@EXECUTIONFLOW:2、分别查找第2个支持直播的Demux,  支持EM_UDI_AUD_STREAM_MPEG2类型的Audio, 支持EM_UDI_VID_STREAM_MPEG2类型Video等设备ID
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen，以直播方式打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、与步骤3打开的播放器共用Video  decoder和Demux。调用CSUDIPLAYEROpen打开播放器，期望返回CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:5、与步骤3打开的播放器共用Audio  decoder和Demux。调用CSUDIPLAYEROpen打开播放器，期望返回CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:6、与步骤3打开的播放器共用Video decoder，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:7、与步骤3打开的播放器共用Audio decoder，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:8、与步骤3打开的播放器共用Demux，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERClose关闭上一步打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、与步骤3打开的播放器不共用任何设备，调用CSUDIPLAYEROpen打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用CSUDIPLAYERClose关闭上一步打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose关闭步骤3所打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、重复步骤3-步骤12指定次数
//@EXECUTIONFLOW:14、恢复现场
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

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nAudioID)&&(-1 != nVideoID)),"步骤1失败");

	nDemuxID1=PLAYER_searchDemuxID_WithIndex(EM_UDI_DEMUX_PLAY,2);

	nAudioID1=PLAYER_searchAudioID_WithIndex(EM_UDI_AUD_STREAM_MPEG2,2);
	
	nVideoID1=PLAYER_searchVideoID_WithIndex(EM_UDI_VID_STREAM_MPEG2,2);
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");

		 //共用了Video decoder和Demux
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
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤4失败");

		//共用了Audio decoder和Demux
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
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤5失败");

		if (nDemuxID1 != -1)
		{
			if (nAudioID1 != -1)
			{
				//共用Video decoder
				sPlayerChnl.m_nAudioDecoder = nAudioID1;
				sPlayerChnl.m_nVideoDecoder = nVideoID; 
				sPlayerChnl.m_nDemux = nDemuxID1;  
				CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤6失败");
			}

			if (nVideoID1 != -1)
			{
				//共用Audio decoder
				sPlayerChnl.m_nAudioDecoder = nAudioID;
				sPlayerChnl.m_nVideoDecoder = nVideoID1;
				sPlayerChnl.m_nDemux = nDemuxID1;   
				CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤7失败");
			}
		}
		
		if ((nVideoID1 != -1)&&(nAudioID1 != -1))
		{
			//共用demux 
			sPlayerChnl.m_nDemux = nDemuxID;
			sPlayerChnl.m_nAudioDecoder = nAudioID1;
			sPlayerChnl.m_nVideoDecoder = nVideoID1;  
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤8失败");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "步骤9失败");
			hPlayer1 = CSUDI_NULL;
		}
		

		if (((nVideoID1 != -1)&&(nAudioID1 != -1)) && (nDemuxID1 != -1))
		{
			//没有共用设备
			sPlayerChnl.m_nAudioDecoder = nAudioID1;
			sPlayerChnl.m_nVideoDecoder = nVideoID1;
			sPlayerChnl.m_nDemux = nDemuxID1;   
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer1), "步骤10失败");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "步骤11失败");
			hPlayer1 = CSUDI_NULL;
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤12失败");

		hPlayer = CSUDI_NULL;
		
	}
	
	CSTK_FATAL_POINT;

	//恢复现场
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤14-1失败");		
	}
	if (hPlayer1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer1), "步骤14-2失败");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:测试是否支持H264播放器
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、能按直播方式正常打开播放器
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、查找支持直播的Demux, 支持 EM_UDI_VID_STREAM_H264 类型的视频解码设备nVideoID
//@EXECUTIONFLOW:2、调用CSUDIPLAYEROpen，以直播方式打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERClose，关闭步骤2打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复步骤2-步骤3指定次数
//@EXECUTIONFLOW:5、恢复现场
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0007(void)
{
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;

	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int nDemuxID,nAudioID,nVideoID;
	
	CSUDIPlayerChnl_S  sPlayerChnl;

	int i = 0;

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("系统支持H264解码\r\n");
		CSTCPrint("The chip surpports decode of H264format\r\n");
	}
	else
	{
		CSTCPrint("系统不支持H264解码\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	
	nDemuxID=PLAYER_searchDemuxID_Y(EM_UDI_DEMUX_PLAY);

	nAudioID=PLAYER_searchAudioID_Y(EM_UDI_AUD_STREAM_MPEG2);
	
	nVideoID=PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264);

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nVideoID)),"步骤1失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = -1;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤2-1失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤3-1失败");

		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤2-2失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤3-2失败");
		hPlayer = CSUDI_NULL;
		
	}
	
	CSTK_FATAL_POINT;
	
	//恢复现场
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤5失败");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen&CSUDIPLAYERClose
//@DESCRIPTION:测试是否支持EM_UDI_AUD_STREAM_AC3播放器
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、能按直播方式正常打开播放器
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、查找支持直播的Demux, 支持 EM_UDI_AUD_STREAM_AC3 类型的音频解码设备nVideoID
//@EXECUTIONFLOW:2、调用CSUDIPLAYEROpen，以直播方式打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERClose，关闭步骤2打开的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复步骤2-步骤3指定次数
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

	CSTK_ASSERT_TRUE_FATAL(((-1 != nDemuxID)&&(-1 != nAudioID)),"步骤1失败");


	for (i=0;i<REPEAT_TIMES;i++)
	{
		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = -1;
		sPlayerChnl.m_nDemux = nDemuxID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤2-1失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤3-1失败");

		sPlayerChnl.m_nAudioDecoder = nAudioID;
		sPlayerChnl.m_nVideoDecoder = nVideoID;
		sPlayerChnl.m_nDemux = nDemuxID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤2-2失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤3-2失败");

		hPlayer = CSUDI_NULL;
				
	}
	
	CSTK_FATAL_POINT;
	
	//恢复现场
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤14-1失败");		
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:测试参数或应用场景在不符合接口设计要求的情况下，调用CSUDIPLAYERSetStream函数的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器
//@EXECUTIONFLOW:2、测试hPlayer为CSUDI_NULL的情况下，调用CSUDIPLAYERSetStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、测试psStreamInfo为CSUDI_NULL的情况下，调用CSUDIPLAYERSetStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、测试psStreamInfo为CSUDI_NULL并且nStreamCnt为0的情况下，调用CSUDIPLAYERSetStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、测试nStreamCnt为0的情况下，调用CSUDIPLAYERSetStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、测试nStreamCnt为-1的情况下，调用CSUDIPLAYERSetStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、测试hPlayer为CSUDI_NULL的情况下，调用CSUDIPLAYERGetStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、测试psStreamInfo为CSUDI_NULL的情况下，调用CSUDIPLAYERGetStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、测试pnStreamCnt为CSUDI_NULL的情况下，调用CSUDIPLAYERGetStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤2-步骤9 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0001(void)
{
	int nStreamCnt = 1;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE == CSUDIPLAYERSetStream (CSUDI_NULL,asStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,CSUDI_NULL,nStreamCnt,CSUDI_NULL), "步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,CSUDI_NULL,0,CSUDI_NULL), "步骤4失败");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,0,CSUDI_NULL), "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_BAD_PARAMETER == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,-1,CSUDI_NULL), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetStream (CSUDI_NULL,asStreamInfo,&nStreamCnt), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetStream (hPlayer,CSUDI_NULL,&nStreamCnt), "步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetStream (hPlayer,asStreamInfo,CSUDI_NULL), "步骤9失败");

	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
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
	{2,{0,		EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}}, 	"直播状态下，PID不能为0"},
	{3,{-1,		EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}}, 	"直播状态下，PID不能为-1"},
	{4,{0x1FFF,EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}}, 	"直播状态下，PID不能为0x1FFF"},
	{5,{0x100,	EM_UDI_CONTENT_PCR+1,		{EM_UDI_VID_STREAM_MPEG2}}, 	"直播状态下，CSUDIContentType_E 类型非法"},
	{6,{0x100,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAMTYPE_NUM}},	"直播状态下，视频类型非法"},
	{7,{0x100,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_UNKNOWN}},	"直播状态下，视频类型非法"},
	{8,{0x101,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAMTYPE_NUM}},	"直播状态下，音频类型非法"},
	{9,{0x101,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_UNKNOWN}},	"直播状态下，音频类型非法"},	
};

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:测试流信息参数(CSUDIStreamInfo_S)或其参数组合不符合接口设计要求的情况下，调用CSUDIPLAYERSetStream函数的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器
//@EXECUTIONFLOW:2、测试在直播状态下，设置PID为0的情况
//@EXECUTIONFLOW:3、测试在直播状态下，设置PID为-1的情况
//@EXECUTIONFLOW:4、测试在直播状态下，设置PID为0x1FFF的情况
//@EXECUTIONFLOW:5、测试在直播状态下，设置CSUDIContentType_E 类型非法的情况
//@EXECUTIONFLOW:6、测试在直播状态下，视频类型为EM_UDI_VID_STREAMTYPE_NUM的情况
//@EXECUTIONFLOW:7、测试在直播状态下，视频类型为EM_UDI_VID_STREAM_UNKNOWN的情况
//@EXECUTIONFLOW:8、测试在直播状态下，音频类型为EM_UDI_AUD_STREAMTYPE_NUM的情况
//@EXECUTIONFLOW:9、测试在直播状态下，音频类型为EM_UDI_AUD_STREAM_UNKNOWN的情况
//@EXECUTIONFLOW:10、重复步骤2-步骤9  指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0,j=0, count = 0;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	Testcase_StreamInfo_T *pstreaminfo = CSUDI_NULL;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));

	count = sizeof(s_SetGetStream_0002_streamInfo)/sizeof(Testcase_StreamInfo_T);
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");

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
				CSTCPrint("测试用例CSTC_LIVE_TEST_IT_SetGetStream_0002 第%d步 \"%s\" 测试失败",pstreaminfo->testID, pstreaminfo->m_pcDesc);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=udiRe, "以不符合设计要求的参数调用CSUDIPLAYERSetStream，期望返回非CSUDI_SUCCESS");	
		}
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

static Testcase_StreamInfo_T s_SetGetStream_0003_streamInfo[]=
{
	{1,{0x101,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 CSUDI_NULL},
	{1,{0x102,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 "一个播放器同时设置了两个不同的音频PID，设置失败，返回非CSUDI_SUCCESS"},
	{2,{0x101,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 CSUDI_NULL},
	{2,{0x103,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 "一个播放器同时设置了两个不同的视频PID，设置失败，返回非CSUDI_SUCCESS"},
	{3,{0x101,	EM_UDI_CONTENT_PCR,		{0}},	 CSUDI_NULL},
	{3,{0x104,	EM_UDI_CONTENT_PCR,		{0}},	 "一个播放器同时设置了两个不同的PCR PID，设置失败，返回非CSUDI_SUCCESS"},
	{4,{0x102,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 CSUDI_NULL},
	{4,{0x102,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 "一个播放器同时设置了两个相同的音频PID，设置失败，返回非CSUDI_SUCCESS"},
	{5,{0x103,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 CSUDI_NULL},
	{5,{0x103,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 "一个播放器同时设置了两个相同的视频PID，设置失败，返回非CSUDI_SUCCESS"},
	{6,{0x104,	EM_UDI_CONTENT_PCR,		{0}},	 CSUDI_NULL},
	{6,{0x104,	EM_UDI_CONTENT_PCR,		{0}},	 "一个播放器同时设置了两个相同的PCR PID，设置失败，返回非CSUDI_SUCCESS"},
	{7,{0x105,	EM_UDI_CONTENT_VIDEO,		{EM_UDI_VID_STREAM_MPEG2}},	 CSUDI_NULL},
	{7,{0x105,	EM_UDI_CONTENT_AUDIO,		{EM_UDI_AUD_STREAM_MPEG2}},	 "一个播放器同时设置了两个相同PID的流信息，设置失败，返回非CSUDI_SUCCESS"},

};

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:测试同时存在两个音频、视频或PCR流信息时，调用CSUDIPLAYERSetStream函数的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器
//@EXECUTIONFLOW:2、同时设置了两个不同的音频PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、同时设置了两个不同的视频PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、同时设置了两个不同的PCR PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、同时设置了两个相同的音频PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、同时设置了两个相同的视频PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、同时设置了两个相同的PCR PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、同时设置了两个相同的PID的音频和视频流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复步骤2-步骤8 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0,j=0, count = 0;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	Testcase_StreamInfo_T *pstreaminfo = CSUDI_NULL;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));

	count = sizeof(s_SetGetStream_0003_streamInfo)/(2*sizeof(Testcase_StreamInfo_T));

	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");

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
				CSTCPrint("测试用例CSTC_LIVE_TEST_IT_SetGetStream_0003 第%d步 \"%s\" 测试失败",pstreaminfo->testID, pstreaminfo->m_pcDesc);
			}
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=udiRe, "以不符合设计要求的参数调用CSUDIPLAYERSetStream，期望返回非CSUDI_SUCCESS");	
		}
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:对一个视频播放器，能通过CSUDIPLAYERGetStream获取到唯一的视频流信息；通过CSUDIPLAYERSetStream设置音频流信息时，期望返回非CSUDI_SUCCESS
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个纯视频DVB直播播放器
//@EXECUTIONFLOW:2、调用CSUDIPLAYERGetStream，获取流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、判断上一步获取的流信息有并且只有一个视频流信息
//@EXECUTIONFLOW:4、对于步骤1打开的播放器，设置视频流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤2 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0004(void)
{
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERGetStream (hPlayer,asStreamInfo,&nStreamCnt), "步骤2失败");

	CSTCPrint("asStreamInfo[0].m_eContentType=%d\n",asStreamInfo[0].m_eContentType);
	CSTCPrint("nStreamCnt=%d\n",nStreamCnt);
	
	CSTK_ASSERT_TRUE_FATAL((nStreamCnt <= 1),"步骤3失败");

	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_nPid  = 0x101;
	asStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 0x102;
	asStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;		

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "步骤4-1失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[1],1,CSUDI_NULL), "步骤4-2失败");
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:对一个音频播放器，能通过CSUDIPLAYERGetStream获取到唯一的音频流信息；通过CSUDIPLAYERSetStream设置视频流信息时，期望返回非CSUDI_SUCCESS
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个纯音频DVB直播播放器
//@EXECUTIONFLOW:2、调用CSUDIPLAYERGetStream，获取流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、判断上一步获取的流信息有并且只有一个音频流信息
//@EXECUTIONFLOW:4、对于步骤1打开的播放器，设置视频流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤2 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0005(void)
{
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERGetStream (hPlayer,asStreamInfo,&nStreamCnt), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL((nStreamCnt <= 1),"步骤3失败");

	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_nPid  = 0x101;
	asStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 0x102;
	asStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;		

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "步骤4-1失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,1,CSUDI_NULL), "步骤4-2失败");
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION::设置播放器不支持的视频流类型时，期望返回非CSUDI_SUCCESS
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个不支持某种视频解码器的DVB直播播放器
//@EXECUTIONFLOW:2、对于步骤1打开的播放器，设置视频流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、重复步骤2 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0006(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
	CSUDIPlayerChnl_S  psPlayerChnl;
	int nAudioID, nVideoID, nDemuxID = -1;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_Y(eAudStreamType);
	nVideoID=PLAYER_searchVideoID_N((unsigned int *)&eVidStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nDemuxID, "没有支持直播的demux");
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioID, "没有支持MPEG2类型的音频解码器");
	if(nVideoID == -1)
	{
		CSTCPrint("找不到不支持的类型,直接测试成功\n");
		return CSUDI_TRUE;
	}

//创建播放器参数
	psPlayerChnl.m_nDemux=nDemuxID;
	psPlayerChnl.m_nAudioDecoder=nAudioID;
	psPlayerChnl.m_nVideoDecoder=nVideoID;

//设置节目信息参数
	asStreamInfo[0].m_nPid=128;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=eVidStreamType;

	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 129;
	asStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;		

	CSTCPrint("use video index:%d type:%d\n",nVideoID,eVidStreamType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYEROpen (&psPlayerChnl,ePlayerType,&hPlayer), "步骤1失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "步骤2-1失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,1,CSUDI_NULL), "步骤2-2失败");

	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION::设置播放器不支持的音频流类型，期望返回非CSUDI_SUCCESS
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个不支持某种音频解码器的DVB直播播放器
//@EXECUTIONFLOW:2、对于步骤1打开的播放器，设置音频流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、重复步骤2 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0007(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
	CSUDIPlayerChnl_S  psPlayerChnl;
	int nAudioID, nVideoID, nDemuxID = -1;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_N((unsigned int *)&eAudStreamType);
	nVideoID=PLAYER_searchVideoID_Y(eVidStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nDemuxID, "没有支持直播的demux");
	CSTK_ASSERT_TRUE_FATAL(-1 != nVideoID, "没有支持MPEG2类型的视频解码器");
	if(nAudioID == -1)
	{
		CSTCPrint("找不到不支持的类型,直接测试成功\n");
		return CSUDI_TRUE;
	}

//创建播放器参数
	psPlayerChnl.m_nDemux=nDemuxID;
	psPlayerChnl.m_nAudioDecoder=nAudioID;
	psPlayerChnl.m_nVideoDecoder=nVideoID;

//设置节目信息参数
	asStreamInfo[0].m_nPid=128;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 129;
	asStreamInfo[1].m_uStreamType.m_eVideoType = eAudStreamType;		

	CSTCPrint("use video index:%d type:%d\n",nVideoID,eVidStreamType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYEROpen (&psPlayerChnl,ePlayerType,&hPlayer), "步骤1失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],2,CSUDI_NULL), "步骤2-1失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[1],1,CSUDI_NULL), "步骤2-2失败");

	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

static CSUDI_BOOL verify_streaminfo(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S asStreamInfo[],int nStreamCnt)
{
	int nCnt = 10;
	int i = 0,j =0;
	
	CSUDIStreamInfo_S sStreamInfo[10];
	
	CSUDI_BOOL bRe = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERGetStream (hPlayer,sStreamInfo,&nCnt), "获取流信息失败");

	CSTK_ASSERT_TRUE_FATAL((nStreamCnt == nCnt), "流信息个数不对");

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
//@DESCRIPTION:测试在纯视频/音频播放器start/stop之后调用CSUDIPLAYERSetStream&CSUDIPLAYERGetStream的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、hPlayer有效
//@INPUT:2、psStreamInfo->m_nPid= 352,
//@INPUT:	  psStreamInfo->m_eContentTpye=EM_UDI_CONTENT_VIDEO  ,
//@INPUT:	  psStreamInfo->m_uStreamType->m_eVideoType=EM_UDI_VID_STREAM_MPEG2 
//@INPUT:3、nStreamCnt=1
//@INPUT:4、psPlaybackParam=CSUDI_NULL
//@EXPECTATION:start之后，stop之前返回非 CSUDI_SUCCESS，stop之后，start之前返回非 CSUDI_SUCCESS,Open之后，Close之前均可以调用CSUDIPLAYERGetStream
//@REMARK: SetStream只能用在播放器刚被创建的时候与播放器被停止之后
//@EXECUTIONFLOW:1、打开一个纯视频的DVB直播播放器
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，设置视频流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、通过CSUDIPLAYERGetStream，判断获取的流信息与最近设置的流信息是否一致
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERSetStream，设置视频流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、通过CSUDIPLAYERGetStream，判断获取的流信息与最近设置的流信息是否一致
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、通过CSUDIPLAYERGetStream，判断获取的流信息与最近设置的流信息是否一致
//@EXECUTIONFLOW:9、重复步骤2-步骤8 指定次数
//@EXECUTIONFLOW:10、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0008(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");

	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_nPid  = 352;
	asStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERSetStream (hPlayer,asStreamInfo,1,CSUDI_NULL), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,1), "步骤3失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStart (hPlayer), "步骤4失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,1,CSUDI_NULL), "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,1), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStop(hPlayer), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,1), "步骤8失败");
		
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetStream&CSUDIPLAYERGetStream
//@DESCRIPTION:测试在播放器start/stop之后调用CSUDIPLAYERSetStream&CSUDIPLAYERGetStream的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、hPlayer有效
//@INPUT:2、psStreamInfo，音视频流信息
//@INPUT:3、nStreamCnt=2
//@INPUT:4、psPlaybackParam=CSUDI_NULL
//@EXPECTATION:start之后，stop之前返回非 CSUDI_SUCCESS，stop之后，start之前返回非 CSUDI_SUCCESS,Open之后，Close之前均可以调用CSUDIPLAYERGetStream
//@REMARK: SetStream只能用在播放器刚被创建的时候与播放器被停止之后
//@EXECUTIONFLOW:1、打开一个包含音视频的DVB直播播放器
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，设置视频流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、通过CSUDIPLAYERGetStream，判断获取的流信息与最近设置的流信息是否一致
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERSetStream，设置视频流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、通过CSUDIPLAYERGetStream，判断获取的流信息与最近设置的流信息是否一致
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、通过CSUDIPLAYERGetStream，判断获取的流信息与最近设置的流信息是否一致
//@EXECUTIONFLOW:9、重复步骤2-步骤8 指定次数
//@EXECUTIONFLOW:10、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0009(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_nPid  = 352;
	asStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 353;
	asStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,2), "步骤3失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStart (hPlayer), "步骤4失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERSetStream (hPlayer,asStreamInfo,2,CSUDI_NULL), "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,2), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStop(hPlayer), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==verify_streaminfo (hPlayer,asStreamInfo,2), "步骤8失败");
		
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPlaybackParam    
//@DESCRIPTION:测试直播模式下，CSUDIPLAYERGetPlaybackParam函数的执行结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、
//@INPUT:2、
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、hPlayer为CSUDI_NULL，调用CSUDIPLAYERGetPlaybackParam，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、psPlaybackParam为CSUDI_NULL，调用CSUDIPLAYERGetPlaybackParam，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、hPlayer为步骤1得到的句柄，psStreamInfo为有效地址，调用CSUDIPLAYERGetPlaybackParam,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤2-步骤4指定次数
//@EXECUTIONFLOW:6、调用CSUDIPLAYERClose 关闭播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPlaybackParam_0001(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIPlaybackParam_S  sPlaybackParam;
	int i=0;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;
	
//创建播放器
	hPlayer= PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetPlaybackParam (CSUDI_NULL,&sPlaybackParam), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetPlaybackParam (hPlayer,CSUDI_NULL), "步骤3失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERGetPlaybackParam (hPlayer,&sPlaybackParam), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤6失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream   
//@DESCRIPTION:测试在参数非法的情况下，接口函数CSUDIPLAYERModifyStream的执行情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:参数非法的情况下，返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，设置流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，启动此直播播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、hPlayer为CSUDI_NULL的情况下，调用CSUDIPLAYERModifyStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、psStreamInfo为CSUDI_NULL的情况下，调用CSUDIPLAYERModifyStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、nStreamCnt为0的情况下，调用CSUDIPLAYERModifyStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、nStreamCnt为-1的情况下，调用CSUDIPLAYERModifyStream的情况，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、重复步骤4-步骤7 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0001(void)
{
  	CSUDIStreamInfo_S  sStreamInfo[10];
	int nStreamCnt;
	
	CSUDI_HANDLE   hPlayer= CSUDI_NULL;
	
	int i=0;
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;

//设置节目信息
	sStreamInfo[0].m_nPid=352;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	sStreamInfo[1].m_nPid=353;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;	

	nStreamCnt=2;
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo[0],nStreamCnt,CSUDI_NULL), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart (hPlayer), "步骤3失败");
	
	sStreamInfo[0].m_nPid=354;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	sStreamInfo[1].m_nPid=356;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;	
	
	for (i=0;i<REPEAT_TIMES;i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (CSUDI_NULL,&sStreamInfo[0],nStreamCnt), "步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,CSUDI_NULL,2), "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,&sStreamInfo[0],0), "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,&sStreamInfo[0],-1), "步骤7失败");
	}
	
	CSTK_FATAL_POINT;
	
	if (CSUDI_NULL  != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream
//@DESCRIPTION:测试流信息参数(CSUDIStreamInfo_S)或其参数组合不符合接口设计要求的情况下，调用CSUDIPLAYERModifyStream函数的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream设置流信息
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart启动播放器
//@EXECUTIONFLOW:4、测试在直播状态下，调用CSUDIPLAYERModifyStream修改PID为0的情况
//@EXECUTIONFLOW:5、测试在直播状态下，调用CSUDIPLAYERModifyStream修改PID为-1的情况
//@EXECUTIONFLOW:6、测试在直播状态下，调用CSUDIPLAYERModifyStream修改PID为0x1FFF的情况
//@EXECUTIONFLOW:7、测试在直播状态下，调用CSUDIPLAYERModifyStream修改CSUDIContentType_E 类型非法的情况
//@EXECUTIONFLOW:8、测试在直播状态下，调用CSUDIPLAYERModifyStream修改视频类型为EM_UDI_VID_STREAMTYPE_NUM的情况
//@EXECUTIONFLOW:9、测试在直播状态下，调用CSUDIPLAYERModifyStream修改视频类型为EM_UDI_VID_STREAM_UNKNOWN的情况
//@EXECUTIONFLOW:10、测试在直播状态下，调用CSUDIPLAYERModifyStream修改音频类型为EM_UDI_AUD_STREAMTYPE_NUM的情况
//@EXECUTIONFLOW:11、测试在直播状态下，调用CSUDIPLAYERModifyStream修改音频类型为EM_UDI_AUD_STREAM_UNKNOWN的情况
//@EXECUTIONFLOW:12、重复步骤4-步骤11  指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0002(void)
{
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0,j=0, count = 0;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	Testcase_StreamInfo_T *pstreaminfo = CSUDI_NULL;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
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
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],nStreamCnt,CSUDI_NULL), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hPlayer), "步骤3失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		for (j=0;j<count;j++)
		{
			pstreaminfo = &s_SetGetStream_0002_streamInfo[j];				
			asStreamInfo[0] = pstreaminfo->m_sStreamInfo;
			
			udiRe  = CSUDIPLAYERModifyStream (hPlayer,asStreamInfo,1);

			if (udiRe == CSUDI_SUCCESS)
			{
				CSTCPrint("测试用例%s 第%d步 \"%s\" 测试失败",__FUNCTION__,pstreaminfo->testID+3, pstreaminfo->m_pcDesc);
			}
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=udiRe, "以不符合设计要求的参数调用CSUDIPLAYERModifyStream，期望返回非CSUDI_SUCCESS");	
		}
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop( hPlayer), "停止播放器失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "关闭播放器失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream
//@DESCRIPTION:测试同时存在两个音频、视频或PCR流信息时，调用CSUDIPLAYERModifyStream函数的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、参数组合不符合设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream设置流信息
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart启动播放器
//@EXECUTIONFLOW:4、同时设置了两个不同的音频PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、同时设置了两个不同的视频PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、同时设置了两个不同的PCR PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、同时设置了两个相同的音频PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、同时设置了两个相同的视频PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、同时设置了两个相同的PCR PID，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、同时设置了两个相同的PID的音频和视频流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:11、重复步骤4-步骤10 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0003(void)
{
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0,j=0, count = 0;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	Testcase_StreamInfo_T *pstreaminfo = CSUDI_NULL;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
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
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],nStreamCnt,CSUDI_NULL), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hPlayer), "步骤3失败");

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
				CSTCPrint("测试用例%s 第%d步 \"%s\" 测试失败",__FUNCTION__,pstreaminfo->testID+3, pstreaminfo->m_pcDesc);
			}
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=udiRe, "以不符合设计要求的参数调用CSUDIPLAYERModifyStream，期望返回非CSUDI_SUCCESS");	
		}
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop( hPlayer), "停止播放器失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "关闭播放器失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream   
//@DESCRIPTION:测试在参数合法但应用场景不符合设计要求的情况下，接口函数CSUDIPLAYERModifyStream的执行情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:参数非法的情况下，返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器
//@EXECUTIONFLOW:2、启动播放器之前，调用CSUDIPLAYERModifyStream更新流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、启动播放器之前，调用CSUDIPLAYERSetStream设置流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、设置流信息之后，启动播放器之前，调用CSUDIPLAYERModifyStream更新流信息，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，启动此直播播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、启动播放器之后，调用CSUDIPLAYERModifyStream更新流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStop，停止此直播播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、重复步骤2-步骤7 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0004(void)
{
  	CSUDIStreamInfo_S  sStreamInfo[10];
	int nStreamCnt;
	
	CSUDI_HANDLE   hPlayer= CSUDI_NULL;
	
	int i=0;
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE;

//设置节目信息
	sStreamInfo[0].m_nPid=352;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;
	
	sStreamInfo[1].m_nPid=353;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;	

	nStreamCnt=2;
	
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,sStreamInfo,nStreamCnt), "步骤2失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo[0],nStreamCnt,CSUDI_NULL), "步骤3失败");
		
		sStreamInfo[0].m_nPid=354;
		sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
		sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;		
		sStreamInfo[1].m_nPid=356;
		sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
		sStreamInfo[1].m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERModifyStream (hPlayer,sStreamInfo,nStreamCnt), "步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart (hPlayer), "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERModifyStream (hPlayer,sStreamInfo,nStreamCnt), "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤7失败");
	}
	
	CSTK_FATAL_POINT;
	
	if (CSUDI_NULL  != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream
//@DESCRIPTION:通过CSUDIPLAYERModifyStream修改播放器到不支持的音频流类型，期望返回非CSUDI_SUCCESS
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，仅设置视频流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERModifyStream,修改音频流信息到解码器不支持的流类型，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStop停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、重复步骤2-步骤5 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0005(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
	CSUDIPlayerChnl_S  psPlayerChnl;
	int nAudioID, nVideoID, nDemuxID = -1;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_N((unsigned int *)(&eAudStreamType));
	nVideoID=PLAYER_searchVideoID_Y(eVidStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nDemuxID, "没有支持直播的demux");
	CSTK_ASSERT_TRUE_FATAL(-1 != nVideoID, "没有支持MPEG2类型的视频解码器");
	if(nAudioID == -1)
	{
		CSTCPrint("找不到不支持的类型,直接测试成功\n");
		return CSUDI_TRUE;
	}

//创建播放器参数
	psPlayerChnl.m_nDemux=nDemuxID;
	psPlayerChnl.m_nAudioDecoder=nAudioID;
	psPlayerChnl.m_nVideoDecoder=nVideoID;

//设置节目信息参数
	asStreamInfo[0].m_nPid=128;
	asStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[1].m_nPid  = 129;
	asStreamInfo[1].m_uStreamType.m_eVideoType = eAudStreamType;		

	CSTCPrint("use video index:%d type:%d\n",nVideoID,eVidStreamType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYEROpen (&psPlayerChnl,ePlayerType,&hPlayer), "步骤1失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],1,CSUDI_NULL), "步骤2失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStart( hPlayer), "步骤3失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERModifyStream (hPlayer,&asStreamInfo[0],2), "步骤4失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop( hPlayer), "步骤5失败");		
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "关闭播放器失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERModifyStream
//@DESCRIPTION:通过CSUDIPLAYERModifyStream修改播放器到不支持的视频流类型，期望返回非CSUDI_SUCCESS
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个DVB直播播放器
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，仅设置音频流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERModifyStream,修改视频流信息到解码器不支持的流类型，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStop停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、重复步骤2-步骤5 指定次数
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0006(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[10];
	int  i= 0;
	
	CSUDIPlayerChnl_S  psPlayerChnl;
	int nAudioID, nVideoID, nDemuxID = -1;
	
//设置播放器信息	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;         //查找具有播放功能的demx
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	memset(asStreamInfo,0,sizeof(asStreamInfo));
	
	nDemuxID=PLAYER_searchDemuxID_Y(uDemuxType);
	nAudioID=PLAYER_searchAudioID_Y(eAudStreamType);
	nVideoID=PLAYER_searchVideoID_N((unsigned int *)(&eVidStreamType));
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nDemuxID, "没有支持直播的demux");
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioID, "没有支持MPEG2类型的视频解码器");
	if(nVideoID == -1)
	{
		CSTCPrint("找不到不支持的类型,直接测试成功\n");
		return CSUDI_TRUE;
	}

//创建播放器参数
	psPlayerChnl.m_nDemux=nDemuxID;
	psPlayerChnl.m_nAudioDecoder=nAudioID;
	psPlayerChnl.m_nVideoDecoder=nVideoID;

//设置节目信息参数

	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_nPid  = 129;
	asStreamInfo[0].m_uStreamType.m_eVideoType = eAudStreamType;		
	
	asStreamInfo[1].m_nPid=128;
	asStreamInfo[1].m_eContentType=EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType=eVidStreamType;

	CSTCPrint("use video index:%d type:%d\n",nVideoID,eVidStreamType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYEROpen (&psPlayerChnl,ePlayerType,&hPlayer), "步骤1失败");


	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],1,CSUDI_NULL), "步骤2失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIPLAYERStart( hPlayer), "步骤3失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=CSUDIPLAYERModifyStream (hPlayer,&asStreamInfo[0],2), "步骤4失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop( hPlayer), "步骤5失败");		
	}
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "关闭播放器失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   & CSUDIPLAYERStop
//@DESCRIPTION:测试播放器指针非法的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、hPlayer=CSUDI_NULL
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIPLAYERStart，使hPlayer=CSUDI_NULL，期望返回CSUDIPLAYER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:2、调用CSUDIPLAYERStop，使hPlayer=CSUDI_NULL，期望返回CSUDIPLAYER_ERROR_INVALID_HANDLE
CSUDI_BOOL CSTC_LIVE_TEST_IT_StartStop_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERStart(CSUDI_NULL), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERStop(CSUDI_NULL), "步骤2失败");
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   & CSUDIPLAYERStop
//@DESCRIPTION:测试正常启动/停止一个播放器的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、hPlayer有效
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream,设置流属性信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤3-步骤4指定次数
//@EXECUTIONFLOW:6、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_StartStop_0002(void)
{
	CSUDIStreamInfo_S  asStreamInfo[10];
	int nStreamCnt,i;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

//设置节目信息
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

//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤3失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERStart   
//@DESCRIPTION:在应用场景不符合设计要求的情况下，调用CSUDIPLAYERStart/ CSUDIPLAYERStop,期望返回非CSUDI_SUCCESS
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、hPlayer有效
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_STATUS 
//@REMARK:CSUDIPLAYERStart只能在设置码流之后与停止播放器之后调用，CSUDIPLAYERStop只能在启动播放器之后，停止播放器之前才能调用
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、CSUDIPLAYEROpen之后立即调用CSUDIPLAYERStop，停止播放器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、CSUDIPLAYEROpen之后立即调用CSUDIPLAYERStart，启动播放器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、CSUDIPLAYEROpen之后调用CSUDIPLAYERSetStream,设置流属性信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、CSUDIPLAYERSetStream之后、CSUDIPLAYERStart之前，调用CSUDIPLAYERStop，停止播放器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、再次调用CSUDIPLAYERStart，启动播放器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、再次调用CSUDIPLAYERStop，停止播放器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤5-步骤9指定次数
//@EXECUTIONFLOW:11、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_StartStop_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int nStreamCnt = 0, i=0;
	
	CSUDIStreamInfo_S  asStreamInfo[10];
//创建播放器参数
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
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
//创建播放器
	hPlayer = PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer), "步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&asStreamInfo[0],nStreamCnt,CSUDI_NULL), "步骤4失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤8失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "步骤9失败");
		
	}
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:测试播放器句柄为CSUDI_NULL时，CSUDIPLAYERPause & CSUDIPLAYERResume的调用情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、hPlayer=CSUDI_NULL
//@EXPECTATION:CSUDIPLAYER_ERROR_INVALID_HANDLE 
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIPLAYERPause，使hPlayer=CSUDI_NULL，期望返回CSUDIPLAYER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:2、调用CSUDIPLAYERResume，使hPlayer=CSUDI_NULL，期望返回CSUDIPLAYER_ERROR_INVALID_HANDLE 
CSUDI_BOOL CSTC_LIVE_TEST_IT_PauseResume_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERPause(CSUDI_NULL), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERResume(CSUDI_NULL), "步骤2失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERPause & CSUDIPLAYERResume
//@DESCRIPTION:测试应用场景不符合设计要求时，CSUDIPLAYERPause & CSUDIPLAYERResume函数的执行情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:应用场景不符合设计要求时应该返回非CSUDI_SUCCESS
//@REMARK:直播播放器要求支持CSUDIPLAYERPause & CSUDIPLAYERResume
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、Open播放器后立即调用CSUDIPLAYERPause，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、Open播放器后立即调用CSUDIPLAYERResume，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、Open播放器后调用CSUDIPLAYERSetStream设置流属性信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、Start播放之前，调用CSUDIPLAYERPause，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、Start播放之前，调用CSUDIPLAYERResume，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart，Start播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、Start之后、Pause播放之前，调用CSUDIPLAYERResume，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERPause暂停播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStart，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:11、调用CSUDIPLAYERSetStream，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:12、调用CSUDIPLAYERModifyStream，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:13、再次调用CSUDIPLAYERPause，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:14、调用CSUDIPLAYERResume，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:15、再次调用CSUDIPLAYERResume，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:16、调用CSUDIPLAYERPause，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、重复步骤5-步骤17指定次数
//@EXECUTIONFLOW:19、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_PauseResume_0002(void)
{
	CSUDIStreamInfo_S  asStreamInfo[10];
	
	int nStreamCnt,i;
	
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

//创建播放器参数

	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;
//设置节目信息
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
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause (hPlayer), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "步骤4失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause (hPlayer), "步骤5失败");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤7失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "步骤8失败");

		udiRe = CSUDIPLAYERPause(hPlayer);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == udiRe),"步骤9失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERStart(hPlayer), "步骤10失败");
					
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERModifyStream (hPlayer,asStreamInfo,nStreamCnt), "步骤12失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERPause(hPlayer), "步骤13失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤14失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERResume(hPlayer), "步骤15失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤16失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤17失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSetSpeed       
//@DESCRIPTION:测试直播情况下，CSUDIPLAYERSetSpeed函数的执行情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:
//@REMARK:
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、hPlayer为CSUDI_NULL，调用 CSUDIPLAYERSetSpeed，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、eSpeed为无效值，调用 CSUDIPLAYERSetSpeed，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、参数正确的情况下，Start播放器之前调用CSUDIPLAYERSetSpeed，期望返回CSUDI_SUCCESS或CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStart，Start播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、参数正确的情况下，Start之后调用CSUDIPLAYERSetSpeed，期望返回CSUDI_SUCCESS或CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复步骤3-步骤8指定次数
//@EXECUTIONFLOW:10、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetSpeed_0001(void)
{
	CSUDIPlayerSpeed_E  eSpeed=EM_UDIPLAYER_SPEED_NORMAL;
	CSUDIStreamInfo_S  sStreamInfo;
	int nStreamCnt, i;
	
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code errorCode = CSUDI_SUCCESS;
	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E ePlayerType=EM_UDIPLAYER_LIVE ;

//设置节目信息
	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream(hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤2失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_INVALID_HANDLE  == CSUDIPLAYERSetSpeed(CSUDI_NULL,eSpeed), "步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSetSpeed(CSUDI_NULL,EM_UDIPLAYER_SPEED_FASTFORWARD_32+10), "步骤4失败");

		for (eSpeed = EM_UDIPLAYER_SPEED_NORMAL;eSpeed <=EM_UDIPLAYER_SPEED_FASTFORWARD_32;eSpeed++)
		{
			errorCode = CSUDIPLAYERSetSpeed(hPlayer,eSpeed);

			//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
			CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "步骤5失败");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤6失败");

		for (eSpeed = EM_UDIPLAYER_SPEED_NORMAL;eSpeed <=EM_UDIPLAYER_SPEED_FASTFORWARD_32;eSpeed++)
		{
			errorCode = CSUDIPLAYERSetSpeed(hPlayer, eSpeed);

			//in live mode, it's return CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
			CSTK_ASSERT_TRUE_FATAL((CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == errorCode)||(CSUDI_SUCCESS == errorCode), "步骤7失败");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤8失败");

	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERSeek        
//@DESCRIPTION:测试直播情况下，接口函数CSUDIPLAYERSeek的执行情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、有效的hPlayer  
//@INPUT:2、有效的nPosInSec
//@INPUT:3、ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_CURRENT 
//@EXPECTATION:直播模式下不支持  CSUDIPLAYERSeek
//@REMARK:
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、hPlayer为CSUDI_NULL，调用 CSUDIPLAYERSeek，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、以正常合法参数调用 CSUDIPLAYERSeek，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤3-步骤4指定次数
//@EXECUTIONFLOW:6、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_Seek_0001(void)
{
	CSUDIPlayPosition_E   ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_CURRENT;
	
	CSUDIStreamInfo_S  sStreamInfo;
	int nStreamCnt, i;
 
	CSUDIPLAYERType_E  ePlayerType  =EM_UDIPLAYER_LIVE;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数	
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;

//设置节目信息
	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSeek(CSUDI_NULL,1,ePlayPosFlag), "步骤3失败");
		
		for (ePlayPosFlag=EM_UDIPLAYER_POSITION_FROM_HEAD;ePlayPosFlag<=EM_UDIPLAYER_POSITION_FROM_END;ePlayPosFlag++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERSeek(hPlayer,0,ePlayPosFlag), "步骤4失败");
		}
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPosInSec           
//@DESCRIPTION:测试直播情况下，接口函数CSUDIPLAYERGetCurPosInSec的执行情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:直播状态下不支持  CSUDIPLAYERGetCurPosInSec
//@REMARK:
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、hPlayer为CSUDI_NULL，调用 CSUDIPLAYERGetCurPosInSec，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、pnPosInSec为CSUDI_NULL，调用 CSUDIPLAYERGetCurPosInSec，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、以正常合法参数调用 CSUDIPLAYERGetCurPosInSec，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、重复步骤3-步骤5指定次数
//@EXECUTIONFLOW:7、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPosInSec_0001(void)
{
	int  nPosInSec;
	
//设置节目信息参数
	CSUDIStreamInfo_S  sStreamInfo;              
	CSUDIPlaybackParam_S 	sPlaybackParam;
	int nStreamCnt,i;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
		
//设置播放器参数
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;
	char * szName="ts";

//设置节目信息
	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	sPlaybackParam.m_eSpeed=EM_UDIPLAYER_SPEED_NORMAL ;
	sPlaybackParam.m_nSecondPos=0;
	strcpy(sPlaybackParam.m_szFileName, szName);

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(CSUDI_NULL,&nPosInSec), "步骤3失败");

		//check porting code pls,it have not check pnPosInSec
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(hPlayer,CSUDI_NULL), "步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPosInSec(hPlayer,&nPosInSec), "步骤5失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPTS          
//@DESCRIPTION:测试CSUDIPLAYERGetCurPTS在参数错误的情况下执行结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、hPlayer为CSUDI_NULL，调用 CSUDIPLAYERGetCurPTS，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、eContentType为非法值，调用 CSUDIPLAYERGetCurPTS，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、psPTS为CSUDI_NULL，调用 CSUDIPLAYERGetCurPTS，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、重复步骤3-步骤6指定次数
//@EXECUTIONFLOW:8、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0001(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	CSUDI_UINT64  sPTS;
	int i;

//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤3失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(CSUDI_NULL,eContentType,&sPTS), "步骤4失败");

		//pls check porting code,it's have not check eContentType
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,  ERRORDATA, &sPTS), "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,CSUDI_NULL), "步骤6失败");
	}
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场错误");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPTS
//@DESCRIPTION:测试CSUDIPLAYERGetCurPTS在应用场景不符合要求的情况下的执行结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERGetCurPTS在start之后、stop之前调用有效，其他时候无效
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、打开播放器后，立即调用CSUDIPLAYERGetCurPTS，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、设置播放器流属性信息后，立即调用CSUDIPLAYERGetCurPTS，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、启动播放器后，调用 CSUDIPLAYERGetCurPTS，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERPause暂停播放器，期望 返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用 CSUDIPLAYERGetCurPTS，期望返回CSUDI_SUCCESS或CSUDIPLAYER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERResume恢复暂停，期望 返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、恢复暂停后，调用 CSUDIPLAYERGetCurPTS，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop停止播放器，期望 返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、停止播放器后，调用 CSUDIPLAYERGetCurPTS，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:13、重复步骤4-步骤12指定次数
//@EXECUTIONFLOW:14、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0002(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	CSUDI_UINT64  sPTS;
	int i;

	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
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
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "步骤3失败");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		for (eContentType=EM_UDI_CONTENT_VIDEO;eContentType<=EM_UDI_CONTENT_AUDIO;eContentType++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "步骤4失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤5失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "步骤6失败");

			udiRe =  CSUDIPLAYERPause(hPlayer);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == udiRe), "步骤7失败");

			udiRe =  CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS);
			
			//pls check poritng code ,it is only work in START mode, when in PAUSE,it's return CSUDIPLAYER_ERROR_INVALID_STATUS 
			CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS  == udiRe)||(CSUDIPLAYER_ERROR_INVALID_STATUS==udiRe)), "步骤8失败");
			
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), "步骤9失败");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "步骤10失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "步骤12失败");
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场错误");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPTS          
//@DESCRIPTION:测试对于视频播放器获取音频PTS信息
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlaye
//@INPUT:2、eContentType= EM_UDI_CONTENT_AUDIO   
//@INPUT:3、 psPTS非空
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERGetCurPTS在start之后、stop之前调用有效，其他时候无效
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERGetCurPTS，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤4指定次数
//@EXECUTIONFLOW:6、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0003(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_AUDIO;
	CSUDI_UINT64  sPTS;
	int i=0;
	
//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤3失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场错误");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetCurPTS          
//@DESCRIPTION:测试对于音频播放器获取视频PTS信息
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlaye
//@INPUT:2、eContentType= EM_UDI_CONTENT_VIDEO   
//@INPUT:3、 psPTS非空
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERGetCurPTS在start之后、stop之前调用有效，其他时候无效
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERGetCurPTS，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤4指定次数
//@EXECUTIONFLOW:6、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0004(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	CSUDI_UINT64  sPTS;
	int i=0;
	
//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤3失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetCurPTS(hPlayer,eContentType,&sPTS), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场错误");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount          
//@DESCRIPTION:测试CSUDIPLAYERGetPacketCount在参数错误的情况下执行结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、hPlayer为CSUDI_NULL，调用 CSUDIPLAYERGetPacketCount，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、eContentType为非法值，调用 CSUDIPLAYERGetPacketCount，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、puPacketCnt为CSUDI_NULL，调用 CSUDIPLAYERGetPacketCount，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、重复步骤3-步骤6指定次数
//@EXECUTIONFLOW:8、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0001(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	unsigned int   uPacketCnt;
	int i;

//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤3失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(NULL,eContentType,&uPacketCnt), "步骤4失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,  ERRORDATA, &uPacketCnt), "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,CSUDI_NULL), "步骤6失败");
	}
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场错误");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount
//@DESCRIPTION:测试CSUDIPLAYERGetPacketCount在应用场景不符合要求的情况下的执行结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:CSUDIPLAYERGetPacketCount在start之后、stop之前调用有效，其他时候无效
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、打开播放器后，立即调用CSUDIPLAYERGetCurPTS，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、设置播放器流属性信息后，立即调用CSUDIPLAYERGetPacketCount，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、启动播放器后，调用 CSUDIPLAYERGetPacketCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERPause暂停播放器，期望 返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用 CSUDIPLAYERGetPacketCount，期望返回CSUDI_SUCCESS或CSUDIPLAYER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:9、如果暂停播放器成功，调用CSUDIPLAYERResume恢复暂停，期望 返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、如果暂停播放器成功并且恢复暂停成功后，调用 CSUDIPLAYERGetPacketCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop停止播放器，期望 返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、停止播放器后，调用 CSUDIPLAYERGetPacketCount，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:13、重复步骤4-步骤12指定次数
//@EXECUTIONFLOW:14、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0002(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	unsigned   int uPacketCnt;
	int i;

	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
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
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,asStreamInfo,nStreamCnt,CSUDI_NULL), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "步骤4失败");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		for (eContentType=EM_UDI_CONTENT_VIDEO;eContentType<=EM_UDI_CONTENT_AUDIO;eContentType++)
		{
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤5失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "步骤6失败");

			udiRe =  CSUDIPLAYERPause(hPlayer);

			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == udiRe), "步骤7失败");

			udiRe =  CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt);
			
			//pls check poritng code ,it is only work in START mode, when in PAUSE,it's return CSUDIPLAYER_ERROR_INVALID_STATUS 
			CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS  == udiRe) || (CSUDIPLAYER_ERROR_INVALID_STATUS == udiRe)), "步骤8失败");
			
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), "步骤9失败");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "步骤10失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&uPacketCnt), "步骤12失败");
		}
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场错误");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount          
//@DESCRIPTION:测试对于视频播放器获取音频PacketCount
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlayer
//@INPUT:2、eContentType= EM_UDI_CONTENT_AUDIO   
//@INPUT:3、 puPacketCnt非空
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:对视频播放器获取音频PacketCount，将返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERGetPacketCount，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤4指定次数
//@EXECUTIONFLOW:6、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0003(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_AUDIO;

	unsigned int  puPacketCnt;
	
	int i=0;
	
//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤3失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&puPacketCnt), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场错误");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount          
//@DESCRIPTION:测试对于音频播放器获取视频Packet Count
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlaye
//@INPUT:2、eContentType= EM_UDI_CONTENT_AUDIO   
//@INPUT:3、 puPacketCnt非空
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:对音频播放器获取视频PacketCount，将返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERGetPacketCount，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤4指定次数
//@EXECUTIONFLOW:6、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0004(void)
{
	CSUDIContentType_E eContentType=EM_UDI_CONTENT_VIDEO;
	unsigned int  puPacketCnt;
	int i=0;
	
//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAMTYPE_NUM;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_AUD_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤3失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERGetPacketCount(hPlayer,eContentType,&puPacketCnt), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场错误");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERGetPacketCount			
//@DESCRIPTION:测试单独播放音频能够准确获取音频packetcount
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlaye
//@INPUT:2、eContentType= EM_UDI_CONTENT_AUDIO	 
//@INPUT:3、 puPacketCnt非空
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:能够准确获取GetPacketCount
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、调用CSUDIPLAYERGetPacketCount获取音频解码packet个数，在10秒之内期望个数不断改变
//@EXECUTIONFLOW:6、提示测试人员拔掉信号线，期望5秒之内音频PacketCount没有改变
//@EXECUTIONFLOW:7、提示测试人员重新插上信号线，期望立即有音频PacketCount发生改变
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:9、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0005(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	unsigned int uPacketCnt = 0;
	unsigned int uLastPacketCnt = 0;
	int i;

	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");

	//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_AUDIO), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSTCPrint("音频是否能够正常播放？\n");
	CSTCPrint("Is the audio output normal? \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤4失败：音频播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "步骤5失败");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约%d秒\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "步骤5失败");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt!=uLastPacketCnt),"步骤5失败：音频PacketCnt没有改变");
		uLastPacketCnt = uPacketCnt;
	}

	CSTCPrint("请拔掉信号线\n");
	CSTCPrint("Pls disattach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤6失败：没有拔掉信号线");

	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约5秒\n");
	CSUDIOSThreadSleep(5000);//等待5秒再检查数据
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "步骤6失败");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约%d秒\n", REPEAT_TIMES/2);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt),"步骤6失败");
		CSTCPrint("@%d lastPacketCnt=%d , thisPacketCnt=%d \n", i, uLastPacketCnt, uPacketCnt);
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt==uLastPacketCnt),"步骤6失败：音频PacketCnt变化了");
		uLastPacketCnt = uPacketCnt;
		
	}

	CSTCPrint("请重新插上信号线\n");
	CSTCPrint("Pls attach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：没有插上信号线");

	CSUDIOSThreadSleep(500);//等待
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "步骤6失败");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约%d秒\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uPacketCnt), "步骤6失败");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt!=uLastPacketCnt),"步骤7失败：音频PacketCnt没有改变");
		uLastPacketCnt = uPacketCnt;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤8失败");
	CSTK_FATAL_POINT;

	CSTCPrint("请确认已重新插上信号线\n");
	CSTKWaitAnyKey();

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤9失败");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}
//@CASEGROUP:CSUDIPLAYERGetPacketCount			
//@DESCRIPTION:测试单独播放视频能够准确获取视频的packetcount
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlaye
//@INPUT:2、eContentType= EM_UDI_CONTENT_VIDEO	 
//@INPUT:3、 puPacketCnt非空
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:能够准确获取GetPacketCount
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、调用CSUDIPLAYERGetPacketCount获取视频解码packet个数，期望个数不断改变
//@EXECUTIONFLOW:6、提示测试人员拔掉信号线，期望5秒之内视频PacketCount没有改变
//@EXECUTIONFLOW:7、提示测试人员重新插上信号线，期望立即有视频PacketCount发生改变
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:9、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0006(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	unsigned int uPacketCnt = 0;
	unsigned int uLastPacketCnt = 0;
	int i;
	
	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");
	
	//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO), "步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSTCPrint("视频是否能够正常播放？\n");
	CSTCPrint("Is the video output normal? \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤4失败：视频播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "步骤5失败");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约%d秒\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "步骤5失败");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt!=uLastPacketCnt),"步骤5失败：视频PacketCnt没有改变");
		uLastPacketCnt = uPacketCnt;
	}
	
	CSTCPrint("请拔掉信号线\n");
	CSTCPrint("Pls disattach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤6失败：没有拔掉信号线");
	
	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约5秒\n");
	CSUDIOSThreadSleep(5000);//等待5秒再检查数据
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "步骤6失败");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约%d秒\n", REPEAT_TIMES/2);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt),"步骤6失败");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt==uLastPacketCnt),"步骤6失败：视频PacketCnt变化了");
		uLastPacketCnt = uPacketCnt;
		
	}
	
	CSTCPrint("请重新插上信号线\n");
	CSTCPrint("Pls attach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：没有插上信号线");
	
	CSUDIOSThreadSleep(500);//等待
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "步骤6失败");
	uLastPacketCnt = uPacketCnt;
	
	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约%d秒\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uPacketCnt), "步骤6失败");
		CSTK_ASSERT_TRUE_FATAL((uPacketCnt!=uLastPacketCnt),"步骤7失败：视频PacketCnt没有改变");
		uLastPacketCnt = uPacketCnt;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤8失败");
	CSTK_FATAL_POINT;

	CSTCPrint("请确认已重新插上信号线\n");
	CSTKWaitAnyKey();
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤9失败");
		hPlayer=CSUDI_NULL;
	}

	return CSUDI_TRUE;

}
//@CASEGROUP:CSUDIPLAYERGetPacketCount			
//@DESCRIPTION:测试播放音视频能够准确获取视频的packetcount
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlaye
//@INPUT:2、eContentType= EM_UDI_CONTENT_AUDIO|EM_UDI_CONTENT_VIDEO	 
//@INPUT:3、 puPacketCnt非空
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:能够准确获取GetPacketCount
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、调用CSUDIPLAYERGetPacketCount获取音视频解码packet个数，期望个数不断改变
//@EXECUTIONFLOW:6、提示测试人员拔掉信号线，期望5秒之内音视频PacketCount没有改变
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClear清除缓存的数据，期望再获取到的音视频PacketCount都为0或者无任何变化
//@EXECUTIONFLOW:8、提示测试人员重新插上信号线，期望立即有音视频PacketCount发生改变
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:10、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0007(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	unsigned int uAudioPacketCnt = 0;
	unsigned int uLastAudioPacketCnt = 0;
	unsigned int uVideoPacketCnt = 0;
	unsigned int uLastVideoPacketCnt = 0;

	int i;
	
	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");
	
	//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	
	CSTCPrint("音视频是否能够正常播放？\n");
    CSTCPrint("Is A/V output normal ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤4失败：音视频播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "步骤5失败");
	uLastAudioPacketCnt = uAudioPacketCnt;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "步骤5失败");
	uLastVideoPacketCnt = uVideoPacketCnt;

	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约%d秒\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "步骤5失败");
		CSTK_ASSERT_TRUE_FATAL((uAudioPacketCnt!=uLastAudioPacketCnt),"步骤5失败：音频PacketCnt没有改变");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "步骤5失败");
		CSTK_ASSERT_TRUE_FATAL((uVideoPacketCnt!=uLastVideoPacketCnt),"步骤5失败：视频PacketCnt没有改变");
		uLastVideoPacketCnt = uVideoPacketCnt;
		uLastAudioPacketCnt = uAudioPacketCnt;
	}
	
	CSTCPrint("请拔掉信号线\n");
	CSTCPrint("Pls disattach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤6失败：没有拔掉信号线");
	
	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约5秒\n");
	CSUDIOSThreadSleep(5000);//等待5秒再检查数据
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "步骤6失败");
	uLastAudioPacketCnt = uAudioPacketCnt;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "步骤6失败");
	uLastVideoPacketCnt = uVideoPacketCnt;

	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约%d秒\n", REPEAT_TIMES/2);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(500);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "步骤6失败");
		CSTK_ASSERT_TRUE_FATAL((uAudioPacketCnt==uLastAudioPacketCnt),"步骤6失败：音频PacketCnt变化了");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "步骤6失败");
		CSTK_ASSERT_TRUE_FATAL((uVideoPacketCnt==uLastVideoPacketCnt),"步骤6失败: 视频PacketCntPacketCnt变化了");
		uLastVideoPacketCnt = uVideoPacketCnt;
		uLastAudioPacketCnt = uAudioPacketCnt;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClear(hPlayer), "清除播放器中缓存的数据失败");

	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约2秒\n");
	CSUDIOSThreadSleep(2000);//等待2秒再检查数据

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL((uAudioPacketCnt == 0 || uLastAudioPacketCnt == uAudioPacketCnt), "步骤7失败：CSUDIPLAYERClear之后音频PacketCnt错误");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL((uVideoPacketCnt == 0 || uLastVideoPacketCnt == uVideoPacketCnt), "步骤7失败：CSUDIPLAYERClear之后视频PacketCnt错误");
	
	CSTCPrint("请重新插上信号线\n");
	CSTCPrint("Pls attach the cable \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：没有插上信号线");
	
	CSUDIOSThreadSleep(500);//等待
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "步骤7失败");
	uLastAudioPacketCnt = uAudioPacketCnt;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "步骤7失败");
	uLastVideoPacketCnt = uVideoPacketCnt;

	CSTCPrint("测试CSUDIPLAYERGetPacketCount，请等待约%d秒\n", REPEAT_TIMES);
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSUDIOSThreadSleep(1000);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&uAudioPacketCnt), "步骤7失败");
		CSTK_ASSERT_TRUE_FATAL((uAudioPacketCnt!=uLastAudioPacketCnt),"步骤7失败：音频PacketCnt没有改变");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&uVideoPacketCnt), "步骤7失败");
		CSTK_ASSERT_TRUE_FATAL((uVideoPacketCnt!=uLastVideoPacketCnt),"步骤7失败：视频PacketCnt没有改变");
		uLastVideoPacketCnt = uVideoPacketCnt;
		uLastAudioPacketCnt = uAudioPacketCnt;
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤8失败");
	CSTK_FATAL_POINT;

	CSTCPrint("请确认已重新插上信号线\n");
	CSTKWaitAnyKey();
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤9失败");
		hPlayer=CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:测试CSUDIPLAYERTrickMode在参数错误的情况下执行结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:非法的播放句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、hPlayer为CSUDI_NULL,  bTrickMode为CSUDI_TRUE，调用 CSUDIPLAYERTrickMode，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、hPlayer为CSUDI_NULL,  bTrickMode为CSUDI_FALSE，调用 CSUDIPLAYERTrickMode，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0001(void)
{
//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1打开播放器失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, &sStreamInfo, nStreamCnt, CSUDI_NULL), "步骤2设置流信息失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤3开始播放失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERTrickMode(CSUDI_NULL, CSUDI_TRUE), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIPLAYERTrickMode(CSUDI_NULL, CSUDI_FALSE), "步骤5失败");
	
	CSTK_FATAL_POINT;

	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop (hPlayer), "恢复现场错误");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "恢复现场错误");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:测试重复进入和退出TrickMode的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、
//@EXPECTATION:返回CSUDIPLAYER_ERROR_INVALID_STATUS
//@REMARK:
//@EXECUTIONFLOW:1、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:2、调用CSUDIPLAYERSetStream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYERStart，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERTrickMode,退出TrickMode模式，即设置TrickMode为假，期望返回CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR
//@EXECUTIONFLOW:5、调用CSUDIPLAYERTrickMode,进入TrickMode模式，即设置TrickMode为真，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、再次调用CSUDIPLAYERTrickMode,进入TrickMode模式，即设置TrickMode为真，期望返回CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR
//@EXECUTIONFLOW:7、调用CSUDIPLAYERTrickMode,退出TrickMode模式，即设置TrickMode为假，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、再次调用CSUDIPLAYERTrickMode,退出TrickMode模式，即设置TrickMode为假，期望返回CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR
//@EXECUTIONFLOW:9、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0002(void)
{
//设置播放器信息		
	unsigned int  uDemuxType=EM_UDI_DEMUX_PLAY;  // 获取具有播放功能的播放器
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_MPEG2;
	CSUDIVIDStreamType_E  eVidStreamType=EM_UDI_VID_STREAM_MPEG2 ;
	CSUDIPLAYERType_E  ePlayerType=EM_UDIPLAYER_LIVE;
	
//设置节目信息
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S  sStreamInfo;              
	int nStreamCnt;

	sStreamInfo.m_nPid=352;
	sStreamInfo.m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo.m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	nStreamCnt=1;
	
//创建播放器
	hPlayer =PLAYER_openPlayer( uDemuxType, eAudStreamType, eVidStreamType, ePlayerType);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤1打开播放器失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer, &sStreamInfo, nStreamCnt, CSUDI_NULL), "步骤2设置流信息失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤3开始播放失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE), "步骤5失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE), "步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "步骤7失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "步骤8失败");		

	CSTK_FATAL_POINT;

	if(hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop (hPlayer), "步骤9-1恢复现场错误");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤9-2恢复现场错误");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:测试在标清码流下，进入TrickMOde模式的执行结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlayer
//@EXPECTATION:期望画面能够变速播放,无回帧,卡壳等异常
//@REMARK:
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来
//@EXECUTIONFLOW:6、调用CSUDIPLAYERTrickMode,进入TrickMode模式，即置trickMode为真
//@EXECUTIONFLOW:7、提示测试人员观察，期望产生变速播放，即只有I帧的播放并且无回帧,卡壳等异常
//@EXECUTIONFLOW:8、调用CSUDIPLAYERTrickMode，退出TrickMOde
//@EXECUTIONFLOW:9、提示测试人员观察，期望回到正常速率播放
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop停止播放
//@EXECUTIONFLOW:11、调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0003(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	CSTCPrint("CSTC_LIVE_TEST_IT_GetTrickMode_0003: 测试GetTrickMode是否有效(Audio&Video Test_27Mbps_20070524.ts)\n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1锁频失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2打开播放器失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3设置流信息失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4开始播放失败");

	CSTCPrint("音视频是否能够正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	udiRe = CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"步骤6失败：播放不正常");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("视频是否变速播放\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：变速播放不正常");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "步骤8失败");
		CSTCPrint("音视频是否恢复正常播放速率\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败：开始播放不正常");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败");

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤11失败");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:测试在高清码流下，进入TrickMOde模式的执行结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlayer
//@EXPECTATION:期望画面能够变速播放
//@REMARK:
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来
//@EXECUTIONFLOW:6、调用CSUDIPLAYERTrickMode,进入TrickMode模式，即置trickMode为真
//@EXECUTIONFLOW:7、提示测试人员观察，期望产生变速播放，即只有I帧的播放
//@EXECUTIONFLOW:8、调用CSUDIPLAYERTrickMode，退出TrickMOde
//@EXECUTIONFLOW:9、提示测试人员观察，期望播放正常
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop停止播放
//@EXECUTIONFLOW:11、调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0004(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	if (-1 == PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("不支持H264视频格式\n");
		CSTCPrint("Not support STREAM_H264\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_GetTrickMode_0004: 测试GetTrickMode是否有效(CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg)\n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "步骤1失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG1, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer, EM_LIVE_H264_HD_720P, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSTCPrint("音视频是否能够正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	udiRe = CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"步骤6失败：播放不正常");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("视频是否变速播放\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：变速播放不正常");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "步骤8失败");
		CSTCPrint("音视频是否恢复正常播放速率\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败：开始播放不正常");

	}

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤11失败");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:测试在标清码流下，进入TrickMOde模式之后换台执行的结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlayer
//@EXPECTATION:期望换台之后能够正常播放节目,能够正常进入TrickMode
//@REMARK:
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来
//@EXECUTIONFLOW:6、调用CSUDIPLAYERTrickMode,进入TrickMode模式，即置trickMode为真
//@EXECUTIONFLOW:7、提示测试人员观察，期望产生变速播放，即只有I帧的播放
//@EXECUTIONFLOW:8、调用CSUDIPLAYERTrickMode,退出TrickMode模式，
//@EXECUTIONFLOW:9、提示测试人员观察，期望正常播放
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop停止播放
//@EXECUTIONFLOW:11、调用CSUDIPLAYERSetStream设置码流信息，即换台
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart开始播放
//@EXECUTIONFLOW:13、提示测试人员观察，是否有音视频播放出来
//@EXECUTIONFLOW:14、调用CSUDIPLAYERTrickMode,进入TrickMode模式，即置trickMode为真
//@EXECUTIONFLOW:15、提示测试人员观察，期望产生变速播放，即只有I帧的播放
//@EXECUTIONFLOW:16、调用CSUDIPLAYERTrickMode,退出TrickMode模式，
//@EXECUTIONFLOW:17、提示测试人员观察，期望正常播放
//@EXECUTIONFLOW:18、调用CSUDIPLAYERStop停止播放
//@EXECUTIONFLOW:19、调用CSUDIPLAYERClose关闭播放器
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

	CSTCPrint("CSTC_LIVE_TEST_IT_GetTrickMode_0005: 测试GetTrickMode是否有效(Audio&Video Test_27Mbps_20070524.ts)\n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1锁频失败\n");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2创建播放器失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3设置码流信息失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4开始播放失败\n");

	CSTCPrint("音视频是否能够正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常\n");

	udiRe = CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"步骤6失败：播放不正常");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("视频是否变速播放\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：变速播放不正常");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "步骤8失败，退出TrickMode播放失败");
		CSTCPrint("音视频是否再次正常播放了\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败：开始播放不正常");

	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败:停止播放失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream(hPlayer, &sStreamInfo[0], nStreamCnt, NULL), "步骤11:设备码流信息失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤12:开始播放失败");
	CSTCPrint("音视频换台之后是否再次正常播放了\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败：开始播放不正常");

	udiRe = CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"步骤14失败：播放不正常");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("视频是否变速播放\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤15失败：变速播放不正常");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "步骤16:退出TrcikMode失败");
		CSTCPrint("音视频是否再次正常播放了\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤17失败：正常播放失败");

	}
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤18停止播放失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤19关闭播放器失败");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERTrickMode         
//@DESCRIPTION:测试在高清码流下，进入TrickMOde模式之后换台执行的结果
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、合法的hPlayer
//@EXPECTATION:期望能够换台成功
//@REMARK:
//@EXECUTIONFLOW:1、根据配置文件，前端锁CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg码流对应的频点
//@EXECUTIONFLOW:2、根据CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg流的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来
//@EXECUTIONFLOW:6、调用CSUDIPLAYERTrickMode,进入TrickMode模式，即置trickMode为真
//@EXECUTIONFLOW:7、提示测试人员观察，期望产生变速播放，即只有I帧的播放
//@EXECUTIONFLOW:8、调用CSUDIPLAYERTrickMode，退出TrickMOde
//@EXECUTIONFLOW:9、提示测试人员观察，期望播放正常
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop，停止当前码流的播放
//@EXECUTIONFLOW:11、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:12、根据Audio&Video Test_27Mbps_20070524.ts流AC3标准测试节目的码流属性设置此播放器的属性
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStart，开始播放
//@EXECUTIONFLOW:14、提示测试人员观察，期望能够进入CCTV1高清直播
//@EXECUTIONFLOW:15、调用CSUDIPLAYERTrickMode,进入TrickMode模式，即置trickMode为真
//@EXECUTIONFLOW:16、提示测试人员观察，期望产生变速播放，即只有I帧的播放
//@EXECUTIONFLOW:17、调用CSUDIPLAYERTrickMode，退出TrickMOde
//@EXECUTIONFLOW:18、提示测试人员观察，期望播放正常
//@EXECUTIONFLOW:19、调用CSUDIPLAYERStop停止播放
//@EXECUTIONFLOW:20、调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0006(void)
{

	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	
	if (-1 == PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("不支持H264视频格式\n");
		CSTCPrint("Not support STREAM_H264\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_GetTrickMode_0006: 测试GetTrickMode是否有效(CCTV_HP4.1_CBR20Mbps_1920x1080_25fps_mp2stereo224kbps_48KHz.mpg)\n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "步骤1锁频失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG1, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2创建播放器失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer, EM_LIVE_H264_HD_720P, \
																EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3设置码流信息失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4开始播放失败");
	CSTCPrint("音视频是否能够正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE), "步骤6进入TrickMode失败");
	CSTCPrint("音视频是否进入变速播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：变速播放不正常");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "步骤8退出TrickMode失败");
	CSTCPrint("音视频是否进入正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败：正常播放失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10停止播放失败");
												
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AC3), "步骤11锁频失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE,hPlayer, EM_LIVE_MPEG2_AC3, \
																EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤12设置码流信息失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤13开始播放失败");
	CSTCPrint("音视频换台之后是否能够正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤14失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_TRUE), "步骤15进入TrickMode失败");
	CSTCPrint("音视频是否进入变速播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤16失败：变速播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERTrickMode(hPlayer, CSUDI_FALSE), "步骤17退出TrickMode失败");
	CSTCPrint("音视频是否进入正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤18失败：正常播放不正常");
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤19停止播放失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤20关闭播放器失败");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYERClear          
//@DESCRIPTION:测试传入参数非法及合法的情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、非法的hPlayer及合法的hPlayer
//@EXPECTATION:传入非法参数返回非CSUDI_SUCCESS，传入合法参数返回CSUDI_SUCCESS
//@REMARK:传入CSUDI_NULL，然后调用此接口，将返回非CSUDI_SUCCESS，传入合法参数则返回CSUDI_SUCCESS，如果平台不支持此接口则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
//@EXECUTIONFLOW:1、传入非法播放句柄CSUDI_NULL并调用CSUDIPLAYERClear，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:2、传入合法播放句柄并调用CSUDIPLAYERClear，期望返回CSUDI_SUCCESS，如果平台不支持此接口则返回CSUDI_ERROR_PLAYER_NOTSUPPORT
CSUDI_BOOL CSTC_LIVE_TEST_IT_Clear_0001(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPLAYERType_E ePlayerType;
	CSUDI_Error_Code uRetStatus;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERClear(hPlayer), "步骤1失败");

	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nAudioDecoder = 0;
	sPlayerChnl.m_nVideoDecoder = 0;
	ePlayerType = EM_UDIPLAYER_LIVE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer), "步骤2失败，没有成功创建一个Liva播放器");

	uRetStatus = CSUDIPLAYERClear(hPlayer);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == uRetStatus) || (CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED == uRetStatus), "步骤2失败");
	
	CSTK_FATAL_POINT
	{
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "销毁播放器失败")
		}
	}

	return CSUDI_TRUE;
}

/**
* param[in]: bIsSet 如果为CSUDI_TRUE表示调用CSUDIPLAYERSetStream来改变，反之调用CSUDIPLAYERModifyStream
* param[in]: eSerIndex 对应PlayerTestServiceIndex_E枚举类型中的定义
* param[in]: dwStreamType 为PlayerTestStreamType_E枚举类型的组合,例如:EM_STREAM_VIDEO|EM_STREAM_AUDIO
*/
static CSUDI_BOOL PLAYER_iSetStream(CSUDI_BOOL bIsSet,CSUDI_HANDLE hPlayer,PlayerTestServiceIndex_E eSerIndex,unsigned int dwStreamType)
{
	CSUDIStreamInfo_S  sStreamInfo[10];  //不会超过10个
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

	CSTCPrint("正在播放 %s...... 节目\r\n",g_sPlayerSeviceInfo[eSerIndex].m_pcServiceDescript);
	
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
//@DESCRIPTION:以下是测试用例中用到的音视频信息
//@REMARK:以下所有用例中的输入参数均以下信息的枚举值来替代
//@DESCRIPTION:结构体信息为:
//@INPUT:typedef struct 
//@INPUT:{	
//@INPUT:	const char*          m_pcTsFilename;  	  //码流文件名称
//@INPUT:	const char* 		 m_pcServiceDescript; 	//码流描述
//@INPUT:	int                  m_nVideoPid;           ///< 数据所在PID，-1表示不存在
//@INPUT:	CSUDIVIDStreamType_E m_eVidStreamType;      ///视频类型
//@INPUT:	int                  m_nAudioPid;           ///< 数据所在PID ，-1表示不存在
//@INPUT:	CSUDIAUDStreamType_E m_eAudStreamType;      ///<音频类型
//@INPUT:	int 				 m_nPcrPid;				///< PCR类型PID，-1表示不存在
//@INPUT:	int 				 m_nSubPid;				///<SUBTITLE类型的PID，-1表示不存在
//@INPUT:	int 				 m_nTelPid;				///<TELETEXT类型的PID，-1表示不存在
//@INPUT:}PlayerTestServiceInfo_S;
//@INPUT:		{
//@INPUT:			"Audio&Video Test_27Mbps_20070524.ts",
//@INPUT:			"多语言AC3MP2",
//@INPUT:			60,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			62,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			60,
//@INPUT:		}, /*EM_LIVE_MPEG2_MULTI_LAN1*/
//@INPUT:		{
//@INPUT:			"Audio&Video Test_27Mbps_20070524.ts",
//@INPUT:			"测试视音同步",
//@INPUT:			100,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			101,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			100,
//@INPUT:		}, /*EM_LIVE_MPEG2_AV_SYN*/
//@INPUT:		{
//@INPUT:			"Audio&Video Test_27Mbps_20070524.ts",
//@INPUT:			"多语言左右声道",
//@INPUT:			1140,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			1142,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:		}, /*EM_LIVE_MPEG2_MULTI_LAN2*/
//@INPUT:		{
//@INPUT:			"Irdeto.PCR.CCITT.ts",
//@INPUT:			"指标测试节目",
//@INPUT:			100,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			101,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			100,
//@INPUT:		}, /*EM_LIVE_MPEG2_ABILITY_TEST1*/
//@INPUT:		{
//@INPUT:			"Irdeto.PCR.CCITT.ts",
//@INPUT:			"PCR间隔为0.01S",
//@INPUT:			2049,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			2052,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			2049,
//@INPUT:		}, /*EM_LIVE_MPEG2_ABILITY_TEST2*/
//@INPUT:		{
//@INPUT:			"Irdeto.PCR.CCITT.ts",
//@INPUT:			"PCR间隔为0.1S",
//@INPUT:			2065,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			2066,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			2065,
//@INPUT:		}, /*EM_LIVE_MPEG2_ABILITY_TEST3*/
//@INPUT:		{
//@INPUT:			"H.264.ts",
//@INPUT:			"MPEG1视频测试节目2",
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
//@INPUT:			"H264高清720P音视频",
//@INPUT:			0x1e1,
//@INPUT:			EM_UDI_VID_STREAM_H264,
//@INPUT:			0x1e2,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG1,
//@INPUT:		}, /*EM_LIVE_H264_HD_720P*/
//@INPUT:		{
//@INPUT:			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
//@INPUT:			"H264高清1080I音视频",
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
//@INPUT:			"新加波亚洲新闻台 PAL",
//@INPUT:			0x300,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			0x301,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			0x200
//@INPUT:		}, /*EM_LIVE_MPEG2_PAL*/	
//@INPUT:		{
//@INPUT:			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
//@INPUT:			"新加波亚洲新闻台 NTSC",
//@INPUT:			0x315,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			0x316,
//@INPUT:			EM_UDI_AUD_STREAM_MPEG2,
//@INPUT:			0x201
//@INPUT:		}, /*EM_LIVE_MPEG2_NTSC*/
//@INPUT:		{
//@INPUT:			"Audio&Video Test_27Mbps_20070524.ts",
//@INPUT:			"DolbyAC3标准测试",
//@INPUT:			512,
//@INPUT:			EM_UDI_VID_STREAM_MPEG2,
//@INPUT:			650,
//@INPUT:			EM_UDI_AUD_STREAM_AC3,
//@INPUT:			128,
//@INPUT:		}, /*EM_LIVE_MPEG2_AC3*/
//@INPUT:		{
//@INPUT:			"Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts",
//@INPUT:			"测试VBI方式的TELTEXT",
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
//@INPUT:			"AC3PLUS音频节目",
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
//@INPUT:			"AAC音频节目",
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
//@INPUT:			"测试视音同步",
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
//@DESCRIPTION:测试对播放器进行控制从而实现音视频的播放
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: 对播放器能够正常控制（包括开启，暂停，继续，停止）
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERPause,暂停解码器，期望返回CSUDI_SUCCESS或CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:7、如果步骤6返回CSUDI_SUCCESS，则提示测试人员判断音视频是否暂停或停止?
//@EXECUTIONFLOW:8、如果步骤6返回CSUDI_SUCCESS，则调用CSUDIPLAYERResume，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、如果步骤6返回CSUDI_SUCCESS，则提示测试人员判断音视频是否恢复播放?
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、提示测试人员观察，音视频是否停止播放
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart 开启播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、提示测试人员观察，音视频是否重新开始播放
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0001(void)
{
	CSUDI_HANDLE   hPlayer=CSUDI_NULL;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0001: 测试对播放器进行控制从而实现音视频的播放(Audio&Video Test_27Mbps_20070524.ts)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0001: Test the control ability of the player \n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSTCPrint("音视频是否能够正常播放？\n");
    CSTCPrint("Is A/V output normal ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	udiRe = CSUDIPLAYERPause(hPlayer);
	CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS== udiRe)||(CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED== udiRe)),"步骤6失败：播放不正常");
	if (CSUDI_SUCCESS == udiRe)
	{
		CSTCPrint("音视频是否再次暂停播放了\n");
        CSTCPrint("Does the player pause ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：暂停播放不正常");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤8失败");
		CSTCPrint("音视频是否再次开始播放了\n");
        CSTCPrint("Does the player start again and the A/V has normal output ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败：开始播放不正常");

	}
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败");
    CSTCPrint("音视频是否停止播放了\n");
    CSTCPrint("Does the player stop ?\n");
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败：停止播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤12失败");
	
	CSTCPrint("音视频是否再次开始播放了\n");
    CSTCPrint("Does the player start again and the A/V has normal output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败：开始播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤14失败");

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤15失败");
		hPlayer=CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试单独播放视频MPEG2标清的情况 
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION:播放器能够正常播放所有的支持的视频
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的视频码流类型，打开视频直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流的视频属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否只有视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0002:MPEG2 标清视频测试(Audio&Video Test_27Mbps_20070524.ts码流)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0002: Test the ability of playing SD program \n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE ==  PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("请确定是否是\"视频可以正常播放，并且音频不能播放出来\"\n");
    CSTCPrint("Do we get normal video output and get no audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试单独播放视频MPEG2高清的情况 
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT:  EM_LIVE_MPEG2_HD
//@EXPECTATION:播放器能够正常播放所有的支持的视频
//@EXECUTIONFLOW:1、根据配置文件，前端锁Player_Mpeg_HD.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Player_Mpeg_HD.ts流的视频码流类型，打开视频直播播放器
//@EXECUTIONFLOW:3、根据Player_Mpeg_HD.ts流的视频属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否只有视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	if (!s_bPlayerSupportHD)
	{
		CSTCPrint("不支持高清视频\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0003:MPEG2 高清视频测试\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0003:Test the ability of playing MPEG2 HD program\n");
	
//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_HD), "步骤1失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_HD,EM_STREAM_VIDEO), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	CSUDIOSThreadSleep(SLEEPTIME);
	
	CSTCPrint("请确定是否是\"视频可以正常播放，并且音频不能播放出来\"\n");
	CSTCPrint("Do we get normal video output and get no audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试单独播放视频MPEG1的情况
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG1_AV
//@EXPECTATION: 播放器能够正常播放所有的支持的视频
//@EXECUTIONFLOW:1、根据配置文件，前端锁H.264.ts码流对应的频点
//@EXECUTIONFLOW:2、根据H.264.ts流的视频码流类型(MPEG1视频)，打开视频直播播放器
//@EXECUTIONFLOW:3、根据H.264.ts流的视频属性(MPEG1视频)设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有只有视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0004(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0004:MPEG1 标清视频测试(H.264.ts码流)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0004:Test the ability of playing MPEG1 SD video\n");

	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG1_AV), "步骤1失败");

	//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_MPEG1, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG1_AV,EM_STREAM_VIDEO), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	
	CSTCPrint("请确定是否是\"视频可以正常播放，并且音频不能播放出来\"\n");
	CSTCPrint("Do we get normal video output and get no audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");


	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试单独播放视频H.264标清的情况
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_H264_SD
//@EXPECTATION: 如果系统支持H264，期望播放器能够正常播放所有的支持的视频
//@EXECUTIONFLOW:1、根据配置文件，前端锁H.264.ts码流对应的频点
//@EXECUTIONFLOW:2、根据H.264.ts流的视频码流类型(H264视频)，打开视频直播播放器
//@EXECUTIONFLOW:3、根据H.264.ts流的视频属性(H264视频)设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有只有视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0005(void)
{
	unsigned int enStream = EM_LIVE_H264_SD;
	if(-1 != PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSUDI_HANDLE  hPlayer = CSUDI_NULL;

		CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0005:H264 标清视频播放测试\n");
        CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0005:Test the ability of playing H.264 SD video\n");

		//根据配置文件上的信息进行锁频
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(enStream), "步骤1失败");

		//创建播放器
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,enStream,EM_STREAM_VIDEO), "步骤3失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

		CSUDIOSThreadSleep(SLEEPTIME);

		CSTCPrint("视频可以正常播放，并且音频不能播放出来?\n");
        CSTCPrint("Do we get normal video output and get no audio output\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");

		CSTK_FATAL_POINT;
		
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
			hPlayer = CSUDI_NULL;
		}
		return CSUDI_TRUE;
		}
	else
	{
		CSTCPrint("不支持H264视频格式\r \n");
		CSTCPrint("Not supported STREAM_H264\r \n");
		return CSUDI_FALSE;
	}
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试单独播放视频H.264高清的情况
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_H264_HD_720P
//@EXPECTATION: 如果系统支持H264，期望播放器能够正常播放所有的支持的视频
//@EXECUTIONFLOW:1、根据配置文件，前端锁测试码流对应的频点
//@EXECUTIONFLOW:2、根据测试流的视频码流类型(H264视频)，打开视频直播播放器
//@EXECUTIONFLOW:3、根据测试流的视频属性(H264视频)设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有只有视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0006(void)
{
	unsigned int enStream = EM_LIVE_H264_HD_720P;
	if(-1 != PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSUDI_HANDLE   hPlayer = CSUDI_NULL;

		CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0006:H264 高清视频播放测试\n");
        CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0006:Test the ability of playing H.264 HD video\n");

		//根据配置文件上的信息进行锁频
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(enStream), "步骤1失败");

		//创建播放器
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, INVALIDE_TYPE, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,enStream,EM_STREAM_VIDEO), "步骤3失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

		CSUDIOSThreadSleep(SLEEPTIME);

		CSTCPrint("视频可以正常播放，并且音频不能播放出来?\n");
        CSTCPrint("Do we get normal video output and get no audio output\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");

		CSTK_FATAL_POINT;
		
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
			hPlayer = CSUDI_NULL;
		}

		return CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("不支持H264视频格式\r \n");
		CSTCPrint("Not supported STREAM_H264\r \n");
		return CSUDI_FALSE;
	}
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试PAL-NTSC制式切换时的播放情况
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化 
//@INPUT:1、EM_LIVE_MPEG2_PAL
//@INPUT:2、EM_LIVE_MPEG2_NTSC
//@EXPECTATION: 期望播放器能够正常播放，不出现异常现象
//@EXECUTIONFLOW:1、根据配置文件，前端锁SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts码流对应的频点
//@EXECUTIONFLOW:2、根据SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts流的音视频码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、添加EM_UDIPLAYER_VIDEO_SOURCE_CHANGE事件的回调函数
//@EXECUTIONFLOW:4、根据SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts流的音视频属性设置此播放器的码流属性
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:6、提示测试人员观察，PAL节目的内容是否播放出来?
//@EXECUTIONFLOW:7、PAL/NTSC节目切换，每5秒切换一次，共切换5次，并提示用户注意观察视频变化
//@EXECUTIONFLOW:8、判断是否有EM_UDIPLAYER_VIDEO_SOURCE_CHANGE事件回调上来?
//@EXECUTIONFLOW:9、提示测试人员确认PAL/NTSC节目切换过程中是否正常(音视频有断续，但能播放)?
//@EXECUTIONFLOW:10、删除已注册的回调函数
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0007(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int nLoop = 0;
	PlayerTestServiceIndex_E eIndex = EM_LIVE_MPEG2_PAL;

    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0007:PAL-NTSC制式切换测试(SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts码流)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0007:Test the ability of switching between PAL and NTSC format\n");

	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "步骤1失败");

	//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,fnPlayerCallback1,EM_UDIPLAYER_VIDEO_SOURCE_CHANGE,CSUDI_NULL),	"步骤3失败");
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤4失败");
	CSUDIVIDEOShow(0,TRUE);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤5失败");
	
	CSUDIOSThreadSleep(SLEEPTIME);
	
	CSTCPrint("请确认: 是否已正确播放出PAL制节目的音视频?\n");
	CSTCPrint("Do we get normal PAL format A/V output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤6失败");
	
	CSTCPrint("请注意观察视频变化，下面将进行节目源PAL/NTSC切换，每次持续5秒，共5次 \n");
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
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,eIndex,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iIsSourceEventCallback(SLEEPTIME*5),"步骤8失败");
	}
	
	CSTCPrint("请确认: PAL/NTSC节目切换是否正常(音视频有断续，但能播放出来)\n");
	CSTCPrint("Is the switch between PAL and NTSC normal?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer,fnPlayerCallback1,EM_UDIPLAYER_VIDEO_SOURCE_CHANGE,CSUDI_NULL),"步骤10失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop (hPlayer), "步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤12失败");
		
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试单独播放MPEG1音频的情况
//@PRECONDITION:PLAYER模块，音音频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG1_AV
//@EXPECTATION: 期望播放器能够正常播放出声音
//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试单独播放音频MPEG1的情况
//@PRECONDITION:PLAYER模块，音音频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG1_AV
//@EXPECTATION: 播放器能够正常播放所有的支持的音频
//@EXECUTIONFLOW:1、根据配置文件，前端锁H.264.ts码流对应的频点
//@EXECUTIONFLOW:2、根据H.264.ts流MPEG1节目的音频码流类型(MPEG1音频)，打开音频直播播放器
//@EXECUTIONFLOW:3、根据H.264.ts流MPEG1节目的音频属性(MPEG1音频)设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有只有音频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0008(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

 	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0008:MPEG1音频测试\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0008:Test the ability of playing MPEG1 audio\n");

//根据配置文件上的信息进行锁频

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG1_AV), "步骤1失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG1, INVALIDE_TYPE, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG1_AV,EM_STREAM_AUDIO), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	
	CSTCPrint("音频可以正常播放吗？\n");
    CSTCPrint("Is the audio output normal ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试单独播放MPEG2音频的情况
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: 期望播放器能够正常播放出声音
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流MPEG2节目的音频码流类型(MPEG2音频)，打开音频直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流MPEG2节目的音频属性(MPEG2音频)设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有只有音频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0009(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

 	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0009:MPEG2的标清音频测试\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0009:Test the ability of playing MPEG2 audio\n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, INVALIDE_TYPE, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_AUDIO), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSUDIOSThreadSleep(SLEEPTIME*5);

	CSTCPrint("音频可以正常播放吗？\n");
    CSTCPrint("Is the audio output normal ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");


	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试单独播放AC3音频的情况
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_AC3
//@EXPECTATION: 期望播放器能够正常播放出声音
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流AC3节目的音频码流类型(AC3音频)，打开音频直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流AC3节目的音频属性(AC3音频)设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有只有音频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0010(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AC3))
	{
		CSTCPrint("系统支持AC3解码，期望所有合法设备都有声音输出 \r\n");
        CSTCPrint("The chip surpports decode of AC3 format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("系统不支持AC3解码，期望RCA设备无声音输出 \r\n");
		CSTCPrint("The chip doesn't surpport decode of AC3 format, we expect RCA having no audio output.\r\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0010:AC3音频测试(Audio&Video Test_27Mbps_20070524.ts码流)\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0010:Test the ability of playing AC3 audio\n");
	
//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AC3), "步骤1失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_AC3, EM_UDIPLAYER_LIVE, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_AC3,EM_STREAM_AUDIO), "步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSUDIOSThreadSleep(SLEEPTIME);

	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Does the audio output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试播放MPEG2视频+MPEG2音频 
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT:1、EM_LIVE_MPEG2_AV_SYN
//@INPUT:2、EM_LIVE_MPEG2_MULTI_LAN2
//@INPUT:3、EM_LIVE_MPEG2_ABILITY_TEST1
//@INPUT:4、EM_LIVE_MPEG2_ABILITY_TEST2
//@INPUT:5、EM_LIVE_MPEG2_ABILITY_TEST3
//@EXPECTATION:播放器能够正常播放所有的支持的音视频，正常输出
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流黑白场节目的码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流黑白场节目的音视频属性设置此播放器的码流属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示确认"视频画面白屏的时候，声音是否同时发出"
//@EXECUTIONFLOW:6、调用CSUDIPLAYERModify(),修改流属性，使其播放同一码流中的左右声道节目
//@EXECUTIONFLOW:7、提示确认"音视频是否能正常播放,且左右声道分别为电视和广播的声音?"
//@EXECUTIONFLOW:8、根据配置文件，前端锁Irdeto.PCR.CCITT.ts码流对应的频点
//@EXECUTIONFLOW:9、调用CSUDIPLAYERModify(),修改流属性，使其播放"指标测试"节目
//@EXECUTIONFLOW:10、提示确认"音视频是否能正常播放?"
//@EXECUTIONFLOW:11、调用CSUDIPLAYERModify(),修改流属性，使其播放"PCR间隔为0.01S"节目
//@EXECUTIONFLOW:12、提示确认"音视频是否能正常播放?"
//@EXECUTIONFLOW:13、调用CSUDIPLAYERModify(),修改流属性，使其播放"PCR间隔为0.1S"节目
//@EXECUTIONFLOW:14、提示确认"音视频是否能正常播放?"
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0011(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

 	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0011:MPEG2的音视频测试\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0011:Test the ability of playing MPEG2 grogram \n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AV_SYN), "步骤1失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_AV_SYN,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("视频画面白屏的时候，声音是否同时发出? \r\n");
    CSTCPrint("Is the audio synchronized with the video ?");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN2,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤6失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("音视频是否能正常播放,且左右声道为电视和广播的声音?\r\n");
    CSTCPrint("Is A/V output normal,together with TV sound  in the RCA's left output and broadcast sound in the RCA's rigth output?");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_ABILITY_TEST1), "步骤8失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_ABILITY_TEST1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤9失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("音视频是否能正常播放?\r\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_ABILITY_TEST2,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤11失败");
	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("音视频是否能正常播放?\r\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_ABILITY_TEST3,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤13失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("音视频是否能正常播放?\r\n");
    CSTCPrint("Does the audio output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤14失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤15失败");
	
 	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤16失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试播放MPEG2视频+AC3音频 
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: 1、EM_LIVE_MPEG2_AC3
//@EXPECTATION:播放器能够正常播放所有的支持的音视频，正常输出
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流DolbyAC3标准测试节目的码流类型，打开音频直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流DolbyAC3标准测试节目的码流属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0012(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AC3))
	{
		CSTCPrint("系统支持AC3解码，期望视频可以正常播放，所有合法设备都有声音输出 \r\n");
		CSTCPrint("The chip surpports decode of AC3 format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("系统不支持AC3解码，期望RCA设备无声音输出 \r\n");
		CSTCPrint("The chip doesn't surpport decode of AC3 format, we expect RCA having no audio output.\r\n");
		return CSUDI_FALSE;
	}
	
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0012:MPEG2的视频和AC3的音频测试(Audio&Video Test_27Mbps_20070524.ts码流)\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0012:Test the ability of playing MPEG2 video and AC3 audio at the same time \n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AC3), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_AC3, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_AC3,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");



	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Does the A/V output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	
 	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试播放音视频时传PCR为0和不存在PCR的情况
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_AV_SYN_ERROR_PCR
//@EXPECTATION:播放器能够正常播放所有的支持的音视频，正常输出
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流DolbyAC3标准测试节目的码流类型，打开音频直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流DolbyAC3标准测试节目的码流属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERModify(),修改流属性，使其PCR PID为一个错误值
//@EXECUTIONFLOW:7、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:9、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0013(void)
{
	CSUDIStreamInfo_S  sStreamInfo[3];
	int nStreamCnt;
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0013:测试播放音视频时传PCR为0和不存在PCR的情况\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0013:Test the ability of playing a program with PCR pid set to zero or to one nonexistence in the stream\n");
	
//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_AV_SYN_ERROR_PCR), "步骤1锁频失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_AV_SYN_ERROR_PCR,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");
	
	sStreamInfo[0].m_nPid = g_sPlayerSeviceInfo[EM_LIVE_MPEG2_AV_SYN_ERROR_PCR].m_nVideoPid;
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType = g_sPlayerSeviceInfo[EM_LIVE_MPEG2_AV_SYN_ERROR_PCR].m_eVidStreamType;
	
	sStreamInfo[1].m_nPid = g_sPlayerSeviceInfo[EM_LIVE_MPEG2_AV_SYN_ERROR_PCR].m_nAudioPid;
	sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType = g_sPlayerSeviceInfo[EM_LIVE_MPEG2_AV_SYN_ERROR_PCR].m_eAudStreamType;

	sStreamInfo[2].m_nPid = 0xff;
	sStreamInfo[2].m_eContentType = EM_UDI_CONTENT_PCR;
	nStreamCnt = 3;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERModifyStream(hPlayer,sStreamInfo,nStreamCnt), "步骤6失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤8失败");
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤9失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试播放MPEG2音频+MPEG2视频的时候同时播放teltext的情况
//@PRECONDITION: PLAYER模块，音视频解码器被正常初始化
//@PRECONDITION: 准备可以播放VBI方式的电视机
//@INPUT: EM_LIVE_MPEG2_TELETEX
//@EXPECTATION: 播放器能够正常播放所有的支持的音视频，并且能够正常播放TELETEX图文电视
//@EXECUTIONFLOW:1、根据配置文件，前端锁Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts流包含teltext节目的码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts流包含teltext节目的码流属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来，并且能够正常播放TELETEX图文字幕?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0014(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;

	if (!s_bPlayerSupportTTxt)
	{
		CSTCPrint("不支持图文电视\r \n");
		CSTCPrint("Not supported teltext\r \n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0014:MPEG1的音频和MPEG2的视频,同时播放teltext\n");
	CSTCPrint("请准备好可以播放VBI方式的电视机\r\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0014:Test the ability of playing a program with Teletext context.\n");
		
//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_TELETEX), "步骤1锁频失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_TELETEX,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR|EM_STREAM_TELETEXT),
			"步骤3失败"
		);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("期望播出音视频，并且能够正常播放TELETEX图文电视？\n");
    CSTCPrint("Is A/V output normal together with Teletext ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}
  
//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试不断对一个节目进行停止，启动的动作
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_PAL
//@EXPECTATION:在连续动作的时候播放器仍能正常工作
//@EXECUTIONFLOW:1、根据配置文件，前端锁SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts码流对应的频点
//@EXECUTIONFLOW:2、根据SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts流新加波亚洲新闻台 PAL节目的码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts流新加波亚洲新闻台 PAL节目的码流属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、调用CSUDIOSThreadSleep，sleep1到10秒时间
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop，关闭播放器
//@EXECUTIONFLOW:7、重复步骤4-步骤6,共100次以上
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:9、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:11、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0015(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int i;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0015:不断对一个节目进行停止，启动的动作的测试 \n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0015:Test continuously start and stop playing a program\n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "步骤1失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO),
			"步骤3失败"
		);
	
	for ( i=0 ;i < 100; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
		CSUDIOSThreadSleep(SLEEPTIME);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
		CSTCPrint("压力测试完成%d 次,共100 次。\n", i);
        CSTCPrint("Total times 100. Still has %d times left.",(100-i-1));
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is A/V output normal ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败");
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤11失败");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试不断对一个节目进行创建播放器，设置音视频类型，开启，停止，关闭的动作
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_PAL 
//@EXPECTATION:在一定的工作强度下能够正常工作
//@EXECUTIONFLOW:1、根据配置文件，前端锁SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts码流对应的频点
//@EXECUTIONFLOW:2、根据SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts流新加波亚洲新闻台 PAL节目的码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts流新加波亚洲新闻台 PAL节目的码流属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、调用CSUDIOSThreadSleep，sleep1到10秒时间
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop，关闭播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose，删除播放器，释放资源
//@EXECUTIONFLOW:8、重复步骤2-步骤7,共100次以上
//@EXECUTIONFLOW:9、根据SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts流新加波亚洲新闻台 PAL节目的码流类型，再次打开音视频直播播放器
//@EXECUTIONFLOW:10、根据SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts流新加波亚洲新闻台 PAL节目的码流属性设置此播放器的属性，
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:12、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop，关闭播放器
//@EXECUTIONFLOW:14、提示测试人员观察，音视频播放是否已停止?
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:16、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:17、调用CSUDIPLAYERPause,暂停解码器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、提示测试人员判断音视频是否暂停或停止?
//@EXECUTIONFLOW:19、调用CSUDIPLAYERResume，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:20、提示测试人员判断音视频是否恢复播放?
//@EXECUTIONFLOW:21、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:22、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0016(void)
{	
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	int i;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	void *pData1= CSUDI_NULL,*pData2 = CSUDI_NULL;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0016:音视频各种状态之间反复切换的测试\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0016:Test continuously open,setstream,start and stop a player\n");

	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "步骤1失败");

	pData1 = CSUDIOSMalloc(0x10000);  //测试内存资源交叉分配的情况
	
	//创建播放器
	for ( i=0 ;i < 100; i++ )
	{	
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
		CSTK_ASSERT_TRUE_FATAL(
				CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO),
				"步骤3失败"
			);
		pData2 = CSUDIOSMalloc(0x10000);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

		CSUDIOSFree(pData1);
		pData1 = CSUDI_NULL;
		
		CSUDIOSThreadSleep(SLEEPTIME);

		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");

		pData1 = CSUDIOSMalloc(0x10000);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
		
		CSUDIOSFree(pData2);
		pData2 = CSUDI_NULL;
	}
	
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤9：创建播放器失败");
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO),
			"步骤10失败"
		);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤11：始动失败");
	CSTCPrint("音视频开始播放了吗？\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败：开始播放失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤13失败");
	CSTCPrint("音视频停止播放了吗？\n");
    CSTCPrint("Is A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤14失败：停止播放失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤15：始动失败");
	CSTCPrint("音视频重新开始播放了吗？\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤16失败：开始播放失败");

	udiRe = CSUDIPLAYERPause(hPlayer);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== udiRe),"步骤17失败：播放不正常");

	CSTCPrint("音视频是否再次停止播放了\n");
	CSTCPrint("Does A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤18失败：暂停播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤19失败");
	CSTCPrint("音视频是否再次开始播放了\n");
	CSTCPrint("Does A/V start playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤20失败：开始播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤21失败");
	
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤22失败");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试在一个节目正常播放的时候，用同一参数再次创建播放器的情况
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION:再次调用时返因设备忙，且节目不受影响继续播放
//@REMARK: 主要是测试一个播放器正常播放的时候不能受其他错误调用的影响
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，再次打开音视频直播播放器，期望返回CSUDIPLAYER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:6、调用CSUDIOSThreadSleep，sleep 50ms时间
//@EXECUTIONFLOW:7、重复步骤5-步骤6,共100次以上
//@EXECUTIONFLOW:8、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStop，关闭播放器
//@EXECUTIONFLOW:10、提示测试人员观察，音视频播放是否已停止?
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:12、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:13、调用CSUDIPLAYERPause,暂停解码器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、提示测试人员判断音视频是否暂停或停止?
//@EXECUTIONFLOW:15、调用CSUDIPLAYERResume，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、提示测试人员判断音视频是否恢复播放?
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:18、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0017(void)
{	
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hTmpPlayer = CSUDI_NULL;
	CSUDI_Error_Code  udiRe;
	int i;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0017:音视频播放时再用同一参数创建一个播放器的测试\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0017:Test open a player to play a program and open another player with the same parameters at the same time.\n");
	
	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");

	//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR),
			"步骤3失败"
		);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	
	for ( i=0; i< REPEAT_TIMES; i++)
	{
		udiRe = PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hTmpPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDIPLAYER_ERROR_DEVICE_BUSY  == udiRe && hTmpPlayer == CSUDI_NULL, "步骤5失败");
		CSUDIOSThreadSleep(50);
	}	

	CSTCPrint("音视频在正常播放吗\n");
    CSTCPrint("Does A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤8 失败：开始播放失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤9失败");
	CSTCPrint("音视频停止播放了吗？\n");
    CSTCPrint("Does A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败：停止播放失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤11失败");
	
	CSUDIOSThreadSleep(SLEEPTIME);
	CSTCPrint("音视频开始播放了吗？\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败");

	udiRe = CSUDIPLAYERPause(hPlayer);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== udiRe),"步骤13失败：播放不正常");

	CSTCPrint("音视频是否再次暂停播放了\n");
	CSTCPrint("Does A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤14失败：暂停播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤15失败");
	
	CSTCPrint("音视频是否再次开始播放了\n");
	CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤16失败：开始播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤17失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤18失败");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试在一个节目正常播放的时候对接口进行多次调用的情况
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN2
//@EXPECTATION: 返回非 CSUDI_SUCCESS，节目的播放状态不受影响
//@REMARK: 主要是测试一个播放器正常播放的时候不能受其他错误调用的影响
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复调用CSUDIPLAYERStart，次数至少100次，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、重复调用CSUDIPLAYERResume，次数至少100次，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:8、调用CSUDIPLAYERPause,暂停解码器，期望返回CSUDI_SUCCESS或CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED，如果返回成功则可执行步骤9-步骤12
//@EXECUTIONFLOW:9、重复调用CSUDIPLAYERPause，次数至少100次，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、提示测试人员判断音视频是否暂停或停止?
//@EXECUTIONFLOW:11、调用CSUDIPLAYERResume，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、重复调用CSUDIPLAYERResume，次数至少100次，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:13、提示测试人员观察，音视频是否在播放中?
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、重复调用CSUDIPLAYERStop，次数至少100次，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:16、提示测试人员观察，音视频播放是否已停止?
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:18、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:19、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:20、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0018(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	int i;	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0018:测试在一个节目正常播放的时候对接口进行多次调用的情况\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0018:Test the situation of invoking the player apis many times while playing a program.\n");
	
//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN2), "步骤1失败");
	
//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN2,EM_STREAM_VIDEO|EM_STREAM_AUDIO),
			"步骤3失败"
		);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	for ( i=0 ; i < REPEAT_TIMES; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS !=  CSUDIPLAYERStart(hPlayer), "步骤5失败");
						
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS !=  CSUDIPLAYERResume(hPlayer)), "步骤6失败");

	}
	
	CSTCPrint("音视频开始播放了吗？\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败");

	udiRe = CSUDIPLAYERPause(hPlayer);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== udiRe),"步骤8失败");

	for ( i=0 ; i < REPEAT_TIMES; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERPause(hPlayer), "步骤9失败");
	}
	
	CSTCPrint("音视频暂停播放了吗?\n");
	CSTCPrint("Does A/V pause playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤11失败");

	for ( i=0 ; i < REPEAT_TIMES; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERResume(hPlayer), "步骤12失败");
	}

	CSTCPrint("请确认音视频是否在播放中?\n");
    CSTCPrint("Is A/V playing normally ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤14失败");

	for ( i=0 ; i < REPEAT_TIMES; i++ )
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer), "步骤15失败");
	}
	CSTCPrint("音视频停止播放了吗？\n");
    CSTCPrint("Does A/V stop playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤16失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤17失败");

	CSTCPrint("音视频开始播放了吗？\n");
    CSTCPrint("Does A/V start playing ?\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤18失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤19失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤20失败");
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试播放H.264高清节目和MPEG2节目切换操作 
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化，前端系统需要能同时播放两个码流频点
//@INPUT:1、EM_LIVE_H264_HD
//@INPUT:2、EM_LIVE_MPEG2_PAL
//@EXPECTATION:播放器能够正常播放所有的支持的音视频，正常输出
//@EXECUTIONFLOW:1、根据配置文件，前端锁测试码流对应的频点
//@EXECUTIONFLOW:2、根据测试流,H264高清节目的码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据测试流,H264高清节目的码流属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:8、根据新加波亚洲新闻台 PAL节目的码流属性更新此播放器的属性，
//@EXECUTIONFLOW:9、sleep 5s
//@EXECUTIONFLOW:11、根据H264高清节目的码流属性更新此播放器的属性，
//@EXECUTIONFLOW:12、重复步骤8-步骤12,至少2次以上
//@EXECUTIONFLOW:13、提示测试人员观察，是否仍有音视频播放出来?
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0019(void)
{
	if(-1 != PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_H264))
	{
		CSUDI_HANDLE  hPlayer = CSUDI_NULL;
		int nLoop = 0;
		
		CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0019:播放H.264高清节目和MPEG2节目间切换操作 \n");
        	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0019:Test the situation of swithing between H.264 HD program and MPEG2 SD.\n");
		
		//根据配置文件上的信息进行锁频
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "步骤1失败");

		//创建播放器
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_HD_720P,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

		CSTCPrint("H264高清节目的音视频播放正常?\n");
        CSTCPrint("Is H264 HD program playing normally ?\r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败");
		
		for(nLoop = 0;nLoop < 5;nLoop++)
		{
			if((nLoop%2) == 0) 
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤13失败");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_FALSE,hPlayer,EM_LIVE_H264_HD_720P,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤15失败");
			}
			
			CSUDIOSThreadSleep(SLEEPTIME*5);
		}
		
		CSTCPrint("节目切换操作后音视频是否能正常播放?\r\n");
        CSTCPrint("Is the player still playing normally after the swith?\r\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤14失败：播放不正常");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤15失败");
		
		CSTK_FATAL_POINT;
		
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤16失败");
			hPlayer = CSUDI_NULL;
		}

		return CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("不支持H264视频格式\n");
		CSTCPrint("Not support STREAM_H264\n");
		return CSUDI_FALSE;
	}
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION: 测试播放AC3plus音频
//@PRECONDITION: PLAYER模块，音视频解码器被正常初始化
//@PRECONDITION: 平台存在支持AC3plus音频解码器
//@INPUT: EM_LIVE_AC3PLUS
//@EXPECTATION: 播放器能够正常播放所有的支持的音视频，正常输出
//@EXECUTIONFLOW:1、根据配置文件，前端锁测试码流对应的频点
//@EXECUTIONFLOW:2、根据测试流AC3PLUS节目的音视频码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据测试流AC3PLUS节目的音视频属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，在指定通道是否音视频播放输出
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0020(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;

	if(!PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AC3_PLUS))
	{
		CSTCPrint("系统不支持AC3plus解码 \r\n");
		CSTCPrint("The chip doesn't surpport decode of AC3plus format.\r\n");
		return CSUDI_FALSE;
	}
	
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0020:测试播放H264视频 + AC3plus音频 \n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0020:Test the ability of playing MPEG2 video and AC3_PLUS audio at the same time \n");

	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AC3PLUS), "步骤1失败");

	//创建播放器
	nRet = PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_AC3_PLUS,EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);//, EM_UDI_VID_STREAM_MPEG2

	if(nRet == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_AC3PLUS,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
		
		if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AC3_PLUS))
		{
			CSTCPrint("系统支持AC3plus解码，期望视频可以正常播放，所有合法设备都有声音输出 \r\n");
	        CSTCPrint("The chip surpports decode of AC3_PLUS format,we expect all audio device having normal output.\r\n");
		}
		else
		{
			CSTCPrint("系统不支持AC3plus解码，期望视频可以正常播放，所有合法设备无声音输出 \r\n");
	        CSTCPrint("The chip doesn't surpport decode of AC3plus format, we expect RCA having no audio output.\r\n");
		}
		CSTCPrint("是否为预期结果?\n ");
	    CSTCPrint("Do we get the expectation ?\n ");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	}
	else
	{
		CSTCPrint("系统不支持AC3plus解码 \r\n");
        CSTCPrint("The chip doesn't surpports decode of AC3_PLUS format \r\n");
	}
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
		hPlayer = CSUDI_NULL;
	}		
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试播放AAC音频
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_AAC
//@EXPECTATION: 播放器能够正常播放所有的支持的音视频，正常输出
//@EXECUTIONFLOW:1、根据配置文件，前端锁H.264.ts码流对应的频点
//@EXECUTIONFLOW:2、根据H.264.ts流包含AAC音频节目的音视频码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据H.264.ts流包含AAC音频节目的音视频属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，在指定通道是否音视频播放输出
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0021(void)
{
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;

	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_MPEG_AAC))	
	{
		CSTCPrint("系统支持MPEG_AAC解码，期望视频可以正常播放，所有合法设备都有声音输出 \r\n");
		CSTCPrint("The chip surpports decode of AAC format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("系统不支持MPEG_AAC解码\r\n");
		CSTCPrint("The chip doesn't surpport decode of AAC format.\r\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0021:测试播放H.264视频 + AAC音频 \n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0021:Test the ability of playing H.264 video and AAC audio \n");
	
	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AAC), "步骤1失败");

	//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY,EM_UDI_AUD_STREAM_MPEG_AAC,EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);//qluo:2011-8-21:EM_UDI_AUD_STREAM_MPEG_AAC,EM_UDI_VID_STREAM_MPEG2
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_AAC,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");


	CSTCPrint("是否为预期结果?\n ");
    CSTCPrint("Do we get the expectation ? \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	
	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤6失败");
		hPlayer = CSUDI_NULL;
	}	
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试播放AACplus音频
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_AACPLUS
//@EXPECTATION: 播放器能够正常播放所有的支持的音视频，正常输出
//@EXECUTIONFLOW:1、根据配置文件，前端锁MPEG2_AAC.ts码流对应的频点
//@EXECUTIONFLOW:2、根据EM_LIVE_AACPLUS.ts流包含AACplus音频节目的音视频码流类型，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据EM_LIVE_AACPLUS.ts流包含AACplus音频节目的音视频属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，在指定通道是否音视频播放输出
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0022(void)
{
#if 0
	CSUDI_HANDLE  hPlayer = CSUDI_NULL;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;

	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AAC_PLUS_ADTS))	
	{
		CSTCPrint("系统支持AACPLUS解码，期望视频可以正常播放，所有合法设备都有声音输出 \r\n");
		CSTCPrint("The chip surpports decode of AACPLUS format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("系统不支持AACPLUS解码\r\n");
		CSTCPrint("The chip doesn't surpport decode of AACPLUS format.\r\n");
		return CSUDI_FALSE;
	}
	
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0022:测试播放AACplus音频 \n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0022:Test the ability of playing AACplus audio \n");

	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AACPLUS), "步骤1失败");

	//创建播放器
	nRet = PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_AAC_PLUS_ADTS, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);//根据码流实际情况Editedby Qluo2011-8-22,old value:EM_UDI_VID_STREAM_MPEG2

	if(nRet == CSUDI_SUCCESS)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_AACPLUS,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
		
		if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_AAC_PLUS_ADTS))
		{
			CSTCPrint("系统支持AAC_PLUS解码，期望所有合法设备都有声音输出 \r\n");
			CSTCPrint("The chip surpports decode of AAC_PLUS format,we expect all audio device having normal output.\r\n");
		}
		else
		{
			CSTCPrint("系统不支持AAC_PLUS解码，期望RCA设备无声音输出 \r\n");
			CSTCPrint("The chip doesn't surpport decode of AAC_PLUS format, we expect RCA having no audio output.\r\n");
		}
		CSTCPrint("是否为预期结果?\n ");
		CSTCPrint("Do we get the expectation ? \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");
	    
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	}
	else
	{
		CSTCPrint("系统不支持AAC_PLUS解码 \r\n");
		CSTCPrint("The chip doesn't surpports decode of AAC_PLUS format \r\n");
	}

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤6失败");
		hPlayer = CSUDI_NULL;
	}	
#endif
	CSTCPrint("暂时没有可用的ACCPlus音频码流 \r\n");
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试播放VC1视频
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_VC1
//@EXPECTATION: 播放器能够正常播放所有的支持的音视频，正常输出
//@EXECUTIONFLOW:1、根据配置文件，前端锁VC1_Apollo13_1080p.ts码流对应的频点
//@EXECUTIONFLOW:2、根据VC1_Apollo13_1080p.ts流的视频码流类型，打开视频直播播放器
//@EXECUTIONFLOW:3、根据VC1_Apollo13_1080p.ts流的视频属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否只有视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0023(void)
{
	if(-1 != PLAYER_searchVideoID_Y(EM_UDI_VID_STREAM_VC1))
	{
		CSUDI_HANDLE  hPlayer = CSUDI_NULL;

		CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0023:测试播放VC1视频\n");
        CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0023:Test the ability of playing AACplus audio \n");
	
		//根据配置文件上的信息进行锁频
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_VC1), "步骤1失败");

		//创建播放器
		PLAYER_openPlayerWithType(EM_STREAM_VIDEO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_VC1, EM_UDIPLAYER_LIVE,&hPlayer);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_VC1,EM_STREAM_VIDEO), "步骤3失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

		CSTCPrint("期望视频播放正常\r\n");
        CSTCPrint("Is it playing normally ? \n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
		
		CSTK_FATAL_POINT;
		
		if(hPlayer != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
			hPlayer = CSUDI_NULL;
		}	
		return CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("不支持VC1视频格式 \r\n");
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
//@DESCRIPTION:测试音视频的播放时监听新帧事件
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化,信号量测试已经通过
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: 注册的新帧回调函数能监听到新帧事件，且能在回调中成功移除自身
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、向player模块注册新帧回调函数
//@EXECUTIONFLOW:4、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性，
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，启动播放器 
//@EXECUTIONFLOW:6、检测新帧事件（2秒超时）新帧回调函数中如果收到新帧事件，在回调函数中移除自身，否则返回FALSE 
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStop停止播放器
//@EXECUTIONFLOW:8、调用CSUDIPLAYERClose释放播放器资源
//@EXECUTIONFLOW:9、调用CSUDIOSSemDestroy释放信号量资源
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0024(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   hSem = CSUDI_NULL;
	UserData_S     sUserData = {CSUDI_NULL,CSUDI_FALSE,1};

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0024: 测试对播放器进行控制从而实现音视频的播放(Audio&Video Test_27Mbps_20070524.ts)\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0024: Test the new frame event of the player \n");

	//1根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");
	
	//2创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	//3注册新帧回调
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("newframe",0 , 1, &hSem), "步骤3失败");
	sUserData.phSem = &hSem;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,PlayerEventCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &sUserData),"步骤3失败");

	//4设置播放器属性
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤4失败");

	//5启动播放器
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤5失败");

	//6检测新新帧回调
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(hSem, 2000),"步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == sUserData.bTestPass,"步骤6失败");

	//7停止播放
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤7失败");

	CSTK_FATAL_POINT;

	//8释放player资源
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤8失败");
		hPlayer=CSUDI_NULL;
	}

	//9释放信号量资源
	if(CSUDI_NULL != hSem)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemDestroy(hSem),"步骤9失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:模拟实际播放流程，测试音视频播放时注册两个回调函数监听新帧事件，并重复对其中一个回调重复进行注册移除
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化，信号量测试已经通过
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: 注册的两个新帧回调函数均能监听到新帧事件，且能在回调中成功移除自身
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、向player模块注册两个新帧回调函数
//@EXECUTIONFLOW:4、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:6、检测新帧回调，两个新帧回调函数中如果收到新帧事件，都在回调函数中移除自身，否则返回FALSE
//@EXECUTIONFLOW:7、重复向player模块注册新帧回调AVCtrlCallback 5次，每次都等待新帧到来后移除自身
//@EXECUTIONFLOW:8、每次注册AVCtrlCallback应该均能够收到新帧回调消息且都能够成功移除自身，否则返回FALSE
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStop停止播放器
//@EXECUTIONFLOW:10、调用CSUDIPLAYERClose释放播放器相关资源
//@EXECUTIONFLOW:11、调用CSUDIOSSemDestroy释放信号量资源
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0025(void)
{

	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_HANDLE   ahSem[2] = {0};
	static UserData_S     asUserData[2];
	int				i = 0;
	
	memset(asUserData,0,sizeof(asUserData));

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0025: 测试对播放器进行控制从而实现音视频的播放(Audio&Video Test_27Mbps_20070524.ts)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0025: Test the new frame event of the player \n");

	//1根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");
	
	//2创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败")

	//3向player模块注册两个新帧回调函数
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("newframe1",0 , 1, &ahSem[0]), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("newframe2",0 , 1, &ahSem[1]), "步骤3失败");
	asUserData[0].phSem = &ahSem[0];
	asUserData[1].phSem = &ahSem[1];
	asUserData[0].nId = 1;
	asUserData[1].nId = 2;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,PlayerEventCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &asUserData[0]),"步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,AVCtrlCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &asUserData[1]),"步骤3失败");

	//4设置播放器属性
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤4失败");

	//5启动player
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤5失败");

	//6检测新帧回调
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(ahSem[0], 2000),"步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == asUserData[0].bTestPass,"步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(ahSem[1], 2000),"步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == asUserData[1].bTestPass,"步骤6失败");

	//7重复注册新帧回调的测试
	asUserData[1].bTestPass = CSUDI_FALSE;
	for(i = 0;i < 5;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,AVCtrlCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &asUserData[1]),"步骤7失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(ahSem[1], 2000),"步骤8失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == asUserData[1].bTestPass,"步骤8失败");

		CSUDIOSThreadSleep(100);
		
		if(CSUDI_TRUE  != asUserData[1].bTestPass)
		{
			break;
		}
	}

	//9停止播放器
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤9失败");

	CSTK_FATAL_POINT;

	//10释放播放器相关资源
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤10失败");
		hPlayer=CSUDI_NULL;
	}

	//11释放信号量相关资源
	if(CSUDI_NULL != ahSem[0])
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemDestroy(ahSem[0]),"步骤11失败");
	}

	if(CSUDI_NULL != ahSem[1])
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemDestroy(ahSem[1]),"步骤11失败");
	}
	
	
	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:检测播放过程音视频解码包个数是否正常变化
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化，信号量测试已经通过
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: 获取到的解码包的个数音视频发生变化
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、向player模块注册新帧回调函数
//@EXECUTIONFLOW:4、根据Audio&Video Test_27Mbps_20070524.ts流的属性设置此播放器的属性，
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:6、检测新帧事件
//@EXECUTIONFLOW:7、每隔1s钟去获取视频包和音频包解码个数与上一次进行比较，共比较5次,每一次音视频包个数都应该发生变化,否则返回FALSE
//@EXECUTIONFLOW:8、停止播放器
//@EXECUTIONFLOW:9、释放播放器相关资源
//@EXECUTIONFLOW:10、释放信号量相关资源
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

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0026: 测试对播放器进行控制从而实现音视频的播放(Audio&Video Test_27Mbps_20070524.ts)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0026: Test the PacketCount of the player \n");

	//1根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");
	
	//2创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	//3注册新帧回调
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("newframe",0 , 1, &hSem), "步骤3失败");
	sUserData.phSem = &hSem;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer,PlayerEventCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &sUserData),"步骤3失败");

	//4设置播放器属性
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤4失败");

	//5启动player
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤5失败");
	CSUDIOSThreadSleep(2000);//等2s再开始检测，因为播放到播放成功可能需要一定时间，这个时间段内解码包个数可能不变	


	//6检测新新帧回调
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemWait(hSem, 2000),"步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == sUserData.bTestPass,"步骤6失败");
	
	//7检测音视频包的个数是否发生变化
	while(i < 5)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_VIDEO,&nVideoCount), "步骤7失败")
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERGetPacketCount(hPlayer,EM_UDI_CONTENT_AUDIO,&nAudioCount), "步骤7失败")
		CSTK_ASSERT_TRUE_FATAL(nPreVideoCount != nVideoCount || nPreAudioCount != nAudioCount,"步骤7失败");

		nPreAudioCount = nVideoCount;
		nPreVideoCount = nAudioCount;
		i++;

		CSUDIOSThreadSleep(1000);
	}

	//8停止播放器
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤8失败");

	CSTK_FATAL_POINT;

	//9释放播放器相关资源
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤9失败");
		hPlayer=CSUDI_NULL;
	}

	//10释放信号量等资源
	if(CSUDI_NULL != hSem)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSSemDestroy(hSem),"步骤10失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:PLAYER           
//@DESCRIPTION:测试播放H264视频+MP3音频 
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MP3
//@EXPECTATION: 播放器能够正常播放所有的支持的音视频，正常输出
//@EXECUTIONFLOW:1、根据配置文件，前端锁H.264.ts码流对应的频点
//@EXECUTIONFLOW:2、根据H.264.ts流MP3节目的音频码流类型(MP3音频)，打开音视频直播播放器
//@EXECUTIONFLOW:3、根据H.264.ts流MP3节目的音频属性(MP3音频)设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0027(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	if(PLAYER_iIsAudioSupportDecodeType(EM_UDI_AUD_STREAM_MP3))
	{
		CSTCPrint("系统支持EM_UDI_AUD_STREAM_MP3解码，期望所有合法设备都有声音输出 \r\n");
        CSTCPrint("The chip surpports decode of EM_UDI_AUD_STREAM_MP3 format,we expect all audio device having normal output.\r\n");
	}
	else
	{
		CSTCPrint("系统不支持EM_UDI_AUD_STREAM_MP3解码 \r\n");
		CSTCPrint("The chip doesn't surpport decode of EM_UDI_AUD_STREAM_MP3 format.\r\n");
		return CSUDI_FALSE;
	}

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("系统支持H264解码\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("系统不支持H264解码\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}

	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0027:EM_UDI_AUD_STREAM_MP3音频测试(H.264.ts码流)\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_LIVE_0027:Test the ability of playing MP3 audio\n");
	
//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MP3), "步骤1失败");
	
//创建播放器
	
	PLAYER_openPlayerWithType(EM_STREAM_VIDEO|EM_STREAM_AUDIO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MP3, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MP3,EM_STREAM_AUDIO|EM_STREAM_VIDEO), "步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");

	CSUDIOSThreadSleep(SLEEPTIME);

	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Is A/V output normal ? ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤7失败");
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
		 	CSTCPrint("创建事件失败\n");
			
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
		 	CSTCPrint("创建事件失败\n");
			
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
		CSTCPrint("参数错误\n");
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
		CSTCPrint("参数错误\n");
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
		CSTCPrint("参数错误\n");
		return 0;
	}
}


//指定事件类型的回调函数
static void fnPlayerNewFrameEvent(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{

	if( (eEvent==EM_UDIPLAYER_VIDEO_FRAME_COMING) && (pvUserData == NEW_FRAME_AT_SWITCHING))
	{
		CS_iSetTiming(EM_TIMING_NEW_FRAME_COMING);

		if (notify_for_siwtch_event() != CSUDI_SUCCESS)
		{
			CSTCPrint("第一帧事件通知失败%s, %d\n",__FILE__,__LINE__);
		}
	}
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:测试同频点同类型节目(MPEG2视频)之间切换的时间
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1
//@EXPECTATION: 清流MPEG2之间的切换平均时间在1.2秒以内较为合适，超过1.2秒需要优化
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERAddPlayerCallback注册新帧回调
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:6、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream，在"多语言AC3MP2"和"多语言左右声道"节目之间切换
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:10、等待第一帧上来事件，第一帧上来后计算并显示节目切换时间和平均节目切换时间，期望平均切台时间在1.2秒内
//@EXECUTIONFLOW:11、期望新帧事件能够在500ms内上来，否则测试失败
//@EXECUTIONFLOW:12、重复步骤5-步骤11,10次以上
//@EXECUTIONFLOW:13、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:15、调用CSUDIPLAYERDelPlayerCallback删除回调
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0001(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe= CSUDI_SUCCESS;
	
	int i=0,switchtime,totalTime=0;

 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0001:测试同频点同类型节目(MPEG2标清视频)之间切换的时间\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0001:Test the time of switching programs in the same frequence .\n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO,EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤3失败");

	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤5失败");
	
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤6失败：播放不正常");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤7失败");
				
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL,EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤8-1失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_NTSC, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤8-2失败");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
			CSTCPrint("第%d 次切换时间为%d毫秒:,平均切换时间为:%d毫秒\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
		}
		else
		{
			totalTime += 5000;
			CSTCPrint("第%d 次切换在5000毫秒内没有收到新帧事件\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "切台在5000ms内未收到新帧事件");
		}
	}

	CSTCPrint("%d 次平均切换时间为:%d毫秒(同频点同类型(MPEG2)节目切换，要求不超过1200毫秒)\n", i, totalTime/i);
    CSTCPrint("The average switching time is: %d(MS) with %d times\n",totalTime/i, i);
	CSTK_ASSERT_TRUE_FATAL((totalTime/i <= 1200), "平均切台时间超过1200毫秒，切台速度需要优化");
	
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤14失败");	

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤16失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:测试跨频点，同类型节目(MPEg2)之间切换的时间
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_MPEG2_PAL
//@EXPECTATION: 清流MPEG2之间跨频点切换平均时间在1.5秒以内较为合适，超过1.5秒需要优化
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERAddPlayerCallback注册新帧回调
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:6、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:8、根据配置文件，前端锁Player_PAL_NTSC码流对应的频点
//@EXECUTIONFLOW:9、根据Player_PAL_NTSC流,"新加波亚洲新闻台 PAL"节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:11、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:13、前端锁频(频点对应Audio&Video Test_27Mbps_20070524.ts/SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts码流)
//@EXECUTIONFLOW:14、根据所锁的频点和节目信息，设置播放器码流属性
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:16、等待第一帧上来事件，第一帧上来后计算并显示节目切换时间和平均节目切换时间
//@EXECUTIONFLOW:17、期望新帧事件能够在500ms内上来，否则测试失败
//@EXECUTIONFLOW:18、重复步骤12-步骤17,10次以上
//@EXECUTIONFLOW:19、期望平均切台时间在1.5秒以内
//@EXECUTIONFLOW:20、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:21、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:22、调用CSUDIPLAYERDelPlayerCallback删除回调
//@EXECUTIONFLOW:23、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0002(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe= CSUDI_SUCCESS;
	
	int i=0,switchtime,totalTime=0;
	int nFreqShare = -1;
	char acValue[32] = {0};
	
 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0002:测试跨频点同类型节目(MPEg2)之间切换的时间\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0002:Test the time of switching programs at different frequences .\n");
	CSTCPrint("本用例所需测试码流为 %s 和 %s \n", g_sPlayerSeviceInfo[EM_LIVE_MPEG2_MULTI_LAN1].m_pcTsFilename, g_sPlayerSeviceInfo[EM_LIVE_MPEG2_PAL].m_pcTsFilename);

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_DEV_SHARE", acValue, sizeof(acValue)))
	{
		nFreqShare = CSTKGetIntFromStr(acValue,10);
	}

	CSTK_ASSERT_TRUE_FATAL(nFreqShare == 0, "执行本用例需将配置文件中FREQ_DEV_SHARE配置为0(码流设备不共享)，并将所需测试码流在不同频点播放");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");

	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤5失败");
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤6失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤7失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "步骤8失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤9失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤10失败");	
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败：播放不正常");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");
				
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤13-1失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤14-1失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "步骤13-2失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤14-2失败");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤15失败");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
			CSTCPrint("第%d次切换时间为%d毫秒:,平均切换时间为:%d毫秒\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
		}
		else
		{
			totalTime += 5000;
			CSTCPrint("第%d 次切换在5000毫秒内没有收到新帧事件\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "切台在5000ms内未收到新帧事件");
		}
	}

	CSTCPrint("%d 次平均切换时间为:%d毫秒(跨频点同类型(MPEG2)节目切换，要求不超过1500毫秒)\n", i, totalTime/i);
    CSTCPrint("The average switching time is: %d(MS) with %d times\n",totalTime/i, i);
	CSTK_ASSERT_TRUE_FATAL((totalTime/i <= 1500), "平均切台时间超过1500毫秒，切台速度需要优化");
	
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤20失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤21失败");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤23失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:测试同频点H.264视频类型节目之间切换的时间
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_H264_HD_720P
//@EXPECTATION: 清流H264标清之间的切换平均时间在2.2秒以内较为合适，超过2.2秒需要优化
//@EXECUTIONFLOW:1、根据配置文件，前端锁SD_PAL_NTSC_and_HD_H.264_1080i_720P_merge.ts码流对应的频点
//@EXECUTIONFLOW:2、根据码流设置H264高清720P节目的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据码流设置H264高清720P节目的播放参数，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream，在H264 高清720P和高清1080i节目之间切换
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:9、等待第一帧上来事件，第一帧上来后计算并显示节目切换时间和平均节目切换时间
//@EXECUTIONFLOW:10、期望新帧事件能够在500ms内上来，否则测试失败
//@EXECUTIONFLOW:11、重复步骤6-步骤10,10次以上
//@EXECUTIONFLOW:12、期望平均切台时间在2.2秒以内
//@EXECUTIONFLOW:13、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0003(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe= CSUDI_SUCCESS;
	
	int i=0,switchtime,totalTime=0;

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("系统支持H264解码\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("系统不支持H264解码\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	

	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0003:测试同频点H.264视频类型节目之间切换的时间\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0003:Test the time of switching H.264 programs in the same frequence .\n");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE,&hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_HD_720P, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤3失败");

	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
				
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_HD_720P,EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤7-1失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_HD_1080i, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤7-2失败");

		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
			CSTCPrint("第%d次切换时间为%d毫秒:,平均切换时间为:%d毫秒\n", i+1, switchtime,(totalTime/(i+1)));
			CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
        }
        else
        {
			totalTime += 5000;
			CSTCPrint("第%d次切换在5000毫秒内没有收到新帧事件\n", i+1);
			CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "切台在5000ms内未收到新帧事件");
        }
		
	}

	CSTCPrint("%d 次平均切换时间为:%d毫秒(同频点同类型(H.264)节目切换，要求不超过2200毫秒)\n", i, totalTime/i);
    CSTCPrint("The average switching time is: %d(MS) with %d times\n",totalTime/i, i);
	CSTK_ASSERT_TRUE_FATAL((totalTime/i <= 2200), "平均切台时间超过2200毫秒，切台速度需要优化");
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤14失败");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤15失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:测试跨频点，不同类型节目(H264,MPEG2)之间切换的时间
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_H264_SD
//@EXPECTATION: 清流MPEG2/H264之间跨频点切换平均时间在2.0秒以内较为合适，超过2.0秒需要优化
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、根据配置文件，前端锁mpeg2hd+h264sd.ts码流对应的频点
//@EXECUTIONFLOW:8、根据mpeg2hd+h264sd.ts流,H264标清节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:10、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:12、前端锁频(频点对应Audio&Video Test_27Mbps_20070524.ts/mpeg2+h264_8M.trp码流)
//@EXECUTIONFLOW:13、根据所锁的频点和节目信息，设置播放器码流属性
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:15、等待第一帧上来事件，第一帧上来后计算并显示节目切换时间和平均节目切换时间
//@EXECUTIONFLOW:16、期望新帧事件能够在500ms内上来，否则测试失败
//@EXECUTIONFLOW:17、重复步骤11-步骤16,10次以上
//@EXECUTIONFLOW:18、期望平均切台时间在2.0秒以内
//@EXECUTIONFLOW:19、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:20、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:21、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0004(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe= CSUDI_SUCCESS;
	int i=0,switchtime,totalTime=0;
	int nFreqShare = -1;
	char acValue[32] = {0};

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("系统支持H264解码\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("系统不支持H264解码\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}

 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0004:测试跨频点不同类型节目(H264,MPEG2)之间切换的时间\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0004:Test the time of switching different type(MPEG2&H.264) of programs at different frequences .\n");
	CSTCPrint("本用例所需测试码流为 %s 和 %s \n", g_sPlayerSeviceInfo[EM_LIVE_MPEG2_MULTI_LAN1].m_pcTsFilename, g_sPlayerSeviceInfo[EM_LIVE_H264_SD].m_pcTsFilename);

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_DEV_SHARE", acValue, sizeof(acValue)))
	{
		nFreqShare = CSTKGetIntFromStr(acValue,10);
	}

	CSTK_ASSERT_TRUE_FATAL(nFreqShare == 0, "执行本用例需将配置文件中FREQ_DEV_SHARE配置为0(码流设备不共享)，并将所需测试码流在不同频点播放");
 	
//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");
	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "步骤7失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤8失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");	
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败：播放不正常");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");
		
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤12-1失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤13-1失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "步骤12-2失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤13-2失败");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤14失败");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
			CSTCPrint("第%d次切换时间为%d毫秒:,平均切换时间为:%d毫秒\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
        }
        else
        {
            totalTime += 5000;
            CSTCPrint("第%d次切换在5000毫秒内没有收到新帧事件\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "切台在5000ms内未收到新帧事件");
        }

	}

	CSTCPrint("%d 次平均切换时间为:%d毫秒(跨频点不同类型(H264,MPEG2)节目切换，要求不超过2000毫秒)\n", i, totalTime/i);
    CSTCPrint("The average switching time is: %d(MS) with %d times\n",totalTime/i, i);
	CSTK_ASSERT_TRUE_FATAL((totalTime/i <= 2000), "平均切台时间超过2000毫秒，切台速度需要优化");
	
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤19失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤20失败");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤21失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

#define PLAYER_STRESS_TESTTIME (10000)

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:跨频点，同类型节目(MPEg2)之间正常切换的老化测试
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_MPEG2_PAL
//@EXPECTATION: 清流MPEG2之间跨频点切换平均时间在1.5秒以内较为合适，超过1.5秒需要优化
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、根据配置文件，前端锁Player_PAL_NTSC码流对应的频点
//@EXECUTIONFLOW:8、根据Player_PAL_NTSC流,"新加波亚洲新闻台 PAL"节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:10、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:12、前端锁频(频点对应Audio&Video Test_27Mbps_20070524.ts/SD_PAL_NTSC_and_HD_H.264_1080i_720P_merge.ts码流)
//@EXECUTIONFLOW:13、根据所锁的频点和节目信息，设置播放器码流属性
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:15、等待第一帧上来事件，第一帧上来后计算并显示节目切换时间和平均节目切换时间
//@EXECUTIONFLOW:16、重复步骤11-步骤15,10000次以上
//@EXECUTIONFLOW:17、判断平均切换节目的时间，期望在1.5s以内
//@EXECUTIONFLOW:18、提示测试人员观察，多次切换后，音视频是否还能正常播放出来?
//@EXECUTIONFLOW:19、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:20、调用CSUDIPLAYERClose删除播放器
//@REMARK:这个测试用例需要很长时间才能执行完成，不适合于短时间测试
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0005(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	int i=0,switchtime,totalTime=0;
	int nTimeoutNum = 0;
	int nFreqShare = -1;
	char acValue[32] = {0};

 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0005:跨频点同类型节目(MPEg2)之间正常切换的老化测试，这个测试用例执行完成将需要很长时间(6个小时以上)\n");	
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0005:Test the time of switching (MPEG2) programs at different frequences .\n");
    CSTCPrint("NOTE: The testcase will take about six hours!!!!\r\n");
	CSTCPrint("本用例所需测试码流为 %s 和 %s \n", g_sPlayerSeviceInfo[EM_LIVE_MPEG2_MULTI_LAN1].m_pcTsFilename, g_sPlayerSeviceInfo[EM_LIVE_MPEG2_PAL].m_pcTsFilename);

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_DEV_SHARE", acValue, sizeof(acValue)))
	{
		nFreqShare = CSTKGetIntFromStr(acValue,10);
	}

	CSTK_ASSERT_TRUE_FATAL(nFreqShare == 0, "执行本用例需将配置文件中FREQ_DEV_SHARE配置为0(码流设备不共享)，并将所需测试码流在不同频点播放");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"未准备好开始测试");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");
	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "步骤7失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤8失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");	
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败：播放不正常");

	for (i=0;i<PLAYER_STRESS_TESTTIME;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");
		
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤12-1失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤13-1失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "步骤12-2失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤13-2失败");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤14失败");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
            CSTCPrint("第%d次切换时间为%d毫秒:,平均切换时间为:%d毫秒\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
        }
        else
        {
            totalTime += 5000;
            CSTCPrint("第%d次切换在5000毫秒内没有收到新帧事件\n", i+1);
            CSTCPrint("We don't get video output within 5000(MS) at the %dth time\n", i+1);
			
			nTimeoutNum++;
			CSTK_ASSERT_TRUE_FATAL((nTimeoutNum < 10), "多次出现5000毫秒内没有收到新帧事件的情况");
        }

	}

	CSTCPrint("平均切换时间为:%d毫秒(跨频点同类型(MPEg2)节目切换，要求不超过1500毫秒)\n", (totalTime/PLAYER_STRESS_TESTTIME));
	CSTK_ASSERT_TRUE_FATAL(((totalTime/PLAYER_STRESS_TESTTIME) <= 1500), "平均切台时间超过1500毫秒，切台速度需要优化");
	
	CSTCPrint("多次切换后，音视频还可以正常播放吗(没有出现花屏、死机、卡住等现象)？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤18失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤19失败");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤20失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:跨频点，不同类型节目(H264,MPEG2)之间切换的老化测试
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_H264_SD
//@EXPECTATION: 清流MPEG2/H264之间跨频点切换平均时间在2.0秒以内较为合适，超过2.0秒需要优化
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、根据配置文件，前端锁mpeg2+h264_8M.trp码流对应的频点
//@EXECUTIONFLOW:8、根据mpeg2hd+h264sd.ts流,"新加波亚洲新闻台 PAL"节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:10、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:12、前端锁频(频点对应Audio&Video Test_27Mbps_20070524.ts/mpeg2hd+h264sd.ts码流)
//@EXECUTIONFLOW:13、根据所锁的频点和节目信息，设置播放器码流属性
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:15、等待第一帧上来事件，第一帧上来后计算并显示节目切换时间和平均节目切换时间
//@EXECUTIONFLOW:16、重复步骤11-步骤15,10000次以上
//@EXECUTIONFLOW:17、判断平均切换节目的时间，期望在2.0s以内
//@EXECUTIONFLOW:18、提示测试人员观察，多次切换后，音视频是否还能正常播放出来?
//@EXECUTIONFLOW:19、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:20、调用CSUDIPLAYERClose删除播放器
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
		CSTCPrint("系统支持H264解码\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("系统不支持H264解码\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	
 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0006:跨频点不同类型节目(H264,MPEG2)之间切换的老化测试，这个测试用例执行完成将需要很长时间(6个小时以上)\n");	
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0006:Test the time of switching different type(MPEG2&H.264) of programs at different frequences .\n");
    CSTCPrint("NOTE: This a Stress testcase!It will take about six hours!!!!\r\n");
	CSTCPrint("本用例所需测试码流为 %s 和 %s \n", g_sPlayerSeviceInfo[EM_LIVE_MPEG2_MULTI_LAN1].m_pcTsFilename, g_sPlayerSeviceInfo[EM_LIVE_H264_SD].m_pcTsFilename);

	if(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","FREQ_DEV_SHARE", acValue, sizeof(acValue)))
	{
		nFreqShare = CSTKGetIntFromStr(acValue,10);
	}

	CSTK_ASSERT_TRUE_FATAL(nFreqShare == 0, "执行本用例需将配置文件中FREQ_DEV_SHARE配置为0(码流设备不共享)，并将所需测试码流在不同频点播放");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"未准备好开始测试");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");
	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "步骤7失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤8失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");	
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败：播放不正常");

	for (i=0;i<PLAYER_STRESS_TESTTIME;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");
		
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤12-1失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤13-1失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "步骤12-2失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤13-2失败");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤14失败");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH,EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
            CSTCPrint("第%d次切换时间为%d毫秒:,平均切换时间为:%d毫秒\n", i+1, switchtime,(totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n",i+1, switchtime,(totalTime/(i+1)));
        }
        else
        {
            totalTime += 5000;
            CSTCPrint("第%d次切换在5000毫秒内没有收到新帧事件\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);

			nTimeoutNum++;
			CSTK_ASSERT_TRUE_FATAL((nTimeoutNum < 10), "多次出现5000毫秒内没有收到新帧事件的情况");
        }

	}
	
	CSTCPrint("平均切换时间为:%d毫秒(跨频点不同类型(H264,MPEG2)节目切换，要求不超过2000毫秒)\n", (totalTime/PLAYER_STRESS_TESTTIME));
	CSTK_ASSERT_TRUE_FATAL(((totalTime/PLAYER_STRESS_TESTTIME) <= 2000), "平均切台时间超过2000毫秒，切台速度需要优化");
	
	CSTCPrint("多次切换后，音视频还可以正常播放吗(没有出现花屏、死机、卡住等现象)？\n");
    CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤18失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤19失败");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤20失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:同频点，同类型节目(MPEg2)之间正常切换的老化测试
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_MPEG2_PAL
//@EXPECTATION: 清流MPEG2之间同频点切换平均时间在1秒以内较为合适，超过1秒需要优化
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、根据Audio&Video Test_27Mbps_20070524.ts流,多语言左右声道节目的码流类型，打开直播播放器
//@EXECUTIONFLOW:8、根据Audio&Video Test_27Mbps_20070524.ts流,多语言左右声道节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:10、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:12、前端锁频(频点对应Audio&Video Test_27Mbps_20070524.ts码流)
//@EXECUTIONFLOW:13、根据所锁的频点和节目信息，设置播放器码流属性
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:15、等待第一帧上来事件，第一帧上来后计算并显示节目切换时间和平均节目切换时间
//@EXECUTIONFLOW:16、重复步骤11-步骤15,10000次以上
//@EXECUTIONFLOW:17、判断平均切换节目的时间，期望在1.0s以内
//@EXECUTIONFLOW:18、提示测试人员观察，多次切换后，音视频是否还能正常播放出来?
//@EXECUTIONFLOW:19、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:20、调用CSUDIPLAYERClose删除播放器
//@REMARK:这个测试用例需要很长时间才能执行完成，不适合于短时间测试
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0007(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	int i=0,switchtime,totalTime=0;
	int nTimeoutNum = 0;

 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0007:同频点同类型节目(MPEg2)之间正常切换的老化测试，这个测试用例执行完成将需要很长时间(6个小时以上)\n");	
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0007:Test the time of switching (MPEG2) programs at different frequences .\n");
    CSTCPrint("NOTE: The testcase will take about six hours!!!!\r\n");
	
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"未准备好开始测试");

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE, &hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");
	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN2), "步骤7失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN2, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤8失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "步骤9失败");	
	CSTCPrint("音视频可以正常播放吗？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败：播放不正常");

	for (i=0;i<PLAYER_STRESS_TESTTIME;i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");

		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤12-1失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤13-1失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤12-1失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_MPEG2_MULTI_LAN2, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤13-2失败");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "步骤14失败");

		udiRe = wait_for_siwtch_event(5000);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			switchtime = CS_iGetTiming(EM_TIMING_BEFORE_SWITCH, EM_TIMING_NEW_FRAME_COMING);
			
			totalTime += switchtime;
			
            CSTCPrint("第%d次切换时间为%d毫秒:,平均切换时间为:%d毫秒\n", i+1, switchtime, (totalTime/(i+1)));
            CSTCPrint("The %dth time of switching takes %d(MS),The average switching time is: %d(MS)\n", i+1, switchtime, (totalTime/(i+1)));
        }
        else
        {
            totalTime += 5000;
            CSTCPrint("第%d次切换在5000毫秒内没有收到新帧事件\n", i+1);
            CSTCPrint("We don't get new frame coming event within 5000(MS) at the %dth time\n", i+1);
			
			nTimeoutNum++;
			CSTK_ASSERT_TRUE_FATAL((nTimeoutNum < 10), "多次出现5000毫秒内没有收到新帧事件的情况");
        }

	}

	CSTCPrint("平均切换时间为:%d毫秒(同频点同类型(MPEG2)节目切换，要求不超过1000毫秒)\n", (totalTime/PLAYER_STRESS_TESTTIME));
	CSTK_ASSERT_TRUE_FATAL(((totalTime/PLAYER_STRESS_TESTTIME) < 1000), "平均切台时间超过1000毫秒，切台速度需要优化");
	
	CSTCPrint("多次切换后，音视频还可以正常播放吗(没有出现花屏、死机、卡住等现象)？\n");
    CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤18失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤19失败");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤20失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:跨频点，不同类型节目(H264,MPEG2)之间快速切换的老化测试
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_LIVE_MPEG2_MULTI_LAN1&EM_LIVE_H264_SD
//@EXPECTATION: 清流MPEG2/H264之间跨频点切换平均时间在2.0秒以内较为合适，超过2.0秒需要优化
//@EXECUTIONFLOW:1、根据配置文件，前端锁Audio&Video Test_27Mbps_20070524.ts码流对应的频点
//@EXECUTIONFLOW:2、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的码流类型，打开直播播放器
//@EXECUTIONFLOW:3、根据Audio&Video Test_27Mbps_20070524.ts流,多语言AC3MP2节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:5、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:7、根据配置文件，前端锁mpeg2+h264_8M.trp码流对应的频点
//@EXECUTIONFLOW:8、根据mpeg2hd+h264sd.ts流,"新加波亚洲新闻台 PAL"节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:10、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:12、前端锁频(频点对应Audio&Video Test_27Mbps_20070524.ts/mpeg2hd+h264sd.ts码流)
//@EXECUTIONFLOW:13、根据所锁的频点和节目信息，设置播放器码流属性
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:15、sleep 80毫秒
//@EXECUTIONFLOW:16、重复步骤11-步骤15,10000次以上
//@EXECUTIONFLOW:17、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:18、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:19、调用CSUDIPLAYERClose删除播放器
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0008(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
	int i=0;

	if(PLAYER_iIsVideoSupportDecodeType(EM_UDI_VID_STREAM_H264))
	{
		CSTCPrint("系统支持H264解码\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("系统不支持H264解码\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	
 	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0008:跨频点不同类型节目(H264,MPEG2)之间快速切换的老化测试，这个测试用例执行完成将需要很长时间(6个小时以上，请注意一个小时后观察音视频的变化)\n");
    CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0008:Test quick switching between different type(MPEG2&H.264) of programs at different frequences .\n");
    CSTCPrint("NOTE: This a Stress testcase!It will take about six hours!!!!\r\n");

	CSTKWaitAnyKey();

//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤1失败");

//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_MPEG2, EM_UDIPLAYER_LIVE,&hPlayer);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "步骤7失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤8失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败：播放不正常");

	for (i=0;i<PLAYER_STRESS_TESTTIME;i++)
	{
		CSTCPrint("共进行了%d次切换\n",i+1);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_MULTI_LAN1), "步骤12-1失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_MPEG2_MULTI_LAN1, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤13-1失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_SD), "步骤12-2失败");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE,hPlayer,EM_LIVE_H264_SD, EM_STREAM_VIDEO|EM_STREAM_AUDIO), "步骤13-2失败");
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤14失败");

		CSUDIOSThreadSleep(80);

	}
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤17失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤18失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤19失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:测试同频点高清h.264与标清mpeg2之间切台的时间
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: EM_UDI_VID_STREAM_H264,EM_LIVE_MPEG2_PAL
//@EXPECTATION: 1.标清MPEG2切换到高清h.264的时间小于等于1.5s,如果不满足建议优化
//@EXPECTATION: 1.高清h.264切换到标清MPEG2的时间小于等于500ms,如果不满足建议优化
//@EXECUTIONFLOW:1、根据配置文件，前端锁SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts码流对应的频点
//@EXECUTIONFLOW:2、打开支持H.264的直播播放器,期望返回成功
//@EXECUTIONFLOW:3、根据码流VES2节目的属性设置此播放器的属性，
//@EXECUTIONFLOW:4、调用CSUDIPLAYERAddPlayerCallback注册新帧回调
//@EXECUTIONFLOW:5、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:6、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:7、记录开始时间
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:9、调用CSUDIPLAYERSetStream，在"VES2"和"CNAI PAL"节目之间切换
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStart，启动播放器
//@EXECUTIONFLOW:11、等待第一帧上来事件，第一帧上来后计算并显示节目切换时间和平均节目切换时间，期望平均切台时间在1.2秒内
//@EXECUTIONFLOW:12、期望新帧事件能够在500ms内上来，否则测试失败
//@EXECUTIONFLOW:13、重复步骤7-步骤12,20次
//@EXECUTIONFLOW:14、提示测试人员观察，是否有音视频播放出来?
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:16、调用CSUDIPLAYERDelPlayerCallback删除回调
//@EXECUTIONFLOW:17、调用CSUDIPLAYERClose删除播放器
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
		CSTCPrint("系统支持H264解码\r\n");
		CSTCPrint("The chip surpports decode of h264 format\r\n");
	}
	else
	{
		CSTCPrint("系统不支持H264解码\r\n");
		CSTCPrint("The chip doesn't surpport decode of H264 format\r\n");
		return CSUDI_FALSE;
	}
	

	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0003:测试同频点H.264视频类型节目之间切换的时间\n");
	CSTCPrint("CSTC_LIVE_TEST_IT_SWITCH_0003:Test the time of switching H.264 programs in the same frequence .\n");

	//根据配置文件上的信息进行锁频
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_H264_HD_720P), "步骤1失败");

	//创建播放器
	PLAYER_openPlayerWithType(EM_STREAM_AUDIO|EM_STREAM_VIDEO, EM_UDI_DEMUX_PLAY, EM_UDI_AUD_STREAM_MPEG2, EM_UDI_VID_STREAM_H264, EM_UDIPLAYER_LIVE, &hPlayer);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != hPlayer, "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_H264_HD_720P, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤3失败");

	CSUDIPLAYERAddPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤4失败");
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Is it playing normally ?\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败：播放不正常");

	for (i=0; i<REPEAT_TIMES*2; i++)
	{
		CS_iSetTiming(EM_TIMING_BEFORE_SWITCH);
		
		CS_iResetTiming(EM_TIMING_NEW_FRAME_COMING);
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤6失败");
				
		if (i%2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_H264_HD_720P, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤7-1失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iSetStream(CSUDI_TRUE, hPlayer, EM_LIVE_MPEG2_PAL, EM_STREAM_VIDEO|EM_STREAM_AUDIO|EM_STREAM_PCR), "步骤7-2失败");

		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "步骤8失败");

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
			
			CSTCPrint("第%d次%s 切换时间为%d毫秒\n", i+1,(i%2)?"标清->高清":"高清->标清",switchtime);
			CSTCPrint("The %dth time of switching takes %d(MS) from %s\n",i+1, switchtime,(i%2)?"SD->HD":"HD->SD");
	        }
	        else
	        {
	        		if (i%2)
				{
					totalTime_SD2HD += nWaitTimeSD2HD;
					CSTCPrint("第%d 次标清->高清切换在%d毫秒内没有收到新帧事件\n", i+1,nWaitTimeSD2HD);
					CSTCPrint("We don't get new frame coming event within %d(MS) at the %dth time from SD->HD\n", nWaitTimeSD2HD,i+1);
				}
				else
				{
					totalTime_HD2SD += nWaitTimeHD2SD;
					CSTCPrint("第%d 次高清->标清切换在%d毫秒内没有收到新帧事件\n", i+1,nWaitTimeHD2SD);
					CSTCPrint("We don't get new frame coming event within %d(MS) at the %dth time from HD->SD\n", nWaitTimeHD2SD,i+1);
				}
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "切台在规定时间内未收到新帧事件");
	        }
	}

	nAvgTimeSD2HD = totalTime_SD2HD/(i/2);
	nAvgTimeHD2SD = totalTime_HD2SD/(i/2);

	CSTCPrint("%d 次高清->标清平均切换时间为:%d毫秒\n", i/2, nAvgTimeHD2SD);
    	CSTCPrint("The average switching time (HD->SD)is: %d(MS) with %d times\n",nAvgTimeHD2SD, i/2);

	CSTCPrint("%d 次标清->高清平均切换时间为:%d毫秒\n", i/2, nAvgTimeSD2HD);
    	CSTCPrint("The average switching time (SD->HD)is: %d(MS) with %d times\n",nAvgTimeSD2HD, i/2);
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败：播放不正常");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤14失败");

	CSUDIPLAYERDelPlayerCallback(hPlayer, fnPlayerNewFrameEvent, EM_UDIPLAYER_VIDEO_FRAME_COMING, (void *) NEW_FRAME_AT_SWITCHING);

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hPlayer), "步骤15失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}


static int   s_nLiveTestdata = 0;
static void  fnPlayerCallback3(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{
	CSTCPrint("进入回调函数!!");
	if( eEvent==EM_UDIPLAYER_VIDEO_FRAME_COMING)
	{ 
		CSTCPrint("新帧到来,发出消息!!\n");
		s_nLiveTestdata = 1001;
	}
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:测试节目播放时事件为EM_UDIPLAYER_VIDEO_FRAME_COMING 的回调函数情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、hPlayer有效
//@EXPECTATION:CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用PLAYER_iLockTuner锁定指定的频点
//@EXECUTIONFLOW:2、隐藏视频解码器的输出
//@EXECUTIONFLOW:3、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用PLAYER_iSetStream,设置流属性信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERAddPlayerCallback添加回调函数，事件为EM_UDIPLAYER_VIDEO_FRAME_COMING，设置消息s_nLiveTestdata
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、判断回调消息是否正确，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复2~8
//@EXECUTIONFLOW:10、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_Program_0001(void)
{
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDI_HANDLE   hAVPlayer;
	CSUDIPlayerEventType_E  eEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING;
	int nUserData = NUSERDATA;
	int i = 0;
	int j = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_MPEG2_PAL), "步骤1，锁频失败!");
	for(i = 0;i < 2; i++)
	{
		s_nLiveTestdata = 0;
	  	sPlayerChnl.m_nDemux = 0;
		sPlayerChnl.m_nAudioDecoder = 0;
		sPlayerChnl.m_nVideoDecoder = 0;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, FALSE),"步骤2，关闭视频失败!");
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE, &hAVPlayer)) && hAVPlayer != CSUDI_NULL,
			"步骤3，打开播放器失败! "
			);
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE, hAVPlayer,EM_LIVE_MPEG2_PAL, EM_STREAM_AUDIO|EM_STREAM_VIDEO),
			"步骤4，设置节目流失败! "
			);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hAVPlayer,fnPlayerCallback3,eEvent,&nUserData), "步骤5，注册回调失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hAVPlayer), "步骤6，开始播放节目失败!");
		for(j = 0;j < 10 ;j ++)
		{
			if(s_nLiveTestdata ==1001)
			break;
	    		CSUDIOSThreadSleep(2000);
		}
		CSTK_ASSERT_TRUE_FATAL(s_nLiveTestdata == 1001, "步骤7，回调消息错误!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "步骤8，停止播放节目失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hAVPlayer,fnPlayerCallback3,eEvent, &nUserData), "删除回调失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hAVPlayer), "关闭播放器失败 !");
		
	}
	CSTK_FATAL_POINT
	if (hAVPlayer != CSUDI_NULL)
	{
	    CSUDIPLAYERClose (hAVPlayer);
	     hAVPlayer = CSUDI_NULL;
	}
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, TRUE),"打开视频失败!");
	return CSUDI_TRUE;
 
}

static void  fnPlayerCallback4(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{
	CSTCPrint("进入回调函数!!");
	if( eEvent==EM_UDIPLAYER_AUDIO_SOURCE_CHANGE)
	{ 
		CSTCPrint("音频源发生改变,发出消息!!\n");
		s_nLiveTestdata = 1001;
	}
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:测试节目播放时事件为EM_LIVE_AUDIO_SOURCE_CHANGE 的回调函数情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、hPlayer有效
//@EXPECTATION:CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用PLAYER_iLockTuner锁定指定的频点
//@EXECUTIONFLOW:2、隐藏视频解码器的输出
//@EXECUTIONFLOW:3、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用PLAYER_iSetStream,设置流属性信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERAddPlayerCallback添加回调函数，事件为EM_LIVE_AUDIO_SOURCE_CHANGE，设置消息s_nLiveTestdata
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、判断回调消息是否正确，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复2~8
//@EXECUTIONFLOW:10、恢复现场
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
		CSTCPrint("系统支持解码\r\n");
		CSTCPrint("The chip surpports decode format\r\n");
	}
	else
	{
		CSTCPrint("系统不支持解码\r\n");
		CSTCPrint("The chip doesn't surpport decode format\r\n");
		return CSUDI_FALSE;
	}
	
	if(!s_abPlayerCBType[EM_UDIPLAYER_AUDIO_SOURCE_CHANGE])
	{
		CSTCPrint("不支持音频源改变事件\r\n");
		CSTCPrint("Not surpport the event of audio source change.\r\n");
		return CSUDI_FALSE;
	}
	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AUDIO_SOURCE_CHANGE), "步骤1，锁频失败!");
	for(i = 0;i < 2; i++)
	{
		s_nLiveTestdata = 0;
	  	sPlayerChnl.m_nDemux = 0;
		sPlayerChnl.m_nAudioDecoder = 0;
		sPlayerChnl.m_nVideoDecoder = 0;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, FALSE),"步骤2，关闭视频失败!");
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE, &hAVPlayer)) && hAVPlayer != CSUDI_NULL,
			"步骤3，打开播放器失败! "
			);
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE, hAVPlayer,EM_LIVE_AUDIO_SOURCE_CHANGE, EM_STREAM_AUDIO|EM_STREAM_VIDEO|EM_STREAM_PCR),
			"步骤4，设置节目流失败! "
			);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hAVPlayer,fnPlayerCallback4,eEvent,&nUserData), "步骤5，注册回调失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hAVPlayer), "步骤6，开始播放节目失败!");
		for(j = 0;j < 10 ;j ++)
		{
			if(s_nLiveTestdata ==1001)
			break;
	    		CSUDIOSThreadSleep(2000);
		}
		CSTK_ASSERT_TRUE_FATAL(s_nLiveTestdata == 1001, "步骤7，回调消息错误!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "步骤8，停止播放节目失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hAVPlayer,fnPlayerCallback4,eEvent, &nUserData), "删除回调失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hAVPlayer), "关闭播放器失败 !");
		
	}
	CSTK_FATAL_POINT
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer = CSUDI_NULL;
	}
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, TRUE),"打开视频失败!");
	
	return CSUDI_TRUE;
 
}

#define AUDIO_DECODER 0
static void  fnPlayerCallback5(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{
	CSUDIAUDIOStatus_S stStatus;
	const char * pAC3 = "EM_UDI_AUD_STREAM_AC3";
	const char * pAC3Plus = "EM_UDI_AUD_STREAM_AC3_PLUS";
	const char * pCurrentType = pAC3;
	
	CSTCPrint("进入回调函数!!");
	if( eEvent==EM_UDIPLAYER_AUDIO_SOURCE_CHANGE)
	{ 
		CSTCPrint("音频源发生改变,发出消息!!\n");
        CSTCPrint("The audio source changes !!\n");
		CSUDIAUDIOGetStatus(AUDIO_DECODER, &stStatus);
		printf("[chenyajie] type=%d(%d, %d, %d, %d)\n", stStatus.m_eStreamType, stStatus.m_nSampleRate, stStatus.m_nBitWidth, stStatus.m_nChannels, stStatus.m_uFrameCount);
		pCurrentType = (stStatus.m_eStreamType == EM_UDI_AUD_STREAM_AC3?pAC3:pAC3Plus);
		CSTCPrint("音频类型改变为%s!!\n",pCurrentType);
        CSTCPrint("The auido type changed to %s !!\n",pCurrentType);
	}
}

//@CASEGROUP:LivePLAYER
//@DESCRIPTION:测试节目播放时事件为EM_LIVE_AUDIO_SOURCE_CHANGE 的回调函数情况
//@PRECONDITION:PLAYER模块被正常初始化
//@INPUT:1、hPlayer有效
//@EXPECTATION:CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用PLAYER_iLockTuner锁定指定的频点
//@EXECUTIONFLOW:2、隐藏视频解码器的输出
//@EXECUTIONFLOW:3、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用PLAYER_iSetStream,设置流属性信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERAddPlayerCallback添加回调函数，事件为EM_LIVE_AUDIO_SOURCE_CHANGE，设置消息s_nLiveTestdata
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、判断回调消息是否正确，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、恢复现场
CSUDI_BOOL CSTC_LIVE_TEST_IT_Program_0003(void)
{
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDI_HANDLE   hAVPlayer;
	CSUDIPlayerEventType_E  eEvent = EM_UDIPLAYER_AUDIO_SOURCE_CHANGE;
	int nUserData = NUSERDATA;

	if(PLAYER_iIsVideoSupportDecodeType(g_sPlayerSeviceInfo[EM_LIVE_AUDIO_SOURCE_CHANGE].m_eVidStreamType))
	{
		CSTCPrint("系统支持解码\r\n");
		CSTCPrint("The chip surpports decode format\r\n");
	}
	else
	{
		CSTCPrint("系统不支持解码\r\n");
		CSTCPrint("The chip doesn't surpport decode format\r\n");
		return CSUDI_FALSE;
	}

	if(!s_abPlayerCBType[EM_UDIPLAYER_AUDIO_SOURCE_CHANGE])
	{
		CSTCPrint("不支持音频源改变事件\r\n");
		CSTCPrint("Not surpport the event of audio source change.\r\n");
		return CSUDI_FALSE;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == PLAYER_iLockTuner(EM_LIVE_AUDIO_SOURCE_CHANGE), "步骤1，锁频失败!");

  	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nAudioDecoder = AUDIO_DECODER;
	sPlayerChnl.m_nVideoDecoder = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE, &hAVPlayer)) && hAVPlayer != CSUDI_NULL,
		"步骤3，打开播放器失败! "
		);
	CSTK_ASSERT_TRUE_FATAL(
		CSUDI_TRUE  == PLAYER_iSetStream(CSUDI_TRUE, hAVPlayer,EM_LIVE_AUDIO_SOURCE_CHANGE, EM_STREAM_AUDIO|EM_STREAM_VIDEO|EM_STREAM_PCR),
		"步骤4，设置节目流失败! "
		);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hAVPlayer,fnPlayerCallback5,eEvent,&nUserData), "步骤5，注册回调失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hAVPlayer), "步骤6，开始播放节目失败!");

	CSTCPrint("请确认播放的音频声调发生变化的时候有'音频源发生改变'的回调消息打印，并且打印的音频类型与视频上显示的一致\n");
	CSTCPrint("Make sure that we get the audio source change event! And make sure  the notified log is consistent with the live playing!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7，回调消息错误!");
	//CSTK_ASSERT_TRUE_FATAL(s_nLiveTestdata == 1001, "回调消息错误!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "步骤8，停止播放节目失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERDelPlayerCallback(hAVPlayer,fnPlayerCallback5,eEvent, &nUserData), "删除回调失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hAVPlayer), "关闭播放器失败 !");
		
	CSTK_FATAL_POINT
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
 
}



