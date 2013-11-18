/**@defgroup PublicShellSAU PublicShellSAU ����ShellSAU��������ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_SAU_H
#define CSSHELL_SAU_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "mmcp_os.h"
#include "CSUSPDMCommonType.h"
#include "SectionInterface2C.h"
#include "SICommonType.h"
/**
@brief ��ts���е�ԭʼ��Ŀ����ת��Ϊdm�еĽ�Ŀ����
@param [in] index ts��sdt����������ԭʼ��Ŀ����
@return ��Ӧ��dm�еĽ�Ŀ���͡�����Ŀ����ԭʼ��Ŀ��������ȡ
*/
typedef CSUSPDMServiceType (*GetServiceType_F)(BYTE index);

/**
@brief �жϽ�Ŀ�Ƿ���Ҫ������
@param [in] hSrv ��Ŀ���
@return ��Ҫ���˷���TRUE�����򷵻�FALSE
*/
typedef BOOL (*IsNeedFilterService_F)(HCSHANDLE hSrv);

/**
@brief �Ƚ�������Ŀ,�Ծ�����Ŀ����λ��
@param [in] hSrv1 ��Ŀ1�ľ��
@param [in] hSrv2 ��Ŀ2�ľ��
@return �����Ŀ1���ڽ�Ŀ2֮ǰ���򷵻�-1�������Ŀ1���ڽ�Ŀ2֮���򷵻�1����Ŀ��ͬ�򷵻�0
*/
typedef int  (*CompareService_F)(HCSHANDLE hSrv1,HCSHANDLE hSrv2);

/**
@brief ��dm�����н�Ŀ���й���
@return ��
@note �������Ҫ���˽�Ŀ���ýӿڿ��Բ�ʵ�ֻ���ֱ�ӷ���
*/
typedef void (*FilterAllService_F)();

/**
@brief ������Ŀ���±�־
@param [in] pbData ���±�־���ڵ�section����
@param [in] nLength  pbData�ĳ���
@return �õ����±�־����TRUE,���򷵻�FALSE
*/
typedef BOOL  (*ParseUpdateFlag_F)(PBYTE pbData, DWORD nLength);

/**
@brief ������±�־��Ŀǰ���±�־ͳһ�����ڻ��������йؼ���ΪUpdateFlag

*/
typedef void (*SaveUpdateFlag_F)();

/**breif ��nit��sdt��bat��˽�������������Ļص�*/
typedef void (*ParseDescriptor_F)(HCSHANDLE hTP, PBYTE pDescriptorData, WORD dwDesLen);

/**
@brief sau�ص�ע��ṹ��sau�ṩ�������ָ��ȻӦ��ʵ�֣�������Ŀ��ͬ��Щʵ�ֶ���ͬ��Ӧ�����Ⱥ���ϵͳ��ʼ��ʱ����Щʵ�ֵĺ���ע���sau��
*/
typedef struct _CSShellSAUCallback
{
	int m_nSize;		//�ص�ע��ṹָ���С���������sizeof(CSShellSAUCallback)

	GetServiceType_F m_fnGetServiceType;
	IsNeedFilterService_F m_fnIsNeedFilter;
	CompareService_F m_fnCompareService;
	FilterAllService_F m_fnFilterAllService;
	ParseUpdateFlag_F m_fnParseupdateFlag;
	ParseDescriptor_F  m_fnParseNitDescriptor;
	SaveUpdateFlag_F m_fnSaveUpdateFlag;
}CSShellSAUCallback;

/**
@brief ע��sauģ���õ��ĺ���ָ���ʵ��
@param [in] sCallback �����ص�ע��ṹ
@return ��
*/
void CSShellSAURegisterCallBack(CSShellSAUCallback sCallback);

/**
@brief ����sauʱ�������ݴ���״̬��
@param [in] bState ������ڴ����������ΪTRUE������ΪFALSE
*/
void CSShellSAUSetProcessState(BOOL bState);

/**
@brief ��ȡsau�������ݴ���״̬
@param ��
@return ���ڴ����������ΪTRUE������ΪFALSE
*/
BOOL CSShellSAUGetProcessState();

#ifdef __cplusplus
}
#endif
/** @} */
#endif   



