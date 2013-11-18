#ifndef CSSHELL_finesumCA_PRIVATE_H
#define CSSHELL_finesumCA_PRIVATE_H

#include "csshell_finesumca.h"
#include "thirdparty/ca/finesum/csfinesumcainterface.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
/**
@brief ��ȡFinesum CAʵ�ֽӿ�
@return �ɹ�����֮ǰ���õ�Finesumcaʵ�ֽӿ�
@see CSShellFinesumCaGetDefaultParam
*/
const CSShellFinesumCaInitParam_S* CSShellFinesumCaGetParam(void);


/**
@brief ��ȡFinesumCA�����������Ϣ
@return �ɹ�����������Ϣ��ʧ�ܷ���NULL
@note ��Ӧ��û�е���CSShellFinesumCaSetConfig()����������Ϣʱ������NULL
@see CSShellFinesumCaSetConfig
*/
const CSShellFinesumCaConfig_S* CSShellFinesumCaGetConfig(void);


/******************************************************************************
@breif ����ca�����ͺ���Ϣ���������ܿ�״̬
@param dwType--dwParam3,��Ӧcs_cainterface_prvext.h�� F_CBMsg�еĲ���
@return �ɹ�����TRUE,���򷵻�FALSE
@note �ڽ��յ�ca�����ģ����ܿ�������������ܿ������ܽ�Ŀ����Ŀδ��Ȩ��Ҫֹͣ����Ƶ�������Ϣʱ����Ҫ����ԭ����߸�mediaplayer�����mediaplayer��֪��
��Ŀͣ����ԭ�򣬾ͻ����ͽ�Ŀͣ������Ϣ
*******************************************************************************/
BOOL CSShellFinesumCaSetSmartCardState(DWORD dwType, DWORD dwParm1, DWORD dwParm2);
#ifdef __cplusplus
}
#endif
/** @} */
#endif


