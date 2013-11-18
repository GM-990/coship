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
	EM_BOUND_CHECK_BELOW	 =			0,/*����ڴ�����*/
	EM_BOUND_CHECK_ABOVE =			1/*����ڴ�����*/
}CSBoundCheckMode_E;

void *bc_malloc(int s);
void *bc_realloc(void *buf, int s);
void *bc_calloc(int n, int s);
void *bc_realloc(void *buf, int s);
int bc_free(void *buf);


/**�����ڴ���ģʽ
* ����eBCMode Ϊ������ģʽ
*�������øýӿڣ���ΪĬ��
*Ĭ�ϼ��ģʽEM_BOUND_CHECK_BELOW
*/
int CSSetBoundCheckMode(CSBoundCheckMode_E eBCMode);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* CS_OS_H */
