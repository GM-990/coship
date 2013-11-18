#ifndef _CS_OSUDI2_TEST_CASE_H_
#define _CS_OSUDI2_TEST_CASE_H_

#include "cs_testkit.h"
#include "udi2_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_CS_OSUDI2_Init(void);
CSUDI_BOOL CSTC_CS_OSUDI2_UnInit(void);

CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0001( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0002( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0003( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0004( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0005( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0006( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0007( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0008( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0009( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0010( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0011( void );
CSUDI_BOOL CSTC_OS_IT_ThreadCreate_0012( void );
CSUDI_BOOL CSTC_OS_IT_ThreadDestroy_0001( void );
CSUDI_BOOL CSTC_OS_IT_ThreadDestroy_0002(void);
CSUDI_BOOL CSTC_OS_IT_ThreadCreateDestroy_0001( void );
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0001( void );
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0002( void );
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0003( void );
CSUDI_BOOL CSTC_OS_IT_ThreadSuspend_0004( void );
CSUDI_BOOL CSTC_OS_IT_ThreadResume_0001( void );
CSUDI_BOOL CSTC_OS_IT_ThreadResume_0002( void );
CSUDI_BOOL CSTC_OS_IT_ThreadResume_0003( void );
CSUDI_BOOL CSTC_OS_IT_ThreadJoin_0001( void );
CSUDI_BOOL CSTC_OS_IT_ThreadJoin_0002( void );
CSUDI_BOOL CSTC_OS_IT_ThreadSelf_0001( void );
CSUDI_BOOL CSTC_OS_IT_ThreadSelfJoin_0001( void );
CSUDI_BOOL CSTC_OS_IT_ThreadSleep_0001( void );
CSUDI_BOOL CSTC_OS_IT_ThreadYield_0001( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0001( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0002( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0003( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0004( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0005( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0006( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0007( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0008( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueCreate_0009( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueDestroy_0001( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0001( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0002( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0003( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0004( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0005( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0006( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueSend_0007( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0001( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0002( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0003( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0004( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0005( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueueReceive_0006( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0001( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0002( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0003( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0004( void );
CSUDI_BOOL CSTC_OS_IT_MsgQueue_0005( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0001( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0002( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0003( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0004( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0005( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0006( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0007( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0008( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0009( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0010( void );
CSUDI_BOOL CSTC_OS_IT_SemCreate_0011( void );
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0001( void );
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0002( void );
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0003( void );
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0004( void );
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0005( void );
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0006( void );
CSUDI_BOOL CSTC_OS_IT_SemWaitRelease_0007( void );
CSUDI_BOOL CSTC_OS_IT_Sem_0001( void );
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0001( void );
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0002( void );
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0003( void );
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0004( void );
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0005( void );
CSUDI_BOOL CSTC_OS_IT_MutexCreateDestroy_0006( void );
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0001( void );
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0002( void );
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0003( void );
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0004( void );
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0005( void );
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0006( void );
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0007( void );
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0008( void );
CSUDI_BOOL CSTC_OS_IT_MutexWaitRelease_0009( void );
CSUDI_BOOL CSTC_OS_IT_GetTime_0001( void );
CSUDI_BOOL CSTC_OS_IT_GetTime_0002( void );
CSUDI_BOOL CSTC_OS_IT_GetTime_0003( void );
CSUDI_BOOL CSTC_OS_IT_GetTime_0004( void );
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0001( void );
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0002( void );
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0003( void );
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0004( void );
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0005( void );
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0006( void );
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0007( void );
CSUDI_BOOL CSTC_OS_IT_EventCreateDestroy_0008( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0001( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0002( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0003( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0004( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0005( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0006( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0007( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0008( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0009( void );
CSUDI_BOOL CSTC_OS_IT_EventWaitResetSet_0010( void );
CSUDI_BOOL CSTC_OS_IT_MallocFree_0001( void );
CSUDI_BOOL CSTC_OS_IT_MallocFree_0002( void );
CSUDI_BOOL CSTC_OS_IT_MallocFree_0003( void );
CSUDI_BOOL CSTC_OS_IT_MallocFree_0004( void );
CSUDI_BOOL CSTC_OS_IT_MallocFree_0005( void );
CSUDI_BOOL CSTC_OS_IT_CallocFree_0001( void );
CSUDI_BOOL CSTC_OS_IT_CallocFree_0002( void );
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0001( void );
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0002( void );
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0003( void );
CSUDI_BOOL CSTC_OS_IT_ReallocFree_0004( void );

CSUDI_BOOL CSTC_OS_IT_Available_Memory( void );
CSUDI_BOOL CSTC_OS_IT_Memory_Fragments( void );

CSUDI_BOOL CSTC_OS_IT_MemoryPerformance_0001(void);

CSUDI_BOOL CSTC_OS_IT_GetFreeRAM_0001( void );
CSUDI_BOOL CSTC_OS_IT_GetFreeRAM_0002( void );
CSUDI_BOOL CSTC_OS_IT_GetCPUCount_0001( void );
CSUDI_BOOL CSTC_OS_IT_GetCPUCount_0002( void );
CSUDI_BOOL CSTC_OS_IT_GetCPUUsage_0001( void );
CSUDI_BOOL CSTC_OS_IT_GetCPUUsage_0002( void );
CSUDI_BOOL CSTC_OS_IT_GetAllThreadHandle_0001( void );
CSUDI_BOOL CSTC_OS_IT_GetAllThreadHandle_0002( void );
CSUDI_BOOL CSTC_OS_IT_GetAllThreadHandle_0003( void );
CSUDI_BOOL CSTC_OS_IT_GetAllThreadHandle_0004( void );
CSUDI_BOOL CSTC_OS_IT_GetThreadInfo_0001( void );
CSUDI_BOOL CSTC_OS_IT_GetThreadInfo_0002( void );
CSUDI_BOOL CSTC_OS_IT_GetThreadUsedRAM_0001( void );
CSUDI_BOOL CSTC_OS_IT_GetThreadUsedRAM_0002( void );
CSUDI_BOOL CSTC_OS_IT_GetThreadCPULoad_0001( void );
CSUDI_BOOL CSTC_OS_IT_GetThreadCPULoad_0002( void );

#ifdef __cplusplus
}
#endif

#endif

