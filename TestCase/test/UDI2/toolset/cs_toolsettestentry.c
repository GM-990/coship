#include "cs_testkit.h"
#include "cs_toolsettestentry.h"

//#include "sub/cs_subtestentry.h" ///下级测试用例的目录

#include "cs_toolsettestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_TOOLSET_Init, CSTC_TOOLSET_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	//CS_TK_AUTOCASE,测试过程无需干预
	{CS_TK_AUTOCASE(CSTC_TOOLSET_IT_GetChipID_0001)},
	{CS_TK_AUTOCASE(CSTC_TOOLSET_IT_GetChipID_0002)},
	{CS_TK_AUTOCASE(CSTC_TOOLSET_IT_GetSerialKey_0001)},
	{CS_TK_AUTOCASE(CSTC_TOOLSET_IT_GetSerialKey_0002)},
	{CS_TK_MANUALCASE(CSTC_TOOLSET_IT_Reboot_0001)},
	{CS_TK_MANUALCASE(CSTC_TOOLSET_IT_Halt_0001)},
	{CS_TK_AUTOCASE(CSTC_TOOLSET_IT_Standby_0001)},
	{CS_TK_AUTOCASE(CSTC_TOOLSET_IT_Standby_0002)},
	{CS_TK_MANUALCASE(CSTC_TOOLSET_IT_Standby_0003)},
	{CS_TK_MANUALCASE(CSTC_TOOLSET_IT_Standby_0004)},
	{CS_TK_MANUALCASE(CSTC_TOOLSET_IT_Standby_0005)},
	
	//CS_TK_MANUALCASE，测试过程需与测试者交互
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSTOOLSETTESTGetObj,	//对应自己，必须是第一个，必须要有
	//CSSUBTESTGetObj,	//下级测试用例目录入口，可以没有

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(TOOLSET)
/*end don't change*/

