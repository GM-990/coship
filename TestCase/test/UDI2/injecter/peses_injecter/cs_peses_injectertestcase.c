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
    EM_INJECT_PESES_VIDEO_MPEG2 = 0, 	/*��������*/
    EM_INJECT_PESES_VIDEO_H264, 		/**/
    EM_INJECT_PESES_AUDIO_MPEG2,		/**/
    EM_INJECT_PESES_AUDIO_AAC,    		/**/
    EM_INJECT_PESES_MAX_COUNT
}ES_ServiceIndex_E;

typedef struct _ESStreamInfo_S
{
    char * m_pcName; //ע���ļ�����
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
					for (j=0; j<eAudStreamTypeNum; j++)                                //�����������������棺֧�ֽ����������֧��ֱ�����������
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


///���ݴ����index������Ҫ�����������͵���Ƶ������
//�������������ʾ���Ӧ����Ƶ����Ƶ����ö�����ȡֵ
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

static void ES_injectTask(void * ESStreamInfo)
{
	void * ppvBuffer = NULL;

	unsigned  int  uLength = 0;
	
	ESStreamInfo_S *pESStreamInfo = (ESStreamInfo_S*)ESStreamInfo;

	CSUDI_HANDLE hInjecter = pESStreamInfo->m_hInjecter;
	
	int nBlockNum = 0;
	long nFileAllLength = 0;  //�ļ��ܳ���
	int nFileCurrentPos = 0; //�ļ���ǰλ��
	int nFileLeftLength = 0; //ʣ��δ���ļ�����
	int nInjectedLength = 0; //��ע��Ĵ�С
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
		//�����ǰʣ��δ���ļ��ĳ���
		nFileCurrentPos=InjectFSTell(file);	//��ǰ�Ѷ��ļ��ĳ���
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //ʣ��δ���ļ��ĳ���

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
				
		if(uLength != 0)
		{
			nReadLength = pESStreamInfo->m_uBufferLen < nFileLeftLength ? pESStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //ʣ���ļ�����С�ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = nReadLength;
			}
			else  //ʣ���ļ����ȴ��ڵ��ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)pBuffer, nInjectedLength); //��ȡ��Ӧ���ȵ��ļ�
									
			memcpy(ppvBuffer, pBuffer, nInjectedLength); //��ע�뻺�����п�������

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
	long nFileAllLength = 0;  //�ļ��ܳ���
	int nFileCurrentPos = 0; //�ļ���ǰλ��
	int nFileLeftLength = 0; //ʣ��δ���ļ�����
	int nInjectedLength = 0; //��ע��Ĵ�С
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
		//�����ǰʣ��δ���ļ��ĳ���
		nFileCurrentPos=InjectFSTell(file);	//��ǰ�Ѷ��ļ��ĳ���
		
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //ʣ��δ���ļ��ĳ���

              uLength = 0;
		
		CSUDIINJECTERGetFreeBuffer (hInjecter, &ppvBuffer,&uLength);
				
		if(uLength != 0)
		{
			nReadLength = pESStreamInfo->m_uBufferLen < nFileLeftLength ? pESStreamInfo->m_uBufferLen:nFileLeftLength;

			if(nReadLength < uLength) //ʣ���ļ�����С�ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = nReadLength;
			}
			else  //ʣ���ļ����ȴ��ڵ��ڿ�ע��ռ�ĳ���
			{
				nInjectedLength = uLength;
			}
		
			nBlockNum= InjectFSRead(file,(void*)pBuffer, nInjectedLength); //��ȡ��Ӧ���ȵ��ļ�
									
			memcpy(ppvBuffer, pBuffer, nInjectedLength); //��ע�뻺�����п�������

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

//ע�����Ƶ������
	psInjecterChnl->m_nAudioDecoder=-1;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;	
	psInjecterChnl->m_nDemux=-1;
		
	pOpenParam->m_eInjecterType= EM_INJECTER_ES;
	
	if (pESStreamInfo->m_ESContentInfo.m_eContentType == EM_UDI_CONTENT_VIDEO)
	{
		nVideoId = searchVideoID_Y_InInject(pESStreamInfo->m_ESContentInfo.m_uStreamType.m_eVideoType);
		if(nVideoId ==-1)
		{
			CSTCPrint( "û��֧��ָ��������Ƶ������!\n\r");
		}
		psInjecterChnl->m_nVideoDecoder = nVideoId;
		
		pOpenParam->m_eContentType = EM_UDIINJECTER_CONTENT_VIDEO;
	}
	else if (pESStreamInfo->m_ESContentInfo.m_eContentType == EM_UDI_CONTENT_AUDIO)
	{
		nAudioId = searchAudioID_Y_InInject(pESStreamInfo->m_ESContentInfo.m_uStreamType.m_eAudioType);
		if(nAudioId ==-1)
		{
			CSTCPrint( "û��֧��ָ��������Ƶ������!\n\r");
		}
		psInjecterChnl->m_nAudioDecoder = nAudioId;
		pOpenParam->m_eContentType = EM_UDIINJECTER_CONTENT_AUDIO;

	}

//���ý�Ŀ��Ϣ
       memcpy(psStreamInfo,&(pESStreamInfo->m_ESContentInfo),sizeof(CSUDIStreamInfo_S));

//����ͨ��
	psPlayerChnl->m_nDemux=psInjecterChnl->m_nDemux;
	psPlayerChnl->m_nAudioDecoder=psInjecterChnl->m_nAudioDecoder;
	psPlayerChnl->m_nVideoDecoder=psInjecterChnl->m_nVideoDecoder;
	
	return;
}

static CSUDI_BOOL closeInjectPlayer(CSUDI_HANDLE hPlayer)
{
	CSUDI_BOOL bRe = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL !=hPlayer,"��������");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "ֹͣ����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "�رղ�����ʧ��");
	
	bRe = CSUDI_TRUE;
	
	CSTK_FATAL_POINT;
	
	return bRe;
}

static CSUDI_HANDLE OpenESInectPlayer(CSUDIPlayerChnl_S *pChnl, CSUDIStreamInfo_S *pStreamInfo)
{
	CSUDI_HANDLE hPlayer= CSUDI_NULL;
	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pChnl, "����1����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != pStreamInfo, "����2����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (pChnl, EM_UDIPLAYER_INJECT,&hPlayer), "�򿪲�����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,pStreamInfo,1,CSUDI_NULL), "������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "��������ʧ��");

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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
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
	//�ڱ�����������ִ��ǰ����
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PESES_INJECTER_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:����Open/Close PES/ESע����ʱ���ڲ������������Ҫ������µ�ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:����������PES/ESע�����ӿ����Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2��psOpenParams->m_eContentType=EM_UDIINJECTER_CONTENT_DEFAULT,����CSUDIINJECTEROpen��ͼ��һ��PES/ESע����,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��psInjecterChnl->m_nDemux ������-1ʱ������CSUDIINJECTEROpen��ͼ��һ��PES/ESע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��psInjecterChnl->m_nVideoDecoder ����ʱʱ������CSUDIINJECTEROpen��ͼ��һ��PES/ESע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��psInjecterChnl->m_nAudioDecoder ����ʱʱ������CSUDIINJECTEROpen��ͼ��һ��PES/ESע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��phINJECTER = CSUDI_NULL ʱ������CSUDIINJECTEROpen��ͼ��һ��PES/ESע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7��psInjecterChnl = CSUDI_NULLʱ������CSUDIINJECTEROpen��ͼ��һ��PES/ESע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8��psOpenParams = CSUDI_NULLʱ������CSUDIINJECTEROpen��ͼ��һ��PES/ESע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��hINJECTER = CSUDI_NULLʱ������CSUDIINJECTERClose��ͼ�ر�һ����Ч��PES/ESע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����2 ������7����100��
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
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
	for (i=0;i<REPEAT_TIMES; i++)
	{
		e_ContentType = sOpenParams.m_eContentType;
		
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_DEFAULT;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

		sOpenParams.m_eContentType = e_ContentType;
		
		sInjecterChnl.m_nDemux=ERRORDATA;		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����3ʧ��");
		sInjecterChnl.m_nDemux=-1;
		
		nDecoderID = sInjecterChnl.m_nVideoDecoder;
		sInjecterChnl.m_nVideoDecoder = ERRORDATA;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_DEVICE_ID == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����4ʧ��");
		sInjecterChnl.m_nVideoDecoder = nDecoderID;

		nDecoderID = sInjecterChnl.m_nAudioDecoder;
		sInjecterChnl.m_nAudioDecoder = ERRORDATA;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����5ʧ��");
		sInjecterChnl.m_nAudioDecoder = nDecoderID;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,CSUDI_NULL), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (CSUDI_NULL,&sOpenParams,&hINJECTER), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,CSUDI_NULL,&hINJECTER), "����8ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(CSUDI_NULL), "����9ʧ��");

	}

	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:�������в��������Ͻӿ����Ҫ�������£�CSUDIINJECTEROpen & CSUDIINJECTERClose��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:���в�����Ӧ�ó��������Ͻӿ����Ҫ��
//@EXPECTATION:����CSUDI_SUCCESS  
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERClose����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2�Ͳ���3��1000�Ρ�
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
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
		
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����3ʧ��");
	}
	CSTK_FATAL_POINT;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:����ES ע����Open��Close ���ɶԵ��ã����Close��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:���������Ͻӿ����Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS  
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen��������һ��PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTEROpen��������һ��PES/ESע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERClose����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERClose���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����2������5����1000��
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
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "����2ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS !=  CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER1)), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(hINJECTER), "����5ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:���߳̽������CSUDIINJECTEROpen & CSUDIINJECTERClose�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �Ϸ���
//@EXPECTATION:�����������������߼���ȷ
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2����������һ�����񣬸�����ͬʱ���첽ִ������Ĳ���3��4
//@EXECUTIONFLOW:3������CSUDIINJECTEROpen��������һ��PES/ESע��������������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:4���������3�ɹ��������CSUDIINJECTERClose����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����3������4����1000��
//@EXECUTIONFLOW:6���ȴ�����2������������������˳�
//@EXECUTIONFLOW:7�����ٲ���2����������
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
	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OpenClose", INJECTTHREADPRO, INJECTTHREADBUF, task_For_ESOpenColse, (void *)EM_INJECT_PESES_VIDEO_MPEG2, &hInjectThread), "������ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		udiRe = CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_DEVICE_BUSY == udiRe )
			|| (CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED == udiRe)),"����3ʧ��");

		CSUDIOSThreadSleep(2);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����4ʧ��");
		}
	}
	
	CSTK_FATAL_POINT;
	
	if (hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����6ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����7ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:����ESע���������ΪCSUDI_NULLʱ��Start/Stop������ִ��Ч��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER = CSUDI_NULL
//@EXPECTATION:Start/Stop����������CSUDIINJECTER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2��������PES/ESע�������õ�PES/ESע�������
//@EXECUTIONFLOW:3����hINJECTER = CSUDI_NULL,����CSUDIINJECTERStart,��������CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:4����hINJECTER = CSUDI_NULL,����CSUDIINJECTERStop,��������CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:5���ظ�����3������4����1000��
//@EXECUTIONFLOW:6������CSUDIINJECTERClose���رմ�PES/ESע����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStart(CSUDI_NULL), "����3ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStop(CSUDI_NULL), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����6ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:������������£�CSUDIINJECTERStart��CSUDIINJECTERStop�ɶԵ��õ����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ�����򿪵�ESע���������
//@EXPECTATION:CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2��������PES/ESע�������õ�PES/ESע�������
//@EXECUTIONFLOW:3������CSUDIINJECTERStart,������PES/ESע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:4������CSUDIINJECTERStop,ֹͣ��PES/ESע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:5���ظ�����3������4����1000��
//@EXECUTIONFLOW:6������CSUDIINJECTERClose���رմ�PES/ESע����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����4ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����6ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:������������£�CSUDIINJECTERStart��CSUDIINJECTERStop���ɶԻ�������õ����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ�����򿪵�ESע���������
//@EXPECTATION:���ط�CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2��������PES/ESע�������õ�PES/ESע�������
//@EXECUTIONFLOW:3������CSUDIINJECTERStop,ֹͣ��PES/ESע�������������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:4������CSUDIINJECTERStart,������PES/ESע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:5������CSUDIINJECTERStart,������PES/ESע�������������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:6������CSUDIINJECTERStop,ֹͣ��PES/ESע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:7������CSUDIINJECTERStop,ֹͣ��PES/ESע�������������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:8���ظ�����3������7����1000��
//@EXECUTIONFLOW:9������CSUDIINJECTERClose���رմ�PES/ESע����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStart(hINJECTER), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "����7ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����9ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:���߳̽������CSUDIINJECTERStart & CSUDIINJECTERStop�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �Ϸ���ESע����
//@EXPECTATION:�����������������߼���ȷ
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen��������һ��PES/ESע�������õ�ע�������
//@EXECUTIONFLOW:3����������һ�����񣬸�����ͬʱ���첽ִ������Ĳ���4��5
//@EXECUTIONFLOW:4������CSUDIINJECTERStart��������PES/ESע��������������CSUDI_SUCCESS��CSUDIREC_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:5������CSUDIINJECTERStop��ֹͣ��PES/ESע��������������CSUDI_SUCCESS��CSUDIREC_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:6���ظ�����6������7����1000��
//@EXECUTIONFLOW:7������CSUDIOSThreadJoin���ȴ�����5����������ִ����ɣ��ر��˳�
//@EXECUTIONFLOW:8������CSUDIINJECTERClose���رմ�PES/ESע����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ESStartStop", INJECTTHREADPRO, INJECTTHREADBUF, task_For_ESStartStop, (void *)hINJECTER, &hInjectThread), "����3ʧ��");
	
	for (i=0;i<REPEAT_TIMES; i++)
	{	
		udiRe = CSUDIINJECTERStart(hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STARTED == udiRe )),"����4ʧ��");

		CSUDIOSThreadSleep(2);

		udiRe =  CSUDIINJECTERStop(hINJECTER);
		
		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STOPPED == udiRe )),"����5ʧ��");
	}
    	
	CSTK_FATAL_POINT;
	
	if (hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����7ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����8ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����9ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:������ESע��ģʽ���ӿڲ������������Ҫ�������£�CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute��ִ��Ч��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1�������Ͻӿ����Ҫ�� 
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3��hINJECTER = CSUDI_NULLʱ������CSUDIINJECTERSetAttribute�������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERSetAttribute����ͬ�����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERSetAttribute ���ûص��������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERSetAttribute ����ֻ��״̬���ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERSetAttribute ����ֻ�����������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERSetAttribute ���÷Ƿ����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��hINJECTER = CSUDI_NULLʱ������CSUDIINJECTERGetAttribute��ȡ���ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:11��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute ��ȡͬ�����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute��ȡ�ص����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute ��ȡ״̬���ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:13��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute ��ȡ���������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERSetAttribute ��ȡ�Ƿ����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:15���ظ�����3~����14��1000��
//@EXECUTIONFLOW:16������CSUDIINJECTERClose���رմ�PES/ESע����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
//����		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,CSUDI_NULL),  "����4ʧ��");
				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_STATUS, &sStatus),  "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sBuffer),  "����7ʧ��");

//��ȡ	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,CSUDI_NULL),  "����10ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,CSUDI_NULL),  "����12ʧ��");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,CSUDI_NULL),  "����13ʧ��");		
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:������ESע��ģʽ���ӿڲ������������Ҫ�������£�CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute��ִ��Ч��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1�������Ͻӿ����Ҫ�� 
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����ͬ����ʽ���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ���ûص��������ԣ���������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5������CSUDIINJECTERGetAttribute��ȡͬ����ʽ���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���жϻ�ȡ�õ���ͬ����ʽ����Ϊ����3�����õ�����һ��
//@EXECUTIONFLOW:7������CSUDIINJECTERGetAttribute��ȡ���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERGetAttribute��ȡ״̬���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIINJECTERGetAttribute��ȡ�ص��������ԣ���������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:10�������һ�����سɹ������жϻ�ȡ�õ��Ļص���������Ϊ����4�����õ�����һ��
//@EXECUTIONFLOW:11���ظ�����3~����10��1000��
//@EXECUTIONFLOW:12������CSUDIINJECTERClose���رմ�PES/ESע����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	sSyncMode.m_eSyncMode =  EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
//����			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode),  "����3ʧ��");				
//��ȡ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode),  "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(((sSyncMode.m_eSyncMode == EM_UDIINJECTER_SYNC_NONE)
			||(sSyncMode.m_hSyncHandle == CSUDI_NULL)),"����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sBuffer),  "����7ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,&sStatus),  "����8ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����12ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:����PES/ESע������start֮���ٵ���CSUDIINJECTERSetAttribute/CSUDIINJECTERGetAttribute�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �ǿ�
//@INPUT:2��eAttrType=EM_UDIINJECTER_PESES_ENDIAN  
//@INPUT:3��pvData�ǿ�
//@EXPECTATION:����CSUDIINJECTERSetAttribute    ����CSUDIINJECTER_ERROR_INVALID_STATUS������CSUDIINJECTERGetAttribute����CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK:����ע����ܵ���CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����ͬ�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERStart������PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:6������CSUDIINJECTERSetAttribute����"�ص�����"���ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERGetAttribute��ȡ"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:8���жϻ�ȡ�õ���ͬ������Ϊ����3�����õ�����һ��
//@EXECUTIONFLOW:9������CSUDIINJECTERGetAttribute��ȡ"״̬"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERGetAttribute��ȡ"������"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:11������CSUDIINJECTERGetAttribute��ȡ"�ص�����"���ԣ���������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:12���ظ�����6~����11��1000��
//@EXECUTIONFLOW:13������CSUDIINJECTERStop��ֹͣ��PES/ESע����
//@EXECUTIONFLOW:14������CSUDIINJECTERClose���رմ�PES/ESע����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	sSyncMode.m_eSyncMode =  EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	for (i=0; i<REPEAT_TIMES; i++)
	{
//����
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����5ʧ��");			

//��ȡ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����7ʧ��");

		CSTK_ASSERT_TRUE_FATAL(((sSyncMode.m_eSyncMode == EM_UDIINJECTER_SYNC_NONE)
			||(sSyncMode.m_hSyncHandle == CSUDI_NULL)),"����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_STATUS,&sStatus), "����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_BUFFER,&sStatus), "����10ʧ��");
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����13ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush 
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERFlush��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��PES/ESע�������
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL��2) Startע����֮ǰ,3) Stopע����֮�����������µ��ô˺��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��������ע��֮ǰ����CSUDIINJECTERFlush,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7����hINJECTER == CSUDI_NULL������£�����CSUDIINJECTERFlush���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERStopֹͣ��PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9����hINJECTER Ϊ����2�õ���PES/ESע�������ʱ������CSUDIINJECTERFlush���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����6~����9��1000��
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���ر�ע����
//@EXECUTIONFLOW:12���رղ���2�򿪵Ĳ�����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "����4ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����5ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(CSUDI_NULL), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "����9ʧ��");		
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush                
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERFlush��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��PES/ESע�������
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERStart������PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERGetFreeBuffer�������õ�ע�������л������ĵ�ַ�ʹ�С��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERWriteComplete��֪ͨ�ײ�ע��������׼���ã�������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERFlush���ȴ�����ע������ݲ����꣬������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����6~����8��5��
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��PES/ESע����
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���رմ�PES/ESע����
//@EXECUTIONFLOW:12���رղ���2�򿪵Ĳ�����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	for (i=0;i<5; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer(hINJECTER, &pvBuffer, &uLength), "����6ʧ��");	

		if (uLength > 0)
		{
			memset(pvBuffer,0x0,uLength);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERWriteComplete (hINJECTER,uLength), "����7ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER), "����8ʧ��");
		}
		else
		{
			CSUDIOSThreadSleep(10);
		}
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����10ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERClear 
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERClear��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��PES/ESע�������
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL��2) Startע����֮ǰ,3) Stopע����֮�����������µ��ô˺��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��������ע��֮ǰ����CSUDIINJECTERFlush,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7����hINJECTER == CSUDI_NULL������£�����CSUDIINJECTERClear���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERStopֹͣ��PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9����hINJECTER Ϊ����2�õ���PES/ESע�������ʱ������CSUDIINJECTERFlush���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����6~����9��1000��
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���ر�ע����
//@EXECUTIONFLOW:12���رղ���2�򿪵Ĳ�����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "����4ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����5ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(CSUDI_NULL), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "����9ʧ��");		
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush                
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERFlush��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��PES/ESע�������
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERStart������PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERGetFreeBuffer�������õ�ע�������л������ĵ�ַ�ʹ�С��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERWriteComplete��֪ͨ�ײ�ע��������׼���ã�������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERFlush���ȴ�����ע������ݲ����꣬������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����6~����8��1000��
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��PES/ESע����
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���رմ�PES/ESע����
//@EXECUTIONFLOW:12���رղ���2�򿪵Ĳ�����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");	

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	for (i=0;i<REPEAT_TIMES; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer(hINJECTER, &pvBuffer, &uLength), "����6ʧ��");	

		if (uLength > 0)
		{
			memset(pvBuffer,0x0,uLength);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERWriteComplete (hINJECTER,uLength), "����7ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER), "����8ʧ��");
		}
		else
		{
			CSUDIOSThreadSleep(10);
		}
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����10ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����11ʧ��");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERGetFreeBuffer
//@DESCRIPTION:���Խӿڲ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERGetFreeBuffer��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��PES/ESע�������
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:1) Start֮ǰ��2)Stop֮��3)ע����ΪCSUDI_NULL��4) ����������ָ��ΪCSUDI_NULL��5)���������Ȳ���ΪCSUDI_NULL������5������£�������Ӧ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5�����в���������������£�����CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7��hINJECTERΪCSUDI_NULL������CSUDIINJECTERGetFreeBuffer����������CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8��ppvBufferΪCSUDI_NULL������CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��puLengthΪCSUDI_NULL������CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��PES/ESע����
//@EXECUTIONFLOW:11�����в���������������£�����CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12���ظ�����6~����10��1000��
//@EXECUTIONFLOW:13������CSUDIINJECTERClose���رմ�PES/ESע����
//@EXECUTIONFLOW:14���رղ���2�򿪵Ĳ�����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "����5ʧ��");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERGetFreeBuffer (CSUDI_NULL,&pvBuffer,&uLength), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,CSUDI_NULL), "����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer(hINJECTER, CSUDI_NULL, &uLength), "����9ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "����11ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����13ʧ��");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERWriteComplete
//@DESCRIPTION:���Խӿڲ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERWriteComplete��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��PES/ESע�������
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:1) Start֮ǰ��2)Stop֮��3)ע����ΪCSUDI_NULL��4) uWrittenSize��ǰ�ڵõ��Ŀ��г��ȴ�����4������£�������Ӧ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERWriteComplete��ͼд��1�ֽ����ݣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������PES/ESע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERGetFreeBuffer�õ����л�������ַ�ͳ���
//@EXECUTIONFLOW:8��hINJECTERΪCSUDI_NULL��uWrittenSizeΪ����7�õ��Ŀ��л��������ȣ�����CSUDIINJECTERWriteComplete����������CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:9��hINJECTERΪ����2�õ��ľ����uWrittenSizeΪ����7�õ��Ŀ��л��������ȼ�1������CSUDIINJECTERWriteComplete���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��PES/ESע����
//@EXECUTIONFLOW:11��hINJECTERΪ����2�õ��ľ����uWrittenSizeΪ����7�õ��Ŀ��л��������ȣ�����CSUDIINJECTERWriteComplete���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12���ظ�����6~����11��1000��
//@EXECUTIONFLOW:13������CSUDIINJECTERClose���رմ�PES/ESע����
//@EXECUTIONFLOW:14���رղ���2�򿪵Ĳ�����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����3ʧ��");
	
	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER,1), "����5ʧ��");

	for (i=0; i<REPEAT_TIMES; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERWriteComplete (CSUDI_NULL,uLength), "����8ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER, uLength+1), "����9ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER, uLength), "����11ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����13ʧ��");

	if (hPlayer != CSUDI_NULL)
	{
		closeInjectPlayer(hPlayer);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:1��CSUDIINJECTERWriteComplete     
//@CASEGROUP:2��CSUDIINJECTERGetFreeBuffer   
//@DESCRIPTION:����·��CSUDIINJECTERGetFreeBuffer-->ֹͣPlayer-->���»���������-->CSUDIINJECTERWriteComplete���������������״̬
//@PRECONDITION:INJECTERģ�鱻������ʼ����ES Player����������
//@INPUT:1�����в���������
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PES/ESע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����"ͬ��"���ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4�����ݲ���1��ȡ����Դ��������һ�����������õ����������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart��������PES/ESע����
//@EXECUTIONFLOW:6������CSUDIINJECTERGetFreeBuffer ��ʹppvBuffer  �ǿգ�puLength �ǿ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStop��ֹͣ������
//@EXECUTIONFLOW:8���޸�ppvBuffer�е�����
//@EXECUTIONFLOW:9������CSUDIINJECTERWriteComplete ,д�����ݣ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERClose���رղ�����
//@EXECUTIONFLOW:11������CSUDIINJECTERStop��ֹͣ��ע����
//@EXECUTIONFLOW:12������CSUDIINJECTERClose���رմ�ע����
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	hPlayer = OpenESInectPlayer(&sPlayerChnl,&sStreamInfo);
	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength),"����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(uLength > 0,"����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����8ʧ��");			
	
	memset(pvBuffer,0xFE,uLength);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  != CSUDIINJECTERWriteComplete(hINJECTER, uLength),"����9ʧ��");
	 
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����10ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����11ʧ��");		

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����12ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_VIDEO_MPEG2 PES/ES�������������ŵ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��MPEG2��Ƶ
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�����Ƶ��Ŀ���ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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
	
	CSTCPrint("����ע��EM_INJECT_PESES_VIDEO_MPEG2 �������������ŵ����");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSTCPrint("����ע��EM_INJECT_PESES_VIDEO_MPEG2 �������������ŵ����");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����12ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����13ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_VIDEO_MPEG2 PES/ES���������в��š���ͣ�ͻָ���ͣ
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��ES��Ƶ���ܿ���ES���ŵ��������š���ͣ��ָ�����
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10������CSUDIPLAYERPause ����ͣES����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ���Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:12������CSUDIPLAYERResume ���ָ�ES����
//@EXECUTIONFLOW:13���ظ�����9-����12��5��
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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

	CSTCPrint("����ע��EM_INJECT_PESES_VIDEO_MPEG2  ���������в��š���ͣ�ͻָ���ͣ");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("��Ƶ���ų�������\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����10ʧ��");
		
		CSTCPrint("��Ƶֹͣ��������\n");
		CSTCPrint("Does it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����12ʧ��");
		
	}
	
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����15ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����16ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����17ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����18ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����19ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_VIDEO_MPEG2 PES/ES���������в��š�ֹͣ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��ES��Ƶ���ܿ��Ʋ����е�ֹͣ��ָ�
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10������CSUDIPLAYERStop ��ֹͣES����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ���Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:12������CSUDIPLAYERStart ���ָ�ES����
//@EXECUTIONFLOW:13���ظ�����9-����12��5��
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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
	
	CSTCPrint("����ע��EM_INJECT_PESES_VIDEO_MPEG2   ���������в��š�ֹͣ����");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("��Ƶ���ų�������\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");
		
		CSTCPrint("��Ƶֹͣ��������\n");
		CSTCPrint("Does it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����12ʧ��");
		
	}
	
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����15ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����16ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����17ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����18ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����19ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_VIDEO_MPEG2 PES/ES����������ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����ָ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  CSUDIINJECTERFlush�ȴ���ע������ݲ�����ȫ���ܷ���
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIINJECTERFlush ���ȴ���ע�������ȫ�������꣬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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
	
	CSTCPrint("����ע��EM_INJECT_PESES_VIDEO_MPEG2 ����������ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��ƵƵ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"����11ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:"����ע��EM_INJECT_PESES_VIDEO_MPEG2 ����������ע�����������CSUDIINJECTERClear�����ע�뵫δ�����������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  CSUDIINJECTERClear����������
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIINJECTERClear �������ע������ݣ�������Ƶ����ֹͣ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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
	
	CSTCPrint("����ע��EM_INJECT_PESES_VIDEO_MPEG2 ����������ע�����������CSUDIINJECTERClear�����ע�뵫δ�����������");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��ƵƵ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"����11ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_VIDEO_H264 PES/ES�������������ŵ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��H264��Ƶ
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�����Ƶ��Ŀ���ų���?//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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
	
	CSTCPrint("����ע��EM_INJECT_PESES_VIDEO_H264 �������������ŵ����");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����12ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����13ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_AUDIO_MPEG2 PES/ES�������������ŵ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��MPEG2��Ƶ
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�����Ƶ��Ŀ���ų���?//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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

	CSTCPrint("����������:ע��EM_INJECT_PESES_AUDIO_MPEG2 �������������ŵ����");

	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����12ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����13ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_AUDIO_AAC PES/ES�������������ŵ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��AAC��Ƶ
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�����Ƶ��Ŀ���ų���?//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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
	
	CSTCPrint("����ע��EM_INJECT_PESES_AUDIO_AAC �������������ŵ����");

	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����12ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����13ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_AUDIO_MPEG2 PES/ES���������в��š���ͣ�ͻָ���ͣ
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��EM_INJECT_PESES_AUDIO_MPEG2�����ܿ����������š���ͣ��ָ�����
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���ƵƵ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10������CSUDIPLAYERPause ����ͣES����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ���Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:12������CSUDIPLAYERResume ���ָ�ES����
//@EXECUTIONFLOW:13���ظ�����9-����12��5��
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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
	
	CSTCPrint("����ע��EM_INJECT_PESES_AUDIO_MPEG2 ���������в��š���ͣ�ͻָ���ͣ");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("��Ƶ���ų�������\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����10ʧ��");
		
		CSTCPrint("��Ƶֹͣ��������\n");
		CSTCPrint("Does it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����12ʧ��");
		
	}
	
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����15ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����16ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����17ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����18ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����19ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_AUDIO_MPEG2 PES/ES���������в��š�ֹͣ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ����Ƶ�����ܿ��Ʋ����е�ֹͣ��ָ�
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10������CSUDIPLAYERStop ��ֹͣES����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ���Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:12������CSUDIPLAYERStart ���ָ�ES����
//@EXECUTIONFLOW:13���ظ�����9-����12��5��
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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

	CSTCPrint("����ע��EM_INJECT_PESES_AUDIO_MPEG2 ���������в��š�ֹͣ����");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	for (i=0;i<5;i++)
	{
		CSTCPrint("��Ƶ���ų�������\n");
		CSTCPrint("Do we get A/V output normally ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");
		
		CSTCPrint("��Ƶֹͣ��������\n");
		CSTCPrint("Does it stop playing ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����12ʧ��");
		
	}
	
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����14ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����15ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����16ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����17ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����18ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����19ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_AUDIO_MPEG2 PES/ES����������ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����ָ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  CSUDIINJECTERFlush�ȴ���ע������ݲ�����ȫ���ܷ���
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIINJECTERFlush ���ȴ���ע�������ȫ�������꣬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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
	
	CSTCPrint("����ע��EM_INJECT_PESES_AUDIO_MPEG2 ����������ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����");
	
	getESInjectInfo(eServiceStream,&sInjecterChnl,&sPlayerChnl,&sOpenParams,&sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"����11ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_AUDIO_MPEG2 ����������ע�����������CSUDIINJECTERClear�����ע������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  CSUDIINJECTERClear����������
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIINJECTERClear �������ע������ݣ�������Ƶ����ֹͣ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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
	
	CSTCPrint("����ע��EM_INJECT_PESES_AUDIO_MPEG2 ����������ע�����������CSUDIINJECTERClear�����ע������");

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Do we get A/V output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"����11ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����13ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_VIDEO_MPEG2 PES/ES���������������򿪵�����Ƶ�����������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  ���ܲ��ų�ָ��MPEG2��Ƶ
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ��Ŀ���ܲ��ų���?
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");

	sStreamInfo.m_eContentType = EM_UDI_CONTENT_AUDIO;	
	sStreamInfo.m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_MPEG2;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSTCPrint("����ע��EM_INJECT_PESES_VIDEO_MPEG2 ���������������򿪵�����Ƶ�����������");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��ȷ����Ƶ��Ŀ���ܲ��ų���\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����12ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����13ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��EM_INJECT_PESES_AUDIO_MPEG2 PES/ES���������������򿪵�����Ƶ�����������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  ���ܲ��ų�ָ����Ƶ
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ��Ŀ���ݲ��ܲ��ų���?
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");

	sStreamInfo.m_eContentType = EM_UDI_CONTENT_VIDEO;	
	sStreamInfo.m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSTCPrint("����ע��EM_INJECT_PESES_AUDIO_MPEG2 ���������������򿪵�����Ƶ�����������");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��ȷ����Ƶ��Ŀ���ݲ��ܲ��ų���\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����12ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����13ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע������ݴ���ͨ��CSUDIINJECTERGetFreeBuffer�õ��Ļ�������С
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PES/ES������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  ���ܲ��ų�ָ��MPEG2��Ƶ
//@EXECUTIONFLOW:1������ָ����PES/ES��������ȡ��������������Ľ�����Դ����
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��ES  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ������"ͬ��"����
//@EXECUTIONFLOW:4������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������һ���߳�,ѭ����ȡES   ���ļ���������ESע�����
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream �����ú�ES������Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ���Ƶ��Ŀ���ݲ����ų���?
//@EXECUTIONFLOW:10��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
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

	CSTK_ASSERT_TRUE_FATAL(((sInjecterChnl.m_nAudioDecoder != -1) || (sInjecterChnl.m_nVideoDecoder != -1)),"û��ָ���Ľ�����");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	sSyncMode.m_eSyncMode = EM_UDIINJECTER_SYNC_NONE;
	sSyncMode.m_hSyncHandle = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PES_SYNC,&sSyncMode), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	g_ESStreamInfo[eServiceStream].m_hInjecter = hINJECTER;
	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("ES Inject", INJECTTHREADPRO, INJECTTHREADBUF, ES_injectTask_OverLengh, ( void* ) &g_ESStreamInfo[eServiceStream], &hInjectThread), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "����6ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,&sStreamInfo,1,CSUDI_NULL), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����8ʧ��");

	CSTCPrint("����ע��EM_INJECT_PESES_VIDEO_MPEG2 ������ע������ݴ���ͨ��CSUDIINJECTERGetFreeBuffer�õ��Ļ�������С");
	
	CSUDIOSThreadSleep(2000);
	
	CSTCPrint("��Ƶ��Ŀ���ݲ��ܲ��ų�����\n");
	CSTCPrint("We expect there's no A/V output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	g_ESStreamInfo[eServiceStream].m_bRun = CSUDI_FALSE; //ֹͣע��

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����12ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����13ʧ��");	
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����14ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");

	return CSUDI_TRUE;
}

