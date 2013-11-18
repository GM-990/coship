/* --------------------------------------------------------------------
注意：
1.在需要与用户交互的测试用例中，可以：
	a. 使用CSTKWaitAnyKey等待用户输入任意按键
	b. 使用CSTKWaitYes等待用户输入YES
2.测试用例函数命名：测试用例ID，"测试用例ID"定义在测试用例文档中
-----------------------------------------------------------------------*/
#include "cs_aptestcase.h"
#include "udi2_ap.h"
#include "udi2_router.h"
#include "cs_testkit.h"
#include "../../cs_udi2testcase.h"
#include "udiplus_debug.h"
#include "udi2_os.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#define     AP_TEST_IP_LEN                  (32)
#define     AP_TEST_NAME_LEN                (32+1)
#define     AP_TEST_MAX_SSID            (8)

#define     SSIDNAME            ("coshipTestAp")

static char g_scWanName[AP_TEST_NAME_LEN] = {'\0'};
static char g_scApName[AP_TEST_NAME_LEN] = {'\0'};
static char g_scApAddr[AP_TEST_IP_LEN] = {'\0'};
static char g_scApMask[AP_TEST_NAME_LEN] = {'\0'};
static int  g_snMaxSSIDCount = 0;

typedef struct
{  
    char        m_pDhcpBeginAdd[AP_TEST_IP_LEN];
    char        m_pDhcpEndAdd[AP_TEST_IP_LEN]; 
}APDhcpAdd_S;

static APDhcpAdd_S  g_sApDhcpAdd;

static void AP_iAuthMode2Str(CSUDIAPAuthMode_E eAuthMode)
{
    switch(eAuthMode)
    {
        case EM_AP_AUTH_OPEN:
            CSTCPrint("认证方式为 OPEN\n");
            break;

        case EM_AP_AUTH_SHARE:
            CSTCPrint("认证方式为 SHARE\n");
            break;

        case EM_AP_AUTH_AUTO:
            CSTCPrint("认证方式为 AUTO\n");
            break;

        case EM_AP_AUTH_WPAPSK:
            CSTCPrint("认证方式为 WPAPSK\n");
            break;

        case EM_AP_AUTH_WPA2PSK:
            CSTCPrint("认证方式为 WPA2PSK\n");
            break;

        case EM_AP_AUTH_WPAPSK_WPA2PSK:
            CSTCPrint("认证方式为 WPAPSK_WPA2PSK\n");
            break; 

        default:
            CSTCPrint("无此认证方式 %d\n", eAuthMode);
    }
    
}

static void AP_iEncrypType2Str(CSUDIAPEncrypType_E eEncrypType)
{
    switch(eEncrypType)
    {
        case EM_AP_ENCRYP_WEP:
            CSTCPrint("加密模式为 WEP\n");
            break;

        case EM_AP_ENCRYP_TKIP:
            CSTCPrint("加密模式为 TKIP\n");
            break;

        case EM_AP_ENCRYP_AES:
            CSTCPrint("加密模式为 AES\n");
            break;

        case EM_AP_ENCRYP_TKIP_AES:
            CSTCPrint("加密模式为 TKIP_AES\n");
            break;

        case EM_AP_ENCRYP_NONE:
            CSTCPrint("加密模式为 NONE\n");
            break;

        default:
            CSTCPrint("无此加密模式 %d\n", eEncrypType);
    }
    
}


static void AP_iChannel2Str(CSUDIAPFreqChannel_E eChannel)
{
    switch(eChannel)
    {
        case EM_AP_CHANNEL_1_2412M:
            CSTCPrint("AP current channel 1   2.412GHZ\n");
            break;

        case EM_AP_CHANNEL_2_2417M:
            CSTCPrint("AP current channel 2   2.417GHZ\n");
            break;

        case EM_AP_CHANNEL_3_2422M:
            CSTCPrint("AP current channel 3   2.422GHZ\n");
            break;

        case EM_AP_CHANNEL_4_2427M:
            CSTCPrint("AP current channel 4   2.427GHZ\n");
            break;
            
        case EM_AP_CHANNEL_5_2432M:
            CSTCPrint("AP current channel 5   2.432GHZ\n");
            break;

        case EM_AP_CHANNEL_6_2437M:
            CSTCPrint("AP current channel 6   2.437GHZ\n");
            break;


        case EM_AP_CHANNEL_7_2442M:
            CSTCPrint("AP current channel 7   2.442GHZ\n");
            break;

        case EM_AP_CHANNEL_8_2447M:
            CSTCPrint("AP current channel 8   2.4447GHZ\n");
            break;          


        case EM_AP_CHANNEL_9_2452M:
            CSTCPrint("AP current channel 9   2.452GHZ\n");
            break;      


        case EM_AP_CHANNEL_10_2457M:
            CSTCPrint("AP current channel 10   2.457GHZ\n");
            break;      

        case EM_AP_CHANNEL_11_2462M:
            CSTCPrint("AP current channel 11   2.462GHZ\n");
            break;      

        case EM_AP_CHANNEL_12_2467M:
            CSTCPrint("AP current channel 12   2.467GHZ\n");
            break;      

        case EM_AP_CHANNEL_13_2472M:
            CSTCPrint("AP current channel 13   2.472GHZ\n");
            break;      

        case EM_AP_CHANNEL_14_2484M:
            CSTCPrint("AP current channel 14   2.484GHZ\n");
            break;   
            
        case EM_AP_CHANNEL_AUTO:
            CSTCPrint("AP current channel auto, channel=%d\n", eChannel);
            break;   

        default:
            CSTCPrint("AP current channel  unknow channel\n");
            break;   
            
    }
}


#define     TEST_AP_PRINT_SSIDINFO(name, authmod, encryp, passwd)    \
            CSTCPrint("请连接SSID:%s\n", name);   \
            AP_iAuthMode2Str(authmod);   \
            AP_iEncrypType2Str(encryp);   \
            if(passwd != CSUDI_NULL)     \
            {   \
                CSTCPrint("密码:%s\n", passwd);                    \
            }       \
            CSTCPrint("请确认是否能够连接成功y:能够连接,n:不能够连接\n"); 


#define     TEST_AP_PRINT_PING(bRet)    \
        CSTCPrint("连接成功^^,请把无线终端设置为此段内的IP，%s--%s,\n", \
        g_sApDhcpAdd.m_pDhcpBeginAdd, g_sApDhcpAdd.m_pDhcpEndAdd, g_scApAddr);  \
        CSTCPrint("设置好IP之后，请使用无线终端ping AP:%s, 并确认能否ping 通\n", g_scApAddr);   \
        if(!CSTKWaitYes())  \
        {                               \
            bRet = CSUDI_FALSE;     \
            CSTCPrint("failed 无线终端无法ping 通AP\n");    \
            break;      \
        }


static CSUDI_BOOL AP_iReadConfig(void)
{
	CSUDI_BOOL bRet = CSUDI_TRUE;
	char cBuf[32];

	do
	{
		if(CSTKGetConfigInfo("ROUTER","WAN_NIC", g_scWanName, sizeof(g_scWanName)) != CS_TK_CONFIG_SUCCESS)
		{
			bRet = CSUDI_FALSE;
			break;
		}
		if(g_scWanName == CSUDI_NULL)
		{
			bRet = CSUDI_FALSE;
			break;
		}

		if(CSTKGetConfigInfo("AP","AP_NAME", g_scApName,sizeof(g_scApName)) != CS_TK_CONFIG_SUCCESS)
		{
			bRet = CSUDI_FALSE;
			break;
		}
		if(g_scApName == CSUDI_NULL)
		{
			bRet = CSUDI_FALSE;
			break;
		}

		if(CSTKGetConfigInfo("AP","AP_IP", g_scApAddr, AP_TEST_IP_LEN) != CS_TK_CONFIG_SUCCESS)
		{
			bRet = CSUDI_FALSE;
			break;
		}
		if(g_scApAddr == CSUDI_NULL)
		{
			bRet = CSUDI_FALSE;
			break;
		}
		if(CSTKGetConfigInfo("AP","AP_MASK", g_scApMask, AP_TEST_IP_LEN) != CS_TK_CONFIG_SUCCESS)
		{
			bRet = CSUDI_FALSE;
			break;
		}
		if(g_scApMask == CSUDI_NULL)
		{
			bRet = CSUDI_FALSE;
			break;
		}

		if(CSTKGetConfigInfo("AP","DHCP_BEGIN", g_sApDhcpAdd.m_pDhcpBeginAdd, AP_TEST_IP_LEN) != CS_TK_CONFIG_SUCCESS)
		{
			bRet = CSUDI_FALSE;
			break;
		}
		if(g_sApDhcpAdd.m_pDhcpBeginAdd == CSUDI_NULL)
		{
			bRet = CSUDI_FALSE;
			break;
		}

		if(CSTKGetConfigInfo("AP","DHCP_END", g_sApDhcpAdd.m_pDhcpEndAdd, AP_TEST_IP_LEN) != CS_TK_CONFIG_SUCCESS)
		{
			bRet = CSUDI_FALSE;
			break;
		}
		if(g_sApDhcpAdd.m_pDhcpEndAdd == CSUDI_NULL)
		{
			bRet = CSUDI_FALSE;
			break;
		}         

		memset(cBuf, 0, sizeof(cBuf));
		if(CSTKGetConfigInfo("AP","MAX_SSID",cBuf,sizeof(cBuf)) != CS_TK_CONFIG_SUCCESS)
		{
			bRet = CSUDI_FALSE;
			break;
		}
	    g_snMaxSSIDCount = CSTKGetIntFromStr(cBuf,10);    
	}while(0);

	if(bRet)
	{
		CSTCPrint("AP name: %s\n", g_scApName);
		CSTCPrint("AP IP: %s\n", g_scApAddr);
		CSTCPrint("AP mask: %s\n", g_scApMask);
		CSTCPrint("AP dhcp begin IP: %s\n", g_sApDhcpAdd.m_pDhcpBeginAdd);
		CSTCPrint("AP dhcp end IP: %s\n", g_sApDhcpAdd.m_pDhcpEndAdd);
		CSTCPrint("MAX_SSID: %d\n", g_snMaxSSIDCount);
	}

	return bRet;

}

CSUDI_BOOL CSTC_AP_Init(void)
{
	CSUDI_BOOL bRet = CSUDI_FALSE;

	bRet = AP_iReadConfig();
	if(bRet)
	{
		CSUDIRTSetInterfaces(g_scWanName, g_scApName);
		CSUDIRTSetLanIp(g_scApName, g_scApAddr, g_scApMask);
	}

	return bRet;
}

CSUDI_BOOL CSTC_AP_UnInit(void)
{
    //在本测试用例集执行后调用
    g_snMaxSSIDCount = 0;

    memset(g_scApName, '\0', AP_TEST_NAME_LEN);
    memset(g_sApDhcpAdd.m_pDhcpBeginAdd, '\0', AP_TEST_IP_LEN);
    memset(g_sApDhcpAdd.m_pDhcpEndAdd, '\0', AP_TEST_IP_LEN);
    memset(g_scApAddr, '\0', AP_TEST_IP_LEN);

	return CSUDI_TRUE;
}




//@CASEGROUP:1、CSUDIAPSetSignal(char *szApName,CSAP_BOOL bEnable)
//@DESCRIPTION: 测试参数为非法值CSUDI_NULL的情况
//@PRECONDITION: 1、平台成功初始化
//@INPUT: 1、pszApName = CSUDI_NULL
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.调用CSUDIAPSetSignal，AP名称szApName传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetSignal_0001(void)
{
    	CSTK_ASSERT_TRUE_FATAL(			
         (CSUDI_SUCCESS != CSUDIAPSetSignal(CSUDI_NULL, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Failure !!!"		
         );	
        
       CSTK_FATAL_POINT
        
	return CSUDI_TRUE;
}


//@CASEGROUP:1、CSUDIAPSetSignal(const char* szApName,CSUDI_BOOL bEnable)
//@DESCRIPTION: 测试关闭无线信号之后再打开无线信号是否能够被扫描到
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够扫描无线AP的终端
//@INPUT: 1、bEnable 
//@EXPECTATION: 期望关闭AP时无法扫描到所有的SSID，打开时能够扫描到
//@EXECUTIONFLOW: 1.调用CSUDIAPSetSignal，bEnable传入CSUDI_TRUE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.调用CSUDIAPGetSSIDCapbility，获取AP支持的SSID个数，期望大于等于1
//@EXECUTIONFLOW: 3.调用CSUDIAPSetSSIDs，依次设置SSID，名称为coshipTestApI，I为0 1...pnMaxCount-1，期望设置成功
//@EXECUTIONFLOW: 4.使用无线设备进行扫描，期望能够扫描到所有SSID
//@EXECUTIONFLOW: 5.调用CSUDIAPSetSignal，bEnable传入CSUDI_FALSE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 6.使用无线设备进行扫描，期望不能扫描到任何提示出的SSID
CSUDI_BOOL CSTC_AP_SetSignal_0002(void)
{
        int     nMaxSSIDCount = 0;
        int     nSSIDIndex = 0;
        char   paSSIDName[32];

        CSUDI_BOOL      bRet = CSUDI_TRUE;
        CSUDIAPSSID_S   saSSID[AP_TEST_MAX_SSID];
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxSSIDCount)),			
            "Step 2 Get AP Capbility should be Success !!!"		
        );	
        CSTCPrint("Max SSID count %d\n", nMaxSSIDCount);
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxSSIDCount > 0),
            "Step 2 check  max ssid count Failure!!!"
        );

        memset(saSSID, 0, AP_TEST_MAX_SSID*sizeof(CSUDIAPSSID_S));
        
        for(nSSIDIndex=0; nSSIDIndex<nMaxSSIDCount; ++nSSIDIndex)
        {
            memset(paSSIDName, '\0', 32);
            sprintf(paSSIDName, "%s%d",  SSIDNAME, nSSIDIndex);
            saSSID[nSSIDIndex].nIndex = nSSIDIndex;
            saSSID[nSSIDIndex].bIsEnable = CSUDI_TRUE;
            memcpy(saSSID[nSSIDIndex].szSSID, paSSIDName, sizeof(paSSIDName));
            
            saSSID[nSSIDIndex].eWifiMode = EM_AP_WIFI_MODE_11BGN;
            saSSID[nSSIDIndex].eAuthMode = EM_AP_AUTH_OPEN;
            saSSID[nSSIDIndex].eEncrypType = EM_AP_ENCRYP_NONE;
            memset(saSSID[nSSIDIndex].szPassWord, 0, CSUDIAP_PASSWORD_LEN);
            saSSID[nSSIDIndex].uUpdateTime = 0;
        }

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, saSSID, nMaxSSIDCount)),			
            "Step 3 Set SSID Failure !!!"		
        );

        CSTCPrint("请使用带有无线功能的设备进行扫描，并确认能扫描到以下SSID\n");
        for(nSSIDIndex=0; nSSIDIndex<nMaxSSIDCount; ++nSSIDIndex)
        {
            CSTCPrint("%s ", saSSID[nSSIDIndex].szSSID);
        }
        CSTCPrint("\n");

        do
        {
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("Step 4 扫描不到所有SSID，打开AP signal 测试失败\n");
                break;
            }

            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_FALSE)),			
                "Step 5 Set Ap Signal should be Success !!!"		
            );	

            CSTCPrint("请使用带有无线功能的设备进行扫描，并确认不能扫描到以下SSID\n");
            for(nSSIDIndex=0; nSSIDIndex<nMaxSSIDCount; ++nSSIDIndex)
            {
                CSTCPrint("%s ", saSSID[nSSIDIndex].szSSID);
            }
            CSTCPrint("\n");

            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("Step 6 关闭无线信号还能扫描到无线SSID，关闭无线信号失败\n");
                break;
            }
            
        }while(0);
         

       CSTK_FATAL_POINT
        
        return bRet;
}



//@CASEGROUP:1、CSUDIAPSetSignal(char *szApName,CSAP_BOOL bEnable)
//@DESCRIPTION: 测试连续关闭和打开无线信号1000次之后能够扫描到无线SSID并连接成功
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够扫描无线AP的终端
//@INPUT: 1、bEnable 
//@EXPECTATION: 期望打开和关闭无线AP时都能够成功，最终应能扫描到无线AP，并能连接成功
//@EXECUTIONFLOW: 1.调用CSUDIAPSetSignal，bEnable传入CSUDI_TRUE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望大于0
//@EXECUTIONFLOW: 3.调用CSUDIAPSetSSIDs，设置第一个SSID为coshipTestAp0，并用如下参数配置，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0
//@EXECUTIONFLOW: 4.反复调用 CSUDIAPSetSignal，关闭和使能AP 1000次，期望每一次都成功
//@EXECUTIONFLOW: 5.使用带有无线终端设备进行扫描，期望能够扫描到coshipTestAp0
//@EXECUTIONFLOW: 6.提示进行无线连接，并期望能够连接成功
//@EXECUTIONFLOW: 7.提示进行IP配置，并ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSignal_0003(void)
{
        int     nMaxSSIDCount = 0;
        int     i = 0;
        char   paSSIDName[AP_TEST_NAME_LEN]={0};


        CSUDI_BOOL      bRet = CSUDI_TRUE;
        CSUDIAPSSID_S   sSSID={0};
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxSSIDCount)),			
            "Step 2 Get AP Capbility should be Success !!!"		
        );	
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxSSIDCount > 0),
            "Step 2 check  max ssid count Failure!!!"
        );


        memset(paSSIDName, '\0', AP_TEST_NAME_LEN);
        sprintf(paSSIDName, "%s%d",  SSIDNAME, 0);
        sSSID.nIndex = 0;
        sSSID.bIsEnable = CSUDI_TRUE;
        memcpy(sSSID.szSSID, paSSIDName, sizeof(paSSIDName));
        
        sSSID.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSSID.eAuthMode = EM_AP_AUTH_OPEN;
        sSSID.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSSID.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSSID.szPassWord, "%s", "1234567890123");
        sSSID.uUpdateTime = 0;
    
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSSID, 1)),			
            "Step 3 Set SSID Failure !!!"		
        );


        for(i=0; i<1000; ++i)
        {
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_FALSE)),			
                "Step 4 Set Ap  Signal disable should be Success !!!"		
            );	
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
                "Step 4 Set Ap Signal enable should be Success !!!"		
            );	
        }

        do
        {
            CSTCPrint("请使用带有无线功能的设备进行扫描，并确认能够扫描到coshipTestAp0\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 打开无线信号失败，无法扫描到SSID\n");
                break;
            }

            TEST_AP_PRINT_SSIDINFO(paSSIDName, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSSID.szPassWord);
            
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 无法连接到%s\n", paSSIDName);
                break;
            }
            
            TEST_AP_PRINT_PING(bRet)  
        }while(0);
         

       CSTK_FATAL_POINT
        
        return bRet;

}


//@CASEGROUP:1、CSAPSetFreqChannel(char *szApName,CSAPFreqChannel_E enChannel)
//@DESCRIPTION: 测试参数为非法参数的情况
//@PRECONDITION: 1、平台成功初始化
//@INPUT: 1、szApName = CSUDI_NULL
//@INPUT: 2、enChannel = EM_AP_Channel_Auto+1
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.调用CSAPSetFreqChannel，szApName传入为空，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.调用CSAPSetFreqChannel，enChannel传入EM_AP_Channel_Auto+1，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetFreqChannel_0001(void)
{
    	CSTK_ASSERT_TRUE_FATAL(			
         (CSUDI_SUCCESS != CSUDIAPSetFreqChannel(CSUDI_NULL, EM_AP_CHANNEL_1_2412M)),			
            "Step 1 Set Ap channel should be Failure !!!"		
         );	

    	CSTK_ASSERT_TRUE_FATAL(			
         (CSUDI_SUCCESS != CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_AUTO+1)),			
            "Step 2 Set Ap channel should be Failure !!!"		
         );	
        
        
       CSTK_FATAL_POINT
        
	return CSUDI_TRUE;

}


//@CASEGROUP:1、CSAPSetFreqChannel(char *pszApName,CSAPFreqChannel_E enChannel)
//@DESCRIPTION: 测试设置AP信道的功能
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、能够扫描无线AP的终端
//@INPUT: 1、pszApName
//@INPUT: 2、enChannel 为CSAPFreqChannel_E中枚举值
//@EXPECTATION: 期望返回正确的信道,并能够连接成功
//@EXECUTIONFLOW: 1.调用CSUDIAPSetSignal，bEnable传入CSUDI_TRUE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望大于0
//@EXECUTIONFLOW: 3.调用CSUDIAPSetSSIDs，设置第一个SSID为coshipTestAp0，并使用如下参数进行配置，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0
//@EXECUTIONFLOW: 4.调用CSAPSetFreqChannel，传入EM_AP_CHANNEL_1_2412M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5.使用无线终端设备进行扫描，期望能够扫描到coshipTestAp0
//@EXECUTIONFLOW: 6.使用无线终端设备进行连接 coshipTestAp0, 期望连接成功
//@EXECUTIONFLOW: 7.提示设置无线终端设备IP，期望能够ping通 AP
//@EXECUTIONFLOW: 8.重复步骤3-4，依次传入CSAPFreqChannel_E中的枚举值，直到所有信道都测试通过
CSUDI_BOOL CSTC_AP_SetFreqChannel_0002(void)
{
        int     nMaxSSIDCount = 0;
        int     nChannelIndex = 0;
        char   paSSIDName[AP_TEST_NAME_LEN]={0};

        CSUDI_BOOL      bRet = CSUDI_TRUE;
        CSUDIAPSSID_S   sSSID={0};

        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxSSIDCount)),			
            "Step 2 Get AP Capbility should be Success !!!"		
        );	
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxSSIDCount > 0),
            "Step 2 check  max ssid count Failure!!!"
        );


        memset(paSSIDName, '\0', AP_TEST_NAME_LEN);
        sprintf(paSSIDName, "%s%d",  SSIDNAME, 0);
        sSSID.nIndex = 0;
        sSSID.bIsEnable = CSUDI_TRUE;
        memcpy(sSSID.szSSID, paSSIDName, sizeof(paSSIDName));
        
        sSSID.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSSID.eAuthMode = EM_AP_AUTH_OPEN;
        sSSID.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSSID.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSSID.szPassWord, "%s", "1234567890123");
        sSSID.uUpdateTime = 0;
    
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSSID, 1)),			
            "Step 3 Set SSID Failure !!!"		
        );

        for(nChannelIndex = EM_AP_CHANNEL_1_2412M; nChannelIndex<=EM_AP_CHANNEL_AUTO; ++nChannelIndex)
        {
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, nChannelIndex)),			
                "Step 4 set ap channel Failure !!!"		
            );

            AP_iChannel2Str(nChannelIndex);
            CSTCPrint("请使用带有无线功能的设备进行扫描，并确认能够扫描到%s\n", paSSIDName);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 打开无线信号失败，无法扫描到%s, channel: %d\n", paSSIDName, nChannelIndex);
                break;
            }

            TEST_AP_PRINT_SSIDINFO(paSSIDName, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSSID.szPassWord);
            
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 无法连接到%s,信道:%d\n", paSSIDName, nChannelIndex);
                break;
            }
            
            TEST_AP_PRINT_PING(bRet)
        }


      CSTK_FATAL_POINT

	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDSeparate(const char* szApName, CSUDI_BOOL bEnable)
//@DESCRIPTION: 测试非法参数的情况
//@PRECONDITION: 1、平台成功初始化
//@INPUT: 1、szApName = CSUDI_NULL
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.调用CSUDIAPSetSSIDSeparate，szApName传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetSSIDSeparate_0001(void)
{
    	CSTK_ASSERT_TRUE_FATAL(			
         (CSUDI_SUCCESS != CSUDIAPSetSSIDSeparate(CSUDI_NULL, CSUDI_TRUE)),			
            "Step 1 Set Ap separate should be Failure !!!"		
         );	
        
       CSTK_FATAL_POINT
        
	return CSUDI_TRUE;

}


//@CASEGROUP:1、CSUDIAPSetSSIDSeparate(const char* szApName, CSUDI_BOOL bEnable)
//@DESCRIPTION: 测试关闭AP隔离，通过不同SSID接入的station能够互通，打开AP隔离，通过不同SSID接入的station不能够互通，相同SSID接入的station能够互通
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、2台以上一无线终端设备
//@PRECONDITION: 3、设置IP时必须在同一网段
//@INPUT: 1、szApName 
//@EXPECTATION: 期望关闭AP隔离，通过不同SSID接入的station能够互通，打开AP隔离，通过不同SSID接入的station不能够互通，相同SSID接入的station能够互通
//@EXECUTIONFLOW: 1.调用CSUDIAPSetSignal，bEnable传入CSUDI_TRUE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.调用CSUDIAPSetSSIDSeparate，bEnable传入CSUDI_FALSE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3.调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望最大个数大于等于2
//@EXECUTIONFLOW: 4.调用CSUDIAPSetFreqChannel，设置AP的信道，期望设置成功
//@EXECUTIONFLOW: 5.调用CSUDIAPSetSSIDs，设置第一个SSID名称为coshipTestAp0,并进行如下的配置,期望返回成功
//@EXECUTIONFLOW: 配置coshipTestAp0，参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 6.调用CSUDIAPSetSSIDs，设置第二个SSID名称为coshipTestAp1,并进行如下的配置,期望返回成功
//@EXECUTIONFLOW: 配置coshipTestAp1，参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="aaaabbbbcc", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 7.提示分别用两台无线终端设备进行coshipTestAp0,coshipTestAp1的连接，期望连接成功
//@EXECUTIONFLOW: 8.提示设置此两台无线终端设备的IP地址，并进行ping测试，期望能够ping通
//@EXECUTIONFLOW: 9.调用CSUDIAPSetSSIDSeparate，bEnable传入CSUDI_TRUE，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 10.提示使用此两台无线终端设备进行ping测试，期望不能够ping通
//@EXECUTIONFLOW: 11.提示使一台设备断开coshipTestAp1，连接coshipTestAp0,期望连接成功
//@EXECUTIONFLOW: 12.提示设置此设备的IP，并向设备进行ping测试，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDSeparate_0002(void)
{
        int     nMaxSSIDCount = 0;
        int     nSsidIndex = 0;
        char   paSSIDName[AP_TEST_NAME_LEN]={0};

        CSUDI_BOOL      bRet = CSUDI_TRUE;
        CSUDIAPSSID_S   saSSID[4];

        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	
        

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDSeparate(g_scApName, CSUDI_FALSE)),			
            "Step 2 Set Ap Signal should be Success !!!"		
        );


        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 3 Set Ap channel should be Success !!!"		
        );	
        
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxSSIDCount)),			
            "Step 4 Get AP Capbility should be Success !!!"		
        );	
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxSSIDCount > 0),
            "Step 5 check  max ssid count Failure!!!"
        );

        for(nSsidIndex = 0; nSsidIndex<2; ++nSsidIndex)
        {
            memset(paSSIDName, '\0', AP_TEST_NAME_LEN);
            sprintf(paSSIDName, "%s%d",  SSIDNAME, nSsidIndex);
            memcpy(saSSID[nSsidIndex].szSSID, paSSIDName, sizeof(paSSIDName));
            saSSID[nSsidIndex].nIndex = nSsidIndex;
            saSSID[nSsidIndex].bIsEnable = CSUDI_TRUE;

            saSSID[nSsidIndex].eWifiMode = EM_AP_WIFI_MODE_11BGN;
            saSSID[nSsidIndex].eAuthMode = EM_AP_AUTH_OPEN;
            saSSID[nSsidIndex].eEncrypType = EM_AP_ENCRYP_WEP;
            memset(saSSID[nSsidIndex].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
            sprintf(saSSID[nSsidIndex].szPassWord, "%s", "1234567890123");
            saSSID[nSsidIndex].uUpdateTime = 0;
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &saSSID[nSsidIndex], 1)),			
                "Step 6 Set SSID Failure !!!"		
            );
            
        }


    do
    {
            CSTCPrint("请使用带有无线功能的设备进行扫描，并确认能够扫描到%s, %s\n", saSSID[0].szSSID, saSSID[1].szSSID);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 7 打开无线信号失败，无法扫描到%s 或%s\n", saSSID[0].szSSID, saSSID[1].szSSID);
                break;
            }

            CSTCPrint("请用一台无线终端设备连接%s", saSSID[0].szSSID);
            TEST_AP_PRINT_SSIDINFO(saSSID[0].szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, saSSID[0].szPassWord);
            CSTCPrint("请用另一台无线终端设备连接%s", saSSID[1].szSSID);
            TEST_AP_PRINT_SSIDINFO(saSSID[1].szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, saSSID[1].szPassWord);
                    
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 8 无法连接到%s 或 %s\n", saSSID[0].szSSID, saSSID[1].szSSID);
                break;
            }
            
            CSTCPrint("连接成功^^,请把两台无线终端设置为此段内的IP，%s--%s,\n", 
                g_sApDhcpAdd.m_pDhcpBeginAdd, g_sApDhcpAdd.m_pDhcpEndAdd, g_scApAddr);
            
            CSTCPrint("设置好IP之后，请使用一台ping另一台无线终端设备, 并确认能否ping 通\n");

            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 9  关闭AP隔离，不同SSID之间无法ping 通AP\n");
                break;
            }     


            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSSIDSeparate(g_scApName, CSUDI_TRUE)),			
                "Step 10 Set Ap Signal should be Success !!!"		
            );          

            CSTCPrint("打开AP隔离，请使用一台ping另一台无线终端设备, 并确认不能够ping 通\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 11   Failed 打开AP隔离，不同SSID之间能够ping 通AP\n");
                break;
            }     

            CSTCPrint("请断开与%s的连接，并连接到%s,并期望能够连接成功\n", saSSID[1].szSSID, saSSID[0].szSSID);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 12   Failed 第二台无线终端设备无法连接到%s\n", saSSID[0].szSSID);
                break;
            }                 

            CSTCPrint("请使用一台ping另一台无线终端设备, 并确认能够ping 通\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 13   Failed 打开AP隔离，相同SSID之间无法ping通\n");
                break;
            }               

    }while(0);

      CSTK_FATAL_POINT

	return bRet;
}


//@CASEGROUP:1、CSUDIAPGetConfig(const char* szApName,CSUDIAPConfig_S* pstOutConfig)
//@DESCRIPTION: 测试非法参数的情况
//@PRECONDITION: 1、平台成功初始化
//@INPUT: 1、szApName
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.调用CSUDIAPGetConfig，szApName传入CSUDI_NULL,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.调用CSUDIAPGetConfig，pstOutConfig传入CSUDI_NULL,期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_GetConfig_0001(void)
{
        CSUDIAPConfig_S stOutConfig;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetConfig(CSUDI_NULL, &stOutConfig)),			
            "Step 1 Get Ap config should be Failure !!!"		
        );	
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetConfig(g_scApName, CSUDI_NULL)),			
            "Step 2 Get Ap config should be Failure !!!"		
        );	

        
       CSTK_FATAL_POINT
        
	return CSUDI_TRUE;

}


//@CASEGROUP:1、CSUDIAPGetConfig(const char* szApName,CSUDIAPConfig_S* pstOutConfig)
//@DESCRIPTION: 测试能够正确获取AP配置信息
//@PRECONDITION: 1、平台成功初始化
//@INPUT: 1、szApName 
//@EXPECTATION: 期望返设置好的AP配置
//@EXECUTIONFLOW: 1.调用CSUDIAPSetSignal，打开无线信号,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.调用CSUDIAPSetFreqChannel，设置无线信道为EM_AP_CHANNEL_1_2412M,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3.调用CSUDIAPSetSSIDSeparate，关闭AP隔离，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4.调用CSUDIAPGetConfig，获取AP配置，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5.比较AP配置信息，期望与预期设置一致
//@EXECUTIONFLOW: 6.调用CSUDIAPSetFreqChannel，设置无线信道为EM_AP_CHANNEL_6_2437M,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7.调用CSUDIAPSetSSIDSeparate， 打开AP隔离，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 8.调用CSUDIAPGetConfig，获取AP配置，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9.比较AP配置信息，期望与预期设置一致
CSUDI_BOOL CSTC_AP_GetConfig_0002(void)
{
        CSUDIAPConfig_S stOutConfig;
        CSUDI_BOOL      bRet = CSUDI_TRUE;

        CSUDI_BOOL      bSignal = CSUDI_TRUE;
        CSUDI_BOOL      bSeparate = CSUDI_FALSE;
        CSUDIAPFreqChannel_E    eChannel = EM_AP_CHANNEL_1_2412M;

        do
        {
                CSTK_ASSERT_TRUE_FATAL(			
                    (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, bSignal)),			
                    "Step 1 Set Ap Signal should be Success !!!"		
                );	
                
                CSTK_ASSERT_TRUE_FATAL(			
                    (CSUDI_SUCCESS == CSUDIAPSetSSIDSeparate(g_scApName, bSeparate)),			
                    "Step 2 Set Ap Signal should be Success !!!"		
                );

                CSTK_ASSERT_TRUE_FATAL(			
                    (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, eChannel)),			
                    "Step 3 set ap channel Failure !!!"		
                );

                CSTK_ASSERT_TRUE_FATAL(			
                    (CSUDI_SUCCESS == CSUDIAPGetConfig(g_scApName,&stOutConfig)),			
                    "Step 4 get ap config Failure !!!"		
                );

                
                CSTCPrint("signal: %d, separa: %d, echannel: %d\n", stOutConfig.bSignalEnable, stOutConfig.bSSIDSeparateEnable, stOutConfig.eChannel);

                if(!(stOutConfig.bSignalEnable == bSignal &&
                   stOutConfig.bSSIDSeparateEnable == bSeparate &&
                   stOutConfig.eChannel == eChannel))
                {
                    CSTCPrint("step 5 failed check ap config fail\n");
                    bRet = CSUDI_FALSE;
                    break;
                }


                bSeparate = CSUDI_TRUE;
                CSTK_ASSERT_TRUE_FATAL(			
                    (CSUDI_SUCCESS == CSUDIAPSetSSIDSeparate(g_scApName, bSeparate)),			
                    "Step 6 Set Ap Signal should be Success !!!"		
                );

                eChannel = EM_AP_CHANNEL_6_2437M;
                CSTK_ASSERT_TRUE_FATAL(			
                    (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, eChannel)),			
                    "Step 7 set ap channel Failure !!!"		
                );
                
                CSTK_ASSERT_TRUE_FATAL(			
                    (CSUDI_SUCCESS == CSUDIAPGetConfig(g_scApName,&stOutConfig)),			
                    "Step 8 get ap config Failure !!!"		
                );

                if(!(stOutConfig.bSignalEnable == bSignal &&
                   stOutConfig.bSSIDSeparateEnable == bSeparate &&
                   stOutConfig.eChannel == eChannel))
                {
                    CSTCPrint("step 9 failed check ap config fail\n");
                    bRet = CSUDI_FALSE;
                    break;
                }                
        }while(0);
        
       CSTK_FATAL_POINT
        
        return bRet;
}


//@CASEGROUP:1、CSAPGetSSIDCapbility(char *szApName,int *pnMaxCount)
//@DESCRIPTION: 测试参数为非法值的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、pszApName=CSUDI_NULL
//@INPUT:  2、pnMaxCount=CSUDI_NULL
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSAPGetSSIDCapbility，szApName传入CSUDI_NULL；pnMaxCount传入合法值，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSAPGetSSIDCapbility，szApName，pnMaxCount传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSAPGetSSIDCapbility，szApName, pnMaxCount, 传入非法值期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_GetSSIDCapblitity_0001(void)
{
        int     nMaxCount = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetSSIDCapbility(CSUDI_NULL, &nMaxCount)),			
            "Step 1 Get SSID capbility should be Failure !!!"		
        );	
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetSSIDCapbility(g_scApName, CSUDI_NULL)),			
            "Step 2 Get SSID capbility should be Failure !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetSSIDCapbility(CSUDI_NULL, CSUDI_NULL)),			
            "Step 3 Get SSID capbility should be Failure !!!"		
        );	

        CSTK_FATAL_POINT
        
	return CSUDI_TRUE;

}


//@CASEGROUP:1、CSAPGetSSIDCapbility(char *szApName,int *pnMaxCount)
//@DESCRIPTION: 测试AP支持的最大SSID个数
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、pszApName
//@EXPECTATION: 期望获取到的SSID个数大于等于1
//@EXECUTIONFLOW: 1、调用CSAPGetSSIDCapbility，szApName传入ra0,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、比较返回的最大SSID个数,期望大于等于1
CSUDI_BOOL CSTC_AP_GetSSIDCapblitity_0002(void)
{
        int     nMaxCount = 0;

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 1 Get AP Capbility should be Success !!!"		
        );	
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 2 check  max ssid count Failure!!!"
        );
        CSTK_FATAL_POINT
        
	return CSUDI_TRUE;

}


//@CASEGROUP:1、CSUDIAPGetSSIDs(const char* szApName,CSUDIAPSSID_S* psOutSsids,int* pnCount)
//@DESCRIPTION: 测试非法参数的情况
//@PRECONDITION: 1、平台成功初始化
//@INPUT: 1、szApName = CSUDI_NULL
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.调用CSUDIAPGetSSIDs，szApName传入CSUDI_NULL,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.调用CSUDIAPGetSSIDs，psOutSsids传入CSUDI_NULL,期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3.调用CSUDIAPGetSSIDs，pnCount传入CSUDI_NULL,期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_GetSSIDs_0001(void)
{
        CSUDIAPSSID_S   sOutSsids[4];
        int     nCount =4 ;
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetSSIDs(CSUDI_NULL, sOutSsids, &nCount)),			
            "Step 1 Get SSID capbility should be Failure !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetSSIDs(g_scApName,CSUDI_NULL, &nCount)),			
            "Step 2 Get SSID capbility should be Failure !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetSSIDs(g_scApName, sOutSsids, CSUDI_NULL)),			
            "Step 3 Get SSID capbility should be Failure !!!"		
        );	
        CSTK_FATAL_POINT
            
	return CSUDI_TRUE;
}


//@CASEGROUP:1、CSUDIAPGetSSIDs(const char* szApName,CSUDIAPSSID_S* psOutSsids,int* pnCount)
//@DESCRIPTION: 测试获取用户设置的所有SSID
//@PRECONDITION: 1、平台成功初始化
//@INPUT: 1、szApName = ra0
//@EXPECTATION: 期望获取到设置的所有SSID
//@EXECUTIONFLOW: 1.调用CSUDIAPSetSignal，打开无线信号，期望返回成功
//@EXECUTIONFLOW: 2.调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回成功
//@EXECUTIONFLOW: 3.调用CSUDIAPSetSSIDs，设置所有SSID，名称依次为coshipTestApi，i为0 1 2......pnMaxCount-1，并使能，期望返回成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 4.调用CSUDIAPGetSSIDs，依次验证所有的SSID名称和使能状态以及配置参数，期望和设置匹配
CSUDI_BOOL CSTC_AP_GetSSIDs_0002(void)
{
        int nMaxCount = 0;
        int i = 0;
        CSUDI_BOOL bRet = CSUDI_TRUE;
        CSUDIAPSSID_S   sSsid[AP_TEST_MAX_SSID];
        CSUDIAPSSID_S   sOutSsids[AP_TEST_MAX_SSID];
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 2 Get SSID capbility should be Success !!!"		
        );

        for(i=0; i<nMaxCount; ++i)
        {
            sSsid[i].nIndex = i;
            sSsid[i].bIsEnable = CSUDI_TRUE;
            sprintf(sSsid[i].szSSID, "%s%d", SSIDNAME, i);

            sSsid[i].eWifiMode = EM_AP_WIFI_MODE_11BGN;
            sSsid[i].eAuthMode = EM_AP_AUTH_OPEN;
            sSsid[i].eEncrypType = EM_AP_ENCRYP_WEP;
            memset(sSsid[i].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
            sprintf(sSsid[i].szPassWord, "%s", "1234567890123");
            sSsid[i].uUpdateTime = 0;
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSsid[i], 1)),			
                "Step 3 Get SSID capbility should be Success !!!"		
            );
        }

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDs(g_scApName, sOutSsids, &nMaxCount)),			
            "Step 4 Get SSID  should be Success !!!"		
        );

        for(i=0; i<nMaxCount; ++i)
        {
            if((sOutSsids[i].nIndex == sSsid[i].nIndex) &&
                (sOutSsids[i].bIsEnable== sSsid[i].bIsEnable) &&
                !memcmp(sOutSsids[i].szSSID, sSsid[i].szSSID, sizeof(sSsid[i].szSSID)) &&
                (sOutSsids[i].eWifiMode == EM_AP_WIFI_MODE_11BGN) &&
                (sOutSsids[i].eAuthMode == EM_AP_AUTH_OPEN)&&
                (sOutSsids[i].eEncrypType == EM_AP_ENCRYP_WEP) &&
                !memcmp(sOutSsids[i].szPassWord, "1234567890123", sizeof("1234567890123")))
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("获取的信息与设置的不符\n");
                break;
            }
        }
        
        CSTK_FATAL_POINT
            
	return bRet;

}


//@CASEGROUP:1、CSUDIAPGetSSIDs(const char* szApName,CSUDIAPSSID_S* psOutSsids,int* pnCount)
//@DESCRIPTION: 测试获取SSID时，传入的Count比实际设置的数量小的情况
//@PRECONDITION: 1、平台成功初始化
//@INPUT: 1、szApName 
//@EXPECTATION: 期望不死机，能获取到nCount个的SSID信息
//@EXECUTIONFLOW: 1.调用CSUDIAPSetSignal，打开无线信号，期望返回成功
//@EXECUTIONFLOW: 2.调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回成功
//@EXECUTIONFLOW: 3.调用CSUDIAPSetSSIDs，设置所有SSID，名称依次为coshipTestApi，i为0 1 2......pnMaxCount-1，并使能，期望返回成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 4.调用CSUDIAPGetSSIDs，传入的nCount个数为pnMaxCount-1，期望能获取到nCount个SSID的信息，并验证通过
CSUDI_BOOL CSTC_AP_GetSSIDs_0003(void)
{
        int nMaxCount = 0;
        int nCount = 0;
        int i = 0;
        
        CSUDI_BOOL bRet = CSUDI_TRUE;
        CSUDIAPSSID_S   sSsid[AP_TEST_MAX_SSID];
        CSUDIAPSSID_S   sOutSsids[AP_TEST_MAX_SSID];
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 2 Get SSID capbility should be Success !!!"		
        );

        for(i=0; i<nMaxCount; ++i)
        {
            sSsid[i].nIndex = i;
            sSsid[i].bIsEnable = CSUDI_TRUE;
            sprintf(sSsid[i].szSSID, "%s%d", SSIDNAME, i);

            sSsid[i].eWifiMode = EM_AP_WIFI_MODE_11BGN;
            sSsid[i].eAuthMode = EM_AP_AUTH_OPEN;
            sSsid[i].eEncrypType = EM_AP_ENCRYP_WEP;
            memset(sSsid[i].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
            sprintf(sSsid[i].szPassWord, "%s", "1234567890123");
            sSsid[i].uUpdateTime = 0;
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSsid[i], 1)),			
                "Step 3 Get SSID capbility should be Success !!!"		
            );
        }

        nCount = nMaxCount -1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDs(g_scApName, sOutSsids, &nCount)),			
            "Step 4 Get SSID should be Success !!!"		
        );

        for(i=0; i<nCount; ++i)
        {
            if((sOutSsids[i].nIndex == sSsid[i].nIndex) &&
                (sOutSsids[i].bIsEnable== sSsid[i].bIsEnable) &&
                !memcmp(sOutSsids[i].szSSID, sSsid[i].szSSID, sizeof(sSsid[i].szSSID)) &&
                (sOutSsids[i].eWifiMode == EM_AP_WIFI_MODE_11BGN) &&
                (sOutSsids[i].eAuthMode == EM_AP_AUTH_OPEN)&&
                (sOutSsids[i].eEncrypType == EM_AP_ENCRYP_WEP) &&
                !memcmp(sOutSsids[i].szPassWord, "1234567890123", sizeof("1234567890123")))
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("获取的信息与设置的不符\n");
                break;
            }
        }
        
        CSTK_FATAL_POINT
            
	return bRet;

}


//@CASEGROUP:1、CSUDIAPSetSSIDs(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount)
//@DESCRIPTION: 测试参数为非法值的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPSetSSIDs,szApName传入为CSUDI_NULL，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,psSsids传入为CSUDI_NULL，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIAPSetSSIDs,第一个SSID不使能，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIAPSetSSIDs,SSID的值为CSUDI_NULL，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIAPSetSSIDs, eWifiMode传入EM_AP_WIFI_MODE_11GN+1，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSUDIAPSetSSIDs, eAuthMode传入EM_AP_AUTH_WPAPSK_WPA2PSK+1，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIAPSetSSIDs, eEncrypType传入EM_AP_ENCRYP_NONE+1，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、调用CSUDIAPSetSSIDs, eAuthMode为EM_AP_AUTH_WPA2PSK, eEncrypType为EM_AP_ENCRYP_AES, szPassWord传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetSSIDs_0001(void)
{
        CSUDIAPSSID_S   sSetSsids;
        int     nMaxCount =0 ;
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 2 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 2 check  max ssid count Failure!!!"
        );
        
        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, sSetSsids.nIndex);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11GN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(CSUDI_NULL, &sSetSsids, 1)),			
            "Step 3 set SSID  should be Failure !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, CSUDI_NULL, 1)),			
            "Step 4 set SSID  should be Failure !!!"		
        );	     

        sSetSsids.bIsEnable = CSUDI_FALSE;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 5 set SSID  should be Failure !!!"		
        );	          

        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 6 set SSID  should be Failure !!!"		
        );

        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, sSetSsids.nIndex);
        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11GN+1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 7 set SSID  should be Failure !!!"		
        );

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11GN;
        sSetSsids.eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK+1;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 8 set SSID  should be Failure !!!"		
        );


        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_NONE+1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 9 set SSID  should be Failure !!!"		
        );


        sSetSsids.eAuthMode = EM_AP_AUTH_WPA2PSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_AES;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 10 set SSID  should be Failure !!!"		
        );        
        
        CSTK_FATAL_POINT
        return CSUDI_TRUE;
        
}


//@CASEGROUP:1、CSUDIAPSetSSIDs(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount)
//@DESCRIPTION: 测试SSID为特殊字符的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、pszApName
//@EXPECTATION: 期望设置SSID成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望大于0
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs, 设置SSID名称为特殊字符"!@#$%^&*&123"的情况，期望返回CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetSSIDs_0002(void)
{
        CSUDIAPSSID_S   sSetSsids;
        int     nMaxCount =0 ;
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

    	CSTK_ASSERT_TRUE_FATAL(			
         (CSUDI_SUCCESS != CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_AUTO+1)),			
            "Step 2 Set Ap channel should be Failure !!!"		
         );	
        

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );
        
        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", "!@#$%^&*&123", sSetSsids.nIndex);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;

        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 set SSID  should be success !!!"		
        );	   

        CSTCPrint("请扫描并确认能够扫描到: %s，输入y能够扫描到，输入n不能够扫描到\n", sSetSsids.szSSID);
        CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()), "Step 4 扫描不到特殊字符的SSID!!!"		
        );	   

        
        CSTK_FATAL_POINT
        return CSUDI_TRUE;
        
}

//@CASEGROUP:1、CSUDIAPSetSSIDs(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount)
//@DESCRIPTION: 测试是否能正确的设置SSID
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、无线终端设备
//@INPUT:  1、pszApName
//@INPUT:  2、SSID名称为coship_testap%d，如coship_testap0,coship_testap1.....
//@EXPECTATION: 期望设置SSID成功,并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,依次以coshipTestAp0,coshipTestAp1.....设置SSID名称,并且使能配置所有的SSID
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示用无线终端进行扫描，期望能够扫描到所有SSID
//@EXECUTIONFLOW: 6、调用CSUDIAPSetSSIDs，关闭除coshipTestAp0以外的其它SSID，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、提示进行扫描，期望除coshipTestAp0以外，其它SSID都无法扫描到
//@EXECUTIONFLOW: 8、调用CSUDIAPSetSSIDs，使能所有SSID，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、提示进行扫描，期望能够扫描到所有SSID
//@EXECUTIONFLOW: 10、提示用户进行连接coshipTestAp1，期望连接成功
//@EXECUTIONFLOW: 11、提示 设置IP地址，并进行ping AP测试，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0003(void)
{
        CSUDIAPSSID_S   sSetSsids[AP_TEST_MAX_SSID];
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        int     i =0;
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

    	CSTK_ASSERT_TRUE_FATAL(			
         (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
         );	
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 1),
            "Step 3 check  max ssid count Failure!!!"
        );

        for(i=0; i<nMaxCount && i<AP_TEST_MAX_SSID; ++i)
        {
            sSetSsids[i].nIndex = i;
            sSetSsids[i].bIsEnable = CSUDI_TRUE;
            memset(sSetSsids[i].szSSID, '\0', AP_TEST_NAME_LEN);
            sprintf(sSetSsids[i].szSSID, "%s%d", SSIDNAME, i);

            sSetSsids[i].eWifiMode = EM_AP_WIFI_MODE_11BGN;
            sSetSsids[i].eAuthMode = EM_AP_AUTH_OPEN;
            sSetSsids[i].eEncrypType = EM_AP_ENCRYP_WEP;
            memset(sSetSsids[i].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
            sprintf(sSetSsids[i].szPassWord, "%s", "1234567890123");
            sSetSsids[i].uUpdateTime = 0;
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[i], 1)),			
                "Step 4 Set SSID  should be Success !!!"		
            );            
        }

        do
        {
            CSTCPrint("请使用带有无线功能的设备进行扫描，并确认能够扫描到%s---%d\n", sSetSsids[0].szSSID, nMaxCount-1);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5  打开无线信号失败，无法扫描到所有SSID\n");
                break;
            }

            for(i=1; i<nMaxCount && i<AP_TEST_MAX_SSID; ++i)
            {
                sSetSsids[i].bIsEnable = CSUDI_FALSE;
                CSTK_ASSERT_TRUE_FATAL(			
                    (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[i], 1)),			
                    "Step 6 Set SSID  should be Success !!!"		
                );            
            }

            CSTCPrint("请使用带有无线功能的设备进行扫描，并确认在%s---%d，只能扫描到%s\n", sSetSsids[0].szSSID, nMaxCount-1, sSetSsids[0].szSSID);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 7 关闭SSID失败\n");
                break;
            }

            for(i=1; i<nMaxCount && i<AP_TEST_MAX_SSID; ++i)
            {
                sSetSsids[i].bIsEnable = CSUDI_TRUE;                
                CSTK_ASSERT_TRUE_FATAL(			
                    (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[i], 1)),			
                    "Step 8 Set SSID  should be Success !!!"		
                );            
            }

            CSTCPrint("请使用带有无线功能的设备进行扫描，并确认能够扫描到%s---%d\n", sSetSsids[0].szSSID, nMaxCount-1);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 9 使能无线SSID失败，无法扫描到所有SSID\n");
                break;
            }            


            TEST_AP_PRINT_SSIDINFO(sSetSsids[1].szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids[1].szPassWord)

            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 11 连接SSID:%s失败\n", sSetSsids[1].szSSID);
                break;
            }            

            TEST_AP_PRINT_PING(bRet)
        }while(0);


       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDs(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount)
//@DESCRIPTION: 使能SSID的强化测试，连续关闭和使能1000次，最终能够扫描到，并连接成功
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、无线终端设备
//@INPUT:  1、pszApName
//@INPUT:  2、SSID名称为coship_testap%d，如coship_testap0,coship_testap1.....
//@EXPECTATION: 期望设置SSID成功,并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,依次以coshipTestAp0,coshipTestAp1.....设置SSID名称,并且使能所有的SSID
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、调用CSUDIAPSetSSIDs,关闭和使能coshipTestAp1连续1000次
//@EXECUTIONFLOW: 6、提示进行扫描，期望能够扫描到所有SSID
//@EXECUTIONFLOW: 7、提示用户进行连接coshipTestAp1，期望连接成功
//@EXECUTIONFLOW: 8、提示 设置IP地址，并进行ping AP测试，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0004(void)
{
        CSUDIAPSSID_S   sSetSsids[AP_TEST_MAX_SSID];
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        
        int     nMaxCount =0 ;
        int     i =0;
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

    	CSTK_ASSERT_TRUE_FATAL(			
         (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
         );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 1),
            "Step 4 check  max ssid count Failure!!!"
        );

        for(i=0; i<nMaxCount && i<AP_TEST_MAX_SSID; ++i)
        {
            sSetSsids[i].nIndex = i;
            sSetSsids[i].bIsEnable = CSUDI_TRUE;
            memset(sSetSsids[i].szSSID, '\0', AP_TEST_NAME_LEN);
            sprintf(sSetSsids[i].szSSID, "%s%d", SSIDNAME, i);

            sSetSsids[i].eWifiMode = EM_AP_WIFI_MODE_11BGN;
            sSetSsids[i].eAuthMode = EM_AP_AUTH_OPEN;
            sSetSsids[i].eEncrypType = EM_AP_ENCRYP_WEP;
            memset(sSetSsids[i].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
            sprintf(sSetSsids[i].szPassWord, "%s", "1234567890123");
            sSetSsids[i].uUpdateTime = 0;
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[i], 1)),			
                "Step 5 Set SSID  should be Success !!!"		
            );            
        }

        for(i=0; i<1000; ++i)
        {
            sSetSsids[1].bIsEnable = CSUDI_FALSE;
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[1], 1)),			
                "Step 6 Set SSID  should be Success !!!"		
            );   
            
            sSetSsids[1].bIsEnable = CSUDI_TRUE;
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[1], 1)),			
                "Step 6 Set SSID  should be Success !!!"		
            );   
        }
            
        do
        {
            CSTCPrint("请使用带有无线功能的设备进行扫描，并确认能够扫描到%s---%d\n", sSetSsids[0].szSSID, nMaxCount-1);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 7 打开无线信号失败，无法扫描到所有SSID\n");
                break;
            }

            TEST_AP_PRINT_SSIDINFO(sSetSsids[1].szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids[1].szPassWord)

            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 8 连接SSID:%s失败\n", sSetSsids[1].szSSID);
                break;
            }            

            TEST_AP_PRINT_PING(bRet)
            
        }while(0);


       CSTK_FATAL_POINT
        
	return bRet;
}



//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试授权模式和加密方式组合后不合法的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid=coshipTestAp0
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 5、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_OPEN，加密方式为EM_AP_ENCRYP_TKIP，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_OPEN，加密方式为EM_AP_ENCRYP_AES，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_OPEN，加密方式为EM_AP_ENCRYP_TKIP_AES，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 8、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_SHARE，加密方式为EM_AP_ENCRYP_TKIP，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 9、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_SHARE，加密方式为EM_AP_ENCRYP_AES，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 10、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_SHARE，加密方式为EM_AP_ENCRYP_TKIP_AES，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 11、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_SHARE，加密方式为EM_AP_ENCRYP_NONE，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 12、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_AUTO，加密方式为EM_AP_ENCRYP_TKIP，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 13、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_AUTO，加密方式为EM_AP_ENCRYP_AES，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 14、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_AUTO，加密方式为EM_AP_ENCRYP_TKIP_AES，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 15、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_AUTO，加密方式为EM_AP_ENCRYP_NONE，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 16、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_WPAPSK，加密方式为EM_AP_ENCRYP_WEP，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 17、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_WPAPSK，加密方式为EM_AP_ENCRYP_NONE，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 18、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_WPA2PSK，加密方式为EM_AP_ENCRYP_WEP，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 19、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_WPA2PSK，加密方式为EM_AP_ENCRYP_NONE，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 20、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_WPAPSK_WPA2PSK，加密方式为EM_AP_ENCRYP_WEP，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 21、调用CSUDIAPSetSSIDs, ，认证方式为 EM_AP_AUTH_WPAPSK_WPA2PSK，加密方式为EM_AP_ENCRYP_NONE，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetSSIDs_0005(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        
        int     nMaxCount =0 ;
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	


        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 

               
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 5 config SSID should be Failure !!!"		
        );
        sSetSsids.eEncrypType = EM_AP_ENCRYP_AES;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 6 config SSID should be Failure !!!"		
        );
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP_AES;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 7 config SSID should be Failure !!!"		
        );        

        sSetSsids.eAuthMode = EM_AP_AUTH_SHARE;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 8 config SSID should be Failure !!!"		
        );
        sSetSsids.eEncrypType = EM_AP_ENCRYP_AES;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 9 config SSID should be Failure !!!"		
        );
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP_AES;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 10 config SSID should be Failure !!!"		
        );        
        sSetSsids.eEncrypType = EM_AP_ENCRYP_NONE;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 11 config SSID should be Failure !!!"		
        );    

        sSetSsids.eAuthMode = EM_AP_AUTH_AUTO;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 12 config SSID should be Failure !!!"		
        );
        sSetSsids.eEncrypType = EM_AP_ENCRYP_AES;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 13 config SSID should be Failure !!!"		
        );
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP_AES;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 14 config SSID should be Failure !!!"		
        );   
        sSetSsids.eEncrypType = EM_AP_ENCRYP_NONE;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 15 config SSID should be Failure !!!"		
        );            


        sSetSsids.eAuthMode = EM_AP_AUTH_WPAPSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 16 config SSID should be Failure !!!"		
        );
        sSetSsids.eEncrypType = EM_AP_ENCRYP_NONE;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 17 config SSID should be Failure !!!"		
        );

        sSetSsids.eAuthMode = EM_AP_AUTH_WPA2PSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 18 config SSID should be Failure !!!"		
        );   
        sSetSsids.eEncrypType = EM_AP_ENCRYP_NONE;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 19 config SSID should be Failure !!!"		
        );            


        sSetSsids.eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 20 config SSID should be Failure !!!"		
        );   
        sSetSsids.eEncrypType = EM_AP_ENCRYP_NONE;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),
            "Step 21 config SSID should be Failure !!!"		
        );    

        
       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_OPEN，加密方式为EM_AP_ENCRYP_WEP的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_OPEN
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_WEP
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进提示进行设置IP，并ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0006(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	
        

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 


        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_SHARE，加密方式为EM_AP_ENCRYP_WEP的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_SHARE
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_WEP
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_SHARE,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进提示进行设置IP，并ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0007(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );
        

        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_SHARE;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 

        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_SHARE, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}

//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_AUTO，加密方式为EM_AP_ENCRYP_WEP的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_AUTO
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_WEP
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_AUTO,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进提示进行设置IP，并ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0008(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_AUTO;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 


        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_AUTO, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_OPEN，加密方式为EM_AP_ENCRYP_NONE的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_OPEN
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_NONE
//@INPUT:  5、szPassWord=CSUDI_NULL
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_NONE, uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进行ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0009(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_NONE;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sSetSsids.uUpdateTime = 0;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 


        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_NONE, CSUDI_NULL)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_WPAPSK，加密方式为EM_AP_ENCRYP_TKIP的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_WPAPSK
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_TKIP
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK,eEncrypType = EM_AP_ENCRYP_TKIP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进行ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0010(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );
        

        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_WPAPSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 

        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_WPAPSK, EM_AP_ENCRYP_TKIP, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_WPAPSK，加密方式为EM_AP_ENCRYP_AES的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_WPAPSK
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_AES
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK,eEncrypType = EM_AP_ENCRYP_AES, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进行ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0011(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_WPAPSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_AES;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 

        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_WPAPSK, EM_AP_ENCRYP_AES, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_WPA2PSK，加密方式为EM_AP_ENCRYP_TKIP的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_WPA2PSK
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_TKIP
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK,eEncrypType = EM_AP_ENCRYP_TKIP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进行ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0012(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_WPA2PSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 

        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_WPA2PSK, EM_AP_ENCRYP_TKIP, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_WPA2PSK，加密方式为EM_AP_ENCRYP_AES的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_WPA2PSK
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_AES
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK,eEncrypType = EM_AP_ENCRYP_AES, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进行ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0013(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );

        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_WPA2PSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_AES;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 


        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_WPA2PSK, EM_AP_ENCRYP_AES, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_WPA2PSK，加密方式为EM_AP_ENCRYP_TKIP_AES的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_WPA2PSK
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_TKIP_AES
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK,eEncrypType = EM_AP_ENCRYP_TKIP_AES, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进行ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0014(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_WPA2PSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP_AES;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 


        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_WPA2PSK, EM_AP_ENCRYP_TKIP_AES, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_WPAPSK_WPA2PSK，加密方式为EM_AP_ENCRYP_TKIP的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_WPAPSK_WPA2PSK
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_TKIP
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK,eEncrypType = EM_AP_ENCRYP_TKIP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进行ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0015(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 


        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_WPAPSK_WPA2PSK, EM_AP_ENCRYP_TKIP, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_WPAPSK_WPA2PSK，加密方式为EM_AP_ENCRYP_AES的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_WPAPSK_WPA2PSK
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_AES
//@INPUT:  5、szPassWord="1234567890123"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK,eEncrypType = EM_AP_ENCRYP_AES, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进行ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0016(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_AES;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 


        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_WPAPSK_WPA2PSK, EM_AP_ENCRYP_AES, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，认证方式为EM_AP_AUTH_WPAPSK_WPA2PSK，加密方式为EM_AP_ENCRYP_TKIP_AES的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3、eAuthMode=EM_AP_AUTH_WPAPSK_WPA2PSK
//@INPUT:  4、eEncrypType=EM_AP_ENCRYP_TKIP_AES
//@INPUT:  5、szPassWord="ababababGJLYZ"
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取支持的最大SSID数目，期望最大个数大于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，并使用下面参数进行配置,期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK,eEncrypType = EM_AP_ENCRYP_TKIP_AES, szPassWord="ababababGJLYZ", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，并连接，期望能够连接成功
//@EXECUTIONFLOW: 6、提示进行ping AP，期望能够ping通
CSUDI_BOOL CSTC_AP_SetSSIDs_0017(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	


        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_TKIP_AES;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "ababababGJLYZ");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 

        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_WPAPSK_WPA2PSK, EM_AP_ENCRYP_TKIP_AES, sSetSsids.szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 连接SSID:%s失败\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，配置index为0, 1, 2的SSID的情况
//@PRECONDITION: 平台成功初始化
//@PRECONDITION: AP支持的SSID个数大于等于2
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode
//@INPUT:  3、eAuthMode
//@INPUT:  4、eEncrypType
//@INPUT:  5、szPassWord
//@EXPECTATION: 期望设置成功，并能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回大于等于2
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第1个SSID名称为coshipTestAp0，期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、调用CSUDIAPSetSSIDs,设置第2个SSID名称为coshipTestAp1，期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK, eEncrypType = EM_AP_ENCRYP_AES, szPassWord="1234aaaaa1234", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 6、调用CSUDIAPSetSSIDs,设置第3个SSID名称为coshipTestAp2，期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK, eEncrypType = EM_AP_ENCRYP_AES, szPassWord="aaaa12345cccc", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 7、提示进行无线扫描，并连接SSID为coshipTestAp0,coshipTestAp1，期望都能够连接成功,并ping通 
CSUDI_BOOL CSTC_AP_SetSSIDs_0018(void)
{
        CSUDIAPSSID_S   sSetSsids[AP_TEST_MAX_SSID];
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 2),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids[0].nIndex = 0;
        sSetSsids[0].bIsEnable = CSUDI_TRUE;
        memset(sSetSsids[0].szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids[0].szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids[0].eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids[0].eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids[0].eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids[0].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids[0].szPassWord, "%s", "1234567890123");
        sSetSsids[0].uUpdateTime = 0;

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[0], 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 


        sSetSsids[1].nIndex = 1;
        sSetSsids[1].bIsEnable = CSUDI_TRUE;
        memset(sSetSsids[1].szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids[1].szSSID, "%s%d", SSIDNAME, 1);

        sSetSsids[1].eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids[1].eAuthMode = EM_AP_AUTH_WPA2PSK;
        sSetSsids[1].eEncrypType = EM_AP_ENCRYP_AES;
        memset(sSetSsids[1].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids[1].szPassWord, "%s", "1234aaaaa1234");
        sSetSsids[1].uUpdateTime = 0;

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[1], 1)),			
            "Step 5 Set SSID  should be Success !!!"		
        ); 
        


        sSetSsids[2].nIndex = 2;
        sSetSsids[2].bIsEnable = CSUDI_TRUE;
        memset(sSetSsids[2].szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids[2].szSSID, "%s%d", SSIDNAME, 2);

        sSetSsids[2].eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids[2].eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK;
        sSetSsids[2].eEncrypType = EM_AP_ENCRYP_TKIP_AES;
        memset(sSetSsids[2].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids[2].szPassWord, "%s", "aaaa12345cccc");
        sSetSsids[2].uUpdateTime = 0;

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[2], 1)),			
            "Step 6 Set SSID  should be Success !!!"		
        ); 
        

        do
        {
            TEST_AP_PRINT_SSIDINFO(sSetSsids[0].szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids[0].szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 7 连接SSID:%s失败\n", sSetSsids[0].szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

            TEST_AP_PRINT_SSIDINFO(sSetSsids[1].szSSID, EM_AP_AUTH_WPA2PSK, EM_AP_ENCRYP_AES, sSetSsids[1].szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 8 连接SSID:%s失败\n", sSetSsids[1].szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)
        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}



//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，配置SSID的情况
//@PRECONDITION: 平台成功初始化
//@PRECONDITION: AP支持的SSID个数大于等于2
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode
//@INPUT:  3、eAuthMode
//@INPUT:  4、eEncrypType
//@INPUT:  5、szPassWord
//@EXPECTATION: 期望无加密SSID不用输入密码连接成功，需要加密SSID需要输入密码连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回大于等于2
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第1个SSID为coshipTestAp0, 并用如下参数进行配置,期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_NONE, uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、调用CSUDIAPSetSSIDs,设置第2个SSID为coshipTestAp1, 并用如下参数进行配置,期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK, eEncrypType = EM_AP_ENCRYP_AES, szPassWord="abcdefghijk", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 6、提示进行无线扫描，期望能够扫描到coshipTestAp0，coshipTestAp1
//@EXECUTIONFLOW: 7、提示连接  coshipTestAp0,期望不用输入密码就能够连接成功
//@EXECUTIONFLOW: 8、提示进行设置IP，并ping AP,期望能够ping通AP
//@EXECUTIONFLOW: 9、提示连接  coshipTestAp1,期望不用输入密码，期望不能够连接成功
//@EXECUTIONFLOW: 10、提示连接  coshipTestAp1,提示输入密码，期望能够连接成功
//@EXECUTIONFLOW: 11、提示进行设置IP，并ping AP,期望能够ping通AP
CSUDI_BOOL CSTC_AP_SetSSIDs_0019(void)
{
        CSUDIAPSSID_S   sSetSsids[2];
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 1),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids[0].nIndex = 0;
        sSetSsids[0].bIsEnable = CSUDI_TRUE;
        memset(sSetSsids[0].szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids[0].szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids[0].eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids[0].eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids[0].eEncrypType = EM_AP_ENCRYP_NONE;
        memset(sSetSsids[0].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sSetSsids[0].uUpdateTime = 0;

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[0], 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 


        sSetSsids[1].nIndex = 1;
        sSetSsids[1].bIsEnable = CSUDI_TRUE;
        memset(sSetSsids[1].szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids[1].szSSID, "%s%d", SSIDNAME, 1);

        sSetSsids[1].eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids[1].eAuthMode = EM_AP_AUTH_WPA2PSK;
        sSetSsids[1].eEncrypType = EM_AP_ENCRYP_AES;
        memset(sSetSsids[1].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids[1].szPassWord, "%s", "abcdefghijk");
        sSetSsids[1].uUpdateTime = 0;

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[1], 1)),			
            "Step 5 Set SSID  should be Success !!!"		
        ); 
        
        do
        {
            CSTCPrint("请扫描无线AP，期望能够扫描到%s, %s\n", sSetSsids[0].szSSID, sSetSsids[1].szSSID);
            CSTCPrint("请确认能扫描到y:能够;n:不能扫描到n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 failed  扫描不到无线AP\n");
                break;
            }

            
            CSTCPrint("请连接SSID:%s\n", sSetSsids[0].szSSID);
            AP_iAuthMode2Str(EM_AP_AUTH_OPEN);
            AP_iEncrypType2Str(EM_AP_ENCRYP_NONE);
            CSTCPrint("请确认是否能够连接成功y:能够连接,n:不能够连接\n"); 
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 7 连接SSID:%s失败\n", sSetSsids[0].szSSID);
                break;
            }
            
            TEST_AP_PRINT_PING(bRet)
                
            CSTCPrint("请不用输入密码连接SSID:%s,期望连接不成功\n", sSetSsids[1].szSSID);
            CSTCPrint("请确认不够能连接y:不能够连接;n:可以连接\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 8 failed  连接加密SSID:%s 应该不成功\n", sSetSsids[1].szSSID);
                break;
            }
            
            TEST_AP_PRINT_SSIDINFO(sSetSsids[1].szSSID, EM_AP_AUTH_WPA2PSK, EM_AP_ENCRYP_AES, sSetSsids[1].szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 9 连接SSID:%s失败\n", sSetSsids[1].szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)
        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1、CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: 测试无线连接模式为EM_AP_WIFI_MODE_11BGN，输入错误密码连接情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid
//@INPUT:  2、eWifiMode
//@INPUT:  3、eAuthMode
//@INPUT:  4、eEncrypType
//@INPUT:  5、szPassWord
//@EXPECTATION: 期望加密SSID输入错误密码连接不成功，正确密码连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回大于等于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第2个SSID为coshipTestAp1，期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK, eEncrypType = EM_AP_ENCRYP_AES, szPassWord="abcdefghijk", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示进行无线扫描，期望能够扫描到coshipTestAp0
//@EXECUTIONFLOW: 6、提示连接  coshipTestAp0,期望输入错误密码，连接不成功
//@EXECUTIONFLOW: 7、提示连接  coshipTestAp0,期望输入正确密码，连接成功
//@EXECUTIONFLOW: 8、提示进行设置IP，并ping AP,期望能够ping通AP
CSUDI_BOOL CSTC_AP_SetSSIDs_0020(void)
{
        CSUDIAPSSID_S   sSetSsids[2];
        CSUDI_BOOL       bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 1),
            "Step 3 check  max ssid count Failure!!!"
        );

        sSetSsids[0].nIndex = 1;
        sSetSsids[0].bIsEnable = CSUDI_TRUE;
        memset(sSetSsids[0].szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids[0].szSSID, "%s%d", SSIDNAME, 1);

        sSetSsids[0].eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids[0].eAuthMode = EM_AP_AUTH_WPA2PSK;
        sSetSsids[0].eEncrypType = EM_AP_ENCRYP_AES;
        memset(sSetSsids[0].szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids[0].szPassWord, "%s", "abcdefghijk");
        sSetSsids[0].uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids[0], 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        ); 
        

        do
        {
            CSTCPrint("请扫描无线AP，期望能够扫描到%s\n", sSetSsids[0].szSSID);
            CSTCPrint("请确认能扫描到y:能够;n:不能扫描到n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 failed  扫描不到无线AP\n");
                break;
            }
            
            CSTCPrint("请输入错误密码连接SSID:%s,正确密码为:%s\n", sSetSsids[0].szSSID, sSetSsids[0].szPassWord);
            CSTCPrint("请确认是否能够连接y:不能够连接;n:可以连接\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 failed  输入错误密码连接加密SSID:%s 应该不成功\n", sSetSsids[0].szSSID);
                break;
            }

            
            TEST_AP_PRINT_SSIDINFO(sSetSsids[0].szSSID, EM_AP_AUTH_WPA2PSK, EM_AP_ENCRYP_AES, sSetSsids[0].szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 连接SSID:%s失败\n", sSetSsids[1].szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)
        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}



//@CASEGROUP:CSUDIAPGetStationsInfo(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount)
//@DESCRIPTION: 测试参数为非法值CSUDI_NULL的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szSsid=CSUDI_NULL
//@INPUT:  2、psOutArray=CSUDI_NULL
//@INPUT:  3、pnCount=CSUDI_NULL
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP的能力，期望返回的SSID个数大于0
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs，配置SSID，期望配置成功
//@EXECUTIONFLOW: 5、调用CSUDIAPGetStationsInfo，szSsid传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 6、调用CSUDIAPGetStationsInfo，psOutArray传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、调用CSUDIAPGetStationsInfo，pnCount传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_GetStationsInfo_0001(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDIAPStationInfo_S  sAPStationInfo;
        int     nMaxCount =0 ;
        int     nStationCount = 0;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );
        
        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"	
        );

        nStationCount = 1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetStationsInfo(CSUDI_NULL, &sAPStationInfo, &nStationCount)),			
            "Step 5 Set SSID  should be Success !!!"		
        ); 

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetStationsInfo(sSetSsids.szSSID, CSUDI_NULL, &nStationCount)),			
            "Step 6 Set SSID  should be Success !!!"		
        ); 

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetStationsInfo(sSetSsids.szSSID, &sAPStationInfo, CSUDI_NULL)),			
            "Step 7 Set SSID  should be Success !!!"		
        ); 
        
       CSTK_FATAL_POINT

	return CSUDI_TRUE;
}


//@CASEGROUP:CSUDIAPGetStationsInfo(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount)
//@DESCRIPTION: 测试能够正确获取WIFI终端的信息
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、psOutArray
//@INPUT:  2、pnCount
//@EXPECTATION: 期望返回WIFI终端信息
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回大于等于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipTestAp0，期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示使用WIFI终端进行连接，期望连接成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetStationsInfo，获取WIFI终端信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、检验获取到的信息，提示进行确认是否正确
CSUDI_BOOL CSTC_AP_GetStationsInfo_0002(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDIAPStationInfo_S  sAPStationInfo;
        int     nMaxCount =0 ;
        int     nStationCount = 0;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	


        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );

        
        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"	
        );

       
       TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 5 连接SSID失败!!!"		
        );        

       CSTCPrint("连接成功^^,请把无线终端设置为此段内的IP，%s--%s,\n",
        g_sApDhcpAdd.m_pDhcpBeginAdd, g_sApDhcpAdd.m_pDhcpEndAdd, g_scApAddr);

        nStationCount = 1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetStationsInfo(sSetSsids.szSSID, &sAPStationInfo, &nStationCount)),			
            "Step 6 Set SSID  should be Success !!!"		
        ); 


       CSTCPrint("speed: %d, signale strength: %d\n", sAPStationInfo.uSpeed, sAPStationInfo.uSignalStrength);
       CSTK_ASSERT_TRUE_FATAL(
            (sAPStationInfo.uSpeed > 0 && sAPStationInfo.uSignalStrength > 0),
            "Step 7 check station info fail !!!"		
        );
       CSTCPrint("请确认Mac: %02X:%02X:%02X:%02X:%02X:%02X ", sAPStationInfo.aucMac[0], 
	   	sAPStationInfo.aucMac[1], sAPStationInfo.aucMac[2], sAPStationInfo.aucMac[3], 
	   	sAPStationInfo.aucMac[4], sAPStationInfo.aucMac[5]);
       CSTCPrint("是否和WIFI终端一致,输入y一致，输入n不一致:\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 8 获取WIFI客户端信息失败!!!"		
        );        
       
       CSTK_FATAL_POINT

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIAPGetStationsInfo(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount)
//@DESCRIPTION: 测试获取客户端信息时输入数组大小小于客户端数量的情况
//@PRECONDITION: 平台成功初始化
//@PRECONDITION: 两台或两台以上无线终端设备
//@INPUT:  1、psOutArray
//@INPUT:  2、pnCount
//@EXPECTATION: 期望不死机，正确返回WIFI终端信息
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回大于等于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipTestAp0，期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示使用两台WIFI终端进行连接，期望连接成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetStationsInfo，pnCount传入数值1，获取WIFI终端信息，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 7、检验获取到的信息，提示进行确认是否正确
CSUDI_BOOL CSTC_AP_GetStationsInfo_0003(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDIAPStationInfo_S  sAPStationInfo;
        int     nMaxCount =0 ;
        int     nStationCount = 0;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	


        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );

        
        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        );


       CSTCPrint("请使用两台设备同时连接以下SSID\n");
       TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 5 连接SSID失败!!!"		
        );        

        nStationCount = 1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetStationsInfo(sSetSsids.szSSID, &sAPStationInfo, &nStationCount)),			
            "Step 6 Set SSID  should be Success !!!"		
        ); 

       CSTCPrint("speed: %d, signale strength: %d\n", sAPStationInfo.uSpeed, sAPStationInfo.uSignalStrength);
       CSTK_ASSERT_TRUE_FATAL(
            (sAPStationInfo.uSpeed > 0 && sAPStationInfo.uSignalStrength > 0),
            "Step 7 check station info fail !!!"		
        );

       CSTCPrint("请确认Mac: %02X:%02X:%02X:%02X:%02X:%02X ", sAPStationInfo.aucMac[0], 
	   	sAPStationInfo.aucMac[1], sAPStationInfo.aucMac[2], sAPStationInfo.aucMac[3], 
	   	sAPStationInfo.aucMac[4], sAPStationInfo.aucMac[5]);
       CSTCPrint("是否和其中一个WIFI终端一致,输入y一致，输入n不一致:\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 8 获取WIFI客户端信息失败!!!"		
        );        
       
       CSTK_FATAL_POINT

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIAPGetStationsInfo(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount)
//@DESCRIPTION: 测试禁用无线网络之后不能够获取WIFI终端信息
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、psOutArray
//@INPUT:  2、pnCount
//@EXPECTATION: 期望断开之后不能够获取WIFI终端信息
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回大于等于1
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipTestAp0，期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示使用WIFI终端进行连接，期望连接成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetStationsInfo，获取WIFI终端信息，期望所有值都不为空，工作模式为EM_AP_WIFI_MODE_11BGN
//@EXECUTIONFLOW: 7、提示断开WIFI终端连接
//@EXECUTIONFLOW: 8、调用CSUDIAPGetStationsInfo，获取WIFI终端信息，期望不能够获取WIFI终端信息
CSUDI_BOOL CSTC_AP_GetStationsInfo_0004(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDIAPStationInfo_S  sAPStationInfo[2];
        int     nMaxCount =0 ;
        int     nStationCount = 0;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );

        
        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        );
        

       TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 5 连接SSID失败!!!"		
        );        

        memset(sAPStationInfo, 0, sizeof(CSUDIAPStationInfo_S)*2);
        nStationCount = 1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetStationsInfo(sSetSsids.szSSID, &sAPStationInfo[0], &nStationCount)),			
            "Step 6 Get StationInfo  should be Success !!!"		
        ); 

       CSTCPrint("speed: %d, signale strength: %d\n", sAPStationInfo[0].uSpeed, sAPStationInfo[0].uSignalStrength);
       CSTK_ASSERT_TRUE_FATAL(
            (sAPStationInfo[0].uSpeed > 0 && sAPStationInfo[0].uSignalStrength > 0),
            "Step 7 check station info fail !!!"		
        );

       CSTCPrint("请确认Mac: %02X:%02X:%02X:%02X:%02X:%02X ", sAPStationInfo[0].aucMac[0], 
	   	sAPStationInfo[0].aucMac[1], sAPStationInfo[0].aucMac[2], sAPStationInfo[0].aucMac[3], 
	   	sAPStationInfo[0].aucMac[4], sAPStationInfo[0].aucMac[5]);
       CSTCPrint("是否和其中一个WIFI终端一致,输入y一致，输入n不一致:\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 7获取WIFI客户端信息失败!!!"		
        );       


        CSTCPrint("请禁用无线网络\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 8 下面过程需要断开WIFI连接!!!"		
        );   

        
         nStationCount = 1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetStationsInfo(sSetSsids.szSSID, &sAPStationInfo[1], &nStationCount)),			
            "Step 9 Get StationInfo  should be Success !!!"		
        ); 

       CSTK_ASSERT_TRUE_FATAL(
            (sAPStationInfo[1].uSpeed == 0),
            "Step 10 check station info fail !!!"		
        );       
       
       CSTK_FATAL_POINT

	return CSUDI_TRUE;
}




//@CASEGROUP:CSUDIAPGetStationsInfo(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount)
//@DESCRIPTION: 测试不使能SSID之后，不能够获取客户端信息
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、psOutArray
//@INPUT:  2、pnCount
//@EXPECTATION: 期望关闭SSID使能之后，不能够获取客户端信息
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望返回大于等于2
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第二个SSID为coshipTestAp1，期望设置成功
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、提示使用WIFI终端进行连接，期望连接成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetStationsInfo，获取WIFI终端信息，期望所有值都不为空，工作模式为EM_AP_WIFI_MODE_11BGN
//@EXECUTIONFLOW: 7、调用CSUDIAPSetSSIDs，关闭此SSID的使能，期望关闭成功
//@EXECUTIONFLOW: 8、调用CSUDIAPGetStationsInfo，获取WIFI终端信息，期望不能够获取WIFI终端信息
CSUDI_BOOL CSTC_AP_GetStationsInfo_0005(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDIAPStationInfo_S  sAPStationInfo[2];
        int     nMaxCount =0 ;
        int     nStationCount = 0;
        
            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap channel should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );

        
        sSetSsids.nIndex = 1;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 1);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        );


       TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 5 连接SSID失败!!!"		
        );        

        nStationCount = 1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetStationsInfo(sSetSsids.szSSID, &sAPStationInfo[0], &nStationCount)),			
            "Step 6 Set SSID  should be Success !!!"		
        ); 

       CSTCPrint("speed: %d, signale strength: %d\n", sAPStationInfo[0].uSpeed, sAPStationInfo[0].uSignalStrength);
       CSTK_ASSERT_TRUE_FATAL(
            (sAPStationInfo[0].uSpeed > 0 && sAPStationInfo[0].uSignalStrength > 0),
            "Step 7 check station info fail !!!"		
        );
       
       CSTCPrint("请确认Mac: %02X:%02X:%02X:%02X:%02X:%02X ", sAPStationInfo[0].aucMac[0], 
	   	sAPStationInfo[0].aucMac[1], sAPStationInfo[0].aucMac[2], sAPStationInfo[0].aucMac[3], 
	   	sAPStationInfo[0].aucMac[4], sAPStationInfo[0].aucMac[5]);
       CSTCPrint("是否和其中一个WIFI终端一致,输入y一致，输入n不一致:\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 6 获取WIFI客户端信息失败!!!"		
        );      


        sSetSsids.bIsEnable = CSUDI_FALSE;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 7 Set SSID  should be Success !!!"		
        );


        nStationCount = 1;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetStationsInfo(sSetSsids.szSSID, &sAPStationInfo[1], &nStationCount)),			
            "Step 8 Set SSID  should be Success !!!"		
        ); 

       CSTK_ASSERT_TRUE_FATAL(
            (sAPStationInfo[1].uSpeed == 0 && sAPStationInfo[1].uSignalStrength == 0),
            "Step 9 check station info fail !!!"		
        );
       
       
       CSTK_FATAL_POINT

	return CSUDI_TRUE;
}



//@CASEGROUP:CSAPSetAdvancedInfo(char *szApName,CSAPAdvancedInfo_S *psInfo)
//@DESCRIPTION: 测试参数为非法值CSUDI_NULL的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、szApName=CSUDI_NULL
//@INPUT:  2、psInfo=CSUDI_NULL
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSAPSetAdvancedInfo，AP名称szApName传入CSUDI_NULL,psInfo传入合法值，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSAPSetAdvancedInfo，AP名称szApName传入合法值,psInfo传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSAPSetAdvancedInfo，AP名称szApName传入CSUDI_NULL,psInfo传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetGetAdvancedInfo_0001(void)
{
        CSUDIAPAdvancedInfo_S   sInfo;

        sInfo.eBGMode = EM_AP_BGPROTECT_MODE_AUTO;
        sInfo.nBeaconInterval = 999;
        sInfo.nDivideLimit = 2346;
        sInfo.nFreqPower = 100;
        sInfo.nTranferLimit = 2347;
        sInfo.bDataPacketGrab = CSUDI_TRUE;
        sInfo.bShortStartCode = CSUDI_TRUE;
        sInfo.bShortClashSlot = CSUDI_TRUE;
        sInfo.bTransportBurst = CSUDI_TRUE;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetAdvancedInfo(CSUDI_NULL, &sInfo)),			
            "Step 1 Set advanced info  should not be Success !!!"		
        ); 

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetAdvancedInfo(g_scApName, CSUDI_NULL)),			
            "Step 2 Set advanced info  should not be Success !!!"		
        ); 

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPSetAdvancedInfo(CSUDI_NULL, CSUDI_NULL)),			
            "Step 3 Set advanced info  should not be Success !!!"		
        ); 

       CSTK_FATAL_POINT
        
        return CSUDI_TRUE;
}


//@CASEGROUP:CSAPGetAdvancedInfo(char *szApName,CSAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: 测试参数为非法值CSUDI_NULL的情况
//@PRECONDITION: 平台成功初始化
//@INPUT:  1、pszApName=CSUDI_NULL
//@INPUT:  2、pstAdvancedInfo=CSUDI_NULL
//@EXPECTATION: 期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 1、调用CSAPGetAdvancedInfo，AP名称szApName传入CSUDI_NULL,psOutInfo传入合法值，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSAPGetAdvancedInfo，AP名称szApName传入合法值,psOutInfo传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSAPGetAdvancedInfo，AP名称szApName传入CSUDI_NULL,psOutInfo传入CSUDI_NULL，期望返回非CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetGetAdvancedInfo_0002(void)
{
        CSUDIAPAdvancedInfo_S   sInfo;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetAdvancedInfo(CSUDI_NULL, &sInfo)),			
            "Step 1 Set advanced info  should not be Success !!!"		
        ); 

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetAdvancedInfo(g_scApName, CSUDI_NULL)),			
            "Step 2 Set advanced info  should not be Success !!!"		
        ); 

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS != CSUDIAPGetAdvancedInfo(CSUDI_NULL, CSUDI_NULL)),			
            "Step 3 Set advanced info  should not be Success !!!"		
        ); 

       CSTK_FATAL_POINT
        
        return CSUDI_TRUE;
}


//@CASEGROUP:1、CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2、CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: 测试改变BG模式之后WIFI终端是否能够连接成功
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、无线终端设备
//@INPUT:  1、m_enBGMode
//@EXPECTATION: 期望能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望大于0
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、调用CSAPSetAdvancedInfo，设置BG模式为EM_AP_BGPROTECT_MODE_AUTO，期望设置成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetAdvancedInfo，获取BG模式，并进行比较，期望和设置是一致
//@EXECUTIONFLOW: 7、提示进行无线连接，期望能够连接成功,并进行Ping测试，期望能够ping通AP
//@EXECUTIONFLOW: 8、重复步骤5-7，依次设置BG模式为EM_AP_BGPROTECT_MODE_OPEN，EM_AP_BGPROTECT_MODE_CLOSE，期望每一步都成功
CSUDI_BOOL CSTC_AP_SetGetAdvancedInfo_0003(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL      bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        int     i = 0;
        
        CSUDIAPAdvancedInfo_S   sSetInfo;
        CSUDIAPAdvancedInfo_S   sGetInfo;

            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap Signal should be Success !!!"		
        );	        

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        )


        sSetInfo.nBeaconInterval = 999;
        sSetInfo.nDivideLimit = 2346;
        sSetInfo.nFreqPower = 100;
        sSetInfo.nTranferLimit = 2347;
        sSetInfo.bDataPacketGrab = CSUDI_TRUE;
        sSetInfo.bShortStartCode = CSUDI_TRUE;
        sSetInfo.bShortClashSlot = CSUDI_TRUE;
        sSetInfo.bTransportBurst = CSUDI_TRUE;


        for(i = 0; i<=EM_AP_BGPROTECT_MODE_CLOSE; ++i)
        {
            sSetInfo.eBGMode = EM_AP_BGPROTECT_MODE_AUTO+i;

            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetAdvancedInfo(g_scApName, &sSetInfo)),			
                "Step 5 Set SSID  should be Success !!!"		
            )
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPGetAdvancedInfo(g_scApName, &sGetInfo)),			
                "Step 6 Set SSID  should be Success !!!"		
            )

           CSTK_ASSERT_TRUE_FATAL(			
                (sSetInfo.eBGMode == sGetInfo.eBGMode),"step 7 连接SSID失败!!!"		
            );                

           TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
           CSTK_ASSERT_TRUE_FATAL(			
                (CSTKWaitYes()),"step 8 连接SSID失败!!!"		
            );        

           TEST_AP_PRINT_PING(bRet)
        }
       
       CSTK_FATAL_POINT

	return bRet;
}



//@CASEGROUP:1、CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2、CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: 测试改变发射功率之后WIFI终端是否能够连接成功
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、具有无线网卡的设备
//@INPUT:  1、nFreqPower
//@EXPECTATION: 发射功率减小，信号强度减弱
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望大于0
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、调用CSAPSetAdvancedInfo，设置发射功率为100，期望设置成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetAdvancedInfo，获取发射功率，并验证，期望和之前设置一致
//@EXECUTIONFLOW: 7、提示进行无线连接，期望能够连接成功
//@EXECUTIONFLOW: 8、提示进行保存信号强度，并Ping Ap，期望能够ping通
//@EXECUTIONFLOW: 9、重复步骤5-8，设置发射功率为76期望每一步都成功
//@EXECUTIONFLOW: 10、重复步骤5-8，设置发射功率为52期望每一步都成功
//@EXECUTIONFLOW: 11、重复步骤5-8，设置发射功率为28期望每一步都成功
//@EXECUTIONFLOW: 12、重复步骤5-8，设置发射功率为4期望无法扫描到coshipTestAp0的SSID
//@EXECUTIONFLOW: 13、检验信号强度，期望当发射功率减弱时，信号强度响应减弱
CSUDI_BOOL CSTC_AP_SetGetAdvancedInfo_0004(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL      bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        int     i = 0;
        
        CSUDIAPAdvancedInfo_S   sSetInfo;
        CSUDIAPAdvancedInfo_S   sGetInfo;

            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap Signal should be Success !!!"		
        );	        

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        )

        sSetInfo.eBGMode = EM_AP_BGPROTECT_MODE_AUTO;
        sSetInfo.nBeaconInterval = 999;
        sSetInfo.nDivideLimit = 2346;
        sSetInfo.nTranferLimit = 2347;
        sSetInfo.bDataPacketGrab = CSUDI_TRUE;
        sSetInfo.bShortStartCode = CSUDI_TRUE;
        sSetInfo.bShortClashSlot = CSUDI_TRUE;
        sSetInfo.bTransportBurst = CSUDI_TRUE;


        for(i = 100; i>=1; i=i-24)
        {

            sSetInfo.nFreqPower = i;
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetAdvancedInfo(g_scApName, &sSetInfo)),			
                "Step 5 Set AP advanced  should be Success !!!"		
            )
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPGetAdvancedInfo(g_scApName, &sGetInfo)),			
                "Step 6 Get AP advanced  should be Success !!!"		
            )

           CSTK_ASSERT_TRUE_FATAL(			
                (sSetInfo.nFreqPower == sGetInfo.nFreqPower),"step 6 check AP freq power failed!!!"		
            );                
             
           TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
           CSTK_ASSERT_TRUE_FATAL(			
                (CSTKWaitYes()),"step 7 连接SSID失败!!!"		
            );        

          CSTCPrint("当前发射功率为%d，请记录信号强度,后续需要检验\n", sSetInfo.nFreqPower);

           TEST_AP_PRINT_PING(bRet)

        }

        CSTCPrint("发射功率依次从100 -76-52-4,请确认信号强度依次减弱\n");
        CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 8 验证信号强度失败!!!"		
        );            
        
       CSTK_FATAL_POINT

	return bRet;
}



//@CASEGROUP:1、CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2、CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: 测试改变信标间隔之后，WIFI终端是否能够连接成功
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、具有无线网卡的设备
//@INPUT:  1、nBeaconInterval
//@EXPECTATION: 期望能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望大于0
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、调用CSAPSetAdvancedInfo，设置信标间隔为999，期望设置成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetAdvancedInfo，获取信标间隔，并比较，期望和设置时一致
//@EXECUTIONFLOW: 7、提示进行无线连接，期望能够连接成功
//@EXECUTIONFLOW: 8、提示进行ping测试，期望能够ping通AP
//@EXECUTIONFLOW: 9、重复步骤5-8，设置信标间隔为510，期望每一步都成功
//@EXECUTIONFLOW: 10、重复步骤5-8，设置信标间隔为21，期望每一步都成功
CSUDI_BOOL CSTC_AP_SetGetAdvancedInfo_0005(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL      bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        int     i = 0;
        
        CSUDIAPAdvancedInfo_S   sSetInfo;
        CSUDIAPAdvancedInfo_S   sGetInfo;

            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap Signal should be Success !!!"		
        );	        

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        )


        sSetInfo.eBGMode = EM_AP_BGPROTECT_MODE_AUTO;
        sSetInfo.nDivideLimit = 2346;
        sSetInfo.nTranferLimit = 2347;
        sSetInfo.nFreqPower = 100;
        sSetInfo.bDataPacketGrab = CSUDI_TRUE;
        sSetInfo.bShortStartCode = CSUDI_TRUE;
        sSetInfo.bShortClashSlot = CSUDI_TRUE;
        sSetInfo.bTransportBurst = CSUDI_TRUE;

        for(i = 999; i>=0; i=i-489) //beacon interval value:  999--510--21
        {
            sSetInfo.nBeaconInterval = i;
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetAdvancedInfo(g_scApName, &sSetInfo)),			
                "Step 5 Set AP advanced  should be Success !!!"		
            )
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPGetAdvancedInfo(g_scApName, &sGetInfo)),			
                "Step 6 Get AP advanced  should be Success !!!"		
            )

           CSTK_ASSERT_TRUE_FATAL(			
                (sSetInfo.nBeaconInterval == sGetInfo.nBeaconInterval),"step 7 check beacon interval failed!!!"		
            );                
            

           TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
           CSTK_ASSERT_TRUE_FATAL(			
                (CSTKWaitYes()),"step 8 连接SSID失败!!!"		
            );        
           TEST_AP_PRINT_PING(bRet)

        }
        
       CSTK_FATAL_POINT

	return bRet;
}


//@CASEGROUP:1、CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2、CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: 测试改变分割界限之后，WIFI终端是否能够连接成功
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、具有无线网卡的设备
//@INPUT:  1、nDivideLimit
//@EXPECTATION: 期望能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望大于0
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、调用CSAPSetAdvancedInfo，设置分割界限为2347，期望设置成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetAdvancedInfo，获取分割界限，并比较，期望获取和设置时 一致
//@EXECUTIONFLOW: 7、提示进行无线连接，期望能够连接成功
//@EXECUTIONFLOW: 8、提示Ping测试，期望能够Ping通AP
//@EXECUTIONFLOW: 9、重复步骤5-8，设置分割界限为1174，期望每一步都成功
//@EXECUTIONFLOW: 10、重复步骤5-8，设置分割界限为0，期望每一步都成功
CSUDI_BOOL CSTC_AP_SetGetAdvancedInfo_0006(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL      bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        int     i = 0;
        
        CSUDIAPAdvancedInfo_S   sSetInfo;
        CSUDIAPAdvancedInfo_S   sGetInfo;

            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap Signal should be Success !!!"		
        );	        

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        )


        sSetInfo.eBGMode = EM_AP_BGPROTECT_MODE_AUTO;
        sSetInfo.nBeaconInterval = 999;
        sSetInfo.nTranferLimit = 2347;
        sSetInfo.nFreqPower = 100;
        sSetInfo.bDataPacketGrab = CSUDI_TRUE;
        sSetInfo.bShortStartCode = CSUDI_TRUE;
        sSetInfo.bShortClashSlot = CSUDI_TRUE;
        sSetInfo.bTransportBurst = CSUDI_TRUE;

        for(i = 2346; i>=0; i=i-1045) //nDivideLimit value:  2346--1301--256
        {
            sSetInfo.nDivideLimit = i;
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetAdvancedInfo(g_scApName, &sSetInfo)),			
                "Step 5 Set AP advanced  should be Success !!!"		
            )
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPGetAdvancedInfo(g_scApName, &sGetInfo)),			
                "Step 6 Get AP advanced  should be Success !!!"		
            )

           CSTK_ASSERT_TRUE_FATAL(			
                (sSetInfo.nDivideLimit == sGetInfo.nDivideLimit),"step 7 check beacon interval failed!!!"		
            );                


           TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
           CSTK_ASSERT_TRUE_FATAL(			
                (CSTKWaitYes()),"step 8 连接SSID失败!!!"		
            );        
           TEST_AP_PRINT_PING(bRet)

        }
        
       CSTK_FATAL_POINT

	return bRet;
}



//@CASEGROUP:1、CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2、CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: 测试改变传输分割界限之后，WIFI终端是否能够连接成功
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、具有无线网卡的设备
//@INPUT:  1、nTranferLimit
//@EXPECTATION: 期望改变传输分割界限之后，能够连接成功
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望大于0
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、调用CSAPSetAdvancedInfo，设置传输分割界限为2347，期望设置成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetAdvancedInfo，获取分割界限，并比较，期望获取和设置时 一致
//@EXECUTIONFLOW: 7、提示进行无线连接，期望能够连接成功
//@EXECUTIONFLOW: 8、提示Ping测试，期望能够Ping通AP
//@EXECUTIONFLOW: 9、重复步骤5-8，设置分割界限为1174，期望每一步都成功
//@EXECUTIONFLOW: 10、重复步骤5-8，设置分割界限为1，期望每一步都成功
CSUDI_BOOL CSTC_AP_SetGetAdvancedInfo_0007(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL      bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        int     i = 0;
        
        CSUDIAPAdvancedInfo_S   sSetInfo;
        CSUDIAPAdvancedInfo_S   sGetInfo;

            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap Signal should be Success !!!"		
        );	        

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        )


        sSetInfo.eBGMode = EM_AP_BGPROTECT_MODE_AUTO;
        sSetInfo.nBeaconInterval = 999;
        sSetInfo.nDivideLimit = 2346;
        sSetInfo.nFreqPower = 100;
        sSetInfo.bDataPacketGrab = CSUDI_TRUE;
        sSetInfo.bShortStartCode = CSUDI_TRUE;
        sSetInfo.bShortClashSlot = CSUDI_TRUE;
        sSetInfo.bTransportBurst = CSUDI_TRUE;

        for(i = 2347; i>=0; i=i-1173) //TranferLimit value:  2347--1174--1
        {
            sSetInfo.nTranferLimit = i;
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetAdvancedInfo(g_scApName, &sSetInfo)),			
                "Step 5 Set AP advanced  should be Success !!!"		
            )
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPGetAdvancedInfo(g_scApName, &sGetInfo)),			
                "Step 6 Get AP advanced  should be Success !!!"		
            )

           CSTK_ASSERT_TRUE_FATAL(			
                (sSetInfo.nTranferLimit == sGetInfo.nTranferLimit),"step 7 check TranferLimit failed!!!"		
            );                


           TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
           CSTK_ASSERT_TRUE_FATAL(			
                (CSTKWaitYes()),"step 8 连接SSID失败!!!"		
            );        
           TEST_AP_PRINT_PING(bRet)

        }
        
       CSTK_FATAL_POINT

	return bRet;
}



//@CASEGROUP:1、CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2、CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: 测试是否能改变AP的高级参数，前导码，端碰撞槽，传输突发，数据包聚合
//@PRECONDITION: 1、平台成功初始化
//@PRECONDITION: 2、具有无线网卡的设备
//@INPUT:  1、bShortStartCode
//@INPUT:  2、bShortClashSlot
//@INPUT:  3、bTransportBurst
//@INPUT:  4、bDataPacketGrab
//@EXPECTATION: 能够成功设置AP高级参数
//@EXECUTIONFLOW: 1、调用CSUDIAPSetSignal，使能AP，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 2、调用CSUDIAPSetFreqChannel，设置AP信道为EM_AP_CHANNEL_6_2437M，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 3、调用CSUDIAPGetSSIDCapbility，获取AP支持的最大SSID个数，期望大于0
//@EXECUTIONFLOW: 4、调用CSUDIAPSetSSIDs,设置第一个SSID为coshipApTest0，期望返回CSUDI_SUCCESS
//@EXECUTIONFLOW: 配置参数为eWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0，期望设置成功
//@EXECUTIONFLOW: 5、调用CSAPSetAdvancedInfo，设置所有参数都为CSUDI_TRUE，期望设置成功
//@EXECUTIONFLOW: 6、调用CSUDIAPGetAdvancedInfo，获取所设置参数，期望设置成功
//@EXECUTIONFLOW: 7、比较设置的高级参数，期望全为CSUDI_TRUE
//@EXECUTIONFLOW: 8、提示进行无线连接，期望能够连接成功
//@EXECUTIONFLOW: 9、调用CSAPSetAdvancedInfo，设置所有参数都为CSUDI_FALSE，期望设置成功
//@EXECUTIONFLOW: 10、调用CSUDIAPGetAdvancedInfo，获取所设置参数，期望设置成功
//@EXECUTIONFLOW: 11、比较设置的高级参数，期望全为CSUDI_FALSE
//@EXECUTIONFLOW: 12、提示进行无线连接，期望能够连接成功
CSUDI_BOOL CSTC_AP_SetGetAdvancedInfo_0008(void)
{
        CSUDIAPSSID_S   sSetSsids;
        CSUDI_BOOL      bRet = CSUDI_TRUE;
        int     nMaxCount =0 ;
        
        CSUDIAPAdvancedInfo_S   sSetInfo;
        CSUDIAPAdvancedInfo_S   sGetInfo;

            
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Success !!!"		
        );	

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetFreqChannel(g_scApName, EM_AP_CHANNEL_6_2437M)),			
            "Step 2 Set Ap Signal should be Success !!!"		
        );	        

        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetSSIDCapbility(g_scApName, &nMaxCount)),			
            "Step 3 Get SSID capbility should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(
            (nMaxCount > 0),
            "Step 3 check  max ssid count Failure!!!"
        );


        sSetSsids.nIndex = 0;
        sSetSsids.bIsEnable = CSUDI_TRUE;
        memset(sSetSsids.szSSID, '\0', AP_TEST_NAME_LEN);
        sprintf(sSetSsids.szSSID, "%s%d", SSIDNAME, 0);

        sSetSsids.eWifiMode = EM_AP_WIFI_MODE_11BGN;
        sSetSsids.eAuthMode = EM_AP_AUTH_OPEN;
        sSetSsids.eEncrypType = EM_AP_ENCRYP_WEP;
        memset(sSetSsids.szPassWord, '\0', CSUDIAP_PASSWORD_LEN);
        sprintf(sSetSsids.szPassWord, "%s", "1234567890123");
        sSetSsids.uUpdateTime = 0;
        
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetSSIDs(g_scApName, &sSetSsids, 1)),			
            "Step 4 Set SSID  should be Success !!!"		
        )


        sSetInfo.eBGMode = EM_AP_BGPROTECT_MODE_AUTO;
        sSetInfo.nBeaconInterval = 999;
        sSetInfo.nTranferLimit = 2347;
        sSetInfo.nFreqPower = 100;
        sSetInfo.nDivideLimit = 2346;


        sSetInfo.bDataPacketGrab = CSUDI_TRUE;
        sSetInfo.bShortStartCode = CSUDI_TRUE;
        sSetInfo.bShortClashSlot = CSUDI_TRUE;
        sSetInfo.bTransportBurst = CSUDI_TRUE;
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPSetAdvancedInfo(g_scApName, &sSetInfo)),			
            "Step 6 Set AP advanced  should be Success !!!"		
        );
        CSTK_ASSERT_TRUE_FATAL(			
            (CSUDI_SUCCESS == CSUDIAPGetAdvancedInfo(g_scApName, &sGetInfo)),			
            "Step 7 Get AP advanced  should be Success !!!"		
        );
       CSTK_ASSERT_TRUE_FATAL(			
            (sSetInfo.bDataPacketGrab == sGetInfo.bDataPacketGrab &&
                sSetInfo.bShortStartCode == sGetInfo.bShortStartCode &&
                sSetInfo.bShortClashSlot == sGetInfo.bShortClashSlot &&
                sSetInfo.bTransportBurst == sGetInfo.bTransportBurst 
            ), "step 7 check advanced failed!!!"		
        );                

        do
        {
           TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
            if(!CSTKWaitYes())
            {
                    CSTCPrint("step 8 连接SSID失败!!!\n");	
            }
           
           TEST_AP_PRINT_PING(bRet)


            sSetInfo.bDataPacketGrab = CSUDI_FALSE;
            sSetInfo.bShortStartCode = CSUDI_FALSE;
            sSetInfo.bShortClashSlot = CSUDI_FALSE;
            sSetInfo.bTransportBurst = CSUDI_FALSE;
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetAdvancedInfo(g_scApName, &sSetInfo)),			
                "Step 9 Set AP advanced  should be Success !!!"		
            );
            
            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPGetAdvancedInfo(g_scApName, &sGetInfo)),			
                "Step 10 Get AP advanced  should be Success !!!"		
            );
           CSTK_ASSERT_TRUE_FATAL(			
                (sSetInfo.bDataPacketGrab == sGetInfo.bDataPacketGrab &&
                    sSetInfo.bShortStartCode == sGetInfo.bShortStartCode &&
                    sSetInfo.bShortClashSlot == sGetInfo.bShortClashSlot &&
                    sSetInfo.bTransportBurst == sGetInfo.bTransportBurst 
                ), "step 11 check advanced failed!!!"		
            );                

           TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
            if(!CSTKWaitYes())
            {
                    CSTCPrint("step 12 连接SSID失败!!!\n");	
            }
           
           TEST_AP_PRINT_PING(bRet)
        }while(0);
        
       CSTK_FATAL_POINT

	return bRet;
}





