#include "cs_testkit.h"
#include "cs_performancetestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_performancetestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_PERFORMANCE_Init, CSTC_PERFORMANCE_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_PERFORMANCE_MT_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
	{CS_TK_AUTOCASE(CSTC_PERFORMANCE_MT_0002)},
	{CS_TK_AUTOCASE(CSTC_PERFORMANCE_MT_0003)},
	{CS_TK_AUTOCASE(CSTC_PERFORMANCE_MT_0004)},
	//{CS_TK_MANUALCASE(CSTC_PERFORMANCE_MT_0002)},//CS_TK_MANUALCASE�����Թ�����������߽���
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSPERFORMANCETESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(PERFORMANCE)
/*end don't change*/

