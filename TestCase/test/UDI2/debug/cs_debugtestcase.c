/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_debugtestcase.h"
#include "udi2_os.h"

CSUDI_BOOL CSTC_DEBUG_Init(void)
{
	//在本测试用例集执行前调用
	return CSUDI_TRUE;
}
CSUDI_BOOL CSTC_DEBUG_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试输入指针为CSUDI_NULL的情况
//@PRECONDITION:平台被初始化
//@INPUT:CSUDI_NULL指针
//@EXPECTATION:在两行星号之间没有打印
//@REMARK:
//@EXECUTIONFLOW:1、调用CSDebugDefaultOutputString，输入CSUDI_NULL指针，终端应该没输出
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0001(void)
{

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(CSUDI_NULL);
	CSTCPrint("****************************\n");
	CSTCPrint("在两行星号之间没有任何打印\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入指针为CSUDI_NULL时失败");
	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试输入指针为空字符串的情况
//@PRECONDITION:平台被初始化
//@INPUT:空字符串指针
//@EXPECTATION:在两行星号之间没有打印
//@REMARK:
//@EXECUTIONFLOW:1、调用CSDebugDefaultOutputString，输入空字符串指针，终端应该没输出
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0002(void)
{
	char * pcString1 = "";
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSTCPrint("****************************\n");
	CSTCPrint("在两行星号之间没有任何打印\n");
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入空字符串时失败");
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试对 "~!@#$%^&*()_[]{}\/."";"等35 个特殊字符的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:由"~!@#$%^&*()_[]{}\/."";"等单个特殊字符组合成的字符串
//@EXPECTATION:正常输出字符串，无乱码
//@REMARK:正常工作时终端应该打印出相对应的特殊字符
//@EXECUTIONFLOW:1、调用CSDebugDefaultOutputString，输入""""""""""指针，终端应该有相应输出
//@EXECUTIONFLOW:2、调用CSDebugDefaultOutputString，输入"~~~~~~~~"指针，终端应该有相应输出
//@EXECUTIONFLOW:3、调用CSDebugDefaultOutputString，输入"!!!!!!!!"指针，终端应该有相应输出
//@EXECUTIONFLOW:4、调用CSDebugDefaultOutputString，输入"@@@@@@@@"指针，终端应该有相应输出
//@EXECUTIONFLOW:5、调用CSDebugDefaultOutputString，输入"########"指针，终端应该有相应输出
//@EXECUTIONFLOW:6、调用CSDebugDefaultOutputString，输入"$$$$$$$$"指针，终端应该有相应输出
//@EXECUTIONFLOW:7、调用CSDebugDefaultOutputString，输入"%%%%%%%%"指针，终端应该有相应输出
//@EXECUTIONFLOW:8、调用CSDebugDefaultOutputString，输入"^^^^^^^^"指针，终端应该有相应输出
//@EXECUTIONFLOW:9、调用CSDebugDefaultOutputString，输入"&&&&&&&&"指针，终端应该有相应输出
//@EXECUTIONFLOW:10、调用CSDebugDefaultOutputString，输入"********"指针，终端应该有相应输出
//@EXECUTIONFLOW:11、调用CSDebugDefaultOutputString，输入"(((((((("指针，终端应该有相应输出
//@EXECUTIONFLOW:12、调用CSDebugDefaultOutputString，输入"))))))))"指针，终端应该有相应输出
//@EXECUTIONFLOW:13、调用CSDebugDefaultOutputString，输入"||||||||"指针，终端应该有相应输出
//@EXECUTIONFLOW:14、调用CSDebugDefaultOutputString，输入"\\\\\\\\\\\\\\\\"指针，终端应该有相应输出
//@EXECUTIONFLOW:15、调用CSDebugDefaultOutputString，输入"????????"指针，终端应该有相应输出
//@EXECUTIONFLOW:16、调用CSDebugDefaultOutputString，输入"////////"指针，终端应该有相应输出
//@EXECUTIONFLOW:17、调用CSDebugDefaultOutputString，输入"<<<<<<<<"指针，终端应该有相应输出
//@EXECUTIONFLOW:18、调用CSDebugDefaultOutputString，输入",,,,,,,,"指针，终端应该有相应输出
//@EXECUTIONFLOW:19、调用CSDebugDefaultOutputString，输入">>>>>>>>"指针，终端应该有相应输出
//@EXECUTIONFLOW:20、调用CSDebugDefaultOutputString，输入"........"指针，终端应该有相应输出
//@EXECUTIONFLOW:21、调用CSDebugDefaultOutputString，输入"[[[[[[[["指针，终端应该有相应输出
//@EXECUTIONFLOW:22、调用CSDebugDefaultOutputString，输入"]]]]]]]]"指针，终端应该有相应输出
//@EXECUTIONFLOW:23、调用CSDebugDefaultOutputString，输入"::::::::"指针，终端应该有相应输出
//@EXECUTIONFLOW:24、调用CSDebugDefaultOutputString，输入";;;;;;;;"指针，终端应该有相应输出
//@EXECUTIONFLOW:25、调用CSDebugDefaultOutputString，输入"________"指针，终端应该有相应输出
//@EXECUTIONFLOW:26、调用CSDebugDefaultOutputString，输入"--------"指针，终端应该有相应输出
//@EXECUTIONFLOW:27、调用CSDebugDefaultOutputString，输入"++++++++"指针，终端应该有相应输出
//@EXECUTIONFLOW:28、调用CSDebugDefaultOutputString，输入"========"指针，终端应该有相应输出
//@EXECUTIONFLOW:29、调用CSDebugDefaultOutputString，输入"''''''''"指针，终端应该有相应输出
//@EXECUTIONFLOW:30、调用CSDebugDefaultOutputString，输入"00000000"指针，终端应该有相应输出
//@EXECUTIONFLOW:31、调用CSDebugDefaultOutputString，输入"11111111"指针，终端应该有相应输出
//@EXECUTIONFLOW:32、调用CSDebugDefaultOutputString，输入"22222222"指针，终端应该有相应输出
//@EXECUTIONFLOW:33、调用CSDebugDefaultOutputString，输入"33333333"指针，终端应该有相应输出
//@EXECUTIONFLOW:34、调用CSDebugDefaultOutputString，输入"44444444"指针，终端应该有相应输出
//@EXECUTIONFLOW:35、调用CSDebugDefaultOutputString，输入"55555555"指针，终端应该有相应输出
//@EXECUTIONFLOW:36、调用CSDebugDefaultOutputString，输入"66666666"指针，终端应该有相应输出
//@EXECUTIONFLOW:37、调用CSDebugDefaultOutputString，输入"77777777"指针，终端应该有相应输出
//@EXECUTIONFLOW:38、调用CSDebugDefaultOutputString，输入"88888888"指针，终端应该有相应输出
//@EXECUTIONFLOW:39、调用CSDebugDefaultOutputString，输入"99999999"指针，终端应该有相应输出
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

		CSTCPrint("在星号下面，应该能够显示步骤%d对应的8个特殊字符\n", i+1);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输出特殊字符时失败");
	}

	CSTK_FATAL_POINT
     
	return CSUDI_TRUE;
}


//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试CSDebugDefaultOutputString输入转义字符的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:"a\nb","a\tb"，" aa\bb","\r\r\r\r\r\r\r\r\r\"(99个\r)
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:\n 换行，\t 水平制表符,\b 退格符, \r 回车(发送0x0D,终端可能不作外理，所以要转十六进制观察)
//@REMARK: 对于\r (回车) , 发送0x0D,终端可能不作外理，所以要转十六进制观察
//@EXECUTIONFLOW:1、调用CSDebugDefaultOutputString，输入"a\nb" ,终端第一行显示a,第二行显示b
//@EXECUTIONFLOW:2、调用CSDebugDefaultOutputString，输入"a\tb" ,终端显示a,然后打一个tab 键，显示b
//@EXECUTIONFLOW:3、调用CSDebugDefaultOutputString，输入 "aa\bb",终端显示ab
//@EXECUTIONFLOW:4、调用CSDebugDefaultOutputString，输入"\r\r\r\r\r\r\r\r\r\"(99个\r),终端应该解释转义字符并发送99 个0X0D
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
	CSTCPrint("在a 与b 之间有换行\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入输义字符\\n失败");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString2);
	CSTCPrint("\n");
	CSTCPrint("在a 与b 之间有tab 键\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入输义字符\\t失败");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString3);
 	CSTCPrint("\n");
	CSTCPrint("只能看到 ab \n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入转义字符\\b失败");

	CSTCPrint("请转用可以十六进制显示的串口工具，清空窗口，按任意键继续\n");
	CSTKWaitAnyKey();
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(szBuf);
	CSTCPrint("\n");
	CSTCPrint("请勾上HEX 显示选项，使显示转为十六进制显示，你将会看到连续的99 个OD，正确？\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入转义字符\\r失败");
	
	CSTK_FATAL_POINT
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试CSDebugDefaultOutputString对"%d,%D,%b,%B,%x,%X,%f,%F,%s,%S,%o,%O"字符串的输出能力
//@PRECONDITION:平台被初始化
//@INPUT:%d,%D,%b,%B,%x,%X,%f,%F,%s,%S,abc%sabc,abc%Sabc,%o,%O,%,%%,%%%
//@EXPECTATION:按下面
//@REMARK:
//@EXECUTIONFLOW:1、调用CSDebugDefaultOutputString，输入"%d"字符串指针，终端应该显示:%d
//@EXECUTIONFLOW:2、调用CSDebugDefaultOutputString，输入"%D"字符串指针，终端应该显示:%D
//@EXECUTIONFLOW:3、调用CSDebugDefaultOutputString，输入"%b"字符串指针，终端应该显示:%b
//@EXECUTIONFLOW:4、调用CSDebugDefaultOutputString，输入"%B"字符串指针，终端应该显示:%B
//@EXECUTIONFLOW:5、调用CSDebugDefaultOutputString，输入"%x"字符串指针，终端应该显示:%x
//@EXECUTIONFLOW:6、调用CSDebugDefaultOutputString，输入"%X"字符串指针，终端应该显示:%X
//@EXECUTIONFLOW:7、调用CSDebugDefaultOutputString，输入"%f"字符串指针，终端应该显示:%f
//@EXECUTIONFLOW:8、调用CSDebugDefaultOutputString，输入"%F"字符串指针，终端应该显示:%F
//@EXECUTIONFLOW:9、调用CSDebugDefaultOutputString，输入"%s"字符串指针，终端应该显示:%s
//@EXECUTIONFLOW:10、调用CSDebugDefaultOutputString，输入"%S"字符串指针，终端应该显示:%S
//@EXECUTIONFLOW:11、调用CSDebugDefaultOutputString，输入"abc%sabc"字符串指针，终端应该显示:abc%sabc
//@EXECUTIONFLOW:12、调用CSDebugDefaultOutputString，输入"abc%Sabc"字符串指针，终端应该显示:abc%Sabc
//@EXECUTIONFLOW:13、调用CSDebugDefaultOutputString，输入"%o"字符串指针，终端应该显示:%o
//@EXECUTIONFLOW:14、调用CSDebugDefaultOutputString，输入"%O"字符串指针，终端应该显示:%O
//@EXECUTIONFLOW:15、调用CSDebugDefaultOutputString，输入"%"字符串指针，终端应该显示:%
//@EXECUTIONFLOW:16、调用CSDebugDefaultOutputString，输入"%%"字符串指针，终端应该显示:%%
//@EXECUTIONFLOW:17、调用CSDebugDefaultOutputString，输入"%%%"字符串指针，终端应该显示:%%%
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
		CSTCPrint("在星号下面，应该能够显示步骤%d对应的字符串\n", i+1);
		CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "依次输出%d,%D,%b,%B,%x,%X,%f,%F,%s,%S,abc%sabc,abc%Sabc,%o,%S,%,%%,%%%失败");	
	}
	
	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试CSDebugDefaultOutputString对空格的输出能力
//@PRECONDITION:平台被初始化
//@INPUT:" ","abc  abc"
//@EXPECTATION:空格与"abc  abc"
//@REMARK:
//@EXECUTIONFLOW:调用CSDebugDefaultOutputString，输入" ","abc abc"字符串指针，终端应该有相应显示
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0006(void)
{
	char * pcString1 = " ";
	char * pcString2 = "abc abc";
	char * pcString3 = "aa";
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSDebugDefaultOutputString(pcString3);
	CSTCPrint("\n");
	CSTCPrint("在字符串aa 前面有一空格？\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入空格时失败");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString2);
	CSTCPrint("\n");
	CSTCPrint("在星号下面，期望打印出两个abc 字符串，且中间有空格\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入字符串中含空格时失败");
	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试CSDebugDefaultOutputString对含"\0"字符串的输出能力
//@PRECONDITION:平台被初始化
//@INPUT:"abcd\0","abcd\0 123456","abc\0123456","abc\x0a 3456"
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:\012 与\X0a  应该解释为换行（八进制的12 为十进制的10 ，在ascii 中为换行）
//@EXECUTIONFLOW:1、调用CSDebugDefaultOutputString，输入"abcd\0"字符串指针，终端应该显示：abcd
//@EXECUTIONFLOW:2、调用CSDebugDefaultOutputString，输入"abcd\0 123456"字符串指针，终端应该显示:abcd
//@EXECUTIONFLOW:3、调用CSDebugDefaultOutputString，输入"abc\0123456"字符串指针，终端应该显示:第一行：abc,第二行:3456
//@EXECUTIONFLOW:4、调用CSDebugDefaultOutputString，输入"abc\x0a 3456"字符串指针，终端应该显示:第一行：abc,第二行: 3456（注意3456前有一空格）
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0007(void)
{
	char *pcString1 = "abcd\0";
	char *pcString2 = "abcd\0 123456";
	char *pcString3="abc\0123456";
	char *pcString4="abc\x0a 3456";

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSTCPrint("\n");
	CSTCPrint("在星号下面，期望打印出abcd \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入字符串最后两个字符为\"\\0\"时失败");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString2);
	CSTCPrint("\n");
	CSTCPrint("在星号下面，期望打印出abcd \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入字符串中含两个字符为\"\\0\"时失败");

	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString3);
	CSTCPrint("\n");
	CSTCPrint("在星号下面，第一行打印abc ,第二行打印3456 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入字符串中含两个字符为\"\\0\"时失败");
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString4);
	CSTCPrint("\n");
	CSTCPrint("在星号下面，第一行打印abc ,第二行打印3456 ,且3456 前有空格\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入字符串中含两个字符为\"\\x\"时失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试CSDebugDefaultOutputString对只含一个非空格字符的字符串的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:"a"
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:
//@EXECUTIONFLOW:调用CSDebugDefaultOutputString，输入"a"字符串指针，终端应该显示"a"
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0008(void)
{
	char *pcString1 = "a";
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSTCPrint("\n");
	CSTCPrint("在星号下面，期望打印出a \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入只含一个字符的字符串时失败");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试CSDebugDefaultOutputString对长度为10的字符串的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:"0123456789"
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:
//@EXECUTIONFLOW:调用CSDebugDefaultOutputString，输入"0123456789"指针，终端应该有"0123456789"输出
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0009(void)
{
	char *pcString1 = "0123456789";
	
	CSTCPrint("****************************\n");
	CSDebugDefaultOutputString(pcString1);
	CSTCPrint("\n");
	CSTCPrint("在星号下面，期望打印出0123456789 \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入只含10个字符的字符串时失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试CSDebugDefaultOutputString对长度为1K的字符串的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:1K长的字符串
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:
//@EXECUTIONFLOW:调用CSDebugDefaultOutputString，输入长度为1K的字符串指针，终端应该显示1K长的'a'
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
	CSTCPrint("在星号下面，期望打印出以9 个E 结尾的1023 长的字符a \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(),"输入1023长的'a'字符串时失败");

	CSTK_FATAL_POINT

	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试CSDebugDefaultOutputString对长度为2K的字符串的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:2K长的字符串
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:
//@EXECUTIONFLOW:调用CSDebugDefaultOutputString，输入长度为2K长的'a'字符串指针，终端应该显示2K个'b'
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
	CSTCPrint("在星号下面，期望打印出以9 个E 结尾的2047长的字符b \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入2047长的'b'字符串时失败");

	CSTK_FATAL_POINT
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试CSDebugDefaultOutputString对3 K 长字符串的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:长度为3K的超长字符串
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:
//@EXECUTIONFLOW:调用CSDebugDefaultOutputString，输出长度为3 k 的字符串
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
	CSTCPrint("在星号下面，期望打印出以9 个E 结尾的3071 长的字符c \n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "输入3071长的'b'字符串时失败");

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
//@DESCRIPTION:测试3个线程同时调用CSDebugDefaultOutputString时的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:3个线程同时调用的CSDebugDefaultOutputString，分别不断输出"00000000"，"11111111"，"22222222"，分别输出一千次
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:
//@EXECUTIONFLOW:1、建立3个线程
//@EXECUTIONFLOW:2、使线程0不断输出"00000000"，线程1不断输出"11111111"，线程2不断输出"22222222"
//@EXECUTIONFLOW:3、终端应该不断输出"000000","010101","020202",字符串次序不定，但不会被打断
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0013(void)
{
	int i;
	CSUDI_HANDLE  ahThreads[3];

	CSTCPrint("****************************\n");

	for(i=0; i<3; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate(pszName[i], (i*20)%256, 4096, DebugMutilThreadTest, (void*)i , &ahThreads[i]), "创建任务失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != ahThreads[i], "创建任务失败");
	}

	CSUDIOSThreadSleep(20000);
	CSTCPrint("\n");
	CSTCPrint("在星号下面，三个字符串00000000 、11111111 、22222222 各被 随机地打印一千次 ，要求不能出现字符串间相互打断的情况\n");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "三个任务同时调用CSDebugDefaultOutputString时失败");

	CSTK_FATAL_POINT
		
	for(i=0; i<3; i++)
	{
		if(ahThreads[i]!= CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(ahThreads[i]),  "删除任务失败");
			ahThreads[i] = CSUDI_NULL;
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试10个线程同时调用CSDebugDefaultOutputString时的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:9个线程同时调用的CSDebugDefaultOutputString，分别不断输出"000000"~"090909"，持续1000次
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:
//@EXECUTIONFLOW:1、建立10个线程
//@EXECUTIONFLOW:2、使线程0输出"00000000" 1 千次，线程1不断输出"11111111"1 千次，线程2 输出"22222222”1 千次……线程9 输出"99999999"1 千次
//@EXECUTIONFLOW:3、终端应该不断输出"00000000"~"99999999"，次序不定
CSUDI_BOOL CSTC_DEBUG_IT_OutputString_0014(void)
{
	int i;
	CSUDI_HANDLE  ahThreads[10];
	CSTCPrint("****************************\n");

	for(i=0; i<10; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate(pszName[i], (i*20)%256, 4096, DebugMutilThreadTest, (void*)i , &ahThreads[i]), "创建任务失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != ahThreads[i], "创建任务失败");
	}

	CSUDIOSThreadSleep(30000);
	
	CSTCPrint("\n");
	CSTCPrint("在星号下面，00000000~99999999   九个字符串各被随机地打印1 千次，要求不能出现字符串间相互打断的情况\n");
 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "10个任务同时调用CSDebugDefaultOutputString时失败");

	CSTK_FATAL_POINT

	for(i=0; i<10; i++)
	{
		if(ahThreads[i]!=CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(ahThreads[i]), "删除任务失败");
			ahThreads[i] = CSUDI_NULL;
		}
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSDebugDefaultOutputString 
//@DESCRIPTION:测试100个线程同时调用CSDebugDefaultOutputString时的输出情况
//@PRECONDITION:平台被初始化
//@INPUT:100个线程同时调用的CSDebugDefaultOutputString，分别不断输出"00000000"~"9999999999999999"，持续1千次
//@EXPECTATION:正常输出，不能死机，不能乱码
//@REMARK:将线程号重复八次拼接起来就是该线程要输出的字符串
//@EXECUTIONFLOW:1、建立100个线程
//@EXECUTIONFLOW:2、使线程0不断输出"00000000"，线程1不断输出"11111111"，线程10不断输出"1010101010101010"……线程99不断输出"9999999999999999"持续持续1千次
//@EXECUTIONFLOW:3、终端应该不断输出"00000000"～"9999999999999999"，次序不定
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadCreate(bufName[i], (i*20)%256, 4096, DebugMutilThreadTest, (void*)i , &ahThreads[i]), "创建任务失败");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_NULL != ahThreads[i], "创建任务失败");
	}

	CSUDIOSThreadSleep(160000);

	CSTCPrint("\n");
	CSTCPrint("在星号下面，00000000~9999999999999999   连续8个相同字符为1个字符串、共100款字符串分别打印1 千次，要求不能出现字符串间相互打断的情况\n");
 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "100个任务同时调用CSDebugDefaultOutputString时失败");

	CSTK_FATAL_POINT

	for(i=0; i<100; i++)
	{
		if(ahThreads[i]!= CSUDI_NULL)
		{
			CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIOSThreadDestroy(ahThreads[i]), "删除任务失败");
			ahThreads[i] = CSUDI_NULL;
		}
	}

	return CSUDI_TRUE;
}

