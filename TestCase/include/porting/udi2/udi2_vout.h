/**@defgroup VOUT VOUT 定义视频输出设备控制接口
@brief 本模块主要定义了视频输出设备控制接口

@version 2.0.0 2009/12/29 初稿
@{
*/
#ifndef _UDI2_VOUT_H_
#define _UDI2_VOUT_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief VOUT错误代码值*/
enum
{
	CSUDIVOUT_ERROR_BAD_PARAMETER = CSUDI_VOUT_ERROR_BASE,  ///< 错误参数
	CSUDIVOUT_ERROR_INVALID_DEVICE_ID,						///< 非法设备ID
	CSUDIVOUT_ERROR_FEATURE_NOT_SUPPORTED					///< 不支持此操作
};

/**@brief 视频输出设备类型*/
typedef enum
{
	EM_UDIVOUT_CVBS,	/**< CSBVS视频输出接口 */
	EM_UDIVOUT_YC,	/**< S-Video 视频输出接口 */
	EM_UDIVOUT_YUV, /**< 分量视频输出接口 */
	EM_UDIVOUT_RF, /**< 射频信号视频输出接口 */
	EM_UDIVOUT_SCART, /**< SCART视频输出接口 */
	EM_UDIVOUT_HDMI, /**< HDMI视频输出接口 */
	EM_UDIVOUT_DVO,  /**< 数字视频输出*/
	EM_UDIVOUT_NUM /**< 视频接口个数 */
}CSUDIVOUTDevices_E;

/**@brief 视频输出端口的颜色类型 */
typedef enum
{
	EM_UDIVOUT_OUTPUT_TYPE_YUV, /**< YUV模式 */
	EM_UDIVOUT_OUTPUT_TYPE_RGB, 	/**< RGB模式 */
	EM_UDIVOUT_OUTPUT_TYPE_NUM	 /**< 模式计数 */
}CSUDIVOUTOutputType_E;

/**
@brief 打开指定的视频输出设备

@param[in] eVoutDevice  视频输出设备，具体请参见CSUDIVOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVOUTEnable(CSUDIVOUTDevices_E eVoutDevice);

/**
@brief 关闭指定的视频输出设备

@param[in] eVoutDevice	视频输出设备，具体请参见CSUDIVOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIVOUTDisable(CSUDIVOUTDevices_E eVoutDevice);

/**
@brief 设置视频输出端口的颜色类型

@param[in] eVoutDevice 视频输出设备,只支持分量(EM_UDIVOUT_YUV)/HDMI(EM_UDIVOUT_HDMI)
@param[in] eType 输出的颜色类型，RGB或是YUV
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置立即生效
*/
CSUDI_Error_Code CSUDIVOUTSetOutputType(CSUDIVOUTDevices_E eVoutDevice, CSUDIVOUTOutputType_E eType);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

