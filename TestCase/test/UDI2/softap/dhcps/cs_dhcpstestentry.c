#include "cs_testkit.h"
#include "cs_dhcpstestentry.h"

//#include "sub/cs_subtestentry.h" ///下级测试用例的目录

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

	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSDHCPSTESTGetObj,	//对应自己，必须是第一个，必须要有
	//CSSUBTESTGetObj,	//下级测试用例目录入口，可以没有

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(DHCPS)
/*end don't change*/

