#ifndef _CS_IFRAME_INJECTER_TEST_CASE_H_
#define _CS_IFRAME_INJECTER_TEST_CASE_H_

#include "cs_testkit.h"
#include "udi2_typedef.h"
#include "udi2_input.h"
#include "udi2_error.h"
#include "udi2_player.h"
#include "udi2_inject.h"
#include "udi2_fs.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_IFRAME_INJECTER_Init(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_UnInit(void);

CSUDI_BOOL CSTC_IFRAME_INJECTER_OpenClose_001(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_OpenClose_002(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_OpenClose_003(void);


CSUDI_BOOL CSTC_IFRAME_INJECTER_SetGetAttribute_001(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_SetGetAttribute_002(void);

CSUDI_BOOL CSTC_IFRAME_INJECTER_StartStop_001(void);

CSUDI_BOOL CSTC_IFRAME_INJECTER_001(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_002(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_003(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_480I(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_576I(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_720P(void);
CSUDI_BOOL CSTC_IFRAME_INJECTER_DATA_1080I(void);

#ifdef __cplusplus
}
#endif

#endif

