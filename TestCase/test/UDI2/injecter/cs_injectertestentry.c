#include "cs_testkit.h"
#include "cs_injectertestentry.h"
#include "pcm_injecter/cs_pcm_injectertestentry.h"
#include "ts_injecter/cs_ts_injectertestentry.h"
#include "peses_injecter/cs_peses_injectertestentry.h"
#include "iframe_injecter/cs_iframe_injectertestentry.h"
#include "cs_injectertestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_INJECTER_Init, CSTC_INJECTER_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSINJECTERTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	CSPCM_INJECTERTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��
	CSTS_INJECTERTESTGetObj,
	CSPESES_INJECTERTESTGetObj,
	CSIFRAME_INJECTERTESTGetObj
};
/*end change*/
CS_TEST_BUILD_FRAME_CODE(INJECTER)
/*end don't change*/

