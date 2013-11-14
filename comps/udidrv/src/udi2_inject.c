/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_inject.c
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

#include "udi2_inject.h"

/**
@brief ����һ��INJECTERע��ʵ��,ΪINJECTERע������׼��
@param[in] psInjecterChnl ����ע��ͨ�����ݽṹָ�롣
@param[in] psOpenParams ��Ҫ������INJECTERʵ�������ͣ���Ҫ����PCM,TS,PES,ES�ȣ�������μ�CSUDIINJECTERType_E
@param[out] phINJECTER ���δ򿪵�INJECTERע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note psInjecterChnl�ṹ���е��豸��������Ϊ��Чֵ(-1),������豸����Ҫ�򲻴���
@note Demux��������Tuner�Ͽ����ӣ��������ڴ���Inject����һ��Demux����ͬʱӵ������Դ�豸
@note ����Inject�뾡��ʹ�����ٵ��豸���磺
- ע��TS����ʱ��ע������ֻ����EM_UDIINJECTER_CONTENT_DEFAULT�����򷵻�CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- ע��PCM��ʱ��ע������ֻ����EM_UDIINJECTER_CONTENT_AUDIO�����򷵻�CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- ע��TS����ʱ��ע��ͨ��ֻ��ѡDemuxID������ƵID��Ϊ-1�����򷵻�CSUDIINJECTER_ERROR_DEVICE_DISORDER
- ע��PCM��ʱ��ע��ͨ��ֻ��ѡAudioID����ƵID��DemuxID��Ϊ-1�����򷵻�CSUDIINJECTER_ERROR_DEVICE_DISORDER
- ע��I֡��ʱ��ע��ͨ��ֻ��ѡVideoID,��ƵID��DemuxID��Ϊ-1�����򷵻�CSUDIINJECTER_ERROR_DEVICE_DISORDER
- AVI,RM,RMVB, ���漰���Ŷ�ý���ļ����ŵ�ע��ɲ�֧�֡����оƬ�����Զ�ý���ļ������и��õ�API�����֪ͬ�ޣ�
	ͬ�޿ɶԴ�API����������������ʣ����װ�ϲ�ӿ�ֱ�ӵ�����API.�Ͳ�����ʵ�ֵײ��ע��֧�֡�
*/
CSUDI_Error_Code  CSUDIINJECTEROpen(const CSUDIINJECTERChnl_S * psInjecterChnl, const CSUDIINJECTEROpenParam_S * psOpenParams, CSUDI_HANDLE * phINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �ر�һ��INJECTERע��ʵ��,�ͷ������Դ
@param[in] hINJECTER INJECTERע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code  CSUDIINJECTERClose(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����INJECTERע��ʵ����������Ϣ������������INJECTERʵ�������ԡ���ز���������Դ����Ϣ��
@param[in] hINJECTER ע��ʵ���������CSUDIINJECTEROpen����
@param[in] eAttrType ����Ҫ�������������ͣ���μ�CSUDIINJECTERAttributeType_E����ϸ˵��
@param[in] pvData ������������ݽṹָ�룬������μ�CSUDIINJECTERAttributeType_E����ϸ˵����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����Ѿ�����CSUDIINJECTERStart��������ע�룬�������ٵ��ô˽ӿڶԻص������������á�
@note �����֧�����ûص����������ô˽ӿڽ����ز�֧�ֲ���
*/
CSUDI_Error_Code CSUDIINJECTERSetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, const void * pvData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡINJECTERע��ʵ���ĸ�������Ϣ��
@param[in] hINJECTER ע��ʵ���������CSUDIINJECTEROpen����
@param[in] eAttrType ����Ҫ�������������ͣ���μ�CSUDIINJECTERAttributeType_E����ϸ˵��
@param[out] pvData ������������ݽṹָ�룬������μ�CSUDIINJECTERAttributeType_E����ϸ˵����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIINJECTERGetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, void * pvData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief  ��ʼһ��INJECTER ���� ע��
@param[in] hINJECTER INJECTERע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  ���ע��ʵ���Ѿ���ʼ���򷵻�CSUDIREC_ERROR_ALREADY_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStart(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ֹͣһ��INJECTER ����ע��
@param[in] hINJECTER INJECTERע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  ���ע��ʵ���Ѿ���ֹͣ���򷵻�CSUDIREC_ERROR_ALREADY_STOPPED
@note  ���ע��ʵ��û���������򷵻�CSUDIREC_ERROR_NOT_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStop(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief�ȴ�Inecter�Ѿ�ע�뵽Ŀ�껺�����е����ݲ������

�������������һֱ�ȵ������е����ݲ�������˲ŷ���CSUDI_SUCCESS�����²�������ʱ����CSUDI_FAILURE
һ�����ڴ���������ȫ��ע����Ϻ󣬵ȴ����Ž���
@param[in] hINJECTER INJECTER����ע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  
*/
CSUDI_Error_Code CSUDIINJECTERFlush(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ���Inecter�Ѿ�ע�뵽Ŀ�껺�����еģ���û�в������ȫ������

��Ҫ���ڿ�������ˡ���λ�Ȳ���ʱ�����ע������ݣ��ѷ�ֹ��Щ�������ӳ�
@param[in] hINJECTER INJECTER����ע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  
*/
CSUDI_Error_Code CSUDIINJECTERClear(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ�´�ע�뵽InjecterĿ�껺�����еĿ���Buffer��ַָ�����������Buffer�Ĵ�С��
@param[in] hINJECTER INJECTER����ע��ʵ�����
@param[out] ppvBuffer ���ػ�ȡ�����ڴ��ַָ��
@param[out] puLength ���ػ�ȡ�����ڴ泤��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  ���buffer�Ѿ�������ȥ���ô˽ӿ�Ӧ�÷���ʧ�ܣ���puLength����Ϊ0
@note  �ú���������CSUDIINJECTERWriteComplete���ʹ�ã��Ըú����ĵ���Ҫ�����̰߳�ȫ
@code 
	char  	*pcBuf = NULL; 
	unsigned int unBufSize = 0;
	...
	if( (CSUDIINJECTERGetFreeBuffer(hInject, (void *)&pcBuf, &unBufSize) == CSUDI_SUCCESS) && (unBufSize > 0))
	{
		//��buffer��д������,  �˴�����unWriteLen <= unBufSize
		memcpy(pcBuf, pcDataTobeWrite, unWriteLen );		
		CSUDIINJECTERWriteComplete(hInject, unWriteLen);
	}
	...
@endcode
*/
CSUDI_Error_Code CSUDIINJECTERGetFreeBuffer(CSUDI_HANDLE hINJECTER, void ** ppvBuffer, unsigned  int * puLength )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ֪ͨ����ע�������
@param[in] hINJECTER INJECTER����ע��ʵ�����
@param[in] uWrittenSize �ѿ������ݵĴ�С
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���buffer����ΪnLength, ע�볤�ȴ���nLength,��ýӿڷ���CSUDIINJECTER_ERROR_NO_MEMORY
@note  �ú���������CSUDIINJECTERGetFreeBuffer���ʹ�ã��Ըú����ĵ���Ҫ�����̰߳�ȫ
*/
CSUDI_Error_Code CSUDIINJECTERWriteComplete(CSUDI_HANDLE hINJECTER, unsigned int uWrittenSize)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

