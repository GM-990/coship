/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_input.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_input.h"

/**
@brief 注册输入设备按键回调函数

任何输入设备产生有效的按键信息后，通过注册的回调函数通知应用层\n
@param[in] fnInputCallback 指向回调函数的函数指针，用户通过回调函数得到按键来源、状态和功能码
@param[in] pvUserData 用户数据，可以为CSUDI_NULL
@return 注册成功则返回CSUDI_SUCCESS ;注册失败则返回相关错误代码
@note 
- 支持注册的回调个数至少需支持8个，最大不限
- fnInputCallback和pvUserData唯一标识一个回调类型，当两次注册它们完全一样时，第二次将返回CSUDIINPUT_ERROR_ALREADY_ADDED错误

@note Booter版回调函数可以在中断中调用，上层会按中断使用原则使用回调
*/
CSUDI_Error_Code CSUDIINPUTAddCallback( CSUDIInputCallback_F fnInputCallback ,void * pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}



/**
@brief 删除输入设备按键回调函数

@param[in] fnInputCallback 指向回调函数的函数指针
@param[in] pvUserData 用户数据
@return 删除成功则返回CSUDI_SUCCESS;删除失败则返回相关错误代码
@note 如果要删除一个不存在的回调，则返回CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST
@note fnInputCallback和pvUserData唯一标识一个回调类型

@note Booter版回调函数可以在中断中调用，上层会按中断使用原则使用回调
*/
CSUDI_Error_Code CSUDIINPUTDelCallback(  CSUDIInputCallback_F fnInputCallback ,void * pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

