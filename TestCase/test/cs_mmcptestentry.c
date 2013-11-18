#include "cs_testkit.h"
#include "version.h"
#include "cs_mmcptestentry.h"
//#include "dtv/cs_dtvtestentry.h"
//#include "graph/cs_graphtestentry.h"
//#include "graphics/cs_graphicstestentry.h"
//#include "guitool/cs_guitooltestentry.h"
#include "kernel/cs_kerneltestentry.h"
#include "mediaplayer/cs_mediaplayertestentry.h"
//#include "porting/cs_portingtestentry.h"
//#include "udi/cs_uditestentry.h"
//#include "protocol/cs_protocoltestentry.h"
//#include "pvr/cs_pvrtestentry.h"
//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

//#include "cs_mmcptestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {NULL, NULL};
//static CSTestInit_S s_sTestInit = {CSTC_MMCP_Init, CSTC_MMCP_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	//{CS_TK_AUTOCASE(CSTC_MMCP_MT_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
	//{CS_TK_MANUALCASE(CSTC_MMCP_MT_0002)},//CS_TK_MANUALCASE�����Թ�����������߽���
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
    CSMMCPTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ���¼���������Ŀ¼��ڣ�����û��
    //CSGUITOOLTESTGetObj,
    //CSGRAPHTESTGetObj, 
    CSKERNELTESTGetObj,
    CSMEDIAPLAYERTESTGetObj,
    //CSUDITESTGetObj,
    //CSPVRTESTGetObj,
    //CSDTVTESTGetObj,
  //CSGRAPHICSTESTGetObj,
  //CSPROTOCOLTESTGetObj,
  //CSPORTINGTESTGetObj,
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(MMCP)
/*end don't change*/

