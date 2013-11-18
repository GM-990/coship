#include "cs_testkit.h"
#include "cs_toolsettestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_toolsettestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_TOOLSET_Init, CSTC_TOOLSET_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	//CS_TK_AUTOCASE,���Թ��������Ԥ
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
	
	//CS_TK_MANUALCASE�����Թ�����������߽���
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSTOOLSETTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(TOOLSET)
/*end don't change*/

