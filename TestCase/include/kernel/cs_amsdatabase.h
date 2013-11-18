/**@defgroup 应用管理器数据库接口
@brief
*/
#ifndef _CS_AMSDATABASE_H
#define _CS_AMSDATABASE_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**@brief 数据库事件类型枚举定义*/
typedef enum _CSAMSDBEventType_E{
	EM_AMSDB_UPDATE = 0,		       ///<数据库更新，appid不为空时表示某个应用更新了
	EM_AMSDB_ADDED = 1,		       ///<数据库新添加了某个应用，appid为新添加应用的应用id
	EM_AMSDB_REMOVED = 2             ///<应用将被从数据库中移除，appid 为被移除的应用id，在回调中可以从数据库中获取该应用
}CSAMSDBEventType_E;


/**
@brief 数据库变化通知回调函数定义
@param[in] eType 通知类型，见CSAMSDBEventType_E定义
@param[in] pcAppid 应用唯一标识符
@param[in] pUserData 用户自定义数据，用户注册回调函数时传入的
*/
typedef void (*CSAMSDBOnDbChanged_Fn)(CSAMSDBEventType_E eType,const char* pcAppid,void* pvUserData);


/**
@brief 应用管理器初始化
@param[in] hAMSProp 应用系统属性句柄(properties句柄)
@return 成功返回TRUE，失败返回FALSE
@note 应用管理器开机流程
*/
BOOL CSAMSInit(HCSHANDLE hAMSProp);


/**
@brief 设置应用管理器系统属性
@param[in] pcKey 属性键值见CSAMSPROP_系列键值定义
@param[in] pcValue 属性取值
@return 成功返回TRUE，失败返回FALSE
*/
BOOL CSAMSSetProperty(const char* pcKey, const char* pcValue);

/**
@brief 获取应用集

@param[in] pcFilter  过滤器字符串，可传空表示获取所有应用
@param[out] pOutAppHandleArray  输出应用句柄数组
@param[in] nArraySize 句柄数组大小
@return 成功获取则返回获取得到的应用个数，失败则返回-1
@note 过滤器支持[key=value]的形式，key可取值为"state" 或者所有的应用属性字段
@note 例如: type==java_cdc获取所有javacdc应用
*/
int CSAMSDBGetApps(const char* pcFilter,HCSHANDLE* phOutAppHandleArray,int nArraySize);

/**
@brief 获取应用句柄
@param[in] pcAppId 应用id
@return 返回应用句柄
*/
HCSHANDLE CSAMSDBGetApp(const char* pcAppid);

/**
@brief 注册监听回调
@param[in] fnCb 回调函数
@param[in] pUserData 用户自定义参数，在回调函数中将传给用户
@return 成功返回TRUE，否则返回FALSE
*/
BOOL CSAMSDBAddListener(CSAMSDBOnDbChanged_Fn fnCb,void* pvUserData);

/**
@brief 移除监听回调
@param[in] fnCb 回调函数
*/
void CSAMSDBRemoveListener(CSAMSDBOnDbChanged_Fn fnCb);

/**
@brief 删除应用(从存储设备中彻底删除)
@param[in] hAppHandle 应用句柄
@param[in] bDeleteAppFile 是否删除应用文件
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSAMSDBDeleteApp(HCSHANDLE hAppHandle, BOOL bDeleteAppFile);

/**
@brief 添加一个应用
@param[in] pcFilePath 应用存储文件夹路径
@return 返回应用句柄
*/
HCSHANDLE CSAMSDBAddApp(const char* pcfilePath);


/**
@brief 安装应用
@param[in] pcAppUrl 应用zip包的url(远程链接或本地文件)
*/
void CSAMSDBInstallApp(const char* pcAppUrl);

/**
@brief 升级应用
@param[in] pcAppUrl 应用zip包的url(远程链接或本地文件)
@param[in] pcAppId 应用的ID
*/
void CSAMSDBUpgradeApp(const char* pcAppUrl, const char* pcAppId);

/**
@brief 获取主应用
@return hMainApp 成功返回主应用句柄,失败返回NULL
*/
HCSHANDLE CSAMSDBGetMainApp();


#ifdef  __cplusplus
}
#endif

#endif

