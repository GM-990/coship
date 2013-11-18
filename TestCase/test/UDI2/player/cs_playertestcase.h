#ifndef _CS_PLAYER_TEST_CASE_H_
#define _CS_PLAYER_TEST_CASE_H_

#include "cs_testkit.h"

#include "udi2_typedef.h"
#include "udi2_input.h"
#include "udi2_error.h"



#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_PLAYER_Init(void);
CSUDI_BOOL CSTC_PLAYER_UnInit(void);

CSUDI_BOOL CSTC_PLAYER_MT_0001(void);
CSUDI_BOOL CSTC_PLAYER_MT_0002(void);

#ifdef __cplusplus
}
#endif

#endif

