
#ifndef CSSHELL_DVTCADATA_H
#define CSSHELL_DVTCADATA_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@brief DVT CA��ʼ������ */
typedef struct _CSShellDvtCaInitParam
{
	int m_nSize;
	int m_nFrequency;
	DWORD m_dwCaAddr;
	DWORD m_dwCaAddrBak;
}CSShellDvtCaInitData_S;


/**
@brief ��ȡDVT CA��ʼ������
@param[in] psDvtCaParam DVT CA��ʼ���ṹ��ָ��
@param[in] pcFilePath �����ļ�·��
@param[in] nFilePathLen �����ļ�·������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���pcFilePathΪNULL��nFilePathLenΪ0����Ĭ�������ļ�·��ΪROOT_PATH"config/dvtcadata.properties"
@notes ���ô˽ӿ�ʱpsDvtCaParam������m_nSize�踳ֵΪsizeof(CSShellDvtCaInitData_S)��������ܻ�ȡ��������
*/
BOOL CSShellGetDvtCaData(CSShellDvtCaInitData_S* psDvtCaParam, const char* pcFilePath, int nFilePathLen);

/**
@brief ��ȡDVT CAĬ�ϳ�ʼ����������ȡ������ʱ����
@param[in] psDvtCaParam DVT CA��ʼ���ṹ��ָ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���ô˽ӿ�ʱpsDvtCaParam������m_nSize�踳ֵΪsizeof(CSShellDvtCaInitParam_S)��������ܻ�ȡ��������
*/
BOOL CSShellGetDefaultDvtCaData(CSShellDvtCaInitData_S* psDvtCaParam);

#ifdef __cplusplus
}
#endif

#endif  
