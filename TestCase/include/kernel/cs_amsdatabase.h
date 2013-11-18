/**@defgroup Ӧ�ù��������ݿ�ӿ�
@brief
*/
#ifndef _CS_AMSDATABASE_H
#define _CS_AMSDATABASE_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**@brief ���ݿ��¼�����ö�ٶ���*/
typedef enum _CSAMSDBEventType_E{
	EM_AMSDB_UPDATE = 0,		       ///<���ݿ���£�appid��Ϊ��ʱ��ʾĳ��Ӧ�ø�����
	EM_AMSDB_ADDED = 1,		       ///<���ݿ��������ĳ��Ӧ�ã�appidΪ�����Ӧ�õ�Ӧ��id
	EM_AMSDB_REMOVED = 2             ///<Ӧ�ý��������ݿ����Ƴ���appid Ϊ���Ƴ���Ӧ��id���ڻص��п��Դ����ݿ��л�ȡ��Ӧ��
}CSAMSDBEventType_E;


/**
@brief ���ݿ�仯֪ͨ�ص���������
@param[in] eType ֪ͨ���ͣ���CSAMSDBEventType_E����
@param[in] pcAppid Ӧ��Ψһ��ʶ��
@param[in] pUserData �û��Զ������ݣ��û�ע��ص�����ʱ�����
*/
typedef void (*CSAMSDBOnDbChanged_Fn)(CSAMSDBEventType_E eType,const char* pcAppid,void* pvUserData);


/**
@brief Ӧ�ù�������ʼ��
@param[in] hAMSProp Ӧ��ϵͳ���Ծ��(properties���)
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note Ӧ�ù�������������
*/
BOOL CSAMSInit(HCSHANDLE hAMSProp);


/**
@brief ����Ӧ�ù�����ϵͳ����
@param[in] pcKey ���Լ�ֵ��CSAMSPROP_ϵ�м�ֵ����
@param[in] pcValue ����ȡֵ
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
*/
BOOL CSAMSSetProperty(const char* pcKey, const char* pcValue);

/**
@brief ��ȡӦ�ü�

@param[in] pcFilter  �������ַ������ɴ��ձ�ʾ��ȡ����Ӧ��
@param[out] pOutAppHandleArray  ���Ӧ�þ������
@param[in] nArraySize ��������С
@return �ɹ���ȡ�򷵻ػ�ȡ�õ���Ӧ�ø�����ʧ���򷵻�-1
@note ������֧��[key=value]����ʽ��key��ȡֵΪ"state" �������е�Ӧ�������ֶ�
@note ����: type==java_cdc��ȡ����javacdcӦ��
*/
int CSAMSDBGetApps(const char* pcFilter,HCSHANDLE* phOutAppHandleArray,int nArraySize);

/**
@brief ��ȡӦ�þ��
@param[in] pcAppId Ӧ��id
@return ����Ӧ�þ��
*/
HCSHANDLE CSAMSDBGetApp(const char* pcAppid);

/**
@brief ע������ص�
@param[in] fnCb �ص�����
@param[in] pUserData �û��Զ���������ڻص������н������û�
@return �ɹ�����TRUE�����򷵻�FALSE
*/
BOOL CSAMSDBAddListener(CSAMSDBOnDbChanged_Fn fnCb,void* pvUserData);

/**
@brief �Ƴ������ص�
@param[in] fnCb �ص�����
*/
void CSAMSDBRemoveListener(CSAMSDBOnDbChanged_Fn fnCb);

/**
@brief ɾ��Ӧ��(�Ӵ洢�豸�г���ɾ��)
@param[in] hAppHandle Ӧ�þ��
@param[in] bDeleteAppFile �Ƿ�ɾ��Ӧ���ļ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSAMSDBDeleteApp(HCSHANDLE hAppHandle, BOOL bDeleteAppFile);

/**
@brief ���һ��Ӧ��
@param[in] pcFilePath Ӧ�ô洢�ļ���·��
@return ����Ӧ�þ��
*/
HCSHANDLE CSAMSDBAddApp(const char* pcfilePath);


/**
@brief ��װӦ��
@param[in] pcAppUrl Ӧ��zip����url(Զ�����ӻ򱾵��ļ�)
*/
void CSAMSDBInstallApp(const char* pcAppUrl);

/**
@brief ����Ӧ��
@param[in] pcAppUrl Ӧ��zip����url(Զ�����ӻ򱾵��ļ�)
@param[in] pcAppId Ӧ�õ�ID
*/
void CSAMSDBUpgradeApp(const char* pcAppUrl, const char* pcAppId);

/**
@brief ��ȡ��Ӧ��
@return hMainApp �ɹ�������Ӧ�þ��,ʧ�ܷ���NULL
*/
HCSHANDLE CSAMSDBGetMainApp();


#ifdef  __cplusplus
}
#endif

#endif

