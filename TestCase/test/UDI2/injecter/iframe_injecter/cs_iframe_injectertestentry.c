#include "cs_testkit.h"
#include "cs_iframe_injectertestentry.h"
#include "cs_iframe_injectertestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_IFRAME_INJECTER_Init, CSTC_IFRAME_INJECTER_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_IFRAME_INJECTER_OpenClose_001)},
	{CS_TK_AUTOCASE(CSTC_IFRAME_INJECTER_OpenClose_002)},
	{CS_TK_AUTOCASE(CSTC_IFRAME_INJECTER_OpenClose_003)},
	{CS_TK_AUTOCASE(CSTC_IFRAME_INJECTER_SetGetAttribute_001)},
	{CS_TK_AUTOCASE(CSTC_IFRAME_INJECTER_SetGetAttribute_002)},
	{CS_TK_AUTOCASE(CSTC_IFRAME_INJECTER_StartStop_001)},
	{CS_TK_MANUALCASE(CSTC_IFRAME_INJECTER_001)},
	{CS_TK_AUTOCASE(CSTC_IFRAME_INJECTER_002)},
	{CS_TK_MANUALCASE(CSTC_IFRAME_INJECTER_003)},
	{CS_TK_MANUALCASE(CSTC_IFRAME_INJECTER_DATA_480I)},	
	{CS_TK_MANUALCASE(CSTC_IFRAME_INJECTER_DATA_576I)},
	{CS_TK_MANUALCASE(CSTC_IFRAME_INJECTER_DATA_720P)},	
	{CS_TK_MANUALCASE(CSTC_IFRAME_INJECTER_DATA_1080I)},
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSIFRAME_INJECTERTESTGetObj,	//对应自己，必须是第一个，必须要有
	//CSPCM_INJECTERTESTGetObj,	    //下级测试用例目录入口，可以没有
	//CSTS_INJECTERTESTGetObj,
	//CSPESES_INJECTERTESTGetObj
};
/*end change*/
CS_TEST_BUILD_FRAME_CODE(IFRAME_INJECTER)
/*end don't change*/

