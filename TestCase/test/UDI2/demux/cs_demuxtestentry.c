#include "cs_testkit.h"
#include "cs_demuxtestentry.h"

//#include "sub/cs_subtestentry.h" ///下级测试用例的目录

#include "cs_demuxtestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_DEMUX_Init, CSTC_DEMUX_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCount_0001)},//CS_TK_AUTOCASE,测试过程无需干预
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCount_0002)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCapability_0001)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCapability_0002)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCapability_0003)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCapability_0004)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0001)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0002)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0003)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0004)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0005)},
    {CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0006)},
    {CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0007)},
    {CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0008)},       
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0009)}, 
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0010)}, 
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetFreeFilterCount_0001)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetFreeFilterCount_0002)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetFreeFilterCount_0003)},
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSDEMUXTESTGetObj,	//对应自己，必须是第一个，必须要有
	//CSSUBTESTGetObj,	//下级测试用例目录入口，可以没有

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(DEMUX)
/*end don't change*/

