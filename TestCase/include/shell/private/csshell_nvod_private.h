#ifndef CSSHELL_NVOD_PRIVATE_H
#define CSSHELL_NVOD_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "csshell_nvod.h"

/**
@brief ��ʼ��Nvod
@param[in] �ص�����,��dtvmx native��ʵ��
@return �ɹ��򷵻س�ʼ������Nvod�����ʧ���򷵻�NULL
@note ����Ѿ���ʼ����epg����ֱ�ӷ����ϴγ�ʼ������epg���
*/
HCSHANDLE CSShellNvodInit(NVODCallBack callback );

/**
@brief ��ȡ��ʼ��Nvod����
@param ��
@return ֮ǰ���õ�nvod����
*/
const CSShellNvodInitParam_S* CSShellNvodGetParam();

/**
@brief ��ȡNVODʵ�ֽӿ�
@return �ɹ�����֮ǰ���õ�NVODʵ�ֽӿ�
@see CSShellNvodGetDefaulFunc
*/
const CSShellNvodFunc_S* CSShellNvodGetFunc(void);

/**
@brief ����Nvod
@param ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellNvodDestroy();


#ifdef __cplusplus
}
#endif

#endif  /* SHELL_SUBTITLE_H */

