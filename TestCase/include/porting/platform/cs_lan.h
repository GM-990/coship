/********************************************************************************

* cs_lan.h																		*

* Copyright ? Shenzhen Coship Electronics Co.#,LTD.  2005.10

* All rights reserved.															*

*																				*

* Author: wenrongchang															*

* Date:  2005/10/26																*
	
* Compiler:																		*

*																				*

* Description:	Coship Lan														*

* Notes:																		*

*********************************************************************************/

#ifndef _CS_LAN_H_
#define _CS_LAN_H_


#include "mmcp_typedef.h"
#include "cs_lan_plus.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------
**                       宏定义                                          **
-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
**                      有线网卡                                         **
-------------------------------------------------------------------------*/
#define  MAX_LAN_CHIP_NUMBER  1          /*On STB Support Lan chip max Number  */


/* get link status 
the "speed bit" is no meaning when the link is down*/
#define CS_ETHIF_LINK_DOWN        (unsigned short)0x1U   /* link up: 0, link down: 1 */
#define CS_ETHIF_LINK_UP          (unsigned short)0x0U   /* link up: 0, link down: 1 */
#define CS_ETHIF_LINK_10BASE      (unsigned short)0x2U   /* 100BASE: 0, 10BASE: 1 */
#define CS_ETHIF_LINK_HALF        (unsigned short)0x4U   /* FULL: 0, HALF: 1, valid only for SMSC9115 */
#define CS_ETHIF_LINK_100BASE     (unsigned short)0x0U   /* 100BASE: 0, 10BASE: 1 */


/*-------------------------------------------------------------------------
**                     无线网卡                                          **
-------------------------------------------------------------------------*/
#define CSWLAN_ESSID_MAX_LEN 			32/*无线网卡认证的ESSID字符串的长度*/
#define CSWLAN_ENCKEY_MAX_LEN 		64/*无线网卡认证的密钥的字符串的长度*/
#define CSWLAN_NETIFNAME_MAX_LEN 	8/*无线网卡名称字符串的长度*/
#define CSWLAN_WPA_MSG_LEN 			128/*WPA认证程序消息的长度*/
#define CSWLAN_APNAME_MSX_LEN 		32/*AP ESSID 的长度*/


/*-------------------------------------------------------------------------
**                       结构体，枚举                                    **
-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
**                      有线网卡                                         **
-------------------------------------------------------------------------*/
typedef struct
{
    unsigned int      nLanManuFactory;   /* manufacturer ID                   */
    unsigned int	  nLanType ;	     /*The specific type of a chip Vendor.*/
}CS_LANINFO;



/*-------------------------------------------------------------------------
**                     无线网卡                                          **
-------------------------------------------------------------------------*/
typedef enum /*无线网卡认证的加密类型*/
{
	EM_WLAN_ENC_NONE = 0,
	EM_WLAN_ENC_WEPOPENAUTO,
	EM_WLAN_ENC_WEPSHARE,
	EM_WLAN_ENC_WPA,
	EM_WLAN_ENC_WPA2,
	EM_WLAN_ENC_WPA_TKIP,
	EM_WLAN_ENC_WPA_AES,
	EM_WLAN_ENC_WPA2_TKIP,
	EM_WLAN_ENC_WPA2_AES,
	EM_WLAN_ENC_UNKNOW
} CSWLanEncType_E;


typedef enum     /*无线网卡密钥的数据类型*/   
{	
    EM_WLAN_KEY_HEX = 0, //16进制
    EM_WLAN_KEY_ANSI,    //ASCII码
    EM_WLAN_KEY_STRING   //还不知道这个有什么用，先保留。
 } CSWLanKeyType_E;

typedef enum /*无线网卡的网络连接状态*/
{
    EM_WLAN_STATUS_UP = 0,
    EM_WLAN_STATUS_DOWN
}CSWLanStatus_E;


typedef enum /*AP的WPA认证的状态*/
{
    EM_WLAN_WPA_STATUS_UP = 0,
    EM_WLAN_WPA_STATUS_DOWN 
}CSWLanWPAStatus_E;



typedef enum/*WPA认证程序返回的消息的数据类型*/
{
    EM_WLAN_WPA_PID = 1,
    EM_WLAN_WPA_INFO
}CSWLanWPAMsgType_E;

typedef struct /*WPA认证程序返回的消息体的数据结构*/
{
    int nType;
    char acMsg[CSWLAN_WPA_MSG_LEN];
}CSWLanWPAInfoForMsg_S;


typedef struct _CSWLanConfig_S/*无线网卡的配置信息*/
{
    	char acNetIfName[CSWLAN_NETIFNAME_MAX_LEN+4];/*无线网卡的名称*/
	char acEssid[CSWLAN_ESSID_MAX_LEN+4];/*认证的ESSID*/
	char acKey[CSWLAN_ENCKEY_MAX_LEN+4];/*认证的密钥*/
	CSWLanEncType_E 	eEncType;/*认证的类型*/
	CSWLanKeyType_E 	eKeyType;/*认证的密钥的数据类型*/
}CSWLanConfig_S;


typedef struct _CSWLanApInfo_S/*无线AP的信息*/
{
    	char acApName[CSWLAN_APNAME_MSX_LEN];/*AP ESSID*/
	int nSignalStrength;/*AP 信号强度*/
	int nLinkQuality;/*AP连接质量*/
	CSWLanEncType_E eApEncType;/*AP的加密方式*/
}CSWLanApInfo_S;

typedef enum CSIP_WLAN_RET_Code_e
{
    CSIP_RET_WLAN_IFUP_ERROR = 2700,
    CSIP_RET_WLAN_IFDOWN_ERROR,
    CSIP_RET_WLAN_CONNECT_AP_ERROR,
    CSIP_RET_WLAN_SET_KEY_ERROR,
    CSIP_RET_WLAN_CLEAN_KEY_ERROR,
    CSIP_RET_WLAN_GETINFO_ERROR,
    CSIP_RET_WLAN_DISCONNECT_ERROR,
    CSIP_RET_WLAN_SET_WPA_ERROR
}CSIP_WLAN_RET_Code;



/*-------------------------------------------------------------------------
**                       函数申明                                        **
-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
**                      有线网卡                                         **
-------------------------------------------------------------------------*/
/****************************************************************************  
*   Function:	Get Configed Lan  Chip Count										    
*   Input:	None															    
*   Output:	None															    
*   Return:	The total number of  Lan Chip Used									    
****************************************************************************/ 
int  CSLanGetCount(void);


/****************************************************************************  
*   Function:	Get Configed Lan Chip's Information about chip manufacture&type 		    
*   Input:	nChipIndex:may be 0 or other ( if there are muti piece of chips)				    
*   Output:	tChipInfo:	ManuFactory,manufacturer ID;						    
*   Return:	SUCCESS for successfully get and FAILURE for get failed.	    
****************************************************************************/  
int CSLanGetChipInfo (char * pcEthName, CS_LANINFO* psLanInfo);


/****************************************************************************  
*   Function:	Get Lan Mac Address to chip 		    
*   Input:	 nChipIndex:may be 0 or other ( if there are muti piece of chips)	
*   Output:	 pbMacAddress:is Mac Address						    
*   Return:	 0 for successfully get and -1 for get failed.	    
****************************************************************************/ 
int  CSLanGetMacAddress(int nChipIndex,unsigned char* pucMacAddress);


/****************************************************************************  
*   Function:	Get Lan Mac Address by device name 		    
*   Input:	 pchEthName:device name,eg:eth0	
*   Output:	 pbMacAddress:is Mac Address						    
*   Return:	 0 for successfully get and -1 for get failed.	    
****************************************************************************/ 
int  CSLanGetMacByName(char * pchEthName,unsigned char *pucMacAddress);


/***********************************************************
* Function: CSLanGetVersion
*
* Input:  nSize: The size of Version Buffer.
* Output:  pchVer: The top address of Version Buffer
* Return: 
*   1>If [pchVer] is NULL,return the actual length of
*     version information,including '\0'.
*   2>If [pchVer] is not NULL,and the call is success,
*     return the length of version information returned.
*   3>If the call is failure,return 0.   
*   
* Note: This function is used to get version information.
*  Display format is:
*  "[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
************************************************************/
int CSLanGetVersion(char* pcVer, int nSize);


/****************************************************************************  
*   Function:	CSLanGetLinkStatus() 
*               reads the current link settings of PHY 		    
*   Input:	 nChipIndex:may be 0 or other ( if there are muti piece of chips)
*			 pcEthName: lan device name
*   Output:	 pwLinkState:link status of lan 						    
*   Return:	 0 for successfully get else for get failed.	    
****************************************************************************/ 
int CSLanGetLinkStatus(int nChipIndex, char* pcEthName, unsigned short* pwLinkState);

/****************************************************************************  
*   Function:	CSLan_GetLinkUpDown() 
*               reads the current link settings of PHY 		    
*   Input:	 nChipIndex:may be 0 or other ( if there are muti piece of chips)
*			 pcEthName: lan device name
*   Output:	 pwLinkState:link status of lan 						    
*   Return:	 0 for successfully get and -1 for get failed.	    
* only fit for smsc9115,check link up or down 

****************************************************************************/ 

int CSLan_GetLinkUpDown(int nChipIndex, 
								char* pcEthName, 
								unsigned short* pwLinkState);

/****************************************************************************  
*   Function:CSLan_OpenNetif 
*            This function allocates and initializes driver private
*	         structures, Lan9115 hardware, platform specific timing.
*	         It identifies Lan911x variation and attaches interrupt.
*	         you can change the lan work mode
*   Input:	 pcEthName: lan device name
*			 ucMode: lan work mode
*			 ucMode  3    2    1   0
*                                   		  0 ---  auto_negotiating 
*                                   		  1 ---  force negotiation
*                              		    0 --- full duplex
*                                            1 --- half duplex
*                                        0 --- 100M
*                                        1 --- 10M
*   Output:	 						    
*   Return:	 0 for successfully get and else for get failed.	    
*   Note   :      only fit for smsc9115
****************************************************************************/ 
int CSLan_OpenNetif(char* pcIfname, unsigned char ucMode);


/****************************************************************************  
*   Function:CSLan_CloseNetif 
*            This function shutdown the NIC,
*             
*   Input:	 pcEthName: lan device name
*			
*   Output:	 						    
*   Return:	 0 for successfully get and else for get failed.	    
*   Note   :      only fit for smsc9115
****************************************************************************/ 
int CSLan_CloseNetif(char* pcIfname);


/****************************************************************************  
*   Function:CSLan_ReopenNetif 
*            This function First, shutdown the NIC,
*            Then, change the lan work mode and raise the NIC 
*            If the applications want to change the work mode of NIC while 
*            running, this function will be used. 
*   Input:	 pcEthName: lan device name
*			 ucMode: lan work mode
*   Output:	 					    
*   Return:	 0 for successfully get and else for get failed.	    
*   Note   :      only fit for smsc9115
****************************************************************************/
int CSLan_ReopenNetif(char* pcIfname, unsigned char ucMode);


/*增加网卡收发包个数的统计*/
/***************************************************************************  
*   Function:CSLanGetSendPacketsNum 
*            This function get the num of send packets
*   Input:	 
*   Output:						    
*   Return:	 the num of send packets.	    
****************************************************************************/ 
unsigned long CSLanGetSendPacketsNum(void);


/****************************************************************************  
*   Function:CSLanGetReceivePacketsNum 
*            This function get the num of received packets
*   Input:	 
*   Output:						    
*   Return:	 the num of received packets.	    
****************************************************************************/ 
unsigned long CSLanGetReceivePacketsNum(void);

/****************************************************************************  
*   Function:CSLAN_EnableRerceiveBroadcast 
*            This function enable NIC receive the broadcast packets
*   Input:	 bEnablebroadcast
*   Output:	none				    
*   Return:	 none	    
****************************************************************************/ 
void CSLAN_EnableReceiveBroadcast(BOOL bEnablebroadcast);


/****************************************************************************  
*   Function:CSLAN_IsEnableRerceiveBroadcast 
*            This function get the status if  NIC receive the broadcast packets
*   Input:	 none
*   Output:	none				    
*   Return:	 the status	    
****************************************************************************/ 
BOOL CSLAN_IsEnableReceiveBroadcast(void);


/******************************************************************************
Function Name:    CSLanUp
Description:
        ifconfig ethx up.
Input:
    pcNetIfname:the name of the Lan
Output:
    NONE
Return Value:0 for success and -1 for FAILURE	
*******************************************************************************/
int CSLanUp(char *pchEthName);

 /******************************************************************************
Function Name:    CSLanDown
Description:
        ifconfig ethx down.
Input:
    pcNetIfname:the name of the Lan
Output:
    NONE
Return Value:0 for success and -1 for FAILURE	
*******************************************************************************/
int CSLanDown(char *pchEthName);


/*-------------------------------------------------------------------------
**                     无线网卡                                          **
-------------------------------------------------------------------------*/

/******************************************************************************
Function Name:    CSWLanConnectAP
Description:
    Connect to AP.There are three mode to connect AP:None,WEP,WPA.Use one of them to config the setting.
Input:
    psWLanConf: the config of WLAN
Output:
    None
Return Value:
    EM_WLAN_INVALPARA              
    EM_WLAN_CONNECTAP_ERROR       
    EM_WLAN_CLEANKEY_ERROR  
    EM_WLAN_SETKEY_ERROR    
    EM_WLAN_SUCCESS            
*******************************************************************************/
CSWLanRet_E CSWLanConnectAP(CSWLanConfig_S* psWLanConf);

/******************************************************************************
Function Name:    CSWLanDisconnectAP
Description:
    reinit the global variable,if open the wpa_supplication thred ,should kill the thread.
Input:
    pcNetIfname: the name of WLAN
Output:
    None
Return Value:
    EM_WLAN_INVALPARA                
    EM_WLAN_FAIL   
    EM_WLAN_SUCCESS                 
*******************************************************************************/
CSWLanRet_E CSWLanDisconnectAP(char *pcNetIfname);

/******************************************************************************
Function Name:    CSWLanGetStatus
Description:
    Get the WLAN's status in up or down.
Input:
    pcNetIfname: the name of WLAN
Output:
    peWlanStatus:
    EM_WLAN_STATUS_DOWN  :wlan down
    EM_WLAN_STATUS_UP       :wlan up
Return Value:
    EM_WLAN_INVALPARA          
    EM_WLAN_FAIL 
    EM_WLAN_SUCCESS         
*******************************************************************************/
CSWLanRet_E CSWLanGetStatus(char *pcNetIfname,CSWLanStatus_E*peWlanStatus);

/******************************************************************************
Function Name:    CSWLanGetWPAStatus
Description:
    Get the WPA's status,startup or stop.
Input:
    NONE
Output:
    peWPAStatus:
    EM_WLAN_WPA_STATUS_UP
    EM_WLAN_WPA_STATUS_DOWN
Return Value:
    EM_WLAN_INVALPARA                   
    EM_WLAN_SUCCESS                    
*******************************************************************************/
CSWLanRet_E CSWLanGetWPAStatus(CSWLanWPAStatus_E*peWPAStatus);

/******************************************************************************
Function Name:    CSWLanGetAPEssid
Description:
        Execute the iwlist to return the AP's name.
Input:
    pcNetIfname:the name of the WLan
    pcBuffer: storage the AP's name buffer
    nLength: tthe buffer's length
Output:
    pBuffer:the AP's name separated by '$'
Return Value:
    EM_WLAN_INVALPARA 
    EM_WLAN_FAIL 
    EM_WLAN_NOTGETAPNAME_ERROR
    EM_WLAN_SUCCESS 
*******************************************************************************/
CSWLanRet_E CSWLanGetAPEssid(char *pcNetIfname,char *pcBuffer, int nLength);

/******************************************************************************
Function Name:    CSWLanGetAPInfo
Description:
        Execute the iwlist to return the AP's info.
Input:
    pcNetIfname:the name of the WLan
    psWlanApInfo: storage the AP's info array
    pnWlanApInfoNum: the array's length
Output:
    pnWlanApInfoNum:the actual AP's info number
Return Value:
    EM_WLAN_INVALPARA 
    EM_WLAN_FAIL 
    EM_WLAN_GETINFO_ERROR
    EM_WLAN_SUCCESS 
*******************************************************************************/
CSWLanRet_E CSWLanGetAPInfo(char *pcNetIfname,CSWLanApInfo_S *psWlanApInfo[], int* pnWlanApInfoNum);

/******************************************************************************
Function Name:    CSWLanGetApSigQulityStrength

Description:
        return the AP's signal qulity and strength.

Input:
	NONE
Output:
    nWlanApSigQulity:the actual AP's signal qulity
    nWlanApSigStrength:the actual AP's signal strength

Return Value:
    EM_WLAN_INVALPARA 
    EM_WLAN_SUCCESS 

Other:
*******************************************************************************/
CSWLanRet_E CSWLanGetApSigQulityStrength(int *pnWlanApSigQulity,int *pnWlanApSigStrength);

#ifdef __cplusplus
}
#endif

#endif

