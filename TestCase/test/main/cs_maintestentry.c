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
//#include "sub/cs_subtestentry.h" ///下级测试用例的目录

//#include "cs_cfstestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {NULL, NULL};
//static CSTestInit_S s_sTestInit = {CSTC_KERNEL_Init, CSTC_KERNEL_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	//{CS_TK_AUTOCASE(CSTC_KERNEL_MT_0001)},//CS_TK_AUTOCASE,测试过程无需干预
	//{CS_TK_MANUALCASE(CSTC_KERNEL_MT_0002)},//CS_TK_MANUALCASE，测试过程需与测试者交互
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSMAINTESTGetObj,	//对应自己，必须是第一个，必须要有
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

