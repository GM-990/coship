#ifndef MMCP_TUNER_H
#define MMCP_TUNER_H
#ifdef __cplusplus
extern "C" {
#endif

/**
无论是基于UDI2.0还是1.0的接口实现MMCP，都直接使用UDI2.0
的接口。在1.0的平台上，MMCP会统一提供UDI2.0的Adapter。
当然，这两套接口的定义会有略微的差异，请慎重使用这些细节。
*/
#include "udi2_tuner.h"

#ifdef __cplusplus
}
#endif

#endif  /* MMCP_TUNER_H */

