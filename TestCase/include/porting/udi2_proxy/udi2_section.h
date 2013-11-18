#ifndef _UDI2_SECTION_H_PROXY
#define _UDI2_SECTION_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_section.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIFILTERAllocate(format, ...) CSUDIFILTERAllocate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFILTERAllocate_PROXY(const CSUDISECTIONRequestInfo_S * psRequestInfo, CSUDISECTIONCallback_F fnSectionCallback, const void * pvUserData ,CSUDI_HANDLE * phFilterHandle, const char *pcFuncName, const int nLine);

#define CSUDIFILTERStart(format, ...) CSUDIFILTERStart_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFILTERStart_PROXY(CSUDI_HANDLE hFilterHandle, const char *pcFuncName, const int nLine);

#define CSUDIFILTERStop(format, ...) CSUDIFILTERStop_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFILTERStop_PROXY(CSUDI_HANDLE hFilterHandle , const char *pcFuncName, const int nLine);

#define CSUDIFILTERFree(format, ...) CSUDIFILTERFree_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFILTERFree_PROXY(CSUDI_HANDLE hFilterHandle, const char *pcFuncName, const int nLine);

#define CSUDIFILTERModify(format, ...) CSUDIFILTERModify_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFILTERModify_PROXY(CSUDI_HANDLE hFilterHandle, const CSUDIFILTERMatchMask_S * psMMFilter,  CSUDI_BOOL bCRCCheck, const char *pcFuncName, const int nLine);

#define CSUDIFILTERSetBufSize(format, ...) CSUDIFILTERSetBufSize_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFILTERSetBufSize_PROXY(CSUDI_HANDLE hFilterHandle,  int nBufSize, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
