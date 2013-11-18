/******************************************************************************
FileName:  cs_pvrmgrnotif.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

文件描述:
定义了消息通知管理接口
支持一对多的通知，并且是线程安全的
请不要在消息响应函数里堵塞线程

修改记录:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      罗金华           Create

*******************************************************************************/
#ifndef _CSPM_NOTIFYMGR_H_
#define _CSPM_NOTIFYMGR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**
	Notify Manager的通知响应函数形式
@param [in] dwNotifyType 通知的类型，具体的通知类型定义由各个Service模块定义
@param [in] pNotifyData  通知的数据
@param [in] pUserData 用户Client自定义数据，在Client注册回调响应函数时参数传入的
@param [in] hSender 发出通知的Serice对象的句柄
*/
typedef int (*CSNOTIFYOnNotify_F)(DWORD dwNotifyType, void *pNotifyData, void* pUserData, HCSHANDLE hSender);

/**
	创建一个消息通知管理器
@return 消息通知管理器的句柄
@note 仅由Service调用
*/
HCSHANDLE CSNOTIFYCreate(void);

/**
	发出消息通知所有客户端
@param [in] hNotifyMgr 消息通知管理器句柄
@param [in] dwNotifyType 通知的类型，具体的通知类型定义由各个Service模块定义
@param [in] pNotifyData  通知的数据
@param [in] hSender 发出通知的Serice对象的句柄
\note 仅service调用
*/
void CSNOTIFYNotifyClient(HCSHANDLE hNotifyMgr, DWORD dwNotifyType, void *pNotifyData,HCSHANDLE hSender);

/**
	销毁消息通知管理器
@param [in] hNotifyMgr 消息通知管理器句柄
\note 仅service调用
*/
void CSNOTIFYDestroy(HCSHANDLE hNotifyMgr);

/**
	添加消息响应函数
@param [in] hNotifyMgr 消息通知管理器句柄
@param [in] fnOnCallbackMgrEvent 该事件对应的回调函数
@param [in] pUserData 用户自定义参数，将在回调中原封不动的传回给客户端

\note 仅由Client调用
\同一个消息响应函数可以添加多次，则一次通知时，该回调响应函数也会被调用多次
*/
BOOL CSNOTIFYAddNotifyCb(HCSHANDLE hNotifyMgr, CSNOTIFYOnNotify_F fnCallback, void* pUserData);

/**
	移除消息响应函数
@param [in] hNotifyMgr 消息通知管理器句柄
@param [in] fnOnCallbackMgrEvent 该事件对应的回调函数
@param [in] pUserData 用户Client自定义数据，在Client注册回调响应函数时参数传入的

\note 仅由客户端调用，但客户端的Destroy函数中，比如类析构函数中不能调用
\需要与Add函数配对出现，对同以各消息响应函数，Add了几次则需要Remove几次
*/
BOOL CSNOTIFYRemoveNotifyCb(HCSHANDLE hNotifyMgr, CSNOTIFYOnNotify_F fnCallback, void* pUserData);

/**
	删除消息响应函数
@param [in] hNotifyMgr 消息通知管理器句柄
@param [in] fnOnCallbackMgrEvent 该事件对应的回调函数
@param [in] pUserData 用户Client自定义数据，在Client注册回调响应函数时参数传入的

\note 仅由客户端调用，仅在客户端的Destroy函数中，比如类析构函数中调用
\not 消息响应函数被Add多次时，调用本函数一次即可删除
*/
BOOL CSNOTIFYDelClient(HCSHANDLE hNotifyMgr, CSNOTIFYOnNotify_F fnCallback, void* pUserData);

#ifdef __cplusplus
}
#endif

#endif

