

#ifndef _AMS_ASSOCIATEAPP_H_
#define _AMS_ASSOCIATEAPP_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "mmcp_typedef.h"

/**@brief ��Ŀ����Ӧ���¼�*/
typedef enum _CSAMSAssociateAppEvent_E
{
	EM_AMS_ASSOCIATEAPP_DETECTED         = 0,          //��⵽��Ŀ����Ӧ��
	EM_AMS_ASSOCIATEAPP_LOAD_BEGIN       = 1,         //����Ӧ�ÿ�ʼ����,pcAppidΪ��Ӧ��ID
	EM_AMS_ASSOCIATEAPP_LOAD_SUCCESS     = 2,         //����Ӧ�����سɹ�,pcAppidΪ��Ӧ��ID
	EM_AMS_ASSOCIATEAPP_LOAD_FAILED      =3           //����Ӧ������ʧ��,pcAppidΪ��Ӧ��ID
}CSAMSAssociateAppEvent_e;


/**
@brief ��Ŀ����Ӧ���¼��ص�֪ͨ
@param[in] eEvent �¼�����
@param[in] pcAppid Ӧ��ID
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
*/
typedef void (*CSAMSAssociateAppOnAction_Fn) (CSAMSAssociateAppEvent_e eEvent, const char* pcAppid, void* pvUserData);


/**
@brief ��������Ŀ����Ӧ��
@return �����ɹ�����TRUE�����򷵻�FALSE
*/
BOOL CSAMSAssociateAppInit();


/**
@brief ����AitManager
@param[in] pcLocator ��ǰ���ڲ��ŵĽ�Ŀ��locator
*/
BOOL CSAMSAssociateAppAitManagerStart(const char *pcLocator);


/**
@brief ֹͣAitManager
*/
void CSAMSAssociateAppAitManagerStop();


/**
@brief ע������ص�
@param[in] fnCb �ص�����
@param[in] pUserData �û��Զ���������ڻص������н������û�
@return �ɹ�����TRUE�����򷵻�FALSE
*/
BOOL CSAMSAssociateAppAddListener(CSAMSAssociateAppOnAction_Fn fnCb, void* pvUserData);

/**
@brief �Ƴ������ص�
@param[in] fnCb �ص�����
*/
void CSAMSAssociateAppRemoveListener(CSAMSAssociateAppOnAction_Fn fnCb);


#ifdef __cplusplus
}
#endif

#endif


