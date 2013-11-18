/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_tunertestcase.h"
#include "cs_tunertest.h"
#include "udi2_rdi.h"
#include "udi2_os.h"

#define ADD_REMOVE_COUNT 50
#define FREQUENCY_NUM_MAX 50

#define TEST_COUNT  10
#define TUNER_SIGNAL_CONNECTED 1001
#define TUNER_SIGNAL_LOST 2002
#define TUNER_SIGNAL_SEARCHING 3003

static int s_nTunerTestData = 0;

static CSUDI_BOOL s_bIsSignalLost = CSUDI_FALSE;
static CSUDI_BOOL s_bIsSignalSearching = CSUDI_FALSE;
static CSUDI_BOOL s_bIsSignalConnected = CSUDI_FALSE;

static CSUDI_BOOL s_bCallBackFlag = CSUDI_FALSE;
static int s_nUserData =0x1111;

static int s_nRDIEventData = -1;

static CSUDI_BOOL s_bRDITunerIsReady = CSUDI_FALSE;

static CSUDITunerSRCDeliver_U s_unDeliver[100];

static CSUDI_UINT32 s_dwRDITunerId = 0;

static CSUDITunerInfo_S s_asTunerInfo[10];

static int s_nActTunerCnt = 0;

static int s_nDeliverCount = 0;

static DWORD s_dwLockCount = 0; 

static BOOL s_bRightFreqPiont = FALSE;

static CSUDIOSTimeVal_S s_sTimeEnd ;

static CSUDI_BOOL s_bIsSupportRDITuner = CSUDI_FALSE;

extern CSUDI_BOOL g_bIsThreadNaturalEndFlag1;

extern CSUDI_BOOL g_bIsThreadNaturalEndFlag2;

extern CSUDI_BOOL g_bIsTunerConnected;

typedef enum
{
      EM_TUNER_QAM64,  
      EM_TUNER_QAM128,        
      EM_TUNER_QAM256    
}CSTUNERQAMType_E;


static CSUDI_BOOL CSTC_TUNER_IT_QamLockInvail(CSUDI_UINT32 dwTunerIndex);

static void Tuner_CallbackTest1( CSUDITunerType_E eType, CSUDITunerCallbackMessage_S* psMessage)
{
	if(psMessage != CSUDI_NULL)
	{
		if(psMessage->m_pvUserData == CSUDI_NULL)
		{
			s_nTunerTestData = 0x1234;
		}
		else
		{
			s_nTunerTestData = *((int *)(psMessage->m_pvUserData));
		}
	}
	else
	{
		CSTCPrint("\n�ص���ϢpsMessageΪ��!!!!\n");
	}
}

static void Tuner_CallbackTest2(CSUDITunerType_E eType, CSUDITunerCallbackMessage_S *psMessage)
{
	CSUDITunerSRCDeliver_U * p = CSUDI_NULL;
		
	if(psMessage != CSUDI_NULL)
	{
		if(CSUDI_NULL != psMessage->m_pvUserData)
		{
			p = (CSUDITunerSRCDeliver_U *)psMessage->m_pvUserData;
		}
		
		switch(psMessage->m_eEvent)
		{
			case EM_UDITUNER_SIGNAL_CONNECTED:
				s_nTunerTestData = TUNER_SIGNAL_CONNECTED;
				g_bIsTunerConnected = CSUDI_TRUE;
                s_bIsSignalConnected = CSUDI_TRUE;
				s_bIsSignalSearching = CSUDI_FALSE;
				CSTCPrint("\nSignal has been connected\n");
				break;
			case EM_UDITUNER_SIGNAL_LOST:
                s_nTunerTestData = TUNER_SIGNAL_LOST;
                s_bIsSignalLost = CSUDI_TRUE;				
				s_bIsSignalSearching = CSUDI_FALSE;
				CSTCPrint("\nSignal has been lost\n");
				break;
			case EM_UDITUNER_SIGNAL_SEARCHING:
				if(CSUDI_NULL != psMessage->m_pvUserData)
				{
					s_dwLockCount++;
				
					if((p->m_sCableDeliver.m_uFrequency == psMessage->m_uFrequency) && \
				  	 (p->m_sCableDeliver.m_uModulation_8 == psMessage->m_uQAMMode) && \
				   	 (p->m_sCableDeliver.m_uSymbolRate_24 == psMessage->m_uSymbolRate)
				     )
				    {
						if(1 == s_dwLockCount)
						{
							CSUDIOSGetTime(&s_sTimeEnd);
							s_bRightFreqPiont = TRUE;
						}
					}
				}	
				
				s_nTunerTestData = TUNER_SIGNAL_SEARCHING;
                s_bIsSignalSearching = CSUDI_TRUE;
				CSTCPrint("\nSignal has been searching\n");
				break;
			default:
				s_bIsSignalLost = CSUDI_FALSE;
				s_nTunerTestData = 0;
				break;
		}
	}
	else
	{
		CSTCPrint("\n�ص���ϢpsMessageΪ��!!!!\n");
	}
}

static void Tuner_CallbackTest3( CSUDITunerType_E eType, CSUDITunerCallbackMessage_S* psMessage)
{
    s_nTunerTestData = 0x1234;
		
	if(psMessage != CSUDI_NULL)
	{
		if(psMessage->m_uFrequency != s_unDeliver[0].m_sCableDeliver.m_uFrequency
			|| psMessage->m_uSymbolRate != s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24
			|| psMessage->m_uQAMMode != s_unDeliver[0].m_sCableDeliver .m_uModulation_8)
		{
			s_nTunerTestData = 0x5678;
			CSTCPrint("\n �ص�ʱ���ص�Ƶ���������Ƶʱ���ݵ�Ƶ�������һ��");
		}
	}
	else
	{
		CSTCPrint("\n�ص���ϢpsMessageΪ��!!!!\n");
	}
}

static void Tuner_CallbackTest4( CSUDITunerType_E eType, CSUDITunerCallbackMessage_S* psMessage)
{
	int nTunerId = 0;
    CSUDITunerSignalInfo_S s_TunerSignalInfo;

    memset(&s_TunerSignalInfo, 0, sizeof(s_TunerSignalInfo));
	if(CSUDI_SUCCESS != CSUDITunerGetSignalInfo(nTunerId, EM_UDITUNER_SIGNAL_QUALITY 
     	| EM_UDITUNER_SIGNAL_STRENGTH, &s_TunerSignalInfo))
    {
    	*(int*)(psMessage->m_pvUserData) = 0x1234;//CSUDITunerGetSignalInfoʧ�ܵ����
    }
    
    *(int*)(psMessage->m_pvUserData) = 0x5678;//�����˳�

    return;
}

static void Tuner_CallbackTest5(CSUDITunerType_E eType, CSUDITunerCallbackMessage_S *psMessage)
{	
	CSUDIOSThreadSleep(5000);

	if(psMessage != CSUDI_NULL)
	{
		switch(psMessage->m_eEvent)
		{
			case EM_UDITUNER_SIGNAL_CONNECTED:
				CSTCPrint("\nSignal has been connected\n");
				break;
			case EM_UDITUNER_SIGNAL_LOST:
				CSTCPrint("\nSignal has been lost\n");
				break;
			case EM_UDITUNER_SIGNAL_SEARCHING:
				CSTCPrint("\nSignal has been searching\n");
				break;
			default:
				break;
		}
	}
	else
	{
		CSTCPrint("\n�ص���ϢpsMessageΪ��!!!!\n");
	}

	g_bIsTunerConnected = CSUDI_TRUE;//������������Ǻ������أ�����ע��Ƶ���

    return;
}

static void Tuner_CallbackTest6( CSUDITunerType_E eType, CSUDITunerCallbackMessage_S* psMessage)
{
	CSUDI_UINT32 adwTunerIds = 0;
	
	CSTCPrint("before CSUDITunerDelCallback... \n");
	
	if(CSUDI_SUCCESS != CSUDITunerDelCallback(adwTunerIds, Tuner_CallbackTest6, &s_nUserData))
	{	
		s_bCallBackFlag = CSUDI_TRUE;
		
		CSTCPrint("call CSUDITunerDelCallback fail. \n");
	}
	
	CSTCPrint("after CSUDITunerDelCallback... \n");

	return;
}

static void Tuner_RDICallbackTest1( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData)
{
	int *pnEventData;
	
	pnEventData = (int *)pvEventData;

	switch(eType)
	{
		case EM_UDIRDI_EVENT_READY:
			s_bRDITunerIsReady = CSUDI_TRUE;
			s_dwRDITunerId = dwDeviceId;
			break;
		case EM_UDIRDI_EVENT_PLUGOUT:
			s_nRDIEventData = *pnEventData;
			break;
		default:
			break;
	}
}

CSUDI_BOOL CSTC_TUNER_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	CSTC_TUNER_InitCfg();
	CSTC_TUNER_Deliver_Init(s_unDeliver,sizeof(s_unDeliver)/sizeof(s_unDeliver[0]));
	CSTC_TUNER_TunerInfo_Init(s_asTunerInfo, sizeof(s_asTunerInfo)/sizeof(s_asTunerInfo[0]));
	CSTC_TUNER_TunerParam_Init(&s_nActTunerCnt, &s_nDeliverCount,&s_bIsSupportRDITuner);
	
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_TUNER_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:����pdwIdsΪ��ʱ�õ���ʵ���豸ID���Ƿ���ȷ
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ���ǰ��ʵ���豸ID��
//@INPUT:nMaxIdCnt = 50
//@EXPECTATION:�ɹ���ȡʵ���豸ID�������������ļ��е�ʵ��ID�����
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ��ʵ���豸ID����pdwIdsΪ�գ�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2���Ƚϻ�ȡ���豸ID���������ļ���ָ����ʵ���豸ID���Ƿ�һ�£���һ�����������ʧ��
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0001( void )
{
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(CSUDI_NULL,50,&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt == s_nActTunerCnt, "����2ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:���Բ���ȫ���Ϸ�ʱ�õ���ʵ���豸��ID�Ƿ���ȷ
//@PRECONDITION:1���豸��ʼ���ɹ���Ϊÿ���豸������ID
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ���ǰ��ʵ���豸ID��
//@INPUT:nMaxIdCnt = 20
//@EXPECTATION:�ɹ���ȡʵ���豸ID������ʵ�ʷ����ID���
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2���Ƚϻ�ȡ���豸ID���������ļ���ָ����ʵ���豸ID���Ƿ�һ�£���һ�����������ʧ��
//@EXECUTIONFLOW:3���Ƚϻ�ȡ��ID�Ƿ��ʵ���豸ID��ȣ���������������ʧ��
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt == s_nActTunerCnt, "����2ʧ��");

	for(i = 0 ; i < nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(adwTunerIds[i] == i, "����3ʧ��");
	}

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:���Բ���nMaxIdCntΪ0ʱ�ܷ�ɹ���ȡʵ���豸ID��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ���ǰ��ʵ���豸ID��
//@INPUT:nMaxIdCnt = 0
//@EXPECTATION:�ɹ���ȡʵ���豸ID�������������ļ��е�ʵ��ID�����
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ��ʵ���豸ID����pdwIdsΪ�գ�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2���Ƚϻ�ȡ���豸ID���������ļ���ָ����ʵ���豸ID���Ƿ�һ�£���һ�����������ʧ��
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0003( void )
{
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(CSUDI_NULL,0,&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt == s_nActTunerCnt, "����2ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:���Բ���nMaxIdCntΪ0ʱ�ܷ�ɹ���ȡʵ���豸��ID
//@PRECONDITION:1���豸��ʼ���ɹ���Ϊÿ���豸������ID
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ���ǰ��ʵ���豸ID��
//@INPUT:nMaxIdCnt = 0
//@EXPECTATION:�޷���ȡ�豸ID��ֻ�ܵõ�ID��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID��Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2���Ƚϻ�ȡ���豸ID���������ļ���ָ����ʵ���豸ID���Ƿ�һ�£���һ�����������ʧ��
//@EXECUTIONFLOW:3���ж�pdwIds�е���ֵ��Ҫ����Ϊ��ʼֵ(pdwIds[0]:11,pdwIds[1]:22)������δ�����豸ID�������������ʧ��
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0004( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[2] = {11,22};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,0,&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt == s_nActTunerCnt, "����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(adwTunerIds[0] == 11,"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(adwTunerIds[1] == 22,"����3ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:���Բ���pnActIdCntΪ��ʱ�ܷ�ɹ���ȡ��ǰʵ���豸ID��
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:nMaxIdCnt = 50
//@EXPECTATION:��ȡʧ�ܷ��ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ��ʵ���豸ID����Ҫ�󷵻ش������ֵ(��CSUDI_SUCCESS)
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0005( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerGetAllDeviceId(CSUDI_NULL,50,CSUDI_NULL), "����1ʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetDeviceInfo 
//@DESCRIPTION:���Բ���dwTunerId�Ƿ�ʱ�ܷ�ɹ���ȡ�豸��������Ϣ
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:dwTunerId = 0x1234
//@EXPECTATION:��ȡʧ�ܷ��ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDITunerGetDeviceInfo��ȡ�豸ID�Ƿ����豸������ϢҪ�󷵻ش������ֵ(��CSUDI_SUCCESS)
CSUDI_BOOL CSTC_TUNER_IT_GetDeviceInfo_0001( void )
{
	CSUDITunerInfo_S sTunerInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerGetDeviceInfo(0x1234,&sTunerInfo), "����1ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetDeviceInfo 
//@DESCRIPTION:���Բ���ȫ����ȷʱ�ܷ�ɹ���ȡ�����豸��������Ϣ
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ���ǰTuner��������Ϣ
//@INPUT:dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@EXPECTATION:��ȡ�ɹ�������������Ϣ��ȷ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerGetDeviceInfo��ȡ��ǰ�����豸��������Ϣ�������سɹ��������������ʧ��
//@EXECUTIONFLOW:3���Ƚϻ�ȡ�������豸��������Ϣ�������ļ���ָ����������Ϣ�Ƿ�һ�£���һ�������ʧ��
CSUDI_BOOL CSTC_TUNER_IT_GetDeviceInfo_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerInfo_S asTunerInfo[5];
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for (i = 0 ; i < nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(adwTunerIds[i],&asTunerInfo[i]), "����2ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSTC_TUNER_InfoCmp(asTunerInfo[i],s_asTunerInfo[i]),"����3ʧ��");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDITunerGetDeviceInfo 
//@DESCRIPTION:����psTunerInfoΪ��ʱ�ܷ�ɹ���ȡ�����豸��������Ϣ
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@EXPECTATION:��ȡʧ�ܷ��ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerGetDeviceInfo��ȡ��ǰ�����豸��������Ϣ������ʧ��
CSUDI_BOOL CSTC_TUNER_IT_GetDeviceInfo_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
		
	for (i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerGetDeviceInfo(adwTunerIds[i],CSUDI_NULL), "����2ʧ��");
	}
	
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:���Բ���ȫ���Ϸ�ʱ�ܷ�ע��ص��ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData = 0x88888888
//@EXPECTATION:ע��ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2�������е���ЧTuner Id����CSUDITunerAddCallbackע��ص���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect��ָ����Ƶ�������Ƶ������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4���ȴ�5����жϻص��л�ȡ���û����ݺʹ�����Ƿ�һ�£�һ���򷵻سɹ������򷵻�ʧ��
//@EXECUTIONFLOW:5������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0001( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{		
		s_nTunerTestData = 0;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest1,&nUserData), "����2ʧ��");

		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����3ʧ��");

		CSTCPrint("��ȴ�5��\n");

		CSUDIOSThreadSleep(5000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == 0x88888888,"����4ʧ��");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest1,&nUserData),"����5ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:����dwTunerId�Ƿ�ʱ�ܷ�ע��ص��ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId  = 0x1234
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData = 0x88888888
//@EXPECTATION:ע��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerAddCallbackע��ص���Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:2���������1���سɹ������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0002( void )
{
	int nUserData = 0x88888888;
	CSUDI_BOOL bHasJump = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerAddCallback(0x1234,Tuner_CallbackTest1,&nUserData), "����1ʧ��");

	bHasJump = CSUDI_FALSE;

	CSTK_FATAL_POINT
	{
		if(bHasJump)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(0x1234,Tuner_CallbackTest1,&nUserData),"����3ʧ��");
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:����fnTunerCallback�Ƿ�ʱ�ܷ�ע��ص��ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = CSUDI_NULL
//@INPUT:3��UserData = 0x88888888
//@EXPECTATION:ע��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص���Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:3���������1���سɹ������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	CSUDI_BOOL bHasJump[2] = {CSUDI_TRUE, CSUDI_TRUE};
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i = 0 ; i < nActIdCnt ; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerAddCallback(adwTunerIds[i],CSUDI_NULL,&nUserData), "����2ʧ��");

		bHasJump[i] = CSUDI_FALSE;
	}
	
	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nActIdCnt ; i++)
		{
			if(bHasJump[i])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],CSUDI_NULL,&nUserData),"����3ʧ��");
			}
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:����UserDataΪ��ʱ�ܷ�ע��ص��ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData = CSUDI_NULL
//@EXPECTATION:ע��ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect��ָ����Ƶ�������Ƶ������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4���ڻص����ж�UserData�����ΪNull��s_nTunerTestData��ֵΪ0x1234
//@EXECUTIONFLOW:5���ȴ�5����ж�s_nTunerTestData�Ƿ�Ϊ0x1234��������򷵻سɹ������򷵻�ʧ��
//@EXECUTIONFLOW:6������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0004( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i = 0 ; i < nActIdCnt ; i++)
	{
		s_nTunerTestData = 0;
				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest1,CSUDI_NULL), "����2ʧ��");

		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����3ʧ��");

		CSTCPrint("��ȴ�5��\n");

		CSUDIOSThreadSleep(5000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == 0x1234,"����5ʧ��");
	}
	
	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest1,CSUDI_NULL),"����6ʧ��");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:���Բ���ȫ���Ϸ�ʱΪͬһTunerע��8���ص������ܷ�ע��ص��ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData = anUserData[i]
//@EXPECTATION:ȫ��ע��ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallback����ע��8���ص�������ע��ĺ��������ͬ��UserData��ͬ��Ҫ��ȫ�����سɹ��������������ʧ��
//@EXECUTIONFLOW:3������CSUDITunerDelCallbackɾ��ע��ɹ��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0005( void )
{
	int nActIdCnt;
	CSUDI_UINT32 adwTunerIds[20];
	int anUserData[8] = {0};
	int nAddCallbackCount = 0;
	int nIdCount = 0;
	int nLoopCount = 0;
	int i,j;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	for(i=0 ; i < nActIdCnt ; i++)
	{
		nIdCount++;
		
		for(j = 0 ; j < 8 ; j++)
		{			
			anUserData[j] = i * 10 + j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest1,&anUserData[j]), "����2ʧ��");

			nAddCallbackCount++;
		}
	}
	
	CSTK_FATAL_POINT
	{
		if(nIdCount >0)
		{
			for(i=0 ; i<nIdCount ; i++)
			{
				nLoopCount = nAddCallbackCount - 8 * i > 8 ? 8 : nAddCallbackCount - 8 * i;
				
				for(j = 0 ; j < nLoopCount ; j++)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest1,&anUserData[j]),"����3ʧ��");
				}
			}
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:����ע���Ѿ�ע��Ļص��ܷ�ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData = nUserData
//@EXPECTATION:ע��ʧ�ܷ���CSUDITUNER_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallback��ͬһ�ص�����ע�����Σ�Ҫ���һ�η��سɹ����ڶ��η���CSUDITUNER_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW:3������CSUDITunerDelCallbackɾ��ע��ɹ��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0006( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "��һ��ע��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_ALREADY_ADDED == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "����2ʧ��");

	CSTK_FATAL_POINT
	{
		if(nActIdCnt > 0)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"����3ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:����ע��dwTunerId��UserData����ͬ��fnTunerCallback��ͬ�Ļص��ܷ�ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1/Tuner_CallbackTest2
//@INPUT:3��UserData = nUserData
//@EXPECTATION:����ע��ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��fnTunerCallback = Tuner_CallbackTest1�Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerAddCallback��ͬһdwTunerIdע��fnTunerCallback = Tuner_CallbackTest2�Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ��ע��ɹ��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0007( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "����2ʧ��");

	nAddCallbackCount++;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData), "����3ʧ��");

	nAddCallbackCount++;

	CSTK_FATAL_POINT
	{
		if(nAddCallbackCount >= 2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"����4ʧ��");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData),"����4ʧ��");
		}
		else if(nAddCallbackCount >= 1)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"����4ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:��������Ƶ�ص��л�ȡ�ź���Ϣ�Ƿ������쳣(��������)
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest4
//@INPUT:3��UserData = nUserData
//@EXPECTATION:�����������쳣
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��fnTunerCallback = Tuner_CallbackTest4�Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect����һ��Ƶ�㣬��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������ص������е���CSUDITunerGetSignalInfo��ȡ�ź���Ϣ��������ȡ�ɹ����һص������˳�
//@EXECUTIONFLOW:5������CSUDITunerDelCallbackɾ��ע��ɹ��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0008( void )
{
    int nActIdCnt = 0;
    CSUDI_UINT32 adwTunerIds[20];
    int nUserData = 0x1111;
    int nAddCallbackCount = 0;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");

    CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0], Tuner_CallbackTest4, &nUserData), "����2ʧ��");
    
    nAddCallbackCount++;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "����3ʧ��");

	CSTCPrint("��ȴ�2s ...\n");
    
	CSUDIOSThreadSleep(2000);
    
    CSTK_ASSERT_TRUE_FATAL(0x5678 == nUserData, "����4ʧ��");

    CSTK_FATAL_POINT
    {
        if(nAddCallbackCount >= 1)
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0], Tuner_CallbackTest4, &nUserData), "����5ʧ��");
        }
    }

    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:��������Ƶ�ص���ɾ���ص���������
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest4
//@INPUT:3��UserData = nUserData
//@EXPECTATION:�����������쳣
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��Tuner_CallbackTest5�Ļص��������ú�����ɾ���ص�����
//@EXECUTIONFLOW:3������CSUDITunerConnect����һ��Ƶ�㣬��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0009( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	
	CSTCPrint("���������ܻ�����쳣(��������������)�����ȴ�5������û�н�����������ʧ��\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0], Tuner_CallbackTest6, &s_nUserData), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "����3ʧ��");

	CSTCPrint("��ȴ�2s ...\n");
	
	CSUDIOSThreadSleep(2000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE != s_bCallBackFlag, "�ص��У�����CSUDITunerDelCallbackʧ��!");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:����dwTunerId�Ƿ�ʱ�ܷ�ɾ���ص��ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId  = 0x1234
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData = 0x88888888
//@EXPECTATION:ɾ��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerDelCallbackɾ���豸ID�Ƿ��Ļص�������Ҫ�󷵻�ʧ��
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0001( void )
{
	int nUserData = 0x88888888;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(0x1234,Tuner_CallbackTest1,&nUserData),"����1ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:����fnTunerCallback�Ƿ�ʱ�ܷ�ɾ���ص��ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = CSUDI_NULL
//@INPUT:3��UserData = 0x88888888
//@EXPECTATION:ɾ��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerDelCallbackɾ��fnTunerCallback�Ƿ��Ļص�������Ҫ�󷵻�ʧ��
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0002( void )
{
	int nUserData = 0x88888888;
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(adwTunerIds[0],CSUDI_NULL,&nUserData),"����2ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:����ɾ��û��ע��Ļص������Ƿ�ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData = 0x88888888
//@EXPECTATION:ɾ��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerDelCallbackɾ��û��ע��Ļص�����Tuner_CallbackTest1��Ҫ�󷵻�CSUDI_FAILURE
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0003( void )
{
	int nUserData = 0x88888888;
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"����2ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:����ɾ��UserData��ע��ʱ��һ�µĻص������Ƿ�ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData1 = 0x88888888, UserData2 = 0x11111111
//@EXPECTATION:ɾ��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص���UserDataΪUserData1��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerDelCallbackɾ��ע��Ļص�����Tuner_CallbackTest1��UserDataΪUserData2��Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ��ע��Ļص�����Tuner_CallbackTest1��UserDataΪUserData1��Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0004( void )
{
	int nUserData1 = 0x88888888;
	int nUserData2 = 0x11111111;
	int nActIdCnt = 0;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData1), "����2ʧ��");
	
	nAddCallbackCount++;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData2),"����3ʧ��");

	CSTK_FATAL_POINT
	{
		if (nAddCallbackCount != 0)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData1),"����4ʧ��");
		}
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:����ɾ���Ѿ�ɾ���Ļص������Ƿ�ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData = 0x88888888
//@EXPECTATION:ɾ��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�����Tuner_CallbackTest1��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerDelCallbackɾ��ע��Ļص�����Tuner_CallbackTest1��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallback�ٴ�ɾ��ע��Ļص�����Tuner_CallbackTest1��Ҫ�󷵻�ʧ��
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0005( void )
{
	int nUserData = 0x88888888;
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"����3ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"����4ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:���Զ�����֮��ע���ɾ���ص�����
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1��Tuner_CallbackTest2
//@INPUT:3��UserData = 0
//@EXPECTATION:ÿһ������������ֵ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDIOSThreadCreate��������������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3����������1�е���CSUDITunerAddCallbackΪTuner0ע��ص�����1���ȴ�2������CSUDITunerDelCallbackɾ��Tuner0�Ѿ�ע��Ļص�����2��Ҫ��ȫ���ɹ�
//@EXECUTIONFLOW:4����������2�е���CSUDITunerAddCallbackΪTuner0ע��ص�����2���ȴ�2������CSUDITunerDelCallbackɾ��Tuner0�Ѿ�ע��Ļص�����1��Ҫ��ȫ���ɹ�
//@EXECUTIONFLOW:5��ɾ������������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_AddDelCallback_0001( void )
{	
	int nUserData1 = 0x88888888;
	int nUserData2 = 0x11111111; 
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;

	TunerCallbackParam_S asTunerCallbackParam[2];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	asTunerCallbackParam[0].dwTunerId = adwTunerIds[0];
	asTunerCallbackParam[0].fnTunerCallback = Tuner_CallbackTest1;
	asTunerCallbackParam[0].pvUserData = (int*)(&nUserData1);
	asTunerCallbackParam[0].bThreadFlag = CSUDI_FALSE;

	asTunerCallbackParam[1].dwTunerId = adwTunerIds[0];
	asTunerCallbackParam[1].fnTunerCallback = Tuner_CallbackTest2;
	asTunerCallbackParam[1].pvUserData = (int*)(&nUserData2);
	asTunerCallbackParam[1].bThreadFlag = CSUDI_FALSE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread1_1", 64, 8*1024, Tuner_TestThread1_1Entry, 
								asTunerCallbackParam, &hThreadhandle1),
								"����2����������1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread1_2", 64, 8*1024, Tuner_TestThread1_2Entry, 
								asTunerCallbackParam, &hThreadhandle2),
								"����2����������2ʧ��");

	CSUDIOSThreadSleep(3000);

	CSTK_ASSERT_TRUE_FATAL(asTunerCallbackParam[0].bThreadFlag == CSUDI_TRUE, "������1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(asTunerCallbackParam[1].bThreadFlag == CSUDI_TRUE, "������1ʧ��");
	
	CSTK_FATAL_POINT
	{
		if(hThreadhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ��������1ʧ��");
			hThreadhandle1 = CSUDI_NULL;
		}
		if(hThreadhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ��������2ʧ��");
			hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDITunerAddCallback
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:�ص�������ע���ɾ��ǿ�Ȳ���
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3��UserData = 0x88888888
//@EXPECTATION:ÿһ������������ֵ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�����Tuner_CallbackTest1��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerDelCallbackɾ��ע��Ļص�����Tuner_CallbackTest1��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4���ظ�ִ��2��3��TESTCOUNT��(TESTCOUNT���ݾ����������)
CSUDI_BOOL CSTC_TUNER_IT_AddDelCallback_0002( void )
{
	int nUserData = 0x88888888;
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];  
	int i;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	for(i=0 ; i<TEST_COUNT; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "����2ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"����3ʧ��");
	}

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:����dwTunerId�Ƿ�ʱ��ָ��Ƶ�������Ƶ�ܷ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@INPUT:1��dwTunerId  = 0x1234
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:��Ƶʧ��
//@EXECUTIONFLOW:1������CSUDITunerConnect��ָ��Ƶ�������Ƶ����������ʧ��
CSUDI_BOOL CSTC_TUNER_IT_Connect_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerConnect(0x1234,&s_unDeliver[0]),"����1ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:����punDeliverΪ��ʱ��ָ��Ƶ�������Ƶ�ܷ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :CSUDI_NULL
//@EXPECTATION:��Ƶʧ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect������Ƶ������Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5������dwTunerId��ִ�в���2~4
CSUDI_BOOL CSTC_TUNER_IT_Connect_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for (i=0 ; i<nActIdCnt ; i++)
	{
             if(adwTunerIds[i] != -1)
             { 
                CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest1,&nUserData), "����2ʧ��");

		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerConnect(adwTunerIds[i], CSUDI_NULL),"����3ʧ��");
             }
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest1,&nUserData),"����6ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Բ�������ʱ��ָ��Ƶ�������Ƶ�ܷ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:��Ƶ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3���ڻص�����������жϵ���Ƶ�ɹ���s_nTunerTestData��Ϊ1001
//@EXECUTIONFLOW:4������CSUDITunerConnect������Ƶ������Ҫ�󷵻سɹ����һص���Ϣ��EM_UDITUNER_SIGNAL_CONNECTED������s_nTunerTestData��Ϊ1001
//@EXECUTIONFLOW:5���ȴ�5����ж�s_nTunerTestData��ֵ��Ҫ��Ϊ1001�������������ʧ��
//@EXECUTIONFLOW:6���ж�s_bIsSignalLost��s_bIsSignalSearching�Ƿ�ΪCSUDI_FALSE�������ǣ����򷵻�ʧ��
//@EXECUTIONFLOW:7������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:8������dwTunerId��ִ�в���2~6
CSUDI_BOOL CSTC_TUNER_IT_Connect_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;
			s_bIsSignalLost = CSUDI_FALSE;
            
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����4ʧ��");

			CSTCPrint("��ȴ�5��\n");
            
            CSUDIOSThreadSleep(5000);

            CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "����5ʧ��");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == s_bIsSignalLost, "����6ʧ�ܣ���ʱ��Ӧ����EM_UDITUNER_SIGNAL_LOST��Ϣ����");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == s_bIsSignalSearching, "����6ʧ�ܣ���ʱ��Ӧ����EM_UDITUNER_SIGNAL_SEARCHING��Ϣ����");
		}
	} 

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����7ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:����Ƶ��Ƿ�ʱ(����47M~867M��Χ��DVB-C��׼)������Ƶ�ܷ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ƿ���Ƶ����Ϣ(Ƶ��Ϊ1000KHz)
//@EXPECTATION:��Ƶʧ�ܲ����ط�CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect������Ƶ������Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5������dwTunerId��ִ�в���2~4
CSUDI_BOOL CSTC_TUNER_IT_Connect_0004( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	CSUDITunerSRCDeliver_U unDeliver;
	int i;

	memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));

	unDeliver.m_sCableDeliver.m_uFrequency = 1000;
	unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8;
	unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_BAD_PARAMETER == CSUDITunerConnect(adwTunerIds[i], &unDeliver),"����3ʧ��");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����4ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:����Ƶ����Чʱ������Ƶ�ܷ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :��Ч��Ƶ����Ϣ(Ƶ��Ϊ44400KHz)
//@EXPECTATION:��Ƶʧ���һص�һֱ�յ�EM_UDITUNER_SIGNAL_SEARCHING��Ϣ��CSUDITunerConnect�ķ���ֵӦ����CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ����һص���Ϣ��EM_UDITUNER_SIGNAL_SEARCHING������s_nTunerTestData��Ϊ3003
//@EXECUTIONFLOW:3������CSUDITunerConnect������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS�����޷���ס��Ƶ�㣬�ص���Ϣһֱ��EM_UDITUNER_SIGNAL_SEARCHING
//@EXECUTIONFLOW:4���ȴ�5����ж�s_nTunerTestData��ֵ��Ҫ��Ϊ3003�������������ʧ��
//@EXECUTIONFLOW:5���ж�s_bIsSignalLost�Ƿ�ΪCSUDI_FALSE������ΪCSUDI_FALSE����û��LOST��Ϣ����
//@EXECUTIONFLOW:6������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7������dwTunerId��ִ�в���2~5
CSUDI_BOOL CSTC_TUNER_IT_Connect_0005( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	CSUDITunerSRCDeliver_U unDeliver;
	int i;

	memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));
	
	unDeliver.m_sCableDeliver.m_uFrequency = s_unDeliver[0].m_sCableDeliver.m_uFrequency + 3000;
	unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8;
	unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;	
			s_bIsSignalLost = CSUDI_FALSE;
            
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver),"����3ʧ��");

			CSTCPrint("��ȴ�5��\n");

			CSUDIOSThreadSleep(5000);
                  
			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING,"����4ʧ��");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == s_bIsSignalLost, "����5ʧ�ܣ���ʱ��Ӧ����EM_UDITUNER_SIGNAL_LOST��Ϣ����");
        }
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����4ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Բ�������ʱ����ź��߶���Ƶ��Ӱ��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:�����ź�����Ƶ�ɹ����ε���Ƶʧ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3���ڻص�����������жϵ���Ƶ�ɹ���s_nTunerTestData��Ϊ1001���źŶ�ʧ��s_bIsSignalLost��false��Ϊtrue��SEARCHING��s_nTunerTestData��Ϊ3003
//@EXECUTIONFLOW:4������CSUDITunerConnect������Ƶ������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5���ȴ�2����ж�s_nTunerTestData��ֵ��Ҫ��Ϊ1001�������������ʧ��
//@EXECUTIONFLOW:6����ʾ������Ա�ε��ź��߲��۲��Ƿ���1�������źŶ�ʧ�Ĵ�ӡ������'Y'����������'N'����������ʧ��
//@EXECUTIONFLOW:7���ж�s_bIsSignalLost��ֵ��Ҫ��Ϊtrue�������������ʧ��
//@EXECUTIONFLOW:8���ȴ�500ms���ж�s_nTunerTestData��ֵ��Ҫ��Ϊ3003������s_bIsSignalSearching��ΪCSUDI_FALSE
//@EXECUTIONFLOW:9����ʾ�����ź��߲��۲��Ƿ���1��������Ƶ�ɹ��Ĵ�ӡ������'Y'����������'N'����������ʧ��
//@EXECUTIONFLOW:10���ж�s_bIsSignalSearching��ֵ��Ҫ��ΪCSUDI_FALSE�������������ʧ��
//@EXECUTIONFLOW:11������dwTunerId��ִ�в���2~11
//@EXECUTIONFLOW:12������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0006( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;
		
			s_bIsSignalLost = CSUDI_FALSE;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����4ʧ��");

			CSTCPrint("��ȴ�2��\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"����5ʧ��");

			s_nTunerTestData = 0;

			CSTCPrint("\n��ε��ź��߲���ϸ�۲죬����1�������źŶ�ʧ�Ĵ�ӡ��'Y'������'N'\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"����6ʧ��");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSignalLost,"����7ʧ��");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING, "����8ʧ��");

			CSUDIOSThreadSleep(5000);

			CSTCPrint("\n��ȷ���ڴ��ڼ��ж���ź��������ӵĴ�ӡ\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"����6ʧ��");


			CSTCPrint("\n������ź��߲���ϸ�۲죬����1��������Ƶ�ɹ��Ĵ�ӡ��'Y'������'N'\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"����9ʧ��");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == s_bIsSignalSearching, "����10����ʱ��Ӧ����TUNER_SIGNAL_SEARCHING��Ϣ");
			
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����12ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Զ�ͬһƵ�����������Ƶ�����ܷ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:������Ƶ������CSUDI_SUCCESS����Ƶ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3���ڻص�����������жϵ���Ƶ�ɹ���s_nTunerTestData��Ϊ1001
//@EXECUTIONFLOW:4������CSUDITunerConnect��ָ��Ƶ�������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:5������CSUDITunerConnectͬһƵ������ٴ���Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:6��������Tuner��ִ�в���2~5
//@EXECUTIONFLOW:7������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0007( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����4ʧ��");

			CSTCPrint("��ȴ�2��\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"����4ʧ��");

			s_nTunerTestData = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����5ʧ��");

			CSTCPrint("��ȴ�2��\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"����5ʧ��");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����6ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Զ�������ͬƵ�������Ƶ�����ܷ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:������Ƶ������CSUDI_SUCCESS����Ƶ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3���ڻص�����������жϵ���Ƶ�ɹ���s_nTunerTestData��Ϊ1001
//@EXECUTIONFLOW:4������CSUDITunerConnect��ָ��Ƶ��1������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:5����¼�µ�һ����Ƶ����Ҫ��ʱ�䣬Ҫ��С��800ms
//@EXECUTIONFLOW:6������CSUDITunerConnect��ָ��Ƶ��2������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:7����¼�µڶ�����Ƶ����Ҫ��ʱ�䣬Ҫ��С��800ms
//@EXECUTIONFLOW:8��������Tuner��ִ�в���2~7
//@EXECUTIONFLOW:9������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0008( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	
	CSUDIOSTimeVal_S s_time1;
	CSUDIOSTimeVal_S s_time2;
	int nMiliSecond= 0;

	int nSleepCount = 0;
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;

			nSleepCount = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[1]),"����4ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&s_time1), "��ȡʱ��ʧ��");

			while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
			{
				CSUDIOSThreadSleep(10);

				if(nSleepCount++ > 200)
				{
					break;
				}
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&s_time2), "��ȡʱ��ʧ��");

			nMiliSecond = (s_time2.m_nSecond - s_time1.m_nSecond)*1000 + (s_time2.m_nMiliSecond - s_time1.m_nMiliSecond);

			CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 800, "����5ʧ��");

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"��һ����Ƶʧ��");

			s_nTunerTestData = 0;

			nSleepCount = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[2]),"����6ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&s_time1), "��ȡʱ��ʧ��");

			while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
			{
				CSUDIOSThreadSleep(10);

				if(nSleepCount++ > 200)
				{
					break;
				}
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&s_time2), "��ȡʱ��ʧ��");

			nMiliSecond = (s_time2.m_nSecond - s_time1.m_nSecond)*1000 + (s_time2.m_nMiliSecond - s_time1.m_nMiliSecond);

			CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 800, "����7ʧ��");

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"�ڶ�����Ƶʧ��");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����6ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Զ�������ͬƵ����п�����Ƶ�����ܷ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:������Ƶ������CSUDI_SUCCESS����Ƶ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect��ָ��Ƶ��1������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:4�����ȴ�ֱ�ӵ���CSUDITunerConnect��ָ��Ƶ��2������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:5��������Tuner��ִ�в���2~4
//@EXECUTIONFLOW:6������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0009( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;

	int nSleepCount = 0;
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;

			nSleepCount = 0;
	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����4ʧ��");

			s_nTunerTestData = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[1]),"����6ʧ��");

			while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
			{
				CSUDIOSThreadSleep(10);

				if(nSleepCount++ > 200)
				{
					break;
				}
			}
		
			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"�ڶ�����Ƶʧ��");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����6ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Զ�һ��Ƶ����������������Ҫ��ʱ��͸���Ƶ���ƽ������ʱ��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:����ƽ������ʱ��<500ms
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect��һ��Ƶ������������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:4����¼��ȫ������ʱ��
//@EXECUTIONFLOW:5���������Ƶ��ƽ������ʱ�䲢Ҫ����500ms����
//@EXECUTIONFLOW:6������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0010( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;

	CSUDIOSTimeVal_S sTime1;
	CSUDIOSTimeVal_S sTime2;
	int anMiliSecond[10] = {0};
	int anAverageMs[10] = {0};
	CSUDIOSTimeVal_S sTimeStart[FREQUENCY_NUM_MAX] = {{0}};  /*Ƶ����������Ϊ20*/
	CSUDIOSTimeVal_S sTimeEnd[FREQUENCY_NUM_MAX] = {{0}};
	BOOL bConnetcRet[FREQUENCY_NUM_MAX] = {0};
    
	int nSleepCount = 0;
	int nConnectRtnFaildCnt = 0;
	int nConnectFaildCnt = 0;
	int nConnectSuccessCnt = 0;
	int i,j;

	CSTCPrint("�����ӱ���ʵ����Ӫ�̵�����,��ȷ��Ƶ����Ϣ����!ȷ�ϰ�Y,����N.\n");
    CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes()," ��Ϣ����! ");
    
	CSTK_ASSERT_TRUE_FATAL(s_nDeliverCount >= 12, "�������Ĳ���������Ҫ12��Ƶ����Ϣ�������������������ļ�");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	CSTK_ASSERT_TRUE_FATAL(s_nDeliverCount < FREQUENCY_NUM_MAX,"���Ե�Ƶ�����������Ԥ������ֵ50");

	
	for (i = 0 ; i < nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount++;

        		CSUDIOSGetTime(&sTime1);

        		for(j=0 ; j<s_nDeliverCount-1 ; j++)
        		{
        			s_nTunerTestData = 0;

        			nSleepCount = 0;

                    		CSUDIOSGetTime(&sTimeStart[j]);
        			if(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[j]))
        			{			
        				while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
        				{
        					CSUDIOSThreadSleep(10);

        					if(nSleepCount++ > 200)
        					{
        						break;
        					}
        				}
        				if(s_nTunerTestData == TUNER_SIGNAL_CONNECTED)
        				{
        					nConnectSuccessCnt++;
        					bConnetcRet[j] = TRUE;
        				}
        				else
        				{
        					nConnectFaildCnt++;
        					bConnetcRet[j] = FALSE;
        				}
        			}
        			else
        			{
        				nConnectRtnFaildCnt++;
        			}
        			
        			CSUDIOSGetTime(&sTimeEnd[j]);
        		}
        		
        		CSUDIOSGetTime(&sTime2);
        		
        		anMiliSecond[i] = (sTime2.m_nSecond - sTime1.m_nSecond)*1000 + (sTime2.m_nMiliSecond - sTime1.m_nMiliSecond);

        		anAverageMs[i] = anMiliSecond[i]/(s_nDeliverCount-1);
                	CSTCPrint("\n ��������������ƵƵ�����Ϊ��CS_TUNER_ALL_DELIVER_COUNT-1��===%d\n",s_nDeliverCount-1);
                	CSTCPrint("\n ������������Ƶ�ɹ���Ƶ�����Ϊ%d\n",nConnectSuccessCnt);

			for(j=0; j<s_nDeliverCount-1; j++)
			{
				anMiliSecond[i] = (sTimeEnd[j].m_nSecond - sTimeStart[j].m_nSecond)*1000 + (sTimeEnd[j].m_nMiliSecond - sTimeStart[j].m_nMiliSecond);

				CSTCPrint("Ƶ������: %2d����Ƶ���: %d,Ƶ��: %d, ��Ƶʱ��: %d ms\n",j,bConnetcRet[j],s_unDeliver[j].m_sCableDeliver.m_uFrequency,anMiliSecond[i]);
			}
                
			CSTK_ASSERT_TRUE_FATAL((s_nDeliverCount-1) == nConnectSuccessCnt, "��Ƶ�����г���ʧ��");
                  
                	CSTCPrint("\n ƽ����Ƶʱ�� ===%d********",anAverageMs[i]);
			CSTK_ASSERT_TRUE_FATAL(anAverageMs[i] < 500, "����5ʧ��");
		}
	}


	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����6ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Զ�������ͬһTuner��ͬһƵ����з�����Ƶ��ϵͳ�ܷ񱣳��ȶ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:ϵͳ��Ȼ�����ȶ�������Ƶ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate��������������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4�����������ж�ͬһTuner��ͬһƵ����з�����Ƶ10�Σ�ͬһ����������Ƶ֮��ȴ�һ��10��500ms�����ʱ�䣬Ҫ��ȫ������CSUDI_SUCCESS��Ȼ�����з���
//@EXECUTIONFLOW:5���������������񷵻غ����CSUDITunerConnect��ͬһƵ�������ƵҪ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:6�����ٴ���������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0011( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0; 
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	int nSleepCount = 0;

	g_bIsThreadNaturalEndFlag1 = CSUDI_FALSE;
	g_bIsThreadNaturalEndFlag2 = CSUDI_FALSE;
	s_nTunerTestData = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	if(adwTunerIds[0] != -1)
	{ 
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

		nAddCallbackCount ++;
        	
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread2_1", 64, 8*1024, Tuner_TestThread2_1Entry, 
        								&adwTunerIds[0], &hThreadhandle1), "����������1ʧ��");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread2_2", 64, 8*1024, Tuner_TestThread2_2Entry, 
        								&adwTunerIds[0], &hThreadhandle2), "����������2ʧ��");

        	CSTCPrint("\n��ȴ�10�롣����\n");

		while(!g_bIsThreadNaturalEndFlag1 && !g_bIsThreadNaturalEndFlag2)
		{
        		CSUDIOSThreadSleep(3000);

        		if(nSleepCount++ > 100)
        		{
        			break;
        		}
        	}

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag1, "����4��������1������Ƶʧ��");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag2, "����4��������2������Ƶʧ��");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "����5ʧ��");

        	CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "����5ʧ��");
            
        }
        CSTK_FATAL_POINT
        {
        	if(nAddCallbackCount > 0)
        	{
        		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0], Tuner_CallbackTest2, &nUserData), "ɾ���ص�ʧ��");
        	}
        	if(hThreadhandle1 != CSUDI_NULL)
        	{
        		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ��������1ʧ��");
        		hThreadhandle1 = CSUDI_NULL;
        	}
        	if(hThreadhandle2 != CSUDI_NULL)
        	{
        		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ��������2ʧ��");
        		hThreadhandle2 = CSUDI_NULL;
        	}
        }
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Զ�������ͬһTuner�Ĳ�ͬƵ����з�����Ƶ��ϵͳ�ܷ񱣳��ȶ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:ϵͳ��Ȼ�����ȶ�������Ƶ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate��������������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4�����������зֱ��ͬһTuner�Ĳ�ͬƵ����з�����Ƶ10�Σ�ͬһ����������Ƶ֮��ȴ�һ��10��500ms�����ʱ�䣬Ҫ��ȫ������CSUDI_SUCCESS��Ȼ�����з���
//@EXECUTIONFLOW:5���������������񷵻غ����CSUDITunerConnect��ͬһƵ�������ƵҪ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:6�����ٴ���������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0012( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	int nSleepCount = 0;

	g_bIsThreadNaturalEndFlag1 = CSUDI_FALSE;
	g_bIsThreadNaturalEndFlag2 = CSUDI_FALSE;
	s_nTunerTestData = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	if(adwTunerIds[0] != -1)
	{ 
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

		nAddCallbackCount ++;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread3_1", 64, 8*1024, Tuner_TestThread2_1Entry, 
								&adwTunerIds[0], &hThreadhandle1), "����������1ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread3_2", 64, 8*1024, Tuner_TestThread3Entry, 
								&adwTunerIds[0], &hThreadhandle2), "����������2ʧ��");
        
		CSTCPrint("\n��ȴ�10�롣����\n");

		while(!g_bIsThreadNaturalEndFlag1 && !g_bIsThreadNaturalEndFlag2)
		{
			CSUDIOSThreadSleep(3000);

			if(nSleepCount++ > 100)
			{
				break;
			}
		}
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag1, "����4��������1������Ƶʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag2, "����4��������2������Ƶʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "����5ʧ��");
        
	}

        CSTK_FATAL_POINT
        {
           if(nAddCallbackCount > 0)
           {
        	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0], Tuner_CallbackTest2, &nUserData), "ɾ���ص�ʧ��");
            }
            if(hThreadhandle1 != CSUDI_NULL)
            {
        	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ��������1ʧ��");
        	    hThreadhandle1 = CSUDI_NULL;
            }
            if(hThreadhandle2 != CSUDI_NULL)
            {
        	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ��������2ʧ��");
        	    hThreadhandle2 = CSUDI_NULL;
            }
         }         
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Զ������Բ�ͬTuner������Ƶ�ܷ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:����CSUDI_SUCCESS���ɹ���Ƶ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDIOSThreadCreate��������������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4����������1�ж�Tuner0������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͬ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:5����������2�ж�Tuner1������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͬ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:6�����ٴ���������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0013( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	
	int nSleepCount = 0;
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(s_nActTunerCnt >= 2, "ϵͳTuner������Ϊ2��ʱ����ִ�б�����");

	g_bIsThreadNaturalEndFlag1 = CSUDI_FALSE;
	g_bIsThreadNaturalEndFlag2 = CSUDI_FALSE;
	g_bIsTunerConnected = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<2 ; i++)
	{    
		if(adwTunerIds[i] != -1)
		{ 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount ++;
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread3_1", 64, 8*1024, Tuner_TestThread4_1Entry, 
								&adwTunerIds[0], &hThreadhandle1), "����������1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread3_2", 64, 8*1024, Tuner_TestThread4_2Entry, 
								&adwTunerIds[1], &hThreadhandle2), "����������2ʧ��");

	CSTCPrint("\n��ȴ�15�롣����\n");

	while(!g_bIsThreadNaturalEndFlag1 && !g_bIsThreadNaturalEndFlag2)
	{
		CSUDIOSThreadSleep(100);

		if(nSleepCount++ > 150)
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag1, "����4��������1������Ƶʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag2, "����5��������2������Ƶʧ��");

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "ɾ���ص�ʧ��");
		}
		if(hThreadhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "ɾ��������1ʧ��");
			hThreadhandle1 = CSUDI_NULL;
		}
		if(hThreadhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "ɾ��������2ʧ��");
			hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:Ƶ��ƫ�Ʋ���
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :��ȷ��Ƶ�㲢����ƫ��150KHz
//@EXPECTATION:��Ƶ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5������dwTunerId��ִ�в���2~4
CSUDI_BOOL CSTC_TUNER_IT_Connect_0014( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	CSUDITunerSRCDeliver_U unDeliver;
	int i;
	
	s_nTunerTestData = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<nActIdCnt ; i++)
	{	
		if(adwTunerIds[i] != -1)
		{ 
			memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));

			unDeliver.m_sCableDeliver.m_uFrequency = s_unDeliver[0].m_sCableDeliver.m_uFrequency - 150;
			unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8;
			unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "����2ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "��Ƶ������ƫ��150KHzʱ����3ʧ��");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "��Ƶ������ƫ��150KHzʱ��Ƶʧ��");

			memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));

			unDeliver.m_sCableDeliver.m_uFrequency = s_unDeliver[0].m_sCableDeliver.m_uFrequency + 150;
			unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8;
			unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24;

			s_nTunerTestData = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "��Ƶ������ƫ��150KHzʱ����3ʧ��");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "��Ƶ������ƫ��150KHzʱ��Ƶʧ��");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����4ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:������ƫ�Ʋ���
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :��ȷ��Ƶ�㲢������������ƫ��2~10Kbps
//@EXPECTATION:��Ƶ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5������dwTunerId��ִ�в���2~4
CSUDI_BOOL CSTC_TUNER_IT_Connect_0015( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	CSUDITunerSRCDeliver_U unDeliver;
	int i,j;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<nActIdCnt ; i++)
	{	
		if(adwTunerIds[i] != -1)
		{ 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "����2ʧ��");

			nAddCallbackCount++;

			for(j=2 ; j<=10 ; j+=2)
			{	
				s_nTunerTestData = 0;

				memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));

				unDeliver.m_sCableDeliver.m_uFrequency = s_unDeliver[0].m_sCableDeliver.m_uFrequency;
				unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24 - j;
				unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8 ;

				if(j == 2)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��2Kbpsʱʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��2Kbpsʱ��Ƶʧ��");
				}
				else if(j == 4)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��2Kbpsʱ�ɹ���4Kbpsʱʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��2Kbpsʱ��Ƶ�ɹ���4Kbpsʱʧ��");
				}
				else if(j == 6)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��4Kbpsʱ�ɹ���6Kbpsʱʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��4Kbpsʱ��Ƶ�ɹ���6Kbpsʱʧ��");
				}
				else if(j == 8)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��6Kbpsʱ�ɹ���8Kbpsʱʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��6Kbpsʱ��Ƶ�ɹ���8Kbpsʱʧ��");
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��8Kbpsʱ�ɹ���10Kbpsʱʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��8Kbpsʱ��Ƶ�ɹ���10Kbpsʱʧ��");
				}	

				memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));

				unDeliver.m_sCableDeliver.m_uFrequency = s_unDeliver[0].m_sCableDeliver.m_uFrequency;
				unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24 + j;
				unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8 ;

				s_nTunerTestData = 0;
			
				if(j == 2)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��2Kbpsʱ����3ʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��2Kbpsʱ��Ƶʧ��");
				}
				else if(j == 4)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��2Kbpsʱ�ɹ���4Kbpsʱʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��2Kbpsʱ��Ƶ�ɹ���4Kbpsʱʧ��");
				}
				else if(j == 6)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��4Kbpsʱ�ɹ���6Kbpsʱʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��4Kbpsʱ��Ƶ�ɹ���6Kbpsʱʧ��");
				}
				else if(j == 8)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��6Kbpsʱ�ɹ���8Kbpsʱʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��6Kbpsʱ��Ƶ�ɹ���8Kbpsʱʧ��");
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "������������ƫ��8Kbpsʱ�ɹ���10Kbpsʱʧ��");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "������������ƫ��8Kbpsʱ��Ƶ�ɹ���10Kbpsʱʧ��");
				}	
			}
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����4ʧ��");
		}
	}

	return CSUDI_TRUE; 
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Բ��ſհ�����ʱ�ܷ���Ƶ�ɹ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź����ź��в��ŵ��ǿհ�����
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :��ȷ��Ƶ��
//@EXPECTATION:��Ƶ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5������dwTunerId��ִ�в���2~4
CSUDI_BOOL CSTC_TUNER_IT_Connect_0016( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;

    char cTunerCfgBuf[32];
    unsigned int uFrequency = 0;
	if(CSTKGetConfigInfo("TUNER","CS_TUNER_CABLE_DELIVER_FREQUENCY_SPECIAL",cTunerCfgBuf,sizeof(cTunerCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		uFrequency = CSTKGetIntFromStr(cTunerCfgBuf,10);
	}
  	CSTCPrint("\n��ȷ���� %d Ƶ�㲥�ŵ��ǿհ���������Y����������N�� \n",uFrequency);


	if(CSTKWaitYes())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

		CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
		
		for (i=0 ; i<nActIdCnt ; i++)
		{
			if(adwTunerIds[i] != -1)
			{ 
				s_nTunerTestData = 0;
		
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");
			
				nAddCallbackCount++;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[s_nDeliverCount-2]),"����4ʧ��");

				CSTCPrint("��ȴ�5��\n");

				CSUDIOSThreadSleep(5000);

				CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"����5ʧ��");
			}
		}

	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "����û׼���ÿհ��������ʸ���������ʧ�� !!!");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����6ʧ��");
		}
	}
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:����������Ƶ���̵��ź����е���ȷ��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��unDeliver :��Ч��Ƶ����Ϣ(Ƶ��Ϊ44400KHz)
//@INPUT:3��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect����ЧƵ�������Ƶ������Ҫ�󷵻سɹ����һص���Ϣ��EM_UDITUNER_SIGNAL_SEARCHING
//@EXECUTIONFLOW:4������CSUDITunerConnect��ָ����ЧƵ�������Ƶ�������������سɹ����һص���Ϣ��EM_UDITUNER_SIGNAL_CONNECT
//@EXECUTIONFLOW:5����ʾ������Ա�ε��ź���,����1�������źŶ�ʧ����Ϣ����
//@EXECUTIONFLOW:6����ʾ�����ź��ߣ����ӳɹ����ص���Ϣ��EM_UDITUNER_SIGNAL_CONNECT
//@EXECUTIONFLOW:7������CSUDITunerConnect��һ��ЧƵ������ٴ���Ƶ��������ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:8������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0017( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;
	int j=0;
	CSUDITunerSRCDeliver_U unDeliver;
	memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));

	unDeliver.m_sCableDeliver.m_uFrequency = s_unDeliver[0].m_sCableDeliver.m_uFrequency + 3000;
	unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8;
	unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;
	        s_bIsSignalLost = CSUDI_FALSE;

            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "�ӻص�ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver),"��Ч��Ƶ����Ƶ��Ӧ�óɹ�");

			CSTCPrint("��ȴ�\n");

			for(j = 0;j < 10 ;j ++)
			{
				if(s_nTunerTestData==TUNER_SIGNAL_SEARCHING)
					break;
				CSUDIOSThreadSleep(2000);
			}

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING,"�ص���Ϣ1����");

		 	s_nTunerTestData = 0;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"������Ƶ��ʧ��");

			CSTCPrint("��ȴ�\n");

			for(j = 0;j < 10 ;j ++)
			{
				if(s_nTunerTestData==TUNER_SIGNAL_CONNECTED)
					break;
				CSUDIOSThreadSleep(2000);
			}

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"�ص���Ϣ2����");

			s_nTunerTestData = 0;

			CSTCPrint("\n��ε��ź��߲���ϸ�۲죬����1�������źŶ�ʧ�Ĵ�ӡ��'Y'������'N'\n");

        		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"����6ʧ��");
			
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSignalLost,"����7ʧ��");

			for(j = 0;j < 10 ;j ++)
			{
				if(s_nTunerTestData==TUNER_SIGNAL_SEARCHING)
					break;
				CSUDIOSThreadSleep(2000);
			}

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING, "�ص���Ϣ3����");

			CSTCPrint("\n������ź��߲���ϸ�۲죬����1��������Ƶ�ɹ��Ĵ�ӡ��'Y'������'N'\n");
		
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"����9ʧ��");
			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"�ص���Ϣ4����");
		
			 s_nTunerTestData = 0;

		    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[1]),"����6ʧ��");

		    	while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
		    	{
				CSUDIOSThreadSleep(10);
	   
		    	}
		
			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"�ڶ�����Ƶʧ��");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����6ʧ��");
		}
	}

	return CSUDI_TRUE;


}


//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Ե��Ʒ�ʽQAM���ٰ���64��128��256����
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��unDeliver[3]:�Ϸ���Ƶ����Ϣ��QAM�ֱ���64��128��256
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect�Ե�һ��Ƶ��QAMΪ64������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:4���ȴ�2����ж�s_nTunerTestData��ֵ��Ҫ��ΪTUNER_SIGNAL_CONNECTED�������������ʧ��
//@EXECUTIONFLOW:5������CSUDITunerConnect�Եڶ���Ƶ��QAMΪ128������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:6���ȴ�2����ж�s_nTunerTestData��ֵ��Ҫ��ΪTUNER_SIGNAL_CONNECTED�������������ʧ��
//@EXECUTIONFLOW:7������CSUDITunerConnect�Ե�����Ƶ��QAMΪ256������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS����ͨ���ص���Ϣ�ж���Ƶ�ɹ�
//@EXECUTIONFLOW:8���ȴ�2����ж�s_nTunerTestData��ֵ��Ҫ��ΪTUNER_SIGNAL_CONNECTED�������������ʧ��
//@EXECUTIONFLOW:9������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�

CSUDI_BOOL CSTC_TUNER_IT_Connect_0018( void )
{
	int nActIdCnt = 0;
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
    int nDeliverCount = 0;
	int i = 0;
	int j = 0;
	int t = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerSRCDeliver_U unDeliver[3];

	memset(unDeliver, 0, sizeof(unDeliver));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	nDeliverCount = CSTC_TUNER_GetDeliverCount();
   
    for( i = 0; i < nDeliverCount; i++ )
    {
    	if((s_unDeliver[i].m_sCableDeliver.m_uModulation_8 == EM_UDITUNER_QAM64)
			&& (unDeliver[EM_TUNER_QAM64].m_sCableDeliver.m_uFrequency == 0))
        {
        	unDeliver[EM_TUNER_QAM64].m_sCableDeliver.m_uFrequency = s_unDeliver[i].m_sCableDeliver.m_uFrequency;
			unDeliver[EM_TUNER_QAM64].m_sCableDeliver.m_uModulation_8 = s_unDeliver[i].m_sCableDeliver.m_uModulation_8;
			unDeliver[EM_TUNER_QAM64].m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[i].m_sCableDeliver.m_uSymbolRate_24;
		}
		else if((s_unDeliver[i].m_sCableDeliver.m_uModulation_8 == EM_UDITUNER_QAM128) 
			&& (unDeliver[EM_TUNER_QAM128].m_sCableDeliver.m_uFrequency == 0))
        {
			unDeliver[EM_TUNER_QAM128].m_sCableDeliver.m_uFrequency = s_unDeliver[i].m_sCableDeliver.m_uFrequency;
			unDeliver[EM_TUNER_QAM128].m_sCableDeliver.m_uModulation_8 = s_unDeliver[i].m_sCableDeliver.m_uModulation_8;
			unDeliver[EM_TUNER_QAM128].m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[i].m_sCableDeliver.m_uSymbolRate_24;
		}
		else if((s_unDeliver[i].m_sCableDeliver.m_uModulation_8 == EM_UDITUNER_QAM256)
			&& (unDeliver[EM_TUNER_QAM256].m_sCableDeliver.m_uFrequency == 0))
        {
			unDeliver[EM_TUNER_QAM256].m_sCableDeliver.m_uFrequency = s_unDeliver[i].m_sCableDeliver.m_uFrequency;
			unDeliver[EM_TUNER_QAM256].m_sCableDeliver.m_uModulation_8 = s_unDeliver[i].m_sCableDeliver.m_uModulation_8;
			unDeliver[EM_TUNER_QAM256].m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[i].m_sCableDeliver.m_uSymbolRate_24;
    	}

        if((unDeliver[EM_TUNER_QAM64].m_sCableDeliver.m_uFrequency != 0) 
			&& (unDeliver[EM_TUNER_QAM128].m_sCableDeliver.m_uFrequency != 0) 
			&& (unDeliver[EM_TUNER_QAM256].m_sCableDeliver.m_uFrequency != 0))
        {
            break;
        }
    }

    CSTK_ASSERT_TRUE_FATAL((i < nDeliverCount), "��ȷ�������ļ��� QAM64/QAM128/QAM256 ���ֵ�ʽ��ʽ��Ƶ����Ϣ������!");
	for (i = 0 ; i < nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			nAddCallbackCount++;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "�ӻص�ʧ��");

			for(j = 0; j < 3;j ++)
			{
				s_nTunerTestData = 0;
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver[j]),"����ʧ��");

				CSTCPrint("��ȴ�\n");

				for(t = 0;t < 10 ;t ++)
				{
					if(s_nTunerTestData==TUNER_SIGNAL_CONNECTED)
					break;
					CSUDIOSThreadSleep(2000);
				}
				CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"�ص���Ϣ����");
			}
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����6ʧ��");
		}
	}
	return CSUDI_TRUE;

}


//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Բ����ź��������ź�ʱ������ʧ�ܵ���Ϣ����̫��ʱ�䷵��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@INPUT:dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@EXPECTATION: ���Բ����ź��������ź�ʱ������ʧ�ܵ���Ϣ����ʱ��С��1��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect�Ե�һ��Ƶ�������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ε��ź��ߺ�ͳ�Ƶȴ��źŶ�ʧ��Ϣ������ʱ�� ��Ҫ��С��1��
//@EXECUTIONFLOW:5������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0019( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;

	int i = 0;
	int j = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData), "����2ע��ص�ʧ��");
	for (i = 0;i <5 ; i++)
	{
		s_bIsSignalLost = CSUDI_FALSE;
		j = 0;
		CSTCPrint("��ȷ���ź��߲��ϰ�'Y'!\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"CSTKWaitYesʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]),"������3 ����ʧ��\n");

        CSTCPrint("�뽫�ź��߰γ����ٰ�'Y'ȷ��!\n");

		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"CSTKWaitYesʧ��");
		while(s_bIsSignalLost != CSUDI_TRUE)
		{
			CSUDIOSThreadSleep(10);
	   		j++;
			
			if(j*10 > 1000)
				break;
		}
		CSTCPrint("�ȴ�ʱ��= %dms\n",j*10);
		CSTK_ASSERT_TRUE_FATAL(j*10<1000,"����4:��Ϣ����ʱ��̫��!");
		CSTK_ASSERT_TRUE_FATAL(s_bIsSignalLost == CSUDI_TRUE,"�ε��ź���ʱ��Ƶ��Ӧ�ɹ�\n");
	
} 

    CSTCPrint("�뽫�ź��߲��ϣ��ٰ�'Y'ȷ��!\n");
    CSTKWaitYes();
	
	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData),"����5:ɾ���ص�ʧ��");
	}
	return CSUDI_TRUE;

}
//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:���Իص�����SUDITunerAddCallback�ڵȴ�ʱ��CSUDITunerConnect�Ƿ�����
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@INPUT:dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@EXPECTATION:�ص������ȴ����ᵼ��CSUDITunerConnect����
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallback���ڻص�����Tuner_CallbackTest5�еȴ�5�룬����g_bIsTunerConnected��ΪCSUDI_TRUE;
//@EXECUTIONFLOW:3������CSUDITunerConnect�Ե�һ��Ƶ�������Ƶ������Ҫ�󷵻�CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ж�g_bIsTunerConnected�Ƿ�ΪCSUDI_TRUE����������ʧ��
//@EXECUTIONFLOW:5������CSUDITunerDelCallbackɾ���ص�
CSUDI_BOOL CSTC_TUNER_IT_Connect_0020( void )
{
	int i = 0;
	int nActIdCnt = 0;
	int nUserData = 0;
	CSUDI_UINT32 adwTunerIds[20];
	
	memset(adwTunerIds, 0, sizeof(adwTunerIds));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1����ȡ�豸IDʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest5, &nUserData), "����2����ӻص�ʧ��");	
	}

	g_bIsTunerConnected = CSUDI_FALSE;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "����3����Ƶʧ��");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bIsTunerConnected, "����4ʧ�ܣ�CSUDITunerConnect�ӿڲ�Ӧ�ö���");

	while(!g_bIsTunerConnected)
	{
		CSUDIOSThreadSleep(100);
	}
	
	CSTK_FATAL_POINT

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest5, &nUserData), "����5��ɾ���ص�ʧ��");	
	}
	
	return CSUDI_TRUE;

}


//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:����dwTunerId�Ƿ�ʱ�ܷ���ȷ����ź���Ϣ
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId = 0x1234
//@INPUT:2��u32SignalInfoMask = EM_UDITUNER_SIGNAL_QUALITY
//@EXPECTATION:��ȡ��Ϣʧ�ܲ�����CSUDITUNER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:1������CSUDITunerGetSignalInfo��ȡ�ź���Ϣ��Ҫ�󷵻�CSUDITUNER_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0001( void )
{
	CSUDITunerSignalInfo_S s_TunerSignalInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_INVALID_DEVICE_ID == CSUDITunerGetSignalInfo(0x1234, EM_UDITUNER_SIGNAL_QUALITY,&s_TunerSignalInfo),"����1ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:����u32SignalInfoMask�Ƿ�ʱ�ܷ���ȷ����ź���Ϣ
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��u32SignalInfoMask = 0x1234
//@EXPECTATION:��ȡ��Ϣʧ�ܲ�����CSUDITUNER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerGetSignalInfo��ȡ�ź���Ϣ��Ҫ�󷵻�CSUDITUNER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3�������кϷ���dwTunerIdִ�в���2
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerSignalInfo_S s_TunerSignalInfo;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_BAD_PARAMETER == CSUDITunerGetSignalInfo(adwTunerIds[i], 0,&s_TunerSignalInfo),"����2ʧ��");
	}

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:���Ա�����Ϣ��ָ��Ϊ��ʱ�ܷ���ȷ����ź���Ϣ
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��u32SignalInfoMask = EM_UDITUNER_SIGNAL_QUALITY
//@EXPECTATION:��ȡ��Ϣʧ�ܲ�����CSUDITUNER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerGetSignalInfo��ȡ�ź���Ϣ��Ҫ�󷵻�CSUDITUNER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3�������кϷ���dwTunerIdִ�в���2
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_BAD_PARAMETER == CSUDITunerGetSignalInfo(adwTunerIds[i], EM_UDITUNER_SIGNAL_QUALITY,CSUDI_NULL),"����2ʧ��");
	}

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:������������ʱ�ܷ���ȷ����ź���Ϣ
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��u32SignalInfoMask = EM_UDITUNER_SIGNAL_QUALITY | EM_UDITUNER_SIGNAL_STRENGTH |EM_UDITUNER_CUR_FREQU
//@EXPECTATION:��ȡ��Ϣ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect��ָ��Ƶ�������Ƶ������Ҫ�󷵻سɹ������ڻص��н�s_nTunerTestData��Ϊ1001
//@EXECUTIONFLOW:4������CSUDITunerGetSignalInfo��ȡ����Ƶ�ʵ��ź���Ϣ��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5���жϻ�ȡ���ź���Ϣ�е�Ƶ����Ϣ�Ƿ��������һ�£������һ����ʧ��
//@EXECUTIONFLOW:6���жϻ�ȡ���ź���Ϣ���ź��������ź�ǿ���Ƿ���0~100��Χ�����������ʧ��
//@EXECUTIONFLOW:7������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:8�������кϷ���dwTunerIdִ�в���2~7
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0004( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nAddCallbackCount = 0;
	CSUDITunerSignalInfo_S s_TunerSignalInfo;
	int nUserData = 0x88888888;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		memset(&s_TunerSignalInfo,0,sizeof(CSUDITunerSignalInfo_S));
		s_nTunerTestData = 0;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "����2ʧ��");
		
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����3ʧ��");

		CSTCPrint("��ȴ�2��\n");

		CSUDIOSThreadSleep(2000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"����3ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i], 
								EM_UDITUNER_SIGNAL_QUALITY | EM_UDITUNER_SIGNAL_STRENGTH |EM_UDITUNER_CUR_FREQU,
								&s_TunerSignalInfo),"����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_nCurrentFreq == s_unDeliver[0].m_sCableDeliver.m_uFrequency, "����5ʧ��");

		CSTK_ASSERT_TRUE_FATAL((s_TunerSignalInfo.m_nQuality >= 0) && (s_TunerSignalInfo.m_nQuality <=100),"����6�ź��������ڷ�Χ��");

		CSTK_ASSERT_TRUE_FATAL((s_TunerSignalInfo.m_nStrength>= 0) && (s_TunerSignalInfo.m_nStrength <=100),"����6�ź�ǿ�Ȳ��ڷ�Χ��");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "����7ʧ��");
		}
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:���԰ε��ź��ߺ�������ʣ��Լ���Ƶ�ɹ����ܷ����ź��������źŵȼ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�����ȷ��Ƶ����Ϣ
//@PRECONDITION:3�����ź�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��punDeliver :�Ϸ���Ƶ�����ݵ�ַ
//@EXPECTATION:�����ź�����Ƶ�ɹ����ε���Ƶʧ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect����ЧƵ�������Ƶ������Ҫ�󷵻سɹ��һص���ϢΪEM_UDITUNER_SIGNAL_CONNECTED
//@EXECUTIONFLOW:4����ʾ������Ա�ε��ź��߲��۲��Ƿ���1�������źŶ�ʧ�Ĵ�ӡ������'Y'����������'N'����������ʧ��
//@EXECUTIONFLOW:5���ж�s_bIsSignalLost��ֵ��Ҫ��Ϊtrue�������������ʧ��
//@EXECUTIONFLOW:6������CSUDITunerGetSignalInfo��ȡ�����ʣ�Ҫ��ε��ź��ߺ��Ϊȫ���룬�һص���ϢΪEM_UDITUNER_SIGNAL_SEARCHING
//@EXECUTIONFLOW:7����ʾ�����ź��߲��۲��Ƿ���1��������Ƶ�ɹ��Ĵ�ӡ������'Y'����������'N'����������ʧ��
//@EXECUTIONFLOW:8����Ƶ�ɹ������CSUDITunerGetSignalInfo�ж��ܷ����ź�ǿ�Ⱥ��ź�����
//@EXECUTIONFLOW:9������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10������dwTunerId��ִ�в���2~10
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0006( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;
	CSUDITunerSignalInfo_S s_TunerSignalInfo;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			memset(&s_TunerSignalInfo,0,sizeof(CSUDITunerSignalInfo_S));
			s_nTunerTestData = 0;

			s_bIsSignalLost = CSUDI_FALSE;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "����2ʧ��");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����3ʧ��");

			CSTCPrint("��ȴ�2��\n");
			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"����4ʧ��");

			s_nTunerTestData = 0;

			CSTCPrint("\n��ε��ź��߲���ϸ�۲죬����1�������źŶ�ʧ�Ĵ�ӡ��'Y'������'N'�����ڿ�����ʾ��Ų����ź���\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"����5ʧ��");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSignalLost,"����6ʧ��");

			CSUDIOSThreadSleep(5000);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i],EM_UDITUNER_ERROR_RATE,
								&s_TunerSignalInfo),"����7ʧ��");

			CSTCPrint("������ = %d\n",s_TunerSignalInfo.m_nErrorRate);
			CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_nErrorRate ==10000000,"���ź��������ʴ���!!!");

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING, "����8ʧ��");

			CSTCPrint("\n������ź��߲���ϸ�۲죬����1��������Ƶ�ɹ��Ĵ�ӡ��'Y'������'N'\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"����9ʧ��");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i], 
								EM_UDITUNER_SIGNAL_QUALITY | EM_UDITUNER_SIGNAL_STRENGTH |EM_UDITUNER_CUR_FREQU,
								&s_TunerSignalInfo),"����10ʧ��");

			CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_nCurrentFreq == s_unDeliver[0].m_sCableDeliver.m_uFrequency, "����11ʧ��");

			CSTK_ASSERT_TRUE_FATAL((s_TunerSignalInfo.m_nQuality >= 0) && (s_TunerSignalInfo.m_nQuality <=100),"����12�ź��������ڷ�Χ��");

			CSTK_ASSERT_TRUE_FATAL((s_TunerSignalInfo.m_nStrength>= 0) && (s_TunerSignalInfo.m_nStrength <=100),"����13�ź�ǿ�Ȳ��ڷ�Χ��");
			
		}
		
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"����14ʧ��");
		}
	}

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:������������ʱ�ܷ���ȷ����źŵ�Ƶ����Ϣ
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��u32SignalInfoMask = EM_UDITUNER_CUR_FREQU
//@EXPECTATION:��ȡ��Ϣ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect��ָ��Ƶ�������Ƶ������Ҫ�󷵻سɹ������ڻص��н�s_nTunerTestData��Ϊ1001
//@EXECUTIONFLOW:4������CSUDITunerGetSignalInfo��ȡ����Ƶ���Ƶ����Ϣ��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5���жϻ�ȡ���ź���Ϣ�е�Ƶ���Ƿ��������һ�£������һ����ʧ��
//@EXECUTIONFLOW:6������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7�������кϷ���dwTunerIdִ�в���2~6
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0007( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nAddCallbackCount = 0;
	CSUDITunerSignalInfo_S s_TunerSignalInfo;
	int nUserData = 0x88888888;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		memset(&s_TunerSignalInfo,0,sizeof(CSUDITunerSignalInfo_S));
		s_nTunerTestData = 0;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "����2ʧ��");
		
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����3ʧ��");

		CSUDIOSThreadSleep(2000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"�ص���Ϣ����");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i], EM_UDITUNER_CUR_FREQU,&s_TunerSignalInfo),"����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_nCurrentFreq == s_unDeliver[0].m_sCableDeliver.m_uFrequency, "����5ʧ��");

	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "����6ʧ��");
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:������������ʱ�ܷ���ȷ����źŵĵ�ƽ�ȼ���Ϣ
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��u32SignalInfoMask = EM_UDITUNER_SIGNAL_LEVEL
//@EXPECTATION:��ȡ��Ϣ�ɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect��ָ��Ƶ�������Ƶ������Ҫ�󷵻سɹ������ڻص��н�s_nTunerTestData��Ϊ1001
//@EXECUTIONFLOW:4������CSUDITunerGetSignalInfo��ȡ����Ƶ��ĵ�ƽ�ȼ���Ϣ��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5���жϻ�ȡ���ź���Ϣ�еĵ�ƽ�ȼ��Ƿ����0�����С��0��ʧ��
//@EXECUTIONFLOW:6������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7�������кϷ���dwTunerIdִ�в���2~6
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0008( void )
{
    int nActIdCnt = 0;
    CSUDI_UINT32 adwTunerIds[20];
    int nAddCallbackCount = 0;
    CSUDITunerSignalInfo_S s_TunerSignalInfo;
    int nUserData = 0x88888888;
    int i;

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

    CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

    for(i=0 ; i<nActIdCnt ; i++)
    {
        memset(&s_TunerSignalInfo,0,sizeof(CSUDITunerSignalInfo_S));
        s_nTunerTestData = 0;
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "����2ʧ��");
        
        nAddCallbackCount++;

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"����3ʧ��");

        CSUDIOSThreadSleep(2000);

        CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"�ص���Ϣ����");
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i], EM_UDITUNER_SIGNAL_LEVEL, &s_TunerSignalInfo), "����4ʧ��");

        CSTCPrint("s_TunerSignalInfo.m_dSignalLevel=%lf\n", s_TunerSignalInfo.m_dSignalLevel);
        
        CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_dSignalLevel > 0, "����5ʧ��");
    }

    CSTK_FATAL_POINT
    {
        for(i=0 ; i<nAddCallbackCount ; i++)
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "����6ʧ��");
        }
    }
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddPID 
//@DESCRIPTION:����dwTunerId�Ƿ�ʱ�ܷ�ɹ���ӹ���PID
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId = 0x1234	
//@INPUT:2��nPID = 0x0101
//@EXPECTATION:���ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDITunerAddPID��ӹ���PID��Ҫ�󷵻�ʧ��
CSUDI_BOOL CSTC_TUNER_IT_AddPID_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerAddPID(0x1234, 0x0101), "����1ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddPID 
//@DESCRIPTION:����nPID�Ƿ�ʱ�ܷ�ɹ���ӹ���PID
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��nPID = 0x1234
//@EXPECTATION:���ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerGetDeviceInfo��ȡ��ǰ�����豸��������Ϣ�������سɹ��������������ʧ��
//@EXECUTIONFLOW:3������CSUDITunerAddPID��ӹ���PID�������Tuner��m_bPIDFilterSupportΪCSUDI_TRUE�򷵻ش������(��CSUDI_SUCCESS)��ΪCSUDI_FALSE�򷵻�CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:4�������кϷ���dwTunerIdִ�в���2~3
CSUDI_BOOL CSTC_TUNER_IT_AddPID_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerInfo_S s_TunerInfo;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(adwTunerIds[i], &s_TunerInfo), "����2ʧ��");

		if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_TRUE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerAddPID(adwTunerIds[i], 0x1234), "����3ʧ��");
		}
		else if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_FALSE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerAddPID(adwTunerIds[i], 0x1234), "����3ʧ��");
		}
	}

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddPID 
//@DESCRIPTION:���ԺϷ������ܷ�ɹ���ӹ���PID
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��nPID = 0
//@EXPECTATION:��ӳɹ�
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerGetDeviceInfo��ȡ��ǰ�����豸��������Ϣ�������سɹ��������������ʧ��
//@EXECUTIONFLOW:3�������Tuner��m_bPIDFilterSupportΪCSUDI_TRUE��ִ�в������²��裬����ֱ�ӵ���CSUDITunerAddPID��ӹ���PID��Ҫ�󷵻�CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED��ֱ����������10
//@EXECUTIONFLOW:4������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5���ڻص�����������жϵ���Ƶ�ɹ���s_nTunerTestData��ΪTUNER_SIGNAL_CONNECTED
//@EXECUTIONFLOW:6������CSUDITunerConnect������Ƶ������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7���ȴ�2����ж�s_nTunerTestData��ֵ��Ҫ��ΪTUNER_SIGNAL_CONNECTED�������������ʧ��
//@EXECUTIONFLOW:8������CSUDITunerAddPID��ӹ���PID��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:9������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10�������кϷ���dwTunerIdִ�в���2~10
CSUDI_BOOL CSTC_TUNER_IT_AddPID_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerInfo_S s_TunerInfo;
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(adwTunerIds[i], &s_TunerInfo), "����2ʧ��");

		if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_TRUE)
		{
			s_nTunerTestData = 0;
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "����4ʧ��");
			
			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]), "����6ʧ��");

			CSTCPrint("��ȴ�2��\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"����7ʧ��");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddPID(adwTunerIds[i], 0), "����8ʧ��");
		}
		else if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_FALSE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerAddPID(adwTunerIds[i], 0), "��֧��Ӧ�÷���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest1, &nUserData), "����9ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRemovePID 
//@DESCRIPTION:����dwTunerId�Ƿ�ʱ�ܷ�ɹ�ɾ������PID
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId = 0x1234	
//@INPUT:2��nPID = 0x0101
//@EXPECTATION:ɾ��ʧ�ܲ����ش������ֵ(��CSUDI_SUCCESS)
//@EXECUTIONFLOW:1������CSUDITunerRemovePIDɾ������PID��Ҫ�󷵻�ʧ��
CSUDI_BOOL CSTC_TUNER_IT_RemovePID_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRemovePID(0x1234, 0x0101), "����1ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRemovePID 
//@DESCRIPTION:���ԺϷ������ܷ�ɹ����ɾ������PID
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��nPID = 0
//@EXPECTATION:ɾ���ɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerGetDeviceInfo��ȡ��ǰ�����豸��������Ϣ�������سɹ��������������ʧ��
//@EXECUTIONFLOW:3�������Tuner��m_bPIDFilterSupportΪCSUDI_TRUE��ִ�в������²��裬����ֱ�ӵ���CSUDITunerRemovePIDɾ������PID��Ҫ�󷵻�CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED��ֱ����������11
//@EXECUTIONFLOW:4������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:5���ڻص�����������жϵ���Ƶ�ɹ���s_nTunerTestData��ΪTUNER_SIGNAL_CONNECTED
//@EXECUTIONFLOW:6������CSUDITunerConnect������Ƶ������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:7���ȴ�2����ж�s_nTunerTestData��ֵ��Ҫ��ΪTUNER_SIGNAL_CONNECTED�������������ʧ��
//@EXECUTIONFLOW:8������CSUDITunerAddPID��ӹ���PID��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:9������CSUDITunerRemovePIDɾ����ӵĹ���PID��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:11�������кϷ���dwTunerIdִ�в���2~10
CSUDI_BOOL CSTC_TUNER_IT_AddRemovePID_0001( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerInfo_S s_TunerInfo;
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner������Ϊ0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(adwTunerIds[i], &s_TunerInfo), "����2ʧ��");

		if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_TRUE)
		{
			s_nTunerTestData = 0;
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest1, &nUserData), "����4ʧ��");
			
			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]), "����6ʧ��");

			CSTCPrint("��ȴ�2��\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"����7ʧ��");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddPID(adwTunerIds[i], 0), "����8ʧ��");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRemovePID(adwTunerIds[i], 0), "����9ʧ��");
		}
		else if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_FALSE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerRemovePID(adwTunerIds[i], 0), "��֧��Ӧ�÷���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest1, &nUserData), "����10ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@DESCRIPTION:�������������ܷ�ɹ�����豸֪ͨ�ص�����
//@PRECONDITION:1���豸��ʼ���ɹ�
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�Ҫ������ƶ�Tuner����Ϣ
//@INPUT:1��fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2��UserData = 0x88888888
//@EXPECTATION:ע��ɹ�
//@EXECUTIONFLOW:1������CSUDITunerRDIAddCallbackע��ص���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:2���ȴ�����ɲ��Tuner
//@EXECUTIONFLOW:3���ڻص������л�ȡ������豸ID��������s_dwRDITunerIdȻ���ж��豸ID�ĸ�16λ�Ƿ�Ϊ3������ɹ�������ʧ��
//@EXECUTIONFLOW:4������CSUDITunerGetDeviceInfo��ȡ�豸��Ϣ��Ҫ�󷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:5��ֱ�Ӱε����ƶ�Tuner
//@EXECUTIONFLOW:6�����аγ��������жϻص���ϢҪ��ΪEM_UDIRDI_EVENT_PLUGOUT������EventData��ֵΪEM_UDIRDI_PLUGOUT_UNSAFE�����򷵻ز���ʧ��
//@EXECUTIONFLOW:7������CSUDITunerRDIRemoveCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_RDIAddCallback_0001( void )
{
	int nUserData = 0x88888888;
	CSUDITunerInfo_S sTunerInfo;
	int nSleepCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback( Tuner_RDICallbackTest1, &nUserData), "����1ʧ��");

	CSTCPrint("\n����Ͽ��ƶ�Tuner����'Y����������������'\n");

	if(CSTKWaitYes())
	{
		while(!s_bRDITunerIsReady)
		{
			CSUDIOSThreadSleep(50);
			if(nSleepCount++ > 100)
			{
				break;
			}
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_UINT16)(s_dwRDITunerId >> 16) == 3,"����3ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(s_dwRDITunerId, &sTunerInfo),"����4ʧ��");
	}

	CSTCPrint("\n��ε����ϵĿ��ƶ�Tuner����'Y'����������������\n");

	if(CSTKWaitYes())
	{
		CSUDIOSThreadSleep(2000);
		CSTK_ASSERT_TRUE_FATAL(EM_UDIRDI_PLUGOUT_UNSAFE == s_nRDIEventData, "����6ʧ��");
	}

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback( Tuner_RDICallbackTest1, &nUserData), "����7ʧ��");
		}
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@DESCRIPTION:����fnCallback�Ƿ�ʱ�ܷ�ɹ�����豸֪ͨ�ص�����
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��fnTunerCallback = CSUDI_NULL
//@INPUT:2��UserData = 0x88888888
//@EXPECTATION:ע��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerRDIAddCallbackע��ص���Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:2���������1���سɹ������CSUDITunerRDIRemoveCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�������1����ʧ����ִ�иò���
CSUDI_BOOL CSTC_TUNER_IT_RDIAddCallback_0002( void )
{
	int nUserData = 0x88888888;
	CSUDI_BOOL bHasJump = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRDIAddCallback( CSUDI_NULL, &nUserData), "����1ʧ��");

	bHasJump = CSUDI_FALSE;
	
	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			if(bHasJump)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(CSUDI_NULL, &nUserData),"����2ʧ��");
			}
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@DESCRIPTION:����UserDataΪ��ʱ�ܷ�ɹ�����豸֪ͨ�ص�����
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2��UserData = CSUDI_NULL
//@EXPECTATION:ע��ɹ�
//@EXECUTIONFLOW:1������CSUDITunerRDIAddCallbackע��ص���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:2������CSUDITunerRDIRemoveCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_RDIAddCallback_0003( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback( Tuner_RDICallbackTest1, CSUDI_NULL), "����1ʧ��");
	
	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, CSUDI_NULL),"����2ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@DESCRIPTION:����fnCallback�Ƿ�ʱ�ܷ�ɹ�ɾ���豸֪ͨ�ص�����
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��fnTunerCallback = CSUDI_NULL
//@INPUT:2��UserData = 0x88888888
//@EXPECTATION:ɾ��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerRDIRemoveCallbackɾ���ص�������Ҫ�󷵻�ʧ��
CSUDI_BOOL CSTC_TUNER_IT_RDIRemoveCallback_0001( void )
{
	int nUserData = 0x88888888;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRDIRemoveCallback(CSUDI_NULL, &nUserData), "����1ʧ��");	
	
	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@DESCRIPTION:����UserDataΪ��ʱ�ܷ�ɹ�ɾ���豸֪ͨ�ص�����
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2��UserData = CSUDI_NULL
//@EXPECTATION:ɾ���ɹ�
//@EXECUTIONFLOW:1������CSUDITunerRDIAddCallbackע��ص���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:2������CSUDITunerRDIRemoveCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_RDIRemoveCallback_0002( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback( Tuner_RDICallbackTest1, CSUDI_NULL), "����1ʧ��");
	
	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, CSUDI_NULL),"����2ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@DESCRIPTION:�����ܷ�ɹ�ɾ��û��ע����豸֪ͨ�ص�����
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2��UserData = 0x88888888
//@EXPECTATION:ɾ��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerRDIRemoveCallbackɾ��û��ע��Ļص�������Ҫ�󷵻�ʧ��
CSUDI_BOOL CSTC_TUNER_IT_RDIRemoveCallback_0003( void )
{
	int nUserData = 0x88888888;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData), "����1ʧ��");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@DESCRIPTION:���Ե�UserData��ע��Ĳ�һ��ʱ�ܷ�ɹ�ɾ���豸֪ͨ�ص�����
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2��UserData = 0x88888888
//@EXPECTATION:ɾ��ʧ��
//@EXECUTIONFLOW:1������CSUDITunerRDIAddCallbackע��ص���UserDataΪ0Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:2������CSUDITunerRDIRemoveCallbackɾ��UserData��ע��Ļص���һ�µĻص�������Ҫ�󷵻�ʧ��
//@EXECUTIONFLOW:2������CSUDITunerRDIRemoveCallbackɾ������1ע��Ļص�������Ҫ�󷵻�ʧ��
CSUDI_BOOL CSTC_TUNER_IT_RDIRemoveCallback_0004( void )
{
	int nUserData1 = 0;
	int nUserData2 = 0x88888888;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback( Tuner_RDICallbackTest1, &nUserData1), "����1ʧ��");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData2),"����2ʧ��");

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData1), "����3ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@CASEGROUP:CSUDITunerRDIRemoveDev 
//@DESCRIPTION:����������ο��ƶ�Tuner�����
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2��UserData = 0x88888888
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDITunerRDIAddCallbackע��ص���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:2���ȴ�����ɲ��Tuner
//@EXECUTIONFLOW:3���ڻص������л�ȡ������豸ID��������s_dwRDITunerIdȻ���ж��豸ID�ĸ�16λ�Ƿ�Ϊ3������ɹ�������ʧ��
//@EXECUTIONFLOW:4������CSUDITunerGetDeviceInfo��ȡ�豸��Ϣ��Ҫ�󷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:5������CSUDITunerRDIRemoveDev��ȫ�Ƴ����ƶ�Tuner��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:6�����а�ȫ�Ƴ��������жϻص���ϢҪ��ΪEM_UDIRDI_EVENT_PLUGOUT������EventData��ֵΪEM_UDIRDI_PLUGOUT_SAFE�����򷵻ز���ʧ��
//@EXECUTIONFLOW:7������CSUDITunerRDIRemoveCallbackɾ��ע��Ļص�������Ҫ�󷵻�ʧ��
CSUDI_BOOL CSTC_TUNER_IT_RDI_0001( void )
{
	int nUserData = 0x88888888;
	CSUDITunerInfo_S sTunerInfo;
	int nSleepCount = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback(Tuner_RDICallbackTest1, &nUserData), "����1ʧ��");

	CSTCPrint("\n����Ͽ��ƶ�Tuner����'Y����������������'\n");

	if(CSTKWaitYes())
	{
		while(!s_bRDITunerIsReady)
		{
			CSUDIOSThreadSleep(50);
			
			if(nSleepCount++ > 100)
			{
				break;
			}
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_UINT16)(s_dwRDITunerId >> 16) == 3,"����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(s_dwRDITunerId, &sTunerInfo),"����4ʧ��");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveDev(s_dwRDITunerId),"����5ʧ��");

		CSUDIOSThreadSleep(2000);
		
		CSTK_ASSERT_TRUE_FATAL(EM_UDIRDI_PLUGOUT_SAFE == s_nRDIEventData, "����6ʧ��");
	}

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData), "����7ʧ��");
		}
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@CASEGROUP:CSUDITunerRDIRemoveDev 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerIORead
//@DESCRIPTION:���Կ��ƶ�Tuner���롢��Ƶ����ȡ�����Լ��Ƴ��Ȳ���
//@PRECONDITION:1��ϵͳ��ʼ���ɹ����ź�����
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�Ҫ������ƶ�Tuner����Ϣ�Լ�Ҫ��Ƶ��Ƶ����Ϣ
//@INPUT:�ڲ���������
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDITunerRDIAddCallbackע��ص���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:2���ȴ�����ɲ��Tuner
//@EXECUTIONFLOW:3���ڻص������л�ȡ������豸ID��������s_dwRDITunerIdȻ���ж��豸ID�ĸ�16λ�Ƿ�Ϊ3������ɹ�������ʧ��
//@EXECUTIONFLOW:4������CSUDITunerGetDeviceInfo��ȡ�豸��Ϣ��Ҫ�󷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:5���ڻص������л�ȡ�ƶ�Tuner��ID��������CSUDITunerAddCallbackΪ���豸ע����Ƶ��Ϣ�ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:6������CSUDITunerConnectʹ�ø��ƶ��豸��ָ��Ƶ�������Ƶ��Ҫ�󷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:7���ȴ�2����ж���Ƶ��Ϣ�ص�������ȡ�Ļص���Ϣ��Ҫ��ΪEM_UDITUNER_SIGNAL_CONNECTED����s_nTunerTestData��Ϊ1001�������������ʧ��
//@EXECUTIONFLOW:8�������Tuner֧��Ӳ����PID�����CSUDITunerAddPID���TunerӲ����PID��Ҫ�󷵻سɹ������򷵻�CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9������CSUDITunerIORead��ȡTS�����ݣ�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10������CSUDITunerRDIRemoveDev��ȫ�Ƴ����ƶ�Tuner��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:11������CSUDITunerRDIRemoveCallbackɾ���豸֪ͨ�ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:12������CSUDITunerDelCallbackɾ����Ƶ��Ϣ�ص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_RDI_0002( void )
{
	int nUserData1 = 0x88888888;
	int nUserData2 = 0x11111111;
	CSUDITunerInfo_S sTunerInfo;
	CSUDI_BOOL bHasJump = CSUDI_TRUE;
	char acData[128] = {0};
	int nReadLength = 0;
	
	int nSleepCount = 0;

	s_nTunerTestData = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback(Tuner_RDICallbackTest1, &nUserData1), "����1ʧ��");

	CSTCPrint("\n����Ͽ��ƶ�Tuner����'Y����������������'\n");

	if(CSTKWaitYes())
	{
		while(!s_bRDITunerIsReady)
		{
			CSUDIOSThreadSleep(50);
			
			if(nSleepCount++ > 100)
			{
				break;
			}
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_UINT16)(s_dwRDITunerId >> 16) == 3,"����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(s_dwRDITunerId, &sTunerInfo),"����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(s_dwRDITunerId, Tuner_CallbackTest2, &nUserData2),"����5ʧ��");

		bHasJump = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(s_dwRDITunerId, &s_unDeliver[0]),"����6ʧ��");

		CSUDIOSThreadSleep(2000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "����7ʧ��");

		if(sTunerInfo.m_bPIDFilterSupport)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddPID(s_dwRDITunerId, 0),"����8ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerAddPID(s_dwRDITunerId, 0),"����8ʧ��");
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerIORead(s_dwRDITunerId, acData, sizeof(acData)/sizeof(acData[0]), &nReadLength),"����9ʧ��");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveDev(s_dwRDITunerId),"����10ʧ��");

	}

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData1), "����11ʧ��");

			if(!bHasJump)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(s_dwRDITunerId, Tuner_CallbackTest2, &nUserData2), "����12ʧ��");
			}
		}
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@CASEGROUP:CSUDITunerRDIRemoveDev 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerIORead
//@DESCRIPTION:���ƶ�Tuner�Ĳ��ǿ�Ȳ���
//@PRECONDITION:1��ϵͳ��ʼ���ɹ����ź�����
//@PRECONDITION:2�������ļ���ȷ���Ѿ��������ļ��еõ�Ҫ������ƶ�Tuner����Ϣ�Լ�Ҫ��Ƶ��Ƶ����Ϣ
//@INPUT:�ڲ���������
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDITunerRDIAddCallbackע��ص���Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:2�������ƶ�Tuner���ADD_REMOVE_COUNT�κ���Ͽ��ƶ�Tuner
//@EXECUTIONFLOW:3���ڻص������л�ȡ������豸ID��������s_dwRDITunerIdȻ���ж��豸ID�ĸ�16λ�Ƿ�Ϊ3������ɹ�������ʧ��
//@EXECUTIONFLOW:4������CSUDITunerGetDeviceInfo��ȡ�豸��Ϣ��Ҫ�󷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:5���ڻص������л�ȡ�ƶ�Tuner��ID��������CSUDITunerAddCallbackΪ���豸ע����Ƶ��Ϣ�ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:6������CSUDITunerConnectʹ�ø��ƶ��豸��ָ��Ƶ�������Ƶ��Ҫ�󷵻سɹ��������������ʧ��
//@EXECUTIONFLOW:7���ȴ�2����ж���Ƶ��Ϣ�ص�������ȡ�Ļص���Ϣ��Ҫ��ΪEM_UDITUNER_SIGNAL_CONNECTED�������������ʧ��
//@EXECUTIONFLOW:8�������Tuner֧��Ӳ����PID�����CSUDITunerAddPID���TunerӲ����PID��Ҫ�󷵻سɹ������򷵻�CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9������CSUDITunerIORead��ȡTS�����ݣ�Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:10������CSUDITunerRDIRemoveDev��ȫ�Ƴ����ƶ�Tuner��Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:11������CSUDITunerRDIRemoveCallbackɾ���豸֪ͨ�ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:12������CSUDITunerDelCallbackɾ����Ƶ��Ϣ�ص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_RDI_0003( void )
{
	int nUserData1 = 0x88888888;
	int nUserData2 = 0x11111111;
	CSUDITunerInfo_S sTunerInfo;
	CSUDI_BOOL bHasJump = CSUDI_TRUE;
	char acData[128] = {0};
	int nReadLength = 0;
	int nAddRemoveCont = ADD_REMOVE_COUNT;
	
	int nSleepCount = 0;

	s_nTunerTestData = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "��֧�ֿ��ƶ�Tuner�Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback(Tuner_RDICallbackTest1, &nUserData1), "����1ʧ��");

	CSTCPrint("\n���ο��ƶ�Tuner %d �κ��ٲ��Ͽ��ƶ�Tuner����'Y����������������'\n", nAddRemoveCont);

	if(CSTKWaitYes())
	{
		while(!s_bRDITunerIsReady)
		{
			CSUDIOSThreadSleep(50);
			
			if(nSleepCount++ > 100)
			{
				break;
			}
		}
		CSTK_ASSERT_TRUE_FATAL((CSUDI_UINT16)(s_dwRDITunerId >> 16) == 3,"����3ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(s_dwRDITunerId, &sTunerInfo),"����4ʧ��");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(s_dwRDITunerId, Tuner_CallbackTest2, &nUserData2),"����5ʧ��");

		bHasJump = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(s_dwRDITunerId, &s_unDeliver[0]),"����6ʧ��");

		CSUDIOSThreadSleep(2000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "����7ʧ��");

		if(sTunerInfo.m_bPIDFilterSupport)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddPID(s_dwRDITunerId, 0),"����8ʧ��");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerAddPID(s_dwRDITunerId, 0),"����8ʧ��");
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerIORead(s_dwRDITunerId, acData, sizeof(acData)/sizeof(acData[0]), &nReadLength),"����9ʧ��");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveDev(s_dwRDITunerId),"����10ʧ��");

	}

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData1), "����11ʧ��");

			if(!bHasJump)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(s_dwRDITunerId, Tuner_CallbackTest2, &nUserData2), "����12ʧ��");
			}
		}
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerDelCallback 
//@CASEGROUP:CSUDITunerGetSignalInfo 
//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:������Ƶʱ���ݵ�Ƶ�������ص�ʱ���ص�Ƶ�����һ��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��fnTunerCallback = Tuner_CallbackTest3
//@INPUT:3��u32SignalInfoMask = EM_UDITUNER_CUR_FREQU|EM_UDITUNER_SIGNAL_QUALITY|EM_UDITUNER_SIGNAL_STRENGTH
//@EXPECTATION:ÿһ�������Ԥ�ڽ��
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallbackע��ص�������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:3������CSUDITunerConnect��ָ��Ƶ�������Ƶ������Ҫ�󷵻سɹ�
//@EXECUTIONFLOW:4���ж��Ƿ����յ��ص���Ϣ��Ҫ���ܹ��յ�
//@EXECUTIONFLOW:5���ж���Ƶʱ���ݵ�Ƶ������Ƿ���ص�ʱ���ص�Ƶ�����һ�£������һ����ʧ��
//@EXECUTIONFLOW:6������CSUDITunerDelCallbackɾ��ע��Ļص�������Ҫ�󷵻سɹ�
CSUDI_BOOL CSTC_TUNER_IT_UnifiedFreqTrans_0001(void)
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int i = 0;
	
	s_nTunerTestData = 0;
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0], Tuner_CallbackTest3, &nUserData), "ע��ص�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "��Ƶ����ʧ��");

 	for(i = 0; i < 10; i ++)
	{
		if(s_nTunerTestData != 0)
        {
			break;
        }
        CSUDIOSThreadSleep(1000);
	}
	
	CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == 0x1234, "δ�յ��κλص���Ϣ");

	CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData != 0x5678, "��ǰ�����������ź�Ƶ���������Ƶʱ���ݵ�Ƶ�������һ��");

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0], Tuner_CallbackTest3, &nUserData), "ɾ���ص�ʧ��");

	return CSUDI_TRUE;
}


/***************************************************************************/
/*���²���������Ҫ�����������õ�Ƶ�����Ƶʱ�䣬���Ƶ����Ϣ�������ļ��л��*/
/***************************************************************************/
static CSUDI_BOOL CSTC_TUNER_IT_QamLockInvail(CSUDI_UINT32 eTunerIndex)
{
	CSUDI_BOOL bRet = CSUDI_TRUE;
	CSUDITunerSRCDeliver_U unSignalPtr;
	CSUDI_Error_Code eErr = CSUDI_SUCCESS;

	memset(&unSignalPtr,0,sizeof(CSUDITunerSRCDeliver_U));
	unSignalPtr.m_sCableDeliver.m_uFrequency = s_unDeliver[s_nDeliverCount - 1].m_sCableDeliver.m_uFrequency;
	unSignalPtr.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[s_nDeliverCount - 1].m_sCableDeliver.m_uSymbolRate_24;
	unSignalPtr.m_sCableDeliver.m_uModulation_8 = s_unDeliver[s_nDeliverCount - 1].m_sCableDeliver.m_uModulation_8;
	
	eErr = CSUDITunerConnect(eTunerIndex,&unSignalPtr);
	if(CSUDI_SUCCESS != eErr)
	{
		bRet = CSUDI_TRUE;
	}

	return bRet;
}

CSUDI_BOOL CSTC_TUNER_IT_QamFindFreq(int nFreqCnt,CSUDITunerQAMMode_E * peQAMType,CSUDITunerSRCDeliver_U * punTunerDeliver)
{
	CSUDI_BOOL eRet = CSUDI_TRUE;
	int i = 0;
	int j = 0;
	int nFreqIndex = 0;
	int nDeliverCount = 0;
	BOOL abUsedFlag[FREQUENCY_NUM_MAX] = {FALSE};

	nDeliverCount = CSTC_TUNER_GetDeliverCount();

	for(i = 0; i < nFreqCnt; i++)
	{
		j = 0;
		
		do
		{
			if((s_unDeliver[j].m_sCableDeliver.m_uModulation_8 == peQAMType[nFreqIndex]) && (!abUsedFlag[j]))
			{
				abUsedFlag[j] = TRUE;
				punTunerDeliver[nFreqIndex].m_sCableDeliver.m_uFrequency = s_unDeliver[j].m_sCableDeliver.m_uFrequency; 
				punTunerDeliver[nFreqIndex].m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[j].m_sCableDeliver.m_uSymbolRate_24;
				punTunerDeliver[nFreqIndex].m_sCableDeliver.m_uModulation_8 = s_unDeliver[j].m_sCableDeliver.m_uModulation_8;
				nFreqIndex++;			
				break;
			}
			j++;
			
		}while(j < nDeliverCount);
	}

	if(nFreqCnt != nFreqIndex)
	{
		eRet = CSUDI_FALSE;
	}

	return eRet;
}

CSUDI_BOOL CSTC_TUNER_IT_QamConnect(CSUDI_UINT32 dwTunerIndex,int nCnt,CSUDITunerSRCDeliver_U * punTunerPara)
{
	CSUDI_BOOL eRet = CSUDI_TRUE;
	int i = 0;
	int nSleepCount = 0;
	int nTotalTime = 0;
	CSUDIOSTimeVal_S sTimeStart;
	CSUDIOSTimeVal_S sTimeEnd;
	
	memset(&sTimeStart,0,sizeof(CSUDIOSTimeVal_S));
	memset(&sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

	do
	{
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));
		s_nTunerTestData = 0;
        s_dwLockCount = 0;
		s_bRightFreqPiont = FALSE;

		CSUDIOSGetTime(&sTimeStart);
		if(CSUDI_SUCCESS == CSUDITunerConnect(dwTunerIndex, &punTunerPara[i]))
		{
			while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
			{
				CSUDIOSThreadSleep(10);
				
        		if(nSleepCount++ > 200)
        		{
        			break;
        		}
			}

			if(s_nTunerTestData == TUNER_SIGNAL_CONNECTED)
			{
				CSUDIOSGetTime(&sTimeEnd);
				
				nTotalTime = ((sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 200)
				{
					CSTCPrint("\r\n[%s]��%d��Ƶ��:The Connnect Time is:%d(ms), too long\r\n",__FUNCTION__,i,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else if(s_nTunerTestData == TUNER_SIGNAL_SEARCHING)
			{
				if(s_bRightFreqPiont)
				{
					nTotalTime = ((s_sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (s_sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
					if(nTotalTime > 3000)
					{
						CSTCPrint("\r\n[%s]��%d��Ƶ��:Signal Lost Time is:%d(ms), too long\r\n",__FUNCTION__,i,nTotalTime);
					}
					eRet = CSUDI_FALSE;
				}
				break;
			}
			else
			{
				eRet = CSUDI_FALSE;
				break;
			}	
		}	
		else
		{
			eRet = CSUDI_FALSE;
			break;
		}

		i++;	
	}while(i < nCnt);

	return eRet;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerDelCallback 
//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSTC_TUNER_IT_QamLockInvail 
//@CASEGROUP:CSTC_TUNER_IT_QamFindFreq 
//@CASEGROUP:CSTC_TUNER_IT_QamConnect 
//@DESCRIPTION:��������QAM64��һ��Ƶ�����Ƶʱ�䣬Ҫ��:��Ƶ�ɹ���ʱ��С��200ms�����سɹ�������Ƶ���ɹ���Ҫ���⵽ʧ��״̬��ʱ��С��3000ms��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��nFreqCnt Ҫ���Ե�Ƶ��ĸ���
//@INPUT:2��aeQamType ���ڲ��Եĵ��Ʒ�ʽ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSTC_TUNER_IT_QamLockInvail ������һ����Ч��Ƶ�㣬Ҫ�󷵻�ֵCSUDI_TRUE������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSTC_TUNER_IT_QamFindFreq �ҵ�����Ƶ�㣬Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerAddCallback ��ӻص�Tuner_CallbackTest2��Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
//@EXECUTIONFLOW:5������CSTC_TUNER_IT_QamConnect ��Ƶ��Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:6������CSUDITunerDelCallback ע���ص���Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0001(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 1;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM64;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "��������ЧƵ��ʧ��");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"û���ҵ���Ҫ��Ƶ�㣬���������������ļ�");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "��ӻص�����ʧ��");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"��Ƶʧ��");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"ɾ���ص�ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerDelCallback 
//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSTC_TUNER_IT_QamLockInvail 
//@CASEGROUP:CSTC_TUNER_IT_QamFindFreq 
//@CASEGROUP:CSTC_TUNER_IT_QamConnect 
//@DESCRIPTION:��������QAM128��һ��Ƶ�����Ƶʱ�䣬Ҫ��:��Ƶ�ɹ���ʱ��С��200ms�����سɹ�������Ƶ���ɹ���Ҫ���⵽ʧ��״̬��ʱ��С��3000ms��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��nFreqCnt Ҫ���Ե�Ƶ��ĸ���
//@INPUT:2��aeQamType ���ڲ��Եĵ��Ʒ�ʽ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSTC_TUNER_IT_QamLockInvail ������һ����Ч��Ƶ�㣬Ҫ�󷵻�ֵCSUDI_TRUE������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSTC_TUNER_IT_QamFindFreq �ҵ�����Ƶ�㣬Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerAddCallback ��ӻص�Tuner_CallbackTest2��Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
//@EXECUTIONFLOW:5������CSTC_TUNER_IT_QamConnect ��Ƶ��Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:6������CSUDITunerDelCallback ע���ص���Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0002(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 1;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM128;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "��������ЧƵ��ʧ��");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"û���ҵ���Ҫ��Ƶ�㣬���������������ļ�");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "��ӻص�����ʧ��");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"��Ƶʧ��");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"ɾ���ص�ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerDelCallback 
//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSTC_TUNER_IT_QamLockInvail 
//@CASEGROUP:CSTC_TUNER_IT_QamFindFreq 
//@CASEGROUP:CSTC_TUNER_IT_QamConnect 
//@DESCRIPTION:��������QAM256��һ��Ƶ�����Ƶʱ�䣬Ҫ��:��Ƶ�ɹ���ʱ��С��200ms�����سɹ�������Ƶ���ɹ���Ҫ���⵽ʧ��״̬��ʱ��С��3000ms��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��nFreqCnt Ҫ���Ե�Ƶ��ĸ���
//@INPUT:2��aeQamType ���ڲ��Եĵ��Ʒ�ʽ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSTC_TUNER_IT_QamLockInvail ������һ����Ч��Ƶ�㣬Ҫ�󷵻�ֵCSUDI_TRUE������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSTC_TUNER_IT_QamFindFreq �ҵ�����Ƶ�㣬Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerAddCallback ��ӻص�Tuner_CallbackTest2��Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
//@EXECUTIONFLOW:5������CSTC_TUNER_IT_QamConnect ��Ƶ��Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:6������CSUDITunerDelCallback ע���ص���Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0003(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 1;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM256;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "��������ЧƵ��ʧ��");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"û���ҵ���Ҫ��Ƶ�㣬���������������ļ�");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "��ӻص�����ʧ��");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"��Ƶʧ��");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"ɾ���ص�ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerDelCallback 
//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSTC_TUNER_IT_QamLockInvail 
//@CASEGROUP:CSTC_TUNER_IT_QamFindFreq 
//@CASEGROUP:CSTC_TUNER_IT_QamConnect 
//@DESCRIPTION:������һ��QAM64Ƶ�������£�ȥ��������һ��QAM64��Ƶ�㣬Ҫ����Ƶ�ɹ�������ʱ��С��200ms������ʧ�ܡ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��nFreqCnt Ҫ���Ե�Ƶ��ĸ���
//@INPUT:2��aeQamType ���ڲ��Եĵ��Ʒ�ʽ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSTC_TUNER_IT_QamLockInvail ������һ����Ч��Ƶ�㣬Ҫ�󷵻�ֵCSUDI_TRUE������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSTC_TUNER_IT_QamFindFreq �ҵ�����Ƶ�㣬Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerAddCallback ��ӻص�Tuner_CallbackTest2��Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
//@EXECUTIONFLOW:5������CSTC_TUNER_IT_QamConnect ��Ƶ��Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:6������CSUDITunerDelCallback ע���ص���Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0004(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 2;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM64;
	aeQAMType[1] = EM_UDITUNER_QAM64;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "��������ЧƵ��ʧ��");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"û���ҵ���Ҫ��Ƶ�㣬���������������ļ�");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "��ӻص�����ʧ��");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"��Ƶʧ��");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"ɾ���ص�ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerDelCallback 
//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSTC_TUNER_IT_QamLockInvail 
//@CASEGROUP:CSTC_TUNER_IT_QamFindFreq 
//@CASEGROUP:CSTC_TUNER_IT_QamConnect 
//@DESCRIPTION:������һ��QAM64Ƶ�������£�ȥ��������һ��QAM128��Ƶ�㣬Ҫ����Ƶ�ɹ�������ʱ��С��200ms������ʧ�ܡ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��nFreqCnt Ҫ���Ե�Ƶ��ĸ���
//@INPUT:2��aeQamType ���ڲ��Եĵ��Ʒ�ʽ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSTC_TUNER_IT_QamLockInvail ������һ����Ч��Ƶ�㣬Ҫ�󷵻�ֵCSUDI_TRUE������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSTC_TUNER_IT_QamFindFreq �ҵ�����Ƶ�㣬Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerAddCallback ��ӻص�Tuner_CallbackTest2��Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
//@EXECUTIONFLOW:5������CSTC_TUNER_IT_QamConnect ��Ƶ��Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:6������CSUDITunerDelCallback ע���ص���Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0005(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 2;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM64;
	aeQAMType[1] = EM_UDITUNER_QAM128;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "��������ЧƵ��ʧ��");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"û���ҵ���Ҫ��Ƶ�㣬���������������ļ�");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "��ӻص�����ʧ��");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"��Ƶʧ��");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"ɾ���ص�ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerDelCallback 
//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSTC_TUNER_IT_QamLockInvail 
//@CASEGROUP:CSTC_TUNER_IT_QamFindFreq 
//@CASEGROUP:CSTC_TUNER_IT_QamConnect 
//@DESCRIPTION:������һ��QAM128Ƶ�������£�ȥ��������һ��QAM64��Ƶ�㣬Ҫ����Ƶ�ɹ�������ʱ��С��200ms������ʧ�ܡ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��nFreqCnt Ҫ���Ե�Ƶ��ĸ���
//@INPUT:2��aeQamType ���ڲ��Եĵ��Ʒ�ʽ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSTC_TUNER_IT_QamLockInvail ������һ����Ч��Ƶ�㣬Ҫ�󷵻�ֵCSUDI_TRUE������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSTC_TUNER_IT_QamFindFreq �ҵ�����Ƶ�㣬Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerAddCallback ��ӻص�Tuner_CallbackTest2��Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
//@EXECUTIONFLOW:5������CSTC_TUNER_IT_QamConnect ��Ƶ��Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:6������CSUDITunerDelCallback ע���ص���Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0006(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 2;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM128;
	aeQAMType[1] = EM_UDITUNER_QAM64;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "��������ЧƵ��ʧ��");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"û���ҵ���Ҫ��Ƶ�㣬���������������ļ�");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "��ӻص�����ʧ��");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"��Ƶʧ��");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"ɾ���ص�ʧ��");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerDelCallback 
//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSTC_TUNER_IT_QamLockInvail 
//@CASEGROUP:CSTC_TUNER_IT_QamFindFreq 
//@CASEGROUP:CSTC_TUNER_IT_QamConnect 
//@DESCRIPTION:������һ��QAM256Ƶ�������£�ȥ��������һ��QAM64��Ƶ�㣬Ҫ����Ƶ�ɹ�������ʱ��С��200ms������ʧ�ܡ�
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��nFreqCnt Ҫ���Ե�Ƶ��ĸ���
//@INPUT:2��aeQamType ���ڲ��Եĵ��Ʒ�ʽ
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSTC_TUNER_IT_QamLockInvail ������һ����Ч��Ƶ�㣬Ҫ�󷵻�ֵCSUDI_TRUE������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSTC_TUNER_IT_QamFindFreq �ҵ�����Ƶ�㣬Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerAddCallback ��ӻص�Tuner_CallbackTest2��Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
//@EXECUTIONFLOW:5������CSTC_TUNER_IT_QamConnect ��Ƶ��Ҫ�󷵻�CSUDI_TRUE�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:6������CSUDITunerDelCallback ע���ص���Ҫ�󷵻�CSUDI_SUCCESS������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0007(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 2;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM128;
	aeQAMType[1] = EM_UDITUNER_QAM64;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "��������ЧƵ��ʧ��");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"û���ҵ���Ҫ��Ƶ�㣬���������������ļ�");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "��ӻص�����ʧ��");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"��Ƶʧ��");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"ɾ���ص�ʧ��");
		}
	}

	return CSUDI_TRUE;
}


#if 0
/***************************************************************************/
/*���²���������Ҫ����������QPSK����Ƶʱ��                                 */
/*���еĲ����������ǽ�����������                                           */
/***************************************************************************/

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:��QPSK��ʼ���ɹ���ǰ���£�����freqΪ950000Khz,symbolΪ4000kbs��Ƶ�㣬Ҫ���ܹ���Ƶ�ɹ���������Ƶʱ��С��600ms��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��s_unDeliver[0].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency ��Ҫ��Ƶ��Ƶ���freq.
//@INPUT:3��s_unDeliver[0].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 ��Ҫ��Ƶ��Ƶ���symbol.
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallback ����ӻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSUDITunerConnect�������õ�Ƶ�㣬Ҫ�󷵻�CSUDI_SUCCESS�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ����ӵĻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QpskConnectTime_0001(void)
{
	CSUDI_BOOL eRet = CSUDI_TRUE;
	int i = 0;
	int nActIdCnt = 0;
	int nSleepCount = 0;
	int nAddCallbackCount = 0;
	int nTotalTime = 0; 
	CSUDIOSTimeVal_S sTimeStart;
	CSUDIOSTimeVal_S sTimeEnd;
	CSUDI_UINT32 adwTunerIds[20] = {0};

	memset(&sTimeStart,0,sizeof(CSUDIOSTimeVal_S));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "��ӻص�����ʧ��");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"��Ƶʧ��");

			while(EM_UDITUNER_SIGNAL_CONNECTED != s_nTunerTestData)
			{
				CSUDIOSThreadSleep(10);
				
        		if(nSleepCount++ > 200)
        		{
        			break;
        		}
			}
			
			if(EM_UDITUNER_SIGNAL_CONNECTED == s_nTunerTestData)
			{
				CSUDIOSGetTime(&sTimeEnd);
				
				nTotalTime = ((sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 600)
				{
					CSTCPrint("\r\n[%s]The Connect Time:%d, too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else if(EM_UDITUNER_SIGNAL_SEARCHING == s_nTunerTestData)
			{
				nTotalTime = ((s_sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (s_sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 3000)
				{
					CSTCPrint("\r\n[%s]Signal Lost Time is:%d,too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else
			{
				eRet = CSUDI_FALSE;
				break;
			}	
		}
		
		i++;	
	}while(i < nActIdCnt);

	CSTK_FATAL_POINT
	{
		for(i = 0 ; i < nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"ɾ���ص�ʧ��");
		}
	}

	return eRet;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:��QPSK��ʼ���ɹ���ǰ���£�����freqΪ950000Khz,symbolΪ45000kbs��Ƶ�㣬Ҫ���ܹ���Ƶ�ɹ���������Ƶʱ��С��600ms��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��s_unDeliver[1].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency ��Ҫ��Ƶ��Ƶ���freq.
//@INPUT:3��s_unDeliver[1].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 ��Ҫ��Ƶ��Ƶ���symbol.
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallback ����ӻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSUDITunerConnect�������õ�Ƶ�㣬Ҫ�󷵻�CSUDI_SUCCESS�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ����ӵĻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QpskConnectTime_0002(void)
{
	CSUDI_BOOL eRet = CSUDI_TRUE;
	int i = 0;
	int nActIdCnt = 0;
	int nSleepCount = 0;
	int nAddCallbackCount = 0;
	int nTotalTime = 0; 
	CSUDIOSTimeVal_S sTimeStart;
	CSUDIOSTimeVal_S sTimeEnd;
	CSUDI_UINT32 adwTunerIds[20] = {0};

	memset(&sTimeStart,0,sizeof(CSUDIOSTimeVal_S));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "��ӻص�����ʧ��");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[1]),"��Ƶʧ��");

			while(EM_UDITUNER_SIGNAL_CONNECTED != s_nTunerTestData)
			{
				CSUDIOSThreadSleep(10);
				
        		if(nSleepCount++ > 200)
        		{
        			break;
        		}
			}
			
			if(EM_UDITUNER_SIGNAL_CONNECTED == s_nTunerTestData)
			{
				CSUDIOSGetTime(&sTimeEnd);
				
				nTotalTime = ((sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 300)
				{
					CSTCPrint("\r\n[%s]The Connect Time:%d, too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else if(EM_UDITUNER_SIGNAL_SEARCHING == s_nTunerTestData)
			{
				nTotalTime = ((s_sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (s_sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 3000)
				{
					CSTCPrint("\r\n[%s]Signal Lost Time is:%d,too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else
			{
				eRet = CSUDI_FALSE;
				break;
			}	
		}
		
		i++;	
	}while(i < nActIdCnt);

	CSTK_FATAL_POINT
	{
		for(i = 0 ; i < nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"ɾ���ص�ʧ��");
		}
	}

	return eRet;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:��QPSK��ʼ���ɹ���ǰ���£�����freqΪ1370000Khz,symbolΪ20000kbs��Ƶ�㣬Ҫ���ܹ���Ƶ�ɹ���������Ƶʱ��С��600ms��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��s_unDeliver[2].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency ��Ҫ��Ƶ��Ƶ���freq.
//@INPUT:3��s_unDeliver[2].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 ��Ҫ��Ƶ��Ƶ���symbol.
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallback ����ӻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSUDITunerConnect�������õ�Ƶ�㣬Ҫ�󷵻�CSUDI_SUCCESS�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ����ӵĻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QpskConnectTime_0003(void)
{
	CSUDI_BOOL eRet = CSUDI_TRUE;
	int i = 0;
	int nActIdCnt = 0;
	int nSleepCount = 0;
	int nAddCallbackCount = 0;
	int nTotalTime = 0; 
	CSUDIOSTimeVal_S sTimeStart;
	CSUDIOSTimeVal_S sTimeEnd;
	CSUDI_UINT32 adwTunerIds[20] = {0};

	memset(&sTimeStart,0,sizeof(CSUDIOSTimeVal_S));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "��ӻص�����ʧ��");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[2]),"��Ƶʧ��");

			while(EM_UDITUNER_SIGNAL_CONNECTED != s_nTunerTestData)
			{
				CSUDIOSThreadSleep(10);
				
        		if(nSleepCount++ > 200)
        		{
        			break;
        		}
			}
			
			if(EM_UDITUNER_SIGNAL_CONNECTED == s_nTunerTestData)
			{
				CSUDIOSGetTime(&sTimeEnd);
				
				nTotalTime = ((sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 300)
				{
					CSTCPrint("\r\n[%s]The Connect Time:%d, too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else if(EM_UDITUNER_SIGNAL_SEARCHING == s_nTunerTestData)
			{
				nTotalTime = ((s_sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (s_sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 3000)
				{
					CSTCPrint("\r\n[%s]Signal Lost Time is:%d,too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else
			{
				eRet = CSUDI_FALSE;
				break;
			}	
		}
		
		i++;	
	}while(i < nActIdCnt);

	CSTK_FATAL_POINT
	{
		for(i = 0 ; i < nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"ɾ���ص�ʧ��");
		}
	}

	return eRet;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:��QPSK��ʼ���ɹ���ǰ���£�����freqΪ2150000Khz,symbolΪ4000kbs��Ƶ�㣬Ҫ���ܹ���Ƶ�ɹ���������Ƶʱ��С��600ms��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��s_unDeliver[3].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency ��Ҫ��Ƶ��Ƶ���freq.
//@INPUT:3��s_unDeliver[3].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 ��Ҫ��Ƶ��Ƶ���symbol.
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallback ����ӻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSUDITunerConnect�������õ�Ƶ�㣬Ҫ�󷵻�CSUDI_SUCCESS�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ����ӵĻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QpskConnectTime_0004(void)
{
	CSUDI_BOOL eRet = CSUDI_TRUE;
	int i = 0;
	int nActIdCnt = 0;
	int nSleepCount = 0;
	int nAddCallbackCount = 0;
	int nTotalTime = 0; 
	CSUDIOSTimeVal_S sTimeStart;
	CSUDIOSTimeVal_S sTimeEnd;
	CSUDI_UINT32 adwTunerIds[20] = {0};

	memset(&sTimeStart,0,sizeof(CSUDIOSTimeVal_S));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "��ӻص�����ʧ��");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[3]),"��Ƶʧ��");

			while(EM_UDITUNER_SIGNAL_CONNECTED != s_nTunerTestData)
			{
				CSUDIOSThreadSleep(10);
				
        		if(nSleepCount++ > 200)
        		{
        			break;
        		}
			}
			
			if(EM_UDITUNER_SIGNAL_CONNECTED == s_nTunerTestData)
			{
				CSUDIOSGetTime(&sTimeEnd);
				
				nTotalTime = ((sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 600)
				{
					CSTCPrint("\r\n[%s]The Connect Time:%d, too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else if(EM_UDITUNER_SIGNAL_SEARCHING == s_nTunerTestData)
			{
				nTotalTime = ((s_sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (s_sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 3000)
				{
					CSTCPrint("\r\n[%s]Signal Lost Time is:%d,too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else
			{
				eRet = CSUDI_FALSE;
				break;
			}	
		}
		
		i++;	
	}while(i < nActIdCnt);

	CSTK_FATAL_POINT
	{
		for(i = 0 ; i < nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"ɾ���ص�ʧ��");
		}
	}

	return eRet;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:��QPSK��ʼ���ɹ���ǰ���£�����freqΪ2150000Khz,symbolΪ45000kbs��Ƶ�㣬Ҫ���ܹ���Ƶ�ɹ���������Ƶʱ��С��600ms��
//@PRECONDITION:1���豸��ʼ���ɹ�
//@INPUT:1��dwTunerId ͨ��CSUDITunerGetAllDeviceId��ȡ
//@INPUT:2��s_unDeliver[4].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency ��Ҫ��Ƶ��Ƶ���freq.
//@INPUT:3��s_unDeliver[4].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 ��Ҫ��Ƶ��Ƶ���symbol.
//@EXECUTIONFLOW:1������CSUDITunerGetAllDeviceId ��ȡ��ǰ���е��豸ID������ID����pdwIds�У�Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ��
//@EXECUTIONFLOW:2������CSUDITunerAddCallback ����ӻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
//@EXECUTIONFLOW:3������CSUDITunerConnect�������õ�Ƶ�㣬Ҫ�󷵻�CSUDI_SUCCESS�����򷵻�ʧ�ܡ�
//@EXECUTIONFLOW:4������CSUDITunerDelCallbackɾ����ӵĻص���Ҫ�󷵻�CSUDI_SUCCESS�������������ʧ�ܡ�
CSUDI_BOOL CSTC_TUNER_IT_QpskConnectTime_0005(void)
{
	CSUDI_BOOL eRet = CSUDI_TRUE;
	int i = 0;
	int nActIdCnt = 0;
	int nSleepCount = 0;
	int nAddCallbackCount = 0;
	int nTotalTime = 0; 
	CSUDIOSTimeVal_S sTimeStart;
	CSUDIOSTimeVal_S sTimeEnd;
	CSUDI_UINT32 adwTunerIds[20] = {0};

	memset(&sTimeStart,0,sizeof(CSUDIOSTimeVal_S));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner������Ϊ0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "��ӻص�����ʧ��");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[4]),"��Ƶʧ��");

			while(EM_UDITUNER_SIGNAL_CONNECTED != s_nTunerTestData)
			{
				CSUDIOSThreadSleep(10);
				
        		if(nSleepCount++ > 200)
        		{
        			break;
        		}
			}
			
			if(EM_UDITUNER_SIGNAL_CONNECTED == s_nTunerTestData)
			{
				CSUDIOSGetTime(&sTimeEnd);
				
				nTotalTime = ((sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 300)
				{
					CSTCPrint("\r\n[%s]The Connect Time:%d, too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else if(EM_UDITUNER_SIGNAL_SEARCHING == s_nTunerTestData)
			{
				nTotalTime = ((s_sTimeEnd.m_nSecond - sTimeStart.m_nSecond) * 1000 + (s_sTimeEnd.m_nMiliSecond - sTimeStart.m_nMiliSecond));
				if(nTotalTime > 3000)
				{
					CSTCPrint("\r\n[%s]Signal Lost Time is:%d,too long\r\n",__FUNCTION__,nTotalTime);
					eRet = CSUDI_FALSE;
					break;
				}
			}
			else
			{
				eRet = CSUDI_FALSE;
				break;
			}	
		}
		
		i++;	
	}while(i < nActIdCnt);

	CSTK_FATAL_POINT
	{
		for(i = 0 ; i < nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"ɾ���ص�ʧ��");
		}
	}

	return eRet;
}
#endif

