#include "cs_testkit.h"
#include "cs_bootshowtestentry.h"

//#include "sub/cs_subtestentry.h" ///下级测试用例的目录

#include "cs_bootshowtestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_BOOTSHOW_Init, CSTC_BOOTSHOW_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
//	{CS_TK_AUTOCASE(CSTC_BOOTSHOW_MT_0001)},//CS_TK_AUTOCASE,测试过程无需干预
//	{CS_TK_MANUALCASE(CSTC_BOOTSHOW_MT_0002)},//CS_TK_MANUALCASE，测试过程需与测试者交互
	{CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetUnitCount_0001)},
	{CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetUnitCount_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetCapability_0001)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetCapability_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetShowUnitInfo_0001)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetShowUnitInfo_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetShowUnitInfo_0003)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetFreeSize_0001)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetFreeSize_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetFreeSize_0003)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0001)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0003)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0004)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0005)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0006)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0007)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0008)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0009)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0010)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0011)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0012)},
    {CS_TK_MANUALCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0013)},
    {CS_TK_MANUALCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0014)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0015)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetNeedShow_0001)},
    {CS_TK_MANUALCASE(CSTC_BOOTSHOW_SetNeedShow_0002)},  
    {CS_TK_MANUALCASE(CSTC_BOOTSHOW_SetNeedShow_0003)},
  	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSBOOTSHOWTESTGetObj,	//对应自己，必须是第一个，必须要有
	//CSSUBTESTGetObj,	//下级测试用例目录入口，可以没有

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(BOOTSHOW)
/*end don't change*/

