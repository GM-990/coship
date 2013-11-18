#ifndef _UDI2_INJECT_H_PROXY
#define _UDI2_INJECT_H_PROXY

#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_inject.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIINJECTEROpen(format, ...) CSUDIINJECTEROpen_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIINJECTEROpen_PROXY(const CSUDIINJECTERChnl_S * psInjecterChnl, const CSUDIINJECTEROpenParam_S * psOpenParams, CSUDI_HANDLE * phINJECTER, const char *pcFuncName, const int nLine);

#define CSUDIINJECTERClose(format, ...) CSUDIINJECTERClose_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code  CSUDIINJECTERClose_PROXY(CSUDI_HANDLE hINJECTER, const char *pcFuncName, const int nLine);

#define CSUDIINJECTERSetAttribute(format, ...) CSUDIINJECTERSetAttribute_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINJECTERSetAttribute_PROXY(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, const void * pvData, const char *pcFuncName, const int nLine);

#define CSUDIINJECTERGetAttribute(format, ...) CSUDIINJECTERGetAttribute_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINJECTERGetAttribute_PROXY(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, void * pvData, const char *pcFuncName, const int nLine);

#define CSUDIINJECTERStart(format, ...) CSUDIINJECTERStart_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINJECTERStart_PROXY(CSUDI_HANDLE hINJECTER, const char *pcFuncName, const int nLine);

#define CSUDIINJECTERStop(format, ...) CSUDIINJECTERStop_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINJECTERStop_PROXY(CSUDI_HANDLE hINJECTER, const char *pcFuncName, const int nLine);

#define CSUDIINJECTERFlush(format, ...) CSUDIINJECTERFlush_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINJECTERFlush_PROXY(CSUDI_HANDLE hINJECTER, const char *pcFuncName, const int nLine);

#define CSUDIINJECTERClear(format, ...) CSUDIINJECTERClear_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINJECTERClear_PROXY(CSUDI_HANDLE hINJECTER, const char *pcFuncName, const int nLine);

#define CSUDIINJECTERGetFreeBuffer(format, ...) CSUDIINJECTERGetFreeBuffer_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINJECTERGetFreeBuffer_PROXY(CSUDI_HANDLE hINJECTER, void ** ppvBuffer, unsigned  int * puLength , const char *pcFuncName, const int nLine);

#define CSUDIINJECTERWriteComplete(format, ...) CSUDIINJECTERWriteComplete_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIINJECTERWriteComplete_PROXY(CSUDI_HANDLE hINJECTER, unsigned int uWrittenSize, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
