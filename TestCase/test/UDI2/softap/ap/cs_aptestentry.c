#include "cs_testkit.h"
#include "cs_aptestentry.h"

//#include "sub/cs_subtestentry.h" ///下级测试用例的目录

#include "cs_aptestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_AP_Init, CSTC_AP_UnInit};

static CSTestCase_S s_asTestCase[] = 
{
        {CS_TK_AUTOCASE(CSTC_AP_SetSignal_0001)},//CS_TK_AUTOCASE,测试过程无需干预
        {CS_TK_MANUALCASE(CSTC_AP_SetSignal_0002)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSignal_0003)},

        {CS_TK_AUTOCASE(CSTC_AP_SetFreqChannel_0001)},
        {CS_TK_MANUALCASE(CSTC_AP_SetFreqChannel_0002)},
            
        {CS_TK_AUTOCASE(CSTC_AP_SetSSIDSeparate_0001)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDSeparate_0002)},

        {CS_TK_AUTOCASE(CSTC_AP_GetConfig_0001)},
        {CS_TK_MANUALCASE(CSTC_AP_GetConfig_0002)},
            
        {CS_TK_AUTOCASE(CSTC_AP_GetSSIDCapblitity_0001)},
        {CS_TK_AUTOCASE(CSTC_AP_GetSSIDCapblitity_0002)},

        {CS_TK_AUTOCASE(CSTC_AP_GetSSIDs_0001)},
        {CS_TK_AUTOCASE(CSTC_AP_GetSSIDs_0002)},
        {CS_TK_AUTOCASE(CSTC_AP_GetSSIDs_0003)},
            
        {CS_TK_AUTOCASE(CSTC_AP_SetSSIDs_0001)},
        {CS_TK_AUTOCASE(CSTC_AP_SetSSIDs_0002)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0003)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0004)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0005)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0006)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0007)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0008)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0009)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0010)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0011)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0012)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0013)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0014)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0015)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0016)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0017)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0018)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0019)},
        {CS_TK_MANUALCASE(CSTC_AP_SetSSIDs_0020)},

        {CS_TK_AUTOCASE(CSTC_AP_GetStationsInfo_0001)},
        {CS_TK_MANUALCASE(CSTC_AP_GetStationsInfo_0002)},
        {CS_TK_MANUALCASE(CSTC_AP_GetStationsInfo_0003)},
        {CS_TK_MANUALCASE(CSTC_AP_GetStationsInfo_0004)},
        {CS_TK_MANUALCASE(CSTC_AP_GetStationsInfo_0005)},

        {CS_TK_AUTOCASE(CSTC_AP_SetGetAdvancedInfo_0001)},
        {CS_TK_AUTOCASE(CSTC_AP_SetGetAdvancedInfo_0002)},
        {CS_TK_MANUALCASE(CSTC_AP_SetGetAdvancedInfo_0003)},
        {CS_TK_MANUALCASE(CSTC_AP_SetGetAdvancedInfo_0004)},
        {CS_TK_MANUALCASE(CSTC_AP_SetGetAdvancedInfo_0005)},
        {CS_TK_MANUALCASE(CSTC_AP_SetGetAdvancedInfo_0006)},
        {CS_TK_MANUALCASE(CSTC_AP_SetGetAdvancedInfo_0007)},
        {CS_TK_MANUALCASE(CSTC_AP_SetGetAdvancedInfo_0008)},
            
        CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个

};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSAPTESTGetObj,	//对应自己，必须是第一个，必须要有
	//CSSUBTESTGetObj,	//下级测试用例目录入口，可以没有

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(AP)
/*end don't change*/

