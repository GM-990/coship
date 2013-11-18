#ifndef _INJECTER_H_
#define _INJECTER_H_

#include "cs_testkit.h"
#include "udi2_typedef.h"
#include "udi2_input.h"
#include "udi2_error.h"
#include "udi2_inject.h"

#ifdef __cplusplus
extern "C" {
#endif

int searchDemuxID_Y_InInject(int index);
int searchAudioID_Y_InInject(int index);
int searchVideoID_Y_InInject(int type_index);
void fnReqDataCallback(void *pvContext);


#ifdef __cplusplus
}
#endif

#endif
