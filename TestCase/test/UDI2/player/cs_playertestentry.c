#include "cs_testkit.h"
#include "cs_playertestentry.h"
#include "cs_playertestcase.h"
#include "live/cs_livetestentry.h"
#include "inj/cs_injtestentry.h"
#include "file/cs_filetestentry.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_PLAYER_Init, CSTC_PLAYER_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_PLAYER_MT_0001)},//CS_TK_AUTOCASE,测试过程无需干预
	{CS_TK_MANUALCASE(CSTC_PLAYER_MT_0002)},//CS_TK_MANUALCASE，测试过程需与测试者交互
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSPLAYERTESTGetObj,	//对应自己，必须是第一个，必须要有
	CSLIVETESTGetObj,	//下级测试用例目录入口，可以没有
	CSINJTESTGetObj,
	CSFILETESTGetObj
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(PLAYER)
/*end don't change*/

