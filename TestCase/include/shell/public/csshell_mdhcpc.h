/**@defgroup SHELLMDHCP 本模块对DHCP对外接口和数据结构进行了定义
@brief MDHCP模块对应的头文件<csshell_mdhcpc.h>,定义了DHCP常用操作接口，支持DHCP RFC2131,
RFC2132和RFC1533标准

1、本接口支持多实例，网卡设备名称作为唯一标识符
2、CSMDHCPInit函数会创建一个实例，所有的接口的调用必须在CSMDHCPInit函数之后
3、本模块的实现依据RFC2131、RFC1533和RFC2132，获取到IP地址、子网掩码和网关
4、本模块所除常见option之外，还支持几种特殊的option选项:

        操作码           描述                                                     用途      
        
        60                      Class-identifier                                         用来做终端、服务端双方校验、DHCP服务器划分子网等
        43                      Vendor specific information                         下发某个服务器地址,用来收视率调查等
        151-155              private                                                     Coship标准，用来存放应用下载服务器地址，现在用于广东省网、天威
        125                     Vendor-Identifying Vendor-Specific.             终端对服务器端进行校验
如果客户需要支持上面描述的这几个option选项，必须调用CSMDHCPSetSendOption来设置，如果需要客户端也验证，则还需要
调用CSMDHCPSetRecvOption来设置客户端校验字段

5、常见option对照表，这些option不需要用户设置，DHCP 模块发包就会带上这些option

        操作码     			      描述                                                           
		1			 		      掩码
		3						网关
		6						DNS服务器
		51						DHCP租期信息
		53						DHCP服务器 IP地址
		55						DHCP 请求选项列表
5、用户从自动获取ip地址切换到静态设置IP地址，必须调用CSMDHCPStop来停止DHCP后再设置静态IP地址
6、用户从静态IP地址切换到自动获取IP,直接调用CSMDHCPStart来获取IP地址

@version 2.0.1 2011/11/14 
@version 2.0.2 2011/12/05
@{
*/
#ifndef __SHELL_MDHCPC_H_INCLUDED__
#define __SHELL_MDHCPC_H_INCLUDED__

#include "mmcp_typedef.h"

#if defined(__cplusplus) || defined(__CPLUSPLUS)
extern "C" {
#endif 

#define MDHCP_ETH_NAME_LEN 32
#define MDHCP_SERVER_HOSTNAME_LEN 64

/**@brief  MDHCP模块消息状态枚举类型定义*/
typedef enum 
{
    EM_MDHCP_UP = 0,			///<  获取到ip地址
    EM_MDHCP_DOWN,                	///< 网卡处于down的状态 
    EM_MDHCP_INFORM,                 ///< 通知消息
    EM_MDHCP_RENEW,                  ///<正在更新状态
    EM_MDHCP_RENEWED,              ///<更新成功
    EM_MDHCP_RELEASED,             ///<IP地址被释放
    EM_MDHCP_FAILED                  ///<获取ip地址失败
}CSMDHCPEvent_E;

/**@brief  MDHCP报文类型定义*/
typedef enum
{
    EM_MDHCP_DISCOVER = 0,		     ///<  DHCPDISCOVER报文
    EM_MDHCP_REQUEST = 1              ///< DHCPREQUEST报文
}CSMDHCPPacketType_E;

/**@brief  MDHCP模块设置DHCP收发包模式*/
typedef enum
{
    EM_MDHCP_UNICAST = 0,		///<发包时，表示以单播形式发送，收包时，指明服务器以单播方式应答，此时IP分组的目的IP地址就是指派给客户的地址，因为客户没有获取到IP地址之前，还不知道它的IP地址，它可能会丢弃这个分组
    EM_MDHCP_BROADCAST = 1		///<发包时，表示以广播方式发送，收包时，指明服务器以广播方式应答，每一个主机都会接收并处理这个报文
} CSMDHCPPacketMode_E;

/**@brief  MDHCP模块回调信息结构体定义*/
typedef struct  
{
    unsigned int      uServerIp;            ///<DHCP server的IP地址,网络字节序
    unsigned int   	uClientIp;              ///<分配给客户端的 IP地址,网络字节序 
    unsigned int      uClientMast;          ///<客户端的子网掩码,网络字节序 
    unsigned int      uClientGateWay;   ///<客户端的网关,网络字节序 
    unsigned int      uDns1;                 ///<由DHCP下发的DNS 服务器地址,网络字节序
    unsigned int      uDns2;                 ///<由DHCP下发的DNS 服务器地址,网络字节序
    unsigned long  	ulLease;                ///< 分配给客户端的租约时间 ,主机字节序
} CSMDHCPClientInfo_S;

/**@brief  MDHCP模块初始化信息结构体定义*/
typedef struct  
{
	CSMDHCPPacketMode_E eServerAnswerType;  ///<设置服务器的应答模式,广播还是单播模式
	BOOL bIsEnableDNS;						///<设置是否采用DHCP服务器分配的DNS，TRUE表示采用，FALSE表示不采用
	BOOL bIsEnableGW;						///<设置是否采用DHCP服务器分配的gateway,TRUE表示采用，FALSE表示不采用
	char szServerHostName[MDHCP_SERVER_HOSTNAME_LEN];	///< "\0"结束的ASCII字符串，设置serverhostname字段,根据RFC2131标准，该字段由服务器填写，如果用户设置此字段，则在客户端收到DHCPOFFER时，会比较此字段，如果和所设置的相同则接受该offer包，否则不接受该offer包，继续发送请求
} CSMDHCPInitPara_S;

/**
@brief MDHCP获取发包时间间隔，由用户实现

@param[in] nIndex 发包次数
@note 根据传入的index,获取发包时间间隔
*/
typedef unsigned int (*CSOnMDHCPGetSendInterval_F)(unsigned int nIndex);

/**
@brief MDHCP模块事件回调函数原型定义

@param[in] pcEthName 网卡设备名称
@param[in] eEvent 回调消息，详见CSMDHCPEvent_E定义
@param[in] sClientInfo 回调消息数据，详见CSMDHCPClientInfo_S定义。
@param[in] pvUserData 用户自定义数据
*/
typedef void  (*CSOnMDHCPChange_F)(CSMDHCPEvent_E eEvent, CSMDHCPClientInfo_S *sClientInfo, void * pvUserData);

typedef struct 
{

	int m_nSize;		///<本结构大小，一定等于sizeof(CSDHCPInitParam_S)

	/**
	@brief 初始化DHCP

	@param[in] pcEthName:网卡设备名称
	@return 
	- 如果成功返回0。
	- 如果失败返回-1
	@note 该接口不会启动dhcp，仅仅是初始化动作
	*/
	int (* m_fnDHCPInit)(char * pcEthName,CSMDHCPInitPara_S *sInitPara);

	/**
	@brief 启动DHCP

	@param[in] pcEthName:网卡设备名称
	@return 
	- 如果成功返回0。
	- 如果失败返回-1
	@note 本接口异步执行，返回成功仅表示命令已接收，不表示命令执行结果。
	*/
	int (* m_fnDHCPStart)(char* pcEthname);

	/**
	@brief 停止DHCP

	@param[in] pcEthName:网卡设备名称
	@return 
	- 如果成功返回0。
	- 如果失败返回-1
	@note 本接口异步执行，返回成功仅表示命令已接收，不表示命令执行结果。
	*/
	int (* m_fnDHCPStop)(char* pcEthname);

	/**
	@brief 设置DHCP option选项,设置之后，发包就会带上所设置的选项

	@param[in] pcEthName:网卡设备名称
	@param[in] nOptCode:Option选项的操作码,操作码范围0-255，各个操作码代表的含义请参见RFC2132
	@param[in] pOptVal:Option选项的值
	@param[in] nOptLen:Option选项的长度
	@return 
	- 成功返回0。
	- 失败返回-1
	*/
	int (* m_fnDHCPSetSendOption)(char * pcEthName,  int nOptCode, void *pOptVal,int nOptLen);

	/**
	@brief 设置DHCP option选项，设置之后，客户端将会对DHCP服务器应答的数据包做相应的校验

	@param[in] pcEthName:网卡设备名称
	@param[in] nOptCode:Option选项的操作码,操作码范围0-255，各个操作码代表的含义请参见RFC2132
	@param[in] pOptVal:Option选项的值
	@param[in] nOptLen:Option选项的长度
	@return 
	- 如果成功返回0。
	- 如果失败返回-1
	*/
	int (* m_fnDHCPSetRecvOption)(char * pcEthName,  int nOptCode, void *pOptVal,int nOptLen);

	/**
	@brief 获取DHCP option选项

	@param[in] pcEthName:网卡设备名称
	@param[in] nOptCode:Option选项的操作码,操作码范围0-255，各个操作码代表的含义请参见RFC2132
	@param[out] pOptVal:用来存放获取的option 选项的值，buffer的长度不能小于256
	@param[out] nOptLen:pOptVal所指向的存储空间的大小
	@return 
	- 如果成功返回0。
	- 如果失败返回-1
	*/
	int (* m_fnDHCPGetOption)(char * pcEthName,  int nOptCode,void *pOptVal, int nOptLen);

	/**
	@brief 设置DHCP报文的发包时间间隔以及发包次数

	@param[in] pcEthName 网卡设备名称
	@param[in] ePacketType: 包类型，详见CSMDHCPPacketType_E 定义
	@param[in] fnGetSendInterval: 函数指针，获取发包时间间隔，该函数由用户实现，返回值单位:秒(s),如果不设置，将采用默认值序列发包
	@return
	- 如果成功返回0。
	- 如果失败返回-1
	@note  不设置，DHCPDISCOVER报文将采用默认时间间隔，默认时间间隔[1,2,4,8,16,32,64,128,256],循环
	*/
	int  (*m_fnDHCPSetPacketInterval)(char * pcEthName,CSMDHCPPacketType_E ePacketType,CSOnMDHCPGetSendInterval_F fnGetSendInterval);

	/**
	@brief 增加DHCP续约模式

	@param[in] pcEthName 网卡设备名称
	@param[in] nSendTime: 发送RENEW报文的时间，租约的万分比，例如:天威是75%续约，需要传7500
	@param[in] eSendType: 发包模式，详见CSMDHCPPacketMode_E定义
	@return
	- 如果成功返回0。
	- 如果失败返回-1
	@note  按照RFC2131标准，DHCP模块在50%租约发送单播包续约，续约不成功，在87.5%发送广播包续约，\n
	续约不成功，等待租约满的时候重新发送DHCPDISCOVER 报文
	@note  各个项目在标准的基础上加上了私有的发包方式，目前有两种需求
	@note  天威需求:50%续约不成功的时候，增加在75%租约的时候发送单播包续约
	@note 上海电信需求:50%续约不成功的时候，增加在68.75%发送单播包续约，87.5%续约不成功的时候，增加在93.75%发送广播包续约
	*/
	int (*m_fnDHCPAddRenewPacketSendMode)(char * pcEthName,int nSendTime,CSMDHCPPacketMode_E eSendType);

	/**
	@brief  获取DHCP服务器主机名字段

	@param[in] pcEthName 网卡设备名称
	@param[out] pucServerHostName: 用户设置的服务器主机名字段的值，长度不超过64字节
	@param[in] dwLen pucServerHostName所指向的存储空间的大小
	@return 
	- 成功返回0
	- 失败返回-1
	@note:有的局方将VOD服务器的地址放在这个字段中下发下来，应用需要调用该接口获取vod服务器的地址去点播
	*/
	int (* m_fnDHCPGetServerHostName)(char * pcEthName,BYTE *pucServerHostName,DWORD dwLen );

	/**
	@brief 添加DHCP通知回调函数

	@param[in] fnCallback 回调函数,当DHCP状态发生变化时调用该函数
	@param[in] pvUserData 用户数据,将作为回调函数的pvUserData参数返回
	@return 
	- 如果成功返回0。
	- 如果失败返回-1
	@note 添加回调前产生的DHCP状态变化不会补充通知
	*/
	int (* m_fnAddDHCPListener)(char * pcEthName,CSOnMDHCPChange_F pCallback,void* pvUserData);

	/**
	@brief 删除DHCP通知回调函数

	@param[in] fnCallback 回调函数,当DHCP状态发生变化时调用该函数
	@param[in] pvUserData 用户数据,将作为回调函数的pvUserData参数返回
	@return 
	- 如果成功返回0。
	- 如果失败返回-1
	*/
	int (* m_fnRemoveDHCPListener)(char * pcEthName,CSOnMDHCPChange_F pCallback);
}CSMDHCPInitParam_S;

/**
@brief  DHCP 注册接口
@param psParam  DHCP  Shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSMDHCPSetParam(CSMDHCPInitParam_S * psParam);

/**
@brief 获取 获取 接口参数
@param 无
@成功返回CSMDHCPInitParam_S,失败返回NULL
*/
const CSMDHCPInitParam_S* CSMDHCPGetParam();


#if defined(__cplusplus) || defined(__CPLUSPLUS)
}
#endif

#endif 

