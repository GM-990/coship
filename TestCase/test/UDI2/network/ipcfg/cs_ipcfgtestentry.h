#ifndef _CS_IPCFG_TEST_ENTRY_H_
#define _CS_IPCFG_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSIPCFGTESTEntry(void);
extern CSTestObj_S* CSIPCFGTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

