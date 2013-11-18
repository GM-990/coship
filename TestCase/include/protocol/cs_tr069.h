/*@defgroup TR069数据类型的定义，以及接口的说明
@brief tr069对应的头文件cs_tr069.h，tr069模块是一个网络管理系统的终端，启动的时候，
会向网管系统注册，注册之后，网管系统就可以远程控制这个终端了，网管系统主要的操作有
设置参数和获取参数两种，这些设置和获取都是通过tr069协议进行远程控制，tr069模块主要
实现了tr069协议的通信，解析网管系统发过来的数据包，做相应的操作之后再将信息反馈给网
管系统。
tr069模块的使用:
用户通过调用CSTR069Init函数，然后再调用CSTR069Start就可以启动tr069模块，启动之后，会
向前段网管系统注册，之后双方就可以通信了。

@version 2.0.1 2010/08/01
@version 2.0.2 2011/01/10
@{
*/
#ifndef _CS_TR069_H
#define _CS_TR069_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/
#include "udiplus_os.h"
#include "udiplus_typedef.h"
/******************************************************************************
 *                                 Macro/Define/Structus                       *
 ******************************************************************************/

#define CSTR069_NAME_LEN 256       ///< tr069中定义的数据字段名称的最大长度
#define CSTR069_VLAUE_LEN    2048  ///< tr069中定义的数据字段值的最大长度


/**@brief tr069事件类型*/
typedef enum {
	EM_TR069_MSG_BOOTSTRAP=0,      ///<启动tr069模块
	EM_TR069_MSG_GETPARAM_VALUE, ///<获取参数
	EM_TR069_MSG_SETPARAM_VALUE, ///<设置参数
	EM_TR069_MSG_REBOOT,         ///<重启设备
	EM_TR069_MSG_FACTORYRESET,   ///<设备复位
	EM_TR069_MSG_DOWNLOAD,       ///<下载文件
	EM_TR069_MSG_UPLOAD,         ///<上传文件
	EM_TR069_MSG_GETPARAM_ATTRI, ///<获取参数属性
	EM_TR069_MSG_SETPARAM_ATTRI, ///<设置参数属性
	EM_TR069_MSG_GETPRCMETHOD,   ///<tr069 获取参数RPC方法
	EM_TR069_MSG_GETPARAM_NAME=10,  ///<获取参数名称
	EM_TR069_MSG_COMMON_FAULT,   ///<通信出错
	EM_TR069_MSG_SETPARAM_FAULT, ///<设置参数错误
	EM_TR069_MSG_END_SESSION,    ///<结束会话
	EM_TR069_MSG_PING,           ///<ping操作
	EM_TR069_MSG_TRACERT,        ///<trace路由操作
	EM_TR069_MSG_TRAPE,          ///<trape操作
	EM_TR069_MSG_PERIODIC,       ///<定时上报
	EM_TR069_MSG_OPENEPG,        ///<打开EPG
	EM_TR069_MSG_RESTARTSERVER ,  ///<重启http服务器
	EM_TR069_MSG_SETPARAM_VALUEA=20,
	EM_TR069_MSG_GETCONFIGFILE,
	EM_TR069_MSG_GETERRORLOG,
	EM_TR069_MSG_ADDOBJECT,
	EM_TR069_MSG_DELOBJECT,
	EM_TR069_MSG_STARTAPP,
	EM_TR069_MSG_EMERGENCYRADIO,
	EM_TR069_MSG_UNKNOW
}CSTR069_EVENT_CODE_E;


/**@brief data类型 */
typedef enum {
	EM_TR069_STRING = 0, ///string
	EM_TR069_INT = 1,    ///int
	EM_TR069_UNSIGNEDINT = 2,     ///unsignedInt
	EM_TR069_BOOL = 3,    ///<boolean
	EM_TR069_DATETIME = 4, ///dateTime
	EM_TR069_OBJECT = 5,  ///object
	EM_TR069_OTHER
}CSTR069_DATATYPE_E;


/**@brief tr069返回值类型*/
typedef enum {
	EM_TR069_SUCCESS = 0,   ///<成功
	EM_TR069_FAILURE,       ///<失败
	EM_TR069_INVALPARA      ///<参数错误
}CSTR069_RET_E;

/**@brief inform类型 */
typedef enum {
	EM_TR069_INFONOMAL = 0, ///<基本inform信息
	EM_TR069_INFOPERIOD,    ///<周期性上报类型信息
	EM_TR069_INFOTRAPE,     ///<trape 类型信息
	EM_TR069_INFODEVICEID   ///<基本deviceid类型信息
}CSTR069_INFOTYPE_E;

/**@brief tr069模块状态 */
typedef enum {
	EM_TR069_STATE_INIT ,    ///<初始状态,调用回调函数CSOnTR069StateChange_F时，dwPara1和dwPara2为空
	EM_TR069_STATE_READY,    ///<就绪状态，就绪之后可以添加节点信息,调用回调函数CSOnTR069StateChange_F时，dwPara1和dwPara2为空
	EM_TR069_STATE_END      ///<结束状态,调用回调函数CSOnTR069StateChange_F时，dwPara1和dwPara2为空
}CSTR069State_E;

/**@brief tr069事件结构体，调用CSTR069PostMsg会传入该值 */
typedef struct{
 	int nEventCode;         ///<事件操作码，取值为CSTR069_EVENT_CODE_E中定义的类型
  	int nEventData;         ///<事件值
	HCSHANDLE hTr069handle; ///<tr069句柄
}CSTR069_EVENT_S;


/*
@brief 函数指针，用来读取用户信息

@param[out] pcBuffer:输出参数，输出用户信息
@param[in] nLen:用户信息长度
@param[in] pvUserdata:用户参数，由用户自定义使用，用户在初始化tr069或者添加节点信息时，在CSTR069_Item_S中传入pvUserdata，
tr069调用CSOnTR069Read_F 的时候将该参数回调给用户
*/
typedef int (*CSOnTR069Read_F)(char *pcBuffer,int nLen,void* pvUserdata);  


/*
@brief 函数指针，用来设置用户信息

@param[in] pcBuffer:存放要设置的用户信息
@param[in] nLen:用户信息长度
@param[in] pvUserdata:用户参数，由用户自定义使用，用户在初始化tr069或者添加节点的时候，在CSTR069_Item_S中传入pvUserdata，
tr069调用CSOnTR069Write_F 的时候将该参数回调给用户
*/
typedef int (*CSOnTR069Write_F)(char *pcBuffer,int nLen,void* pvUserdata);


/**@brief TR069节点信息描述

@note 调用CSTR069Init函数的时候要传入CSTR069_Item_S类型的数组，该结构体中定义了节点的操作
类型:nType,设置分为两种:
1、当前端网管系统获取参数时:
nType =0，表示调用CSOnTR069GetPara_F函数(由用户实现)来获取值
nType =1 ,表示 当CSTR069_Item_S类型的节点acData不为""，即默认值不为空字符串时，取默认值acData，
如果acData为空，而pfRead不为空，则调用pfRead获取值
nType =2，表示使用tr069内部实现来获取，不依赖用户注册的函数指针
2、当前段网管系统设置参数时:
nType =0，表示调用CSOnTR069SetPara_F函数(由用户实现)来设置
nType =1 ,表示 当CSTR069_Item_S类型的节点pfWrite不为空，调用pfWrite来设置
nType =2，表示使用tr069内部实现来设置，不依赖用户注册的函数指针
*/
typedef struct
{
	char acFullName[CSTR069_NAME_LEN];  ///< 描述节点全路径，比如"Device.LAN.1.IPAddress"
	int  nType;                     ///< 描述节点操作类型,详细说明见上面结构体说明
	int nWritable;	
	char acData[CSTR069_VLAUE_LEN]; ///< 节点默认值	
	CSOnTR069Read_F pfRead;            ///< 获取用户数据信息	
	CSOnTR069Write_F pfWrite;          ///< 设置用户数据信息
    void* pvUserdata;               ///< 保存用户自定义信息，例如:两个节点信息完全相同的叶子节点，
                                    ///< 用pvUserdata这个参数来区分叶子节点的差异性,假设第一块网卡的一个叶子的描
                                    ///< 述是:{"IPAddress",0,1,"",CSNetWork_GetUsrIP,CSNetWork_SetUsrIP,(void*)1},
                                    ///< 第二块网卡的描述是:{"IPAddress",0,1,"",CSNetWork_GetUsrIP,CSNetWork_SetUsrIP,(void*)2}, 
                                    ///< 用户设置好之后，tr069模块调用pfRead和pfWrite的时候会将所设置的pvUserdata回调上来，告知用户是
                                    ///<取哪块网卡的信息
 	CSTR069_DATATYPE_E eDataType;   ///<节点的数据类型，具体取值见CSTR069_DATATYPE_E的定义
}CSTR069_Item_S;


/*
@brief TR069当前状态指针，在tr069模块状态变化的时候会触发该回调，调用CSTR069AddCallback的时候需要传入此类型的函数指针

@param[in] dwUserData:用来保存用户数据
@param[in] eMsg:返回tr069当前状态值，当为就绪状态的时候表示tr069已经就绪，可以添加节点信息
@param[in] dwPara1: 参数1
@param[in] dwPara2: 参数2
*/
typedef void (*CSOnTR069StateChange_F)(DWORD dwUserData, CSTR069State_E eMsg, DWORD dwPara1, DWORD dwPara2);


/*
@brief 设置用户参数函数指针，调用CSTR069Init的时候需要传入此类型的函数指针

@param[in] pcParaName:所设置的参数的名称，含全路径名称，如:Device.DeviceInfo.Manufacturer
@param[in] pcParaValue:所设置的参数的值
@param[in] nParaLen: 所设置的参数的长度
@param[in] unUserData:用来保存用户数据
*/
typedef int (*CSOnTR069SetPara_F)(char* pcParaName,char *pcParaValue,int nParaLen,unsigned int unUserData);


/*
@brief 获取用户参数函数指针，调用CSTR069Init的时候需要传入此类型的函数指针

@param[in] pcParaName:所要获取的的参数的名称，含全路径名称，如:Device.DeviceInfo.Manufacturer
@param[out] pcParaValue:输出参数，用来保存所要获取的参数的值
@param[in] nParaLen: 获取的参数的长度
@param[in] unUserData:用来保存用户数据
*/
typedef int (*CSOnTR069GetPara_F)(char* pcParaName,char *pcParaValue,int *pnParaLen,unsigned int unUserData);


/*
@brief  tr069 模块初始化

@param[in] 	psItem:	 CSTR069_Item_S类型的结构体数组.
@param[in] 	nItemNum: psItem的个数.
@param[in]  fnSetPara: 设置参数的函数指针.
@param[in]  fnGetPara: 获取参数的函数指针.
@param[in]  unUserData:  用来保存用户数据
@return  成功返回 EM_TR069_SUCCESS  失败返回 EM_TR069_FAILURE 或其他
*/ 
CSTR069_RET_E CSTR069Init(CSTR069_Item_S *psItem,int nItemNum,CSOnTR069SetPara_F fnSetPara,CSOnTR069GetPara_F fnGetPara,unsigned int unUserData);


/*
@brief  tr069 设置inform信息

@note 设置之后，在tr069INFORM包中将携带这些信息发送出去给前段
@param[in]	Inform: 要设置的INFORM信息的数组.
@param[in]	nInformNum: 设置的INFORM信息的个数.
@param[in]	eInfoType:	INFORM信息类型，取值为CSTR069_INFOTYPE_E中定义的类型
@return  成功返回 EM_TR069_SUCCESS  失败返回 EM_TR069_FAILURE 或其他
*/ 
CSTR069_RET_E CSTR069SetInform(char *ppInform[],int nInformNum,CSTR069_INFOTYPE_E eInfoType);


/*
@brief  启动tr069

@return  成功返回 EM_TR069_SUCCESS  失败返回 EM_TR069_FAILURE 或其他
*/ 
CSTR069_RET_E CSTR069Start(void);


/*
@brief  发送消息给tr069模块

@param[in] 	pEvent: 将要发送给tr069模块的消息，消息取值见CSTR069_EVENT_S中的定义.
@return  成功返回 EM_TR069_SUCCESS  失败返回 EM_TR069_FAILURE 或其他
*/ 
CSTR069_RET_E CSTR069PostMsg(CSTR069_EVENT_S *psEvent);


/*
@brief 获取首次连接网管系统成功的时间

@return  返回首次连接网管系统成功的时间
*/ 
CSUDIPLUS_TIME_T CSTR069GetFirstLinkTimes(void);

/*
@brief  设置前段网管系统的URL，用户名和密码

@note 该操作应该在调用CSTR069Start之前做，否则无效，CSTR069Start之后发送INFORM
包的时候会用到该URL，用户名和密码
@param[in] 	pcUrl: 前段网管系统的URL
@param[in] 	pcUserName: 前端网管系统的用户名.
@param[in] 	pcPassWord:  前端网管系统的密码.
@return  成功返回 EM_TR069_SUCCESS  失败返回 EM_TR069_FAILURE 或其他
*/ 
CSTR069_RET_E CSTR069SetServerInform(char *pcUrl,char *pcUserName,char *pcPassWord);


/*
@brief  添加tr069的一组节点

@note 该接口的调用应该在tr069状态为EM_TR069_STATE_READY
@param[in] 	psItem:      所要添加的一组节点的信息.
例如添加一组节点信息:
    CSTR069_Item_S tr069LanArr_s[] = {
    {"Device.LAN.1.AddressingType", 0,"GetAddrType",CSUDI_NULL,CSUDI_NULL,(void*)1},
    {"Device.LAN.1.IPAddress", 1, "", CSNetWork_GetUsrIP,CSNetWork_SetUsrIP,(void*)1},
    {"Device.LAN.1.SubnetMask", 1, "", CSNetWork_GetUsrMask, CSNetWork_SetUsrMask,(void*)1},
    {"Device.LAN.1.DefaultGateway",1, "", CSNetWork_GetUsrGateway,CSNetWork_SetUsrGateway,(void*)1},
    {"Device.LAN.1.DNSServers", 1, "",CSNetWork_GetUsrDNSServer,CSNetWork_SetUsrDNSServer,(void*)1},
    {"Device.LAN.1.MACAddress",1, "",CSNetWork_GetUsrMac,CSUDI_NULL,(void*)1}
};
@param[in] 	nItemNum:      总共要添加的节点数，即为psItem数组的大小.
@return  成功返回 EM_TR069_SUCCESS  失败返回 EM_TR069_FAILURE 或其他
*/ 
CSTR069_RET_E CSTR069AddItem(CSTR069_Item_S *psItem,int nItemNum);

/*
@brief  删除tr069的一组节点

@note 该接口的调用应该在tr069状态为EM_TR069_STATE_READY
@param[in] 	psItem:      所要删除的一组节点的信息.
例如添加一组节点信息:
    CSTR069_Item_S tr069LanArr_s[] = {
    {"Device.LAN.1.AddressingType", 0,"GetAddrType",CSUDI_NULL,CSUDI_NULL,(void*)1},
    {"Device.LAN.1.IPAddress", 1, "", CSNetWork_GetUsrIP,CSNetWork_SetUsrIP,(void*)1},
    {"Device.LAN.1.SubnetMask", 1, "", CSNetWork_GetUsrMask, CSNetWork_SetUsrMask,(void*)1},
    {"Device.LAN.1.DefaultGateway",1, "", CSNetWork_GetUsrGateway,CSNetWork_SetUsrGateway,(void*)1},
    {"Device.LAN.1.DNSServers", 1, "",CSNetWork_GetUsrDNSServer,CSNetWork_SetUsrDNSServer,(void*)1},
    {"Device.LAN.1.MACAddress",1, "",CSNetWork_GetUsrMac,CSUDI_NULL,(void*)1}
};
@param[in] 	nItemNum:      总共要删除的节点数，即为psItem数组的大小.
@return  成功返回 EM_TR069_SUCCESS  失败返回 EM_TR069_FAILURE 或其他
*/ 
CSTR069_RET_E CSTR069DelItem(CSTR069_Item_S *psItem,int nItemNum);
/*
@brief  向TR069注册一个callback

@param[in] dwUserData: 保存用户数据信息
@param[in] Callback:  要添加的回调	
*/ 
BOOL CSTR069AddCallback(DWORD dwUserData, CSOnTR069StateChange_F fnOnTR069StateChange);

#ifdef __cplusplus
}
#endif
/** @} */
#endif

