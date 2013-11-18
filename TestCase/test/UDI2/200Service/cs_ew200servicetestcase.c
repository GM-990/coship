/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
-----------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../../../include/porting/udi2/udi2_EW200Service.h"
#include "../../../include/porting/udi2/udi2_tuner.h"
#include "cs_ew200servicetestcase.h"
#include "../cs_udi2testcase.h" 
#include "udi2_error.h"
#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_os.h"
#include "cs_testkit.h"
#include "udi2_player.h"
#include "CSEWVod.h"
#include "udiplus_debug.h"

static int s_nDeliverCnt = 1;     //Ƶ�����
static int s_nRunCnt = 10;

static char* s_pcTestDvbcUrl = "dvbc://659000000.6875.64.67";     
static int s_nUserData = 12345;          //tunercallback
static int s_nTestData = -1; 
static int s_nCallbackUserData = 0x1234;
static int s_anTestData[4501];            //test addcallback function

BOOL CSTC_EW200SERVICE_Init(void)
{
	//�ڱ�����������ִ��ǰ����
	return TRUE;
}

BOOL CSTC_EW200SERVICE_UnInit(void)
{
	//�ڱ�����������ִ�к����
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SReadClientInfo 
//@DESCRIPTION: ���Ժ���CSUDIEW200SReadClientInfo�ڲ�������ȷʱ��ִ��Ч��(��Ҫ�Ե������ṩ��EM_UDIEW200S_CARDID, \
//EM_UDIEW200S_SN,EM_UDIEW200S_PROVIDER,EM_UDIEW200S_ALIAS,EM_UDIEW200S_MODULE,EM_UDIEW200S_LOCAL_IP,EM_UDIEW200S_LOCAL_MAC, \
//EM_UDIEW200S_CITV_PSD,EM_UDIEW200S_CITV_SERVERIP,EM_UDIEW200S_CITV_SERVERPORT,EM_UDIEW200S_CITV_VIDEOTYPE,EM_UDIEW200S_CITV_BROWSERVER����)
//@PRECONDITION:�������ṩ������Ϣ
//@INPUT:1���ֱ�����CSUDIEW200SClientInfoType_Eö���е�ֵ
//@INPUT:2��Ҫ��ȡ����CSUDIEW200SClientInfoType_E�и�ö�����Ͷ�Ӧ����ϢacValue
//@INPUT:3����ȡ������Ϣ����Ϊsizeof(acValue)
//@EXPECTATION:������ȡ����ֵ��������л�ö�Ӧ��ֵ���
//@EXECUTIONFLOW:1���������ļ��зֱ���EM_UDIEW200S_CARDID, \
//@EXECUTIONFLOW:EM_UDIEW200S_SN,EM_UDIEW200S_PROVIDER,EM_UDIEW200S_ALIAS,EM_UDIEW200S_MODULE,EM_UDIEW200S_LOCAL_IP,EM_UDIEW200S_LOCAL_MAC, \
//@EXECUTIONFLOW:EM_UDIEW200S_CITV_PSD,EM_UDIEW200S_CITV_SERVERIP,EM_UDIEW200S_CITV_SERVERPORT,EM_UDIEW200S_CITV_VIDEOTYPE,EM_UDIEW200S_CITV_BROWSERVER��Ӧ��ֵ
//@EXECUTIONFLOW:2�����ú���CSUDIEW200SReadClientInfo�ֱ��ȡEM_UDIEW200S_CARDID, \
//@EXECUTIONFLOW:EM_UDIEW200S_SN,EM_UDIEW200S_PROVIDER,EM_UDIEW200S_ALIAS,EM_UDIEW200S_MODULE,EM_UDIEW200S_LOCAL_IP,EM_UDIEW200S_LOCAL_MAC, \
//@EXECUTIONFLOW:EM_UDIEW200S_CITV_PSD,EM_UDIEW200S_CITV_SERVERIP,EM_UDIEW200S_CITV_SERVERPORT,EM_UDIEW200S_CITV_VIDEOTYPE,EM_UDIEW200S_CITV_BROWSERVER����֤��Ϣ
//@EXECUTIONFLOW:3���Ƚϴ������ļ��л�õ�ֵ����ú���CSUDIEW200SReadClientInfo��ȡ����ֵ�������������
BOOL CSTC_EW200SERVICE_MT_0001(void)
{
	char acCardID[32] = {0}; 
	char acSn[32] = {0};
	char acProvider[32] = {0};
	char acAlias[32] = {0};
	char acModule[32] = {0};
	char acLocalIp[32] = {0};
	char acLocalMac[32] = {0};
	char acPsd[32] = {0};
	char acServerIp[32] = {0};
	int nServerPort = 0;
	char acVideoType[32] = {0};
	char acBrowserver[32] = {0};
	char acTemp[32] = {0};
	char acValue[32] = {0};
	int nValue;
	CSUDIEW200SClientInfoType_E eType;
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CARDID", acCardID, sizeof(acCardID)), "�������ļ���ȡֻ�ܿ���ʧ��\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_SN", acSn, sizeof(acSn)), "�������ļ���ȡ���к�ʧ��\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_PROVIDER", acProvider, sizeof(acProvider)), "�������ļ���ȡ�ն��ṩ����Ϣʧ��\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_ALIAS", acAlias, sizeof(acAlias)), "�������ļ���ȡ�ն˱���ʧ��\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_MODULE", acModule, sizeof(acModule)), "�������ļ���ȡ�ն�ģ��ʧ��\n")
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_LOCAL_IP", acLocalIp, sizeof(acLocalIp)), "�������ļ���ȡ����IP��ַʧ��\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_LOCAL_MAC", acLocalMac, sizeof(acLocalMac)), "�������ļ���ȡ����MAC��ַʧ��\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_PSD", acPsd, sizeof(acPsd)), "�������ļ���ȡ�û�����ʧ��\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_SERVERIP", acServerIp, sizeof(acServerIp)), "�������ļ���ȡ������IPʧ��\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_SERVERPORT", acTemp, sizeof(acTemp)), "�������ļ���ȡ�������˿ں�ʧ��\n");
	nServerPort = CSTKGetIntFromStr(acTemp, 10);
		
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_VIDEOTYPE", acVideoType, sizeof(acVideoType)), "�������ļ���ȡ��Ƶ����ʧ��\n")
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_BROWSERVER", acBrowserver, sizeof(acBrowserver)), "�������ļ���ȡ������汾��ʧ��\n");

	eType = EM_UDIEW200S_CARDID;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ���ܿ���Ϣʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue, acCardID), "��ȡ�������к���Ϣ����\n");

	eType = EM_UDIEW200S_SN;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ���к���Ϣʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue, acSn), "��ȡ�������ܿ���Ϣ����\n");

	eType = EM_UDIEW200S_PROVIDER;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ��Ӧ����Ϣʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acProvider), "��ȡ���Ĺ�Ӧ����Ϣ����\n");

	eType = EM_UDIEW200S_ALIAS;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ�ն˱�����Ϣʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acAlias), "��ȡ�����ն˱�����Ϣ����\n");

	eType = EM_UDIEW200S_MODULE;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ�ն��ͺ���Ϣʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acModule), "��ȡ�����ն��ͺ���Ϣ����\n");

	eType = EM_UDIEW200S_LOCAL_IP;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ����IP��ַʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acLocalIp), "��ȡ���ı���IP��ַ����\n");

	eType = EM_UDIEW200S_LOCAL_MAC;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ����MAC��ַʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acLocalMac), "��ȡ���ı���MAC��ַ����\n");

	eType = EM_UDIEW200S_CITV_PSD;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ�û�����ʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acPsd), "��ȡ�����û��������\n");

	eType = EM_UDIEW200S_CITV_SERVERIP;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ������IPʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acServerIp), "��ȡ���ķ�����IP����\n");

	eType = EM_UDIEW200S_CITV_SERVERPORT;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)&nValue, sizeof(int)), "��ȡ�������˿ں�ʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(nValue == nServerPort, "��ȡ���ķ������˿ںŴ���\n");

	eType = EM_UDIEW200S_CITV_VIDEOTYPE;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ��Ƶ�����ʽʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acVideoType), "��ȡ������Ƶ�����ʽ����\n");
	
	eType = EM_UDIEW200S_CITV_BROWSERVER;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "��ȡ������汾��ʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acBrowserver), "��ȡ����������汾�Ŵ���\n");

	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SReadClientInfo 
//@DESCRIPTION: ���Ժ���CSUDIEW200SReadClientInfo��CSUDIEW200SWriteClientInfo�ڲ�������ȷʱ��ִ��Ч��(���userid��token�Ĳ���)
//@PRECONDITION:
//@INPUT:1��EM_UDIEW200S_CITV_USERID, acValue,sizeof(acValue)
//@INPUT:2��EM_UDIEW200S_CITV_TOKEN,acValue,sizeof(acValue)
//@INPUT:3����ȡ������Ϣ����Ϊsizeof(acValue)
//@EXPECTATION:����д���ֵ���ȡ����ֵ���
//@EXECUTIONFLOW:1������CSUDIEW200SWriteClientInfoд��USERID��Ȼ�����CSUDIEW200SReadClientInfo��ȡUSERID������д�����Ϣ���ȡ������Ϣ���
//@EXECUTIONFLOW:2������CSUDIEW200SWriteClientInfoд��TOKEN��Ȼ�����CSUDIEW200SReadClientInfo��ȡTOKEN������д�����Ϣ���ȡ������Ϣ���
BOOL CSTC_EW200SERVICE_MT_0002(void)
{
	char* pcUserID = "123456";
	char* pcToken = "555555";
	char acValue[32] = {0};
	
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(EM_UDIEW200S_CITV_USERID, (void*)pcUserID, strlen(pcUserID)), "д���û�IDʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(EM_UDIEW200S_CITV_USERID, (void*)acValue, sizeof(acValue)), "��ȡ�û�IDʧ��\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(pcUserID, acValue), "д����û�ID����");

	memset(acValue, 0 , sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(EM_UDIEW200S_CITV_TOKEN, (void*)pcToken, strlen(pcToken)), "д��������Ϣʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(EM_UDIEW200S_CITV_TOKEN, (void*)acValue, sizeof(acValue)), "��ȡ������Ϣʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(pcToken, acValue), "д������ƴ���");

	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SReadClientInfo 
//@DESCRIPTION: ���Ժ���CSUDIEW200SReadClientInfo�������������ȷʱ��ִ��Ч��
//@PRECONDITION:None
//@INPUT:���벻���Ͻӿ����Ҫ��Ĳ������
//@INPUT:1��eType=EM_UDIEW200S_CARDID-1, acValue,sizeof(acValue)
//@INPUT:2��eType=EM_UDIEW200S_PROGRAMEINFO+1, acValue,sizeof(acValue)
//@INPUT:3��eType=EM_UDIEW200S_CARDID,acValue,length=0
//@EXPECTATION:����������������º���CSUDIEW200SReadClientInfo�ķ��ض�Ϊ-1
//@EXECUTIONFLOW: 1���������eType=EM_UDIEW200S_CARDID-1��acValue��sizeof(acValue)������CSUDIEW200SReadClientInfo������������ֵΪ-1
//@EXECUTIONFLOW: 2���������eType=EM_UDIEW200S_PROGRAMEINFO+1��acValue��sizeof(acValue)������CSUDIEW200SReadClientInfo������������ֵΪ-1
//@EXECUTIONFLOW: 3���������EM_UDIEW200S_CARDID��acValue��0������CSUDIEW200SReadClientInfo������������ֵΪ-1
BOOL CSTC_EW200SERVICE_MT_0003(void)
{
	char acValue[32] = {0};
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SReadClientInfo(EM_UDIEW200S_CARDID-1, (void*)acValue, sizeof(acValue)), "����1ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SReadClientInfo(EM_UDIEW200S_PROGRAMEINFO+1, (void*)acValue, sizeof(acValue)), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SReadClientInfo(EM_UDIEW200S_CARDID, (void*)acValue, 0), "����3ʧ��");

	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SWriteClientInfo�ڲ������������Ҫ��ʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:����������������Ҫ��Ĳ������
//@INPUT:1������CSUDIEW200SClientInfoType_E��ֵΪEM_UDIEW200S_ISMUTE
//@INPUT:2��Ҫд���eType����Ϣ�ֱ�ΪCSUDI_TRUE��CSUDI_FALSE
//@INPUT:3��д�����Ϣ����Ϊsizeof(int)
//@EXPECTATION:�ɹ�д����Ϣ����д�����Ϣ��Ч��������Ա�ܿ���д����Ϣ����ӦЧ��
//@EXECUTIONFLOW: 1�����ú���CSUDIEW200SWriteClientInfo��д�뾲����ϢCSUDI_TRUE
//@EXECUTIONFLOW: 2������CSUDIEW200SPlayStream���Ž�Ŀ,����������Ա��������ƵЧ��Ϊ����
//@EXECUTIONFLOW: 3������CSTKWaitYes,�ȴ�������Ա��д��CSUDI_TRUEִ��Ч���ķ���
//@EXECUTIONFLOW: 4�����ú���CSUDIEW200SWriteClientInfo��д�뾲����ϢCSUDI_FALSE������������Ա��������ƵЧ��Ϊ�Ǿ���
//@EXECUTIONFLOW: 5������CSTKWaitYes,�ȴ�������Ա��ִ��Ч���ķ���
BOOL CSTC_EW200SERVICE_MT_0004(void)
{
	CSUDI_BOOL bIsmute = CSUDI_TRUE;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_ISMUTE;
	
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&bIsmute, sizeof(int)), "����1ʧ��");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSTCPrint("��Ƶ����Ϊ������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���þ�����Ч");
	
	bIsmute = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&bIsmute, sizeof(int)), "����4ʧ��");
	CSTCPrint("��Ƶ������������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "���÷Ǿ�����Ч");                      
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SWriteClientInfo�ڲ������������Ҫ��ʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:����������������Ҫ��Ĳ������
//@INPUT:1������CSUDIEW200SClientInfoType_E��ֵΪEM_UDIEW200S_VOLUME
//@INPUT:2��Ҫд���eType����ϢvalueΪ0-31֮���ֵ��������
//@INPUT:3��д�����Ϣ����Ϊsizeof(int)
//@EXPECTATION:�ɹ�д����Ϣ����д�����Ϣ��Ч��������Ա�ܿ���д����Ϣ����ӦЧ��
//@EXECUTIONFLOW: 1������CSUDIEW200SPlayStream������Ƶ
//@EXECUTIONFLOW: 2������CSUDIEW200SWriteClientInfo��0-31ÿһ����5ѭ��д������ֵ���ڴ�������Ա����������ǿ
//@EXECUTIONFLOW: 3��ÿ�θı�ֵ�����CSUDIOSThreadSleep�ȴ�5s���ò�����Ա��Ч��
//@EXECUTIONFLOW: 4������CSTKWaitYes,�ȴ�������Ա����д��������Ч��
//@EXECUTIONFLOW: 5������CSUDIEW200SStopPlayֹͣ��Ƶ�Ĳ���
BOOL CSTC_EW200SERVICE_MT_0005(void)
{
	CSUDIEW200SClientInfoType_E eType  = EM_UDIEW200S_VOLUME;
	int  nVolume = 0;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	while(nVolume <= 31)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nVolume, sizeof(int)), "����2ʧ��");
		nVolume += 5;
		CSUDIOSThreadSleep(5000);
	}

	CSTCPrint("��Ƶ����������������ǿ���������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "д��������Ч");                      			  
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SWriteClientInfo�ڲ������������Ҫ��ʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:����������������Ҫ��Ĳ������
//@INPUT:1������CSUDIEW200SClientInfoType_E��ֵΪEM_UDIEW200S_VIDEOALPHA
//@INPUT:2��Ҫд���eType����Ϣvalue
//@INPUT:3��д�����Ϣ����Ϊsizeof(int)
//@EXPECTATION:�ɹ�д����Ϣ����д�����Ϣ��Ч��������Ա�ܿ���д����Ϣ����ӦЧ��
//@EXECUTIONFLOW: 1������CSUDIEW200SPlayStream������Ƶ
//@EXECUTIONFLOW: 2������CSUDIEW200SWriteClientInfo��0-100ÿһ�μ�10ѭ��д��͸���ȵ�ֵ���ڴ�������Ա������Ƶ�ɺڱ�����Ч��
//@EXECUTIONFLOW: 3��ÿ�θı�ֵ�����CSUDIOSThreadSleep�ȴ�5s���ò�����Ա��Ч��
//@EXECUTIONFLOW: 4������CSTKWaitYes,�ȴ�������Ա����д��͸���ȵ�Ч��
//@EXECUTIONFLOW: 5������CSUDIEW200SWriteClientInfo,д��͸����ֵΪ50��ʹ��Ƶ����
//@EXECUTIONFLOW: 6������CSUDIEW200SStopPlayֹͣ��Ƶ�Ĳ���
BOOL CSTC_EW200SERVICE_MT_0006(void)
{
	
	int  nVideoAlpha = 0;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_VIDEOALPHA;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);	
	
	while(nVideoAlpha <= 100)
	{			
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nVideoAlpha, sizeof(int)), "����2ʧ��");
		nVideoAlpha += 10;	
		CSUDIOSThreadSleep(5000);
	}
	
	CSTCPrint("��ʼ������Ƶ����Ƶ��ȫ���𽥱��͸����\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "д��͸������Ч");        

	nVideoAlpha = 50;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nVideoAlpha, sizeof(int)), "д��͸����ʧ��");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT	
	
	CSUDIEW200SStopPlay(0);	
	
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SWriteClientInfo�ڲ������������Ҫ��ʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:����������������Ҫ��Ĳ������
//@INPUT:1������CSUDIEW200SClientInfoType_E��ֵΪEM_UDIEW200S_BRIGHTNESS
//@INPUT:2��Ҫд���eType����Ϣvalue
//@INPUT:3��д�����Ϣ����Ϊsizeof(int)
//@EXPECTATION:�ɹ�д����Ϣ����д�����Ϣ��Ч��������Ա�ܿ���д����Ϣ����ӦЧ��
//@EXECUTIONFLOW: 1������CSUDIEW200SPlayStream������Ƶ
//@EXECUTIONFLOW: 2������CSUDIEW200SWriteClientInfo��0-100ÿһ�μ�10ѭ��д�����ȵ�ֵ���ڴ�������Ա������Ƶ�ɺڱ��������ٱ��ȫ�׵�Ч��
//@EXECUTIONFLOW: 3��ÿ�θı�ֵ�����CSUDIOSThreadSleep�ȴ�5s���ò�����Ա��Ч��
//@EXECUTIONFLOW: 4������CSTKWaitYes,�ȴ�������Ա����д�����ȵ�Ч��
//@EXECUTIONFLOW: 5������CSUDIEW200SWriteClientInfo,д������ֵΪ50��ʹ��Ƶ����
//@EXECUTIONFLOW: 6������CSUDIEW200SStopPlayֹͣ��Ƶ�Ĳ���
BOOL CSTC_EW200SERVICE_MT_0007(void)
{
	int  nBrightness = 0;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_BRIGHTNESS;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	while(nBrightness <= 100)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nBrightness, sizeof(int)), "����2ʧ��");
		nBrightness += 10;
		CSUDIOSThreadSleep(5000);
	}
	CSTCPrint("��Ƶ��ʼ���ź���Ƶ�����ɺڱ��������ٱ��ȫ����\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "д����Ƶ������Ч");                      			  

	nBrightness = 50;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nBrightness, sizeof(int)), "����5ʧ��");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SWriteClientInfo�ڲ������������Ҫ��ʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:����������������Ҫ��Ĳ������
//@INPUT:1������CSUDIEW200SClientInfoType_E��ֵΪEM_UDIEW200S_CONTRAST
//@INPUT:2��Ҫд���eType����Ϣvalue
//@INPUT:3��д�����Ϣ����Ϊsizeof(int)
//@EXPECTATION:�ɹ�д����Ϣ����д�����Ϣ��Ч��������Ա�ܿ���д����Ϣ����ӦЧ��
//@EXECUTIONFLOW: 1������CSUDIEW200SPlayStream������Ƶ
//@EXECUTIONFLOW: 2������CSUDIEW200SWriteClientInfo��0-100ÿһ�μ�10д��Աȶȵ�ֵ���ڴ�������Ա������Ƶ�ɲ�������������Ч��
//@EXECUTIONFLOW: 3��ÿ�θı�ֵ�����CSUDIOSThreadSleep�ȴ�5s���ò�����Ա��Ч��
//@EXECUTIONFLOW: 4������CSTKWaitYes,�ȴ�������Ա����д��Աȶȵ�Ч��
//@EXECUTIONFLOW: 5������CSUDIEW200SWriteClientInfo,д��Աȶ�ֵΪ50��ʹ��Ƶ����
//@EXECUTIONFLOW: 6������CSUDIEW200SStopPlayֹͣ��Ƶ�Ĳ���
BOOL CSTC_EW200SERVICE_MT_0008(void)
{
	int  nContrast = 0;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_CONTRAST;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	while(nContrast <= 100)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nContrast, sizeof(int)), "����2ʧ��");
		 nContrast += 10;
		 CSUDIOSThreadSleep(5000);
	}
	CSTCPrint("��ʼ������Ƶ����Ƶ�ɲ����������������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "д����Ƶ�Աȶ���Ч");                      			  

	nContrast = 50;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nContrast, sizeof(int)), "����5ʧ��");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SWriteClientInfo�ڲ������������Ҫ��ʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:����������������Ҫ��Ĳ������
//@INPUT:1������CSUDIEW200SClientInfoType_E��ֵΪEM_UDIEW200S_SATURATION
//@INPUT:2��Ҫд���eType����Ϣvalue
//@INPUT:3��д�����Ϣ����Ϊsizeof(int)
//@EXPECTATION:�ɹ�д����Ϣ����д�����Ϣ��Ч��������Ա�ܿ���д����Ϣ����ӦЧ��
//@EXECUTIONFLOW: 1������CSUDIEW200SPlayStream������Ƶ
//@EXECUTIONFLOW: 2������CSUDIEW200SWriteClientInfo��0-100ÿһ�μ�10д�뱥�Ͷȵ�ֵ���ڴ�������Ա������Ƶ�ɻұ����������Ч��
//@EXECUTIONFLOW: 3��ÿ�θı�ֵ�����CSUDIOSThreadSleep�ȴ�5s���ò�����Ա��Ч��
//@EXECUTIONFLOW: 4������CSTKWaitYes,�ȴ�������Ա����д�뱥�Ͷȵ�Ч��
//@EXECUTIONFLOW: 5������CSUDIEW200SWriteClientInfo,д�뱥�Ͷ�ֵΪ50��ʹ����Ƶ�ܿ���
//@EXECUTIONFLOW: 6������CSUDIEW200SStopPlayֹͣ��Ƶ�Ĳ���
BOOL CSTC_EW200SERVICE_MT_0009(void)
{
	int  nSaturation = 0;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_SATURATION;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	while(nSaturation <= 100)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nSaturation, sizeof(int)), "����2ʧ��");
		nSaturation += 10;
		CSUDIOSThreadSleep(5000);
	}
	CSTCPrint("��ʼ������Ƶ����Ƶ��ɫ�Աȶ��ɻұ����������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "д����Ƶ���Ͷ���Ч\n");                      			  

	nSaturation = 50;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nSaturation, sizeof(int)), "����5ʧ��\n");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;    
}
	
//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SWriteClientInfo�ڲ������������Ҫ��ʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:����������������Ҫ��Ĳ������
//@INPUT:1������CSUDIEW200SClientInfoType_E��ֵΪEM_UDIEW200S_ASPECTRATIO
//@INPUT:2��Ҫд���eType����Ϣvalue
//@INPUT:3��д�����Ϣ����Ϊsizeof(int)
//@EXPECTATION:�ɹ�д����Ϣ����д�����Ϣ��Ч��������Ա�ܿ���д����Ϣ����ӦЧ��
//@EXECUTIONFLOW: 1������CSUDIEW200SWriteClientInfoд����ϢEM_UDIEW200S_WIN_ASPECT_RATIO_4_3
//@EXECUTIONFLOW: 2������CSUDIEW200SPlayStream,����������Ա������Ч��Ϊ��Ļ��߱�Ϊ4:3
//@EXECUTIONFLOW: 3������CSUDIEW200SWriteClientInfoд����ϢEM_UDIEW200S_WIN_ASPECT_RATIO_16_9
//@EXECUTIONFLOW: 4������CSUDIEW200SPlayStream,����������Ա������Ч��Ϊ��Ļ��߱�Ϊ16:9
//@EXECUTIONFLOW: 5������CSTKWaitYes,�ȴ�������Ա����д�벻ͬ��Ƶ��Ч��
//@EXECUTIONFLOW: 6������CSUDIEW200SStopPlayֹͣ������Ƶ
BOOL CSTC_EW200SERVICE_MT_0010(void)
{
	int  nCSUDI_ASPECTRATIO = EM_UDIEW200S_WIN_ASPECT_RATIO_4_3;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_ASPECTRATIO;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nCSUDI_ASPECTRATIO, sizeof(int)), "����1ʧ��");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSTCPrint("��Ƶ��߱�Ϊ4:3��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "д�����Ƶ��߱�4:3��Ч");  
	
	nCSUDI_ASPECTRATIO = EM_UDIEW200S_WIN_ASPECT_RATIO_16_9;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nCSUDI_ASPECTRATIO, sizeof(int)), "����3ʧ��");
	CSTCPrint("��Ƶ��߱�Ϊ16:9��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "д����Ƶ��߱�16:9��Ч");       
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SWriteClientInfo�ڲ������������Ҫ��ʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:����������������Ҫ��Ĳ������
//@INPUT:1������CSUDIEW200SClientInfoType_Eö���е�ֵEM_UDIEW200S_STOPMODE
//@INPUT:2��Ҫд���eType����Ϣvalue
//@INPUT:3��д�����Ϣ����Ϊsizeof(int)
//@EXPECTATION:�ɹ�д����Ϣ����д�����Ϣ��Ч��������Ա�ܿ���д����Ϣ����ӦЧ��
//@EXECUTIONFLOW: 1������CSUDIEW200SWriteClientInfoд��eType=EM_UDIEW200S_STOPMODEʱ����ϢEM_UDIEW200S_STOPMODE_BLACK
//@EXECUTIONFLOW: 2������CSUDIEW200SPlayStream������Ƶ 
//@EXECUTIONFLOW: 3������CSUDIOSThreadSleep�ȴ�5s
//@EXECUTIONFLOW: 4������CSUDIEW200SStopPlayֹͣ��Ƶ���ţ�����������Ա������Ƶֹͣ����ʱ��Ч��Ϊ����
//@EXECUTIONFLOW: 5������CSTKWaitYes,�ȴ�������Ա������������Ƶֹͣʱ��Ч��Ϊ����
//@EXECUTIONFLOW: 6������CSUDIEW200SWriteClientInfoд��eType=EM_UDIEW200S_STOPMODEʱ����ϢEM_UDIEW200S_STOPMODE_FREEZE
//@EXECUTIONFLOW: 7������CSUDIEW200SPlayStream������Ƶ
//@EXECUTIONFLOW: 8������CSUDIOSThreadSleep�ȴ�5s
//@EXECUTIONFLOW: 9������CSUDIEW200SStopPlayֹͣ��Ƶ���ţ�����������Ա������Ƶֹͣ����ʱ��Ч��Ϊ��֡
//@EXECUTIONFLOW: 10������CSTKWaitYes,�ȴ�������Ա������������Ƶֹͣʱ��Ч��Ϊ��֡
BOOL CSTC_EW200SERVICE_MT_0011(void)
{
	int  nCSUDI_STOPMODE = EM_UDIEW200S_STOPMODE_BLACK;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_STOPMODE;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nCSUDI_STOPMODE, sizeof(int)), "����1ʧ��");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSUDIOSThreadSleep(5000);
	CSUDIEW200SStopPlay(0);
	CSTCPrint("��ƵֹͣʱΪ������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "д���ֹͣ����ʱ��Ƶ������Ч"); 
	
	nCSUDI_STOPMODE = EM_UDIEW200S_STOPMODE_FREEZE;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nCSUDI_STOPMODE, sizeof(int)), "����6ʧ��");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSUDIOSThreadSleep(5000);
	CSUDIEW200SStopPlay(0);
	CSTCPrint("��ƵֹͣʱΪ��֡��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "д��ֹͣ����ʱ��Ƶ������Ч"); 
	
	CSTK_FATAL_POINT
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo 
//@DESCRIPTION: ���Ժ���CSUDIEW200SWriteClientInfo�������������ȷʱ��ִ��Ч��
//@PRECONDITION:None
//@INPUT:���벻���Ͻӿ����Ҫ��Ĳ������
//@INPUT:1��eType=EM_UDIEW200S_CARDID-1������������ȷ
//@INPUT:2��eType=EM_UDIEW200S_PROGRAMEINFO+1������������ȷ
//@INPUT:3��length=0������������ȷ
//@INPUT:4��value�Ƿ�ʱ��eType=EM_UDIEW200S_AUDIOPID,value=-1,length=sizeof(int)
//@EXPECTATION:����CSUDIEW200SWriteClientInfo����-1
//@EXECUTIONFLOW: 1���������eType=EM_UDIEW200S_CARDID-1��value = 700092200002, length = 32,���ú���CSUDIEW200SWriteClientInfoд����Ϣ��������������ֵΪ-1
//@EXECUTIONFLOW: 2���������eType=EM_UDIEW200S_PROGRAMEINFO+1��value = 700092200002, length = 32,���ú���CSUDIEW200SWriteClientInfoд����Ϣ��������������ֵΪ-1
//@EXECUTIONFLOW: 3���������eType=EM_UDIEW200S_CARDID��value = 700092200002, length = 0������CSUDIEW200SWriteClientInfoд����Ϣ��������������ֵΪ-1
//@EXECUTIONFLOW: 4���������eType=EM_UDIEW200S_CARDID��value = NULL, length = 32������CSUDIEW200SWriteClientInfoд����Ϣ��������������ֵΪ-1
BOOL CSTC_EW200SERVICE_MT_0012(void)
{
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SWriteClientInfo(EM_UDIEW200S_CARDID-1, "700092200002", 32), "����1ʧ��");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SWriteClientInfo(EM_UDIEW200S_PROGRAMEINFO+1, "700092200002", 32), "����2ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SWriteClientInfo(EM_UDIEW200S_CARDID, "700092200002", 0), "����3ʧ��");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SWriteClientInfo(EM_UDIEW200S_AUDIOPID, NULL, 32), "����4ʧ��");

	CSTK_FATAL_POINT
		
	return TRUE;
}

static void  TunerCallback ( CSUDITunerType_E eType, CSUDITunerCallbackMessage_S* psMessage)
{
	if (psMessage == NULL )
		return ;

	if(psMessage->m_pvUserData == (void*)s_nUserData)
	{
		
		switch (psMessage->m_eEvent)
		{
		case EM_UDITUNER_SIGNAL_CONNECTED:
			{
				s_nTestData = 0;                        //��Ƶ�ɹ�
			}
			break;
		case EM_UDITUNER_SIGNAL_LOST:
			{
				s_nTestData = 1;                       //����תΪʧ��
			}
			break;
		default:
			break;
		}
	}
}

//@CASEGROUP:CSUDIEW200SGetTuneParam
//@DESCRIPTION: ���Ժ���CSUDIEW200SGetTuneParam��������ȷ����ʱ��ִ�н��
//@PRECONDITION:�ź����������ӣ������ź�
//@INPUT:1�� nOrNetId�� nTsId��nServiceId
//@INPUT:2��CSUDITunerAddCallback
//@INPUT:3��CSUDITunerConnect
//@INPUT:4���ص���Ϣ��Ӧ����ֵs_ntestData
//@EXPECTATION:����CSUDIEW200SGetTuneParam����������CSUDI_TRUE
//@EXECUTIONFLOW: 1������CSUDIEW200SGetTuneParam�����Ƶ����Ϣ
//@EXECUTIONFLOW: 2������ CSUDITunerAddCallback��CSUDITunerConnect�����û�ȡ������Ϣ������Ƶ����Ƶ�ɹ����ȡ������Դ��Ϣ��Ч
//@EXECUTIONFLOW: 3������ CSUDIOSThreadSleep�ȴ�2s
//@EXECUTIONFLOW: 4���鿴�ص���Ϣ��Ӧ����Ϣs_ntestData��ֵ������ֵ���
BOOL CSTC_EW200SERVICE_MT_0013(void)
{
	int nTsId =1;
	int nOrNetId = 100;
	int nServiceId = 103;
	CSUDI_Error_Code ret;
	CSUDIEW200SSourceParam_S sSourceParam;
	memset(&sSourceParam, 0, sizeof( sSourceParam));
	
	ret = CSUDIEW200SGetTuneParam(nOrNetId, nTsId, nServiceId, &sSourceParam);
	CSTK_ASSERT_TRUE_FATAL(ret == CSUDI_SUCCESS, "����1ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback( sSourceParam.m_dwTunerId, TunerCallback, (void *)s_nUserData), "ע��ص�ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(sSourceParam.m_dwTunerId, &sSourceParam.m_punDeliver), "��Ƶʧ��\n");
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL((s_nTestData == 0 || s_nTestData == 1), "��Ƶʧ��\n");

	CSTK_FATAL_POINT	
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SGetTuneParam
//@DESCRIPTION: ���Ժ���CSUDIEW200SGetTuneParam������������ʱ��ִ�н��
//@PRECONDITION:None
//@INPUT:������������Ͻӿ���Ƶ�Ҫ��
//@INPUT:1���������nOrNetId=-1,nTsId��nServiceId�ֱ�������ļ��ж�ȡ��ֵ
//@INPUT:2���������nTsId=-1��nOrNetId��nServiceId�ֱ�������ļ��ж�ȡ��ֵ
//@INPUT:3���������nServiceId=-1��nOrNetId��nTsId�ֱ�������ļ��ж�ȡ��ֵ
//@EXPECTATION:����CSUDIEW200S_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1���������nOrNetId=-1ʱ,����CSUDIEW200SGetTuneParam,������������ֵ��ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 2���������nTsId=-1ʱ,����CSUDIEW200SGetTuneParam,������������ֵ��ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 3���������nServiceId=-1ʱ,����CSUDIEW200SGetTuneParam,������������ֵ��ΪCSUDI_SUCCESS
BOOL CSTC_EW200SERVICE_MT_0014(void)
{
	CSUDI_Error_Code ret = CSUDI_SUCCESS;
	CSUDIEW200SSourceParam_S sSourceParam;
	int nTsId = 10;
	int nOrNetId = 1;
	int nServiceId = 103;
	
	memset(&sSourceParam , 0 , sizeof( sSourceParam));
	ret = CSUDIEW200SGetTuneParam(-1, nTsId, nServiceId, &sSourceParam);
	CSTK_ASSERT_TRUE_FATAL(ret != CSUDI_SUCCESS, "����1ʧ��");
	
	ret = CSUDIEW200SGetTuneParam(nOrNetId, -1, nServiceId, &sSourceParam);
	CSTK_ASSERT_TRUE_FATAL(ret != CSUDI_SUCCESS, "����2ʧ��");
	
	ret = CSUDIEW200SGetTuneParam(nOrNetId, nTsId, -1, &sSourceParam);
	CSTK_ASSERT_TRUE_FATAL(ret != CSUDI_SUCCESS, "����3ʧ��");

	CSTK_FATAL_POINT
		
	return TRUE;
}

CSUDI_BOOL initChInfo(CSUDITunerSRCDeliver_U *auDeliverList)
{
	char acTemp[32] = {0};
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","CAB_TS1_FREQ",acTemp, sizeof(acTemp)), "�������ļ���ȡƵ��ʧ��\n");
	auDeliverList[0].m_sCableDeliver.m_uFrequency = CSTKGetIntFromStr(acTemp, 10);
		
	memset(acTemp, 0 , sizeof(acTemp));
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","CAB_TS1_SR",acTemp, sizeof(acTemp)), "�������ļ���ȡ������ʧ��\n");
	auDeliverList[0].m_sCableDeliver.m_uSymbolRate_24 = CSTKGetIntFromStr(acTemp, 10);
		
	memset(acTemp, 0 , sizeof(acTemp));
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","CAB_TS1_MODE",acTemp, sizeof(acTemp)), "�������ļ���ȡ���Ʒ�ʽʧ��\n");
	auDeliverList[0].m_sCableDeliver.m_uModulation_8 = CSTKGetIntFromStr(acTemp, 10);

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEW200SGetIPQamDomainInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SGetIPQamDomainInfo��������ȷ����ʱ��ִ�н��
//@PRECONDITION:�����Ե�������������
//@INPUT:1��Ƶ����Ϣ����auDeliverList
//@INPUT:2��Ƶ�����
//@INPUT:3��nPid��ucTableId��char caIpQamDataBuffer[16]��nBufferLength = sizeof(caIpQamDataBuffer)��pvReserved=NULL
//@EXPECTATION:��������CSUDI_TRUE, ����ֵ������ֵ���
//@EXECUTIONFLOW: 1������CSUDIEW200SGetIPQamDomainInfo���IPQam
//@EXECUTIONFLOW: 2���Ƚϻ�ȡ����ֵ��Ԥ�ڵ�ֵ���������
BOOL CSTC_EW200SERVICE_MT_0015(void)
{
	CSUDITunerSRCDeliver_U auDeliverList[s_nDeliverCnt];
	memset(&auDeliverList, 0, sizeof(auDeliverList));
	initChInfo(auDeliverList);
	char acIpQamDataBuffer[16] = {0};
	char acIPQam[16]={0};
	memset(acIpQamDataBuffer, 0, sizeof(acIpQamDataBuffer));
	CSUDI_BOOL bRet = CSUDI_FALSE;
	
	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, 0, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == bRet, "��ȡIPQam��Ϣʧ��\n");
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_IPQAM", acIPQam, sizeof(acIPQam)), "�������ļ���ȡIPQAM����Ϣʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acIpQamDataBuffer, acIPQam), "��ȡ��IPQAM����");

	CSTK_FATAL_POINT
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SGetIPQamDomainInfo
//@DESCRIPTION: ���Ժ���CSUDIEW200SGetIPQamDomainInfo������������ʱ��ִ��Ч��
//@PRECONDITION:None
//@INPUT:���벻���Ͻӿ���ƵĲ������
///@INPUT:1��auDeliverListΪ��,nDeliverCount=0,nPid=0,ucTableId=0,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:2��auDeliverList�����ڣ�nDeliverCount=s_nDeliverCnt,nPid=0,ucTableId=0,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:3��auDeliverList��ȷ,nDeliverCount=0,nPid=0,ucTableId=0,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:4��auDeliverList��ȷ,nDeliverCount=s_nDeliverCnt,nPid=-1,ucTableId=0,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:5��auDeliverList��ȷ,nDeliverCount=s_nDeliverCnt,nPid=0,ucTableId=-1,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:6��auDeliverList��ȷ,nDeliverCount=s_nDeliverCnt,nPid=0,ucTableId=0,NULL,sizeof(pIpQamDataBuffer),NULL
//@INPUT:7��auDeliverList��ȷ,nDeliverCount=s_nDeliverCnt,nPid=0,ucTableId=0,pIpQamDataBuffer,0,NULL
//@EXPECTATION:��������º�������CSUDI_FALSE
//@EXECUTIONFLOW: 1��������CSUDIEW200SGetIPQamDomainInfo������յ�Ƶ����Ϣ��������,��������ֵΪCSUDI_FALSE
//@EXECUTIONFLOW: 2��������CSUDIEW200SGetIPQamDomainInfo�����벻���ڵ�Ƶ����Ϣ��������,��������ֵΪCSUDI_FALSE
//@EXECUTIONFLOW: 3��������CSUDIEW200SGetIPQamDomainInfo����������nDeliverCount�����,��������ֵΪCSUDI_FALSE
//@EXECUTIONFLOW: 4��������CSUDIEW200SGetIPQamDomainInfo����������nPid�����,��������ֵΪCSUDI_FALSE
//@EXECUTIONFLOW: 5��������CSUDIEW200SGetIPQamDomainInfo����������ucTableId�����,��������ֵΪCSUDI_FALSE
//@EXECUTIONFLOW: 6��������CSUDIEW200SGetIPQamDomainInfo�������СΪ0�����ڱ���IPQam��Ϣ������acIpQamDataBuffer�����,��������ֵΪCSUDI_FALSE
//@EXECUTIONFLOW: 7��������CSUDIEW200SGetIPQamDomainInfo������nBufferLength=0�����,��������ֵΪCSUDI_FALSE
BOOL CSTC_EW200SERVICE_MT_0016(void)
{
	CSUDITunerSRCDeliver_U auDeliverList[s_nDeliverCnt];
	memset(&auDeliverList, 0, sizeof(auDeliverList));
	char acIpQamDataBuffer[16] = {0};
	memset(acIpQamDataBuffer, 0, sizeof(acIpQamDataBuffer));
	CSUDI_BOOL bRet = CSUDI_TRUE;
	
	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, 0, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "����1ʧ��");

	auDeliverList[0].m_sCableDeliver.m_uFrequency = 307111;
	auDeliverList[0].m_sCableDeliver.m_uSymbolRate_24 = 6875;
	auDeliverList[0].m_sCableDeliver.m_uModulation_8 = 3;
	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, 0, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "����2ʧ��");

	initChInfo(auDeliverList);
	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, 0, 0, 0, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "����3ʧ��");

	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, -1, 0, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "����4ʧ��");

	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, -1, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "����5ʧ��");

	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, 0, NULL, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "����6ʧ��");

	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, 0, acIpQamDataBuffer, 0, NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "����7ʧ��");

	CSTK_FATAL_POINT
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SPlayStream & CSUDIEW200SSetPlayRect & CSUDIEW200SStopPlay
//@DESCRIPTION: ���Ժ���CSUDIEW200SPlayStream��CSUDIEW200SSetPlayRect��CSUDIEW200SStopPlay��������ȷ�Ĳ���ʱ��ִ��Ч��
//@PRECONDITION:����ָ����Ҫ�صĲ��������ҹ����Ե�������������
//@INPUT:������ȷ�Ĳ���
//@INPUT:1��dvb��ʽ��URL��nplayId=0
//@INPUT:2��dvbc��ʽ��URL��nplayId=0
//@INPUT:3��avpid��ʽ��URL��nplayId=0
//@EXPECTATION:����ָ�����ڴ�С�������Ž�Ŀ��ֹͣ��Ŀ����
//@EXECUTIONFLOW: 1������CSUDIEW200SSetPlayRect�����ò��Ŵ���Ϊȫ��
//@EXECUTIONFLOW: 2������CSUDIEW200SPlayStream����ָ����dvbc��ʽ�������в���
//@EXECUTIONFLOW: 3������CSTKWaitYes��������Աȷ����Ƶ�Ƿ��ѳɹ�ȫ������
//@EXECUTIONFLOW: 4������CSUDIEW200SSetPlayRect�����ò��Ŵ��ڵ���ϢΪ0,0,500,500
//@EXECUTIONFLOW: 5������CSTKWaitYes��������Ա��Ƶ����0,0,500,500�Ĵ��ڸ�ʽ��������
//@EXECUTIONFLOW: 6������CSUDIEW200SStopPlayֹͣ��Ƶ����
//@EXECUTIONFLOW: 7������CSTKWaitYes��������Ա������Ŀ����ֹͣ����
BOOL CSTC_EW200SERVICE_MT_0017(void)
{ 
	CSUDIWinRect_S sRect;
	memset(&sRect, 0, sizeof(sRect));
	sRect.m_nX = 0;
	sRect.m_nY = 0;
	sRect.m_nWidth = 500;
	sRect.m_nHeight = 500;
	
	CSUDIEW200SSetPlayRect(NULL,0); 
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl,0);
	CSTCPrint("��Ƶȫ��������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����2ʧ��");    

	CSUDIEW200SSetPlayRect(&sRect,0); 
	CSTCPrint("��Ƶ������0,0,500,500��λ����ʾ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����4ʧ��");    

	CSTCPrint("��Ƶ��Ŀ����ֹͣ��������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����6ʧ��");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SPlayStream & CSUDIEW200SSetPlayRect & CSUDIEW200SStopPlay
//@DESCRIPTION: ���Ժ���CSUDIEW200SPlayStream������������ʱ��ִ��Ч��
//@PRECONDITION:�����Ե�������������
//@INPUT:���벻������ƵĲ������
//@INPUT:1��dvbc��ʽ��URLͷ������ʽΪhttp://OriginalNetworkId.TS_Id.ServiceId
//@INPUT:2��dvbc��ʽ��URL��ServiceId����,��ʽΪdvbc://490000000:6875:64:-1
//@INPUT:3��URLΪ��
//@EXPECTATION:�����������Ž�Ŀ
//@EXECUTIONFLOW: 1���������Ϊ�����URL��ַ
//@EXECUTIONFLOW: 2������CSUDIEW200SSetPlayRect�����ò��Ŵ���Ϊȫ��
//@EXECUTIONFLOW: 3������CSUDIEW200SPlayStream���Կ�URL���в��ţ�������Ŀ������������
//@EXECUTIONFLOW: 4������CSTKWaitYes��������Աȷ�Ͻ�Ŀ�Ƿ񲥷�
//@EXECUTIONFLOW: 5������CSUDIEW200SStopPlay��ֹͣ��Ƶ����
//@EXECUTIONFLOW: 6������CSUDIEW200SPlayStream����http://403000000:6875:64:103��ʽ��URL���в��ţ�������Ŀ������������
//@EXECUTIONFLOW: 7������CSTKWaitYes��������Աȷ�Ͻ�Ŀ�Ƿ񲥷�
//@EXECUTIONFLOW: 8������CSUDIEW200SStopPlay��ֹͣ��Ƶ����
//@EXECUTIONFLOW: 9������CSUDIEW200SPlayStream����dvbc://403:6875:64:103��ʽ��URL���в��ţ�������Ŀ������������
//@EXECUTIONFLOW: 10������CSTKWaitYes��������Աȷ�Ͻ�Ŀ�Ƿ񲥷�
//@EXECUTIONFLOW: 11������CSUDIEW200SStopPlay��ֹͣ��Ƶ����
BOOL CSTC_EW200SERVICE_MT_0018(void)
{ 
	char* pcTesturl_wrongHead = "http://403000000:6875:64:103";
	char* pcTesturl_wrongItem = "dvbc://403:6875:64:103";
	char* pcTestwrongurl = "";
	
	CSUDIEW200SSetPlayRect(NULL,0);
	CSUDIEW200SPlayStream(pcTesturl_wrongHead, 0);
	CSTCPrint("��Ŀ���ܲ�����\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����3ʧ��");    
	CSUDIEW200SStopPlay(0);
	
	CSUDIEW200SPlayStream(pcTesturl_wrongItem, 0);
	CSTCPrint("��Ŀ���ܲ�����\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����6ʧ��");    
	CSUDIEW200SStopPlay(0);
	
	CSUDIEW200SPlayStream(pcTestwrongurl,0);
	CSTCPrint("��Ŀ���ܲ�����\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����9ʧ��");    
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SPlayStream & CSUDIEW200SSetPlayRect & CSUDIEW200SStopPlay
//@DESCRIPTION: ���Ժ���CSUDIEW200SSetPlayRect������������ʱ��ִ��Ч��
//@PRECONDITION:�����Ե�������������
//@INPUT:���벻������ƵĵĲ������
//@INPUT:1��CSUDIStreamInfo_S psRect, psRect->m_nX=0�� psRect->m_nY=0�� psRect->m_nWidth=0�� psRect->m_nHeight=500
//@INPUT:2��CSUDIStreamInfo_S psRect, psRect->m_nX=0�� psRect->m_nY=0�� psRect->m_nWidth=500�� psRect->m_nHeight=0
//@EXPECTATION:��Ŀ���ܰ���ָ���Ĵ��ڴ�С����
//@EXECUTIONFLOW: 1������CSUDIEW200SSetPlayRect�����ò��Ŵ�������Ϊ0,0,0,500
//@EXECUTIONFLOW: 2������CSUDIEW200SPlayStream,��dvbc��ʽ��URL���в��ţ�������Ŀ���ŵ���Ƶ������ʾ��Ϊ0,0,0,500
//@EXECUTIONFLOW: 3������CSTKWaitYes��������Աȷ���Ƿ��Ŀ���Ŵ��ڵ�����Ϊ0,0,0,500
//@EXECUTIONFLOW: 4������CSUDIEW200SSetPlayRect�����ò��Ŵ��ڵ�����Ϊ0,0,500,0
//@EXECUTIONFLOW: 5������CSTKWaitYes��������Աȷ���Ƿ��Ŀ���Ŵ��ڵ�����Ϊ0,0,500,0
//@EXECUTIONFLOW: 6������CSUDIEW200SStopPlay��ֹͣ��Ŀ����
BOOL CSTC_EW200SERVICE_MT_0019(void)
{
	CSUDIWinRect_S sRect_wrong;
	memset(&sRect_wrong, 0, sizeof(sRect_wrong));
	sRect_wrong.m_nX = 0;
	sRect_wrong.m_nY = 0;
	sRect_wrong.m_nWidth = 0;
	sRect_wrong.m_nHeight = 500; 
	
	CSUDIEW200SSetPlayRect(&sRect_wrong, 0); 
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSTCPrint("��Ƶ��Ŀ���Ŵ��ڵľ���������Ϊ0,0,0,500��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����1ʧ��");    
	
	sRect_wrong.m_nWidth = 500;
	sRect_wrong.m_nHeight = 0;
	CSUDIEW200SSetPlayRect(&sRect_wrong, 0); 
	CSTCPrint("��Ƶ��Ŀ���Ŵ��ڵ�����Ϊ0,0,500,0��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����4ʧ��");    
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SPlayStream & CSUDIEW200SSetPlayRect & CSUDIEW200SStopPlay          
//@DESCRIPTION:������һ����Ŀ�������ŵ�ʱ��Խӿڽ��ж�ε��õ����
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT: ����������URLΪdvbc://403000000.6875.64.67
//@EXPECTATION: ��Ŀ�Ĳ���״̬����Ӱ��
//@REMARK: ��Ҫ�ǲ���һ����Ŀ�������ŵ�ʱ����������������õ�Ӱ��
//@EXECUTIONFLOW:1��CSUDIEW200SPlayStream����urlΪdvbc://403000000.6875.64.67����
//@EXECUTIONFLOW:2������CSTKWaitYes������Ա������Ŀ��������
//@EXECUTIONFLOW:3��ѭ������CSUDIEW200SPlayStream����10�Σ�������Ŀ����Ӱ����Ȼ��������
//@EXECUTIONFLOW:4������CSTKWaitYes������Ա������Ŀ��Ȼ��������
//@EXECUTIONFLOW:5������CSUDIEW200SSetPlayRect������Ƶ���Ŵ���Ϊȫ��
//@EXECUTIONFLOW:6������CSTKWaitYes������Ա������Ŀ���������Ҵ���Ϊȫ��
//@EXECUTIONFLOW:7������CSUDIEW200SSetPlayRect����10�Σ�������Ŀ���������Ų���Ӱ��
//@EXECUTIONFLOW:8������CSTKWaitYes������Ա������Ŀ����������
//@EXECUTIONFLOW:9������CSUDIEW200SSetPlayRect������Ƶ���Ŵ�������Ϊ0,0,500,500
//@EXECUTIONFLOW:10������CSTKWaitYes������Ա������Ŀ���������Ҵ�������Ϊ0,0,500,500
//@EXECUTIONFLOW:11������CSUDIEW200SSetPlayRect����10�Σ�������Ŀ���������Ų���Ӱ��
//@EXECUTIONFLOW:12������CSTKWaitYes������Ա������Ŀ����������
//@EXECUTIONFLOW:13������CSTKWaitYes������Աȷ����Ŀ��ֹͣ���
//@EXECUTIONFLOW:14������CSUDIEW200SStopPlay��ֹͣ��Ŀ�����
BOOL CSTC_EW200SERVICE_MT_0024(void)
{
	int i = 0;
	CSUDIWinRect_S sRect;
	memset(&sRect, 0, sizeof(sRect));
	sRect.m_nX = 0;
	sRect.m_nY = 0;
	sRect.m_nWidth = 500;
	sRect.m_nHeight = 500;
	
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSTCPrint("��Ƶ��Ŀ����������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����2ʧ��\n");    
	while(i < s_nRunCnt)
	{
		CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
		i++;
	}
	CSTCPrint("��Ƶ��Ŀ������������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����4ʧ��\n");    
	
	CSUDIEW200SSetPlayRect(NULL, 0);
	CSTCPrint("��Ƶ��Ŀȫ��������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����6ʧ��\n");  

	i = 0;
	while(i < s_nRunCnt)
	{
		CSUDIEW200SSetPlayRect(NULL, 0);
		i++;
	}
	CSTCPrint("��Ƶ��Ŀȫ������������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����8ʧ��\n"); 

	CSUDIEW200SSetPlayRect(&sRect,0); 
	CSTCPrint("��Ƶ������0,0,500,500��λ����ʾ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����10ʧ��"); 

	i = 0;
	while(i < s_nRunCnt)
	{
		CSUDIEW200SSetPlayRect(&sRect, 0);
		i++;
	}
	CSTCPrint("��Ƶ��������0,0,500,500��λ����ʾ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����12ʧ��");

	CSTCPrint("��Ƶ��Ŀ����ֹͣ��������\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "����14ʧ��");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
	
	return TRUE;
}

static int fnCallback(unsigned int dwMsg, CSUDIEW200SParam_S *psParam, void* pUserData)
{
	
	if( pUserData!=NULL)
	{
		switch(dwMsg)
	{
		case EM_EW_MSG_VOD_PLAYSTATE_PAUSESUCC:
		{
			s_anTestData[0] = 0;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_PAUSEFAILED:
		{
			s_anTestData[1] = 1;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_RESUMESUCC:
		{
			s_anTestData[2] = 2;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_RESUMEFAILED:
		{
			s_anTestData[3] = 3;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SETRATESUCC:
		{
			s_anTestData[4] = 4;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SETRATEFAILED:
		{
			s_anTestData[5] = 5;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SEEKSUCC:
		{
			s_anTestData[6] = 6;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SEEKFAILED:
		{
			s_anTestData[7] = 7;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SERVERERROR:
		{
			s_anTestData[10] = 10;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_TOSTART:
		{
			s_anTestData[11] = 11;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_TOEND:
		{
			s_anTestData[12] = 12;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SERVER_REDIRECT:
		{
			s_anTestData[13] = 13;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SERVER_OPTIONS:
		{
			s_anTestData[14] = 14;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SERVER_GETPARAMETER:
		{
			s_anTestData[15] = 15;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SERVER_SETPARAMETER:
		{
			s_anTestData[16] = 16;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_DESCRIBE_SUCC:
		{
			s_anTestData[17] = 17;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_DESCRIBE_FAIL:
		{
			s_anTestData[18] = 18;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SETUP_SUCC:
		{
			s_anTestData[19] = 19;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SETUP_FAIL:
		{
			s_anTestData[20] = 20;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_AUTH_SUCC:
		{
			s_anTestData[21] = 21;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_AUTH_FAIL:
		{
			s_anTestData[22] = 22;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SERVER_NODATA:
		{
			s_anTestData[23] = 23;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_LIVE_TO_TIMESHIFT:
		{
			s_anTestData[24] = 24;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_TIMESHIFT_TO_LIVE:
		{
			s_anTestData[25] = 25;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SAVE_BOOK_MARK:
		{
			s_anTestData[26] = 26;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_GET_START_TIME:
		{
			s_anTestData[27] = 27;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_PLAYSUCC:
		{
			s_anTestData[28] = 28;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_SHAKEHAND_TIMEOUT:
		{
			s_anTestData[30] = 30;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_INVALID:
		{
			s_anTestData[31] = 31;
			break;
		}
		case EM_EW_MSG_VOD_CONNECT_SUCCESS:
		{
			s_anTestData[90] = 90;
			break;
		}
		case EM_EW_MSG_VOD_CONNECT_FAILED:
		{
			s_anTestData[91] = 91;
			break;
		}
		case EM_EW_MSG_VOD_NO_PROGRAME:
		{
			s_anTestData[92] = 92;
			break;
		}
		case EM_EW_MSG_VOD_LOAD_SUCCESS:
		{
			s_anTestData[93] = 93;
			break;
		}
		case EM_EW_MSG_VOD_NOT_SUPPORT:
		{
			s_anTestData[94] = 94;
			break;
		}case EM_EW_MSG_VOD_FRONTTS_STOP:
		{
			s_anTestData[95] = 95;
			break;
		}
		case EM_EW_MSG_VOD_FRONTTS_RUN:
		{
			s_anTestData[96] = 96;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_READY:
		{
			s_anTestData[4096] = 4096;
			break;
		}
		case EM_EW_MSG_VOD_PLAYSTATE_PLAYFAILED:
		{
			s_anTestData[4097] = 4097;
			break;
		}
		 case EM_EW_MSG_VOD_CARD_VALID:
		 {
		 	s_anTestData[4352] = 4352;
			break;
		 }
   		case EM_EW_MSG_VOD_CARD_INVALID:
		{
			s_anTestData[4353] = 4353;
			break;
   		}
    		case EM_EW_MSG_VOD_CARD_PREPARING:
		{
			s_anTestData[4354] = 4354;
			break;
    		}
			
		default:
		s_anTestData[4500] = -1;	
		break;
	}
	}
	return 0;
	
}


//@CASEGROUP:CSUDIEW200SAddCallback
//@DESCRIPTION: ���Ժ���CSUDIEW200SAddCallback��������ȷ�Ĳ���ʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:���в��������Ͻӿ���Ƶ�Ҫ��
//@INPUT:1��nPlayId=0
//@INPUT:2��s_nCallbackUserData = 0x1234;
//@INPUT:3��fncallback
//@INPUT:4��s_anTestData[4501];   
//@EXPECTATION:����CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������ncallbackTestdataΪ���Իص������Ƿ���Ӧ��Ϣ��ֵ
//@EXECUTIONFLOW: 2������CSUDIEW200SAddCallbackע��ص���������������ֵΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIEW200SPlayStream������Ϣ��������Ŀ��������
//@EXECUTIONFLOW: 4������CSUDIOSThreadSleep�ȴ�2s���жϻص���������Ϣ��Ӧ�ֱ�ΪEM_EW_MSG_VOD_CONNECT_SUCCESS��\
//@EXECUTIONFLOW:EM_EW_MSG_VOD_LOAD_SUCCESS��EM_EW_MSG_VOD_PLAYSTATE_READY
//@EXECUTIONFLOW: 5������CSUDIEW200SDelCallbackɾ��ע��Ļص�����������CSUDI_SUCCESS
BOOL CSTC_EW200SERVICE_MT_0020(void)
{
	memset(s_anTestData, 0, sizeof(s_anTestData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SAddCallback(0, fnCallback, (void*)s_nCallbackUserData), "����2ʧ��\n");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl,0);
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL((s_anTestData[90] == 90) && (s_anTestData[93] == 93) && (s_anTestData[4096] == 4096), "����3ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "����5ʧ��\n"); 
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SAddCallback
//@DESCRIPTION: ���Ժ���CSUDIEW200SAddCallback������fncallback = CSUDI_NULLʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:���������Ͻӿ���Ƶ�Ҫ��
//@INPUT:1��nPlayId=0
//@INPUT:2��s_nCallbackUserData = 0x1234;
//@INPUT:3��fncallback = CSUDI_NULL
//@INPUT:4��s_anTestData[4501];   
//@EXPECTATION:�ص�����ע�᲻�ɹ������ط�CSUDI_SUCCESS�����Ե�ֵ����
//@EXECUTIONFLOW: 1������ncallbackTestdataΪ���Իص������Ƿ񱻵��õ�ֵ
//@EXECUTIONFLOW: 2������CSUDIEW200SAddCallback���(pnUserData Ϊ0x1234������Ϸ���ֵַ)����������ֵΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIOSThreadSleep�ȴ�2s������ncallbackTestdata��ֵ����
//@EXECUTIONFLOW: 4������CSUDIEW200SDelCallbackɾ��ע��Ļص�����������ֵΪCSUDIEW200S_ERROR_NO_CALLBACK
BOOL CSTC_EW200SERVICE_MT_0021(void)
{
	memset(s_anTestData, 0, sizeof(s_anTestData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIEW200SAddCallback(0, CSUDI_NULL, (void*)s_nCallbackUserData), "����2ʧ��\n");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl,0);
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL(s_anTestData[4500] == 0, "����2ʧ��");
	CSUDIEW200SStopPlay(0);
	CSTK_ASSERT_TRUE_FATAL(CSUDIEW200S_ERROR_NO_CALLBACK == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "����4ʧ��\n");
	
	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SAddCallback
//@DESCRIPTION: ���Ժ���CSUDIEW200SAddCallback������s_nCallbackUserData=CSUDI_NULLʱ��ִ��Ч��
//@PRECONDITION:�ź����������ӣ������Ե�������������
//@INPUT:���������Ͻӿ���Ƶ�Ҫ��
//@INPUT:1��nPlayId=0
//@INPUT:2��s_nCallbackUserData = CSUDI_NULL
//@INPUT:3��fncallback
//@INPUT:4��s_anTestData[4501];   
//@EXPECTATION:�ص�����ע��ɹ�������CSUDI_SUCCESS��������Ӧ��Ϣ��ֵ����
//@EXECUTIONFLOW: 1������ncallbackTestdataΪ���Իص������Ƿ񱻵��õ�ֵ
//@EXECUTIONFLOW: 2������CSUDIEW200SAddCallback���(pnUserData ΪΪCSUDI_NULL)�ص���������������ֵΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIEW200SPlayStream������Ϣ��������Ŀ��������
//@EXECUTIONFLOW: 4������CSUDIOSThreadSleep�ȴ�2s������ncallbackTestdata��ֵ����
//@EXECUTIONFLOW: 5������CSUDIEW200SDelCallbackɾ��ע��Ļص�����������ֵΪCSUDIEW200S_ERROR_NO_CALLBACK
BOOL CSTC_EW200SERVICE_MT_0022(void)
{
	memset(s_anTestData, 0, sizeof(s_anTestData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SAddCallback(0, fnCallback, CSUDI_NULL), "����2ʧ��\n");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl,0);
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL(s_anTestData[4500] == 0, "����3ʧ��\n");
	CSUDIEW200SStopPlay(0);
	CSTK_ASSERT_TRUE_FATAL(CSUDIEW200S_ERROR_NO_CALLBACK == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "����5ʧ��\n\n"); 
	
	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SAddCallback & CSUDIEW200SDelCallback
//@DESCRIPTION: ���Ժ���CSUDIEW200SAddCallback������ע������ɾ��������ͬ�Ļص�ʱ��ִ��Ч��
//@PRECONDITION:��
//@INPUT:���в��������Ͻӿ���Ƶ�Ҫ��
//@INPUT:1��nPlayId=0
//@INPUT:2��s_nCallbackUserData = 0x1234
//@INPUT:3��fncallback
//@INPUT:4��s_anTestData[4501];   
//@EXPECTATION:��һ��ע�᷵��CSUDI_SUCCESS���ڶ���ע�᷵��CSUDIEW200S_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 1������ncallbackTestdataΪ���Իص������Ƿ񱻵��õ�ֵ
//@EXECUTIONFLOW: 2������CSUDIEW200SAddCallbackע��ص���������������ֵΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIEW200SAddCallbackע���벽��2��ͬ�Ļص���������������ֵ��ΪCSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIEW200SDelCallbackɾ��ע��Ļص�,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIEW200SDelCallbackɾ��ע��Ļص�,��������CSUDIEW200S_ERROR_NO_CALLBACK
BOOL CSTC_EW200SERVICE_MT_0023(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SAddCallback(0, fnCallback, (void*)s_nCallbackUserData), "����2ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIEW200S_ERROR_ALREADY_ADDED == CSUDIEW200SAddCallback(0, fnCallback, (void*)s_nCallbackUserData), "����3ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "����4ʧ��\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIEW200S_ERROR_NO_CALLBACK == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "����5ʧ��\n");

	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SAddCallback  & CSUDIEW200SDelCallback
//@DESCRIPTION:���Բ���ȫ���Ϸ�ʱע��5���ص������ܷ�ע��ص��ɹ�
//@PRECONDITION:�豸��ʼ���ɹ�
//@INPUT:1��nPlayId = 0
//@INPUT:2��fnCallback = fncallback
//@INPUT:3��UserData = anUserData[i]
//@EXPECTATION:ȫ��ע��ɹ�
//@EXECUTIONFLOW:1������CSUDIEW200SAddCallback����ע��5���ص�������ע��ĺ��������ͬ��UserData��ͬ��Ҫ��ȫ�����سɹ��������������ʧ��
//@EXECUTIONFLOW:2������CSUDIEW200SDelCallbackɾ��ע��ɹ��Ļص�������Ҫ�󷵻سɹ�
BOOL CSTC_EW200SERVICE_MT_0025(void)
{
	int anUserData[5] = {0};
	int nAddCallbackCount = 5;
	int i;

	for(i=0 ; i<nAddCallbackCount ; i++)
	{			
		anUserData[i] = i*10+i;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SAddCallback(0,fnCallback,&anUserData[i]), "����1ʧ��");
	}
			
	for(i=0; i<nAddCallbackCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SDelCallback(0, fnCallback, &anUserData[i]),"����2ʧ��");
	}
			
	CSTK_FATAL_POINT
	
	return TRUE;
}




