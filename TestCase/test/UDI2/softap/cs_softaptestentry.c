#include "cs_testkit.h"
#include "version.h"
#include "cs_softaptestentry.h"
#include "ap/cs_aptestentry.h"
#include "dhcps/cs_dhcpstestentry.h"
#include "router/cs_routertestentry.h"
#include "cs_softaptestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_SOFTAP_Init, CSTC_SOFTAP_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSSOFTAPTESTGetObj,	//对应自己，必须是第一个，必须要有
	CSDHCPSTESTGetObj,	//下级测试用例目录入口，可以没有
	CSROUTERTESTGetObj,
	CSAPTESTGetObj
};
/*end change*/
CS_TEST_BUILD_FRAME_CODE(SOFTAP)
/*end don't change*/

