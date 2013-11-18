/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_audiotestcase.h"
#include "udi2_audio.h"
#include "udi2_aout.h"
#include "cs_testkit.h"
#include "udi2_player.h"
#include "udi2_os.h"
#include "../cs_udi2testcase.h"
#include "udiplus_debug.h"

#define AUDIO_INDEX_0  (0)
#define AUDIO_TEST_TIMES  (5)
#define AUDIO_DEFAULT_VOLUME (16)

//系统中音频解码器个数
static int s_nAudioCount = 0;

////音频解码器支持的音频格式类型数组
static char * g_pcAudioStreamType[EM_UDI_AUD_STREAMTYPE_NUM] ={
	"STREAM_INVALID",
	"STREAM_AC3",
	"STREAM_DTS",
	"STREAM_MPEG1",
	"STREAM_MPEG2",
	"STREAM_CDDA",
	"STREAM_PCM",
	"STREAM_LPCM",
	"STREAM_MP3",
	"STREAM_MPEG_AAC",
	"STREAM_HE_AAC",
	"STREAM_RAW_AAC",
	"STREAM_AAC_PLUS_ADTS",
	"STREAM_AAC_PLUS_LOAS",
	"STREAM_AC3_PLUS",
	"STREAM_DTS_HD",
	"STREAM_WMA_STD",
	"STREAM_WMA_PRO",
};

//音频通道输出的模式
static char * g_pcAudioMode[2] ={
	"DECODE",
	"BYPASS"
};


typedef struct 
{
	CSUDIAUDIOChannelType_E m_eChannel;
	char  			m_cChannelInfo[64];
}AudioTestChannel_S;

typedef struct {
	unsigned int  	uAudOutput;
	char 		    cAudOutputName[128];
}AudioTestOutputCfg_S;

typedef enum 
{
    EM_AUDIO_MULTILANGUAGE1 = 0, /*还珠格格*/
    EM_AUDIO_MULTILANGUAGE2, /*多语言测试及左右声道测试，左右声道音频不同*/
    EM_AUDIO_DOLBYAC3, /*DolbyAC3标准测试*/
    EM_AUDIO_MPEG1 /*MPEG1视频测试节目*/
}AudioServiceIndex_E;

typedef struct 
{	
	const char*          m_pcTsFilename;  //码流文件名称
	const char* 		 m_pcServiceDescript;  //码流描述
	int                  m_nPid;              ///< 数据所在PID 
	CSUDIAUDStreamType_E m_eStreamType;       ///<音频类型
	CSUDI_BOOL			 m_bIsDiffChannel; /*该节目是否支持左右声道测试*/
	int					 m_nSampleRate;  ///<  音频采样率 
	int					 m_nBitWidth;	///<  音频采样位数 
	int					 m_nChannels;  	///<  通道数:1-mono,2-stereo 
}AudioTestServiceInfo_S;

static AudioTestServiceInfo_S s_sAudioServices[] = 
{
    {
        "Audio&Video Test_27Mbps_20070524.ts",
        "还珠格格",
        0X0475,
        EM_UDI_AUD_STREAM_MPEG2,
        CSUDI_TRUE,
		48000,
		16,
		2
    },
    {
    	"Audio&Video Test_27Mbps_20070524.ts",
        "多语言左右声道",
        0X0476,
        EM_UDI_AUD_STREAM_MPEG2,
        CSUDI_FALSE,
		48000,
		16,
		2
    },
    {
        "Audio&Video Test_27Mbps_20070524.ts",
        "DolbyAC3标准测试",
        0x28A,
        EM_UDI_AUD_STREAM_AC3,
        CSUDI_FALSE,
		48000,
		16,
		2
    },
    {
    	"H.264.ts",
    	"MPEG1视频测试节目",  
        0X65,
        EM_UDI_AUD_STREAM_MPEG1,
        CSUDI_FALSE,
		44100,
		16,
		2
    }    
};
static AudioTestChannel_S s_sAudioChannels[] = 
{
  	{EM_UDIAUDIO_CHANNEL_STEREO,		"STEREO"},
  	{EM_UDIAUDIO_CHANNEL_LEFT,			"LEFT"},
  	{EM_UDIAUDIO_CHANNEL_RIGHT,		"RIGHT"},
  	{EM_UDIAUDIO_CHANNEL_MIXED_MONO,	"MONO"}
};

static char s_aValue[64];
#define AUDIOCFG2VALUE_EUM(section,key)  \
	(memset(s_aValue, '\0', sizeof(s_aValue)) != NULL)? \
	((CSTKGetConfigInfo(section,#key, s_aValue, sizeof(s_aValue)) == CS_TK_CONFIG_SUCCESS) ? \
        CSTKGetIntFromStr(s_aValue,10) > 0 ? key : 0    \
        :0) \
        :0 
        
static char *cButTmp;
#define AUDIOCFG2STRING(key)\
	if(psOutput->uAudOutput & (EM_UDIAOUT_##key)) \
	{ \
	memcpy(cButTmp, #key, strlen(#key)); \
	strcat(cButTmp, ","); \
	cButTmp += (strlen(#key) + strlen(",")); \
	}

static CSUDI_BOOL IsAOUTSupport(CSUDIAOUTDevices_E enType);

//检查相应的项是否存在配置文件中
//nSupportTypeIndex为CSUDIAUDStreamType_E枚举定义中相应的值
CSUDI_BOOL AUDIO_iCheckCapability(int nAudioIndex,int nType/*decode or bypass*/,int nSupportTypeIndex)
{
	char cBuf[32];
	char cKey[64];
	
	if(nSupportTypeIndex >= EM_UDI_AUD_STREAMTYPE_NUM || nType >=2) return CSUDI_FALSE;
	
	memset(cKey, 0, sizeof(cKey));

	//组成配置项
	sprintf(cKey,"AUDIO_%d_%s_%s",nAudioIndex ,g_pcAudioMode[nType],g_pcAudioStreamType[nSupportTypeIndex]);
	
	memset(cBuf, '\0', sizeof(cBuf));

	if(CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("AUDIO", cKey, cBuf, sizeof(cBuf))) 
	{
		CSTCPrint("Can't find cfg %s\n",cKey);
		return CSUDI_FALSE;
	}

	if(cBuf[0] == '1')
	{
		return CSUDI_TRUE;
	}
	return CSUDI_FALSE;
}

//是否支持相应流类型的bypass输出
//nType 为CSUDIAUDStreamType_E枚举定义支持的类型
CSUDI_BOOL AUDIO_iIsSupportBypassType(int nAudioIndex,int nType)
{
	int nSupportIndex;
	CSUDIAUDIOCapability_S sCapablity;
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	if(CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity) != CSUDI_SUCCESS)
	{
		CSTCPrint("Get Audio (%d) capability failure !!\n",nAudioIndex);
		return CSUDI_FALSE;
	}
	
	for(nSupportIndex = 0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportByPass[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
	{
		if(sCapablity.m_sSupportByPass[nSupportIndex] == nType)
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}
//是否支持相应流类型的解码输出
//nType 为CSUDIAUDStreamType_E枚举定义支持的类型
CSUDI_BOOL AUDIO_iIsSupportDecodeType(int nAudioIndex,int nType)
{
	int nSupportIndex;
	CSUDIAUDIOCapability_S sCapablity;
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	if(CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity) != CSUDI_SUCCESS)
	{
		CSTCPrint("Get Audio (%d) capability failure !!\n",nAudioIndex);
		return CSUDI_FALSE;
	}
	
	for(nSupportIndex = 0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportDecode[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
	{
		if(sCapablity.m_sSupportDecode[nSupportIndex] == nType)
		{
			return CSUDI_TRUE;
		}
	}
	return CSUDI_FALSE;
}

//获取支持decode相应数据类型的解码器索引
//nType 为CSUDIAUDStreamType_E枚举定义支持的类型
CSUDI_BOOL AUDIO_iGetAudioIndexWithDecodeByType(int nType,int * nAudioIndex)
{
	int nAudioDecCount = 0;
	int ii;
	if(CSUDI_SUCCESS != CSUDIAUDIOGetCount(&nAudioDecCount) && nAudioDecCount != 0)
	{
		CSTCPrint(" Audio Count Failure !!!");
		return CSUDI_FALSE;
	}

	for(ii = 0; ii < nAudioDecCount; ii++)
	{
		if(AUDIO_iIsSupportDecodeType(ii,nType))	
			break;
	}

	if(ii != nAudioDecCount)
	{
		*nAudioIndex = ii;
		return CSUDI_TRUE;
	}
	
	CSTCPrint("There's no audio support type %d !!!!\n",nType);
	return CSUDI_FALSE;
}

//获取支持bypass相应数据类型的解码器索引
//nType 为CSUDIAUDStreamType_E枚举定义支持的类型
CSUDI_BOOL AUDIO_iGetAudioIndexWithBypassByType(int nType,int * nAudioIndex)
{
	int nAudioDecCount = 0;
	int ii;
	if(CSUDI_SUCCESS != CSUDIAUDIOGetCount(&nAudioDecCount) && nAudioDecCount != 0)
	{
		CSTCPrint(" Audio Count Failure !!!");
		return CSUDI_FALSE;
	}

	for(ii = 0; ii < nAudioDecCount; ii++)
	{
		if(AUDIO_iIsSupportBypassType(ii,nType))	
			break;
	}

	if(ii != nAudioDecCount)
	{
		*nAudioIndex = ii;
		return CSUDI_TRUE;
	}
	
	CSTCPrint("There's no audio support type %d !!!!\n",nType);
	return CSUDI_FALSE;
}
static CSUDI_BOOL AUDIO_iGetAudioOutputNameFromCfg(AudioTestOutputCfg_S * psOutput)
{
	if(psOutput == CSUDI_NULL) return CSUDI_FALSE;
	
	psOutput->uAudOutput = 0;
	memset(psOutput->cAudOutputName, '\0', sizeof(psOutput->cAudOutputName));

	cButTmp = psOutput->cAudOutputName;

	psOutput->uAudOutput |= AUDIOCFG2VALUE_EUM("AUDIO",EM_UDIAOUT_RCA);
	psOutput->uAudOutput |= AUDIOCFG2VALUE_EUM("AUDIO",EM_UDIAOUT_SPDIF);
	psOutput->uAudOutput |= AUDIOCFG2VALUE_EUM("AUDIO",EM_UDIAOUT_HDMI);

	AUDIOCFG2STRING(RCA);
	AUDIOCFG2STRING(SPDIF);
	AUDIOCFG2STRING(HDMI);
	
	CSTCPrint("uAudOutput Type = %x , cAudOutputName = %s\r\n",  \
		psOutput->uAudOutput, psOutput->cAudOutputName);
	return CSUDI_TRUE;
}

static CSUDI_BOOL AUDIO_iCheckPlay(int nAudioIndex,AudioTestServiceInfo_S * pService,AudioTestOutputCfg_S * psOutput)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	if(pService == CSUDI_NULL) return CSUDI_FALSE;
	
	CSTCPrint("开始播放码流[%s]上的节目[%s]...\r\n",pService->m_pcTsFilename,pService->m_pcServiceDescript);
	CSTCPrint("当前播放音频为:%s,数据格式为:%s.....\r\n", \
		pService->m_pcServiceDescript, (pService->m_eStreamType == EM_UDI_AUD_STREAM_AC3)?"AC3":"非AC3");

	if((pService->m_nPid == 0) || (pService->m_nPid == 0x1fff))
	{
		CSTCPrint("期望:无音频输出......\r\n");
	}
	else
	{
		switch(pService->m_eStreamType)
		{
			case EM_UDI_AUD_STREAM_AC3:
				if ((psOutput->uAudOutput) & EM_UDIAOUT_SPDIF)
				{
					CSTCPrint("期望:SPDIF音箱有声音输出........\r\n");							
				}
				
				if(AUDIO_iIsSupportDecodeType(nAudioIndex,EM_UDI_AUD_STREAM_AC3))
				{
					if((psOutput->uAudOutput) & EM_UDIAOUT_SPDIF)
					{
						CSTCPrint("期望:被测平台能解AC3数据，要求所有合法设备均有声音输出.....\r\n");
					}				
				}
				else
				{
					if((psOutput->uAudOutput) & EM_UDIAOUT_SPDIF)
					{
						CSTCPrint("期望:被测平台不能解AC3数据，要求音箱之外的合法设备均无声音输出.....\r\n");
					}				
				}
				break;

			default:
				CSTCPrint("期望:所有设备(%s)都有音频输出.\r\n", psOutput->cAudOutputName);
				break;
		}
	
	}

	CSTCPrint("请确认以上音频在所有支持的设备上播放正常,没有停顿、爆音等异常现象\r\n");
	CSTCPrint("0:Yes,与期望现象相同 .\r\n");
	CSTCPrint("1:No,与期望现象不同.\r\n");
	bRet = CSTKWaitYes(); 		
	return bRet;
}

static void AUDIO_iSetDefault(int nAudioIndex)
{
	if(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(nAudioIndex,AUDIO_DEFAULT_VOLUME))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAUDIOSetVolume failed!\r\n");
	}
		
	if(CSUDI_SUCCESS != CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAUDIOSetChannel failed!\r\n");
	}
	
	if(CSUDI_SUCCESS != CSUDIAUDIOUnmute(nAudioIndex))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAUDIOUnmute failed!\r\n");
	}
	
	if(CSUDI_SUCCESS != CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_DECODE))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAOUTSetOPMode EM_UDIAOUT_SPDIF failed!\r\n");
	}
	
	if(CSUDI_SUCCESS != CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE))
	{
		CSTCPrint("[AUDIO_iSetDefault] CSUDIAOUTSetOPMode EM_UDIAOUT_HDMI failed!\r\n");
	}
}

static CSUDI_BOOL AUDIO_iStartPlay(int nAudioIndex, AudioTestServiceInfo_S * pService,AudioTestOutputCfg_S * psOutput,CSUDI_HANDLE * phPlayer,CSUDI_BOOL bSetDefault)
{
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	if(pService == CSUDI_NULL || phPlayer == CSUDI_NULL || psOutput == CSUDI_NULL)
	{
		CSTCPrint("pService == CSUDI_NULL || phPlayer == CSUDI_NULL|| psOutput == CSUDI_NULL!!!\r\n");
		return CSUDI_FALSE;
	}

	if(CSUDI_TRUE != CSTC_UDI2PortingLock(0, pService->m_pcTsFilename))
	{
		CSTCPrint("Lock failure !!!\r\n");
		return CSUDI_FALSE;
	}
	
	if(CSUDI_TRUE != AUDIO_iGetAudioOutputNameFromCfg(psOutput))
	{
		CSTCPrint("Lock failure !!!\r\n");
		return CSUDI_FALSE;
	}
	
	sPlayerChnl.m_nAudioDecoder = 0;
	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nVideoDecoder = 0;



	if(CSUDI_SUCCESS != CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) || hPlayer == CSUDI_NULL)
	{
		CSTCPrint("[%s:%d]Open Player failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	
	*phPlayer = hPlayer;//一旦open成功就应该记录hPlayer,否则播放失败后无法关闭。

	sStreamInfo.m_nPid = pService->m_nPid;
	sStreamInfo.m_uStreamType.m_eAudioType  = pService->m_eStreamType;
	sStreamInfo.m_eContentType = EM_UDI_CONTENT_AUDIO;

	
	if(CSUDI_SUCCESS != CSUDIPLAYERSetStream(hPlayer, &sStreamInfo, 1, CSUDI_NULL))
	{
		CSTCPrint("[%s:%d]Set Stream failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	if(bSetDefault)
	{
		AUDIO_iSetDefault(nAudioIndex);  ///set default
	}
	
	if(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer))
	{
		CSTCPrint("[%s:%d]Start Player failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}	
	
	CSUDIOSThreadSleep(1000);

	return CSUDI_TRUE;
	
}

static CSUDI_BOOL AUDIO_iStopPlay(int nAudioIndex,CSUDI_HANDLE hPlayer,CSUDI_BOOL bSetDefault)
{
	if(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer))
	{
		CSTCPrint("[%s:%d]Stop Player failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	
	if(bSetDefault)
	{
		AUDIO_iSetDefault(nAudioIndex);  ///set default
	}
	

	if(CSUDI_SUCCESS != CSUDIPLAYERClose(hPlayer))
	{
		CSTCPrint("[%s:%d]Close Player failure !!! \r\n", __FUNCTION__, __LINE__);
		return CSUDI_FALSE;
	}
	
	
	CSTCPrint("停止播放音频..............!!\r\n");
	
	return CSUDI_TRUE;
}

static CSUDI_BOOL  AUDIO_iGetChannelInfo(const AudioTestChannel_S * psAudioChannel,  char **pcAudioChannelInfo)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	if((NULL != psAudioChannel)&&(NULL != pcAudioChannelInfo))
	{
		*pcAudioChannelInfo = (char *)(psAudioChannel->m_cChannelInfo);
		bRet = CSUDI_TRUE;
	}
	return bRet;
}

static CSUDI_BOOL  AUDIO_iCheckChannel(AudioTestChannel_S * psAudioChannel, unsigned int uAudOutputType)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;

	CSTCPrint("[AUDIO]*******************************************************\r\n");
	CSTCPrint("[AUDIO]动作:设置声道: \r\n");
	
	CSTCPrint("[AUDIO]期望:当前声道为%s  .\r\n", psAudioChannel->m_cChannelInfo);
	{
		if(uAudOutputType & EM_UDIAOUT_RCA)
		{
			switch(psAudioChannel->m_eChannel)
			{
			case EM_UDIAUDIO_CHANNEL_STEREO:
				CSTCPrint("[AUDIO]1.同时听到广播和电视的声音.\r\n");
				CSTCPrint("[AUDIO]2.拔掉红色输出线（白色输出有效），听到电视节目的声音；拔掉白色输出线（红色输出有效），听到广播节目的声音.\r\n");
				CSTCPrint("[AUDIO]3.切换瞬间无噪音.\r\n");\
				CSTCPrint("[AUDIO]4.切换声道时视频无闪烁.\r\n");
				break;
				
			case EM_UDIAUDIO_CHANNEL_LEFT:
				CSTCPrint("[AUDIO]1.不管是拔白色还是红色输出线，都应该能听到电视节目的声音.\r\n");
				CSTCPrint("[AUDIO]2.切换瞬间无噪音.\r\n");
				CSTCPrint("[AUDIO]3.切换声道时视频无闪烁.\r\n");				
				break;
				
			case EM_UDIAUDIO_CHANNEL_RIGHT:
				CSTCPrint("[AUDIO]1.不管是拔白色还是红色输出线，都应该能听到广播节目的声音.\r\n");
				CSTCPrint("[AUDIO]2.切换瞬间无噪音.\r\n");
				CSTCPrint("[AUDIO]3.切换声道时视频无闪烁.\r\n");				
				break;
				
			case EM_UDIAUDIO_CHANNEL_MIXED_MONO:
				CSTCPrint("[AUDIO]1.同时听到广播和电视的声音.\r\n");
				CSTCPrint("[AUDIO]2.不管拔掉红色还是白色输出线（保证一根输出线有效），均能听到电视节目和广播节目的混合声.\r\n");
				CSTCPrint("[AUDIO]3.切换瞬间无噪音.\r\n");
				CSTCPrint("[AUDIO]4.切换声道时视频无闪烁.\r\n");				
				break;
				
			default:
				return CSUDI_FALSE;
				break;
			}
		}
	}
	
	CSTCPrint("0:Yes,与期望现象相同 .\r\n");
	CSTCPrint("1:No,与期望现象不同.\r\n");
	CSTCPrint("[AUDIO]*******************************************************\r\n");

	bRet = CSTKWaitYes();
	
	return bRet;

}
static CSUDI_BOOL AUDIO_iCheckVolume(CSUDI_BOOL bHasVolume)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	if(bHasVolume)
	{
		CSTCPrint("期望所有合法设备均有声音输出,操作瞬间无噪音...........!!\r\n");
		CSTCPrint("0:Yes,与期望现象相同 .\r\n");
		CSTCPrint("1:No,与期望现象不同.\r\n");
		bRet = CSTKWaitYes();
	}
	else
	{
		CSTCPrint("期望所有合法设备均无声音输出,操作瞬间无噪音...........!!\r\n");
		CSTCPrint("0:Yes,与期望现象相同 .\r\n");
		CSTCPrint("1:No,与期望现象不同.\r\n");
		bRet = CSTKWaitYes();
	}
	
	return bRet;
}


static int AUDIO_iGetNextServiceInx(int nServiceInx)
{
	int nIndex ;
	
	if(EM_AUDIO_MULTILANGUAGE2 <= nServiceInx)
	{
		nIndex = EM_AUDIO_MULTILANGUAGE1;
	}
	else
	{
		nIndex = EM_AUDIO_MULTILANGUAGE2;
	}
	
	return nIndex;
}

CSUDI_BOOL CSTC_AUDIO_Init(void)
{
	//在本测试用例集执行前调用
	char cBuf[32];
	memset(cBuf, 0, sizeof(cBuf));
	if(CSTKGetConfigInfo("AUDIO","AUDIO_NUMBER",cBuf,sizeof(cBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		s_nAudioCount = CSTKGetIntFromStr(cBuf,10);    
	}
	else
	{
		CSTCPrint("[AUDIO]字段中找不到AUDIO_NUMBER\r\n");
	}
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_AUDIO_UnInit(void)
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

//@CASEGROUP:CSUDIAUDIOGetCount(int * pnAudioDecCount)
//@DESCRIPTION: 测试参数为合法值的情况
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、配置文件提供系统支持的音频解码的个数
//@INPUT:  pnAudioDecCount合法指针
//@EXPECTATION: 成功返回CSUDI_SUCCESS，并且获取的音频解码器个数与配置文件中的个数一致
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、验证获取的音频解码器个数，期望与配置文件数据相符
CSUDI_BOOL CSTC_AUDIO_GetCount_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTCPrint("Audio Count %d! Expected count is %d\n", nAudioDecCount,s_nAudioCount);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount == s_nAudioCount),
			"Step 2 Check Audio Count Failure !!!"
		);
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetCount(int * pnAudioDecCount)
//@DESCRIPTION: 测试参数为非法值CSUDI_NULL的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  pnAudioDecCount=CSUDI_NULL
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 调用CSUDIAUDIOGetCount，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_GetCount_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetCount(CSUDI_NULL)),
			"Get Audio Count should be Failure !!!"
		);
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}
//@CASEGROUP:CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo)
//@DESCRIPTION: 测试所有参数为合法值的情况
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、配置文件提供系统支持的音频解码的个数
//@PRECONDITION: 3、配置文件提供每个音频解码器的能力信息
//@INPUT: 1、nAudioIndex = 系统存在的所有Audio索引值
//@INPUT: 2、psCapabilityInfo = CSUDIAUDIOCapability_S结构指针
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount得到Audio的数目nAudio，期望成功获取系统支持的个数
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOGetCapability获取所有nAudio的能力信息，期望能成功获取
//@EXECUTIONFLOW: 3、校验每个Audio的能力信息，期望与配置文件中的信息一致
CSUDI_BOOL CSTC_AUDIO_GetCapability_0001(void)
{
	CSUDIAUDIOCapability_S sCapablity;
	int nAudioDecCount = 0;
	int nAudioIndex;
	int nSupportIndex;
	
	memset(&sCapablity,EM_UDI_AUD_STREAM_UNKNOWN,sizeof(CSUDIAUDIOCapability_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);
	for(nAudioIndex = 0; nAudioIndex < nAudioDecCount; nAudioIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(nAudioIndex,&sCapablity)),
				"Step 2 Get  Audio Capability Failure !!!"
			);
		//测试decode模式支持的数据类型
		for(nSupportIndex=0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportDecode[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iCheckCapability(nAudioIndex,EM_UDIAOUT_DECODE,sCapablity.m_sSupportDecode[nSupportIndex])),
					"Step 3 Check Decode mode Audio Capability Failure !!!"
				);
		}
		//测试bypass模式支持的数据类型
		for(nSupportIndex=0; nSupportIndex < EM_UDI_AUD_STREAMTYPE_NUM && sCapablity.m_sSupportByPass[nSupportIndex] != EM_UDI_AUD_STREAM_UNKNOWN; nSupportIndex++)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iCheckCapability(nAudioIndex,EM_UDIAOUT_BYPASS,sCapablity.m_sSupportByPass[nSupportIndex])),
					"Step 3 Check Bypass mode Audio Capability Failure !!!"
				);
		}
	}
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo)
//@DESCRIPTION: 测试音频索引值为非法值的情况
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统存在音频解码器
//@INPUT: 1、nAudioIndex = -1，nAudio，nAudio+1(nAudio为系统支持的个数)
//@INPUT: 2、psCapabilityInfo = CSUDIAUDIOCapability_S结构指针
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount得到Audio的数目nAudio，期望成功获取系统支持的个数
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOGetCapability(-1,psCapabilityInfo)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOGetCapability(nAudio,psCapabilityInfo)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOGetCapability(nAudio+1,psCapabilityInfo)，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_GetCapability_0002(void)
{	
	int nAudioDecCount = 0;
	CSUDIAUDIOCapability_S sCapablity;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetCapability(-1,&sCapablity)),
			"Step 2 Get  Audio Capability with -1 index should be Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetCapability(nAudioDecCount,&sCapablity)),
			"Step 2 Get  Audio Capability with nAudioDecCount index should be Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetCapability(nAudioDecCount+1,&sCapablity)),
			"Step 2 Get  Audio Capability with nAudioDecCount+1 index should be Failure !!!"
		);
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo)
//@DESCRIPTION: 测试能力信息结构体指针非法值CSUDI_NULL的情况
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统存在音频解码器
//@INPUT: 1、nAudioIndex = 系统存在的所有Audio索引值
//@INPUT: 2、psCapabilityInfo = CSUDI_NULL
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount得到Audio的数目nAudio，期望成功获取系统支持的个数
//@EXECUTIONFLOW: 2、遍历系统中所有音频解码器调用CSUDIAUDIOGetCapability(nAudioIndex,CSUDI_NULL)，期望每一步返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_GetCapability_0003(void)
{
	int nAudioDecCount = 0;
	int ii;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);
	for(ii = 0; ii < nAudioDecCount; ii++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS != CSUDIAUDIOGetCapability(ii,CSUDI_NULL)),
				"Get Audio Count should be Failure !!!"
			);
	}
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
//@DESCRIPTION: 测试参数为非法值的情况
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统存在音频解码器
//@INPUT: 1、nAudioIndex = -1，nAudio，nAudio+1(nAudio为系统支持的个数)
//@INPUT: 2、psStatus = CSUDIAUDIOStatus_S结构指针，CSUDI_NULL
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount得到Audio的数目nAudio，期望成功获取系统支持的个数
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOGetStatus(-1,psStatus)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOGetStatus(nAudio,psStatus)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOGetStatus(nAudio+1,psStatus)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、对于每一个audio依次调用CSUDIAUDIOGetStatus(nAudioindex,CSUDI_NULL)，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_GetStatus_0001(void)
{
	int nAudioDecCount = 0;
	int nAudioIndex = 0;
	CSUDIAUDIOStatus_S sStatus;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetStatus(-1,&sStatus)),
			"Step 2 Get  Audio Status with -1 index should be Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetStatus(nAudioDecCount,&sStatus)),
			"Step 3 Get  Audio Status with nAudioDecCount index should be Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOGetStatus(nAudioDecCount+1,&sStatus)),
			"Step 4 Get  Audio Status with nAudioDecCount+1 index should be Failure !!!"
		);
	
	for(nAudioIndex=0; nAudioIndex<nAudioDecCount; ++nAudioIndex)
	{
		CSTCPrint("Audio Index %d \r\n",nAudioIndex);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS != CSUDIAUDIOGetStatus(nAudioIndex,CSUDI_NULL)),
				"Step 5 Get  Audio Status with  psStatus=CSUDI_NULL should be Failure !!!"
			);
	}
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
//@DESCRIPTION: 测试能够正确获取AC3音频类型的音频状态信息
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统存在支持AC3类型的音频解码器
//@PRECONDITION: 3、能够正常播放音频流
//@INPUT: 1、nAudioIndex = 支持AC3类型的音频解码器
//@INPUT: 2、psStatus = CSUDIAUDIOStatus_S结构指针
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount和CSUDIAUDIOGetCapability获取支持AC3类型的音频解码器索引值nAudio
//@EXECUTIONFLOW: 2、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 3、在nAudioIndex上播放"DolbyAC3标准测试节目"，期望播放成功
//@EXECUTIONFLOW: 4、确认播放成功后，调用CSUDIAUDIOGetStatus，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、校验获取的音频状态信息，期望与码流中的信息一致
//@EXECUTIONFLOW: 6、停止nAudioIndex播放
CSUDI_BOOL CSTC_AUDIO_GetStatus_0002(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIAUDIOStatus_S sStatus;	
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	memset(&sStatus,0,sizeof(CSUDIAUDIOStatus_S));
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));

	bRet = AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex);
	if(bRet == CSUDI_FALSE)
	{
		CSTCPrint("\rThere's no audio support EM_UDI_AUD_STREAM_AC3 !!!!\r\n");
		return CSUDI_TRUE;
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 3 start play DolbyAC3标准测试节目 failure!!!!"
		);

	CSTCPrint("Step 3 start play DolbyAC3标准测试节目\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
			"step 3 Check play DolbyAC3标准测试节目 failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetStatus(nAudioIndex,&sStatus)),
			"Step 4 Get audio status failure !!!!"
		);
	
	CSTCPrint("Audio Status: sStatus.m_nSampleRate == %d \t sStatus.m_nBitWidth == %d \t sStatus.m_nChannels==%d \n ",
						sStatus.m_nSampleRate,sStatus.m_nBitWidth,sStatus.m_nChannels);
		
#if 0 /*<!-- yangpengchao 2010/3/12 17:32:26 */
	CSTK_ASSERT_TRUE(
			(CSUDI_TRUE == AUDIO_iCheckStatus(&sStatus,&s_sAudioServices[EM_AUDIO_DOLBYAC3])),
			"Step 5 check audio status failure !!!!"
		);
#endif /*0*//* yangpengchao 2010/3/12 17:32:26 --!>*/
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_DOLBYAC3].m_nSampleRate == sStatus.m_nSampleRate),
			"Step 5 check audio status failure !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_DOLBYAC3].m_nBitWidth == sStatus.m_nBitWidth),
			"Step 5 check audio status failure !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_DOLBYAC3].m_nChannels == sStatus.m_nChannels),
			"Step 5 check audio status failure !!!!"
		);
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 6 stop play failure !!!!"
			);
	}
	hPlayer = CSUDI_NULL;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
//@DESCRIPTION: 测试能够正确获取MPEG2音频类型的音频状态信息
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统存在支持MPEG2类型的音频解码器
//@PRECONDITION: 3、能够正常播放音频流
//@INPUT: 1、nAudioIndex = 支持MPEG1类型的音频解码器
//@INPUT: 2、psStatus = CSUDIAUDIOStatus_S结构指针
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、通过CSUDIAUDIOGetCount和CSUDIAUDIOGetCapability获取支持MPEG2类型的音频解码器索引值nAudio
//@EXECUTIONFLOW: 2、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 3、在nAudioIndex上播放"还珠格格"，期望播放成功
//@EXECUTIONFLOW: 4、确认播放成功后，调用CSUDIAUDIOGetStatus，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、校验获取的音频状态信息，期望与码流中的信息一致
//@EXECUTIONFLOW: 6、停止nAudio播放
CSUDI_BOOL CSTC_AUDIO_GetStatus_0003(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIAUDIOStatus_S sStatus;	
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sStatus,0,sizeof(CSUDIAUDIOStatus_S));
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 3 start play MPEG2音频测试节目 failure!!!!"
		);

	CSTCPrint("Step 3 start play MPEG2音频测试节目\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 3 Check play MPEG2音频测试节目 failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetStatus(nAudioIndex,&sStatus)),
			"Step 4 Get audio status failure !!!!"
		);
	
	CSTCPrint("Audio Status: sStatus.m_nSampleRate == %d \t sStatus.m_nBitWidth == %d \t sStatus.m_nChannels==%d \n ",
						sStatus.m_nSampleRate,sStatus.m_nBitWidth,sStatus.m_nChannels);

	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MULTILANGUAGE1].m_nSampleRate == sStatus.m_nSampleRate),
			"Step 5 check audio status failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MULTILANGUAGE1].m_nBitWidth == sStatus.m_nBitWidth),
			"Step 5 check audio status failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MULTILANGUAGE1].m_nChannels == sStatus.m_nChannels),
			"Step 5 check audio status failure !!!!"
		);

	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 6 stop play failure !!!!"
			);
	}
	hPlayer = CSUDI_NULL;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
//@DESCRIPTION: 测试能够正确获取MPEG1音频类型的音频状态信息
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、系统存在支持MPEG1类型的音频解码器
//@PRECONDITION: 3、能够正常播放音频流
//@INPUT: 1、nAudioIndex = 支持MPEG1类型的音频解码器
//@INPUT: 2、psStatus = CSUDIAUDIOStatus_S结构指针
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、通过CSUDIAUDIOGetCount和CSUDIAUDIOGetCapability获取支持MPEG1类型的音频解码器索引值nAudio
//@EXECUTIONFLOW: 2、锁定H.264.ts码流所在的频点
//@EXECUTIONFLOW: 3、在nAudio上播放"MPEG1视频测试节目"，期望播放成功
//@EXECUTIONFLOW: 4、确认播放成功后，调用CSUDIAUDIOGetStatus，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、校验获取的音频状态信息，期望与码流中的信息一致
//@EXECUTIONFLOW: 6、停止nAudio播放
CSUDI_BOOL CSTC_AUDIO_GetStatus_0004(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIAUDIOStatus_S sStatus;	
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sStatus,0,sizeof(CSUDIAUDIOStatus_S));
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG1,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG1 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MPEG1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 3 start play MPEG1音频测试节目 failure!!!!"
		);

	CSTCPrint("Step 3 start play MPEG1音频测试节目\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MPEG1],&sOutputCfg)),
			"step 3 Check play MPEG1音频测试节目 failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetStatus(nAudioIndex,&sStatus)),
			"Step 4 Get audio status failure !!!!"
		);
	
	CSTCPrint("Audio Status: sStatus.m_nSampleRate == %d \t sStatus.m_nBitWidth == %d \t sStatus.m_nChannels==%d \n ",
						sStatus.m_nSampleRate,sStatus.m_nBitWidth,sStatus.m_nChannels);

	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MPEG1].m_nSampleRate == sStatus.m_nSampleRate),
			"Step 5 check audio status failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MPEG1].m_nBitWidth == sStatus.m_nBitWidth),
			"Step 5 check audio status failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(s_sAudioServices[EM_AUDIO_MPEG1].m_nChannels == sStatus.m_nChannels),
			"Step 5 check audio status failure !!!!"
		);

	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 6 stop play failure !!!!"
			);
	}
	hPlayer = CSUDI_NULL;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: 测试nAudioIndex为非法值的情况
//@PRECONDITION: 平台成功初始化
//@INPUT: 1、nAudioIndex = -1，nAudio，nAudio+1(nAudio为系统支持的个数)
//@INPUT: 2、eChannelType = 系统支持的类型(EM_UDIAUDIO_CHANNEL_STEREO)
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount得到Audio的数目nAudio，期望成功获取系统支持的个数
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOSetChannel(-1,eChannelType)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOSetChannel(nAudio,eChannelType)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOSetChannel(nAudio+1,eChannelType)，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_SetChannel_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetChannel(-1,EM_UDIAUDIO_CHANNEL_STEREO)),
			"Step 2 set audio channel with -1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetChannel(nAudioDecCount,EM_UDIAUDIO_CHANNEL_STEREO)),
			"Step 3 set audio channel with nAudioDecCount index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetChannel(nAudioDecCount+1,EM_UDIAUDIO_CHANNEL_STEREO)),
			"Step 4 set audio channel with nAudioDecCount+1 index should be Failure !!!"
		);
	
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: 测试能够正常切换声道
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流，音频输出通道为EM_UDIAOUT_RCA(常规音频左右声道输出)
//@INPUT: 1、nAudioIndex = 0
//@INPUT: 2、eChannelType = 系统支持的类型CSUDIAUDIOChannelType_E
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、播放"还珠格格",期望播放成功
//@EXECUTIONFLOW: 3、确认播放成功后，遍历所有支持的声道调用CSUDIAUDIOSetChannel(nAudioIndex，eChannelType)
//@EXECUTIONFLOW: 声道为EM_UDIAUDIO_CHANNEL_STEREO的测试步骤
//@EXECUTIONFLOW: 	1、同时听到广播和电视的声音
//@EXECUTIONFLOW: 	2、拔掉红色输出线（白色输出有效），听到电视节目的声音；拔掉白色输出线（红色输出有效），听到广播节目的声音
//@EXECUTIONFLOW: 	3、切换瞬间无噪音
//@EXECUTIONFLOW: 声道为EM_UDIAUDIO_CHANNEL_LEFT的测试步骤
//@EXECUTIONFLOW: 	1、不管是拔白色还是红色输出线，都应该能听到电视节目的声音
//@EXECUTIONFLOW: 	2、切换瞬间无噪音
//@EXECUTIONFLOW: 声道为EM_UDIAUDIO_CHANNEL_RIGHT的测试步骤
//@EXECUTIONFLOW: 	1、不管是拔白色还是红色输出线，都应该能听到广播节目的声音
//@EXECUTIONFLOW: 	2、切换瞬间无噪音
//@EXECUTIONFLOW: 声道为EM_UDIAUDIO_CHANNEL_MIXED_MONO的测试步骤
//@EXECUTIONFLOW: 	1、不管拔掉红色还是白色输出线（保证一根输出线有效），均能听到电视节目和广播节目的混合声
//@EXECUTIONFLOW: 	2、切换瞬间无噪音
//@EXECUTIONFLOW: 4、停止nAudioIndex播放
CSUDI_BOOL CSTC_AUDIO_SetChannel_0002(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int jj;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play 还珠格格 failure!!!!"
		);

	CSTCPrint("Step 2 start play 还珠格格\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play 还珠格格 failure!!!!"
		);
	
	for(jj=0;jj < sizeof(s_sAudioChannels)/sizeof(AudioTestChannel_S); jj++)
	{
		CSTCPrint("声道为EM_UDIAUDIO_CHANNEL_%s的测试步骤\n",s_sAudioChannels[jj].m_cChannelInfo);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(AUDIO_INDEX_0,s_sAudioChannels[jj].m_eChannel)),
				"step 2 set Channel failure!!!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[jj],sOutputCfg.uAudOutput)),
				"step 3 check Channel failure!!!!"
			);
	}
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
				"Step 6 stop play failure !!!!"
			);
	}
	hPlayer = CSUDI_NULL;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: 播放一个单声道MPEG码流，设置成码流不存在的声道时表现为下面两种现象之一的即可认为正常：
//@DESCRIPTION: 1.不应该有声音输出，并且电视机不出现异常声音
//@DESCRIPTION: 2.用存在的声道内容替换不存在的声道
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流，音频输出通道为EM_UDIAOUT_RCA(常规音频左右声道输出)
//@INPUT: 1、nAudioIndex = 0
//@INPUT: 2、eChannelType = 系统支持的类型CSUDIAUDIOChannelType_E
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、锁定左声道TBD 码流所在的频点
//@EXECUTIONFLOW: 2、播放"TBD",期望播放成功
//@EXECUTIONFLOW: 3、确认播放成功后，遍历所有支持的声道调用CSUDIAUDIOSetChannel(nAudioIndex，eChannelType)
//@EXECUTIONFLOW: 声道为EM_UDIAUDIO_CHANNEL_STEREO的测试步骤
//@EXECUTIONFLOW: 4、停止nAudioIndex播放
CSUDI_BOOL CSTC_AUDIO_SetChannel_0003(void)
{
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIAUDIOMute(int nAudioIndex)
//@DESCRIPTION: 测试nAudioIndex为非法值的情况
//@PRECONDITION: 平台成功初始化
//@INPUT: nAudioIndex = -1，nAudio，nAudio+1(nAudio为系统支持的个数)
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount得到Audio的数目nAudio，期望成功获取系统支持的个数
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOMute(-1)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOMute(nAudio)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOMute(nAudio+1)，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_Mute_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOMute(-1)),
			"Step 2 set mute with -1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOMute(nAudioDecCount)),
			"Step 3 set mute with nAudioDecCount index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOMute(nAudioDecCount+1)),
			"Step 4 set mute with nAudioDecCount+1 index should be Failure !!!"
		);
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}
//@CASEGROUP:CSUDIAUDIOUnmute(int nAudioIndex)
//@DESCRIPTION: 测试nAudioIndex为非法值的情况
//@PRECONDITION: 平台成功初始化
//@INPUT: nAudioIndex =  -1，nAudio，nAudio+1(nAudio为系统支持的个数)
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount得到Audio的数目nAudio，期望成功获取系统支持的个数
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOUnmute(-1)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOUnmute(nAudio)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOUnmute(nAudio+1)，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_Unmute_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOUnmute(-1)),
			"Step 2 set unmute with -1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOUnmute(nAudioDecCount)),
			"Step 3 set unmute with nAudioDecCount index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOUnmute(nAudioDecCount+1)),
			"Step 4 set unmute with nAudioDecCount+1 index should be Failure !!!"
		);
	
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: 测试nAudioIndex和nVolume为非法值的情况
//@PRECONDITION: 平台成功初始化
//@INPUT: 1、nAudioIndex = -1，nAudio，nAudio+1(nAudio为系统支持的个数)
//@INPUT: 2、nVolume = -1，32 (音量[0,31]为合法值)
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、调用CSUDIAUDIOGetCount得到Audio的数目nAudio，期望成功获取系统支持的个数
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOSetVolume(-1,0)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOSetVolume(nAudio,0)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOSetVolume(nAudio+1,0)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIAUDIOSetVolume(0，-1)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIAUDIOSetVolume(0，32)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIAUDIOSetVolume(-1，-1)，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSUDIAUDIOSetVolume(-1，32)，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AUDIO_SetVolume_0001(void)
{
	int nAudioDecCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioDecCount)),
			"Step 1 Audio Count Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(nAudioDecCount != 0),
			"Audio Count shoulde not be 0 !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(-1,0)),
			"Step 2 set Volume with -1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(nAudioDecCount,0)),
			"Step 3 set Volume with nAudioDecCount index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(nAudioDecCount+1,0)),
			"Step 4 set Volume with nAudioDecCount+1 index should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(0,-1)),
			"Step 5 set Volume with Volume -1 should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(0,32)),
			"Step 4 set Volume with audio_index 0 and volume 32 should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(-1,-1)),
			"Step 4 set Volume with audio_index -1 and volume -1 should be Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIAUDIOSetVolume(-1,32)),
			"Step 4 set Volume with audio_index -1 and volume 32 should be Failure !!!"
		);
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: 测试正常增加音量的功能
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@INPUT: 1、nAudioIndex = 0
//@INPUT: 2、nVolume = 从0开始，在有效音量范围内每次递增5个单位
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、播放"还珠格格",期望播放成功
//@EXECUTIONFLOW: 3、从nVolume=0开始,在有效音量范围内,nVolume每200ms递增1,调用CSUDIAUDIOSetVolume(nAudioIndex,nVolume)，期望每次调用成功
//@EXECUTIONFLOW: 4、校验每次递增音量值，期望音量有逐渐变大(0为最小音量,31为最大音量)
//@EXECUTIONFLOW: 5、停止播放音频，期望所有合法声音设备无声音输出，操作间无噪音
//@EXECUTIONFLOW: 6、重复步骤2到5 N(N=5)次，期望每一步骤都得到期望值
//@REMARK:是否需要仪器测试音量值?
CSUDI_BOOL CSTC_AUDIO_SetVolume_0002(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int jj;
	int nLoop;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play 还珠格格 failure!!!!"
		);

	CSTCPrint("Step 2 start play 还珠格格\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play 还珠格格 failure!!!!"
		);

	CSTCPrint("注意观察以下过程中所有设备音量逐渐增大!!\r\n");
	CSTKWaitAnyKey();	
	
	for(nLoop = 0; nLoop < AUDIO_TEST_TIMES; nLoop++)
	{
		for(jj = 0;jj<32;jj += 1)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,jj)),
					"Step 4 set Volume Failure !!!"
				);
			
			CSTCPrint("The current volume is : %d \r\n",jj);
			CSUDIOSThreadSleep(200);
		}
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,0)),
				"Step 4 set Volume Failure !!!"
			);
		CSTCPrint("The current volume is : 0 \r\n");
		CSUDIOSThreadSleep(1000);
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
			"Step 5 stop play failure !!!!"
		);
	hPlayer = CSUDI_NULL;
	
	CSTCPrint("期望操作过程中所有设备音量逐渐增大，停止播放间没有噪音!!\r\n");
	CSTCPrint("0:Yes,与期望现象相同 .\r\n");
	CSTCPrint("1:No,与期望现象不同.\r\n");
	bRet = CSTKWaitYes();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"Step 4 check volume failure !!!!"
		);
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
				"Step 5 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: 测试正常减小音量的功能
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@INPUT: 1、nAudioIndex = 0
//@INPUT: 2、nVolume = 从31开始，在有效音量范围内每次递减5个单位
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、播放"还珠格格",期望播放成功
//@EXECUTIONFLOW: 3、从nVolume=31开始,在有效音量范围内,nVolume每200ms递减1,调用CSUDIAUDIOSetVolume(nAudioIndex,nVolume)，期望每次调用成功
//@EXECUTIONFLOW: 4、校验每次递减音量值，期望音量有逐渐变小(0为最小音量,31为最大音量)
//@EXECUTIONFLOW: 5、停止播放音频，期望所有合法声音设备无声音输出，操作间无噪音
//@EXECUTIONFLOW: 6、重复步骤2到5 N(N=5)次，期望每一步骤都得到期望值
//@REMARK:是否需要仪器测试音量值?
CSUDI_BOOL CSTC_AUDIO_SetVolume_0003(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int jj;
	int nLoop;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play 还珠格格 failure!!!!"
		);

	CSTCPrint("Step 2 start play 还珠格格\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"check 2 check play 还珠格格 failure!!!!"
		);

	CSTCPrint("注意观察以下过程中所有设备音量逐渐减小!!\r\n");
	CSTKWaitAnyKey();	
	
	for(nLoop = 0; nLoop < AUDIO_TEST_TIMES; nLoop++)
	{

		for(jj = 31;jj>=0;jj -= 1)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,jj)),
					"Step 4 set Volume Failure !!!"
				);
			CSTCPrint("The current volume is : %d \r\n",jj);
			CSUDIOSThreadSleep(200);
		}	
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,31)),
				"Step 4 set Volume Failure !!!"
			);
		CSTCPrint("The current volume is : 31 \r\n");
		CSUDIOSThreadSleep(1000);
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
			"Step 5 stop play failure !!!!"
		);
	hPlayer = CSUDI_NULL;
	CSTCPrint("期望操作过程中所有设备音量逐渐减小，停止播放间没有噪音!!\r\n");
	CSTCPrint("0:Yes,与期望现象相同 .\r\n");
	CSTCPrint("1:No,与期望现象不同.\r\n");
	bRet = CSTKWaitYes();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"Step 4 check volume failure !!!!"
		);
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
				"Step 5 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: 将声音调成0，并将电视声音开到最大，确认无声音
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@INPUT: 1、nAudioIndex = 0
//@INPUT: 2、nVolume = 31,0
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、播放"还珠格格",期望播放成功
//@EXECUTIONFLOW: 3、nVolume=31调用CSUDIAUDIOSetVolume，期望调用成功,并能听到声音
//@EXECUTIONFLOW: 4、nVolume=0调用CSUDIAUDIOSetVolume，将接收设备音量调至最小，期望调用成功，听不到声音，操作过程中无杂音和爆音
//@EXECUTIONFLOW: 5、恢复现场	
//@REMARK:
CSUDI_BOOL CSTC_AUDIO_SetVolume_0004(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play 还珠格格 failure!!!!"
		);

	CSTCPrint("Step 2 start play 还珠格格\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(AUDIO_INDEX_0,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play 还珠格格 failure!!!!"
		);
	
	//EXECUTIONFLOW: 3、nVolume=31调用CSUDIAUDIOSetVolume，期望调用成功,并能听到声音
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,CSUDI_MAX_VOLUME)),
			"Step 3 set Volume Failure !!!"
		);
	
	CSTCPrint("Step 3 请将接收设备音量调至最大，并注意聆听设备输出\r\n");
	CSTKWaitAnyKey();	

	//EXECUTIONFLOW: 4、nVolume=0调用CSUDIAUDIOSetVolume，将接收设备音量调至最小，期望调用成功，听不到声音，操作过程中无杂音和爆音
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(AUDIO_INDEX_0,0)),
			"Step 4 set Volume Failure !!!"
		);	

	CSTCPrint("Step 4 期望操作过程中所有设备无爆音、杂音异常，输出无声音!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 4 check result failure!");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
			"Step 5 stop play failure !!!!"
		);
	hPlayer = CSUDI_NULL;	

	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(AUDIO_INDEX_0,hPlayer,CSUDI_TRUE)),
				"Step 5 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	
	CSUDIAUDIOSetVolume(AUDIO_INDEX_0,AUDIO_DEFAULT_VOLUME);
	return CSUDI_TRUE;
}


//@CASEGROUP:1、CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2、CSUDIAUDIOUnmute(int nAudioIndex)
//@DESCRIPTION: 测试能正常静音和取消静音功能
//@PRECONDITION: 平台成功初始化
//@INPUT: nAudioIndex = 0
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、在nAudio上播放"还珠格格"，期望播放成功
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOMute(nAudio)设置静音，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、等待确认:期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 5、调用CSUDIAUDIOUnmute(nAudio)取消静音，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、等待确认:期望所有合法设备均恢复声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 7、重复步骤3到6 N(N=5)次，期望每一步骤都得到期望值
//@EXECUTIONFLOW: 8、停止nAudioIndex播放
CSUDI_BOOL CSTC_AUDIO_MuteAndUnmute_0001(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;	
	AudioTestOutputCfg_S sOutputCfg;
	int nLoop = 0;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_AC3 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play 还珠格格 failure!!!!"
		);

	CSTCPrint("Step 2 start play 还珠格格\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play 还珠格格 failure!!!!"
		);
	
	for(nLoop = 0; nLoop < AUDIO_TEST_TIMES; nLoop++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
				"step 3 set mute failure !!!!"
			);
		CSTCPrint("step 3 The current status: [mute] [stereo] [31]\r\n");
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
				"Step 4 check result failure !!!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
				"step 5 set mute failure !!!!"
			);
		CSTCPrint("step 5 The current status: [unmute] [stereo] [31]\r\n");
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
				"Step 6 check result failure !!!!"
			);
	}
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 8 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}



//@CASEGROUP:1、CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2、CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3、CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@DESCRIPTION: 静音、非静音和音量操作流程切换时的组合测试
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@INPUT: nAudioIndex = 0
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 初始值为:(音量31+非静音) 静音+音量31 -> 非静音+音量0 -> 音量31+非静音 -> 静音+音量31-> 停止播放
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、在nAudioIndex上播放"还珠格格"，期望播放成功
//@EXECUTIONFLOW: 静音+音量31
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOMute(nAudioIndex)设置静音，期望返回CSUDI_SUCCESS，且期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS，且期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 非静音+音量0
//@EXECUTIONFLOW: 5、调用CSUDIAUDIOUnmute(nAudioIndex)设置非静音，期望返回CSUDI_SUCCESS，且期望所有合法设备均有声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 6、调用CSUDIAUDIOSetVolume(nAudioIndex,0)，期望返回CSUDI_SUCCESS，且期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 音量31+静音
//@EXECUTIONFLOW: 7、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS，且期望所有合法设备均有声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 8、调用CSUDIAUDIOMute(nAudioIndex)，期望返回CSUDI_SUCCESS，且期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 非静音+音量31
//@EXECUTIONFLOW: 9、调用CSUDIAUDIOUnmute(nAudio)，期望返回CSUDI_SUCCESS，且期望所有合法设备均有声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 10、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS，且期望所有合法设备均有声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 11、停止nAudioIndex播放
CSUDI_BOOL CSTC_AUDIO_IT_0001(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;	
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play 还珠格格 failure!!!!"
		);

	CSTCPrint("step 2 start play 还珠格格\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play 还珠格格 failure!!!!"
		);
	
	//静音+音量31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 3 set mute failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 4 set volume 31 failure !!!!"
		);
	CSTCPrint("step 3、4 The current status: [%s] volume=%d \n","mute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 4 check result failure !!!!"
		);
	
	//非静音+音量0
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 5 set unmute failure !!!!"
		);
	
	CSTCPrint("step 5 The current status: [%s] volume=%d \n","unmute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 5 check result failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 6 set volume  0 failure !!!!"
		);
	
	CSTCPrint("step 6 The current status: [%s] volume=%d \n","unmute",0);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 6 check result failure !!!!"
		);
	
	//音量31+静音
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 7 set volume  31 failure !!!!"
		);
	CSTCPrint("step 7 The current status: [%s] volume=%d \n","unmute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 7 check result failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 8 set mute failure !!!!"
		);
	CSTCPrint("step 8 The current status: [%s] volume=%d \n","mute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 8 check result failure !!!!"
		);
	
	//非静音+音量31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 9 set unmute failure !!!!"
		);
	CSTCPrint("step 9 The current status: [%s] volume=%d \n","unmute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 9 check result failure !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 10 set volume  31 failure !!!!"
		);
	CSTCPrint("step 10 The current status: [%s] volume=%d \n","unmute",31);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 10 check result failure !!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 11 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}
//@CASEGROUP:1、CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2、CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3、CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: 声道和音量操作流程切换时的组合测试
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@INPUT: nAudioIndex = 支持MPG2类型的音频解码器
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 初始值为(立体声道+音量31):播放+左+31 ->暂停播放 -> 右+31+恢复播放 -> 立体声+31 -> 混合+31 -> 停止播放
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、播放"还珠格格",期望播放成功，所有合法设备能同时听到广播和电视的声音
//@EXECUTIONFLOW: 播放+左+31
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)设置左声道，期望返回CSUDI_SUCCESS，且期望所有合法设备应该能听到电视节目的声音
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS，且期望所有合法设备应该能听到电视节目的声音
//@EXECUTIONFLOW: 暂停播放 
//@EXECUTIONFLOW: 5、暂停播放,期望所有设备均无声音输出
//@EXECUTIONFLOW: 右+31+恢复播放  
//@EXECUTIONFLOW: 6、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)设置右声道，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、恢复播放，且期望所有合法设备均能听到广播节目的声音
//@EXECUTIONFLOW: 立体声+31
//@EXECUTIONFLOW: 9、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)设置立体声道，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS，所有合法设备能同时听到广播和电视的声音
//@EXECUTIONFLOW: 混合+31
//@EXECUTIONFLOW: 11、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)设置立体声道，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 12、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS，所有合法设备能听到电视节目和广播节目的混合声
//@EXECUTIONFLOW: 13、停止nAudioIndex播放
CSUDI_BOOL CSTC_AUDIO_IT_0002(void)
{
	int nAudioIndex = -1;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	
	//播放+左+31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play 还珠格格 failure!!!!"
		);
	
	CSTCPrint("step 2 start play 还珠格格\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play 还珠格格 failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)),
			"step 3 set Channel LEFT failure!!!!"
		);
	CSTCPrint("step 3 The current status: [left] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_LEFT],sOutputCfg.uAudOutput)),
			"step 3 check Channel failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 4 set volume 31 failure!!!!"
		);
	CSTCPrint("step 3、4 The current status: [left] [31] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 3、4 check result failure !!!!"
		);

	nRet = CSUDIPLAYERPause(hPlayer);
	
	//暂停播放 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == nRet),
			"step 5 pause play failure!!!!"
		);

	CSTCPrint("step 5 The current status: [pause] [left] [31] \n");

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
		"Step 5 check result failure !!!!"
	);
	
	//右+31+恢复播放 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 6 set Channel RIGHT failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 7 set volume 31 failure!!!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)),
		"step 8 resume play failure!!!!"
	);

	CSTCPrint("step 6、7、8 The current status: [resume] [right] [31] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_RIGHT],sOutputCfg.uAudOutput)),
			"step 6、7、8 check Channel failure!!!!"
		);
	
	//立体声+31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)),
			"step 9 set Channel STEREO failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 10 set volume 31 failure!!!!"
		);
	CSTCPrint("step 9、10 The current status: [stereo] [31] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_STEREO],sOutputCfg.uAudOutput)),
			"step 9、10 check Channel failure!!!!"
		);

	//混合+31
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)),
			"step 11 set Channel STEREO failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 12 set volume 31 failure!!!!"
		);
	CSTCPrint("step 11、12 The current status: [mono] [31] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_MIXED_MONO],sOutputCfg.uAudOutput)),
			"step 11、12 check Channel failure!!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 13 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}
//@CASEGROUP:1、CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2、CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3、CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: 声道和音量操作流程切换时的组合测试(续)
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@INPUT: nAudioIndex = 支持MPG2类型的音频解码器
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 初始值为(立体声道+音量31):暂停播放+左+0 -> 恢复播放 -> 右+0 -> 右+31 -> 暂停播放->立体+31+恢复播放
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、播放"多语言左右声道",期望播放成功，所有合法设备能同时听到广播和电视的声音
//@EXECUTIONFLOW: 暂停播放+左+0
//@EXECUTIONFLOW: 3、暂停播放,期望所有设备均无声音输出
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)设置左声道，期望返回CSUDI_SUCCESS，期望无声音输出
//@EXECUTIONFLOW: 5、调用CSUDIAUDIOSetVolume(nAudioIndex,0)，期望返回CSUDI_SUCCESS，且期望所有合法设备都无声音输出
//@EXECUTIONFLOW: 恢复播放 
//@EXECUTIONFLOW: 6、恢复播放,期望所有合法设备无声音输出
//@EXECUTIONFLOW: 右+0 
//@EXECUTIONFLOW: 7、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)设置右声道，期望返回CSUDI_SUCCESS,且期望所有合法设备都无声音输出
//@EXECUTIONFLOW: 8、调用CSUDIAUDIOSetVolume(nAudioIndex,0)，期望返回CSUDI_SUCCESS,且期望所有合法设备都无声音输出
//@EXECUTIONFLOW: 右+31 
//@EXECUTIONFLOW: 9、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)设置右声道，期望返回CSUDI_SUCCESS,且期望所有合法设备都无声音输出
//@EXECUTIONFLOW: 10、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS，且期望所有合法设备应该能听到电视节目的声音
//@EXECUTIONFLOW: 暂停播放
//@EXECUTIONFLOW: 11、暂停播放,如果平台支持直播情况下暂停,则期望所有设备均无声音输出
//@EXECUTIONFLOW: 立体+31+恢复播放
//@EXECUTIONFLOW: 12、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)设置立体声道，期望返回CSUDI_SUCCESS，期望返回CSUDI_SUCCESS,且期望所有合法设备都无声音输出
//@EXECUTIONFLOW: 13、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS，期望返回CSUDI_SUCCESS,且期望所有合法设备都无声音输出
//@EXECUTIONFLOW: 14、恢复播放，所有合法设备能听到电视节目和广播节目的混合声
//@EXECUTIONFLOW: 15、停止nAudioIndex播放
CSUDI_BOOL CSTC_AUDIO_IT_0003(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_BOOL bRet;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 2 start play 多语言左右声道 failure!!!!"
		);

	CSTCPrint("step 2 start play 多语言左右声道\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play 多语言左右声道 failure!!!!"
		);
	
	//暂停播放+左+0
	nRet = CSUDIPLAYERPause(hPlayer);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == nRet),
			"step 3 pause play failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)),
			"step 4 set Channel RIGHT failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 5 set volume 0 failure!!!!"
		);
	
	CSTCPrint("Step 3、4、5 The current status: [pause] [left] [volume=%d] \n",0);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 3、4、5 check result failure !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)),
		"step 6 resume play failure!!!!"
	);
	
	CSTCPrint("step 6 The current status: [resume] [left] [volume=%d] \n",0);
	CSTCPrint("期望所有合法设备均无声音输出,操作瞬间无噪音...........!!\r\n");
	CSTCPrint("0:Yes,与期望现象相同 .\r\n");
	CSTCPrint("1:No,与期望现象不同.\r\n");
	bRet = CSTKWaitYes();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"Step 6 check result failure !!!!"
		);
	
	//右+0 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 7 set Channel RIGHT failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 8 set volume 31 failure!!!!"
		);
	CSTCPrint("step 7、8 The current status: [play] [right] [volume=%d] \n",0);
	CSTCPrint("期望所有合法设备均无声音输出,操作瞬间无噪音...........!!\r\n");
	CSTCPrint("0:Yes,与期望现象相同 .\r\n");
	CSTCPrint("1:No,与期望现象不同.\r\n");
	bRet = CSTKWaitYes();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"Step 7、8 check result failure !!!!"
		);
	
	//右+31 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 9 set Channel RIGHT failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 10 set volume 31 failure!!!!"
		);	
	
	CSTCPrint("step 9、10 The current status: [play] [right] [volume=%d] \n",31);
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_RIGHT],sOutputCfg.uAudOutput)),
		"step 9、10 check Channel failure!!!!"
	);
	
	//暂停播放
	nRet = CSUDIPLAYERPause(hPlayer);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == nRet),
			"step 11 pause play failure!!!!"
		);
	
	CSTCPrint("step 11 The current status: [pause] [right] [volume=%d] \n",31);

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
		"Step 11 check result failure !!!!"
	);
	
	//立体+31+恢复播放 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)),
			"step 12 set Channel STEREO failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 13 set volume 31 failure!!!!"
		);	

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)),
		"step 14 resume play failure!!!!"
	);
	
	CSTCPrint("step 12、13、14 The current status: [resume] [stereo] [volume=%d] \n",31);	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_STEREO],sOutputCfg.uAudOutput)),
		"step 12、13、14 check Channel failure!!!!"
	);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 15 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2、CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3、CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@DESCRIPTION: 声道和静音操作流程切换时的组合测试
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@INPUT: nAudioIndex = 支持MPG2类型的音频解码器
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 初始值为(立体声道+非静音+31):静音+左 -> 非静音+左 -> 非静音+右 ->静音+右->静音+立体->非静音+立体->非静音+混合->静音+混合
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、播放"多语言左右声道",期望播放成功，所有合法设备能同时听到广播和电视的声音
//@EXECUTIONFLOW: 静音+左 
//@EXECUTIONFLOW: 3、调用CSUDIAUDIOMute(nAudioIndex)设置静音，期望返回CSUDI_SUCCESS，且期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)设置左声道，期望返回CSUDI_SUCCESS，且期望所有合法设备都无声音输出
//@EXECUTIONFLOW: 非静音+左
//@EXECUTIONFLOW: 5、调用CSUDIAUDIOUnmute(nAudioIndex)，期望返回CSUDI_SUCCESS，且期望所有合法设备能听到电视节目的声音
//@EXECUTIONFLOW: 6、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)设置左声道，期望返回CSUDI_SUCCESS，且期望所有合法设备应该能听到电视节目的声音
//@EXECUTIONFLOW: 非静音+右
//@EXECUTIONFLOW: 7、调用CSUDIAUDIOUnmute(nAudioIndex)，期望返回CSUDI_SUCCESS，且期望所有合法设备能听到电视节目的声音
//@EXECUTIONFLOW: 8、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)设置右声道，期望返回CSUDI_SUCCESS,且期望所有合法设备均能听到广播节目的声音
//@EXECUTIONFLOW: 静音+右
//@EXECUTIONFLOW: 9、调用CSUDIAUDIOMute(nAudioIndex)设置静音，期望返回CSUDI_SUCCESS，且期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 10、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)设置右声道，期望返回CSUDI_SUCCESS，期望所有设备均无声音输出
//@EXECUTIONFLOW: 静音+立体
//@EXECUTIONFLOW: 11、调用CSUDIAUDIOMute(nAudioIndex)设置静音，期望返回CSUDI_SUCCESS，且期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 12、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)设置立体声道，期望返回CSUDI_SUCCESS，期望所有设备均无声音输出
//@EXECUTIONFLOW: 非静音+立体
//@EXECUTIONFLOW: 13、调用CSUDIAUDIOUnmute(nAudioIndex)，期望返回CSUDI_SUCCESS，且期望所有合法设备能同时听到广播和电视的声音
//@EXECUTIONFLOW: 14、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)设置立体声道，期望返回CSUDI_SUCCESS，所有合法设备能同时听到广播和电视的声音
//@EXECUTIONFLOW: 非静音+混合
//@EXECUTIONFLOW: 15、调用CSUDIAUDIOUnmute(nAudioIndex)，期望返回CSUDI_SUCCESS，且期望所有合法设备能听到电视节目和广播节目的混合声
//@EXECUTIONFLOW: 16、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)设置混合声道，期望返回CSUDI_SUCCESS，所有合法设备能听到电视节目和广播节目的混合声
//@EXECUTIONFLOW: 静音+混合
//@EXECUTIONFLOW: 17、调用CSUDIAUDIOUnmute(nAudioIndex)，期望返回CSUDI_SUCCESS，且期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 18、调用CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)设置混合声道，期望返回CSUDI_SUCCESS，且期望所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 19、停止nAudioIndex播放
CSUDI_BOOL CSTC_AUDIO_IT_0004(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AudioTestOutputCfg_S sOutputCfg;

	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_MPEG2 !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 2 start play 多语言左右声道 failure!!!!"
		);

	CSTCPrint("step 2 start play 多语言左右声道\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 2 check play 多语言左右声道 failure!!!!"
		);
	
	//静音+左 
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 3 set mute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)),
			"step 4 set Channel LEFT failure!!!!"
		);
	CSTCPrint("step 3、4 The current status: [mute] [left] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 3、4 check result failure !!!!"
		);
	
	//非静音+左
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 5 set unmute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_LEFT)),
			"step 6 set Channel LEFT failure!!!!"
		);
	CSTCPrint("step 5、6 The current status: [unmute] [left] \n");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_LEFT],sOutputCfg.uAudOutput)),
		"step 5、6 check Channel failure!!!!"
	);
	
	//非静音+右
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 7 set unmute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 8 set Channel RIGHT failure!!!!"
		);
	CSTCPrint("step 7、8 The current status: [unmute] [right] \n");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_RIGHT],sOutputCfg.uAudOutput)),
		"step 7、8 check Channel failure!!!!"
	);
	
	//静音+右	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 9 set mute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_RIGHT)),
			"step 10 set Channel RIGHT failure!!!!"
		);
	CSTCPrint("step 9、10 The current status: [mute] [right] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 9、10 check result failure !!!!"
		);
	
	//静音+立体
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 11 set mute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)),
			"step 12 set Channel STEREO failure!!!!"
		);
	CSTCPrint("step 11、12 The current status: [mute] [stereo] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 11、12 check result failure !!!!"
		);
	
	//非静音+立体
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 13 set Unmute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_STEREO)),
			"step 14 set Channel STEREO failure!!!!"
		);
	CSTCPrint("step 13、14 The current status: [unmute] [stereo] \n");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_STEREO],sOutputCfg.uAudOutput)),
		"step 13、14  check Channel failure!!!!"
	);

	//非静音+混合
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 15 set Unmute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)),
			"step 16 set Channel MIXED_MONO failure!!!!"
		);
	CSTCPrint("step 15、16 The current status: [unmute] [MONO] \n");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[EM_UDIAUDIO_CHANNEL_MIXED_MONO],sOutputCfg.uAudOutput)),
		"step 15、16  check Channel failure!!!!"
	);

	//静音+混合
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 17 set mute failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,EM_UDIAUDIO_CHANNEL_MIXED_MONO)),
			"step 18 set Channel STEREO failure!!!!"
		);
	CSTCPrint("step 17、18 The current status: [mute] [MONO] \n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 17、18 check result failure !!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 11 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2、CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3、CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@CASEGROUP:4、CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice, CSUDIAUDIOMode_E eMode)
//@DESCRIPTION: 组合测试播放DD或DD+节目时，音频的解码模式同音量设置的对应关系
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@PRECONDITION: 2、系统存在音频SPDIF输出
//@INPUT: nAudioIndex = 0
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 初始值为(EM_UDIAUDIO_DECODE+31):decode+AC3+音量31 -> bypass+AC3+音量31 -> bypass+AC3+音量0 ->decode+MEPG2+音量31
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOGetCount和CSUDIAUDIOGetCapability获取支持AC3类型decode模式的音频解码器索引值nAudioIndex,如果不支持AC3解码，则执行步骤6
//@EXECUTIONFLOW: 3、调用CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAUDIO_DECODE)，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、在nAudioIndex上播放"DolbyAC3标准测试节目"，期望播放成功,且所有输出设备均有声音输出
//@EXECUTIONFLOW: 6、调用CSUDIAUDIOSetVolume 设置nAudioIndex的音量为0，期望返回CSUDI_SUCCESS，所有设备无声音输出
//@EXECUTIONFLOW: 7、调用CSUDIAUDIOSetVolume 设置nAudioIndex的音量为31，期望返回CSUDI_SUCCESS，所有输出设备均有声音输出
//@EXECUTIONFLOW: 8、调用CSUDIAOUTSetOPMode设置EM_UDIAOUT_SPDIF为BYPASS，期望返回CSUDI_SUCCESS，所有输出设备均有声音输出，音箱设备输出dolby5.1，操作瞬间无噪音
//@EXECUTIONFLOW: 9、调用CSUDIAUDIOSetVolume设置nAudioIndex的音量为0，期望返回CSUDI_SUCCESS，期望音箱设备有声音输出,其它设备无声音输出
//@EXECUTIONFLOW: 10、调用CSUDIAUDIOMute设置nAudioIndex静音，期望返回CSUDI_SUCCESS，期望所有设备无声音输出
//@EXECUTIONFLOW: 11、停止播放，期望所有设备均无声音输出
//@EXECUTIONFLOW: 12、调用CSUDIAOUTSetOPMode设置SPDIF、HDMI 为DECODE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 13、调用CSUDIAUDIOSetVolume 设置nAudioIndex的音量为31，调用CSUDIAUDIOUnmute设置nAudioIndex退出静音， 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 14、播放"还珠格格",期望播放成功，期望所有合法设备有声音输出
//@EXECUTIONFLOW: 15、停止播放，期望所有设备均无声音输出
//@EXECUTIONFLOW: 16、恢复现场
CSUDI_BOOL CSTC_AUDIO_IT_0005(void)
{
	int nAudioIndex = -1;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_BOOL bRet = CSUDI_FALSE;

	if(IsAOUTSupport(EM_UDIAOUT_SPDIF))
	{
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	//EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
	//EXECUTIONFLOW: 2、调用CSUDIAUDIOGetCount和CSUDIAUDIOGetCapability获取支持AC3类型decode模式的音频解码器索引值nAudioIndex,如果不支持AC3解码，则执行步骤6
	//EXECUTIONFLOW: 3、调用CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAUDIO_DECODE)，期望返回CSUDI_SUCCESS
	//EXECUTIONFLOW: 4、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS
	//EXECUTIONFLOW: 5、在nAudioIndex上播放"DolbyAC3标准测试节目"，期望播放成功,且所有输出设备均有声音输出
	bRet = AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex);
	
	if(bRet == CSUDI_FALSE) //如果不支持AC3解码，则执行步骤6
	{
		CSTCPrint("There's no audio support EM_UDI_AUD_STREAM_AC3 !!!!\r\n");
		bRet = AUDIO_iGetAudioIndexWithBypassByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex);
		if(bRet == CSUDI_FALSE) ///如果不支持AC3bypass，则执行步骤12
			goto LABEL;
	}
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
		"step 5 start play DolbyAC3标准测试节目 failure!!!!"
	);

	CSTCPrint("step 5 start play DolbyAC3标准测试节目\n");

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
		"step 5 check play DolbyAC3标准测试节目 failure!!!!"
	);

	//EXECUTIONFLOW: 6、调用CSUDIAUDIOSetVolume 设置nAudioIndex的音量为0，期望返回CSUDI_SUCCESS，所有设备无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 6 set volume 0 failure!!!!"
		);

	CSTCPrint("step 6 set volume 0 \n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 6 check result failure !!!!"
		);

	//EXECUTIONFLOW: 7、调用CSUDIAUDIOSetVolume 设置nAudioIndex的音量为31，期望返回CSUDI_SUCCESS，所有输出设备均有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 7 set volume 31 failure!!!!"
		);

	CSTCPrint("step 7 set volume 31 \n");
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
		"Step 7 check result failure !!!!"
	);
	
	//EXECUTIONFLOW: 8、调用CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAUDIO_BYPASS)，
	//期望返回CSUDI_SUCCESS，所有输出设备均有声音输出，音箱设备输出dolby5.1，操作瞬间无噪音
	//bypass+AC3+音量31
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_BYPASS)),
		"step 8 set audio decode bypass mode failure!!!!"
	);
	
	CSTCPrint("step 8 set spdif audio decode bypass mode \n");
	CSTCPrint("期望音箱设备输出格式为dolby格式!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
		"Step 8 check result failure !!!!"
	);

	//EXECUTIONFLOW: 9、调用CSUDIAUDIOSetVolume设置nAudioIndex的音量为0，期望返回CSUDI_SUCCESS，期望音箱设备有声音输出,其它设备无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,0)),
			"step 9 set volume 0 failure!!!!"
		);

	CSTCPrint("step 9 set volume 0\n");
	CSTCPrint("期望SPDIF音箱设备有输出，输出格式为dolby 5.1格式其它设备无声音!!\r\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 9 check result failure !!!!"
		);
		
	//EXECUTIONFLOW: 10、调用CSUDIAUDIOMute，期望返回CSUDI_SUCCESS，期望音箱设备无声音输出
	//bypass+AC3+静音
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 10 set mute failure!!!!"
		);

	CSTCPrint("step 10 set mute \n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 10 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 11、停止播放，期望所有设备均无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
			"Step 11 stop play failure !!!!"
		);
	hPlayer = CSUDI_NULL;
	
	CSTCPrint("step 11 stop play \n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 11 check result failure !!!!"
		);
	
	//decode+MEPG1+音量31	
LABEL:	
	//EXECUTIONFLOW: 12、调用CSUDIAOUTSetOPMode设置SPDIF、HDMI 为DECODE，期望返回CSUDI_SUCCESS
	CSTK_ASSERT_TRUE(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_DECODE)),
			"step 12 set SPDIF output decode mode failure!!!!"
		);	
	if( IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
		CSTK_ASSERT_TRUE(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
			"step 12 set HDMI output decode mode failure!!!!");	
		CSTCPrint("step 12 set SPDIF&HDMI output decode mode \n");
	}
	
	//EXECUTIONFLOW: 13、调用CSUDIAUDIOSetVolume(nAudioIndex,31)，期望返回CSUDI_SUCCESS
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex,31)),
			"step 13 set volume 31 failure!!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 13 set unmute failure!!!!"
		);
	
	CSTCPrint("step 13 set volume 31 & set unmute \n");

	//EXECUTIONFLOW: 14、播放"还珠格格",期望播放成功，期望所有合法设备有声音输出
	bRet = AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_MPEG2,&nAudioIndex);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == bRet),
			"There's no audio decode support decode mpeg  !!!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 14 start play 还珠格格failure!!!!"
		);

	CSTCPrint("step 14 start play 还珠格格 \n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_MULTILANGUAGE1],&sOutputCfg)),
			"step 14 check play 还珠格格failure!!!!"
		);
	
	//EXECUTIONFLOW: 15、停止播放，期望所有设备均无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
			"Step 15 stop play failure !!!!"
		);
	
	hPlayer = CSUDI_NULL;

	CSTCPrint("step 15 stop play \n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 15 check result failure !!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 16 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("Not support SPDIF!\n");
		return CSUDI_FALSE;
	}
}



//@CASEGROUP:1、CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2、CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3、CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@CASEGROUP:4、CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice)
//@CASEGROUP:5、CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice)
//@DESCRIPTION: 切换台不能影响音频状态，包括音量大小和静音、左右声道
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@INPUT: nAudioIndex = 0
//@INPUT:节目1--"多语言左右声道"
//@INPUT:节目2-- "还珠格格" 
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOSetChannel设置nAudioIndex为立体声，音量最大，不静音；
//@EXECUTIONFLOW: 3、播放"多语言左右声道",期望播放成功，所有合法设备能同时听到"多语言左右声道"广播和电视的声音
//@EXECUTIONFLOW: 4、停止播放"多语言左右声道",播放"还珠格格",期望播放成功，所有合法设备能同时听到"还珠格格"广播和电视的声音
//@EXECUTIONFLOW: 5、调用CSUDIAUDIOSetChannel设置nAudioIndex左声道，期望返回CSUDI_SUCCESS,
//@EXECUTIONFLOW: 6、停止播放"还珠格格",播放"多语言左右声道",期望播放成功，所有合法设备能听到"多语言左右声道"电视节目的声音
//@EXECUTIONFLOW: 7、停止播放"多语言左右声道",播放"还珠格格",期望播放成功，所有合法设备能听到"还珠格格"电视节目的声音
//@EXECUTIONFLOW: 8、调用CSUDIAUDIOSetChannel设置nAudioIndex右声道，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、停止播放"还珠格格",播放"多语言左右声道",期望播放成功，所有合法设备能听到"多语言左右声道"广播节目的声音
//@EXECUTIONFLOW: 10、停止播放"多语言左右声道",播放"还珠格格",期望播放成功，所有合法设备能同时听到"还珠格格"广播节目的声音
//@EXECUTIONFLOW: 11、调用CSUDIAUDIOSetChannel设置nAudioIndex混合声，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 12、停止播放"还珠格格",播放"多语言左右声道",期望播放成功，所有合法设备能同时听到"多语言左右声道"广播和电视的声音
//@EXECUTIONFLOW: 13、停止播放"多语言左右声道",播放"还珠格格",期望播放成功，所有合法设备能同时听到"还珠格格"广播和电视的声音
//@EXECUTIONFLOW: 14、调用CSUDIAUDIOMute 设置nAudioIndex静音，期望返回CSUDI_SUCCESS，所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 15、停止播放"多语言左右声道",播放"还珠格格",期望播放成功，所有合法设备均无声音输出,操作瞬间无噪音
//@EXECUTIONFLOW: 16、调用CSUDIAUDIOUnmute设置nAudioIndex非静音，期望返回CSUDI_SUCCESS，所有合法设备能同时听到"多语言左右声道"广播和电视的声音
//@EXECUTIONFLOW: 17、停止nAudioIndex播放
//@EXECUTIONFLOW: 18、恢复现场
CSUDI_BOOL CSTC_AUDIO_IT_0006(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_UINT32 uAudioChannelTyp = EM_UDIAUDIO_CHANNEL_STEREO;
	int nLoop;
	int nAudioIndex = -1;
	int nCurServiceInx = -1;
	int nlastServiceInx = -1;
	int nStep = 0;
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	nAudioIndex = AUDIO_INDEX_0;
	
	//EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点	
	//EXECUTIONFLOW: 2、设置输出为立体声，音量最大，不静音；
	//"还珠格格"
	nCurServiceInx = EM_AUDIO_MULTILANGUAGE1;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play failure!!!!"
		);
	
	CSTCPrint("Step 1 start play %s\n", s_sAudioServices[nCurServiceInx].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg)),
			"check 1 start play failure!!!!"
		);
	
	++nStep ;
	
	//nCurServiceInx = EM_AUDIO_MULTILANGUAGE1;
	for(uAudioChannelTyp=0;uAudioChannelTyp < sizeof(s_sAudioChannels)/sizeof(AudioTestChannel_S); uAudioChannelTyp++)
	{
		++nStep;	
		CSTCPrint("Step %d set %s channel\n",nStep,s_sAudioChannels[uAudioChannelTyp].m_cChannelInfo);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex,s_sAudioChannels[uAudioChannelTyp].m_eChannel)),
				"set Channel failure!!!!"
			);
		
		for(nLoop=0; nLoop<2; nLoop++)
		{
			if(hPlayer != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_FALSE)),
						"stop play failure !!!!"
					);
				hPlayer = CSUDI_NULL;
			}		
			
			nlastServiceInx = nCurServiceInx;		
			nCurServiceInx = AUDIO_iGetNextServiceInx(nlastServiceInx);
			++nStep;				
			CSTCPrint("Step %d play %s \n",nStep,s_sAudioServices[nCurServiceInx].m_pcServiceDescript);

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg,&hPlayer,CSUDI_FALSE)),
					"start play failure!!!!"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg)),
					"check play failure!!!!"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[uAudioChannelTyp],sOutputCfg.uAudOutput)),
					"check Channel failure!!!!"
				);	
		}		
	}

	uAudioChannelTyp = (sizeof(s_sAudioChannels)/sizeof(AudioTestChannel_S)-1);

	CSTCPrint("Step 14 mute audio decode\n");	

	//nStep = 13	
	//EXECUTIONFLOW: 14、调用CSUDIAUDIOMute 设置nAudioIndex静音，期望返回CSUDI_SUCCESS，所有合法设备均无声音输出,操作瞬间无噪音
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"Step 14 mute audio decode failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
		"Step 14 check result failure !!!!"
	);
	
	//EXECUTIONFLOW: 15、停止播放"多语言左右声道",播放"还珠格格",期望播放成功，所有合法设备均无声音输出,操作瞬间无噪音
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_FALSE)),
				"Step 15 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}		

	nCurServiceInx = EM_AUDIO_MULTILANGUAGE1;
	CSTCPrint("Step 15 play %s \n",s_sAudioServices[nCurServiceInx].m_pcServiceDescript);	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[nCurServiceInx],&sOutputCfg,&hPlayer,CSUDI_FALSE)),
			"Step 15 start play failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
		"Step 15 check result failure !!!!"
	);

	//EXECUTIONFLOW: 16、调用CSUDIAUDIOUnmute设置nAudioIndex非静音，期望返回CSUDI_SUCCESS，所有合法设备能同时听到"多语言左右声道"广播和电视的声音
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 16 unmoute audio decode  failure!!!!"
		);

	//恢复静音后检查当前播放是否能正常输出声音
	CSTCPrint("Step 16 play unmute %s \n",s_sAudioServices[nCurServiceInx].m_pcServiceDescript);

	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
		"Step 16 check result failure !!!!"
	);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckChannel(&s_sAudioChannels[uAudioChannelTyp],sOutputCfg.uAudOutput)),
			"Step 16 check Channel failure!!!!"
		);	

	//EXECUTIONFLOW: 17、停止nAudioIndex播放	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 17 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	
	//EXECUTIONFLOW: 18、恢复现场
	AUDIO_iSetDefault(nAudioIndex);
	
	return CSUDI_TRUE;
}


//@CASEGROUP:1、CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2、CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3、CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@CASEGROUP:4、CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@CASEGROUP:5、CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice, CSUDIAUDIOMode_E eMode)
//@DESCRIPTION:DD和DD+码流bypass输出时对HDMI接口（HDMI设备支持DD或DD+解码）和SPDIF接口
//@DESCRIPTION:设置左右声道也没有效果；设置音量无效果；
//@DESCRIPTION:设置静音和解除静音有效（CSUDIAUDIOMute、CSUDIAUDIOUnmute、CSUDIAOUTMute、CSUDIAOUTUnmute）
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够正常播放音频流
//@INPUT: nAudioIndex = 0
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、在nAudioIndex=0上播放"DolbyAC3标准测试"，设置EM_UDIAOUT_SPDIF和HDMI输出模式为BYPASS,期望播放成功
//@EXECUTIONFLOW: 3、从nVolume=0开始,在有效音量范围内,nVolume每200ms递增加1,调用CSUDIAUDIOSetVolume(nAudioIndex,nVolume)，调用CSUDIAUDIOSetChannel依次设置声道类型，期望每次调用成功
//@EXECUTIONFLOW: 4、校验每次递增音量值，期望SPDIF和HDMI输出音量没有任何变化，RCA输出音量有逐渐变大(0为最小音量,31为最大音量)
//@EXECUTIONFLOW: 5、调用CSUDIAUDIOMute(nAudioIndex),期望所有合法声音设备无声音输出
//@EXECUTIONFLOW: 6、调用CSUDIAUDIOUnmute(nAudioIndex),期望所有合法声音设备有声音输出
//@EXECUTIONFLOW: 7、停止本次播放，释放player
//@EXECUTIONFLOW: 8、获取支持DolbyAC3的解码器nAudioIndex，在nAudioIndex上播放"DolbyAC3标准测试"，期望播放成功
//@EXECUTIONFLOW: 9、设置SPDIF和HDMI输出模式为DECODE,期望所有合法声音设备有声音输出
//@EXECUTIONFLOW: 10、调用CSUDIAUDIOSetVolume将音量设置为0,期望所有合法声音设备无声音输出
//@EXECUTIONFLOW: 11、调用CSUDIAUDIOSetVolume将音量设置为31(最大),期望所有合法声音设备有声音输出
//@EXECUTIONFLOW: 12、调用CSUDIAUDIOMute(nAudioIndex),期望所有合法声音设备无声音输出
//@EXECUTIONFLOW: 13、调用CSUDIAUDIOUnmute(nAudioIndex),期望所有合法声音设备有声音输出
//@EXECUTIONFLOW: 14、调用CSUDIAUDIOMute(nAudioIndex),调用CSUDIAUDIOSetVolume将音量设置为0,期望所有合法声音设备无声音输出
//@EXECUTIONFLOW: 15、调用CSUDIAUDIOSetVolume将音量设置为31(最大),期望所有合法声音设备无声音输出
//@EXECUTIONFLOW: 16、调用CSUDIAUDIOUnmute(nAudioIndex),期望所有合法声音设备有声音输出
//@EXECUTIONFLOW: 17、恢复现场	
CSUDI_BOOL CSTC_AUDIO_AOUT_IT_0001(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_UINT32 uAudioChannelTyp = EM_UDIAUDIO_CHANNEL_STEREO;
	int jj;
	int nLoop;
	int nAudioIndex = -1;
	
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	nAudioIndex = AUDIO_INDEX_0;
	//EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
	//EXECUTIONFLOW: 2、在nAudioIndex=0上播放"DolbyAC3标准测试"，设置EM_UDIAOUT_SPDIF输出模式为BYPASS,期望播放成功
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 1 start play DolbyAC3标准测试 failure!!!!"
		);

	CSTCPrint("step 1 start play DolbyAC3标准测试\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex ,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
			"step 1 check play DolbyAC3标准测试 failure!!!!"
		);

	CSTCPrint("step 2 set SPDIF&HDMI output bypass mode \n");

	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_BYPASS)),
			"step 2.1 set SPDIF bypass mode failure!!!!"
		);
    }
	if( IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
	
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_BYPASS)),
			"step 2.2 set HDMI bypass mode failure!!!!");
	}
	
	//EXECUTIONFLOW: 3、从nVolume=0开始,在有效音量范围内,nVolume每200ms递增加1,调用CSUDIAUDIOSetVolume(nAudioIndex,nVolume)，调用CSUDIAUDIOSetChannel依次设置声道类型，期望每次调用成功
	CSTCPrint("step 3、4 请注意关注SPDIF、HDMI音箱输出和RCA设备输出音量!!\r\n");
	//EXECUTIONFLOW: 4、校验每次递增音量值，期望SPDIF、HDMI输出音量没有任何变化，RCA输出音量有逐渐变大(0为最小音量,31为最大音量)
	CSTCPrint("期望操作过程中SPDIF和HDMI音箱输出音量没有任何变化，并且输出格式一直为Dolby格式!!\r\n");
	CSTCPrint("期望操作过程中RCA设备音量逐渐增大!!\r\n");

	CSTKWaitAnyKey();	
	
	for(nLoop = 0; nLoop < 2; nLoop++)
	{

		for(jj = 0;jj<CSUDI_MAX_VOLUME;jj += 1)
		{	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,jj)),
					"Step 3 set Volume Failure !!!"
				);
			CSTCPrint("The current volume is : %d \r\n",jj);
			CSUDIOSThreadSleep(200);
		}	
		
		/*测试音量为CSUDI_MAX_VOLUME时的情况*/
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,CSUDI_MAX_VOLUME)),
				"Step 3 set Volume Failure !!!"
			);
		CSTCPrint("The current volume is : %d \r\n",CSUDI_MAX_VOLUME);
		CSUDIOSThreadSleep(1000);

		/*测试CSUDIAUDIOSetChannel对SPDIF输出无影响*/
		for(uAudioChannelTyp = EM_UDIAUDIO_CHANNEL_STEREO; uAudioChannelTyp <= EM_UDIAUDIO_CHANNEL_MIXED_MONO; uAudioChannelTyp++)
		{
			CSTCPrint("The current channle type is : %d \r\n",uAudioChannelTyp);
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioIndex ,uAudioChannelTyp)),
				"Step 3 set channel Failure !!!"
			);
			{
				char *pcAudioChannelInfo = NULL;
				AUDIO_iGetChannelInfo(&s_sAudioChannels[uAudioChannelTyp],&pcAudioChannelInfo);
				if(NULL != pcAudioChannelInfo)
				{
					CSTCPrint("The current channle type is : %s \r\n",pcAudioChannelInfo);
					CSUDIOSThreadSleep(1000);
				}
			}
		}
	}	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 4 check volume failure !!!!"
		);	
	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
		CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_DECODE);
    }
	
	CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE);
	
	CSTCPrint("step 5 mute audio decode \n");
		
	//EXECUTIONFLOW: 5、调用CSUDIAUDIOMute(nAudioIndex),期望所有合法声音设备无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 5 mute audio decode failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 5 check result failure !!!!"
		);

	CSTCPrint("step 6 unmoute audio decode \n");
	
	//EXECUTIONFLOW: 6、调用CSUDIAUDIOUnmute(nAudioIndex),期望所有合法声音设备有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 6 unmoute audio decode  failure!!!!"
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 6 check result failure !!!!"
		);

	CSTCPrint("step 7 stop play \n");
	
	//EXECUTIONFLOW: 7、停止本次播放，释放player
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 7 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}

	nAudioIndex = -1;
	//EXECUTIONFLOW: 8、获取支持DolbyAC3的解码器nAudioIndex，在nAudioIndex上播放"DolbyAC3标准测试"，期望播放成功
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex)),
			"There's no audio support EM_UDI_AUD_STREAM_AC3 !!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 8 start play DolbyAC3标准测试 failure!!!!"
		);

	CSTCPrint("step 8 start play DolbyAC3标准测试\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
			"step 8 check play DolbyAC3标准测试 failure!!!!"
		);

	CSTCPrint("step 9 set SPDIF&HDMI output decode mode \n");

	//EXECUTIONFLOW: 9、设置SPDIF和HDMI输出模式为DECODE,期望所有合法声音设备有声音输出
	if( IsAOUTSupport( EM_UDIAOUT_SPDIF ))
    {
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_SPDIF,EM_UDIAOUT_DECODE)),
				"step 9 set audio decode decode mode failure!!!!");
	}
	if( IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
				"step 9 set audio decode decode mode failure!!!!");
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 9 check result failure !!!!"
		);

	CSTCPrint("step 10 The current volume is : %d \r\n",0);
	
	//EXECUTIONFLOW: 10、调用CSUDIAUDIOSetVolume将音量设置为0,期望所有合法声音设备无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,0)),
			"Step 10 set Volume Failure !!!"
		);
	CSTCPrint("The current volume is : %d \r\n",0);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 10 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 11、调用CSUDIAUDIOSetVolume将音量设置为31(最大),期望所有合法声音设备有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,CSUDI_MAX_VOLUME)),
			"Step 11 set Volume Failure !!!"
		);
	CSTCPrint("step 11 The current volume is : %d \r\n",CSUDI_MAX_VOLUME);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 11 check result failure !!!!"
		);

	CSTCPrint("step 12 mute audio decode \n");
	//EXECUTIONFLOW: 12、调用CSUDIAUDIOMute(nAudioIndex),期望所有合法声音设备无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 12 mute audio decode  failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 12 check result failure !!!!"
		);

	CSTCPrint("step 13 unmute audio decode \n");
	//EXECUTIONFLOW: 13、调用CSUDIAUDIOUnmute(nAudioIndex),期望所有合法声音设备有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 13 unmute audio decode  failure!!!!"
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 13 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 14、调用CSUDIAUDIOMute(nAudioIndex),调用CSUDIAUDIOSetVolume将音量设置为0,期望所有合法声音设备无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,0)),
			"Step 14.1 set Volume Failure !!!"
		);
	
	CSTCPrint("step 14 mute audio decode \r\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 14.2 mute audio decode  failure!!!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 14 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 15、调用CSUDIAUDIOSetVolume将音量设置为31(最大),期望所有合法声音设备无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOSetVolume(nAudioIndex ,CSUDI_MAX_VOLUME)),
			"Step 15 set Volume Failure !!!"
		);
	CSTCPrint("step 15 The current volume is : %d \r\n",CSUDI_MAX_VOLUME);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 15 check result failure !!!!"
		);

	//EXECUTIONFLOW: 16、调用CSUDIAUDIOUnmute(nAudioIndex),期望所有合法声音设备有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 16 unmute audio decode  failure!!!!"
		);

	CSTCPrint("step 16 unmute audio decode \r\n");
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 16 check result failure !!!!"
		);
	
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 17 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIAUDIOMute(int nAudioIndex)
//@CASEGROUP:2、CSUDIAUDIOUnmute(int nAudioIndex)
//@CASEGROUP:3、CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
//@CASEGROUP:4、CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
//@CASEGROUP:5、CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice, CSUDIAUDIOMode_E eMode)
//@CASEGROUP:6、CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice)
//@CASEGROUP:7、CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice)
//@DESCRIPTION:组合测试DD和DD+节目在HDMI输出设备上输出效果，CSUDIAUDIOMute\CSUDIAUDIOUnmute\CSUDIAOUTMute\CSUDIAOUTUnmute同CSUDIAOUTSetOPMode组合测试
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、HDMI设备不支持DD和DD+解码
//@INPUT: 
//@EXPECTATION: 每一步都获得期望值
//@EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIAUDIOGetCount和CSUDIAUDIOGetCapability获取支持AC3类型decode模式的音频解码器索引值nAudioIndex,在nAudioIndex上播放"DolbyAC3标准测试"，如果不支持AC3解码，直接退出本测试用例
//@EXECUTIONFLOW: 3、调用CSUDIAOUTSetOPMode设置HDMI输出为decode模式，在nAudioIndex上播放"DolbyAC3标准测试"，期望播放成功，所有设备有声音输出
//@EXECUTIONFLOW: 4、调用CSUDIAOUTSetOPMode设置HDMI输出为bypass模式,期望HDMI设备无声音输出，RCA输出有声音
//@EXECUTIONFLOW: 5、调用CSUDIAOUTSetOPMode设置HDMI输出为decode模式,期望HDMI设备有声音输出
//@EXECUTIONFLOW: 6、调用CSUDIAUDIOMute 将nAudioIndex音频输出静音，期望所有设备无声音输出
//@EXECUTIONFLOW: 7、调用CSUDIAOUTSetOPMode设置HDMI输出为bypass模式，期望所有设备无声音输出
//@EXECUTIONFLOW: 8、调用CSUDIAUDIOUnmute将nAudioIndex音频输出打开,期望HDMI设备无声音输出，RCA有声音输出
//@EXECUTIONFLOW: 9、调用CSUDIAOUTSetOPMode设置HDMI输出为decode模式,期望所有设备有声音输出
//@EXECUTIONFLOW: 10、调用CSUDIAOUTMute将HDMI设备输出关闭,期望HDMI设备无声音输出，RCA有声音输出
//@EXECUTIONFLOW: 11、 调用CSUDIAOUTSetOPMode设置HDMI输出为bypass模式,期望HDMI设备无声音输出，RCA有声音输出
//@EXECUTIONFLOW: 12、调用CSUDIAOUTUnmute将HDMI设备输出打开，期望HDMI设备无声音输出，RCA有声音输出
//@EXECUTIONFLOW: 13、调用CSUDIAOUTSetOPMode设置HDMI输出为decode模式,期望所有设备有声音输出
//@EXECUTIONFLOW: 14、恢复现场
CSUDI_BOOL CSTC_AUDIO_AOUT_IT_0002(void)
{
	AudioTestOutputCfg_S sOutputCfg;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int nAudioIndex = -1;

	if (!IsAOUTSupport(EM_UDIAOUT_HDMI))
	{
		CSTCPrint("Not support HDMI!\n");
		return CSUDI_FALSE;
	}
	
	memset(&sOutputCfg,0,sizeof(AudioTestOutputCfg_S));
	
	//EXECUTIONFLOW: 1、锁定Audio&Video Test_27Mbps_20070524.ts码流所在的频点
	//EXECUTIONFLOW: 2、调用CSUDIAUDIOGetCount和CSUDIAUDIOGetCapability获取支持AC3类型decode模式的音频解码器索引值nAudioIndex,在nAudioIndex上播放"DolbyAC3标准测试"，如果不支持AC3解码，直接退出本测试用例
	//EXECUTIONFLOW: 3、调用CSUDIAOUTSetOPMode设置HDMI输出为decode模式，在nAudioIndex上播放"DolbyAC3标准测试"，期望播放成功，所有设备有声音输出
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == AUDIO_iGetAudioIndexWithDecodeByType(EM_UDI_AUD_STREAM_AC3,&nAudioIndex)),
		"There's no audio support EM_UDI_AUD_STREAM_AC3 !!!!"
	);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iStartPlay(nAudioIndex ,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg,&hPlayer,CSUDI_TRUE)),
			"step 3 start play DolbyAC3标准测试 failure!!!!"
		);	

	CSTCPrint("step 3 start play DolbyAC3标准测试!!\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckPlay(nAudioIndex ,&s_sAudioServices[EM_AUDIO_DOLBYAC3],&sOutputCfg)),
			"step 3 check play DolbyAC3标准测试 failure!!!!"
		);	

	CSTCPrint("以下操作请用不支持DD解码的HDMI设备测试!! \r\n");	
	CSTKWaitYes();
	
	//EXECUTIONFLOW: 4、调用CSUDIAOUTSetOPMode设置HDMI输出为bypass模式,期望HDMI设备无声音输出，RCA输出有声音
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_BYPASS)),
			"step 4 set HDMI output bypass mode failure!!!!"
		);

	CSTCPrint("step 4 set HDMI output bypass mode!!\r\n");
	
	CSTCPrint("期望HDMI设备无声音输出，RCA输出有声音!!\r\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 4 check result failure !!!!"
		);

	//EXECUTIONFLOW: 5、调用CSUDIAOUTSetOPMode设置HDMI输出为decode模式,期望HDMI设备有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
			"step 5 set HDMI output decode mode failure!!!!"
		);
	
	CSTCPrint("step 5 set HDMI output decode mode!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 5 check result failure !!!!"
		);

	//EXECUTIONFLOW: 6、调用CSUDIAUDIOMute 将nAudioIndex音频输出静音，期望所有设备无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioIndex)),
			"step 6 mute audio decode failure!!!!"
		);

	CSTCPrint("step 6 mute audio decode!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 6 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 7、调用CSUDIAOUTSetOPMode设置HDMI输出为bypass模式，期望所有设备无声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_BYPASS)),
			"step 7 set HDMI output bypass mode failure!!!!"
		);
	
	CSTCPrint("step 7 set HDMI output bypass mode!! \r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_FALSE)),
			"Step 7 check result failure !!!!"
		);

	//EXECUTIONFLOW: 8、调用CSUDIAUDIOUnmute将nAudioIndex音频输出打开,期望HDMI设备无声音输出，RCA有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioIndex)),
			"step 8 unmoute audio decode  failure!!!!"
		);
	
	CSTCPrint("step 8 unmoute audio decode!! \r\n");
	CSTCPrint("期望HDMI设备无声音输出，RCA输出有声音!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 8 check result failure !!!!"
		);

	//EXECUTIONFLOW: 9、调用CSUDIAOUTSetOPMode设置HDMI输出为decode模式,期望所有设备有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
			"step 9 set HDMI output decode mode failure!!!!"
		);
	
	CSTCPrint("step 9 set HDMI output decode mode!! \r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 9 check result failure !!!!"
		);

	//EXECUTIONFLOW: 10、调用CSUDIAOUTMute将HDMI设备输出关闭,期望HDMI设备无声音输出，RCA有声音输出
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAOUTMute(EM_UDIAOUT_HDMI)),
				"step 10 mute HDMI output  failure!!!!"
			);
	
	CSTCPrint("step 10 mute HDMI output!! \r\n");
	CSTCPrint("期望HDMI设备无声音输出，RCA输出有声音!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 10 check result failure !!!!"
		);

	//EXECUTIONFLOW: 11、 调用CSUDIAOUTSetOPMode设置HDMI输出为bypass模式,期望HDMI设备无声音输出，RCA有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_BYPASS)),
			"step 11 set HDMI output bypass mode failure!!!!"
		);

	CSTCPrint("step 11 set HDMI output bypass mode!! \r\n");
	CSTCPrint("期望HDMI设备无声音输出，RCA输出有声音!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 11 check result failure !!!!"
		);

	//EXECUTIONFLOW: 12、调用CSUDIAOUTUnmute将HDMI设备输出打开，期望HDMI设备无声音输出，RCA有声音输出
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIAOUTUnmute(EM_UDIAOUT_HDMI)),
				"step 12 unmute HDMI output  failure!!!!"
			);

	CSTCPrint("step 12 unmute HDMI output!! \r\n");
	CSTCPrint("期望HDMI设备无声音输出，RCA输出有声音!!\r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE ==	CSTKWaitYes()),
			"Step 12 check result failure !!!!"
		);

	//EXECUTIONFLOW: 13、调用CSUDIAOUTSetOPMode设置HDMI输出为decode模式,期望所有设备有声音输出
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIAOUTSetOPMode(EM_UDIAOUT_HDMI,EM_UDIAOUT_DECODE)),
			"step 13 set HDMI output decode mode failure!!!!"
		);

	CSTCPrint("step 13 set HDMI output decode mode!! \r\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == AUDIO_iCheckVolume(CSUDI_TRUE)),
			"Step 13 check result failure !!!!"
		);
	
	//EXECUTIONFLOW: 14、恢复现场
	CSTK_FATAL_POINT
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == AUDIO_iStopPlay(nAudioIndex,hPlayer,CSUDI_TRUE)),
				"Step 14 stop play failure !!!!"
			);
		hPlayer = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}


