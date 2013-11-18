/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_videotestcase.h" 
#include "../cs_udi2testcase.h" 
#include "udi2_osg.h"

#include "udi2_audio.h"
#include "udi2_error.h"
#include "udi2_demux.h"
#include "udi2_tuner.h"
#include "udi2_player.h"
#include "udi2_vout.h"
#include "udi2_player.h"
#include "udi2_inject.h"
#include "udi2_public.h"
#include "udi2_os.h"
#include "udi2_fs.h"
#include "udi2_video.h"
#include "cs_testkit.h"

static int g_nFullScreenWidth_SD = 0;
static int g_nFullScreenHeight_SD = 0;
static int g_nFullScreenWidth_HD = 0;
static int g_nFullScreenHeight_HD = 0;
static int g_nFullWindowWidth_HD = 0;
static int g_nFullWindowHeight_HD = 0;
static CSUDI_BOOL g_bShareHdDispalySurface = CSUDI_FALSE;
static CSUDI_BOOL g_bSupportHdDispaly = CSUDI_FALSE;

#define INJECTTHREADPRO (210)
#define INJECTTHREADBUF (1024 * 148)
#define INJECT_SEEK_SET (0)
#define INJECT_SEEK_CUR (1)
#define INJECT_SEEK_END (2)
#define INJECT_INVALID_PID (0x1fff)
#define MAX_PESES_COUNT_IN_TS (EM_UDI_CONTENT_PCR+1) /*请参考CSUDIContentType_E枚举的类型个数*/
typedef enum 
{
    EM_INJECT_TS_H264_AAC,
    EM_INJECT_TS_MAX_COUNT
}ES_ServiceIndex_E;

typedef struct _TSStreamInfo_S
{
    char * m_pcName; //注入文件的名
    CSUDIStreamInfo_S    m_TSContentInfo[MAX_PESES_COUNT_IN_TS];
    unsigned int m_uBufferLen;
    CSUDI_BOOL m_bRun;
    CSUDI_HANDLE m_hInjecter;
}TSStreamInfo_S;

static TSStreamInfo_S g_TSStreamInfo[EM_INJECT_TS_MAX_COUNT] = 
{
	{
		"Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts",
		{
			{0x41, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_MPEG2}},   
			{0x42, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG2}}, 
			{0x41, EM_UDI_CONTENT_PCR, {0}}, 
			{INJECT_INVALID_PID, 0, {0}},   
			{INJECT_INVALID_PID, 0, {0}},   
		},
		10*1024,	
		CSUDI_TRUE,
		CSUDI_NULL,	
	},
};


static CSUDI_BOOL testInit()
{
	char cBuf[32] = {0};
	int nResult = CSUDI_FAILURE;
	
	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("OSG", "CS_UDI2OSG_SD_DISPLAY_WIDTH", cBuf, sizeof(cBuf));
	if(CSUDI_SUCCESS == nResult)
	{
		g_nFullScreenWidth_SD = CSTKGetIntFromStr(cBuf,10);
	}
	else
	{
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]字段中找不到CS_UDI2OSG_SD_DISPLAY_WIDTH\r\n");
		return CSUDI_FALSE;
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("OSG", "CS_UDI2OSG_SD_DISPLAY_HEIGHT", cBuf, sizeof(cBuf));
	if(CSUDI_SUCCESS == nResult)
	{
		g_nFullScreenHeight_SD = CSTKGetIntFromStr(cBuf,10);
	}
	else
	{
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]字段中找不到CS_UDI2OSG_SD_DISPLAY_HEIGHT\r\n");
		return CSUDI_FALSE;
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("OSG", "CS_UDI2OSG_HD_DISPLAY_WIDTH", cBuf, sizeof(cBuf));
	if(CSUDI_SUCCESS == nResult)
	{
		g_nFullScreenWidth_HD = CSTKGetIntFromStr(cBuf,10);
	}
	else
	{
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]字段中找不到CS_UDI2OSG_HD_DISPLAY_WIDTH\r\n");
		return CSUDI_FALSE;
	}
	
	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("OSG", "CS_UDI2OSG_HD_DISPLAY_HEIGHT", cBuf, sizeof(cBuf));
	if(CSUDI_SUCCESS == nResult)
	{
		g_nFullScreenHeight_HD = CSTKGetIntFromStr(cBuf,10);
	}
	else
	{
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]字段中找不到CS_UDI2OSG_HD_DISPLAY_HEIGHT\r\n");
		return CSUDI_FALSE;
	}

	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("OSG", "CS_UDIVIDEO_HD_WINDOW_WIDTH", cBuf, sizeof(cBuf));
	if(CSUDI_SUCCESS == nResult)
	{
		g_nFullWindowWidth_HD = CSTKGetIntFromStr(cBuf,10);
	}
	else
	{
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]字段中找不到CS_UDIVIDEO_HD_WINDOW_WIDTH\r\n");
		return CSUDI_FALSE;
	}

	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("OSG", "CS_UDIVIDEO_HD_WINDOW_HEIGHT", cBuf, sizeof(cBuf));
	if(CSUDI_SUCCESS == nResult)
	{
		g_nFullWindowHeight_HD = CSTKGetIntFromStr(cBuf,10);
	}
	else
	{
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]字段中找不到CS_UDIVIDEO_HD_WINDOW_HEIGHT\r\n");
		return CSUDI_FALSE;
	}

	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("OSG", "CS_OSGUDI2_SURPORT_HD_SD_DISPLAY", cBuf, sizeof(cBuf));
	if(CSUDI_SUCCESS == nResult)
	{
		g_bSupportHdDispaly = (CSUDI_BOOL)CSTKGetIntFromStr(cBuf,10);
	}
	else
	{
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]字段中找不到CS_OSGUDI2_SURPORT_HD_SD_DISPLAY\r\n");
		return CSUDI_FALSE;
	}

	memset(cBuf, '\0', sizeof(cBuf));
	nResult =  CSTKGetConfigInfo("OSG", "CS_OSGUDI2_SHARE_HD_DISPLAY_SERFACE", cBuf, sizeof(cBuf));
	if(CSUDI_SUCCESS == nResult)
	{
		g_bShareHdDispalySurface = (CSUDI_BOOL)CSTKGetIntFromStr(cBuf,10);
	}
	else
	{
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]字段中找不到CS_OSGUDI2_SHARE_HD_DISPLAY_SERFACE\r\n");
		return CSUDI_FALSE;
	}

	return CSUDI_TRUE;
}

static CSUDI_BOOL testUnInit()
{
	g_nFullScreenWidth_SD = 0;
	g_nFullScreenHeight_SD = 0;
	g_nFullScreenWidth_HD = 0;
	g_nFullScreenHeight_HD = 0;

	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_VIDEO_Init(void)
{
	//在本测试用例集执行前调用
	return testInit();
}

CSUDI_BOOL CSTC_VIDEO_UnInit(void)
{
	//在本测试用例集执行后调用
	return testUnInit();
}

/**
@brief 判断是否支持高清输出

@return 如果平台支持，则返回CSUDI_TRUE；平台不支持则返回CSUDI_FALSE
*/
static CSUDI_BOOL CSTC_VIDEO_IsSupportHdDispaly(void)
{
	return g_bSupportHdDispaly;
}

/**
@brief 判断高标清是否同源输出，即高标清共用一个高清显存

@return 如果平台支持，则返回CSUDI_TRUE；平台不支持则返回CSUDI_FALSE
*/
static CSUDI_BOOL CSTC_VIDEO_IsShareHDDisplaySurface(void)
{
	return g_bShareHdDispalySurface;
}

//获取输入pResult的进制。 0x/0X开头为16进制，0开头为8进制，否则为10进制
int CSTC_VIDEO_CheckStr(const char *pResult)
{
	int nResult = 10;
	char cTmp[] = "0xX";

	if (CSUDI_NULL == pResult)
	{
		CSTCPrint("[%s][line:%d] pResult is NULL!!!\n\r", __FUNCTION__, __LINE__);
		return -1;
	}

	if (cTmp[0] == pResult[0])
	{
		if((cTmp[1] == pResult[1]) || (cTmp[2] == pResult[1]))
		{
			nResult = 16;
		}
		else
		{
			nResult = 8;
		}
	}
	return nResult;
}

CSUDI_BOOL CSTC_VIDEO_CheckFromVideoToConfig(const char *pSection,const char *pKey, int nCheck)
{
	char acResult[255] = {0};
	int nResultSize = (int)sizeof(acResult);
	int nBase = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;

	if ((CSUDI_NULL == pSection) || (CSUDI_NULL == pKey))
	{
		CSTCPrint("输入参数指针为空!\n");
		return bResult;
	}
	if (CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo(pSection, pKey, acResult, nResultSize))
	{
		CSTCPrint("获取Config文件信息失败!\n");
		return bResult;
	}
	
	nBase = CSTC_VIDEO_CheckStr(acResult);
	if (-1 == nBase)
	{
		CSTCPrint("无法获得Config 信息所对应的进制(16/10/8)\n");
		return bResult;
	}
	if (nCheck != CSTKGetIntFromStr(acResult, nBase))
	{	
		CSTCPrint("Video信息和Config信息不一致!\n");
		return bResult;
	}
	
	bResult = CSUDI_TRUE;
	
	return bResult;
}

/*测试中流的类型*/
typedef struct _CSTC_StreamType_S
{
	char *m_pcSteamType;
	CSUDIVIDStreamType_E m_eSteamType;
}CSTC_StreamType_S;

static CSTC_StreamType_S s_asStreamType[] = 
{
	{"MPEG1", EM_UDI_VID_STREAM_MPEG1 }, 
	{"MPEG2", EM_UDI_VID_STREAM_MPEG2},
	{"MPEG4", EM_UDI_VID_STREAM_MPEG4},
	{"H264", EM_UDI_VID_STREAM_H264},
	{"H263", EM_UDI_VID_STREAM_H263 },
	{"VC1", EM_UDI_VID_STREAM_VC1 },
	{"VC1_SM", EM_UDI_VID_STREAM_VC1_SM},
	{"DIVX_311", EM_UDI_VID_STREAM_DIVX_311},
};

/**
* 将标清的矩形区域转化成对应的高清的值
* 该接口主要用于Video的矩形区域的转化
* 该接口主要用于高标清同源的平台的测试
*/
static CSUDI_BOOL CSTC_VIDEO_iWinRect_SDTransformHD(CSUDIWinRect_S* psRect)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	char acBuf[64] = {0};
	int nHDWinWidth = 0;
	int nHDWinHeight = 0;

	memset(acBuf, '\0', sizeof(acBuf));       
	CSTKGetConfigInfo("OSG", "CS_UDIVIDEO_HD_WINDOW_WIDTH", acBuf, sizeof(acBuf));
	if (acBuf[0] != '\0')
	{
		nHDWinWidth = CSTKGetIntFromStr(acBuf, 10);
	}

	memset(acBuf, '\0', sizeof(acBuf));       
	CSTKGetConfigInfo("OSG", "CS_UDIVIDEO_HD_WINDOW_HEIGHT", acBuf, sizeof(acBuf));
	if (acBuf[0] != '\0')
	{
		nHDWinHeight = CSTKGetIntFromStr(acBuf, 10);
	}

	if ((nHDWinWidth != 0 && nHDWinHeight != 0) && (psRect != CSUDI_NULL))
	{
		psRect->m_nX = (psRect->m_nX*nHDWinWidth/720);
		psRect->m_nY = (psRect->m_nY*nHDWinHeight/576);
		psRect->m_nWidth = (psRect->m_nWidth*nHDWinWidth/720);
		psRect->m_nHeight = (psRect->m_nHeight*nHDWinHeight/576);
		
		CSTCPrint("[%s] : HDRect {x:%d, y:%d, w:%d, h:%d}\n", __FUNCTION__, psRect->m_nX, psRect->m_nY, psRect->m_nWidth, psRect->m_nHeight);

		bRet = CSUDI_TRUE;
	}
	
	return bRet;
}

/**
* 将标清的矩形区域转化成对应的高清的值
* 该接口主要用于OSD的矩形区域的转化
* 该接口主要用于高标清同源的平台的测试
*/
static CSUDI_BOOL CSTC_VIDEO_iOSGRect_SDTransformHD(CSUDIOSGRect_S* psRect)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	char acBuf[64] = {0};
	int nHDDisplayWidth = 0;
	int nHDDisplayHeight = 0;

	memset(acBuf, '\0', sizeof(acBuf));       
	CSTKGetConfigInfo("OSG", "CS_UDI2OSG_HD_DISPLAY_WIDTH", acBuf, sizeof(acBuf));
	if (acBuf[0] != '\0')
	{
		nHDDisplayWidth = CSTKGetIntFromStr(acBuf, 10);
	}

	memset(acBuf, '\0', sizeof(acBuf));       
	CSTKGetConfigInfo("OSG", "CS_UDI2OSG_HD_DISPLAY_HEIGHT", acBuf, sizeof(acBuf));
	if (acBuf[0] != '\0')
	{
		nHDDisplayHeight = CSTKGetIntFromStr(acBuf, 10);
	}

	if ((nHDDisplayWidth != 0 && nHDDisplayHeight != 0) && (psRect != CSUDI_NULL))
	{
		psRect->m_nX = (psRect->m_nX*nHDDisplayWidth/720);
		psRect->m_nY = (psRect->m_nY*nHDDisplayHeight/576);
		psRect->m_nWidth = (psRect->m_nWidth*nHDDisplayWidth/720);
		psRect->m_nHeight = (psRect->m_nHeight*nHDDisplayHeight/576);
		
		CSTCPrint("[%s] : HDRect {x:%d, y:%d, w:%d, h:%d}\n", __FUNCTION__, psRect->m_nX, psRect->m_nY, psRect->m_nWidth, psRect->m_nHeight);

		bRet = CSUDI_TRUE;
	}
	
	return bRet;
}

CSUDI_BOOL CSTC_VIDEO_CheckFromConfigToVideo(const CSUDIVIDEOCapability_S *psCapabilityInfo, 
															CSUDIVIDStreamType_E  eStreamType, int nIsAble)
{
	int nStreamTypeIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;

	if (CSUDI_NULL == psCapabilityInfo)
	{
		CSTCPrint("[%s][line:%d] psCapabilityInfo is NULL!!!\n\r", __FUNCTION__, __LINE__);
	}
	
	for (nStreamTypeIndex = 0; nStreamTypeIndex < EM_UDI_VID_STREAMTYPE_NUM; nStreamTypeIndex++)
	{
		if ((eStreamType == psCapabilityInfo->m_eStreamType[nStreamTypeIndex])
			||(EM_UDI_VID_STREAM_UNKNOWN == psCapabilityInfo->m_eStreamType[nStreamTypeIndex]))
		{
			break;
		}
	}

	if ((nStreamTypeIndex < EM_UDI_VID_STREAMTYPE_NUM)
		&& (EM_UDI_VID_STREAM_UNKNOWN != psCapabilityInfo->m_eStreamType[nStreamTypeIndex]))
	{
		if (nIsAble)
		{
			bResult = CSUDI_TRUE;
		}
		else
		{
			bResult = CSUDI_FALSE;
		}
	}
	else
	{
		if (nIsAble)
		{
			bResult = CSUDI_FALSE;
		}
		else
		{
			bResult = CSUDI_TRUE;
		}
	}

	return bResult;	
}

//查找支持nDemxType类型的demx
//nDemxType参见CSUDIDEMUXWorkType_E
static int CSUDI_VIDEO_FindDemuxID(int nDemxType)
{
    int i = 0;
    int nDemuxCount = 0;
    int nDemuxID = -1;
    CSUDIDEMUXCapability_S sDemuxCapabilityInfo;

    if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //先取得demux的数量
    {
        for (i = 0; i < nDemuxCount; i++)
        {
            if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //取相对应ID=i 的demux的能力
            {
                if((sDemuxCapabilityInfo.m_dwWorkTypeMask & nDemxType) != 0)
                {
                    nDemuxID = i;
                    break;
                }
            }   
			else
			{
				CSTCPrint("[%s][line:%d] CSUDIDEMUXGetCapability failed!!! Demux Index:%d\n\r", __FUNCTION__, __LINE__, i);
			}
        }
    }
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIDEMUXGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
	}
    CSTCPrint("nDemuxID=%d\n\r", nDemuxID);
    return  nDemuxID;
}

//寻找视频解码器
static int CSUDI_VIDEO_FindVideoDecoder(CSUDIVIDStreamType_E eFindVideoType)
{
    CSUDIVIDEOCapability_S sVideoCapabilityInfo;
    int nVideoDecCount = 0;
    int nVideoID = -1;
    int i = 0;
	int k = 0;

    if(CSUDI_SUCCESS != CSUDIVIDEOGetCount(&nVideoDecCount))
	{
		CSTCPrint("[%s][line:%d] CSUDIVIDEOGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
		return -1;
	}
		
    for (i = 0; i < nVideoDecCount; i++)
    {
    	memset(&sVideoCapabilityInfo, 0, sizeof(sVideoCapabilityInfo));
		if(CSUDI_SUCCESS == CSUDIVIDEOGetCapability(i, &sVideoCapabilityInfo))
		{
			for (k = 0; k < EM_UDI_VID_STREAMTYPE_NUM; k++)
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
		else
		{
			CSTCPrint("[%s][line:%d] CSUDIVIDEOGetCapability failed!!! Video Index:%d\n\r", __FUNCTION__, __LINE__, i);
		}
              
        if (nVideoID != -1)
        {
            break;
        }
    }
    CSTCPrint(" nVideoID=%d\n\r", nVideoID);

    return nVideoID;
}

//寻找音频解码器
static int CSUDI_VIDEO_FindAudioDecoder(CSUDIVIDStreamType_E eFindAudioType)
{
    CSUDIAUDIOCapability_S sAudioCapabilityInfo;
    int nAudioDecCount = 0;
    int nAudioID = -1;
    int i = 0;
	int k = 0;

	if(CSUDI_SUCCESS != CSUDIAUDIOGetCount(&nAudioDecCount))
	{
		CSTCPrint("[%s][line:%d] CSUDIAUDIOGetCount failed!!!\n\r", __FUNCTION__, __LINE__);
		return -1;
	}
	
    for (i = 0; i < nAudioDecCount; i++)
    {
        memset(&sAudioCapabilityInfo, 0, sizeof(sAudioCapabilityInfo));
		if(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo))
		{
			for (k = 0; k < EM_UDI_AUD_STREAMTYPE_NUM; k++)
			{
				if (eFindAudioType == sAudioCapabilityInfo.m_sSupportDecode[k])
				{
					nAudioID = i;
					break;
				}
				if (EM_UDI_AUD_STREAMTYPE_NUM == sAudioCapabilityInfo.m_sSupportDecode[k])
				{
					break;
				}
            }
		
        }
		else
		{
			CSTCPrint("[%s][line:%d] CSUDIAUDIOGetCapability failed!!! Audio Index:%d\n\r", __FUNCTION__, __LINE__, i);
		}
              
        if (nAudioID != -1)
        {
            break;
        }
    }
    CSTCPrint(" nAudioID=%d\n\r", nAudioID);
    
    return nAudioID;
}

/*标清码流锁频*/
static CSUDI_BOOL LockFrequency(CSUDI_HANDLE *phPlayer)
{
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[2];
	CSUDI_BOOL bResult = CSUDI_FALSE; 
	int nStreamCnt =2;
	int nTunerIndex =0;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	//////////////////////设置节目信息
	sStreamInfo[0].m_nPid=1140;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	sStreamInfo[1].m_nPid=1141;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;

	sPlayerChnl.m_nDemux = CSUDI_VIDEO_FindDemuxID(EM_UDI_DEMUX_PLAY);
	sPlayerChnl.m_nVideoDecoder = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_MPEG2);
	sPlayerChnl.m_nAudioDecoder = CSUDI_VIDEO_FindAudioDecoder(EM_UDI_AUD_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex,"Audio&Video Test_27Mbps_20070524.ts"), "锁频失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,phPlayer)), "打开播放器失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != phPlayer, "创建播放器句柄失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (*phPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "设置解码信息失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(*phPlayer), "开始播放失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE), "打开视频流失败\n");

	CSUDIOSThreadSleep(1500);
	CSTCPrint("请将标清输出连接到标清电视上\n");
	CSTCPrint("音视频是否能够正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"播放不正常");
	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;
    
    g_bAssertBeforeFatalPoint = CSUDI_TRUE;

	return bResult;
} 

static CSUDI_BOOL PlayLocalProgram(CSUDI_HANDLE *phPlayer ,int nStreamCnt ,CSUDIStreamInfo_S  sStreamInfo[], char *pTSFilename)
{
	CSUDIPlayerChnl_S  sPlayerChnl;
    CSUDI_BOOL bResult = CSUDI_FAILURE; 
    int nTunerIndex =0;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	sPlayerChnl.m_nDemux = CSUDI_VIDEO_FindDemuxID(EM_UDI_DEMUX_PLAY);
	sPlayerChnl.m_nVideoDecoder = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_MPEG2);
	sPlayerChnl.m_nAudioDecoder = CSUDI_VIDEO_FindAudioDecoder(EM_UDI_AUD_STREAM_MPEG2);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex, pTSFilename), "锁频失败!");
    CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,phPlayer)), "打开播放器失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != phPlayer, "创建播放器句柄失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (*phPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "设置解码信息失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(*phPlayer), "开始播放失败!");

	CSUDIOSThreadSleep(1500);

    bResult = CSUDI_SUCCESS;

	CSTK_FATAL_POINT;
    
    g_bAssertBeforeFatalPoint = CSUDI_TRUE;

	return bResult;		
}

/*高清码流锁频*/
static CSUDI_BOOL LockHDFrequency(CSUDI_HANDLE *phPlayer)
{
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[2];
	CSUDI_BOOL bResult = CSUDI_FALSE; 
	int nStreamCnt =2;
	int nTunerIndex =0;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	//////////////////////设置节目信息
	sStreamInfo[0].m_nPid=0x33c;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_H264;

	sStreamInfo[1].m_nPid=0x33d;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG1;

	sPlayerChnl.m_nDemux = CSUDI_VIDEO_FindDemuxID(EM_UDI_DEMUX_PLAY);
	sPlayerChnl.m_nVideoDecoder = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_H264);
	sPlayerChnl.m_nAudioDecoder = CSUDI_VIDEO_FindAudioDecoder(EM_UDI_AUD_STREAM_MPEG1);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex,"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts"), "锁频失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,phPlayer)), "打开播放器失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != phPlayer, "创建播放器句柄失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (*phPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "设置解码信息失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(*phPlayer), "开始播放失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE), "打开视频流失败\n");

	CSUDIOSThreadSleep(1500);
	CSTCPrint("请将高清输出连接到高清电视上\n");
	CSTCPrint("音视频是否能够正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"播放不正常");
	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;

	g_bAssertBeforeFatalPoint = CSUDI_TRUE;

	return bResult;
} 

static BOOL bIsSupportHD()
{
	CSUDIVIDEOCapability_S sCapabilityInfo;
	int i = 0;
	BOOL bIsSupport = FALSE;
	
	if (CSUDI_SUCCESS == CSUDIVIDEOGetCapability(0, &sCapabilityInfo))
	{
		for(i=0;i<EM_UDI_VID_STREAMTYPE_NUM;i++)
		{
			if (sCapabilityInfo.m_eStreamType[i] == EM_UDI_VID_STREAM_UNKNOWN)
			{
				break;
			}
			if (sCapabilityInfo.m_eStreamType[i] == EM_UDI_VID_STREAM_MPEG4
				|| sCapabilityInfo.m_eStreamType[i] == EM_UDI_VID_STREAM_H264)
			{
				bIsSupport = TRUE;
			}
		}
	}
	else
	{
		CSTCPrint("[%s][line:%d] CSUDIVIDEOGetCapability failed!!! Video Index:%d\n\r", __FUNCTION__, __LINE__, i);
	}

	return bIsSupport;
}

/*设置指定视频解码输出的窗口大小，用于测试*/
typedef struct _CSTC_WindowSize_S
{
	int m_nTestID;
	CSUDIWinRect_S	*m_sWinRect;
	char			*m_pcDes;
}CSTC_WindowSize_S;

static CSUDIWinRect_S s_aWinRect_WinSize_SD[] = 
{
	{0,		 0, 	 720/2,  576/2},
	{720/2,576/2,720/3,	576/3},
	{50, 	50, 	720,	 576   },
	{0,		 0, 	 720/3,  576/3},
	{720/2, 0,	   720/3,  576/2},
	{	  0, 576/2,720/2,  576/3},
	{720/3,576/3, 720/3,  576/3},
	{-50,   -50, 	720,	 576   },
	{720/2, 576/2,	  -1,	  -1  },
};

static CSUDIWinRect_S s_aWinRect_WinSize_HD[] = 
{
	{0,		 0, 	 1920/2,  1080/2},
	{1920/2,1080/2,1920/3,	1080/3},
	{50, 	50, 	1920,	 1080   },
	{0,		 0, 	 1920/3,  1080/3},
	{1920/2, 0,	   1920/3,  1080/2},
	{	  0, 1080/2,1920/2,  1080/3},
	{1920/3,1080/3, 1920/3,  1080/3},
	{-50,   -50, 	1920,	 1080   },
	{1920/2, 1080/2,	  -1,	  -1  },
};
static CSTC_WindowSize_S s_aSetWindowSizeTestCase[]=
{
	{0,CSUDI_NULL,"全屏显示"},
	{1,&s_aWinRect_WinSize_SD[0],"位于屏幕左上角{0, 0, 720/2, 576/2}的缩小图像"},
	{2,&s_aWinRect_WinSize_SD[1],"位于屏幕右下方{720/2, 576/2, 720/3, 576/3}的缩小图像"},
	{3,&s_aWinRect_WinSize_SD[2],"图像右下方向移位50个像素，移出屏幕的部分被切除"},
	{4,&s_aWinRect_WinSize_SD[3],"位于屏幕左上角{0, 0, 720/3, 576/3}的缩小图像"},
	{5,&s_aWinRect_WinSize_SD[4],"位于屏幕右上方{720/2, 0, 720/3, 576/2}的拉长缩小图像"},
	{6,&s_aWinRect_WinSize_SD[5],"位于屏幕左下方{0, 576/2, 720/2, 576/3}的压扁缩小图像"},
	{7,&s_aWinRect_WinSize_SD[6],"位于屏幕中间位置{720/3, 576/3, 720/3, 576/3}的缩小图像"},
	{8,&s_aWinRect_WinSize_SD[7],"图像左上方向移位50个像素,移出屏幕的部分被切除"},
	{9,&s_aWinRect_WinSize_SD[8],"参数错误，窗口宽高应为大于0的数"},
	{10,CSUDI_NULL,"全屏显示"},
	{11,&s_aWinRect_WinSize_HD[0],"位于屏幕左上角{0, 0, 1920/2, 1080/2}的缩小图像"},
	{12,&s_aWinRect_WinSize_HD[1],"位于屏幕右下方{1920/2, 1080/2, 1920/3, 1080/3}缩小图像"},
	{13,&s_aWinRect_WinSize_HD[2],"图像右下方向移位50个像素，移出屏幕的部分被切除"},
	{14,&s_aWinRect_WinSize_HD[3],"位于屏幕左上角{0, 0, 1920/3, 1080/3}的缩小图像"},
	{15,&s_aWinRect_WinSize_HD[4],"位于屏幕右上方{1920/2, 0, 1920/3, 1080/2}的拉长缩小图像"},
	{16,&s_aWinRect_WinSize_HD[5],"位于屏幕左下方{0, 1080/2, 1920/2, 1080/3}屏幕大小的压扁缩小图像"},
	{17,&s_aWinRect_WinSize_HD[6],"位于屏幕中间位置{1920/3, 1080/3, 1920/3, 1080/3}的缩小图像"},
	{18,&s_aWinRect_WinSize_HD[7],"图像左上方向移位50个像素,移出屏幕的部分被切除"},
	{19,&s_aWinRect_WinSize_HD[8],"参数错误，窗口宽高应为大于0的数"}
};

/*WinClip和WinSize 测试数据,测试视频解码输出的窗口大小，实现缩放功能*/
typedef struct _CSTC_WinClipAndWinSize_S
{
	int m_nTestID;
	CSUDISCREENType_E m_eDevice;
	CSUDIWinRect_S	*m_sWinClip;
	CSUDIWinRect_S	*m_sWinRect;
	char			*m_pcDes;
}CSTC_WinClipAndWinSize_S;

static CSUDIWinRect_S s_aWinRect_WinChip_SD[]=
{
	{0,0,720/2, 576/2},
	{720/2, 576/2,720/2, 576/2},
	{0, 576/2,720/2, 576/2},
	{720/2, 0,720/2, 576/2},
	{720/3, 576/3,720/3, 576/3},
	{2*720/3, 2*576/3,720/3, 576/3},	
	{720/2, 576/2, 720/2, -1  }
};

static CSUDIWinRect_S s_aWinRect_WinChip_HD[]=
{
	{0,0,1920/2, 1080/2},
	{1920/2, 1080/2,1920/2, 1080/2},
	{0, 1080/2,1920/2, 1080/2},
	{1920/2, 0,1920/2, 1080/2},
	{1920/3, 1080/3,1920/3, 1080/3},
	{2*1920/3, 2*1080/3,1920/3, 1080/3},	
	{1920/2, 1080/2, 1920/2, -1  }
};

static CSTC_WinClipAndWinSize_S s_aWinClipWindowSizeTestCase[]=
{
	{0,EM_UDI_VOUT_DEVICE_SD,CSUDI_NULL,&(s_aWinRect_WinChip_SD[0]), "全屏图像被压缩为1/4屏幕大小，位于屏幕左上角显示"},
	{1,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[0]),CSUDI_NULL,"左上角1/4屏幕大小的图像被放大到全屏显示"},
	{2,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[1]),CSUDI_NULL,"右下角1/4屏幕大小的图像被放大到全屏显示"},
	{3,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[2]),CSUDI_NULL,"左下角1/4屏幕大小的图像被放大到全屏显示"},
	{4,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[3]),CSUDI_NULL,"右上角1/4屏幕大小的图像被放大到全屏显示"},
	{5,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[4]),CSUDI_NULL,"屏幕正中心1/9屏幕大小的图像被放大到全屏显示"},
	{6,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[4]),&s_aWinRect_WinChip_SD[5],"屏幕正中心1/9屏幕大小的图像被剪切后，原样显示在屏幕右下角"},
	{7,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[6]),CSUDI_NULL,"剪切区域参数错误"},

	{8,EM_UDI_VOUT_DEVICE_HD,CSUDI_NULL,&(s_aWinRect_WinChip_HD[0]), "全屏图像被压缩为1/4屏幕大小，位于屏幕左上角显示"},
	{9,EM_UDI_VOUT_DEVICE_HD, &(s_aWinRect_WinChip_HD[0]),CSUDI_NULL,"左上角1/4屏幕大小的图像被放大到全屏显示"},
	{10,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[1]),CSUDI_NULL,"右下角1/4屏幕大小的图像被放大到全屏显示"},
	{11,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[2]),CSUDI_NULL,"左下角1/4屏幕大小的图像被放大到全屏显示"},
	{12,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[3]),CSUDI_NULL,"右上角1/4屏幕大小的图像被放大到全屏显示"},
	{13,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[4]),CSUDI_NULL,"屏幕正中心1/9屏幕大小的图像被放大到全屏显示"},
	{14,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[4]),&s_aWinRect_WinChip_HD[5],"屏幕正中心1/9屏幕大小的图像被剪切后，原样显示在屏幕右下角"},
	{15,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[6]),CSUDI_NULL,"剪切区域参数错误"},
		
};

/*视频截图(快照)接口测试信息，包括输入输出区域和Surface区域*/
typedef struct _CSTC_SnapShot_S 
{
	int 	m_nTestID;
	CSUDIWinRect_S	*m_sInputWinRect;
	CSUDIWinRect_S	*m_sOutputWinRect;
	int 				m_nSurfaceWidth;
	int 				m_nSurfaceHigh; 
	char *m_pcDes;
}CSTC_SnapShot_S;

static CSUDIWinRect_S s_aWinRect_SnapShot[]=
{	
	{0,0,720/2, 576/2},
	{720/2, 576/2,720/2, 576/2},
	{0, 576/2,720/2, 576/2},
	{720/2, 0,720/2, 576/2},
	{720/3, 576/3,720/3, 576/3},		
	{0, 0,250, 180},
};

static CSTC_SnapShot_S s_aSnapShotTest[]=
{
	{0,CSUDI_NULL,CSUDI_NULL,720,576,"全屏显示，静帧并且没有压缩和放大现象"},
	{1,CSUDI_NULL,CSUDI_NULL,250,180,"全屏图像被压缩为250*180大小，静帧并且显示窗口位于左上角,显示图像大小为250*180"},
	{2,CSUDI_NULL,&s_aWinRect_SnapShot[5],300,240,"全屏图像被压缩为250*180大小，静帧并且显示窗口位于左上角,显示图像大小为250*180"}, 
	{3,&s_aWinRect_SnapShot[5],CSUDI_NULL,300,240,"图像左上角250*180的区域拷贝到缓存空间，静帧并且显示窗口位于左上角,显示图像大小为300*240"},
	{4,&s_aWinRect_SnapShot[4],CSUDI_NULL,250,180,"图像正中间的1/9画面被压缩成250*180大小，静帧并且显示窗口位于左上角,显示图像大小为250*180"},
	{5,&s_aWinRect_SnapShot[3],CSUDI_NULL,250,180,"图像右上角的1/4画面被压缩成250*180大小，静帧并且显示窗口位于左上角,显示图像大小为250*180"},
	{6,&s_aWinRect_SnapShot[2],CSUDI_NULL,250,180,"图像左下角的1/4画面被压缩成250*180大小，静帧并且显示窗口位于左上角,显示图像大小为250*180"},
	{7,&s_aWinRect_SnapShot[1],CSUDI_NULL,250,180,"图像右下角的1/4画面被压缩成250*180大小，静帧并且显示窗口位于左上角,显示图像大小为250*180"},
	{8,&s_aWinRect_SnapShot[0],CSUDI_NULL,250,180,"图像左上角的1/4画面被压缩成250*180大小，静帧并且显示窗口位于左上角,显示图像大小为250*180"},
	{9,&s_aWinRect_SnapShot[0],&s_aWinRect_SnapShot[5],720,576,"图像左上角的1/4画面被压缩为250*180大小，静帧并且显示窗口位于左上角,显示图像大小为250*180"},
	{10,&s_aWinRect_SnapShot[0],&s_aWinRect_SnapShot[1],720,576,"图像左上角的1/4画面被移到右下角显示，静帧并且显示大小仍然是1/4画面"},
	{11,&s_aWinRect_SnapShot[0],&s_aWinRect_SnapShot[1],40,30,"缓存空间不足"},

};

static CSUDIWinRect_S s_aWinRect_SnapShot_HD[]=
{	
	{0, 0, 1920/2, 1080/2},
	{1920/2, 1080/2,1920/2, 1080/2},
	{0, 1080/2,1920/2, 1080/2},
	{1920/2, 0,1920/2, 1080/2},
	{1920/3, 1080/3, 1920/3, 1080/3},		
	{0, 0, 1920/2, 1080/2},
	//以下是相对1280*720的坐标位置
	{0, 0, 1280/2, 720/2},
	{1280/2, 720/2, 1280/2, 720/2},
};

static CSTC_SnapShot_S s_aSnapShotTest_HD[]=
{
	{0,CSUDI_NULL,CSUDI_NULL,1280,720,"全屏显示，静帧并且没有压缩和放大现象"},
	{1,CSUDI_NULL,CSUDI_NULL,640,360,"全屏图像被压缩为640*360大小，静帧并且显示窗口位于左上角,显示图像大小为640*360"},
	{2,CSUDI_NULL,&s_aWinRect_SnapShot_HD[6],800,500,"全屏图像被压缩为640*360大小，静帧并且显示窗口位于左上角,显示图像大小为640*360"}, 
	{3,&s_aWinRect_SnapShot_HD[5],CSUDI_NULL,800,500,"图像左上角640*360的区域拷贝到缓存空间，静帧并且显示窗口位于左上角,显示图像大小为800*500"},
	{4,&s_aWinRect_SnapShot_HD[4],CSUDI_NULL,640,360,"图像正中间的1/9画面被压缩成640*360大小，静帧并且显示窗口位于左上角,显示图像大小为640*360"},
	{5,&s_aWinRect_SnapShot_HD[3],CSUDI_NULL,640,360,"图像右上角的1/4画面被压缩成640*360大小，静帧并且显示窗口位于左上角,显示图像大小为640*360"},
	{6,&s_aWinRect_SnapShot_HD[2],CSUDI_NULL,640,360,"图像左下角的1/4画面被压缩成640*360大小，静帧并且显示窗口位于左上角,显示图像大小为640*360"},
	{7,&s_aWinRect_SnapShot_HD[1],CSUDI_NULL,640,360,"图像右下角的1/4画面被压缩成640*360大小，静帧并且显示窗口位于左上角,显示图像大小为640*360"},
	{8,&s_aWinRect_SnapShot_HD[0],CSUDI_NULL,640,360,"图像左上角的1/4画面被压缩成640*360大小，静帧并且显示窗口位于左上角,显示图像大小为640*360"},
	{9,&s_aWinRect_SnapShot_HD[0],&s_aWinRect_SnapShot_HD[6],1280,720,"图像左上角的1/4画面被压缩为640*360大小，静帧并且显示窗口位于左上角,显示图像大小为640*360"},
	{10,&s_aWinRect_SnapShot_HD[0],&s_aWinRect_SnapShot_HD[7],1280,720,"图像左上角的1/4画面被移到右下角显示，静帧并且显示大小仍然是1/4画面"},
	{11,&s_aWinRect_SnapShot_HD[0],&s_aWinRect_SnapShot_HD[7],71,37,"缓存空间不足"},
};

/*视频流的16:9 和4:3信息枚举类型*/
typedef enum _CSTC_STREAM_AspectRatio_Type_E
{
	EM_STREAM_ASPECTRATIO_4_3,
	EM_STREAM_ASPECTRATIO_16_9
	
}CSTC_STREAM_AspectRatio_Type_E;

/*画面比和适配模式测试用例数据结构*/
typedef struct _CSTC_AspectRatio_MatchMethod_S
{
	int                                                    m_nTestID;
	CSTC_STREAM_AspectRatio_Type_E 	m_eStreamType;
	CSUDIVIDEOMatchMethod_E    		m_eMatchMode;	
	CSUDIVIDEOAspectRatioType_E 		m_eAspectRatio;
	CSUDISCREENType_E   		    	m_eScreenType;
	char 					  		*m_pcDes;
}CSTC_AspectRatio_MatchMethod_S;

static CSTC_AspectRatio_MatchMethod_S s_aAspectRatioMAtchMethod_Table[]=
{
	{0,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏不变形"},		
	{1,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"画面满屏显示，左右方向被拉伸"},
	{2,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"画面左右方向被压窄，左右有黑边"},
	{3,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"画面比例不变形，左右有黑边"},		
	{4,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"画面比例不变形，上下有黑边"},		
	{5,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"画面上下被压扁，上下有黑边"},
	{6,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏变形，图像上下方向被拉伸"},
	{7,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"满屏不变形"},	
	{8,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏不变形"},
	{9,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"画面满屏显示，左右方向被拉伸"},
	{10,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"图像上下方向被拉伸，上下有内容被截掉"},
	{11,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"满屏不变形，上下方向有内容被截掉"},
	{12,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏不变形，左右方向有内容被截掉"},		
	{13,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"画面左右被拉伸，左右两边有内容被截掉"},
	{14,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏变形，图像上下方向被拉伸"},
	{15,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"满屏不变形"},
	{16,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏不变形"},
	{17,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"画面满屏显示，左右方向被拉伸"},
	{18,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"画面左右方向被压窄，左右有黑边，上下有内容被截掉"},
	{19,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		" 画面比例不变形，左右有黑边，上下有内容被截掉"},
	{20,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"  画面比例不变形，上下有黑边，左右有内容被截掉"},		
	{21,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"  画面左右方向被拉伸，上下有黑边，左右有内容被截掉"},
	{22,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏变形，图像上下方向被拉伸"},
	{23,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"满屏不变形"},
	{24,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏不变形"},
	{25,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"满屏变形，画面左右方向被拉伸"},
	{26,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏不变形"},
	{27,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"满屏变形，画面左右方向被拉伸"},
	{28,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏变形，图像上下方向被拉伸"},		
	{29,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"满屏不变形"},
	{30,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"满屏变形，图像上下方向被拉伸"},
	{31,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"满屏不变形"},
};

//@CASEGROUP:CSUDIVIDEOGetCount(int * pnVideoDecCount)
//@DESCRIPTION: 测试函数CSUDIVIDEOGetCount在pnVideoDecCount参数正确情况下的执行结果
//@PRECONDITION:
//@INPUT:pnVideoDecCount=整型指针
//@EXPECTATION:能正确获取底层驱动存在的Video解码器资源数目，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、通过CSUDIVIDEOGetCount获取Video资源数目，判断是否返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、比较获取到的video数目,期望应该大于0
//@EXECUTIONFLOW: 3、通过config文件获取Video资源数目
//@EXECUTIONFLOW: 4、比较两次获取的Video资源数目是否相等。如果不等，则测试失败
CSUDI_BOOL CSTC_VIDEO_IT_GetCount_0001(void)
{
	int nVideoCount = 0;
	char acSection[] = "VIDEO";
	char acKey[] = "VIDEO_NUMBER";
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	CSTK_ASSERT_TRUE_FATAL(nVideoCount > 0,"video count should more than zero\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE== CSTC_VIDEO_CheckFromVideoToConfig(acSection, acKey, nVideoCount)), 
						"与Config 文件比较失败!");
	
	bResult = CSUDI_TRUE;
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetCount(int * pnVideoDecCount) 
//@DESCRIPTION: 测试CSUDIVIDEOGetCount在pnVideoDecCount参数为空情况下的执行结果
//@PRECONDITION:
//@INPUT:1、pnVideoDecCount=CSUDI_NULL
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、pnVideoDecCount为CSUDI_NULL,调用CSUDIVIDEOGetCount函数
//@EXECUTIONFLOW: 2、判断函数返回是否为非CSUDI_SUCCESS
//@REMARK:pnVideoDecCount为CSUDI_NULL的情况下，系统不能出现死机，底层能正确分辨参数错误
CSUDI_BOOL CSTC_VIDEO_IT_GetCount_0002(void)
{
	int *pnVideoCount = CSUDI_NULL;
	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOGetCount(pnVideoCount)), "获取Video Decoder设备的总数参数判读错误");
	 
	bResult = CSUDI_TRUE;
	CSTK_FATAL_POINT;
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetCapability(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo)
//@DESCRIPTION: 测试CSUDIVIDEOGetCapability在所有参数均正确的情况下，函数执行的结果，并对返回的能力进行正确性检查
//@PRECONDITION:Video设备的数目不为0
//@INPUT:1、nVideoIndex = [0...nVideo-1]
//@INPUT:2、psCapabilityInfo = CSUDIVIDEOCapability_S结构指针
//@EXPECTATION: 返回CSUDI_SUCCESS
//@EXPECTATION: 获取指定Video的能力,并且该Video的能力能通过合法性检查
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo.
//@EXECUTIONFLOW: 2、获取[0..nVideo-1]内各Video的能力
//@EXECUTIONFLOW: 3、通过config文件，获取对应Video的能力
//@EXECUTIONFLOW: 4、比较2和3所获取的能力，看这两次得到的能力是否相等
//@EXECUTIONFLOW: 5、比较获取的能力中是否有重复的能力项,期望没有
//@REMARK:Video能力请参考CSUDIVIDEOCapability_S
//@REMARK:Video能力数组中,详细描述所支持的视频码流类型,EM_UDI_VID_STREAM_INVALID类型后的内容应被忽略。
CSUDI_BOOL CSTC_VIDEO_IT_GetCapability_0001(void)
{
	int nVideoCount = 0;
	int nVideoIndex =0;
	char acSection[] = "VIDEO";
	int nBase = 0;
	int nIsAble = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		CSUDIVIDEOCapability_S sCapabilityInfo;
		int  nStreamTypeIndex =0;
		int i=0,j=0;

		memset(&sCapabilityInfo, 0, sizeof(CSUDIVIDEOCapability_S));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCapability(nVideoIndex, &sCapabilityInfo), "获取Video能力失败!");
		for (nStreamTypeIndex =0;nStreamTypeIndex < (EM_UDI_VID_STREAMTYPE_NUM -1); nStreamTypeIndex ++)
		{	
			char ackey[255] ={0};
			char acResult[255] = {0};
			int nResultSize = sizeof(acResult);
			
			sprintf(ackey, "VIDEO_%d_STREAM_%s", nVideoIndex, s_asStreamType[nStreamTypeIndex].m_pcSteamType);
			
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(acSection, ackey, acResult, nResultSize), "获取Config文件信息失败!");
			
			nBase = CSTC_VIDEO_CheckStr(acResult);
			
			CSTK_ASSERT_TRUE_FATAL((-1 != nBase), "无法获得Config 信息所对应的进制(16/10/8)");
			
			nIsAble = CSTKGetIntFromStr(acResult, nBase);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_VIDEO_CheckFromConfigToVideo(&sCapabilityInfo, s_asStreamType[nStreamTypeIndex].m_eSteamType, nIsAble), "获取Video能力与Config信息不完全匹配!");
		}

		for(i=0;i<EM_UDI_VID_STREAMTYPE_NUM;i++)
		{
			BOOL bRepeated = FALSE;
			if (sCapabilityInfo.m_eStreamType[i] == EM_UDI_VID_STREAM_UNKNOWN)
			{
				break;
			}
			for(j=i+1;j<EM_UDI_VID_STREAMTYPE_NUM;j++)
			{
				if (sCapabilityInfo.m_eStreamType[i] == sCapabilityInfo.m_eStreamType[j])
				{
					bRepeated = TRUE;
				}
			}
			CSTK_ASSERT_TRUE_FATAL(!bRepeated,"error:There is repeated Capability option\n");
		}
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetCapability(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo)
//@DESCRIPTION: 测试CSUDIVIDEOGetCapability在nVideoIndex参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@INPUT:1、nVideoIndex={-1,nVideo,nVideo+1}
//@INPUT:2、psCapabilityInfo = CSUDIVIDEOCapability_S结构指针
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取值{-1,nVideo,nVideo+1}，分别调用CSUDIVIDEOGetCapability，判断返回值是否为非CSUDI_SUCCESS
//@REMARK:获取nVideoIndex={-1,nVideo,nVideo+1}的各Video的能力，CSUDIVIDEOGetCapability函数均应返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_VIDEO_IT_GetCapability_0002(void)
{
	int nVideoCount = 0;
	CSUDIVIDEOCapability_S sCapabilityInfo;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	int anVideoErrorIndex[3] = {0};
	int nVideoErrorCount = 0;
	int nVideoErrorIndex = 0;

	memset(&sCapabilityInfo, 0, sizeof(CSUDIVIDEOCapability_S));

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");

	anVideoErrorIndex[0] = -1;
	anVideoErrorIndex[1] = nVideoCount;
	anVideoErrorIndex[2] = nVideoCount + 1;

	nVideoErrorCount = sizeof(anVideoErrorIndex) / sizeof(anVideoErrorIndex[0]);
	
	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOGetCapability(anVideoErrorIndex[nVideoErrorIndex], &sCapabilityInfo)), 
							"获取指定Video Decoder设备的能力信息参数判断错误!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetCapability(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo)
//@DESCRIPTION: 测试CSUDIVIDEOGetCapability在psCapabilityInfo参数为空情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、psCapabilityInfo = CSUDI_NULL
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、获取[0..nVideo]内各Video的能力，判断返回值是否为非CSUDI_SUCCESS
//@REMARK:如果psCapabilityInfo参数为空,则无论其它参数如何，CSUDIVIDEOGetCapability函数均返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_VIDEO_IT_GetCapability_0003(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOGetCapability(nVideoIndex, CSUDI_NULL)), 
							"获取指定Video Decoder设备的能力信息参数判断错误!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE * phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot在nVideoIndex  参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:1、nVideoIndex =  {-1,nVideo,nVideo+1}
//@INPUT:2、psSrcRect = CSUDIWinRect_S结构指针
//@INPUT:3、psDstRect = CSUDIWinRect_S结构指针
//@INPUT:4、psSurface = CSUDIOSGSurfaceInfo_S结构指针
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取{-1,nVideo,nVideo+1}内的值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSnapshot，判断返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0001(void)
{
	int nVideoCount = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hSurface = 0;
	int anVideoErrorIndex[3] = {0};
	int nVideoErrorCount = 0;
	int nVideoErrorIndex = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, s_aSnapShotTest[5].m_nSurfaceWidth,s_aSnapShotTest[5].m_nSurfaceHigh, &hSurface),"创建绘图平面失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	anVideoErrorIndex[0] = -1;
	anVideoErrorIndex[1] = nVideoCount;
	anVideoErrorIndex[2] = nVideoCount + 1;

	nVideoErrorCount = sizeof(anVideoErrorIndex) / sizeof(anVideoErrorIndex[0]);

	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSnapshot(anVideoErrorIndex[nVideoErrorIndex], CSUDI_NULL, CSUDI_NULL, &hSurface)),"检查参数失败!");
	}
	
	bResult = CSUDI_TRUE;
		
	CSTK_FATAL_POINT;

	if (hSurface)
	{
		CSUDIOSGDestroySurface(hSurface);
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot在psSurface  参数为空情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、psSrcRect = CSUDIWinRect_S结构指针
//@INPUT:3、psDstRect = CSUDIWinRect_S结构指针
//@INPUT:4、psSurface = CSUDI_NULL
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、psSurface取空
//@EXECUTIONFLOW: 4、分别调用CSUDIVIDEOSnapshot，判断返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0002(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_Error_Code nSnapshotResult = CSUDI_FAILURE;
	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器!\n");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		nSnapshotResult = CSUDIVIDEOSnapshot(nVideoIndex, CSUDI_NULL, CSUDI_NULL, CSUDI_NULL);
		
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == nSnapshotResult), "错误参数检测失败\n");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}


//判断是否支持指定 的图形格式
static CSUDI_BOOL testOSGFormat(CSUDIOSGPixelFormat_E ePixelFormat)
{	
	char pStr[64] = {0};
	CSUDI_BOOL bResult = CSUDI_FALSE;
	char cBuf[32] = {0};

	switch((int)ePixelFormat)
	{
		case EM_UDIOSG_PIXEL_FORMAT_RGB_565:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_RGB_565", sizeof("CS_OSGUDI2_SURPORT_RGB_565"));
			break;
		}///< 16-bit, no per-pixel alpha 
    		case EM_UDIOSG_PIXEL_FORMAT_ARGB_1555:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_ARGB_1555", sizeof("CS_OSGUDI2_SURPORT_ARGB_1555"));
			break;
		}///<  16-bit 
    		case EM_UDIOSG_PIXEL_FORMAT_RGBA_5551:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_RGBA_5551", sizeof("CS_OSGUDI2_SURPORT_RGBA_5551"));
			break;
		}///<  16-bitse 
    		case EM_UDIOSG_PIXEL_FORMAT_ARGB_4444:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_ARGB_4444", sizeof("CS_OSGUDI2_SURPORT_ARGB_4444"));
			break;
		}///<  16-bit 
    		case EM_UDIOSG_PIXEL_FORMAT_RGBA_4444:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_RGBA_4444", sizeof("CS_OSGUDI2_SURPORT_RGBA_4444"));
			break;
		}///<  16-bit 
   		case EM_UDIOSG_PIXEL_FORMAT_RGB_888:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_RGB_888", sizeof("CS_OSGUDI2_SURPORT_RGB_888"));
			break;
		}///<  24-bit 
    		case EM_UDIOSG_PIXEL_FORMAT_ARGB_8888:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_ARGB_8888", sizeof("CS_OSGUDI2_SURPORT_ARGB_8888"));
			break;
		}///<  32-bit 
    		case EM_UDIOSG_PIXEL_FORMAT_PALETTE8:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_PALETTE8", sizeof("CS_OSGUDI2_SURPORT_PALETTE8"));
			break;
		}///<  8-bit Palette index，目前没有调色板的Surface，调色板的结构预留。
    		case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_444:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_YCBCR888_444", sizeof("CS_OSGUDI2_SURPORT_YCBCR888_444"));
			break;
		}///< Y, Cb, Cr signed components with 8 bit precision in a 4:4:4 sampling model
    		case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_422:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_YCBCR888_422", sizeof("CS_OSGUDI2_SURPORT_YCBCR888_422"));
			break;
		}///< Y, Cb, Cr signed components with 8 bit precision in a 4:2:2 sampling model
		case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_420:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_YCBCR888_420", sizeof("CS_OSGUDI2_SURPORT_YCBCR888_420"));
			break;
		}///< Y, Cb, Cr signed components with 8 bit precision in a 4:2:0 sampling model
    		case EM_UDIOSG_PIXEL_FORMAT_YCBCR888_411:
		{
			memcpy(pStr, "CS_OSGUDI2_SURPORT_YCBCR888_411", sizeof("CS_OSGUDI2_SURPORT_YCBCR888_411"));
			break;
		}///< Y, Cb, Cr signed components with 8 bit precision in a 4:1:1 sampling model
	}
	
	if(CSTKGetConfigInfo("OSG",pStr,cBuf,sizeof(cBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		if (CSTKGetIntFromStr(cBuf,10) == 1)
		{
			bResult =  CSUDI_TRUE;
		}
	}

	return bResult;
}

//在图形层上显示surface
static CSUDI_BOOL showSurface(CSUDI_HANDLE hSurface, int nDisplayIndex, CSTC_SnapShot_S *psSnapShot)
{
	CSUDI_HANDLE hGraphics=0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDIOSGRect_S *psSrcRect, *psDstRect, sSurface;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	
	psDstRect = (CSUDIOSGRect_S *) (psSnapShot->m_sOutputWinRect);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL !=psSnapShot),"参数错误\n");
	
	CSTK_ASSERT_TRUE_FATAL((0 !=hSurface),"参数错误\n");

	CSTK_ASSERT_TRUE_FATAL( ((0 == nDisplayIndex) || (1 == nDisplayIndex)), "参数错误\n");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(nDisplayIndex,&hGraphics)), "获取显存失败\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),"获取原始区域失败\n");

	if (psDstRect == CSUDI_NULL)
	{
		sSurface.m_nX =  0;
		sSurface.m_nY =  0;
		sSurface.m_nHeight = sSurfaceInfo.m_nHeight;
		sSurface.m_nWidth  = sSurfaceInfo.m_nWidth;
		psDstRect = &sSurface;
	}

	psSrcRect = psDstRect;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hGraphics, psDstRect,hSurface, psSrcRect, EM_UDIOSG_OPMODE_COPY)),"在OSG上显示失败!\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"在OSG上显示同步失败");
	
	CSUDIOSThreadSleep(1000);

	CSTCPrint("请确认电视画面是否为%s\n",psSnapShot->m_pcDes);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),  "确认没有静帧!\n");

	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if(hSurface && hGraphics)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurface,CSUDI_NULL,0x00000000),  "抓取的图形在Graphics层显示失败\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hGraphics, NULL, hSurface, NULL, EM_UDIOSG_OPMODE_COPY),"关闭静帧失败\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"关闭静帧同步失败");
	}
	
	return bResult;
}

//测试制定的测试用例
static CSUDI_BOOL testSnapShot(int nTestID)
{	 
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	int nVideoCount = 0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	CSUDISCREENResolution_E eResolution = EM_UDISCREEN_RESOLUTION_INVALID;
	CSUDISCREENResolution_E eSrcResolution = EM_UDISCREEN_RESOLUTION_INVALID;

	if(CSUDI_FALSE == CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		CSUDISCREENType_E eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

		// 1
		if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
		} 
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_RGB_565)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_RGB_565, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
		}
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_RGB_888)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_RGB_888, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
		}
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL((0), "没有合适的OSG图像格式!\n"); 		
		}

		// 2
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!\n");

		CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器!\n");

		// 3-4
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
		
		CSTCPrint("以下检测当前视频分辨率，需要把SD的视频分辨率设置为PAL\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(eScreenDevice, &eSrcResolution), "获取测试前分辨率失败");

		if(eSrcResolution == EM_UDISCREEN_RESOLUTION_PAL)
		{
			CSTCPrint("当前分辨率已经是PAL了\n");
		}
		else
		{
			eResolution = EM_UDISCREEN_RESOLUTION_PAL;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &eResolution, 1), "设置PAL分辨率失败");
			CSTCPrint("改变分辨率为PAL成功\n");
		}

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");


		// 6-8
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSnapshot(0, s_aSnapShotTest[nTestID].m_sInputWinRect, s_aSnapShotTest[nTestID].m_sOutputWinRect, &hSurface), "视频快照设置失败!\n");

		//9--10
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == showSurface(hSurface,0,&s_aSnapShotTest[nTestID]),  "抓取的图形在Graphics层显示失败\n");

		if(eSrcResolution > EM_UDISCREEN_RESOLUTION_INVALID && eSrcResolution < EM_UDISCREEN_RESOLUTION_NUM)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &eSrcResolution, 1), "恢复测试前分辨率失败");
		}

		bResult = CSUDI_TRUE;
	}
	else 
	{
		// 2
		if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, s_aSnapShotTest_HD[nTestID].m_nSurfaceWidth, s_aSnapShotTest_HD[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
		} 
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_RGB_565)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_RGB_565, s_aSnapShotTest_HD[nTestID].m_nSurfaceWidth, s_aSnapShotTest_HD[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
		}
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_RGB_888)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_RGB_888, s_aSnapShotTest_HD[nTestID].m_nSurfaceWidth, s_aSnapShotTest_HD[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
		}
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888, s_aSnapShotTest_HD[nTestID].m_nSurfaceWidth, s_aSnapShotTest_HD[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL((0), "没有合适的OSG图像格式!\n"); 		
		}

		// 2
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!\n");

		CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器!\n");

		// 3-4
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "锁频失败!\n");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

		// 6-8
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSnapshot(0, s_aSnapShotTest_HD[nTestID].m_sInputWinRect, s_aSnapShotTest_HD[nTestID].m_sOutputWinRect, &hSurface), "视频快照设置失败!\n");

		//9--10
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == showSurface(hSurface, 1, &s_aSnapShotTest_HD[nTestID]),  "抓取的图形在Graphics层显示失败\n");

		bResult = CSUDI_TRUE;
	}
	
	CSTK_FATAL_POINT;
	
	if (hSurface)
	{
		CSUDIOSGDestroySurface(hSurface);
	}

	if (hPlayer != CSUDI_NULL)
	{
		// 11
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface)
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot全屏抓取并全屏输出的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@EXPECTATION: 返回CSUDI_SUCCESS，OSG重现效果为:全屏显示，没有压缩和放大现象
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:" 全屏显示，图像没有压缩和放大现象"
//@EXECUTIONFLOW: 9、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0003(void)
{
	return  testSnapShot(0);	
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot全屏抓取，依据缓存空间进行压缩后的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: 返回CSUDI_SUCCESS, OSG重现效果为:全屏图像被压缩为250*180大小，显示窗口位于左上角,显示图像大小为250*180
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:" 全屏图像被压缩为250*180大小，显示窗口位于左上角,显示图像大小为250*180"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0004(void)
{
	return  testSnapShot(1);	
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot全屏抓取，依据输出窗口进行图像压缩后效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect  为指向数据结构{0, 0,250, 180}的指针
//@INPUT:psSurface.m_nWidth =300
//@INPUT:psSurface.m_nHigh =240
//@EXPECTATION: 返回CSUDI_SUCCESS，OSG重现效果为:全屏图像被压缩为250*180大小，显示窗口位于左上角,显示图像大小为250*180
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"全屏图像被压缩为250*180大小，显示窗口位于左上角,显示图像大小为250*180"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0005(void)
{
	return  testSnapShot(2);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot小部分被抓取，原形拷贝(不压缩)的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect 为指向数据结构{0, 0,250, 180}的指针
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =300
//@INPUT:psSurface.m_nHigh =240
//@EXPECTATION: 返回CSUDI_SUCCESS, OSG重现效果为:图像左上角250*180的区域拷贝到缓存空间，显示窗口位于左上角,显示图像大小为300*240
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"图像左上角250*180的区域拷贝到缓存空间，显示窗口位于左上角,显示图像大小为300*240"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0006(void)
{
	return testSnapShot(3);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot中间部分1/9图像被抓取，依据缓存空间进行压缩的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect 为指向数据结构{720/3, 576/3,720/3, 576/3}的指针
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: 返回CSUDI_SUCCESS，OSG重现效果为:图像正中间的1/9画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:" 图像正中间的1/9画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0007(void)
{
	return testSnapShot(4);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot右上角1/4图像被抓取，依据缓存空间进行压缩的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect 为指向数据结构{720/2, 0,720/2, 576/2}的指针
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: 返回CSUDI_SUCCESS，OSG重现效果为:图像右上角的1/4画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"图像右上角的1/4画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0008(void)
{
	return testSnapShot(5);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot左下角1/4图像被抓取，依据缓存空间进行压缩的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect 为指向数据结构{0, 576/2,720/2, 576/2}的指针
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: 返回CSUDI_SUCCESS, OSG重现效果为:图像左下角的1/4画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"图像左下角的1/4画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0009(void)
{
	return testSnapShot(6);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION:  测试CSUDIVIDEOSnapshot右下角1/4图像被抓取，依据缓存空间进行压缩的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect 为指向数据结构{720/2, 576/2,720/2, 576/2}的指针
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: 返回CSUDI_SUCCESS，OSG重现效果为:图像右下角的1/4画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"图像右下角的1/4画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0010(void)
{
	return testSnapShot(7);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION:  测试CSUDIVIDEOSnapshot左上角1/4图像被抓取，依据缓存空间进行压缩的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect 为指向数据结构{0,0,720/2, 576/2}的指针
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: 返回CSUDI_SUCCESS, OSG重现效果为:图像左上角的1/4画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"图像左上角的1/4画面被压缩成250*180大小，显示窗口位于左上角,显示图像大小为250*180"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0011(void)
{
	return testSnapShot(8);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot部分图像被抓取(左上角1/4屏幕)，依据缓存空间进行压缩的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect 为指向数据结构{0,0,720/2, 576/2}的指针
//@INPUT:psDstRect 为指向数据结构{0, 0,250, 180}的指针
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@EXPECTATION: 返回CSUDI_SUCCESS, OSG重现效果为:图像左上角的1/4画面被压缩为250*180大小，显示窗口位于左上角,显示图像大小为250*180
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"图像左上角的1/4画面被压缩为250*180大小，显示窗口位于左上角,显示图像大小为250*180"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0012(void)
{
	return testSnapShot(9);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot部分图像被抓取(左上角1/4屏幕)，拷贝到相同大小的输出空间区域(右下角1/4屏幕)的图形效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect 为指向数据结构{0,0,720/2, 576/2}的指针
//@INPUT:psDstRect 为指向数据结构{720/2, 576/2,720/2, 576/2}的指针
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@EXPECTATION: 返回CSUDI_SUCCESS, OSG重现效果为:图像左上角的1/4画面被移到右下角显示，显示大小仍然是1/4画面
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"图像左上角的1/4画面被移到右下角显示，显示大小仍然是1/4画面"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0013(void)
{
	return testSnapShot(10);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试CSUDIVIDEOSnapshot在缓存空间不足的情况下的执行效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect 为指向数据结构{0,0,720/2, 576/2}的指针
//@INPUT:psDstRect 为指向数据结构{720/2, 576/2,720/2, 576/2}的指针
//@INPUT:psSurface.m_nWidth =40
//@INPUT:psSurface.m_nHigh =30
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSnapshot，判断返回值是否为非CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0014(void)
{
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	int nVideoCount = 0;
	int 	nTestID = 11;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	// 1
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
	   
	// 2
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
	// 3-4
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
    
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

	// 5
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIVIDEOSnapshot(0, s_aSnapShotTest[nTestID].m_sInputWinRect, s_aSnapShotTest[nTestID].m_sOutputWinRect, &hSurface), "测试结果不符合预期!\n");

	// 6
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_FALSE)), "显示视频失败!\n");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	if (hSurface)
	{
		CSUDIOSGDestroySurface(hSurface);
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

static CSUDI_BOOL testSnapShot_Pixel(CSUDIOSGPixelFormat_E ePixelFormat)
{			 
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	int nVideoCount = 0;
	int nTestID = 0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
      
	// 1
	if (testOSGFormat(ePixelFormat))
	{
	       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(ePixelFormat, s_aSnapShotTest[nTestID].m_nSurfaceWidth, s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"创建绘图平面失败!\n");
	}
	else
	{
		return CSUDI_TRUE;
	}

	// 2
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
	   
	// 3-4
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency(&hPlayer)), "锁频失败!\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

	// 6
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSnapshot(0, s_aSnapShotTest[nTestID].m_sInputWinRect, s_aSnapShotTest[nTestID].m_sOutputWinRect, &hSurface), "视频快照设置失败!\n");

	// 7
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_FALSE)), "显示视频失败!\n");

	// 8
	CSTCPrint("请确认电视画面已被成功关闭\n");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),  "视频画面不能成功关闭\n");

	// 9
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == showSurface(hSurface,0,&s_aSnapShotTest[nTestID]),  "抓取的图形在Graphics层显示失败\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;

	if (hSurface)
	{
		CSUDIOSGDestroySurface(hSurface);
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试以EM_UDIOSG_PIXEL_FORMAT_RGB_565 格式执行CSUDIVIDEOSnapshot, 抓取图像
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@INPUT:psSurface.m_ePixelFormat =EM_UDIOSG_PIXEL_FORMAT_RGB_565 
//@EXPECTATION: 返回CSUDI_SUCCESS 
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"全屏显示，图像没有压缩和放大现象"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0015(void)
{
	return testSnapShot_Pixel(EM_UDIOSG_PIXEL_FORMAT_RGB_565);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试以EM_UDIOSG_PIXEL_FORMAT_RGB_888 格式执行CSUDIVIDEOSnapshot, 抓取图像
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@INPUT:psSurface.m_ePixelFormat =EM_UDIOSG_PIXEL_FORMAT_RGB_888
//@EXPECTATION: 返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"全屏显示，图像没有压缩和放大现象"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0016(void)
{
	return testSnapShot_Pixel(EM_UDIOSG_PIXEL_FORMAT_RGB_888);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: 测试以EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 格式执行CSUDIVIDEOSnapshot, 抓取图像
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@INPUT:psSurface.m_ePixelFormat =EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 
//@EXPECTATION: 返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、以psSurface.m_nWidth和psSurface.m_nHigh，创建psSurface
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo-1]内一个值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上正确循环播放标清数据流
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSnapshot，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭4中播放的数据流，关闭视频窗口
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示窗口已被成功关闭
//@EXECUTIONFLOW: 9、利用CSUDIVIDEOSnapshot获取的psSurface，在OSG上显示该psSurface
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认10显示的内容与5显示的内容相比较为:"全屏显示，图像没有压缩和放大现象"
//@EXECUTIONFLOW: 11、销毁psSurface
//@REMARK:该测试用例前端播放的码流为标清PAL制码流，720*576认为是全屏
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0017(void)
{
	return testSnapShot_Pixel(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888);
}

static BOOL TestSnapShotDisplaySurface(BOOL bSD)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL; 
	CSUDI_HANDLE hOSGDisplay = CSUDI_NULL;
	CSUDIWinRect_S stRectVideo;
	CSUDIOSGSurfaceInfo_S stInfo;

	/*如果是标清平台,直接返回测试成功*/
	if ((!bSD) && (!bIsSupportHD()))
	{
		return TRUE;
	}
		
	if (bSD)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency(&hPlayer)), "锁频失败!\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency(&hPlayer)), "锁频失败!\n");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(bSD?0:1,&hOSGDisplay),
		"fail to get display surface by CSUDIOSGGetDisplaySurface\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hOSGDisplay,&stInfo),
		"fail to get display surface information by CSUDIOSGGetSurfaceInfo\n");
	stRectVideo.m_nX = 0;
	stRectVideo.m_nY = 0;
	stRectVideo.m_nWidth = stInfo.m_nWidth /2;
	stRectVideo.m_nHeight = stInfo.m_nHeight/2;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSnapshot(0, NULL, &stRectVideo, &hOSGDisplay),
		"fail to snapshot video picture to display surface\n");
	
	CSTCPrint("请确认%s输出上,左上角1/4屏图像为抓出的视频全屏图片(Y/N)\n",
		bSD?"标清":"高清");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"确认快照到显存上的效果失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0, EM_UDIVIDEO_STOPMODE_FREEZE),
		"fail to set stop mode\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),
		"fail to stop player\n");
	
	CSTCPrint("请确认视频节目已经停止,画面静帧(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"确认效果失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSnapshot(0, NULL, &stRectVideo, &hOSGDisplay),
		"fail to snapshot video picture to display surface\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0,FALSE),
		"fail to hide video show\n");
	
	CSTCPrint("请确认%s输出上屏幕左上角1/4变成视频最后一帧图像,其余区域为黑色(Y/N)\n",
		bSD?"标清":"高清");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"确认效果失败\n");
		
	CSTK_FATAL_POINT

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	if (hOSGDisplay)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSGFill(hOSGDisplay,NULL,0x0),
			"fail to fill display surface to transparence\n");
	}

	return TRUE;
}

//@CASEGROUP:CSUDIVIDEOSnapshot
//@DESCRIPTION: 测试标清节目源的抓图,以及停止播放器后的抓图,以及直接抓图到现存的功能
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:psSurface = 标清显存
//@EXPECTATION:1. 节目stop后还能将最后的图像抓出来
//@EXPECTATION:2. 可以直接将图像抓到显存上
//@EXECUTIONFLOW: 1、播放一个标清测试码流,期望播放成功有图像显示
//@EXECUTIONFLOW: 2、调用CSUDIOSGGetDisplaySurface获取标清显存，期望返回成功
//@EXECUTIONFLOW: 3、调用CSUDIOSGGetSurfaceInfo获取显存信息，期望返回成功
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOSnapshot抓取视频全屏图像到显存的左上角1/4处
//@EXECUTIONFLOW: 5、确认输出效果
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetStopMode设置成静帧模式
//@EXECUTIONFLOW: 7、调用CSUDIPLAYERStop停止播放,期望视频停止,video画面静帧
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOSnapshot再次抓取全屏视频图像到显存左上角1/4处
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow关闭视频显示,期望返回成功
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERClose关闭播放器
//@EXECUTIONFLOW: 11、调用CSUDIOSGFill将OSG显存填充成0x0恢复测试现场
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0018(void)
{
	return TestSnapShotDisplaySurface(TRUE);
}

//@CASEGROUP:CSUDIVIDEOSnapshot
//@DESCRIPTION: 测试高清1080I节目源的抓图,以及停止播放器后的抓图,以及直接抓图到现存的功能
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放标清数据流
//@INPUT:psSurface = 标清显存
//@EXPECTATION:1. 节目stop后还能将最后的图像抓出来
//@EXPECTATION:2. 可以直接将图像抓到显存上
//@EXECUTIONFLOW: 1、播放一个高清测试码流,期望播放成功有图像显示
//@EXECUTIONFLOW: 2、调用CSUDIOSGGetDisplaySurface获取标清显存，期望返回成功
//@EXECUTIONFLOW: 3、调用CSUDIOSGGetSurfaceInfo获取显存信息，期望返回成功
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOSnapshot抓取视频全屏图像到显存的左上角1/4处
//@EXECUTIONFLOW: 5、确认输出效果
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetStopMode设置成静帧模式
//@EXECUTIONFLOW: 7、调用CSUDIPLAYERStop停止播放,期望视频停止,video画面静帧
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOSnapshot再次抓取全屏视频图像到显存左上角1/4处
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow关闭视频显示,期望返回成功
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERClose关闭播放器
//@EXECUTIONFLOW: 11、调用CSUDIOSGFill将OSG显存填充成0x0恢复测试现场

CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0019(void)
{
	return TestSnapShotDisplaySurface(FALSE);
}

static CSUDI_BOOL testGetStreamStatus(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDIVIDEOStreamStatus_S sStreamStatus_1;
	CSUDIVIDEOStreamStatus_S sStreamStatus_2;	
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	// 1-3
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n"); 

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video数量失败!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	// 4
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

	// 5-6
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStreamStatus(nVideoIndex, &sStreamStatus_1),"第一次获取解码器信息失败!");

	// 7 
	CSUDIOSThreadSleep(1000);

	// 8
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStreamStatus(nVideoIndex, &sStreamStatus_2),  "第二次获取解码器信息失败!");

	// 9
	if ((sStreamStatus_1.m_bInterlaced != sStreamStatus_2.m_bInterlaced)
		||(sStreamStatus_1.m_dwFrameCount >= sStreamStatus_2.m_dwFrameCount)
		||(sStreamStatus_1.m_eVidStreamType != sStreamStatus_2.m_eVidStreamType)
		||(sStreamStatus_1.m_nFrameRate != sStreamStatus_2.m_nFrameRate)
		||(sStreamStatus_1.m_nSourceHight != sStreamStatus_2.m_nSourceHight)
		||(sStreamStatus_1.m_nSourceWidth != sStreamStatus_2.m_nSourceWidth))
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "两次获得解码器信息不一致");
	}	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放器失败!");

	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus)
//@DESCRIPTION: 测试CSUDIVIDEOGetStreamStatus在所有参数均正确的情况下，函数执行的结果，并对返回的能力进行正确性检查
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放视频数据流
//@INPUT:1、nVideoIndex = [0..nVideo-1]
//@INPUT:2、psStreamStatus = CSUDIVIDEOStreamStatus_S结构指针
//@EXPECTATION: 返回CSUDI_SUCCESS
//@EXPECTATION: 能正确得到指定Video解码器的状态，在播放时，视频帧数目在不断变化
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo.
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放一个视频流
//@EXECUTIONFLOW: 4、调用CSTKWaitYesKey，让用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOGetStreamStatus获取该Video解码器的状态，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、对返回的解码器状态结构进行合理性检查
//@EXECUTIONFLOW: 7、Sleep 1000ms
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetStreamStatus获取该Video解码器的状态，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、比较两次状态的变化
//@REMARK:1、Video状态合理性检查请参考CSUDIVIDEOStreamStatus_S的定义
//@REMARK:2、视频解码器在关闭的情况下，也应该能得到状态信息
//@REMARK:2、在播放的时候，视频帧数目应该不断增加
CSUDI_BOOL CSTC_VIDEO_IT_GetStreamStatus_0001(void)
{
	return testGetStreamStatus();
}

//@CASEGROUP:CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus)
//@DESCRIPTION: 测试CSUDIVIDEOGetCapability在nVideoIndex参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放视频数据流
//@INPUT:1、nVideoIndex={-1,nVideo,nVideo+1}
//@INPUT:2、psStreamStatus = CSUDIVIDEOStreamStatus_S结构指针
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取值{-1,nVideo,nVideo+1}，分别调用CSUDIVIDEOGetStreamStatus，判断返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_GetStreamStatus_0002(void)
{
	int nVideoCount = 0;
	
	CSUDIVIDEOStreamStatus_S sStreamStatus;
	
	CSUDI_BOOL bResult = CSUDI_FALSE;

	int anVideoErrorIndex[3] = {0};
	
	int nVideoErrorCount = 0;
	
	int nVideoErrorIndex = 0;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");

	anVideoErrorIndex[0] = -1;
	anVideoErrorIndex[1] = nVideoCount;
	anVideoErrorIndex[2] = nVideoCount + 1;

	nVideoErrorCount = sizeof(anVideoErrorIndex) / sizeof(anVideoErrorIndex[0]);
	
	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex ++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOGetStreamStatus(anVideoErrorIndex[nVideoErrorIndex], &sStreamStatus)),
						"获取解码器播放状态信息 参数判断错误!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}


//@CASEGROUP:CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus)
//@DESCRIPTION: 测试CSUDIVIDEOGetStreamStatus在psStreamStatus参数为空情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确循环播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、psStreamStatus = CSUDI_NULL
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内各值，分别调用CSUDIVIDEOGetStreamStatus，判断返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_GetStreamStatus_0003(void)
{
	int nVideoCount = 0;
	
	int nVideoIndex = 0;
	
	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数参数判读错误");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOGetStreamStatus(nVideoIndex, CSUDI_NULL)),
						"获取解码器播放状态信息 参数判断错误!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}


//@CASEGROUP:CSUDIVIDEOShow(int nVideoIndex, CSUDI_BOOL bIsShow)
//@DESCRIPTION: 测试CSUDIVIDEOShow在所有参数均正确情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放显示视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、bIsShow=CSUDI_TRUE,CSUDI_FALSE
//@EXPECTATION: 返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上正确播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSTKWaitYes()，等待用户确认视频成功播放并显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_FALSE)，关闭视频，判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYes()，等待用户确认视频已被成功关闭
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_Show_0001(void)
{
	int nVideoCount = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	// 1-2
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video数量失败!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	// 3
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
	// 4
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

	// 5
	CSTCPrint("请将标清输出端口连接电视并确定视频正确播放出来!\n");
	   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "无法获得按键确认!");
      
	// 6
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_FALSE)), "显示视频失败!\n");

	CSTCPrint("请将标清输出端口连接电视并确定视频是否被关闭!\n");
	// 7
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "无法获得按键确认!");

	// 10
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放器失败!");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOShow(int nVideoIndex, CSUDI_BOOL bIsShow)
//@DESCRIPTION: 测试CSUDIVIDEOShow在参数nVideoIndex错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@INPUT:1、nVideoIndex = {-1,nVideo,nVideo+1}
//@INPUT:2、bIsShow=CSUDI_TRUE,SUDI_FALSE
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、nVideoIndex取{-1,nVideo,nVideo+1}内各值
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOShow，判断返回值是否为非CSUDI_SUCCESS
//@REMARK:在nVideoIndex取{-1,nVideo,nVideo+1}值时，bIsShow不论取什么值，都返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_VIDEO_IT_Show_0002(void)
{
	static int nVideoCount = 0;
	int nErrorParmCount = 3 * 2;
	int nErrorParmIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	typedef struct 
	{
		int m_nVideoIndex;
		int m_bIsShow;	
	}CSTC_ErrorShow_S;
	
	CSTC_ErrorShow_S s_ErrorShowTpye[6];

	memset(s_ErrorShowTpye, 0, sizeof(s_ErrorShowTpye));

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");

	s_ErrorShowTpye[0].m_nVideoIndex = -1;
	s_ErrorShowTpye[0].m_bIsShow = CSUDI_TRUE;

	s_ErrorShowTpye[1].m_nVideoIndex = -1;
	s_ErrorShowTpye[1].m_bIsShow = CSUDI_FALSE;

	s_ErrorShowTpye[2].m_nVideoIndex = nVideoCount;
	s_ErrorShowTpye[2].m_bIsShow = CSUDI_TRUE;

	s_ErrorShowTpye[3].m_nVideoIndex = nVideoCount;
	s_ErrorShowTpye[3].m_bIsShow = CSUDI_FALSE;

	s_ErrorShowTpye[4].m_nVideoIndex = nVideoCount + 1;
	s_ErrorShowTpye[4].m_bIsShow = CSUDI_TRUE;

	s_ErrorShowTpye[5].m_nVideoIndex = nVideoCount + 1;
	s_ErrorShowTpye[5].m_bIsShow = CSUDI_FALSE;
	
	for (nErrorParmIndex = 0; nErrorParmIndex < nErrorParmCount; nErrorParmIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOShow(s_ErrorShowTpye[nErrorParmIndex].m_nVideoIndex, s_ErrorShowTpye[nErrorParmIndex].m_bIsShow)), 
							"显示/隐藏视频解码器的输出参数判断失败!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOShow CSUDISCREENSetResolution 
//@DESCRIPTION: 测试分辨率的切换不回影响video show状态
//@PRECONDITION:Video设备的数目不为0
//@INPUT:1、nVideoIndex = 0
//@EXPECTATION: 分辨率切换不影响video显示状态
//@EXECUTIONFLOW: 1、播放标清测试码流,期望播放成功
//@EXECUTIONFLOW: 2、如果有高清调用CSUDISCREENSetResolution将高清分辨率设置成1080I_50HZ
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOShow关闭video显示,期望返回成功
//@EXECUTIONFLOW: 4、调用CSUDISCREENSetResolution将标清输出设置成NTSC,高清输出显示设置成720P_50HZ
//@EXECUTIONFLOW:期望分辨率的设置不影响视频显示状态
//@EXECUTIONFLOW: 5、调用CSUDISCREENSetResolution将标清输出设置成PAL,高清输出显示设置成1080I_50HZ
//@EXECUTIONFLOW:期望分辨率的设置不影响视频显示状态
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOShow打开video显示,期望返回成功
//@EXECUTIONFLOW: 7、重复步骤4~5
//@EXECUTIONFLOW: 8、调用CSUDIPLAYERClose关闭测试播放器
CSUDI_BOOL CSTC_VIDEO_IT_Show_0003(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E enDev;
	CSUDISCREENResolution_E enRes;
	int i=0;
	BOOL bHDSupport = bIsSupportHD();
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");

	if (bHDSupport)
	{
		enDev = EM_UDI_VOUT_DEVICE_HD;
		enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev,&enRes,1),
			"fail to set resolution\n");
	}
	for(i=0;i<2;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0,i?TRUE:FALSE),"fail to hide video\n");
		CSTCPrint("高标清输出视频都%s(Y/N)\n",i?"有图像显示":"黑屏");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

		enRes=EM_UDISCREEN_RESOLUTION_NTSC;
		enDev = EM_UDI_VOUT_DEVICE_SD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
			"fail to set resolution\n");

		if (bHDSupport)
		{
			enRes=EM_UDISCREEN_RESOLUTION_720P_50HZ;
			enDev = EM_UDI_VOUT_DEVICE_HD;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
				"fail to set resolution\n");
		}
		
		CSTCPrint("标清分辨率设置成NTSC,高清设置成720P_50HZ(如果有),高标清输出依旧%s(Y/N)\n",
			i?"显示":"黑屏");
		if (i)
		{
			CSTCPrint("允许短暂的黑屏或者闪烁\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

		enRes=EM_UDISCREEN_RESOLUTION_PAL;
		enDev = EM_UDI_VOUT_DEVICE_SD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
			"fail to set resolution\n");

		if (bHDSupport)
		{
			enRes=EM_UDISCREEN_RESOLUTION_1080I_50HZ;
			enDev = EM_UDI_VOUT_DEVICE_HD;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
				"fail to set resolution\n");
		}
		CSTCPrint("标清分辨率设置成PAL,高清设置成1080I_50HZ(如果有),高标清输出依旧%s(Y/N)\n",
			i?"显示":"黑屏");
		if (i)
		{
			CSTCPrint("允许短暂的黑屏或者闪烁\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return TRUE;
}


/*画面比和适配模式测试用例数据组合
|---------------------------------------------------------------------------------|      	
| 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述                            |
|---------------------------------------------------------------------------------|
|  4:3   | LETTER_BOX|  4:3    | 标清   | 满屏不变形                              |
|  4:3   | LETTER_BOX|  4:3    | 高清   | 画面不变形，左右有黑边                  |
|---------------------------------------------------------------------------------|
|  4:3   | LETTER_BOX| 16:9    | 标清   | 满屏不变形                              |
|  4:3   | LETTER_BOX| 16:9    | 高清   | 满屏变形，画面左右方向被拉伸            |
|---------------------------------------------------------------------------------|
| 16:9   | LETTER_BOX|  4:3    | 标清   | 满屏变形，上下方向被拉伸                |
| 16:9   | LETTER_BOX|  4:3    | 高清   | 满屏不变形                              |
|---------------------------------------------------------------------------------|
| 16:9   | LETTER_BOX| 16:9    | 标清   | 画面不变形，上下有黑边                  |
| 16:9   | LETTER_BOX| 16:9    | 高清   | 满屏不变形                              |
|---------------------------------------------------------------------------------|
|  4:3   | PAN_SCAN  |  4:3    | 标清   | 满屏不变形                              |
|  4:3   | PAN_SCAN  |  4:3    | 高清   | 满屏不变形，上下方向有内容被截掉        |
|---------------------------------------------------------------------------------|
|  4:3   | PAN_SCAN  | 16:9    | 标清   | 满屏不变形                              |
|  4:3   | PAN_SCAN  | 16:9    | 高清   | 满屏不变形，上下方向有内容被截掉        |
|---------------------------------------------------------------------------------|
| 16:9   | PAN_SCAN  |  4:3    | 标清   | 满屏不变形，左右方向有内容被截掉        |
| 16:9   | PAN_SCAN  |  4:3    | 高清   | 满屏不变形                              |
|---------------------------------------------------------------------------------|
| 16:9   | PAN_SCAN  | 16:9    | 标清   | 满屏不变形，左右方向有内容被截掉        |
| 16:9   | PAN_SCAN  | 16:9    | 高清   | 满屏不变形                              |
|---------------------------------------------------------------------------------|
|  4:3   | COMBINED  |  4:3    | 标清   | 满屏不变形                              |
|  4:3   | COMBINED  |  4:3    | 高清   | 画面不变形，边缘有内容被截掉            |
|---------------------------------------------------------------------------------|
|  4:3   | COMBINED  | 16:9    | 标清   | 满屏不变形                              |
|  4:3   | COMBINED  | 16:9    | 高清   | 画面不变形，边缘有内容被截掉            |
|---------------------------------------------------------------------------------|
| 16:9   | COMBINED  |  4:3    | 标清   | 画面不变形，边缘有内容被截掉            |
| 16:9   | COMBINED  |  4:3    | 高清   | 满屏不变形                              |
|---------------------------------------------------------------------------------|
| 16:9   | COMBINED  | 16:9    | 标清   | 画面不变形，边缘有内容被截掉            |
| 16:9   | COMBINED  | 16:9    | 高清   | 满屏不变形                              |
|---------------------------------------------------------------------------------|
|  4:3   | IGNORE    |  4:3    | 标清   | 满屏不变形                              |
|  4:3   | IGNORE    |  4:3    | 高清   | 满屏变形，画面左右方向被拉伸            |
|---------------------------------------------------------------------------------|
|  4:3   | IGNORE    | 16:9    | 标清   | 满屏不变形                              |
|  4:3   | IGNORE    | 16:9    | 高清   | 满屏变形，画面左右方向被拉伸            |
|---------------------------------------------------------------------------------|
| 16:9   | IGNORE    |  4:3    | 标清   | 满屏变形，图像上下方向被拉伸            |
| 16:9   | IGNORE    |  4:3    | 高清   | 满屏不变形                              |
|---------------------------------------------------------------------------------|
| 16:9   | IGNORE    | 16:9    | 标清   | 满屏变形，画面上下方向被拉伸            |
| 16:9   | IGNORE    | 16:9    | 高清   | 满屏不变形                              |
|---------------------------------------------------------------------------------|

*/
static CSUDI_BOOL Stream16Ratio9LockFrequency(CSUDI_HANDLE *phPlayer)
{
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[2];
	CSUDI_BOOL bResult = CSUDI_FALSE; 
	int nStreamCnt =2;
	int nTunerIndex =0;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

//////////////////////设置节目信息
	sStreamInfo[0].m_nPid=5137;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	sStreamInfo[1].m_nPid=5138;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;

	sPlayerChnl.m_nDemux=0;
	sPlayerChnl.m_nAudioDecoder=0;
	sPlayerChnl.m_nVideoDecoder=0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex,"Audio&Video Test_27Mbps_20070524.ts"), "锁频失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,phPlayer)), "打开播放器失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != phPlayer, "创建播放器句柄失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (*phPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "设置解码信息失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(*phPlayer), "开始播放失败!");
	CSUDIOSThreadSleep(1500);
	CSTCPrint("音视频是否能够正常播放？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"播放不正常");
	bResult = CSUDI_TRUE;
       
	CSTK_FATAL_POINT;

	g_bAssertBeforeFatalPoint = CSUDI_TRUE;

	return bResult;
} 

static CSUDI_BOOL testMatchMethodAspectRatio(int nTestID)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIVIDEOAspectRatioType_E 	eAspectRatio = -1;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = -1;

	// 获得videoID
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!\n");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
	   
	// 锁频并播放
	if(s_aAspectRatioMAtchMethod_Table[nTestID].m_eStreamType == EM_STREAM_ASPECTRATIO_4_3)
	{ 
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == Stream16Ratio9LockFrequency( &hPlayer)), "锁频失败!\n");
	}
    
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

	if (s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType == EM_UDI_VOUT_DEVICE_SD)
	{
       	CSTCPrint("请将标清输出端口连接标清电视(4:3)并确定标清视频正确播放出来!\n");
	}
	else
	{
		CSTCPrint("请将高清输出端口连接高清电视机(16:9)并确定高清视频正确播放出来!\n");
	}
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "无法获得按键确认");

	// 设置MatchMethod
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eMatchMode)), "SetMatchMode设置失败");
	// 检测CSUDIVIDEOGetMatchMethod
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, &eMatchMethod)), "GetMatchMode调用失败");
	CSTK_ASSERT_TRUE_FATAL((s_aAspectRatioMAtchMethod_Table[nTestID].m_eMatchMode == eMatchMethod), "获得的eMatchMethod和设置的eMatchMethod不一致");

	// 设置AspectRatio
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eAspectRatio)), "SetAspectRatio设置失败");
	// 检测CSUDIVIDEOGetAspectRatio
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, &eAspectRatio)), "GetAspectRatio调用失败");
	CSTK_ASSERT_TRUE_FATAL((s_aAspectRatioMAtchMethod_Table[nTestID].m_eAspectRatio == eAspectRatio), "获得的eAspectRatio和设置的eAspectRatio不一致");

	CSTCPrint(" 视频源 |  适应模式  | 画面比 | 输出设备 \n");

    if(s_aAspectRatioMAtchMethod_Table[nTestID].m_eStreamType == EM_STREAM_ASPECTRATIO_4_3)
    {
	    CSTCPrint("  4 : 3 |");
    }
    else 
	{		
        CSTCPrint(" 16 : 9 |");
   	}
	switch(s_aAspectRatioMAtchMethod_Table[nTestID].m_eMatchMode)
    {
    	case EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX :
         	CSTCPrint(" LETTER_BOX | ");
            break;
		case EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN :
          	CSTCPrint(" PAN_SCAN | ");
            break;
        case EM_UDIVIDEO_MATCH_METHOD_COMBINED :
			CSTCPrint(" COMBINED | ");
            break;
        case EM_UDIVIDEO_MATCH_METHOD_IGNORE :
            CSTCPrint(" IGNORE | ");
            break;
        default :
            break;
    }
    if(s_aAspectRatioMAtchMethod_Table[nTestID].m_eAspectRatio == EM_UDIVIDEO_ASPECT_RATIO_4_3)
    {
	    CSTCPrint(" 4 : 3 | ");
    }
    else 
	{		
        CSTCPrint("16 : 9 | ");
   	}
    
 	if (s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType == EM_UDI_VOUT_DEVICE_SD)
	{
       	CSTCPrint("标 清 \n");
	}
	else
	{
		CSTCPrint("高 清 \n");
	}

    CSTCPrint("注意:要关注视频输出效果,而不是变化过程!\n");

	CSTCPrint("视频输出效果是否为:\"%s\"!\n", s_aAspectRatioMAtchMethod_Table[nTestID].m_pcDes);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "无法获得按键确认");

	// 让硬件自己根据码流处理
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, 
						EM_UDIVIDEO_MATCH_METHOD_IGNORE)), "SetMatchMode设置失败");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, 
						EM_UDIVIDEO_ASPECT_RATIO_AUTO)), "SetAspectRatio设置失败");

	// 检测CSUDIVIDEOGetAspectRatio,CSUDIVIDEOGetMatchMethod
	eAspectRatio = -1;
	eMatchMethod = -1;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, &eMatchMethod)), "GetMatchMode调用失败");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_IGNORE == eMatchMethod), "获得的eMatchMethod和设置的eMatchMethod不一致");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, &eAspectRatio)), "GetAspectRatio调用失败");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_AUTO == eAspectRatio), "获得的eAspectRatio和设置的eAspectRatio不一致");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放器失败!");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 标清输出通道上设置成LETTER_BOX的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"满屏不变形"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接标清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且满屏不变形
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | 标  清 | 满屏不变形        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0001(void)
{
	int nTestID = 0;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 高清输出通道上设置成LETTER_BOX的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"画面满屏显示，左右方向被拉伸 "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | 高  清 | 画面满屏显示，左右方向被拉伸 |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0002(void)
{
	int nTestID = 1;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 标清输出通道上设置成LETTER_BOX的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"画面左右方向被压窄，左右有黑边"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | 标  清 | 画面左右方向被压窄，左右有黑边  |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0003(void)
{
	int nTestID = 2;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 高清输出通道上设置成LETTER_BOX的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"画面显示正常，左右有黑边"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | 高  清 | 画面显示正常，左右有黑边           |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0004(void)
{
	int nTestID = 3;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 标清输出通道上设置成LETTER_BOX的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为" 画面显示正常，上下有黑边 "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | 标  清 |  画面显示正常，上下有黑边|
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0005(void)
{
	int nTestID = 4;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 高清输出通道上设置成LETTER_BOX的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"画面左右被拉伸，上下有黑边"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | 高  清 | 画面上下被压扁，上下有黑边     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0006(void)
{
	int nTestID = 5;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 标清输出通道上设置成LETTER_BOX的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏变形，图像上下方向被拉伸"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接标清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | 标  清 | 满屏变形，图像上下方向被拉伸        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0007(void)
{
	int nTestID = 6;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 高清输出通道上设置成LETTER_BOX的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏不变形"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | 高  清 | 满屏不变形            |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0008(void)
{
	int nTestID = 7;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 标清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"满屏不变形"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |PAN_SCAN |  4 : 3  | 标  清 | 满屏不变形        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0009(void)
{
	int nTestID = 8;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 高清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"画面满屏显示，左右方向被拉伸      "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |PAN_SCAN |  4 : 3  | 高  清 |画面满屏显示，左右方向被拉伸
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0010(void)
{
	int nTestID = 9;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 标清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为" 图像上下方向被拉伸，上下有内容被截掉 "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |PAN_SCAN | 16 : 9  | 标  清 | 图像上下方向被拉伸，上下有内容被截掉     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0011(void)
{
	int nTestID = 10;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 高清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏不变形，上下方向有内容被截掉 "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |PAN_SCAN | 16 : 9  | 高  清 | 满屏不变形，上下方向有内容被截掉   |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0012(void)
{
	int nTestID = 11;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 标清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"满屏不变形，左右方向有内容被截掉"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |PAN_SCAN |  4 : 3  | 标  清 |  满屏不变形，左右方向有内容被截掉 |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0013(void)
{
	int nTestID = 12;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 高清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"画面左右被拉伸，左右两边有内容被截掉"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |PAN_SCAN |  4 : 3  | 高  清 |画面左右被拉伸，左右两边有内容被截掉     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0014(void)
{
	int nTestID = 13;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 标清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏变形，图像上下方向被拉伸"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |PAN_SCAN | 16 : 9  | 标  清 |  满屏变形，图像上下方向被拉伸 |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0015(void)
{
	int nTestID = 14;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 高清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏不变形"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |PAN_SCAN | 16 : 9  | 高  清 | 满屏不变形            |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0016(void)
{
	int nTestID = 15;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 标清输出通道上设置成LETTER_BOX的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"满屏不变形"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | 标  清 | 满屏不变形        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0017(void)
{
	int nTestID = 16;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 高清输出通道上设置成LETTER_BOX的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"画面满屏显示，左右方向被拉伸 "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | 高  清 |  画面满屏显示，左右方向被拉伸           |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0018(void)
{
	int nTestID = 17;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 标清输出通道上设置成LETTER_BOX的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为" 画面左右方向被压窄，左右有黑边，上下有内容被截掉 "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | 标  清 | 画面左右方向被压窄，左右有黑边，上下有内容被截掉     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0019(void)
{
	int nTestID = 18;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 高清输出通道上设置成LETTER_BOX的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"画面比例不变形，左右有黑边，上下有内容被截掉"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | 高  清 |画面比例不变形，左右有黑边，上下有内容被截掉 |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0020(void)
{
	int nTestID = 19;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 标清输出通道上设置成LETTER_BOX的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"画面比例不变形，上下有黑边，左右有内容被截掉"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | 标  清 |  画面比例不变形，上下有黑边，左右有内容被截掉|
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0021(void)
{
	int nTestID = 20;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 高清输出通道上设置成LETTER_BOX的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为" 画面左右方向被拉伸，上下有黑边，左右有内容被截掉"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | 高  清 |  画面左右方向被拉伸，上下有黑边，左右有内容被截掉     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0022(void)
{
	int nTestID = 21;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 标清输出通道上设置成LETTER_BOX的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏变形，图像上下方向被拉伸"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | 标  清 | 满屏变形，图像上下方向被拉伸|
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0023(void)
{
	int nTestID = 22;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 高清输出通道上设置成LETTER_BOX的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏不变形"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | 高  清 | 满屏不变形            |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0024(void)
{
	int nTestID = 23;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 标清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_IGNORE的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"满屏不变形"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |IGNORE |  4 : 3  | 标  清 | 满屏不变形        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0025(void)
{
	int nTestID = 24;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 高清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_IGNORE的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"满屏变形，画面左右方向被拉伸 "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |IGNORE |  4 : 3  | 高  清 |满屏变形，画面左右方向被拉伸 |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0026(void)
{
	int nTestID = 25;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 标清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_IGNORE的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏不变形"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |IGNORE | 16 : 9  | 标  清 | 满屏不变形        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0027(void)
{
	int nTestID = 26;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放4:3的源码流， 高清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_IGNORE的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示4:3的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏变形，画面左右方向被拉伸    "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |IGNORE | 16 : 9  | 高  清 | 满屏变形，画面左右方向被拉伸   |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0028(void)
{
	int nTestID = 27;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 标清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_IGNORE的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"满屏变形，图像上下方向被拉伸  "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |IGNORE |  4 : 3  | 标  清 |  满屏变形，图像上下方向被拉伸   |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0029(void)
{
	int nTestID = 28;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 高清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_IGNORE的适配模式，4:3 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 输出效果为"满屏不变形 "
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |IGNORE |  4 : 3  | 高  清 | 满屏不变形        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0030(void)
{
	int nTestID = 29;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 标清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_IGNORE的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏变形，画面上下方向被拉伸"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |IGNORE | 16 : 9  | 标  清 |  满屏变形，画面上下方向被拉伸 |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0031(void)
{
	int nTestID = 30;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  播放16:9的源码流， 高清输出通道上设置成EM_UDIVIDEO_MATCH_METHOD_IGNORE的适配模式，16:9 的画面比时的输出效果。 
//@PRECONDITION:Video设备能正确播放并显示16:9的视频数据流
//@INPUT:1、nVideoIndex   =  [0..nVideo-1]
//@INPUT:2、eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3、eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4、eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 输出效果为"满屏不变形"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、调用CSTKWaitYesKey，提示测试人员接高清输出设备
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示。
//@EXECUTIONFLOW: 10、调用CSTKWaitYesKey，测试人员确认视频成功播放并显示出来，并且画面不变形，左右有黑边
//@EXECUTIONFLOW: 11、恢复现场
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | 码流源 | 适应模式  |  画面比 |输出设备| 输出效果描述      |
//@REMARK: | 4 : 3  |IGNORE | 16 : 9  | 高  清 | 满屏不变形            |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0032(void)
{
	int nTestID = 31;
	return testMatchMethodAspectRatio(nTestID);
}

static void DumpVideoMsg(CSUDIVIDEOAspectRatioType_E enSource,
	CSUDIVIDEOAspectRatioType_E enOutput,
	CSUDIVIDEOMatchMethod_E enMatchMethod,
	CSUDISCREENType_E enDev)
{
	if (enOutput == enSource)
	{
		CSTCPrint("%s输出节目源AspectRatio同输出设置一样,画面全屏无拉伸和压缩(Y/N)\n",
			enDev==EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
		return ;
	}

	if (enSource == EM_UDIVIDEO_ASPECT_RATIO_4_3)
	{
		switch(enMatchMethod)
		{
			case EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX:
				CSTCPrint("%s输出节目源4:3 ,输出设置16:9,letter box 画面左右被压缩有黑边(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN:
				CSTCPrint("%s输出节目源4:3 ,输出设置16:9,pan scan 画面上下被拉伸,上下画面显示不全(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_COMBINED:
				CSTCPrint("%s输出节目源4:3 ,输出设置16:9,combind 画面上下被拉伸,左右被压缩有黑边(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_IGNORE:
				CSTCPrint("%s输出节目源4:3 ,输出设置16:9,ignore 画面全屏无拉伸和压缩(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
				break;
			default:
				break;
		}
	}
	else if (enSource == EM_UDIVIDEO_ASPECT_RATIO_16_9)
	{
		switch(enMatchMethod)
		{
			case EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX:
				CSTCPrint("%s输出节目源16:9 ,输出设置4:3,letter box 画面被上下压缩有黑边(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN:
				CSTCPrint("%s输出节目源16:9 ,输出设置4:3,pan scan 画面左右被拉伸,左右画面显示不全(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_COMBINED:
				CSTCPrint("%s输出节目源16:9 ,输出设置4:3,combind 画面左右被拉伸,上下被压缩有黑边(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_IGNORE:
				CSTCPrint("%s输出节目源16:9 ,输出设置4:3,ignore 画面全屏无拉伸和压缩(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
				break;
			default:
				break;
		}
	}
}

//@CASEGROUP: 1、video
//@CASEGROUP: 2、screen
//@DESCRIPTION:测试aspect ratio 设置,mach method 设置,不受分辨率变化影响
//@PRECONDITION:1、平台成功初始化
//@EXPECTATION:aspect ratio 设置,mach method 设置,不受分辨率变化影响
//@EXECUTIONFLOW:1、播放高清测试码流(源aspect ratio :16:9),期望播放成功 
//@EXECUTIONFLOW:2、调用CSUDISCREENSetResolution将高清输出分辨率设置成1080I_50HZ,期望设置成功
//@EXECUTIONFLOW:3、调用CSUDIVIDEOSetAspectRatio同时将高标清输出设置成4:3
//@EXECUTIONFLOW:4、调用CSUDIVIDEOSetMatchMethod将高标清输出同时设置为letter box 模式
//@EXECUTIONFLOW:5、确认输出效果
//@EXECUTIONFLOW:6、调用CSUDISCREENSetResolution将高清分辨率设置成1080I_50HZ,标清分辨率设置成NTSC
//@EXECUTIONFLOW:7、确认输出效果,期望分辨率改变不影响画形比和match method的设置
//@EXECUTIONFLOW:8、调用CSUDISCREENSetResolution将高清分辨率设置成720P_50HZ,标清分辨率设置成PAL
//@EXECUTIONFLOW:9、确认输出效果,期望分辨率改变不影响画形比和match method的设置
//@EXECUTIONFLOW:10 、调用CSUDIPLAYERClose关闭播放器
//@EXECUTIONFLOW:11、调用CSUDIVIDEOSetAspectRatio将高清aspect ratio设置回16:9,恢复测试现场
//@EXECUTIONFLOW:12、调用CSUDIVIDEOSetMatchMethod将高标清输出同时设回ignore模式,恢复测试现场
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0033(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E enDev;
	CSUDISCREENType_E enDevArray[]={EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E enRes[2];

	/*如果不支持高清节目直接测试成功?*/
	if (!bIsSupportHD())
	{
		return TRUE;
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "锁频失败!\n");

	enDev = EM_UDI_VOUT_DEVICE_HD;
	enRes[0] = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev,enRes,1),
		"fail to set resolution\n");
	
	enDev = EM_UDI_VOUT_DEVICE_SD|EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, enDev,EM_UDIVIDEO_ASPECT_RATIO_4_3),
		"fail to set aspect ratio\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
			"fail to set matchmethod\n");

	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_SD);
	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_HD);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

	enRes[0]=EM_UDISCREEN_RESOLUTION_NTSC;
	enRes[1]=EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(enDevArray, enRes, 2),
		"fail to set resolution\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, enDev,EM_UDIVIDEO_ASPECT_RATIO_4_3),
		"fail to set aspect ratio\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
			"fail to set matchmethod\n");
	CSTCPrint("标清分辨率设置成NTSC,高清设置成720P_50HZ\n");
	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_SD);
	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_HD);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

	enRes[0]=EM_UDISCREEN_RESOLUTION_PAL;
	enRes[1]=EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(enDevArray, enRes, 2),
		"fail to set resolution\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, enDev,EM_UDIVIDEO_ASPECT_RATIO_4_3),
		"fail to set aspect ratio\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
			"fail to set matchmethod\n");
	CSTCPrint("标清分辨率设置成PAL,高清设置成1080I_50HZ\n");
	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_SD);
	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_HD);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

	CSTK_FATAL_POINT;

	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	enDev = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, enDev,EM_UDIVIDEO_ASPECT_RATIO_16_9),
		"fail to set aspect ratio\n");

	enDev = EM_UDI_VOUT_DEVICE_SD|EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev, EM_UDIVIDEO_MATCH_METHOD_IGNORE),
			"fail to set matchmethod\n");

	return TRUE;
}


//@CASEGROUP:CSUDIVIDEOSetAspectRatio CSUDIVIDEOSetMatchMethod
//@DESCRIPTION: 测试高清节目源(16:9)的情况下画型比设置与matchmethod设置
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:平台支持高清节目解码和高清输出
//@INPUT:1、nVideoIndex = 0
//@INPUT:2、SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts
//@EXPECTATION: 画型比设置与matchmethod设置后video输出效果与预期一致
//@EXECUTIONFLOW: 1、播放高清测试码流，期望播放成功
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOSetMatchMethod设置高标清输出为letter box,确认效果
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetMatchMethod设置高标清输出为pan scan,确认效果
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOSetMatchMethod设置高标清输出为combind,确认效果
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetMatchMethod设置高标清输出为ignore,确认效果
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetAspectRatio设置高标清输出为4:3,期望返回成功
//@EXECUTIONFLOW: 7 、重复步骤2~5
//@EXECUTIONFLOW: 8 、调用CSUDIPLAYERClose关闭播放器
//@EXECUTIONFLOW: 9 、调用CSUDIVIDEOSetAspectRatio将高清输出设置回16:9恢复测试现场
//@EXECUTIONFLOW: 10 、调用CSUDIVIDEOSetMatchMethod将高标清match method 设置回ignore模式,恢复测试现场
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0034(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = -1;
	CSUDISCREENType_E enDev;
	int i=0;

	/*如果不支持高清节目直接测试成功?*/
	if (!bIsSupportHD())
	{
		return TRUE;
	}

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "锁频失败!\n");

	for(i=0;i<EM_UDIVIDEO_MATCH_METHOD_NUM;i++)
	{
		enDev = EM_UDI_VOUT_DEVICE_SD;
		eMatchMethod = i;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev,  eMatchMethod),
			"fail to set matchmethod\n");
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev,  eMatchMethod),
			"fail to set matchmethod\n");
		DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,eMatchMethod,EM_UDI_VOUT_DEVICE_SD);
		DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_16_9,eMatchMethod,EM_UDI_VOUT_DEVICE_HD);
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3),
		"fail to set aspect ratio\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_ASPECT_RATIO_4_3),
		"fail to set aspect ratio\n");

	for(i=0;i<EM_UDIVIDEO_MATCH_METHOD_NUM;i++)
	{
		enDev = EM_UDI_VOUT_DEVICE_SD;
		eMatchMethod = i;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev,  eMatchMethod),
			"fail to set matchmethod\n");
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev,  eMatchMethod),
			"fail to set matchmethod\n");
		DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,eMatchMethod,EM_UDI_VOUT_DEVICE_SD);
		DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,eMatchMethod,EM_UDI_VOUT_DEVICE_HD);
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_ASPECT_RATIO_16_9),
		"fail to set aspect ratio\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_MATCH_METHOD_IGNORE),
			"fail to set matchmethod\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_MATCH_METHOD_IGNORE),
			"fail to set matchmethod\n");

	return TRUE;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio
//@DESCRIPTION: 测试CSUDIVIDEOSetAspectRatio在nVideoIndex参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  {-1,nVideo,nVideo+1}
//@INPUT:2、eVideoDevice = EM_UDI_VOUT_DEVICE_HD,EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、eAspectRatio=  EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取{-1,nVideo,nVideo+1}内各值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetAspectRatio，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:在nVideoIndex取非法值时，CSUDIVIDEOSetAspectRatio应返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0001(void)
{
	int nVideoCount =0;
	int nVideoErrorCount = 3 * 2 * 2;
	int nVideoErrorIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	typedef struct 
	{
		int m_nVideoErrorIndex;
		CSUDISCREENType_E  m_eVideoDevice;
	 	CSUDIVIDEOAspectRatioType_E  m_eAspectRatio; 
	}CSTC_ErrorSetAspectRatio_S;

	CSTC_ErrorSetAspectRatio_S s_SetErrorAspectRatio[12];

	memset(s_SetErrorAspectRatio, 0, sizeof(s_SetErrorAspectRatio));
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
	
	s_SetErrorAspectRatio[0].m_nVideoErrorIndex = -1;
	s_SetErrorAspectRatio[0].m_eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
	s_SetErrorAspectRatio[0].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_16_9;

	s_SetErrorAspectRatio[1].m_nVideoErrorIndex = -1;
	s_SetErrorAspectRatio[1].m_eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
	s_SetErrorAspectRatio[1].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_4_3;

	s_SetErrorAspectRatio[2].m_nVideoErrorIndex = -1;
	s_SetErrorAspectRatio[2].m_eVideoDevice = EM_UDI_VOUT_DEVICE_HD;
	s_SetErrorAspectRatio[2].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_16_9;

	s_SetErrorAspectRatio[3].m_nVideoErrorIndex = -1;
	s_SetErrorAspectRatio[3].m_eVideoDevice = EM_UDI_VOUT_DEVICE_HD;
	s_SetErrorAspectRatio[3].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_4_3;

	s_SetErrorAspectRatio[4].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorAspectRatio[4].m_eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
	s_SetErrorAspectRatio[4].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_16_9;

	s_SetErrorAspectRatio[5].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorAspectRatio[5].m_eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
	s_SetErrorAspectRatio[5].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_4_3;

	s_SetErrorAspectRatio[6].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorAspectRatio[6].m_eVideoDevice = EM_UDI_VOUT_DEVICE_HD;
	s_SetErrorAspectRatio[6].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_16_9;

	s_SetErrorAspectRatio[7].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorAspectRatio[7].m_eVideoDevice = EM_UDI_VOUT_DEVICE_HD;
	s_SetErrorAspectRatio[7].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_4_3;

	s_SetErrorAspectRatio[8].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorAspectRatio[8].m_eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
	s_SetErrorAspectRatio[8].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_16_9;

	s_SetErrorAspectRatio[9].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorAspectRatio[9].m_eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
	s_SetErrorAspectRatio[9].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_4_3;

	s_SetErrorAspectRatio[10].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorAspectRatio[10].m_eVideoDevice = EM_UDI_VOUT_DEVICE_HD;
	s_SetErrorAspectRatio[10].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_16_9;

	s_SetErrorAspectRatio[11].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorAspectRatio[11].m_eVideoDevice = EM_UDI_VOUT_DEVICE_HD;
	s_SetErrorAspectRatio[11].m_eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_4_3;

	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSetAspectRatio(
						s_SetErrorAspectRatio[nVideoErrorIndex].m_nVideoErrorIndex, 
						s_SetErrorAspectRatio[nVideoErrorIndex].m_eVideoDevice, 
						s_SetErrorAspectRatio[nVideoErrorIndex].m_eAspectRatio)), "参数判断错误!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio
//@DESCRIPTION: 测试CSUDIVIDEOSetAspectRatio在eVideoDevice参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eVideoDevice=不存在的设备类型
//@INPUT:3、eAspectRatio=  EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取值[0..nVideo]内
//@EXECUTIONFLOW: 3、eVideoDevice取不存在的设备类型
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOSetAspectRatio，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0002(void)
{
	int nVideoCount =0;
	int nVideoIndex = 0;
	CSUDISCREENType_E  eVideoDevice = 0;
	CSUDIVIDEOAspectRatioType_E  eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_16_9;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetAspectRatio(nVideoIndex, eVideoDevice, eAspectRatio)),
							"参数判断错误!(16:9)");
		
		eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_4_3;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetAspectRatio(nVideoIndex, eVideoDevice, eAspectRatio)),
							"参数判断错误!(4:3)");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio
//@DESCRIPTION: 测试CSUDIVIDEOSetAspectRatio在eAspectRatio参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eVideoDevice= EM_UDI_VOUT_DEVICE_HD,EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、eAspectRatio非法
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内各值
//@EXECUTIONFLOW: 3、eAspectRatio取非法值
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOSetAspectRatio，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0003(void)
{
	int nVideoCount =0;
	int nVideoIndex = 0;
	CSUDISCREENType_E  eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDIVIDEOAspectRatioType_E  eAspectRatio = -1;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetAspectRatio(nVideoIndex, eVideoDevice, eAspectRatio)),
							"参数判断错误!(SD)");
		
		eVideoDevice = EM_UDI_VOUT_DEVICE_HD;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetAspectRatio(nVideoIndex, eVideoDevice, eAspectRatio)),
							"参数判断错误!(HD)");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio
//@DESCRIPTION: 测试CSUDIVIDEOSetAspectRatio标清通道的上的参数改变不影响高清通道上的输出效果
//@PRECONDITION:高标清不同源
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放4:3的标清视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eVideoDevice= EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、eMatchMethod= EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX
//@INPUT:3、eAspectRatio=EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: 标清通道设置画面比不影响高清通道的输出
//@EXECUTIONFLOW: 1、判断是否高标清同源，若是则直接跳到步骤8
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个标清视频数据流
//@EXECUTIONFLOW: 5、请测试人员确认高清输出通道正确输出视频
//@EXECUTIONFLOW: 6、设置标清通道的MatchMothed和AspectRatio
//@EXECUTIONFLOW: 7、请测试人员确认高清输出通道的视频没有任何变化
//@EXECUTIONFLOW: 8、恢复现场
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0004(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	if(CSUDI_FALSE == CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		// 2-3
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!");
		
		CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
		   
		// 4
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency(&hPlayer)), "锁频失败!\n");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

		// 5
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)), "SetMatchMode设置失败");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_4_3)), "SetAspectRatio设置失败");

		CSTCPrint("请将高清输出端口连接电视并确定高清视频正确播放出来!\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "无法获得按键确认");

		// 6
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)), "SetMatchMode设置失败");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_16_9)), "SetAspectRatio设置失败");

		// 7
		CSTCPrint("请确认高清视频输出没有发生任何变化!\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "无法获得按键确认");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放器失败!");
		
		bResult = CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("高标清同源!\n");
	}
	
	CSTK_FATAL_POINT;
		
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio
//@DESCRIPTION: 测试CSUDIVIDEOSetAspectRatio高清通道的上的参数改变不影响标清通道上的输出效果
//@PRECONDITION:高标清不同源
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放16:9的标清视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eVideoDevice= EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、eAspectRatio=  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@INPUT:4、eMatchMethod= EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN
//@EXPECTATION: 高清通道的画面比参数改变不影响标清通道的输出效果
//@EXECUTIONFLOW: 1、判断是否高标清同源，若是则直接跳到步骤8
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 3、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 4、在nVideoIndex设备上循环播放一个16:9 视频数据流
//@EXECUTIONFLOW: 5、请测试人员确认标清输出通道正确输出视频
//@EXECUTIONFLOW: 6、设置高清通道的MatchMothed和AspectRatio
//@EXECUTIONFLOW: 7、请测试人员确认标清输出通道的视频没有任何变化
//@EXECUTIONFLOW: 8、恢复现场
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0005(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	if(CSUDI_FALSE == CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		// 2-3
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!");
		
		CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
		   
		// 4
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

		// 5
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)), "SetMatchMode设置失败");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_4_3)), "SetAspectRatio设置失败");

		CSTCPrint("请将标清输出端口连接电视，并确定标清视频正确播放出来!\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "无法获得按键确认");

		// 6
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)), "SetMatchMode设置失败");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_16_9)), "SetAspectRatio设置失败");

		// 7
		CSTCPrint("请确认标清视频输出没有发生任何变化!\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "无法获得按键确认");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放器失败!");
			
		bResult = CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("高标清同源!\n");
	}
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetAspectRatio
//@DESCRIPTION: 测试CSUDIVIDEOGetAspectRatio 接口对错误参数的检测能力
//@INPUT:1、nVideoIndex =  非法
//@INPUT:2、eScreenDevice= 非法
//@INPUT:3、peAspectRatio=  非法
//@EXPECTATION:对该接口传递错误参数，不应该返回成功
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount 获得平台video 的数量
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetAspectRatio 传递错误参数，希望返回CSUDIVIDEO_ERROR_BAD_PARAMETER
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetAspectRatio_0001(void)
{
	int 						nVideoIndex = 0;
	CSUDISCREENType_E 			eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDIVIDEOAspectRatioType_E	eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_AUTO;
	int							nVideoCount = 0;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	int							i = 0;

	typedef struct _ErrorParam_GetAspectRatio_S
	{
		int 							m_nVideoIndex;
		CSUDISCREENType_E 			m_eScreenDevice;
		CSUDIVIDEOAspectRatioType_E	*m_peAspectRatio;
	}ErrorParam_GetAspectRatio_S;

	ErrorParam_GetAspectRatio_S ErrorParamList[5];

	memset(ErrorParamList, 0, sizeof(ErrorParamList));
	
	// 获得 video 的数量
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获得video 数量失败");

	ErrorParamList[0].m_nVideoIndex = -1;
	ErrorParamList[0].m_eScreenDevice = eScreenDevice;
	ErrorParamList[0].m_peAspectRatio = &eAspectRatio;
	
	ErrorParamList[1].m_nVideoIndex = nVideoCount;
	ErrorParamList[1].m_eScreenDevice = eScreenDevice;
	ErrorParamList[1].m_peAspectRatio = &eAspectRatio;

	ErrorParamList[2].m_nVideoIndex = nVideoIndex;
	ErrorParamList[2].m_eScreenDevice = -1;
	ErrorParamList[2].m_peAspectRatio = &eAspectRatio;

	ErrorParamList[3].m_nVideoIndex = nVideoIndex;
	ErrorParamList[3].m_eScreenDevice = EM_UDI_VOUT_DEVICE_HD+1;
	ErrorParamList[3].m_peAspectRatio = &eAspectRatio;

	ErrorParamList[4].m_nVideoIndex = nVideoIndex;
	ErrorParamList[4].m_eScreenDevice = eScreenDevice;
	ErrorParamList[4].m_peAspectRatio = CSUDI_NULL;


	//检测错误参数
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetAspectRatio(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eScreenDevice, ErrorParamList[i].m_peAspectRatio);
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == nResult), "检测错误参数失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIVIDEOSetMatchMethod
//@DESCRIPTION: 测试CSUDIVIDEOSetMatchMethod在nVideoIndex参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放并显示视频数据流
//@INPUT:1、nVideoIndex =  {-1,nVideo,nVideo+1}
//@INPUT:2、eVideoDevice=EM_UDI_VOUT_DEVICE_HD,EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、eMatchMethod=  CSUDIVIDEOMatchMethod_E 枚举值
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取{-1,nVideo,nVideo+1}内各值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetMatchMethod，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:在nVideoIndex取非法值时，CSUDIVIDEOSetMatchMethod应返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_VIDEO_IT_SetMatchMethod_0001(void)
{
	int nVideoCount =0;
	int nVideoErrorCount = 3 * 2;
	int nVideoErrorIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	typedef struct 
	{
		int m_nVideoErrorIndex;
		CSUDISCREENType_E  m_eVideoDevice;
	}CSTC_ErrorSetMatchMethod_S;
	
	CSTC_ErrorSetMatchMethod_S s_SetErrorMatchMethod[6];

	memset(s_SetErrorMatchMethod, 0, sizeof(s_SetErrorMatchMethod));

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	s_SetErrorMatchMethod[0].m_nVideoErrorIndex = -1;
	s_SetErrorMatchMethod[0].m_eVideoDevice = EM_UDI_VOUT_DEVICE_SD;

	s_SetErrorMatchMethod[1].m_nVideoErrorIndex = -1;
	s_SetErrorMatchMethod[1].m_eVideoDevice = EM_UDI_VOUT_DEVICE_HD;

	s_SetErrorMatchMethod[2].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorMatchMethod[2].m_eVideoDevice = EM_UDI_VOUT_DEVICE_SD;

	s_SetErrorMatchMethod[3].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorMatchMethod[3].m_eVideoDevice = EM_UDI_VOUT_DEVICE_HD;
			
	s_SetErrorMatchMethod[4].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorMatchMethod[4].m_eVideoDevice = EM_UDI_VOUT_DEVICE_SD;

	s_SetErrorMatchMethod[5].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorMatchMethod[5].m_eVideoDevice = EM_UDI_VOUT_DEVICE_HD;

	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex ++)
	{
		int nMatchMethodIndex = 0;
		
		for (nMatchMethodIndex = 0; nMatchMethodIndex < EM_UDIVIDEO_MATCH_METHOD_NUM; nMatchMethodIndex ++)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSetMatchMethod(
						s_SetErrorMatchMethod[nVideoErrorIndex].m_nVideoErrorIndex, 
						s_SetErrorMatchMethod[nVideoErrorIndex].m_eVideoDevice, 
						EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX + nMatchMethodIndex)), 
						"参数判断错误!");
		}
	}
	
	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;

	return bResult; 
}

//@CASEGROUP:CSUDIVIDEOSetMatchMethod
//@DESCRIPTION: 测试CSUDIVIDEOSetMatchMethod在eVideoDevice参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eVideoDevice非法
//@INPUT:3、eMatchMethod=  CSUDIVIDEOMatchMethod_E 枚举值
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取值[0..nVideo]内
//@EXECUTIONFLOW: 3、eVideoDevice取非法值
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOSetMatchMethod，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetMatchMethod_0002(void)
{
	int nVideoCount =0;
	int nVideoIndex = 0;
	CSUDISCREENType_E  eVideoDevice = 0; 
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");	
	   
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		int nErrorMatchMethodIndex = 0;
		
		for (nErrorMatchMethodIndex = 0; nErrorMatchMethodIndex < EM_UDIVIDEO_MATCH_METHOD_NUM; nErrorMatchMethodIndex ++)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetMatchMethod(
								nVideoIndex, 
								eVideoDevice, 
								EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX + nErrorMatchMethodIndex)),
								"参数判断错误!\n"); 
		}
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}


//@CASEGROUP:CSUDIVIDEOSetMatchMethod
//@DESCRIPTION: 测试CSUDIVIDEOSetMatchMethod在eMatchMethod参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eVideoDevice=EM_UDI_VOUT_DEVICE_HD,EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、eMatchMethod非法
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内各值
//@EXECUTIONFLOW: 3、eMatchMethod取非法值
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOSetMatchMethod，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetMatchMethod_0003(void)
{
	int nVideoCount =0;
	int nVideoIndex = 0;
	CSUDIVIDEOMatchMethod_E  eMatchMethod = EM_UDIVIDEO_MATCH_METHOD_NUM;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
	   
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetMatchMethod(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, eMatchMethod)),
							"参数判断错误!(SD)");
				
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetMatchMethod(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, eMatchMethod)),
							"参数判断错误!(HD)");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}



//@CASEGROUP:CSUDIVIDEOGetMatchMethod
//@DESCRIPTION: 测试CSUDIVIDEOGetMatchMethod 接口对错误参数的检测能力
//@INPUT:1、nVideoIndex =  非法
//@INPUT:2、eScreenDevice= 非法
//@INPUT:3、peMatchMethod=  非法
//@EXPECTATION:对该接口传递错误参数，不应该返回成功
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount 获得平台video 的数量
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetMatchMethod 传递错误参数，希望返回CSUDIVIDEO_ERROR_BAD_PARAMETER
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetMatchMethod_0001(void)
{
	int 							nVideoIndex = 0;
	CSUDISCREENType_E 			eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX;
	int							nVideoCount = 0;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	int							i = 0;

	typedef struct _ErrorParam_GetMatchMethod_S
	{
		int 							m_nVideoIndex;
		CSUDISCREENType_E 			m_eScreenDevice;
		CSUDIVIDEOMatchMethod_E		*m_peMatchMethod;
	}ErrorParam_GetMatchMethod_S;

	ErrorParam_GetMatchMethod_S ErrorParamList[5];

	memset(ErrorParamList, 0, sizeof(ErrorParamList));

	// 获得 video 的数量
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获得video 数量失败");

	ErrorParamList[0].m_nVideoIndex = -1;
	ErrorParamList[0].m_eScreenDevice = eScreenDevice;
	ErrorParamList[0].m_peMatchMethod = &eMatchMethod;
	
	ErrorParamList[1].m_nVideoIndex = nVideoCount;
	ErrorParamList[1].m_eScreenDevice = eScreenDevice;
	ErrorParamList[1].m_peMatchMethod = &eMatchMethod;

	ErrorParamList[2].m_nVideoIndex = nVideoIndex;
	ErrorParamList[2].m_eScreenDevice = -1;
	ErrorParamList[2].m_peMatchMethod = &eMatchMethod;

	ErrorParamList[3].m_nVideoIndex = nVideoIndex;
	ErrorParamList[3].m_eScreenDevice = EM_UDIVIDEO_MATCH_METHOD_NUM;
	ErrorParamList[3].m_peMatchMethod = &eMatchMethod;

	ErrorParamList[4].m_nVideoIndex = nVideoIndex;
	ErrorParamList[4].m_eScreenDevice = eScreenDevice;
	ErrorParamList[4].m_peMatchMethod = CSUDI_NULL;

	//检测错误参数
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetMatchMethod(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eScreenDevice, ErrorParamList[i].m_peMatchMethod);
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == nResult), "检测错误参数失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIVIDEOGetAspectRatio,CSUDIVIDEOGetMatchMethod
//@DESCRIPTION: 测试在CSUDIVIDEOSetAspectRatio ,CSUDIVIDEOGetMatchMethod接口在未进行任何播放的时候被调用
//@INPUT:1、nVideoIndex =  0
//@INPUT:2、eScreenDevice= EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、peAspectRatio=  非空
//@INPUT:4、peMatchMethod=  非空
//@EXPECTATION:对该接口使用场景进行逻辑检测
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetAspectRatio, 希望在合理范围内
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetMatchMethod, 希望合理范围内
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetAspectRatioMatchMethod_0001(void)
{
	CSUDIVIDEOAspectRatioType_E	eAspectRatio = -1;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = -1;

	//测试在未进行播放时获得eAspectRatio和eMatchMethod 的情况
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, &eAspectRatio)), "CSUDIVIDEOGetAspectRatio 调用失败");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN < eAspectRatio) &&(eAspectRatio <= EM_UDIVIDEO_ASPECT_RATIO_AUTO), "eAspectRatio不在合理范围内");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD, &eMatchMethod)), "CSUDIVIDEOGetMatchMethod调用失败");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX <= eMatchMethod) &&(eMatchMethod < EM_UDIVIDEO_MATCH_METHOD_NUM), "eMatchMethod不在合理范围内");
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIVIDEOGetAspectRatio,CSUDIVIDEOGetMatchMethod
//@DESCRIPTION: 测试在正常播放之后,CSUDIVIDEOSetAspectRatio，CSUDIVIDEOSetMatchMethod之前使用CSUDIVIDEOGetAspectRatio，CSUDIVIDEOGetMatchMethod
//@INPUT:1、nVideoIndex =  0
//@INPUT:2、eScreenDevice= EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、peAspectRatio=  非空
//@INPUT:4、peMatchMethod=  非空
//@EXPECTATION:对该接口使用场景进行逻辑检测
//@EXECUTIONFLOW: 1、调用LockFrequency 进行锁频, 打开播放器等操作
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetAspectRatio, 希望得到合理范围值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOGetMatchMethod, 希望得到合理范围值
//@EXECUTIONFLOW: 4、释放播放资源
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetAspectRatioMatchMethod_0002(void)
{
	CSUDI_HANDLE 				hPlayer = CSUDI_NULL;
	CSUDIVIDEOAspectRatioType_E	eAspectRatio = -1;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = -1;
	   
	// 播放
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

	//测试eAspectRatio和eMatchMethod 的情况
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD, &eAspectRatio)), "step2 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN < eAspectRatio) &&(eAspectRatio <= EM_UDIVIDEO_ASPECT_RATIO_AUTO), "eAspectRatio不在合理范围内");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD, &eMatchMethod)), "step3 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX <= eMatchMethod) &&(eMatchMethod < EM_UDIVIDEO_MATCH_METHOD_NUM), "eMatchMethod不在合理范围内");

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOGetAspectRatio,CSUDIVIDEOGetMatchMethod
//@DESCRIPTION: 测试CSUDIVIDEOGetAspectRatio 和 CSUDIVIDEOGetMatchMethod接口在正常播放,设置,停止之后才被调用
//@INPUT:1、nVideoIndex =  0
//@INPUT:2、eScreenDevice= EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、peAspectRatio=  非空
//@INPUT:4、peMatchMethod=  非空
//@EXPECTATION:对该接口使用场景进行逻辑检测
//@EXECUTIONFLOW: 1、调用LockFrequency 进行锁频, 打开播放器等操作
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOSetAspectRatio 设置播放模式
//@EXECUTIONFLOW: 3、对标清通道调用CSUDIVIDEOSetMatchMethod 设置播放模式
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStop 停止视频播放
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOGetAspectRatio, 希望得到设定值
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOGetMatchMethod, 希望得到设定值
//@EXECUTIONFLOW: 7、对高清通道CSUDIVIDEOSetAspectRatio,CSUDIVIDEOSetMatchMethod ,期望不影响标清设置的eAspectRatio和eMatchMethod
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetAspectRatioMatchMethod_0003(void)
{
	CSUDI_HANDLE 				hPlayer = CSUDI_NULL;
	CSUDIVIDEOAspectRatioType_E	eAspectRatio = -1;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = -1;
	CSUDI_BOOL					bStopFlag = CSUDI_FALSE;
	   
	// 播放
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

	// 设置播放
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, 
						EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_AUTO)), "SetAspectRatio设置失败");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, 
						EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)), "SetMatchMethod设置失败");

	// 停止播放
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
	bStopFlag = CSUDI_TRUE;

	//测试eAspectRatio和eMatchMethod 的情况
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, &eAspectRatio)), "step5 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_AUTO == eAspectRatio), "eAspectRatio不是设置的值");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD, &eMatchMethod)), "step6 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX == eMatchMethod), "eMatchMethod不是设置的值");

	//对高清通道CSUDIVIDEOSetAspectRatio,CSUDIVIDEOSetMatchMethod 
	//期望不影响标清通道设置的eAspectRatio和eMatchMethod
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, 
						EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_16_9)), "SetAspectRatio设置失败");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, 
						EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)), "SetMatchMethod设置失败");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD, &eAspectRatio)), "step5 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_16_9 == eAspectRatio), "eAspectRatio不是设置的值");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, &eAspectRatio)), "step5 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_AUTO == eAspectRatio), "eAspectRatio不是设置的值");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD, &eMatchMethod)), "step6 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN == eMatchMethod), "eMatchMethod不是设置的值");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD, &eMatchMethod)), "step6 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX == eMatchMethod), "eMatchMethod不是设置的值");
	

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		if(!bStopFlag)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}
		
	return CSUDI_TRUE;
}

static CSUDI_BOOL testSetStopMode(CSUDIVIDEOStopMode_E eStopModeType)
{
	int nVideoCount = 0;	
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIVIDEOStopMode_E eStopMode = -1;
       
	CSTK_ASSERT_TRUE_FATAL(((EM_UDIVIDEO_STOPMODE_BLACK==eStopModeType)||(EM_UDIVIDEO_STOPMODE_FREEZE ==eStopModeType)),"参数错误\n");
	
	// 1
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
	   
	// 2-3
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
    
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");

	// 5
	CSTCPrint("请将标清输出端口连接电视并确定标清视频正确播放出来!\n");
	   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "无法获得按键确认");

	// 6
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0,eStopModeType)), "显示视频失败!\n");

	// 7
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIPLAYERStop(hPlayer)), "停止播放失败!\n");

       // 8
	if (eStopModeType == EM_UDIVIDEO_STOPMODE_BLACK)
	{
		CSTCPrint("请观察目前屏幕视频是黑屏状态!\n");
		// 进行GetStopMode 的检查
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode), "GetStopMode 调用失败");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIVIDEO_STOPMODE_BLACK == eStopMode, "获得的eStopMode和设置的eStopMode不一致");
	}
	else
	{
		CSTCPrint("请观察目前屏幕视频是静帧状态!\n");
		// 进行GetStopMode 的检查
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode), "GetStopMode 调用失败");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIVIDEO_STOPMODE_FREEZE == eStopMode, "获得的eStopMode和设置的eStopMode不一致");
	}
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "无法获得按键确认");

	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: 测试CSUDIVIDEOSetStopMode在EM_UDIVIDEO_STOPMODE_BLACK情况下的执行效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eStopMode=EM_UDIVIDEO_STOPMODE_BLACK
//@EXPECTATION: 返回CSUDI_SUCCESS，视频出现黑屏
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetStopMode(nVideoIndex,EM_UDIVIDEO_STOPMODE_BLACK)，并判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭nVideoIndex设备上的视频播放
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频是否黑屏
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0001(void)
{		
	return testSetStopMode(EM_UDIVIDEO_STOPMODE_BLACK);
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: 测试CSUDIVIDEOSetStopMode在EM_UDIVIDEO_STOPMODE_FREEZE情况下的执行效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eStopMode=EM_UDIVIDEO_STOPMODE_FREEZE
//@EXPECTATION: 返回CSUDI_SUCCESS，视频出现静帧
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的一个值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetStopMode(nVideoIndex,EM_UDIVIDEO_STOPMODE_FREEZE)，并判断返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、关闭nVideoIndex设备上的视频播放
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频是否静帧
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0002(void)
{
	return testSetStopMode(EM_UDIVIDEO_STOPMODE_FREEZE);
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: 测试CSUDIVIDEOSetStopMode在nVideoIndex参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex = {-1,nVideo,nVideo+1}
//@INPUT:2、eStopMode=CSUDIVIDEOStopMode_E枚举值
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取{-1,nVideo,nVideo+1}内的值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetStopMode(nVideoIndex,...)，并判断返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0003(void)
{
	int nVideoCount = 0;
	int nErrorParmCount = 3 * 2;
	int nErrorParmIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	typedef struct 
	{
		int m_nVideoErrorIndex;
		CSUDIVIDEOStopMode_E  m_eStopModeType;	
	}CSTC_ErrorStopMode_S;

	CSTC_ErrorStopMode_S s_ErrorStopModeTpye[6];

	memset(s_ErrorStopModeTpye, 0, sizeof(s_ErrorStopModeTpye));

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");

    CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	s_ErrorStopModeTpye[0].m_nVideoErrorIndex = -1;
	s_ErrorStopModeTpye[0].m_eStopModeType = EM_UDIVIDEO_STOPMODE_BLACK;

	s_ErrorStopModeTpye[1].m_nVideoErrorIndex = -1;
	s_ErrorStopModeTpye[1].m_eStopModeType = EM_UDIVIDEO_STOPMODE_FREEZE;

	s_ErrorStopModeTpye[2].m_nVideoErrorIndex = nVideoCount;
	s_ErrorStopModeTpye[2].m_eStopModeType = EM_UDIVIDEO_STOPMODE_BLACK;

	s_ErrorStopModeTpye[3].m_nVideoErrorIndex = nVideoCount;
	s_ErrorStopModeTpye[3].m_eStopModeType = EM_UDIVIDEO_STOPMODE_FREEZE;

	s_ErrorStopModeTpye[4].m_nVideoErrorIndex = nVideoCount + 1;
	s_ErrorStopModeTpye[4].m_eStopModeType = EM_UDIVIDEO_STOPMODE_BLACK;

	s_ErrorStopModeTpye[5].m_nVideoErrorIndex = nVideoCount + 1;
	s_ErrorStopModeTpye[5].m_eStopModeType = EM_UDIVIDEO_STOPMODE_FREEZE;

	
	for (nErrorParmIndex = 0; nErrorParmIndex < nErrorParmCount; nErrorParmIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSetStopMode(s_ErrorStopModeTpye[nErrorParmIndex].m_nVideoErrorIndex, s_ErrorStopModeTpye[nErrorParmIndex].m_eStopModeType)), 
							"视频停止模式 参数判断失败!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: 测试CSUDIVIDEOSetStopMode在eStopMode参数非法情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex = [0,nVideo-1]
//@INPUT:2、eStopMode取非法值
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0,nVideo-1]内的值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetStopMode(nVideoIndex,...)，并判断返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0004(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDIVIDEOStopMode_E  m_eStopModeErrorType =  EM_UDIVIDEO_STOPMODE_FREEZE + 10;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSetStopMode(nVideoIndex, m_eStopModeErrorType)), 
							"视频停止模式 参数判断失败!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
		
	return bResult;
}

//循环切换节目
void CSUDI_VIDEO_ChangeProgram(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S* sStreamInfo)
{
	int nStreamCnt = 2;

	if(CSUDI_SUCCESS != CSUDIVIDEOShow(0, CSUDI_FALSE))
	{
		CSTCPrint("步骤8，隐藏视频窗口失败!\n");
	       return;
	}

	if(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer))
	{
	    CSTCPrint("步骤9，停止播放失败!\n");
	    return;
	}
	    
	if(CSUDI_SUCCESS != CSUDIPLAYERSetStream(hPlayer, sStreamInfo, nStreamCnt, CSUDI_NULL))
	{
	    CSTCPrint("步骤10，设置解码信息失败!\n");
	    return;
	}

	CSUDIOSThreadSleep(1000);

	if(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer))
	{
	    CSTCPrint("步骤11，开始播放失败!\n");
	    return;
	}

	if(CSUDI_SUCCESS != CSUDIVIDEOShow(0, CSUDI_TRUE))
	{
	    CSTCPrint("步骤12，显示视频窗口失败!\n");
	    return;
	}

	CSUDIOSThreadSleep(3000);
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: 模拟加锁节目切换到清流，切台为静帧模式
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eStopMode=EM_UDIVIDEO_STOPMODE_FREEZE
//@EXPECTATION: 在节目切换过程中，期望没有残留帧
//@EXECUTIONFLOW: 1、获取视频解码器个数，至少存在一个视频解码器
//@EXECUTIONFLOW: 2、锁定一频点，该频点上播放的码流为Audio&Video Test_27Mbps_20070524.ts
//@EXECUTIONFLOW: 3、调用CSUDIPLAYEROpen打开播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERSetStream设置解码信息,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIPLAYERStart开始播放，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOShow显示视频窗口，期望成功
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetStopMode设置为静帧模式，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调CSUDIVIDEOShow隐藏窗口，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIPLAYERStop停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERSetStream设置节目属性，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW: 11、调用CSUDIPLAYERStart启动播放同频点另一个节目，观察是否有前一个节目残留帧，期望没有
//@EXECUTIONFLOW: 12、调用CSUDIVIDEOShow显示窗口，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW: 13、重复步骤(8~12)5次,在同一频点的两个不同节目中切换
//@EXECUTIONFLOW: 14、调用CSUDIPLAYERStop停止播放，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 15、调用CSUDIPLAYERClose关闭播放器，期望返回CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0005(void)
{
    int i = 0;
    int nStreamCnt = 2;
    int nTunerIndex = 0;
    int nVideoCount = 0;
    CSUDI_HANDLE hPlayer;
    CSUDIPlayerChnl_S  sPlayerChnl;
    CSUDIStreamInfo_S  sStreamInfo[4];
    CSUDIPLAYERType_E  ePlayerType = EM_UDIPLAYER_LIVE;

    sStreamInfo[0].m_nPid = 1140;
    sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
    sStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;

    sStreamInfo[1].m_nPid = 1141;
    sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
    sStreamInfo[1].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_MPEG2;

    sStreamInfo[2].m_nPid = 60;
    sStreamInfo[2].m_eContentType = EM_UDI_CONTENT_VIDEO;
    sStreamInfo[2].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
    
    sStreamInfo[3].m_nPid = 62;
    sStreamInfo[3].m_eContentType = EM_UDI_CONTENT_AUDIO;
    sStreamInfo[3].m_uStreamType.m_eVideoType = EM_UDI_AUD_STREAM_MPEG2;

    sPlayerChnl.m_nDemux = CSUDI_VIDEO_FindDemuxID(EM_UDI_DEMUX_PLAY);
    sPlayerChnl.m_nVideoDecoder = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_MPEG2);
    sPlayerChnl.m_nAudioDecoder = CSUDI_VIDEO_FindAudioDecoder(EM_UDI_AUD_STREAM_MPEG2);

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "步骤1，获取Video数量失败!");
    
    CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0), "没有可用的视频解码器");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex, "Audio&Video Test_27Mbps_20070524.ts"), "步骤2，锁频失败!");

    CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer)), "步骤3，打开播放器失败!");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "创建播放器句柄失败!");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, sStreamInfo, nStreamCnt, CSUDI_NULL), "步骤4，设置解码信息失败!");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "步骤5，开始播放失败!");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE), "步骤6，显示视频播放器失败");

    CSTCPrint("请将标清输出连接到标清电视上\n");
    
    CSTCPrint("音视频是否能够正常播放？\n");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"播放不正常");

    CSTCPrint("通过设置黑屏模拟从加锁节目切换到清流节目，请注意观察是否有上个节目的残留帧(循环5次)\n");

    CSTKWaitAnyKey();
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0, EM_UDIVIDEO_STOPMODE_FREEZE), "步骤7，设置静帧失败!");

    for(i = 0; i < 5; i++)
    {
        CSUDI_VIDEO_ChangeProgram(hPlayer, &sStreamInfo[2]);
    
        CSUDI_VIDEO_ChangeProgram(hPlayer, &sStreamInfo[0]);
    }

    CSTCPrint("在循环5次切台过程中，没有残留上一个节目的I帧请按'Y'，否则按'N'\n");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "有上个节目的帧残留");

    CSTK_FATAL_POINT;

    if(CSUDI_NULL != hPlayer)
    {
        CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), "步骤14，停止播放失败!");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤15，关闭播放器失败!");
    }
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOGetStopMode
//@DESCRIPTION: 测试CSUDIVIDEOGetStopMode 接口对错误参数的检测能力
//@INPUT:1、nVideoIndex =  非法
//@INPUT:2、peStopMode = 非法
//@EXPECTATION:对该接口传递错误参数，不应该返回成功
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount 获得平台video 的数量
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetStopMode 传递错误参数，希望返回CSUDIVIDEO_ERROR_BAD_PARAMETER
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetStopMode_0001(void)
{
	int 						nVideoIndex = 0;
	CSUDIVIDEOStopMode_E 		eStopMode = EM_UDIVIDEO_STOPMODE_BLACK;
	int							nVideoCount = 0;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	int							i = 0;

	typedef struct _ErrorParam_GetStopMode_S
	{
		int 							m_nVideoIndex;
		CSUDIVIDEOStopMode_E		*eStopMode;
	}ErrorParam_GetStopMode_S;

	ErrorParam_GetStopMode_S ErrorParamList[3];

	memset(ErrorParamList, 0, sizeof(ErrorParamList));
			
	// 获得 video 的数量
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获得video 数量失败");

	ErrorParamList[0].m_nVideoIndex = -1;
	ErrorParamList[0].eStopMode = &eStopMode;

	ErrorParamList[1].m_nVideoIndex = nVideoCount;
	ErrorParamList[1].eStopMode = &eStopMode;

	ErrorParamList[2].m_nVideoIndex = nVideoIndex;
	ErrorParamList[2].eStopMode = CSUDI_NULL;

	//检测错误参数
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetStopMode(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].eStopMode);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "检测错误参数失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}


static CSUDI_BOOL testGetStopMode(CSUDI_HANDLE hPlayer, CSUDIVIDEOStopMode_E eSetStopMode)
{
	CSUDIVIDEOStopMode_E 	eGetStopMode = -1;
	CSUDI_BOOL 				bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "播放失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0, eSetStopMode)), "CSUDIVIDEOSetStopMode 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eGetStopMode)), "CSUDIVIDEOGetStopMode失败");
	CSTK_ASSERT_TRUE_FATAL((eSetStopMode == eGetStopMode), "获得的eStopMode不正确");

	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetStopMode
//@DESCRIPTION: 测试CSUDIVIDEOGetStopMode 接口在视频播放,暂停,设置停止模式之前调用的情况
//@INPUT:1、nVideoIndex =  0
//@INPUT:2、peStopMode = EM_UDIVIDEO_STOPMODE_BLACK,EM_UDIVIDEO_STOPMODE_FREEZE
//@EXPECTATION:对该接口使用场景进行逻辑检测
//@EXECUTIONFLOW: 1、在未进行播放之前CSUDIVIDEOGetStopMode,希望成功,且在正确范围内
//@EXECUTIONFLOW: 2、在播放时CSUDIVIDEOGetStopMode,希望成功,且在正确范围内
//@EXECUTIONFLOW: 3、在暂停时CSUDIVIDEOGetStopMode,希望成功,且在正确范围内
//@EXECUTIONFLOW: 4、在CSUDIVIDEOSetStopMode 之前,StopPlayer之后CSUDIVIDEOGetStopMode,希望成功,且在正确范围内
//@EXECUTIONFLOW: 5、在StopPlayer之后CSUDIVIDEOSetStopMode ,调用CSUDIVIDEOGetStopMode,希望获得设定值EM_UDIVIDEO_STOPMODE_BLACK
//@EXECUTIONFLOW: 6、在CSUDIVIDEOSetStopMode 之后,StopPlayer之后CSUDIVIDEOGetStopMode,希望获得设定值EM_UDIVIDEO_STOPMODE_FREEZE
//@EXECUTIONFLOW: 7、在ClosePlayer之后CSUDIVIDEOGetStopMode,希望成功
CSUDI_BOOL CSTC_VIDEO_IT_GetStopMode_0002(void)
{
	CSUDIVIDEOStopMode_E 		eStopMode = -1;
	CSUDI_HANDLE 				hPlayer = CSUDI_NULL;
	CSUDI_BOOL					bStopFlag = CSUDI_FALSE;

	//播放前调用CSUDIVIDEOGetStopMode
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "播放码流前CSUDIVIDEOGetStopMode应该成功");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_BLACK <= eStopMode) &&(eStopMode <=EM_UDIVIDEO_STOPMODE_FREEZE), "获得的eStopMode不在正确范围内");


	// 播放时调用CSUDIVIDEOGetStopMode
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "播放码流时CSUDIVIDEOGetStopMode应该成功");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_BLACK <= eStopMode) &&(eStopMode <=EM_UDIVIDEO_STOPMODE_FREEZE), "获得的eStopMode不在正确范围内");
	
	//停止播放时CSUDIVIDEOGetStopMode(未进行CSUDIVIDEOSetStopMode)
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
	bStopFlag = CSUDI_TRUE;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "CSUDIVIDEOGetStopMode应该成功");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_BLACK <= eStopMode) &&(eStopMode <=EM_UDIVIDEO_STOPMODE_FREEZE), "获得的eStopMode不在正确范围内");

	//停止播放后CSUDIVIDEOSetStopMode
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0, EM_UDIVIDEO_STOPMODE_BLACK)), "无码流播放也应该设置成功");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "无码流播放CSUDIVIDEOGetStopMode调用应该成功");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_BLACK == eStopMode), "获得的eStopMode不是设置值");

	//停止播放前CSUDIVIDEOSetStopMode,CSUDIVIDEOGetStopMode 希望获得EM_UDIVIDEO_STOPMODE_FREEZE
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == testGetStopMode(hPlayer, EM_UDIVIDEO_STOPMODE_FREEZE), "Step6 fail");

	//关闭播放后CSUDIVIDEOGetStopMode
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	hPlayer = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "关闭播放器后CSUDIVIDEOGetStopMode应该成功");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_FREEZE == eStopMode), "获得的eStopMode不是设置的正确值");
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		if(!bStopFlag)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//预留未使用参数nVideoIndex,以便以后扩展
static CSUDI_BOOL testCheckGetWinSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstSetRect, const CSUDIWinRect_S * pstGetRect )
{
	if(CSUDI_NULL == pstSetRect)
	{
		if(EM_UDI_VOUT_DEVICE_SD == eScreenDevice)
		{
			if(
				(0 != pstGetRect->m_nX)||
				(0 != pstGetRect->m_nY)||
				(g_nFullScreenWidth_SD != pstGetRect->m_nWidth)||
				(g_nFullScreenHeight_SD != pstGetRect->m_nHeight))
			{
				return CSUDI_FALSE;
			}
		}
		else
		{
			if(
				(0 != pstGetRect->m_nX)||
				(0 != pstGetRect->m_nY)||
				((g_nFullScreenWidth_HD != pstGetRect->m_nWidth) && (g_nFullWindowWidth_HD != pstGetRect->m_nWidth))||
				((g_nFullScreenHeight_HD != pstGetRect->m_nHeight) && (g_nFullWindowHeight_HD != pstGetRect->m_nHeight)))
			{
				return CSUDI_FALSE;
			}
		}
	}
	else
	{
		if(pstSetRect->m_nHeight != pstGetRect->m_nHeight)
		{
			return CSUDI_FALSE;
		}
		if(pstSetRect->m_nWidth != pstGetRect->m_nWidth)
		{
			return CSUDI_FALSE;
		}
		if(pstSetRect->m_nX != pstGetRect->m_nX)
		{
			return CSUDI_FALSE;
		}
		if(pstSetRect->m_nY != pstGetRect->m_nY)
		{
			return CSUDI_FALSE;
		}
	}

	return CSUDI_TRUE;
}

static CSUDI_BOOL testSetWindowSize(int nTestID, CSUDISCREENType_E eScreenDevice)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	CSUDISCREENResolution_E enResBak = EM_UDISCREEN_RESOLUTION_INVALID;
	CSUDISCREENType_E enDev = EM_UDI_VOUT_DEVICE_HD;
	CSUDISCREENResolution_E enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSUDIWinRect_S *pstRect = s_aSetWindowSizeTestCase[nTestID].m_sWinRect;
	CSUDIWinRect_S	stGetRect;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDIWinRect_S sFullWinRect;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器");

	if (EM_UDI_VOUT_DEVICE_SD== eScreenDevice)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!");
	}
	else
	{
		CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &enResBak);
		if(enResBak != enRes)
		{
			CSUDISCREENSetResolution(&enDev, &enRes,1);
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "锁频失败!");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "无法打开视频流!");

	//获取全屏视频窗口大小
	memset(&sFullWinRect, 0, sizeof(sFullWinRect));
	CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL);
	CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &sFullWinRect);

	nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, pstRect);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult)||(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult), "CSUDIVIDEOSetWindowSize调用失败");

	if(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "CSUDIVIDEOSetWindowSize not support!!!");
	}
	
	if(CSUDI_SUCCESS == nResult)
	{
		//检测CSUDIVIDEOGetWindowSize
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "CSUDIVIDEOGetWindowSize调用失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, pstRect, &stGetRect), "获得的Rect和设置的不一致");
	}

	if (EM_UDI_VOUT_DEVICE_HD== eScreenDevice)
	{
		CSTCPrint("高清全屏视频窗口为{%d, %d, %d, %d}，请确认高清输出视频画面是否为:\"%s\"!\n", \
				sFullWinRect.m_nX, sFullWinRect.m_nX, sFullWinRect.m_nWidth, sFullWinRect.m_nHeight, s_aSetWindowSizeTestCase[nTestID].m_pcDes);
	}
	else
	{
		CSTCPrint("标清全屏视频窗口为{%d, %d, %d, %d}，请确认标清输出视频画面是否为:\"%s\"!\n",\
				sFullWinRect.m_nX, sFullWinRect.m_nX, sFullWinRect.m_nWidth, sFullWinRect.m_nHeight, s_aSetWindowSizeTestCase[nTestID].m_pcDes);
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "画面与预设不符!");

	if ((EM_UDI_VOUT_DEVICE_HD == eScreenDevice) && (enResBak != enRes))
	{
		CSUDISCREENSetResolution(&enDev, &enResBak,1);
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSize失败!");

	//检测CSUDIVIDEOGetWindowSize
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "CSUDIVIDEOGetWindowSize调用失败!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "获得的Rect和设置的不一致");

	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSize失败!");

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

static CSUDI_BOOL testSetWindowSizeError(int nTestID, CSUDISCREENType_E eScreenDevice)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDIWinRect_S *pstRect = s_aSetWindowSizeTestCase[nTestID].m_sWinRect;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
		   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器!");

	if (EM_UDI_VOUT_DEVICE_SD== eScreenDevice)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "锁频失败!");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "无法打开视频流!");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, pstRect)), "SetWindowSize未失败!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "关闭播放器失败!");
 
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL)), "还原全屏WindowSize失败!");

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect=CSUDI_NULL情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect=CSUDI_NULL
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像全屏显示
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,CSUDI_NULL)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"全屏幕显示"
//@REMARK:
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0001(void)
{
	int nTestID = 0;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect指向数据结构{0,       0,      720/2,  576/2}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect指向数据结构{0,       0,      720/2,  576/2}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕左上角{0, 0, 720/2, 576/2}的缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"位于屏幕左上角，{0, 0, 720/2, 576/2}的缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0002(void)
{
	int nTestID = 1;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect指向数据结构{720/2,576/2,720/3,  576/9}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect指向数据结构{720/2,576/2,720/3,  576/9}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕右下方{720/2, 576/2, 720/3, 576/3}的缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"位于屏幕右下方{720/2, 576/2, 720/3, 576/3}的缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0003(void)
{
	int nTestID = 2;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect指向数据结构{50,     50,     720,     576   }情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect指向数据结构{50,     50,     720,     576   }
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"图像右下方向移位50个像素，移出屏幕的部分被切除"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"图像右下方向移位50个像素，移出屏幕的部分被切除"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0004(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E eScreenDevice= EM_UDI_VOUT_DEVICE_SD;
	CSUDI_Error_Code nResult = CSUDI_FAILURE;

	CSUDIWinRect_S *pstRect = s_aSetWindowSizeTestCase[3].m_sWinRect;
	
	CSUDI_BOOL bResult = CSUDI_FALSE;
		   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!\n");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "无法打开视频流!\n");

	CSTCPrint("请确认标清输出通道上视频是否成功显示:起始坐标为(0,0)，全屏显示？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 5 确认结果失败 !!!!");
		
	nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, pstRect);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult)||(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult), "CSUDIVIDEOSetWindowSize调用失败");

	if(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "CSUDIVIDEOSetWindowSize not support!!!");
	}
	
	CSTCPrint("请确认显示的视频为:图像右下方向移位50个像素，移出屏幕的部分被切除？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 7 确认结果失败 !!!!");
            
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSize失败!\n");

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放器失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect指向数据结构{0,       0,      720/3,  576/3}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect指向数据结构{0,       0,      720/3,  576/3}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕左上角{0, 0, 720/3, 576/3}的缩小图像""
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"位于屏幕左上角{0, 0, 720/3, 576/3}的缩小图像""
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0005(void)
{
	int nTestID = 4;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect指向数据结构{720/2, 0,      720/3,  576/2}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect指向数据结构{720/2, 0,      720/3,  576/2}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕右上方{720/2, 0, 720/3, 576/2}的拉长缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"位于屏幕右上方{720/2, 0, 720/3, 576/2}的拉长缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0006(void)
{
	int nTestID = 5;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect指向数据结构{      0, 576/2,720/2,  576/3}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect指向数据结构{      0, 576/2,720/2,  576/3}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕左下方{0, 576/2, 720/2, 576/3}的压扁缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"位于屏幕左下方{0, 576/2, 720/2, 576/3}的压扁缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0007(void)
{
	int nTestID = 6;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect指向数据结构{720/3,576/3, 720/3,  576/3}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect指向数据结构{720/3,576/3, 720/3,  576/3}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕中间位置{720/3, 576/3, 720/3, 576/3}的缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"位于屏幕中间位置{720/3, 576/3, 720/3, 576/3}的缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0008(void)
{
	int nTestID = 7;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect指向数据结构{-50,   -50,     720,     576   }情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect指向数据结构{-50,   -50,     720,     576   }
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"图像左上方向移位50个像素,移出屏幕的部分被切除"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"图像左上方向移位50个像素,移出屏幕的部分被切除"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0009(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	CSUDISCREENType_E eScreenDevice= EM_UDI_VOUT_DEVICE_SD;

	CSUDIWinRect_S *pstRect = s_aSetWindowSizeTestCase[8].m_sWinRect;

	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "无法打开视频流!\n");

	CSTCPrint("请确认标清输出通道上视频是否成功显示:起始坐标为(0,0)，全屏显示？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 5 确认结果失败 !!!!");

	nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, pstRect);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult)||(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult), "CSUDIVIDEOSetWindowSize调用失败");

	if(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "CSUDIVIDEOSetWindowSize not support!!!");
	}

	CSTCPrint("请确认显示的视频为:图像左上方向移位50个像素,移出屏幕的部分被切除？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 7 确认结果失败 !!!!");
      
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSize失败!\n");

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放器失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在pstRect指向数据结构{720/2, 576/2,     -1,     -1  }情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3、pstRect指向数据结构{720/2, 576/2,     -1,     -1  }
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0010(void)
{
	int nTestID= 9;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSizeError(nTestID, eScreenDevice);
}


//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect=CSUDI_NULL情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect=CSUDI_NULL
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像全屏显示
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,CSUDI_NULL)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认高清输出端口显示的视频为"全屏幕显示"
//@REMARK:
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0011(void)
{
	int nTestID = 10;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect指向数据结构{0,		 0, 	 1920/2,  1080/2}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect指向数据结构{0,		 0, 	 1920/2,  1080/2}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕左上角{0, 0, 1920/2, 1080/2}的缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认高清输出端口显示的视频为"位于屏幕左上角{0, 0, 1920/2, 1080/2}的缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0012(void)
{
	int nTestID = 11;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect指向数据结构{1920/2,1080/2,1920/3,	1080/3}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect指向数据结构{1920/2,1080/2,1920/3,	1080/3}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕右下方{1920/2, 1080/2, 1920/3, 1080/3}的缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认高清输出端口显示的视频为"位于屏幕右下方{1920/2, 1080/2, 1920/3, 1080/3}的缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0013(void)
{
	int nTestID = 12;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect指向数据结构{50, 	50, 	1920,	 1080   }情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect指向数据结构{50, 	50, 	1920,	 1080   }
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"图像右下方向移位50个像素，移出屏幕的部分被切除"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认高清输出端口显示的视频为"图像右下方向移位50个像素，移出屏幕的部分被切除"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0014(void)
{
	int nTestID = 13;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect指向数据结构{0,       0,      1920/3,  1080/3}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect指向数据结构{0,       0,      1920/3,  1080/3}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕左上角{0, 0, 1920/3, 1080/3}的缩小图像""
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认高清输出端口显示的视频为"位于屏幕左上角{0, 0, 1920/3, 1080/3}的缩小图像""
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0015(void)
{
	int nTestID = 14;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect指向数据结构{1920/2, 0,      1920/3,  1080/2}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect指向数据结构{1920/2, 0,      1920/3,  1080/2}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕右上方{1920/2, 0, 1920/3, 1080/2}的拉长缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认高清输出端口显示的视频为"位于屏幕右上方{1920/2, 0, 1920/3, 1080/2}的拉长缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0016(void)
{
	int nTestID = 15;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect指向数据结构{      0, 1080/2,1920/2,  1080/3}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect指向数据结构{      0, 1080/2,1920/2,  1080/3}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕左下方{0, 1080/2, 1920/2, 1080/3}的压扁缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认高清输出端口显示的视频为"位于屏幕左下方{0, 1080/2, 1920/2, 1080/3}的压扁缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0017(void)
{
	int nTestID = 16;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect指向数据结构{1920/3,1080/3, 1920/3,  1080/3}情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect指向数据结构{1920/3,1080/3, 1920/3,  1080/3}
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕中间位置{1920/3, 1080/3, 1920/3, 1080/3}的缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认高清输出端口显示的视频为"位于屏幕中间位置{1920/3, 1080/3, 1920/3, 1080/3}的缩小图像"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0018(void)
{
	int nTestID = 17;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect指向数据结构{-50,   -50,     1920,     1080   }情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect指向数据结构{-50,   -50,     1920,     1080   }
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"图像左上方向移位50个像素,移出屏幕的部分被切除"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认高清输出端口显示的视频为"图像左上方向移位50个像素,移出屏幕的部分被切除"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0019(void)
{
	int nTestID = 18;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWindowSize在pstRect指向数据结构{1920/2, 1080/2,     -1,     -1  }情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3、pstRect指向数据结构{1920/2, 1080/2,     -1,     -1  }
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清输出端口视频成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0020(void)
{
	int nTestID= 19;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSizeError(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在nVideoIndex参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex ={-1,nVideo,nVideo+1}
//@INPUT:2、eScreenDevice =  [EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD]
//@INPUT:3、pstRect=CSUDIWinRect_S结构指针
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取{-1,nVideo,nVideo+1}内的值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetWindowSize，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0021(void)
{
	int nVideoCount = 0;
	int nVideoErrorCount = 3 * 2;
	int nVideoErrorIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	typedef struct 
	{
		int m_nVideoErrorIndex;
		CSUDISCREENType_E m_eScreenDevice;	
	}CSTC_ErrorSetWinSize_S;

	CSTC_ErrorSetWinSize_S s_SetErrorWinSizeType[6];

	memset(s_SetErrorWinSizeType, 0, sizeof(s_SetErrorWinSizeType));

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	s_SetErrorWinSizeType[0].m_nVideoErrorIndex = -1;
	s_SetErrorWinSizeType[0].m_eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	s_SetErrorWinSizeType[1].m_nVideoErrorIndex = -1;
	s_SetErrorWinSizeType[1].m_eScreenDevice = EM_UDI_VOUT_DEVICE_HD;

	s_SetErrorWinSizeType[2].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorWinSizeType[2].m_eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	s_SetErrorWinSizeType[3].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorWinSizeType[3].m_eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	s_SetErrorWinSizeType[4].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorWinSizeType[4].m_eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	s_SetErrorWinSizeType[5].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorWinSizeType[5].m_eScreenDevice = EM_UDI_VOUT_DEVICE_HD;

	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSetWindowSize(s_SetErrorWinSizeType[nVideoErrorIndex].m_nVideoErrorIndex, 
						s_SetErrorWinSizeType[nVideoErrorIndex].m_eScreenDevice, CSUDI_NULL)), "参数判断错误!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在eScreenDevice参数错误情况下的执行结果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex ={0...nVideo-1}
//@INPUT:2、eScreenDevice =  [Error]
//@INPUT:3、pstRect=CSUDIWinRect_S结构指针
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、eScreenDevice取Error 值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetWindowSize，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0022(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDISCREENType_E eScreenDevice = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL)), "参数判断错误!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

static BOOL TCSetWindowSize(CSUDISCREENType_E enDev,CSUDIWinRect_S *pstRect)
{
	BOOL bGetYes ;
	if (CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0, enDev,pstRect))
	{
		if (pstRect)
		{
			CSTCPrint("请确认将%s 输出设置为小窗口(%d,%d,%d,%d),%s维持不变(Y/N)\n",
			enDev == EM_UDI_VOUT_DEVICE_SD?"标清":"高清",
			pstRect->m_nX,pstRect->m_nY,pstRect->m_nWidth,pstRect->m_nHeight,
			enDev == EM_UDI_VOUT_DEVICE_SD?"高清":"标清");
		}
		else
		{
			CSTCPrint("请确认将%s 设置为全屏,%s维持不变(Y/N)\n",
			enDev == EM_UDI_VOUT_DEVICE_SD?"标清":"高清",
			enDev == EM_UDI_VOUT_DEVICE_SD?"高清":"标清");
		}
		bGetYes = CSTKWaitYes();

		return bGetYes;
	}
	else
	{
		if (pstRect)
		{
			CSTCPrint("设置%s 输出设置为(%d,%d,%d,%d) 失败\n",
				enDev == EM_UDI_VOUT_DEVICE_SD?"标清":"高清",
				pstRect->m_nX,pstRect->m_nY,pstRect->m_nWidth,pstRect->m_nHeight);
		}
		else
		{
			CSTCPrint("设置%s 输出设置为全屏 失败\n",
				enDev == EM_UDI_VOUT_DEVICE_SD?"标清":"高清");
		}
	}

	return FALSE;
}

static BOOL TestWindowSizeInArray(CSUDIWinRect_S *pstWinArraySD,int nCountSD,
	CSUDIWinRect_S *pstWinArrayHD,int nCountHD,BOOL bCheckResult)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIWinRect_S *pstWindowSD = pstWinArraySD;
	CSUDIWinRect_S *pstWindowHD = pstWinArrayHD;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");

	while(1)
	{
		if (nCountSD > 0)
		{
			if (CSUDI_SUCCESS != CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,pstWindowSD))
			{
				CSTCPrint("标清设置(%d,%d,%d,%d) 失败\n",
					pstWindowSD->m_nX,pstWindowSD->m_nY,
					pstWindowSD->m_nWidth,pstWindowSD->m_nHeight);
			}
			
			if (bCheckResult)
			{
				CSTCPrint("标清视频窗口为(%d,%d,%d,%d),",
					pstWindowSD->m_nX,pstWindowSD->m_nY,
					pstWindowSD->m_nWidth,pstWindowSD->m_nHeight);
			}
			pstWindowSD++;
			nCountSD--;
		}

		if (nCountHD > 0)
		{
			if(CSUDI_SUCCESS != CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,pstWindowHD))
			{
				CSTCPrint("高清设置(%d,%d,%d,%d) 失败\n",
					pstWindowHD->m_nX,pstWindowHD->m_nY,
					pstWindowHD->m_nWidth,pstWindowHD->m_nHeight);
			}
			if (bCheckResult)
			{
				CSTCPrint("高清视频窗口为(%d,%d,%d,%d),",
					pstWindowHD->m_nX,pstWindowHD->m_nY,
					pstWindowHD->m_nWidth,pstWindowHD->m_nHeight);
			}
			pstWindowHD++;
			nCountHD--;
		}
		
		if (bCheckResult)
		{
			CSTCPrint("输出效果正确,且没有抖动闪烁等异常(Y/N)\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");
		}
		else
		{
			CSUDIOSThreadSleep(80);
		}
		
		if ((nCountSD<= 0) && (nCountHD <= 0))
		{
			break;
		}
	}

	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	if (pstWinArraySD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,NULL),
				"fail to set window size\n");
	}

	if (pstWinArrayHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,NULL),
				"fail to set window size\n");
	}

	return TRUE;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize
//@DESCRIPTION: 测试平台的视频窗口缩小强度测试,从全屏到1*1
//@INPUT:1. nVideoIndex =0
//@EXPECTATION: 视频窗口延对角线逐渐收缩到宽高为1*1时,平台无死机
//@EXECUTIONFLOW: 1. 播放测试流,期望播放成功
//@EXECUTIONFLOW: 2.  如果平台支持高清设置高清分辨率为DISCREEN_RESOLUTION_1080I_50HZ
//@EXECUTIONFLOW: 3. 设置标清输出到小窗口模式,高宽逐渐缩小直到1*1，期望无死机
//@EXECUTIONFLOW: 4. 设置高清输出到小窗口模式,高宽逐渐缩小直到1*1，期望无死机
//@EXECUTIONFLOW: 5. 调用CSUDIPLAYERClose关闭测试播放器
//@EXECUTIONFLOW: 6.  调用CSUDIVIDEOSetWindowSize设置高标清到全屏,恢复测试现场
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0023(void)
{
	BOOL bRet = CSUDI_FALSE;
	CSUDIWinRect_S *pstRectSD;
	CSUDIWinRect_S *pstRectHD;
	int nHDCnt,nSDCnt;
	int i=0;
	int nWidthStep,nWeightStep;

	nSDCnt = 720/50;
	nHDCnt = nSDCnt;
	pstRectSD = CSUDIOSMalloc(sizeof(CSUDIWinRect_S)*nSDCnt);
	pstRectHD = CSUDIOSMalloc(sizeof(CSUDIWinRect_S)*nHDCnt);
	CSTK_ASSERT_TRUE_FATAL(pstRectSD != NULL && pstRectHD != NULL,"malloc memory fail\n");

	nWidthStep = 720/nSDCnt;
	nWeightStep = 576/nSDCnt;
	for(i=0;i<nSDCnt;i++)
	{
		pstRectSD[i].m_nX = 0;
		pstRectSD[i].m_nY = 0;
		pstRectSD[i].m_nWidth = 720 - i*nWidthStep;
		pstRectSD[i].m_nHeight = 576 - i*nWeightStep;
	}
	/*最后一个宽高为0,调整下为1*/
	pstRectSD[nSDCnt-1].m_nWidth = 1;
	pstRectSD[nSDCnt-1].m_nHeight = 1;

	if (bIsSupportHD())
	{
		CSUDISCREENType_E enDev = EM_UDI_VOUT_DEVICE_HD;
		CSUDISCREENResolution_E enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSUDISCREENSetResolution(&enDev, &enRes,1);
	}

	if (bIsSupportHD())
	{
		nWidthStep = 1920/nHDCnt;
		nWeightStep = 1080/nHDCnt;
		for(i=0;i<nHDCnt;i++)
		{
			pstRectHD[i].m_nX = 0;
			pstRectHD[i].m_nY = 0;
			pstRectHD[i].m_nWidth = 1920 - i*nWidthStep;
			pstRectHD[i].m_nHeight = 1080 - i*nWeightStep;
		}
		/*最后一个宽高为0,调整下为1*/
		pstRectHD[nHDCnt-1].m_nWidth = 1;
		pstRectHD[nHDCnt-1].m_nHeight = 1;
		
		bRet = TestWindowSizeInArray(pstRectSD,nSDCnt,pstRectHD,nHDCnt,FALSE);
	}
	else
	{
		bRet = TestWindowSizeInArray(pstRectSD,nSDCnt,NULL,0,FALSE);
	}

	CSTK_FATAL_POINT;

	if (pstRectSD)
	{
		CSUDIOSFree(pstRectSD);
	}

	if (pstRectHD)
	{
		CSUDIOSFree(pstRectHD);
	}

	return bRet;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize
//@DESCRIPTION:  测试平台的视频窗口单独水平方向上和垂直方向上的缩小强度测试,从全屏到width*1,1*weight
//@INPUT:1. nVideoIndex =0
//@EXPECTATION: 视频窗口沿水平方向上逐渐收缩到1*576(SD)和1*1080(HD),无死机现象
//@EXPECTATION: 视频窗口沿垂直方向上逐渐收缩到720*1(SD)和1920*1(HD),无死机现象
//@EXECUTIONFLOW: 1. 播放测试流,期望播放成功
//@EXECUTIONFLOW: 2.  如果平台支持高清设置高清分辨率为DISCREEN_RESOLUTION_1080I_50HZ
//@EXECUTIONFLOW: 3. 设置标清输出到小窗口模式,宽沿垂直方向上逐渐缩小直到1*576，期望无死机
//@EXECUTIONFLOW: 3. 设置高清输出到小窗口模式,宽沿垂直方向上逐渐缩小直到1*1080，期望无死机
//@EXECUTIONFLOW: 3. 设置标清输出到小窗口模式,宽沿水平方向上逐渐缩小直到720*1，期望无死机
//@EXECUTIONFLOW: 3. 设置高清输出到小窗口模式,宽沿垂直方向上逐渐缩小直到1920*1，期望无死机
//@EXECUTIONFLOW: 5. 调用CSUDIPLAYERClose关闭测试播放器
//@EXECUTIONFLOW: 6.  调用CSUDIVIDEOSetWindowSize设置高标清到全屏,恢复测试现场
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0024(void)
{
	BOOL bRet = CSUDI_FALSE;
	CSUDIWinRect_S *pstRectSD;
	CSUDIWinRect_S *pstRectHD;
	int nHDCnt,nSDCnt;
	int i=0;
	int nWidthStep,nWeightStep;

	nSDCnt = 720/50;
	nHDCnt = nSDCnt;
	pstRectSD = CSUDIOSMalloc(sizeof(CSUDIWinRect_S)*nSDCnt);
	pstRectHD = CSUDIOSMalloc(sizeof(CSUDIWinRect_S)*nHDCnt);
	CSTK_ASSERT_TRUE_FATAL(pstRectSD != NULL && pstRectHD != NULL,"malloc memory fail\n");

	nWidthStep = 720/nSDCnt;
	nWeightStep = 576/nSDCnt;
	for(i=0;i<nSDCnt;i++)
	{
		pstRectSD[i].m_nX = 0;
		pstRectSD[i].m_nY = 0;
		pstRectSD[i].m_nWidth = 720 - i*nWidthStep;
		pstRectSD[i].m_nHeight = 576;
	}
	/*最后一个宽高为0,调整下为1*/
	pstRectSD[nSDCnt-1].m_nWidth = 1;

	if (bIsSupportHD())
	{
		CSUDISCREENType_E enDev = EM_UDI_VOUT_DEVICE_HD;
		CSUDISCREENResolution_E enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		CSUDISCREENSetResolution(&enDev, &enRes,1);
	}

	if (bIsSupportHD())
	{
		nWidthStep = 1920/nHDCnt;
		nWeightStep = 1080/nHDCnt;
		for(i=0;i<nHDCnt;i++)
		{
			pstRectHD[i].m_nX = 0;
			pstRectHD[i].m_nY = 0;
			pstRectHD[i].m_nWidth = 1920 - i*nWidthStep;
			pstRectHD[i].m_nHeight = 1080;
		}
		/*最后一个宽高为0,调整下为1*/
		pstRectHD[nHDCnt-1].m_nWidth = 1;
		
		bRet = TestWindowSizeInArray(pstRectSD,nSDCnt,pstRectHD,nHDCnt,FALSE);
	}
	else
	{
		bRet = TestWindowSizeInArray(pstRectSD,nSDCnt,NULL,0,FALSE);
	}

	nWeightStep = 576/nSDCnt;
	for(i=0;i<nSDCnt;i++)
	{
		pstRectSD[i].m_nWidth = 720;
		pstRectSD[i].m_nHeight = 576 - i*nWeightStep;
	}
	/*最后一个宽高为0,调整下为1*/
	pstRectSD[nSDCnt-1].m_nHeight = 1;

	if (bIsSupportHD())
	{
		nWeightStep = 1080/nHDCnt;
		for(i=0;i<nHDCnt;i++)
		{
			pstRectHD[i].m_nWidth = 1920;
			pstRectHD[i].m_nHeight = 1080 - i*nWeightStep;
		}
		/*最后一个宽高为0,调整下为1*/
		pstRectHD[nHDCnt-1].m_nHeight = 1;
		
		bRet = TestWindowSizeInArray(pstRectSD,nSDCnt,pstRectHD,nHDCnt,FALSE);
	}
	else
	{
		bRet = TestWindowSizeInArray(pstRectSD,nSDCnt,NULL,0,FALSE);
	}

	CSTK_FATAL_POINT;

	if (pstRectSD)
	{
		CSUDIOSFree(pstRectSD);
	}

	if (pstRectHD)
	{
		CSUDIOSFree(pstRectHD);
	}

	return bRet;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize
//@DESCRIPTION: 测试高标清可以分开设置窗口大小,两者相互不影响
//@PRECONDITION:平台支持高清解码和高清输出
//@PRECONDITION:高标清不同源
//@INPUT:1. nVideoIndex =0
//@INPUT:2. SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts
//@EXPECTATION: 高清和标清分别进行窗口设置时不相互影响
//@EXECUTIONFLOW: 1. 如果平台不支持高清则直接返回TRUE,作为测试成功，判断方法是平台支持mpeg4或者是h.264的解码
//@EXECUTIONFLOW: 2. 如果平台高标清同源，直接返回TRUE,测试成功
//@EXECUTIONFLOW: 3. 播放测试流SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts中的高清节目,期望成功并显示出来
//@EXECUTIONFLOW: 4. 设置高清分辨为EM_UDISCREEN_RESOLUTION_1080I_50HZ,标清为EM_UDISCREEN_RESOLUTION_PAL,期望返回成功
//@EXECUTIONFLOW: 5. 调用CSUDIVIDEOSetWindowSize设置标清视频窗口到{0,0,720/2,576/2},期望标清输出到左上角1/2处,高清输出无变化
//@EXECUTIONFLOW: 6. 调用CSUDIVIDEOSetWindowSize设置标清视频窗口到{0,0,1920/2,1080/2},期望标清输出到左上角1/2处,高清输出无变化
//@EXECUTIONFLOW: 7. 调用CSUDIVIDEOSetWindowSize设置高标清到全屏,恢复测试现场
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0025(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDISCREENType_E enDev[]={EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E enRes[]={EM_UDISCREEN_RESOLUTION_PAL,EM_UDISCREEN_RESOLUTION_1080I_50HZ};
	CSUDIWinRect_S stWinRectSD = {0,0,720/2,576/2};
	CSUDIWinRect_S stWinRectHD = {0,0,1920/2,1080/2};
	
	/*如果平台不支持高清解码则直接返回测试成功*/
	if (!bIsSupportHD())
	{
		return TRUE;
	}

	if(CSUDI_FALSE == CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "锁频失败!\n");

		CSUDISCREENSetResolution(enDev, enRes, 2);

		CSTK_ASSERT_TRUE_FATAL(TCSetWindowSize(EM_UDI_VOUT_DEVICE_SD,&stWinRectSD),"设置标清到小窗口失败\n");
		CSTK_ASSERT_TRUE_FATAL(TCSetWindowSize(EM_UDI_VOUT_DEVICE_HD,&stWinRectHD),"设置高清到小窗口失败\n");
		CSTK_ASSERT_TRUE_FATAL(TCSetWindowSize(EM_UDI_VOUT_DEVICE_SD,NULL),"设置标清到小窗口失败\n");
		CSTK_ASSERT_TRUE_FATAL(TCSetWindowSize(EM_UDI_VOUT_DEVICE_HD,NULL),"设置高清到小窗口失败\n");

		bResult = CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("高标清同源!\n");
	}
	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
	
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWindowSize在解码器打开前设置是否成功
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT:1、nVideoIndex =  [0..nVideo-1]
//@INPUT:2、stRect = {0,0,720/2,576/2} (高清:{0,0,1920/2,1080/2})
//@EXPECTATION: 返回CSUDI_SUCCESS,视频图像显示为:"位于屏幕左上角，1/4屏幕大小的缩小图像"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、获取支持播放节目类型的nVideoIndex
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、在nVideoIndex设备上循环播放一个段视频数据
//@EXECUTIONFLOW: 6、调用CSTKWaitYesKey，测试人员确认视频成功显示出来
//@EXECUTIONFLOW: 7、调用CSTKWaitYesKey，测试人员确认显示的视频为"位于屏幕左上角，1/4屏幕大小的缩小图像"
//@EXECUTIONFLOW: 8、调用CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,CSUDI_NULL)恢复全屏显示，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、如果支持高清输出，重复步骤3-8
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0026(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_Error_Code nResult = CSUDI_FAILURE;
	CSUDISCREENResolution_E enResBak = EM_UDISCREEN_RESOLUTION_INVALID;
	CSUDISCREENType_E enDev = EM_UDI_VOUT_DEVICE_HD;
	CSUDISCREENResolution_E enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDIWinRect_S stWinRectSD = {0,0,720/2,576/2};
	CSUDIWinRect_S stWinRectHD = {0,0,1920/2,1080/2};
	CSUDIWinRect_S	stGetRect;
	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器");

	if(!CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

		nVideoIndex = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_MPEG2);

		nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, &stWinRectSD);

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "步骤4:(SD)CSUDIVIDEOSetWindowSize调用失败");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "步骤5:(SD)锁频播放失败!");

		if(CSUDI_SUCCESS == nResult)
		{
			//检测CSUDIVIDEOGetWindowSize
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "(SD)CSUDIVIDEOGetWindowSize调用获取1/4屏幕大小失败!");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stWinRectSD, &stGetRect), "(SD)获得的Rect和设置的1/4屏幕大小不一致");
		}

		CSTCPrint("请将标清输出端口连接标清电视，并确定视频画面为:位于屏幕左上角，1/4屏幕大小的缩小图像!\n");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤7:(SD)画面与预设不符!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "(SD)SetWindowSize设置全屏失败!");

		//检测CSUDIVIDEOGetWindowSize
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "(SD)CSUDIVIDEOGetWindowSize调用获取全屏大小失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "(SD)获得的Rect和设置的全屏大小不一致");

		if(CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
		}
		hPlayer = CSUDI_NULL;
	}
	
	if(bIsSupportHD())
	{
		CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;

		nVideoIndex = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_H264);

		nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, &stWinRectHD);

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "步骤4:(HD)CSUDIVIDEOSetWindowSize调用失败");

		CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &enResBak);
		if(enResBak != enRes)
		{
			CSUDISCREENSetResolution(&enDev, &enRes,1);
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "步骤5:(HD)锁频播放失败!");
		

		if(CSUDI_SUCCESS == nResult)
		{
			//检测CSUDIVIDEOGetWindowSize
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "(HD)CSUDIVIDEOGetWindowSize调用获取1/4屏幕大小失败!");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stWinRectHD, &stGetRect), "(HD)获得的Rect和设置的1/4屏幕大小不一致");
		}

		CSTCPrint("请将高清输出端口连接高清电视，并确定视频画面为:位于屏幕左上角，1/4屏幕大小的缩小图像!\n");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤7:(HD)画面与预设不符!");

		if (enResBak != enRes)
		{
			CSUDISCREENSetResolution(&enDev, &enResBak,1);
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "(HD)SetWindowSize设置全屏失败!");

		//检测CSUDIVIDEOGetWindowSize
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "(HD)CSUDIVIDEOGetWindowSize调用获取全屏大小失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "(HD)获得的Rect和设置的全屏大小不一致");

	}

	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;

	if(!CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "(SD)SetWindowSize恢复全屏窗口失败!");
	}
	
	if(bIsSupportHD())
	{
		eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "(HD)SetWindowSize恢复全屏窗口失败!");
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize
//@DESCRIPTION: 播放高清16:9的节目时,测试画面比设置只在全屏时起效果,小窗口时要求全屏
//@INPUT:1. nVideoIndex =0
//@INPUT:2. SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts
//@EXPECTATION: letter_box全屏生效，小窗口仍为全屏
//@EXECUTIONFLOW: 1. 如果平台不支持高清解码则直接返回,判断方法是测试配置文件中
//@EXECUTIONFLOW: 2. 播放测试流SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts,期望成功并显示出来
//@EXECUTIONFLOW: 4. 调用CSUDIVIDEOSetMatchMethod设置为letter_box的效果,期望标清输出屏幕上下被压缩,上下有黑边
//@EXECUTIONFLOW: 5. 设置视频小窗口,并把小窗口四周的OSG用红色填充,期望标清屏幕上出现全屏红色OSG背景以及视频小窗口且视频小窗口仍为全屏显示
//@EXECUTIONFLOW: 6. 调用CSUDIVIDEOSetWindowSize设置高标清到全屏,恢复测试现场
CSUDI_BOOL CSTC_VIDEO_IT_WindowSizeMatchMethod_0001(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIWinRect_S stWinRect = {180,200,720/2,576/2};
	CSUDI_HANDLE hDisSurface = CSUDI_NULL;
	CSUDIOSGRect_S stRectOSD;
	
	stRectOSD.m_nX = stWinRect.m_nX;
	stRectOSD.m_nY = stWinRect.m_nY;
	stRectOSD.m_nWidth = stWinRect.m_nWidth;
	stRectOSD.m_nHeight = stWinRect.m_nHeight;
	
	/*如果平台不支持高清解码则直接返回测试成功*/
	if (!bIsSupportHD())
	{
		CSTCPrint("该平台不支持高清解码\n");
		return CSUDI_FALSE;
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "锁频失败!\n");

	/*高标清同源，只在高清上操作*/
	if(CSUDI_TRUE == CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		CSTC_VIDEO_iWinRect_SDTransformHD(&stWinRect);
		CSTC_VIDEO_iOSGRect_SDTransformHD(&stRectOSD);
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
			"fail to set HD to letter box\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_4_3), 
			"fail to set HD to 4:3");

		CSTCPrint("letter box 模式下16:9的节目源在4:3的标清输出上\n");
		CSTCPrint("标清输出屏幕上下被压缩,上下有黑边(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认标清输出letter box 全屏效果失败\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisSurface),
			"fail to get hd surface by CSUDIOSGGetDisplaySurface\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurface, CSUDI_NULL, 0xFFFF0000),
			"fail to fill hd display surface\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurface, &stRectOSD, 0x0),
			"fail to fill hd display surface\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIVIDEOSetWindowSize(0, EM_UDI_VOUT_DEVICE_HD,&stWinRect),
			"fail to set hd window by CSUDIVIDEOSetWindowSize\n");
		CSTCPrint("请确认标清屏幕上出现全屏红色OSG背景以及视频小窗口(视频相应缩小),视频小窗口四周刚好紧贴红色OSG,四周无黑边(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认画面比在小窗口模式不起作用失败\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
			"fail to set SD to letter box\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_4_3), 
			"fail to set SD to 4:3");

		CSTCPrint("letter box 模式下16:9的节目源在4:3的标清输出上\n");
		CSTCPrint("标清输出屏幕上下被压缩,上下有黑边(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认标清输出letter box 全屏效果失败\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDisSurface),
			"fail to get sd surface by CSUDIOSGGetDisplaySurface\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurface, CSUDI_NULL, 0xFFFF0000),
			"fail to fill sd display surface\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurface, &stRectOSD, 0x0),
			"fail to fill sd display surface\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIVIDEOSetWindowSize(0, EM_UDI_VOUT_DEVICE_SD, &stWinRect),
			"fail to set sd window by CSUDIVIDEOSetWindowSize\n");
		CSTCPrint("请确认标清屏幕上出现全屏红色OSG背景以及视频小窗口(视频相应缩小),视频小窗口四周刚好紧贴红色OSG,四周无黑边(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认画面比在小窗口模式不起作用失败\n");
	}

	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	if(CSUDI_TRUE == CSTC_VIDEO_IsShareHDDisplaySurface())
	{

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0, EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL),
			"fail to set HD window by CSUDIVIDEOSetWindowSize\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_IGNORE),
			"fail to set HD to letter box\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0, EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL),
			"fail to set SD window by CSUDIVIDEOSetWindowSize\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_IGNORE),
			"fail to set SD to letter box\n");
	}
	
	if (hDisSurface)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurface, CSUDI_NULL, 0x0),
				"fail to fill sd display surface\n");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize
//@DESCRIPTION: 播放标清4:3的节目时,测试设置letter_box只在全屏时起效果,小窗口时要求全屏
//@INPUT:1. nVideoIndex =0
//@INPUT:2. Player_PAL_NTSC
//@EXPECTATION: letter_box全屏生效，小窗口仍为全屏
//@EXECUTIONFLOW: 1. 如果平台不支持高清输出或高清解码则直接返回,判断方法是测试配置文件中
//@EXECUTIONFLOW: SCREEN字段中的EM_UDI_VOUT_DEVICE_HD=1
//@EXECUTIONFLOW: 2. 播放测试流Audio&Video Test_27Mbps_20070524.ts中的还珠格格,期望成功并显示出来
//@EXECUTIONFLOW: 3. 设置高清分辨率为EM_UDISCREEN_RESOLUTION_1080I_50HZ,期望返回成功
//@EXECUTIONFLOW: 4. 调用CSUDIVIDEOSetMatchMethod设置为letter_box的效果,期望高清输出屏幕视频不变形,左右有黑边
//@EXECUTIONFLOW: 5. 设置视频小窗口,并把小窗口四周的OSG用红色填充,期望高清屏幕上出现全屏红色OSG背景以及视频小窗口且视频小窗口仍为全屏显示
//@EXECUTIONFLOW: 6. 调用CSUDIVIDEOSetWindowSize设置高标清到全屏,恢复测试现场
CSUDI_BOOL CSTC_VIDEO_IT_WindowSizeMatchMethod_0002(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIWinRect_S stWinRectHD = {180,200,720/2,576/2};
	CSUDI_HANDLE hDisSurfaceHD = CSUDI_NULL;
	CSUDIOSGRect_S stRectOSD;
	CSUDISCREENType_E enDev = EM_UDI_VOUT_DEVICE_HD;
	CSUDISCREENResolution_E enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	
	stRectOSD.m_nX = stWinRectHD.m_nX;
	stRectOSD.m_nY = stWinRectHD.m_nY;
	stRectOSD.m_nWidth = stWinRectHD.m_nWidth;
	stRectOSD.m_nHeight = stWinRectHD.m_nHeight;

	/*如果平台不支持高清输出则直接返回测试成功*/
	if(CSUDI_FALSE == CSTC_VIDEO_IsSupportHdDispaly())
	{
		CSTCPrint("该平台不支持高清输出\n");
		return CSUDI_FALSE;
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");

	CSTC_VIDEO_iWinRect_SDTransformHD(&stWinRectHD);
	CSTC_VIDEO_iOSGRect_SDTransformHD(&stRectOSD);

	CSUDISCREENSetResolution(&enDev,&enRes, 1);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
		"fail to set HD to letter box\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_16_9), 
		"fail to set HD to 16:9");
	
	CSTCPrint("letter box 模式下4:3的节目源在16:9的高清输出上\n");
	CSTCPrint("高清输出屏幕视频不变形,左右有黑边(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认标清输出letter box 全屏效果失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisSurfaceHD),
		"fail to get hd surface by CSUDIOSGGetDisplaySurface\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurfaceHD,NULL,0xFFFF0000),
		"fail to fill hd display surface\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurfaceHD,&stRectOSD,0x0),
		"fail to fill hd display surface\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIVIDEOSetWindowSize(0, EM_UDI_VOUT_DEVICE_HD,&stWinRectHD),
		"fail to set sd window by CSUDIVIDEOSetWindowSize\n");
	CSTCPrint("请确认高清屏幕上出现全屏红色OSG背景以及视频小窗口(视频相应缩小),视频小窗口四周刚好紧贴红色OSG,四周无黑边(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认画面比在小窗口模式不起作用失败\n");

	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(0, EM_UDI_VOUT_DEVICE_HD,NULL),
			"fail to set hd window by CSUDIVIDEOSetWindowSize\n");
	if (hDisSurfaceHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurfaceHD,NULL,0x0),
				"fail to fill hd display surface\n");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0,EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_IGNORE),
		"fail to set HD to letter box\n");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOGetWindowSize
//@DESCRIPTION: 测试CSUDIVIDEOGetWindowSize 接口对错误参数的检测能力
//@INPUT:1、nVideoIndex =  非法
//@INPUT:2、eScreenDevice = 非法
//@INPUT:3.	  pstRect = 非法	
//@EXPECTATION:对该接口传递错误参数，不应该返回成功
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount 获得平台video 的数量
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetWindowSize 传递错误参数，希望返回CSUDIVIDEO_ERROR_BAD_PARAMETER
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetWindowSize_0001(void)
{
	CSUDISCREENType_E 			eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDIWinRect_S				stRect;
	int							nVideoCount = 0;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	int							i = 0;

	typedef struct _ErrorParam_GetWindowSize_S
	{
		int 							m_nVideoIndex;
		CSUDISCREENType_E			m_eScreenDevice;
		CSUDIWinRect_S				*m_PstRect;
		
	}ErrorParam_GetWindowSize_S;

	ErrorParam_GetWindowSize_S ErrorParamList[5];

	memset(ErrorParamList, 0, sizeof(ErrorParamList));

	// 获得 video 的数量
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获得video 数量失败");
	ErrorParamList[0].m_nVideoIndex = -1;
	ErrorParamList[0].m_eScreenDevice = eScreenDevice;
	ErrorParamList[0].m_PstRect = &stRect;

	ErrorParamList[1].m_nVideoIndex = nVideoCount;
	ErrorParamList[1].m_eScreenDevice = eScreenDevice;
	ErrorParamList[1].m_PstRect = &stRect;

	ErrorParamList[2].m_nVideoIndex = nVideoCount;
	ErrorParamList[2].m_eScreenDevice = -1;
	ErrorParamList[2].m_PstRect = &stRect;
	
	ErrorParamList[3].m_nVideoIndex = nVideoCount;
	ErrorParamList[3].m_eScreenDevice = EM_UDI_VOUT_DEVICE_HD+1;
	ErrorParamList[3].m_PstRect = &stRect;

	ErrorParamList[4].m_nVideoIndex = nVideoCount;
	ErrorParamList[4].m_eScreenDevice = eScreenDevice;
	ErrorParamList[4].m_PstRect = CSUDI_NULL;

	//检测错误参数
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetWindowSize(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eScreenDevice, ErrorParamList[i].m_PstRect);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "检测错误参数失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}



static CSUDI_BOOL testSetWinClipError(int nTestID, CSUDISCREENType_E eScreenDevice)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;	   
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	// 1-2
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!\n");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	// 3
	if (EM_UDI_VOUT_DEVICE_SD== eScreenDevice)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "锁频失败!\n");
	}

	// 4
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "打开视频流失败\n");

	// 5-6
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, s_aWinClipWindowSizeTestCase[nTestID].m_sWinRect), "SetWindowSize失败!\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, s_aWinClipWindowSizeTestCase[nTestID].m_sWinClip), "SetWinClip失败!\n");

	// 7
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSize失败!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWinClip失败!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放器失败!");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}
		
	return bResult;
}


//预留未使用参数nVideoIndex,以便以后扩展
static CSUDI_BOOL testCheckGetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstSetRect, const CSUDIWinRect_S * pstGetRect)
{
	if(CSUDI_NULL == pstSetRect)
	{
		if(EM_UDI_VOUT_DEVICE_SD == eScreenDevice)
		{
			if(
				(0 != pstGetRect->m_nX)||
				(0 != pstGetRect->m_nY)||
				(g_nFullScreenWidth_SD != pstGetRect->m_nWidth)||
				(g_nFullScreenHeight_SD != pstGetRect->m_nHeight))
			{
				return CSUDI_FALSE;
			}
		}
		else
		{
			if(
				(0 != pstGetRect->m_nX)||
				(0 != pstGetRect->m_nY)||
				((g_nFullScreenWidth_HD != pstGetRect->m_nWidth) && (g_nFullWindowWidth_HD != pstGetRect->m_nWidth))||
				((g_nFullScreenHeight_HD != pstGetRect->m_nHeight) && (g_nFullWindowHeight_HD != pstGetRect->m_nHeight)))
			{
				return CSUDI_FALSE;
			}
		}
	}
	else
	{
		if(pstSetRect->m_nHeight != pstGetRect->m_nHeight)
		{
			return CSUDI_FALSE;
		}
		if(pstSetRect->m_nWidth != pstGetRect->m_nWidth)
		{
			return CSUDI_FALSE;
		}
		if(pstSetRect->m_nX != pstGetRect->m_nX)
		{
			return CSUDI_FALSE;
		}
		if(pstSetRect->m_nY != pstGetRect->m_nY)
		{
			return CSUDI_FALSE;
		}
	}

	return CSUDI_TRUE;
}


static CSUDI_BOOL testSetWinClip(int nTestID, CSUDISCREENType_E eScreenDevice)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;	   
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIWinRect_S	stGetRect;

	// 1-2
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "获取Video数量失败!\n");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");

	// 3
	if (EM_UDI_VOUT_DEVICE_SD== eScreenDevice)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "锁频失败!\n");
	}
    
	// 4
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "打开视频流失败\n");

	// 5-7
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, s_aWinClipWindowSizeTestCase[nTestID].m_sWinRect), "SetWindowSize失败!\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, s_aWinClipWindowSizeTestCase[nTestID].m_sWinClip), "SetWinClip失败!\n");

	//检测CSUDIVIDEOGetWinClip
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect), "CSUDIVIDEOGetWinClip调用失败!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == testCheckGetWinClip(
		nVideoIndex, 
		eScreenDevice, 
		s_aWinClipWindowSizeTestCase[nTestID].m_sWinClip, 
		&stGetRect)), "获得的Rect和设置的不一致\n");
	
	// 8
	if (EM_UDI_VOUT_DEVICE_HD == eScreenDevice)
	{
       	CSTCPrint("请将高清输出端口连接高清电视，并确定视频画面为:\"%s\"!\n",s_aWinClipWindowSizeTestCase[nTestID].m_pcDes);
	}
	else
	{
       	CSTCPrint("请将标清输出端口连接标清电视，并确定视频画面为:\"%s\"!\n",s_aWinClipWindowSizeTestCase[nTestID].m_pcDes);
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "画面与预设不符\n");

	// 9
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSize失败!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWinClip失败!\n");

	//检测CSUDIVIDEOGetWinClip
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect), "CSUDIVIDEOGetWinClip调用失败!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == testCheckGetWinClip(
		nVideoIndex, 
		eScreenDevice, 
		CSUDI_NULL, 
		&stGetRect)), "获得的Rect和设置的不一致\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放器失败!");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}
		
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip窗口参数为CSUDI_NULL，CSUDIVIDEOSetWindowSize窗口参数为{0,0,720/2, 576/2}情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针为CSUDI_NULL
//@INPUT: WindowSize 的窗口指针指向{0,0,720/2, 576/2}
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:全屏图像被压缩为1/4屏幕大小，位于屏幕左上角显示
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示效果为:"全屏图像被压缩为1/4屏幕大小，位于屏幕左上角显示"
//@EXECUTIONFLOW: 9、恢复现场
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0001(void)
{
	int nTestID = 0;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip窗口参数为{0,0,720/2, 576/2}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{0,0,720/2, 576/2}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:左上角1/4屏幕大小的图像被放大到全屏显示
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示效果为:"左上角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0002(void)
{
	int nTestID = 1;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip窗口参数为{720/2, 576/2,720/2, 576/2}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{720/2, 576/2,720/2, 576/2}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice = EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:右下角1/4屏幕大小的图像被放大到全屏显示
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示效果为:"右下角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0003(void)
{
	int nTestID = 2;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip窗口参数为{0, 576/2,720/2, 576/2}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{0, 576/2,720/2, 576/2}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:"左下角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示效果为:"左下角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0004(void)
{
	int nTestID = 3;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}


//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip窗口参数为{720/2, 0,720/2, 576/2}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的执行
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{720/2, 0,720/2, 576/2}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:"右上角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示效果为:"右上角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0005(void)
{
	int nTestID = 4;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip窗口参数为{720/3, 576/3,720/3, 576/3}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{720/3, 576/3,720/3, 576/3}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:"屏幕正中心1/9屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示效果为:"屏幕正中心1/9屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0006(void)
{
	int nTestID = 5;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip窗口参数为{720/3, 576/3,720/3, 576/3}，CSUDIVIDEOSetWindowSize窗口参数为{2*720/3, 2*576/3,720/3, 576/3}情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{720/3, 576/3,720/3, 576/3}
//@INPUT: WindowSize 的窗口指针指向{2*720/3, 2*576/3,720/3, 576/3}
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:"屏幕正中心1/9屏幕大小的图像被剪切后，原样显示在屏幕右下角"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认视频显示效果为:"屏幕正中心1/9屏幕大小的图像被剪切后，原样显示在屏幕右下角"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0007(void)
{
	int nTestID = 6;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip窗口参数为{720/2, 576/2, 720/2, -1  }情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex [0...nVideo-1]
//@INPUT: WinClip 的窗口指针指向{720/2, 576/2, 720/2, -1  },非法。
//@INPUT:eScreenDevice = EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetWinClip，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:剪切窗口的宽高不能小于或等于0
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0008(void)
{
	int nTestID = 7;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClipError(nTestID, eScreenDevice);
}


//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWinClip窗口参数为CSUDI_NULL，CSUDIVIDEOSetWindowSize窗口参数为{0,0,1920/2, 1080/2}情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针为CSUDI_NULL
//@INPUT: WindowSize 的窗口指针指向{0,0,1920/2, 1080/2}
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:全屏图像被压缩为1/4屏幕大小，位于屏幕左上角显示
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清端口视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认高清端口视频显示效果为:"全屏图像被压缩为1/4屏幕大小，位于屏幕左上角显示"
//@EXECUTIONFLOW: 9、恢复现场
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0009(void)
{
	int nTestID = 8;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWinClip窗口参数为{0,0,1920/2, 1080/2}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{0,0,1920/2, 1080/2}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:左上角1/4屏幕大小的图像被放大到全屏显示
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清端口视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认高清端口视频显示效果为:"左上角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0010(void)
{
	int nTestID = 9;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWinClip窗口参数为{1920/2, 1080/2,1920/2, 1080/2}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{1920/2, 1080/2,1920/2, 1080/2}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:右下角1/4屏幕大小的图像被放大到全屏显示
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清端口视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认高清端口视频显示效果为:"右下角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0011(void)
{
	int nTestID = 10;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWinClip窗口参数为{0, 1080/2,1920/2, 1080/2}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{0, 1080/2,1920/2, 1080/2}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:"左下角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清端口视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认高清端口视频显示效果为:"左下角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0012(void)
{
	int nTestID = 11;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}


//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWinClip窗口参数为{1920/2, 0,1920/2, 1080/2}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的执行
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{1920/2, 0,1920/2, 1080/2}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:"右上角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清端口视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认高清端口视频显示效果为:"右上角1/4屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0013(void)
{
	int nTestID = 12;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWinClip窗口参数为{1920/3, 1080/3,1920/3, 1080/3}，CSUDIVIDEOSetWindowSize窗口参数为CSUDI_NULL情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{1920/3, 1080/3,1920/3, 1080/3}
//@INPUT: WindowSize 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:"屏幕正中心1/9屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清端口视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认高清端口视频显示效果为:"屏幕正中心1/9屏幕大小的图像被放大到全屏显示"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0014(void)
{
	int nTestID = 13;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWinClip窗口参数为{1920/3, 1080/3,1920/3, 1080/3}，CSUDIVIDEOSetWindowSize窗口参数为{2*1920/3, 2*1080/3,1920/3, 1080/3}情况下的显示效果
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为合法的视频解码器索引
//@INPUT: WinClip 的窗口指针指向{1920/3, 1080/3,1920/3, 1080/3}
//@INPUT: WindowSize 的窗口指针指向{2*1920/3, 2*1080/3,1920/3, 1080/3}
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: 返回CSUDI_SUCCESS，屏幕显示效果为:"屏幕正中心1/9屏幕大小的图像被剪切后，原样显示在屏幕右下角"
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSTKWaitYesKey，测试人员确认高清端口视频已成功显示出来
//@EXECUTIONFLOW: 6、调用CSUDIVIDEOSetWinClip，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWindowSize，检查返回值是否为CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSTKWaitYesKey，测试人员确认高清端口视频显示效果为:"屏幕正中心1/9屏幕大小的图像被剪切后，原样显示在屏幕右下角"
//@EXECUTIONFLOW: 9、恢复现场
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0015(void)
{
	int nTestID = 14;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@DESCRIPTION: 测试高清输出端口，CSUDIVIDEOSetWinClip窗口参数为{1920/2, 1080/2, 1920/2, -1  }情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex [0...nVideo-1]
//@INPUT: WinClip 的窗口指针指向{1920/2, 1080/2, 1920/2, -1  },非法。
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]内的值
//@EXECUTIONFLOW: 3、在nVideoIndex解码器上播放视频数据流
//@EXECUTIONFLOW: 4、调用CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)，打开视频显示
//@EXECUTIONFLOW: 5、调用CSUDIVIDEOSetWinClip，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:剪切窗口的宽高不能小于或等于0
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0016(void)
{
	int nTestID = 15;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClipError(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip在视频解码器索引参数错误情况下的执行情况
//@PRECONDITION:[-1.nVideo,nVideo+1]
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex 为非法的视频解码器索引
//@INPUT: WinClip 的窗口指针为CSUDI_NULL
//@INPUT:eScreenDevice =  [EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD]
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]外的值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetWinClip，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0017(void)
{
	int nVideoCount =0;
	int nVideoErrorCount = 3 * 2;
	int nVideoErrorIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	typedef struct 
	{
		int m_nVideoErrorIndex;
		CSUDISCREENType_E m_eScreenDevice;	
	}CSTC_ErrorSetWinClip_S;
	
	CSTC_ErrorSetWinClip_S s_SetErrorWinClipTpye[6];

	memset(s_SetErrorWinClipTpye, 0, sizeof(s_SetErrorWinClipTpye));

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"没有可用的视频解码器\n");
	s_SetErrorWinClipTpye[0].m_nVideoErrorIndex = -1;
	s_SetErrorWinClipTpye[0].m_eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	s_SetErrorWinClipTpye[0].m_nVideoErrorIndex = -1;
	s_SetErrorWinClipTpye[0].m_eScreenDevice = EM_UDI_VOUT_DEVICE_HD;

	s_SetErrorWinClipTpye[0].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorWinClipTpye[0].m_eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	s_SetErrorWinClipTpye[0].m_nVideoErrorIndex = nVideoCount;
	s_SetErrorWinClipTpye[0].m_eScreenDevice = EM_UDI_VOUT_DEVICE_HD;

	s_SetErrorWinClipTpye[0].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorWinClipTpye[0].m_eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	s_SetErrorWinClipTpye[0].m_nVideoErrorIndex = nVideoCount + 1;
	s_SetErrorWinClipTpye[0].m_eScreenDevice = EM_UDI_VOUT_DEVICE_HD;	

	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSetWinClip(s_SetErrorWinClipTpye[nVideoErrorIndex].m_nVideoErrorIndex, 
						s_SetErrorWinClipTpye[nVideoErrorIndex].m_eScreenDevice, CSUDI_NULL)), "参数判断错误!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
		
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip在eScreenDevice 参数错误情况下的执行情况
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex [0...nVideoIndex -1]
//@INPUT: WinClip 的窗口指针为[合法]
//@INPUT:eScreenDevice =  [Error]
//@EXPECTATION: 返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount得到Video的数目nVideo
//@EXECUTIONFLOW: 2、nVideoIndex取[0..nVideo]外的值
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetWinClip，检查返回值是否为非CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0018(void)
{
	int nVideoCount = 0;
	
	int nVideoIndex = 0;
	
	CSUDISCREENType_E eScreenDevice = 0;
	
	CSUDI_BOOL bResult = CSUDI_FALSE;
		
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获取Video Decoder设备的总数调用失败!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0), "没有可用的视频解码器\n");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL)), "参数判断错误!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bResult;	
}

//@CASEGROUP:CSUDIVIDEOSetWinClip CSUDISCREENSetResolution
//@DESCRIPTION: 测试CSUDIVIDEOSetWinClip裁剪的是数据源的窗口,与输出分辨率设置无关
//@PRECONDITION:Video设备的数目不为0
//@PRECONDITION:Video设备能正确播放视频数据流
//@INPUT: nVideoIndex = 0
//@INPUT: WinClip 的窗口指针为{0,0,300,200}
//@EXPECTATION: 设置裁剪窗口后,不管输出分辨如何设置,屏幕上的画面内容不变
//@EXECUTIONFLOW: 1、播放一个测试节目,期望播放成功
//@EXECUTIONFLOW: 2、调用CSUDISCREENSetResolution将高清输出设置成1080I_50HZ
//@EXECUTIONFLOW: 3、调用CSUDIVIDEOSetWinClip将高标清输出裁剪窗口同时设置成{0,0,300,200},确认输出效果
//@EXECUTIONFLOW: 4、调用CSUDISCREENSetResolution将标清输出设置成NTSC,高清输出设置成720P,期望裁剪窗口不变
//@EXECUTIONFLOW: 5、调用CSUDISCREENSetResolution将标清输出设置成PAL,高清输出设置成1080I,期望裁剪窗口不变
//@EXECUTIONFLOW: 6、调用CSUDIPLAYERClose关闭播放器
//@EXECUTIONFLOW: 7、调用CSUDIVIDEOSetWinClip将高标清的输出裁剪窗口设置为全屏,恢复测试环境
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0019(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E enDev;
	CSUDISCREENResolution_E enRes;
	BOOL bHDSupport = bIsSupportHD();
	CSUDIWinRect_S stRect = {0,0,300,200};

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");

	if (bHDSupport)
	{
		enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
		"fail to set resolution\n");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(0,
		EM_UDI_VOUT_DEVICE_SD,&stRect),
		"fail to set window clip\n");
	if (bHDSupport)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(0,
		EM_UDI_VOUT_DEVICE_HD,&stRect),
		"fail to set window clip\n");
	}

	CSTCPrint("(%d,%d,%d,%d)位置的画面被裁剪全屏输出(Y/N)\n",
		stRect.m_nX,stRect.m_nY,stRect.m_nWidth,stRect.m_nHeight);
	
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

	enRes = EM_UDISCREEN_RESOLUTION_NTSC;
	enDev = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
	"fail to set resolution\n");
	if (bHDSupport)
	{
		enRes = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
		"fail to set resolution\n");
	}

	CSTCPrint("标清输出NTSC,高清输出720P(如果有的话)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

	enRes = EM_UDISCREEN_RESOLUTION_PAL;
	enDev = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
	"fail to set resolution\n");
	if (bHDSupport)
	{
		enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
		"fail to set resolution\n");
	}

	CSTCPrint("标清输出PAL,高清输出1080I(如果有的话)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(0,
		EM_UDI_VOUT_DEVICE_SD|EM_UDI_VOUT_DEVICE_HD,NULL),
		"fail to set window clip\n");

	return TRUE;
}

//@CASEGROUP:CSUDIVIDEOSetVBIEnable
//@DESCRIPTION:测试直播CC TS码流，正常播放的情况
//@PRECONDITION:平台支持VBI方式的CloseCaption
//@INPUT:
//@EXPECTATION:  能播放出指定TS流，显示CC正常
//@EXECUTIONFLOW:1、播放指定的TS码流
//@EXECUTIONFLOW:2、提示用户确认，音视频是否播放出来
//@EXECUTIONFLOW:3、调用CSUDIVIDEOSetVBIEnable ，Enable设为TRUE  期望显示CC
//@EXECUTIONFLOW:4、提示用户确认，显示CC正常
//@EXECUTIONFLOW:5、调用CSUDIVIDEOSetVBIEnable ，Enable设为FALSE  期望不显示CC
//@EXECUTIONFLOW:6、提示用户确认，显示CC正常
//@EXECUTIONFLOW:7、调用CSUDIVIDEOSetVBIEnable ，Enable设为TRUE，期望显示CC
//@EXECUTIONFLOW:8、重复步骤3-步骤7共3次
//@EXECUTIONFLOW:9、停止播放
CSUDI_BOOL CSTC_VIDEO_IT_SetVBIEnable_0002(void)
{
	int                           i = 0;
	int                           nVideoIndex = 0;
	CSUDIVIDEOVBIType_E           eVideoVBIType = EM_UDIVIDEO_VBI_CLOSECAPTION;
	BOOL                          bEnable = TRUE;
	CSUDIStreamInfo_S  sStreamInfo[10];
	int nStreamCnt                 = 3;
	CSUDI_HANDLE                  hPlayer = CSUDI_NULL;

	for(i=0;i<MAX_PESES_COUNT_IN_TS;i++)
	{
		memcpy(&sStreamInfo[i],&(g_TSStreamInfo[0].m_TSContentInfo[i]),sizeof(CSUDIStreamInfo_S));
	}
	i=0;

   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == PlayLocalProgram(&hPlayer, nStreamCnt, sStreamInfo, g_TSStreamInfo[0].m_pcName), "调用inject接口播放节目失败");

	CSTCPrint("音视频播放出来了吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"步骤2失败");
	
	for (i=0;i<3;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex, eVideoVBIType, bEnable)), "步骤3失败");
		
		CSTCPrint("CC播放了吗？\n");
		CSTCPrint("Do we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤4失败");
		bEnable=FALSE;
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex,eVideoVBIType,bEnable)), "步骤5失败");
		CSTCPrint("CC关闭了播放了吗？\n");
		CSTCPrint("Don't we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤6失败");
		bEnable=TRUE;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex,eVideoVBIType,bEnable), "步骤7失败");
	    	CSTCPrint("CC播放了吗？\n");
		CSTCPrint("Do we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤8失败");	
	}
	
	CSTK_FATAL_POINT;

    if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放器失败");   
        
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败");

        hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOSetVBIEnable
//@DESCRIPTION: 测试CSUDIVIDEOSetVBIEnable 接口对错误参数的检测能力
//@INPUT:1、nVideoIndex  =  非法
//@INPUT:2、eVideoVBIType  = 非法
//@INPUT:3.	  pbEnable = 非法	
//@EXPECTATION:对该接口传递错误参数，不应该返回成功
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount 获得平台video 的数量
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOSetVBIEnable 传递错误参数，希望返回CSUDIVIDEO_ERROR_BAD_PARAMETER
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_SetVBIEnable_0001(void)
{
    int                           nVideoIndex = 0;
	CSUDIVIDEOVBIType_E           eVideoVBIType = EM_UDIVIDEO_VBI_CLOSECAPTION;
	BOOL                          pbEnable = TRUE;
	int							  nVideoCount = 0;
	CSUDI_Error_Code			  nResult = CSUDI_FAILURE;
	int							  i = 0;
 
	typedef struct _ErrorParam_GetVBIEnable_S
	{
		int 							m_nVideoIndex;
		CSUDIVIDEOVBIType_E			    m_eVideoVBIType;
		BOOL				    m_pbEnable;
		
	}ErrorParam_GetVBIEnable_S;

	ErrorParam_GetVBIEnable_S ErrorParamList[6];

	memset(ErrorParamList, 0, sizeof(ErrorParamList));
	
	// 获得 video 的数量
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获得video 数量失败");

	ErrorParamList[0].m_nVideoIndex = -1;
	ErrorParamList[0].m_eVideoVBIType = eVideoVBIType;
	ErrorParamList[0].m_pbEnable = pbEnable;

	ErrorParamList[1].m_nVideoIndex = nVideoCount;
	ErrorParamList[1].m_eVideoVBIType = eVideoVBIType;
	ErrorParamList[1].m_pbEnable = pbEnable;

	ErrorParamList[2].m_nVideoIndex = nVideoIndex;
	ErrorParamList[2].m_eVideoVBIType = -1;
	ErrorParamList[2].m_pbEnable = pbEnable;

	ErrorParamList[3].m_nVideoIndex = nVideoIndex;
	ErrorParamList[3].m_eVideoVBIType = EM_UDIVIDEO_VBI_CGMS;
	ErrorParamList[3].m_pbEnable = pbEnable;	

	ErrorParamList[4].m_nVideoIndex = nVideoIndex;
	ErrorParamList[4].m_eVideoVBIType = eVideoVBIType;
	ErrorParamList[4].m_pbEnable = FALSE;
	
	ErrorParamList[5].m_nVideoIndex = nVideoIndex;
	ErrorParamList[5].m_eVideoVBIType = eVideoVBIType;
	ErrorParamList[5].m_pbEnable = pbEnable;
	
	//检测错误参数
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOSetVBIEnable(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eVideoVBIType, ErrorParamList[i].m_pbEnable);
		if(nResult==CSUDI_SUCCESS)
		{
        return CSUDI_TRUE;
		}
		else
		{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "检测错误参数失败");
		}
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOGetWinClip
//@DESCRIPTION: 测试CSUDIVIDEOGetWinClip 接口对错误参数的检测能力
//@INPUT:1、nVideoIndex =  非法
//@INPUT:2、eScreenDevice = 非法
//@INPUT:3.	  pstRect = 非法	
//@EXPECTATION:对该接口传递错误参数，不应该返回成功
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount 获得平台video 的数量
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetWinClip 传递错误参数，希望返回CSUDIVIDEO_ERROR_BAD_PARAMETER
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetWinClip_0001(void)
{
	int 							nVideoIndex = 0;
	CSUDISCREENType_E 			eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDIWinRect_S				stRect;
	int							nVideoCount = 0;
	CSUDI_Error_Code			nResult = CSUDI_FAILURE;
	int							i = 0;

	typedef struct _ErrorParam_GetWinClip_S
	{
		int 							m_nVideoIndex;
		CSUDISCREENType_E			m_eScreenDevice;
		CSUDIWinRect_S				*m_PstRect;
		
	}ErrorParam_GetWinClip_S;

	ErrorParam_GetWinClip_S ErrorParamList[5];

	memset(ErrorParamList, 0, sizeof(ErrorParamList));

	// 获得 video 的数量
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获得video 数量失败");
	
	ErrorParamList[0].m_nVideoIndex = -1;
	ErrorParamList[0].m_eScreenDevice = eScreenDevice;
	ErrorParamList[0].m_PstRect = &stRect;

	ErrorParamList[1].m_nVideoIndex = nVideoCount;
	ErrorParamList[1].m_eScreenDevice = eScreenDevice;
	ErrorParamList[1].m_PstRect = &stRect;

	ErrorParamList[2].m_nVideoIndex = nVideoIndex;
	ErrorParamList[2].m_eScreenDevice = -1;
	ErrorParamList[2].m_PstRect = &stRect;
	
	ErrorParamList[3].m_nVideoIndex = nVideoIndex;
	ErrorParamList[3].m_eScreenDevice = EM_UDI_VOUT_DEVICE_HD+1;
	ErrorParamList[3].m_PstRect = &stRect;
	
	ErrorParamList[4].m_nVideoIndex = nVideoIndex;
	ErrorParamList[4].m_eScreenDevice = eScreenDevice;
	ErrorParamList[4].m_PstRect = CSUDI_NULL;	

	//检测错误参数
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetWinClip(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eScreenDevice, ErrorParamList[i].m_PstRect);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "检测错误参数失败");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

typedef struct _ErrorParam_GetVBIEnable_S
{
	int 							m_nVideoIndex;
	CSUDIVIDEOVBIType_E			    m_eVideoVBIType;
	BOOL				            *m_pbEnable;		
}ErrorParam_GetVBIEnable_S;

//@CASEGROUP:CSUDIVIDEOGetVBIEnable
//@DESCRIPTION: 测试CSUDIVIDEOGetVBIEnable 接口对错误参数的检测能力
//@INPUT:1、nVideoIndex  =  非法
//@INPUT:2、eVideoVBIType  = 非法
//@INPUT:3.	  pbEnable = 非法	
//@EXPECTATION:对该接口传递错误参数，不应该返回成功
//@EXECUTIONFLOW: 1、调用CSUDIVIDEOGetCount 获得平台video 的数量
//@EXECUTIONFLOW: 2、调用CSUDIVIDEOGetVBIEnable 传递错误参数，希望返回CSUDIVIDEO_ERROR_BAD_PARAMETER
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetVBIEnable_0001(void)
{
    int                           nVideoIndex = 0;
	CSUDIVIDEOVBIType_E           eVideoVBIType = EM_UDIVIDEO_VBI_CLOSECAPTION;
	BOOL                          pbEnable;
	int							  nVideoCount = 0;
	CSUDI_Error_Code			  nResult = CSUDI_FAILURE;
	int							  i = 0;
	ErrorParam_GetVBIEnable_S ErrorParamList[6];

	memset(ErrorParamList, 0, sizeof(ErrorParamList));
	
	// 获得 video 的数量
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "获得video 数量失败");

	ErrorParamList[0].m_nVideoIndex = -1;
	ErrorParamList[0].m_eVideoVBIType = eVideoVBIType;	
	ErrorParamList[0].m_pbEnable= &pbEnable;	

	ErrorParamList[1].m_nVideoIndex = nVideoCount;
	ErrorParamList[1].m_eVideoVBIType = eVideoVBIType;	
	ErrorParamList[1].m_pbEnable= &pbEnable;	

	ErrorParamList[2].m_nVideoIndex = nVideoIndex;
	ErrorParamList[2].m_eVideoVBIType = -1;	
	ErrorParamList[2].m_pbEnable= &pbEnable;		

	ErrorParamList[3].m_nVideoIndex = nVideoIndex;
	ErrorParamList[3].m_eVideoVBIType = EM_UDIVIDEO_VBI_CGMS+1;	
	ErrorParamList[3].m_pbEnable= &pbEnable;

	ErrorParamList[4].m_nVideoIndex = nVideoIndex;
	ErrorParamList[4].m_eVideoVBIType = eVideoVBIType;	
	ErrorParamList[4].m_pbEnable= CSUDI_NULL;

	ErrorParamList[5].m_nVideoIndex = nVideoIndex;
	ErrorParamList[5].m_eVideoVBIType = eVideoVBIType;	
	ErrorParamList[5].m_pbEnable= &pbEnable;
	
	//检测错误参数
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetVBIEnable(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eVideoVBIType, ErrorParamList[i].m_pbEnable);
		if(nResult==CSUDI_SUCCESS)
		{
        return CSUDI_TRUE;
		}
		else
		{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "检测错误参数失败");
		}
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}
//@CASEGROUP:PLAYER & show CC
//@DESCRIPTION:测试直播CC TS码流，正常播放的情况
//@PRECONDITION:平台支持VBI方式的CloseCaption
//@INPUT:
//@EXPECTATION:  能播放出指定TS流且显示CC正常
//@EXECUTIONFLOW:1、播放根据指定的TS码流
//@EXECUTIONFLOW:2、提示用户确认，音视频是否播放出来
//@EXECUTIONFLOW:3、调用CSUDIVIDEOSetVBIEnable ，Enable设为TRUE  期望显示CC
//@EXECUTIONFLOW:4、提示用户确认，显示CC正常
//@EXECUTIONFLOW:5、调用CSUDIVIDEOGetVBIEnable获取VBI方式的CloseCaption功能的开关状态，期望成功
//@EXECUTIONFLOW:6、判断获取到的开关状态和上面步骤设置的一致
//@EXECUTIONFLOW:7、调用CSUDIVIDEOSetVBIEnable ，Enable设为FALSE  期望显示CC
//@EXECUTIONFLOW:8、提示用户确认，显示CC正常
//@EXECUTIONFLOW:9、调用CSUDIVIDEOGetVBIEnable获取VBI方式的CloseCaption功能的开关状态，期望成功
//@EXECUTIONFLOW:10、判断获取到的开关状态和上面步骤设置的一致
//@EXECUTIONFLOW:11、调用CSUDIVIDEOSetVBIEnable ，设置其他字幕的显示，期望CC继续显示
//@EXECUTIONFLOW:12、重复步骤3-步骤11共3次
//@EXECUTIONFLOW:13、停止播放
BOOL CSTC_VIDEO_IT_GetVBIEnable_0002(void)
{
	int                           i = 0;
    int                           nVideoIndex = 0;
	CSUDIStreamInfo_S             sStreamInfo[10];
	int nStreamCnt                 = 3;//MAX_PESES_COUNT_IN_TS-1;
	CSUDIVIDEOVBIType_E           eVideoVBIType = EM_UDIVIDEO_VBI_CLOSECAPTION;
	BOOL                          bEnable = TRUE;
	BOOL                          tEnable = FALSE;
	CSUDI_HANDLE                  hPlayer = CSUDI_NULL;

	for(i=0;i<MAX_PESES_COUNT_IN_TS;i++)
	{
		memcpy(&sStreamInfo[i],&(g_TSStreamInfo[0].m_TSContentInfo[i]),sizeof(CSUDIStreamInfo_S)); 
	}
	i=0;

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS  ==  PlayLocalProgram(&hPlayer, nStreamCnt, sStreamInfo, g_TSStreamInfo[0].m_pcName), "调用inject接口播放节目失败");
	
	CSTCPrint("音视频播放出来了吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"步骤2失败");
	
	for (i=0;i<3;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex, eVideoVBIType, bEnable)), "步骤3失败");
		
		CSTCPrint("CC播放了吗？\n");
		CSTCPrint("Do we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤4失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOGetVBIEnable(nVideoIndex,eVideoVBIType,&tEnable), "步骤5失败");
		CSTK_ASSERT_TRUE_FATAL(tEnable == bEnable, "步骤6失败");

		if(tEnable)
		{
			bEnable = FALSE;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex,eVideoVBIType,bEnable), "步骤7失败");	
        	}
		CSTCPrint("CC不可以播放了吗？\n");
		CSTCPrint("Do we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOGetVBIEnable(nVideoIndex,eVideoVBIType,&tEnable), "步骤9失败");
		CSTK_ASSERT_TRUE_FATAL(tEnable == bEnable, "步骤10失败");

		if(!tEnable)
		{
			tEnable=TRUE;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex,eVideoVBIType,tEnable), "步骤11失败");	
        	}
	}

	CSTK_FATAL_POINT;

    if(CSUDI_NULL != hPlayer)
      {
          CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放器失败");   
          
          CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败");
    
          hPlayer = CSUDI_NULL;
      }

	return CSUDI_TRUE;
}
//@CASEGROUP:CSUDIVIDEOGetWinClip,CSUDIVIDEOGetWindowSize
//@DESCRIPTION: 测试CSUDIVIDEOGetWinClip,CSUDIVIDEOGetWindowSize 接口在正常播放前后的调用情况
//@INPUT:1、nVideoIndex =  0
//@INPUT:2、eScreenDevice = EM_UDI_VOUT_DEVICE_SD;EM_UDI_VOUT_DEVICE_HD
//@INPUT:3.	  pstRect = 合法值
//@EXPECTATION:对该接口使用场景进行逻辑检测
//@EXECUTIONFLOW: 1、播放前调用CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip,希望成功,且返回全屏信息
//@EXECUTIONFLOW: 2、播放前调用CSUDIVIDEOSetWindowSize,CSUDIVIDEOSetWinClip,然后CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip, 期望设置成功,且返回设置值
//@EXECUTIONFLOW: 3、播放时调用CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip,期望Step2未设置部分没有被Set影响
//@EXECUTIONFLOW: 4、停止播放后CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip,期望Step2设置项依然有效
//@EXECUTIONFLOW: 5、关闭播放句柄后SUDIVIDEOSetWindowSize,CSUDIVIDEOSetWinClip,然后CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip,期望Step2设置项依然后效,Step5设置项起作用,且返回设置值
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetWinSizeWinClip_0001(void)
{
	int 						nVideoIndex = 0;
	CSUDISCREENType_E 			eScreenDevice = -1;
	CSUDIWinRect_S				stGetRect;
	CSUDI_HANDLE				hPlayer = CSUDI_NULL;
	CSUDI_BOOL					bStopFlag = CSUDI_FALSE;
    CSUDI_BOOL					bCloseFlag = CSUDI_FALSE;
	CSUDIWinRect_S 				stSetRect_SD1;
	CSUDIWinRect_S 				stSetRect_HD1;
	CSUDIWinRect_S 				stSetRect_SD2;
	CSUDIWinRect_S 				stSetRect_HD2;

	//播放前调用CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "播放码流前CSUDIVIDEOGetWindowSize应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,获得的Rect和设置的不一致\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "播放码流前CSUDIVIDEOGetWinClip应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,获得的Rect和设置的不一致\n");

	//播放前调用CSUDIVIDEOSetWindowSize,CSUDIVIDEOSetWinClip,
	//然后CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	stSetRect_SD1.m_nX = 50;
	stSetRect_SD1.m_nY = 50;
	stSetRect_SD1.m_nHeight = 50;
	stSetRect_SD1.m_nWidth = 50;

	stSetRect_HD1.m_nX = 1000;
	stSetRect_HD1.m_nY = 1000;
	stSetRect_HD1.m_nHeight = 50;
	stSetRect_HD1.m_nWidth = 50;

	eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, &stSetRect_SD1)), "播放码流前CSUDIVIDEOSetWinClip应该成功");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "播放码流前CSUDIVIDEOGetWinClip应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, &stSetRect_SD1, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,获得的Rect和设置的不一致\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, &stSetRect_HD1)), "播放码流前CSUDIVIDEOSetWindowSize应该成功");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "播放码流前CSUDIVIDEOGetWindowSize应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stSetRect_HD1, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,获得的Rect和设置的不一致\n");


	// 播放时调用CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "显示视频失败!\n");
	
	eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "播放码流时CSUDIVIDEOGetWindowSize应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,获得的Rect和设置的不一致\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "播放码流时CSUDIVIDEOGetWinClip应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,获得的Rect和设置的不一致\n");
	
	//停止播放后CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop (hPlayer), "停止播放失败!");
	bStopFlag = CSUDI_TRUE;
	
	eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "停止播放码流后CSUDIVIDEOGetWinClip应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, &stSetRect_SD1, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,获得的Rect和设置的不一致\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "停止播放码流后CSUDIVIDEOGetWindowSize应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stSetRect_HD1, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,获得的Rect和设置的不一致\n");

	//关闭播放句柄后CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	//要求高清标清两个通道相互不影响
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
    bCloseFlag = CSUDI_TRUE;
	stSetRect_SD2.m_nX = 100;
	stSetRect_SD2.m_nY = 100;
	stSetRect_SD2.m_nHeight = 100;
	stSetRect_SD2.m_nWidth = 100;

	stSetRect_HD2.m_nX = 800;
	stSetRect_HD2.m_nY = 800;
	stSetRect_HD2.m_nHeight = 80;
	stSetRect_HD2.m_nWidth = 80;

	eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, &stSetRect_SD2)), "关闭播放器后CSUDIVIDEOSetWindowSize应该成功");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "关闭播放器后CSUDIVIDEOGetWindowSize应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stSetRect_SD2, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,获得的Rect和设置的不一致\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "关闭播放器后CSUDIVIDEOGetWinClip应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, &stSetRect_SD1, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,获得的Rect和设置的不一致\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, &stSetRect_HD2)), "关闭播放器后CSUDIVIDEOSetWinClip应该成功");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "关闭播放器后CSUDIVIDEOGetWinClip应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, &stSetRect_HD2, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,获得的Rect和设置的不一致\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "关闭播放器后CSUDIVIDEOGetWindowSize应该成功");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stSetRect_HD1, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,获得的Rect和设置的不一致\n");
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		if(!bStopFlag)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "停止播放失败!");
		}
		if(!bCloseFlag)
		{
		    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
		}
		hPlayer = CSUDI_NULL;
        //恢复环境
        CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,CSUDI_NULL);
        CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,CSUDI_NULL);
        CSUDIVIDEOSetWinClip(0,EM_UDI_VOUT_DEVICE_SD,CSUDI_NULL);
        CSUDIVIDEOSetWinClip(0,EM_UDI_VOUT_DEVICE_HD,CSUDI_NULL);
	}

	return CSUDI_TRUE;
}

static BOOL GetVOutCfg(char *pkey)
{
	char acResult[255] = {0};
	int nResultSize = (int)sizeof(acResult);

	if (CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo("VIDEO", pkey, acResult, nResultSize))
	{
		CSTCPrint( "获取%s 字段Config文件信息失败!",pkey);
		return CSUDI_FALSE;
	}

	if (1 == CSTKGetIntFromStr(acResult, 10))
	{
		return CSUDI_TRUE;	// 表示不支持高清平台
	}

	return CSUDI_FALSE;
}

static unsigned int GetBoardOutputPort()
{
	unsigned int unPort = 0;
	
	if (GetVOutCfg("VOUT_HAS_CVBS"))
	{
		unPort |= 1<<EM_UDIVOUT_CVBS;
	}
	if (GetVOutCfg("VOUT_HAS_YC"))
	{
		unPort |= 1<<EM_UDIVOUT_YC;
	}
	if (GetVOutCfg("VOUT_HAS_YUV"))
	{
		unPort |= 1<<EM_UDIVOUT_YUV;
	}
	if (GetVOutCfg("VOUT_HAS_RF"))
	{
		unPort |= 1<<EM_UDIVOUT_RF;
	}
	if (GetVOutCfg("VOUT_HAS_SCART"))
	{
		unPort |= 1<<EM_UDIVOUT_SCART;
	}
	if (GetVOutCfg("VOUT_HAS_HDMI"))
	{
		unPort |= 1<<EM_UDIVOUT_HDMI;
	}
	if (GetVOutCfg("VOUT_HAS_DVO"))
	{
		unPort |= 1<<EM_UDIVOUT_DVO;
	}

	return unPort;
}

static char* VOutMsg(CSUDIVOUTDevices_E enPort)
{
	switch(enPort)
	{
		case EM_UDIVOUT_CVBS:
			return "SD的A/V";
		case EM_UDIVOUT_YC:
			return "S端子";
		case EM_UDIVOUT_YUV:
			return "YPbPr分量输出";
		case EM_UDIVOUT_RF:
			return "RF";
		case EM_UDIVOUT_SCART:
			return "SCART口";
		case EM_UDIVOUT_HDMI:
			return "HDMI";
		case EM_UDIVOUT_DVO:
			return "DVO";
		default:
			return "未知端口";
	}
}

//@CASEGROUP:CSUDIVOUTDisable CSUDIVOUTEnable
//@DESCRIPTION: 测试CSUDIVOUTDisable关闭输出端口,CSUDIVOUTEnable打开输出端口功能
//@PRECONDITION:能正常播放节目
//@INPUT: nVideoIndex=0
//@INPUT: 所有支持的输出端口
//@REMARK:平台支持那些输出端口通过测试配置文件[VIDEO]的VOUT_HAS_xx字段配置
//@EXPECTATION:1. CSUDIVOUTDisable能关闭指定端口
//@EXPECTATION:2. CSUDIVOUTEnable能打开指定端口
//@EXECUTIONFLOW: 1、获取测试配置文件中VOUT的端口配置,期望至少支持一个输出端口
//@EXECUTIONFLOW: 2、播放测试节目,期望播放成功
//@EXECUTIONFLOW: 3、调用CSUDIVOUTDisable依次关闭所有支持的输出端口,并确认是否已经关闭
//@EXECUTIONFLOW: 4、调用CSUDIVOUTEnable依次打开所有支持的输出端口,期望打开成功
//@EXECUTIONFLOW: 5、调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_VOUT_DisableEnable_0001(void)
{
	unsigned int unPort = 0;
	int i=0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	unPort = GetBoardOutputPort();

	CSTK_ASSERT_TRUE_FATAL(unPort,"没有获取到平台VOUT配置,请在测试配置文件中配置VOUT_HAS_xx字段\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");

	for (i=0;i<EM_UDIVOUT_NUM;i++)
	{
		if (unPort&(1<<i))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVOUTDisable(i),
				"disable vout port fail\n");
			CSTCPrint("请确认%s输出已经断开无输出(Y/N)\n",VOutMsg(i));
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");
		}
	}

	for (i=0;i<EM_UDIVOUT_NUM;i++)
	{
		if (unPort&(1<<i))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVOUTEnable(i),
				"disable vout port fail\n");
			CSTCPrint("请确认%s输出已经恢复有图像输出(Y/N)\n",VOutMsg(i));
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");
		}
	}
	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return TRUE;
}

//@CASEGROUP:CSUDIVOUTDisable CSUDIVOUTEnable
//@CASEGROUP:CSUDISCREENSetResolution
//@DESCRIPTION: 测试VOUT输出端口的打开关闭状态不受分辨率切换影响,以及不受HDMI插拔影响
//@INPUT: nVideoIndex=0
//@INPUT: 所有支持的输出端口
//@REMARK:平台支持那些输出端口通过测试配置文件[VIDEO]的VOUT_HAS_xx字段配置
//@EXPECTATION:1.输出端口关闭后,切换分辨率不影响端口状态
//@EXPECTATION:2.插拔HDMI线不影响VOUT的输出端口状态 
//@EXECUTIONFLOW: 1、获取测试配置文件中VOUT的端口配置,期望至少支持一个输出端口
//@EXECUTIONFLOW: 2、播放测试节目,期望播放成功
//@EXECUTIONFLOW: 3、调用CSUDISCREENSetResolution将标清输出设置成PAL,高清输出设置成1080I
//@EXECUTIONFLOW: 4、调用CSUDIVOUTDisable依次关闭所有支持的输出端口,并确认是否已经关闭
//@EXECUTIONFLOW: 5、调用CSUDISCREENSetResolution将标清输出设置成NTSC,高清输出设置成720P,
//@EXECUTIONFLOW:确认端口状态无变化
//@EXECUTIONFLOW: 6、调用CSUDISCREENSetResolution将标清输出设置成PAL,高清输出设置成1080I
//@EXECUTIONFLOW:确认端口状态无变化
//@EXECUTIONFLOW: 7、提示插拔HDMI线操作,确认插拔HDMI不影响VOUT端口状态
//@EXECUTIONFLOW: 8、调用CSUDIVOUTEnable依次打开所有支持的输出端口,期望打开成功
//@EXECUTIONFLOW: 9、调用CSUDIPLAYERClose关闭播放器
CSUDI_BOOL CSTC_VOUT_DisableEnable_0002(void)
{
	unsigned int unPort = 0;
	int i=0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E enDev;
	CSUDISCREENResolution_E enRes;
	BOOL bHDSupport = bIsSupportHD();

	unPort = GetBoardOutputPort();

	CSTK_ASSERT_TRUE_FATAL(unPort,"没有获取到平台VOUT配置,请在测试配置文件中配置VOUT_HAS_xx字段\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "锁频失败!\n");

	enRes = EM_UDISCREEN_RESOLUTION_PAL;
	enDev = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
		"fail to set resolution\n");
	if (bHDSupport)
	{
		enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
		"fail to set resolution\n");
	}
	for (i=0;i<EM_UDIVOUT_NUM;i++)
	{
		if (unPort&(1<<i))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVOUTDisable(i),
				"disable vout port fail\n");
		}
	}

	CSTCPrint("请确认平台所有的输出端口已经断开无输出(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

	enRes = EM_UDISCREEN_RESOLUTION_NTSC;
	enDev = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
		"fail to set resolution\n");
	if (bHDSupport)
	{
		enRes = EM_UDISCREEN_RESOLUTION_720P_50HZ;
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
		"fail to set resolution\n");
	}

	CSTCPrint("分辨率标清切换到 NTSC,高清切换到720P(如果有高清)\n");
	CSTCPrint("请确认平台所有的输出端口依旧断开无输出(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

	enRes = EM_UDISCREEN_RESOLUTION_PAL;
	enDev = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
		"fail to set resolution\n");
	if (bHDSupport)
	{
		enRes = EM_UDISCREEN_RESOLUTION_1080I_50HZ;
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enRes, 1),
		"fail to set resolution\n");
	}

	CSTCPrint("分辨率标清切换到 PAL,高清切换到1080I(如果有高清)\n");
	CSTCPrint("请确认平台所有的输出端口依旧断开无输出(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");

	if (unPort&(1<<EM_UDIVOUT_HDMI))
	{
		CSTCPrint("请拔掉HDMI线再插上\n");
		CSTCPrint("请确认插拔HDMI线后,所有输出依旧处于关闭状态(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");
	}

	for (i=0;i<EM_UDIVOUT_NUM;i++)
	{
		if (unPort&(1<<i))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVOUTEnable(i),
				"disable vout port fail\n");
			CSTCPrint("请确认%s输出已经恢复有图像输出(Y/N)\n",VOutMsg(i));
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"确认效果失败\n");
		}
	}
	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败!");
	}

	return TRUE;
}


