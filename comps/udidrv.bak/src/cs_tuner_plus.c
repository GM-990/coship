/****************************************************************************/
/*
 * Filename:        cs_tuner_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------

 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_tuner_plus.h"


 /**
@brief Tunerģ���ʼ��

@param[in] uTunerId tuner�豸��
@param[in] psTunerInitParam Tuner��ʼ����ز�������
@return 
	- �ɹ����ظ���ʼ������CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
	- �������󷵻�CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDITunerInit(unsigned int uTunerId, CSUDITunerInitParam_S *psTunerInitParam)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

 /**
@brief RDI Tuner��� 

@param[in] nPriority ����̵߳����ȼ�  
@return 
	- �ɹ����ظ���ʼ������CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
	- �������󷵻�CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSRDITunerDetect(int nPriority)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}



 /**
@brief Tunerģ�鷴��ʼ��

@param[in] uTunerId tuner�豸��
@return 
	- �ɹ����ظ���ʼ������CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
	- �������󷵻�CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDITunerTerm(unsigned int uTunerId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}


 /**
@brief ע��QAMģ����غ���

@param[in] ��
@param[in] psTunerInitParam Tuner��ʼ����ز�������
@return 
	- �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterQam(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}


 /**
@brief ע��COFDMģ����غ���

@param[in] ��
@param[in] psTunerInitParam Tuner��ʼ����ز�������
@return 
	- �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterCofdm(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}


 /**
@brief ע��QPSKģ����غ���

@param[in] ��
@param[in] psTunerInitParam Tuner��ʼ����ز�������
@return 
	- �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterQpsk(void)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

