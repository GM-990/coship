#include "cs_testkit.h"
#include "cs_paneltestentry.h"
#include "cs_paneltestcase.h"

/*begin change*/
static CSTestInit_S s_sTestInit = {CSTC_PANEL_Init, CSTC_PANEL_UnInit};
static CSTestCase_S s_asTestCase[] = 
 
{
	{CS_TK_AUTOCASE(CSTC_PANEL_IT_GetCapabilityByName_0001)},
	{CS_TK_AUTOCASE(CSTC_PANEL_IT_GetCapabilityByName_0002)},
	{CS_TK_AUTOCASE(CSTC_PANEL_IT_GetCapabilityByName_0003)},
	{CS_TK_AUTOCASE(CSTC_PANEL_IT_GetCapabilityByName_0004)},
	{CS_TK_AUTOCASE(CSTC_PANEL_IT_GetCapabilityByName_0005)},

	{CS_TK_MANUALCASE( CSTC_PANEL_IT_PanelDisplay_0001)},
	{CS_TK_MANUALCASE( CSTC_PANEL_IT_PanelDisplay_0002)},
	{CS_TK_MANUALCASE( CSTC_PANEL_IT_PanelDisplay_0003)},
	{CS_TK_MANUALCASE( CSTC_PANEL_IT_PanelDisplay_0004)},
	{CS_TK_AUTOCASE( CSTC_PANEL_IT_PanelDisplay_0005)},
	{CS_TK_AUTOCASE( CSTC_PANEL_IT_PanelDisplay_0006)},
	{CS_TK_AUTOCASE( CSTC_PANEL_IT_PanelDisplay_0007)},
	{CS_TK_AUTOCASE( CSTC_PANEL_IT_PanelDisplay_0008)},
	{CS_TK_MANUALCASE( CSTC_PANEL_IT_PanelDisplay_0009)},
	{CS_TK_MANUALCASE( CSTC_PANEL_IT_PanelDisplay_0010)},
	{CS_TK_AUTOCASE( CSTC_PANEL_IT_PanelDisplay_0011)},
	{CS_TK_AUTOCASE( CSTC_PANEL_IT_PanelDisplay_0012)},
	{CS_TK_AUTOCASE( CSTC_PANEL_IT_PanelDisplay_0013)},	
    {CS_TK_MANUALCASE( CSTC_PANEL_IT_PanelDisplay_0014)},

	{CS_TK_MANUALCASE(CSTC_PANEL_IT_PanelClear_0001)},  
	{CS_TK_AUTOCASE(CSTC_PANEL_IT_PanelClear_0002)},  
	{CS_TK_AUTOCASE(CSTC_PANEL_IT_PanelClear_0003)},  
	{CS_TK_AUTOCASE(CSTC_PANEL_IT_PanelClear_0004)},  
	{CS_TK_MANUALCASE(CSTC_PANEL_IT_PanelClear_0005)},  
	{CS_TK_MANUALCASE(CSTC_PANEL_IT_PanelClear_0006)},  
	{CS_TK_AUTOCASE(CSTC_PANEL_IT_PanelClear_0007)},  
	{CS_TK_MANUALCASE(CSTC_PANEL_IT_PanelClear_0008)}, 

	CS_TK_ENDCASE //��������������ǣ�����Ҫ�У��������һ��
};

static CSTestGetObj_F s_afnTestGetObj[]= 
{
	CSPANELTESTGetObj,	//��Ӧ�Լ��������ǵ�һ��������Ҫ��
	//CSSUBTESTGetObj,	//�¼���������Ŀ¼��ڣ�����û��

};
/*end change*/

CS_TEST_BUILD_FRAME_CODE(PANEL)
/*end don't change*/

