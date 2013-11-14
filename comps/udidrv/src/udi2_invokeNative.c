/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_invokeNative.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_invokeNative.h"
/**
@brief ʵ�ֺ������ù���.

���������� : ����һ��������ַ���ú����Ĳ����б����������������ͣ�ʵ�ָú����ĵ��ù��ܣ������غ���ִ�еķ���ֵ
@param[in] pfFunction �����õĺ���ָ�롣
@param[in] psArgs ��ű����ú��������в�������Ϣ�����CSUDIInvokeArg_S���������˳��{{arg1,type},{arg2,type}, {arg3,type}, ... ,{argn, type}, ...}��
@param[in] nArgsSize �����ú����Ĳ����ĸ�����
@param[in] eReturnType ���ú����ķ���ֵ�����ͣ����CSUDIInvokeValueType_E��
@param[out] psReturnValue ��ű����ú����ķ���ֵ�����CSUDIInvokeValue_S��
@return �ɹ�����CSUDI_SUCCESS, ���򷵻ش������
@note �洢����������ֵ�Ĺ����CSUDIInvokeValueType_E���������߱���һ�¡�
*/
CSUDI_Error_Code CSUDIInvokeNative(void * pfFunction, CSUDIInvokeArg_S * psArgs, int nArgsSize, 
	CSUDIInvokeValueType_E eReturnType, CSUDIInvokeValue_S* psReturnValue)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
