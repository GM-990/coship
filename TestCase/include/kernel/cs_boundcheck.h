/** @addtogroup CSOS
@{
*/

#ifndef _CS_BOUNDCHECK_H_
#define _CS_BOUNDCHECK_H_

#include "mmcp_typedef.h"
#include "mmcp_debug.h"

#ifdef  __cplusplus
extern "C"
{
#endif

typedef enum _CSBoundCheckMode_E
{
	EM_BOUND_CHECK_BELOW	 =			0,/*检测内存下溢*/
	EM_BOUND_CHECK_ABOVE =			1/*检测内存上溢*/
}CSBoundCheckMode_E;

void *bc_malloc(int s);
void *bc_realloc(void *buf, int s);
void *bc_calloc(int n, int s);
void *bc_realloc(void *buf, int s);
int bc_free(void *buf);


/**设置内存检测模式
* 参数eBCMode 为待设置模式
*若不调用该接口，则为默认
*默认检测模式EM_BOUND_CHECK_BELOW
*/
int CSSetBoundCheckMode(CSBoundCheckMode_E eBCMode);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* CS_OS_H */
