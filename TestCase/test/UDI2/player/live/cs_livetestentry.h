#ifndef _CS_LIVE_TEST_ENTRY_H_
#define _CS_LIVE_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSLIVETESTEntry(void);
extern CSTestObj_S* CSLIVETESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

