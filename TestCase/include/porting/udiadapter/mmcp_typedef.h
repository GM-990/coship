#ifndef MMCP_TYPEDEF_H
#define MMCP_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
UDI2.0对于基本类型的定义本身已经考虑了1.0的兼容
而且这些类型的修改量会比较大，为保持稳定，快速发布，
暂时无论1.0还是2.0均使用旧的定义，即DWORD,HCSHANDLE等。
但这些定义在两个版本中定义在不同的头文件里。
需特别说明的是，64位整数使用2.0版本的定义，及CSUDI_INT64。
*/
#include "udiplus_typedef.h"

#ifdef __cplusplus
}
#endif 

#endif  /* MMCP_TYPEDEF_H */


