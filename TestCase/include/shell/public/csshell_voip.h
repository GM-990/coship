#ifndef CSSHELL_VOIP_H
#define CSSHELL_VOIP_H

#include "mmcp_typedef.h"
#include "udi2_rdi.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSSHELLVOIP_USERID_LEN	128 		///< VOIP用户名长度
#define CSSHELLVOIP_PWD_LEN 	128 			///< VOIP用户密码长度

/**@brief  会话类型枚举  */
typedef enum 
{
	CSSHELLVOIP_Session_Video = 0,   ///<视频通话
	CSSHELLVOIP_Session_Audio = 1    ///<音频通话
	
}CSShellVoipSessionType_E;

/**@brief  会话信息结构体 */
typedef struct 
{
	CSShellVoipSessionType_E m_eCallType;
	char m_acUserId[CSSHELLVOIP_USERID_LEN];       ///< 用户ID ,允许为空 '\0'	
}CSShellVoipSessionInfo_S;

/**@brief  登录注销消息类型 */
typedef enum 
{
	EM_SHELL_VOIP_TYPE_START           = 50,      										///< voip 类型起始           
	EM_SHELL_VOIP_TYPE_LOGIN_SUCCESS 		=EM_SHELL_VOIP_TYPE_START+1,	///< voip 登录成功消息
	EM_SHELL_VOIP_TYPE_LOGIN_FAIL                 	=EM_SHELL_VOIP_TYPE_START+2,	///< voip 登录失败消息
	EM_SHELL_VOIP_TYPE_LCLLOGOUT_SUCCESS     =EM_SHELL_VOIP_TYPE_START+3,	///< voip注销成功消息
	EM_SHELL_VOIP_TYPE_SRVLOGOUT_SUCCESS   =EM_SHELL_VOIP_TYPE_START+4	///< voip 注销失败消息或被服务端踢出
	
}CSShellVoipLoginoutType_E;


/**@brief  登录注销消息枚举 */
typedef enum 
{
	/** @brief VOIP status code no error. */
	EM_SHELL_VOIP_ERR_NO                     =100,

	/** @brief VOIP status code of register error. */
	EM_SHELL_VOIP_ERR_LOCAL_REQ              	=EM_SHELL_VOIP_ERR_NO+101,	///本地请求错误. 
	EM_SHELL_VOIP_ERR_REG_SEND_MSG           	=EM_SHELL_VOIP_ERR_NO+102,	///发送注册消息错误 
	EM_SHELL_VOIP_ERR_REG_INVALID_USER       	=EM_SHELL_VOIP_ERR_NO+103, 	///无效的用户名
	EM_SHELL_VOIP_ERR_REG_TIMEOUT           	=EM_SHELL_VOIP_ERR_NO+104, 	///超时
	EM_SHELL_VOIP_ERR_REG_SERV_BUSY          	=EM_SHELL_VOIP_ERR_NO+105, 	///服务器忙 
	EM_SHELL_VOIP_ERR_REG_SERV_NOT_REACH  =EM_SHELL_VOIP_ERR_NO+106, 	///服务器不可达
	EM_SHELL_VOIP_ERR_REG_SRV_FORBIDDEN     =EM_SHELL_VOIP_ERR_NO+107, 	///禁止注册
	EM_SHELL_VOIP_ERR_REG_SRV_UNAVAILIABLE=EM_SHELL_VOIP_ERR_NO+108, 	///brief Register unavailiable. */
	EM_SHELL_VOIP_ERR_REG_OTHER              	=EM_SHELL_VOIP_ERR_NO+109 	///其他错误 
}CSShellVoipLoginoutEvent_E;

/**@brief  会话消息枚举 */
typedef enum 
{
	EM_SHELL_VOIP_SESS_CALLIN		=EM_SHELL_VOIP_ERR_NO+201,		///新的来电		
	EM_SHELL_VOIP_SESS_CALLOUT		=EM_SHELL_VOIP_ERR_NO+202,		///呼叫已呼出
	EM_SHELL_VOIP_SESS_ALERTED		=EM_SHELL_VOIP_ERR_NO+203,		///对方已响铃
	EM_SHELL_VOIP_SESS_TALKING		=EM_SHELL_VOIP_ERR_NO+204,		///正在通话中
	EM_SHELL_VOIP_SESS_TERM			=EM_SHELL_VOIP_ERR_NO+205,		///会话结束
	EM_SHELL_VOIP_SESS_HOLDOK		=EM_SHELL_VOIP_ERR_NO+206,		///保持通话成功
	EM_SHELL_VOIP_SESS_UNHOLDOK	=EM_SHELL_VOIP_ERR_NO+207,		///解除保持通话成功
	EM_SHELL_VOIP_POSTSRVEVNT		=EM_SHELL_VOIP_ERR_NO+208		///
}CSShellVoipSessionEvent_E;

/**@brief  电话机消息枚举 */
typedef enum 
{
	EM_SHELL_VOIP_PUSH_KEY			=EM_SHELL_VOIP_ERR_NO+301,								
	EM_SHELL_VOIP_HOOKOFFON		=EM_SHELL_VOIP_ERR_NO+302	
}CSShellVoipPhoneEvent_E;

/**@brief  音频设备类型枚举 */
typedef enum 
{
	EM_SHELL_VOIP_AUDIODEV_SLIC	= 0,			///< slic 音频设备  
	EM_SHELL_VOIP_AUDIODEV_USBMIC = 1,		///< usb mic 音频设备  
	EM_SHELL_VOIP_AUDIODEV_USBWireless = 2,	///< usb 无线电话
	EM_SHELL_VOIP_AUDIODEV_USBCard = 3, 		///< usb 声卡
	EM_SHELL_VOIP_AUDIODEV_COUNT			///< 声卡设备个数
	
}CSShellVoipAudioDeviceType_E;

/**@brief  音频设备信息结构体 */
typedef struct 
{
	DWORD	m_dwDeviceId;
	CSShellVoipAudioDeviceType_E m_eDeviceType;
	DWORD	m_dwCardNo;
	char	m_caName[32]; 
	
}CSShellVoipAudioDeviceInfo_S;

/**@brief  截屏上传的类型 */
typedef enum 
{
	EM_SHELL_VOIP_SCREENCUT_TYPE_OSD	= 0,			///<OSD层
	EM_SHELL_VOIP_SCREENCUT_TYPE_VIDEO,		            ///<video视频层
	EM_SHELL_VOIP_SCREENCUT_TYPE_CAMERA,	                ///< 外设摄像头视频层
	EM_SHELL_VOIP_SCREENCUT_TYPE_ALL, 		            ///<osd和video混合截屏，代表屏幕上所有可以看到的信息
	EM_SHELL_VOIP_SCREENCUT_TYPE_END
}CSShellVoipSrceenCutType_E;

/**@brief 音视频录制的类型 */
typedef enum 
{
	EM_SHELL_VOIP_AVRECORD_TYPE_ALL	= 0,			    ///<同时录制音视频
	EM_SHELL_VOIP_AVRECORD_TYPE_VIDEO,		            ///<只录制视频
	EM_SHELL_VOIP_AVRECORD_TYPE_AUDIO,	                ///< 只录制音频
	EM_SHELL_VOIP_AVRECORD_TYPE_END
}CSShellVoipAvRecordType_E;

/*
*@brief voip初始化参数,定义了一系列函数指针，这些函数指针由应用的voip实现(C代码)，然后注册给shell。
@see OTAInvoke*/

/**
@brief     voip登录注销回调函数。
@param[in] hVoip  voip句柄
@param[in] eType 事件类型  见CSShellVoipLoginoutType_E
@param[in] eCode 事件类型   见CSShellVoipLoginoutEvent_E
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 无。
*/
typedef int (*CSShellVOIPLoginoutCallback_F)(HCSHANDLE hVoip, CSShellVoipLoginoutType_E  eType, CSShellVoipLoginoutEvent_E eCode, DWORD dwParam, void *pvUserData);
/**
@brief     voip会话回调函数。
@param[in] hVoip  voip句柄
@param[in] hSession 会话句柄  
@param[in] eEvent 事件类型   见CSShellVoipSessionEvent_E
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 无。
*/
typedef int (*CSShellVOIPSessionCallback_F)(HCSHANDLE hVoip, HCSHANDLE hSession, CSShellVoipSessionEvent_E eEvent, DWORD dwParam, void * pvUserData);

/**
@brief     voip电话机回调函数。
@param[in] hVoip  voip句柄
@param[in] eEvent 事件类型   见CSShellVoipPhoneEvent_E
@param[in] nKeyVal  按键的键值
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 无。
*/
typedef int (*CSShellVOIPPhoneCallback_F)(HCSHANDLE hVoip, CSShellVoipPhoneEvent_E eEvent, int nKeyVal, DWORD dwParam, void * pvUserData);

/**
@brief     voip音频设备回调
@param[in] eDeviceType 设备类型，详见 CSShellVoipAudioDeviceType_E
@param[in] eType 事件类型，详见 CSUDIRDIEvent_E 
@param[in] dwDeviceId 设备id 
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 无。
*/
typedef int (*CSShellVOIPAudioDeviceCallback_F)(CSShellVoipAudioDeviceType_E eDeviceType, CSUDIRDIEvent_E eType, DWORD dwDeviceId, DWORD dwParam, void * pvUserData);

typedef struct 
{
	int m_nSize;	///<本结构大小，一定等于sizeof(CSShellVOIPInitParam)
	/**
	@brief	 初始化一个voip  实例
	@return VOIP实例句柄，失败返回NULL
	*/
	HCSHANDLE (* m_fnOpen)();
		
	/**
	@brief	 销毁VOIP实例	
	@param[in] hVoip  voip	 句柄
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	
	*/ 
	int (* m_fnClose)(HCSHANDLE hVoip);
	/**
	@brief	 注册voip通知回调函数。
	@param[in] hVoip  voip 句柄
	@param[in] fnCb voip 回调处理函数
	@param[in] pvEventData 用户自定义数据，注册回调函数时传入的
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	// 可以注册多回调
	int (* m_fnAddLoginoutListener)(HCSHANDLE hVoip ,  CSShellVOIPLoginoutCallback_F  fnCb, void* pUserData);
	
	/**
	@brief	 删除voip回调函数。
	@param[in] hVoip  voip 句柄
	@param[in] fnCb   voip 回调处理函数
	@param[in] pvEventData 用户自定义数据，注册回调函数时传入的
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnRemoveLoginoutListener)(HCSHANDLE hVoip, CSShellVOIPLoginoutCallback_F fnCb, void* pUserData);
	/**
	@brief	 注册voip通知回调函数。
	@param[in] hVoip  voip 句柄
	@param[in] fnCb  voip 回调处理函数
	@param[in] pvEventData 用户自定义数据，注册回调函数时传入的
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	// 可以注册多回调
	int (* m_fnAddSessionListener)(HCSHANDLE hVoip, CSShellVOIPSessionCallback_F fnCb, void* pUserData);
	
	/**
	@brief	 删除voip回调函数。
	@param[in] hVoip  voip 句柄
	@param[in] fnCb   voip 回调处理函数
	@param[in] pvEventData 用户自定义数据，注册回调函数时传入的
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnRemoveSessionListener)(HCSHANDLE hVoip, CSShellVOIPSessionCallback_F fnCb, void* pUserData);
	/**
	@brief	 注册voip通知回调函数。
	@param[in] hVoip  voip 句柄
	@param[in] fnCb   voip 回调处理函数
	@param[in] pvEventData 用户自定义数据，注册回调函数时传入的
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	// 可以注册多回调
	int (* m_fnAddPhoneListener)(HCSHANDLE hVoip, CSShellVOIPPhoneCallback_F fnCb, void* pUserData);
	
	/**
	@brief	 删除voip回调函数。
	@param[in] hVoip  voip 句柄
	@param[in] fnCb   voip 回调处理函数
	@param[in] pvEventData 用户自定义数据，注册回调函数时传入的
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnRemovePhoneListener)(HCSHANDLE hVoip, CSShellVOIPPhoneCallback_F fnCb, void* pUserData);
	
	/**
	@brief	 voip用户登录。异步接口
	@param[in] hVoip  voip 句柄
	@param[in] pszUserId 用户名，长度不能超过CSSHELLVOIP_USERID_LEN
	@param[in] pszPassWord 用户密码,长度不能超过CSSHELLVOIP_PWD_LEN
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note  异步接口，触发login 的回调
	*/
	int (* m_fnLogin)(HCSHANDLE hVoip, const char *pcUserId, const char *pcPassWord);
	
	/**
	@brief	 注销当前用户
	@param[in] hVoip  voip 句柄
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note  异步接口，触发logout 的回调
	*/
	int (* m_fnLogout)(HCSHANDLE hVoip);
	
	
	/**
	@brief	 发起通话。本过程是一个异步过程，会话建立成功，
			会有对应session 消息抛上来。
	@param[in] hVoip  voip	 句柄
	@param[in] pszUserID 对方用户ID,长度不能超过CSSHELLVOIP_USERID_LEN
	@param[in] eType 呼叫类型 见eType
	@param[out] phSession 会话句柄，用于获取会话参数。
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note  异步接口，触发session 的回调
	*/
	int (* m_fnCall)(HCSHANDLE hVoip,const char * pcUserID, CSShellVoipSessionType_E eType, HCSHANDLE *phSession);
	
	/**
	@brief	 发起通话。本过程是一个异步过程，会话建立成功，
			会有对应session 消息抛上来。
	@param[in] hVoip  voip	 句柄
	@param[in] pszIP， 对方IP地址
	@param[in] eType 呼叫类型 见eType
	@param[out] phSession 会话句柄，用于获取会话参数。
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note  异步接口，触发session 的回调
	*/
	int (* m_fnCallIP)(HCSHANDLE hVoip, const char *pcIP, CSShellVoipSessionType_E eType, HCSHANDLE *phSession);
	/**
	@brief	 获取会话信息。
	@param[in] hSession  Session 句柄
	@param[out] psSessionInfo Session信息 见CSSHELLVOIPSessionInfo
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnGetSessionInfo)(HCSHANDLE hSession, CSShellVoipSessionInfo_S *psSessionInfo);
	/**
	@brief	 获取会话对方是否有视频输出。
	
	@param[in] hSession  Session 句柄
	@return  有视频输出返回TRUE；否则返回FALSE。
	@note	 对方发出会话邀请，如果对方是发出视频会话邀请，返回TRUE
			 主动发起会话邀请，如果对方应答位视频会话，返回TRUE
	*/
	BOOL (* m_fnHasVideo)(HCSHANDLE hSession);
	/**
	@brief	 voip接听呼叫。
	@param[in] hSession  会话句柄，由回调传上来。
	@param[in] eType 见 CSShellVoipSessionType_E
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note  异步接口，触发session 的回调
	*/
	int (* m_fnAnswer)(HCSHANDLE hSession, CSShellVoipSessionType_E eType);
	
	
	/**
	@brief	 voip停止会话函数。
	@param[in] hSession  会话句柄，由回调传上来。
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note  异步接口，触发session 的回调
	*/
	int (* m_fnBye) (HCSHANDLE  hSession);
	
	/**
	@brief	 voip设置本地视频窗口大小
	@param[in] hVoip  voip	 句柄
	@param[in] x 窗口起始横坐标
	@param[in] y 窗口起始纵坐标
	@param[in] w 窗口宽度
	@param[in] h 窗口高度
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetLocalVideoPos) (HCSHANDLE hVoip, int x,int y,int w, int h);

	/**
	@brief	 voip设置对方视频窗口大小
	@param[in] hSession  会话句柄
	@param[in] x 窗口起始横坐标
	@param[in] y 窗口起始纵坐标
	@param[in] w 窗口宽度
	@param[in] h 窗口高度
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetRemoteVideoPos) (HCSHANDLE hSession, int x,int y,int w, int h);
	
	/**
	@brief	 voip设置本地视频是否可见
	@param[in] hVoip  voip	 句柄
	@param[in] bVisible 是否显示视频
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	
	int (* m_fnSetLocalVideoVisible )( HCSHANDLE hVoip, BOOL  bVisible);

	/**
	@brief	 voip设置本地视频是否可见
	@param[in] hSession  会话句柄
	@param[in] bVisible 是否显示视频
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	
	int (* m_fnSetRemoteVideoVisible )( HCSHANDLE hSession, BOOL  bVisible);	
	/**
	@brief	 voip设置窗口Z序。
	@param[in] pnUser	用户数组的指针，索引小的标示在前方显示。  
					0 标示本地， 其它值即 hSession 句柄代表的会话对端
	@param[in] nNum   用户数组的长度。
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetVideoZOrder )(HCSHANDLE  hVoip, int *pnUser,  int nNum);
	
	/**
	@brief	 voip设置本地(输入)静音
	@param[in] hVoip  voip	 句柄
	@param[in] bMute 是否静音
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetMicMute) (HCSHANDLE hVoip, BOOL  bMute);

	/**
	@brief	 voip设置对方(输出)静音
	@param[in] hSession  会话句柄
	@param[in] bMute 是否静音
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetSpkMute) (HCSHANDLE  hSession, BOOL  bMute);	
	/**
	@brief	 voip设置本地（麦克风）音量 
	@param[in] hVoip  voip	 句柄
	@param[in] nVolume 设置音量值
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetMicVolume) (HCSHANDLE  hVoip, int nVolume);
	
	/**
	@brief	 voip设置对方（喇叭）音量  
	@param[in] hSession  会话句柄
	@param[in] nVolume 设置音量值
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetSpkVolume) (HCSHANDLE  hSession, int nVolume);
	
	/**
	@brief	 voip呼叫是否保持
	@param[in] hSession  Session会话句柄
	@param[in] bHold   是否进行呼叫保持
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	@note  异步接口，触发session 的回调
	*/
	int (* m_fnHold )(HCSHANDLE hSession, BOOL bHold);
	
	/**
	@brief	 voip设置代理服务器
	@param[in] hVoip  voip	 句柄
	@param[in] pszProxyServer	代理服务器地址
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetProxyServer) (HCSHANDLE  hVoip, const char *pcProxyServer);
	/**
	@brief	 voip设置注册服务器
	@param[in] hVoip  voip	 句柄
	@param[in] pszRegisterServer   注册服务器地址
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetRegisterServer) (HCSHANDLE  hVoip, const char *pcRegisterServer);

	/**
	@brief	 得到指定设备的信息
	@param[out] ppsAudioDeviceInfo, 设备的信息
	@param[in] pnDevicesCount,输入数组长度
	@param[out] pnDevicesCount,输出设备的总数
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnGetAudioDeviceInfo)(CSShellVoipAudioDeviceInfo_S *psAudioDeviceInfo, int *pnDevicesCount);

	/**
	@brief	 设置当前可用的设备
	@param[in] hVoip  voip	 句柄
	@param[in] dwDeviceId 设备的id,通过m_fnGetAudioDeviceInfo 得到
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnSetActiveDevice)(HCSHANDLE  hVoip, DWORD dwDeviceId);
	
	/**
	@brief	 注册设备监听
	@param[in] hVoip  voip	 句柄
	@param[in] fnCb   回调函数
	@param[in] pvEventData 用户自定义数据，注册回调函数时传入的
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnAddAudoDeviceListener) (HCSHANDLE  hVoip, CSShellVOIPAudioDeviceCallback_F fnCb, void* pUserData);

	/**
	@brief	 删除设备监听
	@param[in] hVoip  voip	 句柄
	@param[in] fnCb   回调函数
	@param[in] pvEventData 用户自定义数据，注册回调函数时传入的
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnRemoveAudoDeviceListener) (HCSHANDLE  hVoip, CSShellVOIPAudioDeviceCallback_F fnCb, void* pUserData);

	/**
	@brief	 发送二次拨号键值
	@param[in] hVoip  voip	 句柄
	@param[in] ucType   拨号键值
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnTone)(HCSHANDLE hVoip, BYTE ucType);

	/**
	@brief	 获取指定用户的在线状态
	@param[in] hVoip  voip	 句柄
	@param[in] pcUserId   用户ID
	@return 0代表离线，1代表在线
	*/
	int (* m_fnGetOnlineState)(HCSHANDLE hVoip, const char* pcUserId);

	/**
	@brief	 上传截屏获取的osd、video信息上传到指定服务器地址。
	@param[in] hVoip  voip	 句柄
	@param[in] eUploadType  截屏上传的类型
	@param[in] pcUrl  指定服务器地址，上传方式为http post方式，格式为: host:port， 默认端口为80
	@note 本接口为同步接口
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnScreenCutUpload)(HCSHANDLE hVoip, CSShellVoipSrceenCutType_E eSrceenCutType, char* pcUrl);

	/**
	@brief	 开始录制音频、视频或音视频信息，并将录制结果上传到指定服务器地址。
	@param[in] hVoip  voip	 句柄
	@param[in] eAvRecordType  录制音视频的类型
	@param[in] pcUrl  指定服务器地址，上传方式为http post方式，格式为: host:port， 默认端口为80
	@note 本接口为异步接口
	@note 边上传边录制
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnAvRecordStart)(HCSHANDLE hVoip, CSShellVoipAvRecordType_E eAvRecordType, char* pcUrl);

	/**
	@brief	 暂停录制
	@param[in] hVoip  voip	 句柄
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnAvRecordPause)(HCSHANDLE hVoip);

	/**
	@brief	 恢复录制
	@param[in] hVoip  voip	 句柄
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnAvRecordResume)(HCSHANDLE hVoip);

	/**
	@brief	 停止录制，重新开始录制必须调用start
	@param[in] hVoip  voip	 句柄
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	int (* m_fnAvRecordStop)(HCSHANDLE hVoip);
}CSShellVoipInitParam_S;

/**
@brief 设置 VOIP 接口参数
@param psParam  VOIP  Shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSShellVoipSetParam(CSShellVoipInitParam_S * psParam);

/**
@brief 获取 VOIP 接口参数
@param 无
@成功返回CSShellVoipInitParam_S,失败返回NULL
*/
const CSShellVoipInitParam_S* CSShellVoipgetParam();


#ifdef __cplusplus
}
#endif

#endif

