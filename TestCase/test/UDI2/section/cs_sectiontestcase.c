/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_sectiontestcase.h"
#include "cs_sectiontest.h"
#include "udi2_demux.h"
#include "udi2_player.h"
#include "udi2_inject.h"
#include "udi2_fs.h"
#include "udi2_section.h"
#include "udi2_tuner.h"
#include "../cs_udi2testcase.h"

#define MAX_FILTER  			(100)   //����demux֧�ֵ�filter����
#define MBYTE              (0x100000)  // 1M
#define EPG_TEST_ORIGNETWORKID (0X4080)
#define EPG_TEST_TSID   (0xffff)
#define EPG_TEST_SERVID (0xffff)

#define EPG_0606_121458_TS  "EPG_0606_121458.ts"    ///"EPG_0606_121458.ts"   //10.10.96.61:/f:/test_epg
#define TELETEXT_SUBTITLE_25MBPS_071220_TS "Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts" //���� //10.10.96.61:/f:/test_epg
#define OC_TEST_TS "oc_test.ts"	 //10.10.96.61:/f:/test_epg
#define COSHIP_ADS_SECTION_TS "coship_ads_section.ts"
#define PES_PERFORMANCE_TEST_TS "PES_performance_test.ts"
#define EPG_0606_121458_TS_FOR_INJECT "testdata/EPG_0606_121458.ts"

#define CSMaxSectionLength (1024*4)
static char *g_pcSectionUserBuff = NULL;

extern CSUDI_HANDLE g_hUDISectionTestFilter;


CSUDI_BOOL CSTC_SECTION_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	SECTION_iTestInit();
	
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_SECTION_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

static void CSSectionCallback_common(CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	return ;
}

//@CASEGROUP:���в�������
//@DESCRIPTION:�������������������о٣�������ÿ��Filterȡ�Ĳ���ֵ
//@PRECONDITION: �������в���������������ο��������е���������
//@PRECONDITION: ����ִ��Ŀ¼�µ�testdata/��д��һ����������Ƶ���������Ϣ
//@PRECONDITION: 1�����������ļ�EPG_0606_121458.ts
//@PRECONDITION: 2�����������ļ�Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts
//@PRECONDITION: 3�����������ļ�coship_oc_1M.ts
//@INPUT:PAT_FILTER001: �ɻ�ȡ���ݣ����Ϊ1
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00
//@INPUT: MASK  :0xff
//@INPUT: NEGATE:0x00
//@INPUT:------------------------------
//@INPUT:PAT_FILTER002: �ɻ�ȡ���ݣ����Ϊ1
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0xff
//@INPUT: MASK  :0xff
//@INPUT: NEGATE:0x01
//@INPUT:------------------------------
//@INPUT:PAT_FILTER003: ���ɻ�ȡ���ݣ����Ϊ1
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00
//@INPUT: MASK  :0xff
//@INPUT: NEGATE:0x01
//@INPUT:------------------------------
//@INPUT:PAT_FILTER004: �ɻ�ȡ���ݣ����Ϊ3
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b 
//@INPUT: MASK  :0xff,0xff,0xff 
//@INPUT: NEGATE:0x00,0x00,0x00 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER005: �ɻ�ȡ���ݣ����Ϊ4
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7 
//@INPUT: MASK  :0xff,0xff,0xff,0xff 
//@INPUT: NEGATE:0x00,0x00,0x00,0x00 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER006: �ɻ�ȡ���ݣ����Ϊ6
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER007: ���ɻ�ȡ���ݣ����Ϊ6
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x01 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER008: �ɻ�ȡ���ݣ����Ϊ8
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER009: ���ɻ�ȡ���ݣ����Ϊ8
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER010: �ɻ�ȡ���ݣ����Ϊ10
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:PAT_FILTER011: ���ɻ�ȡ���ݣ����Ϊ10
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
//@INPUT:------------------------------
//@INPUT:PAT_FILTER012: �ɻ�ȡ���ݣ����Ϊ12
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:PAT_FILTER013: ���ɻ�ȡ���ݣ����Ϊ12
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
//@INPUT:------------------------------
//@INPUT:PAT_FILTER014: �ɻ�ȡ���ݣ����Ϊ14
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70,0xe4,0x65 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:PAT_FILTER015: ���ɻ�ȡ���ݣ����Ϊ14
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70,0xe4,0x65 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
//@INPUT:------------------------------
//@INPUT:CAT_FILTER001:
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 1
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x01
//@INPUT: MASK  :0xff
//@INPUT: NEGATE:0x00
//@INPUT:------------------------------
//@INPUT:TDT_FILTER001:
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0x14
//@INPUT: CRCCheck: 0
//@INPUT: MACTH :0x70
//@INPUT: MASK  :0xff
//@INPUT: NEGATE:0x00
//@INPUT:------------------------------
//@INPUT:EIT_FILTER001: ��ǰ��������(other)�����Ϊ3
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0x12
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x4f,0x00,0x66
//@INPUT: MASK  :0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:EIT_FILTER002:��ǰ��������(other)�����Ϊ5
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0x12
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x4f,0x00,0x66,0xc1,0x00
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:EIT_FILTER003: ��ǰ��������(other)�����Ϊ5
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0x12
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x4f,0x00,0x66,0xc1,0x01
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:EIT_FILTER004: schedule����(other)
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0x12
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x60
//@INPUT: MASK  :0xf0
//@INPUT: NEGATE:0x00
//@INPUT:------------------------------
//@INPUT:VIDEO_FILTER001:
//@INPUT: ReqType: PES_DATA
//@INPUT: PID: 0x155ae
//@INPUT:------------------------------
//@INPUT:VIDEO_FILTER002:
//@INPUT: ReqType: PES_DATA
//@INPUT: PID: 0x15b8
//@INPUT:------------------------------
//@INPUT:AUDIO_FILTER001:
//@INPUT: ReqType: PES_DATA
//@INPUT: PID: 0x15af
//@INPUT:------------------------------
//@INPUT:AUDIO_FILTER002:
//@INPUT: ReqType: PES_DATA
//@INPUT: PID: 0x15b1
//@INPUT:------------------------------
//@INPUT:�������ݾ���EPG_0606_121458.ts�����з������
//@INPUT:SUBTITLE_FILTER001:
//@INPUT: ReqType: PES_DATA
//@INPUT: PID: 0x401
//@INPUT:------------------------------
//@INPUT:TELETEXT_FILTER001:
//@INPUT: ReqType: PES_DATA
//@INPUT: PID: 0xa84
//@INPUT:------------------------------
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0000(void)
{
	extern void SECTION_IT_0016_Filter_Thread1(void*arg);
	extern void SECTION_IT_0016_Filter_Thread2(void * pvArg);
	extern void SECTION_IT_0016_Filter_Thread3(void * pvArg);
	CSTC_UDI2PortingLock(0,EPG_0606_121458_TS);
	SECTION_IT_0016_Filter_Thread1(CSUDI_NULL);
	CSTKWaitYes();
	SECTION_IT_0016_Filter_Thread2(CSUDI_NULL);
	CSTKWaitYes();
	SECTION_IT_0016_Filter_Thread3(CSUDI_NULL);
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERAllocate
//@DESCRIPTION: ���Ը���������Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: ��
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��Filter�����ṹ��ָ��ΪCSUDI_NULL������Ϊ�Ϸ�ֵ
//@INPUT: 2���ص�����ָ��ΪCSUDI_NULL������Ϊ�Ϸ�ֵ
//@INPUT: 3��Filter������ΪCSUDI_NUL������Ϊ�Ϸ�ֵ
//@INPUT: 4��Filter����ָ�롢�ص�����ָ�롢Filter��������ΪCSUDI_NULL�������
//@EXPECTATION:ÿ�����������ʧ��
//@EXECUTIONFLOW: 1������CSUDIFILTERAllocate(CSUDI_NULL,�Ϸ�ָ��,CSUDI_NULL,�Ϸ�ָ��)��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate(�Ϸ�ָ��,CSUDI_NULL,CSUDI_NULL,�Ϸ�ָ��),������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate(�Ϸ�ָ��,�Ϸ�ָ��,CSUDI_NULL,CSUDI_NULL),������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIFILTERAllocate(CSUDI_NULL,CSUDI_NULL,CSUDI_NULL,CSUDI_NULL),������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0001(void)
{
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIFILTERAllocate(CSUDI_NULL,SECTION_iCallback_1,CSUDI_NULL,&hFilter)),
			"Step 1 Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID)),
			"Set ReqestFilter Failuer !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIFILTERAllocate(&sTmpReqInfo,CSUDI_NULL,CSUDI_NULL,&hFilter)),
			"Step 2 Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIFILTERAllocate(&sTmpReqInfo,SECTION_iCallback_1,CSUDI_NULL,CSUDI_NULL)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIFILTERAllocate(CSUDI_NULL,CSUDI_NULL,CSUDI_NULL,CSUDI_NULL)),
			"Step 4 Failure !!!"
		);	
	
	CSTK_FATAL_POINT;
		
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERAllocate
//@DESCRIPTION: ���Կ��Է���ָ��demux֧�ֵ�filter����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��CSUDIDEMUXGetCapability������ȷ��ȡ������Ϣ
//@PRECONDITION: 3��CSUDIDEMUXGetFreeFilterCount������ȷ��ȡ���и���
//@PRECONDITION: 4��CSUDIFILTERFree�ܳɹ��ͷ�һ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INOUT: 3���û�����ȡ��ͬ��ֵ
//@INOUT: 4��demuxIndex = 0
//@EXPECTATION: ���Գɹ�����õ�demux֧�ֵ�filter����
//@REMARK: ��������������ע���뵽��filter�Ƿ���Խ��յ����ݣ����ҷ����ÿ��filterֵ����ͬ��
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCapability��ȡdemux 0 ��filter�������������DemuxCount(��������0)��filter
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3���ظ�����2(DemuxCount-1)�Σ�����ÿһ�ζ�����ɹ�
//@EXECUTIONFLOW: 4������CSUDIDEMUXGetFreeFilterCount��ȡPAT_FILTER001��ָ��demux���е�filter����������������Ϊ0
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷŵ�һ��PAT_FILTER001����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIDEMUXGetFreeFilterCount��ȡPAT_FILTER001��ָ��demux���е�filter���������������� = �ͷ��ܸ���
//@EXECUTIONFLOW: 7���ظ�5��6����(DemuxCount-1)�Σ�����ÿ�ζ��ͷųɹ�
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0002(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nIndex = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*һ��ֻ��32��filter*/
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCapability(sTmpReqInfo.m_nDemuxID, &sTmpCap),
			"Step 1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nTotalFilteCount = sTmpCap.m_nFilterNum) > 0,
			"Step 1 failure ! Get Demux filter count < 0 !!"
		);
	
	CSTCPrint("DEMUX_0 has filters( %d )\n",nTotalFilteCount);
		
	for(; (nIndex < nTotalFilteCount) && (nIndex < MAX_FILTER); nIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIFILTERAllocate(&sTmpReqInfo,SECTION_iCallback_1,(void*)nIndex,&ahFilter[nIndex])),
				"Step 3 Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_NULL != ahFilter[nIndex]),
				"Step 3 Allocate filter failure !!!"
			);
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(sTmpReqInfo.m_nDemuxID, &nFreeFilteCount)),
			"Step 4 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nFreeFilteCount == 0),
			"Step 4 Free Filter should be 0 !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERFree(ahFilter[0])),
			"Step 5 Free Filter failure !!!"
		);
	
	ahFilter[0] = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(sTmpReqInfo.m_nDemuxID, &nFreeFilteCount)),
			"Step 6 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nFreeFilteCount == 1),
			"Step 6 Free filters should be 1 !!!"
		);

	//���ڴ˴���Ϊ�˱�֤�������ͷ�����filter
	//�����step3-step5֮��ʧ�ܣ�filterid == 0��filter�Ͳ��ᱻ�ͷŵ�
	//�����ͷ���Դ��ȻӦ�ô�0��ʼ����if(CSUDI_NULL != ahFilter[nIndex])
	//���õ����ظ��ͷ���Դ���³�������
	CSTK_FATAL_POINT;
		
	for(nFreeFilteCount = 0,nIndex = 0; (nIndex < nTotalFilteCount) && (nIndex < MAX_FILTER); nIndex++)
	{
		if(CSUDI_NULL != ahFilter[nIndex])
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIFILTERFree(ahFilter[nIndex])),
					"Step 5 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(sTmpReqInfo.m_nDemuxID, &nFreeFilteCount)),
					"Step 6 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(nFreeFilteCount  == (nIndex + 1)),
					"Step 6 failure !!!"
				);
			nFreeFilteCount = 0;
		}
	}
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFILTERAllocate
//@DESCRIPTION:���Դ�����û�����ָ��ΪCSUDI_NULL
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3��CSUDIFILTERStart�ܳɹ�����һ��filter
//@PRECONDITION: 4��CSUDIFILTERStop�ܳɹ�ֹͣһ��filter
//@PRECONDITION: 5��CSUDIFILTERFree�ܳɹ��ͷ�һ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���û�����ָ��Ϊ�Ϸ�ֵCSUDI_NULL
//@INPUT: 3���Ϸ��Ļص�����
//@INOUT: 4��demuxIndex = 0
//@EXPECTATION: �ܹ��ɹ�����filter���ܹ��ɹ���������
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ�������ݣ����������ɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪPAT_FILTER001ָ��������,���û�����Ӧ��ΪCSUDI_NULL
//@EXECUTIONFLOW: 5���������4�������ݳ�ʱ���������ʧ��
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ�PAT_FILTER001�������ɹ�CSUDI_SUCCESS
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0003(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	extern CSUDI_BOOL g_bUserDataIsNUll;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
	SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_001,CSUDI_FALSE,CSUDI_FALSE);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERAllocate(&sTmpReqInfo,SECTION_iCallback_UserDataNULL,CSUDI_NULL,&hFilter)),
			"Step 2 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERStart(hFilter)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus_UserDataNULL(CSUDI_TRUE,&sTmpReqInfo,CSUDI_NULL,CSUDI_TRUE) && g_bUserDataIsNUll == CSUDI_TRUE),
			"Step 4 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			( g_bUserDataIsNUll == CSUDI_TRUE),
			"The UserData Should be CSUDI_NULL!!!"
		);

	CSTK_FATAL_POINT
		
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 5 Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFILTERAllocate
//@DESCRIPTION:���Կ�����ȷ�õ�������û�����(����ϵͳ������demux)
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ����ӺͶϿ�tuner����ȡ����
//@PRECONDITION: 3��CSUDIFILTERStart�ܳɹ�����һ��filter
//@PRECONDITION: 4��CSUDIFILTERStop�ܳɹ�ֹͣһ��filter
//@PRECONDITION: 5��CSUDIFILTERFree�ܳɹ��ͷ�һ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���û�����Ϊ���� 0 
//@INPUT: 3���Ϸ��Ļص�����
//@EXPECTATION: ������ȷ��ȡ�û�����,����ÿ��demux�Ĳ��Զ���������ֵ
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount��ȡdemux����
//@EXECUTIONFLOW: 2������CSUDIDEMUXGetCapability��ȡDemuxIndex֧�ֵ�filter����filterCount
//@EXECUTIONFLOW: 3�����filterCountΪ0(��֧�ֹ���)����ִ�в���11
//@EXECUTIONFLOW: 4������CSUDIDEMUXConnectTuner(nDemuxIndex,0)��nDemuxIndex���ӵ�0��tuner(��֧����ִ�в���11)
//@EXECUTIONFLOW: 5������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 6������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 7������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ�������ݣ����������ɹ�
//@EXECUTIONFLOW: 8�������ڻص������л�ȡ���û�����Ϊ���� 0
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�PAT_FILTER001�������ɹ�CSUDI_SUCCESS
//@EXECUTIONFLOW: 10������CSUDIDEMUXDisconnectTuner�Ͽ�nDemuxIndex��0��tuner����
//@EXECUTIONFLOW: 11��ȡ��һ��demux(nDemuxIndex++)
//@EXECUTIONFLOW: 12���ظ�����2��11��ֱ������demux������
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0004(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	int nDemuxCount = 0;
	int nDemuxIndex = 0;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount)),
			"Step 2 Allocate filter Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(0 != nDemuxCount),
			"Step 2 Failure !!!"
		);
	
	CSTCPrint("Demux Count ===%d\n",nDemuxCount);
	
	for(nDemuxIndex = 0;nDemuxIndex < nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sTmpCap),
				"Step 2 failure !!!"
			);
		
		if(!(sTmpCap.m_dwWorkTypeMask & EM_UDI_DEMUX_DMA) ||  sTmpCap.m_nFilterNum <= 0) 
		{
			CSTCPrint("Demux %d not support filter \n",nDemuxIndex);
			continue;
		}
		
		nRet = CSUDIDEMUXConnectTuner(TUNER_0,nDemuxIndex);
		if( CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED == nRet ) //if not support then go on trying
		{
			CSTCPrint("Demux %d cannot connect to tuner 0 !\n");
			continue;
		}
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == nRet),
				"Step 4 failure !!!"
			);
			
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
				"Step 5 lock frequence failure !!!"
			);	
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,nDemuxIndex,EM_PAT_PID)),
				"Step 6 Set PatFilter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
				"Step 6 Allocate filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 7 Start Filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
				(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
				"Step 8 Get UserData Failure !!!"
			);
		if(hFilter != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 9 Free Filter Failure !!!"
				);	
			hFilter = CSUDI_NULL;
		}
		nRet = CSUDIDEMUXDisconnectTuner(TUNER_0, nDemuxIndex);
		if( CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED == nRet ) //if not support then go on trying
		{
			CSTCPrint("Demux %d cannot disconnect to tuner 0 !\n");
			continue;
		}
	}
	
	CSTK_FATAL_POINT

	//�ָ�Ĭ�ϵ�TUNER_0��DEMUX_0����
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(TUNER_0, DEMUX_0)),
		"Connect tuner0_demux0 failure !!!");

	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 9 Free Filter Failure !!!"
			);	//make sure free the filter when error happens before step 9
	}	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERAllocate
//@DESCRIPTION: ���Կ��Է����filter��������ָ��demux֧�ֵ�filter����������ÿ��filterȡ��ֵͬ�����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��CSUDIDEMUXGetCapability������������
//@PRECONDITION: 3��CSUDIDEMUXGetFreeFilterCount������������
//@PRECONDITION: 4��CSUDIFILTERFree�ܳɹ��ͷ�һ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INOUT: 3��demuxIndex = 0
//@EXPECTATION: ���䳬��demux֧�ֵĸ���ʱ������CSUDISECTION_ERROR_NO_FREE_FILTER
//@REMARK: ��������������ע���뵽��filter�Ƿ���Խ��յ����ݣ����ҷ����ÿ��filter��������ͬ��
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCapability��ȡdemux0��filter�������������FilterCount��filter
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����filter���������سɹ�
//@EXECUTIONFLOW: 4���ظ�����2(FilterCount)�Σ�����ÿһ�ζ�����ɹ�
//@EXECUTIONFLOW: 5������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 �п��е�filter����������������Ϊ0
//@EXECUTIONFLOW: 6���ٴε���CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ʧ�ܣ�������CSUDISECTION_ERROR_NO_FREE_FILTER
//@EXECUTIONFLOW: 7������CSUDIFILTERFree�ͷ����з����filter
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0005(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nIndex = 0;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*һ��ֻ��32��filter*/

	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCapability(sTmpReqInfo.m_nDemuxID, &sTmpCap),
			"Step 1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nTotalFilteCount = sTmpCap.m_nFilterNum) > 0,
			"Step 1 failure ! Get Demux filter count < 0 !!"
		);
	CSTCPrint("Filter Count ===%d\n",nTotalFilteCount);
	
	for(nIndex = 0; (nIndex < nTotalFilteCount) && (nIndex < MAX_FILTER); nIndex++)
	{
		nRet = CSUDIFILTERAllocate(&sTmpReqInfo,SECTION_iCallback_1,(void*)nIndex,&ahFilter[nIndex]);
		if(CSUDI_SUCCESS != nRet)
		{
			CSTCPrint("Allocate the %d Filter Failure!!!\n",nIndex);
		}
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == nRet),
				"Step 2 Allocate filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_NULL != ahFilter[nIndex]),
				"Step 2 Allocate filter failure !!!"
			);
    		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIFILTERStart(ahFilter[nIndex])),
				"Step 3 Start filter failure !!!"
			);
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(DEMUX_0, &nFreeFilteCount)),
			"Step 5 Get Filter count Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(0 == nFreeFilteCount),
			"Step 5 Free filters should be 0 !!!"
			);
	CSTCPrint("Free Filter Count ===%d\n",nFreeFilteCount);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDISECTION_ERROR_NO_FREE_FILTER == CSUDIFILTERAllocate(&sTmpReqInfo,SECTION_iCallback_1,(void*)nIndex,&ahFilter[nIndex])),
			"Step 6 Allocate filter Failure !!!"
		);	

	CSTK_FATAL_POINT
	for(nIndex = 0; (nIndex < nTotalFilteCount) && (nIndex < MAX_FILTER); nIndex++)
	{
		if(ahFilter[nIndex] != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIFILTERFree(ahFilter[nIndex])),
					"Step 7 Free Filter Failure !!!"
				);	
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERAllocate
//@DESCRIPTION:  ���Կ��Է����filter��������16�����Ƿ��ܷ���ɹ����ҿ������뵽����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��CSUDIDEMUXGetCapability������������
//@PRECONDITION: 3��CSUDIDEMUXGetFreeFilterCount������������
//@PRECONDITION: 4��CSUDIFILTERFree�ܳɹ��ͷ�һ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INOUT: 3��demuxIndex = 0
//@EXPECTATION: ����ɹ������ĳ���16��filter������ÿ��filter������������
//@REMARK: ��������������ע���뵽��filter�Ƿ���Խ��յ����ݣ����ҷ����ÿ��filter��������ͬ��
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCapability��ȡdemux0��filter�������������FilterCount��filter
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����0��EM_SECTION_PAT_FILTER_MAX��EM_SECTION_EIT_FILTER_001��EM_SECTION_EIT_FILTER_MAXָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����filter���������سɹ�
//@EXECUTIONFLOW: 4����鲽��2���������filter��ȡ���ݵ�������������ÿ��filter�ƶ�������
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷ����з����filter
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0006(void)
{
	int nTotalFilteCount = 0;
	int nIndex = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*һ��ֻ��32��filter*/

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCapability(DEMUX_0, &sTmpCap),
			"Step 1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nTotalFilteCount = sTmpCap.m_nFilterNum) > 16,/*�����������ٴ���16��filter*/
			"Step 1 failure ! Get Demux filter count < 0 !!"
		);
	CSTCPrint("Filter Count ===%d\n",nTotalFilteCount);

	for(nIndex = 0; nIndex < EM_SECTION_PAT_FILTER_MAX; nIndex++)
	{
        SECTION_iSetSectionFilter(&sTmpReqInfo,nIndex,DEMUX_0,EM_PAT_PID);
        CSTK_ASSERT_TRUE_FATAL(
                (CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)nIndex,&ahFilter[nIndex])),
                "Step 2 Allocate filter Failure !!!"
            );
        CSTK_ASSERT_TRUE_FATAL(
                (CSUDI_NULL != ahFilter[nIndex]),
                "Step 2 Failure !!!"
            );
        CSTK_ASSERT_TRUE_FATAL(
                (CSUDI_TRUE == SECTION_iStartFilter(ahFilter[nIndex],(void*)nIndex)),
                "Step 3 Failure !!!"
            );
        switch(nIndex)
        {
            /*not expected data  received*/
            case EM_SECTION_PAT_FILTER_003:
            case EM_SECTION_PAT_FILTER_007:
            case EM_SECTION_PAT_FILTER_009:
            case EM_SECTION_PAT_FILTER_011:
            case EM_SECTION_PAT_FILTER_013:
            case EM_SECTION_PAT_FILTER_015:
            {
                CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
                        (CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)nIndex,CSUDI_FALSE)),
                        "Step 4 Check Expected Data Failure !!!"
                    );  
            }
            break;
            /*expected data  received*/
            default:
            {
                CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
                        (CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)nIndex,CSUDI_TRUE)),
                        "Step 4 Check Expected Data Failure !!!"
                    ); 
            }
            break;
        }

	}
    
	for(nIndex = EM_SECTION_EIT_FILTER_001; nIndex < EM_SECTION_EIT_FILTER_004; nIndex++)
	{
        SECTION_iSetSectionFilter(&sTmpReqInfo,nIndex,DEMUX_0,EM_EIT_PID);
        CSTK_ASSERT_TRUE_FATAL(
                (CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)nIndex,&ahFilter[nIndex])),
                "Step 2 Allocate filter Failure !!!"
            );
        CSTK_ASSERT_TRUE_FATAL(
                (CSUDI_NULL != ahFilter[nIndex]),
                "Step 2 Failure !!!"
            );
        CSTK_ASSERT_TRUE_FATAL(
                (CSUDI_TRUE == SECTION_iStartFilter(ahFilter[nIndex],(void*)nIndex)),
                "Step 3 Failure !!!"
            );
        CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
                (CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)nIndex,CSUDI_TRUE)),
                "Step 4 Check Expected Data Failure !!!"
            ); 
	}
    
	CSTK_FATAL_POINT
	for(nIndex = 0; (nIndex < nTotalFilteCount) && (nIndex < MAX_FILTER); nIndex++)
	{
		if(ahFilter[nIndex] != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIFILTERFree(ahFilter[nIndex])),
					"Step 5 Free Filter Failure !!!"
				);	
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERStart
//@DESCRIPTION: ����Filter���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: ��
//@INPUT: Filter���ΪCSUDI_NULL
//@EXPECTATION: ����ʧ��
//@EXECUTIONFLOW: ����CSUDIFILTERStart(CSUDI_NULL),�������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_SECTION_IT_Start_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIFILTERStart(CSUDI_NULL)),
			"Start an Invalid Filter should Fail !!!"
		);

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERStart
//@DESCRIPTION: ���Կ��Գɹ�����һ���Ϸ���filter��������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3��CSUDIFILTERAllocate�ܳɹ�����filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: �ɹ�����filter������CSUDI_SUCCESS���õ�PAT_FILTER001ָ��������
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷ�PAT_FILTER001
CSUDI_BOOL CSTC_SECTION_IT_Start_0002(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 2 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 4 Failure !!!"
		);
	
	CSTK_FATAL_POINT
		
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 5  Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERStart
//@DESCRIPTION: ���Կ��Գɹ�����һ���Ѿ�ֹͣ��filter
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3��CSUDIFILTERAllocate�ܳɹ�����filter
//@PRECONDITION: 4��CSUDIFILTERStop�ܳɹ�ֹͣfilter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: �ɹ�����filter������CSUDI_SUCCESS���õ�PAT_FILTER001ָ��������
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 5������CSUDIFILTERStopֹͣPAT_FILTER001������ֹͣ��������
//@EXECUTIONFLOW: 6������CSUDIFILTERStart���¿�ʼ�������ݣ��������سɹ��������µõ�PAT_FILTER001ָ��������
//@EXECUTIONFLOW: 7������CSUDIFILTERFree�ͷ�PAT_FILTER001
CSUDI_BOOL CSTC_SECTION_IT_Start_0003(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 2 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 4 Check Expected Data Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 5 Stop Filter Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s ��ȡ��������*/
			(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
			"Step 5 Check Expected Data Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 6 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 6 Check Expected Data Failure !!!"
		);
	
	CSTK_FATAL_POINT
		
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 7  Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERStop
//@DESCRIPTION: ����Filter���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: ��
//@INPUT:Filter ���ΪCSUDI_NULL
//@EXPECTATION: ����ʧ��
//@EXECUTIONFLOW: ����CSUDIFILTERStop(CSUDI_NULL),�������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_SECTION_IT_Stop_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIFILTERStop(CSUDI_NULL)),
			"Stop an Invalid Filter should Fail !!!"
		);

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERStop
//@DESCRIPTION: ����Filterû�������ͽ���stop�����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��CSUDIFILTERAllocate�ܳɹ�����filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ֹͣʧ�ܣ�����ʧ��
//@EXECUTIONFLOW: 1������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 2������CSUDIFILTERStopֹͣPAT_FILTER001������ֹͣʧ�ܣ���������CSUDISECTION_ERROR_INVALID_STATUS
//@EXECUTIONFLOW: 3������CSUDIFILTERFree�ͷ�PAT_FILTER001�������ͷųɹ�����������CSUDI_SUCCESS
CSUDI_BOOL CSTC_SECTION_IT_Stop_0002(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 1 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 1 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDISECTION_ERROR_INVALID_STATUS == CSUDIFILTERStop(hFilter)),
			"Step 2 Failure !!!"
		);

	CSTK_FATAL_POINT
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter)),
				"Step 3  Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

#define TestCount_Stop_003	(4)
static unsigned int g_unStatusStop003 = 0;
static SECTION_Filter_E g_eaFilter[TestCount_Stop_003] = {EM_SECTION_PAT_FILTER_001, EM_SECTION_PAT_FILTER_004, EM_SECTION_PAT_FILTER_005, EM_SECTION_PAT_FILTER_006};

static void CSUDIOSThreadEntry_Stop_003(void * pvParam)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = NULL;
	unsigned int unIdx = (unsigned int )pvParam;
	int i = 0;
#define Stop_0003_TEST_TIME 5

	SECTION_iSetSectionFilter(&sTmpReqInfo,g_eaFilter[unIdx],DEMUX_0,EM_PAT_PID);	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)g_eaFilter[unIdx],&hFilter)),
			"Step 3 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Failure !!!"
		);

	for(i=0;i<Stop_0003_TEST_TIME;i++)
	{
		SECTION_iSetCallbackStatus(g_eaFilter[unIdx],FALSE,FALSE);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)g_eaFilter[unIdx])),
				"Step 4 Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)g_eaFilter[unIdx],CSUDI_TRUE)),
				"Step 5 Check Expected Data Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStopFilter(hFilter,(void*)g_eaFilter[unIdx])),
				"Step 6 Failure !!!"
			);
		SECTION_iSetCallbackStatus(g_eaFilter[unIdx],FALSE,FALSE);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)g_eaFilter[unIdx],CSUDI_FALSE)),
				"Step 7 Check Expected Data Failure !!!"
			);
	}

	CSTK_FATAL_POINT

	if(hFilter != NULL)
	{
		SECTION_iFreeFilter(hFilter, (void*)g_eaFilter[unIdx]);
	}

	g_unStatusStop003 &= ~(1<<unIdx);

	return;
}

//@CASEGROUP: CSUDIFILTERStart CSUDIFILTERStop
//@DESCRIPTION: ����filter�ڻ�û��start��ʱ���������ݻص���start��Ӧ�������ݻص���stop�����������ݻص�
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��CSUDIFILTERAllocate�ܳɹ�����filter
//@INPUT:����EPG_0606_121458.ts
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: PAT_FILTER001 PAT_FILTER004��PAT_FILTER005��PAT_FILTER006(��ο�CSTC_SECTION_IT_Allocate_0000)
//@EXPECTATION: ֹͣʧ�ܣ�����ʧ��
//@EXECUTIONFLOW: 1����סEPG_0606_121458.ts���ڵ�Ƶ��,�����ɹ�
//@EXECUTIONFLOW: 2������CSUDIOSThreadCreate����4���̷ֱ߳�������3~9 ����:
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate����filter,filter�ֱ�ΪPAT_FILTER004��PAT_FILTER005��PAT_FILTER006,��������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart����filter,�������سɹ�
//@EXECUTIONFLOW: 5���������״̬�����������ݽ��յ�
//@EXECUTIONFLOW: 6������CSUDIFILTERStopֹͣfilter,�������سɹ�
//@EXECUTIONFLOW: 7���������״̬������ֹͣ��û�����ݻص�
//@EXECUTIONFLOW: 8���ظ�����4~7,����ÿ�����ɹ�
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�filter�����������سɹ�
//@EXECUTIONFLOW: 10������CSUDIOSThreadDestroy���Ĳ����߳�
CSUDI_BOOL CSTC_SECTION_IT_Stop_0003(void)
{
	CSUDI_HANDLE hThread[TestCount_Stop_003]={NULL};
	int i=0;

	g_unStatusStop003 = ~((~0)<<TestCount_Stop_003);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);

	for(i=0;i<TestCount_Stop_003;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == 
            CSUDIOSThreadCreate("test",17*6, 1024*8,CSUDIOSThreadEntry_Stop_003,(void*)(i),&hThread[i]),
				"step 2 fail to create test thread\n");
	}

	while(g_unStatusStop003)
	{
		CSUDIOSThreadSleep(200);
	}

    CSTK_FATAL_POINT
	for (i=0;i<TestCount_Stop_003;i++)
	{
        if(hThread[i] != NULL);
		    CSUDIOSThreadDestroy(hThread[i]);
        hThread[i] = NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERFree
//@DESCRIPTION: ����Filter���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: ��
//@INPUT: Filter���ΪCSUDI_NULL
//@EXPECTATION: ����ʧ��
//@EXECUTIONFLOW: ����CSUDIFILTERFree(CSUDI_NULL),�������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_SECTION_IT_Free_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIFILTERFree(CSUDI_NULL)),
			"Free an Invalid Filter should Fail !!!"
		);

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERFree
//@DESCRIPTION: �����ͷ�һ��δ������filter
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��CSUDIFILTERAllocate�ܳɹ�����filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: �ͷųɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 2������CSUDIFILTERFree�ͷ�PAT_FILTER001�������ͷųɹ�
CSUDI_BOOL CSTC_SECTION_IT_Free_0002(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 1 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 1 Failure !!!"
		);
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter)),
				"Step 2  Free Filter Failure !!!"
			);
	}

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERAllocate CSUDIFILTERFree
//@DESCRIPTION: ���Է��������ͷ�filter
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��CSUDIFILTERAllocate�ܳɹ�����filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��pid = 0x0,m_eReqType=EM_UDI_REQ_SECTION_DATA,��������ȫΪ0
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: 1.�ڲ�����CSUDIFILTERStart������������ͷ�500��,����ÿ�ζ�����CSUDI_SUCCESS
//@EXPECTATION: 2.�ڵ���CSUDIFILTERStart������������ͷ�500��,����ÿ�ζ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFILTERAllocate����ָ����filter����������ɹ�
//@EXECUTIONFLOW: 2������CSUDIFILTERFree�ͷ�filter�������ͷųɹ�
//@EXECUTIONFLOW: 3���ظ�����1��2��500��
//@EXECUTIONFLOW: 4������CSUDIFILTERAllocate����ָ����filter����������ɹ�
//@EXECUTIONFLOW: 5������CSUDIFILTERStart��ʼfilter,�����ͷųɹ�
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ�filter�������ͷųɹ�
//@EXECUTIONFLOW: 7���ظ�����4~6��500��
CSUDI_BOOL CSTC_SECTION_IT_Free_0003(void)
{
	CSUDISECTIONRequestInfo_S stReq;
	CSUDI_HANDLE  hFilterHandle = NULL;
	int i=0;
	
	memset(&stReq,0,sizeof(stReq));
	stReq.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stReq.m_nDemuxID = 0;
	stReq.m_nPID = 0x0;

	for(i=0;i<500;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stReq,CSSectionCallback_common,NULL,&hFilterHandle),
				"step 1 fail to malloc one filter\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hFilterHandle,"step 1 CSUDIFILTERAllocate return success but handle is null\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle),"step 2 fail to free filter\n");
		hFilterHandle = NULL;
	}

	for(i=0;i<500;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stReq,CSSectionCallback_common,NULL,&hFilterHandle),
				"step 4 fail to malloc one filter\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hFilterHandle,"step 4 CSUDIFILTERAllocate return success but handle is null\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"step 5 fail to start filter\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle),"step 6 fail to free filter\n");
		hFilterHandle = NULL;
	}

	CSTK_FATAL_POINT
		
	if (hFilterHandle)
	{
		CSUDIFILTERFree(hFilterHandle);
	}
	
	return CSUDI_TRUE;
}


//@CASEGROUP: CSUDIFILTERAllocate CSUDIFILTERFree
//@DESCRIPTION: �����ڲ�ͬPID����·��������ͷ�filter
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��CSUDIFILTERAllocate�ܳɹ�����filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��pid����Դ�������,m_eReqType=EM_UDI_REQ_SECTION_DATA,��������ȫΪ0
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: 1.�ڲ�����CSUDIFILTERStart������������ͷ�1000��,ÿ�ζ��ò�ͬPID,����ÿ�ζ�����CSUDI_SUCCESS
//@EXPECTATION: 2.�ڵ���CSUDIFILTERStart������������ͷ�1000��,ÿ�ζ��ò�ͬPID,����ÿ�ζ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIFILTERAllocate����ָ����filter����������ɹ�
//@EXECUTIONFLOW: 2������CSUDIFILTERFree�ͷ�filter�������ͷųɹ�
//@EXECUTIONFLOW: 3�������ε�PID��1�����´�PID
//@EXECUTIONFLOW: 4���ظ�����1~3��1000��
//@EXECUTIONFLOW: 5������CSUDIFILTERAllocate����ָ����filter����������ɹ�
//@EXECUTIONFLOW: 6������CSUDIFILTERStart��ʼfilter,�����ͷųɹ�
//@EXECUTIONFLOW: 7������CSUDIFILTERFree�ͷ�filter�������ͷųɹ�
//@EXECUTIONFLOW: 8�������ε�PID��1�����´�PID
//@EXECUTIONFLOW: 9���ظ�����5~8��1000��
CSUDI_BOOL CSTC_SECTION_IT_Free_0004(void)
{
	CSUDISECTIONRequestInfo_S stReq;
	CSUDI_HANDLE  hFilterHandle = NULL;
	int nPid = 0x0;
	int i = 0;
		
	memset(&stReq,0,sizeof(stReq));
	stReq.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stReq.m_nDemuxID = 0;
	stReq.m_nPID = nPid;

	for(i=0, nPid=0; i<1000; i++, nPid++)
	{
		stReq.m_nPID = nPid;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stReq,CSSectionCallback_common,NULL,&hFilterHandle),
				"step 1 fail to malloc one filter\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hFilterHandle,"step 1 CSUDIFILTERAllocate return success but handle is null\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle),"step 2 fail to free filter\n");
		hFilterHandle = NULL;
	}

	for(i=0, nPid=0; i<1000;i++,nPid++)
	{
		stReq.m_nPID = nPid;
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stReq,CSSectionCallback_common,NULL,&hFilterHandle),
				"step 4 fail to malloc one filter\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hFilterHandle,"step 4 CSUDIFILTERAllocate return success but handle is null\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterHandle),"step 5 fail to start filter\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterHandle),"step 6 fail to free filter\n");
		hFilterHandle = NULL;
	}

	CSTK_FATAL_POINT
		
	if (hFilterHandle)
	{
		CSUDIFILTERFree(hFilterHandle);
	}
	
	return CSUDI_TRUE;
}



//@CASEGROUP: CSUDIFILTERModify
//@DESCRIPTION: ����Filter���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: ��
//@INPUT: Filter���ΪCSUDI_NULL
//@EXPECTATION: ����ʧ��
//@EXECUTIONFLOW: ����CSUDIFILTERModify(CSUDI_NULL��CSUDI_NULL��CSUDI_FALSE),�������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_SECTION_IT_Modify_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIFILTERModify(CSUDI_NULL,CSUDI_NULL,CSUDI_FALSE)),
			"Modify an Invalid Filter should Fail !!!"
		);

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERModify
//@DESCRIPTION: �����޸�һ��δֹͣ��filter
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3��CSUDIFILTERAllocate�ܳɹ�����filter
//@PRECONDITION: 4��CSUDIFILTERStart�ܳɹ�����һ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: �޸�ʧ�ܣ�����CSUDISECTION_ERROR_INVALID_STATUS
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 5������CSUDIFILTERModify(filter,CSUDI_NULL,CSUDI_TRUE)�޸�PAT_FILTER001���޸�ʧ�ܣ�������CSUDISECTION_ERROR_INVALID_STATUS
//@EXECUTIONFLOW: 6��У��õ������ݣ�������ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 7������CSUDIFILTERFree�ͷ�PAT_FILTER001
CSUDI_BOOL CSTC_SECTION_IT_Modify_0002(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 2 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 4 Check Expected Data Failure !!!"
		);
	
	SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_001,CSUDI_FALSE,CSUDI_FALSE);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDISECTION_ERROR_INVALID_STATUS == CSUDIFILTERModify(hFilter,CSUDI_NULL,CSUDI_TRUE)),
			"Step 5 should not modify a running Filter !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
			"Step 6 Check Expected Data Failure !!!"
		);
	
	CSTK_FATAL_POINT
		
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 7  Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERModify
//@DESCRIPTION: �����ܹ��ɹ��޸�һ��ֹͣ��filter
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3��CSUDIFILTERAllocate�ܳɹ�����filter
//@PRECONDITION: 4��CSUDIFILTERStart�ܳɹ�����һ��filter
//@PRECONDITION: 5��CSUDIFILTERSop�ܳɹ�ֹͣһ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2��PAT_FILTER002(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 3���Ϸ��Ļص�����
//@EXPECTATION: �޸ĳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 5������CSUDIFILTERStopֹͣPAT_FILTER001
//@EXECUTIONFLOW: 6������CSUDIFILTERModify�޸�PAT_FILTER001ΪPAT_FILTER002���޸ĳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIFILTERStart��ʼ����PAT_FILTER002ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 8��У��õ������ݣ�����ΪFILTER02ָ��������
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�filter
CSUDI_BOOL CSTC_SECTION_IT_Modify_0003(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIFILTERMatchMask_S sMatch;
	CSUDI_HANDLE hFilter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 2 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 4 Check Expected Data Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 5 Stop Filter Failure !!!"
		);	
	
	CSTK_ASSERT_TRUE_FATAL(
			SECTION_iSetMatchMask(&sMatch,EM_SECTION_PAT_FILTER_002),
			"Set filter value failure at step 6 !!!"
			); //modify filter1 to filter2
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iModifyFilter(hFilter,&sMatch,CSUDI_TRUE,(void*)EM_SECTION_PAT_FILTER_002)),
			"Step 6 Modify Filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_002)),
			"Step 7 Failure !!!"
		);
    /*����ȥ������ݵ�ʱ���Ǵ�EM_SECTION_PAT_FILTER_001������
     *����ΪSECTION_iAllocFilter����Ļص�����ʵ����ֻ���������EM_SECTION_PAT_FILTER_001��
     *��˴˴���Ӧ�ô�EM_SECTION_PAT_FILTER_002
    */
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 8 Check Expected Data Failure !!!"
		);
	
	CSTK_FATAL_POINT
		
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_002)),
				"Step 9  Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERModify
//@DESCRIPTION: �����ܹ��ɹ��޸�һ��û�н���CRC����filter��ʹ�����CRC���(����û��CRCֵ������(TDT)����CRC��飬Ӧ�ý��ղ�������)
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3��CSUDIFILTERAllocate�ܳɹ�����filter
//@PRECONDITION: 4��CSUDIFILTERStart�ܳɹ�����һ��filter
//@PRECONDITION: 5��CSUDIFILTERSop�ܳɹ�ֹͣһ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��TDT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2��CRC���ΪCSUDI_FALSE
//@INPUT: 3���Ϸ��Ļص�����
//@EXPECTATION: �޸ĳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate(TDT_FILTER001,callback,userdata,�Ϸ�ָ��)����TDT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERModify�޸�bCRCCheckΪCSUDI_TRUE���޸ĳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIFILTERStart��ʼ����TDT_FILTER001ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 5��У��õ������ݣ������޷���������
//@EXECUTIONFLOW: 6������CSUDIFILTERStopֹͣfilter
//@EXECUTIONFLOW: 7������CSUDIFILTERModify�޸�bCRCCheckΪCSUDI_FALSE���޸ĳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSUDIFILTERStart��ʼ����TDT_FILTER001ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 9���������Խ��յ�TDT_FILTER001ָ��������
//@EXECUTIONFLOW: 10������CSUDIFILTERFree�ͷ�filter
CSUDI_BOOL CSTC_SECTION_IT_Modify_0004(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_TDT_FILTER_001,DEMUX_0,EM_TDT_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_TDT_FILTER_001,&hFilter)),
			"Step 2 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iModifyFilter(hFilter,CSUDI_NULL,CSUDI_TRUE,(void*)EM_SECTION_TDT_FILTER_001)),
			"Step 3 Modify Filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_TDT_FILTER_001)),
			"Step 4 Start filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_TDT_FILTER_001,CSUDI_FALSE)),
			"Step 5 Expected No Data Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter,(void*)EM_SECTION_TDT_FILTER_001)),
			"Step 6 Stop Filter Failure !!!"
		);	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iModifyFilter(hFilter,CSUDI_NULL,CSUDI_FALSE,(void*)EM_SECTION_TDT_FILTER_001)),
			"Step 7 Modify Filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_TDT_FILTER_001)),
			"Step 8 Start filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_TDT_FILTER_001,CSUDI_TRUE)),
			"Step 9 Check Expected Data Failure !!!"
		);

	CSTK_FATAL_POINT
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_TDT_FILTER_001)),
				"Step 10 Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIFILTERSetBufSize
//@DESCRIPTION:����Filter���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION:��
//@INPUT:Filter���ΪCSUDI_NULL
//@EXPECTATION:����ʧ��
//@EXECUTIONFLOW: ����CSUDIFILTERSetBufSize(CSUDI_NULL��1M),�������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_SECTION_IT_SetBufSize_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIFILTERSetBufSize(CSUDI_NULL,(2<<20))),
			"Set the buffer of an Invalid Filter should Fail !!!"
		);

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIFILTERFree
//@DESCRIPTION: ��������һ��δֹͣ��filter��buffer��С
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��CSUDIFILTERAllocate�ܳɹ�����filter
//@PRECONDITION: 3��CSUDIFILTERStart�ܳɹ�����һ��filter
//@PRECONDITION: 4��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT: 3��buffer��С2M
//@EXPECTATION: ���������ã�����ʧ��
//@REMARK: �ɹ�����һ��filter��buffer��С�Ĳ�������������demux�����ܲ���һ����ԡ�
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERSetBufSize����PAT_FILTER001��buffer��С������ʧ�ܣ�������ʧ��
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷ�PAT_FILTER001
CSUDI_BOOL CSTC_SECTION_IT_SetBufSize_0002(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 2 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 3 Check Expected Data Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDISECTION_ERROR_INVALID_STATUS == CSUDIFILTERSetBufSize(hFilter,(2<<20))),
			"Step 4 Set the buffer of a running Filter should be failure !!!"
		);
	
	CSTK_FATAL_POINT
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 5 Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����demux��Filter��Դ�Ĺ������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001~PAT_FILTER006(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ���������Ļ��Filter������������
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIDEMUXGetCapability��ȡdemux 0 ��filter����
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ��������
//@EXECUTIONFLOW: 5��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 6������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 ���е�filter����������������+1=����
//@EXECUTIONFLOW: 7������CSUDIFILTERStopֹͣPAT_FILTER001
//@EXECUTIONFLOW: 8������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 ���е�filter����������������+1=����
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�PAT_FILTER001
//@EXECUTIONFLOW: 10������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 ���е�filter����������������=����
//@EXECUTIONFLOW: 11���ֱ���PAT_FILTER001~PAT_FILTER006������filter����������ɹ���������ע��ȡ������
//@EXECUTIONFLOW: 12������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 ���е�filter����,����������+6=����
//@EXECUTIONFLOW: 13������CSUDIFILTERFree�ͷ�PAT_FILTER001
//@EXECUTIONFLOW: 14������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 ���е�filter����,����������+5=����
//@EXECUTIONFLOW: 15������CSUDIFILTERFree�ͷ�PAT_FILTER002~PAT_FILTER006
//@EXECUTIONFLOW: 16������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 ���е�filter����,����������=����
CSUDI_BOOL CSTC_SECTION_IT_0001(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nFilterIndex = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*һ��ֻ��32��filter*/

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCapability(DEMUX_0, &sTmpCap),
			"Step 2 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nTotalFilteCount = sTmpCap.m_nFilterNum) > 0,
			"Step 2 failure ! Get Demux filter count < 0 !!"
		);
	
	CSTCPrint("DEMUX_0 has filters( %d )\n",nTotalFilteCount);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERAllocate(&sTmpReqInfo,SECTION_iCallback_1,(void*)EM_SECTION_PAT_FILTER_001,&ahFilter[0])),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != ahFilter[0]),
			"Step 3 Allocate filter failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[0],(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 4 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 5 Check Expected Data Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(sTmpReqInfo.m_nDemuxID, &nFreeFilteCount)),
			"Step 6 Get Free filters failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			((nFreeFilteCount + 1) == nTotalFilteCount),
			"Step 6 Free Filter should be (nFreeFilteCount + 1 = nTotalFilteCount) !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(ahFilter[0],(void*)EM_SECTION_TDT_FILTER_001)),
			"Step 7 Stop Filter Failure !!!"
		);	
	
	nFreeFilteCount = 0; 
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(sTmpReqInfo.m_nDemuxID, &nFreeFilteCount)),
			"Step 8 Get Free filters failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			((nFreeFilteCount + 1) == nTotalFilteCount),
			"Step 8 Free Filter should be (nFreeFilteCount + 1 = nTotalFilteCount) !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERFree(ahFilter[0])),
			"Step 9 Free Filter failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(sTmpReqInfo.m_nDemuxID, &nFreeFilteCount)),
			"Step 10 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nFreeFilteCount == nTotalFilteCount),
			"Step 10 Free filters should be nTotalFilteCount !!!"
		);
	
	for(nFilterIndex = EM_SECTION_PAT_FILTER_001; nFilterIndex <= EM_SECTION_PAT_FILTER_006 ; nFilterIndex ++)
	{
		SECTION_iSetSectionFilter(&sTmpReqInfo,nFilterIndex,DEMUX_0,EM_PAT_PID);	
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIFILTERAllocate(&sTmpReqInfo,SECTION_iCallback_1,(void*)nFilterIndex,&ahFilter[nFilterIndex])),
				"Step 11 Allocate Filters Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_NULL != ahFilter[nFilterIndex]),
				"Step  11 Allocate Filters failure !!!"
			);
	}
	nFreeFilteCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(sTmpReqInfo.m_nDemuxID, &nFreeFilteCount)),
			"Step 12 Get Free filters failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			((nFreeFilteCount + 6) == nTotalFilteCount),
			"Step 12 Free Filter should be (nFreeFilteCount + 6 = nTotalFilteCount) !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERFree(ahFilter[0])),
			"Step 13 Free Filter failure !!!"
		);
	
	ahFilter[0] = CSUDI_NULL;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(sTmpReqInfo.m_nDemuxID, &nFreeFilteCount)),
			"Step 14 Get Free Filters failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			((nFreeFilteCount + 5) == nTotalFilteCount),
			"Step 14 Free filters should be (nFreeFilteCount + 5 = nTotalFilteCount) !!!"
		);	

	CSTK_FATAL_POINT       //���ڴ˴���Ϊ�˱�֤�������ͷ�����filter
	for(nFilterIndex = EM_SECTION_PAT_FILTER_001; nFilterIndex <= EM_SECTION_PAT_FILTER_006 ; nFilterIndex ++)
	{
		if(ahFilter[nFilterIndex] != CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIFILTERFree(ahFilter[nFilterIndex])),
					"Step 15 Free Filter Failure !!!"
				);
		}
	}
	nFreeFilteCount = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(sTmpReqInfo.m_nDemuxID, &nFreeFilteCount)),
			"Step 16 Get Free Filters  failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			((nFreeFilteCount ) == nTotalFilteCount),
			"Step 16 Free filters should be (nFreeFilteCount = nTotalFilteCount) !!!"
		);	
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ����CSUDIFILTERModify�޸�filterΪͬһ��fitler�����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ���Գɹ��޸�filter�����һ�ȡ����
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ��������
//@EXECUTIONFLOW: 5��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 6������CSUDIFILTERStopֹͣPAT_FILTER001����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIFILTERModify�޸�PAT_FILTER001��������ΪPAT_FILTER001���޸ĳɹ�������CSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 10������CSUDIFILTERFree�ͷ�PAT_FILTER001
CSUDI_BOOL CSTC_SECTION_IT_0002(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIFILTERMatchMask_S sMatch;
	CSUDI_HANDLE hFilter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 2 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 4 Check Expected Data Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 5 Stop Filter Failure !!!"
		);	
	
	CSTK_ASSERT_TRUE_FATAL(
			SECTION_iSetMatchMask(&sMatch,EM_SECTION_PAT_FILTER_001),
			"Set filter value failure at step 7 !!!"
			); //modify filter1 to filter1
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iModifyFilter(hFilter,&sMatch,CSUDI_TRUE,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 7 Stop Filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 8 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 9 Check Expected Data Failure !!!"
		);
	
	CSTK_FATAL_POINT;
		
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 10  Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ���ԶԽӿڽ����ظ����õ����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿһ���ظ����ö�ʧ�ܣ�����ʧ��
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ��������
//@EXECUTIONFLOW: 5��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 6������CSUDIFILTERStart����filter10�Σ�ÿ�ζ�����ʧ��
//@EXECUTIONFLOW: 7������CSUDIFILTERStopֹͣfilter�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSUDIFILTERStopֹͣfilter10�Σ�ÿ�ζ�����ʧ��
//@EXECUTIONFLOW: 9������CSUDIFILTERStart��������PAT_FILTER001ָ��������
//@EXECUTIONFLOW: 10��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 11����start״̬�£�����CSUDIFILTERModify�޸�PAT_FILTER001ΪPAT_FILTER002����������10�Σ�ÿ�ζ�ʧ�ܣ�������ʧ��
//@EXECUTIONFLOW: 12��У��õ������ݣ�������ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 13������CSUDIFILTERStopֹͣfilter
//@EXECUTIONFLOW: 14������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 15���ٴε���CSUDIFILTERStart����filter10�Σ�ÿ�ζ�����ʧ��
//@EXECUTIONFLOW: 16������CSUDIFILTERStopֹͣfilter10�Σ�ÿ�ζ�����ʧ��
//@EXECUTIONFLOW: 17���ظ�2-15����50��
CSUDI_BOOL CSTC_SECTION_IT_0003(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIFILTERMatchMask_S sMatch;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	int i = 0;
	int nLoop = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
	do{ //100 times
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
					"Step 2 Allocate filter Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != hFilter),
					"Step 2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 3 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 4 Check Expected Data Failure !!!"
				);
			for(i = 0;i<10; i++)
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS != CSUDIFILTERStart(hFilter)),
						"Step 6 Failure !!!"
					);
			}
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 7 Stop Filter Failure !!!"
				);
			for(i = 0;i<10; i++)
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS != CSUDIFILTERStop(hFilter)),
						"Step 8 Failure !!!"
					);
			}
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 9 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 10 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					SECTION_iSetMatchMask(&sMatch,EM_SECTION_PAT_FILTER_002),
					"Set filter value failure at step 11 !!!"
					); //modify filter1 to filter2
			for(i = 0;i<10; i++)
			{
				CSTK_ASSERT_TRUE_FATAL(/*�˴�����EM_SECTION_PAT_FILTER_001����step12 �����*/
						(CSUDI_FALSE == SECTION_iModifyFilter(hFilter,&sMatch,CSUDI_TRUE,(void*)EM_SECTION_PAT_FILTER_001)),
						"Step 11 Modify Filter at running status should be Failure !!!"
					);
			}
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 12 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter,EM_SECTION_PAT_FILTER_001)),
					"Step 13 stop filter Failure !!!"
				);
			if(hFilter != CSUDI_NULL)
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,EM_SECTION_PAT_FILTER_001)),
						"Step 14  Free Filter Failure !!!"
					);
				hFilter = CSUDI_NULL;
			}
			
			for(i = 0;i<10; i++)
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS != CSUDIFILTERStart(hFilter)),
						"Step 15 start an already-freed filter shoudl be Failure !!!"
					);
			}
			for(i = 0;i<10; i++)
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS != CSUDIFILTERStop(hFilter)),
						"Step 16 stop an already-freed filter shoudl be Failure !!!"
					);
			}	
			CSTCPrint("���뱾��ѭ���������� %d ��\n",TESTTIMES-nLoop);
	}while((++nLoop) < TESTTIMES);
	
	CSTK_FATAL_POINT;
	
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,EM_SECTION_PAT_FILTER_001)),
				"Step 14  Free Filter Failure !!!"
			);
		hFilter = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����FilterΪ��ͬ���ʱ��PAT��������(filter��ȷֱ�Ϊ1��3��4��6;����CSTC_SECTION_IT_0005����8��10��12��14)
//@DESCRIPTION: ��Щƽ̨��������ʱ��ͬһpid������������ͬ��filter����ˣ�ÿ������֮�󶼽�filter�ͷ�
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3��CSUDIDEMUXGetCapability������������
//@PRECONDITION: 4��CSUDIDEMUXGetFreeFilterCount������������
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001~PAT_FILTER007(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT: 3������demux 0
//@EXPECTATION: ÿһ���趼�ܹ��ɹ���ȡ����δ��ȡָ��������
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ��������
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪPAT_FILTER001ָ�������ݣ����ͷ�filter
//@EXECUTIONFLOW: 5������CSUDIFILTERAllocate����PAT_FILTER002ָ����filter2����������ɹ�
//@EXECUTIONFLOW: 6������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7��У��õ������ݣ������õ�PAT_FILTER002ָ�������ݣ����ͷ�filter
//@EXECUTIONFLOW: 8������CSUDIFILTERAllocate����PAT_FILTER003ָ����filter3����������ɹ�
//@EXECUTIONFLOW: 9������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10��У��õ������ݣ������ò����κ����ݣ����ͷ�filter
//@EXECUTIONFLOW: 11������CSUDIFILTERAllocate����PAT_FILTER004ָ����filter4����������ɹ�
//@EXECUTIONFLOW: 12������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 13��У��õ������ݣ������õ�PAT_FILTER004ָ�������ݣ����ͷ�filter
//@EXECUTIONFLOW: 14������CSUDIFILTERAllocate����PAT_FILTER005ָ����filter5����������ɹ�
//@EXECUTIONFLOW: 15������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 16��У��õ������ݣ������õ�PAT_FILTER005ָ�������ݣ����ͷ�filter
//@EXECUTIONFLOW: 17������CSUDIFILTERAllocate����PAT_FILTER006ָ����filter6����������ɹ�
//@EXECUTIONFLOW: 18������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 19��У��õ������ݣ������õ�PAT_FILTER006ָ�������ݣ����ͷ�filter
//@EXECUTIONFLOW: 20������CSUDIFILTERAllocate����PAT_FILTER007ָ����filter7����������ɹ�
//@EXECUTIONFLOW: 21������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 22��У��õ������ݣ������ò����κ����ݣ����ͷ�filter
//@EXECUTIONFLOW: 23������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 ���е�filter����,���������� = ����
CSUDI_BOOL CSTC_SECTION_IT_0004(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nFilterIndex = 0;
	int nLoop = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*һ��ֻ��32��filter*/


	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCapability(DEMUX_0, &sTmpCap),
			"Get DEMUX_0 Filters count failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nTotalFilteCount = sTmpCap.m_nFilterNum) > 0,
			"Get Demux filter count < 0 !!"
		);
	CSTCPrint("DEMUX_0 has filters( %d )\n",nTotalFilteCount);
	
	do{		//filter1
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&ahFilter[EM_SECTION_PAT_FILTER_001])),
					"Step 2 Allocate filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_001]),
					"Step 2 Allocate filter1 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_001],(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 3 Start Filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 4 Check Expected Data Failure !!!"
				);
			//free filter1
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_001],(void*)EM_SECTION_PAT_FILTER_001)),
						"Step 23  Free Filter1 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_001] = CSUDI_NULL;
			}
			
			//filter2
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_002,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_002,&ahFilter[EM_SECTION_PAT_FILTER_002])),
					"Step 5 Allocate filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_002]),
					"Step 5 Allocate filter2 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_002],(void*)EM_SECTION_PAT_FILTER_002)),
					"Step 6 Start Filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_002,CSUDI_TRUE)),
					"Step 7 Check Expected Data Failure !!!"
				);
					//free filter2
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_002],(void*)EM_SECTION_PAT_FILTER_002)),
						"Step 23  Free Filter2 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_002] = CSUDI_NULL;
			}	
			//filter3
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_003,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,&ahFilter[EM_SECTION_PAT_FILTER_003])),
					"Step 8 Allocate filter3 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_003]),
					"Step 8 Allocate filter3 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_003],(void*)EM_SECTION_PAT_FILTER_003)),
					"Step 9 Start Filter3 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,CSUDI_FALSE)),
					"Step 10 Expected No Data Failure !!!"
				);
			//free filter3
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_003],(void*)EM_SECTION_PAT_FILTER_003)),
						"Step 23  Free Filter3 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_003] = CSUDI_NULL;
			}
			//filter4
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_004,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_004,&ahFilter[EM_SECTION_PAT_FILTER_004])),
					"Step 11 Allocate filter4 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_004]),
					"Step 11 Allocate filter4 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_004],(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 12 Start Filter4 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 13 Check Expected Data Failure !!!"
				);
				//free filter4
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_004],(void*)EM_SECTION_PAT_FILTER_004)),
						"Step 23  Free Filter4 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_004] = CSUDI_NULL;
			}
			//filter5
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_005,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_005,&ahFilter[EM_SECTION_PAT_FILTER_005])),
					"Step 14 Allocate filter5 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_005]),
					"Step 14 Allocate filter failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_005],(void*)EM_SECTION_PAT_FILTER_005)),
					"Step 15 Start Filter5 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_005,CSUDI_TRUE)),
					"Step 16 Check Expected Data Failure !!!"
				);
		
			//free filter5
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_005],(void*)EM_SECTION_PAT_FILTER_005)),
						"Step 23  Free Filter5 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_005] = CSUDI_NULL;
			}
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(DEMUX_0, &nFreeFilteCount)),
					"step 24 Get DEMUX_0 Free Filters count failure !!!"
				);
			CSTCPrint("DEMUX_0 Current free filters is: %d\n",nFreeFilteCount);
			//filter6
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_006,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_006,&ahFilter[EM_SECTION_PAT_FILTER_006])),
					"Step 17 Allocate filter6 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_006]),
					"Step 18 Allocate filter6 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_006],(void*)EM_SECTION_PAT_FILTER_006)),
					"Step 19 Start Filter6 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_006,CSUDI_TRUE)),
					"Step 20 Check Expected Data Failure !!!"
				);	
			
			//free filter6
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_006],(void*)EM_SECTION_PAT_FILTER_006)),
						"Step 25  Free Filter6 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_006] = CSUDI_NULL;
			}
			//filter7
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_007,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_007,&ahFilter[EM_SECTION_PAT_FILTER_007])),
					"Step 21 Allocate filter7 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_007]),
					"Step 21 Allocate filter7 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_007],(void*)EM_SECTION_PAT_FILTER_007)),
					"Step 22 Start Filter7 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_007,CSUDI_TRUE)),
					"Step 23 Expected No Data Failure !!!"
				);

			//free filter7
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_007],(void*)EM_SECTION_PAT_FILTER_007)),
						"Step 25  Free Filter7 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_007] = CSUDI_NULL;
			}
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(DEMUX_0,&nFreeFilteCount)),
						"Step 26 Get Free Filters Failure !!!"
					);
			CSTK_ASSERT_TRUE(
						(nFreeFilteCount == nTotalFilteCount),
						"Step 26 Free Filters should be(nFreeFilteCount == nTotalFilteCount) !!!"
					);
		}while((++nLoop) < TESTTIMES);

	CSTK_FATAL_POINT
	for(nFilterIndex = EM_SECTION_PAT_FILTER_001; nFilterIndex <= EM_SECTION_PAT_FILTER_007 ; nFilterIndex ++)
	{	
		if(CSUDI_NULL != ahFilter[nFilterIndex])
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[nFilterIndex],(void*)nFilterIndex)),
					"Free Filter Failure !!!"
				);
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ����FilterΪ��ͬ���ʱ��PAT��������(filter���Ϊ8��10��12��14)
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER008~PAT_FILTER015(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿһ���趼�ܹ��ɹ���ȡ����δ��ȡָ��������
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER008ָ����filter8����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER008ָ��������
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪPAT_FILTER008ָ��������
//@EXECUTIONFLOW: 5������CSUDIFILTERAllocate����PAT_FILTER009ָ����filter9����������ɹ�
//@EXECUTIONFLOW: 6������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7��У��õ������ݣ������ò����κ�����
//@EXECUTIONFLOW: 8������CSUDIFILTERAllocate����PAT_FILTER010ָ����filter10����������ɹ�
//@EXECUTIONFLOW: 9������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10��У��õ������ݣ�����ΪPAT_FILTER010ָ��������
//@EXECUTIONFLOW: 11������CSUDIFILTERAllocate����PAT_FILTER011ָ����filter11����������ɹ�
//@EXECUTIONFLOW: 12������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 13��У��õ������ݣ������ò����κ�����
//@EXECUTIONFLOW: 14������CSUDIFILTERAllocate����PAT_FILTER012ָ����filter12����������ɹ�
//@EXECUTIONFLOW: 15������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 16��У��õ������ݣ������õ�PAT_FILTER012ָ��������
//@EXECUTIONFLOW: 17������CSUDIFILTERAllocate����PAT_FILTER013ָ����filter13����������ɹ�
//@EXECUTIONFLOW: 18������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 19��У��õ������ݣ������ò����κ�����
//@EXECUTIONFLOW: 20������CSUDIFILTERAllocate����PAT_FILTER014ָ����filter14����������ɹ�
//@EXECUTIONFLOW: 21������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 22��У��õ������ݣ������õ�PAT_FILTER014ָ��������
//@EXECUTIONFLOW: 23������CSUDIFILTERAllocate����PAT_FILTER015ָ����filter15����������ɹ�
//@EXECUTIONFLOW: 24������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 25��У��õ������ݣ������ò����κ�����
//@EXECUTIONFLOW: 26������CSUDIFILTERFree�ͷ�filter8~filter13
//@EXECUTIONFLOW: 27������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 ���е�filter����,���������� + 2 = ����
//@EXECUTIONFLOW: 28������CSUDIFILTERFree�ͷ�filter14~filter15
//@EXECUTIONFLOW: 29������CSUDIDEMUXGetFreeFilterCount��ȡdemux 0 ���е�filter����,���������� = ����
//@EXECUTIONFLOW: 30���ظ�2-29����10��
CSUDI_BOOL CSTC_SECTION_IT_0005(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nFilterIndex = 0;
	int nLoop = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*һ��ֻ��32��filter*/


	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCapability(DEMUX_0, &sTmpCap),
			"Get DEMUX_0 Filters count failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nTotalFilteCount = sTmpCap.m_nFilterNum) > 0,
			"Get Demux filter count < 0 !!"
		);
	CSTCPrint("DEMUX_0 has filters( %d )\n",nTotalFilteCount);
	
	do{		//filter8
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_008,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_008,&ahFilter[EM_SECTION_PAT_FILTER_008])),
					"Step 2 Allocate filter8 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_008]),
					"Step 2 Allocate filter8failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_008],(void*)EM_SECTION_PAT_FILTER_008)),
					"Step 3 Start Filter8 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_008,CSUDI_TRUE)),
					"Step 4 Check Expected Data Failure !!!"
				);
			//filter9
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_009,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_009,&ahFilter[EM_SECTION_PAT_FILTER_009])),
					"Step 5 Allocate filter9 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_009]),
					"Step 5 Allocate filter9 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_009],(void*)EM_SECTION_PAT_FILTER_009)),
					"Step 6 Start filter9 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_009,CSUDI_TRUE)),
					"Step 7 Check Expected Data Failure !!!"
				);
			//filter10
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_010,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_010,&ahFilter[EM_SECTION_PAT_FILTER_010])),
					"Step 8 Allocate filter10 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_010]),
					"Step 8 Allocate filter10 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_010],(void*)EM_SECTION_PAT_FILTER_010)),
					"Step 9 Start filter10 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_010,CSUDI_TRUE)),
					"Step 10 Expected No Data Failure !!!"
				);
			//filter11
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_011,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_011,&ahFilter[EM_SECTION_PAT_FILTER_011])),
					"Step 11 Allocate filter11 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_011]),
					"Step 11 Allocate filter11 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_011],(void*)EM_SECTION_PAT_FILTER_011)),
					"Step 12 Start filter11 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_011,CSUDI_TRUE)),
					"Step 13 Check Expected Data Failure !!!"
				);
			//filter12
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_012,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_012,&ahFilter[EM_SECTION_PAT_FILTER_012])),
					"Step 14 Allocate filter12 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_012]),
					"Step 14 Allocate filter12 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_012],(void*)EM_SECTION_PAT_FILTER_012)),
					"Step 15 Start filter12 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_012,CSUDI_TRUE)),
					"Step 16 Check Expected Data Failure !!!"
				);
			//filter13
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_013,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_013,&ahFilter[EM_SECTION_PAT_FILTER_013])),
					"Step 17 Allocate filter13 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_013]),
					"Step 17 Allocate filter13 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_013],(void*)EM_SECTION_PAT_FILTER_013)),
					"Step 18 Start filter13 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_013,CSUDI_TRUE)),
					"Step 19 Check Expected Data Failure !!!"
				);	
			//filter14
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_014,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_014,&ahFilter[EM_SECTION_PAT_FILTER_014])),
					"Step 20 Allocate filter14 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_014]),
					"Step 20 Allocate filter14 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_014],(void*)EM_SECTION_PAT_FILTER_014)),
					"Step 21 Start filter14 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_014,CSUDI_TRUE)),
					"Step 22 Expected No Data Failure !!!"
				);
			//filter15
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_015,DEMUX_0,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_015,&ahFilter[EM_SECTION_PAT_FILTER_015])),
					"Step 23 Allocate filter15 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_015]),
					"Step 23 Allocate filter15 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_015],(void*)EM_SECTION_PAT_FILTER_015)),
					"Step 24 Start filter15 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_015,CSUDI_TRUE)),
					"Step 25 Expected No Data Failure !!!"
				);
			//free filter8
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_008],(void*)EM_SECTION_PAT_FILTER_008)),
						"Step 26  Free Filter8 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_008] = CSUDI_NULL;
			}
			//free filter9
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_009],(void*)EM_SECTION_PAT_FILTER_009)),
						"Step 26  Free Filter9 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_009] = CSUDI_NULL;
			}	
			//free filter10
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_010],(void*)EM_SECTION_PAT_FILTER_010)),
						"Step 26  Free Filter10 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_010] = CSUDI_NULL;
			}
			//free filter11
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_011],(void*)EM_SECTION_PAT_FILTER_011)),
						"Step 26  Free Filter11 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_011] = CSUDI_NULL;
			}
			//free filter12
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_012],(void*)EM_SECTION_PAT_FILTER_012)),
						"Step 26  Free Filter12 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_012] = CSUDI_NULL;
			}
			//free filter13
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_013],(void*)EM_SECTION_PAT_FILTER_013)),
						"Step 26  Free Filter13 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_013] = CSUDI_NULL;
			}
			nFreeFilteCount = 0;
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(DEMUX_0,&nFreeFilteCount)),
						"Step 27 Get Free Filters Failure !!!"
					);
			CSTCPrint("DEMUX_0 Current free filters is: %d\n",nFreeFilteCount);
			CSTK_ASSERT_TRUE(
						((nFreeFilteCount + 2) == nTotalFilteCount),
						"Step 27 Free Filters should be((nFreeFilteCount + 2) == nTotalFilteCount) !!!"
					);
			//free filter14
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_014],(void*)EM_SECTION_PAT_FILTER_014)),
						"Step 28  Free Filter14 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_014] = CSUDI_NULL;
			}
			//free filter15
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_015],(void*)EM_SECTION_PAT_FILTER_015)),
						"Step 28  Free Filter15 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_015] = CSUDI_NULL;
			}
			CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_SUCCESS == CSUDIDEMUXGetFreeFilterCount(DEMUX_0,&nFreeFilteCount)),
						"Step 29 Get Free Filters Failure !!!"
					);
			CSTK_ASSERT_TRUE(
						(nFreeFilteCount == nTotalFilteCount),
						"Step 29 Free Filters should be(nFreeFilteCount == nTotalFilteCount) !!!"
					);
		}while((++nLoop) < TESTTIMES);

	CSTK_FATAL_POINT
	for(nFilterIndex = EM_SECTION_PAT_FILTER_008; nFilterIndex <= EM_SECTION_PAT_FILTER_015 ; nFilterIndex ++)
	{	
		if(CSUDI_NULL != ahFilter[nFilterIndex])
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[nFilterIndex],(void*)nFilterIndex)),
					"Free Filter Failure !!!"
				);
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ����FilterΪCAT��������
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��CAT_FILTER001 (��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿһ���õ�����ֵ
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����CAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����filter�����������ɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪCAT_FILTER001ָ��������
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 6���ظ�2-5����10��
CSUDI_BOOL CSTC_SECTION_IT_0006(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	int nLoop = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	do{
		SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_CAT_FILTER_001,DEMUX_0,EM_CAT_PID);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_CAT_FILTER_001,&hFilter)),
				"Step 2 Allocate filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_CAT_FILTER_001)),
				"Step 3 Start filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�1s*/
				(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_CAT_FILTER_001,CSUDI_TRUE)),
				"Step 4 Check Expected Data Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_CAT_FILTER_001)),
				"Step 5 Free Filter Failure !!!"
			);	
		hFilter = CSUDI_NULL;
	}while((++nLoop) < TESTTIMES);
	
	CSTK_FATAL_POINT
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_CAT_FILTER_001)),
				"Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERFree
//@DESCRIPTION: ����FilterΪ��Ƶ��������
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��VIDEO_FILTER001~VIDEO_FILTER002 (��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿһ���õ�����ֵ
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����VIDEO_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����filter�����������ɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ������õ�����(�˴��������ݽ���У��)
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 6������CSUDIFILTERAllocate����VIDEO_FILTER002ָ����filter����������ɹ�
//@EXECUTIONFLOW: 7������CSUDIFILTERStart��ʼ����VIDEO_FILTER002ָ��������
//@EXECUTIONFLOW: 8��У��õ������ݣ������õ�����(�˴��������ݽ���У��)
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 10���ظ�2-9����10��
CSUDI_BOOL CSTC_SECTION_IT_0007(void)
{
#if 0
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;
	int nLoop = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	do{
		SECTION_iSetPESReq(&sTmpReqInfo,EM_SECTION_PES_VIDEO_001,DEMUX_0,EM_VIDEO_001_PID);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PES_VIDEO_001,&hFilter1)),
				"Step 2 Allocate filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PES_VIDEO_001)),
				"Step 3 Start filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
				(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PES_VIDEO_001,CSUDI_FALSE)),
				"Step 4 Check Expected Data Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PES_VIDEO_001)),
				"Step 5 Start filter Failure !!!"
			)
		hFilter1 = CSUDI_NULL;
		SECTION_iSetPESReq(&sTmpReqInfo,EM_SECTION_PES_VIDEO_002,DEMUX_0,EM_VIDEO_002_PID);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PES_VIDEO_002,&hFilter2)),
				"Step 6 Allocate filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PES_VIDEO_002)),
				"Step 7 Start filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
				(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PES_VIDEO_002,CSUDI_FALSE)),
				"Step 8 Check Expected Data Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PES_VIDEO_002)),
				"Step 9 Start filter Failure !!!"
			);
		hFilter2 = CSUDI_NULL;
	}while((++nLoop) < 10);
	
	CSTK_FATAL_POINT
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PES_VIDEO_001)),
				"Free Filter Failure !!!"
			);
	}
	if(hFilter2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PES_VIDEO_002)),
				"Free Filter Failure !!!"
			);
	}
#endif
	return CSUDI_TRUE;
}
//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����FilterΪ��Ƶ��������
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��AUDIO_FILTER001~AUDIO_FILTER002 (��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿһ���õ�����ֵ
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����AUDIO_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����filter�����������ɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ������õ�����(�˴��������ݽ���У��)
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 6������CSUDIFILTERAllocate����AUDIO_FILTER002ָ����filter����������ɹ�
//@EXECUTIONFLOW: 7������CSUDIFILTERStart��ʼ����AUDIO_FILTER002ָ��������
//@EXECUTIONFLOW: 8��У��õ������ݣ������õ�����(�˴��������ݽ���У��)
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 10���ظ�2-9����10��
CSUDI_BOOL CSTC_SECTION_IT_0008(void)
{
#if 0
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;
	int nLoop = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	do{
		SECTION_iSetPESReq(&sTmpReqInfo,EM_SECTION_PES_AUDIO_001,DEMUX_0,EM_AUDIO_001_PID);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PES_AUDIO_001,&hFilter1)),
				"Step 2 Allocate filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PES_AUDIO_001)),
				"Step 3 Start filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
				(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PES_AUDIO_001,CSUDI_TRUE)),
				"Step 4 Check Expected Data Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PES_AUDIO_001)),
				"Step 5 Start filter Failure !!!"
			);
		hFilter1 = CSUDI_NULL;
		
		SECTION_iSetPESReq(&sTmpReqInfo,EM_SECTION_PES_AUDIO_002,DEMUX_0,EM_AUDIO_002_PID);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PES_AUDIO_002,&hFilter2)),
				"Step 6 Allocate filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PES_AUDIO_002)),
				"Step 7 Start filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
				(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PES_AUDIO_002,CSUDI_TRUE)),
				"Step 8 Check Expected Data Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PES_AUDIO_002)),
				"Step 9 Start filter Failure !!!"
			);
		hFilter2 = CSUDI_NULL;
	}while((++nLoop) < 10);
	
	CSTK_FATAL_POINT
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PES_AUDIO_001)),
				"Free Filter Failure !!!"
			);
	}
	if(hFilter2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PES_AUDIO_002)),
				"Free Filter Failure !!!"
			);
	}
#endif
	return CSUDI_TRUE;
}
//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����FilterΪsubtitle��������
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��SUBTITLE_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿһ���õ�����ֵ
//@EXECUTIONFLOW: 1������Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����SUBTITLE_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����filter�����������ɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪSUBTITLE_FILTER001ָ��������(������Ӧ���������İ�����)
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 6���ظ�2-5����10��
CSUDI_BOOL CSTC_SECTION_IT_0009(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	int nLoop = 0;

	CSTCPrint("���������: %s \r\n",TELETEXT_SUBTITLE_25MBPS_071220_TS);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,TELETEXT_SUBTITLE_25MBPS_071220_TS)),
			"Step 1 lock frequence failure !!!"
		);
	do{
		SECTION_iSetPESReq(&sTmpReqInfo,EM_SECTION_PES_SUBTITLE_001,DEMUX_0,EM_SUBTITLE_001_PID);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PES_SUBTITLE_001,&hFilter1)),
				"Step 2 Allocate filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PES_SUBTITLE_001)),
				"Step 3 Start filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�5s*/
				(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PES_SUBTITLE_001,CSUDI_FALSE)),
				"Step 4 Check Expected Data Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PES_SUBTITLE_001)),
				"Step 5 Start filter Failure !!!"
			);
		hFilter1 = CSUDI_NULL;
	}while((++nLoop) < 10);
	
	CSTK_FATAL_POINT
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PES_SUBTITLE_001)),
				"Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����FilterΪteletext��������
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��TELETEXT_FILTER001 (��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿһ���õ�����ֵ
//@EXECUTIONFLOW: 1������Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����TELETEXT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����filter�����������ɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪTELETEXT_FILTER001ָ��������
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 6���ظ�2-5����10��
CSUDI_BOOL CSTC_SECTION_IT_0010(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	int nLoop = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,TELETEXT_SUBTITLE_25MBPS_071220_TS)),
			"Step 1 lock frequence failure !!!"
		);
	do{
		SECTION_iSetPESReq(&sTmpReqInfo,EM_SECTION_PES_TELETEXT_001,DEMUX_0,EM_TELETEXT_001_PID);
		
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PES_TELETEXT_001,&hFilter1)),
				"Step 2 Allocate filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PES_TELETEXT_001)),
				"Step 3 Start filter Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
				(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PES_TELETEXT_001,CSUDI_TRUE)),
				"Step 4 Check Expected Data Failure !!!"
			);
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PES_TELETEXT_001)),
				"Step 5 Start filter Failure !!!"
			);
		hFilter1 = CSUDI_NULL;
	}while((++nLoop) < 10);
	
	CSTK_FATAL_POINT
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PES_TELETEXT_001)),
				"Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ����������������ͬ�Ĺ�����������ͬ�Ļص��������û����ݵ�filterʱ����������һ��Ӧ�ò�Ӱ����һ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2����ͬ�Ļص�����callback1��callback2 
//@EXPECTATION: ֹͣ�����ͷ�����һ��filter������Ӱ����һ��filter��������������
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate(PAT_FILTER001,callback1,userdata1,filter1)����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate(PAT_FILTER001,callback2,userdata2,filter2)����filter2����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart����filter1��filter2�����������ɹ�
//@EXECUTIONFLOW: 5��У��õ������ݣ�����callback1��callback2�õ������ݶ�ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 6������CSUDIFILTERStopֹͣfilter1
//@EXECUTIONFLOW: 7��У��õ������ݣ�����callback1���ղ����κ�������callback2���Խ��յ�����
//@EXECUTIONFLOW: 8������CSUDIFILTERFree�ͷ�filter1�������ͷųɹ�
//@EXECUTIONFLOW: 9��У��õ������ݣ�����callback1���ղ����κ�������callback2���Խ��յ�����
//@EXECUTIONFLOW: 10������CSUDIFILTERFree�ͷ�filter2
//@EXECUTIONFLOW: 11���ظ�2~10����10��
CSUDI_BOOL CSTC_SECTION_IT_0011(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo1;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;
	int nLoop = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	do{
			SECTION_iSetSectionFilter(&sTmpReqInfo1,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,&hFilter1)),
					"Step 2 Allocate filter1 Failure !!!"
				);
            
			//set userdata2
			SECTION_iSetSectionFilter(CSUDI_NULL,EM_SECTION_PAT_FILTER_002,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_002,&hFilter2)),
					"Step 3 Allocate filter2 Failure !!!"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 4 Start filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_002)),
					"Step 4 Start filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 5 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_002,CSUDI_TRUE)),
					"Step 5 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 6 Stop filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 7 Filter1 Expected No Data Failure !!!"
				);	
			//����filter2�Ƿ��ȡ����
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_002,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_002,CSUDI_TRUE)),
					"Step 7 Filter2 Expected Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 8 Free filter1 Failure !!!"
				);
			hFilter1 = CSUDI_NULL;
			
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 9 Filter1 Expected No Data Failure !!!"
				);	
			//����filter2�Ƿ��ȡ����
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_002,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_002,CSUDI_TRUE)),
					"Step 7 Filter2 Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_002)),
					"Step 8 Free filter2 Failure !!!"
				);	
			hFilter2 = CSUDI_NULL;
	}while((++nLoop)<10);

	CSTK_FATAL_POINT;
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
				"Free filter1 Failure !!!"
			);	
	}
	if(hFilter2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_002)),
				"Free filter2 Failure !!!"
			);	
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ����������������ͬ�Ĺ�����������ͬ�Ļص���������ͬ���û�����ʱ��filterʱ����������һ��Ӧ�ò�Ӱ����һ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: 1��PAT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2����ͬ�Ļص�����callback
//@EXPECTATION: ֹͣ�����ͷ�����һ��filter������Ӱ����һ��filter��������������
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate(PAT_FILTER001,callback,userdata1,filter1)����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate(PAT_FILTER001,callback,userdata2,filter2)����filter2����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart����filter1��filter2�����������ɹ�
//@EXECUTIONFLOW: 5��У��õ������ݣ�����callback�õ�(����)���ݶ�ΪPAT_FILTER001ָ��������,�����û����ݷֱ�Ϊuserdata1��userdata2
//@EXECUTIONFLOW: 6������CSUDIFILTERStopֹͣfilter1
//@EXECUTIONFLOW: 7��У��õ������ݣ�����callback���ղ����û�����Ϊuserdata1�����ݵ����Խ��յ��û�����Ϊuserdata2����
//@EXECUTIONFLOW: 8������CSUDIFILTERFree�ͷ�filter1�������ͷųɹ�
//@EXECUTIONFLOW: 9��У��õ������ݣ�����callback���ղ����κ��û�����Ϊuserdata1�����Խ��յ��û�����Ϊuserdata2����
//@EXECUTIONFLOW: 10������CSUDIFILTERFree�ͷ�filter2
//@EXECUTIONFLOW: 11���ظ�2~10����10��
CSUDI_BOOL CSTC_SECTION_IT_0012(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;
	int nLoop = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	do{
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter1)),
					"Step 2 Allocate filter1 Failure !!!"
				);
			//set userdata2
			SECTION_iSetSectionFilter(CSUDI_NULL,EM_SECTION_PAT_FILTER_003,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,&hFilter2)),
					"Step 3 Allocate filter2 Failure !!!"
				);

			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 4 Start filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_003)),
					"Step 4 Start filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 5 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,CSUDI_TRUE)),
					"Step 5 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 6 Stop filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 7 Filter1 Expected No Data Failure !!!"
				);	
			//����filter2�Ƿ��ȡ����
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_003,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,CSUDI_TRUE)),
					"Step 7 Filter2 Expected Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 8 Free filter1 Failure !!!"
				);
			hFilter1 = CSUDI_NULL;
			
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 9 Filter1 Expected No Data Failure !!!"
				);	
			//����filter2�Ƿ��ȡ����
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_003,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,CSUDI_TRUE)),
					"Step 7 Filter2 Expected Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_003)),
					"Step 8 Free filter2 Failure !!!"
				);	
			hFilter2 = CSUDI_NULL;
	}while((++nLoop)<10);

	CSTK_FATAL_POINT
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
				"Free filter1 Failure !!!"
			);	
	}
	if(hFilter2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_003)),
				"Free filter2 Failure !!!"
			);	
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ���������������ص��Ĺ�����������ͬ�Ļص�������filterʱ����������һ��Ӧ�ò�Ӱ����һ��filter
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001��PAT_FILTER004(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2����ͬ�Ļص�����callback1��callback2 
//@EXPECTATION: ֹͣ�����ͷ�����һ��filter(��ͬ˳��)������Ӱ����һ��filter��������������
//@EXECUTIONFLOW: ����1-10����������ͷ�PAT_FILTER001��filter���ͷ�PAT_FILTER004��filter
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate(PAT_FILTER001,callback1,userdata1,filter1)����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate(PAT_FILTER004,callback2,userdata2,filter2)����filter2����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart����filter1��filter2�����������ɹ�
//@EXECUTIONFLOW: 5��У��õ������ݣ�����callback1��callback2�õ������ݷֱ�ΪPAT_FILTER001��PAT_FILTER004ָ��������
//@EXECUTIONFLOW: 6������CSUDIFILTERStopֹͣfilter1
//@EXECUTIONFLOW: 7��У��õ������ݣ�����callback1���ղ����κ�������callback2���Խ��յ�����
//@EXECUTIONFLOW: 8������CSUDIFILTERFree�ͷ�filter1�������ͷųɹ�
//@EXECUTIONFLOW: 9��У��õ������ݣ�����callback1���ղ����κ�������callback2���Խ��յ�����
//@EXECUTIONFLOW: 10������CSUDIFILTERFree�ͷ�filter2
//@EXECUTIONFLOW: ����11-19����������ͷ�PAT_FILTER004��filter���ͷ�PAT_FILTER001��filter
//@EXECUTIONFLOW: 11������CSUDIFILTERAllocate(PAT_FILTER001,callback1,userdata1,filter1)����filter1����������ɹ�
//@EXECUTIONFLOW: 12������CSUDIFILTERAllocate(PAT_FILTER004,callback2,userdata2,filter2)����filter2����������ɹ�
//@EXECUTIONFLOW: 13������CSUDIFILTERStart����filter1��filter2�����������ɹ�
//@EXECUTIONFLOW: 14��У��õ������ݣ�����callback1��callback2�õ������ݷֱ�ΪPAT_FILTER001��PAT_FILTER004ָ��������
//@EXECUTIONFLOW: 15������CSUDIFILTERStopֹͣfilter2
//@EXECUTIONFLOW: 16��У��õ������ݣ�����callback2���ղ����κ�������callback1���Խ��յ�����
//@EXECUTIONFLOW: 17������CSUDIFILTERFree�ͷ�filter2�������ͷųɹ�
//@EXECUTIONFLOW: 18��У��õ������ݣ�����callback2���ղ����κ�������callback1���Խ��յ�����
//@EXECUTIONFLOW: 19������CSUDIFILTERFree�ͷ�filter1
//@EXECUTIONFLOW: 11���ظ�1~19����10��
CSUDI_BOOL CSTC_SECTION_IT_0013(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo1;
	CSUDISECTIONRequestInfo_S sTmpReqInfo2;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;
	int nLoop = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	do{
			SECTION_iSetSectionFilter(&sTmpReqInfo1,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,&hFilter1)),
					"Step 2 Allocate filter1 Failure !!!"
				);
			
			SECTION_iSetSectionFilter(&sTmpReqInfo2,EM_SECTION_PAT_FILTER_004,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,&hFilter2)),
					"Step 3 Allocate filter2 Failure !!!"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 4 Start filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 4 Start filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 5 Filter1 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 5 Filter2 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 6 Stop filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
					"Step 7 Filter1 Expected No Data Failure !!!"
				);	
			//����filter2�Ƿ��ȡ����
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_004,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 7 Filter2 Expected Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 8 Free filter1 Failure !!!"
				);
			hFilter1 = CSUDI_NULL;
			
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
					"Step 9 Filter1 Expected No Data Failure !!!"
				);	
			//����filter2�Ƿ��ȡ����
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_004,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 9 Filter2 Expected Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 10 Free filter2 Failure !!!"
				);	
			hFilter2 = CSUDI_NULL;

			SECTION_iSetSectionFilter(&sTmpReqInfo1,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,&hFilter1)),
					"Step 11 Allocate filter1 Failure !!!"
				);
			
			SECTION_iSetSectionFilter(&sTmpReqInfo2,EM_SECTION_PAT_FILTER_004,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,&hFilter2)),
					"Step 12 Allocate filter2 Failure !!!"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 13 Start filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 13 Start filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�1s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 14 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�1s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 14 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 15 Stop filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
					"Step 16 Filter1 Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_FALSE)),
					"Step 16 Filter2 Expected No Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 17 Free filter2 Failure !!!"
				);
			hFilter2 = CSUDI_NULL;
			
			//����filter1�Ƿ��ȡ����
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_001,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 18 Filter1 Expected Data Failure !!!"
				);	

			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_FALSE)),
					"Step 18 Filter2 Expected No Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 19 Free filter2 Failure !!!"
				);	
			hFilter1 = CSUDI_NULL;
	}while((++nLoop)<10);
	
	CSTK_FATAL_POINT
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
				"Free filter1 Failure !!!"
			);	
	}
	if(hFilter2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
				"Free filter2 Failure !!!"
			);	
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ���������������ص��Ĺ��������Ҳ�ͬ�Ļص�������filterʱ���ֱ��޸�����filterΪ�Է��Ĺ�������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001��PAT_FILTER004(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2����ͬ�Ļص�����callback1��callback2 
//@EXPECTATION: �޸�֮���Կ����������յ�����
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate(PAT_FILTER001,callback1,userdata1,filter1)����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate(PAT_FILTER004,callback2,userdata2,filter2)����filter2����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart����filter1��filter2�����������ɹ�
//@EXECUTIONFLOW: 5��У��õ������ݣ�����callback1��callback2�õ������ݶ�ΪPAT_FILTER001ָ��������(PAT_FILTER001��PAT_FILTER004ָ����������һ����)
//@EXECUTIONFLOW: 6������CSUDIFILTERStopֹͣfilter1��filter2,�����ɹ�
//@EXECUTIONFLOW: 7������CSUDIFILTERModify�޸�filter1ΪPAT_FILTER004���������������ɹ�
//@EXECUTIONFLOW: 8������CSUDIFILTERModify�޸�filter2ΪPAT_FILTER001���������������ɹ�
//@EXECUTIONFLOW: 9������CSUDIFILTERStart����filter1��filter2�����������ɹ�
//@EXECUTIONFLOW: 10��У��õ������ݣ�����filter1�õ�������ΪPAT_FILTER004ָ�������ݡ�filter2�õ�������ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 11������CSUDIFILTERFree�ͷ�filter1�������ͷųɹ�
//@EXECUTIONFLOW: 12��У��õ������ݣ�����callback1���ղ����κ�������callback2���Խ��յ�����
//@EXECUTIONFLOW: 13������CSUDIFILTERFree�ͷ�filter2
//@EXECUTIONFLOW: 11���ظ�2~13����10��
CSUDI_BOOL CSTC_SECTION_IT_0014(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo1;
	CSUDISECTIONRequestInfo_S sTmpReqInfo2;
	CSUDIFILTERMatchMask_S sMatch;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;
	int nLoop = 0;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	do{
			SECTION_iSetSectionFilter(&sTmpReqInfo1,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,&hFilter1)),
					"Step 2 Allocate filter1 Failure !!!"
				);
			
			SECTION_iSetSectionFilter(&sTmpReqInfo2,EM_SECTION_PAT_FILTER_004,DEMUX_0,EM_PAT_PID);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,&hFilter2)),
					"Step 3 Allocate filter2 Failure !!!"
				);
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 4 Start filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 4 Start filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�1s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 5 filter1 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�1s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 5 filter2 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 6 Stop filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 6 Stop filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					SECTION_iSetMatchMask(&sMatch,EM_SECTION_PAT_FILTER_004),
					"Set filter value failure at step 7 !!!"
					); //modify filter1 to filter4
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iModifyFilter(hFilter1,&sMatch,CSUDI_TRUE,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 7 Modify filter1 Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					SECTION_iSetMatchMask(&sMatch,EM_SECTION_PAT_FILTER_001),
					"Set filter value failure at step 8 !!!"
					); //modify filter4 to filter1
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iModifyFilter(hFilter2,&sMatch,CSUDI_TRUE,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 8 Modify filter2 Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 9 Start filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 9 Start filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*///filter2
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 10 filter2 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*///fiter1
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 10 filter1 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 11 Free filter1 Failure !!!"
				);	
			hFilter1 = CSUDI_NULL;
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*///fiter1
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
					"Step 12 Filter1 Check Expected No Data Failure !!!"
				);	
			
			//����filter2�Ƿ��ȡ����
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_004,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*///fiter2
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 12 Filter2 Check Expected Data Failure !!!"
				);	
			
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 13 Free filter2 Failure !!!"
				);	
			hFilter2 = CSUDI_NULL;
	}while((++nLoop)<10);

	CSTK_FATAL_POINT
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
				"Free filter1 Failure !!!"
			);	
	}
	if(hFilter2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
				"Free filter2 Failure !!!"
			);	
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ����������filterȥ���˴��ڶ��section_num��PID����������һ��Ӧ�ò�Ӱ������filter
//@DESCRIPTION: �����������������section_num:filter1������section_num��filter2����section_num_0��filter3����section_num_1
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��EIT_FILTER001��EIT_FILTER002��EIT_FILTER003(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2����ͬ�Ļص�����callback1��callback2 
//@EXPECTATION: �޸�ÿ��filter֮������filter�Կ����������յ�����
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate(EIT_FILTER001,callback1,userdata1,filter1)����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate(EIT_FILTER002,callback2,userdata2,filter2)����filter2����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERAllocate(EIT_FILTER003,callback3,userdata3,filter3)����filter3����������ɹ�
//@EXECUTIONFLOW: 5������CSUDIFILTERStart����filter1��filter2��filter3�����������ɹ�
//@EXECUTIONFLOW: 6��У��õ�������,������5s����ȫ����:
//@EXECUTIONFLOW: (1)����callback1���Եõ�EIT_FILTER001ָ�������ݣ�
//@EXECUTIONFLOW: (2)����callback2���Եõ�EIT_FILTER002ָ�������ݣ�
//@EXECUTIONFLOW: (3)����callback3���Եõ�EIT_FILTER003ָ�������ݣ�
//@EXECUTIONFLOW: 7������CSUDIFILTERStopֹͣfilter1��filter2,�����ɹ�
//@EXECUTIONFLOW: 8��У��õ������ݣ�����callback1��callback2�ò������ݣ�callback3���Եõ�EIT_FILTER003ָ��������
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�filter1��filter2�������ͷųɹ�
//@EXECUTIONFLOW: 10��У��õ������ݣ�����callback1��callback2���ղ����κ�������callback3���Խ��յ�����
//@EXECUTIONFLOW: 11������CSUDIFILTERFree�ͷ�filter3
CSUDI_BOOL CSTC_SECTION_IT_0015(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo1;
	CSUDISECTIONRequestInfo_S sTmpReqInfo2;
	CSUDISECTIONRequestInfo_S sTmpReqInfo3;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;
	CSUDI_HANDLE hFilter3 = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	SECTION_iSetSectionFilter(&sTmpReqInfo1,EM_SECTION_EIT_FILTER_001,DEMUX_0,EM_EIT_PID);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo1,(void*)EM_SECTION_EIT_FILTER_001,&hFilter1)),
			"Step 2 Allocate filter1 Failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sTmpReqInfo2,EM_SECTION_EIT_FILTER_002,DEMUX_0,EM_EIT_PID);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo2,(void*)EM_SECTION_EIT_FILTER_002,&hFilter2)),
			"Step 3 Allocate filter2 Failure !!!"
		);
	SECTION_iSetSectionFilter(&sTmpReqInfo3,EM_SECTION_EIT_FILTER_003,DEMUX_0,EM_EIT_PID);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo3,(void*)EM_SECTION_EIT_FILTER_003,&hFilter3)),
			"Step 4 Allocate filter3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_EIT_FILTER_001)),
			"Step 5 Start filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_EIT_FILTER_002)),
			"Step 5 Start filter2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter3,(void*)EM_SECTION_EIT_FILTER_003)),
			"Step 5 Start filter3 Failure !!!"
		);
	//check data begin should get two section
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo1,(void*)EM_SECTION_EIT_FILTER_001, CSUDI_TRUE)),
			"Step 6 Expected Data at filter1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo2,(void*)EM_SECTION_EIT_FILTER_002, CSUDI_TRUE)),
			"Step 6 Expected Data at filter2 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo3,(void*)EM_SECTION_EIT_FILTER_003, CSUDI_TRUE)),
			"Step 6 Expected Data at filter2 failure !!!"
		);
	//check data end
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter1, (void*)EM_SECTION_EIT_FILTER_001)),
			"Step 7 Stop filter1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter2, (void*)EM_SECTION_EIT_FILTER_002)),
			"Step 7 Stop filter2 failure !!!"
		);
	//check data begin
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE, &sTmpReqInfo1,(void*)EM_SECTION_EIT_FILTER_001, CSUDI_FALSE)),
			"Step 8 Expected No Data at filter1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE, &sTmpReqInfo2,(void*)EM_SECTION_EIT_FILTER_002, CSUDI_FALSE)),
			"Step 8 Expected No Data at filter2 failure !!!"
		);
	//reset callback
	SECTION_iSetCallbackStatus(EM_SECTION_EIT_FILTER_003,CSUDI_FALSE,CSUDI_FALSE);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo3,(void*)EM_SECTION_EIT_FILTER_003, CSUDI_TRUE)),
			"Step 8 Expected Data at filter2 failure !!!"
		);
	//check data end
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1, (void*)EM_SECTION_EIT_FILTER_001)),
			"Step 9 Free filter1 failure !!!"
		);	
	hFilter1 = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2, (void*)EM_SECTION_EIT_FILTER_001)),
			"Step 9 Free filter2 failure !!!"
		);	
	hFilter2 = CSUDI_NULL;
	//check data begin
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE, &sTmpReqInfo1,(void*)EM_SECTION_EIT_FILTER_001, CSUDI_FALSE)),
			"Step 10 Expected No Data at filter1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE, &sTmpReqInfo2,(void*)EM_SECTION_EIT_FILTER_002, CSUDI_FALSE)),
			"Step 10 Expected No Data at filter2 failure !!!"
		);
	//reset callback
	SECTION_iSetCallbackStatus(EM_SECTION_EIT_FILTER_003,CSUDI_FALSE,CSUDI_FALSE);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE, &sTmpReqInfo3,(void*)EM_SECTION_EIT_FILTER_003, CSUDI_TRUE)),
			"Step 10 Expected Data at filter2 failure !!!"
		);
 	//check data end
 	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iFreeFilter(hFilter3, (void*)EM_SECTION_EIT_FILTER_003)),
			"Step 11 Free filter3 failure !!!"
		);	
	hFilter3 = CSUDI_NULL;

	CSTK_FATAL_POINT
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_EIT_FILTER_001)),
				"Free filter1 Failure !!!"
			);	
	}
	if(hFilter2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_EIT_FILTER_002)),
				"Free filter2 Failure !!!"
			);	
	}
	if(hFilter3 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter3,(void*)EM_SECTION_EIT_FILTER_003)),
				"Free filter2 Failure !!!"
			);	
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ���Զ���߳�ȥ����ͬһPID�����ݣ�ÿ���߳���ͨ��modify���л�filter
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001~PAT_FILTER006(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT: 3������demux 0 �� tuner 0
//@EXPECTATION: ÿ���̰߳�Ԥ�ڻ�ȡ��δ��ȡ����
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2�������߳�1-3���в���
//@EXECUTIONFLOW: 3���ȴ��������������̲߳��Խ������߳�ʱ����������̲߳��Գɹ��������ɹ�������ʧ��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: �߳�1ִ�����¹���:
//@EXECUTIONFLOW: 1.1������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 1.2������CSUDIFILTERStart����filter1�����������ɹ�
//@EXECUTIONFLOW: 1.3��У��õ������ݣ�����ΪPAT_FILTER001
//@EXECUTIONFLOW: 1.4������CSUDIFILTERStopֹͣfilter1
//@EXECUTIONFLOW: 1.5������CSUDIFILTERModify�޸�filter1ΪPAT_FILTER002
//@EXECUTIONFLOW: 1.6������CSUDIFILTERStart����filter1�����������ɹ�
//@EXECUTIONFLOW: 1.7��У��õ������ݣ�����ΪPAT_FILTER002ָ��������
//@EXECUTIONFLOW: 1.8������CSUDIFILTERFree�ͷ�filter1���������سɹ�
//@EXECUTIONFLOW: 1.9���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW:�߳�2ִ�����¹���:
//@EXECUTIONFLOW: 2.1������CSUDIFILTERAllocate����PAT_FILTER003ָ����filter2����������ɹ�
//@EXECUTIONFLOW: 2.2������CSUDIFILTERStart����filter2�����������ɹ�
//@EXECUTIONFLOW: 2.3��У��PAT_FILTER003�����ݣ��������κ�����
//@EXECUTIONFLOW: 2.4������CSUDIFILTERStopֹͣfilter2
//@EXECUTIONFLOW: 2.5������CSUDIFILTERModify�޸�filter2ΪPAT_FILTER004
//@EXECUTIONFLOW: 2.6������CSUDIFILTERStart����filter2�����������ɹ�
//@EXECUTIONFLOW: 2.7��У��õ������ݣ�����ΪPAT_FILTER004ָ��������
//@EXECUTIONFLOW: 2.8������CSUDIFILTERFree�ͷ�filter2���������سɹ�
//@EXECUTIONFLOW: 2.9���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW:�߳�3ִ�����¹���:
//@EXECUTIONFLOW: 3.1������CSUDIFILTERAllocate����PAT_FILTER005ָ����filter3����������ɹ�
//@EXECUTIONFLOW: 3.2������CSUDIFILTERStart����filter3�����������ɹ�
//@EXECUTIONFLOW: 3.3��У��õ������ݣ�����ΪPAT_FILTER005ָ��������
//@EXECUTIONFLOW: 3.4������CSUDIFILTERStopֹͣfilter3
//@EXECUTIONFLOW: 3.5������CSUDIFILTERModify�޸�filter3ΪPAT_FILTER006
//@EXECUTIONFLOW: 3.6������CSUDIFILTERStart����filter3�����������ɹ�
//@EXECUTIONFLOW: 3.7��У�����ݣ�����ΪPAT_FILTER006ָ��������
//@EXECUTIONFLOW: 3.8������CSUDIFILTERFree�ͷ�filter3���������سɹ�
//@EXECUTIONFLOW: 3.9���ظ����Ϲ���5��
CSUDI_BOOL CSTC_SECTION_IT_0016(void)
{
	CSUDI_HANDLE hThread1 = CSUDI_NULL;
	CSUDI_HANDLE hThread2 = CSUDI_NULL;
	CSUDI_HANDLE hThread3 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	SECTION_iInitPowerCasePar();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Thread1",5*17, 32*1024, SECTION_IT_0016_Filter_Thread1, CSUDI_NULL, &hThread1)),
			"Step 2 Create Filter_Thread1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Thread2",5*17, 32*1024, SECTION_IT_0016_Filter_Thread2, CSUDI_NULL, &hThread2)),
			"Step 2 Create Filter_Thread2 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Thread3",5*17, 32*1024, SECTION_IT_0016_Filter_Thread3, CSUDI_NULL, &hThread3)),
			"Step 2 Create Filter_Thread3 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == SECTION_iGetPowerCallBackStatus()),
		"Step 3 Failure"
		);
	
	CSTK_FATAL_POINT
	if(CSUDI_NULL != hThread1)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread1)),
			"Destroy Filter_Thread1 Failure"
			);
	}
	if(CSUDI_NULL != hThread2)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread2)),
			"Destroy Filter_Thread2 Failure"
			);
	}
	if(CSUDI_NULL != hThread3)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread3)),
			"Destroy Filter_Thread3 Failure"
			);
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ���Զ���߳�ȥ����ͬPID������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001��CAT_FILTER001��EIT_FILTER001��TDT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT: 3������demux 0 �� tuner 0
//@EXPECTATION: ÿ���̰߳�Ԥ�ڻ�ȡ����
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2�������߳�1-4���в���
//@EXECUTIONFLOW: 3���ȴ��������������̲߳��Խ������߳�ʱ����������̲߳��Գɹ��������ɹ�������ʧ��
//@EXECUTIONFLOW: �߳�1ִ�����¹���:
//@EXECUTIONFLOW: 1.1������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 1.2������CSUDIFILTERStart����filter1�����������ɹ�
//@EXECUTIONFLOW: 1.3��У��õ������ݣ�����ΪPAT_FILTER001�������ֶ���
//@EXECUTIONFLOW: 1.4������CSUDIFILTERFree�ͷ�filter1���������سɹ�
//@EXECUTIONFLOW: 1.5���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: �߳�2ִ�����¹���:
//@EXECUTIONFLOW: 2.1������CSUDIFILTERAllocate����CAT_FILTER001ָ����filter2����������ɹ�
//@EXECUTIONFLOW: 2.2������CSUDIFILTERStart����filter2�����������ɹ�
//@EXECUTIONFLOW: 2.3��У��õ������ݣ�����ΪCAT_FILTER001�������ֶ���
//@EXECUTIONFLOW: 2.4������CSUDIFILTERFree�ͷ�filter2���������سɹ�
//@EXECUTIONFLOW: 2.5���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: �߳�3ִ�����¹���:
//@EXECUTIONFLOW: 3.1������CSUDIFILTERAllocate����EIT_FILTER001ָ����filter3����������ɹ�
//@EXECUTIONFLOW: 3.2������CSUDIFILTERStart����filter3�����������ɹ�
//@EXECUTIONFLOW: 3.3��У��õ������ݣ�����ΪEIT_FILTER001�������ֶ���
//@EXECUTIONFLOW: 3.4������CSUDIFILTERFree�ͷ�filter3���������سɹ�
//@EXECUTIONFLOW: 3.5���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: �߳�4ִ�����¹���:
//@EXECUTIONFLOW: 4.1������CSUDIFILTERAllocate����TDT_FILTER001ָ����filter4����������ɹ�
//@EXECUTIONFLOW: 4.2������CSUDIFILTERStart����filter4�����������ɹ�
//@EXECUTIONFLOW: 4.3��У��õ������ݣ�����ΪTDT_FILTER001�������ֶ���
//@EXECUTIONFLOW: 4.4������CSUDIFILTERFree�ͷ�filter4���������سɹ�
//@EXECUTIONFLOW: 4.5���ظ����Ϲ���5��
CSUDI_BOOL CSTC_SECTION_IT_0017(void)
{
	CSUDI_HANDLE hThread1 = CSUDI_NULL;
	CSUDI_HANDLE hThread2 = CSUDI_NULL;
	CSUDI_HANDLE hThread3 = CSUDI_NULL;
	CSUDI_HANDLE hThread4 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	SECTION_iInitRequestCasePar();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Pat_Thread",5*17, 32*1024, ThreadWithFilterNum, (void*)EM_SECTION_PAT_FILTER_001, &hThread1)),
			"Step 2 Create Filter_Pat_Thread failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Cat_Thread",5*17, 32*1024, ThreadWithFilterNum, (void*)EM_SECTION_CAT_FILTER_001, &hThread2)),
			"Step 2 Create Filter_Cat_Thread failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Eit_Thread",5*17, 32*1024, ThreadWithFilterNum, (void*)EM_SECTION_EIT_FILTER_001, &hThread3)),
			"Step 2 Create Filter_Eit_Thread failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Tdt_Thread",5*17, 32*1024, ThreadWithFilterNum, (void*)EM_SECTION_TDT_FILTER_001, &hThread4)),
			"Step 2 Create Filter_Tdt_Thread failure !!!"
		);
	CSTCPrint("������������...��ȴ�!!!");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == SECTION_iGetRequestCallBackStatus()),
		"Step 3 Failure"
		);

	CSTK_FATAL_POINT
	if(CSUDI_NULL != hThread1)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread1)),
			"Destroy Filter_Pat_Thread Failure"
			);
		hThread1 = CSUDI_NULL;
	}
	if(CSUDI_NULL != hThread2)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread2)),
			"Destroy Filter_Cat_Thread Failure"
			);
		hThread2 = CSUDI_NULL;
	}
	if(CSUDI_NULL != hThread3)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread3)),
			"Destroy Filter_Eit_Thread Failure"
			);
		hThread3 = CSUDI_NULL;
	}
	if(CSUDI_NULL != hThread4)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread4)),
			"Destroy Filter_Tdt_Thread Failure"
			);
		hThread4 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}
#if 1
static unsigned int   s_nEPGRecStartTime = 0;
static unsigned int   s_nEPGRecEndTime   = 0;
static int   s_nRecTime         = 0;
static int   s_nRecMaxTime      = 0;
static int   s_nRecMinTime      = 0;
static int   s_nRecAllTime      = 0;
static CSUDI_BOOL   s_bRecAllEIT      = CSUDI_FALSE;
static CSUDI_BOOL   s_bRecFirstFlag    = CSUDI_TRUE;
static CSUDI_HANDLE s_hEPGCallbackEvent = CSUDI_NULL;

static void *SECTION_iEPGCallBack(EPGCallbackType eType, void * pvParam)
{
	unsigned int nTempTime = 0;
	switch( eType )
	{

	case EPG_CURSERVICE_SIMPLE_PRESENT_FINISHED: //��ǰ����PresentEPG�ѵ�
	case EPG_CURSERVICE_SIMPLE_PRESENT_CHANGED:  //��ǰ����PresentEPG�ı�
		{
			nTempTime = CSUDIPlusOSGetTickCount();
			CSTCPrint( "[TestEPGCallBack]EPG_CURSERVICE_SIMPLE_PRESENT_CHANGED at %d\n",nTempTime - s_nEPGRecStartTime);
		}
		break;

	case EPG_CURSERVICE_SIMPLE_FOLLOW_FINISHED:  //��ǰ����FollowEPG�ѵ�
	case EPG_CURSERVICE_SIMPLE_FOLLOW_CHANGED:   //��ǰ����FollowEPG�ı�
		{		
			nTempTime = CSUDIPlusOSGetTickCount();
			CSTCPrint( "[TestEPGCallBack]EPG_CURSERVICE_SIMPLE_PRESENT_CHANGED at %d \n",nTempTime - s_nEPGRecStartTime);
		}
		break;

	case EPG_ALLSERVICE_SIMPLE_FINISHED:
		{
			nTempTime = CSUDIPlusOSGetTickCount();
			CSTCPrint( "[TestEPGCallBack]EPG_ALLSERVICE_SIMPLE_FINISHED  at %d\n", nTempTime - s_nEPGRecStartTime);
		}
		break;

	case EPG_CURSERVICE_SCHEDLE_FINISHED:        //һ��Service��ScheduleEPG��ȫ   
		{
			nTempTime = CSUDIPlusOSGetTickCount();
			CSTCPrint( "[TestEPGCallBack]EPG_CURSERVICE_SCHEDLE_FINISHED at %d \n", nTempTime - s_nEPGRecStartTime);
		}
		break;

	case EPG_ALLSERVICE_SCHEDLE_FINISHED:        //���������ScheduleEPG��ȫ
		{					
			if( s_bRecFirstFlag == CSUDI_FALSE)
			{
				break;
			}
			
			s_bRecFirstFlag = CSUDI_FALSE;
			
			s_nEPGRecEndTime= CSUDIPlusOSGetTickCount();
			
			s_nRecTime = s_nEPGRecEndTime - s_nEPGRecStartTime;

			if(s_nRecTime < s_nRecMinTime )
			{
				s_nRecMinTime = s_nRecTime;
			}

			if(s_nRecTime > s_nRecMaxTime )
			{
				s_nRecMaxTime = s_nRecTime;
			}

			if (s_nRecTime > 0)
			{
				s_nRecAllTime += s_nRecTime;
			}

			s_bRecAllEIT = CSUDI_TRUE;

			CSTCPrint( "[TestEPGCallBack]EPG_ALLSERVICE_SCHEDLE_FINISHED at %d,used time is %d\n", s_nEPGRecEndTime,s_nRecTime );
			CSUDIOSEventSet(s_hEPGCallbackEvent);
		}
		break;

	case EPG_MEMORY_FULL:                        //�ڴ治��
		CSTCPrint( "[TestEPGCallBack]EPG_MEMORY_FULL \n");
		break;

	case EPG_ANALYSE_PRIVATEDATA:
	case EPG_GET_PRIVATEDATA: 
	case EPG_FREE_PRIVATEDATA:
		{ 
			CSTCPrint( "[TestEPGCallBack]EPG_ANALYSE_PRIVATEDATA \n");
		} 
		break;
	default:
		break;
	}
	return CSUDI_NULL;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ��������������������,��ȡ���缶�������(����pf��schedule����)
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: 1��EPG��������Ϊ���缶��
//@INPUT: 2��EPG�洢����Ϊȫ������
//@INPUT: 3���ڴ��Ϊ7M
//@EXPECTATION: ÿ�����趼���Եõ������Ľ��
//@REMARK:����������DTVģ��
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2����ʼSectionSource��Դ���������EPG�ڴ��
//@EXECUTIONFLOW: 3������CreateEPG��ʼ��EPG��Դ���
//@EXECUTIONFLOW: 4������EPG��������Ϊ���缶������
//@EXECUTIONFLOW: 5������EPG���أ� �ȴ������ӣ��������������ڽ���������
//@EXECUTIONFLOW: 6��������ɺ󣬼�¼�������������ʱ��
//@EXECUTIONFLOW: 7���ظ�10�β���3-6�����в���ִ����󣬼����ܹ����յ�ʱ�䣬Ҫ��ƽ��ʱ������20��
CSUDI_BOOL CSTC_SECTION_IT_0018(void)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	HCSHANDLE hSection = NULL; 
	HCSHANDLE hEPGMemPool = NULL;
	HCSHANDLE hEPG = NULL;
	unsigned char* pucEpgMem = CSUDI_NULL;
	int i = 0;
	int nAveRecTime = 0;
	s_nRecAllTime = 0;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			((hSection = SECTION_iUSPSectionInit()) != NULL),
			"Init SourceSectiton Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			((pucEpgMem = (unsigned char*)CSUDIOSMalloc(7*MBYTE))!= CSUDI_NULL),
			"Malloc memory pool Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			((hEPGMemPool = CSUSPCreateMemoryPoolEx2(pucEpgMem,7*MBYTE,CSUDI_FALSE,CSUDI_TRUE))!= NULL),
			"Create memory pool Failure !!!"
		);
	s_nRecMinTime = 0x1fffffff;
	s_nRecMaxTime = 0;
	
	if( s_hEPGCallbackEvent == CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSEventCreate("EPGCallbackEvent", 0,&s_hEPGCallbackEvent)),
				"Create EPGCallbackEvent Failure !!!"
		);
	}
	
	for( i = 0; i < 10; i++ )
	{
		hEPG = CreateEPG(hSection, hEPGMemPool, (EPGCallBack)SECTION_iEPGCallBack, 
			EPG_REG_CURSERVICE_SIMPLE_PRESENT_CHANGED | EPG_REG_CURSERVICE_SIMPLE_FOLLOW_CHANGED | \
			EPG_REG_CURSERVICE_SCHEDLE_FINISHED | EPG_REG_ALLSERVICE_SCHEDLE_FINISHED | \
			EPG_REG_MEMORY_FULL  | \
			EPG_REG_ALLSERVICE_SIMPLE_FINISHED | EPG_REG_ALLSERVICE_SCHEDLE_FINISHED, 0, 0 );
		CSTK_ASSERT_TRUE((hEPG != NULL),"Create Epg Handle Failure !!!");
		
		SetEPGRecPolicy( hEPG,EPG_SEARCH_ORINETWORK , EPG_SEARCH_ORINETWORK );
		SetEPGSavePolicy( hEPG, EPG_SAVE_ALL,EPG_SAVE_ALL );
		SetEPGCheckParam( hEPG, CSUDI_FALSE, CSUDI_FALSE ); 
		
		s_nEPGRecStartTime = CSUDIPlusOSGetTickCount();
		
		s_bRecAllEIT = CSUDI_FALSE;
		s_bRecFirstFlag = CSUDI_TRUE;

		//���缶�������
		StartRecEPG(hEPG, EPG_TEST_ORIGNETWORKID,EPG_TEST_TSID, EPG_TEST_SERVID);
		CSTCPrint("[SECTION]�ȴ�:%d s \r\n",120);
		
		CSUDIOSEventWait(s_hEPGCallbackEvent, 120000);
		CSUDIOSEventReset(s_hEPGCallbackEvent);

		bRet = s_bRecAllEIT;
		if(!bRet)
		{
			CSTCPrint("[SECTION]�� %d �ν�������ʧ��",10-i-1);
			CSTK_ASSERT_TRUE(bRet,"δ���뵽���� !!!");
		}
		
		bRet = StopRecEPG(hEPG);
		CSTK_ASSERT_TRUE(bRet,"StopRecEPG ʧ��\n");

		bRet = DeleteAllEPG(hEPG);
		CSTK_ASSERT_TRUE(bRet,"DeleteAllEPG ʧ��\n");

		bRet = FreeEPG( hEPG);
		CSTK_ASSERT_TRUE(bRet,"FreeEPG ʧ��\n");

		hEPG = NULL;
		
		CSTCPrint("[SECTION]����%d��,��ѭ�����ɽ���,�����ĵȴ�\r\n",10-i-1);
		CSUDIOSThreadSleep(1000);
	}

	nAveRecTime = s_nRecAllTime/i;
	CSTCPrint( "[SECTION]ave time is  %d\n",nAveRecTime);
	if(bRet)
	{

		CSTCPrint(": ��̽���ʱ��%d��!\n",s_nRecMinTime/1000);
		CSTCPrint(": �����ʱ��%d��!\n",s_nRecMaxTime/1000);
		if( nAveRecTime < 20000 )
		{
			bRet = CSUDI_TRUE;
			CSTCPrint(": ƽ��ʱ��%d�룬���Գɹ�!\n",nAveRecTime/1000);
		}
		else
		{
			bRet = CSUDI_FALSE;
			CSTCPrint(": ƽ��ʱ��%d��,���ڷ�������20�����Χ��,����ʧ��\n",nAveRecTime/1000);
			CSTK_ASSERT_TRUE_FATAL(0,"���ڷ�������20�����Χ��,����ʧ��");
		}
	}
	CSTK_FATAL_POINT
	if (hEPG != NULL)
	{
		StopRecEPG(hEPG);
		DeleteAllEPG(hEPG);
		FreeEPG( hEPG);
	}

	if (hEPGMemPool != NULL)
	{
		CSUSPDestroyMemoryPool( hEPGMemPool);
	}
	if( s_hEPGCallbackEvent != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL((CSUDI_SUCCESS==CSUDIOSEventDestroy(s_hEPGCallbackEvent)),"Destroy Event Failure !!!");
	}
    if (pucEpgMem != CSUDI_NULL)
    {
        CSUDIOSFree(pucEpgMem);
        pucEpgMem = CSUDI_NULL;
    }
	
	return bRet;

}
#endif
//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION:����ͬһPID�¶�filter����,����ĳ��filter�Ƿ�Ӱ������filter�����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: OC_FILTER001��OC_FILTER002��OC_FILTER003��OC_FILTER004(��ο�CSTC_SECTION_IT_Allocate_0000)
//@EXPECTATION: ������������һ�ֽ�����ȫ������
//@EXECUTIONFLOW: 1������oc_test.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����OC_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart������filter�����������ɹ�
//@EXECUTIONFLOW: 4���������ݲ��Ҽ�¼section���У�������ֶ���û����������ȡ����filter
//@EXECUTIONFLOW: 5������Ƿ�������ȫ�Ƿ񶪰�
//@EXECUTIONFLOW: 6���ظ�����2��5������16��OC_FILTER001~OC_FILTER004ָ����filter������ÿһ�����ɹ�
CSUDI_BOOL CSTC_SECTION_IT_0019(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,OC_TEST_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == SECTION_iTest_IT_20()),
		"Test do not pass !!!"
		);

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

#define FS_READONLY  "r"

typedef struct{
	char * pTsFile;
	CSUDI_HANDLE hInjecter;
}SECTIONInjetParam_S;

static CSUDI_BOOL g_bInjectTestFinish = CSUDI_FALSE;

#define SECTION_BUFFER_SIZE (1*1024*1024)

static void iInjectTSFile(void * pvArg)
{  
	CSUDI_HANDLE  hTSFile= CSUDI_NULL;
	CSUDI_HANDLE  hINJECTER = CSUDI_NULL;
	long lFileLen = 0;
	long lCurrentPos = 0;
	long lLeftLen = 0;
	int nReadLength = 0;
    int nInjectedLength = 0; //��ע��Ĵ�С
    
	char * pcbuf = CSUDI_NULL;  //��������ַ
	unsigned int ulength = 0;  //����������
	
	SECTIONInjetParam_S * psInjectParam = (SECTIONInjetParam_S*)pvArg;
	
	if(psInjectParam == CSUDI_NULL)
	{
		CSTCPrint("psInjectParam == CSUDI_NULL\n");
		return;
	}
	
	hINJECTER = psInjectParam->hInjecter;

	hTSFile  = CSUDIFSOpen (psInjectParam->pTsFile,FS_READONLY);
	if(!hTSFile)
	{
		CSTCPrint("���ļ�ʧ�ܣ���ȷ����testdataĿ¼�´��� %s �ļ� !!!\n", psInjectParam->pTsFile);
		return;
	}
	if(CSUDIFSSeek(hTSFile,0,CSUDIFS_SEEK_END) != 0)
	{
		CSTCPrint("CSUDIFSSeek failuer !!!\n");
		return;
	}
	
	lFileLen = CSUDIFSTell (hTSFile);

    CSUDIFSSeek(hTSFile,0,CSUDIFS_SEEK_SET);
     
	while(1) //ѭ����ȡ�ļ�
	{
		///�����ǰʣ���ļ��ĳ���
		lCurrentPos  = CSUDIFSTell (hTSFile);	
		lLeftLen = lFileLen - lCurrentPos  ;
        
        if(lLeftLen == 0)
        {
            CSUDIFSSeek(hTSFile,0,CSUDIFS_SEEK_SET);
            lLeftLen = lFileLen;
        }

        ulength = 0;
		CSUDIINJECTERGetFreeBuffer (hINJECTER,(void*)&pcbuf,&ulength);

        if(ulength != 0)
        {
            nReadLength = SECTION_BUFFER_SIZE < lLeftLen ? SECTION_BUFFER_SIZE : lLeftLen;
            if(nReadLength < ulength) //ʣ���ļ�����С�ڿ�ע��ռ�ĳ���
            {
                nInjectedLength = nReadLength;
            }
            else  //ʣ���ļ����ȴ��ڵ��ڿ�ע��ռ�ĳ���
            {
                nInjectedLength = ulength;
            }

            CSUDIFSRead (hTSFile,(char*)pcbuf,nInjectedLength);

            CSUDIINJECTERWriteComplete(hINJECTER,ulength);
        }
   		else
        {
		    CSUDIOSThreadSleep(10);
   		}
        
		if(g_bInjectTestFinish)  
        {
            CSUDIFSClose(hTSFile);
            return;    //������������ֹ�߳�
		}
	}
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ����demux������ԴΪTSע��ʱ�����,����FilterΪ��ͬ���ʱ��PAT��������(filter��ȷֱ�Ϊ1��3��4��6)
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2������֧��TS����ע���demux
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001~PAT_FILTER004(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@REMARK:����INJECTERģ��
//@EXPECTATION: ÿһ���趼�ܹ��ɹ���ȡָ��������
//@EXECUTIONFLOW: 1������CSUDIDEMUXGetCount��ȡdemux����,������Ϊ0
//@EXECUTIONFLOW: 2������CSUDIDEMUXGetCapability��ȡ֧������ע���demuxID����������
//@EXECUTIONFLOW: 3������CSUDIINJECTEROpen����һ��TSע�����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIINJECTERStart����ע������ʼ����ע�룬�����ɹ�
//@EXECUTIONFLOW: 5������һ����ȡEPG_0606_121458_TS�ļ����߳̿�ʼע������,�����ɹ�
//@EXECUTIONFLOW: 6������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 7������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ��������
//@EXECUTIONFLOW: 8��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 9������CSUDIFILTERAllocate����PAT_FILTER002ָ����filter2����������ɹ�
//@EXECUTIONFLOW: 10������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 11��У��õ������ݣ������õ�PAT_FILTER002ָ��������
//@EXECUTIONFLOW: 12������CSUDIFILTERAllocate����PAT_FILTER003ָ����filter3����������ɹ�
//@EXECUTIONFLOW: 13������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 14��У��õ������ݣ������ò����κ�����
//@EXECUTIONFLOW: 15������CSUDIFILTERAllocate����PAT_FILTER004ָ����filter4����������ɹ�
//@EXECUTIONFLOW: 16������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 17��У��õ������ݣ������õ�PAT_FILTER004ָ��������
//@EXECUTIONFLOW: 18���ͷ�����filter
CSUDI_BOOL CSTC_SECTION_IT_0020(void)
{
	CSUDIINJECTERChnl_S sInjecterChnl;  //ע��ͨ������Դ����
	CSUDIINJECTEROpenParam_S sOpenParams;
	SECTIONInjetParam_S sInjecParam;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*һ��ֻ��32��filter*/
	CSUDI_HANDLE hINJECTER = CSUDI_NULL;
	CSUDI_HANDLE hInjectThread = CSUDI_NULL;
	int nFilterIndex = 0;
	int nDemuxCount = 0;
	int nDemuxIndex = 0;

	g_bInjectTestFinish = CSUDI_FALSE;  //initial 
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCount(&nDemuxCount),
			"Step 1 get demux count failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(0 != nDemuxCount),
			"Step 2 demux count should not be 0 Failure !!!"
		);
	
    for(nDemuxIndex = 0; nDemuxIndex < nDemuxCount; nDemuxIndex++)
	{
		CSTK_ASSERT_TRUE_FATAL(
				CSUDI_SUCCESS == CSUDIDEMUXGetCapability(nDemuxIndex, &sTmpCap),
				"Step 2 Get demux capability failure !!!"
			);
		
		if((sTmpCap.m_dwWorkTypeMask & EM_UDI_DEMUX_INJECT) == EM_UDI_DEMUX_INJECT && sTmpCap.m_nFilterNum > 0) //֧������ע��
		{
			break;
		}
    }
	
	if(nDemuxIndex == nDemuxCount)
	{
		CSTCPrint("�Ҳ�����inject������demux�豸����֧��inject���ܵ�demux�豸�ϲ�����filter�豸����ƽ̨��֧�ָò�������!!!");
		return CSUDI_TRUE;
	}
	
	CSTCPrint("Demux Id = %d support Ts inject\n",nDemuxIndex);
	
	sInjecterChnl.m_nDemux = nDemuxIndex; //��inject������demuxid
	sInjecterChnl.m_nAudioDecoder = -1;
	sInjecterChnl.m_nVideoDecoder = -1;

	sOpenParams.m_eContentType = EM_UDIINJECTER_CONTENT_DEFAULT;  
	sOpenParams.m_eInjecterType = EM_INJECTER_TS;  //ts����
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTEROpen(&sInjecterChnl,&sOpenParams, &hINJECTER) && hINJECTER != CSUDI_NULL),
			"Step 3 open an injecter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERStart(hINJECTER)),
			"Step 4 start inject Failure !!!"
		);
	
	sInjecParam.hInjecter = hINJECTER;
	sInjecParam.pTsFile = EPG_0606_121458_TS_FOR_INJECT;
	
	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIOSThreadCreate("TestInjecter",5*17,4*1024,iInjectTSFile,(void*)&sInjecParam, &hInjectThread),
			"step 5 create Injectthread failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(hInjectThread != CSUDI_NULL),
			"step 5 create Injectthread failure !!!"
		);
	
	CSUDIOSThreadSleep(100); //wait a moment
	
	do{		//filter1
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,nDemuxIndex,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&ahFilter[EM_SECTION_PAT_FILTER_001])),
					"Step 6 Allocate filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_001]),
					"Step 6 Allocate filter1 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_001],(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 7 Start Filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 8 Check Expected Data Failure !!!"
				);
			//filter2
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_002,nDemuxIndex,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_002,&ahFilter[EM_SECTION_PAT_FILTER_002])),
					"Step 9 Allocate filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_002]),
					"Step 9 Allocate filter2 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_002],(void*)EM_SECTION_PAT_FILTER_002)),
					"Step 10 Start Filter2 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_002,CSUDI_TRUE)),
					"Step 11 Check Expected Data Failure !!!"
				);
			//filter3
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_003,nDemuxIndex,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,&ahFilter[EM_SECTION_PAT_FILTER_003])),
					"Step 12 Allocate filter3 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_003]),
					"Step 12 Allocate filter3 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_003],(void*)EM_SECTION_PAT_FILTER_003)),
					"Step 13 Start Filter3 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,CSUDI_FALSE)),
					"Step 14 Expected No Data Failure !!!"
				);
			//filter4
			SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_004,nDemuxIndex,EM_PAT_PID);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_004,&ahFilter[EM_SECTION_PAT_FILTER_004])),
					"Step 15 Allocate filter4 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_NULL != ahFilter[EM_SECTION_PAT_FILTER_004]),
					"Step 15 Allocate filter4 failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStartFilter(ahFilter[EM_SECTION_PAT_FILTER_004],(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 16 Start Filter4 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 17 Check Expected Data Failure !!!"
				);
			//free filter1
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_001],(void*)EM_SECTION_PAT_FILTER_001)),
						"Step 18  Free Filter1 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_001] = CSUDI_NULL;
			}
			//free filter2
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_002],(void*)EM_SECTION_PAT_FILTER_002)),
						"Step 18  Free Filter2 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_002] = CSUDI_NULL;
			}	
			//free filter3
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_003],(void*)EM_SECTION_PAT_FILTER_003)),
						"Step 18  Free Filter3 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_003] = CSUDI_NULL;
			}
			//free filter4
			{
				CSTK_ASSERT_TRUE_FATAL(
						(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[EM_SECTION_PAT_FILTER_004],(void*)EM_SECTION_PAT_FILTER_004)),
						"Step 18  Free Filter4 Failure !!!"
					);
				ahFilter[EM_SECTION_PAT_FILTER_004] = CSUDI_NULL;
			}
			g_bInjectTestFinish = CSUDI_TRUE;
	}while(0);
	
	CSTK_FATAL_POINT
		
	g_bInjectTestFinish = CSUDI_TRUE;	
	
	for(nFilterIndex = EM_SECTION_PAT_FILTER_001; nFilterIndex <= EM_SECTION_PAT_FILTER_004 ; nFilterIndex ++)
	{	
		if(CSUDI_NULL != ahFilter[nFilterIndex])
		{
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(ahFilter[nFilterIndex],(void*)nFilterIndex)),
					"Free Filter Failure !!!"
				);
		}
	}
	if(hInjectThread != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hInjectThread)),
				"Destroy Thread Failure !!!"
			);
	}
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIINJECTERClose(hINJECTER)),
			"Step 4 start inject Failure !!!"
		);
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ���Զ���߳�ȥ����ͬPID��PES����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��SUBTITLE_001��AUDIO_001��TELETEXT_001��AUDIO_002(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT: 3������demux 0 �� tuner 0
//@EXPECTATION: ÿ���̰߳�Ԥ�ڻ�ȡ����
//@EXECUTIONFLOW: 1������Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2�������߳�1-4���в���
//@EXECUTIONFLOW: 3���ȴ��������������̲߳��Խ������߳�ʱ����������̲߳��Գɹ��������ɹ�������ʧ��
//@EXECUTIONFLOW: �߳�1ִ�����¹���:
//@EXECUTIONFLOW: 1.1������CSUDIFILTERAllocate����SUBTITLE_001ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 1.2������CSUDIFILTERStart����filter1�����������ɹ�
//@EXECUTIONFLOW: 1.3��У��õ������ݣ�����ΪSUBTITLE_001�������ֶ���
//@EXECUTIONFLOW: 1.4������CSUDIFILTERFree�ͷ�filter1���������سɹ�
//@EXECUTIONFLOW: 1.5���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: �߳�2ִ�����¹���:
//@EXECUTIONFLOW: 2.1������CSUDIFILTERAllocate����TELETEXT_001ָ����filter2����������ɹ�
//@EXECUTIONFLOW: 2.2������CSUDIFILTERStart����filter2�����������ɹ�
//@EXECUTIONFLOW: 2.3��У��õ������ݣ�����ΪTELETEXT_001�������ֶ���
//@EXECUTIONFLOW: 2.4������CSUDIFILTERFree�ͷ�filter2���������سɹ�
//@EXECUTIONFLOW: 2.5���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: �߳�3ִ�����¹���:
//@EXECUTIONFLOW: 3.1������CSUDIFILTERAllocate����AUDIO_001ָ����filter3����������ɹ�
//@EXECUTIONFLOW: 3.2������CSUDIFILTERStart����filter3�����������ɹ�
//@EXECUTIONFLOW: 3.3��У��õ������ݣ�����ΪAUDIO_001�������ֶ���
//@EXECUTIONFLOW: 3.4������CSUDIFILTERFree�ͷ�filter3���������سɹ�
//@EXECUTIONFLOW: 3.5���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: �߳�4ִ�����¹���:
//@EXECUTIONFLOW: 4.1������CSUDIFILTERAllocate����AUDIO_002ָ����filter4����������ɹ�
//@EXECUTIONFLOW: 4.2������CSUDIFILTERStart����filter4�����������ɹ�
//@EXECUTIONFLOW: 4.3��У��õ������ݣ�����ΪAUDIO_002�������ֶ���
//@EXECUTIONFLOW: 4.4������CSUDIFILTERFree�ͷ�filter4���������سɹ�
//@EXECUTIONFLOW: 4.5���ظ����Ϲ���5��
CSUDI_BOOL CSTC_SECTION_IT_0021(void)
{
	CSUDI_HANDLE hThread1 = CSUDI_NULL;
	CSUDI_HANDLE hThread2 = CSUDI_NULL;
	CSUDI_HANDLE hThread3 = CSUDI_NULL;
	CSUDI_HANDLE hThread4 = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,TELETEXT_SUBTITLE_25MBPS_071220_TS)),
			"Step 1 lock frequence failure !!!"
		);
	SECTION_iInitRequestCasePar();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_SUB_01_Thread",5*17, 32*1024, ThreadWithFilterNum, (void*)EM_SECTION_PES_SUBTITLE_001, &hThread1)),
			"Step 2 Create Filter_SUBTITLE_001_Thread failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_TELE_01_Thread",5*17, 32*1024, ThreadWithFilterNum, (void*)EM_SECTION_PES_TELETEXT_001, &hThread2)),
			"Step 2 Create Filter_TELETEXT_001_Thread failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_AUDIO_001_Thread",5*17, 32*1024, ThreadWithFilterNum, (void*)EM_SECTION_PES_AUDIO_001, &hThread3)),
			"Step 2 Create Filter_AUDIO_001_Thread failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_AUDIO_002_Thread",5*17, 32*1024, ThreadWithFilterNum, (void*)EM_SECTION_PES_AUDIO_002, &hThread4)),
			"Step 2 Create Filter_AUDIO_002_Thread failure !!!"
		);
	CSTCPrint("������������...��ȴ�!!!");
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == SECTION_iGetRequestCallBackStatus()),
		"Step 3 Failure"
		);

	CSTK_FATAL_POINT
	if(CSUDI_NULL != hThread1)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread1)),
			"Destroy Filter_SUBTITLE_001_Thread Failure"
			);
		hThread1 = CSUDI_NULL;
	}
	if(CSUDI_NULL != hThread2)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread2)),
			"Destroy Filter_TELETEXT_001_Thread Failure"
			);
		hThread2 = CSUDI_NULL;
	}
	if(CSUDI_NULL != hThread3)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread3)),
			"Destroy Filter_TELETEXT_002_Thread Failure"
			);
		hThread3 = CSUDI_NULL;
	}
	if(CSUDI_NULL != hThread4)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread4)),
			"Destroy Filter_SUBTITLE_002_Thread Failure"
			);
		hThread4 = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@CASEGROUP: 5��CSUDIFILTERModify
//@DESCRIPTION: ���Զ���߳�ȥ����ͬһPID�����ݣ�ÿ���߳���ͨ��Free���л�filter
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PAT_FILTER001~PAT_FILTER006(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT: 3������demux 0 �� tuner 0
//@EXPECTATION: ÿ���̰߳�Ԥ�ڻ�ȡ��δ��ȡ����
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2�������߳�1-3���в���
//@EXECUTIONFLOW: 3���ȴ��������������̲߳��Խ������߳�ʱ����������̲߳��Գɹ��������ɹ�������ʧ��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: �߳�1ִ�����¹���:
//@EXECUTIONFLOW: 1.1������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 1.2������CSUDIFILTERStart����filter1�����������ɹ�
//@EXECUTIONFLOW: 1.3��У��õ������ݣ�����ΪPAT_FILTER001
//@EXECUTIONFLOW: 1.4������CSUDIFILTERFree�ͷ�filter1
//@EXECUTIONFLOW: 1.5������CSUDIFILTERAllocate����PAT_FILTER002ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 1.6������CSUDIFILTERStart����filter1�����������ɹ�
//@EXECUTIONFLOW: 1.7��У��õ������ݣ�����ΪPAT_FILTER002ָ��������
//@EXECUTIONFLOW: 1.8������CSUDIFILTERFree�ͷ�filter1���������سɹ�
//@EXECUTIONFLOW: 1.9���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW:�߳�2ִ�����¹���:
//@EXECUTIONFLOW: 2.1������CSUDIFILTERAllocate����PAT_FILTER003ָ����filter2����������ɹ�
//@EXECUTIONFLOW: 2.2������CSUDIFILTERStart����filter2�����������ɹ�
//@EXECUTIONFLOW: 2.3��У��PAT_FILTER003�����ݣ��������κ�����
//@EXECUTIONFLOW: 2.4������CSUDIFILTERFree�ͷ�filter2
//@EXECUTIONFLOW: 2.5������CSUDIFILTERAllocate����PAT_FILTER004ָ����filter2����������ɹ�
//@EXECUTIONFLOW: 2.6������CSUDIFILTERStart����filter2�����������ɹ�
//@EXECUTIONFLOW: 2.7��У��õ������ݣ�����ΪPAT_FILTER004ָ��������
//@EXECUTIONFLOW: 2.8������CSUDIFILTERFree�ͷ�filter2���������سɹ�
//@EXECUTIONFLOW: 2.9���ظ����Ϲ���5��
//@EXECUTIONFLOW:
//@EXECUTIONFLOW:�߳�3ִ�����¹���:
//@EXECUTIONFLOW: 3.1������CSUDIFILTERAllocate����PAT_FILTER005ָ����filter3����������ɹ�
//@EXECUTIONFLOW: 3.2������CSUDIFILTERStart����filter3�����������ɹ�
//@EXECUTIONFLOW: 3.3��У��õ������ݣ�����ΪPAT_FILTER005ָ��������
//@EXECUTIONFLOW: 3.4������CSUDIFILTERFree�ͷ�filter3
//@EXECUTIONFLOW: 3.5������CSUDIFILTERAllocate����PAT_FILTER006ָ����filter3����������ɹ�
//@EXECUTIONFLOW: 3.6������CSUDIFILTERStart����filter3�����������ɹ�
//@EXECUTIONFLOW: 3.7��У�����ݣ�����ΪPAT_FILTER006ָ��������
//@EXECUTIONFLOW: 3.8������CSUDIFILTERFree�ͷ�filter3���������سɹ�
//@EXECUTIONFLOW: 3.9���ظ����Ϲ���5��
CSUDI_BOOL CSTC_SECTION_IT_0022(void)
{
	CSUDI_HANDLE hThread1 = CSUDI_NULL;
	CSUDI_HANDLE hThread2 = CSUDI_NULL;
	CSUDI_HANDLE hThread3 = CSUDI_NULL;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	SECTION_iInitPowerCasePar();
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Thread1",5*17, 32*1024, SECTION_IT_0022_Filter_Thread1, CSUDI_NULL, &hThread1)),
			"Step 2 Create Filter_Thread1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Thread2",5*17, 32*1024, SECTION_IT_0022_Filter_Thread2, CSUDI_NULL, &hThread2)),
			"Step 2 Create Filter_Thread2 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadCreate("Filter_Thread3",5*17, 32*1024, SECTION_IT_0022_Filter_Thread3, CSUDI_NULL, &hThread3)),
			"Step 2 Create Filter_Thread3 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == SECTION_iGetPowerCallBackStatus()),
		"Step 3 Failure"
		);
	
	CSTK_FATAL_POINT
	if(CSUDI_NULL != hThread1)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread1)),
			"Destroy Filter_Thread1 Failure"
			);
	}
	if(CSUDI_NULL != hThread2)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread2)),
			"Destroy Filter_Thread2 Failure"
			);
	}
	if(CSUDI_NULL != hThread3)
	{
		CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThread3)),
			"Destroy Filter_Thread3 Failure"
			);
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1������sectionģ��
//@CASEGROUP: 2��CSUDIDEMUXConnectTuner,CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: ����demux��tuner������ֻӰ��������Դ��Ӱ��section�Ĺ�������
//@INPUT: 1��PAT_FILTER001
//@EXPECTATION: 1. ��demux��tuner���ӵ�ʱ��section��������
//@EXPECTATION: 2. ��demux��tuner���ӶϿ���ʱ��section���ٽ��յ��κ�����
//@EXPECTATION: 3.��demux��tuner�ָ����ӵ�ʱ��,sectionӦ�ûָ�����
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����filter�����������ɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ������õ�����
//@EXECUTIONFLOW: 5������CSUDIDEMUXDisconnectTuner�Ͽ�demux��tuner0������,�����ɹ�
//@EXECUTIONFLOW: 6��У��õ������ݣ�����û���յ��κ���������
//@EXECUTIONFLOW: 7������CSUDIDEMUXConnectTuner�ָ�demux0��tuner0������,�������ݻָ�����
//@EXECUTIONFLOW: 8��У��õ������ݣ������õ�����(�˴��������ݽ���У��)
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�filter
CSUDI_BOOL CSTC_SECTION_IT_0023(void)
{
	int nTotalFilteCount = 0;
	int nFilterIndex = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE hFilter = NULL;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCapability(DEMUX_0, &sTmpCap),
			"Step 2 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nTotalFilteCount = sTmpCap.m_nFilterNum) > 0,
			"Step 2 failure ! Get Demux filter count < 0 !!"
		);

	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 2 Allocate filter Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 3 Check Expected Data Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(TUNER_0,DEMUX_0),
			" disconnect demux -> tuner Failure !!!");

	CSUDIOSThreadSleep(200);
	//clear statistic 
	SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);
	
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"expect no data received when disconnect demux -> tuner Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(TUNER_0,DEMUX_0),
		" connect demux -> tuner Failure !!!");

	CSUDIOSThreadSleep(200);
	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 3 Check Expected Data Failure !!!"
		);

	CSTK_FATAL_POINT
	if (hFilter)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter),
			"free test filter failure");
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERSetBufSize
//@CASEGROUP: 3��CSUDIFILTERStart
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ���Խ���OC���ݵĵ�ʱ��
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��EM_SECTION_OC_FILTER001~EM_SECTION_OC_FILTER004(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT: 3������demux 0 �� tuner 0
//@REMARK:��Ҫ��Ӧ��������
//@EXPECTATION: 1������oc_test.ts�������ڵ�Ƶ��
//@EXPECTATION: 2������CSUDIFILTERAllocate����EM_SECTION_OC_FILTER001ָ����filter����������ɹ�
//@EXPECTATION: 3������CSTKGetTickCount(&nStartTime)��ȡ��ǰϵͳʱ��
//@EXPECTATION: 4������CSUDIFILTERStart��ʼ����EM_SECTION_OC_FILTER001ָ�������ݣ����������ɹ�
//@EXPECTATION: 5��У��õ������ݣ����Ƿ��ж���
//@EXPECTATION: 6������CSTKGetTickCount(&nEndTime)��ȡ��ǰϵͳʱ�䣬ͨ��2��ϵͳʱ�����ó�ʵ���հ�ʱ��
//@EXPECTATION: 7���ظ�2~6���裬�ֱ�����EM_SECTION_OC_FILTER002���ݣ��������ʵ���հ�ʱ��
//@EXPECTATION: 8���ظ�2~6���裬�ֱ�����EM_SECTION_OC_FILTER003���ݣ��������ʵ���հ�ʱ��
//@EXPECTATION: 9���ظ�2~6���裬�ֱ�����EM_SECTION_OC_FILTER004���ݣ��������ʵ���հ�ʱ��
CSUDI_BOOL CSTC_SECTION_IT_0024(void)
{
	if(CSUDI_TRUE != CSTC_UDI2PortingLock(0,OC_TEST_TS))
	{
		CSTCPrint("Lock frequence failure !!!");
		return CSUDI_FALSE;
	}
	CSTK_ASSERT_TRUE_FATAL(SECTION_iTest_IT_21(EM_SECTION_OC_FILTER001), "Test do not pass !!!");
	CSTK_ASSERT_TRUE_FATAL(SECTION_iTest_IT_21(EM_SECTION_OC_FILTER002), "Test do not pass !!!");
	CSTK_ASSERT_TRUE_FATAL(SECTION_iTest_IT_21(EM_SECTION_OC_FILTER003), "Test do not pass !!!");
	CSTK_ASSERT_TRUE_FATAL(SECTION_iTest_IT_21(EM_SECTION_OC_FILTER004), "Test do not pass !!!");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERSetBufSize
//@CASEGROUP: 3��CSUDIFILTERStart
//@CASEGROUP: 4��CSUDIFILTERFree
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��EM_SECTION_OC_FILTER001~EM_SECTION_OC_FILTER004(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT: 3������demux 0 �� tuner 0
//@INPUT: 1��EIT_FILTER004(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿ�����趼���Եõ������Ľ��
//@REMARK: ��Ҫ��Ӧ��������
//@EXPECTATION: 1������oc_test.ts�������ڵ�Ƶ��
//@EXPECTATION: 2������CSUDIFILTERAllocate����EM_SECTION_OC_FILTER001ָ����filter����������ɹ�
//@EXPECTATION: 3������CSUDIFILTERStart��ʼ����EM_SECTION_OC_FILTER001ָ�������ݣ����������ɹ�
//@EXPECTATION: 4�����ô��д�ӡ�Ļص�����У��õ������ݣ����Ƿ��ж���
//@EXPECTATION: 5���ظ�2~4���裬�ֱ�����EM_SECTION_OC_FILTER002��EM_SECTION_OC_FILTER003��EM_SECTION_OC_FILTER004���ݿ��Ƿ��ж�������
CSUDI_BOOL CSTC_SECTION_IT_0025(void)
{
	if(CSUDI_TRUE != CSTC_UDI2PortingLock(0,OC_TEST_TS))
	{
		CSTCPrint("Lock frequence failure !!!");
		return CSUDI_FALSE;
	}
	CSTK_ASSERT_TRUE_FATAL(SECTION_iTest_IT_22(EM_SECTION_OC_FILTER004), "Test do not pass !!!");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}


#define ThreadPriority	(17*6)
#define ThreadStackSize	(1024*4)
#define SectionTestTime	(30)

enum {
	CAT_SECTION,
	PAT_SECTION,
	PMT_SECTION,
	TDT_SECTION,
	MAX_SECTION
}SECTION_TABLE;

struct SectionInfo{
	unsigned int unPid;
	unsigned int unHasRecieved;
	unsigned int unTotalSetion;
	BOOL	bError;
	char	cName[8];
};

#define SectionTestCount (4)
static CSUDI_HANDLE g_CATSem = CSUDI_NULL;
static unsigned int g_unFinish = 0;
static struct SectionInfo g_stSectionInfo[SectionTestCount];

static  void CSUDISECTIONCallback_IT( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	unsigned int unIndex = (unsigned int )pvUserData;
	
   	if(pSection == CSUDI_NULL)
	{
		return ;
	}

	g_stSectionInfo[unIndex].unHasRecieved ++;
	nLength  = ((0x0F & pSection->m_pucData[1]) << 8) + pSection->m_pucData[2] + 3;
	if (nLength != pSection->m_nDataLen)
	{
		CSTCPrint("%s table length compare failure\n",g_stSectionInfo[unIndex].cName);
		g_stSectionInfo[unIndex].bError = CSUDI_TRUE;
	}
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
			{
				int i=0;
				printf("%s:",g_stSectionInfo[unIndex].cName);
				for(i=0;i<8;i++)
				{
					printf("%02x ", pSection->m_pucData[i]);
				}
				printf("\n");
			}
			/*compare table id*/
			if (unIndex == 0)//PAT
 			{
				if (pSection->m_pucData[0] != 0x0)	
				{
					CSTCPrint("%s table tableid compare failure\n",g_stSectionInfo[unIndex].cName);
					g_stSectionInfo[unIndex].bError = CSUDI_TRUE;
				}
			}
			else if (unIndex == 1)
			{
				if (pSection->m_pucData[0] != 0x70 && pSection->m_pucData[0] != 0x73)	
				{
					CSTCPrint("%s table tableid compare failure\n",g_stSectionInfo[unIndex].cName);
					g_stSectionInfo[unIndex].bError = CSUDI_TRUE;
				}
			}
			else if (unIndex == 2)
			{
				if (pSection->m_pucData[0] != 0x2)	
				{
					CSTCPrint("%s table tableid compare failure\n",g_stSectionInfo[unIndex].cName);
					g_stSectionInfo[unIndex].bError = CSUDI_TRUE;
				}
			}
			g_stSectionInfo[unIndex].unTotalSetion++;
			break;
		default:
			break;
	}
}

static  void CSUDISECTIONCallback_CAT( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	char cExactSection[6] = {0x01,0xb0,0x2c,0xff,0xff,0xc5};
	
   	if(pSection == CSUDI_NULL)
	{
		return ;
	}
	
	nLength  = ((0x0F & pSection->m_pucData[1]) << 8) + pSection->m_pucData[2] + 3;
	if (nLength != pSection->m_nDataLen)
	{
		CSTCPrint("%s table length compare failure\n",g_stSectionInfo[3].cName);
		g_stSectionInfo[3].bError = CSUDI_TRUE;
	}
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
			{
				int i=0;
				printf("%s:",g_stSectionInfo[3].cName);
				for(i=0;i<8;i++)
				{
					printf("%02x ",pSection->m_pucData[i]);
				}
				printf("\n");
			}
			if (pvUserData)
			{
				CSTCPrint("%s table data compare failure 111\n", g_stSectionInfo[3].cName);
				g_stSectionInfo[3].bError = CSUDI_TRUE;
			}
			else
			{
				if (memcmp(cExactSection,pSection->m_pucData,sizeof(cExactSection)))
				{
					CSTCPrint("%s table data compare failure 222\n", g_stSectionInfo[3].cName);
					g_stSectionInfo[3].bError = CSUDI_TRUE;
				}
			}
			g_stSectionInfo[3].unTotalSetion++;
			CSUDIOSSemRelease(g_CATSem);
			break;
		default:
			break;
	}
}

static void SectionThread(void * pvParam)
{
	unsigned int unIndex = (unsigned int )pvParam;
	CSUDI_HANDLE hFilter = NULL;
	CSUDISECTIONRequestInfo_S stSectionReq;

	memset(&stSectionReq,0,sizeof(stSectionReq));
	stSectionReq.m_nDemuxID = DEMUX_0;
	stSectionReq.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSectionReq.m_nPID = g_stSectionInfo[unIndex].unPid;
		
	while(!(g_unFinish&(1<<unIndex)))
	{
		if (g_stSectionInfo[unIndex].unHasRecieved == 0 && hFilter == NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == 
				CSUDIFILTERAllocate(&stSectionReq,CSUDISECTIONCallback_IT,(void*)unIndex,&hFilter),
				"failure: create filter fail\n");
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilter),
				"failure: start filter fail\n");
		}
		else if (g_stSectionInfo[unIndex].unHasRecieved > 3)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter),
				"failure: free filter fail\n");
			hFilter = CSUDI_NULL;
			g_stSectionInfo[unIndex].unHasRecieved = 0;
		}

		CSUDIOSThreadSleep(100);
	}

	CSTK_FATAL_POINT
	if (hFilter)
	{
		CSUDIFILTERFree(hFilter);
		hFilter = CSUDI_NULL;
	}
	g_unFinish &= (~(1<<unIndex));
}

static void SectionThreadCAT(void * pvParam)
{
	unsigned int unIndex = (unsigned int )pvParam;
	CSUDI_HANDLE hFilter = NULL;
	CSUDISECTIONRequestInfo_S stSectionReq;
	unsigned int ngate = 0;

	memset(&stSectionReq,0,sizeof(stSectionReq));
	stSectionReq.m_nDemuxID = DEMUX_0;
	stSectionReq.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSectionReq.m_nPID = 0x1;
	stSectionReq.m_sMMFilter.match[3] = 0x2<<1;
	stSectionReq.m_sMMFilter.mask[3] =0x3E;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSectionReq,CSUDISECTIONCallback_CAT,(void*)ngate,&hFilter),
		"failure create CAT filter error\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilter),
		"failure start CAT filter error\n");
	while(!(g_unFinish&(1<<unIndex)))
	{
		if (CSUDI_SUCCESS == CSUDIOSSemWait(g_CATSem,30*1000))
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter),
				"failure free CAT filter error\n");
			if (stSectionReq.m_sMMFilter.negate[3])
			{
				stSectionReq.m_sMMFilter.negate[3] = 0x0;
				ngate = 0;
			}
			else
			{
				stSectionReq.m_sMMFilter.negate[3] = 0xFF;
				ngate = 1;
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSectionReq,CSUDISECTIONCallback_CAT,(void*)ngate,&hFilter),
				"failure create CAT filter error\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilter),
				"failure start CAT filter error\n");
		}
	}

	CSTK_FATAL_POINT
	if (hFilter)
	{
		CSUDIFILTERFree(hFilter);
		hFilter = CSUDI_NULL;
	}

	g_unFinish &= (~(1<<unIndex));
}

//@CASEGROUP: ����sectionģ��
//@DESCRIPTION: ���Զ��߳̽��ղ�ͬPID��section,����ÿ��section���ղ�ͣ�Ľ����ͷ��ٽ��յĲ���,
//@DESCRIPTION:����CAT����汾�ű仯,�汾�ű仯��ʱ������filter��ngate�ֶ�,�Ӷ����а汾�仯���
//@INPUT: 1��EPG_0606_121458.ts����
//@INPUT: 2��pid=0x0��PAT��, pid=0x14 ��TDT��,pid=0x101��PMT��,pid=0x1��CAT��
//@EXPECTATION:1.section�������ͷŲ��໥Ӱ��,ÿ�������������ܽ��յ���ȷ������,���������ױ�����
//@EXPECTATION:2.ÿ�������������յ��İ��Ƿ���ȷ���жϱ�׼�ǻص��ĳ��Ⱥ�section�а����ĳ���һ��
//@EXPECTATION:@ �Լ���һ���ֽڵ�tableid������ȷ
//@EXPECTATION: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXPECTATION: 2������CSUDIDEMUXGetCapability��ȡdemux0������,������filter�����������0
//@EXPECTATION: 3������CSUDIOSSemCreate����CAT������̵߳�֪ͨ�ź���,���������ɹ�
//@EXPECTATION: 4������CSUDIOSThreadCreate���δ���PAT TDT PMT ��Ľ����߳�,�����̵߳Ĵ�����������:
//@EXPECTATION: 4.1   ����CSUDIFILTERAllocate����PID��Ӧ�Ĺ�����,��������ȫΪ0,��������CSUDI_SUCCESS
//@EXPECTATION: 4.2	����CSUDIFILTERStart���������filter,��������CSUDI_SUCCESS
//@EXPECTATION: 4.3   �жϸ�filter���յ������ݰ��Ƿ��Ѿ�����3��,����������CSUDIFILTERFree�ͷŸ�filter,��������CSUDI_SUCCESS
//@EXPECTATION: 4.4   sleep 100ms,�ж��Ƿ���Խ���,û�еĻ��ظ�����4.1��4.4
//@EXPECTATION: 5������CSUDIOSThreadCreate����CAT��Ľ����߳�ģ��汾�ż��,��ϸ��������:
//@EXPECTATION: 5.1   ����CSUDIFILTERAllocate����CAT���filter,��������ȫΪmatch[3] = 0x2<<1,mask[3] =0x3E,��������CSUDI_SUCCESS
//@EXPECTATION: 5.2   ����CSUDIFILTERStart���������filter,��������CSUDI_SUCCESS
//@EXPECTATION: 5.3   ����CSUDIOSSemWait�ȴ�CAT����յ����ź�,���ź���section�Ļص����յ������ͷ�
//@EXPECTATION: 5.4   ���յ��źź�,��תnegate[3],���ԭ����0x0,��ת��0xFF,�ͷ������filter,�ظ�����5.1��5.4
//@EXPECTATION:6���жϲ����Ƿ�ͨ��:1),�����߳������section���յ��İ�����0. 2)���յõ���sectionû�г��ִ���
//@EXPECTATION:7������CSUDIOSThreadDestroy���Ĳ����߳�
//@EXPECTATION:8������CSUDIOSSemDestroy���Ĳ����ź���
CSUDI_BOOL CSTC_SECTION_IT_0026(void)
{
	int nTotalFilteCount = 0;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE hThread[SectionTestCount];
	int i;
	CSUDI_BOOL bRet = TRUE;

	g_unFinish = 0;
	memset(&g_stSectionInfo,0,sizeof(g_stSectionInfo));
	g_stSectionInfo[0].unPid = 0x0; //PAT
	strcpy(g_stSectionInfo[0].cName,"PAT");
	g_stSectionInfo[1].unPid = 0x14; // TDT
	strcpy(g_stSectionInfo[1].cName,"TDT");
	g_stSectionInfo[2].unPid = 0x1c7; //PMT
	strcpy(g_stSectionInfo[2].cName,"PMT");
	g_stSectionInfo[3].unPid = 0x1; //CAT
	strcpy(g_stSectionInfo[3].cName,"CAT");

	memset(hThread, 0x00, sizeof(hThread));
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCapability(DEMUX_0, &sTmpCap),
			"Step 2 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nTotalFilteCount = sTmpCap.m_nFilterNum) > 0,
			"Step 2 failure ! Get Demux filter count < 0 !!"
		);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSSemCreate("cat",0,1,&g_CATSem),
		"failure:create CAT's semphore error\n");	

	for(i=0;i<SectionTestCount;i++)
	{
		if (g_stSectionInfo[i].unPid == 0x1)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == 
				CSUDIOSThreadCreate("CAT",ThreadPriority,ThreadStackSize,SectionThreadCAT,(void*)i,&hThread[i]),
				"failure :create thread error\n");
		}
		else
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == 
				CSUDIOSThreadCreate("section",ThreadPriority,ThreadStackSize,SectionThread,(void*)i,&hThread[i]),
				"failure :create thread error\n");
		}
	}

	CSTCPrint("pls wait for 30s testing......\n");
	for (i=0;i<60;i++)
	{
		int j=0;
		for(;j<SectionTestCount;j++)
		{
			if (g_stSectionInfo[j].bError)
			{
				CSTCPrint("%s table receive error\n",g_stSectionInfo[j].cName);
				bRet = CSUDI_FALSE;
				goto out;
			}
		}
		CSUDIOSThreadSleep(500);
	}
	
out:
	CSTCPrint("dump section packet statistic\n");
	for(i=0;i<SectionTestCount;i++)
	{
		CSTCPrint("%s table receive %d packet\n",g_stSectionInfo[i].cName,g_stSectionInfo[i].unTotalSetion);
		if (g_stSectionInfo[i].unTotalSetion == 0)
		{
			bRet = CSUDI_FALSE;
		}
	}

	g_unFinish = (1<<SectionTestCount)-1;
	CSTCPrint("wait for thread exit\n");
	while(g_unFinish)
	{
		CSTCPrint("finish:%x\n",g_unFinish);
		CSUDIOSThreadSleep(500);
	}

CSTK_FATAL_POINT
	CSTCPrint("clean up test resource\n");
	for(i=0;i<SectionTestCount;i++)
	{
		if (hThread[i])
		{
			CSUDIOSThreadDestroy(hThread[i]);
		}
	}

	if (g_CATSem)
	{
		CSUDIOSSemDestroy(g_CATSem);
		g_CATSem = CSUDI_NULL;
	}
	
	return bRet;
}

static BOOL 	g_bErrorEit = TRUE,g_bErrorSDT = TRUE;
static  void CSUDISECTIONCallback_EIT_SDT( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	unsigned int unIndex = (unsigned int)pvUserData;
	
   	if(pSection == CSUDI_NULL)
	{
		return ;
	}
	
	nLength  = ((0x0F & pSection->m_pucData[1]) << 8) + pSection->m_pucData[2] + 3;
	if (nLength != pSection->m_nDataLen)
	{
		if (unIndex == 0)
		{
			CSTCPrint("Eit receive error\n");
			g_bErrorEit = FALSE;
		}
		else 
		{
			CSTCPrint("EMM receive error\n");
			g_bErrorSDT = FALSE;
		}
	}
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
		{
			unsigned char ucTableid = pSection->m_pucData[0];
			if (unIndex == 0)
			{
				if (ucTableid < 0x60 || ucTableid > 0x6f)
				{
					CSTCPrint("recieve error section for eit, tableid:%x\n",ucTableid);
					g_bErrorSDT = FALSE;
				}
			}
			else
			{
				if (ucTableid != 0x42)
				{
					CSTCPrint("recieve error section for sdt, tableid:%x\n",ucTableid);
					g_bErrorSDT = FALSE;
				}
			}
			/*ģ��Ӧ����һ�����ݿ���*/
			memcpy(g_pcSectionUserBuff,pSection->m_pucData,pSection->m_nDataLen);
		}
			break;
		default:
			break;
	}
}

static void CSUDIOSThreadEntry_test_Eit(void * pvParam)
{
	if (pvParam == CSUDI_NULL)
	{
		return ;
	}

	//�߳���ת40�ξ���������
	while((*(int*)pvParam) <= 40)
	{
		CSTCPrint("HaHa , I am still alive\n");
		CSUDIOSThreadSleep(500);
		(*(int*)pvParam)++; 
	}
}

//@CASEGROUP: ����sectionģ��
//@DESCRIPTION: �����ڽ���section�������ϴ�ʱ,sectionģ���Ƿ��һֱռ��CPU�����ͷ�
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: 1��EPG_0606_121458.ts����
//@INPUT: 2����������EIT�����������SDT��
//@INPUT: 3��EIT��SDT section�ص��л���һ��section�Ŀ���
//@EXPECTATION: �ڽ��нϴ���������section����ʱ,�����ȼ�������Ҳ�ܻ��CPU���еĻ���
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIOSThreadCreate����һ��1*17�ĵ����ȼ��߳�,���̵߳�ִ�в������������:
//@EXECUTIONFLOW: 2.1   sleep 500ms Ȼ�����ֵ��1,������ͨ���ü���ֵ�жϵ����ȼ��߳��Ƿ�õ��㹻��CPU
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate����pid=0x12��EIT section,�������سɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart������filter,�������سɹ�
//@EXECUTIONFLOW: 5������CSUDIFILTERAllocate����pid=0x11��SDT section,�������سɹ�
//@EXECUTIONFLOW: 6������CSUDIFILTERStart������filter,�������سɹ�
//@EXECUTIONFLOW: 7���ȴ�30s��Ϊ���Թ���
//@EXECUTIONFLOW: 8�������Խ��,���EIT����SDT���н��յ���������,���ߴ����ĵ����ȼ��߳����д�������40��,����Խ��ʧ��,����ɹ�
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�EIT ����filter
//@EXECUTIONFLOW: 10������CSUDIFILTERFree�ͷ�SDT ����filter
CSUDI_BOOL CSTC_SECTION_IT_0027(void)
{
	CSUDISECTIONRequestInfo_S stSectionReq;
	CSUDI_HANDLE hFilterEit = NULL;
	CSUDI_HANDLE hFilterEMM = NULL;
	CSUDI_HANDLE hThread = NULL;
	int i=0;
	int *pCount = NULL;
	CSUDI_BOOL bRet = FALSE;

	g_bErrorEit = TRUE;
	g_bErrorSDT = TRUE;

	g_pcSectionUserBuff = NULL;
	pCount = CSUDIOSMalloc(sizeof(int));
	*pCount = 0;
	g_pcSectionUserBuff = CSUDIOSMalloc(CSMaxSectionLength);
	CSTK_ASSERT_TRUE_FATAL(g_pcSectionUserBuff != NULL,"fail to allocate memory\n");
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);

	/*create one lowwer priority thread to check if the section module exhaust CPU*/
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("eit_test",1*17,1024*4,CSUDIOSThreadEntry_test_Eit,pCount,&hThread),
		"create test thread failure\n");

	memset(&stSectionReq,0,sizeof(stSectionReq));
	stSectionReq.m_nPID = 0x12; //EIT
	stSectionReq.m_nDemuxID = DEMUX_0;
	stSectionReq.m_eReqType = EM_UDI_REQ_SECTION_DATA;
    stSectionReq.m_sMMFilter.match[0] = 0x60;
    stSectionReq.m_sMMFilter.mask[0] = 0xf0;
    stSectionReq.m_sMMFilter.negate[0] = 0; 

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSectionReq,CSUDISECTIONCallback_EIT_SDT,0,&hFilterEit),
		"failure create CAT filter error\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterEit),
		"failure start CAT filter error\n");
	stSectionReq.m_nPID = 0x11; //SDT
	stSectionReq.m_eReqType = EM_UDI_REQ_SECTION_DATA;
    stSectionReq.m_sMMFilter.match[0] = 0x42;
    stSectionReq.m_sMMFilter.mask[0] = 0xff;
    stSectionReq.m_sMMFilter.negate[0] = 0; 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSectionReq,CSUDISECTIONCallback_EIT_SDT,(void*)1,&hFilterEMM),
		"failure create CAT filter error\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilterEMM),
		"failure start CAT filter error\n");

	for(i=0;i<60;i++)
	{
		CSUDIOSThreadSleep(500);
	}

	CSTK_FATAL_POINT
	CSUDIOSThreadDestroy(hThread);
	if (hFilterEit)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterEit),
			"free Eit filter failure");
	}
	if (hFilterEMM)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilterEMM),
			"free EMM filter failure");
	}

	if (pCount)
	{
		bRet = (g_bErrorEit&&g_bErrorSDT&&(*pCount > 40));
		CSUDIOSFree(pCount);
	}

	if (g_pcSectionUserBuff)
	{
		CSUDIOSFree(g_pcSectionUserBuff);
		g_pcSectionUserBuff = NULL;
	}

	return bRet;
}

struct TestInfo_It_0028
{
	unsigned int unPacketCnt;
	unsigned int unLastSectionNum;
	BOOL nStatus;
};
static struct TestInfo_It_0028 *g_pstTestInfo0028 = NULL;
static BOOL g_bSectionLenError0028 = FALSE;
#define MAX_FILTER_SHIFT	(8)
static  void CSUDISECTIONCallback_0028( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	int nLength = 0;
	unsigned int unTotalSec = (unsigned int )pvUserData;
	unsigned char ucExtraId = 0;
	unsigned char ucCurrentSecNum,ucLastSecNum;
	
   	if(pSection == CSUDI_NULL)
	{
		return ;
	}

	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
		{
			nLength = ((pSection->m_pucData[1]&0xF)<<8)+pSection->m_pucData[2] + 3;

			/*���ݿ����Ѿ��ҵ���,����ֱ��ȥȡextra table id*/
			if (nLength != pSection->m_nDataLen)
			{
				g_bSectionLenError0028 = TRUE;
				CSTCPrint("section length don't match length from callback\n");
				return ;
			}
	
			/*ֻȡ��չtable id�����λ,��Ϊ��λ����������ж�Ϊ0*/
			ucExtraId = pSection->m_pucData[4];
			if (ucExtraId > unTotalSec)
			{
				CSTCPrint("error receive big extra table id:%x\n",ucExtraId);
				return ;
			}
			ucCurrentSecNum = pSection->m_pucData[6];
			ucLastSecNum =  pSection->m_pucData[7];
			if (((g_pstTestInfo0028[ucExtraId].unLastSectionNum+1) != ucCurrentSecNum)
				&& (g_pstTestInfo0028[ucExtraId].unLastSectionNum != -1)
				&& (g_pstTestInfo0028[ucExtraId].unLastSectionNum != ucLastSecNum)
				&& (ucCurrentSecNum != 0))
			{
				g_pstTestInfo0028[ucExtraId].nStatus = TRUE;
				CSTCPrint("extra id:%x drop packet ,cur:%x last:%x\n",
					ucExtraId,ucCurrentSecNum,g_pstTestInfo0028[ucExtraId].unLastSectionNum);
			}

			g_pstTestInfo0028[ucExtraId].unLastSectionNum = ucCurrentSecNum;
			g_pstTestInfo0028[ucExtraId].unPacketCnt++;
			/*ģ��Ӧ�ûص�,��һ�ο���*/
			memcpy(g_pcSectionUserBuff,pSection->m_pucData,pSection->m_nDataLen);
			break;
		}
		default:
			break;
	}
}

/*descripition:GetFilter�����γ�ĳ���ֽڵĹ�������.��Ҫ���˵��ֽ�λ��
��nSecIndexָ��.��nCnt = 5ʱ��ʾ�γ�������չtableid 0~4�ı�Ĺ�������,
nCnt = 10ʱ��ʾ�γ�������չtableid 0~9�ı�Ĺ�������
nCnt:��Ҫ���˶��ٸ���
nSecIndex:���section�еڼ����ֽ�
pnGetFilterCnt:���γ��˶��ٸ�filter
*/
static CSUDISECTIONRequestInfo_S* GetFilter(int nCnt,int nSecIndex,int *pnGetFilterCnt)
{
	int i=MAX_FILTER_SHIFT,j=0;
	int nFilterCnt = 0;
	CSUDISECTIONRequestInfo_S *pTmp,*pOutFilter;
	int ntrace[MAX_FILTER_SHIFT];
	unsigned char match=0x0,mask=0x0;

	pOutFilter = CSUDIOSMalloc(MAX_FILTER_SHIFT*sizeof(CSUDIFILTERMatchMask_S));
	memset(pOutFilter,0,MAX_FILTER_SHIFT*sizeof(CSUDIFILTERMatchMask_S));
	pTmp = pOutFilter;
	memset(ntrace,0,sizeof(ntrace));
		
	for(i=MAX_FILTER_SHIFT;i>=0;i--)
	{
		if ((nCnt-(1<<i)) >= 0)
		{
			match = 0x0;
			mask = 0x0;
			for (j=0;j<nFilterCnt;j++)
			{
				 match=match|(1<<ntrace[j]);
			}			

			mask = ~((1<<i)-1);

			pTmp->m_sMMFilter.match[nSecIndex] = match;
			pTmp->m_sMMFilter.mask[nSecIndex] = mask;
			pTmp++;
			ntrace[nFilterCnt++] = i;
			nCnt = nCnt - (1<<i);

			CSTCPrint("got one filter match:%x,mask:%x\n",match,mask);
		}
	}

	*pnGetFilterCnt = nFilterCnt;

	return pOutFilter;
}

static CSUDI_BOOL CSUDISECTION_IT_ads(unsigned int unPid,int nTableCnt)	
{
	CSUDISECTIONRequestInfo_S *pstSectionReq = NULL, *pstSectionReqTmp = NULL;
	CSUDI_HANDLE hFilter[MAX_FILTER_SHIFT] = {NULL};
	int i=0;
	CSUDI_BOOL bRet = TRUE;
	int nFilterCnt = 0;

	CSTCPrint("��������һ�ֲ��ſ�ʼʱ��ʼ����,����������ͷʱ������ж϶����Ӷ�����ʧ��\n");
	CSTCPrint("���������ʼ����\n");
    CSTCPrint("Attention Please!! Make sure you run this testcase right after you start playing the test stream,\t");
    CSTCPrint("if not,you may get \"section lost\" scenario happen!!!\n");
	CSTKWaitAnyKey();
	
	g_pstTestInfo0028 = NULL;
	g_pcSectionUserBuff = NULL;
	g_bSectionLenError0028 = FALSE;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,COSHIP_ADS_SECTION_TS)),
			"Step 1 lock frequence failure !!!"
		);

	g_pstTestInfo0028 = CSUDIOSMalloc(nTableCnt*sizeof(struct TestInfo_It_0028));
	CSTK_ASSERT_TRUE_FATAL(g_pstTestInfo0028 != NULL,"malloc memory fail\n");
	memset(g_pstTestInfo0028,0,nTableCnt*sizeof(struct TestInfo_It_0028));

	for(i=0;i<nTableCnt;i++)
	{
		g_pstTestInfo0028[i].unLastSectionNum =  -1;
	}
	
	g_pcSectionUserBuff = CSUDIOSMalloc(CSMaxSectionLength);
	CSTK_ASSERT_TRUE_FATAL(g_pcSectionUserBuff != NULL,"malloc memory fail\n");
		
	pstSectionReq = GetFilter(nTableCnt,2,&nFilterCnt);
	
	for(i=0;i<nFilterCnt;i++)
	{
		pstSectionReqTmp = &pstSectionReq[i];
		pstSectionReqTmp->m_nPID = unPid;
		pstSectionReqTmp->m_nDemuxID = 0;
		pstSectionReqTmp->m_eReqType = EM_UDI_REQ_SECTION_DATA;
		
		pstSectionReqTmp->m_sMMFilter.match[0]=0x3c;
		pstSectionReqTmp->m_sMMFilter.mask[0]=0xff;

 		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(pstSectionReqTmp,CSUDISECTIONCallback_0028,(void*)nTableCnt,&hFilter[i]),
			"failure create  filter error\n");
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilter[i]),
			"failure start  filter error\n");
	}

	CSUDIOSThreadSleep(15*1000);

	CSTK_FATAL_POINT
	for (i=0;i<nFilterCnt;i++)
	{
		if (hFilter[i])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter[i]),
				"free Eit filter failure");
		}
	}
	/*û����չtable idΪ0�ı�*/
	if(g_pstTestInfo0028)
	{
		for (i=1;i<nTableCnt;i++)
		{
			CSTCPrint("extra table id:%x receive %x packet status:%s\n",
				i,g_pstTestInfo0028[i].unPacketCnt,g_pstTestInfo0028[i].nStatus?"fail":"pass");
			if (g_pstTestInfo0028[i].nStatus ||(g_pstTestInfo0028[i].unPacketCnt == 0))
			{
				bRet = FALSE;
			}
		}
	}

	if (pstSectionReq)
	{
		CSUDIOSFree(pstSectionReq);
	}

	if(g_pstTestInfo0028)
	{
		CSUDIOSFree(g_pstTestInfo0028);
		g_pstTestInfo0028 = NULL;
	}

	if (g_pcSectionUserBuff)
	{
		CSUDIOSFree(g_pcSectionUserBuff);
		g_pcSectionUserBuff = NULL;
	}
	
	return (bRet&&(!g_bSectionLenError0028));
}

//@CASEGROUP: ����sectionģ��
//@DESCRIPTION: ����demux��section���ݽ�������,���ն̰�(128B)��������section
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3������ʱ���������һ�ֲ��ſ�ʼ,������������ͷʱ����ж���
//@INPUT: 1��coship_ads_section.ts����,���������ص�����:
//@INPUT:(1) pid=6000 ,����Ϊ8Mbps,��������tableid �ֱ�Ϊ0x2b��0x3c,����0x3c�ı�ռ98%,tableid=0x3c
//@INPUT:��section�ַ�Ϊ38(0x1~0x26)����չtableid,����0��ʼ������section��3��4���ֽ�,tableid=0x3c
//@INPUT:��section����last section�⣬����İ�����Ϊ128B,������Ҫ����С����������section���ղ���
//@INPUT:(2) pid=6001,����Ϊ8Mbps,��������tableid �ֱ�Ϊ0x2b��0x3c,����0x3c�ı�ռ98%,tableid=0x3c
//@INPUT:��section�ַ�Ϊ2(0x1~0x2)����չtableid,����0��ʼ������section��3��4���ֽ�,tableid=0x3c
//@INPUT:��section����last section�⣬����İ�����Ϊ4KB,������Ҫ���ڴ����������section���ղ���
//@INPUT:(3) pid=6003 ��pid=6001������������һ��,����pid=6003��������CRC����ÿ������CRCֵ���Ǵ����
//@INPUT:��������CRC���ܲ���
//@EXPECTATION: ���ն̰���������section��ʱ��demuxӦ������һ��������Ҫ��
//@EXECUTIONFLOW: 1������coship_ads_section.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIOSMalloc �������ڱ���section���ݵ���ʱbuffer,��buffer����section�Ļص���ģ��һ�����ݿ���
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate����pid=6000��section��ֻ����tableid=0x3c��section
//@EXECUTIONFLOW: 4������CSUDIFILTERStart������filter,�������سɹ�
//@EXECUTIONFLOW: 5��sleep 15s 
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ����в���filter
//@EXECUTIONFLOW: 7���жϲ��Խ��,����ͨ�����ж�����Ϊ:
//@EXECUTIONFLOW: 1)ÿ����չtableid��section�������ݰ�������0
//@EXECUTIONFLOW: 2)ÿ��section���صĳ��Ⱥ�seciton�����ֶεĳ���ƥ��
//@EXECUTIONFLOW: 3)û�ж�������,�����յ��İ���section number�ֶ���������
CSUDI_BOOL CSTC_SECTION_IT_0028(void)
{
	return CSUDISECTION_IT_ads(6000,38);
}

//@CASEGROUP: ����sectionģ��
//@DESCRIPTION: ����demux��section���ݽ�������,���ճ���(4KB)��������section
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3������ʱ���������һ�ֲ��ſ�ʼ,������������ͷʱ����ж���
//@INPUT: 1��coship_ads_section.ts����,����������Ϣ��ο�CSTC_SECTION_IT_0028
//@INPUT: 2��pid=6001,tableid=0x3c
//@EXPECTATION: ���ճ�����������section��ʱ��demuxӦ������һ��������Ҫ��
//@EXECUTIONFLOW:�����CSTC_SECTION_IT_0028�Ĳ��Բ���
CSUDI_BOOL CSTC_SECTION_IT_0029(void)
{
	return CSUDISECTION_IT_ads(6001,3);
}

struct CSPESTestInfo0030{
	BOOL bPass;
	unsigned int unPktNum;
	unsigned char *pTmpBuf;
	unsigned int unPktCount;
};
static  void CSUDISECTIONCallback_IT_0030( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	unsigned char *pes = NULL;
	unsigned int unPacketNum;
	struct CSPESTestInfo0030 *pstTestInfo = pvUserData;
	
   	if(pSection == CSUDI_NULL || pstTestInfo == NULL)
	{
		return ;
	}

	pes = pSection->m_pucData;
	unPacketNum = (((pes)[11] << 24) | ((pes)[12] << 16) | ((pes)[13] << 8) | ((pes)[14]));
	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
			break;
		case EM_UDI_REQ_PES_DATA:
			if (pes[0]!=0x0&&pes[1]!=0x0&&pes[2]!=0x1)
			{
				CSTCPrint("errror:get one pkt without 001 starting \n");
				pstTestInfo->bPass = FALSE;
				break;
			}
			if (pstTestInfo->unPktNum != -1 && (unPacketNum != (pstTestInfo->unPktNum+1)) && (unPacketNum!= 0))
			{
				pstTestInfo->bPass = FALSE;
				CSTCPrint("drop pkt now:%d last:%d\n",unPacketNum,pstTestInfo->unPktNum);
				break;
			}
			else
			{
				CSTCPrint("get pes:%d pkt num:%d\n",pSection->m_nDataLen,unPacketNum);
			}

			memcpy(pstTestInfo->pTmpBuf,pSection->m_pucData,pSection->m_nDataLen);

			//�����ݵĵ�dataLen-1�ֽڲ�����Ҫ�����쳣
 			pstTestInfo->pTmpBuf[pSection->m_nDataLen-1]++;
			
			pstTestInfo->unPktNum = unPacketNum;
			pstTestInfo->unPktCount++;
			break;
		default:
			break;
	}
}

//@CASEGROUP: ����sectionģ��
//@DESCRIPTION: ����ƽ̨PES��������,ʹ������Ϊcoship ��������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡPES����
//@PRECONDITION: 3������ʱ���������һ�ֲ��ſ�ʼ,������������ͷʱ����ж���
//@INPUT: PES_performance_test.ts,��������������������PES�ֱ�Ϊ:
//@INPUT:1. ÿ����Ϊ5K����0x1df0,����Ϊ10Mbps(��������������5k)
//@INPUT:2. ÿ����Ϊ32K����0x1def,����Ϊ15Mbps(��������������32k)
//@INPUT:3. ÿ��PES���ı�Ż�ȡ��ʽΪ(((pes)[11] << 24) | ((pes)[12] << 16) | ((pes)[13] << 8) | ((pes)[14]))
//@EXPECTATION: �������������������Ľ���PES����û�ж���
//@EXECUTIONFLOW: 1������PES_performance_test.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate������0x1df0ָ����filter����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart��ʼ����ָ����PES����
//@EXECUTIONFLOW: 5��ÿ��1s����Ƿ��в���ʧ�����,һ�����30s,�Ƿ�����Ƿ�ʧ����PES�ص��н���,��������:
//@EXECUTIONFLOW: 5.1   �������Ƿ���PES��־001��ͷ,���������ʧ��
//@EXECUTIONFLOW: 5.2   ���������Ƿ�����,Ӧ��Ϊ0,1,2,3,4.......
//@EXECUTIONFLOW: 5.3   ���յ���PES������һ����ʱbufferģ��Ӧ���ڻص��еĲ���
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 7��������PID���ĳ�15Mbps�Ĳ�����0x1def,�ظ�����2~6
CSUDI_BOOL CSTC_SECTION_IT_0030(void)
{
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	CSUDI_BOOL bRet = FALSE;
	CSUDISECTIONRequestInfo_S stSectionReq;
	struct CSPESTestInfo0030 stTestInfo;
	int i=0;
    
    memset(&stSectionReq, 0, sizeof(stSectionReq));
    memset(&stTestInfo, 0, sizeof(stTestInfo));

	#define MAX_TMP_BUF	(34*1024)
	#define MAX_TEST_TIME (30)//second

	CSTCPrint("�����������Ĳ�������Ϊ24M!\n");
	CSTCPrint("��������һ�ֲ��ſ�ʼʱ��ʼ����,����������ͷʱ������ж϶����Ӷ�����ʧ��\n");
	CSTCPrint("���������ʼ����\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,PES_PERFORMANCE_TEST_TS)),
			"Step 1 lock frequence failure !!!"
		);

	stSectionReq.m_nDemuxID = DEMUX_0;
	stSectionReq.m_eReqType = EM_UDI_REQ_PES_DATA;
	stSectionReq.m_nPID = 0x1df0;

	stTestInfo.bPass = TRUE;
	stTestInfo.unPktNum = -1;
	stTestInfo.pTmpBuf = CSUDIOSMalloc(MAX_TMP_BUF);
	CSTK_ASSERT_TRUE_FATAL(stTestInfo.pTmpBuf != NULL,"failure malloc memory fail\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSectionReq,CSUDISECTIONCallback_IT_0030,&stTestInfo,&hFilter),
		"failure create  filter error\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilter),
		"failure start  filter error\n");

	while(i<MAX_TEST_TIME)
	{
		if (!stTestInfo.bPass)
		{
			break;
		}
		CSUDIOSThreadSleep(1000);
		i++;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStop(hFilter),
		"failure stop  filter error\n");

	CSTK_ASSERT_TRUE_FATAL(((i==MAX_TEST_TIME) && (stTestInfo.unPktCount>0)),"pes 10Mbps, 5K per packet length ,test fail\n");
	if (hFilter)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter),
			"free Eit filter failure");
		hFilter = NULL;
	}

	stSectionReq.m_nPID = 0x1df0-1;

	stTestInfo.bPass = TRUE;
	stTestInfo.unPktNum = -1;
	stTestInfo.unPktCount = 0;
	i = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSectionReq,CSUDISECTIONCallback_IT_0030,&stTestInfo,&hFilter),
		"failure create  filter error\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilter),
		"failure start  filter error\n");

	while(i<MAX_TEST_TIME)
	{
		if (!stTestInfo.bPass)
		{
			break;
		}
		CSUDIOSThreadSleep(1000);
		i++;
	}

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStop(hFilter),
		"failure stop  filter error\n");

	CSTK_ASSERT_TRUE_FATAL((i == MAX_TEST_TIME&&stTestInfo.unPktCount>0),"pes 15Mbps, 32K per packet length ,test fail\n");

	bRet = TRUE;
	
	CSTK_FATAL_POINT
	
	if (hFilter)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter),
			"free  filter failure");
	}
	if (stTestInfo.pTmpBuf )
	{
		CSUDIOSFree(stTestInfo.pTmpBuf);
	}

	return bRet;
}

static  void CSUDISECTIONCallback_0031( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	unsigned int *punTotalSec = (unsigned int *)pvUserData;
	
   	if(pSection == CSUDI_NULL || punTotalSec == NULL)
	{
		return ;
	}

	switch(eType)
	{
		case EM_UDI_REQ_SECTION_DATA:
		{
			int i=0;

			(*punTotalSec)++;
			for(i=0;i<8;i++)
			{
				CSTCPrint("%x ",pSection->m_pucData[i]);
			}
			CSTCPrint("\n");
			break;
		}
		default:
			break;
	}
}

static unsigned int g_unSectionCnt = 0;

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����CRC�����section�������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: 1.һ��CRCֵȫ�������section����
//@INPUT: 2.pid = 6003
//@INPUT: 3.match[0]=0x3c,mask[0]=0xff,match[2]=0x1,mask[2]=0xff
//@EXPECTATION:1.��CRCCheck = FALSEʱ,Ӧ�����յ�section
//@EXPECTATION:2.��CRCCheck = TRUEʱ,Ӧ�ò����յ�section
//@EXECUTIONFLOW: 1������coship_ads_section.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����filter,�ر�CRC��飬��������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����filter�����������ɹ�
//@EXECUTIONFLOW: 4��sleep 3s,Ȼ�����������״̬,�������յ��İ�������0
//@EXECUTIONFLOW: 5������CSUDIFILTERFree�ͷ�filter,�������سɹ�
//@EXECUTIONFLOW: 6����CRC���,�ٵ���CSUDIFILTERAllocate����filter,��������ɹ�
//@EXECUTIONFLOW: 7������CSUDIFILTERStart����filter�����������ɹ�
//@EXECUTIONFLOW: 8��sleep 3s,Ȼ�����������״̬,�������յ��İ���Ϊ0
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�filter,�������سɹ�
CSUDI_BOOL CSTC_SECTION_IT_0031(void)
{
	CSUDI_HANDLE hFilter = CSUDI_NULL;
	CSUDISECTIONRequestInfo_S stSectionReq;

	g_unSectionCnt = 0;
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,COSHIP_ADS_SECTION_TS)),
			"Step 1 lock frequence failure !!!"
		);

	memset(&stSectionReq,0,sizeof(stSectionReq));
	stSectionReq.m_nDemuxID = DEMUX_0;
	stSectionReq.m_eReqType = EM_UDI_REQ_SECTION_DATA;
	stSectionReq.m_nPID = 6003;
	stSectionReq.m_sMMFilter.match[0]=0x3c;
	stSectionReq.m_sMMFilter.mask[0]=0xff;
	stSectionReq.m_sMMFilter.match[2]=0x1;
	stSectionReq.m_sMMFilter.mask[2]=0xff;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSectionReq,CSUDISECTIONCallback_0031,&g_unSectionCnt,&hFilter),
			"failure create filter error\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilter),
		"failure start  filter error\n");

	CSUDIOSThreadSleep(3000);
	CSTK_ASSERT_TRUE_FATAL(g_unSectionCnt > 0,"error : CRC check = false , there should be notifiction for CRC value stream\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter),
		"failure stop filter error\n");
	
	stSectionReq.m_bCRCCheck  = TRUE;
	g_unSectionCnt = 0;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERAllocate(&stSectionReq,CSUDISECTIONCallback_0031,&g_unSectionCnt,&hFilter),
			"failure create filter error\n");
		
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(hFilter),
		"failure start  filter error\n");
	CSUDIOSThreadSleep(3000);
	CSTK_ASSERT_TRUE_FATAL(g_unSectionCnt == 0,"error : CRC check = true , there should not be notifiction for CRC value stream\n");

	CSTK_FATAL_POINT
	if (hFilter)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(hFilter),
		"failure stop filter error\n");
	}

	return TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����filter��5���ֽ���ȵĹ��ˣ����������˲��õ����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PMT_FILTER004(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION:ÿһ�����õ�����ֵ
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PMT_FILTER004ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PMT_FILTER004ָ��������
//@EXECUTIONFLOW: 4��У��õ������ݣ�PMT_FILTER004�����ݻ�ȡ�ɹ��� 
//@EXECUTIONFLOW: 5������CSUDIFILTERStopֹͣfilter1�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ�filter2
CSUDI_BOOL CSTC_SECTION_IT_0032(void)
{
	CSUDISECTIONRequestInfo_S sFilterReqInfo;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sFilterReqInfo,EM_SECTION_PMT_FILTER_004,DEMUX_0,EM_PMT_PRO1_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sFilterReqInfo,(void*)EM_SECTION_PMT_FILTER_004,&hFilter1)),
			"Step 2 Allocate filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter1),
			"Step 2 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_004)),
			"Step 3 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sFilterReqInfo,(void*)EM_SECTION_PMT_FILTER_004,CSUDI_TRUE)),
			"Step 4 Check Expected Program1 Failure !!!"
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_004)),
			"Step 5 Stop Filter Failure !!!"
		);

	CSTK_FATAL_POINT;
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_004)),
				"Step 6 Free Filter Failure !!!"
			);
		hFilter1 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����filter��5����ˣ��ҳ�TableID,ȫ��Ϊ������˵����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PMT_FILTER005(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION:ÿһ�����õ�����ֵ
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PMT_FILTER005ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PMT_FILTER005ָ��������
//@EXECUTIONFLOW: 4��У��õ������ݣ�PMT_FILTER005�����ݻ�ȡ�ɹ���
//@EXECUTIONFLOW: 5������CSUDIFILTERStopֹͣfilter�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ�filter
CSUDI_BOOL CSTC_SECTION_IT_0033(void)
{
	CSUDISECTIONRequestInfo_S sFilterReqInfo;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sFilterReqInfo,EM_SECTION_PMT_FILTER_005,DEMUX_0,EM_PMT_PRO2_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sFilterReqInfo,(void*)EM_SECTION_PMT_FILTER_005,&hFilter1)),
			"Step 2 Allocate filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter1),
			"Step 2 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_005)),
			"Step 3 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sFilterReqInfo,(void*)EM_SECTION_PMT_FILTER_005,CSUDI_TRUE)),
			"Step 4 Check Expected Program1 Failure !!!"
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_005)),
			"Step 5 Stop Filter Failure !!!"
		);

	
	CSTK_FATAL_POINT;
	
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_005)),
				"Step 6 Free Filter Failure !!!"
			);
		hFilter1 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ���Թ������������Ϊ15���ֽ�ʱ��������(��������)
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PMT_FILTER006(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION:ÿһ�����õ�����ֵ
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PMT_FILTER006ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PMT_FILTER006ָ��������
//@EXECUTIONFLOW: 4��У��õ������ݣ�PMT_FILTER006�����ݻ�ȡ�ɹ����һ�ȡ��������ȷ��
//@EXECUTIONFLOW: 5������CSUDIFILTERStopֹͣfilter1�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ�filter1
CSUDI_BOOL CSTC_SECTION_IT_0034(void)
{
	CSUDISECTIONRequestInfo_S sFilterReqInfo1;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sFilterReqInfo1,EM_SECTION_PMT_FILTER_006,DEMUX_0,EM_PMT_PRO3_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sFilterReqInfo1,(void*)EM_SECTION_PMT_FILTER_006,&hFilter1)),
			"Step 2 Allocate filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter1),
			"Step 2 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_006)),
			"Step 4 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sFilterReqInfo1,(void*)EM_SECTION_PMT_FILTER_006,CSUDI_TRUE)),
			"Step 6 Check Expected EM_SECTION_EIT_FILTER_002 Failure !!!"
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_006)),
			"Step 7 Stop Filter Failure !!!"
		);

	CSTK_FATAL_POINT;
	
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_006)),
				"Step 9 Free Filter Failure !!!"
			);
		hFilter1 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����ͬʱ����ò�ͬ��filter����tableID ��ͬPID��ͬ�ı�����,�ص�������ͬ
//@DESCRIPTION: ����:���PMT ������table_ext��������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PMT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 1��PMT_FILTER002(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 1��PMT_FILTER003(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION:ÿһ�����õ�����ֵ
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PMT_FILTER001ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate����PMT_FILTER002ָ����filter2����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERAllocate����PMT_FILTER003ָ����filter3����������ɹ�
//@EXECUTIONFLOW: 5������CSUDIFILTERStart��ʼ����PMT_FILTER001ָ��������
//@EXECUTIONFLOW: 6������CSUDIFILTERStart��ʼ����PMT_FILTER002ָ��������
//@EXECUTIONFLOW: 7������CSUDIFILTERStart��ʼ����PMT_FILTER003ָ��������
//@EXECUTIONFLOW: 8��У��õ������ݣ�PMT_FILTER001,PMT_FILTER002,PMT_FILTER003�����ݶ���ȡ�ɹ����һ�ȡ�����ݶ���ȷ��û�ж�������ݡ�
//@EXECUTIONFLOW: 9������CSUDIFILTERStopֹͣfilter1�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 10������CSUDIFILTERStopֹͣfilter2�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 11������CSUDIFILTERStopֹͣfilter3�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 12������CSUDIFILTERFree�ͷ�filter1
//@EXECUTIONFLOW: 13������CSUDIFILTERFree�ͷ�filter2
//@EXECUTIONFLOW: 14������CSUDIFILTERFree�ͷ�filter3
CSUDI_BOOL CSTC_SECTION_IT_0035(void)
{
	CSUDISECTIONRequestInfo_S sFilterReqInfo1;
	CSUDISECTIONRequestInfo_S sFilterReqInfo2;
	CSUDISECTIONRequestInfo_S sFilterReqInfo3;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;
	CSUDI_HANDLE hFilter3 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sFilterReqInfo1,EM_SECTION_PMT_FILTER_001,DEMUX_0,EM_PMT_PRO1_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sFilterReqInfo1,(void*)EM_SECTION_PMT_FILTER_001,&hFilter1)),
			"Step 2 Allocate filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter1),
			"Step 2 Failure !!!"
		);

	SECTION_iSetSectionFilter(&sFilterReqInfo2,EM_SECTION_PMT_FILTER_002,DEMUX_0,EM_PMT_PRO2_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sFilterReqInfo2,(void*)EM_SECTION_PMT_FILTER_002,&hFilter2)),
			"Step 3 Allocate filter2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter2),
			"Step 3 Failure !!!"
		);

	SECTION_iSetSectionFilter(&sFilterReqInfo3,EM_SECTION_PMT_FILTER_003,DEMUX_0,EM_PMT_PRO3_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sFilterReqInfo3,(void*)EM_SECTION_PMT_FILTER_003,&hFilter3)),
			"Step 4 Allocate filter3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter3),
			"Step 4 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_001)),
			"Step 5 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_PMT_FILTER_002)),
			"Step 6 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter3,(void*)EM_SECTION_PMT_FILTER_003)),
			"Step 7 Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sFilterReqInfo1,(void*)EM_SECTION_PMT_FILTER_001,CSUDI_TRUE)),
			"Step 8 Check Expected Program1 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sFilterReqInfo2,(void*)EM_SECTION_PMT_FILTER_002,CSUDI_TRUE)),
			"Step 8 Check Expected Program2 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sFilterReqInfo3,(void*)EM_SECTION_PMT_FILTER_003,CSUDI_TRUE)),
			"Step 8 Check Expected Program3 Failure !!!"
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_001)),
			"Step 9 Stop Filter Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter2,(void*)EM_SECTION_PMT_FILTER_002)),
			"Step 10 Stop Filter Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter3,(void*)EM_SECTION_PMT_FILTER_003)),
			"Step 11 Stop Filter Failure !!!"
		);
	
	CSTK_FATAL_POINT;
	
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PMT_FILTER_001)),
				"Step 12  Free Filter Failure !!!"
			);
		hFilter1 = CSUDI_NULL;
	}

	if(hFilter2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PMT_FILTER_002)),
				"Step 13  Free Filter Failure !!!"
			);
		hFilter2 = CSUDI_NULL;
	}

	if(hFilter3 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter3,(void*)EM_SECTION_PMT_FILTER_003)),
				"Step 14  Free Filter Failure !!!"
			);
		hFilter3 = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����ͬʱ����ò�ͬ��filter����ͬtableID ͬPID�ı�����,�ص�������ͬ
//@DESCRIPTION: ����:EIT�Ķ��section������sectionNum��������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��EIT_FILTER002(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 1��EIT_FILTER003(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION:ÿһ�����õ�����ֵ
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����EIT_FILTER002ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERAllocate����EIT_FILTER003ָ����filter2����������ɹ�
//@EXECUTIONFLOW: 4������CSUDIFILTERStart��ʼ����PMT_FILTER001ָ��������
//@EXECUTIONFLOW: 5������CSUDIFILTERStart��ʼ����PMT_FILTER002ָ��������
//@EXECUTIONFLOW: 6��У��õ������ݣ�EIT_FILTER002,EIT_FILTER003�����ݶ���ȡ�ɹ����һ�ȡ�����ݶ���ȷ��û�ж�������ݡ�
//@EXECUTIONFLOW: 7������CSUDIFILTERStopֹͣfilter1�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSUDIFILTERStopֹͣfilter2�������ɹ�����CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIFILTERFree�ͷ�filter1
//@EXECUTIONFLOW: 10������CSUDIFILTERFree�ͷ�filter2
CSUDI_BOOL CSTC_SECTION_IT_0036(void)
{
	CSUDISECTIONRequestInfo_S sFilterReqInfo1;
	CSUDISECTIONRequestInfo_S sFilterReqInfo2;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	CSUDI_HANDLE hFilter2 = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	SECTION_iSetSectionFilter(&sFilterReqInfo1,EM_SECTION_EIT_FILTER_002,DEMUX_0,EM_EIT_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sFilterReqInfo1,(void*)EM_SECTION_EIT_FILTER_002,&hFilter1)),
			"Step 2 Allocate filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter1),
			"Step 2 Failure !!!"
		);

	SECTION_iSetSectionFilter(&sFilterReqInfo2,EM_SECTION_EIT_FILTER_003,DEMUX_0,EM_EIT_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sFilterReqInfo2,(void*)EM_SECTION_EIT_FILTER_003,&hFilter2)),
			"Step 3 Allocate filter2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter2),
			"Step 3 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter1,(void*)EM_SECTION_EIT_FILTER_002)),
			"Step 4 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter2,(void*)EM_SECTION_EIT_FILTER_003)),
			"Step 5 Failure !!!"
	);

	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sFilterReqInfo1,(void*)EM_SECTION_EIT_FILTER_002,CSUDI_TRUE)),
			"Step 6 Check Expected EM_SECTION_EIT_FILTER_002 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(/*�˴���ȴ�2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sFilterReqInfo2,(void*)EM_SECTION_EIT_FILTER_003,CSUDI_TRUE)),
			"Step 6 Check Expected EM_SECTION_EIT_FILTER_003 Failure !!!"
		);
		
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_EIT_FILTER_002)),
			"Step 7 Stop Filter Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter2,(void*)EM_SECTION_EIT_FILTER_003)),
			"Step 8 Stop Filter Failure !!!"
		);

	CSTK_FATAL_POINT;
	
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_EIT_FILTER_002)),
				"Step 9 Free Filter Failure !!!"
			);
		hFilter1 = CSUDI_NULL;
	}

	if(hFilter2 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_EIT_FILTER_003)),
				"Step 10  Free Filter Failure !!!"
			);
		hFilter2 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERSetBufSize
//@CASEGROUP: 3��CSUDIFILTERFree
//@DESCRIPTION: �������õ�bufsize�����ײ�֧�ֵ����ֵ�����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��PMT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT:CSUDIFILTERSetBufSize����:
//@INPUT:1��409600
//@EXPECTATION:ÿһ�����õ�����ֵ
//@EXECUTIONFLOW: 1������CSUDIFILTERAllocate����PMT_FILTER001ָ����filter1����������ɹ�
//@EXECUTIONFLOW: 2������CSUDIFILTERSetBufSize����buf��С�����سɹ���
//@EXECUTIONFLOW: 3������CSUDIFILTERFree�ͷ�filter1
CSUDI_BOOL CSTC_SECTION_IT_0037(void)
{
	CSUDISECTIONRequestInfo_S sFilterReqInfo1;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;

	SECTION_iSetSectionFilter(&sFilterReqInfo1,EM_SECTION_EIT_FILTER_002,DEMUX_0,EM_EIT_PID);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iAllocFilter(&sFilterReqInfo1,(void*)EM_SECTION_EIT_FILTER_002,&hFilter1)),
			"Step 1 Allocate filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter1),
			"Step 1 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERSetBufSize(hFilter1,409600)),
			"Step 2 Failure !!!"
		);

	CSTK_FATAL_POINT;
	
	if(hFilter1 != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_EIT_FILTER_002)),
				"Step 3 Free Filter Failure !!!"
			);
		hFilter1 = CSUDI_NULL;
	}

	return CSUDI_TRUE;
}

unsigned char g_CSTC_SECTION_cVersion=0;

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERSetBufSize
//@CASEGROUP: 3��CSUDIFILTERFree
//@DESCRIPTION: ��������PAT��ʱ����汾�Ų������仯�ڶ������󲻵�����
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3��CSUDIFILTERAllocate�ܳɹ�����filter
//@PRECONDITION: 4��CSUDIFILTERStart�ܳɹ�����һ��filter
//@PRECONDITION: 5��CSUDIFILTERSop�ܳɹ�ֹͣһ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��EIT_FILTER001(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ��һ������ɹ����ڶ������󲻵�����
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER001ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪPAT_FILTER001ָ��������
//@EXECUTIONFLOW: 5������CSUDIFILTERStopֹͣPAT_FILTER001
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ�filter
//@EXECUTIONFLOW: 7�����ð汾��Ϊ��ע�ֶΣ��ٴε���CSUDIFILTERAllocate����PAT_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 8������CSUDIFILTERStart��ʼ�������ݣ���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9��У��õ������ݣ�����Ϊ����ʧ�ܣ����벻������
//@EXECUTIONFLOW: 10������CSUDIFILTERFree�ͷ�filter
CSUDI_BOOL CSTC_SECTION_IT_0038(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter = CSUDI_NULL;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);
	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_001,DEMUX_0,EM_PAT_PID);
	SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_001,CSUDI_FALSE,CSUDI_FALSE);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERAllocate(&sTmpReqInfo,SECTION_iCallback_3,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 2 Allocate filter failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 3 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE== SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 4 Check Expected Data Failure !!!"
				);
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 5 Stop filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 6 Free Filter Failure !!!"
			);
	sTmpReqInfo.m_sMMFilter.match[3] = g_CSTC_SECTION_cVersion << 1;
	sTmpReqInfo.m_sMMFilter.mask[3] =  0x3E;
	sTmpReqInfo.m_sMMFilter.negate[3] = 0x3E;
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERAllocate(&sTmpReqInfo,SECTION_iCallback_3,(void*)EM_SECTION_PAT_FILTER_001,&hFilter)),
			"Step 7 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != hFilter),
			"Step 7 Allocate filter failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 8Failure !!!"
		);
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE!= SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 9 Check Expected Data Failure !!!"
				);
	
	CSTK_FATAL_POINT;
		
	if(hFilter != CSUDI_NULL)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_TRUE == SECTION_iFreeFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
				"Step 10  Free Filter Failure !!!"
			);
	}
	return CSUDI_TRUE;

}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERSetBufSize
//@CASEGROUP: 3��CSUDIFILTERFree
//@DESCRIPTION: ��������buf��СΪ512Kʱ����������ʮ�����(ƽ��ÿ������50����section),���ᶪ��
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��OC_FILTER001~OC_FILTER009(��ο�CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2���Ϸ��Ļص�����
//@INPUT:CSUDIFILTERSetBufSize����:
//@INPUT:1��512K
//@EXPECTATION:������������һ�ֽ�����ȫ������
//@EXECUTIONFLOW: 1������oc_test.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����OC_FILTER001ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart������filter�����������ɹ�
//@EXECUTIONFLOW: 4���������ݲ��Ҽ�¼section���У�������ֶ���û����������ȡ����filter
//@EXECUTIONFLOW: 5������Ƿ�������ȫ�Ƿ񶪰�
//@EXECUTIONFLOW: 6���ظ�����2��5������16��OC_FILTER001~OC_FILTER009ָ����filter������ÿһ�����ɹ�
CSUDI_BOOL CSTC_SECTION_IT_0039(void)
{
	CSTCPrint("�����������Ĳ�������Ϊ24M!\n");
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(0,OC_TEST_TS)),
			"Step 1 lock frequence failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
		(CSUDI_TRUE == SECTION_iTest_IT_39()),
		"Test do not pass !!!"
		);

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP: 1��CSUDIFILTERAllocate
//@CASEGROUP: 2��CSUDIFILTERStart
//@CASEGROUP: 3��CSUDIFILTERStop
//@CASEGROUP: 4��CSUDIFILTERFree
//@DESCRIPTION: ����Filterͬʱ����teletext���ݺ�PSI/SI����
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��EM_SECTION_PSISI_PES_FILTER_PES,EM_SECTION_PSISI_PES_FILTER_PSISI(���SECTION_Filter_E)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿһ���õ�����ֵ
//@EXECUTIONFLOW: 1������Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����hFilter_pes��hFilter_psisi����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart����hFilter_pes��hFilter_psisi�����������ɹ�
//@EXECUTIONFLOW: 4��У��õ������ݣ�����ΪEM_SECTION_PSISI_PES_FILTER_PES��EM_SECTION_PSISI_PES_FILTER_PSISIָ��������ƥ��
//@EXECUTIONFLOW: 5������CSUDIFILTERStopֹͣhFilter_pes��hFilter_psisi
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ�hFilter_pes��hFilter_psisi
CSUDI_BOOL CSTC_SECTION_IT_0040(void)
{
	CSUDISECTIONRequestInfo_S stReqInfo_pes,stReqInfo_psisi;
	CSUDI_HANDLE hFilter_pes = CSUDI_NULL;
	CSUDI_HANDLE hFilter_psisi = CSUDI_NULL;
	CSUDISECTIONCallbackData_S sParam;

	memset(&sParam, 0 ,sizeof(CSUDISECTIONCallbackData_S));
	
	CSTCPrint("�뽫%s����������������Ϊ 38M \n", TELETEXT_SUBTITLE_25MBPS_071220_TS);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,TELETEXT_SUBTITLE_25MBPS_071220_TS)),
	    	"Step 1 lock frequence failure !!!"
		);

	SECTION_iSetPESReq(&stReqInfo_pes,EM_SECTION_PSISI_PES_FILTER_PES,DEMUX_0,EM_SUBTITLE_001_PID);
	CSTK_ASSERT_TRUE_FATAL(
	    	(CSUDI_TRUE == SECTION_iAllocFilter(&stReqInfo_pes,(void*)EM_SECTION_PSISI_PES_FILTER_PES,&hFilter_pes)),
	    	"Step 2 Allocate filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
	    (CSUDI_NULL != hFilter_pes),
	    "Step 2 Failure !!!"
	);

	SECTION_iSetSectionFilter(&stReqInfo_psisi,EM_SECTION_PSISI_PES_FILTER_PSISI,DEMUX_0,EM_EIT_PID);
	CSTK_ASSERT_TRUE_FATAL(
	    	(CSUDI_TRUE == SECTION_iAllocFilter(&stReqInfo_psisi,(void*)EM_SECTION_PSISI_PES_FILTER_PSISI,&hFilter_psisi)),
	    	"Step 2 Allocate filter1 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
	    	(CSUDI_NULL != hFilter_pes),
	    	"Step 2 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
	    	(CSUDI_TRUE == SECTION_iStartFilter(hFilter_pes,(void*)EM_SECTION_PSISI_PES_FILTER_PES)),
	   		"Step 3 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
	    	(CSUDI_TRUE == SECTION_iStartFilter(hFilter_psisi,(void*)EM_SECTION_PSISI_PES_FILTER_PSISI)),
	    	"Step 3 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
	    	(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&stReqInfo_pes,(void*)EM_SECTION_PSISI_PES_FILTER_PES,CSUDI_TRUE)),
	    	"Step 4 Check Expected Data Failure !!!"
		);
    
	CSTK_ASSERT_TRUE_FATAL(
	    	(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&stReqInfo_psisi,(void*)EM_SECTION_PSISI_PES_FILTER_PSISI,CSUDI_TRUE)),
	    	"Step 4 Check Expected Data Failure !!!"
		);


	CSTK_ASSERT_TRUE_FATAL(
	    	(CSUDI_TRUE == SECTION_iStopFilter(hFilter_pes,(void*)EM_SECTION_PSISI_PES_FILTER_PES)),
	    	"Step 5 Stop Filter Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
	    	(CSUDI_TRUE == SECTION_iStopFilter(hFilter_psisi,(void*)EM_SECTION_PSISI_PES_FILTER_PSISI)),
	    	"Step 5 Stop Filter Failure !!!"
		);

	CSTK_FATAL_POINT;

	if(CSUDI_NULL != hFilter_pes)
	{
		CSTK_ASSERT_TRUE_FATAL(
		    	(CSUDI_TRUE == SECTION_iFreeFilter(hFilter_pes,(void*)EM_SECTION_PSISI_PES_FILTER_PES)),
		    	"Step 6  Free Filter Failure !!!"
			);
		hFilter_pes = CSUDI_NULL;
	}
	if(CSUDI_NULL != hFilter_psisi)
	{
		CSTK_ASSERT_TRUE_FATAL(
		    	(CSUDI_TRUE == SECTION_iFreeFilter(hFilter_psisi,(void*)EM_SECTION_PSISI_PES_FILTER_PSISI)),
		    	"Step 6  Free Filter Failure !!!"
			);
		hFilter_psisi = CSUDI_NULL;
	}
	return CSUDI_TRUE;
}

static CSUDI_BOOL s_bFilterStopped__0041 = CSUDI_FALSE;
static CSUDI_HANDLE s_hFilter_0041 = CSUDI_NULL; 
static void Section_iCallback_0041(CSUDISECTIONRequestType_E eType, void *pvParam, void *pvUserData)
{
	CSUDISECTIONCallbackData_S * pSection = (CSUDISECTIONCallbackData_S *)pvParam;
	
   	if(pSection == CSUDI_NULL || (int)pvUserData != EM_SECTION_PAT_FILTER_005)
	{
		return ;
	}
	
	if(CSUDI_SUCCESS != CSUDIFILTERStop(s_hFilter_0041))
	{
		CSTCPrint("Stop Filter error\n");
		return;
	}
	s_bFilterStopped__0041 = CSUDI_TRUE;
	
	return;
}

//@DESCRIPTION: ����Filter�ص��н���stop�Ȳ����᲻�ᵼ������
//@PRECONDITION: 1��ƽ̨�ѳɹ���ʼ��
//@PRECONDITION: 2��demux���Գɹ�����tuner����ȡ����
//@PRECONDITION: 3��CSUDIFILTERAllocate�ܳɹ�����filter
//@PRECONDITION: 4��CSUDIFILTERStart�ܳɹ�����һ��filter
//@PRECONDITION: 5��CSUDIFILTERSop�ܳɹ�ֹͣһ��filter
//@INPUT: CSUDIFILTERAllocate����:
//@INPUT: 1��EM_SECTION_PSISI_PES_FILTER_PES,EM_SECTION_PSISI_PES_FILTER_PSISI(���SECTION_Filter_E)
//@INPUT: 2���Ϸ��Ļص�����
//@EXPECTATION: ÿһ���õ�����ֵ
//@EXECUTIONFLOW: 1������EPG_0606_121458.ts�������ڵ�Ƶ��
//@EXECUTIONFLOW: 2������CSUDIFILTERAllocate����PAT_FILTER005ָ����filter����������ɹ�
//@EXECUTIONFLOW: 3������CSUDIFILTERStart��ʼ����PAT_FILTER005ָ�������ݣ������سɹ�
//@EXECUTIONFLOW: 4���ڻص������е���CSUDIFILTERStop������ϣ����������
//@EXECUTIONFLOW: 5��������ص�û��stop�ɹ�������CSUDIFILTERStop����
//@EXECUTIONFLOW: 6������CSUDIFILTERFree�ͷ�filter
CSUDI_BOOL CSTC_SECTION_IT_0041(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);

	SECTION_iSetSectionFilter(&sTmpReqInfo,EM_SECTION_PAT_FILTER_005,DEMUX_0,EM_PAT_PID);

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIFILTERAllocate(&sTmpReqInfo, Section_iCallback_0041, (void*)EM_SECTION_PAT_FILTER_005,&s_hFilter_0041)),
			"Step 2 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_NULL != s_hFilter_0041),
			"Step 2 Allocate filter failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStart(s_hFilter_0041),"Step 3 Start filter failure !!!");

	CSUDIOSThreadSleep(5000);

	CSTK_ASSERT_TRUE_FATAL(s_bFilterStopped__0041, "Step 4 stop filter failed in callback");

	CSTK_FATAL_POINT;

	if (!s_bFilterStopped__0041)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERStop(s_hFilter_0041),"Step 5 Stop filter failure !!!");
	}

	if(CSUDI_NULL != s_hFilter_0041)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIFILTERFree(s_hFilter_0041),"Step 6 Free filter failure !!!");
		s_hFilter_0041 = CSUDI_NULL;
	}

	s_bFilterStopped__0041 = CSUDI_FALSE;
	
	return CSUDI_TRUE;
}


