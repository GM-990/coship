#ifndef _CS_SECTION_TEST_H_
#define _CS_SECTION_TEST_H_

#include "udi2_typedef.h"
#include "udi2_section.h"
#include "udi2_os.h"
#include "udiplus_os.h"
#include "SectionInterface2C.h"
#include "MemoryPoolInterface.h"
#include "EPGReceiverInterface.h"
#include "EPGUserInterface.h"
#ifdef  __cplusplus
extern "C" {
#endif

#define DEMUX_0  (0)
#define TUNER_0  (0)
#define TESTTIMES (10)
#define DATA_LEN (16)
#define PES_LENGTH (12)
#define PSISI_LENGTH (3)

typedef enum {
	EM_SECTION_PAT_FILTER_001, // 0
	EM_SECTION_PAT_FILTER_002,
	EM_SECTION_PAT_FILTER_003,
	EM_SECTION_PAT_FILTER_004,
	EM_SECTION_PAT_FILTER_005,
	EM_SECTION_PAT_FILTER_006,
	EM_SECTION_PAT_FILTER_007,
	EM_SECTION_PAT_FILTER_008,
	EM_SECTION_PAT_FILTER_009,
	EM_SECTION_PAT_FILTER_010,
	EM_SECTION_PAT_FILTER_011,
	EM_SECTION_PAT_FILTER_012,
	EM_SECTION_PAT_FILTER_013,
	EM_SECTION_PAT_FILTER_014,
	EM_SECTION_PAT_FILTER_015,
	EM_SECTION_PAT_FILTER_MAX, // 15
	EM_SECTION_TDT_FILTER_001,// 16
	EM_SECTION_CAT_FILTER_001,// 17
	EM_SECTION_EIT_FILTER_001,// 18
	EM_SECTION_EIT_FILTER_002,
	EM_SECTION_EIT_FILTER_003,
	EM_SECTION_EIT_FILTER_004,
	EM_SECTION_EIT_FILTER_MAX,
	EM_SECTION_OC_FILTER001, // 23
	EM_SECTION_OC_FILTER002,
	EM_SECTION_OC_FILTER003,
	EM_SECTION_OC_FILTER004,
	EM_SECTION_OC_FILTER005, 
	EM_SECTION_OC_FILTER006,
	EM_SECTION_OC_FILTER007,
	EM_SECTION_OC_FILTER008, //30
	EM_SECTION_OC_FILTER009,
	EM_SECTION_OC_FILTER_MAX,
	EM_SECTION_PES_VIDEO_001, // 33
	EM_SECTION_PES_VIDEO_002,
	EM_SECTION_PES_AUDIO_001, // 35
	EM_SECTION_PES_AUDIO_002,
	EM_SECTION_PES_SUBTITLE_001,// 37
	EM_SECTION_PES_SUBTITLE_002,
	EM_SECTION_PES_TELETEXT_001, // 39
	EM_SECTION_PES_TELETEXT_002,
	EM_SECTION_PES_MAX,
	EM_SECTION_PMT_FILTER_001,// 42
	EM_SECTION_PMT_FILTER_002,
	EM_SECTION_PMT_FILTER_003,
	EM_SECTION_PMT_FILTER_004,
	EM_SECTION_PMT_FILTER_005,
	EM_SECTION_PMT_FILTER_006,
	EM_SECTION_PMT_FILTER_MAX,
	EM_SECTION_PSISI_PES_FILTER_PSISI, //49 EIT
	EM_SECTION_PSISI_PES_FILTER_PES, // SUBTITLE
	EM_SECTION_FILTER_MAX
}SECTION_Filter_E;

typedef enum{
	EM_PAT_PID = 0x00,
	EM_CAT_PID = 0x01,
	EM_EIT_PID = 0x12,
	EM_NIT_PID = 0x10,
	EM_TDT_PID = 0x14,
	EM_PMT_PRO1_PID = 0x465,
	EM_PMT_PRO2_PID = 0x15bd,
	EM_PMT_PRO3_PID = 0x15b3
}SECTION_PID_E;
/*
*�����ݴ�����Teltex.Subtitle.ClosedCapion_25Mbps_071220.ts�л�ȡ
*/
typedef enum{
	EM_VIDEO_001_PID = 0x15ae,
	EM_VIDEO_002_PID = 0x15b8,
	EM_AUDIO_001_PID = 0xa82,
	EM_AUDIO_002_PID = 0xa83,
	EM_SUBTITLE_001_PID = 0x401,
	EM_SUBTITLE_002_PID = 0x401, //not define
	EM_TELETEXT_001_PID = 0xa84,
	EM_TELETEXT_002_PID = 0xa84 //not define
}PES_PID_E;

#define SECTION_GOTO(expression, label, pErrorInfo)  \
{ \
	if(!(expression)) \
	{\
		CSTCPrint("[SECTION]Assertion: \"%s\" failed, in file %s, line %d.\r\n",#expression, __FILE__, __LINE__); \
		CSTCPrint("[SECTION]%s\r\n",pErrorInfo); \
		goto label;\
	}\
}




/*
* sectionģ����������ĳ�ʼ��
*/
void SECTION_iTestInit();
/*
* filter�Ļص��������˺������ж����ݵ���ȷ�ԣ���Ҫ��������CSTC_SECTION_IT_0039
����
*/
void SECTION_iCallback_3( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData);

/*
* filter�Ļص��������˺������ж����ݵ���ȷ��
*/
void SECTION_iCallback_1( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData);
/*
*filter�Ļص��������˺������ж����ݵ���ȷ��,��Ҫ��������CSTC_SECTION_IT_Allocate_0003����
*/
void SECTION_iCallback_UserDataNULL( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData);
//��Ҫ��������CSTC_SECTION_IT_Allocate_0003����
CSUDI_BOOL SECTION_iGetCallbackStatus_UserDataNULL( CSUDI_BOOL bCheck, const CSUDISECTIONRequestInfo_S * psRequestInfo, const void* pvUserData, CSUDI_BOOL bDebug);


void SECTION_iSetCallbackStatus( int nFilterIndex, CSUDI_BOOL bSuc ,CSUDI_BOOL bReq );
/*
*��ȡfilter�ص�������״̬������������ݣ������ж���ȷ�򷵻�CSUDI_TRUE�����򷵻�CSUDI_FALSE
*�����2s�ڻ�ȡ���������򷵻�ʧ�ܣ�bDebugΪCSUDI_TRUE,���ӡ�������ֵ
*/
CSUDI_BOOL SECTION_iGetCallbackStatus( CSUDI_BOOL bCheck, const CSUDISECTIONRequestInfo_S * psRequestInfo, const void* pvUserData, CSUDI_BOOL bDebug);
/*
* ����filter��ֵ
*/
CSUDI_BOOL SECTION_iSetMatchMask(CSUDIFILTERMatchMask_S * psMatch, int nFilterIndex);

/*
* ��������section���ݵ�filter
* ����psReqInfoΪCSUDI_NULL����ʱֻ������g_nDMXFilterIndex��ֵ
*/
CSUDI_BOOL SECTION_iSetSectionFilter(CSUDISECTIONRequestInfo_S * psReqInfo,int nFilterIndex ,int nDemuxIndex,int nPid);
/*
*��������PES���ݵ�requestion
*/
CSUDI_BOOL SECTION_iSetPESReq(CSUDISECTIONRequestInfo_S * psReqInfo,int nFilterIndex ,int nDemuxIndex,int nPid);

/*
* ����һ��filter�������ûص�����״ֵ̬
* ��Ҫ���ڲ��Թ�עfilter��ȡ������ֵ���������������CSTC_SECTION_IT_Allocate_0003
*/
CSUDI_BOOL SECTION_iAllocFilter(const CSUDISECTIONRequestInfo_S * psRequestInfo, const void* pvUserData,CSUDI_HANDLE *phFilter);
/*
*����һ��filter�������ûص�����״ֵ̬
*/
CSUDI_BOOL SECTION_iStartFilter(CSUDI_HANDLE hFilter,const void* pvUserData);
/*
* ֹͣһ��filter�������ûص�����״ֵ̬
*/
CSUDI_BOOL SECTION_iStopFilter(CSUDI_HANDLE hFilter,const void* pvUserData);
/*
* �޸�һ��filter�������ûص�����״ֵ̬
*/
CSUDI_BOOL SECTION_iModifyFilter(CSUDI_HANDLE hFilter,const CSUDIFILTERMatchMask_S *psMatchFilter,CSUDI_BOOL bCRCCheck,const void* pvUserData);

/*
* �ͷ�һ��filter�������ûص�����״ֵ̬
*/
CSUDI_BOOL SECTION_iFreeFilter(CSUDI_HANDLE hFilter,const void* pvUserData);

CSUDI_BOOL SECTION_iGetPowerCallBackStatus();
CSUDI_BOOL SECTION_iGetRequestCallBackStatus();

HCSHANDLE SECTION_iUSPSectionInit(void);

void SECTION_iInitPowerCasePar();
void SECTION_iInitRequestCasePar();
void SECTION_IT_0016_Filter_Thread1(void * pvArg);
void SECTION_IT_0016_Filter_Thread2(void * pvArg);
void SECTION_IT_0016_Filter_Thread3(void * pvArg);

void SECTION_IT_0022_Filter_Thread1(void * pvArg);
void SECTION_IT_0022_Filter_Thread2(void * pvArg);
void SECTION_IT_0022_Filter_Thread3(void * pvArg);
void ThreadWithFilterNum(void * pvArg);

CSUDI_BOOL SECTION_iTest_IT_20(void);

CSUDI_BOOL SECTION_iTest_IT_21(int nFilterIndex);

CSUDI_BOOL SECTION_iTest_IT_22(int nFilterNum);

CSUDI_BOOL SECTION_iTest_IT_39(void);

#ifdef  __cplusplus
}
#endif

/** @} */
#endif

