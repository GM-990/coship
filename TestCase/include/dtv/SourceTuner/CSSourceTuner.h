#ifndef _CS_SOURCE_TUNER_H_
#define _CS_SOURCE_TUNER_H_

#include "udi2_tuner.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum {
	EM_ST_BEFORE_TUNE,
	EM_ST_AFTER_TUNE
}CSSourceTunerEvent_E;

/**
@brief tuner锁频前回调函数原型定义

@param[in] dwTunerId 用于锁频的Tuner设备的设备号
@param[in] punLastDeliver 上一次锁频的频点信息
@param[in] punNewDeliver 本次锁频的频点信息
@param[in] punLastDeliver 用户自定义数据
*/
typedef  void ( *CSSourceTunerCallback_Fn)(CSSourceTunerEvent_E eEvent, CSUDI_UINT32 dwTunerId, const CSUDITunerSRCDeliver_U* punLastDeliver, const CSUDITunerSRCDeliver_U* punNewDeliver, void *pvUserData);

/**
@brief 根据指定的频点进行锁频操作.

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] punDeliver 用户存放频点信息的数据块的地址。
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回错误代码。\n
-- CSUDITUNER_ERROR_BAD_PARAMETER：参数错误。\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID：非法ID。\n
-- CSUDITUNER_ERROR_I2C_ERR：I2C总线错误。\n
@note 本接口异步执行，返回成功仅表示命令已接收，不表示命令执行结果。
@note 本接口返回后，如果本次锁的频点与上次不同，则必须马上停止掉上个频点的demux数据接收
*/
CSUDI_Error_Code CSSourceTunerConnect(CSUDI_UINT32 dwTunerId, const CSUDITunerSRCDeliver_U* punDeliver);

 /**
@brief 注册Tuner锁频前信息回调

@param[in] fnOnBeforeTune 回调函数句柄。
@param[in] pvUserData 用户传入的自定义指针,用来记录返回给用户的数据信息。
@return 如果成功返回CSUDI_SUCCESS, 失败返回错误代码
@note
- 回调函数的注册个数至少支持8个
*/
CSUDI_Error_Code CSSourceTunerAddCallback(CSSourceTunerCallback_Fn  fnCallback, void *pvUserData );

/**
@brief 	移除回调
@param[in] fnOnBeforeTune  回调函数指针
@return 成功返回CSUDI_SUCCESS,失败则返回错误码值
*/
CSUDI_Error_Code CSSourceTunerRemoveCallback(CSSourceTunerCallback_Fn  fnCallback);

/**
@brief 	设置sourceTuner锁定的频点
@param[in] punDeliver:频点列表
@param[in] nDeliveryCnt: 频点个数
@return 成功返回CSUDI_SUCCESS,失败则返回错误码值
*/
CSUDI_Error_Code CSSSourceTunerSetDeliver(const CSUDITunerSRCDeliver_U* punDeliver, int nDeliveryCnt);

#ifdef __cplusplus
}
#endif

/** @} */

#endif 


