#if !defined(_CSTTSIFLYTEK_H)
#define _CSTTSIFLYTEK_H

#include "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**
@brief ���ÿƴ�Ѷ����������������Դ����Ĭ��·��
@param[in] pcDefResPath ��������������Դ·��
@return �ɹ�����TRUE; ʧ�ܷ���FALSE
*/
BOOL CSTTSSetDefaultResPath(const char* pcDefResPath, DWORD dwResPathLen);

/**
@brief ����һ���ƴ�Ѷ������������Դ���ĵ�·��
@param[in] pcResPath ��������������Դ·��
@param[in] dwResPathLen pcResPath�ĳ���
@return �ɹ�����TRUE; ʧ�ܷ���FALSE
@remark	1)����һ���Ѵ��ڵ���Դ��·����ֱ�ӷ��سɹ���
		2)��ͨ��CSTTSDeleteResPath()�ӿ���ɾ�����õ���Դ��·����
		3)�ýӿڲ�֧�ֶ��̲߳�����
*/
BOOL CSTTSAddResPath(const char * pcResPath, DWORD dwResPathLen);

/**
@brief ɾ���ƴ�Ѷ������������Դ���ĵ�·��
@param[in] pcResPath ��������������Դ·��
@param[in] dwResPathLen pcResPath�ĳ���
@return �ɹ�����TRUE; ʧ�ܷ���FALSE
@remark	1)���ӿ�ֻɾ����CSTTSAddResPath()�ӿ����õ���Դ��·����
		2)������pcResPath����NULLʱ��ɾ����CSTTSAddResPath���õ�ȫ����Դ·����
		3)ɾ��һ�������ڵ���Դ·��ʱ������ʧ�ܣ�
		4)�ýӿڲ�֧�ֶ��̲߳�����
*/
BOOL CSTTSDeleteResPath(const char * pcResPath, DWORD dwResPathLen);

/**
@brief �����ƴ�Ѷ������������Դ���
@param[in] pResPath ��������������Դ·��
@return �ɹ�������Դ���;ʧ�ܷ���NULL
@remark 1)��pResPath��ΪNULL����ʵ��ʹ��pResPath + CSTTSAddResPath���õ�·����
		2)��pResPathΪNULL����ʵ��ʹ��CSTTSSetDefaultResPath���õ�·�� + CSTTSAddResPath���õ�·����
		3)���������CSTTSSetDefaultResPath����Ĭ��·����Ҳ������CSTTSAddResPath���·������pcResPath���ܸ�ΪNULL��
*/
HCSHANDLE CSTTSCreateIFlyTek(const char* pcResPath);

#ifdef __cplusplus
}
#endif

#endif

