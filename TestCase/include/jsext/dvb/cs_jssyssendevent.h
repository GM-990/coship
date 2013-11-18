/**
Copyright (c) 2005,ShenZhen Coship Electronic Ltd Co.
All rights reserved.

File Name: cs_jssyssendevent.h
Summary: 广东省网消息监听头文件
Current Version: 1.0
Author: Zheng Fen

History:
1.0.1 2012/04/11 1初稿
*/
#ifndef _CS_SYSSENDEVENT_H_
#define _CS_SYSSENDEVENT_H_


#ifdef  __cplusplus
extern "C" {
#endif

/**
@brief 调用CSJSSysSendEventAddCallBack和CSJSSysSendEventRemoveCallBack的时候, 需要传入此类型的函数指针
@param[in] dwMessageType:发送的消息的类型，取值范围参见JSMessage.h头文件定义
@param[in]  caJsonStr：与消息类型相应的消息的附属信息，以json字符串的形式表示，如果不需要此参数，则传0
@param[in] nLength：json字符串的长度
@param[in] pvUserData:用来保存用户数据
*/
typedef void  (*CSOnJsSysSendEvent_F)(DWORD dwMessageType, const char* caJsonStr, int nLength, void * pvUserData);

/**
@brief 添加js消息通知回调函数

@param[in] pCallback 回调函数,当应用调用SysSetting.sendEvent时，会触发该回调
@param[in] pvUserData 用户数据,将作为回调函数的pvUserData参数返回
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
@note 添加回调前产生的状态变化不会补充通知
*/
int CSJSSysSendEventAddCallBack(CSOnJsSysSendEvent_F pCallBack, void * pvUserData);

/**
@brief 删除js消息通知回调函数

@param[in] pCallback 回调函数,之前注册的函数
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回CSUDI_FAILURE
*/
int CSJSSysSendEventRemoveCallBack(CSOnJsSysSendEvent_F pCallBack);
#ifdef __cplusplus
}
#endif


#endif

