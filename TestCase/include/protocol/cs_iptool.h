#ifndef _CS_IPTOOL_H_
#define _CS_IPTOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "udi2_socket.h"

#define MAX_TTL		256
#define HOST_NAME_LEN 64
typedef enum
{
    EM_DNS_PRIORITY = 0,//采用优先策略，即如果有多个DNS服务器，则优先访问上一次解析出了域名的那台服务器
    EM_DNS_UNPRIORITY,//不采用优先策略，每次从第一个DNS访问
    EM_DNS_UNDEFINED
} CSDNSStrategyType_E;

typedef struct 
{
    char acHost[HOST_NAME_LEN];		 ///< 用于ping诊断的主机名或地址；
    int nNumberOfRepetitions;		 ///< 在报告结果之前，ping诊断重复的次数；
	int nTimeout;					 ///< 用毫秒表示的ping诊断超时时间；
	int nDataBlockSize;			     ///< 每个ping命令发送的数据大小，以字节为单位;
	int nTTL;						 ///< TTL值，即生存时间;
	int nDSCP;					     ///< 测试包中用于DiffServ的码点，默认值为0；
}CSPINGAttribute_S;

typedef struct 
{
	char acHost[HOST_NAME_LEN];			 ///< 应答ping诊断的主机地址；
	int nTotalCount;				     ///< 在最近的ping测试中的总次数；
	int nSuccessCount;			         ///< 在最近的ping测试中成功的次数；
	int nFailureCount;				     ///< 在最近的ping测试中失败的次数；
	int nAverageResponseTime;	         ///< 以毫秒为单位的最近一次ping测试所有成功响应的平均时间；
	int nMinimumResponseTime;	         ///< 以毫秒为单位的最近一次ping测试所有成功响应中的最短时间；
	int nMaximumResponseTime;	         ///< 以毫秒为单位的最近一次ping测试所有成功响应中的最长时间；
}CSPINGResult_S;


typedef struct 
{
	char acHost[HOST_NAME_LEN];		  ///< 用于路由诊断的主机名或地址；
	int nTimeout;					  ///< 用毫秒表示的路由诊断超时时间；
	int nDataBlockSize;			      ///< 每个路由诊断发送的数据大小;
	int nMaxHopCount;			      ///< 发送的测试数据包的最大跳数(最大TTL数)，默认为30跳；
	int nDSCP;					      ///< 测试包中用于DiffServ的码点，默认值为0；
}CSTRACERTAttribute_S;


typedef struct 
{
	int nTTL;            ///< 生存时间
	char acHopHost[HOST_NAME_LEN];  ///< 路由过程中经过的主机IP地址
}CSTRACERTHopHost_S;


typedef struct 
{
	char acHost[HOST_NAME_LEN];					    ///< 用于路由诊断的主机地址；
	int nResponseTime;			            ///< 以毫秒表示的最近一次路由主机测试的响应时间，如果无法决定具体路由，则默认为0；
	int nNumberOfRouteHops;		            ///< 用于发现路由的跳数，如果无法决定路由，则默认为0；
	CSTRACERTHopHost_S gHopHost[MAX_TTL];	///< 路由路径；
}CSTRACERTResult_S;


/**@brief ping回调消息类型定义*/
typedef enum 
{
    EM_PING_SUCCESS = 0,     ///< ping成功
    EM_PING_TIMEOUT,          ///< ping超时
    EM_PING_FAILURE            ///< ping 未知原因失败
}CSPingState_E; 


/**
@brief Ping事件回调函数原型定义

@param[in] ePingState 产生回调的ping的状态类型,详细见CSPingState_E的说明
@param[in] pBackStr 回调消息数据，ping的结果
@param[in] pUserData 用户自定义数据
*/
typedef BOOL (*CSOnPingStateChange_F)(CSPingState_E ePingState, char *pcBackStr,void *pUserData);


 /**
@brief 注册ping模块回调

@param[in] dwUserData  保存用户数据信息。
@param[in] fnOnPingStateChange  回调函数句柄。
@return 如果成功返回CSUDI_TRUE, 失败CSUDI_FALSE
@note
- 回调函数的注册个数至多支持20个
*/
BOOL CSPingAddCallback(DWORD dwUserData, CSOnPingStateChange_F fnOnPingStateChange);


 /**
@brief 删除指定的ping 回调函数的注册

@param[in] dwUserData 用户数据信息。
@param[in] fnOnPingStateChange 回调函数句柄。
@return成功返回CSUDI_TRUE 如果该回调函数根本就没有注册，将返回CSUDI_FALSE
*/
BOOL CSPingRemoveCallback(DWORD dwUserData, CSOnPingStateChange_F fnOnPingStateChange);


/**
@brief 测试到达另外一台主机经过的路径及路由器的跳数。

@param[in] psTracertAttribute,用来存放tracert所需要的参数
@param[out] psTracertResult ,用来存放tracert的结果
@return      成功返回0，失败返回-1                      
*/
int CSTracert(CSTRACERTAttribute_S* psTracertAttribute, CSTRACERTResult_S * psTracertResult);


/**
@brief 测试另一台主机是否到达。

@param[in] psPingAttribute,用来存放ping所需要的参数
@param[out] psPingResult 用来存放ping的结果。
@return      成功返回0，失败返回-1                      
*/
int CSPingStat(CSPINGAttribute_S *psPingAttribute,CSPINGResult_S *psPingResult);


/**
@brief 测试另一台主机是否到达。

@param[in] pcPingStr 表示ping 命令行，如："ping -n 8  192.168.110.90"
@param[in] pcBackStr 返回字符串的长度，不小于512字节
@param[in] uBackStrLen 表示ping包发出后，等待应答的时间,单位ms
@param[out] uSleepTime 用来存放ping的结果。空间由用户申请，不小于512字节。结果如："Reply from 192.168.1.1,  ….. time<5ms  "
@return      成功返回0，失败返回-1                      
*/
int CSPing(char * pcPingStr,char * pcBackStr,unsigned int uBackStrLen,unsigned int  uSleepTime);
/**
@brief 测试另一台主机是否到达。

@param[in] psPingAttribute,用来存放ping所需要的参数
@param[out] phPingHandle,用来保存ping句柄
@return      成功返回0，失败返回-1                      
@note  异步实现ping操作，ping的结果如:成功或者超时，通过回调抛给应用
*/
int CSPingAsyn(CSPINGAttribute_S *psPingAttribute, HCSHANDLE *phPingHandle);


/**
@brief  取消ping操作。

@param[in] hPingHandle,ping句柄,取CSPingAsyn的输出句柄
@param[out] 如果当前无ping操作，或者ping操作已经结束，则输出0，否则输出1
@note 该接口调用后,CSPingAsyn 中调用的ping操作将被中断
@return      成功返回0，失败返回-1              
*/
int CSCancelPing(HCSHANDLE hPingHandle,int * pnState);


/**
@brief 检查本机IP地址是否与其它机器有冲突

@param[in]  pcEthName 网卡设备名，如："eth0"  
@param[in]  uInAddr 被检查IP地址，网络字节序地址
@param[in]  uTimeOut 等待arp应答的时间，单位ms  
@return      成功返回0，失败返回-1                      
*/
int  CSArpSniffer(const char * pcEthName, const unsigned int uInAddr, const unsigned int uTimeOut);


/**
@brief 设置CSDNSGetHostByName函数的默认超时时间，单位ms

@param[in]  dwTimeout  CSDNSGetHostByName函数的默认超时时间，单位ms. 可根据环境的实际情况设置合理超时时间
@return 设置成功则返回0,如果dwTimeout = 0, 则按照内部默认超时处理并返回-1
*@note 内部 默认超时为2000ms  
*/
int CSDNSSetDefaultTimeout(unsigned int dwTimeout) ;


/**
@brief 域名解析，与gethostbyname不同的是可以设置超时，对应每个域名服务器

@param[in] pcName  要解析的域名, 如"www.baidu.com",也可以是IP地址
@param[in] dwTimeout  解析超时时间, 单位ms  
@return  解析成功则返回结果CSUDIHostent_S*，否则返回CSUDI_NULL
*@note   dwTimeout 为等待一个DNS服务器应答的时间，如果设置了n个DNS服务器，则最多等待的时间为n*dwTimeout(ms)
*/
CSUDIHostent_S * CSDNSGetHostByNameWithTimeout(const char* pcName,unsigned int dwTimeout);


/**
@brief 域名解析，使用CSDNSSetDefaultTimeout 设置的默认超时

@param[in] pcName  要解析的域名, 如"www.baidu.com",也可以是IP地址
@return  解析成功则返回结果CSUDIHostent_S*，否则返回CSUDI_NULL
*@note   除了超时时间使用默认超时以外，其他同CSDNSGetHostByNameWithTimeout
*/
CSUDIHostent_S *  CSDNSGetHostByName(const char* pcName); 


/***********************************************************
* 函数: CSDNSGetHostByNameR
* 功能: 使用CSDNSSetDefaultTimeout 设置的默认超时
* 输入:   
*     name :  要解析的域名, 如"www.baidu.com",也可以是IP地址
*     
*     buf:   指向一个buf空间，解析后的数据存放在buf中
*     buflen: buf 大小 
* 
*  注意:
*     设置了n个DNS服务器，则最多等待的时间为n*所设置的默认超时时间
*     除超时时间外，其他的同CSDNSGetHostByNameWithTimeoutR
*          
* 输出: 
*     result_buf: 指向一个主机结构,作为输出参数
*     result:解析后的结果，获取的主机结构的指针，作为输出参数
*     h_errnop:错误代码。
* 说明:
*     result_buf和result都是作为输出参数，result_buf指向buf，
*     而result的指针指向buf，表现形式不相同，为用户提供了两种输出形式
* 返回值: 
*       解析成功0
*       否则返回错误代码
*     
************************************************************/
int CSDNSGetHostByNameR(const char * name,
                           CSUDIHostent_S * result_buf,
                           char * buf, unsigned int buflen,
                           CSUDIHostent_S ** result,
                           int * h_errnop);



/***********************************************************
* 函数: CSDNSGetHostByNameWithTimeoutR
* 功能: 域名解析，支持多任务重入,对应标准函数gethostbyname_r
* 输入:   
*     name :  要解析的域名, 如"www.baidu.com",也可以是IP地址
*     buf:   指向一个buf空间
*     buflen: buf 大小 
*     dwTimeout : 解析超时时间, 单位ms  
*     
*  注意:
*     dwTimeout 为等待一个DNS服务器应答的时间，如果
*     设置了n个DNS服务器，则最多等待的时间为n*dwTimeout(ms)
*          
* 输出: 
*     result_buf: 指向一个主机结构，解析后的数据存于buf中，指向buf
*     result:解析后的结果，其指针指向buf
*     h_errnop:错误代码
* 说明:
*     result_buf和result都是作为输出参数，result_buf指向buf，
*     而result的指针指向buf，表现形式不相同，为用户提供了两种输出形式
* 返回值: 
*       解析成功0
*       否则返回错误代码
*     
************************************************************/
int CSDNSGetHostByNameWithTimeoutR(const char * name,
                           CSUDIHostent_S * result_buf,
                           char * buf, unsigned int buflen,
                           CSUDIHostent_S ** result,
                           int * h_errnop,unsigned int dwTimeout);




/***********************************************************
* 函数: CSDNSSetCache
* 功能: 设置是否启用域名缓存机制,默认为启用域名缓存机制，启用之后，
* 只要解析过的域名，下次就不会再解析，重新开机才会再解析
* 输入: bFlag为TRUE表示 启用CACHE机制，设置为FALSE表示不使用缓存 ，设置为      
* FALSE之后，每次都会解析域名，会影响访问网页的速度         
* 输出: 无
* 说明:
* 返回值: 
*      设置成功返回0
*       否则返回错误代码
*     
************************************************************/
int CSDNSSetCache(CSUDI_BOOL bFlag);

/***********************************************************
* 函数: CSDNSSetStrategyType
* 功能: 设置DNS的访问策略，如果有多个DNS，则设置不同的访问DNS服务器策略
* 输入: eDNSStrategyType 代表访问DNS服务器的策略类型
* EM_DNS_PRIORITY表示优先访问上一次解析出来域名的DNS服务器;
* EM_DNS_UNPRIORITY表示不管之前解析域名情况如何，每次都从第一个DNS访问,
* 默认采用EM_DNS_PRIORITY方式   
* 输出: 无
* 说明:
* 返回值: 
*      设置成功返回0
*       否则返回-1
*     
************************************************************/
int CSDNSSetStrategyType(CSDNSStrategyType_E eDNSStrategyType);


#ifdef __cplusplus
}
#endif

#endif

