/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include "cs_debugtestcase.h"
#include "udi2_os.h"

CSUDI_BOOL CSTC_DEBUG_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	return CSUDI_TRUE;
}
CSUDI_BOOL CSTC_DEBUG_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:��������ָ��ΪCSUDI_NULL�����
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:CSUDI_NULLָ��
//@EXPECTATION:�������Ǻ�֮��û�д�ӡ
//@REMARK:
//@EXECUTIONFLOW:1������CSDebugDefaultOutputString������CSUDI_NULLָ�룬�ն�Ӧ��û���
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0001(void)
{

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(CSUDI_NULL);
	CSTCPrint("****************************\n");
	CSTCPrint("�������Ǻ�֮��û���κδ�ӡ\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����ָ��ΪCSUDI_NULLʱʧ��");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:��������ָ��Ϊ���ַ��������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:���ַ���ָ��
//@EXPECTATION:�������Ǻ�֮��û�д�ӡ
//@REMARK:
//@EXECUTIONFLOW:1������CSDebugDefaultOutputString��������ַ���ָ�룬�ն�Ӧ��û���
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0002(void)
{
	char * pcString1 = "";
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSTCPrint("****************************\n");
	CSTCPrint("�������Ǻ�֮��û���κδ�ӡ\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "������ַ���ʱʧ��");
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:���Զ� "~!@#$%^&*()_[]{}\/."";"��35 �������ַ���������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:��"~!@#$%^&*()_[]{}\/."";"�ȵ��������ַ���ϳɵ��ַ���
//@EXPECTATION:��������ַ�����������
//@REMARK:��������ʱ�ն�Ӧ�ô�ӡ�����Ӧ�������ַ�
//@EXECUTIONFLOW:1������CSDebugDefaultOutputString������""""""""""ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:2������CSDebugDefaultOutputString������"~~~~~~~~"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:3������CSDebugDefaultOutputString������"!!!!!!!!"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:4������CSDebugDefaultOutputString������"@@@@@@@@"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:5������CSDebugDefaultOutputString������"########"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:6������CSDebugDefaultOutputString������"$$$$$$$$"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:7������CSDebugDefaultOutputString������"%%%%%%%%"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:8������CSDebugDefaultOutputString������"^^^^^^^^"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:9������CSDebugDefaultOutputString������"&&&&&&&&"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:10������CSDebugDefaultOutputString������"********"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:11������CSDebugDefaultOutputString������"(((((((("ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:12������CSDebugDefaultOutputString������"))))))))"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:13������CSDebugDefaultOutputString������"||||||||"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:14������CSDebugDefaultOutputString������"\\\\\\\\\\\\\\\\"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:15������CSDebugDefaultOutputString������"????????"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:16������CSDebugDefaultOutputString������"////////"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:17������CSDebugDefaultOutputString������"<<<<<<<<"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:18������CSDebugDefaultOutputString������",,,,,,,,"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:19������CSDebugDefaultOutputString������">>>>>>>>"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:20������CSDebugDefaultOutputString������"........"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:21������CSDebugDefaultOutputString������"[[[[[[[["ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:22������CSDebugDefaultOutputString������"]]]]]]]]"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:23������CSDebugDefaultOutputString������"::::::::"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:24������CSDebugDefaultOutputString������";;;;;;;;"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:25������CSDebugDefaultOutputString������"________"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:26������CSDebugDefaultOutputString������"--------"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:27������CSDebugDefaultOutputString������"++++++++"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:28������CSDebugDefaultOutputString������"========"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:29������CSDebugDefaultOutputString������"''''''''"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:30������CSDebugDefaultOutputString������"00000000"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:31������CSDebugDefaultOutputString������"11111111"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:32������CSDebugDefaultOutputString������"22222222"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:33������CSDebugDefaultOutputString������"33333333"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:34������CSDebugDefaultOutputString������"44444444"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:35������CSDebugDefaultOutputString������"55555555"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:36������CSDebugDefaultOutputString������"66666666"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:37������CSDebugDefaultOutputString������"77777777"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:38������CSDebugDefaultOutputString������"88888888"ָ�룬�ն�Ӧ������Ӧ���
//@EXECUTIONFLOW:39������CSDebugDefaultOutputString������"99999999"ָ�룬�ն�Ӧ������Ӧ���
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0003(void)
{
	int i;
	char * apcString1[] = {"\"\"\"\"\"\"\"\"", 	/*1*/
						"~~~~~~~~", 	/*2*/
						"!!!!!!!!",		/*3*/
						"@@@@@@@@",	/*4*/
						"########",		/*5*/
						"$$$$$$$$",		/*6*/
						"%%%%%%%%",	/*7*/
						"^^^^^^^^",		/*8*/
						"&&&&&&&&",		/*9*/
						"********",		/*10*/
						"((((((((",			/*11*/
						"))))))))",			/*12*/
						"||||||||",		/*13*/
						"\\\\\\\\\\\\\\\\",	/*14*/
						"????????",		/*15*/
						"////////",		/*16*/
						"<<<<<<<<",		/*17*/
						",,,,,,,,",			/*18*/
						">>>>>>>>",		/*19*/
						"........",			/*20*/
						"[[[[[[[[",			/*21*/
						"]]]]]]]]",			/*22*/
						"::::::::",			/*23*/
						";;;;;;;;",			/*24*/
						"________",		/*25*/
						"--------",		/*26*/
						"++++++++",		/*27*/
						"========",		/*28*/
						"''''''''",			/*29*/
						"00000000",		/*30*/
						"11111111",		/*31*/
						"22222222",		/*32*/
						"33333333",		/*33*/
						"44444444",		/*34*/
						"55555555",		/*35*/
						"66666666",		/*36*/
						"77777777",		/*37*/
						"88888888",		/*38*/
						"99999999"};		/*39*/

	CSTCPrint("****************************\n");

	for (i=0; i<sizeof(apcString1)/sizeof(apcString1[0]); i++)
	{
		CSTCPrint("****************************\n");
		CSDebugDefaultOutputString(apcString1[i]);
		CSTCPrint("\n");

		CSTCPrint("���Ǻ����棬Ӧ���ܹ���ʾ����%d��Ӧ��8�������ַ�\n", i+1);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "��������ַ�ʱʧ��");
	}

	CSTK_FATAL_POINT
     
	return CSUDI_TRUE;
}


//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����CSDebugDefaultOutputString����ת���ַ���������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:"a\nb","a\tb"��" aa\bb","\r\r\r\r\r\r\r\r\r\"(99��\r)
//@EXPECTATION:���������������������������
//@REMARK:\n ���У�\t ˮƽ�Ʊ��,\b �˸��, \r �س�(����0x0D,�ն˿��ܲ�����������Ҫתʮ�����ƹ۲�)
//@REMARK: ����\r (�س�) , ����0x0D,�ն˿��ܲ�����������Ҫתʮ�����ƹ۲�
//@EXECUTIONFLOW:1������CSDebugDefaultOutputString������"a\nb" ,�ն˵�һ����ʾa,�ڶ�����ʾb
//@EXECUTIONFLOW:2������CSDebugDefaultOutputString������"a\tb" ,�ն���ʾa,Ȼ���һ��tab ������ʾb
//@EXECUTIONFLOW:3������CSDebugDefaultOutputString������ "aa\bb",�ն���ʾab
//@EXECUTIONFLOW:4������CSDebugDefaultOutputString������"\r\r\r\r\r\r\r\r\r\"(99��\r),�ն�Ӧ�ý���ת���ַ�������99 ��0X0D
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0004(void)
{
	char * pcString1 = "a\nb";
	char * pcString2 = "a\tb";
	char * pcString3 = "aa\bb";

	char szBuf[100];
	memset(szBuf, '\r', sizeof(szBuf));
	szBuf[sizeof(szBuf)-1]='\0';
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
 	CSTCPrint("\n");
	CSTCPrint("��a ��b ֮���л���\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���������ַ�\\nʧ��");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString2);
	CSTCPrint("\n");
	CSTCPrint("��a ��b ֮����tab ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���������ַ�\\tʧ��");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString3);
 	CSTCPrint("\n");
	CSTCPrint("ֻ�ܿ��� ab \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����ת���ַ�\\bʧ��");

	CSTCPrint("��ת�ÿ���ʮ��������ʾ�Ĵ��ڹ��ߣ���մ��ڣ������������\n");
	CSTKWaitAnyKey();
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(szBuf);
	CSTCPrint("\n");
	CSTCPrint("�빴��HEX ��ʾѡ�ʹ��ʾתΪʮ��������ʾ���㽫�ῴ��������99 ��OD����ȷ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����ת���ַ�\\rʧ��");
	
	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����CSDebugDefaultOutputString��"%d,%D,%b,%B,%x,%X,%f,%F,%s,%S,%o,%O"�ַ������������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:%d,%D,%b,%B,%x,%X,%f,%F,%s,%S,abc%sabc,abc%Sabc,%o,%O,%,%%,%%%
//@EXPECTATION:������
//@REMARK:
//@EXECUTIONFLOW:1������CSDebugDefaultOutputString������"%d"�ַ���ָ�룬�ն�Ӧ����ʾ:%d
//@EXECUTIONFLOW:2������CSDebugDefaultOutputString������"%D"�ַ���ָ�룬�ն�Ӧ����ʾ:%D
//@EXECUTIONFLOW:3������CSDebugDefaultOutputString������"%b"�ַ���ָ�룬�ն�Ӧ����ʾ:%b
//@EXECUTIONFLOW:4������CSDebugDefaultOutputString������"%B"�ַ���ָ�룬�ն�Ӧ����ʾ:%B
//@EXECUTIONFLOW:5������CSDebugDefaultOutputString������"%x"�ַ���ָ�룬�ն�Ӧ����ʾ:%x
//@EXECUTIONFLOW:6������CSDebugDefaultOutputString������"%X"�ַ���ָ�룬�ն�Ӧ����ʾ:%X
//@EXECUTIONFLOW:7������CSDebugDefaultOutputString������"%f"�ַ���ָ�룬�ն�Ӧ����ʾ:%f
//@EXECUTIONFLOW:8������CSDebugDefaultOutputString������"%F"�ַ���ָ�룬�ն�Ӧ����ʾ:%F
//@EXECUTIONFLOW:9������CSDebugDefaultOutputString������"%s"�ַ���ָ�룬�ն�Ӧ����ʾ:%s
//@EXECUTIONFLOW:10������CSDebugDefaultOutputString������"%S"�ַ���ָ�룬�ն�Ӧ����ʾ:%S
//@EXECUTIONFLOW:11������CSDebugDefaultOutputString������"abc%sabc"�ַ���ָ�룬�ն�Ӧ����ʾ:abc%sabc
//@EXECUTIONFLOW:12������CSDebugDefaultOutputString������"abc%Sabc"�ַ���ָ�룬�ն�Ӧ����ʾ:abc%Sabc
//@EXECUTIONFLOW:13������CSDebugDefaultOutputString������"%o"�ַ���ָ�룬�ն�Ӧ����ʾ:%o
//@EXECUTIONFLOW:14������CSDebugDefaultOutputString������"%O"�ַ���ָ�룬�ն�Ӧ����ʾ:%O
//@EXECUTIONFLOW:15������CSDebugDefaultOutputString������"%"�ַ���ָ�룬�ն�Ӧ����ʾ:%
//@EXECUTIONFLOW:16������CSDebugDefaultOutputString������"%%"�ַ���ָ�룬�ն�Ӧ����ʾ:%%
//@EXECUTIONFLOW:17������CSDebugDefaultOutputString������"%%%"�ַ���ָ�룬�ն�Ӧ����ʾ:%%%
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0005(void)
{
	int i;
	char * apcString1[] = {"%d","%D","%b","%B","%x","%X","%f","%F","%s","%S","abc%sabc","abc%Sabc","%o","%O","%","%%","%%%"};
	
	CSTCPrint("****************************\n");

	for (i = 0; i<sizeof(apcString1)/sizeof(apcString1[0]); i++)
	{
		CSTCPrint("****************************\n");
		
		CSDebugDefaultOutputString(apcString1[i]);

		CSTCPrint("\n");
		CSTCPrint("���Ǻ����棬Ӧ���ܹ���ʾ����%d��Ӧ���ַ���\n", i+1);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�������%d,%D,%b,%B,%x,%X,%f,%F,%s,%S,abc%sabc,abc%Sabc,%o,%S,%,%%,%%%ʧ��");	
	}
	
	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����CSDebugDefaultOutputString�Կո���������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:" ","abc  abc"
//@EXPECTATION:�ո���"abc  abc"
//@REMARK:
//@EXECUTIONFLOW:����CSDebugDefaultOutputString������" ","abc abc"�ַ���ָ�룬�ն�Ӧ������Ӧ��ʾ
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0006(void)
{
	char * pcString1 = " ";
	char * pcString2 = "abc abc";
	char * pcString3 = "aa";
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSDebugDefaultOutputString(pcString3);
	CSTCPrint("\n");
	CSTCPrint("���ַ���aa ǰ����һ�ո�\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����ո�ʱʧ��");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString2);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬������ӡ������abc �ַ��������м��пո�\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�����ַ����к��ո�ʱʧ��");
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����CSDebugDefaultOutputString�Ժ�"\0"�ַ������������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:"abcd\0","abcd\0 123456","abc\0123456","abc\x0a 3456"
//@EXPECTATION:���������������������������
//@REMARK:\012 ��\X0a  Ӧ�ý���Ϊ���У��˽��Ƶ�12 Ϊʮ���Ƶ�10 ����ascii ��Ϊ���У�
//@EXECUTIONFLOW:1������CSDebugDefaultOutputString������"abcd\0"�ַ���ָ�룬�ն�Ӧ����ʾ��abcd
//@EXECUTIONFLOW:2������CSDebugDefaultOutputString������"abcd\0 123456"�ַ���ָ�룬�ն�Ӧ����ʾ:abcd
//@EXECUTIONFLOW:3������CSDebugDefaultOutputString������"abc\0123456"�ַ���ָ�룬�ն�Ӧ����ʾ:��һ�У�abc,�ڶ���:3456
//@EXECUTIONFLOW:4������CSDebugDefaultOutputString������"abc\x0a 3456"�ַ���ָ�룬�ն�Ӧ����ʾ:��һ�У�abc,�ڶ���: 3456��ע��3456ǰ��һ�ո�
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0007(void)
{
	char *pcString1 = "abcd\0";
	char *pcString2 = "abcd\0 123456";
	char *pcString3="abc\0123456";
	char *pcString4="abc\x0a 3456";

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬������ӡ��abcd \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�����ַ�����������ַ�Ϊ\"\\0\"ʱʧ��");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString2);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬������ӡ��abcd \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�����ַ����к������ַ�Ϊ\"\\0\"ʱʧ��");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString3);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬��һ�д�ӡabc ,�ڶ��д�ӡ3456 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�����ַ����к������ַ�Ϊ\"\\0\"ʱʧ��");
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString4);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬��һ�д�ӡabc ,�ڶ��д�ӡ3456 ,��3456 ǰ�пո�\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "�����ַ����к������ַ�Ϊ\"\\x\"ʱʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����CSDebugDefaultOutputString��ֻ��һ���ǿո��ַ����ַ�����������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:"a"
//@EXPECTATION:���������������������������
//@REMARK:
//@EXECUTIONFLOW:����CSDebugDefaultOutputString������"a"�ַ���ָ�룬�ն�Ӧ����ʾ"a"
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0008(void)
{
	char *pcString1 = "a";
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬������ӡ��a \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����ֻ��һ���ַ����ַ���ʱʧ��");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����CSDebugDefaultOutputString�Գ���Ϊ10���ַ�����������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:"0123456789"
//@EXPECTATION:���������������������������
//@REMARK:
//@EXECUTIONFLOW:����CSDebugDefaultOutputString������"0123456789"ָ�룬�ն�Ӧ����"0123456789"���
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0009(void)
{
	char *pcString1 = "0123456789";
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬������ӡ��0123456789 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����ֻ��10���ַ����ַ���ʱʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����CSDebugDefaultOutputString�Գ���Ϊ1K���ַ�����������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:1K�����ַ���
//@EXPECTATION:���������������������������
//@REMARK:
//@EXECUTIONFLOW:����CSDebugDefaultOutputString�����볤��Ϊ1K���ַ���ָ�룬�ն�Ӧ����ʾ1K����'a'
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0010(void)
{
	char szBuf[1024];
	int nBufLastLine;
	
	nBufLastLine=(int)(szBuf)+sizeof(szBuf)-10;
	memset(szBuf, 'a', sizeof(szBuf));
	memset((char *)nBufLastLine, 'E', 9);
	szBuf[sizeof(szBuf)-1]='\0';

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(szBuf);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬������ӡ����9 ��E ��β��1023 �����ַ�a \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"����1023����'a'�ַ���ʱʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����CSDebugDefaultOutputString�Գ���Ϊ2K���ַ�����������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:2K�����ַ���
//@EXPECTATION:���������������������������
//@REMARK:
//@EXECUTIONFLOW:����CSDebugDefaultOutputString�����볤��Ϊ2K����'a'�ַ���ָ�룬�ն�Ӧ����ʾ2K��'b'
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0011(void)
{

	char szBuf[1024*2];
	int nBufLastLine;
	
	nBufLastLine=(int)(szBuf)+sizeof(szBuf)-10;
	memset(szBuf, 'b', sizeof(szBuf));
	memset((char *)nBufLastLine, 'E', 9);
	szBuf[sizeof(szBuf)-1] = '\0';

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(szBuf);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬������ӡ����9 ��E ��β��2047�����ַ�b \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����2047����'b'�ַ���ʱʧ��");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����CSDebugDefaultOutputString��3 K ���ַ�����������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:����Ϊ3K�ĳ����ַ���
//@EXPECTATION:���������������������������
//@REMARK:
//@EXECUTIONFLOW:����CSDebugDefaultOutputString���������Ϊ3 k ���ַ���
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0012(void)
{
	char szBuf[1024*3];
	int nBufLastLine;
	
	nBufLastLine=(int)(szBuf)+sizeof(szBuf)-10;
	memset(szBuf, 'c', sizeof(szBuf));
	memset((char *)nBufLastLine, 'E', 9);
	szBuf[sizeof(szBuf)-1]='\0';

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(szBuf);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬������ӡ����9 ��E ��β��3071 �����ַ�c \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����3071����'b'�ַ���ʱʧ��");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

static char * pszName[]={	"test01",
						"test02",
						"test03",
						"test04",
						"test05",
						"test06",
						"test07",
						"test08",
						"test09",
						"test10"
						};

static void DebugMutilThreadTest(void* param)
{
	int i;
	int index = (int)param;
	char buf[20];
	sprintf(buf," %d%d%d%d%d%d%d%d ",index,index,index,index,index,index,index,index);

	for(i=0; i<1000; i++)
	{
		CSDebugDefaultOutputString(buf);
		CSUDIOSThreadSleep(5);
	}
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����3���߳�ͬʱ����CSDebugDefaultOutputStringʱ��������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:3���߳�ͬʱ���õ�CSDebugDefaultOutputString���ֱ𲻶����"00000000"��"11111111"��"22222222"���ֱ����һǧ��
//@EXPECTATION:���������������������������
//@REMARK:
//@EXECUTIONFLOW:1������3���߳�
//@EXECUTIONFLOW:2��ʹ�߳�0�������"00000000"���߳�1�������"11111111"���߳�2�������"22222222"
//@EXECUTIONFLOW:3���ն�Ӧ�ò������"000000","010101","020202",�ַ������򲻶��������ᱻ���
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0013(void)
{
	int i;
	CSUDI_HANDLE  ahThreads[3];

	CSTCPrint("****************************\n");

	for(i=0; i<3; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate(pszName[i], (i*20)%256, 4096, DebugMutilThreadTest, (void*)i , &ahThreads[i]), "��������ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != ahThreads[i], "��������ʧ��");
	}

	CSUDIOSThreadSleep(20000);
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬�����ַ���00000000 ��11111111 ��22222222 ���� ����ش�ӡһǧ�� ��Ҫ���ܳ����ַ������໥��ϵ����\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "��������ͬʱ����CSDebugDefaultOutputStringʱʧ��");

	CSTK_FATAL_POINT
		
	for(i=0; i<3; i++)
	{
		if(ahThreads[i]!= CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(ahThreads[i]),  "ɾ������ʧ��");
			ahThreads[i] = CSUDI_NULL;
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����10���߳�ͬʱ����CSDebugDefaultOutputStringʱ��������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:9���߳�ͬʱ���õ�CSDebugDefaultOutputString���ֱ𲻶����"000000"~"090909"������1000��
//@EXPECTATION:���������������������������
//@REMARK:
//@EXECUTIONFLOW:1������10���߳�
//@EXECUTIONFLOW:2��ʹ�߳�0���"00000000" 1 ǧ�Σ��߳�1�������"11111111"1 ǧ�Σ��߳�2 ���"22222222��1 ǧ�Ρ����߳�9 ���"99999999"1 ǧ��
//@EXECUTIONFLOW:3���ն�Ӧ�ò������"00000000"~"99999999"�����򲻶�
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0014(void)
{
	int i;
	CSUDI_HANDLE  ahThreads[10];
	CSTCPrint("****************************\n");

	for(i=0; i<10; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate(pszName[i], (i*20)%256, 4096, DebugMutilThreadTest, (void*)i , &ahThreads[i]), "��������ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != ahThreads[i], "��������ʧ��");
	}

	CSUDIOSThreadSleep(30000);
	
	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬00000000~99999999   �Ÿ��ַ�����������ش�ӡ1 ǧ�Σ�Ҫ���ܳ����ַ������໥��ϵ����\n");
 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "10������ͬʱ����CSDebugDefaultOutputStringʱʧ��");

	CSTK_FATAL_POINT

	for(i=0; i<10; i++)
	{
		if(ahThreads[i]!=CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(ahThreads[i]), "ɾ������ʧ��");
			ahThreads[i] = CSUDI_NULL;
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:����100���߳�ͬʱ����CSDebugDefaultOutputStringʱ��������
//@PRECONDITION:ƽ̨����ʼ��
//@INPUT:100���߳�ͬʱ���õ�CSDebugDefaultOutputString���ֱ𲻶����"00000000"~"9999999999999999"������1ǧ��
//@EXPECTATION:���������������������������
//@REMARK:���̺߳��ظ��˴�ƴ���������Ǹ��߳�Ҫ������ַ���
//@EXECUTIONFLOW:1������100���߳�
//@EXECUTIONFLOW:2��ʹ�߳�0�������"00000000"���߳�1�������"11111111"���߳�10�������"1010101010101010"�����߳�99�������"9999999999999999"��������1ǧ��
//@EXECUTIONFLOW:3���ն�Ӧ�ò������"00000000"��"9999999999999999"�����򲻶�
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0015(void)
{
	int i ;
	CSUDI_HANDLE   ahThreads[100];

	char bufName[100][5];

	CSTCPrint("****************************\n");

	for(i=0; i<100; i++)
	{
		sprintf(bufName[i],"%d",i);
	}

	for(i=0; i<100; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate(bufName[i], (i*20)%256, 4096, DebugMutilThreadTest, (void*)i , &ahThreads[i]), "��������ʧ��");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != ahThreads[i], "��������ʧ��");
	}

	CSUDIOSThreadSleep(160000);

	CSTCPrint("\n");
	CSTCPrint("���Ǻ����棬00000000~9999999999999999   ����8����ͬ�ַ�Ϊ1���ַ�������100���ַ����ֱ��ӡ1 ǧ�Σ�Ҫ���ܳ����ַ������໥��ϵ����\n");
 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "100������ͬʱ����CSDebugDefaultOutputStringʱʧ��");

	CSTK_FATAL_POINT

	for(i=0; i<100; i++)
	{
		if(ahThreads[i]!= CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(ahThreads[i]), "ɾ������ʧ��");
			ahThreads[i] = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

