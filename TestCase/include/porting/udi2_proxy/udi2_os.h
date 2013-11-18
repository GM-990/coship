#ifndef _UDI2_OS_H_PROXY
#define _UDI2_OS_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_os.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIOSThreadCreate(format, ...) CSUDIOSThreadCreate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSThreadCreate_PROXY(const char * pcName,int nPriority,int nStackSize, CSUDIOSThreadEntry_F fnThreadEntry,void * pvArg,CSUDI_HANDLE * phThread, const char *pcFuncName, const int nLine);

#define CSUDIOSThreadDestroy(format, ...) CSUDIOSThreadDestroy_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSThreadDestroy_PROXY(CSUDI_HANDLE hThread, const char *pcFuncName, const int nLine);

#define CSUDIOSThreadSuspend(format, ...) CSUDIOSThreadSuspend_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSThreadSuspend_PROXY(CSUDI_HANDLE hThread, const char *pcFuncName, const int nLine);

#define CSUDIOSThreadResume(format, ...) CSUDIOSThreadResume_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSThreadResume_PROXY(CSUDI_HANDLE hThread, const char *pcFuncName, const int nLine);

#define CSUDIOSThreadJoin(format, ...) CSUDIOSThreadJoin_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSThreadJoin_PROXY(CSUDI_HANDLE hThread, const char *pcFuncName, const int nLine);

#define CSUDIOSThreadSelf(format, ...) CSUDIOSThreadSelf_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIOSThreadSelf_PROXY(CSUDI_HANDLE * phThread, const char *pcFuncName, const int nLine);

#define CSUDIOSThreadSleep(format, ...) CSUDIOSThreadSleep_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
void CSUDIOSThreadSleep_PROXY(unsigned int uMilliSeconds, const char *pcFuncName, const int nLine);

#define CSUDIOSThreadYield() CSUDIOSThreadYield_PROXY(__FUNCTION__, __LINE__)
void CSUDIOSThreadYield_PROXY(const char *pcFuncName, const int nLine);

#define CSUDIOSMsgQueueCreate(format, ...) CSUDIOSMsgQueueCreate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSMsgQueueCreate_PROXY(const char * pcName,int nMaxMsgs,int nSizePerMsg,CSUDI_HANDLE * phMsgQueue, const char *pcFuncName, const int nLine);

#define CSUDIOSMsgQueueDestroy(format, ...) CSUDIOSMsgQueueDestroy_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSMsgQueueDestroy_PROXY(CSUDI_HANDLE hMsgQueue, const char *pcFuncName, const int nLine);

#define CSUDIOSMsgQueueReceive(format, ...) CSUDIOSMsgQueueReceive_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSMsgQueueReceive_PROXY(CSUDI_HANDLE hMsgQueue,void * pvMsg,int nMaxMsgBytes,unsigned int uTimeout, const char *pcFuncName, const int nLine);

#define CSUDIOSMsgQueueSend(format, ...) CSUDIOSMsgQueueSend_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSMsgQueueSend_PROXY(CSUDI_HANDLE hMsgQueue, const void * pvMsg, int nMsgBytes, unsigned int uTimeout, const char *pcFuncName, const int nLine);

#define CSUDIOSSemCreate(format, ...) CSUDIOSSemCreate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSSemCreate_PROXY(const char * pcName,int nInitialCount,int nMaxCount,CSUDI_HANDLE * phSemaphore, const char *pcFuncName, const int nLine);

#define CSUDIOSSemDestroy(format, ...) CSUDIOSSemDestroy_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSSemDestroy_PROXY(CSUDI_HANDLE hSemaphore, const char *pcFuncName, const int nLine);

#define CSUDIOSSemWait(format, ...) CSUDIOSSemWait_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSSemWait_PROXY(CSUDI_HANDLE hSemaphore,unsigned int uTimeout, const char *pcFuncName, const int nLine);

#define CSUDIOSSemRelease(format, ...) CSUDIOSSemRelease_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSSemRelease_PROXY(CSUDI_HANDLE hSemaphore, const char *pcFuncName, const int nLine);

#define CSUDIOSMutexCreate(format, ...) CSUDIOSMutexCreate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSMutexCreate_PROXY(const char * pcName, unsigned int uOptions,CSUDI_HANDLE * phMutex, const char *pcFuncName, const int nLine);

#define CSUDIOSMutexDestroy(format, ...) CSUDIOSMutexDestroy_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSMutexDestroy_PROXY(CSUDI_HANDLE hMutex, const char *pcFuncName, const int nLine);

#define CSUDIOSMutexWait(format, ...) CSUDIOSMutexWait_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSMutexWait_PROXY(CSUDI_HANDLE hMutex,unsigned int uTimeout, const char *pcFuncName, const int nLine);

#define CSUDIOSMutexRelease(format, ...) CSUDIOSMutexRelease_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSMutexRelease_PROXY(CSUDI_HANDLE hMutex, const char *pcFuncName, const int nLine);

#define CSUDIOSGetTime(format, ...) CSUDIOSGetTime_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGetTime_PROXY(CSUDIOSTimeVal_S * psTime, const char *pcFuncName, const int nLine);

#define CSUDIOSEventCreate(format, ...) CSUDIOSEventCreate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSEventCreate_PROXY(const char * pcName,unsigned int  uFlags,CSUDI_HANDLE * phEvent, const char *pcFuncName, const int nLine);

#define CSUDIOSEventDestroy(format, ...) CSUDIOSEventDestroy_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSEventDestroy_PROXY(CSUDI_HANDLE hEvent, const char *pcFuncName, const int nLine);

#define CSUDIOSEventReset(format, ...) CSUDIOSEventReset_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSEventReset_PROXY(CSUDI_HANDLE hEvent, const char *pcFuncName, const int nLine);

#define CSUDIOSEventSet(format, ...) CSUDIOSEventSet_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSEventSet_PROXY(CSUDI_HANDLE hEvent, const char *pcFuncName, const int nLine);

#define CSUDIOSEventWait(format, ...) CSUDIOSEventWait_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSEventWait_PROXY(CSUDI_HANDLE hEvent,unsigned int uTimeout, const char *pcFuncName, const int nLine);

#define CSUDIOSMalloc(format, ...) CSUDIOSMalloc_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
void * CSUDIOSMalloc_PROXY(unsigned int uSize , const char *pcFuncName, const int nLine);

#define CSUDIOSRealloc(format, ...) CSUDIOSRealloc_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
void* CSUDIOSRealloc_PROXY(void * pvAddr,unsigned int uSize , const char *pcFuncName, const int nLine);

#define CSUDIOSCalloc(format, ...) CSUDIOSCalloc_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
void* CSUDIOSCalloc_PROXY(unsigned int uElements, unsigned int uElementSize, const char *pcFuncName, const int nLine);

#define CSUDIOSFree(format, ...) CSUDIOSFree_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSFree_PROXY(void * pvAddr , const char *pcFuncName, const int nLine);

#define CSUDIOSGetFreeRAM(format, ...) CSUDIOSGetFreeRAM_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGetFreeRAM_PROXY(unsigned int * puFreeCount, const char *pcFuncName, const int nLine);

#define CSUDIOSGetCPUCount(format, ...) CSUDIOSGetCPUCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGetCPUCount_PROXY(unsigned int * puCpuCount, const char *pcFuncName, const int nLine);

#define CSUDIOSGetCPUUsage(format, ...) CSUDIOSGetCPUUsage_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGetCPUUsage_PROXY(unsigned int index, unsigned int * puUsage, const char *pcFuncName, const int nLine);

#define CSUDIOSGetAllThreadHandle(format, ...) CSUDIOSGetAllThreadHandle_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGetAllThreadHandle_PROXY(CSUDI_HANDLE* phThread, unsigned int*  puSize, const char *pcFuncName, const int nLine);

#define CSUDIOSGetThreadInfo(format, ...) CSUDIOSGetThreadInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGetThreadInfo_PROXY(CSUDI_HANDLE hThread, CSUDIOSThreadInfo_S* psThreadInfo, const char *pcFuncName, const int nLine);

#define CSUDIOSGetThreadUsedRAM(format, ...) CSUDIOSGetThreadUsedRAM_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGetThreadUsedRAM_PROXY(CSUDI_HANDLE hThread, unsigned int * puUsedRam, const char *pcFuncName, const int nLine);

#define CSUDIOSGetThreadCPUUsage(format, ...) CSUDIOSGetThreadCPUUsage_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIOSGetThreadCPUUsage_PROXY(CSUDI_HANDLE hThread, unsigned int * puUsage, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
