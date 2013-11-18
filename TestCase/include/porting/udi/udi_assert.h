/**@defgroup CSASSERT print 日志信息输出
@brief 本模块用于按模块名关键字及级别对日志信息进行输出和控制。
@brief 每一个模块(关键字)对应一个输出级别\n
每一条日志输出语句也对应一个输出级别和一个模块名(关键字)\n
当输出语句的级别不低于对应模块的输出级别时，输出该日志信息\n
CSASSERT、CSDEBUG、CSVDEBUG等全大写宏用于在非Debug模式下关闭所以打印\n
本模块可提供实现参考源代码
 
@version UDI_ASSERT_0.1
- 初始版本
@{
*/
#ifndef _UDI_ASSERT_H_
#define _UDI_ASSERT_H_

#include "udiplus_debug.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
	#include <assert.h>
	#define CSTRACE    USPPrint
#else
	#ifdef _DEBUG
		#define CSTRACE USPPrint
	#else
		#define CSTRACE 1?(void)0:USPPrint
	#endif

#endif

void USPPrint(int nLevel,const char *format, ...);


#ifdef	__cplusplus
}
#endif

/** @} */

#endif 
