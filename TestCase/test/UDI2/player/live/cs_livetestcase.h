#ifndef _CS_LIVE_TEST_CASE_H_
#define _CS_LIVE_TEST_CASE_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_LIVE_Init(void);
CSUDI_BOOL CSTC_LIVE_UnInit(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0003(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_Add_DelPlayerCallback_0004(void);

CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0001(void);
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0002(void);
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0003(void);
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0004(void);
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0005(void);
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0006(void);
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0007(void);
CSUDI_BOOL CSTC_LIVE_IT_CSUDIPLAYEROpenClose_0008(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0003(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0004(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0005(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0006(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0007(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0008(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SetGetStream_0009(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_StartStop_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_StartStop_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_StartStop_0003(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_PauseResume_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_PauseResume_0002(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_SetSpeed_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_Seek_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPosInSec_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPlaybackParam_0001(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0003(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetCurPTS_0004(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0003(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0004(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0005(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0006(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetPacketCount_0007(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0003(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0004(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0005(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_GetTrickMode_0006(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0003(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0004(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0005(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_ModifyStream_0006(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0003(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0004(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0005(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0006(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0007(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0008(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0009(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0010(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0011(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0012(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0013(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0014(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0015(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0016(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0017(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0018(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0019(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0020(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0021(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0022(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0023(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0024(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0025(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0026(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_LIVE_0027(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0003(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0004(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0005(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0006(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0007(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0008(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_SWITCH_0009(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_Clear_0001(void);

CSUDI_BOOL CSTC_LIVE_TEST_IT_Program_0001(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_Program_0002(void);
CSUDI_BOOL CSTC_LIVE_TEST_IT_Program_0003(void);

#ifdef __cplusplus
}
#endif

#endif

