#include "cs_testkit.h"
#include "cs_dhcptestentry.h"
//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_dhcptestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_DHCP_Init, CSTC_DHCP_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_MANUALCASE(CSTC_DHCP_IT_0001)},

	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSDHCPTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(DHCP)
/*end don't change*/

