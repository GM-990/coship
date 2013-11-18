/**@defgroup ��ģ���Զ�̵��÷������˶���ӿں����ݽṹ�����˶���
@brief RMIServerģ���Ӧ��ͷ�ļ�<cs_rmiserver.h>,������Զ�̵��÷������˵Ĳ������������˵Ĳ���Ӧ�Ϳͻ��˲������Ӧ���ͻ��˶˽ӿ���鿴<cs_rmiclient.h>

@version 2.0.1 2011/12/5 
@{
*/


#ifndef _CS_RMISERVER_H_
#define _CS_RMISERVER_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief  rmiserverģ��������Ϣ*/
typedef struct _CSRMISConfig_S{
	int nListenPort;		// �������ļ����˿ں�
	int nBufferSize;		// �������ڴ�Żش�����buffer��С����ֵӦΪ�ش����ݳ��ȵ����ֵ
}CSRMISConfig_S;

/**
@brief action������

@param[in] pcFuncName - Զ�̵��ú�����
@param[in] pcParamJson - ����������Ϣ
@param[out] pOutData - ���ڴ�ź���ִ�н����buffer
@param[in] nDataSize - pOutData�Ĵ�С
@param pUserData �û��Զ������(ע��ص�����ʱ����Ĳ���)
@return
@note 
- 1��pcFuncName��Զ�̵��ÿͻ��˽ӿ�CSRMICCallRemoteFunc��pcFuncName��������һ�¡�
- 2��pcParamJson��Զ�̵��ÿͻ��˽ӿ�CSRMICCallRemoteFunc��pcParamJson��������һ�¡�
- 3��nDataSize�봴��������ʱ��������ò���nBufferSize����һ�¡�
*/
typedef int (*CSRMISOnProcess_F)(
	const char * pcFuncName, const char * pcParamJson, void * pOutData, int nDataSize, void * pUserData);

/**
@brief ��ȡRMIServer��Ĭ��������Ϣ
@param[out] psOutConfig ���RMIServerĬ�ϵ�������Ϣ
@return
@note 
*/
void CSRMISGetDefaultConfig(CSRMISConfig_S * psOutConfig);

/**
@brief ����һ��RMIServer������

@param[in] psConfig ������Ϣ��Ĭ�ϵ�������Ϣ��ͨ��CSRMISGetDefaultConfig�ӿڻ�ȡ
@param[out] phClient ���ɵľ������������ʹ�øþ��
@return �ɹ�����TRUE;ʧ�ܷ���FALSE����phThreadΪCSUDI_NULL
*/
BOOL CSRMISCreate(const CSRMISConfig_S * psConfig, CSUDI_HANDLE * phServer);

/**
@brief ����hServer������

@param[in] hServer �������������CSRMISCreate���������phServer
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note 
-  ��������һ�����ڱ���������:����CSRMISAddAction����hClient������Ϊ�ᵼ�²�ȷ���Ľ��
*/
BOOL CSRMISDestroy(CSUDI_HANDLE hServer);

/**
@brief ���Rmiserver action

@param[in] hServer �������������CSRMISCreate���������phServer
@param[in] pcModuleName ģ����
@param fnCb - action�ص�����
@param pUserData �û��Զ������(�ص�֪ͨʱ�ش����ص�����)
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note 
- һ��ģ���Ӧ��һ��action������Ϊһ��ģ���������actionʱ���ڶ��λ᷵��ʧ�ܡ�
*/
BOOL CSRMISAddAction(CSUDI_HANDLE hServer, const char * pcModuleName, CSRMISOnProcess_F fnOnProcess, void * pUserData);

/**
@brief �Ƴ�Rmiserver action

@param[in] hServer �������������CSRMISCreate���������phServer
@param[in] pcModuleName ģ����
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL CSRMISRemoveAction(CSUDI_HANDLE hServer, const char * pcModuleName);

#ifdef __cplusplus
}
#endif

#endif
