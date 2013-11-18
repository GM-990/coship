#ifndef _CS_TOOLSET_TEST_ENTRY_H_
#define _CS_TOOLSET_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSTOOLSETTESTEntry(void);
extern CSTestObj_S* CSTOOLSETTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

