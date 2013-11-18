#include "cs_testkit.h"
#include "cs_inputtestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_inputtestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_INPUT_Init, CSTC_INPUT_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0001)},
	{CS_TK_AUTOCASE(CSTC_INPUT_IT_AddCallback_0002)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0003)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0004)},
	//CSTC_INPUT_IT_AddCallback_0005��CSTC_INPUT_IT_AddCallback_0003���������ظ�
	/*{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0005)},*/
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0006)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0007)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0008)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0009)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0010)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0011)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0012)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0013)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0014)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0015)},
	/*{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0016)},*/
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0017)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_AddCallback_0018)},
	{CS_TK_MANUALCASE(CSTC_INPUT_IT_DelCallback_0001)},
	{CS_TK_AUTOCASE(CSTC_INPUT_IT_DelCallback_0002)},
	{CS_TK_AUTOCASE(CSTC_INPUT_IT_DelCallback_0003)},
	{CS_TK_AUTOCASE(CSTC_INPUT_IT_DelCallback_0004)},

	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSINPUTTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(INPUT)
/*end don't change*/

