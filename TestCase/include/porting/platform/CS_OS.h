/** @addtogroup CSOS
@{
*/

#ifndef _CS_OS_H_
#define _CS_OS_H_

#include "cs_typedef.h"
#include "cs_assert.h"
#include "cs_clocktimer.h"
#include "udi_os.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
@brief 永不超时
@deprecated 仅为保持兼容保留，建议使用CS_OSP_TIMEOUT_INFINITY
*/
#define CS_WAIT_FOREVER					CS_OSP_TIMEOUT_INFINITY

//#define CS_OSP_TASK_FLAGS_MIN_STATCK_SIZE 		(2) 


#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* CS_OS_H */
