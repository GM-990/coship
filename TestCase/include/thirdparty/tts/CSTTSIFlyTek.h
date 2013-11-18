#if !defined(_CSTTSIFLYTEK_H)
#define _CSTTSIFLYTEK_H

#include "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**
@brief 设置科大讯飞语音引擎所需资源包的默认路径
@param[in] pcDefResPath 语音引擎所需资源路径
@return 成功返回TRUE; 失败返回FALSE
*/
BOOL CSTTSSetDefaultResPath(const char* pcDefResPath, DWORD dwResPathLen);

/**
@brief 增加一个科大讯飞引擎所需资源包的的路径
@param[in] pcResPath 语音引擎所需资源路径
@param[in] dwResPathLen pcResPath的长度
@return 成功返回TRUE; 失败返回FALSE
@remark	1)增加一个已存在的资源包路径，直接返回成功；
		2)可通过CSTTSDeleteResPath()接口来删除设置的资源包路径；
		3)该接口不支持多线程操作；
*/
BOOL CSTTSAddResPath(const char * pcResPath, DWORD dwResPathLen);

/**
@brief 删除科大讯飞引擎所需资源包的的路径
@param[in] pcResPath 语音引擎所需资源路径
@param[in] dwResPathLen pcResPath的长度
@return 成功返回TRUE; 失败返回FALSE
@remark	1)本接口只删除由CSTTSAddResPath()接口设置的资源包路径；
		2)当参数pcResPath传入NULL时，删除由CSTTSAddResPath设置的全部资源路径；
		3)删除一个不存在的资源路径时，返回失败；
		4)该接口不支持多线程操作；
*/
BOOL CSTTSDeleteResPath(const char * pcResPath, DWORD dwResPathLen);

/**
@brief 创建科大讯飞语音引擎资源句柄
@param[in] pResPath 语音引擎所需资源路径
@return 成功返回资源句柄;失败返回NULL
@remark 1)若pResPath不为NULL，则实际使用pResPath + CSTTSAddResPath设置的路径；
		2)若pResPath为NULL，则实际使用CSTTSSetDefaultResPath设置的路径 + CSTTSAddResPath设置的路径；
		3)如果不调用CSTTSSetDefaultResPath设置默认路径，也不调用CSTTSAddResPath添加路径，则pcResPath不能赋为NULL；
*/
HCSHANDLE CSTTSCreateIFlyTek(const char* pcResPath);

#ifdef __cplusplus
}
#endif

#endif

