/****************************************************************************/
/*
 * Filename:        cs_input_device_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------

 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_input_device_plus.h"

/**
@brief Input device module 初始化。
@return CSHDI_SUCCESS 表示初始化成功，CSHDI_FAILURE表示初始化失败。
*/
CSHDI_Error_t CSInputDeviceInit(void)
{
    CSHDIComErrType Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief注册鼠标、键盘、手柄等input device 回调函数，input device 驱动将按键、坐标位置等信息通过回调函数传递给应用

@param[in] fnCallback 指向回调函数的函数指针，用户通过回调函数得到input device 句柄、键值和坐标位置等信息，详见CSHDICallback定义。
@return CSHDI_SUCCESS：注册成功;CSHDI_ERROR_BAD_PARAMETER：输入参数错误。
@note
*/
CSHDI_Error_t CSInputDeviceRegisterKeySendCallback(CSHDICallback fnCallback)
{
    CSHDIComErrType Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

