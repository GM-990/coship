/******************************************************************************
FileName:  cs_ipplugin.h
Copyright (c) 2009， Shen Zhen Coship Electronics CO.,LTD.

Version:       Ver 1.0
Author:        
File Description:
        1、本文件规定了插件的标准接口和插件管理的接口
        2、插件必须实现创建、销毁、读数据、能力查询、当前媒体查询五个方法，其它方法则可以不提供
        如果某个方法不提供应该能够在能力查询方法中获知
Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
*******************************************************************************/
/*
冲突部分

回调函数FOnCSIPPluginCallback
返回值CSIPPluginRet_E
设置速率统一度量
媒体类型
回调消息类型
*/

#ifndef _CS_IPPLUGIN_H_
#define _CS_IPPLUGIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "cs_player_message.h"

/**
@brief  macro
*/
#define  CS_IPPLUGIN_NAME_LEN    64         
#define  CS_IPPLUGIN_DESCRIPTION_LEN   256
#define  CS_IPPLUGIN_PROTOCOL_NAME_LEN   64

/**
@brief  插件INVOKE方法命令类型枚举
*/
typedef enum _CSIPPlugin_Invoke_CommandType_E
{
	EM_CSIPPLUG_INVOKE_COMMAND_SAVE_BOOKMARK,
	EM_CSIPPLUG_INVOKE_COMMAND_GET_STARTTIME,
	EM_CSIPPLUG_INVOKE_COMMAND_DISABLE_BREAKPOINT_TOSEE
}CSIPPlugin_Invoke_CommandType_E;

/**
@brief  插件能力枚举
*/
typedef  enum  _CSIPPluginCapability_E
{
	EM_CSIPPLUG_CAPABILITY_PAUSE,
	EM_CSIPPLUG_CAPABILITY_RESUME,
	EM_CSIPPLUG_CAPABILITY_SETRATE,
	EM_CSIPPLUG_CAPABILITY_GETRATE,
	EM_CSIPPLUG_CAPABILITY_SETTIME,
	EM_CSIPPLUG_CAPABILITY_GETTIME,
	EM_CSIPPLUG_CAPABILITY_GETDURATION,
	EM_CSIPPLUG_CAPABILITY_GETSENDDECODERELEN,  
	EM_CSIPPLUG_CAPABILITY_INVOKE,
	EM_CSIPPLUG_CAPABILITY_SETDURATION,
	EM_CSIPPLUG_CAPABILITY_SETDMXCHECKBUFFERINGLEN,
	EM_CSIPPLUG_CAPABILITY_GETDMXBUFFERINGPERCENT,
	EM_CSIPPLUG_CAPABILITY_GETSTARTCLOCK
} CSIPPluginCapability_E;

/**
@brief  媒体类型
*/
typedef enum  _CSIPPluginMediaType_E
{
    EM_PUMP_SOURCE_TYPE_TS = 0,
	EM_PUMP_SOURCE_TYPE_ES,  	  
	EM_PUMP_SOURCE_TYPE_PCM,
	EM_PUMP_SOURCE_TYPE_MP3,
	EM_PUMP_SOURCE_TYPE_DIR, //yzh, test
	EM_PUMP_SOURCE_TYPE_UNKNOWN
}CSIPPluginMediaType_E;

/**
@brief  编码类型
*/
typedef enum _CSEncodeType_E
{    
	EM_ENCODE_H264,
    EM_ENCODE_MPEG1,
    EM_ENCODE_MPEG2,
    EM_ENCODE_UNKNOWN
}CSEncodeType_E;

/**
@brief  TS信息
*/
typedef struct  _CSPSIInfo_S
{
    int nAudPid;
    int nAudStreamType;  //类型参考udi
    int nVidPid;
    int nVidStreamType;   //类型参考udi
    int nPcrPid;
    int nProgramId;
}CSPSIInfo_S;

/**
@brief  返回值类型
*/
typedef enum  _CSIPPluginRet_E
{
    EM_PUMP_SUCCESS = 0, 
    EM_PUMP_FAILURE,
    EM_PUMP_TIMEOUT,
    EM_PUMP_INVALPARA,
    EM_PUMP_NOMEM,
    EM_PUMP_URL_ERROR, 
    EM_PUMP_NOT_SUPPORT,
    EM_PUMP_SOCKET_ERROR ,
    EM_PUMP_RESOURCES_FAILURE,
    EM_PUMP_DEMUX_ERROR,
    EM_PUMP_END_OF_STREAM
} CSIPPluginRet_E;
/**
@brief  返回值类型
*/
typedef enum  _CSIPPluginNameType_E
{
    EM_PUMP_Plugin_NOT_SET = 0, 
    EM_PUMP_Plugin_CITV,
    EM_PUMP_Plugin_NGOD,
    EM_PUMP_Plugin_SEARCH_VOD,
} CSIPPluginNameType_E;


/**
@brief 回调函数类型
@param[in] nUserData 用户数据
@param[in] nMsg 消息类型,具体参见枚举类型CSIPPUMPMsg_E(cs_player_message.h)
@param[in] nPara1 消息参数1
@param[in] nPara2 消息参数2
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    特殊说明:
		1.以下消息可能为插件收到的服务器的主动通知，也需要通过回调告之调用者:
			EM_CSIPPUMP_MSG_SERVER_START_STREAM:快退到头，不需要参数
			EM_CSIPPUMP_MSG_SERVER_END_STREAM :快进到尾，不需要参数
			EM_CSIPPUMP_MSG_SERVER_OPTIONS:，不需要参数
			EM_CSIPPUMP_MSG_SERVER_REDIRECT:需要重定向，不需要参数
		2.在与服务器交互过程中，任何由于服务器原因引起的失败，都应通过回调告之调用者:
			EM_CSIPPUMP_MSG_SERVER_ERROR，且参数1表示错误原因，参数2预留
		3.有的服务器会要求机顶盒定时发送心跳消息，心跳消息的处理一般为插件内部处理，不需要外部知道	
*/
typedef int (*FOnCSIPPluginCallback) (int  nUserData,int  nMsg,int nPara1,int nPara2);

/**
@brief   创建插件
@param[in]  pszURL  统一资源定位器，表明了协议的类型和资源地址
@param[in]  dwTimeout 网络超时时间，单位为ms
@param[in]  fnCallback 需要注册的回调函数
@param[in]  nUserData  回调时回传的用户数据
@param[out] phIPClient 插件句柄
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note 本方法必须实现，且为同步方法，包含了与服务器建立连接并请求播放的过程
	  此函数调用结束以后应该可以正常播出音视频
@note 常见URL格式如下
     "dvb://490000000:6875:64:8"
     "igmp://224.0.0.10:1234"
     "file://image.mp3"
     "http://video/haha.ts"
     "rtsp://192.168.101.9:554/Video/h264/v.mpg"
@note 此函数需要回调的消息:
	1)如果使用IPQAM方式(需要锁频)点播，在获取到点播节目的频点信息后，
	回调EM_CSIPPUMP_MSG_CATVINFO_TOMP消息，参数1为节目号serviceid，参数2为CSUDITunerCableDeliver_S类型的结构体指针，存有节目频点信息
	2)如果使用IPTV方式,不需要发送EM_CSIPPUMP_MSG_CATVINFO_TOMP消息
	3)在与点播服务器交互过程中，由于服务器引起的错误或失败
	回调EM_CSIPPUMP_MSG_SERVER_ERROR，参数1为错误码或函数返回值，参数2为预留
*/
typedef  int(*FonIPPluginCreate)(const char *pszURL,  DWORD  dwTimeout,  FOnCSIPPluginCallback  fnCallback, int  nUserData, HCSHANDLE  *phIPClient);

/**
@brief   销毁插件
@param[in] hIPClient 插件句柄
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note  本方法必须实现，且为同步方法，包括通知服务器终端退出，以及资源释放
*/
typedef  int(*FonIPPluginDestroy) (HCSHANDLE hIPClient);

/**
@brief   判断能力支持
@param[in] hIPClient 插件句柄
@param[out] eCapability  要判断的能力，具体参见枚举类型CSIPPluginCapability_E
@return  支持返回TRUE，不支持返回FALSE
@note  通常使用某项能力之前需要判断是否支持，对于某个插件来说，某项能力是否
         支持不是一成不变的，有可能跟插件当前的状态有关
@note  本方法每个插件都必须实现，以供查询某种能力是否支持
@note  如果某项能力不支持，可以不实现对应的方法但必须在本方法中可以查询到
@note  本方法必须实现
*/
typedef  BOOL(*FonIPPluginCheckCapability)( HCSHANDLE hIPClient , CSIPPluginCapability_E eCapability);

/**
@brief  获取当前媒体类型
@param[in] hIPClient 插件句柄
@param[out] peMediaType 媒体类型
@param[out] pParam  返回参数，内存由外部分配，如果不关心可以传空，必须支持传空
		   当peMediaType = EM_PUMP_SOURCE_TYPE_TS时pParam表示CSPSIInfo_S信息
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    本方法必须实现
*/ 
typedef  int(*FonIPPluginGetCurMediaType)( HCSHANDLE hIPClient, CSIPPluginMediaType_E *peMediaType, void* pParam);

/**
@brief   读取数据
@param[in] hIPClient 插件句柄
@param[out] pcBuf 读出的数据，内存由外部申请
@param[in/out] pdwLen 既作输入又作输出，作输入时表示pcBuf的内存分配大小,作输出时表示实现读出的数据
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    本方法必须实现，对于只支持IPQAM方式点播的插件，可以直接返回。如果是IPTV方式，则需要实现从网络接收数据的功能
@note    读取成功时，实际读出的数据可以比期望小。
		 此函数的pcBuf中的数据是纯TS数据，如果接收的数据有特定封装，需要去掉封装。
		 如果采用TCP方式，需要注意接收数据的完整性，需要处理好接收超时，接收数据小于指定长度的情况。
		 此函数接收的数据交给datadriver以后，datadriver累积数据到一定长度以后，就会注入demux进行播放。
*/ 
typedef  int(*FonIPPluginReadStream)(HCSHANDLE  hIPClient, char *pcBuf, DWORD *pdwLen);

/**
@brief   暂停
@param[in] hIPClient 插件句柄
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    无论是同步还是异步，本方法的实现中必须通过回调函数将执行结果告之调用者.
		 如果是同步方式，则在此函数中接收应答；如果是异步方式，需要每隔固定时间去接收应答。
		 得到服务器成功应答后: 在回调函数中发送EM_CSIPPUMP_MSG_PAUSE_SUCC消息
         接收应答失败后，在回调函数中发送EM_CSIPPUMP_MSG_SERVER_ERROR消息，且参数1表示错误原因，参数2预留

@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginPause) (HCSHANDLE hIPClient);

/**
@brief   恢复
@param[in] hIPClient 插件句柄
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    无论是同步还是异步，本方法的实现中必须通过回调函数将执行结果告之调用者
		 如果是同步方式，则在此函数中接收应答；如果是异步方式，需要每隔固定时间去接收应答。
         执行成功在回调函数中发送EM_CSIPPUMP_MSG_RESUME_SUCC消息
         执行失败在回调函数中发送EM_CSIPPUMP_MSG_SERVER_ERROR消息，且参数1表示错误原因，参数2预留
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginResume) (HCSHANDLE hIPClient);

/**
@brief   设置速率
@param[in] hIPClient 插件句柄
@param[in] nRate  要设置的播放速度。为了支持慢进且不使用浮点数，输入速率与标准倍率
         符合以下的公式: nRate = RealRate * 1000
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    无论是同步还是异步，本方法的实现中必须通过回调函数将执行结果告之调用者
		 如果是同步方式，则在此函数中接收应答；如果是异步方式，需要每隔固定时间去接收应答。
         执行成功在回调函数中发送EM_CSIPPUMP_MSG_SETRATE_SUCC消息，且参数1表示服务器响应的实际倍率，如1000，2000等
         执行失败在回调函数中发送EM_CSIPPUMP_MSG_SERVER_ERROR消息，且参数1表示错误原因，参数2预留
@note    输入速率1000时为1倍速正常播放，2000为两倍速快进，4000为四倍速快进，为-2000表示两倍速快退。
         如果小于0说明为后退。
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginSetRate) (HCSHANDLE hIPClient, int nRate);

/**
@brief   获得速率
@param[in] hIPClient 插件句柄
@param[out] pnRate  获得的播放速度。为了与设置保持一致，实际速度要除以1000
         符合以下的公式: RealRate = nRate÷1000(非整除运算)
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    得到速率1000时为1倍速正常播放，2000为两倍速快进，4000为四倍速快进，为-2000表示两倍速快退。
         如果小于0说明为后退。
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginGetRate) (HCSHANDLE hIPClient, int *pnRate);

/**
@brief   设置播放开始时间
@param[in] hIPClient 插件句柄
@param[in] nTime  相对于节目开始时间的播放时间点，单位为秒。对于NPT时间，节目开始时间为0s
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    无论是同步还是异步，本方法的实现中必须通过回调函数将执行结果告之调用者
		 如果是同步方式，则在此函数中接收应答；如果是异步方式，需要每隔固定时间去接收应答。
         执行成功在回调函数中发送EM_CSIPPUMP_MSG_SETTIME_SUCC消息
         执行失败在回调函数中发送EM_CSIPPUMP_MSG_SERVER_ERROR消息，且参数1表示错误原因，参数2预留
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginSetTime) (HCSHANDLE hIPClient, int nTime);

/**
@brief   获取当前播放时间
@param[in] hIPClient 插件句柄
@param[out] pnTime  当前媒体播放时间，相对于节目开始的时间，单位为秒
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
@note    通过与服务器的特殊约定的参数来取得
		假设一个点播节目，开始时间为0，一倍速播放了10s，然后两倍速快进了5s，
		则此接口应该返回的*pnTime = 10 + 2*5 = 20s
*/
typedef  int(*FonIPPluginGetTime) (HCSHANDLE hIPClient, int *pnTime);

/**
@brief   获取节目时长，
@param[in] hIPClient 插件句柄
@param[out] pdwTime  当前媒体总时间，单位为秒
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
		一般情况下，本方式是针对点播和精彩回放实现，时移无法获取到总时长
*/
typedef  int(*FonIPPluginGetDuration) (HCSHANDLE hIPClient, DWORD *pdwTime);

/**
@brief   设置时间长度
@param[in] hIPClient 插件句柄
@param[in] dwTime  当前媒体时间，单位为秒
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginSetDuration) (HCSHANDLE hIPClient, DWORD dwTime);

/**
@brief   设置DMX缓冲大小
@param[in] hIPClient 插件句柄
@param[in] dwSize  设置DMX 缓存区的大小
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginSetDMXBufferingCheckMaxSize) (HCSHANDLE hIPClient, DWORD dwSize);

/**
@brief   设置DMX缓冲检查
@param[in] hIPClient 插件句柄
@param[out] pnPercent  输出参数，DMX缓存区空闲区域所占百分比
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginGetDMXBufferingPercent) (HCSHANDLE hIPClient, int * pnPercent);

/**
@brief   获取一次写到decoder或demux中的最佳数据长度
@param[in] hIPClient 插件句柄
@param[out] pdwLen  一次注入的最佳长度
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    每种媒体一次注入的最佳大小可能不一样，插件知道媒体的格式信息，从而可以提供给外部
@note    典型的应用是TS流的高清节目和标清节目一次注入的最佳大小并不一样
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginGetSendDecoderLen) (HCSHANDLE hIPClient, DWORD *pdwLen);

/**
@brief   插件的万能方法
@param[in] hIPClient 插件句柄
@param[in] nCommand  要执行的命令，每个插件都不一样，由插件自己去定义，可能是插件内部定义的枚举型
@param[in] pInputPara  输出参数，内存由外部分配，此处类型不定义，由插件自己去定义，可能是插件自己定义的结构体
@param[out] pOutputPara  输出参数，内存由外部分配，此处类型不定义，由插件自己去定义，可能是插件自己定义的结构体
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    本方法用于实现每个插件独有的功能，每个插件都可能有些与其它插件不一样的功能
@note    本方法参数都未显示定义，目的是交给每个插件自己去定义
@note    每个插件都可以通过该方法来无限扩展自己的特殊功能，只要使用这些特殊功能的客户知道
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginInvoke) (HCSHANDLE hIPClient, int  nCommand, void *pInputPara, void *pOutputPara);

/**
@brief   设置播放开始时间
@param[in] hIPClient 插件句柄
@param[in] nTime  相对于节目开始时间的播放时间点，单位为秒。是UTC时间转换的数值
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note    无论是同步还是异步，本方法的实现中必须通过回调函数将执行结果告之调用者
		 如果是同步方式，则在此函数中接收应答；如果是异步方式，需要每隔固定时间去接收应答。
         执行成功在回调函数中发送EM_CSIPPUMP_MSG_SETTIME_SUCC消息
         执行失败在回调函数中发送EM_CSIPPUMP_MSG_SERVER_ERROR消息，且参数1表示错误原因，参数2预留
@note    本方法可以不实现，但必须能通过m_fnCheckCapability查询到
*/
typedef  int(*FonIPPluginSetUTCTime) (HCSHANDLE hIPClient, DWORD nTime);
/**
@brief  插件完整信息
@note   一个插件必须有这样一些信息
@note   m_fnCreate、m_fnDestory、FonIPPluginReadStream、m_fnCheckCapability、m_fnGetCurMediaType是每个插件必须实现的，
        其它方法则可以选择实现
@note   通过m_fnCheckCapability方法每个插件都可以查询到某种能力是否支持
@note   如果某项能力不支持，可以不实现对应的方法但必须在本方法中可以查询到
*/
typedef struct _CSIPPluginInfo_S 
{
	char   m_szName[CS_IPPLUGIN_NAME_LEN];                 //插件名字，名字唯一标识了插件，两个插件的名字不能一样，以'\0'结尾
	char   m_szDescription[CS_IPPLUGIN_DESCRIPTION_LEN];  //插件的描述信息，以'\0'结尾
	char   m_szType[CS_IPPLUGIN_PROTOCOL_NAME_LEN];      //插件支持的协议类型，以'\0'结尾
	FonIPPluginCreate            m_fnCreate;
	FonIPPluginDestroy          m_fnDestory;
	FonIPPluginReadStream    m_fnReadStream;
	FonIPPluginCheckCapability        m_fnCheckCapability;
	FonIPPluginGetCurMediaType     m_fnGetCurMediaType;
	//以此为界限前面的方法必须实现，后面的方法可选择实现
	FonIPPluginPause        m_fnPause;
	FonIPPluginResume     m_fnResume;
	FonIPPluginSetRate     m_fnSetRate;
	FonIPPluginGetRate     m_fnGetRate;
	FonIPPluginSetTime     m_fnSetTime;
	FonIPPluginGetTime     m_fnGetTime;
	FonIPPluginGetDuration            m_fnGetDuration;
	FonIPPluginGetSendDecoderLen    m_fnGetSendDecoderLen;
	FonIPPluginInvoke        m_fnInvoke;
	FonIPPluginSetDuration            m_fnSetDuration;
	FonIPPluginSetDMXBufferingCheckMaxSize m_fnSetDMXBufferingCheckMaxSize;
	FonIPPluginGetDMXBufferingPercent m_fnGetDMXBufferingPercent;
	FonIPPluginSetUTCTime     m_fnSetUTCTime;
} CSIPPluginInfo_S;

/**
@brief   获取插件的完整信息
@param[out] psPluginInfo 插件的信息结构体，内存由外部申请
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note  本方法是每一个插件必须实现的对外公开方法，通过本方法应该可以获得插件的完整信息
@note  除本方法外，插件的其它方法应当全部是私有的，应当申明为static类型
@note  外部申请插件信息的结构体内存，本方法内部去填充该结构体
*/
typedef int (*FOnCSIPGetPluginInfo) (CSIPPluginInfo_S  *psPluginInfo);


/**
@brief   注册一个插件
@param[in] fnIPGetPluginInfo 插件暴露的获取其完整信息的方法
@return  成功返回EM_PUMP_SUCCESS，其它为失败，具体参见枚举型CSIPPluginRet_E
@note  本方法提供给应用使用，应用通过该方法注册一个插件
@note  同一个插件只能注册一次，重复注册会返回失败
*/
int  CSIPPMRegister(FOnCSIPGetPluginInfo  fnIPGetPluginInfo);

/**
@brief   请求一个插件
@param[in] pcType 要请求的插件的协议类型，如"rtsp"、"http"等
@param[in] pUserData用户私有数据，对于IPUDP类型选择插件的时候用这个参数来区分点播类型的rtsp插件和IPDUP类型的rtsp插件，非IPUDP类型可以传空
@return  成功返回一个插件的信息结构体，失败返回NULL
@note    插件的使用者通过该方法获取指定协议的插件
*/
CSIPPluginInfo_S *  CSIPPMRequest(char *pcType, void *pUserData);

/**
@brief   设置点播插件的名称
@param[in] ePluginType 点播插件名称类型
@return  成功返回TRUE，失败返回FALSE
@note    此接口作用在于一个版本同时注册多个点播插件，通过在CFG媒体播放初始化设置插件名称来识别插件
*/
BOOL CSIPPMSetVodPluginType(CSIPPluginNameType_E ePluginType);

#ifdef __cplusplus
}
#endif

#endif  

