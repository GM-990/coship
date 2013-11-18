/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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
	//�ڱ�����������ִ��ǰ����
	 CSUDIOSMutexCreate("default", CSUDIOS_MUTEX_DEFAULT, &g_hIframeTestMutex);
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_IFRAME_INJECTER_UnInit(void)
{
	//�ڱ�����������ִ�к����
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
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"INJECT ��ʧ��");

	memset(&stPlayerCh, 0, sizeof(stPlayerCh));
	stPlayerCh.m_nAudioDecoder = CSUDI_INVALID_INDEX;
	stPlayerCh.m_nVideoDecoder = 0;
	stPlayerCh.m_nDemux = CSUDI_INVALID_INDEX;
	ErrCode = CSUDIPLAYEROpen(&stPlayerCh, EM_UDIPLAYER_INJECT, hPlayer);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"��PLAYER ʧ��");

	ErrCode = CSUDIOSSemCreate("Sem4NewFrame", 0, 1, &g_hNewFrameSem);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"�ź�������ʧ��");

	ErrCode = CSUDIPLAYERAddPlayerCallback(*hPlayer, 
								ifarme_player_callback, 
								EM_UDIPLAYER_VIDEO_FRAME_COMING,
								(void*)0);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"���ûص�ʧ��");

	get_iframe_data(&stIFrameData,IFrameDataIndex);
	ErrCode = CSUDIINJECTERSetAttribute(*hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, (void*)&stIFrameData);
	CSTK_ASSERT_TRUE_FATAL((ErrCode == CSUDI_SUCCESS),"���ò���ʧ��");

	memset(&stStreamInfo, 0, sizeof(stStreamInfo));
	stStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;
	stStreamInfo.m_nPid = CSUDI_INVALID_PID;
	stStreamInfo.m_uStreamType.m_eVideoType = eIframeStreamType; 
	ErrCode = CSUDIPLAYERSetStream(*hPlayer, &stStreamInfo, 1, CSUDI_NULL);

	CSTK_FATAL_POINT;
	
	if (*hInjecter == CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(*hInjecter), "����10ʧ��");	
	}
	return ErrCode;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:����Open/Close IFrameע��ʱ���ڲ������������Ҫ������µ�ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:����������Iframeע��ӿ����Ҫ��
//@EXPECTATION:������ȷ�Ĵ�����
//@EXECUTIONFLOW:1��CSUDIINJECTEROpen psInjecterChnl����ΪNULL����������I֡ע�봫����ȷ����ʱ��Ҫ�󷵻�CSUDIINJECTER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2��CSUDIINJECTEROpen psOpenParams����ΪNULL����������I֡ע�봫����ȷ����ʱ��Ҫ�󷵻�CSUDIINJECTER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3��CSUDIINJECTEROpen phINJECTER����ΪNULL����������I֡ע�봫����ȷ����ʱ��Ҫ�󷵻�CSUDIINJECTER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4��CSUDIINJECTERClose hINJECTER����ΪNULLʱ��Ҫ�󷵻�CSUDIINJECTER_ERROR_INVALID_HANDLE
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (CSUDI_NULL,&stInjectOpenParams,&hInjecter), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&stInjectCh,CSUDI_NULL,&hInjecter), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&stInjectCh,&stInjectOpenParams,CSUDI_NULL), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(CSUDI_NULL), "����4ʧ��");

	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:���Զ�δ�������I֡ע���Injecter
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:��������I֡ע���Injecter
//@EXPECTATION:CSUDIINJECTEROpen&CSUDIINJECTERClose������
//@EXECUTIONFLOW:1������CSUDIINJECTEROpen������ȷ�Ĳ�������I֡ע���Injecter����������CSUDI_SUCCESS,�ҷ��ص�handle����ΪCSUDI_NULL
//@EXECUTIONFLOW:2������CSUDIINJECTERClose�رմ�����Injecter handle����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3���ظ���������1-2 1000��
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen(&stInjectCh,&stInjectOpenParams,&hInjecter),"Step 1: ��PLAYER ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hInjecter),"Step 2: �ر�PLAYER ʧ��");
	}

	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:���Դ���I֡ע���Injecter��ʱ��ͬһ��video�豸���ܶ��Open
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:����I֡ע���Injecter��ʱ��ͬһ��video�豸���Open
//@EXPECTATION:CSUDIINJECTEROpen����CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:1��ʹ��VideoDecoder 0��Open I֡ע���Injecter��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:2���ٴ�ʹ��VideoDecoder 0��Open I֡ע���Injecter��Ҫ�󷵻�CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:3������CSUDIINJECTERClose�ر��ϴγɹ�������Injecter��Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ���������1 - 3
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen(&stInjectCh,&stInjectOpenParams,&hInjecter),"Step 1: ��PLAYER ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_BUSY == CSUDIINJECTEROpen(&stInjectCh,&stInjectOpenParams,&hInjecter2),"Step 2: ��Ӧ��PLAYER ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hInjecter),"Step 3: �ر�PLAYER ʧ��");
	}

CSTK_FATAL_POINT;
	if (hInjecter)
	{
		CSUDIINJECTERClose(hInjecter);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute
//@DESCRIPTION:����IFrameע��ʱ��CSUDIINJECTERSetAttribute�����������
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:CSUDIINJECTERSetAttribute����������
//@EXPECTATION:������ȷ�Ĵ�����
//@EXECUTIONFLOW:1������CSUDIINJECTEROpen����I֡ע���Injecter
//@EXECUTIONFLOW:2������CSUDIINJECTERSetAttribute����EM_UDIINJECTER_IFRAME_PARAMS����pvDataΪCSUDI_NULL���������ش���
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����EM_UDIINJECTER_PCM_PARAMS��pvDataΪ��ȷ��PCM�������������ش���
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����EM_UDIINJECTER_PCM_ENDIAN,pvDataΪ��ȷ��PCM��С�˲������������ش���
//@EXECUTIONFLOW:5������CSUDIINJECTERClose����֮ǰ������I֡ע���Injecter����������CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_IFRAME_PARAMS,CSUDI_NULL),  "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),  "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN, &ePcmLitEndian),  "����4ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN, &ePcmBigEndian),  "����4ʧ��");
	
CSTK_FATAL_POINT;
	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERGetAttribute
//@DESCRIPTION:����IFrameע��ʱ��CSUDIINJECTERGetAttribute��������ȡ���õ�I֡����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:CSUDIINJECTERSetAttribute������ȷ����
//@EXPECTATION:CSUDIINJECTERGetAttribute��������ȡ���õ�I֡����
//@EXECUTIONFLOW:1������CSUDIINJECTEROpen����I֡ע���Injecter
//@EXECUTIONFLOW:2������CSUDIINJECTERSetAttribute����EM_UDIINJECTER_IFRAME_PARAMS��pvDataΪ��ȷ��I֡�������������سɹ�
//@EXECUTIONFLOW:3������CSUDIINJECTERGetAttribute��ȡEM_UDIINJECTER_IFRAME_PARAMS���������سɹ�
//@EXECUTIONFLOW:4���������õĸ���ȡ��Iframe����һ��
//@EXECUTIONFLOW:5������CSUDIINJECTERClose����֮ǰ������I֡ע���Injecter����������CSUDI_SUCCESS
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_IFRAME_PARAMS,&stIFrameData),  "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_IFRAME_PARAMS,&stIFrameDataGet),  "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(stIFrameData.m_pcIFrameData == stIFrameDataGet.m_pcIFrameData,  "����4ʧ��, CSUDIINJECTERIFrameParam_S::m_pcIFrameData compare failed!");
	CSTK_ASSERT_TRUE_FATAL(stIFrameData.m_nDataLen == stIFrameDataGet.m_nDataLen,  "����4ʧ��, CSUDIINJECTERIFrameParam_S::m_nDataLen compare failed!");
	
CSTK_FATAL_POINT;
	
	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:����IFrameע��Injecter���������CSUDIINJECTERStart & CSUDIINJECTERStop�Ĵ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:�������ε���CSUDIINJECTERStart���������ε���CSUDIINJECTERStop
//@EXPECTATION:������ȷ�Ĵ�����
//@EXECUTIONFLOW:1������CSUDIINJECTEROpen����I֡ע���Injecter
//@EXECUTIONFLOW:2������CSUDIINJECTERSetAttribute����EM_UDIINJECTER_IFRAME_PARAMS��pvDataΪ��ȷ��I֡�������������سɹ�
//@EXECUTIONFLOW:3���ٴε���CSUDIINJECTERStop��������CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:4������CSUDIINJECTERStart�������سɹ�
//@EXECUTIONFLOW:5���ٴε���CSUDIINJECTERStart��������CSUDIINJECTER_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:6������CSUDIINJECTERStop�������سɹ�
//@EXECUTIONFLOW:7���ٴε���CSUDIINJECTERStop��������CSUDIINJECTER_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:8������CSUDIINJECTERClose����֮ǰ������I֡ע���Injecter����������CSUDI_SUCCESS
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hInjecter), "����1ʧ��");

	get_iframe_data(&stIFrameData,IFRAME_DATA_DEFAULT);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hInjecter,EM_UDIINJECTER_IFRAME_PARAMS,&stIFrameData),  "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERStop(hInjecter), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter), "����4ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_ALREADY_STARTED == CSUDIINJECTERStart(hInjecter), "����5ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInjecter), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERStop(hInjecter), "����7ʧ��");

CSTK_FATAL_POINT;
	
	if(hInjecter != CSUDI_NULL)
	{
		CSUDIINJECTERClose(hInjecter);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:����Start/Stop IFrameע��ʱ����CSUDIVIDEOShow����TRUE ��FALSEʱ��ʾ�Ľ��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:��������Iframeע��ӿ����Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1��������ʼ��I֡�����Inject&Player,������I֡����
//@EXECUTIONFLOW:2������CSUDIVIDEOShow����bIsShowΪTRUE����ʾ�û�ȷ��I֡������ʾ
//@EXECUTIONFLOW:3������CSUDIVIDEOShow����bIsShowΪFALSE����ʾ�û�ȷ��I֡û����ʾ��ʵ��Ϊ����
//@EXECUTIONFLOW:4�����½���I֡����ʾ�û�ȷ��I֡û����ʾ��ʵ��Ϊ����
//@EXECUTIONFLOW:5���ٴε���CSUDIVIDEOShow����bIsShowΪTRUE����ʾ�û�ȷ��I֡������ʾ
//@EXECUTIONFLOW:6������CSUDIINJECTERStop ��CSUDIPLAYERStop ֹͣ����I֡
CSUDI_BOOL CSTC_IFRAME_INJECTER_001(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hInjecter = CSUDI_NULL;
	CSUDIINJECTERIFrameParam_S stIFrameData;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == SetupIFrameInjecter(&hPlayer,&hInjecter, IFRAME_DATA_DEFAULT),"��������ʧ��");

	/* �ݴ�I֡����*/
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, &stIFrameData),"��ȡIFrame����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERStart(hInjecter),"����1ʧ��:Inject Start  ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer),"player Start  ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem,3000), "iframe���볬ʱ");
	CSUDIVIDEOShow(0, CSUDI_TRUE);
	
	CSTCPrint("��ȷ��I֡�Ƿ���ʾ�ɹ�?\n");
    CSTCPrint("We expect there's an Iframe on the screen !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����2ʧ��");

	CSUDIVIDEOShow(0, CSUDI_FALSE);
	CSTCPrint("��ȷ��I֡�Ƿ�ر�?\n");
    CSTCPrint("We expect the Iframe is turned off !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERStop(hInjecter),"Inject Start  ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer),"player Start  ʧ��");

	/* reset event */
	while (CSUDIOSSemWait(g_hNewFrameSem,0) == CSUDI_SUCCESS);
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, &stIFrameData),"���ò���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERStart(hInjecter),"Inject Start  ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer),"player Start  ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem,3000), "iframe���볬ʱ");

	CSTCPrint("�ٴν���I֡������ǰӦ��Ϊ����\n");
    CSTCPrint("We expect there's no Iframe on the screen !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����4ʧ��");

	CSUDIVIDEOShow(0, CSUDI_TRUE);
	CSTCPrint("��ȷ��I֡������ʾ\n");
    CSTCPrint("We expect there's an Iframe on the screen !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERStop(hInjecter),"Inject Stop  ʧ��");

	//2011.5.17 10:30  ��������ǰɾ���ص��������´�����ͬ�¼��ص�ʧ��(������CSUDIPLAYERClose֮����ã�hPlayer���ÿ�)
	CSUDIPLAYERDelPlayerCallback(hPlayer,ifarme_player_callback,EM_UDIPLAYER_VIDEO_FRAME_COMING,(void*)0);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer),"player Stop  ʧ��");

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
//@DESCRIPTION:I֡�������ܲ��ԣ��Զ�������
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:576i I֡����
//@EXPECTATION:������ע����뵽�յ���֡�¼������ó���500ms
//@EXECUTIONFLOW:1����ʼ��I֡ע���Player��Injecter
//@EXECUTIONFLOW:2��ͨ��CSUDIINJECTERSetAttribute����576i��I֡����
//@EXECUTIONFLOW:3��ע��New Frame�ص�
//@EXECUTIONFLOW:4���������ڵȴ���֡�¼����ź���hNewFrameSem����֡�¼��л�release�ź���A
//@EXECUTIONFLOW:5������I֡����
//@EXECUTIONFLOW:6���ȴ��ź���hNewFrameSem����ʱΪ500ms
//@EXECUTIONFLOW:7���ź���hNewFrameSem��ʱ�������ʧ�ܣ���֮���Գɹ�
//@EXECUTIONFLOW:8��ֹͣI֡���룬��ע���¼���ɾ���ź���hNewFrameSem,�ر�player&injecter
CSUDI_BOOL CSTC_IFRAME_INJECTER_002(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hInjecter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == SetupIFrameInjecter(&hPlayer, &hInjecter, IFRAME_DATA_DEFAULT),"Step 1~4: Iframeע���ʼ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter),"Step 5: inject start ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"Step 5: player start ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDIOS_ERROR_TIMEOUT != CSUDIOSSemWait(g_hNewFrameSem,500), "iframe���볬ʱ,����ʧ��");

	CSTK_FATAL_POINT;

	if (hInjecter != CSUDI_NULL)
	{
		CSUDIINJECTERStop(hInjecter);
		CSUDIINJECTERClose(hInjecter);
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSUDIPLAYERStop(hPlayer);

	//2011.5.17 10:30  ��������ǰɾ���ص��������´�����ͬ�¼��ص�ʧ��(������CSUDIPLAYERClose֮����ã�hPlayer���ÿ�)
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
//@DESCRIPTION:CSUDIVIDEOSetStopMode�벥I֡��ϲ���
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:���ú���ģʽ�;�֡ģʽ
//@EXPECTATION:1������ģʽ�����Ѳ���I frame������£�CSUDIPLAYERStop����
//@EXPECTATION:2����֡ģʽ�����Ѳ���I frame������£�CSUDIPLAYERStop,I frame���ֲ���
//@EXECUTIONFLOW:1����ʼ��I֡ע���õ�Injecter��Player������I֡����
//@EXECUTIONFLOW:2������I֡���벢����CSUDIVIDEOShow��ʾI֡
//@EXECUTIONFLOW:3����ʾ�û�ȷ��I֡������ʾ
//@EXECUTIONFLOW:4������CSUDIVIDEOSetStopMode����Ϊblackģʽ��������CSUDIPLAYERStop
//@EXECUTIONFLOW:5����ʾ�û�ȷ�ϵ�ǰΪ����
//@EXECUTIONFLOW:6������I֡���벢����CSUDIVIDEOShow��ʾI֡
//@EXECUTIONFLOW:7������CSUDIVIDEOSetStopMode����Ϊfreezeģʽ��������CSUDIPLAYERStop
//@EXECUTIONFLOW:8����ʾ�û�ȷ�ϵ�ǰI֡������ʾ����Ϊ��֡
//@EXECUTIONFLOW:9��ֹͣI֡���룬�ر�player&injecter
CSUDI_BOOL CSTC_IFRAME_INJECTER_003(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hInjecter = CSUDI_NULL;
	CSUDIINJECTERIFrameParam_S stIFrameData;
	CSUDIVIDEOStopMode_E eOldStopMode;
	
	CSUDIVIDEOGetStopMode(0,&eOldStopMode);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == SetupIFrameInjecter(&hPlayer, &hInjecter, IFRAME_DATA_DEFAULT),"Step 1: Iframeע���ʼ��ʧ��");

	/* �ݴ�I֡����*/
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, &stIFrameData),"��ȡIFrame����ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter),"Step 2: inject start ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"Step 2: player start ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem,3000), "iframe���볬ʱ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, CSUDI_TRUE),"player start ʧ��");	

	CSTCPrint("��ȷ���Ƿ�������ʾ\n");
    CSTCPrint("We expect there's an Iframe on the screen!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 3:ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0,EM_UDIVIDEO_STOPMODE_BLACK), "Step 4:��stop modeʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"Step 4:player stop ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInjecter),"Step 4:inject stop ʧ��");

	CSTCPrint("��ǰ�Ƿ�Ϊ����?\n");
    CSTCPrint("We expect there's no Iframe on the screen !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 5:ʧ��");

	/* reset event */
	while (CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem, 1000));

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hInjecter, EM_UDIINJECTER_IFRAME_PARAMS, &stIFrameData),"Setp 6:��ȡIFrame����ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter),"Setp 6:inject start ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"Setp 6:player start ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem, 1000), "iframe���볬ʱ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, CSUDI_TRUE),"player start ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOSetStopMode(0,EM_UDIVIDEO_STOPMODE_FREEZE), "Step 7 ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"player stop ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInjecter),"inject stop ʧ��");

	CSTCPrint("��ȷ���Ƿ�Ϊ��֡(I֡������ʾ)?\n");
    CSTCPrint("We expect there's an Iframe on the screen!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����8ʧ��");
	
CSTK_FATAL_POINT;
	CSUDIVIDEOShow(0, CSUDI_FALSE);
	CSUDIVIDEOSetStopMode(0, eOldStopMode);
	
	if (hInjecter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hInjecter), "inject close ʧ��");
	}
	//2011.5.17 10:30  ��������ǰɾ���ص��������´�����ͬ�¼��ص�ʧ��(������CSUDIPLAYERClose֮����ã�hPlayer���ÿ�)
	CSUDIPLAYERDelPlayerCallback(hPlayer,ifarme_player_callback,EM_UDIPLAYER_VIDEO_FRAME_COMING,(void*)0);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose(hPlayer), "player close ʧ��");	
	}

	CSUDIOSMutexWait(g_hIframeTestMutex, 100);
        if(g_hNewFrameSem != CSUDI_NULL)
        {
	  	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemDestroy(g_hNewFrameSem),"Sem ɾ��ʧ��");
	 	 g_hNewFrameSem = CSUDI_NULL;
        }
	CSUDIOSMutexRelease(g_hIframeTestMutex);
	
	return CSUDI_TRUE;
}

CSUDI_BOOL DecodeIFrameTest(int nIframeIndex)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hInjecter = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == SetupIFrameInjecter(&hPlayer, &hInjecter, nIframeIndex),"Step 1: Iframeע���ʼ��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hInjecter),"Step 2: inject start ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer),"Step 2: player start ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemWait(g_hNewFrameSem,1000), "iframe���볬ʱ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIVIDEOShow(0, CSUDI_TRUE),"player start ʧ��");	

	CSTCPrint("��ȷ���Ƿ�������ʾ\n");
    CSTCPrint("We expect there's an Iframe on the screen!!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"Step 3:ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer),"Step 4:player stop ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hInjecter),"Step 4:inject stop ʧ��");

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
		//2011.5.17 10:30  ��������ǰɾ���ص��������´�����ͬ�¼��ص�ʧ��(������CSUDIPLAYERClose֮����ã�hPlayer���ÿ�)
		CSUDIPLAYERDelPlayerCallback(hPlayer,ifarme_player_callback,EM_UDIPLAYER_VIDEO_FRAME_COMING,(void*)0);

		CSUDIPLAYERClose(hPlayer);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:���Էֱ���Ϊ480i��I֡����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:480i��I֡����
//@EXPECTATION:����������ʾ
//@EXECUTIONFLOW:1��������ʼ��I֡�����Inject&Player,
//@EXECUTIONFLOW:2��������injecter���÷ֱ���Ϊ480i��I֡����
//@EXECUTIONFLOW:3������I֡���벢����CSUDIVIDEOShow����bIsShowΪTRUE
//@EXECUTIONFLOW:4����ʾ�û�ȷ��I֡������ʾ
//@EXECUTIONFLOW:5��ֹͣI֡���룬���ͷ�Player/Injecter Handle
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_480I(void)
{
	return DecodeIFrameTest(IFRAME_DATA_480I);
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:���Էֱ���Ϊ576i��I֡����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:576i��I֡����
//@EXPECTATION:����������ʾ
//@EXECUTIONFLOW:1��������ʼ��I֡�����Inject&Player,
//@EXECUTIONFLOW:2��������injecter���÷ֱ���Ϊ576i��I֡����
//@EXECUTIONFLOW:3������I֡���벢����CSUDIVIDEOShow����bIsShowΪTRUE
//@EXECUTIONFLOW:4����ʾ�û�ȷ��I֡������ʾ
//@EXECUTIONFLOW:5��ֹͣI֡���룬���ͷ�Player/Injecter Handle
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_576I(void)
{
	return DecodeIFrameTest(IFRAME_DATA_576I);
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:���Էֱ���Ϊ720p��I֡����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:720p��I֡����
//@EXPECTATION:����������ʾ
//@EXECUTIONFLOW:1��������ʼ��I֡�����Inject&Player,
//@EXECUTIONFLOW:2��������injecter���÷ֱ���Ϊ720p��I֡����
//@EXECUTIONFLOW:3������I֡���벢����CSUDIVIDEOShow����bIsShowΪTRUE
//@EXECUTIONFLOW:4����ʾ�û�ȷ��I֡������ʾ
//@EXECUTIONFLOW:5��ֹͣI֡���룬���ͷ�Player/Injecter Handle
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_720P(void)
{
	return DecodeIFrameTest(IFRAME_DATA_720P);
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:���Էֱ���Ϊ1080i��I֡����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:��������Iframeע��ӿ����Ҫ��
//@EXPECTATION:����������ʾ
//@EXECUTIONFLOW:1��������ʼ��I֡�����Inject&Player,
//@EXECUTIONFLOW:2��������injecter���÷ֱ���Ϊ1080i��I֡����
//@EXECUTIONFLOW:3������I֡���벢����CSUDIVIDEOShow����bIsShowΪTRUE
//@EXECUTIONFLOW:4����ʾ�û�ȷ��I֡������ʾ
//@EXECUTIONFLOW:5��ֹͣI֡���룬���ͷ�Player/Injecter Handle
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_1080I(void)
{
	return DecodeIFrameTest(IFRAME_DATA_1080I);
}

