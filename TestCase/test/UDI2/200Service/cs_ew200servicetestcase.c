/* --------------------------------------------------------------------
×¢Òâ£º
1.ÔÚÐèÒªÓëÓÃ»§½»»¥µÄ²âÊÔÓÃÀýÖÐ£¬¿ÉÒÔ£º
	a. Ê¹ÓÃCSTKWaitAnyKeyµÈ´ýÓÃ»§ÊäÈëÈÎÒâ°´¼ü
	b. Ê¹ÓÃCSTKWaitYesµÈ´ýÓÃ»§ÊäÈëYES
2.²âÊÔÓÃÀýº¯ÊýÃüÃû£º²âÊÔÓÃÀýID£¬"²âÊÔÓÃÀýID"¶¨ÒåÔÚ²âÊÔÓÃÀýÎÄµµÖÐ
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

static int s_nDeliverCnt = 1;     //Æµµã¸öÊý
static int s_nRunCnt = 10;

static char* s_pcTestDvbcUrl = "dvbc://659000000.6875.64.67";     
static int s_nUserData = 12345;          //tunercallback
static int s_nTestData = -1; 
static int s_nCallbackUserData = 0x1234;
static int s_anTestData[4501];            //test addcallback function

BOOL CSTC_EW200SERVICE_Init(void)
{
	//ÔÚ±¾²âÊÔÓÃÀý¼¯Ö´ÐÐÇ°µ÷ÓÃ
	return TRUE;
}

BOOL CSTC_EW200SERVICE_UnInit(void)
{
	//ÔÚ±¾²âÊÔÓÃÀý¼¯Ö´ÐÐºóµ÷ÓÃ
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SReadClientInfo 
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SReadClientInfoÔÚ²ÎÊý¶¼ÕýÈ·Ê±µÄÖ´ÐÐÐ§¹û(Ö÷Òª¶ÔµÚÈý·½Ìá¹©µÄEM_UDIEW200S_CARDID, \
//EM_UDIEW200S_SN,EM_UDIEW200S_PROVIDER,EM_UDIEW200S_ALIAS,EM_UDIEW200S_MODULE,EM_UDIEW200S_LOCAL_IP,EM_UDIEW200S_LOCAL_MAC, \
//EM_UDIEW200S_CITV_PSD,EM_UDIEW200S_CITV_SERVERIP,EM_UDIEW200S_CITV_SERVERPORT,EM_UDIEW200S_CITV_VIDEOTYPE,EM_UDIEW200S_CITV_BROWSERVER²âÊÔ)
//@PRECONDITION:µÚÈý·½Ìá¹©ÅäÖÃÐÅÏ¢
//@INPUT:1¡¢·Ö±ðÊäÈëCSUDIEW200SClientInfoType_EÃ¶¾ÙÖÐµÄÖµ
//@INPUT:2¡¢Òª¶ÁÈ¡³öµÄCSUDIEW200SClientInfoType_EÖÐ¸÷Ã¶¾ÙÀàÐÍ¶ÔÓ¦µÄÐÅÏ¢acValue
//@INPUT:3¡¢¶ÁÈ¡³öµÄÐÅÏ¢³¤¶ÈÎªsizeof(acValue)
//@EXPECTATION:ÆÚÍû¶ÁÈ¡³öµÄÖµÓë´ÓÅäÖÃÖÐ»ñµÃ¶ÔÓ¦µÄÖµÏàµÈ
//@EXECUTIONFLOW:1¡¢´ÓÅäÖÃÎÄ¼þÖÐ·Ö±ð»ñµÃEM_UDIEW200S_CARDID, \
//@EXECUTIONFLOW:EM_UDIEW200S_SN,EM_UDIEW200S_PROVIDER,EM_UDIEW200S_ALIAS,EM_UDIEW200S_MODULE,EM_UDIEW200S_LOCAL_IP,EM_UDIEW200S_LOCAL_MAC, \
//@EXECUTIONFLOW:EM_UDIEW200S_CITV_PSD,EM_UDIEW200S_CITV_SERVERIP,EM_UDIEW200S_CITV_SERVERPORT,EM_UDIEW200S_CITV_VIDEOTYPE,EM_UDIEW200S_CITV_BROWSERVER¶ÔÓ¦µÄÖµ
//@EXECUTIONFLOW:2¡¢µ÷ÓÃº¯ÊýCSUDIEW200SReadClientInfo·Ö±ð¶ÁÈ¡EM_UDIEW200S_CARDID, \
//@EXECUTIONFLOW:EM_UDIEW200S_SN,EM_UDIEW200S_PROVIDER,EM_UDIEW200S_ALIAS,EM_UDIEW200S_MODULE,EM_UDIEW200S_LOCAL_IP,EM_UDIEW200S_LOCAL_MAC, \
//@EXECUTIONFLOW:EM_UDIEW200S_CITV_PSD,EM_UDIEW200S_CITV_SERVERIP,EM_UDIEW200S_CITV_SERVERPORT,EM_UDIEW200S_CITV_VIDEOTYPE,EM_UDIEW200S_CITV_BROWSERVERµÄÈÏÖ¤ÐÅÏ¢
//@EXECUTIONFLOW:3¡¢±È½Ï´ÓÅäÖÃÎÄ¼þÖÐ»ñµÃµÄÖµÓëµ÷ÓÃº¯ÊýCSUDIEW200SReadClientInfo¶ÁÈ¡³öµÄÖµ£¬ÆÚÍûÁ½ÕßÏàµÈ
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
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CARDID", acCardID, sizeof(acCardID)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡Ö»ÄÜ¿¨ºÅÊ§°Ü\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_SN", acSn, sizeof(acSn)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡ÐòÁÐºÅÊ§°Ü\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_PROVIDER", acProvider, sizeof(acProvider)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡ÖÕ¶ËÌá¹©ÉÌÐÅÏ¢Ê§°Ü\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_ALIAS", acAlias, sizeof(acAlias)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡ÖÕ¶Ë±ðÃûÊ§°Ü\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_MODULE", acModule, sizeof(acModule)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡ÖÕ¶ËÄ£ÐÍÊ§°Ü\n")
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_LOCAL_IP", acLocalIp, sizeof(acLocalIp)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡±¾µØIPµØÖ·Ê§°Ü\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_LOCAL_MAC", acLocalMac, sizeof(acLocalMac)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡±¾µØMACµØÖ·Ê§°Ü\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_PSD", acPsd, sizeof(acPsd)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡ÓÃ»§ÃÜÂëÊ§°Ü\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_SERVERIP", acServerIp, sizeof(acServerIp)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡·þÎñÆ÷IPÊ§°Ü\n");
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_SERVERPORT", acTemp, sizeof(acTemp)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡·þÎñÆ÷¶Ë¿ÚºÅÊ§°Ü\n");
	nServerPort = CSTKGetIntFromStr(acTemp, 10);
		
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_VIDEOTYPE", acVideoType, sizeof(acVideoType)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡ÊÓÆµÀàÐÍÊ§°Ü\n")
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_CITV_BROWSERVER", acBrowserver, sizeof(acBrowserver)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡ä¯ÀÀÆ÷°æ±¾ºÅÊ§°Ü\n");

	eType = EM_UDIEW200S_CARDID;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡ÖÇÄÜ¿¨ÐÅÏ¢Ê§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue, acCardID), "¶ÁÈ¡³öµÄÐòÁÐºÅÐÅÏ¢´íÎó\n");

	eType = EM_UDIEW200S_SN;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡ÐòÁÐºÅÐÅÏ¢Ê§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue, acSn), "¶ÁÈ¡³öµÄÖÇÄÜ¿¨ÐÅÏ¢´íÎó\n");

	eType = EM_UDIEW200S_PROVIDER;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡¹©Ó¦ÉÌÐÅÏ¢Ê§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acProvider), "¶ÁÈ¡³öµÄ¹©Ó¦ÉÌÐÅÏ¢´íÎó\n");

	eType = EM_UDIEW200S_ALIAS;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡ÖÕ¶Ë±ðÃûÐÅÏ¢Ê§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acAlias), "¶ÁÈ¡³öµÄÖÕ¶Ë±ðÃûÐÅÏ¢´íÎó\n");

	eType = EM_UDIEW200S_MODULE;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡ÖÕ¶ËÐÍºÅÐÅÏ¢Ê§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acModule), "¶ÁÈ¡³öµÄÖÕ¶ËÐÍºÅÐÅÏ¢´íÎó\n");

	eType = EM_UDIEW200S_LOCAL_IP;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡±¾»úIPµØÖ·Ê§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acLocalIp), "¶ÁÈ¡³öµÄ±¾»úIPµØÖ·´íÎó\n");

	eType = EM_UDIEW200S_LOCAL_MAC;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡±¾»úMACµØÖ·Ê§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acLocalMac), "¶ÁÈ¡³öµÄ±¾»úMACµØÖ·´íÎó\n");

	eType = EM_UDIEW200S_CITV_PSD;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡ÓÃ»§ÃÜÂëÊ§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acPsd), "¶ÁÈ¡³öµÄÓÃ»§ÃÜÂë´íÎó\n");

	eType = EM_UDIEW200S_CITV_SERVERIP;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡·þÎñÆ÷IPÊ§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acServerIp), "¶ÁÈ¡³öµÄ·þÎñÆ÷IP´íÎó\n");

	eType = EM_UDIEW200S_CITV_SERVERPORT;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)&nValue, sizeof(int)), "¶ÁÈ¡·þÎñÆ÷¶Ë¿ÚºÅÊ§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(nValue == nServerPort, "¶ÁÈ¡³öµÄ·þÎñÆ÷¶Ë¿ÚºÅ´íÎó\n");

	eType = EM_UDIEW200S_CITV_VIDEOTYPE;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡ÊÓÆµ½âÂë¸ñÊ½Ê§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acVideoType), "¶ÁÈ¡³öµÄÊÓÆµ½âÂë¸ñÊ½´íÎó\n");
	
	eType = EM_UDIEW200S_CITV_BROWSERVER;
	memset(acValue, 0, sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(eType, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡ä¯ÀÀÆ÷°æ±¾ºÅÊ§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acValue,  acBrowserver), "¶ÁÈ¡³öµÄä¯ÀÀÆ÷°æ±¾ºÅ´íÎó\n");

	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SReadClientInfo 
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SReadClientInfoÓëCSUDIEW200SWriteClientInfoÔÚ²ÎÊý¶¼ÕýÈ·Ê±µÄÖ´ÐÐÐ§¹û(Õë¶ÔuseridºÍtokenµÄ²âÊÔ)
//@PRECONDITION:
//@INPUT:1¡¢EM_UDIEW200S_CITV_USERID, acValue,sizeof(acValue)
//@INPUT:2¡¢EM_UDIEW200S_CITV_TOKEN,acValue,sizeof(acValue)
//@INPUT:3¡¢¶ÁÈ¡³öµÄÐÅÏ¢³¤¶ÈÎªsizeof(acValue)
//@EXPECTATION:ÆÚÍûÐ´ÈëµÄÖµÓë¶ÁÈ¡³öµÄÖµÏàµÈ
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIEW200SWriteClientInfoÐ´ÈëUSERID£¬È»ºóµ÷ÓÃCSUDIEW200SReadClientInfo¶ÁÈ¡USERID£¬ÆÚÍûÐ´ÈëµÄÐÅÏ¢Óë¶ÁÈ¡³öµÄÐÅÏ¢ÏàµÈ
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIEW200SWriteClientInfoÐ´ÈëTOKEN£¬È»ºóµ÷ÓÃCSUDIEW200SReadClientInfo¶ÁÈ¡TOKEN£¬ÆÚÍûÐ´ÈëµÄÐÅÏ¢Óë¶ÁÈ¡³öµÄÐÅÏ¢ÏàµÈ
BOOL CSTC_EW200SERVICE_MT_0002(void)
{
	char* pcUserID = "123456";
	char* pcToken = "555555";
	char acValue[32] = {0};
	
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(EM_UDIEW200S_CITV_USERID, (void*)pcUserID, strlen(pcUserID)), "Ð´ÈëÓÃ»§IDÊ§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(EM_UDIEW200S_CITV_USERID, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡ÓÃ»§IDÊ§°Ü\n"	);
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(pcUserID, acValue), "Ð´ÈëµÄÓÃ»§ID´íÎó");

	memset(acValue, 0 , sizeof(acValue));
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(EM_UDIEW200S_CITV_TOKEN, (void*)pcToken, strlen(pcToken)), "Ð´ÈëÁîÅÆÐÅÏ¢Ê§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SReadClientInfo(EM_UDIEW200S_CITV_TOKEN, (void*)acValue, sizeof(acValue)), "¶ÁÈ¡ÁîÅÆÐÅÏ¢Ê§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(pcToken, acValue), "Ð´ÈëµÄÁîÅÆ´íÎó");

	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SReadClientInfo 
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SReadClientInfoÔÚÊäÈë²ÎÊý²»ÕýÈ·Ê±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:None
//@INPUT:ÊäÈë²»·ûºÏ½Ó¿ÚÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢eType=EM_UDIEW200S_CARDID-1, acValue,sizeof(acValue)
//@INPUT:2¡¢eType=EM_UDIEW200S_PROGRAMEINFO+1, acValue,sizeof(acValue)
//@INPUT:3¡¢eType=EM_UDIEW200S_CARDID,acValue,length=0
//@EXPECTATION:ÆÚÍûÉÏÊöÈýÖÖÇé¿öÏÂº¯ÊýCSUDIEW200SReadClientInfoµÄ·µ»Ø¶¼Îª-1
//@EXECUTIONFLOW: 1¡¢ÊäÈë²ÎÊýeType=EM_UDIEW200S_CARDID-1¡¢acValue¡¢sizeof(acValue)£¬µ÷ÓÃCSUDIEW200SReadClientInfoÆÚÍûº¯Êý·µ»ØÖµÎª-1
//@EXECUTIONFLOW: 2¡¢ÊäÈë²ÎÊýeType=EM_UDIEW200S_PROGRAMEINFO+1¡¢acValue¡¢sizeof(acValue)£¬µ÷ÓÃCSUDIEW200SReadClientInfoÆÚÍûº¯Êý·µ»ØÖµÎª-1
//@EXECUTIONFLOW: 3¡¢ÊäÈë²ÎÊýEM_UDIEW200S_CARDID¡¢acValue¡¢0£¬µ÷ÓÃCSUDIEW200SReadClientInfoÆÚÍûº¯Êý·µ»ØÖµÎª-1
BOOL CSTC_EW200SERVICE_MT_0003(void)
{
	char acValue[32] = {0};
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SReadClientInfo(EM_UDIEW200S_CARDID-1, (void*)acValue, sizeof(acValue)), "²½Öè1Ê§°Ü");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SReadClientInfo(EM_UDIEW200S_PROGRAMEINFO+1, (void*)acValue, sizeof(acValue)), "²½Öè2Ê§°Ü");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SReadClientInfo(EM_UDIEW200S_CARDID, (void*)acValue, 0), "²½Öè3Ê§°Ü");

	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SWriteClientInfoÔÚ²ÎÊý¶¼·ûºÏÉè¼ÆÒªÇóÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²ÎÊý¾ù·ûºÏÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢ÊäÈëCSUDIEW200SClientInfoType_EµÄÖµÎªEM_UDIEW200S_ISMUTE
//@INPUT:2¡¢ÒªÐ´ÈëµÄeTypeµÄÐÅÏ¢·Ö±ðÎªCSUDI_TRUEºÍCSUDI_FALSE
//@INPUT:3¡¢Ð´ÈëµÄÐÅÏ¢³¤¶ÈÎªsizeof(int)
//@EXPECTATION:³É¹¦Ð´ÈëÐÅÏ¢£¬ÇÒÐ´ÈëµÄÐÅÏ¢ÉúÐ§£¬²âÊÔÈËÔ±ÄÜ¿´µ½Ð´ÈëÐÅÏ¢µÄÏàÓ¦Ð§¹û
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃº¯ÊýCSUDIEW200SWriteClientInfo£¬Ð´Èë¾²ÒôÏûÏ¢CSUDI_TRUE
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SPlayStream²¥·Å½ÚÄ¿,ÆÚÍû²âÊÔÈËÔ±Ìýµ½µÄÒôÆµÐ§¹ûÎª¾²Òô
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±¶ÔÐ´ÈëCSUDI_TRUEÖ´ÐÐÐ§¹ûµÄ·´À¡
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃº¯ÊýCSUDIEW200SWriteClientInfo£¬Ð´Èë¾²ÒôÏûÏ¢CSUDI_FALSE£¬ÆÚÍû²âÊÔÈËÔ±¿´µ½µÄÊÓÆµÐ§¹ûÎª·Ç¾²Òô
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±¶ÔÖ´ÐÐÐ§¹ûµÄ·´À¡
BOOL CSTC_EW200SERVICE_MT_0004(void)
{
	CSUDI_BOOL bIsmute = CSUDI_TRUE;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_ISMUTE;
	
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&bIsmute, sizeof(int)), "²½Öè1Ê§°Ü");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSTCPrint("ÒôÆµ²¥·ÅÎª¾²ÒôÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "ÉèÖÃ¾²ÒôÎÞÐ§");
	
	bIsmute = CSUDI_FALSE;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&bIsmute, sizeof(int)), "²½Öè4Ê§°Ü");
	CSTCPrint("ÒôÆµ²¥·ÅÓÐÉùÒôÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "ÉèÖÃ·Ç¾²ÒôÎÞÐ§");                      
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SWriteClientInfoÔÚ²ÎÊý¶¼·ûºÏÉè¼ÆÒªÇóÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²ÎÊý¾ù·ûºÏÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢ÊäÈëCSUDIEW200SClientInfoType_EµÄÖµÎªEM_UDIEW200S_VOLUME
//@INPUT:2¡¢ÒªÐ´ÈëµÄeTypeµÄÐÅÏ¢valueÎª0-31Ö®¼äµÄÖµÒÀ´ÎÔö´ó
//@INPUT:3¡¢Ð´ÈëµÄÐÅÏ¢³¤¶ÈÎªsizeof(int)
//@EXPECTATION:³É¹¦Ð´ÈëÐÅÏ¢£¬ÇÒÐ´ÈëµÄÐÅÏ¢ÉúÐ§£¬²âÊÔÈËÔ±ÄÜ¿´µ½Ð´ÈëÐÅÏ¢µÄÏàÓ¦Ð§¹û
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SPlayStream²¥·ÅÊÓÆµ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SWriteClientInfo´Ó0-31Ã¿Ò»²½¼Ó5Ñ­»·Ð´ÈëÒôÁ¿Öµ£¬ÆÚ´ý²âÊÔÈËÔ±Ìýµ½ÉùÒô½¥Ç¿
//@EXECUTIONFLOW: 3¡¢Ã¿´Î¸Ä±äÖµºóµ÷ÓÃCSUDIOSThreadSleepµÈ´ý5s£¬ÈÃ²âÊÔÈËÔ±¿´Ð§¹û
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±·´À¡Ð´ÈëÒôÁ¿µÄÐ§¹û
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSUDIEW200SStopPlayÍ£Ö¹ÊÓÆµµÄ²¥·Å
BOOL CSTC_EW200SERVICE_MT_0005(void)
{
	CSUDIEW200SClientInfoType_E eType  = EM_UDIEW200S_VOLUME;
	int  nVolume = 0;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	while(nVolume <= 31)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nVolume, sizeof(int)), "²½Öè2Ê§°Ü");
		nVolume += 5;
		CSUDIOSThreadSleep(5000);
	}

	CSTCPrint("ÒôÆµµÄÉùÒô³öÏÖÉùÒô½¥Ç¿µÄÇé¿öÁËÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "Ð´ÈëÒôÁ¿ÎÞÐ§");                      			  
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SWriteClientInfoÔÚ²ÎÊý¶¼·ûºÏÉè¼ÆÒªÇóÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²ÎÊý¾ù·ûºÏÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢ÊäÈëCSUDIEW200SClientInfoType_EµÄÖµÎªEM_UDIEW200S_VIDEOALPHA
//@INPUT:2¡¢ÒªÐ´ÈëµÄeTypeµÄÐÅÏ¢value
//@INPUT:3¡¢Ð´ÈëµÄÐÅÏ¢³¤¶ÈÎªsizeof(int)
//@EXPECTATION:³É¹¦Ð´ÈëÐÅÏ¢£¬ÇÒÐ´ÈëµÄÐÅÏ¢ÉúÐ§£¬²âÊÔÈËÔ±ÄÜ¿´µ½Ð´ÈëÐÅÏ¢µÄÏàÓ¦Ð§¹û
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SPlayStream²¥·ÅÊÓÆµ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SWriteClientInfo´Ó0-100Ã¿Ò»´Î¼Ó10Ñ­»·Ð´ÈëÍ¸Ã÷¶ÈµÄÖµ£¬ÆÚ´ý²âÊÔÈËÔ±¿´µ½ÊÓÆµÓÉºÚ±äÁÁµÄÐ§¹û
//@EXECUTIONFLOW: 3¡¢Ã¿´Î¸Ä±äÖµºóµ÷ÓÃCSUDIOSThreadSleepµÈ´ý5s£¬ÈÃ²âÊÔÈËÔ±¿´Ð§¹û
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±·´À¡Ð´ÈëÍ¸Ã÷¶ÈµÄÐ§¹û
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSUDIEW200SWriteClientInfo,Ð´ÈëÍ¸Ã÷¶ÈÖµÎª50£¬Ê¹ÊÓÆµÕý³£
//@EXECUTIONFLOW: 6¡¢µ÷ÓÃCSUDIEW200SStopPlayÍ£Ö¹ÊÓÆµµÄ²¥·Å
BOOL CSTC_EW200SERVICE_MT_0006(void)
{
	
	int  nVideoAlpha = 0;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_VIDEOALPHA;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);	
	
	while(nVideoAlpha <= 100)
	{			
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nVideoAlpha, sizeof(int)), "²½Öè2Ê§°Ü");
		nVideoAlpha += 10;	
		CSUDIOSThreadSleep(5000);
	}
	
	CSTCPrint("¿ªÊ¼²¥·ÅÊÓÆµºó£¬ÊÓÆµÓÉÈ«ºÚÖð½¥±äµÃÍ¸Ã÷Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "Ð´ÈëÍ¸Ã÷¶ÈÎÞÐ§");        

	nVideoAlpha = 50;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nVideoAlpha, sizeof(int)), "Ð´ÈëÍ¸Ã÷¶ÈÊ§°Ü");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT	
	
	CSUDIEW200SStopPlay(0);	
	
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SWriteClientInfoÔÚ²ÎÊý¶¼·ûºÏÉè¼ÆÒªÇóÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²ÎÊý¾ù·ûºÏÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢ÊäÈëCSUDIEW200SClientInfoType_EµÄÖµÎªEM_UDIEW200S_BRIGHTNESS
//@INPUT:2¡¢ÒªÐ´ÈëµÄeTypeµÄÐÅÏ¢value
//@INPUT:3¡¢Ð´ÈëµÄÐÅÏ¢³¤¶ÈÎªsizeof(int)
//@EXPECTATION:³É¹¦Ð´ÈëÐÅÏ¢£¬ÇÒÐ´ÈëµÄÐÅÏ¢ÉúÐ§£¬²âÊÔÈËÔ±ÄÜ¿´µ½Ð´ÈëÐÅÏ¢µÄÏàÓ¦Ð§¹û
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SPlayStream²¥·ÅÊÓÆµ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SWriteClientInfo´Ó0-100Ã¿Ò»´Î¼Ó10Ñ­»·Ð´ÈëÁÁ¶ÈµÄÖµ£¬ÆÚ´ý²âÊÔÈËÔ±¿´µ½ÊÓÆµÓÉºÚ±äÇåÎú£¬ÔÙ±ä³ÉÈ«°×µÄÐ§¹û
//@EXECUTIONFLOW: 3¡¢Ã¿´Î¸Ä±äÖµºóµ÷ÓÃCSUDIOSThreadSleepµÈ´ý5s£¬ÈÃ²âÊÔÈËÔ±¿´Ð§¹û
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±·´À¡Ð´ÈëÁÁ¶ÈµÄÐ§¹û
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSUDIEW200SWriteClientInfo,Ð´ÈëÁÁ¶ÈÖµÎª50£¬Ê¹ÊÓÆµÕý³£
//@EXECUTIONFLOW: 6¡¢µ÷ÓÃCSUDIEW200SStopPlayÍ£Ö¹ÊÓÆµµÄ²¥·Å
BOOL CSTC_EW200SERVICE_MT_0007(void)
{
	int  nBrightness = 0;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_BRIGHTNESS;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	while(nBrightness <= 100)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nBrightness, sizeof(int)), "²½Öè2Ê§°Ü");
		nBrightness += 10;
		CSUDIOSThreadSleep(5000);
	}
	CSTCPrint("ÊÓÆµ¿ªÊ¼²¥·Åºó£¬ÊÓÆµ³öÏÖÓÉºÚ±äÇåÎú£¬ÔÙ±ä³ÉÈ«°×Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "Ð´ÈëÊÓÆµÁÁ¶ÈÎÞÐ§");                      			  

	nBrightness = 50;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nBrightness, sizeof(int)), "²½Öè5Ê§°Ü");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SWriteClientInfoÔÚ²ÎÊý¶¼·ûºÏÉè¼ÆÒªÇóÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²ÎÊý¾ù·ûºÏÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢ÊäÈëCSUDIEW200SClientInfoType_EµÄÖµÎªEM_UDIEW200S_CONTRAST
//@INPUT:2¡¢ÒªÐ´ÈëµÄeTypeµÄÐÅÏ¢value
//@INPUT:3¡¢Ð´ÈëµÄÐÅÏ¢³¤¶ÈÎªsizeof(int)
//@EXPECTATION:³É¹¦Ð´ÈëÐÅÏ¢£¬ÇÒÐ´ÈëµÄÐÅÏ¢ÉúÐ§£¬²âÊÔÈËÔ±ÄÜ¿´µ½Ð´ÈëÐÅÏ¢µÄÏàÓ¦Ð§¹û
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SPlayStream²¥·ÅÊÓÆµ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SWriteClientInfo´Ó0-100Ã¿Ò»´Î¼Ó10Ð´Èë¶Ô±È¶ÈµÄÖµ£¬ÆÚ´ý²âÊÔÈËÔ±¿´µ½ÊÓÆµÓÉ²»ÇåÎú±äÇåÎúµÄÐ§¹û
//@EXECUTIONFLOW: 3¡¢Ã¿´Î¸Ä±äÖµºóµ÷ÓÃCSUDIOSThreadSleepµÈ´ý5s£¬ÈÃ²âÊÔÈËÔ±¿´Ð§¹û
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±·´À¡Ð´Èë¶Ô±È¶ÈµÄÐ§¹û
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSUDIEW200SWriteClientInfo,Ð´Èë¶Ô±È¶ÈÖµÎª50£¬Ê¹ÊÓÆµÕý³£
//@EXECUTIONFLOW: 6¡¢µ÷ÓÃCSUDIEW200SStopPlayÍ£Ö¹ÊÓÆµµÄ²¥·Å
BOOL CSTC_EW200SERVICE_MT_0008(void)
{
	int  nContrast = 0;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_CONTRAST;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	while(nContrast <= 100)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nContrast, sizeof(int)), "²½Öè2Ê§°Ü");
		 nContrast += 10;
		 CSUDIOSThreadSleep(5000);
	}
	CSTCPrint("¿ªÊ¼²¥·ÅÊÓÆµºó£¬ÊÓÆµÓÉ²»ÇåÎú±äµÃÖð½¥ÇåÎúÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "Ð´ÈëÊÓÆµ¶Ô±È¶ÈÎÞÐ§");                      			  

	nContrast = 50;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nContrast, sizeof(int)), "²½Öè5Ê§°Ü");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SWriteClientInfoÔÚ²ÎÊý¶¼·ûºÏÉè¼ÆÒªÇóÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²ÎÊý¾ù·ûºÏÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢ÊäÈëCSUDIEW200SClientInfoType_EµÄÖµÎªEM_UDIEW200S_SATURATION
//@INPUT:2¡¢ÒªÐ´ÈëµÄeTypeµÄÐÅÏ¢value
//@INPUT:3¡¢Ð´ÈëµÄÐÅÏ¢³¤¶ÈÎªsizeof(int)
//@EXPECTATION:³É¹¦Ð´ÈëÐÅÏ¢£¬ÇÒÐ´ÈëµÄÐÅÏ¢ÉúÐ§£¬²âÊÔÈËÔ±ÄÜ¿´µ½Ð´ÈëÐÅÏ¢µÄÏàÓ¦Ð§¹û
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SPlayStream²¥·ÅÊÓÆµ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SWriteClientInfo´Ó0-100Ã¿Ò»´Î¼Ó10Ð´Èë±¥ºÍ¶ÈµÄÖµ£¬ÆÚ´ý²âÊÔÈËÔ±¿´µ½ÊÓÆµÓÉ»Ò±äµÃÖð½¥ÇåÎúµÄÐ§¹û
//@EXECUTIONFLOW: 3¡¢Ã¿´Î¸Ä±äÖµºóµ÷ÓÃCSUDIOSThreadSleepµÈ´ý5s£¬ÈÃ²âÊÔÈËÔ±¿´Ð§¹û
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±·´À¡Ð´Èë±¥ºÍ¶ÈµÄÐ§¹û
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSUDIEW200SWriteClientInfo,Ð´Èë±¥ºÍ¶ÈÖµÎª50£¬Ê¹µÃÊÓÆµÄÜ¿´¼û
//@EXECUTIONFLOW: 6¡¢µ÷ÓÃCSUDIEW200SStopPlayÍ£Ö¹ÊÓÆµµÄ²¥·Å
BOOL CSTC_EW200SERVICE_MT_0009(void)
{
	int  nSaturation = 0;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_SATURATION;

	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	while(nSaturation <= 100)
	{
		CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nSaturation, sizeof(int)), "²½Öè2Ê§°Ü");
		nSaturation += 10;
		CSUDIOSThreadSleep(5000);
	}
	CSTCPrint("¿ªÊ¼²¥·ÅÊÓÆµºó£¬ÊÓÆµÑÕÉ«¶Ô±È¶ÈÓÉ»Ò±äµÃÖð½¥ÇåÎúÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "Ð´ÈëÊÓÆµ±¥ºÍ¶ÈÎÞÐ§\n");                      			  

	nSaturation = 50;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nSaturation, sizeof(int)), "²½Öè5Ê§°Ü\n");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;    
}
	
//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SWriteClientInfoÔÚ²ÎÊý¶¼·ûºÏÉè¼ÆÒªÇóÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²ÎÊý¾ù·ûºÏÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢ÊäÈëCSUDIEW200SClientInfoType_EµÄÖµÎªEM_UDIEW200S_ASPECTRATIO
//@INPUT:2¡¢ÒªÐ´ÈëµÄeTypeµÄÐÅÏ¢value
//@INPUT:3¡¢Ð´ÈëµÄÐÅÏ¢³¤¶ÈÎªsizeof(int)
//@EXPECTATION:³É¹¦Ð´ÈëÐÅÏ¢£¬ÇÒÐ´ÈëµÄÐÅÏ¢ÉúÐ§£¬²âÊÔÈËÔ±ÄÜ¿´µ½Ð´ÈëÐÅÏ¢µÄÏàÓ¦Ð§¹û
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SWriteClientInfoÐ´ÈëÐÅÏ¢EM_UDIEW200S_WIN_ASPECT_RATIO_4_3
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SPlayStream,ÆÚÍû²âÊÔÈËÔ±¿´µ½µÄÐ§¹ûÎªÆÁÄ»¿í¸ß±ÈÎª4:3
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSUDIEW200SWriteClientInfoÐ´ÈëÐÅÏ¢EM_UDIEW200S_WIN_ASPECT_RATIO_16_9
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSUDIEW200SPlayStream,ÆÚÍû²âÊÔÈËÔ±¿´µ½µÄÐ§¹ûÎªÆÁÄ»¿í¸ß±ÈÎª16:9
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±·´À¡Ð´Èë²»Í¬ÊÓÆµµÄÐ§¹û
//@EXECUTIONFLOW: 6¡¢µ÷ÓÃCSUDIEW200SStopPlayÍ£Ö¹²¥·ÅÊÓÆµ
BOOL CSTC_EW200SERVICE_MT_0010(void)
{
	int  nCSUDI_ASPECTRATIO = EM_UDIEW200S_WIN_ASPECT_RATIO_4_3;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_ASPECTRATIO;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nCSUDI_ASPECTRATIO, sizeof(int)), "²½Öè1Ê§°Ü");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSTCPrint("ÊÓÆµ¿í¸ß±ÈÎª4:3Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "Ð´ÈëµÄÊÓÆµ¿í¸ß±È4:3ÎÞÐ§");  
	
	nCSUDI_ASPECTRATIO = EM_UDIEW200S_WIN_ASPECT_RATIO_16_9;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nCSUDI_ASPECTRATIO, sizeof(int)), "²½Öè3Ê§°Ü");
	CSTCPrint("ÊÓÆµ¿í¸ß±ÈÎª16:9Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "Ð´ÈëÊÓÆµ¿í¸ß±È16:9ÎÞÐ§");       
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SWriteClientInfoÔÚ²ÎÊý¶¼·ûºÏÉè¼ÆÒªÇóÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²ÎÊý¾ù·ûºÏÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢ÊäÈëCSUDIEW200SClientInfoType_EÃ¶¾ÙÖÐµÄÖµEM_UDIEW200S_STOPMODE
//@INPUT:2¡¢ÒªÐ´ÈëµÄeTypeµÄÐÅÏ¢value
//@INPUT:3¡¢Ð´ÈëµÄÐÅÏ¢³¤¶ÈÎªsizeof(int)
//@EXPECTATION:³É¹¦Ð´ÈëÐÅÏ¢£¬ÇÒÐ´ÈëµÄÐÅÏ¢ÉúÐ§£¬²âÊÔÈËÔ±ÄÜ¿´µ½Ð´ÈëÐÅÏ¢µÄÏàÓ¦Ð§¹û
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SWriteClientInfoÐ´ÈëeType=EM_UDIEW200S_STOPMODEÊ±µÄÐÅÏ¢EM_UDIEW200S_STOPMODE_BLACK
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SPlayStream²¥·ÅÊÓÆµ 
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSUDIOSThreadSleepµÈ´ý5s
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSUDIEW200SStopPlayÍ£Ö¹ÊÓÆµ²¥·Å£¬ÆÚÍû²âÊÔÈËÔ±¿´µ½ÊÓÆµÍ£Ö¹²¥·ÅÊ±µÄÐ§¹ûÎªºÚÆÁ
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±·´À¡¿´µ½µÄÊÓÆµÍ£Ö¹Ê±µÄÐ§¹ûÎªºÚÆÁ
//@EXECUTIONFLOW: 6¡¢µ÷ÓÃCSUDIEW200SWriteClientInfoÐ´ÈëeType=EM_UDIEW200S_STOPMODEÊ±µÄÐÅÏ¢EM_UDIEW200S_STOPMODE_FREEZE
//@EXECUTIONFLOW: 7¡¢µ÷ÓÃCSUDIEW200SPlayStream²¥·ÅÊÓÆµ
//@EXECUTIONFLOW: 8¡¢µ÷ÓÃCSUDIOSThreadSleepµÈ´ý5s
//@EXECUTIONFLOW: 9¡¢µ÷ÓÃCSUDIEW200SStopPlayÍ£Ö¹ÊÓÆµ²¥·Å£¬ÆÚÍû²âÊÔÈËÔ±¿´µ½ÊÓÆµÍ£Ö¹²¥·ÅÊ±µÄÐ§¹ûÎª¾²Ö¡
//@EXECUTIONFLOW: 10¡¢µ÷ÓÃCSTKWaitYes,µÈ´ý²âÊÔÈËÔ±·´À¡¿´µ½µÄÊÓÆµÍ£Ö¹Ê±µÄÐ§¹ûÎª¾²Ö¡
BOOL CSTC_EW200SERVICE_MT_0011(void)
{
	int  nCSUDI_STOPMODE = EM_UDIEW200S_STOPMODE_BLACK;
	CSUDIEW200SClientInfoType_E eType = EM_UDIEW200S_STOPMODE;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nCSUDI_STOPMODE, sizeof(int)), "²½Öè1Ê§°Ü");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSUDIOSThreadSleep(5000);
	CSUDIEW200SStopPlay(0);
	CSTCPrint("ÊÓÆµÍ£Ö¹Ê±ÎªºÚÆÁÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "Ð´ÈëµÄÍ£Ö¹²¥·ÅÊ±ÊÓÆµÀàÐÍÎÞÐ§"); 
	
	nCSUDI_STOPMODE = EM_UDIEW200S_STOPMODE_FREEZE;
	CSTK_ASSERT_TRUE_FATAL(-1 != CSUDIEW200SWriteClientInfo(eType, (void*)&nCSUDI_STOPMODE, sizeof(int)), "²½Öè6Ê§°Ü");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
	CSUDIOSThreadSleep(5000);
	CSUDIEW200SStopPlay(0);
	CSTCPrint("ÊÓÆµÍ£Ö¹Ê±Îª¾²Ö¡Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "Ð´ÈëÍ£Ö¹²¥·ÅÊ±ÊÓÆµÀàÐÍÎÞÐ§"); 
	
	CSTK_FATAL_POINT
		
	return TRUE;    
}

//@CASEGROUP:CSUDIEW200SWriteClientInfo 
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SWriteClientInfoÔÚÊäÈë²ÎÊý²»ÕýÈ·Ê±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:None
//@INPUT:ÊäÈë²»·ûºÏ½Ó¿ÚÉè¼ÆÒªÇóµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢eType=EM_UDIEW200S_CARDID-1£¬ÆäËû²ÎÊýÕýÈ·
//@INPUT:2¡¢eType=EM_UDIEW200S_PROGRAMEINFO+1£¬ÆäËû²ÎÊýÕýÈ·
//@INPUT:3¡¢length=0£¬ÆäËû²ÎÊýÕýÈ·
//@INPUT:4¡¢value·Ç·¨Ê±£¬eType=EM_UDIEW200S_AUDIOPID,value=-1,length=sizeof(int)
//@EXPECTATION:º¯ÊýCSUDIEW200SWriteClientInfo·µ»Ø-1
//@EXECUTIONFLOW: 1¡¢ÊäÈë²ÎÊýeType=EM_UDIEW200S_CARDID-1£¬value = 700092200002, length = 32,µ÷ÓÃº¯ÊýCSUDIEW200SWriteClientInfoÐ´ÈëÐÅÏ¢£¬ÆÚÍûº¯Êý·µ»ØÖµÎª-1
//@EXECUTIONFLOW: 2¡¢ÊäÈë²ÎÊýeType=EM_UDIEW200S_PROGRAMEINFO+1£¬value = 700092200002, length = 32,µ÷ÓÃº¯ÊýCSUDIEW200SWriteClientInfoÐ´ÈëÐÅÏ¢£¬ÆÚÍûº¯Êý·µ»ØÖµÎª-1
//@EXECUTIONFLOW: 3¡¢ÊäÈë²ÎÊýeType=EM_UDIEW200S_CARDID£¬value = 700092200002, length = 0£¬µ÷ÓÃCSUDIEW200SWriteClientInfoÐ´ÈëÐÅÏ¢£¬ÆÚÍûº¯Êý·µ»ØÖµÎª-1
//@EXECUTIONFLOW: 4¡¢ÊäÈë²ÎÊýeType=EM_UDIEW200S_CARDID£¬value = NULL, length = 32£¬µ÷ÓÃCSUDIEW200SWriteClientInfoÐ´ÈëÐÅÏ¢£¬ÆÚÍûº¯Êý·µ»ØÖµÎª-1
BOOL CSTC_EW200SERVICE_MT_0012(void)
{
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SWriteClientInfo(EM_UDIEW200S_CARDID-1, "700092200002", 32), "²½Öè1Ê§°Ü");

	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SWriteClientInfo(EM_UDIEW200S_PROGRAMEINFO+1, "700092200002", 32), "²½Öè2Ê§°Ü");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SWriteClientInfo(EM_UDIEW200S_CARDID, "700092200002", 0), "²½Öè3Ê§°Ü");
	
	CSTK_ASSERT_TRUE_FATAL(-1 == CSUDIEW200SWriteClientInfo(EM_UDIEW200S_AUDIOPID, NULL, 32), "²½Öè4Ê§°Ü");

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
				s_nTestData = 0;                        //ËøÆµ³É¹¦
			}
			break;
		case EM_UDITUNER_SIGNAL_LOST:
			{
				s_nTestData = 1;                       //Ëø¶¨×ªÎªÊ§Ëø
			}
			break;
		default:
			break;
		}
	}
}

//@CASEGROUP:CSUDIEW200SGetTuneParam
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SGetTuneParamÔÚÊäÈëÕýÈ·²ÎÊýÊ±µÄÖ´ÐÐ½á¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬ÇÒÓÐÐÅºÅ
//@INPUT:1¡¢ nOrNetId¡¢ nTsId¡¢nServiceId
//@INPUT:2¡¢CSUDITunerAddCallback
//@INPUT:3¡¢CSUDITunerConnect
//@INPUT:4¡¢»Øµ÷ÏûÏ¢ÏìÓ¦²âÊÔÖµs_ntestData
//@EXPECTATION:µ÷ÓÃCSUDIEW200SGetTuneParam£¬ÆÚÍû·µ»ØCSUDI_TRUE
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SGetTuneParam£¬»ñµÃÆµµãÐÅÏ¢
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃ CSUDITunerAddCallbackÓëCSUDITunerConnect£¬ÀûÓÃ»ñÈ¡µ½µÄÐÅÏ¢½øÐÐËøÆµ£¬ËøÆµ³É¹¦Ôò»ñÈ¡µÄÊý¾ÝÔ´ÐÅÏ¢ÓÐÐ§
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃ CSUDIOSThreadSleepµÈ´ý2s
//@EXECUTIONFLOW: 4¡¢²é¿´»Øµ÷ÏûÏ¢ÏìÓ¦µÄÐÅÏ¢s_ntestDataµÄÖµÓëÆÚÍûÖµÏàµÈ
BOOL CSTC_EW200SERVICE_MT_0013(void)
{
	int nTsId =1;
	int nOrNetId = 100;
	int nServiceId = 103;
	CSUDI_Error_Code ret;
	CSUDIEW200SSourceParam_S sSourceParam;
	memset(&sSourceParam, 0, sizeof( sSourceParam));
	
	ret = CSUDIEW200SGetTuneParam(nOrNetId, nTsId, nServiceId, &sSourceParam);
	CSTK_ASSERT_TRUE_FATAL(ret == CSUDI_SUCCESS, "²½Öè1Ê§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerAddCallback( sSourceParam.m_dwTunerId, TunerCallback, (void *)s_nUserData), "×¢²á»Øµ÷Ê§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDITunerConnect(sSourceParam.m_dwTunerId, &sSourceParam.m_punDeliver), "ËøÆµÊ§°Ü\n");
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL((s_nTestData == 0 || s_nTestData == 1), "ËøÆµÊ§°Ü\n");

	CSTK_FATAL_POINT	
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SGetTuneParam
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SGetTuneParamÔÚÊäÈë´íÎó²ÎÊýÊ±µÄÖ´ÐÐ½á¹û
//@PRECONDITION:None
//@INPUT:ÊäÈë²ÎÊý²»·ûºÏ½Ó¿ÚÉè¼ÆµÄÒªÇó
//@INPUT:1¡¢ÊäÈë²ÎÊýnOrNetId=-1,nTsId¡¢nServiceId·Ö±ð´ÓÅäÖÃÎÄ¼þÖÐ¶ÁÈ¡ÆäÖµ
//@INPUT:2¡¢ÊäÈë²ÎÊýnTsId=-1£¬nOrNetId¡¢nServiceId·Ö±ð´ÓÅäÖÃÎÄ¼þÖÐ¶ÁÈ¡ÆäÖµ
//@INPUT:3¡¢ÊäÈë²ÎÊýnServiceId=-1£¬nOrNetId¡¢nTsId·Ö±ð´ÓÅäÖÃÎÄ¼þÖÐ¶ÁÈ¡ÆäÖµ
//@EXPECTATION:·µ»ØCSUDIEW200S_ERROR_BAD_PARAMETER
//@EXECUTIONFLOW: 1¡¢ÊäÈë²ÎÊýnOrNetId=-1Ê±,µ÷ÓÃCSUDIEW200SGetTuneParam,ÆÚÍûº¯Êý·µ»ØÖµ²»ÎªCSUDI_SUCCESS
//@EXECUTIONFLOW: 2¡¢ÊäÈë²ÎÊýnTsId=-1Ê±,µ÷ÓÃCSUDIEW200SGetTuneParam,ÆÚÍûº¯Êý·µ»ØÖµ²»ÎªCSUDI_SUCCESS
//@EXECUTIONFLOW: 3¡¢ÊäÈë²ÎÊýnServiceId=-1Ê±,µ÷ÓÃCSUDIEW200SGetTuneParam,ÆÚÍûº¯Êý·µ»ØÖµ²»ÎªCSUDI_SUCCESS
BOOL CSTC_EW200SERVICE_MT_0014(void)
{
	CSUDI_Error_Code ret = CSUDI_SUCCESS;
	CSUDIEW200SSourceParam_S sSourceParam;
	int nTsId = 10;
	int nOrNetId = 1;
	int nServiceId = 103;
	
	memset(&sSourceParam , 0 , sizeof( sSourceParam));
	ret = CSUDIEW200SGetTuneParam(-1, nTsId, nServiceId, &sSourceParam);
	CSTK_ASSERT_TRUE_FATAL(ret != CSUDI_SUCCESS, "²½Öè1Ê§°Ü");
	
	ret = CSUDIEW200SGetTuneParam(nOrNetId, -1, nServiceId, &sSourceParam);
	CSTK_ASSERT_TRUE_FATAL(ret != CSUDI_SUCCESS, "²½Öè2Ê§°Ü");
	
	ret = CSUDIEW200SGetTuneParam(nOrNetId, nTsId, -1, &sSourceParam);
	CSTK_ASSERT_TRUE_FATAL(ret != CSUDI_SUCCESS, "²½Öè3Ê§°Ü");

	CSTK_FATAL_POINT
		
	return TRUE;
}

CSUDI_BOOL initChInfo(CSUDITunerSRCDeliver_U *auDeliverList)
{
	char acTemp[32] = {0};
	
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","CAB_TS1_FREQ",acTemp, sizeof(acTemp)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡ÆµÂÊÊ§°Ü\n");
	auDeliverList[0].m_sCableDeliver.m_uFrequency = CSTKGetIntFromStr(acTemp, 10);
		
	memset(acTemp, 0 , sizeof(acTemp));
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","CAB_TS1_SR",acTemp, sizeof(acTemp)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡·ûºÅÂÊÊ§°Ü\n");
	auDeliverList[0].m_sCableDeliver.m_uSymbolRate_24 = CSTKGetIntFromStr(acTemp, 10);
		
	memset(acTemp, 0 , sizeof(acTemp));
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("UDI2","CAB_TS1_MODE",acTemp, sizeof(acTemp)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡µ÷ÖÆ·½Ê½Ê§°Ü\n");
	auDeliverList[0].m_sCableDeliver.m_uModulation_8 = CSTKGetIntFromStr(acTemp, 10);

	CSTK_FATAL_POINT
		
	return CSUDI_TRUE;
}

//@CASEGROUP:CSUDIEW200SGetIPQamDomainInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SGetIPQamDomainInfoÔÚÊäÈëÕýÈ·²ÎÊýÊ±µÄÖ´ÐÐ½á¹û
//@PRECONDITION:¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:1¡¢ÆµµãÐÅÏ¢Êý×éauDeliverList
//@INPUT:2¡¢Æµµã¸öÊý
//@INPUT:3¡¢nPid¡¢ucTableId¡¢char caIpQamDataBuffer[16]¡¢nBufferLength = sizeof(caIpQamDataBuffer)¡¢pvReserved=NULL
//@EXPECTATION:º¯Êý·µ»ØCSUDI_TRUE, ·µ»ØÖµÓëÆÚÍûÖµÏàµÈ
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SGetIPQamDomainInfo»ñµÃIPQam
//@EXECUTIONFLOW: 2¡¢±È½Ï»ñÈ¡µ½µÄÖµÓëÔ¤ÆÚµÄÖµ£¬ÆÚÍûÏàµÈ
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
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == bRet, "»ñÈ¡IPQamÐÅÏ¢Ê§°Ü\n");
	CSTK_ASSERT_TRUE(CS_TK_CONFIG_SUCCESS == CSTKGetConfigInfo("EW200Service","CSUDICFG_IPQAM", acIPQam, sizeof(acIPQam)), "´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡IPQAMµÄÐÅÏ¢Ê§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(0 == strcmp(acIpQamDataBuffer, acIPQam), "»ñÈ¡µÄIPQAM´íÎó");

	CSTK_FATAL_POINT
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SGetIPQamDomainInfo
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SGetIPQamDomainInfoÔÚÊäÈë´íÎó²ÎÊýÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:None
//@INPUT:ÊäÈë²»·ûºÏ½Ó¿ÚÉè¼ÆµÄ²ÎÊý×éºÏ
///@INPUT:1¡¢auDeliverListÎª¿Õ,nDeliverCount=0,nPid=0,ucTableId=0,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:2¡¢auDeliverList²»´æÔÚ£¬nDeliverCount=s_nDeliverCnt,nPid=0,ucTableId=0,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:3¡¢auDeliverListÕýÈ·,nDeliverCount=0,nPid=0,ucTableId=0,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:4¡¢auDeliverListÕýÈ·,nDeliverCount=s_nDeliverCnt,nPid=-1,ucTableId=0,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:5¡¢auDeliverListÕýÈ·,nDeliverCount=s_nDeliverCnt,nPid=0,ucTableId=-1,pIpQamDataBuffer,sizeof(pIpQamDataBuffer),NULL
//@INPUT:6¡¢auDeliverListÕýÈ·,nDeliverCount=s_nDeliverCnt,nPid=0,ucTableId=0,NULL,sizeof(pIpQamDataBuffer),NULL
//@INPUT:7¡¢auDeliverListÕýÈ·,nDeliverCount=s_nDeliverCnt,nPid=0,ucTableId=0,pIpQamDataBuffer,0,NULL
//@EXPECTATION:ÉÏÊöÇé¿öÏÂº¯Êý·µ»ØCSUDI_FALSE
//@EXECUTIONFLOW: 1¡¢²âÊÔÔÚCSUDIEW200SGetIPQamDomainInfoÖÐÊäÈë¿ÕµÄÆµµãÐÅÏ¢Êý×éµÄÇé¿ö,ÆÚÍû·µ»ØÖµÎªCSUDI_FALSE
//@EXECUTIONFLOW: 2¡¢²âÊÔÔÚCSUDIEW200SGetIPQamDomainInfoÖÐÊäÈë²»´æÔÚµÄÆµµãÐÅÏ¢Êý×éµÄÇé¿ö,ÆÚÍû·µ»ØÖµÎªCSUDI_FALSE
//@EXECUTIONFLOW: 3¡¢²âÊÔÔÚCSUDIEW200SGetIPQamDomainInfoÖÐÊäÈë´íÎóµÄnDeliverCountµÄÇé¿ö,ÆÚÍû·µ»ØÖµÎªCSUDI_FALSE
//@EXECUTIONFLOW: 4¡¢²âÊÔÔÚCSUDIEW200SGetIPQamDomainInfoÖÐÊäÈë´íÎóµÄnPidµÄÇé¿ö,ÆÚÍû·µ»ØÖµÎªCSUDI_FALSE
//@EXECUTIONFLOW: 5¡¢²âÊÔÔÚCSUDIEW200SGetIPQamDomainInfoÖÐÊäÈë´íÎóµÄucTableIdµÄÇé¿ö,ÆÚÍû·µ»ØÖµÎªCSUDI_FALSE
//@EXECUTIONFLOW: 6¡¢²âÊÔÔÚCSUDIEW200SGetIPQamDomainInfoÖÐÊäÈë´óÐ¡Îª0µÄÓÃÓÚ±£´æIPQamÐÅÏ¢µÄÊý×éacIpQamDataBufferµÄÇé¿ö,ÆÚÍû·µ»ØÖµÎªCSUDI_FALSE
//@EXECUTIONFLOW: 7¡¢²âÊÔÔÚCSUDIEW200SGetIPQamDomainInfoÖÐÊäÈënBufferLength=0µÄÇé¿ö,ÆÚÍû·µ»ØÖµÎªCSUDI_FALSE
BOOL CSTC_EW200SERVICE_MT_0016(void)
{
	CSUDITunerSRCDeliver_U auDeliverList[s_nDeliverCnt];
	memset(&auDeliverList, 0, sizeof(auDeliverList));
	char acIpQamDataBuffer[16] = {0};
	memset(acIpQamDataBuffer, 0, sizeof(acIpQamDataBuffer));
	CSUDI_BOOL bRet = CSUDI_TRUE;
	
	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, 0, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "²½Öè1Ê§°Ü");

	auDeliverList[0].m_sCableDeliver.m_uFrequency = 307111;
	auDeliverList[0].m_sCableDeliver.m_uSymbolRate_24 = 6875;
	auDeliverList[0].m_sCableDeliver.m_uModulation_8 = 3;
	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, 0, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "²½Öè2Ê§°Ü");

	initChInfo(auDeliverList);
	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, 0, 0, 0, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "²½Öè3Ê§°Ü");

	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, -1, 0, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "²½Öè4Ê§°Ü");

	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, -1, acIpQamDataBuffer, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "²½Öè5Ê§°Ü");

	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, 0, NULL, sizeof(acIpQamDataBuffer), NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "²½Öè6Ê§°Ü");

	bRet = CSUDIEW200SGetIPQamDomainInfo(auDeliverList, s_nDeliverCnt, 0, 0, acIpQamDataBuffer, 0, NULL);
	CSTK_ASSERT_TRUE_FATAL(CSUDI_FALSE == bRet, "²½Öè7Ê§°Ü");

	CSTK_FATAL_POINT
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SPlayStream & CSUDIEW200SSetPlayRect & CSUDIEW200SStopPlay
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SPlayStream¡¢CSUDIEW200SSetPlayRectºÍCSUDIEW200SStopPlayÔÚÊäÈëÕýÈ·µÄ²ÎÊýÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:´æÔÚÖ¸¶¨ÈýÒªËØµÄ²âÊÔÁ÷£¬ÇÒ¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈëÕýÈ·µÄ²ÎÊý
//@INPUT:1¡¢dvb¸ñÊ½µÄURL£¬nplayId=0
//@INPUT:2¡¢dvbc¸ñÊ½µÄURL£¬nplayId=0
//@INPUT:3¡¢avpid¸ñÊ½µÄURL£¬nplayId=0
//@EXPECTATION:°´ÕÕÖ¸¶¨´°¿Ú´óÐ¡Õý³£²¥·Å½ÚÄ¿ºÍÍ£Ö¹½ÚÄ¿²¥·Å
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SSetPlayRect£¬ÉèÖÃ²¥·Å´°¿ÚÎªÈ«ÆÁ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SPlayStream£¬¶ÔÖ¸¶¨µÄdvbc¸ñÊ½µÄÁ÷½øÐÐ²¥·Å
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSTKWaitYes£¬²âÊÔÈËÔ±È·ÈÏÊÓÆµÊÇ·ñÒÑ³É¹¦È«ÆÁ²¥·Å
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSUDIEW200SSetPlayRect£¬ÉèÖÃ²¥·Å´°¿ÚµÄÐÅÏ¢Îª0,0,500,500
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSTKWaitYes£¬²âÊÔÈËÔ±ÊÓÆµ°´ÕÕ0,0,500,500µÄ´°¿Ú¸ñÊ½Õý³£²¥³ö
//@EXECUTIONFLOW: 6¡¢µ÷ÓÃCSUDIEW200SStopPlayÍ£Ö¹ÊÓÆµ²¥·Å
//@EXECUTIONFLOW: 7¡¢µ÷ÓÃCSTKWaitYes£¬²âÊÔÈËÔ±·´À¡½ÚÄ¿Õý³£Í£Ö¹²¥·Å
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
	CSTCPrint("ÊÓÆµÈ«ÆÁ²¥·ÅÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè2Ê§°Ü");    

	CSUDIEW200SSetPlayRect(&sRect,0); 
	CSTCPrint("ÊÓÆµ´°¿ÚÒÔ0,0,500,500µÄÎ»ÖÃÏÔÊ¾Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè4Ê§°Ü");    

	CSTCPrint("ÊÓÆµ½ÚÄ¿Õý³£Í£Ö¹²¥·ÅÂðÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè6Ê§°Ü");
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SPlayStream & CSUDIEW200SSetPlayRect & CSUDIEW200SStopPlay
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SPlayStreamÔÚÊäÈë´íÎó²ÎÊýÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²»·ûºÏÉè¼ÆµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢dvbc¸ñÊ½µÄURLÍ·´íÎó£¬ÐÎÊ½Îªhttp://OriginalNetworkId.TS_Id.ServiceId
//@INPUT:2¡¢dvbc¸ñÊ½µÄURLÖÐServiceId³ö´í,ÐÎÊ½Îªdvbc://490000000:6875:64:-1
//@INPUT:3¡¢URLÎª¿Õ
//@EXPECTATION:²»ÄÜÕý³£²¥·Å½ÚÄ¿
//@EXECUTIONFLOW: 1¡¢ÊäÈë²ÎÊýÎª´íÎóµÄURLµØÖ·
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SSetPlayRect£¬ÉèÖÃ²¥·Å´°¿ÚÎªÈ«ÆÁ
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSUDIEW200SPlayStream£¬¶Ô¿ÕURL½øÐÐ²¥·Å£¬ÆÚÍû½ÚÄ¿²»ÄÜÕý³£²¥·Å
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSTKWaitYes£¬²âÊÔÈËÔ±È·ÈÏ½ÚÄ¿ÊÇ·ñ²¥·Å
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSUDIEW200SStopPlay£¬Í£Ö¹ÊÓÆµ²¥·Å
//@EXECUTIONFLOW: 6¡¢µ÷ÓÃCSUDIEW200SPlayStream£¬¶Ôhttp://403000000:6875:64:103¸ñÊ½µÄURL½øÐÐ²¥·Å£¬ÆÚÍû½ÚÄ¿²»ÄÜÕý³£²¥·Å
//@EXECUTIONFLOW: 7¡¢µ÷ÓÃCSTKWaitYes£¬²âÊÔÈËÔ±È·ÈÏ½ÚÄ¿ÊÇ·ñ²¥·Å
//@EXECUTIONFLOW: 8¡¢µ÷ÓÃCSUDIEW200SStopPlay£¬Í£Ö¹ÊÓÆµ²¥·Å
//@EXECUTIONFLOW: 9¡¢µ÷ÓÃCSUDIEW200SPlayStream£¬¶Ôdvbc://403:6875:64:103¸ñÊ½µÄURL½øÐÐ²¥·Å£¬ÆÚÍû½ÚÄ¿²»ÄÜÕý³£²¥·Å
//@EXECUTIONFLOW: 10¡¢µ÷ÓÃCSTKWaitYes£¬²âÊÔÈËÔ±È·ÈÏ½ÚÄ¿ÊÇ·ñ²¥·Å
//@EXECUTIONFLOW: 11¡¢µ÷ÓÃCSUDIEW200SStopPlay£¬Í£Ö¹ÊÓÆµ²¥·Å
BOOL CSTC_EW200SERVICE_MT_0018(void)
{ 
	char* pcTesturl_wrongHead = "http://403000000:6875:64:103";
	char* pcTesturl_wrongItem = "dvbc://403:6875:64:103";
	char* pcTestwrongurl = "";
	
	CSUDIEW200SSetPlayRect(NULL,0);
	CSUDIEW200SPlayStream(pcTesturl_wrongHead, 0);
	CSTCPrint("½ÚÄ¿²»ÄÜ²¥·ÅÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè3Ê§°Ü");    
	CSUDIEW200SStopPlay(0);
	
	CSUDIEW200SPlayStream(pcTesturl_wrongItem, 0);
	CSTCPrint("½ÚÄ¿²»ÄÜ²¥·ÅÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè6Ê§°Ü");    
	CSUDIEW200SStopPlay(0);
	
	CSUDIEW200SPlayStream(pcTestwrongurl,0);
	CSTCPrint("½ÚÄ¿²»ÄÜ²¥·ÅÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè9Ê§°Ü");    
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}


//@CASEGROUP:CSUDIEW200SPlayStream & CSUDIEW200SSetPlayRect & CSUDIEW200SStopPlay
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SSetPlayRectÔÚÊäÈë´íÎó²ÎÊýÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ÊäÈë²»·ûºÏÉè¼ÆµÄµÄ²ÎÊý×éºÏ
//@INPUT:1¡¢CSUDIStreamInfo_S psRect, psRect->m_nX=0£¬ psRect->m_nY=0£¬ psRect->m_nWidth=0£¬ psRect->m_nHeight=500
//@INPUT:2¡¢CSUDIStreamInfo_S psRect, psRect->m_nX=0£¬ psRect->m_nY=0£¬ psRect->m_nWidth=500£¬ psRect->m_nHeight=0
//@EXPECTATION:½ÚÄ¿²»ÄÜ°´ÕÕÖ¸¶¨µÄ´°¿Ú´óÐ¡²¥·Å
//@EXECUTIONFLOW: 1¡¢µ÷ÓÃCSUDIEW200SSetPlayRect£¬ÉèÖÃ²¥·Å´°¿Ú×ø±êÎª0,0,0,500
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SPlayStream,¶Ôdvbc¸ñÊ½µÄURL½øÐÐ²¥·Å£¬ÆÚÍû½ÚÄ¿²¥·Åµ«ÊÓÆµ´°¿ÚÏÔÊ¾²»Îª0,0,0,500
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSTKWaitYes£¬²âÊÔÈËÔ±È·ÈÏÊÇ·ñ½ÚÄ¿²¥·Å´°¿ÚµÄ×ø±êÎª0,0,0,500
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSUDIEW200SSetPlayRect£¬ÉèÖÃ²¥·Å´°¿ÚµÄ×ø±êÎª0,0,500,0
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSTKWaitYes£¬²âÊÔÈËÔ±È·ÈÏÊÇ·ñ½ÚÄ¿²¥·Å´°¿ÚµÄ×ø±êÎª0,0,500,0
//@EXECUTIONFLOW: 6¡¢µ÷ÓÃCSUDIEW200SStopPlay£¬Í£Ö¹½ÚÄ¿²¥·Å
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
	CSTCPrint("ÊÓÆµ½ÚÄ¿²¥·Å´°¿ÚµÄ¾ØÐÎÇø×ø±êÎª0,0,0,500Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè1Ê§°Ü");    
	
	sRect_wrong.m_nWidth = 500;
	sRect_wrong.m_nHeight = 0;
	CSUDIEW200SSetPlayRect(&sRect_wrong, 0); 
	CSTCPrint("ÊÓÆµ½ÚÄ¿²¥·Å´°¿ÚµÄ×ø±êÎª0,0,500,0Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè4Ê§°Ü");    
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SPlayStream & CSUDIEW200SSetPlayRect & CSUDIEW200SStopPlay          
//@DESCRIPTION:²âÊÔÔÚÒ»¸ö½ÚÄ¿Õý³£²¥·ÅµÄÊ±ºò¶Ô½Ó¿Ú½øÐÐ¶à´Îµ÷ÓÃµÄÇé¿ö
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT: ´ý²¥·ÅÁ÷µÄURLÎªdvbc://403000000.6875.64.67
//@EXPECTATION: ½ÚÄ¿µÄ²¥·Å×´Ì¬²»ÊÜÓ°Ïì
//@REMARK: Ö÷ÒªÊÇ²âÊÔÒ»¸ö½ÚÄ¿Õý³£²¥·ÅµÄÊ±ºò²»ÄÜÊÜÆäËû´íÎóµ÷ÓÃµÄÓ°Ïì
//@EXECUTIONFLOW:1¡¢CSUDIEW200SPlayStream²¥·ÅurlÎªdvbc://403000000.6875.64.67µÄÁ÷
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSTKWaitYes²âÊÔÈËÔ±·´À¡½ÚÄ¿Õý³£²¥·Å
//@EXECUTIONFLOW:3¡¢Ñ­»·µ÷ÓÃCSUDIEW200SPlayStreamÖÁÉÙ10´Î£¬ÆÚÍû½ÚÄ¿²»ÊÜÓ°ÏìÈÔÈ»Õý³£²¥·Å
//@EXECUTIONFLOW:4¡¢µ÷ÓÃCSTKWaitYes²âÊÔÈËÔ±·´À¡½ÚÄ¿ÈÔÈ»Õý³£²¥·Å
//@EXECUTIONFLOW:5¡¢µ÷ÓÃCSUDIEW200SSetPlayRectÉèÖÃÊÓÆµ²¥·Å´°¿ÚÎªÈ«ÆÁ
//@EXECUTIONFLOW:6¡¢µ÷ÓÃCSTKWaitYes²âÊÔÈËÔ±·´À¡½ÚÄ¿Õý³£²¥·ÅÇÒ´°¿ÚÎªÈ«ÆÁ
//@EXECUTIONFLOW:7¡¢µ÷ÓÃCSUDIEW200SSetPlayRectÖÁÉÙ10´Î£¬ÆÚÍû½ÚÄ¿ÈÔÕý³£²¥·Å²»ÊÜÓ°Ïì
//@EXECUTIONFLOW:8¡¢µ÷ÓÃCSTKWaitYes²âÊÔÈËÔ±·´À¡½ÚÄ¿ÈÔÕý³£²¥·Å
//@EXECUTIONFLOW:9¡¢µ÷ÓÃCSUDIEW200SSetPlayRectÉèÖÃÊÓÆµ²¥·Å´°¿Ú×ø±êÎª0,0,500,500
//@EXECUTIONFLOW:10¡¢µ÷ÓÃCSTKWaitYes²âÊÔÈËÔ±·´À¡½ÚÄ¿Õý³£²¥·ÅÇÒ´°¿Ú×ø±êÎª0,0,500,500
//@EXECUTIONFLOW:11¡¢µ÷ÓÃCSUDIEW200SSetPlayRectÖÁÉÙ10´Î£¬ÆÚÍû½ÚÄ¿ÈÔÕý³£²¥·Å²»ÊÜÓ°Ïì
//@EXECUTIONFLOW:12¡¢µ÷ÓÃCSTKWaitYes²âÊÔÈËÔ±·´À¡½ÚÄ¿ÈÔÕý³£²¥·Å
//@EXECUTIONFLOW:13¡¢µ÷ÓÃCSTKWaitYes²âÊÔÈËÔ±È·¶¨½ÚÄ¿ÒÑÍ£Ö¹²¥·
//@EXECUTIONFLOW:14¡¢µ÷ÓÃCSUDIEW200SStopPlay£¬Í£Ö¹½ÚÄ¿²¥·ÅÅ
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
	CSTCPrint("ÊÓÆµ½ÚÄ¿Õý³£²¥·ÅÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè2Ê§°Ü\n");    
	while(i < s_nRunCnt)
	{
		CSUDIEW200SPlayStream(s_pcTestDvbcUrl, 0);
		i++;
	}
	CSTCPrint("ÊÓÆµ½ÚÄ¿ÈÔÕý³£²¥·ÅÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè4Ê§°Ü\n");    
	
	CSUDIEW200SSetPlayRect(NULL, 0);
	CSTCPrint("ÊÓÆµ½ÚÄ¿È«ÆÁ²¥·ÅÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè6Ê§°Ü\n");  

	i = 0;
	while(i < s_nRunCnt)
	{
		CSUDIEW200SSetPlayRect(NULL, 0);
		i++;
	}
	CSTCPrint("ÊÓÆµ½ÚÄ¿È«ÆÁÕý³£²¥·ÅÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè8Ê§°Ü\n"); 

	CSUDIEW200SSetPlayRect(&sRect,0); 
	CSTCPrint("ÊÓÆµ´°¿ÚÒÔ0,0,500,500µÄÎ»ÖÃÏÔÊ¾Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè10Ê§°Ü"); 

	i = 0;
	while(i < s_nRunCnt)
	{
		CSUDIEW200SSetPlayRect(&sRect, 0);
		i++;
	}
	CSTCPrint("ÊÓÆµ´°¿ÚÈÔÒÔ0,0,500,500µÄÎ»ÖÃÏÔÊ¾Âð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè12Ê§°Ü");

	CSTCPrint("ÊÓÆµ½ÚÄ¿Õý³£Í£Ö¹²¥·ÅÂðÂð£¿\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_TRUE == CSTKWaitYes(), "²½Öè14Ê§°Ü");
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
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SAddCallbackÔÚÊäÈëÕýÈ·µÄ²ÎÊýÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:ËùÓÐ²ÎÊý¾ù·ûºÏ½Ó¿ÚÉè¼ÆµÄÒªÇó
//@INPUT:1¡¢nPlayId=0
//@INPUT:2¡¢s_nCallbackUserData = 0x1234;
//@INPUT:3¡¢fncallback
//@INPUT:4¡¢s_anTestData[4501];   
//@EXPECTATION:·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW: 1¡¢ÉèÖÃncallbackTestdataÎª²âÊÔ»Øµ÷º¯ÊýÊÇ·ñÏìÓ¦ÏûÏ¢µÄÖµ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SAddCallback×¢²á»Øµ÷º¯Êý£¬ÆÚÍû·µ»ØÖµÎªCSUDI_SUCCESS
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSUDIEW200SPlayStream´¥·¢ÏûÏ¢£¬ÆÚÍû½ÚÄ¿Õý³£²¥·Å
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSUDIOSThreadSleepµÈ´ý2sºóÅÐ¶Ï»Øµ÷º¯ÊýÖÐÏûÏ¢ÏìÓ¦·Ö±ðÎªEM_EW_MSG_VOD_CONNECT_SUCCESS£¬\
//@EXECUTIONFLOW:EM_EW_MSG_VOD_LOAD_SUCCESS£¬EM_EW_MSG_VOD_PLAYSTATE_READY
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSUDIEW200SDelCallbackÉ¾³ý×¢²áµÄ»Øµ÷£¬ÆÚÍû·µ»ØCSUDI_SUCCESS
BOOL CSTC_EW200SERVICE_MT_0020(void)
{
	memset(s_anTestData, 0, sizeof(s_anTestData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SAddCallback(0, fnCallback, (void*)s_nCallbackUserData), "²½Öè2Ê§°Ü\n");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl,0);
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL((s_anTestData[90] == 90) && (s_anTestData[93] == 93) && (s_anTestData[4096] == 4096), "²½Öè3Ê§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "²½Öè5Ê§°Ü\n"); 
	CSUDIEW200SStopPlay(0);
	
	CSTK_FATAL_POINT

	CSUDIEW200SStopPlay(0);
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SAddCallback
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SAddCallbackÔÚÊäÈëfncallback = CSUDI_NULLÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:²ÎÊý²»·ûºÏ½Ó¿ÚÉè¼ÆµÄÒªÇó
//@INPUT:1¡¢nPlayId=0
//@INPUT:2¡¢s_nCallbackUserData = 0x1234;
//@INPUT:3¡¢fncallback = CSUDI_NULL
//@INPUT:4¡¢s_anTestData[4501];   
//@EXPECTATION:»Øµ÷º¯Êý×¢²á²»³É¹¦£¬·µ»Ø·ÇCSUDI_SUCCESS£¬²âÊÔµÄÖµ²»±ä
//@EXECUTIONFLOW: 1¡¢ÉèÖÃncallbackTestdataÎª²âÊÔ»Øµ÷º¯ÊýÊÇ·ñ±»µ÷ÓÃµÄÖµ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SAddCallbackÌí¼Ó(pnUserData Îª0x1234»òÈÎÒâºÏ·¨µØÖ·Öµ)£¬ÆÚÍû·µ»ØÖµÎªCSUDI_SUCCESS
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSUDIOSThreadSleepµÈ´ý2sºóÆÚÍûncallbackTestdataµÄÖµ²»±ä
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSUDIEW200SDelCallbackÉ¾³ý×¢²áµÄ»Øµ÷£¬ÆÚÍû·µ»ØÖµÎªCSUDIEW200S_ERROR_NO_CALLBACK
BOOL CSTC_EW200SERVICE_MT_0021(void)
{
	memset(s_anTestData, 0, sizeof(s_anTestData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS != CSUDIEW200SAddCallback(0, CSUDI_NULL, (void*)s_nCallbackUserData), "²½Öè2Ê§°Ü\n");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl,0);
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL(s_anTestData[4500] == 0, "²½Öè2Ê§°Ü");
	CSUDIEW200SStopPlay(0);
	CSTK_ASSERT_TRUE_FATAL(CSUDIEW200S_ERROR_NO_CALLBACK == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "²½Öè4Ê§°Ü\n");
	
	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SAddCallback
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SAddCallbackÔÚÊäÈës_nCallbackUserData=CSUDI_NULLÊ±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÐÅºÅÏßÕý³£Á¬½Ó£¬¹©²âÊÔµÄÂëÁ÷Õý³£²¥·Å
//@INPUT:²ÎÊý²»·ûºÏ½Ó¿ÚÉè¼ÆµÄÒªÇó
//@INPUT:1¡¢nPlayId=0
//@INPUT:2¡¢s_nCallbackUserData = CSUDI_NULL
//@INPUT:3¡¢fncallback
//@INPUT:4¡¢s_anTestData[4501];   
//@EXPECTATION:»Øµ÷º¯Êý×¢²á³É¹¦£¬·µ»ØCSUDI_SUCCESS£¬²âÊÔÏàÓ¦ÏûÏ¢µÄÖµ²»±ä
//@EXECUTIONFLOW: 1¡¢ÉèÖÃncallbackTestdataÎª²âÊÔ»Øµ÷º¯ÊýÊÇ·ñ±»µ÷ÓÃµÄÖµ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SAddCallbackÌí¼Ó(pnUserData ÎªÎªCSUDI_NULL)»Øµ÷º¯Êý£¬ÆÚÍû·µ»ØÖµÎªCSUDI_SUCCESS
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSUDIEW200SPlayStream´¥·¢ÏûÏ¢£¬ÆÚÍû½ÚÄ¿Õý³£²¥·Å
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSUDIOSThreadSleepµÈ´ý2sºóÆÚÍûncallbackTestdataµÄÖµ²»±ä
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSUDIEW200SDelCallbackÉ¾³ý×¢²áµÄ»Øµ÷£¬ÆÚÍû·µ»ØÖµÎªCSUDIEW200S_ERROR_NO_CALLBACK
BOOL CSTC_EW200SERVICE_MT_0022(void)
{
	memset(s_anTestData, 0, sizeof(s_anTestData));
	
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SAddCallback(0, fnCallback, CSUDI_NULL), "²½Öè2Ê§°Ü\n");
	CSUDIEW200SPlayStream(s_pcTestDvbcUrl,0);
	CSUDIOSThreadSleep(2000);
	CSTK_ASSERT_TRUE_FATAL(s_anTestData[4500] == 0, "²½Öè3Ê§°Ü\n");
	CSUDIEW200SStopPlay(0);
	CSTK_ASSERT_TRUE_FATAL(CSUDIEW200S_ERROR_NO_CALLBACK == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "²½Öè5Ê§°Ü\n\n"); 
	
	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SAddCallback & CSUDIEW200SDelCallback
//@DESCRIPTION: ²âÊÔº¯ÊýCSUDIEW200SAddCallbackÔÚÁ¬Ðø×¢²áÁ½´ÎÉ¾³ýÁ½´ÎÏàÍ¬µÄ»Øµ÷Ê±µÄÖ´ÐÐÐ§¹û
//@PRECONDITION:ÎÞ
//@INPUT:ËùÓÐ²ÎÊý¾ù·ûºÏ½Ó¿ÚÉè¼ÆµÄÒªÇó
//@INPUT:1¡¢nPlayId=0
//@INPUT:2¡¢s_nCallbackUserData = 0x1234
//@INPUT:3¡¢fncallback
//@INPUT:4¡¢s_anTestData[4501];   
//@EXPECTATION:µÚÒ»´Î×¢²á·µ»ØCSUDI_SUCCESS£¬µÚ¶þ´Î×¢²á·µ»ØCSUDIEW200S_ERROR_ALREADY_ADDED
//@EXECUTIONFLOW: 1¡¢ÉèÖÃncallbackTestdataÎª²âÊÔ»Øµ÷º¯ÊýÊÇ·ñ±»µ÷ÓÃµÄÖµ
//@EXECUTIONFLOW: 2¡¢µ÷ÓÃCSUDIEW200SAddCallback×¢²á»Øµ÷º¯Êý£¬ÆÚÍû·µ»ØÖµÎªCSUDI_SUCCESS
//@EXECUTIONFLOW: 3¡¢µ÷ÓÃCSUDIEW200SAddCallback×¢²áÓë²½Öè2ÏàÍ¬µÄ»Øµ÷º¯Êý£¬ÆÚÍû·µ»ØÖµ²»ÎªCSUDI_SUCCESS
//@EXECUTIONFLOW: 4¡¢µ÷ÓÃCSUDIEW200SDelCallbackÉ¾³ý×¢²áµÄ»Øµ÷,ÆÚÍû·µ»ØCSUDI_SUCCESS
//@EXECUTIONFLOW: 5¡¢µ÷ÓÃCSUDIEW200SDelCallbackÉ¾³ý×¢²áµÄ»Øµ÷,ÆÚÍû·µ»ØCSUDIEW200S_ERROR_NO_CALLBACK
BOOL CSTC_EW200SERVICE_MT_0023(void)
{
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SAddCallback(0, fnCallback, (void*)s_nCallbackUserData), "²½Öè2Ê§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIEW200S_ERROR_ALREADY_ADDED == CSUDIEW200SAddCallback(0, fnCallback, (void*)s_nCallbackUserData), "²½Öè3Ê§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "²½Öè4Ê§°Ü\n");
	CSTK_ASSERT_TRUE_FATAL(CSUDIEW200S_ERROR_NO_CALLBACK == CSUDIEW200SDelCallback(0, fnCallback, (void*)s_nCallbackUserData), "²½Öè5Ê§°Ü\n");

	CSTK_FATAL_POINT
		
	return TRUE;
}

//@CASEGROUP:CSUDIEW200SAddCallback  & CSUDIEW200SDelCallback
//@DESCRIPTION:²âÊÔ²ÎÊýÈ«²¿ºÏ·¨Ê±×¢²á5¸ö»Øµ÷º¯ÊýÄÜ·ñ×¢²á»Øµ÷³É¹¦
//@PRECONDITION:Éè±¸³õÊ¼»¯³É¹¦
//@INPUT:1¡¢nPlayId = 0
//@INPUT:2¡¢fnCallback = fncallback
//@INPUT:3¡¢UserData = anUserData[i]
//@EXPECTATION:È«²¿×¢²á³É¹¦
//@EXECUTIONFLOW:1¡¢µ÷ÓÃCSUDIEW200SAddCallbackÁ¬Ðø×¢²á5¸ö»Øµ÷º¯Êý£¬×¢²áµÄº¯Êý¾ä±úÏàÍ¬µ«UserData²»Í¬£¬ÒªÇóÈ«²¿·µ»Ø³É¹¦£¬·ñÔò²âÊÔÓÃÀýÊ§°Ü
//@EXECUTIONFLOW:2¡¢µ÷ÓÃCSUDIEW200SDelCallbackÉ¾³ý×¢²á³É¹¦µÄ»Øµ÷º¯Êý£¬ÒªÇó·µ»Ø³É¹¦
BOOL CSTC_EW200SERVICE_MT_0025(void)
{
	int anUserData[5] = {0};
	int nAddCallbackCount = 5;
	int i;

	for(i=0 ; i<nAddCallbackCount ; i++)
	{			
		anUserData[i] = i*10+i;
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SAddCallback(0,fnCallback,&anUserData[i]), "²½Öè1Ê§°Ü");
	}
			
	for(i=0; i<nAddCallbackCount; i++)
	{
		CSTK_ASSERT_TRUE_FATAL(CSUDI_SUCCESS == CSUDIEW200SDelCallback(0, fnCallback, &anUserData[i]),"²½Öè2Ê§°Ü");
	}
			
	CSTK_FATAL_POINT
	
	return TRUE;
}




