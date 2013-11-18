#ifndef _CS_DLNA_MEDIA_SERVER_H_
#define _CS_DLNA_MEDIA_SERVER_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**
@brief 创建一个Server
@param[in] pcServerName Server的名称，ASCII码，最长不能超过256 
@param[in] pcSharedDir 增加的共享目录,以'\0'结束字符串
@return 
- 成功返回Server句柄。
- 失败返回NULL。
*/
HCSHANDLE CSDLNAMediaServerCreate(const char * pcServerName, const char *pcSharedDir);

/**
@brief 销毁一个Server
@param[in] hServer Server的句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaServerDestroy(HCSHANDLE hServer);

/**
@brief 启动Server
@param[in] hServer Server句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaServerStart(HCSHANDLE hServer);

/**
@brief 停止Server
@param[in] hServer Server句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaServerStop(HCSHANDLE hServer);

#ifdef __cplusplus
}
#endif

#endif

























