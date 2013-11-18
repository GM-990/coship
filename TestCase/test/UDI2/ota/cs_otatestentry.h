#ifndef _CS_OTA_TEST_ENTRY_H_
#define _CS_OTA_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSOTATESTEntry(void);
extern CSTestObj_S* CSOTATESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

