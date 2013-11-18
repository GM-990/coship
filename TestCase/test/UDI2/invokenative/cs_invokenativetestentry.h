#ifndef _CS_INVOKENATIVE_TEST_ENTRY_H_
#define _CS_INVOKENATIVE_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSINVOKENATIVETESTEntry(void);
extern CSTestObj_S* CSINVOKENATIVETESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

