/**@defgroup PublicShellTF3CA PublicShellTF3CA 定义Top CA对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_TOPCA_PRIVATE_H
#define CSSHELL_TOPCA_PRIVATE_H

#include "csshell_topca.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
	/**
	@brief 获取Top CA实现接口
	@return 成功返回之前设置的Top ca实现接口
	@see CSShellTf3CaGetDefaultParam
	*/
const CSShellTopCaInitParam_S* CSShellTopCaGetParam(void);


/**
@brief 获取Top ca的相关配置信息
@return 成功返回配置信息，失败返回NULL
@note 当应用没有调用CSShellTopCaSetConfig()设置配置信息时，返回NULL
@see CSShellTopCaSetConfig
*/
const CSShellTopCaConfig_S* CSShellTopCaGetConfig(void);
#ifdef __cplusplus
}
#endif
/** @} */
#endif


