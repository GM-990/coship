#include "cs_testkit.h"
#include "cs_bootshowtestentry.h"

//#include "sub/cs_subtestentry.h" ///�¼�����������Ŀ¼

#include "cs_bootshowtestcase.h"

/*begin change*/
//static CSTestInit_S s_sTestInit = {NULL, NULL};
static CSTestInit_S s_sTestInit = {CSTC_BOOTSHOW_Init, CSTC_BOOTSHOW_UnInit};
static CSTestCase_S s_asTestCase[] = 
{
//	{CS_TK_AUTOCASE(CSTC_BOOTSHOW_MT_0001)},//CS_TK_AUTOCASE,���Թ��������Ԥ
//	{CS_TK_MANUALCASE(CSTC_BOOTSHOW_MT_0002)},//CS_TK_MANUALCASE�����Թ�����������߽���
	{CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetUnitCount_0001)},
	{CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetUnitCount_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetCapability_0001)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetCapability_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetShowUnitInfo_0001)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetShowUnitInfo_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetShowUnitInfo_0003)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetFreeSize_0001)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetFreeSize_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_GetFreeSize_0003)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0001)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0002)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0003)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0004)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0005)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0006)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0007)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0008)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0009)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0010)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0011)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0012)},
    {CS_TK_MANUALCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0013)},
    {CS_TK_MANUALCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0014)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetShowUnitInfo_0015)},
    {CS_TK_AUTOCASE(CSTC_BOOTSHOW_SetNeedShow_0001)},
    {CS_TK_MANUALCASE(CSTC_BOOTSHOW_SetNeedShow_0002)},  
    {CS_TK_MANUALCASE(CSTC_BOOTSHOW_SetNeedShow_0003)},
  	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSBOOTSHOWTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(BOOTSHOW)
/*end don't change*/

