/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_flash.c
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

#include "udi2_flash.h"
/**
@brief ��Flash�ж�ȡ��������� 

@param[in] dwStartAddress �洢���ݵ�Flash�ռ���ʼ��ַ
@param[out] pucBuf ����������ݵĻ�������ʼ��ַ
@param[in] uDataLength Ҫ��ȡ�����ݳ���(���ֽ�Ϊ��λ)
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���CSUDI_FAILURE��ʾ��ȡʧ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashRead (CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf,  unsigned int uDataLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��Flash��д����Ҫ��������� ,�ڽ�����

@param[in] dwStartAddress �洢���ݵ�Flash�ռ��ַ
@param[in] pucBuf Ҫд�����ݵĻ�������ʼ��ַ
@param[in] uDataLength Ҫд������ݳ���(���ֽ�Ϊ��λ)
@return CSUDI_SUCCESS��ʾд��ɹ���CSUDI_FAILURE��ʾд��ʧ��
@note �Ѿ��ڴ���״̬�µ��ñ��ӿ��Է���TRUE

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashWrite(CSUDI_UINT32 dwStartAddress, unsigned char * pucBuf, unsigned int uDataLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��ȡFlash����������Ϣ
@param[in] eSuitType ���ȡ��Flash������������
@param[out] psFlashSuit ָ��flash�����Ϣ�ṹ���ָ�룬��ϸ�����CSUDIFLASHRegionSuit_S
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���CSUDI_FAILURE��ʾ��ȡʧ��
@note ��ƽ̨��֧�ֻ�ȡ��ϸ��Block��Ϣ���ɽ�����Flash����Ϊͬһ��Block������������������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashGetInfo(CSUDIFLASHRegionSuitType_E eSuitType,CSUDIFLASHRegionSuit_S * psFlashSuit)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����ָ���ռ䡣

@param[in] dwStartAddress �������ռ����ʼ��ַ�����Ե�ַ��
@param[in] uSpaceSize �������ռ��С�����ֽ�Ϊ��λ��
@return CSUDI_SUCCESS��ʾ���������ɹ�,CSUDI_FAILURE��ʾ��������ʧ��
@note 
- ����Flash��Ҫ����Block���У�������ĵ�ַ����С������Block�����Է���CSUDIFLASH_ERROR_BAD_PARAMETER����Ҳ
��������ִ�У����������ִ�У��豣֤δָ���������ֵ����ݲ��ñ��ƻ���
- ��0-0x10000Ϊһ��Block����Ҫ�����0-0x8000��������Է���CSUDIFLASH_ERROR_BAD_PARAMETER���������Flash�洢
�����ݲ������κα仯��Ҳ��Ҳ����CSUDI_SUCCESS������Flash0-0x7fff�����ݱ�����,������Flash��������ݣ��ر���
0x8000-0x10000�����ݲ��÷����仯��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashEraseSpace(CSUDI_UINT32 dwStartAddress, unsigned int uSpaceSize)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ��FLASH��д����Ҫ��������� ,Ϊ�����Ч��ʵ��ʱ��������

@param[in] dwStartAddress ��д��ռ����ʼ��ַ�����Ե�ַ��
@param[in] pucBuf Ҫд�����ݵĻ�������ʼ��ַ
@param[in] uDataLength ��д�����ݳ��ȣ����ֽ�Ϊ��λ��
@return CSUDI_SUCCESS ��ʾд�����ɹ���CSUDI_FAILURE��ʾд����ʧ��
@note 
- ����д����δ�������Ȳ������Ч��CSUDIFlashWrite
- �м������flash�����Ա�֤д���ַ�����ݶ��Ǵ�1->0��������ִ�0��Ϊ1�Ĳ�������˸ýӿ�ֻ��Ҫ��д��ĵط�ֱ�Ӱ�λ�޸����ݼ���
- ����:���flash��ĳ��ַ��Ӧ������: 0x31 0x39 0x38 0x34 дΪ: 0x31 0x39 0x38 0x30,ֻ��Ҫ��0x34�е�һλ1,��Ϊ0���ɡ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFlashWriteWithoutErase( CSUDI_UINT32 dwStartAddress, const unsigned char * pucBuf, unsigned int uDataLength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

