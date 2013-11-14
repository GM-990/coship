/****************************************************************************/
/*
 * Filename:        cs_osg_plus.c
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

#include "cs_osg_plus.h"

/**
@brief 设置OSD输出屏幕

设置OSD输出屏幕,是高清还是标清
@param[in] unScreen 输出屏幕Flag
@param[in] pvData 属性项值,其内容见CSUDIHDMIPropertyType_E枚举中对各种属性的描述
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
@note unScreen目前可以为EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD或EM_UDI_VOUT_DEVICE_SD|EM_UDI_VOUT_DEVICE_HD
*/
CSUDI_Error_Code CSUDIOSGSetOutputScreen(unsigned int unScreen)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief 注册blit操作的钩子函数

@param[in] eMode 钩子函数的调用模式，见CSUDIOSGBlitCallMode_E定义。
@param[in] fHook 钩子函数，见CSUDIOSGBlitHook_F定义。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 。
@note 
- 只允许注册一个钩子函数
- 通过注册钩子函数为NULL，来注销已注册的钩子函数
*/
CSUDI_Error_Code CSUDIOSGSetBlitHook(CSUDIOSGBlitCallMode_E eMode, CSUDIOSGBlitHook_F fHook)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


