#ifndef _CS_INJ_TEST_ENTRY_H_
#define _CS_INJ_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSINJTESTEntry(void);
extern CSTestObj_S* CSINJTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

