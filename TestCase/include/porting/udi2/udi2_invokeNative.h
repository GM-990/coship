/**@defgroup invokeNative invokeNative ����Java���ص��õ�ʵ�ֽӿ�

@brief invokeNativeģ�鶨����ñ��ط����Ľӿڣ��൱��������ʱ(���Ǳ���ʱ)����һ������������,������֮��
��ģ��һ���û�����Ի�����Ƕ���ʵ�֣���ʵ��һ��ֻ����оƬ��ϵ��أ���mips,arm�ȡ�

@version 2.0.1 2010/03/25 ��ʼ�汾
@{
*/

#ifndef _UDI2_INVOKENATIVE_H_
#define _UDI2_INVOKENATIVE_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief ��ģ��������ֵ*/
enum
{
	CSUDIINVOKE_ERROR_BAD_PARAMETER = CSUDI_INVOKE_ERROR_BASE,	///< ��������
	CSUDIINVOKE_ERROR_FEATURE_NOT_SUPPORTED,				    ///< ��֧�ִ˲���
	CSUDIINVOKE_ERROR_UNKNOWN_ERROR,						    ///< һ�����
	CSUDIINVOKE_ERROR_NO_MEMORY							        ///< ���ڴ���ô���
};

/**@brief ���������������������ֵ������*/
typedef enum
{
	EM_UDIINVOKE_VALUE_TYPEID_NONE		= 0,         ///< �޲������߽�ֵ����ʹ��
	EM_UDIINVOKE_VALUE_TYPEID_ENDFUNC	= 1,         ///< ����������־���������
	EM_UDIINVOKE_VALUE_TYPEID_VOID		= 2,         ///< void���Ʋ��������޲��������ڷ���ֵ
	EM_UDIINVOKE_VALUE_TYPEID_INT		= 3,	     ///< 32λ�з������ͣ���CSUDIInvokeValue_S.m_nLowȡ32λ��Ϊshort���Ͳ����򷵻�ֵ
	EM_UDIINVOKE_VALUE_TYPEID_SHORT		= 4,         ///< 16λ�з������ͣ���CSUDIInvokeValue_S.m_nLowȡ��16λ��Ϊshort���Ͳ����򷵻�ֵ
	EM_UDIINVOKE_VALUE_TYPEID_CHAR		= 5,         ///< �ַ��ͣ���CSUDIInvokeValue_S.m_nLowȡ��16λ��Ϊunsigned short���Ͳ����򷵻�ֵ
	EM_UDIINVOKE_VALUE_TYPEID_LONG		= 6,         ///< 64λ�з������ͣ���CSUDIInvokeValue_Sȡ64λ��ΪCSUDI_INT64���Ͳ����򷵻�ֵ
	EM_UDIINVOKE_VALUE_TYPEID_BYTE		= 7,         ///< 8λ�з������ͣ���CSUDIInvokeValue_S.m_nLowȡ��8λ��Ϊchar���Ͳ����򷵻�ֵ
	EM_UDIINVOKE_VALUE_TYPEID_FLOAT		= 8,         ///< �����ȸ����ͣ���CSUDIInvokeValue_S.m_nLowȡ32λ��Ϊfloat���Ͳ����򷵻�ֵ
	EM_UDIINVOKE_VALUE_TYPEID_DOUBLE	= 9,         ///< ˫���ȸ����ͣ���CSUDIInvokeValue_Sȡ64λ��Ϊdouble���Ͳ����򷵻�ֵ
	EM_UDIINVOKE_VALUE_TYPEID_BOOLEAN	= 10,        ///< �����ͣ���CSUDIInvokeValue_S.m_nLowȡ��8λ��Ϊchar���Ͳ����򷵻�ֵ
	EM_UDIINVOKE_VALUE_TYPEID_OBJ		= 11         ///< �����ͣ���CSUDIInvokeValue_S.m_nLowȡ32λ��Ϊvoid*���Ͳ������߷���(ע��:�ݲ�������64λ��)
}CSUDIInvokeValueType_E;

/**@brief ���������������������ֵ��ֵ�洢�ṹ
��ϸ��Ӧ��ϵ��CSUDIInvokeValueType_E����
*/
typedef struct _CSUDIInvokeValue_S
{
	unsigned int		m_nLow;		///< ���ڴ洢32λ���͵�����, ��64λ���ݵĵ�32λ
	unsigned int		m_nHigh;		///< ���ڴ洢64λ���͵����ݵĸ�32λ
} CSUDIInvokeValue_S;

/**@brief ��������������*/
typedef struct{
	CSUDIInvokeValue_S     m_sArg;	 ///< ����ֵ
	CSUDIInvokeValueType_E m_eType;	 ///< ��������
}CSUDIInvokeArg_S;

/**
@brief ʵ�ֺ������ù���.

���������� : ����һ��������ַ���ú����Ĳ����б����������������ͣ�ʵ�ָú����ĵ��ù��ܣ������غ���ִ�еķ���ֵ
@param[in] pfFunction �����õĺ���ָ�롣
@param[in] psArgs ��ű����ú��������в�������Ϣ�����CSUDIInvokeArg_S���������˳��{{arg1,type},{arg2,type}, {arg3,type}, ... ,{argn, type}, ...}��
@param[in] nArgsSize �����ú����Ĳ����ĸ�����
@param[in] eReturnType ���ú����ķ���ֵ�����ͣ����CSUDIInvokeValueType_E��
@param[out] psReturnValue ��ű����ú����ķ���ֵ�����CSUDIInvokeValue_S��
@return �ɹ�����CSUDI_SUCCESS, ���򷵻ش������
@note �洢����������ֵ�Ĺ����CSUDIInvokeValueType_E���������߱���һ�¡�
*/
CSUDI_Error_Code CSUDIInvokeNative(void * pfFunction, CSUDIInvokeArg_S * psArgs, int nArgsSize, 
	CSUDIInvokeValueType_E eReturnType, CSUDIInvokeValue_S* psReturnValue);

#ifdef __cplusplus
}
#endif
/** @} */
#endif  //_UDI2_INVOKENATIVE_H_

