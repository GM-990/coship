#ifndef _CS_SAITIONCOSHIP_H_
#define _CS_SAITIONCOSHIP_H_

#include "csshell_saition.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
@brief 	����˦�Ź���ǰ�ĳ�ʼ������
@param[in]  eDeviceType: ˦��ģ��ʹ�õĴ����豸,���嶨���CSSaitionTransDevType_E
@param[in] pvParam: ��ʼ��ʱ��Ҫ����Ĳ���������EM_SAITION_TANSDEVTYPE_BLUETOOTH�����豸����
                                ��������ΪCSSaitionBTHInfo_S*���ͣ���ʾ����sdpע����Ϣ������ EM_SAITION_TANSDEVTYPE_ETHERNET
                                ������ �����˲�������Ϊ*UINT�ͣ���ʾ�����з���˶˿ں�,
@return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
@note 
-����pvParamΪ��ʱ����������CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionInit(CSSaitionTransDevType_E eDeviceType, void *pvParam);

/**
@brief 	����˦�ŷ���, ��CSSaitionInit֮�����
@param[in] uPort: ˦�ŷ���˶˿ڣ��˶˿��ڴ�˦��cfg �Ĺ��������Ǵ�saitionconfig.properties�����ļ��ж�ȡ
@return  �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
@note 
- �����Ҫ����˦�ŷ������ȵ���CSSaitionInit�������ٵ��ô˺����Ϳ����ˡ�
*/
CSUDI_Error_Code CSSaitionStart();

/**
@brief 	ֹͣ˦�ŷ���
@return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
@note 
- 
*/
CSUDI_Error_Code CSSaitionStop();

/**
@brief 	����˦������URL, ��URL��˦���е�"��"�����й�
@param[char*] pcBuildUrl: ˦������URL, pcBuildUrl��������'\0'��β��ASCII�����ַ���, ���ַ����м䲻�ܺ���'\0\.
					   ��pcBuildUrl�ĳ�����С��(1024)�� �����ܽ�˦������URL��ȷ���õ�Cģ������
@return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
@note 
-����pcBuildUrlΪ��ʱ����������CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionSetBuildURL(const char *pcBuildUrl);

/**
@brief 	��ȡ˦������URL
@param[out] pcBuildUrl:  ��Ż�ȡ��˦������URL����
@param[in] nUrlLen:  pcBuildUrl���泤��
@return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
@note 
-����pcBuildUrlΪ��ʱ��������CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionGetBuildURL(char* pcBuildUrl, int nUrlLen);

/**
@brief 	����˦������URL, ��URL��˦�ŵ�"˦" �����й�
@param[in] pcHomeUrl: ˦������URL, pcHomeUrl��������'\0'��β��ASCII�����ַ���, ���ַ����м䲻�ܺ���'\0\.
				     ��pcHomeUrl�ĳ�����С��(1024)�� �����ܽ�˦������URL��ȷ���õ�Cģ������
@return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
@note 
-����pcHomeUrlΪ��ʱ��������CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionSetHomeURL(const char *pcHomeUrl);

/**
@brief 	��ȡ˦������URL
@param[out] pcHomeUrl:  ��Ż�ȡ��˦������URL����
@param[in] nUrlLen:  pcHomeUrl���泤��
@return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
@note 
-����pcHomeurlΪ��ʱ��������CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionGetHomeURL(char* pcHomeUrl, int nUrlLen);

/**
@brief 	�˽ӿ�Ϊ��ʵ�ֻ�����֪ͨ�ֻ�ִ��ĳ�����̡�������ģ����ý�pcContent
		���õ�˦��Cģ�飬������˫��Э�̾��������ӿ�ֻ����͸��
@param[char*] pcContent:ҵ�����ݵ��������, pcContent��������'\0'��β��ASCII�����ַ���, ���ַ����м䲻�ܺ���'\0\.
					 ��pcContent�ĳ�����С��(2*1024)�� �����ܽ�������ȷ���õ�Cģ������,
@return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
@note ������˽ӿں󣬻�ͨ���첽�������ݵķ�ʽ��������͸�����ֻ���
-����pcContentΪ��ʱ��������CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionSendContent(const char *pcContent);

/**
@brief  ��˦��ģ��ע��һ���ص�
@param[in] fnOnSaitionEventCallBack:  Ҫ��ӵĻص�
@param[in] dwUserData: �����û�������Ϣ
@return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE	
*/
CSUDI_Error_Code CSSaitionAddCallback(CSOnSaitionEventCallBack_F fnOnSaitionEventCallBack, DWORD dwUserData);

/**
@brief  ��˦��ģ���Ƶ�һ��callback
@param[in] fnOnSaitionEventCallBack:  Ҫɾ���Ļص�
@param[in] dwUserData: �����û�������Ϣ
@return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionRemoveCallback(CSOnSaitionEventCallBack_F fnOnSaitionEventCallBack, DWORD dwUserData);

#ifdef __cplusplus
}
#endif
/** @} */
#endif  



