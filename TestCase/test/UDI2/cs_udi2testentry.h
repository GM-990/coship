#ifndef _CS_UDI2_TEST_ENTRY_H_
#define _CS_UDI2_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSUDI2TESTEntry(void);

int CSUDIGetTestCaseVersion( char* pcVersionInfo, int nSize);

extern CSTestObj_S* CSUDI2TESTGetObj(void);//�����������

#ifdef __cplusplus
}
#endif

#endif

