#ifndef _UDI2_BOOTSHOW_H_PROXY
#define _UDI2_BOOTSHOW_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_tuner.h"
#include "../udi2/udi2_bootshow.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIBOOTSHOWGetCapability(format, ...) CSUDIBOOTSHOWGetCapability_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBOOTSHOWGetCapability_PROXY(CSUDIBOOTSHOWCapability* psOutCapability, const char *pcFuncName, const int nLine);

#define CSUDIBOOTSHOWGetShowUnitInfo(format, ...) CSUDIBOOTSHOWGetShowUnitInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBOOTSHOWGetShowUnitInfo_PROXY(int nIndex,CSUDIBOOTSHOWUnitInfo* psOutInfo, const char *pcFuncName, const int nLine);

#define CSUDIBOOTSHOWGetFreeSize(format, ...) CSUDIBOOTSHOWGetFreeSize_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBOOTSHOWGetFreeSize_PROXY(unsigned int* pnOutSize, const char *pcFuncName, const int nLine);

#define CSUDIBOOTSHOWGetUnitCount(format, ...) CSUDIBOOTSHOWGetUnitCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBOOTSHOWGetUnitCount_PROXY(unsigned int* pnOutCount, const char *pcFuncName, const int nLine);

#define CSUDIBOOTSHOWSetNeedShow(format, ...) CSUDIBOOTSHOWSetNeedShow_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBOOTSHOWSetNeedShow_PROXY(int nIndex, CSUDI_BOOL bNeedShow, const char *pcFuncName, const int nLine);

#define CSUDIBOOTSHOWSetShowUnitInfo(format, ...) CSUDIBOOTSHOWSetShowUnitInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIBOOTSHOWSetShowUnitInfo_PROXY(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo,void* pData, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
