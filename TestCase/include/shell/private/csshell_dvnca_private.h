/**@defgroup PublicShellDVNCA PublicShellDVNCA �������CA����ӿ�

@version 2.0.0 2011/10/28 ����
@{
*/
#ifndef CSSHELL_DVNCA_PRIVATE_H
#define CSSHELL_DVNCA_PRIVATE_H

#include "csshell_dvnca.h"
#include "thirdparty/ca/dvn/CS_DVNCA_Interface.h"


#ifdef __cplusplus
extern "C"
{
#endif
/**
@brief ��ȡDvn CAʵ�ֽӿ�
@param ��
@return �ɹ�����֮ǰ���õ�dvtcaʵ�ֽӿ�
@see CSShellDvnCaGetDefaultParam
*/
const CSShellDvnCaInitParam_S* CSShellDvnCaGetParam(void);

/**
@brief ��ȡDvn CA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellDvnCaSetConfig()����������Ϣʱ������NULL
@see CSShellDvnCaSetConfig
*/
const CSShellDvnCaConfig_S* CSShellDvnCaGetConfig(void);

/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellDvnCaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif

#endif

