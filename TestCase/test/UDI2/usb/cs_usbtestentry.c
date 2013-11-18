#include "cs_testkit.h"
#include "cs_usbtestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_usbtestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_USB_Init, CSTC_USB_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_MANUALCASE(CSTC_USB_IT_GetCount_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
	{CS_TK_AUTOCASE(CSTC_USB_IT_GetCount_0002)},
	{CS_TK_MANUALCASE(CSTC_USB_IT_GetInfo_0001)},
	{CS_TK_MANUALCASE(CSTC_USB_IT_GetInfo_0002)},
	//{CS_TK_AUTOCASE(CSTC_USB_IT_GetInfo_0003)},
	/*{CS_TK_MANUALCASE(CSTC_USB_MT_0002)},*///CS_TK_MANUALCASE�����Թ�����������߽���
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSUSBTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(USB)
/*end don't change*/

