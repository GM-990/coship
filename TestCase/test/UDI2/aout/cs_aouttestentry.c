#include "cs_testkit.h"
#include "cs_aouttestentry.h"

//#include "sub/cs_subtestentry.h" ///下级测试用例的目录

#include "cs_aouttestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {CSUDI_NULL, CSUDI_NULL};
static CSTestInit_S s_sTestInit = {CSTC_AOUT_Init, CSTC_AOUT_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
    //{CS_TK_AUTOCASE(CSTC_AOUT_MT_0001)},//CS_TK_AUTOCASE,测试过程无需干预
    //{CS_TK_MANUALCASE(CSTC_AOUT_MT_0002)},//CS_TK_MANUALCASE，测试过程需与测试者交互
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTMute_0001)},
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTMute_0002)},
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTMute_0003)},
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTMute_0004)},
    {CS_TK_AUTOCASE(CSTC_AOUT_TEST_IT_AOUTMute_0005)},
    #if 0
    {CS_TK_AUTOCASE(CSTC_AOUT_TEST_IT_AOUTMute_0006)},
    #endif
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTUnmute_0001)},
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTUnmute_0002)},
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTUnmute_0003)},
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTUnmute_0004)},
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTUnmute_0005)},
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTUnmute_0006)},
    {CS_TK_MANUALCASE(CSTC_AOUT_TEST_IT_AOUTUnmute_0007)},
    {CS_TK_AUTOCASE(CSTC_AOUT_TEST_IT_AOUTUnmute_0008)},
    {CS_TK_MANUALCASE(CSTC_AOUT_SetOPMode_0001)},
    {CS_TK_AUTOCASE(CSTC_AOUT_SetOPMode_0002)},
    {CS_TK_AUTOCASE(CSTC_AOUT_SetOPMode_0003)},
    {CS_TK_MANUALCASE(CSTC_AOUT_SetOPMode_0004)},
    {CS_TK_MANUALCASE(CSTC_AOUT_SetOPMode_0005)},
    {CS_TK_MANUALCASE(CSTC_AOUT_SetOPMode_0006)},
    CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
    CSAOUTTESTGetObj,   //对应自己，必须是第一个，必须要有
    //CSSUBTESTGetObj,  //下级测试用例目录入口，可以没有

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(AOUT)
/*end don't change*/

