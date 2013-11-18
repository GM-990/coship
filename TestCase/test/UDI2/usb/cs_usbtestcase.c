/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_usbtestcase.h"
#include "udi2_usb.h"

unsigned int g_nUSBCount = 0;

BOOL CSTC_USB_Init(void)
{
	//在本测试用例集执行前调用
	CSUDI_BOOL bRet = CSUDI_FALSE;
	char acCfgBuf[32];

	memset(acCfgBuf, '\0', sizeof(acCfgBuf));
	if(CSTKGetConfigInfo("USB", "CS_USBUDI2_USB_COUNT", acCfgBuf, sizeof(acCfgBuf)) == CS_TK_CONFIG_SUCCESS)
	{
		g_nUSBCount = CSTKGetIntFromStr(acCfgBuf, 10);
		bRet = CSUDI_TRUE;
	}
	else
	{
		CSTCPrint("请检查配置文件中是否已经包含[USB]段、CS_USBUDI2_USB_COUNT是否已正确配置\n");
	}
	
	return bRet;
}

BOOL CSTC_USB_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIUSBGetCount   
//@DESCRIPTION:测试获取当前插入的USB 设备个数的正确性
//@PRECONDITION:
//@INPUT:合法参数
//@EXPECTATION:获取到的当前插入的USB 设备个数与实际情况一致
//@EXECUTIONFLOW:1、通过配置文件中的描述判断平台支持的USB接口最大个数是否0，如果为0测试用例不通过并结束本用例测试，否则执行步骤2
//@EXECUTIONFLOW:2、提示测试者插入指定个数的USB设备，如果未准备好则结束测试
//@EXECUTIONFLOW:3、调用CSUDIUSBGetCount获取当前插入的USB设备个数，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:4、步骤3获取到的个数和当前实际个数相比较，应该相等
//@EXECUTIONFLOW:5、重复2~4步，循环次数为配置文件中配置的平台支持的最大USB接口个数
BOOL CSTC_USB_IT_GetCount_0001(void)
{ 
	unsigned int uUSBCount = 0;
	unsigned int uTestCount = 0;
	unsigned int i = 0;

	CSTK_ASSERT_TRUE_FATAL(g_nUSBCount > 0, "该平台不支持任何USB设备");

	for (i=0; i<=g_nUSBCount; i++)
	{
		CSTCPrint("请确保机顶盒已插入且只有 %d 个USB设备\n", i);
		CSTCPrint("已准备好:y；未准备好:n\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "未准备好指定数量USB设备");

		uTestCount = i;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIUSBGetCount(&uUSBCount), "获取当前插入的USB 设备个数失败");
		CSTK_ASSERT_TRUE_FATAL(uUSBCount == uTestCount, "得到的USB个数与实际情况不相等");
	}

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIUSBGetCount   
//@DESCRIPTION:CSUDIUSBGetCount接口非法参数测试
//@PRECONDITION:
//@INPUT:非法参数CSUDI_NULL
//@EXPECTATION:传入非法参数应该返回CSUDIUSB_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:1、调用CSUDIUSBGetCount(CSUDI_NULL)，应该返回CSUDIUSB_ERROR_BAD_PARAMETER
BOOL CSTC_USB_IT_GetCount_0002(void)
{ 
	CSTK_ASSERT_TRUE_FATAL(CSUDIUSB_ERROR_BAD_PARAMETER == CSUDIUSBGetCount(CSUDI_NULL), "传入非法参数，应该返回参数异常错误");

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIUSBGetInfo   
//@DESCRIPTION:测试获取指定索引的USB设备信息的正确性
//@PRECONDITION:
//@INPUT:1、合法USB设备索引
//@INPUT:2、CSUDIUSBInfo_S指针&sUsbInfo
//@EXPECTATION:获取到的USB设备信息与实际情况一致
//@EXECUTIONFLOW:1、提示测试者插入和配置文件中描述的平台支持的最大USB接口个数相等的USB设备，如果未准备好则结束测试
//@EXECUTIONFLOW:2、调用CSUDIUSBGetCount获取已插入USB设备的个数，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、判断步骤2获取到的个数，期望与实际情况一致
//@EXECUTIONFLOW:4、调用CSUDIUSBGetInfo获取合法索引的USB设备信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:5、判断sUsbInfo.m_bIsUsed == CSUDI_TRUE，期望成功
//@EXECUTIONFLOW:6、测试者判断获取到的USB设备类型信息是否正确
//@EXECUTIONFLOW:7、测试者判断获取到的USB设备描述信息是否正确
//@EXECUTIONFLOW:8、针对0~count-1范围内的所有合法索引执行步骤4~7
BOOL CSTC_USB_IT_GetInfo_0001(void)
{
	unsigned int uUSBCount = 0;
	unsigned int i = 0;
	CSUDIUSBInfo_S sUSBInfo;

	CSTCPrint("请确保机顶盒已插入且只有 %d 个USB存储设备\n", g_nUSBCount);
	CSTCPrint("已准备好:y；未准备好:n\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "未准备好指定数量USB设备");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIUSBGetCount(&uUSBCount), "获取已插入的USB设备个数失败");

	CSTK_ASSERT_TRUE_FATAL(uUSBCount == g_nUSBCount, "得到的USB个数与实际情况不相等");

	for (i=0; i<uUSBCount; i++)
	{
		CSTCPrint("获取index为%d的USB存储设备信息\n", i);
		
		memset(&sUSBInfo, 0, sizeof(sUSBInfo));
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIUSBGetInfo(i, &sUSBInfo), "获取指定合法索引的USB信息失败");

		CSTK_ASSERT_TRUE_FATAL(sUSBInfo.m_bIsUsed == CSUDI_TRUE, "获取到的USB设备信息错误");

		CSTCPrint("获取到的USB设备类型信息为: %s\n", sUSBInfo.m_szUSBType);
		CSTCPrint("类型信息与实际情况一致:y；与实际情况不符:n\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "获取到的USB设备类型信息错误");

		CSTCPrint("获取到的USB设备描述信息为: \n%s\n", sUSBInfo.m_szUSBDescription);
		CSTCPrint("描述信息与实际情况一致:y；与实际情况不符:n\n");
		CSTCPrint("(请注意:可以与USB存储设备内置文件描述进行比较，通常在/proc/scsi/usb-storage/目录下)\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "获取到的USB设备描述信息错误");
	}

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIUSBGetInfo   
//@DESCRIPTION:CSUDIUSBGetInfo接口非法参数测试
//@PRECONDITION:
//@INPUT:1、index为count+1
//@INPUT:2、psUsbInfo为CSUDI_NULL
//@EXPECTATION:传入非法参数能够正确返回对应错误代码
//@EXECUTIONFLOW:1、提示测试者插入指定个数的USB设备，如果未准备好则结束测试
//@EXECUTIONFLOW:2、调用CSUDIUSBGetInfo，index取步骤2插入的USB设备个数加1，psUsbInfo取合法值，期望返回CSUDIUSB_ERROR_INVALID_DEVICE_ID
//@EXECUTIONFLOW:3、在机顶盒已插入一个或以上个数的USB设备后，调用CSUDIUSBGetInfo，index取步骤2插入的USB设备个数减1，psUsbInfo取CSUDI_NULL，期望返回CSUDIUSB_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW:4、重复1~3步，循环次数为配置文件中配置的平台支持的最大USB接口个数
BOOL CSTC_USB_IT_GetInfo_0002(void)
{
	unsigned int i = 0;
	CSUDIUSBInfo_S stUsbInfo;

	memset(&stUsbInfo, 0, sizeof(stUsbInfo));

	for (i=0; i<=g_nUSBCount; i++)
	{
		CSTCPrint("请确保机顶盒已插入且只有 %d 个USB设备\n", i);
		CSTCPrint("已准备好:y；未准备好:n\n");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "未准备好指定数量USB设备");

		memset(&stUsbInfo, 0, sizeof(stUsbInfo));
		CSTK_ASSERT_TRUE_FATAL(CSUDIUSB_ERROR_INVALID_DEVICE_ID == CSUDIUSBGetInfo(i, &stUsbInfo), "传入索引值count+1，应该返回CSUDIUSB_ERROR_INVALID_DEVICE_ID");

		if (i > 0)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDIUSB_ERROR_BAD_PARAMETER == CSUDIUSBGetInfo(i-1, CSUDI_NULL), "psUsbInfo传CSUDI_NULL，应该返回CSUDIUSB_ERROR_BAD_PARAMETER");
		}
	}
	
	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}


