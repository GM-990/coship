/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
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

#define MAX_FILTER  			(100)   //测试demux支持的filter个数
#define MBYTE              (0x100000)  // 1M
#define EPG_TEST_ORIGNETWORKID (0X4080)
#define EPG_TEST_TSID   (0xffff)
#define EPG_TEST_SERVID (0xffff)

#define EPG_0606_121458_TS  "EPG_0606_121458.ts"    ///"EPG_0606_121458.ts"   //10.10.96.61:/f:/test_epg
#define TELETEXT_SUBTITLE_25MBPS_071220_TS "Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts" //公网 //10.10.96.61:/f:/test_epg
#define OC_TEST_TS "oc_test.ts"	 //10.10.96.61:/f:/test_epg
#define COSHIP_ADS_SECTION_TS "coship_ads_section.ts"
#define PES_PERFORMANCE_TEST_TS "PES_performance_test.ts"
#define EPG_0606_121458_TS_FOR_INJECT "testdata/EPG_0606_121458.ts"

#define CSMaxSectionLength (1024*4)
static char *g_pcSectionUserBuff = NULL;

extern CSUDI_HANDLE g_hUDISectionTestFilter;


CSUDI_BOOL CSTC_SECTION_Init(void)
{
	//在本测试用例集执行前调用
	SECTION_iTestInit();
	
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_SECTION_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

static void CSSectionCallback_common(CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData)
{
	return ;
}

//@CASEGROUP:所有测试用例
//@DESCRIPTION:测试用例的输入数据列举，描述了每个Filter取的测试值
//@PRECONDITION: 以下所有测试用例的输入请参考本用例中的输入描述
//@PRECONDITION: 请在执行目录下的testdata/填写好一下码流所在频点的配置信息
//@PRECONDITION: 1、测试码流文件EPG_0606_121458.ts
//@PRECONDITION: 2、测试码流文件Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts
//@PRECONDITION: 3、测试码流文件coship_oc_1M.ts
//@INPUT:PAT_FILTER001: 可获取数据，深度为1
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00
//@INPUT: MASK  :0xff
//@INPUT: NEGATE:0x00
//@INPUT:------------------------------
//@INPUT:PAT_FILTER002: 可获取数据，深度为1
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0xff
//@INPUT: MASK  :0xff
//@INPUT: NEGATE:0x01
//@INPUT:------------------------------
//@INPUT:PAT_FILTER003: 不可获取数据，深度为1
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00
//@INPUT: MASK  :0xff
//@INPUT: NEGATE:0x01
//@INPUT:------------------------------
//@INPUT:PAT_FILTER004: 可获取数据，深度为3
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b 
//@INPUT: MASK  :0xff,0xff,0xff 
//@INPUT: NEGATE:0x00,0x00,0x00 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER005: 可获取数据，深度为4
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7 
//@INPUT: MASK  :0xff,0xff,0xff,0xff 
//@INPUT: NEGATE:0x00,0x00,0x00,0x00 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER006: 可获取数据，深度为6
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER007: 不可获取数据，深度为6
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x01 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER008: 可获取数据，深度为8
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER009: 不可获取数据，深度为8
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID:0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01 
//@INPUT:------------------------------
//@INPUT:PAT_FILTER010: 可获取数据，深度为10
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:PAT_FILTER011: 不可获取数据，深度为10
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
//@INPUT:------------------------------
//@INPUT:PAT_FILTER012: 可获取数据，深度为12
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:PAT_FILTER013: 不可获取数据，深度为12
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
//@INPUT:------------------------------
//@INPUT:PAT_FILTER014: 可获取数据，深度为14
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x00,0x00,0x0b,0xd7,0x00,0x00,0x00,0x00,0xe0,0x10,0x00,0x70,0xe4,0x65 
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:PAT_FILTER015: 不可获取数据，深度为14
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
//@INPUT:EIT_FILTER001: 当前后续数据(other)，深度为3
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0x12
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x4f,0x00,0x66
//@INPUT: MASK  :0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:EIT_FILTER002:当前后续数据(other)，深度为5
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0x12
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x4f,0x00,0x66,0xc1,0x00
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:EIT_FILTER003: 当前后续数据(other)，深度为5
//@INPUT: ReqType: SECTION_DATA
//@INPUT: PID: 0x12
//@INPUT: CRCCheck: 1
//@INPUT: MACTH :0x4f,0x00,0x66,0xc1,0x01
//@INPUT: MASK  :0xff,0xff,0xff,0xff,0xff
//@INPUT: NEGATE:0x00,0x00,0x00,0x00,0x00
//@INPUT:------------------------------
//@INPUT:EIT_FILTER004: schedule数据(other)
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
//@INPUT:以上数据均从EPG_0606_121458.ts码流中分析获得
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
//@DESCRIPTION: 测试各个参数都为非法值CSUDI_NULL的情况
//@PRECONDITION: 无
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、Filter参数结构体指针为CSUDI_NULL，其它为合法值
//@INPUT: 2、回调函数指针为CSUDI_NULL，其它为合法值
//@INPUT: 3、Filter输出句柄为CSUDI_NUL，其它为合法值
//@INPUT: 4、Filter参数指针、回调函数指针、Filter输出句柄都为CSUDI_NULL的情况。
//@EXPECTATION:每种情况都返回失败
//@EXECUTIONFLOW: 1、调用CSUDIFILTERAllocate(CSUDI_NULL,合法指针,CSUDI_NULL,合法指针)，结果返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate(合法指针,CSUDI_NULL,CSUDI_NULL,合法指针),结果返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate(合法指针,合法指针,CSUDI_NULL,CSUDI_NULL),结果返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIFILTERAllocate(CSUDI_NULL,CSUDI_NULL,CSUDI_NULL,CSUDI_NULL),结果返回非CSUDI_SUCCESS
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
//@DESCRIPTION: 测试可以分配指定demux支持的filter个数
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、CSUDIDEMUXGetCapability可以正确获取能力信息
//@PRECONDITION: 3、CSUDIDEMUXGetFreeFilterCount可以正确获取空闲个数
//@PRECONDITION: 4、CSUDIFILTERFree能成功释放一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INOUT: 3、用户数据取不同的值
//@INOUT: 4、demuxIndex = 0
//@EXPECTATION: 可以成功分配得到demux支持的filter个数
//@REMARK: 本测试用例不关注申请到的filter是否可以接收到数据，并且分配的每个filter值是相同的
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCapability获取demux 0 的filter总数，期望获得DemuxCount(期望大于0)个filter
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、重复步骤2(DemuxCount-1)次，期望每一次都分配成功
//@EXECUTIONFLOW: 4、调用CSUDIDEMUXGetFreeFilterCount获取PAT_FILTER001中指定demux空闲的filter个数，期望空闲数为0
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放第一个PAT_FILTER001，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIDEMUXGetFreeFilterCount获取PAT_FILTER001中指定demux空闲的filter个数，期望空闲数 = 释放总个数
//@EXECUTIONFLOW: 7、重复5、6步骤(DemuxCount-1)次，期望每次都释放成功
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0002(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nIndex = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*一般只有32个filter*/
	
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

	//放在此处是为了保证都遍历释放所有filter
	//如果在step3-step5之间失败，filterid == 0的filter就不会被释放掉
	//所以释放资源依然应该从0开始。有if(CSUDI_NULL != ahFilter[nIndex])
	//不用担心重复释放资源导致出错的情况
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
//@DESCRIPTION:测试传入的用户数据指针为CSUDI_NULL
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、CSUDIFILTERStart能成功启动一个filter
//@PRECONDITION: 4、CSUDIFILTERStop能成功停止一个filter
//@PRECONDITION: 5、CSUDIFILTERFree能成功释放一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、用户数据指针为合法值CSUDI_NULL
//@INPUT: 3、合法的回调函数
//@INOUT: 4、demuxIndex = 0
//@EXPECTATION: 能够成功分配filter，能够成功接收数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据，期望启动成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为PAT_FILTER001指定的数据,且用户数据应该为CSUDI_NULL
//@EXECUTIONFLOW: 5、如果步骤4接收数据超时，则此用例失败
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放PAT_FILTER001，期望成功CSUDI_SUCCESS
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus_UserDataNULL(CSUDI_TRUE,&sTmpReqInfo,CSUDI_NULL,CSUDI_TRUE) && g_bUserDataIsNUll == CSUDI_TRUE),
			"Step 4 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
//@DESCRIPTION:测试可以正确得到传入的用户数据(测试系统中所有demux)
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接和断开tuner并获取数据
//@PRECONDITION: 3、CSUDIFILTERStart能成功启动一个filter
//@PRECONDITION: 4、CSUDIFILTERStop能成功停止一个filter
//@PRECONDITION: 5、CSUDIFILTERFree能成功释放一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、用户数据为整数 0 
//@INPUT: 3、合法的回调函数
//@EXPECTATION: 可以正确获取用户数据,并且每个demux的测试都符合期望值
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount获取demux个数
//@EXECUTIONFLOW: 2、调用CSUDIDEMUXGetCapability获取DemuxIndex支持的filter个数filterCount
//@EXECUTIONFLOW: 3、如果filterCount为0(不支持过滤)，则执行步骤11
//@EXECUTIONFLOW: 4、调用CSUDIDEMUXConnectTuner(nDemuxIndex,0)将nDemuxIndex连接到0号tuner(不支持则执行步骤11)
//@EXECUTIONFLOW: 5、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 6、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据，期望启动成功
//@EXECUTIONFLOW: 8、期望在回调函数中获取的用户数据为整数 0
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放PAT_FILTER001，期望成功CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、调用CSUDIDEMUXDisconnectTuner断开nDemuxIndex与0号tuner连接
//@EXECUTIONFLOW: 11、取下一个demux(nDemuxIndex++)
//@EXECUTIONFLOW: 12、重复步骤2到11，直到所有demux都测试
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
		CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

	//恢复默认的TUNER_0和DEMUX_0连接
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
//@DESCRIPTION: 测试可以分配的filter个数大于指定demux支持的filter个数，并且每个filter取相同值的情况
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、CSUDIDEMUXGetCapability可以正常工作
//@PRECONDITION: 3、CSUDIDEMUXGetFreeFilterCount可以正常工作
//@PRECONDITION: 4、CSUDIFILTERFree能成功释放一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INOUT: 3、demuxIndex = 0
//@EXPECTATION: 分配超过demux支持的个数时，返回CSUDISECTION_ERROR_NO_FREE_FILTER
//@REMARK: 本测试用例不关注申请到的filter是否可以接收到数据，并且分配的每个filter条件是相同的
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCapability获取demux0的filter总数，期望获得FilterCount个filter
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动filter，期望返回成功
//@EXECUTIONFLOW: 4、重复步骤2(FilterCount)次，期望每一次都分配成功
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 中空闲的filter个数，期望空闲数为0
//@EXECUTIONFLOW: 6、再次调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配失败，并返回CSUDISECTION_ERROR_NO_FREE_FILTER
//@EXECUTIONFLOW: 7、调用CSUDIFILTERFree释放所有分配的filter
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0005(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nIndex = 0;
	CSUDI_Error_Code nRet = CSUDI_FAILURE;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*一般只有32个filter*/

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
//@DESCRIPTION:  测试可以分配的filter个数大于16以上是否能分配成功并且可以申请到数据
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、CSUDIDEMUXGetCapability可以正常工作
//@PRECONDITION: 3、CSUDIDEMUXGetFreeFilterCount可以正常工作
//@PRECONDITION: 4、CSUDIFILTERFree能成功释放一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INOUT: 3、demuxIndex = 0
//@EXPECTATION: 分配成功期望的超过16个filter，并且每个filter都能正常工作
//@REMARK: 本测试用例不关注申请到的filter是否可以接收到数据，并且分配的每个filter条件是相同的
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCapability获取demux0的filter总数，期望获得FilterCount个filter
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配0到EM_SECTION_PAT_FILTER_MAX和EM_SECTION_EIT_FILTER_001到EM_SECTION_EIT_FILTER_MAX指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动filter，期望返回成功
//@EXECUTIONFLOW: 4、检查步骤2分配的所有filter获取数据的情况，期望获得每个filter制定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放所有分配的filter
CSUDI_BOOL CSTC_SECTION_IT_Allocate_0006(void)
{
	int nTotalFilteCount = 0;
	int nIndex = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*一般只有32个filter*/

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == CSTC_UDI2PortingLock(TUNER_0,EPG_0606_121458_TS)),
			"Step 1 lock frequence failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(
			CSUDI_SUCCESS == CSUDIDEMUXGetCapability(DEMUX_0, &sTmpCap),
			"Step 1 failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(nTotalFilteCount = sTmpCap.m_nFilterNum) > 16,/*这里期望至少大于16个filter*/
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
                CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
                        (CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)nIndex,CSUDI_FALSE)),
                        "Step 4 Check Expected Data Failure !!!"
                    );  
            }
            break;
            /*expected data  received*/
            default:
            {
                CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
        CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
//@DESCRIPTION: 测试Filter句柄为非法值CSUDI_NULL的情况
//@PRECONDITION: 无
//@INPUT: Filter句柄为CSUDI_NULL
//@EXPECTATION: 返回失败
//@EXECUTIONFLOW: 调用CSUDIFILTERStart(CSUDI_NULL),期望返回非CSUDI_SUCCESS
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
//@DESCRIPTION: 测试可以成功启动一个合法的filter接收数据
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、CSUDIFILTERAllocate能成功分配filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 成功启动filter，返回CSUDI_SUCCESS；得到PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据，并返回成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放PAT_FILTER001
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
//@DESCRIPTION: 测试可以成功启动一个已经停止的filter
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、CSUDIFILTERAllocate能成功分配filter
//@PRECONDITION: 4、CSUDIFILTERStop能成功停止filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 成功启动filter，返回CSUDI_SUCCESS；得到PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据，并返回成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStop停止PAT_FILTER001，期望停止接收数据
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStart重新开始接收数据，期望返回成功，并重新得到PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 7、调用CSUDIFILTERFree释放PAT_FILTER001
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 4 Check Expected Data Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStopFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 5 Stop Filter Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s 获取不到数据*/
			(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
			"Step 5 Check Expected Data Failure !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_TRUE == SECTION_iStartFilter(hFilter,(void*)EM_SECTION_PAT_FILTER_001)),
			"Step 6 Failure !!!"
		);
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
//@DESCRIPTION: 测试Filter句柄为非法值CSUDI_NULL的情况
//@PRECONDITION: 无
//@INPUT:Filter 句柄为CSUDI_NULL
//@EXPECTATION: 返回失败
//@EXECUTIONFLOW: 调用CSUDIFILTERStop(CSUDI_NULL),期望返回非CSUDI_SUCCESS
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
//@DESCRIPTION: 测试Filter没有启动就进行stop的情况
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、CSUDIFILTERAllocate能成功分配filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 停止失败，返回失败
//@EXECUTIONFLOW: 1、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 2、调用CSUDIFILTERStop停止PAT_FILTER001，期望停止失败，期望返回CSUDISECTION_ERROR_INVALID_STATUS
//@EXECUTIONFLOW: 3、调用CSUDIFILTERFree释放PAT_FILTER001，期望释放成功，期望返回CSUDI_SUCCESS
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
//@DESCRIPTION: 测试filter在还没有start的时候不能有数据回调，start后应该有数据回调，stop后不能再有数据回调
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、CSUDIFILTERAllocate能成功分配filter
//@INPUT:码流EPG_0606_121458.ts
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: PAT_FILTER001 PAT_FILTER004、PAT_FILTER005、PAT_FILTER006(请参考CSTC_SECTION_IT_Allocate_0000)
//@EXPECTATION: 停止失败，返回失败
//@EXECUTIONFLOW: 1、锁住EPG_0606_121458.ts锁在的频点,期望成功
//@EXECUTIONFLOW: 2、调用CSUDIOSThreadCreate创建4个线程分别做如下3~9 步骤:
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate分配filter,filter分别为PAT_FILTER004、PAT_FILTER005、PAT_FILTER006,期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart启动filter,期望返回成功
//@EXECUTIONFLOW: 5、检查数据状态，期望有数据接收到
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStop停止filter,期望返回成功
//@EXECUTIONFLOW: 7、检查数据状态，期望停止后没有数据回调
//@EXECUTIONFLOW: 8、重复步骤4~7,期望每步都成功
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放filter，期望都返回成功
//@EXECUTIONFLOW: 10、调用CSUDIOSThreadDestroy消耗测试线程
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
//@DESCRIPTION: 测试Filter句柄为非法值CSUDI_NULL的情况
//@PRECONDITION: 无
//@INPUT: Filter句柄为CSUDI_NULL
//@EXPECTATION: 返回失败
//@EXECUTIONFLOW: 调用CSUDIFILTERFree(CSUDI_NULL),期望返回非CSUDI_SUCCESS
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
//@DESCRIPTION: 测试释放一个未启动的filter
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、CSUDIFILTERAllocate能成功分配filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 释放成功，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 2、调用CSUDIFILTERFree释放PAT_FILTER001，期望释放成功
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
//@DESCRIPTION: 测试反复分配释放filter
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、CSUDIFILTERAllocate能成功分配filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、pid = 0x0,m_eReqType=EM_UDI_REQ_SECTION_DATA,过滤条件全为0
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 1.在不调用CSUDIFILTERStart的情况下申请释放500次,期望每次都返回CSUDI_SUCCESS
//@EXPECTATION: 2.在调用CSUDIFILTERStart的情况下申请释放500次,期望每次都返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFILTERAllocate分配指定的filter，期望分配成功
//@EXECUTIONFLOW: 2、调用CSUDIFILTERFree释放filter，期望释放成功
//@EXECUTIONFLOW: 3、重复步骤1和2共500次
//@EXECUTIONFLOW: 4、调用CSUDIFILTERAllocate分配指定的filter，期望分配成功
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStart开始filter,期望释放成功
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放filter，期望释放成功
//@EXECUTIONFLOW: 7、重复步骤4~6共500次
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
//@DESCRIPTION: 测试在不同PID情况下反复分配释放filter
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、CSUDIFILTERAllocate能成功分配filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、pid随测试次数递增,m_eReqType=EM_UDI_REQ_SECTION_DATA,过滤条件全为0
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 1.在不调用CSUDIFILTERStart的情况下申请释放1000次,每次都用不同PID,期望每次都返回CSUDI_SUCCESS
//@EXPECTATION: 2.在调用CSUDIFILTERStart的情况下申请释放1000次,每次都用不同PID,期望每次都返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIFILTERAllocate分配指定的filter，期望分配成功
//@EXECUTIONFLOW: 2、调用CSUDIFILTERFree释放filter，期望释放成功
//@EXECUTIONFLOW: 3、将本次的PID加1用作下次PID
//@EXECUTIONFLOW: 4、重复步骤1~3共1000次
//@EXECUTIONFLOW: 5、调用CSUDIFILTERAllocate分配指定的filter，期望分配成功
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStart开始filter,期望释放成功
//@EXECUTIONFLOW: 7、调用CSUDIFILTERFree释放filter，期望释放成功
//@EXECUTIONFLOW: 8、将本次的PID加1用作下次PID
//@EXECUTIONFLOW: 9、重复步骤5~8共1000次
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
//@DESCRIPTION: 测试Filter句柄为非法值CSUDI_NULL的情况
//@PRECONDITION: 无
//@INPUT: Filter句柄为CSUDI_NULL
//@EXPECTATION: 返回失败
//@EXECUTIONFLOW: 调用CSUDIFILTERModify(CSUDI_NULL，CSUDI_NULL，CSUDI_FALSE),期望返回非CSUDI_SUCCESS
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
//@DESCRIPTION: 测试修改一个未停止的filter
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、CSUDIFILTERAllocate能成功分配filter
//@PRECONDITION: 4、CSUDIFILTERStart能成功启动一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 修改失败，返回CSUDISECTION_ERROR_INVALID_STATUS
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据，并返回成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERModify(filter,CSUDI_NULL,CSUDI_TRUE)修改PAT_FILTER001，修改失败，并返回CSUDISECTION_ERROR_INVALID_STATUS
//@EXECUTIONFLOW: 6、校验得到的数据，期望仍为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 7、调用CSUDIFILTERFree释放PAT_FILTER001
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 4 Check Expected Data Failure !!!"
		);
	
	SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_001,CSUDI_FALSE,CSUDI_FALSE);
	
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDISECTION_ERROR_INVALID_STATUS == CSUDIFILTERModify(hFilter,CSUDI_NULL,CSUDI_TRUE)),
			"Step 5 should not modify a running Filter !!!"
		);
	
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
//@DESCRIPTION: 测试能够成功修改一个停止的filter
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、CSUDIFILTERAllocate能成功分配filter
//@PRECONDITION: 4、CSUDIFILTERStart能成功启动一个filter
//@PRECONDITION: 5、CSUDIFILTERSop能成功停止一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、PAT_FILTER002(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 3、合法的回调函数
//@EXPECTATION: 修改成功，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据，并返回成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStop停止PAT_FILTER001
//@EXECUTIONFLOW: 6、调用CSUDIFILTERModify修改PAT_FILTER001为PAT_FILTER002，修改成功，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStart开始请求PAT_FILTER002指定的数据，并返回成功
//@EXECUTIONFLOW: 8、校验得到的数据，期望为FILTER02指定的数据
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放filter
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
    /*这里去检查数据的时候还是传EM_SECTION_PAT_FILTER_001索引，
     *是因为SECTION_iAllocFilter里面的回调函数实际上只处理和设置EM_SECTION_PAT_FILTER_001，
     *因此此处不应该传EM_SECTION_PAT_FILTER_002
    */
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
//@DESCRIPTION: 测试能够成功修改一个没有进行CRC检查的filter，使其进行CRC检查(测试没有CRC值的数据(TDT)进行CRC检查，应该接收不到数据)
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、CSUDIFILTERAllocate能成功分配filter
//@PRECONDITION: 4、CSUDIFILTERStart能成功启动一个filter
//@PRECONDITION: 5、CSUDIFILTERSop能成功停止一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、TDT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、CRC检测为CSUDI_FALSE
//@INPUT: 3、合法的回调函数
//@EXPECTATION: 修改成功，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate(TDT_FILTER001,callback,userdata,合法指针)分配TDT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERModify修改bCRCCheck为CSUDI_TRUE，修改成功，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart开始请求TDT_FILTER001指定的数据，并返回成功
//@EXECUTIONFLOW: 5、校验得到的数据，期望无法请求到数据
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStop停止filter
//@EXECUTIONFLOW: 7、调用CSUDIFILTERModify修改bCRCCheck为CSUDI_FALSE，修改成功，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSUDIFILTERStart开始请求TDT_FILTER001指定的数据，并返回成功
//@EXECUTIONFLOW: 9、期望可以接收到TDT_FILTER001指定的数据
//@EXECUTIONFLOW: 10、调用CSUDIFILTERFree释放filter
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
//@DESCRIPTION:测试Filter句柄为非法值CSUDI_NULL的情况
//@PRECONDITION:无
//@INPUT:Filter句柄为CSUDI_NULL
//@EXPECTATION:返回失败
//@EXECUTIONFLOW: 调用CSUDIFILTERSetBufSize(CSUDI_NULL，1M),期望返回非CSUDI_SUCCESS
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
//@DESCRIPTION: 测试设置一个未停止的filter的buffer大小
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、CSUDIFILTERAllocate能成功分配filter
//@PRECONDITION: 3、CSUDIFILTERStart能成功启动一个filter
//@PRECONDITION: 4、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT: 3、buffer大小2M
//@EXPECTATION: 不允许设置，返回失败
//@REMARK: 成功设置一个filter的buffer大小的测试用例建议与demux的性能测试一起测试。
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据，并返回成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERSetBufSize设置PAT_FILTER001的buffer大小，设置失败，并返回失败
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放PAT_FILTER001
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试demux对Filter资源的管理情况
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001~PAT_FILTER006(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 可以正常的获得Filter总数及空闲数
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIDEMUXGetCapability获取demux 0 的filter总数
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 6、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 空闲的filter个数，期望空闲数+1=总数
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStop停止PAT_FILTER001
//@EXECUTIONFLOW: 8、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 空闲的filter个数，期望空闲数+1=总数
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放PAT_FILTER001
//@EXECUTIONFLOW: 10、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 空闲的filter个数，期望空闲数=总数
//@EXECUTIONFLOW: 11、分别以PAT_FILTER001~PAT_FILTER006来分配filter，期望分配成功，但不关注获取的数据
//@EXECUTIONFLOW: 12、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 空闲的filter个数,期望空闲数+6=总数
//@EXECUTIONFLOW: 13、调用CSUDIFILTERFree释放PAT_FILTER001
//@EXECUTIONFLOW: 14、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 空闲的filter个数,期望空闲数+5=总数
//@EXECUTIONFLOW: 15、调用CSUDIFILTERFree释放PAT_FILTER002~PAT_FILTER006
//@EXECUTIONFLOW: 16、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 空闲的filter个数,期望空闲数=总数
CSUDI_BOOL CSTC_SECTION_IT_0001(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nFilterIndex = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*一般只有32个filter*/

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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

	CSTK_FATAL_POINT       //放在此处是为了保证都遍历释放所有filter
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试CSUDIFILTERModify修改filter为同一个fitler的情况
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 可以成功修改filter，并且获取数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStop停止PAT_FILTER001，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIFILTERModify修改PAT_FILTER001，参数仍为PAT_FILTER001，修改成功，返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 10、调用CSUDIFILTERFree释放PAT_FILTER001
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试对接口进行重复调用的情况
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每一次重复调用都失败，返回失败
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStart启动filter10次，每次都返回失败
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStop停止filter，期望成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSUDIFILTERStop停止filter10次，每次都返回失败
//@EXECUTIONFLOW: 9、调用CSUDIFILTERStart重新请求PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 10、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 11、在start状态下，调用CSUDIFILTERModify修改PAT_FILTER001为PAT_FILTER002，连续调用10次，每次都失败，并返回失败
//@EXECUTIONFLOW: 12、校验得到的数据，期望仍为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 13、调用CSUDIFILTERStop停止filter
//@EXECUTIONFLOW: 14、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 15、再次调用CSUDIFILTERStart启动filter10次，每次都返回失败
//@EXECUTIONFLOW: 16、调用CSUDIFILTERStop停止filter10次，每次都返回失败
//@EXECUTIONFLOW: 17、重复2-15步骤50次
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 10 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(
					SECTION_iSetMatchMask(&sMatch,EM_SECTION_PAT_FILTER_002),
					"Set filter value failure at step 11 !!!"
					); //modify filter1 to filter2
			for(i = 0;i<10; i++)
			{
				CSTK_ASSERT_TRUE_FATAL(/*此处传入EM_SECTION_PAT_FILTER_001便于step12 检查结果*/
						(CSUDI_FALSE == SECTION_iModifyFilter(hFilter,&sMatch,CSUDI_TRUE,(void*)EM_SECTION_PAT_FILTER_001)),
						"Step 11 Modify Filter at running status should be Failure !!!"
					);
			}
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTCPrint("距离本次循环结束还有 %d 次\n",TESTTIMES-nLoop);
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试Filter为不同深度时的PAT数据请求(filter深度分别为1、3、4、6;用例CSTC_SECTION_IT_0005测试8、10、12、14)
//@DESCRIPTION: 有些平台请求数据时，同一pid的请求会分配相同的filter，因此，每个请求之后都将filter释放
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、CSUDIDEMUXGetCapability可以正常工作
//@PRECONDITION: 4、CSUDIDEMUXGetFreeFilterCount可以正常工作
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001~PAT_FILTER007(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT: 3、测试demux 0
//@EXPECTATION: 每一步骤都能够成功获取或者未获取指定的数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 4、校验得到的数据，期望为PAT_FILTER001指定的数据，并释放filter
//@EXECUTIONFLOW: 5、调用CSUDIFILTERAllocate分配PAT_FILTER002指定的filter2，期望分配成功
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、校验得到的数据，期望得到PAT_FILTER002指定的数据，并释放filter
//@EXECUTIONFLOW: 8、调用CSUDIFILTERAllocate分配PAT_FILTER003指定的filter3，期望分配成功
//@EXECUTIONFLOW: 9、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、校验得到的数据，期望得不到任何数据，并释放filter
//@EXECUTIONFLOW: 11、调用CSUDIFILTERAllocate分配PAT_FILTER004指定的filter4，期望分配成功
//@EXECUTIONFLOW: 12、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 13、校验得到的数据，期望得到PAT_FILTER004指定的数据，并释放filter
//@EXECUTIONFLOW: 14、调用CSUDIFILTERAllocate分配PAT_FILTER005指定的filter5，期望分配成功
//@EXECUTIONFLOW: 15、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 16、校验得到的数据，期望得到PAT_FILTER005指定的数据，并释放filter
//@EXECUTIONFLOW: 17、调用CSUDIFILTERAllocate分配PAT_FILTER006指定的filter6，期望分配成功
//@EXECUTIONFLOW: 18、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 19、校验得到的数据，期望得到PAT_FILTER006指定的数据，并释放filter
//@EXECUTIONFLOW: 20、调用CSUDIFILTERAllocate分配PAT_FILTER007指定的filter7，期望分配成功
//@EXECUTIONFLOW: 21、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 22、校验得到的数据，期望得不到任何数据，并释放filter
//@EXECUTIONFLOW: 23、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 空闲的filter个数,期望空闲数 = 总数
CSUDI_BOOL CSTC_SECTION_IT_0004(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nFilterIndex = 0;
	int nLoop = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*一般只有32个filter*/


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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试Filter为不同深度时的PAT数据请求(filter深度为8、10、12、14)
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER008~PAT_FILTER015(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每一步骤都能够成功获取或者未获取指定的数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER008指定的filter8，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER008指定的数据
//@EXECUTIONFLOW: 4、校验得到的数据，期望为PAT_FILTER008指定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERAllocate分配PAT_FILTER009指定的filter9，期望分配成功
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、校验得到的数据，期望得不到任何数据
//@EXECUTIONFLOW: 8、调用CSUDIFILTERAllocate分配PAT_FILTER010指定的filter10，期望分配成功
//@EXECUTIONFLOW: 9、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、校验得到的数据，期望为PAT_FILTER010指定的数据
//@EXECUTIONFLOW: 11、调用CSUDIFILTERAllocate分配PAT_FILTER011指定的filter11，期望分配成功
//@EXECUTIONFLOW: 12、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 13、校验得到的数据，期望得不到任何数据
//@EXECUTIONFLOW: 14、调用CSUDIFILTERAllocate分配PAT_FILTER012指定的filter12，期望分配成功
//@EXECUTIONFLOW: 15、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 16、校验得到的数据，期望得到PAT_FILTER012指定的数据
//@EXECUTIONFLOW: 17、调用CSUDIFILTERAllocate分配PAT_FILTER013指定的filter13，期望分配成功
//@EXECUTIONFLOW: 18、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 19、校验得到的数据，期望得不到任何数据
//@EXECUTIONFLOW: 20、调用CSUDIFILTERAllocate分配PAT_FILTER014指定的filter14，期望分配成功
//@EXECUTIONFLOW: 21、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 22、校验得到的数据，期望得到PAT_FILTER014指定的数据
//@EXECUTIONFLOW: 23、调用CSUDIFILTERAllocate分配PAT_FILTER015指定的filter15，期望分配成功
//@EXECUTIONFLOW: 24、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 25、校验得到的数据，期望得不到任何数据
//@EXECUTIONFLOW: 26、调用CSUDIFILTERFree释放filter8~filter13
//@EXECUTIONFLOW: 27、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 空闲的filter个数,期望空闲数 + 2 = 总数
//@EXECUTIONFLOW: 28、调用CSUDIFILTERFree释放filter14~filter15
//@EXECUTIONFLOW: 29、调用CSUDIDEMUXGetFreeFilterCount获取demux 0 空闲的filter个数,期望空闲数 = 总数
//@EXECUTIONFLOW: 30、重复2-29步骤10次
CSUDI_BOOL CSTC_SECTION_IT_0005(void)
{
	int nTotalFilteCount = 0;
	int nFreeFilteCount  = 0;
	int nFilterIndex = 0;
	int nLoop = 0;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*一般只有32个filter*/


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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试Filter为CAT数据请求
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、CAT_FILTER001 (请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每一步得到期望值
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配CAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动filter，期望启动成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为CAT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 6、重复2-5步骤10次
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
		CSTK_ASSERT_TRUE_FATAL(/*此处会等待1s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERFree
//@DESCRIPTION: 测试Filter为视频数据请求
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、VIDEO_FILTER001~VIDEO_FILTER002 (请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每一步得到期望值
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配VIDEO_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动filter，期望启动成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望得到数据(此处不对数据进行校验)
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 6、调用CSUDIFILTERAllocate分配VIDEO_FILTER002指定的filter，期望分配成功
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStart开始请求VIDEO_FILTER002指定的数据
//@EXECUTIONFLOW: 8、校验得到的数据，期望得到数据(此处不对数据进行校验)
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 10、重复2-9步骤10次
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
		CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
		CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试Filter为音频数据请求
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、AUDIO_FILTER001~AUDIO_FILTER002 (请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每一步得到期望值
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配AUDIO_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动filter，期望启动成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望得到数据(此处不对数据进行校验)
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 6、调用CSUDIFILTERAllocate分配AUDIO_FILTER002指定的filter，期望分配成功
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStart开始请求AUDIO_FILTER002指定的数据
//@EXECUTIONFLOW: 8、校验得到的数据，期望得到数据(此处不对数据进行校验)
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 10、重复2-9步骤10次
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
		CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
		CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试Filter为subtitle数据请求
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、SUBTITLE_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每一步得到期望值
//@EXECUTIONFLOW: 1、锁定Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配SUBTITLE_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动filter，期望启动成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为SUBTITLE_FILTER001指定的数据(该数据应该是完整的包数据)
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 6、重复2-5步骤10次
CSUDI_BOOL CSTC_SECTION_IT_0009(void)
{
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDI_HANDLE hFilter1 = CSUDI_NULL;
	int nLoop = 0;

	CSTCPrint("请更换码流: %s \r\n",TELETEXT_SUBTITLE_25MBPS_071220_TS);
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
		CSTK_ASSERT_TRUE_FATAL(/*此处会等待5s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试Filter为teletext数据请求
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、TELETEXT_FILTER001 (请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每一步得到期望值
//@EXECUTIONFLOW: 1、锁定Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配TELETEXT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动filter，期望启动成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为TELETEXT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 6、重复2-5步骤10次
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
		CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试申请两个有相同的过滤条件，不同的回调函数和用户数据的filter时，操作其中一个应该不影响另一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、不同的回调函数callback1和callback2 
//@EXPECTATION: 停止或者释放其中一个filter，不会影响另一个filter的正常接收数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate(PAT_FILTER001,callback1,userdata1,filter1)分配filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate(PAT_FILTER001,callback2,userdata2,filter2)分配filter2，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart启动filter1和filter2，期望启动成功
//@EXECUTIONFLOW: 5、校验得到的数据，期望callback1和callback2得到的数据都为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStop停止filter1
//@EXECUTIONFLOW: 7、校验得到的数据，期望callback1接收不到任何数据且callback2可以接收到数据
//@EXECUTIONFLOW: 8、调用CSUDIFILTERFree释放filter1，期望释放成功
//@EXECUTIONFLOW: 9、校验得到的数据，期望callback1接收不到任何数据且callback2可以接收到数据
//@EXECUTIONFLOW: 10、调用CSUDIFILTERFree释放filter2
//@EXECUTIONFLOW: 11、重复2~10步骤10次
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 5 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_002,CSUDI_TRUE)),
					"Step 5 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 6 Stop filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 7 Filter1 Expected No Data Failure !!!"
				);	
			//检验filter2是否获取数据
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_002,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_002,CSUDI_TRUE)),
					"Step 7 Filter2 Expected Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 8 Free filter1 Failure !!!"
				);
			hFilter1 = CSUDI_NULL;
			
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 9 Filter1 Expected No Data Failure !!!"
				);	
			//检验filter2是否获取数据
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_002,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试申请两个有相同的过滤条件、相同的回调函数、不同的用户数据时的filter时，操作其中一个应该不影响另一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: 1、PAT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、相同的回调函数callback
//@EXPECTATION: 停止或者释放其中一个filter，不会影响另一个filter的正常接收数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate(PAT_FILTER001,callback,userdata1,filter1)分配filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate(PAT_FILTER001,callback,userdata2,filter2)分配filter2，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart启动filter1和filter2，期望启动成功
//@EXECUTIONFLOW: 5、校验得到的数据，期望callback得到(两次)数据都为PAT_FILTER001指定的数据,并且用户数据分别为userdata1和userdata2
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStop停止filter1
//@EXECUTIONFLOW: 7、校验得到的数据，期望callback接收不到用户数据为userdata1的数据但可以接收到用户数据为userdata2数据
//@EXECUTIONFLOW: 8、调用CSUDIFILTERFree释放filter1，期望释放成功
//@EXECUTIONFLOW: 9、校验得到的数据，期望callback接收不到任何用户数据为userdata1但可以接收到用户数据为userdata2数据
//@EXECUTIONFLOW: 10、调用CSUDIFILTERFree释放filter2
//@EXECUTIONFLOW: 11、重复2~10步骤10次
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 5 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,CSUDI_TRUE)),
					"Step 5 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 6 Stop filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 7 Filter1 Expected No Data Failure !!!"
				);	
			//检验filter2是否获取数据
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_003,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_003,CSUDI_TRUE)),
					"Step 7 Filter2 Expected Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 8 Free filter1 Failure !!!"
				);
			hFilter1 = CSUDI_NULL;
			
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 9 Filter1 Expected No Data Failure !!!"
				);	
			//检验filter2是否获取数据
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_003,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试申请两个有重叠的过滤条件，不同的回调函数的filter时，操作其中一个应该不影响另一个filter
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001、PAT_FILTER004(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、不同的回调函数callback1和callback2 
//@EXPECTATION: 停止或者释放其中一个filter(不同顺序)，不会影响另一个filter的正常接收数据
//@EXECUTIONFLOW: 以下1-10步骤测试先释放PAT_FILTER001的filter再释放PAT_FILTER004的filter
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate(PAT_FILTER001,callback1,userdata1,filter1)分配filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate(PAT_FILTER004,callback2,userdata2,filter2)分配filter2，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart启动filter1和filter2，期望启动成功
//@EXECUTIONFLOW: 5、校验得到的数据，期望callback1和callback2得到的数据分别为PAT_FILTER001、PAT_FILTER004指定的数据
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStop停止filter1
//@EXECUTIONFLOW: 7、校验得到的数据，期望callback1接收不到任何数据且callback2可以接收到数据
//@EXECUTIONFLOW: 8、调用CSUDIFILTERFree释放filter1，期望释放成功
//@EXECUTIONFLOW: 9、校验得到的数据，期望callback1接收不到任何数据且callback2可以接收到数据
//@EXECUTIONFLOW: 10、调用CSUDIFILTERFree释放filter2
//@EXECUTIONFLOW: 以下11-19步骤测试先释放PAT_FILTER004的filter再释放PAT_FILTER001的filter
//@EXECUTIONFLOW: 11、调用CSUDIFILTERAllocate(PAT_FILTER001,callback1,userdata1,filter1)分配filter1，期望分配成功
//@EXECUTIONFLOW: 12、调用CSUDIFILTERAllocate(PAT_FILTER004,callback2,userdata2,filter2)分配filter2，期望分配成功
//@EXECUTIONFLOW: 13、调用CSUDIFILTERStart启动filter1和filter2，期望启动成功
//@EXECUTIONFLOW: 14、校验得到的数据，期望callback1和callback2得到的数据分别为PAT_FILTER001、PAT_FILTER004指定的数据
//@EXECUTIONFLOW: 15、调用CSUDIFILTERStop停止filter2
//@EXECUTIONFLOW: 16、校验得到的数据，期望callback2接收不到任何数据且callback1可以接收到数据
//@EXECUTIONFLOW: 17、调用CSUDIFILTERFree释放filter2，期望释放成功
//@EXECUTIONFLOW: 18、校验得到的数据，期望callback2接收不到任何数据且callback1可以接收到数据
//@EXECUTIONFLOW: 19、调用CSUDIFILTERFree释放filter1
//@EXECUTIONFLOW: 11、重复1~19步骤10次
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 5 Filter1 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 5 Filter2 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 6 Stop filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
					"Step 7 Filter1 Expected No Data Failure !!!"
				);	
			//检验filter2是否获取数据
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_004,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 7 Filter2 Expected Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 8 Free filter1 Failure !!!"
				);
			hFilter1 = CSUDI_NULL;
			
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
					"Step 9 Filter1 Expected No Data Failure !!!"
				);	
			//检验filter2是否获取数据
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_004,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待1s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 14 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待1s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 14 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iStopFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 15 Stop filter1 Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
					"Step 16 Filter1 Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_FALSE)),
					"Step 16 Filter2 Expected No Data Failure !!!"
				);		
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter2,(void*)EM_SECTION_PAT_FILTER_004)),
					"Step 17 Free filter2 Failure !!!"
				);
			hFilter2 = CSUDI_NULL;
			
			//检验filter1是否获取数据
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_001,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 18 Filter1 Expected Data Failure !!!"
				);	

			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试申请两个有重叠的过滤条件且不同的回调函数的filter时，分别修改两个filter为对方的过滤条件
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001、PAT_FILTER004(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、不同的回调函数callback1和callback2 
//@EXPECTATION: 修改之后仍可以正常接收到数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate(PAT_FILTER001,callback1,userdata1,filter1)分配filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate(PAT_FILTER004,callback2,userdata2,filter2)分配filter2，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart启动filter1和filter2，期望启动成功
//@EXECUTIONFLOW: 5、校验得到的数据，期望callback1和callback2得到的数据都为PAT_FILTER001指定的数据(PAT_FILTER001和PAT_FILTER004指定的数据是一样的)
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStop停止filter1、filter2,期望成功
//@EXECUTIONFLOW: 7、调用CSUDIFILTERModify修改filter1为PAT_FILTER004过滤条件，期望成功
//@EXECUTIONFLOW: 8、调用CSUDIFILTERModify修改filter2为PAT_FILTER001过滤条件，期望成功
//@EXECUTIONFLOW: 9、调用CSUDIFILTERStart启动filter1和filter2，期望启动成功
//@EXECUTIONFLOW: 10、校验得到的数据，期望filter1得到的数据为PAT_FILTER004指定的数据、filter2得到的数据为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 11、调用CSUDIFILTERFree释放filter1，期望释放成功
//@EXECUTIONFLOW: 12、校验得到的数据，期望callback1接收不到任何数据且callback2可以接收到数据
//@EXECUTIONFLOW: 13、调用CSUDIFILTERFree释放filter2
//@EXECUTIONFLOW: 11、重复2~13步骤10次
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待1s*/
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 5 filter1 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待1s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*///filter2
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo1,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
					"Step 10 filter2 Check Expected Data Failure !!!"
				);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*///fiter1
					(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_004,CSUDI_TRUE)),
					"Step 10 filter1 Check Expected Data Failure !!!"
				);	
			CSTK_ASSERT_TRUE_FATAL(
					(CSUDI_TRUE == SECTION_iFreeFilter(hFilter1,(void*)EM_SECTION_PAT_FILTER_001)),
					"Step 11 Free filter1 Failure !!!"
				);	
			hFilter1 = CSUDI_NULL;
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*///fiter1
					(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo2,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_FALSE)),
					"Step 12 Filter1 Check Expected No Data Failure !!!"
				);	
			
			//检验filter2是否获取数据
			SECTION_iSetCallbackStatus(EM_SECTION_PAT_FILTER_004,CSUDI_FALSE,CSUDI_FALSE);
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*///fiter2
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试申请多个filter去过滤存在多个section_num的PID，操作其中一个应该不影响其它filter
//@DESCRIPTION: 例如存在两个连续的section_num:filter1不过滤section_num、filter2过滤section_num_0、filter3过滤section_num_1
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、EIT_FILTER001、EIT_FILTER002、EIT_FILTER003(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、不同的回调函数callback1和callback2 
//@EXPECTATION: 修改每个filter之后其它filter仍可以正常接收到数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate(EIT_FILTER001,callback1,userdata1,filter1)分配filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate(EIT_FILTER002,callback2,userdata2,filter2)分配filter2，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERAllocate(EIT_FILTER003,callback3,userdata3,filter3)分配filter3，期望分配成功
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStart启动filter1和filter2、filter3，期望启动成功
//@EXECUTIONFLOW: 6、校验得到的数据,期望在5s内收全数据:
//@EXECUTIONFLOW: (1)期望callback1可以得到EIT_FILTER001指定的数据；
//@EXECUTIONFLOW: (2)期望callback2可以得到EIT_FILTER002指定的数据；
//@EXECUTIONFLOW: (3)期望callback3可以得到EIT_FILTER003指定的数据；
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStop停止filter1、filter2,期望成功
//@EXECUTIONFLOW: 8、校验得到的数据，期望callback1和callback2得不到数据；callback3可以得到EIT_FILTER003指定的数据
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放filter1、filter2，期望释放成功
//@EXECUTIONFLOW: 10、校验得到的数据，期望callback1、callback2接收不到任何数据且callback3可以接收到数据
//@EXECUTIONFLOW: 11、调用CSUDIFILTERFree释放filter3
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试多个线程去请求同一PID的数据，每个线程中通过modify来切换filter
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001~PAT_FILTER006(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT: 3、测试demux 0 与 tuner 0
//@EXPECTATION: 每个线程按预期获取或未获取数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、创建线程1-3进行测试
//@EXECUTIONFLOW: 3、等待所有已启动的线程测试结束或者超时，如果所有线程测试成功则用例成功，否则失败
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: 线程1执行如下过程:
//@EXECUTIONFLOW: 1.1、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter1，期望分配成功
//@EXECUTIONFLOW: 1.2、调用CSUDIFILTERStart启动filter1，期望启动成功
//@EXECUTIONFLOW: 1.3、校验得到的数据，期望为PAT_FILTER001
//@EXECUTIONFLOW: 1.4、调用CSUDIFILTERStop停止filter1
//@EXECUTIONFLOW: 1.5、调用CSUDIFILTERModify修改filter1为PAT_FILTER002
//@EXECUTIONFLOW: 1.6、调用CSUDIFILTERStart启动filter1，期望启动成功
//@EXECUTIONFLOW: 1.7、校验得到的数据，期望为PAT_FILTER002指定的数据
//@EXECUTIONFLOW: 1.8、调用CSUDIFILTERFree释放filter1，期望返回成功
//@EXECUTIONFLOW: 1.9、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW:线程2执行如下过程:
//@EXECUTIONFLOW: 2.1、调用CSUDIFILTERAllocate分配PAT_FILTER003指定的filter2，期望分配成功
//@EXECUTIONFLOW: 2.2、调用CSUDIFILTERStart启动filter2，期望启动成功
//@EXECUTIONFLOW: 2.3、校验PAT_FILTER003的数据，期望无任何数据
//@EXECUTIONFLOW: 2.4、调用CSUDIFILTERStop停止filter2
//@EXECUTIONFLOW: 2.5、调用CSUDIFILTERModify修改filter2为PAT_FILTER004
//@EXECUTIONFLOW: 2.6、调用CSUDIFILTERStart启动filter2，期望启动成功
//@EXECUTIONFLOW: 2.7、校验得到的数据，期望为PAT_FILTER004指定的数据
//@EXECUTIONFLOW: 2.8、调用CSUDIFILTERFree释放filter2，期望返回成功
//@EXECUTIONFLOW: 2.9、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW:线程3执行如下过程:
//@EXECUTIONFLOW: 3.1、调用CSUDIFILTERAllocate分配PAT_FILTER005指定的filter3，期望分配成功
//@EXECUTIONFLOW: 3.2、调用CSUDIFILTERStart启动filter3，期望启动成功
//@EXECUTIONFLOW: 3.3、校验得到的数据，期望为PAT_FILTER005指定的数据
//@EXECUTIONFLOW: 3.4、调用CSUDIFILTERStop停止filter3
//@EXECUTIONFLOW: 3.5、调用CSUDIFILTERModify修改filter3为PAT_FILTER006
//@EXECUTIONFLOW: 3.6、调用CSUDIFILTERStart启动filter3，期望启动成功
//@EXECUTIONFLOW: 3.7、校验数据，期望为PAT_FILTER006指定的数据
//@EXECUTIONFLOW: 3.8、调用CSUDIFILTERFree释放filter3，期望返回成功
//@EXECUTIONFLOW: 3.9、重复以上过程5次
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试多个线程去请求不同PID的数据
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001、CAT_FILTER001、EIT_FILTER001、TDT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT: 3、测试demux 0 与 tuner 0
//@EXPECTATION: 每个线程按预期获取数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、创建线程1-4进行测试
//@EXECUTIONFLOW: 3、等待所有已启动的线程测试结束或者超时，如果所有线程测试成功则用例成功，否则失败
//@EXECUTIONFLOW: 线程1执行如下过程:
//@EXECUTIONFLOW: 1.1、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter1，期望分配成功
//@EXECUTIONFLOW: 1.2、调用CSUDIFILTERStart启动filter1，期望启动成功
//@EXECUTIONFLOW: 1.3、校验得到的数据，期望为PAT_FILTER001，不出现丢包
//@EXECUTIONFLOW: 1.4、调用CSUDIFILTERFree释放filter1，期望返回成功
//@EXECUTIONFLOW: 1.5、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: 线程2执行如下过程:
//@EXECUTIONFLOW: 2.1、调用CSUDIFILTERAllocate分配CAT_FILTER001指定的filter2，期望分配成功
//@EXECUTIONFLOW: 2.2、调用CSUDIFILTERStart启动filter2，期望启动成功
//@EXECUTIONFLOW: 2.3、校验得到的数据，期望为CAT_FILTER001，不出现丢包
//@EXECUTIONFLOW: 2.4、调用CSUDIFILTERFree释放filter2，期望返回成功
//@EXECUTIONFLOW: 2.5、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: 线程3执行如下过程:
//@EXECUTIONFLOW: 3.1、调用CSUDIFILTERAllocate分配EIT_FILTER001指定的filter3，期望分配成功
//@EXECUTIONFLOW: 3.2、调用CSUDIFILTERStart启动filter3，期望启动成功
//@EXECUTIONFLOW: 3.3、校验得到的数据，期望为EIT_FILTER001，不出现丢包
//@EXECUTIONFLOW: 3.4、调用CSUDIFILTERFree释放filter3，期望返回成功
//@EXECUTIONFLOW: 3.5、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: 线程4执行如下过程:
//@EXECUTIONFLOW: 4.1、调用CSUDIFILTERAllocate分配TDT_FILTER001指定的filter4，期望分配成功
//@EXECUTIONFLOW: 4.2、调用CSUDIFILTERStart启动filter4，期望启动成功
//@EXECUTIONFLOW: 4.3、校验得到的数据，期望为TDT_FILTER001，不出现丢包
//@EXECUTIONFLOW: 4.4、调用CSUDIFILTERFree释放filter4，期望返回成功
//@EXECUTIONFLOW: 4.5、重复以上过程5次
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
	CSTCPrint("正在申请数据...请等待!!!");
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

	case EPG_CURSERVICE_SIMPLE_PRESENT_FINISHED: //当前流的PresentEPG搜到
	case EPG_CURSERVICE_SIMPLE_PRESENT_CHANGED:  //当前流的PresentEPG改变
		{
			nTempTime = CSUDIPlusOSGetTickCount();
			CSTCPrint( "[TestEPGCallBack]EPG_CURSERVICE_SIMPLE_PRESENT_CHANGED at %d\n",nTempTime - s_nEPGRecStartTime);
		}
		break;

	case EPG_CURSERVICE_SIMPLE_FOLLOW_FINISHED:  //当前流的FollowEPG搜到
	case EPG_CURSERVICE_SIMPLE_FOLLOW_CHANGED:   //当前流的FollowEPG改变
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

	case EPG_CURSERVICE_SCHEDLE_FINISHED:        //一个Service的ScheduleEPG搜全   
		{
			nTempTime = CSUDIPlusOSGetTickCount();
			CSTCPrint( "[TestEPGCallBack]EPG_CURSERVICE_SCHEDLE_FINISHED at %d \n", nTempTime - s_nEPGRecStartTime);
		}
		break;

	case EPG_ALLSERVICE_SCHEDLE_FINISHED:        //请求的所有ScheduleEPG搜全
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

	case EPG_MEMORY_FULL:                        //内存不足
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试请求大数据量的情况,获取网络级别的数据(包括pf和schedule数据)
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: 1、EPG搜索策略为网络级别
//@INPUT: 2、EPG存储策略为全部保存
//@INPUT: 3、内存池为7M
//@EXPECTATION: 每个步骤都可以得到期望的结果
//@REMARK:本用例依赖DTV模块
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、初始SectionSource资源句柄，创建EPG内存池
//@EXECUTIONFLOW: 3、调用CreateEPG初始化EPG资源句柄
//@EXECUTIONFLOW: 4、设置EPG搜索策略为网络级别搜索
//@EXECUTIONFLOW: 5、启动EPG搜素， 等待两分钟，期望在两分钟内接收完数据
//@EXECUTIONFLOW: 6、接收完成后，记录接收数据所需的时间
//@EXECUTIONFLOW: 7、重复10次步骤3-6，所有步骤执行完后，计算总共接收的时间，要求平均时间少于20秒
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

		//网络级别的搜素
		StartRecEPG(hEPG, EPG_TEST_ORIGNETWORKID,EPG_TEST_TSID, EPG_TEST_SERVID);
		CSTCPrint("[SECTION]等待:%d s \r\n",120);
		
		CSUDIOSEventWait(s_hEPGCallbackEvent, 120000);
		CSUDIOSEventReset(s_hEPGCallbackEvent);

		bRet = s_bRecAllEIT;
		if(!bRet)
		{
			CSTCPrint("[SECTION]第 %d 次接收数据失败",10-i-1);
			CSTK_ASSERT_TRUE(bRet,"未申请到数据 !!!");
		}
		
		bRet = StopRecEPG(hEPG);
		CSTK_ASSERT_TRUE(bRet,"StopRecEPG 失败\n");

		bRet = DeleteAllEPG(hEPG);
		CSTK_ASSERT_TRUE(bRet,"DeleteAllEPG 失败\n");

		bRet = FreeEPG( hEPG);
		CSTK_ASSERT_TRUE(bRet,"FreeEPG 失败\n");

		hEPG = NULL;
		
		CSTCPrint("[SECTION]还有%d次,本循环即可结束,请耐心等待\r\n",10-i-1);
		CSUDIOSThreadSleep(1000);
	}

	nAveRecTime = s_nRecAllTime/i;
	CSTCPrint( "[SECTION]ave time is  %d\n",nAveRecTime);
	if(bRet)
	{

		CSTCPrint(": 最短接收时间%d秒!\n",s_nRecMinTime/1000);
		CSTCPrint(": 最长接收时间%d秒!\n",s_nRecMaxTime/1000);
		if( nAveRecTime < 20000 )
		{
			bRet = CSUDI_TRUE;
			CSTCPrint(": 平均时间%d秒，测试成功!\n",nAveRecTime/1000);
		}
		else
		{
			bRet = CSUDI_FALSE;
			CSTCPrint(": 平均时间%d秒,不在发送周期20秒的误差范围内,测试失败\n",nAveRecTime/1000);
			CSTK_ASSERT_TRUE_FATAL(0,"不在发送周期20秒的误差范围内,测试失败");
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
//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION:测试同一PID下多filter过滤,操作某个filter是否影响其他filter的情况
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: OC_FILTER001、OC_FILTER002、OC_FILTER003、OC_FILTER004(请参考CSTC_SECTION_IT_Allocate_0000)
//@EXPECTATION: 不丢包，能在一轮将数据全部接收
//@EXECUTIONFLOW: 1、锁定oc_test.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配OC_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动的filter，期望启动成功
//@EXECUTIONFLOW: 4、搜索数据并且记录section序列，如果两轮都还没有搜索完则取消该filter
//@EXECUTIONFLOW: 5、检查是否搜索完全是否丢包
//@EXECUTIONFLOW: 6、重复步骤2到5，分配16个OC_FILTER001~OC_FILTER004指定的filter，期望每一步都成功
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
    int nInjectedLength = 0; //可注入的大小
    
	char * pcbuf = CSUDI_NULL;  //缓冲区地址
	unsigned int ulength = 0;  //缓冲区长度
	
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
		CSTCPrint("打开文件失败，请确定在testdata目录下存在 %s 文件 !!!\n", psInjectParam->pTsFile);
		return;
	}
	if(CSUDIFSSeek(hTSFile,0,CSUDIFS_SEEK_END) != 0)
	{
		CSTCPrint("CSUDIFSSeek failuer !!!\n");
		return;
	}
	
	lFileLen = CSUDIFSTell (hTSFile);

    CSUDIFSSeek(hTSFile,0,CSUDIFS_SEEK_SET);
     
	while(1) //循环读取文件
	{
		///求出当前剩余文件的长度
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
            if(nReadLength < ulength) //剩余文件长度小于可注入空间的长度
            {
                nInjectedLength = nReadLength;
            }
            else  //剩余文件长度大于等于可注入空间的长度
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
            return;    //由用例主动终止线程
		}
	}
}

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试demux数据来源为TS注入时的情况,测试Filter为不同深度时的PAT数据请求(filter深度分别为1、3、4、6)
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、存在支持TS数据注入的demux
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001~PAT_FILTER004(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@REMARK:依赖INJECTER模块
//@EXPECTATION: 每一步骤都能够成功获取指定的数据
//@EXECUTIONFLOW: 1、调用CSUDIDEMUXGetCount获取demux个数,期望不为0
//@EXECUTIONFLOW: 2、调用CSUDIDEMUXGetCapability获取支持数据注入的demuxID，期望存在
//@EXECUTIONFLOW: 3、调用CSUDIINJECTEROpen创建一个TS注入器，期望成功
//@EXECUTIONFLOW: 4、调用CSUDIINJECTERStart启动注入器开始数据注入，期望成功
//@EXECUTIONFLOW: 5、创建一个读取EPG_0606_121458_TS文件的线程开始注入数据,期望成功
//@EXECUTIONFLOW: 6、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter1，期望分配成功
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 8、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 9、调用CSUDIFILTERAllocate分配PAT_FILTER002指定的filter2，期望分配成功
//@EXECUTIONFLOW: 10、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 11、校验得到的数据，期望得到PAT_FILTER002指定的数据
//@EXECUTIONFLOW: 12、调用CSUDIFILTERAllocate分配PAT_FILTER003指定的filter3，期望分配成功
//@EXECUTIONFLOW: 13、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 14、校验得到的数据，期望得不到任何数据
//@EXECUTIONFLOW: 15、调用CSUDIFILTERAllocate分配PAT_FILTER004指定的filter4，期望分配成功
//@EXECUTIONFLOW: 16、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 17、校验得到的数据，期望得到PAT_FILTER004指定的数据
//@EXECUTIONFLOW: 18、释放所有filter
CSUDI_BOOL CSTC_SECTION_IT_0020(void)
{
	CSUDIINJECTERChnl_S sInjecterChnl;  //注入通道的资源数据
	CSUDIINJECTEROpenParam_S sOpenParams;
	SECTIONInjetParam_S sInjecParam;
	CSUDISECTIONRequestInfo_S sTmpReqInfo;
	CSUDIDEMUXCapability_S sTmpCap;
	CSUDI_HANDLE ahFilter[MAX_FILTER] = {CSUDI_NULL}; /*一般只有32个filter*/
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
		
		if((sTmpCap.m_dwWorkTypeMask & EM_UDI_DEMUX_INJECT) == EM_UDI_DEMUX_INJECT && sTmpCap.m_nFilterNum > 0) //支持数据注入
		{
			break;
		}
    }
	
	if(nDemuxIndex == nDemuxCount)
	{
		CSTCPrint("找不到有inject能力的demux设备或者支持inject功能的demux设备上不存在filter设备，此平台不支持该测试用例!!!");
		return CSUDI_TRUE;
	}
	
	CSTCPrint("Demux Id = %d support Ts inject\n",nDemuxIndex);
	
	sInjecterChnl.m_nDemux = nDemuxIndex; //有inject能力的demuxid
	sInjecterChnl.m_nAudioDecoder = -1;
	sInjecterChnl.m_nVideoDecoder = -1;

	sOpenParams.m_eContentType = EM_UDIINJECTER_CONTENT_DEFAULT;  
	sOpenParams.m_eInjecterType = EM_INJECTER_TS;  //ts数据
	
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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
			CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试多个线程去请求不同PID的PES数据
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、SUBTITLE_001、AUDIO_001、TELETEXT_001、AUDIO_002(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT: 3、测试demux 0 与 tuner 0
//@EXPECTATION: 每个线程按预期获取数据
//@EXECUTIONFLOW: 1、锁定Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts码流所在的频点
//@EXECUTIONFLOW: 2、创建线程1-4进行测试
//@EXECUTIONFLOW: 3、等待所有已启动的线程测试结束或者超时，如果所有线程测试成功则用例成功，否则失败
//@EXECUTIONFLOW: 线程1执行如下过程:
//@EXECUTIONFLOW: 1.1、调用CSUDIFILTERAllocate分配SUBTITLE_001指定的filter1，期望分配成功
//@EXECUTIONFLOW: 1.2、调用CSUDIFILTERStart启动filter1，期望启动成功
//@EXECUTIONFLOW: 1.3、校验得到的数据，期望为SUBTITLE_001，不出现丢包
//@EXECUTIONFLOW: 1.4、调用CSUDIFILTERFree释放filter1，期望返回成功
//@EXECUTIONFLOW: 1.5、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: 线程2执行如下过程:
//@EXECUTIONFLOW: 2.1、调用CSUDIFILTERAllocate分配TELETEXT_001指定的filter2，期望分配成功
//@EXECUTIONFLOW: 2.2、调用CSUDIFILTERStart启动filter2，期望启动成功
//@EXECUTIONFLOW: 2.3、校验得到的数据，期望为TELETEXT_001，不出现丢包
//@EXECUTIONFLOW: 2.4、调用CSUDIFILTERFree释放filter2，期望返回成功
//@EXECUTIONFLOW: 2.5、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: 线程3执行如下过程:
//@EXECUTIONFLOW: 3.1、调用CSUDIFILTERAllocate分配AUDIO_001指定的filter3，期望分配成功
//@EXECUTIONFLOW: 3.2、调用CSUDIFILTERStart启动filter3，期望启动成功
//@EXECUTIONFLOW: 3.3、校验得到的数据，期望为AUDIO_001，不出现丢包
//@EXECUTIONFLOW: 3.4、调用CSUDIFILTERFree释放filter3，期望返回成功
//@EXECUTIONFLOW: 3.5、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: 线程4执行如下过程:
//@EXECUTIONFLOW: 4.1、调用CSUDIFILTERAllocate分配AUDIO_002指定的filter4，期望分配成功
//@EXECUTIONFLOW: 4.2、调用CSUDIFILTERStart启动filter4，期望启动成功
//@EXECUTIONFLOW: 4.3、校验得到的数据，期望为AUDIO_002，不出现丢包
//@EXECUTIONFLOW: 4.4、调用CSUDIFILTERFree释放filter4，期望返回成功
//@EXECUTIONFLOW: 4.5、重复以上过程5次
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
	CSTCPrint("正在申请数据...请等待!!!");
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@CASEGROUP: 5、CSUDIFILTERModify
//@DESCRIPTION: 测试多个线程去请求同一PID的数据，每个线程中通过Free来切换filter
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PAT_FILTER001~PAT_FILTER006(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT: 3、测试demux 0 与 tuner 0
//@EXPECTATION: 每个线程按预期获取或未获取数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、创建线程1-3进行测试
//@EXECUTIONFLOW: 3、等待所有已启动的线程测试结束或者超时，如果所有线程测试成功则用例成功，否则失败
//@EXECUTIONFLOW:
//@EXECUTIONFLOW: 线程1执行如下过程:
//@EXECUTIONFLOW: 1.1、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter1，期望分配成功
//@EXECUTIONFLOW: 1.2、调用CSUDIFILTERStart启动filter1，期望启动成功
//@EXECUTIONFLOW: 1.3、校验得到的数据，期望为PAT_FILTER001
//@EXECUTIONFLOW: 1.4、调用CSUDIFILTERFree释放filter1
//@EXECUTIONFLOW: 1.5、调用CSUDIFILTERAllocate分配PAT_FILTER002指定的filter1，期望分配成功
//@EXECUTIONFLOW: 1.6、调用CSUDIFILTERStart启动filter1，期望启动成功
//@EXECUTIONFLOW: 1.7、校验得到的数据，期望为PAT_FILTER002指定的数据
//@EXECUTIONFLOW: 1.8、调用CSUDIFILTERFree释放filter1，期望返回成功
//@EXECUTIONFLOW: 1.9、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW:线程2执行如下过程:
//@EXECUTIONFLOW: 2.1、调用CSUDIFILTERAllocate分配PAT_FILTER003指定的filter2，期望分配成功
//@EXECUTIONFLOW: 2.2、调用CSUDIFILTERStart启动filter2，期望启动成功
//@EXECUTIONFLOW: 2.3、校验PAT_FILTER003的数据，期望无任何数据
//@EXECUTIONFLOW: 2.4、调用CSUDIFILTERFree释放filter2
//@EXECUTIONFLOW: 2.5、调用CSUDIFILTERAllocate分配PAT_FILTER004指定的filter2，期望分配成功
//@EXECUTIONFLOW: 2.6、调用CSUDIFILTERStart启动filter2，期望启动成功
//@EXECUTIONFLOW: 2.7、校验得到的数据，期望为PAT_FILTER004指定的数据
//@EXECUTIONFLOW: 2.8、调用CSUDIFILTERFree释放filter2，期望返回成功
//@EXECUTIONFLOW: 2.9、重复以上过程5次
//@EXECUTIONFLOW:
//@EXECUTIONFLOW:线程3执行如下过程:
//@EXECUTIONFLOW: 3.1、调用CSUDIFILTERAllocate分配PAT_FILTER005指定的filter3，期望分配成功
//@EXECUTIONFLOW: 3.2、调用CSUDIFILTERStart启动filter3，期望启动成功
//@EXECUTIONFLOW: 3.3、校验得到的数据，期望为PAT_FILTER005指定的数据
//@EXECUTIONFLOW: 3.4、调用CSUDIFILTERFree释放filter3
//@EXECUTIONFLOW: 3.5、调用CSUDIFILTERAllocate分配PAT_FILTER006指定的filter3，期望分配成功
//@EXECUTIONFLOW: 3.6、调用CSUDIFILTERStart启动filter3，期望启动成功
//@EXECUTIONFLOW: 3.7、校验数据，期望为PAT_FILTER006指定的数据
//@EXECUTIONFLOW: 3.8、调用CSUDIFILTERFree释放filter3，期望返回成功
//@EXECUTIONFLOW: 3.9、重复以上过程5次
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

//@CASEGROUP: 1、整个section模块
//@CASEGROUP: 2、CSUDIDEMUXConnectTuner,CSUDIDEMUXDisconnectTuner
//@DESCRIPTION: 测试demux与tuner的连接只影响数据来源不影响section的过滤条件
//@INPUT: 1、PAT_FILTER001
//@EXPECTATION: 1. 当demux与tuner连接的时候section接收正常
//@EXPECTATION: 2. 当demux和tuner连接断开的时候section不再接收到任何数据
//@EXPECTATION: 3.当demux与tuner恢复连接的时候,section应该恢复接收
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动filter，期望启动成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望得到数据
//@EXECUTIONFLOW: 5、调用CSUDIDEMUXDisconnectTuner断开demux和tuner0的连接,期望成功
//@EXECUTIONFLOW: 6、校验得到的数据，期望没有收到任何数据数据
//@EXECUTIONFLOW: 7、调用CSUDIDEMUXConnectTuner恢复demux0与tuner0的连接,期望数据恢复接收
//@EXECUTIONFLOW: 8、校验得到的数据，期望得到数据(此处不对数据进行校验)
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放filter
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
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"Step 3 Check Expected Data Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXDisconnectTuner(TUNER_0,DEMUX_0),
			" disconnect demux -> tuner Failure !!!");

	CSUDIOSThreadSleep(200);
	//clear statistic 
	SECTION_iSetCallbackStatus(nFilterIndex,CSUDI_FALSE,CSUDI_FALSE);
	
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
			(CSUDI_FALSE == SECTION_iGetCallbackStatus(CSUDI_FALSE,&sTmpReqInfo,(void*)EM_SECTION_PAT_FILTER_001,CSUDI_TRUE)),
			"expect no data received when disconnect demux -> tuner Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIDEMUXConnectTuner(TUNER_0,DEMUX_0),
		" connect demux -> tuner Failure !!!");

	CSUDIOSThreadSleep(200);
	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERSetBufSize
//@CASEGROUP: 3、CSUDIFILTERStart
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试接收OC数据的的时间
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、EM_SECTION_OC_FILTER001~EM_SECTION_OC_FILTER004(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT: 3、测试demux 0 与 tuner 0
//@REMARK:需要相应制作码流
//@EXPECTATION: 1、锁定oc_test.ts码流所在的频点
//@EXPECTATION: 2、调用CSUDIFILTERAllocate分配EM_SECTION_OC_FILTER001指定的filter，期望分配成功
//@EXPECTATION: 3、调用CSTKGetTickCount(&nStartTime)获取当前系统时间
//@EXPECTATION: 4、调用CSUDIFILTERStart开始请求EM_SECTION_OC_FILTER001指定的数据，期望启动成功
//@EXPECTATION: 5、校验得到的数据，看是否有丢包
//@EXPECTATION: 6、调用CSTKGetTickCount(&nEndTime)获取当前系统时间，通过2次系统时间间隔得出实际收包时间
//@EXPECTATION: 7、重复2~6步骤，分别请求EM_SECTION_OC_FILTER002数据，并获得其实际收包时间
//@EXPECTATION: 8、重复2~6步骤，分别请求EM_SECTION_OC_FILTER003数据，并获得其实际收包时间
//@EXPECTATION: 9、重复2~6步骤，分别请求EM_SECTION_OC_FILTER004数据，并获得其实际收包时间
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERSetBufSize
//@CASEGROUP: 3、CSUDIFILTERStart
//@CASEGROUP: 4、CSUDIFILTERFree
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、EM_SECTION_OC_FILTER001~EM_SECTION_OC_FILTER004(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT: 3、测试demux 0 与 tuner 0
//@INPUT: 1、EIT_FILTER004(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每个步骤都可以得到期望的结果
//@REMARK: 需要相应制作码流
//@EXPECTATION: 1、锁定oc_test.ts码流所在的频点
//@EXPECTATION: 2、调用CSUDIFILTERAllocate分配EM_SECTION_OC_FILTER001指定的filter，期望分配成功
//@EXPECTATION: 3、调用CSUDIFILTERStart开始请求EM_SECTION_OC_FILTER001指定的数据，期望启动成功
//@EXPECTATION: 4、调用带有打印的回调函数校验得到的数据，看是否有丢包
//@EXPECTATION: 5、重复2~4步骤，分别请求EM_SECTION_OC_FILTER002，EM_SECTION_OC_FILTER003，EM_SECTION_OC_FILTER004数据看是否有丢包现象
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

//@CASEGROUP: 整个section模块
//@DESCRIPTION: 测试多线程接收不同PID的section,并且每个section接收不停的进行释放再接收的操作,
//@DESCRIPTION:其中CAT表监测版本号变化,版本号变化的时候设置filter的ngate字段,从而进行版本变化监控
//@INPUT: 1、EPG_0606_121458.ts码流
//@INPUT: 2、pid=0x0的PAT表, pid=0x14 的TDT表,pid=0x101的PMT表,pid=0x1的CAT表
//@EXPECTATION:1.section的申请释放不相互影响,每个过滤条件都能接收到正确的数据,不存在乱抛表的情况
//@EXPECTATION:2.每个过滤条件接收到的包是否正确的判断标准是回调的长度和section中包含的长度一致
//@EXPECTATION:@ 以及第一个字节的tableid必须正确
//@EXPECTATION: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXPECTATION: 2、调用CSUDIDEMUXGetCapability获取demux0的能力,期望其filter个数必须大于0
//@EXPECTATION: 3、调用CSUDIOSSemCreate创建CAT表接收线程的通知信号量,期望创建成功
//@EXPECTATION: 4、调用CSUDIOSThreadCreate依次创建PAT TDT PMT 表的接收线程,接收线程的处理流程如下:
//@EXPECTATION: 4.1   调用CSUDIFILTERAllocate分配PID对应的过滤器,过滤条件全为0,期望返回CSUDI_SUCCESS
//@EXPECTATION: 4.2	调用CSUDIFILTERStart启动申请的filter,期望返回CSUDI_SUCCESS
//@EXPECTATION: 4.3   判断该filter接收到的数据包是否已经超过3个,如果是则调用CSUDIFILTERFree释放该filter,期望返回CSUDI_SUCCESS
//@EXPECTATION: 4.4   sleep 100ms,判断是否测试结束,没有的话重复步骤4.1到4.4
//@EXPECTATION: 5、调用CSUDIOSThreadCreate创建CAT表的接收线程模拟版本号监控,详细步骤如下:
//@EXPECTATION: 5.1   调用CSUDIFILTERAllocate分配CAT表的filter,过滤条件全为match[3] = 0x2<<1,mask[3] =0x3E,期望返回CSUDI_SUCCESS
//@EXPECTATION: 5.2   调用CSUDIFILTERStart启动申请的filter,期望返回CSUDI_SUCCESS
//@EXPECTATION: 5.3   调用CSUDIOSSemWait等待CAT表接收到的信号,该信号在section的回调中收到包后释放
//@EXPECTATION: 5.4   接收到信号后,反转negate[3],如果原来是0x0,则反转成0xFF,释放申请的filter,重复步骤5.1到5.4
//@EXPECTATION:6、判断测试是否通过:1),所有线程申请的section接收到的包大于0. 2)接收得到的section没有出现错误
//@EXPECTATION:7、调用CSUDIOSThreadDestroy消耗测试线程
//@EXPECTATION:8、调用CSUDIOSSemDestroy消耗测试信号量
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
			/*模拟应用做一次数据拷贝*/
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

	//线程运转40次就主动跳出
	while((*(int*)pvParam) <= 40)
	{
		CSTCPrint("HaHa , I am still alive\n");
		CSUDIOSThreadSleep(500);
		(*(int*)pvParam)++; 
	}
}

//@CASEGROUP: 整个section模块
//@DESCRIPTION: 测试在接收section数据量较大时,section模块是否会一直占用CPU而不释放
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: 1、EPG_0606_121458.ts码流
//@INPUT: 2、请求整个EIT表和请求整个SDT表
//@INPUT: 3、EIT和SDT section回调中会做一次section的拷贝
//@EXPECTATION: 在进行较大数据量的section请求时,低优先级的任务也能获得CPU运行的机会
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIOSThreadCreate创建一个1*17的低优先级线程,该线程的执行步骤和作用如下:
//@EXECUTIONFLOW: 2.1   sleep 500ms 然后计数值加1,最后可以通过该计数值判断低优先级线程是否得到足够的CPU
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate申请pid=0x12的EIT section,期望返回成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart启动该filter,期望返回成功
//@EXECUTIONFLOW: 5、调用CSUDIFILTERAllocate申请pid=0x11的SDT section,期望返回成功
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStart启动该filter,期望返回成功
//@EXECUTIONFLOW: 7、等待30s作为测试过程
//@EXECUTIONFLOW: 8、检查测试结果,如果EIT或者SDT表有接收到错误数据,或者创建的低优先级线程运行次数少于40次,则测试结果失败,否则成功
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放EIT 测试filter
//@EXECUTIONFLOW: 10、调用CSUDIFILTERFree释放SDT 测试filter
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

			/*数据可能已经乱掉了,不能直接去取extra table id*/
			if (nLength != pSection->m_nDataLen)
			{
				g_bSectionLenError0028 = TRUE;
				CSTCPrint("section length don't match length from callback\n");
				return ;
			}
	
			/*只取扩展table id的最低位,因为高位在这个码流中都为0*/
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
			/*模拟应用回调,做一次拷贝*/
			memcpy(g_pcSectionUserBuff,pSection->m_pucData,pSection->m_nDataLen);
			break;
		}
		default:
			break;
	}
}

/*descripition:GetFilter用于形成某个字节的过滤条件.需要过滤的字节位置
由nSecIndex指定.当nCnt = 5时表示形成搜索扩展tableid 0~4的表的过滤条件,
nCnt = 10时表示形成搜索扩展tableid 0~9的表的过滤条件
nCnt:需要过滤多少个表
nSecIndex:针对section中第几个字节
pnGetFilterCnt:共形成了多少个filter
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

	CSTCPrint("请在码流一轮播放开始时开始测试,否则码流回头时会错误判断丢包从而测试失败\n");
	CSTCPrint("按任意键开始测试\n");
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
	/*没有扩展table id为0的表*/
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

//@CASEGROUP: 整个section模块
//@DESCRIPTION: 测试demux的section数据接收性能,接收短包(128B)大数据量section
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、测试时必须从码流一轮播放开始,否则在码流回头时会错判丢包
//@INPUT: 1、coship_ads_section.ts码流,该码流的特点如下:
//@INPUT:(1) pid=6000 ,码率为8Mbps,包含两个tableid 分别为0x2b和0x3c,其中0x3c的表占98%,tableid=0x3c
//@INPUT:的section又分为38(0x1~0x26)个扩展tableid,即从0开始计数的section第3和4个字节,tableid=0x3c
//@INPUT:的section除了last section外，其余的包长都为128B,该流主要用于小包大数据量section接收测试
//@INPUT:(2) pid=6001,码率为8Mbps,包含两个tableid 分别为0x2b和0x3c,其中0x3c的表占98%,tableid=0x3c
//@INPUT:的section又分为2(0x1~0x2)个扩展tableid,即从0开始计数的section第3和4个字节,tableid=0x3c
//@INPUT:的section除了last section外，其余的包长都为4KB,该流主要用于大包大数据量section接收测试
//@INPUT:(3) pid=6003 和pid=6001的流其他属性一致,但是pid=6003的流包含CRC，且每个包的CRC值都是错误的
//@INPUT:该流用于CRC功能测试
//@EXPECTATION: 接收短包大数据量section的时候demux应该满足一定的性能要求
//@EXECUTIONFLOW: 1、锁定coship_ads_section.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIOSMalloc 分配用于保存section数据的临时buffer,该buffer用于section的回调中模拟一次数据拷贝
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate申请pid=6000的section，只接收tableid=0x3c的section
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart启动该filter,期望返回成功
//@EXECUTIONFLOW: 5、sleep 15s 
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放所有测试filter
//@EXECUTIONFLOW: 7、判断测试结果,测试通过的判断依据为:
//@EXECUTIONFLOW: 1)每个扩展tableid的section接收数据包数大于0
//@EXECUTIONFLOW: 2)每个section返回的长度和seciton长度字段的长度匹配
//@EXECUTIONFLOW: 3)没有丢包现象,即接收到的包的section number字段是连续的
CSUDI_BOOL CSTC_SECTION_IT_0028(void)
{
	return CSUDISECTION_IT_ads(6000,38);
}

//@CASEGROUP: 整个section模块
//@DESCRIPTION: 测试demux的section数据接收性能,接收长包(4KB)大数据量section
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、测试时必须从码流一轮播放开始,否则在码流回头时会错判丢包
//@INPUT: 1、coship_ads_section.ts码流,该码流的信息请参考CSTC_SECTION_IT_0028
//@INPUT: 2、pid=6001,tableid=0x3c
//@EXPECTATION: 接收长包大数据量section的时候demux应该满足一定的性能要求
//@EXECUTIONFLOW:请参照CSTC_SECTION_IT_0028的测试步骤
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

			//对数据的第dataLen-1字节操作，要求无异常
 			pstTestInfo->pTmpBuf[pSection->m_nDataLen-1]++;
			
			pstTestInfo->unPktNum = unPacketNum;
			pstTestInfo->unPktCount++;
			break;
		default:
			break;
	}
}

//@CASEGROUP: 整个section模块
//@DESCRIPTION: 测试平台PES接收性能,使用码流为coship 自制码流
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取PES数据
//@PRECONDITION: 3、测试时必须从码流一轮播放开始,否则在码流回头时会错判丢包
//@INPUT: PES_performance_test.ts,该码流里面有两个测试PES分别为:
//@INPUT:1. 每个包为5K的流0x1df0,码率为10Mbps(有少量包长不足5k)
//@INPUT:2. 每个包为32K的流0x1def,码率为15Mbps(有少量包长不足32k)
//@INPUT:3. 每个PES包的编号获取方式为(((pes)[11] << 24) | ((pes)[12] << 16) | ((pes)[13] << 8) | ((pes)[14]))
//@EXPECTATION: 两个测试流都能正常的接收PES，且没有丢包
//@EXECUTIONFLOW: 1、锁定PES_performance_test.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配流0x1df0指定的filter，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart开始请求指定的PES数据
//@EXECUTIONFLOW: 5、每隔1s检查是否有测试失败情况,一共检测30s,是否测试是否失败在PES回调中进行,步骤如下:
//@EXECUTIONFLOW: 5.1   检查包的是否以PES标志001开头,不是则测试失败
//@EXECUTIONFLOW: 5.2   检查包序列是否连续,应该为0,1,2,3,4.......
//@EXECUTIONFLOW: 5.3   将收到的PES拷贝到一个临时buffer模拟应用在回调中的操作
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 7、将测试PID更改成15Mbps的测试流0x1def,重复步骤2~6
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

	CSTCPrint("请设置码流的播出码率为24M!\n");
	CSTCPrint("请在码流一轮播放开始时开始测试,否则码流回头时会错误判断丢包从而测试失败\n");
	CSTCPrint("按任意键开始测试\n");
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试CRC错误的section接收情况
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: 1.一个CRC值全部错误的section码流
//@INPUT: 2.pid = 6003
//@INPUT: 3.match[0]=0x3c,mask[0]=0xff,match[2]=0x1,mask[2]=0xff
//@EXPECTATION:1.当CRCCheck = FALSE时,应该能收到section
//@EXPECTATION:2.当CRCCheck = TRUE时,应该不能收到section
//@EXECUTIONFLOW: 1、锁定coship_ads_section.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配filter,关闭CRC检查，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动filter，期望启动成功
//@EXECUTIONFLOW: 4、sleep 3s,然后检查接收数据状态,期望接收到的包数大于0
//@EXECUTIONFLOW: 5、调用CSUDIFILTERFree释放filter,期望返回成功
//@EXECUTIONFLOW: 6、打开CRC检查,再调用CSUDIFILTERAllocate分配filter,期望分配成功
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStart启动filter，期望启动成功
//@EXECUTIONFLOW: 8、sleep 3s,然后检查接收数据状态,期望接收到的包数为0
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放filter,期望返回成功
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试filter有5个字节深度的过滤，且正反过滤并用的情况
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PMT_FILTER004(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION:每一步都得到期望值
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PMT_FILTER004指定的filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PMT_FILTER004指定的数据
//@EXECUTIONFLOW: 4、校验得到的数据，PMT_FILTER004的数据获取成功。 
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStop停止filter1，期望成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放filter2
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

	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试filter有5层过滤，且除TableID,全部为反向过滤的情况
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PMT_FILTER005(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION:每一步都得到期望值
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PMT_FILTER005指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PMT_FILTER005指定的数据
//@EXECUTIONFLOW: 4、校验得到的数据，PMT_FILTER005的数据获取成功。
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStop停止filter，期望成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放filter
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

	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试过滤条件的深度为15个字节时请求数据(正反过滤)
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PMT_FILTER006(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION:每一步都得到期望值
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PMT_FILTER006指定的filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PMT_FILTER006指定的数据
//@EXECUTIONFLOW: 4、校验得到的数据，PMT_FILTER006的数据获取成功。且获取的数据正确。
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStop停止filter1，期望成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放filter1
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

	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试同时多次用不同的filter请求tableID 相同PID不同的表的情况,回调函数相同
//@DESCRIPTION: 例如:多个PMT 表，根据table_ext过滤请求
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PMT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 1、PMT_FILTER002(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 1、PMT_FILTER003(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION:每一步都得到期望值
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PMT_FILTER001指定的filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate分配PMT_FILTER002指定的filter2，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERAllocate分配PMT_FILTER003指定的filter3，期望分配成功
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStart开始请求PMT_FILTER001指定的数据
//@EXECUTIONFLOW: 6、调用CSUDIFILTERStart开始请求PMT_FILTER002指定的数据
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStart开始请求PMT_FILTER003指定的数据
//@EXECUTIONFLOW: 8、校验得到的数据，PMT_FILTER001,PMT_FILTER002,PMT_FILTER003的数据都获取成功。且获取的数据都正确，没有多余的数据。
//@EXECUTIONFLOW: 9、调用CSUDIFILTERStop停止filter1，期望成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、调用CSUDIFILTERStop停止filter2，期望成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 11、调用CSUDIFILTERStop停止filter3，期望成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 12、调用CSUDIFILTERFree释放filter1
//@EXECUTIONFLOW: 13、调用CSUDIFILTERFree释放filter2
//@EXECUTIONFLOW: 14、调用CSUDIFILTERFree释放filter3
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试同时多次用不同的filter请求同tableID 同PID的表的情况,回调函数相同
//@DESCRIPTION: 例如:EIT的多个section，根据sectionNum过滤请求
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、EIT_FILTER002(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 1、EIT_FILTER003(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION:每一步都得到期望值
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配EIT_FILTER002指定的filter1，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERAllocate分配EIT_FILTER003指定的filter2，期望分配成功
//@EXECUTIONFLOW: 4、调用CSUDIFILTERStart开始请求PMT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStart开始请求PMT_FILTER002指定的数据
//@EXECUTIONFLOW: 6、校验得到的数据，EIT_FILTER002,EIT_FILTER003的数据都获取成功。且获取的数据都正确，没有多余的数据。
//@EXECUTIONFLOW: 7、调用CSUDIFILTERStop停止filter1，期望成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSUDIFILTERStop停止filter2，期望成功返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIFILTERFree释放filter1
//@EXECUTIONFLOW: 10、调用CSUDIFILTERFree释放filter2
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

	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
			(CSUDI_TRUE == SECTION_iGetCallbackStatus(CSUDI_TRUE,&sFilterReqInfo1,(void*)EM_SECTION_EIT_FILTER_002,CSUDI_TRUE)),
			"Step 6 Check Expected EM_SECTION_EIT_FILTER_002 Failure !!!"
		);

	CSTK_ASSERT_TRUE_FATAL(/*此处会等待2s*/
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERSetBufSize
//@CASEGROUP: 3、CSUDIFILTERFree
//@DESCRIPTION: 测试设置的bufsize超过底层支持的最大值的情况
//@PRECONDITION: 1、平台已成功初始化
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、PMT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT:CSUDIFILTERSetBufSize参数:
//@INPUT:1、409600
//@EXPECTATION:每一步都得到期望值
//@EXECUTIONFLOW: 1、调用CSUDIFILTERAllocate分配PMT_FILTER001指定的filter1，期望分配成功
//@EXECUTIONFLOW: 2、调用CSUDIFILTERSetBufSize设置buf大小，返回成功。
//@EXECUTIONFLOW: 3、调用CSUDIFILTERFree释放filter1
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERSetBufSize
//@CASEGROUP: 3、CSUDIFILTERFree
//@DESCRIPTION: 测试请求PAT表时如果版本号不发生变化第二次请求不到数据
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、CSUDIFILTERAllocate能成功分配filter
//@PRECONDITION: 4、CSUDIFILTERStart能成功启动一个filter
//@PRECONDITION: 5、CSUDIFILTERSop能成功停止一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、EIT_FILTER001(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 第一次请求成功，第二次请求不到数据
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER001指定的数据，并返回成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为PAT_FILTER001指定的数据
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStop停止PAT_FILTER001
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放filter
//@EXECUTIONFLOW: 7、设置版本号为关注字段，再次调用CSUDIFILTERAllocate分配PAT_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 8、调用CSUDIFILTERStart开始请求数据，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、校验得到的数据，期望为返回失败，申请不到数据
//@EXECUTIONFLOW: 10、调用CSUDIFILTERFree释放filter
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERSetBufSize
//@CASEGROUP: 3、CSUDIFILTERFree
//@DESCRIPTION: 测试设置buf大小为512K时，连续请求十五个表(平均每个表有50几个section),不会丢包
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、OC_FILTER001~OC_FILTER009(请参考CSTC_SECTION_IT_Allocate_0000)
//@INPUT: 2、合法的回调函数
//@INPUT:CSUDIFILTERSetBufSize参数:
//@INPUT:1、512K
//@EXPECTATION:不丢包，能在一轮将数据全部接收
//@EXECUTIONFLOW: 1、锁定oc_test.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配OC_FILTER001指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动的filter，期望启动成功
//@EXECUTIONFLOW: 4、搜索数据并且记录section序列，如果两轮都还没有搜索完则取消该filter
//@EXECUTIONFLOW: 5、检查是否搜索完全是否丢包
//@EXECUTIONFLOW: 6、重复步骤2到5，分配16个OC_FILTER001~OC_FILTER009指定的filter，期望每一步都成功
CSUDI_BOOL CSTC_SECTION_IT_0039(void)
{
	CSTCPrint("请设置码流的播出码率为24M!\n");
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

//@CASEGROUP: 1、CSUDIFILTERAllocate
//@CASEGROUP: 2、CSUDIFILTERStart
//@CASEGROUP: 3、CSUDIFILTERStop
//@CASEGROUP: 4、CSUDIFILTERFree
//@DESCRIPTION: 测试Filter同时请求teletext数据和PSI/SI数据
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、EM_SECTION_PSISI_PES_FILTER_PES,EM_SECTION_PSISI_PES_FILTER_PSISI(详见SECTION_Filter_E)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每一步得到期望值
//@EXECUTIONFLOW: 1、锁定Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配hFilter_pes和hFilter_psisi，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart启动hFilter_pes和hFilter_psisi，期望启动成功
//@EXECUTIONFLOW: 4、校验得到的数据，期望为EM_SECTION_PSISI_PES_FILTER_PES、EM_SECTION_PSISI_PES_FILTER_PSISI指定的数据匹配
//@EXECUTIONFLOW: 5、调用CSUDIFILTERStop停止hFilter_pes、hFilter_psisi
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放hFilter_pes、hFilter_psisi
CSUDI_BOOL CSTC_SECTION_IT_0040(void)
{
	CSUDISECTIONRequestInfo_S stReqInfo_pes,stReqInfo_psisi;
	CSUDI_HANDLE hFilter_pes = CSUDI_NULL;
	CSUDI_HANDLE hFilter_psisi = CSUDI_NULL;
	CSUDISECTIONCallbackData_S sParam;

	memset(&sParam, 0 ,sizeof(CSUDISECTIONCallbackData_S));
	
	CSTCPrint("请将%s码流播发码率设置为 38M \n", TELETEXT_SUBTITLE_25MBPS_071220_TS);

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

//@DESCRIPTION: 测试Filter回调中进行stop等操作会不会导致死锁
//@PRECONDITION: 1、平台已成功初始化
//@PRECONDITION: 2、demux可以成功连接tuner并获取数据
//@PRECONDITION: 3、CSUDIFILTERAllocate能成功分配filter
//@PRECONDITION: 4、CSUDIFILTERStart能成功启动一个filter
//@PRECONDITION: 5、CSUDIFILTERSop能成功停止一个filter
//@INPUT: CSUDIFILTERAllocate参数:
//@INPUT: 1、EM_SECTION_PSISI_PES_FILTER_PES,EM_SECTION_PSISI_PES_FILTER_PSISI(详见SECTION_Filter_E)
//@INPUT: 2、合法的回调函数
//@EXPECTATION: 每一步得到期望值
//@EXECUTIONFLOW: 1、锁定EPG_0606_121458.ts码流所在的频点
//@EXECUTIONFLOW: 2、调用CSUDIFILTERAllocate分配PAT_FILTER005指定的filter，期望分配成功
//@EXECUTIONFLOW: 3、调用CSUDIFILTERStart开始请求PAT_FILTER005指定的数据，并返回成功
//@EXECUTIONFLOW: 4、在回调函数中调用CSUDIFILTERStop操作，希望不会死锁
//@EXECUTIONFLOW: 5、如果，回调没有stop成功，调用CSUDIFILTERStop操作
//@EXECUTIONFLOW: 6、调用CSUDIFILTERFree释放filter
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


