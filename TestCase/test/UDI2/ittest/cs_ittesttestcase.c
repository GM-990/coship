/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_ittesttestcase.h"
#include "udi2_osg.h"
#include "udi2_player.h"
#include "udi2_inject.h"
#include "cs_testkit.h"
#include "cs_testkit_porting.h"
#include "udiplus_debug.h"
#include "udi2_os.h"
#include "udi2_fs.h"
#include "../cs_udi2testcase.h"
#include "udi2_screen.h"
#include "udi2_video.h"
#include "udi2_audio.h"
#include "udi2_toolset.h"
#include"udi2_section.h"
#include "udiplus_os.h"
#include "udi2_flash.h"
#include "udi2_demux.h"

#undef INJECT_SEEK_SET
#undef INJECT_SEEK_CUR
#undef INJECT_SEEK_END
#define INJECT_SEEK_SET (0)
#define INJECT_SEEK_CUR (1)
#define INJECT_SEEK_END (2)
#undef PCM_BUFFER_SIZE

#define PCM_BUFFER_SIZE (1*1024*1024) //����ÿ��ע��Ĵ�С
#define REPEAT_TIMES (3)
#define INJECTTHREADPRO (100)
#define INJECTTHREADBUF (1024 * 148)
#define INJECT_INVALID_PID (0x1fff)
static unsigned  int g_uItTestStartAddress = 0;
static CSUDI_BOOL s_exitinject = CSUDI_FALSE;

#define MAX_PESES_COUNT_IN_TS (EM_UDI_CONTENT_PCR+1) /*��ο�CSUDIContentType_Eö�ٵ����͸���*/

typedef struct _TSStreamInfo_S
{
    char * m_pcName; //ע���ļ�����
    CSUDIStreamInfo_S    m_TSContentInfo[MAX_PESES_COUNT_IN_TS];
    unsigned int m_uBufferLen;
    CSUDI_BOOL m_bRun;
    CSUDI_HANDLE m_hInjecter;
    CSUDI_BOOL m_bSeek;
    CSUDI_HANDLE m_hSeekEvent;
    CSUDI_BOOL m_bOpenFileSuccess;
}TSStreamInfo_S;

typedef enum
{
	EM_TS_INJECT,
	EM_PS_INJECT,
	EM_MAX_INJECT
}EM_TYPE_INJECT;


static TSStreamInfo_S st_TSStreamInfo[] = 
{
    {
        "H.264.ts",/*EM_INJECT_TS_H264_AAC*/
       	{
			{0x1401, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_H264}},   
			{0x1402, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG_AAC}}, 
			{0x1401, EM_UDI_CONTENT_PCR, {0}}, 
			{INJECT_INVALID_PID, 0, {0}},   
			{INJECT_INVALID_PID, 0, {0}},   
       	},
		10*1024,	
		CSUDI_TRUE,
		CSUDI_NULL,
		CSUDI_FALSE,
		CSUDI_NULL,
		CSUDI_FALSE,
    },
    {
        "test.ps",/*EM_INJECT_TS_MPEG2_MPEG1*/
       	{
			{-1, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_MPEG2}},   
			{-1, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG1}}, 
			{INJECT_INVALID_PID, 0, {0}}, 
			{INJECT_INVALID_PID, 0, {0}},   
			{INJECT_INVALID_PID, 0, {0}},   
       	},
		64*1024,	
		CSUDI_TRUE,
		CSUDI_NULL,
		CSUDI_FALSE,
		CSUDI_NULL,
		CSUDI_FALSE,
    	},
};

typedef enum 
{
	EM_IT_MPEG2_PAL,                /*�¼Ӳ���������̨ PAL*/
	EM_IT_MPEG2_NTSC,
	EM_IT_COORDINATE_SHIFT,              /*��������ƫ����֤����*/
	EM_IT_MAX	
}ITTestServiceIndex_E;
/*
*  ����������CSUDIPLAYERSetStream�õ��ĸ������͵�PID��
*  �������ʹ�ã���(EM_STREAM_VIDEO|EM_STREAM_AUDIO)ͬʱ��������Ƶ��pid
*/
typedef enum
{
	EM_IT_STREAM_VIDEO = 1<<0,   //��Ƶ
	EM_IT_STREAM_AUDIO = 1<<1,	  //��Ƶ
	EM_IT_STREAM_PCR   = 1<<2,   //pcr
	EM_IT_STREAM_SUBTITLE = 1<<3, //subtitle
	EM_IT_STREAM_TELETEXT = 1<<4  //teletext
}ITTestStreamType_E;

/**
* �����������õ��Ľ�Ŀ��ص�������Ϣ
*/
typedef struct 
{	
	const char*          m_pcTsFilename;  	    //�����ļ�����
	const char* 		 m_pcServiceDescript; 	//��������
	int                  m_nVideoPid;           ///< ��������PID��-1��ʾ������
	CSUDIVIDStreamType_E m_eVidStreamType;      ///��Ƶ����
	int                  m_nAudioPid;           ///< ��������PID ��-1��ʾ������
	CSUDIAUDStreamType_E m_eAudStreamType;      ///<��Ƶ����
	int 				 m_nPcrPid;				///< PCR����PID��-1��ʾ������
	int 				 m_nSubPid;				///<SUBTITLE���͵�PID��-1��ʾ������
	int 				 m_nTelPid;				///<TELETEXT���͵�PID��-1��ʾ������
}ITTestServiceInfo_S;

static ITTestServiceInfo_S g_sITTestSeviceInfo[] ={
		{
			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
			"�¼Ӳ���������̨ PAL",
			0x300,
			EM_UDI_VID_STREAM_MPEG2,
			0x301,
			EM_UDI_AUD_STREAM_MPEG2,
			0x200,
			-1,
			-1
		}, /*EM_PLAYER_MPEG2_PAL*/
		{
			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
			"�¼Ӳ���������̨ NTSC",
			0x314,
			EM_UDI_VID_STREAM_MPEG2,
			0x315,
			EM_UDI_AUD_STREAM_MPEG2,
			0x201,
			-1,
			-1
		}, /*EM_PLAYER_MPEG2_NTSC*/
		{
			"Irdeto.PCR.CCITT.ts",
			"������Ƶ����ƫ��",
			0x64,
			EM_UDI_VID_STREAM_MPEG2,
			0x65,
			EM_UDI_AUD_STREAM_MPEG2,
			-1,
			-1,
			-1
		}/*EM_IT_COORDINATE_SHIFT */
};

/*
*����ͼƬ��Դ����ö��
*/
typedef enum
{
	EM_IT_IFRAME_720_576,
	EM_IT_IFRAME_1280_720,
	EM_IT_JPG_720_576,
	EM_IT_JPG_1280_720,
	EM_IT_IFRAME_720_576_2,
	EM_IT_IMAGE_MAX
}ITTestUDI2ImageType_E;
/*
* ͼƬ����֧������ö��
*/
typedef enum
{
	EM_IT_DECODE_IFRAME,   //֧��IFRAMEͼƬӲ����
	EM_IT_DECODE_JPG,   //֧��JPGͼƬӲ����
	EM_IT_DECODE_MAX 
}ITTestSupportDecIamgeType_E;

//ͼƬӲ����ͼƬ��Դ����
static char * g_aITImageFile[] = {
	"UDI2OSG_720_576.mpg",
	"UDI2OSG_1280_720.mpg",
	"UDI2OSG_720_576.jpg",
	"UDI2OSG_1280_720.jpg",
	"UDI2OSG_720_576_2.mpg",	
	""
};

typedef struct
{
    char * m_pcFileName; //ע���ļ�����
    CSUDIINJECTERPcmStartParam_S m_sStartParam;//PCM���Ų���
    CSUDIINJECTERPcmEndian_E m_eEndian;//�ļ����ݵĴ�С���ֽ�������
}ITTestPCMInfo_S;

static ITTestPCMInfo_S g_PCMInfo[] = {
   {
        "Amani.wav",
        {
        	16, 
			44100,
			2
		},
        EM_UDIINJECTER_LITTLE_ENDIAN,
    }
};
//������Ƶ����Ƶ��������ʼ����Ӧ�Ĳ�����Ϣ
static void initAVInfo(int nAudioId, int nVideoId)
{
	if(nAudioId != -1)
	{
		CSUDIAUDIOSetVolume(nAudioId, 31);
	}
	if(nVideoId != -1)
	{
		CSUDIVIDEOShow(nVideoId, CSUDI_TRUE);
		CSUDIVIDEOSetAspectRatio(nVideoId,EM_UDI_VOUT_DEVICE_SD, EM_UDIVIDEO_ASPECT_RATIO_AUTO);
		CSUDIVIDEOSetMatchMethod(nVideoId,EM_UDI_VOUT_DEVICE_HD,EM_UDIVIDEO_MATCH_METHOD_IGNORE);
	}
	return ;
}
CSUDI_BOOL CSTC_ITTEST_Init(void)
{
	//�ڱ�����������ִ��ǰ����
    char cBuf[64];  

	initAVInfo(0,0);
//	initAVInfo(1,1);

    memset(cBuf, '\0', sizeof(cBuf));       
    CSTKGetConfigInfo("FLASH", "CS_FLASHUDI2_TEST_START_ADDRESS", cBuf, sizeof(cBuf));  
    if(cBuf[0] != '\0') 
    {       
        g_uItTestStartAddress = CSTKGetIntFromStr(cBuf, 16);    
    }
    
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_ITTEST_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

static CSUDI_BOOL IT_iIsSupportHDDisplay()
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	int nResult = 0;
    char acBuf[32] = {0};
    memset(acBuf, '\0', sizeof(acBuf));
	nResult = CSTKGetConfigInfo("OSG", "CS_OSGUDI2_SURPORT_HD_SD_DISPLAY", acBuf, sizeof(acBuf));
	if (CS_TK_CONFIG_SUCCESS == nResult)
	{
		if (acBuf[0] == '1')
		{
			bRet = CSUDI_TRUE;
		}
	}

	return bRet;
}

/**
* �������ļ��л�ȡ�����Ե�ƽ̨�Ƿ�Ϊ�߱���ͬԴ
* ����CSUDI_TRUE��ʾ�Ǹ߱���ͬԴ������CSUDI_FALSE��ʾ���Ǹ߱���ͬԴ
*/
static CSUDI_BOOL IT_iIsShareHDDisplaySurface()
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
    int nResult = 0;
    char cBuf[32] = {0};
    memset(cBuf, '\0', sizeof(cBuf));
	nResult = CSTKGetConfigInfo("OSG", "CS_OSGUDI2_SHARE_HD_DISPLAY_SERFACE", cBuf, sizeof(cBuf));
	if (CS_TK_CONFIG_SUCCESS == nResult)
	{
		if (cBuf[0] == '1')
		{
			bRet = CSUDI_TRUE;
		}
	}

	return bRet;
}

static CSUDI_BOOL iFindSupportPixelFormat(CSUDIOSGPixelFormat_E *ePixelFormat)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	char cBuf[64] = {0};

	*ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_NUM;

	memset(cBuf, '\0', sizeof(cBuf));       
	CSTKGetConfigInfo("OSG", "CS_OSGUDI2_SURPORT_ARGB_8888", cBuf, sizeof(cBuf));
	
	if (cBuf[0] != '\0')
	{
		if (CSTKGetIntFromStr(cBuf, 10))
		{
			*ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;
			bRet = CSUDI_TRUE;
		}
		else
		{
			memset(cBuf, '\0', sizeof(cBuf));       
			CSTKGetConfigInfo("OSG", "CS_OSGUDI2_SURPORT_ARGB_1555", cBuf, sizeof(cBuf));

			if (cBuf[0] != '\0')
			{
				if (CSTKGetIntFromStr(cBuf, 10))
				{
					*ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_1555;
					bRet = CSUDI_TRUE;
				}
			}
			else
			{
				CSTCPrint("û���ҵ�֧�ֵ�����Ҫ�����ɫģʽ\n");
			}
		}
	}

	CSTCPrint("Find Pixelformat : %d (%d: ARGB1555, %d: ARGB8888, %d: wrong)\n", 
		*ePixelFormat, EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, EM_UDIOSG_PIXEL_FORMAT_ARGB_8888, EM_UDIOSG_PIXEL_FORMAT_NUM);

	return bRet;
}

/**
* ������ľ�������ת���ɶ�Ӧ�ĸ����ֵ
* �ýӿ���Ҫ����Video�ľ��������ת��
* �ýӿ���Ҫ���ڸ߱���ͬԴ��ƽ̨�Ĳ���
*/
static CSUDI_BOOL IT_iWinRect_SDTransformHD(CSUDIWinRect_S* psRect)
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
static CSUDI_BOOL IT_iOSGRect_SDTransformHD(CSUDIOSGRect_S* psRect)
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

static void IT_iOSGFill( const CSUDIOSGRect_S * psDstRect, unsigned int uPixel)
{
	CSUDI_HANDLE hDisplay = CSUDI_NULL;
	CSUDI_HANDLE hSurface_S = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurfaceInfo;
	CSUDIOSGPixelFormat_E ePixelFormat = EM_UDIOSG_PIXEL_FORMAT_ARGB_8888;

	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == iFindSupportPixelFormat(&ePixelFormat) && (ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_8888 || ePixelFormat == EM_UDIOSG_PIXEL_FORMAT_ARGB_1555)), 
				"û���ҵ��κ�֧�ֵ�����Ҫ���PixelFormat"
			);

	if (IT_iIsShareHDDisplaySurface())
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisplay) && (CSUDI_NULL != hDisplay)),"��ȡ�����Դ�ʧ��");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDisplay) && (CSUDI_NULL != hDisplay)),"��ȡ�����Դ�ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplay, &sSurfaceInfo)), "��ȡ�Դ���Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGCreateSurface(ePixelFormat, 
			sSurfaceInfo.m_nWidth, sSurfaceInfo.m_nHeight, &hSurface_S)),"����ƽ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGFill(hSurface_S, psDstRect, uPixel)),
					"Fill Display Failure !"
				);	
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay,psDstRect,hSurface_S,psDstRect,EM_UDIOSG_OPMODE_COPY)),
				"blit image failure"
			);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");		
	CSTK_FATAL_POINT
	if(hSurface_S != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"���ٻ�ͼƽ��ʧ��");	
		hSurface_S = CSUDI_NULL;
	}
	
 	return ;	
}

static void IT_iClearScreen()
{
    CSUDI_HANDLE hDisplay = CSUDI_NULL;
    CSUDI_HANDLE hDisplay_HD = CSUDI_NULL;
    CSUDI_BOOL bSupportHD = IT_iIsSupportHDDisplay();
	CSUDI_BOOL bShareHD = IT_iIsShareHDDisplaySurface();

	if (!bShareHD)
	{
	    CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDisplay) && (CSUDI_NULL != hDisplay)),"��ȡ�Դ�ʧ��");
		/*<***********��������ֱ����Fill *********Modified by lwh at 2010-12-09>*/
	    CSTK_ASSERT_TRUE_FATAL(
	                    (CSUDI_SUCCESS == CSUDIOSGFill(hDisplay, CSUDI_NULL, 0x0)),
	                    "Fill Display Failure !"
	                );  
	}

    if(bSupportHD)
    {
        CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hDisplay_HD)),
                    "��ȡ�����Դ�ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(
                        (CSUDI_SUCCESS == CSUDIOSGFill(hDisplay_HD, CSUDI_NULL, 0x0)),
                        "Fill Display Failure !"
                    );  
    } 

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");

	CSTK_FATAL_POINT

 	return ;		
}

static CSUDI_BOOL IT_iLockTuner(ITTestServiceIndex_E eSerIndex)
{
	return CSTC_UDI2PortingLock(0,g_sITTestSeviceInfo[eSerIndex].m_pcTsFilename);
}
/**
* param[in]: bIsSet ���ΪCSUDI_TRUE��ʾ����CSUDIPLAYERSetStream���ı䣬��֮����CSUDIPLAYERModifyStream
* param[in]: eSerIndex ��ӦPlayerTestServiceIndex_Eö�������еĶ���
* param[in]: dwStreamType ΪPlayerTestStreamType_Eö�����͵����,����:EM_IT_STREAM_VIDEO|EM_IT_STREAM_AUDIO
*/
static CSUDI_BOOL IT_iSetStream(CSUDI_BOOL bIsSet,CSUDI_HANDLE hPlayer,ITTestServiceIndex_E eSerIndex,unsigned int dwStreamType)
{
	CSUDIStreamInfo_S  sStreamInfo[10];  //���ᳬ��10��
	int nStreamCnt = 0;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	
	if(eSerIndex >= sizeof(g_sITTestSeviceInfo)/sizeof(ITTestServiceInfo_S))  return CSUDI_FALSE;

	switch(dwStreamType)
	{
		case (EM_IT_STREAM_VIDEO):
		{
			sStreamInfo[0].m_nPid = g_sITTestSeviceInfo[eSerIndex].m_nVideoPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			sStreamInfo[0].m_uStreamType.m_eVideoType = g_sITTestSeviceInfo[eSerIndex].m_eVidStreamType;
			nStreamCnt = 1;
			
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sITTestSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		case (EM_IT_STREAM_AUDIO):
		{
			sStreamInfo[0].m_nPid = g_sITTestSeviceInfo[eSerIndex].m_nAudioPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
			sStreamInfo[0].m_uStreamType.m_eAudioType = g_sITTestSeviceInfo[eSerIndex].m_eAudStreamType;
			nStreamCnt = 1;
			
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sITTestSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		case (EM_IT_STREAM_VIDEO|EM_IT_STREAM_AUDIO):
		{
			sStreamInfo[0].m_nPid = g_sITTestSeviceInfo[eSerIndex].m_nVideoPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			sStreamInfo[0].m_uStreamType.m_eVideoType = g_sITTestSeviceInfo[eSerIndex].m_eVidStreamType;
			
			sStreamInfo[1].m_nPid = g_sITTestSeviceInfo[eSerIndex].m_nAudioPid;
			sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
			sStreamInfo[1].m_uStreamType.m_eAudioType = g_sITTestSeviceInfo[eSerIndex].m_eAudStreamType;
			nStreamCnt = 2;
			
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sITTestSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		case (EM_IT_STREAM_VIDEO|EM_IT_STREAM_AUDIO|EM_IT_STREAM_PCR):
		{
			sStreamInfo[0].m_nPid = g_sITTestSeviceInfo[eSerIndex].m_nVideoPid;
			sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
			sStreamInfo[0].m_uStreamType.m_eVideoType = g_sITTestSeviceInfo[eSerIndex].m_eVidStreamType;
			
			sStreamInfo[1].m_nPid = g_sITTestSeviceInfo[eSerIndex].m_nAudioPid;
			sStreamInfo[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
			sStreamInfo[1].m_uStreamType.m_eAudioType = g_sITTestSeviceInfo[eSerIndex].m_eAudStreamType;

			if (g_sITTestSeviceInfo[eSerIndex].m_nPcrPid <= 0)
			{
				nStreamCnt = 2;
			}
			else
			{
				sStreamInfo[2].m_nPid = g_sITTestSeviceInfo[eSerIndex].m_nPcrPid;
				sStreamInfo[2].m_eContentType = EM_UDI_CONTENT_PCR;
				nStreamCnt = 3;
			}
			CSTCPrint("Playing file %s  streamtype= 0x%x \n",g_sITTestSeviceInfo[eSerIndex].m_pcTsFilename,dwStreamType);
		}
		break;
		default:
			CSTCPrint("Not Such Stream Type !!!!!!!\n");
			return CSUDI_FALSE;
	}
	CSTCPrint("���ڲ��� %s...... ��Ŀ\r\n",g_sITTestSeviceInfo[eSerIndex].m_pcServiceDescript);
	CSTCPrint("Playing program %s  .............\n",g_sITTestSeviceInfo[eSerIndex].m_pcServiceDescript);
	if(bIsSet)
	{
		nRet = CSUDIPLAYERSetStream(hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL);
	}
	else
	{
		nRet = CSUDIPLAYERModifyStream(hPlayer,sStreamInfo,nStreamCnt);
	}
	return (nRet == CSUDI_SUCCESS);
}
static CSUDI_BOOL IT_iIsSupportDecImage(ITTestSupportDecIamgeType_E eSupportType)
{
	char cImageType[64] = {0};
	char cBuf[64];
	memset(cBuf, '\0', sizeof(cBuf));
	switch(eSupportType)
	{
		case EM_IT_DECODE_IFRAME:
			strcpy(cImageType,"CS_UDI2OSG_DECODE_IFRAME");
			break;
		case EM_IT_DECODE_JPG:
			strcpy(cImageType,"CS_UDI2OSG_DECODE_JPG");
			break;
		default:
			CSTCPrint("OSG_iTestDecImage ����δ֪\n");
			return CSUDI_FALSE;
	}
	CSTKGetConfigInfo("OSG", cImageType, cBuf, sizeof(cBuf));
	
	if(cBuf[0] == '1')
	{
		return  CSUDI_TRUE;
	}
	return  CSUDI_FALSE;
}
/*
*��ȡͼƬ����
*/
static unsigned int  IT_iGetPhoto(const char *photoName, unsigned char **pBuff )
{
	CSUDI_HANDLE fp = CSUDI_NULL;
    unsigned int nIframeLen = 0;
	CSUDI_BOOL bOpen = CSUDI_FALSE;

	if(CSUDI_NULL != (fp = CSTKP_FOpen(photoName, "rb")))
	{
		CSTCPrint("ͼƬ%s�򿪳ɹ�\n ", photoName);
		bOpen = CSUDI_TRUE;
	}

	else
	{
		CSTCPrint("ͼƬ%s ��ʧ��\n", photoName);
		bOpen = CSUDI_FALSE;
	}

	if(bOpen)
	{
		if(0 == CSTKP_FSeek(fp, 0, CSTK_SEEK_END))
		{
			nIframeLen = CSTKP_FTell(fp);
			CSTCPrint("iframeLen = 0x%x\n", nIframeLen);
			
			if(nIframeLen > 0 && nIframeLen < 4*1920*1080 )       //ͼƬ��С�Ϸ����ж�1920*1080�ֱ���ARGB8888 
			{
				*pBuff = CSUDIOSMalloc(nIframeLen); 
				//*pBuff = malloc(nIframeLen);
				if(*pBuff == CSUDI_NULL)
				{
					CSTCPrint("����ռ�ʧ��!\n");
					if(fp != CSUDI_NULL)
					{
						CSTKP_FClose(fp);
					}
					return 0;
				}
				memset(*pBuff, 0,nIframeLen);

		   		if(0 != CSTKP_FSeek(fp,  0, CSTK_SEEK_SET))
	         	{
					CSTCPrint("������ʼָ��ʧ��!\n");
					bOpen = CSUDI_FALSE;
		   		}
	
				if(nIframeLen != (CSTKP_FRead(*pBuff, sizeof(unsigned char),nIframeLen, fp)))
				{
					CSTCPrint("��%s�ļ�ʧ��!\n", photoName);
					bOpen = CSUDI_FALSE;
				}
			}
			else
			{
				CSTCPrint("ͼƬ��С������ȷ��Χ��ʧ��!\n");
				bOpen = CSUDI_FALSE;
			}
		}
	
		else
		{
			CSTCPrint("Ѱ��ͼƬ%s ĩָ��ʧ��!\n", photoName);
			bOpen = CSUDI_FALSE;
		}
	}
	
	if(fp != CSUDI_NULL)
	{
		CSTKP_FClose(fp);
	}

	if (bOpen)
	{
		return nIframeLen;
	}
	else
	{
		return 0;
	}

}
static CSUDI_BOOL IT_iTestDecImageWithScaleMode(
				ITTestSupportDecIamgeType_E eSupportType,/*Ҫ�����ͼƬ����*/
				ITTestUDI2ImageType_E eImageType,        /*ͼƬ��������*/
				CSUDIOSGScalingMode_E eScalingMode,      /*����ģʽ*/
				const CSUDIOSGRect_S * psDstRect,         /*�����������*/
				CSUDI_HANDLE *phSurface,
				unsigned char ** pcBuff,
				unsigned int * pnImageLen
		)
{
	unsigned char * pcImageBuff = CSUDI_NULL;
//	unsigned int nImageLen = 0;
	char cType[32];
	
	switch(eSupportType)
	{
		case EM_IT_DECODE_IFRAME:
			strcpy(cType,"IFRAME");
			break;
		case EM_IT_DECODE_JPG:
			strcpy(cType,"JPEG");
			break;
		default:
			CSTCPrint("IT_iTestDecImageWithScaleMode ����δ֪\n");
			return CSUDI_FALSE;
	}
	*pnImageLen = IT_iGetPhoto(g_aITImageFile[eImageType],&pcImageBuff);


	if(CSASSERT_FAILED((pcImageBuff != CSUDI_NULL) && *pnImageLen != 0))
	{
		CSTCPrint("get photo data failure \n");
		return CSUDI_FALSE;
	}
	if(CSASSERT_FAILED(CSUDI_SUCCESS == CSUDIOSGDecodeImage(cType,pcImageBuff,*pnImageLen,eScalingMode,psDstRect,phSurface)
				&& (*phSurface != CSUDI_NULL)))
	{
		CSTCPrint("decode image failure\n");
		if(pcImageBuff != CSUDI_NULL)
		{
			CSUDIOSFree(pcImageBuff);
			pcImageBuff = CSUDI_NULL;
			CSTCPrint("[UDI2IT]CSUDIOSFree success !! \r\n");
		}		
		return CSUDI_FALSE;
	}
	
	*pcBuff = pcImageBuff;
	return CSUDI_TRUE;	
}

typedef struct{
	CSUDIOSGSurfaceInfo_S * phSurfaceInfo;
	CSUDI_HANDLE hInjecter;
}ITInjectESThreadParam_S;

//Ѱ����Ƶ������
static int IT_FindVideoDecoder(CSUDIVIDStreamType_E eFindVideoType)
{
	CSUDIVIDEOCapability_S sVideoCapabilityInfo;
	int nVideoDecCount = 0;
	int nVideoID = -1;
	int i = 0;
       	int k = 0;

	CSUDIVIDEOGetCount(&nVideoDecCount);	
	for (i = 0; i < nVideoDecCount; i++)
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

static void  fniframeCallback(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{
	if( eEvent==EM_UDIPLAYER_VIDEO_FRAME_COMING)
	{ 
		CSTCPrint("new frame come,show video\n");
		CSUDIVIDEOShow(0, CSUDI_TRUE);

		if (CSUDI_SUCCESS != CSUDIPLAYERDelPlayerCallback(hPlayer, fniframeCallback, EM_UDIPLAYER_VIDEO_FRAME_COMING, pvUserData))
		{
			CSTCPrint("[%s, %d]CSUDIPLAYERDelPlayerCallback failed!\n", __FUNCTION__, __LINE__);
		}
	}
}

/*
����I֡�������ûص�����
*/
static int   s_nIttestTestdata = 0;
static void  fnIttestCallback1(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{
	if( eEvent==EM_UDIPLAYER_VIDEO_FRAME_COMING)
	{ 
		CSTCPrint("��֡����,������Ϣ!!\n");
		s_nIttestTestdata = 1001;

		CSUDIVIDEOShow(0, CSUDI_TRUE);

		if (CSUDI_SUCCESS != CSUDIPLAYERDelPlayerCallback(hPlayer, fnIttestCallback1, EM_UDIPLAYER_VIDEO_FRAME_COMING, pvUserData))
		{
			CSTCPrint("[%s, %d]CSUDIPLAYERDelPlayerCallback failed!\n", __FUNCTION__, __LINE__);
		}
	}
}

static CSUDI_BOOL IT_iPlayIFrame(ITTestUDI2ImageType_E nImageIndex,CSUDI_HANDLE *phIFramePlayer,CSUDI_HANDLE *phIframeInject,CSUDI_HANDLE *phInjectIFrameThread,unsigned char ** pcBuff )
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDIINJECTERChnl_S sInjectChnl;
	CSUDIINJECTEROpenParam_S sInjectOpenParam;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	unsigned int nImageLen = 0;
	unsigned char * pcImageBuff = CSUDI_NULL;
	CSUDIINJECTERIFrameParam_S 	sSyncMode;
	int nUserData = 0x88888888;
	int i = 0;
	s_nIttestTestdata = 0;
	
	if(phIFramePlayer == CSUDI_NULL || phIFramePlayer == CSUDI_NULL ||
		phIframeInject == CSUDI_NULL || phInjectIFrameThread == CSUDI_NULL)
	{
		return CSUDI_FALSE;
	}

	nImageLen = IT_iGetPhoto(g_aITImageFile[nImageIndex],&pcImageBuff);
	if(nImageLen == 0)
	{
		CSTCPrint("step 1 decode Iframe image failure !\n");
		return bRet;
	}
	*pcBuff = pcImageBuff;

	sInjectChnl.m_nAudioDecoder = -1;
	sInjectChnl.m_nDemux = -1;
	sInjectChnl.m_nVideoDecoder = 0;  //I֡����ֻ��ҪVIDEO������

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_IFRAME;  //I֡ΪVIDEO����
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_ES;				//I֡ΪES��������
	nRet = CSUDIINJECTEROpen(&sInjectChnl, &sInjectOpenParam, phIframeInject);
/*<!-- Modify by 903126,2011-1-5 9:17:33: */
/* Note: add verifying of device busy*/
	if(CSUDIINJECTER_ERROR_DEVICE_BUSY == nRet || CSUDI_SUCCESS != nRet || *phIframeInject == CSUDI_NULL)
 /* Modify end 2011-1-5 9:17:33 --!>*/
	{
		CSTCPrint("step 2 open injecter  failure nRet =%d!!\n",nRet);
		return CSUDI_FALSE;
	}
	
	sSyncMode.m_pcIFrameData = (char *)pcImageBuff;
	sSyncMode.m_nDataLen = nImageLen;
	CSUDIINJECTERSetAttribute(*phIframeInject,EM_UDIINJECTER_IFRAME_PARAMS,&sSyncMode);
			
	nRet = CSUDIINJECTERStart(*phIframeInject);
	if(CSUDI_SUCCESS != nRet)
	{
		CSTCPrint("step 3 start injecter  failure !\n");
		return CSUDI_FALSE;
	}

	sPlayerChnl.m_nDemux = -1;
	sPlayerChnl.m_nAudioDecoder = -1;
	sPlayerChnl.m_nVideoDecoder = 0;
	
	nRet = CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_INJECT,phIFramePlayer);
	
	if(CSUDIPLAYER_ERROR_DEVICE_BUSY == nRet || CSUDI_SUCCESS != nRet || *phIFramePlayer == CSUDI_NULL)
	{
		CSTCPrint("step 4 open iframe player failure nRet =%d!!\n",nRet);
		
		return CSUDI_FALSE;
	}
	
	nRet = CSUDIPLAYERAddPlayerCallback(*phIFramePlayer,fnIttestCallback1,EM_UDIPLAYER_VIDEO_FRAME_COMING,&nUserData);//
	if(CSUDI_SUCCESS!=nRet)
	{
		CSTCPrint("step 5 CSUDIPLAYERAddPlayerCallback failure !!\n");
		return CSUDI_FALSE;

	}
	sStreamInfo[0].m_nPid = -1;
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	
	nRet = CSUDIPLAYERSetStream(*phIFramePlayer,sStreamInfo, 1, CSUDI_NULL);
	if(CSUDI_SUCCESS != nRet)
	{
		CSTCPrint("step 6 set iframe stream failure!\n");
		return CSUDI_FALSE;
	}
		
	nRet = CSUDIPLAYERStart(*phIFramePlayer);
	if(CSUDI_SUCCESS != nRet)
	{
		CSTCPrint("step 7 start iframe player failure!\n");
		return CSUDI_FALSE;
	}
	for(i = 0;i < 10 ;i ++)
	{
		if(s_nIttestTestdata==1001)
		{
		    break;
		}
		CSUDIOSThreadSleep(2000);
	}
	if(s_nIttestTestdata !=1001)
	{
		CSTCPrint("�ص���Ϣ����!\n");
		return CSUDI_FALSE;
	}

	return CSUDI_TRUE;
}

//���Ž�Ŀ�������Ŀ����ʧ���򷵻�-1���ɹ��򷵻���Ƶ������������
static int IT_iPlayTV(CSUDI_HANDLE * phAVPlayer,ITTestServiceIndex_E eSerIndex,ITTestStreamType_E nStreamType)
{
	int nRet = -1;
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	CSUDIPlayerChnl_S sPlayerChnl;
	int nVideoDecoder = IT_FindVideoDecoder(EM_UDI_VID_STREAM_MPEG2);

	if (IT_iLockTuner(eSerIndex))
	{
		sPlayerChnl.m_nDemux = 0;
		sPlayerChnl.m_nAudioDecoder = 0;
		switch(nStreamType)
		{
			case (EM_IT_STREAM_AUDIO):
			{
            			sPlayerChnl.m_nVideoDecoder = CSUDI_INVALID_INDEX;
			}
        		break;
			default:
			{
            			sPlayerChnl.m_nVideoDecoder = nVideoDecoder;
			}
        		break;
		}

		eRet = CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, phAVPlayer);
		if ((CSUDI_SUCCESS == eRet) && (*phAVPlayer != CSUDI_NULL))
		{
			if (IT_iSetStream(CSUDI_TRUE,*phAVPlayer,eSerIndex,nStreamType))
			{
				eRet = CSUDIPLAYERStart(*phAVPlayer);
				if (CSUDI_SUCCESS == eRet)
				{
					nRet = nVideoDecoder;
				}
				else
				{
					CSTCPrint("start av player failure\n");
				}
			}
			else
			{
				CSTCPrint("set av stream failure\n");
			}
		}
		else
		{
			CSTCPrint("open av player failure\n");
		}
	}
		
	return nRet;		
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@CASEGROUP: 3��OSG
//@DESCRIPTION:���Ե�������MPEG2��Ƶ+����I֡
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧��I֡ͼƬע��
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.mpg
//@EXPECTATION:�����ܹ�����������Ƶ��ͬʱ������I֡
//@EXECUTIONFLOW: 1����ȡI֡ͼƬUDI2OSG_720_576.mpg�����ݣ�������ȡ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIINJECTEROpen����һ��ESע���������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIINJECTERStart��ʼע��I֡������ע��ɹ�
//@EXECUTIONFLOW: 4������CSUDIPLAYEROpen����һ������I֡�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 5������CSUDIPLAYERSetStream���ý�Ŀ��ϢΪI֡��Ϣ
//@EXECUTIONFLOW: 6������CSUDIPLAYERStart��ʼ����I֡,��������CSUDI_SUCCESS�����Ҳ���I֡
//@EXECUTIONFLOW: 7������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 8������CSUDIPLAYEROpen����һ��������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 9������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 10������CSUDIPLAYERStart��ʼ������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 11�����������ܿ���I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ
//@EXECUTIONFLOW: 12������CSUDIPLAYERModifyStream������Ƶ��ϢΪEM_IT_MPEG2_NTSC���������óɹ�
//@EXECUTIONFLOW: 13�����������ܿ���I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ
//@EXECUTIONFLOW: 14���ͷ�ͼƬ��Դ������surface������ע���������ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0001(void)
{
	CSUDI_HANDLE hAudioPlayer = CSUDI_NULL;
	CSUDI_HANDLE hIFramePlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIINJECTERChnl_S sInjectChnl;
	CSUDIINJECTEROpenParam_S sInjectOpenParam;
	CSUDIStreamInfo_S  sStreamInfo[1];  
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	CSUDIINJECTERIFrameParam_S 	sSyncMode;
	int nUserData = 0x11111111;

	CSTK_ASSERT_TRUE_FATAL(
			(nImageLen = IT_iGetPhoto("UDI2OSG_720_576.mpg",&pcBuff))>0,
			"step 1 get Iframe image file failure !"
		);

	if(CSASSERT_FAILED((pcBuff != CSUDI_NULL) && nImageLen != 0))
	{
		CSTCPrint("get photo data failure \n");
		return CSUDI_FALSE;
	}
	sInjectChnl.m_nAudioDecoder = -1;
	sInjectChnl.m_nDemux = -1;
	sInjectChnl.m_nVideoDecoder = 0;  //I֡����ֻ��ҪVIDEO������

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_IFRAME;  //I֡ΪVIDEO����
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_ES;				//I֡ΪES��������
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTEROpen(&sInjectChnl, &sInjectOpenParam, &hINJECTER) && hINJECTER != CSUDI_NULL),
			"step 2 open injecter  failure !"
		);

	sSyncMode.m_pcIFrameData = (char *)pcBuff;
	sSyncMode.m_nDataLen = nImageLen;
	CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_IFRAME_PARAMS,&sSyncMode);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER)),
			"step 3 start injecter  failure !"
		);

	//IFramePlayer
	sPlayerChnl.m_nDemux = -1;
	sPlayerChnl.m_nAudioDecoder = -1;
	sPlayerChnl.m_nVideoDecoder = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_INJECT,&hIFramePlayer)) && hIFramePlayer != CSUDI_NULL,
			"step 4 open iframe player failure !"
		);
	sStreamInfo[0].m_nPid = -1;
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIPLAYERSetStream(hIFramePlayer,sStreamInfo, 1, CSUDI_NULL),
			"step 5 set iframe stream failure !"
		);

	/*for test target ,we should hide video*/
	CSUDIVIDEOShow(0, CSUDI_FALSE);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hIFramePlayer,fniframeCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &nUserData),
		"error:fail to add iframe player call back!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hIFramePlayer), "step 6 start iframe player failure!");

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IT_iLockTuner(EM_IT_MPEG2_PAL), "step 7 lock singal failure");

	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nAudioDecoder = 0;
	sPlayerChnl.m_nVideoDecoder = -1;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE,&hAudioPlayer)) && hAudioPlayer != CSUDI_NULL,
			"step 8 open audio player failure "
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE,hAudioPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO),
			"step 9 set audio stream failure "
		);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hAudioPlayer), "step 10 start audio player failure!");
	
	CSTCPrint("���������ܿ���I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ \n");
    	CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the screen and to get normal audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 11 check result failure!");


	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_FALSE,hAudioPlayer,EM_IT_MPEG2_NTSC,EM_IT_STREAM_AUDIO),
			"step 12 set audio stream failure "
		);

	CSTCPrint("���������ܿ���I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the screen and to get normal audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 13 check result failure!");
    
	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(0, CSUDI_TRUE);
		
	if (hAudioPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAudioPlayer);
		hAudioPlayer=CSUDI_NULL;
	}
	if (hIFramePlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hIFramePlayer);
		hIFramePlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}

	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
	}	
	
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@CASEGROUP: 3��OSG
//@DESCRIPTION:���Ե�������MPEG2��Ƶ+����I֡+������ʾJPEGͼƬ(��ʾ��)
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧��I֡ͼƬע��
//@PRECONDITION:3��֧��Ӳ����JPEGͼƬ
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.mpg
//@INPUT: 3��UDI2OSG_720_576.jpg
//@EXPECTATION:�����ܹ�����������Ƶ��ͬʱ������I֡����ʾJPEGͼƬ
//@EXECUTIONFLOW: 1����ȡI֡ͼƬUDI2OSG_720_576.mpg�����ݣ�������ȡ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIINJECTEROpen����һ��ESע���������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIINJECTERStart��ʼע��I֡������ע��ɹ�
//@EXECUTIONFLOW: 4������CSUDIPLAYEROpen����һ������I֡�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 5������CSUDIPLAYERSetStream���ý�Ŀ��ϢΪI֡��Ϣ
//@EXECUTIONFLOW: 6������CSUDIPLAYERStart��ʼ����I֡,��������CSUDI_SUCCESS�����Ҳ���I֡
//@EXECUTIONFLOW: 7������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 8������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 9������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 10������CSUDIPLAYERStart��ʼ������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 11������CSUDIOSGDecodeImage����ָ��JPEGͼƬUDI2OSG_720_576.jpg������ģʽΪEM_UDIOSG_SCALE_FULLSIZE����������ɹ�
//@EXECUTIONFLOW: 12������CSUDIOSGGetDisplaySurface��ȡ�Դ����������ɹ�
//@EXECUTIONFLOW: 13������CSUDIOSGBlit��JPEGͼƬblit���Դ�������������������ɹ�
//@EXECUTIONFLOW: 14�����������ܿ���I֡ͼƬUDI2OSG_720_576.mpg����ͼƬ���ܿ�����ʾ���JPEGͼƬUDI2OSG_720_576.jpg,���ܹ�����������Ƶ
//@EXECUTIONFLOW: 15������CSUDIPLAYERModifyStream������Ƶ��ϢΪEM_IT_MPEG2_NTSC���������óɹ�
//@EXECUTIONFLOW: 16�����������ܿ���I֡ͼƬUDI2OSG_720_576.mpg����ͼƬ���ܿ�����ʾ���JPEGͼƬUDI2OSG_720_576.jpg,���ܹ�����������Ƶ
//@EXECUTIONFLOW: 17���ͷ�ͼƬ��Դ������surface������ע���������ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0002(void)
{
	CSUDI_HANDLE hAudioPlayer = CSUDI_NULL;
	CSUDI_HANDLE hIFramePlayer = CSUDI_NULL;
	CSUDI_HANDLE hJPEGSurface = CSUDI_NULL;
	CSUDI_HANDLE hDispSurface = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sJPEGSurInfo;
	CSUDIINJECTERChnl_S sInjectChnl;
	CSUDIINJECTEROpenParam_S sInjectOpenParam;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIOSGRect_S sBlitRect;
	CSUDIStreamInfo_S  sStreamInfo[1];
	unsigned char * pcBuff = CSUDI_NULL;
	unsigned int nImageLen = 0;
	unsigned char * pcJpgBuff = CSUDI_NULL;
    unsigned int nJpgLen = 0;
	CSUDIINJECTERIFrameParam_S 	sSyncMode;
    CSUDI_HANDLE hHDDispSurface;
	int nUserData = 0x22222222;
	
	CSTK_ASSERT_TRUE_FATAL(
			(nImageLen = IT_iGetPhoto("UDI2OSG_720_576.mpg",&pcBuff))>0,
			"step 1 get Iframe image file failure !"
		);

	sInjectChnl.m_nAudioDecoder = -1;
	sInjectChnl.m_nDemux = -1;
	sInjectChnl.m_nVideoDecoder = 0;  //I֡����ֻ��ҪVIDEO������

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_IFRAME;  //I֡ΪVIDEO����
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_ES;				//I֡ΪES��������
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTEROpen(&sInjectChnl, &sInjectOpenParam, &hINJECTER) && hINJECTER != CSUDI_NULL),
			"step 2 open injecter  failure !"
		);
	
	sSyncMode.m_pcIFrameData = (char *)pcBuff;
	sSyncMode.m_nDataLen = nImageLen;
	CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_IFRAME_PARAMS,&sSyncMode);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER)),
			"step 3 start injecter  failure !"
		);

	//IFramePlayer
	sPlayerChnl.m_nDemux = -1;
	sPlayerChnl.m_nAudioDecoder = -1;
	sPlayerChnl.m_nVideoDecoder = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_INJECT,&hIFramePlayer)) && hIFramePlayer != CSUDI_NULL,
			"step 4 open iframe player failure !"
		);
	sStreamInfo[0].m_nPid = -1; //no used
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStreamInfo[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIPLAYERSetStream(hIFramePlayer,sStreamInfo, 1, CSUDI_NULL),
			"step 5 set iframe stream failure !"
		);

	/*for test target ,we should hide video*/
	CSUDIVIDEOShow(0, CSUDI_FALSE);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hIFramePlayer,fniframeCallback,EM_UDIPLAYER_VIDEO_FRAME_COMING, &nUserData),
		"error:fail to add iframe player call back!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hIFramePlayer), "step 6 start iframe player failure!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IT_iLockTuner(EM_IT_MPEG2_PAL), "step 7 lock singal failure");

	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nAudioDecoder = 0;
	sPlayerChnl.m_nVideoDecoder = -1;
	
	//audio player
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE,&hAudioPlayer)) && hAudioPlayer != CSUDI_NULL,
			"step 8 open player failure "
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE,hAudioPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO),
			"step 9 set stream failure "
		);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hAudioPlayer), "step 10 start player failure!");

	if(IT_iIsSupportDecImage(EM_IT_DECODE_JPG))
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == IT_iTestDecImageWithScaleMode(EM_IT_DECODE_JPG,EM_IT_JPG_720_576,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hJPEGSurface,&pcJpgBuff,&nJpgLen)),
				"step 11 decode jpeg image failure !"
			);

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hJPEGSurface, &sJPEGSurInfo)),
				"Get JPEG surface info failure !"
			);	
		
		//��Ļ����
		sBlitRect.m_nX = 60;
		sBlitRect.m_nY = 48;
		sBlitRect.m_nWidth = 600;
		sBlitRect.m_nHeight =  480;
		
		if (!IT_iIsShareHDDisplaySurface())
		{
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDispSurface) && hDispSurface != CSUDI_NULL),
				"step 12 get display surface failure !"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hDispSurface, &sBlitRect, hJPEGSurface, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
				"step 13 blit failure !"
				);	
		}

		if(IT_iIsSupportHDDisplay())
		{
			IT_iOSGRect_SDTransformHD(&sBlitRect);
		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hHDDispSurface) && hHDDispSurface != CSUDI_NULL),
					"step 12 get display surface failure !"
					);

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGBlit(hHDDispSurface, &sBlitRect, hJPEGSurface, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
					"step 13 blit failure !"
			        );	
		}	
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"osg sync  failure !"
			);

		CSUDIOSThreadSleep(50);
	
		CSTCPrint("���������ܿ���I֡����ͼƬ���ܿ�����ʾ���JPEGͼƬ�����ܹ�����������Ƶ \n");
		CSTCPrint("We expect one Iframe on screen and one JPEG picture above the IFrame picture,"\
			"and get normal audio output at the same time\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 14 check result failure!");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_FALSE,hAudioPlayer,EM_IT_MPEG2_NTSC,EM_IT_STREAM_AUDIO),
			"step 15 set stream failure "
		);

		CSTCPrint("���������ܿ���I֡����ͼƬ���ܿ�����ʾ���JPEGͼƬ�����ܹ�����������Ƶ \n");
    		CSTCPrint("We expect one Iframe on screen and one JPEG picture above the IFrame picture,"\
                		"and get normal audio output at the same time\n");

		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 16 check result failure!");
	}
		
	CSTK_FATAL_POINT

	if (hIFramePlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hIFramePlayer);
		hIFramePlayer=CSUDI_NULL;
	}
	if (hAudioPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAudioPlayer);
		hAudioPlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}

	if (hJPEGSurface)
	{
		CSUDIOSGDestroySurface(hJPEGSurface);
		
		if (!IT_iIsShareHDDisplaySurface())
		{
			CSUDIOSGFill(hDispSurface, CSUDI_NULL, 0);
		}
		
		if (IT_iIsSupportHDDisplay())
		{
		    CSUDIOSGFill(hHDDispSurface, CSUDI_NULL, 0);
		}

		CSUDIOSGSync();
	}

	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
	}	
	if(pcJpgBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcJpgBuff);
		pcJpgBuff = CSUDI_NULL;
	}

	CSUDIVIDEOShow(0, CSUDI_TRUE);
	
	return CSUDI_TRUE;
}

typedef struct{
	char * pcFile;
	CSUDI_HANDLE hInjecter;
}ITInjectPCMParam_S;

static CSUDI_BOOL g_bInjectPCMFinish = CSUDI_FALSE;

static void * InjectFSOpen(const char * pcFileName, const char * pcMode)
{
	return (void *)CSTKP_FOpen(pcFileName, pcMode);
}

static int  InjectFSSeek(void *pFile, long lOffset, unsigned int uOrigin)
{		
	return CSTKP_FSeek((CSUDI_HANDLE)pFile, lOffset,  uOrigin);

}

static long  InjectFSTell(void *pFile)
{	
	return CSTKP_FTell((CSUDI_HANDLE)pFile);
}

static int InjectFSRead(void* pFile, char * pcBuf, unsigned int uCount)
{
	return CSTKP_FRead(pcBuf, 1, uCount, (CSUDI_HANDLE)pFile);
}

static int  InjectFSClose(void *pFile)
{
	return CSTKP_FClose((CSUDI_HANDLE)pFile);
}

static CSUDI_BOOL getInjecterFile(const char *pFileName, void **file, long *pLength)
{
    void *pFileTemp = NULL;
    int nReturn = -1;
     
    pFileTemp = InjectFSOpen(pFileName, "r");

    if (!pFileTemp)
    {
    	CSTCPrint("���������ļ� %s ��ʧ�ܣ���ȷ��testdataĿ¼�´��ڸ��ļ�!!!\n", pFileName);
        CSTCPrint("[in getInjecterFile]InjectFSOpen is failed!file name is %s\n", pFileName);
        return CSUDI_FALSE;
    }

    nReturn = InjectFSSeek(pFileTemp,0,INJECT_SEEK_END);
    if(nReturn != 0)
    {
        CSTCPrint("[in getInjecterFile]InjectFSSeek end is failed!\n\r");
        return CSUDI_FALSE; 
    }

    *pLength = InjectFSTell(pFileTemp);
    if(*pLength < 0)
    {
        CSTCPrint("[in getInjecterFile]InjectFSTell is failed!\n\r");
        return CSUDI_FALSE; 
    }

    nReturn = InjectFSSeek(pFileTemp,0,INJECT_SEEK_SET);
    if(nReturn != 0)
    {
        CSTCPrint("[in getInjecterFile]InjectFSSeek begin is failed!\n\r");
        return CSUDI_FALSE; 
    }

    *file = pFileTemp;
    
     return CSUDI_TRUE;   
}

static void iInjectPCMFile(void * pvArg)
{  
	CSUDI_HANDLE hPcm = CSUDI_NULL;
    void * ppvBuffer = NULL;
    unsigned  int  uLength = 0;
    int nBlockNum = 0;
    long nFileAllLength = 0;  //�ļ��ܳ���
    int nFileCurrentPos = 0; //�ļ���ǰλ��
    int nFileLeftLength = 0; //ʣ��δ���ļ�����
    int nInjectedLength = 0; //��ע��Ĵ�С
    int nReadLength = 0;
    void* file = NULL;

	
	ITInjectPCMParam_S * psInjectParam = (ITInjectPCMParam_S*)pvArg;
	
	if(psInjectParam == CSUDI_NULL)
	{
		CSTCPrint("NULL HANDLE \n");
		return;
	}
	
	hPcm = psInjectParam->hInjecter;

    getInjecterFile((const char*)psInjectParam->pcFile, &file, &nFileAllLength);
    
    if(file==NULL || nFileAllLength <= 0)
    {
        return;
    }
    
    while(!g_bInjectPCMFinish)
    {
        //CSTCPrint("pcm_injectTask g_bEnterTask\n\r");
        //�����ǰʣ��δ���ļ��ĳ���
        
        nFileCurrentPos=InjectFSTell(file); //��ǰ�Ѷ��ļ��ĳ���
        nFileLeftLength=nFileAllLength-nFileCurrentPos; //ʣ��δ���ļ��ĳ���
    
        if(nFileLeftLength == 0)
        {
            InjectFSSeek(file,0,INJECT_SEEK_SET);
            nFileLeftLength = nFileAllLength;
        }
    
        uLength = 0;
        CSUDIINJECTERGetFreeBuffer (hPcm,&ppvBuffer,&uLength);
        
        if(uLength != 0)
        {
            nReadLength = PCM_BUFFER_SIZE < nFileLeftLength ? PCM_BUFFER_SIZE:nFileLeftLength;
    
            if(nReadLength < uLength) //ʣ���ļ�����С�ڿ�ע��ռ�ĳ���
            {
                nInjectedLength = nReadLength;
            }
            else  //ʣ���ļ����ȴ��ڵ��ڿ�ע��ռ�ĳ���
            {
                nInjectedLength = uLength;
            }
        
            nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //��ȡ��Ӧ���ȵ��ļ�
            
            CSUDIINJECTERWriteComplete(hPcm,nInjectedLength);       
        }
        else
        {
             CSUDIOSThreadSleep(10);
        }
    }
    
    if(g_bInjectPCMFinish)
    {
        InjectFSClose(file);    
    }
    

}

//����֧��nAudioType���͵�Audio Decoder
//nAudioType�μ�CSUDIAUDStreamType_E, ����ΪEM_UDI_AUD_STREAM_AC3���⴦��
static int searchAudioID_Y_InInject(int nAudioType)
{
	CSUDIAUDStreamType_E  eAudStreamTypeNum=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIAUDIOCapability_S   sAudioCapabilityInfo;
	int nAudioCount = 0;
	int nAudioID = -1;
	int i = 0;
	int j = 0;
	
	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //��ȡ����Ƶ������������
	{
		for (i=0; i<nAudioCount; i++)
		{
			
			if(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo))           //�����ID����Ƶ������������
			{
				if(nAudioType == EM_UDI_AUD_STREAM_AC3)
				{
					for (j=0; j<eAudStreamTypeNum; j++)                                //�����������������棺֧�ֽ����������֧��ֱ�����������
					{
						if (sAudioCapabilityInfo.m_sSupportByPass[j] == EM_UDI_AUD_STREAM_UNKNOWN )
						{
							break;
						}
						if (sAudioCapabilityInfo.m_sSupportByPass[j] == nAudioType)
						{
							nAudioID=i;
							break;
						}
					}
				}
				else
				{
					for (j=0; j<eAudStreamTypeNum; j++)                                //�����������������棺֧�ֽ����������֧��ֱ�����������
					{
						if (sAudioCapabilityInfo.m_sSupportDecode[j] == EM_UDI_AUD_STREAM_UNKNOWN )
						{
                                                 //CSTKPrint("eAudStreamTypeNum=%d\n",sAudioCapabilityInfo.m_sSupportDecode[j]);
							break;
						}
						if (sAudioCapabilityInfo.m_sSupportDecode[j] == nAudioType)
						{
							nAudioID=i;
							break;
						}
					}
				}
				
				if (nAudioID != -1)
					break;
			}
		}
	}	

	return nAudioID;
}




//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@CASEGROUP: 3��OSG
//@DESCRIPTION:���Բ���MPEG2��Ƶ��ͬʱ����PCM��Ƶ����
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��ƽ̨֧��PCM����ע��
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2_TEST_PCM.wav
//@EXECUTIONFLOW:�����ܹ�������������Ƶ
//@EXECUTIONFLOW: 1������CSUDIINJECTEROpen����һ��PCMע���������������ɹ�
//@EXECUTIONFLOW: 2������CSUDIINJECTERSetAttribute����pcm�������ԣ��������óɹ�
//@EXECUTIONFLOW: 3������CSUDIINJECTERStart��ʼע��PCM���ݣ�����ע��ɹ�
//@EXECUTIONFLOW: 4������CSUDIPLAYEROpen����һ������PCM�����������������ɹ�
//@EXECUTIONFLOW: 5������CSUDIPLAYERSetStream���ý�Ŀ��ϢΪPCM��Ϣ
//@EXECUTIONFLOW: 6������CSUDIPLAYERStart��ʼ����PCM,��������CSUDI_SUCCESS�����Ҳ�����Ƶ
//@EXECUTIONFLOW: 7������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 8������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 9������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 10������CSUDIPLAYERStart��ʼ������Ƶ��PCM��Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 11�������ܹ�������������Ƶ
//@EXECUTIONFLOW: 12������CSUDIPLAYERModifyStream������Ƶ��ϢΪEM_IT_MPEG2_NTSC���������óɹ�
//@EXECUTIONFLOW: 13�������ܹ�������������Ƶ
//@EXECUTIONFLOW: 14������ע�����Ͳ�����
CSUDI_BOOL CSTC_ITTEST_IT_0003(void)
{
	CSUDI_HANDLE hVideoPlayer = CSUDI_NULL;
	CSUDI_HANDLE hPcmPlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectPCMThread = CSUDI_NULL;
	CSUDIINJECTERChnl_S sInjectChnl;
	CSUDIINJECTEROpenParam_S sInjectOpenParam;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIINJECTERPcmStartParam_S sPcmStartParam;
	ITInjectPCMParam_S  sThreadParam;
	CSUDIStreamInfo_S  sStreamInfo[1];
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	int nAudioDecoderId = -1;
		
	nAudioDecoderId = searchAudioID_Y_InInject(eAudStreamType);

	sInjectChnl.m_nAudioDecoder = nAudioDecoderId; //PCM���ݲ���ֻҪ��Ƶ������
	sInjectChnl.m_nDemux = -1;
	sInjectChnl.m_nVideoDecoder = -1;  

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_AUDIO;  
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_PCM;				

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTEROpen(&sInjectChnl, &sInjectOpenParam, &hINJECTER) && hINJECTER != CSUDI_NULL),
			"step 1 open injecter  failure !"
		);
	sPcmStartParam.m_uBitsSample = g_PCMInfo[0].m_sStartParam.m_uBitsSample;
	sPcmStartParam.m_uChannels = g_PCMInfo[0].m_sStartParam.m_uChannels;
	sPcmStartParam.m_uSampleRate = g_PCMInfo[0].m_sStartParam.m_uSampleRate;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam)),
			"step 2 set PCM_PARAMS attribute  failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN, &g_PCMInfo[0].m_eEndian)),
			"step 2 set PCM_ENDIAN attribute  failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER)),
			"step 3 start injecter  failure !"
		);
	sThreadParam.pcFile = g_PCMInfo[0].m_pcFileName;
	sThreadParam.hInjecter = hINJECTER;
	g_bInjectPCMFinish = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIOSThreadCreate("PcmInjecter",5*17,4*1024,iInjectPCMFile,(void*)&sThreadParam, &hInjectPCMThread),
			"Create Inject thread failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(hInjectPCMThread != CSUDI_NULL),
			"Create Inject thread failure !"
		);
	
	//pcm player
	sPlayerChnl.m_nDemux = -1;
	sPlayerChnl.m_nAudioDecoder = nAudioDecoderId;
	sPlayerChnl.m_nVideoDecoder = -1;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPcmPlayer)) && hPcmPlayer != CSUDI_NULL,
			"step 4 open pcm player failure !"
		);
	sStreamInfo[0].m_nPid = 0; //no used
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_PCM;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPcmPlayer,sStreamInfo, 1, CSUDI_NULL),
			"step 5 set pcm stream failure !"
		);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPcmPlayer), "step 6 start pcm player failure!");
	
	//video player
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IT_iLockTuner(EM_IT_MPEG2_PAL), "step 7 lock singal failure");

	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nAudioDecoder = -1;
	sPlayerChnl.m_nVideoDecoder = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE,&hVideoPlayer)) && hVideoPlayer != CSUDI_NULL,
			"step 8 open video player failure "
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE,hVideoPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_VIDEO),
			"step 9 set video stream failure "
		);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hVideoPlayer), "step 10 start video player failure!");
	CSUDIOSThreadSleep(50);
	
	CSTCPrint("�����ܹ�����������Ƶ����Ƶ������Ƶ���� %s\n", sThreadParam.pcFile);
    CSTCPrint("We expect normal A/V output,and the audio is from PCM file!\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 11 check result failure!");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_FALSE,hVideoPlayer,EM_IT_MPEG2_NTSC,EM_IT_STREAM_VIDEO),
			"step 12 modify video stream failure "
		);
	
	CSTCPrint("�����ܹ�����������Ƶ����Ƶ ������Ƶ���� %s\n", sThreadParam.pcFile);
    CSTCPrint("We expect normal A/V output,and the audio is from PCM file!\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 13 check result failure!");
	
	CSTK_FATAL_POINT
		
	g_bInjectPCMFinish = CSUDI_TRUE;    //ֹͣע���߳�

	if (hPcmPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hPcmPlayer);
		hPcmPlayer=CSUDI_NULL;
	}
	
	if (hVideoPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hVideoPlayer);
		hVideoPlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}
	if (hInjectPCMThread != CSUDI_NULL)
	{
		CSUDIOSThreadDestroy(hInjectPCMThread);
		hInjectPCMThread = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@CASEGROUP: 3��OSG
//@DESCRIPTION:����MPEG2����Ƶ��ͬʱ����JEPGͼƬ
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧�ֽ���JPEGͼƬ
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.jpg
//@EXPECTATION:�����ܹ�������������Ƶ��ͬʱ����ʾJPEGͼƬ
//@EXECUTIONFLOW: 1������CSUDIOSGDecodeImage����ָ��JPEGͼƬUDI2OSG_720_576.jpg������ģʽΪEM_UDIOSG_SCALE_FULLSIZE����������ɹ�
//@EXECUTIONFLOW: 2������CSUDIOSGBlit��JPEGͼƬ��ʾ���Դ���������������ܹ�����ͼƬUDI2OSG_720_576.jpg
//@EXECUTIONFLOW: 3������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 4������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 5������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 6������CSUDIPLAYERStart��ʼ��������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7�����������ܿ���UDI2OSG_720_576.jpgͼƬ�����ܹ�������������Ƶ
//@EXECUTIONFLOW: 8������CSUDIPLAYERModifyStream������Ƶ��ϢΪEM_IT_MPEG2_NTSC���������óɹ�
//@EXECUTIONFLOW: 9�����������ܿ���UDI2OSG_720_576.jpgͼƬ�����ܹ�������������Ƶ
//@EXECUTIONFLOW: 10���ͷ�ͼƬ��Դ������surface������ע���������ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0004(void)
{
	if(IT_iIsSupportDecImage(EM_IT_DECODE_JPG))
	{
		CSUDI_HANDLE hPlayer = CSUDI_NULL;
		CSUDI_HANDLE hJPEGSurface = CSUDI_NULL;
		CSUDI_HANDLE hDisplay = CSUDI_NULL;
		CSUDIOSGRect_S sShowRect;
		CSUDIOSGSurfaceInfo_S sSurInfo;
		unsigned char * pcBuff = CSUDI_NULL;
		unsigned int nImageLen = 0;
		int nVideoIndex = 0;
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == IT_iTestDecImageWithScaleMode(EM_IT_DECODE_JPG,EM_IT_JPG_720_576,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hJPEGSurface,&pcBuff,&nImageLen)),
				"step 1 decode jpeg image failure !"
			);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hJPEGSurface, &sSurInfo)),
				"get surface info failure !"
			);

		//��Ļ����
		sShowRect.m_nX = 60;
		sShowRect.m_nY = 48;
		sShowRect.m_nWidth = 600;
		sShowRect.m_nHeight = 480;

		if (!IT_iIsShareHDDisplaySurface())
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDisplay)),
					"get display failure !"
				);
		}
		else
		{
			IT_iOSGRect_SDTransformHD(&sShowRect);
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisplay)),
					"get display failure !"
				);
		}

		nVideoIndex = IT_iPlayTV(&hPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
		
		CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay, &sShowRect, hJPEGSurface, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
				"Blit jpeg failure !"
			);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"sync  failure !"
			);
		
		CSTCPrint("�������ӱ�������ܿ���JPEGͼƬUDI2OSG_720_576.jpg�����ܹ�������������Ƶ \n");
        CSTCPrint("We expect the JPEG UDI2OSG_720_576.jpg picture on the SD screen and get normal A/V output\n");
        CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 8 check result failure!");

		CSTK_ASSERT_TRUE_FATAL(
				CSUDI_TRUE  == IT_iSetStream(CSUDI_FALSE,hPlayer,EM_IT_MPEG2_NTSC,EM_IT_STREAM_AUDIO),
				"step 9 set stream failure "
			);

		CSTCPrint("�������ӱ�������ܿ���JPEGͼƬUDI2OSG_720_576.jpg�����ܹ�������������Ƶ \n");    
        CSTCPrint("We expect the JPEG UDI2OSG_720_576.jpg picture on the SD screen and get normal A/V output\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 10 check result failure!");
		
		CSTK_FATAL_POINT
		
		if (hPlayer != CSUDI_NULL)
		{
			CSUDIPLAYERClose (hPlayer);
			hPlayer = CSUDI_NULL;
		}
		if (hJPEGSurface != CSUDI_NULL)
		{
			CSUDIOSGFill(hDisplay, CSUDI_NULL, 0);
			CSUDIOSGSync();
			CSUDIOSGDestroySurface(hJPEGSurface);
			hJPEGSurface = CSUDI_NULL;
		}
		if(pcBuff != CSUDI_NULL)
		{
			CSUDIOSFree(pcBuff);
			pcBuff = CSUDI_NULL;
		}	

		return CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("Not support !\n");
		return CSUDI_TRUE;
	}
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@DESCRIPTION:���Բ���MPEG2��Ƶ��ͬʱ����I֡ͼƬ
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧��I֡ͼƬע��
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:�����ܹ�������������Ƶ��(û�п��е���Ƶ����������I֡������������CSUDIOSG_ERROR_DEVICE_BUSY)
//@EXECUTIONFLOW: 1������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIPLAYEROpen����һ��������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStart��ʼ������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5�������ܹ�����������Ƶ
//@EXECUTIONFLOW: 6�����²��貥��I֡������7-12���ʧ������ת����13
//@EXECUTIONFLOW: 7������IT_FindVideoDecoderѰ����Ƶ���������ɹ��򷵻���Ƶ�������豸������
//@EXECUTIONFLOW: 8������CSUDIINJECTEROpen����һ��ESע���������������ɹ�
//@EXECUTIONFLOW: 9������CSUDIINJECTERStart��ʼע��I֡������ע��ɹ�
//@EXECUTIONFLOW: 10������CSUDIPLAYEROpen����һ������I֡�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 11������CSUDIPLAYERSetStream���ý�Ŀ��ϢΪI֡��Ϣ
//@EXECUTIONFLOW: 12������CSUDIPLAYERStart��ʼ����I֡,��������CSUDI_SUCCESS�����Ҳ���I֡
//@EXECUTIONFLOW: 13���粥��I֡�ɹ��������������ܿ���I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ���粥��I֡ʧ�ܣ��������ܹ�����������Ƶ����Ƶ����I֡ͼƬUDI2OSG_720_576.mpg��ʾ
//@EXECUTIONFLOW: 14���ͷ�ͼƬ��Դ������surface������ע���������ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0005(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDI_HANDLE hIFramePlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectIFrameThread = CSUDI_NULL;
	unsigned char * pcBuff = CSUDI_NULL;
	CSUDI_BOOL bCheckIframe = CSUDI_FALSE;
	int nVideoIndex = 0;

	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");

	CSTCPrint("�����ܹ�����������Ƶ\n");
	CSTCPrint("We expect normal A output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 5 check result failure!");

	bCheckIframe = IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);
	if(bCheckIframe)
	{
		CSTCPrint("���������ܿ���I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ \n");
		CSTCPrint("If could,We expect IFrame UDI2OSG_720_576.mpg on screen for a while and then get normal A/V output..\n");
	}
	else
	{
		CSTCPrint("�����ܹ�����������Ƶ����Ƶ����I֡ͼƬUDI2OSG_720_576.mpg��ʾ\n");
		CSTCPrint("NOTE:In this testcase we probably could not get IFRAME on screen,that's OK!\n");
	}

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 13 check result failure!");

	CSTK_FATAL_POINT

	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer=CSUDI_NULL;
	}
	if (hIFramePlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hIFramePlayer);
		hIFramePlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}
	if (hInjectIFrameThread != CSUDI_NULL)
	{
		CSUDIOSThreadDestroy(hInjectIFrameThread);
		hInjectIFrameThread = CSUDI_NULL;
	}
	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2IT]CSUDIOSFree success !! \r\n");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@DESCRIPTION:����ʵʱ������С����I֡��ȫ��I֡�л�
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧��I֡ͼƬע��
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:�����ܹ�����������Ƶ��С����I֡��ȫ��I֡�л������������ȷ���������
//@EXECUTIONFLOW: 1������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIPLAYEROpen����һ��������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStart��ʼ������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������IT_FindVideoDecoderѰ����Ƶ���������ɹ��򷵻���Ƶ�������豸������
//@EXECUTIONFLOW: 6������CSUDIINJECTEROpen����һ��ESע���������������ɹ�
//@EXECUTIONFLOW: 7������CSUDIINJECTERStart��ʼע��I֡������ע��ɹ�
//@EXECUTIONFLOW: 8������CSUDIPLAYEROpen����һ������I֡�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 9������CSUDIPLAYERSetStream���ý�Ŀ��ϢΪI֡��Ϣ
//@EXECUTIONFLOW: 10������CSUDIPLAYERStart��ʼ����I֡,��������CSUDI_SUCCESS�����Ҳ���I֡
//@EXECUTIONFLOW: 11�����������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ 
//@EXECUTIONFLOW: 12������CSUDIVIDEOSetWindowSize������������С���ڲ���,��������Ļ����,�����Ϊ200
//@EXECUTIONFLOW: 13���������������ܿ���С����I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ 
//@EXECUTIONFLOW: 14������CSUDIVIDEOSetWindowSize��������ȫ������
//@EXECUTIONFLOW: 15�����������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ 
//@EXECUTIONFLOW: 16���ͷ�ͼƬ��Դ������surface�����ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0006(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDI_HANDLE hIFramePlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectIFrameThread = CSUDI_NULL;		
	unsigned char * pcBuff = CSUDI_NULL;
	CSUDIWinRect_S stRect = ITTEST_SMALLWINDOW_RECT;
	int nVideoIndex = 0;
	
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");
	
	CSTCPrint("�����ܹ�����������Ƶ(û����Ƶ)\n");
    CSTCPrint("We expect normal audio output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 4 check result failure!");

	IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);

	CSTCPrint("���������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the full screen and to get normal audio output\n");
    CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 11 check result failure!");
	
	//�߱���ͬԴֻ�ڸ������������
	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iWinRect_SDTransformHD(&stRect);
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, &stRect);		
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, &stRect);		
	}
	CSTCPrint("�������ӱ�����������ܿ���С����I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg A/V output on the screen in a small size window and to get normal audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 13 check result failure!");

	if (IT_iIsShareHDDisplaySurface())
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);		
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);			
	}
	
	CSTCPrint("���������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the full screen and to get normal audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 14 check result failure!");
       
	CSTK_FATAL_POINT;

	if (IT_iIsShareHDDisplaySurface())
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);		
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);			
	}
	
	IT_iClearScreen();
	
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer=CSUDI_NULL;
	}
	if (hIFramePlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hIFramePlayer);
		hIFramePlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
            CSUDIINJECTERClose (hINJECTER);
            hINJECTER=CSUDI_NULL;
	}
    if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2IT]CSUDIOSFree success !! \r\n");
	}
	return CSUDI_TRUE;		
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@DESCRIPTION:����I֡��ʵʱ�����л�,С����I֡��ȫ��I֡�л�
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧��I֡ͼƬע��
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:�����ܹ�������������Ƶ��С����I֡��ȫ��I֡�л������������ȷ���������
//@EXECUTIONFLOW: 1������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStart��ʼ��������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5�������ܹ�������������Ƶ
//@EXECUTIONFLOW: 6��ֹͣ��Ŀ����
//@EXECUTIONFLOW: 7������IT_FindVideoDecoderѰ����Ƶ���������ɹ��򷵻���Ƶ�������豸������
//@EXECUTIONFLOW: 8������CSUDIINJECTEROpen����һ��ESע���������������ɹ�
//@EXECUTIONFLOW: 9������CSUDIINJECTERStart��ʼע��I֡������ע��ɹ�
//@EXECUTIONFLOW: 10������CSUDIPLAYEROpen����һ������I֡�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 11������CSUDIPLAYERSetStream���ý�Ŀ��ϢΪI֡��Ϣ
//@EXECUTIONFLOW: 12������CSUDIPLAYERStart��ʼ����I֡,��������CSUDI_SUCCESS�����Ҳ���I֡
//@EXECUTIONFLOW: 13�����������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 14������CSUDIVIDEOSetWindowSize��������С���ڲ���
//@EXECUTIONFLOW: 15�����������ܿ���С��ĻI֡ͼƬUDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 16������CSUDIVIDEOSetWindowSize��������ȫ������
//@EXECUTIONFLOW: 17�����������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 18������CSUDIVIDEOSetWindowSize��������С���ڲ���
//@EXECUTIONFLOW: 19�����������ܿ���С��ĻI֡ͼƬUDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 20������surface������ע���������ٲ�����
//@EXECUTIONFLOW: 21������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 22������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 23������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 24������CSUDIPLAYERStart��ʼ��������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 25�������ܹ�����С��Ļ��������Ƶ
//@EXECUTIONFLOW: 26������CSUDIVIDEOSetWindowSize��������ȫ������
//@EXECUTIONFLOW: 27�������ܹ�����ȫ����������Ƶ
//@EXECUTIONFLOW: 28���ͷ�ͼƬ��Դ������surface������ע���������ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0007(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDI_HANDLE hIFramePlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectIFrameThread = CSUDI_NULL;		
	unsigned char * pcBuff = CSUDI_NULL;
	CSUDIWinRect_S stRect = ITTEST_SMALLWINDOW_RECT;
	int nVideoIndex = 0;

	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");
	
	CSTCPrint("�����ܹ�����������Ƶ����Ƶ\n");
    	CSTCPrint("We expect normal A/V output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 5 check result failure!");
	
	if (hAVPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hAVPlayer), "step 6 close AVPlayer failure!");
		hAVPlayer = CSUDI_NULL;
	}

	IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);

	CSTCPrint("���������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the full screen\n");
    CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 13 check result failure!");

	//�߱���ͬԴֻ�ڸ������������
	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iWinRect_SDTransformHD(&stRect);
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, &stRect);		
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, &stRect);		
	}
	
	CSTCPrint("�������ӱ�������ܿ���С����I֡ͼƬUDI2OSG_720_576.mpg \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg A/V output on the screen in a small size window \n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 15 check result failure!");

	//�߱���ͬԴֻ�ڸ������������
	if (IT_iIsShareHDDisplaySurface())
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);	
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);		
	}
		
	CSTCPrint("���������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the full screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 17 check result failure!");

	//�߱���ͬԴֻ�ڸ������������
	if (IT_iIsShareHDDisplaySurface())
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, &stRect);		
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, &stRect);		
	}
		
	CSTCPrint("�������ӱ�������ܿ���С����I֡ͼƬUDI2OSG_720_576.mpg \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg A/V output on the screen in a small size window \n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 19 check result failure!");

	if (hAVPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS==CSUDIPLAYERClose (hAVPlayer), "step 20 close AVPlayer failure!");
		hAVPlayer=CSUDI_NULL;
	}
	if (hIFramePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS==CSUDIPLAYERClose (hIFramePlayer), "step 20 close IframePlayer failure!");
		hIFramePlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS==CSUDIINJECTERClose(hINJECTER), "step 20 close IframeInjecter failure!");
		hINJECTER = CSUDI_NULL;
	}
	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
	}
	if (hInjectIFrameThread != CSUDI_NULL)
	{
		CSUDIOSThreadDestroy(hInjectIFrameThread);
		hInjectIFrameThread = CSUDI_NULL;
	}

	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");

	CSTCPrint("�������ӱ����������С��Ļ�ܹ�����������Ƶ������Ƶ����\n");
    CSTCPrint("We expect normal A/V output on the screen in a small size window \n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 25 check result failure!");		

	//�߱���ͬԴֻ�ڸ������������
	if (IT_iIsShareHDDisplaySurface())
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);	
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);		
	}
	
	CSTCPrint("�����ܹ�ȫ��Ļ������Ƶ����Ƶ\n");
    CSTCPrint("We expect normal A/V output on the screen in full size \n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 27 check result failure!");

	CSTK_FATAL_POINT
	
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer=CSUDI_NULL;
	}
	if (hIFramePlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hIFramePlayer);
		hIFramePlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}
	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
	}
	if (hInjectIFrameThread != CSUDI_NULL)
	{
		CSUDIOSThreadDestroy(hInjectIFrameThread);
		hInjectIFrameThread = CSUDI_NULL;
	}	
	return CSUDI_TRUE;	
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��OSG
//@DESCRIPTION:���Խ�Ŀ����ʱ,����OSD͸����
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��EM_IT_MPEG2_PAL
//@EXPECTATION:�����ܹ�������������Ƶ��ͬʱ������CSUDISCREENSetOSDTransparency���Կ���OSD͸���ȵı仯
//@EXECUTIONFLOW: 1������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStart��ʼ��������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5�����������ܹ�������������Ƶ
//@EXECUTIONFLOW: 6������CSUDIOSGGetDisplaySurface��ȡ��ʾsurface
//@EXECUTIONFLOW: 7������CSUDIOSGFill����Ļ�л�һ����ɫ����
//@EXECUTIONFLOW: 8������CSUDISCREENSetOSDTransparency����͸����Ϊ100
//@EXECUTIONFLOW: 9���������Կ�����Ļ����һ����͸���ĺ��,��������Ƶ��������
//@EXECUTIONFLOW: 10������CSUDISCREENSetOSDTransparency����͸����Ϊ50
//@EXECUTIONFLOW: 11���������Կ�����Ļ����һ����͸���ĺ��,��������Ƶ��������
//@EXECUTIONFLOW: 12������CSUDISCREENSetOSDTransparency,����OSD͸����Ϊ0
//@EXECUTIONFLOW: 13����������Ƶ��������,������Ļ��ֻ��ʵʱ��Ŀ,û�к�ɫ����
//@EXECUTIONFLOW: 14������CSUDISCREENSetOSDTranspare
CSUDI_BOOL CSTC_ITTEST_IT_0008(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDIOSGRect_S asDRect  =  ITTEST_SMALLWINDOW_RECT;
	int nVideoIndex = 0;
	
	//avplay
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");

	CSTCPrint("�����ܹ�������������Ƶ\n");
	CSTCPrint("We expect normal A/V output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "Check result failure!");
	
	//�����
	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iOSGRect_SDTransformHD(&asDRect);
	}
	IT_iOSGFill(&asDRect, ITTEST_COLOR_RED_32BIT);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

	CSUDISCREENSetOSDTransparency(100);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTCPrint("���������ܿ���һ����͸���ĺ�飬���ܹ�������������Ƶ \n");
	CSTCPrint("We expect normal A/V output and a small red OSD square which is not transparent in the middle screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSUDISCREENSetOSDTransparency(50);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");	

	CSTCPrint("���������ܿ���һ����͸���ĺ��(�������Ƶͼ�����)�����ܹ�������������Ƶ \n");
	CSTCPrint("We expect normal A/V output and a small red OSD square which is half transparent in the middle of screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");
		
	CSUDISCREENSetOSDTransparency(0);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");	

	CSTCPrint("��������Ƶ��������,������Ļ��ֻ��ʵʱ��Ŀ,û�к�ɫ����\n");
	CSTCPrint("We expect normal A/V output and no red OSD square on screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");	

	CSTK_FATAL_POINT;

	CSUDISCREENSetOSDTransparency(100);	
	IT_iClearScreen();
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");	
	
	if (hAVPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hAVPlayer), "close av player failure !");
		hAVPlayer=CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��OSG
//@DESCRIPTION:���Խ�Ŀ����ʱ,OSDȫ��͸������ֲ����ص�͸���ȵĹ�ϵ
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��EM_IT_MPEG2_PAL
//@EXPECTATION:CSUDISCREENSetOSDTransparency�ӿ����õ�ȫ����ֲ����ص��͸���Ƚӿ�֮�����Ϲ�ϵ��ز�����������ȫ��͸����Ϊ��͸���������ص�͸����Ϊȫ͸������ʱ��Ч��ӦΪȫ͸����
//@EXECUTIONFLOW:1������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW:4������CSUDIPLAYERStart��ʼ��������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5����������Ƶ��������
//@EXECUTIONFLOW:6������CSUDISCREENSetOSDTransparency����͸����Ϊ100
//@EXECUTIONFLOW:7������CSUDIOSGGetDisplaySurface��ȡ��ʾSurface
//@EXECUTIONFLOW:8������CSUDIOSGFill��������
//@EXECUTIONFLOW:9������CSUDIOSGFill��һ����ɫ��͸������{160,88,400,400}
//@EXECUTIONFLOW:10������CSUDIOSGFill��һ����ɫ��͸������{210,138,300,300}
//@EXECUTIONFLOW:11������CSUDIOSGFill��һ����ɫ͸������{260,188,200,200}
//@EXECUTIONFLOW:12������CSUDIOSGSync
//@EXECUTIONFLOW:13����������Ƶ��������,��ʾ�ⲿ��ɫ��͸������,�м���ɫ��͸������,�м�ȫ͸��
//@EXECUTIONFLOW:14������CSUDISCREENSetOSDTransparency,����OSD͸����Ϊ50
//@EXECUTIONFLOW:15����������Ƶ��������,��ʾ�ⲿ��ɫ��͸��������м���ɫ��͸������ȫΪ��͸��,�м�ȫ͸��
//@EXECUTIONFLOW:16������CSUDISCREENSetOSDTransparency,����OSD͸����Ϊ0
//@EXECUTIONFLOW:17����������Ƶ��������,û��OSD��ʾ
//@EXECUTIONFLOW:18������CSUDISCREENSetOSDTransparency,����OSD͸����Ϊ100
//@EXECUTIONFLOW:19������,���ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0009(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDIOSGRect_S asOutRect  = ITTEST_OUT_RECT;
	CSUDIOSGRect_S asCenterRect  = ITTEST_CENTER_RECT;
	CSUDIOSGRect_S asInRect  = ITTEST_IN_RECT;
	int nVideoIndex = 0;
	
	//avPlayer
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");

	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iOSGRect_SDTransformHD(&asOutRect);
		IT_iOSGRect_SDTransformHD(&asCenterRect);
		IT_iOSGRect_SDTransformHD(&asInRect);
	}

	//��ɫ��͸������
	IT_iOSGFill(&asOutRect, ITTEST_COLOR_RED_32BIT);	
	//��ɫ͸������
	IT_iOSGFill( &asCenterRect, ITTEST_COLOR_GREEN_32BIT);
	//��ɫ͸������
	IT_iOSGFill(  &asInRect, ITTEST_COLOR_BLUE_32BIT);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

	CSTCPrint("1.��������Ƶ������������\n");
	CSTCPrint("2.�����ⲿ��ɫ����͸������ɫ�����п�������Ƶ����\n");
	CSTCPrint("3.�����м���ɫ�����͸�������Կ�����ɫ��������Ƶͼ�����\n");
	CSTCPrint("4.����������ķ�����ȫ͸�� ����������������Ƶͼ��\n");
	CSTCPrint("1.We expect normal A/V output\n");
	CSTCPrint("2.We expect the most outside red OSD square not transparent on the screen and can't see video under the red OSD square\n");
	CSTCPrint("3.We expect the middle green OSD square half transparent on the screen and can see video overlap with  the green OSD square\n");
	CSTCPrint("4.We expect the most inner  OSD square totally transparent on the screen and can see video normally in the middle\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSUDISCREENSetOSDTransparency(50);//����ȫ��
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTCPrint("1.��������Ƶ������������\n");
	CSTCPrint("2.�����ⲿ��ɫ�����͸��\n");
	CSTCPrint("3.�����м���ɫ�����͸��\n");
	CSTCPrint("4.����������ķ�����ȫ͸������������������Ƶͼ��\n");
	CSTCPrint("1.We expect normal A/V output\n");
	CSTCPrint("2.We expect the most outside red OSD square half transparent on the screen and can see video overlap with  the red OSD square\n");
	CSTCPrint("3.We expect the middle green OSD square half transparent on the screen and can see video overlap with  the green OSD square\n");
	CSTCPrint("4.We expect the most inner  OSD square totally transparent on the screen and can see video normally in the middle\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSUDISCREENSetOSDTransparency(0);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTCPrint("��������Ƶ��������,��û��OSD��ʾ\n");
	CSTCPrint("We expect normal A/V output and no OSD output on the screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	CSUDIOSThreadSleep(500);
		
	CSTK_FATAL_POINT;

	IT_iClearScreen();
	CSUDISCREENSetOSDTransparency(100);
	
	if (hAVPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hAVPlayer), "close av player failure !");
		hAVPlayer=CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��Stanby
//@CASEGROUP: 3��Wakeup
//@CASEGROUP: 3��OSG
//@DESCRIPTION:�����ڽ�Ŀ����,��Ŀֹͣ����,��ʾI֡�����Standby��Wakeup�����
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧�ֽ���JPEGͼƬ
//@PRECONDITION:3��֧�ֽ�Ŀ����
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.jpg
//@EXPECTATION:�����ڸ���״̬��,Standby��Wakeup����������.
//@EXECUTIONFLOW: 1������CSUDIOSGDecodeImage����ָ��JPEGͼƬUDI2OSG_720_576.jpg������ģʽΪEM_UDIOSG_SCALE_FULLSIZE����������ɹ�
//@EXECUTIONFLOW: 2������CSUDIOSGBlit��JPEGͼƬ��ʾ���Դ棬�����ܹ�����ͼƬUDI2OSG_720_576.jpg
//@EXECUTIONFLOW: 3������CSUDISYSStandby,�������Խ������״̬
//@EXECUTIONFLOW: 4������10������CSUDISYSStandby,�������Ѵ���״̬,�����Կ���ͼƬ
//@EXECUTIONFLOW: 5������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 6������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 7������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 8������CSUDIPLAYERStart��ʼ��������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9�����������ܿ���UDI2OSG_720_576.jpgͼƬ�����ܹ�������������Ƶ
//@EXECUTIONFLOW: 10������CSUDISYSStandby,�������Խ������״̬
//@EXECUTIONFLOW: 11������10������CSUDISYSStandby,�������Ѵ���״̬,�����Կ���ͼƬ,���ܹ�������������Ƶ
//@EXECUTIONFLOW: 12�����������ܿ���UDI2OSG_720_576.jpgͼƬ�����ܹ�������������Ƶ
//@EXECUTIONFLOW: 13���ͷ�ͼƬ��Դ������surface������ע���������ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0010(void)
{
//��δ��?
/*
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	 CSUDI_BOOL bReal;
		
	//avPlayer
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IT_iLockTuner(EM_IT_MPEG2_PAL), "step 1 lock singal failure");

	sPlayerChnl.m_nDemux = 0;
	sPlayerChnl.m_nAudioDecoder = 0;
	sPlayerChnl.m_nVideoDecoder = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE,&hAVPlayer)) && hAVPlayer != CSUDI_NULL,
			"step 2 open av player failure "
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE,hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO),
			"step 3 set av stream failure "
		);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hAVPlayer), "tart avplayer failure!");

	CSTCPrint("�����ܹ�������������Ƶ\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "heck result failure!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_TRUE,20000,&bReal),"��һ����������ʧ��");		

	if(bReal == CSUDI_FALSE)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_FALSE,20000,&bReal),"���Ĳ��˳��ٴ�����������ʧ��");		
	}	
	else
	{
		CSTCPrint("\n********************���Գɹ�******************************\n");	
	}

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSTK_FATAL_POINT
	if (hAVPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hAVPlayer), "close av player failure !");
		hAVPlayer=CSUDI_NULL;
	}
	*/
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@CASEGROUP: 3��OSG
//@DESCRIPTION:����I֡�ڸ�������µĹ���ʵ��:I֡��ʾ,I֡���л�,I֡+�㲥,I֡+�㲥�л���ʵʱ����Ƶ,ʵʱ����Ƶ�л���I֡+�㲥,I֡+�㲥״̬�´��ڴ�С���л�
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧��I֡ͼƬע��
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:����I֡�ڸ��������������ʾ
//@EXECUTIONFLOW: 1������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIPLAYEROpen����һ��������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStart��ʼ������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSTKP_FOpen,CSTKP_FRead�Ⱥ�����UDI2OSG_720_576.mpgI֡ͼƬ
//@EXECUTIONFLOW: 6������CSUDIINJECTEROpen����һ��ESע���������������ɹ�
//@EXECUTIONFLOW: 7������CSUDIINJECTERSetAttribute����ESע����������,ע��I֡ΪUDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 8������CSUDIINJECTERStart��ʼע��I֡������ע��ɹ�
//@EXECUTIONFLOW: 9������CSUDIPLAYEROpen����һ������I֡�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 10������CSUDIPLAYERAddPlayerCallback�����֡�¼��ص��������ɹ�
//@EXECUTIONFLOW: 11������CSUDIPLAYERSetStream���ý�Ŀ��ϢΪI֡��Ϣ
//@EXECUTIONFLOW: 12������CSUDIPLAYERStart��ʼ����I֡,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 13���������Կ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg��������������Ƶ
//@EXECUTIONFLOW: 14������CSUDIPLAYERClose�ص�������
//@EXECUTIONFLOW: 15������CSUDIINJECTERClose�ص�ע��
//@EXECUTIONFLOW: 16������CSTKP_FOpen,CSTKP_FRead�Ⱥ�����UDI2OSG_720_576_2.mpgI֡ͼƬ
//@EXECUTIONFLOW: 17������CSUDIINJECTEROpen����һ��ESע���������������ɹ�
//@EXECUTIONFLOW: 18������CSUDIINJECTERSetAttribute����ESע����������,ע��I֡ΪUDI2OSG_720_576_2.mpg
//@EXECUTIONFLOW: 19������CSUDIINJECTERStart��ʼע��I֡������ע��ɹ�
//@EXECUTIONFLOW: 20������CSUDIPLAYEROpen����һ������I֡�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 21������CSUDIPLAYERAddPlayerCallback�����֡�¼��ص��������ɹ�
//@EXECUTIONFLOW: 22������CSUDIPLAYERSetStream���ý�Ŀ��ϢΪI֡��Ϣ
//@EXECUTIONFLOW: 23������CSUDIPLAYERStart��ʼ����I֡,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 24���������Կ���ȫ��I֡ͼƬUDI2OSG_720_576_2.mpg��������������Ƶ
//@EXECUTIONFLOW: 25������CSUDIOSGFill����Ļ�м仭һ��200*200�İ�͸����ɫ����
//@EXECUTIONFLOW: 26������CSUDIOSGFill����Ļ�м仭һ��150*150��ȫ͸������
//@EXECUTIONFLOW: 27������CSUDIOSGSync����ͬ��
//@EXECUTIONFLOW: 28������CSUDIVIDEOSetWindowSize������Ƶ���ڴ�С��150*150,λ������Ļ����
//@EXECUTIONFLOW: 29������CSUDIOSGSync����ͬ��
//@EXECUTIONFLOW: 30������I֡��������ʾ����Ļ�м�150*150����,����I֡��ʾ�������͸����ɫ���������ʾ
//@EXECUTIONFLOW: 31������CSUDIVIDEOSetWindowSize������Ƶ����Ϊȫ��
//@EXECUTIONFLOW: 32������I֡����ȫ����ʾ,���ҿ���������Ƶ
//@EXECUTIONFLOW: 33������CSUDIVIDEOSetWindowSize������Ƶ���ڴ�С��150*150,λ������Ļ����
//@EXECUTIONFLOW: 34������I֡��������ʾ����Ļ�м�150*150����,����I֡��ʾ�������͸����ɫ���������ʾ
//@EXECUTIONFLOW: 35������CSUDIPLAYERCloseֹͣI֡����
//@EXECUTIONFLOW: 36������CSUDIINJECTERCloseֹͣע��
//@EXECUTIONFLOW: 37������CSUDIPLAYERClose�ص���Ƶ������
//@EXECUTIONFLOW: 38�����OSD��ʾ,������Ļû�а�͸����ɫ����
//@EXECUTIONFLOW: 39������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 40������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 41������CSUDIPLAYERSetStream���ý�Ŀ����Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 42������CSUDIPLAYERStart��ʼ��������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 43��������������Ļ�м�150*150���򿴵�ʵʱ����Ƶ
//@EXECUTIONFLOW: 44���ͷ�ͼƬ��Դ������surface������ע����,���ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0011(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDI_HANDLE hIFramePlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectIFrameThread = CSUDI_NULL;
	unsigned char * pcBuff = CSUDI_NULL;
	CSUDIOSGRect_S stRect = ITTEST_SMALLWINDOW_RECT;
	CSUDIOSGRect_S stInRect = ITTEST_SMALLWINDOW_IN_RECT;
	CSUDIWinRect_S  stWinRect= ITTEST_SMALLWINDOW_IN_RECT;
	int nVideoIndex = 0;

	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");
	
	IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);

	CSTCPrint("����������Ļ�ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ \n");
	CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the screen and to get normal audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 13 check result failure!");

	if (hIFramePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose (hIFramePlayer), "step 14 close iframe player failure !");
		hIFramePlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "step 15 close injecter failure !");
		hINJECTER = CSUDI_NULL;
	}	
	CSUDIOSThreadSleep(100);
	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2IT]CSUDIOSFree success !! \r\n");
	}
	
	IT_iPlayIFrame(EM_IT_IFRAME_720_576_2,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);
		
	CSTCPrint("����������Ļ�ܿ���ȫ��I֡ͼƬUDI2OSG_720_576_2.mpg�����ܹ�����������Ƶ \n");	
	CSTCPrint("We expect the Iframe UDI2OSG_720_576_2.mpg on the screen and to get normal audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 24 check result failure!");

	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iOSGRect_SDTransformHD(&stRect);
		IT_iOSGRect_SDTransformHD(&stInRect);
		IT_iWinRect_SDTransformHD(&stWinRect);
	}

	//��ɫ��͸������	
	IT_iOSGFill(&stRect, ITTEST_COLOR_RED_TRANSPARENT_32BIT);
	//ȫ͸������
	IT_iOSGFill(&stInRect, ITTEST_COLOR_TRANSPARENT_32BIT);

	if (IT_iIsShareHDDisplaySurface())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, &stWinRect),
			"step 28 set windowsize failure!");	
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, &stWinRect),
			"step 28 set windowsize failure!");	
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"step 29 Sync failure !");
	
	CSTCPrint("1.�����ܹ�����������Ƶ \n");		
	CSTCPrint("2.�������ӱ��������Ļ�����ܿ���С����I֡ͼƬUDI2OSG_720_576_2.mpg�����ܹ�����������Ƶ \n");				
	CSTCPrint("3.������Ļ�м��и���͸����ɫ�߿�\n");				
	CSTCPrint("4.������ɫ�߿����ð�ΧסI֡ͼƬ,��û�е�����ʾ\n");	
	CSTCPrint("1.We expect normal audio output\n");
	CSTCPrint("2.We expect the Iframe UDI2OSG_720_576_2.mpg A/V output with small size in the middle of the screen and get normal audio output\n");
	CSTCPrint("3.We expect half transparent red OSD frame in the middle of screen \n");
	CSTCPrint("4.We expect the red OSD frame surrounds exactly the Iframe and there's no overlap \n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 30 check result failure!");

	if (IT_iIsShareHDDisplaySurface())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, NULL), "step 31 set windowsize failure!");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, NULL), "step 31 set windowsize failure!");
	}

	CSTCPrint("1.�����ܹ�����������Ƶ \n");		
	CSTCPrint("2.����I֡ͼƬUDI2OSG_720_576_2.mpgȫ����ʾ \n");				
	CSTCPrint("3.������Ļ�м��и���͸����ɫ�߿�\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 32 check result failure!");

	if (IT_iIsShareHDDisplaySurface())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS==CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, &stWinRect),
			"step 33 set windowsize failure!");		
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS==CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, &stWinRect),
			"step 33 set windowsize failure!");		
	}

	CSTCPrint("1.�����ܹ�����������Ƶ \n");		
	CSTCPrint("2.�������ӱ��������Ļ�����ܿ���С����I֡ͼƬUDI2OSG_720_576_2.mpg�����ܹ�����������Ƶ \n");				
	CSTCPrint("3.������Ļ�м��и���͸����ɫ�߿�\n");				
	CSTCPrint("4.������ɫ�߿����ð�ΧסI֡ͼƬ,��û�е�����ʾ\n");	
	CSTCPrint("1.We expect normal audio output\n");
	CSTCPrint("2.We expect the Iframe UDI2OSG_720_576_2.mpg A/V output with small size in the middle of the screen and get normal audio output\n");
	CSTCPrint("3.We expect half transparent red OSD frame in the middle of screen \n");
	CSTCPrint("4.We expect the red OSD frame surrounds exactly the Iframe and there's no overlap \n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 34 check result failure!");

	if (hIFramePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hIFramePlayer), "step 35 close iframe player failure !");
		hIFramePlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose(hINJECTER), "step 36 close injecter failure !");
		hINJECTER = CSUDI_NULL;
	}	
	CSUDIOSThreadSleep(100);
	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2IT]CSUDIOSFree success !! \r\n");
	}

	if (hAVPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hAVPlayer), "step 37 close av player failure !");
		hAVPlayer=CSUDI_NULL;
	}

	IT_iClearScreen();

	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");
	
	CSTCPrint("�����ڱ���������ܹ�������Ļ����С����ʵʱ��Ŀ�Ĳ���\n");
    	CSTCPrint("We expect the video with small size A/V output in the middle of the screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 4 check result failure!");

	CSTK_FATAL_POINT;

	IT_iClearScreen();

	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer=CSUDI_NULL;
	}
	if (hIFramePlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hIFramePlayer);
		hIFramePlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
		hINJECTER = CSUDI_NULL;
	}
	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2IT]CSUDIOSFree success !! \r\n");
	}

	if (IT_iIsShareHDDisplaySurface())
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@DESCRIPTION:���Խ���I֡ʱ�ص���Ϣ����ȷ��
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧��I֡ͼƬע��
//@INPUT: 1��UDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 1������IT_FindVideoDecoderѰ����Ƶ���������ɹ��򷵻���Ƶ�������豸������
//@EXECUTIONFLOW: 2������CSUDIINJECTEROpen����һ��ESע���������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIINJECTERStart��ʼע��I֡������ע��ɹ�
//@EXECUTIONFLOW: 4������CSUDIPLAYEROpen����һ������I֡�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 5������CSUDIPLAYERAddPlayerCallback��ӻص��������¼�ΪEM_UDIPLAYER_VIDEO_FRAME_COMING��������Ϣs_nIttestTestdata
//@EXECUTIONFLOW: 6������CSUDIPLAYERSetStream���ý�Ŀ��ϢΪI֡��Ϣ
//@EXECUTIONFLOW: 7������CSUDIPLAYERStart��ʼ����I֡,��������CSUDI_SUCCESS�����Ҳ���I֡
//@EXECUTIONFLOW: 8�����������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 9���ͷ�ͼƬ��Դ������surface������ע���������ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0012(void)
{	
	CSUDI_HANDLE hIFramePlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectIFrameThread = CSUDI_NULL;		
	unsigned char * pcBuff = CSUDI_NULL;
	
	IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);

	CSTCPrint("���������ܿ���ȫ��I֡ͼƬUDI2OSG_720_576.mpg \n");
	CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the full screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 2 check result failure!");

	CSTK_FATAL_POINT
	if (hIFramePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hIFramePlayer), "step 11 close iframe player failure !");
		hIFramePlayer=CSUDI_NULL;
	}
	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose(hINJECTER), "step 11 close injecter failure !");
		hINJECTER = CSUDI_NULL;
	}
	
	if (hInjectIFrameThread != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIOSThreadDestroy(hInjectIFrameThread), "step 11 destroy thread failure !");
		hInjectIFrameThread = CSUDI_NULL;
	}
	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
		CSTCPrint("[UDI2IT]CSUDIOSFree success !! \r\n");
	}
	return CSUDI_TRUE;
}

void Ittest_iCallback_1( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
   	if(pSection == CSUDI_NULL)
	{
		return ;
	}
	CSTCPrint("\n*************** ���뵽������Ϊ%02x\n",pSection->m_pucData[0]);
   	return;
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��INJECT
//@DESCRIPTION:����Ӧ��ͨ��sectionģ��ȥ����һ���Ѿ���avģ����䣨ռ�ã���pidʱ����Ӧ��Ӱ�������Ĳ������̣�
//@PRECONDITION:ƽ̨�ɹ���ʼ��
//@INPUT: EM_IT_MPEG2_PAL
//@EXPECTATION:�����ܹ�������������Ƶ��ͬʱ��������ͬ��pid/channel������filer�����ͷ�filter�����ܳɹ���񣬲�Ӱ���Ŀ����  
//@EXECUTIONFLOW: 1������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStart��ʼ��������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5�������ܹ�������������Ƶ
//@EXECUTIONFLOW: 6��Ϊfliter�������Ŀ��ͬ��pid������CSUDIFILTERAllocate����fliter��������
//@EXECUTIONFLOW: 7������CSUDIFILTERStart��ʼ����
//@EXECUTIONFLOW: 8������CSUDIFILTERFree  ֹͣFilter��������,���ͷ�Filter��Դ
//@EXECUTIONFLOW: 9����������Ƶ�������Ų���Ӱ��
//@EXECUTIONFLOW: 10������CSUDIPLAYERStop ֹͣ���Ž�Ŀ
//@EXECUTIONFLOW: 11������CSUDIPLAYERClose �رղ�����
//@EXECUTIONFLOW: 12���ظ�����2_4���ٴβ��Ž�Ŀ��������������Ƶ�ɹ�
//@EXECUTIONFLOW: 13�����������ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0013(void)
{
	CSUDI_Error_Code nRet;
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	int nUserData = 0x88888888;
	int nVideoIndex = 0;

	memset(&sTmpReqInfo,0,sizeof(sTmpReqInfo));
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");
	
	CSTCPrint("�����ܹ�������������Ƶ\n");
	CSTCPrint("We expect normal A/V output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 1 check result failure!");
	
	sTmpReqInfo.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	sTmpReqInfo.m_nDemuxID = 0;
	sTmpReqInfo.m_nPID = g_sITTestSeviceInfo[EM_IT_MPEG2_PAL].m_nVideoPid;          
	sTmpReqInfo.m_bCRCCheck = CSUDI_TRUE;

	nRet = CSUDIFILTERAllocate(&sTmpReqInfo,Ittest_iCallback_1,&nUserData,&hFilter);
	
	if(hFilter != CSUDI_NULL && nRet == CSUDI_SUCCESS )
	{
		CSTCPrint("Start filter.....\n");
		CSUDIFILTERStart(hFilter);
		
		CSUDIOSThreadSleep(2000);
		
		CSTCPrint("Free filter.....\n");
		CSUDIFILTERFree(hFilter);
	}
	
	CSTCPrint("��������Ƶ��Ȼ��������\n");
	CSTCPrint("We expect normal A/V output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "����Ƶ������������!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "ֹͣ���Ž�Ŀʧ��!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose( hAVPlayer), "�رս�Ŀʧ��!");
	hAVPlayer = CSUDI_NULL;
	CSUDIOSThreadSleep(2000);
	
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");
	
	CSTCPrint("�����ٴβ�������Ƶ�ɹ�\n");
	CSTCPrint("We expect normal A/V output again\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "�ٴβ�������Ƶʧ��!");
		
	CSTK_FATAL_POINT;
		
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer=CSUDI_NULL;
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@DESCRIPTION:������Ƶ����ƫ��
//@PRECONDITION:ƽ̨�ɹ���ʼ��
//@EXECUTIONFLOW: 1������EM_IT_COORDINATE_SHIFTָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIPLAYEROpen����һ����������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_COORDINATE_SHIFTָ����Ϣ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStart��ʼ��������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5�������ܹ�������������Ƶ
//@EXECUTIONFLOW: 6�����������ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0014(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int nVideoIndex = 0;
	
 	CSTCPrint("CSTC_ITTEST_IT_00014:��Ƶ����ƫ�Ʋ���\n");

	nVideoIndex = IT_iPlayTV(&hPlayer,EM_IT_COORDINATE_SHIFT,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");
	
	CSTCPrint("�����Ļ�۲���Ƶ���½�����Ƿ���ʾ�����ҶԳƣ����Ұױ��Ƿ���ʾ�����ҶԳ�\n");
	CSTCPrint("We expect normal A/V output,care about integrity and symmetry!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"��Ƶ���겻Ӧ��ƫ�Ƶ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "CSUDIPLAYERStopʧ��");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "CSUDIPLAYERCloseʧ��");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@DESCRIPTION:���Խ�Ŀ����ʱ�¼�ΪEM_UDIPLAYER_VIDEO_FRAME_COMING �Ļص��������
//@PRECONDITION:ƽ̨�ɹ���ʼ��
//@INPUT:1��hPlayer��Ч
//@EXPECTATION:CSUDI_SUCCESS
//@EXECUTIONFLOW:1������IT_iLockTuner����ָ����Ƶ��
//@EXECUTIONFLOW:2��������Ƶ�����������
//@EXECUTIONFLOW:3����һ��ֱ�����������õ����������
//@EXECUTIONFLOW:4������IT_iSetStream,������������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIPLAYERAddPlayerCallback��ӻص��������¼�ΪEM_UDIPLAYER_VIDEO_FRAME_COMING��������Ϣs_nLiveTestdata
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����������������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���жϻص���Ϣ�Ƿ���ȷ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStop��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�2~8
//@EXECUTIONFLOW:10���ָ��ֳ�	
CSUDI_BOOL CSTC_ITTEST_IT_0015(void)
{
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDI_HANDLE   hAVPlayer;
	CSUDIPlayerEventType_E  eEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING;
	int nUserData = 0x66666666;
	int i = 0;
	int j = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IT_iLockTuner(EM_IT_MPEG2_NTSC), "��Ƶʧ��!");
	for(i = 0;i < 2; i++)
	{
		s_nIttestTestdata = 0;
	  	sPlayerChnl.m_nDemux = 0;
		sPlayerChnl.m_nAudioDecoder = 0;
		sPlayerChnl.m_nVideoDecoder = 0;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, FALSE),"�ر���Ƶʧ��!");
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE, &hAVPlayer)) && hAVPlayer != CSUDI_NULL,
			"�򿪲�����ʧ��! "
			);
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE, hAVPlayer,EM_IT_MPEG2_NTSC, EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO),
			"���ý�Ŀ��ʧ��! "
			);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hAVPlayer,fnIttestCallback1,eEvent,&nUserData), "ע��ص�ʧ��!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hAVPlayer), "��ʼ���Ž�Ŀʧ��!");
		for(j = 0;j < 10 ;j ++)
		{
			if(s_nIttestTestdata ==1001)
			break;
	    		CSUDIOSThreadSleep(2000);
		}
		CSTK_ASSERT_TRUE_FATAL(s_nIttestTestdata == 1001, "�ص���Ϣ����!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "ֹͣ���Ž�Ŀʧ��!");
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

static CSUDI_BOOL IT_iGetPhotoToDisplay(CSUDI_HANDLE *phSurface,
	unsigned int unWidth,unsigned int unHeight,
	char *pPhotoFile)
{
	unsigned char *pPhoto = NULL;
	int i=0;
	CSUDIOSGSurfaceInfo_S stInfo;
	unsigned char *pPixel,*pSerach;
	CSUDI_BOOL bRet = FALSE;
	
	IT_iGetPhoto(pPhotoFile,&pPhoto);
	CSTK_ASSERT_TRUE_FATAL(pPhoto != NULL,"fail to get photo\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGCreateSurface(EM_UDIOSG_PIXEL_FORMAT_ARGB_8888,unWidth,unHeight,phSurface),
		"fail to create OSG surface\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(*phSurface,&stInfo),"fail to get surface information\n");
	pPixel = (unsigned char*)stInfo.m_pvSurfaceMem;
	pSerach = pPhoto+54;//4//54B is bmp header
	for(i=unHeight-1;i>=0;i--)
	{
		memcpy(pPixel,pSerach+i*unWidth*4,unWidth*4);
		pPixel+=stInfo.m_nPitch;
	}
	bRet = TRUE;
	
	CSTK_FATAL_POINT
	if (pPhoto)
	{
		CSUDIOSFree(pPhoto);
		pPhoto = CSUDI_NULL;
	}
	g_bAssertBeforeFatalPoint = CSUDI_TRUE;
	return bRet;
}

struct IT_ResolutionInfo{
	CSUDI_HANDLE m_hSurfaceUser;
	CSUDIWinRect_S m_stWinRectSD;
	CSUDIWinRect_S m_stWinRectHD;
};

static void IT_iScreenCB(CSUDISCREENEvent_E eEvt, CSUDISCREENType_E eScreenDevice,void * pvUserData)
{
	CSUDI_HANDLE hSurface;
	CSUDISCREENResolution_E	enRes;
	struct IT_ResolutionInfo *pstInfo=(struct IT_ResolutionInfo *)pvUserData;
	CSUDIWinRect_S stRect;
	
	if (eEvt == EM_UDISCREEN_RESOLUTION_CHANGED)
	{
		if (eScreenDevice & EM_UDI_VOUT_DEVICE_SD)
		{
			CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD,&enRes);
			memcpy(&stRect,&pstInfo->m_stWinRectSD,sizeof(stRect));
			if (enRes == EM_UDISCREEN_RESOLUTION_NTSC)
			{
				stRect.m_nY = pstInfo->m_stWinRectSD.m_nY*480/576;
				stRect.m_nHeight = pstInfo->m_stWinRectSD.m_nHeight*480/576;
			}
			CSUDIOSGGetDisplaySurface(0,&hSurface);
			CSUDIOSGBlit(hSurface,NULL,pstInfo->m_hSurfaceUser,NULL,EM_UDIOSG_OPMODE_COPY);
			CSUDIOSGSync();
			CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_SD,&stRect);
		}
		if (eScreenDevice & EM_UDI_VOUT_DEVICE_HD)
		{
			CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD,&enRes);
			memcpy(&stRect,&pstInfo->m_stWinRectHD,sizeof(stRect));
			if (enRes >= EM_UDISCREEN_RESOLUTION_1080I && enRes <= EM_UDISCREEN_RESOLUTION_1080P_50HZ)
			{
				stRect.m_nX = pstInfo->m_stWinRectHD.m_nX*1920/1280;
				stRect.m_nWidth = pstInfo->m_stWinRectHD.m_nWidth*1920/1280;
				stRect.m_nY = pstInfo->m_stWinRectHD.m_nY*1080/720;
				stRect.m_nHeight = pstInfo->m_stWinRectHD.m_nHeight*1080/720;
			}
			CSUDIOSGGetDisplaySurface(1,&hSurface);
			CSUDIOSGBlit(hSurface,NULL,pstInfo->m_hSurfaceUser,NULL,EM_UDIOSG_OPMODE_COPY);
			CSUDIOSGSync();
			CSUDIVIDEOSetWindowSize(0,EM_UDI_VOUT_DEVICE_HD,&stRect);
		}
	}
}

static void IT_iDumpMsg(BOOL bIsHD)
{
	CSTCPrint("����%s �ֱ���ʱ�������û�п���,�������쳣(Y/N)",bIsHD?"����":"����");
	CSTCPrint("OSG�ܹ��Զ�����,OSGͼ����ȫ����������ʾ����Ļ(Y/N)\n");
	CSTCPrint("OSG û�ж��ѡ�ģ�����쳣(Y/N)\n");
	CSTCPrint("��ƵС��������?(Y/N)\n");
	CSTCPrint("We expect normal A/V output in small windown on the screen\n");
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��OSG
//@DESCRIPTION:�ֱ����л���OSG��video����ϲ���
//@PRECONDITION:ƽ̨�ɹ���ʼ��
//@INPUT:1��OSG��С������Ƶ
//@EXPECTATION:1.�ֱ����л��������С������Ƶ�쳣
//@EXPECTATION:2.�ֱ����л�ʱ,OSGͼ���������쳣
//@EXPECTATION:3.�ֱ����л�ʱ��Ƶ����������˸
//@EXPECTATION:4.�ֱ����л�ʱ�ޱ������쳣
//@REMARK:ֻ��ƽ̨֧�ָ���ʱ�Ž��и���������ԣ��Ƿ�֧�ָ����������ļ�CS_OSGUDI2_SURPORT_HD_SD_DISPLAY�ֶ�ָ��
//@EXECUTIONFLOW:1������IT_iLockTuner����ָ��Player_PAL_NTSC��Ŀ��Ƶ��
//@EXECUTIONFLOW:2������CSUDIVIDEOShow����Ƶ�����
//@EXECUTIONFLOW:3����һ��ֱ��������������Player_PAL_NTSC�����е�PAL�ƽ�Ŀ
//@EXECUTIONFLOW:4�����ø߱������С����
//@EXECUTIONFLOW:5�����ļ�UDI2OSG_search.bmp�������ļ��е�ARGB8888 ���ݿ�����������1280*720��surface��
//@EXECUTIONFLOW:6������CSUDIOSGGetDisplaySurface��ȡ�߱�����ִ���
//@EXECUTIONFLOW:7������ CSUDIOSGBlit������UDI2OSG_search.bmp���ݵ�surface����blit���߱����Դ���
//@EXECUTIONFLOW:8������CSUDIOSGFill�ڸ߱��������ƵС���ڴ����߱����Դ���Ӧ��������͸��ɫ0x0
//@EXECUTIONFLOW:9������CSUDISCREENAddCallbackע��ֱ����л��ص�����,�ûص�������������
//@EXECUTIONFLOW:9.1   ����CSUDISCREENGetResolution��ȡ�л���ķֱ���
//@EXECUTIONFLOW:9.2   �����л���ķֱ��ʼ�����ƵС����λ��
//@EXECUTIONFLOW:9.3   ����CSUDIOSGGetDisplaySurface��ȡ�����ֱ����л����豸���Դ�
//@EXECUTIONFLOW:9.4   ����CSUDIOSGBlit�ٽ�����UDI2OSG_search.bmp��surface����blit�����ֱ����л����豸�Դ���
//@EXECUTIONFLOW:9.5   ����CSUDIVIDEOSetWindowSize���÷����ֱ����л����豸��Ƶ���С����
//@EXECUTIONFLOW:10������CSUDISCREENSetResolution���ø߱���ֱ���,ÿ����һ���ȴ�����ȷ�Ͻ��
//@EXECUTIONFLOW:11������CSUDISCREENSetResolution���ñ���ֱ���10��,ÿ�μ��1s,�������ȷ�Ͻ��
//@EXECUTIONFLOW:12������CSUDISCREENSetResolution���ø���ֱ���10��,ÿ�μ��2s,�������ȷ�Ͻ��
//@EXECUTIONFLOW:13���ָ��ֳ�,�����رղ�����,���Ĳ���surface,����߱����Դ�ͼ��	
CSUDI_BOOL CSTC_ITTEST_IT_0016(void)
{
	CSUDI_HANDLE   hAVPlayer = NULL;
	CSUDI_HANDLE   hSurfaceUser = NULL;
	CSUDI_HANDLE hOSGSD=NULL,hOSGHD=NULL;
	CSUDIOSGRect_S stRect={600,150,550,230};
	CSUDI_BOOL bIsSupportHD = IT_iIsSupportHDDisplay();
	CSUDI_BOOL bIsShareHD = IT_iIsShareHDDisplaySurface();
	CSUDIWinRect_S stWinRectSD,stWinRectHD;
	CSUDISCREENResolution_E eOriginalSDResolution,eOriginalHDResolution;
	CSUDISCREENResolution_E enTypeSD[]={EM_UDISCREEN_RESOLUTION_NTSC,EM_UDISCREEN_RESOLUTION_PAL};
	CSUDISCREENResolution_E enTypeHD[]={EM_UDISCREEN_RESOLUTION_1080I_50HZ,EM_UDISCREEN_RESOLUTION_720P_50HZ};
	int i=0;
	CSUDISCREENType_E enDev;
	struct IT_ResolutionInfo stResInfo;
	int nSDResCnt = sizeof(enTypeSD)/sizeof(CSUDISCREENResolution_E);
	int nHDResCnt = sizeof(enTypeHD)/sizeof(CSUDISCREENResolution_E);
	int nVideoIndex = 0;
	int nTestTimes = 10;

	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_VIDEO|EM_IT_STREAM_AUDIO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");
	
	CSUDIVIDEOSetStopMode(nVideoIndex, EM_UDIVIDEO_STOPMODE_BLACK);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE),"����Ƶʧ��!");

	memcpy(&stWinRectHD,&stRect,sizeof(stRect));
	stWinRectSD.m_nX = stWinRectHD.m_nX*720/1280;
	stWinRectSD.m_nY = stWinRectHD.m_nY*576/720;
	stWinRectSD.m_nWidth = stWinRectHD.m_nWidth*720/1280;
	stWinRectSD.m_nHeight = stWinRectHD.m_nHeight*576/720;
	
	//��ȡԭʼ�ķֱ��ʣ����ڻָ��ֳ�
	//���ø���ֱ���Ϊ720p����֤��Ƶ�������Ϊ1280*720���Ա�������С����λ����ȷ
	if (bIsSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_HD, &eOriginalHDResolution), "fail to get HD screen resolution\n");
		
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev, &enTypeHD[1], 1),
				"fail to set HD resolution\n");
	}
	
	if (!bIsShareHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENGetResolution(EM_UDI_VOUT_DEVICE_SD, &eOriginalSDResolution), "fail to get SD screen resolution\n");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD,&stWinRectSD),
				"fail to set SD screen\n");
	}

	if (bIsSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD,&stWinRectHD),
				"fail to set HD screen\n");
	}

	//UDI2OSG_search.bmp�Ĵ�СΪ1280*720
	CSTK_ASSERT_TRUE_FATAL(IT_iGetPhotoToDisplay(&hSurfaceUser,1280,720,"UDI2OSG_search.bmp"),
		"get photo file to display surface fail\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGFill(hSurfaceUser,&stRect,0x0),"fail to fill surface\n");

	if (!bIsShareHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0,&hOSGSD),"fail to get SD display surface\n");
	}

	if (bIsSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hOSGHD),"fail to get HD display surface\n");
	}

	if (!bIsShareHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hOSGSD,NULL,hSurfaceUser,NULL,EM_UDIOSG_OPMODE_COPY),
			"fail to blit to sd display surface\n");
	}
	if (bIsSupportHD)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGBlit(hOSGHD,NULL,hSurfaceUser,NULL,EM_UDIOSG_OPMODE_COPY),
				"fail to blit to hd display surface\n");
	}

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"snyc failure");
	
	CSTCPrint("��Ļ����ʾ�������Ƚ������Ƶ��ĿС����(Y/N)\n");
	CSTCPrint("We expect normal A/V output in small windown on the screen\n");
	CSTKWaitYes();

	stResInfo.m_hSurfaceUser = hSurfaceUser;
	memcpy(&stResInfo.m_stWinRectHD,&stWinRectHD,sizeof(stRect));
	memcpy(&stResInfo.m_stWinRectSD,&stWinRectSD,sizeof(stRect));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENAddCallback(IT_iScreenCB,(void*)&stResInfo),
			"fail to call CSUDISCREENAddCallback\n");

	if (!bIsShareHD)
	{
		enDev = EM_UDI_VOUT_DEVICE_SD;
		for(i=0;i<sizeof(enTypeSD)/sizeof(CSUDISCREENResolution_E);i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev,&enTypeSD[i],1),
				"fail to set SD resolution\n");
			IT_iDumpMsg(FALSE);
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"fail to verify the sd result\n");
		}
	}
	
	if (bIsSupportHD)
	{
		enDev = EM_UDI_VOUT_DEVICE_HD;
		for(i=0;i<sizeof(enTypeHD)/sizeof(CSUDISCREENResolution_E);i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev,&enTypeHD[i],1),
				"fail to set HD resolution\n");
			IT_iDumpMsg(TRUE);
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"fail to verify the hd result\n");
		}
	}

	if (!bIsShareHD)
	{
		CSTCPrint("���潫���Ա��巴���ֱ����л�,��ע���������л��������Ƿ����쳣\n");
		CSTCPrint("�����������\n");
		CSTCPrint("Now we will switch the resolution of SD,and pls pay attention during the routine\n");
		CSTCPrint("Pres any key to continue..\n");
		CSTKWaitAnyKey();
	
		enDev = EM_UDI_VOUT_DEVICE_SD;
		for(i=0;i<nTestTimes;i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev,&enTypeSD[i%nSDResCnt],1),
				"fail to set SD resolution\n");
			CSUDIOSThreadSleep(1000);
		}
		
		IT_iDumpMsg(FALSE);
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"fail to verify the sd result\n");
	}

	if (bIsSupportHD)
	{
		CSTCPrint("���潫���Ը��巴���ֱ����л�,��ע���������л��������Ƿ����쳣\n");
		CSTCPrint("�����������\n");
		CSTCPrint("Now we will switch the resolution of HD,and pls pay attention during the routine\n");
		CSTCPrint("Pres any key to continue..\n");
		CSTKWaitAnyKey();

		enDev = EM_UDI_VOUT_DEVICE_HD;
		for(i=0;i<nTestTimes;i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISCREENSetResolution(&enDev,&enTypeHD[i%nHDResCnt],1),
					"fail to set HD resolution\n");
			CSUDIOSThreadSleep(2000);
		}

		IT_iDumpMsg(TRUE);
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"fail to verify the sd result\n");
	}
	
	CSTK_FATAL_POINT
	if (hAVPlayer != CSUDI_NULL)
	{
	    CSUDIPLAYERClose (hAVPlayer);
	     hAVPlayer = CSUDI_NULL;
	}
	if (hSurfaceUser != CSUDI_NULL)
	{
		CSUDIOSGDestroySurface(hSurfaceUser);
	}
	if (hOSGSD)
	{
		CSUDIOSGFill(hOSGSD,NULL,0x0);
	}
	if (hOSGHD)
	{
		CSUDIOSGFill(hOSGHD,NULL,0x0);
	}

	CSUDISCREENDelCallback(IT_iScreenCB,(void*)&stResInfo);

	if (!bIsShareHD)
	{
		enDev = EM_UDI_VOUT_DEVICE_SD;
		CSUDISCREENSetResolution(&enDev, &eOriginalSDResolution, 1);
		
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);
	}
	if (bIsSupportHD)
	{
		enDev = EM_UDI_VOUT_DEVICE_HD;
		CSUDISCREENSetResolution(&enDev, &eOriginalHDResolution, 1);

		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);
	}
	
	return TRUE;
}

#define RWFLASHSIZE (1024)
static CSUDI_BOOL s_bErrFlash = CSUDI_FALSE;
static CSUDI_BOOL s_bErrFs = CSUDI_FALSE;

void iTestFlashReadWrite(void * pvParam)
{
	int nTime1;
	int nTime;
	 BYTE bVirtualFlash[RWFLASHSIZE] = {0};
	memset(bVirtualFlash,0x55,RWFLASHSIZE);
	CSTCPrint("\r\n================iTestReadWriteFlash =========\n\n");
	nTime1 = CSUDIPlusOSGetTickCount();
	while(1)
	{
		if(CSUDIFlashRead(g_uItTestStartAddress, bVirtualFlash, RWFLASHSIZE)!= CSUDI_SUCCESS)
		{
			CSTCPrint("����TestFlashReadWrite ��flashʧ��\n");
			s_bErrFlash = CSUDI_TRUE;
			return;
		}
		
		CSUDIOSThreadSleep(100);
		
		if(CSUDIFlashWrite(g_uItTestStartAddress, bVirtualFlash, RWFLASHSIZE)!= CSUDI_SUCCESS)
		{
			CSTCPrint("�����һ��TestFlashReadWrite дflashʧ��\n");
			s_bErrFlash = CSUDI_TRUE;
			return;
		}
		CSUDIOSThreadSleep(500);
		memset(bVirtualFlash,0xff,RWFLASHSIZE);
		
		if(CSUDIFlashWrite(g_uItTestStartAddress, bVirtualFlash, RWFLASHSIZE) != CSUDI_SUCCESS)
		{
			CSTCPrint("����ڶ���TestFlashReadWrite дflashʧ��\n");
			s_bErrFlash = CSUDI_TRUE;
			return;
		}
		CSUDIOSThreadSleep(100);
		memset(bVirtualFlash,0x0,RWFLASHSIZE);
		nTime = CSUDIPlusOSGetTickCount() - nTime1;

		if(nTime >= 60000*15)
		{
			CSTCPrint("\n\n*****��ʱ�˳�***\n");
			
			return;
		}
	}

}

void iTestFsReadWrite()
{
	CSUDI_HANDLE hFile = CSUDI_NULL;
	char ch64[64];
	CSUDI_UINT32 dwIds[32];
	CSUDI_UINT32 dwVolumeId = 0;
	CSUDIFSDeviceInfo_S sDeviceInfo;
	char   szFilePath[CSUDI_MAX_FILE_NAME_LEN];
	int  nActIdCnt = 0; 
	CSUDI_BOOL bRet;
	int i = 0;
	int nLen = 0;
	unsigned int loop = 0;
	unsigned int nwrite= 0;
	unsigned int nread= 0;
	CSUDIFSStatFs_S  sFsStat;

	int nTime1;
	int nTime;
	
	CSTCPrint("\r\n================iTestFSReadWrite =========\n\n");
 	bRet = CSUDIFSGetAllDeviceId(dwIds, 32, &nActIdCnt);
	memset(szFilePath, 0, CSUDI_MAX_FILE_NAME_LEN);
	for(i = 0; i < nActIdCnt; i++)
	{	
		if(!CSUDI_ISREMOVEABLE(dwIds[i] ))
		{	
			dwVolumeId = dwIds[i];
			
			bRet= CSUDIFSGetDeviceInfo(dwVolumeId, &sDeviceInfo);
	
			if(sDeviceInfo.m_eDeviceType == EM_UDIFS_DEVTYPE_VOLUME)
			{	
				strcpy(szFilePath, sDeviceInfo.m_szMountPoint);
				CSUDIFSStatfs(szFilePath, &sFsStat);
				
                strcat(szFilePath, "/testfile.txt");
				
				if(sFsStat.m_eFsType == EM_UDIFS_PARTITION_JFFS2 && strlen(szFilePath) > 0)
				{	
					break;
				}
			}
		}
	}
	nLen = strlen(szFilePath);
	if(nLen > CSUDI_MAX_FILE_NAME_LEN)
	{
		return;
	}
	CSTCPrint("szFilePath = %s\n",szFilePath);
	memset(ch64,0xff,sizeof(ch64));
    
	CSUDIFSRemove(szFilePath);
	
	hFile = CSUDIFSOpen(szFilePath, "w+");
	if(hFile==CSUDI_NULL) 
	{
		CSTCPrint("����TestFsReadWriten���ļ�ʧ��\n");
		s_bErrFs = CSUDI_TRUE;
		return;
	}
	
	nTime1 = CSUDIPlusOSGetTickCount();
	
	while(1)
	{	
		 i = 0;
		if(hFile == CSUDI_NULL)
		{
			hFile = CSUDIFSOpen(szFilePath, "w+");
			if(hFile == CSUDI_NULL)
			{
				CSTCPrint("����TestFsReadWriten���ļ�ʧ��\n");
				s_bErrFs = CSUDI_TRUE;
				return;
			}
		}
		for(i = 0;i <1024;i += sizeof(ch64))
		{
			nwrite += CSUDIFSWrite(hFile, (char*)ch64, sizeof(ch64));
		}

		CSUDIFSFlush(hFile);
		
		CSUDIFSSeek(hFile, 0, CSUDIFS_SEEK_SET);
		CSTCPrint("\n(%d)finish write (%d) data testwrite CurrentTimer %u\n",loop++,nwrite,CSUDIPlusOSGetTickCount());
		CSUDIOSThreadSleep(100);
		
		for(i = 0;i <1024;i += sizeof(ch64))
		{
			nread += CSUDIFSRead(hFile, (char*)ch64, sizeof(ch64));
			memset(ch64,0,sizeof(ch64));
		}
		CSTCPrint("\n(%d)finish read (%d) data testwrite CurrentTimer %u\n",loop,nread,CSUDIPlusOSGetTickCount());
		CSUDIFSClose(hFile);
		hFile = CSUDI_NULL;
		memset(ch64,0xff,sizeof(ch64));
		CSUDIOSThreadSleep(500);
		nTime = CSUDIPlusOSGetTickCount() - nTime1;

		if(nTime >= 60000*15)
		{
			CSTCPrint("\n\n*****��ʱ�˳�***\n");
			CSUDIFSRemove(szFilePath);
			return;
		}
	}
	
	CSUDIFSRemove(szFilePath);
}

//@CASEGROUP: 1��FS
//@CASEGROUP: 2��FLASH
//@DESCRIPTION:ͬʱ��дmtd�豸��JFFS2�ļ�ϵͳʱ��������
//@PRECONDITION:ƽ̨�ѳɹ���ʼ��
//@INPUT:��������Ϸ�
//@EXPECTATION:ÿһ��������Ԥ�ڽ��
//@EXECUTIONFLOW:1����������1��Flashĳ�����򲻶Ͻ��ж�д����
//@EXECUTIONFLOW:2����������2��JFFS2�ļ�ϵͳ���Ͻ��ж�д����
//@EXECUTIONFLOW:3���ȴ�15���ӣ�������������д����������������������Ҫ����������
CSUDI_BOOL CSTC_ITTEST_IT_0017(void)
{

	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	int nParam = 0;
	int nPriority = 128;
	CSUDI_BOOL bErr = CSUDI_FAILURE;
	int i = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread1", nPriority , 64*1024, 
							 iTestFlashReadWrite, &nParam, &hThreadhandle1),"��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"�����ɹ����������������������Ϊ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread2", nPriority , 64*1024, 
							(CSUDIOSThreadEntry_F )iTestFsReadWrite, (void *)&nParam, &hThreadhandle2),"��������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"�����ɹ����������������������Ϊ��");

	CSTCPrint("����ͬʱ���϶�дFlash��JFFS2�ļ�ϵͳ�������Լ15���ӣ���ȴ�\n");
	
	while(1)
	{
		CSUDIOSThreadSleep(1000);
		i++;
		if(s_bErrFs ==CSUDI_TRUE || s_bErrFlash ==CSUDI_TRUE)
		{
			if (hThreadhandle1 != CSUDI_NULL)
			{
		 		 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ������ʧ��");
		  		 hThreadhandle1 = CSUDI_NULL;
			}
	
			if (hThreadhandle2 != CSUDI_NULL)
			{
				  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ������ʧ��");
		 	 	 hThreadhandle2 = CSUDI_NULL;
			}
			bErr = CSUDI_TRUE;
			break;
		}
		if(i > 60*15)//����15�������Զ���������
		{
			CSUDIOSThreadSuspend(hThreadhandle1);
			CSUDIOSThreadSuspend(hThreadhandle2);
			if (hThreadhandle1 != CSUDI_NULL)
			{
		  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ������ʧ��");
		  		 hThreadhandle1 = CSUDI_NULL;
			}
	
			if (hThreadhandle2 != CSUDI_NULL)
			{
		  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ������ʧ��");
		   		hThreadhandle2 = CSUDI_NULL;
			}
			break;
		}
	}
	if(bErr == CSUDI_TRUE)
	return CSUDI_FALSE;

	CSTK_FATAL_POINT;
		
	if (hThreadhandle1 != CSUDI_NULL)
	{
		  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ������ʧ��");
		   hThreadhandle1 = CSUDI_NULL;
	}
	
	if (hThreadhandle2 != CSUDI_NULL)
	{
		  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ������ʧ��");
		   hThreadhandle2 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//����֧��nDemxType���͵�demx
//nDemxType�μ�CSUDIDEMUXWorkType_E
static int searchDemuxID_Y_InInject(int nDemxType)
{
	int i = 0;
	int  nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S   sDemuxCapabilityInfo;

	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //��ȡ��demux������
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //ȡ���ӦID=i ��demux������
			{
				if((sDemuxCapabilityInfo.m_dwWorkTypeMask & nDemxType ) != 0)
				{
					nDemuxID=i;
					break;
				}
			}	
		}
	}
	CSTCPrint("nDemuxID nDemuxID=%d\n\r",nDemuxID);
	return  nDemuxID;
}

//@CASEGROUP:PLAYER INJECTER
//@DESCRIPTION:����PCMע�벥����ֱ���ܹ������л�
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: 
//@EXECUTIONFLOW:1������CSUDIINJECTEROpen������һ��ע��ʵ��
//@EXECUTIONFLOW:2������CSUDIINJECTERSetAttribute�� ����INJECTERע��ʵ����������Ϣ
//@EXECUTIONFLOW:2������CSUDIINJECTERSetAttribute�� �ٴ�����INJECTERע��ʵ����������Ϣ
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen������һ��ע�벥����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart  ����ע��ʵ��
//@EXECUTIONFLOW:5������CSUDIOSThreadCreate������һ��ע���߳�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream�����ò���������
//@EXECUTIONFLOW:8������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ���Ƶ���ų�����
//@EXECUTIONFLOW:10������CSUDIOSThreadJoin���ȴ�ע���߳̽���
//@EXECUTIONFLOW:11������CSUDIPLAYERStop���ر�ע�벥����
//@EXECUTIONFLOW:12������CSUDIPLAYERClose��ɾ��ע�벥����
//@EXECUTIONFLOW:13������CSUDIINJECTERStop��ֹͣ����ע��ʾ��
//@EXECUTIONFLOW:13������CSUDIINJECTERClose��ɾ��ע��ʵ��
//@EXECUTIONFLOW:14������CSUDIOSThreadDestroy������ע���߳�
//@EXECUTIONFLOW:15������CSUDIPLAYEROpen������һ��ֱ��������
//@EXECUTIONFLOW:16������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:17������CSUDIPLAYERAddPlayerCallback�������Ƶ��Դ�ı�Ļص��¼�
//@EXECUTIONFLOW:18������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:19����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:20������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:21����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:22    ����CSUDIPLAYERCloseɾ��������
//@EXECUTIONFLOW:       �ָ��ֳ�
CSUDI_BOOL CSTC_ITTEST_IT_0018(void)
{

	CSUDI_HANDLE hLivePlayer = CSUDI_NULL;
	CSUDI_HANDLE hPcmPlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectPCMThread = CSUDI_NULL;
	CSUDIINJECTERChnl_S sInjectChnl;
	CSUDIINJECTEROpenParam_S sInjectOpenParam;
	CSUDIPlayerChnl_S stPlayerChnl;
	CSUDIINJECTERPcmStartParam_S sPcmStartParam;
	ITInjectPCMParam_S  sThreadParam;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nAudioDecoderId = -1;
	int i = 0;
	
	memset(&sThreadParam, 0, sizeof(sThreadParam));

     for (i = 0; i < REPEAT_TIMES; i++)
     {
	memset(&stPlayerChnl, -1, sizeof(stPlayerChnl));
	memset(&sInjectChnl, -1, sizeof(sInjectChnl));	
	
	nAudioDecoderId = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);

	sInjectChnl.m_nAudioDecoder = nAudioDecoderId; //PCM���ݲ���ֻҪ��Ƶ������

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_AUDIO;  
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_PCM;				

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTEROpen(&sInjectChnl, &sInjectOpenParam, &hINJECTER) && hINJECTER != CSUDI_NULL),
			"���� 1 open injecter  failure !"
		);
	sPcmStartParam.m_uBitsSample = g_PCMInfo[0].m_sStartParam.m_uBitsSample;
	sPcmStartParam.m_uChannels = g_PCMInfo[0].m_sStartParam.m_uChannels;
	sPcmStartParam.m_uSampleRate = g_PCMInfo[0].m_sStartParam.m_uSampleRate;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam)),
			"���� 2 set PCM_PARAMS attribute  failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN, &g_PCMInfo[0].m_eEndian)),
			"���� 2 set PCM_ENDIAN attribute  failure !"
		);

	stPlayerChnl.m_nAudioDecoder = nAudioDecoderId;//PCM���ݲ���ֻҪ��Ƶ������
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&stPlayerChnl,EM_UDIPLAYER_INJECT,&hPcmPlayer)) && hPcmPlayer != CSUDI_NULL,
			"���� 3 open pcm player failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER)),
			"���� 4 start injecter  failure !"
		);
		
	sThreadParam.pcFile = g_PCMInfo[0].m_pcFileName;
	sThreadParam.hInjecter = hINJECTER;
	g_bInjectPCMFinish = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIOSThreadCreate("PcmInjecter",5*17,4*1024,iInjectPCMFile,(void*)&sThreadParam, &hInjectPCMThread),
			"Create Inject thread failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(hInjectPCMThread != CSUDI_NULL),
			"Create Inject thread failure !"
		);
	
	sStreamInfo[0].m_nPid = 0; //no used
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_PCM;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPcmPlayer,sStreamInfo, 1, CSUDI_NULL),
			"����5ʧ��"
		);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPcmPlayer), "����6ʧ��");

	CSTCPrint("��Ƶ���ų�������\n");
	CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ��");

	g_bInjectPCMFinish = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectPCMThread), "����8ʧ��"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPcmPlayer), "����9ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPcmPlayer), "����10ʧ��");

	hPcmPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "ֹͣע��ʵ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectPCMThread), "����12ʧ��");

	hInjectPCMThread= CSUDI_NULL;
		
	stPlayerChnl.m_nDemux = searchDemuxID_Y_InInject(EM_UDI_DEMUX_PLAY);
	stPlayerChnl.m_nAudioDecoder = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG2);
	stPlayerChnl.m_nVideoDecoder = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== IT_iLockTuner (EM_IT_MPEG2_PAL), "��Ƶʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_LIVE,&hLivePlayer), "����13ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE,hLivePlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_VIDEO|EM_IT_STREAM_AUDIO|EM_IT_STREAM_PCR),
			"���� 14 set video stream failure "
		);

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hLivePlayer), "����18ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
    	CSTCPrint("Is A/V output normal ?\n");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����16ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hLivePlayer), "����17ʧ��");
    	CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
  	CSTCPrint("Does the player stop ?\n");

    		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����18ʧ�ܣ�ֹͣ���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "����20ʧ��");

	hLivePlayer=CSUDI_NULL;
     }

	CSTK_FATAL_POINT;
	
	if (hPcmPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPcmPlayer), "����21ʧ��");
		hPcmPlayer=CSUDI_NULL;
	}

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "����22ʧ��");
		hLivePlayer=CSUDI_NULL;
	}

	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hINJECTER), "����23ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectPCMThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectPCMThread), "����24ʧ��");
		hInjectPCMThread = CSUDI_NULL;
	}
		
	return CSUDI_TRUE;
}

static void TS_injectTask(void * TSStreamInfo)
{
	void * ppvBuffer = NULL;

	unsigned  int  uLength = 0;
	
	TSStreamInfo_S *pTSStreamInfo = (TSStreamInfo_S*)TSStreamInfo;

	CSUDI_HANDLE hInjecter = pTSStreamInfo->m_hInjecter;
	
	int nBlockNum = 0;
	long nFileAllLength = 0;  //�ļ��ܳ���
	int nFileCurrentPos = 0; //�ļ���ǰλ��
	int nFileLeftLength = 0; //ʣ��δ���ļ�����
	int nInjectedLength = 0; //��ע��Ĵ�С
	int nReadLength = 0;
		
	void* file = CSUDI_NULL;
	
	if (!getInjecterFile((const char*)pTSStreamInfo->m_pcName, &file, &nFileAllLength))
	{
       		CSTCPrint("getInjecterFile failed!\n\r");
       		pTSStreamInfo->m_bRun = CSUDI_TRUE;
       		pTSStreamInfo->m_bOpenFileSuccess = CSUDI_FALSE;
       		return;
	}
	
	if (pTSStreamInfo->m_uBufferLen<10240||pTSStreamInfo->m_uBufferLen>1024*1024)
	{
		pTSStreamInfo->m_uBufferLen = 10*1024;
	}
	pTSStreamInfo->m_bOpenFileSuccess = CSUDI_TRUE;
	pTSStreamInfo->m_bRun = CSUDI_TRUE;

	
	while(pTSStreamInfo->m_bRun)
	{
		//�����ǰʣ��δ���ļ��ĳ���
		nFileCurrentPos=InjectFSTell(file);	//��ǰ�Ѷ��ļ��ĳ���
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //ʣ��δ���ļ��ĳ���

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
		
				
		if(uLength != 0)
		{
			nReadLength = pTSStreamInfo->m_uBufferLen < nFileLeftLength ? pTSStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //ʣ���ļ�����С�ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = nReadLength;
			}
			else  //ʣ���ļ����ȴ��ڵ��ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //��ȡ��Ӧ���ȵ��ļ�
									
			CSUDIINJECTERWriteComplete(hInjecter,nInjectedLength);
			
			if(nFileLeftLength == 0)
			{
				InjectFSSeek(file,0,INJECT_SEEK_SET);
			}		
		}
		else
	        {
			CSUDIOSThreadSleep(10);
	        }
	}

	if(!pTSStreamInfo->m_bRun )
	{
		InjectFSClose(file);
	}
	
}

static void PS_injectTask(void * TSStreamInfo)
{
	void * ppvBuffer = NULL;

	unsigned  int  uLength = 0;
	
	TSStreamInfo_S *pTSStreamInfo = (TSStreamInfo_S*)TSStreamInfo;

	CSUDI_HANDLE hInjecter = pTSStreamInfo->m_hInjecter;
	
	int nBlockNum = 0;
	long nFileAllLength = 0;  //�ļ��ܳ���
	int nFileCurrentPos = 0; //�ļ���ǰλ��
	int nFileLeftLength = 0; //ʣ��δ���ļ�����
	int nInjectedLength = 0; //��ע��Ĵ�С
	int nReadLength = 0;
		
	void* file = CSUDI_NULL;
	
	if (!getInjecterFile((const char*)pTSStreamInfo->m_pcName, &file, &nFileAllLength))
	{
       		CSTCPrint("getInjecterFile failed!\n\r");
       		pTSStreamInfo->m_bOpenFileSuccess = CSUDI_FALSE;
       		pTSStreamInfo->m_bRun = CSUDI_TRUE;
       		return;
	}
	
	if (pTSStreamInfo->m_uBufferLen<10240||pTSStreamInfo->m_uBufferLen>1024*1024)
	{
		pTSStreamInfo->m_uBufferLen = 10*1024;
	}
	
	pTSStreamInfo->m_bOpenFileSuccess = CSUDI_TRUE;
	pTSStreamInfo->m_bRun = CSUDI_TRUE;

	while(!s_exitinject)
	{
		//�����ǰʣ��δ���ļ��ĳ���
		nFileCurrentPos=InjectFSTell(file);	//��ǰ�Ѷ��ļ��ĳ���
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //ʣ��δ���ļ��ĳ���

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
		
				
		if(uLength != 0)
		{
			nReadLength = pTSStreamInfo->m_uBufferLen < nFileLeftLength ? pTSStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //ʣ���ļ�����С�ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = nReadLength;
			}
			else  //ʣ���ļ����ȴ��ڵ��ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //��ȡ��Ӧ���ȵ��ļ�
									
			CSUDIINJECTERWriteComplete(hInjecter,nInjectedLength);
			
			if (nFileLeftLength == 0)
			{
				InjectFSSeek(file,0,INJECT_SEEK_SET);
				break;
			}		
		}
		else
	        {
			CSUDIOSThreadSleep(10);
	        }
	}

	CSUDIINJECTERFlush(hInjecter);

	s_exitinject = CSUDI_TRUE;
	
	if(s_exitinject)
	{
		InjectFSClose(file);
	}
	
}

static int searchVideoID_Y_InInject(int type_index)
{
	int i;
	int j;
	CSUDIVIDStreamType_E  eVidStreamTypeNum=EM_UDI_VID_STREAMTYPE_NUM ;
	int nVideoCount;
	int nVidioID=-1;
	CSUDIVIDEOCapability_S   sVideoCapabilityInfo;	
	
	CSUDIVIDEOGetCount(&nVideoCount);                                      //��ȡ����

	for ( i=0;i < nVideoCount; i ++)
	{
		CSUDIVIDEOGetCapability( i , &sVideoCapabilityInfo);	//��ȡ�������������Ƶ��ͬ������Ƶֻ��֧�ֵ���Ƶ����һ��

		for (j=0; j < eVidStreamTypeNum; j++)
		{
			if (sVideoCapabilityInfo.m_eStreamType[j] == EM_UDI_VID_STREAM_UNKNOWN )
			{
				break;
			}
			if (sVideoCapabilityInfo.m_eStreamType[j]==type_index)
			{
				nVidioID=i;
				break;
			}
		}

		if (nVidioID!=-1)
		{
			break;
		}
	}

	return nVidioID;

}

static int  getTSInjectInfo(int index,CSUDIINJECTERChnl_S  *psInjecterChnl,CSUDIPlayerChnl_S  *psPlayerChnl,CSUDIINJECTEROpenParam_S *pOpenParam ,CSUDIStreamInfo_S  sStreamInfo[])
{
	int nAudioId = -1;
	int nVideoId = -1;
	int nDemuxID = -1;
	int i = 0;
	int nStreamCnt = 0;
	TSStreamInfo_S *pTSStreamInfo = &st_TSStreamInfo[index];

//ע�������Ƶ������
	psInjecterChnl->m_nAudioDecoder=-1;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;	
	psInjecterChnl->m_nDemux=-1;
		
	nDemuxID=searchDemuxID_Y_InInject(EM_UDI_DEMUX_INJECT);
	if(nDemuxID == -1)
	{
		CSTCPrint( "û��֧��ע���demux! \n\r");
        CSTCPrint("There's no demux device supporting inject \n");
        return -1;
	}

	psPlayerChnl->m_nDemux = nDemuxID;
	
	for (i=0;i<MAX_PESES_COUNT_IN_TS;i++)
	{
	    if (pTSStreamInfo->m_TSContentInfo[i].m_nPid == INJECT_INVALID_PID)
		{
			continue;
		}
            
		if (pTSStreamInfo->m_TSContentInfo[i].m_eContentType == EM_UDI_CONTENT_VIDEO)
		{
			nVideoId = searchVideoID_Y_InInject(pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eVideoType);
			if(nVideoId ==-1)
			{
				CSTCPrint( "û��֧��ָ��������Ƶ������!\n\r");
                CSTCPrint("There's no video decode supporting stream type %d \n",\
                        pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eVideoType);
                return -1;
			}
			psPlayerChnl->m_nVideoDecoder = nVideoId;
	
		}
		else if (pTSStreamInfo->m_TSContentInfo[i].m_eContentType == EM_UDI_CONTENT_AUDIO)
		{
			nAudioId = searchAudioID_Y_InInject(pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eAudioType);
			if(nAudioId ==-1)
			{
				CSTCPrint( "û��֧��ָ��������Ƶ������!\n\r");
                CSTCPrint("There's no audio decode supporting stream type %d \n",\
                    pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eAudioType);
                return -1;
			}
			psPlayerChnl->m_nAudioDecoder = nAudioId;
		}
        
		memcpy(&sStreamInfo[i],&(pTSStreamInfo->m_TSContentInfo[i]),sizeof(CSUDIStreamInfo_S));
        nStreamCnt++;
        
	}

	if(nStreamCnt<1)//����ע��һ·����(video,audio,pcr...)
	{
        CSTCPrint("û������ע������!!\n");
        CSTCPrint("There's no stream type configed !!\n");
        return -1;
	}
    
//����ͨ��
	psInjecterChnl->m_nDemux=psPlayerChnl->m_nDemux;

	pOpenParam->m_eInjecterType = EM_INJECTER_TS;
	pOpenParam->m_eContentType = EM_UDIINJECTER_CONTENT_DEFAULT;

    initAVInfo(psPlayerChnl->m_nAudioDecoder,psPlayerChnl->m_nVideoDecoder);
	return nStreamCnt;
}


static int  getPSInjectInfo(int index,CSUDIINJECTERChnl_S  *psInjecterChnl,CSUDIPlayerChnl_S  *psPlayerChnl,CSUDIINJECTEROpenParam_S *pOpenParam ,CSUDIStreamInfo_S  sStreamInfo[])
{
	int i = 0;
	int nStreamCnt = 2;
	TSStreamInfo_S *pPSStreamInfo = &st_TSStreamInfo[index];

//ע�������Ƶ������
	psInjecterChnl->m_nAudioDecoder=0;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=0;	
	psInjecterChnl->m_nDemux=-1;

	psPlayerChnl->m_nAudioDecoder=0;//nAudioId;
	psPlayerChnl->m_nVideoDecoder=0;	
	psPlayerChnl->m_nDemux=-1;

	for (i=0;i<nStreamCnt;i++)
	{
		memcpy(&sStreamInfo[i],&(pPSStreamInfo->m_TSContentInfo[i]),sizeof(CSUDIStreamInfo_S));
	}
	
	pOpenParam->m_eInjecterType = EM_INJECTER_PS;
	pOpenParam->m_eContentType = EM_UDIINJECTER_CONTENT_DEFAULT;

   	initAVInfo(psPlayerChnl->m_nAudioDecoder,psPlayerChnl->m_nVideoDecoder);
   	
	return nStreamCnt;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����TSע�벥����ֱ���л�
//@PRECONDITION:1��INJECTER PLAYERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����TS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXECUTIONFLOW:1������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:8����ʾ�û�ȷ�ϣ��Ƿ�������Ƶ��Ŀ���ų���?//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:9������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:11������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYEROpen������һ��ֱ��������
//@EXECUTIONFLOW:15������CSUDIPLAYERSetStream�����ò�������Ӧ����
//@EXECUTIONFLOW:16������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:17����ʾ������Ա�۲죬�Ƿ�������Ƶ���ų���
//@EXECUTIONFLOW:18������CSUDIPLAYERStop  ֹͣ������
//@EXECUTIONFLOW:19����ʾ������Ա�۲죬�Ƿ�����Ƶֹͣ����
//@EXECUTIONFLOW:20    ����CSUDIPLAYERCloseɾ��������
//@EXECUTIONFLOW:21    �ظ�����1-23
//@EXECUTIONFLOW:22    �����ֳ�
CSUDI_BOOL CSTC_ITTEST_IT_0019(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S stPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[10];
	int i = 0;

	CSUDI_HANDLE   hLivePlayer = CSUDI_NULL;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nStreamCount = 0;

	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&stPlayerChnl,-1,sizeof(stPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	

   for(i= 0;i< REPEAT_TIMES;i++)
   {
	nStreamCount = getTSInjectInfo(EM_TS_INJECT,&sInjecterChnl,&stPlayerChnl,&sOpenParams,sStreamInfo);
   	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
   	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
        CSTK_ASSERT_TRUE_FATAL(((stPlayerChnl.m_nDemux!= -1)&& ((stPlayerChnl.m_nAudioDecoder != -1) || (stPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����3ʧ��");

	st_TSStreamInfo[EM_TS_INJECT].m_hInjecter = hINJECTER;
	st_TSStreamInfo[EM_TS_INJECT].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &st_TSStreamInfo[EM_TS_INJECT], &hInjectThread), "����4ʧ��");

   	while(!st_TSStreamInfo[EM_TS_INJECT].m_bRun) CSUDIOSThreadSleep(500);

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == st_TSStreamInfo[EM_TS_INJECT].m_bOpenFileSuccess, "����5 �ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,2,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	st_TSStreamInfo[EM_TS_INJECT].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����12ʧ��");

	hPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����13ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "ɾ��ע�����ʧ��");

	hInjectThread = CSUDI_NULL;
		
	stPlayerChnl.m_nDemux = searchDemuxID_Y_InInject(EM_UDI_DEMUX_PLAY);
	stPlayerChnl.m_nAudioDecoder = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG2);
	stPlayerChnl.m_nVideoDecoder = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== IT_iLockTuner (EM_IT_MPEG2_PAL), "��Ƶʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_LIVE,&hLivePlayer), "����15ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE,hLivePlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_VIDEO|EM_IT_STREAM_AUDIO|EM_IT_STREAM_PCR),
			"set video stream failure "
		);

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hLivePlayer), "����17ʧ��");

	CSUDIOSThreadSleep(2000);

	CSTCPrint("����Ƶ�Ƿ��ܹ��������ţ�\n");
    	CSTCPrint("Is A/V output normal ?\n");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����18ʧ�ܣ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hLivePlayer), "����19ʧ��");
    	CSTCPrint("����Ƶ�Ƿ�ֹͣ������\n");
  	CSTCPrint("Does the player stop ?\n");

    		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����20ʧ�ܣ�ֹͣ���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "����21ʧ��");

	hLivePlayer=CSUDI_NULL;
     }

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����22ʧ��");
		hPlayer=CSUDI_NULL;
	}

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "����23ʧ��");
		hLivePlayer=CSUDI_NULL;
	}

	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hINJECTER), "����24ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����25ʧ��");
		hInjectThread = CSUDI_NULL;
	}
		
	return CSUDI_TRUE;
}
	
//@CASEGROUP:PLAYER INJECTER
//@DESCRIPTION:����PCMע�벥����TS ע�벥���ܹ�ֱ���л������Ե������þ���
//@PRECONDITION:PLAYERģ�飬����Ƶ��������������ʼ��
//@INPUT: 
//@EXECUTIONFLOW:1������CSUDIINJECTEROpen������һ��ע��ʵ��
//@EXECUTIONFLOW:2������CSUDIINJECTERSetAttribute�� ����INJECTERע��ʵ����������Ϣ
//@EXECUTIONFLOW:2������CSUDIINJECTERSetAttribute�� �ٴ�����INJECTERע��ʵ����������Ϣ
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen������һ��ע�벥����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart  ����ע��ʵ��
//@EXECUTIONFLOW:5������CSUDIOSThreadCreate������һ��ע���߳�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream�����ò���������
//@EXECUTIONFLOW:8������CSUDIPLAYERStart  �򿪲�����
//@EXECUTIONFLOW:9����ʾ������Ա�۲죬�Ƿ���Ƶ���ų�����
//@EXECUTIONFLOW:10������CSUDIOSThreadJoin���ȴ�ע���߳̽���
//@EXECUTIONFLOW:11������CSUDIPLAYERStop���ر�ע�벥����
//@EXECUTIONFLOW:12������CSUDIPLAYERClose��ɾ��ע�벥����
//@EXECUTIONFLOW:13������CSUDIINJECTERStop��ֹͣ����ע��ʾ��
//@EXECUTIONFLOW:13������CSUDIINJECTERClose��ɾ��ע��ʵ��
//@EXECUTIONFLOW:14������CSUDIOSThreadDestroy������ע���߳�
//@EXECUTIONFLOW:15������ָ����TS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:16������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:18������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:20������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:21������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:22������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:23����ʾ�û�ȷ�ϣ��Ƿ�������Ƶ��Ŀ���ų���
//@EXECUTIONFLOW:24������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:25������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:26������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:27������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:28������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:29���ظ�����1-28
//@EXECUTIONFLOW:30�������ֳ�
CSUDI_BOOL CSTC_ITTEST_IT_0020(void)
{
	CSUDI_HANDLE hTSPlayer = CSUDI_NULL;
	CSUDI_HANDLE hPcmPlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectPCMThread = CSUDI_NULL;
	CSUDI_HANDLE hInjectTSThread = CSUDI_NULL;
	CSUDIINJECTERChnl_S sInjectChnl;
	CSUDIINJECTEROpenParam_S sInjectOpenParam;
	CSUDIPlayerChnl_S stPlayerChnl;
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERPcmStartParam_S sPcmStartParam;
	ITInjectPCMParam_S  sThreadParam;
	CSUDIStreamInfo_S  sStreamInfo[10];
	int nAudioDecoderId = -1;
	int nStreamCount = 0;
	int i = 0;

     for (i = 0; i < REPEAT_TIMES; i++)
     {

	memset(&stPlayerChnl, -1, sizeof(stPlayerChnl));
	memset(&sInjectChnl, -1, sizeof(sInjectChnl));
	nAudioDecoderId = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);

	sInjectChnl.m_nAudioDecoder = nAudioDecoderId; //PCM���ݲ���ֻҪ��Ƶ������

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_AUDIO;  
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_PCM;				

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTEROpen(&sInjectChnl, &sInjectOpenParam, &hINJECTER) && hINJECTER != CSUDI_NULL),
			"���� 1 open injecter  failure !"
		);
	sPcmStartParam.m_uBitsSample = g_PCMInfo[0].m_sStartParam.m_uBitsSample;
	sPcmStartParam.m_uChannels = g_PCMInfo[0].m_sStartParam.m_uChannels;
	sPcmStartParam.m_uSampleRate = g_PCMInfo[0].m_sStartParam.m_uSampleRate;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam)),
			"���� 2 set PCM_PARAMS attribute  failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN, &g_PCMInfo[0].m_eEndian)),
			"���� 2 set PCM_ENDIAN attribute  failure !"
		);
		
		//pcm player
	stPlayerChnl.m_nAudioDecoder = nAudioDecoderId;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&stPlayerChnl,EM_UDIPLAYER_INJECT,&hPcmPlayer)) && hPcmPlayer != CSUDI_NULL,
			"���� 3 open pcm player failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER)),
			"���� 4 start injecter  failure !"
		);
		
	sThreadParam.pcFile = g_PCMInfo[0].m_pcFileName;
	sThreadParam.hInjecter = hINJECTER;
	g_bInjectPCMFinish = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIOSThreadCreate("PcmInjecter",5*17,4*1024,iInjectPCMFile,(void*)&sThreadParam, &hInjectPCMThread),
			"Create Inject thread failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(hInjectPCMThread != CSUDI_NULL),
			"Create Inject thread failure !"
		);
		
	sStreamInfo[0].m_nPid = 0; //no used
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_PCM;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPcmPlayer,sStreamInfo, 1, CSUDI_NULL),
			"����5ʧ��"
		);
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPcmPlayer), "����6ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioDecoderId), "ȡ������ʧ��");
	
	CSTCPrint("��Ƶ���ų�������\n");
	CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioDecoderId), "����ʧ��");

	CSTCPrint("û���κ��������\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����7ʧ��");

	g_bInjectPCMFinish = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectPCMThread), "����8ʧ��"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPcmPlayer), "����9ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPcmPlayer), "����10ʧ��");

	hPcmPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioDecoderId), "ȡ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "ֹͣע��ʵ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectPCMThread), "����12ʧ��");

	hInjectPCMThread = CSUDI_NULL;
	
	//TSע��
	nStreamCount = getTSInjectInfo(EM_TS_INJECT,&sInjectChnl,&stPlayerChnl,&sOpenParams,sStreamInfo);
   	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
   	CSTK_ASSERT_TRUE_FATAL(((sInjectChnl.m_nDemux != -1)&&(sInjectChnl.m_nAudioDecoder==-1)&&(sInjectChnl.m_nVideoDecoder==-1)),"TSע���豸ָ�����Ϸ�");
        CSTK_ASSERT_TRUE_FATAL(((stPlayerChnl.m_nDemux!= -1)&& ((stPlayerChnl.m_nAudioDecoder != -1) || (stPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjectChnl,&sOpenParams,&hINJECTER), "����13ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����14ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����15ʧ��");

	st_TSStreamInfo[EM_TS_INJECT].m_hInjecter = hINJECTER;
	st_TSStreamInfo[EM_TS_INJECT].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &st_TSStreamInfo[0], &hInjectTSThread), "����15ʧ��");

   	while(!st_TSStreamInfo[EM_TS_INJECT].m_bRun) CSUDIOSThreadSleep(500);

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == st_TSStreamInfo[EM_TS_INJECT].m_bOpenFileSuccess, "�ļ���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_INJECT,&hTSPlayer), "����16ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hTSPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "����17ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hTSPlayer), "����18ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(stPlayerChnl.m_nAudioDecoder), "ȡ������ʧ��");

	CSTCPrint("����Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(stPlayerChnl.m_nAudioDecoder), "����ʧ��");
	
	CSTCPrint("û����������Ƶ��Ȼ����\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����ʧ��");

	st_TSStreamInfo[EM_TS_INJECT].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectTSThread),"����19ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hTSPlayer), "����20ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hTSPlayer), "����21ʧ��");

	hTSPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(stPlayerChnl.m_nAudioDecoder), "����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����22ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����23ʧ��");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectTSThread), "����24ʧ��");

	hInjectTSThread = CSUDI_NULL;

   }
	CSTK_FATAL_POINT;

	if (nAudioDecoderId != -1)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioDecoderId), "ȡ������ʧ��");
	}
	
	if (stPlayerChnl.m_nAudioDecoder != -1)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(stPlayerChnl.m_nAudioDecoder), "����ʧ��");
	}
	
	if (hPcmPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPcmPlayer), "����25ʧ��");
		hPcmPlayer=CSUDI_NULL;
	}

	if (hTSPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hTSPlayer), "����26ʧ��");
		hTSPlayer=CSUDI_NULL;
	}

	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hINJECTER), "����27ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectPCMThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectPCMThread), "����28ʧ��");
		hInjectPCMThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectTSThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectTSThread), "����29ʧ��");
		hInjectTSThread = CSUDI_NULL;
	}
		
	return CSUDI_TRUE;
}


//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����PSע�벥��
//@PRECONDITION:1��INJECTER PLAYERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PS������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXECUTIONFLOW:1������ָ����PS��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��TS  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡTS�����ļ���������TSע�����
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�TS������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�������������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�������Ƶ��Ŀ���ų���
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16�������ֳ�
CSUDI_BOOL CSTC_ITTEST_IT_0021(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S stPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo[2];

	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nStreamCount = 0;

	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&stPlayerChnl,-1,sizeof(stPlayerChnl));	
	memset(sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	nStreamCount = getPSInjectInfo(EM_PS_INJECT,&sInjecterChnl,&stPlayerChnl,&sOpenParams,sStreamInfo);
   	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"ע����Դ��ʼ��ʧ��");
   	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux == -1)&&(sInjecterChnl.m_nAudioDecoder!=-1)&&(sInjecterChnl.m_nVideoDecoder!=-1)),"PSע���豸ָ�����Ϸ�");
        CSTK_ASSERT_TRUE_FATAL(((stPlayerChnl.m_nDemux == -1)&& ((stPlayerChnl.m_nAudioDecoder != -1) || (stPlayerChnl.m_nVideoDecoder != -1))),"�������豸ָ�����Ϸ�");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	st_TSStreamInfo[EM_PS_INJECT].m_hInjecter = hINJECTER;
	st_TSStreamInfo[EM_PS_INJECT].m_bRun = CSUDI_FALSE;
	s_exitinject = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("PS Inject", INJECTTHREADPRO, INJECTTHREADBUF, PS_injectTask, ( void* ) &st_TSStreamInfo[EM_PS_INJECT], &hInjectThread), "����5ʧ��");

	while (!st_TSStreamInfo[EM_PS_INJECT].m_bRun) 
	{
		CSUDIOSThreadSleep(20);
	}

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == st_TSStreamInfo[EM_PS_INJECT].m_bOpenFileSuccess, "�ļ���ʧ��");

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,2,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	while(!s_exitinject)
	{
		CSUDIOSThreadSleep(20);
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����12ʧ��");

	hPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����13ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "ɾ��ע�����ʧ��");

	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "����16ʧ��");
		hPlayer = CSUDI_NULL;
	}

	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hINJECTER), "����17ʧ��");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����18ʧ��");
		hInjectThread = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��OSG
//@DESCRIPTION:���Բ���MPEG2��Ƶ��ͬʱ��Ӳ����ķ�ʽ����I֡ͼƬ
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧��I֡ͼƬӲ����
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:�����ܹ�����������Ƶ
//@EXECUTIONFLOW: 1������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIPLAYEROpen����һ��������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStart��ʼ������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5�������ܹ�����������Ƶ
//@EXECUTIONFLOW: 6�����²������I֡������7-12���ʧ������ת����13
//@EXECUTIONFLOW: 7����ȡUDI2OSG_720_576.mpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW: 8������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW: 9������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW: 10������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW: 11������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW: 12�������I֡�ɹ��������������ܿ���I֡ͼƬUDI2OSG_720_576.mpg�����ܹ�����������Ƶ
//@EXECUTIONFLOW: 13���ͷ�ͼƬ��Դ������surface�����ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0022(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_HANDLE hDispSurface = CSUDI_NULL;
	CSUDI_HANDLE hHDDispSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurInfo;
	unsigned char * pcBuff = CSUDI_NULL;
    unsigned int nLen = 0;
	int nVideoIndex = 0;
	
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");

	CSTCPrint("��Ƶ�������������� ?\n");
	CSTCPrint("Does the audio output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 5 check result failure!");

	if(IT_iIsSupportDecImage(EM_IT_DECODE_IFRAME))
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == IT_iTestDecImageWithScaleMode(EM_IT_DECODE_IFRAME,EM_IT_IFRAME_720_576,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface,&pcBuff,&nLen)),
				"step 7��8 decode iframe image failure !"
			);

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurInfo)),
				"Get IFRAME surface info failure !"
			);	
		
		if (!IT_iIsShareHDDisplaySurface())
		{
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDispSurface) && hDispSurface != CSUDI_NULL),
				"step 9 get display surface (sd) failure !"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hDispSurface, CSUDI_NULL, hSurface, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
				"step 10 blit (sd) failure !"
				);	
		}

		if(IT_iIsSupportHDDisplay())
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hHDDispSurface) && hHDDispSurface != CSUDI_NULL),
					"step 9 get display surface (hd) failure !"
					);

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGBlit(hHDDispSurface, CSUDI_NULL, hSurface, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
					"step 10 blit (hd) failure !"
			        );	
		}	
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"step 11 osg sync  failure !"
			);

		CSUDIOSThreadSleep(50);
	
		CSTCPrint("���������ܿ���ȫ��I֡UDI2OSG_720_576.mpg�����ܹ�����������Ƶ \n");
		CSTCPrint("We expect one IFRAME UDI2OSG_720_576.mpg on screen for a while and get normal audio output at the same time\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 14 check result failure!");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "Not support IFRAME decoding!");
	}

	CSTK_FATAL_POINT
	
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer=CSUDI_NULL;
	}
	
	if (hSurface)
	{
		CSUDIOSGDestroySurface(hSurface);

		hSurface = CSUDI_NULL;
	}

	IT_iClearScreen();
	
	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
	}
			
	return CSUDI_TRUE;
}


//@CASEGROUP: 1��PLAYER
//@CASEGROUP: 2��OSG
//@DESCRIPTION:���Բ���MPEG2��Ƶ��ͬʱ��Ӳ����ķ�ʽ����JPGͼƬ
//@PRECONDITION:1��ƽ̨�ɹ���ʼ��
//@PRECONDITION:2��֧��JPGͼƬӲ����
//@INPUT: 1��EM_IT_MPEG2_PAL
//@INPUT: 2��UDI2OSG_720_576.jpg
//@EXECUTIONFLOW:�����ܹ�����������Ƶ
//@EXECUTIONFLOW: 1������EM_IT_MPEG2_PALָ����Ƶ�㣬������Ƶ�ɹ�
//@EXECUTIONFLOW: 2������CSUDIPLAYEROpen����һ��������Ƶ�Ĳ����������������ɹ�
//@EXECUTIONFLOW: 3������CSUDIPLAYERSetStream���ý�Ŀ��Ƶ��ϢΪEM_IT_MPEG2_PALָ����Ϣ
//@EXECUTIONFLOW: 4������CSUDIPLAYERStart��ʼ������Ƶ,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5�������ܹ�����������Ƶ
//@EXECUTIONFLOW: 6�����²������JPGͼƬ������7-12���ʧ������ת����13
//@EXECUTIONFLOW: 7����ȡUDI2OSG_720_576.jpgͼƬ���ݵ��ڴ��ַ�ͳ���
//@EXECUTIONFLOW: 8������CSUDIOSGDecodeImage����ͼƬ��Ӧ�÷���CSUDI_SUCCESS������õ���phSurfaceӦ�ò�ΪCSUDI_NULL
//@EXECUTIONFLOW: 9������CSUDIOSGGetDisplaySurface�ӿڻ�ȡ�Դ�ռ�hDisSurface���
//@EXECUTIONFLOW: 10������CSUDIOSGBlit�ӿڽ�����õ���ͼƬ��Ϣblit���Դ�ռ�hDisSurface�ϣ�blit�ľ�������ΪCSUDI_NULL��
//@EXECUTIONFLOW: 11������CSUDIOSGSync���ͼƬ��blit�������ɹ�����CSUDI_SUCCESS��
//@EXECUTIONFLOW: 12�������JPGͼƬ�ɹ��������������ܿ���I֡ͼƬUDI2OSG_720_576.jpg�����ܹ�����������Ƶ
//@EXECUTIONFLOW: 13���ͷ�ͼƬ��Դ������surface�����ٲ�����
CSUDI_BOOL CSTC_ITTEST_IT_0023(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDI_HANDLE hSurface = CSUDI_NULL;
	CSUDI_HANDLE hDispSurface = CSUDI_NULL;
	CSUDI_HANDLE hHDDispSurface = CSUDI_NULL;
	CSUDIOSGSurfaceInfo_S sSurInfo;
	unsigned char * pcBuff = CSUDI_NULL;
    unsigned int nLen = 0;
	int nVideoIndex = 0;
	
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "��Ŀ����ʧ��");

	CSTCPrint("��Ƶ�������������� ?\n");
	CSTCPrint("Does the audio output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 5 check result failure!");

	if(IT_iIsSupportDecImage(EM_IT_DECODE_JPG))
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == IT_iTestDecImageWithScaleMode(EM_IT_DECODE_JPG,EM_IT_JPG_720_576,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface,&pcBuff,&nLen)),
				"step 7��8 decode jpeg image failure !"
			);

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hSurface, &sSurInfo)),
				"Get JPEG surface info failure !"
			);	
		
		if (!IT_iIsShareHDDisplaySurface())
		{
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDispSurface) && hDispSurface != CSUDI_NULL),
				"step 9 get display surface (sd) failure !"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hDispSurface, CSUDI_NULL, hSurface, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
				"step 10 blit (sd) failure !"
				);	
		}

		if(IT_iIsSupportHDDisplay())
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hHDDispSurface) && hHDDispSurface != CSUDI_NULL),
					"step 9 get display surface (hd) failure !"
					);

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIOSGBlit(hHDDispSurface, CSUDI_NULL, hSurface, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
					"step 10 blit (hd) failure !"
			        );	
		}	
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"step 11 osg sync  failure !"
			);

		CSUDIOSThreadSleep(50);
	
		CSTCPrint("���������ܿ���ȫ��JPEGͼƬUDI2OSG_720_576.jpg�����ܹ�����������Ƶ \n");
		CSTCPrint("We expect one JPEG UDI2OSG_720_576.jpg on screen for a while and get normal audio output at the same time\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 14 check result failure!");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "Not support JPG decoding!");
	}

	CSTK_FATAL_POINT
	
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer=CSUDI_NULL;
	}
	
	if (hSurface)
	{
		CSUDIOSGDestroySurface(hSurface);

		hSurface = CSUDI_NULL;
	}

	IT_iClearScreen();
	
	if(pcBuff != CSUDI_NULL)
	{
		CSUDIOSFree(pcBuff);
		pcBuff = CSUDI_NULL;
	}
			
	return CSUDI_TRUE;
}


