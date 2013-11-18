/**@defgroup PublicShellDVTCA PublicShellDVTCA ����������ѶCA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_DVTCA_PRIVATE_H
#define CSSHELL_DVTCA_PRIVATE_H

#include "csshell_dvtca.h"
#include "thirdparty/ca/shumashixun/CS_SHIXUNCA_Interface.h"


#ifdef __cplusplus
extern "C"
{
#endif
/**
@brief ��ȡDvt CAʵ�ֽӿ�
@param ��
@return �ɹ�����֮ǰ���õ�dvtcaʵ�ֽӿ�
@see CSShellDvtCaGetDefaultParam
*/
const CSShellDvtCaInitParam_S* CSShellDvtCaGetParam(void);

/**
@brief ��ȡDvt CA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellDvtCaSetConfig()����������Ϣʱ������NULL
@see CSShellDvtCaSetConfig
*/
const CSShellDvtCaConfig_S* CSShellDvtCaGetConfig(void);

/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellDvtCaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif

#endif

