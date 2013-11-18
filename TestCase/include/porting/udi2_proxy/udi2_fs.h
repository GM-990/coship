#ifndef _UDI2_FS_H_PROXY
#define _UDI2_FS_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"
#include "../udi2/udi2_fs.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIFSRDIAddCallback(format, ...) CSUDIFSRDIAddCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFSRDIAddCallback_PROXY(CSUDIRDICallback_F fnCallback,const void * pvUserData, const char *pcFuncName, const int nLine);

#define CSUDIFSRDIRemoveCallback(format, ...) CSUDIFSRDIRemoveCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFSRDIRemoveCallback_PROXY(CSUDIRDICallback_F fnCallback,const void * pvUserData, const char *pcFuncName, const int nLine);

#define CSUDIFSRDIRemoveDev(format, ...) CSUDIFSRDIRemoveDev_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFSRDIRemoveDev_PROXY(CSUDI_UINT32 dwDeviceId, const char *pcFuncName, const int nLine);

#define CSUDIFSGetAllDeviceId(format, ...) CSUDIFSGetAllDeviceId_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFSGetAllDeviceId_PROXY(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt, const char *pcFuncName, const int nLine);

#define CSUDIFSGetDeviceInfo(format, ...) CSUDIFSGetDeviceInfo_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFSGetDeviceInfo_PROXY(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo, const char *pcFuncName, const int nLine);

#define CSUDIFSStandby(format, ...) CSUDIFSStandby_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFSStandby_PROXY(CSUDI_UINT32 dwDeviceId, const char *pcFuncName, const int nLine);

#define CSUDIFSWakeup(format, ...) CSUDIFSWakeup_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFSWakeup_PROXY(CSUDI_UINT32 dwDeviceId, const char *pcFuncName, const int nLine);

#define CSUDIFSFormat(format, ...) CSUDIFSFormat_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFSFormat_PROXY(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType, const char *pcFuncName, const int nLine);

#define CSUDIFSGetCodePage(format, ...) CSUDIFSGetCodePage_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIFSGetCodePage_PROXY(int * pnCodePage, const char *pcFuncName, const int nLine);

#define CSUDIFSOpen(format, ...) CSUDIFSOpen_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_HANDLE CSUDIFSOpen_PROXY(const char * pcFileName, const char * pcMode, const char *pcFuncName, const int nLine);

#define CSUDIFSRead(format, ...) CSUDIFSRead_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSRead_PROXY(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount, const char *pcFuncName, const int nLine);

#define CSUDIFSWrite(format, ...) CSUDIFSWrite_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSWrite_PROXY(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount, const char *pcFuncName, const int nLine);

#define CSUDIFSClose(format, ...) CSUDIFSClose_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSClose_PROXY(CSUDI_HANDLE hFile, const char *pcFuncName, const int nLine);

#define CSUDIFSSeek(format, ...) CSUDIFSSeek_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSSeek_PROXY(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin, const char *pcFuncName, const int nLine);

#define CSUDIFSLseek(format, ...) CSUDIFSLseek_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSLseek_PROXY(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin, const char *pcFuncName, const int nLine);

#define CSUDIFSTell(format, ...) CSUDIFSTell_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
long CSUDIFSTell_PROXY(CSUDI_HANDLE hFile, const char *pcFuncName, const int nLine);

#define CSUDIFSLtell(format, ...) CSUDIFSLtell_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_INT64 CSUDIFSLtell_PROXY(CSUDI_HANDLE hFile, const char *pcFuncName, const int nLine);

#define CSUDIFSFlush(format, ...) CSUDIFSFlush_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSFlush_PROXY(CSUDI_HANDLE hFile, const char *pcFuncName, const int nLine);

#define CSUDIFSRemove(format, ...) CSUDIFSRemove_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSRemove_PROXY(const char * pcFileName, const char *pcFuncName, const int nLine);

#define CSUDIFSRename(format, ...) CSUDIFSRename_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSRename_PROXY(const char * pcOldPath, const char * pcNewPath, const char *pcFuncName, const int nLine);

#define CSUDIFSTruncate(format, ...) CSUDIFSTruncate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSTruncate_PROXY(const char * pcPathName, unsigned long ulLength, const char *pcFuncName, const int nLine);

#define CSUDIFSFTruncate(format, ...) CSUDIFSFTruncate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSFTruncate_PROXY(CSUDI_HANDLE hFile, unsigned long ulLength, const char *pcFuncName, const int nLine);

#define CSUDIFSLTruncate(format, ...) CSUDIFSLTruncate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSLTruncate_PROXY(const char * pcPathName,CSUDI_UINT64 u64Length, const char *pcFuncName, const int nLine);

#define CSUDIFSLFTruncate(format, ...) CSUDIFSLFTruncate_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSLFTruncate_PROXY(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length, const char *pcFuncName, const int nLine);

#define CSUDIFSStat(format, ...) CSUDIFSStat_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSStat_PROXY(const char * pcFileName, CSUDIFSFileStat_S * psFileStat, const char *pcFuncName, const int nLine);

#define CSUDIFSFstat(format, ...) CSUDIFSFstat_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSFstat_PROXY(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat, const char *pcFuncName, const int nLine);

#define CSUDIFSMkdir(format, ...) CSUDIFSMkdir_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSMkdir_PROXY(const char * pcPathName, int nReserved, const char *pcFuncName, const int nLine);

#define CSUDIFSRmdir(format, ...) CSUDIFSRmdir_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSRmdir_PROXY(const char * pcPathName, const char *pcFuncName, const int nLine);

#define CSUDIFSOpenDir(format, ...) CSUDIFSOpenDir_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_HANDLE CSUDIFSOpenDir_PROXY(const char * pcPathName, const char *pcFuncName, const int nLine);

#define CSUDIFSCloseDir(format, ...) CSUDIFSCloseDir_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSCloseDir_PROXY(CSUDI_HANDLE hDir, const char *pcFuncName, const int nLine);

#define CSUDIFSStatfs(format, ...) CSUDIFSStatfs_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSStatfs_PROXY(const char * pcPathName, CSUDIFSStatFs_S * psFsStat, const char *pcFuncName, const int nLine);

#define CSUDIFSFstatfs(format, ...) CSUDIFSFstatfs_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIFSFstatfs_PROXY(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
