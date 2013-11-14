/****************************************************************************/
/*
 * Filename:        cs_uti.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_uti.h"

/**
@brief UTI模块初始化

@param[in] uUtiIndex  UTI设备索引
@param[in] psUtiInitParam UTI初始化相关参数配置
@return 
	- 成功和重复初始化返回CSUDI_SUCCESS
	- 失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUTIInit(unsigned int uUtiIndex, const CSUTIInitParam_S *psUtiInitParam)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

 /**
@brief UTI模块反初始化

@param[in] uUtiIndex  UTI设备索引
@return 
	- 成功和重复反初始化返回CSUDI_SUCCESS
	- 失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSUTITerm(unsigned int uUtiIndex)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


