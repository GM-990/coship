#ifndef _CS_OS_TEST_ENTRY_H_
#define _CS_OS_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSOSTESTEntry(void);
extern CSTestObj_S* CSOSTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

