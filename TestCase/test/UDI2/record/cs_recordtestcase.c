/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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
#define MAX_ERROR_TIME 5 //��ȡʱ����Ϣ�Ĳ�������������Χ(��λ:��)

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
		"MPEG1��Ƶ���Խ�Ŀ2",
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
			CSTCPrint("�����õ��ļ�·��������\n");
		}
	}

	return bRet;
}

CSUDI_BOOL CSTC_RECORD_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

static int iRecCallback(CSUDI_HANDLE hRecHandle, const CSUDIRecEvent_E eEvent, void* pvEventData, void * pvUserData)
{
	return 0;
}

//@CASEGROUP:CSUDIRECAddCallback, CSUDIRECDelCallback
//@DESCRIPTION:���Իص���������Ӻ�ɾ��
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1���Ϸ�����
//@EXPECTATION:���سɹ�
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS, �Ҿ����Ϊ��
//@EXECUTIONFLOW:3������CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData1)���һ��δ��ӹ��Ļص�, pvUserData1 != CSUDI_NULL, ��������CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECDelCallback(hRecHandle, fnRecordCallback, pvUserData1)ɾ������3��ӵĻص�, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData2), ����fnRecordCallback�벽��2��fnRecordCallbackΪͬһ���ص�����ָ��,pvUserData2 != pvUserData1��pvUserData2 != CSUDI_NULL, ��������CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIRECDelCallback(hRecHandle, fnRecordCallback, pvUserData2)ɾ������4��ӵĻص�, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:7������CSUDIRECAddCallback(hRecHandle, fnRecordCallback, CSUDI_NULL), ����fnRecordCallback�벽��2��fnRecordCallbackΪͬһ���ص�����ָ��, ��������CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIRECDelCallback(hRecHandle, fnRecordCallback, CSUDI_NULL)ɾ������5��ӵĻص�, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:9������CSUDIRECClose(hRecHandle)���ٲ���2������¼����, ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_AddDelCallback_0001(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int nUserData1 = 31;
	int nUserData2 = 32;

	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)),
			"����¼����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)&nUserData1)),
			"��ӻص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)&nUserData1)),
			"ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)&nUserData2)),
			"��ӻص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)&nUserData2)),
			"ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, CSUDI_NULL)),
			"��ӻص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, CSUDI_NULL)),
			"ɾ���ص�ʧ��");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECAddCallback, CSUDIRECDelCallback
//@DESCRIPTION:���Ե���CSUDIRECAddCallback, CSUDIRECDelCallback�����������ʱ�ܷ��ض�Ӧ������
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1��hRecHandleΪCSUDI_NULL, ���������Ϸ�
//@INPUT:2��fnRecordCallbackΪCSUDI_NULL, ���������Ϸ�
//@INPUT:3��hRecHandle��fnRecordCallbackΪCSUDI_NULL, ���������Ϸ�
//@EXPECTATION:���ض�Ӧ������
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS, �Ҿ����Ϊ��
//@EXECUTIONFLOW:3������CSUDIRECAddCallback(CSUDI_NULL, fnRecordCallback, pvUserData), ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:4������CSUDIRECAddCallback(CSUDI_NULL, fnRecordCallback, pvUserData), ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:5������CSUDIRECAddCallback(hRecHandle, CSUDI_NULL, pvUserData), hRecHandleΪ�����������CSUDIRECOpen���, ��������CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIRECDelCallback(CSUDI_NULL, fnRecordCallback, pvUserData), ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:7������CSUDIRECDelCallback(CSUDI_NULL, CSUDI_NULL, pvUserData), ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8������CSUDIRECDelCallback(hRecHandle, CSUDI_NULL, pvUserData), hRecHandleΪ�����������CSUDIRECOpen���, ��������CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:9������CSUDIRECClose(hRecHandle)���ٴ�����¼����, ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_AddDelCallback_0002(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int nUserData = 1;

	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"����¼����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECAddCallback(CSUDI_NULL, iRecCallback, (void*)&nUserData)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECAddCallback(CSUDI_NULL, CSUDI_NULL, (void*)&nUserData)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECAddCallback(hRecHandle, CSUDI_NULL, (void*)&nUserData)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECDelCallback(CSUDI_NULL, iRecCallback, (void*)&nUserData)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECDelCallback(CSUDI_NULL, CSUDI_NULL, (void*)&nUserData)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECDelCallback(hRecHandle, CSUDI_NULL, (void*)&nUserData)),
			"���ش����벻��ȷ");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECAddCallback, CSUDIRECDelCallback
//@DESCRIPTION:�����ظ���Ӻ�ɾ��ͬһ���ص�ʱ�ܷ��ض�Ӧ������
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1���Ϸ�����
//@EXPECTATION:���ض�Ӧ������
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS, �Ҿ����Ϊ��
//@EXECUTIONFLOW:3������CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData)���һ��δ��ӹ��Ļص�, ��������CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData), ��������CSUDIREC_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW:5������CSUDIRECDelCallback(hRecHandle, fnRecordCallback, pvUserData)ɾ���ص�, ��������CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIRECDelCallback(hRecHandle, fnRecordCallback, pvUserData), ��������CSUDIREC_ERROR_NO_CALLBACK
//@EXECUTIONFLOW:7������CSUDIRECClose(hRecHandle)���ٴ�����¼����, ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_AddDelCallback_0003(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int nUserData = 2;

	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"����¼����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)&nUserData)),
			"��ӻص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_ALREADY_ADDED == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)&nUserData)),
			"���һ���Ѿ���ӵĻص������벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)&nUserData)),
			"ɾ���ص�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_NO_CALLBACK == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)&nUserData)),
			"ɾ��һ���Ѿ�ɾ�����Ļص������벻��ȷ");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)),
				"����¼����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECAddCallback, CSUDIRECDelCallback
//@DESCRIPTION:�����ܹ�֧��ͬʱ����32���ص�(�ص�����ָ����ͬ, �û����ݲ�ͬ)
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1���Ϸ�����
//@EXPECTATION:���سɹ�
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS�Ҿ����Ϊ��
//@EXECUTIONFLOW:3���ظ�����CSUDIRECAddCallback(hRecHandle, fnRecordCallback, pvUserData)���һ��δ��ӹ��Ļص�(pvUserDataÿ�α仯)��32��, ����������CSUDICSUDI_SUCCESS
//@EXECUTIONFLOW:4���ظ�����CSUDIRECDelCallbackɾ������������ӵ����лص�, ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIRECClose(hRecHandle)�����Ѵ�����¼����, ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_AddDelCallback_0004(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int anAddFlag[MAX_CALLBACK_COUNT] = {0};
	int i = 0;

	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"����¼����ʧ��");

	for (i=0; i<MAX_CALLBACK_COUNT; i++)
	{
		CSTCPrint("��ӵ�%d���ص�\n", i+1);
		
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRecHandle, iRecCallback, (void*)i)),
			"��ӻص�ʧ��");
		
		anAddFlag[i] = 1;
	}

	CSTK_FATAL_POINT;

	for (i=0; i<MAX_CALLBACK_COUNT; i++)
	{
		if (anAddFlag[i] == 1)
		{
			CSTCPrint("ɾ����%d���ص�\n", i+1);
		
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECDelCallback(hRecHandle, iRecCallback, (void*)i)),
				"ɾ���ص�ʧ��");

			anAddFlag[i] = 0;
		}		
	}

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
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
//@DESCRIPTION:�����ܷ�������洢�豸û�пռ���Ϣ, userdata�Ƿ���ȷ
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ��
//@INPUT:1���Ϸ�����
//@EXPECTATION:�ܼ�������Ϣ�ҷ���userdata�����õ���ͬ
//@EXECUTIONFLOW:1����ʾ�����߲���ʣ��ռ�С��5M�Ŀ��ƶ��洢�豸���ڴ洢¼���ļ�, ���޸������ļ��д洢·��������
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3������֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:4������tuner��demux, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:6���ظ�����CSUDIRECAddCallback��ӻص�, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIRECStart����¼��, �����ɹ�
//@EXECUTIONFLOW:8������30S�����յ��洢�ռ�������Ϣ��userdata�����õ���ͬ
//@EXECUTIONFLOW:9������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:10���ظ�����CSUDIRECDelCallbackɾ���ص�, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECClose����¼����, �����ɹ�
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
	
	CSTCPrint("�����ܷ�������洢�豸û�пռ���Ϣ\n");
	CSTCPrint("��ʹ��ʣ��ռ�С��5M�Ĵ洢�豸����֤�����ļ��д洢·��������ȷ\n");
	CSTCPrint("y:׼����������ʼ���ԣ�n:δ׼���������ݲ�����\n");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ׼���������ݲ�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec)) && (CSUDI_NULL != hRec)), 
			"����¼����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECAddCallback(hRec, iRecCallback_0005, (void*)&nUserData)),
			"��ӻص�ʧ��");
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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)), 
			"����¼��ʧ��");

	while (!s_bEventRight && nRecTime<30)
	{
		CSUDIOSThreadSleep(1000);
		nRecTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL((s_bEventRight), "δ�յ��洢�ռ�������Ϣ");

	CSTK_ASSERT_TRUE_FATAL((!s_bUserdataWrong), "�ص�userdata����ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECDelCallback(hRec, iRecCallback_0005, (void*)&nUserData)),
			"ɾ���ص�ʧ��");
	bCallbackAdded = CSUDI_FALSE;

	CSTK_FATAL_POINT;

	if (bCallbackAdded)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECDelCallback(hRec, iRecCallback_0005, (void*)&nUserData)),
				"ɾ���ص�ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
				"����¼����ʧ��");
	}

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose
//@DESCRIPTION:���Զ�ͬһ��demux��δ���������¼����
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1���Ϸ�����
//@EXPECTATION:���ܳɹ�
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen(psChnl, phRecHandle1)����¼����1,  ��������CSUDI_SUCCESS�Ҿ����Ϊ��
//@EXECUTIONFLOW:3������CSUDIRECOpen(psChnl, phRecHandle2)����¼����2,  ��������CSUDI_SUCCESS�Ҿ����Ϊ��
//@EXECUTIONFLOW:4������CSUDIRECClose(hRecHandle1)����¼����1, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:5������CSUDIRECClose(hRecHandle2)����¼����2, ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_OpenClose_0001(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle1 = CSUDI_NULL;
	CSUDI_HANDLE hRecHandle2 = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;

	nDemuxId = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle1)) && (CSUDI_NULL != hRecHandle1)), 
			"����¼����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle2)) && (CSUDI_NULL != hRecHandle2)), 
			"����¼����2ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle1)), 
			"����¼����1ʧ��");
	hRecHandle1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle2)), 
			"����¼����2ʧ��");
	hRecHandle2 = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if (hRecHandle1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle1)), 
				"����¼����1ʧ��");
	}

	if (hRecHandle2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle2)), 
				"����¼����2ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose
//@DESCRIPTION:��������֧��¼�Ƶ�demux��������������¼����
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1���Ϸ�����
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW:1�����ҳ�����֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen(psChnl, phRecHandle), ����һ��¼����,  ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIRECClose(hRecHandle)�����������贴����¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���������֧��¼�Ƶ�demux(��psChnl->m_nDemuxȡ��ͬ��demuxIdֵ)�ظ����ϲ���
CSUDI_BOOL CSTC_RECORD_IT_OpenClose_0002(void)
{
	int anDemuxId[DEMUX_COUNT] = {-1};
	int nActualCnt = 0;
	int i = 0;
	CSUDIRecChnl_S sRecChnl;
	CSUDI_HANDLE ahRecHandle[DEMUX_COUNT] = {CSUDI_NULL};

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt)),
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt > 0),
			"δ�����κ�֧��¼�Ƶ�demux");

	for (i=0; i<nActualCnt; i++)
	{
		CSTCPrint("������%d��¼������demuxId=%d\n", i+1, anDemuxId[i]);
		
		sRecChnl.m_nDemux = anDemuxId[i];
		CSTK_ASSERT_TRUE_FATAL(
				((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRecHandle[i])) && (CSUDI_NULL != ahRecHandle[i])), 
				"����¼����ʧ��");

		CSTCPrint("���ٵ�%d��¼����\n", i+1);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(ahRecHandle[i])), 
				"����¼����ʧ��");
		ahRecHandle[i] = CSUDI_NULL;
	}

	CSTK_FATAL_POINT;

	for (i=0; i<nActualCnt; i++)
	{
		if (ahRecHandle[i] != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRecHandle[i])), 
					"����¼����ʧ��");
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose
//@DESCRIPTION:���Ե���CSUDIRECOpen, CSUDIRECClose�����������ʱ�ܷ��ض�Ӧ������
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1��psChnlΪCSUDI_NULL, phRecHandle�Ϸ�
//@INPUT:2��psChnl�Ϸ�, phRecHandleΪCSUDI_NULL
//@INPUT:3��psChnl��m_nDemuxΪ-1, phRecHandle�Ϸ�
//@INPUT:4��psChnl��m_nDemuxΪ��֧��¼�Ƶ�demuxId, phRecHandle�Ϸ�
//@INPUT:5��hRecHandleΪCSUDI_NULL
//@EXPECTATION:���ض�Ӧ������
//@EXECUTIONFLOW:1������CSUDIRECOpen(CSUDI_NULL, phRecHandle), ����phRecHandleΪ�Ϸ�ֵ, ��������CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2������CSUDIRECOpen(psChnl, CSUDI_NULL), ����psChnlΪ�Ϸ�ֵ, ��������CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3������CSUDIRECOpen(psChnl, CSUDI_NULL), ����psChnl->m_nDemux=-1, ��������CSUDIREC_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:4������һ����֧��¼�Ƶ�demux, ���������ִ�в���5, ����ִ�в���6
//@EXECUTIONFLOW:5������CSUDIRECOpen(psChnl, CSUDI_NULL), ����psChnl->m_nDemuxΪ��֧��¼�Ƶ�demuxId, ��������CSUDIREC_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:6������CSUDIRECClose(CSUDI_NULL), ��������CSUDIREC_ERROR_INVALID_HANDLE
CSUDI_BOOL CSTC_RECORD_IT_OpenClose_0003(void)
{
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	int nDemuxId = -1;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECOpen(CSUDI_NULL, &hRecHandle)), 
			"���ش����벻��ȷ");

	nDemuxId = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECOpen(&sRecChnl, CSUDI_NULL)), 
			"���ش����벻��ȷ");

	sRecChnl.m_nDemux = -1;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_DEVICE_ID == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"���ش����벻��ȷ");

	nDemuxId = -1;
	nDemuxId = FindDemuxNotSupportRec();

	if (nDemuxId >= 0)
	{
		sRecChnl.m_nDemux = nDemuxId;
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDIREC_ERROR_INVALID_DEVICE_ID == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
				"���ش����벻��ȷ");
	}
	else
	{
		CSTCPrint("δ�ҵ���֧��¼�Ƶ�demux\n");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECClose(CSUDI_NULL)), 
			"���ش����벻��ȷ");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose
//@DESCRIPTION:��������һ���Ѿ����ٵ�¼����
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1���Ϸ�����
//@EXPECTATION:���ܳɹ�
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen(psChnl, phRecHandle)����һ��¼����,  ��������CSUDI_SUCCESS�Ҿ����Ϊ��
//@EXECUTIONFLOW:3������CSUDIRECClose(hRecHandle)����¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4���ٴε���CSUDIRECClose(hRecHandle), �������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_OpenClose_0004(void)
{	
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDI_HANDLE hRecHandleBak = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"����¼����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	
	hRecHandleBak = hRecHandle;
	hRecHandle = CSUDI_NULL;

	if (hRecHandleBak != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS != CSUDIRECClose(hRecHandleBak)), 
				"��ͼ�����Ѿ����ٵ�¼����Ӧ��ʧ��");
	}

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECStart, CSUDIRECStop, CSUDIRECStopTshift
//@DESCRIPTION:���Ե���CSUDIRECStart, CSUDIRECStop, CSUDIRECStopTshift�����������ʱ�ܷ��ض�Ӧ������
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1��hRecHandleΪCSUDI_NULL, psStartParam�Ϸ�
//@INPUT:2��hRecHandle�Ϸ�, psStartParamΪCSUDI_NULL
//@INPUT:3��hRecHandleΪCSUDI_NULL
//@EXPECTATION:���ض�Ӧ������
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS�Ҿ����Ϊ��
//@EXECUTIONFLOW:3������CSUDIRECStart(CSUDI_NULL, psStartParam), ����psStartParam�Ϸ�, ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:4������CSUDIRECStart(hRecHandle, CSUDI_NULL), ����hRecHandleΪCSUDIRECOpen���, ��������CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIRECStart(CSUDI_NULL, CSUDI_NULL), ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:6������CSUDIRECStop(CSUDI_NULL), ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:7������CSUDIRECStopTshift(CSUDI_NULL, CSUDI_FALSE), ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:8������CSUDIRECClose(hRecHandle)�����Ѵ�����¼����, ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_StartStop_0001(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sStartPar;
	
	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"����¼����ʧ��");

	memset(&sStartPar, 0, sizeof(sStartPar));
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECStart(CSUDI_NULL, &sStartPar)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECStart(hRecHandle, CSUDI_NULL)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECStart(CSUDI_NULL, CSUDI_NULL)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECStop(CSUDI_NULL)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECStopTshift(CSUDI_NULL, CSUDI_FALSE)),
			"���ش����벻��ȷ");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:���ԷǷ�״̬�µ���CSUDIRECStart, CSUDIRECStopʱ�ܷ��ض�Ӧ������
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1���Ϸ�����
//@EXPECTATION:���ض�Ӧ������
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS�Ҿ����Ϊ��
//@EXECUTIONFLOW:4������CSUDIRECStop(hRecHandle), ��������CSUDIREC_ERROR_NOT_STARTED
//@EXECUTIONFLOW:5������CSUDIRECStart(hRecHandle, psStartParam)����¼��, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6������CSUDIRECStart(hRecHandle, psStartParam), ��������CSUDIREC_ERROR_ALREADY_STARTED
//@EXECUTIONFLOW:7������CSUDIRECStop(hRecHandle), ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:8������CSUDIRECStop(hRecHandle), ��������CSUDIREC_ERROR_ALREADY_STOPPED
//@EXECUTIONFLOW:9������CSUDIRECClose(hRecHandle)�����Ѵ�����¼����, ��������CSUDI_SUCCESS
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
			"��Ƶʧ��");
	
	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, nDemuxId)),
			"����tuner��demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"����¼����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_NOT_STARTED == CSUDIRECStop(hRecHandle)), 
			"���ش�����벻��ȷ");
	
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

	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_ALREADY_STARTED == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"���ش�����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"ֹͣ¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_ALREADY_STOPPED == CSUDIRECStop(hRecHandle)), 
			"���ش�����벻��ȷ");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
	}

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECStart
//@DESCRIPTION:������ͨ¼��, ����Ƿ�PID(������PID)�����
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1���Ƿ�PID
//@EXPECTATION:���ض�Ӧ������
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS�Ҿ����Ϊ��
//@EXECUTIONFLOW:3������CSUDIRECStart(hRecHandle, psStartParam)����¼��(��ͨ¼��, ����ƵPID��Ϊ-1), ��������CSUDIREC_ERROR_INVALID_PIDS
//@EXECUTIONFLOW:4������CSUDIRECStart(hRecHandle, psStartParam)����¼��(��ͨ¼��, PID����Ϊ0), ��������CSUDIREC_ERROR_INVALID_PIDS
//@EXECUTIONFLOW:5������CSUDIRECClose(hRecHandle)�����Ѵ�����¼����, ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_StartStop_0003(void)
{
	CSUDI_HANDLE hRec = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecStartParam_S sRecStartParam;

	memset(&sRecStartParam, 0, sizeof(sRecStartParam));

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec)) && (CSUDI_NULL != hRec)), 
			"����¼����ʧ��");

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
			"���ش�����벻��ȷ");

	sRecStartParam.m_nCount = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_PIDS == CSUDIRECStart(hRec, &sRecStartParam)),
			"���ش�����벻��ȷ");
	
	CSTK_FATAL_POINT;

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRec)), 
				"����¼����ʧ��");
	}

	ClearFile(sRecStartParam.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECGetSupportEncryptTypes
//@DESCRIPTION:���Ե���CSUDIRECGetSupportEncryptTypes�����������ʱ�ܷ��ض�Ӧ������
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1��hRecHandleΪCSUDI_NULL, ���������Ϸ�
//@INPUT:2��peOutTypeArrayΪCSUDI_NULL, ���������Ϸ�
//@INPUT:3��nArraySizeΪ0, ���������Ϸ�
//@INPUT:4��pnActualCntΪCSUDI_NULL, ���������Ϸ�
//@EXPECTATION:���ض�Ӧ������
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIRECGetSupportEncryptTypes(CSUDI_NULL, peOutTypeArray, nArraySize, pnActualCnt), ����peOutTypeArray,nArraySize,pnActualCnt�Ϸ�, ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:4������CSUDIRECGetSupportEncryptTypes(hRecHandle, CSUDI_NULL, nArraySize, pnActualCnt), ����hRecHandle,nArraySize,pnActualCnt�Ϸ�, ��������CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIRECGetSupportEncryptTypes(hRecHandle, peOutTypeArray, 0, pnActualCnt), ����hRecHandle,peOutTypeArray,pnActualCnt�Ϸ�, ��������CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6������CSUDIRECGetSupportEncryptTypes(hRecHandle, peOutTypeArray, nArraySize, CSUDI_NULL), ����hRecHandle,nArraySize,peOutTypeArray�Ϸ�, ��������CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:7������CSUDIRECClose(hRecHandle)���ٲ���2������¼����, ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_GetSupportEncryptTypes_0001(void)
{
	int nDemuxId = -1;
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIStreamEncryptType_E aeEncryptType[3] = {0};
	int nArraySize = sizeof(aeEncryptType);
	int nActualCnt = 0;
	
	nDemuxId = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"����¼����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECGetSupportEncryptTypes(CSUDI_NULL, aeEncryptType, nArraySize, &nActualCnt)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECGetSupportEncryptTypes(hRecHandle, CSUDI_NULL, nArraySize, &nActualCnt)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECGetSupportEncryptTypes(hRecHandle, aeEncryptType, 0, &nActualCnt)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECGetSupportEncryptTypes(hRecHandle, aeEncryptType, nArraySize, CSUDI_NULL)),
			"���ش����벻��ȷ");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECGetSupportEncryptTypes
//@DESCRIPTION:����������ȡ¼��֧�ֵļ��ܷ�ʽ 
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1���Ϸ�����
//@EXPECTATION:����CSUDI_SUCCESS�һ�ȡ������Ϣ�������ļ��е���Ϣһ��
//@EXECUTIONFLOW:1�����ҳ�����֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIRECGetSupportEncryptTypes(hRecHandle, peOutTypeArray, nArraySize, pnActualCnt), ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4��������3�л�ȡ������Ϣ�������ļ��е���Ϣ�Ƚ�, ����һ��
//@EXECUTIONFLOW:5������CSUDIRECClose(hRecHandle)�����Ѵ�����¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:6���������֧��¼�Ƶ�demux�ظ�����2~5
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

	CSTK_ASSERT_TRUE_FATAL((nDemuxId >= 0), "����֧��¼�Ƶ�demuxʧ��");

	sRecChnl.m_nDemux = nDemuxId;
	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"����¼����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECGetSupportEncryptTypes(hRecHandle, aeEncryptType, nArraySize, &nActualCnt)),
			"��ȡ¼��֧�ֵļ��ܷ�ʽʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("RECORD", "ENCRYPT_TYPE_FREE", acBuf, sizeof(acBuf))),
			"�������ļ��л�ȡ�Ƿ�֧�� FREE ���ܷ�ʽʧ��");
	nSupportFree = CSTKGetIntFromStr(acBuf,10);
	if (nSupportFree == 1)
	{
		nCount ++;
	}

	memset(acBuf, 0, sizeof(acBuf));
	CSTK_ASSERT_TRUE_FATAL(
			(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("RECORD", "ENCRYPT_TYPE_CA", acBuf, sizeof(acBuf))),
			"�������ļ��л�ȡ�Ƿ�֧�� CA ���ܷ�ʽʧ��");
	nSupportCA = CSTKGetIntFromStr(acBuf,10);
	if (nSupportCA == 1)
	{
		nCount ++;
	}

	memset(acBuf, 0, sizeof(acBuf));
	CSTK_ASSERT_TRUE_FATAL(
			(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("RECORD", "ENCRYPT_TYPE_REENCRYPT", acBuf, sizeof(acBuf))),
			"�������ļ��л�ȡ�Ƿ�֧�� REENCRYPT ���ܷ�ʽʧ��");
	nSupportReencrypt = CSTKGetIntFromStr(acBuf,10);
	if (nSupportReencrypt == 1)
	{
		nCount ++;
	}

	CSTK_ASSERT_TRUE_FATAL((nCount == nActualCnt), "��ȡ����֧�ֵļ��ܷ�ʽ��������ȷ");

	for (i=0; i<nActualCnt; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				((aeEncryptType[i] == EM_UDI_ENCRYPT_TYPE_FREE && nSupportFree == 1) 
				|| (aeEncryptType[i] == EM_UDI_ENCRYPT_TYPE_CA && nSupportCA == 1)
				|| (aeEncryptType[i] == EM_UDI_ENCRYPT_TYPE_REENCRYPT && nSupportReencrypt == 1)),
				"��ȡ����֧�ֵļ��ܷ�ʽ����ȷ");
	}
		
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECGetRecTime
//@DESCRIPTION:���Ե���CSUDIRECGetRecTime�����������ʱ�ܷ��ض�Ӧ������
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1��hRecHandleΪCSUDI_NULL, ���������Ϸ�
//@INPUT:2��psTimeInfoΪCSUDI_NULL, ���������Ϸ�
//@INPUT:3�����в����Ϸ�, ��¼����δ����
//@EXPECTATION:���ض�Ӧ������
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:2������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������CSUDIRECGetRecTime(CSUDI_NULL, psTimeInfo), ����psTimeInfo�Ϸ�, ��������CSUDIREC_ERROR_INVALID_HANDLE
//@EXECUTIONFLOW:4������CSUDIRECGetRecTime(hRecHandle, CSUDI_NULL), ����hRecHandleΪCSUDIRECOpen���, ��������CSUDIREC_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5������CSUDIRECGetRecTime(hRecHandle, psTimeInfo), ����hRecHandle��psTimeInfo���Ϸ�, ��������CSUDIREC_ERROR_TIME_FAILED
//@EXECUTIONFLOW:6������CSUDIRECClose(hRecHandle)�����Ѵ�����¼����, ��������CSUDI_SUCCESS
CSUDI_BOOL CSTC_RECORD_IT_GetRecTime_0001(void)
{
	CSUDI_HANDLE hRecHandle = CSUDI_NULL;
	CSUDIRecChnl_S sRecChnl;
	CSUDIRecTimeInfo_S sRecTimeInfo;
	
	sRecChnl.m_nDemux = FindDemuxSupportRec();

	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			((CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)) && (CSUDI_NULL != hRecHandle)), 
			"����¼����ʧ��");

	memset(&sRecTimeInfo, 0, sizeof(sRecTimeInfo));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_INVALID_HANDLE == CSUDIRECGetRecTime(CSUDI_NULL, &sRecTimeInfo)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECGetRecTime(hRecHandle, CSUDI_NULL)),
			"���ش����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDIREC_ERROR_TIME_FAILED == CSUDIRECGetRecTime(hRecHandle, &sRecTimeInfo)),
			"���ش����벻��ȷ");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIRECGetRecTime
//@DESCRIPTION:���Ի�ȡ��ͨ¼�ƻ�ȡ¼��ʱ����Ϣ����ȷ��
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ���ȡ����ȷ��¼��ʱ����Ϣ
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:3������demux��tuner, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIRECStart����¼��(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:6���ȴ�¼��30S
//@EXECUTIONFLOW:7������CSUDIRECGetRecTime��ȡ¼��ʱ����Ϣ, �����ɹ�
//@EXECUTIONFLOW:8���ж�¼��ʱ����Ϣ��¼�ƿ�ʼ�ľ���ʱ���Ƿ�Ϊ0, ����Ϊ��
//@EXECUTIONFLOW:9���ж�¼��ʱ����Ϣ��¼����Чʱ���Ƿ���30����(ǰ����������5S), ����Ϊ��
//@EXECUTIONFLOW:10������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:12���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"����¼����ʧ��");

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

	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	memset(&sTimeInfo, 0, sizeof(sTimeInfo));
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECGetRecTime(hRecHandle, &sTimeInfo)),
			"��ȡ¼��ʱ����Ϣʧ��");

	CSTCPrint("¼��ʱ����Ϣ starttime=%d, duration=%d\n", sTimeInfo.m_nStartTime, sTimeInfo.m_nValidDuration);
	CSTK_ASSERT_TRUE_FATAL(
			(sTimeInfo.m_nStartTime == 0), 
			"¼��ʱ����Ϣ��¼�ƿ�ʼ�ľ���ʱ�䲻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(sTimeInfo.m_nValidDuration >= (nRecTime/1000-MAX_ERROR_TIME) && sTimeInfo.m_nValidDuration <= (nRecTime/1000+MAX_ERROR_TIME)), 
			"¼��ʱ����Ϣ��¼����Чʱ�䲻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"ֹͣ¼��ʧ��");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIRECGetRecTime
//@DESCRIPTION:����ʱ��¼�ƻ�ȡ¼��ʱ����Ϣ����ȷ��
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1��ʱ���ļ�δ�ﵽ����޶���С
//@EXPECTATION:�ܹ���ȡ����ȷ��¼��ʱ����Ϣ
//@REMARK:  ����ֻ������¼��δ��������޶���С��������ѳ��������CSTC_RECORD_IT_TShiftRec_0004�ɼ�˲⵽
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:3������demux��tuner, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIRECStart����¼��(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:6���ȴ�¼��30S
//@EXECUTIONFLOW:7������CSUDIRECGetRecTime��ȡ¼��ʱ����Ϣ, �����ɹ�
//@EXECUTIONFLOW:8���ж�¼��ʱ����Ϣ��¼�ƿ�ʼ�ľ���ʱ���Ƿ���ڵ���0, ����Ϊ��
//@EXECUTIONFLOW:9���ж�¼��ʱ����Ϣ��¼����Чʱ���Ƿ���30����(ǰ����������5S), ����Ϊ��
//@EXECUTIONFLOW:10������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:12���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"����¼����ʧ��");

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

	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	memset(&sTimeInfo, 0, sizeof(sTimeInfo));
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECGetRecTime(hRecHandle, &sTimeInfo)),
			"��ȡ¼��ʱ����Ϣʧ��");

	CSTCPrint("¼��ʱ����Ϣ starttime=%d, duration=%d\n", sTimeInfo.m_nStartTime, sTimeInfo.m_nValidDuration);
	CSTK_ASSERT_TRUE_FATAL(
			(sTimeInfo.m_nStartTime >= 0), 
			"¼��ʱ����Ϣ��¼�ƿ�ʼ�ľ���ʱ�䲻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(sTimeInfo.m_nValidDuration >= (nRecTime/1000-MAX_ERROR_TIME) && sTimeInfo.m_nValidDuration <= (nRecTime/1000+MAX_ERROR_TIME)), 
			"¼��ʱ����Ϣ��¼����Чʱ�䲻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"ֹͣ¼��ʧ��");

	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
				"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECFileCut
//@DESCRIPTION:���Ե���CSUDIRECFileCut�����������ʱ�ܷ��ض�Ӧ������
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@INPUT:1��szFileNameΪCSUDI_NULL, ���������Ϸ�
//@EXPECTATION:���ض�Ӧ������
//@EXECUTIONFLOW:1������CSUDIRECFileCut(CSUDI_NULL, 5, 10), ��������CSUDIREC_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_RECORD_IT_FileCut_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDIREC_ERROR_BAD_PARAMETER == CSUDIRECFileCut(CSUDI_NULL, 5, 10)), 
				"���ش����벻��ȷ");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECFileCut
//@DESCRIPTION:���Զ��Ѵ��ڵ�¼���ļ��и����ȷ��
//@PRECONDITION:1��Recordģ��������ʼ��
//@PRECONDITION:2��Demuxģ��������ʼ��
//@INPUT:1���Ϸ�����
//@EXPECTATION:�и���ļ���С
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:3������demux��tuner, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIRECStart����¼��(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:6���ȴ�¼��10S
//@EXECUTIONFLOW:10������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:11�����¼�Ƶ��ļ���С�Ƿ����0, ����Ϊ��
//@EXECUTIONFLOW:11������CSUDIRECFileCut�и�¼�Ƶ��ļ�, �и�ʱ��Ϊ2��5��, �����ɹ�
//@EXECUTIONFLOW:11����ȡ�и�ʱ����ļ���С, ��������0��С���и�ǰ�ļ���С
//@EXECUTIONFLOW:12���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"ֹͣ¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	hRecHandle = CSUDI_NULL;

	lFileSizeBefore = GetRecFileSize(sStartPar.m_szFileName);
	CSTK_ASSERT_TRUE_FATAL((lFileSizeBefore > 0), "¼�Ƶ��ļ���С��鲻ͨ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECFileCut(sStartPar.m_szFileName, 2, 5)), 
			"¼���ļ��и�ʧ��");

	lFileSizeAfter = GetRecFileSize(sStartPar.m_szFileName);
	CSTK_ASSERT_TRUE_FATAL(
			(lFileSizeAfter > 0 && lFileSizeAfter < lFileSizeBefore),
			"�и����ļ���С����ȷ");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");
	
	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����¼��һ·��Ŀ����׷��¼�ơ�����
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼��, ¼�Ƶ��ļ���������
//@EXECUTIONFLOW:1������Ҫ¼�Ƶ���������Ƶ��, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECStart����¼��(¼��һ·��Ŀ����׷��¼�ơ�����), �����ɹ�
//@EXECUTIONFLOW:5���ȴ�10S¼��ʱ��
//@EXECUTIONFLOW:6������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:8���鿴¼�Ƶ��ļ��Ѿ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:9������֧�ֲ��ŵ�demux�����õ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIPLAYEROpen����PVR¼�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYERStart��������, �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�����¼���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:15������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:16���Ͽ������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle) && hRecHandle != CSUDI_NULL), 
			"����¼����ʧ��");

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

	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"ֹͣ¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("¼����ϣ���ʼ¼���ļ��ļ�飬��ע��۲�����Ƶ�ܷ���������\n");
	CSTCPrint("¼��ʱ��Ϊ%dms����ʱ�����������ż���\n", nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"¼���ļ���鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:����¼��һ·��Ŀ����׷��¼�ơ����μ�����
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼����¼�Ƶ��ļ�����������
//@EXECUTIONFLOW:1������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:2������Ҫ¼�Ƶ���������Ƶ��, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECStart����¼��(¼��һ·��Ŀ����׷��¼�ơ����μ�����, ���ü�����Կ), �����ɹ�
//@EXECUTIONFLOW:5���ȴ�30S¼��ʱ��
//@EXECUTIONFLOW:6������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:8���Ͽ����������������ӵ�demux��tuner, �����ɹ�
//@EXECUTIONFLOW:9���鿴¼�Ƶ��ļ��Ѿ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:10������֧�ֲ��ŵ�demux�����õ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYEROpen����PVR¼�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIPLAYERStart��������, �����ɹ�
//@EXECUTIONFLOW:14��ѯ�ʲ������Ƿ��Ѿ��������ų���, ����Ϊ��
//@EXECUTIONFLOW:15������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:16������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"����¼����ʧ��");

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

	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"ֹͣ¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("¼����ϣ���ʼ¼���ļ��ļ�飬��ע��۲�����Ƶ�ܷ���������\n");
	CSTCPrint("¼��ʱ��Ϊ%d��%dms���������ż���\n", nRecTime, nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"¼���ļ���鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:����ʱ��¼�ơ���׷��¼�ơ�����
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼����¼�Ƶ��ļ�����������
//@EXECUTIONFLOW:1������Ҫ¼�Ƶ���������Ƶ��, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECStart����¼��(ʱ��¼�ơ���׷��¼�ơ�����), �����ɹ�
//@EXECUTIONFLOW:5���ȴ�10S¼��ʱ��
//@EXECUTIONFLOW:6������CSUDIRECStopTshiftֹͣʱ��¼��, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:8���鿴¼�Ƶ��ļ��Ѿ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:9������֧�ֲ��ŵ�demux�����õ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIPLAYEROpen����ʱ�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYERStart��������, �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�����¼���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:15������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:16���Ͽ������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRecHandle, CSUDI_FALSE)), 
			"ֹͣʱ��¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("¼����ϣ���ʼ¼���ļ��ļ�飬��ע��۲�����Ƶ�ܷ���������\n");
	CSTCPrint("¼��ʱ��Ϊ%dms����ʱ�����������ż���\n", nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"¼���ļ���鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:����ʱ��¼�ơ���׷��¼�ơ����μ�����
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼����¼�Ƶ��ļ��ܹ���������
//@EXECUTIONFLOW:1������Ҫ¼�Ƶ���������Ƶ��, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECStart����¼��(ʱ��¼�ơ���׷��¼�ơ����μ�����), �����ɹ�
//@EXECUTIONFLOW:5���ȴ�10S¼��ʱ��
//@EXECUTIONFLOW:6������CSUDIRECStopTshiftֹͣʱ��¼��, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:8���鿴¼�Ƶ��ļ��Ѿ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:9������֧�ֲ��ŵ�demux�����õ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIPLAYEROpen����ʱ�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYERStart��������, �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�����¼���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:15������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:16���Ͽ������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"����¼����ʧ��");

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

	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRecHandle, CSUDI_FALSE)), 
			"ֹͣʱ��¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("¼����ϣ���ʼ¼���ļ��ļ�飬��ע��۲�����Ƶ�ܷ���������\n");
	CSTCPrint("¼��ʱ��Ϊ%d��%dms���������ż���\n", nRecTime, nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"¼���ļ���鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStopTshift
//@DESCRIPTION:����ʱ��¼��ֹͣʱת������ͨ�ļ�
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼����¼�Ƶ��ļ��ܹ���������
//@EXECUTIONFLOW:1������Ҫ¼�Ƶ���������Ƶ��, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECStart����¼��(ʱ��¼�ơ���׷��¼�ơ�����), �����ɹ�
//@EXECUTIONFLOW:5���ȴ�10S¼��ʱ��
//@EXECUTIONFLOW:6������CSUDIRECStopTshiftֹͣʱ��¼�Ʋ���ʱ���ļ�ת����ͨ�ļ�, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:8���鿴¼�Ƶ��ļ��Ѿ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:9������֧�ֲ��ŵ�demux�����õ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIPLAYEROpen����ʱ�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYERStart��������, �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�����¼���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:15������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:16���Ͽ������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRecHandle, CSUDI_TRUE)), 
			"ֹͣʱ��¼���ҽ�ʱ���ļ�ת����ͨ�ļ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("¼����ϣ���ʼ¼���ļ��ļ�飬��ע��۲�����Ƶ�ܷ���������\n");
	CSTCPrint("¼��ʱ��Ϊ%dms����ʱ�����������ż���\n", nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"¼���ļ���鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:����ʱ��¼�ƴﵽ��󳤶Ⱥ����¼��һ��ʱ������
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼����¼�Ƶ��ļ�����������
//@EXECUTIONFLOW:1������Ҫ¼�Ƶ���������Ƶ��, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECStart����¼��(ʱ��¼�ơ���׷��¼�ơ�����), �����ɹ�
//@EXECUTIONFLOW:5���ȴ�¼��ʱ���Լ3���ӣ�����ȡ����¼��ʱ����Ϣ����ʼʱ�����0ʱ����
//@EXECUTIONFLOW:6���жϻ�ȡ����¼����ʼʱ���Ƿ����0, ����Ϊ��
//@EXECUTIONFLOW:7������CSUDIRECStopTshiftֹͣʱ��¼��, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:9���鿴¼�Ƶ��ļ��Ѿ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:10������֧�ֲ��ŵ�demux�����õ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYEROpen����ʱ�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIPLAYERStart��������, �����ɹ�
//@EXECUTIONFLOW:14��ѯ�ʲ�����¼���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:15������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:16������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:17���Ͽ������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle)), 
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("¼��ʱ���賬������޶����ȣ���Լ��ȴ�3����\n");

	memset(&sRecTimeInfo, 0, sizeof(sRecTimeInfo));
	while((sRecTimeInfo.m_nStartTime == 0) && (nWaitTime < 6))
	{
		CSUDIOSThreadSleep(nRecTime);

		memset(&sRecTimeInfo, 0, sizeof(sRecTimeInfo));
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECGetRecTime(hRecHandle, &sRecTimeInfo)),
				"��ȡ¼��ʱ����Ϣʧ��");
		
		nWaitTime++;
	}

	CSTCPrint("�ȴ�����ʱ��Ϊ: %d, ¼���ļ��޶���СΪ: %d(M)\n", nWaitTime*nRecTime, sStartPar.m_nMaxSize);

	CSTK_ASSERT_TRUE_FATAL(
			(sRecTimeInfo.m_nStartTime > 0), 
			"¼�Ƶȴ�ʱ��̻��ȡ¼��ʱ����Ϣ����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRecHandle, CSUDI_FALSE)), 
			"ֹͣʱ��¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	hRecHandle = CSUDI_NULL;

	CSTCPrint("¼����ϣ���ʼ¼���ļ��ļ�飬��ע��۲�����Ƶ�ܷ���������\n");
	CSTCPrint("¼��ʱ��Ϊ%dms����ʱ�����������ż���\n", nRecTime);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sStartPar)),
			"¼���ļ���鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:����TS¼�ơ���׷��¼�ơ�����
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼����¼�Ƶ��ļ�����������
//@EXECUTIONFLOW:1������Ҫ¼�Ƶ���������Ƶ��, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECStart����¼��(TS¼�ơ���׷��¼�ơ�����), �����ɹ�
//@EXECUTIONFLOW:5���ȴ�10S¼��ʱ��
//@EXECUTIONFLOW:6������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:8���鿴¼�Ƶ��ļ��Ѿ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:9������֧�ֲ��ŵ�demux�����õ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIPLAYEROpen����PVR¼�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYERStart��������, �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�����¼���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:15������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:16���Ͽ������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle) && hRecHandle != CSUDI_NULL), 
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"ֹͣ¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	hRecHandle = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(GetRecFileSize(sStartPar.m_szFileName) > 0), 
			"�ļ������ڻ��СΪ0");

	CSTCPrint("¼����ϣ���ʼ¼���ļ��ļ�飬��ע��۲�����Ƶ�ܷ���������\n");
	CSTCPrint("¼��ʱ��Ϊ%dms����ʱ�����������ż���\n", nRecTime);
	CSTKWaitAnyKey();

	memcpy(&asStreamInfo[0], &(sStartPar.m_psPids[0]), sizeof(CSUDIStreamInfo_S));
	memcpy(&asStreamInfo[1], &(sStartPar.m_psPids[1]), sizeof(CSUDIStreamInfo_S));

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"����֧��ʱ�Ʋ��ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), 
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)), 
			"���ٲ�����ʧ��");
	}

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop
//@DESCRIPTION:����TS¼�ơ���׷��¼�ơ����μ�����
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼����¼�Ƶ��ļ�����������
//@EXECUTIONFLOW:1������Ҫ¼�Ƶ���������Ƶ��, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��¼�Ƶ�demux, ����tuner����, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIRECOpen����һ��¼����, ��������CSUDI_SUCCESS
//@EXECUTIONFLOW:4������CSUDIRECStart����¼��(TS¼�ơ���׷��¼�ơ����μ�����), �����ɹ�
//@EXECUTIONFLOW:5���ȴ�10S¼��ʱ��
//@EXECUTIONFLOW:6������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:8���鿴¼�Ƶ��ļ��Ѿ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:9������֧�ֲ��ŵ�demux�����õ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIPLAYEROpen����PVR¼�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYERStart��������, �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�����¼���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:15������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:16���Ͽ������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "����֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"����tuner��demuxʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRecHandle) && hRecHandle != CSUDI_NULL), 
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRecHandle, &sStartPar)), 
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRecHandle)), 
			"ֹͣ¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	hRecHandle = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(GetRecFileSize(sStartPar.m_szFileName) > 0), 
			"�ļ������ڻ��СΪ0");

	CSTCPrint("¼����ϣ���ʼ¼���ļ��ļ�飬��ע��۲�����Ƶ�ܷ���������\n");
	CSTCPrint("¼��ʱ��Ϊ%dms����ʱ�����������ż���\n", nRecTime);
	CSTKWaitAnyKey();

	memcpy(&asStreamInfo[0], &(sStartPar.m_psPids[0]), sizeof(CSUDIStreamInfo_S));
	memcpy(&asStreamInfo[1], &(sStartPar.m_psPids[1]), sizeof(CSUDIStreamInfo_S));

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"����֧��ʱ�Ʋ��ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	memcpy(&(sPlaybackPar.m_stEncryptParam), &(sStartPar.m_stEncryptParam), sizeof(CSUDIStreamEncryptParam_S));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), 
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)), 
			"���ٲ�����ʧ��");
	}

	if (hRecHandle != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRecHandle)), 
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)), 
			"�Ͽ�tuner��demux������ʧ��");

	ClearFile(sStartPar.m_szFileName);

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSetSpeed, CSUDIPLAYERSeek
//@DESCRIPTION:����PVR¼���ļ��ط�
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:PVR¼���ļ���������
//@EXECUTIONFLOW:1�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:2������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:3���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:7��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��(��ʾ�ļ���С����, �˴�����ͣ��̫��ʱ��)
//@EXECUTIONFLOW:8������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:9��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:10������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:11��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:12������CSUDIPLAYERSeek��λ���ļ�ͷ��ʼ��1S��λ��
//@EXECUTIONFLOW:13��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERSetSpeed����8���ٿ��, �����ɹ�
//@EXECUTIONFLOW:15������CSUDIPLAYERGetSpeed��ȡ��������, �����ɹ��Һ����õ����
//@EXECUTIONFLOW:16��ѯ�ʲ����߲��������Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:17������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:18������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"û�п������ڲ��Ե��ļ�");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"����֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTCPrint("���½�����PVR�طŲ��ԣ������ļ��϶̣��벻Ҫ�����²���ͣ��̫��ʱ�䣬�������Ӱ����Խ��\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("���������ţ���ȷ�ϲ����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"���ò�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPlayer, &eSpeed)), 
			"��ȡ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"��ȡ���Ĳ�������ֵ�����õĲ�һ��");

	CSTCPrint("�Ѹı䲥�����ʣ���ȷ�ϲ��������Ƿ��ѱ仯\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò������ʲ�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), 
			"ֹͣ����ʧ��");

	CSTCPrint("��ֹͣ���ţ���ȷ�ϲ����Ƿ���ֹͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣ���Ų�����");
	
	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"���ٲ�����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSetSpeed, CSUDIPLAYERSeek
//@DESCRIPTION:����ʱ���ļ�ʱ�Ʋ���(¼������δ�ﵽ����޶���С/¼�������ѳ�������޶���С)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:ʱ�Ʋ�������
//@EXECUTIONFLOW:1�������Ƿ����һ��������������ʱ���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:2������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:3������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:5��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��(��ʾ�ļ���С����, �˴�����ͣ��̫��ʱ��)
//@EXECUTIONFLOW:6������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:7��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:8������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:9��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:10������CSUDIPLAYERSeek��λ����ǰλ�ú�30S��λ��
//@EXECUTIONFLOW:11��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:12������CSUDIPLAYERSetSpeed����8���ٿ��, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIPLAYERGetSpeed��ȡ��������, �����ɹ��Һ����õ����
//@EXECUTIONFLOW:14��ѯ�ʲ����߲��������Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:15������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:16������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"û�п������ڲ��Ե��ļ�");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"����֧��ʱ�Ʋ��ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTCPrint("���½�����ʱ�Ʋ��Ų��ԣ������ļ��϶̣��벻Ҫ�����²���ͣ��̫��ʱ�䣬�������Ӱ����Խ��\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("���������ţ���ȷ�ϲ����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"���ò�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPlayer, &eSpeed)), 
			"��ȡ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"��ȡ���Ĳ�������ֵ�����õĲ�һ��");

	CSTCPrint("�Ѹı䲥�����ʣ���ȷ�ϲ��������Ƿ��ѱ仯\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò������ʲ�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)), 
			"ֹͣ����ʧ��");

	CSTCPrint("��ֹͣ���ţ���ȷ�ϲ����Ƿ���ֹͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣ���Ų�����");
	
	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"���ٲ�����ʧ��");
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
			CSTCPrint("�յ��������ļ�ĩβ����Ϣ\n");
			s_bEndofStream = CSUDI_TRUE;
			break;
		}

		case EM_UDIPLAYER_BEGIN_OF_STREAM:
		{
			CSTCPrint("�յ��������ļ���ͷ����Ϣ\n");
			s_bBeginofStream = CSUDI_TRUE;
			break;
		}

		default:
			break;
	}
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERAddPlayerCallback
//@DESCRIPTION:���Բ���PVR¼���ļ��ܷ�������������ļ�ĩβ���������ļ���ͷ����Ϣ
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:2��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ���������Ϣ
//@EXECUTIONFLOW:1������һ��������������PVR¼���ļ�(¼��3����), �������������¼��
//@EXECUTIONFLOW:2������һ�����ϲ���Ҫ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:3������һ�����ϲ���Ҫ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYERAddPlayerCallbackע�Ქ�����ļ�ĩβ���͵Ļص�, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERAddPlayerCallbackע�Ქ�����ļ���ͷ���͵Ļص�, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ò��Ų���, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:9���ȴ�����1s
//@EXECUTIONFLOW:10������CSUDIPLAYERSetSpeed����32���ٿ���, �����ɹ�
//@EXECUTIONFLOW:11������Ƿ���2S�����յ��������ļ���ͷ����Ϣ, �����յ�
//@EXECUTIONFLOW:12������CSUDIPLAYERSetSpeed���ó��ٲ���, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIPLAYERSeek��λ���ļ�βǰ3S��λ�ò���, �����ɹ�
//@EXECUTIONFLOW:14������Ƿ���4S�����յ��������ļ�ĩβ����Ϣ, �����յ�
//@EXECUTIONFLOW:15������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:16������CSUDIPLAYERDelPlayerCallback�Ƴ��������ļ�ĩβ��Ϣ�ص�, �����ɹ�
//@EXECUTIONFLOW:17������CSUDIPLAYERDelPlayerCallback�Ƴ��������ļ���ͷ��Ϣ�ص�, �����ɹ�
//@EXECUTIONFLOW:18������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"û�п������ڲ��Ե��ļ�");

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");
	
	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer)),
			"����������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_END_OF_STREAM, (void*)&nUserdata)),
			"ע�Ქ�����ļ�ĩβ���͵Ļص�ʧ��");
	bCallbackAdded1 = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_BEGIN_OF_STREAM, (void*)&nUserdata)),
			"ע�Ქ�����ļ���ͷ���͵Ļص�ʧ��");
	bCallbackAdded2 = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ų���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ȴ�����1S\n");
	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTREWIND_32)),
			"���ò�������ʧ��");

	while (!s_bBeginofStream && nWaitTime < 4)
	{
		CSUDIOSThreadSleep(500);
		nWaitTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL(
			(s_bBeginofStream),
			"δ�յ��������ļ���ͷ����Ϣ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_NORMAL)),
			"���ò�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, -3, EM_UDIPLAYER_POSITION_FROM_END)),
			"��λ���ļ���βǰ3Sλ��ʧ��");

	nWaitTime = 0;
	while (!s_bEndofStream && nWaitTime < 8)
	{
		CSUDIOSThreadSleep(500);
		nWaitTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL(
			(s_bEndofStream),
			"δ�յ��������ļ�ĩβ����Ϣ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (bCallbackAdded1)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_END_OF_STREAM, (void*)&nUserdata)),
				"�Ƴ��������ļ�ĩβ���͵Ļص�ʧ��");
	}

	if (bCallbackAdded2)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_BEGIN_OF_STREAM, (void*)&nUserdata)),
				"�Ƴ��������ļ���ͷ���͵Ļص�ʧ��");
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"���ٲ�����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERAddPlayerCallback
//@DESCRIPTION:���Բ���ʱ���ļ��ܷ�������������ļ�ĩβ���������ļ���ͷ����Ϣ(¼������δ�ﵽ����޶���С/¼�������ѳ�������޶���С)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:2��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ���������Ϣ
//@EXECUTIONFLOW:1������һ��������������ʱ���ļ�(¼��3����), �������������¼��
//@EXECUTIONFLOW:2������һ�����ϲ���Ҫ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:3������һ�����ϲ���Ҫ�����Ƶ������, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYERAddPlayerCallbackע�Ქ�����ļ�ĩβ���͵Ļص�, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERAddPlayerCallbackע�Ქ�����ļ���ͷ���͵Ļص�, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ò��Ų���, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:9���ȴ�����1s
//@EXECUTIONFLOW:10������CSUDIPLAYERSetSpeed����32���ٿ���, �����ɹ�
//@EXECUTIONFLOW:11������Ƿ���2S�����յ��������ļ���ͷ����Ϣ, �����յ�
//@EXECUTIONFLOW:12������CSUDIPLAYERSetSpeed���ó��ٲ���, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIPLAYERSeek��λ���ļ�βǰ3S��λ�ò���, �����ɹ�
//@EXECUTIONFLOW:14������Ƿ���4S�����յ��������ļ�ĩβ����Ϣ, �����յ�
//@EXECUTIONFLOW:15������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:16������CSUDIPLAYERDelPlayerCallback�Ƴ��������ļ�ĩβ��Ϣ�ص�, �����ɹ�
//@EXECUTIONFLOW:17������CSUDIPLAYERDelPlayerCallback�Ƴ��������ļ���ͷ��Ϣ�ص�, �����ɹ�
//@EXECUTIONFLOW:18������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"û�п������ڲ��Ե��ļ�");

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");
	
	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer)),
			"����������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_END_OF_STREAM, (void*)&nUserdata)),
			"ע�Ქ�����ļ�ĩβ���͵Ļص�ʧ��");
	bCallbackAdded1 = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERAddPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_BEGIN_OF_STREAM, (void*)&nUserdata)),
			"ע�Ქ�����ļ���ͷ���͵Ļص�ʧ��");
	bCallbackAdded2 = CSUDI_TRUE;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ų���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ȴ�����1S\n");
	CSUDIOSThreadSleep(1000);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_FASTREWIND_32)),
			"���ò�������ʧ��");

	while (!s_bBeginofStream && nWaitTime < 4)
	{
		CSUDIOSThreadSleep(500);
		nWaitTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL(
			(s_bBeginofStream),
			"δ�յ��������ļ���ͷ����Ϣ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPlayer, EM_UDIPLAYER_SPEED_NORMAL)),
			"���ò�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, -3, EM_UDIPLAYER_POSITION_FROM_END)),
			"��λ���ļ���βǰ3Sλ��ʧ��");

	nWaitTime = 0;
	while (!s_bEndofStream && nWaitTime < 8)
	{
		CSUDIOSThreadSleep(500);
		nWaitTime ++;
	}

	CSTK_ASSERT_TRUE_FATAL(
			(s_bEndofStream),
			"δ�յ��������ļ�ĩβ����Ϣ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (bCallbackAdded1)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_END_OF_STREAM, (void*)&nUserdata)),
				"�Ƴ��������ļ�ĩβ���͵Ļص�ʧ��");
	}

	if (bCallbackAdded2)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERDelPlayerCallback(hPlayer, iPlayerCallback, EM_UDIPLAYER_BEGIN_OF_STREAM, (void*)&nUserdata)),
				"�Ƴ��������ļ���ͷ���͵Ļص�ʧ��");
	}

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"���ٲ�����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERGetCurPosInSec
//@DESCRIPTION:���Ի�ȡPVR¼���ļ����ŵ�ǰ����λ�õľ���ʱ��
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:2��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:��ȷ��ȡPVR¼���ļ����ŵ�ǰ����λ�õľ���ʱ��
//@EXECUTIONFLOW:1�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:2������һ��֧��PVR���ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:3���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:7���ȴ�����60s
//@EXECUTIONFLOW:8������CSUDIPLAYERGetCurPosInSec��ȡ��ǰ����λ�õľ���ʱ��, �����ɹ�
//@EXECUTIONFLOW:9������ȡ���ĵ�ǰ����λ�õľ���ʱ���Ƿ���60S����(ǰ����������5S), ����Ϊ��
//@EXECUTIONFLOW:10������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"û�п������ڲ��Ե��ļ�");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"����֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer)),
			"����������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ų���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ȵ�����%dms(����ʱ��̫�̵Ļ���ȡ���ž���ʱ������ʸߣ�����һ�����)\n", nPlayTime);
	CSUDIOSThreadSleep(nPlayTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetCurPosInSec(hPlayer, &nPosInSec)),
			"��ȡ��ǰ���ŵľ���ʱ��ʧ��");

	CSTCPrint("��ȡ���ĵ�ǰ���ŵľ���ʱ��Ϊ %d\n", nPosInSec);
	CSTK_ASSERT_TRUE_FATAL((nPosInSec >= (nPlayTime/1000-MAX_ERROR_TIME) && nPosInSec <= (nPlayTime/1000+MAX_ERROR_TIME)), "��ȡ���ĵ�ǰ���ŵľ���ʱ������ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"���ٲ�����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERGetCurPosInSec
//@DESCRIPTION:���Ի�ȡʱ���ļ����ŵ�ǰ����λ�õľ���ʱ��(¼������δ�ﵽ����޶���С/¼�������ѳ�������޶���С)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:2��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:��ȷ��ȡ��ʱ���ļ����ŵ�ǰ����λ�õľ���ʱ��
//@EXECUTIONFLOW:1�������Ƿ����һ��������������ʱ���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:2������һ��֧��PVR���ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:3���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:7���ȴ�����60s
//@EXECUTIONFLOW:8������CSUDIPLAYERGetCurPosInSec��ȡ��ǰ����λ�õľ���ʱ��, �����ɹ�
//@EXECUTIONFLOW:9������ȡ���ĵ�ǰ����λ�õľ���ʱ���Ƿ���ڵ���60S(ǰ����������5S), ����Ϊ��
//@EXECUTIONFLOW:10������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"û�п������ڲ��Ե��ļ�");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nDemux >= 0), 
			"����֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPlayer)),
			"����������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ų���ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ȵ�����%d ms(����ʱ��̫�̵Ļ���ȡ���ž���ʱ������ʸߣ�����һ�����)\n", nPlayTime);
	CSUDIOSThreadSleep(nPlayTime);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetCurPosInSec(hPlayer, &nPosInSec)),
			"��ȡ��ǰ���ŵľ���ʱ��ʧ��");

	CSTCPrint("��ȡ���ĵ�ǰ���ŵľ���ʱ��Ϊ %d\n", nPosInSec);
	CSTK_ASSERT_TRUE_FATAL((nPosInSec >= (nPlayTime/1000-MAX_ERROR_TIME) && nPosInSec <= (nPlayTime/1000+MAX_ERROR_TIME)), "��ȡ���ĵ�ǰ���ŵľ���ʱ������ȷ");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
				"����¼����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSeek
//@DESCRIPTION:����ֱ����PVR�ļ��ط�֮ǰ�л�
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:2��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:���ܹ���������
//@EXECUTIONFLOW:1�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:2������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:3������һ��֧��¼���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:4�����ҿ����ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen��������ֱ���Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream����ֱ����Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStart����ֱ��, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:10��ѯ�ʲ����߲����Ƿ���ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:11������CSUDIPLAYERClose����ֱ���Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:14������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��(��ʾ�ļ���С����, �˴�����ͣ��̫��ʱ��)
//@EXECUTIONFLOW:16������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:18������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIPLAYERSeek��λ���ļ�ͷ��ʼ��1S��λ��
//@EXECUTIONFLOW:21������CSUDIPLAYERGetSpeed��ȡ��������, �����ɹ��Һ����õ����
//@EXECUTIONFLOW:22��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIPLAYERSetSpeed����8���ٿ��, �����ɹ�
//@EXECUTIONFLOW:24��ѯ�ʲ����߲��������Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:25������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:26������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"û�п������ڲ��Ե��ļ�");

	nDmxForLive = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForLive >= 0), 
			"����֧��ֱ����demuxʧ��");

	nDmxForPvr = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForPvr >= 0), 
			"����֧��PVR���ŵ�demuxʧ��");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"��Ƶʧ��");

	sPlayerChnl.m_nDemux = nDmxForLive;
	hLivePlayer = LivePlayStart(&sPlayerChnl, &asStreamInfo[0], &asStreamInfo[1]);
	CSTK_ASSERT_TRUE_FATAL(
			(hLivePlayer != CSUDI_NULL), 
			"��ʼֱ��ʧ��");

	CSTCPrint("�ѿ�ʼֱ���������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֱ��������");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == LivePlayStop(hLivePlayer, nDmxForLive)), 
			"��ʼֱ��ʧ��");
	hLivePlayer = CSUDI_NULL;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	sPlayerChnl.m_nDemux = nDmxForPvr;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPvrPlayer) && hPvrPlayer != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPvrPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTCPrint("���½�����PVR�طŲ��ԣ������ļ��϶̣��벻Ҫ�����²���ͣ��̫��ʱ�䣬�������Ӱ����Խ��\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPvrPlayer)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("���������ţ���ȷ�ϲ����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPvrPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPvrPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPvrPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPvrPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"���ò�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPvrPlayer, &eSpeed)), 
			"��ȡ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"��ȡ���Ĳ�������ֵ�����õĲ�һ��");

	CSTCPrint("�Ѹı䲥�����ʣ���ȷ�ϲ��������Ƿ��ѱ仯\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò������ʲ�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPvrPlayer)), 
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���ţ���ȷ�ϲ����Ƿ���ֹͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣ���Ų�����");
	
	CSTK_FATAL_POINT;

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
				"����ֱ��������ʧ��");
	}

	if (hPvrPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPvrPlayer)),
				"����PVR������ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSeek
//@DESCRIPTION:����ֱ����ʱ�Ʋ���֮ǰ�л�
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:2��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:���ܹ���������
//@EXECUTIONFLOW:1�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:2������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:3������һ��֧��¼���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:4�����ҿ����ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen��������ֱ���Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream����ֱ����Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStart����ֱ��, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:10��ѯ�ʲ����߲����Ƿ���ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:11������CSUDIPLAYERClose����ֱ���Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:14������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��(��ʾ�ļ���С����, �˴�����ͣ��̫��ʱ��)
//@EXECUTIONFLOW:16������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:18������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIPLAYERSeek��λ���ļ���ǰλ�ú�30S��λ��
//@EXECUTIONFLOW:21��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:22������CSUDIPLAYERSetSpeed����8���ٿ��, �����ɹ�
//@EXECUTIONFLOW:23������CSUDIPLAYERGetSpeed��ȡ��������, �����ɹ��Һ����õ����
//@EXECUTIONFLOW:24��ѯ�ʲ����߲��������Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:25������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:26������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"û�п������ڲ��Ե��ļ�");

	nDmxForLive = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForLive >= 0), 
			"����֧��ֱ����demuxʧ��");

	nDmxForPvr = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForPvr >= 0), 
			"����֧��PVR���ŵ�demuxʧ��");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));
	
	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"��Ƶʧ��");

	sPlayerChnl.m_nDemux = nDmxForLive;
	hLivePlayer = LivePlayStart(&sPlayerChnl, &asStreamInfo[0], &asStreamInfo[1]);
	CSTK_ASSERT_TRUE_FATAL(
			(hLivePlayer != CSUDI_NULL), 
			"��ʼֱ��ʧ��");

	CSTCPrint("�ѿ�ʼֱ���������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֱ��������");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == LivePlayStop(hLivePlayer, nDmxForLive)), 
			"��ʼֱ��ʧ��");
	hLivePlayer = CSUDI_NULL;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	sPlayerChnl.m_nDemux = nDmxForPvr;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, ePlayerType, &hPvrPlayer) && hPvrPlayer != CSUDI_NULL),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPvrPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTCPrint("���½�����ʱ�Ʋ��Ų��ԣ������ļ��϶̣��벻Ҫ�����²���ͣ��̫��ʱ�䣬�������Ӱ����Խ��\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPvrPlayer)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("���������ţ���ȷ�ϲ����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPvrPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPvrPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPvrPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPvrPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"���ò�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPvrPlayer, &eSpeed)), 
			"��ȡ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"��ȡ���Ĳ�������ֵ�����õĲ�һ��");

	CSTCPrint("�Ѹı䲥�����ʣ���ȷ�ϲ��������Ƿ��ѱ仯\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò������ʲ�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPvrPlayer)), 
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���ţ���ȷ�ϲ����Ƿ���ֹͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣ���Ų�����");
	
	CSTK_FATAL_POINT;

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
				"����ֱ��������ʧ��");
	}

	if (hPvrPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPvrPlayer)),
				"����ʱ�Ʋ�����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop, CSUDIPLAYERPause, CSUDIPLAYERResume, CSUDIPLAYERSeek
//@DESCRIPTION:����PVR�ļ��طź�ʱ�Ʋ���֮ǰ�л�
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:2��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:���ܹ���������
//@EXECUTIONFLOW:1�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:1�������Ƿ����һ��������������ʱ���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:3������һ��֧��¼���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:4�����ҿ����ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen��������PVR�طŵĲ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream����ֱ����Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStart����PVR�ط�, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:10��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:11������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:12��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:13������CSUDIPLAYERSeek��λ���ļ�ͷ��ʼ��1S��λ��
//@EXECUTIONFLOW:14��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:15������CSUDIPLAYERSetSpeed����8���ٿ��, �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ����߲��������Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:17������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:18��ѯ�ʲ����߲����Ƿ���ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:19������CSUDIPLAYERClose����ֱ���Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:20������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:21������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:22������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:23��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��(��ʾ�ļ���С����, �˴�����ͣ��̫��ʱ��)
//@EXECUTIONFLOW:24������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:25��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:26������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:27��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:28������CSUDIPLAYERSeek��λ����ǰλ�ú�30S��λ��
//@EXECUTIONFLOW:29��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:30������CSUDIPLAYERSetSpeed����8���ٿ��, �����ɹ�
//@EXECUTIONFLOW:31������CSUDIPLAYERGetSpeed��ȡ��������, �����ɹ��Һ����õ����
//@EXECUTIONFLOW:32��ѯ�ʲ����߲��������Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:33������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:34������CSUDIPLAYERClose���ٲ�����, �����ɹ�
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
			"û�п������ڲ��Ե�PVR¼���ļ�");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == PreparePlayTestFile(EM_UDIRECORD_TYPE_TSHIFT, sPlaybackParTShift.m_szFileName, &s_sServiceInfo[EM_RECORD_H264_HD])),
			"û�п������ڲ��Ե�ʱ���ļ�");
	
	nDmxForPvr = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL(
			(nDmxForPvr >= 0), 
			"����֧��PVR���ŵ�demuxʧ��");

	memset(&sPlayerChnl, -1, sizeof(sPlayerChnl));

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL(
			(sPlayerChnl.m_nAudioDecoder >= 0), 
			"���ҿ��õ���Ƶ������ʧ��");

	sPlayerChnl.m_nDemux = nDmxForPvr;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPvrPlayer) && hPvrPlayer != CSUDI_NULL),
			"��������PVR�طŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPvrPlayer, asStreamInfo, 2, &sPlaybackParPvr)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTCPrint("���½�����PVR�طŲ��ԣ������ļ��϶̣��벻Ҫ�����²���ͣ��̫��ʱ�䣬�������Ӱ����Խ��\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPvrPlayer)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("���������ţ���ȷ�ϲ����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPvrPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPvrPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPvrPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hPvrPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"���ò�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hPvrPlayer, &eSpeed)), 
			"��ȡ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"��ȡ���Ĳ�������ֵ�����õĲ�һ��");

	CSTCPrint("�Ѹı䲥�����ʣ���ȷ�ϲ��������Ƿ��ѱ仯\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò������ʲ�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPvrPlayer)), 
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���ţ���ȷ�ϲ����Ƿ���ֹͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣ���Ų�����");

	if (hPvrPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPvrPlayer)),
				"����PVR�طŲ�����ʧ��");
		hPvrPlayer = CSUDI_NULL;
	}
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hTShiftPlayer) && hTShiftPlayer != CSUDI_NULL),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hTShiftPlayer, asStreamInfo, 2, &sPlaybackParTShift)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTCPrint("���½�����ʱ�Ʋ��Ų��ԣ������ļ��϶̣��벻Ҫ�����²���ͣ��̫��ʱ�䣬�������Ӱ����Խ��\n");
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hTShiftPlayer)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("���������ţ���ȷ�ϲ����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hTShiftPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hTShiftPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hTShiftPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetSpeed(hTShiftPlayer, EM_UDIPLAYER_SPEED_FASTFORWARD_8)), 
			"���ò�������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERGetSpeed(hTShiftPlayer, &eSpeed)), 
			"��ȡ��������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(eSpeed == EM_UDIPLAYER_SPEED_FASTFORWARD_8), 
			"��ȡ���Ĳ�������ֵ�����õĲ�һ��");

	CSTCPrint("�Ѹı䲥�����ʣ���ȷ�ϲ��������Ƿ��ѱ仯\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò������ʲ�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hTShiftPlayer)), 
			"ֹͣ����ʧ��");

	CSTCPrint("��ֹͣ���ţ���ȷ�ϲ����Ƿ���ֹͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣ���Ų�����");
	
	CSTK_FATAL_POINT;

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	if (hTShiftPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hTShiftPlayer)),
				"����ʱ�Ʋ�����ʧ��");
	}

	if (hPvrPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIPLAYERClose(hPvrPlayer)),
				"����PVR�طŲ�����ʧ��");
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:��������֧��¼�Ƶ�demux���ֱܷ����¼��
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:����֧��¼�Ƶ�demux���������¼����¼�Ƶ��ļ�����������
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2����������֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:3����һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:4������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIRECStart����¼��(��Ŀ¼�ơ���׷��¼�ơ�����), �����ɹ�
//@EXECUTIONFLOW:6���ȴ�¼��10S
//@EXECUTIONFLOW:7������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:9���Ͽ�demux��tuner������
//@EXECUTIONFLOW:10���ж�¼�Ƶ��ļ��Ƿ��Ѵ����Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIPLAYEROpen����PVR¼�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYERStart����¼���ļ��Ĳ���, �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERStopֹͣ����
//@EXECUTIONFLOW:15������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:16���Ͽ�֧��¼�Ƶ�demux��tuner������, �����ɹ�
//@EXECUTIONFLOW:17��������֧��¼�Ƶ�demux�ظ�����3~16
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
			"��Ƶʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == FindAllDemuxSupportRec(anDemuxId, sizeof(anDemuxId), &nActualCnt) && nActualCnt>0),
			"��������֧��¼�Ƶ�demuxʧ��");

	for (i=0; i<nActualCnt; i++)
	{
		CSTCPrint("���Ե�%d��demux, demuxId=%d\n", i+1, anDemuxId[i]);

		memset(&sStartPar, 0, sizeof(sStartPar));	
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[i])),
				"demux��tuner����ʧ��");

		sRecChnl.m_nDemux = anDemuxId[i];
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRecHandle[i]) && ahRecHandle[i] != CSUDI_NULL),
				"����¼����ʧ��");	

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
		CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);

		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(ahRecHandle[i], &sStartPar)), 
			"����¼��ʧ��");

		CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
		CSUDIOSThreadSleep(nRecTime);

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECStop(ahRecHandle[i])),
				"ֹͣ¼��ʧ��");	

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(ahRecHandle[i])),
				"����¼����ʧ��");
		ahRecHandle[i] = CSUDI_NULL;

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[i])),
				"�Ͽ�demux��tuner������ʧ��");

		CSTCPrint("¼����ϣ���ʼ¼���ļ��ļ�飬��ע��۲�����Ƶ�ܷ���������\n");
		CSTCPrint("¼��ʱ��Ϊ%dms����ʱ�����������ż���\n", nRecTime);
		CSTKWaitAnyKey();

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == CheckRecResult(sStartPar)),
				"¼���ļ���鲻ͨ��");

		ClearFile(sStartPar.m_szFileName);
	}

	CSTK_FATAL_POINT;

	for (i=0; i<nActualCnt; i++)
	{
		if (ahRecHandle[i] != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIRECClose(ahRecHandle[i])),
				"����¼����ʧ��");
			ahRecHandle[i] = CSUDI_NULL;

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[i])),
					"�Ͽ�demux��tuner������ʧ��");
		}
	}

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ͬʱ������·¼�Ƶ����(��ͬ¼��ģʽ)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼����¼�Ƶ��ļ��ܹ���������
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3�����֧��¼�Ƶ�demux1����CSUDIDEMUXConnectTuner��tuner����, �����ɹ�
//@EXECUTIONFLOW:4�����demux1����CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:5�����֧��¼�Ƶ�demux2����CSUDIDEMUXConnectTuner��tuner����, �����ɹ�
//@EXECUTIONFLOW:6�����demux2����CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:7����������Ѵ�����¼��������CSUDIRECStart����¼��(��Ŀ¼��/ʱ��¼�ơ���׷��¼�ơ�����), ¼���ļ��������Ʋ�ͬ, ����ȫ���ɹ�
//@EXECUTIONFLOW:8������¼�Ƶȴ�¼��10S
//@EXECUTIONFLOW:9���������¼��������CSUDIRECStopֹͣ¼��, ����ȫ���ɹ�
//@EXECUTIONFLOW:10����������¼����, ����ȫ���ɹ�
//@EXECUTIONFLOW:11�����һ��¼�Ƶ��ļ��Ƿ��Ѵ����Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIPLAYEROpen����PVR¼�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIPLAYERStart����һ��¼���ļ��Ĳ���, �����ɹ�
//@EXECUTIONFLOW:14��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:15������CSUDIPLAYERStopֹͣ����
//@EXECUTIONFLOW:16������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:17����������¼�Ƶ��ļ�ִ�в���11~17
//@EXECUTIONFLOW:18���Ͽ���������demux��tuner������, ����ȫ���ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[0])),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRec[0]) && ahRec[0] != CSUDI_NULL),
			"����¼����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[1])),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRec[1]) && ahRec[1] != CSUDI_NULL),
			"����¼����2ʧ��");

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
			CSTCPrint("������%d��¼������¼��ģʽΪ: %d�� �ļ���Ϊ: %s\n", i+1, asStartPar[i].m_eRecType, asStartPar[i].m_szFileName);

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECStart(ahRec[i], &(asStartPar[i]))),
					"����¼��ʧ��");
		}
	}

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("ֹͣ��%d��¼����\n", i+1);

			if (asStartPar[i].m_eRecType == EM_UDIRECORD_TYPE_TSHIFT)
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIRECStopTshift(ahRec[i], CSUDI_FALSE)),
						"ֹͣ¼��ʧ��");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIRECStop(ahRec[i])),
						"ֹͣ¼��ʧ��");
			}
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("���ٵ�%d��¼����\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRec[i])),
					"����¼����ʧ��");
			ahRec[i] = CSUDI_NULL;
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		CSTCPrint("����%d��¼�ƽ��\n", i+1);
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(asStartPar[i])),
			"¼���ļ���鲻ͨ��");
	}

	CSTK_FATAL_POINT;

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{			
			CSTCPrint("���ٵ�%d��¼����\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRec[i])),
					"����¼����ʧ��");
		}
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	for (i=0; i<nRecCount; i++)
	{
		ClearFile(asStartPar[i].m_szFileName);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����һ·¼������һ��ʱ�����������һ·¼�Ƶ����
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�ܹ�����¼����¼�Ƶ��ļ��ܹ���������
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3�����֧��¼�Ƶ�demux1����CSUDIDEMUXConnectTuner��tuner����, �����ɹ�
//@EXECUTIONFLOW:4�����demux1����CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:5�����֧��¼�Ƶ�demux2����CSUDIDEMUXConnectTuner��tuner����, ����ȫ���ɹ�
//@EXECUTIONFLOW:6�����demux2����CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIRECStart����¼����1��һ·¼��(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:8���ȴ�¼��10S
//@EXECUTIONFLOW:9������CSUDIRECStart����¼����2��һ·¼��(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:10���ȴ�¼��10S
//@EXECUTIONFLOW:11������CSUDIRECStop�ֱ�ֹͣ������·¼��, ����ȫ���ɹ�
//@EXECUTIONFLOW:12������CSUDIRECClose������������¼����, ����ȫ���ɹ�
//@EXECUTIONFLOW:13�����һ��¼�Ƶ��ļ��Ƿ��Ѵ����Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:14������һ��֧��¼���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:15������CSUDIPLAYEROpen����PVR¼�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:16������CSUDIPLAYERStart����һ��¼���ļ��Ĳ���, �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:18������CSUDIPLAYERStopֹͣ����
//@EXECUTIONFLOW:19������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:20����������¼�Ƶ��ļ��ֱ�ִ�в���13~20
//@EXECUTIONFLOW:24���Ͽ���������demux��tuner������, ����ȫ���ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)), 
			"��Ƶʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[0])),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRec[0]) && ahRec[0] != CSUDI_NULL),
			"����¼����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, anDemuxId[1])),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &ahRec[1]) && ahRec[1] != CSUDI_NULL),
			"����¼����2ʧ��");
		
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
	CSTCPrint("¼���ļ���1: %s\n", asStartPar[0].m_szFileName);
	
	memcpy(&asStartPar[1], &asStartPar[0], sizeof(CSUDIRecStartParam_S));
	asStartPar[1].m_eRecType = EM_UDIRECORD_TYPE_TSHIFT;
	asStartPar[1].m_nMaxSize = TSHIFT_MAX_SIZE;
	sprintf(asStartPar[1].m_szFileName, "%s/%s_2", g_acFilePath, __FUNCTION__);
	CSTCPrint("¼���ļ���2: %s\n", asStartPar[1].m_szFileName);

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("������%d��¼����\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECStart(ahRec[i], &asStartPar[i])),
					"����¼��ʧ��");

			CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
			CSUDIOSThreadSleep(nRecTime);
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("ֹͣ��%d��¼����\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECStop(ahRec[i])),
					"ֹͣ¼��ʧ��");
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{
			CSTCPrint("���ٵ�%d��¼����\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRec[i])),
					"����¼����ʧ��");
			ahRec[i] = CSUDI_NULL;
		}
	}

	for (i=0; i<nRecCount; i++)
	{
		CSTCPrint("����%d��¼�ƽ��\n", i+1);
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(asStartPar[i])),
			"¼���ļ���鲻ͨ��");
	}

	CSTK_FATAL_POINT;

	for (i=0; i<nRecCount; i++)
	{
		if (ahRec[i] != CSUDI_NULL)
		{			
			CSTCPrint("���ٵ�%d��¼����\n", i+1);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIRECClose(ahRec[i])),
					"����¼����ʧ��");
		}
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	for (i=0; i<nRecCount; i++)
	{
		ClearFile(asStartPar[i].m_szFileName);
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ֱ����ĿA��ͬʱ����һ·��ͨ¼��(��ĿA/��ĿB)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:���ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:3����֧��ֱ����demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:4�����ҷ���ֱ�����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����ֱ��ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �������سɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYStart����ֱ����ĿA, �������سɹ�
//@EXECUTIONFLOW:7��ѯ�ʲ����ߵ�ǰδ����¼�ơ�ֱ����ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:8������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:9����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECStart����¼�ƽ�ĿA(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:12��ѯ�ʲ�������������ͨ¼�ƽ�ĿA��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:13���ȴ�¼��10S
//@EXECUTIONFLOW:14������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ�������ֹͣ¼�ơ�ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:16������CSUDIRECStart����¼�ƽ�ĿB(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ�������������ͨ¼�ƽ�ĿB��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:18���ȴ�¼��10S
//@EXECUTIONFLOW:19������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:20��ѯ�ʲ�������ֹͣ¼�ơ�ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:21������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:22������CSUDIPLAYERStopֱֹͣ��, �����ɹ�
//@EXECUTIONFLOW:23��ѯ�ʲ�����ֱ���Ƿ���ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:24������CSUDIPLAYERClose����ֱ��������, �����ɹ�
//@EXECUTIONFLOW:25������һ��֧��¼���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:26������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:27������CSUDIPLAYERStart����¼���ļ�A�Ĳ���, �������سɹ�
//@EXECUTIONFLOW:28��ѯ�ʲ�����¼���ļ�A�Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:29������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:30������CSUDIPLAYERStart����¼���ļ�B�Ĳ���, �������سɹ�
//@EXECUTIONFLOW:31��ѯ�ʲ�����¼���ļ�B�Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:32������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:33������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:34���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��ֱ����demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"֧��ֱ����demux��tuner����ʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ֱ�������쳣");

	CSTCPrint("�ȴ�¼��%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ֱ�������쳣");

	CSTCPrint("�ȴ�¼��%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֱֹͣ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֱֹͣ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������ֱ���Ĳ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;	

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"�Ͽ�����ֱ����demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�����¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ֱ����ĿA��ͬʱ��·��ͨ¼��(��ĿA�ͽ�ĿB)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:���ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:4����֧��ֱ����demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:5�����ҷ���ֱ�����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen����ֱ��ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYStart����ֱ����ĿA, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����ߵ�ǰδ����¼�ơ�ֱ����ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:11�����ڶ���֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECStart����¼�ƽ�ĿA(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:14��ѯ�ʲ�������������ͨ¼�ƽ�ĿA��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�������������ͨ¼�ƽ�ĿB��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17���ȴ�¼��10S
//@EXECUTIONFLOW:18������CSUDIRECStopֹͣ������������·¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣ¼�ơ�ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIRECClose������������¼����, �����ɹ�
//@EXECUTIONFLOW:21������CSUDIPLAYERStopֱֹͣ��, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ�����ֱ���Ƿ���ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIPLAYERClose����ֱ��������, �����ɹ�
//@EXECUTIONFLOW:24�����¼�Ƶ��ļ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:25������һ��֧��¼���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:26������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:27������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:28��ѯ�ʲ�����¼���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:29������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:30������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:31��������¼���ļ��ֱ�ִ��24~30
//@EXECUTIONFLOW:32���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��ֱ����demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"֧��ֱ����demux��tuner����ʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"����¼����1ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"����¼����2ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ1:%s �ͽ�Ŀ2:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"����¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ��������·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ֱ�������쳣");

	CSTCPrint("�ȴ�¼��%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("����ֹͣ¼����·��Ŀ����ע��۲�ֱ���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"ֹͣ¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec2)),
			"ֹͣ¼��2ʧ��");

	CSTCPrint("��ǰ��ֹͣ��·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����2ʧ��");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֱֹͣ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֱֹͣ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������ֱ���Ĳ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����1ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"�Ͽ�����ֱ����demux��tuner������ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ֱ����ĿA��ͬʱ����һ·ʱ��¼��(��ĿA/��ĿB)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:���ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:3����֧��ֱ����demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:4�����ҷ���ֱ�����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����ֱ��ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYStart����ֱ����ĿA, �������سɹ�
//@EXECUTIONFLOW:7��ѯ�ʲ����ߵ�ǰδ����¼�ơ�ֱ����ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:8������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:9����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECStart����¼�ƽ�ĿA(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:12��ѯ�ʲ�����������ʱ��¼�ƽ�ĿA��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:13���ȴ�¼��10S
//@EXECUTIONFLOW:14������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�����������ʱ��¼�ƽ�ĿB��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17���ȴ�¼��10S
//@EXECUTIONFLOW:18������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣ¼�ơ�ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:21�����¼�Ƶ������ļ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:22������CSUDIPLAYERStopֱֹͣ��, �����ɹ�
//@EXECUTIONFLOW:23��ѯ�ʲ�����ֱ���Ƿ���ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:24������CSUDIPLAYERClose����ֱ��������, �����ɹ�
//@EXECUTIONFLOW:25������һ��֧��ʱ���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:26������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:27������CSUDIPLAYERStart����ʱ���ļ�A�Ĳ���, �������سɹ�
//@EXECUTIONFLOW:28��ѯ�ʲ�����ʱ���ļ�A�Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:29������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:30������CSUDIPLAYERStartB����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:31��ѯ�ʲ�����ʱ���ļ�B�Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:32������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:33������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:34���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��ֱ����demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"֧��ֱ����demux��tuner����ʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ֱ�������쳣");

	CSTCPrint("�ȴ�¼��%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ֱ�������쳣");

	CSTCPrint("�ȴ�¼��%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֱֹͣ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֱֹͣ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"����ֱ���Ĳ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"�Ͽ�����ֱ����demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�����¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ֱ����ĿA��ͬʱ��·ʱ��¼��(��ĿA�ͽ�ĿB)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:���ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:4����֧��ֱ����demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:5�����ҷ���ֱ�����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen����ֱ��ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYStart����ֱ����ĿA, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����ߵ�ǰδ����¼�ơ�ֱ����ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:11������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECStart����¼�ƽ�ĿA(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:14��ѯ�ʲ�����������ʱ��¼�ƽ�ĿA��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�����������ʱ��¼�ƽ�ĿB��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17���ȴ�¼��10S
//@EXECUTIONFLOW:18������CSUDIRECStopֹͣ¼��������������·¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣ¼�ơ�ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIRECClose������������¼����, �����ɹ�
//@EXECUTIONFLOW:21������CSUDIPLAYERStopֱֹͣ��, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ�����ֱ���Ƿ���ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIPLAYERClose����ֱ��������, �����ɹ�
//@EXECUTIONFLOW:24�����¼�Ƶ��ļ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:25������һ��֧��ʱ���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:26������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:27������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:28��ѯ�ʲ�����ʱ���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:29������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:30������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:31��������ʱ���ļ��ֱ�ִ��24~30
//@EXECUTIONFLOW:32���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��ֱ����demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"֧��ֱ����demux��tuner����ʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"����¼����1ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"����¼����2ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ1:%s �ͽ�Ŀ2:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"����¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ��������·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ֱ�������쳣");

	CSTCPrint("�ȴ�¼��%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("����ֹͣ¼����·��Ŀ����ע��۲�ֱ���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec1, CSUDI_FALSE)),
			"ֹͣ¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"ֹͣ¼��2ʧ��");

	CSTCPrint("��ǰ��ֹͣ��·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����2ʧ��");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֱֹͣ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֱֹͣ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"����ֱ���Ĳ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����1ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"�Ͽ�����ֱ����demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ֱ����ĿA��ͬʱ��·¼��(ʱ��¼�ƽ�ĿA����ͨ¼�ƽ�ĿB)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:���ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:4����֧��ֱ����demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:5�����ҷ���ֱ�����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen����ֱ��ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYStart����ֱ����ĿA, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����ߵ�ǰδ����¼�ơ�ֱ����ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:11�����ڶ���֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECStart����¼�ƽ�ĿA(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:14��ѯ�ʲ�����������ʱ��¼�ƽ�ĿA��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(��ͨ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�������������ͨ¼�ƽ�ĿB��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17���ȴ�¼��10S
//@EXECUTIONFLOW:18������CSUDIRECStopֹͣ¼��������������·¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣ¼�ơ�ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIRECClose������������¼����, �����ɹ�
//@EXECUTIONFLOW:21������CSUDIPLAYERStopֱֹͣ��, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ�����ֱ���Ƿ���ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIPLAYERClose����ֱ��������, �����ɹ�
//@EXECUTIONFLOW:24�����¼�Ƶ������ļ������Ҵ�С��Ϊ0, ����ȫ���ɹ�
//@EXECUTIONFLOW:25������һ��֧��ʱ���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:26������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:27������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:28��ѯ�ʲ�����ʱ���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:29������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:30������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:31������һ��֧��¼���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:32������CSUDIPLAYEROpen����¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:33������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:34��ѯ�ʲ�����¼���ļ��Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:35������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:36������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:37���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��ֱ����demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"֧��ֱ����demux��tuner����ʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"����¼����1ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"����¼����2ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ1:%s �ͽ�Ŀ2:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"����¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ��������·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ֱ�������쳣");

	CSTCPrint("�ȴ�¼��%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("����ֹͣ¼����·��Ŀ����ע��۲�ֱ���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"ֹͣ¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"ֹͣ¼��2ʧ��");

	CSTCPrint("��ǰ��ֹͣ��·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����2ʧ��");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֱֹͣ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֱֹͣ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"����ֱ���Ĳ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����1ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"�Ͽ�����ֱ����demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ֱ����ĿA��ͬʱ¼��TS
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:���ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:3����֧��ֱ����demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:4�����ҷ���ֱ�����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����ֱ��ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYStart����ֱ����ĿA, �������سɹ�
//@EXECUTIONFLOW:7��ѯ�ʲ����ߵ�ǰδ����¼�ơ�ֱ����ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:8������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:9����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECStart����¼��(TS¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:12��ѯ�ʲ�����������TS¼�ơ�ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:13���ȴ�¼��10S
//@EXECUTIONFLOW:14������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ�������ֹͣ¼�ơ�ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:16������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:17�����¼�Ƶ��ļ������Ҵ�С��Ϊ0, �����ɹ�
//@EXECUTIONFLOW:18������CSUDIPLAYERStopֱֹͣ��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�����ֱ���Ƿ���ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIPLAYERClose����ֱ��������, �����ɹ�
//@EXECUTIONFLOW:21������һ��֧��¼���ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:22������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:23������CSUDIPLAYERStart����¼��TS�н�ĿA�Ĳ���, �������سɹ�
//@EXECUTIONFLOW:24��ѯ�ʲ����߽�ĿA�Ƿ���������, ����Ϊ��
//@EXECUTIONFLOW:25������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:26������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:27���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��ֱ����demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"֧��ֱ����demux��tuner����ʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, CSUDI_NULL)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);

	CSTCPrint("������ʼTS¼�ƣ���ע��۲�ֱ���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ������TS¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ֱ�������쳣");

	CSTCPrint("�ȴ�¼��%d(ms)\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	CSTCPrint("����ֹͣTS¼�ƣ���ע��۲�ֱ���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣTS¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֱֹͣ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֱֹͣ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"����ֱ���Ĳ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sStartPar.m_szFileName);
	CSTKWaitAnyKey();

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayerPvr) && hPlayerPvr != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayerPvr, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayerPvr)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayerPvr)), 
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"���ٲ�����ʧ��");
	hPlayerPvr = CSUDI_NULL;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hPlayerPvr != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"���ٲ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sPlayerChnl.m_nDemux)),
			"�Ͽ�����ֱ����demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�����¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����PVR�ļ��طŵ�ͬʱ����һ·��ͨ¼��(PID��ͬ/��ͬ)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�طź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:3������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:4���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:10����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIRECStart����¼�ƽ�ĿA(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�������������ͨ¼�ƽ�ĿA��PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:16������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:18������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIPLAYERSeek��λ���ļ�ͷ��ʼ��1S��λ��
//@EXECUTIONFLOW:21��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:22������CSUDIRECStart����¼�ƽ�ĿB(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:23��ѯ�ʲ�������������ͨ¼�ƽ�ĿB��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:24������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:25��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:26������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:27��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:28��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:29������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:30��ѯ�ʲ�������ֹͣ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:31������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:32������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:33������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:34�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
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
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR�طŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼPVR�طŽ�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������PVR���ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����PVR�ļ��طŵ�ͬʱ������·��ͨ¼��(PID��ͬ�Ͳ�ͬ)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�طź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:4������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:5���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:9��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:10������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:12�����ڶ���֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:14������CSUDIRECStart����¼�ƽ�ĿA(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�������������·��ͨ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:18��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:19������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:20��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:21������CSUDIRECStop�ֱ�ֹͣ��·¼��, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ�������ֹͣ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:24������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:25������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:26�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:27���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������PVR�ط��쳣");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"����¼����1ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"����¼����2ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ1:%s �ͽ�Ŀ2:%s����ע��۲첥���Ƿ���Ӱ��\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"����¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ��������·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼����·��Ŀ����ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"ֹͣ¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec2)),
			"ֹͣ¼��2ʧ��");

	CSTCPrint("��ǰ��ֹͣ��·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����2ʧ��");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������PVR���ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����1ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����PVR�ļ��طŵ�ͬʱ����һ·ʱ��¼��(PID��ͬ/��ͬ)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�طź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:3������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:4���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:10����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIRECStart����¼�ƽ�ĿA(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�����������ʱ��¼�ƽ�ĿA��PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:16������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:18������CSUDIRECStopTshiftֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣʱ��¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIPLAYERSeek��λ���ļ�ͷ��ʼ��1S��λ��
//@EXECUTIONFLOW:21��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:22������CSUDIRECStart����¼�ƽ�ĿB(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:23��ѯ�ʲ�����������ʱ��¼�ƽ�ĿB��PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:24������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:25��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:26������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:27��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:28��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:29������CSUDIRECStopTshiftֹͣʱ��¼��, �����ɹ�
//@EXECUTIONFLOW:30��ѯ�ʲ�������ֹͣ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:31������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:32������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:33������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:34�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:35���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR�طŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼPVR�طŽ�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������PVR�ط��쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"����ʱ��¼��ʧ��");

	CSTCPrint("��ǰ������ʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������ʱ��¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"ֹͣʱ��¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������PVR�ط��쳣");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 1, EM_UDIPLAYER_POSITION_FROM_HEAD)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"����ʱ��¼��ʧ��");

	CSTCPrint("��ǰ������ʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������ʱ��¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"ֹͣʱ��¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣʱ��¼������²����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������PVR���ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����PVR�ļ��طŵ�ͬʱ������·ʱ��¼��(PID��ͬ�Ͳ�ͬ)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�طź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:4������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:5���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:9��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:10������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:12������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:14������CSUDIRECStart����¼�ƽ�ĿA(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�������������·ʱ��¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:18��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:19������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:20��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:21������CSUDIRECStopTshift�ֱ�ֹͣ��·ʱ��¼��, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ�������ֹͣ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:24������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:25������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:26�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:27���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������PVR�ط��쳣");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"����¼����1ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"����¼����2ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ1:%s �ͽ�Ŀ2:%s����ע��۲첥���Ƿ���Ӱ��\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"����¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ��������·ʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������ʱ��¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼����·��Ŀ����ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec1, CSUDI_FALSE)),
			"ֹͣ¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"ֹͣ¼��2ʧ��");

	CSTCPrint("��ǰ��ֹͣ��·ʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����2ʧ��");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������PVR���ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����1ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����PVR�ļ��طŵ�ͬʱ������·¼��(��ͨ¼�ƺ�ʱ��¼��)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�طź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:4������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:5���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:9��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:10������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:12�����ڶ���֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:14������CSUDIRECStart����¼�ƽ�ĿA(��ͨ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�������������·¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:18��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:19������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:20��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:21��ֹͣ��·¼��, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ�������ֹͣ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:24������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:25������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:26�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:27���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������PVR�ط��쳣");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"����¼����1ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"����¼����2ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ1:%s �ͽ�Ŀ2:%s����ע��۲첥���Ƿ���Ӱ��\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"����¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ��������·¼��(ʱ��+��ͨ)�������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼����·��Ŀ����ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"ֹͣ¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"ֹͣ¼��2ʧ��");

	CSTCPrint("��ǰ��ֹͣ��·¼��(ʱ��+��ͨ)�������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����2ʧ��");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������PVR���ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����1ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����PVR�ļ��طŵ�ͬʱ����TS¼��
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�طź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2�������Ƿ����һ��������������PVR¼���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:3������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:4���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����PVR¼���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:10����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIRECStart����¼�ƽ�Ŀ(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�������������ͨTS¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:16������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:18������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:21������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:22������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:23�����¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:24���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR�طŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼPVR�طŽ�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);

	CSTCPrint("������ʼTS¼�ƣ���ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣTS¼�ƣ���ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������PVR���ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sStartPar.m_szFileName);
	CSTKWaitAnyKey();

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayerPvr) && hPlayerPvr != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayerPvr, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayerPvr)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayerPvr)), 
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"���ٲ�����ʧ��");
	hPlayerPvr = CSUDI_NULL;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hPlayerPvr != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"���ٲ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�demux��tuner������ʧ��");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ʱ�Ʋ��ŵ�ͬʱ����һ·��ͨ¼��(PID��ͬ/��ͬ)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:ʱ�Ʋ��ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2�������Ƿ����һ��������������ʱ���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:3������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:4���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:10����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIRECStart����¼�ƽ�ĿA(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�������������ͨ¼�ƽ�ĿA��ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:16������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:18������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣ¼�ơ�ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIPLAYERSeek��λ����ǰλ�ú�30S��λ��
//@EXECUTIONFLOW:21��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:22������CSUDIRECStart����¼�ƽ�ĿB(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:23��ѯ�ʲ�������������ͨ¼�ƽ�ĿB��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:24������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:25��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:26������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:27��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:28��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:29������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:30��ѯ�ʲ�������ֹͣ¼�ơ�ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:31������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:32������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:33������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:34�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:35���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼʱ�Ʋ��Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������PVR���ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ʱ�Ʋ��ŵ�ͬʱ������·��ͨ¼��(PID��ͬ�Ͳ�ͬ)
//@PRECONDITION:1��Demuxģ��������ʼ��
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:ʱ�Ʋ��ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3�������Ƿ����һ��������������ʱ���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:4������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:5���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:9��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:10������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:12�����ڶ���֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:14������CSUDIRECStart����¼�ƽ�ĿA(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�������������·��ͨ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:18��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:19������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:20��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:21������CSUDIRECStop�ֱ�ֹͣ��·¼��, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ�������ֹͣ¼�ơ�PVR�ط��Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:24������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:25������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:26�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:27���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ʱ�Ʋ����쳣");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"����¼����1ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"����¼����2ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ1:%s �ͽ�Ŀ2:%s����ע��۲첥���Ƿ���Ӱ��\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"����¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ��������·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼����·��Ŀ����ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"ֹͣ¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec2)),
			"ֹͣ¼��2ʧ��");

	CSTCPrint("��ǰ��ֹͣ��·¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����2ʧ��");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������PVR���ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����1ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[2])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ʱ�Ʋ��ŵ�ͬʱ����һ·ʱ��¼��(PID��ͬ/��ͬ)
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�طź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2�������Ƿ����һ��������������ʱ���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:3������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:4���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:10����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIRECStart����¼�ƽ�ĿA(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�����������ʱ��¼�ƽ�ĿA��ʱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:16������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:18������CSUDIRECStopTshiftֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣʱ��¼�ơ�ʱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIPLAYERSeek��λ����ǰλ�ú�30S��λ��
//@EXECUTIONFLOW:21��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:22������CSUDIRECStart����¼�ƽ�ĿB(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:23��ѯ�ʲ�����������ʱ��¼�ƽ�ĿB��ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:24������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:25��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:26������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:27��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:28��ѯ�ʲ����߲���λ���Ƿ��ѱ仯, ����Ϊ��
//@EXECUTIONFLOW:29������CSUDIRECStopTshiftֹͣʱ��¼��, �����ɹ�
//@EXECUTIONFLOW:30��ѯ�ʲ�������ֹͣ¼�ơ�ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:31������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:32������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:33������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:34�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:35���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ʱ�ƵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼʱ�Ʋ��Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ʱ�Ʋ����쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam1)),
			"����ʱ��¼��ʧ��");

	CSTCPrint("��ǰ������ʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������ʱ��¼�������ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"ֹͣʱ��¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ʱ�Ʋ����쳣");

	CSTCPrint("���������ò���λ�ã���ע��۲첥��λ���Ƿ�����ת\n");
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSeek(hPlayer, 30, EM_UDIPLAYER_POSITION_FROM_CURRENT)), 
			"���ò���λ��ʧ��");

	CSTCPrint("�����ò���λ�ã���ȷ�ϲ����Ƿ�����ת����������\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���ò���λ�ò�����");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sRecStartParam2)),
			"����ʱ��¼��ʧ��");

	CSTCPrint("��ǰ������ʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������ʱ��¼�������ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼�ƽ�Ŀ:%s����ע��۲첥���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"ֹͣʱ��¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣʱ��¼������²����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������PVR���ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;	

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ʱ�Ʋ��ŵ�ͬʱ������·ʱ��¼��(PID��ͬ�Ͳ�ͬ)
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:ʱ�Ʋ��ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3�������Ƿ����һ��������������ʱ���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:4������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:5���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:9��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:10������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:12������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:14������CSUDIRECStart����¼�ƽ�ĿA(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�������������·ʱ��¼�ơ�ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:18��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:19������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:20��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:21������CSUDIRECStopTshift�ֱ�ֹͣ��·ʱ��¼��, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ�������ֹͣ¼�ơ�ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:24������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:25������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:26�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:27���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ʱ�Ʋ����쳣");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"����¼����1ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"����¼����2ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ1:%s �ͽ�Ŀ2:%s����ע��۲첥���Ƿ���Ӱ��\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"����¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ��������·ʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������ʱ��¼�������ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼����·��Ŀ����ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec1, CSUDI_FALSE)),
			"ֹͣ¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"ֹͣ¼��2ʧ��");

	CSTCPrint("��ǰ��ֹͣ��·ʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����2ʧ��");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣPVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����1ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ʱ�Ʋ��ŵ�ͬʱ������·¼��(��ͨ¼�ƺ�ʱ��¼��)
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:ʱ�Ʋ��ź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1����������֧��¼�Ƶ�demux, �����ɹ��Ҹ������ڵ���2
//@EXECUTIONFLOW:2��������������, �����ɹ�
//@EXECUTIONFLOW:3�������Ƿ����һ��������������ʱ���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:4������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:5���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:8������CSUDIPLAYERStart����ʱ���ļ��Ĳ���, �����ɹ�
//@EXECUTIONFLOW:9��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:10������һ��֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����1, �����ɹ�
//@EXECUTIONFLOW:12�����ڶ���֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECOpen����¼����2, �����ɹ�
//@EXECUTIONFLOW:14������CSUDIRECStart����¼�ƽ�ĿA(��ͨ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:15������CSUDIRECStart����¼�ƽ�ĿB(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:16��ѯ�ʲ�������������·¼�ơ�ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:17������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:18��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:19������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:20��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:21��ֹͣ��·¼��, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ�������ֹͣ¼�ơ�ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:24������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:25������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:26�������·¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:27���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��������֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(nActualCnt >= 2),
			"���ҵ���֧��¼�Ƶ�demux��������Ϊ2���ܼ�������Ĳ���");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(dwTunerId, s_sServiceInfo[EM_RECORD_H264_HD].m_pcTsFilename)),
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ʱ�Ʋ����쳣");

	sRecChnl.m_nDemux = anDemuxId[0];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"��һ��֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec1) && hRec1 != CSUDI_NULL),
			"����¼����1ʧ��");

	sRecChnl.m_nDemux = anDemuxId[1];
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�ڶ���֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec2) && hRec2 != CSUDI_NULL),
			"����¼����2ʧ��");

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

	CSTCPrint("������ʼ¼�ƽ�Ŀ1:%s �ͽ�Ŀ2:%s����ע��۲첥���Ƿ���Ӱ��\n", 
			s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript, s_sServiceInfo[EM_RECORD_H264_MPEG1].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec1, &sRecStartParam1)),
			"����¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec2, &sRecStartParam2)),
			"����¼��1ʧ��");

	CSTCPrint("��ǰ��������·¼��(ʱ��+��ͨ)�������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣ¼����·��Ŀ����ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec1)),
			"ֹͣ¼��1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec2, CSUDI_FALSE)),
			"ֹͣ¼��2ʧ��");

	CSTCPrint("��ǰ��ֹͣ��·¼��(ʱ��+��ͨ)�������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	hRec1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����2ʧ��");
	hRec2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam1.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam1)),
			"¼���ļ�1��鲻ͨ��");

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sRecStartParam2.m_szFileName);
	CSTKWaitAnyKey();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CheckRecResult(sRecStartParam2)),
			"¼���ļ�2��鲻ͨ��");
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hRec1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec1)),
			"����¼����1ʧ��");
	}

	if (hRec2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec2)),
			"����¼����1ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[0])),
			"�Ͽ���һ��֧��¼�Ƶ�demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, anDemuxId[1])),
			"�Ͽ��ڶ���֧��¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sRecStartParam1.m_szFileName);
	ClearFile(sRecStartParam2.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:����ʱ�Ʋ��ŵ�ͬʱ����TS¼��
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:�Ϸ�����
//@EXPECTATION:�طź�¼�ƻ���Ӱ��
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2�������Ƿ����һ��������������ʱ���ļ�, �������������¼���ٽ������沽��
//@EXECUTIONFLOW:3������һ��֧��PVR�ļ����ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:4���������ڲ��ŵ���Ƶ����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����ʱ���ļ�����ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYERSetStream���ý�Ŀ��Ϣ, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERStart����¼���ļ��Ĳ���, �������سɹ�
//@EXECUTIONFLOW:8��ѯ�ʲ����߲����Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:9������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:10����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:12������CSUDIRECStart����¼�ƽ�Ŀ(��Ŀ¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:13��ѯ�ʲ�������������ͨTS¼�ơ�ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:14������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:15��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:16������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:17��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:18������CSUDIRECStopֹͣ¼��, �����ɹ�
//@EXECUTIONFLOW:19��ѯ�ʲ�������ֹͣ¼�ơ�ʱ�Ʋ����Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:20������CSUDIRECClose����¼����, �����ɹ�
//@EXECUTIONFLOW:21������CSUDIPLAYERStopֹͣ����, �����ɹ�
//@EXECUTIONFLOW:22������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:23�����¼�Ƶ��ļ��Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:24���Ͽ��������������ӵ�demux��tuner, �����ɹ�
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
			"��Ƶʧ��");

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
			"û�п������ڲ��Ե��ļ�");
	
	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(asStreamInfo[0].m_uStreamType.m_eAudioType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(asStreamInfo[1].m_uStreamType.m_eVideoType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hPlayer) && hPlayer != CSUDI_NULL),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼʱ�Ʋ��Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�ѿ�ʼ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("��ǰ��δ����¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);

	CSTCPrint("������ʼTS¼�ƣ���ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������PVR�ط��쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTCPrint("����ֹͣTS¼�ƣ���ע��۲첥���Ƿ���Ӱ��\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStop(hRec)),
			"ֹͣ¼��ʧ��");

	CSTCPrint("��ǰ��ֹͣ¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ֹͣ¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("��ֹͣ���Ž�Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");
	hPlayer = CSUDI_NULL;

	CSTCPrint("��ʼ���¼���ļ�:%s\n", sStartPar.m_szFileName);
	CSTKWaitAnyKey();

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_PVR, &hPlayerPvr) && hPlayerPvr != CSUDI_NULL),
			"��������PVR���ŵĲ�����ʧ��");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hPlayerPvr, asStreamInfo, 2, &sPlaybackPar)),
			"���ò��Ž�Ŀ��Ϣʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hPlayerPvr)), 
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("�����Ƿ�����\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"���Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hPlayerPvr)), 
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"���ٲ�����ʧ��");
	hPlayerPvr = CSUDI_NULL;
	
	CSTK_FATAL_POINT;

	if (hPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayer)),
			"���ٲ�����ʧ��");
	}

	if (hPlayerPvr != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hPlayerPvr)),
			"���ٲ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�demux��tuner������ʧ��");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:ģ���Զ�ʱ��(ֱ����ĿA+ʱ��¼�ƽ�ĿA -> ֱֹͣ��+ʱ��¼�� -> ʱ�Ʋ���+ʱ��¼��)
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:1��¼������δ�ﵽ����޶���С
//@INPUT:2��¼�������ѳ�������޶���С
//@EXPECTATION:
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:3����֧��ֱ����demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:4�����ҷ���ֱ�����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����ֱ��ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ò��Ų���, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYStart����ֱ����ĿA, �������سɹ�
//@EXECUTIONFLOW:7��ѯ�ʲ�����ֱ����ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:8������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:9����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:10������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:11������CSUDIRECStart����¼�ƽ�ĿA(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:12��ѯ�ʲ�����������ʱ��¼�ƽ�ĿA��ֱ���Ƿ�����������, ����Ϊ��
//@EXECUTIONFLOW:13������CSUDIPLAYERStopֱֹͣ��, �����ɹ�
//@EXECUTIONFLOW:14��ѯ�ʲ�����ֱ���Ƿ�������ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:15������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:16���ȴ�¼��30s
//@EXECUTIONFLOW:17������һ��֧��PVR���ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:18������CSUDIPLAYEROpen����ʱ�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:19������CSUDIPLAYERSetStream���ò��Ų���, �����ɹ�
//@EXECUTIONFLOW:20������CSUDIPLAYStart����ʱ�Ʋ��Ž�ĿA, �����ɹ�
//@EXECUTIONFLOW:21��ѯ�ʲ�����ʱ�Ʋ��Ž�ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:22������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:23��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:24������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:25��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:26������CSUDIPLAYERStopֹͣʱ�Ʋ���, �����ɹ�
//@EXECUTIONFLOW:27������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:28������CSUDIRECStopֹͣ¼��
//@EXECUTIONFLOW:29������CSUDIRECClose����¼����
//@EXECUTIONFLOW:30���Ͽ��������������ӵ�tuner��demux
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
			"��Ƶʧ��");
	
	nDmxForLive = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((nDmxForLive >= 0), "��ȡ֧��ֱ����demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, nDmxForLive)),
			"֧��ֱ����demux��tuner����ʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nDemux = nDmxForLive;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hLivePlayer) && hLivePlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hLivePlayer, asStreamInfo, 2, CSUDI_NULL)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hLivePlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("��ǰΪֱ���������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"δ����¼�������ֱ�������쳣");

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);

	CSTCPrint("������ʼʱ��¼�ƽ�Ŀ:%s����ע��۲�ֱ���Ƿ���Ӱ��\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"����¼��ʧ��");

	CSTCPrint("��ǰ������ʱ��¼�ƣ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"������¼�������ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hLivePlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣ�����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
			"����ֱ��������ʧ��");
	hLivePlayer = CSUDI_NULL;

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hTshiftPlayer) && hTshiftPlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hTshiftPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("����ʼʱ�Ʋ��Ž�Ŀ:%s, ��ע��۲�\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hTshiftPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("��ǰΪʱ�Ʋ��ţ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hTshiftPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hTshiftPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hTshiftPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hTshiftPlayer)),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");
	hTshiftPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"ֹͣ¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
			"����ֱ��������ʧ��");
	}

	if (hTshiftPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hTshiftPlayer)),
			"����ʱ�Ʋ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, nDmxForLive)),
			"�Ͽ�ֱ����demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIRECOpen, CSUDIRECClose, CSUDIRECStart, CSUDIRECStop, CSUDIPLAYEROpen, CSUDIPLAYERClose, CSUDIPLAYERStart, CSUDIPLAYERStop
//@DESCRIPTION:ģ���ֶ�ʱ��(ֱ����ĿA -> ֱֹͣ�� -> ʱ��¼�� -> ʱ�Ʋ���+ʱ��¼��)
//@PRECONDITION:1��Demuxģ��������ʼ���Ҵ���֧��¼�Ƶ�demux
//@PRECONDITION:2��Recordģ��������ʼ��
//@PRECONDITION:3��Playerģ��������ʼ��
//@INPUT:1��¼������δ�ﵽ����޶���С
//@INPUT:2��¼�������ѳ�������޶���С
//@EXECUTIONFLOW:1��������������, �����ɹ�
//@EXECUTIONFLOW:2������һ��֧��ֱ����demux, �����ɹ�
//@EXECUTIONFLOW:3����֧��ֱ����demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:4�����ҷ���ֱ�����������Ƶ������, �����ɹ�
//@EXECUTIONFLOW:5������CSUDIPLAYEROpen����ֱ��ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:7������CSUDIPLAYERSetStream���ò��Ų���, �����ɹ�
//@EXECUTIONFLOW:6������CSUDIPLAYStart����ֱ����ĿA, �������سɹ�
//@EXECUTIONFLOW:7��ѯ�ʲ�����ֱ����ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:8������CSUDIPLAYERStopֱֹͣ��, �����ɹ�
//@EXECUTIONFLOW:9��ѯ�ʲ�����ֱ���Ƿ�������ֹͣ, ����Ϊ��
//@EXECUTIONFLOW:10������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:11������һ��֧��¼�Ƶ�demux, �����ɹ�
//@EXECUTIONFLOW:12����֧��¼�Ƶ�demux��tuner����, �����ɹ�
//@EXECUTIONFLOW:13������CSUDIRECOpen����¼����, �����ɹ�
//@EXECUTIONFLOW:14������CSUDIRECStart����¼�ƽ�ĿA(ʱ��¼�ơ���׷�ӡ�����), �����ɹ�
//@EXECUTIONFLOW:15���ȴ�¼��30s
//@EXECUTIONFLOW:16������һ��֧��PVR���ŵ�demux, �����ɹ�
//@EXECUTIONFLOW:17������CSUDIPLAYEROpen����ʱ�Ʋ���ģʽ�Ĳ�����, �����ɹ�
//@EXECUTIONFLOW:18������CSUDIPLAYERSetStream���ò��Ų���, �����ɹ�
//@EXECUTIONFLOW:19������CSUDIPLAYStart����ʱ�Ʋ��Ž�ĿA, �����ɹ�
//@EXECUTIONFLOW:20��ѯ�ʲ�����ʱ�Ʋ��Ž�ĿA�Ƿ�����, ����Ϊ��
//@EXECUTIONFLOW:21������CSUDIPLAYERPause��ͣ����, �����ɹ�
//@EXECUTIONFLOW:22��ѯ�ʲ����߲����Ƿ�����ͣ, ����Ϊ��
//@EXECUTIONFLOW:23������CSUDIPLAYERResume�ָ�����, �����ɹ�
//@EXECUTIONFLOW:24��ѯ�ʲ����߲����Ƿ��ѻָ�, ����Ϊ��
//@EXECUTIONFLOW:25������CSUDIPLAYERStopֹͣʱ�Ʋ���, �����ɹ�
//@EXECUTIONFLOW:26������CSUDIPLAYERClose���ٲ�����, �����ɹ�
//@EXECUTIONFLOW:27������CSUDIRECStopֹͣ¼��
//@EXECUTIONFLOW:28������CSUDIRECClose����¼����
//@EXECUTIONFLOW:29���Ͽ��������������ӵ�tuner��demux
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
			"��Ƶʧ��");
	
	nDmxForLive = FindDemuxSupportLivePlay();
	CSTK_ASSERT_TRUE_FATAL((nDmxForLive >= 0), "��ȡ֧��ֱ����demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, nDmxForLive)),
			"֧��ֱ����demux��tuner����ʧ��");

	sPlayerChnl.m_nAudioDecoder = FindAudioDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nAudioDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nVideoDecoder = FindVideoDecoder(s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType);
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nVideoDecoder >= 0), "��ȡ���ڲ��ŵ���Ƶ������ʧ��");

	sPlayerChnl.m_nDemux = nDmxForLive;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_LIVE, &hLivePlayer) && hLivePlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	asStreamInfo[0].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nAudioPid;
	asStreamInfo[0].m_eContentType = EM_UDI_CONTENT_AUDIO;
	asStreamInfo[0].m_uStreamType.m_eAudioType = s_sServiceInfo[EM_RECORD_H264_HD].m_eAudStreamType;
	asStreamInfo[1].m_nPid = s_sServiceInfo[EM_RECORD_H264_HD].m_nVideoPid;
	asStreamInfo[1].m_eContentType = EM_UDI_CONTENT_VIDEO;
	asStreamInfo[1].m_uStreamType.m_eVideoType = s_sServiceInfo[EM_RECORD_H264_HD].m_eVidStreamType;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hLivePlayer, asStreamInfo, 2, CSUDI_NULL)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("��ʼֱ����Ŀ:%s\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hLivePlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("��ǰΪֱ���������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֱ�������쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hLivePlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣ�����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
			"����ֱ��������ʧ��");
	hLivePlayer = CSUDI_NULL;

	sRecChnl.m_nDemux = FindDemuxSupportRec();
	CSTK_ASSERT_TRUE_FATAL((sRecChnl.m_nDemux >= 0), "��ȡ֧��¼�Ƶ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"֧��¼�Ƶ�demux��tuner����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECOpen(&sRecChnl, &hRec) && hRec != CSUDI_NULL),
			"����¼����ʧ��");

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
	CSTCPrint("¼���ļ���: %s\n", sStartPar.m_szFileName);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStart(hRec, &sStartPar)),
			"����¼��ʧ��");

	CSTCPrint("�ȴ�¼��%dms\n", nRecTime);
	CSUDIOSThreadSleep(nRecTime);

	sPlayerChnl.m_nDemux = FindDemuxSupportPvrPlay();
	CSTK_ASSERT_TRUE_FATAL((sPlayerChnl.m_nDemux >= 0), "��ȡ֧��PVR���ŵ�demuxʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYEROpen(&sPlayerChnl, EM_UDIPLAYER_TSHIFT, &hTshiftPlayer) && hTshiftPlayer != CSUDI_NULL),
			"��������ֱ���Ĳ�����ʧ��");

	memset(&sPlaybackPar, 0, sizeof(sPlaybackPar));
	memcpy(sPlaybackPar.m_szFileName, sStartPar.m_szFileName, CSUDI_MAX_FILE_NAME_LEN);
	sPlaybackPar.m_nSecondPos = 0;
	sPlaybackPar.m_eSpeed = EM_UDIPLAYER_SPEED_NORMAL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERSetStream(hTshiftPlayer, asStreamInfo, 2, &sPlaybackPar)),
			"���ý�Ŀ��Ϣʧ��");

	CSTCPrint("����ʼʱ�Ʋ��Ž�Ŀ:%s, ��ע��۲�\n", s_sServiceInfo[EM_RECORD_H264_HD].m_pcServiceDescript);
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStart(hTshiftPlayer)),
			"��������ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_TRUE);

	CSTCPrint("��ǰΪʱ�Ʋ��ţ������Ƿ�����?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERPause(hTshiftPlayer)), 
			"��ͣ����ʧ��");

	CSTCPrint("����ͣ���ţ���ȷ�ϲ����Ƿ�����ͣ\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"��ͣ���Ų�����");

	CSTCPrint("���ָ����ţ���ע��۲첥���Ƿ����ͣǰ�����ν�\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERResume(hTshiftPlayer)), 
			"�ָ�����ʧ��");

	CSTCPrint("�ѻָ����ţ���ȷ�ϲ����Ƿ��ѻָ��Һ���ͣǰ�����ν�\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"�ָ����Ų�����");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERStop(hTshiftPlayer)),
			"ֹͣ����ʧ��");

	CSUDIVIDEOShow(sPlayerChnl.m_nVideoDecoder, CSUDI_FALSE);

	CSTCPrint("�����Ƿ���ֹͣ?\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTKWaitYes()),
			"ֹͣʱ�Ʋ����쳣");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hTshiftPlayer)),
			"��������ʱ�Ʋ��ŵĲ�����ʧ��");
	hTshiftPlayer = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECStopTshift(hRec, CSUDI_FALSE)),
			"ֹͣ¼��ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	hRec = CSUDI_NULL;

	CSTK_FATAL_POINT;

	if (hLivePlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hLivePlayer)),
			"����ֱ��������ʧ��");
	}

	if (hTshiftPlayer != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIPLAYERClose(hTshiftPlayer)),
			"����ʱ�Ʋ�����ʧ��");
	}

	if (hRec != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIRECClose(hRec)),
			"����¼����ʧ��");
	}

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, nDmxForLive)),
			"�Ͽ�ֱ����demux��tuner������ʧ��");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(dwTunerId, sRecChnl.m_nDemux)),
			"�Ͽ�¼�Ƶ�demux��tuner������ʧ��");

	ClearFile(sStartPar.m_szFileName);
	
	return CSUDI_TRUE;
}

