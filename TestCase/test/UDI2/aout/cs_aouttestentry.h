#ifndef _CS_AOUT_TEST_ENTRY_H_
#define _CS_AOUT_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSAOUTTESTEntry(void);
extern CSTestObj_S* CSAOUTTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

