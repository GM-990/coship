#include "cs_testkit.h"
#include "cs_demuxtestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_demuxtestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_DEMUX_Init, CSTC_DEMUX_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCount_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCount_0002)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCapability_0001)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCapability_0002)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCapability_0003)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetCapability_0004)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0001)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0002)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0003)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0004)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0005)},
    {CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0006)},
    {CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0007)},
    {CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0008)},       
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0009)}, 
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_ConnectTuner_0010)}, 
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetFreeFilterCount_0001)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetFreeFilterCount_0002)},
	{CS_TK_AUTOCASE(CSTC_DEMUX_IT_GetFreeFilterCount_0003)},
	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};


static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSDEMUXTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(DEMUX)
/*end don't change*/

