/**@defgroup CSENCODER[CSENCODER]同洲项目专用ENCODER驱动接口
@brief 本模块主要定义了同洲项目专用平台无关ENCODER驱动接口
@brief 不知道是个什么东东。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.3 2008/11/5  第一次细化版本
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/


#ifndef _CS_ENCODER_H_
#define _CS_ENCODER_H_

#include "cs_typedef.h"
#include "CSHDI_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif


/**@brief ENCODER Format定义*/
typedef enum
{
	CSVENC_PAL_BDGHI,					/**< 需要添加说明 */
	CSVENC_PAL_COMBINATION_N,	/**< 需要添加说明 */
	CSVENC_PAL_M,							/**< 需要添加说明 */
	CSVENC_PAL_60,						/**< 需要添加说明 */
	CSVENC_NTSC_M,						/**< 需要添加说明 */
	CSVENC_NTSC_443,					/**< 需要添加说明 */
	CSVENC_SECAM_BGLK1				/**< 需要添加说明 */
} T_CSVENC_FORMAT;

/**
@brief 执行ENCODER初始化。

@param[in] enmI2CIndex 器件所在的I2C总线的索引
@param[in] bTTXFlag 是否支持图文电视功能
@return
- CSHDI_SUCCESS:	初始化成功
- CSHDI_FAILURE：	初始化失败
*/
CSHDI_Error_t CSENCInit(CSHDII2CIndex enmI2CIndex, BOOL bTTXFlag);

/**
@brief 激活ENCODER设备。

@return CSHDI_SUCCESS表示激活成功， CSHDI_FAILURE表示激活失败。
*/
CSHDI_Error_t CSENCEnable(void);

/**
@brief 设置ENCODER解码格式。

@param[in] enmVENCFormat ENCODER解码输出的格式参数。
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSENCSetEncoderFormat(T_CSVENC_FORMAT  enmVENCFormat);

/**
@brief 打开图文电视功能

@return CSHDI_SUCCESS表示打开成功，CSHDI_FAILURE表示打开失败。
*/
CSHDI_Error_t CSENCOpenTeletext(void);

/**
@brief 关闭图文电视功能

@return  success:CSHDI_SUCCESS表示关闭成功，fail:CSHDI_FAILURE表示关闭失败。
*/
CSHDI_Error_t CSENCCloseTeletext(void);

/*
@brief 设置ENCDOER进入standby 模式

@return
- success:CSHDI_SUCCESS表示设置成功，
- fail:CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSENCStandBy (void);

/**
@brief 关闭ENCODER

@return 
- success:CSHDI_SUCCESS表示关闭成功，
- fail:CSHDI_FAILURE表示关闭失败。
*/
CSHDI_Error_t CSENCDisable(void);

/**
@brief 激活颜色条功能

@return
- success:CSHDI_SUCCESS表示操作成功，
- fail:CSHDI_FAILURE表示操作失败。
*/
CSHDI_Error_t CSENCEnableColorBar(void);

/**
@brief 关闭颜色条功能

@return
- success:CSHDI_SUCCESS表示关闭成功，
- fail:CSHDI_FAILURE表示关闭失败。
*/
CSHDI_Error_t CSENCDisableColorBar(void);

/**
@brief 取得该模块驱动的版本号

@param[out] pcVer The top address of Version Buffer
@param[in] nSize 是否支持图文电视功能
@return  success:CSHDI_SUCCESS表示关闭成功， fail:CSHDI_FAILURE表示关闭失败。
*/
int CSENCGetVersion(PCHAR pcVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

