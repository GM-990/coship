/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_uart.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_uart.h"
/**
@brief ��ȡ��ƽ̨֧�ֵĴ��ڸ���
@param[out] pdwUartDeviceCount  ��ƽ̨֧�ֵĴ��ڸ���
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIUartGetCount(unsigned int* pdwUartDeviceCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief ��ȡָ�����ڵ�����
@param[in] nUartIndex ��������ֵ
@param[out] psUartAttribute  ���洮�����Եĵ�ַ������ṹ�ο�CSUDIUartAttribute_S���ݽṹ��
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIUartGetAttribute(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief ����ָ�����ڵ�����
@param[in] nUartIndex ��������ֵ
@param[in] psUartAttribute  �������Եĵ�ַ������ṹ�ο�CSUDIUartAttribute_S���ݽṹ��
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIUartSetAttribute(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief ��uart�ӿڶ�ȡ���uLength�ֽ�����
@param[in] nUartIndex Ҫ�����Ĵ���������
@param[in] pucBuffer ������Ŷ������ݵĻ��档
@param[in] uLength ���������ȡ�
@param[in] nTimeout ��ʱ��ʱ�䣬�����ַ�֮��ĳ�ʱʱ�䣬��λ���롣����nTimeoutδ�յ����ݱ�ʾ��ʱ��
@return ����ʵ�ʶ��������ݵĳ��ȣ�0��ʾû�ж����κ����ݡ�
@note ��������ʱ���߻�����������֮�󣬸ýӿڷ��ء�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
int CSUDIUartRead(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief ��uart�ӿڷ������uLength�ֽ�����

@param[in] nUartIndex Ҫ�����Ĵ���������
@param[in] pucBuffer ���Ҫ�������ݵĻ��档
@param[in] uLength Ҫ�������ݵĳ��ȡ�
@param[in] nTimeout ��ʱ��ʱ�䣬�����ַ�֮��ĳ�ʱʱ�䣬��λ���롣����nTimeoutδ�յ����ݱ�ʾ��ʱ��
@return ����ʵ�ʷ��͵����ݵĳ��ȣ�0��ʾû�з����κ����ݡ�
@note ��������ʱ���߻���������ȫ�����ͳ��󣬸ýӿڷ��ء�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
int CSUDIUartWrite(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief ����Ѿ���uart�յ����д��ڻ�������û�б�CSUDIUartRead���ߵ�����

@param[in] nUartIndex Ҫ�����Ĵ���������

@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIUartClear(int nUartIndex)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

