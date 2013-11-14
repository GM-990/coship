/**@defgroup DEBUG DEBUG 日志信息输出

@brief Debug模块对应头文件<udi2_debug.h>，定义了基本日志输出接口。现对该模块进行如下说明：

- 建议将日志打印输出到串口。\n
- 实现时需特别注意输出字符串中可能包含“%s”等的情况。\n
- EastWin会处理好日志输出中的参数规则，移植层无需关心。\n
- 由于调试情况下日志信息较多，该模块的实现需特别注意效率问题。\n
- 注意当多个线程同时调用输出接口时，输出内容不可交叉、混淆。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.0 2009/8/22 将控制逻辑上移,移植层仅保留基本输出接口,相关说明同时上移
@version 1.0.1 2008/8/10 增加FAQ
@version 1.0.0 2008/7/25 初始版本
@{
*/

#ifndef _UDI2_DEBUG_H_
#define _UDI2_DEBUG_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
@brief 日志输出函数

@param[in] pStr 格式化好的日志内容
@code 
printf("%s",pStr)//参考实现
@endcode
@note 
- 本函数仅被特定模块调用，不推荐外部直接用于输出，除非有充分的理由，否则不允许上层代码绕过上层打印控制接口直接使用该函数输出。\n
- EastWin会处理好日志输出中的参数规则，移植层无需关心。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
void CSDebugDefaultOutputString(const char * pStr );


#ifdef	__cplusplus
}
#endif

/**@}*/

#endif 

