/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_playertestcase.h"
CSUDI_BOOL CSTC_PLAYER_Init(void)
{
	//在本测试用例集执行前调用
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PLAYER_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PLAYER_MT_0001(void)
{
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_PLAYER_MT_0002(void)
{
	return CSUDI_TRUE;
}

