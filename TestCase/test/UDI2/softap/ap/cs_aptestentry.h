#ifndef _CS_AP_TEST_ENTRY_H_
#define _CS_AP_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSAPTESTEntry(void);
extern CSTestObj_S* CSAPTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

