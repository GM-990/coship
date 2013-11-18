
#ifndef _CS_TUNER_TEST_H_
#define _CS_TUNER_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_tuner.h"
#include "udi2_os.h"

//用于各线程之间注册删除回调传递参数
typedef struct
{
	CSUDI_UINT32 dwTunerId;
	CSUDITunerCallback  fnTunerCallback;
	void *pvUserData;
	CSUDI_BOOL bThreadFlag;
}TunerCallbackParam_S;

void CSTC_TUNER_InitCfg(void);

CSUDI_BOOL CSTC_TUNER_InfoCmp(CSUDITunerInfo_S sTunerInfo1,CSUDITunerInfo_S sTunerInfo2);

void CSTC_TUNER_Deliver_Init(CSUDITunerSRCDeliver_U unDeliver[], int nCount);

void CSTC_TUNER_TunerInfo_Init(CSUDITunerInfo_S asTunerInfo[], int nCount);

void CSTC_TUNER_TunerParam_Init(int *pnTunerCount, int *pnDeliverCount, CSUDI_BOOL *pbSupportRDITuner);

int CSTC_TUNER_GetDeliverCount(void);

void Tuner_TestThread1_1Entry(void *pvParam);
void Tuner_TestThread1_2Entry(void *pvParam);

void Tuner_TestThread2_1Entry(void *pvParam);
void Tuner_TestThread2_2Entry(void *pvParam);

void Tuner_TestThread3Entry(void *pvParam);

void Tuner_TestThread4_1Entry(void *pvParam);
void Tuner_TestThread4_2Entry(void *pvParam);

#ifdef __cplusplus
}
#endif

#endif/*_CS_TUNER_TEST_H_*/
