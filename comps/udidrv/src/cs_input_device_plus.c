/****************************************************************************/
/*
 * Filename:        cs_input_device_plus.c
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

#include "cs_input_device_plus.h"

/**
@brief Input device module ��ʼ����
@return CSHDI_SUCCESS ��ʾ��ʼ���ɹ���CSHDI_FAILURE��ʾ��ʼ��ʧ�ܡ�
*/
CSHDI_Error_t CSInputDeviceInit(void)
{
    CSHDIComErrType Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@briefע����ꡢ���̡��ֱ���input device �ص�������input device ����������������λ�õ���Ϣͨ���ص��������ݸ�Ӧ��

@param[in] fnCallback ָ��ص������ĺ���ָ�룬�û�ͨ���ص������õ�input device �������ֵ������λ�õ���Ϣ�����CSHDICallback���塣
@return CSHDI_SUCCESS��ע��ɹ�;CSHDI_ERROR_BAD_PARAMETER�������������
@note
*/
CSHDI_Error_t CSInputDeviceRegisterKeySendCallback(CSHDICallback fnCallback)
{
    CSHDIComErrType Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

