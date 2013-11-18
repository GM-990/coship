/*************************************************************************
**************************************************************************
* Copyright (c) 2007,ShenZhen Coship Electronic Ltd Co.
* All rights reserved.
* 
* File Name:		Tcpip_setup.h
* File Identify:
* Summary:		Interface about TCP/IP Stack
* 
* Current Version��1.1
* Author(s)��Peng Dong, Huai Yonglong
* 
*--------------------------------------------------------------------------/
*revision    author            		reason                date
*1.1           Huai Yonglong           Add interface      Mar 15,2007
*1.0           Peng Dong                Original               August 11,2005
*----------------------------------------------------------------------------/
*/

#ifndef _TCPIP_SETUP_H
#define _TCPIP_SETUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "udi2_socket.h"

/*IP Address length*/
#define CS_TCPIP_ADDR_LEN (20)
/*url(only have ip and port) length*/
#define CS_TCPIP_IPURL (32)
/*string url length*/
#define CS_TCPIP_STRURL (512)

typedef enum
{
	TCPIP_NO_PRINT = 0X0,
	TCPIP_PRINT_ERROR = 0X1,
	TCPIP_PRINT_STACK = 0X2,
	TCPIP_PRINT_CM = 0X4,
	TCPIP_PRINT_ALL = 0X0FFFFFFF
}TCPIP_PRINT;

enum
{
	TCPIP_ERROR = 0X00000001,
	TCPIP_STACK = 0X00000002,
	TCPIP_CM = 0X00000004
};

typedef enum
{
	DHCP = 0X10,
	MANUAL_CONFIG
}IP_SET_MODE;

typedef enum
{
	GET_IP_ADDR = 0X20,
	ETH_CONNECT,
	ETH_DISCONNECT,
	CM_ON_LINE,
	CM_OFF_LINE
}NETWORK_STATUS;

typedef enum
{
	CM_ONLINE = 0X0,
	CM_OFFLINE,
	CM_ERR,
	CM_INTERRUPT,
	CM_OFF_ONLINE
}CM_STATUS;


/**@brief �źŵ�ָʾ
@note                                                                              
@note                                       ����        ����           ����       
@note CM_Init                               OFF         OFF           OFF       
@note CM_NOT_READY                          BLINK       OFF           OFF       
@note CM_NOT_LOCK                           NA          NA            BLINK    
@note CM_PHY_SYN                            ON          BLINK         OFF      
@note EM_CM_USPARA_ACQUIRED                 ON          BLINK         OFF       
@note EM_CM_RANGE_COMPLETE                  ON          ON            OFF       
@note EM_CM_IP_COMPLETE                     ON          ON            BLINK     
@note EM_CM_TOD_ESTAB                       ON          ON            BLINK     
@note EM_CM_SECURITY_ESTAB                  ON          ON            BLINK     
@note EM_CM_PARA_TS_COMPLETE                ON          ON            BLINK     
@note EM_CM_REG_COMPLETE                    ON          ON            BLINK     
@note EM_CM_OPERTIONAL                      ON          ON            ON        
@note EM_CM_ACCESS_DENIED                   BLINK       BLINK         OFF      
*/


/**@brief CM״ֵ̬ */                            
typedef enum
{
    EM_CM_FAIL=0,                      ///< cm״̬���󣬴�ά�ĺ����и�״̬����˾�ĺ���û�и�״̬
    EM_CM_INIT,                           ///< ��ʼ��״̬��
    EM_CM_NOT_READY,                     ///< δ����״̬��
    EM_CM_NOT_LOCK,                       ///< δ��Ƶ
    EM_CM_PHY_SYN,                        ///< ͬ������   
    EM_CM_USPARA_ACQUIRED,                ///< ��ȡ����ͨ������
    EM_CM_RANGE_COMPLETE,                 ///< ���в�����
    EM_CM_IP_COMPLETE,                    ///< ����ip����
    EM_CM_TOD_ESTAB,                      ///< ����todͬ��
    EM_CM_SECURITY_ESTAB,                 ///< ������ȫ����
    EM_CM_PARA_TS_COMPLETE,               ///< ���ò�����ȡ�ɹ�
    EM_CM_REG_COMPLETE,                   ///< cmע�����
    EM_CM_OPERTIONAL,                     ///< cm����
    EM_CM_ACCESS_DENIED                   ///< ��������
}CSCMStatus_E;


typedef void (*TcpipEventCallback)(NETWORK_STATUS event);
typedef void (*TcpipPrintCallback)(const char * const Format_p,...);


/**@brief ����modeȡֵ��ΧΪ CM_NETWORK_ONLINE �� CM_NETWORK_OFFLINE ���˻ص�������֪ͨ�û����������״���������������� */
typedef void (*FOnCMStatusCallback)(int nMode,DWORD dwUserData);    

typedef struct _ip_parameter_tag_
{
	DWORD m_dwFlashSign;
	DWORD m_dwIpSetMode;
	char m_caIPAddr[CS_TCPIP_ADDR_LEN];
	char m_caMask[CS_TCPIP_ADDR_LEN];
	char m_caGateWay[CS_TCPIP_ADDR_LEN];
	char m_caMainDNS[CS_TCPIP_ADDR_LEN];
	char m_caBackDNS[CS_TCPIP_ADDR_LEN];
	char m_caHttpServer[CS_TCPIP_ADDR_LEN];
	char m_caVodServer[CS_TCPIP_ADDR_LEN];
	char m_caExServer1[CS_TCPIP_ADDR_LEN];
	char m_caExServer2[CS_TCPIP_ADDR_LEN];
	char m_caExServer3[CS_TCPIP_ADDR_LEN];
	char m_caExServer4[CS_TCPIP_ADDR_LEN];
}IP_PARAMETER;

typedef struct _register_info_
{
	BOOL m_bHaveCM;
	BOOL m_bHaveSwitch;
	char * m_pcaClintClassID;
	char * m_pcaServerClassID;
    int  nOptChkMode;
	char * m_pcaServerHostName;
	char * m_pEthName;
	DWORD m_dwFlashAddr;
	int m_nPrintLevel;
	PBYTE m_pTcpIPStack;
	int   m_nTcpIPStackSize;
	TcpipEventCallback m_pEventCallback;
	TcpipPrintCallback m_pPrintCallback;
}REGISTER_INFO;

typedef struct _ip_info_
{
	DWORD m_dwIpMode;
	char *m_pcIpAddr;
	char *m_pcMask;
	char *m_pcGateWay;
	char* m_pcMainDNS;
	char *m_pcBackDNS;
}IP_INFO;


/*********************************UI developer use*****************************/
/*==========================================================
����ԭ��:int CSTcpipRegistInfo(REGISTER_INFO stbInfo);
�������:ע�� �û�ѡ�����ָ��
�������:��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :����TCPIP��װ�ӿ�ǰע���û���Ϣ
ע������:Ӧ�ڱ�ģ�����нӿ�ǰ����
=========================================================== */
int CSTcpipRegistInfo(REGISTER_INFO stbInfo);


/*==========================================================
����ԭ��:int CSTcpIpInit(void);
�������:��
�������:��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :��ʼ��TCPIPЭ��ջ
ע������:Ӧ������Ӳ����ʼ����Ϻ���ã����Ҫ����
 	dhcp��renew�����ͷ�ʽΪ�������ǹ㲥�������ڵ��ô˺�
 	��ǰ(������dhcpǰ)����cs_dhcpc.h�ļ������CSDHCPSetRenewUnicastMode
 	�ӿ�,�������ô˽ӿڣ���Ĭ��renew���ķ��ͷ�ʽΪ�㲥
=========================================================== */
int CSTcpIpInit(void);

/*==========================================================
����ԭ��:int CSTcpIpInitExtra(IP_INFO sipinfo);
�������:IP_INFO sipinfo
�������:��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :��ʼ��TCPIPЭ��ջ
ע������:Ӧ������Ӳ����ʼ����Ϻ����,���Ҫ����
 	dhcp��renew�����ͷ�ʽΪ�������ǹ㲥�������ڵ��ô˺�
 	��ǰ(������dhcpǰ)����cs_dhcpc.h�ļ������CSDHCPSetRenewUnicastMode
 	�ӿ�,�������ô˽ӿڣ���Ĭ��renew���ķ��ͷ�ʽΪ�㲥
=========================================================== */
int CSTcpIpInitExtra(IP_INFO sIpinfo);


/*==========================================================
����ԭ��:void CSTcpipSaveIpparameter(void);
�������:��
�������:��
����ֵ: ��
���� :����IP���������ü�����
ע������:
=========================================================== */
void CSTcpipSaveIpparameter(void);


/*==========================================================
����ԭ��:int CSTcpipGetIpparameter(IP_PARAMETER * pIpparameter);
�������:��
�������:ָ��IP������ָ��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :��ȡЭ��ջ�д˿̵�IP��Ϣ
ע������:
=========================================================== */
int CSTcpipGetIpparameter(IP_PARAMETER * pIpparameter);


/*==========================================================
����ԭ��:int CSTcpipSetIpparameter(IP_PARAMETER * pIpParameter);
�������:ָ��IP������ָ��
�������:��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :����IP������Э��ջ��
ע������:
=========================================================== */
int CSTcpipSetIpparameter(IP_PARAMETER * pIpParameter);


/*==========================================================
����ԭ��:int CSTcpipIpToStr(const char * pIp, char * pStr);
�������:ָ��IP��ַ�ַ�����ָ��
�������:ָ���ڴ���ָ��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :��IP��ַ�ִ�ת����15λ�ַ����������û���ʾ
��:��192.168.1.1ת����192.168.001.001
ע������:
=========================================================== */
int CSTcpipIpToStr(const char * pIp, char * pStr);


/*==========================================================
����ԭ��:int CSTcpipStrToIp(const char * pStr, char * pIp);
�������:ָ��IP��ַ�ַ�����ָ��
�������:ָ���ڴ���ָ��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :��15λ�ַ���ת����IP��ַ�ִ������ڷ��ظ�Э��ջ
��:��192.168.001.001ת����192.168.1.1
ע������:
=========================================================== */
int CSTcpipStrToIp(const char * pStr, char * pIp);


/*==========================================================
����ԭ��:int CSTcpipCalculateMask(const char * pIp, char * pMask);
�������:ָ��IP��ַ�ַ�����ָ��
�������:ָ���ڴ���ָ��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :����IP��ַ���������룬�˺���ֻ�ܼ����׼��������
��:����192.168.1.1���255.255.255.000
ע������:
=========================================================== */
int CSTcpipCalculateMask(const char * pIp, char * pMask);



/*==========================================================
����ԭ��:int CSTcpipCalculateMask(const char * pIp, char * pMask);
�������:ָ��IP��ַ�ַ�����ָ��
�������:ָ���ڴ���ָ��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :����IP��ַ�����أ����������޷����㣬�˽ӿڴ��ھ�����
��:����192.168.1.1���192.168.1.254
ע������:����������Ҫ���û�����
=========================================================== */
int CSTcpipCalculateGateway(const char * pIp, char * pGateway);


/*==========================================================
����ԭ��:int CSTcpipGetVersion(char * pcVersion, int nSize);
�������:�ڴ��ĳ���
�������:ָ��洢�汾�ŵ��ڴ��ָ��
����ֵ: ִ�н����0�ɹ���-1ʧ��(����������ʧ��)
���� :��ȡTCPIP��װģ��汾��Ϣ
ע������:�������ʧ�ܣ�������ڴ���С
=========================================================== */
int CSTcpipGetSetupVersion(char * pcVersion, int nSize);


/*==========================================================
����ԭ��:int CSTcpipGetCompileTime(char * pcCompileTime, int nSize);
�������:����ʱ�� ���ַ������ַ����ĳ���
�������:ָ��洢�������ڵ�ָ��
����ֵ: ִ�н����0�ɹ���-1ʧ��(����������ʧ��)
���� :��ȡ��������
ע������:�������ʧ�ܣ�������ڴ���С
=========================================================== */
int CSTcpipGetSetupCompileTime(char * pcCompileTime, int nSize);


/*******************************Intergration developer use***********************/
/*==========================================================
����ԭ��:void CSTcpipReadIpparameter(void);
�������:��
�������:��
����ֵ: ��
���� :�����ü������ж�ȡIP�������ڴ���
ע������:�˽ӿ�һ�㲻����ã�����ʱ������Ҫ
=========================================================== */
void CSTcpipReadIpparameter(void);


/*==========================================================
����ԭ��:BOOL CSTcpipConnectionOK(void);
�������:��
�������:��
����ֵ: BOOLֵ�����ʾ�����������ٱ�ʾ���Ӳ�����
���� :��ȡ�˿���������״�����������˫��Ӧ��ǰ�����ж�
ע������:�˽ӿ�ʵ�ʻ����Ƿ��ȡ��IP�����������Ƿ�����
=========================================================== */
BOOL CSTcpipConnectionOK(void);


/*==========================================================
����ԭ��:BOOL CSTcpipConnectionOKCM(BOOL bCheckCMStatus);
�������:BOOLֵ�����ʾ��Ҫ���CM״̬���ٲ����
�������:��
����ֵ: BOOLֵ�����ʾ�����������ٱ�ʾ���Ӳ�����
���� :��ȡ�˿���������״�����������˫��Ӧ��ǰ�����ж�
ע������:�˽ӿ�ʵ�ʻ����Ƿ��ȡ��IP������������CM�Ƿ�����
****ע��:�˽ӿ������ڴ���CM��˫�������
=========================================================== */
BOOL CSTcpipConnectionOKCM(BOOL bCheckCMStatus);


/*==========================================================
����ԭ��:int CSTcpipGetIpaddr(char * pIpaddr, int nAddrLen);
�������:�ڴ��ĳ���
�������:�˿�Э��ջ��ʹ�õ�IP��ַ
����ֵ: ��ȡ�ɹ�����0�����򷵻�-1
���� :��ȡ�˿�Э��ջ��ʹ�õ�IP��ַ
ע������:
=========================================================== */
int CSTcpipGetIpaddr(char * pIpaddr, int nAddrLen);


/*==========================================================
����ԭ��:int CSTcpipGetMask(char * pIpaddr, int nAddrLen);
�������:�ڴ��ĳ���
�������:�˿�Э��ջ��ʹ�õ���������
����ֵ: ��ȡ�ɹ�����0�����򷵻�-1
���� :��ȡ�˿�Э��ջ��ʹ�õ���������
ע������:
=========================================================== */
int CSTcpipGetMask(char * pIpaddr, int nAddrLen);

/*==========================================================
����ԭ��:int CSTcpipGetCmMac(BYTE * pCmMac, int nCmMacLen);
�������:CableModelm  MAC ��ַ�ĳ���
�������:CableModelm ��MAC��ַ
����ֵ: ��ȡ�ɹ�����0�����򷵻�-1
���� :��ȡCM��MAC��ַ
ע������:
=========================================================== */
int CSTcpipGetCmMac( BYTE  * pCmMac, int nCmMacLen);


/*==========================================================
����ԭ��:int CSTcpipGetGateway(char * pIpaddr, int nAddrLen);
�������:�ڴ��ĳ���
�������:�˿�Э��ջ��ʹ�õ�����
����ֵ: ��ȡ�ɹ�����0�����򷵻�-1
���� :��ȡ�˿�Э��ջ��ʹ�õ�����
ע������:
=========================================================== */
int CSTcpipGetGateway(char * pIpaddr, int nAddrLen);


/*==========================================================
����ԭ��:int CSTcpipGetMainDNS(char * pDNSaddr, int nAddrLen);
�������:�ڴ��ĳ���
�������:�˿�Э��ջ��ʹ�õ���DNS
����ֵ: ��ȡ�ɹ�����0�����򷵻�-1
���� :��ȡ�˿�Э��ջ��ʹ�õ���DNS
ע������:
=========================================================== */
int CSTcpipGetMainDNS(char * pDNSaddr, int nAddrLen);


/*==========================================================
����ԭ��:int CSTcpipGetBackDNS(char * pDNSaddr, int nAddrLen);
�������:�ڴ��ĳ���
�������:�˿�Э��ջ��ʹ�õı���DNS
����ֵ: ��ȡ�ɹ�����0�����򷵻�-1
���� :��ȡ�˿�Э��ջ��ʹ�õı���DNS
ע������:
=========================================================== */
int CSTcpipGetBackDNS(char * pDNSaddr, int nAddrLen);


/*==========================================================
����ԭ��:int CSTcpipGetExServer(int nIndex, char * pServerIP, int nSize);
�������:�ڴ��ĳ��ȣ���չ������������
�������:��չ�������ĵ�ַ
����ֵ: ��ȡ�ɹ�����0�����򷵻�-1
���� :��ȡ��չ�������ĵ�ַ
ע������:
=========================================================== */
int CSTcpipGetExServer(int nIndex, char * pServerIP, int nSize);


/*==========================================================
����ԭ��:int CSTcpipGetHttpServer(char * pServerIP, int nSize);
�������:�ڴ��ĳ���
�������:HTTP�������ĵ�ַ
����ֵ: ��ȡ�ɹ�����0�����򷵻�-1
���� :��ȡHTTP�������ĵ�ַ
ע������:
=========================================================== */
int CSTcpipGetHttpServer(char * pServerIP, int nSize);


/*==========================================================
����ԭ��:int CSTcpipGetVodServer(char * pServerIP, int nSize);
�������:�ڴ��ĳ���
�������:VOD�������ĵ�ַ
����ֵ: ��ȡ�ɹ�����0�����򷵻�-1
���� :��ȡVOD�������ĵ�ַ
ע������:
=========================================================== */
int CSTcpipGetVodServer(char * pServerIP, int nSize);




/******************************Twoway developer use***************************/
/*==========================================================
����ԭ��:int CSTcpipIpconfig(char * pIpAddr, char * pMask, char * pGateway);
�������:IP��ַ���������룬����
�������:��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :����IP��ַ���������룬���ص�Э��ջ��
ע������:
=========================================================== */
int CSTcpipIpconfig(char * pIpAddr, char * pMask, char * pGateway);


/*==========================================================
����ԭ��:int CSTcpipIpconfig(char * pIpAddr, char * pMask, char * pGateway);
�������:IPģʽ
�������:��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :���û�ȡIP�ķ�ʽDHCP���ֶ�����
ע������:
=========================================================== */
int CSTcpipSetIPMode(IP_SET_MODE mode);


/*==========================================================
����ԭ��:int CSTcpipIpconfig(char * pIpAddr, char * pMask, char * pGateway);
�������:IPģʽ
�������:��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :���û�ȡIP�ķ�ʽDHCP���ֶ�����
ע������:
=========================================================== */
int CSTcpipSetIPModeExtra(IP_SET_MODE mode);


/*==========================================================
����ԭ��:DWORD CSTcpipGetIpMode(void);
�������:��ȡ��ʱIPģʽ
�������:��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :��ȡIP�ķ�ʽDHCP���ֶ�����
ע������:
=========================================================== */
DWORD CSTcpipGetIpMode(void);




/*==========================================================
����ԭ��:DWORD CSTcpipGetIpMode(void);
�������:��ȡ��ʱIPģʽ
�������:��
����ֵ: ִ�н����0�ɹ���-1ʧ��
���� :��ȡIP�ķ�ʽDHCP���ֶ�����
ע������:
=========================================================== */
DWORD CSTcpipGetIpModeExtra(void);


/*==========================================================
����ԭ��:BOOL CSTcpipGetHaveIp(void);
�������:��
�������:��
����ֵ: BOOLֵ����ȡ��IP�����棬���򷵻ؼ�
���� :��ѯ�Ƿ��ȡ��IP
ע������:
=========================================================== */
BOOL CSTcpipGetHaveIp(void);


/*==========================================================
����ԭ��:BOOL CSTcpipGetEthStatus(void);
�������:��
�������:��
����ֵ: BOOLֵ�������������������棬���򷵻ؼ�
���� :��ѯ���������Ƿ�����
ע������:
=========================================================== */
BOOL CSTcpipGetEthStatus(void);


/*==========================================================
����ԭ��:int CSTcpipGetCMState(void);
�������:��
�������:��
����ֵ: CM������״̬
���� :��ѯCM������״̬
ע������:
=========================================================== */
CM_STATUS CSTcpipGetCMState(void);


/*==========================================================
����ԭ��:BOOL CSTcpipGetEthStatus(void);
�������:��
�������:��
����ֵ: ��
���� :���»�ȡIP���˶������ͷŵ�ǰIP������ȥ��ȡ
ע������:
=========================================================== */
void CSTcpipStartDHCP(void);

/*==========================================================
����ԭ��:void CSTcpipSetRestartDHCP(BOOL bFlag);
�������:�Ƿ���������dhcp��־λ
�������:��
����ֵ: ��
���� :�����Ƿ���������dhcp
ע������:
=========================================================== */
void CSTcpipSetRestartDHCP(BOOL bFlag);



/*==========================================================
����ԭ��:BOOL CSTcpipGetRestartDHCP(void);
�������:��
�������:��
����ֵ: ��ȡ�Ƿ��������� dhcp�ı�־λtrue or false
���� :��ȡ�Ƿ��������� dhcp�ı�־λ
ע������:
=========================================================== */
BOOL CSTcpipGetRestartDHCP(void);



/*==========================================================
����ԭ��:void CSSetCMIp(char* pcIpAddr,int nLen);
�������:
pcIpAddr:  CM��ip��ַ
nLen:      ip��ַ����
�������:��
����ֵ: ��
���� :����CM��IP��ַ
ע������:
=========================================================== */
void CSSetCMIp(char* pcIpAddr,int nLen);




/*==========================================================
����ԭ��:void CSGetCMIp(char* pcIpAddr,int nLen);
�������:
nLen: ip��ַ����
�������:
pcIpAddr:�����ȡ��CM��IP��ַ
����ֵ: ��
���� :��ȡCM��IP��ַ
ע������:
=========================================================== */
void CSGetCMIp(char* pcIpAddr,int nLen);


/**
@brief ע��CM״̬���Ļص�����

@param[in] fnCallback �ص�����ָ�룬���FOnCMStatusCallback����
@param[in] dwUserData �û�����
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE��
@note �ص�����������û�����Ψһȷ��һ���ص�������������ע��������ȫһ��ʱ���ڶ��ν�����CSUDI_FALSE,������Ӷ���ص�  
*/
BOOL CSCMAddCallback(DWORD dwUserData, FOnCMStatusCallback fnCallback);


/**
@brief ɾ��CM״̬�Ļص�����

@param[in] fnCallback �ص�����ָ�룬���FOnCMStatusCallback����
@param[in] dwUserData ͬCSCMAddCallback��dwUserData
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE��
*/
BOOL CSCMRemoveCallback(DWORD dwUserData, FOnCMStatusCallback fnCallback);


/**
@brief ��ȡCM����Ƶ��

@param[out] puCmDownFreq ��ŵ�ǰ����Ƶ��,��λΪHz
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE����puCmDownFreq������δ���塣
*/
BOOL CSCMGetDownFrequency(unsigned int * puCmDownFreq); 


/**
@brief ��ȡCM�����ź�ǿ��

@param[out] puCmDownLevel ��ŵ�ǰ�����ź�ǿ��, ���������10�Ժ󣬵�λΪdBmv
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE����puCmDownLevel������δ���塣
*/
BOOL CSCMGetDownLevel(unsigned int * puCmDownLevel) ; 


/**
@brief ��ȡCM���е��Ʒ�ʽ

@param[out] pcDownModulation ��ŵ�ǰ�����ź�ǿ��,*pcDownModulationֵ�Ķ�Ӧ��ϵ
		    unknown(1)��
		    other(2)��
		    QAM64(3)��
		    QAM256(4)
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE��
*/
BOOL CSCMGetDownModulation(unsigned int * puCmDownModulation);


/**
@brief ��ȡCM���������

@param[out]  puCmDownSNR ��ŵ�ǰ���������,���������10�Ժ�ĵ�λΪdB
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE����puCmDownSNR������δ���塣��
*/
BOOL  CSCMGetDownSNR(unsigned int * puCmDownSNR);  


/**
@brief ��ȡCM����Ƶ��

@param[out] puCmUpFreq ��ŵ�ǰ����Ƶ�㣬��λΪHz
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE����puCmUpFreq������δ���塣
*/
BOOL  CSCMGetUpFrequency(unsigned int * puCmUpFreq);  


/**
@brief ��ȡCM�����ź�ǿ��

@param[out] puCmUpLevel ��ŵ�ǰ�����ź�ǿ��,�������10�Ժ�λΪdBmv
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE����puCmUpLevel������δ���塣
*/
BOOL  CSCMGetUpLevel(unsigned int * puCmUpLevel); 


/**
@brief ��ȡCM���е��Ʒ�ʽ

@param[out] puCmUpModulation ���е��Ʒ�ʽ,ֵΪ0���������е��Ʒ�ʽ
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE��
*/
BOOL CSCMGetUpModulation(unsigned int *puCmUpModulation);


/**
@brief ��ȡCM��MAC�㹤����ʽ

@param[out] puCmMACMode ���MAC�㹤����ʽ
@return �ɹ�����CSUDI_TRUE��ʧ���򷵻�CSUDI_FALSE��
*/
BOOL CSCMGetMACLayerMode(unsigned int *puCmMACMode);


#ifdef __cplusplus
}
#endif

#endif

/**********************************EOF**********************************/

