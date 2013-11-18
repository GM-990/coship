#ifndef _CS_FILE_TEST_ENTRY_H_
#define _CS_FILE_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSFILETESTEntry(void);
extern CSTestObj_S* CSFILETESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

