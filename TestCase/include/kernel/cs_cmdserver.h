#ifndef _CS_CMDSERVER_H_
#define _CS_CMDSERVER_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief ��Ӧ������Server�յ�������
@param[in] pvCmd ָ����յ����������ָ��
@param[out] result_p ָ������ַ��������ָ�룬���Բ�����
@return �ɹ�����1���쳣����0
@note 
- ����ͨ��pvCmdָ�������λ�ȡ���յ���ÿ����������Ϣ������Ҫ���������ƥ��\n
-����,�ͻ��˷��͵���������˳������Ϊint int string����������ε���\n
-CSCLIGetInteger (pvCmd, 0, &nRet)	\n
-CSCLIGetInteger (pvCmd, 0, &nRet)	\n
-CSCLIGetString(pvCmd, "", szRet, sizeof(szRet))	\n
-���ͻ��˷��͵����ݱ��浽��Ӧ��nRet��szRet��\n
-��ϸʹ�ÿɲο�cs_cli.h	\n
*/
typedef int (*CSOnCmdServerProcess_F)(void *pvCmd, char *pcResult);

/**
@brief ��Ӧ��������ģ���ʼ������

@param[in] nPluginId �ò����Ӧ��ID��
@return �ɹ�����1���쳣����0
@note 
-
*/
typedef int (*CSOnCmdServerInitPlugin_F)(int nPluginId);

/**
@brief ���ʵ���ṹ��
@param[in] m_szModuleName ע��֧�ֵ�ģ����������AVPlayer, DeviceManager...
@param[in] m_szModuleCmdFormat �����cmd��ʽ,��Ҫ���ղ�ͬ��ģ��涨�ĸ�ʽ���룬����AVPlayer�������ʽ����AVPlayer 0 0 0 "......" 1 0 0\n 
@param[out] m_fnOnCmdServerAddCLI ע��� CLIģ������ִ�к��������cs_cli.h��CSCLIRegisterCommand˵��
@param[out] m_fnOnCmdServerAddInitPlugin ע����֧�ֵĲ��ģ��ĳ�ʼ������
@note 
-֧�ֵ�������ع��ܵ�ʵ����������Ƶ����AVPlayer���豸����DeviceManager��\n
-��֧����չ��������ʵ��CSOnCmdServerProcess_F��CSOnCmdServerInitPlugin_F	\n
*/
typedef struct 
{
	char* m_szModuleName;
	char* m_szModuleCmdFormat;
	CSOnCmdServerProcess_F m_fnOnCmdServerAddCLI;
	CSOnCmdServerInitPlugin_F	m_fnOnCmdServerAddInitPlugin;
}CSCmdInterface_S;


/**
@brief ע������Ϣ�ṹ��
*/
typedef struct 
{
	int m_nPluginId;
	BOOL m_bUsed;
	CSCmdInterface_S m_sCmdInterface;
	void * m_pUsr;
}CSCMDServeRegPlugin_S;

/**
@brief ��ʼ��CMDServer��������socket��ز�����CLIģ��

@param[in] nServerPort �������˼����Ķ˿ںţ���0ΪĬ��ֵ6463
@return ��ʼ���ɹ�����1���쳣����0
@note 
- 
*/
int CSCMDServerInit(int nServerPort);

/**
@brief ע��CMDServerģ��֧�ֵ�ĳһ�ֲ����һ��ֻ��ע��һ�����

@param[in] psCmdServerInter :֧�ֵĲ��ʵ���ṹ�壬���CSCmdInterface_S
@return ע��ɹ�����1��ʧ�ܷ���0
@note 
- 
*/
int CSCMDServerRegister(CSCmdInterface_S *psCmdServerInter);


/**
@brief ��ͻ��˻ط���Ϣ

@param[in] nServerId :��Ӧ���ʵ���ľ��
@param[in] pvData :�ط��ͻ��˵�����
@param[in] nDataLen :�ط��ͻ��˵����ݴ�С
@return ���ͳɹ�����1��ʧ�ܻ�ʱ����0
@note 
- 
*/
int CSCMDServerSend(int nPluginId, void *pvData, int nDataLen);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
