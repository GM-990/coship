/*
*cs_ipcfg.h
*/


#ifndef _CS_IPCFG_H_
#define _CS_IPCFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cs_ipcfg_plus.h"

typedef int ip_size_t;

/* config dhcpc and pppoe 
	unsigned short i.e. WORD
	high bit <==> high layer
	low  bit <==> low  layer
	bit [15] ... ... [4] ... [0]
	0 ..........: laod dhcps
	1 ..........: load dhcpc
	0 ..: load ethernet
	1 ..: load pppoe*/
#define CS_CONFIG_DHCPC    (unsigned short)0x10U   
#define CS_CONFIG_PPPOE    (unsigned short)0x01U

/*����ͳ����Ϣ*/
typedef struct {
    unsigned int uConnectTime;          //���ӳ���ʱ��
    unsigned int uTotalBytesSend;       //���Ͱ��ֽ���
    unsigned int uTotalBytesReceived;   //���հ��ֽ���
    unsigned int uTotalPacketSend;      //���Ͱ�����
    unsigned int uTotalPacketReceived;  // �ӷ�������
}CSIPCFGEtherStat_S;

/***********************************************************
* ����: CSIPCFG_GetStatInfo
* ����: ��ȡ����ͳ����Ϣ
* ����:  
*       pcName : �����豸����
*                     
* ���: 
*       psNetStatus : �洢ʵ�ʻ�ȡ��������ͳ����Ϣ�Ľṹ��
*
* ����ֵ: �ɹ�����0��ʧ�ܷ��ش�����
*   
************************************************************/
int CSIPCFG_GetStatInfo(char *pcName,CSIPCFGEtherStat_S *psNetStatus);

// for DNS
/***********************************************************
* ����: CSIPCFG_SetDNSServer1/CSIPCFG_SetDNSServer2/CSIPCFG_SetDomain
* ����: ���õ�һ��DNS��������IP��ַ
* ����:  
*       ncDNS1 : DNS��������IP��ַ����"192.168.99.5"�ĵ��ʮ����IP��ַ����
*       nLen : DNS��������IP��ַ�ַ������ȣ���С��16 
*                   
* ���: 
*     
* ����ֵ: 
*   
************************************************************/
void  CSIPCFG_SetDNSServer1(char* pcDNS1,int nLen);
void  CSIPCFG_SetDNSServer2(char* pcDNS2,int nLen);
void  CSIPCFG_SetDomain(char* pcDomain,int nLen);


/***********************************************************
* ����: CSIPCFG_GetDNSServer1/CSIPCFG_GetDNSServer2/CSIPCFG_GetDomain
* ����: ���DNS��������IP��ַ
* ����:  
*       nLen : DNS��������IP��ַ�ַ������ȣ���С��16 
*                   
* ���: 
*     ncDNS1 : DNS��������IP��ַ����"192.168.99.5"�ĵ��ʮ����IP��ַ����
* ����ֵ: 
*   
************************************************************/
void  CSIPCFG_GetDNSServer1(char* pcDNS1,int nLen);
void  CSIPCFG_GetDNSServer2(char* pcDNS2,int nLen);
void  CSIPCFG_GetDomain(char* pcDomain,int nLen);


/***********************************************************
* ����: CSIPCFG_SetDNSServer
* ����: ���õ�i��DNS��������IP��ַ
* ����:  
*       nDNSIndex:  Ҫ���õ�dns��������
*       pcDNS : DNS��������IP��ַ����"192.168.99.5"�ĵ��ʮ����IP��ַ����
*       nLen : DNS��������IP��ַ�ַ�������
*                   
* ���: 
*     
* ����ֵ: �ɹ�����0��ʧ�ܷ��ش�����
*   
************************************************************/
int CSIPCFG_SetDNSServer(int nDNSIndex,char *pcDNS, int nLen);


/***********************************************************
* ����: CSIPCFG_GetDNSServer
* ����: ��õ�i��DNS��������IP��ַ
* ����:  
*       nDNSIndex:  Ҫ���õ�dns��������
*       nLen : DNS��������IP��ַ�ַ������ȣ���С��16 
*                   
* ���: 
*     pcDNS : DNS��������IP��ַ����"192.168.99.5"�ĵ��ʮ����IP��ַ����
* ����ֵ: �ɹ�����0��ʧ�ܷ��ش�����
*   
************************************************************/
int  CSIPCFG_GetDNSServer(int nDNSIndex,char* pcDNS, int nLen);
	


/***************************************************************************
* ����: CSIPCFG_IpConfig
* ����:  ����������IP��ַ���������룬���ص�ַ
* ����:  
*       pcEthName : �����豸�����磺"eth0"��
*       pcIpAddr : IP��ַ�ַ�������"192.168.1.1"��     
*	    pcNetMask: ���������ַ�������"255.255.255.0"
*       pcGateWay : ���ص�ַ�ַ�������"192.168.1.254"
*                   
* ���: 
*     
* ����ֵ: 
*   �ɹ���0
*   ʧ�ܣ���0������������
********************************************************************************/
int  CSIPCFG_IpConfig(char* pcEthName,char* pcIpAddr, char* pcNetMask, char* pcGateWay);

/***************************************************************************
* ����: CSIPCFG_SetAddr(�½ӿ�)/csIfSetAddr(�ɽӿ�)
* ����:  ����������IP��ַ����������
* ����:  
*       pcName : �����豸�����磺"eth0"��
*       dwAaddr :  IP��ַ�������ֽ���������ַ��     
*	    dwNetmask:  �������룬�����ֽ���������ַ       
*
*                   
* ���: 
*     
* ����ֵ: 
*   �ɹ���0
*   ʧ�ܣ���0������������
********************************************************************************/
int  CSIPCFG_SetAddr( char* pcName, unsigned int dwAaddr, unsigned int dwNetmask);
int  csIfSetAddr(char *pcName, unsigned int dwAddr, unsigned int dwNetmask);


/***************************************************************************
* ����: CSIPCFG_SetRouteDefault
* ����: ����Ĭ�����ص�ַ
* ����:  
*       dwGateway :���ص�ַ�������ֽ���������ַ��
*                   
* ���: 
*     
* ����ֵ: 
*   �ɹ���0
*   ʧ�ܣ���0������������
********************************************************************************/
int  CSIPCFG_SetRouteDefault( unsigned int dwGateway);


/***************************************************************************
* ����: CSIPCFG_GetEherName
* ����: ��ȡ�����豸��
* ����:  
*       nEhterIndex :�����豸�����ţ��� 0��1 ����?
*       nLen:pszEheterName��ָ�ռ�ĳ��ȣ��������1С��20
*                   
* ���: 
*      pszEheterName:��ȡ���������豸�������û�����ռ䣬���ȱ������1С��20
*
* ����ֵ: 
*   �ɹ���0
*   ʧ�ܣ���0������������
********************************************************************************/
int  CSIPCFG_GetEherName(int nEhterIndex,char* pszEheterName,int nLen);


/***************************************************************************
* ����: CSIPCFG_GetIP
* ����: ��ȡIP��ַ
* ����:  
*       pcName :�����豸��
*       nIplen: IP��ַ�ַ����ĳ��ȣ����벻С��16
*                   
* ���: 
*      pcIp:��ȡ����IP��ַ�����û�����ռ䣬���ȱ��벻С��16
*
* ����ֵ: 
*   �ɹ���0
*   ʧ�ܣ���0������������
********************************************************************************/
int CSIPCFG_GetIP(char* pcName,char* pcIp,int nIplen);


/***************************************************************************
* ����: CSIPCFG_GetMASK
* ����: ��ȡ��������
* ����:  
*       pcName :�����豸��
*       nLen: ���������ַ����ĳ��ȣ����벻С��16
*                   
* ���: 
*      pcMask: ��ȡ�������������ַ�����û�����ռ�,���ȱ��벻С��16
*
* ����ֵ: 
*      ��
********************************************************************************/
void CSIPCFG_GetMASK(char* pcName,char *pcMask,int nLen);


/***************************************************************************
* ����: CSIPCFG_GetGateWay
* ����: ��ȡ���ص�ַ
* ����:  
*       pcName :�����豸��
*       nLen: ���ص�ַ�ַ����ĳ��ȣ����벻С��16
*                   
* ���: 
*      pcGateWay:  ��ȡ�������ص�ַ�����û�����ռ�,���ȱ��벻С��16
*
* ����ֵ: 
*      ��
********************************************************************************/
void CSIPCFG_GetGateWay(char* pcName,char *pcGateWay,int nLen);

/***************************************************************************
* ����: CSTcpipGetVersion/CSIpcfgGetVersion
* ����:  ��ȡTcpIp/cs_ipcfg�汾��
* ����:  
*       nSize  :version buffer�ĳ��ȣ���С����
*       
*                   
* ���: 
*       pcVer: ��ȡ���İ汾�ţ����û�����ռ䣬���Ȳ���
*
* ����ֵ: 
*      1>���pchVer= NULL, ���ذ汾��Ϣ��ʵ�ʳ��ȣ�����������'\0'.
*      2>���pchVer != NULL, �ҵ��óɹ�,����ʵ�ʻ�ȡ�İ汾��Ϣ�ĳ���nSize.	
*      3>�������ʧ��, ���� 0.
********************************************************************************/
int CSTcpipGetVersion( char* pcVer, int nSize );
int CSIpcfgGetVersion( char* pcVer, int nSize );


/***************************************************************************
* ����: CSIPCFG_SetArpUpdateTime
* ����: ����ARP�����ʱ�䡣
* ����:  
*       unArpUpdateTime   ARP�����ʱ�䣬��λ:s
*                   
* ���: 
*     
*
* ����ֵ: 
*      �ɹ���0
*      ʧ�ܣ���0����������ֵ
********************************************************************************/
int  CSIPCFG_SetArpUpdateTime(unsigned int  unArpUpdateTime);


/***************************************************************************
* ����: CSIPCFG_SetArpUpdateTime
* ����: ���ARP�����ʱ�䡣
* ����:  
*       
* ���: 
*     
* ����ֵ: 
*      �ɹ���0
*      ʧ�ܣ���0����������ֵ
********************************************************************************/
int  CSIPCFG_GetArpUpdateTime();



#ifdef __cplusplus
}
#endif
#endif /* __CSIPCFG_H__ */





