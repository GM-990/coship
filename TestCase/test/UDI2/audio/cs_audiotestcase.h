#ifndef _CS_AUDIO_TEST_CASE_H_
#define _CS_AUDIO_TEST_CASE_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_AUDIO_Init(void);
CSUDI_BOOL CSTC_AUDIO_UnInit(void);

CSUDI_BOOL CSTC_AUDIO_GetCount_0001(void);
CSUDI_BOOL CSTC_AUDIO_GetCount_0002(void);
CSUDI_BOOL CSTC_AUDIO_GetCapability_0001(void);
CSUDI_BOOL CSTC_AUDIO_GetCapability_0002(void);
CSUDI_BOOL CSTC_AUDIO_GetCapability_0003(void);
CSUDI_BOOL CSTC_AUDIO_GetStatus_0001(void);
CSUDI_BOOL CSTC_AUDIO_GetStatus_0002(void);
CSUDI_BOOL CSTC_AUDIO_GetStatus_0003(void);
CSUDI_BOOL CSTC_AUDIO_GetStatus_0004(void);
CSUDI_BOOL CSTC_AUDIO_SetChannel_0001(void);
CSUDI_BOOL CSTC_AUDIO_SetChannel_0002(void);
CSUDI_BOOL CSTC_AUDIO_SetChannel_0003(void);
CSUDI_BOOL CSTC_AUDIO_Mute_0001(void);
CSUDI_BOOL CSTC_AUDIO_Unmute_0001(void);
CSUDI_BOOL CSTC_AUDIO_SetVolume_0001(void);
CSUDI_BOOL CSTC_AUDIO_SetVolume_0002(void);
CSUDI_BOOL CSTC_AUDIO_SetVolume_0003(void);
CSUDI_BOOL CSTC_AUDIO_SetVolume_0004(void);

CSUDI_BOOL CSTC_AUDIO_MuteAndUnmute_0001(void);
CSUDI_BOOL CSTC_AUDIO_IT_0001(void);
CSUDI_BOOL CSTC_AUDIO_IT_0002(void);
CSUDI_BOOL CSTC_AUDIO_IT_0003(void);
CSUDI_BOOL CSTC_AUDIO_IT_0004(void);
CSUDI_BOOL CSTC_AUDIO_IT_0005(void);
CSUDI_BOOL CSTC_AUDIO_IT_0006(void);
CSUDI_BOOL CSTC_AUDIO_AOUT_IT_0001(void);
CSUDI_BOOL CSTC_AUDIO_AOUT_IT_0002(void);



#ifdef __cplusplus
}
#endif

#endif

