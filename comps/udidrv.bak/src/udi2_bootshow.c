/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_bootshow.c
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

#include "udi2_bootshow.h"
 /**
@brief ��ȡbooter����ʾ��ص�����
@param[out] psCapability ��ȡbooter����ʾ����������ṹ�嶨��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ����ṹ��CSUDIBOOTSHOWCapability����
@note �ýӿڸ�����Ŀ��Ҫ��ѡʵ��
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetCapability(CSUDIBOOTSHOWCapability* psOutCapability)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ���booter��ʾ�õ����Ѵ洢��ÿ����ʾ��Ԫ����Ϣ
@param[in] nIndex �洢��ʾ��Ԫ��������ȡֵΪ0--�Ѵ洢��ʾ��Ԫ����-1��
@param[out] psOutInfo ��ʾ��Ԫ��Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetShowUnitInfo(int nIndex,CSUDIBOOTSHOWUnitInfo* psOutInfo)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
/**
@brief ��ȡbooter���ڴ洢����ʾ��Ԫ���ݵ�flashʣ��ռ��С
@param[out] pnOutSize ʣ��ռ��С����λΪBYTE
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �ɴ洢��ʾ���ݴ�С = ʣ��ռ��С - 1����ʾ��Ԫ��Ϣͷ����С
@note ʣ��ռ�Ϊflash��ַ������ʣ����ÿռ��С��
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetFreeSize(unsigned int* pnOutSize)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡboot�Ѵ洢����������ʾ��Ԫ����
@param[out] pnOutCount �Ѵ洢����ʾ��Ԫ����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 
*/
CSUDI_Error_Code CSUDIBOOTSHOWGetUnitCount(unsigned int* pnOutCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����booter��ʾĳ��ָ�����Ѵ洢������ʾ��Ԫ
@param[in] nIndex ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С���Ѵ洢��ʾ��Ԫ������
@param[in] bNeedShow �Ƿ���Ҫ��ʾ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note ����ĳ����ʾ��Ԫ��ʾ��������������ʾ��Ԫ������ʾ������ֻ��ʾһ����ʾ��Ԫ
@note ����Զ����Ԫ�������˸ýӿڣ�����index��С�ĵ�ԪΪ׼����������Ԫ��ʱ��������ʾҲ����Ч��
*/
CSUDI_Error_Code CSUDIBOOTSHOWSetNeedShow(int nIndex, CSUDI_BOOL bNeedShow)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����booter��ʾ�õ���ÿ����ʾ��Ԫ����Ϣ
@param[in] nIndex ��ʾ��Ԫ��������ȡֵΪ���ڵ���0��С�ڵ����Ѵ洢��ʾ��Ԫ������
@param[in] psInfo ��ʾ��Ԫ��Ϣ
@param[in] pData  ��ʾ��Ԫ��Ϣ��Ӧ������������ʼ��ַ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �������==�Ѵ洢��ʾ��Ԫ���������ʾ����һ����ʾ��Ԫ��
@note ����޸ĺ����ʾ��Ԫ��С���޸�ǰ�Ĳ�һ��������Ҫ����flash�������·���Ȳ�����
@note ĳЩ��ʾ��Ԫ�����ݿ����ǹ̶��ģ��������޸ġ���ʱ������CSUDIBOOTSHOWSetShowUnitInfo���ش���
*/
CSUDI_Error_Code CSUDIBOOTSHOWSetShowUnitInfo(int nIndex, CSUDIBOOTSHOWUnitInfo* psInfo,void* pData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

