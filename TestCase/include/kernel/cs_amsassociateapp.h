

#ifndef _AMS_ASSOCIATEAPP_H_
#define _AMS_ASSOCIATEAPP_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "mmcp_typedef.h"

/**@brief 节目关联应用事件*/
typedef enum _CSAMSAssociateAppEvent_E
{
	EM_AMS_ASSOCIATEAPP_DETECTED         = 0,          //检测到节目关联应用
	EM_AMS_ASSOCIATEAPP_LOAD_BEGIN       = 1,         //关联应用开始下载,pcAppid为该应用ID
	EM_AMS_ASSOCIATEAPP_LOAD_SUCCESS     = 2,         //关联应用下载成功,pcAppid为该应用ID
	EM_AMS_ASSOCIATEAPP_LOAD_FAILED      =3           //关联应用下载失败,pcAppid为该应用ID
}CSAMSAssociateAppEvent_e;


/**
@brief 节目关联应用事件回调通知
@param[in] eEvent 事件类型
@param[in] pcAppid 应用ID
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
*/
typedef void (*CSAMSAssociateAppOnAction_Fn) (CSAMSAssociateAppEvent_e eEvent, const char* pcAppid, void* pvUserData);


/**
@brief 启动检测节目关联应用
@return 启动成功返回TRUE，否则返回FALSE
*/
BOOL CSAMSAssociateAppInit();


/**
@brief 启动AitManager
@param[in] pcLocator 当前正在播放的节目的locator
*/
BOOL CSAMSAssociateAppAitManagerStart(const char *pcLocator);


/**
@brief 停止AitManager
*/
void CSAMSAssociateAppAitManagerStop();


/**
@brief 注册监听回调
@param[in] fnCb 回调函数
@param[in] pUserData 用户自定义参数，在回调函数中将传给用户
@return 成功返回TRUE，否则返回FALSE
*/
BOOL CSAMSAssociateAppAddListener(CSAMSAssociateAppOnAction_Fn fnCb, void* pvUserData);

/**
@brief 移除监听回调
@param[in] fnCb 回调函数
*/
void CSAMSAssociateAppRemoveListener(CSAMSAssociateAppOnAction_Fn fnCb);


#ifdef __cplusplus
}
#endif

#endif


