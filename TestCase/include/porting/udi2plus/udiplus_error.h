/**@defgroup ERROR_PLUS ERROR Plus 定义UDIPlus中各个模块的错误代码

本模块用于MMCP中所有模块中的错误代码基本值定义，所有规则与UDI ERROR模块相同，详情请参考udi2_error.h

@warning  本模块仅供MMCP内部使用，不允许将本模块的任何定义、接口提供给MMCP以外的模块实用，包括但不限于UDI、集成接口中
@warning  不允许在需要对MMCP外部发布的头文件中包含本头文件

@version 2.0.0 2009/09/16 初稿
@{
*/

#ifndef _UDIPLUS_ERROR_H_
#define _UDIPLUS_ERROR_H_

#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief 各模块错误代码高位起始值枚举*/
enum
{
	EM_ERROR_START_UDI		= 0,		///< UDI模块错误代码高位起始值
	EM_ERROR_START_UDIPLUS	= 100,		///< UDIPlus模块错误代码高位起始值，需特别注意该值需与udi2_plus.h中的定义保持不冲突
	
	EM_ERROR_START_MAX								///< 边界值
};

/**@brief 各个模块错误代码基本值枚举定义

@note 每个模块的错误代码值都会包含四个基本的错误代码类型:BAD_PARAMETER，NO_MEMORY,FEATURE_NOT_SUPPORTED,UNKNOWN_ERROR
*/
enum
{
	CSUDIPLUS_OS_ERROR_BASE			= (EM_ERROR_START_UDIPLUS+1) << 16,	///< OS Plus模块的错误代码基本值
	CSUDIPLUS_DEBUG_ERROR_BASE		= (EM_ERROR_START_UDIPLUS+2) << 16,	///< Debug Plus模块的错误代码基本值
	
	CSUDIPLUS_MAX_ERROR_BASE
};

#ifdef __cplusplus
}
#endif
/** @} */
#endif  //_UDI2_ERROR_H_

