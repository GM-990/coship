
#ifndef _CS_OSUDI2_TEST_H_
#define _CS_OSUDI2_TEST_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_os.h"
#include "udi2_typedef.h"
#include "cs_testkit.h"

CSUDI_BOOL CSTC_OS_InitCfg(void);
void OS_TestThread1Entry(void *pvParam);
void OS_TestThread2Entry(void *pvParam);
void OS_TestThread3_1Entry(void *pvParam);
void OS_TestThread3_2Entry(void *pvParam);
void OS_TestThread3_3Entry(void *pvParam);
void OS_TestThread4Entry(void *pvParam);
void OS_TestThread5_1Entry(void *pvParam);
void OS_TestThread5_2Entry(void *pvParam);
void OS_TestThread6Entry(void *pvParam);
void OS_TestThread7Entry(void *pvParam);
void OS_TestThread8_1Entry(void *pvParam);
void OS_TestThread8_2Entry(void *pvParam);
void OS_TestMsgQThread1Entry(void *pvParam);
void OS_TestMsgQThread2Entry(void *pvParam);
void OS_TestMsgQThread3Entry(void *pvParam);
void OS_TestMsgQThread4Entry(void *pvParam);
void OS_TestSemThreadEntry(void *pvParam);
void OS_TestSemThread1Entry(void *pvParam);
void OS_TestMutexThread1Entry(void *pvParam);
void OS_TestMutexThread2_1Entry(void *pvParam);
void OS_TestMutexThread2_2Entry(void *pvParam);
void OS_TestMutexThread2_3Entry(void *pvParam);
void OS_TestMutexThread3Entry(void *pvParam);
void OS_TestMutexThread4Entry(void *pvParam);
void OS_TestMutexThread5Entry(void *pvParam);
void OS_TestEventThread1Entry(void *pvParam);
void OS_TestEventThread2Entry(void *pvParam);
void OS_TestEventThread3Entry(void *pvParam);
void OS_TestEventThread4Entry(void *pvParam);
void OS_TestThread1_1Entry(void *pvParam);

#ifdef __cplusplus
}
#endif

#endif/*_CS_OSUDI2_TEST_H_*/

