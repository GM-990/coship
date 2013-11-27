/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_tuner.c
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

#include "udi2_tuner.h"
#include "generic_include.h"

#define TUNER_TYPE  1 //1: CABLE, 2:SATALLITE, 3:TERRISIAL

extern CS_TM_PIPE_OBJECTS gTmPipeObject;

/**
@brief ����ָ����Ƶ�������Ƶ����.

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] punDeliver �û����Ƶ����Ϣ�����ݿ�ĵ�ַ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����롣\n
-- CSUDITUNER_ERROR_BAD_PARAMETER����������\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID���Ƿ�ID��\n
-- CSUDITUNER_ERROR_I2C_ERR��I2C���ߴ���\n
@note ���ӿ��첽ִ�У����سɹ�����ʾ�����ѽ��գ�����ʾ����ִ�н����
@note ���ӿڷ��غ������������Ƶ�����ϴβ�ͬ�����������ֹͣ���ϸ�Ƶ���demux���ݽ���

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerConnect(CSUDI_UINT32 dwTunerId, const CSUDITunerSRCDeliver_U* punDeliver)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 

	PIPE_TUNING_PARAMETERS PipeTuneParams;
	PIPE_STATUS       PipeStatus = CNXT_STATUS_OK;
#if (TUNER_TYPE == 1) //cable
	PipeTuneParams.bAsync = TRUE;
	PipeTuneParams.Type = PIPE_CABLE_TUNER_TYPE;
	PipeTuneParams.TuningParameters.CableTuningParameters.uFrequencyKHz = punDeliver->m_sCableDeliver.m_uFrequency;
	
	switch (punDeliver->m_sCableDeliver.m_uModulation_8)
	{
	case 1:		
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_16QAM;
		break;
		
	case 2: 	
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_32QAM;
		break;

	case 3:		
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_64QAM;
		break;

	case 4:		
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_128QAM;
		break;		

	case 5:		
		PipeTuneParams.TuningParameters.CableTuningParameters.Modulation = PIPE_TUNER_MOD_TYPE_256QAM;
		break;		
		
	}
	
	
	PipeTuneParams.TuningParameters.CableTuningParameters.uSymbolRateSs = punDeliver->m_sCableDeliver.m_uSymbolRate_24*1000;
	PipeTuneParams.TuningParameters.CableTuningParameters.Annex=PIPE_TUNER_CABLE_ANNEX_A;
	PipeTuneParams.TuningParameters.CableTuningParameters.bAutoSpectrum=TRUE;
	PipeTuneParams.TuningParameters.CableTuningParameters.bInvertedSpectrum = FALSE;
	PipeTuneParams.TuningParameters.CableTuningParameters.uSDPMax =255;
    PipeTuneParams.TuningParameters.CableTuningParameters.uSDPMin =0;

	UDIDRV_LOGI("frank.zhou m_uFrequency =%d\n", PipeTuneParams.TuningParameters.CableTuningParameters.uFrequencyKHz);
	UDIDRV_LOGI("frank.zhou m_uModulation_8 =%d\n", PipeTuneParams.TuningParameters.CableTuningParameters.Modulation);
	UDIDRV_LOGI("frank.zhou m_uSymbolRate_24 =%d\n", PipeTuneParams.TuningParameters.CableTuningParameters.uSymbolRateSs);

	PipeStatus = gTmPipeObject.hTunerObj[0]->tune(gTmPipeObject.hTunerObj[0], &PipeTuneParams);
	if(PipeStatus != PIPE_STATUS_OK)
	{
		UDIDRV_LOGI("frank.zhou tune fail!\n");
	}
	else
	{
		UDIDRV_LOGI("frank.zhou tune success!\n");
	}
#else if (TUNER_TYPE == 2) //sat

#endif
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

 /**
@brief ע��Tuner��Ƶ��Ϣ�ص�

@param[in] dwTunerId tuner�豸�š�
@param[in] fnTunerCallback �ص����������
@param[in] pvUserData �û�������Զ���ָ��,����֪ͨ�������ظ��û���������Ϣ��
@return ����ɹ�����CSUDI_SUCCESS, ʧ�ܷ��ش������
@note
- �ص�������ע���������֧��8�������Ӧ��ע��ص�����������֧�ָ���������CSUDITUNER_ERROR_NO_MEMORY
- һ��tuner id,callback,userdata��ͬ����һ��Tuner��Ƶ��Ϣ�ص���������ע��������ȫһ��ʱ���ڶ��ν�����CSUDITUNER_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerAddCallback(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	int *p = pvUserData;
	*p = 1;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

 /**
@brief ɾ��ָ������Ƶ�ص�������ע��

@param[in] dwTunerId tuner�豸�š�
@param[in] fnTunerCallback �ص����������
@param[in] pvUserData ͬCSUDITunerAddCallback��pvUserData
@return�ɹ�����CSUDI_SUCCESS ����ûص�����������û��ע�ᣬ������CSUDI_FAILURE
@note ���ڿ��ƶ���Tuner�����䱻�γ�������Ҫ����ɾ�����Ӧ�Ļص�ע�ᣬ�����Ի�ռ�ò�����Դ����Ҫ���ڴ棩

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerDelCallback(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

 /**
@brief ȡ�õ�ǰ�����������ź���Ϣ

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸��
@param[in] u32SignalInfoMask ���ȡ���ź���Ϣ����,ȡֵΪCSUDITunerSignalInfoType_E�е�ö��ֵ��ö��������ֵ��򣬶�ӦλΪ1�򷵻ظ���Ϣ��Ϊ0�򲻷��أ���ʱ��Ӧ�ṹ��ֵ�ǲ�ȷ���ġ�
@param[out] psSignalInfo ���������ȡ���ź���Ϣ��
@return 
- �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
- ���ʧ�ܷ������´������:\n
-- CSUDITUNER_ERROR_BAD_PARAMETER ����������\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID���Ƿ��豸ID��\n 
-- CSUDITUNER_ERROR_I2C_ERROR��I2C���ߴ���\n 
@note ��ȡ������Щ��Ϣ���ܻ��漰һЩ��ͬ��Tuner������Ϊ���Ч�ʣ��뾡������ȡ���ע����Ϣ��������ȫ������Ϊ���������£���Ҫ����ϢԽ�࣬�ýӿ���Ҫ��ʱ��Խ��
@note �ýӿ�һ��ҪѸ�ٷ��أ����ػ�ȡ�ǳ���ȷ����Ϣ���������ڸýӿ���ֱ�ӵ������������ӿڲ�ѯ��������udi�㶨ʱ��ѯ�����������Ϣ��ȡ���������ñ��ӿ�ʱֱ�ӻ�ȡ���ɡ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerGetSignalInfo( CSUDI_UINT32 dwTunerId, CSUDI_UINT32 u32SignalInfoMask, CSUDITunerSignalInfo_S *psSignalInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}
 /**
@brief ȡ��Tuner�豸��������Ϣ

@param[in] dwTunerId ���ڱ��Tuner�豸���豸��
@param[out] psTunerInfo ���������ȡ��Tuner������Ϣ
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerGetDeviceInfo( CSUDI_UINT32 dwTunerId, CSUDITunerInfo_S *psTunerInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

/**
@brief ��ȡ�����豸��Id

�������ƶ���Tuner�豸�͹̶����豸
@param[out] pdwIds ���ڴ洢���д��ڵ��豸��Id
@param[in] nMaxIdCnt pdwIds����Ĵ�С,������ܷ��ض����豸Id
@param[out] pnActIdCnt ʵ�ʵ��豸ID��
@return �ɹ�����CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ
@note
- Ϊ���ּ��ݣ����зǿ��ƶ��豸���豸���Դ�0��ʼ����
- ����pdwIdsΪCSUDI_NULL����������±�ʾӦ�ý���ע�ж��ٸ�tuner�豸�������������Ǿ����Id

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ����TunerӲ����Pid

- ����m_bPIDFilterSupportΪCSUDI_TRUE��Tuner������ͨ���ýӿ�����Ӳ����Pid�󣬷��ɻ�ȡ���ݡ�
- ����m_bPIDFilterSupportΪCSUDI_FALSE��Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED�����Ҳ���Ҫ���øýӿڼ��ɻ�ȡ��������
@param[in] dwTunerId Tuner�豸��
@param[in] nPID ����ӹ��˵�����Pid����Ӻ��Pid�����ݿ�ͨ�������Ӧ�ĺ���ģ�飨һ��ΪDemux��
@return �ɹ�����CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ
@note ��ʼ״̬�£�Tuner��ͨ���κ�Pid������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerAddPID (CSUDI_UINT32 dwTunerId ,int nPID)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ɾ��TunerӲ����Pid

- ����m_bPIDFilterSupportΪCSUDI_TRUE��Tuner������ͨ���ýӿ�ɾ��CSUDITunerAddPID�ӿ����ӵ�Ӳ����Pid����ֹ��ȡ��ӦPID�����ݡ�
- ����m_bPIDFilterSupportΪCSUDI_FALSE��Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED�����Ҳ���Ҫ���øýӿڼ��ɻ�ȡ��������
@param[in] dwTunerId Tuner�豸��
@param[in] nPID ��ɾ�����˵�����Pid��ɾ�����Pid�����ݲ���ͨ�������Ӧ�ĺ���ģ�飨һ��ΪDemux��
@return �ɹ�����CSUDI_SUCCESS��ʧ�ܷ��ش������ֵ
@note nPIDΪ~0��ʾɾ����Tuner�µ�����pid���ˣ����˺��޷�ͨ����Tuner��ȡ�κ����ݣ������ٴε���CSUDITunerAddPID��
@note ��ʼ״̬�£�Tuner��ͨ���κ�Pid������
@see CSUDITunerAddPID

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerRemovePID (CSUDI_UINT32 dwTunerId ,int nPID)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/*-----------------���½ӿڶ�����ƶ�Tuner����ؽӿ�---------------------*/
/**
@brief ����豸֪ͨ�ص�����

���RDIģ��CSUDIRDIAddCallback_F�ӿڶ��壬�����ڴ���TUNER�����͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,���豸״̬�����仯ʱ���øú���
@param[in] pvUserData �û�����,����Ϊ�ص�������pvUserData��������
@return �ɹ���ӷ���CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��ӻص�ǰ�������豸�仯��Ϣ,���Ჹ��֪ͨ.��������ʱ��Щ�豸�ʹ���,���ñ�����ʱ����õ�֪ͨ.
@note ��ƽ̨����Ҫ֧���κο��ƶ�Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ɾ���豸֪ͨ�ص�����

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ���TUNER�����͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,֮ǰע��ĺ���
@param[in] pvUserData �û�����,������ע��ʱ��һ��
@return �ɹ�ɾ������CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note fnCallback��pUserDataһ��Ψһȷ��һ���ص�����
@note ��ƽ̨����Ҫ֧���κο��ƶ�Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȫ�Ƴ����ƶ�Tuner�豸

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ���TUNER�����͵Ŀ��ƶ��豸
@param[in] dwTunerId Ŀ���豸ID
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note
- ֻ��ɾ�����ƶ��豸����ͼɾ�������豸�����򷵻�CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED��ͨ���豸���豸Id���ж��豸���ͣ�ɾ���豸�󲻿���ͨ�����豸Id��ȡ�豸��Ϣ���û����øýӿڻᴥ��EM_UDIRDI_PLUGOUT_SAFE���͵�PLUGOUT�¼��� 
- ��ƽ̨����Ҫ֧���κο��ƶ�Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerRDIRemoveDev(CSUDI_UINT32 dwTunerId)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief �ӿ��ƶ�Tuner��ȡ����

�ɹ���Ƶ�����ù���PID(�����Ҫ)�󣬿�ͨ���ýӿڶ�ȡ��TS������
@param[in] dwTunerId Ŀ���豸Id
@param[in] pvData ָ��������ݵ�Buffer
@param[in] nBuffLength pvData�ĳ���
@param[out] pnReadLength ʵ�ʶ��������ݳ���
@return �ɹ��򷵻�CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note �ӿ�Ӧ���̷��أ������ܶ����κ����ݣ���*pnReadLength==0�����Է��سɹ�
@note ���RDIģ����CSUDIRDIIORead_Fԭ��
@note ��ƽ̨����Ҫ֧���κο��ƶ�Tuner���ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerIORead(CSUDI_UINT32 dwTunerId,void* pvData,int nBuffLength,int * pnReadLength)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡһ��PlpID,PlpIDΪ0~255������

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[out] pbPlpIds ���ڷ�PLP ID�����飬�����СӦ������256����������ڿ��õ�PLP ID�������������С�����ش���
@param[in] nPlpIdsCount pbPlpIdsָ��������С
@param[out] pnRetIdCount ���ڷ���ʵ�ʱ��浽pbPlpIds�е�PlpID����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����롣\n
-- CSUDITUNER_ERROR_BAD_PARAMETER����������\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID���Ƿ�ID��\n
-- CSUDITUNER_ERROR_I2C_ERR��I2C���ߴ���\n
@note ���ӿ�ΪDVB-T2ר�á�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerT2GetPlpInfo(CSUDI_UINT32 dwTunerId, CSUDI_UINT8* pbPlpIds, int nPlpIdsCount, int* pnRetIdCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief ��ȡ��ǰ��Ƶͷ������״̬

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[out] pnLockStatus ���ڷ��ص�ǰ������״̬��*pnLockStatus == 0 ��ʾδ������*pnLockStatus == 1��ʾ������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����롣\n
-- CSUDITUNER_ERROR_BAD_PARAMETER����������\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID���Ƿ�ID��\n
-- CSUDITUNER_ERROR_I2C_ERR��I2C���ߴ���\n

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDITunerGetLockStatus(CSUDI_UINT32 dwTunerId, int *pnLockStatus)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

