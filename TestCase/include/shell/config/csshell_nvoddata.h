
#ifndef CSSHELL_NVODDATA_H
#define CSSHELL_NVODDATA_H

#include "mmcp_typedef.h"
#include "csshell_nvod.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ��ȡNVOD��ʼ������
@param[in] psNvodParam NVOD��ʼ���ṹ��ָ��
@param[in] pcFilePath �����ļ�·��
@param[in] nFilePathLen �����ļ�·������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���pcFilePathΪNULL��nFilePathLenΪ0����Ĭ�������ļ�·��ΪROOT_PATH"config/nvoddata.properties"
*/
BOOL CSShellGetNvodData(CSShellNvodInitParam_S* psNvodParam, const char* pcFilePath, int nFilePathLen);

/**
@brief ��ȡNVODĬ�ϳ�ʼ����������ȡ������ʱ����
@param[in] psNvodParam NVOD��ʼ���ṹ��ָ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellGetDefaultNvodData(CSShellNvodInitParam_S* psNvodParam);

#ifdef __cplusplus
}
#endif

#endif  
