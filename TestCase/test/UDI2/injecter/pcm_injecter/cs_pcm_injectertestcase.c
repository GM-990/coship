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
#define PCM_BUFFER_SIZE (1*1024*1024) //����ÿ��ע��Ĵ�С

static CSUDI_HANDLE g_hPcmHandle = (CSUDI_HANDLE)NULL;
static CSUDI_HANDLE g_hInjectThread = (CSUDI_HANDLE)NULL;
static CSUDI_BOOL g_bEnterTask = CSUDI_FALSE;

typedef enum 
{
    EM_INJECT_TS = 0, /*��������*/
    EM_INJECT_PCM_BIG, /*pcm�ļ�������ֽ���*/
    EM_INJECT_PCM_LIT  /*pcm�ļ���С���ֽ���*/
}INJECTServiceIndex_E;

typedef struct PCMPlayInfo_S
{
    char  m_pcName[64]; //ע���ļ�����
    CSUDIINJECTERPcmStartParam_S m_StartParam;//PCM���Ų���,ts��ʱ����Ϊ0
    CSUDIINJECTERPcmEndian_E m_enmEndian;//�ļ����ݵĴ�С���ֽ������� ,tsʱ����Ϊ0   
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
#endif

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
		CSTCPrint("���������ļ� %s ��ʧ�ܣ���ȷ��testdataĿ¼�´��ڸ��ļ�!!!\n", pFileName);
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

// �ñ���ΪCSUDI_TRUEʱ����ע���굱ǰ�ļ��е�pcm����ʱ���˳�ע���߳�pcm_injectTask
static CSUDI_BOOL g_bInjectOnlyOnce = CSUDI_FALSE;

static void pcm_injectTask(void * PCMPlayInfo)
{
	void * ppvBuffer = NULL;
	unsigned  int  uLength = 0;
	PCMPlayInfo_S *pPCMPlayInfo = (PCMPlayInfo_S*)PCMPlayInfo;
	CSUDI_HANDLE hPcm = g_hPcmHandle;

	int nBlockNum = 0;
	long nFileAllLength = 0;  //�ļ��ܳ���
	int nFileCurrentPos = 0; //�ļ���ǰλ��
	int nFileLeftLength = 0; //ʣ��δ���ļ�����
	int nInjectedLength = 0; //��ע��Ĵ�С
	int nReadLength = 0;

	void* file = NULL;
	getInjecterFile((const char*)pPCMPlayInfo->m_pcName, &file, &nFileAllLength);

	if(file==NULL || nFileAllLength <= 0)
	{
		return;
	}
	
	while(g_bEnterTask)
	{
		//�����ǰʣ��δ���ļ��ĳ���
		nFileCurrentPos=InjectFSTell(file);	//��ǰ�Ѷ��ļ��ĳ���
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //ʣ��δ���ļ��ĳ���

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

	long nFileAllLength = 0;  //�ļ��ܳ���
	int nFileCurrentPos = 0; //�ļ���ǰλ��
	int nFileLeftLength = 0; //ʣ��δ���ļ�����
	void *file;
	
	getInjecterFile((const char*)pPCMPlayInfo->m_pcName, &file, &nFileAllLength);
    if(file==NULL || nFileAllLength <= 0)
    {
        return;
    }

	while(g_bEnterTask)
	{
		//�����ǰʣ��δ���ļ��ĳ���
		nFileCurrentPos=InjectFSTell(file);	//��ǰ�Ѷ��ļ��ĳ���
		nFileLeftLength=nFileAllLength-nFileCurrentPos; //ʣ��δ���ļ��ĳ���
		
        uLength = 0;
			  
		CSUDIINJECTERGetFreeBuffer (hPcm,&ppvBuffer,&uLength);
		
		if(uLength > 0)
		{
			InjectFSRead(file, (void*)ppvBuffer, uLength); //��ȡ��Ӧ���ȵ��ļ�
			
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
		CSTCPrint( "û��֧��PCM  ����Ƶ������!\n\r");
        return;
	}
	if ((eFileType != EM_INJECT_PCM_LIT) && (eFileType != EM_INJECT_PCM_BIG))
	{
		CSTCPrint( "û��ָ����С��!\n\r");
        return;
	}

    CSTCPrint( "PCM  ����Ƶ������=%d!\n\r",nAudioId);
	
//ע�����Ƶ������
	psInjecterChnl->m_nDemux = CSUDI_INVALID_INDEX;
	psInjecterChnl->m_nAudioDecoder=nAudioId;//nAudioId;
	psInjecterChnl->m_nVideoDecoder=-1;
	
//ע������������	
	psOpenParams->m_eInjecterType=EM_INJECTER_PCM;
	psOpenParams->m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

//���ý�Ŀ��Ϣ
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,EM_UDIPLAYER_INJECT,&hPlayer), "�򿪲�����ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERSetStream (hPlayer,psStreamInfo,nStreamCnt,CSUDI_NULL), "������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "��������ʧ��");

	CSTK_FATAL_POINT;

	return hPlayer;
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "����һʧ��:û���ҵ�֧��pcm����Ƶ������");

	
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
	//�ڱ�����������ִ��ǰ����
	//CSUDIFSInit();
	//CSUDIOSThreadSleep(3000);
	
	if(!PCM_Init())
	{
		CSTCPrint("��ʼ��ʧ��\n");
		return CSUDI_FALSE;
	}
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PCM_INJECTER_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:����Open/Close PCMע����ʱ���ڲ������������Ҫ������µ�ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:����������PCMע�����ӿ����Ҫ��
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2��psInjecterChnl->m_nDemux ������-1ʱ������CSUDIINJECTEROpen��ͼ��һ��PCMע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:3��psOpenParams->m_eContentType��ΪEM_UDIINJECTER_CONTENT_AUDIOʱ������CSUDIINJECTEROpen��ͼ��һ��PCMע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��phINJECTER = CSUDI_NULL ʱ������CSUDIINJECTEROpen��ͼ��һ��PCMע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��psInjecterChnl = CSUDI_NULLʱ������CSUDIINJECTEROpen��ͼ��һ��PCMע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��psOpenParams = CSUDI_NULLʱ������CSUDIINJECTEROpen��ͼ��һ��PCMע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7��hINJECTER = CSUDI_NULLʱ������CSUDIINJECTERClose��ͼ�ر�һ����Ч��PCMע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8���ظ�����2 ������7����100��
CSUDI_BOOL CSTC_PCM_INJECTER_IT_OpenClose_0001(void)
{
	CSUDIINJECTEROpenParam_S  sOpenParams;
	CSUDIINJECTERChnl_S sInjecterChnl;
	
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	
	CSUDIAUDStreamType_E  eAudStreamType=EM_UDI_AUD_STREAM_PCM;
	
	int nAudioDecoderId = -1;
	
	int i=0;

	nAudioDecoderId=searchAudioID_Y_InInject(eAudStreamType);
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "����һʧ��:û���ҵ�֧��pcm����Ƶ������");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_VIDEO;
		sInjecterChnl.m_nDemux=-1;
		sInjecterChnl.m_nVideoDecoder=-1;		
		sInjecterChnl.m_nAudioDecoder = nAudioDecoderId;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "�����ʧ��");

		sOpenParams.m_eInjecterType=EM_INJECTER_PCM;		
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
		sInjecterChnl.m_nDemux=ERRORDATA;
		sInjecterChnl.m_nVideoDecoder=-1;		
		sInjecterChnl.m_nAudioDecoder = nAudioDecoderId;
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_DEVICE_DISORDER == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "������ʧ��");

		sOpenParams.m_eInjecterType=EM_INJECTER_PCM;		
		sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
		sInjecterChnl.m_nDemux=-1;
		sInjecterChnl.m_nVideoDecoder=-1;		
		sInjecterChnl.m_nAudioDecoder = nAudioDecoderId;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,CSUDI_NULL), "������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (CSUDI_NULL,&sOpenParams,&hINJECTER), "������ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTEROpen (&sInjecterChnl,CSUDI_NULL,&hINJECTER), "������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(CSUDI_NULL), "������ʧ��");

	}

	CSTK_FATAL_POINT;
	
	if ( hINJECTER!= CSUDI_NULL)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:����PCMע������������ر����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1���Ϸ�sInjecterChnl��psInjecterChnl->m_nDemux Ϊ-1
//@INPUT:2��sOpenParams-> m_eInjecterType = EM_INJECTER_PCM
//@INPUT:	  sOpenParams-> m_eContentType = EM_UDIINJECTER_CONTENT_AUDIO
//@INPUT:3��phINJECTER�ǿ�
//@EXPECTATION:����CSUDI_SUCCESS  
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERClose����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����2�Ͳ���3��1000�Ρ�
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "����һʧ��:û���ҵ�֧��pcm����Ƶ������");
	
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioDecoderId;
	
	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "�����ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "������ʧ��");
	}
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:����PCM ע����Open��Close ���ɶԵ��ã����Close��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �ǿ�
//@INPUT:2��psOpenParams-> m_eInjecterType=EM_INJECTER_PCM   ,
//@INPUT:	  psOpenParams-> m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO   
//@EXPECTATION:���ط�CSUDI_SUCCESS  
//@EXECUTIONFLOW:1������֧��PCMע�����Ƶ������������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen��������һ��PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTEROpen��������һ��PCMע�������������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERClose����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERClose���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6���ظ�����2������5����100��
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "����һʧ��:û���ҵ�֧��pcm����Ƶ������");
	
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioDecoderId;
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER)), "�����ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS !=  CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER1)), "������ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "������ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClose(hINJECTER), "������ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTEROpen & CSUDIINJECTERClose
//@DESCRIPTION:���߳̽������CSUDIINJECTEROpen & CSUDIINJECTERClose�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �Ϸ���
//@INPUT:2��psOpenParams-> m_eInjecterType=EM_INJECTER_PCM   ,
//@INPUT:	  psOpenParams-> m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO   
//@EXPECTATION:�����������������߼���ȷ
//@EXECUTIONFLOW:1������֧��PCMע�����Ƶ������������Ƶͨ��
//@EXECUTIONFLOW:2����������һ�����񣬸�����ͬʱ���첽ִ������Ĳ���3��4
//@EXECUTIONFLOW:3������CSUDIINJECTEROpen��������һ��PCMע��������������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_DEVICE_BUSY
//@EXECUTIONFLOW:4���������3�ɹ��������CSUDIINJECTERClose����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5���ظ�����2������4����1000��
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "����һʧ��:û���ҵ�֧��pcm����Ƶ������");

	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioDecoderId;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("OpenClose", INJECTTHREADPRO, INJECTTHREADBUF, task_For_PCMOpenColse, (void *)0, &hInjectThread), "������ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		udiRe = CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_DEVICE_BUSY == udiRe )
			|| (CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED == udiRe)),"�����ʧ��");

		CSUDIOSThreadSleep(2);
		
		if (udiRe == CSUDI_SUCCESS)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "������ʧ��");
		}
	}
	
	CSTK_FATAL_POINT;
	
	if (hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"�ȴ��������ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "������ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:����PCMע���������ΪCSUDI_NULLʱ��Start/Stop������ִ��Ч��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER = CSUDI_NULL
//@EXPECTATION:Start/Stop����������CSUDIINJECTER_ERROR_INVALID_HANDLE
//@REMARK:
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2��������PCM��Ƶע�������õ�PCMע�������
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5����hINJECTER = CSUDI_NULL,����CSUDIINJECTERStart,��������CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:6����hINJECTER = CSUDI_NULL,����CSUDIINJECTERStop,��������CSUDIINJECTER_ERROR_INVALID_HANDLE 
//@EXECUTIONFLOW:7������CSUDIINJECTERClose���رմ�PCMע����
CSUDI_BOOL CSTC_PCM_INJECTER_IT_StartStop_0001(void)
{
	CSUDIINJECTERPcmEndian_E  ePcmEndian=EM_UDIINJECTER_LITTLE_ENDIAN;
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	
	CSUDIINJECTERChnl_S sInjecterChnl;
	int nAudioID=-1;
	CSUDI_HANDLE hINJECTER= CSUDI_NULL;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	
	nAudioID=searchAudioID_Y_InInject(EM_UDI_AUD_STREAM_PCM);
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
	
//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;
	
	sPcmStartParam.m_uBitsSample = 16;
	sPcmStartParam.m_uChannels = 2;
	sPcmStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStart(CSUDI_NULL), "����5ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERStop(CSUDI_NULL), "����6ʧ��");

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����7ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:������������£�CSUDIINJECTERStart��CSUDIINJECTERStop�ɶԵ��õ����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ�����򿪵�PCMע���������
//@EXPECTATION:CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2��������PCM��Ƶע�������õ�PCMע�������
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart,������PCMע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:6������CSUDIINJECTERStop,ֹͣ��PCMע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:7������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
	
//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sPcmStartParam.m_uBitsSample = 16;
	sPcmStartParam.m_uChannels = 2;
	sPcmStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����6ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����7ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:������������£�CSUDIINJECTERStart��CSUDIINJECTERStop���ɶԻ�������õ����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ�����򿪵�PCMע���������
//@EXPECTATION:���ط�CSUDI_SUCCESS  
//@REMARK:
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2��������PCM��Ƶע�������õ�PCMע�������
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStop,ֹͣ��PCMע�������������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:6������CSUDIINJECTERStart,������PCMע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:7������CSUDIINJECTERStart,������PCMע�������������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:8������CSUDIINJECTERStop,ֹͣ��PCMע��������������CSUDI_SUCCESS 
//@EXECUTIONFLOW:9������CSUDIINJECTERStop,ֹͣ��PCMע�������������ط�CSUDI_SUCCESS 
//@EXECUTIONFLOW:10������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;
	
//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sPcmStartParam.m_uBitsSample = 16;
	sPcmStartParam.m_uChannels = 2;
	sPcmStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	for (i=0;i<100;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER), "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStart(hINJECTER), "����7ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����8ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERStop(hINJECTER), "����9ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����10ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERStart & CSUDIINJECTERStop
//@DESCRIPTION:���߳̽������CSUDIINJECTERStart & CSUDIINJECTERStop�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �Ϸ���
//@INPUT:2��psOpenParams-> m_eInjecterType=EM_INJECTER_PCM   ,
//@INPUT:	  psOpenParams-> m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO   
//@EXPECTATION:�����������������߼���ȷ
//@EXECUTIONFLOW:1������֧��PCMע�����Ƶ������������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen��������һ��PCMע�������õ�ע�������
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5����������һ�����񣬸�����ͬʱ���첽ִ������Ĳ���4��5
//@EXECUTIONFLOW:6������CSUDIINJECTERStart��������PCMע��������������CSUDI_SUCCESS��CSUDIREC_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:7������CSUDIINJECTERStop��ֹͣ��PCMע��������������CSUDI_SUCCESS��CSUDIREC_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:8���ظ�����6������7����1000��
//@EXECUTIONFLOW:9������CSUDIOSThreadJoin���ȴ�����5����������ִ����ɣ��ر��˳�
//@EXECUTIONFLOW:10������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 != nAudioDecoderId, "����һʧ��:û���ҵ�֧��pcm����Ƶ������");
	
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioDecoderId;
	
	sPcmStartParam.m_uBitsSample = 16;
	sPcmStartParam.m_uChannels = 2;
	sPcmStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("PCMStartStop", INJECTTHREADPRO, INJECTTHREADBUF, task_For_PCMStartStop, (void *)hINJECTER, &hInjectThread), "����5ʧ��");
	
	for (i=0;i<REPEAT_TIMES;i++)
	{	
		udiRe = CSUDIINJECTERStart(hINJECTER);

		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STARTED == udiRe )),"����6ʧ��");

		CSUDIOSThreadSleep(2);


		udiRe =  CSUDIINJECTERStop(hINJECTER);
		
		CSTK_ASSERT_TRUE_FATAL(((CSUDI_SUCCESS == udiRe) 
			|| (CSUDIINJECTER_ERROR_ALREADY_STOPPED == udiRe )),"����7ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	if (hInjectThread)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����8ʧ��");	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����9ʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����10ʧ��");
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:������PCMע��ģʽ���ӿڲ������������Ҫ�������£�CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute��ִ��Ч��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1�������Ͻӿ����Ҫ�� 
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PCMע�������õ����
//@EXECUTIONFLOW:3��hINJECTER = CSUDI_NULLʱ������CSUDIINJECTERSetAttribute�������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERSetAttribute����PCM��С�����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:5��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERSetAttribute ����PCM�����������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:6��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERSetAttribute ����PCM�ص��������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERSetAttribute ����ֻ�����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERSetAttribute ���÷Ƿ����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��hINJECTER = CSUDI_NULLʱ������CSUDIINJECTERGetAttribute��ȡ���ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute��ȡPCM��С�����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:11��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute ��ȡPCM�����������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12��pvData = CSUDI_NULL,ʱ������CSUDIINJECTERGetAttribute ��ȡPCM�ص��������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERSetAttribute ��ȡ�Ƿ����ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:14���ظ�����3~����13��1000��
//@EXECUTIONFLOW:15������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(CSUDI_NULL,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����3ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,CSUDI_NULL),  "����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,CSUDI_NULL),  "����5ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_STATUS, &ePcmEndian),  "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERSetAttribute(hINJECTER,ERRORDATA, &ePcmEndian),  "����8ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(CSUDI_NULL,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����9ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,CSUDI_NULL),  "����10ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,CSUDI_NULL),  "����11ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_CALLBACK,CSUDI_NULL),  "����12ʧ��");		

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetAttribute(hINJECTER,ERRORDATA,CSUDI_NULL),  "����13ʧ��");

	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����15ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:������PCMע��ģʽ���ӿڲ������������Ҫ�������£�CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute��ִ��Ч��
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1�������Ͻӿ����Ҫ�� 
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PCMע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����PCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����PCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERSetAttribute ����PCM�ص��������ԣ���������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:6������CSUDIINJECTERGetAttribute��ȡPCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:7���жϻ�ȡ�õ���PCM��С������Ϊ����3�����õ�����һ��
//@EXECUTIONFLOW:8������CSUDIINJECTERGetAttribute��ȡPCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���жϻ�ȡ�õ���PCM����������Ϊ����4�����õ�����һ��
//@EXECUTIONFLOW:10������CSUDIINJECTERGetAttribute��ȡPCM�ص��������ԣ���������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:11�������һ�����سɹ������жϻ�ȡ�õ���PCM�ص���������Ϊ����5�����õ�����һ��
//@EXECUTIONFLOW:12���ظ�����3~����11��1000��
//@EXECUTIONFLOW:13������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 16;
	sPCMStartParam.m_uChannels = 2;
	sPCMStartParam.m_uSampleRate = 44100;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
//����		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����3ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam),  "����4ʧ��");

//��ȡ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian),  "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL((ePcmEndian== EM_UDIINJECTER_LITTLE_ENDIAN),"����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam),  "����8ʧ��");
						
		CSTK_ASSERT_TRUE_FATAL(((sPCMStartParam.m_uBitsSample == 16)
			&&(sPCMStartParam.m_uChannels == 2)
			&&(sPCMStartParam.m_uSampleRate == 44100)),"����9ʧ��");
	}
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����13ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERSetAttribute&CSUDIINJECTERGetAttribute
//@DESCRIPTION:����PCMע������start֮���ٵ���CSUDIINJECTERSetAttribute/CSUDIINJECTERGetAttribute�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER �ǿ�
//@INPUT:2��eAttrType=EM_UDIINJECTER_PCM_ENDIAN  
//@INPUT:3��pvData�ǿ�
//@EXPECTATION:����CSUDIINJECTERSetAttribute    ����CSUDIINJECTER_ERROR_INVALID_STATUS������CSUDIINJECTERGetAttribute����CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@REMARK:����ע����ܵ���CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PCMע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����PCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����PCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERStart������PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERSetAttribute����PCM��С�����ԣ���������CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:7������CSUDIINJECTERSetAttribute����PCM���������ԣ���������CSUDIINJECTER_ERROR_INVALID_STATUS
//@EXECUTIONFLOW:8������CSUDIINJECTERSetAttribute ����PCM�ص��������ԣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIINJECTERGetAttribute��ȡPCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:10���жϻ�ȡ�õ���PCM��С������Ϊ����3�����õ�����һ��
//@EXECUTIONFLOW:11������CSUDIINJECTERGetAttribute��ȡPCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:12���жϻ�ȡ�õ���PCM����������Ϊ����4�����õ�����һ��
//@EXECUTIONFLOW:13������CSUDIINJECTERGetAttribute��ȡPCM�ص��������ԣ���������CSUDI_SUCCESS��CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:14���ظ�����6~����13��1000��
//@EXECUTIONFLOW:15������CSUDIINJECTERStop��ֹͣ��PCMע����
//@EXECUTIONFLOW:16������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM ;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		//����
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����6ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_STATUS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "����7ʧ��");

		//��ȡ
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����9ʧ��");

		CSTK_ASSERT_TRUE_FATAL((ePcmEndian== EM_UDIINJECTER_LITTLE_ENDIAN),"����10ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "����11ʧ��");

		CSTK_ASSERT_TRUE_FATAL(((sPCMStartParam.m_uBitsSample == 8)
			&&(sPCMStartParam.m_uChannels == 2)
			&&(sPCMStartParam.m_uSampleRate == 44100)),"����12ʧ��");		
	}
	
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop (hINJECTER), "����15ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINJECTERFlush 
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERFlush��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��PCMע�������
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL��2) Startע����֮ǰ,3) Stopע����֮�����������µ��ô˺��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PCMע�������õ����
//@EXECUTIONFLOW:3��������ע��֮ǰ����CSUDIINJECTERFlush,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����PCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERSetAttribute����PCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7����hINJECTER == CSUDI_NULL������£�����CSUDIINJECTERFlush���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERStopֹͣ��PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9����hINJECTER Ϊ����2�õ���PCMע�������ʱ������CSUDIINJECTERFlush���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����6~����9��1000��
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");
	
//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERFlush(hINJECTER), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "����5ʧ��");
	
	hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"��PCM������ʧ��");
    
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
//@INPUT:1��hINJECTER Ϊ��Ч��PCMע�������
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PCMע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����PCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����PCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERStart������PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERGetFreeBuffer�������õ�ע�������л������ĵ�ַ�ʹ�С��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERWriteComplete��֪ͨ�ײ�ע��������׼���ã�������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERFlush���ȴ�����ע������ݲ����꣬������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����6~����8��10��
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��PCMע����
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "����4ʧ��");

    	hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"��PCM������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	for (i=0;i<10;i++)
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
//@DESCRIPTION:���Բ����򳡾����������Ҫ��ʱ,�ӿں���CSUDIINJECTERClear��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��PCMע�������
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:1) hINJECTER=CSUDI_NULL��2) Startע����֮ǰ,3) Stopע����֮�����������µ��ô˺��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PCMע�������õ����
//@EXECUTIONFLOW:3��������ע��֮ǰ����CSUDIINJECTERClear,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����PCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERSetAttribute����PCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7����hINJECTER == CSUDI_NULL������£�����CSUDIINJECTERClear���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERStopֹͣ��PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:9����hINJECTER Ϊ����2�õ���PCMע�������ʱ������CSUDIINJECTERClear���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10���ظ�����6~����9��1000��
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERClear(hINJECTER), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "����5ʧ��");
	
	hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"��PCM������ʧ��");

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

//@CASEGROUP:CSUDIINJECTERClear                
//@DESCRIPTION:���Բ�����Ӧ�ó������������Ҫ��ʱ,�ӿں���CSUDIINJECTERClear��ִ�����
//@PRECONDITION:INJECTERģ�鱻������ʼ��
//@INPUT:1��hINJECTER Ϊ��Ч��PCMע�������
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PCMע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute����PCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����PCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERStart������PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERGetFreeBuffer�������õ�ע�������л������ĵ�ַ�ʹ�С��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERWriteComplete��֪ͨ�ײ�ע��������׼���ã�������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIINJECTERClear���������ע������ݣ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:9���ظ�����6~����8��10��
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��PCMע����
//@EXECUTIONFLOW:11������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "����4ʧ��");
    
    hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"��PCM������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
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
//@INPUT:1��hINJECTER Ϊ��Ч��PCMע�������
//@EXPECTATION:����CSUDI_SUCCESS
//@REMARK:1) Start֮ǰ��2)Stop֮��3)PCM���ΪCSUDI_NULL��4) ����������ָ��ΪCSUDI_NULL��5)���������Ȳ���ΪCSUDI_NULL������5������£�������Ӧ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PCMע�������õ����
//@EXECUTIONFLOW:3�����в���������������£�����CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����PCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERSetAttribute����PCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7��hINJECTERΪCSUDI_NULL������CSUDIINJECTERGetFreeBuffer����������CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8��ppvBufferΪCSUDI_NULL������CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:9��puLengthΪCSUDI_NULL������CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��PCMע����
//@EXECUTIONFLOW:11�����в���������������£�����CSUDIINJECTERGetFreeBuffer���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12���ظ�����6~����10��1000��
//@EXECUTIONFLOW:13������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "����5ʧ��");
    
    hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"��PCM������ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
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
//@INPUT:1��hINJECTER Ϊ��Ч��PCMע�������
//@EXPECTATION:���ط�CSUDI_SUCCESS
//@REMARK:1) Start֮ǰ��2)Stop֮��3)PCM���ΪCSUDI_NULL��4) uWrittenSize��ǰ�ڵõ��Ŀ��г��ȴ�����4������£�������Ӧ���ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1����ȡ֧��PCMע�����Ƶ����������Ƶͨ��
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��PCMע�������õ����
//@EXECUTIONFLOW:3������CSUDIINJECTERWriteComplete���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute����PCM��С�����ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERSetAttribute����PCM���������ԣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIINJECTERStart������PCMע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIINJECTERGetFreeBuffer�õ����л�������ַ�ͳ���
//@EXECUTIONFLOW:8��hINJECTERΪCSUDI_NULL��uWrittenSizeΪ����7�õ��Ŀ��л��������ȣ�����CSUDIINJECTERWriteComplete����������CSUDIINJECTER_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:9��hINJECTERΪ����2�õ��ľ����uWrittenSizeΪ����7�õ��Ŀ��л��������ȼ�1������CSUDIINJECTERWriteComplete���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:10������CSUDIINJECTERStop��ֹͣ��PCMע����
//@EXECUTIONFLOW:11��hINJECTERΪ����2�õ��ľ����uWrittenSizeΪ����7�õ��Ŀ��л��������ȣ�����CSUDIINJECTERWriteComplete���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:12���ظ�����6~����11��1000��
//@EXECUTIONFLOW:13������CSUDIINJECTERClose���رմ�PCMע����
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
	
	CSTK_ASSERT_TRUE_FATAL(-1 !=nAudioID, "û��֧��PCM���͵���Ƶ������");

//����ע��ʵ���������
	sInjecterChnl.m_nDemux=-1;
	sInjecterChnl.m_nVideoDecoder=-1;
	sInjecterChnl.m_nAudioDecoder=nAudioID;

	sOpenParams.m_eInjecterType=EM_INJECTER_PCM;
	sOpenParams.m_eContentType=EM_UDIINJECTER_CONTENT_AUDIO;

	sPCMStartParam.m_uBitsSample = 8;
	sPCMStartParam.m_uSampleRate = 44100;
	sPCMStartParam.m_uChannels  =2;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIINJECTERWriteComplete (hINJECTER,1), "����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_ENDIAN,&ePcmEndian), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute(hINJECTER,EM_UDIINJECTER_PCM_PARAMS,&sPCMStartParam), "����5ʧ��");

    	hPlayer = OpenPCMInectPlayer(nAudioID);

	CSTK_ASSERT_TRUE_FATAL((CSUDI_NULL != hPlayer ),"��PCM������ʧ��");

	for (i=0;i<REPEAT_TIMES;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength), "����7ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIINJECTER_ERROR_INVALID_HANDLE == CSUDIINJECTERWriteComplete (CSUDI_NULL,uLength), "����87ʧ��");

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
//@PRECONDITION:INJECTERģ�鱻������ʼ����PCM Player����������
//@INPUT:1�����в���������
//@EXECUTIONFLOW:1������CSUDIDEMUXGetCount��ȡdemux������
//@EXECUTIONFLOW:2������CSUDIDEMUXGetCapability����Ѱ�ҵ�һ��֧��EM_UDI_DEMUX_INJECT  ��demux�����ڽ�Ŀ���ŵ�demux
//@EXECUTIONFLOW:3������CSUDIINJECTEROpen��psOpenParams,demux ����������ʾ������Ƶ��������Ϊ-1��phINJECTER�ǿ�
//@EXECUTIONFLOW:4������CSUDIINJECTERStart��ʹhINJECTER ������һ����õľ��
//@EXECUTIONFLOW:5������CSUDIINJECTERGetFreeBuffer ��ʹppvBuffer  �ǿգ�puLength �ǿ�
//@EXECUTIONFLOW:6��ֹͣ������
//@EXECUTIONFLOW:7���޸�ppvBuffer�е�����
//@EXECUTIONFLOW:8������CSUDIINJECTERWriteComplete ,д�����ݣ���ʱӦ�÷��ط�CSUDI_SUCCESS������������
//@EXECUTIONFLOW:9������CSUDIINJECTERClose���ر�ע��ʵ��
CSUDI_BOOL CSTC_PCM_INJECTER_IT_WriteComplete_0002(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
		
//���������ò������Ĳ���
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

//��ȡд�뻺�����Ĳ���
	void *pvBuffer= CSUDI_NULL;
	unsigned int uLength=0;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����3ʧ��");
	
	CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam);
	CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����4ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIINJECTERGetFreeBuffer (hINJECTER,&pvBuffer,&uLength),"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(uLength > 0,"��ȡ�Ļ�����������0");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����6ʧ��");			
	
	memset(pvBuffer,0xFE,uLength);

	CSUDIINJECTERWriteComplete(hINJECTER, uLength);

	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����ʮһʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "ֹͣע����ʧ��");		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����ʮ��ʧ��");	

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע�����ֽ����PCM ���ݣ��������ŵ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ��Ƿ�������PCM���ֲ��ų���
//@EXECUTIONFLOW:11��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0001(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0001:����ע�����ֽ����PCM ���ݣ��������ŵ����\n");
	CSTCPrint("Test pcm inject of big endian data\n");
	CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Is it playing correctly ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

	CSTK_FATAL_POINT;
    
    if(CSUDI_NULL != hInjectThread)
    {
        g_bEnterTask = CSUDI_FALSE;

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����11ʧ��"); 
    
   		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");

    }

    if(CSUDI_NULL != hPlayer)
    {    
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����13ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ��");
	}
    
    if(CSUDI_NULL != hINJECTER)
    {
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����15ʧ��");	
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");
	}
    
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע�����ֽ����PCM ���ݣ����в��š���ͣ�ͻָ���ͣ
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ���֣��ܿ���PCM���ŵ��������š���ͣ��ָ�����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ��������
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:11������CSUDIPLAYERPause ����ͣPCM����
//@EXECUTIONFLOW:12����ʾ�û�ȷ�ϣ���Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:13������CSUDIPLAYERResume ���ָ�PCM����
//@EXECUTIONFLOW:14���ظ�����10-����13��3��
//@EXECUTIONFLOW:15��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:20������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0002(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0002:����ע�����ֽ����PCM ���ݣ����в��š���ͣ�ͻָ���ͣ\n");
	CSTCPrint("Test pcm inject of big endian data with pause/resume control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

	for (i=0;i<LOOP_TIMES;i++)
	{
		if (i == 0)
		{
			CSTCPrint("��Ƶ���ų�������\n");
			CSTCPrint("Do we get audio output normally ?\n");
		}
		else
		{
			CSTCPrint("�ѻָ����ţ���Ƶ�Ƿ����ͣ��������ţ�\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����11ʧ��");
		
		CSTCPrint("��Ƶ��ͣ��������\n");
		CSTCPrint("Does the audio stop playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����13ʧ��");
		
	}
	
	CSTK_FATAL_POINT;
	
    if(CSUDI_NULL != hInjectThread)
    {
        g_bEnterTask = CSUDI_FALSE;

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����15ʧ��"); 
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16ʧ��");
	}
    
    if(CSUDI_NULL != hPlayer)
    {    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����17ʧ��");    
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����18ʧ��");
    }
    
    if(CSUDI_NULL != hINJECTER)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����19ʧ��");    
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����20ʧ��");
    }

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע�����ֽ����PCM ���ݣ����в��š�ֹͣ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ���֣��ܿ���PCM���ŵ��������š���ͣ��ָ�����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ��������
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣPCM����
//@EXECUTIONFLOW:12����ʾ�û�ȷ�ϣ���Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:13������CSUDIPLAYERStart ���ָ�PCM����
//@EXECUTIONFLOW:14���ظ�����10-����13��3��
//@EXECUTIONFLOW:15��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:20������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0003(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0003:����ע�����ֽ����PCM ���ݣ����в��š�ֹͣ����\n");
	CSTCPrint("Test pcm inject of big endian data with stop/start control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

	for (i=0;i<LOOP_TIMES;i++)
	{
		if (i == 0)
		{
			CSTCPrint("��Ƶ���ų�������\n");
			CSTCPrint("Do we get audio output normally ?\n");
		}
		else
		{
			CSTCPrint("�������������ţ���Ƶ���ų�������(��Ҫ����ֹͣǰ����)\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");
		
		CSTCPrint("��Ƶֹͣ��������\n");
		CSTCPrint("Does the audio stop playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����13ʧ��");
		
	}
	
	CSTK_FATAL_POINT;
    
    if(CSUDI_NULL != hInjectThread)
    {
	    g_bEnterTask = CSUDI_FALSE;

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����15ʧ��"); 
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16ʧ��");
    }
    
    if(CSUDI_NULL != hPlayer)
    {    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����17ʧ��");    
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����18ʧ��");
    }
    
    if(CSUDI_NULL != hINJECTER)
    {
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����19ʧ��");    
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����20ʧ��");
    }

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע�����ֽ����PCM ���ݣ�����ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ��Ƿ�������PCM���ֲ��ų���
//@EXECUTIONFLOW:11��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERFlush ���ȴ���ע�������ȫ�������꣬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0004(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0004:����ע�����ֽ����PCM ���ݣ�����ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����\n");
	CSTCPrint("Test pcm inject of big endian data with CSUDIINJECTERFlush control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

    g_bEnterTask = CSUDI_FALSE;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����11ʧ��"); 
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");

	hInjectThread = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"����13ʧ��");	

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����14ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����16ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע�����ֽ����PCM ���ݣ�����ע�����������CSUDIINJECTERClear�����ע�뵫δ�����������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ��Ƿ�������PCM���ֲ��ų���
//@EXECUTIONFLOW:11��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERClear �������ע������ݣ�������Ƶ����ֹͣ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0005(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0005:����ע�����ֽ����PCM ���ݣ�����ע�����������CSUDIINJECTERClear�����ע�뵫δ�����������\n");
	CSTCPrint("Test pcm inject of big endian data with CSUDIINJECTERClear control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");
    
    g_bEnterTask = CSUDI_FALSE;
        
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����11ʧ��"); 
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");

	hInjectThread = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"����13ʧ��");	

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{
	    g_bEnterTask = CSUDI_FALSE;
	        
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����14ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����16ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��С���ֽ����PCM ���ݣ��������ŵ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ��Ƿ�������PCM���ֲ��ų���
//@EXECUTIONFLOW:11��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0006(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_INJECTER_IT_INJECTER_0006:����ע��С���ֽ����PCM ���ݣ��������ŵ����\n");
	CSTCPrint("Test pcm inject of little endian data \n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Is it playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

	CSTK_FATAL_POINT;
    
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;
	    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����11ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����13ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����15ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��С���ֽ����PCM ���ݣ����в��š���ͣ�ͻָ���ͣ
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ���֣��ܿ���PCM���ŵ��������š���ͣ��ָ�����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ��������
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:11������CSUDIPLAYERPause ����ͣPCM����
//@EXECUTIONFLOW:12����ʾ�û�ȷ�ϣ���Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:13������CSUDIPLAYERResume ���ָ�PCM����
//@EXECUTIONFLOW:14���ظ�����10-����13��3��
//@EXECUTIONFLOW:15��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:20������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0007(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0007:����ע��С���ֽ����PCM ���ݣ����в��š���ͣ�ͻָ���ͣ\n");
	CSTCPrint("Test pcm inject of little endian data with pause/resume control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

	for (i=0;i<LOOP_TIMES;i++)
	{
		if (i == 0)
		{
			CSTCPrint("��Ƶ���ų�������\n");
			CSTCPrint("Do we get audio output normally ?\n");
		}
		else
		{
			CSTCPrint("�ѻָ����ţ���Ƶ�Ƿ����ͣ��������ţ�\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����11ʧ��");
		
		CSTCPrint("��Ƶ��ͣ��������\n");
		CSTCPrint("Does the audio pause playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����13ʧ��");
		
	}
	
	CSTK_FATAL_POINT;
    
	if(CSUDI_NULL != hInjectThread)
	{
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����15ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����17ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����18ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����19ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����20ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��С���ֽ����PCM ���ݣ����в��š�ֹͣ����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ���֣��ܿ���PCM���ŵ��������š���ͣ��ָ�����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ��������
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣPCM����
//@EXECUTIONFLOW:12����ʾ�û�ȷ�ϣ���Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:13������CSUDIPLAYERStart ���ָ�PCM����
//@EXECUTIONFLOW:14���ظ�����10-����13��3��
//@EXECUTIONFLOW:15��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:20������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0008(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0008:����ע�����ֽ����PCM ���ݣ����в��š�ֹͣ����\n");
	CSTCPrint("Test pcm inject of little endian data with stop/start control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

	for (i=0;i<LOOP_TIMES;i++)
	{
		if (i == 0)
		{
			CSTCPrint("��Ƶ���ų�������\n");
			CSTCPrint("Do we get audio output normally ?\n");
		}
		else
		{
			CSTCPrint("�������������ţ���Ƶ���ų�������(��Ҫ����ֹͣǰ����)\n");
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");
		
		CSTCPrint("��Ƶֹͣ��������\n");
		CSTCPrint("Does the audio stop playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����13ʧ��");
		
	}
	
	CSTK_FATAL_POINT;
    
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����15ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����16ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����17ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����18ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����19ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����20ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��С���ֽ����PCM ���ݣ�����ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ��Ƿ�������PCM���ֲ��ų���
//@EXECUTIONFLOW:11��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERFlush ���ȴ���ע�������ȫ�������꣬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:14��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0009(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0009:����ע�����ֽ����PCM ���ݣ�����ע�����������CSUDIINJECTERFlush�ȴ���ע�����ݲ�����\n");
	CSTCPrint("Test pcm inject of little endian data with CSUDIINJECTERFlush control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Do we get audio output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

    g_bEnterTask = CSUDI_FALSE;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����11ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");

	hInjectThread = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER),"����13ʧ��");	

	CSTK_FATAL_POINT;
    
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����14ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����16ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��С���ֽ����PCM ���ݣ�����ע�����������CSUDIINJECTERClear�����ע�뵫δ�����������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ��Ƿ�������PCM���ֲ��ų���
//@EXECUTIONFLOW:11��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIINJECTERClear �������ע������ݣ�������Ƶ����ֹͣ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0010(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0010:����ע��С���ֽ����PCM ���ݣ�����ע�����������CSUDIINJECTERClear�����ע�뵫δ�����������\n");
	CSTCPrint("Test pcm inject of little endian data with CSUDIINJECTERClear control\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");
	
	CSTCPrint("��Ƶ��������������\n");
	CSTCPrint("Do we get audio output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

    g_bEnterTask = CSUDI_FALSE;
        
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����11ʧ��"); 
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");

	hInjectThread = CSUDI_NULL;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClear(hINJECTER),"����13ʧ��");    
    
	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hInjectThread)
	{
	    g_bEnterTask = CSUDI_FALSE;
	        
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����14ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����15ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����16ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����17ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����18ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����19ʧ��");
	}
    
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:���Ա����Ǵ��ֽ����pcm�ļ����ó�С���ֽ���Ĳ���
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:10����ʾ�û�ȷ����Ƶ�Ƿ�Ϊ���������߸�����û������
//@EXECUTIONFLOW:11��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0011(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0011:���Ա����Ǵ��ֽ����pcm�ļ����ó�С���ֽ���Ĳ���\n");
	CSTCPrint("Test playing a pcm stream with little endian which is original big endian \n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_BIG, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT011", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_BIG], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");
	
	CSTCPrint("��ȷ����Ƶ�Ƿ�Ϊ���������߸�����û������\n");
	CSTCPrint("Do we get noise sound output or nothing output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����11ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����13ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����15ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע������ݴ���ͨ��CSUDIINJECTERGetFreeBuffer�õ��Ļ�������С
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  ���ܲ��ų�ָ��PCM��Ƶ����
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ�������ݣ������ܹ�����������Ƶ
//@EXECUTIONFLOW:10����ʾ�û�ȷ����Ƶ�Ƿ�Ϊ���������߸�����û������
//@EXECUTIONFLOW:11��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:12������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:13������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0012(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0012:����ע������ݴ���ͨ��CSUDIINJECTERGetFreeBuffer�õ��Ļ�������С\n");
	CSTCPrint("Test that the inject data len is larger than the size got from CSUDIINJECTERGetFreeBuffer \n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT012", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask_overLength, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");
	
	CSTCPrint("��ȷ����Ƶ���ܲ��ų���\n");
	CSTCPrint("Make sure we don't get any audio output !!\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����11ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����12ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����13ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����14ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����15ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��С���ֽ����PCM ���ݣ����в��š���������,�����;����ָ�����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ���֣��ܿ���PCM���ŵ��������š��������ں;�������
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ��������
//@EXECUTIONFLOW:10������CSUDIAUDIOSetVolume ��������Ϊ���31
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:12������CSUDIAUDIOSetVolume ����������,ÿ���������2,����15��
//@EXECUTIONFLOW:13����ʾ�û�ȷ�ϣ������Ƿ��𽥼�С
//@EXECUTIONFLOW:14������CSUDIAUDIOSetVolume ����������,ÿ�ΰ���������2,����15��
//@EXECUTIONFLOW:15����ʾ�û�ȷ�ϣ������Ƿ�������
//@EXECUTIONFLOW:16������CSUDIAUDIOMute���þ���
//@EXECUTIONFLOW:17����ʾ�û�ȷ�ϣ���Ƶ�Ƿ���
//@EXECUTIONFLOW:18������CSUDIAUDIOUnmute�������
//@EXECUTIONFLOW:19����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷�
//@EXECUTIONFLOW:20��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:21������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:22������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:23������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:24������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:25������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0013(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	int i =0;
	
//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0013:����ע��С���ֽ����PCM ���ݣ����в��š���������,�����;����ָ�\n");
	CSTCPrint("Test pcm inject with little endian data,and make control of volume,mute\n");
	//CSTKWaitAnyKey();

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT0013", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "����6ʧ��");

	CSUDIOSThreadSleep(1000);
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(sPlayerChnl.m_nAudioDecoder, 31), "����10ʧ��");

	CSTCPrint("��Ƶ���ų�������\n");	
    CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");

	for(i = 0;i < 15;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(sPlayerChnl.m_nAudioDecoder,31 - 2 * i), "����12ʧ��");
		CSTCPrint("��ǰ����Ϊ:%d\n", 31 - 2 * i);
		CSUDIOSThreadSleep(500);
	}
	CSTCPrint("�������𽥼�С����?\n");
    CSTCPrint("Does the volume decrease all the way ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ��");

	for(i = 0;i < 15;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(sPlayerChnl.m_nAudioDecoder, 2 * i), "����14ʧ��");
		CSTCPrint("��ǰ����Ϊ:%d\n", 2 * i);
		CSUDIOSThreadSleep(500);
	}
	CSTCPrint("���������������?\n");	
    CSTCPrint("Does the volume increase all the way ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����15ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOMute(sPlayerChnl.m_nAudioDecoder), "����16ʧ��");
	CSTCPrint("��������\n");
    CSTCPrint("Is it mute  ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����17ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOUnmute(sPlayerChnl.m_nAudioDecoder), "����18ʧ��");
	CSTCPrint("�����ָ�����?\n");
    CSTCPrint("Is it unmute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����19ʧ��");
				
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����20ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����21ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����22ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����23ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����24ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����25ʧ��");
	}

	return CSUDI_TRUE;

}

//@CASEGROUP:INJECTER 
//@DESCRIPTION:PCMע����δ����ʱҪ����Ĭ�ϵĴ�С�ˣ����Ǵ���״̬
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@EXPECTATION:  ��δ����CSUDIINJECTERSetAttribute֮ǰ������CSUDIINJECTERGetAttributeʱ���Է�����������ô�С��Ĭ��ֵ
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERGetAttribute ����ȡPCM�Ĵ�С��ֵ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0014(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0014:PCMע����δ����ʱҪ����Ĭ�ϵĴ�С�ˣ����Ǵ���״̬\n");
	
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(((EM_UDIINJECTER_LITTLE_ENDIAN == ePcmEndian )||(EM_UDIINJECTER_BIG_ENDIAN == ePcmEndian)) ,"����4ʧ��");
	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����5ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����ע��PCM ���ݣ����в���,ֹͣע�룬������Ƶ�Ƿ����ڲ���
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  �ܲ��ų�ָ��PCM��Ƶ���֣�ֹͣע�����ƵӦ�ò��ܼ�������
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ��������
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:11������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:12����ʾ�û�ȷ�ϣ���Ƶ�Ƿ����ڲ���
//@EXECUTIONFLOW:13��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:14������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIPLAYERStop ��ֹͣ����������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18���ڲ���11������û�ȷ������Ƶ���ţ������������ͨ��������û�ȷ��û����Ƶ���ţ����������ͨ��
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0015(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0015:����ע��PCM ���ݣ����в���,ֹͣע�룬������Ƶ�Ƿ����ڲ���\n");

	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT015", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* ) &g_PCMPlayInfo[EM_INJECT_PCM_LIT], &hInjectThread), "����6ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

	CSTCPrint("��Ƶ���ų�������\n");
    CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����10ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����11ʧ��");	
		
	CSTCPrint("��Ƶֹͣ��������?\n");	
    CSTCPrint("Does the audio stop playing  ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����12ʧ��");
	
	CSTK_FATAL_POINT;
	
	if(CSUDI_NULL != hInjectThread)
	{    
	    g_bEnterTask = CSUDI_FALSE;

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����13ʧ��"); 

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����14ʧ��");
	}

	if(CSUDI_NULL != hPlayer)
	{    
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����15ʧ��");    

	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����16ʧ��");
	}

	if(CSUDI_NULL != hINJECTER)
	{
	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����18ʧ��");
	}

	return CSUDI_TRUE;
}

static void PcmSinWaveInjectTask(PCMPlayInfo_S *pPcmInfo)
{
	void * ppvBuffer = NULL;
	unsigned  int  uLength = 0;
	CSUDI_HANDLE hPcm = g_hPcmHandle;
	CSUDI_HANDLE hPcmSin ;
	int nInjectedLength = 0; //��ע��Ĵ�С

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

	//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S sPCMPlayInfo;

	#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

	//�������
	int anTestChNum[] = {1, 2}; // channel 1 ~ 2
	int anTestSampleRate[] = {11000, 48000};
	int anTestBitWidth[] = {8, 16, 32};

	int i, j, k;
	
	//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	CSTCPrint("��ע��:���������Ե������Ҳ������Ƚ���������һ��!\n");
    
	CSTKWaitAnyKey();
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����4ʧ��");

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
				
				CSTCPrint("����ע�벥��ͨ����Ϊ%d,λ��%d,������Ϊ%d,%s��PCM����\n",
							sPcmStartParam.m_uChannels, 
							sPcmStartParam.m_uBitsSample, 
							sPcmStartParam.m_uSampleRate,
							(ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"���":"С��"));
                CSTCPrint("Playing status: channels is:%d,BitsSample is:%d,SampleRate is :%d Endian mode is: %s\n",
                            sPcmStartParam.m_uChannels, 
                            sPcmStartParam.m_uBitsSample, 
                            sPcmStartParam.m_uSampleRate,
                            (ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"BIG ENDIAN":"LITTLE ENDIAN"));

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����5ʧ��");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����7ʧ��");

				g_hPcmHandle = hINJECTER;
				
				g_bEnterTask = CSUDI_TRUE;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)PcmSinWaveInjectTask, ( void* ) &sPCMPlayInfo, &hInjectThread), "����8ʧ��");
				
				g_hInjectThread = hInjectThread;
				
				
				CSTCPrint("��Ƶ��������������\n");
				CSTCPrint("Do we get audio output normally ?\n");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");
				
				g_bEnterTask = CSUDI_FALSE;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����10ʧ��");	
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"����10ʧ��");	

                hInjectThread = CSUDI_NULL;
                
                CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����11ʧ��");    
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����12ʧ��");	
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
//@DESCRIPTION:���Բ�ͬ������С��PCM���ݵĲ��������PCM����Ϊ���ʵʱ�������Ҳ�
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM����������֧��С��PCM����
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  ��ͬ����PCM������������������
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen ������һ��PCMע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERStream ����ΪPCM Stream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͨ��CSUDIINJECTERSetAttribute ����Ϊ���һ��PCM ������ϣ�����PCM���������ó���Ӧ���ԣ�PCM������Ϸ�����ͨ����[mono,setero],������[11K,48K],λ��[8,16,32]
//@EXECUTIONFLOW:6������CSUDIINJECTERStart ������ע��
//@EXECUTIONFLOW:7������CSUDIPLAYERStart ����������
//@EXECUTIONFLOW:8������ע���̣߳����ȴ�1��
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�����������"��"����������������������������Ƶ���������ʧ��
//@EXECUTIONFLOW:10��ֹͣע���߳�
//@EXECUTIONFLOW:11������CSUDIINJECTERStop ��ֹͣע��
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����
//@EXECUTIONFLOW:13���ظ�step 4 - 12�������������Ե�PCM���ݣ�ֱ�����е���ϲ������
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0016(void)
{
	return DiffPcmTest(EM_UDIINJECTER_LITTLE_ENDIAN);
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:���Բ�ͬ�����Ĵ��PCM���ݵĲ��������PCM����Ϊ���ʵʱ�������Ҳ�
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM����������֧�ִ��PCM����
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  ��ͬ����PCM������������������
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen ������һ��PCMע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERStream ����ΪPCM Stream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͨ��CSUDIINJECTERSetAttribute ����Ϊ���һ��PCM ������ϣ�����PCM���������ó���Ӧ���ԣ�PCM������Ϸ�����ͨ����[mono,setero],������[11K,48K],λ��[8,16,32]
//@EXECUTIONFLOW:6������CSUDIINJECTERStart ������ע��
//@EXECUTIONFLOW:7������CSUDIPLAYERStart ����������
//@EXECUTIONFLOW:8������ע���̣߳����ȴ�1��
//@EXECUTIONFLOW:9����ʾ�û�ȷ�ϣ��Ƿ�����������"��"����������������������������Ƶ���������ʧ��
//@EXECUTIONFLOW:10��ֹͣע���߳�
//@EXECUTIONFLOW:11������CSUDIINJECTERStop ��ֹͣע��
//@EXECUTIONFLOW:12������CSUDIPLAYERStop ��ֹͣ����
//@EXECUTIONFLOW:13���ظ�step 4 - 12�������������Ե�PCM���ݣ�ֱ�����е���ϲ������
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0017(void)
{
	return DiffPcmTest(EM_UDIINJECTER_BIG_ENDIAN);
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:������ͣ��ָ�����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  ����֧����ͣ��ָ����һָ�ʱ����������������û����Ծ
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen ������һ��PCMע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERStream ����ΪPCM Stream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͨ��CSUDIINJECTERSetAttribute ��ָΪָ��PCM����PCM���ԣ�PCM���������ԣ�ֻҪ��ƽ̨֧��
//@EXECUTIONFLOW:6������CSUDIINJECTERStart ������ע��
//@EXECUTIONFLOW:7������CSUDIPLAYERStart ����������
//@EXECUTIONFLOW:8������ע���̣߳�
//@EXECUTIONFLOW:9���ȴ�1��
//@EXECUTIONFLOW:10������CSUDIPLAYERPause����ͣ����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ����������Ƿ���ͣ�������������������ʧ��
//@EXECUTIONFLOW:12������CSUDIPLAYERResume���ָ�����
//@EXECUTIONFLOW:13����ʾ�û�ȷ�ϣ����������Ƿ�ָ�������ͣ��ָ���û����Ծ���������ʧ��
//@EXECUTIONFLOW:14���ظ�9 - 13 һ��
//@EXECUTIONFLOW:15������CSUDIINJECTERStop ��ֹͣע��
//@EXECUTIONFLOW:16������CSUDIPLAYERStop ��ֹͣ����
//@EXECUTIONFLOW:17��ֹͣע���߳�
//@EXECUTIONFLOW:18������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0018(void)
{

	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S* psPCMPlayInfo = NULL;

	int nTestCount = 2;

	//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����3ʧ��");

	/* get best endian */
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
	
	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
	}
	else
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
	}

	sPcmStartParam = psPCMPlayInfo->m_StartParam;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����5ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����7ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "����8ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSUDIOSThreadSleep(1000);
	
	CSTCPrint("��Ƶ�Ƿ��������ţ�\n");
    CSTCPrint("Do we get audio output normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");
	
	while (nTestCount--)
	{

		//pause
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����10ʧ��");
		
		CSTCPrint("��Ƶ�Ƿ���ͣ���ţ�\n");
        CSTCPrint("Is it stop playing  ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");

		//resume
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����12ʧ��");
		
		CSTCPrint("��Ƶ�Ƿ�ָ�����,������ͣ��ָ���û����Ծ?\n");
        CSTCPrint("Does it resume playing at the previous point ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ��");
	}

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����15ʧ��");    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����16ʧ��");	

	CSTK_FATAL_POINT;
	if (hPlayer)
	{
		CSUDIPLAYERClose(hPlayer);
	}
	
	if (hInjectThread)
	{
		g_bEnterTask = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����17ʧ��");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"����18ʧ��");	

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
//@DESCRIPTION:����PCMֹͣע��󣬲��ܻ�һֱ������
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@EXPECTATION:  PCMֹͣע��󣬲��ܻ�һֱ����,���ò���ʹ�ò�ͬ�����ʵ�PCM�ļ����ԣ�8bit��16bit��32bit��Ҫ���ԣ�
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen ������һ��PCMע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERStream ����ΪPCM Stream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͨ��CSUDIINJECTERSetAttribute ����Ϊ���һ��PCM �������,PCM������Ϸ�����������[8,16,32]����������
//@EXECUTIONFLOW:6������CSUDIINJECTERStart ������ע��
//@EXECUTIONFLOW:7������CSUDIPLAYERStart ����������
//@EXECUTIONFLOW:8������ʹ��CSUDIINJECTERGetFreeBuffer/CSUDIINJECTERWriteCompleteд�����ݣ�ֱ��free bufferΪ0
//@EXECUTIONFLOW:9������CSUDIINJECTERStop ��ֹͣע��
//@EXECUTIONFLOW:10������CSUDIPLAYERStop ��ֹͣ����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ�Ҫ��û����Ƶ�ٲ��ţ��������ʧ��
//@EXECUTIONFLOW:12���ظ�step 5 - 12�������������Ե�PCM���ݣ�ֱ�����е���ϲ������
//@EXECUTIONFLOW:14������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0019(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hPcmSin = CSUDI_NULL;
	
	//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S sPCMPlayInfo;

	#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

	//�������
	int anTestBitWidth[] = {8, 16, 32};

	int i;
	
	//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����4ʧ��");

	for (i = 0; i < ARRAY_LENGTH(anTestBitWidth); i++)
	{
		sPcmStartParam.m_uChannels = 1;
		sPcmStartParam.m_uBitsSample = anTestBitWidth[i];
		sPcmStartParam.m_uSampleRate = 48000;
		
		/* get best endian */
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");
	
		sPCMPlayInfo.m_StartParam = sPcmStartParam;
		sPCMPlayInfo.m_enmEndian = ePcmEndian;
		sPCMPlayInfo.m_pcName[0] = '\0';

		hPcmSin = CSPcmSin_Open(&sPcmStartParam, ePcmEndian, 500);
		
		CSTCPrint("����ע�벥��ͨ����Ϊ%d,λ��%d,������Ϊ%d,%s��PCM����\n",
					sPcmStartParam.m_uChannels, 
					sPcmStartParam.m_uBitsSample, 
					sPcmStartParam.m_uSampleRate,
					(ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"���":"С��"));
        CSTCPrint("Playing status: channels is:%d,BitsSample is:%d,SampleRate is :%d Endian mode is: %s\n",
                    sPcmStartParam.m_uChannels, 
                    sPcmStartParam.m_uBitsSample, 
                    sPcmStartParam.m_uSampleRate,
                    (ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"BIG ENDIAN":"LITTLE ENDIAN"));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����3ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����7ʧ��");

		InjectToFull(hPcmSin, hINJECTER);

		CSTCPrint("��ע��Buffer������ֹͣע��\n");
		CSTCPrint("The inject Buffer is full,we stop injecting !\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����9ʧ��");	
		
		CSTCPrint("��Ƶ�Ƿ�ֹͣ����?\n");
		CSTCPrint("Does it stop playing and we don't get audio output ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����10ʧ��");	

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
//@DESCRIPTION:����flush����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@EXPECTATION:  PCMֹͣע��󣬵���CSUDIINJECTERFlush��ֱ����������Ž���
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen ������һ��PCMע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERStream ����ΪPCM Stream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͨ��CSUDIINJECTERSetAttribute ����Ϊ���һ��PCM �������,PCM������Ϸ�����������[8,16,32]����������
//@EXECUTIONFLOW:6������CSUDIINJECTERStart ������ע��
//@EXECUTIONFLOW:7������CSUDIPLAYERStart ����������
//@EXECUTIONFLOW:8������ʹ��CSUDIINJECTERGetFreeBuffer/CSUDIINJECTERWriteCompleteд�����ݣ�ֱ��free bufferΪ0
//@EXECUTIONFLOW:9������CSUDIINJECTERFlush ���ȴ�ע�����������
//@EXECUTIONFLOW:10������CSUDIINJECTERStop ��ֹͣע��
//@EXECUTIONFLOW:11������CSUDIPLAYERStop ��ֹͣ����
//@EXECUTIONFLOW:12����ʾ�û�ȷ�ϣ�������һ�񲥷�һ��ʱ��Ž������������ʧ��
//@EXECUTIONFLOW:13������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:14������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:15������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0020(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hPcmSin = CSUDI_NULL;
	
	//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S sPCMPlayInfo;

	#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

	//�������
	int anTestBitWidth[] = {8, 16, 32};

	int i;
	
	//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����4ʧ��");

	for (i = 0; i < ARRAY_LENGTH(anTestBitWidth); i++)
	{
		sPcmStartParam.m_uChannels = 1;
		sPcmStartParam.m_uBitsSample = anTestBitWidth[i];
		sPcmStartParam.m_uSampleRate = 48000;
		
		/* get best endian */
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");
	
		sPCMPlayInfo.m_StartParam = sPcmStartParam;
		sPCMPlayInfo.m_enmEndian = ePcmEndian;
		sPCMPlayInfo.m_pcName[0] = '\0';

		hPcmSin = CSPcmSin_Open(&sPcmStartParam, ePcmEndian, 500);
		
		CSTCPrint("����ע�벥��ͨ����Ϊ%d,λ��%d,������Ϊ%d,%s��PCM����\n",
					sPcmStartParam.m_uChannels, 
					sPcmStartParam.m_uBitsSample, 
					sPcmStartParam.m_uSampleRate,
					(ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"���":"С��"));
        CSTCPrint("Playing status: channels is:%d,BitsSample is:%d,SampleRate is :%d Endian mode is: %s\n",
                    sPcmStartParam.m_uChannels, 
                    sPcmStartParam.m_uBitsSample, 
                    sPcmStartParam.m_uSampleRate,
                    (ePcmEndian == EM_UDIINJECTER_BIG_ENDIAN?"BIG ENDIAN":"LITTLE ENDIAN"));

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����5ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����7ʧ��");

		InjectToFull(hPcmSin, hINJECTER);// 8

		CSTCPrint("��ע��Buffer�����ڵȴ�����������\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER), "����9ʧ��");	
		
		CSTCPrint("����ֹͣ����\n");
        	CSTCPrint("Now stop the injecter and player !\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����10ʧ��");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����11ʧ��");	

		CSTCPrint("�Ƿ񲥷�һС����Ƶ�Ž���?\n");
        	CSTCPrint("Does it have a short audio output before it stops ?\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ��");

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
//@DESCRIPTION:PCMע��ʱ���������á������������á��������ܲ���
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  PCMע�벥��ʱ�������������������������ܹ���������
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen ������һ��PCMע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERStream ����ΪPCM Stream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͨ��CSUDIINJECTERSetAttribute ����ѡ������PCM����
//@EXECUTIONFLOW:6������CSUDIINJECTERStart ������ע��
//@EXECUTIONFLOW:7������CSUDIPLAYERStart ����������
//@EXECUTIONFLOW:8������ע���̣߳�ѭ��ע�벥��
//@EXECUTIONFLOW:9��ȷ���Ƿ���������
//@EXECUTIONFLOW:10��������������31��0��ÿ��1�ȴ�200ms����0ʱ�ȴ�2�룬����0��31��ÿ��1�ȴ�200ms
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ������Ƿ��ɴ�����������������Ϊ�ܴ�
//@EXECUTIONFLOW:12������Ϊ������
//@EXECUTIONFLOW:13����ʾ�û�ȷ�ϣ���ǰ�Ƿ�Ϊ������
//@EXECUTIONFLOW:14������Ϊ������
//@EXECUTIONFLOW:15����ʾ�û�ȷ�ϣ���ǰ�Ƿ�Ϊ������
//@EXECUTIONFLOW:16������Ϊ������
//@EXECUTIONFLOW:17����ʾ�û�ȷ�ϣ���ǰ�Ƿ�Ϊ������
//@EXECUTIONFLOW:18������
//@EXECUTIONFLOW:19����ʾ�û�ȷ�ϣ���ǰ�Ƿ�������
//@EXECUTIONFLOW:20��ȡ������
//@EXECUTIONFLOW:21����ʾ�û�ȷ�ϣ���ǰ�������Ƿ�ָ�
//@EXECUTIONFLOW:22������CSUDIINJECTERStop ��ֹͣע��
//@EXECUTIONFLOW:23������CSUDIPLAYERStop ��ֹͣ����
//@EXECUTIONFLOW:24��ֹͣע���߳�
//@EXECUTIONFLOW:25��ɾ��ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0021(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S* psPCMPlayInfo = NULL;

	int nAudioId = 0;

	int nVol = 0;

	//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	nAudioId = sPlayerChnl.m_nAudioDecoder;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����4ʧ��");

	/* get best endian */
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");
	
	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
	}
	else
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
	}

	sPcmStartParam = psPCMPlayInfo->m_StartParam;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����5ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����7ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "����8ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSUDIOSThreadSleep(100);
	
	CSTCPrint("��Ƶ�Ƿ��������ţ�\n");
    CSTCPrint("Is the audio playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

	//volume 31-0 0-31
	for (nVol = 31; nVol >= 0; nVol--)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, nVol), "����10ʧ��");
		CSTCPrint("��ǰ����Ϊ:%d\n", nVol);
		CSUDIOSThreadSleep(200);
	}
	
	CSUDIOSThreadSleep(2000);
    
	for (nVol = 1; nVol <= 31; nVol++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, nVol), "����10ʧ��");
		CSTCPrint("��ǰ����Ϊ:%d\n", nVol);
		CSUDIOSThreadSleep(200);
	}
	
	CSTCPrint("�����Ƿ��ɴ�����������������Ϊ�ܴ�?\n");
    CSTCPrint("Does the volume decrease from very loud to null and  from null to very loud?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");

	//set left channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_LEFT), "����12ʧ��");	
	CSTCPrint("�Ƿ�Ϊ������?\n");
    CSTCPrint("Is the audio left channel ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ��");

	//set right channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_RIGHT), "����14ʧ��");	
	CSTCPrint("�Ƿ�Ϊ������?\n");
    CSTCPrint("Is the audio rigth channel ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����15ʧ��");

	//set stereo channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_STEREO), "����16ʧ��");	
	CSTCPrint("�Ƿ�Ϊ������?\n");
    CSTCPrint("Is the audio stereo channel ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����17ʧ��");

	//mute
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "����18ʧ��");	
	CSTCPrint("�Ƿ���?\n");
    CSTCPrint("Is the audio mute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����19ʧ��");

	//unmute
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioId), "����20ʧ��");	
	CSTCPrint("�����Ƿ��Ѿ�ȡ��?\n");
    CSTCPrint("Is the audio umute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����21ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����22ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����23ʧ��");	

CSTK_FATAL_POINT;
	if (hPlayer)
	{
		CSUDIPLAYERClose(hPlayer);
	}
	
	if (hInjectThread)
	{
		g_bEnterTask = CSUDI_FALSE;
        
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����24ʧ��");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"����25ʧ��");
        
		hInjectThread = CSUDI_NULL;
	}

	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER & AUDIO
//@DESCRIPTION:PCMע�뾲������£�ֹͣע�������²��ţ����Ծ��������Ƿ�����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  PCMע�벥��ʱ������������ظ�ע�룬������Ǿ�����������
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen ������һ��PCMע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERStream ����ΪPCM Stream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͨ��CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:6������CSUDIINJECTERStart ������ע��
//@EXECUTIONFLOW:7������CSUDIPLAYERStart ����������
//@EXECUTIONFLOW:8������ע���̣߳�ѭ��ע�벥��
//@EXECUTIONFLOW:9��ȷ���Ƿ���������
//@EXECUTIONFLOW:10������CSUDIAUDIOMute����
//@EXECUTIONFLOW:11����ʾ�û�ȷ�ϣ���ǰ�Ƿ�������
//@EXECUTIONFLOW:12����ͣע���߳�
//@EXECUTIONFLOW:13������CSUDIINJECTERStop ��ֹͣע��
//@EXECUTIONFLOW:14������CSUDIPLAYERStop ��ֹͣ����
//@EXECUTIONFLOW:15������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:16������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:17������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:18������CSUDIPLAYEROpen ������һ��PCMע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:19������CSUDIPLAYERStream ����ΪPCM Stream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:20��ͨ��CSUDIINJECTERSetAttribute
//@EXECUTIONFLOW:21������CSUDIINJECTERStart ������ע��
//@EXECUTIONFLOW:22������CSUDIPLAYERStart ����������
//@EXECUTIONFLOW:23������ע���̣߳�ѭ��ע�벥��
//@EXECUTIONFLOW:24��ȷ���Ƿ��������ţ������������
//@EXECUTIONFLOW:25��ȡ���������������������
//@EXECUTIONFLOW:26��ֹͣע���߳�
//@EXECUTIONFLOW:27������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:28������CSUDIPLAYERStop ��ֹͣ����
//@EXECUTIONFLOW:29������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:30������CSUDIINJECTERStop ��ֹͣע��
//@EXECUTIONFLOW:31������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:32���ظ�����1��31��3�Σ��������ж���������ֵ
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0022(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E        ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S* psPCMPlayInfo = NULL;

	int nAudioId = 0;
    int i = 0;
	//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	nAudioId = sPlayerChnl.m_nAudioDecoder;
	for(i=0;i<LOOP_TIMES;i++)
	{
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����4ʧ��");

    	/* get best endian */
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");
    	
    	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
    	{
    		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
    	}
    	else
    	{
    		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
    	}

    	sPcmStartParam = psPCMPlayInfo->m_StartParam;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����5ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����7ʧ��");

    	g_hPcmHandle = hINJECTER;
    	
    	g_bEnterTask = CSUDI_TRUE;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "����8ʧ��");
    	
    	g_hInjectThread = hInjectThread;

    	CSUDIOSThreadSleep(100);
    	
    	CSTCPrint("��Ƶ�Ƿ��������ţ�\n");
        CSTCPrint("Is the audio playing normally ?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

    	//mute
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "����10ʧ��");	
    	CSTCPrint("�Ƿ���?\n");
        CSTCPrint("Is the audio mute ?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����11ʧ��");
        
    	g_bEnterTask = CSUDI_FALSE;
        //ֹͣ������
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����12ʧ��");	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"����12ʧ��");	

       	hInjectThread = CSUDI_NULL;
        
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����13ʧ��");	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����14ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERClose(hPlayer), "����15ʧ��");	
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����16ʧ��");

        ///�л��������²���
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����17ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����18ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����19ʧ��");

    	/* get best endian */
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����20ʧ��");
    	
    	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
    	{
    		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
    	}
    	else
    	{
    		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
    	}

    	sPcmStartParam = psPCMPlayInfo->m_StartParam;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����5ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����21ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����22ʧ��");

    	g_hPcmHandle = hINJECTER;
    	
    	g_bEnterTask = CSUDI_TRUE;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "����23ʧ��");
    	
    	g_hInjectThread = hInjectThread;

    	CSUDIOSThreadSleep(100);
    	
    	CSTCPrint("����������PCM���ţ��������������豸���������?\n");
        CSTCPrint("Does the audio has no output?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����24ʧ��");

    	//unmute
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioId), "����25ʧ��");	
    	CSTCPrint("�����Ƿ��Ѿ�ȡ��?\n");
        CSTCPrint("Is the audio umute ?\n");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"ȡ������ʧ��");

        g_bEnterTask = CSUDI_FALSE;
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����26ʧ��"); 
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"����27ʧ��");  

        hInjectThread = CSUDI_NULL;

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer), "����28ʧ��");	
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����29ʧ��");   
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����30ʧ��");
    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����31ʧ��");

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
//@DESCRIPTION:PCMע��ʱ�ڷǲ���״̬���������á��������ܲ���
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  PCMע��ֹͣ������ͣ����ʱ���������������ܹ��������ã��������²��ź���Ч��
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIPLAYEROpen ������һ��PCMע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIPLAYERStream ����ΪPCM Stream����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5��ͨ��CSUDIINJECTERSetAttribute ����ѡ������PCM����
//@EXECUTIONFLOW:6������CSUDIINJECTERStart ������ע��
//@EXECUTIONFLOW:7������CSUDIPLAYERStart ����������
//@EXECUTIONFLOW:8������ע���̣߳�ѭ��ע�벥��
//@EXECUTIONFLOW:9��ȷ���Ƿ���������
//@EXECUTIONFLOW:10����������Ϊ0����ʾ�û�ȷ��û���������
//@EXECUTIONFLOW:11����ͣ���ţ�Ȼ����������Ϊ31
//@EXECUTIONFLOW:12���ָ����ţ���ʾ�û�ȷ�ϴ�ʱ�����������������Ӧ����31
//@EXECUTIONFLOW:13����������Ϊ0����ʾ�û�ȷ��û���������
//@EXECUTIONFLOW:14��ֹͣ���ţ�Ȼ����������Ϊ31
//@EXECUTIONFLOW:15�����²��ţ���ʾ�û�ȷ�ϴ�ʱ�����������������Ӧ����31
//@EXECUTIONFLOW:16�����þ�������ʾ�û�ȷ��û���������
//@EXECUTIONFLOW:17����ͣ���ţ�Ȼ��ָ�����
//@EXECUTIONFLOW:18���ָ����ţ���ʾ�û�ȷ�ϴ�ʱ���������
//@EXECUTIONFLOW:19�����þ�������ʾ�û�ȷ��û���������
//@EXECUTIONFLOW:20��ֹͣ���ţ�Ȼ��ָ�����
//@EXECUTIONFLOW:21�����²��ţ���ʾ�û�ȷ�ϴ�ʱ���������
//@EXECUTIONFLOW:22��ֹͣ���ţ�Ȼ����о��������������سɹ�
//@EXECUTIONFLOW:23������������0��31�������ɹ�
//@EXECUTIONFLOW:24���ָ����������������ɹ��������������������������óɹ�
//@EXECUTIONFLOW:25�����²��ţ���ʾ�û�ȷ�ϴ�ʱ������������Ҵ�������Ӧ����31
//@EXECUTIONFLOW:26������CSUDIINJECTERStop ��ֹͣע��
//@EXECUTIONFLOW:27������CSUDIPLAYERStop ��ֹͣ����
//@EXECUTIONFLOW:28��ֹͣע���߳�
//@EXECUTIONFLOW:29������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0023(void)
{
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E      ePcmEndian = EM_UDIINJECTER_BIG_ENDIAN;

	//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
		
	//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	PCMPlayInfo_S* psPCMPlayInfo = NULL;

	int nAudioId = 0;

	int nVol = 0;

	//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	//set default params
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);

	nAudioId = sPlayerChnl.m_nAudioDecoder;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����3ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����4ʧ��");

	/* get best endian */
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");
	
	if (ePcmEndian == EM_UDIINJECTER_LITTLE_ENDIAN)
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_LIT];
	}
	else
	{
		psPCMPlayInfo = &g_PCMPlayInfo[EM_INJECT_PCM_BIG];
	}

	sPcmStartParam = psPCMPlayInfo->m_StartParam;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &sPcmStartParam),"����5ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����5ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����6ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����7ʧ��");

	g_hPcmHandle = hINJECTER;
	
	g_bEnterTask = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT001", INJECTTHREADPRO, 64*1024, (CSUDIOSThreadEntry_F)pcm_injectTask, ( void* ) psPCMPlayInfo, &hInjectThread), "����8ʧ��");
	
	g_hInjectThread = hInjectThread;

	CSUDIOSThreadSleep(100);
	
	CSTCPrint("��Ƶ�Ƿ��������ţ�\n");
    CSTCPrint("Is the audio playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����9ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, 0), "����10ʧ��");
	CSTCPrint("�Ƿ������豸��������� ?\n");
    CSTCPrint("Does all the device have no audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����11ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, 31), "����11ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����12ʧ��");
	CSTCPrint("�Ƿ������豸��������� ,�Ҵ�ʱ����Ӧ��Ϊ���ֵ?\n");
    CSTCPrint("Does the all the device have audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����12ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, 0), "����13ʧ��");
	CSTCPrint("�Ƿ������豸��������� ?\n");
    CSTCPrint("Does all the device have no audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����13ʧ��");


    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����14ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, 31), "����14ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����15ʧ��");
	CSTCPrint("�Ƿ������豸��������� ,�Ҵ�ʱ����Ӧ��Ϊ���ֵ?\n");
    CSTCPrint("Does the all the device have audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����15ʧ��");

	//mute
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "����16ʧ��");	
	CSTCPrint("�Ƿ���?\n");
    CSTCPrint("Is the audio mute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����16ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����17ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOUnmute(nAudioId), "����17ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����18ʧ��");
	CSTCPrint("�����Ƿ��Ѿ�ȡ��?\n");
    CSTCPrint("Is the audio umute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����18ʧ��");

	//mute
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "����19ʧ��");	
	CSTCPrint("�Ƿ���?\n");
    CSTCPrint("Is the audio mute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����19ʧ��");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����20ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOUnmute(nAudioId), "����20ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����21ʧ��");
	CSTCPrint("�����Ƿ��Ѿ�ȡ��?\n");
    CSTCPrint("Is the audio umute ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����21ʧ��");
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����22ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOMute(nAudioId), "����22ʧ��");	

	for (nVol = 0; nVol <= 31; nVol++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOSetVolume(nAudioId, nVol), "����23ʧ��");
		CSUDIOSThreadSleep(200);
	}

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIAUDIOUnmute(nAudioId), "����24ʧ��");
    
	//set left channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_LEFT), "����24ʧ��");	

	//set right channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_RIGHT), "����24ʧ��");	

	//set stereo channel
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIAUDIOSetChannel(nAudioId, EM_UDIAUDIO_CHANNEL_STEREO), "����24ʧ��");	


	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer), "����25ʧ��");	
	CSTCPrint("��Ƶ�Ƿ��������ţ�\n");
    CSTCPrint("Is the audio playing normally ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����25ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����26ʧ��");	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStop(hINJECTER), "����27ʧ��");	

    CSTK_FATAL_POINT;
	if (hPlayer)
	{
		CSUDIPLAYERClose(hPlayer);
	}
	
	if (hInjectThread)
	{
		g_bEnterTask = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread),"����28ʧ��");	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread),"����29ʧ��");	

        hInjectThread = CSUDI_NULL;
	}

	if (hINJECTER)
	{
		CSUDIINJECTERClose(hINJECTER);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:INJECTER & PLAYER
//@DESCRIPTION:����PCM����ע����ɵ����������һ��ʱ���ڵ�pause��resume��GetAttribute��flush����
//@PRECONDITION:1��INJECTERģ�鱻������ʼ��
//@PRECONDITION:2��ϵͳ����PCM������
//@PRECONDITION:3���������ļ�����U�̸�Ŀ¼��,����U��
//@INPUT:	1������ע����
//@INPUT:	2������������
//@INPUT:	3������ע���߳�
//@EXPECTATION:  
//@EXECUTIONFLOW:1������֧��pcm����Ƶ������
//@EXECUTIONFLOW:2������CSUDIINJECTEROpen������һ��ע��PCM  ������ע��ʵ������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIINJECTERSetAttribute ����PCM  ������������������
//@EXECUTIONFLOW:4������CSUDIINJECTERSetAttribute ����PCM�Ĵ�С�˽�������
//@EXECUTIONFLOW:5������CSUDIINJECTERStart����ʼ����ע�룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������һ���߳�,ѭ����ȡPCM   ���ļ���������PCMע�����
//@EXECUTIONFLOW:7������CSUDIPLAYEROpen ������һ��ע��Ĳ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIPLAYERSetStream �����ú�pcm����Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIPLAYERStart ����ʼ��������
//@EXECUTIONFLOW:10����ʾ�û�ȷ�ϣ���Ƶ�Ƿ񲥷ų���
//@EXECUTIONFLOW:11������CSUDIOSThreadSleep�ȴ�5��
//@EXECUTIONFLOW:12��֪ͨע������ֹͣע�����ݣ��ȴ�ע������������������
//@EXECUTIONFLOW:13������CSUDIINJECTERGetAttribute��ȡInjecter�Ѿ�ע�뵽Ŀ�껺������������������е���δ�����ĵ������ݳ���(��λ�ֽ�)nAvailableDataSize����������CSUDI_SUCCESS
//@EXECUTIONFLOW:14����δ���ĵ����ݳ���nAvailableDataSize����0��ִ����һ����������ת��19��
//@EXECUTIONFLOW:15��������CSUDIPLAYERPause������ͣ�Ĵ���δ����1�Σ���ִ����һ����������ת��13��
//@EXECUTIONFLOW:16������CSUDIPLAYERPause����ͣPCM���ţ�
//@EXECUTIONFLOW:17����ʾ�û�ȷ�ϣ���Ƶ�Ƿ�ֹͣ
//@EXECUTIONFLOW:18������CSUDIPLAYERResume ���ָ�PCM����,����ת��13��
//@EXECUTIONFLOW:19������CSUDIINJECTERFlush()�ȴ���������,��������CSUDI_SUCCESS
//@EXECUTIONFLOW:20����ʾ�û�ȷ�ϣ���Ƶ�Ƿ���������
//@EXECUTIONFLOW:21������CSUDIPLAYERClose ���رղ���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:22������CSUDIINJECTERStop ��ֹͣע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:23������CSUDIINJECTERClose ���ر�ע��������������CSUDI_SUCCESS
//@EXECUTIONFLOW:24������CSUDIOSThreadDestroy ��ɾ��ע��������������CSUDI_SUCCESS
CSUDI_BOOL CSTC_PCM_INJECTER_IT_INJECTER_0024(void)
{
	int i = 0;
	CSUDIINJECTERPcmStartParam_S	sPcmStartParam;
	CSUDIINJECTERPcmEndian_E	  ePcmEndian = EM_UDIINJECTER_LITTLE_ENDIAN;

	//ע��ʵ���Ĳ���	
	CSUDIINJECTERChnl_S  sInjecterChnl;
	CSUDIINJECTEROpenParam_S   sOpenParams;
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;

	//���������ò������Ĳ���	
	CSUDIPlayerChnl_S  sPlayerChnl;
	CSUDIStreamInfo_S  sStreamInfo[1];
	int nStreamCnt = 1;
	CSUDI_HANDLE   hPlayer = CSUDI_NULL;

	//��������������
	CSUDIPLAYERType_E	ePlayerType=EM_UDIPLAYER_INJECT;

	static PCMPlayInfo_S s_TTSPlayInfo =
	{
		{"tts.wav"},
		{16, 16000, 1},
		EM_UDIINJECTER_LITTLE_ENDIAN
	};

	CSTCPrint("CSTC_PCM_INJECTER_IT_INJECTER_0024: ����PCM����ע����ɵ����������һ��ʱ���ڵ�pause��resume��GetAttribute��flush����\n");
	
	getPcmInjectInfo(EM_INJECT_PCM_LIT, &sInjecterChnl,&sPlayerChnl, &sOpenParams,&sPcmStartParam,sStreamInfo);
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTEROpen (&sInjecterChnl,&sOpenParams,&hINJECTER), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_PARAMS, &s_TTSPlayInfo.m_StartParam),"����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERSetAttribute (hINJECTER,EM_UDIINJECTER_PCM_ENDIAN , &ePcmEndian),"����4ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERStart (hINJECTER), "����5ʧ��");

	g_hPcmHandle = hINJECTER;
	g_bEnterTask = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("INJECT024", INJECTTHREADPRO, INJECTTHREADBUF, pcm_injectTask, ( void* )&s_TTSPlayInfo, &hInjectThread), "����6ʧ��");

	g_hInjectThread = hInjectThread;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYEROpen (&sPlayerChnl,ePlayerType,&hPlayer), "����7ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERSetStream (hPlayer,sStreamInfo,nStreamCnt,CSUDI_NULL), "����8ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStart(hPlayer), "����9ʧ��");

	CSTCPrint("��Ƶ���ų�������\n");
	CSTCPrint("Do we get audio output ?\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����10ʧ��");

	// �ȴ�ע���굱ǰ�ļ���pcm���ݺ��˳�ע���߳�
	g_bInjectOnlyOnce = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadJoin(hInjectThread), "����12ʧ��"); 

	i = 0;
	while (1)
	{
		CSUDIINJECTERStatus_S sInjectStatus;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERGetAttribute(hINJECTER, EM_UDIINJECTER_STATUS, (void *)&sInjectStatus), "����13ʧ��");

		if (sInjectStatus.m_nAvailableDataSize > 0)
		{
			if (i < 1)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERPause(hPlayer), "����16ʧ��");

				CSTCPrint("��Ƶ��ͣ��������\n");
				CSTCPrint("Does the audio pause playing?\n");
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����17ʧ��");

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERResume(hPlayer), "����18ʧ��");

				i++;
			}
		}
		else
		{
			unsigned int uTime = CSUDIPlusOSGetTickCount();

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERFlush(hINJECTER), "����19ʧ��");

			uTime = CSUDIPlusOSGetTickCount() - uTime;

			// m_nAvailableDataSize����0���ٵ���flush�ӿ�ʱ��flushʱ�䲻�ô���2400ms����ֵΪ����ֵ
			CSTK_ASSERT_TRUE_FATAL(uTime < 2400, "����20ʧ��");
			
			CSTCPrint("��Ƶ�Ƿ������������ţ��Ҳ��ŵ���󼸸���Ϊ\"�Ҳ���ͷ�乶���������\"��\n");
			CSTCPrint("Does the audio finished Immediately?\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����21ʧ��");
			break;
		}

		CSUDIOSThreadSleep(10);
	}

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS  == CSUDIPLAYERStop(hPlayer), "����22ʧ��");	

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer), "����23ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER), "����24ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread), "����25ʧ��");

	return CSUDI_TRUE;
}

