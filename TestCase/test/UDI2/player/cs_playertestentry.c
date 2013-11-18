#include "cs_testkit.h"
#include "cs_playertestentry.h"
#include "cs_playertestcase.h"
#include "live/cs_livetestentry.h"
#include "inj/cs_injtestentry.h"
#include "file/cs_filetestentry.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_PLAYER_Init, CSTC_PLAYER_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_PLAYER_MT_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
	{CS_TK_MANUALCASE(CSTC_PLAYER_MT_0002)},//CS_TK_MANUALCASE�����Թ�����������߽���
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSPLAYERTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	CSLIVETESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��
	CSINJTESTGetObj,
	CSFILETESTGetObj
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(PLAYER)
/*end don't change*/

