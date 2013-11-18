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

/*网卡统计信息*/
typedef struct {
    unsigned int uConnectTime;          //连接持续时间
    unsigned int uTotalBytesSend;       //发送包字节数
    unsigned int uTotalBytesReceived;   //接收包字节数
    unsigned int uTotalPacketSend;      //发送包个数
    unsigned int uTotalPacketReceived;  // 接发包个数
}CSIPCFGEtherStat_S;

/***********************************************************
* 函数: CSIPCFG_GetStatInfo
* 功能: 获取网卡统计信息
* 输入:  
*       pcName : 网卡设备名称
*                     
* 输出: 
*       psNetStatus : 存储实际获取到的网卡统计信息的结构体
*
* 返回值: 成功返回0，失败返回错误码
*   
************************************************************/
int CSIPCFG_GetStatInfo(char *pcName,CSIPCFGEtherStat_S *psNetStatus);

// for DNS
/***********************************************************
* 函数: CSIPCFG_SetDNSServer1/CSIPCFG_SetDNSServer2/CSIPCFG_SetDomain
* 功能: 设置第一个DNS服务器的IP地址
* 输入:  
*       ncDNS1 : DNS服务器的IP地址（如"192.168.99.5"的点分十进制IP地址）。
*       nLen : DNS服务器的IP地址字符串长度，不小于16 
*                   
* 输出: 
*     
* 返回值: 
*   
************************************************************/
void  CSIPCFG_SetDNSServer1(char* pcDNS1,int nLen);
void  CSIPCFG_SetDNSServer2(char* pcDNS2,int nLen);
void  CSIPCFG_SetDomain(char* pcDomain,int nLen);


/***********************************************************
* 函数: CSIPCFG_GetDNSServer1/CSIPCFG_GetDNSServer2/CSIPCFG_GetDomain
* 功能: 获得DNS服务器的IP地址
* 输入:  
*       nLen : DNS服务器的IP地址字符串长度，不小于16 
*                   
* 输出: 
*     ncDNS1 : DNS服务器的IP地址（如"192.168.99.5"的点分十进制IP地址）。
* 返回值: 
*   
************************************************************/
void  CSIPCFG_GetDNSServer1(char* pcDNS1,int nLen);
void  CSIPCFG_GetDNSServer2(char* pcDNS2,int nLen);
void  CSIPCFG_GetDomain(char* pcDomain,int nLen);


/***********************************************************
* 函数: CSIPCFG_SetDNSServer
* 功能: 设置第i个DNS服务器的IP地址
* 输入:  
*       nDNSIndex:  要设置的dns的索引号
*       pcDNS : DNS服务器的IP地址（如"192.168.99.5"的点分十进制IP地址）。
*       nLen : DNS服务器的IP地址字符串长度
*                   
* 输出: 
*     
* 返回值: 成功返回0，失败返回错误码
*   
************************************************************/
int CSIPCFG_SetDNSServer(int nDNSIndex,char *pcDNS, int nLen);


/***********************************************************
* 函数: CSIPCFG_GetDNSServer
* 功能: 获得第i个DNS服务器的IP地址
* 输入:  
*       nDNSIndex:  要设置的dns的索引号
*       nLen : DNS服务器的IP地址字符串长度，不小于16 
*                   
* 输出: 
*     pcDNS : DNS服务器的IP地址（如"192.168.99.5"的点分十进制IP地址）。
* 返回值: 成功返回0，失败返回错误码
*   
************************************************************/
int  CSIPCFG_GetDNSServer(int nDNSIndex,char* pcDNS, int nLen);
	


/***************************************************************************
* 函数: CSIPCFG_IpConfig
* 功能:  设置网卡的IP地址，子网掩码，网关地址
* 输入:  
*       pcEthName : 网卡设备名，如："eth0"。
*       pcIpAddr : IP地址字符串，如"192.168.1.1"。     
*	    pcNetMask: 子网掩码字符串，如"255.255.255.0"
*       pcGateWay : 网关地址字符串，如"192.168.1.254"
*                   
* 输出: 
*     
* 返回值: 
*   成功：0
*   失败：非0，其他错误码
********************************************************************************/
int  CSIPCFG_IpConfig(char* pcEthName,char* pcIpAddr, char* pcNetMask, char* pcGateWay);

/***************************************************************************
* 函数: CSIPCFG_SetAddr(新接口)/csIfSetAddr(旧接口)
* 功能:  设置网卡的IP地址，子网掩码
* 输入:  
*       pcName : 网卡设备名，如："eth0"。
*       dwAaddr :  IP地址，网络字节序的网络地址。     
*	    dwNetmask:  子网掩码，网络字节序的网络地址       
*
*                   
* 输出: 
*     
* 返回值: 
*   成功：0
*   失败：非0，其他错误码
********************************************************************************/
int  CSIPCFG_SetAddr( char* pcName, unsigned int dwAaddr, unsigned int dwNetmask);
int  csIfSetAddr(char *pcName, unsigned int dwAddr, unsigned int dwNetmask);


/***************************************************************************
* 函数: CSIPCFG_SetRouteDefault
* 功能: 设置默认网关地址
* 输入:  
*       dwGateway :网关地址，网络字节序的网络地址。
*                   
* 输出: 
*     
* 返回值: 
*   成功：0
*   失败：非0，其他错误码
********************************************************************************/
int  CSIPCFG_SetRouteDefault( unsigned int dwGateway);


/***************************************************************************
* 函数: CSIPCFG_GetEherName
* 功能: 获取网卡设备名
* 输入:  
*       nEhterIndex :网卡设备索引号，如 0，1 …　?
*       nLen:pszEheterName所指空间的长度，必须大于1小于20
*                   
* 输出: 
*      pszEheterName:获取到的网卡设备名，由用户申请空间，长度必须大于1小于20
*
* 返回值: 
*   成功：0
*   失败：非0，其他错误码
********************************************************************************/
int  CSIPCFG_GetEherName(int nEhterIndex,char* pszEheterName,int nLen);


/***************************************************************************
* 函数: CSIPCFG_GetIP
* 功能: 获取IP地址
* 输入:  
*       pcName :网卡设备名
*       nIplen: IP地址字符串的长度，必须不小于16
*                   
* 输出: 
*      pcIp:获取到的IP地址，由用户申请空间，长度必须不小于16
*
* 返回值: 
*   成功：0
*   失败：非0，其他错误码
********************************************************************************/
int CSIPCFG_GetIP(char* pcName,char* pcIp,int nIplen);


/***************************************************************************
* 函数: CSIPCFG_GetMASK
* 功能: 获取子网掩码
* 输入:  
*       pcName :网卡设备名
*       nLen: 子网掩码字符串的长度，必须不小于16
*                   
* 输出: 
*      pcMask: 获取到的子网掩码地址，由用户申请空间,长度必须不小于16
*
* 返回值: 
*      无
********************************************************************************/
void CSIPCFG_GetMASK(char* pcName,char *pcMask,int nLen);


/***************************************************************************
* 函数: CSIPCFG_GetGateWay
* 功能: 获取网关地址
* 输入:  
*       pcName :网卡设备名
*       nLen: 网关地址字符串的长度，必须不小于16
*                   
* 输出: 
*      pcGateWay:  获取到的网关地址，由用户申请空间,长度必须不小于16
*
* 返回值: 
*      无
********************************************************************************/
void CSIPCFG_GetGateWay(char* pcName,char *pcGateWay,int nLen);

/***************************************************************************
* 函数: CSTcpipGetVersion/CSIpcfgGetVersion
* 功能:  获取TcpIp/cs_ipcfg版本号
* 输入:  
*       nSize  :version buffer的长度，大小不限
*       
*                   
* 输出: 
*       pcVer: 获取到的版本号，由用户申请空间，长度不限
*
* 返回值: 
*      1>如果pchVer= NULL, 返回版本信息的实际长度，包括结束符'\0'.
*      2>如果pchVer != NULL, 且调用成功,返回实际获取的版本信息的长度nSize.	
*      3>如果调用失败, 返回 0.
********************************************************************************/
int CSTcpipGetVersion( char* pcVer, int nSize );
int CSIpcfgGetVersion( char* pcVer, int nSize );


/***************************************************************************
* 函数: CSIPCFG_SetArpUpdateTime
* 功能: 设置ARP表更新时间。
* 输入:  
*       unArpUpdateTime   ARP表更新时间，单位:s
*                   
* 输出: 
*     
*
* 返回值: 
*      成功：0
*      失败：非0，其他错误值
********************************************************************************/
int  CSIPCFG_SetArpUpdateTime(unsigned int  unArpUpdateTime);


/***************************************************************************
* 函数: CSIPCFG_SetArpUpdateTime
* 功能: 获得ARP表更新时间。
* 输入:  
*       
* 输出: 
*     
* 返回值: 
*      成功：0
*      失败：非0，其他错误值
********************************************************************************/
int  CSIPCFG_GetArpUpdateTime();



#ifdef __cplusplus
}
#endif
#endif /* __CSIPCFG_H__ */





