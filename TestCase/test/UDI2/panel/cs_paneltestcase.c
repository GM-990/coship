/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_paneltestcase.h"
#include "cs_panel_test.h"
#include <stdio.h>
#include <string.h>




//平台支持的面板数，从配置文件读出
extern int g_PanelCnt;

//平台支持的文本灯显示字符个数
extern int g_TextCnt;

//平台支持的组合灯子灯数量
extern int g_SuitCnt;

//平台前面板是否支持该显示灯的布尔数组
extern CSUDI_BOOL g_abSuppLight[EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART + 1];

//控制灯显示属性数组，用来存放该控制灯所具有的属性
extern CSUDI_UINT32  g_CtrlFunMask[EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART + 1];

//显示灯枚举数组，存放该显示灯的枚举值
extern CSUDIPanelLightName_E g_aeLight[EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART];

//显示灯名称数组，存放该显示灯的名称
extern char g_sLight[EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART][10];

static CSUDI_BOOL g_bThreadEnd1;

static CSUDI_BOOL g_bThreadEnd2;

#define PANEL_TEST_STACK_SIZE 4*1024

#define REPEAT_TIME 10

CSUDI_BOOL CSTC_PANEL_Init(void)
{
	//在本测试用例集执行前调用
	CSTC_PANEL_InitCfg();

	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PANEL_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//设置TEXT0文本灯的显示内容为"abcd"
void PANEL_TestThread1Entry(void *pvParam)
{
	int i;
	
	CSUDIPanelLightValule_S  sTextValue;
	
	sTextValue.m_eLightName = EM_UDIPANEL_TEXT0;
	sTextValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
	strcpy(sTextValue.m_uValue.m_sTextValue.m_szStr,"abcd");
	sTextValue.m_uValue.m_sTextValue.m_nStrCount = 4;
	//printf("\nliubing test ++++++++++++ thread 1 has run00\n");
	for(i= 0; i < 10; ++i)
	{
		//printf("\nliubing test ++++++++++++ thread 1 has run11\n");
		CSUDIPanelDisplay(0, &sTextValue) ;

		CSUDIOSThreadSleep(2000);
	}
	
	g_bThreadEnd1 = CSUDI_TRUE;

}

//设置TEXT0文本灯的显示内容为"8888"
void PANEL_TestThread2Entry(void *pvParam)
{
	int i;
	
	CSUDIPanelLightValule_S  sTextValue;
	
	sTextValue.m_eLightName = EM_UDIPANEL_TEXT0;
	sTextValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
	strcpy(sTextValue.m_uValue.m_sTextValue.m_szStr,"8888");
	sTextValue.m_uValue.m_sTextValue.m_nStrCount = 4;
	CSUDIOSThreadSleep(3000);
	//printf("\nliubing test ++++++++++++ thread 2 has run00\n");
	for(i = 0; i < 10; ++i)
	{
		//printf("\nliubing test ++++++++++++ thread 2 has run11\n");
		CSUDIPanelDisplay(0, &sTextValue) ;

		CSUDIOSThreadSleep(2000);
	}
	g_bThreadEnd2 = CSUDI_TRUE;
}



//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:测试能否正确得到显示灯的能力信息 
//@PRECONDITION:前面板已成功初始化 
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、面板显示类型合法  
//@INPUT:3、非空的CSUDIPanelLightCapability_S类型指针psLightCapability
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数，由配置文件给出
//@EXECUTIONFLOW:1、ErrCode = CSUDIPanelGetCapabilityByName(0,EM_UDIPANEL_MAIL,psLightCapability)
//@EXECUTIONFLOW:2、若配置文件指明EM_UDIPANEL_MAIL灯存在，则期望ErrCode == CSUDI_SUCCESS，psLightCapability->m_eLightName == EM_UDIPANEL_MAIL，并且psLightCapability->m_uProperty与配置文件的内容相一致。
//@EXECUTIONFLOW:3、若配置文件指明EM_UDIPANEL_MAIL灯不存在，则期望ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:4、对EM_UDIPANEL_CTRLSTART--EM_UDIPANEL_MAX的所有灯（除CSUDIPanelGetCapabilityByName接口明确声明中不得使用的枚举值外）重复以上步骤
//@EXECUTIONFLOW:5、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0001(void)
{
	CSUDIPanelLightCapability_S sLightCapability;

	CSUDI_Error_Code ErrCode;

	int i;
	
	int j;

	for(i = 0; i < g_PanelCnt; ++i)
		for(j = 1; j < EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART; ++j)
			{
				if(CSUDI_PANEL_IS_TEXT(g_aeLight[j]))
				{
					ErrCode = CSUDIPanelGetCapabilityByName(i,g_aeLight[j],&sLightCapability);

					if(CSUDI_TRUE == g_abSuppLight[j])
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第二步获取文本灯信息返回值失败");
						
						CSTK_ASSERT_TRUE_FATAL(g_aeLight[j] == sLightCapability.m_eLightName,"第二步获取文本灯信息内容失败");
						
						CSTK_ASSERT_TRUE_FATAL(g_TextCnt == sLightCapability.m_uProperty.m_sText.m_nTextCount,"第二步获取文本灯信息内容失败");
					}
					else 
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"获取文本灯第三步失败");					
					}
				}
			
				if(CSUDI_PANEL_IS_CTRL(g_aeLight[j]))
				{
					ErrCode = CSUDIPanelGetCapabilityByName(i,g_aeLight[j],&sLightCapability);

					if(CSUDI_TRUE == g_abSuppLight[j])
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第二步获取控制灯信息返回值失败");
						
						CSTK_ASSERT_TRUE_FATAL(g_aeLight[j] == sLightCapability.m_eLightName,"第二步获取控制灯信息内容失败");
						
						CSTK_ASSERT_TRUE_FATAL(g_CtrlFunMask[g_aeLight[j] -EM_UDIPANEL_CTRLSTART] == sLightCapability.m_uProperty.m_sCtrl.m_dwCtrFunMask,"第二步获取控制灯信息内容失败");
					}
					else 
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"获取控制灯第三步失败");					
					}
				}
				
				if(CSUDI_PANEL_IS_SUIT(g_aeLight[j]))
				{
					ErrCode = CSUDIPanelGetCapabilityByName(i,g_aeLight[j],&sLightCapability);

					if(CSUDI_TRUE == g_abSuppLight[j])
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第二步获取组合灯信息返回失败");
						
						CSTK_ASSERT_TRUE_FATAL(g_aeLight[j] == sLightCapability.m_eLightName,"第二步获取组合灯信息内容失败");
						
						CSTK_ASSERT_TRUE_FATAL(g_SuitCnt == sLightCapability.m_uProperty.m_sSuit.m_nSuitCount,"第二步获取组合灯信息内容失败");
					}
					else 
					{
						CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"获取组合灯第三步失败");					
					}
				
				}		
			}
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:测试输入设备号错误的情况 
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引为-1
//@INPUT:2、面板显示类型为平台支持的显示灯类型    
//@INPUT:3、非空的CSUDIPanelLightCapability_S类型指针psLightCapability
//@EXPECTATION:返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIPanelGetCapabilityByName函数，期望返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0002(void)
{
	CSUDIPanelLightCapability_S sLightCapability;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelGetCapabilityByName(-1, EM_UDIPANEL_TEXT0, &sLightCapability),"测试失败") ;		

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:测试输入设备号超过设备数的情况 
//@PRECONDITION:前面板已成功初始化,平台支持的设备数由配置文件给出
//@INPUT:1、面板显示类型eName为平台支持的显示灯类型  
//@INPUT:2、非空的CSUDIPanelLightCapability_S类型指针psLightCapability
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数，由配置文件给出
//@EXECUTIONFLOW:1、调用CSUDIPanelGetCapabilityByName(g_PanelCnt,eName,psLightCapability)期望得到的返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:2、调用CSUDIPanelGetCapabilityByName(g_PanelCnt + 1,eName,psLightCapability)期望得到的返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:3、调用CSUDIPanelGetCapabilityByName(g_PanelCnt + 100,eName,psLightCapability)期望得到的返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0003(void)
{
	CSUDIPanelLightCapability_S sLightCapability;

	CSUDI_Error_Code ErrCode;

	ErrCode = CSUDIPanelGetCapabilityByName(g_PanelCnt, EM_UDIPANEL_TEXT0, &sLightCapability);

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == ErrCode,"测试失败") ;		

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelGetCapabilityByName(g_PanelCnt + 1, EM_UDIPANEL_TEXT0, &sLightCapability),"测试失败") ;		

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelGetCapabilityByName(g_PanelCnt + 100, EM_UDIPANEL_TEXT0, &sLightCapability),"测试失败") ;		
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:测试显示设备类型名非法的情况 
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、非空的 CSUDIPanelLightCapability_S  类型指针psLightCapability
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数，由配置文件给出
//@EXECUTIONFLOW:1、显示类型为EM_UDIPANEL_CTRLSTART调用CSUDIPanelGetCapabilityByName函数，期望得到的返回值类型为CSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:2、显示类型为EM_UDIPANEL_CTRLALL调用CSUDIPanelGetCapabilityByName函数，期望得到的返回值类型为CSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:3、显示类型为EM_UDIPANEL_TEXTSTART调用CSUDIPanelGetCapabilityByName函数，期望得到的返回值类型为CSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:4、显示类型为EM_UDIPANEL_TEXTALL调用CSUDIPanelGetCapabilityByName函数，期望得到的返回值类型为CSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:5、显示类型为EM_UDIPANEL_SUITSTART调用CSUDIPanelGetCapabilityByName函数，期望得到的返回值类型为CSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:6、显示类型为EM_UDIPANEL_SUITALL调用CSUDIPanelGetCapabilityByName函数，期望得到的返回值类型为CSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:7、显示类型为EM_UDIPANEL_MAX调用CSUDIPanelGetCapabilityByName函数，期望得到的返回值类型为CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:8、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0004(void)
{
	CSUDIPanelLightCapability_S sLightCapability;

	int i;

	for(i = 0; i <g_PanelCnt; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_CTRLSTART, &sLightCapability),"第一步测试失败") ;	
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_CTRLALL, &sLightCapability),"第二步测试失败") ;		

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_TEXTSTART, &sLightCapability),"第三步测试失败") ;		

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_TEXTALL, &sLightCapability),"第四步测试失败") ;		

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_SUITSTART, &sLightCapability),"第五步测试失败") ;		

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_SUITALL, &sLightCapability),"第六步测试失败") ;		
		
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i, EM_UDIPANEL_MAX, &sLightCapability),"第七步测试失败") ;		
	}
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelGetCapabilityByName
//@DESCRIPTION:测试CSUDIPanelLightCapability_S类型指针为空的情况 
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、面板显示类型为平台支持的显示类型    
//@INPUT:3、psLightCapability = CSUDI_NULL
//@EXPECTATION:返回CSUDIPANEL_ERROR_BAD_PARAMETER 
//@REMARK:g_PanelCnt为平台支持的面板数，由配置文件给出
//@EXECUTIONFLOW:1、用以上参数调用CSUDIPanelGetCapabilityByName函数，期望得到的返回值类型为CSUDIPANEL_ERROR_BAD_PARAMETER  
//@EXECUTIONFLOW:2、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_GetCapabilityByName_0005(void)
{
	int i;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelGetCapabilityByName(i,EM_UDIPANEL_TEXT0,CSUDI_NULL),"测试失败");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试能否成功设置字符串文本的显示内容 
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、CSUDIPanelLightValule_S类型指针psValue值合法
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数，g_TextCnt为文本灯支持显示的字符长度
//@EXECUTIONFLOW:1、设置要显示的显示灯名称为EM_UDIPANEL_TEXT0，显示类型为EM_UDIPANEL_TEXT_STRING，要显示的内容为boot，要显示的字符个数为4
//@EXECUTIONFLOW:2、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3、若配置文件指明该文本灯不存在，则期望ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED，执行第47步
//@EXECUTIONFLOW:4、若配置文件指明EM_UDIPANEL_TEXT0灯存在，则期望ErrCode == CSUDI_SUCCESS，执行5--46步
//@EXECUTIONFLOW:5、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:6、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容boot

//@EXECUTIONFLOW:7、修改要显示内容为load，要显示的字符个数为4
//@EXECUTIONFLOW:8、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:9、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:10、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容load

//@EXECUTIONFLOW:11、修改要显示内容为good，要显示的字符个数为4
//@EXECUTIONFLOW:12、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:13、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:14、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容good

//@EXECUTIONFLOW:15、修改要显示内容为888888，要显示的字符个数为6
//@EXECUTIONFLOW:16、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:17、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:18、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容888888

//@EXECUTIONFLOW:19、修改要显示内容为九个空格，要显示的字符个数为9
//@EXECUTIONFLOW:20、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:21、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望前面板上该文本灯显示状态为熄灭
//@EXECUTIONFLOW:22、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望前9个字符的显示状态为熄灭

//@EXECUTIONFLOW:23、修改要显示内容为SCAN，要显示的字符个数为4
//@EXECUTIONFLOW:24、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:25、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:26、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容SCAN

//@EXECUTIONFLOW:24、修改要显示内容为C0123，要显示的字符个数为5
//@EXECUTIONFLOW:25、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:26、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:27、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容C0123

//@EXECUTIONFLOW:28、修改要显示内容为R34567，要显示的字符个数为6
//@EXECUTIONFLOW:29、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:30、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:31、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容R34567

//@EXECUTIONFLOW:32、修改要显示内容为A78，要显示的字符个数为3
//@EXECUTIONFLOW:33、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:34、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:35、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容A78

//@EXECUTIONFLOW:36、要显示的字符个数为0
//@EXECUTIONFLOW:37、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:38、期望前面板无任何字符输出

//@EXECUTIONFLOW:39、修改要显示内容为H307，要显示的字符个数为4
//@EXECUTIONFLOW:40、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:41、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:42、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容H307

//@EXECUTIONFLOW:43、修改要显示内容为PORT，要显示的字符个数为4
//@EXECUTIONFLOW:44、调用CSUDIPanelDisplay(0,EM_UDIPANEL_TEXT0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:45、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt小于设置的要显示的字符个数，则期望显示内容为设置的内容前g_TextCnt个字符
//@EXECUTIONFLOW:46、若配置文件指明EM_UDIPANEL_TEXT0灯支持的显示个数g_TextCnt不小于设置的要显示的字符个数，则期望显示内容为设置的内容PORt

//@EXECUTIONFLOW:47、对EM_UDIPANEL_TEXTSTART--EM_UDIPANEL_TEXTALL的灯重复执行以上步骤。
//@EXECUTIONFLOW:48、对所有存在的面板ID重复以上步骤。
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0001(void)
{
	int i;
	int j;
	CSUDI_Error_Code ErrCode;
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_TEXTALL - EM_UDIPANEL_TEXTSTART;++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_TEXTSTART + j];
			
			sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_STRING;
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"boot");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;

			ErrCode = CSUDIPanelDisplay(i,&sValue);

			if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_TEXTSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"第三步测试失败");
				
				continue;
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第四步测试失败");

				if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
				{
					CSTCPrint("前面板上%s文本灯是否输出字符串boot的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第五步测试失败");			
				}
				else
				{
					CSTCPrint("前面板上%s文本灯是否输出字符串boot\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
					
					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第六步测试失败");
				}
			}
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"load");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第八步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串load的前%d个字符\n",  g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第九步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串load\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十步测试失败");
			}
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"good");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第十二步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串good的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十三步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串good\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十四步测试失败");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"888888");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 6;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第十六步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串888888的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十七步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串888888\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十八步测试失败");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"         ");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 9;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第二十步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否为熄灭状态\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二十一步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯的前九个字符是否为熄灭状态\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二十二步测试失败");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"SCAN");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第二十四步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串SCAN的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二十五步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串SCAN\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二十六步测试失败");
			}	

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"C0123");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 5;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第二十八步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串C0123的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二十九步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串C0123\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三十步测试失败");
			}
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"R34567");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 6;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第三十二步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串R34567的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三十三步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串R34567\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三十四步测试失败");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"A78");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 3;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第三十六步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串A78的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三十七步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串A78\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三十八步测试失败");
			}

			sValue.m_uValue.m_sTextValue.m_nStrCount = 0;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"显示字符个数为0 也应该成功");
			
			CSTCPrint("前面板上文本灯是否无任何字符输出\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"测试0 个字符输出失败");

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"H307");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第三十九步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串H307的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第四十一步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串H307\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第四十二步测试失败");
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"PORT");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 4;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第三十九步测试失败");
			
			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串PORT的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j], g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第四十一步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上%s文本灯是否输出字符串PORT\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第四十二步测试失败");
			}
		}
	}
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试能否成功设置时间文本的显示内容 
//@PRECONDITION:前面板已成功初始化且该平台支持的文本灯显示类型由配置文件给出
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、CSUDIPanelLightValule_S类型指针psValue值合法
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数，具体的时间显示方式由平台决定
//@EXECUTIONFLOW:1、设置要显示的显示灯名称为EM_UDIPANEL_TEXT0，显示类型为EM_UDIPANEL_TEXT_TIME，要显示的内容为19991111111111，要显示的字符个数为14
//@EXECUTIONFLOW:2、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3、若配置文件指明该文本灯不存在，则期望ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED，执行第12步
//@EXECUTIONFLOW:4、若配置文件指明EM_UDIPANEL_TEXT0灯存在，则期望ErrCode == CSUDI_SUCCESS，执行5--11步
//@EXECUTIONFLOW:5、期望前面板的时间显示内容为设置的时间内容
//@EXECUTIONFLOW:6、修改要显示内容为20000101010101
//@EXECUTIONFLOW:7、再次调用CSUDIPanelDisplay函数期望返回CSUDI_SUCCESS，期望面板上显示内容变为修改后的内容
//@EXECUTIONFLOW:8、修改要显示内容为20080808080808
//@EXECUTIONFLOW:9、再次调用CSUDIPanelDisplay函数期望返回CSUDI_SUCCESS，期望面板上显示内容变为修改后的内容
//@EXECUTIONFLOW:10、修改要显示内容为12345678901234
//@EXECUTIONFLOW:11、再次调用CSUDIPanelDisplay函数期望返回CSUDI_SUCCESS，期望面板上显示内容变为修改后的内容
//@EXECUTIONFLOW:12、对EM_UDIPANEL_TEXTSTART--EM_UDIPANEL_TEXTALL的灯重复执行以上步骤。
//@EXECUTIONFLOW:13、对所有存在的面板ID重复以上步骤。
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0002(void)
{	
	int i;
	int j;
	CSUDI_Error_Code ErrCode;
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		CSTCPrint("以下测试时间文本的显示，如果平台没有时间点灯则中间的\" : \"不显示\n");
		CSTKWaitAnyKey();
		
		for(j = 1; j < EM_UDIPANEL_TEXTALL - EM_UDIPANEL_TEXTSTART; ++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_TEXTSTART + j];
			
			sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_TIME;
			
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"19991111111111");
			
			sValue.m_uValue.m_sTextValue.m_nStrCount = 14;
			
			ErrCode = CSUDIPanelDisplay(i,&sValue);

			if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_TEXTSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"第三步测试失败");
				
				continue;
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第四步测试失败");

				CSTCPrint("前面板上%s文本灯是否输出1999年11月11日11时11分11秒相符的时间内容(如时间11:11)\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第五步测试失败");			
	
			}

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"20000101010101");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第七步测试失败");

			CSTCPrint("前面板上%s文本灯是否输出字符串2000年01月01日01时01分01秒相符的时间内容(如时间01:01)\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第七步测试失败");	

			//添加测试要求显示字符个数为0 的情况
			sValue.m_uValue.m_sTextValue.m_nStrCount = 0;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"显示字符个数为0 也应该成功");
			
			CSTCPrint("前面板上文本灯是否无任何字符输出\n");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"测试0 个字符输出失败");

			//
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"20080808080808");
			sValue.m_uValue.m_sTextValue.m_nStrCount = 14;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第九步测试失败");

			CSTCPrint("前面板上%s文本灯是否输出字符串2008年08月08日08时08分08秒相符的时间内容(如时间08:08)\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第九步测试失败");			

			strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"12345678901234");
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第十一步测试失败");

			CSTCPrint("前面板上%s文本灯是否输出字符串1234年56月78日90时12分34秒相符的时间内容(如时间90:12或00:12)\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十一步测试失败");			

		}
	}
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试能否成功设置控制灯显示状态的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、传入的CSUDIPanelLightValule_S类型指针psValue值非空  
//@EXPECTATION:每一步都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数
//@EXECUTIONFLOW:1、设置CSUDIPanelLightValule_S类型指针psValue的显示类型名为EM_UDIPANEL_SIGNAL，显示状态为EM_UDIPANEL_CTRL_ON
//@EXECUTIONFLOW:2、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3、如果平台不支持该控制灯则期望ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED，执行第34步
//@EXECUTIONFLOW:4、如果平台支持该控制灯则执行以下步骤
//@EXECUTIONFLOW:5、该控制灯必须具有EM_UDIPANEL_CTRL_ON属性,期望ErrCode == CSUDI_SUCCESS
//@EXECUTIONFLOW:6、期望前面板上该控制灯被点亮
//@EXECUTIONFLOW:7、设置CSUDIPanelLightValule_S类型指针psValue的显示状态为EM_UDIPANEL_CTRL_OFF
//@EXECUTIONFLOW:8、调用CSUDIPanelDisplay(0,psValue)期望返回CSUDI_SUCCESS且前面板上该控制灯为熄灭状态
//@EXECUTIONFLOW:9、设置CSUDIPanelLightValule_S类型指针psValue的显示状态为EM_UDIPANEL_CTRL_FALSH
//@EXECUTIONFLOW:10、调用CSUDIPanelDisplay(0,psValue)期望返回CSUDI_SUCCESS且前面板上该控制灯为默认颜色闪烁状态
//@EXECUTIONFLOW:11、设置CSUDIPanelLightValule_S类型指针psValue的显示状态为EM_UDIPANEL_CTRL_COLOR2
//@EXECUTIONFLOW:12、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:13、如果该控制灯具有EM_UDIPANEL_CTRL_COLOR2属性则期望ErrCode == CSUDI_SUCCESS
//@EXECUTIONFLOW:14、期望前面板上该控制灯为第二种颜色显示状态
//@EXECUTIONFLOW:15、如果该控制灯不具有EM_UDIPANEL_CTRL_COLOR2属性则期望ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:16、如果该控制灯具有EM_UDIPANEL_CTRL_COLOR2属性则执行步骤17~18，否则跳过直接执行步骤19
//@EXECUTIONFLOW:17、设置CSUDIPanelLightValule_S类型指针psValue的显示状态为EM_UDIPANEL_CTRL_OFF
//@EXECUTIONFLOW:18、调用CSUDIPanelDisplay(0,psValue)期望返回CSUDI_SUCCESS且前面板上该控制灯为熄灭状态
//@EXECUTIONFLOW:19、设置CSUDIPanelLightValule_S类型指针psValue的显示状态为EM_UDIPANEL_CTRL_COLOR2FLASH
//@EXECUTIONFLOW:20、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:21、如果该控制灯具有EM_UDIPANEL_CTRL_COLOR2FLASH属性则期望ErrCode == CSUDI_SUCCESS
//@EXECUTIONFLOW:22、期望前面板上该控制灯为第二种颜色闪烁状态
//@EXECUTIONFLOW:23、如果该控制灯不具有EM_UDIPANEL_CTRL_COLOR2FLASH属性则期望ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:24、设置CSUDIPanelLightValule_S类型指针psValue的显示状态为EM_UDIPANEL_CTRL_COLORSWITCH
//@EXECUTIONFLOW:25、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:26、如果该控制灯具有EM_UDIPANEL_CTRL_COLORSWITCH属性则期望ErrCode == CSUDI_SUCCESS
//@EXECUTIONFLOW:27、期望前面板上该控制灯为两种颜色交替闪烁状态
//@EXECUTIONFLOW:28、如果该控制灯不具有EM_UDIPANEL_CTRL_COLORSWITCH属性则期望ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:29、对EM_UDIPANEL_CTRLSTART--EM_UDIPANEL_CTRLALL的所有灯重复以上步骤
//@EXECUTIONFLOW:30、对所有存在的面板ID重复以上步骤。
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0003(void)
{
	int i;
	int j;
	
	CSUDI_Error_Code ErrCode;
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART; ++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_CTRLSTART + j];
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;

			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_CTRLSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "如果平台不支持该控制灯则应该返回CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
				
				continue;
			}

			CSTCPrint("以下测试%s控制灯的显示状态\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "设置控制灯为点亮状态失败");

			CSTCPrint("前面板上%s控制灯是否为点亮状态\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "设置控制灯为点亮状态失败");

			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
		
			ErrCode = CSUDIPanelDisplay(i, &sValue);

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "设置控制灯为熄灭状态失败");

			CSTCPrint("前面板上%s控制灯是否为熄灭状态\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "设置控制灯为熄灭状态失败");
			
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_FLASH;
		
			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(0 != (g_CtrlFunMask[j]&EM_UDIPANEL_CTRL_FLASH))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "设置控制灯为默认颜色闪烁状态失败");

				CSTCPrint("前面板上%s控制灯是否为默认颜色闪烁\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "设置控制灯为默认颜色闪烁状态失败");			
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "如果不支持以默认颜色闪烁应该返回CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
			}
			
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_COLOR2;

			ErrCode = CSUDIPanelDisplay(i, &sValue);
			
			if(0 != (g_CtrlFunMask[j]&EM_UDIPANEL_CTRL_COLOR2))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "设置控制灯以第二种颜色显示失败");

				CSTCPrint("前面板上%s控制灯是否以第二种颜色显示\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "设置控制灯以第二种颜色显示失败");	

				sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
		
				ErrCode = CSUDIPanelDisplay(i, &sValue);

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "设置控制灯为熄灭状态失败");

				CSTCPrint("前面板上%s控制灯是否为熄灭状态\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "设置控制灯为熄灭状态失败");
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "如果不支持以第二种颜色显示应该返回CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
			}
	
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_COLOR2FLASH;
			
			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(0 != (g_CtrlFunMask[j]&EM_UDIPANEL_CTRL_COLOR2FLASH))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "设置控制灯以第二种颜色闪烁失败");

				CSTCPrint("前面板上%s控制灯是否以第二种颜色闪烁\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "设置控制灯以第二种颜色闪烁失败");			
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "如果不支持以第二种颜色闪烁应该返回CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
			}
			
			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_COLORSWITCH;
			
			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(0 != (g_CtrlFunMask[j]&EM_UDIPANEL_CTRL_COLORSWITCH))
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode, "设置控制灯两种颜色交替闪烁失败");

				CSTCPrint("前面板上%s控制灯是否以两种颜色交替闪烁\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);
			
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "设置控制灯两种颜色交替闪烁失败");			
			}
			else
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode, "如果不支持两种颜色交替闪烁应该返回CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED");
			}	
		}
	}
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试能否成功设置组合灯显示状态的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、传入的CSUDIPanelLightValule_S类型指针psValue非空
//@EXPECTATION:每一步都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数，g_Suitcnt为组合灯的子灯个数，由配置文件给出
//@EXECUTIONFLOW:1、设置psValue的显示类型名EM_UDIPANEL_SUIT0，显示值为全1
//@EXECUTIONFLOW:2、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3、如果平台不支持该组合灯则期望ErrCode == CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED，执行第14步
//@EXECUTIONFLOW:4、如果平台支持该组合灯则期望ErrCode == CSUDI_SUCCESS，执行以下步骤
//@EXECUTIONFLOW:5、期望前面板上该组合灯的所有子灯被点亮
//@EXECUTIONFLOW:6、设置psValue的显示值为全0
//@EXECUTIONFLOW:7、调用CSUDIPanelDisplay(0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:8、期望前面板上该组合灯的所有子灯熄灭
//@EXECUTIONFLOW:9、设置psValue的显示值末尾位为1，其余全为0
//@EXECUTIONFLOW:10、调用CSUDIPanelDisplay(0,psValue)期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:11、左移1位psValue的显示值
//@EXECUTIONFLOW:12、重复执行10--11步g_Suitcnt次
//@EXECUTIONFLOW:13、期望前面板上始终只有一个灯显示的转圈效果
//@EXECUTIONFLOW:14、对EM_UDIPANEL_SUITSTART--EM_UDIPANEL_SUITALL的所有组合灯重复以上步骤
//@EXECUTIONFLOW:15、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0004(void)
{
	int i;
	int j;
	int k;

	CSUDI_Error_Code ErrCode;
	
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_SUITALL - EM_UDIPANEL_SUITSTART;++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_SUITSTART + j];

			sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = (1<<g_SuitCnt) - 1;

			ErrCode = CSUDIPanelDisplay(i, &sValue);

			if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_SUITSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"第三步测试失败");
				CSTCPrint("Not supported !!!!\n");
				continue;
			}

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第四步测试失败");
			
			CSTCPrint("前面板上%s组合灯是否全部点亮\t", g_sLight[EM_UDIPANEL_SUITSTART + j]);
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第五步测试失败");			

			sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = 0;
			
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue),"第七步测试失败");
			
			CSTCPrint("前面板上%s组合灯是否全部熄灭\t", g_sLight[EM_UDIPANEL_SUITSTART + j]);
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第八步测试失败");	

			k = 0; 

			while(k < g_SuitCnt)
			{
				sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = 1<<k;

				k++;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue),"第十步测试失败");

			}
			
			CSTCPrint("前面板上%s组合灯是否始终只有一个灯显示的转圈效果\t", g_sLight[EM_UDIPANEL_SUITSTART + j]);
		
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十三步测试失败");	

		}
	}
		
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试设备号错误时的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引为-1
//@INPUT:2、传入非空的CSUDIPanelLightValule_S类型指针psValue
//@EXPECTATION:返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:g_PanelCnt为平台支持的面板数
//@EXECUTIONFLOW:1、调用CSUDIPanelDisplay(-1,psValue)期望返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0005(void)
{
	CSUDIPanelLightValule_S  sValue;

	sValue.m_eLightName = EM_UDIPANEL_POWER;
	
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelDisplay(-1,&sValue),"测试用例失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试显示设备的索引号大于平台支持的最大设备数的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、面板显示类型值合法    
//@INPUT:2、传入非空的CSUDIPanelLightValule_S类型指针psValue
//@EXPECTATION:返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:g_PanelCnt为平台支持的面板数，由配置文件给出
//@EXECUTIONFLOW:1、调用CSUDIPanelDisplay(g_PanelCnt,psValue)期望得到的返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:2、调用CSUDIPanelDisplay(g_PanelCnt + 1,psValue)期望得到的返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:3、调用CSUDIPanelDisplay(g_PanelCnt + 100,psValue)期望得到的返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0006(void)
{
	CSUDIPanelLightValule_S  sValue;

	sValue.m_eLightName = EM_UDIPANEL_POWER;
	
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
		
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelDisplay(g_PanelCnt,&sValue),"第一步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelDisplay(g_PanelCnt + 1,&sValue),"第二步测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelDisplay(g_PanelCnt + 100,&sValue),"第三步测试失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试CSUDIPanelLightValue_S类型指针psValue = CSUDI_NULL时的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、CSUDIPanelLightValule_S类型指针psValue = CSUDI_NULL
//@EXPECTATION:返回CSUDIPANEL_ERROR_BAD_PARAMETER  
//@REMARK:g_PanelCnt为平台支持的面板数
//@EXECUTIONFLOW:1、调用CSUDIPanelDisplay(0,CSUDI_NULL)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER  
//@EXECUTIONFLOW:2、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0007(void)
{
	int i;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,CSUDI_NULL),"测试用例失败");
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
 }
//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试显示类型名非法的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、CSUDIPanelLightValule_S类型指针psValue值非空
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数
//@EXECUTIONFLOW:1、psValue->m_eLightName = EM_UDIPANEL_CTRLSTART时调用CSUDIPanelDisplay(0,psValue)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2、psValue->m_eLightName = EM_UDIPANEL_CTRLALL时调用CSUDIPanelDisplay(0,psValue)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3、psValue->m_eLightName = EM_UDIPANEL_TEXTSTART时调用CSUDIPanelDisplay(0,psValue)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER 
//@EXECUTIONFLOW:4、psValue->m_eLightName = EM_UDIPANEL_TEXTALL时调用CSUDIPanelDisplay(0,psValue)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:5、psValue->m_eLightName = EM_UDIPANEL_SUITSTART时调用CSUDIPanelDisplay(0,psValue)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:6、psValue->m_eLightName = EM_UDIPANEL_SUITALL时调用CSUDIPanelDisplay(0,psValue)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:7、psValue->m_eLightName = EM_UDIPANEL_MAX时调用CSUDIPanelDisplay(0,psValue)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:8、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0008(void)
{
	int i;
	
	CSUDIPanelLightValule_S  sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		sValue.m_eLightName = EM_UDIPANEL_CTRLSTART;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"第一步测试失败");

		sValue.m_eLightName = EM_UDIPANEL_CTRLALL;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"第二步测试失败");

		sValue.m_eLightName = EM_UDIPANEL_TEXTSTART;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"第三步测试失败");

		sValue.m_eLightName = EM_UDIPANEL_TEXTALL;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"第四步测试失败");

		sValue.m_eLightName = EM_UDIPANEL_SUITSTART;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"第五步测试失败");

		sValue.m_eLightName = EM_UDIPANEL_SUITALL;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"第六步测试失败");

		sValue.m_eLightName = EM_UDIPANEL_MAX;

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelDisplay(i,&sValue),"第七步测试失败");

	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试重复调用CSUDIPanelDisplay时的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、传入的CSUDIPanelLightValule_S类型指针psValue值合法
//@EXPECTATION:每一步都输出预期结果
//@REMARK:g_RepeatCnt为重复执行的次数
//@EXECUTIONFLOW:1、设置显示灯类型为平台支持的控制灯类型、显示状态为EM_UDIPANEL_CTRL_OFF，连续调用十次CSUDIPanelDisplay函数，期望每次都返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、设置显示状态为EM_UDIPANEL_CTRL_ON，连续调用十次CSUDIPanelDisplay函数期望每次返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、重复执行1--2步g_RepeatCnt次，期望面板上该控制灯时灭时亮且最后该控制灯为点亮状态
//@EXECUTIONFLOW:4、设置显示状态为EM_UDIPANEL_CTRL_ON，调用CSUDIPanelDisplay函数期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、设置显示状态为EM_UDIPANEL_CTRL_OFF，调用CSUDIPanelDisplay函数，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:6、重复执行4--5步g_RepeatCnt次，期望面板上该控制灯时灭时亮且最后为熄灭状态
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0009(void)
{
	int i;

	int j;
	
	CSUDIPanelLightValule_S  sValue;
	
	memset(&sValue, 0, sizeof(sValue));
	sValue.m_eLightName = EM_UDIPANEL_SIGNAL;

	for(i=EM_UDIPANEL_CTRLSTART+1; i<EM_UDIPANEL_CTRLALL; i++)
	{
		if(g_abSuppLight[i])
		{
			sValue.m_eLightName = i;
			break;
		}
	}
	
	CSTCPrint("请留意前面板上%s灯是否时灭时亮且时间间隔约为2秒且最后为点亮状态，按任意键继续\n",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);

	CSTKWaitAnyKey();
	
	for(i = 0; i < REPEAT_TIME; ++i)
	{
		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
		for(j = 0; j < 10; ++j)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,&sValue),"第一步测试失败");
		}

		CSUDIOSThreadSleep(2000);

		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
		for(j = 0; j < 10; ++j)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,&sValue),"第二步测试失败");
		}

		CSUDIOSThreadSleep(2000);
	}
	
	CSTCPrint("前面板上%s灯是否时灭时亮且时间间隔约为2秒且最后为点亮状态\n",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三步测试失败");	
	
	CSTCPrint("请留意前面板上%s灯是否时亮时灭且时间间隔约为2秒最后为熄灭状态，按任意键继续\n",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);

	CSTKWaitAnyKey();

	for(i = 0; i < REPEAT_TIME; ++i)
	{
		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,&sValue),"第四步测试失败");
		
		CSUDIOSThreadSleep(2000);

		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,&sValue),"第五步测试失败");

		CSUDIOSThreadSleep(2000);
	}
	
	CSTCPrint("前面板上%s控制灯是否时亮时灭且时间间隔约为2秒最后为熄灭状态\n",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第六步测试失败");	
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;	
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试不同任务设置同一个显示灯的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:合法的输入参数
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:、
//@EXECUTIONFLOW:1、创建两个任务 
//@EXECUTIONFLOW:2、在第一个任务中重复设置TEXT0文本灯的显示状态为abcd
//@EXECUTIONFLOW:3、在第二个任务中重复设置TEXT0文本灯的显示状态为8888
//@EXECUTIONFLOW:4、期望前面板TEXT0文本灯的显示内容发生变化
//@EXECUTIONFLOW:5、关闭TEXT0的显示内容
//@EXECUTIONFLOW:6、销毁这两个任务
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0010(void)
{
	CSUDIPanelLightValule_S  sTextValue;

	int nPriority = 128;
	
	int nParam = 0;
	
	CSUDI_HANDLE hThreadhandle1 = CSUDI_NULL;
	
	CSUDI_HANDLE hThreadhandle2 = CSUDI_NULL;
	
	g_bThreadEnd1 = CSUDI_FALSE;
	
	g_bThreadEnd2 = CSUDI_FALSE;

	CSTCPrint("请留意前面板上%s灯的状态，按任意键继续\n",g_sLight[EM_UDIPANEL_TEXT0]);
	
	CSTKWaitAnyKey();

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("PANEL_TestThread1", nPriority , PANEL_TEST_STACK_SIZE,PANEL_TestThread1Entry, &nParam, &hThreadhandle1),"创建任务失败\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle1,"创建成功的任务输出的任务句柄不能为空\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate("PANEL_TestThread2", nPriority , PANEL_TEST_STACK_SIZE,PANEL_TestThread2Entry, &nParam, &hThreadhandle2),"创建任务失败\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != hThreadhandle2,"创建成功的任务输出的任务句柄不能为空\n");

	while(CSUDI_TRUE != g_bThreadEnd1&&CSUDI_TRUE != g_bThreadEnd2)
	{
		CSUDIOSThreadSleep(6000);
	}

	CSTCPrint("前面板上TEXT0文本灯是否变化\n");
	CSTCPrint("前面板上TEXT0文本灯是否仅有abcd8 5种字符\n");
	CSTCPrint("前面板上TEXT0文本灯若出现的英文字符，是否从左到右按abcd 顺序排列，不允许错位\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"测试用例失败");

	sTextValue.m_eLightName = EM_UDIPANEL_TEXT0;
	sTextValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
	strcpy(sTextValue.m_uValue.m_sTextValue.m_szStr,"    ");
	sTextValue.m_uValue.m_sTextValue.m_nStrCount = 4;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(0,  &sTextValue),"关闭显示灯失败\n");

	CSTK_FATAL_POINT
	{
		if (hThreadhandle1 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle1), "删除任务失败\n");
			   hThreadhandle1 = CSUDI_NULL;
		}
		if (hThreadhandle2 != CSUDI_NULL)
		{
			  CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(hThreadhandle2), "删除任务失败\n");
			   hThreadhandle2 = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试文本灯CSUDIPanelLightValule_S 值赋值错误的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:CSUDIPanelLightValule_S 值赋值错误
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:、
//@EXECUTIONFLOW:1、设置psValue的显示类型名为文本类型
//@EXECUTIONFLOW:2、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3、希望Errcode 不为成功
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0011(void)
{
	CSUDIPanelLightValule_S sValue;
	int i = 0;
	int j = 0;
	CSUDI_Error_Code ErrCode = CSUDI_SUCCESS;
	CSUDIPanelTextValue_S sErrorParam[] =
	{
			{EM_UDIPANEL_TEXT_STRING -1, "abc", 1},
			{EM_UDIPANEL_TEXT_TIME + 1, "abcd", 2},
			//{EM_UDIPANEL_TEXT_STRING, (char)"", 3},
			{EM_UDIPANEL_TEXT_STRING, "abcde", 33},
			{EM_UDIPANEL_TEXT_STRING, "abcef", -1},
			{EM_UDIPANEL_TEXT_TIME, "abcefg", 3},
	};

	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 0; j < sizeof(sErrorParam)/sizeof(sErrorParam[0]); j++)
		{
			sValue.m_eLightName = EM_UDIPANEL_TEXT0;
			sValue.m_uValue.m_sTextValue.m_eDisplayType = sErrorParam[j].m_eDisplayType;
			strcpy(sValue.m_uValue.m_sTextValue.m_szStr, sErrorParam[j].m_szStr);
			sValue.m_uValue.m_sTextValue.m_nStrCount = sErrorParam[j].m_nStrCount;
		
			ErrCode = CSUDIPanelDisplay(i, &sValue);
			CSTCPrint("m_eDisplayType = %d, m_szStr = %s, m_nStrCount = %d \n",
					   sErrorParam[j].m_eDisplayType, sErrorParam[j].m_szStr, sErrorParam[j].m_nStrCount);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "参数检测失败!");
		}
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试控制灯CSUDIPanelLightValule_S 值赋值错误的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:CSUDIPanelLightValule_S 值赋值错误
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:、
//@EXECUTIONFLOW:1、设置psValue的显示类型名为控制类型
//@EXECUTIONFLOW:2、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3、希望Errcode 不为成功
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0012(void)
{
	CSUDIPanelLightValule_S sValue;
	int i = 0;
	CSUDI_Error_Code ErrCode = CSUDI_SUCCESS;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		sValue.m_eLightName = EM_UDIPANEL_SIGNAL;
		
		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = 0;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "参数检测失败1 !");

		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = -1;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "参数检测失败2 !");

		sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = 64;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "参数检测失败3 !");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试组合灯CSUDIPanelLightValule_S 值赋值错误的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:CSUDIPanelLightValule_S 值赋值错误
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:、
//@EXECUTIONFLOW:1、设置psValue的显示类型名为组合类型
//@EXECUTIONFLOW:2、ErrCode = CSUDIPanelDisplay(0,psValue)
//@EXECUTIONFLOW:3、希望Errcode 不为成功
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0013(void)
{
	CSUDIPanelLightValule_S sValue;
	int i = 0;
	CSUDI_Error_Code ErrCode = CSUDI_SUCCESS;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		sValue.m_eLightName = EM_UDIPANEL_SUIT0;
		
		sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = -1;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "参数检测失败1 !");

		sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = 1<<g_SuitCnt;
		ErrCode = CSUDIPanelDisplay(i, &sValue);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != ErrCode, "参数检测失败2 !");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelDisplay
//@DESCRIPTION:测试能否正确显示时间点，且时间点能够独立显示不影响字符串和时间的显示 
//@PRECONDITION:前面板已成功初始化且该平台支持的文本灯显示类型由配置文件给出
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、CSUDIPanelLightValule_S类型指针sValue值合法
//@EXPECTATION:，交替显示时间点、字符串及时间点，每一步骤都输出预期结果
//@REMARK:时间点为显示时间时小时和分钟之间的两个小点
//@EXECUTIONFLOW:1、如果平台不支持此灯类型，则不执行后续步骤
//@EXECUTIONFLOW:2、点亮面板时间点，期望时间点为点亮状态
//@EXECUTIONFLOW:3、熄灭面板时间点，期望时间点为熄灭状态
//@EXECUTIONFLOW:4、点亮面板时间点，期望时间点为点亮状态
//@EXECUTIONFLOW:5、判断是否支持显示时间内容
//@EXECUTIONFLOW:6，7、若支持显示时间内容，期望显示时间内容正确
//@EXECUTIONFLOW:8，9、在步骤7已有显示内容基础上，单独操作显示时间点，期望时间点点亮且不影响先前显示内容
//@EXECUTIONFLOW:10，11、在步骤9 的基础上熄灭面板时间点，期望时间点为熄灭状态，且面板先前显示其它内容不变
//@EXECUTIONFLOW:12、在步骤11 的基础上点亮面板时间点，期望时间点为点亮状态，且面板先前显示其它内容不变
//@EXECUTIONFLOW:13、判断是否支持显示字符串内容
//@EXECUTIONFLOW:14，15，16、若支持显示字符串内容，则期望显示字符串内容正确
//@EXECUTIONFLOW:17，18、在面板已经显示字符串的基础上，单独操作显示时间点，期望时间点为点亮状态且不影响原有字符串显示内容
//@EXECUTIONFLOW:19、在步骤18 的基础上，熄灭时间点，期望时间点为熄灭状态，且不影响原有字符串显示内容
CSUDI_BOOL CSTC_PANEL_IT_PanelDisplay_0014(void)
{
	CSUDIPanelLightValule_S  sValue;
	CSUDI_Error_Code ErrCode;
	
	memset(&sValue, 0, sizeof(sValue));
	
	/***********************显示时间点开始****************************/
	sValue.m_eLightName = EM_UDIPANEL_CLOCK;
	if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_CLOCK])
	{
		CSTCPrint("该平台不支持此种类型灯!\t");
		return CSUDI_FALSE;
	}
	CSTCPrint("请留意前面板上%s灯的状态，按任意键继续\t",g_sLight[sValue.m_eLightName - EM_UDIPANEL_CTRLSTART]);
	CSTKWaitAnyKey();
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第一步测试失败");
	CSTCPrint("前面板上时间点灯是否为点亮状态\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二步测试失败");	
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTCPrint("前面板上时间点灯是否为熄灭状态\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三步测试失败");
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTCPrint("前面板上时间点灯是否为点亮状态\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第四步测试失败");	
	 /***********************显示时间点结束****************************/
	 
	 /****************显示时间开始***********************************/
	sValue.m_eLightName =EM_UDIPANEL_TEXT0;
	sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_TIME;
	strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"19991111111111");
	sValue.m_uValue.m_sTextValue.m_nStrCount = 14;
	ErrCode = CSUDIPanelDisplay(0,&sValue);
	if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_TEXT0])
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"第五步测试失败");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第六步测试失败");
		CSTCPrint("前面板上%s文本灯是否输出1999年11月11日11时11分11秒相符的时间内容(如时间11:11)\t", g_sLight[EM_UDIPANEL_TEXT0]);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第七步测试失败");			
	}
	 /****************显示时间结束***********************************/


	 /***********************显示时间点开始****************************/
	sValue.m_eLightName = EM_UDIPANEL_CLOCK;
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第八步测试失败");
	CSTCPrint("前面板上时间点灯是否为点亮状态，且面板上其它内容无变化(显示11:11) \t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第九步测试失败");	
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第十步测试失败");
	CSTCPrint("前面板上时间点灯是否为熄灭状态，且面板上其它内容无变化(显示11 11) \t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十一步测试失败");
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTCPrint("前面板上时间点灯是否为点亮状态，且面板上其它内容无变化(显示11:11) \t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十二步测试失败");	
	/***********************显示时间点结束****************************/


	   /***********************显示字符串开始****************************/
	sValue.m_eLightName = g_aeLight[EM_UDIPANEL_TEXT0];
	sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_STRING;
	strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"a6b54");
	sValue.m_uValue.m_sTextValue.m_nStrCount = 5;
	ErrCode = CSUDIPanelDisplay(0,&sValue);
	if(CSUDI_TRUE != g_abSuppLight[EM_UDIPANEL_TEXT0])
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == ErrCode,"第十三步测试失败");
       }
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第十四步测试失败");
		if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
		{
			CSTCPrint("前面板上%s文本灯是否正常输出字符串a6b54的前%d个字符(显示a6 b5)\t", g_sLight[EM_UDIPANEL_TEXT0], g_TextCnt);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十五步测试失败");			
		}
		else
		{
			CSTCPrint("前面板上%s文本灯是否正常输出字符串a6b54(显示a6 b5)\t", g_sLight[EM_UDIPANEL_TEXT0]);
			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十六步测试失败");
		}
        }
	/***********************显示字符串结束****************************/

	/***********************显示时间点开始****************************/
	sValue.m_eLightName = EM_UDIPANEL_CLOCK;
	sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第十七步测试失败");
	CSTCPrint("前面板上时间点灯是否为点亮状态，且面板上其它内容无变化(显示a6:b5)\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十八步测试失败");
	 sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_OFF;
	ErrCode=CSUDIPanelDisplay(0,&sValue);
	 CSTCPrint("前面板上时间点灯是否为熄灭状态且面板上其它内容无变化(显示a6 b5)\t");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第十九步测试失败");	
	 /***********************显示时间点结束****************************/
	 
	CSTK_FATAL_POINT;
	return CSUDI_TRUE;	

}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:测试成功清除面板上显示灯的显示信息 
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、面板显示类型值合法
//@EXPECTATION:每一步骤都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数
//@EXECUTIONFLOW:1、若配置文件指明EM_UDIPANEL_MAIL灯存在则设置MAIL显示灯的显示状态为点亮
//@EXECUTIONFLOW:2、调用CSUDIPanelClear(0,EM_UDIPANEL_MAIL)期望返回CSUDI_SUCCESS且前面板上该控制灯的显示信息被清空
//@EXECUTIONFLOW:3、若配置文件指明EM_UDIPANEL_MAIL灯不存在，调用CSUDIPanelClear(0,EM_UDIPANEL_MAIL)期望返回CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:4、对EM_UDIPANEL_CTRLSTART--EM_UDIPANEL_MAX的所有灯（除EM_UDIPANEL_CTRLSTART、EM_UDIPANEL_CTRLALL、EM_UDIPANEL_TEXTSTART、EM_UDIPANEL_TEXTALL、EM_UDIPANEL_SUITSTART、EM_UDIPANEL_SUITALL、EM_UDIPANEL_MAX枚举值外）重复以上步骤
//@EXECUTIONFLOW:5、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0001(void)
{
	int i;
	int j;
	CSUDIPanelLightValule_S  sValue;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_MAX - EM_UDIPANEL_CTRLSTART; ++j)
		{
			if(CSUDI_PANEL_IS_TEXT(g_aeLight[j]))
			{
				if(CSUDI_TRUE == g_abSuppLight[j])
				{
					sValue.m_eLightName = g_aeLight[j];
					
					sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_STRING;
					
					strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"a6b54");
					
					sValue.m_uValue.m_sTextValue.m_nStrCount = 5;

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "TEXT第一步测试失败");

					CSTCPrint("前面板上%s显示灯是否输出字符串a6b54或者为该字符串的前几个字符\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"TEXT第一步测试失败");	

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, g_aeLight[j]), "TEXT第二步测试失败");

					CSTCPrint("前面板上%s显示灯显示内容是否被清空\n", g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"TEXT第二步测试失败");	
			
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == CSUDIPanelClear(i, g_aeLight[j]),"TEXT第三步测试失败");
				}
			}
		
			if(CSUDI_PANEL_IS_CTRL(g_aeLight[j]))
			{
				if(CSUDI_TRUE == g_abSuppLight[j])
				{
					sValue.m_eLightName = g_aeLight[j];
											
					sValue.m_uValue.m_sCtrlValue.m_eCtrlValue  = EM_UDIPANEL_CTRL_ON;

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "CTRL第一步测试失败");

					CSTCPrint("前面板上%s显示灯是否为点亮状态\n",g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"CTRL第一步测试失败");	

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, g_aeLight[j]), "CTRL第二步测试失败");

					CSTCPrint("前面板上%s显示灯显示状态是否被清空\n",g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"CTRL第二步测试失败");	
			
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == CSUDIPanelClear(i, g_aeLight[j]),"CTRL第三步测试失败");
				}

			}
			
			if(CSUDI_PANEL_IS_SUIT(g_aeLight[j]))
			{
				if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_CTRLSTART + j])
				{
					sValue.m_eLightName = g_aeLight[j];
					
					sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = (1<<g_SuitCnt) - 1;

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "SUIT第一步测试失败");

					CSTCPrint("前面板上%s显示灯是否为点亮状态\n",g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"SUIT第一步测试失败");	

					CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, g_aeLight[j]), "SUIT第二步测试失败");

					CSTCPrint("前面板上%s显示灯显示内容是否被清空\n",g_sLight[EM_UDIPANEL_CTRLSTART + j]);

					CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"SUIT第二步测试失败");	
			
				}
				else
				{
					CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == CSUDIPanelClear(i, g_aeLight[j]),"SUIT第三步测试失败");
				}
			}		
		}
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:测试错误设备号情况 
//@PRECONDITION:前面板已成功初始化且前板
//@INPUT:1、显示设备的索引为-1
//@INPUT:2、面板显示类型值合法       
//@EXPECTATION:返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIPanelClear函数期望返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelClear(-1, EM_UDIPANEL_PLAY),"测试失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:测试设备号超过设备数的情况
//@PRECONDITION:前面板已成功初始化
//@INPUT:面板显示类型值合法    
//@EXPECTATION:返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID
//@REMARK:g_PanelCnt为平台支持的面板数，由配置文件给出
//@EXECUTIONFLOW:1、调用CSUDIPanelClear(g_PanelCnt,EM_UDIPANEL_PLAY)函数，期望得到的返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:2、调用CSUDIPanelClear(g_PanelCnt + 1,EM_UDIPANEL_PLAY)函数，期望得到的返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
//@EXECUTIONFLOW:3、调用CSUDIPanelClear(g_PanelCnt + 100,EM_UDIPANEL_PLAY)函数，期望得到的返回值类型为CSUDIPANEL_ERROR_INVALID_DEVICE_ID 
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0003(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelClear(g_PanelCnt, EM_UDIPANEL_TEXT0),"测试失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelClear(g_PanelCnt + 1, EM_UDIPANEL_TEXT0),"测试失败");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_INVALID_DEVICE_ID == CSUDIPanelClear(g_PanelCnt + 100, EM_UDIPANEL_TEXT0),"测试失败");
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;

}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:测试参数eName值非法的情况 
//@PRECONDITION:前面板已成功初始化
//@INPUT:显示设备的索引值合法
//@EXPECTATION:每一步都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数
//@EXECUTIONFLOW:1、调用CSUDIPanelClear(0,EM_UDIPANEL_CTRLSTART)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:2、调用CSUDIPanelClear(0,EM_UDIPANEL_TEXTSTART)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:3、调用CSUDIPanelClear(0,EM_UDIPANEL_SUITSTART)期望返回CSUDIPANEL_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0004(void)
{
	
	int i;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelClear(i,EM_UDIPANEL_CTRLSTART),"第一步测试失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelClear(i,EM_UDIPANEL_TEXTSTART),"第二步测试失败");

		CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_BAD_PARAMETER == CSUDIPanelClear(i,EM_UDIPANEL_SUITSTART),"第三步测试失败");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:测试面板显示类型为EM_UDIPANEL_TEXTALL时的情况 
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、面板显示类型为EM_UDIPANEL_TEXTALL         
//@EXPECTATION:每一步都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数
//@EXECUTIONFLOW:1、若配置文件指明前面板不支持文本灯则执行第4步
//@EXECUTIONFLOW:2、若配置文件指明前面板支持文本灯则设置前面板上文本灯的显示状态
//@EXECUTIONFLOW:3、调用CSUDIPanelClear(0,EM_UDIPANEL_TEXTALL)期望返回CSUDI_SUCCESS，且前面板上文本灯全部熄灭
//@EXECUTIONFLOW:4、调用CSUDIPanelClear(0,EM_UDIPANEL_TEXTALL)期望返回CSUDI_SUCCESS，且前面板的显示状态不变
//@EXECUTIONFLOW:5、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0005(void)
{
	int i;
	int j;
	int temp;
	CSUDIPanelLightValule_S  sValue;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		for(j = 1; j < EM_UDIPANEL_TEXTALL - EM_UDIPANEL_TEXTSTART; ++j)
		{
			temp = 0;
		
			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_TEXTSTART + j])
			{
				temp = 1;
				
				sValue.m_eLightName = g_aeLight[j + EM_UDIPANEL_TEXTSTART];
				
				sValue.m_uValue.m_sTextValue.m_eDisplayType =  EM_UDIPANEL_TEXT_STRING;
				
				strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"a6b54");
				
				sValue.m_uValue.m_sTextValue.m_nStrCount = 5;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "第二步测试失败");

			}
			if(temp == 1)
			{
				CSTCPrint("前面板上%s显示灯是否输出字符串a6b54的前%d个字符\n", g_sLight[EM_UDIPANEL_TEXTSTART + j],g_TextCnt);

				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二步测试失败");	

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_TEXTALL), "第三步测试失败");

				CSTCPrint("前面板上%s文本显示灯的显示内容是否被清空\n", g_sLight[EM_UDIPANEL_TEXTSTART + j]);

				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三步测试失败");	
			}
						
		}
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_TEXTALL), "第四步测试失败");

		CSTCPrint("前面板上的显示内容是否无变化\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第四步测试失败");	
	}
	
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:测试面板显示类型为EM_UDIPANEL_CTRLALL情况 
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、面板显示类型为EM_UDIPANEL_CTRLALL         
//@EXPECTATION:每一步都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数
//@EXECUTIONFLOW:1、若配置文件指明不支持控制灯显示则执行第4步
//@EXECUTIONFLOW:2、若配置文件指明支持控制灯显示则设置控制灯的显示状态为点亮
//@EXECUTIONFLOW:3、调用CSUDIPanelClear(0,EM_UDIPANEL_CTRLALL)期望返回CSUDI_SUCCESS且前面板控制灯全部熄灭
//@EXECUTIONFLOW:4、调用CSUDIPanelClear(0,EM_UDIPANEL_CTRLALL)期望返回CSUDI_SUCCESS且前面板显示信息无变化
//@EXECUTIONFLOW:5、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0006(void)
{
	int i;

	int j;

	int temp;

	CSUDIPanelLightValule_S  sValue;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		temp = 0;
		
		for(j = 1; j < EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART; ++j)
		{
			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_CTRLSTART + j])
			{
				temp = 1;
				
				sValue.m_eLightName = g_aeLight[j];
		
				sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "第二步测试失败");
			}	
		}
		
		if(temp == 1)
		{
			CSTCPrint("前面板上控制灯是否全部被点亮\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二步测试失败");	

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_CTRLALL), "第三步测试失败");

			CSTCPrint("前面板上所有控制灯的显示内容是否被清空\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三步测试失败");	
		}

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_CTRLALL), "第四步测试失败");

		CSTCPrint("前面板上的显示内容是否无变化\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第四步测试失败");	
		
	}
					
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:测试面板显示类型为EM_UDIPANEL_SUITALL情况 
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、面板显示类型为EM_UDIPANEL_SUITALL         
//@EXPECTATION:每一步都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数，g_SuitCnt为组合灯的子灯个数
//@EXECUTIONFLOW:1、若配置文件指明不支持组合灯显示则执行第4步
//@EXECUTIONFLOW:2、若配置文件指明支持组合灯显示则设置组合灯的显示状态为点亮
//@EXECUTIONFLOW:3、调用CSUDIPanelClear(0,EM_UDIPANEL_SUITALL)期望返回CSUDI_SUCCESS且前面板组合显示灯全部熄灭
//@EXECUTIONFLOW:4、调用CSUDIPanelClear(0,EM_UDIPANEL_SUITALL)期望返回CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED
//@EXECUTIONFLOW:5、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0007(void)
{
	int i;
	int j;
	int temp;
	CSUDIPanelLightValule_S  sValue;

	for(i = 0; i < g_PanelCnt; ++i)
	{
		temp = 0;
		
		for(j = 1; j < EM_UDIPANEL_SUITALL - EM_UDIPANEL_SUITSTART; ++j)
		{
			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_SUITSTART + j])
			{
				temp = 1;
				
				sValue.m_eLightName = g_aeLight[j];
		
				sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = (1<<g_SuitCnt) - 1;

				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue), "第二步测试失败");
			}	
		}
		
		if(temp == 1)
		{
			CSTCPrint("前面板上组合灯是否全部被点亮");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二步测试失败");	

			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_SUITALL), "第三步测试失败");

			CSTCPrint("前面板上所有组合灯的显示内容是否被清空\n");

			CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三步测试失败");	
		}
		else 
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED == CSUDIPanelClear(i, EM_UDIPANEL_SUITALL), "第四步测试失败");

		}
		
	}
					
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIPanelClear
//@DESCRIPTION:测试面板显示类型为EM_UDIPANEL_MAX时的情况 
//@PRECONDITION:前面板已成功初始化
//@INPUT:1、显示设备的索引值合法
//@INPUT:2、面板显示类型为EM_UDIPANEL_MAX         
//@EXPECTATION:每一步都输出预期结果
//@REMARK:g_PanelCnt为平台支持的面板数
//@EXECUTIONFLOW:1、根据配置文件设置多个不同类型灯的显示状态为点亮
//@EXECUTIONFLOW:2、调用CSUDIPanelClear(0,EM_UDIPANEL_MAX)期望返回CSUDI_SUCCESS且前面板所有显示灯全部熄灭
//@EXECUTIONFLOW:3、再次调用CSUDIPanelClear(0,EM_UDIPANEL_MAX)期望返回CSUDI_SUCCESS且前面板显示信息无变化
//@EXECUTIONFLOW:4、对所有存在的面板ID重复以上步骤
CSUDI_BOOL CSTC_PANEL_IT_PanelClear_0008(void)
{
	int i;
	int j;
	CSUDI_Error_Code ErrCode;
	CSUDIPanelLightValule_S sValue;
	
	for(i = 0; i < g_PanelCnt; ++i)
	{
		sValue.m_eLightName = EM_UDIPANEL_TEXT0;
		
		sValue.m_uValue.m_sTextValue.m_eDisplayType = EM_UDIPANEL_TEXT_STRING;
		
		strcpy(sValue.m_uValue.m_sTextValue.m_szStr,"88888");
		
		sValue.m_uValue.m_sTextValue.m_nStrCount = 5;

		ErrCode = CSUDIPanelDisplay(i,&sValue);

		if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_TEXT0 - EM_UDIPANEL_CTRLSTART])
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == ErrCode,"第一步测试失败");

			if(g_TextCnt < sValue.m_uValue.m_sTextValue.m_nStrCount)
			{
				CSTCPrint("前面板上是否输出字符串88888的前%d个字符\n",g_TextCnt);
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第一步测试失败");			
			}
			else
			{
				CSTCPrint("前面板上是否输出字符串88888\n");
				
				CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第一步测试失败");
			}
		}
		
		for(j = 1; j < EM_UDIPANEL_CTRLALL - EM_UDIPANEL_CTRLSTART;++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_CTRLSTART + j];

			sValue.m_uValue.m_sCtrlValue.m_eCtrlValue = EM_UDIPANEL_CTRL_ON;

			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_CTRLSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i,&sValue),"第一步测试失败");
			}
		}
	
		CSTCPrint("前面板上所有控制灯是否显示\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第一步测试失败");	
		
		for(j = 1; j < EM_UDIPANEL_SUITALL - EM_UDIPANEL_SUITSTART;++j)
		{
			sValue.m_eLightName = g_aeLight[EM_UDIPANEL_SUITSTART + j];

			sValue.m_uValue.m_sSuitValue.m_dwSuitValueMask = (1<<g_SuitCnt) - 1;

			if(CSUDI_TRUE == g_abSuppLight[EM_UDIPANEL_SUITSTART + j])
			{
				CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelDisplay(i, &sValue),"第一步测试失败");
			}
		}
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i,EM_UDIPANEL_MAX),"第二步测试失败");

		CSTCPrint("前面板上所有显示灯的显示内容是否被清空\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第二步测试失败");	

		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIPanelClear(i, EM_UDIPANEL_MAX), "第三步测试失败");

		CSTCPrint("前面板上的显示内容是否无变化\n");

		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"第三步测试失败");	
	}
						
	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}


