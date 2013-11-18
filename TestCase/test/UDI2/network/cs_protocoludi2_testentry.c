#include "cs_testkit.h"
#include "cs_protocoludi2_testentry.h"
#include "ipcfg/cs_ipcfgtestentry.h" ///�¼�����������Ŀ¼
#include "socket/cs_sockettestentry.h"
#include "dhcp/cs_dhcptestentry.h"
#include "cs_protocoludi2_testcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_CS_PROTOCOLUDI2_TEST_Init, CSTC_CS_PROTOCOLUDI2_TEST_UnInit};
static CSTestCase_S s_asTestCase[] = 
{	
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSPROTOCOLUDI2TESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	CSIPCFGTESTGetObj,//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��
	CSSOCKETTESTGetObj,
	CSDHCPTESTGetObj,
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(PROTOCOLUDI2)
/*end don't change*/

