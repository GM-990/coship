#ifndef _CS_EW200SERVICE_TEST_ENTRY_H_
#define _CS_EW200SERVICE_TEST_ENTRY_H_

#include "cs_testkit.h"
#include "cs_ew200servicetestcase.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSEW200SERVICETESTEntry(void);
extern CSTestObj_S* CSEW200SERVICETESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

