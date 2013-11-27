/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_invokeNative.c
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

#include "udi2_invokeNative.h"
/**
@brief 实现函数调用功能.

本函数功能 : 给定一个函数地址，该函数的参数列表，及各个参数的类型，实现该函数的调用功能，并返回函数执行的返回值
@param[in] pfFunction 被调用的函数指针。
@param[in] psArgs 存放被调用函数的所有参数的信息，详见CSUDIInvokeArg_S，参数存放顺序{{arg1,type},{arg2,type}, {arg3,type}, ... ,{argn, type}, ...}。
@param[in] nArgsSize 被调用函数的参数的个数。
@param[in] eReturnType 调用函数的返回值的类型，详见CSUDIInvokeValueType_E。
@param[out] psReturnValue 存放被调用函数的返回值，详见CSUDIInvokeValue_S。
@return 成功返回CSUDI_SUCCESS, 否则返回错误代码
@note 存储参数及返回值的规则见CSUDIInvokeValueType_E声明，二者保持一致。
*/
CSUDI_Error_Code CSUDIInvokeNative(void * pfFunction, CSUDIInvokeArg_S * psArgs, int nArgsSize, 
	CSUDIInvokeValueType_E eReturnType, CSUDIInvokeValue_S* psReturnValue)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
