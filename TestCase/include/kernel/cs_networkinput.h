/**
Copyright (c) 2012,ShenZhen Coship Electronic Ltd Co.
All rights reserved.

File Name: cs_networkinput.h
Summary: 机顶盒接收网络遥控器的控制
Current Version: 0.0.1
Author: gongxing

History:
0.0.1 2012/05/07 初稿，参照UDI2模块中遥控注册、删除按键回调的方式定义好的接口，继承UDI2的按键处理
*/

#ifndef _CS_NETWORK_INPUT_H_
#define _CS_NETWORK_INPUT_H_

#include "udi2_input.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
@brief 注册网络按键回调函数

网络按键的键值信息会通过注册的回调函数通知应用层\n
@param[in] fnInputCallback 指向回调函数的函数指针，用户通过回调函数得到按键来源、状态和功能码
@param[in] pvUserData 用户数据，可以为CSUDI_NULL
@return 注册成功则返回CSUDI_SUCCESS ;注册失败则返回相关错误代码
@note 
- 支持注册的回调个数最少需支持8个，最大不限
- 达到最大回调个数，将返回CSUDIINPUT_ERROR_CALLBACK_FULL错误
- fnInputCallback和pvUserData唯一标识一个回调类型，当两次注册它们完全一样时，第二次将返回CSUDIINPUT_ERROR_ALREADY_ADDED错误

@note Booter版回调函数可以在中断中调用，上层会按中断使用原则使用回调
*/
CSUDI_Error_Code CSNetworkInputAddCallback(CSUDIInputCallback_F fnInputCallback ,void * pvUserData);

/**
@brief 删除网络按键回调函数

@param[in] fnInputCallback 指向回调函数的函数指针
@param[in] pvUserData 用户数据
@return 删除成功则返回CSUDI_SUCCESS;删除失败则返回相关错误代码
@note 如果要删除一个不存在的回调，则返回CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST
@note fnInputCallback和pvUserData唯一标识一个回调类型

@note Booter版回调函数可以在中断中调用，上层会按中断使用原则使用回调
*/
CSUDI_Error_Code CSNetworkInputDelCallback(CSUDIInputCallback_F fnInputCallback ,void * pvUserData);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

