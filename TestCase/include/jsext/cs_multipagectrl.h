#ifndef CS_MULTIPAGECTRL_H
#define CS_MULTIPAGECTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

typedef enum
{
    EM_MPC_MAIN_PAGE,
    EM_MPC_VOIP_PAGE,
    EM_MPC_MOTO_PAGE,
    EM_MPC_MSG_PAGE,
    EM_MPC_VOICEINPUT_PAGE,
    EM_MPC_PAGE_COUNT
} CSJSMPCPages_E;

typedef enum
{
    EM_MPC_INIT,
    EM_MPC_VISIBLE,
    EM_MPC_HIDE,
    EM_MPC_UNKNOWN
}MPCPageState_E;

/**
@brief 	��ʼ��MultiPageCtrlģ��

@return TRUE��ʾ�ɹ���FALSE��ʾʧ��
@note:
*/
BOOL CSJSMPCInit(void);

/**
@brief 	����ָ����page

@param[in] ePage:ָ���򿪵�page������
@return TRUE��ʾ�ɹ���FALSE��ʾʧ��
@note:
*/
BOOL CSJSMPCCreate(CSJSMPCPages_E ePage, const char* pcUrl);

/**
@brief 	����ָ����page

@param[in] ePage:ָ���򿪵�page������
@return TRUE��ʾ�ɹ���FALSE��ʾʧ��
@note:
*/
BOOL CSJSMPCDestroy(CSJSMPCPages_E ePage);

/**
@brief 	ʹָ����page��ʾ���������Ҵ򿪴����url

@param[in] ePage:ָ���򿪵�page������
@param[in] pcUrl:Ҫ�򿪵�Ŀ��ҳ�棬������գ���ʾ��ָ��page����ҳ
@param[in] nLength:pcUrl�ĳ���
@return TRUE��ʾ�ɹ���FALSE��ʾʧ��
@note:
*/
BOOL CSJSMPCShow(CSJSMPCPages_E ePage, const char *pcUrl, int nLength);

/**
@brief 	����ָ����page

@param[in] ePage:ָ���򿪵�page������
@return TRUE��ʾ�ɹ���FALSE��ʾʧ��
@note:
*/
BOOL CSJSMPCHide(CSJSMPCPages_E ePage);

/**
@brief 	��ȡָ����page

@param[in] ePage:ָ����page������
@return handle
@note:
*/
HCSHANDLE CSJSMPCGetPageHandle(CSJSMPCPages_E ePage);

/**
@brief 	��ȡ��ǰ��ʾ��page

@return ��ǰpage
@note:
*/
CSJSMPCPages_E CSJSMPCGetCurPage();

/**
@brief 	��ȡpage��״̬

@return page��״̬
@note:
*/
MPCPageState_E CSJSMPCGetPageState(CSJSMPCPages_E ePage);

/**
@brief 	��ȡpage��name

@param[in] ePage:ָ���򿪵�page������
@param[out] pcPageName:��ȡָ��page��pageName
@param[in] nPageNameLen:�����pageName�ַ�������
@return TRUE��ʾ�ɹ���FALSE��ʾʧ��
@note:
*/
BOOL CSJSMPCGetPageName(CSJSMPCPages_E ePage, char *pcPageName, int nPageNameLen);

/**
@brief 	��ȡpage��pageUrl

@param[in] ePage:ָ���򿪵�page������
@param[out] pcPageName:��ȡָ��page��pageUrl
@param[in] nPageNameLen:�����pageUrl�ַ�������
@return TRUE��ʾ�ɹ���FALSE��ʾʧ��
@note:
*/
BOOL CSJSMPCGetPageUrl(CSJSMPCPages_E ePage, char *pcPageUrl, int nPageUrlLen);

/**
@brief 	����page��pageUrl

@param[in] ePage:ָ���򿪵�page������
@param[out] pcPageName:��ȡָ��page��pageUrl
@param[in] nPageNameLen:�����pageUrl�ַ�������
@return TRUE��ʾ�ɹ���FALSE��ʾʧ��
@note:
*/
BOOL CSJSMPCSetPageUrl(CSJSMPCPages_E ePage, char *pcPageUrl, int nPageUrlLen);

#ifdef __cplusplus
}
#endif

#endif 

