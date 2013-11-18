#include "cs_testkit.h"
#include "cs_routertestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_routertestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_ROUTER_Init, CSTC_ROUTER_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTSetGetInterfaces_0001)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTSetGetInterfaces_0002)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTSetGetInterfaces_0003)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTSetLanIp_0001)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTSetLanIp_0002)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTStartStopService_0001)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTStartStopService_0002)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTStartStopService_0003)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTStartStopService_0004)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTStartStopService_0005)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTStartStopService_0006)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTGetServiceStatus_0001)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTGetServiceStatus_0002)},
	{CS_TK_MANUALCASE(CSTC_ROUTER_CSUDIRTExecuteIptableCmd_0001)},	

	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSROUTERTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(ROUTER)
/*end don't change*/

