/**@defgroup PublicShellNagraCA PublicShellNagraCA ����Nagra CA����ӿ�

@version 2.0.0 2010/01/24 ����
@{
*/
#ifndef CSSHELL_NAGRACA_PRIVATE_H
#define CSSHELL_NAGRACA_PRIVATE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "csshell_nagraca.h"
		
/**
@brief ��ȡnagra CAʵ�ֽӿ�
@return �ɹ�����֮ǰ���õ�nagraʵ�ֽӿ�
@see CSShellNagraCaGetDefaultParam
*/
const CSShellNagraCaInitParam_S* CSShellNagraCaGetParam(void);

/**
@brief ��ȡnagra CA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellNagraCaSetConfig()����������Ϣʱ������NULL
@see CSShellNagraCaSetConfig
*/
const CSShellNagraCaConfig_S* CSShellNagraCaGetConfig(void);

/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellNagraCaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


