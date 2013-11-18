
#ifndef CSSHELL_SUBTITLEDATA_H
#define CSSHELL_SUBTITLEDATA_H

#include "mmcp_typedef.h"
#include "csshell_subtitle.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ��ȡsubtitle��ʼ������
@param[in] psSubParam subtitle��ʼ���ṹ��ָ��
@param[in] pcFilePath �����ļ�·��
@param[in] nFilePathLen �����ļ�·������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���pcFilePathΪNULL��nFilePathLenΪ0����Ĭ�������ļ�·��ΪROOT_PATH"config/subtidata.properties"
@notes ���ô˽ӿ�ʱpsSubParam������m_nSize�踳ֵΪsizeof(CSShellSubtitleInitParam_S)��������ܻ�ȡ��������
*/
BOOL CSShellGetSubtitleData(CSShellSubtitleInitParam_S* psSubParam, const char* pcFilePath, int nFilePathLen);

/**
@brief ��ȡsubtitleĬ�ϳ�ʼ����������ȡ������ʱ����
@param[in] psSubParam subtitle��ʼ���ṹ��ָ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���ô˽ӿ�ʱpsSubParam������m_nSize�踳ֵΪsizeof(CSShellSubtitleInitParam_S)��������ܻ�ȡ��������
*/
BOOL CSShellGetDefaultSubtitleData(CSShellSubtitleInitParam_S* psSubParam);

#ifdef __cplusplus
}
#endif

#endif  
