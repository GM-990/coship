#ifndef _CS_QUICK_SEARCH_H_
#define _CS_QUICK_SEARCH_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"


typedef enum _CSQSMsgType_E
{	
	EM_MSG_QUICKSEARCH_START = 0x1000000,
	EM_MSG_QUICKSEARCH_NIT_COMPLETE, //��ʾNIT�������
	EM_MSG_QUICKSEARCH_NIT_TIMEOUT, //��ʾNIT������ʱ
	EM_MSG_QUICKSEARCH_SEARCH_FINISH, //��ʾ�������
	EM_MSG_QUICKSEARCH_TUNE_LOST //��ʾ��Ƶʧ��
}CSQSMsgType_E;

/**
@brief ��ӿ��������Ļص�����

@param[in] nMsg ���յ�EM_MSG_QUICKSEARCH_END��Ϣ����ʾ������ɡ�
@param[in] dwUserData �û�����
@return 
@note 
*/
typedef void (*CSOnQuickSearchCallback_F)(int nMsg, DWORD dwUserData);

/**
@brief �Ƿ���Ҫ��CSQuickSearchAddCallback���ӵĻص�add�ɹ��Ļص�����

@param[in] nMsg ���յ�EM_MSG_QUICKSEARCH_END��Ϣ����ʾ������ɡ�
@param[in] dwUserData �û�����
@return ����TRUE����CSQuickSearchAddCallback�ص����ʧ�ܣ��������������FALSE,��������CSQuickSearchAddCallback�Ļص�.ֻ��ע��һ���ص�����
@note 
*/
typedef BOOL (*CSOnSearchIsNeedCallback_F)(DWORD dwUserData);

/**
@brief ���п��������ĳ�ʼ���������������ע��tuner�Ļص�

@return �ɹ�����TRUE��ʧ�ܷ���FALSE
*/
BOOL CSQuicksearchInit();

/**
@brief ��ӿ��������Ļص�����

@param[in] fnInfoCallback �ص�����
@param[in] dwUserData1 �û�����1����fnInfoCallbackƥ��
@param[in] fnInfoCallback �ص�����
@param[in] dwUserData2 �û�����,��fnInfoCallbackƥ��
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note 
- ����ӻص���ʱ�򣬻��ȵ���fnInfoCallback����Ļص���
- ���fnInfoCallback�ص����سɹ�����CSQuickSearchAddCallback��ӵ�fnCallback��Ӳ���ȥ�����fnInfoCallback�ص�����ʧ�ܣ���CSQuickSearchAddCallback������ӳɹ�
*/
BOOL CSQuickSearchAddCallback(CSOnQuickSearchCallback_F fnCallback, DWORD dwUserData1, CSOnSearchIsNeedCallback_F fnInfoCallback, DWORD dwUserData2);

/**
@brief ֹͣ������������qsģ�鹦��ʧЧ
@param[in] 
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*/
BOOL CSQuickSearchDisable();

/**
@brief ����qs������Ч
@param[in] 
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*/
BOOL CSQuickSearchEnable();

/**
@brief ɾ��CSQuickSearchAddCallback��ӵĻص�����

@param[in] fnInfoCallback �ص�����
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note 
*/
BOOL CSQuickSearchRemoveCallback(CSOnQuickSearchCallback_F fnCallback);

/**
@brief ���ƺ�̨����

@param[in] bIsUpdate:���ƺ�̨���µ�ʱ���Ƿ�Ҫ���½�Ŀ����;TRUE����ʾ��Ҫ���£�FALSE��ʾ������
@return 
@note 
*/
BOOL CSQuickSearchSetIsUpdateServiceProperty(BOOL bIsUpdate);


#ifdef	__cplusplus
}
#endif

#endif 

