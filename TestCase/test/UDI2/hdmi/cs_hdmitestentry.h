#ifndef _CS_HDMI_TEST_ENTRY_H_
#define _CS_HDMI_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSHDMITESTEntry(void);
extern CSTestObj_S* CSHDMITESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

