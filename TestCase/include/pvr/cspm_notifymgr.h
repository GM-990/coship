/******************************************************************************
FileName:  cs_pvrmgrnotif.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

�ļ�����:
��������Ϣ֪ͨ����ӿ�
֧��һ�Զ��֪ͨ���������̰߳�ȫ��
�벻Ҫ����Ϣ��Ӧ����������߳�

�޸ļ�¼:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      �޽�           Create

*******************************************************************************/
#ifndef _CSPM_NOTIFYMGR_H_
#define _CSPM_NOTIFYMGR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**
	Notify Manager��֪ͨ��Ӧ������ʽ
@param [in] dwNotifyType ֪ͨ�����ͣ������֪ͨ���Ͷ����ɸ���Serviceģ�鶨��
@param [in] pNotifyData  ֪ͨ������
@param [in] pUserData �û�Client�Զ������ݣ���Clientע��ص���Ӧ����ʱ���������
@param [in] hSender ����֪ͨ��Serice����ľ��
*/
typedef int (*CSNOTIFYOnNotify_F)(DWORD dwNotifyType, void *pNotifyData, void* pUserData, HCSHANDLE hSender);

/**
	����һ����Ϣ֪ͨ������
@return ��Ϣ֪ͨ�������ľ��
@note ����Service����
*/
HCSHANDLE CSNOTIFYCreate(void);

/**
	������Ϣ֪ͨ���пͻ���
@param [in] hNotifyMgr ��Ϣ֪ͨ���������
@param [in] dwNotifyType ֪ͨ�����ͣ������֪ͨ���Ͷ����ɸ���Serviceģ�鶨��
@param [in] pNotifyData  ֪ͨ������
@param [in] hSender ����֪ͨ��Serice����ľ��
\note ��service����
*/
void CSNOTIFYNotifyClient(HCSHANDLE hNotifyMgr, DWORD dwNotifyType, void *pNotifyData,HCSHANDLE hSender);

/**
	������Ϣ֪ͨ������
@param [in] hNotifyMgr ��Ϣ֪ͨ���������
\note ��service����
*/
void CSNOTIFYDestroy(HCSHANDLE hNotifyMgr);

/**
	�����Ϣ��Ӧ����
@param [in] hNotifyMgr ��Ϣ֪ͨ���������
@param [in] fnOnCallbackMgrEvent ���¼���Ӧ�Ļص�����
@param [in] pUserData �û��Զ�����������ڻص���ԭ�ⲻ���Ĵ��ظ��ͻ���

\note ����Client����
\ͬһ����Ϣ��Ӧ����������Ӷ�Σ���һ��֪ͨʱ���ûص���Ӧ����Ҳ�ᱻ���ö��
*/
BOOL CSNOTIFYAddNotifyCb(HCSHANDLE hNotifyMgr, CSNOTIFYOnNotify_F fnCallback, void* pUserData);

/**
	�Ƴ���Ϣ��Ӧ����
@param [in] hNotifyMgr ��Ϣ֪ͨ���������
@param [in] fnOnCallbackMgrEvent ���¼���Ӧ�Ļص�����
@param [in] pUserData �û�Client�Զ������ݣ���Clientע��ص���Ӧ����ʱ���������

\note ���ɿͻ��˵��ã����ͻ��˵�Destroy�����У����������������в��ܵ���
\��Ҫ��Add������Գ��֣���ͬ�Ը���Ϣ��Ӧ������Add�˼�������ҪRemove����
*/
BOOL CSNOTIFYRemoveNotifyCb(HCSHANDLE hNotifyMgr, CSNOTIFYOnNotify_F fnCallback, void* pUserData);

/**
	ɾ����Ϣ��Ӧ����
@param [in] hNotifyMgr ��Ϣ֪ͨ���������
@param [in] fnOnCallbackMgrEvent ���¼���Ӧ�Ļص�����
@param [in] pUserData �û�Client�Զ������ݣ���Clientע��ص���Ӧ����ʱ���������

\note ���ɿͻ��˵��ã����ڿͻ��˵�Destroy�����У����������������е���
\not ��Ϣ��Ӧ������Add���ʱ�����ñ�����һ�μ���ɾ��
*/
BOOL CSNOTIFYDelClient(HCSHANDLE hNotifyMgr, CSNOTIFYOnNotify_F fnCallback, void* pUserData);

#ifdef __cplusplus
}
#endif

#endif

