
#ifndef CSSHELL_EPGDATA_H
#define CSSHELL_EPGDATA_H

#include "mmcp_typedef.h"
#include "csshell_epg.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ��ȡEPG��ʼ������
@param[in] psPFaram PF EPG��ʼ���ṹ��ָ��
@param[in] psScheduleParam Schedule EPG��ʼ���ṹ��ָ��
@param[in] pcFilePath �����ļ�·��
@param[in] nFilePathLen �����ļ�·������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���pcFilePathΪNULL��nFilePathLenΪ0����Ĭ�������ļ�·��ΪROOT_PATH"config/epgdata.properties"
@notes ���ô˽ӿ�ʱCSShellEpgInitParam_S�ṹ�������m_nSizeҪ��ֵΪsizeof(CSShellEpgInitParam_S)��������ܵ���ʧ��
*/
BOOL CSShellGetEpgData(CSShellEpgInitParam_S* psPFParam, CSShellEpgInitParam_S* psScheduleParam, const char* pcFilePath, int nFilePathLen);

/**
@brief ��ȡEPGĬ�ϳ�ʼ����������ȡ������ʱ����
@param[in] psPFaram PF EPG��ʼ���ṹ��ָ��
@param[in] psScheduleParam Schedule EPG��ʼ���ṹ��ָ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@notes ���ô˽ӿ�ʱCSShellEpgInitParam_S�ṹ�������m_nSizeҪ��ֵΪsizeof(CSShellEpgInitParam_S)��������ܵ���ʧ��
*/
BOOL CSShellGetDefaultEpgData(CSShellEpgInitParam_S* psPFaram, CSShellEpgInitParam_S* psScheduleParam);

#ifdef __cplusplus
}
#endif

#endif  
