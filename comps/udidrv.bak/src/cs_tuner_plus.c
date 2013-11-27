/****************************************************************************/
/*
 * Filename:        cs_tuner_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------

 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_tuner_plus.h"


 /**
@brief Tuner模块初始化

@param[in] uTunerId tuner设备号
@param[in] psTunerInitParam Tuner初始化相关参数配置
@return 
	- 成功和重复初始化返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
	- 参数错误返回CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDITunerInit(unsigned int uTunerId, CSUDITunerInitParam_S *psTunerInitParam)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

 /**
@brief RDI Tuner检测 

@param[in] nPriority 检测线程的优先级  
@return 
	- 成功和重复初始化返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
	- 参数错误返回CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSRDITunerDetect(int nPriority)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}



 /**
@brief Tuner模块反初始化

@param[in] uTunerId tuner设备号
@return 
	- 成功和重复初始化返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
	- 参数错误返回CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDITunerTerm(unsigned int uTunerId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}


 /**
@brief 注册QAM模块相关函数

@param[in] 无
@param[in] psTunerInitParam Tuner初始化相关参数配置
@return 
	- 成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterQam(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}


 /**
@brief 注册COFDM模块相关函数

@param[in] 无
@param[in] psTunerInitParam Tuner初始化相关参数配置
@return 
	- 成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterCofdm(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}


 /**
@brief 注册QPSK模块相关函数

@param[in] 无
@param[in] psTunerInitParam Tuner初始化相关参数配置
@return 
	- 成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterQpsk(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

