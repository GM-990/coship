
#ifndef _CSSHELL_AMSREGIST_H_
#define _CSSHELL_AMSREGIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"



//注册C 应用管理器函数集
BOOL CSShellRegistCAppManagerFuns();

//注册Java native应用管理器函数集
BOOL CSShellRegistJavaAppManagerFuns();


#ifdef __cplusplus
}
#endif

#endif

