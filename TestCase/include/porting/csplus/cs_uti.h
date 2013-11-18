/**@defgroup UTI 即Universal Transport Interface，是数字太和公司利用解密芯片代替
SmartCard提供的统一接口，用于对UTI芯片的寄存器进行读写操作

@brief 本模块主要定义了统一的UTI初始化与反初始化接口操作函数

@{
*/

#ifndef _CS_UTI_H_
#define _CS_UTI_H_

#ifdef __cplusplus
extern "C" 
{
#endif

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "cs_hdicommon_plus.h"

typedef enum
{
    EM_UTI_TYPE_1201,
    EM_UTI_TYPE_1203,
    EM_UTI_TYPE_MAX
}CSUTIType_E;


typedef struct 
{
	CSUTIType_E				m_eUtiType;			///< UTI型号
	unsigned int			m_uI2CIndex;		///< UTI I2C索引
	unsigned int			m_uResetGPIONum;	///< UTI reset gpio 索引
	CSHDITsMode				m_eTSOutMode;		///< TS流输出模式
	unsigned int			m_dwReserved0;		///< 保留参数0
	unsigned int			m_dwReserved1;		///< 保留参数1
}CSUTIInitParam_S;

/**
@brief UTI模块初始化

@param[in] uUtiIndex  UTI设备索引
@param[in] psUtiInitParam UTI初始化相关参数配置
@return 
	- 成功和重复初始化返回CSUDI_SUCCESS
	- 失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUTIInit(unsigned int uUtiIndex, const CSUTIInitParam_S *psUtiInitParam);


 /**
@brief UTI模块反初始化

@param[in] uUtiIndex  UTI设备索引
@return 
	- 成功和重复反初始化返回CSUDI_SUCCESS
	- 失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUTITerm(unsigned int uUtiIndex);


#ifdef __cplusplus
}
#endif

/** @} */

#endif




