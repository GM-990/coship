#ifndef CSSHELL_conaxCA_PRIVATE_H
#define CSSHELL_conaxCA_PRIVATE_H

#include "csshell_conaxca.h"
#include "thirdparty/ca/conax/CS_CONAX_Interface.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
/**
@brief ��ȡConax CAʵ�ֽӿ�
@return �ɹ�����֮ǰ���õ�Conaxcaʵ�ֽӿ�
@see CSShellConaxCaGetDefaultParam
*/
const CSShellConaxCaInitParam_S* CSShellConaxCaGetParam(void);


/**
@brief ��ȡConaxCA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellConaxCaSetConfig()����������Ϣʱ������NULL
@see CSShellConaxCaSetConfig
*/
const CSShellConaxCaConfig_S* CSShellConaxCaGetConfig(void);


/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellConaxCaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);
#ifdef __cplusplus
}
#endif
/** @} */
#endif


