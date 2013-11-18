/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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
#define MAX_PESES_COUNT_IN_TS (EM_UDI_CONTENT_PCR+1) /*��ο�CSUDIContentType_Eö�ٵ����͸���*/
typedef enum 
{
    EM_INJECT_TS_H264_AAC,
    EM_INJECT_TS_MAX_COUNT
}ES_ServiceIndex_E;

typedef struct _TSStreamInfo_S
{
    char * m_pcName; //ע���ļ�����
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
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]�ֶ����Ҳ���CS_UDI2OSG_SD_DISPLAY_WIDTH\r\n");
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
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]�ֶ����Ҳ���CS_UDI2OSG_SD_DISPLAY_HEIGHT\r\n");
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
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]�ֶ����Ҳ���CS_UDI2OSG_HD_DISPLAY_WIDTH\r\n");
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
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]�ֶ����Ҳ���CS_UDI2OSG_HD_DISPLAY_HEIGHT\r\n");
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
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]�ֶ����Ҳ���CS_UDIVIDEO_HD_WINDOW_WIDTH\r\n");
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
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]�ֶ����Ҳ���CS_UDIVIDEO_HD_WINDOW_HEIGHT\r\n");
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
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]�ֶ����Ҳ���CS_OSGUDI2_SURPORT_HD_SD_DISPLAY\r\n");
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
		CSTCPrint("[UDI2VIDEOTEST]:[OSG]�ֶ����Ҳ���CS_OSGUDI2_SHARE_HD_DISPLAY_SERFACE\r\n");
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
	//�ڱ�����������ִ��ǰ����
	return testInit();
}

CSUDI_BOOL CSTC_VIDEO_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return testUnInit();
}

/**
@brief �ж��Ƿ�֧�ָ������

@return ���ƽ̨֧�֣��򷵻�CSUDI_TRUE��ƽ̨��֧���򷵻�CSUDI_FALSE
*/
static CSUDI_BOOL CSTC_VIDEO_IsSupportHdDispaly(void)
{
	return g_bSupportHdDispaly;
}

/**
@brief �жϸ߱����Ƿ�ͬԴ��������߱��干��һ�������Դ�

@return ���ƽ̨֧�֣��򷵻�CSUDI_TRUE��ƽ̨��֧���򷵻�CSUDI_FALSE
*/
static CSUDI_BOOL CSTC_VIDEO_IsShareHDDisplaySurface(void)
{
	return g_bShareHdDispalySurface;
}

//��ȡ����pResult�Ľ��ơ� 0x/0X��ͷΪ16���ƣ�0��ͷΪ8���ƣ�����Ϊ10����
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
		CSTCPrint("�������ָ��Ϊ��!\n");
		return bResult;
	}
	if (CS_TK_CONFIG_SUCCESS != CSTKGetConfigInfo(pSection, pKey, acResult, nResultSize))
	{
		CSTCPrint("��ȡConfig�ļ���Ϣʧ��!\n");
		return bResult;
	}
	
	nBase = CSTC_VIDEO_CheckStr(acResult);
	if (-1 == nBase)
	{
		CSTCPrint("�޷����Config ��Ϣ����Ӧ�Ľ���(16/10/8)\n");
		return bResult;
	}
	if (nCheck != CSTKGetIntFromStr(acResult, nBase))
	{	
		CSTCPrint("Video��Ϣ��Config��Ϣ��һ��!\n");
		return bResult;
	}
	
	bResult = CSUDI_TRUE;
	
	return bResult;
}

/*��������������*/
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
* ������ľ�������ת���ɶ�Ӧ�ĸ����ֵ
* �ýӿ���Ҫ����Video�ľ��������ת��
* �ýӿ���Ҫ���ڸ߱���ͬԴ��ƽ̨�Ĳ���
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
* ������ľ�������ת���ɶ�Ӧ�ĸ����ֵ
* �ýӿ���Ҫ����OSD�ľ��������ת��
* �ýӿ���Ҫ���ڸ߱���ͬԴ��ƽ̨�Ĳ���
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

//����֧��nDemxType���͵�demx
//nDemxType�μ�CSUDIDEMUXWorkType_E
static int CSUDI_VIDEO_FindDemuxID(int nDemxType)
{
    int i = 0;
    int nDemuxCount = 0;
    int nDemuxID = -1;
    CSUDIDEMUXCapability_S sDemuxCapabilityInfo;

    if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //��ȡ��demux������
    {
        for (i = 0; i < nDemuxCount; i++)
        {
            if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //ȡ���ӦID=i ��demux������
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

//Ѱ����Ƶ������
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

//Ѱ����Ƶ������
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

/*����������Ƶ*/
static CSUDI_BOOL LockFrequency(CSUDI_HANDLE *phPlayer)
{
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[2];
	CSUDI_BOOL bResult = CSUDI_FALSE; 
	int nStreamCnt =2;
	int nTunerIndex =0;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	//////////////////////���ý�Ŀ��Ϣ
	sStreamInfo[0].m_nPid=1140;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	sStreamInfo[1].m_nPid=1141;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;

	sPlayerChnl.m_nDemux = CSUDI_VIDEO_FindDemuxID(EM_UDI_DEMUX_PLAY);
	sPlayerChnl.m_nVideoDecoder = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_MPEG2);
	sPlayerChnl.m_nAudioDecoder = CSUDI_VIDEO_FindAudioDecoder(EM_UDI_AUD_STREAM_MPEG2);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex,"Audio&Video Test_27Mbps_20070524.ts"), "��Ƶʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,phPlayer)), "�򿪲�����ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != phPlayer, "�������������ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (*phPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "���ý�����Ϣʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(*phPlayer), "��ʼ����ʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE), "����Ƶ��ʧ��\n");

	CSUDIOSThreadSleep(1500);
	CSTCPrint("�뽫����������ӵ����������\n");
	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���Ų�����");
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

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex, pTSFilename), "��Ƶʧ��!");
    CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,phPlayer)), "�򿪲�����ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != phPlayer, "�������������ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (*phPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "���ý�����Ϣʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(*phPlayer), "��ʼ����ʧ��!");

	CSUDIOSThreadSleep(1500);

    bResult = CSUDI_SUCCESS;

	CSTK_FATAL_POINT;
    
    g_bAssertBeforeFatalPoint = CSUDI_TRUE;

	return bResult;		
}

/*����������Ƶ*/
static CSUDI_BOOL LockHDFrequency(CSUDI_HANDLE *phPlayer)
{
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[2];
	CSUDI_BOOL bResult = CSUDI_FALSE; 
	int nStreamCnt =2;
	int nTunerIndex =0;
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_LIVE;

	//////////////////////���ý�Ŀ��Ϣ
	sStreamInfo[0].m_nPid=0x33c;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_H264;

	sStreamInfo[1].m_nPid=0x33d;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG1;

	sPlayerChnl.m_nDemux = CSUDI_VIDEO_FindDemuxID(EM_UDI_DEMUX_PLAY);
	sPlayerChnl.m_nVideoDecoder = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_H264);
	sPlayerChnl.m_nAudioDecoder = CSUDI_VIDEO_FindAudioDecoder(EM_UDI_AUD_STREAM_MPEG1);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex,"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts"), "��Ƶʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,phPlayer)), "�򿪲�����ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != phPlayer, "�������������ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (*phPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "���ý�����Ϣʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(*phPlayer), "��ʼ����ʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE), "����Ƶ��ʧ��\n");

	CSUDIOSThreadSleep(1500);
	CSTCPrint("�뽫����������ӵ����������\n");
	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���Ų�����");
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

/*����ָ����Ƶ��������Ĵ��ڴ�С�����ڲ���*/
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
	{0,CSUDI_NULL,"ȫ����ʾ"},
	{1,&s_aWinRect_WinSize_SD[0],"λ����Ļ���Ͻ�{0, 0, 720/2, 576/2}����Сͼ��"},
	{2,&s_aWinRect_WinSize_SD[1],"λ����Ļ���·�{720/2, 576/2, 720/3, 576/3}����Сͼ��"},
	{3,&s_aWinRect_WinSize_SD[2],"ͼ�����·�����λ50�����أ��Ƴ���Ļ�Ĳ��ֱ��г�"},
	{4,&s_aWinRect_WinSize_SD[3],"λ����Ļ���Ͻ�{0, 0, 720/3, 576/3}����Сͼ��"},
	{5,&s_aWinRect_WinSize_SD[4],"λ����Ļ���Ϸ�{720/2, 0, 720/3, 576/2}��������Сͼ��"},
	{6,&s_aWinRect_WinSize_SD[5],"λ����Ļ���·�{0, 576/2, 720/2, 576/3}��ѹ����Сͼ��"},
	{7,&s_aWinRect_WinSize_SD[6],"λ����Ļ�м�λ��{720/3, 576/3, 720/3, 576/3}����Сͼ��"},
	{8,&s_aWinRect_WinSize_SD[7],"ͼ�����Ϸ�����λ50������,�Ƴ���Ļ�Ĳ��ֱ��г�"},
	{9,&s_aWinRect_WinSize_SD[8],"�������󣬴��ڿ��ӦΪ����0����"},
	{10,CSUDI_NULL,"ȫ����ʾ"},
	{11,&s_aWinRect_WinSize_HD[0],"λ����Ļ���Ͻ�{0, 0, 1920/2, 1080/2}����Сͼ��"},
	{12,&s_aWinRect_WinSize_HD[1],"λ����Ļ���·�{1920/2, 1080/2, 1920/3, 1080/3}��Сͼ��"},
	{13,&s_aWinRect_WinSize_HD[2],"ͼ�����·�����λ50�����أ��Ƴ���Ļ�Ĳ��ֱ��г�"},
	{14,&s_aWinRect_WinSize_HD[3],"λ����Ļ���Ͻ�{0, 0, 1920/3, 1080/3}����Сͼ��"},
	{15,&s_aWinRect_WinSize_HD[4],"λ����Ļ���Ϸ�{1920/2, 0, 1920/3, 1080/2}��������Сͼ��"},
	{16,&s_aWinRect_WinSize_HD[5],"λ����Ļ���·�{0, 1080/2, 1920/2, 1080/3}��Ļ��С��ѹ����Сͼ��"},
	{17,&s_aWinRect_WinSize_HD[6],"λ����Ļ�м�λ��{1920/3, 1080/3, 1920/3, 1080/3}����Сͼ��"},
	{18,&s_aWinRect_WinSize_HD[7],"ͼ�����Ϸ�����λ50������,�Ƴ���Ļ�Ĳ��ֱ��г�"},
	{19,&s_aWinRect_WinSize_HD[8],"�������󣬴��ڿ��ӦΪ����0����"}
};

/*WinClip��WinSize ��������,������Ƶ��������Ĵ��ڴ�С��ʵ�����Ź���*/
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
	{0,EM_UDI_VOUT_DEVICE_SD,CSUDI_NULL,&(s_aWinRect_WinChip_SD[0]), "ȫ��ͼ��ѹ��Ϊ1/4��Ļ��С��λ����Ļ���Ͻ���ʾ"},
	{1,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[0]),CSUDI_NULL,"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{2,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[1]),CSUDI_NULL,"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{3,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[2]),CSUDI_NULL,"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{4,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[3]),CSUDI_NULL,"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{5,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[4]),CSUDI_NULL,"��Ļ������1/9��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{6,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[4]),&s_aWinRect_WinChip_SD[5],"��Ļ������1/9��Ļ��С��ͼ�񱻼��к�ԭ����ʾ����Ļ���½�"},
	{7,EM_UDI_VOUT_DEVICE_SD,&(s_aWinRect_WinChip_SD[6]),CSUDI_NULL,"���������������"},

	{8,EM_UDI_VOUT_DEVICE_HD,CSUDI_NULL,&(s_aWinRect_WinChip_HD[0]), "ȫ��ͼ��ѹ��Ϊ1/4��Ļ��С��λ����Ļ���Ͻ���ʾ"},
	{9,EM_UDI_VOUT_DEVICE_HD, &(s_aWinRect_WinChip_HD[0]),CSUDI_NULL,"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{10,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[1]),CSUDI_NULL,"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{11,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[2]),CSUDI_NULL,"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{12,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[3]),CSUDI_NULL,"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{13,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[4]),CSUDI_NULL,"��Ļ������1/9��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"},
	{14,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[4]),&s_aWinRect_WinChip_HD[5],"��Ļ������1/9��Ļ��С��ͼ�񱻼��к�ԭ����ʾ����Ļ���½�"},
	{15,EM_UDI_VOUT_DEVICE_HD,&(s_aWinRect_WinChip_HD[6]),CSUDI_NULL,"���������������"},
		
};

/*��Ƶ��ͼ(����)�ӿڲ�����Ϣ������������������Surface����*/
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
	{0,CSUDI_NULL,CSUDI_NULL,720,576,"ȫ����ʾ����֡����û��ѹ���ͷŴ�����"},
	{1,CSUDI_NULL,CSUDI_NULL,250,180,"ȫ��ͼ��ѹ��Ϊ250*180��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"},
	{2,CSUDI_NULL,&s_aWinRect_SnapShot[5],300,240,"ȫ��ͼ��ѹ��Ϊ250*180��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"}, 
	{3,&s_aWinRect_SnapShot[5],CSUDI_NULL,300,240,"ͼ�����Ͻ�250*180�����򿽱�������ռ䣬��֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ300*240"},
	{4,&s_aWinRect_SnapShot[4],CSUDI_NULL,250,180,"ͼ�����м��1/9���汻ѹ����250*180��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"},
	{5,&s_aWinRect_SnapShot[3],CSUDI_NULL,250,180,"ͼ�����Ͻǵ�1/4���汻ѹ����250*180��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"},
	{6,&s_aWinRect_SnapShot[2],CSUDI_NULL,250,180,"ͼ�����½ǵ�1/4���汻ѹ����250*180��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"},
	{7,&s_aWinRect_SnapShot[1],CSUDI_NULL,250,180,"ͼ�����½ǵ�1/4���汻ѹ����250*180��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"},
	{8,&s_aWinRect_SnapShot[0],CSUDI_NULL,250,180,"ͼ�����Ͻǵ�1/4���汻ѹ����250*180��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"},
	{9,&s_aWinRect_SnapShot[0],&s_aWinRect_SnapShot[5],720,576,"ͼ�����Ͻǵ�1/4���汻ѹ��Ϊ250*180��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"},
	{10,&s_aWinRect_SnapShot[0],&s_aWinRect_SnapShot[1],720,576,"ͼ�����Ͻǵ�1/4���汻�Ƶ����½���ʾ����֡������ʾ��С��Ȼ��1/4����"},
	{11,&s_aWinRect_SnapShot[0],&s_aWinRect_SnapShot[1],40,30,"����ռ䲻��"},

};

static CSUDIWinRect_S s_aWinRect_SnapShot_HD[]=
{	
	{0, 0, 1920/2, 1080/2},
	{1920/2, 1080/2,1920/2, 1080/2},
	{0, 1080/2,1920/2, 1080/2},
	{1920/2, 0,1920/2, 1080/2},
	{1920/3, 1080/3, 1920/3, 1080/3},		
	{0, 0, 1920/2, 1080/2},
	//���������1280*720������λ��
	{0, 0, 1280/2, 720/2},
	{1280/2, 720/2, 1280/2, 720/2},
};

static CSTC_SnapShot_S s_aSnapShotTest_HD[]=
{
	{0,CSUDI_NULL,CSUDI_NULL,1280,720,"ȫ����ʾ����֡����û��ѹ���ͷŴ�����"},
	{1,CSUDI_NULL,CSUDI_NULL,640,360,"ȫ��ͼ��ѹ��Ϊ640*360��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ640*360"},
	{2,CSUDI_NULL,&s_aWinRect_SnapShot_HD[6],800,500,"ȫ��ͼ��ѹ��Ϊ640*360��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ640*360"}, 
	{3,&s_aWinRect_SnapShot_HD[5],CSUDI_NULL,800,500,"ͼ�����Ͻ�640*360�����򿽱�������ռ䣬��֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ800*500"},
	{4,&s_aWinRect_SnapShot_HD[4],CSUDI_NULL,640,360,"ͼ�����м��1/9���汻ѹ����640*360��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ640*360"},
	{5,&s_aWinRect_SnapShot_HD[3],CSUDI_NULL,640,360,"ͼ�����Ͻǵ�1/4���汻ѹ����640*360��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ640*360"},
	{6,&s_aWinRect_SnapShot_HD[2],CSUDI_NULL,640,360,"ͼ�����½ǵ�1/4���汻ѹ����640*360��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ640*360"},
	{7,&s_aWinRect_SnapShot_HD[1],CSUDI_NULL,640,360,"ͼ�����½ǵ�1/4���汻ѹ����640*360��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ640*360"},
	{8,&s_aWinRect_SnapShot_HD[0],CSUDI_NULL,640,360,"ͼ�����Ͻǵ�1/4���汻ѹ����640*360��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ640*360"},
	{9,&s_aWinRect_SnapShot_HD[0],&s_aWinRect_SnapShot_HD[6],1280,720,"ͼ�����Ͻǵ�1/4���汻ѹ��Ϊ640*360��С����֡������ʾ����λ�����Ͻ�,��ʾͼ���СΪ640*360"},
	{10,&s_aWinRect_SnapShot_HD[0],&s_aWinRect_SnapShot_HD[7],1280,720,"ͼ�����Ͻǵ�1/4���汻�Ƶ����½���ʾ����֡������ʾ��С��Ȼ��1/4����"},
	{11,&s_aWinRect_SnapShot_HD[0],&s_aWinRect_SnapShot_HD[7],71,37,"����ռ䲻��"},
};

/*��Ƶ����16:9 ��4:3��Ϣö������*/
typedef enum _CSTC_STREAM_AspectRatio_Type_E
{
	EM_STREAM_ASPECTRATIO_4_3,
	EM_STREAM_ASPECTRATIO_16_9
	
}CSTC_STREAM_AspectRatio_Type_E;

/*����Ⱥ�����ģʽ�����������ݽṹ*/
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
		"����������"},		
	{1,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"����������ʾ�����ҷ�������"},
	{2,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"�������ҷ���ѹխ�������кڱ�"},
	{3,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"������������Σ������кڱ�"},		
	{4,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"������������Σ������кڱ�"},		
	{5,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"�������±�ѹ�⣬�����кڱ�"},
	{6,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"�������Σ�ͼ�����·�������"},
	{7,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"����������"},	
	{8,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"����������"},
	{9,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"����������ʾ�����ҷ�������"},
	{10,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"ͼ�����·������죬���������ݱ��ص�"},
	{11,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"���������Σ����·��������ݱ��ص�"},
	{12,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"���������Σ����ҷ��������ݱ��ص�"},		
	{13,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"�������ұ����죬�������������ݱ��ص�"},
	{14,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"�������Σ�ͼ�����·�������"},
	{15,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"����������"},
	{16,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"����������"},
	{17,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"����������ʾ�����ҷ�������"},
	{18,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"�������ҷ���ѹխ�������кڱߣ����������ݱ��ص�"},
	{19,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		" ������������Σ������кڱߣ����������ݱ��ص�"},
	{20,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"  ������������Σ������кڱߣ����������ݱ��ص�"},		
	{21,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"  �������ҷ������죬�����кڱߣ����������ݱ��ص�"},
	{22,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"�������Σ�ͼ�����·�������"},
	{23,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_COMBINED,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"����������"},
	{24,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"����������"},
	{25,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"�������Σ��������ҷ�������"},
	{26,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"����������"},
	{27,
		EM_STREAM_ASPECTRATIO_4_3,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"�������Σ��������ҷ�������"},
	{28,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_SD,
		"�������Σ�ͼ�����·�������"},		
	{29,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_4_3,
		EM_UDI_VOUT_DEVICE_HD,
		"����������"},
	{30,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_SD,
		"�������Σ�ͼ�����·�������"},
	{31,
		EM_STREAM_ASPECTRATIO_16_9,
		EM_UDIVIDEO_MATCH_METHOD_IGNORE,
		EM_UDIVIDEO_ASPECT_RATIO_16_9,
		EM_UDI_VOUT_DEVICE_HD,
		"����������"},
};

//@CASEGROUP:CSUDIVIDEOGetCount(int * pnVideoDecCount)
//@DESCRIPTION: ���Ժ���CSUDIVIDEOGetCount��pnVideoDecCount������ȷ����µ�ִ�н��
//@PRECONDITION:
//@INPUT:pnVideoDecCount=����ָ��
//@EXPECTATION:����ȷ��ȡ�ײ��������ڵ�Video��������Դ��Ŀ��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��ͨ��CSUDIVIDEOGetCount��ȡVideo��Դ��Ŀ���ж��Ƿ񷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���Ƚϻ�ȡ����video��Ŀ,����Ӧ�ô���0
//@EXECUTIONFLOW: 3��ͨ��config�ļ���ȡVideo��Դ��Ŀ
//@EXECUTIONFLOW: 4���Ƚ����λ�ȡ��Video��Դ��Ŀ�Ƿ���ȡ�������ȣ������ʧ��
CSUDI_BOOL CSTC_VIDEO_IT_GetCount_0001(void)
{
	int nVideoCount = 0;
	char acSection[] = "VIDEO";
	char acKey[] = "VIDEO_NUMBER";
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(nVideoCount > 0,"video count should more than zero\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE== CSTC_VIDEO_CheckFromVideoToConfig(acSection, acKey, nVideoCount)), 
						"��Config �ļ��Ƚ�ʧ��!");
	
	bResult = CSUDI_TRUE;
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetCount(int * pnVideoDecCount) 
//@DESCRIPTION: ����CSUDIVIDEOGetCount��pnVideoDecCount����Ϊ������µ�ִ�н��
//@PRECONDITION:
//@INPUT:1��pnVideoDecCount=CSUDI_NULL
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��pnVideoDecCountΪCSUDI_NULL,����CSUDIVIDEOGetCount����
//@EXECUTIONFLOW: 2���жϺ��������Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:pnVideoDecCountΪCSUDI_NULL������£�ϵͳ���ܳ����������ײ�����ȷ�ֱ��������
CSUDI_BOOL CSTC_VIDEO_IT_GetCount_0002(void)
{
	int *pnVideoCount = CSUDI_NULL;
	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOGetCount(pnVideoCount)), "��ȡVideo Decoder�豸�����������ж�����");
	 
	bResult = CSUDI_TRUE;
	CSTK_FATAL_POINT;
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetCapability(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo)
//@DESCRIPTION: ����CSUDIVIDEOGetCapability�����в�������ȷ������£�����ִ�еĽ�������Է��ص�����������ȷ�Լ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@INPUT:1��nVideoIndex = [0...nVideo-1]
//@INPUT:2��psCapabilityInfo = CSUDIVIDEOCapability_S�ṹָ��
//@EXPECTATION: ����CSUDI_SUCCESS
//@EXPECTATION: ��ȡָ��Video������,���Ҹ�Video��������ͨ���Ϸ��Լ��
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo.
//@EXECUTIONFLOW: 2����ȡ[0..nVideo-1]�ڸ�Video������
//@EXECUTIONFLOW: 3��ͨ��config�ļ�����ȡ��ӦVideo������
//@EXECUTIONFLOW: 4���Ƚ�2��3����ȡ���������������εõ��������Ƿ����
//@EXECUTIONFLOW: 5���Ƚϻ�ȡ���������Ƿ����ظ���������,����û��
//@REMARK:Video������ο�CSUDIVIDEOCapability_S
//@REMARK:Video����������,��ϸ������֧�ֵ���Ƶ��������,EM_UDI_VID_STREAM_INVALID���ͺ������Ӧ�����ԡ�
CSUDI_BOOL CSTC_VIDEO_IT_GetCapability_0001(void)
{
	int nVideoCount = 0;
	int nVideoIndex =0;
	char acSection[] = "VIDEO";
	int nBase = 0;
	int nIsAble = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		CSUDIVIDEOCapability_S sCapabilityInfo;
		int  nStreamTypeIndex =0;
		int i=0,j=0;

		memset(&sCapabilityInfo, 0, sizeof(CSUDIVIDEOCapability_S));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCapability(nVideoIndex, &sCapabilityInfo), "��ȡVideo����ʧ��!");
		for (nStreamTypeIndex =0;nStreamTypeIndex < (EM_UDI_VID_STREAMTYPE_NUM -1); nStreamTypeIndex ++)
		{	
			char ackey[255] ={0};
			char acResult[255] = {0};
			int nResultSize = sizeof(acResult);
			
			sprintf(ackey, "VIDEO_%d_STREAM_%s", nVideoIndex, s_asStreamType[nStreamTypeIndex].m_pcSteamType);
			
			CSTK_ASSERT_TRUE_FATAL(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo(acSection, ackey, acResult, nResultSize), "��ȡConfig�ļ���Ϣʧ��!");
			
			nBase = CSTC_VIDEO_CheckStr(acResult);
			
			CSTK_ASSERT_TRUE_FATAL((-1 != nBase), "�޷����Config ��Ϣ����Ӧ�Ľ���(16/10/8)");
			
			nIsAble = CSTKGetIntFromStr(acResult, nBase);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_VIDEO_CheckFromConfigToVideo(&sCapabilityInfo, s_asStreamType[nStreamTypeIndex].m_eSteamType, nIsAble), "��ȡVideo������Config��Ϣ����ȫƥ��!");
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
//@DESCRIPTION: ����CSUDIVIDEOGetCapability��nVideoIndex������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@INPUT:1��nVideoIndex={-1,nVideo,nVideo+1}
//@INPUT:2��psCapabilityInfo = CSUDIVIDEOCapability_S�ṹָ��
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡֵ{-1,nVideo,nVideo+1}���ֱ����CSUDIVIDEOGetCapability���жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:��ȡnVideoIndex={-1,nVideo,nVideo+1}�ĸ�Video��������CSUDIVIDEOGetCapability������Ӧ���ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_VIDEO_IT_GetCapability_0002(void)
{
	int nVideoCount = 0;
	CSUDIVIDEOCapability_S sCapabilityInfo;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	int anVideoErrorIndex[3] = {0};
	int nVideoErrorCount = 0;
	int nVideoErrorIndex = 0;

	memset(&sCapabilityInfo, 0, sizeof(CSUDIVIDEOCapability_S));

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");

	anVideoErrorIndex[0] = -1;
	anVideoErrorIndex[1] = nVideoCount;
	anVideoErrorIndex[2] = nVideoCount + 1;

	nVideoErrorCount = sizeof(anVideoErrorIndex) / sizeof(anVideoErrorIndex[0]);
	
	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOGetCapability(anVideoErrorIndex[nVideoErrorIndex], &sCapabilityInfo)), 
							"��ȡָ��Video Decoder�豸��������Ϣ�����жϴ���!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetCapability(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo)
//@DESCRIPTION: ����CSUDIVIDEOGetCapability��psCapabilityInfo����Ϊ������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��psCapabilityInfo = CSUDI_NULL
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2����ȡ[0..nVideo]�ڸ�Video���������жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:���psCapabilityInfo����Ϊ��,����������������Σ�CSUDIVIDEOGetCapability���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_VIDEO_IT_GetCapability_0003(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOGetCapability(nVideoIndex, CSUDI_NULL)), 
							"��ȡָ��Video Decoder�豸��������Ϣ�����жϴ���!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE * phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshot��nVideoIndex  ������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:1��nVideoIndex =  {-1,nVideo,nVideo+1}
//@INPUT:2��psSrcRect = CSUDIWinRect_S�ṹָ��
//@INPUT:3��psDstRect = CSUDIWinRect_S�ṹָ��
//@INPUT:4��psSurface = CSUDIOSGSurfaceInfo_S�ṹָ��
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ{-1,nVideo,nVideo+1}�ڵ�ֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0001(void)
{
	int nVideoCount = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hSurface = 0;
	int anVideoErrorIndex[3] = {0};
	int nVideoErrorCount = 0;
	int nVideoErrorIndex = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, s_aSnapShotTest[5].m_nSurfaceWidth,s_aSnapShotTest[5].m_nSurfaceHigh, &hSurface),"������ͼƽ��ʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

	anVideoErrorIndex[0] = -1;
	anVideoErrorIndex[1] = nVideoCount;
	anVideoErrorIndex[2] = nVideoCount + 1;

	nVideoErrorCount = sizeof(anVideoErrorIndex) / sizeof(anVideoErrorIndex[0]);

	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSnapshot(anVideoErrorIndex[nVideoErrorIndex], CSUDI_NULL, CSUDI_NULL, &hSurface)),"������ʧ��!");
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
//@DESCRIPTION: ����CSUDIVIDEOSnapshot��psSurface  ����Ϊ������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��psSrcRect = CSUDIWinRect_S�ṹָ��
//@INPUT:3��psDstRect = CSUDIWinRect_S�ṹָ��
//@INPUT:4��psSurface = CSUDI_NULL
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3��psSurfaceȡ��
//@EXECUTIONFLOW: 4���ֱ����CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0002(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDI_Error_Code nSnapshotResult = CSUDI_FAILURE;
	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������!\n");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		nSnapshotResult = CSUDIVIDEOSnapshot(nVideoIndex, CSUDI_NULL, CSUDI_NULL, CSUDI_NULL);
		
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == nSnapshotResult), "����������ʧ��\n");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}


//�ж��Ƿ�֧��ָ�� ��ͼ�θ�ʽ
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
		}///<  8-bit Palette index��Ŀǰû�е�ɫ���Surface����ɫ��ĽṹԤ����
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

//��ͼ�β�����ʾsurface
static CSUDI_BOOL showSurface(CSUDI_HANDLE hSurface, int nDisplayIndex, CSTC_SnapShot_S *psSnapShot)
{
	CSUDI_HANDLE hGraphics=0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDIOSGRect_S *psSrcRect, *psDstRect, sSurface;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	
	psDstRect = (CSUDIOSGRect_S *) (psSnapShot->m_sOutputWinRect);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL !=psSnapShot),"��������\n");
	
	CSTK_ASSERT_TRUE_FATAL((0 !=hSurface),"��������\n");

	CSTK_ASSERT_TRUE_FATAL( ((0 == nDisplayIndex) || (1 == nDisplayIndex)), "��������\n");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(nDisplayIndex,&hGraphics)), "��ȡ�Դ�ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurfaceInfo)),"��ȡԭʼ����ʧ��\n");

	if (psDstRect == CSUDI_NULL)
	{
		sSurface.m_nX =  0;
		sSurface.m_nY =  0;
		sSurface.m_nHeight = sSurfaceInfo.m_nHeight;
		sSurface.m_nWidth  = sSurfaceInfo.m_nWidth;
		psDstRect = &sSurface;
	}

	psSrcRect = psDstRect;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGBlit(hGraphics, psDstRect,hSurface, psSrcRect, EM_UDIOSG_OPMODE_COPY)),"��OSG����ʾʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"��OSG����ʾͬ��ʧ��");
	
	CSUDIOSThreadSleep(1000);

	CSTCPrint("��ȷ�ϵ��ӻ����Ƿ�Ϊ%s\n",psSnapShot->m_pcDes);	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),  "ȷ��û�о�֡!\n");

	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if(hSurface && hGraphics)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurface,CSUDI_NULL,0x00000000),  "ץȡ��ͼ����Graphics����ʾʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hGraphics, NULL, hSurface, NULL, EM_UDIOSG_OPMODE_COPY),"�رվ�֡ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"�رվ�֡ͬ��ʧ��");
	}
	
	return bResult;
}

//�����ƶ��Ĳ�������
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
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
		} 
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_RGB_565)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_RGB_565, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
		}
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_RGB_888)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_RGB_888, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
		}
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL((0), "û�к��ʵ�OSGͼ���ʽ!\n"); 		
		}

		// 2
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!\n");

		CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������!\n");

		// 3-4
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
		
		CSTCPrint("���¼�⵱ǰ��Ƶ�ֱ��ʣ���Ҫ��SD����Ƶ�ֱ�������ΪPAL\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(eScreenDevice, &eSrcResolution), "��ȡ����ǰ�ֱ���ʧ��");

		if(eSrcResolution == EM_UDISCREEN_RESOLUTION_PAL)
		{
			CSTCPrint("��ǰ�ֱ����Ѿ���PAL��\n");
		}
		else
		{
			eResolution = EM_UDISCREEN_RESOLUTION_PAL;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &eResolution, 1), "����PAL�ֱ���ʧ��");
			CSTCPrint("�ı�ֱ���ΪPAL�ɹ�\n");
		}

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");


		// 6-8
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSnapshot(0, s_aSnapShotTest[nTestID].m_sInputWinRect, s_aSnapShotTest[nTestID].m_sOutputWinRect, &hSurface), "��Ƶ��������ʧ��!\n");

		//9--10
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == showSurface(hSurface,0,&s_aSnapShotTest[nTestID]),  "ץȡ��ͼ����Graphics����ʾʧ��\n");

		if(eSrcResolution > EM_UDISCREEN_RESOLUTION_INVALID && eSrcResolution < EM_UDISCREEN_RESOLUTION_NUM)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&eScreenDevice, &eSrcResolution, 1), "�ָ�����ǰ�ֱ���ʧ��");
		}

		bResult = CSUDI_TRUE;
	}
	else 
	{
		// 2
		if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, s_aSnapShotTest_HD[nTestID].m_nSurfaceWidth, s_aSnapShotTest_HD[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
		} 
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_RGB_565)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_RGB_565, s_aSnapShotTest_HD[nTestID].m_nSurfaceWidth, s_aSnapShotTest_HD[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
		}
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_RGB_888)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_RGB_888, s_aSnapShotTest_HD[nTestID].m_nSurfaceWidth, s_aSnapShotTest_HD[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
		}
		else if (testOSGFormat(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888)==CSUDI_TRUE)
		{
		       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888, s_aSnapShotTest_HD[nTestID].m_nSurfaceWidth, s_aSnapShotTest_HD[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL((0), "û�к��ʵ�OSGͼ���ʽ!\n"); 		
		}

		// 2
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!\n");

		CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������!\n");

		// 3-4
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "��Ƶʧ��!\n");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

		// 6-8
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSnapshot(0, s_aSnapShotTest_HD[nTestID].m_sInputWinRect, s_aSnapShotTest_HD[nTestID].m_sOutputWinRect, &hSurface), "��Ƶ��������ʧ��!\n");

		//9--10
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == showSurface(hSurface, 1, &s_aSnapShotTest_HD[nTestID]),  "ץȡ��ͼ����Graphics����ʾʧ��\n");

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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface)
//@DESCRIPTION: ����CSUDIVIDEOSnapshotȫ��ץȡ��ȫ�������ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@EXPECTATION: ����CSUDI_SUCCESS��OSG����Ч��Ϊ:ȫ����ʾ��û��ѹ���ͷŴ�����
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:" ȫ����ʾ��ͼ��û��ѹ���ͷŴ�����"
//@EXECUTIONFLOW: 9������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0003(void)
{
	return  testSnapShot(0);	
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshotȫ��ץȡ�����ݻ���ռ����ѹ�����ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: ����CSUDI_SUCCESS, OSG����Ч��Ϊ:ȫ��ͼ��ѹ��Ϊ250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:" ȫ��ͼ��ѹ��Ϊ250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0004(void)
{
	return  testSnapShot(1);	
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshotȫ��ץȡ������������ڽ���ͼ��ѹ����Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect  Ϊָ�����ݽṹ{0, 0,250, 180}��ָ��
//@INPUT:psSurface.m_nWidth =300
//@INPUT:psSurface.m_nHigh =240
//@EXPECTATION: ����CSUDI_SUCCESS��OSG����Ч��Ϊ:ȫ��ͼ��ѹ��Ϊ250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ȫ��ͼ��ѹ��Ϊ250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0005(void)
{
	return  testSnapShot(2);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshotС���ֱ�ץȡ��ԭ�ο���(��ѹ��)��ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect Ϊָ�����ݽṹ{0, 0,250, 180}��ָ��
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =300
//@INPUT:psSurface.m_nHigh =240
//@EXPECTATION: ����CSUDI_SUCCESS, OSG����Ч��Ϊ:ͼ�����Ͻ�250*180�����򿽱�������ռ䣬��ʾ����λ�����Ͻ�,��ʾͼ���СΪ300*240
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ͼ�����Ͻ�250*180�����򿽱�������ռ䣬��ʾ����λ�����Ͻ�,��ʾͼ���СΪ300*240"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0006(void)
{
	return testSnapShot(3);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshot�м䲿��1/9ͼ��ץȡ�����ݻ���ռ����ѹ����ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect Ϊָ�����ݽṹ{720/3, 576/3,720/3, 576/3}��ָ��
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: ����CSUDI_SUCCESS��OSG����Ч��Ϊ:ͼ�����м��1/9���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:" ͼ�����м��1/9���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0007(void)
{
	return testSnapShot(4);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshot���Ͻ�1/4ͼ��ץȡ�����ݻ���ռ����ѹ����ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect Ϊָ�����ݽṹ{720/2, 0,720/2, 576/2}��ָ��
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: ����CSUDI_SUCCESS��OSG����Ч��Ϊ:ͼ�����Ͻǵ�1/4���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ͼ�����Ͻǵ�1/4���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0008(void)
{
	return testSnapShot(5);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshot���½�1/4ͼ��ץȡ�����ݻ���ռ����ѹ����ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect Ϊָ�����ݽṹ{0, 576/2,720/2, 576/2}��ָ��
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: ����CSUDI_SUCCESS, OSG����Ч��Ϊ:ͼ�����½ǵ�1/4���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ͼ�����½ǵ�1/4���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0009(void)
{
	return testSnapShot(6);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION:  ����CSUDIVIDEOSnapshot���½�1/4ͼ��ץȡ�����ݻ���ռ����ѹ����ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect Ϊָ�����ݽṹ{720/2, 576/2,720/2, 576/2}��ָ��
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: ����CSUDI_SUCCESS��OSG����Ч��Ϊ:ͼ�����½ǵ�1/4���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ͼ�����½ǵ�1/4���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0010(void)
{
	return testSnapShot(7);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION:  ����CSUDIVIDEOSnapshot���Ͻ�1/4ͼ��ץȡ�����ݻ���ռ����ѹ����ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect Ϊָ�����ݽṹ{0,0,720/2, 576/2}��ָ��
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =250
//@INPUT:psSurface.m_nHigh =180
//@EXPECTATION: ����CSUDI_SUCCESS, OSG����Ч��Ϊ:ͼ�����Ͻǵ�1/4���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ͼ�����Ͻǵ�1/4���汻ѹ����250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0011(void)
{
	return testSnapShot(8);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshot����ͼ��ץȡ(���Ͻ�1/4��Ļ)�����ݻ���ռ����ѹ����ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect Ϊָ�����ݽṹ{0,0,720/2, 576/2}��ָ��
//@INPUT:psDstRect Ϊָ�����ݽṹ{0, 0,250, 180}��ָ��
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@EXPECTATION: ����CSUDI_SUCCESS, OSG����Ч��Ϊ:ͼ�����Ͻǵ�1/4���汻ѹ��Ϊ250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ͼ�����Ͻǵ�1/4���汻ѹ��Ϊ250*180��С����ʾ����λ�����Ͻ�,��ʾͼ���СΪ250*180"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0012(void)
{
	return testSnapShot(9);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshot����ͼ��ץȡ(���Ͻ�1/4��Ļ)����������ͬ��С������ռ�����(���½�1/4��Ļ)��ͼ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect Ϊָ�����ݽṹ{0,0,720/2, 576/2}��ָ��
//@INPUT:psDstRect Ϊָ�����ݽṹ{720/2, 576/2,720/2, 576/2}��ָ��
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@EXPECTATION: ����CSUDI_SUCCESS, OSG����Ч��Ϊ:ͼ�����Ͻǵ�1/4���汻�Ƶ����½���ʾ����ʾ��С��Ȼ��1/4����
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ͼ�����Ͻǵ�1/4���汻�Ƶ����½���ʾ����ʾ��С��Ȼ��1/4����"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0013(void)
{
	return testSnapShot(10);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ����CSUDIVIDEOSnapshot�ڻ���ռ䲻�������µ�ִ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect Ϊָ�����ݽṹ{0,0,720/2, 576/2}��ָ��
//@INPUT:psDstRect Ϊָ�����ݽṹ{720/2, 576/2,720/2, 576/2}��ָ��
//@INPUT:psSurface.m_nWidth =40
//@INPUT:psSurface.m_nHigh =30
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0014(void)
{
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	int nVideoCount = 0;
	int 	nTestID = 11;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	// 1
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, s_aSnapShotTest[nTestID].m_nSurfaceWidth,s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
	   
	// 2
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
	// 3-4
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
    
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

	// 5
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIVIDEOSnapshot(0, s_aSnapShotTest[nTestID].m_sInputWinRect, s_aSnapShotTest[nTestID].m_sOutputWinRect, &hSurface), "���Խ��������Ԥ��!\n");

	// 6
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_FALSE)), "��ʾ��Ƶʧ��!\n");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	if (hSurface)
	{
		CSUDIOSGDestroySurface(hSurface);
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
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
	       CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIOSGCreateSurface(ePixelFormat, s_aSnapShotTest[nTestID].m_nSurfaceWidth, s_aSnapShotTest[nTestID].m_nSurfaceHigh, &hSurface)),"������ͼƽ��ʧ��!\n");
	}
	else
	{
		return CSUDI_TRUE;
	}

	// 2
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
	   
	// 3-4
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency(&hPlayer)), "��Ƶʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

	// 6
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSnapshot(0, s_aSnapShotTest[nTestID].m_sInputWinRect, s_aSnapShotTest[nTestID].m_sOutputWinRect, &hSurface), "��Ƶ��������ʧ��!\n");

	// 7
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_FALSE)), "��ʾ��Ƶʧ��!\n");

	// 8
	CSTCPrint("��ȷ�ϵ��ӻ����ѱ��ɹ��ر�\n");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),  "��Ƶ���治�ܳɹ��ر�\n");

	// 9
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == showSurface(hSurface,0,&s_aSnapShotTest[nTestID]),  "ץȡ��ͼ����Graphics����ʾʧ��\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;

	if (hSurface)
	{
		CSUDIOSGDestroySurface(hSurface);
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ������EM_UDIOSG_PIXEL_FORMAT_RGB_565 ��ʽִ��CSUDIVIDEOSnapshot, ץȡͼ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@INPUT:psSurface.m_ePixelFormat =EM_UDIOSG_PIXEL_FORMAT_RGB_565 
//@EXPECTATION: ����CSUDI_SUCCESS 
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ȫ����ʾ��ͼ��û��ѹ���ͷŴ�����"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0015(void)
{
	return testSnapShot_Pixel(EM_UDIOSG_PIXEL_FORMAT_RGB_565);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ������EM_UDIOSG_PIXEL_FORMAT_RGB_888 ��ʽִ��CSUDIVIDEOSnapshot, ץȡͼ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@INPUT:psSurface.m_ePixelFormat =EM_UDIOSG_PIXEL_FORMAT_RGB_888
//@EXPECTATION: ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ȫ����ʾ��ͼ��û��ѹ���ͷŴ�����"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0016(void)
{
	return testSnapShot_Pixel(EM_UDIOSG_PIXEL_FORMAT_RGB_888);
}

//@CASEGROUP:CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect,  CSUDI_HANDLE *  phSurface )
//@DESCRIPTION: ������EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 ��ʽִ��CSUDIVIDEOSnapshot, ץȡͼ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:nVideoIndex =  [0..nVideo-1]
//@INPUT:psSrcRect = CSUDI_NULL
//@INPUT:psDstRect = CSUDI_NULL
//@INPUT:psSurface.m_nWidth =720
//@INPUT:psSurface.m_nHigh =576
//@INPUT:psSurface.m_ePixelFormat =EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 
//@EXPECTATION: ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1����psSurface.m_nWidth��psSurface.m_nHigh������psSurface
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo-1]��һ��ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸����ȷѭ�����ű���������
//@EXECUTIONFLOW: 5������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSnapshot���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�4�в��ŵ����������ر���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾ�����ѱ��ɹ��ر�
//@EXECUTIONFLOW: 9������CSUDIVIDEOSnapshot��ȡ��psSurface����OSG����ʾ��psSurface
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ��10��ʾ��������5��ʾ��������Ƚ�Ϊ:"ȫ����ʾ��ͼ��û��ѹ���ͷŴ�����"
//@EXECUTIONFLOW: 11������psSurface
//@REMARK:�ò�������ǰ�˲��ŵ�����Ϊ����PAL��������720*576��Ϊ��ȫ��
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

	/*����Ǳ���ƽ̨,ֱ�ӷ��ز��Գɹ�*/
	if ((!bSD) && (!bIsSupportHD()))
	{
		return TRUE;
	}
		
	if (bSD)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency(&hPlayer)), "��Ƶʧ��!\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency(&hPlayer)), "��Ƶʧ��!\n");
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
	
	CSTCPrint("��ȷ��%s�����,���Ͻ�1/4��ͼ��Ϊץ������Ƶȫ��ͼƬ(Y/N)\n",
		bSD?"����":"����");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"ȷ�Ͽ��յ��Դ��ϵ�Ч��ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0, EM_UDIVIDEO_STOPMODE_FREEZE),
		"fail to set stop mode\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),
		"fail to stop player\n");
	
	CSTCPrint("��ȷ����Ƶ��Ŀ�Ѿ�ֹͣ,���澲֡(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSnapshot(0, NULL, &stRectVideo, &hOSGDisplay),
		"fail to snapshot video picture to display surface\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0,FALSE),
		"fail to hide video show\n");
	
	CSTCPrint("��ȷ��%s�������Ļ���Ͻ�1/4�����Ƶ���һ֡ͼ��,��������Ϊ��ɫ(Y/N)\n",
		bSD?"����":"����");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"ȷ��Ч��ʧ��\n");
		
	CSTK_FATAL_POINT

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	if (hOSGDisplay)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSGFill(hOSGDisplay,NULL,0x0),
			"fail to fill display surface to transparence\n");
	}

	return TRUE;
}

//@CASEGROUP:CSUDIVIDEOSnapshot
//@DESCRIPTION: ���Ա����ĿԴ��ץͼ,�Լ�ֹͣ���������ץͼ,�Լ�ֱ��ץͼ���ִ�Ĺ���
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:psSurface = �����Դ�
//@EXPECTATION:1. ��Ŀstop���ܽ�����ͼ��ץ����
//@EXPECTATION:2. ����ֱ�ӽ�ͼ��ץ���Դ���
//@EXECUTIONFLOW: 1������һ�������������,�������ųɹ���ͼ����ʾ
//@EXECUTIONFLOW: 2������CSUDIOSGGetDisplaySurface��ȡ�����Դ棬�������سɹ�
//@EXECUTIONFLOW: 3������CSUDIOSGGetSurfaceInfo��ȡ�Դ���Ϣ���������سɹ�
//@EXECUTIONFLOW: 4������CSUDIVIDEOSnapshotץȡ��Ƶȫ��ͼ���Դ�����Ͻ�1/4��
//@EXECUTIONFLOW: 5��ȷ�����Ч��
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetStopMode���óɾ�֡ģʽ
//@EXECUTIONFLOW: 7������CSUDIPLAYERStopֹͣ����,������Ƶֹͣ,video���澲֡
//@EXECUTIONFLOW: 8������CSUDIVIDEOSnapshot�ٴ�ץȡȫ����Ƶͼ���Դ����Ͻ�1/4��
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow�ر���Ƶ��ʾ,�������سɹ�
//@EXECUTIONFLOW: 10������CSUDIPLAYERClose�رղ�����
//@EXECUTIONFLOW: 11������CSUDIOSGFill��OSG�Դ�����0x0�ָ������ֳ�
CSUDI_BOOL CSTC_VIDEO_IT_Snapshot_0018(void)
{
	return TestSnapShotDisplaySurface(TRUE);
}

//@CASEGROUP:CSUDIVIDEOSnapshot
//@DESCRIPTION: ���Ը���1080I��ĿԴ��ץͼ,�Լ�ֹͣ���������ץͼ,�Լ�ֱ��ץͼ���ִ�Ĺ���
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ�����ű���������
//@INPUT:psSurface = �����Դ�
//@EXPECTATION:1. ��Ŀstop���ܽ�����ͼ��ץ����
//@EXPECTATION:2. ����ֱ�ӽ�ͼ��ץ���Դ���
//@EXECUTIONFLOW: 1������һ�������������,�������ųɹ���ͼ����ʾ
//@EXECUTIONFLOW: 2������CSUDIOSGGetDisplaySurface��ȡ�����Դ棬�������سɹ�
//@EXECUTIONFLOW: 3������CSUDIOSGGetSurfaceInfo��ȡ�Դ���Ϣ���������سɹ�
//@EXECUTIONFLOW: 4������CSUDIVIDEOSnapshotץȡ��Ƶȫ��ͼ���Դ�����Ͻ�1/4��
//@EXECUTIONFLOW: 5��ȷ�����Ч��
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetStopMode���óɾ�֡ģʽ
//@EXECUTIONFLOW: 7������CSUDIPLAYERStopֹͣ����,������Ƶֹͣ,video���澲֡
//@EXECUTIONFLOW: 8������CSUDIVIDEOSnapshot�ٴ�ץȡȫ����Ƶͼ���Դ����Ͻ�1/4��
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow�ر���Ƶ��ʾ,�������سɹ�
//@EXECUTIONFLOW: 10������CSUDIPLAYERClose�رղ�����
//@EXECUTIONFLOW: 11������CSUDIOSGFill��OSG�Դ�����0x0�ָ������ֳ�

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
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n"); 

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo����ʧ��!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

	// 4
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

	// 5-6
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStreamStatus(nVideoIndex, &sStreamStatus_1),"��һ�λ�ȡ��������Ϣʧ��!");

	// 7 
	CSUDIOSThreadSleep(1000);

	// 8
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStreamStatus(nVideoIndex, &sStreamStatus_2),  "�ڶ��λ�ȡ��������Ϣʧ��!");

	// 9
	if ((sStreamStatus_1.m_bInterlaced != sStreamStatus_2.m_bInterlaced)
		||(sStreamStatus_1.m_dwFrameCount >= sStreamStatus_2.m_dwFrameCount)
		||(sStreamStatus_1.m_eVidStreamType != sStreamStatus_2.m_eVidStreamType)
		||(sStreamStatus_1.m_nFrameRate != sStreamStatus_2.m_nFrameRate)
		||(sStreamStatus_1.m_nSourceHight != sStreamStatus_2.m_nSourceHight)
		||(sStreamStatus_1.m_nSourceWidth != sStreamStatus_2.m_nSourceWidth))
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "���λ�ý�������Ϣ��һ��");
	}	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��!");

	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus)
//@DESCRIPTION: ����CSUDIVIDEOGetStreamStatus�����в�������ȷ������£�����ִ�еĽ�������Է��ص�����������ȷ�Լ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ��������Ƶ������
//@INPUT:1��nVideoIndex = [0..nVideo-1]
//@INPUT:2��psStreamStatus = CSUDIVIDEOStreamStatus_S�ṹָ��
//@EXPECTATION: ����CSUDI_SUCCESS
//@EXPECTATION: ����ȷ�õ�ָ��Video��������״̬���ڲ���ʱ����Ƶ֡��Ŀ�ڲ��ϱ仯
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo.
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ���һ����Ƶ��
//@EXECUTIONFLOW: 4������CSTKWaitYesKey�����û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOGetStreamStatus��ȡ��Video��������״̬������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6���Է��صĽ�����״̬�ṹ���к����Լ��
//@EXECUTIONFLOW: 7��Sleep 1000ms
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetStreamStatus��ȡ��Video��������״̬������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9���Ƚ�����״̬�ı仯
//@REMARK:1��Video״̬�����Լ����ο�CSUDIVIDEOStreamStatus_S�Ķ���
//@REMARK:2����Ƶ�������ڹرյ�����£�ҲӦ���ܵõ�״̬��Ϣ
//@REMARK:2���ڲ��ŵ�ʱ����Ƶ֡��ĿӦ�ò�������
CSUDI_BOOL CSTC_VIDEO_IT_GetStreamStatus_0001(void)
{
	return testGetStreamStatus();
}

//@CASEGROUP:CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus)
//@DESCRIPTION: ����CSUDIVIDEOGetCapability��nVideoIndex������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ��������Ƶ������
//@INPUT:1��nVideoIndex={-1,nVideo,nVideo+1}
//@INPUT:2��psStreamStatus = CSUDIVIDEOStreamStatus_S�ṹָ��
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡֵ{-1,nVideo,nVideo+1}���ֱ����CSUDIVIDEOGetStreamStatus���жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_GetStreamStatus_0002(void)
{
	int nVideoCount = 0;
	
	CSUDIVIDEOStreamStatus_S sStreamStatus;
	
	CSUDI_BOOL bResult = CSUDI_FALSE;

	int anVideoErrorIndex[3] = {0};
	
	int nVideoErrorCount = 0;
	
	int nVideoErrorIndex = 0;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");

	anVideoErrorIndex[0] = -1;
	anVideoErrorIndex[1] = nVideoCount;
	anVideoErrorIndex[2] = nVideoCount + 1;

	nVideoErrorCount = sizeof(anVideoErrorIndex) / sizeof(anVideoErrorIndex[0]);
	
	for (nVideoErrorIndex = 0; nVideoErrorIndex < nVideoErrorCount; nVideoErrorIndex ++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOGetStreamStatus(anVideoErrorIndex[nVideoErrorIndex], &sStreamStatus)),
						"��ȡ����������״̬��Ϣ �����жϴ���!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}


//@CASEGROUP:CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus)
//@DESCRIPTION: ����CSUDIVIDEOGetStreamStatus��psStreamStatus����Ϊ������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷѭ��������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��psStreamStatus = CSUDI_NULL
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڸ�ֵ���ֱ����CSUDIVIDEOGetStreamStatus���жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_GetStreamStatus_0003(void)
{
	int nVideoCount = 0;
	
	int nVideoIndex = 0;
	
	CSUDI_BOOL bResult = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸�����������ж�����");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOGetStreamStatus(nVideoIndex, CSUDI_NULL)),
						"��ȡ����������״̬��Ϣ �����жϴ���!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}


//@CASEGROUP:CSUDIVIDEOShow(int nVideoIndex, CSUDI_BOOL bIsShow)
//@DESCRIPTION: ����CSUDIVIDEOShow�����в�������ȷ����µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������ʾ��Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��bIsShow=CSUDI_TRUE,CSUDI_FALSE
//@EXPECTATION: ����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸����ȷ����һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSTKWaitYes()���ȴ��û�ȷ����Ƶ�ɹ����Ų���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOShow(nVideoIndex,CSUDI_FALSE)���ر���Ƶ���жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYes()���ȴ��û�ȷ����Ƶ�ѱ��ɹ��ر�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_Show_0001(void)
{
	int nVideoCount = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	// 1-2
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo����ʧ��!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

	// 3
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
	// 4
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

	// 5
	CSTCPrint("�뽫��������˿����ӵ��Ӳ�ȷ����Ƶ��ȷ���ų���!\n");
	   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�޷���ð���ȷ��!");
      
	// 6
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_FALSE)), "��ʾ��Ƶʧ��!\n");

	CSTCPrint("�뽫��������˿����ӵ��Ӳ�ȷ����Ƶ�Ƿ񱻹ر�!\n");
	// 7
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�޷���ð���ȷ��!");

	// 10
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��!");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOShow(int nVideoIndex, CSUDI_BOOL bIsShow)
//@DESCRIPTION: ����CSUDIVIDEOShow�ڲ���nVideoIndex��������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@INPUT:1��nVideoIndex = {-1,nVideo,nVideo+1}
//@INPUT:2��bIsShow=CSUDI_TRUE,SUDI_FALSE
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1��nVideoIndexȡ{-1,nVideo,nVideo+1}�ڸ�ֵ
//@EXECUTIONFLOW: 2������CSUDIVIDEOShow���жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:��nVideoIndexȡ{-1,nVideo,nVideo+1}ֵʱ��bIsShow����ȡʲôֵ�������ط�CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");

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
							"��ʾ/������Ƶ����������������ж�ʧ��!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOShow CSUDISCREENSetResolution 
//@DESCRIPTION: ���Էֱ��ʵ��л�����Ӱ��video show״̬
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@INPUT:1��nVideoIndex = 0
//@EXPECTATION: �ֱ����л���Ӱ��video��ʾ״̬
//@EXECUTIONFLOW: 1�����ű����������,�������ųɹ�
//@EXECUTIONFLOW: 2������и������CSUDISCREENSetResolution������ֱ������ó�1080I_50HZ
//@EXECUTIONFLOW: 3������CSUDIVIDEOShow�ر�video��ʾ,�������سɹ�
//@EXECUTIONFLOW: 4������CSUDISCREENSetResolution������������ó�NTSC,���������ʾ���ó�720P_50HZ
//@EXECUTIONFLOW:�����ֱ��ʵ����ò�Ӱ����Ƶ��ʾ״̬
//@EXECUTIONFLOW: 5������CSUDISCREENSetResolution������������ó�PAL,���������ʾ���ó�1080I_50HZ
//@EXECUTIONFLOW:�����ֱ��ʵ����ò�Ӱ����Ƶ��ʾ״̬
//@EXECUTIONFLOW: 6������CSUDIVIDEOShow��video��ʾ,�������سɹ�
//@EXECUTIONFLOW: 7���ظ�����4~5
//@EXECUTIONFLOW: 8������CSUDIPLAYERClose�رղ��Բ�����
CSUDI_BOOL CSTC_VIDEO_IT_Show_0003(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E enDev;
	CSUDISCREENResolution_E enRes;
	int i=0;
	BOOL bHDSupport = bIsSupportHD();
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");

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
		CSTCPrint("�߱��������Ƶ��%s(Y/N)\n",i?"��ͼ����ʾ":"����");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

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
		
		CSTCPrint("����ֱ������ó�NTSC,�������ó�720P_50HZ(�����),�߱����������%s(Y/N)\n",
			i?"��ʾ":"����");
		if (i)
		{
			CSTCPrint("������ݵĺ���������˸\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

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
		CSTCPrint("����ֱ������ó�PAL,�������ó�1080I_50HZ(�����),�߱����������%s(Y/N)\n",
			i?"��ʾ":"����");
		if (i)
		{
			CSTCPrint("������ݵĺ���������˸\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return TRUE;
}


/*����Ⱥ�����ģʽ���������������
|---------------------------------------------------------------------------------|      	
| ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������                            |
|---------------------------------------------------------------------------------|
|  4:3   | LETTER_BOX|  4:3    | ����   | ����������                              |
|  4:3   | LETTER_BOX|  4:3    | ����   | ���治���Σ������кڱ�                  |
|---------------------------------------------------------------------------------|
|  4:3   | LETTER_BOX| 16:9    | ����   | ����������                              |
|  4:3   | LETTER_BOX| 16:9    | ����   | �������Σ��������ҷ�������            |
|---------------------------------------------------------------------------------|
| 16:9   | LETTER_BOX|  4:3    | ����   | �������Σ����·�������                |
| 16:9   | LETTER_BOX|  4:3    | ����   | ����������                              |
|---------------------------------------------------------------------------------|
| 16:9   | LETTER_BOX| 16:9    | ����   | ���治���Σ������кڱ�                  |
| 16:9   | LETTER_BOX| 16:9    | ����   | ����������                              |
|---------------------------------------------------------------------------------|
|  4:3   | PAN_SCAN  |  4:3    | ����   | ����������                              |
|  4:3   | PAN_SCAN  |  4:3    | ����   | ���������Σ����·��������ݱ��ص�        |
|---------------------------------------------------------------------------------|
|  4:3   | PAN_SCAN  | 16:9    | ����   | ����������                              |
|  4:3   | PAN_SCAN  | 16:9    | ����   | ���������Σ����·��������ݱ��ص�        |
|---------------------------------------------------------------------------------|
| 16:9   | PAN_SCAN  |  4:3    | ����   | ���������Σ����ҷ��������ݱ��ص�        |
| 16:9   | PAN_SCAN  |  4:3    | ����   | ����������                              |
|---------------------------------------------------------------------------------|
| 16:9   | PAN_SCAN  | 16:9    | ����   | ���������Σ����ҷ��������ݱ��ص�        |
| 16:9   | PAN_SCAN  | 16:9    | ����   | ����������                              |
|---------------------------------------------------------------------------------|
|  4:3   | COMBINED  |  4:3    | ����   | ����������                              |
|  4:3   | COMBINED  |  4:3    | ����   | ���治���Σ���Ե�����ݱ��ص�            |
|---------------------------------------------------------------------------------|
|  4:3   | COMBINED  | 16:9    | ����   | ����������                              |
|  4:3   | COMBINED  | 16:9    | ����   | ���治���Σ���Ե�����ݱ��ص�            |
|---------------------------------------------------------------------------------|
| 16:9   | COMBINED  |  4:3    | ����   | ���治���Σ���Ե�����ݱ��ص�            |
| 16:9   | COMBINED  |  4:3    | ����   | ����������                              |
|---------------------------------------------------------------------------------|
| 16:9   | COMBINED  | 16:9    | ����   | ���治���Σ���Ե�����ݱ��ص�            |
| 16:9   | COMBINED  | 16:9    | ����   | ����������                              |
|---------------------------------------------------------------------------------|
|  4:3   | IGNORE    |  4:3    | ����   | ����������                              |
|  4:3   | IGNORE    |  4:3    | ����   | �������Σ��������ҷ�������            |
|---------------------------------------------------------------------------------|
|  4:3   | IGNORE    | 16:9    | ����   | ����������                              |
|  4:3   | IGNORE    | 16:9    | ����   | �������Σ��������ҷ�������            |
|---------------------------------------------------------------------------------|
| 16:9   | IGNORE    |  4:3    | ����   | �������Σ�ͼ�����·�������            |
| 16:9   | IGNORE    |  4:3    | ����   | ����������                              |
|---------------------------------------------------------------------------------|
| 16:9   | IGNORE    | 16:9    | ����   | �������Σ��������·�������            |
| 16:9   | IGNORE    | 16:9    | ����   | ����������                              |
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

//////////////////////���ý�Ŀ��Ϣ
	sStreamInfo[0].m_nPid=5137;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType=EM_UDI_VID_STREAM_MPEG2;

	sStreamInfo[1].m_nPid=5138;
	sStreamInfo[1].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[1].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_MPEG2;

	sPlayerChnl.m_nDemux=0;
	sPlayerChnl.m_nAudioDecoder=0;
	sPlayerChnl.m_nVideoDecoder=0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex,"Audio&Video Test_27Mbps_20070524.ts"), "��Ƶʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,ePlayerType,phPlayer)), "�򿪲�����ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL  != phPlayer, "�������������ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (*phPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "���ý�����Ϣʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(*phPlayer), "��ʼ����ʧ��!");
	CSUDIOSThreadSleep(1500);
	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���Ų�����");
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

	// ���videoID
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!\n");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
	   
	// ��Ƶ������
	if(s_aAspectRatioMAtchMethod_Table[nTestID].m_eStreamType == EM_STREAM_ASPECTRATIO_4_3)
	{ 
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == Stream16Ratio9LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
	}
    
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

	if (s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType == EM_UDI_VOUT_DEVICE_SD)
	{
       	CSTCPrint("�뽫��������˿����ӱ������(4:3)��ȷ��������Ƶ��ȷ���ų���!\n");
	}
	else
	{
		CSTCPrint("�뽫��������˿����Ӹ�����ӻ�(16:9)��ȷ��������Ƶ��ȷ���ų���!\n");
	}
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�޷���ð���ȷ��");

	// ����MatchMethod
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eMatchMode)), "SetMatchMode����ʧ��");
	// ���CSUDIVIDEOGetMatchMethod
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, &eMatchMethod)), "GetMatchMode����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((s_aAspectRatioMAtchMethod_Table[nTestID].m_eMatchMode == eMatchMethod), "��õ�eMatchMethod�����õ�eMatchMethod��һ��");

	// ����AspectRatio
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eAspectRatio)), "SetAspectRatio����ʧ��");
	// ���CSUDIVIDEOGetAspectRatio
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, &eAspectRatio)), "GetAspectRatio����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((s_aAspectRatioMAtchMethod_Table[nTestID].m_eAspectRatio == eAspectRatio), "��õ�eAspectRatio�����õ�eAspectRatio��һ��");

	CSTCPrint(" ��ƵԴ |  ��Ӧģʽ  | ����� | ����豸 \n");

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
       	CSTCPrint("�� �� \n");
	}
	else
	{
		CSTCPrint("�� �� \n");
	}

    CSTCPrint("ע��:Ҫ��ע��Ƶ���Ч��,�����Ǳ仯����!\n");

	CSTCPrint("��Ƶ���Ч���Ƿ�Ϊ:\"%s\"!\n", s_aAspectRatioMAtchMethod_Table[nTestID].m_pcDes);
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "�޷���ð���ȷ��");

	// ��Ӳ���Լ�������������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, 
						EM_UDIVIDEO_MATCH_METHOD_IGNORE)), "SetMatchMode����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, 
						EM_UDIVIDEO_ASPECT_RATIO_AUTO)), "SetAspectRatio����ʧ��");

	// ���CSUDIVIDEOGetAspectRatio,CSUDIVIDEOGetMatchMethod
	eAspectRatio = -1;
	eMatchMethod = -1;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, &eMatchMethod)), "GetMatchMode����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_IGNORE == eMatchMethod), "��õ�eMatchMethod�����õ�eMatchMethod��һ��");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(nVideoIndex, 
						s_aAspectRatioMAtchMethod_Table[nTestID].m_eScreenType, &eAspectRatio)), "GetAspectRatio����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_AUTO == eAspectRatio), "��õ�eAspectRatio�����õ�eAspectRatio��һ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��!");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"����������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�ӱ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ��������������������
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | ��  �� | ����������        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0001(void)
{
	int nTestID = 0;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"����������ʾ�����ҷ������� "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | ��  �� | ����������ʾ�����ҷ������� |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0002(void)
{
	int nTestID = 1;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"�������ҷ���ѹխ�������кڱ�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | ��  �� | �������ҷ���ѹխ�������кڱ�  |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0003(void)
{
	int nTestID = 2;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"������ʾ�����������кڱ�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | ��  �� | ������ʾ�����������кڱ�           |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0004(void)
{
	int nTestID = 3;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ" ������ʾ�����������кڱ� "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | ��  �� |  ������ʾ�����������кڱ�|
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0005(void)
{
	int nTestID = 4;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"�������ұ����죬�����кڱ�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | ��  �� | �������±�ѹ�⣬�����кڱ�     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0006(void)
{
	int nTestID = 5;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"�������Σ�ͼ�����·�������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�ӱ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | ��  �� | �������Σ�ͼ�����·�������        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0007(void)
{
	int nTestID = 6;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"����������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | ��  �� | ����������            |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0008(void)
{
	int nTestID = 7;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"����������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |PAN_SCAN |  4 : 3  | ��  �� | ����������        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0009(void)
{
	int nTestID = 8;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"����������ʾ�����ҷ�������      "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |PAN_SCAN |  4 : 3  | ��  �� |����������ʾ�����ҷ�������
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0010(void)
{
	int nTestID = 9;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ" ͼ�����·������죬���������ݱ��ص� "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |PAN_SCAN | 16 : 9  | ��  �� | ͼ�����·������죬���������ݱ��ص�     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0011(void)
{
	int nTestID = 10;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"���������Σ����·��������ݱ��ص� "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |PAN_SCAN | 16 : 9  | ��  �� | ���������Σ����·��������ݱ��ص�   |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0012(void)
{
	int nTestID = 11;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"���������Σ����ҷ��������ݱ��ص�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |PAN_SCAN |  4 : 3  | ��  �� |  ���������Σ����ҷ��������ݱ��ص� |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0013(void)
{
	int nTestID = 12;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"�������ұ����죬�������������ݱ��ص�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |PAN_SCAN |  4 : 3  | ��  �� |�������ұ����죬�������������ݱ��ص�     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0014(void)
{
	int nTestID = 13;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"�������Σ�ͼ�����·�������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |PAN_SCAN | 16 : 9  | ��  �� |  �������Σ�ͼ�����·������� |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0015(void)
{
	int nTestID = 14;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"����������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |PAN_SCAN | 16 : 9  | ��  �� | ����������            |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0016(void)
{
	int nTestID = 15;	
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"����������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | ��  �� | ����������        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0017(void)
{
	int nTestID = 16;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"����������ʾ�����ҷ������� "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | ��  �� |  ����������ʾ�����ҷ�������           |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0018(void)
{
	int nTestID = 17;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ" �������ҷ���ѹխ�������кڱߣ����������ݱ��ص� "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | ��  �� | �������ҷ���ѹխ�������кڱߣ����������ݱ��ص�     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0019(void)
{
	int nTestID = 18;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"������������Σ������кڱߣ����������ݱ��ص�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | ��  �� |������������Σ������кڱߣ����������ݱ��ص� |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0020(void)
{
	int nTestID = 19;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"������������Σ������кڱߣ����������ݱ��ص�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | ��  �� |  ������������Σ������кڱߣ����������ݱ��ص�|
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0021(void)
{
	int nTestID = 20;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ" �������ҷ������죬�����кڱߣ����������ݱ��ص�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX |  4 : 3  | ��  �� |  �������ҷ������죬�����кڱߣ����������ݱ��ص�     |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0022(void)
{
	int nTestID = 21;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"�������Σ�ͼ�����·�������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | ��  �� | �������Σ�ͼ�����·�������|
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0023(void)
{
	int nTestID = 22;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�LETTER_BOX������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"����������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |LETTER BOX | 16 : 9  | ��  �� | ����������            |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0024(void)
{
	int nTestID = 23;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_IGNORE������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"����������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |IGNORE |  4 : 3  | ��  �� | ����������        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0025(void)
{
	int nTestID = 24;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_IGNORE������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"�������Σ��������ҷ������� "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |IGNORE |  4 : 3  | ��  �� |�������Σ��������ҷ������� |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0026(void)
{
	int nTestID = 25;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_IGNORE������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"����������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |IGNORE | 16 : 9  | ��  �� | ����������        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0027(void)
{
	int nTestID = 26;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����4:3��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_IGNORE������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ4:3����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"�������Σ��������ҷ�������    "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |IGNORE | 16 : 9  | ��  �� | �������Σ��������ҷ�������   |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0028(void)
{
	int nTestID = 27;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_IGNORE������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"�������Σ�ͼ�����·�������  "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |IGNORE |  4 : 3  | ��  �� |  �������Σ�ͼ�����·�������   |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0029(void)
{
	int nTestID = 28;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_IGNORE������ģʽ��4:3 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���Ч��Ϊ"���������� "
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |IGNORE |  4 : 3  | ��  �� | ����������        |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0030(void)
{
	int nTestID = 29;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_IGNORE������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_SD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"�������Σ��������·�������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |IGNORE | 16 : 9  | ��  �� |  �������Σ��������·������� |
//@REMARK: |-----------------------------------------------------------|
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0031(void)
{
	int nTestID = 30;
	return testMatchMethodAspectRatio(nTestID);
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
//@CASEGROUP:CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eVideoDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
//@DESCRIPTION:  ����16:9��Դ������ �������ͨ�������ó�EM_UDIVIDEO_MATCH_METHOD_IGNORE������ģʽ��16:9 �Ļ����ʱ�����Ч���� 
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ16:9����Ƶ������
//@INPUT:1��nVideoIndex   =  [0..nVideo-1]
//@INPUT:2��eVideoDevice  =  EM_UDI_VOUT_DEVICE_HD 
//@INPUT:3��eMatchMethod=  EM_UDIVIDEO_MATCH_METHOD_IGNORE 
//@INPUT:4��eAspectRatio  =  EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ���Ч��Ϊ"����������"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3������CSTKWaitYesKey����ʾ������Ա�Ӹ�������豸
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod(nVideoIndex,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 8������CSUDIVIDEOGetAspectRatio(nVideoIndex,EM_UDI_VOUT_DEVICE_SD,EM_UDIVIDEO_ASPECT_RATIO_4_3)
//@EXECUTIONFLOW: 9������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ��
//@EXECUTIONFLOW: 10������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ����Ų���ʾ���������һ��治���Σ������кڱ�
//@EXECUTIONFLOW: 11���ָ��ֳ�
//@REMARK: |-----------------------------------------------------------|      	
//@REMARK: | ����Դ | ��Ӧģʽ  |  ����� |����豸| ���Ч������      |
//@REMARK: | 4 : 3  |IGNORE | 16 : 9  | ��  �� | ����������            |
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
		CSTCPrint("%s�����ĿԴAspectRatioͬ�������һ��,����ȫ���������ѹ��(Y/N)\n",
			enDev==EM_UDI_VOUT_DEVICE_SD?"����":"����");
		return ;
	}

	if (enSource == EM_UDIVIDEO_ASPECT_RATIO_4_3)
	{
		switch(enMatchMethod)
		{
			case EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX:
				CSTCPrint("%s�����ĿԴ4:3 ,�������16:9,letter box �������ұ�ѹ���кڱ�(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"����":"����");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN:
				CSTCPrint("%s�����ĿԴ4:3 ,�������16:9,pan scan �������±�����,���»�����ʾ��ȫ(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"����":"����");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_COMBINED:
				CSTCPrint("%s�����ĿԴ4:3 ,�������16:9,combind �������±�����,���ұ�ѹ���кڱ�(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"����":"����");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_IGNORE:
				CSTCPrint("%s�����ĿԴ4:3 ,�������16:9,ignore ����ȫ���������ѹ��(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"����":"����");
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
				CSTCPrint("%s�����ĿԴ16:9 ,�������4:3,letter box ���汻����ѹ���кڱ�(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"����":"����");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN:
				CSTCPrint("%s�����ĿԴ16:9 ,�������4:3,pan scan �������ұ�����,���һ�����ʾ��ȫ(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"����":"����");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_COMBINED:
				CSTCPrint("%s�����ĿԴ16:9 ,�������4:3,combind �������ұ�����,���±�ѹ���кڱ�(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"����":"����");
				break;
			case EM_UDIVIDEO_MATCH_METHOD_IGNORE:
				CSTCPrint("%s�����ĿԴ16:9 ,�������4:3,ignore ����ȫ���������ѹ��(Y/N)\n",
					enDev==EM_UDI_VOUT_DEVICE_SD?"����":"����");
				break;
			default:
				break;
		}
	}
}

//@CASEGROUP: 1��video
//@CASEGROUP: 2��screen
//@DESCRIPTION:����aspect ratio ����,mach method ����,���ֱܷ��ʱ仯Ӱ��
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@EXPECTATION:aspect ratio ����,mach method ����,���ֱܷ��ʱ仯Ӱ��
//@EXECUTIONFLOW:1�����Ÿ����������(Դaspect ratio :16:9),�������ųɹ� 
//@EXECUTIONFLOW:2������CSUDISCREENSetResolution����������ֱ������ó�1080I_50HZ,�������óɹ�
//@EXECUTIONFLOW:3������CSUDIVIDEOSetAspectRatioͬʱ���߱���������ó�4:3
//@EXECUTIONFLOW:4������CSUDIVIDEOSetMatchMethod���߱������ͬʱ����Ϊletter box ģʽ
//@EXECUTIONFLOW:5��ȷ�����Ч��
//@EXECUTIONFLOW:6������CSUDISCREENSetResolution������ֱ������ó�1080I_50HZ,����ֱ������ó�NTSC
//@EXECUTIONFLOW:7��ȷ�����Ч��,�����ֱ��ʸı䲻Ӱ�컭�αȺ�match method������
//@EXECUTIONFLOW:8������CSUDISCREENSetResolution������ֱ������ó�720P_50HZ,����ֱ������ó�PAL
//@EXECUTIONFLOW:9��ȷ�����Ч��,�����ֱ��ʸı䲻Ӱ�컭�αȺ�match method������
//@EXECUTIONFLOW:10 ������CSUDIPLAYERClose�رղ�����
//@EXECUTIONFLOW:11������CSUDIVIDEOSetAspectRatio������aspect ratio���û�16:9,�ָ������ֳ�
//@EXECUTIONFLOW:12������CSUDIVIDEOSetMatchMethod���߱������ͬʱ���ignoreģʽ,�ָ������ֳ�
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0033(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E enDev;
	CSUDISCREENType_E enDevArray[]={EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E enRes[2];

	/*�����֧�ָ����Ŀֱ�Ӳ��Գɹ�?*/
	if (!bIsSupportHD())
	{
		return TRUE;
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "��Ƶʧ��!\n");

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
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

	enRes[0]=EM_UDISCREEN_RESOLUTION_NTSC;
	enRes[1]=EM_UDISCREEN_RESOLUTION_720P_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(enDevArray, enRes, 2),
		"fail to set resolution\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, enDev,EM_UDIVIDEO_ASPECT_RATIO_4_3),
		"fail to set aspect ratio\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
			"fail to set matchmethod\n");
	CSTCPrint("����ֱ������ó�NTSC,�������ó�720P_50HZ\n");
	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_SD);
	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_HD);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

	enRes[0]=EM_UDISCREEN_RESOLUTION_PAL;
	enRes[1]=EM_UDISCREEN_RESOLUTION_1080I_50HZ;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(enDevArray, enRes, 2),
		"fail to set resolution\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, enDev,EM_UDIVIDEO_ASPECT_RATIO_4_3),
		"fail to set aspect ratio\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, enDev, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
			"fail to set matchmethod\n");
	CSTCPrint("����ֱ������ó�PAL,�������ó�1080I_50HZ\n");
	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_SD);
	DumpVideoMsg(EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3,EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,EM_UDI_VOUT_DEVICE_HD);
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

	CSTK_FATAL_POINT;

	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
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
//@DESCRIPTION: ���Ը����ĿԴ(16:9)������»��ͱ�������matchmethod����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:ƽ̨֧�ָ����Ŀ����͸������
//@INPUT:1��nVideoIndex = 0
//@INPUT:2��SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts
//@EXPECTATION: ���ͱ�������matchmethod���ú�video���Ч����Ԥ��һ��
//@EXECUTIONFLOW: 1�����Ÿ�������������������ųɹ�
//@EXECUTIONFLOW: 2������CSUDIVIDEOSetMatchMethod���ø߱������Ϊletter box,ȷ��Ч��
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetMatchMethod���ø߱������Ϊpan scan,ȷ��Ч��
//@EXECUTIONFLOW: 4������CSUDIVIDEOSetMatchMethod���ø߱������Ϊcombind,ȷ��Ч��
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetMatchMethod���ø߱������Ϊignore,ȷ��Ч��
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetAspectRatio���ø߱������Ϊ4:3,�������سɹ�
//@EXECUTIONFLOW: 7 ���ظ�����2~5
//@EXECUTIONFLOW: 8 ������CSUDIPLAYERClose�رղ�����
//@EXECUTIONFLOW: 9 ������CSUDIVIDEOSetAspectRatio������������û�16:9�ָ������ֳ�
//@EXECUTIONFLOW: 10 ������CSUDIVIDEOSetMatchMethod���߱���match method ���û�ignoreģʽ,�ָ������ֳ�
CSUDI_BOOL CSTC_VIDEO_IT_MatchMethodAspectRatio_0034(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = -1;
	CSUDISCREENType_E enDev;
	int i=0;

	/*�����֧�ָ����Ŀֱ�Ӳ��Գɹ�?*/
	if (!bIsSupportHD())
	{
		return TRUE;
	}

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "��Ƶʧ��!\n");

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
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");
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
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");
	}

	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
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
//@DESCRIPTION: ����CSUDIVIDEOSetAspectRatio��nVideoIndex������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  {-1,nVideo,nVideo+1}
//@INPUT:2��eVideoDevice = EM_UDI_VOUT_DEVICE_HD,EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��eAspectRatio=  EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ{-1,nVideo,nVideo+1}�ڸ�ֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetAspectRatio����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:��nVideoIndexȡ�Ƿ�ֵʱ��CSUDIVIDEOSetAspectRatioӦ���ط�CSUDI_SUCCESS
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
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
	
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
						s_SetErrorAspectRatio[nVideoErrorIndex].m_eAspectRatio)), "�����жϴ���!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio
//@DESCRIPTION: ����CSUDIVIDEOSetAspectRatio��eVideoDevice������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eVideoDevice=�����ڵ��豸����
//@INPUT:3��eAspectRatio=  EM_UDIVIDEO_ASPECT_RATIO_16_9,EM_UDIVIDEO_ASPECT_RATIO_4_3
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡֵ[0..nVideo]��
//@EXECUTIONFLOW: 3��eVideoDeviceȡ�����ڵ��豸����
//@EXECUTIONFLOW: 4������CSUDIVIDEOSetAspectRatio����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0002(void)
{
	int nVideoCount =0;
	int nVideoIndex = 0;
	CSUDISCREENType_E  eVideoDevice = 0;
	CSUDIVIDEOAspectRatioType_E  eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_16_9;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetAspectRatio(nVideoIndex, eVideoDevice, eAspectRatio)),
							"�����жϴ���!(16:9)");
		
		eAspectRatio = EM_UDIVIDEO_ASPECT_RATIO_4_3;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetAspectRatio(nVideoIndex, eVideoDevice, eAspectRatio)),
							"�����жϴ���!(4:3)");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio
//@DESCRIPTION: ����CSUDIVIDEOSetAspectRatio��eAspectRatio������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eVideoDevice= EM_UDI_VOUT_DEVICE_HD,EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��eAspectRatio�Ƿ�
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڸ�ֵ
//@EXECUTIONFLOW: 3��eAspectRatioȡ�Ƿ�ֵ
//@EXECUTIONFLOW: 4������CSUDIVIDEOSetAspectRatio����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetAspectRatio_0003(void)
{
	int nVideoCount =0;
	int nVideoIndex = 0;
	CSUDISCREENType_E  eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
	CSUDIVIDEOAspectRatioType_E  eAspectRatio = -1;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		eVideoDevice = EM_UDI_VOUT_DEVICE_SD;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetAspectRatio(nVideoIndex, eVideoDevice, eAspectRatio)),
							"�����жϴ���!(SD)");
		
		eVideoDevice = EM_UDI_VOUT_DEVICE_HD;
		
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetAspectRatio(nVideoIndex, eVideoDevice, eAspectRatio)),
							"�����жϴ���!(HD)");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio
//@DESCRIPTION: ����CSUDIVIDEOSetAspectRatio����ͨ�����ϵĲ����ı䲻Ӱ�����ͨ���ϵ����Ч��
//@PRECONDITION:�߱��岻ͬԴ
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ����4:3�ı�����Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eVideoDevice= EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��eMatchMethod= EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX
//@INPUT:3��eAspectRatio=EM_UDIVIDEO_ASPECT_RATIO_16_9
//@EXPECTATION: ����ͨ�����û���Ȳ�Ӱ�����ͨ�������
//@EXECUTIONFLOW: 1���ж��Ƿ�߱���ͬԴ��������ֱ����������8
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ��������Ƶ������
//@EXECUTIONFLOW: 5���������Աȷ�ϸ������ͨ����ȷ�����Ƶ
//@EXECUTIONFLOW: 6�����ñ���ͨ����MatchMothed��AspectRatio
//@EXECUTIONFLOW: 7���������Աȷ�ϸ������ͨ������Ƶû���κα仯
//@EXECUTIONFLOW: 8���ָ��ֳ�
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!");
		
		CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
		   
		// 4
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency(&hPlayer)), "��Ƶʧ��!\n");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

		// 5
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)), "SetMatchMode����ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_4_3)), "SetAspectRatio����ʧ��");

		CSTCPrint("�뽫��������˿����ӵ��Ӳ�ȷ��������Ƶ��ȷ���ų���!\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�޷���ð���ȷ��");

		// 6
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)), "SetMatchMode����ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_16_9)), "SetAspectRatio����ʧ��");

		// 7
		CSTCPrint("��ȷ�ϸ�����Ƶ���û�з����κα仯!\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�޷���ð���ȷ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��!");
		
		bResult = CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("�߱���ͬԴ!\n");
	}
	
	CSTK_FATAL_POINT;
		
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetAspectRatio
//@DESCRIPTION: ����CSUDIVIDEOSetAspectRatio����ͨ�����ϵĲ����ı䲻Ӱ�����ͨ���ϵ����Ч��
//@PRECONDITION:�߱��岻ͬԴ
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ����16:9�ı�����Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eVideoDevice= EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��eAspectRatio=  EM_UDIVIDEO_ASPECT_RATIO_4_3
//@INPUT:4��eMatchMethod= EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN
//@EXPECTATION: ����ͨ���Ļ���Ȳ����ı䲻Ӱ�����ͨ�������Ч��
//@EXECUTIONFLOW: 1���ж��Ƿ�߱���ͬԴ��������ֱ����������8
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 3��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 4����nVideoIndex�豸��ѭ������һ��16:9 ��Ƶ������
//@EXECUTIONFLOW: 5���������Աȷ�ϱ������ͨ����ȷ�����Ƶ
//@EXECUTIONFLOW: 6�����ø���ͨ����MatchMothed��AspectRatio
//@EXECUTIONFLOW: 7���������Աȷ�ϱ������ͨ������Ƶû���κα仯
//@EXECUTIONFLOW: 8���ָ��ֳ�
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!");
		
		CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
		   
		// 4
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

		// 5
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)), "SetMatchMode����ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_4_3)), "SetAspectRatio����ʧ��");

		CSTCPrint("�뽫��������˿����ӵ��ӣ���ȷ��������Ƶ��ȷ���ų���!\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�޷���ð���ȷ��");

		// 6
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)), "SetMatchMode����ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(nVideoIndex, 
							EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_16_9)), "SetAspectRatio����ʧ��");

		// 7
		CSTCPrint("��ȷ�ϱ�����Ƶ���û�з����κα仯!\n");
		   
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�޷���ð���ȷ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��!");
			
		bResult = CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("�߱���ͬԴ!\n");
	}
	
	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetAspectRatio
//@DESCRIPTION: ����CSUDIVIDEOGetAspectRatio �ӿڶԴ�������ļ������
//@INPUT:1��nVideoIndex =  �Ƿ�
//@INPUT:2��eScreenDevice= �Ƿ�
//@INPUT:3��peAspectRatio=  �Ƿ�
//@EXPECTATION:�Ըýӿڴ��ݴ����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount ���ƽ̨video ������
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetAspectRatio ���ݴ��������ϣ������CSUDIVIDEO_ERROR_BAD_PARAMETER
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
	
	// ��� video ������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "���video ����ʧ��");

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


	//���������
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetAspectRatio(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eScreenDevice, ErrorParamList[i].m_peAspectRatio);
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == nResult), "���������ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIVIDEOSetMatchMethod
//@DESCRIPTION: ����CSUDIVIDEOSetMatchMethod��nVideoIndex������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ���Ų���ʾ��Ƶ������
//@INPUT:1��nVideoIndex =  {-1,nVideo,nVideo+1}
//@INPUT:2��eVideoDevice=EM_UDI_VOUT_DEVICE_HD,EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��eMatchMethod=  CSUDIVIDEOMatchMethod_E ö��ֵ
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ{-1,nVideo,nVideo+1}�ڸ�ֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetMatchMethod����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:��nVideoIndexȡ�Ƿ�ֵʱ��CSUDIVIDEOSetMatchMethodӦ���ط�CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

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
						"�����жϴ���!");
		}
	}
	
	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;

	return bResult; 
}

//@CASEGROUP:CSUDIVIDEOSetMatchMethod
//@DESCRIPTION: ����CSUDIVIDEOSetMatchMethod��eVideoDevice������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eVideoDevice�Ƿ�
//@INPUT:3��eMatchMethod=  CSUDIVIDEOMatchMethod_E ö��ֵ
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡֵ[0..nVideo]��
//@EXECUTIONFLOW: 3��eVideoDeviceȡ�Ƿ�ֵ
//@EXECUTIONFLOW: 4������CSUDIVIDEOSetMatchMethod����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetMatchMethod_0002(void)
{
	int nVideoCount =0;
	int nVideoIndex = 0;
	CSUDISCREENType_E  eVideoDevice = 0; 
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");	
	   
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		int nErrorMatchMethodIndex = 0;
		
		for (nErrorMatchMethodIndex = 0; nErrorMatchMethodIndex < EM_UDIVIDEO_MATCH_METHOD_NUM; nErrorMatchMethodIndex ++)
		{
			CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetMatchMethod(
								nVideoIndex, 
								eVideoDevice, 
								EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX + nErrorMatchMethodIndex)),
								"�����жϴ���!\n"); 
		}
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}


//@CASEGROUP:CSUDIVIDEOSetMatchMethod
//@DESCRIPTION: ����CSUDIVIDEOSetMatchMethod��eMatchMethod������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eVideoDevice=EM_UDI_VOUT_DEVICE_HD,EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��eMatchMethod�Ƿ�
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڸ�ֵ
//@EXECUTIONFLOW: 3��eMatchMethodȡ�Ƿ�ֵ
//@EXECUTIONFLOW: 4������CSUDIVIDEOSetMatchMethod����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetMatchMethod_0003(void)
{
	int nVideoCount =0;
	int nVideoIndex = 0;
	CSUDIVIDEOMatchMethod_E  eMatchMethod = EM_UDIVIDEO_MATCH_METHOD_NUM;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
	   
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetMatchMethod(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, eMatchMethod)),
							"�����жϴ���!(SD)");
				
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetMatchMethod(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, eMatchMethod)),
							"�����жϴ���!(HD)");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}



//@CASEGROUP:CSUDIVIDEOGetMatchMethod
//@DESCRIPTION: ����CSUDIVIDEOGetMatchMethod �ӿڶԴ�������ļ������
//@INPUT:1��nVideoIndex =  �Ƿ�
//@INPUT:2��eScreenDevice= �Ƿ�
//@INPUT:3��peMatchMethod=  �Ƿ�
//@EXPECTATION:�Ըýӿڴ��ݴ����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount ���ƽ̨video ������
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetMatchMethod ���ݴ��������ϣ������CSUDIVIDEO_ERROR_BAD_PARAMETER
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

	// ��� video ������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "���video ����ʧ��");

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

	//���������
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetMatchMethod(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eScreenDevice, ErrorParamList[i].m_peMatchMethod);
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == nResult), "���������ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIVIDEOGetAspectRatio,CSUDIVIDEOGetMatchMethod
//@DESCRIPTION: ������CSUDIVIDEOSetAspectRatio ,CSUDIVIDEOGetMatchMethod�ӿ���δ�����κβ��ŵ�ʱ�򱻵���
//@INPUT:1��nVideoIndex =  0
//@INPUT:2��eScreenDevice= EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��peAspectRatio=  �ǿ�
//@INPUT:4��peMatchMethod=  �ǿ�
//@EXPECTATION:�Ըýӿ�ʹ�ó��������߼����
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetAspectRatio, ϣ���ں���Χ��
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetMatchMethod, ϣ������Χ��
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetAspectRatioMatchMethod_0001(void)
{
	CSUDIVIDEOAspectRatioType_E	eAspectRatio = -1;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = -1;

	//������δ���в���ʱ���eAspectRatio��eMatchMethod �����
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, &eAspectRatio)), "CSUDIVIDEOGetAspectRatio ����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN < eAspectRatio) &&(eAspectRatio <= EM_UDIVIDEO_ASPECT_RATIO_AUTO), "eAspectRatio���ں���Χ��");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD, &eMatchMethod)), "CSUDIVIDEOGetMatchMethod����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX <= eMatchMethod) &&(eMatchMethod < EM_UDIVIDEO_MATCH_METHOD_NUM), "eMatchMethod���ں���Χ��");
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIVIDEOGetAspectRatio,CSUDIVIDEOGetMatchMethod
//@DESCRIPTION: ��������������֮��,CSUDIVIDEOSetAspectRatio��CSUDIVIDEOSetMatchMethod֮ǰʹ��CSUDIVIDEOGetAspectRatio��CSUDIVIDEOGetMatchMethod
//@INPUT:1��nVideoIndex =  0
//@INPUT:2��eScreenDevice= EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��peAspectRatio=  �ǿ�
//@INPUT:4��peMatchMethod=  �ǿ�
//@EXPECTATION:�Ըýӿ�ʹ�ó��������߼����
//@EXECUTIONFLOW: 1������LockFrequency ������Ƶ, �򿪲������Ȳ���
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetAspectRatio, ϣ���õ�����Χֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOGetMatchMethod, ϣ���õ�����Χֵ
//@EXECUTIONFLOW: 4���ͷŲ�����Դ
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetAspectRatioMatchMethod_0002(void)
{
	CSUDI_HANDLE 				hPlayer = CSUDI_NULL;
	CSUDIVIDEOAspectRatioType_E	eAspectRatio = -1;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = -1;
	   
	// ����
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

	//����eAspectRatio��eMatchMethod �����
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD, &eAspectRatio)), "step2 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN < eAspectRatio) &&(eAspectRatio <= EM_UDIVIDEO_ASPECT_RATIO_AUTO), "eAspectRatio���ں���Χ��");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD, &eMatchMethod)), "step3 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX <= eMatchMethod) &&(eMatchMethod < EM_UDIVIDEO_MATCH_METHOD_NUM), "eMatchMethod���ں���Χ��");

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOGetAspectRatio,CSUDIVIDEOGetMatchMethod
//@DESCRIPTION: ����CSUDIVIDEOGetAspectRatio �� CSUDIVIDEOGetMatchMethod�ӿ�����������,����,ֹ֮ͣ��ű�����
//@INPUT:1��nVideoIndex =  0
//@INPUT:2��eScreenDevice= EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��peAspectRatio=  �ǿ�
//@INPUT:4��peMatchMethod=  �ǿ�
//@EXPECTATION:�Ըýӿ�ʹ�ó��������߼����
//@EXECUTIONFLOW: 1������LockFrequency ������Ƶ, �򿪲������Ȳ���
//@EXECUTIONFLOW: 2������CSUDIVIDEOSetAspectRatio ���ò���ģʽ
//@EXECUTIONFLOW: 3���Ա���ͨ������CSUDIVIDEOSetMatchMethod ���ò���ģʽ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStop ֹͣ��Ƶ����
//@EXECUTIONFLOW: 5������CSUDIVIDEOGetAspectRatio, ϣ���õ��趨ֵ
//@EXECUTIONFLOW: 6������CSUDIVIDEOGetMatchMethod, ϣ���õ��趨ֵ
//@EXECUTIONFLOW: 7���Ը���ͨ��CSUDIVIDEOSetAspectRatio,CSUDIVIDEOSetMatchMethod ,������Ӱ��������õ�eAspectRatio��eMatchMethod
//@REMARK:None
CSUDI_BOOL CSTC_VIDEO_IT_GetAspectRatioMatchMethod_0003(void)
{
	CSUDI_HANDLE 				hPlayer = CSUDI_NULL;
	CSUDIVIDEOAspectRatioType_E	eAspectRatio = -1;
	CSUDIVIDEOMatchMethod_E 	eMatchMethod = -1;
	CSUDI_BOOL					bStopFlag = CSUDI_FALSE;
	   
	// ����
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

	// ���ò���
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, 
						EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_AUTO)), "SetAspectRatio����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, 
						EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX)), "SetMatchMethod����ʧ��");

	// ֹͣ����
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
	bStopFlag = CSUDI_TRUE;

	//����eAspectRatio��eMatchMethod �����
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, &eAspectRatio)), "step5 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_AUTO == eAspectRatio), "eAspectRatio�������õ�ֵ");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD, &eMatchMethod)), "step6 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX == eMatchMethod), "eMatchMethod�������õ�ֵ");

	//�Ը���ͨ��CSUDIVIDEOSetAspectRatio,CSUDIVIDEOSetMatchMethod 
	//������Ӱ�����ͨ�����õ�eAspectRatio��eMatchMethod
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, 
						EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_16_9)), "SetAspectRatio����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, 
						EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN)), "SetMatchMethod����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD, &eAspectRatio)), "step5 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_16_9 == eAspectRatio), "eAspectRatio�������õ�ֵ");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, &eAspectRatio)), "step5 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_ASPECT_RATIO_AUTO == eAspectRatio), "eAspectRatio�������õ�ֵ");
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD, &eMatchMethod)), "step6 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN == eMatchMethod), "eMatchMethod�������õ�ֵ");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD, &eMatchMethod)), "step6 fail!");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX == eMatchMethod), "eMatchMethod�������õ�ֵ");
	

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		if(!bStopFlag)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}
		
	return CSUDI_TRUE;
}

static CSUDI_BOOL testSetStopMode(CSUDIVIDEOStopMode_E eStopModeType)
{
	int nVideoCount = 0;	
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIVIDEOStopMode_E eStopMode = -1;
       
	CSTK_ASSERT_TRUE_FATAL(((EM_UDIVIDEO_STOPMODE_BLACK==eStopModeType)||(EM_UDIVIDEO_STOPMODE_FREEZE ==eStopModeType)),"��������\n");
	
	// 1
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
	   
	// 2-3
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
    
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");

	// 5
	CSTCPrint("�뽫��������˿����ӵ��Ӳ�ȷ��������Ƶ��ȷ���ų���!\n");
	   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�޷���ð���ȷ��");

	// 6
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0,eStopModeType)), "��ʾ��Ƶʧ��!\n");

	// 7
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS ==CSUDIPLAYERStop(hPlayer)), "ֹͣ����ʧ��!\n");

       // 8
	if (eStopModeType == EM_UDIVIDEO_STOPMODE_BLACK)
	{
		CSTCPrint("��۲�Ŀǰ��Ļ��Ƶ�Ǻ���״̬!\n");
		// ����GetStopMode �ļ��
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode), "GetStopMode ����ʧ��");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIVIDEO_STOPMODE_BLACK == eStopMode, "��õ�eStopMode�����õ�eStopMode��һ��");
	}
	else
	{
		CSTCPrint("��۲�Ŀǰ��Ļ��Ƶ�Ǿ�֡״̬!\n");
		// ����GetStopMode �ļ��
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode), "GetStopMode ����ʧ��");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIVIDEO_STOPMODE_FREEZE == eStopMode, "��õ�eStopMode�����õ�eStopMode��һ��");
	}
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "�޷���ð���ȷ��");

	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: ����CSUDIVIDEOSetStopMode��EM_UDIVIDEO_STOPMODE_BLACK����µ�ִ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eStopMode=EM_UDIVIDEO_STOPMODE_BLACK
//@EXPECTATION: ����CSUDI_SUCCESS����Ƶ���ֺ���
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetStopMode(nVideoIndex,EM_UDIVIDEO_STOPMODE_BLACK)�����жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�nVideoIndex�豸�ϵ���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ�Ƿ����
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0001(void)
{		
	return testSetStopMode(EM_UDIVIDEO_STOPMODE_BLACK);
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: ����CSUDIVIDEOSetStopMode��EM_UDIVIDEO_STOPMODE_FREEZE����µ�ִ��Ч��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eStopMode=EM_UDIVIDEO_STOPMODE_FREEZE
//@EXPECTATION: ����CSUDI_SUCCESS����Ƶ���־�֡
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�һ��ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetStopMode(nVideoIndex,EM_UDIVIDEO_STOPMODE_FREEZE)�����жϷ���ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7���ر�nVideoIndex�豸�ϵ���Ƶ����
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ�Ƿ�֡
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0002(void)
{
	return testSetStopMode(EM_UDIVIDEO_STOPMODE_FREEZE);
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: ����CSUDIVIDEOSetStopMode��nVideoIndex������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex = {-1,nVideo,nVideo+1}
//@INPUT:2��eStopMode=CSUDIVIDEOStopMode_Eö��ֵ
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ{-1,nVideo,nVideo+1}�ڵ�ֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetStopMode(nVideoIndex,...)�����жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");

    CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

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
							"��Ƶֹͣģʽ �����ж�ʧ��!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: ����CSUDIVIDEOSetStopMode��eStopMode�����Ƿ�����µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex = [0,nVideo-1]
//@INPUT:2��eStopModeȡ�Ƿ�ֵ
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0,nVideo-1]�ڵ�ֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetStopMode(nVideoIndex,...)�����жϷ���ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetStopMode_0004(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDIVIDEOStopMode_E  m_eStopModeErrorType =  EM_UDIVIDEO_STOPMODE_FREEZE + 10;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSetStopMode(nVideoIndex, m_eStopModeErrorType)), 
							"��Ƶֹͣģʽ �����ж�ʧ��!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
		
	return bResult;
}

//ѭ���л���Ŀ
void CSUDI_VIDEO_ChangeProgram(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S* sStreamInfo)
{
	int nStreamCnt = 2;

	if(CSUDI_SUCCESS != CSUDIVIDEOShow(0, CSUDI_FALSE))
	{
		CSTCPrint("����8��������Ƶ����ʧ��!\n");
	       return;
	}

	if(CSUDI_SUCCESS != CSUDIPLAYERStop(hPlayer))
	{
	    CSTCPrint("����9��ֹͣ����ʧ��!\n");
	    return;
	}
	    
	if(CSUDI_SUCCESS != CSUDIPLAYERSetStream(hPlayer, sStreamInfo, nStreamCnt, CSUDI_NULL))
	{
	    CSTCPrint("����10�����ý�����Ϣʧ��!\n");
	    return;
	}

	CSUDIOSThreadSleep(1000);

	if(CSUDI_SUCCESS != CSUDIPLAYERStart(hPlayer))
	{
	    CSTCPrint("����11����ʼ����ʧ��!\n");
	    return;
	}

	if(CSUDI_SUCCESS != CSUDIVIDEOShow(0, CSUDI_TRUE))
	{
	    CSTCPrint("����12����ʾ��Ƶ����ʧ��!\n");
	    return;
	}

	CSUDIOSThreadSleep(3000);
}

//@CASEGROUP:CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
//@DESCRIPTION: ģ�������Ŀ�л�����������̨Ϊ��֡ģʽ
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eStopMode=EM_UDIVIDEO_STOPMODE_FREEZE
//@EXPECTATION: �ڽ�Ŀ�л������У�����û�в���֡
//@EXECUTIONFLOW: 1����ȡ��Ƶ���������������ٴ���һ����Ƶ������
//@EXECUTIONFLOW: 2������һƵ�㣬��Ƶ���ϲ��ŵ�����ΪAudio&Video Test_27Mbps_20070524.ts
//@EXECUTIONFLOW: 3������CSUDIPLAYEROpen�򿪲���������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIPLAYERSetStream���ý�����Ϣ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIPLAYERStart��ʼ���ţ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIVIDEOShow��ʾ��Ƶ���ڣ������ɹ�
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetStopMode����Ϊ��֡ģʽ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 8����CSUDIVIDEOShow���ش��ڣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIPLAYERStopֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10������CSUDIPLAYERSetStream���ý�Ŀ���ԣ���������CSUDI_SUCCESS 
//@EXECUTIONFLOW: 11������CSUDIPLAYERStart��������ͬƵ����һ����Ŀ���۲��Ƿ���ǰһ����Ŀ����֡������û��
//@EXECUTIONFLOW: 12������CSUDIVIDEOShow��ʾ���ڣ���������CSUDI_SUCCESS 
//@EXECUTIONFLOW: 13���ظ�����(8~12)5��,��ͬһƵ���������ͬ��Ŀ���л�
//@EXECUTIONFLOW: 14������CSUDIPLAYERStopֹͣ���ţ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 15������CSUDIPLAYERClose�رղ���������������CSUDI_SUCCESS
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

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "����1����ȡVideo����ʧ��!");
    
    CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0), "û�п��õ���Ƶ������");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_UDI2PortingLock(nTunerIndex, "Audio&Video Test_27Mbps_20070524.ts"), "����2����Ƶʧ��!");

    CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer)), "����3���򿪲�����ʧ��!");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hPlayer, "�������������ʧ��!");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer, sStreamInfo, nStreamCnt, CSUDI_NULL), "����4�����ý�����Ϣʧ��!");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����5����ʼ����ʧ��!");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE), "����6����ʾ��Ƶ������ʧ��");

    CSTCPrint("�뽫����������ӵ����������\n");
    
    CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"���Ų�����");

    CSTCPrint("ͨ�����ú���ģ��Ӽ�����Ŀ�л���������Ŀ����ע��۲��Ƿ����ϸ���Ŀ�Ĳ���֡(ѭ��5��)\n");

    CSTKWaitAnyKey();
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0, EM_UDIVIDEO_STOPMODE_FREEZE), "����7�����þ�֡ʧ��!");

    for(i = 0; i < 5; i++)
    {
        CSUDI_VIDEO_ChangeProgram(hPlayer, &sStreamInfo[2]);
    
        CSUDI_VIDEO_ChangeProgram(hPlayer, &sStreamInfo[0]);
    }

    CSTCPrint("��ѭ��5����̨�����У�û�в�����һ����Ŀ��I֡�밴'Y'������'N'\n");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���ϸ���Ŀ��֡����");

    CSTK_FATAL_POINT;

    if(CSUDI_NULL != hPlayer)
    {
        CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), "����14��ֹͣ����ʧ��!");

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����15���رղ�����ʧ��!");
    }
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOGetStopMode
//@DESCRIPTION: ����CSUDIVIDEOGetStopMode �ӿڶԴ�������ļ������
//@INPUT:1��nVideoIndex =  �Ƿ�
//@INPUT:2��peStopMode = �Ƿ�
//@EXPECTATION:�Ըýӿڴ��ݴ����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount ���ƽ̨video ������
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetStopMode ���ݴ��������ϣ������CSUDIVIDEO_ERROR_BAD_PARAMETER
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
			
	// ��� video ������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "���video ����ʧ��");

	ErrorParamList[0].m_nVideoIndex = -1;
	ErrorParamList[0].eStopMode = &eStopMode;

	ErrorParamList[1].m_nVideoIndex = nVideoCount;
	ErrorParamList[1].eStopMode = &eStopMode;

	ErrorParamList[2].m_nVideoIndex = nVideoIndex;
	ErrorParamList[2].eStopMode = CSUDI_NULL;

	//���������
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetStopMode(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].eStopMode);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "���������ʧ��");
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}


static CSUDI_BOOL testGetStopMode(CSUDI_HANDLE hPlayer, CSUDIVIDEOStopMode_E eSetStopMode)
{
	CSUDIVIDEOStopMode_E 	eGetStopMode = -1;
	CSUDI_BOOL 				bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����ʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0, eSetStopMode)), "CSUDIVIDEOSetStopMode ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eGetStopMode)), "CSUDIVIDEOGetStopModeʧ��");
	CSTK_ASSERT_TRUE_FATAL((eSetStopMode == eGetStopMode), "��õ�eStopMode����ȷ");

	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;
	
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOGetStopMode
//@DESCRIPTION: ����CSUDIVIDEOGetStopMode �ӿ�����Ƶ����,��ͣ,����ֹͣģʽ֮ǰ���õ����
//@INPUT:1��nVideoIndex =  0
//@INPUT:2��peStopMode = EM_UDIVIDEO_STOPMODE_BLACK,EM_UDIVIDEO_STOPMODE_FREEZE
//@EXPECTATION:�Ըýӿ�ʹ�ó��������߼����
//@EXECUTIONFLOW: 1����δ���в���֮ǰCSUDIVIDEOGetStopMode,ϣ���ɹ�,������ȷ��Χ��
//@EXECUTIONFLOW: 2���ڲ���ʱCSUDIVIDEOGetStopMode,ϣ���ɹ�,������ȷ��Χ��
//@EXECUTIONFLOW: 3������ͣʱCSUDIVIDEOGetStopMode,ϣ���ɹ�,������ȷ��Χ��
//@EXECUTIONFLOW: 4����CSUDIVIDEOSetStopMode ֮ǰ,StopPlayer֮��CSUDIVIDEOGetStopMode,ϣ���ɹ�,������ȷ��Χ��
//@EXECUTIONFLOW: 5����StopPlayer֮��CSUDIVIDEOSetStopMode ,����CSUDIVIDEOGetStopMode,ϣ������趨ֵEM_UDIVIDEO_STOPMODE_BLACK
//@EXECUTIONFLOW: 6����CSUDIVIDEOSetStopMode ֮��,StopPlayer֮��CSUDIVIDEOGetStopMode,ϣ������趨ֵEM_UDIVIDEO_STOPMODE_FREEZE
//@EXECUTIONFLOW: 7����ClosePlayer֮��CSUDIVIDEOGetStopMode,ϣ���ɹ�
CSUDI_BOOL CSTC_VIDEO_IT_GetStopMode_0002(void)
{
	CSUDIVIDEOStopMode_E 		eStopMode = -1;
	CSUDI_HANDLE 				hPlayer = CSUDI_NULL;
	CSUDI_BOOL					bStopFlag = CSUDI_FALSE;

	//����ǰ����CSUDIVIDEOGetStopMode
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "��������ǰCSUDIVIDEOGetStopModeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_BLACK <= eStopMode) &&(eStopMode <=EM_UDIVIDEO_STOPMODE_FREEZE), "��õ�eStopMode������ȷ��Χ��");


	// ����ʱ����CSUDIVIDEOGetStopMode
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "��������ʱCSUDIVIDEOGetStopModeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_BLACK <= eStopMode) &&(eStopMode <=EM_UDIVIDEO_STOPMODE_FREEZE), "��õ�eStopMode������ȷ��Χ��");
	
	//ֹͣ����ʱCSUDIVIDEOGetStopMode(δ����CSUDIVIDEOSetStopMode)
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
	bStopFlag = CSUDI_TRUE;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "CSUDIVIDEOGetStopModeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_BLACK <= eStopMode) &&(eStopMode <=EM_UDIVIDEO_STOPMODE_FREEZE), "��õ�eStopMode������ȷ��Χ��");

	//ֹͣ���ź�CSUDIVIDEOSetStopMode
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0, EM_UDIVIDEO_STOPMODE_BLACK)), "����������ҲӦ�����óɹ�");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "����������CSUDIVIDEOGetStopMode����Ӧ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_BLACK == eStopMode), "��õ�eStopMode��������ֵ");

	//ֹͣ����ǰCSUDIVIDEOSetStopMode,CSUDIVIDEOGetStopMode ϣ�����EM_UDIVIDEO_STOPMODE_FREEZE
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == testGetStopMode(hPlayer, EM_UDIVIDEO_STOPMODE_FREEZE), "Step6 fail");

	//�رղ��ź�CSUDIVIDEOGetStopMode
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	hPlayer = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetStopMode(0, &eStopMode)), "�رղ�������CSUDIVIDEOGetStopModeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIVIDEO_STOPMODE_FREEZE == eStopMode), "��õ�eStopMode�������õ���ȷֵ");
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		if(!bStopFlag)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//Ԥ��δʹ�ò���nVideoIndex,�Ա��Ժ���չ
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������");

	if (EM_UDI_VOUT_DEVICE_SD== eScreenDevice)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!");
	}
	else
	{
		CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &enResBak);
		if(enResBak != enRes)
		{
			CSUDISCREENSetResolution(&enDev, &enRes,1);
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "��Ƶʧ��!");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "�޷�����Ƶ��!");

	//��ȡȫ����Ƶ���ڴ�С
	memset(&sFullWinRect, 0, sizeof(sFullWinRect));
	CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL);
	CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &sFullWinRect);

	nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, pstRect);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult)||(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult), "CSUDIVIDEOSetWindowSize����ʧ��");

	if(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "CSUDIVIDEOSetWindowSize not support!!!");
	}
	
	if(CSUDI_SUCCESS == nResult)
	{
		//���CSUDIVIDEOGetWindowSize
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "CSUDIVIDEOGetWindowSize����ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, pstRect, &stGetRect), "��õ�Rect�����õĲ�һ��");
	}

	if (EM_UDI_VOUT_DEVICE_HD== eScreenDevice)
	{
		CSTCPrint("����ȫ����Ƶ����Ϊ{%d, %d, %d, %d}����ȷ�ϸ��������Ƶ�����Ƿ�Ϊ:\"%s\"!\n", \
				sFullWinRect.m_nX, sFullWinRect.m_nX, sFullWinRect.m_nWidth, sFullWinRect.m_nHeight, s_aSetWindowSizeTestCase[nTestID].m_pcDes);
	}
	else
	{
		CSTCPrint("����ȫ����Ƶ����Ϊ{%d, %d, %d, %d}����ȷ�ϱ��������Ƶ�����Ƿ�Ϊ:\"%s\"!\n",\
				sFullWinRect.m_nX, sFullWinRect.m_nX, sFullWinRect.m_nWidth, sFullWinRect.m_nHeight, s_aSetWindowSizeTestCase[nTestID].m_pcDes);
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "������Ԥ�費��!");

	if ((EM_UDI_VOUT_DEVICE_HD == eScreenDevice) && (enResBak != enRes))
	{
		CSUDISCREENSetResolution(&enDev, &enResBak,1);
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSizeʧ��!");

	//���CSUDIVIDEOGetWindowSize
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "CSUDIVIDEOGetWindowSize����ʧ��!");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "��õ�Rect�����õĲ�һ��");

	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSizeʧ��!");

	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
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
		   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������!");

	if (EM_UDI_VOUT_DEVICE_SD== eScreenDevice)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "��Ƶʧ��!");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "�޷�����Ƶ��!");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, pstRect)), "SetWindowSizeδʧ��!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "�رղ�����ʧ��!");
 
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL)), "��ԭȫ��WindowSizeʧ��!");

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRect=CSUDI_NULL����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRect=CSUDI_NULL
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ��ȫ����ʾ
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,CSUDI_NULL)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"ȫ��Ļ��ʾ"
//@REMARK:
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0001(void)
{
	int nTestID = 0;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{0,       0,      720/2,  576/2}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRectָ�����ݽṹ{0,       0,      720/2,  576/2}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���Ͻ�{0, 0, 720/2, 576/2}����Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"λ����Ļ���Ͻǣ�{0, 0, 720/2, 576/2}����Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0002(void)
{
	int nTestID = 1;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{720/2,576/2,720/3,  576/9}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRectָ�����ݽṹ{720/2,576/2,720/3,  576/9}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���·�{720/2, 576/2, 720/3, 576/3}����Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"λ����Ļ���·�{720/2, 576/2, 720/3, 576/3}����Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0003(void)
{
	int nTestID = 2;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{50,     50,     720,     576   }����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRectָ�����ݽṹ{50,     50,     720,     576   }
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"ͼ�����·�����λ50�����أ��Ƴ���Ļ�Ĳ��ֱ��г�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"ͼ�����·�����λ50�����أ��Ƴ���Ļ�Ĳ��ֱ��г�"
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
		   
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!\n");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "�޷�����Ƶ��!\n");

	CSTCPrint("��ȷ�ϱ������ͨ������Ƶ�Ƿ�ɹ���ʾ:��ʼ����Ϊ(0,0)��ȫ����ʾ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 5 ȷ�Ͻ��ʧ�� !!!!");
		
	nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, pstRect);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult)||(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult), "CSUDIVIDEOSetWindowSize����ʧ��");

	if(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "CSUDIVIDEOSetWindowSize not support!!!");
	}
	
	CSTCPrint("��ȷ����ʾ����ƵΪ:ͼ�����·�����λ50�����أ��Ƴ���Ļ�Ĳ��ֱ��г���\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 7 ȷ�Ͻ��ʧ�� !!!!");
            
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSizeʧ��!\n");

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{0,       0,      720/3,  576/3}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRectָ�����ݽṹ{0,       0,      720/3,  576/3}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���Ͻ�{0, 0, 720/3, 576/3}����Сͼ��""
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"λ����Ļ���Ͻ�{0, 0, 720/3, 576/3}����Сͼ��""
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0005(void)
{
	int nTestID = 4;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{720/2, 0,      720/3,  576/2}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRectָ�����ݽṹ{720/2, 0,      720/3,  576/2}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���Ϸ�{720/2, 0, 720/3, 576/2}��������Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"λ����Ļ���Ϸ�{720/2, 0, 720/3, 576/2}��������Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0006(void)
{
	int nTestID = 5;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{      0, 576/2,720/2,  576/3}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRectָ�����ݽṹ{      0, 576/2,720/2,  576/3}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���·�{0, 576/2, 720/2, 576/3}��ѹ����Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"λ����Ļ���·�{0, 576/2, 720/2, 576/3}��ѹ����Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0007(void)
{
	int nTestID = 6;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{720/3,576/3, 720/3,  576/3}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRectָ�����ݽṹ{720/3,576/3, 720/3,  576/3}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ�м�λ��{720/3, 576/3, 720/3, 576/3}����Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"λ����Ļ�м�λ��{720/3, 576/3, 720/3, 576/3}����Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0008(void)
{
	int nTestID = 7;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{-50,   -50,     720,     576   }����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRectָ�����ݽṹ{-50,   -50,     720,     576   }
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"ͼ�����Ϸ�����λ50������,�Ƴ���Ļ�Ĳ��ֱ��г�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"ͼ�����Ϸ�����λ50������,�Ƴ���Ļ�Ĳ��ֱ��г�"
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "�޷�����Ƶ��!\n");

	CSTCPrint("��ȷ�ϱ������ͨ������Ƶ�Ƿ�ɹ���ʾ:��ʼ����Ϊ(0,0)��ȫ����ʾ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 5 ȷ�Ͻ��ʧ�� !!!!");

	nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, pstRect);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult)||(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult), "CSUDIVIDEOSetWindowSize����ʧ��");

	if(CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED == nResult)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "CSUDIVIDEOSetWindowSize not support!!!");
	}

	CSTCPrint("��ȷ����ʾ����ƵΪ:ͼ�����Ϸ�����λ50������,�Ƴ���Ļ�Ĳ��ֱ��г���\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 7 ȷ�Ͻ��ʧ�� !!!!");
      
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSizeʧ��!\n");

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{720/2, 576/2,     -1,     -1  }����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@INPUT:3��pstRectָ�����ݽṹ{720/2, 576/2,     -1,     -1  }
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0010(void)
{
	int nTestID= 9;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
	return testSetWindowSizeError(nTestID, eScreenDevice);
}


//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRect=CSUDI_NULL����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRect=CSUDI_NULL
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ��ȫ����ʾ
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,CSUDI_NULL)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ�ϸ�������˿���ʾ����ƵΪ"ȫ��Ļ��ʾ"
//@REMARK:
CSUDI_BOOL  CSTC_VIDEO_IT_SetWindowSize_0011(void)
{
	int nTestID = 10;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{0,		 0, 	 1920/2,  1080/2}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRectָ�����ݽṹ{0,		 0, 	 1920/2,  1080/2}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���Ͻ�{0, 0, 1920/2, 1080/2}����Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ�ϸ�������˿���ʾ����ƵΪ"λ����Ļ���Ͻ�{0, 0, 1920/2, 1080/2}����Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0012(void)
{
	int nTestID = 11;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{1920/2,1080/2,1920/3,	1080/3}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRectָ�����ݽṹ{1920/2,1080/2,1920/3,	1080/3}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���·�{1920/2, 1080/2, 1920/3, 1080/3}����Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ�ϸ�������˿���ʾ����ƵΪ"λ����Ļ���·�{1920/2, 1080/2, 1920/3, 1080/3}����Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0013(void)
{
	int nTestID = 12;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{50, 	50, 	1920,	 1080   }����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRectָ�����ݽṹ{50, 	50, 	1920,	 1080   }
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"ͼ�����·�����λ50�����أ��Ƴ���Ļ�Ĳ��ֱ��г�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ�ϸ�������˿���ʾ����ƵΪ"ͼ�����·�����λ50�����أ��Ƴ���Ļ�Ĳ��ֱ��г�"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0014(void)
{
	int nTestID = 13;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{0,       0,      1920/3,  1080/3}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRectָ�����ݽṹ{0,       0,      1920/3,  1080/3}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���Ͻ�{0, 0, 1920/3, 1080/3}����Сͼ��""
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ�ϸ�������˿���ʾ����ƵΪ"λ����Ļ���Ͻ�{0, 0, 1920/3, 1080/3}����Сͼ��""
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0015(void)
{
	int nTestID = 14;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{1920/2, 0,      1920/3,  1080/2}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRectָ�����ݽṹ{1920/2, 0,      1920/3,  1080/2}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���Ϸ�{1920/2, 0, 1920/3, 1080/2}��������Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ�ϸ�������˿���ʾ����ƵΪ"λ����Ļ���Ϸ�{1920/2, 0, 1920/3, 1080/2}��������Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0016(void)
{
	int nTestID = 15;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{      0, 1080/2,1920/2,  1080/3}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRectָ�����ݽṹ{      0, 1080/2,1920/2,  1080/3}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���·�{0, 1080/2, 1920/2, 1080/3}��ѹ����Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ�ϸ�������˿���ʾ����ƵΪ"λ����Ļ���·�{0, 1080/2, 1920/2, 1080/3}��ѹ����Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0017(void)
{
	int nTestID = 16;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{1920/3,1080/3, 1920/3,  1080/3}����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRectָ�����ݽṹ{1920/3,1080/3, 1920/3,  1080/3}
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ�м�λ��{1920/3, 1080/3, 1920/3, 1080/3}����Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ�ϸ�������˿���ʾ����ƵΪ"λ����Ļ�м�λ��{1920/3, 1080/3, 1920/3, 1080/3}����Сͼ��"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0018(void)
{
	int nTestID = 17;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{-50,   -50,     1920,     1080   }����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRectָ�����ݽṹ{-50,   -50,     1920,     1080   }
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"ͼ�����Ϸ�����λ50������,�Ƴ���Ļ�Ĳ��ֱ��г�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ�ϸ�������˿���ʾ����ƵΪ"ͼ�����Ϸ�����λ50������,�Ƴ���Ļ�Ĳ��ֱ��г�"
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0019(void)
{
	int nTestID = 18;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSize(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWindowSize��pstRectָ�����ݽṹ{1920/2, 1080/2,     -1,     -1  }����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@INPUT:3��pstRectָ�����ݽṹ{1920/2, 1080/2,     -1,     -1  }
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ�������˿���Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0020(void)
{
	int nTestID= 19;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWindowSizeError(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��nVideoIndex������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex ={-1,nVideo,nVideo+1}
//@INPUT:2��eScreenDevice =  [EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD]
//@INPUT:3��pstRect=CSUDIWinRect_S�ṹָ��
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ{-1,nVideo,nVideo+1}�ڵ�ֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

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
						s_SetErrorWinSizeType[nVideoErrorIndex].m_eScreenDevice, CSUDI_NULL)), "�����жϴ���!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize��eScreenDevice������������µ�ִ�н��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex ={0...nVideo-1}
//@INPUT:2��eScreenDevice =  [Error]
//@INPUT:3��pstRect=CSUDIWinRect_S�ṹָ��
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��eScreenDeviceȡError ֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0022(void)
{
	int nVideoCount = 0;
	int nVideoIndex = 0;
	CSUDISCREENType_E eScreenDevice = 0;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL)), "�����жϴ���!");
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
			CSTCPrint("��ȷ�Ͻ�%s �������ΪС����(%d,%d,%d,%d),%sά�ֲ���(Y/N)\n",
			enDev == EM_UDI_VOUT_DEVICE_SD?"����":"����",
			pstRect->m_nX,pstRect->m_nY,pstRect->m_nWidth,pstRect->m_nHeight,
			enDev == EM_UDI_VOUT_DEVICE_SD?"����":"����");
		}
		else
		{
			CSTCPrint("��ȷ�Ͻ�%s ����Ϊȫ��,%sά�ֲ���(Y/N)\n",
			enDev == EM_UDI_VOUT_DEVICE_SD?"����":"����",
			enDev == EM_UDI_VOUT_DEVICE_SD?"����":"����");
		}
		bGetYes = CSTKWaitYes();

		return bGetYes;
	}
	else
	{
		if (pstRect)
		{
			CSTCPrint("����%s �������Ϊ(%d,%d,%d,%d) ʧ��\n",
				enDev == EM_UDI_VOUT_DEVICE_SD?"����":"����",
				pstRect->m_nX,pstRect->m_nY,pstRect->m_nWidth,pstRect->m_nHeight);
		}
		else
		{
			CSTCPrint("����%s �������Ϊȫ�� ʧ��\n",
				enDev == EM_UDI_VOUT_DEVICE_SD?"����":"����");
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

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");

	while(1)
	{
		if (nCountSD > 0)
		{
			if (CSUDI_SUCCESS != CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,pstWindowSD))
			{
				CSTCPrint("��������(%d,%d,%d,%d) ʧ��\n",
					pstWindowSD->m_nX,pstWindowSD->m_nY,
					pstWindowSD->m_nWidth,pstWindowSD->m_nHeight);
			}
			
			if (bCheckResult)
			{
				CSTCPrint("������Ƶ����Ϊ(%d,%d,%d,%d),",
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
				CSTCPrint("��������(%d,%d,%d,%d) ʧ��\n",
					pstWindowHD->m_nX,pstWindowHD->m_nY,
					pstWindowHD->m_nWidth,pstWindowHD->m_nHeight);
			}
			if (bCheckResult)
			{
				CSTCPrint("������Ƶ����Ϊ(%d,%d,%d,%d),",
					pstWindowHD->m_nX,pstWindowHD->m_nY,
					pstWindowHD->m_nWidth,pstWindowHD->m_nHeight);
			}
			pstWindowHD++;
			nCountHD--;
		}
		
		if (bCheckResult)
		{
			CSTCPrint("���Ч����ȷ,��û�ж�����˸���쳣(Y/N)\n");
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
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
//@DESCRIPTION: ����ƽ̨����Ƶ������Сǿ�Ȳ���,��ȫ����1*1
//@INPUT:1. nVideoIndex =0
//@EXPECTATION: ��Ƶ�����ӶԽ��������������Ϊ1*1ʱ,ƽ̨������
//@EXECUTIONFLOW: 1. ���Ų�����,�������ųɹ�
//@EXECUTIONFLOW: 2.  ���ƽ̨֧�ָ������ø���ֱ���ΪDISCREEN_RESOLUTION_1080I_50HZ
//@EXECUTIONFLOW: 3. ���ñ��������С����ģʽ,�߿�����Сֱ��1*1������������
//@EXECUTIONFLOW: 4. ���ø��������С����ģʽ,�߿�����Сֱ��1*1������������
//@EXECUTIONFLOW: 5. ����CSUDIPLAYERClose�رղ��Բ�����
//@EXECUTIONFLOW: 6.  ����CSUDIVIDEOSetWindowSize���ø߱��嵽ȫ��,�ָ������ֳ�
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
	/*���һ�����Ϊ0,������Ϊ1*/
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
		/*���һ�����Ϊ0,������Ϊ1*/
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
//@DESCRIPTION:  ����ƽ̨����Ƶ���ڵ���ˮƽ�����Ϻʹ�ֱ�����ϵ���Сǿ�Ȳ���,��ȫ����width*1,1*weight
//@INPUT:1. nVideoIndex =0
//@EXPECTATION: ��Ƶ������ˮƽ��������������1*576(SD)��1*1080(HD),����������
//@EXPECTATION: ��Ƶ�����ش�ֱ��������������720*1(SD)��1920*1(HD),����������
//@EXECUTIONFLOW: 1. ���Ų�����,�������ųɹ�
//@EXECUTIONFLOW: 2.  ���ƽ̨֧�ָ������ø���ֱ���ΪDISCREEN_RESOLUTION_1080I_50HZ
//@EXECUTIONFLOW: 3. ���ñ��������С����ģʽ,���ش�ֱ����������Сֱ��1*576������������
//@EXECUTIONFLOW: 3. ���ø��������С����ģʽ,���ش�ֱ����������Сֱ��1*1080������������
//@EXECUTIONFLOW: 3. ���ñ��������С����ģʽ,����ˮƽ����������Сֱ��720*1������������
//@EXECUTIONFLOW: 3. ���ø��������С����ģʽ,���ش�ֱ����������Сֱ��1920*1������������
//@EXECUTIONFLOW: 5. ����CSUDIPLAYERClose�رղ��Բ�����
//@EXECUTIONFLOW: 6.  ����CSUDIVIDEOSetWindowSize���ø߱��嵽ȫ��,�ָ������ֳ�
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
	/*���һ�����Ϊ0,������Ϊ1*/
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
		/*���һ�����Ϊ0,������Ϊ1*/
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
	/*���һ�����Ϊ0,������Ϊ1*/
	pstRectSD[nSDCnt-1].m_nHeight = 1;

	if (bIsSupportHD())
	{
		nWeightStep = 1080/nHDCnt;
		for(i=0;i<nHDCnt;i++)
		{
			pstRectHD[i].m_nWidth = 1920;
			pstRectHD[i].m_nHeight = 1080 - i*nWeightStep;
		}
		/*���һ�����Ϊ0,������Ϊ1*/
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
//@DESCRIPTION: ���Ը߱�����Էֿ����ô��ڴ�С,�����໥��Ӱ��
//@PRECONDITION:ƽ̨֧�ָ������͸������
//@PRECONDITION:�߱��岻ͬԴ
//@INPUT:1. nVideoIndex =0
//@INPUT:2. SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts
//@EXPECTATION: ����ͱ���ֱ���д�������ʱ���໥Ӱ��
//@EXECUTIONFLOW: 1. ���ƽ̨��֧�ָ�����ֱ�ӷ���TRUE,��Ϊ���Գɹ����жϷ�����ƽ̨֧��mpeg4������h.264�Ľ���
//@EXECUTIONFLOW: 2. ���ƽ̨�߱���ͬԴ��ֱ�ӷ���TRUE,���Գɹ�
//@EXECUTIONFLOW: 3. ���Ų�����SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts�еĸ����Ŀ,�����ɹ�����ʾ����
//@EXECUTIONFLOW: 4. ���ø���ֱ�ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,����ΪEM_UDISCREEN_RESOLUTION_PAL,�������سɹ�
//@EXECUTIONFLOW: 5. ����CSUDIVIDEOSetWindowSize���ñ�����Ƶ���ڵ�{0,0,720/2,576/2},����������������Ͻ�1/2��,��������ޱ仯
//@EXECUTIONFLOW: 6. ����CSUDIVIDEOSetWindowSize���ñ�����Ƶ���ڵ�{0,0,1920/2,1080/2},����������������Ͻ�1/2��,��������ޱ仯
//@EXECUTIONFLOW: 7. ����CSUDIVIDEOSetWindowSize���ø߱��嵽ȫ��,�ָ������ֳ�
CSUDI_BOOL CSTC_VIDEO_IT_SetWindowSize_0025(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_BOOL bResult = CSUDI_FALSE;
	CSUDISCREENType_E enDev[]={EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD};
	CSUDISCREENResolution_E enRes[]={EM_UDISCREEN_RESOLUTION_PAL,EM_UDISCREEN_RESOLUTION_1080I_50HZ};
	CSUDIWinRect_S stWinRectSD = {0,0,720/2,576/2};
	CSUDIWinRect_S stWinRectHD = {0,0,1920/2,1080/2};
	
	/*���ƽ̨��֧�ָ��������ֱ�ӷ��ز��Գɹ�*/
	if (!bIsSupportHD())
	{
		return TRUE;
	}

	if(CSUDI_FALSE == CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "��Ƶʧ��!\n");

		CSUDISCREENSetResolution(enDev, enRes, 2);

		CSTK_ASSERT_TRUE_FATAL(TCSetWindowSize(EM_UDI_VOUT_DEVICE_SD,&stWinRectSD),"���ñ��嵽С����ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(TCSetWindowSize(EM_UDI_VOUT_DEVICE_HD,&stWinRectHD),"���ø��嵽С����ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(TCSetWindowSize(EM_UDI_VOUT_DEVICE_SD,NULL),"���ñ��嵽С����ʧ��\n");
		CSTK_ASSERT_TRUE_FATAL(TCSetWindowSize(EM_UDI_VOUT_DEVICE_HD,NULL),"���ø��嵽С����ʧ��\n");

		bResult = CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("�߱���ͬԴ!\n");
	}
	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
	
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWindowSize�ڽ�������ǰ�����Ƿ�ɹ�
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT:1��nVideoIndex =  [0..nVideo-1]
//@INPUT:2��stRect = {0,0,720/2,576/2} (����:{0,0,1920/2,1080/2})
//@EXPECTATION: ����CSUDI_SUCCESS,��Ƶͼ����ʾΪ:"λ����Ļ���Ͻǣ�1/4��Ļ��С����Сͼ��"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����ȡ֧�ֲ��Ž�Ŀ���͵�nVideoIndex
//@EXECUTIONFLOW: 4������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,pstRect)����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 5����nVideoIndex�豸��ѭ������һ������Ƶ����
//@EXECUTIONFLOW: 6������CSTKWaitYesKey��������Աȷ����Ƶ�ɹ���ʾ����
//@EXECUTIONFLOW: 7������CSTKWaitYesKey��������Աȷ����ʾ����ƵΪ"λ����Ļ���Ͻǣ�1/4��Ļ��С����Сͼ��"
//@EXECUTIONFLOW: 8������CSUDIVIDEOSetWindowSize(nVideoIndex,eScreenDevice,CSUDI_NULL)�ָ�ȫ����ʾ����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 9�����֧�ָ���������ظ�����3-8
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!");

	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������");

	if(!CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

		nVideoIndex = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_MPEG2);

		nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, &stWinRectSD);

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "����4:(SD)CSUDIVIDEOSetWindowSize����ʧ��");

		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "����5:(SD)��Ƶ����ʧ��!");

		if(CSUDI_SUCCESS == nResult)
		{
			//���CSUDIVIDEOGetWindowSize
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "(SD)CSUDIVIDEOGetWindowSize���û�ȡ1/4��Ļ��Сʧ��!");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stWinRectSD, &stGetRect), "(SD)��õ�Rect�����õ�1/4��Ļ��С��һ��");
		}

		CSTCPrint("�뽫��������˿����ӱ�����ӣ���ȷ����Ƶ����Ϊ:λ����Ļ���Ͻǣ�1/4��Ļ��С����Сͼ��!\n");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����7:(SD)������Ԥ�費��!");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "(SD)SetWindowSize����ȫ��ʧ��!");

		//���CSUDIVIDEOGetWindowSize
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "(SD)CSUDIVIDEOGetWindowSize���û�ȡȫ����Сʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "(SD)��õ�Rect�����õ�ȫ����С��һ��");

		if(CSUDI_NULL != hPlayer)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
		}
		hPlayer = CSUDI_NULL;
	}
	
	if(bIsSupportHD())
	{
		CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;

		nVideoIndex = CSUDI_VIDEO_FindVideoDecoder(EM_UDI_VID_STREAM_H264);

		nResult = CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, &stWinRectHD);

		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == nResult), "����4:(HD)CSUDIVIDEOSetWindowSize����ʧ��");

		CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &enResBak);
		if(enResBak != enRes)
		{
			CSUDISCREENSetResolution(&enDev, &enRes,1);
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "����5:(HD)��Ƶ����ʧ��!");
		

		if(CSUDI_SUCCESS == nResult)
		{
			//���CSUDIVIDEOGetWindowSize
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "(HD)CSUDIVIDEOGetWindowSize���û�ȡ1/4��Ļ��Сʧ��!");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stWinRectHD, &stGetRect), "(HD)��õ�Rect�����õ�1/4��Ļ��С��һ��");
		}

		CSTCPrint("�뽫��������˿����Ӹ�����ӣ���ȷ����Ƶ����Ϊ:λ����Ļ���Ͻǣ�1/4��Ļ��С����Сͼ��!\n");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����7:(HD)������Ԥ�費��!");

		if (enResBak != enRes)
		{
			CSUDISCREENSetResolution(&enDev, &enResBak,1);
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "(HD)SetWindowSize����ȫ��ʧ��!");

		//���CSUDIVIDEOGetWindowSize
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect), "(HD)CSUDIVIDEOGetWindowSize���û�ȡȫ����Сʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "(HD)��õ�Rect�����õ�ȫ����С��һ��");

	}

	bResult = CSUDI_TRUE;

	CSTK_FATAL_POINT;

	if(!CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "(SD)SetWindowSize�ָ�ȫ������ʧ��!");
	}
	
	if(bIsSupportHD())
	{
		eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "(HD)SetWindowSize�ָ�ȫ������ʧ��!");
	}
	
	if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWindowSize
//@DESCRIPTION: ���Ÿ���16:9�Ľ�Ŀʱ,���Ի��������ֻ��ȫ��ʱ��Ч��,С����ʱҪ��ȫ��
//@INPUT:1. nVideoIndex =0
//@INPUT:2. SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts
//@EXPECTATION: letter_boxȫ����Ч��С������Ϊȫ��
//@EXECUTIONFLOW: 1. ���ƽ̨��֧�ָ��������ֱ�ӷ���,�жϷ����ǲ��������ļ���
//@EXECUTIONFLOW: 2. ���Ų�����SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts,�����ɹ�����ʾ����
//@EXECUTIONFLOW: 4. ����CSUDIVIDEOSetMatchMethod����Ϊletter_box��Ч��,�������������Ļ���±�ѹ��,�����кڱ�
//@EXECUTIONFLOW: 5. ������ƵС����,����С�������ܵ�OSG�ú�ɫ���,����������Ļ�ϳ���ȫ����ɫOSG�����Լ���ƵС��������ƵС������Ϊȫ����ʾ
//@EXECUTIONFLOW: 6. ����CSUDIVIDEOSetWindowSize���ø߱��嵽ȫ��,�ָ������ֳ�
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
	
	/*���ƽ̨��֧�ָ��������ֱ�ӷ��ز��Գɹ�*/
	if (!bIsSupportHD())
	{
		CSTCPrint("��ƽ̨��֧�ָ������\n");
		return CSUDI_FALSE;
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "��Ƶʧ��!\n");

	/*�߱���ͬԴ��ֻ�ڸ����ϲ���*/
	if(CSUDI_TRUE == CSTC_VIDEO_IsShareHDDisplaySurface())
	{
		CSTC_VIDEO_iWinRect_SDTransformHD(&stWinRect);
		CSTC_VIDEO_iOSGRect_SDTransformHD(&stRectOSD);
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
			"fail to set HD to letter box\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_4_3), 
			"fail to set HD to 4:3");

		CSTCPrint("letter box ģʽ��16:9�Ľ�ĿԴ��4:3�ı��������\n");
		CSTCPrint("���������Ļ���±�ѹ��,�����кڱ�(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ�ϱ������letter box ȫ��Ч��ʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisSurface),
			"fail to get hd surface by CSUDIOSGGetDisplaySurface\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurface, CSUDI_NULL, 0xFFFF0000),
			"fail to fill hd display surface\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurface, &stRectOSD, 0x0),
			"fail to fill hd display surface\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIVIDEOSetWindowSize(0, EM_UDI_VOUT_DEVICE_HD,&stWinRect),
			"fail to set hd window by CSUDIVIDEOSetWindowSize\n");
		CSTCPrint("��ȷ�ϱ�����Ļ�ϳ���ȫ����ɫOSG�����Լ���ƵС����(��Ƶ��Ӧ��С),��ƵС�������ܸպý�����ɫOSG,�����޺ڱ�(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ�ϻ������С����ģʽ��������ʧ��\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
			"fail to set SD to letter box\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_4_3), 
			"fail to set SD to 4:3");

		CSTCPrint("letter box ģʽ��16:9�Ľ�ĿԴ��4:3�ı��������\n");
		CSTCPrint("���������Ļ���±�ѹ��,�����кڱ�(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ�ϱ������letter box ȫ��Ч��ʧ��\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDisSurface),
			"fail to get sd surface by CSUDIOSGGetDisplaySurface\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurface, CSUDI_NULL, 0xFFFF0000),
			"fail to fill sd display surface\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurface, &stRectOSD, 0x0),
			"fail to fill sd display surface\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIVIDEOSetWindowSize(0, EM_UDI_VOUT_DEVICE_SD, &stWinRect),
			"fail to set sd window by CSUDIVIDEOSetWindowSize\n");
		CSTCPrint("��ȷ�ϱ�����Ļ�ϳ���ȫ����ɫOSG�����Լ���ƵС����(��Ƶ��Ӧ��С),��ƵС�������ܸպý�����ɫOSG,�����޺ڱ�(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ�ϻ������С����ģʽ��������ʧ��\n");
	}

	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
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
//@DESCRIPTION: ���ű���4:3�Ľ�Ŀʱ,��������letter_boxֻ��ȫ��ʱ��Ч��,С����ʱҪ��ȫ��
//@INPUT:1. nVideoIndex =0
//@INPUT:2. Player_PAL_NTSC
//@EXPECTATION: letter_boxȫ����Ч��С������Ϊȫ��
//@EXECUTIONFLOW: 1. ���ƽ̨��֧�ָ����������������ֱ�ӷ���,�жϷ����ǲ��������ļ���
//@EXECUTIONFLOW: SCREEN�ֶ��е�EM_UDI_VOUT_DEVICE_HD=1
//@EXECUTIONFLOW: 2. ���Ų�����Audio&Video Test_27Mbps_20070524.ts�еĻ�����,�����ɹ�����ʾ����
//@EXECUTIONFLOW: 3. ���ø���ֱ���ΪEM_UDISCREEN_RESOLUTION_1080I_50HZ,�������سɹ�
//@EXECUTIONFLOW: 4. ����CSUDIVIDEOSetMatchMethod����Ϊletter_box��Ч��,�������������Ļ��Ƶ������,�����кڱ�
//@EXECUTIONFLOW: 5. ������ƵС����,����С�������ܵ�OSG�ú�ɫ���,����������Ļ�ϳ���ȫ����ɫOSG�����Լ���ƵС��������ƵС������Ϊȫ����ʾ
//@EXECUTIONFLOW: 6. ����CSUDIVIDEOSetWindowSize���ø߱��嵽ȫ��,�ָ������ֳ�
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

	/*���ƽ̨��֧�ָ��������ֱ�ӷ��ز��Գɹ�*/
	if(CSUDI_FALSE == CSTC_VIDEO_IsSupportHdDispaly())
	{
		CSTCPrint("��ƽ̨��֧�ָ������\n");
		return CSUDI_FALSE;
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");

	CSTC_VIDEO_iWinRect_SDTransformHD(&stWinRectHD);
	CSTC_VIDEO_iOSGRect_SDTransformHD(&stRectOSD);

	CSUDISCREENSetResolution(&enDev,&enRes, 1);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetMatchMethod(0, EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX),
		"fail to set HD to letter box\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetAspectRatio(0, EM_UDI_VOUT_DEVICE_HD, EM_UDIVIDEO_ASPECT_RATIO_16_9), 
		"fail to set HD to 16:9");
	
	CSTCPrint("letter box ģʽ��4:3�Ľ�ĿԴ��16:9�ĸ��������\n");
	CSTCPrint("���������Ļ��Ƶ������,�����кڱ�(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ�ϱ������letter box ȫ��Ч��ʧ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisSurfaceHD),
		"fail to get hd surface by CSUDIOSGGetDisplaySurface\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurfaceHD,NULL,0xFFFF0000),
		"fail to fill hd display surface\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hDisSurfaceHD,&stRectOSD,0x0),
		"fail to fill hd display surface\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS ==  CSUDIVIDEOSetWindowSize(0, EM_UDI_VOUT_DEVICE_HD,&stWinRectHD),
		"fail to set sd window by CSUDIVIDEOSetWindowSize\n");
	CSTCPrint("��ȷ�ϸ�����Ļ�ϳ���ȫ����ɫOSG�����Լ���ƵС����(��Ƶ��Ӧ��С),��ƵС�������ܸպý�����ɫOSG,�����޺ڱ�(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ�ϻ������С����ģʽ��������ʧ��\n");

	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
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
//@DESCRIPTION: ����CSUDIVIDEOGetWindowSize �ӿڶԴ�������ļ������
//@INPUT:1��nVideoIndex =  �Ƿ�
//@INPUT:2��eScreenDevice = �Ƿ�
//@INPUT:3.	  pstRect = �Ƿ�	
//@EXPECTATION:�Ըýӿڴ��ݴ����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount ���ƽ̨video ������
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetWindowSize ���ݴ��������ϣ������CSUDIVIDEO_ERROR_BAD_PARAMETER
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

	// ��� video ������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "���video ����ʧ��");
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

	//���������
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetWindowSize(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eScreenDevice, ErrorParamList[i].m_PstRect);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "���������ʧ��");
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
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!\n");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

	// 3
	if (EM_UDI_VOUT_DEVICE_SD== eScreenDevice)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "��Ƶʧ��!\n");
	}

	// 4
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "����Ƶ��ʧ��\n");

	// 5-6
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, s_aWinClipWindowSizeTestCase[nTestID].m_sWinRect), "SetWindowSizeʧ��!\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, s_aWinClipWindowSizeTestCase[nTestID].m_sWinClip), "SetWinClipʧ��!\n");

	// 7
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSizeʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWinClipʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��!");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}
		
	return bResult;
}


//Ԥ��δʹ�ò���nVideoIndex,�Ա��Ժ���չ
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
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount), "��ȡVideo����ʧ��!\n");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");

	// 3
	if (EM_UDI_VOUT_DEVICE_SD== eScreenDevice)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockHDFrequency( &hPlayer)), "��Ƶʧ��!\n");
	}
    
	// 4
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE), "����Ƶ��ʧ��\n");

	// 5-7
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, s_aWinClipWindowSizeTestCase[nTestID].m_sWinRect), "SetWindowSizeʧ��!\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, s_aWinClipWindowSizeTestCase[nTestID].m_sWinClip), "SetWinClipʧ��!\n");

	//���CSUDIVIDEOGetWinClip
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect), "CSUDIVIDEOGetWinClip����ʧ��!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == testCheckGetWinClip(
		nVideoIndex, 
		eScreenDevice, 
		s_aWinClipWindowSizeTestCase[nTestID].m_sWinClip, 
		&stGetRect)), "��õ�Rect�����õĲ�һ��\n");
	
	// 8
	if (EM_UDI_VOUT_DEVICE_HD == eScreenDevice)
	{
       	CSTCPrint("�뽫��������˿����Ӹ�����ӣ���ȷ����Ƶ����Ϊ:\"%s\"!\n",s_aWinClipWindowSizeTestCase[nTestID].m_pcDes);
	}
	else
	{
       	CSTCPrint("�뽫��������˿����ӱ�����ӣ���ȷ����Ƶ����Ϊ:\"%s\"!\n",s_aWinClipWindowSizeTestCase[nTestID].m_pcDes);
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "������Ԥ�費��\n");

	// 9
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWindowSizeʧ��!\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL), "SetWinClipʧ��!\n");

	//���CSUDIVIDEOGetWinClip
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect), "CSUDIVIDEOGetWinClip����ʧ��!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == testCheckGetWinClip(
		nVideoIndex, 
		eScreenDevice, 
		CSUDI_NULL, 
		&stGetRect)), "��õ�Rect�����õĲ�һ��\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��!");
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}
		
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip���ڲ���ΪCSUDI_NULL��CSUDIVIDEOSetWindowSize���ڲ���Ϊ{0,0,720/2, 576/2}����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT: WindowSize �Ĵ���ָ��ָ��{0,0,720/2, 576/2}
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:ȫ��ͼ��ѹ��Ϊ1/4��Ļ��С��λ����Ļ���Ͻ���ʾ
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾЧ��Ϊ:"ȫ��ͼ��ѹ��Ϊ1/4��Ļ��С��λ����Ļ���Ͻ���ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0001(void)
{
	int nTestID = 0;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip���ڲ���Ϊ{0,0,720/2, 576/2}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{0,0,720/2, 576/2}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾЧ��Ϊ:"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0002(void)
{
	int nTestID = 1;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip���ڲ���Ϊ{720/2, 576/2,720/2, 576/2}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{720/2, 576/2,720/2, 576/2}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice = EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾЧ��Ϊ:"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0003(void)
{
	int nTestID = 2;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip���ڲ���Ϊ{0, 576/2,720/2, 576/2}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{0, 576/2,720/2, 576/2}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾЧ��Ϊ:"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0004(void)
{
	int nTestID = 3;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}


//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip���ڲ���Ϊ{720/2, 0,720/2, 576/2}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ�ִ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{720/2, 0,720/2, 576/2}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾЧ��Ϊ:"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0005(void)
{
	int nTestID = 4;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip���ڲ���Ϊ{720/3, 576/3,720/3, 576/3}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{720/3, 576/3,720/3, 576/3}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:"��Ļ������1/9��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾЧ��Ϊ:"��Ļ������1/9��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0006(void)
{
	int nTestID = 5;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip���ڲ���Ϊ{720/3, 576/3,720/3, 576/3}��CSUDIVIDEOSetWindowSize���ڲ���Ϊ{2*720/3, 2*576/3,720/3, 576/3}����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{720/3, 576/3,720/3, 576/3}
//@INPUT: WindowSize �Ĵ���ָ��ָ��{2*720/3, 2*576/3,720/3, 576/3}
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:"��Ļ������1/9��Ļ��С��ͼ�񱻼��к�ԭ����ʾ����Ļ���½�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ����Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ����Ƶ��ʾЧ��Ϊ:"��Ļ������1/9��Ļ��С��ͼ�񱻼��к�ԭ����ʾ����Ļ���½�"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0007(void)
{
	int nTestID = 6;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip���ڲ���Ϊ{720/2, 576/2, 720/2, -1  }����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex [0...nVideo-1]
//@INPUT: WinClip �Ĵ���ָ��ָ��{720/2, 576/2, 720/2, -1  },�Ƿ���
//@INPUT:eScreenDevice = EM_UDI_VOUT_DEVICE_SD
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:���д��ڵĿ�߲���С�ڻ����0
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0008(void)
{
	int nTestID = 7;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_SD;

	return testSetWinClipError(nTestID, eScreenDevice);
}


//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWinClip���ڲ���ΪCSUDI_NULL��CSUDIVIDEOSetWindowSize���ڲ���Ϊ{0,0,1920/2, 1080/2}����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT: WindowSize �Ĵ���ָ��ָ��{0,0,1920/2, 1080/2}
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:ȫ��ͼ��ѹ��Ϊ1/4��Ļ��С��λ����Ļ���Ͻ���ʾ
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ��ʾЧ��Ϊ:"ȫ��ͼ��ѹ��Ϊ1/4��Ļ��С��λ����Ļ���Ͻ���ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0009(void)
{
	int nTestID = 8;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWinClip���ڲ���Ϊ{0,0,1920/2, 1080/2}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{0,0,1920/2, 1080/2}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ��ʾЧ��Ϊ:"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0010(void)
{
	int nTestID = 9;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWinClip���ڲ���Ϊ{1920/2, 1080/2,1920/2, 1080/2}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{1920/2, 1080/2,1920/2, 1080/2}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ��ʾЧ��Ϊ:"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0011(void)
{
	int nTestID = 10;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWinClip���ڲ���Ϊ{0, 1080/2,1920/2, 1080/2}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{0, 1080/2,1920/2, 1080/2}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ��ʾЧ��Ϊ:"���½�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0012(void)
{
	int nTestID = 11;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}


//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWinClip���ڲ���Ϊ{1920/2, 0,1920/2, 1080/2}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ�ִ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{1920/2, 0,1920/2, 1080/2}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice = EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ��ʾЧ��Ϊ:"���Ͻ�1/4��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0013(void)
{
	int nTestID = 12;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWinClip���ڲ���Ϊ{1920/3, 1080/3,1920/3, 1080/3}��CSUDIVIDEOSetWindowSize���ڲ���ΪCSUDI_NULL����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{1920/3, 1080/3,1920/3, 1080/3}
//@INPUT: WindowSize �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:"��Ļ������1/9��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ��ʾЧ��Ϊ:"��Ļ������1/9��Ļ��С��ͼ�񱻷Ŵ�ȫ����ʾ"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0014(void)
{
	int nTestID = 13;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@CASEGROUP:CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWinClip���ڲ���Ϊ{1920/3, 1080/3,1920/3, 1080/3}��CSUDIVIDEOSetWindowSize���ڲ���Ϊ{2*1920/3, 2*1080/3,1920/3, 1080/3}����µ���ʾЧ��
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ϸ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ָ��{1920/3, 1080/3,1920/3, 1080/3}
//@INPUT: WindowSize �Ĵ���ָ��ָ��{2*1920/3, 2*1080/3,1920/3, 1080/3}
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: ����CSUDI_SUCCESS����Ļ��ʾЧ��Ϊ:"��Ļ������1/9��Ļ��С��ͼ�񱻼��к�ԭ����ʾ����Ļ���½�"
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ�ѳɹ���ʾ����
//@EXECUTIONFLOW: 6������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWindowSize����鷵��ֵ�Ƿ�ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSTKWaitYesKey��������Աȷ�ϸ���˿���Ƶ��ʾЧ��Ϊ:"��Ļ������1/9��Ļ��С��ͼ�񱻼��к�ԭ����ʾ����Ļ���½�"
//@EXECUTIONFLOW: 9���ָ��ֳ�
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0015(void)
{
	int nTestID = 14;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClip(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@DESCRIPTION: ���Ը�������˿ڣ�CSUDIVIDEOSetWinClip���ڲ���Ϊ{1920/2, 1080/2, 1920/2, -1  }����µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex [0...nVideo-1]
//@INPUT: WinClip �Ĵ���ָ��ָ��{1920/2, 1080/2, 1920/2, -1  },�Ƿ���
//@INPUT:eScreenDevice =  EM_UDI_VOUT_DEVICE_HD
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]�ڵ�ֵ
//@EXECUTIONFLOW: 3����nVideoIndex�������ϲ�����Ƶ������
//@EXECUTIONFLOW: 4������CSUDIVIDEOShow(nVideoIndex,CSUDI_TRUE)������Ƶ��ʾ
//@EXECUTIONFLOW: 5������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:���д��ڵĿ�߲���С�ڻ����0
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0016(void)
{
	int nTestID = 15;
	
	CSUDISCREENType_E  eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	
	return testSetWinClipError(nTestID, eScreenDevice);
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip����Ƶ����������������������µ�ִ�����
//@PRECONDITION:[-1.nVideo,nVideo+1]
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex Ϊ�Ƿ�����Ƶ����������
//@INPUT: WinClip �Ĵ���ָ��ΪCSUDI_NULL
//@INPUT:eScreenDevice =  [EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD]
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]���ֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0),"û�п��õ���Ƶ������\n");
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
						s_SetErrorWinClipTpye[nVideoErrorIndex].m_eScreenDevice, CSUDI_NULL)), "�����жϴ���!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
		
	return bResult;
}

//@CASEGROUP:CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip��eScreenDevice ������������µ�ִ�����
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex [0...nVideoIndex -1]
//@INPUT: WinClip �Ĵ���ָ��Ϊ[�Ϸ�]
//@INPUT:eScreenDevice =  [Error]
//@EXPECTATION: ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount�õ�Video����ĿnVideo
//@EXECUTIONFLOW: 2��nVideoIndexȡ[0..nVideo]���ֵ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetWinClip����鷵��ֵ�Ƿ�Ϊ��CSUDI_SUCCESS
//@REMARK:
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0018(void)
{
	int nVideoCount = 0;
	
	int nVideoIndex = 0;
	
	CSUDISCREENType_E eScreenDevice = 0;
	
	CSUDI_BOOL bResult = CSUDI_FALSE;
		
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "��ȡVideo Decoder�豸����������ʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL((nVideoCount > 0), "û�п��õ���Ƶ������\n");
	
	for (nVideoIndex = 0; nVideoIndex < nVideoCount; nVideoIndex ++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDIVIDEO_ERROR_BAD_PARAMETER == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL)), "�����жϴ���!");
	}
	
	bResult = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bResult;	
}

//@CASEGROUP:CSUDIVIDEOSetWinClip CSUDISCREENSetResolution
//@DESCRIPTION: ����CSUDIVIDEOSetWinClip�ü���������Դ�Ĵ���,������ֱ��������޹�
//@PRECONDITION:Video�豸����Ŀ��Ϊ0
//@PRECONDITION:Video�豸����ȷ������Ƶ������
//@INPUT: nVideoIndex = 0
//@INPUT: WinClip �Ĵ���ָ��Ϊ{0,0,300,200}
//@EXPECTATION: ���òü����ں�,��������ֱ��������,��Ļ�ϵĻ������ݲ���
//@EXECUTIONFLOW: 1������һ�����Խ�Ŀ,�������ųɹ�
//@EXECUTIONFLOW: 2������CSUDISCREENSetResolution������������ó�1080I_50HZ
//@EXECUTIONFLOW: 3������CSUDIVIDEOSetWinClip���߱�������ü�����ͬʱ���ó�{0,0,300,200},ȷ�����Ч��
//@EXECUTIONFLOW: 4������CSUDISCREENSetResolution������������ó�NTSC,����������ó�720P,�����ü����ڲ���
//@EXECUTIONFLOW: 5������CSUDISCREENSetResolution������������ó�PAL,����������ó�1080I,�����ü����ڲ���
//@EXECUTIONFLOW: 6������CSUDIPLAYERClose�رղ�����
//@EXECUTIONFLOW: 7������CSUDIVIDEOSetWinClip���߱��������ü���������Ϊȫ��,�ָ����Ի���
CSUDI_BOOL CSTC_VIDEO_IT_SetWinClip_0019(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E enDev;
	CSUDISCREENResolution_E enRes;
	BOOL bHDSupport = bIsSupportHD();
	CSUDIWinRect_S stRect = {0,0,300,200};

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");

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

	CSTCPrint("(%d,%d,%d,%d)λ�õĻ��汻�ü�ȫ�����(Y/N)\n",
		stRect.m_nX,stRect.m_nY,stRect.m_nWidth,stRect.m_nHeight);
	
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

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

	CSTCPrint("�������NTSC,�������720P(����еĻ�)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

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

	CSTCPrint("�������PAL,�������1080I(����еĻ�)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(0,
		EM_UDI_VOUT_DEVICE_SD|EM_UDI_VOUT_DEVICE_HD,NULL),
		"fail to set window clip\n");

	return TRUE;
}

//@CASEGROUP:CSUDIVIDEOSetVBIEnable
//@DESCRIPTION:����ֱ��CC TS�������������ŵ����
//@PRECONDITION:ƽ̨֧��VBI��ʽ��CloseCaption
//@INPUT:
//@EXPECTATION:  �ܲ��ų�ָ��TS������ʾCC����
//@EXECUTIONFLOW:1������ָ����TS����
//@EXECUTIONFLOW:2����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:3������CSUDIVIDEOSetVBIEnable ��Enable��ΪTRUE  ������ʾCC
//@EXECUTIONFLOW:4����ʾ�û�ȷ�ϣ���ʾCC����
//@EXECUTIONFLOW:5������CSUDIVIDEOSetVBIEnable ��Enable��ΪFALSE  ��������ʾCC
//@EXECUTIONFLOW:6����ʾ�û�ȷ�ϣ���ʾCC����
//@EXECUTIONFLOW:7������CSUDIVIDEOSetVBIEnable ��Enable��ΪTRUE��������ʾCC
//@EXECUTIONFLOW:8���ظ�����3-����7��3��
//@EXECUTIONFLOW:9��ֹͣ����
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

   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == PlayLocalProgram(&hPlayer, nStreamCnt, sStreamInfo, g_TSStreamInfo[0].m_pcName), "����inject�ӿڲ��Ž�Ŀʧ��");

	CSTCPrint("����Ƶ���ų�������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"����2ʧ��");
	
	for (i=0;i<3;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex, eVideoVBIType, bEnable)), "����3ʧ��");
		
		CSTCPrint("CC��������\n");
		CSTCPrint("Do we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ��");
		bEnable=FALSE;
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex,eVideoVBIType,bEnable)), "����5ʧ��");
		CSTCPrint("CC�ر��˲�������\n");
		CSTCPrint("Don't we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����6ʧ��");
		bEnable=TRUE;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex,eVideoVBIType,bEnable), "����7ʧ��");
	    	CSTCPrint("CC��������\n");
		CSTCPrint("Do we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����8ʧ��");	
	}
	
	CSTK_FATAL_POINT;

    if(CSUDI_NULL != hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��");   
        
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��");

        hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOSetVBIEnable
//@DESCRIPTION: ����CSUDIVIDEOSetVBIEnable �ӿڶԴ�������ļ������
//@INPUT:1��nVideoIndex  =  �Ƿ�
//@INPUT:2��eVideoVBIType  = �Ƿ�
//@INPUT:3.	  pbEnable = �Ƿ�	
//@EXPECTATION:�Ըýӿڴ��ݴ����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount ���ƽ̨video ������
//@EXECUTIONFLOW: 2������CSUDIVIDEOSetVBIEnable ���ݴ��������ϣ������CSUDIVIDEO_ERROR_BAD_PARAMETER
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
	
	// ��� video ������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "���video ����ʧ��");

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
	
	//���������
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOSetVBIEnable(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eVideoVBIType, ErrorParamList[i].m_pbEnable);
		if(nResult==CSUDI_SUCCESS)
		{
        return CSUDI_TRUE;
		}
		else
		{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "���������ʧ��");
		}
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIVIDEOGetWinClip
//@DESCRIPTION: ����CSUDIVIDEOGetWinClip �ӿڶԴ�������ļ������
//@INPUT:1��nVideoIndex =  �Ƿ�
//@INPUT:2��eScreenDevice = �Ƿ�
//@INPUT:3.	  pstRect = �Ƿ�	
//@EXPECTATION:�Ըýӿڴ��ݴ����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount ���ƽ̨video ������
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetWinClip ���ݴ��������ϣ������CSUDIVIDEO_ERROR_BAD_PARAMETER
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

	// ��� video ������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "���video ����ʧ��");
	
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

	//���������
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetWinClip(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eScreenDevice, ErrorParamList[i].m_PstRect);
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "���������ʧ��");
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
//@DESCRIPTION: ����CSUDIVIDEOGetVBIEnable �ӿڶԴ�������ļ������
//@INPUT:1��nVideoIndex  =  �Ƿ�
//@INPUT:2��eVideoVBIType  = �Ƿ�
//@INPUT:3.	  pbEnable = �Ƿ�	
//@EXPECTATION:�Ըýӿڴ��ݴ����������Ӧ�÷��سɹ�
//@EXECUTIONFLOW: 1������CSUDIVIDEOGetCount ���ƽ̨video ������
//@EXECUTIONFLOW: 2������CSUDIVIDEOGetVBIEnable ���ݴ��������ϣ������CSUDIVIDEO_ERROR_BAD_PARAMETER
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
	
	// ��� video ������
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetCount(&nVideoCount)), "���video ����ʧ��");

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
	
	//���������
	for(i = 0; i < sizeof(ErrorParamList)/sizeof(ErrorParamList[0]); i++)
	{
		nResult = CSUDIVIDEOGetVBIEnable(ErrorParamList[i].m_nVideoIndex, ErrorParamList[i].m_eVideoVBIType, ErrorParamList[i].m_pbEnable);
		if(nResult==CSUDI_SUCCESS)
		{
        return CSUDI_TRUE;
		}
		else
		{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS != nResult), "���������ʧ��");
		}
	}
	
	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}
//@CASEGROUP:PLAYER & show CC
//@DESCRIPTION:����ֱ��CC TS�������������ŵ����
//@PRECONDITION:ƽ̨֧��VBI��ʽ��CloseCaption
//@INPUT:
//@EXPECTATION:  �ܲ��ų�ָ��TS������ʾCC����
//@EXECUTIONFLOW:1�����Ÿ���ָ����TS����
//@EXECUTIONFLOW:2����ʾ�û�ȷ�ϣ�����Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:3������CSUDIVIDEOSetVBIEnable ��Enable��ΪTRUE  ������ʾCC
//@EXECUTIONFLOW:4����ʾ�û�ȷ�ϣ���ʾCC����
//@EXECUTIONFLOW:5������CSUDIVIDEOGetVBIEnable��ȡVBI��ʽ��CloseCaption���ܵĿ���״̬�������ɹ�
//@EXECUTIONFLOW:6���жϻ�ȡ���Ŀ���״̬�����沽�����õ�һ��
//@EXECUTIONFLOW:7������CSUDIVIDEOSetVBIEnable ��Enable��ΪFALSE  ������ʾCC
//@EXECUTIONFLOW:8����ʾ�û�ȷ�ϣ���ʾCC����
//@EXECUTIONFLOW:9������CSUDIVIDEOGetVBIEnable��ȡVBI��ʽ��CloseCaption���ܵĿ���״̬�������ɹ�
//@EXECUTIONFLOW:10���жϻ�ȡ���Ŀ���״̬�����沽�����õ�һ��
//@EXECUTIONFLOW:11������CSUDIVIDEOSetVBIEnable ������������Ļ����ʾ������CC������ʾ
//@EXECUTIONFLOW:12���ظ�����3-����11��3��
//@EXECUTIONFLOW:13��ֹͣ����
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

    CSTK_ASSERT_TRUE_FATAL( CSUDI_SUCCESS  ==  PlayLocalProgram(&hPlayer, nStreamCnt, sStreamInfo, g_TSStreamInfo[0].m_pcName), "����inject�ӿڲ��Ž�Ŀʧ��");
	
	CSTCPrint("����Ƶ���ų�������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()),"����2ʧ��");
	
	for (i=0;i<3;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex, eVideoVBIType, bEnable)), "����3ʧ��");
		
		CSTCPrint("CC��������\n");
		CSTCPrint("Do we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOGetVBIEnable(nVideoIndex,eVideoVBIType,&tEnable), "����5ʧ��");
		CSTK_ASSERT_TRUE_FATAL(tEnable == bEnable, "����6ʧ��");

		if(tEnable)
		{
			bEnable = FALSE;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex,eVideoVBIType,bEnable), "����7ʧ��");	
        	}
		CSTCPrint("CC�����Բ�������\n");
		CSTCPrint("Do we get CC output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOGetVBIEnable(nVideoIndex,eVideoVBIType,&tEnable), "����9ʧ��");
		CSTK_ASSERT_TRUE_FATAL(tEnable == bEnable, "����10ʧ��");

		if(!tEnable)
		{
			tEnable=TRUE;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIVIDEOSetVBIEnable(nVideoIndex,eVideoVBIType,tEnable), "����11ʧ��");	
        	}
	}

	CSTK_FATAL_POINT;

    if(CSUDI_NULL != hPlayer)
      {
          CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ������ʧ��");   
          
          CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��");
    
          hPlayer = CSUDI_NULL;
      }

	return CSUDI_TRUE;
}
//@CASEGROUP:CSUDIVIDEOGetWinClip,CSUDIVIDEOGetWindowSize
//@DESCRIPTION: ����CSUDIVIDEOGetWinClip,CSUDIVIDEOGetWindowSize �ӿ�����������ǰ��ĵ������
//@INPUT:1��nVideoIndex =  0
//@INPUT:2��eScreenDevice = EM_UDI_VOUT_DEVICE_SD;EM_UDI_VOUT_DEVICE_HD
//@INPUT:3.	  pstRect = �Ϸ�ֵ
//@EXPECTATION:�Ըýӿ�ʹ�ó��������߼����
//@EXECUTIONFLOW: 1������ǰ����CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip,ϣ���ɹ�,�ҷ���ȫ����Ϣ
//@EXECUTIONFLOW: 2������ǰ����CSUDIVIDEOSetWindowSize,CSUDIVIDEOSetWinClip,Ȼ��CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip, �������óɹ�,�ҷ�������ֵ
//@EXECUTIONFLOW: 3������ʱ����CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip,����Step2δ���ò���û�б�SetӰ��
//@EXECUTIONFLOW: 4��ֹͣ���ź�CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip,����Step2��������Ȼ��Ч
//@EXECUTIONFLOW: 5���رղ��ž����SUDIVIDEOSetWindowSize,CSUDIVIDEOSetWinClip,Ȼ��CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip,����Step2��������Ȼ��Ч,Step5������������,�ҷ�������ֵ
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

	//����ǰ����CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "��������ǰCSUDIVIDEOGetWindowSizeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,��õ�Rect�����õĲ�һ��\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "��������ǰCSUDIVIDEOGetWinClipӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,��õ�Rect�����õĲ�һ��\n");

	//����ǰ����CSUDIVIDEOSetWindowSize,CSUDIVIDEOSetWinClip,
	//Ȼ��CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	stSetRect_SD1.m_nX = 50;
	stSetRect_SD1.m_nY = 50;
	stSetRect_SD1.m_nHeight = 50;
	stSetRect_SD1.m_nWidth = 50;

	stSetRect_HD1.m_nX = 1000;
	stSetRect_HD1.m_nY = 1000;
	stSetRect_HD1.m_nHeight = 50;
	stSetRect_HD1.m_nWidth = 50;

	eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, &stSetRect_SD1)), "��������ǰCSUDIVIDEOSetWinClipӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "��������ǰCSUDIVIDEOGetWinClipӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, &stSetRect_SD1, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,��õ�Rect�����õĲ�һ��\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, &stSetRect_HD1)), "��������ǰCSUDIVIDEOSetWindowSizeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "��������ǰCSUDIVIDEOGetWindowSizeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stSetRect_HD1, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,��õ�Rect�����õĲ�һ��\n");


	// ����ʱ����CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOShow(0,CSUDI_TRUE)), "��ʾ��Ƶʧ��!\n");
	
	eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "��������ʱCSUDIVIDEOGetWindowSizeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,��õ�Rect�����õĲ�һ��\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "��������ʱCSUDIVIDEOGetWinClipӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, CSUDI_NULL, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,��õ�Rect�����õĲ�һ��\n");
	
	//ֹͣ���ź�CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop (hPlayer), "ֹͣ����ʧ��!");
	bStopFlag = CSUDI_TRUE;
	
	eScreenDevice = EM_UDI_VOUT_DEVICE_SD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "ֹͣ����������CSUDIVIDEOGetWinClipӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, &stSetRect_SD1, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,��õ�Rect�����õĲ�һ��\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "ֹͣ����������CSUDIVIDEOGetWindowSizeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stSetRect_HD1, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,��õ�Rect�����õĲ�һ��\n");

	//�رղ��ž����CSUDIVIDEOGetWindowSize,CSUDIVIDEOGetWinClip
	//Ҫ������������ͨ���໥��Ӱ��
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
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
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, eScreenDevice, &stSetRect_SD2)), "�رղ�������CSUDIVIDEOSetWindowSizeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "�رղ�������CSUDIVIDEOGetWindowSizeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stSetRect_SD2, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,��õ�Rect�����õĲ�һ��\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "�رղ�������CSUDIVIDEOGetWinClipӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, &stSetRect_SD1, &stGetRect), "EM_UDI_VOUT_DEVICE_SD,��õ�Rect�����õĲ�һ��\n");
	eScreenDevice = EM_UDI_VOUT_DEVICE_HD;
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOSetWinClip(nVideoIndex, eScreenDevice, &stSetRect_HD2)), "�رղ�������CSUDIVIDEOSetWinClipӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWinClip(nVideoIndex, eScreenDevice, &stGetRect)), "�رղ�������CSUDIVIDEOGetWinClipӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinClip(nVideoIndex, eScreenDevice, &stSetRect_HD2, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,��õ�Rect�����õĲ�һ��\n");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIVIDEOGetWindowSize(nVideoIndex, eScreenDevice, &stGetRect)), "�رղ�������CSUDIVIDEOGetWindowSizeӦ�óɹ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== testCheckGetWinSize(nVideoIndex, eScreenDevice, &stSetRect_HD1, &stGetRect), "EM_UDI_VOUT_DEVICE_HD,��õ�Rect�����õĲ�һ��\n");
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hPlayer)
	{
		if(!bStopFlag)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��!");
		}
		if(!bCloseFlag)
		{
		    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
		}
		hPlayer = CSUDI_NULL;
        //�ָ�����
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
		CSTCPrint( "��ȡ%s �ֶ�Config�ļ���Ϣʧ��!",pkey);
		return CSUDI_FALSE;
	}

	if (1 == CSTKGetIntFromStr(acResult, 10))
	{
		return CSUDI_TRUE;	// ��ʾ��֧�ָ���ƽ̨
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
			return "SD��A/V";
		case EM_UDIVOUT_YC:
			return "S����";
		case EM_UDIVOUT_YUV:
			return "YPbPr�������";
		case EM_UDIVOUT_RF:
			return "RF";
		case EM_UDIVOUT_SCART:
			return "SCART��";
		case EM_UDIVOUT_HDMI:
			return "HDMI";
		case EM_UDIVOUT_DVO:
			return "DVO";
		default:
			return "δ֪�˿�";
	}
}

//@CASEGROUP:CSUDIVOUTDisable CSUDIVOUTEnable
//@DESCRIPTION: ����CSUDIVOUTDisable�ر�����˿�,CSUDIVOUTEnable������˿ڹ���
//@PRECONDITION:���������Ž�Ŀ
//@INPUT: nVideoIndex=0
//@INPUT: ����֧�ֵ�����˿�
//@REMARK:ƽ̨֧����Щ����˿�ͨ�����������ļ�[VIDEO]��VOUT_HAS_xx�ֶ�����
//@EXPECTATION:1. CSUDIVOUTDisable�ܹر�ָ���˿�
//@EXPECTATION:2. CSUDIVOUTEnable�ܴ�ָ���˿�
//@EXECUTIONFLOW: 1����ȡ���������ļ���VOUT�Ķ˿�����,��������֧��һ������˿�
//@EXECUTIONFLOW: 2�����Ų��Խ�Ŀ,�������ųɹ�
//@EXECUTIONFLOW: 3������CSUDIVOUTDisable���ιر�����֧�ֵ�����˿�,��ȷ���Ƿ��Ѿ��ر�
//@EXECUTIONFLOW: 4������CSUDIVOUTEnable���δ�����֧�ֵ�����˿�,�����򿪳ɹ�
//@EXECUTIONFLOW: 5������CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_VOUT_DisableEnable_0001(void)
{
	unsigned int unPort = 0;
	int i=0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	unPort = GetBoardOutputPort();

	CSTK_ASSERT_TRUE_FATAL(unPort,"û�л�ȡ��ƽ̨VOUT����,���ڲ��������ļ�������VOUT_HAS_xx�ֶ�\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");

	for (i=0;i<EM_UDIVOUT_NUM;i++)
	{
		if (unPort&(1<<i))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVOUTDisable(i),
				"disable vout port fail\n");
			CSTCPrint("��ȷ��%s����Ѿ��Ͽ������(Y/N)\n",VOutMsg(i));
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");
		}
	}

	for (i=0;i<EM_UDIVOUT_NUM;i++)
	{
		if (unPort&(1<<i))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVOUTEnable(i),
				"disable vout port fail\n");
			CSTCPrint("��ȷ��%s����Ѿ��ָ���ͼ�����(Y/N)\n",VOutMsg(i));
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");
		}
	}
	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return TRUE;
}

//@CASEGROUP:CSUDIVOUTDisable CSUDIVOUTEnable
//@CASEGROUP:CSUDISCREENSetResolution
//@DESCRIPTION: ����VOUT����˿ڵĴ򿪹ر�״̬���ֱܷ����л�Ӱ��,�Լ�����HDMI���Ӱ��
//@INPUT: nVideoIndex=0
//@INPUT: ����֧�ֵ�����˿�
//@REMARK:ƽ̨֧����Щ����˿�ͨ�����������ļ�[VIDEO]��VOUT_HAS_xx�ֶ�����
//@EXPECTATION:1.����˿ڹرպ�,�л��ֱ��ʲ�Ӱ��˿�״̬
//@EXPECTATION:2.���HDMI�߲�Ӱ��VOUT������˿�״̬ 
//@EXECUTIONFLOW: 1����ȡ���������ļ���VOUT�Ķ˿�����,��������֧��һ������˿�
//@EXECUTIONFLOW: 2�����Ų��Խ�Ŀ,�������ųɹ�
//@EXECUTIONFLOW: 3������CSUDISCREENSetResolution������������ó�PAL,����������ó�1080I
//@EXECUTIONFLOW: 4������CSUDIVOUTDisable���ιر�����֧�ֵ�����˿�,��ȷ���Ƿ��Ѿ��ر�
//@EXECUTIONFLOW: 5������CSUDISCREENSetResolution������������ó�NTSC,����������ó�720P,
//@EXECUTIONFLOW:ȷ�϶˿�״̬�ޱ仯
//@EXECUTIONFLOW: 6������CSUDISCREENSetResolution������������ó�PAL,����������ó�1080I
//@EXECUTIONFLOW:ȷ�϶˿�״̬�ޱ仯
//@EXECUTIONFLOW: 7����ʾ���HDMI�߲���,ȷ�ϲ��HDMI��Ӱ��VOUT�˿�״̬
//@EXECUTIONFLOW: 8������CSUDIVOUTEnable���δ�����֧�ֵ�����˿�,�����򿪳ɹ�
//@EXECUTIONFLOW: 9������CSUDIPLAYERClose�رղ�����
CSUDI_BOOL CSTC_VOUT_DisableEnable_0002(void)
{
	unsigned int unPort = 0;
	int i=0;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDISCREENType_E enDev;
	CSUDISCREENResolution_E enRes;
	BOOL bHDSupport = bIsSupportHD();

	unPort = GetBoardOutputPort();

	CSTK_ASSERT_TRUE_FATAL(unPort,"û�л�ȡ��ƽ̨VOUT����,���ڲ��������ļ�������VOUT_HAS_xx�ֶ�\n");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == LockFrequency( &hPlayer)), "��Ƶʧ��!\n");

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

	CSTCPrint("��ȷ��ƽ̨���е�����˿��Ѿ��Ͽ������(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

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

	CSTCPrint("�ֱ��ʱ����л��� NTSC,�����л���720P(����и���)\n");
	CSTCPrint("��ȷ��ƽ̨���е�����˿����ɶϿ������(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

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

	CSTCPrint("�ֱ��ʱ����л��� PAL,�����л���1080I(����и���)\n");
	CSTCPrint("��ȷ��ƽ̨���е�����˿����ɶϿ������(Y/N)\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");

	if (unPort&(1<<EM_UDIVOUT_HDMI))
	{
		CSTCPrint("��ε�HDMI���ٲ���\n");
		CSTCPrint("��ȷ�ϲ��HDMI�ߺ�,����������ɴ��ڹر�״̬(Y/N)\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");
	}

	for (i=0;i<EM_UDIVOUT_NUM;i++)
	{
		if (unPort&(1<<i))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVOUTEnable(i),
				"disable vout port fail\n");
			CSTCPrint("��ȷ��%s����Ѿ��ָ���ͼ�����(Y/N)\n",VOutMsg(i));
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"ȷ��Ч��ʧ��\n");
		}
	}
	CSTK_FATAL_POINT;
	
	if (hPlayer)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��!");
	}

	return TRUE;
}


