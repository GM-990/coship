#ifndef _CS_SOURCE_TUNER_H_
#define _CS_SOURCE_TUNER_H_

#include "udi2_tuner.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum {
	EM_ST_BEFORE_TUNE,
	EM_ST_AFTER_TUNE
}CSSourceTunerEvent_E;

/**
@brief tuner��Ƶǰ�ص�����ԭ�Ͷ���

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸��
@param[in] punLastDeliver ��һ����Ƶ��Ƶ����Ϣ
@param[in] punNewDeliver ������Ƶ��Ƶ����Ϣ
@param[in] punLastDeliver �û��Զ�������
*/
typedef  void ( *CSSourceTunerCallback_Fn)(CSSourceTunerEvent_E eEvent, CSUDI_UINT32 dwTunerId, const CSUDITunerSRCDeliver_U* punLastDeliver, const CSUDITunerSRCDeliver_U* punNewDeliver, void *pvUserData);

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
*/
CSUDI_Error_Code CSSourceTunerConnect(CSUDI_UINT32 dwTunerId, const CSUDITunerSRCDeliver_U* punDeliver);

 /**
@brief ע��Tuner��Ƶǰ��Ϣ�ص�

@param[in] fnOnBeforeTune �ص����������
@param[in] pvUserData �û�������Զ���ָ��,������¼���ظ��û���������Ϣ��
@return ����ɹ�����CSUDI_SUCCESS, ʧ�ܷ��ش������
@note
- �ص�������ע���������֧��8��
*/
CSUDI_Error_Code CSSourceTunerAddCallback(CSSourceTunerCallback_Fn  fnCallback, void *pvUserData );

/**
@brief 	�Ƴ��ص�
@param[in] fnOnBeforeTune  �ص�����ָ��
@return �ɹ�����CSUDI_SUCCESS,ʧ���򷵻ش�����ֵ
*/
CSUDI_Error_Code CSSourceTunerRemoveCallback(CSSourceTunerCallback_Fn  fnCallback);

/**
@brief 	����sourceTuner������Ƶ��
@param[in] punDeliver:Ƶ���б�
@param[in] nDeliveryCnt: Ƶ�����
@return �ɹ�����CSUDI_SUCCESS,ʧ���򷵻ش�����ֵ
*/
CSUDI_Error_Code CSSSourceTunerSetDeliver(const CSUDITunerSRCDeliver_U* punDeliver, int nDeliveryCnt);

#ifdef __cplusplus
}
#endif

/** @} */

#endif 


