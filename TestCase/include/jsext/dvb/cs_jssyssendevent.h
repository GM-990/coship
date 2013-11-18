/**
Copyright (c) 2005,ShenZhen Coship Electronic Ltd Co.
All rights reserved.

File Name: cs_jssyssendevent.h
Summary: �㶫ʡ����Ϣ����ͷ�ļ�
Current Version: 1.0
Author: Zheng Fen

History:
1.0.1 2012/04/11 1����
*/
#ifndef _CS_SYSSENDEVENT_H_
#define _CS_SYSSENDEVENT_H_


#ifdef  __cplusplus
extern "C" {
#endif

/**
@brief ����CSJSSysSendEventAddCallBack��CSJSSysSendEventRemoveCallBack��ʱ��, ��Ҫ��������͵ĺ���ָ��
@param[in] dwMessageType:���͵���Ϣ�����ͣ�ȡֵ��Χ�μ�JSMessage.hͷ�ļ�����
@param[in]  caJsonStr������Ϣ������Ӧ����Ϣ�ĸ�����Ϣ����json�ַ�������ʽ��ʾ���������Ҫ�˲�������0
@param[in] nLength��json�ַ����ĳ���
@param[in] pvUserData:���������û�����
*/
typedef void  (*CSOnJsSysSendEvent_F)(DWORD dwMessageType, const char* caJsonStr, int nLength, void * pvUserData);

/**
@brief ���js��Ϣ֪ͨ�ص�����

@param[in] pCallback �ص�����,��Ӧ�õ���SysSetting.sendEventʱ���ᴥ���ûص�
@param[in] pvUserData �û�����,����Ϊ�ص�������pvUserData��������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
@note ��ӻص�ǰ������״̬�仯���Ჹ��֪ͨ
*/
int CSJSSysSendEventAddCallBack(CSOnJsSysSendEvent_F pCallBack, void * pvUserData);

/**
@brief ɾ��js��Ϣ֪ͨ�ص�����

@param[in] pCallback �ص�����,֮ǰע��ĺ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
*/
int CSJSSysSendEventRemoveCallBack(CSOnJsSysSendEvent_F pCallBack);
#ifdef __cplusplus
}
#endif


#endif

