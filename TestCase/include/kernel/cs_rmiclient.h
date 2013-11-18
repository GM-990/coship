/**@defgroup 本模块对远程调用客户端对外接口和数据结构进行了定义
@brief RMIClient模块对应的头文件<cs_rmiclient.h>,定义了远程客户端的操作。客户端的操作应该和服务器端操作相对应，服务器端接口请查看<cs_rmiserver.h>

@version 2.0.1 2011/12/5 
@{
*/

#ifndef _CS_RMICLIENT_H_
#define _CS_RMICLIENT_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSRMIC_IP_LEN 16				

/**@brief  rmiclient模块配置信息*/
typedef struct _CSRMICConfig_S{
	char acServAddr[CSRMIC_IP_LEN];		// 服务器ip地址，字符串形式，如:"127.0.0.1"
	int nServPort;			// 服务器端口号
	int nConnTimeout;		// 与服务器端建立连接时的连接超时时间，单位为毫秒
	int nRecvTimeout;		// 接收数据时的接收超时时间，单位为毫秒
	int nSendTimeout;		// 发送数据时的发送超时时间，单位为毫秒
}CSRMICConfig_S;

/**
@brief 获取CSRMIClient的默认配置信息
@param[out] psOutConfig 输出RMIClient默认的配置信息
@note 
*/
void CSRMICGetDefaultConfig(CSRMICConfig_S * psOutConfig);

/**
@brief 创建一个CSRMIClient客户端,并与服务器建立连接

@param[in] psConfig 配置信息，默认的配置信息可通过CSRMICGetDefaultConfig接口获取
@param[out] phClient 生成的句柄，后续操作使用该句柄
@return 成功返回TRUE;失败返回FALSE，且phThread为CSUDI_NULL
*/
BOOL CSRMICCreate(const CSRMICConfig_S * psConfig, CSUDI_HANDLE * phClient);

/**
@brief 销毁hClient客户端

@param[in] hClient 客户端句柄，即CSRMICCreate的输出参数phClient
@return 成功返回TRUE;失败返回FALSE
@note 
-  不能销毁一个正在被操作（如:正在CSRMICCallRemoteFunc）的hClient，该行为会导致不确定的结果
*/
BOOL CSRMICDestroy(CSUDI_HANDLE hClient);

/**
@brief 进行远程调用

@param[in] hClient 客户端句柄，即CSRMICCreate的输出参数phClient。
@param[in] pcModuleName 模块名，非空
@param[in] pcFuncName 函数名，非空
@param[in] pcParamJson 函数参数信息，非空
@return 成功:返回远程函数执行结果字符串指针；失败返回NULL
@note 
- 1、对返回的字符串进行处理后，应通过CSRMICFree()接口将返回的结果字符串进行释放
- 2、pcModuleName应与远程调用服务器接口CSRMISAddAction中pcModuleName参数保持一致。
*/
char * CSRMICCallRemoteFunc(CSUDI_HANDLE hClient, const char * pcModuleName, const char * pcFuncName, const char * pcParamJson);

/**
@brief 释放CSRMICCallRemoteFunc返回的字符串

@param[in] hClient 客户端句柄，即CSRMICCreate的输出参数phClient。
@param[in] pcJsonStr 执行CSRMICCallRemoteFunc函数返回的字符串指针
@return 成功返回TRUE；失败返回FALSE
*/
BOOL CSRMICFree(CSUDI_HANDLE hClient, char * pcJsonStr);

#ifdef __cplusplus
}
#endif

#endif
