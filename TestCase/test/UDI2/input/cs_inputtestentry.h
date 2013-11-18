#ifndef _CS_INPUT_TEST_ENTRY_H_
#define _CS_INPUT_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSINPUTTESTEntry(void);
extern CSTestObj_S* CSINPUTTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

