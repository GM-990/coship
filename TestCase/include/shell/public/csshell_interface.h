/**@defgroup PublicShell PublicShell ����Shell��������ӿڼ�����

@version 2.0.0 2009/12/16 ����
@{
*/

#include "mmcp_typedef.h"

#ifndef CSSHELL_INTERFACE_H
#define CSSHELL_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

#define MODULE_SHELL         "shell"

typedef enum _CSShellTunerDvbType
{
	EM_CSSHELL_DVB_TUNER_UNKOWN = 0,
	EM_CSSHELL_DVB_S,
	EM_CSSHELL_DVB_C,
	EM_CSSHELL_DVB_T
}CSShellTunerDvbType_E;

/**
@brief MMCP��ʼ��
����ָ�������û���������MMCP�ĳ�ʼ���������ֿ⡢CA�������������ûص�������ģ������ò���������ɺ�ִ��
@return �ɹ�����0��ʧ��<0
@todo �ݲ�ʵ�֣�Ŀǰ���Ƿ���-1
*/
int CSShellInitMMCP();

/**
@brief �������ļ��л�ȡTuner���ͣ�
@param[in] nTunerIndex: Tuner���������ڶ�Tuner�������ͨ��index���֣���Tuner���������0����
@return Tuner����ö�٣�����ȡʧ�ܣ�����DVB_TUNER_UNKOWN
*/
CSShellTunerDvbType_E CSShellGetTunerDvbType(int nTunerIndex);

/**
@brief �������ļ��л�ȡTuner����
@param[out] pnTunerCount: �������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellGetTunerCount(int *pnTunerCount);


#ifdef __cplusplus
}
#endif
/** @} */
#endif   

