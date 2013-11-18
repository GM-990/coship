/**@defgroup 本模块对远程调用服务器端对外接口和数据结构进行了定义
@brief RMIServer模块对应的头文件<cs_rmiserver.h>,定义了远程调用服务器端的操作。服务器端的操作应和客户端操作相对应，客户端端接口请查看<cs_rmiclient.h>

@version 2.0.1 2011/12/5 
@{
*/


#ifndef _CS_RMISERVER_H_
#define _CS_RMISERVER_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief  rmiserver模块配置信息*/
typedef struct _CSRMISConfig_S{
	int nListenPort;		// 服务器的监听端口号
	int nBufferSize;		// 配置用于存放回传数据buffer大小，该值应为回传数据长度的最大值
}CSRMISConfig_S;

/**
@brief action处理函数

@param[in] pcFuncName - 远程调用函数名
@param[in] pcParamJson - 函数参数信息
@param[out] pOutData - 用于存放函数执行结果的buffer
@param[in] nDataSize - pOutData的大小
@param pUserData 用户自定义参数(注册回调函数时传入的参数)
@return
@note 
- 1、pcFuncName与远程调用客户端接口CSRMICCallRemoteFunc中pcFuncName参数保持一致。
- 2、pcParamJson与远程调用客户端接口CSRMICCallRemoteFunc中pcParamJson参数保持一致。
- 3、nDataSize与创建服务器时传入的配置参数nBufferSize保持一致。
*/
typedef int (*CSRMISOnProcess_F)(
	const char * pcFuncName, const char * pcParamJson, void * pOutData, int nDataSize, void * pUserData);

/**
@brief 获取RMIServer的默认配置信息
@param[out] psOutConfig 输出RMIServer默认的配置信息
@return
@note 
*/
void CSRMISGetDefaultConfig(CSRMISConfig_S * psOutConfig);

/**
@brief 创建一个RMIServer服务器

@param[in] psConfig 配置信息，默认的配置信息可通过CSRMISGetDefaultConfig接口获取
@param[out] phClient 生成的句柄，后续操作使用该句柄
@return 成功返回TRUE;失败返回FALSE，且phThread为CSUDI_NULL
*/
BOOL CSRMISCreate(const CSRMISConfig_S * psConfig, CSUDI_HANDLE * phServer);

/**
@brief 销毁hServer服务器

@param[in] hServer 服务器句柄，即CSRMISCreate的输出参数phServer
@return 成功返回TRUE;失败返回FALSE
@note 
-  不能销毁一个正在被操作（如:正在CSRMISAddAction）的hClient，该行为会导致不确定的结果
*/
BOOL CSRMISDestroy(CSUDI_HANDLE hServer);

/**
@brief 添加Rmiserver action

@param[in] hServer 服务器句柄，即CSRMISCreate的输出参数phServer
@param[in] pcModuleName 模块名
@param fnCb - action回调函数
@param pUserData 用户自定义参数(回调通知时回传给回调函数)
@return 成功返回TRUE;失败返回FALSE
@note 
- 一个模块对应于一个action操作。为一个模块添加两个action时，第二次会返回失败。
*/
BOOL CSRMISAddAction(CSUDI_HANDLE hServer, const char * pcModuleName, CSRMISOnProcess_F fnOnProcess, void * pUserData);

/**
@brief 移除Rmiserver action

@param[in] hServer 服务器句柄，即CSRMISCreate的输出参数phServer
@param[in] pcModuleName 模块名
@return 成功返回TRUE;失败返回FALSE
*/
BOOL CSRMISRemoveAction(CSUDI_HANDLE hServer, const char * pcModuleName);

#ifdef __cplusplus
}
#endif

#endif
