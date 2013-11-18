#ifndef _CS_SMARTCARD_TEST_ENTRY_H_
#define _CS_SMARTCARD_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSSMARTCARDTESTEntry(void);
extern CSTestObj_S* CSSMARTCARDTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

