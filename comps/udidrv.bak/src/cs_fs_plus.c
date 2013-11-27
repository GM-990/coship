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
@brief FS��ʼ��

@return
- CSHDI_ERROR_ALREADY_INITIALIZED��FS֮ǰ�Ѿ�����ʼ�����ˡ�
- CSHDI_FAILURE��FS��ʼ��ʧ�ܡ�
- CSHDI_SUCCESS��FS��ʼ���ɹ���
*/
CSUDI_Error_Code CSUDIFSInit(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief ��FS������һ�����Ȳ�η���,��ʹFS�Զ����ظ÷���

@param[in] szPartition ������ 
@param[in] eFsType  ����������
@return 
- CSHDI_FAILURE����FS�����Ӵ˷���ʧ��
- CSHDI_SUCCESS����FS�����Ӵ˷����ɹ���
- CSHDI_ERROR_FEATURE_NOT_SUPPORTED : ��֧�ֵ��ļ�ϵͳ����
@note �ú�����CSUDIFSInit����ã����ڸ�֪FS�Զ�����ָ���ķ����豸��
   ͨ��FS���ڸ����豸�޷���⵽�����޷�����ʹ�á�
   ������FS�м���jffs��cramfs�����͵��豸��
*/
CSUDI_Error_Code CSUDIFSAddPartition(const char * szPartition, CSUDIFSPartitionType_E eFsType)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

