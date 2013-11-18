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
#include "../../cs_udi2testcase.h"

#define MAX_TEST_TIME	(10)
#define ERRORDATA (2010)
#define INJECTTHREADBUF (1024*148)
#define INJECTTHREADPRO (3*17)
#define MAX_CALLBACK_COUNT (32)
#define ITTEST_SMALLWINDOW_RECT        {260,188,200,200}

/* default is PES */
enum
{
	IFRAME_DATA_576I,
	IFRAME_DATA_480I,
	IFRAME_DATA_720P,
	IFRAME_DATA_1080I
};

#define 	IFRAME_DATA_DEFAULT IFRAME_DATA_576I

static int lookup_video_device(CSUDIVIDStreamType_E eDecoderType);
static void ifarme_player_callback(CSUDI_HANDLE hPlayer,CSUDIPlayerEventType_E eEvent,void * pvUserData);
static void get_iframe_data(CSUDIINJECTERIFrameParam_S* pstIframeData,int index);

static CSUDI_HANDLE g_hIframeTestMutex = CSUDI_NULL;
static CSUDI_HANDLE g_hNewFrameSem=CSUDI_NULL;

CSUDI_BOOL CSTC_IFRAME_INJECTER_Init(void)
{
	//在本测试用例集执行前调用
	 CSUDIOSMutexCreate("default", CSUDIOS_MUTEX_DEFAULT, &g_hIframeTestMutex);
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_IFRAME_INJECTER_UnInit(void)
{
	//在本测试用例集执行后调用
	CSUDIOSMutexDestroy(g_hIframeTestMutex);
	g_hIframeTestMutex = CSUDI_NULL;
	
	return CSUDI_TRUE;
}

static void get_iframe_data(CSUDIINJECTERIFrameParam_S* pstIframeData,int index)
{
	extern unsigned char iframe_480i_data[];
	extern int iframe_480i_data_size;

	extern unsigned char iframe_720p_data[];
	extern int iframe_720p_data_size;

	extern unsigned char iframe_1080i_data[];
	extern int iframe_1080i_data_size;
	
	extern unsigned char iframe_576i_data[];
	extern int iframe_576i_data_size;

	switch (index)
	{
	case IFRAME_DATA_576I:
		pstIframeData->m_pcIFrameData = (char*)iframe_576i_data;
		pstIframeData->m_nDataLen = iframe_576i_data_size;
		break;
	case IFRAME_DATA_480I:
		pstIframeData->m_pcIFrameData = (char*)iframe_480i_data;
		pstIframeData->m_nDataLen = iframe_480i_data_size;
		break;
	case IFRAME_DATA_720P:
		pstIframeData->m_pcIFrameData = (char*)iframe_720p_data;
		pstIframeData->m_nDataLen = iframe_720p_data_size;
		break;
	case IFRAME_DATA_1080I:
		pstIframeData->m_pcIFrameData = (char*)iframe_1080i_data;
		pstIframeData->m_nDataLen = iframe_1080i_data_size;
		break;
	default:
		pstIframeData->m_pcIFrameData = (char*)iframe_576i_data;
		pstIframeData->m_nDataLen = iframe_576i_data_size;
		break;

	}
}

static void ifarme_player_callback(CSUDI_HANDLE hPlayer,CSUDIPlayerEventType_E eEvent,void * pvUserData)
{
	CSUDI_Error_Code ErrCode = CSUDI_SUCCESS;
	//CSUDI_HANDLE hNewFrameSem = (CSUDI_HANDLE)pvUserData;

	if (eEvent == EM_UDIPLAYER_VIDEO_FRAME_COMING)
	{
		CSUDIOSMutexWait(g_hIframeTestMutex, 100);
		if (g_hNewFrameSem != CSUDI_NULL)
		{
			ErrCode = CSUDIOSSemRelease(g_hNewFrameSem);
		}
		CSUDIOSMutexRelease(g_hIframeTestMutex);
	}
}

static int lookup_video_device(CSUDIVIDStreamType_E eDecoderType)
{
	CSUDI_Error_Code ErrCode = CSUDI_SUCCESS;
	int nVideoCount = 0;
	int i = 0, j = 0;
	CSUDIVIDEOCapability_S stCapability;
	ErrCode = CSUDIVIDEOGetCount(&nVideoCount);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"");
	
	for (i = 0; i < nVideoCount; i++)
	{
		ErrCode = CSUDIVIDEOGetCapability(i, &stCapability);
		CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"");

		if (ErrCode == CSUDI_SUCCESS)
		{
			for (j = 0; i < EM_UDI_VID_STREAMTYPE_NUM; j++)
			{
				if (stCapability.m_eStreamType[j] == eDecoderType)
				{
					return i;
				}
			}
		}
	}
	CSTK_FATAL_POINT
	return CSUDI_INVALID_INDEX;
}

static CSUDI_BOOL SetupIFrameInjecter(CSUDI_HANDLE *hPlayer,CSUDI_HANDLE *hInjecter, int IFrameDataIndex)
{
	CSUDIPlayerChnl_S stPlayerCh;
	CSUDIINJECTERChnl_S stInjectCh;
	CSUDIINJECTEROpenParam_S stInjectOpenParams;
	CSUDIINJECTERIFrameParam_S stIFrameData;
	CSUDIStreamInfo_S stStreamInfo;

	CSUDI_Error_Code ErrCode = CSUDI_SUCCESS;
	int nVideoIndex = CSUDI_INVALID_INDEX;
	CSUDIVIDStreamType_E eIframeStreamType = EM_UDI_VID_STREAM_MPEG2; /*  assume it's mpeg2 video */

	nVideoIndex = lookup_video_device(eIframeStreamType);
	if (nVideoIndex == CSUDI_INVALID_INDEX)
	{
		printf("no available video decoder!\n");
		return CSUDI_FAILURE;
	}
	
	memset(&stInjectCh, 0, sizeof(stInjectCh));
	stInjectCh.m_nAudioDecoder = CSUDI_INVALID_INDEX;
	stInjectCh.m_nVideoDecoder = 0;
	stInjectCh.m_nDemux = CSUDI_INVALID_INDEX;
	memset(&stInjectOpenParams, 0, sizeof(stInjectOpenParams));
	stInjectOpenParams.m_eContentType = EM_UDIINJECTER_CONTENT_IFRAME;
	stInjectOpenParams.m_eInjecterType = EM_INJECTER_ES;
	ErrCode = CSUDIINJECTEROpen(&stInjectCh, &stInjectOpenParams, hInjecter);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"INJECT 打开失败");

	memset(&stPlayerCh, 0, sizeof(stPlayerCh));
	stPlayerCh.m_nAudioDecoder = CSUDI_INVALID_INDEX;
	stPlayerCh.m_nVideoDecoder = 0;
	stPlayerCh.m_nDemux = CSUDI_INVALID_INDEX;
	ErrCode = CSUDIPLAYEROpen(&stPlayerCh, EM_UDIPLAYER_INJECT, hPlayer);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"打开PLAYER 失败");

	ErrCode = CSUDIOSSemCreate("Sem4NewFrame", 0, 1, &g_hNewFrameSem);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"信号量申请失败");

	ErrCode = CSUDIPLAYERAddPlayerCallback(*hPlayer, 
								ifarme_player_callback, 
								EM_UDIPLAYER_VIDEO_FRAME_COMING,
								(void*)0);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"设置回调失败");

	get_iframe_data(&stIFrameData,IFrameDataIndex);
	ErrCode = CSUDIINJECTERSetAttribute(*hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, (void*)&stIFrameData);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"设置参数失败");

	memset(&stStreamInfo, 0, sizeof(stStreamInfo));
	stStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
	stStreamInfo.m_nPid = CSUDI_INVALID_PID;
	stStreamInfo.m_uStreamType.m_eVideoType = eIframeStreamType; 
	ErrCode = CSUDIPLAYERSetStream(*hPlayer, &stStreamInfo, 1, CSUDI_NULL);

	CSTK_FATAL_POINT;
	
	if (*hInjecter == CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(*hInjecter), "步骤10失败");	
	}
	return ErrCode;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试Open/Close IFrame注入时，在参数不符合设计要求情况下的执行情况
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:参数不符合Iframe注入接口设计要求
//@EXPECTATION:返回正确的错误码
//@EXECUTIONFLOW:1、CSUDIINJECTEROpen psInjecterChnl参数为NULL其它参数按I帧注入传入正确参数时，要求返回CSUDIINJECTER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2、CSUDIINJECTEROpen psOpenParams参数为NULL其它参数按I帧注入传入正确参数时，要求返回CSUDIINJECTER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3、CSUDIINJECTEROpen phINJECTER参数为NULL其它参数按I帧注入传入正确参数时，要求返回CSUDIINJECTER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、CSUDIINJECTERClose hINJECTER参数为NULL时，要求返回CSUDIINJECTER_ERROR_INVALID_HANDLE
CSUDI_BOOL CSTC_IFRAME_INJECTER_OpenClose_001(void)
{
	CSUDIINJECTERChnl_S stInjectCh;
	CSUDIINJECTEROpenParam_S stInjectOpenParams;

	CSUDI_HANDLE hInjecter = CSUDI_NULL;

	stInjectOpenParams.m_eInjecterType=EM_INJECTER_ES;		
	stInjectOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_IFRAME;
	stInjectCh.m_nDemux=CSUDI_INVALID_INDEX;
	stInjectCh.m_nVideoDecoder=0;		
	stInjectCh.m_nAudioDecoder = CSUDI_INVALID_INDEX;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (CSUDI_NULL,&stInjectOpenParams,&hInjecter), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&stInjectCh,CSUDI_NULL,&hInjecter), "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&stInjectCh,&stInjectOpenParams,CSUDI_NULL), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(CSUDI_NULL), "步骤4失败");

	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试多次创建销毁I帧注入的Injecter
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:创建销毁I帧注入的Injecter
//@EXPECTATION:CSUDIINJECTEROpen&CSUDIINJECTERClose都返回
//@EXECUTIONFLOW:1、调用CSUDIINJECTEROpen传入正确的参数创建I帧注入的Injecter，期望返回CSUDI_SUCCESS,且返回的handle不能为CSUDI_NULL
//@EXECUTIONFLOW:2、调用CSUDIINJECTERClose关闭创建的Injecter handle，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、重复上述步骤1-2 1000次
CSUDI_BOOL CSTC_IFRAME_INJECTER_OpenClose_002(void)
{
	CSUDIINJECTERChnl_S stInjectCh;
	CSUDIINJECTEROpenParam_S stInjectOpenParams;

	CSUDI_HANDLE hInjecter = CSUDI_NULL;

	int ii=0;

	stInjectOpenParams.m_eInjecterType=EM_INJECTER_ES;		
	stInjectOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_IFRAME;
	stInjectCh.m_nDemux=CSUDI_INVALID_INDEX;
	stInjectCh.m_nVideoDecoder=0;		
	stInjectCh.m_nAudioDecoder = CSUDI_INVALID_INDEX;
	
	for (ii=0; ii< 1000; ii++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen(&stInjectCh,&stInjectOpenParams,&hInjecter),"Step 1: 打开PLAYER 失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hInjecter),"Step 2: 关闭PLAYER 失败");
	}

	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:测试创建I帧注入的Injecter的时，同一个video设备不能多次Open
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:创建I帧注入的Injecter的时，同一个video设备多次Open
//@EXPECTATION:CSUDIINJECTEROpen返回CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:1、使用VideoDecoder 0来Open I帧注入的Injecter，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、再次使用VideoDecoder 0来Open I帧注入的Injecter，要求返回CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:3、调用CSUDIINJECTERClose关闭上次成功创建的Injecter，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复上述步骤1 - 3
CSUDI_BOOL CSTC_IFRAME_INJECTER_OpenClose_003(void)
{
	CSUDIINJECTERChnl_S stInjectCh;
	CSUDIINJECTEROpenParam_S stInjectOpenParams;

	CSUDI_HANDLE hInjecter = CSUDI_NULL,hInjecter2 = CSUDI_NULL;

	int ii=0;

	stInjectOpenParams.m_eInjecterType=EM_INJECTER_ES;		
	stInjectOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_IFRAME;
	stInjectCh.m_nDemux=CSUDI_INVALID_INDEX;
	stInjectCh.m_nVideoDecoder=0;		
	stInjectCh.m_nAudioDecoder = CSUDI_INVALID_INDEX;
	
	for (ii=0; ii< MAX_TEST_TIME; ii++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen(&stInjectCh,&stInjectOpenParams,&hInjecter),"Step 1: 打开PLAYER 失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_BUSY == CSUDIINJECTEROpen(&stInjectCh,&stInjectOpenParams,&hInjecter2),"Step 2: 打开应该PLAYER 失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hInjecter),"Step 3: 关闭PLAYER 失败");
	}

CSTK_FATAL_POINT;
	if (hInjecter)
	{
		CSUDIINJECTERClose(hInjecter);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute
//@DESCRIPTION:测试IFrame注入时，CSUDIINJECTERSetAttribute错误参数处理
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:CSUDIINJECTERSetAttribute输入错误参数
//@EXPECTATION:返回正确的错误码
//@EXECUTIONFLOW:1、调用CSUDIINJECTEROpen创建I帧注入的Injecter
//@EXECUTIONFLOW:2、调用CSUDIINJECTERSetAttribute设置EM_UDIINJECTER_IFRAME_PARAMS，但pvData为CSUDI_NULL，期望返回错误
//@EXECUTIONFLOW:3、调用CSUDIINJECTERSetAttribute设置EM_UDIINJECTER_PCM_PARAMS，pvData为正确的PCM参数，期望返回错误
//@EXECUTIONFLOW:4、调用CSUDIINJECTERSetAttribute设置EM_UDIINJECTER_PCM_ENDIAN,pvData为正确的PCM大小端参数，期望返回错误
//@EXECUTIONFLOW:5、调用CSUDIINJECTERClose销毁之前创建的I帧注入的Injecter，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IFRAME_INJECTER_SetGetAttribute_001(void)
{
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;

	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;		
	CSUDIINJECTERPcmStartParam_S sPcmStartParam = {16, 48000, 2};	
	CSUDIINJECTERPcmEndian_E      ePcmLitEndian = EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmEndian_E      ePcmBigEndian = EM_UDIINJECTER_BIG_ENDIAN;
	
	sInjecterChnl.m_nDemux=CSUDI_INVALID_INDEX;
	sInjecterChnl.m_nVideoDecoder=0;
	sInjecterChnl.m_nAudioDecoder=CSUDI_INVALID_INDEX;

	sOpenParams.m_eInjecterType=EM_INJECTER_ES ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_IFRAME;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_IFRAME_PARAMS,CSUDI_NULL),  "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),  "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN, &ePcmLitEndian),  "步骤4失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN, &ePcmBigEndian),  "步骤4失败");
	
CSTK_FATAL_POINT;
	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERGetAttribute
//@DESCRIPTION:测试IFrame注入时，CSUDIINJECTERGetAttribute能正常获取设置的I帧参数
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:CSUDIINJECTERSetAttribute输入正确参数
//@EXPECTATION:CSUDIINJECTERGetAttribute能正常获取设置的I帧参数
//@EXECUTIONFLOW:1、调用CSUDIINJECTEROpen创建I帧注入的Injecter
//@EXECUTIONFLOW:2、调用CSUDIINJECTERSetAttribute设置EM_UDIINJECTER_IFRAME_PARAMS，pvData为正确的I帧参数，期望返回成功
//@EXECUTIONFLOW:3、调用CSUDIINJECTERGetAttribute获取EM_UDIINJECTER_IFRAME_PARAMS，期望返回成功
//@EXECUTIONFLOW:4、期望设置的跟获取的Iframe参数一致
//@EXECUTIONFLOW:5、调用CSUDIINJECTERClose销毁之前创建的I帧注入的Injecter，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IFRAME_INJECTER_SetGetAttribute_002(void)
{
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;

	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;		
	CSUDIINJECTERIFrameParam_S stIFrameData;
	CSUDIINJECTERIFrameParam_S stIFrameDataGet;	
	
	sInjecterChnl.m_nDemux=CSUDI_INVALID_INDEX;
	sInjecterChnl.m_nVideoDecoder=0;
	sInjecterChnl.m_nAudioDecoder=CSUDI_INVALID_INDEX;

	sOpenParams.m_eInjecterType=EM_INJECTER_ES ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_IFRAME;

	get_iframe_data(&stIFrameData,IFRAME_DATA_DEFAULT);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_IFRAME_PARAMS,&stIFrameData),  "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_IFRAME_PARAMS,&stIFrameDataGet),  "步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(stIFrameData.m_pcIFrameData == stIFrameDataGet.m_pcIFrameData,  "步骤4失败, CSUDIINJECTERIFrameParam_S::m_pcIFrameData compare failed!");
	CSTK_ASSERT_TRUE_FATAL(stIFrameData.m_nDataLen == stIFrameDataGet.m_nDataLen,  "步骤4失败, CSUDIINJECTERIFrameParam_S::m_nDataLen compare failed!");
	
CSTK_FATAL_POINT;
	
	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试IFrame注入Injecter，多次连续CSUDIINJECTERStart & CSUDIINJECTERStop的错误处理
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:连续两次调用CSUDIINJECTERStart、连续两次调用CSUDIINJECTERStop
//@EXPECTATION:返回正确的错误码
//@EXECUTIONFLOW:1、调用CSUDIINJECTEROpen创建I帧注入的Injecter
//@EXECUTIONFLOW:2、调用CSUDIINJECTERSetAttribute设置EM_UDIINJECTER_IFRAME_PARAMS，pvData为正确的I帧参数，期望返回成功
//@EXECUTIONFLOW:3、再次调用CSUDIINJECTERStop期望返回CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:4、调用CSUDIINJECTERStart期望返回成功
//@EXECUTIONFLOW:5、再次调用CSUDIINJECTERStart期望返回CSUDIINJECTER_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStop期望返回成功
//@EXECUTIONFLOW:7、再次调用CSUDIINJECTERStop期望返回CSUDIINJECTER_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:8、调用CSUDIINJECTERClose销毁之前创建的I帧注入的Injecter，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_IFRAME_INJECTER_StartStop_001(void)
{
	CSUDI_HANDLE hInjecter = CSUDI_NULL;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	CSUDIINJECTEROpenParam_S  sOpenParams;		
	CSUDIINJECTERIFrameParam_S stIFrameData;

	sInjecterChnl.m_nDemux=CSUDI_INVALID_INDEX;
	sInjecterChnl.m_nVideoDecoder=0;
	sInjecterChnl.m_nAudioDecoder=CSUDI_INVALID_INDEX;

	sOpenParams.m_eInjecterType=EM_INJECTER_ES ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_IFRAME;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hInjecter), "步骤1失败");

	get_iframe_data(&stIFrameData,IFRAME_DATA_DEFAULT);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hInjecter,EM_UDIINJECTER_IFRAME_PARAMS,&stIFrameData),  "步骤2失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERStop(hInjecter), "步骤3失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter), "步骤4失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_ALREADY_STARTED == CSUDIINJECTERStart(hInjecter), "步骤5失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInjecter), "步骤6失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERStop(hInjecter), "步骤7失败");

CSTK_FATAL_POINT;
	
	if(hInjecter != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hInjecter);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试Start/Stop IFrame注入时，在CSUDIVIDEOShow参数TRUE 和FALSE时显示的结果
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:参数符合Iframe注入接口设计要求
//@EXPECTATION:返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、正常初始化I帧解码的Inject&Player,并启动I帧解码
//@EXECUTIONFLOW:2、调用CSUDIVIDEOShow参数bIsShow为TRUE，提示用户确认I帧正常显示
//@EXECUTIONFLOW:3、调用CSUDIVIDEOShow参数bIsShow为FALSE，提示用户确认I帧没有显示，实际为黑屏
//@EXECUTIONFLOW:4、重新解码I帧，提示用户确认I帧没有显示，实际为黑屏
//@EXECUTIONFLOW:5、再次调用CSUDIVIDEOShow参数bIsShow为TRUE，提示用户确认I帧正常显示
//@EXECUTIONFLOW:6、调用CSUDIINJECTERStop 和CSUDIPLAYERStop 停止解码I帧
CSUDI_BOOL CSTC_IFRAME_INJECTER_001(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hInjecter = CSUDI_NULL;
	CSUDIINJECTERIFrameParam_S stIFrameData;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == SetupIFrameInjecter(&hPlayer,&hInjecter, IFRAME_DATA_DEFAULT),"参数设置失败");

	/* 暂存I帧参数*/
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, &stIFrameData),"获取IFrame参数失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERStart(hInjecter),"步骤1失败:Inject Start  失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer),"player Start  失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem,3000), "iframe解码超时");
	CSUDIVIDEOShow(0, CSUDI_TRUE);
	
	CSTCPrint("请确认I帧是否显示成功?\n");
    CSTCPrint("We expect there's an Iframe on the screen !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤2失败");

	CSUDIVIDEOShow(0, CSUDI_FALSE);
	CSTCPrint("请确认I帧是否关闭?\n");
    CSTCPrint("We expect the Iframe is turned off !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERStop(hInjecter),"Inject Start  失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer),"player Start  失败");

	/* reset event */
	while (CSUDIOSSemWait(g_hNewFrameSem,0) == CSUDI_SUCCESS);
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, &stIFrameData),"设置参数失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERStart(hInjecter),"Inject Start  失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer),"player Start  失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem,3000), "iframe解码超时");

	CSTCPrint("再次解码I帧，但当前应该为黑屏\n");
    CSTCPrint("We expect there's no Iframe on the screen !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤4失败");

	CSUDIVIDEOShow(0, CSUDI_TRUE);
	CSTCPrint("请确认I帧正常显示\n");
    CSTCPrint("We expect there's an Iframe on the screen !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤5失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERStop(hInjecter),"Inject Stop  失败");

	//2011.5.17 10:30  用例结束前删除回调，避免下次增加同事件回调失败(由于在CSUDIPLAYERClose之后调用，hPlayer被置空)
	CSUDIPLAYERDelPlayerCallback(hPlayer,ifarme_player_callback,EM_UDIPLAYER_VIDEO_FRAME_COMING,(void*)0);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer),"player Stop  失败");

	CSTK_FATAL_POINT
	
	if(hInjecter != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hInjecter);
	}

	if(hPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERClose(hPlayer);
	}

	CSUDIOSMutexWait(g_hIframeTestMutex, 100);
	if (g_hNewFrameSem != CSUDI_NULL)
	{
		CSUDIOSSemDestroy(g_hNewFrameSem);
		g_hNewFrameSem = CSUDI_NULL;
	}
	CSUDIOSMutexRelease(g_hIframeTestMutex);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:I帧解码性能测试，自动化测试
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:576i I帧数据
//@EXPECTATION:从启动注入解码到收到新帧事件，不得超过500ms
//@EXECUTIONFLOW:1、初始化I帧注入的Player和Injecter
//@EXECUTIONFLOW:2、通过CSUDIINJECTERSetAttribute设置576i的I帧数据
//@EXECUTIONFLOW:3、注册New Frame回调
//@EXECUTIONFLOW:4、创建用于等待新帧事件的信号量hNewFrameSem，新帧事件中会release信号量A
//@EXECUTIONFLOW:5、启动I帧解码
//@EXECUTIONFLOW:6、等待信号量hNewFrameSem，超时为500ms
//@EXECUTIONFLOW:7、信号量hNewFrameSem超时，则测试失败，反之测试成功
//@EXECUTIONFLOW:8、停止I帧解码，反注册事件，删除信号量hNewFrameSem,关闭player&injecter
CSUDI_BOOL CSTC_IFRAME_INJECTER_002(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hInjecter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == SetupIFrameInjecter(&hPlayer, &hInjecter, IFRAME_DATA_DEFAULT),"Step 1~4: Iframe注入初始化失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter),"Step 5: inject start 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"Step 5: player start 失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT != CSUDIOSSemWait(g_hNewFrameSem,500), "iframe解码超时,测试失败");

	CSTK_FATAL_POINT;

	if (hInjecter != CSUDI_NULL)
	{
		CSUDIINJECTERStop(hInjecter);
		CSUDIINJECTERClose(hInjecter);
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERStop(hPlayer);

	//2011.5.17 10:30  用例结束前删除回调，避免下次增加同事件回调失败(由于在CSUDIPLAYERClose之后调用，hPlayer被置空)
	CSUDIPLAYERDelPlayerCallback(hPlayer,ifarme_player_callback,EM_UDIPLAYER_VIDEO_FRAME_COMING,(void*)0);
		
		CSUDIPLAYERClose(hPlayer);
	}

	CSUDIOSMutexWait(g_hIframeTestMutex, 100);
	if (g_hNewFrameSem != CSUDI_NULL)
	{
		CSUDIOSSemDestroy(g_hNewFrameSem);
		g_hNewFrameSem = CSUDI_NULL;
	}
	CSUDIOSMutexRelease(g_hIframeTestMutex);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop & CSUDIVIDEOSetStopMode
//@DESCRIPTION:CSUDIVIDEOSetStopMode与播I帧组合测试
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:设置黑屏模式和静帧模式
//@EXPECTATION:1）黑屏模式，在已播出I frame的情况下，CSUDIPLAYERStop黑屏
//@EXPECTATION:2）静帧模式，在已播出I frame的情况下，CSUDIPLAYERStop,I frame保持不变
//@EXECUTIONFLOW:1、初始化I帧注入用的Injecter和Player，设置I帧数据
//@EXECUTIONFLOW:2、启动I帧解码并调用CSUDIVIDEOShow显示I帧
//@EXECUTIONFLOW:3、提示用户确认I帧正常显示
//@EXECUTIONFLOW:4、调用CSUDIVIDEOSetStopMode设置为black模式，并调用CSUDIPLAYERStop
//@EXECUTIONFLOW:5、提示用户确认当前为黑屏
//@EXECUTIONFLOW:6、启动I帧解码并调用CSUDIVIDEOShow显示I帧
//@EXECUTIONFLOW:7、调用CSUDIVIDEOSetStopMode设置为freeze模式，并调用CSUDIPLAYERStop
//@EXECUTIONFLOW:8、提示用户确认当前I帧正常显示，且为静帧
//@EXECUTIONFLOW:9、停止I帧解码，关闭player&injecter
CSUDI_BOOL CSTC_IFRAME_INJECTER_003(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hInjecter = CSUDI_NULL;
	CSUDIINJECTERIFrameParam_S stIFrameData;
	CSUDIVIDEOStopMode_E eOldStopMode;
	
	CSUDIVIDEOGetStopMode(0,&eOldStopMode);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == SetupIFrameInjecter(&hPlayer, &hInjecter, IFRAME_DATA_DEFAULT),"Step 1: Iframe注入初始化失败");

	/* 暂存I帧参数*/
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, &stIFrameData),"获取IFrame参数失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter),"Step 2: inject start 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"Step 2: player start 失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem,3000), "iframe解码超时");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, CSUDI_TRUE),"player start 失败");	

	CSTCPrint("请确认是否正常显示\n");
    CSTCPrint("We expect there's an Iframe on the screen!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 3:失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0,EM_UDIVIDEO_STOPMODE_BLACK), "Step 4:设stop mode失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"Step 4:player stop 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInjecter),"Step 4:inject stop 失败");

	CSTCPrint("当前是否为黑屏?\n");
    CSTCPrint("We expect there's no Iframe on the screen !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 5:失败");

	/* reset event */
	while (CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem, 1000));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, &stIFrameData),"Setp 6:获取IFrame参数失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter),"Setp 6:inject start 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"Setp 6:player start 失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem, 1000), "iframe解码超时");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, CSUDI_TRUE),"player start 失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0,EM_UDIVIDEO_STOPMODE_FREEZE), "Step 7 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"player stop 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInjecter),"inject stop 失败");

	CSTCPrint("请确认是否为静帧(I帧正常显示)?\n");
    CSTCPrint("We expect there's an Iframe on the screen!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"步骤8失败");
	
CSTK_FATAL_POINT;
	CSUDIVIDEOShow(0, CSUDI_FALSE);
	CSUDIVIDEOSetStopMode(0, eOldStopMode);
	
	if (hInjecter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hInjecter), "inject close 失败");
	}
	//2011.5.17 10:30  用例结束前删除回调，避免下次增加同事件回调失败(由于在CSUDIPLAYERClose之后调用，hPlayer被置空)
	CSUDIPLAYERDelPlayerCallback(hPlayer,ifarme_player_callback,EM_UDIPLAYER_VIDEO_FRAME_COMING,(void*)0);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose(hPlayer), "player close 失败");	
	}

	CSUDIOSMutexWait(g_hIframeTestMutex, 100);
        if(g_hNewFrameSem != CSUDI_NULL)
        {
	  	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(g_hNewFrameSem),"Sem 删除失败");
	 	 g_hNewFrameSem = CSUDI_NULL;
        }
	CSUDIOSMutexRelease(g_hIframeTestMutex);
	
	return CSUDI_TRUE;
}

CSUDI_BOOL DecodeIFrameTest(int nIframeIndex)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hInjecter = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == SetupIFrameInjecter(&hPlayer, &hInjecter, nIframeIndex),"Step 1: Iframe注入初始化失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter),"Step 2: inject start 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"Step 2: player start 失败");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem,1000), "iframe解码超时");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, CSUDI_TRUE),"player start 失败");	

	CSTCPrint("请确认是否正常显示\n");
    CSTCPrint("We expect there's an Iframe on the screen!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 3:失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"Step 4:player stop 失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInjecter),"Step 4:inject stop 失败");

CSTK_FATAL_POINT;
	CSUDIOSMutexWait(g_hIframeTestMutex, 100);
      if(g_hNewFrameSem != CSUDI_NULL)
      {
		CSUDIOSSemDestroy(g_hNewFrameSem);
		g_hNewFrameSem = CSUDI_NULL;
      }
	CSUDIOSMutexRelease(g_hIframeTestMutex);  
	
	if (hInjecter != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hInjecter);
	}

	if (hPlayer != CSUDI_NULL)
	{
		//2011.5.17 10:30  用例结束前删除回调，避免下次增加同事件回调失败(由于在CSUDIPLAYERClose之后调用，hPlayer被置空)
		CSUDIPLAYERDelPlayerCallback(hPlayer,ifarme_player_callback,EM_UDIPLAYER_VIDEO_FRAME_COMING,(void*)0);

		CSUDIPLAYERClose(hPlayer);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试分辨率为480i的I帧数据
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:480i的I帧数据
//@EXPECTATION:正常解码显示
//@EXECUTIONFLOW:1、正常初始化I帧解码的Inject&Player,
//@EXECUTIONFLOW:2、设置向injecter设置分辨率为480i的I帧数据
//@EXECUTIONFLOW:3、启动I帧解码并调用CSUDIVIDEOShow参数bIsShow为TRUE
//@EXECUTIONFLOW:4、提示用户确认I帧正常显示
//@EXECUTIONFLOW:5、停止I帧解码，并释放Player/Injecter Handle
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_480I(void)
{
	return DecodeIFrameTest(IFRAME_DATA_480I);
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试分辨率为576i的I帧数据
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:576i的I帧数据
//@EXPECTATION:正常解码显示
//@EXECUTIONFLOW:1、正常初始化I帧解码的Inject&Player,
//@EXECUTIONFLOW:2、设置向injecter设置分辨率为576i的I帧数据
//@EXECUTIONFLOW:3、启动I帧解码并调用CSUDIVIDEOShow参数bIsShow为TRUE
//@EXECUTIONFLOW:4、提示用户确认I帧正常显示
//@EXECUTIONFLOW:5、停止I帧解码，并释放Player/Injecter Handle
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_576I(void)
{
	return DecodeIFrameTest(IFRAME_DATA_576I);
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试分辨率为720p的I帧数据
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:720p的I帧数据
//@EXPECTATION:正常解码显示
//@EXECUTIONFLOW:1、正常初始化I帧解码的Inject&Player,
//@EXECUTIONFLOW:2、设置向injecter设置分辨率为720p的I帧数据
//@EXECUTIONFLOW:3、启动I帧解码并调用CSUDIVIDEOShow参数bIsShow为TRUE
//@EXECUTIONFLOW:4、提示用户确认I帧正常显示
//@EXECUTIONFLOW:5、停止I帧解码，并释放Player/Injecter Handle
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_720P(void)
{
	return DecodeIFrameTest(IFRAME_DATA_720P);
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:测试分辨率为1080i的I帧数据
//@PRECONDITION:INJECTER模块被正常初始化
//@INPUT:参数符合Iframe注入接口设计要求
//@EXPECTATION:正常解码显示
//@EXECUTIONFLOW:1、正常初始化I帧解码的Inject&Player,
//@EXECUTIONFLOW:2、设置向injecter设置分辨率为1080i的I帧数据
//@EXECUTIONFLOW:3、启动I帧解码并调用CSUDIVIDEOShow参数bIsShow为TRUE
//@EXECUTIONFLOW:4、提示用户确认I帧正常显示
//@EXECUTIONFLOW:5、停止I帧解码，并释放Player/Injecter Handle
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_1080I(void)
{
	return DecodeIFrameTest(IFRAME_DATA_1080I);
}

