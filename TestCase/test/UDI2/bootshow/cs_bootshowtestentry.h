#ifndef _CS_BOOTSHOW_TEST_ENTRY_H_
#define _CS_BOOTSHOW_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSBOOTSHOWTESTEntry(void);
extern CSTestObj_S* CSBOOTSHOWTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

