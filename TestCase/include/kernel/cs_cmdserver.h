#ifndef _CS_CMDSERVER_H_
#define _CS_CMDSERVER_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief 响应并处理Server收到的命令
@param[in] pvCmd 指向接收到到的命令的指针
@param[out] result_p 指向输出字符串结果的指针，可以不处理
@return 成功返回1，异常返回0
@note 
- 可以通过pvCmd指针来依次获取接收到的每个参数的信息，但需要与参数类型匹配\n
-例如,客户端发送的数据类型顺序依次为int int string，则可以依次调用\n
-CSCLIGetInteger (pvCmd, 0, &nRet)	\n
-CSCLIGetInteger (pvCmd, 0, &nRet)	\n
-CSCLIGetString(pvCmd, "", szRet, sizeof(szRet))	\n
-将客户端发送的数据保存到对应的nRet和szRet中\n
-详细使用可参考cs_cli.h	\n
*/
typedef int (*CSOnCmdServerProcess_F)(void *pvCmd, char *pcResult);

/**
@brief 响应并处理插件模块初始化操作

@param[in] nPluginId 该插件对应的ID号
@return 成功返回1，异常返回0
@note 
-
*/
typedef int (*CSOnCmdServerInitPlugin_F)(int nPluginId);

/**
@brief 插件实例结构体
@param[in] m_szModuleName 注册支持的模块名，例如AVPlayer, DeviceManager...
@param[in] m_szModuleCmdFormat 输入的cmd格式,需要按照不同的模块规定的格式输入，对于AVPlayer，输入格式例如AVPlayer 0 0 0 "......" 1 0 0\n 
@param[out] m_fnOnCmdServerAddCLI 注册的 CLI模块命令执行函数，详见cs_cli.h中CSCLIRegisterCommand说明
@param[out] m_fnOnCmdServerAddInitPlugin 注册所支持的插件模块的初始化函数
@note 
-支持第三方相关功能的实例，如音视频播放AVPlayer，设备管理DeviceManager等\n
-可支持扩展，但必须实现CSOnCmdServerProcess_F和CSOnCmdServerInitPlugin_F	\n
*/
typedef struct 
{
	char* m_szModuleName;
	char* m_szModuleCmdFormat;
	CSOnCmdServerProcess_F m_fnOnCmdServerAddCLI;
	CSOnCmdServerInitPlugin_F	m_fnOnCmdServerAddInitPlugin;
}CSCmdInterface_S;


/**
@brief 注册插件信息结构体
*/
typedef struct 
{
	int m_nPluginId;
	BOOL m_bUsed;
	CSCmdInterface_S m_sCmdInterface;
	void * m_pUsr;
}CSCMDServeRegPlugin_S;

/**
@brief 初始化CMDServer服务器端socket相关操作和CLI模块

@param[in] nServerPort 服务器端监听的端口号，传0为默认值6463
@return 初始化成功返回1，异常返回0
@note 
- 
*/
int CSCMDServerInit(int nServerPort);

/**
@brief 注册CMDServer模块支持的某一种插件，一次只能注册一个插件

@param[in] psCmdServerInter :支持的插件实例结构体，详见CSCmdInterface_S
@return 注册成功返回1，失败返回0
@note 
- 
*/
int CSCMDServerRegister(CSCmdInterface_S *psCmdServerInter);


/**
@brief 向客户端回发消息

@param[in] nServerId :对应插件实例的句柄
@param[in] pvData :回发客户端的数据
@param[in] nDataLen :回发客户端的数据大小
@return 发送成功返回1，失败或超时返回0
@note 
- 
*/
int CSCMDServerSend(int nPluginId, void *pvData, int nDataLen);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
