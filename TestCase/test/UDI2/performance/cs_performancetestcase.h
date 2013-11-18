#ifndef _CS_PERFORMANCE_TEST_CASE_H_
#define _CS_PERFORMANCE_TEST_CASE_H_

#include "cs_testkit.h"
#include "udi2_typedef.h"


#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_PERFORMANCE_Init(void);
CSUDI_BOOL CSTC_PERFORMANCE_UnInit(void);

CSUDI_BOOL CSTC_PERFORMANCE_MT_0001(void);
CSUDI_BOOL CSTC_PERFORMANCE_MT_0002(void);
CSUDI_BOOL CSTC_PERFORMANCE_MT_0003(void);
CSUDI_BOOL CSTC_PERFORMANCE_MT_0004(void);

#ifdef __cplusplus
}
#endif

#endif

