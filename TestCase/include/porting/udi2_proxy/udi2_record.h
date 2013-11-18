#ifndef _UDI2_RECORD_H_PROXY
#define _UDI2_RECORD_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "../udi2/udi2_record.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIRECAddCallback(format, ...) CSUDIRECAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECAddCallback_PROXY(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback, void * pvUserData, const char *pcFuncName, const int nLine);

#define CSUDIRECDelCallback(format, ...) CSUDIRECDelCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECDelCallback_PROXY(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback,void * pvUserData, const char *pcFuncName, const int nLine);

#define CSUDIRECOpen(format, ...) CSUDIRECOpen_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECOpen_PROXY(const CSUDIRecChnl_S * psChnl, CSUDI_HANDLE * phRecHandle, const char *pcFuncName, const int nLine);

#define CSUDIRECGetSupportEncryptTypes(format, ...) CSUDIRECGetSupportEncryptTypes_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECGetSupportEncryptTypes_PROXY(CSUDI_HANDLE hRecHandle,CSUDIStreamEncryptType_E* peOutTypeArray,int nArraySize,int* pnActualCnt, const char *pcFuncName, const int nLine);

#define CSUDIRECClose(format, ...) CSUDIRECClose_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECClose_PROXY(CSUDI_HANDLE hRecHandle, const char *pcFuncName, const int nLine);

#define CSUDIRECStart(format, ...) CSUDIRECStart_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECStart_PROXY(CSUDI_HANDLE  hRecHandle,const CSUDIRecStartParam_S * psStartParam, const char *pcFuncName, const int nLine);

#define CSUDIRECStop(format, ...) CSUDIRECStop_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECStop_PROXY(CSUDI_HANDLE  hRecHandle, const char *pcFuncName, const int nLine);

#define CSUDIRECStopTshift(format, ...) CSUDIRECStopTshift_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECStopTshift_PROXY(CSUDI_HANDLE  hRecHandle,CSUDI_BOOL bSwitchToFile, const char *pcFuncName, const int nLine);

#define CSUDIRECGetRecTime(format, ...) CSUDIRECGetRecTime_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECGetRecTime_PROXY(CSUDI_HANDLE  hRecHandle, CSUDIRecTimeInfo_S * psTimeInfo, const char *pcFuncName, const int nLine);

#define CSUDIRECFileCut(format, ...) CSUDIRECFileCut_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIRECFileCut_PROXY(const char* szFileName,unsigned int unStartPos,unsigned int unEndPos, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
