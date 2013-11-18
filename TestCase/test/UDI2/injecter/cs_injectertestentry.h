#ifndef _CS_INJECTER_TEST_ENTRY_H_
#define _CS_INJECTER_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSINJECTERTESTEntry(void);
extern CSTestObj_S* CSINJECTERTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

