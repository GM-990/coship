#ifndef MMCP_OSG_H
#define MMCP_OSG_H


#ifdef __cplusplus
extern "C" {
#endif

/**
osgģ�����graphģ��ʹ�ã�graphģ���ڲ�Ŀǰ��ʵ����
��graph��ʵ����һ��UDI1.0->2.0���������������޸�graph
��������������graph��ͷ�ļ������Ǳ��ļ���
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
