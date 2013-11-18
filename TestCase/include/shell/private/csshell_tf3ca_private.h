/**@defgroup PublicShellTF3CA PublicShellTF3CA ����ͬ��3 CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_TF3CA_PRIVATE_H
#define CSSHELL_TF3CA_PRIVATE_H

#include "csshell_tf3ca.h"
#include "thirdparty/ca/tongfang3/CS_TFCA_Interface.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
	/**
	@brief ��ȡtf3 CAʵ�ֽӿ�
	@return �ɹ�����֮ǰ���õ�tf3caʵ�ֽӿ�
	@see CSShellTf3CaGetDefaultParam
	*/
	const CSShellTf3CaInitParam_S* CSShellTf3CaGetParam(void);


/**
@brief ��ȡͬ��3CA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellTf3CaSetConfig()����������Ϣʱ������NULL
@see CSShellTf3CaSetConfig
*/
const CSShellTf3CaConfig_S* CSShellTf3CaGetConfig(void);


/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellTf3CaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


