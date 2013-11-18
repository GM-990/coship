/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：CSTC_测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_toolsettestcase.h"
#include "cs_toolset_test.h"
#include <string.h>

//芯片支持获取ChipID否
extern CSUDI_BOOL g_bSupportGetChipID;

//主芯片的ID信息
extern CSUDICHIPID_S g_sCHIPID;

//机器的序列号和Key值
extern CSUDISerialKey_S g_sSerialKey;

//系统支持不支持halt功能
extern CSUDI_BOOL g_bSupportSysHalt;

CSUDI_BOOL CSTC_TOOLSET_Init(void)
{
	//在本测试用例集执行前调用
	CSTC_TOOLSET_InitCfg();
// 	printf("#####\n");
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_TOOLSET_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDIGetChipID
//@DESCRIPTION:测试参数psChipID = CSUDI_NULL时的情况
//@PRECONDITION:主芯片已成功初始化
//@INPUT: psChipID = CSUDI_NULL 
//@EXPECTATION:返回非CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIGetChipID期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_TOOLSET_IT_GetChipID_0001(void)
{

    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIGetChipID(CSUDI_NULL), "测试用例失败");

    	CSTK_FATAL_POINT

	return CSUDI_TRUE;
} 

//@CASEGROUP: CSUDIGetChipID
//@DESCRIPTION:测试能否正确得到主芯片ID信息
//@PRECONDITION:主芯片已成功初始化
//@INPUT:psChipID值合法
//@EXPECTATION:返回CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1、如果主芯片支持获取ChipID调用CSUDIGetChipID期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、期望得到的主芯片ID信息与配置文件里的主芯片ID信息相同
//@EXECUTIONFLOW:3、否则调用CSUDIGetChipID期望返回 CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_TOOLSET_IT_GetChipID_0002(void)
{
	CSUDICHIPID_S sChipID;
	
	if(CSUDI_TRUE == g_bSupportGetChipID)
	{
	    	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIGetChipID(&sChipID), "第一步测试用例失败");
			
		CSTK_ASSERT_TRUE_FATAL(g_sCHIPID.m_nLength == sChipID.m_nLength, "第二步获取ChipID信息失败");
		
		CSTK_ASSERT_TRUE_FATAL(0 == (strcmp(g_sCHIPID.m_szChipID ,  sChipID.m_szChipID)), "第二步获取ChipID信息失败");

	    	CSTK_FATAL_POINT
	}
	else
	{
	    	CSTK_ASSERT_TRUE_FATAL(CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED == CSUDIGetChipID(&sChipID), "第三步测试失败");
	}
	return CSUDI_TRUE;
    
} 

//@CASEGROUP: CSUDIGetSerialKey 
//@DESCRIPTION:测试参数psSerialKey = CSUDI_NULL时的情况
//@PRECONDITION:目标机器已成功初始化
//@INPUT:psSerialKey = CSUDI_NULL
//@EXPECTATION:返回非CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIGetSerialKey期望返回非CSUDI_SUCCESS 
CSUDI_BOOL CSTC_TOOLSET_IT_GetSerialKey_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIGetSerialKey(CSUDI_NULL),"测试用例失败");
	
    	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
    
} 

//@CASEGROUP: CSUDIGetSerialKey 
//@DESCRIPTION:测试正确得到目标机器序列号和KEY值时的情况
//@PRECONDITION:目标机器已成功初始化
//@INPUT: psSerialKey值合法
//@EXPECTATION: 返回CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIGetSerialKey期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、期望得到的目标机器序列号与配置文件里的信息相同
//@EXECUTIONFLOW:3、期望得到的KEY值与配置文件里的KEY值相同
CSUDI_BOOL CSTC_TOOLSET_IT_GetSerialKey_0002(void)
{
	CSUDISerialKey_S  sSerialKey;


   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIGetSerialKey(&sSerialKey),"第一步返回值不为CSUDI_SUCCESS ");

	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(sSerialKey.m_szSerial,g_sSerialKey.m_szSerial),"第二步获取序列号失败");
	
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(sSerialKey.m_szPairsKey,g_sSerialKey.m_szPairsKey),"第三步获取KEY值失败");
	
	CSTK_FATAL_POINT
	 	
	return CSUDI_TRUE;
} 

//@CASEGROUP: CSUDISYSReboot
//@DESCRIPTION:测试能否成功实现软重启目标机器
//@PRECONDITION:目标机器成功初始化
//@INPUT: 
//@EXPECTATION: 实现成功返回CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDISYSReboot期望返回CSUDI_SUCCESS且目标机器软重启成功
CSUDI_BOOL CSTC_TOOLSET_IT_Reboot_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSReboot(),"测试用例失败"); 
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "没有成功实现软重启"); 
	
    CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSHalt
//@DESCRIPTION:测试能否成功实现系统关机
//@PRECONDITION:目标机器成功初始化
//@INPUT: 
//@EXPECTATION: 实现成功返回CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1、如果平台支持系统关机调用CSUDISYSHalt期望返回CSUDI_SUCCESS或无返回值且目标机器关机
//@EXECUTIONFLOW:1、如果平台不支持系统关机调用CSUDISYSHalt期望返回CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED
CSUDI_BOOL CSTC_TOOLSET_IT_Halt_0001(void)
{
	if(CSUDI_TRUE == g_bSupportSysHalt)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSHalt(),"测试用例失败"); 
		
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "没有成功实现系统关机"); 	
	}
	else
	    	CSTK_ASSERT_TRUE_FATAL(CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED == CSUDISYSHalt(),"测试用例失败"); 

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:测试参数pbReal = CSUDI_NULL时的情况
//@PRECONDITION:系统成功初始化
//@INPUT: 参数bStandby值合法
//@INPUT: 参数nStandbyTime值合法
//@INPUT: 参数pbReal = CSUDI_NULL
//@EXPECTATION: 返回非CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDISYSStandby函数期望返回值非CSUDI_SUCCESS  
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0001(void)
{
   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDISYSStandby(CSUDI_TRUE,50,CSUDI_NULL),"测试用例失败"); 

	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:测试参数nStandbyTime非法的情况
//@PRECONDITION:系统成功初始化
//@INPUT: 参数bStandby = CSUDI_TRUE
//@INPUT: 参数nStandbyTime = -1
//@INPUT: 参数pbReal非空
//@EXPECTATION: 返回非CSUDI_SUCCESS 
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDISYSStandby函数期望返回值非CSUDI_SUCCESS  
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0002(void)
{
	CSUDI_BOOL bReal;
	
   	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDISYSStandby(CSUDI_TRUE,-1,&bReal),"测试用例失败"); 
		
    CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:测试能否成功实现待机功能且能自动唤醒
//@PRECONDITION:系统成功初始化
//@INPUT: 参数bStandby = CSUDI_TRUE  
//@INPUT: 参数nStandbyTime = 20
//@INPUT: 参数pbReal非空
//@EXPECTATION: 每一步都达到预期结果
//@REMARK:手动测试
//@EXECUTIONFLOW:1、调用CSUDISYSStandby期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、如果*pbReal被置为CSUDI_FALSE，执行3至5步
//@EXECUTIONFLOW:3、检测显示屏显示是否关闭及前面板上显示是否为假待机显示状态
//@EXECUTIONFLOW:4、输入参数bStandby为 CSUDI_FALSE时调用CSUDISYSStandby能立即从假待机返回
//@EXECUTIONFLOW:5、检测前面板显示及显示屏的显示是否全部显示出来
//@EXECUTIONFLOW:6、如果*pbReal被置为CSUDI_TRUE，执行以下步骤
//@EXECUTIONFLOW:7、检测前面板显示及显示屏的显示是否是否全部关闭
//@EXECUTIONFLOW:8、期望过20秒后系统退出待机状态
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0003(void)
{
	 CSUDI_BOOL bReal;

	CSTCPrint("即将进入待机状态，成功待机后20秒后自动唤醒，请注意能否正常待机和唤醒\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_TRUE,20,&bReal),"第一步进入待机状态失败");		

	if(bReal == CSUDI_FALSE)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_FALSE,20,&bReal),"第四步退出假待机测试用例失败");		
	}	
	else
	{
		printf("\n********************测试成功******************************\n");	
	}
    	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:测试能否成功实现待机功能且能通过遥控器按键唤醒
//@PRECONDITION:系统成功初始化
//@INPUT: 参数bStandby = CSUDI_TRUE  
//@INPUT: 参数nStandbyTime = 0
//@INPUT: 参数pbReal非空
//@EXPECTATION: 每一步都达到预期结果
//@REMARK:手动测试
//@EXECUTIONFLOW:1、调用CSUDISYSStandby期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:2、如果*pbReal被置为CSUDI_FALSE，执行3至5步
//@EXECUTIONFLOW:3、检测显示屏显示是否关闭及前面板上显示是否为假待机显示状态
//@EXECUTIONFLOW:4、输入参数bStandby为 CSUDI_FALSE时调用CSUDISYSStandby能立即从假待机返回
//@EXECUTIONFLOW:5、检测前面板显示及显示屏的显示是否全部显示出来
//@EXECUTIONFLOW:6、如果*pbReal被置为CSUDI_TRUE，执行以下步骤
//@EXECUTIONFLOW:7、检测前面板显示及显示屏的显示是否是否全部关闭
//@EXECUTIONFLOW:8、按遥控器待机键，期望能够正常唤醒
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0004(void)
{
	 CSUDI_BOOL bReal;

	CSTCPrint("即将进入待机状态，成功待机后请按遥控器待机键唤醒，请注意能否正常待机和唤醒\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_TRUE,0,&bReal),"第一步进入待机状态失败");		

	if(bReal == CSUDI_FALSE)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_FALSE,60,&bReal),"第四步退出假待机测试用例失败");		
	}	
	else
	{
			
	}
    	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP: CSUDISYSStandby
//@DESCRIPTION:测试能否成功实现待机功能且能通过前面板按键唤醒
//@PRECONDITION:系统成功初始化
//@INPUT: 参数bStandby = CSUDI_TRUE  
//@INPUT: 参数nStandbyTime = 0
//@INPUT: 参数pbReal非空
//@EXPECTATION: 每一步都达到预期结果
//@REMARK:手动测试
//@EXECUTIONFLOW:1、如果前面板有待机键则执行以下步骤，否则跳过本用例
//@EXECUTIONFLOW:2、调用CSUDISYSStandby期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、如果*pbReal被置为CSUDI_FALSE，执行3至5步
//@EXECUTIONFLOW:4、检测显示屏显示是否关闭及前面板上显示是否为假待机显示状态
//@EXECUTIONFLOW:5、输入参数bStandby为 CSUDI_FALSE时调用CSUDISYSStandby能立即从假待机返回
//@EXECUTIONFLOW:6、检测前面板显示及显示屏的显示是否全部显示出来
//@EXECUTIONFLOW:7、如果*pbReal被置为CSUDI_TRUE，执行以下步骤
//@EXECUTIONFLOW:8、检测前面板显示及显示屏的显示是否是否全部关闭
//@EXECUTIONFLOW:9、按前面板待机键，期望能够正常唤醒
CSUDI_BOOL CSTC_TOOLSET_IT_Standby_0005(void)
{
	 CSUDI_BOOL bReal;

	CSTCPrint("测试待机后是否能通过前面板按键唤醒，请确认前面板是否有待机键\n");
	CSTCPrint("y: 有待机键，继续下面的测试；n: 无待机键，跳过测试\n");
	 CSTK_ASSERT_TRUE_FATAL((CSUDI_TRUE == CSTKWaitYes()), "前面板无待机键，跳过测试");

	CSTCPrint("即将进入待机状态，成功待机后请按前面板待机键唤醒，请注意能否正常待机和唤醒\n");
	CSTKWaitAnyKey();
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_TRUE,0,&bReal),"第一步进入待机状态失败");		

	if(bReal == CSUDI_FALSE)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDISYSStandby(CSUDI_FALSE,60,&bReal),"第四步退出假待机测试用例失败");		
	}	
	else
	{
			
	}
    	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

