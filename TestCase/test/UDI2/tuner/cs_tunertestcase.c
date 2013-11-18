/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
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
		CSTCPrint("\n回调消息psMessage为空!!!!\n");
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
		CSTCPrint("\n回调消息psMessage为空!!!!\n");
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
			CSTCPrint("\n 回调时返回的频点参数和锁频时传递的频点参数不一致");
		}
	}
	else
	{
		CSTCPrint("\n回调消息psMessage为空!!!!\n");
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
    	*(int*)(psMessage->m_pvUserData) = 0x1234;//CSUDITunerGetSignalInfo失败的情况
    }
    
    *(int*)(psMessage->m_pvUserData) = 0x5678;//正常退出

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
		CSTCPrint("\n回调消息psMessage为空!!!!\n");
	}

	g_bIsTunerConnected = CSUDI_TRUE;//本用例用来标记函数返回，不关注锁频结果

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
	//在本测试用例集执行前调用
	CSTC_TUNER_InitCfg();
	CSTC_TUNER_Deliver_Init(s_unDeliver,sizeof(s_unDeliver)/sizeof(s_unDeliver[0]));
	CSTC_TUNER_TunerInfo_Init(s_asTunerInfo, sizeof(s_asTunerInfo)/sizeof(s_asTunerInfo[0]));
	CSTC_TUNER_TunerParam_Init(&s_nActTunerCnt, &s_nDeliverCount,&s_bIsSupportRDITuner);
	
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_TUNER_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:测试pdwIds为空时得到的实际设备ID数是否正确
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到当前的实际设备ID数
//@INPUT:nMaxIdCnt = 50
//@EXPECTATION:成功获取实际设备ID数，且与配置文件中的实际ID数相等
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前的实际设备ID数，pdwIds为空，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、比较获取的设备ID数与配置文件中指定的实际设备ID数是否一致，不一致则测试用例失败
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0001( void )
{
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(CSUDI_NULL,50,&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt == s_nActTunerCnt, "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:测试参数全部合法时得到的实际设备的ID是否正确
//@PRECONDITION:1、设备初始化成功并为每个设备分配了ID
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到当前的实际设备ID数
//@INPUT:nMaxIdCnt = 20
//@EXPECTATION:成功获取实际设备ID，且与实际分配的ID相等
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、比较获取的设备ID数与配置文件中指定的实际设备ID数是否一致，不一致则测试用例失败
//@EXECUTIONFLOW:3、比较获取的ID是否和实际设备ID相等，不相等则测试用例失败
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt == s_nActTunerCnt, "步骤2失败");

	for(i = 0 ; i < nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(adwTunerIds[i] == i, "步骤3失败");
	}

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:测试参数nMaxIdCnt为0时能否成功获取实际设备ID数
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到当前的实际设备ID数
//@INPUT:nMaxIdCnt = 0
//@EXPECTATION:成功获取实际设备ID数，且与配置文件中的实际ID数相等
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前的实际设备ID数，pdwIds为空，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、比较获取的设备ID数与配置文件中指定的实际设备ID数是否一致，不一致则测试用例失败
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0003( void )
{
	int nActIdCnt = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(CSUDI_NULL,0,&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt == s_nActTunerCnt, "步骤2失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:测试参数nMaxIdCnt为0时能否成功获取实际设备的ID
//@PRECONDITION:1、设备初始化成功并为每个设备分配了ID
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到当前的实际设备ID数
//@INPUT:nMaxIdCnt = 0
//@EXPECTATION:无法获取设备ID，只能得到ID数
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、比较获取的设备ID数与配置文件中指定的实际设备ID数是否一致，不一致则测试用例失败
//@EXECUTIONFLOW:3、判断pdwIds中的数值，要求仍为初始值(pdwIds[0]:11,pdwIds[1]:22)，而并未存入设备ID，否则测试用例失败
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0004( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[2] = {11,22};

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,0,&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt == s_nActTunerCnt, "步骤2失败");

	CSTK_ASSERT_TRUE_FATAL(adwTunerIds[0] == 11,"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(adwTunerIds[1] == 22,"步骤3失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@DESCRIPTION:测试参数pnActIdCnt为空时能否成功获取当前实际设备ID数
//@PRECONDITION:设备初始化成功
//@INPUT:nMaxIdCnt = 50
//@EXPECTATION:获取失败返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前的实际设备ID数，要求返回错误代码值(非CSUDI_SUCCESS)
CSUDI_BOOL CSTC_TUNER_IT_GetAllDeviceId_0005( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerGetAllDeviceId(CSUDI_NULL,50,CSUDI_NULL), "步骤1失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetDeviceInfo 
//@DESCRIPTION:测试参数dwTunerId非法时能否成功获取设备的能力信息
//@PRECONDITION:设备初始化成功
//@INPUT:dwTunerId = 0x1234
//@EXPECTATION:获取失败返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDITunerGetDeviceInfo获取设备ID非法的设备能力信息要求返回错误代码值(非CSUDI_SUCCESS)
CSUDI_BOOL CSTC_TUNER_IT_GetDeviceInfo_0001( void )
{
	CSUDITunerInfo_S sTunerInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerGetDeviceInfo(0x1234,&sTunerInfo), "步骤1失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetDeviceInfo 
//@DESCRIPTION:测试参数全部正确时能否成功获取所有设备的能力信息
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到当前Tuner的能力信息
//@INPUT:dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@EXPECTATION:获取成功并所有能力信息正确
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerGetDeviceInfo获取当前所有设备的能力信息，并返回成功，否则测试用例失败
//@EXECUTIONFLOW:3、比较获取的所有设备的能力信息与配置文件中指定的能力信息是否一致，不一致则测试失败
CSUDI_BOOL CSTC_TUNER_IT_GetDeviceInfo_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerInfo_S asTunerInfo[5];
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for (i = 0 ; i < nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(adwTunerIds[i],&asTunerInfo[i]), "步骤2失败");

		CSTK_ASSERT_TRUE_FATAL(CSTC_TUNER_InfoCmp(asTunerInfo[i],s_asTunerInfo[i]),"步骤3失败");
	}

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDITunerGetDeviceInfo 
//@DESCRIPTION:测试psTunerInfo为空时能否成功获取所有设备的能力信息
//@PRECONDITION:设备初始化成功
//@INPUT:dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@EXPECTATION:获取失败返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerGetDeviceInfo获取当前所有设备的能力信息，返回失败
CSUDI_BOOL CSTC_TUNER_IT_GetDeviceInfo_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
		
	for (i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerGetDeviceInfo(adwTunerIds[i],CSUDI_NULL), "步骤2失败");
	}
	
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:测试参数全部合法时能否注册回调成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData = 0x88888888
//@EXPECTATION:注册成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、对所有的有效Tuner Id调用CSUDITunerAddCallback注册回调，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对指定的频点进行锁频操作，要求返回成功
//@EXECUTIONFLOW:4、等待5秒后判断回调中获取的用户数据和传入的是否一致，一致则返回成功，否则返回失败
//@EXECUTIONFLOW:5、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0001( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{		
		s_nTunerTestData = 0;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest1,&nUserData), "步骤2失败");

		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤3失败");

		CSTCPrint("请等待5秒\n");

		CSUDIOSThreadSleep(5000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == 0x88888888,"步骤4失败");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest1,&nUserData),"步骤5失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:测试dwTunerId非法时能否注册回调成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId  = 0x1234
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData = 0x88888888
//@EXPECTATION:注册失败
//@EXECUTIONFLOW:1、调用CSUDITunerAddCallback注册回调，要求返回失败
//@EXECUTIONFLOW:2、如果步骤1返回成功则调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0002( void )
{
	int nUserData = 0x88888888;
	CSUDI_BOOL bHasJump = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerAddCallback(0x1234,Tuner_CallbackTest1,&nUserData), "步骤1失败");

	bHasJump = CSUDI_FALSE;

	CSTK_FATAL_POINT
	{
		if(bHasJump)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(0x1234,Tuner_CallbackTest1,&nUserData),"步骤3失败");
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:测试fnTunerCallback非法时能否注册回调成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = CSUDI_NULL
//@INPUT:3、UserData = 0x88888888
//@EXPECTATION:注册失败
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调，要求返回失败
//@EXECUTIONFLOW:3、如果步骤1返回成功则调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	CSUDI_BOOL bHasJump[2] = {CSUDI_TRUE, CSUDI_TRUE};
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i = 0 ; i < nActIdCnt ; i++)
	{		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerAddCallback(adwTunerIds[i],CSUDI_NULL,&nUserData), "步骤2失败");

		bHasJump[i] = CSUDI_FALSE;
	}
	
	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nActIdCnt ; i++)
		{
			if(bHasJump[i])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],CSUDI_NULL,&nUserData),"步骤3失败");
			}
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:测试UserData为空时能否注册回调成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData = CSUDI_NULL
//@EXPECTATION:注册成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对指定的频点进行锁频操作，要求返回成功
//@EXECUTIONFLOW:4、在回调中判断UserData，如果为Null则将s_nTunerTestData赋值为0x1234
//@EXECUTIONFLOW:5、等待5秒后判断s_nTunerTestData是否为0x1234，相等则则返回成功，否则返回失败
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0004( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i = 0 ; i < nActIdCnt ; i++)
	{
		s_nTunerTestData = 0;
				
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest1,CSUDI_NULL), "步骤2失败");

		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤3失败");

		CSTCPrint("请等待5秒\n");

		CSUDIOSThreadSleep(5000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == 0x1234,"步骤5失败");
	}
	
	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest1,CSUDI_NULL),"步骤6失败");
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:测试参数全部合法时为同一Tuner注册8个回调函数能否注册回调成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData = anUserData[i]
//@EXPECTATION:全部注册成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback连续注册8个回调函数，注册的函数句柄相同但UserData不同，要求全部返回成功，否则测试用例失败
//@EXECUTIONFLOW:3、调用CSUDITunerDelCallback删除注册成功的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0005( void )
{
	int nActIdCnt;
	CSUDI_UINT32 adwTunerIds[20];
	int anUserData[8] = {0};
	int nAddCallbackCount = 0;
	int nIdCount = 0;
	int nLoopCount = 0;
	int i,j;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	for(i=0 ; i < nActIdCnt ; i++)
	{
		nIdCount++;
		
		for(j = 0 ; j < 8 ; j++)
		{			
			anUserData[j] = i * 10 + j;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest1,&anUserData[j]), "步骤2失败");

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
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest1,&anUserData[j]),"步骤3失败");
				}
			}
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:测试注册已经注册的回调能否成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData = nUserData
//@EXPECTATION:注册失败返回CSUDITUNER_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback对同一回调连续注册两次，要求第一次返回成功，第二次返回CSUDITUNER_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW:3、调用CSUDITunerDelCallback删除注册成功的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0006( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "第一次注册失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_ALREADY_ADDED == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "步骤2失败");

	CSTK_FATAL_POINT
	{
		if(nActIdCnt > 0)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"步骤3失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:测试注册dwTunerId和UserData都相同而fnTunerCallback不同的回调能否成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1/Tuner_CallbackTest2
//@INPUT:3、UserData = nUserData
//@EXPECTATION:期望注册成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册fnTunerCallback = Tuner_CallbackTest1的回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerAddCallback对同一dwTunerId注册fnTunerCallback = Tuner_CallbackTest2的回调函数，要求返回成功
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除注册成功的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0007( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "步骤2失败");

	nAddCallbackCount++;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData), "步骤3失败");

	nAddCallbackCount++;

	CSTK_FATAL_POINT
	{
		if(nAddCallbackCount >= 2)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"步骤4失败");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData),"步骤4失败");
		}
		else if(nAddCallbackCount >= 1)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"步骤4失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:测试在锁频回调中获取信号信息是否会存在异常(例如死锁)
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest4
//@INPUT:3、UserData = nUserData
//@EXPECTATION:期望不出现异常
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册fnTunerCallback = Tuner_CallbackTest4的回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect锁定一个频点，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、在锁屏回调函数中调用CSUDITunerGetSignalInfo获取信号信息，期望获取成功并且回调正常退出
//@EXECUTIONFLOW:5、调用CSUDITunerDelCallback删除注册成功的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0008( void )
{
    int nActIdCnt = 0;
    CSUDI_UINT32 adwTunerIds[20];
    int nUserData = 0x1111;
    int nAddCallbackCount = 0;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");

    CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0], Tuner_CallbackTest4, &nUserData), "步骤2失败");
    
    nAddCallbackCount++;
    
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "步骤3失败");

	CSTCPrint("请等待2s ...\n");
    
	CSUDIOSThreadSleep(2000);
    
    CSTK_ASSERT_TRUE_FATAL(0x5678 == nUserData, "步骤4失败");

    CSTK_FATAL_POINT
    {
        if(nAddCallbackCount >= 1)
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0], Tuner_CallbackTest4, &nUserData), "步骤5失败");
        }
    }

    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@DESCRIPTION:测试在锁频回调中删除回调本身的情况
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest4
//@INPUT:3、UserData = nUserData
//@EXPECTATION:期望不出现异常
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册Tuner_CallbackTest5的回调函数，该函数中删除回调本身
//@EXECUTIONFLOW:3、调用CSUDITunerConnect锁定一个频点，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_TUNER_IT_AddCallback_0009( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	
	CSTCPrint("该用例可能会出现异常(如死机，死锁等)，若等待5秒用例没有结束，则用例失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0], Tuner_CallbackTest6, &s_nUserData), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "步骤3失败");

	CSTCPrint("请等待2s ...\n");
	
	CSUDIOSThreadSleep(2000);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE != s_bCallBackFlag, "回调中，调用CSUDITunerDelCallback失败!");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:测试dwTunerId非法时能否删除回调成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId  = 0x1234
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData = 0x88888888
//@EXPECTATION:删除失败
//@EXECUTIONFLOW:1、调用CSUDITunerDelCallback删除设备ID非法的回调函数，要求返回失败
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0001( void )
{
	int nUserData = 0x88888888;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(0x1234,Tuner_CallbackTest1,&nUserData),"步骤1失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:测试fnTunerCallback非法时能否删除回调成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = CSUDI_NULL
//@INPUT:3、UserData = 0x88888888
//@EXPECTATION:删除失败
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerDelCallback删除fnTunerCallback非法的回调函数，要求返回失败
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0002( void )
{
	int nUserData = 0x88888888;
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(adwTunerIds[0],CSUDI_NULL,&nUserData),"步骤2失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:测试删除没有注册的回调函数是否成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData = 0x88888888
//@EXPECTATION:删除失败
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerDelCallback删除没有注册的回调函数Tuner_CallbackTest1，要求返回CSUDI_FAILURE
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0003( void )
{
	int nUserData = 0x88888888;
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"步骤2失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:测试删除UserData与注册时不一致的回调函数是否成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData1 = 0x88888888, UserData2 = 0x11111111
//@EXPECTATION:删除失败
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调，UserData为UserData1，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerDelCallback删除注册的回调函数Tuner_CallbackTest1，UserData为UserData2，要求返回失败
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除注册的回调函数Tuner_CallbackTest1，UserData为UserData1，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0004( void )
{
	int nUserData1 = 0x88888888;
	int nUserData2 = 0x11111111;
	int nActIdCnt = 0;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData1), "步骤2失败");
	
	nAddCallbackCount++;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData2),"步骤3失败");

	CSTK_FATAL_POINT
	{
		if (nAddCallbackCount != 0)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData1),"步骤4失败");
		}
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:测试删除已经删除的回调函数是否成功
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData = 0x88888888
//@EXPECTATION:删除失败
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数Tuner_CallbackTest1，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerDelCallback删除注册的回调函数Tuner_CallbackTest1，要求返回成功
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback再次删除注册的回调函数Tuner_CallbackTest1，要求返回失败
CSUDI_BOOL CSTC_TUNER_IT_DelCallback_0005( void )
{
	int nUserData = 0x88888888;
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "步骤2失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"步骤3失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"步骤4失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:测试多任务之间注册和删除回调函数
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1、Tuner_CallbackTest2
//@INPUT:3、UserData = 0
//@EXPECTATION:每一步都返回期望值
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDIOSThreadCreate创建两个子任务，要求返回成功
//@EXECUTIONFLOW:3、在子任务1中调用CSUDITunerAddCallback为Tuner0注册回调函数1，等待2秒后调用CSUDITunerDelCallback删除Tuner0已经注册的回调函数2，要求全部成功
//@EXECUTIONFLOW:4、在子任务2中调用CSUDITunerAddCallback为Tuner0注册回调函数2，等待2秒后调用CSUDITunerDelCallback删除Tuner0已经注册的回调函数1，要求全部成功
//@EXECUTIONFLOW:5、删除创建的任务，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_AddDelCallback_0001( void )
{	
	int nUserData1 = 0x88888888;
	int nUserData2 = 0x11111111; 
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;

	TunerCallbackParam_S asTunerCallbackParam[2];
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

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
								"步骤2创建子任务1失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread1_2", 64, 8*1024, Tuner_TestThread1_2Entry, 
								asTunerCallbackParam, &hThreadhandle2),
								"步骤2创建子任务2失败");

	CSUDIOSThreadSleep(3000);

	CSTK_ASSERT_TRUE_FATAL(asTunerCallbackParam[0].bThreadFlag == CSUDI_TRUE, "子任务1失败");

	CSTK_ASSERT_TRUE_FATAL(asTunerCallbackParam[1].bThreadFlag == CSUDI_TRUE, "子任务1失败");
	
	CSTK_FATAL_POINT
	{
		if(hThreadhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除子任务1失败");
			hThreadhandle1 = CSUDI_NULL;
		}
		if(hThreadhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除子任务2失败");
			hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDITunerAddCallback
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:回调函数的注册和删除强度测试
//@PRECONDITION:设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest1
//@INPUT:3、UserData = 0x88888888
//@EXPECTATION:每一步都返回期望值
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数Tuner_CallbackTest1，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerDelCallback删除注册的回调函数Tuner_CallbackTest1，要求返回成功
//@EXECUTIONFLOW:4、重复执行2、3步TESTCOUNT次(TESTCOUNT根据具体情况而定)
CSUDI_BOOL CSTC_TUNER_IT_AddDelCallback_0002( void )
{
	int nUserData = 0x88888888;
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];  
	int i;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	for(i=0 ; i<TEST_COUNT; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData), "步骤2失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest1,&nUserData),"步骤3失败");
	}

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试dwTunerId非法时对指定频点进行锁频能否成功
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@INPUT:1、dwTunerId  = 0x1234
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:锁频失败
//@EXECUTIONFLOW:1、调用CSUDITunerConnect对指定频点进行锁频操作，返回失败
CSUDI_BOOL CSTC_TUNER_IT_Connect_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerConnect(0x1234,&s_unDeliver[0]),"步骤1失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试punDeliver为空时对指定频点进行锁频能否成功
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :CSUDI_NULL
//@EXPECTATION:锁频失败
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect进行锁频操作，要求返回失败
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:5、遍历dwTunerId并执行步骤2~4
CSUDI_BOOL CSTC_TUNER_IT_Connect_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for (i=0 ; i<nActIdCnt ; i++)
	{
             if(adwTunerIds[i] != -1)
             { 
                CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest1,&nUserData), "步骤2失败");

		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerConnect(adwTunerIds[i], CSUDI_NULL),"步骤3失败");
             }
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest1,&nUserData),"步骤6失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试参数正常时对指定频点进行锁频能否成功
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:锁频成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、在回调函数中如果判断到锁频成功则将s_nTunerTestData赋为1001
//@EXECUTIONFLOW:4、调用CSUDITunerConnect进行锁频操作，要求返回成功，且回调消息是EM_UDITUNER_SIGNAL_CONNECTED，并将s_nTunerTestData赋为1001
//@EXECUTIONFLOW:5、等待5秒后判断s_nTunerTestData的值，要求为1001，否则测试用例失败
//@EXECUTIONFLOW:6、判断s_bIsSignalLost和s_bIsSignalSearching是否都为CSUDI_FALSE，期望是，否则返回失败
//@EXECUTIONFLOW:7、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:8、遍历dwTunerId并执行步骤2~6
CSUDI_BOOL CSTC_TUNER_IT_Connect_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;
			s_bIsSignalLost = CSUDI_FALSE;
            
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤4失败");

			CSTCPrint("请等待5秒\n");
            
            CSUDIOSThreadSleep(5000);

            CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "步骤5失败");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == s_bIsSignalLost, "步骤6失败，此时不应该有EM_UDITUNER_SIGNAL_LOST消息上抛");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == s_bIsSignalSearching, "步骤6失败，此时不应该有EM_UDITUNER_SIGNAL_SEARCHING消息上抛");
		}
	} 

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤7失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试频点非法时(不在47M~867M范围内DVB-C标准)进行锁频能否成功
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :非法的频点信息(频率为1000KHz)
//@EXPECTATION:锁频失败并返回非CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect进行锁频操作，要求返回失败
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:5、遍历dwTunerId并执行步骤2~4
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_BAD_PARAMETER == CSUDITunerConnect(adwTunerIds[i], &unDeliver),"步骤3失败");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤4失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试频点无效时进行锁频能否成功
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :无效的频点信息(频率为44400KHz)
//@EXPECTATION:锁频失败且回调一直收到EM_UDITUNER_SIGNAL_SEARCHING消息但CSUDITunerConnect的返回值应该是CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功，且回调消息是EM_UDITUNER_SIGNAL_SEARCHING，并将s_nTunerTestData赋为3003
//@EXECUTIONFLOW:3、调用CSUDITunerConnect进行锁频操作，要求返回CSUDI_SUCCESS，但无法锁住该频点，回调消息一直是EM_UDITUNER_SIGNAL_SEARCHING
//@EXECUTIONFLOW:4、等待5秒后判断s_nTunerTestData的值，要求为3003，否则测试用例失败
//@EXECUTIONFLOW:5、判断s_bIsSignalLost是否为CSUDI_FALSE，期望为CSUDI_FALSE，即没有LOST消息上抛
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:7、遍历dwTunerId并执行步骤2~5
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;	
			s_bIsSignalLost = CSUDI_FALSE;
            
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver),"步骤3失败");

			CSTCPrint("请等待5秒\n");

			CSUDIOSThreadSleep(5000);
                  
			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING,"步骤4失败");

            CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == s_bIsSignalLost, "步骤5失败，此时不应该有EM_UDITUNER_SIGNAL_LOST消息上抛");
        }
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤4失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试参数正常时插拔信号线对锁频的影响
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:接上信号线锁频成功，拔掉锁频失败
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、在回调函数中如果判断到锁频成功则将s_nTunerTestData赋为1001，信号丢失则将s_bIsSignalLost从false改为true，SEARCHING则将s_nTunerTestData改为3003
//@EXECUTIONFLOW:4、调用CSUDITunerConnect进行锁频操作，要求返回成功
//@EXECUTIONFLOW:5、等待2秒后判断s_nTunerTestData的值，要求为1001，否则测试用例失败
//@EXECUTIONFLOW:6、提示测试人员拔掉信号线并观察是否在1秒内有信号丢失的打印，有则按'Y'继续，否则按'N'，测试用例失败
//@EXECUTIONFLOW:7、判断s_bIsSignalLost的值，要求为true，否则测试用例失败
//@EXECUTIONFLOW:8、等待500ms后判断s_nTunerTestData的值，要求为3003，并将s_bIsSignalSearching设为CSUDI_FALSE
//@EXECUTIONFLOW:9、提示插上信号线并观察是否在1秒内有锁频成功的打印，有则按'Y'继续，否则按'N'，测试用例失败
//@EXECUTIONFLOW:10、判断s_bIsSignalSearching的值，要求为CSUDI_FALSE，否则测试用例失败
//@EXECUTIONFLOW:11、遍历dwTunerId并执行步骤2~11
//@EXECUTIONFLOW:12、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_Connect_0006( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;
		
			s_bIsSignalLost = CSUDI_FALSE;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤4失败");

			CSTCPrint("请等待2秒\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"步骤5失败");

			s_nTunerTestData = 0;

			CSTCPrint("\n请拔掉信号线并仔细观察，如在1秒内有信号丢失的打印则按'Y'，否则按'N'\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"步骤6失败");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSignalLost,"步骤7失败");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING, "步骤8失败");

			CSUDIOSThreadSleep(5000);

			CSTCPrint("\n请确认在此期间有多个信号正在连接的打印\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"步骤6失败");


			CSTCPrint("\n请插上信号线并仔细观察，如在1秒内有锁频成功的打印则按'Y'，否则按'N'\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"步骤9失败");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == s_bIsSignalSearching, "步骤10，此时不应该有TUNER_SIGNAL_SEARCHING消息");
			
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤12失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试对同一频点进行两次锁频操作能否成功
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:两次锁频都返回CSUDI_SUCCESS并锁频成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、在回调函数中如果判断到锁频成功则将s_nTunerTestData赋为1001
//@EXECUTIONFLOW:4、调用CSUDITunerConnect对指定频点进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:5、调用CSUDITunerConnect同一频点进行再次锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:6、对所有Tuner都执行步骤2~5
//@EXECUTIONFLOW:7、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_Connect_0007( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤4失败");

			CSTCPrint("请等待2秒\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"步骤4失败");

			s_nTunerTestData = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤5失败");

			CSTCPrint("请等待2秒\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"步骤5失败");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤6失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试对两个不同频点进行锁频操作能否成功
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:两次锁频都返回CSUDI_SUCCESS并锁频成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、在回调函数中如果判断到锁频成功则将s_nTunerTestData赋为1001
//@EXECUTIONFLOW:4、调用CSUDITunerConnect对指定频点1进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:5、记录下第一次锁频所需要的时间，要求小于800ms
//@EXECUTIONFLOW:6、调用CSUDITunerConnect对指定频点2进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:7、记录下第二次锁频所需要的时间，要求小于800ms
//@EXECUTIONFLOW:8、对所有Tuner都执行步骤2~7
//@EXECUTIONFLOW:9、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;

			nSleepCount = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[1]),"步骤4失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&s_time1), "获取时间失败");

			while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
			{
				CSUDIOSThreadSleep(10);

				if(nSleepCount++ > 200)
				{
					break;
				}
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&s_time2), "获取时间失败");

			nMiliSecond = (s_time2.m_nSecond - s_time1.m_nSecond)*1000 + (s_time2.m_nMiliSecond - s_time1.m_nMiliSecond);

			CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 800, "步骤5失败");

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"第一次锁频失败");

			s_nTunerTestData = 0;

			nSleepCount = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[2]),"步骤6失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&s_time1), "获取时间失败");

			while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
			{
				CSUDIOSThreadSleep(10);

				if(nSleepCount++ > 200)
				{
					break;
				}
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSGetTime(&s_time2), "获取时间失败");

			nMiliSecond = (s_time2.m_nSecond - s_time1.m_nSecond)*1000 + (s_time2.m_nMiliSecond - s_time1.m_nMiliSecond);

			CSTK_ASSERT_TRUE_FATAL(nMiliSecond <= 800, "步骤7失败");

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"第二次锁频失败");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤6失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试对两个不同频点进行快速锁频操作能否成功
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:两次锁频都返回CSUDI_SUCCESS并锁频成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对指定频点1进行锁频操作，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、不等待直接调用CSUDITunerConnect对指定频点2进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:5、对所有Tuner都执行步骤2~4
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_Connect_0009( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;

	int nSleepCount = 0;
	int i;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;

			nSleepCount = 0;
	
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤4失败");

			s_nTunerTestData = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[1]),"步骤6失败");

			while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
			{
				CSUDIOSThreadSleep(10);

				if(nSleepCount++ > 200)
				{
					break;
				}
			}
		
			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"第二次锁频失败");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤6失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试对一个频点表进行依次搜索需要的时间和各个频点的平均搜索时间
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:期望平均搜索时间<500ms
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对一个频点表进行依次锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:4、记录下全部搜索时间
//@EXECUTIONFLOW:5、计算各个频点平均搜索时间并要求在500ms以内
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
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
	CSUDIOSTimeVal_S sTimeStart[FREQUENCY_NUM_MAX] = {{0}};  /*频点个数最大设为20*/
	CSUDIOSTimeVal_S sTimeEnd[FREQUENCY_NUM_MAX] = {{0}};
	BOOL bConnetcRet[FREQUENCY_NUM_MAX] = {0};
    
	int nSleepCount = 0;
	int nConnectRtnFaildCnt = 0;
	int nConnectFaildCnt = 0;
	int nConnectSuccessCnt = 0;
	int i,j;

	CSTCPrint("请连接本地实际运营商的网络,并确认频点信息无误!确认按Y,否则按N.\n");
    CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes()," 信息有误! ");
    
	CSTK_ASSERT_TRUE_FATAL(s_nDeliverCount >= 12, "本用例的测试至少需要12个频点信息，如需测试请更改配置文件");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	CSTK_ASSERT_TRUE_FATAL(s_nDeliverCount < FREQUENCY_NUM_MAX,"测试的频点个数超过了预设的最大值50");

	
	for (i = 0 ; i < nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

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
                	CSTCPrint("\n 本测试用例需锁频频点个数为【CS_TUNER_ALL_DELIVER_COUNT-1】===%d\n",s_nDeliverCount-1);
                	CSTCPrint("\n 本测试用例锁频成功的频点个数为%d\n",nConnectSuccessCnt);

			for(j=0; j<s_nDeliverCount-1; j++)
			{
				anMiliSecond[i] = (sTimeEnd[j].m_nSecond - sTimeStart[j].m_nSecond)*1000 + (sTimeEnd[j].m_nMiliSecond - sTimeStart[j].m_nMiliSecond);

				CSTCPrint("频点索引: %2d，锁频结果: %d,频点: %d, 锁频时间: %d ms\n",j,bConnetcRet[j],s_unDeliver[j].m_sCableDeliver.m_uFrequency,anMiliSecond[i]);
			}
                
			CSTK_ASSERT_TRUE_FATAL((s_nDeliverCount-1) == nConnectSuccessCnt, "锁频过程中出现失败");
                  
                	CSTCPrint("\n 平均锁频时间 ===%d********",anAverageMs[i]);
			CSTK_ASSERT_TRUE_FATAL(anAverageMs[i] < 500, "步骤5失败");
		}
	}


	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤6失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试多个任务对同一Tuner的同一频点进行反复锁频后系统能否保持稳定
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:系统依然保持稳定并能锁频成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate创建两个子任务，要求返回成功
//@EXECUTIONFLOW:4、在子任务中对同一Tuner的同一频点进行反复锁频10次，同一任务两次锁频之间等待一个10到500ms的随机时间，要求全部返回CSUDI_SUCCESS，然后自行返回
//@EXECUTIONFLOW:5、主任务在子任务返回后调用CSUDITunerConnect对同一频点进行锁频要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:6、销毁创建的任务，要求返回成功
//@EXECUTIONFLOW:7、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	if(adwTunerIds[0] != -1)
	{ 
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData), "步骤2失败");

		nAddCallbackCount ++;
        	
        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread2_1", 64, 8*1024, Tuner_TestThread2_1Entry, 
        								&adwTunerIds[0], &hThreadhandle1), "创建子任务1失败");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread2_2", 64, 8*1024, Tuner_TestThread2_2Entry, 
        								&adwTunerIds[0], &hThreadhandle2), "创建子任务2失败");

        	CSTCPrint("\n请等待10秒。。。\n");

		while(!g_bIsThreadNaturalEndFlag1 && !g_bIsThreadNaturalEndFlag2)
		{
        		CSUDIOSThreadSleep(3000);

        		if(nSleepCount++ > 100)
        		{
        			break;
        		}
        	}

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag1, "步骤4中子任务1出现锁频失败");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag2, "步骤4中子任务2出现锁频失败");

        	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "步骤5失败");

        	CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "步骤5失败");
            
        }
        CSTK_FATAL_POINT
        {
        	if(nAddCallbackCount > 0)
        	{
        		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0], Tuner_CallbackTest2, &nUserData), "删除回调失败");
        	}
        	if(hThreadhandle1 != CSUDI_NULL)
        	{
        		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除子任务1失败");
        		hThreadhandle1 = CSUDI_NULL;
        	}
        	if(hThreadhandle2 != CSUDI_NULL)
        	{
        		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除子任务2失败");
        		hThreadhandle2 = CSUDI_NULL;
        	}
        }
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试多个任务对同一Tuner的不同频点进行反复锁频后系统能否保持稳定
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:系统依然保持稳定并能锁频成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate创建两个子任务，要求返回成功
//@EXECUTIONFLOW:4、在子任务中分别对同一Tuner的不同频点进行反复锁频10次，同一任务两次锁频之间等待一个10到500ms的随机时间，要求全部返回CSUDI_SUCCESS，然后自行返回
//@EXECUTIONFLOW:5、主任务在子任务返回后调用CSUDITunerConnect对同一频点进行锁频要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:6、销毁创建的任务，要求返回成功
//@EXECUTIONFLOW:7、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	if(adwTunerIds[0] != -1)
	{ 
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData), "步骤2失败");

		nAddCallbackCount ++;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread3_1", 64, 8*1024, Tuner_TestThread2_1Entry, 
								&adwTunerIds[0], &hThreadhandle1), "创建子任务1失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread3_2", 64, 8*1024, Tuner_TestThread3Entry, 
								&adwTunerIds[0], &hThreadhandle2), "创建子任务2失败");
        
		CSTCPrint("\n请等待10秒。。。\n");

		while(!g_bIsThreadNaturalEndFlag1 && !g_bIsThreadNaturalEndFlag2)
		{
			CSUDIOSThreadSleep(3000);

			if(nSleepCount++ > 100)
			{
				break;
			}
		}
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag1, "步骤4中子任务1出现锁频失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag2, "步骤4中子任务2出现锁频失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "步骤5失败");

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "步骤5失败");
        
	}

        CSTK_FATAL_POINT
        {
           if(nAddCallbackCount > 0)
           {
        	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0], Tuner_CallbackTest2, &nUserData), "删除回调失败");
            }
            if(hThreadhandle1 != CSUDI_NULL)
            {
        	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除子任务1失败");
        	    hThreadhandle1 = CSUDI_NULL;
            }
            if(hThreadhandle2 != CSUDI_NULL)
            {
        	    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除子任务2失败");
        	    hThreadhandle2 = CSUDI_NULL;
            }
         }         
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试多个任务对不同Tuner进行锁频能否成功
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:返回CSUDI_SUCCESS并成功锁频
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDIOSThreadCreate创建两个子任务，要求返回成功
//@EXECUTIONFLOW:4、在子任务1中对Tuner0进行锁频操作，要求返回CSUDI_SUCCESS，并同过回调消息判断锁频成功
//@EXECUTIONFLOW:5、在子任务2中对Tuner1进行锁频操作，要求返回CSUDI_SUCCESS，并同过回调消息判断锁频成功
//@EXECUTIONFLOW:6、销毁创建的任务，要求返回成功
//@EXECUTIONFLOW:7、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
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
	
	CSTK_ASSERT_TRUE_FATAL(s_nActTunerCnt >= 2, "系统Tuner数至少为2个时才能执行本用例");

	g_bIsThreadNaturalEndFlag1 = CSUDI_FALSE;
	g_bIsThreadNaturalEndFlag2 = CSUDI_FALSE;
	g_bIsTunerConnected = CSUDI_FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<2 ; i++)
	{    
		if(adwTunerIds[i] != -1)
		{ 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

			nAddCallbackCount ++;
		}
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread3_1", 64, 8*1024, Tuner_TestThread4_1Entry, 
								&adwTunerIds[0], &hThreadhandle1), "创建子任务1失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("Tuner_TestThread3_2", 64, 8*1024, Tuner_TestThread4_2Entry, 
								&adwTunerIds[1], &hThreadhandle2), "创建子任务2失败");

	CSTCPrint("\n请等待15秒。。。\n");

	while(!g_bIsThreadNaturalEndFlag1 && !g_bIsThreadNaturalEndFlag2)
	{
		CSUDIOSThreadSleep(100);

		if(nSleepCount++ > 150)
		{
			break;
		}
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag1, "步骤4中子任务1出现锁频失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == g_bIsThreadNaturalEndFlag2, "步骤5中子任务2出现锁频失败");

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "删除回调失败");
		}
		if(hThreadhandle1 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除子任务1失败");
			hThreadhandle1 = CSUDI_NULL;
		}
		if(hThreadhandle2 != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除子任务2失败");
			hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:频率偏移测试
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :正确的频点并左右偏移150KHz
//@EXPECTATION:锁频成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:5、遍历dwTunerId并执行步骤2~4
CSUDI_BOOL CSTC_TUNER_IT_Connect_0014( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	CSUDITunerSRCDeliver_U unDeliver;
	int i;
	
	s_nTunerTestData = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<nActIdCnt ; i++)
	{	
		if(adwTunerIds[i] != -1)
		{ 
			memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));

			unDeliver.m_sCableDeliver.m_uFrequency = s_unDeliver[0].m_sCableDeliver.m_uFrequency - 150;
			unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8;
			unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "步骤2失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当频率向下偏移150KHz时步骤3失败");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当频率向下偏移150KHz时锁频失败");

			memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));

			unDeliver.m_sCableDeliver.m_uFrequency = s_unDeliver[0].m_sCableDeliver.m_uFrequency + 150;
			unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8;
			unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24;

			s_nTunerTestData = 0;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当频率向上偏移150KHz时步骤3失败");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当频率向上偏移150KHz时锁频失败");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤4失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:符号率偏移测试
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :正确的频点并将符号率左右偏移2~10Kbps
//@EXPECTATION:锁频成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:5、遍历dwTunerId并执行步骤2~4
CSUDI_BOOL CSTC_TUNER_IT_Connect_0015( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	CSUDITunerSRCDeliver_U unDeliver;
	int i,j;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<nActIdCnt ; i++)
	{	
		if(adwTunerIds[i] != -1)
		{ 
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "步骤2失败");

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
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向下偏移2Kbps时失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向下偏移2Kbps时锁频失败");
				}
				else if(j == 4)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向下偏移2Kbps时成功，4Kbps时失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向下偏移2Kbps时锁频成功，4Kbps时失败");
				}
				else if(j == 6)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向下偏移4Kbps时成功，6Kbps时失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向下偏移4Kbps时锁频成功，6Kbps时失败");
				}
				else if(j == 8)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向下偏移6Kbps时成功，8Kbps时失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向下偏移6Kbps时锁频成功，8Kbps时失败");
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向下偏移8Kbps时成功，10Kbps时失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向下偏移8Kbps时锁频成功，10Kbps时失败");
				}	

				memset(&unDeliver,0,sizeof(CSUDITunerSRCDeliver_U));

				unDeliver.m_sCableDeliver.m_uFrequency = s_unDeliver[0].m_sCableDeliver.m_uFrequency;
				unDeliver.m_sCableDeliver.m_uSymbolRate_24 = s_unDeliver[0].m_sCableDeliver.m_uSymbolRate_24 + j;
				unDeliver.m_sCableDeliver.m_uModulation_8 = s_unDeliver[0].m_sCableDeliver.m_uModulation_8 ;

				s_nTunerTestData = 0;
			
				if(j == 2)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向上偏移2Kbps时步骤3失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向上偏移2Kbps时锁频失败");
				}
				else if(j == 4)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向上偏移2Kbps时成功，4Kbps时失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向上偏移2Kbps时锁频成功，4Kbps时失败");
				}
				else if(j == 6)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向上偏移4Kbps时成功，6Kbps时失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向上偏移4Kbps时锁频成功，6Kbps时失败");
				}
				else if(j == 8)
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向上偏移6Kbps时成功，8Kbps时失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向上偏移6Kbps时锁频成功，8Kbps时失败");
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver), "当符号率向上偏移8Kbps时成功，10Kbps时失败");

					CSUDIOSThreadSleep(2000);

					CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "当符号率向上偏移8Kbps时锁频成功，10Kbps时失败");
				}	
			}
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤4失败");
		}
	}

	return CSUDI_TRUE; 
}

//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试播放空包码流时能否锁频成功
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号且信号中播放的是空包码流
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :正确的频点
//@EXPECTATION:锁频成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:5、遍历dwTunerId并执行步骤2~4
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
  	CSTCPrint("\n请确认在 %d 频点播放的是空包码流，按Y继续，否则按N。 \n",uFrequency);


	if(CSTKWaitYes())
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

		CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
		
		for (i=0 ; i<nActIdCnt ; i++)
		{
			if(adwTunerIds[i] != -1)
			{ 
				s_nTunerTestData = 0;
		
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");
			
				nAddCallbackCount++;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[s_nDeliverCount-2]),"步骤4失败");

				CSTCPrint("请等待5秒\n");

				CSUDIOSThreadSleep(5000);

				CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"步骤5失败");
			}
		}

	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "由于没准备好空包码流，故该用例测试失败 !!!");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤6失败");
		}
	}
	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试正常锁频流程的信号序列的正确性
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、unDeliver :无效的频点信息(频率为44400KHz)
//@INPUT:3、punDeliver :合法的频点数据地址
//@EXPECTATION:每一步都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对无效频点进行锁频操作，要求返回成功，且回调消息是EM_UDITUNER_SIGNAL_SEARCHING
//@EXECUTIONFLOW:4、调用CSUDITunerConnect对指定有效频点进行锁频操作，期望返回成功，且回调消息是EM_UDITUNER_SIGNAL_CONNECT
//@EXECUTIONFLOW:5、提示测试人员拔掉信号线,期望1秒内有信号丢失的消息上来
//@EXECUTIONFLOW:6、提示插上信号线，连接成功，回调消息是EM_UDITUNER_SIGNAL_CONNECT
//@EXECUTIONFLOW:7、调用CSUDITunerConnect另一有效频点进行再次锁频操作，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:8、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			s_nTunerTestData = 0;
	        s_bIsSignalLost = CSUDI_FALSE;

            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "加回调失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver),"无效的频点锁频不应该成功");

			CSTCPrint("请等待\n");

			for(j = 0;j < 10 ;j ++)
			{
				if(s_nTunerTestData==TUNER_SIGNAL_SEARCHING)
					break;
				CSUDIOSThreadSleep(2000);
			}

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING,"回调消息1错误");

		 	s_nTunerTestData = 0;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"锁正常频点失败");

			CSTCPrint("请等待\n");

			for(j = 0;j < 10 ;j ++)
			{
				if(s_nTunerTestData==TUNER_SIGNAL_CONNECTED)
					break;
				CSUDIOSThreadSleep(2000);
			}

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"回调消息2错误");

			s_nTunerTestData = 0;

			CSTCPrint("\n请拔掉信号线并仔细观察，如在1秒内有信号丢失的打印则按'Y'，否则按'N'\n");

        		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"步骤6失败");
			
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSignalLost,"步骤7失败");

			for(j = 0;j < 10 ;j ++)
			{
				if(s_nTunerTestData==TUNER_SIGNAL_SEARCHING)
					break;
				CSUDIOSThreadSleep(2000);
			}

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING, "回调消息3错误");

			CSTCPrint("\n请插上信号线并仔细观察，如在1秒内有锁频成功的打印则按'Y'，否则按'N'\n");
		
			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"步骤9失败");
			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"回调消息4错误");
		
			 s_nTunerTestData = 0;

		    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[1]),"步骤6失败");

		    	while(s_nTunerTestData != TUNER_SIGNAL_CONNECTED)
		    	{
				CSUDIOSThreadSleep(10);
	   
		    	}
		
			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"第二次锁频失败");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤6失败");
		}
	}

	return CSUDI_TRUE;


}


//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试调制方式QAM至少包括64，128，256三种
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、unDeliver[3]:合法的频点信息，QAM分别是64，128，256
//@EXPECTATION:每一步都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对第一个频点QAM为64进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:4、等待2秒后判断s_nTunerTestData的值，要求为TUNER_SIGNAL_CONNECTED，否则测试用例失败
//@EXECUTIONFLOW:5、调用CSUDITunerConnect对第二个频点QAM为128进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:6、等待2秒后判断s_nTunerTestData的值，要求为TUNER_SIGNAL_CONNECTED，否则测试用例失败
//@EXECUTIONFLOW:7、调用CSUDITunerConnect对第三个频点QAM为256进行锁频操作，要求返回CSUDI_SUCCESS，并通过回调消息判断锁频成功
//@EXECUTIONFLOW:8、等待2秒后判断s_nTunerTestData的值，要求为TUNER_SIGNAL_CONNECTED，否则测试用例失败
//@EXECUTIONFLOW:9、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功

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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

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

    CSTK_ASSERT_TRUE_FATAL((i < nDeliverCount), "请确认配置文件中 QAM64/QAM128/QAM256 三种调式方式的频点信息都存在!");
	for (i = 0 ; i < nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			nAddCallbackCount++;
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "加回调失败");

			for(j = 0; j < 3;j ++)
			{
				s_nTunerTestData = 0;
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &unDeliver[j]),"连接失败");

				CSTCPrint("请等待\n");

				for(t = 0;t < 10 ;t ++)
				{
					if(s_nTunerTestData==TUNER_SIGNAL_CONNECTED)
					break;
					CSUDIOSThreadSleep(2000);
				}
				CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"回调消息错误");
			}
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤6失败");
		}
	}
	return CSUDI_TRUE;

}


//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试不插信号线连接信号时，连接失败的消息不能太长时间返回
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@INPUT:dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@EXPECTATION: 测试不插信号线连接信号时，连接失败的消息返回时间小于1秒
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对第一个频点进行锁频操作，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、拔掉信号线后统计等待信号丢失消息到来的时间 ，要求小于1秒
//@EXECUTIONFLOW:5、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_Connect_0019( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;

	int i = 0;
	int j = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData), "步骤2注册回调失败");
	for (i = 0;i <5 ; i++)
	{
		s_bIsSignalLost = CSUDI_FALSE;
		j = 0;
		CSTCPrint("请确认信号线插上按'Y'!\n");
		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"CSTKWaitYes失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]),"：步骤3 连接失败\n");

        CSTCPrint("请将信号线拔出，再按'Y'确定!\n");

		CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"CSTKWaitYes失败");
		while(s_bIsSignalLost != CSUDI_TRUE)
		{
			CSUDIOSThreadSleep(10);
	   		j++;
			
			if(j*10 > 1000)
				break;
		}
		CSTCPrint("等待时间= %dms\n",j*10);
		CSTK_ASSERT_TRUE_FATAL(j*10<1000,"步骤4:消息返回时间太长!");
		CSTK_ASSERT_TRUE_FATAL(s_bIsSignalLost == CSUDI_TRUE,"拔掉信号线时锁频不应成功\n");
	
} 

    CSTCPrint("请将信号线插上，再按'Y'确定!\n");
    CSTKWaitYes();
	
	CSTK_FATAL_POINT
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0],Tuner_CallbackTest2,&nUserData),"步骤5:删除回调失败");
	}
	return CSUDI_TRUE;

}
//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试回调函数SUDITunerAddCallback在等待时，CSUDITunerConnect是否会堵塞
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@INPUT:dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@EXPECTATION:回调函数等待不会导致CSUDITunerConnect堵塞
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback，在回调函数Tuner_CallbackTest5中等待5秒，并将g_bIsTunerConnected置为CSUDI_TRUE;
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对第一个频点进行锁频操作，要求返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、判断g_bIsTunerConnected是否为CSUDI_TRUE，是则用例失败
//@EXECUTIONFLOW:5、调用CSUDITunerDelCallback删除回调
CSUDI_BOOL CSTC_TUNER_IT_Connect_0020( void )
{
	int i = 0;
	int nActIdCnt = 0;
	int nUserData = 0;
	CSUDI_UINT32 adwTunerIds[20];
	
	memset(adwTunerIds, 0, sizeof(adwTunerIds));
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1，获取设备ID失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest5, &nUserData), "步骤2，添加回调失败");	
	}

	g_bIsTunerConnected = CSUDI_FALSE;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "步骤3，锁频失败");
	}
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == g_bIsTunerConnected, "步骤4失败，CSUDITunerConnect接口不应该堵塞");

	while(!g_bIsTunerConnected)
	{
		CSUDIOSThreadSleep(100);
	}
	
	CSTK_FATAL_POINT

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest5, &nUserData), "步骤5，删除回调失败");	
	}
	
	return CSUDI_TRUE;

}


//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:测试dwTunerId非法时能否正确获得信号信息
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId = 0x1234
//@INPUT:2、u32SignalInfoMask = EM_UDITUNER_SIGNAL_QUALITY
//@EXPECTATION:获取信息失败并返回CSUDITUNER_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:1、调用CSUDITunerGetSignalInfo获取信号信息，要求返回CSUDITUNER_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0001( void )
{
	CSUDITunerSignalInfo_S s_TunerSignalInfo;

	CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_INVALID_DEVICE_ID == CSUDITunerGetSignalInfo(0x1234, EM_UDITUNER_SIGNAL_QUALITY,&s_TunerSignalInfo),"步骤1失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:测试u32SignalInfoMask非法时能否正确获得信号信息
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、u32SignalInfoMask = 0x1234
//@EXPECTATION:获取信息失败并返回CSUDITUNER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerGetSignalInfo获取信号信息，要求返回CSUDITUNER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3、对所有合法的dwTunerId执行步骤2
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerSignalInfo_S s_TunerSignalInfo;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_BAD_PARAMETER == CSUDITunerGetSignalInfo(adwTunerIds[i], 0,&s_TunerSignalInfo),"步骤2失败");
	}

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:测试保存信息的指针为空时能否正确获得信号信息
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、u32SignalInfoMask = EM_UDITUNER_SIGNAL_QUALITY
//@EXPECTATION:获取信息失败并返回CSUDITUNER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerGetSignalInfo获取信号信息，要求返回CSUDITUNER_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3、对所有合法的dwTunerId执行步骤2
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_BAD_PARAMETER == CSUDITunerGetSignalInfo(adwTunerIds[i], EM_UDITUNER_SIGNAL_QUALITY,CSUDI_NULL),"步骤2失败");
	}

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:测试正常参数时能否正确获得信号信息
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、u32SignalInfoMask = EM_UDITUNER_SIGNAL_QUALITY | EM_UDITUNER_SIGNAL_STRENGTH |EM_UDITUNER_CUR_FREQU
//@EXPECTATION:获取信息成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对指定频点进行锁频操作，要求返回成功，并在回调中将s_nTunerTestData改为1001
//@EXECUTIONFLOW:4、调用CSUDITunerGetSignalInfo获取锁定频率的信号信息，要求返回成功
//@EXECUTIONFLOW:5、判断获取的信号信息中的频率信息是否和锁定的一致，如果不一致则失败
//@EXECUTIONFLOW:6、判断获取的信号信息中信号质量和信号强度是否在0~100范围，不在则测试失败
//@EXECUTIONFLOW:7、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:8、对所有合法的dwTunerId执行步骤2~7
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0004( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nAddCallbackCount = 0;
	CSUDITunerSignalInfo_S s_TunerSignalInfo;
	int nUserData = 0x88888888;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		memset(&s_TunerSignalInfo,0,sizeof(CSUDITunerSignalInfo_S));
		s_nTunerTestData = 0;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "步骤2失败");
		
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤3失败");

		CSTCPrint("请等待2秒\n");

		CSUDIOSThreadSleep(2000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"步骤3失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i], 
								EM_UDITUNER_SIGNAL_QUALITY | EM_UDITUNER_SIGNAL_STRENGTH |EM_UDITUNER_CUR_FREQU,
								&s_TunerSignalInfo),"步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_nCurrentFreq == s_unDeliver[0].m_sCableDeliver.m_uFrequency, "步骤5失败");

		CSTK_ASSERT_TRUE_FATAL((s_TunerSignalInfo.m_nQuality >= 0) && (s_TunerSignalInfo.m_nQuality <=100),"步骤6信号质量不在范围内");

		CSTK_ASSERT_TRUE_FATAL((s_TunerSignalInfo.m_nStrength>= 0) && (s_TunerSignalInfo.m_nStrength <=100),"步骤6信号强度不在范围内");
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "步骤7失败");
		}
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:测试拔掉信号线后的误码率，以及锁频成功后能否获得信号质量、信号等级
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到了正确的频点信息
//@PRECONDITION:3、有信号
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、punDeliver :合法的频点数据地址
//@EXPECTATION:接上信号线锁频成功，拔掉锁频失败
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对有效频点进行锁频操作，要求返回成功且回调消息为EM_UDITUNER_SIGNAL_CONNECTED
//@EXECUTIONFLOW:4、提示测试人员拔掉信号线并观察是否在1秒内有信号丢失的打印，有则按'Y'继续，否则按'N'，测试用例失败
//@EXECUTIONFLOW:5、判断s_bIsSignalLost的值，要求为true，否则测试用例失败
//@EXECUTIONFLOW:6、调用CSUDITunerGetSignalInfo获取误码率，要求拔掉信号线后的为全误码，且回调消息为EM_UDITUNER_SIGNAL_SEARCHING
//@EXECUTIONFLOW:7、提示插上信号线并观察是否在1秒内有锁频成功的打印，有则按'Y'继续，否则按'N'，测试用例失败
//@EXECUTIONFLOW:8、锁频成功后调用CSUDITunerGetSignalInfo判断能否获得信号强度和信号质量
//@EXECUTIONFLOW:9、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:10、遍历dwTunerId并执行步骤2~10
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0006( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;
	CSUDITunerSignalInfo_S s_TunerSignalInfo;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");
	
	for (i=0 ; i<nActIdCnt ; i++)
	{
		if(adwTunerIds[i] != -1)
		{ 
			memset(&s_TunerSignalInfo,0,sizeof(CSUDITunerSignalInfo_S));
			s_nTunerTestData = 0;

			s_bIsSignalLost = CSUDI_FALSE;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData), "步骤2失败");

			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤3失败");

			CSTCPrint("请等待2秒\n");
			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"步骤4失败");

			s_nTunerTestData = 0;

			CSTCPrint("\n请拔掉信号线并仔细观察，如在1秒内有信号丢失的打印则按'Y'，否则按'N'，并在看到提示后才插上信号线\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"步骤5失败");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSignalLost,"步骤6失败");

			CSUDIOSThreadSleep(5000);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i],EM_UDITUNER_ERROR_RATE,
								&s_TunerSignalInfo),"步骤7失败");

			CSTCPrint("误码率 = %d\n",s_TunerSignalInfo.m_nErrorRate);
			CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_nErrorRate ==10000000,"拔信号线误码率错误!!!");

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_SEARCHING, "步骤8失败");

			CSTCPrint("\n请插上信号线并仔细观察，如在1秒内有锁频成功的打印则按'Y'，否则按'N'\n");

			CSTK_ASSERT_TRUE_FATAL(CSTKWaitYes(),"步骤9失败");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i], 
								EM_UDITUNER_SIGNAL_QUALITY | EM_UDITUNER_SIGNAL_STRENGTH |EM_UDITUNER_CUR_FREQU,
								&s_TunerSignalInfo),"步骤10失败");

			CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_nCurrentFreq == s_unDeliver[0].m_sCableDeliver.m_uFrequency, "步骤11失败");

			CSTK_ASSERT_TRUE_FATAL((s_TunerSignalInfo.m_nQuality >= 0) && (s_TunerSignalInfo.m_nQuality <=100),"步骤12信号质量不在范围内");

			CSTK_ASSERT_TRUE_FATAL((s_TunerSignalInfo.m_nStrength>= 0) && (s_TunerSignalInfo.m_nStrength <=100),"步骤13信号强度不在范围内");
			
		}
		
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,&nUserData),"步骤14失败");
		}
	}

	return CSUDI_TRUE;
	
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:测试正常参数时能否正确获得信号的频率信息
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、u32SignalInfoMask = EM_UDITUNER_CUR_FREQU
//@EXPECTATION:获取信息成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对指定频点进行锁频操作，要求返回成功，并在回调中将s_nTunerTestData改为1001
//@EXECUTIONFLOW:4、调用CSUDITunerGetSignalInfo获取锁定频点的频率信息，要求返回成功
//@EXECUTIONFLOW:5、判断获取的信号信息中的频率是否和锁定的一致，如果不一致则失败
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:7、对所有合法的dwTunerId执行步骤2~6
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0007( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nAddCallbackCount = 0;
	CSUDITunerSignalInfo_S s_TunerSignalInfo;
	int nUserData = 0x88888888;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		memset(&s_TunerSignalInfo,0,sizeof(CSUDITunerSignalInfo_S));
		s_nTunerTestData = 0;
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "步骤2失败");
		
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤3失败");

		CSUDIOSThreadSleep(2000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"回调消息错误");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i], EM_UDITUNER_CUR_FREQU,&s_TunerSignalInfo),"步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_nCurrentFreq == s_unDeliver[0].m_sCableDeliver.m_uFrequency, "步骤5失败");

	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "步骤6失败");
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerGetSignalInfo 
//@DESCRIPTION:测试正常参数时能否正确获得信号的电平等级信息
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、u32SignalInfoMask = EM_UDITUNER_SIGNAL_LEVEL
//@EXPECTATION:获取信息成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对指定频点进行锁频操作，要求返回成功，并在回调中将s_nTunerTestData改为1001
//@EXECUTIONFLOW:4、调用CSUDITunerGetSignalInfo获取锁定频点的电平等级信息，要求返回成功
//@EXECUTIONFLOW:5、判断获取的信号信息中的电平等级是否大于0，如果小于0则失败
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:7、对所有合法的dwTunerId执行步骤2~6
CSUDI_BOOL CSTC_TUNER_IT_GetSignalInfo_0008( void )
{
    int nActIdCnt = 0;
    CSUDI_UINT32 adwTunerIds[20];
    int nAddCallbackCount = 0;
    CSUDITunerSignalInfo_S s_TunerSignalInfo;
    int nUserData = 0x88888888;
    int i;

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

    CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

    for(i=0 ; i<nActIdCnt ; i++)
    {
        memset(&s_TunerSignalInfo,0,sizeof(CSUDITunerSignalInfo_S));
        s_nTunerTestData = 0;
    
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "步骤2失败");
        
        nAddCallbackCount++;

        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"步骤3失败");

        CSUDIOSThreadSleep(2000);

        CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"回调消息错误");
        
        CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetSignalInfo(adwTunerIds[i], EM_UDITUNER_SIGNAL_LEVEL, &s_TunerSignalInfo), "步骤4失败");

        CSTCPrint("s_TunerSignalInfo.m_dSignalLevel=%lf\n", s_TunerSignalInfo.m_dSignalLevel);
        
        CSTK_ASSERT_TRUE_FATAL(s_TunerSignalInfo.m_dSignalLevel > 0, "步骤5失败");
    }

    CSTK_FATAL_POINT
    {
        for(i=0 ; i<nAddCallbackCount ; i++)
        {
            CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "步骤6失败");
        }
    }
    
    return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddPID 
//@DESCRIPTION:测试dwTunerId非法时能否成功添加过滤PID
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId = 0x1234	
//@INPUT:2、nPID = 0x0101
//@EXPECTATION:添加失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDITunerAddPID添加过滤PID，要求返回失败
CSUDI_BOOL CSTC_TUNER_IT_AddPID_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerAddPID(0x1234, 0x0101), "步骤1失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddPID 
//@DESCRIPTION:测试nPID非法时能否成功添加过滤PID
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、nPID = 0x1234
//@EXPECTATION:添加失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerGetDeviceInfo获取当前所有设备的能力信息，并返回成功，否则测试用例失败
//@EXECUTIONFLOW:3、调用CSUDITunerAddPID添加过滤PID，如果该Tuner的m_bPIDFilterSupport为CSUDI_TRUE则返回错误代码(非CSUDI_SUCCESS)，为CSUDI_FALSE则返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:4、对所有合法的dwTunerId执行步骤2~3
CSUDI_BOOL CSTC_TUNER_IT_AddPID_0002( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerInfo_S s_TunerInfo;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(adwTunerIds[i], &s_TunerInfo), "步骤2失败");

		if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_TRUE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerAddPID(adwTunerIds[i], 0x1234), "步骤3失败");
		}
		else if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_FALSE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerAddPID(adwTunerIds[i], 0x1234), "步骤3失败");
		}
	}

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddPID 
//@DESCRIPTION:测试合法参数能否成功添加过滤PID
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、nPID = 0
//@EXPECTATION:添加成功
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerGetDeviceInfo获取当前所有设备的能力信息，并返回成功，否则测试用例失败
//@EXECUTIONFLOW:3、如果该Tuner的m_bPIDFilterSupport为CSUDI_TRUE则执行步骤以下步骤，否则直接调用CSUDITunerAddPID添加过滤PID，要求返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED并直接跳至步骤10
//@EXECUTIONFLOW:4、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:5、在回调函数中如果判断到锁频成功则将s_nTunerTestData赋为TUNER_SIGNAL_CONNECTED
//@EXECUTIONFLOW:6、调用CSUDITunerConnect进行锁频操作，要求返回成功
//@EXECUTIONFLOW:7、等待2秒后判断s_nTunerTestData的值，要求为TUNER_SIGNAL_CONNECTED，否则测试用例失败
//@EXECUTIONFLOW:8、调用CSUDITunerAddPID添加过滤PID，要求返回成功
//@EXECUTIONFLOW:9、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:10、对所有合法的dwTunerId执行步骤2~10
CSUDI_BOOL CSTC_TUNER_IT_AddPID_0003( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerInfo_S s_TunerInfo;
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(adwTunerIds[i], &s_TunerInfo), "步骤2失败");

		if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_TRUE)
		{
			s_nTunerTestData = 0;
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest2, &nUserData), "步骤4失败");
			
			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]), "步骤6失败");

			CSTCPrint("请等待2秒\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"步骤7失败");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddPID(adwTunerIds[i], 0), "步骤8失败");
		}
		else if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_FALSE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerAddPID(adwTunerIds[i], 0), "不支持应该返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest1, &nUserData), "步骤9失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRemovePID 
//@DESCRIPTION:测试dwTunerId非法时能否成功删除过滤PID
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId = 0x1234	
//@INPUT:2、nPID = 0x0101
//@EXPECTATION:删除失败并返回错误代码值(非CSUDI_SUCCESS)
//@EXECUTIONFLOW:1、调用CSUDITunerRemovePID删除过滤PID，要求返回失败
CSUDI_BOOL CSTC_TUNER_IT_RemovePID_0001( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRemovePID(0x1234, 0x0101), "步骤1失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRemovePID 
//@DESCRIPTION:测试合法参数能否成功添加删除过滤PID
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、nPID = 0
//@EXPECTATION:删除成功并返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerGetDeviceInfo获取当前所有设备的能力信息，并返回成功，否则测试用例失败
//@EXECUTIONFLOW:3、如果该Tuner的m_bPIDFilterSupport为CSUDI_TRUE则执行步骤以下步骤，否则直接调用CSUDITunerRemovePID删除过滤PID，要求返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED并直接跳至步骤11
//@EXECUTIONFLOW:4、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:5、在回调函数中如果判断到锁频成功则将s_nTunerTestData赋为TUNER_SIGNAL_CONNECTED
//@EXECUTIONFLOW:6、调用CSUDITunerConnect进行锁频操作，要求返回成功
//@EXECUTIONFLOW:7、等待2秒后判断s_nTunerTestData的值，要求为TUNER_SIGNAL_CONNECTED，否则测试用例失败
//@EXECUTIONFLOW:8、调用CSUDITunerAddPID添加过滤PID，要求返回成功
//@EXECUTIONFLOW:9、调用CSUDITunerRemovePID删除添加的过滤PID，要求返回成功
//@EXECUTIONFLOW:10、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
//@EXECUTIONFLOW:11、对所有合法的dwTunerId执行步骤2~10
CSUDI_BOOL CSTC_TUNER_IT_AddRemovePID_0001( void )
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	CSUDITunerInfo_S s_TunerInfo;
	int nUserData = 0x88888888;
	int nAddCallbackCount = 0;
	int i;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds,sizeof(adwTunerIds)/sizeof(adwTunerIds[0]),&nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0,"Tuner数不能为0");

	for(i=0 ; i<nActIdCnt ; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(adwTunerIds[i], &s_TunerInfo), "步骤2失败");

		if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_TRUE)
		{
			s_nTunerTestData = 0;
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i], Tuner_CallbackTest1, &nUserData), "步骤4失败");
			
			nAddCallbackCount++;

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]), "步骤6失败");

			CSTCPrint("请等待2秒\n");

			CSUDIOSThreadSleep(2000);

			CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED,"步骤7失败");
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddPID(adwTunerIds[i], 0), "步骤8失败");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRemovePID(adwTunerIds[i], 0), "步骤9失败");
		}
		else if(s_TunerInfo.m_bPIDFilterSupport == CSUDI_FALSE)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerRemovePID(adwTunerIds[i], 0), "不支持应该返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED");
		}
	}

	CSTK_FATAL_POINT
	{
		for(i=0 ; i<nAddCallbackCount ; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i], Tuner_CallbackTest1, &nUserData), "步骤10失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@DESCRIPTION:测试正常参数能否成功添加设备通知回调函数
//@PRECONDITION:1、设备初始化成功
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到要接入的移动Tuner的信息
//@INPUT:1、fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2、UserData = 0x88888888
//@EXPECTATION:注册成功
//@EXECUTIONFLOW:1、调用CSUDITunerRDIAddCallback注册回调，要求返回成功
//@EXECUTIONFLOW:2、等待插入可插拔Tuner
//@EXECUTIONFLOW:3、在回调函数中获取插入的设备ID，并赋给s_dwRDITunerId然后判断设备ID的高16位是否为3，是则成功，否则失败
//@EXECUTIONFLOW:4、调用CSUDITunerGetDeviceInfo获取设备信息，要求返回成功，否则测试用例失败
//@EXECUTIONFLOW:5、直接拔掉可移动Tuner
//@EXECUTIONFLOW:6、进行拔出操作后判断回调消息要求为EM_UDIRDI_EVENT_PLUGOUT，而且EventData的值为EM_UDIRDI_PLUGOUT_UNSAFE，否则返回测试失败
//@EXECUTIONFLOW:7、调用CSUDITunerRDIRemoveCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_RDIAddCallback_0001( void )
{
	int nUserData = 0x88888888;
	CSUDITunerInfo_S sTunerInfo;
	int nSleepCount = 0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback( Tuner_RDICallbackTest1, &nUserData), "步骤1失败");

	CSTCPrint("\n请插上可移动Tuner并按'Y继续。。。。。。'\n");

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
		CSTK_ASSERT_TRUE_FATAL((CSUDI_UINT16)(s_dwRDITunerId >> 16) == 3,"步骤3失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(s_dwRDITunerId, &sTunerInfo),"步骤4失败");
	}

	CSTCPrint("\n请拔掉插上的可移动Tuner并按'Y'继续。。。。。。\n");

	if(CSTKWaitYes())
	{
		CSUDIOSThreadSleep(2000);
		CSTK_ASSERT_TRUE_FATAL(EM_UDIRDI_PLUGOUT_UNSAFE == s_nRDIEventData, "步骤6失败");
	}

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback( Tuner_RDICallbackTest1, &nUserData), "步骤7失败");
		}
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@DESCRIPTION:测试fnCallback非法时能否成功添加设备通知回调函数
//@PRECONDITION:设备初始化成功
//@INPUT:1、fnTunerCallback = CSUDI_NULL
//@INPUT:2、UserData = 0x88888888
//@EXPECTATION:注册失败
//@EXECUTIONFLOW:1、调用CSUDITunerRDIAddCallback注册回调，要求返回失败
//@EXECUTIONFLOW:2、如果步骤1返回成功则调用CSUDITunerRDIRemoveCallback删除注册的回调函数，要求返回成功，步骤1返回失败则不执行该步骤
CSUDI_BOOL CSTC_TUNER_IT_RDIAddCallback_0002( void )
{
	int nUserData = 0x88888888;
	CSUDI_BOOL bHasJump = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRDIAddCallback( CSUDI_NULL, &nUserData), "步骤1失败");

	bHasJump = CSUDI_FALSE;
	
	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			if(bHasJump)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(CSUDI_NULL, &nUserData),"步骤2失败");
			}
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@DESCRIPTION:测试UserData为空时能否成功添加设备通知回调函数
//@PRECONDITION:设备初始化成功
//@INPUT:1、fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2、UserData = CSUDI_NULL
//@EXPECTATION:注册成功
//@EXECUTIONFLOW:1、调用CSUDITunerRDIAddCallback注册回调，要求返回成功
//@EXECUTIONFLOW:2、调用CSUDITunerRDIRemoveCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_RDIAddCallback_0003( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback( Tuner_RDICallbackTest1, CSUDI_NULL), "步骤1失败");
	
	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, CSUDI_NULL),"步骤2失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@DESCRIPTION:测试fnCallback非法时能否成功删除设备通知回调函数
//@PRECONDITION:设备初始化成功
//@INPUT:1、fnTunerCallback = CSUDI_NULL
//@INPUT:2、UserData = 0x88888888
//@EXPECTATION:删除失败
//@EXECUTIONFLOW:1、调用CSUDITunerRDIRemoveCallback删除回调函数，要求返回失败
CSUDI_BOOL CSTC_TUNER_IT_RDIRemoveCallback_0001( void )
{
	int nUserData = 0x88888888;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRDIRemoveCallback(CSUDI_NULL, &nUserData), "步骤1失败");	
	
	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@DESCRIPTION:测试UserData为空时能否成功删除设备通知回调函数
//@PRECONDITION:设备初始化成功
//@INPUT:1、fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2、UserData = CSUDI_NULL
//@EXPECTATION:删除成功
//@EXECUTIONFLOW:1、调用CSUDITunerRDIAddCallback注册回调，要求返回成功
//@EXECUTIONFLOW:2、调用CSUDITunerRDIRemoveCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_RDIRemoveCallback_0002( void )
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback( Tuner_RDICallbackTest1, CSUDI_NULL), "步骤1失败");
	
	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, CSUDI_NULL),"步骤2失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@DESCRIPTION:测试能否成功删除没有注册的设备通知回调函数
//@PRECONDITION:设备初始化成功
//@INPUT:1、fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2、UserData = 0x88888888
//@EXPECTATION:删除失败
//@EXECUTIONFLOW:1、调用CSUDITunerRDIRemoveCallback删除没有注册的回调函数，要求返回失败
CSUDI_BOOL CSTC_TUNER_IT_RDIRemoveCallback_0003( void )
{
	int nUserData = 0x88888888;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData), "步骤1失败");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@DESCRIPTION:测试当UserData与注册的不一致时能否成功删除设备通知回调函数
//@PRECONDITION:设备初始化成功
//@INPUT:1、fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2、UserData = 0x88888888
//@EXPECTATION:删除失败
//@EXECUTIONFLOW:1、调用CSUDITunerRDIAddCallback注册回调，UserData为0要求返回成功
//@EXECUTIONFLOW:2、调用CSUDITunerRDIRemoveCallback删除UserData与注册的回调不一致的回调函数，要求返回失败
//@EXECUTIONFLOW:2、调用CSUDITunerRDIRemoveCallback删除步骤1注册的回调函数，要求返回失败
CSUDI_BOOL CSTC_TUNER_IT_RDIRemoveCallback_0004( void )
{
	int nUserData1 = 0;
	int nUserData2 = 0x88888888;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback( Tuner_RDICallbackTest1, &nUserData1), "步骤1失败");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData2),"步骤2失败");

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData1), "步骤3失败");
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@CASEGROUP:CSUDITunerRDIRemoveDev 
//@DESCRIPTION:测试正常插拔可移动Tuner的情况
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、fnTunerCallback = Tuner_RDICallbackTest1
//@INPUT:2、UserData = 0x88888888
//@EXPECTATION:每一步都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDITunerRDIAddCallback注册回调，要求返回成功
//@EXECUTIONFLOW:2、等待插入可插拔Tuner
//@EXECUTIONFLOW:3、在回调函数中获取插入的设备ID，并赋给s_dwRDITunerId然后判断设备ID的高16位是否为3，是则成功，否则失败
//@EXECUTIONFLOW:4、调用CSUDITunerGetDeviceInfo获取设备信息，要求返回成功，否则测试用例失败
//@EXECUTIONFLOW:5、调用CSUDITunerRDIRemoveDev安全移除可移动Tuner，要求返回成功
//@EXECUTIONFLOW:6、进行安全移除操作后判断回调消息要求为EM_UDIRDI_EVENT_PLUGOUT，而且EventData的值为EM_UDIRDI_PLUGOUT_SAFE，否则返回测试失败
//@EXECUTIONFLOW:7、调用CSUDITunerRDIRemoveCallback删除注册的回调函数，要求返回失败
CSUDI_BOOL CSTC_TUNER_IT_RDI_0001( void )
{
	int nUserData = 0x88888888;
	CSUDITunerInfo_S sTunerInfo;
	int nSleepCount = 0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback(Tuner_RDICallbackTest1, &nUserData), "步骤1失败");

	CSTCPrint("\n请插上可移动Tuner并按'Y继续。。。。。。'\n");

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
		CSTK_ASSERT_TRUE_FATAL((CSUDI_UINT16)(s_dwRDITunerId >> 16) == 3,"步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(s_dwRDITunerId, &sTunerInfo),"步骤4失败");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveDev(s_dwRDITunerId),"步骤5失败");

		CSUDIOSThreadSleep(2000);
		
		CSTK_ASSERT_TRUE_FATAL(EM_UDIRDI_PLUGOUT_SAFE == s_nRDIEventData, "步骤6失败");
	}

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData), "步骤7失败");
		}
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerRDIAddCallback 
//@CASEGROUP:CSUDITunerRDIRemoveCallback 
//@CASEGROUP:CSUDITunerRDIRemoveDev 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerIORead
//@DESCRIPTION:测试可移动Tuner插入、锁频、读取数据以及移除等操作
//@PRECONDITION:1、系统初始化成功且信号良好
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到要接入的移动Tuner的信息以及要锁频的频点信息
//@INPUT:在步骤中描述
//@EXPECTATION:每一步都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDITunerRDIAddCallback注册回调，要求返回成功
//@EXECUTIONFLOW:2、等待插入可插拔Tuner
//@EXECUTIONFLOW:3、在回调函数中获取插入的设备ID，并赋给s_dwRDITunerId然后判断设备ID的高16位是否为3，是则成功，否则失败
//@EXECUTIONFLOW:4、调用CSUDITunerGetDeviceInfo获取设备信息，要求返回成功，否则测试用例失败
//@EXECUTIONFLOW:5、在回调函数中获取移动Tuner的ID，并调用CSUDITunerAddCallback为该设备注册锁频信息回调函数，要求返回成功
//@EXECUTIONFLOW:6、调用CSUDITunerConnect使用该移动设备对指定频点进行锁频，要求返回成功，否则测试用例失败
//@EXECUTIONFLOW:7、等待2秒后判断锁频信息回调函数获取的回调信息，要求为EM_UDITUNER_SIGNAL_CONNECTED并将s_nTunerTestData改为1001，否则测试用例失败
//@EXECUTIONFLOW:8、如果该Tuner支持硬过滤PID则调用CSUDITunerAddPID添加Tuner硬过滤PID，要求返回成功，否则返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9、调用CSUDITunerIORead读取TS流数据，要求返回成功
//@EXECUTIONFLOW:10、调用CSUDITunerRDIRemoveDev安全移除可移动Tuner，要求返回成功
//@EXECUTIONFLOW:11、调用CSUDITunerRDIRemoveCallback删除设备通知回调函数，要求返回成功
//@EXECUTIONFLOW:12、调用CSUDITunerDelCallback删除锁频信息回调函数，要求返回成功
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback(Tuner_RDICallbackTest1, &nUserData1), "步骤1失败");

	CSTCPrint("\n请插上可移动Tuner并按'Y继续。。。。。。'\n");

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
		CSTK_ASSERT_TRUE_FATAL((CSUDI_UINT16)(s_dwRDITunerId >> 16) == 3,"步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(s_dwRDITunerId, &sTunerInfo),"步骤4失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(s_dwRDITunerId, Tuner_CallbackTest2, &nUserData2),"步骤5失败");

		bHasJump = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(s_dwRDITunerId, &s_unDeliver[0]),"步骤6失败");

		CSUDIOSThreadSleep(2000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "步骤7失败");

		if(sTunerInfo.m_bPIDFilterSupport)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddPID(s_dwRDITunerId, 0),"步骤8失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerAddPID(s_dwRDITunerId, 0),"步骤8失败");
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerIORead(s_dwRDITunerId, acData, sizeof(acData)/sizeof(acData[0]), &nReadLength),"步骤9失败");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveDev(s_dwRDITunerId),"步骤10失败");

	}

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData1), "步骤11失败");

			if(!bHasJump)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(s_dwRDITunerId, Tuner_CallbackTest2, &nUserData2), "步骤12失败");
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
//@DESCRIPTION:可移动Tuner的插拔强度测试
//@PRECONDITION:1、系统初始化成功且信号良好
//@PRECONDITION:2、配置文件正确并已经从配置文件中得到要接入的移动Tuner的信息以及要锁频的频点信息
//@INPUT:在步骤中描述
//@EXPECTATION:每一步都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDITunerRDIAddCallback注册回调，要求返回成功
//@EXECUTIONFLOW:2、将可移动Tuner插拔ADD_REMOVE_COUNT次后插上可移动Tuner
//@EXECUTIONFLOW:3、在回调函数中获取插入的设备ID，并赋给s_dwRDITunerId然后判断设备ID的高16位是否为3，是则成功，否则失败
//@EXECUTIONFLOW:4、调用CSUDITunerGetDeviceInfo获取设备信息，要求返回成功，否则测试用例失败
//@EXECUTIONFLOW:5、在回调函数中获取移动Tuner的ID，并调用CSUDITunerAddCallback为该设备注册锁频信息回调函数，要求返回成功
//@EXECUTIONFLOW:6、调用CSUDITunerConnect使用该移动设备对指定频点进行锁频，要求返回成功，否则测试用例失败
//@EXECUTIONFLOW:7、等待2秒后判断锁频信息回调函数获取的回调信息，要求为EM_UDITUNER_SIGNAL_CONNECTED，否则测试用例失败
//@EXECUTIONFLOW:8、如果该Tuner支持硬过滤PID则调用CSUDITunerAddPID添加Tuner硬过滤PID，要求返回成功，否则返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:9、调用CSUDITunerIORead读取TS流数据，要求返回成功
//@EXECUTIONFLOW:10、调用CSUDITunerRDIRemoveDev安全移除可移动Tuner，要求返回成功
//@EXECUTIONFLOW:11、调用CSUDITunerRDIRemoveCallback删除设备通知回调函数，要求返回成功
//@EXECUTIONFLOW:12、调用CSUDITunerDelCallback删除锁频信息回调函数，要求返回成功
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == s_bIsSupportRDITuner, "不支持可移动Tuner的测试");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIAddCallback(Tuner_RDICallbackTest1, &nUserData1), "步骤1失败");

	CSTCPrint("\n请插拔可移动Tuner %d 次后再插上可移动Tuner并按'Y继续。。。。。。'\n", nAddRemoveCont);

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
		CSTK_ASSERT_TRUE_FATAL((CSUDI_UINT16)(s_dwRDITunerId >> 16) == 3,"步骤3失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetDeviceInfo(s_dwRDITunerId, &sTunerInfo),"步骤4失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(s_dwRDITunerId, Tuner_CallbackTest2, &nUserData2),"步骤5失败");

		bHasJump = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(s_dwRDITunerId, &s_unDeliver[0]),"步骤6失败");

		CSUDIOSThreadSleep(2000);

		CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == TUNER_SIGNAL_CONNECTED, "步骤7失败");

		if(sTunerInfo.m_bPIDFilterSupport)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddPID(s_dwRDITunerId, 0),"步骤8失败");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED == CSUDITunerAddPID(s_dwRDITunerId, 0),"步骤8失败");
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerIORead(s_dwRDITunerId, acData, sizeof(acData)/sizeof(acData[0]), &nReadLength),"步骤9失败");
	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveDev(s_dwRDITunerId),"步骤10失败");

	}

	CSTK_FATAL_POINT
	{
		if(s_bIsSupportRDITuner)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerRDIRemoveCallback(Tuner_RDICallbackTest1, &nUserData1), "步骤11失败");

			if(!bHasJump)
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(s_dwRDITunerId, Tuner_CallbackTest2, &nUserData2), "步骤12失败");
			}
		}
	}
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerDelCallback 
//@CASEGROUP:CSUDITunerGetSignalInfo 
//@CASEGROUP:CSUDITunerConnect 
//@DESCRIPTION:测试锁频时传递的频点参数与回调时返回的频点参数一致
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、fnTunerCallback = Tuner_CallbackTest3
//@INPUT:3、u32SignalInfoMask = EM_UDITUNER_CUR_FREQU|EM_UDITUNER_SIGNAL_QUALITY|EM_UDITUNER_SIGNAL_STRENGTH
//@EXPECTATION:每一步都输出预期结果
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback注册回调函数，要求返回成功
//@EXECUTIONFLOW:3、调用CSUDITunerConnect对指定频点进行锁频操作，要求返回成功
//@EXECUTIONFLOW:4、判断是否有收到回调消息，要求能够收到
//@EXECUTIONFLOW:5、判断锁频时传递的频点参数是否与回调时返回的频点参数一致，如果不一致则失败
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback删除注册的回调函数，要求返回成功
CSUDI_BOOL CSTC_TUNER_IT_UnifiedFreqTrans_0001(void)
{
	int nActIdCnt = 0;
	CSUDI_UINT32 adwTunerIds[20];
	int nUserData = 0x88888888;
	int i = 0;
	
	s_nTunerTestData = 0;
    
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[0], Tuner_CallbackTest3, &nUserData), "注册回调函数失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[0], &s_unDeliver[0]), "锁频操作失败");

 	for(i = 0; i < 10; i ++)
	{
		if(s_nTunerTestData != 0)
        {
			break;
        }
        CSUDIOSThreadSleep(1000);
	}
	
	CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData == 0x1234, "未收到任何回调消息");

	CSTK_ASSERT_TRUE_FATAL(s_nTunerTestData != 0x5678, "当前正在锁定的信号频点参数与锁频时传递的频点参数不一致");

	CSTK_FATAL_POINT;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[0], Tuner_CallbackTest3, &nUserData), "删除回调失败");

	return CSUDI_TRUE;
}


/***************************************************************************/
/*以下测试用例主要用来测试设置的频点的锁频时间，相关频点信息从配置文件中获得*/
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
					CSTCPrint("\r\n[%s]第%d个频点:The Connnect Time is:%d(ms), too long\r\n",__FUNCTION__,i,nTotalTime);
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
						CSTCPrint("\r\n[%s]第%d个频点:Signal Lost Time is:%d(ms), too long\r\n",__FUNCTION__,i,nTotalTime);
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
//@DESCRIPTION:测试有线QAM64的一个频点的锁频时间，要求:锁频成功的时间小于200ms，返回成功。若锁频不成功，要求检测到失锁状态的时间小于3000ms。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、nFreqCnt 要测试的频点的个数
//@INPUT:2、aeQamType 用于测试的调制方式
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSTC_TUNER_IT_QamLockInvail 先锁定一个无效的频点，要求返回值CSUDI_TRUE，否则失败。
//@EXECUTIONFLOW:3、调用CSTC_TUNER_IT_QamFindFreq 找到测试频点，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerAddCallback 添加回调Tuner_CallbackTest2，要求返回CSUDI_SUCCESS，否则失败。
//@EXECUTIONFLOW:5、调用CSTC_TUNER_IT_QamConnect 锁频，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback 注销回调，要求返回CSUDI_SUCCESS，否则失败。
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0001(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 1;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM64;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "设置锁无效频点失败");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"没有找到需要的频点，请重新设置配置文件");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "添加回调函数失败");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"锁频失败");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"删除回调失败");
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
//@DESCRIPTION:测试有线QAM128的一个频点的锁频时间，要求:锁频成功的时间小于200ms，返回成功。若锁频不成功，要求检测到失锁状态的时间小于3000ms。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、nFreqCnt 要测试的频点的个数
//@INPUT:2、aeQamType 用于测试的调制方式
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSTC_TUNER_IT_QamLockInvail 先锁定一个无效的频点，要求返回值CSUDI_TRUE，否则失败。
//@EXECUTIONFLOW:3、调用CSTC_TUNER_IT_QamFindFreq 找到测试频点，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerAddCallback 添加回调Tuner_CallbackTest2，要求返回CSUDI_SUCCESS，否则失败。
//@EXECUTIONFLOW:5、调用CSTC_TUNER_IT_QamConnect 锁频，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback 注销回调，要求返回CSUDI_SUCCESS，否则失败。
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0002(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 1;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM128;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "设置锁无效频点失败");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"没有找到需要的频点，请重新设置配置文件");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "添加回调函数失败");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"锁频失败");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"删除回调失败");
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
//@DESCRIPTION:测试有线QAM256的一个频点的锁频时间，要求:锁频成功的时间小于200ms，返回成功。若锁频不成功，要求检测到失锁状态的时间小于3000ms。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、nFreqCnt 要测试的频点的个数
//@INPUT:2、aeQamType 用于测试的调制方式
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSTC_TUNER_IT_QamLockInvail 先锁定一个无效的频点，要求返回值CSUDI_TRUE，否则失败。
//@EXECUTIONFLOW:3、调用CSTC_TUNER_IT_QamFindFreq 找到测试频点，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerAddCallback 添加回调Tuner_CallbackTest2，要求返回CSUDI_SUCCESS，否则失败。
//@EXECUTIONFLOW:5、调用CSTC_TUNER_IT_QamConnect 锁频，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback 注销回调，要求返回CSUDI_SUCCESS，否则失败。
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0003(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 1;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM256;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "设置锁无效频点失败");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"没有找到需要的频点，请重新设置配置文件");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "添加回调函数失败");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"锁频失败");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"删除回调失败");
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
//@DESCRIPTION:再锁定一个QAM64频点的情况下，去锁定另外一个QAM64的频点，要求锁频成功，并且时间小于200ms。否则失败。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、nFreqCnt 要测试的频点的个数
//@INPUT:2、aeQamType 用于测试的调制方式
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSTC_TUNER_IT_QamLockInvail 先锁定一个无效的频点，要求返回值CSUDI_TRUE，否则失败。
//@EXECUTIONFLOW:3、调用CSTC_TUNER_IT_QamFindFreq 找到测试频点，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerAddCallback 添加回调Tuner_CallbackTest2，要求返回CSUDI_SUCCESS，否则失败。
//@EXECUTIONFLOW:5、调用CSTC_TUNER_IT_QamConnect 锁频，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback 注销回调，要求返回CSUDI_SUCCESS，否则失败。
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0004(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 2;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM64;
	aeQAMType[1] = EM_UDITUNER_QAM64;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "设置锁无效频点失败");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"没有找到需要的频点，请重新设置配置文件");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "添加回调函数失败");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"锁频失败");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"删除回调失败");
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
//@DESCRIPTION:再锁定一个QAM64频点的情况下，去锁定另外一个QAM128的频点，要求锁频成功，并且时间小于200ms。否则失败。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、nFreqCnt 要测试的频点的个数
//@INPUT:2、aeQamType 用于测试的调制方式
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSTC_TUNER_IT_QamLockInvail 先锁定一个无效的频点，要求返回值CSUDI_TRUE，否则失败。
//@EXECUTIONFLOW:3、调用CSTC_TUNER_IT_QamFindFreq 找到测试频点，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerAddCallback 添加回调Tuner_CallbackTest2，要求返回CSUDI_SUCCESS，否则失败。
//@EXECUTIONFLOW:5、调用CSTC_TUNER_IT_QamConnect 锁频，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback 注销回调，要求返回CSUDI_SUCCESS，否则失败。
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0005(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 2;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM64;
	aeQAMType[1] = EM_UDITUNER_QAM128;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "设置锁无效频点失败");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"没有找到需要的频点，请重新设置配置文件");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "添加回调函数失败");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"锁频失败");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"删除回调失败");
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
//@DESCRIPTION:再锁定一个QAM128频点的情况下，去锁定另外一个QAM64的频点，要求锁频成功，并且时间小于200ms。否则失败。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、nFreqCnt 要测试的频点的个数
//@INPUT:2、aeQamType 用于测试的调制方式
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSTC_TUNER_IT_QamLockInvail 先锁定一个无效的频点，要求返回值CSUDI_TRUE，否则失败。
//@EXECUTIONFLOW:3、调用CSTC_TUNER_IT_QamFindFreq 找到测试频点，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerAddCallback 添加回调Tuner_CallbackTest2，要求返回CSUDI_SUCCESS，否则失败。
//@EXECUTIONFLOW:5、调用CSTC_TUNER_IT_QamConnect 锁频，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback 注销回调，要求返回CSUDI_SUCCESS，否则失败。
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0006(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 2;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM128;
	aeQAMType[1] = EM_UDITUNER_QAM64;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "设置锁无效频点失败");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"没有找到需要的频点，请重新设置配置文件");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "添加回调函数失败");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"锁频失败");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"删除回调失败");
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
//@DESCRIPTION:再锁定一个QAM256频点的情况下，去锁定另外一个QAM64的频点，要求锁频成功，并且时间小于200ms。否则失败。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、nFreqCnt 要测试的频点的个数
//@INPUT:2、aeQamType 用于测试的调制方式
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSTC_TUNER_IT_QamLockInvail 先锁定一个无效的频点，要求返回值CSUDI_TRUE，否则失败。
//@EXECUTIONFLOW:3、调用CSTC_TUNER_IT_QamFindFreq 找到测试频点，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerAddCallback 添加回调Tuner_CallbackTest2，要求返回CSUDI_SUCCESS，否则失败。
//@EXECUTIONFLOW:5、调用CSTC_TUNER_IT_QamConnect 锁频，要求返回CSUDI_TRUE，否则返回失败。
//@EXECUTIONFLOW:6、调用CSUDITunerDelCallback 注销回调，要求返回CSUDI_SUCCESS，否则失败。
CSUDI_BOOL CSTC_TUNER_IT_QamConnectTime_0007(void)
{
	int nActIdCnt = 0;
	int i = 0;
	int nFreqCnt = 2;
	int nAddCallbackCount = 0;
	CSUDI_UINT32 adwTunerIds[20] = {0};
	CSUDITunerQAMMode_E aeQAMType[FREQUENCY_NUM_MAX] = {EM_UDITUNER_QAM64};
	CSUDITunerSRCDeliver_U aunTunerDeliver[FREQUENCY_NUM_MAX];

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");
	
	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");

	memset(aunTunerDeliver,0,sizeof(aunTunerDeliver));

	aeQAMType[0] = EM_UDITUNER_QAM128;
	aeQAMType[1] = EM_UDITUNER_QAM64;

	for(i = 0; i < nActIdCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamLockInvail(adwTunerIds[i]), "设置锁无效频点失败");
			
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamFindFreq(nFreqCnt,aeQAMType,aunTunerDeliver),"没有找到需要的频点，请重新设置配置文件");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]), "添加回调函数失败");
		nAddCallbackCount++;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTC_TUNER_IT_QamConnect(adwTunerIds[i],nFreqCnt,aunTunerDeliver),"锁频失败");
	}

	CSTK_FATAL_POINT
	{
		for(i = 0; i < nAddCallbackCount; i++)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,(void *)&aunTunerDeliver[nFreqCnt - 1]),"删除回调失败");
		}
	}

	return CSUDI_TRUE;
}


#if 0
/***************************************************************************/
/*以下测试用例主要是用来测试QPSK的锁频时间                                 */
/*所有的测试用例都是接码流播放器                                           */
/***************************************************************************/

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:在QPSK初始化成功的前提下，锁定freq为950000Khz,symbol为4000kbs的频点，要求能够锁频成功，并且锁频时间小于600ms。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、s_unDeliver[0].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency 需要锁频的频点的freq.
//@INPUT:3、s_unDeliver[0].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 需要锁频的频点的symbol.
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback 来添加回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
//@EXECUTIONFLOW:3、调用CSUDITunerConnect来锁设置的频点，要求返回CSUDI_SUCCESS，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除添加的回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "添加回调函数失败");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[0]),"锁频失败");

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
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"删除回调失败");
		}
	}

	return eRet;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:在QPSK初始化成功的前提下，锁定freq为950000Khz,symbol为45000kbs的频点，要求能够锁频成功，并且锁频时间小于600ms。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、s_unDeliver[1].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency 需要锁频的频点的freq.
//@INPUT:3、s_unDeliver[1].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 需要锁频的频点的symbol.
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback 来添加回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
//@EXECUTIONFLOW:3、调用CSUDITunerConnect来锁设置的频点，要求返回CSUDI_SUCCESS，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除添加的回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "添加回调函数失败");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[1]),"锁频失败");

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
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"删除回调失败");
		}
	}

	return eRet;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:在QPSK初始化成功的前提下，锁定freq为1370000Khz,symbol为20000kbs的频点，要求能够锁频成功，并且锁频时间小于600ms。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、s_unDeliver[2].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency 需要锁频的频点的freq.
//@INPUT:3、s_unDeliver[2].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 需要锁频的频点的symbol.
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback 来添加回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
//@EXECUTIONFLOW:3、调用CSUDITunerConnect来锁设置的频点，要求返回CSUDI_SUCCESS，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除添加的回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "添加回调函数失败");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[2]),"锁频失败");

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
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"删除回调失败");
		}
	}

	return eRet;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:在QPSK初始化成功的前提下，锁定freq为2150000Khz,symbol为4000kbs的频点，要求能够锁频成功，并且锁频时间小于600ms。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、s_unDeliver[3].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency 需要锁频的频点的freq.
//@INPUT:3、s_unDeliver[3].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 需要锁频的频点的symbol.
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback 来添加回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
//@EXECUTIONFLOW:3、调用CSUDITunerConnect来锁设置的频点，要求返回CSUDI_SUCCESS，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除添加的回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "添加回调函数失败");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[3]),"锁频失败");

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
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"删除回调失败");
		}
	}

	return eRet;
}

//@CASEGROUP:CSUDITunerGetAllDeviceId 
//@CASEGROUP:CSUDITunerAddCallback 
//@CASEGROUP:CSUDITunerConnect 
//@CASEGROUP:CSUDITunerDelCallback 
//@DESCRIPTION:在QPSK初始化成功的前提下，锁定freq为2150000Khz,symbol为45000kbs的频点，要求能够锁频成功，并且锁频时间小于600ms。
//@PRECONDITION:1、设备初始化成功
//@INPUT:1、dwTunerId 通过CSUDITunerGetAllDeviceId获取
//@INPUT:2、s_unDeliver[4].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uFrequency 需要锁频的频点的freq.
//@INPUT:3、s_unDeliver[4].m_sSourceSatelliteDeliver.m_sSatelliteDeliver.m_uSymbolRate_24 需要锁频的频点的symbol.
//@EXECUTIONFLOW:1、调用CSUDITunerGetAllDeviceId 获取当前所有的设备ID，并将ID存入pdwIds中，要求返回CSUDI_SUCCESS，否则测试用例失败
//@EXECUTIONFLOW:2、调用CSUDITunerAddCallback 来添加回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
//@EXECUTIONFLOW:3、调用CSUDITunerConnect来锁设置的频点，要求返回CSUDI_SUCCESS，否则返回失败。
//@EXECUTIONFLOW:4、调用CSUDITunerDelCallback删除添加的回调，要求返回CSUDI_SUCCESS，否则测试用例失败。
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerGetAllDeviceId(adwTunerIds, sizeof(adwTunerIds)/sizeof(adwTunerIds[0]), &nActIdCnt), "步骤1失败");

	CSTK_ASSERT_TRUE_FATAL(nActIdCnt > 0, "Tuner数不能为0");
	
	do
	{
		s_nTunerTestData = 0;
		nSleepCount = 0;
		memset(&s_sTimeEnd,0,sizeof(CSUDIOSTimeVal_S));

		if(-1 != adwTunerIds[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL), "添加回调函数失败");
			nAddCallbackCount++;

			CSUDIOSGetTime(&sTimeStart);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(adwTunerIds[i], &s_unDeliver[4]),"锁频失败");

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
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerDelCallback(adwTunerIds[i],Tuner_CallbackTest2,NULL),"删除回调失败");
		}
	}

	return eRet;
}
#endif

