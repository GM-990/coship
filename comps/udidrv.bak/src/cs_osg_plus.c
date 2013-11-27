/****************************************************************************/
/*
 * Filename:        cs_osg_plus.c
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

#include "cs_osg_plus.h"

/**
@brief ����OSD�����Ļ

����OSD�����Ļ,�Ǹ��廹�Ǳ���
@param[in] unScreen �����ĻFlag
@param[in] pvData ������ֵ,�����ݼ�CSUDIHDMIPropertyType_Eö���жԸ������Ե�����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
@note unScreenĿǰ����ΪEM_UDI_VOUT_DEVICE_SD,EM_UDI_VOUT_DEVICE_HD��EM_UDI_VOUT_DEVICE_SD|EM_UDI_VOUT_DEVICE_HD
*/
CSUDI_Error_Code CSUDIOSGSetOutputScreen(unsigned int unScreen)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ע��blit�����Ĺ��Ӻ���

@param[in] eMode ���Ӻ����ĵ���ģʽ����CSUDIOSGBlitCallMode_E���塣
@param[in] fHook ���Ӻ�������CSUDIOSGBlitHook_F���塣
@return �ɹ����� CSUDI_SUCCESS�����򷵻ش������ֵ ��
@note 
- ֻ����ע��һ�����Ӻ���
- ͨ��ע�ṳ�Ӻ���ΪNULL����ע����ע��Ĺ��Ӻ���
*/
CSUDI_Error_Code CSUDIOSGSetBlitHook(CSUDIOSGBlitCallMode_E eMode, CSUDIOSGBlitHook_F fHook)
{
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


