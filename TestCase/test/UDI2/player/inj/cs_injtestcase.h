#ifndef _CS_INJ_TEST_CASE_H_
#define _CS_INJ_TEST_CASE_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_INJ_Init(void);
CSUDI_BOOL CSTC_INJ_UnInit(void);

CSUDI_BOOL CSTC_INJ_MT_0001(void);
CSUDI_BOOL CSTC_INJ_MT_0002(void);

#ifdef __cplusplus
}
#endif

#endif

