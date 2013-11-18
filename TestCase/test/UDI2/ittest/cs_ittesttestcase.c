/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
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

#define PCM_BUFFER_SIZE (1*1024*1024) //限制每次注入的大小
#define REPEAT_TIMES (3)
#define INJECTTHREADPRO (100)
#define INJECTTHREADBUF (1024 * 148)
#define INJECT_INVALID_PID (0x1fff)
static unsigned  int g_uItTestStartAddress = 0;
static CSUDI_BOOL s_exitinject = CSUDI_FALSE;

#define MAX_PESES_COUNT_IN_TS (EM_UDI_CONTENT_PCR+1) /*请参考CSUDIContentType_E枚举的类型个数*/

typedef struct _TSStreamInfo_S
{
    char * m_pcName; //注入文件的名
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
	EM_IT_MPEG2_PAL,                /*新加波亚洲新闻台 PAL*/
	EM_IT_MPEG2_NTSC,
	EM_IT_COORDINATE_SHIFT,              /*包含坐标偏移验证码流*/
	EM_IT_MAX	
}ITTestServiceIndex_E;
/*
*  测试用例中CSUDIPLAYERSetStream用到的各种类型的PID，
*  可以相或使用，如(EM_STREAM_VIDEO|EM_STREAM_AUDIO)同时设置音视频的pid
*/
typedef enum
{
	EM_IT_STREAM_VIDEO = 1<<0,   //视频
	EM_IT_STREAM_AUDIO = 1<<1,	  //音频
	EM_IT_STREAM_PCR   = 1<<2,   //pcr
	EM_IT_STREAM_SUBTITLE = 1<<3, //subtitle
	EM_IT_STREAM_TELETEXT = 1<<4  //teletext
}ITTestStreamType_E;

/**
* 测试用例中用到的节目相关的码流信息
*/
typedef struct 
{	
	const char*          m_pcTsFilename;  	    //码流文件名称
	const char* 		 m_pcServiceDescript; 	//码流描述
	int                  m_nVideoPid;           ///< 数据所在PID，-1表示不存在
	CSUDIVIDStreamType_E m_eVidStreamType;      ///视频类型
	int                  m_nAudioPid;           ///< 数据所在PID ，-1表示不存在
	CSUDIAUDStreamType_E m_eAudStreamType;      ///<音频类型
	int 				 m_nPcrPid;				///< PCR类型PID，-1表示不存在
	int 				 m_nSubPid;				///<SUBTITLE类型的PID，-1表示不存在
	int 				 m_nTelPid;				///<TELETEXT类型的PID，-1表示不存在
}ITTestServiceInfo_S;

static ITTestServiceInfo_S g_sITTestSeviceInfo[] ={
		{
			"SD_PAL_NTSC_and_HD_H.264_1080i_720P_DDplus_merge.ts",
			"新加波亚洲新闻台 PAL",
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
			"新加波亚洲新闻台 NTSC",
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
			"测试视频坐标偏移",
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
*各种图片资源名称枚举
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
* 图片解码支持类型枚举
*/
typedef enum
{
	EM_IT_DECODE_IFRAME,   //支持IFRAME图片硬解码
	EM_IT_DECODE_JPG,   //支持JPG图片硬解码
	EM_IT_DECODE_MAX 
}ITTestSupportDecIamgeType_E;

//图片硬解码图片资源数组
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
    char * m_pcFileName; //注入文件的名
    CSUDIINJECTERPcmStartParam_S m_sStartParam;//PCM播放参数
    CSUDIINJECTERPcmEndian_E m_eEndian;//文件数据的大小端字节序特性
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
//根据音频和视频解码器初始化相应的播放信息
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
	//在本测试用例集执行前调用
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
	//在本测试用例集执行后调用
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
* 从配置文件中获取锁测试的平台是否为高标清同源
* 返回CSUDI_TRUE表示是高标清同源，返回CSUDI_FALSE表示不是高标清同源
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
				CSTCPrint("没有找到支持的满足要求的颜色模式\n");
			}
		}
	}

	CSTCPrint("Find Pixelformat : %d (%d: ARGB1555, %d: ARGB8888, %d: wrong)\n", 
		*ePixelFormat, EM_UDIOSG_PIXEL_FORMAT_ARGB_1555, EM_UDIOSG_PIXEL_FORMAT_ARGB_8888, EM_UDIOSG_PIXEL_FORMAT_NUM);

	return bRet;
}

/**
* 将标清的矩形区域转化成对应的高清的值
* 该接口主要用于Video的矩形区域的转化
* 该接口主要用于高标清同源的平台的测试
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
* 将标清的矩形区域转化成对应的高清的值
* 该接口主要用于OSD的矩形区域的转化
* 该接口主要用于高标清同源的平台的测试
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
				"没有找到任何支持的满足要求的PixelFormat"
			);

	if (IT_iIsShareHDDisplaySurface())
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1, &hDisplay) && (CSUDI_NULL != hDisplay)),"获取高清显存失败");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDisplay) && (CSUDI_NULL != hDisplay)),"获取标清显存失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetSurfaceInfo(hDisplay, &sSurfaceInfo)), "获取显存信息失败");

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGCreateSurface(ePixelFormat, 
			sSurfaceInfo.m_nWidth, sSurfaceInfo.m_nHeight, &hSurface_S)),"创建平面失败");

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
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGDestroySurface(hSurface_S)),"销毁绘图平面失败");	
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
	    CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(0, &hDisplay) && (CSUDI_NULL != hDisplay)),"获取显存失败");
		/*<***********清屏操作直接用Fill *********Modified by lwh at 2010-12-09>*/
	    CSTK_ASSERT_TRUE_FATAL(
	                    (CSUDI_SUCCESS == CSUDIOSGFill(hDisplay, CSUDI_NULL, 0x0)),
	                    "Fill Display Failure !"
	                );  
	}

    if(bSupportHD)
    {
        CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGGetDisplaySurface(1,&hDisplay_HD)),
                    "获取高清显存失败!");
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
* param[in]: bIsSet 如果为CSUDI_TRUE表示调用CSUDIPLAYERSetStream来改变，反之调用CSUDIPLAYERModifyStream
* param[in]: eSerIndex 对应PlayerTestServiceIndex_E枚举类型中的定义
* param[in]: dwStreamType 为PlayerTestStreamType_E枚举类型的组合,例如:EM_IT_STREAM_VIDEO|EM_IT_STREAM_AUDIO
*/
static CSUDI_BOOL IT_iSetStream(CSUDI_BOOL bIsSet,CSUDI_HANDLE hPlayer,ITTestServiceIndex_E eSerIndex,unsigned int dwStreamType)
{
	CSUDIStreamInfo_S  sStreamInfo[10];  //不会超过10个
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
	CSTCPrint("正在播放 %s...... 节目\r\n",g_sITTestSeviceInfo[eSerIndex].m_pcServiceDescript);
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
			CSTCPrint("OSG_iTestDecImage 类型未知\n");
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
*获取图片内容
*/
static unsigned int  IT_iGetPhoto(const char *photoName, unsigned char **pBuff )
{
	CSUDI_HANDLE fp = CSUDI_NULL;
    unsigned int nIframeLen = 0;
	CSUDI_BOOL bOpen = CSUDI_FALSE;

	if(CSUDI_NULL != (fp = CSTKP_FOpen(photoName, "rb")))
	{
		CSTCPrint("图片%s打开成功\n ", photoName);
		bOpen = CSUDI_TRUE;
	}

	else
	{
		CSTCPrint("图片%s 打开失败\n", photoName);
		bOpen = CSUDI_FALSE;
	}

	if(bOpen)
	{
		if(0 == CSTKP_FSeek(fp, 0, CSTK_SEEK_END))
		{
			nIframeLen = CSTKP_FTell(fp);
			CSTCPrint("iframeLen = 0x%x\n", nIframeLen);
			
			if(nIframeLen > 0 && nIframeLen < 4*1920*1080 )       //图片大小合法性判断1920*1080分辨率ARGB8888 
			{
				*pBuff = CSUDIOSMalloc(nIframeLen); 
				//*pBuff = malloc(nIframeLen);
				if(*pBuff == CSUDI_NULL)
				{
					CSTCPrint("分配空间失败!\n");
					if(fp != CSUDI_NULL)
					{
						CSTKP_FClose(fp);
					}
					return 0;
				}
				memset(*pBuff, 0,nIframeLen);

		   		if(0 != CSTKP_FSeek(fp,  0, CSTK_SEEK_SET))
	         	{
					CSTCPrint("返回起始指针失败!\n");
					bOpen = CSUDI_FALSE;
		   		}
	
				if(nIframeLen != (CSTKP_FRead(*pBuff, sizeof(unsigned char),nIframeLen, fp)))
				{
					CSTCPrint("读%s文件失败!\n", photoName);
					bOpen = CSUDI_FALSE;
				}
			}
			else
			{
				CSTCPrint("图片大小超出正确范围，失败!\n");
				bOpen = CSUDI_FALSE;
			}
		}
	
		else
		{
			CSTCPrint("寻找图片%s 末指针失败!\n", photoName);
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
				ITTestSupportDecIamgeType_E eSupportType,/*要解码的图片类型*/
				ITTestUDI2ImageType_E eImageType,        /*图片类型索引*/
				CSUDIOSGScalingMode_E eScalingMode,      /*解码模式*/
				const CSUDIOSGRect_S * psDstRect,         /*解码输出区域*/
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
			CSTCPrint("IT_iTestDecImageWithScaleMode 类型未知\n");
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

//寻找视频解码器
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
测试I帧到来所用回调函数
*/
static int   s_nIttestTestdata = 0;
static void  fnIttestCallback1(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData)  
{
	if( eEvent==EM_UDIPLAYER_VIDEO_FRAME_COMING)
	{ 
		CSTCPrint("新帧到来,发出消息!!\n");
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
	sInjectChnl.m_nVideoDecoder = 0;  //I帧播放只需要VIDEO解码器

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_IFRAME;  //I帧为VIDEO类型
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_ES;				//I帧为ES数据类型
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
		CSTCPrint("回调消息错误!\n");
		return CSUDI_FALSE;
	}

	return CSUDI_TRUE;
}

//播放节目，如果节目播放失败则返回-1，成功则返回视频解码器的索引
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@CASEGROUP: 3、OSG
//@DESCRIPTION:测试单独播放MPEG2音频+播放I帧
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持I帧图片注入
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.mpg
//@EXPECTATION:期望能够正常播放音频的同时，播放I帧
//@EXECUTIONFLOW: 1、获取I帧图片UDI2OSG_720_576.mpg的数据，期望获取成功
//@EXECUTIONFLOW: 2、调用CSUDIINJECTEROpen创建一个ES注入器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIINJECTERStart开始注入I帧，期望注入成功
//@EXECUTIONFLOW: 4、调用CSUDIPLAYEROpen创建一个播放I帧的播放器，期望创建成功
//@EXECUTIONFLOW: 5、调用CSUDIPLAYERSetStream设置节目信息为I帧信息
//@EXECUTIONFLOW: 6、调用CSUDIPLAYERStart开始播放I帧,期望返回CSUDI_SUCCESS，并且播出I帧
//@EXECUTIONFLOW: 7、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 8、调用CSUDIPLAYEROpen创建一个播放音频的播放器，期望创建成功
//@EXECUTIONFLOW: 9、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERStart开始播放音频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 11、期望电视能看到I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频
//@EXECUTIONFLOW: 12、调用CSUDIPLAYERModifyStream设置音频信息为EM_IT_MPEG2_NTSC，期望设置成功
//@EXECUTIONFLOW: 13、期望电视能看到I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频
//@EXECUTIONFLOW: 14、释放图片资源，销毁surface，销毁注入器，销毁播放器
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
	sInjectChnl.m_nVideoDecoder = 0;  //I帧播放只需要VIDEO解码器

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_IFRAME;  //I帧为VIDEO类型
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_ES;				//I帧为ES数据类型
	
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
	
	CSTCPrint("期望电视能看到I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 \n");
    	CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the screen and to get normal audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 11 check result failure!");


	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_FALSE,hAudioPlayer,EM_IT_MPEG2_NTSC,EM_IT_STREAM_AUDIO),
			"step 12 set audio stream failure "
		);

	CSTCPrint("期望电视能看到I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 \n");
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@CASEGROUP: 3、OSG
//@DESCRIPTION:测试单独播放MPEG2音频+播放I帧+解码显示JPEG图片(显示层)
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持I帧图片注入
//@PRECONDITION:3、支持硬解码JPEG图片
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.mpg
//@INPUT: 3、UDI2OSG_720_576.jpg
//@EXPECTATION:期望能够正常播放音频的同时，播放I帧和显示JPEG图片
//@EXECUTIONFLOW: 1、获取I帧图片UDI2OSG_720_576.mpg的数据，期望获取成功
//@EXECUTIONFLOW: 2、调用CSUDIINJECTEROpen创建一个ES注入器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIINJECTERStart开始注入I帧，期望注入成功
//@EXECUTIONFLOW: 4、调用CSUDIPLAYEROpen创建一个播放I帧的播放器，期望创建成功
//@EXECUTIONFLOW: 5、调用CSUDIPLAYERSetStream设置节目信息为I帧信息
//@EXECUTIONFLOW: 6、调用CSUDIPLAYERStart开始播放I帧,期望返回CSUDI_SUCCESS，并且播出I帧
//@EXECUTIONFLOW: 7、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 8、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 9、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERStart开始播放音频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 11、调用CSUDIOSGDecodeImage解码指定JPEG图片UDI2OSG_720_576.jpg，解码模式为EM_UDIOSG_SCALE_FULLSIZE，期望解码成功
//@EXECUTIONFLOW: 12、调用CSUDIOSGGetDisplaySurface获取显存句柄，期望成功
//@EXECUTIONFLOW: 13、调用CSUDIOSGBlit将JPEG图片blit到显存的中央区域，期望操作成功
//@EXECUTIONFLOW: 14、期望电视能看到I帧图片UDI2OSG_720_576.mpg背景图片，能看到显示层的JPEG图片UDI2OSG_720_576.jpg,且能够正常播放音频
//@EXECUTIONFLOW: 15、调用CSUDIPLAYERModifyStream设置音频信息为EM_IT_MPEG2_NTSC，期望设置成功
//@EXECUTIONFLOW: 16、期望电视能看到I帧图片UDI2OSG_720_576.mpg背景图片，能看到显示层的JPEG图片UDI2OSG_720_576.jpg,且能够正常播放音频
//@EXECUTIONFLOW: 17、释放图片资源，销毁surface，销毁注入器，销毁播放器
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
	sInjectChnl.m_nVideoDecoder = 0;  //I帧播放只需要VIDEO解码器

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_IFRAME;  //I帧为VIDEO类型
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_ES;				//I帧为ES数据类型
	
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
		
		//屏幕中央
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
	
		CSTCPrint("期望电视能看到I帧背景图片，能看到显示层的JPEG图片，且能够正常播放音频 \n");
		CSTCPrint("We expect one Iframe on screen and one JPEG picture above the IFrame picture,"\
			"and get normal audio output at the same time\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 14 check result failure!");

		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_FALSE,hAudioPlayer,EM_IT_MPEG2_NTSC,EM_IT_STREAM_AUDIO),
			"step 15 set stream failure "
		);

		CSTCPrint("期望电视能看到I帧背景图片，能看到显示层的JPEG图片，且能够正常播放音频 \n");
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
    	CSTCPrint("测试所需文件 %s 打开失败，请确保testdata目录下存在该文件!!!\n", pFileName);
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
    long nFileAllLength = 0;  //文件总长度
    int nFileCurrentPos = 0; //文件当前位置
    int nFileLeftLength = 0; //剩余未读文件长度
    int nInjectedLength = 0; //可注入的大小
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
        //求出当前剩余未读文件的长度
        
        nFileCurrentPos=InjectFSTell(file); //当前已读文件的长度
        nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度
    
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
    
            if(nReadLength < uLength) //剩余文件长度小于可注入空间的长度
            {
                nInjectedLength = nReadLength;
            }
            else  //剩余文件长度大于等于可注入空间的长度
            {
                nInjectedLength = uLength;
            }
        
            nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //读取相应长度的文件
            
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

//查找支持nAudioType类型的Audio Decoder
//nAudioType参见CSUDIAUDStreamType_E, 类型为EM_UDI_AUD_STREAM_AC3特殊处理
static int searchAudioID_Y_InInject(int nAudioType)
{
	CSUDIAUDStreamType_E  eAudStreamTypeNum=EM_UDI_AUD_STREAMTYPE_NUM;
	CSUDIAUDIOCapability_S   sAudioCapabilityInfo;
	int nAudioCount = 0;
	int nAudioID = -1;
	int i = 0;
	int j = 0;
	
	if (CSUDI_SUCCESS == CSUDIAUDIOGetCount(&nAudioCount))           //先取得音频解码器的数量
	{
		for (i=0; i<nAudioCount; i++)
		{
			
			if(CSUDI_SUCCESS == CSUDIAUDIOGetCapability(i, &sAudioCapabilityInfo))           //到相关ID的音频解码器的能力
			{
				if(nAudioType == EM_UDI_AUD_STREAM_AC3)
				{
					for (j=0; j<eAudStreamTypeNum; j++)                                //其中能力包含两方面：支持解码的能力与支持直接输出的能力
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
					for (j=0; j<eAudStreamTypeNum; j++)                                //其中能力包含两方面：支持解码的能力与支持直接输出的能力
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




//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@CASEGROUP: 3、OSG
//@DESCRIPTION:测试播放MPEG2视频的同时播放PCM音频数据
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、平台支持PCM数据注入
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2_TEST_PCM.wav
//@EXECUTIONFLOW:期望能够正常播放音视频
//@EXECUTIONFLOW: 1、调用CSUDIINJECTEROpen创建一个PCM注入器，期望创建成功
//@EXECUTIONFLOW: 2、设置CSUDIINJECTERSetAttribute设置pcm播放属性，期望设置成功
//@EXECUTIONFLOW: 3、调用CSUDIINJECTERStart开始注入PCM数据，期望注入成功
//@EXECUTIONFLOW: 4、调用CSUDIPLAYEROpen创建一个播放PCM播放器，期望创建成功
//@EXECUTIONFLOW: 5、调用CSUDIPLAYERSetStream设置节目信息为PCM信息
//@EXECUTIONFLOW: 6、调用CSUDIPLAYERStart开始播放PCM,期望返回CSUDI_SUCCESS，并且播出音频
//@EXECUTIONFLOW: 7、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 8、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 9、调用CSUDIPLAYERSetStream设置节目视频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERStart开始播放视频和PCM音频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 11、期望能够正常播放音视频
//@EXECUTIONFLOW: 12、调用CSUDIPLAYERModifyStream设置视频信息为EM_IT_MPEG2_NTSC，期望设置成功
//@EXECUTIONFLOW: 13、期望能够正常播放音视频
//@EXECUTIONFLOW: 14、销毁注入器和播放器
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

	sInjectChnl.m_nAudioDecoder = nAudioDecoderId; //PCM数据播放只要音频解码器
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
	
	CSTCPrint("期望能够正常播放音频和视频，且音频来自 %s\n", sThreadParam.pcFile);
    CSTCPrint("We expect normal A/V output,and the audio is from PCM file!\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 11 check result failure!");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_FALSE,hVideoPlayer,EM_IT_MPEG2_NTSC,EM_IT_STREAM_VIDEO),
			"step 12 modify video stream failure "
		);
	
	CSTCPrint("期望能够正常播放音频和视频 ，且音频来自 %s\n", sThreadParam.pcFile);
    CSTCPrint("We expect normal A/V output,and the audio is from PCM file!\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 13 check result failure!");
	
	CSTK_FATAL_POINT
		
	g_bInjectPCMFinish = CSUDI_TRUE;    //停止注入线程

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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@CASEGROUP: 3、OSG
//@DESCRIPTION:测试MPEG2音视频的同时播放JEPG图片
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持解码JPEG图片
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.jpg
//@EXPECTATION:期望能够正常播放音视频的同时，显示JPEG图片
//@EXECUTIONFLOW: 1、调用CSUDIOSGDecodeImage解码指定JPEG图片UDI2OSG_720_576.jpg，解码模式为EM_UDIOSG_SCALE_FULLSIZE，期望解码成功
//@EXECUTIONFLOW: 2、调用CSUDIOSGBlit把JPEG图片显示到显存的中央区域，期望能够看到图片UDI2OSG_720_576.jpg
//@EXECUTIONFLOW: 3、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 4、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 5、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 6、调用CSUDIPLAYERStart开始播放音视频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、期望电视能看到UDI2OSG_720_576.jpg图片，且能够正常播放音视频
//@EXECUTIONFLOW: 8、调用CSUDIPLAYERModifyStream设置音频信息为EM_IT_MPEG2_NTSC，期望设置成功
//@EXECUTIONFLOW: 9、期望电视能看到UDI2OSG_720_576.jpg图片，且能够正常播放音视频
//@EXECUTIONFLOW: 10、释放图片资源，销毁surface，销毁注入器，销毁播放器
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

		//屏幕中央
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
		
		CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGBlit(hDisplay, &sShowRect, hJPEGSurface, CSUDI_NULL, EM_UDIOSG_OPMODE_COPY)),
				"Blit jpeg failure !"
			);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSGSync()),
				"sync  failure !"
			);
		
		CSTCPrint("期望电视标清输出能看到JPEG图片UDI2OSG_720_576.jpg，且能够正常播放音视频 \n");
        CSTCPrint("We expect the JPEG UDI2OSG_720_576.jpg picture on the SD screen and get normal A/V output\n");
        CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 8 check result failure!");

		CSTK_ASSERT_TRUE_FATAL(
				CSUDI_TRUE  == IT_iSetStream(CSUDI_FALSE,hPlayer,EM_IT_MPEG2_NTSC,EM_IT_STREAM_AUDIO),
				"step 9 set stream failure "
			);

		CSTCPrint("期望电视标清输出能看到JPEG图片UDI2OSG_720_576.jpg，且能够正常播放音视频 \n");    
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@DESCRIPTION:测试播放MPEG2音频的同时播放I帧图片
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持I帧图片注入
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:期望能够正常播放音视频，(没有空闲的视频解码器期望I帧解码期望返回CSUDIOSG_ERROR_DEVICE_BUSY)
//@EXECUTIONFLOW: 1、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 2、调用CSUDIPLAYEROpen创建一个播放音频的播放器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStart开始播放音频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、期望能够正常播放音频
//@EXECUTIONFLOW: 6、以下步骤播放I帧，步骤7-12如果失败则跳转步骤13
//@EXECUTIONFLOW: 7、调用IT_FindVideoDecoder寻找视频解码器，成功则返回视频解码器设备索引号
//@EXECUTIONFLOW: 8、调用CSUDIINJECTEROpen创建一个ES注入器，期望创建成功
//@EXECUTIONFLOW: 9、调用CSUDIINJECTERStart开始注入I帧，期望注入成功
//@EXECUTIONFLOW: 10、调用CSUDIPLAYEROpen创建一个播放I帧的播放器，期望创建成功
//@EXECUTIONFLOW: 11、调用CSUDIPLAYERSetStream设置节目信息为I帧信息
//@EXECUTIONFLOW: 12、调用CSUDIPLAYERStart开始播放I帧,期望返回CSUDI_SUCCESS，并且播出I帧
//@EXECUTIONFLOW: 13、如播放I帧成功，则期望电视能看到I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频；如播放I帧失败，则期望能够正常播放音频和视频，无I帧图片UDI2OSG_720_576.mpg显示
//@EXECUTIONFLOW: 14、释放图片资源，销毁surface，销毁注入器，销毁播放器
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");

	CSTCPrint("期望能够正常播放音频\n");
	CSTCPrint("We expect normal A output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 5 check result failure!");

	bCheckIframe = IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);
	if(bCheckIframe)
	{
		CSTCPrint("期望电视能看到I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 \n");
		CSTCPrint("If could,We expect IFrame UDI2OSG_720_576.mpg on screen for a while and then get normal A/V output..\n");
	}
	else
	{
		CSTCPrint("期望能够正常播放音频和视频，无I帧图片UDI2OSG_720_576.mpg显示\n");
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@DESCRIPTION:测试实时播放下小窗口I帧和全屏I帧切换
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持I帧图片注入
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:期望能够正常播放音频，小窗口I帧和全屏I帧切换不会引起花屏等非正常现象
//@EXECUTIONFLOW: 1、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 2、调用CSUDIPLAYEROpen创建一个播放音频的播放器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStart开始播放音频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用IT_FindVideoDecoder寻找视频解码器，成功则返回视频解码器设备索引号
//@EXECUTIONFLOW: 6、调用CSUDIINJECTEROpen创建一个ES注入器，期望创建成功
//@EXECUTIONFLOW: 7、调用CSUDIINJECTERStart开始注入I帧，期望注入成功
//@EXECUTIONFLOW: 8、调用CSUDIPLAYEROpen创建一个播放I帧的播放器，期望创建成功
//@EXECUTIONFLOW: 9、调用CSUDIPLAYERSetStream设置节目信息为I帧信息
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERStart开始播放I帧,期望返回CSUDI_SUCCESS，并且播出I帧
//@EXECUTIONFLOW: 11、期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 
//@EXECUTIONFLOW: 12、调用CSUDIVIDEOSetWindowSize函数设置设置小窗口播放,窗口在屏幕中央,长宽均为200
//@EXECUTIONFLOW: 13、期望电视中央能看到小窗口I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 
//@EXECUTIONFLOW: 14、调用CSUDIVIDEOSetWindowSize函数设置全屏播放
//@EXECUTIONFLOW: 15、期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 
//@EXECUTIONFLOW: 16、释放图片资源，销毁surface，销毁播放器
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");
	
	CSTCPrint("期望能够正常播放音频(没有视频)\n");
    CSTCPrint("We expect normal audio output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 4 check result failure!");

	IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);

	CSTCPrint("期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the full screen and to get normal audio output\n");
    CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 11 check result failure!");
	
	//高标清同源只在高清输出上设置
	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iWinRect_SDTransformHD(&stRect);
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, &stRect);		
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, &stRect);		
	}
	CSTCPrint("期望电视标清输出中央能看到小窗口I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 \n");
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
	
	CSTCPrint("期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 \n");
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@DESCRIPTION:测试I帧与实时播放切换,小窗口I帧和全屏I帧切换
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持I帧图片注入
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:期望能够正常播放音视频，小窗口I帧和全屏I帧切换不会引起花屏等非正常现象
//@EXECUTIONFLOW: 1、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 2、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStart开始播放音视频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、期望能够正常播放音视频
//@EXECUTIONFLOW: 6、停止节目播放
//@EXECUTIONFLOW: 7、调用IT_FindVideoDecoder寻找视频解码器，成功则返回视频解码器设备索引号
//@EXECUTIONFLOW: 8、调用CSUDIINJECTEROpen创建一个ES注入器，期望创建成功
//@EXECUTIONFLOW: 9、调用CSUDIINJECTERStart开始注入I帧，期望注入成功
//@EXECUTIONFLOW: 10、调用CSUDIPLAYEROpen创建一个播放I帧的播放器，期望创建成功
//@EXECUTIONFLOW: 11、调用CSUDIPLAYERSetStream设置节目信息为I帧信息
//@EXECUTIONFLOW: 12、调用CSUDIPLAYERStart开始播放I帧,期望返回CSUDI_SUCCESS，并且播出I帧
//@EXECUTIONFLOW: 13、期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 14、调用CSUDIVIDEOSetWindowSize函数设置小窗口播放
//@EXECUTIONFLOW: 15、期望电视能看到小屏幕I帧图片UDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 16、调用CSUDIVIDEOSetWindowSize函数设置全屏播放
//@EXECUTIONFLOW: 17、期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 18、调用CSUDIVIDEOSetWindowSize函数设置小窗口播放
//@EXECUTIONFLOW: 19、期望电视能看到小屏幕I帧图片UDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 20、销毁surface，销毁注入器，销毁播放器
//@EXECUTIONFLOW: 21、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 22、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 23、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 24、调用CSUDIPLAYERStart开始播放音视频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 25、期望能够看到小屏幕播放音视频
//@EXECUTIONFLOW: 26、调用CSUDIVIDEOSetWindowSize函数设置全屏播放
//@EXECUTIONFLOW: 27、期望能够正常全屏播放音视频
//@EXECUTIONFLOW: 28、释放图片资源，销毁surface，销毁注入器，销毁播放器
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");
	
	CSTCPrint("期望能够正常播放音频和视频\n");
    	CSTCPrint("We expect normal A/V output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 5 check result failure!");
	
	if (hAVPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hAVPlayer), "step 6 close AVPlayer failure!");
		hAVPlayer = CSUDI_NULL;
	}

	IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);

	CSTCPrint("期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the full screen\n");
    CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 13 check result failure!");

	//高标清同源只在高清输出上设置
	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iWinRect_SDTransformHD(&stRect);
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, &stRect);		
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, &stRect);		
	}
	
	CSTCPrint("期望电视标清输出能看到小窗口I帧图片UDI2OSG_720_576.mpg \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg A/V output on the screen in a small size window \n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 15 check result failure!");

	//高标清同源只在高清输出上设置
	if (IT_iIsShareHDDisplaySurface())
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);	
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);		
	}
		
	CSTCPrint("期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg \n");
    CSTCPrint("We expect the Iframe UDI2OSG_720_576.mpg on the full screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 17 check result failure!");

	//高标清同源只在高清输出上设置
	if (IT_iIsShareHDDisplaySurface())
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, &stRect);		
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, &stRect);		
	}
		
	CSTCPrint("期望电视标清输出能看到小窗口I帧图片UDI2OSG_720_576.mpg \n");
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");

	CSTCPrint("期望电视标清输出中央小屏幕能够正常播放视频，且音频正常\n");
    CSTCPrint("We expect normal A/V output on the screen in a small size window \n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 25 check result failure!");		

	//高标清同源只在高清输出上设置
	if (IT_iIsShareHDDisplaySurface())
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_HD, CSUDI_NULL);	
	}
	else
	{
		CSUDIVIDEOSetWindowSize(nVideoIndex, EM_UDI_VOUT_DEVICE_SD, CSUDI_NULL);		
	}
	
	CSTCPrint("期望能够全屏幕播放音频和视频\n");
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、OSG
//@DESCRIPTION:测试节目播放时,设置OSD透明度
//@PRECONDITION:1、平台成功初始化
//@INPUT: 1、EM_IT_MPEG2_PAL
//@EXPECTATION:期望能够正常播放音视频的同时，调用CSUDISCREENSetOSDTransparency可以看到OSD透明度的变化
//@EXECUTIONFLOW: 1、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 2、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStart开始播放音视频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、清屏，且能够正常播放音视频
//@EXECUTIONFLOW: 6、调用CSUDIOSGGetDisplaySurface获取显示surface
//@EXECUTIONFLOW: 7、调用CSUDIOSGFill在屏幕中画一个红色矩形
//@EXECUTIONFLOW: 8、调用CSUDISCREENSetOSDTransparency设置透明度为100
//@EXECUTIONFLOW: 9、期望可以看到屏幕中有一个不透明的红块,并且音视频正常播放
//@EXECUTIONFLOW: 10、调用CSUDISCREENSetOSDTransparency设置透明度为50
//@EXECUTIONFLOW: 11、期望可以看到屏幕中有一个半透明的红块,并且音视频正常播放
//@EXECUTIONFLOW: 12、调用CSUDISCREENSetOSDTransparency,设置OSD透明度为0
//@EXECUTIONFLOW: 13、期望音视频正常播放,电视屏幕上只有实时节目,没有红色方块
//@EXECUTIONFLOW: 14、调用CSUDISCREENSetOSDTranspare
CSUDI_BOOL CSTC_ITTEST_IT_0008(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDIOSGRect_S asDRect  =  ITTEST_SMALLWINDOW_RECT;
	int nVideoIndex = 0;
	
	//avplay
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");

	CSTCPrint("期望能够正常播放音视频\n");
	CSTCPrint("We expect normal A/V output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "Check result failure!");
	
	//画红块
	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iOSGRect_SDTransformHD(&asDRect);
	}
	IT_iOSGFill(&asDRect, ITTEST_COLOR_RED_32BIT);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

	CSUDISCREENSetOSDTransparency(100);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTCPrint("期望电视能看到一个不透明的红块，且能够正常播放音视频 \n");
	CSTCPrint("We expect normal A/V output and a small red OSD square which is not transparent in the middle screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSUDISCREENSetOSDTransparency(50);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");	

	CSTCPrint("期望电视能看到一个半透明的红块(红块与视频图像叠加)，且能够正常播放音视频 \n");
	CSTCPrint("We expect normal A/V output and a small red OSD square which is half transparent in the middle of screen\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");
		
	CSUDISCREENSetOSDTransparency(0);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");	

	CSTCPrint("期望音视频正常播放,电视屏幕上只有实时节目,没有红色方块\n");
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、OSG
//@DESCRIPTION:测试节目播放时,OSD全局透明度与局部像素点透明度的关系
//@PRECONDITION:1、平台成功初始化
//@INPUT: 1、EM_IT_MPEG2_PAL
//@EXPECTATION:CSUDISCREENSetOSDTransparency接口设置的全局与局部像素点的透明度接口之间的组合关系相关测试用例。如全局透明度为不透明，但像素点透明度为全透明，这时的效果应为全透明。
//@EXECUTIONFLOW:1、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW:2、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW:3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart开始播放音视频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、期望音视频正常播放
//@EXECUTIONFLOW:6、调用CSUDISCREENSetOSDTransparency设置透明度为100
//@EXECUTIONFLOW:7、调用CSUDIOSGGetDisplaySurface获取显示Surface
//@EXECUTIONFLOW:8、调用CSUDIOSGFill进行清屏
//@EXECUTIONFLOW:9、调用CSUDIOSGFill画一个红色不透明矩形{160,88,400,400}
//@EXECUTIONFLOW:10、调用CSUDIOSGFill画一个绿色半透明矩形{210,138,300,300}
//@EXECUTIONFLOW:11、调用CSUDIOSGFill画一个蓝色透明矩形{260,188,200,200}
//@EXECUTIONFLOW:12、调用CSUDIOSGSync
//@EXECUTIONFLOW:13、期望音视频播放正常,显示外部红色不透明方框,中间绿色半透明方框,中间全透明
//@EXECUTIONFLOW:14、调用CSUDISCREENSetOSDTransparency,设置OSD透明度为50
//@EXECUTIONFLOW:15、期望音视频播放正常,显示外部红色不透明方框和中间绿色半透明方框全为半透明,中间全透明
//@EXECUTIONFLOW:16、调用CSUDISCREENSetOSDTransparency,设置OSD透明度为0
//@EXECUTIONFLOW:17、期望音视频播放正常,没有OSD显示
//@EXECUTIONFLOW:18、调用CSUDISCREENSetOSDTransparency,设置OSD透明度为100
//@EXECUTIONFLOW:19、清屏,销毁播放器
CSUDI_BOOL CSTC_ITTEST_IT_0009(void)
{
	CSUDI_HANDLE hAVPlayer = CSUDI_NULL;
	CSUDIOSGRect_S asOutRect  = ITTEST_OUT_RECT;
	CSUDIOSGRect_S asCenterRect  = ITTEST_CENTER_RECT;
	CSUDIOSGRect_S asInRect  = ITTEST_IN_RECT;
	int nVideoIndex = 0;
	
	//avPlayer
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");

	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iOSGRect_SDTransformHD(&asOutRect);
		IT_iOSGRect_SDTransformHD(&asCenterRect);
		IT_iOSGRect_SDTransformHD(&asInRect);
	}

	//红色不透明矩形
	IT_iOSGFill(&asOutRect, ITTEST_COLOR_RED_32BIT);	
	//绿色透明矩形
	IT_iOSGFill( &asCenterRect, ITTEST_COLOR_GREEN_32BIT);
	//蓝色透明矩形
	IT_iOSGFill(  &asInRect, ITTEST_COLOR_BLUE_32BIT);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");

	CSTCPrint("1.期望音视频可以正常播放\n");
	CSTCPrint("2.期望外部红色方框不透明，红色方框中看不到视频播放\n");
	CSTCPrint("3.期望中间绿色方框半透明，可以看到绿色方框与视频图像叠加\n");
	CSTCPrint("4.期望最里面的方块完全透明 ，可以正常看到视频图像\n");
	CSTCPrint("1.We expect normal A/V output\n");
	CSTCPrint("2.We expect the most outside red OSD square not transparent on the screen and can't see video under the red OSD square\n");
	CSTCPrint("3.We expect the middle green OSD square half transparent on the screen and can see video overlap with  the green OSD square\n");
	CSTCPrint("4.We expect the most inner  OSD square totally transparent on the screen and can see video normally in the middle\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSUDISCREENSetOSDTransparency(50);//服从全局
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTCPrint("1.期望音视频可以正常播放\n");
	CSTCPrint("2.期望外部红色方框半透明\n");
	CSTCPrint("3.期望中间绿色方框半透明\n");
	CSTCPrint("4.期望最里面的方块完全透明，可以正常看到视频图像\n");
	CSTCPrint("1.We expect normal A/V output\n");
	CSTCPrint("2.We expect the most outside red OSD square half transparent on the screen and can see video overlap with  the red OSD square\n");
	CSTCPrint("3.We expect the middle green OSD square half transparent on the screen and can see video overlap with  the green OSD square\n");
	CSTCPrint("4.We expect the most inner  OSD square totally transparent on the screen and can see video normally in the middle\n");

	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "check result failure!");

	CSUDISCREENSetOSDTransparency(0);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIOSGSync()),"Sync Failure !");
	
	CSTCPrint("期望音视频正常播放,且没有OSD显示\n");
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、Stanby
//@CASEGROUP: 3、Wakeup
//@CASEGROUP: 3、OSG
//@DESCRIPTION:测试在节目播放,节目停止播放,显示I帧情况下Standby和Wakeup的情况
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持解码JPEG图片
//@PRECONDITION:3、支持节目播放
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.jpg
//@EXPECTATION:期望在各种状态下,Standby和Wakeup都是正常的.
//@EXECUTIONFLOW: 1、调用CSUDIOSGDecodeImage解码指定JPEG图片UDI2OSG_720_576.jpg，解码模式为EM_UDIOSG_SCALE_FULLSIZE，期望解码成功
//@EXECUTIONFLOW: 2、调用CSUDIOSGBlit把JPEG图片显示到显存，期望能够看到图片UDI2OSG_720_576.jpg
//@EXECUTIONFLOW: 3、调用CSUDISYSStandby,期望可以进入待机状态
//@EXECUTIONFLOW: 4、待机10秒后调用CSUDISYSStandby,期望唤醒待机状态,并可以看到图片
//@EXECUTIONFLOW: 5、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 6、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 7、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 8、调用CSUDIPLAYERStart开始播放音视频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、期望电视能看到UDI2OSG_720_576.jpg图片，且能够正常播放音视频
//@EXECUTIONFLOW: 10、调用CSUDISYSStandby,期望可以进入待机状态
//@EXECUTIONFLOW: 11、待机10秒后调用CSUDISYSStandby,期望唤醒待机状态,并可以看到图片,且能够正常播放音视频
//@EXECUTIONFLOW: 12、期望电视能看到UDI2OSG_720_576.jpg图片，且能够正常播放音视频
//@EXECUTIONFLOW: 13、释放图片资源，销毁surface，销毁注入器，销毁播放器
CSUDI_BOOL CSTC_ITTEST_IT_0010(void)
{
//尚未完?
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

	CSTCPrint("期望能够正常播放音视频\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "heck result failure!");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_TRUE,20000,&bReal),"第一步测试用例失败");		

	if(bReal == CSUDI_FALSE)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_FALSE,20000,&bReal),"第四步退出假待机测试用例失败");		
	}	
	else
	{
		CSTCPrint("\n********************测试成功******************************\n");	
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@CASEGROUP: 3、OSG
//@DESCRIPTION:测试I帧在各种情况下的功能实现:I帧显示,I帧间切换,I帧+广播,I帧+广播切换到实时音视频,实时音视频切换到I帧+广播,I帧+广播状态下窗口大小的切换
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持I帧图片注入
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:期望I帧在各种情况下正常显示
//@EXECUTIONFLOW: 1、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 2、调用CSUDIPLAYEROpen创建一个播放音频的播放器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStart开始播放音频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSTKP_FOpen,CSTKP_FRead等函数打开UDI2OSG_720_576.mpgI帧图片
//@EXECUTIONFLOW: 6、调用CSUDIINJECTEROpen创建一个ES注入器，期望创建成功
//@EXECUTIONFLOW: 7、调用CSUDIINJECTERSetAttribute设置ES注入器的属性,注入I帧为UDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 8、调用CSUDIINJECTERStart开始注入I帧，期望注入成功
//@EXECUTIONFLOW: 9、调用CSUDIPLAYEROpen创建一个播放I帧的播放器，期望创建成功
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERAddPlayerCallback添加新帧事件回调，期望成功
//@EXECUTIONFLOW: 11、调用CSUDIPLAYERSetStream设置节目信息为I帧信息
//@EXECUTIONFLOW: 12、调用CSUDIPLAYERStart开始播放I帧,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 13、期望可以看到全屏I帧图片UDI2OSG_720_576.mpg，并且能听到音频
//@EXECUTIONFLOW: 14、调用CSUDIPLAYERClose关掉播放器
//@EXECUTIONFLOW: 15、调用CSUDIINJECTERClose关掉注入
//@EXECUTIONFLOW: 16、调用CSTKP_FOpen,CSTKP_FRead等函数打开UDI2OSG_720_576_2.mpgI帧图片
//@EXECUTIONFLOW: 17、调用CSUDIINJECTEROpen创建一个ES注入器，期望创建成功
//@EXECUTIONFLOW: 18、调用CSUDIINJECTERSetAttribute设置ES注入器的属性,注入I帧为UDI2OSG_720_576_2.mpg
//@EXECUTIONFLOW: 19、调用CSUDIINJECTERStart开始注入I帧，期望注入成功
//@EXECUTIONFLOW: 20、调用CSUDIPLAYEROpen创建一个播放I帧的播放器，期望创建成功
//@EXECUTIONFLOW: 21、调用CSUDIPLAYERAddPlayerCallback添加新帧事件回调，期望成功
//@EXECUTIONFLOW: 22、调用CSUDIPLAYERSetStream设置节目信息为I帧信息
//@EXECUTIONFLOW: 23、调用CSUDIPLAYERStart开始播放I帧,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 24、期望可以看到全屏I帧图片UDI2OSG_720_576_2.mpg，并且能听到音频
//@EXECUTIONFLOW: 25、调用CSUDIOSGFill在屏幕中间画一个200*200的半透明红色矩形
//@EXECUTIONFLOW: 26、调用CSUDIOSGFill在屏幕中间画一个150*150的全透明矩形
//@EXECUTIONFLOW: 27、调用CSUDIOSGSync进行同步
//@EXECUTIONFLOW: 28、调用CSUDIVIDEOSetWindowSize设置视频窗口大小是150*150,位置在屏幕中央
//@EXECUTIONFLOW: 29、调用CSUDIOSGSync进行同步
//@EXECUTIONFLOW: 30、期望I帧可以在显示在屏幕中间150*150区域,并且I帧显示不会与半透明红色方框叠加显示
//@EXECUTIONFLOW: 31、调用CSUDIVIDEOSetWindowSize设置视频窗口为全屏
//@EXECUTIONFLOW: 32、期望I帧可以全屏显示,并且可以听到音频
//@EXECUTIONFLOW: 33、调用CSUDIVIDEOSetWindowSize设置视频窗口大小是150*150,位置在屏幕中央
//@EXECUTIONFLOW: 34、期望I帧可以在显示在屏幕中间150*150区域,并且I帧显示不会与半透明红色方框叠加显示
//@EXECUTIONFLOW: 35、调用CSUDIPLAYERClose停止I帧播放
//@EXECUTIONFLOW: 36、调用CSUDIINJECTERClose停止注入
//@EXECUTIONFLOW: 37、调用CSUDIPLAYERClose关掉音频播放器
//@EXECUTIONFLOW: 38、清除OSD显示,期望屏幕没有半透明红色方框
//@EXECUTIONFLOW: 39、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 40、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 41、调用CSUDIPLAYERSetStream设置节目音视频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 42、调用CSUDIPLAYERStart开始播放音视频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 43、期望可以在屏幕中间150*150区域看到实时音视频
//@EXECUTIONFLOW: 44、释放图片资源，销毁surface，销毁注入器,销毁播放器
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");
	
	IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);

	CSTCPrint("期望电视屏幕能看到全屏I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频 \n");
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
		
	CSTCPrint("期望电视屏幕能看到全屏I帧图片UDI2OSG_720_576_2.mpg，且能够正常播放音频 \n");	
	CSTCPrint("We expect the Iframe UDI2OSG_720_576_2.mpg on the screen and to get normal audio output\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 24 check result failure!");

	if (IT_iIsShareHDDisplaySurface())
	{
		IT_iOSGRect_SDTransformHD(&stRect);
		IT_iOSGRect_SDTransformHD(&stInRect);
		IT_iWinRect_SDTransformHD(&stWinRect);
	}

	//红色半透明矩形	
	IT_iOSGFill(&stRect, ITTEST_COLOR_RED_TRANSPARENT_32BIT);
	//全透明矩形
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
	
	CSTCPrint("1.期望能够正常播放音频 \n");		
	CSTCPrint("2.期望电视标清输出屏幕中央能看到小窗口I帧图片UDI2OSG_720_576_2.mpg，且能够正常播放音频 \n");				
	CSTCPrint("3.期望屏幕中间有个半透明红色边框\n");				
	CSTCPrint("4.期望红色边框正好包围住I帧图片,且没有叠加显示\n");	
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

	CSTCPrint("1.期望能够正常播放音频 \n");		
	CSTCPrint("2.期望I帧图片UDI2OSG_720_576_2.mpg全屏显示 \n");				
	CSTCPrint("3.期望屏幕中间有个半透明红色边框\n");
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

	CSTCPrint("1.期望能够正常播放音频 \n");		
	CSTCPrint("2.期望电视标清输出屏幕中央能看到小窗口I帧图片UDI2OSG_720_576_2.mpg，且能够正常播放音频 \n");				
	CSTCPrint("3.期望屏幕中间有个半透明红色边框\n");				
	CSTCPrint("4.期望红色边框正好包围住I帧图片,且没有叠加显示\n");	
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");
	
	CSTCPrint("期望在标清输出上能够看见屏幕中央小窗口实时节目的播放\n");
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

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@DESCRIPTION:测试解码I帧时回调消息的正确性
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持I帧图片注入
//@INPUT: 1、UDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 1、调用IT_FindVideoDecoder寻找视频解码器，成功则返回视频解码器设备索引号
//@EXECUTIONFLOW: 2、调用CSUDIINJECTEROpen创建一个ES注入器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIINJECTERStart开始注入I帧，期望注入成功
//@EXECUTIONFLOW: 4、调用CSUDIPLAYEROpen创建一个播放I帧的播放器，期望创建成功
//@EXECUTIONFLOW: 5、调用CSUDIPLAYERAddPlayerCallback添加回调函数，事件为EM_UDIPLAYER_VIDEO_FRAME_COMING，设置消息s_nIttestTestdata
//@EXECUTIONFLOW: 6、调用CSUDIPLAYERSetStream设置节目信息为I帧信息
//@EXECUTIONFLOW: 7、调用CSUDIPLAYERStart开始播放I帧,期望返回CSUDI_SUCCESS，并且播出I帧
//@EXECUTIONFLOW: 8、期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg
//@EXECUTIONFLOW: 9、释放图片资源，销毁surface，销毁注入器，销毁播放器
CSUDI_BOOL CSTC_ITTEST_IT_0012(void)
{	
	CSUDI_HANDLE hIFramePlayer = CSUDI_NULL;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectIFrameThread = CSUDI_NULL;		
	unsigned char * pcBuff = CSUDI_NULL;
	
	IT_iPlayIFrame(EM_IT_IFRAME_720_576,&hIFramePlayer,&hINJECTER,&hInjectIFrameThread,&pcBuff);

	CSTCPrint("期望电视能看到全屏I帧图片UDI2OSG_720_576.mpg \n");
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
	CSTCPrint("\n*************** 申请到的数据为%02x\n",pSection->m_pucData[0]);
   	return;
}

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、INJECT
//@DESCRIPTION:测试应用通过section模块去申请一个已经由av模块分配（占用）的pid时，不应该影响正常的播放流程！
//@PRECONDITION:平台成功初始化
//@INPUT: EM_IT_MPEG2_PAL
//@EXPECTATION:期望能够正常播放音视频的同时，分配相同的pid/channel，启动filer，再释放filter，不管成功与否，不影响节目播放  
//@EXECUTIONFLOW: 1、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 2、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStart开始播放音视频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、期望能够正常播放音视频
//@EXECUTIONFLOW: 6、为fliter分配与节目相同的pid，调用CSUDIFILTERAllocate请求fliter过滤数据
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStart开始过滤
//@EXECUTIONFLOW: 8、调用CSUDIFILTERFree  停止Filter接收数据,并释放Filter资源
//@EXECUTIONFLOW: 9、期望音视频正常播放不受影响
//@EXECUTIONFLOW: 10、调用CSUDIPLAYERStop 停止播放节目
//@EXECUTIONFLOW: 11、调用CSUDIPLAYERClose 关闭播放器
//@EXECUTIONFLOW: 12、重复步骤2_4，再次播放节目，期望播放音视频成功
//@EXECUTIONFLOW: 13、清屏，销毁播放器
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");
	
	CSTCPrint("期望能够正常播放音视频\n");
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
	
	CSTCPrint("期望音视频依然正常播放\n");
	CSTCPrint("We expect normal A/V output..\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "音视频不能正常播放!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "停止播放节目失败!");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose( hAVPlayer), "关闭节目失败!");
	hAVPlayer = CSUDI_NULL;
	CSUDIOSThreadSleep(2000);
	
	nVideoIndex = IT_iPlayTV(&hAVPlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");
	
	CSTCPrint("期望再次播放音视频成功\n");
	CSTCPrint("We expect normal A/V output again\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "再次播放音视频失败!");
		
	CSTK_FATAL_POINT;
		
	if (hAVPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose (hAVPlayer);
		hAVPlayer=CSUDI_NULL;
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP: 1、PLAYER
//@DESCRIPTION:测试视频坐标偏移
//@PRECONDITION:平台成功初始化
//@EXECUTIONFLOW: 1、锁定EM_IT_COORDINATE_SHIFT指定的频点，期望锁频成功
//@EXECUTIONFLOW: 2、调用CSUDIPLAYEROpen创建一个播放音视频的播放器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_COORDINATE_SHIFT指定信息
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStart开始播放音视频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、期望能够正常播放音视频
//@EXECUTIONFLOW: 6、清屏，销毁播放器
CSUDI_BOOL CSTC_ITTEST_IT_0014(void)
{
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	int nVideoIndex = 0;
	
 	CSTCPrint("CSTC_ITTEST_IT_00014:视频坐标偏移测试\n");

	nVideoIndex = IT_iPlayTV(&hPlayer,EM_IT_COORDINATE_SHIFT,EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO);
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");
	
	CSTCPrint("请从屏幕观察视频上下渐变边是否显示完整且对称，左右白边是否显示完整且对称\n");
	CSTCPrint("We expect normal A/V output,care about integrity and symmetry!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"视频坐标不应是偏移的");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "CSUDIPLAYERStop失败");

	CSTK_FATAL_POINT;
	
	if(hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "CSUDIPLAYERClose失败");
		hPlayer = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1、PLAYER
//@DESCRIPTION:测试节目播放时事件为EM_UDIPLAYER_VIDEO_FRAME_COMING 的回调函数情况
//@PRECONDITION:平台成功初始化
//@INPUT:1、hPlayer有效
//@EXPECTATION:CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用IT_iLockTuner锁定指定的频点
//@EXECUTIONFLOW:2、隐藏视频解码器的输出
//@EXECUTIONFLOW:3、打开一个直播播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用IT_iSetStream,设置流属性信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIPLAYERAddPlayerCallback添加回调函数，事件为EM_UDIPLAYER_VIDEO_FRAME_COMING，设置消息s_nLiveTestdata
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStart，启动播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、判断回调消息是否正确，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复2~8
//@EXECUTIONFLOW:10、恢复现场	
CSUDI_BOOL CSTC_ITTEST_IT_0015(void)
{
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDI_HANDLE   hAVPlayer;
	CSUDIPlayerEventType_E  eEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING;
	int nUserData = 0x66666666;
	int i = 0;
	int j = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == IT_iLockTuner(EM_IT_MPEG2_NTSC), "锁频失败!");
	for(i = 0;i < 2; i++)
	{
		s_nIttestTestdata = 0;
	  	sPlayerChnl.m_nDemux = 0;
		sPlayerChnl.m_nAudioDecoder = 0;
		sPlayerChnl.m_nVideoDecoder = 0;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, FALSE),"关闭视频失败!");
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl,EM_UDIPLAYER_LIVE, &hAVPlayer)) && hAVPlayer != CSUDI_NULL,
			"打开播放器失败! "
			);
		CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE, hAVPlayer,EM_IT_MPEG2_NTSC, EM_IT_STREAM_AUDIO|EM_IT_STREAM_VIDEO),
			"设置节目流失败! "
			);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hAVPlayer,fnIttestCallback1,eEvent,&nUserData), "注册回调失败!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart( hAVPlayer), "开始播放节目失败!");
		for(j = 0;j < 10 ;j ++)
		{
			if(s_nIttestTestdata ==1001)
			break;
	    		CSUDIOSThreadSleep(2000);
		}
		CSTK_ASSERT_TRUE_FATAL(s_nIttestTestdata == 1001, "回调消息错误!");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop( hAVPlayer), "停止播放节目失败!");
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
	CSTCPrint("设置%s 分辨率时标清电视没有狂闪,花屏等异常(Y/N)",bIsHD?"高清":"标清");
	CSTCPrint("OSG能够自动拉伸,OSG图像能全屏完整的显示在屏幕(Y/N)\n");
	CSTCPrint("OSG 没有断裂、模糊等异常(Y/N)\n");
	CSTCPrint("视频小窗口正常?(Y/N)\n");
	CSTCPrint("We expect normal A/V output in small windown on the screen\n");
}

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、OSG
//@DESCRIPTION:分辨率切换与OSG和video的组合测试
//@PRECONDITION:平台成功初始化
//@INPUT:1、OSG、小窗口视频
//@EXPECTATION:1.分辨率切换不会造成小窗口视频异常
//@EXPECTATION:2.分辨率切换时,OSG图像完整无异常
//@EXPECTATION:3.分辨率切换时视频输出不会狂闪烁
//@EXPECTATION:4.分辨率切换时无爆音等异常
//@REMARK:只有平台支持高清时才进行高清输出测试，是否支持高清由配置文件CS_OSGUDI2_SURPORT_HD_SD_DISPLAY字段指定
//@EXECUTIONFLOW:1、调用IT_iLockTuner锁定指定Player_PAL_NTSC节目的频点
//@EXECUTIONFLOW:2、调用CSUDIVIDEOShow将视频输出打开
//@EXECUTIONFLOW:3、打开一个直播播放器，播放Player_PAL_NTSC码流中的PAL制节目
//@EXECUTIONFLOW:4、设置高标清输出小窗口
//@EXECUTIONFLOW:5、打开文件UDI2OSG_search.bmp并将该文件中的ARGB8888 数据拷贝到创建的1280*720的surface上
//@EXECUTIONFLOW:6、调用CSUDIOSGGetDisplaySurface获取高标清的现存句柄
//@EXECUTIONFLOW:7、调用 CSUDIOSGBlit将保存UDI2OSG_search.bmp数据的surface数据blit到高标清显存上
//@EXECUTIONFLOW:8、调用CSUDIOSGFill在高标清输出视频小窗口处将高标清显存相应区域填充成透明色0x0
//@EXECUTIONFLOW:9、调用CSUDISCREENAddCallback注册分辨率切换回调函数,该回调函数处理如下
//@EXECUTIONFLOW:9.1   调用CSUDISCREENGetResolution获取切换后的分辨率
//@EXECUTIONFLOW:9.2   根据切换后的分辨率计算视频小窗口位置
//@EXECUTIONFLOW:9.3   调用CSUDIOSGGetDisplaySurface获取发生分辨率切换的设备的显存
//@EXECUTIONFLOW:9.4   调用CSUDIOSGBlit再将保存UDI2OSG_search.bmp的surface数据blit发生分辨率切换的设备显存上
//@EXECUTIONFLOW:9.5   调用CSUDIVIDEOSetWindowSize设置发生分辨率切换的设备视频输出小窗口
//@EXECUTIONFLOW:10、调用CSUDISCREENSetResolution设置高标清分辨率,每设置一步等待测试确认结果
//@EXECUTIONFLOW:11、调用CSUDISCREENSetResolution设置标清分辨率10次,每次间隔1s,测试完成确认结果
//@EXECUTIONFLOW:12、调用CSUDISCREENSetResolution设置高清分辨率10次,每次间隔2s,测试完成确认结果
//@EXECUTIONFLOW:13、恢复现场,包括关闭播放器,消耗测试surface,清除高标清显存图像	
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");
	
	CSUDIVIDEOSetStopMode(nVideoIndex, EM_UDIVIDEO_STOPMODE_BLACK);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(nVideoIndex, CSUDI_TRUE),"打开视频失败!");

	memcpy(&stWinRectHD,&stRect,sizeof(stRect));
	stWinRectSD.m_nX = stWinRectHD.m_nX*720/1280;
	stWinRectSD.m_nY = stWinRectHD.m_nY*576/720;
	stWinRectSD.m_nWidth = stWinRectHD.m_nWidth*720/1280;
	stWinRectSD.m_nHeight = stWinRectHD.m_nHeight*576/720;
	
	//获取原始的分辨率，便于恢复现场
	//设置高清分辨率为720p，保证视频输出窗口为1280*720，以便于设置小窗口位置正确
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

	//UDI2OSG_search.bmp的大小为1280*720
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
	
	CSTCPrint("屏幕上显示搜索进度界面和视频节目小窗口(Y/N)\n");
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
		CSTCPrint("下面将测试标清反复分辨率切换,请注意标清输出切换过程中是否有异常\n");
		CSTCPrint("按任意键继续\n");
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
		CSTCPrint("下面将测试高清反复分辨率切换,请注意高清输出切换过程中是否有异常\n");
		CSTCPrint("按任意键继续\n");
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
			CSTCPrint("任务TestFlashReadWrite 读flash失败\n");
			s_bErrFlash = CSUDI_TRUE;
			return;
		}
		
		CSUDIOSThreadSleep(100);
		
		if(CSUDIFlashWrite(g_uItTestStartAddress, bVirtualFlash, RWFLASHSIZE)!= CSUDI_SUCCESS)
		{
			CSTCPrint("任务第一次TestFlashReadWrite 写flash失败\n");
			s_bErrFlash = CSUDI_TRUE;
			return;
		}
		CSUDIOSThreadSleep(500);
		memset(bVirtualFlash,0xff,RWFLASHSIZE);
		
		if(CSUDIFlashWrite(g_uItTestStartAddress, bVirtualFlash, RWFLASHSIZE) != CSUDI_SUCCESS)
		{
			CSTCPrint("任务第二次TestFlashReadWrite 写flash失败\n");
			s_bErrFlash = CSUDI_TRUE;
			return;
		}
		CSUDIOSThreadSleep(100);
		memset(bVirtualFlash,0x0,RWFLASHSIZE);
		nTime = CSUDIPlusOSGetTickCount() - nTime1;

		if(nTime >= 60000*15)
		{
			CSTCPrint("\n\n*****超时退出***\n");
			
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
		CSTCPrint("任务TestFsReadWriten打开文件失败\n");
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
				CSTCPrint("任务TestFsReadWriten打开文件失败\n");
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
			CSTCPrint("\n\n*****超时退出***\n");
			CSUDIFSRemove(szFilePath);
			return;
		}
	}
	
	CSUDIFSRemove(szFilePath);
}

//@CASEGROUP: 1、FS
//@CASEGROUP: 2、FLASH
//@DESCRIPTION:同时读写mtd设备和JFFS2文件系统时导致阻塞
//@PRECONDITION:平台已成功初始化
//@INPUT:输入参数合法
//@EXPECTATION:每一步都返回预期结果
//@EXECUTIONFLOW:1、创建任务1对Flash某个区域不断进行读写操作
//@EXECUTIONFLOW:2、创建任务2对JFFS2文件系统不断进行读写操作
//@EXECUTIONFLOW:3、等待15分钟，期望可正常读写不会出现阻塞，如果阻塞需要重启机顶盒
CSUDI_BOOL CSTC_ITTEST_IT_0017(void)
{

	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	int nParam = 0;
	int nPriority = 128;
	CSUDI_BOOL bErr = CSUDI_FAILURE;
	int i = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread1", nPriority , 64*1024, 
							 iTestFlashReadWrite, &nParam, &hThreadhandle1),"创建任务失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"创建成功的任务输出的任务句柄不能为空");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("FLASH_TestThread2", nPriority , 64*1024, 
							(CSUDIOSThreadEntry_F )iTestFsReadWrite, (void *)&nParam, &hThreadhandle2),"创建任务失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"创建成功的任务输出的任务句柄不能为空");

	CSTCPrint("测试同时不断读写Flash和JFFS2文件系统，需持续约15分钟，请等待\n");
	
	while(1)
	{
		CSUDIOSThreadSleep(1000);
		i++;
		if(s_bErrFs ==CSUDI_TRUE || s_bErrFlash ==CSUDI_TRUE)
		{
			if (hThreadhandle1 != CSUDI_NULL)
			{
		 		 CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除任务失败");
		  		 hThreadhandle1 = CSUDI_NULL;
			}
	
			if (hThreadhandle2 != CSUDI_NULL)
			{
				  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除任务失败");
		 	 	 hThreadhandle2 = CSUDI_NULL;
			}
			bErr = CSUDI_TRUE;
			break;
		}
		if(i > 60*15)//超过15分钟则自动结束测试
		{
			CSUDIOSThreadSuspend(hThreadhandle1);
			CSUDIOSThreadSuspend(hThreadhandle2);
			if (hThreadhandle1 != CSUDI_NULL)
			{
		  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除任务失败");
		  		 hThreadhandle1 = CSUDI_NULL;
			}
	
			if (hThreadhandle2 != CSUDI_NULL)
			{
		  		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除任务失败");
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
		  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除任务失败");
		   hThreadhandle1 = CSUDI_NULL;
	}
	
	if (hThreadhandle2 != CSUDI_NULL)
	{
		  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除任务失败");
		   hThreadhandle2 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//查找支持nDemxType类型的demx
//nDemxType参见CSUDIDEMUXWorkType_E
static int searchDemuxID_Y_InInject(int nDemxType)
{
	int i = 0;
	int  nDemuxCount = 0;
	int nDemuxID = -1;
	CSUDIDEMUXCapability_S   sDemuxCapabilityInfo;

	if (CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount))            //先取得demux的数量
	{
		for (i=0; i<nDemuxCount; i++)
		{
			if (CSUDI_SUCCESS == CSUDIDEMUXGetCapability(i, &sDemuxCapabilityInfo))    //取相对应ID=i 的demux的能力
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
//@DESCRIPTION:测试PCM注入播放与直播能够正常切换
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: 
//@EXECUTIONFLOW:1、调用CSUDIINJECTEROpen，创建一个注入实例
//@EXECUTIONFLOW:2、调用CSUDIINJECTERSetAttribute， 设置INJECTER注入实例的属性信息
//@EXECUTIONFLOW:2、调用CSUDIINJECTERSetAttribute， 再次设置INJECTER注入实例的属性信息
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen，创建一个注入播放器
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart  ，打开注入实例
//@EXECUTIONFLOW:5、调用CSUDIOSThreadCreate，创建一个注入线程
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream，设置播放器属性
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart  打开播放器
//@EXECUTIONFLOW:9、提示测试人员观察，是否音频播放出来了
//@EXECUTIONFLOW:10、调用CSUDIOSThreadJoin，等待注入线程结束
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop，关闭注入播放器
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose，删除注入播放器
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop，停止运行注入示例
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClose，删除注入实例
//@EXECUTIONFLOW:14、调用CSUDIOSThreadDestroy，销毁注入线程
//@EXECUTIONFLOW:15、调用CSUDIPLAYEROpen，创建一个直播播放器
//@EXECUTIONFLOW:16、调用CSUDIPLAYERSetStream，设置播放器相应属性
//@EXECUTIONFLOW:17、调用CSUDIPLAYERAddPlayerCallback，添加音频资源改变的回调事件
//@EXECUTIONFLOW:18、调用CSUDIPLAYERStart  打开播放器
//@EXECUTIONFLOW:19、提示测试人员观察，是否有音视频播放出来
//@EXECUTIONFLOW:20、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:21、提示测试人员观察，是否音视频停止播放
//@EXECUTIONFLOW:22    调用CSUDIPLAYERClose删除播放器
//@EXECUTIONFLOW:       恢复现场
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

	sInjectChnl.m_nAudioDecoder = nAudioDecoderId; //PCM数据播放只要音频解码器

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_AUDIO;  
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_PCM;				

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTEROpen(&sInjectChnl, &sInjectOpenParam, &hINJECTER) && hINJECTER != CSUDI_NULL),
			"步骤 1 open injecter  failure !"
		);
	sPcmStartParam.m_uBitsSample = g_PCMInfo[0].m_sStartParam.m_uBitsSample;
	sPcmStartParam.m_uChannels = g_PCMInfo[0].m_sStartParam.m_uChannels;
	sPcmStartParam.m_uSampleRate = g_PCMInfo[0].m_sStartParam.m_uSampleRate;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam)),
			"步骤 2 set PCM_PARAMS attribute  failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN, &g_PCMInfo[0].m_eEndian)),
			"步骤 2 set PCM_ENDIAN attribute  failure !"
		);

	stPlayerChnl.m_nAudioDecoder = nAudioDecoderId;//PCM数据播放只要音频解码器
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&stPlayerChnl,EM_UDIPLAYER_INJECT,&hPcmPlayer)) && hPcmPlayer != CSUDI_NULL,
			"步骤 3 open pcm player failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER)),
			"步骤 4 start injecter  failure !"
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
			"步骤5失败"
		);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPcmPlayer), "步骤6失败");

	CSTCPrint("音频播放出来了吗？\n");
	CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败");

	g_bInjectPCMFinish = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectPCMThread), "步骤8失败"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPcmPlayer), "步骤9失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPcmPlayer), "步骤10失败");

	hPcmPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "停止注入实例失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectPCMThread), "步骤12失败");

	hInjectPCMThread= CSUDI_NULL;
		
	stPlayerChnl.m_nDemux = searchDemuxID_Y_InInject(EM_UDI_DEMUX_PLAY);
	stPlayerChnl.m_nAudioDecoder = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG2);
	stPlayerChnl.m_nVideoDecoder = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== IT_iLockTuner (EM_IT_MPEG2_PAL), "锁频失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_LIVE,&hLivePlayer), "步骤13失败");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE,hLivePlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_VIDEO|EM_IT_STREAM_AUDIO|EM_IT_STREAM_PCR),
			"步骤 14 set video stream failure "
		);

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hLivePlayer), "步骤18失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频是否能够正常播放？\n");
    	CSTCPrint("Is A/V output normal ?\n");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤16失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hLivePlayer), "步骤17失败");
    	CSTCPrint("音视频是否停止播放了\n");
  	CSTCPrint("Does the player stop ?\n");

    		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤18失败：停止播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "步骤20失败");

	hLivePlayer=CSUDI_NULL;
     }

	CSTK_FATAL_POINT;
	
	if (hPcmPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPcmPlayer), "步骤21失败");
		hPcmPlayer=CSUDI_NULL;
	}

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "步骤22失败");
		hLivePlayer=CSUDI_NULL;
	}

	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hINJECTER), "步骤23失败");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectPCMThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectPCMThread), "步骤24失败");
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
	long nFileAllLength = 0;  //文件总长度
	int nFileCurrentPos = 0; //文件当前位置
	int nFileLeftLength = 0; //剩余未读文件长度
	int nInjectedLength = 0; //可注入的大小
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
		//求出当前剩余未读文件的长度
		nFileCurrentPos=InjectFSTell(file);	//当前已读文件的长度
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
		
				
		if(uLength != 0)
		{
			nReadLength = pTSStreamInfo->m_uBufferLen < nFileLeftLength ? pTSStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //剩余文件长度小于可注入空间的长度
			{
				nInjectedLength = nReadLength;
			}
			else  //剩余文件长度大于等于可注入空间的长度
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //读取相应长度的文件
									
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
	long nFileAllLength = 0;  //文件总长度
	int nFileCurrentPos = 0; //文件当前位置
	int nFileLeftLength = 0; //剩余未读文件长度
	int nInjectedLength = 0; //可注入的大小
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
		//求出当前剩余未读文件的长度
		nFileCurrentPos=InjectFSTell(file);	//当前已读文件的长度
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
		
				
		if(uLength != 0)
		{
			nReadLength = pTSStreamInfo->m_uBufferLen < nFileLeftLength ? pTSStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //剩余文件长度小于可注入空间的长度
			{
				nInjectedLength = nReadLength;
			}
			else  //剩余文件长度大于等于可注入空间的长度
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)ppvBuffer, nInjectedLength); //读取相应长度的文件
									
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
	
	CSUDIVIDEOGetCount(&nVideoCount);                                      //先取数量

	for ( i=0;i < nVideoCount; i ++)
	{
		CSUDIVIDEOGetCapability( i , &sVideoCapabilityInfo);	//再取相关能力，与音频不同的是视频只有支持的视频类型一项

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

//注入的音视频解码器
	psInjecterChnl->m_nAudioDecoder=-1;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;	
	psInjecterChnl->m_nDemux=-1;
		
	nDemuxID=searchDemuxID_Y_InInject(EM_UDI_DEMUX_INJECT);
	if(nDemuxID == -1)
	{
		CSTCPrint( "没有支持注入的demux! \n\r");
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
				CSTCPrint( "没有支持指定类型视频解码器!\n\r");
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
				CSTCPrint( "没有支持指定类型音频解码器!\n\r");
                CSTCPrint("There's no audio decode supporting stream type %d \n",\
                    pTSStreamInfo->m_TSContentInfo[i].m_uStreamType.m_eAudioType);
                return -1;
			}
			psPlayerChnl->m_nAudioDecoder = nAudioId;
		}
        
		memcpy(&sStreamInfo[i],&(pTSStreamInfo->m_TSContentInfo[i]),sizeof(CSUDIStreamInfo_S));
        nStreamCnt++;
        
	}

	if(nStreamCnt<1)//至少注入一路类型(video,audio,pcr...)
	{
        CSTCPrint("没有配置注入类型!!\n");
        CSTCPrint("There's no stream type configed !!\n");
        return -1;
	}
    
//播放通道
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

//注入的音视频解码器
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
//@DESCRIPTION:测试TS注入播放与直播切换
//@PRECONDITION:1、INJECTER PLAYER模块被正常初始化
//@PRECONDITION:2、系统存在TS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXECUTIONFLOW:1、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:8、提示用户确认，是否能音视频节目播放出来?//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYEROpen，创建一个直播播放器
//@EXECUTIONFLOW:15、调用CSUDIPLAYERSetStream，设置播放器相应属性
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStart  打开播放器
//@EXECUTIONFLOW:17、提示测试人员观察，是否有音视频播放出来
//@EXECUTIONFLOW:18、调用CSUDIPLAYERStop  停止播放器
//@EXECUTIONFLOW:19、提示测试人员观察，是否音视频停止播放
//@EXECUTIONFLOW:20    调用CSUDIPLAYERClose删除播放器
//@EXECUTIONFLOW:21    重复步骤1-23
//@EXECUTIONFLOW:22    清理现场
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
   	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
   	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux != -1)&&(sInjecterChnl.m_nAudioDecoder==-1)&&(sInjecterChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
        CSTK_ASSERT_TRUE_FATAL(((stPlayerChnl.m_nDemux!= -1)&& ((stPlayerChnl.m_nAudioDecoder != -1) || (stPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤3失败");

	st_TSStreamInfo[EM_TS_INJECT].m_hInjecter = hINJECTER;
	st_TSStreamInfo[EM_TS_INJECT].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &st_TSStreamInfo[EM_TS_INJECT], &hInjectThread), "步骤4失败");

   	while(!st_TSStreamInfo[EM_TS_INJECT].m_bRun) CSUDIOSThreadSleep(500);

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == st_TSStreamInfo[EM_TS_INJECT].m_bOpenFileSuccess, "步骤5 文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,2,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	st_TSStreamInfo[EM_TS_INJECT].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤12失败");

	hPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤13失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "删除注入进程失败");

	hInjectThread = CSUDI_NULL;
		
	stPlayerChnl.m_nDemux = searchDemuxID_Y_InInject(EM_UDI_DEMUX_PLAY);
	stPlayerChnl.m_nAudioDecoder = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_MPEG2);
	stPlayerChnl.m_nVideoDecoder = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE== IT_iLockTuner (EM_IT_MPEG2_PAL), "锁频失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_LIVE,&hLivePlayer), "步骤15失败");

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_TRUE  == IT_iSetStream(CSUDI_TRUE,hLivePlayer,EM_IT_MPEG2_PAL,EM_IT_STREAM_VIDEO|EM_IT_STREAM_AUDIO|EM_IT_STREAM_PCR),
			"set video stream failure "
		);

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hLivePlayer), "步骤17失败");

	CSUDIOSThreadSleep(2000);

	CSTCPrint("音视频是否能够正常播放？\n");
    	CSTCPrint("Is A/V output normal ?\n");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤18失败：播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hLivePlayer), "步骤19失败");
    	CSTCPrint("音视频是否停止播放了\n");
  	CSTCPrint("Does the player stop ?\n");

    		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤20失败：停止播放不正常");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "步骤21失败");

	hLivePlayer=CSUDI_NULL;
     }

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤22失败");
		hPlayer=CSUDI_NULL;
	}

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hLivePlayer), "步骤23失败");
		hLivePlayer=CSUDI_NULL;
	}

	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hINJECTER), "步骤24失败");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤25失败");
		hInjectThread = CSUDI_NULL;
	}
		
	return CSUDI_TRUE;
}
	
//@CASEGROUP:PLAYER INJECTER
//@DESCRIPTION:测试PCM注入播放与TS 注入播放能够直接切换并可以单独设置静音
//@PRECONDITION:PLAYER模块，音视频解码器被正常初始化
//@INPUT: 
//@EXECUTIONFLOW:1、调用CSUDIINJECTEROpen，创建一个注入实例
//@EXECUTIONFLOW:2、调用CSUDIINJECTERSetAttribute， 设置INJECTER注入实例的属性信息
//@EXECUTIONFLOW:2、调用CSUDIINJECTERSetAttribute， 再次设置INJECTER注入实例的属性信息
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen，创建一个注入播放器
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart  ，打开注入实例
//@EXECUTIONFLOW:5、调用CSUDIOSThreadCreate，创建一个注入线程
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream，设置播放器属性
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart  打开播放器
//@EXECUTIONFLOW:9、提示测试人员观察，是否音频播放出来了
//@EXECUTIONFLOW:10、调用CSUDIOSThreadJoin，等待注入线程结束
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop，关闭注入播放器
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose，删除注入播放器
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop，停止运行注入示例
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClose，删除注入实例
//@EXECUTIONFLOW:14、调用CSUDIOSThreadDestroy，销毁注入线程
//@EXECUTIONFLOW:15、根据指定的TS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:16、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:20、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:21、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:22、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:23、提示用户确认，是否能音视频节目播放出来
//@EXECUTIONFLOW:24、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:25、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:26、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:27、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:28、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:29、重复步骤1-28
//@EXECUTIONFLOW:30、清理现场
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

	sInjectChnl.m_nAudioDecoder = nAudioDecoderId; //PCM数据播放只要音频解码器

	sInjectOpenParam.m_eContentType = EM_UDIINJECTER_CONTENT_AUDIO;  
	sInjectOpenParam.m_eInjecterType = EM_INJECTER_PCM;				

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTEROpen(&sInjectChnl, &sInjectOpenParam, &hINJECTER) && hINJECTER != CSUDI_NULL),
			"步骤 1 open injecter  failure !"
		);
	sPcmStartParam.m_uBitsSample = g_PCMInfo[0].m_sStartParam.m_uBitsSample;
	sPcmStartParam.m_uChannels = g_PCMInfo[0].m_sStartParam.m_uChannels;
	sPcmStartParam.m_uSampleRate = g_PCMInfo[0].m_sStartParam.m_uSampleRate;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam)),
			"步骤 2 set PCM_PARAMS attribute  failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN, &g_PCMInfo[0].m_eEndian)),
			"步骤 2 set PCM_ENDIAN attribute  failure !"
		);
		
		//pcm player
	stPlayerChnl.m_nAudioDecoder = nAudioDecoderId;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS== CSUDIPLAYEROpen(&stPlayerChnl,EM_UDIPLAYER_INJECT,&hPcmPlayer)) && hPcmPlayer != CSUDI_NULL,
			"步骤 3 open pcm player failure !"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER)),
			"步骤 4 start injecter  failure !"
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
			"步骤5失败"
		);
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPcmPlayer), "步骤6失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioDecoderId), "取消静音失败");
	
	CSTCPrint("音频播放出来了吗？\n");
	CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioDecoderId), "静音失败");

	CSTCPrint("没有任何声音输出\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤7失败");

	g_bInjectPCMFinish = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectPCMThread), "步骤8失败"); 

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPcmPlayer), "步骤9失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPcmPlayer), "步骤10失败");

	hPcmPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioDecoderId), "取消静音失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "停止注入实例失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectPCMThread), "步骤12失败");

	hInjectPCMThread = CSUDI_NULL;
	
	//TS注入
	nStreamCount = getTSInjectInfo(EM_TS_INJECT,&sInjectChnl,&stPlayerChnl,&sOpenParams,sStreamInfo);
   	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
   	CSTK_ASSERT_TRUE_FATAL(((sInjectChnl.m_nDemux != -1)&&(sInjectChnl.m_nAudioDecoder==-1)&&(sInjectChnl.m_nVideoDecoder==-1)),"TS注入设备指定不合法");
        CSTK_ASSERT_TRUE_FATAL(((stPlayerChnl.m_nDemux!= -1)&& ((stPlayerChnl.m_nAudioDecoder != -1) || (stPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjectChnl,&sOpenParams,&hINJECTER), "步骤13失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_AUDIO_FIRST;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤14失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤15失败");

	st_TSStreamInfo[EM_TS_INJECT].m_hInjecter = hINJECTER;
	st_TSStreamInfo[EM_TS_INJECT].m_bRun = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("TS Inject", INJECTTHREADPRO, INJECTTHREADBUF, TS_injectTask, ( void* ) &st_TSStreamInfo[0], &hInjectTSThread), "步骤15失败");

   	while(!st_TSStreamInfo[EM_TS_INJECT].m_bRun) CSUDIOSThreadSleep(500);

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == st_TSStreamInfo[EM_TS_INJECT].m_bOpenFileSuccess, "文件打开失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_INJECT,&hTSPlayer), "步骤16失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hTSPlayer,sStreamInfo,nStreamCount,CSUDI_NULL), "步骤17失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hTSPlayer), "步骤18失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(stPlayerChnl.m_nAudioDecoder), "取消静音失败");

	CSTCPrint("音视频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"播放失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(stPlayerChnl.m_nAudioDecoder), "静音失败");
	
	CSTCPrint("没有声音，视频依然正常\r\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"静音失败");

	st_TSStreamInfo[EM_TS_INJECT].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectTSThread),"步骤19失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hTSPlayer), "步骤20失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hTSPlayer), "步骤21失败");

	hTSPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(stPlayerChnl.m_nAudioDecoder), "静音失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤22失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤23失败");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectTSThread), "步骤24失败");

	hInjectTSThread = CSUDI_NULL;

   }
	CSTK_FATAL_POINT;

	if (nAudioDecoderId != -1)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioDecoderId), "取消静音失败");
	}
	
	if (stPlayerChnl.m_nAudioDecoder != -1)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(stPlayerChnl.m_nAudioDecoder), "静音失败");
	}
	
	if (hPcmPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPcmPlayer), "步骤25失败");
		hPcmPlayer=CSUDI_NULL;
	}

	if (hTSPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hTSPlayer), "步骤26失败");
		hTSPlayer=CSUDI_NULL;
	}

	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hINJECTER), "步骤27失败");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectPCMThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectPCMThread), "步骤28失败");
		hInjectPCMThread = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectTSThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectTSThread), "步骤29失败");
		hInjectTSThread = CSUDI_NULL;
	}
		
	return CSUDI_TRUE;
}


//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试PS注入播放
//@PRECONDITION:1、INJECTER PLAYER模块被正常初始化
//@PRECONDITION:2、系统存在PS播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXECUTIONFLOW:1、根据指定的PS码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入TS  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取TS码流文件，并进行TS注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好TS码流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音视频
//@EXECUTIONFLOW:9、提示用户确认，是否能音视频节目播放出来
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、清理现场
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
   	CSTK_ASSERT_TRUE_FATAL((nStreamCount != -1),"注入资源初始化失败");
   	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nDemux == -1)&&(sInjecterChnl.m_nAudioDecoder!=-1)&&(sInjecterChnl.m_nVideoDecoder!=-1)),"PS注入设备指定不合法");
        CSTK_ASSERT_TRUE_FATAL(((stPlayerChnl.m_nDemux == -1)&& ((stPlayerChnl.m_nAudioDecoder != -1) || (stPlayerChnl.m_nVideoDecoder != -1))),"播放器设备指定不合法");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&stPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	st_TSStreamInfo[EM_PS_INJECT].m_hInjecter = hINJECTER;
	st_TSStreamInfo[EM_PS_INJECT].m_bRun = CSUDI_FALSE;
	s_exitinject = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("PS Inject", INJECTTHREADPRO, INJECTTHREADBUF, PS_injectTask, ( void* ) &st_TSStreamInfo[EM_PS_INJECT], &hInjectThread), "步骤5失败");

	while (!st_TSStreamInfo[EM_PS_INJECT].m_bRun) 
	{
		CSUDIOSThreadSleep(20);
	}

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == st_TSStreamInfo[EM_PS_INJECT].m_bOpenFileSuccess, "文件打开失败");

	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,2,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	while(!s_exitinject)
	{
		CSUDIOSThreadSleep(20);
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤12失败");

	hPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤13失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	hINJECTER = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "删除注入进程失败");

	hInjectThread = CSUDI_NULL;

	CSTK_FATAL_POINT;
	
	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose (hPlayer), "步骤16失败");
		hPlayer = CSUDI_NULL;
	}

	if (hINJECTER != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERClose (hINJECTER), "步骤17失败");
		hINJECTER = CSUDI_NULL;
	}

	if(CSUDI_NULL != hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤18失败");
		hInjectThread = CSUDI_NULL;
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、OSG
//@DESCRIPTION:测试播放MPEG2音频的同时用硬解码的方式播放I帧图片
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持I帧图片硬解码
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.mpg
//@EXECUTIONFLOW:期望能够正常播放音频
//@EXECUTIONFLOW: 1、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 2、调用CSUDIPLAYEROpen创建一个播放音频的播放器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStart开始播放音频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、期望能够正常播放音频
//@EXECUTIONFLOW: 6、以下步骤解码I帧，步骤7-12如果失败则跳转步骤13
//@EXECUTIONFLOW: 7、获取UDI2OSG_720_576.mpg图片数据的内存地址和长度
//@EXECUTIONFLOW: 8、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW: 9、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW: 10、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW: 11、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW: 12、如解码I帧成功，则期望电视能看到I帧图片UDI2OSG_720_576.mpg，且能够正常播放音频
//@EXECUTIONFLOW: 13、释放图片资源，销毁surface，销毁播放器
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");

	CSTCPrint("音频可以正常播放吗 ?\n");
	CSTCPrint("Does the audio output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 5 check result failure!");

	if(IT_iIsSupportDecImage(EM_IT_DECODE_IFRAME))
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == IT_iTestDecImageWithScaleMode(EM_IT_DECODE_IFRAME,EM_IT_IFRAME_720_576,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface,&pcBuff,&nLen)),
				"step 7、8 decode iframe image failure !"
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
	
		CSTCPrint("期望电视能看到全屏I帧UDI2OSG_720_576.mpg，且能够正常播放音频 \n");
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


//@CASEGROUP: 1、PLAYER
//@CASEGROUP: 2、OSG
//@DESCRIPTION:测试播放MPEG2音频的同时用硬解码的方式播放JPG图片
//@PRECONDITION:1、平台成功初始化
//@PRECONDITION:2、支持JPG图片硬解码
//@INPUT: 1、EM_IT_MPEG2_PAL
//@INPUT: 2、UDI2OSG_720_576.jpg
//@EXECUTIONFLOW:期望能够正常播放音频
//@EXECUTIONFLOW: 1、锁定EM_IT_MPEG2_PAL指定的频点，期望锁频成功
//@EXECUTIONFLOW: 2、调用CSUDIPLAYEROpen创建一个播放音频的播放器，期望创建成功
//@EXECUTIONFLOW: 3、调用CSUDIPLAYERSetStream设置节目音频信息为EM_IT_MPEG2_PAL指定信息
//@EXECUTIONFLOW: 4、调用CSUDIPLAYERStart开始播放音频,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、期望能够正常播放音频
//@EXECUTIONFLOW: 6、以下步骤解码JPG图片，步骤7-12如果失败则跳转步骤13
//@EXECUTIONFLOW: 7、获取UDI2OSG_720_576.jpg图片数据的内存地址和长度
//@EXECUTIONFLOW: 8、调用CSUDIOSGDecodeImage解码图片，应该返回CSUDI_SUCCESS，解码得到的phSurface应该不为CSUDI_NULL
//@EXECUTIONFLOW: 9、调用CSUDIOSGGetDisplaySurface接口获取显存空间hDisSurface句柄
//@EXECUTIONFLOW: 10、调用CSUDIOSGBlit接口将解码得到的图片信息blit到显存空间hDisSurface上，blit的矩形区域为CSUDI_NULL。
//@EXECUTIONFLOW: 11、调用CSUDIOSGSync完成图片的blit操作，成功返回CSUDI_SUCCESS。
//@EXECUTIONFLOW: 12、如解码JPG图片成功，则期望电视能看到I帧图片UDI2OSG_720_576.jpg，且能够正常播放音频
//@EXECUTIONFLOW: 13、释放图片资源，销毁surface，销毁播放器
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
	CSTK_ASSERT_TRUE_FATAL(nVideoIndex != -1, "节目播放失败");

	CSTCPrint("音频可以正常播放吗 ?\n");
	CSTCPrint("Does the audio output meet the expectation ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(), "step 5 check result failure!");

	if(IT_iIsSupportDecImage(EM_IT_DECODE_JPG))
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == IT_iTestDecImageWithScaleMode(EM_IT_DECODE_JPG,EM_IT_JPG_720_576,EM_UDIOSG_SCALE_FULLSIZE,CSUDI_NULL,&hSurface,&pcBuff,&nLen)),
				"step 7、8 decode jpeg image failure !"
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
	
		CSTCPrint("期望电视能看到全屏JPEG图片UDI2OSG_720_576.jpg，且能够正常播放音频 \n");
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


