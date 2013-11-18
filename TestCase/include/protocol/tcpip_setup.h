/*************************************************************************
**************************************************************************
* Copyright (c) 2007,ShenZhen Coship Electronic Ltd Co.
* All rights reserved.
* 
* File Name:		Tcpip_setup.h
* File Identify:
* Summary:		Interface about TCP/IP Stack
* 
* Current Version：1.1
* Author(s)：Peng Dong, Huai Yonglong
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


/**@brief 信号灯指示
@note                                                                              
@note                                       下行        上行           在线       
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


/**@brief CM状态值 */                            
typedef enum
{
    EM_CM_FAIL=0,                      ///< cm状态错误，创维的盒子有该状态，我司的盒子没有该状态
    EM_CM_INIT,                           ///< 初始化状态，
    EM_CM_NOT_READY,                     ///< 未就绪状态，
    EM_CM_NOT_LOCK,                       ///< 未锁频
    EM_CM_PHY_SYN,                        ///< 同步下行   
    EM_CM_USPARA_ACQUIRED,                ///< 获取上行通道参数
    EM_CM_RANGE_COMPLETE,                 ///< 上行测距完成
    EM_CM_IP_COMPLETE,                    ///< 建立ip连接
    EM_CM_TOD_ESTAB,                      ///< 建立tod同步
    EM_CM_SECURITY_ESTAB,                 ///< 建立安全机制
    EM_CM_PARA_TS_COMPLETE,               ///< 配置参数获取成功
    EM_CM_REG_COMPLETE,                   ///< cm注册完成
    EM_CM_OPERTIONAL,                     ///< cm上线
    EM_CM_ACCESS_DENIED                   ///< 访问受限
}CSCMStatus_E;


typedef void (*TcpipEventCallback)(NETWORK_STATUS event);
typedef void (*TcpipPrintCallback)(const char * const Format_p,...);


/**@brief 参数mode取值范围为 CM_NETWORK_ONLINE 或 CM_NETWORK_OFFLINE 。此回调函数可通知用户网络的连接状况，并做出动作。 */
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
函数原型:int CSTcpipRegistInfo(REGISTER_INFO stbInfo);
输入参数:注册 用户选项及函数指针
输出参数:无
返回值: 执行结果，0成功，-1失败
功能 :调用TCPIP封装接口前注册用户信息
注意事项:应在本模块所有接口前调用
=========================================================== */
int CSTcpipRegistInfo(REGISTER_INFO stbInfo);


/*==========================================================
函数原型:int CSTcpIpInit(void);
输入参数:无
输出参数:无
返回值: 执行结果，0成功，-1失败
功能 :初始化TCPIP协议栈
注意事项:应在网口硬件初始化完毕后调用，如果要设置
 	dhcp的renew包发送方式为单播还是广播，则须在调用此函
 	数前(即启动dhcp前)调用cs_dhcpc.h文件里面的CSDHCPSetRenewUnicastMode
 	接口,若不调用此接口，则默认renew包的发送方式为广播
=========================================================== */
int CSTcpIpInit(void);

/*==========================================================
函数原型:int CSTcpIpInitExtra(IP_INFO sipinfo);
输入参数:IP_INFO sipinfo
输出参数:无
返回值: 执行结果，0成功，-1失败
功能 :初始化TCPIP协议栈
注意事项:应在网口硬件初始化完毕后调用,如果要设置
 	dhcp的renew包发送方式为单播还是广播，则须在调用此函
 	数前(即启动dhcp前)调用cs_dhcpc.h文件里面的CSDHCPSetRenewUnicastMode
 	接口,若不调用此接口，则默认renew包的发送方式为广播
=========================================================== */
int CSTcpIpInitExtra(IP_INFO sIpinfo);


/*==========================================================
函数原型:void CSTcpipSaveIpparameter(void);
输入参数:无
输出参数:无
返回值: 无
功能 :保存IP参数到永久记忆体
注意事项:
=========================================================== */
void CSTcpipSaveIpparameter(void);


/*==========================================================
函数原型:int CSTcpipGetIpparameter(IP_PARAMETER * pIpparameter);
输入参数:无
输出参数:指向IP参数的指针
返回值: 执行结果，0成功，-1失败
功能 :获取协议栈中此刻的IP信息
注意事项:
=========================================================== */
int CSTcpipGetIpparameter(IP_PARAMETER * pIpparameter);


/*==========================================================
函数原型:int CSTcpipSetIpparameter(IP_PARAMETER * pIpParameter);
输入参数:指向IP参数的指针
输出参数:无
返回值: 执行结果，0成功，-1失败
功能 :设置IP参数到协议栈中
注意事项:
=========================================================== */
int CSTcpipSetIpparameter(IP_PARAMETER * pIpParameter);


/*==========================================================
函数原型:int CSTcpipIpToStr(const char * pIp, char * pStr);
输入参数:指向IP地址字符串的指针
输出参数:指向内存块的指针
返回值: 执行结果，0成功，-1失败
功能 :将IP地址字串转化成15位字符串，用于用户显示
如:将192.168.1.1转换成192.168.001.001
注意事项:
=========================================================== */
int CSTcpipIpToStr(const char * pIp, char * pStr);


/*==========================================================
函数原型:int CSTcpipStrToIp(const char * pStr, char * pIp);
输入参数:指向IP地址字符串的指针
输出参数:指向内存块的指针
返回值: 执行结果，0成功，-1失败
功能 :将15位字符串转化成IP地址字串，用于返回给协议栈
如:将192.168.001.001转换成192.168.1.1
注意事项:
=========================================================== */
int CSTcpipStrToIp(const char * pStr, char * pIp);


/*==========================================================
函数原型:int CSTcpipCalculateMask(const char * pIp, char * pMask);
输入参数:指向IP地址字符串的指针
输出参数:指向内存块的指针
返回值: 执行结果，0成功，-1失败
功能 :计算IP地址的子网掩码，此函数只能计算标准网络掩码
如:输入192.168.1.1输出255.255.255.000
注意事项:
=========================================================== */
int CSTcpipCalculateMask(const char * pIp, char * pMask);



/*==========================================================
函数原型:int CSTcpipCalculateMask(const char * pIp, char * pMask);
输入参数:指向IP地址字符串的指针
输出参数:指向内存块的指针
返回值: 执行结果，0成功，-1失败
功能 :计算IP地址的网关，由于网管无法计算，此接口存在局限性
如:输入192.168.1.1输出192.168.1.254
注意事项:真正网关需要由用户设置
=========================================================== */
int CSTcpipCalculateGateway(const char * pIp, char * pGateway);


/*==========================================================
函数原型:int CSTcpipGetVersion(char * pcVersion, int nSize);
输入参数:内存块的长度
输出参数:指向存储版本号的内存块指针
返回值: 执行结果，0成功，-1失败(参数错误导致失败)
功能 :获取TCPIP封装模块版本信息
注意事项:如果返回失败，请调大内存块大小
=========================================================== */
int CSTcpipGetSetupVersion(char * pcVersion, int nSize);


/*==========================================================
函数原型:int CSTcpipGetCompileTime(char * pcCompileTime, int nSize);
输入参数:编译时间 的字符串，字符串的长度
输出参数:指向存储编译日期的指针
返回值: 执行结果，0成功，-1失败(参数错误导致失败)
功能 :获取编译日期
注意事项:如果返回失败，请调大内存块大小
=========================================================== */
int CSTcpipGetSetupCompileTime(char * pcCompileTime, int nSize);


/*******************************Intergration developer use***********************/
/*==========================================================
函数原型:void CSTcpipReadIpparameter(void);
输入参数:无
输出参数:无
返回值: 无
功能 :从永久记忆体中读取IP参数到内存中
注意事项:此接口一般不需调用，调试时可能需要
=========================================================== */
void CSTcpipReadIpparameter(void);


/*==========================================================
函数原型:BOOL CSTcpipConnectionOK(void);
输入参数:无
输出参数:无
返回值: BOOL值，真表示连接正常，假表示连接不正常
功能 :获取此刻网络连接状况，建议进入双向应用前先做判断
注意事项:此接口实际会检测是否获取到IP，物理连接是否正常
=========================================================== */
BOOL CSTcpipConnectionOK(void);


/*==========================================================
函数原型:BOOL CSTcpipConnectionOKCM(BOOL bCheckCMStatus);
输入参数:BOOL值，真表示需要检查CM状态，假不检查
输出参数:无
返回值: BOOL值，真表示连接正常，假表示连接不正常
功能 :获取此刻网络连接状况，建议进入双向应用前先做判断
注意事项:此接口实际会检测是否获取到IP，根据输入检测CM是否上线
****注意:此接口适用于带有CM的双向机顶盒
=========================================================== */
BOOL CSTcpipConnectionOKCM(BOOL bCheckCMStatus);


/*==========================================================
函数原型:int CSTcpipGetIpaddr(char * pIpaddr, int nAddrLen);
输入参数:内存块的长度
输出参数:此刻协议栈中使用的IP地址
返回值: 获取成功返回0，否则返回-1
功能 :获取此刻协议栈中使用的IP地址
注意事项:
=========================================================== */
int CSTcpipGetIpaddr(char * pIpaddr, int nAddrLen);


/*==========================================================
函数原型:int CSTcpipGetMask(char * pIpaddr, int nAddrLen);
输入参数:内存块的长度
输出参数:此刻协议栈中使用的子网掩码
返回值: 获取成功返回0，否则返回-1
功能 :获取此刻协议栈中使用的子网掩码
注意事项:
=========================================================== */
int CSTcpipGetMask(char * pIpaddr, int nAddrLen);

/*==========================================================
函数原型:int CSTcpipGetCmMac(BYTE * pCmMac, int nCmMacLen);
输入参数:CableModelm  MAC 地址的长度
输出参数:CableModelm 的MAC地址
返回值: 获取成功返回0，否则返回-1
功能 :获取CM的MAC地址
注意事项:
=========================================================== */
int CSTcpipGetCmMac( BYTE  * pCmMac, int nCmMacLen);


/*==========================================================
函数原型:int CSTcpipGetGateway(char * pIpaddr, int nAddrLen);
输入参数:内存块的长度
输出参数:此刻协议栈中使用的网管
返回值: 获取成功返回0，否则返回-1
功能 :获取此刻协议栈中使用的网管
注意事项:
=========================================================== */
int CSTcpipGetGateway(char * pIpaddr, int nAddrLen);


/*==========================================================
函数原型:int CSTcpipGetMainDNS(char * pDNSaddr, int nAddrLen);
输入参数:内存块的长度
输出参数:此刻协议栈中使用的主DNS
返回值: 获取成功返回0，否则返回-1
功能 :获取此刻协议栈中使用的主DNS
注意事项:
=========================================================== */
int CSTcpipGetMainDNS(char * pDNSaddr, int nAddrLen);


/*==========================================================
函数原型:int CSTcpipGetBackDNS(char * pDNSaddr, int nAddrLen);
输入参数:内存块的长度
输出参数:此刻协议栈中使用的备用DNS
返回值: 获取成功返回0，否则返回-1
功能 :获取此刻协议栈中使用的备用DNS
注意事项:
=========================================================== */
int CSTcpipGetBackDNS(char * pDNSaddr, int nAddrLen);


/*==========================================================
函数原型:int CSTcpipGetExServer(int nIndex, char * pServerIP, int nSize);
输入参数:内存块的长度，扩展服务器的索引
输出参数:扩展服务器的地址
返回值: 获取成功返回0，否则返回-1
功能 :获取扩展服务器的地址
注意事项:
=========================================================== */
int CSTcpipGetExServer(int nIndex, char * pServerIP, int nSize);


/*==========================================================
函数原型:int CSTcpipGetHttpServer(char * pServerIP, int nSize);
输入参数:内存块的长度
输出参数:HTTP服务器的地址
返回值: 获取成功返回0，否则返回-1
功能 :获取HTTP服务器的地址
注意事项:
=========================================================== */
int CSTcpipGetHttpServer(char * pServerIP, int nSize);


/*==========================================================
函数原型:int CSTcpipGetVodServer(char * pServerIP, int nSize);
输入参数:内存块的长度
输出参数:VOD服务器的地址
返回值: 获取成功返回0，否则返回-1
功能 :获取VOD服务器的地址
注意事项:
=========================================================== */
int CSTcpipGetVodServer(char * pServerIP, int nSize);




/******************************Twoway developer use***************************/
/*==========================================================
函数原型:int CSTcpipIpconfig(char * pIpAddr, char * pMask, char * pGateway);
输入参数:IP地址，子网掩码，网关
输出参数:无
返回值: 执行结果，0成功，-1失败
功能 :设置IP地址，子网掩码，网关到协议栈中
注意事项:
=========================================================== */
int CSTcpipIpconfig(char * pIpAddr, char * pMask, char * pGateway);


/*==========================================================
函数原型:int CSTcpipIpconfig(char * pIpAddr, char * pMask, char * pGateway);
输入参数:IP模式
输出参数:无
返回值: 执行结果，0成功，-1失败
功能 :设置获取IP的方式DHCP或手动设置
注意事项:
=========================================================== */
int CSTcpipSetIPMode(IP_SET_MODE mode);


/*==========================================================
函数原型:int CSTcpipIpconfig(char * pIpAddr, char * pMask, char * pGateway);
输入参数:IP模式
输出参数:无
返回值: 执行结果，0成功，-1失败
功能 :设置获取IP的方式DHCP或手动设置
注意事项:
=========================================================== */
int CSTcpipSetIPModeExtra(IP_SET_MODE mode);


/*==========================================================
函数原型:DWORD CSTcpipGetIpMode(void);
输入参数:获取此时IP模式
输出参数:无
返回值: 执行结果，0成功，-1失败
功能 :获取IP的方式DHCP或手动设置
注意事项:
=========================================================== */
DWORD CSTcpipGetIpMode(void);




/*==========================================================
函数原型:DWORD CSTcpipGetIpMode(void);
输入参数:获取此时IP模式
输出参数:无
返回值: 执行结果，0成功，-1失败
功能 :获取IP的方式DHCP或手动设置
注意事项:
=========================================================== */
DWORD CSTcpipGetIpModeExtra(void);


/*==========================================================
函数原型:BOOL CSTcpipGetHaveIp(void);
输入参数:无
输出参数:无
返回值: BOOL值，获取到IP返回真，否则返回假
功能 :查询是否获取到IP
注意事项:
=========================================================== */
BOOL CSTcpipGetHaveIp(void);


/*==========================================================
函数原型:BOOL CSTcpipGetEthStatus(void);
输入参数:无
输出参数:无
返回值: BOOL值，物理连接正常返回真，否则返回假
功能 :查询物理连接是否正常
注意事项:
=========================================================== */
BOOL CSTcpipGetEthStatus(void);


/*==========================================================
函数原型:int CSTcpipGetCMState(void);
输入参数:无
输出参数:无
返回值: CM的连接状态
功能 :查询CM的连接状态
注意事项:
=========================================================== */
CM_STATUS CSTcpipGetCMState(void);


/*==========================================================
函数原型:BOOL CSTcpipGetEthStatus(void);
输入参数:无
输出参数:无
返回值: 无
功能 :重新获取IP，此动作会释放当前IP，重新去获取
注意事项:
=========================================================== */
void CSTcpipStartDHCP(void);

/*==========================================================
函数原型:void CSTcpipSetRestartDHCP(BOOL bFlag);
输入参数:是否重新启动dhcp标志位
输出参数:无
返回值: 无
功能 :设置是否重新启动dhcp
注意事项:
=========================================================== */
void CSTcpipSetRestartDHCP(BOOL bFlag);



/*==========================================================
函数原型:BOOL CSTcpipGetRestartDHCP(void);
输入参数:无
输出参数:无
返回值: 获取是否重新启动 dhcp的标志位true or false
功能 :获取是否重新启动 dhcp的标志位
注意事项:
=========================================================== */
BOOL CSTcpipGetRestartDHCP(void);



/*==========================================================
函数原型:void CSSetCMIp(char* pcIpAddr,int nLen);
输入参数:
pcIpAddr:  CM的ip地址
nLen:      ip地址长度
输出参数:无
返回值: 无
功能 :设置CM的IP地址
注意事项:
=========================================================== */
void CSSetCMIp(char* pcIpAddr,int nLen);




/*==========================================================
函数原型:void CSGetCMIp(char* pcIpAddr,int nLen);
输入参数:
nLen: ip地址长度
输出参数:
pcIpAddr:保存获取的CM的IP地址
返回值: 无
功能 :获取CM的IP地址
注意事项:
=========================================================== */
void CSGetCMIp(char* pcIpAddr,int nLen);


/**
@brief 注册CM状态检测的回调函数

@param[in] fnCallback 回调函数指针，详见FOnCMStatusCallback定义
@param[in] dwUserData 用户数据
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE。
@note 回调函数句柄和用户数据唯一确定一个回调函数，当两次注册它们完全一样时，第二次将返回CSUDI_FALSE,可以添加多个回调  
*/
BOOL CSCMAddCallback(DWORD dwUserData, FOnCMStatusCallback fnCallback);


/**
@brief 删除CM状态的回调函数

@param[in] fnCallback 回调函数指针，详见FOnCMStatusCallback定义
@param[in] dwUserData 同CSCMAddCallback的dwUserData
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE。
*/
BOOL CSCMRemoveCallback(DWORD dwUserData, FOnCMStatusCallback fnCallback);


/**
@brief 获取CM下行频点

@param[out] puCmDownFreq 存放当前下行频点,单位为Hz
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE，且puCmDownFreq的内容未定义。
*/
BOOL CSCMGetDownFrequency(unsigned int * puCmDownFreq); 


/**
@brief 获取CM下行信号强度

@param[out] puCmDownLevel 存放当前下行信号强度, 将结果除以10以后，单位为dBmv
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE，且puCmDownLevel的内容未定义。
*/
BOOL CSCMGetDownLevel(unsigned int * puCmDownLevel) ; 


/**
@brief 获取CM下行调制方式

@param[out] pcDownModulation 存放当前下行信号强度,*pcDownModulation值的对应关系
		    unknown(1)，
		    other(2)，
		    QAM64(3)，
		    QAM256(4)
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE。
*/
BOOL CSCMGetDownModulation(unsigned int * puCmDownModulation);


/**
@brief 获取CM下行信噪比

@param[out]  puCmDownSNR 存放当前下行信噪比,将结果除以10以后的单位为dB
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE，且puCmDownSNR的内容未定义。。
*/
BOOL  CSCMGetDownSNR(unsigned int * puCmDownSNR);  


/**
@brief 获取CM上行频点

@param[out] puCmUpFreq 存放当前上行频点，单位为Hz
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE，且puCmUpFreq的内容未定义。
*/
BOOL  CSCMGetUpFrequency(unsigned int * puCmUpFreq);  


/**
@brief 获取CM上行信号强度

@param[out] puCmUpLevel 存放当前上行信号强度,结果除以10以后单位为dBmv
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE，且puCmUpLevel的内容未定义。
*/
BOOL  CSCMGetUpLevel(unsigned int * puCmUpLevel); 


/**
@brief 获取CM上行调制方式

@param[out] puCmUpModulation 上行调制方式,值为0代表无上行调制方式
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE。
*/
BOOL CSCMGetUpModulation(unsigned int *puCmUpModulation);


/**
@brief 获取CM的MAC层工作方式

@param[out] puCmMACMode 存放MAC层工作方式
@return 成功返回CSUDI_TRUE；失败则返回CSUDI_FALSE。
*/
BOOL CSCMGetMACLayerMode(unsigned int *puCmMACMode);


#ifdef __cplusplus
}
#endif

#endif

/**********************************EOF**********************************/

