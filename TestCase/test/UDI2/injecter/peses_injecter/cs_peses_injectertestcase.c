/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "udi2_demux.h"
#include "udi2_audio.h"
#include "udi2_video.h"
#include "udi2_fs.h"
#include "udi2_os.h"
#include "udi2_player.h"
#include "udi2_inject.h"
#include "cs_testkit.h"
#include "cs_testkit_porting.h"
#include "cs_peses_injectertestcase.h"

#define REPEAT_TIMES 1
#define ERRORDATA (2009)
#define WAITINJECTTIME (1000)   
#define INJECTTHREADPRO (100)
#define INJECTTHREADBUF (1024 * 148)
#define INJECT_SEEK_SET (0)
#define INJECT_SEEK_CUR (1)
#define INJECT_SEEK_END (2)
#define ESServiceCunt  (10) 

typedef enum 
{
    EM_INJECT_PESES_VIDEO_MPEG2 = 0, 	/*湖南卫视*/
    EM_INJECT_PESES_VIDEO_H264, 		/**/
    EM_INJECT_PESES_AUDIO_MPEG2,		/**/
    EM_INJECT_PESES_AUDIO_AAC,    		/**/
    EM_INJECT_PESES_MAX_COUNT
}ES_ServiceIndex_E;

typedef struct _ESStreamInfo_S
{
    char * m_pcName; //注入文件的名
    CSUDIStreamInfo_S    m_ESContentInfo;
    unsigned int m_uBufferLen;
    CSUDI_BOOL m_bRun;
    CSUDI_HANDLE m_hInjecter;
}ESStreamInfo_S;

static ESStreamInfo_S g_ESStreamInfo[ESServiceCunt] = 
{
  	{
        "mpeg2_video.mpv",
        {0, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_MPEG2}},
		10*1024,
		CSUDI_TRUE,
		CSUDI_NULL,
    },
	{
        "H.264_video.h264",
		{0, EM_UDI_CONTENT_VIDEO, {EM_UDI_VID_STREAM_H264}},   
		10*1024,	
		CSUDI_TRUE,
		CSUDI_NULL,	
	    },
	{
        "mpeg2_audio.mpa",
		{0, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG2}},   
		10*1024,
		CSUDI_TRUE,
		CSUDI_NULL,	
    },
   	{
        "MPEG2_AAC_audio.aac",
		{0, EM_UDI_CONTENT_AUDIO, {EM_UDI_AUD_STREAM_MPEG_AAC}},   
		10*1024,
		CSUDI_TRUE,
		CSUDI_NULL,	
    },    	
};

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
						if (sAudioCapabilityInfo.m_sSupportByPass[j] == EM_UDI_AUD_STREAM_UNKNOWN)
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
						if (sAudioCapabilityInfo.m_sSupportDecode[j] == EM_UDI_AUD_STREAM_UNKNOWN)
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


///根据传入的index来决定要搜索哪种类型的视频解码器
//其中输入参数表示相对应的视频在音频类型枚举里的取值
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
			if (sVideoCapabilityInfo.m_eStreamType[j] == EM_UDI_VID_STREAM_UNKNOWN)
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

static void * InjectFSOpen(const char * pcFileName, const char * pcMode)
{
	return (void *)CSTKP_FOpen(pcFileName, pcMode);
}

static int  InjectFSSeek(void *pFile, long lOffset, unsigned int uOrigin)
{
	return CSTKP_FSeek((CSUDI_HANDLE)pFile, lOffset, uOrigin);

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
	void *pFileTemp;
	int nReturn = -1;

	pFileTemp = InjectFSOpen(pFileName, "rb");

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

static void ES_injectTask(void * ESStreamInfo)
{
	void * ppvBuffer = NULL;

	unsigned  int  uLength = 0;
	
	ESStreamInfo_S *pESStreamInfo = (ESStreamInfo_S*)ESStreamInfo;

	CSUDI_HANDLE hInjecter = pESStreamInfo->m_hInjecter;
	
	int nBlockNum = 0;
	long nFileAllLength = 0;  //文件总长度
	int nFileCurrentPos = 0; //文件当前位置
	int nFileLeftLength = 0; //剩余未读文件长度
	int nInjectedLength = 0; //可注入的大小
	int nReadLength = 0;
	
	char *pBuffer = CSUDI_NULL;
	
	void* file = CSUDI_NULL;
	
	if (!getInjecterFile((const char*)pESStreamInfo->m_pcName, &file, &nFileAllLength))
	{
		return;
	}
	
	if (pESStreamInfo->m_uBufferLen<10240||pESStreamInfo->m_uBufferLen>1024*1024)
	{
		pESStreamInfo->m_uBufferLen = 10*1024;
	}
	
	pESStreamInfo->m_bRun = CSUDI_TRUE;
	
	pBuffer = CSUDIOSMalloc(pESStreamInfo->m_uBufferLen);
	
	while(pESStreamInfo->m_bRun)
	{
		//求出当前剩余未读文件的长度
		nFileCurrentPos=InjectFSTell(file);	//当前已读文件的长度
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
				
		if(uLength != 0)
		{
			nReadLength = pESStreamInfo->m_uBufferLen < nFileLeftLength ? pESStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //剩余文件长度小于可注入空间的长度
			{
				nInjectedLength = nReadLength;
			}
			else  //剩余文件长度大于等于可注入空间的长度
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)pBuffer, nInjectedLength); //读取相应长度的文件
									
			memcpy(ppvBuffer, pBuffer, nInjectedLength); //向注入缓冲区中拷入数据

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

	if(!pESStreamInfo->m_bRun )
	{
		InjectFSClose(file);
	}
	
	if (pBuffer)
	{
		CSUDIOSFree(pBuffer);
	}
}

static void ES_injectTask_OverLengh(void * ESStreamInfo)
{
	void * ppvBuffer = NULL;

	unsigned  int  uLength = 0;
	
	ESStreamInfo_S *pESStreamInfo = (ESStreamInfo_S*)ESStreamInfo;

	CSUDI_HANDLE hInjecter = pESStreamInfo->m_hInjecter;
	
	int nBlockNum = 0;
	long nFileAllLength = 0;  //文件总长度
	int nFileCurrentPos = 0; //文件当前位置
	int nFileLeftLength = 0; //剩余未读文件长度
	int nInjectedLength = 0; //可注入的大小
	int nReadLength = 0;
	
	char *pBuffer = CSUDI_NULL;
	
	void* file = CSUDI_NULL;
	
	getInjecterFile((const char*)pESStreamInfo->m_pcName, &file, &nFileAllLength);
	
	if (pESStreamInfo->m_uBufferLen<10240||pESStreamInfo->m_uBufferLen>1024*1024)
	{
		pESStreamInfo->m_uBufferLen = 10*1024;
	}
	
	pESStreamInfo->m_bRun = CSUDI_TRUE;
	
	pBuffer = CSUDIOSMalloc(pESStreamInfo->m_uBufferLen);
	
	while(pESStreamInfo->m_bRun)
	{
		//求出当前剩余未读文件的长度
		nFileCurrentPos=InjectFSTell(file);	//当前已读文件的长度
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
				
		if(uLength != 0)
		{
			nReadLength = pESStreamInfo->m_uBufferLen < nFileLeftLength ? pESStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //剩余文件长度小于可注入空间的长度
			{
				nInjectedLength = nReadLength;
			}
			else  //剩余文件长度大于等于可注入空间的长度
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)pBuffer, nInjectedLength); //读取相应长度的文件
									
			memcpy(ppvBuffer, pBuffer, nInjectedLength); //向注入缓冲区中拷入数据

			CSUDIINJECTERWriteComplete(hInjecter,uLength+10);
			
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

	if(!pESStreamInfo->m_bRun )
	{
		InjectFSClose(file);
	}
	
	if (pBuffer)
	{
		CSUDIOSFree(pBuffer);
	}
}

static void getESInjectInfo(ES_ServiceIndex_E  eFileType,CSUDIINJECTERChnl_S  *psInjecterChnl,CSUDIPlayerChnl_S  *psPlayerChnl,CSUDIINJECTEROpenParam_S *pOpenParam ,CSUDIStreamInfo_S  *psStreamInfo)
{
	int nAudioId = -1;
	int nVideoId = -1;
	
	ESStreamInfo_S *pESStreamInfo = &g_ESStreamInfo[eFileType];

//注入的音频解码器
	psInjecterChnl->m_nAudioDecoder=-1;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;	
	psInjecterChnl->m_nDemux=-1;
		
	pOpenParam->m_eInjecterType= EM_INJECTER_ES;
	
	if (pESStreamInfo->m_ESContentInfo.m_eContentType == EM_UDI_CONTENT_VIDEO)
	{
		nVideoId = searchVideoID_Y_InInject(pESStreamInfo->m_ESContentInfo.m_uStreamType.m_eVideoType);
		if(nVideoId ==-1)
		{
			CSTCPrint( "没有支持指定类型视频解码器!\n\r");
		}
		psInjecterChnl->m_nVideoDecoder = nVideoId;
		
		pOpenParam->m_eContentType = EM_UDIINJECTER_CONTENT_VIDEO;
	}
	else if (pESStreamInfo->m_ESContentInfo.m_eContentType == EM_UDI_CONTENT_AUDIO)
	{
		nAudioId = searchAudioID_Y_InInject(pESStreamInfo->m_ESContentInfo.m_uStreamType.m_eAudioType);
		if(nAudioId ==-1)
		{
			CSTCPrint( "没有支持指定类型音频解码器!\n\r");
		}
		psInjecterChnl->m_nAudioDecoder = nAudioId;
		pOpenParam->m_eContentType = EM_UDIINJECTER_CONTENT_AUDIO;

	}

//设置节目信息
       memcpy(psStreamInfo,&(pESStreamInfo->m_ESContentInfo),sizeof(CSUDIStreamInfo_S));

//播放通道
	psPlayerChnl->m_nDemux=psInjecterChnl->m_nDemux;
	psPlayerChnl->m_nAudioDecoder=psInjecterChnl->m_nAudioDecoder;
	psPlayerChnl->m_nVideoDecoder=psInjecterChnl->m_nVideoDecoder;
	
	return;
}

static CSUDI_BOOL closeInjectPlayer(CSUDI_HANDLE hPlayer)
{
	CSUDI_BOOL bRe = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL !=hPlayer,"参数错误");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "停止播放失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "关闭播放器失败");
	
	bRe = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bRe;
}

static CSUDI_HANDLE OpenESInectPlayer(CSUDIPlayerChnl_S *pChnl, CSUDIStreamInfo_S *pStreamInfo)
{
	CSUDI_HANDLE hPlayer= CSUDI_NULL;
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pChnl, "参数1错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pStreamInfo, "参数2错误");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (pChnl, EM_UDIPLAYER_INJECT,&hPlayer), "打开播放器失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,pStreamInfo,1,CSUDI_NULL), "设置流失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "启动播放失败");

	CSTK_FATAL_POINT;
	
	return hPlayer;
}

static void task_For_ESOpenColse(void *pvData)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	CSUDI_HANDLE  hINJECTER;
	int i= 0;
	ES_ServiceIndex_E eSeviceID = EM_INJECT_PESES_VIDEO_MPEG2;

	eSeviceID = (ES_ServiceIndex_E)((int)pvData);

	if (eSeviceID > EM_INJECT_PESES_MAX_COUNT)
	{
		eSeviceID = EM_INJECT_PESES_VIDEO_MPEG2;
	}
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));
	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	
	getESInjectInfo(eSeviceID,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	CSUDIOSThreadSleep(10); 
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		hINJECTER = CSUDI_NULL;
		
		CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER);
		
		CSUDIOSThreadSleep(1); 
		
		if (hINJECTER != CSUDI_NULL)
		{
			CSUDIINJECTERClose(hINJECTER);
		}
	}
	
	CSTK_FATAL_POINT;
	
	return ;
}

static void task_For_ESStartStop(void * hInjecterHander )
{
	int i =0;
		
	CSUDI_HANDLE hHandler = CSUDI_NULL;
	
	CSUDIOSThreadSleep(10);
	
	if (hInjecterHander != CSUDI_NULL)
	{
	 	hHandler =hInjecterHander;
		
		for (i=0;i<REPEAT_TIMES;i++)
		{
			CSUDIINJECTERStart (hHandler);
			
			CSUDIOSThreadSleep(1); 
			
			CSUDIINJECTERStop(hHandler);
		}
	}
	
	return ;
}

CSUDI_BOOL CSTC_PESES_INJECTER_Init(void)
{
	//在本测试用例集执行前调用
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PESES_INJECTER_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试Open/Close PES/ES注入器时，在参数不符合设计要求情况下的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:参数不符合PES/ES注入器接口设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、psOpenParams->m_eContentType=EM_UDIINJECTER_CONTENT_DEFAULT,调用CSUDIINJECTEROpen试图打开一个PES/ES注入器,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、psInjecterChnl->m_nDemux 不等于-1时，调用CSUDIINJECTEROpen试图打开一个PES/ES注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、psInjecterChnl->m_nVideoDecoder 错误时时，调用CSUDIINJECTEROpen试图打开一个PES/ES注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、psInjecterChnl->m_nAudioDecoder 错误时时，调用CSUDIINJECTEROpen试图打开一个PES/ES注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、phINJECTER = CSUDI_NULL 时，调用CSUDIINJECTEROpen试图打开一个PES/ES注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、psInjecterChnl = CSUDI_NULL时，调用CSUDIINJECTEROpen试图打开一个PES/ES注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、psOpenParams = CSUDI_NULL时，调用CSUDIINJECTEROpen试图打开一个PES/ES注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、hINJECTER = CSUDI_NULL时，调用CSUDIINJECTERClose试图关闭一个无效的PES/ES注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤2 至步骤7，共100次
CSUDI_BOOL CSTC_PESES_INJECTER_IT_OpenClose_0001(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	CSUDIINJECTERContentType_E e_ContentType = EM_UDIINJECTER_CONTENT_DEFAULT;
	int nDecoderID = -1;
	
	int i=0;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	for (i=0;i<REPEAT_TIMES; i++)
	{
		e_ContentType = sOpenParams.m_eContentType;
		
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_DEFAULT;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

		sOpenParams.m_eContentType = e_ContentType;
		
		sInjecterChnl.m_nDemux=ERRORDATA;		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤3失败");
		sInjecterChnl.m_nDemux=-1;
		
		nDecoderID = sInjecterChnl.m_nVideoDecoder;
		sInjecterChnl.m_nVideoDecoder = ERRORDATA;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_DEVICE_ID == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤4失败");
		sInjecterChnl.m_nVideoDecoder = nDecoderID;

		nDecoderID = sInjecterChnl.m_nAudioDecoder;
		sInjecterChnl.m_nAudioDecoder = ERRORDATA;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤5失败");
		sInjecterChnl.m_nAudioDecoder = nDecoderID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,CSUDI_NULL), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (CSUDI_NULL,&sOpenParams,&hINJECTER), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,CSUDI_NULL,&hINJECTER), "步骤8失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(CSUDI_NULL), "步骤9失败");

	}

	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试所有参数均符合接口设计要求的情况下，CSUDIINJECTEROpen & CSUDIINJECTERClose的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:所有参数和应用场景均符合接口设计要求
//@EXPECTATION:返回CSUDI_SUCCESS  
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERClose，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复步骤2和步骤3共1000次。
CSUDI_BOOL CSTC_PESES_INJECTER_IT_OpenClose_0002(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
		
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤3失败");
	}
	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试ES 注入器Open和Close 不成对调用，多次Close的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:参数均符合接口设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS  
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，正常打开一个PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTEROpen，正常打开一个PES/ES注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERClose，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERClose，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、重复步骤2至步骤5，共1000次
CSUDI_BOOL CSTC_PESES_INJECTER_IT_OpenClose_0003(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL,  hINJECTER1 = CSUDI_NULL;
		
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "步骤2失败");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS !=  CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER1)), "步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(hINJECTER), "步骤5失败");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:多线程交叉调用CSUDIINJECTEROpen & CSUDIINJECTERClose的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 合法的
//@EXPECTATION:不死机，不死锁，逻辑正确
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、启动另外一个任务，该任务同时并异步执行下面的步骤3和4
//@EXECUTIONFLOW:3、调用CSUDIINJECTEROpen，正常打开一个PES/ES注入器，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:4、如果步骤3成功，则调用CSUDIINJECTERClose，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤3至步骤4，共1000次
//@EXECUTIONFLOW:6、等待步骤2启动的任务正常完成退出
//@EXECUTIONFLOW:7、销毁步骤2启动的任务
CSUDI_BOOL CSTC_PESES_INJECTER_IT_OpenClose_0004(void)
{
	CSUDI_HANDLE hInjectThread = 0;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OpenClose", INJECTTHREADPRO, INJECTTHREADBUF, task_For_ESOpenColse, (void *)EM_INJECT_PESES_VIDEO_MPEG2, &hInjectThread), "步骤六失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		udiRe = CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_DEVICE_BUSY == udiRe )
			|| (CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED == udiRe)),"步骤3失败");

		CSUDIOSThreadSleep(2);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤4失败");
		}
	}
	
	CSTK_FATAL_POINT;
	
	if (hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤6失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤7失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试ES注入事例句柄为CSUDI_NULL时，Start/Stop函数的执行效果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER = CSUDI_NULL
//@EXPECTATION:Start/Stop函数均返回CSUDIINJECTER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、正常打开PES/ES注入器，得到PES/ES注入器句柄
//@EXECUTIONFLOW:3、以hINJECTER = CSUDI_NULL,调用CSUDIINJECTERStart,期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:4、以hINJECTER = CSUDI_NULL,调用CSUDIINJECTERStop,期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:5、重复步骤3至步骤4，共1000次
//@EXECUTIONFLOW:6、调用CSUDIINJECTERClose，关闭此PES/ES注入器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_StartStop_0001(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
		
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStart(CSUDI_NULL), "步骤3失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStop(CSUDI_NULL), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤6失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试正常情况下，CSUDIINJECTERStart和CSUDIINJECTERStop成对调用的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为正常打开的ES注入事例句柄
//@EXPECTATION:CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、正常打开PES/ES注入器，得到PES/ES注入器句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERStart,启动此PES/ES注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStop,停止此PES/ES注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:5、重复步骤3至步骤4，共1000次
//@EXECUTIONFLOW:6、调用CSUDIINJECTERClose，关闭此PES/ES注入器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_StartStop_0002(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
		
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤4失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤6失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试正常情况下，CSUDIINJECTERStart和CSUDIINJECTERStop不成对或逆序调用的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为正常打开的ES注入事例句柄
//@EXPECTATION:返回非CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、正常打开PES/ES注入器，得到PES/ES注入器句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERStop,停止此PES/ES注入器，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart,启动此PES/ES注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart,启动此PES/ES注入器，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStop,停止此PES/ES注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:7、调用CSUDIINJECTERStop,停止此PES/ES注入器，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:8、重复步骤3至步骤7，共1000次
//@EXECUTIONFLOW:9、调用CSUDIINJECTERClose，关闭此PES/ES注入器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_StartStop_0003(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
		
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStart(hINJECTER), "步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "步骤7失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤9失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:多线程交叉调用CSUDIINJECTERStart & CSUDIINJECTERStop的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 合法的ES注入句柄
//@EXPECTATION:不死机，不死锁，逻辑正确
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，正常打开一个PES/ES注入器，得到注入器句柄
//@EXECUTIONFLOW:3、启动另外一个任务，该任务同时并异步执行下面的步骤4和5
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，启动此PES/ES注入器，期望返回CSUDI_SUCCESS或CSUDIREC_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStop，停止此PES/ES注入器，期望返回CSUDI_SUCCESS或CSUDIREC_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:6、重复步骤6至步骤7，共1000次
//@EXECUTIONFLOW:7、调用CSUDIOSThreadJoin，等待步骤5启动的任务执行完成，关闭退出
//@EXECUTIONFLOW:8、调用CSUDIINJECTERClose，关闭此PES/ES注入器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_StartStop_0004(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;

	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ESStartStop", INJECTTHREADPRO, INJECTTHREADBUF, task_For_ESStartStop, (void *)hINJECTER, &hInjectThread), "步骤3失败");
	
	for (i=0;i<REPEAT_TIMES; i++)
	{	
		udiRe = CSUDIINJECTERStart(hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STARTED == udiRe )),"步骤4失败");

		CSUDIOSThreadSleep(2);

		udiRe =  CSUDIINJECTERStop(hINJECTER);
		
		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STOPPED == udiRe )),"步骤5失败");
	}
    	
	CSTK_FATAL_POINT;
	
	if (hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤7失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤8失败");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤9失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试在ES注入模式，接口参数不符合设计要求的情况下，CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute的执行效果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、不符合接口设计要求 
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、hINJECTER = CSUDI_NULL时，调用CSUDIINJECTERSetAttribute设置属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、pvData = CSUDI_NULL,时，调用CSUDIINJECTERSetAttribute设置同步属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、pvData = CSUDI_NULL,时，调用CSUDIINJECTERSetAttribute 设置回调函数属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERSetAttribute 设置只读状态属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERSetAttribute 设置只读缓冲区属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERSetAttribute 设置非法属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、hINJECTER = CSUDI_NULL时，调用CSUDIINJECTERGetAttribute获取属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:11、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute 获取同步属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute获取回调属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute 获取状态属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:13、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute 获取缓冲区属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERSetAttribute 获取非法属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:15、重复步骤3~步骤14共1000次
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose，关闭此PES/ES注入器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_SetGetAttribute_0001(void)
{
	CSUDIINJECTERStatus_S  sStatus;
	CSUDIINJECTERBuffer_S  sBuffer;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
//设置		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,CSUDI_NULL),  "步骤4失败");
				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_STATUS, &sStatus),  "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sBuffer),  "步骤7失败");

//获取	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,CSUDI_NULL),  "步骤10失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,CSUDI_NULL),  "步骤12失败");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,CSUDI_NULL),  "步骤13失败");		
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试在ES注入模式，接口参数均符合设计要求的情况下，CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute的执行效果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、均符合接口设计要求 
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置同步方式属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute 设置回调函数属性，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5、调用CSUDIINJECTERGetAttribute获取同步方式属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、判断获取得到的同步方式属性为步骤3所设置的属性一致
//@EXECUTIONFLOW:7、调用CSUDIINJECTERGetAttribute获取缓冲区属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERGetAttribute获取状态属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIINJECTERGetAttribute获取回调函数属性，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:10、如果上一步返回成功，则判断获取得到的回调函数属性为步骤4所设置的属性一致
//@EXECUTIONFLOW:11、重复步骤3~步骤10共1000次
//@EXECUTIONFLOW:12、调用CSUDIINJECTERClose，关闭此PES/ES注入器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_SetGetAttribute_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTERStatus_S  sStatus;
	CSUDIINJECTERBuffer_S  sBuffer;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	sSyncMode.m_eSyncMode =  EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
//设置			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode),  "步骤3失败");				
//获取
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode),  "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(((sSyncMode.m_eSyncMode == EM_UDIINJECTER_SYNC_NONE)
			||(sSyncMode.m_hSyncHandle == CSUDI_NULL)),"步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sBuffer),  "步骤7失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,&sStatus),  "步骤8失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤12失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试PES/ES注入器在start之后再调用CSUDIINJECTERSetAttribute/CSUDIINJECTERGetAttribute的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 非空
//@INPUT:2、eAttrType=EM_UDIINJECTER_PESES_ENDIAN  
//@INPUT:3、pvData非空
//@EXPECTATION:调用CSUDIINJECTERSetAttribute    返回CSUDIINJECTER_ERROR_INVALID_STATUS，调用CSUDIINJECTERGetAttribute返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK:启动注入后不能调用CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置同步属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart启动此PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERSetAttribute设置"回调函数"属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERGetAttribute获取"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、判断获取得到的同步属性为步骤3所设置的属性一致
//@EXECUTIONFLOW:9、调用CSUDIINJECTERGetAttribute获取"状态"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERGetAttribute获取"缓冲区"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、调用CSUDIINJECTERGetAttribute获取"回调函数"属性，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:12、重复步骤6~步骤11共1000次
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop，停止此PES/ES注入器
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose，关闭此PES/ES注入器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_SetGetAttribute_0003(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTERStatus_S  sStatus;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	sSyncMode.m_eSyncMode =  EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	for (i=0; i<REPEAT_TIMES; i++)
	{
//设置
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤5失败");			

//获取
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤7失败");

		CSTK_ASSERT_TRUE_FATAL(((sSyncMode.m_eSyncMode == EM_UDIINJECTER_SYNC_NONE)
			||(sSyncMode.m_hSyncHandle == CSUDI_NULL)),"步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,&sStatus), "步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sStatus), "步骤10失败");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤13失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush 
//@DESCRIPTION:测试参数或应用场景不符合设计要求时,接口函数CSUDIINJECTERFlush的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的PES/ES注入器句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL，2) Start注入器之前,3) Stop注入器之后等三种情况下调用此函数均返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、在启动注入之前调用CSUDIINJECTERFlush,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、在hINJECTER == CSUDI_NULL的情况下，调用CSUDIINJECTERFlush，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERStop停止此PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、在hINJECTER 为步骤2得到的PES/ES注入器句柄时，调用CSUDIINJECTERFlush，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤6~步骤9共1000次
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭注入器
//@EXECUTIONFLOW:12、关闭步骤2打开的播放器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_Flush_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
		
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "步骤4失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤5失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(CSUDI_NULL), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "步骤9失败");		
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush                
//@DESCRIPTION:测试参数和应用场景均符合设计要求时,接口函数CSUDIINJECTERFlush的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的PES/ES注入器句柄
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart启动此PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERGetFreeBuffer，期望得到注入器空闲缓冲区的地址和大小，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERWriteComplete，通知底层注入数据已准备好，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERFlush，等待本次注入的数据播放完，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复步骤6~步骤8共5次
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此PES/ES注入器
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭此PES/ES注入器
//@EXECUTIONFLOW:12、关闭步骤2打开的播放器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_Flush_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
		
	int i= 0;

	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	for (i=0;i<5; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer(hINJECTER, &pvBuffer, &uLength), "步骤6失败");	

		if (uLength > 0)
		{
			memset(pvBuffer,0x0,uLength);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERWriteComplete (hINJECTER,uLength), "步骤7失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER), "步骤8失败");
		}
		else
		{
			CSUDIOSThreadSleep(10);
		}
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤10失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERClear 
//@DESCRIPTION:测试参数或应用场景不符合设计要求时,接口函数CSUDIINJECTERClear的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的PES/ES注入器句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL，2) Start注入器之前,3) Stop注入器之后等三种情况下调用此函数均返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、在启动注入之前调用CSUDIINJECTERFlush,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、在hINJECTER == CSUDI_NULL的情况下，调用CSUDIINJECTERClear，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERStop停止此PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、在hINJECTER 为步骤2得到的PES/ES注入器句柄时，调用CSUDIINJECTERFlush，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤6~步骤9共1000次
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭注入器
//@EXECUTIONFLOW:12、关闭步骤2打开的播放器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_Clear_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	int i= 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "步骤4失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤5失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(CSUDI_NULL), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "步骤9失败");		
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush                
//@DESCRIPTION:测试参数和应用场景均符合设计要求时,接口函数CSUDIINJECTERFlush的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的PES/ES注入器句柄
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart启动此PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERGetFreeBuffer，期望得到注入器空闲缓冲区的地址和大小，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERWriteComplete，通知底层注入数据已准备好，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERFlush，等待本次注入的数据播放完，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复步骤6~步骤8共1000次
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此PES/ES注入器
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭此PES/ES注入器
//@EXECUTIONFLOW:12、关闭步骤2打开的播放器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_Clear_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
		
	int i= 0;

	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");	

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	for (i=0;i<REPEAT_TIMES; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer(hINJECTER, &pvBuffer, &uLength), "步骤6失败");	

		if (uLength > 0)
		{
			memset(pvBuffer,0x0,uLength);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERWriteComplete (hINJECTER,uLength), "步骤7失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER), "步骤8失败");
		}
		else
		{
			CSUDIOSThreadSleep(10);
		}
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤10失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤11失败");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERGetFreeBuffer
//@DESCRIPTION:测试接口参数和应用场景不符合设计要求时,接口函数CSUDIINJECTERGetFreeBuffer的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的PES/ES注入器句柄
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:1) Start之前，2)Stop之后，3)注入句柄为CSUDI_NULL，4) 缓冲区参数指针为CSUDI_NULL，5)缓冲区长度参数为CSUDI_NULL，上列5种情况下，函数均应返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、所有参数均正常的情况下，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、hINJECTER为CSUDI_NULL，调用CSUDIINJECTERGetFreeBuffer，期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8、ppvBuffer为CSUDI_NULL，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、puLength为CSUDI_NULL，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此PES/ES注入器
//@EXECUTIONFLOW:11、所有参数均正常的情况下，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、重复步骤6~步骤10共1000次
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClose，关闭此PES/ES注入器
//@EXECUTIONFLOW:14、关闭步骤2打开的播放器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_GetFreeBuffer_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
		
	int i= 0;

	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "步骤5失败");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERGetFreeBuffer (CSUDI_NULL,&pvBuffer,&uLength), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,CSUDI_NULL), "步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer(hINJECTER, CSUDI_NULL, &uLength), "步骤9失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤10失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "步骤11失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤13失败");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERWriteComplete
//@DESCRIPTION:测试接口参数或应用场景不符合设计要求时,接口函数CSUDIINJECTERWriteComplete的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的PES/ES注入器句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:1) Start之前，2)Stop之后，3)注入句柄为CSUDI_NULL，4) uWrittenSize比前期得到的空闲长度大，上列4种情况下，函数均应返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERWriteComplete试图写入1字节数据，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此PES/ES注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERGetFreeBuffer得到空闲缓冲区地址和长度
//@EXECUTIONFLOW:8、hINJECTER为CSUDI_NULL，uWrittenSize为步骤7得到的空闲缓冲区长度，调用CSUDIINJECTERWriteComplete，期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:9、hINJECTER为步骤2得到的句柄，uWrittenSize为步骤7得到的空闲缓冲区长度加1，调用CSUDIINJECTERWriteComplete，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此PES/ES注入器
//@EXECUTIONFLOW:11、hINJECTER为步骤2得到的句柄，uWrittenSize为步骤7得到的空闲缓冲区长度，调用CSUDIINJECTERWriteComplete，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、重复步骤6~步骤11共1000次
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClose，关闭此PES/ES注入器
//@EXECUTIONFLOW:14、关闭步骤2打开的播放器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_WriteComplete_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	int i= 0;

	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤3失败");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER,1), "步骤5失败");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERWriteComplete (CSUDI_NULL,uLength), "步骤8失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER, uLength+1), "步骤9失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤10失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER, uLength), "步骤11失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤13失败");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1、CSUDIINJECTERWriteComplete     
//@CASEGROUP:2、CSUDIINJECTERGetFreeBuffer   
//@DESCRIPTION:测试路径CSUDIINJECTERGetFreeBuffer-->停止Player-->更新缓存区数据-->CSUDIINJECTERWriteComplete，检查驱动的运行状态
//@PRECONDITION:INJECTER模块被正常初始化，ES Player能正常工作
//@INPUT:1、所有参数均正常
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PES/ES注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置"同步"属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、根据步骤1获取的资源，正常打开一个播放器，得到播放器句柄
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，启动此PES/ES注入器
//@EXECUTIONFLOW:6、调用CSUDIINJECTERGetFreeBuffer ，使ppvBuffer  非空，puLength 非空
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStop，停止播放器
//@EXECUTIONFLOW:8、修改ppvBuffer中的内容
//@EXECUTIONFLOW:9、调用CSUDIINJECTERWriteComplete ,写入数据，期望非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERClose，关闭播放器
//@EXECUTIONFLOW:11、调用CSUDIINJECTERStop，停止此注入器
//@EXECUTIONFLOW:12、调用CSUDIINJECTERClose，关闭此注入器
CSUDI_BOOL CSTC_PESES_INJECTER_IT_WriteComplete_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;	
	
	void * pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(EM_INJECT_PESES_VIDEO_MPEG2,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength),"步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(uLength > 0,"步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤8失败");			
	
	memset(pvBuffer,0xFE,uLength);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIINJECTERWriteComplete(hINJECTER, uLength),"步骤9失败");
	 
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤10失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤11失败");		

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤12失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_VIDEO_MPEG2 PES/ES码流，正常播放的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定MPEG2视频
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放视频
//@EXECUTIONFLOW:9、提示用户确认，是否能视频节目播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0001(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_VIDEO_MPEG2;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_PESES_VIDEO_MPEG2 码流，正常播放的情况");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSTCPrint("测试注入EM_INJECT_PESES_VIDEO_MPEG2 码流，正常播放的情况");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("视频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤12失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤13失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_VIDEO_MPEG2 PES/ES码流，进行播放、暂停和恢复暂停
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定ES视频，能控制ES播放的正常播放、暂停与恢复播放
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放视频
//@EXECUTIONFLOW:9、提示用户确认，视频是否播放出来
//@EXECUTIONFLOW:10、调用CSUDIPLAYERPause ，暂停ES播放
//@EXECUTIONFLOW:11、提示用户确认，视频是否停止
//@EXECUTIONFLOW:12、调用CSUDIPLAYERResume ，恢复ES播放
//@EXECUTIONFLOW:13、重复步骤9-步骤12共5次
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0002(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_VIDEO_MPEG2;
			
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	CSTCPrint("测试注入EM_INJECT_PESES_VIDEO_MPEG2  码流，进行播放、暂停和恢复暂停");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("视频播放出来了吗？\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤10失败");
		
		CSTCPrint("视频停止播放了吗？\n");
		CSTCPrint("Does it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤12失败");
		
	}
	
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤15失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤16失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤17失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤18失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤19失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_VIDEO_MPEG2 PES/ES码流，进行播放、停止操作
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定ES视频，能控制播放中的停止与恢复
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放视频
//@EXECUTIONFLOW:9、提示用户确认，视频是否播放出来
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop ，停止ES播放
//@EXECUTIONFLOW:11、提示用户确认，视频是否停止
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart ，恢复ES播放
//@EXECUTIONFLOW:13、重复步骤9-步骤12共5次
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0003(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_VIDEO_MPEG2;
			
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_PESES_VIDEO_MPEG2   码流，进行播放、停止操作");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("视频播放出来了吗？\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败");
		
		CSTCPrint("视频停止播放了吗？\n");
		CSTCPrint("Does it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤12失败");
		
	}
	
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤15失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤16失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤17失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤18失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤19失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_VIDEO_MPEG2 PES/ES码流，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在指定的PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  CSUDIINJECTERFlush等待已注入的内容播放完全才能返回
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放视频
//@EXECUTIONFLOW:9、提示用户确认，视频是否播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIINJECTERFlush ，等待已注入的数据全部播放完，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0004(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_VIDEO_MPEG2;
			
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_PESES_VIDEO_MPEG2 码流，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("视频频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"步骤11失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:"测试注入EM_INJECT_PESES_VIDEO_MPEG2 码流，正常注入结束，调用CSUDIINJECTERClear清除已注入但未播放完的数据
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  CSUDIINJECTERClear能立即返回
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放视频
//@EXECUTIONFLOW:9、提示用户确认，视频是否播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClear ，清除已注入的数据，期望视频立即停止，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0005(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_VIDEO_MPEG2;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_PESES_VIDEO_MPEG2 码流，正常注入结束，调用CSUDIINJECTERClear清除已注入但未播放完的数据");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("视频频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"步骤11失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_VIDEO_H264 PES/ES码流，正常播放的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定H264视频
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放视频
//@EXECUTIONFLOW:9、提示用户确认，是否能视频节目播放出来?//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0006(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;

	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_VIDEO_H264;

	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_PESES_VIDEO_H264 码流，正常播放的情况");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("视频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤12失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤13失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_AUDIO_MPEG2 PES/ES码流，正常播放的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定MPEG2音频
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:9、提示用户确认，是否能音频节目播放出来?//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0007(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	

	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_AUDIO_MPEG2;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	CSTCPrint("此用例测试:注入EM_INJECT_PESES_AUDIO_MPEG2 码流，正常播放的情况");

	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤12失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤13失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_AUDIO_AAC PES/ES码流，正常播放的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定AAC音频
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:9、提示用户确认，是否能音频节目播放出来?//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0008(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_AUDIO_AAC;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_PESES_AUDIO_AAC 码流，正常播放的情况");

	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤12失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤13失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_AUDIO_MPEG2 PES/ES码流，进行播放、暂停和恢复暂停
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定EM_INJECT_PESES_AUDIO_MPEG2流，能控制正常播放、暂停与恢复播放
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:9、提示用户确认，音频频是否播放出来
//@EXECUTIONFLOW:10、调用CSUDIPLAYERPause ，暂停ES播放
//@EXECUTIONFLOW:11、提示用户确认，音频是否停止
//@EXECUTIONFLOW:12、调用CSUDIPLAYERResume ，恢复ES播放
//@EXECUTIONFLOW:13、重复步骤9-步骤12共5次
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0009(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;

	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_AUDIO_MPEG2;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_PESES_AUDIO_MPEG2 码流，进行播放、暂停和恢复暂停");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("音频播放出来了吗？\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤10失败");
		
		CSTCPrint("音频停止播放了吗？\n");
		CSTCPrint("Does it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤12失败");
		
	}
	
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤15失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤16失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤17失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤18失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤19失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_AUDIO_MPEG2 PES/ES码流，进行播放、停止操作
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定音频流，能控制播放中的停止与恢复
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:9、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop ，停止ES播放
//@EXECUTIONFLOW:11、提示用户确认，音频是否停止
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart ，恢复ES播放
//@EXECUTIONFLOW:13、重复步骤9-步骤12共5次
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0010(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;

	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	int i= 0;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_AUDIO_MPEG2;
	
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	CSTCPrint("测试注入EM_INJECT_PESES_AUDIO_MPEG2 码流，进行播放、停止操作");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("音频播放出来了吗？\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败");
		
		CSTCPrint("音频停止播放了吗？\n");
		CSTCPrint("Does it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤12失败");
		
	}
	
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤14失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤15失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤16失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤17失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤18失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤19失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_AUDIO_MPEG2 PES/ES码流，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在指定的PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  CSUDIINJECTERFlush等待已注入的内容播放完全才能返回
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:9、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIINJECTERFlush ，等待已注入的数据全部播放完，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0011(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_AUDIO_MPEG2;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	CSTCPrint("测试注入EM_INJECT_PESES_AUDIO_MPEG2 码流，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"步骤11失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_AUDIO_MPEG2 码流，正常注入结束，调用CSUDIINJECTERClear清除已注入数据
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  CSUDIINJECTERClear能立即返回
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:9、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClear ，清除已注入的数据，期望音频立即停止，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0012(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_AUDIO_MPEG2;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);
	
	CSTCPrint("测试注入EM_INJECT_PESES_AUDIO_MPEG2 码流，正常注入结束，调用CSUDIINJECTERClear清除已注入数据");

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"步骤11失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤13失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_VIDEO_MPEG2 PES/ES码流，但播放器打开的是音频播放器的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  不能播放出指定MPEG2视频
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放视频
//@EXECUTIONFLOW:9、提示用户确认，视频节目不能播放出来?
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0013(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_VIDEO_MPEG2;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");

	sStreamInfo.m_eContentType = EM_UDI_CONTENT_AUDIO;	
	sStreamInfo.m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_MPEG2;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSTCPrint("测试注入EM_INJECT_PESES_VIDEO_MPEG2 码流，但播放器打开的是音频播放器的情况");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("请确认视频节目不能播放出来\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤12失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤13失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入EM_INJECT_PESES_AUDIO_MPEG2 PES/ES码流，但播放器打开的是视频播放器的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  不能播放出指定音频
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:9、提示用户确认，音频节目内容不能播放出来?
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0014(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_AUDIO_MPEG2;
		
	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");

	sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;	
	sStreamInfo.m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSTCPrint("测试注入EM_INJECT_PESES_AUDIO_MPEG2 码流，但播放器打开的是视频播放器的情况");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("请确认音频节目内容不能播放出来\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤12失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤13失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入的数据大于通过CSUDIINJECTERGetFreeBuffer得到的缓冲区大小
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PES/ES播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  不能播放出指定MPEG2视频
//@EXECUTIONFLOW:1、根据指定的PES/ES码流，获取该码流正常情况的解码资源参数
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入ES  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，设置"同步"属性
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、创建一个线程,循环读取ES   流文件，并进行ES注入操作
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream ，设置好ES的流信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放视频
//@EXECUTIONFLOW:9、提示用户确认，视频节目内容不播放出来?
//@EXECUTIONFLOW:10、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PESES_INJECTER_IT_INJECTER_0015(void)
{
	CSUDIPESSYNCMode_S 	sSyncMode;
	
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S sStreamInfo;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;

	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	ES_ServiceIndex_E eServiceStream = EM_INJECT_PESES_VIDEO_MPEG2;

	memset(&sInjecterChnl,-1,sizeof(sInjecterChnl));
	memset(&sPlayerChnl,-1,sizeof(sPlayerChnl));	
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	memset(&sOpenParams,0,sizeof(sOpenParams));

	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"没有指定的解码器");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask_OverLengh, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "步骤6失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤8失败");

	CSTCPrint("测试注入EM_INJECT_PESES_VIDEO_MPEG2 码流，注入的数据大于通过CSUDIINJECTERGetFreeBuffer得到的缓冲区大小");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("视频节目内容不能播放出来吗？\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //停止注入

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤12失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤13失败");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤14失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");

	return CSUDI_TRUE;
}

