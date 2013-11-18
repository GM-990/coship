/**@defgroup PublicShellSTCA PublicShellSTCA ������ͨ CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_STCTICA_PRIVATE_H
#define CSSHELL_STCTICA_PRIVATE_H

#include "csshell_stctica.h"
#include "thirdparty/ca/suantongcti/CS_SUANTONGCTICA_Interface.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ��ȡSt CAʵ�ֲ���
@return �ɹ�����֮ǰ���õ�stcaʵ�ֲ���
@see CSShellStCTICaGetDefaultParam
*/
const CSShellStCTICaInitParam_S* CSShellStCTICaGetParam(void);

/**
@brief ��ȡ��ͨCA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellStCTICaSetConfig()����������Ϣʱ������NULL
@see CSShellStCTICaConfig_S
*/
const CSShellStCTICaConfig_S* CSShellStCTICaGetConfig(void);

/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellStCTICaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif

