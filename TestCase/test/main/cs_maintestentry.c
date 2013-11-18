#include "cs_testkit.h"
#include "cs_maintestentry.h"

#define TEST_DTV
#define TEST_CODEC
#define TEST_PVR
#define TEST_PROTOCOL
#define TEST_MEDIAPLAYER
#define TEST_KERNEL 

#ifdef TEST_DTV
#include "cs_dtvtestentry.h"
#endif
#ifdef TEST_CODEC
#include "cs_codectestentry.h"
#endif
#ifdef TEST_PVR
#include "cs_pvrtestentry.h"
#endif
#ifdef TEST_MEDIAPLAYER
#include "cs_mediaplayertestentry.h"
#endif
#ifdef TEST_KERNEL
#include "cs_kerneltestentry.h"
#endif
#ifdef TEST_PROTOCOL
#include "cs_protocoltestentry.h"
#endif
//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

//#include "cs_cfstestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {NULL, NULL};
//static CSTestInit_S s_sTestInit = {CSTC_KERNEL_Init, CSTC_KERNEL_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	//{CS_TK_AUTOCASE(CSTC_KERNEL_MT_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
	//{CS_TK_MANUALCASE(CSTC_KERNEL_MT_0002)},//CS_TK_MANUALCASE�����Թ�����������߽���
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSMAINTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
#ifdef TEST_DTV
	CSDTVTESTGetObj,
#endif
#ifdef TEST_CODEC
	CSCODECTESTGetObj,
#endif
#ifdef TEST_PVR
	CSPVRTESTGetObj,
#endif
#ifdef TEST_MEDIAPLAYER
	CSMEDIAPLAYERTESTGetObj,
#endif
#ifdef TEST_KERNEL
	CSKERNELTESTGetObj,
#endif
#ifdef TEST_PROTOCOL
	CSPROTOCOLTESTGetObj,
#endif
};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(MAIN)
/*end don't change*/

