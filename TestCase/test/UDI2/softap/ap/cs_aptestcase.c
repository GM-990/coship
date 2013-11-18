/* --------------------------------------------------------------------
ע�⣺
1.����Ҫ���û������Ĳ��������У����ԣ�
	a. ʹ��CSTKWaitAnyKey�ȴ��û��������ⰴ��
	b. ʹ��CSTKWaitYes�ȴ��û�����YES
2.��������������������������ID��"��������ID"�����ڲ��������ĵ���
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
            CSTCPrint("��֤��ʽΪ OPEN\n");
            break;

        case EM_AP_AUTH_SHARE:
            CSTCPrint("��֤��ʽΪ SHARE\n");
            break;

        case EM_AP_AUTH_AUTO:
            CSTCPrint("��֤��ʽΪ AUTO\n");
            break;

        case EM_AP_AUTH_WPAPSK:
            CSTCPrint("��֤��ʽΪ WPAPSK\n");
            break;

        case EM_AP_AUTH_WPA2PSK:
            CSTCPrint("��֤��ʽΪ WPA2PSK\n");
            break;

        case EM_AP_AUTH_WPAPSK_WPA2PSK:
            CSTCPrint("��֤��ʽΪ WPAPSK_WPA2PSK\n");
            break; 

        default:
            CSTCPrint("�޴���֤��ʽ %d\n", eAuthMode);
    }
    
}

static void AP_iEncrypType2Str(CSUDIAPEncrypType_E eEncrypType)
{
    switch(eEncrypType)
    {
        case EM_AP_ENCRYP_WEP:
            CSTCPrint("����ģʽΪ WEP\n");
            break;

        case EM_AP_ENCRYP_TKIP:
            CSTCPrint("����ģʽΪ TKIP\n");
            break;

        case EM_AP_ENCRYP_AES:
            CSTCPrint("����ģʽΪ AES\n");
            break;

        case EM_AP_ENCRYP_TKIP_AES:
            CSTCPrint("����ģʽΪ TKIP_AES\n");
            break;

        case EM_AP_ENCRYP_NONE:
            CSTCPrint("����ģʽΪ NONE\n");
            break;

        default:
            CSTCPrint("�޴˼���ģʽ %d\n", eEncrypType);
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
            CSTCPrint("������SSID:%s\n", name);   \
            AP_iAuthMode2Str(authmod);   \
            AP_iEncrypType2Str(encryp);   \
            if(passwd != CSUDI_NULL)     \
            {   \
                CSTCPrint("����:%s\n", passwd);                    \
            }       \
            CSTCPrint("��ȷ���Ƿ��ܹ����ӳɹ�y:�ܹ�����,n:���ܹ�����\n"); 


#define     TEST_AP_PRINT_PING(bRet)    \
        CSTCPrint("���ӳɹ�^^,��������ն�����Ϊ�˶��ڵ�IP��%s--%s,\n", \
        g_sApDhcpAdd.m_pDhcpBeginAdd, g_sApDhcpAdd.m_pDhcpEndAdd, g_scApAddr);  \
        CSTCPrint("���ú�IP֮����ʹ�������ն�ping AP:%s, ��ȷ���ܷ�ping ͨ\n", g_scApAddr);   \
        if(!CSTKWaitYes())  \
        {                               \
            bRet = CSUDI_FALSE;     \
            CSTCPrint("failed �����ն��޷�ping ͨAP\n");    \
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
    //�ڱ�����������ִ�к����
    g_snMaxSSIDCount = 0;

    memset(g_scApName, '\0', AP_TEST_NAME_LEN);
    memset(g_sApDhcpAdd.m_pDhcpBeginAdd, '\0', AP_TEST_IP_LEN);
    memset(g_sApDhcpAdd.m_pDhcpEndAdd, '\0', AP_TEST_IP_LEN);
    memset(g_scApAddr, '\0', AP_TEST_IP_LEN);

	return CSUDI_TRUE;
}




//@CASEGROUP:1��CSUDIAPSetSignal(char *szApName,CSAP_BOOL bEnable)
//@DESCRIPTION: ���Բ���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��pszApName = CSUDI_NULL
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.����CSUDIAPSetSignal��AP����szApName����CSUDI_NULL���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetSignal_0001(void)
{
    	CSTK_ASSERT_TRUE_FATAL(			
         (CSUDI_SUCCESS != CSUDIAPSetSignal(CSUDI_NULL, CSUDI_TRUE)),			
            "Step 1 Set Ap Signal should be Failure !!!"		
         );	
        
       CSTK_FATAL_POINT
        
	return CSUDI_TRUE;
}


//@CASEGROUP:1��CSUDIAPSetSignal(const char* szApName,CSUDI_BOOL bEnable)
//@DESCRIPTION: ���Թر������ź�֮���ٴ������ź��Ƿ��ܹ���ɨ�赽
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�ɨ������AP���ն�
//@INPUT: 1��bEnable 
//@EXPECTATION: �����ر�APʱ�޷�ɨ�赽���е�SSID����ʱ�ܹ�ɨ�赽
//@EXECUTIONFLOW: 1.����CSUDIAPSetSignal��bEnable����CSUDI_TRUE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.����CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ�SSID�������������ڵ���1
//@EXECUTIONFLOW: 3.����CSUDIAPSetSSIDs����������SSID������ΪcoshipTestApI��IΪ0 1...pnMaxCount-1���������óɹ�
//@EXECUTIONFLOW: 4.ʹ�������豸����ɨ�裬�����ܹ�ɨ�赽����SSID
//@EXECUTIONFLOW: 5.����CSUDIAPSetSignal��bEnable����CSUDI_FALSE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 6.ʹ�������豸����ɨ�裬��������ɨ�赽�κ���ʾ����SSID
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

        CSTCPrint("��ʹ�ô������߹��ܵ��豸����ɨ�裬��ȷ����ɨ�赽����SSID\n");
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
                CSTCPrint("Step 4 ɨ�費������SSID����AP signal ����ʧ��\n");
                break;
            }

            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSignal(g_scApName, CSUDI_FALSE)),			
                "Step 5 Set Ap Signal should be Success !!!"		
            );	

            CSTCPrint("��ʹ�ô������߹��ܵ��豸����ɨ�裬��ȷ�ϲ���ɨ�赽����SSID\n");
            for(nSSIDIndex=0; nSSIDIndex<nMaxSSIDCount; ++nSSIDIndex)
            {
                CSTCPrint("%s ", saSSID[nSSIDIndex].szSSID);
            }
            CSTCPrint("\n");

            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("Step 6 �ر������źŻ���ɨ�赽����SSID���ر������ź�ʧ��\n");
                break;
            }
            
        }while(0);
         

       CSTK_FATAL_POINT
        
        return bRet;
}



//@CASEGROUP:1��CSUDIAPSetSignal(char *szApName,CSAP_BOOL bEnable)
//@DESCRIPTION: ���������رպʹ������ź�1000��֮���ܹ�ɨ�赽����SSID�����ӳɹ�
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�ɨ������AP���ն�
//@INPUT: 1��bEnable 
//@EXPECTATION: �����򿪺͹ر�����APʱ���ܹ��ɹ�������Ӧ��ɨ�赽����AP���������ӳɹ�
//@EXECUTIONFLOW: 1.����CSUDIAPSetSignal��bEnable����CSUDI_TRUE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.����CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������0
//@EXECUTIONFLOW: 3.����CSUDIAPSetSSIDs�����õ�һ��SSIDΪcoshipTestAp0���������²������ã���������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0
//@EXECUTIONFLOW: 4.�������� CSUDIAPSetSignal���رպ�ʹ��AP 1000�Σ�����ÿһ�ζ��ɹ�
//@EXECUTIONFLOW: 5.ʹ�ô��������ն��豸����ɨ�裬�����ܹ�ɨ�赽coshipTestAp0
//@EXECUTIONFLOW: 6.��ʾ�����������ӣ��������ܹ����ӳɹ�
//@EXECUTIONFLOW: 7.��ʾ����IP���ã���ping AP�������ܹ�pingͨ
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
            CSTCPrint("��ʹ�ô������߹��ܵ��豸����ɨ�裬��ȷ���ܹ�ɨ�赽coshipTestAp0\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 �������ź�ʧ�ܣ��޷�ɨ�赽SSID\n");
                break;
            }

            TEST_AP_PRINT_SSIDINFO(paSSIDName, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSSID.szPassWord);
            
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 �޷����ӵ�%s\n", paSSIDName);
                break;
            }
            
            TEST_AP_PRINT_PING(bRet)  
        }while(0);
         

       CSTK_FATAL_POINT
        
        return bRet;

}


//@CASEGROUP:1��CSAPSetFreqChannel(char *szApName,CSAPFreqChannel_E enChannel)
//@DESCRIPTION: ���Բ���Ϊ�Ƿ����������
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��szApName = CSUDI_NULL
//@INPUT: 2��enChannel = EM_AP_Channel_Auto+1
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.����CSAPSetFreqChannel��szApName����Ϊ�գ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.����CSAPSetFreqChannel��enChannel����EM_AP_Channel_Auto+1���������ط�CSUDI_SUCCESS
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


//@CASEGROUP:1��CSAPSetFreqChannel(char *pszApName,CSAPFreqChannel_E enChannel)
//@DESCRIPTION: ��������AP�ŵ��Ĺ���
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2���ܹ�ɨ������AP���ն�
//@INPUT: 1��pszApName
//@INPUT: 2��enChannel ΪCSAPFreqChannel_E��ö��ֵ
//@EXPECTATION: ����������ȷ���ŵ�,���ܹ����ӳɹ�
//@EXECUTIONFLOW: 1.����CSUDIAPSetSignal��bEnable����CSUDI_TRUE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.����CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������0
//@EXECUTIONFLOW: 3.����CSUDIAPSetSSIDs�����õ�һ��SSIDΪcoshipTestAp0����ʹ�����²����������ã���������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0
//@EXECUTIONFLOW: 4.����CSAPSetFreqChannel������EM_AP_CHANNEL_1_2412M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5.ʹ�������ն��豸����ɨ�裬�����ܹ�ɨ�赽coshipTestAp0
//@EXECUTIONFLOW: 6.ʹ�������ն��豸�������� coshipTestAp0, �������ӳɹ�
//@EXECUTIONFLOW: 7.��ʾ���������ն��豸IP�������ܹ�pingͨ AP
//@EXECUTIONFLOW: 8.�ظ�����3-4�����δ���CSAPFreqChannel_E�е�ö��ֵ��ֱ�������ŵ�������ͨ��
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
            CSTCPrint("��ʹ�ô������߹��ܵ��豸����ɨ�裬��ȷ���ܹ�ɨ�赽%s\n", paSSIDName);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 �������ź�ʧ�ܣ��޷�ɨ�赽%s, channel: %d\n", paSSIDName, nChannelIndex);
                break;
            }

            TEST_AP_PRINT_SSIDINFO(paSSIDName, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSSID.szPassWord);
            
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 �޷����ӵ�%s,�ŵ�:%d\n", paSSIDName, nChannelIndex);
                break;
            }
            
            TEST_AP_PRINT_PING(bRet)
        }


      CSTK_FATAL_POINT

	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDSeparate(const char* szApName, CSUDI_BOOL bEnable)
//@DESCRIPTION: ���ԷǷ����������
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��szApName = CSUDI_NULL
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.����CSUDIAPSetSSIDSeparate��szApName����CSUDI_NULL���������ط�CSUDI_SUCCESS
CSUDI_BOOL CSTC_AP_SetSSIDSeparate_0001(void)
{
    	CSTK_ASSERT_TRUE_FATAL(			
         (CSUDI_SUCCESS != CSUDIAPSetSSIDSeparate(CSUDI_NULL, CSUDI_TRUE)),			
            "Step 1 Set Ap separate should be Failure !!!"		
         );	
        
       CSTK_FATAL_POINT
        
	return CSUDI_TRUE;

}


//@CASEGROUP:1��CSUDIAPSetSSIDSeparate(const char* szApName, CSUDI_BOOL bEnable)
//@DESCRIPTION: ���Թر�AP���룬ͨ����ͬSSID�����station�ܹ���ͨ����AP���룬ͨ����ͬSSID�����station���ܹ���ͨ����ͬSSID�����station�ܹ���ͨ
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2��2̨����һ�����ն��豸
//@PRECONDITION: 3������IPʱ������ͬһ����
//@INPUT: 1��szApName 
//@EXPECTATION: �����ر�AP���룬ͨ����ͬSSID�����station�ܹ���ͨ����AP���룬ͨ����ͬSSID�����station���ܹ���ͨ����ͬSSID�����station�ܹ���ͨ
//@EXECUTIONFLOW: 1.����CSUDIAPSetSignal��bEnable����CSUDI_TRUE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.����CSUDIAPSetSSIDSeparate��bEnable����CSUDI_FALSE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3.����CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������������ڵ���2
//@EXECUTIONFLOW: 4.����CSUDIAPSetFreqChannel������AP���ŵ����������óɹ�
//@EXECUTIONFLOW: 5.����CSUDIAPSetSSIDs�����õ�һ��SSID����ΪcoshipTestAp0,���������µ�����,�������سɹ�
//@EXECUTIONFLOW: ����coshipTestAp0������ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 6.����CSUDIAPSetSSIDs�����õڶ���SSID����ΪcoshipTestAp1,���������µ�����,�������سɹ�
//@EXECUTIONFLOW: ����coshipTestAp1������ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="aaaabbbbcc", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 7.��ʾ�ֱ�����̨�����ն��豸����coshipTestAp0,coshipTestAp1�����ӣ��������ӳɹ�
//@EXECUTIONFLOW: 8.��ʾ���ô���̨�����ն��豸��IP��ַ��������ping���ԣ������ܹ�pingͨ
//@EXECUTIONFLOW: 9.����CSUDIAPSetSSIDSeparate��bEnable����CSUDI_TRUE����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 10.��ʾʹ�ô���̨�����ն��豸����ping���ԣ��������ܹ�pingͨ
//@EXECUTIONFLOW: 11.��ʾʹһ̨�豸�Ͽ�coshipTestAp1������coshipTestAp0,�������ӳɹ�
//@EXECUTIONFLOW: 12.��ʾ���ô��豸��IP�������豸����ping���ԣ������ܹ�pingͨ
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
            CSTCPrint("��ʹ�ô������߹��ܵ��豸����ɨ�裬��ȷ���ܹ�ɨ�赽%s, %s\n", saSSID[0].szSSID, saSSID[1].szSSID);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 7 �������ź�ʧ�ܣ��޷�ɨ�赽%s ��%s\n", saSSID[0].szSSID, saSSID[1].szSSID);
                break;
            }

            CSTCPrint("����һ̨�����ն��豸����%s", saSSID[0].szSSID);
            TEST_AP_PRINT_SSIDINFO(saSSID[0].szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, saSSID[0].szPassWord);
            CSTCPrint("������һ̨�����ն��豸����%s", saSSID[1].szSSID);
            TEST_AP_PRINT_SSIDINFO(saSSID[1].szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, saSSID[1].szPassWord);
                    
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 8 �޷����ӵ�%s �� %s\n", saSSID[0].szSSID, saSSID[1].szSSID);
                break;
            }
            
            CSTCPrint("���ӳɹ�^^,�����̨�����ն�����Ϊ�˶��ڵ�IP��%s--%s,\n", 
                g_sApDhcpAdd.m_pDhcpBeginAdd, g_sApDhcpAdd.m_pDhcpEndAdd, g_scApAddr);
            
            CSTCPrint("���ú�IP֮����ʹ��һ̨ping��һ̨�����ն��豸, ��ȷ���ܷ�ping ͨ\n");

            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 9  �ر�AP���룬��ͬSSID֮���޷�ping ͨAP\n");
                break;
            }     


            CSTK_ASSERT_TRUE_FATAL(			
                (CSUDI_SUCCESS == CSUDIAPSetSSIDSeparate(g_scApName, CSUDI_TRUE)),			
                "Step 10 Set Ap Signal should be Success !!!"		
            );          

            CSTCPrint("��AP���룬��ʹ��һ̨ping��һ̨�����ն��豸, ��ȷ�ϲ��ܹ�ping ͨ\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 11   Failed ��AP���룬��ͬSSID֮���ܹ�ping ͨAP\n");
                break;
            }     

            CSTCPrint("��Ͽ���%s�����ӣ������ӵ�%s,�������ܹ����ӳɹ�\n", saSSID[1].szSSID, saSSID[0].szSSID);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 12   Failed �ڶ�̨�����ն��豸�޷����ӵ�%s\n", saSSID[0].szSSID);
                break;
            }                 

            CSTCPrint("��ʹ��һ̨ping��һ̨�����ն��豸, ��ȷ���ܹ�ping ͨ\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 13   Failed ��AP���룬��ͬSSID֮���޷�pingͨ\n");
                break;
            }               

    }while(0);

      CSTK_FATAL_POINT

	return bRet;
}


//@CASEGROUP:1��CSUDIAPGetConfig(const char* szApName,CSUDIAPConfig_S* pstOutConfig)
//@DESCRIPTION: ���ԷǷ����������
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��szApName
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.����CSUDIAPGetConfig��szApName����CSUDI_NULL,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.����CSUDIAPGetConfig��pstOutConfig����CSUDI_NULL,�������ط�CSUDI_SUCCESS
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


//@CASEGROUP:1��CSUDIAPGetConfig(const char* szApName,CSUDIAPConfig_S* pstOutConfig)
//@DESCRIPTION: �����ܹ���ȷ��ȡAP������Ϣ
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��szApName 
//@EXPECTATION: ���������úõ�AP����
//@EXECUTIONFLOW: 1.����CSUDIAPSetSignal���������ź�,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.����CSUDIAPSetFreqChannel�����������ŵ�ΪEM_AP_CHANNEL_1_2412M,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3.����CSUDIAPSetSSIDSeparate���ر�AP���룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4.����CSUDIAPGetConfig����ȡAP���ã���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5.�Ƚ�AP������Ϣ��������Ԥ������һ��
//@EXECUTIONFLOW: 6.����CSUDIAPSetFreqChannel�����������ŵ�ΪEM_AP_CHANNEL_6_2437M,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7.����CSUDIAPSetSSIDSeparate�� ��AP���룬��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 8.����CSUDIAPGetConfig����ȡAP���ã���������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9.�Ƚ�AP������Ϣ��������Ԥ������һ��
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


//@CASEGROUP:1��CSAPGetSSIDCapbility(char *szApName,int *pnMaxCount)
//@DESCRIPTION: ���Բ���Ϊ�Ƿ�ֵ�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��pszApName=CSUDI_NULL
//@INPUT:  2��pnMaxCount=CSUDI_NULL
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSAPGetSSIDCapbility��szApName����CSUDI_NULL��pnMaxCount����Ϸ�ֵ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSAPGetSSIDCapbility��szApName��pnMaxCount����CSUDI_NULL���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSAPGetSSIDCapbility��szApName, pnMaxCount, ����Ƿ�ֵ�������ط�CSUDI_SUCCESS
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


//@CASEGROUP:1��CSAPGetSSIDCapbility(char *szApName,int *pnMaxCount)
//@DESCRIPTION: ����AP֧�ֵ����SSID����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��pszApName
//@EXPECTATION: ������ȡ����SSID�������ڵ���1
//@EXECUTIONFLOW: 1������CSAPGetSSIDCapbility��szApName����ra0,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2���ȽϷ��ص����SSID����,�������ڵ���1
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


//@CASEGROUP:1��CSUDIAPGetSSIDs(const char* szApName,CSUDIAPSSID_S* psOutSsids,int* pnCount)
//@DESCRIPTION: ���ԷǷ����������
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��szApName = CSUDI_NULL
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1.����CSUDIAPGetSSIDs��szApName����CSUDI_NULL,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 2.����CSUDIAPGetSSIDs��psOutSsids����CSUDI_NULL,�������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3.����CSUDIAPGetSSIDs��pnCount����CSUDI_NULL,�������ط�CSUDI_SUCCESS
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


//@CASEGROUP:1��CSUDIAPGetSSIDs(const char* szApName,CSUDIAPSSID_S* psOutSsids,int* pnCount)
//@DESCRIPTION: ���Ի�ȡ�û����õ�����SSID
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��szApName = ra0
//@EXPECTATION: ������ȡ�����õ�����SSID
//@EXECUTIONFLOW: 1.����CSUDIAPSetSignal���������źţ��������سɹ�
//@EXECUTIONFLOW: 2.����CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������سɹ�
//@EXECUTIONFLOW: 3.����CSUDIAPSetSSIDs����������SSID����������ΪcoshipTestApi��iΪ0 1 2......pnMaxCount-1����ʹ�ܣ��������سɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 4.����CSUDIAPGetSSIDs��������֤���е�SSID���ƺ�ʹ��״̬�Լ����ò���������������ƥ��
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
                CSTCPrint("��ȡ����Ϣ�����õĲ���\n");
                break;
            }
        }
        
        CSTK_FATAL_POINT
            
	return bRet;

}


//@CASEGROUP:1��CSUDIAPGetSSIDs(const char* szApName,CSUDIAPSSID_S* psOutSsids,int* pnCount)
//@DESCRIPTION: ���Ի�ȡSSIDʱ�������Count��ʵ�����õ�����С�����
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@INPUT: 1��szApName 
//@EXPECTATION: �������������ܻ�ȡ��nCount����SSID��Ϣ
//@EXECUTIONFLOW: 1.����CSUDIAPSetSignal���������źţ��������سɹ�
//@EXECUTIONFLOW: 2.����CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������سɹ�
//@EXECUTIONFLOW: 3.����CSUDIAPSetSSIDs����������SSID����������ΪcoshipTestApi��iΪ0 1 2......pnMaxCount-1����ʹ�ܣ��������سɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 4.����CSUDIAPGetSSIDs�������nCount����ΪpnMaxCount-1�������ܻ�ȡ��nCount��SSID����Ϣ������֤ͨ��
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
                CSTCPrint("��ȡ����Ϣ�����õĲ���\n");
                break;
            }
        }
        
        CSTK_FATAL_POINT
            
	return bRet;

}


//@CASEGROUP:1��CSUDIAPSetSSIDs(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount)
//@DESCRIPTION: ���Բ���Ϊ�Ƿ�ֵ�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPSetSSIDs,szApName����ΪCSUDI_NULL���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,psSsids����ΪCSUDI_NULL���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIAPSetSSIDs,��һ��SSID��ʹ�ܣ��������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIAPSetSSIDs,SSID��ֵΪCSUDI_NULL���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIAPSetSSIDs, eWifiMode����EM_AP_WIFI_MODE_11GN+1���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSUDIAPSetSSIDs, eAuthMode����EM_AP_AUTH_WPAPSK_WPA2PSK+1���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIAPSetSSIDs, eEncrypType����EM_AP_ENCRYP_NONE+1���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 10������CSUDIAPSetSSIDs, eAuthModeΪEM_AP_AUTH_WPA2PSK, eEncrypTypeΪEM_AP_ENCRYP_AES, szPassWord����CSUDI_NULL���������ط�CSUDI_SUCCESS
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


//@CASEGROUP:1��CSUDIAPSetSSIDs(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount)
//@DESCRIPTION: ����SSIDΪ�����ַ������
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��pszApName
//@EXPECTATION: ��������SSID�ɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������0
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs, ����SSID����Ϊ�����ַ�"!@#$%^&*&123"���������������CSUDI_SUCCESS
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

        CSTCPrint("��ɨ�貢ȷ���ܹ�ɨ�赽: %s������y�ܹ�ɨ�赽������n���ܹ�ɨ�赽\n", sSetSsids.szSSID);
        CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()), "Step 4 ɨ�費�������ַ���SSID!!!"		
        );	   

        
        CSTK_FATAL_POINT
        return CSUDI_TRUE;
        
}

//@CASEGROUP:1��CSUDIAPSetSSIDs(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount)
//@DESCRIPTION: �����Ƿ�����ȷ������SSID
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�������ն��豸
//@INPUT:  1��pszApName
//@INPUT:  2��SSID����Ϊcoship_testap%d����coship_testap0,coship_testap1.....
//@EXPECTATION: ��������SSID�ɹ�,���ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,������coshipTestAp0,coshipTestAp1.....����SSID����,����ʹ���������е�SSID
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ�������ն˽���ɨ�裬�����ܹ�ɨ�赽����SSID
//@EXECUTIONFLOW: 6������CSUDIAPSetSSIDs���رճ�coshipTestAp0���������SSID����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7����ʾ����ɨ�裬������coshipTestAp0���⣬����SSID���޷�ɨ�赽
//@EXECUTIONFLOW: 8������CSUDIAPSetSSIDs��ʹ������SSID����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 9����ʾ����ɨ�裬�����ܹ�ɨ�赽����SSID
//@EXECUTIONFLOW: 10����ʾ�û���������coshipTestAp1���������ӳɹ�
//@EXECUTIONFLOW: 11����ʾ ����IP��ַ��������ping AP���ԣ������ܹ�pingͨ
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
            CSTCPrint("��ʹ�ô������߹��ܵ��豸����ɨ�裬��ȷ���ܹ�ɨ�赽%s---%d\n", sSetSsids[0].szSSID, nMaxCount-1);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5  �������ź�ʧ�ܣ��޷�ɨ�赽����SSID\n");
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

            CSTCPrint("��ʹ�ô������߹��ܵ��豸����ɨ�裬��ȷ����%s---%d��ֻ��ɨ�赽%s\n", sSetSsids[0].szSSID, nMaxCount-1, sSetSsids[0].szSSID);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 7 �ر�SSIDʧ��\n");
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

            CSTCPrint("��ʹ�ô������߹��ܵ��豸����ɨ�裬��ȷ���ܹ�ɨ�赽%s---%d\n", sSetSsids[0].szSSID, nMaxCount-1);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 9 ʹ������SSIDʧ�ܣ��޷�ɨ�赽����SSID\n");
                break;
            }            


            TEST_AP_PRINT_SSIDINFO(sSetSsids[1].szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids[1].szPassWord)

            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 11 ����SSID:%sʧ��\n", sSetSsids[1].szSSID);
                break;
            }            

            TEST_AP_PRINT_PING(bRet)
        }while(0);


       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDs(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount)
//@DESCRIPTION: ʹ��SSID��ǿ�����ԣ������رպ�ʹ��1000�Σ������ܹ�ɨ�赽�������ӳɹ�
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�������ն��豸
//@INPUT:  1��pszApName
//@INPUT:  2��SSID����Ϊcoship_testap%d����coship_testap0,coship_testap1.....
//@EXPECTATION: ��������SSID�ɹ�,���ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,������coshipTestAp0,coshipTestAp1.....����SSID����,����ʹ�����е�SSID
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5������CSUDIAPSetSSIDs,�رպ�ʹ��coshipTestAp1����1000��
//@EXECUTIONFLOW: 6����ʾ����ɨ�裬�����ܹ�ɨ�赽����SSID
//@EXECUTIONFLOW: 7����ʾ�û���������coshipTestAp1���������ӳɹ�
//@EXECUTIONFLOW: 8����ʾ ����IP��ַ��������ping AP���ԣ������ܹ�pingͨ
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
            CSTCPrint("��ʹ�ô������߹��ܵ��豸����ɨ�裬��ȷ���ܹ�ɨ�赽%s---%d\n", sSetSsids[0].szSSID, nMaxCount-1);
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 7 �������ź�ʧ�ܣ��޷�ɨ�赽����SSID\n");
                break;
            }

            TEST_AP_PRINT_SSIDINFO(sSetSsids[1].szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids[1].szPassWord)

            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 8 ����SSID:%sʧ��\n", sSetSsids[1].szSSID);
                break;
            }            

            TEST_AP_PRINT_PING(bRet)
            
        }while(0);


       CSTK_FATAL_POINT
        
	return bRet;
}



//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������Ȩģʽ�ͼ��ܷ�ʽ��Ϻ󲻺Ϸ������
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid=coshipTestAp0
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 5������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_OPEN�����ܷ�ʽΪEM_AP_ENCRYP_TKIP���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_OPEN�����ܷ�ʽΪEM_AP_ENCRYP_AES���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_OPEN�����ܷ�ʽΪEM_AP_ENCRYP_TKIP_AES���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 8������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_SHARE�����ܷ�ʽΪEM_AP_ENCRYP_TKIP���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 9������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_SHARE�����ܷ�ʽΪEM_AP_ENCRYP_AES���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 10������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_SHARE�����ܷ�ʽΪEM_AP_ENCRYP_TKIP_AES���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 11������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_SHARE�����ܷ�ʽΪEM_AP_ENCRYP_NONE���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 12������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_AUTO�����ܷ�ʽΪEM_AP_ENCRYP_TKIP���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 13������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_AUTO�����ܷ�ʽΪEM_AP_ENCRYP_AES���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 14������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_AUTO�����ܷ�ʽΪEM_AP_ENCRYP_TKIP_AES���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 15������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_AUTO�����ܷ�ʽΪEM_AP_ENCRYP_NONE���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 16������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_WPAPSK�����ܷ�ʽΪEM_AP_ENCRYP_WEP���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 17������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_WPAPSK�����ܷ�ʽΪEM_AP_ENCRYP_NONE���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 18������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_WEP���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 19������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_NONE���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 20������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_WPAPSK_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_WEP���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 21������CSUDIAPSetSSIDs, ����֤��ʽΪ EM_AP_AUTH_WPAPSK_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_NONE���������ط�CSUDI_SUCCESS
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


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_OPEN�����ܷ�ʽΪEM_AP_ENCRYP_WEP�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_OPEN
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_WEP
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ʾ��������IP����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_SHARE�����ܷ�ʽΪEM_AP_ENCRYP_WEP�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_SHARE
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_WEP
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_SHARE,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ʾ��������IP����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}

//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_AUTO�����ܷ�ʽΪEM_AP_ENCRYP_WEP�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_AUTO
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_WEP
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_AUTO,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord ="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ʾ��������IP����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_OPEN�����ܷ�ʽΪEM_AP_ENCRYP_NONE�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_OPEN
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_NONE
//@INPUT:  5��szPassWord=CSUDI_NULL
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_NONE, uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_WPAPSK�����ܷ�ʽΪEM_AP_ENCRYP_TKIP�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_WPAPSK
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_TKIP
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK,eEncrypType = EM_AP_ENCRYP_TKIP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_WPAPSK�����ܷ�ʽΪEM_AP_ENCRYP_AES�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_WPAPSK
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_AES
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK,eEncrypType = EM_AP_ENCRYP_AES, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_TKIP�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_WPA2PSK
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_TKIP
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK,eEncrypType = EM_AP_ENCRYP_TKIP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_AES�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_WPA2PSK
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_AES
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK,eEncrypType = EM_AP_ENCRYP_AES, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_TKIP_AES�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_WPA2PSK
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_TKIP_AES
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK,eEncrypType = EM_AP_ENCRYP_TKIP_AES, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_WPAPSK_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_TKIP�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_WPAPSK_WPA2PSK
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_TKIP
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK,eEncrypType = EM_AP_ENCRYP_TKIP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_WPAPSK_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_AES�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_WPAPSK_WPA2PSK
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_AES
//@INPUT:  5��szPassWord="1234567890123"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK,eEncrypType = EM_AP_ENCRYP_AES, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN����֤��ʽΪEM_AP_AUTH_WPAPSK_WPA2PSK�����ܷ�ʽΪEM_AP_ENCRYP_TKIP_AES�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode=EM_AP_WIFI_MODE_11BGN
//@INPUT:  3��eAuthMode=EM_AP_AUTH_WPAPSK_WPA2PSK
//@INPUT:  4��eEncrypType=EM_AP_ENCRYP_TKIP_AES
//@INPUT:  5��szPassWord="ababababGJLYZ"
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡ֧�ֵ����SSID��Ŀ����������������1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����ʹ�����������������,��������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK,eEncrypType = EM_AP_ENCRYP_TKIP_AES, szPassWord="ababababGJLYZ", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 6����ʾ����ping AP�������ܹ�pingͨ
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
                CSTCPrint("step 5 ����SSID:%sʧ��\n", sSetSsids.szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN������indexΪ0, 1, 2��SSID�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@PRECONDITION: AP֧�ֵ�SSID�������ڵ���2
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode
//@INPUT:  3��eAuthMode
//@INPUT:  4��eEncrypType
//@INPUT:  5��szPassWord
//@EXPECTATION: �������óɹ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������ش��ڵ���2
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�1��SSID����ΪcoshipTestAp0���������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN,eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5������CSUDIAPSetSSIDs,���õ�2��SSID����ΪcoshipTestAp1���������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK, eEncrypType = EM_AP_ENCRYP_AES, szPassWord="1234aaaaa1234", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 6������CSUDIAPSetSSIDs,���õ�3��SSID����ΪcoshipTestAp2���������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPAPSK_WPA2PSK, eEncrypType = EM_AP_ENCRYP_AES, szPassWord="aaaa12345cccc", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 7����ʾ��������ɨ�裬������SSIDΪcoshipTestAp0,coshipTestAp1���������ܹ����ӳɹ�,��pingͨ 
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
                CSTCPrint("step 7 ����SSID:%sʧ��\n", sSetSsids[0].szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)

            TEST_AP_PRINT_SSIDINFO(sSetSsids[1].szSSID, EM_AP_AUTH_WPA2PSK, EM_AP_ENCRYP_AES, sSetSsids[1].szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 8 ����SSID:%sʧ��\n", sSetSsids[1].szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)
        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}



//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN������SSID�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@PRECONDITION: AP֧�ֵ�SSID�������ڵ���2
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode
//@INPUT:  3��eAuthMode
//@INPUT:  4��eEncrypType
//@INPUT:  5��szPassWord
//@EXPECTATION: �����޼���SSID���������������ӳɹ�����Ҫ����SSID��Ҫ�����������ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������ش��ڵ���2
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�1��SSIDΪcoshipTestAp0, �������²�����������,�������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_NONE, uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5������CSUDIAPSetSSIDs,���õ�2��SSIDΪcoshipTestAp1, �������²�����������,�������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK, eEncrypType = EM_AP_ENCRYP_AES, szPassWord="abcdefghijk", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 6����ʾ��������ɨ�裬�����ܹ�ɨ�赽coshipTestAp0��coshipTestAp1
//@EXECUTIONFLOW: 7����ʾ����  coshipTestAp0,������������������ܹ����ӳɹ�
//@EXECUTIONFLOW: 8����ʾ��������IP����ping AP,�����ܹ�pingͨAP
//@EXECUTIONFLOW: 9����ʾ����  coshipTestAp1,���������������룬�������ܹ����ӳɹ�
//@EXECUTIONFLOW: 10����ʾ����  coshipTestAp1,��ʾ�������룬�����ܹ����ӳɹ�
//@EXECUTIONFLOW: 11����ʾ��������IP����ping AP,�����ܹ�pingͨAP
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
            CSTCPrint("��ɨ������AP�������ܹ�ɨ�赽%s, %s\n", sSetSsids[0].szSSID, sSetSsids[1].szSSID);
            CSTCPrint("��ȷ����ɨ�赽y:�ܹ�;n:����ɨ�赽n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 failed  ɨ�費������AP\n");
                break;
            }

            
            CSTCPrint("������SSID:%s\n", sSetSsids[0].szSSID);
            AP_iAuthMode2Str(EM_AP_AUTH_OPEN);
            AP_iEncrypType2Str(EM_AP_ENCRYP_NONE);
            CSTCPrint("��ȷ���Ƿ��ܹ����ӳɹ�y:�ܹ�����,n:���ܹ�����\n"); 
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 7 ����SSID:%sʧ��\n", sSetSsids[0].szSSID);
                break;
            }
            
            TEST_AP_PRINT_PING(bRet)
                
            CSTCPrint("�벻��������������SSID:%s,�������Ӳ��ɹ�\n", sSetSsids[1].szSSID);
            CSTCPrint("��ȷ�ϲ���������y:���ܹ�����;n:��������\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 8 failed  ���Ӽ���SSID:%s Ӧ�ò��ɹ�\n", sSetSsids[1].szSSID);
                break;
            }
            
            TEST_AP_PRINT_SSIDINFO(sSetSsids[1].szSSID, EM_AP_AUTH_WPA2PSK, EM_AP_ENCRYP_AES, sSetSsids[1].szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 9 ����SSID:%sʧ��\n", sSetSsids[1].szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)
        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}


//@CASEGROUP:1��CSUDIAPSetSSIDConfig(const char* szSsid,const CSUDIAPSSIDConfig_S* psConfig)
//@DESCRIPTION: ������������ģʽΪEM_AP_WIFI_MODE_11BGN��������������������
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid
//@INPUT:  2��eWifiMode
//@INPUT:  3��eAuthMode
//@INPUT:  4��eEncrypType
//@INPUT:  5��szPassWord
//@EXPECTATION: ��������SSID��������������Ӳ��ɹ�����ȷ�������ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������ش��ڵ���1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�2��SSIDΪcoshipTestAp1���������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_WPA2PSK, eEncrypType = EM_AP_ENCRYP_AES, szPassWord="abcdefghijk", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾ��������ɨ�裬�����ܹ�ɨ�赽coshipTestAp0
//@EXECUTIONFLOW: 6����ʾ����  coshipTestAp0,��������������룬���Ӳ��ɹ�
//@EXECUTIONFLOW: 7����ʾ����  coshipTestAp0,����������ȷ���룬���ӳɹ�
//@EXECUTIONFLOW: 8����ʾ��������IP����ping AP,�����ܹ�pingͨAP
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
            CSTCPrint("��ɨ������AP�������ܹ�ɨ�赽%s\n", sSetSsids[0].szSSID);
            CSTCPrint("��ȷ����ɨ�赽y:�ܹ�;n:����ɨ�赽n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 failed  ɨ�費������AP\n");
                break;
            }
            
            CSTCPrint("�����������������SSID:%s,��ȷ����Ϊ:%s\n", sSetSsids[0].szSSID, sSetSsids[0].szPassWord);
            CSTCPrint("��ȷ���Ƿ��ܹ�����y:���ܹ�����;n:��������\n");
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 5 failed  ��������������Ӽ���SSID:%s Ӧ�ò��ɹ�\n", sSetSsids[0].szSSID);
                break;
            }

            
            TEST_AP_PRINT_SSIDINFO(sSetSsids[0].szSSID, EM_AP_AUTH_WPA2PSK, EM_AP_ENCRYP_AES, sSetSsids[0].szPassWord)
            if(!CSTKWaitYes())
            {
                bRet = CSUDI_FALSE;
                CSTCPrint("step 6 ����SSID:%sʧ��\n", sSetSsids[1].szSSID);
                break;
            }

            TEST_AP_PRINT_PING(bRet)
        }while(0);

       CSTK_FATAL_POINT
        
	return bRet;
}



//@CASEGROUP:CSUDIAPGetStationsInfo(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount)
//@DESCRIPTION: ���Բ���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szSsid=CSUDI_NULL
//@INPUT:  2��psOutArray=CSUDI_NULL
//@INPUT:  3��pnCount=CSUDI_NULL
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP���������������ص�SSID��������0
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs������SSID���������óɹ�
//@EXECUTIONFLOW: 5������CSUDIAPGetStationsInfo��szSsid����CSUDI_NULL���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 6������CSUDIAPGetStationsInfo��psOutArray����CSUDI_NULL���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 7������CSUDIAPGetStationsInfo��pnCount����CSUDI_NULL���������ط�CSUDI_SUCCESS
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
//@DESCRIPTION: �����ܹ���ȷ��ȡWIFI�ն˵���Ϣ
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��psOutArray
//@INPUT:  2��pnCount
//@EXPECTATION: ��������WIFI�ն���Ϣ
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������ش��ڵ���1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipTestAp0���������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾʹ��WIFI�ն˽������ӣ��������ӳɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetStationsInfo����ȡWIFI�ն���Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7�������ȡ������Ϣ����ʾ����ȷ���Ƿ���ȷ
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
            (CSTKWaitYes()),"step 5 ����SSIDʧ��!!!"		
        );        

       CSTCPrint("���ӳɹ�^^,��������ն�����Ϊ�˶��ڵ�IP��%s--%s,\n",
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
       CSTCPrint("��ȷ��Mac: %02X:%02X:%02X:%02X:%02X:%02X ", sAPStationInfo.aucMac[0], 
	   	sAPStationInfo.aucMac[1], sAPStationInfo.aucMac[2], sAPStationInfo.aucMac[3], 
	   	sAPStationInfo.aucMac[4], sAPStationInfo.aucMac[5]);
       CSTCPrint("�Ƿ��WIFI�ն�һ��,����yһ�£�����n��һ��:\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 8 ��ȡWIFI�ͻ�����Ϣʧ��!!!"		
        );        
       
       CSTK_FATAL_POINT

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIAPGetStationsInfo(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount)
//@DESCRIPTION: ���Ի�ȡ�ͻ�����Ϣʱ���������СС�ڿͻ������������
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@PRECONDITION: ��̨����̨���������ն��豸
//@INPUT:  1��psOutArray
//@INPUT:  2��pnCount
//@EXPECTATION: ��������������ȷ����WIFI�ն���Ϣ
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������ش��ڵ���1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipTestAp0���������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾʹ����̨WIFI�ն˽������ӣ��������ӳɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetStationsInfo��pnCount������ֵ1����ȡWIFI�ն���Ϣ����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 7�������ȡ������Ϣ����ʾ����ȷ���Ƿ���ȷ
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


       CSTCPrint("��ʹ����̨�豸ͬʱ��������SSID\n");
       TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 5 ����SSIDʧ��!!!"		
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

       CSTCPrint("��ȷ��Mac: %02X:%02X:%02X:%02X:%02X:%02X ", sAPStationInfo.aucMac[0], 
	   	sAPStationInfo.aucMac[1], sAPStationInfo.aucMac[2], sAPStationInfo.aucMac[3], 
	   	sAPStationInfo.aucMac[4], sAPStationInfo.aucMac[5]);
       CSTCPrint("�Ƿ������һ��WIFI�ն�һ��,����yһ�£�����n��һ��:\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 8 ��ȡWIFI�ͻ�����Ϣʧ��!!!"		
        );        
       
       CSTK_FATAL_POINT

	return CSUDI_TRUE;
}



//@CASEGROUP:CSUDIAPGetStationsInfo(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount)
//@DESCRIPTION: ���Խ�����������֮���ܹ���ȡWIFI�ն���Ϣ
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��psOutArray
//@INPUT:  2��pnCount
//@EXPECTATION: �����Ͽ�֮���ܹ���ȡWIFI�ն���Ϣ
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������ش��ڵ���1
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipTestAp0���������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾʹ��WIFI�ն˽������ӣ��������ӳɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetStationsInfo����ȡWIFI�ն���Ϣ����������ֵ����Ϊ�գ�����ģʽΪEM_AP_WIFI_MODE_11BGN
//@EXECUTIONFLOW: 7����ʾ�Ͽ�WIFI�ն�����
//@EXECUTIONFLOW: 8������CSUDIAPGetStationsInfo����ȡWIFI�ն���Ϣ���������ܹ���ȡWIFI�ն���Ϣ
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
            (CSTKWaitYes()),"step 5 ����SSIDʧ��!!!"		
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

       CSTCPrint("��ȷ��Mac: %02X:%02X:%02X:%02X:%02X:%02X ", sAPStationInfo[0].aucMac[0], 
	   	sAPStationInfo[0].aucMac[1], sAPStationInfo[0].aucMac[2], sAPStationInfo[0].aucMac[3], 
	   	sAPStationInfo[0].aucMac[4], sAPStationInfo[0].aucMac[5]);
       CSTCPrint("�Ƿ������һ��WIFI�ն�һ��,����yһ�£�����n��һ��:\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 7��ȡWIFI�ͻ�����Ϣʧ��!!!"		
        );       


        CSTCPrint("�������������\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 8 ���������Ҫ�Ͽ�WIFI����!!!"		
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
//@DESCRIPTION: ���Բ�ʹ��SSID֮�󣬲��ܹ���ȡ�ͻ�����Ϣ
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��psOutArray
//@INPUT:  2��pnCount
//@EXPECTATION: �����ر�SSIDʹ��֮�󣬲��ܹ���ȡ�ͻ�����Ϣ
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID�������������ش��ڵ���2
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õڶ���SSIDΪcoshipTestAp1���������óɹ�
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5����ʾʹ��WIFI�ն˽������ӣ��������ӳɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetStationsInfo����ȡWIFI�ն���Ϣ����������ֵ����Ϊ�գ�����ģʽΪEM_AP_WIFI_MODE_11BGN
//@EXECUTIONFLOW: 7������CSUDIAPSetSSIDs���رմ�SSID��ʹ�ܣ������رճɹ�
//@EXECUTIONFLOW: 8������CSUDIAPGetStationsInfo����ȡWIFI�ն���Ϣ���������ܹ���ȡWIFI�ն���Ϣ
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
            (CSTKWaitYes()),"step 5 ����SSIDʧ��!!!"		
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
       
       CSTCPrint("��ȷ��Mac: %02X:%02X:%02X:%02X:%02X:%02X ", sAPStationInfo[0].aucMac[0], 
	   	sAPStationInfo[0].aucMac[1], sAPStationInfo[0].aucMac[2], sAPStationInfo[0].aucMac[3], 
	   	sAPStationInfo[0].aucMac[4], sAPStationInfo[0].aucMac[5]);
       CSTCPrint("�Ƿ������һ��WIFI�ն�һ��,����yһ�£�����n��һ��:\n");
       CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 6 ��ȡWIFI�ͻ�����Ϣʧ��!!!"		
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
//@DESCRIPTION: ���Բ���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��szApName=CSUDI_NULL
//@INPUT:  2��psInfo=CSUDI_NULL
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSAPSetAdvancedInfo��AP����szApName����CSUDI_NULL,psInfo����Ϸ�ֵ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSAPSetAdvancedInfo��AP����szApName����Ϸ�ֵ,psInfo����CSUDI_NULL���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSAPSetAdvancedInfo��AP����szApName����CSUDI_NULL,psInfo����CSUDI_NULL���������ط�CSUDI_SUCCESS
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
//@DESCRIPTION: ���Բ���Ϊ�Ƿ�ֵCSUDI_NULL�����
//@PRECONDITION: ƽ̨�ɹ���ʼ��
//@INPUT:  1��pszApName=CSUDI_NULL
//@INPUT:  2��pstAdvancedInfo=CSUDI_NULL
//@EXPECTATION: �������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 1������CSAPGetAdvancedInfo��AP����szApName����CSUDI_NULL,psOutInfo����Ϸ�ֵ���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSAPGetAdvancedInfo��AP����szApName����Ϸ�ֵ,psOutInfo����CSUDI_NULL���������ط�CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSAPGetAdvancedInfo��AP����szApName����CSUDI_NULL,psOutInfo����CSUDI_NULL���������ط�CSUDI_SUCCESS
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


//@CASEGROUP:1��CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2��CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: ���Ըı�BGģʽ֮��WIFI�ն��Ƿ��ܹ����ӳɹ�
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�������ն��豸
//@INPUT:  1��m_enBGMode
//@EXPECTATION: �����ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������0
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5������CSAPSetAdvancedInfo������BGģʽΪEM_AP_BGPROTECT_MODE_AUTO���������óɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetAdvancedInfo����ȡBGģʽ�������бȽϣ�������������һ��
//@EXECUTIONFLOW: 7����ʾ�����������ӣ������ܹ����ӳɹ�,������Ping���ԣ������ܹ�pingͨAP
//@EXECUTIONFLOW: 8���ظ�����5-7����������BGģʽΪEM_AP_BGPROTECT_MODE_OPEN��EM_AP_BGPROTECT_MODE_CLOSE������ÿһ�����ɹ�
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
                (sSetInfo.eBGMode == sGetInfo.eBGMode),"step 7 ����SSIDʧ��!!!"		
            );                

           TEST_AP_PRINT_SSIDINFO(sSetSsids.szSSID, EM_AP_AUTH_OPEN, EM_AP_ENCRYP_WEP, sSetSsids.szPassWord)        
           CSTK_ASSERT_TRUE_FATAL(			
                (CSTKWaitYes()),"step 8 ����SSIDʧ��!!!"		
            );        

           TEST_AP_PRINT_PING(bRet)
        }
       
       CSTK_FATAL_POINT

	return bRet;
}



//@CASEGROUP:1��CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2��CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: ���Ըı䷢�书��֮��WIFI�ն��Ƿ��ܹ����ӳɹ�
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�����������������豸
//@INPUT:  1��nFreqPower
//@EXPECTATION: ���书�ʼ�С���ź�ǿ�ȼ���
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������0
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5������CSAPSetAdvancedInfo�����÷��书��Ϊ100���������óɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetAdvancedInfo����ȡ���书�ʣ�����֤��������֮ǰ����һ��
//@EXECUTIONFLOW: 7����ʾ�����������ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 8����ʾ���б����ź�ǿ�ȣ���Ping Ap�������ܹ�pingͨ
//@EXECUTIONFLOW: 9���ظ�����5-8�����÷��书��Ϊ76����ÿһ�����ɹ�
//@EXECUTIONFLOW: 10���ظ�����5-8�����÷��书��Ϊ52����ÿһ�����ɹ�
//@EXECUTIONFLOW: 11���ظ�����5-8�����÷��书��Ϊ28����ÿһ�����ɹ�
//@EXECUTIONFLOW: 12���ظ�����5-8�����÷��书��Ϊ4�����޷�ɨ�赽coshipTestAp0��SSID
//@EXECUTIONFLOW: 13�������ź�ǿ�ȣ����������书�ʼ���ʱ���ź�ǿ����Ӧ����
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
                (CSTKWaitYes()),"step 7 ����SSIDʧ��!!!"		
            );        

          CSTCPrint("��ǰ���书��Ϊ%d�����¼�ź�ǿ��,������Ҫ����\n", sSetInfo.nFreqPower);

           TEST_AP_PRINT_PING(bRet)

        }

        CSTCPrint("���书�����δ�100 -76-52-4,��ȷ���ź�ǿ�����μ���\n");
        CSTK_ASSERT_TRUE_FATAL(			
            (CSTKWaitYes()),"step 8 ��֤�ź�ǿ��ʧ��!!!"		
        );            
        
       CSTK_FATAL_POINT

	return bRet;
}



//@CASEGROUP:1��CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2��CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: ���Ըı��ű���֮��WIFI�ն��Ƿ��ܹ����ӳɹ�
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�����������������豸
//@INPUT:  1��nBeaconInterval
//@EXPECTATION: �����ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������0
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5������CSAPSetAdvancedInfo�������ű���Ϊ999���������óɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetAdvancedInfo����ȡ�ű��������Ƚϣ�����������ʱһ��
//@EXECUTIONFLOW: 7����ʾ�����������ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 8����ʾ����ping���ԣ������ܹ�pingͨAP
//@EXECUTIONFLOW: 9���ظ�����5-8�������ű���Ϊ510������ÿһ�����ɹ�
//@EXECUTIONFLOW: 10���ظ�����5-8�������ű���Ϊ21������ÿһ�����ɹ�
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
                (CSTKWaitYes()),"step 8 ����SSIDʧ��!!!"		
            );        
           TEST_AP_PRINT_PING(bRet)

        }
        
       CSTK_FATAL_POINT

	return bRet;
}


//@CASEGROUP:1��CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2��CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: ���Ըı�ָ����֮��WIFI�ն��Ƿ��ܹ����ӳɹ�
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�����������������豸
//@INPUT:  1��nDivideLimit
//@EXPECTATION: �����ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������0
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5������CSAPSetAdvancedInfo�����÷ָ����Ϊ2347���������óɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetAdvancedInfo����ȡ�ָ���ޣ����Ƚϣ�������ȡ������ʱ һ��
//@EXECUTIONFLOW: 7����ʾ�����������ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 8����ʾPing���ԣ������ܹ�PingͨAP
//@EXECUTIONFLOW: 9���ظ�����5-8�����÷ָ����Ϊ1174������ÿһ�����ɹ�
//@EXECUTIONFLOW: 10���ظ�����5-8�����÷ָ����Ϊ0������ÿһ�����ɹ�
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
                (CSTKWaitYes()),"step 8 ����SSIDʧ��!!!"		
            );        
           TEST_AP_PRINT_PING(bRet)

        }
        
       CSTK_FATAL_POINT

	return bRet;
}



//@CASEGROUP:1��CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2��CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: ���Ըı䴫��ָ����֮��WIFI�ն��Ƿ��ܹ����ӳɹ�
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�����������������豸
//@INPUT:  1��nTranferLimit
//@EXPECTATION: �����ı䴫��ָ����֮���ܹ����ӳɹ�
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������0
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5������CSAPSetAdvancedInfo�����ô���ָ����Ϊ2347���������óɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetAdvancedInfo����ȡ�ָ���ޣ����Ƚϣ�������ȡ������ʱ һ��
//@EXECUTIONFLOW: 7����ʾ�����������ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 8����ʾPing���ԣ������ܹ�PingͨAP
//@EXECUTIONFLOW: 9���ظ�����5-8�����÷ָ����Ϊ1174������ÿһ�����ɹ�
//@EXECUTIONFLOW: 10���ظ�����5-8�����÷ָ����Ϊ1������ÿһ�����ɹ�
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
                (CSTKWaitYes()),"step 8 ����SSIDʧ��!!!"		
            );        
           TEST_AP_PRINT_PING(bRet)

        }
        
       CSTK_FATAL_POINT

	return bRet;
}



//@CASEGROUP:1��CSAPSetAdvancedInfo(char *pszApName,CSAPAdvancedInfo_S *pstAdvancedInfo)
//@CASEGROUP:2��CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo)
//@DESCRIPTION: �����Ƿ��ܸı�AP�ĸ߼�������ǰ���룬����ײ�ۣ�����ͻ�������ݰ��ۺ�
//@PRECONDITION: 1��ƽ̨�ɹ���ʼ��
//@PRECONDITION: 2�����������������豸
//@INPUT:  1��bShortStartCode
//@INPUT:  2��bShortClashSlot
//@INPUT:  3��bTransportBurst
//@INPUT:  4��bDataPacketGrab
//@EXPECTATION: �ܹ��ɹ�����AP�߼�����
//@EXECUTIONFLOW: 1������CSUDIAPSetSignal��ʹ��AP����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 2������CSUDIAPSetFreqChannel������AP�ŵ�ΪEM_AP_CHANNEL_6_2437M����������CSUDI_SUCCESS
//@EXECUTIONFLOW: 3������CSUDIAPGetSSIDCapbility����ȡAP֧�ֵ����SSID��������������0
//@EXECUTIONFLOW: 4������CSUDIAPSetSSIDs,���õ�һ��SSIDΪcoshipApTest0����������CSUDI_SUCCESS
//@EXECUTIONFLOW: ���ò���ΪeWifiMode=EM_AP_WIFI_MODE_11BGN, eAuthMode = EM_AP_AUTH_OPEN, eEncrypType = EM_AP_ENCRYP_WEP, szPassWord="1234567890123", uUpdateTime =0���������óɹ�
//@EXECUTIONFLOW: 5������CSAPSetAdvancedInfo���������в�����ΪCSUDI_TRUE���������óɹ�
//@EXECUTIONFLOW: 6������CSUDIAPGetAdvancedInfo����ȡ�����ò������������óɹ�
//@EXECUTIONFLOW: 7���Ƚ����õĸ߼�����������ȫΪCSUDI_TRUE
//@EXECUTIONFLOW: 8����ʾ�����������ӣ������ܹ����ӳɹ�
//@EXECUTIONFLOW: 9������CSAPSetAdvancedInfo���������в�����ΪCSUDI_FALSE���������óɹ�
//@EXECUTIONFLOW: 10������CSUDIAPGetAdvancedInfo����ȡ�����ò������������óɹ�
//@EXECUTIONFLOW: 11���Ƚ����õĸ߼�����������ȫΪCSUDI_FALSE
//@EXECUTIONFLOW: 12����ʾ�����������ӣ������ܹ����ӳɹ�
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
                    CSTCPrint("step 8 ����SSIDʧ��!!!\n");	
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
                    CSTCPrint("step 12 ����SSIDʧ��!!!\n");	
            }
           
           TEST_AP_PRINT_PING(bRet)
        }while(0);
        
       CSTK_FATAL_POINT

	return bRet;
}





