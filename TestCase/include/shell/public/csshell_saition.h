#ifndef _CSSHELL_SAITION_H_
#define _CSSHELL_SAITION_H_

#include "mmcp_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define SAITION_BTH_SERVICE_LEN    32
#define SAITION_URL_LEN            1024

/**@defgroup SAITION SAITION本模块对甩信对外接口和数据结构进行了定义
@brief  

@ 1 本接口中所有定义的字符串均为ASCII字符串。


@ 2 甩信模块支持用户扩展自定格式的内容交互。

@ 2.1手机发送自定义命令至机顶盒时，甩信模块通过EM_SAITION_MTOSNOTIFY消息通知应用，消息内容为字符串，格式为："{Socket:12,Command:5,Paramlen:123,Param:'xxxx'}" , 其中：
- Socket取值为数值型，消息通道socket号。\n
- Command取值为数值型，手机发送的用户自定义命令编号，详见《甩信网元间接口文档.doc》“IF4-15: 万能接口”中定义。\n
- Paramlen取值为数值型，手机发送的用户自定义命令参数长度，详见《甩信网元间接口文档.doc》“IF4-15: 万能接口”中定义。\n
- Param取值为字符串格式，手机发送的用户自定义命令参数，详见《甩信网元间接口文档.doc》“IF4-15: 万能接口”中定义。\n

@ 2.2 机顶盒向手机发送自定义命令时，通过调用接口m_fnSaitionSendContent发送自定义命令。自定义交互内容为json字符串，格式为：\n 
"{Socket:12,Command:5,Paramlen:123,Param:'xxxx'}", 其中：
- Socket取值为数值型，机顶盒与目标手机的消息通道socket号，取自EM_SAITION_MTOSNOTIFY消息内容中的socket号
- Command取值为数值型，需发送给手机的用户自定义命令编码，详见《甩信网元间接口文档.doc》“IF4-16: 万能接口”中定义。
- Paramlen取值为数值型，需发送给手机的用户自定义命令参数长度，详见《甩信网元间接口文档.doc》“IF4-15: 万能接口”中定义。
- Param取值为字符串格式，需发送给手机的用户自定义命令参数，详见《甩信网元间接口文档.doc》“IF4-16: 万能接口”中定义。

@ 3 机顶盒向应用发送EM_SAITION_SETTICKET消息时，消息内容为json字符串,格式为："{TicketID:%d,TicketToken:'%s',ProductID:%d,ContentID:%d,PlayModel:%d,UID:'%s'}", 其中：
- TicketID取值为数值型，详见《甩信网元间接口文档.doc》“IF4-1:播放请求接口（ticket）”中定义。
- TicketToken取值为字符串格式，详见《甩信网元间接口文档.doc》“IF4-1:播放请求接口（ticket）”中定义。
- ProductID取值为数值型，详见《甩信网元间接口文档.doc》“IF4-1:播放请求接口（ticket）”中定义。
- ContentID取值为数值型，详见《甩信网元间接口文档.doc》“IF4-1:播放请求接口（ticket）”中定义。
- PlayModel取值为为数值型，详见《甩信网元间接口文档.doc》“IF4-1:播放请求接口（ticket）”中定义。
- UID取值为字符串格式，详见《甩信网元间接口文档.doc》“IF4-1:播放请求接口（ticket）”中定义。
@{
*/

/**
@brief 甩信模块使用的传输设备
*/
typedef enum  
{
    EM_SAITION_TANSDEVTYPE_UNDEF = 0,
    EM_SAITION_TANSDEVTYPE_BLUETOOTH , 
    EM_SAITION_TANSDEVTYPE_ETHERNET , 
    EM_SAITION_TANSDEVTYPE_COM
} CSSaitionTransDevType_E;

/**
@brief 甩信消息回调类型
*/
typedef enum {
	EM_SAITION_CONNECT, ///<手机和机顶盒连接成功
	EM_SAITION_DISCONNECT ,	///<手机和机顶盒断开连接
	EM_SAITION_SETURL,   ///< 2 中间件将播放的URL发给应用，发送给应用的消息内容是一个CSSaitionPlayURL_S 结构体类型的变量	
	EM_SAITION_SETVOLUME ,     ///< 3 手机遥控器设置机顶盒播放音量,发送给应用的消息内容是音量值，类型为int
	EM_SAITION_SETPROCESS,    ///<4手机设置机顶盒播放进度,发送给应用的消息内容是播放进度值，类型是int,值为当前进度占总进度的百分比
	EM_SAITION_SETTICKET,   ///< 5中间件将播放的ticket发给应用,发送给应用的消息内容是一个字符串，对应的是ticket内容,见文档开头注释3
	EM_SAITION_MTOSNOTIFY  ///< 6自定义命令通知，MOBILE->STB，回调的消息内容是手机传过来的字符串，见文档开头注释2.1
}CSSaitionEvent_E;

/**
@brief 甩信模块返回值
*/
typedef enum  
{
    EM_SAITION_SUCCESS = 0, 
    EM_SAITION_FAILURE,
    EM_SAITION_TIMEOUT,
    EM_SAITION_INVALPARA,
    EM_SAITION_SOCKET_ERROR 
} CSSaitionRet_E;

/**
@brief CSSaitionPlayURL_S  甩信播放url，它包括片花URL和正片URL
*/
typedef struct {
	char m_acPreviewURL[SAITION_URL_LEN]; /*片花url*/
	char m_acPlayURL[SAITION_URL_LEN];    /*正片url*/
}CSSaitionPlayURL_S ;

/**
@brief CSSaitionBTHInfo_S 注册sdp服务时需要的一些信息
*/
typedef struct {
	const char m_acSerialID[SAITION_BTH_SERVICE_LEN]; /*序列号, 对于甩信来说，此序列号是向蓝牙标准协议组 申请了的甩信序
						   列号，和平台无关,唯一标识甩信, 其值为：{ 0x03, 0xDA, 0x49, 0x59, 0x8A, 0xBA, 0x43, 
						   0xBA, 0xB3, 0xD4, 0xC2, 0x77, 0x6B, 0xB0, 0xBB, 0x26} */
	const char m_acServiceName[SAITION_BTH_SERVICE_LEN];     /*服务名，与sdp服务的连接无关*/
	const char m_acServiceDsc[SAITION_BTH_SERVICE_LEN];      /* 服务描述名，与sdp服务的连接无关*/
	const char m_acServiceProv[SAITION_BTH_SERVICE_LEN];     /*服务提供商名，与sdp服务的连接无关*/
	UINT m_uPort;     /* 服务器的侦听端口, 根据现在的手机客户端版本，其值为28,此端口在带甩信cfg 的工程里面是从saitionconfig.properties配置文件中读取*/
}CSSaitionBTHInfo_S;

/**
@brief 调用CSSaitionAddCallback和CSSaitionRemoveCallback的时候, 需要传入此类型的函数指针
@param[in] eMsg:表示当前收到的消息类型
@param[in] dwUserData:用来保存用户数据
@param[in] dwPara: 表示收到的消息类型所对应的参数内容地址
*/
typedef void (*CSOnSaitionEventCallBack_F)(CSSaitionEvent_E eMsg, void *pvPara, DWORD dwUserData);

typedef struct 
{
    int m_nSize;	///<本结构大小，一定等于sizeof(CSShellSaitionInitParam_S)

    /**
    @brief 	启动甩信功能前的初始化工作
    @param[in]  eDeviceType: 甩信模块使用的传输设备,具体定义见CSSaitionTransDevType_E
    @param[in] pvParam: 初始化时需要传入的参数，对于EM_SAITION_TANSDEVTYPE_BLUETOOTH传输设备，此
                参数类型为CSSaitionBTHInfo_S*类型，表示蓝牙sdp注册信息；对于 EM_SAITION_TANSDEVTYPE_ETHERNET
                传输设 备，此参数类型为*UINT型，表示机顶盒服务端端口号,
    @return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
    @note 
    --参数pvParam为空时，函数返回CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionInit)(CSSaitionTransDevType_E eDeviceType, void *pvParam);

    /**
    @brief 	启动甩信服务, 在CSSaitionInit之后调用
    @param[in] uPort: 甩信服务端端口，此端口在带甩信cfg 的工程里面是从saitionconfig.properties配置文件中读取
    @return  成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
    @note 
    - 如果需要启动甩信服务，需先调用CSSaitionInit函数后，再调用此函数就可以了。
    */
    CSUDI_Error_Code (* m_fnSaitionStart)();

    /**
    @brief 	停止甩信服务
    @return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
    @note 
    -
    */
    CSUDI_Error_Code (* m_fnSaitionStop)();

    /**
    @brief 	设置甩信生成URL, 此URL和甩信中的"拉"功能有关
    @param[char*] pcBuildUrl: 甩信生成URL, pcBuildUrl必须是以'\0'结尾的ASCII编码字符串, 且字符串中间不能含有'\0\.
    	   且pcBuildUrl的长度需小于(1024)， 否则不能将甩信生成URL正确设置到C模块里面
    @return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
    @note 
    -参数pcBuildUrl为空时，函数返回CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionSetBuildURL)(const char *pcBuildUrl);

    /**
    @brief 	获取甩信生成URL
    @param[out] pcBuildUrl:  存放获取的甩信生成URL缓存
    @param[in] nUrlLen:  pcBuildUrl缓存长度
    @return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
    @note 
    -参数pcBuildUrl为空时函数返回CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionGetBuildURL)(char* pcBuildUrl, int nUrlLen);

    /**
    @brief 	设置甩信中心URL, 此URL和甩信的"甩" 功能有关
    @param[in] pcHomeUrl: 甩信中心URL, pcHomeUrl必须是以'\0'结尾的ASCII编码字符串, 且字符串中间不能含有'\0\.
    	     且pcHomeUrl的长度需小于(1024)， 否则不能将甩信中心URL正确设置到C模块里面
    @return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
    @note 
    -参数pcHomeUrl为空时函数返回CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionSetHomeURL)(const char *pcHomeUrl);

    /**
    @brief 	获取甩信中心URL
    @param[out] pcHomeUrl:  存放获取的甩信中心URL缓存
    @param[in] nUrlLen:  pcHomeUrl缓存长度
    @return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
    @note 
    -参数pcHomeurl为空时函数返回CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionGetHomeURL)(char* pcHomeUrl, int nUrlLen);

    /**
    @brief 	此接口为了实现机顶盒通知手机执行某个过程。由其它模块调用将pcContent
    设置到甩信C模块，内容需双方协商决定，本接口只负责透传,具体见文档开头注释2.2
    @param[char*] pcContent:业务数据的请求参数, pcContent必须是以'\0'结尾的ASCII编码字符串, 且字符串中间不能含有'\0\.
    	 且pcContent的长度需小于(2*1024)， 否则不能将内容正确设置到C模块里面,
    @return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
    @note 调用完此接口后，会通过异步发送数据的方式将此内容透传给手机。
    -参数pcContent为空时函数返回CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionSendContent)(const char *pcContent);

    /**
    @brief  向甩信模块注册一个回调
    @param[in] fnOnSaitionEventCallBack:  要添加的回调
    @param[in] dwUserData: 保存用户数据信息
    @return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE	
    */
    CSUDI_Error_Code (* m_fnSaitionAddListener)(CSOnSaitionEventCallBack_F fnOnSaitionEventCallBack, DWORD dwUserData);

    /**
    @brief  从甩信模块移掉一个callback
    @param[in] fnOnSaitionEventCallBack:  要删除的回调
    @param[in] dwUserData: 保存用户数据信息
    @return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionRemoveListener)(CSOnSaitionEventCallBack_F fnOnSaitionEventCallBack, DWORD dwUserData);
}CSShellSaitionInitParam_S;

/**
@brief 设置 甩信 接口参数
@param psParam  甩信  Shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSShellSaitionSetParam(CSShellSaitionInitParam_S *psParam);

/**
@brief 获取 获取 接口参数
@param 无
@成功返回CSShellSaitionInitParam_S,失败返回NULL
*/
const CSShellSaitionInitParam_S* CSShellSaitionGetParam();

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



