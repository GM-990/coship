#include "cs_testkit.h"
#include "cs_debugtestentry.h"
#include "cs_debugtestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_DEBUG_Init, CSTC_DEBUG_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0001)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0002)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0003)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0004)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0005)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0006)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0007)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0008)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0009)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0010)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0011)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0012)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0013)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0014)},
	{CS_TK_MANUALCASE(CSTC_DEBUG_IT_OutputString_0015)},

	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSDEBUGTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(DEBUG)
/*end don't change*/

