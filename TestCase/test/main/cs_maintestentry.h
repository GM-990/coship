#ifndef _CS_CFS_TEST_ENTRY_H_
#define _CS_CFS_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSMAINTESTEntry(void);
extern CSTestObj_S* CSMAINTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

