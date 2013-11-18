#ifndef CSSHELL_SUBTITLE_PRIVATE_H
#define CSSHELL_SUBTITLE_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif
#include "csshell_subtitle.h"

/**
@brief ��ʼ��subtitle
@param[in] subtitle callback
@return �ɹ�����Subtitle���,ʧ�ܷ���false
*/
HCSHANDLE CSShellSubtitleInit(SubtitleCallBack callback);

/**
@brief ��ʼ��subtitle,������ö��
@param[in] 
	subtitle callback
@param[in] 
	subtitle ����
@return �ɹ�����Subtitle���,ʧ�ܷ���false
*/
HCSHANDLE CSShellSubtitleInitEx(SubtitleCallBack callback, CSUSPSBTLType_E emType);

/**
@brief  ��ȡsubtitle��ʼ������
@return �ɹ�����psSubtitle ��ʼ�������ṹ��ʵ��
@note 
*/
CSShellSubtitleInitParam_S* CSShellSubtitleGetParam();

#ifdef __cplusplus
}
#endif

#endif  /* CSSHELL_SUBTITLE_PRIVATE_H */

