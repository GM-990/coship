/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_screen.c
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

#include "udi2_screen.h"
/**
@brief ��ȡָ������豸�ķֱ�����������
@param[in] eScreenDevice ��Ƶ���ͨ������
@param[out] psCapabilityInfo  ���ͨ���豸������Ϣ,������μ�CSUDISCREENCapability_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psCapabilityInfo�ṹ������δ����
@note ��ƽ̨��֧�ָ����ͨ��(�粻֧�ָ���)�ýӿڷ���CSUDISCREEN_ERROR_FEATURE_NOT_SUPPORTED
@note �˽ӿ�һ��ֻ������һ��Screen�豸�����ö���豸��򣬷��򷵻�CSUDISCREEN_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDISCREENGetCapability(CSUDISCREENType_E eScreenDevice,CSUDISCREENCapability_S * psCapabilityInfo)
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

 /**
@brief  ע��ָ���¼����͵Ļص�����

@param[in] fnScreenCallback �ص�����ָ�룬���CSUDISCREENCallback_F����
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ��ͬCSUDISCREENCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note �ص���������С���ע�����Ϊ32��������32������ʧ�ܲ����ش������ֵ
@note fnScreenCallback��pvUserDataΨһȷ��һ��ע�ᣬ������ע��������ȫһ��ʱ���ڶ��ν�����CSUDISCREEN_ERROR_ALREADY_ADDED����ֻ��֪ͨһ�Σ����ע���������������ȫһ��ʱ��֪ͨ���
*/
CSUDI_Error_Code CSUDISCREENAddCallback(CSUDISCREENCallback_F fnScreenCallback,void * pvUserData )
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}


 /**
@brief ɾ��ָ���¼����͵Ļص�����

@param[in] fnScreenCallback �ص�����ָ�룬���CSUDISCREENCallback_F����
@param[in] pvUserData ͬCSUDISCREENCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ�����ش������ֵ
@note fnScreenCallback��pvUserDataΨһȷ��һ��ע�ᣬ����������ע��ʱ��ȫһ�����ܳɹ�ɾ��
*/
CSUDI_Error_Code CSUDISCREENDelCallback(CSUDISCREENCallback_F fnScreenCallback, void * pvUserData )
{	 
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);	  
	return Retcode;
}

/**
@brief ��������ֱ��ʼ�ˢ��Ƶ��

����ָ����Ƶ����ӿڵķֱ���
@param[in] peScreenDevice ��ָ����Ƶ����ӿڵ�����
@param[in] peResolution ϵͳ����ֱ�������, 720*480,720*576, 1920*1080 etc����μ�CSUDISCREENResolution_E
@param[in] nCount peScreenDevice��peResolution����Ĵ�С��Ҫ�������������һ����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note Eastwin���ò�ͬ����ӿڵķֱ�����֡������þ��м����ԡ���CVBS�����PAL�ƺ͸����1080i 50Hz����,ͬ1080i 60Hz�����ݡ���������UDIӦ��������
@note Eastwin�ᾡ��һ�������ö��Screen�ķֱ��ʣ��Է���UDI����кϲ�����
@note ��ͬʱ���ö����Ƶ�豸���ͣ����κ�һ����֧��ʱ���ýӿڷ���ʧ�ܣ���������֧�ֵ����ò���Ч
*/
CSUDI_Error_Code CSUDISCREENSetResolution(CSUDISCREENType_E* peScreenDevice, CSUDISCREENResolution_E* peResolution,int nCount)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ��ǰ��Ƶ�ֱ���

@param[in] eScreenDevice ָ����Ƶ����ӿ�
@param[out] peResolution ��ǰ��Ļ�ķֱ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note �˽ӿ�һ��ֻ������һ��Screen�豸�����ö���豸��򣬷��򷵻�CSUDISCREEN_ERROR_BAD_PARAMETER
@note ��Ļ�ֱ�����Player�����ŵ��޹أ����۵�ǰ��ʲô״̬����Ҫ�ܻ�ȡ����Ϣ��
*/
CSUDI_Error_Code CSUDISCREENGetResolution(CSUDISCREENType_E eScreenDevice,CSUDISCREENResolution_E * peResolution)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����Screen OSD͸����

@param[in] nTransparency ͸���ȣ�ȡֵ[0��100];
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����OSD���͸���ȣ���ΧΪ0-100������0Ϊ��ȫ͸����100Ϊ��͸��
*/
CSUDI_Error_Code  CSUDISCREENSetOSDTransparency(int nTransparency)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ������Ƶ���������

@param[in] nBrightness ��Ƶ��������ȣ�ȡֵ[0��100]
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ������Ƶ��������ȣ�0Ϊȫ�ڣ�100Ϊ������ߡ�
@note ���ȵ�Ĭ��ֵΪ50,����ֵ����Ϊ50ʱ,�������Ŀ���ֳ����Ч��
*/
CSUDI_Error_Code CSUDISCREENSetBrightness(int nBrightness)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ������Ƶ����ĶԱȶ�

@param[in] nContrast ��Ƶ����ĶԱȶȣ�ȡֵ[0��100]��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ������Ƶ����Աȶȣ�0Ϊȫ�ڣ�100Ϊ�Աȶ���ߡ�
@note �Աȶȵ�Ĭ��ֵΪ50,����ֵ����Ϊ50ʱ,�������Ŀ���ֳ����Ч��
*/
CSUDI_Error_Code CSUDISCREENSetContrast( int nContrast)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ������Ƶ����ı��Ͷ�

@param[in] nSaturation ��Ƶ����˿ڵı��Ͷȣ�ȡֵ[0��100]��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����ָ����ı��Ͷȣ�0Ϊȫ�ڣ�100Ϊ���Ͷ���ߡ�
@note ���Ͷȵ�Ĭ��ֵΪ50,����ֵ����Ϊ50ʱ,�������Ŀ���ֳ����Ч��
*/
CSUDI_Error_Code CSUDISCREENSetSaturation( int nSaturation)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

