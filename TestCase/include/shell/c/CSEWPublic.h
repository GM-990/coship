/**@defgroup EW200 EW200

@brief 本模块为百汇200中间件的应用层接口,供集成百汇200产品时使用
@note 整个百汇200依赖UDI,部分未标注申明,请参见UDI文档
@{
*/
#ifndef _CS_EW_PUBLIC_H_
#define _CS_EW_PUBLIC_H_

#include "udi2_typedef.h"
#include "udi2_public.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@mainpage EestWin200 User Interface

@version <B>EW200V0.4</B> 2009/03/25
- 调整接口位置，将初始化及使用接口分开，方便第三方集成及使用
- 修改CSEWVODGetEpgEvent接口,使之更方便的使用和实现
- 修改dvb协议的URL,现在该协议的URL均使用16进制表示

@version <B>EW200V0.3</B> 2009/03/20
- 根据评审意见,将EWShell拆分,使它们可以独立发布,扩展

@version <B>EW200V0.2</B> 2009/03/16
- 根据评审意见,调整模块间的级别及依赖关系

@version <B>EW200V0.1</B> 2009/03/11
- 初始版本
*/

/**
@brief 定义百汇200通用消息参数结构。具体参数含义根据消息类型确定?
@see CSEWEventType_E
@see CSEWMsgType
@note 与CSEWEventType_E一起使用时，其含义以:简称=功能(类型)的形式加以描述。具体请参考CSEWEventType_E
@note 与CSEWMsgType一起使用时，作为CSEWCallback_F参数抛出。其含义如下(以下描述中，psParam->px简称为px):
　　1) dwMsg=EM_EW_OC时，表示数据广播消息。此时,
		p0=EM_EW_OC；p1为CSEWOCMsgType（p0、p1虽然是以指针形式传下来的，但是实际上是int的，使用时请按照int数值
		来用）。该消息建议用户根据不同的情况调用中间件的弹提示框接口弹出相应提示。p2=NULL,p3=NULL。
		
　　2) dwMsg=EM_EW_NETWORK时，表示网络连接失败，此时
		p0为网络连接的错误码，见枚举CSEWNetworkMsgType。网络连接失败时，中间件有个默认处理，会打开一个"打开链接失
		败"的错误页面；p1为一个输出参数，表示用户是否要自己处理网络连接失败，如果用户要自己处理，请将该输出参数赋为
		4，中间件就不会打开错误页面了。（目前该消息的用户实现部分还存在问题，所以建议暂时使用中间件的默认处理）。
		p2=NULL,p3=NULL。

　　3) dwMsg=EM_EW_WINDOW_CLOSE时，表示中间件在页面中通过Window.close()退出浏览器，用户在收到该消息时，就可以调用CSEWHide隐
		藏中间件。此时，psParam=NULL.


    4) dwMsg=EM_EW_SYSTEM_OPENURL时，表示中间件打开一个url，该消息仅仅为通知用户，一般不需要用户处理。此时,psParam=NULL.

　　5) dwMsg=EM_EW_SYSTEM_GOBACK表示中间件进行了一次返回的操作，该消息仅仅为通知用户，一般不需要用户处理。此时,psParam=NULL.
　　6) dwMsg=EM_EW_SYSTEM_REPAINT表示中间件进行了一次重绘的操作，该消息仅仅为通知用户，一般不需要用户处理。此时,psParam=NULL.
　　7) dwMsg=EM_EW_SYSTEM_REFRESH表示中间件进行了一次刷新的操作，该消息仅仅为通知用户，一般不需要用户处理。此时,psParam=NULL.
　　8) dwMsg=EM_EW_SYSTEM_EXIT表示通知用户中间件收到了退出消息，用户在收到该消息时，需要判断pPara0，如果pPara0=0就可以调用
		CSEWHide隐藏中间件，pPara0非0则表示不需要退出，用户不需要做任何处理。
    
　　其他的消息类型，中间件暂时没有使用，用户不需要做处理。
　　其中除了dwMsg 为EM_EW_NETWORK时p1为一个输出参数，是指针，其他的情况下，p0和p1都是一个int数值，用户使用时需要强制转换为
	int数值。

@note 除非特别说明,所有指针指向的内容在消息返回后不再有效.
*/
typedef struct
{
    void* pPara0;                         ///< 参数0,简称p0
    void* pPara1;                         ///< 参数1,简称p1
    void* pPara2;                         ///< 参数2,简称p2 预留
    void* pPara3;                         ///< 参数3,简称p3 预留
} CSEWParam_S;


/**
@brief 定义百汇200通用浏览器中弹出框参数结构
在弹出框中 ，对确认，取消，超时的处理
*/
typedef enum _CSEWMsgActionType_E
{
    EM_EW_BUTTON_ENTER = 1,  	///< 处理OK 事件
    EM_EW_BUTTON_CANCEL,		///< 处理CANCEL 事件
    EM_EW_BUTTON_TIMEOUT	        ///< 处理超时 事件
} CSEWMsgActionType_E;

/**
@brief 定义百汇200通用浏览器中弹出框参数结构
弹出框中按钮的个数
*/
typedef enum _CSEWMsgBoxType_E
{
    EM_EW_BUTTON_NULL 	= 0,	///< 弹出框类型，无button
    EM_EW_BUTTON_OK 	= 1,	/// 弹出框类型，ok button
    EM_EW_BUTTON_OKCANCEL	= 2	///< 弹出框类型，Cancel sure button
} CSEWMsgBoxType_E;

/**
 @brief 定义百汇200通用浏览器中弹出框参数结构
 弹出框标题的对齐方式
 */
typedef enum _CSEWMsgBoxAlign_E
{
    EM_EW_MSG_ALIGN_LEFT	= 0,    ///< 左对齐
    EM_EW_MSG_ALIGN_CENTER	= 1,    ///< 居中
    EM_EW_MSG_ALIGN_RIGHT	= 2	///< 右对齐
} CSEWMsgBoxAlign_E;


/**@brief 百汇200输入事件类型

用于发送按键,刷新页面等事件,具体赋值根据事件类型确定.其伴随参数类型为CSEWParam_S
@see CSEWParam_S
@see CSEWSendMessagege
*/
typedef enum
{
    EM_EW_EVENT_UNDEFINED,               ///< 未定义
    EM_EW_EVENT_KEYBOARD,                ///< 按键消息,伴随参数CSEWParam_S中p0=GTKeyStatus_E(GTKeyStatus_E),p1=键值(DWORD)
    EM_EW_EVENT_CMD                      ///< 命令消息,伴随参数CSEWParam_S中p0=GTSystemCmd_E(GTSystemCmd_E)
} CSEWEventType_E;

/**
@brief 浏览器交互按键状态

用于描述按键状态,每次按键必须完整的发送Press,Release过程.
当GTEventType_E为EM_EW_EVENT_KEYBOARD时有效
@see CSEWSendMessagege
*/
typedef enum
{
    EM_EW_KEY_UNDEFINED,                 ///< 未定义
    EM_EW_KEY_PRESS,                     ///< 按键按下时发送该消息
    EM_EW_KEY_REPEATED,                  ///< 按键按下后一直按着不放时重复发送该消息
    EM_EW_KEY_RELEASE                    ///< 按键松开时发送该消息
} GTKeyStatus_E;

/**
@brief 浏览器控制命令
当GTEventType_E为EM_EW_EVENT_CMD时有效
@see CSEWSendMessagege
*/
typedef  enum
{
    EM_EW_CMD_UNDEFINED,                ///< 未定义
    EM_EW_CMD_GOBACK,                   ///< 后退
    EM_EW_CMD_GOFORWARD,                ///< 前进
    EM_EW_CMD_STOP,                     ///< 停止
    EM_EW_CMD_REFRESH,                  ///< 刷新
    EM_EW_CMD_REPAINT,                  ///< 重绘(不重新获取页面)
    EM_EW_CMD_CLEARHISTORY,             ///<清除历史记录
    EM_EW_CMD_SETHOMEPAGE,		    ///<设置主页面功能
    EM_EW_CMD_OPENURL,
    EM_EW_CMD_SHOW_MSGBOX,
    EM_EW_CMD_HIDE_MSGBOX,
    EM_EW_CMD_CLEARCACHE

} GTSystemCmd_E;

/**
@brief 定义百汇200 浏览器通知应用的消息,在CSEWCallback_F中回抛给用户。其伴随参数类型为CSEWParam_S
@see CSEWRegisterCallback 
@see CSEWCallback_F
*/
typedef enum
{
    EM_EW_OC,               ///<  当处于数据广播时候，应用接收到浏览器此消息,p1具体见CSEWOCMsgType中定义的类型
    EM_EW_NETWORK,			///<  当处于http服务中，应用接收到浏览器消息,p1具体见CSEWNetworkMsgType中定义的类型
    EM_EW_SYSTEM,			///< 浏览器中系统消,  p1 具体参见CSEWSystemMsgType 中定义
    EM_EW_KEYCODETRANS,     ///<  页面捕获键值根据项目需要进行转化，p1 浏览器默认键值输入参数，p1转化后的键值(int) 输出参数
    EM_EW_WINDOW_CLOSE,     ///<  通过Window.close() 退出浏览器
    EM_EW_SYSTEM_OPENURL,	///<  通知应用浏览器打开网页完成
    EM_EW_SYSTEM_GOBACK, 	///<  通知应用浏览器导航后退成功
    EM_EW_SYSTEM_GOFORWARD,	///<  通知应用浏览器导航前进成功
    EM_EW_SYSTEM_REPAINT,	///<  通知应用浏览器REPAINT 成功
    EM_EW_SYSTEM_REFRESH, 	///<  通知应用浏览器刷新页面成功
    EM_EW_SYSTEM_EXIT,	    ///<  通知应用浏览器退出的功能处理消息,分两种类型，p0 为0，表示退出浏览器;p0为其他值，表示退出按键交由网页控制，应用不需要处理
    EM_EW_SYSTEM_NOHISTORY  ///<  通知应用浏览已经回到首页
} CSEWMsgType;


/**
@brief 定义百汇200 消息。 在OC中，浏览器通知应用的消息类型
@note  当收到EM_EW_OC这个消息后，p1 对应的消息类型
*/
typedef enum
{
    EM_EW_CABLE_LOST = 1,      ///< Cable 信号中断
    EM_EW_CABLE_RESUME,  	   ///< Cable 信号恢复
    EM_EW_OPEN_DVB_FAIL,	   ///< 打开dvb链接失败
    EM_EW_OPEN_HOMEPAGE_FAIL,  ///< 打开dvb首页失败
    EM_EW_OPEN_DVB_TIMEOUT,    ///< 打开dvb链接超时
    EM_EW_AIT_VER_CHANGE	   ///< AIT版本变化
} CSEWOCMsgType;


/**
@brief 定义百汇200 消息。 当处于http服务中，应用接收到浏览器消息
@note  当收到EM_EW_NETWORK这个消息后，p1对应的消息类型
*/
typedef enum
{
    EM_EW_SOCKET_CONNECT_TIMEOUT    = 1001 , ///< socket连接超时
    EM_EW_COULDNT_RESOLVE_HOST      = 2001 , ///< 无法解析域名
    EM_EW_NO_DNS_SERVER             = 2002 , ///< 找不到DNS服务器
    EM_EW_NO_PROXY_SERVER           = 3001 , ///< 连接不到PROXY服务器
    EM_EW_PROXY_USER_PSD_WRONG      = 3002 , ///< PROXY用户名密码错误
    EM_EW_HTTP_STATUS_CODE_100      = 4100 , ///< HTTP状态码100
    EM_EW_HTTP_STATUS_CODE_101      = 4101 , ///< HTTP状态码101
    EM_EW_HTTP_STATUS_CODE_201      = 4201 , ///< HTTP状态码201
    EM_EW_HTTP_STATUS_CODE_202      = 4202 , ///< HTTP状态码202
    EM_EW_HTTP_STATUS_CODE_203      = 4203 , ///< HTTP状态码203
    EM_EW_HTTP_STATUS_CODE_204      = 4204 , ///< HTTP状态码204
    EM_EW_HTTP_STATUS_CODE_205      = 4205 , ///< HTTP状态码205
    EM_EW_HTTP_STATUS_CODE_206      = 4206 , ///< HTTP状态码206
    EM_EW_HTTP_STATUS_CODE_300      = 4300 , ///< HTTP状态码300
    EM_EW_HTTP_STATUS_CODE_301      = 4301 , ///< HTTP状态码301
    EM_EW_HTTP_STATUS_CODE_303      = 4303 , ///< HTTP状态码303
    EM_EW_HTTP_STATUS_CODE_304      = 4304 , ///< HTTP状态码304
    EM_EW_HTTP_STATUS_CODE_307      = 4307 , ///< HTTP状态码307
    EM_EW_HTTP_STATUS_CODE_400      = 4400 , ///< HTTP状态码400
    EM_EW_HTTP_STATUS_CODE_401      = 4401 , ///< HTTP状态码401
    EM_EW_HTTP_STATUS_CODE_402      = 4402 , ///< HTTP状态码402
    EM_EW_HTTP_STATUS_CODE_403      = 4403 , ///< HTTP状态码403
    EM_EW_HTTP_STATUS_CODE_404      = 4404 , ///< HTTP状态码404
    EM_EW_HTTP_STATUS_CODE_405      = 4405 , ///< HTTP状态码405
    EM_EW_HTTP_STATUS_CODE_406      = 4406 , ///< HTTP状态码406
    EM_EW_HTTP_STATUS_CODE_407      = 4407 , ///< HTTP状态码407
    EM_EW_HTTP_STATUS_CODE_408      = 4408 , ///< HTTP状态码408
    EM_EW_HTTP_STATUS_CODE_409      = 4409 , ///< HTTP状态码409
    EM_EW_HTTP_STATUS_CODE_410      = 4410 , ///< HTTP状态码410
    EM_EW_HTTP_STATUS_CODE_411      = 4411 , ///< HTTP状态码411
    EM_EW_HTTP_STATUS_CODE_412      = 4412 , ///< HTTP状态码412
    EM_EW_HTTP_STATUS_CODE_413      = 4413 , ///< HTTP状态码413
    EM_EW_HTTP_STATUS_CODE_414      = 4414 , ///< HTTP状态码414
    EM_EW_HTTP_STATUS_CODE_415      = 4415 , ///< HTTP状态码415
    EM_EW_HTTP_STATUS_CODE_416      = 4416 , ///< HTTP状态码416
    EM_EW_HTTP_STATUS_CODE_417      = 4417 , ///< HTTP状态码417
    EM_EW_HTTP_STATUS_CODE_500      = 4500 , ///< HTTP状态码500
    EM_EW_HTTP_STATUS_CODE_501      = 4501 , ///< HTTP状态码501
    EM_EW_HTTP_STATUS_CODE_502      = 4502 , ///< HTTP状态码502
    EM_EW_HTTP_STATUS_CODE_503      = 4503 , ///< HTTP状态码503
    EM_EW_HTTP_STATUS_CODE_504      = 4504 , ///< HTTP状态码504
    EM_EW_HTTP_STATUS_CODE_505      = 4505   ///< HTTP状态码505
} CSEWNetworkMsgType;

/**
@brief 定义百汇200 消息。
@note  当收到EM_EW_SYSTEM这个消息后，p0对应的消息类型
*/
typedef enum
{
    EM_EW_NOMEMORY,
    EM_EW_PARSERERROR,
    EM_EW_NOTINIT
} CSEWSystemMsgType;

/**
@brief 定义百汇200通用浏览器中弹出框参数结构,弹出框的处理回调
@param[in] enType 弹出框的动作
@param[in] void *pUserData 可扩展参数
*/
typedef void (*CSEWMsgBoxActionCallBack_F)(CSEWMsgActionType_E enType, void *pData, void *pUserData);

/**
@brief 定义百汇200通用弹出框属性参数结构
*/
typedef struct _CSEWMsgBoxConfig_S
{
    unsigned int			dwMsgBoxId;     ///< 用户提示框id
    unsigned int			dwTimeOut; 	///< ms 0 不超时
    char 			*pcBoxAption;   ///< 提示框标题内容
    char			*pcBoxContent;  ///< 提示框内容
    int			nBoxAptionLen;	///< 提示框标题的长度
    int 			nBoxContentLen; ///< 提示框内容的长度
    void 			*pUserdata;     ///< 用户数据，在m_fnMsgBoxCB中传回给用户
    CSUDIWinRect_S		sRect;		///< Msg 显示位置
    CSEWMsgBoxType_E 	emMsgType;  	///< type 0 - 2, button numble
    CSEWMsgBoxAlign_E       emAlign;		///< 对齐方式，left = 0; right = 1;center = 2;
    CSEWMsgBoxActionCallBack_F  fnMsgBoxCallback;	///< 弹出框的回调
} CSEWMsgBoxConfig_S;

/**
@brief 定义百汇200消息通知回调函数

百汇200将通过该回调与上层应用进行各方面的交互,如状态通知,信息获取等.
@param[in] dwMsg 消息类型,取值为CSEWMsgType中枚举
@param[in] psParam 消息参数,具体参数类型见消息类型定义
@param[in] pvUserData 用户数据,等于回调注册时的相应参数
@return 成功处理该消息返回1,否则返回0
@note 除非特别说明,请尽量不要长时间阻塞回调函数,建议将回调消息以消息形式抛出处理
*/
typedef int (*CSEWCallback_F)(unsigned int dwMsg, CSEWParam_S *psParam, void* pvUserData);



/**
@brief 弹出框接口,当收到浏览器回调消息 时候，需要弹出提示框时候使用
@param[in] psMsgBox 弹出框初始化参数，详见结构体定义
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@note 接口将对结构进行浅拷贝,函数返回后pArgList结构本身不再需要
*/
CSUDI_BOOL CSEWShowMsgBox(CSEWMsgBoxConfig_S sMsgBox);


/**
@brief 激活/隐藏百汇200中间件

当临时不需要使用百汇200时,如进入应用控制的VOD全屏播放时将其隐藏.
@param[in] bHide TRUE为激活,FALSE为隐藏
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWHide(CSUDI_BOOL bHide);

/**
@brief 百汇200消息发送接口

用于向百汇200发送各种控制信息,如按键,刷新等.
@param[in] eEventType 消息类型
@param[in] sEventParam 消息参数,详情见消息定义
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@note 当eEventType为EM_EW_EVENT_KEYBOARD 类型，需要初始化sEventParam的p0,p1;p0=GTKeyStatus_E(GTKeyStatus_E),p1=键值(DWORD)
@note 当eEventType为EM_EW_EVENT_CMD 类型，需要初始化sEventParam的p0 ,p0对应于GTSystemCmd_E
*/
CSUDI_BOOL CSEWSendMessage(CSEWEventType_E eEventType, CSEWParam_S sEventParam);

/**
@brief 使用百汇200打开一个URL

用于网页浏览
@param[in] pUrl 目标URL
@param[in] nLength URL长度
@note 调用该接口会自动将百汇200激活并将百汇浏览器至于前台
@note 标准版本包括http://、https://等开头的URL
@note 可选dvb://networkid.tsid.serviceid(16进制)打开OC形式发送的页面
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWOpenUrl(const char* pUrl, int nLength);


/**
@brief  注册浏览器给应用的回调处理函数

@param[in] fnCallback  浏览器给应用的回调处理函数,
@param[in] pUserdata 用户数据
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@note 回调函数的dwMsg参数在CSEWMsgType中取值
*/
CSUDI_BOOL CSEWRegisterCallback(CSEWCallback_F fnCallback, void *pUserdata);

/**
@brief 退出百汇200中间件

当不需要使用百汇200时，如在应用控制的VOD全屏播放时按退出键，则进入DVB界面.
应用收到遥控器退出按键，调用CSEWShowMsgBox弹出提示框，提示用户是否要退出；
应用在提示框回调里面接收消息，如果是确认就发消息到应用的线程，应用调用CSEWExit退出中间件。
@note 一定不要在提示框的回调里面直接调用CSEWExit，否则中间件线程会被阻塞。
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWExit();

#ifdef __cplusplus
}
#endif

/** @} */

#endif



