/**@defgroup PublicShellIrdeto2CA PublicShellIrdeto2CA ����Irdeto2.26 CA����ӿ�

@version 2.0.0 2010/4/12 ����
@{
*/
#ifndef CSSHELL_IRDETO2CA_PRIVATE_H
#define CSSHELL_IRDETO2CA_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "csshell_irdeto2ca.h"
		
/**
@brief ��ȡirdeto2.26 CAʵ�ֽӿ�
@return �ɹ�����֮ǰ���õ�irdeto2.26ʵ�ֽӿ�
@see CSShellIrdeto2.26CaGetDefaultParam
*/
const CSShellIrdeto2CaInitParam_S* CSShellIrdeto2CaGetParam(void);

/**
@brief ��ȡIrdeto2.26 CA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellIrdeto2CaSetConfig()����������Ϣʱ������NULL
@see CSShellIrdeto2CaSetConfig
*/
const CSShellIrdeto2CaConfig_S* CSShellIrdeto2CaGetConfig(void);

/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellIrdeto2CaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


