#include "cs_testkit.h"
#include "cs_hdmitestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_hdmitestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {CSUDI_NULL, CSUDI_NULL};
static CSTestInit_S s_sTestInit = {CSTC_HDMI_IT_Init, CSTC_HDMI_IT_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIGetCount)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIGetSinkCapabilities_001)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIGetSinkCapabilities_002)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIGetSinkCapabilities_003)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMISetGetProperty_001)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMISetProperty_001)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMISetProperty_002)},
	{CS_TK_MANUALCASE(CSTC_HDMI_IT_CSUDIHDMIAddCallback_001)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIAddCallback_002)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIAddCallback_003)},
	{CS_TK_MANUALCASE(CSTC_HDMI_IT_CSUDIHDMIDelCallback_001)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIDelCallback_002)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIDelCallback_003)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIDelCallback_004)},
	{CS_TK_AUTOCASE(CSTC_HDMI_IT_CSUDIHDMIGetProperty_001)},
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSHDMITESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(HDMI)
/*end don't change*/

