#ifndef _CS_JSMOTOVOD_H_
#define _CS_JSMOTOVOD_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif


/**@brief moto vod点播系统中对应的各种信息，网管系统使用*/
typedef enum
{
	EM_MOVOVOD_SERVICETYPE, 		///< ServiceType 		String(16) 	R 	服务类型：点播、时移
	EM_MOVOVOD_SERVICEID,    		///< ServiceID 		UnsignedInt R 	服务ID
	EM_MOTOVOD_SERVICELENGTH,  		///< ServiceLength 		UnsignedInt R 	服务总时长(秒)
	EM_MOTOVOD_RESTLENGTH, 			///< RestLength 		UnsignedInt R 	剩余时长(秒)
	EM_MOTOVOD_FREQUENCY, 			///< Frequency 		UnsignedInt R 	服务所在频点
	EM_MOTOVOD_SERVICEGROUP,		///< ServiceGroup 		String(8) 	   R 	分组号
	EM_MOTOVOD_GSRMIPADDRESS, 		///< GSRMIPAddress 	String(16) 	R 	Global SRM 的IP 地址
	EM_MOTOVOD_BMIIPADDRESS,		///< BMIIPAddress 		String(16) 	R 	BMI 的IP 地址
	EM_MOTOVOD_DIAGNOSTICSTATE,		///< DiagnosticState 	String(32) 	RW 	VOD 模拟点播请求状态
	EM_MOTOVOD_SRMIPADDRESS, 		///< SRMIPAddress 		String(16) 	RW 	SRM 的IP 地址
	EM_MOTOVOD_CONTENTID, 			///< ContentID 		String(16) 	RW 	点播内容ID
	EM_MOTOVOD_ACKTIME, 			///< AckTime 			UnsignedInt R 	点播信令响应时长
	EM_MOTOVOD_RETURNCODE, 			///< ReturnCode 		UnsignedInt R 	点播返回状态码	
	EM_MOTOVOD_PLAYCURRTIME,		///<CurrentTime             UnsignedInt R 	当前的播放时间(秒)
	EM_MOTOVOD_PROGRAMENAME,		///<Programename         String(32)		当前播放节目的名称
	EM_MOTOVOD_TIMESHIFT_CHANNELNAME	///channelname       String(32)		获得时移的频道名称
}CSMotoVodInfo_E;

/**@brief moto vod 点播系统中的操作的信息，网管系统使用*/
typedef enum
{
	EM_MOTOVOD_STOP,			//停止，取消，退出或消失
	EM_MOTOVOD_PLAY,			//播放，确认，进入或显示
	EM_MOTOVOD_PAUSE, 			//暂停
	EM_MOTOVOD_RESUAME,			//继续
	EM_MOTOVOD_NEXT,			//下一段
	EM_MOTOVOD_PREVIOUS,		//前一段
	EM_MOTOVOD_SEEK,			//定位
	EM_MOTOVOD_FAST,			//快进
	EM_MOTOVOD_BACK			//回退
}CSMotoAction_E;

/**@brief moto vod 点播过程中的错误消息，网管系统使用*/
typedef enum
{
	EM_MOTOVOD_SSP_OPEN_FAILURE,			//ssp链接失败
	EM_MOTOVOD_LSCP_CONNECT_FAILURE,		//LSCP链接失败
	EM_MOTOVOD_LSCP_OPERATOR_FAILURE,		//LSCP操作失败
	EM_MOTOVOD_SSP_REALEAS_FAILURE			//ssp释放连接失败
}CSMotoNetErr_E;


/**
@brief 获得moto vod点播的动作操作
@param [in] eTag当前的vod动作
@note 该回调用于处理用户操作
*/
typedef void(*CSJSMotoOperAction)(CSMotoAction_E eTag);

/**
@brief获得motovod点播过程中的发生的错误
param[in]eTag当前错误的类型
param[in]pAssertID当前播放的资源号
@note该回调用于处理点播过程中和播放过程中发生的错误
*/
typedef void(*CSJSMotoNetError)(char* pcErrorStr, char *pcAssertID);

typedef struct _MOTO_ACTION_RIGISTER
{
	CSJSMotoOperAction m_pMotoOperAction;
	CSJSMotoNetError m_pMotoNetAction;
}CSMotoActionRigister;


/**
@brief 获取moto vod点播过程中对应的信息

@return TRUE:获取成功; FALSE:获取失败
@note 该接口一般在系统初始化时就需要调用，前置条件是CSMISInit已经调用过
*/
BOOL CSJSMotoVodGetInfo(CSMotoVodInfo_E eTag, char *pcValue, int nLen);

/**
@brief 设置moto vod点播信息

@return TRUE:设置成功; FALSE:设置失败
@note 该接口用于模拟点播测试(江苏省网提出)，CSMotoVodInfo_E中具有RW属性的字段才可以调用本接口
*/
BOOL CSJSMotoVodSetInfo(CSMotoVodInfo_E eTag, char *pcValue);

/**
@brief 注册VOD动作操作
@param [in] motoActionvod动作的回调
@return TRUE:设置成功; FALSE:设置失败
*/
BOOL CSJSRigisterMotoAction(CSMotoActionRigister motoAction);

#ifdef __cplusplus
}
#endif

#endif
