
#ifndef CSSHELL_SEARCHDATA_H
#define CSSHELL_SEARCHDATA_H

#include "mmcp_typedef.h"
#include "csshell_search.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ��ȡSearch��ʼ������
@param[in] psSearchParam Search��ʼ���ṹ��ָ��
@param[in] pcFilePath �����ļ�·��
@param[in] nFilePathLen �����ļ�·������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���pcFilePathΪNULL��nFilePathLenΪ0����Ĭ�������ļ�·��ΪROOT_PATH"config/searchdata.properties"
@notes ���ô˽ӿ�ʱpsSearchParam������m_nSize�踳ֵΪsizeof(CSShellCommonSearch_S)��������ܻ�ȡ��������
*/
BOOL CSShellGetSearchData(CSShellCommonSearch_S* psSearchParam, const char* pcFilePath, int nFilePathLen);

/**
@brief ��ȡSearchĬ�ϳ�ʼ����������ȡ������ʱ����
@param[in] psSearchParam Search��ʼ���ṹ��ָ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���ô˽ӿ�ʱpsSearchParam������m_nSize�踳ֵΪsizeof(CSShellCommonSearch_S)��������ܻ�ȡ��������
*/
BOOL CSShellGetDefaultSearchData(CSShellCommonSearch_S* psSearchParam);

#ifdef __cplusplus
}
#endif

#endif  
