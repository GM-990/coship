/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_aouttestcase.h"
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_section.h"
#include "udi2_player.h"
#include "udi2_video.h"
#include "udi2_audio.h"
#include "udi2_demux.h"
#include "udi2_os.h"
#include "udi2_tuner.h"
#include "udiplus_debug.h"
#include "cs_testkit.h"
#include "../cs_udi2testcase.h"

static CSUDI_BOOL IsAOUTSupport(CSUDIAOUTDevices_E enType);
CSUDIAOUTDevices_E eSupportAudioDevice = 0;

typedef enum 
{
    EM_UDI_AOUT_STREAM_MPEG2,          /*MPEG2标准测试*/
	EM_UDI_AOUT_STREAM_AC3,				/*DolbyAC3标准测试*/
	EM_AOUT_MAX
}AoutTestServiceIndex_E;

typedef struct 
{	
	const char*          m_pcTsFilename;  	  //码流文件名称
	const char* 		 m_pcServiceDescript; 	//码流描述
	int                  m_nVideoPid;           ///< 数据所在PID，-1表示不存在
	CSUDIVIDStreamType_E m_eVidStreamType;      ///视频类型
	int                  m_nAudioPid;           ///< 数据所在PID ，-1表示不存在
	CSUDIAUDStreamType_E m_eAudStreamType;      ///<音频类型
}AoutTestServiceInfo_S;

static AoutTestServiceInfo_S g_sAoutSeviceInfo[] ={
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"多语言AC3MP2",
			60,
			EM_UDI_VID_STREAM_MPEG2,
			62,
			EM_UDI_AUD_STREAM_MPEG2,
		}, /*EM_LIVE_MPEG2_MULTI_LAN1*/
		{
			"Audio&Video Test_27Mbps_20070524.ts",
			"DolbyAC3标准测试",
			512,
			EM_UDI_VID_STREAM_MPEG2,
			650,
			EM_UDI_AUD_STREAM_AC3,
		}, /*EM_LIVE_MPEG2_AC3*/
};

CSUDI_BOOL CSTC_AOUT_Init(void)
{
	//在本测试用例集执行前调用
	if(IsAOUTSupport(EM_UDIAOUT_RCA))
	{
		CSTCPrint("此平台支持EM_UDIAOUT_RCA设备\n");
		eSupportAudioDevice |= EM_UDIAOUT_RCA;
	}
	if(IsAOUTSupport(EM_UDIAOUT_SPDIF))
	{
		CSTCPrint("此平台支持SPDIF设备\n");
		eSupportAudioDevice |= EM_UDIAOUT_SPDIF;
	}
	if(IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
		CSTCPrint("此平台支持HDMI设备\n");
		eSupportAudioDevice |= EM_UDIAOUT_HDMI;
	}
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_AOUT_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//通过读取配置文件查询平台是否支持某种音频通道类型
static CSUDI_BOOL IsAOUTSupport(CSUDIAOUTDevices_E enType)
{
	const char acSection[] = "AUDIO";
    	char acKey[25] = {0};
	char acResult[255] = {0};
	int nResultSize = (int)sizeof(acResult);

	switch(enType)
	{
		case EM_UDIAOUT_RCA:
			strcpy(acKey, "EM_UDIAOUT_RCA"); break;
		case EM_UDIAOUT_SPDIF: 	
			strcpy(acKey, "EM_UDIAOUT_SPDIF"); break;
		case EM_UDIAOUT_HDMI: 	
			strcpy(acKey, "EM_UDIAOUT_HDMI"); break;
		default: 
			break;
	}

	if (CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo(acSection, acKey, acResult, nResultSize))
	{
		 CSTCPrint("[UDI2AOUTEST]:获取Config文件信息失败! acSection: %s, acKey: %s\n", acSection, acKey);
	}

	if (1 == CSTKGetIntFromStr(acResult, 10))
	{
		return CSUDI_TRUE;	
	}
	return CSUDI_FALSE;
}

//寻找支持某种视频类型的解码器
static int FindVideoDecoder(int nVideoDecCount, CSUDIVIDStreamType_E eFindVideoType)
{
	CSUDIVIDEOCapability_S sVideoCapabilityInfo;
	int nVideoID = -1;
	int i = 0;
	int k = 0;

	for (i=0; i<nVideoDecCount; i++)
	{
		if(!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOGetCapability(i, &sVideoCapabilityInfo)))
		{
      			for (k=0; k<EM_UDI_VID_STREAMTYPE_NUM; k++)
      			{
				if (eFindVideoType == sVideoCapabilityInfo.m_eStreamType[k])
				{
					nVideoID = i;
					break;
				}
				if (EM_UDI_VID_STREAM_UNKNOWN == sVideoCapabilityInfo.m_eStreamType[k])
				{
					break;
				}
			}
		}
			  
		if (nVideoID != -1)
		{
			break;
		}
	}
	return nVideoID;
}

//寻找支持某种音频类型的解码器
static int FindAudioDecoder(int nAudioDecCount, CSUDIAUDStreamType_E eFindAudioType)
{
	CSUDIAUDIOCapability_S sAudioCapabilityInfo;
	int nAudioID = -1;
	int i = 0;
   	int k = 0;

	for (i=0; i<nAudioDecCount; i++)
	{
		if(!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo)))
		{
			for (k=0; k< EM_UDI_AUD_STREAMTYPE_NUM; k++ )
			{
				if (eFindAudioType == sAudioCapabilityInfo.m_sSupportDecode[k]) 
				{
					nAudioID = i;
					break;
				}
				if (EM_UDI_AUD_STREAM_UNKNOWN == sAudioCapabilityInfo.m_sSupportDecode[k])
				{
					break;
				}
			}
		}
			  
		if (nAudioID != -1)
		{
			break;
		}

	}
   	return nAudioID;
}

//查询平台是否支持某种类型的音频流编码格式
static CSUDI_BOOL IsSupportDecoder(CSUDIAUDStreamType_E eFindAudioType)
{
	CSUDIAUDIOCapability_S sAudioCapabilityInfo;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int nAudioDecCount = 0;
	int i = 0;
	int k = 0;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)))
		return CSUDI_FALSE;

	if(CSASSERT_FAILED(nAudioDecCount > 0))
		return CSUDI_FALSE;

	for (i=0; i<nAudioDecCount; i++)
	{
		if(!CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo)))
		{
			for (k=0; k <= EM_UDI_AUD_STREAMTYPE_NUM; k++ )
			{
				if (EM_UDI_AUD_STREAM_UNKNOWN == sAudioCapabilityInfo.m_sSupportDecode[k])
				{
					break;
				}
				if (EM_UDI_AUD_STREAMTYPE_NUM == sAudioCapabilityInfo.m_sSupportDecode[k])
				{
					break;
				}
				if (eFindAudioType == sAudioCapabilityInfo.m_sSupportDecode[k]) 
				{
					bRet = CSUDI_TRUE;
					return bRet;
				}			
			}
		}
	}	
	return bRet;		
}

// 播放音视频测试节目
static CSUDI_HANDLE PlayProgram01(AoutTestServiceIndex_E AoutTestIndex,int* pnAudioIndex)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIDEMUXCapability_S sDemuxCapabilityInfo;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S scStreamInfo[2];
	int nVideoDecCount = 0;
	int nVideoIndex = -1;
	int nAudioDecCount = 0;
	int nAudioIndex = -1;
	int nDemuxCount = 0 ; 
	int i = 0;

	if(AoutTestIndex >= EM_AOUT_MAX)
	{
		CSTCPrint("输入的播放音频类型无效!!!!\n");
		return CSUDI_NULL;
	}
	
	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoDecCount)))
		return CSUDI_NULL;
	if(CSASSERT_FAILED(nVideoDecCount > 0))
		return CSUDI_NULL;
	nVideoIndex = FindVideoDecoder(nVideoDecCount, g_sAoutSeviceInfo[AoutTestIndex].m_eVidStreamType);

	if(CSASSERT_FAILED(nVideoIndex != -1))
		return CSUDI_NULL;
	
	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)))
		return CSUDI_NULL;
	if(CSASSERT_FAILED(nAudioDecCount > 0))
		return CSUDI_NULL;
	nAudioIndex = FindAudioDecoder(nAudioDecCount, g_sAoutSeviceInfo[AoutTestIndex].m_eAudStreamType);

	if(g_sAoutSeviceInfo[AoutTestIndex].m_eAudStreamType == EM_UDI_AUD_STREAM_AC3)
	{	
		if(!IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
		{
			CSTCPrint("该解码器不支持AC3音频格式!!!!\n");
			nAudioIndex = 0;
		}
	}

	if(CSASSERT_FAILED(nAudioIndex != -1))
		return CSUDI_NULL;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)))
		return CSUDI_NULL;
	if(CSASSERT_FAILED(nDemuxCount > 0))
		return CSUDI_NULL;
	for (i=0; i<nDemuxCount; i++)
	{
		if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo)))
			return CSUDI_NULL;
		if (EM_UDI_DEMUX_PLAY == (sDemuxCapabilityInfo.m_dwWorkTypeMask&EM_UDI_DEMUX_PLAY))  
		{
			break;
		}
	}

	if(CSASSERT_FAILED(i<nDemuxCount))
		return CSUDI_NULL;
	sPlayerChnl.m_nDemux = i;
	sPlayerChnl.m_nAudioDecoder = nAudioIndex;
	sPlayerChnl.m_nVideoDecoder = nVideoIndex;

	//锁测试码流频点
	if(CSASSERT_FAILED(CSUDI_TRUE == CSTC_UDI2PortingLock(0, g_sAoutSeviceInfo[AoutTestIndex].m_pcTsFilename)))
		return CSUDI_NULL;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer)))
		return CSUDI_NULL;

	if(CSASSERT_FAILED(hPlayer != NULL))
		return CSUDI_NULL;

  	//节目参数设置
	scStreamInfo[0].m_nPid = g_sAoutSeviceInfo[AoutTestIndex].m_nAudioPid;
	scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	scStreamInfo[0].m_uStreamType.m_eAudioType = g_sAoutSeviceInfo[AoutTestIndex].m_eAudStreamType;
	scStreamInfo[1].m_nPid = g_sAoutSeviceInfo[AoutTestIndex].m_nVideoPid;
	scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	scStreamInfo[1].m_uStreamType.m_eVideoType = g_sAoutSeviceInfo[AoutTestIndex].m_eVidStreamType;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL)))
		return hPlayer;

	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)))
		return hPlayer;
	
	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)))
		return hPlayer;

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA)))
		return hPlayer;
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF)))
		return hPlayer;
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI)))
		return hPlayer;
	}
	
	if(NULL != pnAudioIndex)
	{
		*pnAudioIndex = nAudioIndex;
	}
	
	return hPlayer;	
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:测试节目源为MPEG音频,依次进行设备mute的情况
//@PRECONDITION:节目播放正常
//@INPUT:1. 节目正常播放,所有待测设备均有声音输出
//@EXPECTATION:mute和unmute功能正确,且互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.调用player模块接口播放MPEG音频码流,详细步骤如下
//@EXECUTIONFLOW:1.1 锁测试码流频点
//@EXECUTIONFLOW:1.2 调用CSUDIVIDEOGetCount获取video解码器数量
//@EXECUTIONFLOW:1.3 调用CSUDIVIDEOGetCapability依次寻找第一个支持MPEG2的decoder,并将其作为节目播放的视频解码器
//@EXECUTIONFLOW:1.4 调用CSUDIAUDIOGetCount获取audio解码器的数量
//@EXECUTIONFLOW:1.5 调用CSUDIAUDIOGetCapability依次寻找第一个支持MPEG2的decoder,并将其作为节目播放的音频解码器
//@EXECUTIONFLOW:1.6 调用CSUDIDEMUXGetCount获取demux的数量
//@EXECUTIONFLOW:1.7 调用CSUDIDEMUXGetCapability依次寻找第一个支持EM_UDI_DEMUX_PLAY的demux用于节目播放的demux
//@EXECUTIONFLOW:1.8 调用CSUDIPLAYEROpen打开一个播放器,期望打开成功
//@EXECUTIONFLOW:1.9 调用CSUDIPLAYERSetStream设置测试码流属性
//@EXECUTIONFLOW:1.10 调用CSUDIPLAYERStart播放测试码流,期望所有设备均有声音输出
//@EXECUTIONFLOW:2. 调用CSUDIAOUTMute关闭RCA的声音输出并等待确认
//@EXECUTIONFLOW:3. 如果平台支持SPDIF输出则调用CSUDIAOUTMute关闭SPDIF声音输出并等待确认
//@EXECUTIONFLOW:4. 如果平台支持HDMI输出则调用CSUDIAOUTMute关闭HDMI声音输出并等待确认
//@EXECUTIONFLOW:5. 调用CSUDIAOUTUnmute打开所有设备的声音输出,并等待确认
//@EXECUTIONFLOW:6. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:7. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;

	hPlayer = PlayProgram01(AoutTestIndex, NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤1失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTCPrint("步骤1:已成功启动播放，请确认RCA有声音输出 \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1失败\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " 步骤2失败，关闭RCA声音输出失败\n");
       	CSTCPrint("步骤2:已关闭RCA的声音输出，请确认RCA是否无声音输出\r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");
	}

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)	
		{
			CSTCPrint("前面步骤的静音不应该影响到SPDIF，请确认SPDIF 有声音输出\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "RCA静音不应该影响到SPDIF\n");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), "步骤3失败，关闭SPDIF声音输出失败\n");
	       	CSTCPrint("步骤3:已关闭SPDIF的声音输出，SPDIF 是否无声音输出\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");
         }
	}

	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTCPrint("前面步骤的静音不应该影响到HDMI，请确认HDMI 有声音输出\r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "前面通道的静音不应该影响到HDMI\n");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " 步骤4失败，关闭HDMI声音输出失败\n");
       	CSTCPrint("步骤4:已关闭HDMI的声音输出，HDMI 是否无声音输出\r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤4失败\n");
	}

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤5-1失败，未能打开RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤5-2失败，未能打开SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤5-3失败，未能打开HDMI设备声音\n");
	}	
	CSTCPrint("步骤5:已取消所有通道的静音状态，请确认所有通道有声音输出，且无杂音和爆音等异常\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");

   	CSTK_FATAL_POINT
  	{
  		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
  			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"程序结束，将RCA设备声音打开失败\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
  			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"程序结束，将SPDIF设备声音打开失败\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
  			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"程序结束，将HDMI设备声音打开失败\n");
		}
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:测试节目源为AC3音频,依次进行设备mute的情况
//@PRECONDITION:节目播放正常
//@INPUT:1. 节目正常播放,所有待测设备均有声音输出
//@EXPECTATION:mute和unmute功能正确,且互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.调用player模块接口播放AC3音频码流,详细步骤如下
//@EXECUTIONFLOW:1.1 锁测试码流频点
//@EXECUTIONFLOW:1.2 调用CSUDIVIDEOGetCount获取video解码器数量
//@EXECUTIONFLOW:1.3 调用CSUDIVIDEOGetCapability依次寻找第一个支持MPEG2的decoder,并将其作为节目播放的视频解码器
//@EXECUTIONFLOW:1.4 调用CSUDIAUDIOGetCapability依次寻找第一个支持AC3的decoder,并将其作为节目播放的音频解码器。如果平台不支持AC3解码，直接指定音频解码器的index为0
//@EXECUTIONFLOW:1.5 调用CSUDIAOUTSetOPMode设置数字音频为BYPASS模式
//@EXECUTIONFLOW:1.6 调用CSUDIDEMUXGetCount获取demux的数量
//@EXECUTIONFLOW:1.7 调用CSUDIDEMUXGetCapability依次寻找第一个支持EM_UDI_DEMUX_PLAY的demux用于节目播放的demux
//@EXECUTIONFLOW:1.8 调用CSUDIPLAYEROpen打开一个播放器,期望打开成功
//@EXECUTIONFLOW:1.9 调用CSUDIPLAYERSetStream设置测试码流属性
//@EXECUTIONFLOW:1.10 调用CSUDIPLAYERStart播放测试码流,期望:如果平台支持AC3解码则所有设备均有声音输出,否则只有SPDIF有声音输出
//@EXECUTIONFLOW:2. 如果平台支持AC3解码，则所有通道都有声音输出
//@EXECUTIONFLOW:3. 依次关闭通道声音，期望互不影响。
//@EXECUTIONFLOW:4. 依次打开所有通道声音，希望所支持的通道声音播放正常
//@EXECUTIONFLOW:5. 将SPDIF通道输出模式改为BYPASS，希望SPDIF通道能正常输出声音
//@EXECUTIONFLOW:6. 如果平台不支持AC3解码，这仅仅有SPDIF通道有声音输出
//@EXECUTIONFLOW:7. 关闭SPDIF 通道声音输出，希望关闭成功
//@EXECUTIONFLOW:9. 将SPDIF 通道输出模式改为DECODE，希望没有声音输出
//@EXECUTIONFLOW:10. 重复2-9步2 次
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	int i = 0;
	int nRepeatTimes = 2;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "播放节目失败,hPlayer 为空!");

	for(i = 0; i < nRepeatTimes; i++) 
	{	
		if(IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
		{
			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
			}
			
			CSTCPrint("步骤2:已设置平台支持的所有通道的输出模式为DECODE模式，请确认是否所有通道都有声音\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");
			if(eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " 步骤3失败，关闭RCA声音输出失败\n");
				CSTCPrint("步骤3:已设置RCA静音，请确认RCA 是否无声音输出\r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");
			}

			if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
				if(eSupportAudioDevice & EM_UDIAOUT_SPDIF)	
				{
					CSTCPrint("前面步骤的静音不应该影响到SPDIF，请确认SPDIF 有声音输出\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "RCA静音不应该影响到SPDIF\n");
			
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " 步骤3失败，关闭SPDIF声音输出失败\n");
				    CSTCPrint("步骤3:已设置SPDIF静音，SPDIF 是否无声音输出\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");		
                }
             }

			if(eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTCPrint("前面步骤的静音不应该影响到HDMI，请确认HDMI 有声音输出\r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "前面通道的静音不应该影响到HDMI\n");
		
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " 步骤3失败，关闭HDMI声音输出失败\n");
			    CSTCPrint("步骤3:已设置HDMI静音，HDMI 是否无声音输出\r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");
			}

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA), " 恢复RCA通道声音输出\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF), " 恢复SPDIF通道声音输出\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI), " 恢复HDMI通道声音输出\n");
			}
	       	CSTCPrint("步骤4:已取消平台支持的所有设备的静音状态，请确认所有设备有声音输出，且无杂音和爆音等异常\r\n");
	       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤4失败\n");

			if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
				if(eSupportAudioDevice & EM_UDIAOUT_SPDIF)	
                {
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS),"设置AC3解码失败\n");
					CSTCPrint("步骤5:已设置SPDIF输出模式为BYPASS，请确认SPDIF仍然有正常声音输出\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");
                }
			}
		}
		else
		{
 
        	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
				if(eSupportAudioDevice & EM_UDIAOUT_SPDIF)	
                {
					//CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"设置DECODE模式失败\n");
					CSTCPrint("步骤1.10:平台不支持AC3解码，请确认所有设备都没有声音输出\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1.10失败\n");
					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS),"设置BYPASS模式失败\n");
				
					CSTCPrint("只有SPDIF有声音输出\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1.10失败\n");
				 
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " 步骤3失败，关闭SPDIF声音输出失败\n");
				    CSTCPrint("SPDIF 是否无声音输出\r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");
				
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF), " 恢复所有通道声音输出\n");
			       	CSTCPrint("SPDIF通道声音恢复，且无杂音和爆音等异常\r\n");
			       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"设置AC3解码失败\n");
					CSTCPrint("请确认所有通道没有声音输出\r\n");
			       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");
                }
			}
		}
	}

   CSTK_FATAL_POINT
   {
   		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"程序结束，将RCA设备声音打开失败\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"程序结束，将SPDIF设备声音打开失败\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"程序结束，将HDMI设备声音打开失败\n");
		}
		
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败\n");
			hPlayer = CSUDI_NULL;
		}
   }
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:测试多次mute后,再unmute是否声音能正常打开
//@PRECONDITION:节目播放正常
//@INPUT:1. 节目正常播放,所有待测设备均有声音输出
//@EXPECTATION:1.多次mute所有设备的时候所有设备没有声音输出
//@EXPECTATION:2.unmute的时候声音恢复正常
//@EXPECTATION:3.mute和unmute互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.调用player模块接口播放测试码流,详细步骤请参考测试用例CSTC_AOUT_TEST_IT_AOUTMute_0001步骤1
//@EXECUTIONFLOW:2. 循环调用5次CSUDIAOUTMute关闭所有设备的声音输出,并等待确认
//@EXECUTIONFLOW:3. 调用CSUDIAOUTUnmute打开所有设备的声音输出并等待确认
//@EXECUTIONFLOW:4. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:5. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0003( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	int i = 0;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex, NULL);  //调用PLAYER播放测试码流
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");

    if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
    }    

	CSTCPrint("[UDI2AOUTEST]步骤1:测试节目源为MPEG音频,是否平台支持的设备都有声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1失败\n");

	for (i= 0; i<5; i++)
	{
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"步骤2-1失败，未能关闭RCA设备声音\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"步骤2-2失败，未能关闭SPDIF设备声音\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"步骤2-3失败，未能关闭HDMI设备声音\n");
		}

		CSTCPrint("[UDI2AOUTEST]步骤2:已对平台支持的所有设备静音\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出 \r\n");
	    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");      
	}

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤3-1失败，未能打开RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤3-2失败，未能打开SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤3-3失败，未能打开HDMI设备声音\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤3:已对平台支持的所有设备取消静音\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,所有设备无声音输出 \r\n");
   	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
		   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:测试当mute设备后,换台不能破坏mute状态
//@PRECONDITION:节目播放正常
//@INPUT:1. 节目正常播放,所有待测设备均有声音输出
//@EXPECTATION:1.mute.unmute功能正确
//@EXPECTATION:2.换台不能破坏静音状态
//@EXPECTATION:3.mute和unmute互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.调用player模块接口播放测试码流,详细步骤请参考测试用例CSTC_AOUT_TEST_IT_AOUTMute_0001步骤1
//@EXECUTIONFLOW:2.调用CSUDIAOUTMute关闭所有设备的声音输出,并等待确认
//@EXECUTIONFLOW:3.调用CSUDIPLAYERStop停止当前节目
//@EXECUTIONFLOW:4.调用CSUDIPLAYERSetStream设置新节目参数
//@EXECUTIONFLOW:5.调用CSUDIPLAYERStart播放新节目,期望节目播放正常,有图像,但是所有设备声音保持静音状态
//@EXECUTIONFLOW:6.调用CSUDIAOUTUnmute打开所有设备声音输出,期望所有设备有声音输出
//@EXECUTIONFLOW:7. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:8. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0004( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	CSUDIStreamInfo_S scStreamInfo[2]; 
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");

	CSTCPrint("[UDI2AOUTEST]步骤1:测试节目源为MPEG音频,是否平台支持的设备都有声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"步骤2-1失败，未能关闭RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"步骤2-2失败，未能关闭SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"步骤2-3失败，未能关闭HDMI设备声音\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤2:已对平台支持的所有设备静音\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出 \r\n");
    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤3失败，停止播放节目失败\n");

	//设置新节目参数,重新播放节目
	scStreamInfo[0].m_nPid = 1141;
	scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_MPEG2;
	scStreamInfo[1].m_nPid = 1140;
	scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL), "设置节目参数失败\n");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "更换频道后，播放节目失败\n");

	   CSTCPrint("[UDI2AOUTEST]步骤5:已更换频道\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,更换频道后，所有设备依然保持静音\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,更换频道后，设备有声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤6-1失败，未能打开RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤6-2失败，未能打开SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤6-3失败，未能打开HDMI设备声音\n");
	}

	 CSTCPrint("[UDI2AOUTEST]步骤6:已对平台支持的所有设备取消静音\n");
    CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,所有设备无声音输出 \r\n");
    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤6失败\n");
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败\n");
		}
  	}
	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:测试CSUDIAOUTMute 参数错误的情况
//@PRECONDITION:CSUDIAOUTMute 返回CSUDIAOUT_ERROR_BAD_PARAMETER
//@INPUT:逻辑错误的参数
//@EXECUTIONFLOW:1.调用CSUDIAOUTMute，传入错误参数
//@EXECUTIONFLOW:2.希望返回CSUDIAOUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0005( void )
{
	int i = 0;
	
	int nErrorParam[3] = {0};

	nErrorParam[0] = -1;
	nErrorParam[1] = 0;
	nErrorParam[2] = EM_UDIAOUT_HDMI+1;
	
	for(i=0; i<sizeof(nErrorParam)/sizeof(nErrorParam[0]); i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIAOUTMute(nErrorParam[i])), "参数检测失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@DESCRIPTION:组合测试PCM播放时各个输出接口的控制
//@PRECONDITION:CSUDIAOUTMute 返回CSUDIAOUT_ERROR_BAD_PARAMETER
//@INPUT:逻辑错误的参数
//@EXECUTIONFLOW:1.调用CSUDIAOUTMute，传入错误参数
//@EXECUTIONFLOW:2.希望返回CSUDIAOUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTMute_0006( void )
{
	int i = 0;
	
	int nErrorParam[3] = {0};

	nErrorParam[0] = -1;
	nErrorParam[1] = 0;
	nErrorParam[2] = EM_UDIAOUT_HDMI+1;

	for(i=0; i<sizeof(nErrorParam)/sizeof(nErrorParam[0]); i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIAOUTMute(nErrorParam[i])), "参数检测失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:测试节目源为MPEG,依次进行设备unmute的情况
//@PRECONDITION:节目播放正常
//@INPUT:1. 节目正常播放,所有待测设备均有声音输出
//@EXPECTATION:1.mute所有设备的时候所有设备没有声音输出
//@EXPECTATION:2.依次unmute设备的时候相应设备恢复声音输出
//@EXPECTATION:3.mute和unmute互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.调用player模块接口播放测试码流,详细步骤请参考测试用例CSTC_AOUT_TEST_IT_AOUTMute_0001步骤1
//@EXECUTIONFLOW:2. 调用CSUDIAOUTMute关闭所有设备的声音输出,并等待确认
//@EXECUTIONFLOW:3. 调用CSUDIAOUTUnmute打开RCA的声音输出并等待确认
//@EXECUTIONFLOW:4. 如果平台支持SPDIF输出则调用CSUDIAOUTUnmute打开SPDIF声音输出并等待确认
//@EXECUTIONFLOW:5. 如果平台支持HDMI输出则调用CSUDIAOUTUnmute打开HDMI声音输出并等待确认
//@EXECUTIONFLOW:6. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:7. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0001( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");

	CSTCPrint("[UDI2AOUTEST]步骤1:测试节目源为MPEG音频,是否平台支持的设备都有声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"步骤2-1失败，未能关闭RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"步骤2-2失败，未能关闭SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"步骤2-3失败，未能关闭HDMI设备声音\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤2:已对平台支持的所有设备静音\n");
    CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出 \r\n");
    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤3失败，未能打开RCA设备声音\n");
		CSTCPrint("[UDI2AOUTEST]步骤3:已对RCA取消静音\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,RCA 有声音输出，且无杂音和爆音等异常\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,RCA 无声音输出 \r\n");
	    CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");
	}

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
        {
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF), " 步骤4失败，打开SPDIF声音输出失败\n");
			CSTCPrint("[UDI2AOUTEST]步骤4:已对SPDIF取消静音\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF 有声音输出，且无杂音和爆音等异常\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,SPDIF 无声音输出 \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤4失败\n");
        }
	}

	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI), " 步骤5失败，打开HDMI声音输出失败\n");
		CSTCPrint("[UDI2AOUTEST]步骤5:已对HDMI取消静音\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,HDMI 有声音输出，且无杂音和爆音等异常\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,HDMI 无声音输出 \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");
	}
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:测试节目源为AC3,依次进行设备unmute的情况
//@PRECONDITION:节目播放正常
//@INPUT:1. 节目正常播放,所有待测设备均有声音输出
//@EXPECTATION:1.mute所有设备的时候所有设备没有声音输出
//@EXPECTATION:2.依次unmute设备的时候相应设备恢复声音输出
//@EXPECTATION:3.mute和unmute互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.如果平台不支持AC3解码直接返回TRUE,否则调用player模块接口播放测试码流,详细步骤请参考测试用例CSTC_AOUT_TEST_IT_AOUTMute_0002步骤1
//@EXECUTIONFLOW:2. 调用CSUDIAOUTMute关闭所有设备的声音输出,并等待确认
//@EXECUTIONFLOW:3. 调用CSUDIAOUTUnmute打开RCA的声音输出并等待确认
//@EXECUTIONFLOW:4. 如果平台支持SPDIF输出则调用CSUDIAOUTUnmute打开SPDIF声音输出并等待确认
//@EXECUTIONFLOW:5. 如果平台支持HDMI输出则调用CSUDIAOUTUnmute打开HDMI声音输出并等待确认
//@EXECUTIONFLOW:6. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:7. 如果平台支持AC3数字音频解码,调用CSUDIAOUTSetOPMode设置数字音频为解码模式
//@EXECUTIONFLOW:8. 重复步骤1.9到步骤6,重复一次
//@EXECUTIONFLOW:9. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0002( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	CSUDIStreamInfo_S scStreamInfo[2]; 
	int i = 2;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;

	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");
	
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{

		if (IsAOUTSupport(EM_UDIAOUT_RCA))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
		}
		if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS) 失败!");
        }
		if (IsAOUTSupport( EM_UDIAOUT_HDMI))
		{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
		}
		CSTCPrint("[UDI2AOUTEST]步骤1:测试节目源为AC3音频，如果平台支持AC3解码，则平台支持的设备都有声音输出，否则只有SPDIF有声音。 \r\n");
   		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1失败\n");	
		do 
		{
			if (1 == i)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
				if (IsAOUTSupport(EM_UDIAOUT_RCA))
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
				}
		    	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		        {
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE) 失败!");
		        }  
				if (IsAOUTSupport( EM_UDIAOUT_HDMI))
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
				}
				//设置码流属性
				scStreamInfo[0].m_nPid = 650;
				scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
				scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_AC3;
				scStreamInfo[1].m_nPid = 512;
				scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
				scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL), "CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL) 失败!");
             			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "CSUDIPLAYERStart(hPlayer) 失败!");
			}
			
	       		i--;
			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"步骤2-1失败，未能打开RCA设备声音\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"步骤2-2失败，未能打开SPDIF设备声音\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"步骤2-3失败，未能打开HDMI设备声音\n");
			}

			CSTCPrint("[UDI2AOUTEST]步骤2:已对平台支持的所有设备静音\n");
		    	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
          		CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出 \r\n");
	       		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤3失败，未能打开RCA设备声音\n");
				CSTCPrint("[UDI2AOUTEST]步骤3:已对RCA取消静音\n");
				CSTCPrint("[UDI2AOUTEST]:Yes,RCA 有声音输出，且无杂音和爆音等异常\r\n");
				CSTCPrint("[UDI2AOUTEST]:No,RCA 无声音输出 \r\n");
		       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");
			}


			if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
			   	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
				{
			    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF), " 步骤4失败，打开SPDIF声音输出失败\n");
					CSTCPrint("[UDI2AOUTEST]步骤4:已对SPDIF取消静音\n");
					CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF 有声音输出，且无杂音和爆音等异常\r\n");
					CSTCPrint("[UDI2AOUTEST]:No,SPDIF 无声音输出 \r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤4失败\n");		
	            }
			}

			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI), " 步骤5失败，打开HDMI声音输出失败\n");
				CSTCPrint("[UDI2AOUTEST]步骤5:已对HDMI取消静音\n");
				CSTCPrint("[UDI2AOUTEST]:Yes,HDMI 有声音输出，且无杂音和爆音等异常\r\n");
				CSTCPrint("[UDI2AOUTEST]:No,HDMI 无声音输出 \r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");
			}
		}while(i);
	}
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤6失败，停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤9失败，关闭播放器失败\n");
		}
   	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:测试节目源为MPEG,mute所有设备和打开所有设备的情况
//@PRECONDITION:节目播放正常
//@INPUT:1. 正常节目播放,所有待测设备均有声音输出
//@EXPECTATION:1.mute所有设备的时候所有设备没有声音输出
//@EXPECTATION:2.unmute所有设备的时候,所有设备均恢复声音输出
//@EXPECTATION:3.mute和unmute互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.调用player模块接口播放测试码流,详细步骤请参考测试用例CSTC_AOUT_TEST_IT_AOUTMute_0001步骤1
//@EXECUTIONFLOW:2. 调用CSUDIAOUTMute关闭所有设备的声音输出,并等待确认
//@EXECUTIONFLOW:3. 调用CSUDIAOUTUnmute打开所有设备声音输出,并等待确认
//@EXECUTIONFLOW:4. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:5. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0003( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");

	CSTCPrint("[UDI2AOUTEST]步骤1:测试节目源为MPEG音频,是否平台支持的设备都有声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " 步骤2-1失败，关闭RCA设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " 步骤2-2失败，关闭SPDIF设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " 步骤2-3失败，关闭HDMI设备声音输出失败\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤2:已对平台支持的所有设备静音\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出 \r\n");
   	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes( ), "步骤2失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤3-1失败，未能打开RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤3-2失败，未能打开SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤3-3失败，未能打开HDMI设备声音\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤3:已对平台支持的所有设备取消静音\n");
 	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,所有设备无声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");
	
   	CSTK_FATAL_POINT
   	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:测试节目源为AC3,mute所有设备和打开所有设备的情况
//@PRECONDITION:节目播放正常
//@INPUT:1. 正常节目播放,所有待测设备均有声音输出
//@EXPECTATION:1.mute所有设备的时候所有设备没有声音输出
//@EXPECTATION:2.unmute所有设备的时候,所有设备均恢复声音输出
//@EXPECTATION:3.mute和unmute互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.调用player模块接口播放测试码流,详细步骤请参考测试用例CSTC_AOUT_TEST_IT_AOUTMute_0002步骤1
//@EXECUTIONFLOW:2. 调用CSUDIAOUTMute关闭所有设备的声音输出,并等待确认
//@EXECUTIONFLOW:3. 调用CSUDIAOUTUnmute打开所有设备声音输出,并等待确认,如果平台不支持AC3解码则只有SPDIF有声音输出
//@EXECUTIONFLOW:4. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:5. 如果平台支持AC3数字音频解码,调用CSUDIAOUTSetOPMode设置数字音频为解码模式
//@EXECUTIONFLOW:6. 重复步骤1.9到步骤4,重复一次
//@EXECUTIONFLOW:7. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0004( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	CSUDIStreamInfo_S scStreamInfo[2]; 
	int i = 2;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");

	if (IsAOUTSupport(EM_UDIAOUT_RCA))
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
	}

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "设置BYPASS模式失败");
    }   
	if (IsAOUTSupport( EM_UDIAOUT_HDMI))
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE),"设置DECODER模式失败\n");
	}
	
	CSTCPrint("[UDI2AOUTEST]步骤1:测试节目源为AC3音频,是否平台支持的设备都有声音输出，如平台不支持AC3解码则只有SPDIF有声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1失败\n");
	
	do 
	{
		if (1 == i)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "设置EM_UDIAOUT_DECODE模式失败");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "设置EM_UDIAOUT_DECODE模式失败");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "设置EM_UDIAOUT_DECODE模式失败");
			}
			
			//设置码流属性
			scStreamInfo[0].m_nPid = 650;
			scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
			scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_AC3;
			scStreamInfo[1].m_nPid = 512;
			scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
			scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL), "CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL)失败!");
         		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "CSUDIPLAYERStart(hPlayer)失败!");
		}
		
      		i--;
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"步骤2-1失败，未能打开RCA设备声音\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"步骤2-2失败，未能打开SPDIF设备声音\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"步骤2-3失败，未能打开HDMI设备声音\n");
		}
		CSTCPrint("[UDI2AOUTEST]步骤2:已对平台支持的所有设备静音\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
       		CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出 \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");

		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤3-1失败，未能打开RCA设备声音\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤3-2失败，未能打开SPDIF设备声音\n");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤3-3失败，未能打开HDMI设备声音\n");
		}
		CSTCPrint("[UDI2AOUTEST]步骤3:已对平台支持的所有设备取消静音\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
      		CSTCPrint("[UDI2AOUTEST]:No,所有设备无声音输出 \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");
		
	}while(IsSupportDecoder(EM_UDI_AUD_STREAM_AC3) && i);

   	CSTK_FATAL_POINT
  	{

		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤6失败，停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤7失败，关闭播放器失败\n");
		}
   	}
	return CSUDI_TRUE;      
}

//@CASEGROUP:CSUDIAOUTMute 
//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:测试节目源为MPEG,CSUDIAUDIOMute\CSUDIAUDIOUnmute与CSUDIAOUTMute\CSUDIAOUTUnmute的逻辑控制情况
//@PRECONDITION:节目播放正常
//@INPUT:1.节目 正常播放,所有待测设备均有声音输出
//@EXPECTATION:1.CSUDIAUDIOMute的时候即使设备是unmute的也不能有声音输出
//@EXPECTATION:2.只有CSUDIAUDIOMute并且相应设备unmute的时候相应设备才能有声音输出
//@EXPECTATION:3.mute和unmute互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.调用player模块接口播放测试码流,详细步骤请参考测试用例CSTC_AOUT_TEST_IT_AOUTMute_0001步骤1
//@EXECUTIONFLOW:2. 调用CSUDIAUDIOUnmute打开audio的声音输出,调用CSUDIAOUTUnmute打开所有设备的声音输出,期望所有设备有声音输出
//@EXECUTIONFLOW:3. 调用CSUDIAUDIOMute关闭audio的声音,期望所有设备都没有声音输出
//@EXECUTIONFLOW:4. 调用CSUDIAUDIOUnmute,期望所有设备恢复声音输出
//@EXECUTIONFLOW:5. 调用CSUDIAOUTMute关闭所有设备的声音输出,期望所有设备静音
//@EXECUTIONFLOW:6. 调用CSUDIAOUTUnmute打开所有设备的声音输出,期望所有设备都有声音输出
//@EXECUTIONFLOW:7. 关闭RCA的声音输出,期望RCA静音
//@EXECUTIONFLOW:8. 如果平台支持HDMI,关闭HDMI的声音输出,期望HDMI静音
//@EXECUTIONFLOW:9. 如果平台支持SPDIF,关闭SPDIF的声音输出,期望SPDIF静音
//@EXECUTIONFLOW:10. 调用CSUDIAOUTUnmute打开所有设备的声音输出,期望所有设备都有声音输出
//@EXECUTIONFLOW:11. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:12. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0005( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int nAudioIndex = -1;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,&nAudioIndex);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");
	CSTK_ASSERT_TRUE_FATAL((nAudioIndex != -1),"启动节目播放失败\n");

	CSTCPrint("[UDI2AOUTEST]步骤1:测试节目源为MPEG音频,是否平台支持的设备都有声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "步骤2失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤2-1失败，未能打开RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤2-2失败，未能打开SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤2-3失败，未能打开HDMI设备声音\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤2:已对Audio及平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex), "步骤3失败\n");
	CSTCPrint("[UDI2AOUTEST]步骤3:已对Audio设置静音(CSUDIAUDIOMute&CSUDIAOUTUnmute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,Mute Audio 所有设备无声音输出\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "步骤4失败\n");
	CSTCPrint("[UDI2AOUTEST]步骤4:已对Audio取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,Unmute Audio 所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤4失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " 步骤5-1失败，关闭RCA设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " 步骤5-2失败，关闭SPDIF设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " 步骤5-3失败，关闭HDMI设备声音输出失败\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤5:已对平台支持的所有设备静音(CSUDIAUDIOUnmute&CSUDIAOUTMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出\r\n");
   	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤6-1失败，未能打开RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤6-2失败，未能打开SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤6-3失败，未能打开HDMI设备声音\n");
	}
	CSTCPrint("[UDI2AOUTEST]步骤6:已对平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,所有设备无声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤6失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " 步骤7失败，关闭RCA声音输出失败\n");
		CSTCPrint("[UDI2AOUTEST]步骤7:已对RCA静音\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,RCA 没有声音输出\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,RCA 有声音输出\r\n");
       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤7失败\n");
	}


    if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
	    if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " 步骤8失败，关闭SPDIF声音输出失败\n");
			CSTCPrint("[UDI2AOUTEST]步骤8:已对SPDIF静音\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF 无声音输出\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,SPDIF 有声音输出 \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤8失败\n");
		}
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " 步骤9失败，关闭HDMI声音输出失败\n");
		CSTCPrint("[UDI2AOUTEST]步骤9:已对HDMI静音\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,HDMI 无声音输出\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,HDMI 有声音输出 \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤9失败\n");
	}

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤10-1失败，未能打开RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤10-2失败，未能打开SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤10-3失败，未能打开HDMI设备声音\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤10:已对平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
    CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤10失败\n");
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:测试节目源为MPEG,CSUDIAUDIOMute\CSUDIAOUTMute与CSUDIAUDIOUnmute\CSUDIAOUTUnmute的逻辑控制情况
//@PRECONDITION:节目播放正常
//@INPUT:1.节目 正常播放,所有待测设备均有声音输出
//@EXPECTATION:1.CSUDIAUDIOMute的时候即使设备是unmute的也不能有声音输出
//@EXPECTATION:2.只有CSUDIAUDIOUnmute并且相应设备unmute的时候相应设备才能有声音输出
//@EXPECTATION:3.mute和unmute互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.调用player模块接口播放测试码流,详细步骤请参考测试用例CSTC_AOUT_TEST_IT_AOUTMute_0001步骤1
//@EXECUTIONFLOW:2. 调用CSUDIAUDIOUnmute打开audio的声音输出,调用CSUDIAOUTUnmute打开所有设备的声音输出,期望所有设备有声音输出
//@EXECUTIONFLOW:3. 调用CSUDIAUDIOMute关闭audio的声音,期望所有设备都没有声音输出
//@EXECUTIONFLOW:4. 调用CSUDIAOUTMute关闭所有设备的声音输出,期望所有设备仍保持静音
//@EXECUTIONFLOW:5. 调用CSUDIAUDIOUnmute打开audio的声音输出,期望所有设备仍保持静音
//@EXECUTIONFLOW:6.  调用CSUDIAOUTUnmute打开所有设备的声音输出,期望所有设备都有声音输出
//@EXECUTIONFLOW:7. 调用CSUDIAOUTMute关闭所有设备的声音输出,期望所有设备都没有声音输出
//@EXECUTIONFLOW:8. 调用CSUDIAUDIOMute关闭audio的声音,期望所有设备都没有声音输出
//@EXECUTIONFLOW:9.  调用CSUDIAOUTUnmute打开所有设备的声音输出，期望所有设备都没有声音输出
//@EXECUTIONFLOW:10.  调用CSUDIAUDIOUnmute打开audio的声音输出,期望所有设备都有声音输出
//@EXECUTIONFLOW:11. 关闭RCA的声音输出,期望RCA静音
//@EXECUTIONFLOW:12. 如果平台支持HDMI,关闭HDMI的声音输出,期望HDMI静音
//@EXECUTIONFLOW:13. 如果平台支持SPDIF,关闭SPDIF的声音输出,期望SPDIF静音
//@EXECUTIONFLOW:14. 调用CSUDIAOUTUnmute打开所有设备的声音输出,期望所有设备都有声音输出
//@EXECUTIONFLOW:15. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:16. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0006( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int nAudioIndex = -1;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_MPEG2;
	
	hPlayer = PlayProgram01(AoutTestIndex,&nAudioIndex);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");
	CSTK_ASSERT_TRUE_FATAL((nAudioIndex != -1),"启动节目播放失败\n");

	CSTCPrint("[UDI2AOUTEST]步骤1:测试节目源为MPEG音频,是否平台支持的设备都有声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤1失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "步骤2打开Audio的声音输出失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤2-1打开RCA输出设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤2-2打开SPDIF输出设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤2-3打开HDMI输出设备声音输出失败\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤2:已对Audio及平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
    	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex), "步骤3关闭Audio的声音输出失败\n");
	CSTCPrint("[UDI2AOUTEST]步骤3:已对Audio设置静音(CSUDIAUDIOUnmute&CSUDIAUDIOMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"步骤4-1关闭RCA设备的声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"步骤4-2关闭SPDIF设备的声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"步骤4-3关闭HDMI设备的声音输出失败\n");
	}
	CSTCPrint("[UDI2AOUTEST]步骤4:已对平台支持的所有设备静音(CSUDIAOUTMute&CSUDIAUDIOMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备应保持静音\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤4失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "步骤5打开Audio的声音输出失败\n");
	CSTCPrint("[UDI2AOUTEST]步骤5:已对Audio取消静音(CSUDIAUDIOUnmute&CSUDIAOUTMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备应保持静音\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤6-1打开RCA设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤6-2打开SPDIF设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤6-3打开HDMI设备声音输出失败\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤6:已对平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤6失败\n");
	
	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA),"步骤7-1关闭RCA设备的声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF),"步骤7-2关闭SPDIF设备的声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI),"步骤7-3关闭HDMI设备的声音输出失败\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤7:已对平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备应保持静音\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤7失败\n");
		           
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex), "步骤8关闭Audio声音输出失败\n");
	CSTCPrint("[UDI2AOUTEST]步骤8:已对Audio设置静音(CSUDIAUDIOMute&CSUDIAOUTMute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备仍保持静音\r\n");
   	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出\r\n");
   	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤8失败\n");

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤9-1打开RCA设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤9-2打开SPDIF设备声音输出失败\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤9-3打开HDMI设备声音输出失败\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤9:已对平台支持的所有设备取消静音(CSUDIAUDIOMute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备仍然没有声音输出\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤9失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "步骤10打开Audio声音输出失败\n");
	CSTCPrint("[UDI2AOUTEST]步骤10:已对Audio取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤10失败\n");
	
	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " 步骤11失败，关闭RCA声音输出失败\n");
		CSTCPrint("[UDI2AOUTEST]步骤11:已对RCA静音\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,RCA 没有声音输出\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,RCA 有声音输出 \r\n");
       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤11失败\n");
	}

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
	{

		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " 步骤12失败，关闭SPDIF声音输出失败\n");
			CSTCPrint("[UDI2AOUTEST]步骤12:已对SPDIF静音\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF 无声音输出\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,SPDIF 有声音输出 \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤12失败\n");	
		}
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " 步骤13失败，关闭HDMI声音输出失败\n");
		CSTCPrint("[UDI2AOUTEST]步骤13:已对HDMI静音\n");
		CSTCPrint("[UDI2AOUTEST]:Yes,HDMI 无声音输出\r\n");
		CSTCPrint("[UDI2AOUTEST]:No,HDMI 有声音输出 \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤13失败\n");	
	}

	if (eSupportAudioDevice & EM_UDIAOUT_RCA)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤14-1失败，未能打开RCA设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤14-2失败，未能打开SPDIF设备声音\n");
	}
	if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤14-3失败，未能打开HDMI设备声音\n");
	}

	CSTCPrint("[UDI2AOUTEST]步骤14:已对平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
   	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
	CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出 \r\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤14失败\n");
	
   	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤15停止播放节目失败\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤16关闭播放器失败\n");
		}
  	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTMute 
//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:测试节目源为AC3,CSUDIAUDIOMute\CSUDIAUDIOUnmute与CSUDIAOUTMute\CSUDIAOUTUnmute的逻辑控制情况
//@PRECONDITION:节目播放正常
//@INPUT:1.节目 正常播放,所有待测设备均有声音输出
//@EXPECTATION:1.CSUDIAUDIOMute的时候即使设备是unmute的也不能有声音输出
//@EXPECTATION:2.只有CSUDIAUDIOMute并且相应设备unmute的时候相应设备才能有声音输出
//@EXPECTATION:3.mute和unmute互相切换时没有杂音和爆音等异常
//@EXECUTIONFLOW:1.如果平台不支持AC3解码直接返回TRUE,否则调用player模块接口播放测试码流,详细步骤请参考测试用例CSTC_AOUT_TEST_IT_AOUTMute_0002步骤1
//@EXECUTIONFLOW:2. 调用CSUDIAUDIOUnmute打开audio的声音输出,调用CSUDIAOUTUnmute打开所有设备的声音输出,期望所有设备有声音输出
//@EXECUTIONFLOW:3. 调用CSUDIAUDIOMute关闭audio的声音,期望所有设备都没有声音输出
//@EXECUTIONFLOW:4. 调用CSUDIAUDIOUnmute,期望所有设备恢复声音输出
//@EXECUTIONFLOW:5. 调用CSUDIAOUTMute关闭所有设备的声音输出,期望所有设备静音
//@EXECUTIONFLOW:6. 调用CSUDIAOUTUnmute打开所有设备的声音输出,期望所有设备都有声音输出
//@EXECUTIONFLOW:7. 关闭RCA的声音输出,期望RCA静音
//@EXECUTIONFLOW:8. 如果平台支持HDMI,关闭HDMI的声音输出,期望HDMI静音
//@EXECUTIONFLOW:9. 如果平台支持SPDIF,关闭SPDIF的声音输出,期望SPDIF静音
//@EXECUTIONFLOW:10. 调用CSUDIAOUTUnmute打开所有设备的声音输出,期望所有设备都有声音输出
//@EXECUTIONFLOW:11. 调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW:12. 如果平台支持AC3数字音频解码,调用CSUDIAOUTSetOPMode设置数字音频为解码模式
//@EXECUTIONFLOW:13. 重复步骤1.9到步骤11,重复一次
//@EXECUTIONFLOW:14. 调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0007( void )
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S scStreamInfo[2];
	int i = 2;
	int nAudioIndex = -1;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;

	hPlayer = PlayProgram01(AoutTestIndex,&nAudioIndex);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");	
	CSTK_ASSERT_TRUE_FATAL((nAudioIndex != -1),"启动节目播放失败\n");
	
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{	
		do 
		{
			if( IsAOUTSupport( EM_UDIAOUT_RCA))
			{
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "设置DECODE模式失败");
			}
			if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "设置BYPASS模式失败");
        	} 

			if( IsAOUTSupport( EM_UDIAOUT_HDMI))
			{
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "设置DECODE模式失败");
			}
			if (1 == i)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放节目失败\n");
                
				if( IsAOUTSupport( EM_UDIAOUT_RCA))
				{
			
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "设置DECODE模式失败");
				}
				if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "设置DECODE模式失败");
                }
				if( IsAOUTSupport( EM_UDIAOUT_HDMI))
				{
			
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "设置DECODE模式失败");
				}
				//设置码流属性
				scStreamInfo[0].m_nPid = 650;
				scStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
				scStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_AC3;
				scStreamInfo[1].m_nPid = 512;
				scStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
				scStreamInfo[1].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL), "CSUDIPLAYERSetStream(hPlayer, scStreamInfo, 2, CSUDI_NULL)失败!");
             			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "CSUDIPLAYERStart(hPlayer)失败!");
			}
          		i--;
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "步骤2失败\n");

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤2-1失败，未能打开RCA设备声音\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤2-2失败，未能打开SPDIF设备声音\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤2-3失败，未能打开HDMI设备声音\n");
			}

			CSTCPrint("[UDI2AOUTEST]步骤2:已对Audio及平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出 \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex), "步骤3失败\n");
			CSTCPrint("[UDI2AOUTEST]步骤3:已对Audio设置静音(CSUDIAUDIOMute&CSUDIAOUTUnmute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
          		CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出 \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex), "步骤4失败\n");
			CSTCPrint("[UDI2AOUTEST]步骤4:已对Audio取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
          		CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出 \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤4失败\n");

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA)," 步骤5-1失败，关闭RCA设备声音输出失败\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF)," 步骤5-2失败，关闭SPDIF设备声音输出失败\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI)," 步骤5-3失败，关闭HDMI设备声音输出失败\n");
			}
			CSTCPrint("[UDI2AOUTEST]步骤5:已对平台支持的所有设备静音(CSUDIAUDIOUnmute&CSUDIAOUTMute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,所有设备无声音输出\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,设备有声音输出 \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤6-1失败，未能打开RCA设备声音\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤6-2失败，未能打开SPDIF设备声音\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤6-3失败，未能打开HDMI设备声音\n");
			}
			CSTCPrint("[UDI2AOUTEST]步骤6:已对平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
			CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
			CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出 \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤6失败\n");


            if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
            {
          	  if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
				{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_RCA), " 步骤7失败，关闭RCA声音输出失败\n");
				CSTCPrint("[UDI2AOUTEST]步骤7:已对RCA静音\n");
				CSTCPrint("[UDI2AOUTEST]:Yes,RCA 无声音输出\r\n");
	          	CSTCPrint("[UDI2AOUTEST]:No,RCA 有声音输出 \r\n");
		       	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤7失败\n");
				}
			
            
				if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_SPDIF), " 步骤8失败，关闭SPDIF声音输出失败\n");
					CSTCPrint("[UDI2AOUTEST]步骤8:已对SPDIF静音\n");
					   CSTCPrint("[UDI2AOUTEST]:Yes,SPDIF 无声音输出\r\n");
			              CSTCPrint("[UDI2AOUTEST]:No,SPDIF 有声音输出 \r\n");
					CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤8失败\n");
					
				}
            }

			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI), " 步骤9失败，关闭HDMI声音输出失败\n");
				CSTCPrint("[UDI2AOUTEST]步骤9:已对HDMI静音\n");
				CSTCPrint("[UDI2AOUTEST]:Yes,HDMI 无声音输出\r\n");
				CSTCPrint("[UDI2AOUTEST]:No,HDMI 有声音输出 \r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤9失败\n");
			}

			if (eSupportAudioDevice & EM_UDIAOUT_RCA)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_RCA),"步骤10-1失败，未能打开RCA设备声音\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_SPDIF),"步骤10-2失败，未能打开SPDIF设备声音\n");
			}
			if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI),"步骤10-3失败，未能打开HDMI设备声音\n");
			}
			CSTCPrint("[UDI2AOUTEST]步骤10:已对平台支持的所有设备取消静音(CSUDIAUDIOUnmute&CSUDIAOUTUnmute)\n");
	       	CSTCPrint("[UDI2AOUTEST]:Yes,所有设备有声音输出，且无杂音和爆音等异常\r\n");
          		CSTCPrint("[UDI2AOUTEST]:No,设备无声音输出 \r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤10失败\n");
		}while(i);

	}

	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤11失败，停止播放节目失败\n");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败，关闭播放器失败\n");
		}
   	}
	return CSUDI_TRUE;
} 

//@CASEGROUP:CSUDIAOUTUnmute 
//@DESCRIPTION:测试CSUDIAOUTMute 参数错误的情况
//@PRECONDITION:CSUDIAOUTUnmute 返回CSUDIAOUT_ERROR_BAD_PARAMETER
//@INPUT:逻辑错误的参数
//@EXECUTIONFLOW:1.调用CSUDIAOUTUnmute，传入错误参数
//@EXECUTIONFLOW:2.希望返回CSUDIAOUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_AOUT_TEST_IT_AOUTUnmute_0008( void )
{
	int i = 0;
	
	int nErrorParam[3] = {0};

	nErrorParam[0] = -1;
	nErrorParam[1] = 0;
	nErrorParam[2] = EM_UDIAOUT_HDMI+1;

	
	for(i=0; i<sizeof(nErrorParam)/sizeof(nErrorParam[0]); i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIAOUT_ERROR_BAD_PARAMETER ==CSUDIAOUTUnmute(nErrorParam[i])), "参数检测失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: 测试音频解码器的输出模式为EM_UDIAOUT_BYPASS功能
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统能正常播放音视频
//@INPUT: 1、eAudioDevice = 支持AC3类型的解码设备
//@INPUT: 2、eMode = EM_UDIAOUT_BYPASS
//@EXPECTATION: 每一步都获得期望值，在播放状态下设置成EM_UDIAOUT_BYPASS之后，应该立即生效
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点,播放AC3音频码流，期望外置解码音箱有声音输出
//@EXECUTIONFLOW: 2、如果平台不支持AC3解码，调用CSUDIAOUTSetOPMode(eAudioDevice, EM_UDIAOUT_BYPASS)，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、期望音箱设备有声音输出
//@EXECUTIONFLOW: 4、如果平台支持AC3解码，调用CSUDIAOUTSetOPMode(eAudioDevice, EM_UDIAOUT_DECODE)，期望返回CSUDI_SUCCESS，否则
//@EXECUTIONFLOW: 5、如果平台支持AC3解码，在eAudioDevice上播放"DolbyAC3标准测试节目"，期望播放成功,且所有音箱之外的合法设备均有声音输出
//@EXECUTIONFLOW: 6、调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW: 7、调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_SetOPMode_0001(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");

	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "步骤4-1失败");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "步骤4-2失败");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "步骤4-3失败");
		}
		CSTCPrint("[UDI2AOUTEST]步骤5:已设置平台支持的所有输出设备的解码模式为DECODE，是否所有的输出接口对应的设备都有声音输出 \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");
	}
	else
	{

      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		{
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "步骤4失败");
				CSTCPrint("[UDI2AOUTEST]步骤5:设置播放模式为BYPASS，是否外置音箱都有声音输出 \r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");
			}
         }
	}
	
	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤6失败，停止播放节目失败\n");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤7失败，关闭播放器失败\n");
		}
   	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: 测试音频解码器的输出模式的逻辑正确性
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统存在音频SPDIF输出
//@INPUT: 1、eAudioDevice = EM_UDIAOUT_RCA
//@INPUT: 2、eMode = EM_UDIAOUT_BYPASS
//@EXPECTATION: 期望在左右声道下仅能执行Decoder 操作，程序返回CSUDIAOUT_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW: 1、调用CSUDIAOUTSetOPMode,eMode = EM_UDIAOUT_BYPASS,希望CSUDIAOUT_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW: 2、调用CSUDIAOUTSetOPMode,eMode = EM_UDIAOUT_DECODE,希望CSUDI_SUCCESS
CSUDI_BOOL CSTC_AOUT_SetOPMode_0002(void)
{	
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	
	nResult = CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_BYPASS);

	CSTK_ASSERT_TRUE_FATAL((CSUDIAOUT_ERROR_FEATURE_NOT_SUPPORTED == nResult), "EM_UDIAOUT_RCA下仅能为EM_UDIAOUT_DECODE");

	nResult = CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS== nResult), "EM_UDIAOUT_RCA下EM_UDIAOUT_DECODE 应该成功");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: 测试音频解码器的输出模式的逻辑正确性
//@PRECONDITION: CSUDIAOUTSetOPMode 返回CSUDIAOUT_ERROR_BAD_PARAMETER
//@INPUT: 1、eAudioDevice = 逻辑错误
//@INPUT: 2、eMode = 逻辑错误
//@EXPECTATION: 期望CSUDIAOUTSetOPMode 返回CSUDIAOUT_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1、调用CSUDIAOUTSetOPMode
//@EXECUTIONFLOW: 2、希望CSUDIAOUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_AOUT_SetOPMode_0003(void)
{	
	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(-1, EM_UDIAOUT_DECODE), "参数检测失败1");

	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(0, EM_UDIAOUT_DECODE), "参数检测失败2");

	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI+1, EM_UDIAOUT_DECODE), "参数检测失败3");

	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE-1), "参数检测失败4");

	CSTK_ASSERT_TRUE_FATAL(CSUDIAOUT_ERROR_BAD_PARAMETER == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_BYPASS+1), "参数检测失败5");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: 播放DD或DD+节目CSUDIAOUTSetOPMode接设置输出模式时SPDIF、HDMI、RCA不能互相影响
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统能正常播放音视频
//@PRECONDITION: 3、播放节目后设置输出模式
//@INPUT: 1、eAudioDevice = 支持AC3类型的解码设备
//@EXPECTATION: 每一步都获得期望值，在播放状态下设置成EM_UDIAOUT_BYPASS之后，应该立即生效
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点,播放AC3音频码流，期望外置解码音箱有声音输出
//@EXECUTIONFLOW: 2、CSUDIAOUTSetOPMode设置HDMI输出的解码模式，SPDIF输出为解码模式，如果芯片支持DD+或DD解码，则各个接口输出解码后的PCM，否则只有支持DD的输出设备才有声音
//@EXECUTIONFLOW: 3、CSUDIAOUTSetOPMode设置HDMI输出的解码模式，SPDIF输出为bypass模式，如果芯片支持DD+或DD解码，则HDMI和RCA输出PCM，SPDIF bypass输出，否则只有支持DD的输出设备才有声音
//@EXECUTIONFLOW: 4、CSUDIAOUTSetOPMode设置HDMI输出的bypass模式，SPDIF输出为解码模式，如果芯片支持DD+或DD解码，则SPDIF、RCA输出P解码后的PCM，HDMI输出无声音，否则只有支持DD的输出设备才有声音
//@EXECUTIONFLOW: 5、CSUDIAOUTSetOPMode设置HDMI输出的bypass模式，SPDIF输出为bypass模式，如果芯片支持DD+或DD解码，RCA输出P解码后的PCM，SPDIF bypass输出，HDMI输出无声音，否则只有支持DD的输出设备才有声音
//@EXECUTIONFLOW: 6、调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW: 7、调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_SetOPMode_0004(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");
	
	//EXECUTIONFLOW: 2、CSUDIAOUTSetOPMode设置HDMI输出的解码模式，SPDIF输出为解码模式，
	//如果芯片支持DD+或DD解码，则各个接口输出解码后的PCM，
	//否则只有支持DD的输出设备才有声音
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "步骤4-1失败");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "步骤4-2失败");
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{	
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "步骤4-3失败");
		}
		CSTCPrint("[UDI2AOUTEST]步骤2:已设置平台支持的所有输出设备的解码模式为DECODE，是否所有设备都有声音输出 \r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");
	}
	else
	{
	
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		{
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "步骤2失败");
				CSTCPrint("[UDI2AOUTEST]步骤2:设置SPDIF输出模式为BYPASS，是否外置音箱都有声音输出 \r\n");
				CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");
			}
		}
        
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "步骤2失败");
			CSTCPrint("[UDI2AOUTEST]步骤2:设置HDMI输出模式为BYPASS，期望结果为下面中的一种 \r\n");	
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备支持DD，则应该输出为Dolby格式的音频数据\r\n");
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备不支持DD，则应该不输出任何音频数据\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败\n");
		}
	}

	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{	
		//EXECUTIONFLOW: 3、CSUDIAOUTSetOPMode设置HDMI输出的解码模式，SPDIF输出为bypass模式，
		//如果芯片支持DD+或DD解码，则HDMI和RCA输出PCM，SPDIF bypass输出
		
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		{
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "步骤3失败");
				CSTCPrint("[UDI2AOUTEST]步骤3:设置SPDIF输出模式为BYPASS，期望SPDIF设备输出为Dolby格式 \r\n");			
			}
		}
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "步骤3失败");
			CSTCPrint("[UDI2AOUTEST]步骤3:设置HDMI输出模式为解码输出，期望HDMI设备有声音输出，并为PCM格式 \r\n");			
		}
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤3失败\n");



			//EXECUTIONFLOW: 4、CSUDIAOUTSetOPMode设置HDMI输出的bypass模式，SPDIF输出为解码模式，
		//如果芯片支持DD+或DD解码，则SPDIF、RCA输出P解码后的PCM，HDMI输出无声音
		
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "步骤4失败");
				CSTCPrint("[UDI2AOUTEST]步骤4:设置SPDIF输出模式为解码输出，期望SPDIF设备输出为PCM格式 \r\n");			
			}
        }
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "步骤4失败");
			CSTCPrint("[UDI2AOUTEST]步骤4:设置HDMI输出模式为BYPASS，期望结果为下面中的一种 \r\n");			
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备支持DD，则应该输出为Dolby格式的音频数据\r\n");
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备不支持DD，则应该不输出任何音频数据\r\n");		
        }
	
		CSTCPrint("[UDI2AOUTEST]RCA等音频输出正常\r\n");	
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤4失败\n");



		//EXECUTIONFLOW: 5、CSUDIAOUTSetOPMode设置HDMI输出的bypass模式，SPDIF输出为bypass模式，
		//如果芯片支持DD+或DD解码，RCA输出P解码后的PCM，SPDIF bypass输出，HDMI输出无声音，
		

      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "步骤4失败");
				CSTCPrint("[UDI2AOUTEST]步骤5:设置SPDIF输出模式为BYPASS，期望SPDIF设备输出为Dolby格式 \r\n"); 		
			}
        }

		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "步骤4失败");
			CSTCPrint("[UDI2AOUTEST]步骤5:设置HDMI输出模式为BYPASS，期望结果为下面中的一种 \r\n");			
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备支持DD，则应该输出为Dolby格式的音频数据\r\n");
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备不支持DD，则应该不输出任何音频数据\r\n");		
		}
		CSTCPrint("[UDI2AOUTEST]步骤5:RCA等音频输出正常\r\n");	
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤5失败\n");

	}

		
	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤6失败，停止播放节目失败\n"); 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤7失败，关闭播放器失败\n");
		
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: 播放DD或DD+节目CSUDIAOUTSetOPMode接设置输出模式时SPDIF、HDMI、RCA不能互相影响
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统能正常播放音视频
//@PRECONDITION: 3、播放节目前设置输出模式
//@INPUT: 1、eAudioDevice = 支持AC3类型的解码设备
//@EXPECTATION: 每一步都获得期望值，在播放状态下设置成EM_UDIAOUT_BYPASS之后，应该立即生效
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点,播放AC3音频码流
//@EXECUTIONFLOW: 2、如果芯片支持DD+或DD解码，CSUDIAOUTSetOPMode设置各个接口输出为解码模式， 并期望有PCM声音输出；否则设置SPDIF和HDMI为bypass模式，并期望有Dolby声音输出
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERStop停止测试节目播放并CSUDIAOUTSetOPMode设置HDMI输出的解码模式，SPDIF输出为bypass模式
//@EXECUTIONFLOW: 4、重新播放AC3音频码流，期望:如果芯片支持DD+或DD解码，则HDMI和RCA输出PCM，SPDIF bypass输出，否则只有支持DD的输出设备才有声音
//@EXECUTIONFLOW: 5、调用CSUDIPLAYERStop停止测试节目播放并CSUDIAOUTSetOPMode设置HDMI输出的bypass模式，SPDIF输出为解码模式
//@EXECUTIONFLOW: 6、重新播放AC3音频码流，期望:如果芯片支持DD+或DD解码，则SPDIF、RCA输出解码后的PCM，HDMI输出无声音，否则只有支持DD的输出设备才有声音
//@EXECUTIONFLOW: 7、调用CSUDIPLAYERStop停止测试节目播放并CSUDIAOUTSetOPMode设置HDMI输出的bypass模式，SPDIF输出为bypass模式
//@EXECUTIONFLOW: 8、重新播放AC3音频码流，期望:如果芯片支持DD+或DD解码，RCA输出解码后的PCM，SPDIF bypass输出，HDMI输出无声音，否则只有支持DD的输出设备才有声音
//@EXECUTIONFLOW: 9、调用CSUDIPLAYERStop停止测试节目播放
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_AOUT_SetOPMode_0005(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;

	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");
	
	//EXECUTIONFLOW: 1、CSUDIAOUTSetOPMode设置HDMI输出的解码模式，SPDIF输出为解码模式，
	//如果芯片支持DD+或DD解码，则各个接口输出解码后的PCM，
	//否则只有支持DD的输出设备才有声音
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		if (eSupportAudioDevice & EM_UDIAOUT_RCA)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_RCA, EM_UDIAOUT_DECODE), "步骤1-1失败");
			CSTCPrint("[UDI2AOUTEST]步骤2:设置RCA输出模式为DECODER \r\n");	
		}
		if (eSupportAudioDevice & EM_UDIAOUT_SPDIF)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "步骤1-2失败");
			CSTCPrint("[UDI2AOUTEST]步骤2:设置SPDIF输出模式为DECODER \r\n");	
		}
		if (eSupportAudioDevice & EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "步骤1-3失败");
			CSTCPrint("[UDI2AOUTEST]步骤2:设置HDMI输出模式为DECODER \r\n");	
		}

		CSTCPrint("[UDI2AOUTEST]步骤2:是否支持输出的设备都有PCM声音输出 \r\n");	
			
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败:与期望输出不符\n");
	}
	else
	{
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "步骤1失败");
				CSTCPrint("[UDI2AOUTEST]步骤2:设置SPDIF输出模式为BYPASS，是否外置音箱都有声音输出 \r\n");
			}
		}
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "步骤1失败");
			CSTCPrint("[UDI2AOUTEST]步骤2:设置HDMI输出模式为BYPASS，期望结果为下面中的一种 \r\n");			
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备支持DD，则应该输出为Dolby格式的音频数据\r\n");
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备不支持DD，则应该不输出任何音频数据\r\n");
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤2失败:与期望输出不符\n");
	}

 	//本用例已经对芯片不支持DD解码的情况进行测试了
 	//现在只需针对芯片支持DD解码进行如下测试
	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		//EXECUTIONFLOW: 3、调用CSUDIPLAYERStop停止测试节目播放并CSUDIAOUTSetOPMode设置HDMI输出的解码模式，SPDIF输出为bypass模式
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤3失败，停止播放节目失败\n"); 

		
	
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "步骤3失败");
				CSTCPrint("[UDI2AOUTEST]步骤3:设置SPDIF输出模式为BYPASS，期望SPDIF设备输出为Dolby格式 \r\n"); 		
			}
		}
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_DECODE), "步骤3失败");
			CSTCPrint("[UDI2AOUTEST]步骤3:设置HDMI输出模式为解码输出，期望HDMI设备有声音输出，并为PCM格式 \r\n");			
		}
		//EXECUTIONFLOW: 4、重新播放AC3音频码流，期望:如果芯片支持DD+或DD解码，则HDMI和RCA输出PCM，SPDIF bypass输出，否则只有支持DD的输出设备才有声音
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),"步骤4启动节目播放失败\n");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤4失败\n");
		
		//EXECUTIONFLOW: 5、调用CSUDIPLAYERStop停止测试节目播放并CSUDIAOUTSetOPMode设置HDMI输出的bypass模式，SPDIF输出为解码模式
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤5失败，停止播放节目失败\n"); 

		
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE), "步骤5失败");
				CSTCPrint("[UDI2AOUTEST]步骤5:设置SPDIF输出模式为解码输出，期望SPDIF设备输出为PCM格式 \r\n"); 		
			}
        }
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "步骤5失败");
			CSTCPrint("[UDI2AOUTEST]步骤5:设置HDMI输出模式为BYPASS，期望结果为下面中的一种 \r\n");			
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备支持DD，则应该输出为Dolby格式的音频数据\r\n");
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备不支持DD，则应该不输出任何音频数据\r\n");		
        }
		
		//EXECUTIONFLOW: 6、重新播放AC3音频码流，期望:如果芯片支持DD+或DD解码，则SPDIF、RCA输出P解码后的PCM，HDMI输出无声音，否则只有支持DD的输出设备才有声音
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),"步骤6启动节目播放失败\n");

		CSTCPrint("[UDI2AOUTEST]步骤5:RCA等音频输出正常\r\n");	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤6失败\n");

		//EXECUTIONFLOW: 7、调用CSUDIPLAYERStop停止测试节目播放并CSUDIAOUTSetOPMode设置HDMI输出的bypass模式，SPDIF输出为bypass模式
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤7失败，停止播放节目失败\n"); 
		
      	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
        {
			if(eSupportAudioDevice& EM_UDIAOUT_SPDIF)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "步骤7失败");
				CSTCPrint("[UDI2AOUTEST]步骤7:设置SPDIF输出模式为BYPASS，期望SPDIF设备输出为Dolby格式 \r\n"); 		
			}
		}
		if(eSupportAudioDevice& EM_UDIAOUT_HDMI)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI, EM_UDIAOUT_BYPASS), "步骤7失败");
			CSTCPrint("[UDI2AOUTEST]步骤7:设置HDMI输出模式为BYPASS，期望结果为下面中的一种 \r\n");			
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备支持DD，则应该输出为Dolby格式的音频数据\r\n");
			CSTCPrint("[UDI2AOUTEST]如果HDMI设备不支持DD，则应该不输出任何音频数据\r\n");		
        }
		
		//EXECUTIONFLOW: 8、重新播放AC3音频码流，期望:如果芯片支持DD+或DD解码，RCA输出P解码后的PCM，SPDIF bypass输出，HDMI输出无声音，否则只有支持DD的输出设备才有声音
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),"步骤8启动节目播放失败\n");

		CSTCPrint("[UDI2AOUTEST]步骤7:RCA等音频输出正常\r\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤8失败\n");
	}
	
	CSTK_FATAL_POINT
	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤9失败，停止播放节目失败\n"); 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤10失败，关闭播放器失败\n");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
//@DESCRIPTION: 测试在暂停状态下是否能成功设置音频通道的输出模式
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统能正常播放音视频
//@INPUT: 1、eAudioDevice = 支持AC3类型的解码设备，该用例以SPDIF为外置解码设备
//@INPUT: 2、eMode = EM_UDIAOUT_BYPASS
//@EXPECTATION: 每一步都获得期望值，在暂停状态下能成功设置音频通道的输出模式，恢复后能正常播放音视频且设置的模式生效
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点,播放AC3音频码流，期望外置解码音箱有声音输出且为PCM格式
//@EXECUTIONFLOW: 2、调用CSUDIPLAYERPause接口，使player处于暂停状态，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、测试人员观察，期望外置解码音箱无声音输出
//@EXECUTIONFLOW: 4、调用CSUDIAOUTSetOPMode接口，输出模式则设成EM_UDIAOUT_BYPASS
//@EXECUTIONFLOW: 5、调用CSUDIPLAYERResume接口，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、测试人员观察测试结果，期望外置解码音箱恢复声音输出且为Dolby格式
//@EXECUTIONFLOW: 7、调用CSUDIPLAYERStop停止测试节目播放，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSUDIPLAYERClose关闭播放器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_AOUT_SetOPMode_0006(void)
{
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AoutTestServiceIndex_E AoutTestIndex = EM_UDI_AOUT_STREAM_AC3;
	
	hPlayer = PlayProgram01(AoutTestIndex,NULL);  //调用PLAYER播放测试码流

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "启动节目播放失败\n");

	if (IsSupportDecoder(EM_UDI_AUD_STREAM_AC3))
	{
		if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_DECODE),"步骤1失败\n");
		
			CSTCPrint("步骤1:已设置SPDIF通道输出模式为DECODE模式，请确认外置解码音箱有声音输出且为PCM格式\r\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤1失败\n");

			nRet = CSUDIPLAYERPause(hPlayer);
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nRet), "步骤2失败\n");

			CSTCPrint("步骤3:已暂停播放，外置解码音箱暂停声音输出?\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤3失败\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF, EM_UDIAOUT_BYPASS), "步骤4失败");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer), "步骤5失败\n");

			CSTCPrint("步骤6:已设置SPDIF通道输出模式为BYPASS模式且恢复播放，请确认外置解码音箱恢复声音输出且为Dolby格式\r\n");
			CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "步骤6失败\n");
		}   
	}
	
	CSTK_FATAL_POINT
  	{
		if (CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤7失败，停止播放节目失败\n");	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤8失败，关闭播放器失败\n");
		}
   	}
   	
	return CSUDI_TRUE;
}



