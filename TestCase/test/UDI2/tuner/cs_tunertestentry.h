#ifndef _CS_TUNER_TEST_ENTRY_H_
#define _CS_TUNER_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSTUNERTESTEntry(void);
extern CSTestObj_S* CSTUNERTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

