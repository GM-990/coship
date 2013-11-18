/**@defgroup PublicShell PublicShell 定义Shell公共对外接口及方法

@version 2.0.0 2009/12/16 初稿
@{
*/

#include "mmcp_typedef.h"

#ifndef CSSHELL_INTERFACE_H
#define CSSHELL_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

#define MODULE_SHELL         "shell"

typedef enum _CSShellTunerDvbType
{
	EM_CSSHELL_DVB_TUNER_UNKOWN = 0,
	EM_CSSHELL_DVB_S,
	EM_CSSHELL_DVB_C,
	EM_CSSHELL_DVB_T
}CSShellTunerDvbType_E;

/**
@brief MMCP初始化
根据指定的设置环境，进行MMCP的初始化，需在字库、CA、搜索等需配置回调函数的模块的配置参数设置完成后执行
@return 成功返回0；失败<0
@todo 暂不实现，目前总是返回-1
*/
int CSShellInitMMCP();

/**
@brief 从配置文件中获取Tuner类型，
@param[in] nTunerIndex: Tuner索引，对于多Tuner的情况，通过index区分，单Tuner的情况，传0即可
@return Tuner类型枚举，若获取失败，返回DVB_TUNER_UNKOWN
*/
CSShellTunerDvbType_E CSShellGetTunerDvbType(int nTunerIndex);

/**
@brief 从配置文件中获取Tuner个数
@param[out] pnTunerCount: 保存个数
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSShellGetTunerCount(int *pnTunerCount);


#ifdef __cplusplus
}
#endif
/** @} */
#endif   

