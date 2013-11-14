/**@defgroup IPCFG IPCFG ��ģ��������������Э��ջ��ؽӿں��������ݶ��塣
@brief ��������ģ���Ӧͷ�ļ�<udi2_ipcfg.h>��������ȡ���ߡ�������������Ϣ������IP���� �Ȳ�������MMCP��CloudTV ���м��ƽ̨�Ȳ��ֹ��ܼ�Щ�Ĳ�Ʒ�У�MMCP��CloudTV ���м��ƽ̨����ʹ�û������粿�ֵĽӿڽ����������ӣ���ȡ���ݵȲ������ڹ��ܸ�ǿ���MMCP��CloudTV ���м��ƽ̨��Ʒ�У�MMCP��CloudTV ���м��ƽ̨��ʹ�øò��ֵĽӿڶ�����������ã�������IP��ַ����ȡ��������״̬�ȡ�

�ڱ�ģ��������������ͨ�����������ƽ���Ψһ��ʶ������Ҳͨ���������Ʋ������п��ơ����ֲ���ϵͳ��Linux֧��һ���������ö��IP����������¿ɽ������������������ȫ��������������eth0:1��eth0:2������������������������ȣ��������������ǵ�Mac��ַ�����ǵĲ��롢�γ�״̬��ȫһ����

��������������������ƽ̨�������������䳤�Ȳ��ó����ӿ��ĵ�����������󳤶ȡ�

UDI���ṩ��������MAC��ַ�Ľӿڣ��������оƬû������MAC��ַ���ն����ڽ���MMCP��CloudTV ���м��ƽ̨֮ǰ�������úã�����UDI�ӿڳ�ʼ����ʱ�򣬴�Flash�ж�ȡ����ʱ���õ����е�MAC��ַ�����������õ������С�

- ���ϵͳ��û�������������ڻ�ȡ����ʱ����Ҫ�ṩ�����������ɡ�
- ���ϵͳ��û���κ���������ģ���ȡ�����κ�������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.6 2012/11/23 ���ӿɰβ�������Ϣ�����ӿ�
@version 2.0.5 2009/08/31 ɾ��������������֤��Ϣ�ṹ��Ϣ
@version 2.0.4 2009/08/25 ����������������ע��ص��ӿ�
@version 2.0.3 2009/08/25 ɾ������Mac��ַ�ӿ�,��Ϊ�ýӿ�����������
@version 2.0.2 2009/08/24 �ۺ������İ汾
@version 0.0.1 2009/08/12 ��ʾ�汾
@{
*/
#ifndef _UDI2_IPCFG_H_
#define _UDI2_IPCFG_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"

#ifdef __cplusplus
extern "C" {
#endif


/**@brief ���ߺ������������ӶϿ�

@note bit [7] ... [3]... [2] [1] ...[0]
@note ...............................0 ..: link up
@note ...............................1 ..: link down  
*/
#define CSUDI_ETHMASK_UP_DOWN       		  	(unsigned short)0x1U   


/**@brief ���ߺ�����������������

@note bit [7] ... [3]... [2] [1] ...[0]
@note ................... 0   0 .......................: 100BASE
@note ................... 0   1 .......................: 10BASE
@note ................... 1   0 .......................: 54M��������
@note ................... 1   1 .......................: δ֪��������
*/
#define CSUDI_ETHMASK_LINKSPEED     			(unsigned short)0x6U   


/**@brief ���ߺ���������ȫ��˫������

@note bit [7] ... [3]... [2] [1] ...[0]
@note .............0 ................................: FULL
@note .............1 ................................: HALF
*/
#define CSUDI_ETHMASK_FULL_HALF     			((unsigned short)0x8U)   

#define CSUDI_ETHER_NETIFNAME_MAX_LEN   		(16)      			///< ���������ַ�������󳤶�
#define CSUDI_ETHER_IPADDR_MAX_LEN   		       (16)      			///< IP��ַ�ַ�������󳤶�
#define CSUDI_ETHER_DNS_MAX_SUPPORT   		(4)      			///< DNS֧�ֵ�������

#define CSUDI_MAX_LAN_CHIP_NUMBER             		(2)     			///< ������֧�ֵ����������  
#define CSUDI_WLAN_ESSID_MAX_LEN 				(32)    			///< ����������֤��ESSID�ַ����ĳ���
#define CSUDI_WLAN_ENCKEY_MAX_LEN 			(64)    			///< ����������֤����Կ���ַ����ĳ���
#define CSUDI_WLAN_WPA_MSG_LEN 				(128)  			///< WPA��֤������Ϣ�ĳ���
#define CSUDI_WLAN_APNAME_MSX_LEN 			(32)   			///< AP ESSID�ĳ���
#define CSUDI_WLAN_APMAC_MAX_LEN                     (32)                      ///< AP MAC�ĳ���


/**@brief IPCFGģ��������ֵ */
enum
{
   CSUDIIPCFG_ERROR_BAD_PARAMETER = CSUDI_IPCFG_ERROR_BASE, ///< ��������
   CSUIDIPCFG_ERROR_FEATURE_NOT_SUPPORTED,                  		///< ������֧��
   CSUDIIPCFG_ERROR_UNKNOWN_ERROR,							///< δ֪����
   CSUDIIPCFG_ERROR_NO_MEMORY,								///< ���ڴ���ô���
   CSUDIIPCFG_ERROR_NETIFUP_FAIL ,								///< ������������ʧ��
   CSUDIIPCFG_ERROR_NETIFDOWN_FAIL,							///< ���������ر�ʧ��
   CSUDIIPCFG_ERROR_CONNECTAP_ERROR,       						///< ���Ӵ���
   CSUDIIPCFG_ERROR_SETKEY_ERROR,              						///< ������Կ����	
   CSUDIIPCFG_ERROR_CLEANKEY_ERROR,         						///< �����֤��Ϣ����
   CSUDIIPCFG_ERROR_GETINFO_ERROR,							///< �����Ϣ����
   CSUDIIPCFG_ERROR_GETAPNAME_ERROR,	   						///< û�л�ȡ��AP����
   CSUDIIPCFG_ERROR_ALREADY_ADDED	                       			 ///< �ص������Ѿ�ע��
};

typedef enum
{
	EM_UDIRAWSENDTYPE_UNICAST = 0,//����
	EM_UDIRAWSENDTYPE_BROADCAST,//�㲥
	EM_UDIRAWSENDTYPE_UNDEFINED
} CSUDIRawSendType_E;

/**@brief ��������*/
typedef enum 
{
   EM_UDIETHER_TYPE_LAN = 0,  									///< ��������
   EM_UDIETHER_TYPE_WLAN,										///< ��������
   EM_UDIEHTER_TYPE_PPPOE,                                      //pppoe
   EM_UDIEHTER_TYPE_AP,										///����AP
   EM_UDIEHTER_TYPE_UNKNOWN                                       ///δ֪��������
}CSUDIEtherType_E;


/**@brief ����������֤�ļ�������*/
typedef enum 
{
   EM_UDIWLAN_ENC_NONE = 0,  									///< �޼���
   EM_UDIWLAN_ENC_WEPOPENAUTO,								///< WEPOpen����
   EM_UDIWLAN_ENC_WEPSHARE,									///< shared����ģʽ������WEP��Ϣ
   EM_UDIWLAN_ENC_WPAPSK_TKIP,								///<WPAPSK ��֤��ʽ��TKIP�����㷨
   EM_UDIWLAN_ENC_WPAPSK_AES,								///<WPAPSK ��֤��ʽ��AES�����㷨
   EM_UDIWLAN_ENC_WPA2PSK_TKIP,								///<WPA2PSK ��֤��ʽ��TKIP�����㷨
   EM_UDIWLAN_ENC_WPA2PSK_AES,								///<WPA2PSK ��֤��ʽ��AES�����㷨
   EM_UDIWLAN_ENC_UNKNOW
}CSUDIWLanEncType_E;


/**@brief ����������Կ����������*/
typedef enum     
{	
    EM_UDIWLAN_KEY_HEX = 0,   									///< 16����
    EM_UDIWLAN_KEY_ANSI,       									///< ASCII��
    EM_UDIWLAN_KEY_STRING    									///< ������
 }CSUDIWLanKeyType_E;


/**@brief AP��WPA��֤��״̬*/
typedef enum 
{
    EM_UDIWLAN_WPA_STATUS_UP = 0,								///< WPA��֤�ɹ�
    EM_UDIWLAN_WPA_STATUS_DOWN 								///< WPA��֤ʧ��
}CSUDIWLanWPAStatus_E;


/**@brief ��������WPA��֤���ص���Ϣ�Լ��ص��������ظ�Ӧ�õ���Ϣ����*/
typedef enum
{
    EM_UDIWLAN_WPA_DISCONNECTED = 0,  						///< WPA�Ͽ�����
    EM_UDIWLAN_WPA_INACTIVE,	 								///< WPA���ڷǼ���״̬
    EM_UDIWLAN_WPA_SCANNING, 									///< ɨ������
    EM_UDIWLAN_WPA_ASSOCIATING, 								///< ��ͼ����BSS/SSID
    EM_UDIWLAN_WPA_ASSOCIATED, 								///< �������
    EM_UDIWLAN_WPA_4WAY_HANDSHAKE, 							///< WPA4������Կ����
    EM_UDIWLAN_WPA_GROUP_HANDSHAKE, 						///< WPA������Կ����
    EM_UDIWLAN_WPA_COMPLETED, 								///< �������
    EM_UDIWLAN_WPA_SET_ERR,									///< ���ô���
    EM_UDIWLAN_WPA_KEY_ERR, 									///< ��Կ����
    EM_UDIWLAN_WPA_NO_FOUND_AP, 								///< δ�ҵ�AP
    EM_UDIWLAN_WPA_FINISH,									///< WPA���
    EM_UDIWLAN_WEP_STATE_UP,									///< ״̬����
    EM_UDIWLAN_WEP_STATE_ERROR,								///< ״̬����
    EM_UDIWLAN_NONE_ENC_UP,									///< ������
    EM_UDIWLAN_CONN_TIMEOUT,									///< ���ӳ�ʱ
    EM_UDIWLAN_DOWN,											///< ����Ͽ�
    EM_UDIWLAN_UP,												///< �������ӳɹ�
    EM_UDIWLAN_APLOST,											///< APȱʧ
    EM_UDIWLAN_APCONNECT										///< AP�ɹ�����
}CSUDIWLanMsgInfo_E;



/**@brief ����������������Ϣ*/
typedef struct 
{
    char szEthName[CSUDI_ETHER_NETIFNAME_MAX_LEN+4];		///< ��������������
    char szEssId[CSUDI_WLAN_ESSID_MAX_LEN+4];			///< ��֤��ESSID
    char szKey[CSUDI_WLAN_ENCKEY_MAX_LEN+4];				///< ��֤����Կ
    CSUDIWLanEncType_E 	eEncType;                     			///< ��֤�ļ�������
    CSUDIWLanKeyType_E 	eKeyType;                     			///< ��֤����Կ����������
}CSUDIWLanConfig_S;


/**@brief ����AP����Ϣ*/
typedef struct 
{
    char szApName[CSUDI_WLAN_APNAME_MSX_LEN]; 			///< AP ESSID
    int    nSignalStrength;                             ///< AP�ź�ǿ�ȣ�0��100��
    int    nLinkQuality;                                ///< AP����������0��100��
    CSUDIWLanEncType_E eApEncType;                      			///< AP�ļ��ܷ�ʽ
    char szApMac[CSUDI_WLAN_APMAC_MAX_LEN+4];                  ///< AP��MAC
}CSUDIWLanApInfo_S;

/**@brief ������оƬ��Ϣ*/
typedef struct
{
    unsigned int      		uLanManuFactory;   					///< ��������ID                   
    CSUDIEtherType_E    eEtherType ;	          				///< ��������
}CSUDIEtherInfo_S;

/**@brief ������ͳ����Ϣ*/
typedef struct
{
    unsigned int uConnectTime;          ///< ���ӳ���ʱ��
    unsigned int uTotalBytesSend;       ///< ���Ͱ��ֽ���
    unsigned int uTotalBytesReceived;   ///< ���հ��ֽ���
    unsigned int uTotalPacketSend;      ///< ���Ͱ�����
    unsigned int uTotalPacketReceived;  ///< �ӷ�������
}CSUDIEtherStat_S;

/**@brief ���������ص�����

@param[in] eMsgType ����������Ϣ���ͣ����CSUDIWLanMsgInfo_E����
@param[in] pcEthName ��������������
@param[in] pvUserData �û���������
*/
typedef void (*CSUDIEtherCallback_F)(CSUDIWLanMsgInfo_E eMsgType,char * pcEthName,void * pvUserData);


 /**
@brief ע�����������Ļص�����

@param[in] fnEtherCallback �ص�����ָ�룬���CSUDIEtherCallback_F����
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �ص�����������û�����Ψһȷ��һ���ص�������������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIIPCFG_ERROR_ALREADY_ADDED
@note ����֧��ע��Ļص���������Ϊ32���������
*/
CSUDI_Error_Code CSUDIIPCFGAddEtherCallback(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData);


 /**
@brief ɾ�����������Ļص�����

@param[in] fnEtherCallback �ص�����ָ�룬���CSUDIEtherCallback_F����
@param[in] pvUserData ͬCSUDIIPCFGAddEtherCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGDelEtherCallback(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData);


/**
@brief �����������֧�����ߺ�����������pppoe���������pppoe���ܣ�����Ӧ����ʵ�ʵ�����������ppp0
@param[out] pnCount ��ȡ����������Ŀ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/ 
CSUDI_Error_Code CSUDIIPCFGGetEtherCount(int* pnCount);


/**
@brief ��ȡ�����豸����֧�����ߺ�����������pppoe��

@param[in] nEthIndex �����豸�����ţ��� 0��1 ������
@param[in] nLen pcEthName��ָ�ռ�ĳ��ȣ��������0С�ڵ���16
@param[out] pcEthName ��ȡ���������豸�������û�����ռ䣬���ȱ������0С�ڵ���16
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherName(int nEthIndex,char * pcEthName,int nLen );


/**
@brief �������оƬ����Ϣ�����������̺����͵ȡ�

@param[in] pcEthName ������������:eth0
@param[out] psChipInfo ��CSUDIEtherInfo_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherChipInfo(char * pcEthName, CSUDIEtherInfo_S * psChipInfo);


/**
@brief ͨ���豸��������ȡ���������ַ��֧�����ߺ�����������

@param[in] pcEthName ������������:eth0
@param[out] pucMacAddress ָ��һ��MAC��ַ���ַ�������, ��ʽΪ{0x00, 0x0E, 0xDB, 0x11, 0x22, 0x33}��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 1.udi2û���ƶ�����mac��ַ�Ľӿڣ���Ϊ����mac��ַ��Ӧ����udi2���ĵ����飬��Ӧ����ϵͳ������ʱ��ִ�еĶ�����
@note ���仰˵��mac��ַ�ڵ��øýӿ�֮ǰ���Ѿ�ȷ���������ȡ��mac��ַ��Ҳ������ϵͳ����ʱ���ý��ı���һ�¡�
*/
CSUDI_Error_Code CSUDIIPCFGGetMacByName(char * pcEthName,unsigned char * pucMacAddress);


/**
@brief ��ȡ���ߺ���������������״̬��

@param[in] pcEthName ָ��һ�������������ַ�������"eth0"
@param[out] pushLinkState ����״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��������������״̬:
@note 1��pushLinkState & CSUDI_ETHMASK_UP_DOWN : Ϊ0�����Ѿ����ӣ�Ϊ1����δ���ӣ�
@note 2��pushLinkState & CSUDI_ETHMASK_100BASE_10BASE:Ϊ00����100Mbps��Ϊ01����10Mbps��Ϊ10��������54Mbps��Ϊ11����δ֪���ʣ�
@note 3��pushLinkState & CSUDI_ETHMASK_FULL_HALF: Ϊ0����ȫ˫����Ϊ1�����˫����
@note ��������������״̬:
@note 1��pushLinkState & CSUDI_ETHMASK_UP_DOWN : Ϊ0�����Ѿ����ӣ�Ϊ1����δ���ӡ�
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherStatus(char * pcEthName, unsigned short * pushLinkState);


/**
@brief ���������ڴ��ڵ�����AP����Ϣ��ֻ��������������

@param[in] pcEthName ��������������
@param[in] psWlanApInfo �洢AP��Ϣ�Ľṹ������
@param[in] pnWlanApInfoNum psWlanApInfo�ṹ������ĸ���
@param[out] psWlanApInfo �洢ʵ�ʻ�ȡ����AP��Ϣ�Ľṹ������
@param[out] pnWlanApInfoNum ʵ�ʻ�ȡ����AP��Ϣ����Ч���� 	
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGSearchAP(char * pcEthName,CSUDIWLanApInfo_S * psWlanApInfo, int * pnWlanApInfoNum);


/**
@brief ��AP�������Ӳ��ҽ�����֤��ֻ��������������

@param[in] psWLanConf �������������ò������ݽṹ�������������������ƣ���֤��ESSID�ַ�������֤����Կ���ַ����ͳ��ȣ���֤�����ͣ���Կ����������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGConnectAP(CSUDIWLanConfig_S * psWLanConf);


/**
@brief �Ͽ���AP�����ӣ�ֻ��������������

@param[in] pcEthName ��������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGDisconnectAP(char * pcEthName);


/**
@brief ��ȡAP�źŵ�������ǿ�ȡ�

@param[out] pnWlanApSigQulity AP�źŵ�����
@param[out] pnWlanApSigStrength AP�źŵ�ǿ��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �˽ӿڽ���������������ȡ��Ϣ��Ӧ����CSUDIIPCFGSearchAP ��ʱ�򱣴���Ϣ
*/
CSUDI_Error_Code CSUDIIPCFGGetAPSignalQulityStrength(int * pnWlanApSigQulity,int * pnWlanApSigStrength);


/**
@brief ����DNS�ĵ�ַ��

@param[in] nDNSIndex DNS�������ţ��� 0��1 �����֧��4��
@param[in] pcDNS DNS��IP��ַ����"192.168.99.5"�ĵ��ʮ����IP��ַ)
@param[in] nLen DNS��IP��ַ�ַ������ȣ���С��16 
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGSetDNSServer(int nDNSIndex,char * pcDNS,int nLen);


/**  
@brief ���DNS�ĵ�ַ��

@param[in] nDNSIndex DNS�������ţ��� 0��1�����֧��4��
@param[in] nLen DNS��IP��ַ�ַ������ȣ���С��16 
@param[out] pcDNS DNS��IP��ַ(��"192.168.99.5"�ĵ��ʮ����IP��ַ����pcDNS�Ŀռ����û�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGGetDNSServer(int nDNSIndex,char * pcDNS,int nLen);


/**
@brief ����������IP��ַ��������������ص�ַ��֧�����ߺ�����������

@param[in] pcEthName �����豸������:"eth0"����ΪCSUDI_NULL���򷵻ش���
@param[in] pcIp IP��ַ�ַ�������"192.168.1.1" ����ΪCSUDI_NULL���򷵻ش���    
@param[in] pcMask ���������ַ�������"255.255.255.0"������ΪCSUDI_NULL
@param[in] pcGateWay ���ص�ַ�ַ�������"192.168.1.254"������ΪCSUDI_NULL
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGSetIpInfo(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay);


/**
@brief ��ȡ������IP��ַ��������������ص�ַ��֧�����ߺ�����������

@param[in] pcEthName �����豸��
@param[out] pcIp ��ȡ����IP��ַ�����ȱ��벻С��16
@param[out] pcMask ��ȡ����mask��ַ�����ȱ��벻С��16
@param[out] pcGateWay ��ȡ����gateway��ַ�����ȱ��벻С��16
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note: 
         ������������ֻҪ��һ����Ϊ�գ��ͻ�ȡ��Ӧ��ֵ���ɹ�����CSUDI_SUCCESS 
         ���pcIp==pcMask==pcGateWay==CSUDI_NULL���᷵��CSUDIIPCFG_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIIPCFGGetIpInfo(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay);


/**
@brief ��Ӧ�ò㴫�����İ��������������������㷢�ͳ�ȥ��

@param[in] pszEthName �����豸����
@param[in] fd socket��������(*fd==-1��ʱ���ڴ˺����ڴ���socket,������sosket�޹�)
@param[in] pcDhcpBuf  Ӧ�ò�����/��̫��������
@param[in] nBufLen buffer�ĳ���
@param[in] nFlag �Ƿ���Ҫ�˺������udp/ip/ethernetͷ nFlag =1,��Ҫ��ӣ�nFlag =0,ֱ�ӷ���
@return �ɹ����� 0,ʧ�ܷ��� -1
*/
int  CSUDIRawSendPacket(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag);


/**
@brief ��Ӧ�ò㴫�����İ��������������������㷢�ͳ�ȥ,�����û����õĲ���eRawSendType�������͵İ��ǵ��������ǹ㲥����

@param[in] pszEthName �����豸����
@param[in] fd socket��������(*fd==-1��ʱ���ڴ˺����ڴ���socket,������sosket�޹�)
@param[in] pcDhcpBuf  Ӧ�ò�����/��̫��������
@param[in] nBufLen buffer�ĳ���
@param[in] nFlag �Ƿ���Ҫ�˺������udp/ip/ethernetͷ nFlag =1,��Ҫ��ӣ�nFlag =0,ֱ�ӷ���
@param[in] eRawSendType ���巢�Ͱ�ʱ�ǵ��������ǹ㲥
@return �ɹ����� 0,ʧ�ܷ��� -1
*/
int  CSUDIRawSendPacketByMode(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag, CSUDIRawSendType_E eRawSendMode);
/**
@brief ���մ����������������İ�������Ӧ�ò�

@param[in] pszEthName �����豸����
@param[in] fd socket��������(*fd==-1��ʱ���ڴ˺����ڴ���socket,������sosket�޹�)
@param[out] pchBuf  ��Ҫ���յ�����
@param[in,out] pnBufLen buffer�ĳ���.�������ʱΪpchBuf�Ļ��������ȣ��������ΪpchBuf����Ч���ݳ���
@param[in] nFlag  �Ƿ���Ҫ�˺���ȥ��udp/ip/ethernetͷ  nFlag =1,��Ҫȥ����nFlag =0,ֱ�ӽ�����һ��
@return �ɹ����� ʵ���յ����ֽ���,ʧ�ܷ��� -1
*/
int CSUDIRawRecvPacket (char *pszEthName, int  *fd, const char *pchBuf, int* pnBufLen, int nFlag);


/**
@brief ��ȡ����ͳ����Ϣ
@param[in] pcName �����豸����
@param[out] psNetStatus �洢ʵ�ʻ�ȡ��������ͳ����Ϣ�Ľṹ��
@return �ɹ�����CSUDI_SUCCESS ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGGetStatInfo(char *pcName,CSUDIEtherStat_S *psNetStatus);


/**
@brief ��������
@param[in] pcEthName �����豸����
@param[out] ��
@return �ɹ�����CSUDI_SUCCESS ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGNICUp(char *pcEthName);


/**
@brief ��������
@param[in] pcEthName �����豸����
@param[out] ��
@return �ɹ�����CSUDI_SUCCESS ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGNICDown(char *pcEthName);

/*-----------------���½ӿڶ���ɲ����������ؽӿ�---------------------*/
/**
@brief ��ӿɲ������֪ͨ�ص�����

���RDIģ��CSUDIRDIAddCallback_F�ӿڶ��壬�����ڴ������������͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,���豸״̬�����仯ʱ���øú���
@param[in] pvUserData �û�����,����Ϊ�ص�������pvUserData��������
@return �ɹ���ӷ���CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��ӻص�ǰ�������豸�仯��Ϣ,���Ჹ��֪ͨ.��������ʱ��Щ�豸�ʹ���,���ñ�����ʱ����õ�֪ͨ.
@note ��ƽ̨����Ҫ֧���κοɲ���������ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
@note ��CSUDIRDICallback_F�ص��У��ɲ�������豸��EM_UDIRDI_EVENT_READY��Ϣ�У�pEventDataΪethname (���������ַ���)�������ڱ�ͷ�ļ����涨���ʹ���������Ľӿ�
*/
CSUDI_Error_Code CSUDIIPCFGRdiAddCallback(CSUDIRDICallback_F fnCallback, void* pvUserData);

/**
@brief ɾ���ɲ������֪ͨ�ص�����

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ������������͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,֮ǰע��ĺ���
@param[in] pvUserData �û�����,������ע��ʱ��һ��
@return �ɹ�ɾ������CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note fnCallback��pUserDataһ��Ψһȷ��һ���ص�����
@note ��ƽ̨����Ҫ֧���κοɲ���������ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDIIPCFGRdiRemoveCallback(CSUDIRDICallback_F fnCallback, void* pvUserData);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

