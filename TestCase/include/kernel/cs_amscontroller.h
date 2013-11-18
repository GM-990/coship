/**@defgroup Ӧ�ÿ������ӿ�
@brief
*/
#ifndef _CS_AMSCONTROLLER_H
#define _CS_AMSCONTROLLER_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**@brief Ӧ�ò���ö�ٶ���*/
typedef enum _CSAMSAction_E{
	EM_APPACTION_START = 0,	///<����Ӧ��
	EM_APPACTION_PAUSE = 1,	///<��ͣӦ��
	EM_APPACTION_RESUME = 2,	///<�ָ�Ӧ��
	EM_APPACTION_STOP = 3,		///<ֹͣӦ��
	EM_APPACTION_INIT = 4		///<��ʼ��Ӧ��
}CSAMSAction_E;

/**@brief Ӧ�ò���״̬*/
typedef enum _CSAMSActionStatus_E{
	EM_APPACTION_TYPE_SUCCEED = 0,	///<����ִ�гɹ�֪ͨ
	EM_APPACTION_TYPE_BEFORE = 1,	///<����ִ��ǰ֪ͨ
	EM_APPACTION_TYPE_FAILED = 2,		///<����ִ��ʧ��֪ͨ
	EM_APPACTION_TYPE_BLOCKED = 3	///<����ִ�ж���֪ͨ
}CSAMSActionStatus_E;

/**
@brief Ӧ�ò����ص�֪ͨ
@param[in] eAction ��������
@param[in] eActionType ����״̬
@param[in] pcAppId 
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
*/
typedef void (*CSAMSCTRLOnAction_Fn) (CSAMSAction_E eAction, CSAMSActionStatus_E eActionType, const char *pcAppId, void* pvUserData);

/**
@brief ����Ӧ��
@param[in] pcAppId Ӧ��id
@param[in] pcParam ����������Asii�ַ�������Ϊ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE(��Ӧ�ò�����ʱ)
@note Ӧ�ò��������첽ִ�еġ���������ʧ�ܻ�ɹ�����ʶ�����Ƿ�ɹ���������ͬ
*/
BOOL CSAMSCTRLStartApp(const char* pcAppId,const char* pcParam);

/**
@brief ��ͣӦ��
@param[in] pcAppId Ӧ��id
@return �ɹ�����TRUE,ʧ�ܷ���FALSE(��Ӧ�ò�����ʱ)
@note Ӧ�ò��������첽ִ�е�
*/
BOOL CSAMSCTRLPauseApp(const char* pcAppId);

/**
@brief ֹͣӦ��
@param[in] pcAppId Ӧ��id
@return �ɹ�����TRUE,ʧ�ܷ���FALSE(��Ӧ�ò�����ʱ)
@note Ӧ�ò��������첽ִ�е�
*/
BOOL CSAMSCTRLStopApp(const char* pcAppId);

/**
@brief ֹͣϵ��Ӧ��
@param[in] ppcAppIds Ӧ��id����
@param[in] nCount Ӧ��id����
@return �ɹ�����TRUE,ʧ�ܷ���FALSE(������Ӧ�ò�����ʱ�򷵻�FALSE)
@note Ӧ�ò��������첽ִ�еġ�
*/
BOOL CSAMSCTRLStopApps(const char** ppcAppIds,int nCount);

/**
@brief �ָ�Ӧ��
@param[in] pcAppId Ӧ��id
@return �ɹ�����TRUE,ʧ�ܷ���FALSE(��Ӧ�ò�����ʱ)
@note Ӧ�ò��������첽ִ�е�
*/
BOOL CSAMSCTRLResumeApp(const char* pcAppId);

/**
@brief ��ȡ�������еĽ���Ӧ��
@return ����Ӧ�þ��
*/
HCSHANDLE CSAMSCTRLGetActiveApp();

/**
@brief ��ȡ�����������е�Ӧ���б�(���ӵ�Ӧ�ã����ں�̨���е�Ӧ���޷�ͨ���ýӿڻ�ȡ)
@param[out] pOutAppHandleArray  ���Ӧ�þ������
@param[in] nArraySize ��������С
@return �ɹ���ȡ�򷵻ػ�ȡ�õ���Ӧ�ø�����ʧ���򷵻�-1
*/
int CSAMSCTRLGetRuningApps(HCSHANDLE* phOutAppHandleArray,int nArraySize);

/**
@brief ע������ص�
@param[in] fnCb �ص�����
@param[in] pUserData �û��Զ���������ڻص������н������û�
@return �ɹ�����TRUE�����򷵻�FALSE
*/
BOOL CSAMSCTRLAddListener(CSAMSCTRLOnAction_Fn fnCb,void* pvUserData);

/**
@brief �Ƴ������ص�
@param[in] fnCb �ص�����
*/
void CSAMSCTRLRemoveListener(CSAMSCTRLOnAction_Fn fnCb);


BOOL CSAMSCTRLStartRemoteApp(const char* classPath,const char* mainClass,const char* entryMain,const char* pzAgus);

#ifdef  __cplusplus
}
#endif

#endif

