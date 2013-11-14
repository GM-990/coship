/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_expand.c
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

#include "udi2_expand.h"

/**
@brief ��ȡ��չ�����豸������״̬������: ӲAP��cable modem��EOC��

@param[in] pcEthName ����չ�����豸��������������
@param[out] pushLinkState ����״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
@note ��������������״̬:
@note 1��pushLinkState & CSUDI_ETHMASK_UP_DOWN : Ϊ0�����Ѿ����ӣ�Ϊ1����δ���ӡ�
@note 2�����ɳ��̿������о�����ȡ����״̬�ķ���������: SNMP ��ѯЭ��
*/
CSUDI_Error_Code CSUDIExpandGetStatus(char * pcEthName, unsigned short * pushLinkState)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

