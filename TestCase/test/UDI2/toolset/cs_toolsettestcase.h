#ifndef _CS_TOOLSET_TEST_CASE_H_
#define _CS_TOOLSET_TEST_CASE_H_

#include "cs_testkit.h"
#include "cs_toolset_test.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL  CSTC_TOOLSET_Init(void);
CSUDI_BOOL  CSTC_TOOLSET_UnInit(void);

CSUDI_BOOL  CSTC_TOOLSET_IT_GetChipID_0001(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_GetChipID_0002(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_GetSerialKey_0001(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_GetSerialKey_0002(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_Reboot_0001(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_Halt_0001(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_Standby_0001(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_Standby_0002(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_Standby_0003(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_Standby_0004(void);
CSUDI_BOOL  CSTC_TOOLSET_IT_Standby_0005(void);

#ifdef __cplusplus
}
#endif

#endif

