/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_toolset.c
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

#include "udi2_toolset.h"
/**
@brief ��ȡ��оƬ��ID��

@param[out] psChipID  ���ڴ�Ż�ȡ����Chip ID���䳤��,��μ�CSUDICHIPID_S��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note оƬ����֧�ֻ�ȡChipId���������оƬ�����CoshipЭ�̴���,Э�̺���ȷ��ȷʵ�޷�ʵ���򷵻�CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED��
*/
CSUDI_Error_Code CSUDIGetChipID(CSUDICHIPID_S * psChipID)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}



/**
@brief ��ȡĿ����������кź�Key��

@param[out] psSerialKey ���ڴ�Ż�ȡ�������кź�Key�����кź�Key������0��β���ַ�������μ�CSUDISerialKey_S��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIGetSerialKey(CSUDISerialKey_S * psSerialKey)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief ����Ŀ����������кź�Key��

@param[in] psSerialKey ���ڴ�����õ����кź�Key����μ�CSUDISerialKey_S��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �ýӿ�Ŀǰ��ʹ��m_szPairsKey ��
*/
CSUDI_Error_Code CSUDISetSerialKey(const CSUDISerialKey_S * psSerialKey)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief ������Ŀ�������

@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDISYSReboot(void)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief ʵ��ϵͳ�Ĺػ���

@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 
- ִ�гɹ����ú������Բ����ء�
- ����֧�ָù��ܿɷ���CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED��
*/
CSUDI_Error_Code CSUDISYSHalt(void)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief ���ô������ԡ�
@param[in] eAttribute ΪCSUDISYSStandbyAttribute_E ���ͣ���ʾ��Ҫ���õĴ�����������
@param[in] pValue Ϊ����ָ�����ͣ��������������eAttribute ȷ�����ɲο�CSUDISYSStandbyAttribute_E ע��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note
-�����õĴ�������ΪEM_SYS_STANDBY_ATTR_INFO �����ɳ����������Ŀ��������������Ƿ�仯��( ��ʱ��)
*/
CSUDI_Error_Code CSUDISYSSetStandbyAttribute(CSUDISYSStandbyAttribute_E eAttribute, void* pValue)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief ʹϵͳ�������״̬�����˳�����״̬��

@param[in] bStandby ΪCSUDI_TRUEʱ��ʾ�������״̬;ΪCSUDI_FALSEʱ��ʾ�˳�����״̬��
@param[in] nStandbyTime ��������ʱ�䣬��λΪ�롣��nStandbyTime����Զ����ѣ�Ϊ0��ʾ���Զ����ѡ����������ʱ��Ч���ٴ���ʱ���Ըò�����
@param[out] pbReal ΪCSUDI_TRUEʱ��ʾ�������;ΪCSUDI_FALSEʱ��ʾ�Ǽٴ���״̬��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 
- ��Ϊ�ٴ�������ýӿ����Ϸ��أ�����*pbReal����ΪCSUDI_FALSE�������������Ҫ�˳�����״̬���ٴ���bStandby=CSUDI_FALSE���ñ��ӿڡ�\n
- ��Ϊ���������ýӿ����˳�����״̬ʱ���أ�����*pbReal����ΪCSUDI_TRUE��������²���Ҫ����bStandby=CSUDI_FALSE���ñ��ӿڣ����˳�����״̬ʱ����״̬�������ڴ����ݣ��豸�����ӵȣ����ֲ��䡣
- ������ٴ����������ȹر�����Ƶ���š�
*/
CSUDI_Error_Code CSUDISYSStandby(CSUDI_BOOL bStandby,int nStandbyTime,CSUDI_BOOL *pbReal)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief ���������ӿڣ�Booter��ר�ã�����������ʵ��

��������������õ�ָ��λ�ú�ʹ�øýӿ�ʹ�ó�����ת���˵�ַ��
@param[in] psExecInfo �洢�������������Ϣ
@return �ýӿڵ��ú�ֱ�ӽ��뱻��������������������践�ء�
@note ����������ο������ڴ��У�Ҳ������Flash�ϣ���������Ĳ��죬ѡ��ʵ�ּ��ɡ�
*/
void CSUDIToolsetExec(CSUDIToolsetExecInfo_S *psExecInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

