/**@defgroup DISEQC DISEQC 定义了统一的diseqc udi2.0接口操作函数
@brief DiSEqC模块对应头文件<udi2_diseqc.h>，定义了DiSEqC的常用操作接口。
支持diseqc1.0/1.1/1.2/1.3标准

@version 1.1.0 2012/1/5 增加接口CSUDIDSC12GotoSTABPosition和CSUDIDSC12GetAntennaAngle
@version 1.0.1 2011/12/28 第一版本
@{
*/

#ifndef _UDI2_DISEQC_H_
#define _UDI2_DISEQC_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum 
{
	EM_UDIDISEQC_EASTERN,	/**< 向东转动*/
	EM_UDIDISEQC_WESTERN	/**< 向西转动*/
}CSUDIRotateDirection_E;

typedef struct 
{
	double ldSatLongitude; /**< 卫星经度*/
	double ldLongitude;    /**< 指定经度*/
	double ldLatitude;     /**< 指定纬度*/
	int    reserved[2];	   /**< 保留参数*/
}CSUDISatLocationInfo_S;

/**
@brief 按指定方向转动指定的时间

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] eDirection 设置转动方向
@param[in] ucTime (s) 设置转动时间(秒钟)
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12RotateTimes(CSUDI_UINT32 dwTunerId, CSUDIRotateDirection_E eDirection, CSUDI_UINT8 ucTime);

/**
@brief 按指定方向转动指定的弧度

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] eDirection 指定方向
@param[in] ucCycles 转动弧度
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12RotateCycles (CSUDI_UINT32 dwTunerId, CSUDIRotateDirection_E eDirection,CSUDI_UINT8 ucCycles);

/**
@brief 将当前位置保存为第XX个卫星位置

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] ucPosition 当前位置
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12StorePosition (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition);

/**
@brief 转到卫星的第XX个位

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] ucPosition 将要转动到的位置
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12GotoPosition (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition);

/**
@brief 转到刻度为0的位

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12ResetPosition (CSUDI_UINT32 dwTunerId);

/**
@brief 电机转动直到最东边(最西边)限制位置或接收到STOP命令才停止转动

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] ucDirection 限定位置
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12RotateWithoutStop (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucDirection);

/**
@brief  停止电机转动

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12RotateStop (CSUDI_UINT32 dwTunerId);

/**
@brief  取消转动范围的限制(最东边和最西边)

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12DisableLimit (CSUDI_UINT32 dwTunerId);

/**
@brief  取消转动范围的限制(最东边和最西边)

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] bDirection 设置当前位置为电机转动的最东边/最西边限制
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12SetLimit (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucDirection);

/**
@brief 根据第XX个卫星的偏移同步所有卫星的位置

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] bPosition 当前卫星偏移位置
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12AdjustTAB (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition);

/**
@brief 转动到指定角度(For STAB USALS Motor)

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] dlAngle 天线方位角(>0 East, <0 West)
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12GotoSTABPosition(CSUDI_UINT32 dwTunerId, double dlAngle);

/**
@brief  转动到指定角度(For STAB USALS Motor)

@param[in]  dlSatLongitude 卫星经度
@param[in]  dlMyLongitude  本地经度
@param[in]  dlMyLatitude   本地纬度
@param[out] pdlAngle       存储天线转动角度
@return CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12GetAntennaAngle(double dlSatLongitude, double dlMyLongitude, double dlMyLatitude, double *pdlAngle);

/**
@brief  调整马达到指定的经纬度

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] psSatLocationInfo,设置马达指定的经纬度信息
@return  CSUDI_SUCCESS：成功；失败返回错误代码
*/
CSUDI_Error_Code CSUDIDSC12TurnSateWithUsals(CSUDI_UINT32 dwTunerId, const CSUDISatLocationInfo_S *psSatLocationInfo);

#ifdef __cplusplus
}
#endif

#endif

