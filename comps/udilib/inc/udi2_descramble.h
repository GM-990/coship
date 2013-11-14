/**@defgroup DESCRAMBLE DESCRAMBLE 数据解扰相关接口定义
@brief DESCRAMBLE模块对应头文件<udi2_descramble.h>，包括解扰器相关的接口。

所有解扰器操作接口（如SetPID）的实现若要求必须先打开相关通道（如请求该PID下的数据、播放该PID下的节目）是可以接受的，MMCP（CloudTV ）中间件平台在使用中会保证先打开该PID通道，然后对该通道进行解扰相关操作。

@note 因为NDS CA使用ICAM模块操作解扰器，所以如果为NDS产品，则需遵循NDS规范使用芯片厂商提供的ICAM模块，而无需实现本模块 。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.1 2009/8/17 
@{
*/

#ifndef _UDI2_DESCRAMBLE_H_
#define _UDI2_DESCRAMBLE_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif 

/**@brief 解扰相关错误代码值*/
enum
{
	CSUDIDSM_ERROR_BAD_PARAMETER = CSUDI_DESCRAMBLE_ERROR_BASE,		///< 错误参数
	CSUDIDSM_ERROR_INVALID_HANDLE,						///< 非法句柄
	CSUDIDSM_ERROR_INVALID_DEVICE_ID,					///< 非法设备ID
	CSUDIDSM_ERROR_INVALID_STATUS,						///< 非法状态
	CSUDIDSM_ERROR_FEATURE_NOT_SUPPORTED,				///< 不支持此操作
	CSUDIDSM_ERROR_NO_FREE_DESCRAMBLER,					///< 没有解扰通道
	CSUDIDSM_ERROR_NO_MEMORY							///< 没有可用内存
};

/**
@brief 分配一个解扰通道，返回解扰通道句柄

@param[in] nDemuxIndex  demux设备的索引
@param[out] phDSM 用于接收返回解扰通道句柄
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pHandle的内容未定义
@note 如果已经分配完所有解扰通道，此接口返回CSUDIDSM_ERROR_NO_FREE_DESCRAMBLER。
@note Eastwin会将CSUDI_NULL当做一个非法的句柄，请在实现时特别注意所有Handle的取值范围
*/
CSUDI_Error_Code CSUDIDSMAllocate(int nDemuxIndex, CSUDI_HANDLE * phDSM);

/**
@brief 释放一个解扰通道

@param[in] hDSM 解扰通道的句柄，由CSUDIDSMAllocate返回得到
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
*/ 
CSUDI_Error_Code CSUDIDSMFree(CSUDI_HANDLE hDSM );

/**
@brief 为解扰的通道指定过滤的PID

@param[in] hDSM 解扰通道句柄
@param[in] ushPID 解扰PID
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置解扰PID前，要求先打开该PID通道(如请求该PID下的数据、播放该PID下的节目)是可以接受的，使用中亦需要注意该顺序问题
*/ 
CSUDI_Error_Code CSUDIDSMSetPID(CSUDI_HANDLE hDSM,  unsigned short ushPID );

/**
@brief 清空解扰通道指定的过滤PID

@param[in] hDSM 解扰通道句柄
@param[in] ushPID 解扰PID
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
*/ 
CSUDI_Error_Code CSUDIDSMClearPID(CSUDI_HANDLE hDSM, unsigned short ushPID );

/**
@brief 设置解扰通道的奇Key值

@param[in] hDSM 解扰通道句柄
@param[in] pucOddKey 解扰的奇KEY
@param[in] ucOddLength Key长度
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设key前，要求先打开该PID通道(如请求该PID下的数据、播放该PID下的节目)是可以接受的
*/ 
CSUDI_Error_Code CSUDIDSMSetOddKey(CSUDI_HANDLE hDSM, const unsigned char * pucOddKey, unsigned char ucOddLength);

/**
@brief 设置解扰通道的偶Key值

@param[in] hDSM 解扰通道句柄
@param[in] pucEvenKey 解扰的偶KEY
@param[in] ucEvenLength Key长度
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设key前，要求先打开该PID通道(如请求该PID下的数据、播放该PID下的节目)是可以接受的
*/ 
CSUDI_Error_Code CSUDIDSMSetEvenKey(CSUDI_HANDLE hDSM, const unsigned char * pucEvenKey, unsigned char ucEvenLength);



#ifdef  __cplusplus
}
#endif
/** @} */
#endif
