/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_recordtestcase.h"
#include "cs_recordtest.h"

#define DEMUX_COUNT 10
#define STREAM_INFO_SIZE 3
#define RECORD_TIME_SHORT 10*1000
#define RECORD_TIME_LONG 30*1000
#define PVRPLAY_TESTFILE_NAME "pvrplaytest"
#define TSHIFT_TESTFILE_NAME "tshiftplaytest"
#define MAX_CALLBACK_COUNT 32
#define MAX_ERROR_TIME 5 //获取时间信息的测试允许的最大误差范围(单位:秒)

char g_acFilePath[CSUDI_MAX_FILE_NAME_LEN] = {0};

static RecordTestServiceInfo_S s_sServiceInfo[] = 
{
	{
		"H.264.ts", 
		"H264+MPEG1-LAER2",
		0x1c01,
		EM_UDI_VID_STREAM_H264,
		0x1c02,
		EM_UDI_AUD_STREAM_MPEG1,
		0x1c01,
		-1,
		-1
	},
	{
		"H.264.ts", 
		"H264+MPEG1-LAER3",
		0x1801,
		EM_UDI_VID_STREAM_H264,
		0x1802,
		EM_UDI_AUD_STREAM_MPEG1,
		0x1802,
		-1,
		-1
	},
	{
		"H.264.ts", 
		"H264+MPEG2-AAC",
		0x1401,
		EM_UDI_VID_STREAM_H264,
		0x1402,
		EM_UDI_AUD_STREAM_MPEG_AAC,
		0x1401,
		-1,
		-1
	},
	{
		"H.264.ts", 
		"H264+MPEG4-AAC",
		0x1001,
		EM_UDI_VID_STREAM_H264,
		0x1002,
		EM_UDI_AUD_STREAM_MPEG_AAC,
		0x1001,
		-1,
		-1
	},
	{
		"H.264.ts", 
		"MPEG1视频测试节目2",
		0x64,
		EM_UDI_VID_STREAM_H264,
		0x65,
		EM_UDI_AUD_STREAM_MPEG_AAC,
		0x64,
		-1,
		-1
	}		
};

CSUDI_BOOL CSTC_RECORD_Init(void)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	char acBuf[CSUDI_MAX_FILE_NAME_LEN] = {0};
	CSUDIFSFileStat_S sFileStat;

	if (CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("RECORD", "FILE_STORE_PATH", acBuf, sizeof(acBuf)))
	{
		memcpy(g_acFilePath, acBuf, CSUDI_MAX_FILE_NAME_LEN);
		CSTCPrint("file store path: %s\n", g_acFilePath);

		memset(&sFileStat, 0, sizeof(sFileStat));
		if (CSUDIFSStat(g_acFilePath, &sFileStat) == 0)
		{
			bRet = CSUDI_TRUE;
		}
		else
		{
			CSTCPrint("所配置的文件路径不存在\n");
		}
	}

	return bRet;
}

CSUDI_BOOL CSTC_RECORD_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

static int iRecCallback(CSUDI_HANDLE hRecHandle, const CSUDIRecEvent_E eEvent, void* pvEventData, void * pvUserData)
{
	return 0;
}

//@CASEGROUP:CSUDIRECAddCallback, CSUDIRECDelCallback
//@DESCRIPTION:测试回调的正常添加和删除
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、合法参数
//@EXPECTATION:返回成功
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS, 且句柄不为空
//@EXECUTIONFLOW:3、调用CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData1)添加一个未添加过的回调, pvUserData1 != CSUDI_NULL, 期望返回CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECDelCallback(hRecHandle, fnRecordCallback, pvUserData1)删除步骤3添加的回调, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData2), 其中fnRecordCallback与步骤2中fnRecordCallback为同一个回调函数指针,pvUserData2 != pvUserData1且pvUserData2 != CSUDI_NULL, 期望返回CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIRECDelCallback(hRecHandle, fnRecordCallback, pvUserData2)删除步骤4添加的回调, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:7、调用CSUDIRECAddCallback(hRecHandle, fnRecordCallback, CSUDI_NULL), 其中fnRecordCallback与步骤2中fnRecordCallback为同一个回调函数指针, 期望返回CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIRECDelCallback(hRecHandle, fnRecordCallback, CSUDI_NULL)删除步骤5添加的回调, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、调用CSUDIRECClose(hRecHandle)销毁步骤2创建的录制器, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_AddDelCallback_0001(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int nUserData1 = 31;
	int nUserData2 = 32;

	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)),
			"创建录制器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)&nUserData1)),
			"添加回调失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)&nUserData1)),
			"删除回调失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)&nUserData2)),
			"添加回调失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)&nUserData2)),
			"删除回调失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, CSUDI_NULL)),
			"添加回调失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, CSUDI_NULL)),
			"删除回调失败");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECAddCallback, CSUDIRECDelCallback
//@DESCRIPTION:测试调用CSUDIRECAddCallback, CSUDIRECDelCallback传入参数错误时能返回对应错误码
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、hRecHandle为CSUDI_NULL, 其他参数合法
//@INPUT:2、fnRecordCallback为CSUDI_NULL, 其他参数合法
//@INPUT:3、hRecHandle和fnRecordCallback为CSUDI_NULL, 其他参数合法
//@EXPECTATION:返回对应错误码
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS, 且句柄不为空
//@EXECUTIONFLOW:3、调用CSUDIRECAddCallback(CSUDI_NULL, fnRecordCallback, pvUserData), 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:4、调用CSUDIRECAddCallback(CSUDI_NULL, fnRecordCallback, pvUserData), 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:5、调用CSUDIRECAddCallback(hRecHandle, CSUDI_NULL, pvUserData), hRecHandle为上述步骤调用CSUDIRECOpen输出, 期望返回CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIRECDelCallback(CSUDI_NULL, fnRecordCallback, pvUserData), 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:7、调用CSUDIRECDelCallback(CSUDI_NULL, CSUDI_NULL, pvUserData), 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8、调用CSUDIRECDelCallback(hRecHandle, CSUDI_NULL, pvUserData), hRecHandle为上述步骤调用CSUDIRECOpen输出, 期望返回CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:9、调用CSUDIRECClose(hRecHandle)销毁创建的录制器, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_AddDelCallback_0002(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int nUserData = 1;

	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"创建录制器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECAddCallback(CSUDI_NULL, iRecCallback, (void*)&nUserData)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECAddCallback(CSUDI_NULL, CSUDI_NULL, (void*)&nUserData)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECAddCallback(hRecHandle, CSUDI_NULL, (void*)&nUserData)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECDelCallback(CSUDI_NULL, iRecCallback, (void*)&nUserData)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECDelCallback(CSUDI_NULL, CSUDI_NULL, (void*)&nUserData)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECDelCallback(hRecHandle, CSUDI_NULL, (void*)&nUserData)),
			"返回错误码不正确");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECAddCallback, CSUDIRECDelCallback
//@DESCRIPTION:测试重复添加和删除同一个回调时能返回对应错误码
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、合法参数
//@EXPECTATION:返回对应错误码
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS, 且句柄不为空
//@EXECUTIONFLOW:3、调用CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData)添加一个未添加过的回调, 期望返回CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData), 期望返回CSUDIREC_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW:5、调用CSUDIRECDelCallback(hRecHandle, fnRecordCallback, pvUserData)删除回调, 期望返回CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIRECDelCallback(hRecHandle, fnRecordCallback, pvUserData), 期望返回CSUDIREC_ERROR_NO_CALLBACK
//@EXECUTIONFLOW:7、调用CSUDIRECClose(hRecHandle)销毁创建的录制器, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_AddDelCallback_0003(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int nUserData = 2;

	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"创建录制器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)&nUserData)),
			"添加回调失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_ALREADY_ADDED == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)&nUserData)),
			"添加一个已经添加的回调错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)&nUserData)),
			"删除回调失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_NO_CALLBACK == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)&nUserData)),
			"删除一个已经删除过的回调错误码不正确");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)),
				"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECAddCallback, CSUDIRECDelCallback
//@DESCRIPTION:测试能够支持同时存在32个回调(回调函数指针相同, 用户数据不同)
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、合法参数
//@EXPECTATION:返回成功
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS且句柄不为空
//@EXECUTIONFLOW:3、重复调用CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData)添加一个未添加过的回调(pvUserData每次变化)共32次, 期望均返回CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:4、重复调用CSUDIRECDelCallback删除上述步骤添加的所有回调, 期望均返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIRECClose(hRecHandle)销毁已创建的录制器, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_AddDelCallback_0004(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int anAddFlag[MAX_CALLBACK_COUNT] = {0};
	int i = 0;

	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"创建录制器失败");

	for (i=0; i<MAX_CALLBACK_COUNT; i++)
	{
		CSTCPrint("添加第%d个回调\n", i+1);
		
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)i)),
			"添加回调失败");
		
		anAddFlag[i] = 1;
	}

	CSTK_FATAL_POINT;

	for (i=0; i<MAX_CALLBACK_COUNT; i++)
	{
		if (anAddFlag[i] == 1)
		{
			CSTCPrint("删除第%d个回调\n", i+1);
		
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)i)),
				"删除回调失败");

			anAddFlag[i] = 0;
		}		
	}

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

static CSUDI_BOOL s_bEventRight = CSUDI_FALSE;
static CSUDI_BOOL s_bUserdataWrong = CSUDI_FALSE;
static int iRecCallback_0005(CSUDI_HANDLE hRecHandle, const CSUDIRecEvent_E eEvent, void* pvEventData, void * pvUserData)
{
	CSTCPrint("callback comming, event=%d, userdata=0x%x\n", eEvent, *(int *)pvUserData);
	
	if (eEvent == EM_UDIRECORD_EVT_DISKFULL)
	{
		s_bEventRight = CSUDI_TRUE;
	}

	if (*(int *)pvUserData != 0x1234)
	{
		s_bUserdataWrong = CSUDI_TRUE;
	}
	
	return 0;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIRECAddCallback, CSUDIRECDelCallback
//@DESCRIPTION:测试能否监听到存储设备没有空间消息, userdata是否正确
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化
//@INPUT:1、合法参数
//@EXPECTATION:能监听到消息且返回userdata和设置的相同
//@EXECUTIONFLOW:1、提示测试者插入剩余空间小于5M的可移动存储设备用于存储录制文件, 并修改配置文件中存储路径配置项
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找支持录制的demux, 期望成功
//@EXECUTIONFLOW:4、连接tuner和demux, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:6、重复调用CSUDIRECAddCallback添加回调, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIRECStart启动录制, 期望成功
//@EXECUTIONFLOW:8、期望30S内能收到存储空间满的消息且userdata与设置的相同
//@EXECUTIONFLOW:9、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:10、重复调用CSUDIRECDelCallback删除回调, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECClose销毁录制器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_AddDelCallback_0005(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nUserData = 0x1234;
	int nRecTime = 0;
	CSUDI_BOOL bCallbackAdded = CSUDI_FALSE;
	
	memset(&sStartPar, 0, sizeof(sStartPar));	
	
	CSTCPrint("测试能否监听到存储设备没有空间消息\n");
	CSTCPrint("请使用剩余空间小于5M的存储设备并保证配置文件中存储路径配置正确\n");
	CSTCPrint("y:准备就绪，开始测试；n:未准备就绪，暂不测试\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未准备就绪，暂不测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec)) && (CSUDI_NULL != hRec)), 
			"创建录制器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRec, iRecCallback_0005, (void*)&nUserData)),
			"添加回调失败");
	bCallbackAdded = CSUDI_TRUE;

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)), 
			"启动录制失败");

	while (!s_bEventRight && nRecTime<30)
	{
		CSUDIOSThreadSleep(1000);
		nRecTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL((s_bEventRight), "未收到存储空间满的消息");

	CSTK_ASSERT_TRUE_FATAL((!s_bUserdataWrong), "回调userdata不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRec, iRecCallback_0005, (void*)&nUserData)),
			"删除回调失败");
	bCallbackAdded = CSUDI_FALSE;

	CSTK_FATAL_POINT;

	if (bCallbackAdded)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECDelCallback(hRec, iRecCallback_0005, (void*)&nUserData)),
				"删除回调失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
				"销毁录制器失败");
	}

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose
//@DESCRIPTION:测试对同一个demux多次创建和销毁录制器
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、合法参数
//@EXPECTATION:都能成功
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen(psChnl, phRecHandle1)创建录制器1,  期望返回CSUDI_SUCCESS且句柄不为空
//@EXECUTIONFLOW:3、调用CSUDIRECOpen(psChnl, phRecHandle2)创建录制器2,  期望返回CSUDI_SUCCESS且句柄不为空
//@EXECUTIONFLOW:4、调用CSUDIRECClose(hRecHandle1)销毁录制器1, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、调用CSUDIRECClose(hRecHandle2)销毁录制器2, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_OpenClose_0001(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle1 = CSUDI_NULL;
	CSUDI_HANDLE hRecHandle2 = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;

	nDemuxId = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle1)) && (CSUDI_NULL != hRecHandle1)), 
			"创建录制器1失败");

	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle2)) && (CSUDI_NULL != hRecHandle2)), 
			"创建录制器2失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle1)), 
			"销毁录制器1失败");
	hRecHandle1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle2)), 
			"销毁录制器2失败");
	hRecHandle2 = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if (hRecHandle1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle1)), 
				"销毁录制器1失败");
	}

	if (hRecHandle2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle2)), 
				"销毁录制器2失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose
//@DESCRIPTION:测试所有支持录制的demux正常创建和销毁录制器
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、合法参数
//@EXPECTATION:返回CSUDI_SUCCESS
//@EXECUTIONFLOW:1、查找出所有支持录制的demux, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen(psChnl, phRecHandle), 创建一个录制器,  期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIRECClose(hRecHandle)销毁上述步骤创建的录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、针对所有支持录制的demux(即psChnl->m_nDemux取不同的demuxId值)重复以上步骤
CSUDI_BOOL CSTC_RECORD_IT_OpenClose_0002(void)
{
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;
	int i = 0;
	CSUDIRecChnl_S sRecChnl;
	CSUDI_HANDLE ahRecHandle[DEMUX_COUNT] = {CSUDI_NULL};

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt > 0),
			"未查找任何支持录制的demux");

	for (i=0; i<nActualCnt; i++)
	{
		CSTCPrint("创建第%d个录制器，demuxId=%d\n", i+1, anDemuxId[i]);
		
		sRecChnl.m_nDemux = anDemuxId[i];
		CSTK_ASSERT_TRUE_FATAL(
				((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRecHandle[i])) && (CSUDI_NULL != ahRecHandle[i])), 
				"创建录制器失败");

		CSTCPrint("销毁第%d个录制器\n", i+1);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(ahRecHandle[i])), 
				"销毁录制器失败");
		ahRecHandle[i] = CSUDI_NULL;
	}

	CSTK_FATAL_POINT;

	for (i=0; i<nActualCnt; i++)
	{
		if (ahRecHandle[i] != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRecHandle[i])), 
					"销毁录制器失败");
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose
//@DESCRIPTION:测试调用CSUDIRECOpen, CSUDIRECClose传入参数错误时能返回对应错误码
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、psChnl为CSUDI_NULL, phRecHandle合法
//@INPUT:2、psChnl合法, phRecHandle为CSUDI_NULL
//@INPUT:3、psChnl的m_nDemux为-1, phRecHandle合法
//@INPUT:4、psChnl的m_nDemux为不支持录制的demuxId, phRecHandle合法
//@INPUT:5、hRecHandle为CSUDI_NULL
//@EXPECTATION:返回对应错误码
//@EXECUTIONFLOW:1、调用CSUDIRECOpen(CSUDI_NULL, phRecHandle), 其中phRecHandle为合法值, 期望返回CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2、调用CSUDIRECOpen(psChnl, CSUDI_NULL), 其中psChnl为合法值, 期望返回CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3、调用CSUDIRECOpen(psChnl, CSUDI_NULL), 其中psChnl->m_nDemux=-1, 期望返回CSUDIREC_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:4、查找一个不支持录制的demux, 如果存在则执行步骤5, 否则执行步骤6
//@EXECUTIONFLOW:5、调用CSUDIRECOpen(psChnl, CSUDI_NULL), 其中psChnl->m_nDemux为不支持录制的demuxId, 期望返回CSUDIREC_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:6、调用CSUDIRECClose(CSUDI_NULL), 期望返回CSUDIREC_ERROR_INVALID_HANDLE
CSUDI_BOOL CSTC_RECORD_IT_OpenClose_0003(void)
{
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int nDemuxId = -1;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECOpen(CSUDI_NULL, &hRecHandle)), 
			"返回错误码不正确");

	nDemuxId = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECOpen(&sRecChnl, CSUDI_NULL)), 
			"返回错误码不正确");

	sRecChnl.m_nDemux = -1;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_DEVICE_ID == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"返回错误码不正确");

	nDemuxId = -1;
	nDemuxId = FindDemuxNotSupportRec();

	if (nDemuxId >= 0)
	{
		sRecChnl.m_nDemux = nDemuxId;
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDIREC_ERROR_INVALID_DEVICE_ID == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
				"返回错误码不正确");
	}
	else
	{
		CSTCPrint("未找到不支持录制的demux\n");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECClose(CSUDI_NULL)), 
			"返回错误码不正确");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose
//@DESCRIPTION:测试销毁一个已经销毁的录制器
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、合法参数
//@EXPECTATION:都能成功
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen(psChnl, phRecHandle)创建一个录制器,  期望返回CSUDI_SUCCESS且句柄不为空
//@EXECUTIONFLOW:3、调用CSUDIRECClose(hRecHandle)销毁录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、再次调用CSUDIRECClose(hRecHandle), 期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_OpenClose_0004(void)
{	
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDI_HANDLE hRecHandleBak = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"创建录制器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	
	hRecHandleBak = hRecHandle;
	hRecHandle = CSUDI_NULL;

	if (hRecHandleBak != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS != CSUDIRECClose(hRecHandleBak)), 
				"试图销毁已经销毁的录制器应该失败");
	}

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECStart, CSUDIRECStop, CSUDIRECStopTshift
//@DESCRIPTION:测试调用CSUDIRECStart, CSUDIRECStop, CSUDIRECStopTshift传入参数错误时能返回对应错误码
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、hRecHandle为CSUDI_NULL, psStartParam合法
//@INPUT:2、hRecHandle合法, psStartParam为CSUDI_NULL
//@INPUT:3、hRecHandle为CSUDI_NULL
//@EXPECTATION:返回对应错误码
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS且句柄不为空
//@EXECUTIONFLOW:3、调用CSUDIRECStart(CSUDI_NULL, psStartParam), 其中psStartParam合法, 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:4、调用CSUDIRECStart(hRecHandle, CSUDI_NULL), 其中hRecHandle为CSUDIRECOpen输出, 期望返回CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIRECStart(CSUDI_NULL, CSUDI_NULL), 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:6、调用CSUDIRECStop(CSUDI_NULL), 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:7、调用CSUDIRECStopTshift(CSUDI_NULL, CSUDI_FALSE), 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8、调用CSUDIRECClose(hRecHandle)销毁已创建的录制器, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_StartStop_0001(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	
	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"创建录制器失败");

	memset(&sStartPar, 0, sizeof(sStartPar));
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECStart(CSUDI_NULL, &sStartPar)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECStart(hRecHandle, CSUDI_NULL)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECStart(CSUDI_NULL, CSUDI_NULL)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECStop(CSUDI_NULL)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECStopTshift(CSUDI_NULL, CSUDI_FALSE)),
			"返回错误码不正确");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:测试非法状态下调用CSUDIRECStart, CSUDIRECStop时能返回对应错误码
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、合法参数
//@EXPECTATION:返回对应错误码
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS且句柄不为空
//@EXECUTIONFLOW:4、调用CSUDIRECStop(hRecHandle), 期望返回CSUDIREC_ERROR_NOT_STARTED
//@EXECUTIONFLOW:5、调用CSUDIRECStart(hRecHandle, psStartParam)启动录制, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、调用CSUDIRECStart(hRecHandle, psStartParam), 期望返回CSUDIREC_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:7、调用CSUDIRECStop(hRecHandle), 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、调用CSUDIRECStop(hRecHandle), 期望返回CSUDIREC_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:9、调用CSUDIRECClose(hRecHandle)销毁已创建的录制器, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_StartStop_0002(void)
{
	int nDemuxId = -1;
	CSUDI_UINT32 dwTunerId = 0;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;

	memset(&sStartPar, 0, sizeof(sStartPar));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");
	
	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, nDemuxId)),
			"连接tuner和demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"创建录制器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_NOT_STARTED == CSUDIRECStop(hRecHandle)), 
			"返回错误代码不正确");
	
	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);

	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_ALREADY_STARTED == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"返回错误代码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"停止录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_ALREADY_STOPPED == CSUDIRECStop(hRecHandle)), 
			"返回错误代码不正确");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECStart
//@DESCRIPTION:测试普通录制, 传入非法PID(包括无PID)的情况
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、非法PID
//@EXPECTATION:返回对应错误码
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS且句柄不为空
//@EXECUTIONFLOW:3、调用CSUDIRECStart(hRecHandle, psStartParam)启动录制(普通录制, 音视频PID均为-1), 期望返回CSUDIREC_ERROR_INVALID_PIDS
//@EXECUTIONFLOW:4、调用CSUDIRECStart(hRecHandle, psStartParam)启动录制(普通录制, PID个数为0), 期望返回CSUDIREC_ERROR_INVALID_PIDS
//@EXECUTIONFLOW:5、调用CSUDIRECClose(hRecHandle)销毁已创建的录制器, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_StartStop_0003(void)
{
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam;

	memset(&sRecStartParam, 0, sizeof(sRecStartParam));

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec)) && (CSUDI_NULL != hRec)), 
			"创建录制器失败");

	sRecStartParam.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam.m_bAppend = CSUDI_FALSE;
	sRecStartParam.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam.m_nCount = 2;
	sRecStartParam.m_psPids[0].m_nPid = -1;
	sRecStartParam.m_psPids[0].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam.m_psPids[0].m_uStreamType.m_eVideoType = EM_UDI_VID_STREAM_MPEG2;
	sRecStartParam.m_psPids[1].m_nPid = -1;
	sRecStartParam.m_psPids[1].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam.m_psPids[1].m_uStreamType.m_eAudioType = EM_UDI_AUD_STREAM_MPEG2;
	sprintf(sRecStartParam.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_PIDS == CSUDIRECStart(hRec, &sRecStartParam)),
			"返回错误代码不正确");

	sRecStartParam.m_nCount = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_PIDS == CSUDIRECStart(hRec, &sRecStartParam)),
			"返回错误代码不正确");
	
	CSTK_FATAL_POINT;

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRec)), 
				"销毁录制器失败");
	}

	ClearFile(sRecStartParam.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECGetSupportEncryptTypes
//@DESCRIPTION:测试调用CSUDIRECGetSupportEncryptTypes传入参数错误时能返回对应错误码
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、hRecHandle为CSUDI_NULL, 其他参数合法
//@INPUT:2、peOutTypeArray为CSUDI_NULL, 其他参数合法
//@INPUT:3、nArraySize为0, 其他参数合法
//@INPUT:4、pnActualCnt为CSUDI_NULL, 其他参数合法
//@EXPECTATION:返回对应错误码
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIRECGetSupportEncryptTypes(CSUDI_NULL, peOutTypeArray, nArraySize, pnActualCnt), 其中peOutTypeArray,nArraySize,pnActualCnt合法, 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:4、调用CSUDIRECGetSupportEncryptTypes(hRecHandle, CSUDI_NULL, nArraySize, pnActualCnt), 其中hRecHandle,nArraySize,pnActualCnt合法, 期望返回CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIRECGetSupportEncryptTypes(hRecHandle, peOutTypeArray, 0, pnActualCnt), 其中hRecHandle,peOutTypeArray,pnActualCnt合法, 期望返回CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、调用CSUDIRECGetSupportEncryptTypes(hRecHandle, peOutTypeArray, nArraySize, CSUDI_NULL), 其中hRecHandle,nArraySize,peOutTypeArray合法, 期望返回CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:7、调用CSUDIRECClose(hRecHandle)销毁步骤2创建的录制器, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_GetSupportEncryptTypes_0001(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIStreamEncryptType_E aeEncryptType[3] = {0};
	int nArraySize = sizeof(aeEncryptType);
	int nActualCnt = 0;
	
	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"创建录制器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECGetSupportEncryptTypes(CSUDI_NULL, aeEncryptType, nArraySize, &nActualCnt)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECGetSupportEncryptTypes(hRecHandle, CSUDI_NULL, nArraySize, &nActualCnt)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECGetSupportEncryptTypes(hRecHandle, aeEncryptType, 0, &nActualCnt)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECGetSupportEncryptTypes(hRecHandle, aeEncryptType, nArraySize, CSUDI_NULL)),
			"返回错误码不正确");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECGetSupportEncryptTypes
//@DESCRIPTION:测试正常获取录制支持的加密方式 
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、合法参数
//@EXPECTATION:返回CSUDI_SUCCESS且获取到的信息与配置文件中的信息一致
//@EXECUTIONFLOW:1、查找出所有支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIRECGetSupportEncryptTypes(hRecHandle, peOutTypeArray, nArraySize, pnActualCnt), 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、将步骤3中获取到的信息和配置文件中的信息比较, 期望一致
//@EXECUTIONFLOW:5、调用CSUDIRECClose(hRecHandle)销毁已创建的录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、针对所有支持录制的demux重复步骤2~5
CSUDI_BOOL CSTC_RECORD_IT_GetSupportEncryptTypes_0002(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIStreamEncryptType_E aeEncryptType[3] = {0};
	int nArraySize = sizeof(aeEncryptType);
	int nActualCnt = 0;
	char acBuf[64] = {0};
	int nSupportFree = 0;
	int nSupportCA = 0;
	int nSupportReencrypt = 0;
	int nCount = 0;
	int i = 0;
	
	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "查找支持录制的demux失败");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"创建录制器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECGetSupportEncryptTypes(hRecHandle, aeEncryptType, nArraySize, &nActualCnt)),
			"获取录制支持的加密方式失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("RECORD", "ENCRYPT_TYPE_FREE", acBuf, sizeof(acBuf))),
			"从配置文件中获取是否支持 FREE 加密方式失败");
	nSupportFree = CSTKGetIntFromStr(acBuf,10);
	if (nSupportFree == 1)
	{
		nCount ++;
	}

	memset(acBuf, 0, sizeof(acBuf));
	CSTK_ASSERT_TRUE_FATAL(
			(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("RECORD", "ENCRYPT_TYPE_CA", acBuf, sizeof(acBuf))),
			"从配置文件中获取是否支持 CA 加密方式失败");
	nSupportCA = CSTKGetIntFromStr(acBuf,10);
	if (nSupportCA == 1)
	{
		nCount ++;
	}

	memset(acBuf, 0, sizeof(acBuf));
	CSTK_ASSERT_TRUE_FATAL(
			(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("RECORD", "ENCRYPT_TYPE_REENCRYPT", acBuf, sizeof(acBuf))),
			"从配置文件中获取是否支持 REENCRYPT 加密方式失败");
	nSupportReencrypt = CSTKGetIntFromStr(acBuf,10);
	if (nSupportReencrypt == 1)
	{
		nCount ++;
	}

	CSTK_ASSERT_TRUE_FATAL((nCount == nActualCnt), "获取到的支持的加密方式个数不正确");

	for (i=0; i<nActualCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				((aeEncryptType[i] == EM_UDI_ENCRYPT_TYPE_FREE && nSupportFree == 1) 
				|| (aeEncryptType[i] == EM_UDI_ENCRYPT_TYPE_CA && nSupportCA == 1)
				|| (aeEncryptType[i] == EM_UDI_ENCRYPT_TYPE_REENCRYPT && nSupportReencrypt == 1)),
				"获取到的支持的加密方式不正确");
	}
		
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECGetRecTime
//@DESCRIPTION:测试调用CSUDIRECGetRecTime传入参数错误时能返回对应错误码
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、hRecHandle为CSUDI_NULL, 其他参数合法
//@INPUT:2、psTimeInfo为CSUDI_NULL, 其他参数合法
//@INPUT:3、所有参数合法, 但录制尚未启动
//@EXPECTATION:返回对应错误码
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:2、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、调用CSUDIRECGetRecTime(CSUDI_NULL, psTimeInfo), 其中psTimeInfo合法, 期望返回CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:4、调用CSUDIRECGetRecTime(hRecHandle, CSUDI_NULL), 其中hRecHandle为CSUDIRECOpen输出, 期望返回CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、调用CSUDIRECGetRecTime(hRecHandle, psTimeInfo), 其中hRecHandle和psTimeInfo均合法, 期望返回CSUDIREC_ERROR_TIME_FAILED
//@EXECUTIONFLOW:6、调用CSUDIRECClose(hRecHandle)销毁已创建的录制器, 期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_GetRecTime_0001(void)
{
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecTimeInfo_S sRecTimeInfo;
	
	sRecChnl.m_nDemux = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"创建录制器失败");

	memset(&sRecTimeInfo, 0, sizeof(sRecTimeInfo));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECGetRecTime(CSUDI_NULL, &sRecTimeInfo)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECGetRecTime(hRecHandle, CSUDI_NULL)),
			"返回错误码不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_TIME_FAILED == CSUDIRECGetRecTime(hRecHandle, &sRecTimeInfo)),
			"返回错误码不正确");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIRECGetRecTime
//@DESCRIPTION:测试获取普通录制获取录制时间信息的正确性
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:合法参数
//@EXPECTATION:能够获取到正确的录制时间信息
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:3、连接demux和tuner, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIRECStart启动录制(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:6、等待录制30S
//@EXECUTIONFLOW:7、调用CSUDIRECGetRecTime获取录制时间信息, 期望成功
//@EXECUTIONFLOW:8、判断录制时间信息中录制开始的绝对时间是否为0, 期望为是
//@EXECUTIONFLOW:9、判断录制时间信息中录制有效时间是否在30左右(前后允许误差各5S), 期望为是
//@EXECUTIONFLOW:10、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:12、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_GetRecTime_0002(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	CSUDIRecTimeInfo_S sTimeInfo;
	int nRecTime = RECORD_TIME_LONG;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);

	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	memset(&sTimeInfo, 0, sizeof(sTimeInfo));
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECGetRecTime(hRecHandle, &sTimeInfo)),
			"获取录制时间信息失败");

	CSTCPrint("录制时间信息 starttime=%d, duration=%d\n", sTimeInfo.m_nStartTime, sTimeInfo.m_nValidDuration);
	CSTK_ASSERT_TRUE_FATAL(
			(sTimeInfo.m_nStartTime == 0), 
			"录制时间信息中录制开始的绝对时间不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(sTimeInfo.m_nValidDuration >= (nRecTime/1000-MAX_ERROR_TIME) && sTimeInfo.m_nValidDuration <= (nRecTime/1000+MAX_ERROR_TIME)), 
			"录制时间信息中录制有效时间不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"停止录制失败");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIRECGetRecTime
//@DESCRIPTION:测试时移录制获取录制时间信息的正确性
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、时移文件未达到最大限定大小
//@EXPECTATION:能够获取到正确的录制时间信息
//@REMARK:  这里只测试了录制未超过最大限定大小的情况，已超过情况在CSTC_RECORD_IT_TShiftRec_0004可兼顾测到
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:3、连接demux和tuner, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIRECStart启动录制(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:6、等待录制30S
//@EXECUTIONFLOW:7、调用CSUDIRECGetRecTime获取录制时间信息, 期望成功
//@EXECUTIONFLOW:8、判断录制时间信息中录制开始的绝对时间是否大于等于0, 期望为是
//@EXECUTIONFLOW:9、判断录制时间信息中录制有效时间是否在30左右(前后允许误差各5S), 期望为是
//@EXECUTIONFLOW:10、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:12、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_GetRecTime_0003(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	CSUDIRecTimeInfo_S sTimeInfo;
	int nRecTime = RECORD_TIME_LONG;

	CSTK_ASSERT_TRUE_FATAL(
			CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"创建录制器失败");

	memset(&sStartPar, 0, sizeof(sStartPar));	
	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nMaxSize = TSHIFT_MAX_SIZE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);

	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	memset(&sTimeInfo, 0, sizeof(sTimeInfo));
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECGetRecTime(hRecHandle, &sTimeInfo)),
			"获取录制时间信息失败");

	CSTCPrint("录制时间信息 starttime=%d, duration=%d\n", sTimeInfo.m_nStartTime, sTimeInfo.m_nValidDuration);
	CSTK_ASSERT_TRUE_FATAL(
			(sTimeInfo.m_nStartTime >= 0), 
			"录制时间信息中录制开始的绝对时间不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(sTimeInfo.m_nValidDuration >= (nRecTime/1000-MAX_ERROR_TIME) && sTimeInfo.m_nValidDuration <= (nRecTime/1000+MAX_ERROR_TIME)), 
			"录制时间信息中录制有效时间不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"停止录制失败");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECFileCut
//@DESCRIPTION:测试调用CSUDIRECFileCut传入参数错误时能返回对应错误码
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化且存在支持录制的demux
//@INPUT:1、szFileName为CSUDI_NULL, 其他参数合法
//@EXPECTATION:返回对应错误码
//@EXECUTIONFLOW:1、调用CSUDIRECFileCut(CSUDI_NULL, 5, 10), 期望返回CSUDIREC_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_RECORD_IT_FileCut_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECFileCut(CSUDI_NULL, 5, 10)), 
				"返回错误码不正确");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECFileCut
//@DESCRIPTION:测试对已存在的录制文件切割的正确性
//@PRECONDITION:1、Record模块正常初始化
//@PRECONDITION:2、Demux模块正常初始化
//@INPUT:1、合法参数
//@EXPECTATION:切割后文件减小
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:3、连接demux和tuner, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIRECStart启动录制(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:6、等待录制10S
//@EXECUTIONFLOW:10、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:11、检查录制的文件大小是否大于0, 期望为是
//@EXECUTIONFLOW:11、调用CSUDIRECFileCut切割录制的文件, 切割时间为2到5秒, 期望成功
//@EXECUTIONFLOW:11、获取切割时候的文件大小, 期望大于0且小于切割前文件大小
//@EXECUTIONFLOW:12、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_FileCut_0002(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;
	unsigned long lFileSizeBefore = 0;
	unsigned long lFileSizeAfter = 0;
	
	memset(&sStartPar, 0, sizeof(sStartPar));	
	
	CSTK_ASSERT_TRUE_FATAL(
			CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"停止录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	hRecHandle = CSUDI_NULL;

	lFileSizeBefore = GetRecFileSize(sStartPar.m_szFileName);
	CSTK_ASSERT_TRUE_FATAL((lFileSizeBefore > 0), "录制的文件大小检查不通过");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECFileCut(sStartPar.m_szFileName, 2, 5)), 
			"录制文件切割失败");

	lFileSizeAfter = GetRecFileSize(sStartPar.m_szFileName);
	CSTK_ASSERT_TRUE_FATAL(
			(lFileSizeAfter > 0 && lFileSizeAfter < lFileSizeBefore),
			"切割后的文件大小不正确");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");
	
	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试录制一路节目、非追加录制、清流
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制, 录制的文件播放正常
//@EXECUTIONFLOW:1、锁定要录制的码流所在频点, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECStart启动录制(录制一路节目、非追加录制、清流), 期望成功
//@EXECUTIONFLOW:5、等待10S录制时间
//@EXECUTIONFLOW:6、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:8、查看录制的文件已经存在且大小不为0, 期望成功
//@EXECUTIONFLOW:9、查找支持播放的demux及可用的音视频解码器, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIPLAYEROpen创建PVR录制播放模式的播放器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart启动播放, 期望成功
//@EXECUTIONFLOW:13、询问测试者录制文件是否正常播放, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:16、断开上述步骤连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_ChnlRec_0001(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle) && hRecHandle != CSUDI_NULL), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);

	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"停止录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("录制完毕，开始录制文件的检查，请注意观察音视频能否正常播放\n");
	CSTCPrint("录制时长为%dms，此时间内正常播放即可\n", nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"录制文件检查不通过");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:测试录制一路节目、非追加录制、二次加密流
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制且录制的文件能正常播放
//@EXECUTIONFLOW:1、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:2、锁定要录制的码流所在频点, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECStart启动录制(录制一路节目、非追加录制、二次加密流, 设置加密密钥), 期望成功
//@EXECUTIONFLOW:5、等待30S录制时间
//@EXECUTIONFLOW:6、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:8、断开步骤上述步骤连接的demux和tuner, 期望成功
//@EXECUTIONFLOW:9、查看录制的文件已经存在且大小不为0, 期望成功
//@EXECUTIONFLOW:10、查找支持播放的demux及可用的音视频解码器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYEROpen创建PVR录制播放模式的播放器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStart启动播放, 期望成功
//@EXECUTIONFLOW:14、询问测试者是否已经正常播放出来, 期望为是
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_ChnlRec_0002(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_REENCRYPT;
	sStartPar.m_stEncryptParam.m_ucEncryptKey[0] = 'a';
	sStartPar.m_stEncryptParam.m_nEncryptKeyLength = 16;	
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);

	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"停止录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("录制完毕，开始录制文件的检查，请注意观察音视频能否正常播放\n");
	CSTCPrint("录制时长为%d，%dms内正常播放即可\n", nRecTime, nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"录制文件检查不通过");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:测试时移录制、非追加录制、清流
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制且录制的文件能正常播放
//@EXECUTIONFLOW:1、锁定要录制的码流所在频点, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECStart启动录制(时移录制、非追加录制、清流), 期望成功
//@EXECUTIONFLOW:5、等待10S录制时间
//@EXECUTIONFLOW:6、调用CSUDIRECStopTshift停止时移录制, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:8、查看录制的文件已经存在且大小不为0, 期望成功
//@EXECUTIONFLOW:9、查找支持播放的demux及可用的音视频解码器, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIPLAYEROpen创建时移播放模式的播放器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart启动播放, 期望成功
//@EXECUTIONFLOW:13、询问测试者录制文件是否正常播放, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:16、断开上述步骤连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_TShiftRec_0001(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;
	
	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nMaxSize = TSHIFT_MAX_SIZE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRecHandle, CSUDI_FALSE)), 
			"停止时移录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("录制完毕，开始录制文件的检查，请注意观察音视频能否正常播放\n");
	CSTCPrint("录制时长为%dms，此时间内正常播放即可\n", nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"录制文件检查不通过");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:测试时移录制、非追加录制、二次加密流
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制且录制的文件能够正常播放
//@EXECUTIONFLOW:1、锁定要录制的码流所在频点, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECStart启动录制(时移录制、非追加录制、二次加密流), 期望成功
//@EXECUTIONFLOW:5、等待10S录制时间
//@EXECUTIONFLOW:6、调用CSUDIRECStopTshift停止时移录制, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:8、查看录制的文件已经存在且大小不为0, 期望成功
//@EXECUTIONFLOW:9、查找支持播放的demux及可用的音视频解码器, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIPLAYEROpen创建时移播放模式的播放器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart启动播放, 期望成功
//@EXECUTIONFLOW:13、询问测试者录制文件是否正常播放, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:16、断开上述步骤连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_TShiftRec_0002(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_REENCRYPT;
	sStartPar.m_stEncryptParam.m_ucEncryptKey[0] = 'a';
	sStartPar.m_stEncryptParam.m_nEncryptKeyLength = 16;
	sStartPar.m_nMaxSize = TSHIFT_MAX_SIZE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);

	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRecHandle, CSUDI_FALSE)), 
			"停止时移录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("录制完毕，开始录制文件的检查，请注意观察音视频能否正常播放\n");
	CSTCPrint("录制时长为%d，%dms内正常播放即可\n", nRecTime, nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"录制文件检查不通过");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStopTshift
//@DESCRIPTION:测试时移录制停止时转换成普通文件
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制且录制的文件能够正常播放
//@EXECUTIONFLOW:1、锁定要录制的码流所在频点, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECStart启动录制(时移录制、非追加录制、清流), 期望成功
//@EXECUTIONFLOW:5、等待10S录制时间
//@EXECUTIONFLOW:6、调用CSUDIRECStopTshift停止时移录制并将时移文件转成普通文件, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:8、查看录制的文件已经存在且大小不为0, 期望成功
//@EXECUTIONFLOW:9、查找支持播放的demux及可用的音视频解码器, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIPLAYEROpen创建时移播放模式的播放器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart启动播放, 期望成功
//@EXECUTIONFLOW:13、询问测试者录制文件是否正常播放, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:16、断开上述步骤连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_TShiftRec_0003(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nMaxSize = TSHIFT_MAX_SIZE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRecHandle, CSUDI_TRUE)), 
			"停止时移录制且将时移文件转成普通文件失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("录制完毕，开始录制文件的检查，请注意观察音视频能否正常播放\n");
	CSTCPrint("录制时长为%dms，此时间内正常播放即可\n", nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"录制文件检查不通过");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:测试时移录制达到最大长度后继续录制一段时间的情况
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制且录制的文件能正常播放
//@EXECUTIONFLOW:1、锁定要录制的码流所在频点, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECStart启动录制(时移录制、非追加录制、清流), 期望成功
//@EXECUTIONFLOW:5、等待录制时间大约3分钟，当获取到的录制时间信息中起始时间大于0时继续
//@EXECUTIONFLOW:6、判断获取到的录制起始时间是否大于0, 期望为是
//@EXECUTIONFLOW:7、调用CSUDIRECStopTshift停止时移录制, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:9、查看录制的文件已经存在且大小不为0, 期望成功
//@EXECUTIONFLOW:10、查找支持播放的demux及可用的音视频解码器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYEROpen创建时移播放模式的播放器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStart启动播放, 期望成功
//@EXECUTIONFLOW:14、询问测试者录制文件是否正常播放, 期望为是
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:17、断开上述步骤连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_TShiftRec_0004(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_LONG;
	CSUDIRecTimeInfo_S sRecTimeInfo;
	int nWaitTime = 0;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nMaxSize = TSHIFT_MAX_SIZE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("录制时长需超过最大限定长度，大约需等待3分钟\n");

	memset(&sRecTimeInfo, 0, sizeof(sRecTimeInfo));
	while((sRecTimeInfo.m_nStartTime == 0) && (nWaitTime < 6))
	{
		CSUDIOSThreadSleep(nRecTime);

		memset(&sRecTimeInfo, 0, sizeof(sRecTimeInfo));
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECGetRecTime(hRecHandle, &sRecTimeInfo)),
				"获取录制时间信息失败");
		
		nWaitTime++;
	}

	CSTCPrint("等待的总时间为: %d, 录制文件限定大小为: %d(M)\n", nWaitTime*nRecTime, sStartPar.m_nMaxSize);

	CSTK_ASSERT_TRUE_FATAL(
			(sRecTimeInfo.m_nStartTime > 0), 
			"录制等待时间短或获取录制时间信息错误");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRecHandle, CSUDI_FALSE)), 
			"停止时移录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("录制完毕，开始录制文件的检查，请注意观察音视频能否正常播放\n");
	CSTCPrint("录制时长为%dms，此时间内正常播放即可\n", nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"录制文件检查不通过");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:测试TS录制、非追加录制、清流
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制且录制的文件能正常播放
//@EXECUTIONFLOW:1、锁定要录制的码流所在频点, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECStart启动录制(TS录制、非追加录制、清流), 期望成功
//@EXECUTIONFLOW:5、等待10S录制时间
//@EXECUTIONFLOW:6、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:8、查看录制的文件已经存在且大小不为0, 期望成功
//@EXECUTIONFLOW:9、查找支持播放的demux及可用的音视频解码器, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIPLAYEROpen创建PVR录制播放模式的播放器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart启动播放, 期望成功
//@EXECUTIONFLOW:13、询问测试者录制文件是否正常播放, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:16、断开上述步骤连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_TSRec_0001(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPlaybackParam_S sPlaybackPar;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle) && hRecHandle != CSUDI_NULL), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TS;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nCount = 4;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sStartPar.m_psPids[2].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sStartPar.m_psPids[2].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[2].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sStartPar.m_psPids[3].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sStartPar.m_psPids[3].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[3].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"停止录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	hRecHandle = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(GetRecFileSize(sStartPar.m_szFileName) > 0), 
			"文件不存在或大小为0");

	CSTCPrint("录制完毕，开始录制文件的检查，请注意观察音视频能否正常播放\n");
	CSTCPrint("录制时长为%dms，此时间内正常播放即可\n", nRecTime);
	CSTKWaitAnyKey();

	memcpy(&asStreamInfo[0], &(sStartPar.m_psPids[0]), sizeof(CSUDIStreamInfo_S));
	memcpy(&asStreamInfo[1], &(sStartPar.m_psPids[1]), sizeof(CSUDIStreamInfo_S));

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"查找支持时移播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放节目信息失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), 
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)), 
			"销毁播放器失败");
	}

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:测试TS录制、非追加录制、二次加密流
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制且录制的文件能正常播放
//@EXECUTIONFLOW:1、锁定要录制的码流所在频点, 期望成功
//@EXECUTIONFLOW:2、查找一个支持录制的demux, 并与tuner相连, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIRECOpen创建一个录制器, 期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、调用CSUDIRECStart启动录制(TS录制、非追加录制、二次加密流), 期望成功
//@EXECUTIONFLOW:5、等待10S录制时间
//@EXECUTIONFLOW:6、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:8、查看录制的文件已经存在且大小不为0, 期望成功
//@EXECUTIONFLOW:9、查找支持播放的demux及可用的音视频解码器, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIPLAYEROpen创建PVR录制播放模式的播放器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart启动播放, 期望成功
//@EXECUTIONFLOW:13、询问测试者录制文件是否正常播放, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:16、断开上述步骤连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_TSRec_0002(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPlaybackParam_S sPlaybackPar;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "查找支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"连接tuner和demux失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle) && hRecHandle != CSUDI_NULL), 
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TS;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_REENCRYPT;
	sStartPar.m_stEncryptParam.m_ucEncryptKey[0] = 'a';
	sStartPar.m_stEncryptParam.m_nEncryptKeyLength = 16;
	sStartPar.m_nCount = 4;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sStartPar.m_psPids[2].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sStartPar.m_psPids[2].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[2].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sStartPar.m_psPids[3].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sStartPar.m_psPids[3].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[3].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"停止录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	hRecHandle = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(GetRecFileSize(sStartPar.m_szFileName) > 0), 
			"文件不存在或大小为0");

	CSTCPrint("录制完毕，开始录制文件的检查，请注意观察音视频能否正常播放\n");
	CSTCPrint("录制时长为%dms，此时间内正常播放即可\n", nRecTime);
	CSTKWaitAnyKey();

	memcpy(&asStreamInfo[0], &(sStartPar.m_psPids[0]), sizeof(CSUDIStreamInfo_S));
	memcpy(&asStreamInfo[1], &(sStartPar.m_psPids[1]), sizeof(CSUDIStreamInfo_S));

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"查找支持时移播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	memcpy(&(sPlaybackPar.m_stEncryptParam), &(sStartPar.m_stEncryptParam), sizeof(CSUDIStreamEncryptParam_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放节目信息失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), 
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)), 
			"销毁播放器失败");
	}

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"断开tuner和demux的连接失败");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSetSpeed, CSUDIPLAYERSeek
//@DESCRIPTION:测试PVR录制文件回放
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:PVR录制文件播放正常
//@EXECUTIONFLOW:1、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:2、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:3、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:7、询问测试者播放是否正常, 期望为是(提示文件大小有限, 此处不能停留太长时间)
//@EXECUTIONFLOW:8、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:9、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:10、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:11、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:12、调用CSUDIPLAYERSeek定位到文件头开始后1S的位置
//@EXECUTIONFLOW:13、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERSetSpeed设置8倍速快进, 期望成功
//@EXECUTIONFLOW:15、调用CSUDIPLAYERGetSpeed获取播放速率, 期望成功且和设置的相等
//@EXECUTIONFLOW:16、询问测试者播放速率是否已变化, 期望为是
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:18、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_Play_0001(void)
{
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_PVR;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPlayerSpeed_E eSpeed = EM_UDIPLAYER_SPEED_MAX;

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"查找支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放节目信息失败");

	CSTCPrint("以下将进行PVR回放测试，测试文件较短，请不要在以下操作停留太长时间，否则可能影响测试结果\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已启动播放，请确认播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"设置播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPlayer, &eSpeed)), 
			"获取播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"获取到的播放速率值与设置的不一致");

	CSTCPrint("已改变播放速率，请确认播放速率是否已变化\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放速率不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), 
			"停止播放失败");

	CSTCPrint("已停止播放，请确认播放是否已停止\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止播放不正常");
	
	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"销毁播放器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSetSpeed, CSUDIPLAYERSeek
//@DESCRIPTION:测试时移文件时移播放(录制内容未达到最大限定大小/录制内容已超过最大限定大小)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:时移播放正常
//@EXECUTIONFLOW:1、查找是否存在一个满足测试需求的时移文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:2、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:3、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:5、询问测试者播放是否正常, 期望为是(提示文件大小有限, 此处不能停留太长时间)
//@EXECUTIONFLOW:6、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:7、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:8、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:9、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:10、调用CSUDIPLAYERSeek定位到当前位置后30S的位置
//@EXECUTIONFLOW:11、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:12、调用CSUDIPLAYERSetSpeed设置8倍速快进, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIPLAYERGetSpeed获取播放速率, 期望成功且和设置的相等
//@EXECUTIONFLOW:14、询问测试者播放速率是否已变化, 期望为是
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_Play_0002(void)
{
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_TSHIFT;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPlayerSpeed_E eSpeed = EM_UDIPLAYER_SPEED_MAX;

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"查找支持时移播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于时移播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放节目信息失败");

	CSTCPrint("以下将进行时移播放测试，测试文件较短，请不要在以下操作停留太长时间，否则可能影响测试结果\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已启动播放，请确认播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"设置播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPlayer, &eSpeed)), 
			"获取播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"获取到的播放速率值与设置的不一致");

	CSTCPrint("已改变播放速率，请确认播放速率是否已变化\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放速率不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), 
			"停止播放失败");

	CSTCPrint("已停止播放，请确认播放是否已停止\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止播放不正常");
	
	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"销毁播放器失败");
	}
	
	return CSUDI_TRUE;
}

static CSUDI_BOOL s_bEndofStream = CSUDI_FALSE;
static CSUDI_BOOL s_bBeginofStream = CSUDI_FALSE;
static void iPlayerCallback(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void * pvUserData)
{
	switch(eEvent)
	{
		case EM_UDIPLAYER_END_OF_STREAM:
		{
			CSTCPrint("收到播放至文件末尾的消息\n");
			s_bEndofStream = CSUDI_TRUE;
			break;
		}

		case EM_UDIPLAYER_BEGIN_OF_STREAM:
		{
			CSTCPrint("收到播放至文件开头的消息\n");
			s_bBeginofStream = CSUDI_TRUE;
			break;
		}

		default:
			break;
	}
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERAddPlayerCallback
//@DESCRIPTION:测试播放PVR录制文件能否监听到播放至文件末尾、播放至文件开头的消息
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:2、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够监听到消息
//@EXECUTIONFLOW:1、查找一个满足测试需求的PVR录制文件(录制3分钟), 如果不存在重新录制
//@EXECUTIONFLOW:2、查找一个符合播放要求的音频解码器, 期望成功
//@EXECUTIONFLOW:3、查找一个符合播放要求的视频解码器, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYERAddPlayerCallback注册播放至文件末尾类型的回调, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERAddPlayerCallback注册播放至文件开头类型的回调, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置播放参数, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:9、等待播放1s
//@EXECUTIONFLOW:10、调用CSUDIPLAYERSetSpeed设置32倍速快退, 期望成功
//@EXECUTIONFLOW:11、检查是否在2S内能收到播放至文件开头的消息, 期望收到
//@EXECUTIONFLOW:12、调用CSUDIPLAYERSetSpeed设置常速播放, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIPLAYERSeek定位到文件尾前3S的位置播放, 期望成功
//@EXECUTIONFLOW:14、检查是否在4S内能收到播放至文件末尾的消息, 期望收到
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:16、调用CSUDIPLAYERDelPlayerCallback移除播放至文件末尾消息回调, 期望成功
//@EXECUTIONFLOW:17、调用CSUDIPLAYERDelPlayerCallback移除播放至文件开头消息回调, 期望成功
//@EXECUTIONFLOW:18、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_Play_0003(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	int nUserdata = 0x1234;
	CSUDI_BOOL bCallbackAdded1 = CSUDI_FALSE;
	CSUDI_BOOL bCallbackAdded2 = CSUDI_FALSE;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	int nWaitTime = 0;

	s_bEndofStream = CSUDI_FALSE;
	s_bBeginofStream = CSUDI_FALSE;

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));
	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");
	
	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer)),
			"创建播放器失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_END_OF_STREAM, (void*)&nUserdata)),
			"注册播放至文件末尾类型的回调失败");
	bCallbackAdded1 = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_BEGIN_OF_STREAM, (void*)&nUserdata)),
			"注册播放至文件开头类型的回调失败");
	bCallbackAdded2 = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放参数失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("等待播放1S\n");
	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTREWIND_32)),
			"设置播放速率失败");

	while (!s_bBeginofStream && nWaitTime < 4)
	{
		CSUDIOSThreadSleep(500);
		nWaitTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL(
			(s_bBeginofStream),
			"未收到播放至文件开头的消息");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_NORMAL)),
			"设置播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, -3, EM_UDIPLAYER_POSITION_FROM_END)),
			"定位到文件结尾前3S位置失败");

	nWaitTime = 0;
	while (!s_bEndofStream && nWaitTime < 8)
	{
		CSUDIOSThreadSleep(500);
		nWaitTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL(
			(s_bEndofStream),
			"未收到播放至文件末尾的消息");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (bCallbackAdded1)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_END_OF_STREAM, (void*)&nUserdata)),
				"移除播放至文件末尾类型的回调失败");
	}

	if (bCallbackAdded2)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_BEGIN_OF_STREAM, (void*)&nUserdata)),
				"移除播放至文件开头类型的回调失败");
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"销毁播放器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERAddPlayerCallback
//@DESCRIPTION:测试播放时移文件能否监听到播放至文件末尾、播放至文件开头的消息(录制内容未达到最大限定大小/录制内容已超过最大限定大小)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:2、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够监听到消息
//@EXECUTIONFLOW:1、查找一个满足测试需求的时移文件(录制3分钟), 如果不存在重新录制
//@EXECUTIONFLOW:2、查找一个符合播放要求的音频解码器, 期望成功
//@EXECUTIONFLOW:3、查找一个符合播放要求的视频解码器, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYERAddPlayerCallback注册播放至文件末尾类型的回调, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERAddPlayerCallback注册播放至文件开头类型的回调, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置播放参数, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:9、等待播放1s
//@EXECUTIONFLOW:10、调用CSUDIPLAYERSetSpeed设置32倍速快退, 期望成功
//@EXECUTIONFLOW:11、检查是否在2S内能收到播放至文件开头的消息, 期望收到
//@EXECUTIONFLOW:12、调用CSUDIPLAYERSetSpeed设置常速播放, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIPLAYERSeek定位到文件尾前3S的位置播放, 期望成功
//@EXECUTIONFLOW:14、检查是否在4S内能收到播放至文件末尾的消息, 期望收到
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:16、调用CSUDIPLAYERDelPlayerCallback移除播放至文件末尾消息回调, 期望成功
//@EXECUTIONFLOW:17、调用CSUDIPLAYERDelPlayerCallback移除播放至文件开头消息回调, 期望成功
//@EXECUTIONFLOW:18、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_Play_0004(void)
{
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	int nUserdata = 0x1234;
	CSUDI_BOOL bCallbackAdded1 = CSUDI_FALSE;
	CSUDI_BOOL bCallbackAdded2 = CSUDI_FALSE;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	int nWaitTime = 0;

	s_bEndofStream = CSUDI_FALSE;
	s_bBeginofStream = CSUDI_FALSE;

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));
	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");
	
	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer)),
			"创建播放器失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_END_OF_STREAM, (void*)&nUserdata)),
			"注册播放至文件末尾类型的回调失败");
	bCallbackAdded1 = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_BEGIN_OF_STREAM, (void*)&nUserdata)),
			"注册播放至文件开头类型的回调失败");
	bCallbackAdded2 = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放参数失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("等待播放1S\n");
	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTREWIND_32)),
			"设置播放速率失败");

	while (!s_bBeginofStream && nWaitTime < 4)
	{
		CSUDIOSThreadSleep(500);
		nWaitTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL(
			(s_bBeginofStream),
			"未收到播放至文件开头的消息");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_NORMAL)),
			"设置播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, -3, EM_UDIPLAYER_POSITION_FROM_END)),
			"定位到文件结尾前3S位置失败");

	nWaitTime = 0;
	while (!s_bEndofStream && nWaitTime < 8)
	{
		CSUDIOSThreadSleep(500);
		nWaitTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL(
			(s_bEndofStream),
			"未收到播放至文件末尾的消息");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (bCallbackAdded1)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_END_OF_STREAM, (void*)&nUserdata)),
				"移除播放至文件末尾类型的回调失败");
	}

	if (bCallbackAdded2)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_BEGIN_OF_STREAM, (void*)&nUserdata)),
				"移除播放至文件开头类型的回调失败");
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"销毁播放器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERGetCurPosInSec
//@DESCRIPTION:测试获取PVR录制文件播放当前播放位置的绝对时长
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:2、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:正确获取PVR录制文件播放当前播放位置的绝对时长
//@EXECUTIONFLOW:1、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:2、查找一个支持PVR播放的demux, 期望成功
//@EXECUTIONFLOW:3、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:7、等待播放60s
//@EXECUTIONFLOW:8、调用CSUDIPLAYERGetCurPosInSec获取当前播放位置的绝对时长, 期望成功
//@EXECUTIONFLOW:9、检查获取到的当前播放位置的绝对时长是否在60S左右(前后允许误差各5S), 期望为是
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_Play_0005(void)
{
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_PVR;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int nPosInSec = 0;
	int nPlayTime = (60*1000);

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"查找支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer)),
			"创建播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放参数失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("等到播放%dms(播放时间太短的话获取播放绝对时长误差率高，允许一定误差)\n", nPlayTime);
	CSUDIOSThreadSleep(nPlayTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetCurPosInSec(hPlayer, &nPosInSec)),
			"获取当前播放的绝对时长失败");

	CSTCPrint("获取到的当前播放的绝对时长为 %d\n", nPosInSec);
	CSTK_ASSERT_TRUE_FATAL((nPosInSec >= (nPlayTime/1000-MAX_ERROR_TIME) && nPosInSec <= (nPlayTime/1000+MAX_ERROR_TIME)), "获取到的当前播放的绝对时长不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"销毁播放器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERGetCurPosInSec
//@DESCRIPTION:测试获取时移文件播放当前播放位置的绝对时长(录制内容未达到最大限定大小/录制内容已超过最大限定大小)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:2、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:正确获取到时移文件播放当前播放位置的绝对时长
//@EXECUTIONFLOW:1、查找是否存在一个满足测试需求的时移文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:2、查找一个支持PVR播放的demux, 期望成功
//@EXECUTIONFLOW:3、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:7、等待播放60s
//@EXECUTIONFLOW:8、调用CSUDIPLAYERGetCurPosInSec获取当前播放位置的绝对时长, 期望成功
//@EXECUTIONFLOW:9、检查获取到的当前播放位置的绝对时长是否大于等于60S(前后允许误差各5S), 期望为是
//@EXECUTIONFLOW:10、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_Play_0006(void)
{
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_TSHIFT;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	int nPosInSec = 0;
	int nPlayTime = (60*1000);

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"查找支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer)),
			"创建播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放参数失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("等到播放%d ms(播放时间太短的话获取播放绝对时长误差率高，允许一定误差)\n", nPlayTime);
	CSUDIOSThreadSleep(nPlayTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetCurPosInSec(hPlayer, &nPosInSec)),
			"获取当前播放的绝对时长失败");

	CSTCPrint("获取到的当前播放的绝对时长为 %d\n", nPosInSec);
	CSTK_ASSERT_TRUE_FATAL((nPosInSec >= (nPlayTime/1000-MAX_ERROR_TIME) && nPosInSec <= (nPlayTime/1000+MAX_ERROR_TIME)), "获取到的当前播放的绝对时长不正确");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"销毁录制器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSeek
//@DESCRIPTION:测试直播和PVR文件回放之前切换
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:2、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:都能够正常播放
//@EXECUTIONFLOW:1、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:2、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:3、查找一个支持录制文件播放的demux, 期望成功
//@EXECUTIONFLOW:4、查找可用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建用于直播的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置直播节目信息, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart启动直播, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:10、询问测试者播放是否已停止, 期望为是
//@EXECUTIONFLOW:11、调用CSUDIPLAYERClose销毁直播的播放器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:15、询问测试者播放是否正常, 期望为是(提示文件大小有限, 此处不能停留太长时间)
//@EXECUTIONFLOW:16、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:17、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:18、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:19、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIPLAYERSeek定位到文件头开始后1S的位置
//@EXECUTIONFLOW:21、调用CSUDIPLAYERGetSpeed获取播放速率, 期望成功且和设置的相等
//@EXECUTIONFLOW:22、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIPLAYERSetSpeed设置8倍速快进, 期望成功
//@EXECUTIONFLOW:24、询问测试者播放速率是否已变化, 期望为是
//@EXECUTIONFLOW:25、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:26、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_Play_0007(void)
{
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_PVR;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDI_HANDLE hLivePlayer = CSUDI_NULL;
	CSUDI_HANDLE hPvrPlayer = CSUDI_NULL;
	int nDmxForLive = -1;
	int nDmxForPvr = -1;
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIPlayerSpeed_E eSpeed = EM_UDIPLAYER_SPEED_MAX;

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");

	nDmxForLive = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForLive >= 0), 
			"查找支持直播的demux失败");

	nDmxForPvr = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForPvr >= 0), 
			"查找支持PVR播放的demux失败");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sPlayerChnl.m_nDemux = nDmxForLive;
	hLivePlayer = LivePlayStart(&sPlayerChnl, &asStreamInfo[0], &asStreamInfo[1]);
	CSTK_ASSERT_TRUE_FATAL(
			(hLivePlayer != CSUDI_NULL), 
			"开始直播失败");

	CSTCPrint("已开始直播，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"直播不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == LivePlayStop(hLivePlayer, nDmxForLive)), 
			"开始直播失败");
	hLivePlayer = CSUDI_NULL;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	sPlayerChnl.m_nDemux = nDmxForPvr;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPvrPlayer) && hPvrPlayer != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPvrPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放节目信息失败");

	CSTCPrint("以下将进行PVR回放测试，测试文件较短，请不要在以下操作停留太长时间，否则可能影响测试结果\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPvrPlayer)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已启动播放，请确认播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPvrPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPvrPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPvrPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPvrPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"设置播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPvrPlayer, &eSpeed)), 
			"获取播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"获取到的播放速率值与设置的不一致");

	CSTCPrint("已改变播放速率，请确认播放速率是否已变化\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放速率不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPvrPlayer)), 
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放，请确认播放是否已停止\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止播放不正常");
	
	CSTK_FATAL_POINT;

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
				"销毁直播播放器失败");
	}

	if (hPvrPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPvrPlayer)),
				"销毁PVR播放器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSeek
//@DESCRIPTION:测试直播和时移播放之前切换
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:2、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:都能够正常播放
//@EXECUTIONFLOW:1、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:2、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:3、查找一个支持录制文件播放的demux, 期望成功
//@EXECUTIONFLOW:4、查找可用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建用于直播的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置直播节目信息, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart启动直播, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:9、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:10、询问测试者播放是否已停止, 期望为是
//@EXECUTIONFLOW:11、调用CSUDIPLAYERClose销毁直播的播放器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:15、询问测试者播放是否正常, 期望为是(提示文件大小有限, 此处不能停留太长时间)
//@EXECUTIONFLOW:16、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:17、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:18、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:19、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIPLAYERSeek定位到文件当前位置后30S的位置
//@EXECUTIONFLOW:21、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:22、调用CSUDIPLAYERSetSpeed设置8倍速快进, 期望成功
//@EXECUTIONFLOW:23、调用CSUDIPLAYERGetSpeed获取播放速率, 期望成功且和设置的相等
//@EXECUTIONFLOW:24、询问测试者播放速率是否已变化, 期望为是
//@EXECUTIONFLOW:25、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:26、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_Play_0008(void)
{
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	CSUDIPLAYERType_E ePlayerType = EM_UDIPLAYER_TSHIFT;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDI_HANDLE hLivePlayer = CSUDI_NULL;
	CSUDI_HANDLE hPvrPlayer = CSUDI_NULL;
	int nDmxForLive = -1;
	int nDmxForPvr = -1;
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIPlayerSpeed_E eSpeed = EM_UDIPLAYER_SPEED_MAX;

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");

	nDmxForLive = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForLive >= 0), 
			"查找支持直播的demux失败");

	nDmxForPvr = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForPvr >= 0), 
			"查找支持PVR播放的demux失败");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));
	
	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的视频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	sPlayerChnl.m_nDemux = nDmxForLive;
	hLivePlayer = LivePlayStart(&sPlayerChnl, &asStreamInfo[0], &asStreamInfo[1]);
	CSTK_ASSERT_TRUE_FATAL(
			(hLivePlayer != CSUDI_NULL), 
			"开始直播失败");

	CSTCPrint("已开始直播，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"直播不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == LivePlayStop(hLivePlayer, nDmxForLive)), 
			"开始直播失败");
	hLivePlayer = CSUDI_NULL;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	sPlayerChnl.m_nDemux = nDmxForPvr;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPvrPlayer) && hPvrPlayer != CSUDI_NULL),
			"创建用于时移播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPvrPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放节目信息失败");

	CSTCPrint("以下将进行时移播放测试，测试文件较短，请不要在以下操作停留太长时间，否则可能影响测试结果\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPvrPlayer)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已启动播放，请确认播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPvrPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPvrPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPvrPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPvrPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"设置播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPvrPlayer, &eSpeed)), 
			"获取播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"获取到的播放速率值与设置的不一致");

	CSTCPrint("已改变播放速率，请确认播放速率是否已变化\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放速率不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPvrPlayer)), 
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放，请确认播放是否已停止\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止播放不正常");
	
	CSTK_FATAL_POINT;

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
				"销毁直播播放器失败");
	}

	if (hPvrPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPvrPlayer)),
				"销毁时移播放器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSeek
//@DESCRIPTION:测试PVR文件回放和时移播放之前切换
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:2、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:都能够正常播放
//@EXECUTIONFLOW:1、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:1、查找是否存在一个满足测试需求的时移文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:3、查找一个支持录制文件播放的demux, 期望成功
//@EXECUTIONFLOW:4、查找可用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建用于PVR回放的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置直播节目信息, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart启动PVR回放, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:9、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:10、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:11、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:12、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:13、调用CSUDIPLAYERSeek定位到文件头开始后1S的位置
//@EXECUTIONFLOW:14、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:15、调用CSUDIPLAYERSetSpeed设置8倍速快进, 期望成功
//@EXECUTIONFLOW:16、询问测试者播放速率是否已变化, 期望为是
//@EXECUTIONFLOW:17、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:18、询问测试者播放是否已停止, 期望为是
//@EXECUTIONFLOW:19、调用CSUDIPLAYERClose销毁直播的播放器, 期望成功
//@EXECUTIONFLOW:20、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:21、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:22、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:23、询问测试者播放是否正常, 期望为是(提示文件大小有限, 此处不能停留太长时间)
//@EXECUTIONFLOW:24、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:25、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:26、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:27、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:28、调用CSUDIPLAYERSeek定位到当前位置后30S的位置
//@EXECUTIONFLOW:29、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:30、调用CSUDIPLAYERSetSpeed设置8倍速快进, 期望成功
//@EXECUTIONFLOW:31、调用CSUDIPLAYERGetSpeed获取播放速率, 期望成功且和设置的相等
//@EXECUTIONFLOW:32、询问测试者播放速率是否已变化, 期望为是
//@EXECUTIONFLOW:33、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:34、调用CSUDIPLAYERClose销毁播放器, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_Play_0009(void)
{
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIPlaybackParam_S sPlaybackParTShift;
	CSUDIPlaybackParam_S sPlaybackParPvr;
	CSUDI_HANDLE hTShiftPlayer = CSUDI_NULL;
	CSUDI_HANDLE hPvrPlayer = CSUDI_NULL;
	int nDmxForPvr = -1;
	CSUDIPlayerSpeed_E eSpeed = EM_UDIPLAYER_SPEED_MAX;

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackParPvr, 0, sizeof(sPlaybackParPvr));
	sprintf(sPlaybackParPvr.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackParPvr.m_nSecondPos = 0;
	sPlaybackParPvr.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	memset(&sPlaybackParTShift, 0, sizeof(sPlaybackParTShift));
	sprintf(sPlaybackParTShift.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackParTShift.m_nSecondPos = 0;
	sPlaybackParTShift.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(EM_UDIRECORD_TYPE_CHNL, sPlaybackParPvr.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的PVR录制文件");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(EM_UDIRECORD_TYPE_TSHIFT, sPlaybackParTShift.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的时移文件");
	
	nDmxForPvr = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForPvr >= 0), 
			"查找支持PVR播放的demux失败");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"查找可用的视频解码器失败");

	sPlayerChnl.m_nDemux = nDmxForPvr;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPvrPlayer) && hPvrPlayer != CSUDI_NULL),
			"创建用于PVR回放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPvrPlayer, asStreamInfo, 2, &sPlaybackParPvr)),
			"设置播放节目信息失败");

	CSTCPrint("以下将进行PVR回放测试，测试文件较短，请不要在以下操作停留太长时间，否则可能影响测试结果\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPvrPlayer)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已启动播放，请确认播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPvrPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPvrPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPvrPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPvrPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"设置播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPvrPlayer, &eSpeed)), 
			"获取播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"获取到的播放速率值与设置的不一致");

	CSTCPrint("已改变播放速率，请确认播放速率是否已变化\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放速率不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPvrPlayer)), 
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放，请确认播放是否已停止\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止播放不正常");

	if (hPvrPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPvrPlayer)),
				"销毁PVR回放播放器失败");
		hPvrPlayer = CSUDI_NULL;
	}
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hTShiftPlayer) && hTShiftPlayer != CSUDI_NULL),
			"创建用于时移播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hTShiftPlayer, asStreamInfo, 2, &sPlaybackParTShift)),
			"设置播放节目信息失败");

	CSTCPrint("以下将进行时移播放测试，测试文件较短，请不要在以下操作停留太长时间，否则可能影响测试结果\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hTShiftPlayer)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已启动播放，请确认播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hTShiftPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hTShiftPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hTShiftPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hTShiftPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"设置播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hTShiftPlayer, &eSpeed)), 
			"获取播放速率失败");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"获取到的播放速率值与设置的不一致");

	CSTCPrint("已改变播放速率，请确认播放速率是否已变化\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放速率不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hTShiftPlayer)), 
			"停止播放失败");

	CSTCPrint("已停止播放，请确认播放是否已停止\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止播放不正常");
	
	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hTShiftPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hTShiftPlayer)),
				"销毁时移播放器失败");
	}

	if (hPvrPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPvrPlayer)),
				"销毁PVR回放播放器失败");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试所有支持录制的demux都能分别完成录制
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:所有支持录制的demux能正常完成录制且录制的文件能正常播放
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找所有支持录制的demux, 期望成功
//@EXECUTIONFLOW:3、将一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:4、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIRECStart启动录制(节目录制、非追加录制、清流), 期望成功
//@EXECUTIONFLOW:6、等待录制10S
//@EXECUTIONFLOW:7、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:9、断开demux与tuner的连接
//@EXECUTIONFLOW:10、判断录制的文件是否已存在且大小不为0, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIPLAYEROpen创建PVR录制播放模式的播放器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYERStart启动录制文件的播放, 期望成功
//@EXECUTIONFLOW:13、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERStop停止播放
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:16、断开支持录制的demux与tuner的连接, 期望成功
//@EXECUTIONFLOW:17、对所有支持录制的demux重复步骤3~16
CSUDI_BOOL CSTC_RECORD_IT_0001(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;
	int i = 0;
	CSUDIRecChnl_S sRecChnl;
	CSUDI_HANDLE ahRecHandle[DEMUX_COUNT] = {CSUDI_NULL};
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt) && nActualCnt>0),
			"查找所有支持录制的demux失败");

	for (i=0; i<nActualCnt; i++)
	{
		CSTCPrint("测试第%d个demux, demuxId=%d\n", i+1, anDemuxId[i]);

		memset(&sStartPar, 0, sizeof(sStartPar));	
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[i])),
				"demux和tuner连接失败");

		sRecChnl.m_nDemux = anDemuxId[i];
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRecHandle[i]) && ahRecHandle[i] != CSUDI_NULL),
				"创建录制器失败");	

		sStartPar.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
		sStartPar.m_bAppend = CSUDI_FALSE;
		sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
		sStartPar.m_nCount = 2;
		sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
		sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
		sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
		sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
		sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
		sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
		sprintf(sStartPar.m_szFileName, "%s/%s_%d", g_acFilePath, __FUNCTION__, i+1);
		CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(ahRecHandle[i], &sStartPar)), 
			"启动录制失败");

		CSTCPrint("等待录制%dms\n", nRecTime);
		CSUDIOSThreadSleep(nRecTime);

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECStop(ahRecHandle[i])),
				"停止录制失败");	

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(ahRecHandle[i])),
				"销毁录制器失败");
		ahRecHandle[i] = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[i])),
				"断开demux和tuner的连接失败");

		CSTCPrint("录制完毕，开始录制文件的检查，请注意观察音视频能否正常播放\n");
		CSTCPrint("录制时长为%dms，此时间内正常播放即可\n", nRecTime);
		CSTKWaitAnyKey();

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == CheckRecResult(sStartPar)),
				"录制文件检查不通过");

		ClearFile(sStartPar.m_szFileName);
	}

	CSTK_FATAL_POINT;

	for (i=0; i<nActualCnt; i++)
	{
		if (ahRecHandle[i] != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(ahRecHandle[i])),
				"销毁录制器失败");
			ahRecHandle[i] = CSUDI_NULL;

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[i])),
					"断开demux和tuner的连接失败");
		}
	}

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试同时启动两路录制的情况(不同录制模式)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制且录制的文件能够正常播放
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、针对支持录制的demux1调用CSUDIDEMUXConnectTuner与tuner连接, 期望成功
//@EXECUTIONFLOW:4、针对demux1调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:5、针对支持录制的demux2调用CSUDIDEMUXConnectTuner与tuner连接, 期望成功
//@EXECUTIONFLOW:6、针对demux2调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:7、针对所有已创建的录制器调用CSUDIRECStart启动录制(节目录制/时移录制、非追加录制、清流), 录制文件保存名称不同, 期望全部成功
//@EXECUTIONFLOW:8、所有录制等待录制10S
//@EXECUTIONFLOW:9、针对所有录制器调用CSUDIRECStop停止录制, 期望全部成功
//@EXECUTIONFLOW:10、销毁所有录制器, 期望全部成功
//@EXECUTIONFLOW:11、检查一个录制的文件是否已存在且大小不为0, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIPLAYEROpen创建PVR录制播放模式的播放器, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStart启动一个录制文件的播放, 期望成功
//@EXECUTIONFLOW:14、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:15、调用CSUDIPLAYERStop停止播放
//@EXECUTIONFLOW:16、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:17、对所有已录制的文件执行步骤11~17
//@EXECUTIONFLOW:18、断开上述所有demux与tuner的连接, 期望全部成功
CSUDI_BOOL CSTC_RECORD_IT_0002(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;
	int i = 0;
	CSUDI_HANDLE ahRec[2] = {CSUDI_NULL};
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S asStartPar[2];
	int nRecTime = RECORD_TIME_SHORT;
	int nRecCount = 2;

	memset(&asStartPar[0], 0, sizeof(CSUDIRecStartParam_S));	
	memset(&asStartPar[1], 0, sizeof(CSUDIRecStartParam_S));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[0])),
			"第一个支持录制的demux和tuner连接失败");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRec[0]) && ahRec[0] != CSUDI_NULL),
			"创建录制器1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[1])),
			"第二个支持录制的demux和tuner连接失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRec[1]) && ahRec[1] != CSUDI_NULL),
			"创建录制器2失败");

	asStartPar[0].m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	asStartPar[0].m_bAppend = CSUDI_FALSE;
	asStartPar[0].m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	asStartPar[0].m_nCount = 2;
	asStartPar[0].m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStartPar[0].m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStartPar[0].m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStartPar[0].m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStartPar[0].m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStartPar[0].m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(asStartPar[0].m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);	

	memcpy(&asStartPar[1], &asStartPar[0], sizeof(CSUDIRecStartParam_S));
	asStartPar[1].m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	asStartPar[1].m_nMaxSize = TSHIFT_MAX_SIZE;
	sprintf(asStartPar[1].m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("启动第%d个录制器，录制模式为: %d， 文件名为: %s\n", i+1, asStartPar[i].m_eRecType, asStartPar[i].m_szFileName);

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECStart(ahRec[i], &(asStartPar[i]))),
					"启动录制失败");
		}
	}

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("停止第%d个录制器\n", i+1);

			if (asStartPar[i].m_eRecType == EM_UDIRECORD_TYPE_TSHIFT)
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIRECStopTshift(ahRec[i], CSUDI_FALSE)),
						"停止录制失败");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIRECStop(ahRec[i])),
						"停止录制失败");
			}
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("销毁第%d个录制器\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRec[i])),
					"销毁录制器失败");
			ahRec[i] = CSUDI_NULL;
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		CSTCPrint("检查第%d个录制结果\n", i+1);
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(asStartPar[i])),
			"录制文件检查不通过");
	}

	CSTK_FATAL_POINT;

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{			
			CSTCPrint("销毁第%d个录制器\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRec[i])),
					"销毁录制器失败");
		}
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第二个支持录制的demux和tuner的连接失败");

	for (i=0; i<nRecCount; i++)
	{
		ClearFile(asStartPar[i].m_szFileName);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试一路录制启动一段时间后再启动另一路录制的情况
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:能够正常录制且录制的文件能够正常播放
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、针对支持录制的demux1调用CSUDIDEMUXConnectTuner与tuner连接, 期望成功
//@EXECUTIONFLOW:4、针对demux1调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:5、针对支持录制的demux2调用CSUDIDEMUXConnectTuner与tuner连接, 期望全部成功
//@EXECUTIONFLOW:6、针对demux2调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIRECStart启动录制器1的一路录制(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:8、等待录制10S
//@EXECUTIONFLOW:9、调用CSUDIRECStart启动录制器2的一路录制(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:10、等待录制10S
//@EXECUTIONFLOW:11、调用CSUDIRECStop分别停止上述两路录制, 期望全部成功
//@EXECUTIONFLOW:12、调用CSUDIRECClose销毁上述两个录制器, 期望全部成功
//@EXECUTIONFLOW:13、检查一个录制的文件是否已存在且大小不为0, 期望成功
//@EXECUTIONFLOW:14、查找一个支持录制文件播放的demux, 期望成功
//@EXECUTIONFLOW:15、调用CSUDIPLAYEROpen创建PVR录制播放模式的播放器, 期望成功
//@EXECUTIONFLOW:16、调用CSUDIPLAYERStart启动一个录制文件的播放, 期望成功
//@EXECUTIONFLOW:17、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:18、调用CSUDIPLAYERStop停止播放
//@EXECUTIONFLOW:19、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:20、对两个已录制的文件分别执行步骤13~20
//@EXECUTIONFLOW:24、断开上述所有demux与tuner的连接, 期望全部成功
CSUDI_BOOL CSTC_RECORD_IT_0003(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;
	int i = 0;
	CSUDI_HANDLE ahRec[2] = {CSUDI_NULL};
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S asStartPar[2];
	int nRecTime = SHORT_REC_TIME;
	int nRecCount = 2;
	
	memset(&asStartPar[0], 0, sizeof(CSUDIRecStartParam_S));
	memset(&asStartPar[1], 0, sizeof(CSUDIRecStartParam_S));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"锁频失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[0])),
			"第一个支持录制的demux和tuner连接失败");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRec[0]) && ahRec[0] != CSUDI_NULL),
			"创建录制器1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[1])),
			"第二个支持录制的demux和tuner连接失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRec[1]) && ahRec[1] != CSUDI_NULL),
			"创建录制器2失败");
		
	asStartPar[0].m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	asStartPar[0].m_bAppend = CSUDI_FALSE;
	asStartPar[0].m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	asStartPar[0].m_nCount = 2;
	asStartPar[0].m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStartPar[0].m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStartPar[0].m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStartPar[0].m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStartPar[0].m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStartPar[0].m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(asStartPar[0].m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名1: %s\n", asStartPar[0].m_szFileName);
	
	memcpy(&asStartPar[1], &asStartPar[0], sizeof(CSUDIRecStartParam_S));
	asStartPar[1].m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	asStartPar[1].m_nMaxSize = TSHIFT_MAX_SIZE;
	sprintf(asStartPar[1].m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名2: %s\n", asStartPar[1].m_szFileName);

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("启动第%d个录制器\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECStart(ahRec[i], &asStartPar[i])),
					"启动录制失败");

			CSTCPrint("等待录制%dms\n", nRecTime);
			CSUDIOSThreadSleep(nRecTime);
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("停止第%d个录制器\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECStop(ahRec[i])),
					"停止录制失败");
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("销毁第%d个录制器\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRec[i])),
					"销毁录制器失败");
			ahRec[i] = CSUDI_NULL;
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		CSTCPrint("检查第%d个录制结果\n", i+1);
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(asStartPar[i])),
			"录制文件检查不通过");
	}

	CSTK_FATAL_POINT;

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{			
			CSTCPrint("销毁第%d个录制器\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRec[i])),
					"销毁录制器失败");
		}
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第二个支持录制的demux和tuner的连接失败");

	for (i=0; i<nRecCount; i++)
	{
		ClearFile(asStartPar[i].m_szFileName);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试直播节目A的同时进行一路普通录制(节目A/节目B)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:播放和录制互不影响
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:3、将支持直播的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:4、查找符合直播需求的音视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建直播模式的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置节目信息, 期望返回成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYStart启动直播节目A, 期望返回成功
//@EXECUTIONFLOW:7、询问测试者当前未启动录制、直播节目A是否正常, 期望为是
//@EXECUTIONFLOW:8、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:9、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECStart启动录制节目A(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:12、询问测试者已启动普通录制节目A、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:13、等待录制10S
//@EXECUTIONFLOW:14、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:15、询问测试者已停止录制、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:16、调用CSUDIRECStart启动录制节目B(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:17、询问测试者已启动普通录制节目B、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:18、等待录制10S
//@EXECUTIONFLOW:19、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:20、询问测试者已停止录制、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:21、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:22、调用CSUDIPLAYERStop停止直播, 期望成功
//@EXECUTIONFLOW:23、询问测试者直播是否已停止, 期望为是
//@EXECUTIONFLOW:24、调用CSUDIPLAYERClose销毁直播播放器, 期望成功
//@EXECUTIONFLOW:25、查找一个支持录制文件播放的demux, 期望成功
//@EXECUTIONFLOW:26、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:27、调用CSUDIPLAYERStart启动录制文件A的播放, 期望返回成功
//@EXECUTIONFLOW:28、询问测试者录制文件A是否正常播放, 期望为是
//@EXECUTIONFLOW:29、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:30、调用CSUDIPLAYERStart启动录制文件B的播放, 期望返回成功
//@EXECUTIONFLOW:31、询问测试者录制文件B是否正常播放, 期望为是
//@EXECUTIONFLOW:32、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:33、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:34、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0004(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int nRecTime = RECORD_TIME_SHORT;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持直播的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"支持直播的demux和tuner连接失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"设置节目信息失败");

	CSTCPrint("开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察直播是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下直播播放异常");

	CSTCPrint("等待录制%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("即将停止录制节目:%s，请注意观察直播是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察直播是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下直播播放异常");

	CSTCPrint("等待录制%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("即将停止录制节目:%s，请注意观察直播是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于直播的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;	

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"断开用于直播的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开用于录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试直播节目A的同时两路普通录制(节目A和节目B)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:播放和录制互不影响
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:4、将支持直播的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:5、查找符合直播需求的音视频解码器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen创建直播模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYStart启动直播节目A, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者当前未启动录制、直播节目A是否正常, 期望为是
//@EXECUTIONFLOW:9、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:11、将第二个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECStart启动录制节目A(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:14、询问测试者已启动普通录制节目A、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动普通录制节目B、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、等待录制10S
//@EXECUTIONFLOW:18、调用CSUDIRECStop停止上述启动的两路录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止录制、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIRECClose销毁上述两个录制器, 期望成功
//@EXECUTIONFLOW:21、调用CSUDIPLAYERStop停止直播, 期望成功
//@EXECUTIONFLOW:22、询问测试者直播是否已停止, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIPLAYERClose销毁直播播放器, 期望成功
//@EXECUTIONFLOW:24、检查录制的文件存在且大小不为0, 期望成功
//@EXECUTIONFLOW:25、查找一个支持录制文件播放的demux, 期望成功
//@EXECUTIONFLOW:26、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:27、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:28、询问测试者录制文件是否正常播放, 期望为是
//@EXECUTIONFLOW:29、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:30、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:31、对两个录制文件分别执行24~30
//@EXECUTIONFLOW:32、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0005(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec1 = CSUDI_NULL;
	CSUDI_HANDLE hRec2 = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int nRecTime = RECORD_TIME_SHORT;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持直播的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"支持直播的demux和tuner连接失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"设置节目信息失败");

	CSTCPrint("开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"创建录制器1失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第二个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"创建录制器2失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目1:%s 和节目2:%s，请注意观察直播是否受影响\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"启动录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"启动录制1失败");

	CSTCPrint("当前已启动两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下直播播放异常");

	CSTCPrint("等待录制%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("即将停止录制两路节目，请注意观察直播是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"停止录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec2)),
			"停止录制2失败");

	CSTCPrint("当前已停止两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器2失败");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于直播的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器1失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"断开用于直播的demux和tuner的连接失败");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第一个支持录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试直播节目A的同时进行一路时移录制(节目A/节目B)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:播放和录制互不影响
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:3、将支持直播的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:4、查找符合直播需求的音视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建直播模式的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYStart启动直播节目A, 期望返回成功
//@EXECUTIONFLOW:7、询问测试者当前未启动录制、直播节目A是否正常, 期望为是
//@EXECUTIONFLOW:8、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:9、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECStart启动录制节目A(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:12、询问测试者已启动时移录制节目A、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:13、等待录制10S
//@EXECUTIONFLOW:14、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动时移录制节目B、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、等待录制10S
//@EXECUTIONFLOW:18、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止录制、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:21、检查录制的两个文件存在且大小不为0, 期望成功
//@EXECUTIONFLOW:22、调用CSUDIPLAYERStop停止直播, 期望成功
//@EXECUTIONFLOW:23、询问测试者直播是否已停止, 期望为是
//@EXECUTIONFLOW:24、调用CSUDIPLAYERClose销毁直播播放器, 期望成功
//@EXECUTIONFLOW:25、查找一个支持时移文件播放的demux, 期望成功
//@EXECUTIONFLOW:26、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:27、调用CSUDIPLAYERStart启动时移文件A的播放, 期望返回成功
//@EXECUTIONFLOW:28、询问测试者时移文件A是否正常播放, 期望为是
//@EXECUTIONFLOW:29、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:30、调用CSUDIPLAYERStartB启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:31、询问测试者时移文件B是否正常播放, 期望为是
//@EXECUTIONFLOW:32、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:33、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:34、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0006(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int nRecTime = RECORD_TIME_SHORT;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持直播的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"支持直播的demux和tuner连接失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"设置节目信息失败");

	CSTCPrint("开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察直播是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下直播播放异常");

	CSTCPrint("等待录制%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("即将停止录制节目:%s，请注意观察直播是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察直播是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下直播播放异常");

	CSTCPrint("等待录制%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("即将停止录制节目:%s，请注意观察直播是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"用于直播的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"断开用于直播的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开用于录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试直播节目A的同时两路时移录制(节目A和节目B)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:播放和录制互不影响
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:4、将支持直播的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:5、查找符合直播需求的音视频解码器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen创建直播模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYStart启动直播节目A, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者当前未启动录制、直播节目A是否正常, 期望为是
//@EXECUTIONFLOW:9、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:11、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECStart启动录制节目A(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:14、询问测试者已启动时移录制节目A、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动时移录制节目B、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、等待录制10S
//@EXECUTIONFLOW:18、调用CSUDIRECStop停止录制上述启动的两路录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止录制、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIRECClose销毁上述两个录制器, 期望成功
//@EXECUTIONFLOW:21、调用CSUDIPLAYERStop停止直播, 期望成功
//@EXECUTIONFLOW:22、询问测试者直播是否已停止, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIPLAYERClose销毁直播播放器, 期望成功
//@EXECUTIONFLOW:24、检查录制的文件存在且大小不为0, 期望成功
//@EXECUTIONFLOW:25、查找一个支持时移文件播放的demux, 期望成功
//@EXECUTIONFLOW:26、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:27、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:28、询问测试者时移文件是否正常播放, 期望为是
//@EXECUTIONFLOW:29、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:30、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:31、对两个时移文件分别执行24~30
//@EXECUTIONFLOW:32、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0007(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec1 = CSUDI_NULL;
	CSUDI_HANDLE hRec2 = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int nRecTime = RECORD_TIME_SHORT;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持直播的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"支持直播的demux和tuner连接失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"设置节目信息失败");

	CSTCPrint("开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"创建录制器1失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第二个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"创建录制器2失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目1:%s 和节目2:%s，请注意观察直播是否受影响\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"启动录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"启动录制1失败");

	CSTCPrint("当前已启动两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下直播播放异常");

	CSTCPrint("等待录制%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("即将停止录制两路节目，请注意观察直播是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec1, CSUDI_FALSE)),
			"停止录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"停止录制2失败");

	CSTCPrint("当前已停止两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器2失败");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"用于直播的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器1失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"断开用于直播的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第二个支持录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试直播节目A的同时两路录制(时移录制节目A和普通录制节目B)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:播放和录制互不影响
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:4、将支持直播的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:5、查找符合直播需求的音视频解码器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen创建直播模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYStart启动直播节目A, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者当前未启动录制、直播节目A是否正常, 期望为是
//@EXECUTIONFLOW:9、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:11、将第二个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECStart启动录制节目A(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:14、询问测试者已启动时移录制节目A、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(普通录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动普通录制节目B、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、等待录制10S
//@EXECUTIONFLOW:18、调用CSUDIRECStop停止录制上述启动的两路录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止录制、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIRECClose销毁上述两个录制器, 期望成功
//@EXECUTIONFLOW:21、调用CSUDIPLAYERStop停止直播, 期望成功
//@EXECUTIONFLOW:22、询问测试者直播是否已停止, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIPLAYERClose销毁直播播放器, 期望成功
//@EXECUTIONFLOW:24、检查录制的两个文件存在且大小不为0, 期望全部成功
//@EXECUTIONFLOW:25、查找一个支持时移文件播放的demux, 期望成功
//@EXECUTIONFLOW:26、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:27、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:28、询问测试者时移文件是否正常播放, 期望为是
//@EXECUTIONFLOW:29、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:30、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:31、查找一个支持录制文件播放的demux, 期望成功
//@EXECUTIONFLOW:32、调用CSUDIPLAYEROpen创建录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:33、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:34、询问测试者录制文件是否正常播放, 期望为是
//@EXECUTIONFLOW:35、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:36、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:37、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0008(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec1 = CSUDI_NULL;
	CSUDI_HANDLE hRec2 = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int nRecTime = RECORD_TIME_SHORT;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持直播的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"支持直播的demux和tuner连接失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"设置节目信息失败");

	CSTCPrint("开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"创建录制器1失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第二个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"创建录制器2失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目1:%s 和节目2:%s，请注意观察直播是否受影响\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"启动录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"启动录制1失败");

	CSTCPrint("当前已启动两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下直播播放异常");

	CSTCPrint("等待录制%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("即将停止录制两路节目，请注意观察直播是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"停止录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"停止录制2失败");

	CSTCPrint("当前已停止两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器2失败");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"用于直播的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器1失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"断开用于直播的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第二个支持录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试直播节目A的同时录制TS
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:播放和录制互不影响
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:3、将支持直播的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:4、查找符合直播需求的音视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建直播模式的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYStart启动直播节目A, 期望返回成功
//@EXECUTIONFLOW:7、询问测试者当前未启动录制、直播节目A是否正常, 期望为是
//@EXECUTIONFLOW:8、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:9、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECStart启动录制(TS录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:12、询问测试者已启动TS录制、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:13、等待录制10S
//@EXECUTIONFLOW:14、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:15、询问测试者已停止录制、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:16、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:17、检查录制的文件存在且大小不为0, 期望成功
//@EXECUTIONFLOW:18、调用CSUDIPLAYERStop停止直播, 期望成功
//@EXECUTIONFLOW:19、询问测试者直播是否已停止, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIPLAYERClose销毁直播播放器, 期望成功
//@EXECUTIONFLOW:21、查找一个支持录制文件播放的demux, 期望成功
//@EXECUTIONFLOW:22、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:23、调用CSUDIPLAYERStart启动录制TS中节目A的播放, 期望返回成功
//@EXECUTIONFLOW:24、询问测试者节目A是否正常播放, 期望为是
//@EXECUTIONFLOW:25、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:26、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:27、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0009(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hPlayerPvr = CSUDI_NULL;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_SHORT;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持直播的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"支持直播的demux和tuner连接失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"设置节目信息失败");

	CSTCPrint("开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TS;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nCount = 4;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sStartPar.m_psPids[2].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sStartPar.m_psPids[2].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[2].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sStartPar.m_psPids[3].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sStartPar.m_psPids[3].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[3].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);

	CSTCPrint("即将开始TS录制，请注意观察直播是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"启动录制1失败");

	CSTCPrint("当前已启动TS录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下直播播放异常");

	CSTCPrint("等待录制%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("即将停止TS录制，请注意观察直播是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"停止录制失败");

	CSTCPrint("当前已停止TS录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"用于直播的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sStartPar.m_szFileName);
	CSTKWaitAnyKey();

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayerPvr) && hPlayerPvr != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayerPvr, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放节目信息失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayerPvr)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayerPvr)), 
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"销毁播放器失败");
	hPlayerPvr = CSUDI_NULL;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hPlayerPvr != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"销毁播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"断开用于直播的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开用于录制的demux和tuner的连接失败");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试PVR文件回放的同时进行一路普通录制(PID相同/不同)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:回放和录制互不影响
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:3、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:4、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:9、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:10、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIRECStart启动录制节目A(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:13、询问测试者已启动普通录制节目A、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:15、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:16、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:17、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:18、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIPLAYERSeek定位到文件头开始后1S的位置
//@EXECUTIONFLOW:21、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:22、调用CSUDIRECStart启动录制节目B(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:23、询问测试者已启动普通录制节目B、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:24、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:25、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:26、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:27、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:28、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:29、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:30、询问测试者已停止录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:31、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:32、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:33、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:34、检查两路录制的文件是否正常, 期望为是
CSUDI_BOOL CSTC_RECORD_IT_0010(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR回放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始PVR回放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于PVR播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试PVR文件回放的同时进行两路普通录制(PID相同和不同)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:回放和录制互不影响
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:4、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:5、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:9、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:10、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:12、将第二个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:14、调用CSUDIRECStart启动录制节目A(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动两路普通录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:18、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:19、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:20、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:21、调用CSUDIRECStop分别停止两路录制, 期望成功
//@EXECUTIONFLOW:22、询问测试者已停止录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:24、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:25、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:26、检查两路录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:27、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0011(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec1 = CSUDI_NULL;
	CSUDI_HANDLE hRec2 = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下PVR回放异常");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"创建录制器1失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第二个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"创建录制器2失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目1:%s 和节目2:%s，请注意观察播放是否受影响\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"启动录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"启动录制1失败");

	CSTCPrint("当前已启动两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制两路节目，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"停止录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec2)),
			"停止录制2失败");

	CSTCPrint("当前已停止两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器2失败");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于PVR播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器1失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第二个支持录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试PVR文件回放的同时进行一路时移录制(PID相同/不同)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:回放和录制互不影响
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:3、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:4、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:9、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:10、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIRECStart启动录制节目A(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:13、询问测试者已启动时移录制节目A、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:15、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:16、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:17、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:18、调用CSUDIRECStopTshift停止录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止时移录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIPLAYERSeek定位到文件头开始后1S的位置
//@EXECUTIONFLOW:21、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:22、调用CSUDIRECStart启动录制节目B(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:23、询问测试者已启动时移录制节目B、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:24、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:25、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:26、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:27、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:28、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:29、调用CSUDIRECStopTshift停止时移录制, 期望成功
//@EXECUTIONFLOW:30、询问测试者已停止录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:31、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:32、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:33、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:34、检查两路录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:35、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0012(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR回放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始PVR回放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下PVR回放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"启动时移录制失败");

	CSTCPrint("当前已启动时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动时移录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"停止时移录制失败");

	CSTCPrint("当前已停止时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下PVR回放异常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"启动时移录制失败");

	CSTCPrint("当前已启动时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动时移录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"停止时移录制失败");

	CSTCPrint("当前已停止时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止时移录制情况下播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于PVR播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试PVR文件回放的同时进行两路时移录制(PID相同和不同)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:回放和录制互不影响
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:4、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:5、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:9、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:10、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:12、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:14、调用CSUDIRECStart启动录制节目A(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动两路时移录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:18、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:19、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:20、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:21、调用CSUDIRECStopTshift分别停止两路时移录制, 期望成功
//@EXECUTIONFLOW:22、询问测试者已停止录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:24、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:25、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:26、检查两路录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:27、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0013(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec1 = CSUDI_NULL;
	CSUDI_HANDLE hRec2 = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下PVR回放异常");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"创建录制器1失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第二个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"创建录制器2失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目1:%s 和节目2:%s，请注意观察播放是否受影响\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"启动录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"启动录制1失败");

	CSTCPrint("当前已启动两路时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动时移录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制两路节目，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec1, CSUDI_FALSE)),
			"停止录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"停止录制2失败");

	CSTCPrint("当前已停止两路时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器2失败");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于PVR播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器1失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第二个支持录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试PVR文件回放的同时进行两路录制(普通录制和时移录制)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:回放和录制互不影响
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:4、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:5、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:9、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:10、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:12、将第二个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:14、调用CSUDIRECStart启动录制节目A(普通录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动两路录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:18、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:19、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:20、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:21、停止两路录制, 期望成功
//@EXECUTIONFLOW:22、询问测试者已停止录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:24、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:25、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:26、检查两路录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:27、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0014(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec1 = CSUDI_NULL;
	CSUDI_HANDLE hRec2 = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下PVR回放异常");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"创建录制器1失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"创建录制器2失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目1:%s 和节目2:%s，请注意观察播放是否受影响\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"启动录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"启动录制1失败");

	CSTCPrint("当前已启动两路录制(时移+普通)，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制两路节目，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"停止录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"停止录制2失败");

	CSTCPrint("当前已停止两路录制(时移+普通)，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器2失败");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于PVR播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器1失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第二个支持录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试PVR文件回放的同时进行TS录制
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:回放和录制互不影响
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找是否存在一个满足测试需求的PVR录制文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:3、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:4、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建PVR录制文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:9、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:10、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIRECStart启动录制节目(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:13、询问测试者已启动普通TS录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:15、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:16、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:17、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:18、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:21、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:22、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:23、检查录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:24、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0015(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_CHNL;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hPlayerPvr = CSUDI_NULL;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, PVRPLAY_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR回放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始PVR回放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TS;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nCount = 4;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sStartPar.m_psPids[2].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sStartPar.m_psPids[2].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[2].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sStartPar.m_psPids[3].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sStartPar.m_psPids[3].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[3].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);

	CSTCPrint("即将开始TS录制，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止TS录制，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于PVR播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sStartPar.m_szFileName);
	CSTKWaitAnyKey();

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayerPvr) && hPlayerPvr != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayerPvr, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放节目信息失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayerPvr)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayerPvr)), 
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"销毁播放器失败");
	hPlayerPvr = CSUDI_NULL;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hPlayerPvr != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"销毁播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开demux和tuner的连接失败");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试时移播放的同时进行一路普通录制(PID相同/不同)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:时移播放和录制互不影响
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找是否存在一个满足测试需求的时移文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:3、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:4、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:9、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:10、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIRECStart启动录制节目A(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:13、询问测试者已启动普通录制节目A、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:15、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:16、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:17、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:18、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止录制、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIPLAYERSeek定位到当前位置后30S的位置
//@EXECUTIONFLOW:21、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:22、调用CSUDIRECStart启动录制节目B(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:23、询问测试者已启动普通录制节目B、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:24、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:25、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:26、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:27、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:28、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:29、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:30、询问测试者已停止录制、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:31、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:32、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:33、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:34、检查两路录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:35、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0016(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于时移播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始时移播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于PVR播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试时移播放的同时进行两路普通录制(PID相同和不同)
//@PRECONDITION:1、Demux模块正常初始化
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:时移播放和录制互不影响
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找是否存在一个满足测试需求的时移文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:4、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:5、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:9、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:10、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:12、将第二个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:14、调用CSUDIRECStart启动录制节目A(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动两路普通录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:18、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:19、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:20、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:21、调用CSUDIRECStop分别停止两路录制, 期望成功
//@EXECUTIONFLOW:22、询问测试者已停止录制、PVR回放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:24、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:25、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:26、检查两路录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:27、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0017(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec1 = CSUDI_NULL;
	CSUDI_HANDLE hRec2 = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下时移播放异常");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"创建录制器1失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第二个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"创建录制器2失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目1:%s 和节目2:%s，请注意观察播放是否受影响\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"启动录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"启动录制1失败");

	CSTCPrint("当前已启动两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制两路节目，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"停止录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec2)),
			"停止录制2失败");

	CSTCPrint("当前已停止两路录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器2失败");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于PVR播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器1失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[2])),
			"断开第二个支持录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试时移播放的同时进行一路时移录制(PID相同/不同)
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:回放和录制互不影响
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找是否存在一个满足测试需求的时移文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:3、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:4、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:9、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:10、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIRECStart启动录制节目A(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:13、询问测试者已启动时移录制节目A、时移是否还在正常进行, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:15、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:16、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:17、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:18、调用CSUDIRECStopTshift停止录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止时移录制、时移是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIPLAYERSeek定位到当前位置后30S的位置
//@EXECUTIONFLOW:21、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:22、调用CSUDIRECStart启动录制节目B(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:23、询问测试者已启动时移录制节目B、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:24、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:25、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:26、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:27、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:28、询问测试者播放位置是否已变化, 期望为是
//@EXECUTIONFLOW:29、调用CSUDIRECStopTshift停止时移录制, 期望成功
//@EXECUTIONFLOW:30、询问测试者已停止录制、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:31、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:32、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:33、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:34、检查两路录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:35、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0018(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于时移的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始时移播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下时移播放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"启动时移录制失败");

	CSTCPrint("当前已启动时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动时移录制情况下时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"停止时移录制失败");

	CSTCPrint("当前已停止时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下时移播放异常");

	CSTCPrint("将重新设置播放位置，请注意观察播放位置是否有跳转\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"设置播放位置失败");

	CSTCPrint("已设置播放位置，请确认播放是否有跳转且正常播放\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"设置播放位置不正常");

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"启动时移录制失败");

	CSTCPrint("当前已启动时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动时移录制情况下时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制节目:%s，请注意观察播放是否受影响\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"停止时移录制失败");

	CSTCPrint("当前已停止时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止时移录制情况下播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于PVR播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;	

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试时移播放的同时进行两路时移录制(PID相同和不同)
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:时移播放和录制互不影响
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找是否存在一个满足测试需求的时移文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:4、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:5、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart启动时移文件的播放, 期望返回成功
//@EXECUTIONFLOW:9、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:10、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:12、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:14、调用CSUDIRECStart启动录制节目A(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动两路时移录制、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:18、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:19、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:20、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:21、调用CSUDIRECStopTshift分别停止两路时移录制, 期望成功
//@EXECUTIONFLOW:22、询问测试者已停止录制、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:24、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:25、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:26、检查两路录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:27、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0019(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec1 = CSUDI_NULL;
	CSUDI_HANDLE hRec2 = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于时移播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下时移播放异常");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"创建录制器1失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第二个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"创建录制器2失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目1:%s 和节目2:%s，请注意观察播放是否受影响\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"启动录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"启动录制1失败");

	CSTCPrint("当前已启动两路时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动时移录制情况下时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制两路节目，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec1, CSUDI_FALSE)),
			"停止录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"停止录制2失败");

	CSTCPrint("当前已停止两路时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器2失败");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于时移播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器1失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第二个支持录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试时移播放的同时进行两路录制(普通录制和时移录制)
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:时移播放和录制互不影响
//@EXECUTIONFLOW:1、查找所有支持录制的demux, 期望成功且个数大于等于2
//@EXECUTIONFLOW:2、锁定测试码流, 期望成功
//@EXECUTIONFLOW:3、查找是否存在一个满足测试需求的时移文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:4、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:5、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStart启动时移文件的播放, 期望成功
//@EXECUTIONFLOW:9、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:10、将第一个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器1, 期望成功
//@EXECUTIONFLOW:12、将第二个支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECOpen创建录制器2, 期望成功
//@EXECUTIONFLOW:14、调用CSUDIRECStart启动录制节目A(普通录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:15、调用CSUDIRECStart启动录制节目B(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:16、询问测试者已启动两路录制、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:17、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:18、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:19、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:20、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:21、停止两路录制, 期望成功
//@EXECUTIONFLOW:22、询问测试者已停止录制、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:24、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:25、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:26、检查两路录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:27、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0020(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hRec1 = CSUDI_NULL;
	CSUDI_HANDLE hRec2 = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam1;
	CSUDIRecStartParam_S sRecStartParam2;
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;

	memset(&sRecStartParam1, 0, sizeof(sRecStartParam1));	
	memset(&sRecStartParam2, 0, sizeof(sRecStartParam2));

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"查找所有支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"查找到的支持录制的demux个数至少为2才能继续下面的测试");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于时移播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下时移播放异常");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第一个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"创建录制器1失败");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"第二个支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"创建录制器2失败");

	sRecStartParam1.m_eRecType = EM_UDIRECORD_TYPE_CHNL;
	sRecStartParam1.m_bAppend = CSUDI_FALSE;
	sRecStartParam1.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam1.m_nCount = 2;
	sRecStartParam1.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sRecStartParam1.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam1.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sRecStartParam1.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sRecStartParam1.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam1.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sRecStartParam1.m_szFileName, "%s/%s_1", g_acFilePath, __FUNCTION__);

	sRecStartParam2.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sRecStartParam2.m_bAppend = CSUDI_FALSE;
	sRecStartParam2.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sRecStartParam2.m_nMaxSize = TSHIFT_MAX_SIZE;
	sRecStartParam2.m_nCount = 2;
	sRecStartParam2.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sRecStartParam2.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sRecStartParam2.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sRecStartParam2.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sRecStartParam2.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sRecStartParam2.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sRecStartParam2.m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);

	CSTCPrint("即将开始录制节目1:%s 和节目2:%s，请注意观察播放是否受影响\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"启动录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"启动录制1失败");

	CSTCPrint("当前已启动两路录制(时移+普通)，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止录制两路节目，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"停止录制1失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"停止录制2失败");

	CSTCPrint("当前已停止两路录制(时移+普通)，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器2失败");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于时移播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"录制文件1检查不通过");

	CSTCPrint("开始检查录制文件:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"录制文件2检查不通过");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"销毁录制器1失败");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"销毁录制器1失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"断开第一个支持录制的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"断开第二个支持录制的demux和tuner的连接失败");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:测试时移播放的同时进行TS录制
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:合法参数
//@EXPECTATION:回放和录制互不影响
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找是否存在一个满足测试需求的时移文件, 如果不存在则先录制再进行下面步骤
//@EXECUTIONFLOW:3、查找一个支持PVR文件播放的demux, 期望成功
//@EXECUTIONFLOW:4、查找用于播放的音频解码器、视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建时移文件播放模式的播放器, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYERSetStream设置节目信息, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERStart启动录制文件的播放, 期望返回成功
//@EXECUTIONFLOW:8、询问测试者播放是否正常, 期望为是
//@EXECUTIONFLOW:9、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:10、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:12、调用CSUDIRECStart启动录制节目(节目录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:13、询问测试者已启动普通TS录制、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:14、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:15、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:16、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:17、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:18、调用CSUDIRECStop停止录制, 期望成功
//@EXECUTIONFLOW:19、询问测试者已停止录制、时移播放是否还在正常进行, 期望为是
//@EXECUTIONFLOW:20、调用CSUDIRECClose销毁录制器, 期望成功
//@EXECUTIONFLOW:21、调用CSUDIPLAYERStop停止播放, 期望成功
//@EXECUTIONFLOW:22、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:23、检查录制的文件是否正常, 期望为是
//@EXECUTIONFLOW:24、断开上述步骤中连接的demux和tuner, 期望成功
CSUDI_BOOL CSTC_RECORD_IT_0021(void)
{
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	CSUDIRecType_E eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	CSUDI_HANDLE hPlayer = CSUDI_NULL;
	CSUDI_HANDLE hPlayerPvr = CSUDI_NULL;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sprintf(sPlaybackPar.m_szFileName, "%s/%s", g_acFilePath, TSHIFT_TESTFILE_NAME);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(eRecType, sPlaybackPar.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"没有可以用于测试的文件");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"创建用于时移播放的播放器失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("开始时移播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("已开始播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("当前尚未启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TS;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nCount = 4;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sStartPar.m_psPids[2].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nAudioPid;
	sStartPar.m_psPids[2].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[2].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eAudStreamType;
	sStartPar.m_psPids[3].m_nPid = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_nVideoPid;
	sStartPar.m_psPids[3].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[3].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_MPEG1].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);

	CSTCPrint("即将开始TS录制，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"启动录制失败");

	CSTCPrint("当前已启动录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下PVR回放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTCPrint("即将停止TS录制，请注意观察播放是否受影响\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"停止录制失败");

	CSTCPrint("当前已停止录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已停止录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("已停止播放节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁用于时移播放的播放器失败");
	hPlayer = CSUDI_NULL;

	CSTCPrint("开始检查录制文件:%s\n", sStartPar.m_szFileName);
	CSTKWaitAnyKey();

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayerPvr) && hPlayerPvr != CSUDI_NULL),
			"创建用于PVR播放的播放器失败");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayerPvr, asStreamInfo, 2, &sPlaybackPar)),
			"设置播放节目信息失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayerPvr)), 
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("播放是否正常\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayerPvr)), 
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"销毁播放器失败");
	hPlayerPvr = CSUDI_NULL;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"销毁播放器失败");
	}

	if (hPlayerPvr != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"销毁播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开demux和tuner的连接失败");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:模拟自动时移(直播节目A+时移录制节目A -> 停止直播+时移录制 -> 时移播放+时移录制)
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:1、录制内容未达到最大限定大小
//@INPUT:2、录制内容已超过最大限定大小
//@EXPECTATION:
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:3、将支持直播的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:4、查找符合直播需求的音视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建直播模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置播放参数, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYStart启动直播节目A, 期望返回成功
//@EXECUTIONFLOW:7、询问测试者直播节目A是否正常, 期望为是
//@EXECUTIONFLOW:8、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:9、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:10、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:11、调用CSUDIRECStart启动录制节目A(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:12、询问测试者已启动时移录制节目A、直播是否还在正常进行, 期望为是
//@EXECUTIONFLOW:13、调用CSUDIPLAYERStop停止直播, 期望成功
//@EXECUTIONFLOW:14、询问测试者直播是否已正常停止, 期望为是
//@EXECUTIONFLOW:15、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:16、等待录制30s
//@EXECUTIONFLOW:17、查找一个支持PVR播放的demux, 期望成功
//@EXECUTIONFLOW:18、调用CSUDIPLAYEROpen创建时移播放模式的播放器, 期望成功
//@EXECUTIONFLOW:19、调用CSUDIPLAYERSetStream设置播放参数, 期望成功
//@EXECUTIONFLOW:20、调用CSUDIPLAYStart启动时移播放节目A, 期望成功
//@EXECUTIONFLOW:21、询问测试者时移播放节目A是否正常, 期望为是
//@EXECUTIONFLOW:22、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:23、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:24、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:25、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:26、调用CSUDIPLAYERStop停止时移播放, 期望成功
//@EXECUTIONFLOW:27、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:28、调用CSUDIRECStop停止录制
//@EXECUTIONFLOW:29、调用CSUDIRECClose销毁录制器
//@EXECUTIONFLOW:30、断开上述步骤中链接的tuner和demux
CSUDI_BOOL CSTC_RECORD_IT_0022(void)
{	
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	int nDmxForLive = -1;
	CSUDI_HANDLE hLivePlayer = CSUDI_NULL;
	CSUDI_HANDLE hTshiftPlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_LONG;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");
	
	nDmxForLive = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((nDmxForLive >= 0), "获取支持直播的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, nDmxForLive)),
			"支持直播的demux和tuner连接失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nDemux = nDmxForLive;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hLivePlayer) && hLivePlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hLivePlayer, asStreamInfo, 2, CSUDI_NULL)),
			"设置节目信息失败");

	CSTCPrint("开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hLivePlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("当前为直播，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"未启动录制情况下直播播放异常");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nMaxSize = TSHIFT_MAX_SIZE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);

	CSTCPrint("即将开始时移录制节目:%s，请注意观察直播是否受影响\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"启动录制失败");

	CSTCPrint("当前已启动时移录制，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"已启动录制情况下直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hLivePlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
			"销毁直播播放器失败");
	hLivePlayer = CSUDI_NULL;

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hTshiftPlayer) && hTshiftPlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hTshiftPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("将开始时移播放节目:%s, 请注意观察\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hTshiftPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("当前为时移播放，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hTshiftPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hTshiftPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hTshiftPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hTshiftPlayer)),
			"销毁用于时移播放的播放器失败");
	hTshiftPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"停止录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
			"销毁直播播放器失败");
	}

	if (hTshiftPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hTshiftPlayer)),
			"销毁时移播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, nDmxForLive)),
			"断开直播的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开录制的demux和tuner的连接失败");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:模拟手动时移(直播节目A -> 停止直播 -> 时移录制 -> 时移播放+时移录制)
//@PRECONDITION:1、Demux模块正常初始化且存在支持录制的demux
//@PRECONDITION:2、Record模块正常初始化
//@PRECONDITION:3、Player模块正常初始化
//@INPUT:1、录制内容未达到最大限定大小
//@INPUT:2、录制内容已超过最大限定大小
//@EXECUTIONFLOW:1、锁定测试码流, 期望成功
//@EXECUTIONFLOW:2、查找一个支持直播的demux, 期望成功
//@EXECUTIONFLOW:3、将支持直播的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:4、查找符合直播需求的音视频解码器, 期望成功
//@EXECUTIONFLOW:5、调用CSUDIPLAYEROpen创建直播模式的播放器, 期望成功
//@EXECUTIONFLOW:7、调用CSUDIPLAYERSetStream设置播放参数, 期望成功
//@EXECUTIONFLOW:6、调用CSUDIPLAYStart启动直播节目A, 期望返回成功
//@EXECUTIONFLOW:7、询问测试者直播节目A是否正常, 期望为是
//@EXECUTIONFLOW:8、调用CSUDIPLAYERStop停止直播, 期望成功
//@EXECUTIONFLOW:9、询问测试者直播是否已正常停止, 期望为是
//@EXECUTIONFLOW:10、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:11、查找一个支持录制的demux, 期望成功
//@EXECUTIONFLOW:12、将支持录制的demux与tuner连接, 期望成功
//@EXECUTIONFLOW:13、调用CSUDIRECOpen创建录制器, 期望成功
//@EXECUTIONFLOW:14、调用CSUDIRECStart启动录制节目A(时移录制、非追加、清流), 期望成功
//@EXECUTIONFLOW:15、等待录制30s
//@EXECUTIONFLOW:16、查找一个支持PVR播放的demux, 期望成功
//@EXECUTIONFLOW:17、调用CSUDIPLAYEROpen创建时移播放模式的播放器, 期望成功
//@EXECUTIONFLOW:18、调用CSUDIPLAYERSetStream设置播放参数, 期望成功
//@EXECUTIONFLOW:19、调用CSUDIPLAYStart启动时移播放节目A, 期望成功
//@EXECUTIONFLOW:20、询问测试者时移播放节目A是否正常, 期望为是
//@EXECUTIONFLOW:21、调用CSUDIPLAYERPause暂停播放, 期望成功
//@EXECUTIONFLOW:22、询问测试者播放是否已暂停, 期望为是
//@EXECUTIONFLOW:23、调用CSUDIPLAYERResume恢复播放, 期望成功
//@EXECUTIONFLOW:24、询问测试者播放是否已恢复, 期望为是
//@EXECUTIONFLOW:25、调用CSUDIPLAYERStop停止时移播放, 期望成功
//@EXECUTIONFLOW:26、调用CSUDIPLAYERClose销毁播放器, 期望成功
//@EXECUTIONFLOW:27、调用CSUDIRECStop停止录制
//@EXECUTIONFLOW:28、调用CSUDIRECClose销毁录制器
//@EXECUTIONFLOW:29、断开上述步骤中链接的tuner和demux
CSUDI_BOOL CSTC_RECORD_IT_0023(void)
{	
	CSUDI_UINT32 dwTunerId = UDI2_TUNER0_ID;
	int nDmxForLive = -1;
	CSUDI_HANDLE hLivePlayer = CSUDI_NULL;
	CSUDI_HANDLE hTshiftPlayer = CSUDI_NULL;
	CSUDIPlayerChnl_S sPlayerChnl;
	CSUDIStreamInfo_S asStreamInfo[2];
	CSUDIPlaybackParam_S sPlaybackPar;
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	int nRecTime = RECORD_TIME_LONG;

	memset(&sStartPar, 0, sizeof(sStartPar));	

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"锁频失败");
	
	nDmxForLive = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((nDmxForLive >= 0), "获取支持直播的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, nDmxForLive)),
			"支持直播的demux和tuner连接失败");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "获取用于播放的音频解码器失败");

	sPlayerChnl.m_nDemux = nDmxForLive;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hLivePlayer) && hLivePlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hLivePlayer, asStreamInfo, 2, CSUDI_NULL)),
			"设置节目信息失败");

	CSTCPrint("开始直播节目:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hLivePlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("当前为直播，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"直播播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hLivePlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
			"销毁直播播放器失败");
	hLivePlayer = CSUDI_NULL;

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "获取支持录制的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"支持录制的demux和tuner连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"创建录制器失败");

	sStartPar.m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	sStartPar.m_bAppend = CSUDI_FALSE;
	sStartPar.m_stEncryptParam.m_eEncryptType = EM_UDI_ENCRYPT_TYPE_FREE;
	sStartPar.m_nMaxSize = TSHIFT_MAX_SIZE;
	sStartPar.m_nCount = 2;
	sStartPar.m_psPids[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	sStartPar.m_psPids[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	sStartPar.m_psPids[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	sStartPar.m_psPids[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	sStartPar.m_psPids[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	sStartPar.m_psPids[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;
	sprintf(sStartPar.m_szFileName, "%s/%s", g_acFilePath, __FUNCTION__);
	CSTCPrint("录制文件名: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"启动录制失败");

	CSTCPrint("等待录制%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "获取支持PVR播放的demux失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hTshiftPlayer) && hTshiftPlayer != CSUDI_NULL),
			"创建用于直播的播放器失败");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hTshiftPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"设置节目信息失败");

	CSTCPrint("将开始时移播放节目:%s, 请注意观察\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hTshiftPlayer)),
			"启动播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("当前为时移播放，播放是否正常?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hTshiftPlayer)), 
			"暂停播放失败");

	CSTCPrint("已暂停播放，请确认播放是否已暂停\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"暂停播放不正常");

	CSTCPrint("将恢复播放，请注意观察播放是否和暂停前紧密衔接\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hTshiftPlayer)), 
			"恢复播放失败");

	CSTCPrint("已恢复播放，请确认播放是否已恢复且和暂停前紧密衔接\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"恢复播放不正常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hTshiftPlayer)),
			"停止播放失败");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("播放是否已停止?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"停止时移播放异常");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hTshiftPlayer)),
			"销毁用于时移播放的播放器失败");
	hTshiftPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"停止录制失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	hRec = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
			"销毁直播播放器失败");
	}

	if (hTshiftPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hTshiftPlayer)),
			"销毁时移播放器失败");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"销毁录制器失败");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, nDmxForLive)),
			"断开直播的demux和tuner的连接失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"断开录制的demux和tuner的连接失败");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

