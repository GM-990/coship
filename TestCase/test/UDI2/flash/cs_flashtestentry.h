#ifndef _CS_FLASH_TEST_ENTRY_H_
#define _CS_FLASH_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSFLASHTESTEntry(void);
extern CSTestObj_S* CSFLASHTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

