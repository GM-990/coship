/**@defgroup 应用控制器接口
@brief
*/
#ifndef _CS_AMSCONTROLLER_H
#define _CS_AMSCONTROLLER_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**@brief 应用操作枚举定义*/
typedef enum _CSAMSAction_E{
	EM_APPACTION_START = 0,	///<启动应用
	EM_APPACTION_PAUSE = 1,	///<暂停应用
	EM_APPACTION_RESUME = 2,	///<恢复应用
	EM_APPACTION_STOP = 3,		///<停止应用
	EM_APPACTION_INIT = 4		///<初始化应用
}CSAMSAction_E;

/**@brief 应用操作状态*/
typedef enum _CSAMSActionStatus_E{
	EM_APPACTION_TYPE_SUCCEED = 0,	///<操作执行成功通知
	EM_APPACTION_TYPE_BEFORE = 1,	///<操作执行前通知
	EM_APPACTION_TYPE_FAILED = 2,		///<操作执行失败通知
	EM_APPACTION_TYPE_BLOCKED = 3	///<操作执行堵塞通知
}CSAMSActionStatus_E;

/**
@brief 应用操作回调通知
@param[in] eAction 操作类型
@param[in] eActionType 操作状态
@param[in] pcAppId 
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
*/
typedef void (*CSAMSCTRLOnAction_Fn) (CSAMSAction_E eAction, CSAMSActionStatus_E eActionType, const char *pcAppId, void* pvUserData);

/**
@brief 启动应用
@param[in] pcAppId 应用id
@param[in] pcParam 启动参数，Asii字符串，可为空
@return 成功返回TRUE,失败返回FALSE(该应用不存在时)
@note 应用操作都是异步执行的。因此这里的失败或成功仅标识命令是否成功发出。下同
*/
BOOL CSAMSCTRLStartApp(const char* pcAppId,const char* pcParam);

/**
@brief 暂停应用
@param[in] pcAppId 应用id
@return 成功返回TRUE,失败返回FALSE(该应用不存在时)
@note 应用操作都是异步执行的
*/
BOOL CSAMSCTRLPauseApp(const char* pcAppId);

/**
@brief 停止应用
@param[in] pcAppId 应用id
@return 成功返回TRUE,失败返回FALSE(该应用不存在时)
@note 应用操作都是异步执行的
*/
BOOL CSAMSCTRLStopApp(const char* pcAppId);

/**
@brief 停止系列应用
@param[in] ppcAppIds 应用id数组
@param[in] nCount 应用id个数
@return 成功返回TRUE,失败返回FALSE(有任意应用不存在时则返回FALSE)
@note 应用操作都是异步执行的。
*/
BOOL CSAMSCTRLStopApps(const char** ppcAppIds,int nCount);

/**
@brief 恢复应用
@param[in] pcAppId 应用id
@return 成功返回TRUE,失败返回FALSE(该应用不存在时)
@note 应用操作都是异步执行的
*/
BOOL CSAMSCTRLResumeApp(const char* pcAppId);

/**
@brief 获取正在运行的焦点应用
@return 返回应用句柄
*/
HCSHANDLE CSAMSCTRLGetActiveApp();

/**
@brief 获取所有正在运行的应用列表。(可视的应用，对于后台运行的应用无法通过该接口获取)
@param[out] pOutAppHandleArray  输出应用句柄数组
@param[in] nArraySize 句柄数组大小
@return 成功获取则返回获取得到的应用个数，失败则返回-1
*/
int CSAMSCTRLGetRuningApps(HCSHANDLE* phOutAppHandleArray,int nArraySize);

/**
@brief 注册监听回调
@param[in] fnCb 回调函数
@param[in] pUserData 用户自定义参数，在回调函数中将传给用户
@return 成功返回TRUE，否则返回FALSE
*/
BOOL CSAMSCTRLAddListener(CSAMSCTRLOnAction_Fn fnCb,void* pvUserData);

/**
@brief 移除监听回调
@param[in] fnCb 回调函数
*/
void CSAMSCTRLRemoveListener(CSAMSCTRLOnAction_Fn fnCb);


BOOL CSAMSCTRLStartRemoteApp(const char* classPath,const char* mainClass,const char* entryMain,const char* pzAgus);

#ifdef  __cplusplus
}
#endif

#endif

