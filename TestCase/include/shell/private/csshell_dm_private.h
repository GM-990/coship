/**@defgroup PublicShellDM PublicShellDM 定义DM对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_DM_PRIVATE_H
#define CSSHELL_DM_PRIVATE_H

#include "csshell_dm.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
/**
@brief 获取DM实现接口
@return 成功返回之前设置的DM实现接口
@see CSShellDMGetDefaulFunc
*/
const CSShellDMFunc_S* CSShellDMGetFunc(void);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


