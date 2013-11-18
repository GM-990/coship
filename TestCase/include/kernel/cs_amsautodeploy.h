

#ifndef _AMS_AUTODEPLOY_H_
#define _AMS_AUTODEPLOY_H_
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif





/**@brief Ӧ�ò����¼�*/
typedef enum _CSAMSAutoDeployEvent_E
{
	//����Ӧ���嵥
	EM_AMS_DEPLOY_REQUEST_APPLIST_START=1,  //json �ַ���Ϊ��
	EM_AMS_DEPLOY_REQUEST_APPLIST_OK=2,        //json�ַ�����ʽ{"UpdateCount":123,"deleteCount":123,"dowloadCount":123};
	EM_AMS_DEPLOY_REQUEST_APPLIST_FAILED=3,  //json�ַ�����ʽ{"resion":"network failed"};

	//����Ӧ��
	EM_AMS_DEPLOY_UPDATE_START=4,     //json�ַ�����ʽ{"appName":"portal","version":"1.5.1","UpdatedCount":123};
	EM_AMS_DEPLOY_UPDATE_OK=5,           //json�ַ�����ʽ{"appName":"portal","version":"1.5.1","UpdatedCount":123};
	EM_AMS_DEPLOY_UPDATE_FAILED=6,    //json�ַ�����ʽ{"appName":"portal","resion":"network failed"};

	//����Ӧ��
	EM_AMS_DEPLOY_DOWNLOAD_START=7, //json�ַ�����ʽ{"appName":"portal","version":"1.5.1","dowloadedCount":123};
	EM_AMS_DEPLOY_DOWNLOAD_OK=8,      //json�ַ�����ʽ{"appName":"portal","version":"1.5.1","dowloadedCount":123};
	EM_AMS_DEPLOY_DOWNLOAD_FAILED=9,//json�ַ�����ʽ{"appName":"portal","resion":"network failed"};

	//ɾ��Ӧ��
	EM_AMS_DEPLOY_DELETE_START=10,   //json�ַ�����ʽ{"appName":"portal","version":"1.5.1","deletedCount":123};
	EM_AMS_DEPLOY_DELETE_OK=11,         //json�ַ�����ʽ{"appName":"portal","version":"1.5.1","deletedCount":123};
	EM_AMS_DEPLOY_DELETE_FAILED=12,   //json�ַ�����ʽ{"appName":"portal","resion":"network failed"};

	//�������
	EM_AMS_DEPLOY_FINISH=13              //json�ַ���ʽ{"UpdatedCount":123,"deletedCount":123,"dowloadedCount":123};
}CSAMSAutoDeployEvent_e;


/**
@brief Ӧ�ò����¼��ص�֪ͨ
@param[in] eEvent  �¼�����
@param[in] pcJson   json �ַ���
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
*/
typedef void (*CSAMSAutoDeployOnAction_Fn) (CSAMSAutoDeployEvent_e eEvent, const char* pcJson, DWORD dwLen, void* pvUserData);



/**
@brief ��������
@return �����ɹ�����TRUE�����򷵻�FALSE
*/
BOOL CSAMSAutoDeployInit();

/** 
@brief ֹͣ���� 
@param[in] bReset �Ƿ�ֹͣ����reset����״̬��TRUE--ֹͣ������ղ�����ص���Ϣ;FALSE--ֹͣ���𵫲���ղ�����ص���Ϣ 
*/ 
void CSAMSAutoDeployStop(BOOL bReset); 


/**
@brief ע������ص�
@param[in] fnCb �ص�����
@param[in] pUserData �û��Զ���������ڻص������н������û�
@return �ɹ�����TRUE�����򷵻�FALSE
*/
BOOL CSAMSAutoDeployAddListener(CSAMSAutoDeployOnAction_Fn fnCb, void* pvUserData);

/**
@brief �Ƴ������ص�
@param[in] fnCb �ص�����
*/
void CSAMSAutoDeployRemoveListener(CSAMSAutoDeployOnAction_Fn fnCb);

BOOL CSAMSAutoDeployStart(); 

BOOL CSAMSAutoDeployStartDeploy();

BOOL CSAMSAutoDeployCancelDeploy();


#ifdef __cplusplus
}
#endif

#endif


