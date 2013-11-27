/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        cs_fs_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
		 This module is used for the production test application. 
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_fs_plus.h"

/**
@brief FS初始化

@return
- CSHDI_ERROR_ALREADY_INITIALIZED：FS之前已经被初始化过了。
- CSHDI_FAILURE：FS初始化失败。
- CSHDI_SUCCESS：FS初始化成功。
*/
CSUDI_Error_Code CSUDIFSInit(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 向FS中增加一个非热插拔分区,以使FS自动挂载该分区

@param[in] szPartition 分区名 
@param[in] eFsType  分区的类型
@return 
- CSHDI_FAILURE：向FS中增加此分区失败
- CSHDI_SUCCESS：向FS中增加此分区成功。
- CSHDI_ERROR_FEATURE_NOT_SUPPORTED : 不支持的文件系统类型
@note 该函数在CSUDIFSInit后调用，用于告知FS自动挂载指定的分区设备。
   通常FS对于该种设备无法检测到或者无法决定使用。
   例如向FS中加入jffs、cramfs等类型的设备。
*/
CSUDI_Error_Code CSUDIFSAddPartition(const char * szPartition, CSUDIFSPartitionType_E eFsType)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

