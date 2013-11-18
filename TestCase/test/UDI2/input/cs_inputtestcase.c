/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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

//�ڱ�����������ִ��ǰ����
CSUDI_BOOL CSTC_INPUT_Init(void)
{
	return CSUDI_TRUE;
}

CSUDI_BOOL CSTC_INPUT_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return CSUDI_TRUE;
}

static void iPrintKeyInfo(int nKeyCode)
{
	switch(nKeyCode)
	{
		case CSUDI_VK_POWER: {CSTCPrint("������(CSUDI_VK_POWER)\n"); break;}
		case CSUDI_VK_0: {CSTCPrint("���ּ�0(CSUDI_VK_0)\n"); break;}
		case CSUDI_VK_1: {CSTCPrint("���ּ�1(CSUDI_VK_1)\n"); break;}
		case CSUDI_VK_2: {CSTCPrint("���ּ�2(CSUDI_VK_2)\n"); break;}
		case CSUDI_VK_3: {CSTCPrint("���ּ�3(CSUDI_VK_2)\n"); break;}
		case CSUDI_VK_4: {CSTCPrint("���ּ�4(CSUDI_VK_4)\n"); break;}
		case CSUDI_VK_5: {CSTCPrint("���ּ�5(CSUDI_VK_5)\n"); break;}
		case CSUDI_VK_6: {CSTCPrint("���ּ�6(CSUDI_VK_6)\n"); break;}
		case CSUDI_VK_7: {CSTCPrint("���ּ�7(CSUDI_VK_7)\n"); break;}
		case CSUDI_VK_8: {CSTCPrint("���ּ�8(CSUDI_VK_8)\n"); break;}
		case CSUDI_VK_9: {CSTCPrint("���ּ�9(CSUDI_VK_9)\n"); break;}
		case CSUDI_VK_OK: {CSTCPrint("ȷ�ϼ�(CSUDI_VK_OK)\n"); break;}
		case CSUDI_VK_TOGGLE: {CSTCPrint("���ؼ�(CSUDI_VK_TOGGLE)\n"); break;}
		case CSUDI_VK_EXIT: {CSTCPrint("�˳���(CSUDI_VK_EXIT)\n"); break;}
		case CSUDI_VK_PAGE_UP: {CSTCPrint("���Ϸ�ҳ(CSUDI_VK_PAGE_UP)\n"); break;}
		case CSUDI_VK_PAGE_DOWN: {CSTCPrint("���·�ҳ(CSUDI_VK_PAGE_DOWN)\n"); break;}
		case CSUDI_VK_LEFT: {CSTCPrint("����� (CSUDI_VK_LEFT)\n"); break;}
		case CSUDI_VK_UP: {CSTCPrint("���ϼ�(CSUDI_VK_UP)\n"); break;}
		case CSUDI_VK_RIGHT: {CSTCPrint("���Ҽ�(CSUDI_VK_RIGHT)\n"); break;}
		case CSUDI_VK_DOWN: {CSTCPrint("���¼�(CSUDI_VK_DOWN)\n"); break;}
		case CSUDI_VK_CHANNEL_UP: {CSTCPrint("Ƶ�����Ӽ�(CSUDI_VK_CHANNEL_UP)\n"); break;}
		case CSUDI_VK_CHANNEL_DOWN: {CSTCPrint("Ƶ�����ټ�(CSUDI_VK_CHANNEL_DOWN)\n"); break;}
		case CSUDI_VK_VOLUME_UP: {CSTCPrint("�������Ӽ�(CSUDI_VK_VOLUME_UP)\n"); break;}
		case CSUDI_VK_VOLUME_DOWN: {CSTCPrint("�������ټ�(CSUDI_VK_VOLUME_DOWN)\n"); break;}
		case CSUDI_VK_MENU: {CSTCPrint("�˵���(CSUDI_VK_MENU)\n"); break;}
		case CSUDI_VK_MUTE: {CSTCPrint("������(CSUDI_VK_MUTE)\n"); break;}
		case CSUDI_VK_GUIDE: {CSTCPrint("��Ŀָ�ϼ�(CSUDI_VK_GUIDE)\n"); break;}
		case CSUDI_VK_AUDIO: {CSTCPrint("������(CSUDI_VK_AUDIO)\n"); break;}
		case CSUDI_VK_COLORED_KEY_0: {CSTCPrint("��ɫ��(CSUDI_VK_COLORED_KEY_0)\n"); break;}
		case CSUDI_VK_COLORED_KEY_1: {CSTCPrint("��ɫ��(CSUDI_VK_COLORED_KEY_1)\n"); break;}
		case CSUDI_VK_COLORED_KEY_2: {CSTCPrint("��ɫ��(CSUDI_VK_COLORED_KEY_2)\n"); break;}
		case CSUDI_VK_COLORED_KEY_3: {CSTCPrint("��ɫ��(CSUDI_VK_COLORED_KEY_3)\n"); break;}
		case CSUDI_VK_PROG_LIST: {CSTCPrint("Ƶ���б��(CSUDI_VK_PROG_LIST)\n"); break;}
		case CSUDI_VK_TV_RADIO: {CSTCPrint("����/��Ƶ�㲥��(CSUDI_VK_TV_RADIO)\n"); break;}
		case CSUDI_VK_CARD_INFO: {CSTCPrint("����Ϣ��(CSUDI_VK_CARD_INFO)\n"); break;}
		case CSUDI_VK_MAIL_INFO: {CSTCPrint("�ʼ���(CSUDI_VK_MAIL_INFO)\n"); break;}
		case CSUDI_VK_VOD: {CSTCPrint("�㲥��(CSUDI_VK_VOD)\n"); break;}
		case CSUDI_VK_SYSSET: {CSTCPrint("ϵͳ���ü�(CSUDI_VK_SYSSET)\n"); break;}
		case CSUDI_VK_HELP: {CSTCPrint("������(CSUDI_VK_HELP)\n"); break;}
		case CSUDI_VK_SERV: {CSTCPrint("��Ѷ��(CSUDI_VK_SERV)\n"); break;}
		case CSUDI_VK_FAV: {CSTCPrint("ϲ����(CSUDI_VK_FAV)\n"); break;}
		case CSUDI_VK_TV: {CSTCPrint("���Ӽ�(CSUDI_VK_TV)\n"); break;}
		case CSUDI_VK_RADIO: {CSTCPrint("�㲥��(CSUDI_VK_RADIO)\n"); break;}
		case CSUDI_VK_INFO: {CSTCPrint("��Ϣ��(CSUDI_VK_INFO)\n"); break;}
		case CSUDI_VK_LANGUAGE: {CSTCPrint("��/Ӣ���л���(CSUDI_VK_LANGUAGE)\n"); break;}
		case CSUDI_VK_INPUT: {CSTCPrint("���뷨����(CSUDI_VK_INPUT)\n"); break;}
		case CSUDI_VK_REFRESH: {CSTCPrint("ˢ����ҳ (CSUDI_VK_REFRESH)\n"); break;}
		case CSUDI_VK_FORWARD: {CSTCPrint("ǰ������ʷ��ҳ��(CSUDI_VK_FORWARD)\n"); break;}
		case CSUDI_VK_BACK: {CSTCPrint("���ˣ���ʷ��ҳ��(CSUDI_VK_BACK)\n"); break;}
		case CSUDI_VK_BROWSER: {CSTCPrint("���������(CSUDI_VK_BROWSER)\n"); break;}
		case CSUDI_VK_RECORD: {CSTCPrint("¼�Ƽ�(CSUDI_VK_RECORD)\n"); break;}
		case CSUDI_VK_PLAY: {CSTCPrint("����(CSUDI_VK_PLAY)\n"); break;}
		case CSUDI_VK_STOP: {CSTCPrint("ֹͣ����(CSUDI_VK_STOP)\n"); break;}
		case CSUDI_VK_PAUSE: {CSTCPrint("��ͣ(CSUDI_VK_PAUSE)\n"); break;}
		case CSUDI_VK_RW: {CSTCPrint("����(CSUDI_VK_RW)\n"); break;}
		case CSUDI_VK_FF: {CSTCPrint("���(CSUDI_VK_FF)\n"); break;}
		case CSUDI_VK_POUND: {CSTCPrint("\"#\"��(CSUDI_VK_POUND)\n"); break;}
		case CSUDI_VK_ASTERISK: {CSTCPrint("\"*\"��(CSUDI_VK_ASTERISK)\n"); break;}
		case CSUDI_VK_SEEK: {CSTCPrint("��λ��(CSUDI_VK_SEEK)\n"); break;}
		case CSUDI_VK_TELETEXT: {CSTCPrint("��Ļ��(CSUDI_VK_TELETEXT)\n"); break;}
		case CSUDI_VK_SUBTITLE: {CSTCPrint("subtitle��(CSUDI_VK_SUBTITLE)\n"); break;}
		case CSUDI_VK_RECALL: {CSTCPrint("Recall��(CSUDI_VK_RECALL)\n"); break;}
		case CSUDI_VK_TIMER_REC: {CSTCPrint("��ʾԤ���б��(CSUDI_VK_TIMER_REC)\n"); break;}
		case CSUDI_VK_GAME: {CSTCPrint("��Ϸ��ݼ�(CSUDI_VK_GAME)\n"); break;}
		case CSUDI_VK_ADS: {CSTCPrint("����ݼ�(CSUDI_VK_ADS)\n"); break;}
		case CSUDI_VK_NVOD: {CSTCPrint("NVOD��ݼ�(CSUDI_VK_NVOD)\n"); break;}
		case CSUDI_VK_STOCK: {CSTCPrint("��Ʊ��ݼ�(CSUDI_VK_STOCK)\n"); break;}
		case CSUDI_VK_PAYOUT: {CSTCPrint("֧����ݼ�(CSUDI_VK_PAYOUT)\n"); break;}
		case CSUDI_VK_PORTAL: {CSTCPrint("Portal��ݼ�(CSUDI_VK_PORTAL)\n"); break;}
		case CSUDI_VK_INTERACTION: {CSTCPrint("������(CSUDI_VK_INTERACTION)\n"); break;}
		case CSUDI_VK_TIMESHIFT: {CSTCPrint("ʱ�Ƽ�(CSUDI_VK_TIMESHIFT)\n"); break;}
		case CSUDI_VK_SEARCH: {CSTCPrint("����(CSUDI_VK_SEARCH)\n"); break;}
		case CSUDI_VK_ALL: {CSTCPrint("ȫ����(CSUDI_VK_ALL)\n"); break;}

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

//�ȴ�ң�����Ļص��Ĵ�������CallBackTime
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

//��ǰ���Ļص��Ĵ�������CallBackTime
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

///�ڻص������������ж���ң��������ǰ���ģ�Ȼ�󱣴���Ӧ�İ�����Ϣ�����Իص�������1 ����
void fnInputCallback_1(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	CSTCPrint("	����ص�\n");
	CSTCPrint("	�����ǣ�%d\n",psInputInfo->m_nKeyCode);
	CSTCPrint("	״̬�ǣ�%d\n",psInputInfo->m_eStatus);

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
//@DESCRIPTION:��������ע��һ���ص����������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1���Ϸ��Ļص�����ָ��fnInputCallback
//@INPUT:2��UserData = 0x12345
//@EXPECTATION:����CSUDI_UCCESS
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص��������ڻص������ﱣ�水����Ϣ��ص������õĴ���
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback��ʹfnInputCallback������һ�������Ļص�������pvUserData�ǿգ����Է���ֵ
//@EXECUTIONFLOW:3������ң�������ּ�'1'  ���ڴ��ص�����������1 �Σ����ؼ�ֵCSUDI_VK_1���û�����=0x12345������״̬ΪEM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:4���ſ�ң�������ּ�'1'  ���ڴ��ص�����������1 �Σ����ؼ�ֵCSUDI_VK_1���û�����=0x12345������״̬ΪEM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5������CSUDIINPUTDelCallback��ɾ���ص�����
//@EXECUTIONFLOW:6���밴�·ſ�����1 ���ڴ��ػص�û�б�����
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
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "��һ��1ʧ��");

	CSTCPrint("�밴��ң�������ּ�'1'  �����Ϸſ�\n");
	
    //�ȴ����°���1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "��ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����3���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");

    //�ȴ��ſ�����1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����4������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����4���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����4���ص�����״̬����");

	CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "����4���ص���������ȷ");

	CSTK_FATAL_POINT;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback_1, (void *)(nUserData)), "ɾ���ص��غ���ʧ��");
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
//@DESCRIPTION:���Իص�����ָ��ΪCSUDI_NULL  �����
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1���Ƿ��Ļص�����ָ��CSUDI_NULL
//@INPUT:2��pvUserData�ǿ�
//@EXPECTATION:����CSUDIINPUT_ERROR_BAD_PARAMETER 
//@REMARK:
//@EXECUTIONFLOW:1������CSUDIINPUTAddCallback��ʹ���еĻص�����ָ��ΪCSUDI_NULL��pvUserData�ǿգ��ڴ�����CSUDIINPUT_ERROR_BAD_PARAMETER
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0002(void)
{
	int  nUserData=0x12345;
	CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_BAD_PARAMETER == CSUDIINPUTAddCallback(CSUDI_NULL,(void *)(nUserData)), "�ص�����Ϊ��ʱʧ��");

	CSTK_FATAL_POINT
	CSUDIOSThreadSleep(500);
	return CSUDI_TRUE;
}

void fnInputCallback4(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	CSTCPrint("	����ص�\n");
	CSTCPrint("	�����ǣ�%d\n",psInputInfo->m_nKeyCode);
	CSTCPrint("	״̬�ǣ�%d\n",psInputInfo->m_eStatus);

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
//@DESCRIPTION:����ע��Ļص�����Ϊ�˸������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1���Ϸ��Ļص�����ָ��fnInputCallback
//@INPUT:2��anUserData[]={1,2,3,4,5,6,7,8};
//@EXPECTATION:�˴�ע��ص�������ʱ�򷵻�CSUDI_SUCCESS
//@REMARK:CSUDIINPUTAddCallback֧��ע��Ļص�����Ϊ8�����������8������᷵��CSUDIINPUT_ERROR_CALLBACK_FULL
//@EXECUTIONFLOW:1������һ���ص��������ڻص������ﱣ�水����Ϣ���ص������õĴ������û����ݵ��ۼӺ�
//@EXECUTIONFLOW:2����������CSUDIINPUTAddCallback8��,ÿһ�δ���ͬ�Ļص�����ָ�룬��pvUserDataָ�벻ͬ
//@EXECUTIONFLOW:3������ң�������ּ�'1'  ���ڴ��ص����������ð˴Σ���ֵCSUDI_VK_1������״̬ΪEM_UDIINPUT_KEY_PRESS ��ͬʱ�����û����ݵĺ�36
//@EXECUTIONFLOW:4���ſ�ң�������ּ�'1'  ���ڴ��ص����������ð˴Σ���ֵCSUDI_VK_1������״̬ΪEM_UDIINPUT_KEY_RELEASE ��ͬʱ�����û����ݵĺ�36
//@EXECUTIONFLOW:5������CSUDIINPUTDelCallback��ɾ���ص�����
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback( fnInputCallback4, (void *)(anUserData[i])), "�ڶ���ʧ��");
	}

	CSTCPrint("�밴��ң�������ּ�'1'  �����Ϸſ�\n");
	
    // �ȴ����°���1
	waitRemoteCallBackTime(8);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "��ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(8 ==g_nRemoteCallBackTime, "����3���ص���������ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(36  == g_nRemoteUserDataSum_1, "����3���ص��û����ݵĺ�ʧ��");

    // �ȴ��ſ�����1
	waitRemoteCallBackTime(16);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(16 ==g_nRemoteCallBackTime, "����4���ص���������ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(36  == g_nRemoteUserDataSum_2, "����4���ص��û����ݵĺ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(16 <=g_nRemoteCallBackTime, "����4���ص���������ȷ");

	CSTK_FATAL_POINT
	
	CSUDIOSThreadSleep(500);
	for (i=0;i<8;i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback4,  (void *)(anUserData[i])), "ɾ���ص��غ���ʧ��");
	}
	return CSUDI_TRUE;
}

//������ͬ��ڵĻص�����
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
//@DESCRIPTION:����ע��3���ص���������ں�����ͬ���û�������ͬ�������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1����ͬ�Ļص�����ָ��fnInputCallback1��fnInputCallback2��fnInputCallback3
//@INPUT:2��nUserData=0x12345;
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:CSUDIINPUTAddCallback֧��ע��Ļص�����Ϊ8��
//@EXECUTIONFLOW:1������3���ص�����fnInputCallback1��fnInputCallback2��fnInputCallback3���ڻص���������鰴��ֵ���û����ݣ���ȷ��ص�������1�����水��״̬
//@EXECUTIONFLOW:2����������CSUDIINPUTAddCallback  3 ��,ע�������3���ص���������������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������ң�������ּ�'1'  ���ڴ��ص�����������3 �Σ�����״̬ΪEM_UDIINPUT_KEY_PRESS ����ֵCSUDI_VK_1
//@EXECUTIONFLOW:4���ſ�ң�������ּ�'1'  ���ڴ��ص�����������3 �Σ�����״̬ΪEM_UDIINPUT_KEY_RELEASE  ����ֵCSUDI_VK_1
//@EXECUTIONFLOW:5������CSUDIINPUTDelCallback��ɾ���ص�����
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

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback1, (void *)(nUserData)), "��һ��1ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback2, (void *)(nUserData)), "��һ��2ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback3, (void *)(nUserData)), "��һ��3ʧ��");

    CSTCPrint("�밴��ң�������ּ�'1'  �����Ϸſ�\n");
	
    // �ȴ����°���1
	waitRemoteCallBackTime(3);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����3����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص����벻��ȷ");

    // �ȴ��ſ�����1
	waitRemoteCallBackTime(6);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����4������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE   == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص����벻��ȷ");

	CSTK_ASSERT_TRUE_FATAL(6 ==g_nRemoteCallBackTime, "����4���ص���������ȷ");
	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback1, (void *)(nUserData)), "ɾ���ص��غ���ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback2, (void *)(nUserData)), " ɾ���ص��غ���ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback3, (void *)(nUserData)), " ɾ���ص��غ���ʧ��");
	return CSUDI_TRUE;
}

//�ص�����4
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
//@DESCRIPTION:����ע��3���ص���������ں�����ͬ���û����ݲ�ͬ�������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1����ͬ�Ļص�����ָ��fnInputCallback
//@INPUT:2��anUserData[]={1,2,3};
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:CSUDIINPUTAddCallback֧��ע��Ļص�����Ϊ8��
//@EXECUTIONFLOW:1�� �����ص�����fnInputCallback���ص������жϰ���ֵ�Ƿ�ΪCSUDI_VK_1�����水��״̬��ص�����
//@EXECUTIONFLOW:2�� ��������CSUDIINPUTAddCallback  3 ��,ע�����潨���Ļص������������û����ݲ�ͬ����������CSUDI_SUCCESS
//@EXECUTIONFLOW:3������ң�������ּ�'1'  ���ڴ��ص�����������3 �Σ���ֵCSUDI_VK_1������״̬ΪEM_UDIINPUT_KEY_PRESS ,�û����ݺ�Ϊ6
//@EXECUTIONFLOW:4���ſ�ң�������ּ�'1'  ���ڴ��ص�����������3 �Σ���ֵCSUDI_VK_1������״̬ΪEM_UDIINPUT_KEY_RELEASE  ���û����ݺ�Ϊ6
//@EXECUTIONFLOW:5������CSUDIINPUTDelCallback��ɾ���ص�����
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
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback6, (void *)(anUserData[i])), "��һ��ʧ��");
	}
	
	CSTCPrint("�밴��ң�������ּ�'1'  �����Ϸſ�\n");

   // �ȴ����°���1
	waitRemoteCallBackTime(3);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "��ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص����벻��ȷ");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(6  == g_nRemoteUserDataSum_1, "����3���ص��û����ݺ�ʧ��");
	
    //	�ȴ��ſ�����1
	waitRemoteCallBackTime(6);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����4������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص����벻��ȷ");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE   == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(6  == g_nRemoteUserDataSum_2, "����4���ص��û����ݺ�ʧ��");

	CSTK_ASSERT_TRUE_FATAL(6 ==g_nRemoteCallBackTime, "����4���ص���������ȷ");
	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);

	for (i=0; i<3; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback6, (void *)(anUserData[i])), "ɾ������ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:���Զ�ͬһ���ص���������5��ע������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1����ͬ�ĺ���ָ��fnInputCallback
//@INPUT:2��UserData=0x12345
//@EXPECTATION:��һ�η���CSUDI_SUCCESS�����Ĵη���CSUDIINPUT_ERROR_ALREADY_ADDED
//@REMARK:CSUDIINPUTAddCallback��ͬһ���ص�����ע��ʱ���᷵��CSUDIINPUT_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW:1������һ���ص�����fnInputCallback
//@EXECUTIONFLOW:2����������CSUDIINPUTAddCallback5�ζ�ͬһ���ص�����ָ��fnInputCallback����ע�ᣬpvUserDataҲ��ͬ�ҷǿ�
//@EXECUTIONFLOW:3������ң�������ּ�'1'  ���ڴ��ص�����������1 �Σ�����״̬ΪEM_UDIINPUT_KEY_PRESS ����ֵΪCSUDI_VK_1 ���û�����Ϊ0x12345
//@EXECUTIONFLOW:4���ſ�ң�������ּ�'1'  ���ڴ��ص�����������1 �Σ�����״̬ΪEM_UDIINPUT_KEY_RELEASE  ����ֵΪCSUDI_VK_1 ���û�����Ϊ0x12345
//@EXECUTIONFLOW:5������CSUDIINPUTDelCallback��ɾ���ص�����
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

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "��һ��ע��ʧ��");

	for (i=0; i<4; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_ALREADY_ADDED == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "�ڶ�~���ע��ʧ��");
	}

	CSTCPrint("�밴��ң�������ּ�1  �����Ϸſ�\n");
    //�ȴ����°���1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "��ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����3���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");

    //�ȴ��ſ�����1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����4������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����4���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");

	CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "����4���ص���������ȷ");

	CSTK_FATAL_POINT
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback_1, (void *)(nUserData)), "ɾ���ص��غ���ʧ��");
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
//@DESCRIPTION:���������������ּ�0 ~ 9 ��ʱ�򣬻ص��������ؼ�ֵ�����
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:�������·ſ����ּ�0 ~ 9 
//@EXPECTATION:ͨ���ص������ܹ��õ������İ�����Ϣ
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص��������ڻص������ﱣ�淵�صļ�ֵ
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback�������潨���Ļص���������ע�ᣬpvUserData�ǿ�
//@EXECUTIONFLOW:3���밴��ʾ���·ſ��涨�İ���0~9
//@EXECUTIONFLOW:4���ж�ÿһ���Ļص�����������ֵ�Ƿ���ȷ
//@EXECUTIONFLOW:5������CSUDIINPUTDelCallback��ɾ���ص�����
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
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "ע��ص�����ʧ��");

    CSTCPrint("�밴��ң�������ּ�0  �����Ϸſ�\n");

    // �ȴ����°���0
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "0����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_0== g_nRemoteKeyCode_1, "0���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "0���ص��û����ݲ���ȷ");

    // �ȴ��ſ�����0
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "0������û���ɿ�");

	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "0���ص���������ȷ");
	CSTCPrint("�밴��ң�������ּ�1  �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
    // �ȴ����°���1
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "1����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "1���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "1���ص��û����ݲ���ȷ");

    // �ȴ��ſ�����1
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "1������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "1���ص���������ȷ");

	CSTCPrint("�밴��ң�������ּ�2 �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
    // �ȴ����°���2
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "2����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_2 == g_nRemoteKeyCode_1, "2���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "2���ص��û����ݲ���ȷ");

    // �ȴ��ſ�����2
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "2������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "2���ص���������ȷ");

	CSTCPrint("�밴��ң�������ּ�3  �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
    // �ȴ����°���3
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "3����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_3 == g_nRemoteKeyCode_1, "3���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "3���ص��û����ݲ���ȷ");

	//�ȴ��ſ�����3
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "3������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "3���ص���������ȷ");

	CSTCPrint("�밴��ң�������ּ�4 �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
	//�ȴ����°���4
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "4����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_4 == g_nRemoteKeyCode_1, "4���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "4���ص��û����ݲ���ȷ");

	//�ȴ��ſ�����4
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "4������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "4���ص���������ȷ");

	CSTCPrint("�밴��ң�������ּ�5  �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
	//�ȴ����°���5
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "5����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_5 == g_nRemoteKeyCode_1, "5���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "5���ص��û����ݲ���ȷ");

	//�ȴ��ſ�����5
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "5������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "5���ص���������ȷ");

	CSTCPrint("�밴��ң�������ּ�6  �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
	//�ȴ����°���6
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "6����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_6== g_nRemoteKeyCode_1, "6���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "6���ص��û����ݲ���ȷ");

	//�ȴ��ſ�����6
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "2������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "6���ص���������ȷ");

	CSTCPrint("�밴��ң�������ּ�7 �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
	//�ȴ����°���7
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "7����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_7 == g_nRemoteKeyCode_1, "7���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "7���ص��û����ݲ���ȷ");

	//�ȴ��ſ�����7
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "7������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "7���ص���������ȷ");

	CSTCPrint("�밴��ң�������ּ�8 �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
	
	//�ȴ����°���8
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "8����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_8 == g_nRemoteKeyCode_1, "8���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "8���ص��û����ݲ���ȷ");

	//�ȴ��ſ�����8
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "8������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "8���ص���������ȷ");

	CSTCPrint("�밴��ң�������ּ�9  �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
	
	//�ȴ����°���9
	waitRemoteCallBackTime(1);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "9����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_9 == g_nRemoteKeyCode_1, "9���ص��������");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "9���ص��û����ݲ���ȷ");

	//�ȴ��ſ�����9
	waitRemoteCallBackTime(2);
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "9������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(2==g_nRemoteCallBackTime, "9���ص���������ȷ");

	CSTK_FATAL_POINT
	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "ɾ������ʧ��");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:���԰���ǰ����������ң����ϣ����¼������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:�������·ſ�ǰ����������ң����ϣ����¼�
//@EXPECTATION:ͨ���ص������ܹ��õ������İ�����Ϣ
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص��������ڻص������ﱣ�淵�صİ�����Ϣ
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback�������潨���Ļص���������ע�ᣬpvUserData�ǿ�
//@EXECUTIONFLOW:3���밴��ǰ���������������ص�1 �Σ��豸����ΪEM_UDIINPUT_FRONTPANEL������ֵΪCSUDI_VK_LEFT,����״̬ΪEM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:4����ſ�ǰ���������������ص�1 �Σ��豸����ΪEM_UDIINPUT_FRONTPANEL������ֵΪCSUDI_VK_LEFT,����״̬ΪEM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:5���밴��ǰ������Ҽ��������ص�1 �Σ��豸����ΪEM_UDIINPUT_FRONTPANEL������ֵΪCSUDI_VK_RIGHT,����״̬ΪEM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:6����ſ�ǰ������Ҽ��������ص�1 �Σ��豸����ΪEM_UDIINPUT_FRONTPANEL������ֵΪCSUDI_VK_RIGHT,����״̬ΪEM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:7���밴��ǰ������ϼ��������ص�1 �Σ��豸����ΪEM_UDIINPUT_FRONTPANEL������ֵΪCSUDI_VK_UP,����״̬ΪEM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:8����ſ�ǰ������ϼ��������ص�1 �Σ��豸����ΪEM_UDIINPUT_FRONTPANEL������ֵΪCSUDI_VK_UP,����״̬ΪEM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:9���밴��ǰ������¼��������ص�1 �Σ��豸����ΪEM_UDIINPUT_FRONTPANEL������ֵΪCSUDI_VK_DOWN,����״̬ΪEM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:10����ſ�ǰ������¼��������ص�1 �Σ��豸����ΪEM_UDIINPUT_FRONTPANEL������ֵΪCSUDI_VK_DOWN,����״̬ΪEM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:11������CSUDIINPUTDelCallback��ɾ���ص�����
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "ע��ص�����ʧ��");
	
	CSTCPrint("�밴��ǰ�������������Ϸſ�\n");

    //�ȴ�����ǰ��������
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����3����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "����3���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "����3���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_1, "����3���ص��������");

    //�ȴ��ſ�ǰ��������
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "����4������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "����4���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "����4���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_2, "����4���ص��������");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "����4���ص���������ȷ");

	g_nPanelCallBackTime=0;
	CSTCPrint("�밴��ǰ������Ҽ������Ϸſ�\n");

    //�ȴ�����ǰ������Ҽ�
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����5����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "����5���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "����5���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_RIGHT== g_nPanelKeyCode_1, "����5���ص��������");

    //�ȴ��ſ�ǰ������Ҽ�
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����6�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "����6������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "����6���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "����6���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_RIGHT == g_nPanelKeyCode_2, "����6���ص��������");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "����6���ص���������ȷ");

	g_nPanelCallBackTime=0;
	CSTCPrint("�밴��ǰ������ϼ������Ϸſ�\n");
    //�ȴ�����ǰ������ϼ�
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����7����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "����7���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "����7���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_UP== g_nPanelKeyCode_1, "����7���ص��������");

    //�ȴ��ſ�ǰ������ϼ�
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����8�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "����8������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "����8���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "����8���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_UP== g_nPanelKeyCode_2, "����8���ص��������");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "����8���ص���������ȷ");

	g_nPanelCallBackTime=0;
	CSTCPrint("�밴��ǰ������¼������·ſ�\n");
    
    //�ȴ�����ǰ������¼�
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����9����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "����9���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "����9���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_DOWN== g_nPanelKeyCode_1, "����9���ص��������");

    //�ȴ��ſ�ǰ������¼�
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����10�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "����10������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "����10���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "����10���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_DOWN== g_nPanelKeyCode_2, "����10���ص��������");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "����10���ص���������ȷ");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "����11��ɾ������ʧ��");
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:�����ظ����°���1 �����
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:ͨ��ң�����ظ��������ּ�'1'
//@EXPECTATION:ÿ����1 �ʹ�EM_UDIINPUT_KEY_PRESS ���ſ��ʹ�EM_UDIINPUT_KEY_RELEASE 
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص��������ڻص������ﱣ�水����Ϣ
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback�������潨���Ļص���������ע�ᣬpvUserData�ǿ�
//@EXECUTIONFLOW:3��ң�����������ּ�1 ���ڴ����Ӽ�ֵΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:4��ң�����ſ����ּ�1 ���ڴ����Ӽ�ֵΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:5���ظ�3~4 ��5 �Σ��ڴ�ÿһ�ζ�������������CSUDI_VK_1��ֵ�밴��״̬
//@EXECUTIONFLOW:6������CSUDIINPUTDelCallback��ɾ���ص�����
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
	 
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "ע��ص�����ʧ��");

	for ( i = 0; i < 5; i ++)
	{
		CSTCPrint("����ٶ���������ң�������ּ�1  �����Ϸſ�,�������\n");
		g_nRemoteCallBackTime=0;
		
	    //�ȴ����°���1	
		waitRemoteCallBackTime(1);

		CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "��ʱû�������£��˳�");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����3���ص��û����ݲ���ȷ");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص��������");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");

	    //�ȴ��ſ�����1	
		waitRemoteCallBackTime(2);

		CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����4������û���ɿ�");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����4���ص��û����ݲ���ȷ");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص��������");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");

		CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "����4���ص���������ȷ");

        g_nRemoteCallBackTime=0;
	}

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "ɾ������ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:���Բ�ͬ�豸��Դ�����
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:ͨ������֧�ֵ������豸�������루ǰ��壬ң������
//@EXPECTATION:ͨ���ص������ܹ��õ������İ�����Ϣ
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص�����
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback�������潨���Ļص���������ע�ᣬpvUserData�ǿ�
//@EXECUTIONFLOW:3������ң�������ּ� 1  ���ڴ��ص�����һ�Σ�����ֵΪCSUDI_VK_1������״̬ΪEM_UDIINPUT_KEY_PRESS ���豸����ΪEM_UDIINPUT_KEYBOARD 
//@EXECUTIONFLOW:4���ſ�ң�������ּ� 1  ���ڴ��ص�����һ�Σ�����ֵΪCSUDI_VK_1������״̬ΪEM_UDIINPUT_KEY_RELEASE  ���豸����ΪEM_UDIINPUT_KEYBOARD 
//@EXECUTIONFLOW:5������ǰ�������� ���ڴ��ص�����һ�Σ�����ֵΪCSUDI_VK_LEFT������״̬ΪEM_UDIINPUT_KEY_PRESS ���豸����ΪEM_UDIINPUT_FRONTPANEL  
//@EXECUTIONFLOW:6���ſ�ǰ�������� ���ڴ��ص�����һ�Σ�����ֵΪCSUDI_VK_LEFT������״̬ΪEM_UDIINPUT_KEY_RELEASE  ���豸����ΪEM_UDIINPUT_FRONTPANEL  
//@EXECUTIONFLOW:7������CSUDIINPUTDelCallback��ɾ���ص�����
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "ע��ص�����ʧ��");

	CSTCPrint("�밴��ң�������ּ�'1'  �����Ϸſ�\n");
	
	//�ȴ����°���1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "��ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����3���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");

    //�ȴ��ſ�����1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����4������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����4���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");

	CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "����4���ص���������ȷ");

	CSTCPrint("�밴��ǰ�������������Ϸſ�\n");
	
    //�ȴ�����ǰ��������
	waitPanelCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����3����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_1, "����3���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_ePanelStatus_1, "����3���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_1, "����3���ص��������");

    //�ȴ��ſ�ǰ��������
	waitPanelCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_ePanelStatus_2, "����4������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_ePanelStatus_2, "����4���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nPanelUserData_2, "����4���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_2, "����4���ص��������");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nPanelCallBackTime, "����4���ص���������ȷ");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "ɾ������ʧ��");

	return CSUDI_TRUE;
}

void fnInputCallback_12(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	CSTCPrint("	����ص�\n");
	CSTCPrint("	�����ǣ�%d\n",psInputInfo->m_nKeyCode);
	CSTCPrint("	״̬�ǣ�%d\n",psInputInfo->m_eStatus);

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
//@DESCRIPTION:���Գ���֧��REPEAT��Ϣ�İ��������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:��������֧��REPEAT��Ϣ�İ���
//@EXPECTATION:����ĳһ������ʱ�򣬲��ϻص�����EM_UDIINPUT_KEY_REPEAT ״̬
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص�����
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback�������潨���Ļص���������ע�ᣬpvUserData�ǿգ������ɹ�
//@EXECUTIONFLOW:3����ȡ�����ļ������õ�����֧��REPEAT��Ϣ�İ����������ɹ�
//@EXECUTIONFLOW:4������ң����һ��֧��REPEAT��Ϣ�İ������ڴ����ض�Ӧ��ֵ��״̬EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:5�����³���3 ���ӣ��ڴ����ض�Ӧ��ֵ��״̬EM_UDIINPUT_KEY_REPEAT
//@EXECUTIONFLOW:6���ſ� �������ڴ����ض�Ӧ��ֵ��״̬EM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:7���������֧��REPEAT��Ϣ�İ����ظ�����4~6
//@EXECUTIONFLOW:8������CSUDIINPUTDelCallback��ɾ���ص�����
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

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_12, (void *)(nUserData)), "ע��ص�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == iGetSupportRepeatKeys(anRepeatKey, sizeof(anRepeatKey), &nKeyCount), "�������ļ��л�ȡ֧��REPEAT��Ϣ�İ���ʧ��");

	for (i=0; i<nKeyCount; i++)
	{
		g_nRemoteCallBackTime = 0;
		
		CSTCPrint("�밴�°�����");
		iPrintKeyInfo(anRepeatKey[i]);
		CSTCPrint("�벻Ҫ�����ſ���������ʾ���ٷſ�\n");

		// �ȴ����°���
		waitRemoteCallBackTime(1);

		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "����3����ʱû�������£��˳�");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����3���ص��û����ݲ���ȷ");
		CSTK_ASSERT_TRUE_FATAL(anRepeatKey[i] == g_nRemoteKeyCode_1, "����3���ص��������");
	    
		// ��������
		waitRemoteCallBackTime(2);
		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE != g_eRemoteStatus_2, "����4������������ſ���ص�����");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����4���ص��û����ݲ���ȷ");
		CSTK_ASSERT_TRUE_FATAL(anRepeatKey[i] == g_nRemoteKeyCode_2, "����4���ص��������");
	    
		// �ȴ��ſ�����
		for (j=0; j<5; j++)
		{
			CSTCPrint("��ſ�������");
			iPrintKeyInfo(anRepeatKey[i]);
		}

		CSUDIOSThreadSleep(3000);
		
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT != g_eRemoteStatus_3, "����5������û���ſ���ص�����");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE == g_eRemoteStatus_3, "����5���ص�����״̬����");
		CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_3, "����5���ص��û����ݲ���ȷ");
		CSTK_ASSERT_TRUE_FATAL(anRepeatKey[i] == g_nRemoteKeyCode_3, "����5���ص��������");
	}
	
	CSTK_FATAL_POINT;
	
	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_12, (void *)(nUserData)), "ɾ������ʧ��");

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:���Կ��������ּ�'1' , '2' ֮���л������
//@PRECONDITION:INPUTģ�鱻������ʼ����������ע����һ���ص�����
//@INPUT:���������ּ�'1' , '2' ֮���л�
//@EXPECTATION:ͨ���ص������ܹ��õ������İ�����Ϣ��������
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص�����
//@EXECUTIONFLOW:2�� ����CSUDIINPUTAddCallback�������潨���Ļص���������ע�ᣬpvUserData�ǿ�
//@EXECUTIONFLOW:3������ң�������ּ�1 ���ڴ����ؼ�ֵCSUDI_VK_1��״̬EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:4���ſ�ң�������ּ�1 ���ڴ����ؼ�ֵCSUDI_VK_1��״̬EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5������ң�������ּ�2 ���ڴ����ؼ�ֵCSUDI_VK_1��״̬EM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:6���ſ�ң�������ּ�2 ���ڴ����ؼ�ֵCSUDI_VK_1��״̬EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:7���ظ�3 ����7 ��5 ��
//@EXECUTIONFLOW:8������CSUDIINPUTDelCallback��ɾ���ص�����
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

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "ע��ص�����ʧ��");

    CSTCPrint("���Ȱ��·ſ����ּ� 1 ���ٰ��·ſ����ּ�2 �����ٶ�������5 ��\n");
	
	for ( i=0; i < 5 ;i ++ )
	{
		g_nRemoteCallBackTime=0;
		waitRemoteCallBackTime(1);
		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "����3����ʱû�������£��˳�");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص����벻��ȷ");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");

		waitRemoteCallBackTime(2);
		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE   ==g_eRemoteStatus_2, "����4���ص�����״̬����");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص����벻��ȷ");
		CSTCPrint("�Ѱ��·ſ����ּ�\"1\" %d ��\n", i+1);

		g_nRemoteCallBackTime=0;
		waitRemoteCallBackTime(1);
		CSTK_ASSERT_TRUE_FATAL(2000 >= g_nTimeCount, "����5����ʱû�������£��˳�");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_2 == g_nRemoteKeyCode_1, "����5���ص����벻��ȷ");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����5���ص�����״̬ʧ��");
		waitRemoteCallBackTime(2);
		CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����6�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
		CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_2 == g_nRemoteKeyCode_2, "����6���ص����벻��ȷ");
		CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE   ==g_eRemoteStatus_2, "����6���ص�����״̬����");
		CSTCPrint("�Ѱ��·ſ����ּ�\"2\" %d ��\n", i+1);
	}

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "ɾ������ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:����ң���������׾������Ӧ���
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:�ڲ�ͬ�ľ��밴�����ּ�'1'
//@EXPECTATION:�ҵ�һ�����ľ���ʹ�豸�ܹ������������Ӷ��ص������ܹ��õ������İ�����Ϣ��������
//@REMARK:�ڴ������׵ķ�Χ��ң�����ź��ܹ�������ʶ��
//@EXECUTIONFLOW:1������һ���ص�����
//@EXECUTIONFLOW:2�� ����CSUDIINPUTAddCallback�������潨���Ļص���������ע�ᣬpvUserData�ǿ�
//@EXECUTIONFLOW:3�� ���ھ������ ���״��������ּ�1 ���ڴ����ؼ�ֵΪCSUDI_VK_1��״̬EM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:4���ſ����ּ�1 ���ڴ����ؼ�ֵCSUDI_VK_1��״̬EM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5������CSUDIINPUTDelCallback��ɾ���ص�����
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
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "ע��ص�����ʧ��");

    CSTCPrint("���ھ���������״����º�ſ����ּ� 1 \n");
	
    //	�ȴ����°���1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����3����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����3���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص��������");
    
    //	�ȴ��ſ�����1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "����4������û�б��ſ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����4���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص��������");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "ɾ������ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:����ң�����ڻ�����1�׷�Χ����������ƫ45 �� ����Ӧ���
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:�ڲ�ͬ�ĽǶȰ������ּ�'1'
//@EXPECTATION: ��45 �ȵ�ƫ����ң�����ź��ܹ�����������
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص��������ڻص������ｫ���������İ�����Ϣ���û����ݽ��д�ӡ
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback�������潨���Ļص���������ע�ᣬpvUserData�ǿ�
//@EXECUTIONFLOW:3����ƫ��45 �Ȱ������ּ�1 ���жϷ��صļ�ֵ�Ƿ�ΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:4����ƫ��45 �ȷſ����ּ�1 ���жϷ��صļ�ֵ�Ƿ�ΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5����ƫ��45 �Ȱ������ּ�1 ���жϷ��صļ�ֵ�Ƿ�ΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:6����ƫ��45 �ȷſ����ּ�1 ���жϷ��صļ�ֵ�Ƿ�ΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:7����ƫ��45 �Ȱ������ּ�1 ���жϷ��صļ�ֵ�Ƿ�ΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:8����ƫ��45 �ȷſ����ּ�1 ���жϷ��صļ�ֵ�Ƿ�ΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:9����ƫ��45 �Ȱ������ּ�1 ���жϷ��صļ�ֵ�Ƿ�ΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:10����ƫ��45 �ȷſ����ּ�1 ���жϷ��صļ�ֵ�Ƿ�ΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:11������CSUDIINPUTDelCallback��ɾ���ص�����
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
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "ע��ص�����ʧ��");

    //ƫ��45 ��
	CSTCPrint("���ڻ�����1 �׷�Χ��ƫ��45 �Ȱ��º�ſ����ּ�1 \n");
    
    //�ȴ����°���1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����3����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����3���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص��������");
    
    //	�ȴ��ſ�����1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "����4������û�б��ſ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����4���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص��������");
    
    //ƫ��45 ��
	CSTCPrint("���ڻ�����1 �׷�Χ��ƫ��45 �Ȱ��º�ſ����ּ�1 \n");
    
	g_nRemoteCallBackTime=0;
    
    //	�ȴ����°���1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����5����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����5���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����5���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����5���ص��������");
    
    //	�ȴ��ſ�����1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����6�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "����6������û�б��ſ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����6���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����6���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����6���ص��������");

    //ƫ��45 ��
	CSTCPrint("���ڻ�����1 �׷�Χ��ƫ��45 �Ȱ��º�ſ����ּ�1 \n");
    
	g_nRemoteCallBackTime=0;
    
    //	�ȴ����°���1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����7����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����7���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����7���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����7���ص��������");
    
    //	�ȴ��ſ�����1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����8�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "����8������û�б��ſ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����8���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����8���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����8���ص��������");

    //ƫ��45 ��
	CSTCPrint("���ڻ�����1 �׷�Χ��ƫ��45 �Ȱ��º�ſ����ּ�1 \n");
	g_nRemoteCallBackTime=0;
    
    //�ȴ����°���1
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����9����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����9���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_1, "����9���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����9���ص��������");
    
    //�ȴ��ſ�����1
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����10�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT   !=g_eRemoteStatus_2, "����10������û�б��ſ�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����10���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x12345 == g_nRemoteUserData_2, "����10���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����10���ص��������");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "ɾ������ʧ��");
	
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
//@DESCRIPTION:����ң��������'1' ���� ʱ��ǰ���ͬʱ���������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:ң������������'1'  ����ǰ���ͬʱ���������
//@EXPECTATION:�ܹ��������������ص���������ȷ�İ�����Ϣ
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص��������ڻص������ｫ���������İ�����Ϣ���û����ݽ��д�ӡ
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback�������潨���Ļص���������ע�ᣬpvUserData�ǿ�
//@EXECUTIONFLOW:3��ң������������'1'  ����ſ���ǰ���ͬʱ�����������ſ����ڴ�ң�����ص����Σ���ֵCSUDI_VK_1��ǰ���ص����Σ���ֵCSUDI_VK_LEFT
//@EXECUTIONFLOW:4������CSUDIINPUTDelCallback��ɾ���ص�����
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

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback16, (void *)(nUserData)), "ע��ص�����ʧ��");

	CSTCPrint("����ң�����������ּ�1��ſ�����ǰ��尴���������ſ�\n");

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
	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����3���ڹ涨ʱ����û�а���ң�������ּ�1 ��ǰ�����������˳�");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIINPUT_KEY_REPEAT   != g_eRemoteStatus_1), "����3��ң��������û�б��ſ�");
	CSTK_ASSERT_TRUE_FATAL((EM_UDIINPUT_KEY_REPEAT   != g_ePanelStatus_1), "����3��ǰ��尴��û�б��ſ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3��ң�����ص��������");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_LEFT == g_nPanelKeyCode_1, "����3��ǰ���ص��������");
	CSTK_ASSERT_TRUE_FATAL((g_nRemoteCallBackTime  == 2), "����3��ң�����ص�����ʧ��");
	CSTK_ASSERT_TRUE_FATAL((g_nPanelCallBackTime  == 2), "����3��ǰ���ص�����ʧ��");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback16, (void *)(nUserData)), "ɾ������ʧ��");

	return CSUDI_TRUE;
}

void fnInputCallback_17(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	CSTCPrint("	����ص�\n");
	CSTCPrint("	�����ǣ�%d\n",psInputInfo->m_nKeyCode);
	CSTCPrint("	״̬�ǣ�%d\n",psInputInfo->m_eStatus);
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:CSUDIINPUTAddCallback�ӿ�pvUserDataΪNULLҲ�ǺϷ��ġ�
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1���Ϸ��Ļص�����ָ��fnInputCallback
//@INPUT:2��UserData = NULL
//@EXPECTATION:����CSUDI_UCCESS
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص��������ڻص������ﱣ�水����Ϣ��ص������õĴ���
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback��ʹfnInputCallback������һ�������Ļص�������pvUserDataΪ�գ����Է���ֵ
//@EXECUTIONFLOW:3������ң�������ּ�'1'  ���ڴ��ص�����������1 �Σ����ؼ�ֵCSUDI_VK_1���û�����Ϊ�գ�����״̬ΪEM_UDIINPUT_KEY_PRESS 
//@EXECUTIONFLOW:4���ſ�ң�������ּ�'1'  ���ڴ��ص�����������1 �Σ����ؼ�ֵCSUDI_VK_1���û�����Ϊ�գ�����״̬ΪEM_UDIINPUT_KEY_RELEASE  
//@EXECUTIONFLOW:5������CSUDIINPUTDelCallback��ɾ���ص�����
//@EXECUTIONFLOW:6���밴�·ſ�����1 ���ڴ��ػص�û�б�����
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0017(void)
{
	int  nUserData=0x12345;
	g_nRemoteCallBackTime=0;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)nUserData), "��һ��1ʧ��");

	CSTCPrint("�밴��ң�������ּ�'1'  �����Ϸſ�\n");
    
    //�ȴ����°���1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "��ʱû�������£��˳�");
	//CSTK_ASSERT_TRUE_FATAL(1 ==g_nRemoteCallBackTime, "����3���ص���������ȷ");
	CSTK_ASSERT_TRUE_FATAL(nUserData == g_nRemoteUserData_1, "����3���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����3���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����3���ص�����״̬ʧ��");

    //�ȴ��ſ�����1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����4������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(nUserData == g_nRemoteUserData_2, "����4���ص��û����ݲ���ȷ");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����4���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����4���ص�����״̬ʧ��");

	CSTK_ASSERT_TRUE_FATAL(2 <=g_nRemoteCallBackTime, "����4���ص���������ȷ");

	CSTK_FATAL_POINT;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback( fnInputCallback_1, (void *)(nUserData)), "ɾ���ص��غ���ʧ��");
	CSUDIOSThreadSleep(500);
	return CSUDI_TRUE;
}

void fnInputCallback18(CSUDIInputInfo_S *psInputInfo,void *pvUserData)
{
	if (psInputInfo->m_eStatus == EM_UDIINPUT_KEY_PRESS)
	{
		CSTCPrint("��ǰ����Ϊ��0x%x\n",psInputInfo->m_nKeyCode);
		CSTCPrint("��ǰ����Ϊ��");
		iPrintKeyInfo(psInputInfo->m_nKeyCode);
		
		CSTCPrint("�����ң����˿ӡ����Ӧ����\"n\"��ֹ���ԣ��������������������\n");
	}
}

//@CASEGROUP:CSUDIINPUTAddCallback 
//@DESCRIPTION:���Իص��������ļ�ֵ�Ƿ���ң����˿ӡ��ȫ��Ӧ
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1���Ϸ��Ļص�����ָ��fnInputCallback
//@INPUT:2���Ϸ����û�����ָ��pvUserData
//@EXPECTATION:�ص��������ļ�ֵ��ң����˿ӡ��ȫ��Ӧ
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص�����fnInputCallback
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallbackע��ص�����fnInputCallback��pvUserData�ǿ�
//@EXECUTIONFLOW:3����ʾ���������ΰ���ң�����������ж��Ƿ���˿ӡ��Ӧ�����ȫ����Ӧ�����ͨ����ֻҪ��һ������Ӧ����Բ�ͨ��
//@EXECUTIONFLOW:4������CSUDIINPUTDelCallback��ɾ���ص�����
CSUDI_BOOL CSTC_INPUT_IT_AddCallback_0018(void)
{
	int  nUserData = 0x12345;

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback18, (void *)(nUserData)), "����һ��ע��ص�����ʧ��");

	CSTCPrint("�밴��ң������ǰ������ⰴ��(����POWER����ѧϰ���������뱣֤ÿ��������������)\n");
	CSTCPrint("��������з����׵ļ�ֵ��˿ӡ����Ӧ��������\"n\"��ֹ����\n");
	CSTCPrint("���а�����ֵȫ����ȷ��ȫ��������ϣ�������\"y\"��ֹ����\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���صļ�ֵ��ң����˿ӡ����Ӧ");
	
	CSTK_FATAL_POINT;
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback18, (void *)(nUserData)), "�����ģ�ɾ���ص�����ʧ��");

	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTDelCallback 
//@DESCRIPTION:��������ɾ��һ���ص����������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1���Ϸ��Ļص�����ָ��fnInputCallback
//@INPUT:2��nUserData1=0x11;
//@INPUT:2��nUserData2=0x22;
//@EXPECTATION:CSUDI_SUCCESS
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص�����fnInputCallback���ڻص������ﱣ�水����Ϣ��ص�������ͬʱ�ۼ��û����ݺ�
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallback�Իص�����fnInputCallback ����ע�ᣬnUserData=0x11
//@EXECUTIONFLOW:3������CSUDIINPUTAddCallback�Իص�����fnInputCallback ����ע�ᣬnUserData=0x22
//@EXECUTIONFLOW:4���������ּ�1 ���ڴ�fnInputCallback���������Σ��û����ݺ�Ϊ0x33����ֵΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:5���ſ����ּ�1 ���ڴ�fnInputCallback���������Σ��û����ݺ�Ϊ0x33����ֵΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:6������CSUDIINPUTDelCallbackɾ���Ѿ��ɹ�ע���˵Ļص�����fnInputCallback��nUserData=0x11
//@EXECUTIONFLOW:7���������ּ�1 ���ڴ�fnInputCallback������1 �Σ��û����ݺ�Ϊ0x22����ֵΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_PRESS
//@EXECUTIONFLOW:8���ſ����ּ�1 ���ڴ�fnInputCallback������1 �Σ��û����ݺ�Ϊ0x22����ֵΪCSUDI_VK_1��״̬ΪEM_UDIINPUT_KEY_RELEASE
//@EXECUTIONFLOW:9������CSUDIINPUTDelCallback��ɾ���ص�����
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

    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback4, (void *)(nUserData1)), "ע��ص�����1 ʧ��");
    CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback4, (void *)(nUserData2)), "ע��ص�����2 ʧ��");

    CSTCPrint("�밴��ң�������ּ�'1'  �����Ϸſ�\n");
    
    //�ȴ����°���1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����4����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����4���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����4���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x33  == g_nRemoteUserDataSum_1, "����4���ص��û����ݺ�ʧ��");
    
    //�ȴ��ſ�����1	
	waitRemoteCallBackTime(4);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����5�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����5������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����5���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����5���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x33  == g_nRemoteUserDataSum_2, "����5���ص��û����ݺ�ʧ��");
	CSTK_ASSERT_TRUE_FATAL(4 ==g_nRemoteCallBackTime, "����5���ص���������ȷ");

	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback4, (void *)(nUserData1)), "ɾ���ص�����1 ʧ��");

	flag=1;
	CSTCPrint("���ٴΰ���ң�������ּ�'1'  �����Ϸſ�\n");
	g_nRemoteCallBackTime=0;
	
	//�ȴ����°���1	
	waitRemoteCallBackTime(1);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����7����ʱû�������£��˳�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_1, "����7���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_PRESS  == g_eRemoteStatus_1, "����7���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x22  == g_nRemoteUserDataSum_1, "����7���ص��û����ݺ�ʧ��");

    //�ȴ��ſ�����1	
	waitRemoteCallBackTime(2);

	CSTK_ASSERT_TRUE_FATAL(2000  >= g_nTimeCount, "����8�����°�����Ȳ���release Ҳ����repeat ��Ϣ����ʱû��Ӧ���˳�");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_REPEAT  !=g_eRemoteStatus_2, "����8������û���ɿ�");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_VK_1 == g_nRemoteKeyCode_2, "����8���ص��������");
	CSTK_ASSERT_TRUE_FATAL(EM_UDIINPUT_KEY_RELEASE  == g_eRemoteStatus_2, "����8���ص�����״̬ʧ��");
	CSTK_ASSERT_TRUE_FATAL(0x22  == g_nRemoteUserDataSum_2, "����8���ص��û����ݺ�ʧ��");
	CSTK_ASSERT_TRUE_FATAL(2 ==g_nRemoteCallBackTime, "����8���ص���������ȷ");

	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback4, (void *)(nUserData2)), "����8��ɾ���ص�����2 ʧ��");
	if (0==flag)
	{	
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback4, (void *)(nUserData1)), "ɾ���ص�����1 ʧ��");
	}
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTDelCallback 
//@DESCRIPTION:����Ҫɾ���Ļص�����û�б�ע������
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1��û��ע��Ļص�����fnInputCallback
//@INPUT:2���Ϸ����û�����ָ��pvUserData
//@EXPECTATION:CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص�����fnInputCallback���ڻص������ｫ������Ϣ���û����ݽ��д�ӡ
//@EXECUTIONFLOW:2������CSUDIINPUTDelCallback������û��ע��Ļص�����ָ��fnInputCallback��pvUserData�ǿգ����Է���ֵ
//@EXECUTIONFLOW:3������CSUDIINPUTDelCallback��ɾ���ص�����
CSUDI_BOOL CSTC_INPUT_IT_DelCallback_0002(void)
{
	int  nUserData=0x12345;

	CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "ɾ���ص�����ʧ��");
	CSTK_FATAL_POINT
	CSUDIOSThreadSleep(2000);
	
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTDelCallback 
//@DESCRIPTION:����Ҫɾ���Ļص�����ָ��ΪCSUDI_NULL�����
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1���Ƿ��Ļص�����ָ��fnInputCallback��CSUDI_NULL
//@INPUT:2���Ϸ����û�����ָ��pvUserData
//@EXPECTATION:����CSUDIINPUT_ERROR_BAD_PARAMETER 
//@REMARK:
//@EXECUTIONFLOW:1�� ����CSUDIINPUTDelCallbackɾ��һ��ΪNULL�Ļص�������pvUserData�ǿգ����Է���ֵ
CSUDI_BOOL CSTC_INPUT_IT_DelCallback_0003(void)
{
	int  nUserData=0x12345;

	CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_BAD_PARAMETER == CSUDIINPUTDelCallback(CSUDI_NULL, (void *)(nUserData)), "ע��ص�����ʧ��");
	CSTK_FATAL_POINT
	CSUDIOSThreadSleep(500);
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIINPUTDelCallback 
//@DESCRIPTION:�����û�����ָ��ΪCSUDI_NULL�����
//@PRECONDITION:INPUTģ�鱻������ʼ����
//@INPUT:1���Ϸ��Ļص�����ָ��fnInputCallback
//@INPUT:2���û�����ָ��pvUserData��CSUDI_NULL
//@EXPECTATION:����CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST 
//@REMARK:
//@EXECUTIONFLOW:1������һ���ص�����fnInputCallback
//@EXECUTIONFLOW:2������CSUDIINPUTAddCallbackע��ص�����fnInputCallback��pvUserData�ǿ�
//@EXECUTIONFLOW:3������CSUDIINPUTDelCallbackɾ����ע��Ļص��������������û�����ָ��pvUserData=CSUDI_NULL�����Է���ֵ
//@EXECUTIONFLOW:4������CSUDIINPUTDelCallback��ɾ���ص�����
CSUDI_BOOL CSTC_INPUT_IT_DelCallback_0004(void)
{
	int  nUserData=0x12345;
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTAddCallback(fnInputCallback_1, (void *)(nUserData)), "����һ��ע��ص�����ʧ��");

	CSTK_ASSERT_TRUE_FATAL(CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST == CSUDIINPUTDelCallback(fnInputCallback_1, CSUDI_NULL), "��������ɾ���ص�����ʧ��");
	CSTK_FATAL_POINT

	CSUDIOSThreadSleep(500);
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIINPUTDelCallback(fnInputCallback_1, (void *)(nUserData)), "�����ģ�ɾ���ص�����ʧ��");
	return CSUDI_TRUE;
}

