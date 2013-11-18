#include "cs_testkit.h"
#include "cs_audiotestentry.h"

//#include "sub/cs_subtestentry.h" ///下级测试用例的目录

#include "cs_audiotestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {CSUDI_NULL, CSUDI_NULL};
static CSTestInit_S s_sTestInit = {CSTC_AUDIO_Init, CSTC_AUDIO_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_AUDIO_GetCount_0001)},//CS_TK_AUTOCASE,测试过程无需干预
	{CS_TK_AUTOCASE(CSTC_AUDIO_GetCount_0002)},//CS_TK_MANUALCASE，测试过程需与测试者交互
	{CS_TK_AUTOCASE(CSTC_AUDIO_GetCapability_0001)},
	{CS_TK_AUTOCASE(CSTC_AUDIO_GetCapability_0002)},
	{CS_TK_AUTOCASE(CSTC_AUDIO_GetCapability_0003)},
	{CS_TK_AUTOCASE(CSTC_AUDIO_GetStatus_0001)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_GetStatus_0002)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_GetStatus_0003)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_GetStatus_0004)},
	{CS_TK_AUTOCASE(CSTC_AUDIO_SetChannel_0001)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_SetChannel_0002)},
	{CS_TK_AUTOCASE(CSTC_AUDIO_Mute_0001)},
	{CS_TK_AUTOCASE(CSTC_AUDIO_Unmute_0001)},
	{CS_TK_AUTOCASE(CSTC_AUDIO_SetVolume_0001)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_SetVolume_0002)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_SetVolume_0003)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_SetVolume_0004)},	
	{CS_TK_MANUALCASE(CSTC_AUDIO_MuteAndUnmute_0001)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_IT_0001)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_IT_0002)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_IT_0003)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_IT_0004)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_IT_0005)},
	{CS_TK_MANUALCASE(CSTC_AUDIO_IT_0006)},	
	{CS_TK_MANUALCASE(CSTC_AUDIO_AOUT_IT_0001)},		
	{CS_TK_MANUALCASE(CSTC_AUDIO_AOUT_IT_0002)}, 	
	CS_TK_ENDCASE //测试用例结束标记，必须要有，且在最后一个
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSAUDIOTESTGetObj,	//对应自己，必须是第一个，必须要有
	//CSSUBTESTGetObj,	//下级测试用例目录入口，可以没有

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(AUDIO)
/*end don't change*/

