/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_otatestcase.h"
#include "udi2_ota.h"

//OTA规范类型枚举定义
typedef enum 
{
	OTA_STANDARD_TYPE_UNKNOW,
	OTA_STANDARD_TYPE_NDS,		//NDS OTA
	OTA_STANDARD_TYPE_NOR,		//普通OTA
	OTA_STANDARD_TYPE_IRDETO		//Irdeto OTA
}OtaStandardType_E;

static OtaStandardType_E g_eOtaStandard = OTA_STANDARD_TYPE_UNKNOW;

CSUDI_BOOL CSTC_OTA_Init(void)
{
	char acBuf[5];
	int nValue = 0;
	
	memset(acBuf, '\0', sizeof(acBuf));

	CSTKGetConfigInfo("OTA", "OTA_STANDDARD_TYPE", acBuf, sizeof(acBuf));
	if(acBuf[0] != '\0')
	{
		nValue = CSTKGetIntFromStr(acBuf,10);
	}

	CSTCPrint("OTA standard type is %d(1:NDS, 2:Normal, 3:Irdeto, other:Unknow)\n", nValue);
	
	switch (nValue)
	{
		case 1:
		{
			g_eOtaStandard = OTA_STANDARD_TYPE_NDS;
			break;
		}
		case 2:
		{
			g_eOtaStandard = OTA_STANDARD_TYPE_NOR;
			break;
		}
		case 3:
		{
			g_eOtaStandard = OTA_STANDARD_TYPE_IRDETO;
			break;
		}
		default:
		{
			g_eOtaStandard = OTA_STANDARD_TYPE_UNKNOW;
			break;
		}
	}

	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_OTA_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIOTAGetInfo
//@DESCRIPTION:测试调用CSUDIOTAGetInfo获取CSUDIOTAInfoType_E定义的所有OTA信息的情况
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:1、CSUDIOTAInfoType_E定义的所有OTA信息合法枚举值
//@INPUT:2、value,length均为合法值
//@EXPECTATION: 获取所有指定OTA信息必须有成功的和不成功的两种
//@EXECUTIONFLOW:1、调用CSUDIOTAGetInfo依次获取NDS OTA信息，记录返回值情况
//@EXECUTIONFLOW:2、调用CSUDIOTAGetInfo依次获取普通 OTA信息，记录返回值情况
//@EXECUTIONFLOW:3、调用CSUDIOTAGetInfo依次获取Irdeto OTA信息，记录返回值情况
//@EXECUTIONFLOW:4、判断步骤1~3是否有成功的情况，要求必须有
//@EXECUTIONFLOW:5、判断步骤1~3是否有不成功的情况，要求必须有
CSUDI_BOOL CSTC_OTA_IT_GetInfo_0001(void)
{
	int i = 0;
	int nValue = 0;
	CSUDI_UINT8 uValue = 0;
	CSUDI_BOOL bValue = CSUDI_FALSE;
	CSUDI_Error_Code eRet = CSUDI_FAILURE;
	CSUDI_BOOL bNdsRet = CSUDI_FALSE;
	CSUDI_BOOL bNorRet = CSUDI_FALSE;
	CSUDI_BOOL bIrdetoRet = CSUDI_FALSE;

	for (i=EM_UDIOTA_NDS_BCA; i<=EM_UDIOTA_NDS_STANDBY_INFO; i++)
	{
		if (i == EM_UDIOTA_NDS_STANDBY_INFO)
		{
			bValue = CSUDI_FALSE;
			eRet = CSUDIOTAGetInfo(i, &bValue, sizeof(CSUDI_UINT8));
		}
		uValue = 0;
		eRet = CSUDIOTAGetInfo(i, &uValue, sizeof(CSUDI_UINT8));
		CSTCPrint("OTA Info type is 0x%x, eRet = 0x%x\n", i, eRet);
		
		if (CSUDI_SUCCESS == eRet)
		{
			bNdsRet = CSUDI_TRUE;
		}
	}

	for (i=EM_UDIOTA_NOR_MANUFACTURER_CODE; i<=EM_UDIOTA_NOR_HARDWARE_BATCHNO; i++)
	{
		nValue = 0;
		eRet = CSUDIOTAGetInfo(i, &nValue, sizeof(int));
		CSTCPrint("OTA Info type is 0x%x, eRet = 0x%x\n", i, eRet);
		
		if (CSUDI_SUCCESS == eRet)
		{
			bNorRet = CSUDI_TRUE;
		}
	}

	for (i=EM_UDIOTA_IRDETO_CPCB; i<=EM_UDIOTA_IRDETO_KEYSTATUS; i++)
	{
		uValue = 0;
		eRet = CSUDIOTAGetInfo(i, &uValue, sizeof(CSUDI_UINT8));
		CSTCPrint("OTA Info type is 0x%x, eRet = 0x%x\n", i, eRet);
		
		if (CSUDI_SUCCESS == eRet)
		{
			bIrdetoRet = CSUDI_TRUE;
		}
	}

	CSTK_ASSERT_TRUE_FATAL((bNdsRet || bNorRet || bIrdetoRet), "获取所有OTA信息均返回失败");
	CSTK_ASSERT_TRUE_FATAL((CSUDI_FALSE == (bNdsRet && bNorRet && bIrdetoRet)), "特定OTA规范下获取所有OTA信息应该有不成功的情况");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OTA_IT_SetInfo_0001
//@CASEGROUP:CSUDIOTASetInfo
//@DESCRIPTION:测试调用CSUDIOTASetInfo设置OTA信息的情况，这里只对NDS规范的进行测试
//@REMARK:目前只针对NDS规范进行测试，其他规范要求按照项目具体需求实现
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:1、EM_UDIOTA_NDS_FDCA，EM_UDIOTA_NDS_STANDBY_INFO
//@INPUT:2、其他参数均取合法值
//@EXPECTATION: 调用CSUDIOTASetInfo设置FDCA, STANDBY_INFO能够返回成功
//@EXECUTIONFLOW:1、判断是否为NDS规范，如果不是则用例直接返回失败，否则继续以下步骤
//@EXECUTIONFLOW:2、调用CSUDIOTASetInfo设置合法的FDCA信息，期望返回成功
//@EXECUTIONFLOW:3、调用CSUDIOTASetInfo设置合法的STANDBY_INFO信息，期望返回成功
CSUDI_BOOL CSTC_OTA_IT_SetInfo_0001(void)
{
	if (g_eOtaStandard == OTA_STANDARD_TYPE_NDS)
	{
		CSUDI_UINT8 uFDCABuf[32] = {0};
		CSUDI_BOOL bStandby = CSUDI_FALSE;

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOTASetInfo(EM_UDIOTA_NDS_FDCA, (void*)uFDCABuf, sizeof(uFDCABuf))),
				"NDS规范设置FDCA失败");

		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOTASetInfo(EM_UDIOTA_NDS_STANDBY_INFO, (void*)&bStandby, sizeof(CSUDI_BOOL))),
				"NDS规范设置STANDBY_INFO失败");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "非NDS规范暂不测试");
	}

	CSTK_FATAL_POINT;

	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OTA_IT_SetOta_0001
//@CASEGROUP:CSUDIOTASetOta
//@DESCRIPTION:测试调用CSUDIOTASetOta取消升级的情况，这里不测试参数传有效字符串的情况
//@REMARK:pUrl字符串的格式因项目差异会有所不同，要求根据具体项目需求实现，这里不对NDS规范进行测试
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:1、CSUDI_NULL
//@EXPECTATION: 调用CSUDIOTASetOta(CSUDI_NULL)取消升级能够返回成功
//@EXECUTIONFLOW:1、判断是否为NDS规范，如果是则用例直接返回失败，否则继续以下步骤
//@EXECUTIONFLOW:2、调用CSUDIOTASetOta(CSUDI_NULL)，期望返回成功
CSUDI_BOOL CSTC_OTA_IT_SetOta_0001(void)
{
	if (g_eOtaStandard != OTA_STANDARD_TYPE_NDS)
	{
		CSTK_ASSERT_TRUE_FATAL(
				(CSUDI_SUCCESS == CSUDIOTASetOta(CSUDI_NULL)), 
				"取消升级失败");
	}
	else
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE, "NDS规范暂不测试");
	}

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

void iOTAWIFICakllback(CSUDIOTAWIFIMsg_E eMsg, void *pvMsgData, void *pvUserData)
{
	CSTCPrint("Recive msg : %d\n", eMsg);
	
	return;
}

//@TESTCASEID:CSTC_OTA_IT_WIFIAddDelCallback_0001
//@CASEGROUP:CSUDIOTAWIFIAddCallback,CSUDIOTAWIFIDelCallback
//@DESCRIPTION:测试能否正常添加和删除WIFI升级回调
//@REMARK:这里只测试添加删除回调能否返回成功，不测试上抛消息的正确性
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:合法参数
//@EXPECTATION: 调用CSUDIOTAWIFIAddCallback,CSUDIOTAWIFIDelCallback都能够返回成功
//@EXECUTIONFLOW:1、调用CSUDIOTAWIFIAddCallback添加回调，期望返回成功
//@EXECUTIONFLOW:2、调用CSUDIOTAWIFIDelCallback删除回调，期望返回成功
//@EXECUTIONFLOW:3、再次调用CSUDIOTAWIFIDelCallback删除已删除的回调，期望返回失败
CSUDI_BOOL CSTC_OTA_IT_WIFIAddDelCallback_0001(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOTAWIFIAddCallback(iOTAWIFICakllback, CSUDI_NULL)), 
			"注册WIFI升级回调失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS == CSUDIOTAWIFIDelCallback(iOTAWIFICakllback, CSUDI_NULL)), 
			"删除已注册的WIFI升级回调失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIOTAWIFIDelCallback(iOTAWIFICakllback, CSUDI_NULL)), 
			"删除已已删除的WIFI升级回调应该失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

//@TESTCASEID:CSTC_OTA_IT_WIFIAddDelCallback_0002
//@CASEGROUP:CSUDIOTAWIFIAddCallback,CSUDIOTAWIFIDelCallback
//@DESCRIPTION:测试添加和删除WIFI升级回调时传入非法参数的情况
//@PRECONDITION:平台UDI已经成功初始化
//@INPUT:非法参数
//@EXPECTATION: 调用CSUDIOTAWIFIAddCallback,CSUDIOTAWIFIDelCallback都能够返回失败
//@EXECUTIONFLOW:1、调用CSUDIOTAWIFIAddCallback(CSUDI_NULL, CSUDI_NULL)，期望返回失败
//@EXECUTIONFLOW:2、调用CSUDIOTAWIFIDelCallback(CSUDI_NULL, CSUDI_NULL)，期望返回失败
CSUDI_BOOL CSTC_OTA_IT_WIFIAddDelCallback_0002(void)
{
	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIOTAWIFIAddCallback(CSUDI_NULL, CSUDI_NULL)), 
			"传入非法参数注册WIFI升级回调应该失败");

	CSTK_ASSERT_TRUE_FATAL(
			(CSUDI_SUCCESS != CSUDIOTAWIFIDelCallback(CSUDI_NULL, CSUDI_NULL)), 
			"传入非法参数删除WIFI升级回调应该失败");

	CSTK_FATAL_POINT;
	
	return CSUDI_TRUE;
}

