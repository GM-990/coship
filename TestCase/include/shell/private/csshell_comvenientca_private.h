#ifndef CSSHELL_ComvenientCA_PRIVATE_H
#define CSSHELL_ComvenientCA_PRIVATE_H

#include "thirdparty/ca/comvenient/CS_COMVENIENT_Interface.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
/**
@brief ��ȡComvenient CAʵ�ֽӿ�
@return �ɹ�����֮ǰ���õ�Comvenientcaʵ�ֽӿ�
@see CSShellComvenientCaGetDefaultParam
*/
const CSShellComvenientCaInitParam_S* CSShellComvenientCaGetParam(void);

/**
@brief ��ȡComvenientCA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellComvenientCaSetConfig()����������Ϣʱ������NULL
@see CSShellComvenientCaSetConfig
*/
const CSShellComvenientCaConfig_S* CSShellComvenientCaGetConfig(void);


/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellComvenientCaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


