#include "cs_testkit.h"
#include "cs_aouttestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_aouttestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {CSUDI_NULL, CSUDI_NULL};
static CSTestInit_S s_sTestInit = {CSTC_AOUT_Init, CSTC_AOUT_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
    //{CS_TK_AUTOCASE(CSTC_AOUT_MT_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
    //{CS_TK_MANUALCASE(CSTC_AOUT_MT_0002)},//CS_TK_MANUALCASE�����Թ�����������߽���
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
    CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
    CSAOUTTESTGetObj,   //��Ӧ�Լ��������ǵ�һ��������Ҫ��
    //CSSUBTESTGetObj,  //�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(AOUT)
/*end don't change*/

