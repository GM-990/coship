/**@defgroup UDI2PPPOE  PPPOE 本模块主要定义了PPPOE相关的接口，包括pppoe的初始化、启动、停止等
@brief PPPOE模块对应头文件<udi2_pppoe.h>，包括了PPPOE的初始化、连接、断开等接口 。
pppoe主要提供通过拨号，获取ip地址，客户使用pppoe方式上网，可以实现流量控制、流量计费等功能

如果机顶盒采用pppoe方式进行拨号上网，则要用到我们这个模块来进行拨号上网，pppoe使用非常简单，在应用
集成的时候，只需要先调用CSUDIPPPOEInit来初始化pppoe模块，CSUDIPPPOEInit主要传递一些初始化的参数，所传的
参数在CSUDIPPPOEInitPara_S结构体中有定义，包括上一次成功获取ip地址后获取的访问集中器的mac地址，会话id，这两个参数
在首次进行pppoe连接的时候可以传空，后面再连接的时候，就输入上次连接成功之后保存的mac值和sessionid值
初始化pppoe模块之后，调用CSUDIPPPOEConnect就可以进行拨号连接，拨号连接要传入用户名和密码，拨号成功就会给机顶盒分配一个
IP地址，机顶盒就可以用这个IP地址畅游网络了。

在每次拨号成功之后，用户应该把AC的mac地址和会话id保存在flash中，AC的mac地址会以回调的形式通知给用户，用户接收到CSUDIPPPOEMsgType_E
中定义的PPPOE_STATE_RUNNING消息，表示获取到了ip地址，然后用户可以从回调上来的参数中保存AC的mac地址和本次的sessionid到flash中，下次开机
直接从flash中读出该值传入初始化模块，这样就会加快以后拨号的速度

拨号之后的状态可以通过CSUDIPPPOEGetState来获取，是CSUDIPPPOEState_E中定义的那几种状态类型

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.2 2010/10/13 第一次综合评审后修改的版本
@version 2.0.1 2009/07/24 创建
@{
*/

#ifndef _UDI2_PPPOE_H_
#define _UDI2_PPPOE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifndef CSUDI_PPPOE_IP4_LEN
#define CSUDI_PPPOE_IP4_LEN  16
#endif


#ifndef CSUDI_PPPOE_MAC_LEN
#define CSUDI_PPPOE_MAC_LEN	6	
#endif

/**@brief PPPOE 相关错误代码值*/
enum 
{
	CSUDIPPPOE_ERROR_BAD_PARAMETER = CSUDI_PPPOE_ERROR_BASE,    ///<  非法参数,当输入参数不符合平台接口定义的参数范围 或其他规定，通常返回该错误号;                       
    CSUDIPPPOE_ERROR_NOT_SUPPORTED,   ///< 不支持操作
	CSUDIPPPOE_ERROR_UNKNOWN_ERROR	, 	///< 未知原因失败(始终为1)  
	CSUDIPPPOE_ERROR_TIMEOUT	, 	///<  超出超时时间限制错误  
	CSUDIPPPOE_ERROR_FATAL  , 	///< 致命错误,通常是在调用操作系统API或CSIP平台OsPorting API函数失败时，需要给上层返回该错误号
	CSUDIPPPOE_ERROR_EXIST	,	///< 资源、文件已经存在或模块已经加载,通常是调用模块 初始化函数(CSUDIPPPOE_XXX_Init)或资源创建函数	 (CSUDIPPPOE_XXX_CreateYYY)失败时返回   
	CSUDIPPPOE_ERROR_NOTEXIST	,	///< 资源、文件不存在或模块已卸载,通常是调用模块反初始化函数(CSUDIPPPOE_XXX_Cleanup)或资源销毁函数(CSUDIPPPOE_XXX_DestoryYYY)失败时返回
	CSUDIPPPOE_ERROR_AUTHFAILURE,       ///< 认证失败
	CSUDIPPPOE_ERROR_OTHER,              ///< 其他错误
	CSUDIPPPOE_ERROR_CALLBACK_ALREADY_ADDED,      ///<回调已经添加
    CSUDIPPPOE_ERROR_CALLBACK_NOT_FOUND      ///<回调函数不存在
};


/**@brief 网卡信息*/
typedef struct
{
    char acACMac[CSUDI_PPPOE_MAC_LEN];              ///<  MAC of AC ，例如 {0x00,0x0e,0x09,0x11,0x22,0x33} ,每次拨号成功之后，将AC的mac地址保存在flash中，以后开机拨号从flash中读取该值，传入初始化函数中
    unsigned short usSessionId;        ///<  PPPoE 会话id，每次获取到ip地址之后，将那次的会话id保存在flash中，以后再开机拨号就从flash中读取会话id，传入初始化模块，加快拨号速度
    int nStackSize;                   ///<  pppoe线程堆栈大小. 
    int nPriority;                    ///<  pppoe线程优先级
}CSUDIPPPOEInitPara_S;


/**@brief pppoe获取到的地址相关信息*/
typedef struct               ///<该结构体由回调函数抛出
{
	int nState;                        ///< pppoe state 其状态可以取得的值为CSUDIPPPOEState_E类型中的值
	int nPID;                          ///< pppd 进程的pid值 
	char acLocalIP[CSUDI_PPPOE_IP4_LEN];  	///< 获取到的ip地址, a string ended with '\0', as "255.255.255.255" 		
	char acRemoteIP [CSUDI_PPPOE_IP4_LEN];     ///< 服务器的ip地址, a string ended with '\0',as "255.255.255.255" 
	char acMask[CSUDI_PPPOE_IP4_LEN];          ///<获取到的子网掩码
	char acGateWay[CSUDI_PPPOE_IP4_LEN];        ///<获取到的网关 
	char acDNS1[CSUDI_PPPOE_IP4_LEN];           ///<获取到的第一个dns
	char acDNS2[CSUDI_PPPOE_IP4_LEN];           ///<获取到的第二个dns
	char acACMac[CSUDI_PPPOE_MAC_LEN];         ///< MAC of AC "FFFFFFFFFFFF"，需要保存到flash中，在初始化函数中传入该值
	unsigned short usSessionId;                 ///<  pppoe本次的会话id ，需要保存到flash中，在初始化函数中传入该值
} CSUDIPPPOECallBackPara_S;


/**@brief pppoe的几种状态*/
typedef enum 
{
	EM_UDIPPPOE_PHASE_DEAD = 0, ///<  link is dead 
	EM_UDIPPPOE_ESTABLISH,      ///< 正在建立连接 
	EM_UDIPPPOE_AUTHENTICATE,    ///<  正在认证 
	EM_UDIPPPOE_NETWORK,        ///< 正在获取ip地址 
	EM_UDIPPPOE_RUNNING,        ///<  获取到ip地址
	EM_UDIPPPOE_AUTH_FAILED ,        ///<  认证失败
	EM_UDIPPPOE_SESSIONID_CHANGED,     ///<  session ID 发生改变 
	EM_UDIPPPOE_SERVER_NORESPONSE,   ///<  服务器无响应 
	EM_UDIPPPOE_TIMEOUT ,             ///<  超时 
	EM_UDIPPPOE_CLEAR_TO_INIT,   ///<清除状态至无连接状态
	EM_UDIPPPOE_TERMINATEBYPEER               ///<PPPOE客户端被踢出
} CSUDIPPPOEState_E;


///<添加多回调处理函数，调用CSUDIPPPOEAddCallback传递下面这种类型的callback,nState为CSUDIPPPOEState_E类型，pPara为CSUDIPPPOECallBackPara_S*类型
typedef void  (*CSUDIPPPOECallback_F)(CSUDIPPPOEState_E nState, void *pPara, void *pvUserData);


/**
@brief 初始化pppoe模块

@param[in] psInitPara 初始化时需要传入的参数,为CSUDIPPPOEInitPara_S类型,堆栈大小和优先级传0的话我们的模块内部会取默认值
@return CSUDI_SUCCESS表示初始化成功，其他表示失败，失败类型见错误码值
@note  ACMAC和会话id从flash中读出(每次连接成功，要将这两个值保存在flash中)，第一次ACMAC传全FF,sessionid传0
*/
CSUDI_Error_Code  CSUDIPPPOEInit(CSUDIPPPOEInitPara_S *psInitPara);


/**
@brief 销毁pppoe模块

@param[in] nFlag kill掉pppd进程所采用的方式，gentle或者rude方式,0为gentle方式，1为rude方式，0表示杀掉该进程即可，1表示杀掉该进程及所有的子进程
@return CSUDI_SUCCESS表示销毁成功，其他表示失败，失败类型见错误码值
@note 该函数销毁了所有的pppoe的资源，如果要重新连接，需要重新初始化后再建立连接
*/
CSUDI_Error_Code  CSUDIPPPOECleanup (int nFlag);


/**
@brief 发起连接

@param[in] pcUserName 用户名
@param[in] pcPasswd 密码
@param[in] pcServiceName pppoe服务名称
@param[in] pcEthName 物理网卡名称，如"eth0"
@param[in] nTimeout 连接超时时间，单位:秒
@return CSUDI_SUCCESS表示连接成功，其他表示失败，失败类型见错误码值
@note 连接成功之后，获取到的ip地址、子网掩码和网关、dns等信息会以回调的形式通知给用户
*/
CSUDI_Error_Code  CSUDIPPPOEConnect(const char *pcUserName, const char *pcPasswd, char *pcServiceName, char *pcEthName, int nTimeout);


/**
@brief 断开连接

@param[in] nTimeout 断开连接的超时时间，单位:秒
@return CSUDI_SUCCESS表示断开连接成功，其他表示失败，失败类型见错误码值
@note 需要断开pppoe连接，可以调用该函数，以后再建立连接就直接调用CSUDIPPPOEConnect
*/
CSUDI_Error_Code  CSUDIPPPOEDisconnect(int nTimeout );


/**
@brief 获取pppoe服务名称

@param[out] pcSevicename 用来保存获取的pppoe服务名称
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
*/
CSUDI_Error_Code  CSUDIPPPOEGetServiceName( char *pcSevicename);


/**
@brief 获取pppoe状态

@param[out] pnState 用来保存获取的pppoe的状态,状态的取值为CSUDIPPPOEState_E中定义的几种值
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
*/
CSUDI_Error_Code  CSUDIPPPOEGetState(int *pnState);


/**
@brief 获取AC的MAC地址

@param[out] pcMAC 用来保存获取的AC的MAC地址
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
@note ac的mac地址的长度为6个字节，传入的buffer的空间要大于6个字节
*/
CSUDI_Error_Code  CSUDIPPPOEGetACMacAddr(char *pcMAC);


/**
@brief 获取pppoe会话id

@param[out] pusSessionID 用来保存获取pppoe的会话id
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
@note 每次连接成功都会产生一个会话id，保存该会话id，下次连接的时候直接传入，会加快连接速度
*/
CSUDI_Error_Code  CSUDIPPPOEGetSessionID(unsigned short *pusSessionID);


/**
@brief 获取pppoe版本号

@param[out] pcVer 用来保存获取pppoe的版本号
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
*/
CSUDI_Error_Code  CSUDIPPPOEGetVersion( char *pcVer);


/**
@brief 添加一个pppoe回调

@param[in] pvUserData 保存用户数据信息,将作为回调函数的dwUserData参数返回
@param[in] fnCallback 要添加的回调，当pppoe状态发生变化的时候会调用该函数
@return CSUDI_SUCCESS表示添加回调成功，其他表示失败，失败类型见错误码值
@note  可以添加多个回调 ,要求 pfCallback和dwUserData都相同的时候才不会添加进去，否则都添加
@note pppoe模块支持注册最小8个回调函数
*/
CSUDI_Error_Code CSUDIPPPOEAddCallback(CSUDIPPPOECallback_F fnCallback,void *pvUserData);


/**
@brief 移除一个回调

@param[in] pvUserData 用户数据信息
@param[in] fnCallback 要移除的回调
@return CSUDI_SUCCESS表示移除回调成功，其他表示失败，失败类型见错误码值
@note  当存在和pfCallback和dwUserData均相同的时候才会移除,两个参数都匹配时才移除
*/
CSUDI_Error_Code CSUDIPPPOERemoveCallback(CSUDIPPPOECallback_F fnCallback,void *pvUserData);

#ifdef __cplusplus
}
#endif


#endif







