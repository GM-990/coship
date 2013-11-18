#ifndef _CS_DEBUG_TEST_ENTRY_H_
#define _CS_DEBUG_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSDEBUGTESTEntry(void);
extern CSTestObj_S* CSDEBUGTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif
