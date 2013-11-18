#ifndef CSSHELL_VOIP_PRIVATE_H
#define CSSHELL_VOIP_PRIVATE_H

#include "mmcp_typedef.h"
#include "csshell_voip.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 获取ota升级函数接口
@param 无
@return psParam ota升级shell层要实现的接口
*/
const CSShellVOIPInitParam_S* CSShellVOIPgetParam();

#ifdef __cplusplus
}
#endif

#endif

