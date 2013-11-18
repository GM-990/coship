#include "cs_testkit.h"
#include "cs_audiotestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_audiotestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {CSUDI_NULL, CSUDI_NULL};
static CSTestInit_S s_sTestInit = {CSTC_AUDIO_Init, CSTC_AUDIO_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_AUDIO_GetCount_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
	{CS_TK_AUTOCASE(CSTC_AUDIO_GetCount_0002)},//CS_TK_MANUALCASE�����Թ�����������߽���
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
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSAUDIOTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(AUDIO)
/*end don't change*/

