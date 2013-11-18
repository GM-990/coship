#ifndef _CS_INJECTER_TEST_CASE_H_
#define _CS_INJECTER_TEST_CASE_H_

#include "cs_testkit.h"
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_ap.h"
#include "udi2_dhcpserver.h"
#include "udi2_router.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_SOFTAP_Init(void);
CSUDI_BOOL CSTC_SOFTAP_UnInit(void);

#ifdef __cplusplus
}
#endif

#endif

