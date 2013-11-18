#ifndef _CS_XMPP_CLIENT_H_
#define _CS_XMPP_CLIENT_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

#define CS_XMPP_MAX_USERNAME_LEN              1024
#define CS_XMPP_MAX_DOMAIN_LEN                     1024
#define CS_XMPP_MAX_RESOURE_LEN                 1024
#define CS_XMPP_MAX_PSW_LEN                            64


/**@brief  �ص���Ϣ���� */
typedef enum
{
    EM_XMPP_LOGIN_OK = 0,                      ///���ӳɹ�
    EM_XMPP_LOGOUT_OK,                        ///���ӶϿ�
    EM_XMPP_MSG_CHAT,                           ///��������Ϣ��������Ϣ��ʽ��CSXMPPChatMsgInfo_S
    EM_XMPP_MSG_ACK,                            ///Ӧ����Ϣ
    EM_XMPP_MSG_ROSTER,                     ///��ȡ�������б�
    EM_XMPP_PINGPONG,                           ///�������ظ�
    EM_XMPP_OTHER = 1000                       ///������Ϣ
}CSXMPPMsgType_E;

/**@brief  ������Ϣ���� */
typedef struct
{
    unsigned char *m_pucMsg;                      ///��Ϣ���ݣ�ASCII���ַ���
    int m_nLen;                                                   ///��Ϣ����
}CSXMPPChatMsgInfo_S;

/**
@brief ����һ��Client
- �ɹ�����Client�����
- ʧ�ܷ���NULL��
*/
HCSHANDLE CSXMPPClientCreate();

/**
@brief ����һ��Client
@param[in] hClient Client�ľ��
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSXMPPClientDestroy(HCSHANDLE hClient);

/**
@brief Client��¼������
@param[in] hClient Client���
@param[in] pcDomain ��¼������������������ܳ���1024
@param[in] pcUserName ��¼�û���������ܳ���1024
@param[in] pcPwd �û������룬����ܳ���64
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
@note:һ��client���ӿ����ж���û���¼������ͬһʱ��ֻ����һ���û���¼��
���������hClient�ٴε�½�����Ȱ���ǰ��CSXMPPClientLogout����
*/
int CSXMPPClientLogin(HCSHANDLE hClient, const char *pcDomain, const char * pcResource, const char *pcUserName, const char *pcPwd);

/**
@brief Client�ӷ������ǳ�
@param[in] hClient Client���
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSXMPPClientLogout(HCSHANDLE hClient);

/**
@brief ������Ϣ���첽��������������
@param[in] hClient Client���
@param[in] psMsg ���͵���Ϣ����
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSXMPPClientSendMsgToServer(HCSHANDLE hClient, CSXMPPChatMsgInfo_S* psMsg);

/**
@brief ������Ϣ��ͬ����������������
@param[in] hClient Client���
@param[in] pcServer ���ӷ�������
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSXMPPClientHeartBeat(HCSHANDLE hClient, const char* pcServer);

/**
@brief    �ص�������������Ϣ����ʱ���ᴥ���ûص���Ϣ��
@param[in] hClient  Client���
@param[in] eType ��Ϣ����
@param[out] pPara ��Ϣ����
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return �ޡ�
*/
typedef int (*CSXMPPClientMsgNotify_F)(HCSHANDLE hClient, CSXMPPMsgType_E eType, void* pPara, void* pUserData);

/**
@brief	 ע��֪ͨ�ص�������
@param[in] fnCb �ص�������
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSXMPPClientAddCallback(CSXMPPClientMsgNotify_F  fnCb, void* pUserData);

/**
@brief	ɾ���ص�������
@param[in] fnCb �ص�������
@param[in] pUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSXMPPClientDelCallback(CSXMPPClientMsgNotify_F  fnCb, void* pUserData);

#ifdef __cplusplus
}
#endif

#endif
