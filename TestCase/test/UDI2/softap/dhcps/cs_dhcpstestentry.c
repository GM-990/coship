#include "cs_testkit.h"
#include "cs_dhcpstestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_dhcpstestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_DHCPS_Init, CSTC_DHCPS_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSStartStop_0001)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSStartStop_0002)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSStartStop_0003)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSStartStop_0004)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSConfigGetConfig_0001)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSConfigGetConfig_0002)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSConfigGetConfig_0003)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0001)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0002)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0003)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDICSUDIDHCPSAddDelGetStaticIPCfg_0004)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSGetLeaseInfo_0001)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSGetLeaseInfo_0002)},
	{CS_TK_MANUALCASE(CSTC_DHCPS_CSUDIDHCPSGetLeaseInfo_0003)},	

	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSDHCPSTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(DHCPS)
/*end don't change*/

