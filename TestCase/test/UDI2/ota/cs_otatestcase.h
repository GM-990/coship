#ifndef _CS_OTA_TEST_CASE_H_
#define _CS_OTA_TEST_CASE_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_OTA_Init(void);
CSUDI_BOOL CSTC_OTA_UnInit(void);

CSUDI_BOOL CSTC_OTA_IT_GetInfo_0001(void);
CSUDI_BOOL CSTC_OTA_IT_SetInfo_0001(void);
CSUDI_BOOL CSTC_OTA_IT_SetOta_0001(void);
CSUDI_BOOL CSTC_OTA_IT_WIFIAddDelCallback_0001(void);
CSUDI_BOOL CSTC_OTA_IT_WIFIAddDelCallback_0002(void);

#ifdef __cplusplus
}
#endif

#endif

