#include "cs_testkit.h"
#include "cs_injtestentry.h"
#include "cs_injtestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_INJ_Init, CSTC_INJ_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_INJ_MT_0001)},//CS_TK_AUTOCASE,测试过程无需干预
	{CS_TK_MANUALCASE(CSTC_INJ_MT_0002)},//CS_TK_MANUALCASE，测试过程需与测试者交互
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSINJTESTGetObj,	//对应自己，必须是第一个，必须要有
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(INJ)
/*end don't change*/

