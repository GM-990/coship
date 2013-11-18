/** @addtogroup RM
@{
*/
#ifndef _CS_RM_H_
#define _CS_RM_H_

#include "cs_typedef.h"
#include "udi_rm.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**
@brief 设置指定Demux的配置信息

用于指定Demux的一些特性，仅在初始化时调用\n
为保证相同平台的不同硬件(可能有不同的设备数量及特性)使用同样的驱动库而提供的配置接口
@param[in] nDmxIndex 待设置demux设备索引
@param[out] psDemuxInfo Demux配置信息，不可为NULL
@return 成功返回TRUE;失败返回FALSE
@note CSRMGetDeviceCounts返回的Demux数量是根据该接口获取的
@note 设置时必须保证索引从0开始，上层模块会通过遍历0--Demux总数-1的索引获取所有Demux设备的信息
*/
BOOL CSRMSetDemuxConfig(int nDmxIndex,CSRMDemuxConfig_S* psDemuxInfo);

/**
@brief 设置指定Tuner的配置信息

用于指定Tuner的一些特性，仅在初始化时调用\n
为保证相同平台的不同硬件(可能有不同的设备数量及特性)使用同样的驱动库而提供的配置接口
@param[in] nTunerIndex 待查询Tuner设备索引
@param[out] psTunerInfo Tuner配置信息，不可为NULL
@return 成功返回TRUE;失败返回FALSE
@note CSRMGetDeviceCounts返回的Tuner数量是根据该接口获取的
@note 设置时必须保证索引从0开始，上层模块会通过遍历0--Tuner总数-1的索引获取所有Tuner设备的信息
*/
BOOL CSRMSetTunerConfig(int nTunerIndex,CSRMTunerConfig_S* psTunerInfo);

/**
@brief 设置当然是否启动RM模式

启动RM模式时，所有底层设备的操着需以hPath为参数唯一标识一个设备，该模式主要用于MMCP\n
不启动RM模式时，所有底层设备以index为参数唯一标识一个设备，该模式主要兼容USP+GUI项目\n
这是一个为旧项目兼容而提供的Coship专用接口，所有MMCP项目包括MMCP中间层必须使用RM模式
@param[in] bSupport TRUE:启动RM模式；FALSE:不启动RM模式
*/
VOID CSRMSetRMSupport(BOOL bSupport);

/**
@brief 查询当前是否启动RM模块

@return bSupport TRUE:当前已启动RM模式；FALSE:当前未启动RM模式
@see CSRMSetRMSupport
*/
BOOL CSRMIsSupport(void);

/**
@brief 连接某个tuner和demux

@param[in] nTunerNo 欲连接的tuner索引
@param[in] nDemuxNo 欲连接的demux索引
@deprecated 该接口仅为保证DTV模块的兼容而预留，请不要使用，空实现即可。
*/
void CSRMTunerConnDemux(int nTunerNo, int nDemuxNo);//找杨风光确定

#ifdef __cplusplus
}
#endif

/** @} */

#endif
