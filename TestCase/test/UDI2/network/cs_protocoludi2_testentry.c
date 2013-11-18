#include "cs_testkit.h"
#include "cs_protocoludi2_testentry.h"
#include "ipcfg/cs_ipcfgtestentry.h" ///下级测试用例的目录
#include "socket/cs_sockettestentry.h"
#include "dhcp/cs_dhcptestentry.h"
#include "cs_protocoludi2_testcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_CS_PROTOCOLUDI2_TEST_Init, CSTC_CS_PROTOCOLUDI2_TEST_UnInit};
static CSTestCase_S s_asTestCase[] = 
{	
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSPROTOCOLUDI2TESTGetObj,	//对应自己，必须是第一个，必须要有
	CSIPCFGTESTGetObj,//CSSUBTESTGetObj,	//下级测试用例目录入口，可以没有
	CSSOCKETTESTGetObj,
	CSDHCPTESTGetObj,
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(PROTOCOLUDI2)
/*end don't change*/

