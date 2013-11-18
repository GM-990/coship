/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_inputtestcase.h"
#include "udi2_os.h"
#include "cs_ir.h"

#define MAX_SUPPORTREPEAT_KEY_COUNT 100

static CSUDIInputType_E  g_eType;
 
static int g_nRemoteCallBackTime;
static int g_nPanelCallBackTime;
static int g_nRemoteUserDataSum;
static int g_nTimeCount;

static int g_nRemoteKeyCode_1 ;
static int g_eRemoteStatus_1 ;
static int g_nRemoteUserData_1;
static int g_nRemoteUserDataSum_1;

static int g_nRemoteKeyCode_2 ;
static int g_eRemoteStatus_2 ;
static int g_nRemoteUserData_2;
static int g_nRemoteUserDataSum_2;

static int g_nRemoteKeyCode_3 ;
static int g_eRemoteStatus_3 ;
static int g_nRemoteUserData_3;

static int g_nPanelKeyCode_1;
static int g_ePanelStatus_1;
static int g_nPanelUserData_1;

static int g_nPanelKeyCode_2;
static int g_ePanelStatus_2;
static int g_nPanelUserData_2;

static int g_nRemoteStatus_repeat;

//在本测试用例集执行前调用
CSUDI_BOOL CSTC_INPUT_Init(void)
{
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_INPUT_UnInit(void)
{
	//在本测试用例集执行后调用
	return CSUDI_TRUE;
}

static void iPrintKeyInfo(int nKeyCode)
{
	switch(nKeyCode)
	{
		case CSUDI_VK_POWER: {CSTCPrint("待机键(CSUDI_VK_POWER)\n"); break;}
		case CSUDI_VK_0: {CSTCPrint("数字键0(CSUDI_VK_0)\n"); break;}
		case CSUDI_VK_1: {CSTCPrint("数字键1(CSUDI_VK_1)\n"); break;}
		case CSUDI_VK_2: {CSTCPrint("数字键2(CSUDI_VK_2)\n"); break;}
		case CSUDI_VK_3: {CSTCPrint("数字键3(CSUDI_VK_2)\n"); break;}
		case CSUDI_VK_4: {CSTCPrint("数字键4(CSUDI_VK_4)\n"); break;}
		case CSUDI_VK_5: {CSTCPrint("数字键5(CSUDI_VK_5)\n"); break;}
		case CSUDI_VK_6: {CSTCPrint("数字键6(CSUDI_VK_6)\n"); break;}
		case CSUDI_VK_7: {CSTCPrint("数字键7(CSUDI_VK_7)\n"); break;}
		case CSUDI_VK_8: {CSTCPrint("数字键8(CSUDI_VK_8)\n"); break;}
		case CSUDI_VK_9: {CSTCPrint("数字键9(CSUDI_VK_9)\n"); break;}
		case CSUDI_VK_OK: {CSTCPrint("确认键(CSUDI_VK_OK)\n"); break;}
		case CSUDI_VK_TOGGLE: {CSTCPrint("返回键(CSUDI_VK_TOGGLE)\n"); break;}
		case CSUDI_VK_EXIT: {CSTCPrint("退出键(CSUDI_VK_EXIT)\n"); break;}
		case CSUDI_VK_PAGE_UP: {CSTCPrint("向上翻页(CSUDI_VK_PAGE_UP)\n"); break;}
		case CSUDI_VK_PAGE_DOWN: {CSTCPrint("向下翻页(CSUDI_VK_PAGE_DOWN)\n"); break;}
		case CSUDI_VK_LEFT: {CSTCPrint("向左键 (CSUDI_VK_LEFT)\n"); break;}
		case CSUDI_VK_UP: {CSTCPrint("向上键(CSUDI_VK_UP)\n"); break;}
		case CSUDI_VK_RIGHT: {CSTCPrint("向右键(CSUDI_VK_RIGHT)\n"); break;}
		case CSUDI_VK_DOWN: {CSTCPrint("向下键(CSUDI_VK_DOWN)\n"); break;}
		case CSUDI_VK_CHANNEL_UP: {CSTCPrint("频道增加键(CSUDI_VK_CHANNEL_UP)\n"); break;}
		case CSUDI_VK_CHANNEL_DOWN: {CSTCPrint("频道减少键(CSUDI_VK_CHANNEL_DOWN)\n"); break;}
		case CSUDI_VK_VOLUME_UP: {CSTCPrint("音量增加键(CSUDI_VK_VOLUME_UP)\n"); break;}
		case CSUDI_VK_VOLUME_DOWN: {CSTCPrint("音量减少键(CSUDI_VK_VOLUME_DOWN)\n"); break;}
		case CSUDI_VK_MENU: {CSTCPrint("菜单键(CSUDI_VK_MENU)\n"); break;}
		case CSUDI_VK_MUTE: {CSTCPrint("静音键(CSUDI_VK_MUTE)\n"); break;}
		case CSUDI_VK_GUIDE: {CSTCPrint("节目指南键(CSUDI_VK_GUIDE)\n"); break;}
		case CSUDI_VK_AUDIO: {CSTCPrint("声道键(CSUDI_VK_AUDIO)\n"); break;}
		case CSUDI_VK_COLORED_KEY_0: {CSTCPrint("红色键(CSUDI_VK_COLORED_KEY_0)\n"); break;}
		case CSUDI_VK_COLORED_KEY_1: {CSTCPrint("绿色键(CSUDI_VK_COLORED_KEY_1)\n"); break;}
		case CSUDI_VK_COLORED_KEY_2: {CSTCPrint("黄色键(CSUDI_VK_COLORED_KEY_2)\n"); break;}
		case CSUDI_VK_COLORED_KEY_3: {CSTCPrint("蓝色键(CSUDI_VK_COLORED_KEY_3)\n"); break;}
		case CSUDI_VK_PROG_LIST: {CSTCPrint("频道列表键(CSUDI_VK_PROG_LIST)\n"); break;}
		case CSUDI_VK_TV_RADIO: {CSTCPrint("电视/音频广播键(CSUDI_VK_TV_RADIO)\n"); break;}
		case CSUDI_VK_CARD_INFO: {CSTCPrint("卡信息键(CSUDI_VK_CARD_INFO)\n"); break;}
		case CSUDI_VK_MAIL_INFO: {CSTCPrint("邮件键(CSUDI_VK_MAIL_INFO)\n"); break;}
		case CSUDI_VK_VOD: {CSTCPrint("点播键(CSUDI_VK_VOD)\n"); break;}
		case CSUDI_VK_SYSSET: {CSTCPrint("系统设置键(CSUDI_VK_SYSSET)\n"); break;}
		case CSUDI_VK_HELP: {CSTCPrint("帮助键(CSUDI_VK_HELP)\n"); break;}
		case CSUDI_VK_SERV: {CSTCPrint("资讯键(CSUDI_VK_SERV)\n"); break;}
		case CSUDI_VK_FAV: {CSTCPrint("喜爱键(CSUDI_VK_FAV)\n"); break;}
		case CSUDI_VK_TV: {CSTCPrint("电视键(CSUDI_VK_TV)\n"); break;}
		case CSUDI_VK_RADIO: {CSTCPrint("广播键(CSUDI_VK_RADIO)\n"); break;}
		case CSUDI_VK_INFO: {CSTCPrint("信息键(CSUDI_VK_INFO)\n"); break;}
		case CSUDI_VK_LANGUAGE: {CSTCPrint("中/英文切换键(CSUDI_VK_LANGUAGE)\n"); break;}
		case CSUDI_VK_INPUT: {CSTCPrint("输入法按键(CSUDI_VK_INPUT)\n"); break;}
		case CSUDI_VK_REFRESH: {CSTCPrint("刷新网页 (CSUDI_VK_REFRESH)\n"); break;}
		case CSUDI_VK_FORWARD: {CSTCPrint("前进（历史网页）(CSUDI_VK_FORWARD)\n"); break;}
		case CSUDI_VK_BACK: {CSTCPrint("后退（历史网页）(CSUDI_VK_BACK)\n"); break;}
		case CSUDI_VK_BROWSER: {CSTCPrint("启动浏览器(CSUDI_VK_BROWSER)\n"); break;}
		case CSUDI_VK_RECORD: {CSTCPrint("录制键(CSUDI_VK_RECORD)\n"); break;}
		case CSUDI_VK_PLAY: {CSTCPrint("播放(CSUDI_VK_PLAY)\n"); break;}
		case CSUDI_VK_STOP: {CSTCPrint("停止播放(CSUDI_VK_STOP)\n"); break;}
		case CSUDI_VK_PAUSE: {CSTCPrint("暂停(CSUDI_VK_PAUSE)\n"); break;}
		case CSUDI_VK_RW: {CSTCPrint("快退(CSUDI_VK_RW)\n"); break;}
		case CSUDI_VK_FF: {CSTCPrint("快进(CSUDI_VK_FF)\n"); break;}
		case CSUDI_VK_POUND: {CSTCPrint("\"#\"键(CSUDI_VK_POUND)\n"); break;}
		case CSUDI_VK_ASTERISK: {CSTCPrint("\"*\"键(CSUDI_VK_ASTERISK)\n"); break;}
		case CSUDI_VK_SEEK: {CSTCPrint("定位键(CSUDI_VK_SEEK)\n"); break;}
		case CSUDI_VK_TELETEXT: {CSTCPrint("字幕建(CSUDI_VK_TELETEXT)\n"); break;}
		case CSUDI_VK_SUBTITLE: {CSTCPrint("subtitle键(CSUDI_VK_SUBTITLE)\n"); break;}
		case CSUDI_VK_RECALL: {CSTCPrint("Recall键(CSUDI_VK_RECALL)\n"); break;}
		case CSUDI_VK_TIMER_REC: {CSTCPrint("显示预定列表键(CSUDI_VK_TIMER_REC)\n"); break;}
		case CSUDI_VK_GAME: {CSTCPrint("游戏快捷键(CSUDI_VK_GAME)\n"); break;}
		case CSUDI_VK_ADS: {CSTCPrint("广告快捷键(CSUDI_VK_ADS)\n"); break;}
		case CSUDI_VK_NVOD: {CSTCPrint("NVOD快捷键(CSUDI_VK_NVOD)\n"); break;}
		case CSUDI_VK_STOCK: {CSTCPrint("股票快捷键(CSUDI_VK_STOCK)\n"); break;}
		case CSUDI_VK_PAYOUT: {CSTCPrint("支付快捷键(CSUDI_VK_PAYOUT)\n"); break;}
		case CSUDI_VK_PORTAL: {CSTCPrint("Portal快捷键(CSUDI_VK_PORTAL)\n"); break;}
		case CSUDI_VK_INTERACTION: {CSTCPrint("交互键(CSUDI_VK_INTERACTION)\n"); break;}
		case CSUDI_VK_TIMESHIFT: {CSTCPrint("时移键(CSUDI_VK_TIMESHIFT)\n"); break;}
		case CSUDI_VK_SEARCH: {CSTCPrint("搜索(CSUDI_VK_SEARCH)\n"); break;}
		case CSUDI_VK_ALL: {CSTCPrint("全部键(CSUDI_VK_ALL)\n"); break;}

		default:{	CSTCPrint("Unknow Key\n"); break;}
	}
}

static CSUDI_BOOL iGetSupportRepeatKeys(int* pnKeyArray, int nArraySize, int* pnKeyCount)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;
	char acKey[64] = {0};
	char acValue[32] = {0};
	int nKeyCount = 0;
	int i = 0;
	
	if ((pnKeyArray != CSUDI_NULL) && (nArraySize > 0) && (pnKeyCount != CSUDI_NULL))
	{
		if (CSTKGetConfigInfo("INPUT", "INPUT_SUPPORTREPEAT_KEY_COUNT", acValue, sizeof(acValue)) == CS_TK_CONFIG_SUCCESS)
		{
			nKeyCount = CSTKGetIntFromStr(acValue, 10);    
			CSTCPrint("support repeat key count is %d\n", nKeyCount);
			
			if (nKeyCount !=0 && nArraySize >= nKeyCount)
			{
				for (i=0; i<nKeyCount; i++)
				{
					memset(acKey, 0, sizeof(acKey));
					sprintf(acKey, "IPUNT_SUPPORTREPEAT_KEY_%d", i);

					memset(acValue, 0, sizeof(acValue));
					CSTKGetConfigInfo("INPUT", acKey, acValue, sizeof(acValue));
					
					pnKeyArray[i] = CSTKGetIntFromStr(acValue, 16);
				}

				*pnKeyCount = nKeyCount;
				bRet = CSUDI_TRUE;
			}
		}
	}

	return bRet;
}

//等待遥控器的回调的次数超过CallBackTime
void waitRemoteCallBackTime(int CallBackTime)
{
	g_nTimeCount=0;
	g_nRemoteUserDataSum=0;
	g_nRemoteUserDataSum_1=0;
	g_nRemoteUserDataSum_2=0;
    while(CallBackTime > g_nRemoteCallBackTime)
	{
		CSUDIOSThreadSleep(10);
		
		g_nTimeCount++;
		if (g_nTimeCount>2000)
		{
			break;
		}
	}
}

//等前面板的回调的次数超过CallBackTime
void waitPanelCallBackTime(int CallBackTime)
{
    g_nTimeCount=0;

	while(CallBackTime > g_nPanelCallBackTime)
	{
		CSUDIOSThreadSleep(10);
		
		g_nTimeCount++;
		if (g_nTimeCount>2000)
		{
			break;
		}
	}
}

///在回调函数里面先判断是遥控器还是前面板的，然后保存相应的按键信息，并对回调次数加1 处理
void fnInputCallback_1(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	CSTCPrint("	进入回调\n");
	CSTCPrint("	键码是：%d\n",psInputInfo->m_nKeyCode);
	CSTCPrint("	状态是：%d\n",psInputInfo->m_eStatus);

	g_eType = psInputInfo->m_eType;
	if( EM_UDIINPUT_REMOTECTRL == psInputInfo->m_eType)
	{	
		g_nRemoteCallBackTime++;
		g_nRemoteStatus_repeat=psInputInfo->m_eStatus;		
		if (g_nRemoteCallBackTime==1)
		{
			g_nRemoteKeyCode_1= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_1= psInputInfo->m_eStatus;
			g_nRemoteUserData_1=(int)pvUserData;
			g_nRemoteUserDataSum_1+=(int )pvUserData;
		}
		else if (g_nRemoteCallBackTime==2)
		{
			g_nRemoteKeyCode_2= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_2= psInputInfo->m_eStatus;
			g_nRemoteUserData_2=(int)pvUserData;
			g_nRemoteUserDataSum_2+=(int )pvUserData;
		}
	}

	if( EM_UDIINPUT_FRONTPANEL  == psInputInfo->m_eType)
	{
		g_nPanelCallBackTime++;
		if (g_nPanelCallBackTime==1 )
		{
			g_nPanelKeyCode_1= psInputInfo->m_nKeyCode;
			g_ePanelStatus_1= psInputInfo->m_eStatus;
			g_nPanelUserData_1=(int)pvUserData;
		}
		else if (g_nPanelCallBackTime==2)
		{
			g_nPanelKeyCode_2= psInputInfo->m_nKeyCode;
			g_ePanelStatus_2= psInputInfo->m_eStatus;
			g_nPanelUserData_2=(int)pvUserData;
		}
	}
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试正常注册一个回调函数的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、合法的回调函数指针fnInputCallback
//@INPUT:2、UserData = 0x12345
//@EXPECTATION:返回CSUDI_UCCESS
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数，在回调函数里保存按键信息与回调被调用的次数
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback，使fnInputCallback等于上一步建立的回调函数，pvUserData非空，测试返回值
//@EXECUTIONFLOW:3、按下遥控器数字键'1'  ，期待回调函数被调用1 次，返回键值CSUDI_VK_1，用户数据=0x12345，按键状态为EM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:4、放开遥控器数字键'1'  ，期待回调函数被调用1 次，返回键值CSUDI_VK_1，用户数据=0x12345，按键状态为EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5、调用CSUDIINPUTDelCallback，删除回调函数
//@EXECUTIONFLOW:6、请按下放开按键1 ，期待回回调没有被调用
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0001(void)
{
	int  nUserData=0x12345;
	g_nRemoteCallBackTime=0;

    g_nRemoteKeyCode_1=0 ;
	g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
	g_nRemoteUserData_1=0;
	g_nRemoteKeyCode_2=0 ;
	g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
	g_nRemoteUserData_2=0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "第一步1失败");

	CSTCPrint("请按下遥控器数字键'1'  后马上放开\n");
	
    //等待按下按键1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤3：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");

    //等待放开按键1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "步骤4：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤4：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤4：回调按键状态错误");

	CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "步骤4：回调次数不正确");

	CSTK_FATAL_POINT;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback_1, (void *)(nUserData)), "删除回调回函数失败");
	CSUDIOSThreadSleep(500);

	g_nRemoteKeyCode_1=0 ;
	g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
	g_nRemoteUserData_1=0;
	 
	g_nRemoteKeyCode_2=0 ;
	g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
	g_nRemoteUserData_2=0;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试回调函数指针为CSUDI_NULL  的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、非法的回调函数指针CSUDI_NULL
//@INPUT:2、pvUserData非空
//@EXPECTATION:返回CSUDIINPUT_ERROR_BAD_PARAMETER 
//@REMARK:
//@EXECUTIONFLOW:1、调用CSUDIINPUTAddCallback，使其中的回调函数指针为CSUDI_NULL，pvUserData非空，期待返回CSUDIINPUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0002(void)
{
	int  nUserData=0x12345;
	CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_BAD_PARAMETER == CSUDIINPUTAddCallback(CSUDI_NULL,(void *)(nUserData)), "回调函数为空时失败");

	CSTK_FATAL_POINT
	CSUDIOSThreadSleep(500);
	return CSUDI_TRUE;
}

void fnInputCallback4(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	CSTCPrint("	进入回调\n");
	CSTCPrint("	键码是：%d\n",psInputInfo->m_nKeyCode);
	CSTCPrint("	状态是：%d\n",psInputInfo->m_eStatus);

	g_eType = psInputInfo->m_eType;
	if( EM_UDIINPUT_REMOTECTRL == psInputInfo->m_eType)
	{	
		g_nRemoteCallBackTime++;
		if (psInputInfo->m_eStatus==EM_UDIINPUT_KEY_PRESS )
		{
			g_nRemoteKeyCode_1= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_1= psInputInfo->m_eStatus;
			g_nRemoteUserData_1=(int)pvUserData;
			g_nRemoteUserDataSum_1+=(int )pvUserData;
		}
		if (psInputInfo->m_eStatus==EM_UDIINPUT_KEY_RELEASE )
		{
			g_nRemoteKeyCode_2= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_2= psInputInfo->m_eStatus;
			g_nRemoteUserData_2=(int)pvUserData;
			g_nRemoteUserDataSum_2+=(int )pvUserData;
		}
	}
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试注册的回调函数为八个的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、合法的回调函数指针fnInputCallback
//@INPUT:2、anUserData[]={1,2,3,4,5,6,7,8};
//@EXPECTATION:八次注册回调函数的时候返回CSUDI_SUCCESS
//@REMARK:CSUDIINPUTAddCallback支持注册的回调函数为8个，如果超过8个，则会返回CSUDIINPUT_ERROR_CALLBACK_FULL
//@EXECUTIONFLOW:1、建立一个回调函数，在回调函数里保存按键信息、回调被调用的次数与用户数据的累加和
//@EXECUTIONFLOW:2、连续调用CSUDIINPUTAddCallback8次,每一次传相同的回调函数指针，但pvUserData指针不同
//@EXECUTIONFLOW:3、按下遥控器数字键'1'  ，期待回调函数被调用八次，键值CSUDI_VK_1，按键状态为EM_UDIINPUT_KEY_PRESS ，同时返回用户数据的和36
//@EXECUTIONFLOW:4、放开遥控器数字键'1'  ，期待回调函数被调用八次，键值CSUDI_VK_1，按键状态为EM_UDIINPUT_KEY_RELEASE ，同时返回用户数据的和36
//@EXECUTIONFLOW:5、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0003(void)
{
    int  anUserData[]={1,2,3,4,5,6,7,8};
    int i;
    g_nRemoteCallBackTime=0;
    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_1=0;
    g_nRemoteUserDataSum_1=0;

    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_2=0;
    g_nRemoteUserDataSum_2=0;
 
	for (i=0;i<8;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback( fnInputCallback4, (void *)(anUserData[i])), "第二步失败");
	}

	CSTCPrint("请按下遥控器数字键'1'  后马上放开\n");
	
    // 等待按下按键1
	waitRemoteCallBackTime(8);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(8 ==g_nRemoteCallBackTime, "步骤3：回调次数不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(36  == g_nRemoteUserDataSum_1, "步骤3：回调用户数据的和失败");

    // 等待放开按键1
	waitRemoteCallBackTime(16);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(16 ==g_nRemoteCallBackTime, "步骤4：回调次数不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤4：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(36  == g_nRemoteUserDataSum_2, "步骤4：回调用户数据的和失败");

	CSTK_ASSERT_TRUE_FATAL(16 <=g_nRemoteCallBackTime, "步骤4：回调次数不正确");

	CSTK_FATAL_POINT
	
	CSUDIOSThreadSleep(500);
	for (i=0;i<8;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback4,  (void *)(anUserData[i])), "删除回调回函数失败");
	}
	return CSUDI_TRUE;
}

//三个不同入口的回调函数
void fnInputCallback1(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	if ( 0x12345 == (int )pvUserData)
	{
		g_nRemoteCallBackTime++;	

		if ( g_nRemoteCallBackTime <= 3)
		{
			g_eRemoteStatus_1= psInputInfo->m_eStatus;
			g_nRemoteKeyCode_1= psInputInfo->m_nKeyCode;
		}

		if (( g_nRemoteCallBackTime >3)&&( g_nRemoteCallBackTime <=6))
		{
			g_nRemoteKeyCode_2= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_2= psInputInfo->m_eStatus;
		}
	}
}

void fnInputCallback2(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	if ( 0x12345 == (int )pvUserData)
	{
		g_nRemoteCallBackTime++;	

		if ( g_nRemoteCallBackTime <= 3)
		{
			g_eRemoteStatus_1= psInputInfo->m_eStatus;
			g_nRemoteKeyCode_1= psInputInfo->m_nKeyCode;
		}

		if (( g_nRemoteCallBackTime >3)&&( g_nRemoteCallBackTime <=6))
		{
			g_nRemoteKeyCode_2= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_2= psInputInfo->m_eStatus;
		}
	}
}

void fnInputCallback3(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	if ( 0x12345 == (int )pvUserData)
	{
		g_nRemoteCallBackTime++;	

		if ( g_nRemoteCallBackTime <= 3)
		{
			g_eRemoteStatus_1= psInputInfo->m_eStatus;
			g_nRemoteKeyCode_1= psInputInfo->m_nKeyCode;
		}

		if (( g_nRemoteCallBackTime >3)&&( g_nRemoteCallBackTime <=6))
		{
			g_nRemoteKeyCode_2= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_2= psInputInfo->m_eStatus;
		}
	}
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试注册3个回调函数（入口函数不同，用户数据相同）的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、不同的回调函数指针fnInputCallback1，fnInputCallback2，fnInputCallback3
//@INPUT:2、nUserData=0x12345;
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:CSUDIINPUTAddCallback支持注册的回调函数为8个
//@EXECUTIONFLOW:1、建立3个回调函数fnInputCallback1，fnInputCallback2，fnInputCallback3，在回调函数里检验按键值与用户数据，正确则回调次数加1，保存按键状态
//@EXECUTIONFLOW:2、连续调用CSUDIINPUTAddCallback  3 次,注册上面的3个回调函数，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、按下遥控器数字键'1'  ，期待回调函数被调用3 次，按键状态为EM_UDIINPUT_KEY_PRESS ，键值CSUDI_VK_1
//@EXECUTIONFLOW:4、放开遥控器数字键'1'  ，期待回调函数被调用3 次，按键状态为EM_UDIINPUT_KEY_RELEASE  ，键值CSUDI_VK_1
//@EXECUTIONFLOW:5、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0004(void)
{
    int  nUserData=0x12345;
    g_nRemoteCallBackTime=0;

    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserDataSum_1=0;

    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserDataSum_2=0;

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback1, (void *)(nUserData)), "第一步1失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback2, (void *)(nUserData)), "第一步2失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback3, (void *)(nUserData)), "第一步3失败");

    CSTCPrint("请按下遥控器数字键'1'  后马上放开\n");
	
    // 等待按下按键1
	waitRemoteCallBackTime(3);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤3：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码不正确");

    // 等待放开按键1
	waitRemoteCallBackTime(6);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "步骤4：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE   == g_eRemoteStatus_2, "步骤4：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码不正确");

	CSTK_ASSERT_TRUE_FATAL(6 ==g_nRemoteCallBackTime, "步骤4：回调次数不正确");
	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback1, (void *)(nUserData)), "删除回调回函数失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback2, (void *)(nUserData)), " 删除回调回函数失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback3, (void *)(nUserData)), " 删除回调回函数失败");
	return CSUDI_TRUE;
}

//回调函数4
void fnInputCallback6(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	if ( (0x1 == (int )pvUserData)||(0x2 == (int )pvUserData)||(0x3== (int )pvUserData))
	{
		g_nRemoteCallBackTime++;	

		if ( g_nRemoteCallBackTime<= 3)
		{
			g_nRemoteUserDataSum_1+=(int )pvUserData;
			g_eRemoteStatus_1= psInputInfo->m_eStatus;
			g_nRemoteKeyCode_1= psInputInfo->m_nKeyCode;
		}
		if (( g_nRemoteCallBackTime >3)&&( g_nRemoteCallBackTime <=6))
		{
			g_nRemoteUserDataSum_2+=(int )pvUserData;
			g_eRemoteStatus_2= psInputInfo->m_eStatus;
			g_nRemoteKeyCode_2= psInputInfo->m_nKeyCode;
		}
	}
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试注册3个回调函数（入口函数相同，用户数据不同）的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、相同的回调函数指针fnInputCallback
//@INPUT:2、anUserData[]={1,2,3};
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:CSUDIINPUTAddCallback支持注册的回调函数为8个
//@EXECUTIONFLOW:1、 建立回调函数fnInputCallback，回调函数判断按键值是否为CSUDI_VK_1，保存按键状态与回调次数
//@EXECUTIONFLOW:2、 连续调用CSUDIINPUTAddCallback  3 次,注册上面建立的回调函数，其中用户数据不同，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW:3、按下遥控器数字键'1'  ，期待回调函数被调用3 次，键值CSUDI_VK_1，按键状态为EM_UDIINPUT_KEY_PRESS ,用户数据和为6
//@EXECUTIONFLOW:4、放开遥控器数字键'1'  ，期待回调函数被调用3 次，键值CSUDI_VK_1，按键状态为EM_UDIINPUT_KEY_RELEASE  ，用户数据和为6
//@EXECUTIONFLOW:5、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0005(void)
{
	int  anUserData[]={1,2,3};
	int i;
	g_nRemoteCallBackTime=0;
	
	g_nRemoteUserDataSum_1=0;
	g_eRemoteStatus_1=0;
	g_nRemoteKeyCode_1=0;

	g_nRemoteUserDataSum_2=0;
	g_eRemoteStatus_2=0;
	g_nRemoteKeyCode_2=0;

	for (i=0; i<3; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback6, (void *)(anUserData[i])), "第一步失败");
	}
	
	CSTCPrint("请按下遥控器数字键'1'  后马上放开\n");

   // 等待按下按键1
	waitRemoteCallBackTime(3);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码不正确");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(6  == g_nRemoteUserDataSum_1, "步骤3：回调用户数据和失败");
	
    //	等待放开按键1
	waitRemoteCallBackTime(6);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "步骤4：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码不正确");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE   == g_eRemoteStatus_2, "步骤4：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(6  == g_nRemoteUserDataSum_2, "步骤4：回调用户数据和失败");

	CSTK_ASSERT_TRUE_FATAL(6 ==g_nRemoteCallBackTime, "步骤4：回调次数不正确");
	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);

	for (i=0; i<3; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback6, (void *)(anUserData[i])), "删除函数失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试对同一个回调函数进行5次注册的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、相同的函数指针fnInputCallback
//@INPUT:2、UserData=0x12345
//@EXPECTATION:第一次返回CSUDI_SUCCESS，后四次返回CSUDIINPUT_ERROR_ALREADY_ADDED
//@REMARK:CSUDIINPUTAddCallback对同一个回调进行注册时将会返回CSUDIINPUT_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW:1、建立一个回调函数fnInputCallback
//@EXECUTIONFLOW:2、连续调用CSUDIINPUTAddCallback5次对同一个回调函数指针fnInputCallback进行注册，pvUserData也相同且非空
//@EXECUTIONFLOW:3、按下遥控器数字键'1'  ，期待回调函数被调用1 次，按键状态为EM_UDIINPUT_KEY_PRESS ，键值为CSUDI_VK_1 ，用户数据为0x12345
//@EXECUTIONFLOW:4、放开遥控器数字键'1'  ，期待回调函数被调用1 次，按键状态为EM_UDIINPUT_KEY_RELEASE  ，键值为CSUDI_VK_1 ，用户数据为0x12345
//@EXECUTIONFLOW:5、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0006(void)
{
    int  nUserData=0x12345;
    int i;
    g_nRemoteCallBackTime=0;

    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_1=0;

    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_2=0;

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "第一次注册失败");

	for (i=0; i<4; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_ALREADY_ADDED == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "第二~五次注册失败");
	}

	CSTCPrint("请按下遥控器数字键1  后马上放开\n");
    //等待按下按键1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤3：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");

    //等待放开按键1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "步骤4：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤4：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤4：回调按键状态失败");

	CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "步骤4：回调次数不正确");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback_1, (void *)(nUserData)), "删除回调回函数失败");
	CSUDIOSThreadSleep(500);

    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_1=0;

    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_2=0;
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试正常按下数字键0 ~ 9 的时候，回调函数返回键值的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:正常按下放开数字键0 ~ 9 
//@EXPECTATION:通过回调函数能够得到正常的按键信息
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数，在回调函数里保存返回的键值
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback，将上面建立的回调函数进行注册，pvUserData非空
//@EXECUTIONFLOW:3、请按提示按下放开规定的按键0~9
//@EXECUTIONFLOW:4、判断每一步的回调次数，按键值是否正确
//@EXECUTIONFLOW:5、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0007(void)
{
    int  nUserData=0x12345;
    g_nRemoteCallBackTime=0;

    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_1=0;

    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_2=0;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "注册回调函数失败");

    CSTCPrint("请按下遥控器数字键0  后马上放开\n");

    // 等待按下按键0
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "0：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_0== g_nRemoteKeyCode_1, "0：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "0：回调用户数据不正确");

    // 等待放开按键0
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "0：按键没被松开");

	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "0：回调次数不正确");
	CSTCPrint("请按下遥控器数字键1  后马上放开\n");
	g_nRemoteCallBackTime=0;
    // 等待按下按键1
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "1：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "1：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "1：回调用户数据不正确");

    // 等待放开按键1
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "1：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "1：回调次数不正确");

	CSTCPrint("请按下遥控器数字键2 后马上放开\n");
	g_nRemoteCallBackTime=0;
    // 等待按下按键2
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "2：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_2 == g_nRemoteKeyCode_1, "2：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "2：回调用户数据不正确");

    // 等待放开按键2
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "2：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "2：回调次数不正确");

	CSTCPrint("请按下遥控器数字键3  后马上放开\n");
	g_nRemoteCallBackTime=0;
    // 等待按下按键3
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "3：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_3 == g_nRemoteKeyCode_1, "3：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "3：回调用户数据不正确");

	//等待放开按键3
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "3：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "3：回调次数不正确");

	CSTCPrint("请按下遥控器数字键4 后马上放开\n");
	g_nRemoteCallBackTime=0;
	//等待按下按键4
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "4：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_4 == g_nRemoteKeyCode_1, "4：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "4：回调用户数据不正确");

	//等待放开按键4
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "4：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "4：回调次数不正确");

	CSTCPrint("请按下遥控器数字键5  后马上放开\n");
	g_nRemoteCallBackTime=0;
	//等待按下按键5
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "5：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_5 == g_nRemoteKeyCode_1, "5：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "5：回调用户数据不正确");

	//等待放开按键5
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "5：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "5：回调次数不正确");

	CSTCPrint("请按下遥控器数字键6  后马上放开\n");
	g_nRemoteCallBackTime=0;
	//等待按下按键6
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "6：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_6== g_nRemoteKeyCode_1, "6：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "6：回调用户数据不正确");

	//等待放开按键6
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "2：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "6：回调次数不正确");

	CSTCPrint("请按下遥控器数字键7 后马上放开\n");
	g_nRemoteCallBackTime=0;
	//等待按下按键7
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "7：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_7 == g_nRemoteKeyCode_1, "7：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "7：回调用户数据不正确");

	//等待放开按键7
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "7：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "7：回调次数不正确");

	CSTCPrint("请按下遥控器数字键8 后马上放开\n");
	g_nRemoteCallBackTime=0;
	
	//等待按下按键8
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "8：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_8 == g_nRemoteKeyCode_1, "8：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "8：回调用户数据不正确");

	//等待放开按键8
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "8：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "8：回调次数不正确");

	CSTCPrint("请按下遥控器数字键9  后马上放开\n");
	g_nRemoteCallBackTime=0;
	
	//等待按下按键9
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "9：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_9 == g_nRemoteKeyCode_1, "9：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "9：回调用户数据不正确");

	//等待放开按键9
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "9：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "9：回调次数不正确");

	CSTK_FATAL_POINT
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "删除函数失败");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试按下前面板向左，向右，向上，向下键的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:正常按下放开前面板向左，向右，向上，向下键
//@EXPECTATION:通过回调函数能够得到正常的按键信息
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数，在回调函数里保存返回的按键信息
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback，将上面建立的回调函数进行注册，pvUserData非空
//@EXECUTIONFLOW:3、请按下前面板向左键，期望回调1 次，设备类型为EM_UDIINPUT_FRONTPANEL，按键值为CSUDI_VK_LEFT,按键状态为EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:4、请放开前面板向左键，期望回调1 次，设备类型为EM_UDIINPUT_FRONTPANEL，按键值为CSUDI_VK_LEFT,按键状态为EM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:5、请按下前面板向右键，期望回调1 次，设备类型为EM_UDIINPUT_FRONTPANEL，按键值为CSUDI_VK_RIGHT,按键状态为EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:6、请放开前面板向右键，期望回调1 次，设备类型为EM_UDIINPUT_FRONTPANEL，按键值为CSUDI_VK_RIGHT,按键状态为EM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:7、请按下前面板向上键，期望回调1 次，设备类型为EM_UDIINPUT_FRONTPANEL，按键值为CSUDI_VK_UP,按键状态为EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:8、请放开前面板向上键，期望回调1 次，设备类型为EM_UDIINPUT_FRONTPANEL，按键值为CSUDI_VK_UP,按键状态为EM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:9、请按下前面板向下键，期望回调1 次，设备类型为EM_UDIINPUT_FRONTPANEL，按键值为CSUDI_VK_DOWN,按键状态为EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:10、请放开前面板向下键，期望回调1 次，设备类型为EM_UDIINPUT_FRONTPANEL，按键值为CSUDI_VK_DOWN,按键状态为EM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:11、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0008(void)
{
	int  nUserData=0x12345;
	g_nPanelKeyCode_1=0;
	g_ePanelStatus_1=0;
	g_nPanelUserData_1=0;
	g_nPanelKeyCode_2=0;
	g_ePanelStatus_2=0;
	g_nPanelUserData_2=0;
	g_nPanelCallBackTime=0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "注册回调函数失败");
	
	CSTCPrint("请按下前面板向左键后马上放开\n");

    //等待按下前面板向左键
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤3：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "步骤3：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "步骤3：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_1, "步骤3：回调键码错误");

    //等待放开前面板向左键
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "步骤4：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "步骤4：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "步骤4：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_2, "步骤4：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "步骤4：回调次数不正确");

	g_nPanelCallBackTime=0;
	CSTCPrint("请按下前面板向右键后马上放开\n");

    //等待按下前面板向右键
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤5：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "步骤5：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "步骤5：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_RIGHT== g_nPanelKeyCode_1, "步骤5：回调键码错误");

    //等待放开前面板向右键
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤6：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "步骤6：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "步骤6：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "步骤6：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_RIGHT == g_nPanelKeyCode_2, "步骤6：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "步骤6：回调次数不正确");

	g_nPanelCallBackTime=0;
	CSTCPrint("请按下前面板向上键后马上放开\n");
    //等待按下前面板向上键
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤7：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "步骤7：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "步骤7：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_UP== g_nPanelKeyCode_1, "步骤7：回调键码错误");

    //等待放开前面板向上键
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤8：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "步骤8：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "步骤8：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "步骤8：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_UP== g_nPanelKeyCode_2, "步骤8：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "步骤8：回调次数不正确");

	g_nPanelCallBackTime=0;
	CSTCPrint("请按下前面板向下键后马下放开\n");
    
    //等待按下前面板向下键
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤9：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "步骤9：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "步骤9：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_DOWN== g_nPanelKeyCode_1, "步骤9：回调键码错误");

    //等待放开前面板向下键
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤10：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "步骤10：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "步骤10：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "步骤10：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_DOWN== g_nPanelKeyCode_2, "步骤10：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "步骤10：回调次数不正确");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "步骤11：删除函数失败");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试重复按下按键1 的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:通过遥控器重复按下数字键'1'
//@EXPECTATION:每按下1 就传EM_UDIINPUT_KEY_PRESS ，放开就传EM_UDIINPUT_KEY_RELEASE 
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数，在回调函数里保存按键信息
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback，将上面建立的回调函数进行注册，pvUserData非空
//@EXECUTIONFLOW:3、遥控器按下数字键1 ，期待返加键值为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:4、遥控器放开数字键1 ，期待返加键值为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:5、重复3~4 步5 次，期待每一次都可以正常返回CSUDI_VK_1键值与按键状态
//@EXECUTIONFLOW:6、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0009(void)
{
	int  nUserData=0x12345;
	int i;
	g_nRemoteCallBackTime=0;

	 g_nRemoteKeyCode_1=0 ;
	 g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
	 g_nRemoteUserData_1=0;
	 g_nRemoteKeyCode_2=0 ;
	 g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
	 g_nRemoteUserData_2=0;
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "注册回调函数失败");

	for ( i = 0; i < 5; i ++)
	{
		CSTCPrint("请快速动作：按下遥控器数字键1  后马上放开,连续五次\n");
		g_nRemoteCallBackTime=0;
		
	    //等待按下按键1	
		waitRemoteCallBackTime(1);

		CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "超时没按键按下，退出");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤3：回调用户数据不正确");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码错误");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");

	    //等待放开按键1	
		waitRemoteCallBackTime(2);

		CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "步骤4：按键没被松开");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤4：回调用户数据不正确");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码错误");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤4：回调按键状态失败");

		CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "步骤4：回调次数不正确");

        g_nRemoteCallBackTime=0;
	}

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "删除函数失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试不同设备来源的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:通过两种支持的输入设备进行输入（前面板，遥控器）
//@EXPECTATION:通过回调函数能够得到正常的按键信息
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback，将上面建立的回调函数进行注册，pvUserData非空
//@EXECUTIONFLOW:3、按下遥控器数字键 1  ，期待回调调用一次，按键值为CSUDI_VK_1，按键状态为EM_UDIINPUT_KEY_PRESS ，设备类型为EM_UDIINPUT_KEYBOARD 
//@EXECUTIONFLOW:4、放开遥控器数字键 1  ，期待回调调用一次，按键值为CSUDI_VK_1，按键状态为EM_UDIINPUT_KEY_RELEASE  ，设备类型为EM_UDIINPUT_KEYBOARD 
//@EXECUTIONFLOW:5、按下前面板向左键 ，期待回调调用一次，按键值为CSUDI_VK_LEFT，按键状态为EM_UDIINPUT_KEY_PRESS ，设备类型为EM_UDIINPUT_FRONTPANEL  
//@EXECUTIONFLOW:6、放开前面板向左键 ，期待回调调用一次，按键值为CSUDI_VK_LEFT，按键状态为EM_UDIINPUT_KEY_RELEASE  ，设备类型为EM_UDIINPUT_FRONTPANEL  
//@EXECUTIONFLOW:7、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0010(void)
{
    int  nUserData=0x12345;
    g_nRemoteCallBackTime=0;

    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_1=0;
    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_2=0;

    g_nPanelKeyCode_1=0;
    g_ePanelStatus_1=0;
    g_nPanelUserData_1=0;

    g_nPanelKeyCode_2=0;
    g_ePanelStatus_2=0;
    g_nPanelUserData_2=0;

    g_nPanelCallBackTime=0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "注册回调函数失败");

	CSTCPrint("请按下遥控器数字键'1'  后马上放开\n");
	
	//等待按下按键1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤3：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");

    //等待放开按键1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "步骤4：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤4：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤4：回调按键状态失败");

	CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "步骤4：回调次数不正确");

	CSTCPrint("请按下前面板向左键后马上放开\n");
	
    //等待按下前面板向左键
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤3：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "步骤3：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "步骤3：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_1, "步骤3：回调键码错误");

    //等待放开前面板向左键
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "步骤4：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "步骤4：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "步骤4：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_2, "步骤4：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "步骤4：回调次数不正确");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "删除函数失败");

	return CSUDI_TRUE;
}

void fnInputCallback_12(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	CSTCPrint("	进入回调\n");
	CSTCPrint("	键码是：%d\n",psInputInfo->m_nKeyCode);
	CSTCPrint("	状态是：%d\n",psInputInfo->m_eStatus);

	g_eType = psInputInfo->m_eType;
	if((EM_UDIINPUT_REMOTECTRL == psInputInfo->m_eType) || (EM_UDIINPUT_FRONTPANEL == psInputInfo->m_eType))
	{	
		g_nRemoteCallBackTime++;
		g_nRemoteStatus_repeat=psInputInfo->m_eStatus;		
		if (g_nRemoteCallBackTime==1)
		{
			g_nRemoteKeyCode_1= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_1= psInputInfo->m_eStatus;
			g_nRemoteUserData_1=(int)pvUserData;
		}
		else if (g_nRemoteCallBackTime==2)
		{
			g_nRemoteKeyCode_2= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_2= psInputInfo->m_eStatus;
			g_nRemoteUserData_2=(int)pvUserData;
		}
		else if(g_nRemoteCallBackTime>2)
		{
			g_nRemoteKeyCode_3= psInputInfo->m_nKeyCode;
			g_eRemoteStatus_3= psInputInfo->m_eStatus;
			g_nRemoteUserData_3=(int)pvUserData;
		}
	}
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试长按支持REPEAT消息的按键的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:长按所有支持REPEAT消息的按键
//@EXPECTATION:长按某一按键的时候，不断回调返回EM_UDIINPUT_KEY_REPEAT 状态
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback，将上面建立的回调函数进行注册，pvUserData非空，期望成功
//@EXECUTIONFLOW:3、获取配置文件中配置的所有支持REPEAT消息的按键，期望成功
//@EXECUTIONFLOW:4、按下遥控器一个支持REPEAT消息的按键，期待返回对应键值，状态EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:5、按下持续3 秒钟，期待返回对应键值，状态EM_UDIINPUT_KEY_REPEAT
//@EXECUTIONFLOW:6、放开 按键，期待返回对应键值，状态EM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:7、针对所有支持REPEAT消息的按键重复步骤4~6
//@EXECUTIONFLOW:8、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0011(void)
{
	int anRepeatKey[MAX_SUPPORTREPEAT_KEY_COUNT] = {0};
	int nKeyCount = 0;
	int i = 0;
	int j = 0;
	int  nUserData=0x12345;
	g_nRemoteCallBackTime=0;

	g_nRemoteKeyCode_1=0 ;
	g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
	g_nRemoteUserData_1=0;
	g_nRemoteKeyCode_2=0 ;
	g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
	g_nRemoteUserData_2=0;
	g_nRemoteKeyCode_3=0 ;
	g_eRemoteStatus_3=-1 ;
	g_nRemoteUserData_3=0;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_12, (void *)(nUserData)), "注册回调函数失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == iGetSupportRepeatKeys(anRepeatKey, sizeof(anRepeatKey), &nKeyCount), "从配置文件中获取支持REPEAT消息的按键失败");

	for (i=0; i<nKeyCount; i++)
	{
		g_nRemoteCallBackTime = 0;
		
		CSTCPrint("请按下按键：");
		iPrintKeyInfo(anRepeatKey[i]);
		CSTCPrint("请不要立即放开，看到提示后再放开\n");

		// 等待按下按键
		waitRemoteCallBackTime(1);

		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "步骤3：超时没按键按下，退出");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS == g_eRemoteStatus_1, "步骤3：回调按键状态失败");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤3：回调用户数据不正确");
		CSTK_ASSERT_TRUE_FATAL(anRepeatKey[i] == g_nRemoteKeyCode_1, "步骤3：回调键码错误");
	    
		// 长按按键
		waitRemoteCallBackTime(2);
		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE != g_eRemoteStatus_2, "步骤4：按键被过早放开或回调错误");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT == g_eRemoteStatus_2, "步骤4：回调按键状态失败");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤4：回调用户数据不正确");
		CSTK_ASSERT_TRUE_FATAL(anRepeatKey[i] == g_nRemoteKeyCode_2, "步骤4：回调键码错误");
	    
		// 等待放开按键
		for (j=0; j<5; j++)
		{
			CSTCPrint("请放开按键：");
			iPrintKeyInfo(anRepeatKey[i]);
		}

		CSUDIOSThreadSleep(3000);
		
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT != g_eRemoteStatus_3, "步骤5：按键没被放开或回调错误");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE == g_eRemoteStatus_3, "步骤5：回调按键状态错误");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_3, "步骤5：回调用户数据不正确");
		CSTK_ASSERT_TRUE_FATAL(anRepeatKey[i] == g_nRemoteKeyCode_3, "步骤5：回调键码错误");
	}
	
	CSTK_FATAL_POINT;
	
	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_12, (void *)(nUserData)), "删除函数失败");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试快速在数字键'1' , '2' 之间切换的情况
//@PRECONDITION:INPUT模块被正常初始化过，正常注册了一个回调函数
//@INPUT:快速在数字键'1' , '2' 之间切换
//@EXPECTATION:通过回调函数能够得到正常的按键信息，不死机
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数
//@EXECUTIONFLOW:2、 调用CSUDIINPUTAddCallback，将上面建立的回调函数进行注册，pvUserData非空
//@EXECUTIONFLOW:3、按下遥控器数字键1 ，期待返回键值CSUDI_VK_1，状态EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:4、放开遥控器数字键1 ，期待返回键值CSUDI_VK_1，状态EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5、按下遥控器数字键2 ，期待返回键值CSUDI_VK_1，状态EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:6、放开遥控器数字键2 ，期待返回键值CSUDI_VK_1，状态EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:7、重复3 到期7 步5 次
//@EXECUTIONFLOW:8、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0012(void)
{
    int  nUserData=0x12345;
    int i;
    g_nRemoteCallBackTime=0;

    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_1=0;
    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_2=0;

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "注册回调函数失败");

    CSTCPrint("请先按下放开数字键 1 ，再按下放开数字键2 ，快速动作连续5 次\n");
	
	for ( i=0; i < 5 ;i ++ )
	{
		g_nRemoteCallBackTime=0;
		waitRemoteCallBackTime(1);
		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "步骤3：超时没按键按下，退出");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码不正确");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");

		waitRemoteCallBackTime(2);
		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE   ==g_eRemoteStatus_2, "步骤4：回调按键状态错误");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码不正确");
		CSTCPrint("已按下放开数字键\"1\" %d 次\n", i+1);

		g_nRemoteCallBackTime=0;
		waitRemoteCallBackTime(1);
		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "步骤5：超时没按键按下，退出");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_2 == g_nRemoteKeyCode_1, "步骤5：回调键码不正确");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤5：回调按键状态失败");
		waitRemoteCallBackTime(2);
		CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤6：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_2 == g_nRemoteKeyCode_2, "步骤6：回调键码不正确");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE   ==g_eRemoteStatus_2, "步骤6：回调按键状态错误");
		CSTCPrint("已按下放开数字键\"2\" %d 次\n", i+1);
	}

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "删除函数失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试遥控器在六米距离的响应情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:在不同的距离按下数字键'1'
//@EXPECTATION:找到一个最大的距离使设备能够正常工作，从而回调函数能够得到正常的按键信息，不死机
//@REMARK:期待在四米的范围内遥控器信号能够被正常识别
//@EXECUTIONFLOW:1、建立一个回调函数
//@EXECUTIONFLOW:2、 调用CSUDIINPUTAddCallback，将上面建立的回调函数进行注册，pvUserData非空
//@EXECUTIONFLOW:3、 请在距机机盒 六米处按下数字键1 ，期待返回键值为CSUDI_VK_1，状态EM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:4、放开数字键1 ，期待返回键值CSUDI_VK_1，状态EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0013(void)
{
    int  nUserData=0x12345;
    g_nRemoteCallBackTime=0;

    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_1=0;
    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_2=0;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "注册回调函数失败");

    CSTCPrint("请在距机顶盒六米处按下后放开数字键 1 \n");
	
    //	等待按下按键1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤3：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤3：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码错误");
    
    //	等待放开按键1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "步骤4：按键没有被放开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤4：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤4：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码错误");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "删除函数失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试遥控器在机顶盒1米范围内上下左右偏45 度 的响应情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:在不同的角度按下数字键'1'
//@EXPECTATION: 在45 度的偏角下遥控器信号能够被正常接收
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数，在回调函数里将所传进来的按键信息与用户数据进行打印
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback，将上面建立的回调函数进行注册，pvUserData非空
//@EXECUTIONFLOW:3、在偏左45 度按下数字键1 ，判断返回的键值是否为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:4、在偏左45 度放开数字键1 ，判断返回的键值是否为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5、在偏右45 度按下数字键1 ，判断返回的键值是否为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:6、在偏右45 度放开数字键1 ，判断返回的键值是否为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:7、在偏上45 度按下数字键1 ，判断返回的键值是否为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:8、在偏上45 度放开数字键1 ，判断返回的键值是否为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:9、在偏下45 度按下数字键1 ，判断返回的键值是否为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:10、在偏下45 度放开数字键1 ，判断返回的键值是否为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:11、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0014(void)
{
    int  nUserData=0x12345;
    g_nRemoteCallBackTime=0;

    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_1=0;
    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_2=0;
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "注册回调函数失败");

    //偏左45 度
	CSTCPrint("请在机顶盒1 米范围内偏左45 度按下后放开数字键1 \n");
    
    //等待按下按键1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤3：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤3：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码错误");
    
    //	等待放开按键1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "步骤4：按键没有被放开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤4：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤4：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码错误");
    
    //偏右45 度
	CSTCPrint("请在机顶盒1 米范围内偏右45 度按下后放开数字键1 \n");
    
	g_nRemoteCallBackTime=0;
    
    //	等待按下按键1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤5：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤5：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤5：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤5：回调键码错误");
    
    //	等待放开按键1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤6：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "步骤6：按键没有被放开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤6：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤6：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤6：回调键码错误");

    //偏上45 度
	CSTCPrint("请在机顶盒1 米范围内偏上45 度按下后放开数字键1 \n");
    
	g_nRemoteCallBackTime=0;
    
    //	等待按下按键1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤7：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤7：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤7：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤7：回调键码错误");
    
    //	等待放开按键1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤8：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "步骤8：按键没有被放开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤8：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤8：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤8：回调键码错误");

    //偏下45 度
	CSTCPrint("请在机顶盒1 米范围内偏下45 度按下后放开数字键1 \n");
	g_nRemoteCallBackTime=0;
    
    //等待按下按键1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤9：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤9：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "步骤9：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤9：回调键码错误");
    
    //等待放开按键1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤10：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "步骤10：按键没有被放开");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤10：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "步骤10：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤10：回调键码错误");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "删除函数失败");
	
	return CSUDI_TRUE;
}

void fnInputCallback16(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	if( EM_UDIINPUT_REMOTECTRL == psInputInfo->m_eType)
	{
		g_eRemoteStatus_1 = psInputInfo->m_eStatus;
		
		if (EM_UDIINPUT_KEY_REPEAT !=psInputInfo->m_eStatus)
		{
			g_nRemoteCallBackTime++;
			g_nRemoteKeyCode_1 = psInputInfo->m_nKeyCode;
		}
	}

	if( EM_UDIINPUT_FRONTPANEL  == psInputInfo->m_eType)
	{
		g_ePanelStatus_1 = psInputInfo->m_eStatus;
		if (EM_UDIINPUT_KEY_REPEAT !=psInputInfo->m_eStatus)
		{
			g_nPanelKeyCode_1 = psInputInfo->m_nKeyCode;
			g_nPanelCallBackTime++;
		}
	}
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试遥控器按下'1' 键的 时候，前面板同时按下向左键
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:遥控器按下数字'1'  键，前面板同时按下向左键
//@EXPECTATION:能够正常产生两个回调，传达正确的按键信息
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数，在回调函数里将所传进来的按键信息与用户数据进行打印
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback，将上面建立的回调函数进行注册，pvUserData非空
//@EXECUTIONFLOW:3、遥控器按下数字'1'  键后放开，前面板同时按下向左键后放开，期待遥控器回调两次，键值CSUDI_VK_1，前面板回调两次，键值CSUDI_VK_LEFT
//@EXECUTIONFLOW:4、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0015(void)
{
	int  nUserData=0x12345;
	g_nRemoteCallBackTime=0;
	g_nPanelCallBackTime=0;
	g_nTimeCount = 0;

	g_eRemoteStatus_1=0;
	g_ePanelStatus_1=0;
	g_nRemoteKeyCode_1=0;
	g_nPanelKeyCode_1=0;

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback16, (void *)(nUserData)), "注册回调函数失败");

	CSTCPrint("请用遥控器按下数字键1后放开，在前面板按下向左键后放开\n");

	while((g_nRemoteCallBackTime  == 0) || (g_nPanelCallBackTime == 0))
	{
		g_nTimeCount++;
		
		if (g_nTimeCount>2000)
		{
			break;
		}
	    CSUDIOSThreadSleep(10);
	}
    
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤3：在规定时间内没有按下遥控器数字键1 与前面板向左键，退出");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIINPUT_KEY_REPEAT   != g_eRemoteStatus_1), "步骤3：遥按器按键没有被放开");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIINPUT_KEY_REPEAT   != g_ePanelStatus_1), "步骤3：前面板按键没有被放开");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：遥按器回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_1, "步骤3：前面板回调键码错误");
	CSTK_ASSERT_TRUE_FATAL((g_nRemoteCallBackTime  == 2), "步骤3：遥按器回调次数失败");
	CSTK_ASSERT_TRUE_FATAL((g_nPanelCallBackTime  == 2), "步骤3：前面板回调次数失败");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback16, (void *)(nUserData)), "删除函数失败");

	return CSUDI_TRUE;
}

void fnInputCallback_17(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	CSTCPrint("	进入回调\n");
	CSTCPrint("	键码是：%d\n",psInputInfo->m_nKeyCode);
	CSTCPrint("	状态是：%d\n",psInputInfo->m_eStatus);
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:CSUDIINPUTAddCallback接口pvUserData为NULL也是合法的。
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、合法的回调函数指针fnInputCallback
//@INPUT:2、UserData = NULL
//@EXPECTATION:返回CSUDI_UCCESS
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数，在回调函数里保存按键信息与回调被调用的次数
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback，使fnInputCallback等于上一步建立的回调函数，pvUserData为空，测试返回值
//@EXECUTIONFLOW:3、按下遥控器数字键'1'  ，期待回调函数被调用1 次，返回键值CSUDI_VK_1，用户数据为空，按键状态为EM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:4、放开遥控器数字键'1'  ，期待回调函数被调用1 次，返回键值CSUDI_VK_1，用户数据为空，按键状态为EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5、调用CSUDIINPUTDelCallback，删除回调函数
//@EXECUTIONFLOW:6、请按下放开按键1 ，期待回回调没有被调用
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0017(void)
{
	int  nUserData=0x12345;
	g_nRemoteCallBackTime=0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)nUserData), "第一步1失败");

	CSTCPrint("请按下遥控器数字键'1'  后马上放开\n");
    
    //等待按下按键1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "超时没按键按下，退出");
	//CSTK_ASSERT_TRUE_FATAL(1 ==g_nRemoteCallBackTime, "步骤3：回调次数不正确");
	CSTK_ASSERT_TRUE_FATAL(nUserData == g_nRemoteUserData_1, "步骤3：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤3：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤3：回调按键状态失败");

    //等待放开按键1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "步骤4：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(nUserData == g_nRemoteUserData_2, "步骤4：回调用户数据不正确");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤4：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤4：回调按键状态失败");

	CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "步骤4：回调次数不正确");

	CSTK_FATAL_POINT;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback_1, (void *)(nUserData)), "删除回调回函数失败");
	CSUDIOSThreadSleep(500);
	return CSUDI_TRUE;
}

void fnInputCallback18(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	if (psInputInfo->m_eStatus == EM_UDIINPUT_KEY_PRESS)
	{
		CSTCPrint("当前键码为：0x%x\n",psInputInfo->m_nKeyCode);
		CSTCPrint("当前按键为：");
		iPrintKeyInfo(psInputInfo->m_nKeyCode);
		
		CSTCPrint("如果与遥控器丝印不对应输入\"n\"终止测试，否则继续测试其他按键\n");
	}
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:测试回调抛上来的键值是否与遥控器丝印完全对应
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、合法的回调函数指针fnInputCallback
//@INPUT:2、合法的用户数据指针pvUserData
//@EXPECTATION:回调抛上来的键值与遥控器丝印完全对应
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数fnInputCallback
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback注册回调函数fnInputCallback，pvUserData非空
//@EXECUTIONFLOW:3、提示测试者依次按下遥控器按键并判断是否与丝印对应，如果全部对应则测试通过，只要有一个不对应则测试不通过
//@EXECUTIONFLOW:4、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0018(void)
{
	int  nUserData = 0x12345;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback18, (void *)(nUserData)), "步骤一：注册回调函数失败");

	CSTCPrint("请按下遥控器和前面板任意按键(除了POWER键和学习区按键，请保证每个按键都被按到)\n");
	CSTCPrint("如果过程中发现抛的键值与丝印不对应，则输入\"n\"中止测试\n");
	CSTCPrint("所有按键键值全部正确并全部测试完毕，则输入\"y\"中止测试\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "返回的键值与遥控器丝印不对应");
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback18, (void *)(nUserData)), "步骤四：删除回调函数失败");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTDelCallback 
//@DESCRIPTION:测试正常删除一个回调函数的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、合法的回调函数指针fnInputCallback
//@INPUT:2、nUserData1=0x11;
//@INPUT:2、nUserData2=0x22;
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数fnInputCallback，在回调函数里保存按键信息与回调次数，同时累加用户数据和
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback对回调函数fnInputCallback 进行注册，nUserData=0x11
//@EXECUTIONFLOW:3、调用CSUDIINPUTAddCallback对回调函数fnInputCallback 进行注册，nUserData=0x22
//@EXECUTIONFLOW:4、按下数字键1 ，期待fnInputCallback被调用两次，用户数据和为0x33，键值为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:5、放开数字键1 ，期待fnInputCallback被调用两次，用户数据和为0x33，键值为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:6、调用CSUDIINPUTDelCallback删除已经成功注册了的回调函数fnInputCallback，nUserData=0x11
//@EXECUTIONFLOW:7、按下数字键1 ，期待fnInputCallback被调用1 次，用户数据和为0x22，键值为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:8、放开数字键1 ，期待fnInputCallback被调用1 次，用户数据和为0x22，键值为CSUDI_VK_1，状态为EM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:9、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_DelCallback_0001(void)
{
    int  nUserData1=0x11;
    int  nUserData2=0x22;
    int flag=0;

    g_nRemoteCallBackTime=0;
    g_nRemoteUserDataSum_1=0;
    g_nRemoteUserDataSum_2=0;

    g_nRemoteKeyCode_1=0 ;
    g_eRemoteStatus_1=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_1=0;

    g_nRemoteKeyCode_2=0 ;
    g_eRemoteStatus_2=EM_UDIINPUT_KEY_REPEAT ;
    g_nRemoteUserData_2=0;

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback4, (void *)(nUserData1)), "注册回调函数1 失败");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback4, (void *)(nUserData2)), "注册回调函数2 失败");

    CSTCPrint("请按下遥控器数字键'1'  后马上放开\n");
    
    //等待按下按键1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤4：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤4：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤4：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x33  == g_nRemoteUserDataSum_1, "步骤4：回调用户数据和失败");
    
    //等待放开按键1	
	waitRemoteCallBackTime(4);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤5：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "步骤5：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤5：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤5：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x33  == g_nRemoteUserDataSum_2, "步骤5：回调用户数据和失败");
	CSTK_ASSERT_TRUE_FATAL(4 ==g_nRemoteCallBackTime, "步骤5：回调次数不正确");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback4, (void *)(nUserData1)), "删除回调函数1 失败");

	flag=1;
	CSTCPrint("请再次按下遥控器数字键'1'  后马上放开\n");
	g_nRemoteCallBackTime=0;
	
	//等待按下按键1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤7：超时没按键按下，退出");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "步骤7：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "步骤7：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x22  == g_nRemoteUserDataSum_1, "步骤7：回调用户数据和失败");

    //等待放开按键1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "步骤8：按下按键后既不发release 也不发repeat 信息，超时没响应，退出");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "步骤8：按键没被松开");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "步骤8：回调键码错误");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "步骤8：回调按键状态失败");
	CSTK_ASSERT_TRUE_FATAL(0x22  == g_nRemoteUserDataSum_2, "步骤8：回调用户数据和失败");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nRemoteCallBackTime, "步骤8：回调次数不正确");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback4, (void *)(nUserData2)), "步骤8：删除回调函数2 失败");
	if (0==flag)
	{	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback4, (void *)(nUserData1)), "删除回调函数1 失败");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTDelCallback 
//@DESCRIPTION:测试要删除的回调函数没有被注册的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、没有注册的回调函数fnInputCallback
//@INPUT:2、合法的用户数据指针pvUserData
//@EXPECTATION:CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数fnInputCallback，在回调函数里将按键信息与用户数据进行打印
//@EXECUTIONFLOW:2、调用CSUDIINPUTDelCallback，输入没有注册的回调函数指针fnInputCallback，pvUserData非空，测试返回值
//@EXECUTIONFLOW:3、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_DelCallback_0002(void)
{
	int  nUserData=0x12345;

	CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "删除回调函数失败");
	CSTK_FATAL_POINT
	CSUDIOSThreadSleep(2000);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTDelCallback 
//@DESCRIPTION:测试要删除的回调函数指针为CSUDI_NULL的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、非法的回调函数指针fnInputCallback＝CSUDI_NULL
//@INPUT:2、合法的用户数据指针pvUserData
//@EXPECTATION:返回CSUDIINPUT_ERROR_BAD_PARAMETER 
//@REMARK:
//@EXECUTIONFLOW:1、 调用CSUDIINPUTDelCallback删除一个为NULL的回调函数，pvUserData非空，测试返回值
CSUDI_BOOL CSTC_INPUT_IT_DelCallback_0003(void)
{
	int  nUserData=0x12345;

	CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_BAD_PARAMETER == CSUDIINPUTDelCallback(CSUDI_NULL, (void *)(nUserData)), "注册回调函数失败");
	CSTK_FATAL_POINT
	CSUDIOSThreadSleep(500);
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTDelCallback 
//@DESCRIPTION:测试用户数据指针为CSUDI_NULL的情况
//@PRECONDITION:INPUT模块被正常初始化过
//@INPUT:1、合法的回调函数指针fnInputCallback
//@INPUT:2、用户数据指针pvUserData＝CSUDI_NULL
//@EXPECTATION:返回CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST 
//@REMARK:
//@EXECUTIONFLOW:1、建立一个回调函数fnInputCallback
//@EXECUTIONFLOW:2、调用CSUDIINPUTAddCallback注册回调函数fnInputCallback，pvUserData非空
//@EXECUTIONFLOW:3、调用CSUDIINPUTDelCallback删除刚注册的回调函数，但输入用户数据指针pvUserData=CSUDI_NULL，测试返回值
//@EXECUTIONFLOW:4、调用CSUDIINPUTDelCallback，删除回调函数
CSUDI_BOOL CSTC_INPUT_IT_DelCallback_0004(void)
{
	int  nUserData=0x12345;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "步骤一：注册回调函数失败");

	CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST == CSUDIINPUTDelCallback(fnInputCallback_1, CSUDI_NULL), "步骤三：删除回调函数失败");
	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "步骤四：删除回调函数失败");
	return CSUDI_TRUE;
}

