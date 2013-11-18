#ifndef _CS_INJECTER_TEST_CASE_H_
#define _CS_INJECTER_TEST_CASE_H_

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

CSUDI_BOOL CSTC_INJECTER_Init(void);
CSUDI_BOOL CSTC_INJECTER_UnInit(void);

#ifdef __cplusplus
}
#endif

#endif

