/**@defgroup PublicShellIrdeto3CA PublicShellIrdeto3CA ����Irdeto3 CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_IRDETO3CA_PRIVATE_H
#define CSSHELL_IRDETO3CA_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "csshell_irdeto3ca.h"
		
/**
@brief ��ȡirdeto3 CAʵ�ֽӿ�
@return �ɹ�����֮ǰ���õ�irdeto3ʵ�ֽӿ�
@see CSShellIrdeto3CaGetDefaultParam
*/
const CSShellIrdeto3CaInitParam_S* CSShellIrdeto3CaGetParam(void);

/**
@brief ��ȡIrdeto3 CA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellIrdeto3CaSetConfig()����������Ϣʱ������NULL
@see CSShellIrdeto3CaSetConfig
*/
const CSShellIrdeto3CaConfig_S* CSShellIrdeto3CaGetConfig(void);

/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellIrdeto3CaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


