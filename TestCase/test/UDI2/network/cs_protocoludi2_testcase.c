
/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.������������������CSTC_��������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_protocoludi2_testcase.h"

//@TESTCASEID: CSTC_CS_PROTOCOLUDI2_TEST_Init
//@CASEGROUP:  PROTOCOLInitialize
//@DESCRIPTION:��ʼ�����Ի���
CSUDI_BOOL CSTC_CS_PROTOCOLUDI2_TEST_Init(void)
{
	//�ڱ�����������ִ��ǰ����

	//CSUDIIPCFGSetIpInfo("eth0", "10.10.98.68", "255.255.255.0", "10.10.98.254");
	//CSUDIIPCFGSetDNSServer(0, "192.168.99.5", 16);
	return CSUDI_TRUE;
}


CSUDI_BOOL CSTC_CS_PROTOCOLUDI2_TEST_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

