/**@defgroup PublicShellBootShow PublicShellBootShow 定义BootShow对外接口

@{
*/
#ifndef CSSHELL_BOOTSHOW_PRIVATE_H
#define CSSHELL_BOOTSHOW_PRIVATE_H

#include "csshell_bootshow.h"
#ifdef __cplusplus
extern "C"
{
#endif

/*=============================================
功能：获取BootShow的值
=============================================*/		
const CSShellBootShowInitParam_S* CSShellBootShowGetParam(void);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


