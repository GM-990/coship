/**@defgroup DEBUG_PLUS DEBUG Plus UDI DEBUG 模块的补充模块，提供更多打印控制接口

@brief 本模块用于按模块名关键字及级别对日志信息进行输出和控制。
@brief 每一个模块(关键字)对应一个输出级别。
每一条日志输出语句也对应一个输出级别和一个模块名(关键字)。
当输出语句的级别不低于对应模块的输出级别时，输出该日志信息。
CSASSERT、CSDEBUG、CSVDEBUG等全大写宏用于在非Debug模式下关闭所以打印。

@warning  本模块仅供MMCP内部使用，不允许将本模块的任何定义、接口提供给MMCP以外的模块实用，包括但不限于UDI、集成接口中
@warning  不允许在需要对MMCP外部发布的头文件中包含本头文件
 
@version 2009/09/16 根据UDI2.1版本的定义，重新调整本模块
- 初始版本
@{
*/

#include <stdarg.h>
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_debug.h"
#include "udiplus_error.h"

#ifndef _UDIPLUS_DEBUG_H_
#define _UDIPLUS_DEBUG_H_

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief Debug Plus 模块相关错误代码值*/
enum
{
	CSUDIDEBUGPLUS_ERROR_BAD_PARAMETER = CSUDIPLUS_DEBUG_ERROR_BASE,	///< 参数错误
	CSUDIDEBUGPLUS_ERROR_FEATURE_NOT_SUPPORTED,							///< 不支持此操作
	CSUDIDEBUGPLUS_ERROR_UNKNOWN_ERROR,									///< 一般错误
	CSUDIDEBUGPLUS_ERROR_NO_MEMORY,										///< 无内存可用错误
	
	CSUDIDEBUGPLUS_ERROR_MAX											///< 边界值
};

/**@brief 调试级*/
#define DEBUG_LEVEL		    (0)

/**@brief 流程等提示级*/
#define INFO_LEVEL 		    (1)

/**@brief 不是错误但可能需要处理级*/
#define NOTICE_LEVEL		(0x0FFFFFFF)

/**@brief 可恢复故障级*/
#define WARN_LEVEL 		    (0x1FFFFFFF)

/**@brief 不确定是否可恢复故障级*/
#define ERROR_LEVEL		    (0x3FFFFFFF)

/**@brief 无法恢复的故障级*/
#define FATAL_LEVEL		    (0x7FFFFFFD)

/**@brief command line interface级，用于shell交互等必须输出的日志*/
#define CLI_LEVEL		    (0x7FFFFFFE)

/**@brief 禁止级，只能用于CSDebugSetDefaultLevel，CSDebugSet等设置接口，不可用于CSDebug等输出接口*/
#define DISABLE_LEVEL		(0x7FFFFFFF)

/**@brief CSDebugSet时，使用该宏表示设置所有模块的日志级别*/
#define MODULE_NAME_ALL     ("*all")

/**@cond */
#ifdef _DEBUG
	#define CSASSERT(expression)  \
	do{ \
		if(!(expression)) \
		CSDebug("CSDebug",FATAL_LEVEL,"Assertion: \"%s\" failed, in file %s, line %d\n", \
				#expression, __FILE__, __LINE__); \
	}while(0)
	
	#define CSASSERT_FAILED(exp) (!(exp)? (CSDebug("CSDebug", FATAL_LEVEL, "Assertion \"%s\" failed, in file %s line %d\n", #exp, __FILE__,__LINE__), 1) : 0) 

	#define CSDEBUG    	CSDebug
	#define CSVDEBUG	CSVDebug
#else
	#define CSASSERT(x)
	#define CSASSERT_FAILED(exp) (!(exp))
/**@brief 为了在release版本删除所有打印信息，MMCP_FORCE_DELETE_DEBUGINFO宏在menuconfig中定义*/
#ifdef MMCP_RELEASE_DEL_LOG
	#define CSDEBUG		1?(void)0:CSDebugNULL
	#define CSVDEBUG	1?(void)0:CSVDebugNULL	
	#define CSDebug		1?(void)0:CSDebugNULL
	#define CSVDebug	1?(void)0:CSVDebugNULL
#else
	#define CSDEBUG		1?(void)0:CSDebug
	#define CSVDEBUG	1?(void)0:CSVDebug
#endif
#endif
/**@endcond*/

/**
@brief 日志信息输出回调函数函数

格式化好的日志信息最终会调用一个此形式的函数进行输出，如果应用想自行控制日志信息的输出，
则可定义一个此形式的函数调用CSDebugRegisterOutputCallback
@param[in] pStr 格式化好的日志字符串，用于输出
@see CSDebugRegisterOutputCallback
*/
typedef  void ( *CSDebugOutPutString )( const char* pStr );

/**
@brief 模块初始化，设置默认级别

一般来说在应用尽量早的地方进行初始化，用于控制以后各模块的打印
@param[in] nDefaultLevel 默认级别
@return 之前的默认级别，首次调用该值不确定
@note 首次调用任何带模块名的函数时，除非明确指定该模块打印级别，否则会被设置成nDefaultLevel
@note 所有后续新增模块的输出级别将使用该值，可多次调用，但每次调用后仅对后续新增模块起作用
@note 所以级别比较采用数学比较，即即负数 < 0 < 正数
@note 这是一个应用配置接口，不得在MMCP代码中调用本接口设置打印级别。
*/
int CSDebugSetDefaultLevel(int nDefaultLevel);

/**
@brief 设置模块日志输出级别

首次设置即为注册，模块以模块名字符串为关键字，如果模块已经注册过，则改变其日志级别
@param[in] pcModuleName 模块名关键字
@param[in] nOutputLevel 模块设置后的日志级别
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 所有模块名仅保留指针，请保证注册时模块名的有效性，建议直接使用字符串常量调用
@note 使用MODULE_NAME_ALL宏作为模块名参数，表示设置所有模块的日志级别
@note 这是一个应用配置接口，不得在MMCP代码中调用本接口设置打印级别。
*/
CSUDI_Error_Code CSDebugSet(const char* pcModuleName,int nOutputLevel);

/**
@brief 获取所以已注册的模块名

模块名会在输出在日志中，如果ppModules和nMaxCount不为0也会存储在其中，并且以CSUDI_NULL结束
@param[out] ppModules 用于存储模块名的指针数组，可以为CSUDI_NULL
@param[in] nMaxCount 指针数组的大小
@return 实际模块数，与ppModules及nMaxCount无关
@note 最多能存储nMaxCount-1个模块名到ppModules中，因为结尾符NULL会占用一个项
@note 无论ppModules和nMaxCount如何取值，所以模块名都会输出到日志文件里。
@note 这是一个应用配置接口，除非特别允许，否则不得在MMCP代码中调用本接口查询模块名称。
*/
int CSDebugGetModuleInfo(const char** ppModules,int nMaxCount);

/**
@brief 注册日志输出函数

所以日志内容经格式化，判断释放要输出后，如需要输出会以格式化好的字符串调用本函数
@param[in] OutputCallback 指定的输出函数
@return 成功返回CSUDI_SUCCESS;失败返回错误代码
@note 注册后，之前的输出函数将不再调用，如果本函数没有调用过，会使用默认输出函数进行输出
@note 除非有特殊目地，如将日志重定向到文件中，否则不推荐使用本函数，因为默认函数就是平台推荐的输出函数
@note 请特别注意不要在回调中再次调用CSDebug及包含CSDebug的函数(包括断言)，否则可能会引起无限递归调用
@note 默认的日志输出函数为CSUDIDEBUGOutputString，当需要恢复默认时也可以使用CSUDIDEBUGOutputString为参数调用本函数
@note 这是一个应用配置接口，除非特别允许，否则不得在MMCP代码中调用本接口重定向日志输出
*/
CSUDI_Error_Code CSDebugRegisterOutputCallback(CSDebugOutPutString OutputCallback);

#if !defined(_DEBUG) && defined(MMCP_RELEASE_DEL_LOG) 
/**
@brief 以下2个函数仅仅是为了解决编译问题；禁止直接调用此接口；驱动也不需要实现此接口；
*/
void CSDebugNULL(const char * pcModuleName, int nOutputLevel, const char *pcFormat, ...);
void CSVDebugNULL(const char * pcModuleName, int nOutputLevel, const char *pcFormat,va_list arg);
#else
/**
@brief 以特定级别输出某个模块的日志信息

当nOutputLevel >= pcModuleName的日志级别时，输出日志信息，否则直接返回
@param[in] pcModuleName 模块名关键字
@param[in] nOutputLevel 日志输出级别
@param[in] pcFormat 日志字符串及格式，同printf的定义
@note 所有模块名仅保留指针，请保证注册时模块名的有效性，建议直接使用字符串常量调用
@note nOutputLevel不能等于DISABLE_LEVEL否则会自动调整为CLI_LEVEL
*/
void CSDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat, ...);

/**
@brief V版本的CSDebug

与CSDebug的关系相当于vprintf与printf的关系
@param[in] pcModuleName 模块名关键字
@param[in] nOutputLevel 日志输出级别
@param[in] pcFormat 日志字符串及格式，同printf的定义
@param[in] arg 日志字符串中的参数列表
@note 所有模块名仅保留指针，请保证注册时模块名的有效性，建议直接使用字符串常量调用
@note nOutputLevel不能等于DISABLE_LEVEL否则会自动调整为CLI_LEVEL
*/
void CSVDebug(const char * pcModuleName, int nOutputLevel, const char *pcFormat,va_list arg);
#endif

#ifdef	__cplusplus
}
#endif
/** @} */

#endif

