/**
Copyright (c) 2012,ShenZhen Coship Electronic Ltd Co.
All rights reserved.

File Name: cs_networkinput.h
Summary: �����н�������ң�����Ŀ���
Current Version: 0.0.1
Author: gongxing

History:
0.0.1 2012/05/07 ���壬����UDI2ģ����ң��ע�ᡢɾ�������ص��ķ�ʽ����õĽӿڣ��̳�UDI2�İ�������
*/

#ifndef _CS_NETWORK_INPUT_H_
#define _CS_NETWORK_INPUT_H_

#include "udi2_input.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
@brief ע�����簴���ص�����

���簴���ļ�ֵ��Ϣ��ͨ��ע��Ļص�����֪ͨӦ�ò�\n
@param[in] fnInputCallback ָ��ص������ĺ���ָ�룬�û�ͨ���ص������õ�������Դ��״̬�͹�����
@param[in] pvUserData �û����ݣ�����ΪCSUDI_NULL
@return ע��ɹ��򷵻�CSUDI_SUCCESS ;ע��ʧ���򷵻���ش������
@note 
- ֧��ע��Ļص�����������֧��8���������
- �ﵽ���ص�������������CSUDIINPUT_ERROR_CALLBACK_FULL����
- fnInputCallback��pvUserDataΨһ��ʶһ���ص����ͣ�������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIINPUT_ERROR_ALREADY_ADDED����

@note Booter��ص������������ж��е��ã��ϲ�ᰴ�ж�ʹ��ԭ��ʹ�ûص�
*/
CSUDI_Error_Code CSNetworkInputAddCallback(CSUDIInputCallback_F fnInputCallback ,void * pvUserData);

/**
@brief ɾ�����簴���ص�����

@param[in] fnInputCallback ָ��ص������ĺ���ָ��
@param[in] pvUserData �û�����
@return ɾ���ɹ��򷵻�CSUDI_SUCCESS;ɾ��ʧ���򷵻���ش������
@note ���Ҫɾ��һ�������ڵĻص����򷵻�CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST
@note fnInputCallback��pvUserDataΨһ��ʶһ���ص�����

@note Booter��ص������������ж��е��ã��ϲ�ᰴ�ж�ʹ��ԭ��ʹ�ûص�
*/
CSUDI_Error_Code CSNetworkInputDelCallback(CSUDIInputCallback_F fnInputCallback ,void * pvUserData);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

