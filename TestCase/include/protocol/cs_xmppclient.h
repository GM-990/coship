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


/**@brief  回调消息类型 */
typedef enum
{
    EM_XMPP_LOGIN_OK = 0,                      ///连接成功
    EM_XMPP_LOGOUT_OK,                        ///连接断开
    EM_XMPP_MSG_CHAT,                           ///有聊天消息到来，消息格式见CSXMPPChatMsgInfo_S
    EM_XMPP_MSG_ACK,                            ///应答消息
    EM_XMPP_MSG_ROSTER,                     ///获取到好友列表
    EM_XMPP_PINGPONG,                           ///心跳包回复
    EM_XMPP_OTHER = 1000                       ///其他消息
}CSXMPPMsgType_E;

/**@brief  聊天消息内容 */
typedef struct
{
    unsigned char *m_pucMsg;                      ///消息内容，ASCII码字符串
    int m_nLen;                                                   ///消息长度
}CSXMPPChatMsgInfo_S;

/**
@brief 创建一个Client
- 成功返回Client句柄。
- 失败返回NULL。
*/
HCSHANDLE CSXMPPClientCreate();

/**
@brief 销毁一个Client
@param[in] hClient Client的句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSXMPPClientDestroy(HCSHANDLE hClient);

/**
@brief Client登录服务器
@param[in] hClient Client句柄
@param[in] pcDomain 登录服务器的域名，最长不能超过1024
@param[in] pcUserName 登录用户名，最长不能超过1024
@param[in] pcPwd 用户名密码，最长不能超过64
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
@note:一个client连接可以有多个用户登录，但是同一时间只允许一个用户登录。
如果还需用hClient再次登陆，需先把先前的CSXMPPClientLogout掉。
*/
int CSXMPPClientLogin(HCSHANDLE hClient, const char *pcDomain, const char * pcResource, const char *pcUserName, const char *pcPwd);

/**
@brief Client从服务器登出
@param[in] hClient Client句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSXMPPClientLogout(HCSHANDLE hClient);

/**
@brief 发送消息，异步函数，立即返回
@param[in] hClient Client句柄
@param[in] psMsg 发送的消息内容
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSXMPPClientSendMsgToServer(HCSHANDLE hClient, CSXMPPChatMsgInfo_S* psMsg);

/**
@brief 发送消息，同步函数，立即返回
@param[in] hClient Client句柄
@param[in] pcServer 连接服务器名
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSXMPPClientHeartBeat(HCSHANDLE hClient, const char* pcServer);

/**
@brief    回调函数，当有消息到来时，会触发该回调信息，
@param[in] hClient  Client句柄
@param[in] eType 消息类型
@param[out] pPara 消息内容
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
@return 无。
*/
typedef int (*CSXMPPClientMsgNotify_F)(HCSHANDLE hClient, CSXMPPMsgType_E eType, void* pPara, void* pUserData);

/**
@brief	 注册通知回调函数。
@param[in] fnCb 回调处理函数
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSXMPPClientAddCallback(CSXMPPClientMsgNotify_F  fnCb, void* pUserData);

/**
@brief	删除回调函数。
@param[in] fnCb 回调处理函数
@param[in] pUserData 用户自定义数据，注册回调函数时传入的
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSXMPPClientDelCallback(CSXMPPClientMsgNotify_F  fnCb, void* pUserData);

#ifdef __cplusplus
}
#endif

#endif
