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
#include "cs_pcm_injectertestcase.h"
#include "udi2_fs.h" 
#include "udi2_os.h"
#include "udi2_player.h"
#include "udi2_inject.h"
#include  "cs_fs_plus.h"
#include "udiplus_os.h"
#include "cs_testkit.h"
#include "cs_testkit_porting.h"
#include "pcm_sin.h"

#define REPEAT_TIMES 10
#define LOOP_TIMES 3

#define ERRORDATA (2009)
#define WAITINJECTTIME (1000)   
#define INJECTTHREADPRO (100)
#define INJECTTHREADBUF (1024 * 148)
#define AUDIODECODER (0)
#define INJECT_SEEK_SET (0)
#define INJECT_SEEK_CUR (1)
#define INJECT_SEEK_END (2)
#define PCMServiceCunt  (3) 
#define PCM_BUFFER_SIZE (1*1024*1024) //限制每次注入的大小

static CSUDI_HANDLE g_hPcmHandle = (CSUDI_HANDLE)NULL;
static CSUDI_HANDLE g_hInjectThread = (CSUDI_HANDLE)NULL;
static CSUDI_BOOL g_bEnterTask = CSUDI_FALSE;

typedef enum 
{
    EM_INJECT_TS = 0, /*湖南卫视*/
    EM_INJECT_PCM_BIG, /*pcm文件，大端字节序*/
    EM_INJECT_PCM_LIT  /*pcm文件，小端字节序*/
}INJECTServiceIndex_E;

typedef struct PCMPlayInfo_S
{
    char  m_pcName[64]; //注入文件的名
    CSUDIINJECTERPcmStartParam_S m_StartParam;//PCM播放参数,ts流时该项为0
    CSUDIINJECTERPcmEndian_E m_enmEndian;//文件数据的大小端字节序特性 ,ts时该项为0   
}PCMPlayInfo_S;

static PCMPlayInfo_S g_PCMPlayInfo[PCMServiceCunt] =
{
    {
        {""},
        {0, 0, 0},
        0,
    },
    {
        {""},
        {16, 44100, 2},
        EM_UDIINJECTER_BIG_ENDIAN,
    },
    {
        {""},
        {16, 44100, 2},
        EM_UDIINJECTER_LITTLE_ENDIAN,
    }
};

#if 0
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
#endif

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

static void getInjecterFile(const char *pFileName, void **file, long *pLength)
{
	void *pFileTemp = NULL;
	int nReturn = -1;
	 
	pFileTemp = InjectFSOpen(pFileName, "r");

	if (!pFileTemp)
	{
		CSTCPrint("测试所需文件 %s 打开失败，请确保testdata目录下存在该文件!!!\n", pFileName);
		CSTCPrint("[in getInjecterFile]InjectFSOpen is failed!file name is %s\n", pFileName);
		return;
	}
	
	nReturn = InjectFSSeek(pFileTemp,0,INJECT_SEEK_END);
	if(nReturn != 0)
	{
		CSTCPrint("[in getInjecterFile]InjectFSSeek end is failed!\n\r");
		return;	
	}
	
	*pLength = InjectFSTell(pFileTemp);
	if(*pLength < 0)
	{
		CSTCPrint("[in getInjecterFile]InjectFSTell is failed!\n\r");
		return;	
	}

	nReturn = InjectFSSeek(pFileTemp,0,INJECT_SEEK_SET);
	if(nReturn != 0)
	{
		CSTCPrint("[in getInjecterFile]InjectFSSeek begin is failed!\n\r");
		return;	
	}

	*file = pFileTemp;
	
}

// 该变量为CSUDI_TRUE时，则当注入完当前文件中的pcm数据时会退出注入线程pcm_injectTask
static CSUDI_BOOL g_bInjectOnlyOnce = CSUDI_FALSE;

static void pcm_injectTask(void * PCMPlayInfo)
{
	void * ppvBuffer = NULL;
	unsigned  int  uLength = 0;
	PCMPlayInfo_S *pPCMPlayInfo = (PCMPlayInfo_S*)PCMPlayInfo;
	CSUDI_HANDLE hPcm = g_hPcmHandle;

	int nBlockNum = 0;
	long nFileAllLength = 0;  //文件总长度
	int nFileCurrentPos = 0; //文件当前位置
	int nFileLeftLength = 0; //剩余未读文件长度
	int nInjectedLength = 0; //可注入的大小
	int nReadLength = 0;

	void* file = NULL;
	getInjecterFile((const char*)pPCMPlayInfo->m_pcName, &file, &nFileAllLength);

	if(file==NULL || nFileAllLength <= 0)
	{
		return;
	}
	
	while(g_bEnterTask)
	{
		//求出当前剩余未读文件的长度
		nFileCurrentPos=InjectFSTell(file);	//当前已读文件的长度
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度

		if(nFileLeftLength == 0)
		{
			if (g_bInjectOnlyOnce)
			{
				g_bInjectOnlyOnce = CSUDI_FALSE;
				g_bEnterTask = CSUDI_FALSE;
				break;
			}
			
			InjectFSSeek(file,0,INJECT_SEEK_SET);
			nFileLeftLength=nFileAllLength;
		}

        uLength = 0;
		
		if(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERGetFreeBuffer (hPcm,&ppvBuffer,&uLength)) 
		{
			CSTCPrint("[pcm_injectTask] CSUDIINJECTERGetFreeBuffer FAILED!\n");
			break;
		}
				
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
		CSUDIOSThreadSleep(10);
	}

	if(!g_bEnterTask)
	{
		InjectFSClose(file);	
	}
}

static void pcm_injectTask_overLength(void * PCMPlayInfo)
{
	void * ppvBuffer = NULL;
	unsigned  int  uLength = 0;
	PCMPlayInfo_S *pPCMPlayInfo = (PCMPlayInfo_S*)PCMPlayInfo;
	CSUDI_HANDLE hPcm = g_hPcmHandle;

	long nFileAllLength = 0;  //文件总长度
	int nFileCurrentPos = 0; //文件当前位置
	int nFileLeftLength = 0; //剩余未读文件长度
	void *file;
	
	getInjecterFile((const char*)pPCMPlayInfo->m_pcName, &file, &nFileAllLength);
    if(file==NULL || nFileAllLength <= 0)
    {
        return;
    }

	while(g_bEnterTask)
	{
		//求出当前剩余未读文件的长度
		nFileCurrentPos=InjectFSTell(file);	//当前已读文件的长度
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //剩余未读文件的长度
		
        uLength = 0;
			  
		CSUDIINJECTERGetFreeBuffer (hPcm,&ppvBuffer,&uLength);
		
		if(uLength > 0)
		{
			InjectFSRead(file, (void*)ppvBuffer, uLength); //读取相应长度的文件
			
			if(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete(hPcm,(uLength+10))) 
			{
				CSTCPrint("[pcm_injectTask_overLength] CSUDIINJECTERWriteComplete FAILED!\n");
				break;
			}
					
			if(nFileLeftLength == 0)
			{
				InjectFSSeek(file,0,INJECT_SEEK_SET);
			}			
		}
		CSUDIOSThreadSleep(10);
	}

	if(!g_bEnterTask)
		InjectFSClose(file);	
}

static void getPcmInjectInfo(INJECTServiceIndex_E eFileType,CSUDIINJECTERChnl_S  *psInjecterChnl,CSUDIPlayerChnl_S  *psPlayerChnl, CSUDIINJECTEROpenParam_S   *psOpenParams,CSUDIINJECTERPcmStartParam_S *psPcmStartParam,CSUDIStreamInfo_S  sStreamInfo[])
{
	int nAudioId = -1;
	
	nAudioId = searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);
	if(nAudioId ==-1)
	{
		CSTCPrint( "没有支持PCM  的音频解码器!\n\r");
        return;
	}
	if ((eFileType != EM_INJECT_PCM_LIT) && (eFileType != EM_INJECT_PCM_BIG))
	{
		CSTCPrint( "没有指定大小端!\n\r");
        return;
	}

    CSTCPrint( "PCM  的音频解码器=%d!\n\r",nAudioId);
	
//注入的音频解码器
	psInjecterChnl->m_nDemux = CSUDI_INVALID_INDEX;
	psInjecterChnl->m_nAudioDecoder=nAudioId;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;
	
//注入类型与内容	
	psOpenParams->m_eInjecterType=EM_INJECTER_PCM;
	psOpenParams->m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

//设置节目信息
	sStreamInfo[0].m_nPid=0;
	sStreamInfo[0].m_eContentType=EM_UDI_CONTENT_AUDIO;
	sStreamInfo[0].m_uStreamType.m_eAudioType=EM_UDI_AUD_STREAM_PCM  ;

	psPlayerChnl->m_nDemux=CSUDI_INVALID_INDEX;
	psPlayerChnl->m_nAudioDecoder=nAudioId;
	psPlayerChnl->m_nVideoDecoder=CSUDI_INVALID_INDEX;

	psPcmStartParam->m_uBitsSample=g_PCMPlayInfo[eFileType].m_StartParam.m_uBitsSample;
	psPcmStartParam->m_uSampleRate=g_PCMPlayInfo[eFileType].m_StartParam.m_uSampleRate;
	psPcmStartParam->m_uChannels=g_PCMPlayInfo[eFileType].m_StartParam.m_uChannels;

	return;
}

static CSUDI_HANDLE openInjectPlayer(int nDemuxID, int nVideoID, int nAudioID, CSUDIStreamInfo_S  *psStreamInfo, int nStreamCnt)
{
	CSUDIPlayerChnl_S sPlayerChnl;
		
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	
	sPlayerChnl.m_nAudioDecoder = nAudioID;
	
	sPlayerChnl.m_nVideoDecoder = nVideoID;
	
	sPlayerChnl.m_nDemux = nDemuxID;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "打开播放器失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,psStreamInfo,nStreamCnt,CSUDI_NULL), "设置流失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "启动播放失败");

	CSTK_FATAL_POINT;

	return hPlayer;
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


static CSUDI_HANDLE OpenPCMInectPlayer(int nAudioID)
{
	CSUDI_HANDLE hPlayer= CSUDI_NULL;
	
	CSUDIStreamInfo_S  sStreamInfo[2];
	memset(&sStreamInfo,0,sizeof(sStreamInfo));
	
	sStreamInfo[0].m_nPid =0;
	sStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStreamInfo[0].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_PCM;	
	
	hPlayer = openInjectPlayer(-1,-1,nAudioID,&sStreamInfo[0],1);

	return hPlayer;
}


static void task_For_PCMOpenColse(void *pvData)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDI_HANDLE  hINJECTER;
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	int nAudioDecoderId = -1;
		
	int i= 0;
		
	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	nAudioDecoderId=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "步骤一失败:没有找到支持pcm的音频解码器");

	
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioDecoderId;
	
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


static void task_For_PCMStartStop(void * hInjecterHander )
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

static CSUDI_BOOL PCM_Init(void)
{    
	CSUDI_BOOL besult = CSUDI_FALSE;
	static int InitFlag = CSUDI_FALSE;

	if (InitFlag)
	{
		return CSUDI_TRUE;
	}
    
	InitFlag = CSUDI_TRUE;

	strcat(g_PCMPlayInfo[0].m_pcName, "Player_Mpeg_FreeService.ts");
	strcat(g_PCMPlayInfo[1].m_pcName, "better man.wav");
	strcat(g_PCMPlayInfo[2].m_pcName, "Amani.wav");
	besult = CSUDI_TRUE;
    
	CSUDIAUDIOSetVolume(0, 31);	
    
	return besult;
}

CSUDI_BOOL CSTC_PCM_INJECTER_Init(void)
{
	//在本测试用例集执行前调用
	//CSUDIFSInit();
	//CSUDIOSThreadSleep(3000);
	
	if(!PCM_Init())
	{
		CSTCPrint("初始化失败\n");
		return CSUDI_FALSE;
	}
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PCM_INJECTER_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试Open/Close PCM注入器时，在参数不符合设计要求情况下的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:参数不符合PCM注入器接口设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、psInjecterChnl->m_nDemux 不等于-1时，调用CSUDIINJECTEROpen试图打开一个PCM注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:3、psOpenParams->m_eContentType不为EM_UDIINJECTER_CONTENT_AUDIO时，调用CSUDIINJECTEROpen试图打开一个PCM注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、phINJECTER = CSUDI_NULL 时，调用CSUDIINJECTEROpen试图打开一个PCM注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、psInjecterChnl = CSUDI_NULL时，调用CSUDIINJECTEROpen试图打开一个PCM注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、psOpenParams = CSUDI_NULL时，调用CSUDIINJECTEROpen试图打开一个PCM注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、hINJECTER = CSUDI_NULL时，调用CSUDIINJECTERClose试图关闭一个无效的PCM注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、重复步骤2 至步骤7，共100次
CSUDI_BOOL CSTC_PCM_INJECTER_IT_OpenClose_0001(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	
	int nAudioDecoderId = -1;
	
	int i=0;

	nAudioDecoderId=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "步骤一失败:没有找到支持pcm的音频解码器");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_VIDEO;
		sInjecterChnl.m_nDemux=-1;
		sInjecterChnl.m_nVideoDecoder=-1;		
		sInjecterChnl.m_nAudioDecoder = nAudioDecoderId;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤二失败");

		sOpenParams.m_eInjecterType=EM_INJECTER_PCM;		
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
		sInjecterChnl.m_nDemux=ERRORDATA;
		sInjecterChnl.m_nVideoDecoder=-1;		
		sInjecterChnl.m_nAudioDecoder = nAudioDecoderId;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤三失败");

		sOpenParams.m_eInjecterType=EM_INJECTER_PCM;		
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
		sInjecterChnl.m_nDemux=-1;
		sInjecterChnl.m_nVideoDecoder=-1;		
		sInjecterChnl.m_nAudioDecoder = nAudioDecoderId;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,CSUDI_NULL), "步骤四失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (CSUDI_NULL,&sOpenParams,&hINJECTER), "步骤五失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,CSUDI_NULL,&hINJECTER), "步骤六失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(CSUDI_NULL), "步骤七失败");

	}

	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试PCM注入器正常打开与关闭情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、合法sInjecterChnl，psInjecterChnl->m_nDemux 为-1
//@INPUT:2、sOpenParams-> m_eInjecterType = EM_INJECTER_PCM
//@INPUT:	  sOpenParams-> m_eContentType = EM_UDIINJECTER_CONTENT_AUDIO
//@INPUT:3、phINJECTER非空
//@EXPECTATION:返回CSUDI_SUCCESS  
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERClose，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复步骤2和步骤3共1000次。
CSUDI_BOOL CSTC_PCM_INJECTER_IT_OpenClose_0002(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	int nAudioDecoderId = -1;
		
	int i= 0;
		
	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	nAudioDecoderId=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "步骤一失败:没有找到支持pcm的音频解码器");
	
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioDecoderId;
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "步骤二失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤三失败");
	}
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试PCM 注入器Open和Close 不成对调用，多次Close的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 非空
//@INPUT:2、psOpenParams-> m_eInjecterType=EM_INJECTER_PCM   ,
//@INPUT:	  psOpenParams-> m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO   
//@EXPECTATION:返回非CSUDI_SUCCESS  
//@EXECUTIONFLOW:1、查找支持PCM注入的音频解码器或者音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，正常打开一个PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTEROpen，正常打开一个PCM注入器，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERClose，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERClose，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、重复步骤2至步骤5，共100次
CSUDI_BOOL CSTC_PCM_INJECTER_IT_OpenClose_0003(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDI_HANDLE  hINJECTER=CSUDI_NULL, hINJECTER1 = CSUDI_NULL;
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	int nAudioDecoderId = -1;
	
	int i= 0;
		
	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	nAudioDecoderId=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "步骤一失败:没有找到支持pcm的音频解码器");
	
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioDecoderId;
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "步骤二失败");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS !=  CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER1)), "步骤三失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤四失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(hINJECTER), "步骤五失败");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:多线程交叉调用CSUDIINJECTEROpen & CSUDIINJECTERClose的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 合法的
//@INPUT:2、psOpenParams-> m_eInjecterType=EM_INJECTER_PCM   ,
//@INPUT:	  psOpenParams-> m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO   
//@EXPECTATION:不死机，不死锁，逻辑正确
//@EXECUTIONFLOW:1、查找支持PCM注入的音频解码器或者音频通道
//@EXECUTIONFLOW:2、启动另外一个任务，该任务同时并异步执行下面的步骤3和4
//@EXECUTIONFLOW:3、调用CSUDIINJECTEROpen，正常打开一个PCM注入器，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:4、如果步骤3成功，则调用CSUDIINJECTERClose，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、重复步骤2至步骤4，共1000次
CSUDI_BOOL CSTC_PCM_INJECTER_IT_OpenClose_0004(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	int nAudioDecoderId = -1;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	int i= 0;
		
	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	nAudioDecoderId=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "步骤一失败:没有找到支持pcm的音频解码器");

	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioDecoderId;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OpenClose", INJECTTHREADPRO, INJECTTHREADBUF, task_For_PCMOpenColse, (void *)0, &hInjectThread), "步骤六失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		udiRe = CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_DEVICE_BUSY == udiRe )
			|| (CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED == udiRe)),"步骤二失败");

		CSUDIOSThreadSleep(2);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤三失败");
		}
	}
	
	CSTK_FATAL_POINT;
	
	if (hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"等待任务完成失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤四失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试PCM注入事例句柄为CSUDI_NULL时，Start/Stop函数的执行效果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER = CSUDI_NULL
//@EXPECTATION:Start/Stop函数均返回CSUDIINJECTER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、正常打开PCM音频注入器，得到PCM注入器句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、以hINJECTER = CSUDI_NULL,调用CSUDIINJECTERStart,期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:6、以hINJECTER = CSUDI_NULL,调用CSUDIINJECTERStop,期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:7、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_StartStop_0001(void)
{
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	int nAudioID=-1;
	CSUDI_HANDLE hINJECTER= CSUDI_NULL;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	
	nAudioID=searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
	
//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;
	
	sPcmStartParam.m_uBitsSample = 16;
	sPcmStartParam.m_uChannels = 2;
	sPcmStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStart(CSUDI_NULL), "步骤5失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStop(CSUDI_NULL), "步骤6失败");

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤7失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试正常情况下，CSUDIINJECTERStart和CSUDIINJECTERStop成对调用的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为正常打开的PCM注入事例句柄
//@EXPECTATION:CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、正常打开PCM音频注入器，得到PCM注入器句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart,启动此PCM注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStop,停止此PCM注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:7、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_StartStop_0002(void)
{
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	int nAudioID=-1;
	CSUDI_HANDLE hINJECTER= CSUDI_NULL;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	int i=0;
	
	nAudioID=searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
	
//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sPcmStartParam.m_uBitsSample = 16;
	sPcmStartParam.m_uChannels = 2;
	sPcmStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤6失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤7失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试正常情况下，CSUDIINJECTERStart和CSUDIINJECTERStop不成对或逆序调用的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为正常打开的PCM注入事例句柄
//@EXPECTATION:返回非CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、正常打开PCM音频注入器，得到PCM注入器句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStop,停止此PCM注入器，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart,启动此PCM注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:7、调用CSUDIINJECTERStart,启动此PCM注入器，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:8、调用CSUDIINJECTERStop,停止此PCM注入器，期望返回CSUDI_SUCCESS 
//@EXECUTIONFLOW:9、调用CSUDIINJECTERStop,停止此PCM注入器，期望返回非CSUDI_SUCCESS 
//@EXECUTIONFLOW:10、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_StartStop_0003(void)
{
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	int nAudioID=-1;
	CSUDI_HANDLE hINJECTER= CSUDI_NULL;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	int i=0;
	
	nAudioID=searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
	
//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sPcmStartParam.m_uBitsSample = 16;
	sPcmStartParam.m_uChannels = 2;
	sPcmStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	for (i=0;i<100;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStart(hINJECTER), "步骤7失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤8失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "步骤9失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤10失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:多线程交叉调用CSUDIINJECTERStart & CSUDIINJECTERStop的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 合法的
//@INPUT:2、psOpenParams-> m_eInjecterType=EM_INJECTER_PCM   ,
//@INPUT:	  psOpenParams-> m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO   
//@EXPECTATION:不死机，不死锁，逻辑正确
//@EXECUTIONFLOW:1、查找支持PCM注入的音频解码器或者音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，正常打开一个PCM注入器，得到注入器句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、启动另外一个任务，该任务同时并异步执行下面的步骤4和5
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart，启动此PCM注入器，期望返回CSUDI_SUCCESS或CSUDIREC_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:7、调用CSUDIINJECTERStop，停止此PCM注入器，期望返回CSUDI_SUCCESS或CSUDIREC_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:8、重复步骤6至步骤7，共1000次
//@EXECUTIONFLOW:9、调用CSUDIOSThreadJoin，等待步骤5启动的任务执行完成，关闭退出
//@EXECUTIONFLOW:10、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_StartStop_0004(void)
{
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	int nAudioDecoderId = -1;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	
	CSUDI_Error_Code udiRe = CSUDI_SUCCESS;
	
	int i= 0;
		
	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	nAudioDecoderId=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "步骤一失败:没有找到支持pcm的音频解码器");
	
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioDecoderId;
	
	sPcmStartParam.m_uBitsSample = 16;
	sPcmStartParam.m_uChannels = 2;
	sPcmStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("PCMStartStop", INJECTTHREADPRO, INJECTTHREADBUF, task_For_PCMStartStop, (void *)hINJECTER, &hInjectThread), "步骤5失败");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		udiRe = CSUDIINJECTERStart(hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STARTED == udiRe )),"步骤6失败");

		CSUDIOSThreadSleep(2);


		udiRe =  CSUDIINJECTERStop(hINJECTER);
		
		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STOPPED == udiRe )),"步骤7失败");
	}
	
	CSTK_FATAL_POINT;
	
	if (hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤8失败");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤9失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤10失败");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试在PCM注入模式，接口参数不符合设计要求的情况下，CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute的执行效果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、不符合接口设计要求 
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PCM注入器并得到句柄
//@EXECUTIONFLOW:3、hINJECTER = CSUDI_NULL时，调用CSUDIINJECTERSetAttribute设置属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、pvData = CSUDI_NULL,时，调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:5、pvData = CSUDI_NULL,时，调用CSUDIINJECTERSetAttribute 设置PCM启动参数属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:6、pvData = CSUDI_NULL,时，调用CSUDIINJECTERSetAttribute 设置PCM回调函数属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERSetAttribute 设置只读属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERSetAttribute 设置非法属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、hINJECTER = CSUDI_NULL时，调用CSUDIINJECTERGetAttribute获取属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute获取PCM大小端属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:11、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute 获取PCM启动参数属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、pvData = CSUDI_NULL,时，调用CSUDIINJECTERGetAttribute 获取PCM回调函数属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERSetAttribute 获取非法属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:14、重复步骤3~步骤13共1000次
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_SetGetAttribute_0001(void)
{
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;

	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	int nAudioID=-1;
	int i = 0;

	nAudioID=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(CSUDI_NULL,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤3失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,CSUDI_NULL),  "步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,CSUDI_NULL),  "步骤5失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_STATUS, &ePcmEndian),  "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,ERRORDATA, &ePcmEndian),  "步骤8失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(CSUDI_NULL,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤9失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,CSUDI_NULL),  "步骤10失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,CSUDI_NULL),  "步骤11失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "步骤12失败");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,ERRORDATA,CSUDI_NULL),  "步骤13失败");

	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤15失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试在PCM注入模式，接口参数均符合设计要求的情况下，CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute的执行效果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、均符合接口设计要求 
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PCM注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERSetAttribute 设置PCM回调函数属性，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:6、调用CSUDIINJECTERGetAttribute获取PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、判断获取得到的PCM大小端属性为步骤3所设置的属性一致
//@EXECUTIONFLOW:8、调用CSUDIINJECTERGetAttribute获取PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、判断获取得到的PCM流参数属性为步骤4所设置的属性一致
//@EXECUTIONFLOW:10、调用CSUDIINJECTERGetAttribute获取PCM回调函数属性，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:11、如果上一步返回成功，则判断获取得到的PCM回调函数属性为步骤5所设置的属性一致
//@EXECUTIONFLOW:12、重复步骤3~步骤11共1000次
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_SetGetAttribute_0002(void)
{
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;

	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;

	CSUDIINJECTERPcmStartParam_S sPCMStartParam;
		
	int nAudioID=-1;

	int i = 0;
	
	nAudioID=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 16;
	sPCMStartParam.m_uChannels = 2;
	sPCMStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
//设置		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤3失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam),  "步骤4失败");

//获取
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian),  "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL((ePcmEndian== EM_UDIINJECTER_LITTLE_ENDIAN),"步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam),  "步骤8失败");
						
		CSTK_ASSERT_TRUE_FATAL(((sPCMStartParam.m_uBitsSample == 16)
			&&(sPCMStartParam.m_uChannels == 2)
			&&(sPCMStartParam.m_uSampleRate == 44100)),"步骤9失败");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤13失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试PCM注入器在start之后再调用CSUDIINJECTERSetAttribute/CSUDIINJECTERGetAttribute的情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 非空
//@INPUT:2、eAttrType=EM_UDIINJECTER_PCM_ENDIAN  
//@INPUT:3、pvData非空
//@EXPECTATION:调用CSUDIINJECTERSetAttribute    返回CSUDIINJECTER_ERROR_INVALID_STATUS，调用CSUDIINJECTERGetAttribute返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK:启动注入后不能调用CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PCM注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart启动此PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERSetAttribute设置PCM流参数属性，期望返回CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERSetAttribute 设置PCM回调函数属性，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIINJECTERGetAttribute获取PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:10、判断获取得到的PCM大小端属性为步骤3所设置的属性一致
//@EXECUTIONFLOW:11、调用CSUDIINJECTERGetAttribute获取PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、判断获取得到的PCM流参数属性为步骤4所设置的属性一致
//@EXECUTIONFLOW:13、调用CSUDIINJECTERGetAttribute获取PCM回调函数属性，期望返回CSUDI_SUCCESS或CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:14、重复步骤6~步骤13共1000次
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop，停止此PCM注入器
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_SetGetAttribute_0003(void)
{
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S sPCMStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	
	int nAudioID=-1;
	
	int i;
	
	nAudioID=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//设置
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤6失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "步骤7失败");

		//获取
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤9失败");

		CSTK_ASSERT_TRUE_FATAL((ePcmEndian== EM_UDIINJECTER_LITTLE_ENDIAN),"步骤10失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "步骤11失败");

		CSTK_ASSERT_TRUE_FATAL(((sPCMStartParam.m_uBitsSample == 8)
			&&(sPCMStartParam.m_uChannels == 2)
			&&(sPCMStartParam.m_uSampleRate == 44100)),"步骤12失败");		
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "步骤15失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush 
//@DESCRIPTION:测试参数或应用场景不符合设计要求时,接口函数CSUDIINJECTERFlush的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的PCM注入器句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL，2) Start注入器之前,3) Stop注入器之后等三种情况下调用此函数均返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PCM注入器并得到句柄
//@EXECUTIONFLOW:3、在启动注入之前调用CSUDIINJECTERFlush,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERSetAttribute设置PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、在hINJECTER == CSUDI_NULL的情况下，调用CSUDIINJECTERFlush，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERStop停止此PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、在hINJECTER 为步骤2得到的PCM注入器句柄时，调用CSUDIINJECTERFlush，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤6~步骤9共1000次
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_Flush_0001(void)
{
	CSUDI_HANDLE hINJECTER, hPlayer= CSUDI_NULL;
	
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S sPCMStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	
	int nAudioID=-1;
	
	int i;
	
	nAudioID=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");
	
//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "步骤5失败");
	
	hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"打开PCM播放器失败");
    
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
//@INPUT:1、hINJECTER 为有效的PCM注入器句柄
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PCM注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart启动此PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERGetFreeBuffer，期望得到注入器空闲缓冲区的地址和大小，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERWriteComplete，通知底层注入数据已准备好，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERFlush，等待本次注入的数据播放完，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复步骤6~步骤8共10次
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此PCM注入器
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_Flush_0002(void)
{
	CSUDI_HANDLE hINJECTER, hPlayer= CSUDI_NULL;
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S sPCMStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;

	void *pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
	
	int nAudioID=-1;
	
	int i;
	
	nAudioID=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "步骤4失败");

    	hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"打开PCM播放器失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	for (i=0;i<10;i++)
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
//@DESCRIPTION:测试参数或场景不符合设计要求时,接口函数CSUDIINJECTERClear的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的PCM注入器句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL，2) Start注入器之前,3) Stop注入器之后等三种情况下调用此函数均返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PCM注入器并得到句柄
//@EXECUTIONFLOW:3、在启动注入之前调用CSUDIINJECTERClear,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERSetAttribute设置PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、在hINJECTER == CSUDI_NULL的情况下，调用CSUDIINJECTERClear，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERStop停止此PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、在hINJECTER 为步骤2得到的PCM注入器句柄时，调用CSUDIINJECTERClear，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、重复步骤6~步骤9共1000次
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_Clear_0001(void)
{
	CSUDI_HANDLE hINJECTER, hPlayer= CSUDI_NULL;
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S sPCMStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	
	int nAudioID=-1;
	
	int i;
	
	nAudioID=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "步骤5失败");
	
	hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"打开PCM播放器失败");

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

//@CASEGROUP:CSUDIINJECTERClear                
//@DESCRIPTION:测试参数和应用场景均符合设计要求时,接口函数CSUDIINJECTERClear的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:1、hINJECTER 为有效的PCM注入器句柄
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PCM注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart启动此PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERGetFreeBuffer，期望得到注入器空闲缓冲区的地址和大小，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERWriteComplete，通知底层注入数据已准备好，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIINJECTERClear，清除本次注入的数据，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、重复步骤6~步骤8共10次
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此PCM注入器
//@EXECUTIONFLOW:11、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_Clear_0002(void)
{
	CSUDI_HANDLE hINJECTER, hPlayer= CSUDI_NULL;
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S sPCMStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;

	void *pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
	
	int nAudioID=-1;
	
	int i;
	
	nAudioID=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "步骤4失败");
    
    hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"打开PCM播放器失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	for (i=0;i<REPEAT_TIMES;i++)
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
//@INPUT:1、hINJECTER 为有效的PCM注入器句柄
//@EXPECTATION:返回CSUDI_SUCCESS
//@REMARK:1) Start之前，2)Stop之后，3)PCM句柄为CSUDI_NULL，4) 缓冲区参数指针为CSUDI_NULL，5)缓冲区长度参数为CSUDI_NULL，上列5种情况下，函数均应返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PCM注入器并得到句柄
//@EXECUTIONFLOW:3、所有参数均正常的情况下，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERSetAttribute设置PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、hINJECTER为CSUDI_NULL，调用CSUDIINJECTERGetFreeBuffer，期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8、ppvBuffer为CSUDI_NULL，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:9、puLength为CSUDI_NULL，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此PCM注入器
//@EXECUTIONFLOW:11、所有参数均正常的情况下，调用CSUDIINJECTERGetFreeBuffer，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、重复步骤6~步骤10共1000次
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_GetFreeBuffer_0001(void)
{
	CSUDI_HANDLE hINJECTER, hPlayer= CSUDI_NULL;
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S sPCMStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;

	void *pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
	
	int nAudioID=-1;
	
	int i;
	
	nAudioID=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "步骤5失败");
    
    hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"打开PCM播放器失败");

	for (i=0;i<REPEAT_TIMES;i++)
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
//@INPUT:1、hINJECTER 为有效的PCM注入器句柄
//@EXPECTATION:返回非CSUDI_SUCCESS
//@REMARK:1) Start之前，2)Stop之后，3)PCM句柄为CSUDI_NULL，4) uWrittenSize比前期得到的空闲长度大，上列4种情况下，函数均应返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、获取支持PCM注入的音频解码器或音频通道
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen正常打开一个PCM注入器并得到句柄
//@EXECUTIONFLOW:3、调用CSUDIINJECTERWriteComplete，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置PCM大小端属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERSetAttribute设置PCM流参数属性，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart启动此PCM注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIINJECTERGetFreeBuffer得到空闲缓冲区地址和长度
//@EXECUTIONFLOW:8、hINJECTER为CSUDI_NULL，uWrittenSize为步骤7得到的空闲缓冲区长度，调用CSUDIINJECTERWriteComplete，期望返回CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:9、hINJECTER为步骤2得到的句柄，uWrittenSize为步骤7得到的空闲缓冲区长度加1，调用CSUDIINJECTERWriteComplete，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop，停止此PCM注入器
//@EXECUTIONFLOW:11、hINJECTER为步骤2得到的句柄，uWrittenSize为步骤7得到的空闲缓冲区长度，调用CSUDIINJECTERWriteComplete，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:12、重复步骤6~步骤11共1000次
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClose，关闭此PCM注入器
CSUDI_BOOL CSTC_PCM_INJECTER_IT_WriteComplete_0001(void)
{
	CSUDI_HANDLE hINJECTER, hPlayer= CSUDI_NULL;
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S sPCMStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;

	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;

	void *pvBuffer = CSUDI_NULL;
	unsigned int uLength = 0;
	
	int nAudioID=-1;
	
	int i;
	
	nAudioID=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "没有支持PCM类型的音频解码器");

//设置注入实例具体参数
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER,1), "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "步骤5失败");

    	hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"打开PCM播放器失败");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "步骤7失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERWriteComplete (CSUDI_NULL,uLength), "步骤87失败");

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
//@PRECONDITION:INJECTER模块被正常初始化，PCM Player能正常工作
//@INPUT:1、所有参数均正常
//@EXECUTIONFLOW:1、调用CSUDIDEMUXGetCount获取demux的数量
//@EXECUTIONFLOW:2、调用CSUDIDEMUXGetCapability依次寻找第一个支持EM_UDI_DEMUX_INJECT  的demux，用于节目播放的demux
//@EXECUTIONFLOW:3、调用CSUDIINJECTEROpen，psOpenParams,demux 设置如上所示，音视频解码器都为-1，phINJECTER非空
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart，使hINJECTER 等于上一步获得的句柄
//@EXECUTIONFLOW:5、调用CSUDIINJECTERGetFreeBuffer ，使ppvBuffer  非空，puLength 非空
//@EXECUTIONFLOW:6、停止播放器
//@EXECUTIONFLOW:7、修改ppvBuffer中的内容
//@EXECUTIONFLOW:8、调用CSUDIINJECTERWriteComplete ,写入数据，这时应该返回非CSUDI_SUCCESS，而不是死机
//@EXECUTIONFLOW:9、调用CSUDIINJECTERClose，关闭注入实例
CSUDI_BOOL CSTC_PCM_INJECTER_IT_WriteComplete_0002(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
		
//创建，设置播放器的参数
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

//获取写入缓冲区的参数
	void *pvBuffer= CSUDI_NULL;
	unsigned int uLength=0;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤3失败");
	
	CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam);
	CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤4失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength),"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(uLength > 0,"获取的缓冲区不大于0");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤6失败");			
	
	memset(pvBuffer,0xFE,uLength);

	CSUDIINJECTERWriteComplete(hINJECTER, uLength);

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤十一失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "停止注入器失败");		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤十二失败");	

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入大端字节序的PCM 数据，正常播放的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:10、提示用户确认，是否能听见PCM音乐播放出来
//@EXECUTIONFLOW:11、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0001(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0001:测试注入大端字节序的PCM 数据，正常播放的情况\n");
	CSTCPrint("Test pcm inject of big endian data\n");
	CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Is it playing correctly ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

	CSTK_FATAL_POINT;
    
    if(CSUDI_NULL != hInjectThread)
    {
        g_bEnterTask = CSUDI_FALSE;

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤11失败"); 
    
   		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");

    }

    if(CSUDI_NULL != hPlayer)
    {    
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤13失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败");
	}
    
    if(CSUDI_NULL != hINJECTER)
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤15失败");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");
	}
    
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入大端字节序的PCM 数据，进行播放、暂停和恢复暂停
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐，能控制PCM播放的正常播放、暂停与恢复播放
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据
//@EXECUTIONFLOW:10、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:11、调用CSUDIPLAYERPause ，暂停PCM播放
//@EXECUTIONFLOW:12、提示用户确认，音频是否停止
//@EXECUTIONFLOW:13、调用CSUDIPLAYERResume ，恢复PCM播放
//@EXECUTIONFLOW:14、重复步骤10-步骤13共3次
//@EXECUTIONFLOW:15、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:20、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0002(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0002:测试注入大端字节序的PCM 数据，进行播放、暂停和恢复暂停\n");
	CSTCPrint("Test pcm inject of big endian data with pause/resume control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");

	for (i=0;i<LOOP_TIMES;i++)
	{
		if (i == 0)
		{
			CSTCPrint("音频播放出来了吗？\n");
			CSTCPrint("Do we get audio output normally ?\n");
		}
		else
		{
			CSTCPrint("已恢复播放，音频是否从暂停点继续播放？\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤11失败");
		
		CSTCPrint("音频暂停播放了吗？\n");
		CSTCPrint("Does the audio stop playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤13失败");
		
	}
	
	CSTK_FATAL_POINT;
	
    if(CSUDI_NULL != hInjectThread)
    {
        g_bEnterTask = CSUDI_FALSE;

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤15失败"); 
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16失败");
	}
    
    if(CSUDI_NULL != hPlayer)
    {    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤17失败");    
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤18失败");
    }
    
    if(CSUDI_NULL != hINJECTER)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤19失败");    
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤20失败");
    }

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入大端字节序的PCM 数据，进行播放、停止操作
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐，能控制PCM播放的正常播放、暂停与恢复播放
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据
//@EXECUTIONFLOW:10、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止PCM播放
//@EXECUTIONFLOW:12、提示用户确认，音频是否停止
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStart ，恢复PCM播放
//@EXECUTIONFLOW:14、重复步骤10-步骤13共3次
//@EXECUTIONFLOW:15、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:20、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0003(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0003:测试注入大端字节序的PCM 数据，进行播放、停止操作\n");
	CSTCPrint("Test pcm inject of big endian data with stop/start control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");

	for (i=0;i<LOOP_TIMES;i++)
	{
		if (i == 0)
		{
			CSTCPrint("音频播放出来了吗？\n");
			CSTCPrint("Do we get audio output normally ?\n");
		}
		else
		{
			CSTCPrint("已重新启动播放，音频播放出来了吗？(不要求与停止前连续)\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");
		
		CSTCPrint("音频停止播放了吗？\n");
		CSTCPrint("Does the audio stop playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤13失败");
		
	}
	
	CSTK_FATAL_POINT;
    
    if(CSUDI_NULL != hInjectThread)
    {
	    g_bEnterTask = CSUDI_FALSE;

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤15失败"); 
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16失败");
    }
    
    if(CSUDI_NULL != hPlayer)
    {    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤17失败");    
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤18失败");
    }
    
    if(CSUDI_NULL != hINJECTER)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤19失败");    
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤20失败");
    }

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入大端字节序的PCM 数据，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:10、提示用户确认，是否能听见PCM音乐播放出来
//@EXECUTIONFLOW:11、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERFlush ，等待已注入的数据全部播放完，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0004(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0004:测试注入大端字节序的PCM 数据，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完\n");
	CSTCPrint("Test pcm inject of big endian data with CSUDIINJECTERFlush control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

    g_bEnterTask = CSUDI_FALSE;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤11失败"); 
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");

	hInjectThread = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"步骤13失败");	

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤14失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤16失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入大端字节序的PCM 数据，正常注入结束，调用CSUDIINJECTERClear清除已注入但未播放完的数据
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:10、提示用户确认，是否能听见PCM音乐播放出来
//@EXECUTIONFLOW:11、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClear ，清除已注入的数据，期望音频立即停止，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0005(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0005:测试注入大端字节序的PCM 数据，正常注入结束，调用CSUDIINJECTERClear清除已注入但未播放完的数据\n");
	CSTCPrint("Test pcm inject of big endian data with CSUDIINJECTERClear control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");
    
    g_bEnterTask = CSUDI_FALSE;
        
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤11失败"); 
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");

	hInjectThread = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"步骤13失败");	

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{
	    g_bEnterTask = CSUDI_FALSE;
	        
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤14失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤16失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入小端字节序的PCM 数据，正常播放的情况
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:10、提示用户确认，是否能听见PCM音乐播放出来
//@EXECUTIONFLOW:11、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0006(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_INJECTER_IT_INJECTER_0006:测试注入小端字节序的PCM 数据，正常播放的情况\n");
	CSTCPrint("Test pcm inject of little endian data \n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

	CSTK_FATAL_POINT;
    
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;
	    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤11失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤13失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤15失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入小端字节序的PCM 数据，进行播放、暂停和恢复暂停
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐，能控制PCM播放的正常播放、暂停与恢复播放
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据
//@EXECUTIONFLOW:10、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:11、调用CSUDIPLAYERPause ，暂停PCM播放
//@EXECUTIONFLOW:12、提示用户确认，音频是否停止
//@EXECUTIONFLOW:13、调用CSUDIPLAYERResume ，恢复PCM播放
//@EXECUTIONFLOW:14、重复步骤10-步骤13共3次
//@EXECUTIONFLOW:15、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:20、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0007(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0007:测试注入小端字节序的PCM 数据，进行播放、暂停和恢复暂停\n");
	CSTCPrint("Test pcm inject of little endian data with pause/resume control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");

	for (i=0;i<LOOP_TIMES;i++)
	{
		if (i == 0)
		{
			CSTCPrint("音频播放出来了吗？\n");
			CSTCPrint("Do we get audio output normally ?\n");
		}
		else
		{
			CSTCPrint("已恢复播放，音频是否从暂停点继续播放？\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤11失败");
		
		CSTCPrint("音频暂停播放了吗？\n");
		CSTCPrint("Does the audio pause playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤13失败");
		
	}
	
	CSTK_FATAL_POINT;
    
	if(CSUDI_NULL != hInjectThread)
	{
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤15失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤17失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤18失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤19失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤20失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入小端字节序的PCM 数据，进行播放、停止操作
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐，能控制PCM播放的正常播放、暂停与恢复播放
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据
//@EXECUTIONFLOW:10、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止PCM播放
//@EXECUTIONFLOW:12、提示用户确认，音频是否停止
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStart ，恢复PCM播放
//@EXECUTIONFLOW:14、重复步骤10-步骤13共3次
//@EXECUTIONFLOW:15、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:20、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0008(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0008:测试注入大端字节序的PCM 数据，进行播放、停止操作\n");
	CSTCPrint("Test pcm inject of little endian data with stop/start control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");

	for (i=0;i<LOOP_TIMES;i++)
	{
		if (i == 0)
		{
			CSTCPrint("音频播放出来了吗？\n");
			CSTCPrint("Do we get audio output normally ?\n");
		}
		else
		{
			CSTCPrint("已重新启动播放，音频播放出来了吗？(不要求与停止前连续)\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");
		
		CSTCPrint("音频停止播放了吗？\n");
		CSTCPrint("Does the audio stop playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤13失败");
		
	}
	
	CSTK_FATAL_POINT;
    
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤15失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤16失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤17失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤18失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤19失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤20失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入小端字节序的PCM 数据，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:10、提示用户确认，是否能听见PCM音乐播放出来
//@EXECUTIONFLOW:11、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERFlush ，等待已注入的数据全部播放完，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0009(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0009:测试注入大端字节序的PCM 数据，正常注入结束，调用CSUDIINJECTERFlush等待已注入数据播放完\n");
	CSTCPrint("Test pcm inject of little endian data with CSUDIINJECTERFlush control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Do we get audio output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

    g_bEnterTask = CSUDI_FALSE;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤11失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");

	hInjectThread = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"步骤13失败");	

	CSTK_FATAL_POINT;
    
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤14失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤16失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入小端字节序的PCM 数据，正常注入结束，调用CSUDIINJECTERClear清除已注入但未播放完的数据
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:10、提示用户确认，是否能听见PCM音乐播放出来
//@EXECUTIONFLOW:11、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIINJECTERClear ，清除已注入的数据，期望音频立即停止，并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0010(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0010:测试注入小端字节序的PCM 数据，正常注入结束，调用CSUDIINJECTERClear清除已注入但未播放完的数据\n");
	CSTCPrint("Test pcm inject of little endian data with CSUDIINJECTERClear control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");
	
	CSTCPrint("音频可以正常播放吗？\n");
	CSTCPrint("Do we get audio output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

    g_bEnterTask = CSUDI_FALSE;
        
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤11失败"); 
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");

	hInjectThread = CSUDI_NULL;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"步骤13失败");    
    
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
	    g_bEnterTask = CSUDI_FALSE;
	        
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤14失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤15失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤16失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤17失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤18失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤19失败");
	}
    
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试本身是大字节序的pcm文件设置成小端字节序的播放
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:10、提示用户确认音频是否为噪声，或者根本就没有声音
//@EXECUTIONFLOW:11、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0011(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0011:测试本身是大字节序的pcm文件设置成小端字节序的播放\n");
	CSTCPrint("Test playing a pcm stream with little endian which is original big endian \n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT011", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");
	
	CSTCPrint("请确认音频是否为噪声，或者根本就没有声音\n");
	CSTCPrint("Do we get noise sound output or nothing output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤11失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤13失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤15失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入的数据大于通过CSUDIINJECTERGetFreeBuffer得到的缓冲区大小
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  不能播放出指定PCM音频音乐
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据，期望能够正常播放音频
//@EXECUTIONFLOW:10、提示用户确认音频是否为噪声，或者根本就没有声音
//@EXECUTIONFLOW:11、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:12、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0012(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0012:测试注入的数据大于通过CSUDIINJECTERGetFreeBuffer得到的缓冲区大小\n");
	CSTCPrint("Test that the inject data len is larger than the size got from CSUDIINJECTERGetFreeBuffer \n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT012", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask_overLength, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");
	
	CSTCPrint("请确认音频不能播放出来\n");
	CSTCPrint("Make sure we don't get any audio output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤11失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤12失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤13失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤14失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤15失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入小端字节序的PCM 数据，进行播放、音量调节,静音和静音恢复操作
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐，能控制PCM播放的正常播放、音量调节和静音操作
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据
//@EXECUTIONFLOW:10、调用CSUDIAUDIOSetVolume 设置音量为最大31
//@EXECUTIONFLOW:11、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:12、调用CSUDIAUDIOSetVolume ，调节音量,每秒把音量减2,共用15秒
//@EXECUTIONFLOW:13、提示用户确认，音量是否逐渐减小
//@EXECUTIONFLOW:14、调用CSUDIAUDIOSetVolume ，调节音量,每次把音量增加2,共用15秒
//@EXECUTIONFLOW:15、提示用户确认，音量是否逐渐增加
//@EXECUTIONFLOW:16、调用CSUDIAUDIOMute设置静音
//@EXECUTIONFLOW:17、提示用户确认，音频是否静音
//@EXECUTIONFLOW:18、调用CSUDIAUDIOUnmute解除静音
//@EXECUTIONFLOW:19、提示用户确认，音频是否播放
//@EXECUTIONFLOW:20、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:21、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:22、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:23、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:24、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:25、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0013(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0013:测试注入小端字节序的PCM 数据，进行播放、音量调节,静音和静音恢复\n");
	CSTCPrint("Test pcm inject with little endian data,and make control of volume,mute\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT0013", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "步骤6失败");

	CSUDIOSThreadSleep(1000);
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(sPlayerChnl.m_nAudioDecoder, 31), "步骤10失败");

	CSTCPrint("音频播放出来了吗？\n");	
    CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");

	for(i = 0;i < 15;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(sPlayerChnl.m_nAudioDecoder,31 - 2 * i), "步骤12失败");
		CSTCPrint("当前音量为:%d\n", 31 - 2 * i);
		CSUDIOSThreadSleep(500);
	}
	CSTCPrint("音量是逐渐减小的吗?\n");
    CSTCPrint("Does the volume decrease all the way ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败");

	for(i = 0;i < 15;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(sPlayerChnl.m_nAudioDecoder, 2 * i), "步骤14失败");
		CSTCPrint("当前音量为:%d\n", 2 * i);
		CSUDIOSThreadSleep(500);
	}
	CSTCPrint("音量是逐渐增大的吗?\n");	
    CSTCPrint("Does the volume increase all the way ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤15失败");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOMute(sPlayerChnl.m_nAudioDecoder), "步骤16失败");
	CSTCPrint("静音了吗\n");
    CSTCPrint("Is it mute  ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤17失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOUnmute(sPlayerChnl.m_nAudioDecoder), "步骤18失败");
	CSTCPrint("声音恢复了吗?\n");
    CSTCPrint("Is it unmute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤19失败");
				
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤20失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤21失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤22失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤23失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤24失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤25失败");
	}

	return CSUDI_TRUE;

}

//@CASEGROUP:INJECTER 
//@DESCRIPTION:PCM注入在未设置时要返回默认的大小端，而非错误状态
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@EXPECTATION:  在未调用CSUDIINJECTERSetAttribute之前，调用CSUDIINJECTERGetAttribute时可以返回正常并获得大小端默认值
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERGetAttribute ，获取PCM的大小端值。期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0014(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0014:PCM注入在未设置时要返回默认的大小端，而非错误状态\n");
	
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(((EM_UDIINJECTER_LITTLE_ENDIAN == ePcmEndian )||(EM_UDIINJECTER_BIG_ENDIAN == ePcmEndian)) ,"步骤4失败");
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤5失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试注入PCM 数据，进行播放,停止注入，测试音频是否仍在播放
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  能播放出指定PCM音频音乐，停止注入后，音频应该不能继续播放
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据
//@EXECUTIONFLOW:10、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:11、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:12、提示用户确认，音频是否仍在播放
//@EXECUTIONFLOW:13、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:14、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop ，停止播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、在步骤11，如果用户确认有音频播放，则测试用例不通过，如果用户确认没有音频播放，则测试用例通过
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0015(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0015:测试注入PCM 数据，进行播放,停止注入，测试音频是否仍在播放\n");

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT015", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "步骤6失败");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");

	CSTCPrint("音频播放出来了吗？\n");
    CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤10失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤11失败");	
		
	CSTCPrint("音频停止播放了吗?\n");	
    CSTCPrint("Does the audio stop playing  ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "步骤12失败");
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤13失败"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤14失败");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤15失败");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤16失败");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤18失败");
	}

	return CSUDI_TRUE;
}

static void PcmSinWaveInjectTask(PCMPlayInfo_S *pPcmInfo)
{
	void * ppvBuffer = NULL;
	unsigned  int  uLength = 0;
	CSUDI_HANDLE hPcm = g_hPcmHandle;
	CSUDI_HANDLE hPcmSin ;
	int nInjectedLength = 0; //可注入的大小

	hPcmSin = CSPcmSin_Open(&pPcmInfo->m_StartParam, pPcmInfo->m_enmEndian, 500);
	
	while(g_bEnterTask)
	{
              uLength = 0;
		CSUDIINJECTERGetFreeBuffer (hPcm,&ppvBuffer,&uLength);
				
		if(uLength != 0)
		{
			nInjectedLength = CSPcmSin_Read(hPcmSin, ppvBuffer, uLength);
			if (nInjectedLength <= 0)
			{
				//error
				g_bEnterTask = FALSE;
				continue;
			}
			
			CSUDIINJECTERWriteComplete(hPcm,nInjectedLength);
		}
		CSUDIOSThreadSleep(10);
	}

	if(!g_bEnterTask)
	{
		CSPcmSin_Close(hPcmSin);	
	}
}

static CSUDI_BOOL DiffPcmTest(CSUDIINJECTERPcmEndian_E eEndian)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S sPCMPlayInfo;

	#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

	//测试组合
	int anTestChNum[] = {1, 2}; // channel 1 ~ 2
	int anTestSampleRate[] = {11000, 48000};
	int anTestBitWidth[] = {8, 16, 32};

	int i, j, k;
	
	//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	CSTCPrint("请注意:该用例测试的是正弦波，请先将音量调低一点!\n");
    
	CSTKWaitAnyKey();
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤4失败");

	for (i = 0; i < ARRAY_LENGTH(anTestChNum); i++)
	{
		for (j = 0; j < ARRAY_LENGTH(anTestSampleRate); j++)
		{
			for (k = 0; k < ARRAY_LENGTH(anTestBitWidth); k++)
			{
				sPcmStartParam.m_uChannels = anTestChNum[i];
				sPcmStartParam.m_uBitsSample = anTestBitWidth[k];
				sPcmStartParam.m_uSampleRate = anTestSampleRate[j];
				ePcmEndian = eEndian;

				sPCMPlayInfo.m_StartParam = sPcmStartParam;
				sPCMPlayInfo.m_enmEndian = ePcmEndian;
				sPCMPlayInfo.m_pcName[0] = '\0';
				
				CSTCPrint("正在注入播放通道数为%d,位宽%d,采样率为%d,%s的PCM数据\n",
							sPcmStartParam.m_uChannels, 
							sPcmStartParam.m_uBitsSample, 
							sPcmStartParam.m_uSampleRate,
							(ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"大端":"小端"));
                CSTCPrint("Playing status: channels is:%d,BitsSample is:%d,SampleRate is :%d Endian mode is: %s\n",
                            sPcmStartParam.m_uChannels, 
                            sPcmStartParam.m_uBitsSample, 
                            sPcmStartParam.m_uSampleRate,
                            (ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"BIG ENDIAN":"LITTLE ENDIAN"));

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤5失败");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤7失败");

				g_hPcmHandle = hINJECTER;
				
				g_bEnterTask = CSUDI_TRUE;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)PcmSinWaveInjectTask, ( void* ) &sPCMPlayInfo, &hInjectThread), "步骤8失败");
				
				g_hInjectThread = hInjectThread;
				
				
				CSTCPrint("音频可以正常播放吗？\n");
				CSTCPrint("Do we get audio output normally ?\n");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");
				
				g_bEnterTask = CSUDI_FALSE;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤10失败");	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"步骤10失败");	

                hInjectThread = CSUDI_NULL;
                
                CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤11失败");    
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤12失败");	
			}
		}
	}
	
	CSTK_FATAL_POINT;
	if (hPlayer)
	{
		CSUDIPLAYERClose(hPlayer);
	}
	
	if (hInjectThread)
	{
        	g_bEnterTask = CSUDI_FALSE;
        
        	CSUDIOSThreadJoin(hInjectThread);
			CSUDIOSThreadDestroy(hInjectThread);

        	hInjectThread = CSUDI_NULL;
	}

	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试不同特征的小端PCM数据的播放情况，PCM数据为软件实时生的正弦波
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器，且支持小端PCM数据
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  不同特征PCM都能听到正常的声音
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen ，创建一个PCM注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStream ，设为PCM Stream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、通过CSUDIINJECTERSetAttribute ，先为设第一种PCM 属性组合，并将PCM生成器设置成相应属性，PCM属性组合方法，通道数[mono,setero],采样率[11K,48K],位宽[8,16,32]
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart ，启动注入
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart ，启动播放
//@EXECUTIONFLOW:8、启动注入线程，并等待1秒
//@EXECUTIONFLOW:9、提示用户确认，是否听到持续的"嘀"的声音，期望听到连续正常的音频，否则测试失败
//@EXECUTIONFLOW:10、停止注入线程
//@EXECUTIONFLOW:11、调用CSUDIINJECTERStop ，停止注入
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放
//@EXECUTIONFLOW:13、重复step 4 - 12，测试其它属性的PCM数据，直到所有的组合测试完成
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0016(void)
{
	return DiffPcmTest(EM_UDIINJECTER_LITTLE_ENDIAN);
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试不同特征的大端PCM数据的播放情况，PCM数据为软件实时生的正弦波
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器，且支持大端PCM数据
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  不同特征PCM都能听到正常的声音
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen ，创建一个PCM注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStream ，设为PCM Stream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、通过CSUDIINJECTERSetAttribute ，先为设第一种PCM 属性组合，并将PCM生成器设置成相应属性，PCM属性组合方法，通道数[mono,setero],采样率[11K,48K],位宽[8,16,32]
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart ，启动注入
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart ，启动播放
//@EXECUTIONFLOW:8、启动注入线程，并等待1秒
//@EXECUTIONFLOW:9、提示用户确认，是否听到持续的"嘀"的声音，期望听到连续正常的音频，否则测试失败
//@EXECUTIONFLOW:10、停止注入线程
//@EXECUTIONFLOW:11、调用CSUDIINJECTERStop ，停止注入
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStop ，停止播放
//@EXECUTIONFLOW:13、重复step 4 - 12，测试其它属性的PCM数据，直到所有的组合测试完成
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0017(void)
{
	return DiffPcmTest(EM_UDIINJECTER_BIG_ENDIAN);
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试暂停与恢复功能
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  必须支持暂停与恢复，且恢复时声音播放是连续的没有跳跃
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen ，创建一个PCM注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStream ，设为PCM Stream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、通过CSUDIINJECTERSetAttribute ，指为指定PCM流的PCM属性，PCM流不限属性，只要该平台支持
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart ，启动注入
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart ，启动播放
//@EXECUTIONFLOW:8、启动注入线程，
//@EXECUTIONFLOW:9、等待1秒
//@EXECUTIONFLOW:10、调用CSUDIPLAYERPause，暂停播放
//@EXECUTIONFLOW:11、提示用户确认，声音播放是否暂停，期望无声，否则测试失败
//@EXECUTIONFLOW:12、调用CSUDIPLAYERResume，恢复播放
//@EXECUTIONFLOW:13、提示用户确认，声音播放是否恢复并在暂停点恢复，没有跳跃，否则测试失败
//@EXECUTIONFLOW:14、重复9 - 13 一次
//@EXECUTIONFLOW:15、调用CSUDIINJECTERStop ，停止注入
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStop ，停止播放
//@EXECUTIONFLOW:17、停止注入线程
//@EXECUTIONFLOW:18、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0018(void)
{

	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S* psPCMPlayInfo = NULL;

	int nTestCount = 2;

	//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤3失败");

	/* get best endian */
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
	
	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
	}
	else
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
	}

	sPcmStartParam = psPCMPlayInfo->m_StartParam;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤5失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤7失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "步骤8失败");
	
	g_hInjectThread = hInjectThread;

	CSUDIOSThreadSleep(1000);
	
	CSTCPrint("音频是否正常播放？\n");
    CSTCPrint("Do we get audio output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");
	
	while (nTestCount--)
	{

		//pause
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤10失败");
		
		CSTCPrint("音频是否暂停播放？\n");
        CSTCPrint("Is it stop playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");

		//resume
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤12失败");
		
		CSTCPrint("音频是否恢复播放,且在暂停点恢复，没有跳跃?\n");
        CSTCPrint("Does it resume playing at the previous point ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败");
	}

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤15失败");    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤16失败");	

	CSTK_FATAL_POINT;
	if (hPlayer)
	{
		CSUDIPLAYERClose(hPlayer);
	}
	
	if (hInjectThread)
	{
		g_bEnterTask = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤17失败");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"步骤18失败");	

        hInjectThread = CSUDI_NULL;
	}

	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

void InjectToFull(CSUDI_HANDLE hPcmSin, CSUDI_HANDLE hINJECTER)
{		
	unsigned  int uLength;
	int nInjectedLength;
	void* pvBuffer;

	for (;;)
	{
		uLength = 0;
		nInjectedLength = 0;
		pvBuffer = NULL;
		
		CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength);
				
		if(uLength >= 32)
		{
			nInjectedLength = CSPcmSin_Read(hPcmSin, pvBuffer, uLength);
			
			CSUDIINJECTERWriteComplete(hINJECTER,nInjectedLength);
		}
		else
	    {
			/* underlying buffer is nearly full */
			return;
	    }
	}	
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试PCM停止注入后，不能还一直有声音
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@EXPECTATION:  PCM停止注入后，不能还一直有声,音该测试使用不同采用率的PCM文件测试（8bit、16bit、32bit都要测试）
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen ，创建一个PCM注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStream ，设为PCM Stream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、通过CSUDIINJECTERSetAttribute ，先为设第一种PCM 属性组合,PCM属性组合方法，采样率[8,16,32]，其它不限
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart ，启动注入
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart ，启动播放
//@EXECUTIONFLOW:8、不断使用CSUDIINJECTERGetFreeBuffer/CSUDIINJECTERWriteComplete写入数据，直到free buffer为0
//@EXECUTIONFLOW:9、调用CSUDIINJECTERStop ，停止注入
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop ，停止播放
//@EXECUTIONFLOW:11、提示用户确认，要求没有音频再播放，否则测试失败
//@EXECUTIONFLOW:12、重复step 5 - 12，测试其它属性的PCM数据，直到所有的组合测试完成
//@EXECUTIONFLOW:14、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0019(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hPcmSin = CSUDI_NULL;
	
	//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S sPCMPlayInfo;

	#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

	//测试组合
	int anTestBitWidth[] = {8, 16, 32};

	int i;
	
	//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤4失败");

	for (i = 0; i < ARRAY_LENGTH(anTestBitWidth); i++)
	{
		sPcmStartParam.m_uChannels = 1;
		sPcmStartParam.m_uBitsSample = anTestBitWidth[i];
		sPcmStartParam.m_uSampleRate = 48000;
		
		/* get best endian */
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");
	
		sPCMPlayInfo.m_StartParam = sPcmStartParam;
		sPCMPlayInfo.m_enmEndian = ePcmEndian;
		sPCMPlayInfo.m_pcName[0] = '\0';

		hPcmSin = CSPcmSin_Open(&sPcmStartParam, ePcmEndian, 500);
		
		CSTCPrint("正在注入播放通道数为%d,位宽%d,采样率为%d,%s的PCM数据\n",
					sPcmStartParam.m_uChannels, 
					sPcmStartParam.m_uBitsSample, 
					sPcmStartParam.m_uSampleRate,
					(ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"大端":"小端"));
        CSTCPrint("Playing status: channels is:%d,BitsSample is:%d,SampleRate is :%d Endian mode is: %s\n",
                    sPcmStartParam.m_uChannels, 
                    sPcmStartParam.m_uBitsSample, 
                    sPcmStartParam.m_uSampleRate,
                    (ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"BIG ENDIAN":"LITTLE ENDIAN"));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤3失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤7失败");

		InjectToFull(hPcmSin, hINJECTER);

		CSTCPrint("已注满Buffer，现在停止注入\n");
		CSTCPrint("The inject Buffer is full,we stop injecting !\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤9失败");	
		
		CSTCPrint("音频是否停止播放?\n");
		CSTCPrint("Does it stop playing and we don't get audio output ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤10失败");	

		CSPcmSin_Close(hPcmSin);
		hPcmSin = CSUDI_NULL;
	}
	
	CSTK_FATAL_POINT;
	if (hPlayer)
	{
		CSUDIPLAYERClose(hPlayer);
	}
	
	if (hPcmSin)
	{
		CSPcmSin_Close(hPcmSin);
	}

	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试flush功能
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@EXPECTATION:  PCM停止注入后，调用CSUDIINJECTERFlush，直到声音播完才结束
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen ，创建一个PCM注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStream ，设为PCM Stream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、通过CSUDIINJECTERSetAttribute ，先为设第一种PCM 属性组合,PCM属性组合方法，采样率[8,16,32]，其它不限
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart ，启动注入
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart ，启动播放
//@EXECUTIONFLOW:8、不断使用CSUDIINJECTERGetFreeBuffer/CSUDIINJECTERWriteComplete写入数据，直到free buffer为0
//@EXECUTIONFLOW:9、调用CSUDIINJECTERFlush ，等待注入的数据用完
//@EXECUTIONFLOW:10、调用CSUDIINJECTERStop ，停止注入
//@EXECUTIONFLOW:11、调用CSUDIPLAYERStop ，停止播放
//@EXECUTIONFLOW:12、提示用户确认，声音声一否播放一段时间才结束，否则测试失败
//@EXECUTIONFLOW:13、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:15、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0020(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hPcmSin = CSUDI_NULL;
	
	//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S sPCMPlayInfo;

	#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

	//测试组合
	int anTestBitWidth[] = {8, 16, 32};

	int i;
	
	//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤4失败");

	for (i = 0; i < ARRAY_LENGTH(anTestBitWidth); i++)
	{
		sPcmStartParam.m_uChannels = 1;
		sPcmStartParam.m_uBitsSample = anTestBitWidth[i];
		sPcmStartParam.m_uSampleRate = 48000;
		
		/* get best endian */
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");
	
		sPCMPlayInfo.m_StartParam = sPcmStartParam;
		sPCMPlayInfo.m_enmEndian = ePcmEndian;
		sPCMPlayInfo.m_pcName[0] = '\0';

		hPcmSin = CSPcmSin_Open(&sPcmStartParam, ePcmEndian, 500);
		
		CSTCPrint("正在注入播放通道数为%d,位宽%d,采样率为%d,%s的PCM数据\n",
					sPcmStartParam.m_uChannels, 
					sPcmStartParam.m_uBitsSample, 
					sPcmStartParam.m_uSampleRate,
					(ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"大端":"小端"));
        CSTCPrint("Playing status: channels is:%d,BitsSample is:%d,SampleRate is :%d Endian mode is: %s\n",
                    sPcmStartParam.m_uChannels, 
                    sPcmStartParam.m_uBitsSample, 
                    sPcmStartParam.m_uSampleRate,
                    (ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"BIG ENDIAN":"LITTLE ENDIAN"));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤5失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤7失败");

		InjectToFull(hPcmSin, hINJECTER);// 8

		CSTCPrint("已注满Buffer，现在等待数据消耗完\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER), "步骤9失败");	
		
		CSTCPrint("现在停止播放\n");
        	CSTCPrint("Now stop the injecter and player !\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤10失败");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤11失败");	

		CSTCPrint("是否播放一小段音频才结束?\n");
        	CSTCPrint("Does it have a short audio output before it stops ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败");

		CSPcmSin_Close(hPcmSin);
		hPcmSin = CSUDI_NULL;
	}
	
	CSTK_FATAL_POINT;
	if (hPlayer)
	{
		CSUDIPLAYERClose(hPlayer);
	}
	
	if (hPcmSin)
	{
		CSPcmSin_Close(hPcmSin);
	}

	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER & AUDIO
//@DESCRIPTION:PCM注入时的音量设置、左右声道设置、静音功能测试
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  PCM注入播放时，音量、左右声道、静音等能够正常设置
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen ，创建一个PCM注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStream ，设为PCM Stream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、通过CSUDIINJECTERSetAttribute ，任选立体声PCM数据
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart ，启动注入
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart ，启动播放
//@EXECUTIONFLOW:8、启动注入线程，循环注入播放
//@EXECUTIONFLOW:9、确认是否正常播放
//@EXECUTIONFLOW:10、设置音量，由31到0，每减1等待200ms，到0时等待2秒，再由0到31，每加1等待200ms
//@EXECUTIONFLOW:11、提示用户确认，声音是否由大到无声，再由无声变为很大
//@EXECUTIONFLOW:12、设置为左声道
//@EXECUTIONFLOW:13、提示用户确认，当前是否为左声道
//@EXECUTIONFLOW:14、设置为右声道
//@EXECUTIONFLOW:15、提示用户确认，当前是否为右声道
//@EXECUTIONFLOW:16、设置为立体声
//@EXECUTIONFLOW:17、提示用户确认，当前是否为立体声
//@EXECUTIONFLOW:18、静音
//@EXECUTIONFLOW:19、提示用户确认，当前是否无声音
//@EXECUTIONFLOW:20、取消静音
//@EXECUTIONFLOW:21、提示用户确认，当前是声音是否恢复
//@EXECUTIONFLOW:22、调用CSUDIINJECTERStop ，停止注入
//@EXECUTIONFLOW:23、调用CSUDIPLAYERStop ，停止播放
//@EXECUTIONFLOW:24、停止注入线程
//@EXECUTIONFLOW:25、删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0021(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S* psPCMPlayInfo = NULL;

	int nAudioId = 0;

	int nVol = 0;

	//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	nAudioId = sPlayerChnl.m_nAudioDecoder;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤4失败");

	/* get best endian */
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");
	
	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
	}
	else
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
	}

	sPcmStartParam = psPCMPlayInfo->m_StartParam;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤5失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤7失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "步骤8失败");
	
	g_hInjectThread = hInjectThread;

	CSUDIOSThreadSleep(100);
	
	CSTCPrint("音频是否正常播放？\n");
    CSTCPrint("Is the audio playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

	//volume 31-0 0-31
	for (nVol = 31; nVol >= 0; nVol--)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, nVol), "步骤10失败");
		CSTCPrint("当前音量为:%d\n", nVol);
		CSUDIOSThreadSleep(200);
	}
	
	CSUDIOSThreadSleep(2000);
    
	for (nVol = 1; nVol <= 31; nVol++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, nVol), "步骤10失败");
		CSTCPrint("当前音量为:%d\n", nVol);
		CSUDIOSThreadSleep(200);
	}
	
	CSTCPrint("声音是否由大到无声，再由无声变为很大?\n");
    CSTCPrint("Does the volume decrease from very loud to null and  from null to very loud?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");

	//set left channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_LEFT), "步骤12失败");	
	CSTCPrint("是否为左声道?\n");
    CSTCPrint("Is the audio left channel ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败");

	//set right channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_RIGHT), "步骤14失败");	
	CSTCPrint("是否为右声道?\n");
    CSTCPrint("Is the audio rigth channel ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤15失败");

	//set stereo channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_STEREO), "步骤16失败");	
	CSTCPrint("是否为立体声?\n");
    CSTCPrint("Is the audio stereo channel ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤17失败");

	//mute
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "步骤18失败");	
	CSTCPrint("是否静音?\n");
    CSTCPrint("Is the audio mute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤19失败");

	//unmute
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioId), "步骤20失败");	
	CSTCPrint("静音是否已经取消?\n");
    CSTCPrint("Is the audio umute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤21失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤22失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤23失败");	

CSTK_FATAL_POINT;
	if (hPlayer)
	{
		CSUDIPLAYERClose(hPlayer);
	}
	
	if (hInjectThread)
	{
		g_bEnterTask = CSUDI_FALSE;
        
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤24失败");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"步骤25失败");
        
		hInjectThread = CSUDI_NULL;
	}

	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER & AUDIO
//@DESCRIPTION:PCM注入静音情况下，停止注入再重新播放，测试静音功能是否正常
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  PCM注入播放时，静音情况下重复注入，静音与非静音功能正常
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen ，创建一个PCM注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStream ，设为PCM Stream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、通过CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart ，启动注入
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart ，启动播放
//@EXECUTIONFLOW:8、启动注入线程，循环注入播放
//@EXECUTIONFLOW:9、确认是否正常播放
//@EXECUTIONFLOW:10、调用CSUDIAUDIOMute静音
//@EXECUTIONFLOW:11、提示用户确认，当前是否无声音
//@EXECUTIONFLOW:12、暂停注入线程
//@EXECUTIONFLOW:13、调用CSUDIINJECTERStop ，停止注入
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop ，停止播放
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:16、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:18、调用CSUDIPLAYEROpen ，创建一个PCM注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:19、调用CSUDIPLAYERStream ，设为PCM Stream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:20、通过CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:21、调用CSUDIINJECTERStart ，启动注入
//@EXECUTIONFLOW:22、调用CSUDIPLAYERStart ，启动播放
//@EXECUTIONFLOW:23、启动注入线程，循环注入播放
//@EXECUTIONFLOW:24、确认是否正常播放，但无声音输出
//@EXECUTIONFLOW:25、取消静音，期望有声音输出
//@EXECUTIONFLOW:26、停止注入线程
//@EXECUTIONFLOW:27、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:28、调用CSUDIPLAYERStop ，停止播放
//@EXECUTIONFLOW:29、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:30、调用CSUDIINJECTERStop ，停止注入
//@EXECUTIONFLOW:31、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:32、重复步骤1到31共3次，期望所有都满足期望值
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0022(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E        ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S* psPCMPlayInfo = NULL;

	int nAudioId = 0;
    int i = 0;
	//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	nAudioId = sPlayerChnl.m_nAudioDecoder;
	for(i=0;i<LOOP_TIMES;i++)
	{
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤4失败");

    	/* get best endian */
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");
    	
    	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
    	{
    		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
    	}
    	else
    	{
    		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
    	}

    	sPcmStartParam = psPCMPlayInfo->m_StartParam;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤5失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤7失败");

    	g_hPcmHandle = hINJECTER;
    	
    	g_bEnterTask = CSUDI_TRUE;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "步骤8失败");
    	
    	g_hInjectThread = hInjectThread;

    	CSUDIOSThreadSleep(100);
    	
    	CSTCPrint("音频是否正常播放？\n");
        CSTCPrint("Is the audio playing normally ?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

    	//mute
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "步骤10失败");	
    	CSTCPrint("是否静音?\n");
        CSTCPrint("Is the audio mute ?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤11失败");
        
    	g_bEnterTask = CSUDI_FALSE;
        //停止播放器
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤12失败");	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"步骤12失败");	

       	hInjectThread = CSUDI_NULL;
        
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤13失败");	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤14失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose(hPlayer), "步骤15失败");	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤16失败");

        ///切换歌曲重新播放
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤17失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤18失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤19失败");

    	/* get best endian */
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤20失败");
    	
    	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
    	{
    		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
    	}
    	else
    	{
    		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
    	}

    	sPcmStartParam = psPCMPlayInfo->m_StartParam;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤5失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤21失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤22失败");

    	g_hPcmHandle = hINJECTER;
    	
    	g_bEnterTask = CSUDI_TRUE;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "步骤23失败");
    	
    	g_hInjectThread = hInjectThread;

    	CSUDIOSThreadSleep(100);
    	
    	CSTCPrint("已重新启动PCM播放，但是期望所有设备无声音输出?\n");
        CSTCPrint("Does the audio has no output?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤24失败");

    	//unmute
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioId), "步骤25失败");	
    	CSTCPrint("静音是否已经取消?\n");
        CSTCPrint("Is the audio umute ?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"取消静音失败");

        g_bEnterTask = CSUDI_FALSE;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤26失败"); 
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"步骤27失败");  

        hInjectThread = CSUDI_NULL;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "步骤28失败");	
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤29失败");   
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤30失败");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤31失败");

	}
    
    CSTK_FATAL_POINT;
	
	if (hInjectThread)
	{
	    	g_bEnterTask = CSUDI_FALSE;
        
	    	CSUDIOSThreadJoin(hInjectThread); 
	    	CSUDIOSThreadDestroy(hInjectThread);  

        	hInjectThread = CSUDI_NULL;
	}
    
    if (hPlayer)
    {
        CSUDIPLAYERStop(hPlayer);
        CSUDIPLAYERClose(hPlayer);
    }

	if (hINJECTER)
	{
		CSUDIINJECTERStop(hINJECTER);
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}
//@CASEGROUP:INJECTER & PLAYER & AUDIO
//@DESCRIPTION:PCM注入时在非播放状态下音量设置、静音功能测试
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  PCM注入停止或者暂停播放时，音量、静音等能够正常设置，并且重新播放后有效果
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIPLAYEROpen ，创建一个PCM注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStream ，设为PCM Stream，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、通过CSUDIINJECTERSetAttribute ，任选立体声PCM数据
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStart ，启动注入
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart ，启动播放
//@EXECUTIONFLOW:8、启动注入线程，循环注入播放
//@EXECUTIONFLOW:9、确认是否正常播放
//@EXECUTIONFLOW:10、设置音量为0，提示用户确认没有声音输出
//@EXECUTIONFLOW:11、暂停播放，然后设置音量为31
//@EXECUTIONFLOW:12、恢复播放，提示用户确认此时有声音输出并且音量应该是31
//@EXECUTIONFLOW:13、设置音量为0，提示用户确认没有声音输出
//@EXECUTIONFLOW:14、停止播放，然后设置音量为31
//@EXECUTIONFLOW:15、重新播放，提示用户确认此时有声音输出并且音量应该是31
//@EXECUTIONFLOW:16、设置静音，提示用户确认没有声音输出
//@EXECUTIONFLOW:17、暂停播放，然后恢复静音
//@EXECUTIONFLOW:18、恢复播放，提示用户确认此时有声音输出
//@EXECUTIONFLOW:19、设置静音，提示用户确认没有声音输出
//@EXECUTIONFLOW:20、停止播放，然后恢复静音
//@EXECUTIONFLOW:21、重新播放，提示用户确认此时有声音输出
//@EXECUTIONFLOW:22、停止播放，然后进行静音操作期望返回成功
//@EXECUTIONFLOW:23、设置音量从0到31，期望成功
//@EXECUTIONFLOW:24、恢复静音操作，期望成功，再设置左右声道，期望设置成功
//@EXECUTIONFLOW:25、重新播放，提示用户确认此时有声音输出并且此事音量应该是31
//@EXECUTIONFLOW:26、调用CSUDIINJECTERStop ，停止注入
//@EXECUTIONFLOW:27、调用CSUDIPLAYERStop ，停止播放
//@EXECUTIONFLOW:28、停止注入线程
//@EXECUTIONFLOW:29、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0023(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S* psPCMPlayInfo = NULL;

	int nAudioId = 0;

	int nVol = 0;

	//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	nAudioId = sPlayerChnl.m_nAudioDecoder;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤4失败");

	/* get best endian */
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");
	
	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
	}
	else
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
	}

	sPcmStartParam = psPCMPlayInfo->m_StartParam;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"步骤5失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤7失败");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "步骤8失败");
	
	g_hInjectThread = hInjectThread;

	CSUDIOSThreadSleep(100);
	
	CSTCPrint("音频是否正常播放？\n");
    CSTCPrint("Is the audio playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤9失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, 0), "步骤10失败");
	CSTCPrint("是否所有设备无声音输出 ?\n");
    CSTCPrint("Does all the device have no audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤11失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, 31), "步骤11失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤12失败");
	CSTCPrint("是否所有设备有声音输出 ,且此时音量应该为最大值?\n");
    CSTCPrint("Does the all the device have audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤12失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, 0), "步骤13失败");
	CSTCPrint("是否所有设备无声音输出 ?\n");
    CSTCPrint("Does all the device have no audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤13失败");


    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤14失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, 31), "步骤14失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤15失败");
	CSTCPrint("是否所有设备有声音输出 ,且此时音量应该为最大值?\n");
    CSTCPrint("Does the all the device have audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤15失败");

	//mute
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "步骤16失败");	
	CSTCPrint("是否静音?\n");
    CSTCPrint("Is the audio mute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤16失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤17失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioId), "步骤17失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤18失败");
	CSTCPrint("静音是否已经取消?\n");
    CSTCPrint("Is the audio umute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤18失败");

	//mute
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "步骤19失败");	
	CSTCPrint("是否静音?\n");
    CSTCPrint("Is the audio mute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤19失败");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤20失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOUnmute(nAudioId), "步骤20失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤21失败");
	CSTCPrint("静音是否已经取消?\n");
    CSTCPrint("Is the audio umute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤21失败");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤22失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "步骤22失败");	

	for (nVol = 0; nVol <= 31; nVol++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, nVol), "步骤23失败");
		CSUDIOSThreadSleep(200);
	}

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOUnmute(nAudioId), "步骤24失败");
    
	//set left channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_LEFT), "步骤24失败");	

	//set right channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_RIGHT), "步骤24失败");	

	//set stereo channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_STEREO), "步骤24失败");	


	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "步骤25失败");	
	CSTCPrint("音频是否正常播放？\n");
    CSTCPrint("Is the audio playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤25失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤26失败");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "步骤27失败");	

    CSTK_FATAL_POINT;
	if (hPlayer)
	{
		CSUDIPLAYERClose(hPlayer);
	}
	
	if (hInjectThread)
	{
		g_bEnterTask = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"步骤28失败");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"步骤29失败");	

        hInjectThread = CSUDI_NULL;
	}

	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:测试PCM数据注入完成到播放完成这一段时间内的pause、resume、GetAttribute、flush操作
//@PRECONDITION:1、INJECTER模块被正常初始化
//@PRECONDITION:2、系统存在PCM播放器
//@PRECONDITION:3、将音乐文件放入U盘根目录下,插入U盘
//@INPUT:	1、创建注入器
//@INPUT:	2、创建播放器
//@INPUT:	3、创建注入线程
//@EXPECTATION:  
//@EXECUTIONFLOW:1、查找支持pcm的音频解码器
//@EXECUTIONFLOW:2、调用CSUDIINJECTEROpen，创建一个注入PCM  码流的注入实例，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute ，对PCM  的启动参数进行设置
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute ，对PCM的大小端进行设置
//@EXECUTIONFLOW:5、调用CSUDIINJECTERStart，开始数据注入，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、创建一个线程,循环读取PCM   流文件，并进行PCM注入操作
//@EXECUTIONFLOW:7、调用CSUDIPLAYEROpen ，创建一个注入的播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIPLAYERSetStream ，设置好pcm的信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStart ，开始播放数据
//@EXECUTIONFLOW:10、提示用户确认，音频是否播放出来
//@EXECUTIONFLOW:11、调用CSUDIOSThreadSleep等待5秒
//@EXECUTIONFLOW:12、通知注入任务停止注入数据，等待注入任务正常结束返回
//@EXECUTIONFLOW:13、调用CSUDIINJECTERGetAttribute获取Injecter已经注入到目标缓冲区或解码器缓冲区中但尚未被消耗掉的数据长度(单位字节)nAvailableDataSize，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:14、若未消耗的数据长度nAvailableDataSize大于0，执行下一步；否则，跳转至19步
//@EXECUTIONFLOW:15、若调用CSUDIPLAYERPause进行暂停的次数未超过1次，则执行下一步，否则跳转至13步
//@EXECUTIONFLOW:16、调用CSUDIPLAYERPause，暂停PCM播放，
//@EXECUTIONFLOW:17、提示用户确认，音频是否停止
//@EXECUTIONFLOW:18、调用CSUDIPLAYERResume ，恢复PCM播放,并跳转至13步
//@EXECUTIONFLOW:19、调用CSUDIINJECTERFlush()等待播音结束,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:20、提示用户确认，音频是否立即结束
//@EXECUTIONFLOW:21、调用CSUDIPLAYERClose ，关闭播放器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:22、调用CSUDIINJECTERStop ，停止注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:23、调用CSUDIINJECTERClose ，关闭注入器，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:24、调用CSUDIOSThreadDestroy ，删除注入任务，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0024(void)
{
	int i = 0;
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E	  ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

	//注入实例的参数	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;

	//创建，设置播放器的参数	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	//创建播放器参数
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	static PCMPlayInfo_S s_TTSPlayInfo =
	{
		{"tts.wav"},
		{16, 16000, 1},
		EM_UDIINJECTER_LITTLE_ENDIAN
	};

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0024: 测试PCM数据注入完成到播放完成这一段时间内的pause、resume、GetAttribute、flush操作\n");
	
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &s_TTSPlayInfo.m_StartParam),"步骤3失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"步骤4失败");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "步骤5失败");

	g_hPcmHandle = hINJECTER;
	g_bEnterTask = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT024", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* )&s_TTSPlayInfo, &hInjectThread), "步骤6失败");

	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "步骤7失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "步骤8失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "步骤9失败");

	CSTCPrint("音频播放出来了吗？\n");
	CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤10失败");

	// 等待注入完当前文件的pcm数据后，退出注入线程
	g_bInjectOnlyOnce = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "步骤12失败"); 

	i = 0;
	while (1)
	{
		CSUDIINJECTERStatus_S sInjectStatus;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER, EM_UDIINJECTER_STATUS, (void *)&sInjectStatus), "步骤13失败");

		if (sInjectStatus.m_nAvailableDataSize > 0)
		{
			if (i < 1)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "步骤16失败");

				CSTCPrint("音频暂停播放了吗？\n");
				CSTCPrint("Does the audio pause playing?\n");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤17失败");

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "步骤18失败");

				i++;
			}
		}
		else
		{
			unsigned int uTime = CSUDIPlusOSGetTickCount();

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER), "步骤19失败");

			uTime = CSUDIPlusOSGetTickCount() - uTime;

			// m_nAvailableDataSize返回0后，再调用flush接口时，flush时间不得大于2400ms，该值为经验值
			CSTK_ASSERT_TRUE_FATAL(uTime < 2400, "步骤20失败");
			
			CSTCPrint("音频是否立即结束播放，且播放的最后几个字为\"我不禁头涔涔而泪潸潸了\"？\n");
			CSTCPrint("Does the audio finished Immediately?\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤21失败");
			break;
		}

		CSUDIOSThreadSleep(10);
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "步骤22失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "步骤23失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "步骤24失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "步骤25失败");

	return CSUDI_TRUE;
}

