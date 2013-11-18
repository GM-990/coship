
#ifndef CSSHELL_SAUDATA_H
#define CSSHELL_SAUDATA_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@brief SAU��ʼ������ */
typedef struct _CSShellSauInitParam
{
	int m_nSize;
	BOOL m_bStatus;
	int m_nFrequency;
	int m_nSymbolRate;			
	int m_nModulation;
}CSShellSauInitParam_S;


/**
@brief ��ȡSAU��ʼ������
@param[in] psSauParam SAU��ʼ���ṹ��ָ��
@param[in] pcFilePath �����ļ�·��
@param[in] nFilePathLen �����ļ�·������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���pcFilePathΪNULL��nFilePathLenΪ0����Ĭ�������ļ�·��ΪROOT_PATH"config/saudata.properties"
@notes ���ô˽ӿ�ʱpsSauParam������m_nSize�踳ֵΪsizeof(CSShellSauInitParam_S)��������ܻ�ȡ��������
*/
BOOL CSShellGetSauCaData(CSShellSauInitParam_S* psSauParam, const char* pcFilePath, int nFilePathLen);

/**
@brief ��ȡSAUĬ�ϳ�ʼ����������ȡ������ʱ����
@param[in] psSauParam SAU��ʼ���ṹ��ָ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���ô˽ӿ�ʱpsSauParam������m_nSize�踳ֵΪsizeof(CSShellSauInitParam_S)��������ܻ�ȡ��������
*/
BOOL CSShellGetDefaultSauData(CSShellSauInitParam_S* psSauParam);

#ifdef __cplusplus
}
#endif

#endif  
