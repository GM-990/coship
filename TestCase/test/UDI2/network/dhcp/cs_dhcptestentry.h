#ifndef _CS_DHCP_TEST_ENTRY_H_
#define _CS_DHCP_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSDHCPTESTEntry(void);
extern CSTestObj_S* CSDHCPTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

