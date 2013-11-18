#ifndef _CS_OSG_TEST_ENTRY_H_
#define _CS_OSG_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSOSGTESTEntry(void);
extern CSTestObj_S* CSOSGTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

