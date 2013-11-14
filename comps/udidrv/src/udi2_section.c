/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_section.c
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

#include "udi2_section.h"


/**
@brief ����Filter��������

����������PSI��SI��Ҳ�����ڹ���PES��RAW������,��Ҫ��CSUDIFILTERStart���ʹ��
@param[in] psRequestInfo Filter������������μ�CSUDISECTIONRequestInfo_S
@param[in] fnSectionCallback �ص����������з������������ݵ���ʱ�����ñ�����֪ͨ�����лص�������ͬһ�߳��ҽ��鲻Ҫ�ڵ����߳���ִ��̫�������������μ�CSUDISECTIONCallback_F
@param[in] pvUserData �û����ݣ�����ΪCSUDI_NULL
@param[out] phFilterHandle	���ڽ��շ��ص�Filter���������ֵΪ0��Ϊ�Ƿ�
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note 
- ��ϸ�Ĺ��˹�����ο�CSUDISECTIONRequestInfo_S�ṹ�Ķ���
- ��Ӧ���豸�����CSUDISECTIONRequestInfo_S�ṹ��
- CSUDIFILTERAlloc�ɹ��󣬵���CSUDIFILTERStart��ʼ��������
- CSUDIFILTERAlloc���Զ�ε��ã�ֱ��Filterͨ����ȫ��������Ϊֹ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERAllocate(const CSUDISECTIONRequestInfo_S * psRequestInfo, CSUDISECTIONCallback_F fnSectionCallback, const void * pvUserData ,CSUDI_HANDLE * phFilterHandle)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief ����Filter��������

@param[in] hFilterHandle ��CSUDIFILTERAllocate����õ���filterͨ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note
- Start�ɹ��󣬼���ʼ���ա�������Ӧ����
- ��һ�ε��ú��ٴ�(���)���ý�����ʧ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/    
CSUDI_Error_Code CSUDIFILTERStart(CSUDI_HANDLE hFilterHandle)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}




/**
@brief ֹͣFilter��������,�������ͷ�Filter��Դ

��ֹͣ�����ͷ���Դ
@param[in] hFilterHandle ��CSUDIFILTERAllocate����õ���filterͨ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note
- Stop�ɹ��󣬼�ֹͣ���ա�������Ӧ����
- ��ͼȥֹͣһ��δ������Filter����CSUDISECTION_ERROR_INVALID_STATUS
- ��һ�ε��ú��ٴ�(���)���ý�����ʧ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERStop(CSUDI_HANDLE hFilterHandle )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief ֹͣFilter��������,���ͷ�Filter��Դ

@param[in] hFilterHandle ��CSUDIFILTERAllocate����õ���filterͨ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note
- Free�ɹ��󣬼�ֹͣ���ա�������Ӧ����
- ��һ�ε��ú��ٴ�(���)���ý�����ʧ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERFree(CSUDI_HANDLE hFilterHandle)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief �޸�Fillter����

@param[in] hFilterHandle ��CSUDIFILTERAllocate����õ���filterͨ�����
@param[in] psMMFilter Filter ������� CSUDIFILTERMatchMask_S����,��ΪCSUDI_NULL��ʾ�����Ĵ���
@param[in] bCRCCheck �Ƿ���CRC
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note
- ������Stop״̬�²����޸�filter���������򷵻�CSUDISECTION_ERROR_INVALID_STATUS
- ��һ�ε��ú��ٴ�(���)���ý�����ʧ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERModify(CSUDI_HANDLE hFilterHandle, const CSUDIFILTERMatchMask_S * psMMFilter,  CSUDI_BOOL bCRCCheck)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief ����Fillter�ĵײ�buffer��С

��Ҫ������Ҫ���˴�������,�ϲ���ܻ�������ȡ�ߵ����
@param[in] hFilterHandle ��CSUDIFILTERAlloc����Ĺ��������
@param[in] nBufSize Ҫ���õĵײ�buffer�Ĵ�С������Ϊ2��n�η�����С1K�����ֵ���ݸ���ƽ̨���첻ͬ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����Fillter�ĵײ�buffer��Сʱ������Fillter����stop״̬�����򷵻�CSUDISECTION_ERROR_INVALID_STATUS	

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERSetBufSize(CSUDI_HANDLE hFilterHandle,  int nBufSize)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


