#include "cs_testkit.h"
#include "cs_injtestentry.h"
#include "cs_injtestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_INJ_Init, CSTC_INJ_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_INJ_MT_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
	{CS_TK_MANUALCASE(CSTC_INJ_MT_0002)},//CS_TK_MANUALCASE�����Թ�����������߽���
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSINJTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(INJ)
/*end don't change*/

