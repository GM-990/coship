#ifndef _CS_USB_TEST_ENTRY_H_
#define _CS_USB_TEST_ENTRY_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

void CSUSBTESTEntry(void);
extern CSTestObj_S* CSUSBTESTGetObj(void);//本级测试入口

#ifdef __cplusplus
}
#endif

#endif

