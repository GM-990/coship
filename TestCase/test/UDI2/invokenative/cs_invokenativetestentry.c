#include "cs_testkit.h"
#include "cs_invokenativetestentry.h"
#include "cs_invokenativetestcase.h"

static CSTestInit_S s_sTestInit = {CSTC_INVOKENATIVE_Init, CSTC_INVOKENATIVE_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0001)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0002)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0003)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0004)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0005)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0006)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0007)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0008)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0009)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0010)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0011)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0012)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0013)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0014)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0015)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0016)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0017)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0018)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0019)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0020)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0021)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0022)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0023)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0024)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0025)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0026)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0027)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0028)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0029)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0030)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0031)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0032)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0033)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0034)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0035)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0036)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0037)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0038)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0039)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0040)},
	{CS_TK_AUTOCASE(CSTC_INVOKENATIVE_MT_0041)},
	{"", NULL, 0} //测试用例结束标记，必须要有，且在最后一个
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSINVOKENATIVETESTGetObj //对应自己，必须是第一个，必须要有
};

CS_TEST_BUILD_FRAME_CODE(INVOKENATIVE)

