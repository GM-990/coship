#ifndef CSSHELL_TELETEXT_PRIVATE_H
#define CSSHELL_TELETEXT_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_teletext.h"

/**
@brief ��ʼ��teletext
@param[in] teletext callback
@return �ɹ�����teletext���,ʧ�ܷ���NULL
*/
HCSHANDLE CSShellTeletextInit(TELECallback callback);

/**
@brief  ��ȡteletext��ʼ������
@return �ɹ�����psTeletext ��ʼ�������ṹ��ʵ��
@note 
*/
CSShellTeletextInitParam_S* CSShellTeletextGetParam();

#ifdef __cplusplus
}
#endif

#endif  /* CSSHELL_TELETEXT_PRIVATE_H */

