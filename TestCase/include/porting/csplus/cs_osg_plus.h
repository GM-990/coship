/**@defgroup CSUDIOSG PLUS驱动接口
@brief 本模块主要定义了OSG PLUS 接口

@version 0.1 2010/5/29 评审后的第一个版本
@{
*/

#ifndef  _CS_UDIOSGPLUS_H
#define  _CS_UDIOSGPLUS_H

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"
#include "udi2_screen.h"

#ifdef  __cplusplus
extern "C" {
#endif


/**@brief BLIT时注册的钩子函数的调用模式 */
typedef enum {
	EM_UDIOSG_BLITHOOK_CALL_FIRST, ///< 先调用钩子函数，再调用blit函数本身动作
	EM_UDIOSG_BLITHOOK_CALL_LAST  ///< 先调用blit函数本身动作，再调用钩子函数
} CSUDIOSGBlitCallMode_E;


/**
@brief Osg Blit的钩子函数定义
@param[in] pPram 钩子函数的参数，暂时没意义，留作以后扩展
@return CSUDI_SUCCESS 表示钩子函数执行成功，CSUDI_FAILURE表示钩子函数执行失败
*/
typedef CSUDI_Error_Code (* CSUDIOSGBlitHook_F)(void *pPram);


/**
@brief 设置OSD输出屏幕

设置OSD输出屏幕,是高清还是标清
@param[in] unScreen 输出屏幕Flag
@param[in] pvData 属性项值,其内容见CSUDIHDMIPropertyType_E枚举中对各种属性的描述
@return 成功获取则返回CSUDI_SUCCESS；失败则返回CSUDI_FAILURE
@note unScreen目前可以为EM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD或EM_UDI_VOUT_DEVICE_SD|EM_UDI_VOUT_DEVICE_HD
*/
CSUDI_Error_Code CSUDIOSGSetOutputScreen(unsigned int unScreen);


/**
@brief 注册blit操作的钩子函数

@param[in] eMode 钩子函数的调用模式，见CSUDIOSGBlitCallMode_E定义。
@param[in] fHook 钩子函数，见CSUDIOSGBlitHook_F定义。
@return 成功返回 CSUDI_SUCCESS，否则返回错误代码值 。
@note 
- 只允许注册一个钩子函数
- 通过注册钩子函数为NULL，来注销已注册的钩子函数
*/
CSUDI_Error_Code CSUDIOSGSetBlitHook(CSUDIOSGBlitCallMode_E eMode, CSUDIOSGBlitHook_F fHook);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif  /* _CS_UDIOSGPLUS_H */


