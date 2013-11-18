#ifndef _CS_UDI2_TEST_ENTRY_H_
#define _CS_UDI2_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSUDI2TESTEntry(void);

int CSUDIGetTestCaseVersion( char* pcVersionInfo, int nSize);

extern CSTestObj_S* CSUDI2TESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

