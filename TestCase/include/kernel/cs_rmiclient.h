/**@defgroup ��ģ���Զ�̵��ÿͻ��˶���ӿں����ݽṹ�����˶���
@brief RMIClientģ���Ӧ��ͷ�ļ�<cs_rmiclient.h>,������Զ�̿ͻ��˵Ĳ������ͻ��˵Ĳ���Ӧ�úͷ������˲������Ӧ���������˽ӿ���鿴<cs_rmiserver.h>

@version 2.0.1 2011/12/5 
@{
*/

#ifndef _CS_RMICLIENT_H_
#define _CS_RMICLIENT_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSRMIC_IP_LEN 16				

/**@brief  rmiclientģ��������Ϣ*/
typedef struct _CSRMICConfig_S{
	char acServAddr[CSRMIC_IP_LEN];		// ������ip��ַ���ַ�����ʽ����:"127.0.0.1"
	int nServPort;			// �������˿ں�
	int nConnTimeout;		// ��������˽�������ʱ�����ӳ�ʱʱ�䣬��λΪ����
	int nRecvTimeout;		// ��������ʱ�Ľ��ճ�ʱʱ�䣬��λΪ����
	int nSendTimeout;		// ��������ʱ�ķ��ͳ�ʱʱ�䣬��λΪ����
}CSRMICConfig_S;

/**
@brief ��ȡCSRMIClient��Ĭ��������Ϣ
@param[out] psOutConfig ���RMIClientĬ�ϵ�������Ϣ
@note 
*/
void CSRMICGetDefaultConfig(CSRMICConfig_S * psOutConfig);

/**
@brief ����һ��CSRMIClient�ͻ���,�����������������

@param[in] psConfig ������Ϣ��Ĭ�ϵ�������Ϣ��ͨ��CSRMICGetDefaultConfig�ӿڻ�ȡ
@param[out] phClient ���ɵľ������������ʹ�øþ��
@return �ɹ�����TRUE;ʧ�ܷ���FALSE����phThreadΪCSUDI_NULL
*/
BOOL CSRMICCreate(const CSRMICConfig_S * psConfig, CSUDI_HANDLE * phClient);

/**
@brief ����hClient�ͻ���

@param[in] hClient �ͻ��˾������CSRMICCreate���������phClient
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note 
-  ��������һ�����ڱ���������:����CSRMICCallRemoteFunc����hClient������Ϊ�ᵼ�²�ȷ���Ľ��
*/
BOOL CSRMICDestroy(CSUDI_HANDLE hClient);

/**
@brief ����Զ�̵���

@param[in] hClient �ͻ��˾������CSRMICCreate���������phClient��
@param[in] pcModuleName ģ�������ǿ�
@param[in] pcFuncName ���������ǿ�
@param[in] pcParamJson ����������Ϣ���ǿ�
@return �ɹ�:����Զ�̺���ִ�н���ַ���ָ�룻ʧ�ܷ���NULL
@note 
- 1���Է��ص��ַ������д����Ӧͨ��CSRMICFree()�ӿڽ����صĽ���ַ��������ͷ�
- 2��pcModuleNameӦ��Զ�̵��÷������ӿ�CSRMISAddAction��pcModuleName��������һ�¡�
*/
char * CSRMICCallRemoteFunc(CSUDI_HANDLE hClient, const char * pcModuleName, const char * pcFuncName, const char * pcParamJson);

/**
@brief �ͷ�CSRMICCallRemoteFunc���ص��ַ���

@param[in] hClient �ͻ��˾������CSRMICCreate���������phClient��
@param[in] pcJsonStr ִ��CSRMICCallRemoteFunc�������ص��ַ���ָ��
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
*/
BOOL CSRMICFree(CSUDI_HANDLE hClient, char * pcJsonStr);

#ifdef __cplusplus
}
#endif

#endif
