#ifndef _KERNEL_LOG_COLLECT_H
#define _KERNEL_LOG_COLLECT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
	函数：	CSLogCollectInit
	描述：	用于完成日志收集工具的初始化操作，用于有java的工程初始化
	输入：
			无
	输出：
			无
	返回：
			无
*/
void CSLogCollectInit(void);

/*
	函数：	CSLogCollectInit
	描述：	用于完成日志收集工具的初始化操作，用于没有java的工程初始化，解除对jni接口的依赖
	输入：
			无
	输出：
			无
	返回：
			无
*/
void CSLogCollectInitSimple(void);

/*
	函数：	CSLogCollecDispatchMsg
	描述：	通过网络、usb两种方式进行分发日志信息
	输入：
			pcMsg - 日志信息
	输出：
			无
	返回：
			无
*/
void CSLogCollectDispatchMsg(const char * pcString);

/*
	函数：	CSLogCollectSetDebug
	描述：	设置打印级别
	输入：
			pcDebugStr - 打印级别信息
			nLength - pcDebugStr的长度
	输出：
			无
	返回：
			无
	备注：
			各个模块的打印级别信息以字符串形式输入，其格式如下：

			C模块的打印格式：
			DbgL.模块名=打印级别编号

			C模块打印级别：
			0：DEBUG_LEVEL
			1：INFO_LEVEL
			2：NOTICE_LEVEL
			3：WARN_LEVEL
			4：ERROR_LEVEL
			5：FATAL_LEVEL
			6：CLI_LEVEL
			7：DISABLE_LEVEL

			将所有的C模块设置为同一打印级别的格式如下：
			DbgL.default=打印级别编号

			注意：在修改打印级别时，若打印级别信息中若无“DbgL.default”来设置默认打印级别，则先将所有的C模块设置为不DISABLE_LEVEL级别，然后再设置所关注模块的打印级别。

			Java模块的打印格式：
			-D模块名.debug=打印级别编码

			Java模块打印级别：
			0：DEBUG_LEVEL
			1：INFO_LEVEL
			2：WARN_LEVEL
			3：ERROR_LEVEL


			网络打印方式打印级别信息示例：
			DbgL.default=5
			DbgL.Java=0
			DbgL.CVM=0
			DbgL.JSPrint=0
			-Dappmanager.debug=3
			注意：不同模块之间以回车换行符(\n或\r\n))进行分割。若某一模块被输入了多次，则按最后一次处理。
*/
void CSLogCollectSetDebug(const char * pcDebugStr, int nLength);

#ifdef __cplusplus
}
#endif

#endif

