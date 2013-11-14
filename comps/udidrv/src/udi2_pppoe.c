/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_pppoe.c
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

#include "udi2_pppoe.h"
/**
@brief ��ʼ��pppoeģ��

@param[in] psInitPara ��ʼ��ʱ��Ҫ����Ĳ���,ΪCSUDIPPPOEInitPara_S����,��ջ��С�����ȼ���0�Ļ����ǵ�ģ���ڲ���ȡĬ��ֵ
@return CSUDI_SUCCESS��ʾ��ʼ���ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note  ACMAC�ͻỰid��flash�ж���(ÿ�����ӳɹ���Ҫ��������ֵ������flash��)����һ��ACMAC��ȫFF,sessionid��0
*/
CSUDI_Error_Code  CSUDIPPPOEInit(CSUDIPPPOEInitPara_S *psInitPara)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ����pppoeģ��

@param[in] nFlag kill��pppd���������õķ�ʽ��gentle����rude��ʽ,0Ϊgentle��ʽ��1Ϊrude��ʽ��0��ʾɱ���ý��̼��ɣ�1��ʾɱ���ý��̼����е��ӽ���
@return CSUDI_SUCCESS��ʾ���ٳɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note �ú������������е�pppoe����Դ�����Ҫ�������ӣ���Ҫ���³�ʼ�����ٽ�������
*/
CSUDI_Error_Code  CSUDIPPPOECleanup (int nFlag)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��������

@param[in] pcUserName �û���
@param[in] pcPasswd ����
@param[in] pcServiceName pppoe��������
@param[in] pcEthName �����������ƣ���"eth0"
@param[in] nTimeout ���ӳ�ʱʱ�䣬��λ:��
@return CSUDI_SUCCESS��ʾ���ӳɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note ���ӳɹ�֮�󣬻�ȡ����ip��ַ��������������ء�dns����Ϣ���Իص�����ʽ֪ͨ���û�
*/
CSUDI_Error_Code  CSUDIPPPOEConnect(const char *pcUserName, const char *pcPasswd, char *pcServiceName, char *pcEthName, int nTimeout)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief �Ͽ�����

@param[in] nTimeout �Ͽ����ӵĳ�ʱʱ�䣬��λ:��
@return CSUDI_SUCCESS��ʾ�Ͽ����ӳɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note ��Ҫ�Ͽ�pppoe���ӣ����Ե��øú������Ժ��ٽ������Ӿ�ֱ�ӵ���CSUDIPPPOEConnect
*/
CSUDI_Error_Code  CSUDIPPPOEDisconnect(int nTimeout )
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡpppoe��������

@param[out] pcSevicename ���������ȡ��pppoe��������
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
*/
CSUDI_Error_Code  CSUDIPPPOEGetServiceName( char *pcSevicename)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡpppoe״̬

@param[out] pnState ���������ȡ��pppoe��״̬,״̬��ȡֵΪCSUDIPPPOEState_E�ж���ļ���ֵ
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
*/
CSUDI_Error_Code  CSUDIPPPOEGetState(int *pnState)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡAC��MAC��ַ

@param[out] pcMAC ���������ȡ��AC��MAC��ַ
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note ac��mac��ַ�ĳ���Ϊ6���ֽڣ������buffer�Ŀռ�Ҫ����6���ֽ�
*/
CSUDI_Error_Code  CSUDIPPPOEGetACMacAddr(char *pcMAC)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡpppoe�Ựid

@param[out] pusSessionID ���������ȡpppoe�ĻỰid
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note ÿ�����ӳɹ��������һ���Ựid������ûỰid���´����ӵ�ʱ��ֱ�Ӵ��룬��ӿ������ٶ�
*/
CSUDI_Error_Code  CSUDIPPPOEGetSessionID(unsigned short *pusSessionID)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡpppoe�汾��

@param[out] pcVer ���������ȡpppoe�İ汾��
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
*/
CSUDI_Error_Code  CSUDIPPPOEGetVersion( char *pcVer)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ���һ��pppoe�ص�

@param[in] pvUserData �����û�������Ϣ,����Ϊ�ص�������dwUserData��������
@param[in] fnCallback Ҫ��ӵĻص�����pppoe״̬�����仯��ʱ�����øú���
@return CSUDI_SUCCESS��ʾ��ӻص��ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note  ������Ӷ���ص� ,Ҫ�� pfCallback��dwUserData����ͬ��ʱ��Ų�����ӽ�ȥ���������
@note pppoeģ��֧��ע����С8���ص�����
*/
CSUDI_Error_Code CSUDIPPPOEAddCallback(CSUDIPPPOECallback_F fnCallback,void *pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief �Ƴ�һ���ص�

@param[in] pvUserData �û�������Ϣ
@param[in] fnCallback Ҫ�Ƴ��Ļص�
@return CSUDI_SUCCESS��ʾ�Ƴ��ص��ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note  �����ں�pfCallback��dwUserData����ͬ��ʱ��Ż��Ƴ�,����������ƥ��ʱ���Ƴ�
*/
CSUDI_Error_Code CSUDIPPPOERemoveCallback(CSUDIPPPOECallback_F fnCallback,void *pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

