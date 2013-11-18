#ifndef _CS_USB_TEST_CASE_H_
#define _CS_USB_TEST_CASE_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOL CSTC_USB_Init(void);
BOOL CSTC_USB_UnInit(void);

BOOL CSTC_USB_IT_GetCount_0001(void);
BOOL CSTC_USB_IT_GetCount_0002(void);
BOOL CSTC_USB_IT_GetInfo_0001(void);
BOOL CSTC_USB_IT_GetInfo_0002(void);

#ifdef __cplusplus
}
#endif

#endif

