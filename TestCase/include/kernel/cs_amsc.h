/**@defgroup 应用管理器数据库接口
@brief
*/
#ifndef _CS_AMSC_H
#define _CS_AMSC_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "cs_amsregist.h"


/**
@brief 获取C 应用管理器函数集
@return 函数集
*/
CSAMSFuns_S *CSAMSCGetFuns();


#ifdef  __cplusplus
}
#endif

#endif

