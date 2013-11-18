#ifndef _CS_TS_INJECTER_TEST_ENTRY_H_
#define _CS_TS_INJECTER_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSTS_INJECTERTESTEntry(void);
extern CSTestObj_S* CSTS_INJECTERTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

