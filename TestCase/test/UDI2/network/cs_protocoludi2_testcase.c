
/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：CSTC_测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_protocoludi2_testcase.h"

//@TESTCASEID: CSTC_CS_PROTOCOLUDI2_TEST_Init
//@CASEGROUP:  PROTOCOLInitialize
//@DESCRIPTION:初始化测试环境
CSUDI_BOOL CSTC_CS_PROTOCOLUDI2_TEST_Init(void)
{
	//在本测试用例集执行前调用

	//CSUDIIPCFGSetIpInfo("eth0", "10.10.98.68", "255.255.255.0", "10.10.98.254");
	//CSUDIIPCFGSetDNSServer(0, "192.168.99.5", 16);
	return CSUDI_TRUE;
}


CSUDI_BOOL CSTC_CS_PROTOCOLUDI2_TEST_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

