#ifndef _CSHDI_TYPEDEF_H_
#define _CSHDI_TYPEDEF_H_

#include "cs_hdicommon_plus.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief 获得移植层平台接口的版本信息

@param[out] pcVersionInfo 显示字符串的最大长度
@param[in] nSize 入口参数，其值为调用CSCreateTimer函数时的pvArg参数。
@return 当前版本的长度,失败返回0
@note 输出格式为：
- "CSHDI-R-主版本.子版本.改动次数 "; Release Version 
- "CSHDI-D-主版本.子版本.改动次数";  Debug Version
- "CSHDI-T-主版本.子版本.改动次数";  Test Version 
*/
int CSHDIGetVersion( char* pcVersionInfo, int nSize);

#ifdef __cplusplus
}
#endif

#endif
