/** @addtogroup CSTUNER
@{
*/
#ifndef _CS_TUNER_H_
#define _CS_TUNER_H_

#include "cs_typedef.h"
#include "udi_tuner.h"

#ifdef __cplusplus
extern "C" 
{
#endif
    
typedef struct _CSUSPSourceUserCallback
{
	CSTUNCallback  m_SourceCallback;
	void * m_pvUserData;
	int m_nSourceHDITunerNo;
} CSUSPSourceUserCallback;

 /**
@brief 设置本地经纬度

@param[in] dLocLongitude 本地经度，以浮点数传入，比如114.5，传入114.5，东经为负传入-114.5，西经为正
@param[in] dLocLatitude 本地纬度，以浮点数传入，比如22.6，传入22.6，北纬为负传入-22.6，南纬为正
@return 设置成功返回TRUE， 否则返回FALSE
*/
BOOL CSTunerSetPosition(double dLocLongitude, double dLocLatitude);

 /**
@brief 取得当前正在锁定的信号频率

@param[in] hHandle 用于锁频的Tuner设备句柄，由资源关联文档中定义的CSRMAlloc（）或 CSRMConnect()函数返回。具体参考资源关联接口设计文档。
@param[out] pdwFrequency 用来保存获取的信号频率
@return 如果信号锁定，应该返回TRUE,否则FALSE
*/
BOOL CSTunerGetCurrentFreq( HCSHANDLE hHandle, DWORD *pdwFrequency);

 /**
@brief 获取Tuner的信号质量

@param[in] hHandle 用于锁频的Tuner设备句柄，由资源关联文档中定义的CSRMAlloc（）或 CSRMConnect()函数返回。具体参考资源关联接口设计文档。
@return 返回信号质量（0～100）
*/
int CSTunerGetSignalQuality(HCSHANDLE hHandle);

 /**
@brief 获取Tuner的信号强度

@param[in] hHandle 用于锁频的Tuner设备句柄，由资源关联文档中定义的CSRMAlloc（）或 CSRMConnect()函数返回。具体参考资源关联接口设计文档。
@return 返回信号强度（0 ～100）
*/
int CSTunerGetSignalStrength(HCSHANDLE hHandle);

 /**
@brief 获取Tuner的错误率

@param[in] hHandle 用于锁频的Tuner设备句柄，由资源关联文档中定义的CSRMAlloc（）或 CSRMConnect()函数返回。具体参考资源关联接口设计文档。
@return 返回错误率，1*107　个码元中错误码元的个数．(除以107就得到误码率）
*/
int CSTunerGetErrorRate(HCSHANDLE hHandle);

/**
@brief 取得驱动的版本号

@param[in] pchVer 用于存放取得的版本号
@param[out] nSize pchVer的空间长度，以字节为单位
@return 实际版本号的长度,可以为零
*/
int CSTunerGetVersion( PCHAR pchVer, int nSize );

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* SourceInterface_H */


