/**@defgroup PublicShellTF3CA PublicShellTF3CA ����Top CA����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_TOPCA_PRIVATE_H
#define CSSHELL_TOPCA_PRIVATE_H

#include "csshell_topca.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
	/**
	@brief ��ȡTop CAʵ�ֽӿ�
	@return �ɹ�����֮ǰ���õ�Top caʵ�ֽӿ�
	@see CSShellTf3CaGetDefaultParam
	*/
const CSShellTopCaInitParam_S* CSShellTopCaGetParam(void);


/**
@brief ��ȡTop ca�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellTopCaSetConfig()����������Ϣʱ������NULL
@see CSShellTopCaSetConfig
*/
const CSShellTopCaConfig_S* CSShellTopCaGetConfig(void);
#ifdef __cplusplus
}
#endif
/** @} */
#endif


