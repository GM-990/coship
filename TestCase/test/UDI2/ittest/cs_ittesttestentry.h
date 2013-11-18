#ifndef _CS_ITTEST_TEST_ENTRY_H_
#define _CS_ITTEST_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSITTESTTESTEntry(void);
extern CSTestObj_S* CSITTESTTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

