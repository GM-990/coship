#include "cs_testkit.h"
#include "cs_otatestentry.h"

//#include "sub/cs_subtestentry.h" ///下级测试用例的目录

#include "cs_otatestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_OTA_Init, CSTC_OTA_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_OTA_IT_GetInfo_0001)},
	{CS_TK_AUTOCASE(CSTC_OTA_IT_SetInfo_0001)},
	{CS_TK_AUTOCASE(CSTC_OTA_IT_SetOta_0001)},
	{CS_TK_AUTOCASE(CSTC_OTA_IT_WIFIAddDelCallback_0001)},
	{CS_TK_AUTOCASE(CSTC_OTA_IT_WIFIAddDelCallback_0002)},
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSOTATESTGetObj,	//对应自己，必须是第一个，必须要有
	//CSSUBTESTGetObj,	//下级测试用例目录入口，可以没有

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(OTA)
/*end don't change*/

