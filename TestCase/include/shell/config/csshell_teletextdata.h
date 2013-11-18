
#ifndef CSSHELL_TELETEXTDATA_H
#define CSSHELL_TELETEXTDATA_H

#include "mmcp_typedef.h"
#include "csshell_teletext.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ��ȡteletext��ʼ������
@param[out] psSubParam teletext��ʼ���ṹ��ָ��
@param[in] pcFilePath �����ļ�·��
@param[in] nFilePathLen �����ļ�·������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���pcFilePathΪNULL��nFilePathLenΪ0����Ĭ�������ļ�·��ΪROOT_PATH"config/teletextdata.properties"
@notes ���ô˽ӿ�ʱpsSubParam������m_nSize�踳ֵΪsizeof(CSShellTeletextInitParam_S)��������ܻ�ȡ��������
@notes �˽ӿڽ��ɻ�ȡ�����ļ��������õĲ������������������ھ�����Ŀ�����и�ֵ
*/
BOOL CSShellGetTeletextData(CSShellTeletextInitParam_S* psSubParam, const char* pcFilePath, int nFilePathLen);

/**
@brief ��ȡteletextĬ�ϳ�ʼ����������ȡ������ʱ����
@param[out] psSubParam teletext��ʼ���ṹ��ָ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���ô˽ӿ�ʱpsSubParam������m_nSize�踳ֵΪsizeof(CSShellTeletextInitParam_S)��������ܻ�ȡ��������
@notes �˽ӿڽ��ɻ�ȡ�����ļ��������õĲ������������������ھ�����Ŀ�����и�ֵ
*/
BOOL CSShellGetDefaultTeletextData(CSShellTeletextInitParam_S* psSubParam);

#ifdef __cplusplus
}
#endif

#endif  
