#include "cs_testkit.h"
#include "cs_otatestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_otatestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_OTA_Init, CSTC_OTA_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_OTA_IT_GetInfo_0001)},
	{CS_TK_AUTOCASE(CSTC_OTA_IT_SetInfo_0001)},
	{CS_TK_AUTOCASE(CSTC_OTA_IT_SetOta_0001)},
	{CS_TK_AUTOCASE(CSTC_OTA_IT_WIFIAddDelCallback_0001)},
	{CS_TK_AUTOCASE(CSTC_OTA_IT_WIFIAddDelCallback_0002)},
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSOTATESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(OTA)
/*end don't change*/

