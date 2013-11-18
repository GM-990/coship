/**@defgroup PublicShellSTCA PublicShellSTCA ������ͨ CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_STCA_PRIVATE_H
#define CSSHELL_STCA_PRIVATE_H

#include "csshell_stca.h"
#include "thirdparty/ca/suantong/CS_SUANTONGCA_Interface.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	@brief ��ȡSt CAʵ�ֲ���
	@return �ɹ�����֮ǰ���õ�stcaʵ�ֲ���
	@see CSShellStCaGetDefaultParam
	*/
	const CSShellStCaInitParam_S* CSShellStCaGetParam(void);

/**
@brief ��ȡ��ͨCA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellStCaSetConfig()����������Ϣʱ������NULL
@see CSShellStCaConfig_S
*/
const CSShellStCaConfig_S* CSShellStCaGetConfig(void);

/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellStCaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif

