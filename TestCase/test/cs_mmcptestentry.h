#ifndef _CS_MMCP_TEST_ENTRY_H_
#define _CS_MMCP_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSMMCPTESTEntry(void);
extern CSTestObj_S* CSMMCPTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

