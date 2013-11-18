/**@defgroup CSDISEQC [CSDISEQC]同洲项目专用DISEQC接口
@brief 本模块主要定义了同洲项目专用平台无关DISEQC驱动接口
@brief  DiSEqC英文为Digital Satellite Equipment Control，直译为“数字卫星设备控制”，是用数字卫星电视接收机控制，发出指令集（控制指令）给相应设备，如切换开关、切换器、天线驱动设备、LNB等。 DiSEqC 事实上是一个控制协议，而不是某种硬件，既然是一个协议就有不同的版本。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.3 2008/11/5  第一次细化版本
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_DISEQC_H_
#define _CS_DISEQC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cs_typedef.h"
#include "CSHDI_typedef.h"

/**
@brief 按指定方向转动指定的时间

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@return CSQPSK_SUCCESS：上电成功;CSQPSK_INVALID_PARAM：参数错误

*/
CSQPSK_Error_t CSDSC10PowerOn ( CSHDITunerIndex bTunerIndex );

/**
@brief 按指定方向转动指定的时间

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@param[in] bDirection 指定方向
@param[in] bTime 需要添加说明
@return CSQPSK_SUCCESS：上电成功；CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12RotateTimes(CSHDITunerIndex bTunerIndex, BYTE bDirection,BYTE bTime);

/**
@brief 按指定方向转动指定的弧度

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@param[in] bDirection 指定方向
@param[in] bCycles 转动弧度
@return CSQPSK_SUCCESS：上电成功；CSQPSK_FAILURE：转动失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12RotateCycles (CSHDITunerIndex bTunerIndex, BYTE bDirection,BYTE bCycles );

/**
@brief 将当前位置保存为第XX个卫星位置

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@param[in] bPosition 当前位置
@return CSQPSK_SUCCESS：上电成功；CSQPSK_FAILURE：失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12StorePosition (CSHDITunerIndex bTunerIndex, BYTE bPosition );

/**
@brief 转到卫星的第XX个位

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@param[in] bPosition 将要转动到的位置
@return CSQPSK_SUCCESS：上电成功；CSQPSK_FAILURE：失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12GotoPosition ( CSHDITunerIndex bTunerIndex, BYTE bPosition );

/**
@brief 转到刻度为0的位

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@return CSQPSK_SUCCESS：成功；CSQPSK_FAILURE：失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12ResetPosition ( CSHDITunerIndex bTunerIndex );

/**
@brief 电机转动直到最东边(最西边)限制位置或接收到STOP命令才停止转动

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@param[in] bDirection 限定位置
@return CSQPSK_SUCCESS：成功；CSQPSK_FAILURE：失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12RotateWithoutStop ( CSHDITunerIndex bTunerIndex, BYTE bDirection );

/**
@brief  停止电机转动

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@return CSQPSK_SUCCESS：成功；CSQPSK_FAILURE：失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12RotateStop (CSHDITunerIndex bTunerIndex);

/**
@brief  取消转动范围的限制(最东边和最西边)

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@return CSQPSK_SUCCESS：成功；CSQPSK_FAILURE：失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12DisableLimit (CSHDITunerIndex bTunerIndex);

/**
@brief  取消转动范围的限制(最东边和最西边)

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@param[in] bDirection 设置当前位置为电机转动的最东边/最西边限制
@return CSQPSK_SUCCESS：成功；CSQPSK_FAILURE：失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12SetLimit ( CSHDITunerIndex bTunerIndex, BYTE bDirection );

/**
@brief 根据第XX个卫星的偏移同步所有卫星的位置

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@param[in] bPosition 当前卫星偏移位置
@return CSQPSK_SUCCESS：成功；CSQPSK_FAILURE：失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12AdjustTAB ( CSHDITunerIndex bTunerIndex, BYTE bPosition );

/**
@brief 转动到指定角度(For STAB USALS Motor)

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@param[in] nAngle_10Multiply 天线方位角(*10>0 East, <0 West)
@return CSQPSK_SUCCESS：成功；CSQPSK_FAILURE：失败;CSQPSK_INVALID_PARAM：参数错误
*/
extern CSQPSK_Error_t CSDSC12GotoSTABPosition  ( CSHDITunerIndex bTunerIndex, int nAngle_10Multiply );

/**
@brief  转动到指定角度(For STAB USALS Motor)

@param[in] sat_longitude 卫星经度
@param[in] my_longitude 本地经度
@param[in] my_latitude 本地纬度
@return 天线角度
*/
double CSDSC12GetAntennaAngle(double sat_longitude, double my_longitude, double my_latitude);

/**
@brief  调整马达到指定的经纬度

@param[in] bTunerIndex tuner的索引：Tuner_0 代表0号tuner；param[in] Tuner_1  代表1号tuner；
@param[in] nSateLongtiude 需要添加说明
@param[in] nBaseLongitude 需要添加说明
@param[in] nLatitude 需要添加说明
@return  需要添加说明
*/
CSQPSK_Error_t CSDSC12TurnSateWithUsals( CSHDITunerIndex bTunerIndex, double nSateLongtiude, double nBaseLongitude, double nLatitude);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


