#ifndef _CS_FS_TEST_ENTRY_H_
#define _CS_FS_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSFSTESTEntry(void);
extern CSTestObj_S* CSFSTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

