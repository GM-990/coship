#ifndef _CS_DHCPS_TEST_ENTRY_H_
#define _CS_DHCPS_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSDHCPSTESTEntry(void);
extern CSTestObj_S* CSDHCPSTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif
