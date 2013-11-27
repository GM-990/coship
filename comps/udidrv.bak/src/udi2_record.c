/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_record.c
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

#include "udi2_record.h"
/**
@brief ע��¼�ƻص�����

@param[in] hRecHandle ¼�ƾ������CSUDIRECOpen()���
@param[in] fnRecordCallback ¼�ƻص����������CSUDIRECCallback_F˵��
@param[in] pvUserData  �û����ݣ������ݻ�ԭ�ⲻ�����ظ��û�
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ����֧��ע��Ļص���������Ϊ32���������
@note �ص�����ָ��+�ص���������+�û�����Ψһ��ʶһ���ص�,������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIREC_ERROR_ALREADY_ADDED
@note ��ע��ĺ����Ѿ�ע���������CSUDIREC_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECAddCallback(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback, void * pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ɾ��¼�ƻص�����

@param[in] hRecHandle ¼�ƾ������CSUDIRECOpen()���
@param[in] fnRecordCallback ¼�ƻص����������CSUDIRECCallback_F˵��
@param[in] pvUserData  �û����ݣ������ݻ�ԭ�ⲻ�����ظ��û�
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ������CSUDIREC_ERROR_NO_CALLBACK
@note �����ڻص�������ɾ���ص�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECDelCallback(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback,void * pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����һ��¼����

@param[in] psChnl   ¼��ͨ�������ݽṹ��
@param[out] phRecHandle ��¼�Ƶľ����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECOpen(const CSUDIRecChnl_S * psChnl, CSUDI_HANDLE * phRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ¼��֧�ֵļ�������

@param[in] hRecHandle ¼�ƾ������CSUDIRECOpen()���
@param[out] peOutTypeArray   ��·¼��֧�ֵļ��ܷ�ʽ(����֧�ֶ���)�����CSUDIRecEncryptType_E����
@param[in]  nArraySize ����peOutTypeArray����ĳ���
@param[out]  pnActualCnt ��·¼��֧�ֵļ��ܷ�ʽ����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECGetSupportEncryptTypes(CSUDI_HANDLE hRecHandle,CSUDIStreamEncryptType_E* peOutTypeArray,int nArraySize,int* pnActualCnt)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����һ��¼����

@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ����һ��¼��������CSUDIRECOpen�ɶ�ʹ�á�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECClose(CSUDI_HANDLE hRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����¼��

@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@param[in] psStartParam  ����¼�Ʋ������CSUDIRecStartParam_S˵��
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note �ɲ���psStartParamָ����Ҫ¼�Ƶ������Լ�¼�ƵĽ�Ŀ
@note ���ô˽ӿ�֮ǰ����Ҫ����CSUDIDEMUXConnectTuner����hRecHandle��Ӧ��demux��tuner�豸����ʹ�ö�Ӧ��tuner�Ѿ���Ƶ�ɹ���
@note �����¼�����õ�demux�Ѿ�����tuner�����ʱ����Ҫ�ٴε���CSUDIDEMUXConnectTuner����demux��tuner������
@see CSUDIDEMUXConnectTuner

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStart(CSUDI_HANDLE  hRecHandle,const CSUDIRecStartParam_S * psStartParam)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ֹͣ¼��

@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStop(CSUDI_HANDLE  hRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ͣ¼��
@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECPause(CSUDI_HANDLE  hRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �ָ�¼��
@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note �ָ�¼�ƺ���¼�Ƶ�����׷�ӵ���ͣǰ��¼���ļ���

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECResume(CSUDI_HANDLE  hRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ֹͣʱ��¼��

@param[in] hRecHandle ����ʱ��¼�Ƶ�¼����������ɺ���CSUDIRECOpen()���
@param[in] bSwitchToFile �Ƿ�ʱ��¼�Ƶ�ѭ���ļ�ת������ͨ�ļ�
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStopTshift(CSUDI_HANDLE  hRecHandle,CSUDI_BOOL bSwitchToFile)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ¼��ʱ����Ϣ

@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@param[out] psTimeInfo ¼�Ƶ�ʱ����Ϣ�����CSUDIRecTimeInfo_S����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECGetRecTime(CSUDI_HANDLE  hRecHandle, CSUDIRecTimeInfo_S * psTimeInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ¼���ļ��и�

@param[in] szFileName ¼�����ɵ��ļ�
@param[in] unStartPos ���п�ʼλ��(��λΪs)
@param[in] unEndPos ���еĽ���λ��(��λΪs)
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note �и������Ҫ��������ʱ��λ��nstartPos��nEndPosת�����ļ��е�����λ�ã������ö�����ɾ�������и����ļ��ɲ��ų��ȱ��и�ǰ������(nEndPos-nStartPos)

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECFileCut(const char* szFileName,unsigned int unStartPos,unsigned int unEndPos)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

