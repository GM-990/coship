#ifndef _CS_PERFORMANCE_TEST_ENTRY_H_
#define _CS_PERFORMANCE_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSPERFORMANCETESTEntry(void);
extern CSTestObj_S* CSPERFORMANCETESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

