#ifndef MMCP_INPUT_H
#define MMCP_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
始终使用UDI2.0的头文件，因为这里仅仅是定义了键值
没有任何接口，无论在1.0还是2.0的环境，我们始终按
2.0的键值进行处理。这不存在任何接口定义问题。
*/
#include "udi2_input.h"

#ifdef __cplusplus
}
#endif

#endif  /* MMCP_INPUT_H */
