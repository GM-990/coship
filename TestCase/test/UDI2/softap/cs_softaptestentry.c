#include "cs_testkit.h"
#include "version.h"
#include "cs_softaptestentry.h"
#include "ap/cs_aptestentry.h"
#include "dhcps/cs_dhcpstestentry.h"
#include "router/cs_routertestentry.h"
#include "cs_softaptestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_SOFTAP_Init, CSTC_SOFTAP_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSSOFTAPTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	CSDHCPSTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��
	CSROUTERTESTGetObj,
	CSAPTESTGetObj
};
/*end change*/
CS_TEST_BUILD_FRAME_CODE(SOFTAP)
/*end don't change*/

