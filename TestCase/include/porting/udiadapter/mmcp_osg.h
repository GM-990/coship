#ifndef MMCP_OSG_H
#define MMCP_OSG_H


#ifdef __cplusplus
extern "C" {
#endif

/**
osg模块仅供graph模块使用，graph模块内部目前的实现是
在graph中实现了一个UDI1.0->2.0的适配器。除非修改graph
本身，否则请引用graph的头文件而不是本文件。
*/
#ifdef MMCP_UDI2 
#include "udi2_osg.h"

#else /* MMCP_UDI2 */
#include "cs_osg.h"
#include "cs_iframe.h"

#endif /* MMCP_UDI2 */

#ifdef __cplusplus
}
#endif


#endif  /* MMCP_OSG_H */
