/**@defgroup PublicShellCA PublicShellCA ����ShellCA��������ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_DM_H
#define CSSHELL_DM_H

#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ��ȡusp-dm��handle
@param ��
@return usp-dm handle
@note shellģ��δ�ṩusp-dm��usp-section�ĵĳ�ʼ���ӿڡ�������ģ��ĳ�ʼ����ҪӦ�����е���usp-dm,usp-section�Ľӿ�������
���ǵ�usp�ṩ�Ļ�ȡ�Գ�ʼ������dm handle�ķ��������ã����ṩ�˽ӿڡ���ȡsection��handle�����Section2CGetSectionSourceHandle��
*/
HCSHANDLE CSShellDMGetContainerHandle();

/**
@brief �����Ŀ������
@param [in] hService ��Ŀ���
@param [in] ucVolume ������С
@note ͳһ�������沢��������ÿ����Ŀ�����󡣶���ÿ����Ŀ����Ҫ������������Ŀ�ṩ�˽ӿڡ�
��Ŀ����Ŀǰ������dm��userData���е�һ��λ�á��������Ǽ���shell 1.0�汾��
��mmmcp hmc�����У�Ӧ�ö�dm�û���������ʹ���������Ƶģ�ԭ����ЩuserData������������̶�������.
���������Ľ��飬����û���Ҫ�����Լ������ݣ�������ļ���ʽ�����档
*/
void CSShellDMSaveVolInfo(int hService, BYTE ucVolume);

/**
@brief ��ȡ��Ŀ������
@param [in] hService ��Ŀ���
@return ��Ŀ����
@note �ýӿڲ�������ͳһ��������ʱ����ȡ��Ŀ�����������
*/
BYTE CSShellDMGetVolInfo(int hService) ;

/**
@brief ��ȡ��Ŀ������
@param[in] hService ��Ŀ���
@param[out] pemCodingType ��Ŀ���Ƶı�������
@param[out] pcServiceName ��Ŀ���ƵĴ洢�׵�ַ,�������
@param[in] nLength �������pcServiceName�ĳ���
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ����Ϊ��Ŀ����̫������DM�洢������ʱ����Ӧ���Լ��洢����ʱ�м��
���ô˷�������ȡ��Ŀʱ��Ӧ�ÿ��Լ�ʵ�ִ˷��������Լ���userdata�л�ȡ������
��Ŀ�����������м��
*/
typedef BOOL (*CSShellDMGetServiceName_F)(HCSHANDLE hService, CSUSPDMCodingType *pemCodingType, char *pcServiceName, int nLength);


/**
���ṹ�����˻�ȡDM�е���Ϣʱ�õ�����Ϣ��
�����ɻ�ȡCSShellDMFunc_S
*/
typedef struct _CSShellDMFunc_S
{
    int                                 m_nSize;            ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CSShellDMFunc_S)
    CSShellDMGetServiceName_F           m_fnGetServiceName; ///< ��ȡ��Ŀ���Ƶĺ���	
}CSShellDMFunc_S;

/**
@brief ����DM��ز����ӿڸ�MMCP
@param[in] psDMFunc  DM��Ϣ����ز����ṹ��ʵ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@code
//�����������°��˷�ʽ���ô˽ӿڼ��ɡ�
CSShellDMSetFunc(CSShellDMGetDefaulFunc());
@endcode
*/
BOOL CSShellDMSetFunc(const CSShellDMFunc_S* psDMFunc);

	
/**
@brief ��ȡDMĬ��ʵ�ֲ���
@return MMCP�ṩDM Shell�ӿڵ�Ĭ��ʵ��
@note ���ӿ�ʵ����ʵ���˵�����Ŀ��������Ϣ����Ŀ������Ҫ�޸������ֱ�ӽ��˵���ʵ�����ø�MMCP Shellģ��
@note MMCP�ڲ�(dtvm native��jsext)����ֱ��ʹ�ñ��ӿڣ���MMCP��Ҫ���DM�Ķ�����ʹ��csshell_dm_private.h�е�CSShellDMGetFunc�ӿ�
@see CSShellDMGetFunc
*/
const CSShellDMFunc_S* CSShellDMGetDefaulFunc(void);

#ifdef __cplusplus
}
#endif
/** @} */
#endif   



