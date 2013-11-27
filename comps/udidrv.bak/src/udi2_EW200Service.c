/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_demux.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------


 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_EW200Service.h"
/**
@brief ��ȡ�ն���Ϣ�ӿ�

Eastwin200��ͨ���ýӿڻ�ȡ�ն˵����к�,���ܿ��ŵ���Ϣ,���ȡ��ҳ�����õ���Ϣ
@param[in] eType ��Ϣ����
@param[in] value eType���Ͷ�Ӧ��ֵ
@param[in] length valueֵ�ĳ���
@return ����ʵ����Ϣ�ĳ���,������-1
@note ��Ҫ���õ���Ϣ��CSUDIEW200SClientInfoType_Eö���е�[r]��ʶ
@note ֧��Coship����Enreach����ʱ��Ҫʵ�ָýӿ�

@code
//�ο�ʵ��(δ���ǻ�����������쳣)
//ע��:�������������value��ʾ�����ͼ��������в�ͬ���밴�ο�����ʵ��/ʹ�ü��ɡ�
if(eType == EM_UDIEW200S_AUDIOCHANNEL)
{
	//EM_UDIEW200S_AUDIOCHANNEL��Ӧ����������int��
	//valueΪint�ı������䳤��Ӧ��Ϊsizeof(int)
	
	value = EM_UDIEW200S_AUD_CHANNEL_STEREO;
	CSUDIEW200SReadClientInfo(eType , (void*)&value, sizeof(int));

}
else if(eType == EM_UDIEW200S_SN)
{
	//EM_UDIEW200S_SN��Ӧ����������CSUDI_UINT8����
	//valueһ��ָ��CSUDI_UINT8���飬�䳤��Ϊlength
	//��valueָ���buffer�л�ȡ��Ӧ����
	value[32];
	CSUDIEW200SReadClientInfo(eType , (void*)value, sizeof(value));

}
@endcode
*/
int CSUDIEW200SReadClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}
/**
@brief �����ն���Ϣ�ӿ�

Eastwin200��ͨ���ýӿڽ�һЩ�û���Ϣ���õ��ն����ô洢����Ӧ���Լ��洢����
@param[in] eType ��Ϣ����
@param[in] value eType���Ͷ�Ӧ��ֵ
@param[in] length valueֵ�ĳ���
@return ����ʵ�ʴ洢��Ϣ�ĳ���,������-1
@note ��Ҫ���õ���Ϣ��CSUDIEW200SClientInfoType_Eö���е�[w]��ʶ
@note ֧��Coship����Enreach����ʱ��Ҫʵ�ָýӿ�
@note �ο�ʵ��ͬCSUDIEW200SReadClientInfo
*/
int CSUDIEW200SWriteClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ͨ��DVB3Ҫ�ز�ѯ����Դ��Ϣ

������ȡ��3Ҫ�ض�Ӧ��Ϣʱ����ʹ�õ�TunerId,DemuxId,��Ƶ��
@param[in] nOrNetId org network id
@param[in] nTsId ts id
@param[in] nServiceId service id
@param[out] psSourceParam ���ڴ洢���������Դ��Ϣ
@return ����ɹ�����CSUDI_SUCCESS, ʧ�ܷ��ش������
@note EW200ͨ���ýӿڲ�ѯһ��Service�Ķ�Ӧ��Ƶ�㼰��Ƶͷ��Ϣ����ƽ̨�ж����Ƶͷ������ӿڵ�ʵ��Ҫ�ر�С�ģ���Ϊ��Ӧ�����ݿ����ڲ�ͬ�ĸ�Ƶͷ�ϣ�����򵥲�ѯ���ݹ�����
*/
CSUDI_Error_Code CSUDIEW200SGetTuneParam(int nOrNetId,int nTsId,int nServiceId,CSUDIEW200SSourceParam_S * psSourceParam)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ��ȡIPQam����Ϣ�ӿ�

������VODģ��
��֤ģ��ͨ�������ն�ʵ�ֵĸýӿڵõ�IPQam������Ϣ
- CITV3.x�汾��VODĿǰȡ����ָ��Section�ĵ�4��5�ֽ�(��data[3],data[4])
@param[in] punDeliverList �����Ϣ����Ƶ���б�����
@param[in] nDeliverCount pDeliverList�����е�Ƶ�����
@param[in] nPid IPQam����Ϣ����Pid
@param[in] ucTableId IPQam����Ϣ����TableId
@param[out] pIpQamDataBuffer ���ڴ��IPQam����Ϣ���ڴ�ָ��
@param[in] nBufferLength pIpQamDataBuffer�ĳ���
@param[in] pvReserved �����ֶ�,Ŀǰ����ΪNULL
@return �ɹ���ȡ����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSUDIEW200SGetIPQamDomainInfo(const CSUDITunerSRCDeliver_U* punDeliverList,
										int nDeliverCount,
										int nPid,
										unsigned char ucTableId,
										char* pIpQamDataBuffer,
										int nBufferLength,
										void* pvReserved
									   )
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ���Ž�Ŀ�ӿ�

�����ƶ�URL�ĸ�ʽ���в���

@param[in] pcURL ����URL
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@code ����URL�ĸ�ʽ�����¼��֣�Ӧ�ÿ��Ը��ݲ�ͬ��URL��ʽ���в��Ŵ���
(1)dvb://OriginalNetworkId.TS_Id.ServiceId  ����ο������ֵ����м�������淶����3���ָ�¼I��1.2.1С�ڡ�
		OriginalNetworkId: original_network_id
		TS_Id: transport_stream_id
		ServiceId: service_id
		����: dvb://1.2.5(ʮ������)
(2)dvbc://Frequency.SymbolRate.Modulation.ServiceId
		Frequency:Ƶ��.��λMHz
		SymbolRate:�����ʣ���λ��Kbps
		Modulation:����ģʽ
		ServiceId:��ĿId
		����: deliver://490000000:6875:64:10(ServiceIdΪʮ������)
(3)avpid://pcrPid.emm.videoPid.audioPid.audio_ecm_pid.video_ecm_pid(ʮ������)	
@endcode
*/
void CSUDIEW200SPlayStream(char* pcURL, int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ֹͣ��Ŀ���Žӿ�

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
*/
void CSUDIEW200SStopPlay(int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ������Ƶ���Ŵ��ڴ�С�ӿ�

@param[in] psRect ��Ƶ���δ�С,ΪNULL��ʾȫ������
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
*/
void CSUDIEW200SSetPlayRect(CSUDIWinRect_S* psRect,int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief �ָ���ͣģʽ 

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���

@return 
- �ɹ�����CSUDI_SUCCESS��
- ʧ�ܷ�������
��֧�ִ˲�������CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
@note �ָ���ͣģʽ;��CSUDIEW200SPause�ɶ�ʹ�á�
*/
CSUDI_Error_Code CSUDIEW200SResume(int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ��ͣ��hPlayer��ص�һ���������Ĺ���

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���

@return 
- �ɹ�����CSUDI_SUCCESS;
- ʧ�ܷ�������
��֧�ִ˲�������CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
@note ��ͣ��hPlayer��ص�һ���������Ĺ�����ֻ�����ڲ���ĳһ��Ŀ��������ֻ����Ƶ������Ƶ��ʱ���ò���Ч��
*/
CSUDI_Error_Code CSUDIEW200SPause(int nPlayId)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}
/**
@brief �����˳�TrickMode

֪ͨ�������������Ƶ���ܻᰴ�������ٶȽ����������
������VOD����ʱ�Ŀ��˿���ȣ����������Tuner����������Ϊ�������ٶȣ�
ƽ̨������׼���Է�ֹ���Ƚ׶μ����Ž׶ε������ˡ�
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@param[in] bTrickMode ΪCSUDI_TRUE��ʾ����TrickMode,ΪCSUDI_FALSE��ʾ�˳�TrickMode
@return 
- �ɹ�����CSUDI_SUCCESS��
��֧�ִ˲�������CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
�Ѿ����ڴ�״̬����CSUDIEW200S_ERROR_TRICKMODE_STATE_ERROR
*/
CSUDI_Error_Code CSUDIEW200STrickMode(int nPlayId,CSUDI_BOOL bTrickMode)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ע��UDI����Ϣ�ص�����

ע���UDI����ص���Ϣ��ͨ���ýӿ�֪ͨ��Ӧ��
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@param[in] pUserData �û�����,���ڻص���ԭ������
@return �ɹ�����CSUDI_SUCCESS,ʧ�ܷ��ش�������CSUDIEW200S_ERRORCODE_E����
@note
- �ص�������ע���������֧��5�������Ӧ��ע��ص�����������֧�ָ���������CSUDIEW200S_ERROR_NO_MEMORY
- һ��nPlayId,fncallback,pUserdata��ͬ����һ���ص���������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIEW200S_ERROR_ALREADY_ADDED
*/
CSUDI_Error_Code CSUDIEW200SAddCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

 /**
@brief ɾ��UDI��ص�������ע��

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@param[in] fnTunerCallback �ص��������
@param[in] pUserData ͬCSUDIEW200SAddCallback�е�pUserData
@return�ɹ�����CSUDI_SUCCESS ʧ�ܴ��󷵻ؼ�CSUDIEW200S_ERRORCODE_E����
*/
CSUDI_Error_Code CSUDIEW200SDelCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_NOT_REQUIRED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
    return Retcode;
}

