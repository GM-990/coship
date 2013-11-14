/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_smc.c
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

#include "udi2_smc.h"
/**
@brief ��ȡϵͳ�п��õ����ܿ����۸���

@param[out] pnSMCCount ϵͳ�п��õ����ܿ����۸���
@note �����оƬ֧���������ۣ���ϵͳֻ����һ������ô�˽ӿڷ���1
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����pnSMCCount������δ����
*/
CSUDI_Error_Code CSUDISMCGetCount(int * pnSMCCount)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �����ܿ�

@param[in] nCardIndex ���ܿ������ţ���0��ʼ   
@param[in] fnSMCCallback �ص����������CSUDISMCCallback����
@return ����ɹ�����CSUDI_SUCCESS�����ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCOpen(int nCardIndex, CSUDISMCCallback fnSMCCallback)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �ر����ܿ�

@param[in] nCardIndex ���ܿ������ţ���0��ʼ��   
@return  ����ɹ�����CSUDI_SUCCESS�����ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCClose(int nCardIndex)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �������ܿ�

@param[in] nCardIndex ���ܿ������ţ���0��ʼ�� 
@param[in] bColdRst ���ȸ�λ��־��TRUEʱ�临λ��FALSEʱ�ȸ�λ
@param[out] pucAtr �������ܿ���Ӧ������
@param[out] pnAtrLength pATr�ĳ���
@return ����ɹ�����CSUDI_SUCCESS�����ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCReset(int nCardIndex, unsigned char * pucAtr, int * pnAtrLength, CSUDI_BOOL bColdRst)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �������ܿ�������Ϣ

@param[in] nCardIndex ���ܿ�������   
@param[in] psSMCParams  ���ܿ�������Ϣ
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCSetParams(int nCardIndex,const CSUDISMCParam_S * psSMCParams)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡ���ܿ�������Ϣ

@param[in] nCardIndex ���ܿ�������   
@param[out] psSMCParams  ���ܿ�������Ϣ
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ����psSMCParams������δ����
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCGetParams(int nCardIndex,CSUDISMCParam_S * psSMCParams)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �豸�����ܿ�֮������ݴ���

@param[in] nCardIndex ���ܿ�������   
@param[in] pucWriteData д�����ܿ�������
@param[in] nNumberToWrite д�����ݳ���
@param[out] pcResponseData ��ȡ���ݵĴ洢λ��
@param[out] pnNumberRead ��ȡ���ݵĳ���
@param[out] pucStatusWord �����ṩ��״̬��
@return  ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCDataExchange(
									int             nCardIndex, 
									unsigned char  *pucWriteData,
	                                int             nNumberToWrite,
	                                unsigned char  *pcResponseData,
	                                int            *pnNumberRead,
	                                unsigned char  *pucStatusWord)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief  �����ܿ�д����

@param[in] nCardIndex ���ܿ�������   
@param[in] pucWriteBuf ׼��д���ݵĴ洢��
@param[in] nNumberToWrite ׼��д���ݵĳ���
@param[out] pnNumberWrite ��д�����ݳ���
@param[in]  nTimeout ��ʱʱ��
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCSend(
					 int             nCardIndex,
				     unsigned char  *pucWriteBuf,
				     int             nNumberToWrite,
				     int            *pnNumberWrite,
				     int             nTimeout)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief �����ܿ�������

@param[in] nCardIndex ���ܿ�������   
@param[in] pucReadBuf ��ȡ���ݴ洢��
@param[in] nNumberToRead ��ȡ�ռ��ڴ泤��
@param[out] pnNumberRead �Ѷ�ȡ�����ݳ���
@param[in] nTimeout ��ʱʱ��
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
*/
CSUDI_Error_Code CSUDISMCReceive(
						int             nCardIndex,
				        unsigned char  *pucReadBuf,
				        int             nNumberToRead,
				        int            *pnNumberRead,
				        int             nTimeout)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief �������ܿ�Э��ģʽ

@param[in] nCardIndex ���ܿ�������   
@param[in] pucWriteData һ��FI,DI��������
@param[in] nNumberToWriteLen �������ݵ��ֽڸ���
@note
- case  ((pucWriteData[0] & 0x10) != 0)    PPS1 valid
- case  ((pucWriteData[0] & 0x20) != 0)    PPS2 valid
- case  ((pucWriteData[0] & 0x40) != 0)    PPS3 valid
@param[out] pucResponseData ������Э��ģʽ��������Ӧ����
@param[out] pnResponseDataLen ��Ӧ���ݵ��ֽڸ���
@return ����ɹ�����CSUDI_SUCCESS�����ʧ�ܷ��ش������ֵ
@note �������Ƿ�Э��ģʽ���򷵻�CSUDISMC_ERROR_INVALID_PROTOCOL
*/
CSUDI_Error_Code CSUDISMCSetPPS(
						 int             nCardIndex,
	                    unsigned char  *pucWriteData,
                           int             nNumberToWriteLen,
	                    unsigned char  *pucResponseData,
                           int            *pnResponseDataLen)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief  ������ܿ�

@param[in] nCardIndex ���ܿ�������   
@return ���ص�ǰ��״̬
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCDetect(int nCardIndex)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief  �������ܿ�

@param[in] nCardIndex ���ܿ�������   
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCActive(int nCardIndex)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief  ȥ����

@param[in] nCardIndex ���ܿ�������   

@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ������ܿ������ڣ��򷵻�CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCDeactive(int nCardIndex)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

