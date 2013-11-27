/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_eeprom.c
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

#include "udi2_eeprom.h"
/**
@brief ��EEPROM�洢������д����

@param[in] dwStartAddress ��д��洢������ʼ��ַ��
@param[in] pucData ��д�������ָ�롣
@param[in] uDataLength ��д������ݳ���(��λ���ֽ�)��
@return CSUDI_SUCCESS��ʾEEPROM����д�ɹ���CSUDI_FAILURE��ʾEEPROM����дʧ�ܡ�
*/
CSUDI_Error_Code CSUDIEPRWrite(CSUDI_UINT32 dwStartAddress, const unsigned char * pucData, unsigned int  uDataLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��EEPROM�洢�����ж�����

@param[in] dwStartAddress �������洢������ʼ��ַ��
@param[out] pucData ��Ŷ������ݵ�����ָ�롣
@param[in] uDataLength ���������ݵĳ���(��λ���ֽ�)��
@return CSUDI_SUCCESS��ʾEEPROM�������ɹ���CSUDI_FAILURE��ʾEEPROM������ʧ�ܡ�
*/
CSUDI_Error_Code CSUDIEPRRead(CSUDI_UINT32 dwStartAddress, unsigned char * pucData, unsigned int  uDataLength )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡEEPROMоƬ�����Ϣ

@param[out] psEPRInfo ��д��EEPROM��Ϣ��ָ�������
@return �����ɹ����� CSUDI_SUCCESS;���򷵻� CSUDI_FAILURE   
@note �ú���������ȡӦ������EEPROM�ṹ����Ϣ������Ϣ�ڽ������ݶ�дʱ�����õ���
@note���������ڽ���EEPROMʵ�ʲ���֮ǰ��Ӧ���õĵڶ�����������һ������Ϊeeprom��ʼ��������
@note�ڵ���ʱ��psEPRInfo�ɵ����߷���ռ䣬�ú���ֻ�轫��Ӧ���ֵ����
*/
CSUDI_Error_Code CSUDIEPRGetInfo(CSUDIEPRInfo_S * psEPRInfo)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

