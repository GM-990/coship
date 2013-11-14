/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

/**@defgroup TUNER TUNER 定义了统一的tuner初始化相关接口操作函数
@brief 本模块主要定义了统一的tuner初始化相关接口操作函数
@brief 统一的tuner接口支持DVB-C标准(QAM调制方式),DVB-S/DVB-S2标准(QPSK,8PSK调制方式),DVB-T标准(COFDM和DMB-TH调制方式)
@{
*/

#ifndef _UDI2_TUNER_PLUS_H_
#define _UDI2_TUNER_PLUS_H_

#ifdef __cplusplus
extern "C" 
{
#endif
	
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "cs_hdicommon_plus.h"



typedef enum
{
    EM_UDITUNER_NAME_NONE,                      ///<自动识别tuner
    ///< 有线tuner类型
    EM_UDITUNER_CAB_CU1216_TDA10021,     ///< CU1216+TDA10021
    EM_UDITUNER_CAB_CU1216_TDA10023,     ///< CU1216+TDA10023

    ///<地面tuner类型
    EM_UDITUNER_TER_TD1611_MT353,         ///< TD1611+MT353
    EM_UDITUNER_TER_TD1611_MT352,         ///< TD1611+MT352

    /// 卫星tuner类型
    EM_UDITUNER_SAT_0165
}CSUDITunerName_E;



typedef struct 
{
	unsigned int m_uI2CIndex;                             ///< I2C索引
	unsigned int m_uResetGPIONum;                   ///< demodulator reset gpio 索引
	CSHDITsMode m_eTSOutMode;                       ///< TS流输出模式
	int               m_nPriority;                                  ///< tuner模块线程优先级
	CSUDITunerName_E m_eTunerName;                  ///< tuner型号
}CSUDITunerInitParam_S;



 /**
@brief Tuner模块初始化

@param[in] uTunerId tuner设备号
@param[in] psTunerInitParam Tuner初始化相关参数配置
@return 
	- 成功和重复初始化返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
	- 参数错误返回CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDITunerInit(unsigned int uTunerId, CSUDITunerInitParam_S *psTunerInitParam);


 /**
@brief RDI Tuner检测 

@param[in] nPriority 检测线程的优先级  
@return 
	- 成功和重复初始化返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
	- 参数错误返回CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSRDITunerDetect(int nPriority);



 /**
@brief Tuner模块反初始化

@param[in] uTunerId tuner设备号
@return 
	- 成功和重复初始化返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
	- 参数错误返回CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDITunerTerm(unsigned int uTunerId);


 /**
@brief 注册QAM模块相关函数

@param[in] 无
@param[in] psTunerInitParam Tuner初始化相关参数配置
@return 
	- 成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterQam(void);


 /**
@brief 注册COFDM模块相关函数

@param[in] 无
@param[in] psTunerInitParam Tuner初始化相关参数配置
@return 
	- 成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterCofdm(void);


 /**
@brief 注册QPSK模块相关函数

@param[in] 无
@param[in] psTunerInitParam Tuner初始化相关参数配置
@return 
	- 成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterQpsk(void);


#ifdef __cplusplus
}
#endif

/** @} */

#endif
