/**@defgroup invokeNative invokeNative 定义Java本地调用的实现接口

@brief invokeNative模块定义调用本地方法的接口，相当于在运行时(而非编译时)决定一个函数的声明,并调用之。
本模块一般用汇编语言或者内嵌汇编实现，其实现一般只与主芯片体系相关，如mips,arm等。

@version 2.0.1 2010/03/25 初始版本
@{
*/

#ifndef _UDI2_INVOKENATIVE_H_
#define _UDI2_INVOKENATIVE_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief 本模块错误代码值*/
enum
{
	CSUDIINVOKE_ERROR_BAD_PARAMETER = CSUDI_INVOKE_ERROR_BASE,	///< 参数错误
	CSUDIINVOKE_ERROR_FEATURE_NOT_SUPPORTED,				    ///< 不支持此操作
	CSUDIINVOKE_ERROR_UNKNOWN_ERROR,						    ///< 一般错误
	CSUDIINVOKE_ERROR_NO_MEMORY							        ///< 无内存可用错误
};

/**@brief 定义输入输出参数及返回值的类型*/
typedef enum
{
	EM_UDIINVOKE_VALUE_TYPEID_NONE		= 0,         ///< 无参数，边界值不会使用
	EM_UDIINVOKE_VALUE_TYPEID_ENDFUNC	= 1,         ///< 参数结束标志，不会出现
	EM_UDIINVOKE_VALUE_TYPEID_VOID		= 2,         ///< void类似参数，即无参数，用于返回值
	EM_UDIINVOKE_VALUE_TYPEID_INT		= 3,	     ///< 32位有符号整型，从CSUDIInvokeValue_S.m_nLow取32位作为short类型参数或返回值
	EM_UDIINVOKE_VALUE_TYPEID_SHORT		= 4,         ///< 16位有符号整型，从CSUDIInvokeValue_S.m_nLow取低16位作为short类型参数或返回值
	EM_UDIINVOKE_VALUE_TYPEID_CHAR		= 5,         ///< 字符型，从CSUDIInvokeValue_S.m_nLow取低16位作为unsigned short类型参数或返回值
	EM_UDIINVOKE_VALUE_TYPEID_LONG		= 6,         ///< 64位有符号整型，从CSUDIInvokeValue_S取64位作为CSUDI_INT64类型参数或返回值
	EM_UDIINVOKE_VALUE_TYPEID_BYTE		= 7,         ///< 8位有符号整型，从CSUDIInvokeValue_S.m_nLow取低8位作为char类型参数或返回值
	EM_UDIINVOKE_VALUE_TYPEID_FLOAT		= 8,         ///< 单精度浮点型，从CSUDIInvokeValue_S.m_nLow取32位作为float类型参数或返回值
	EM_UDIINVOKE_VALUE_TYPEID_DOUBLE	= 9,         ///< 双精度浮点型，从CSUDIInvokeValue_S取64位作为double类型参数或返回值
	EM_UDIINVOKE_VALUE_TYPEID_BOOLEAN	= 10,        ///< 布尔型，从CSUDIInvokeValue_S.m_nLow取低8位作为char类型参数或返回值
	EM_UDIINVOKE_VALUE_TYPEID_OBJ		= 11         ///< 对象型，从CSUDIInvokeValue_S.m_nLow取32位作为void*类型参数或者返回(注意:暂不适用于64位机)
}CSUDIInvokeValueType_E;

/**@brief 定义输入输出参数及返回值的值存储结构
详细对应关系见CSUDIInvokeValueType_E声明
*/
typedef struct _CSUDIInvokeValue_S
{
	unsigned int		m_nLow;		///< 用于存储32位类型的数据, 或64位数据的低32位
	unsigned int		m_nHigh;		///< 用于存储64位类型的数据的高32位
} CSUDIInvokeValue_S;

/**@brief 描述参数的属性*/
typedef struct{
	CSUDIInvokeValue_S     m_sArg;	 ///< 参数值
	CSUDIInvokeValueType_E m_eType;	 ///< 参数类型
}CSUDIInvokeArg_S;

/**
@brief 实现函数调用功能.

本函数功能 : 给定一个函数地址，该函数的参数列表，及各个参数的类型，实现该函数的调用功能，并返回函数执行的返回值
@param[in] pfFunction 被调用的函数指针。
@param[in] psArgs 存放被调用函数的所有参数的信息，详见CSUDIInvokeArg_S，参数存放顺序{{arg1,type},{arg2,type}, {arg3,type}, ... ,{argn, type}, ...}。
@param[in] nArgsSize 被调用函数的参数的个数。
@param[in] eReturnType 调用函数的返回值的类型，详见CSUDIInvokeValueType_E。
@param[out] psReturnValue 存放被调用函数的返回值，详见CSUDIInvokeValue_S。
@return 成功返回CSUDI_SUCCESS, 否则返回错误代码
@note 存储参数及返回值的规则见CSUDIInvokeValueType_E声明，二者保持一致。
*/
CSUDI_Error_Code CSUDIInvokeNative(void * pfFunction, CSUDIInvokeArg_S * psArgs, int nArgsSize, 
	CSUDIInvokeValueType_E eReturnType, CSUDIInvokeValue_S* psReturnValue);

#ifdef __cplusplus
}
#endif
/** @} */
#endif  //_UDI2_INVOKENATIVE_H_

