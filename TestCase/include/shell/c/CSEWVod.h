/**@defgroup EW200VOD EW200VOD

@brief 本模块为百汇200中间件的VOD部分应用层接口,供集成百汇200产品时使用
@note 如果您使用的发布版本不支持VOD功能,以下接口将返回失败
@note 整个百汇200依赖UDI,部分未标注申明,请参见UDI文档
@{
*/
#ifndef _CS_EW_VOD_H_
#define _CS_EW_VOD_H_

#include "udi2_typedef.h"
#include "CSEWPublic.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DEFAULT_PLAY_ID         (0)       ///< PlayId默认值,无法确定使用哪个PlayId或者只支持一路播放的VOD系统上,建议使用该值
#define VOD_MSG_START		(0x0)

/**@brief 定义百汇200 VOD与应用层交互的消息类型

不同消息,可能会附带不同的参数,详细参数见条目的说明
除非特别说明,所有指针型参数指向的内容在消息返回后不再有效
*/
typedef enum
{
    EM_EW_MSG_VOD_PLAYSTATE_PAUSESUCC = VOD_MSG_START + 0,     ///< 暂停成功,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_PAUSEFAILED,   ///< 暂停失败,p0=PlayId(int),p1=错误代码(int)
    EM_EW_MSG_VOD_PLAYSTATE_RESUMESUCC,    ///< 恢复播放成功,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_RESUMEFAILED,  ///< 恢复播放失败,p0=PlayId(int),p1=错误代码(int)
    EM_EW_MSG_VOD_PLAYSTATE_SETRATESUCC,   ///< 设置播放速度成功,p0=PlayId(int),p1=实际速率(int)可能与设置的值不完全一样
    EM_EW_MSG_VOD_PLAYSTATE_SETRATEFAILED = VOD_MSG_START + 5, ///< 设置播放速度失败,p0=PlayId(int),p1=错误代码(int)
    EM_EW_MSG_VOD_PLAYSTATE_SEEKSUCC,      ///< 设置时间成功,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_SEEKFAILED,    ///< 设置时间失败,p0=PlayId(int),p1=错误代码(int)
    EM_EW_MSG_VOD_PLAYSTATE_SERVERERROR = VOD_MSG_START + 10 ,  ///< 服务器出错,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_TOSTART,       ///< 节目到达开始处,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_TOEND,         ///< 节目到达结尾处,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_REDIRECT,		///< 节目重定向连接到另一个服务器,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_OPTIONS,		///<  请求rtsp
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_GETPARAMETER =  VOD_MSG_START + 15,  ///< 从客户端得到的参数
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_SETPARAMETER,        ///<  获取参数
    EM_EW_MSG_VOD_PLAYSTATE_DESCRIBE_SUCC,			///<   对媒体对象的描述正确
    EM_EW_MSG_VOD_PLAYSTATE_DESCRIBE_FAIL,			///< 对媒体对象的描述错误
    EM_EW_MSG_VOD_PLAYSTATE_SETUP_SUCC,				///< 与服务器建立连接成功
    EM_EW_MSG_VOD_PLAYSTATE_SETUP_FAIL = VOD_MSG_START + 20,			///< 与服务器建立连接失败
    EM_EW_MSG_VOD_PLAYSTATE_AUTH_SUCC,   				///< 鉴权成功
    EM_EW_MSG_VOD_PLAYSTATE_AUTH_FAIL, 				///< 鉴权失败
    EM_EW_MSG_VOD_PLAYSTATE_SERVER_NODATA, 			///<从服务器端收不到数据
    EM_EW_MSG_VOD_PLAYSTATE_LIVE_TO_TIMESHIFT, 		///<IPTV 直播到时移消息
    EM_EW_MSG_VOD_PLAYSTATE_TIMESHIFT_TO_LIVE = VOD_MSG_START + 25,	///IPTV 直播到时移消息
    EM_EW_MSG_VOD_PLAYSTATE_SAVE_BOOK_MARK,
    EM_EW_MSG_VOD_PLAYSTATE_GET_START_TIME,                 ///< 获取节目开始时间
    EM_EW_MSG_VOD_PLAYSTATE_PLAYSUCC,		///< 播放成功,p0=PlayId(int)
    EM_EW_MSG_VOD_PLAYSTATE_SHAKEHAND_TIMEOUT= VOD_MSG_START + 30 ,  ///< 播放过程中每隔50s向服务器发一次请求，收不到时候，返回这个消息
    EM_EW_MSG_VOD_PLAYSTATE_INVALID ,	///<
    ///<以下为新增加的消息
    EM_EW_MSG_VOD_CONNECT_SUCCESS= VOD_MSG_START + 90	,	///< 锁频成功
    EM_EW_MSG_VOD_CONNECT_FAILED,		///< 锁频失败
    EM_EW_MSG_VOD_NO_PROGRAME ,   ///< 无节目
    EM_EW_MSG_VOD_LOAD_SUCCESS,   ///< 加载PAT、PMT成功
    EM_EW_MSG_VOD_NOT_SUPPORT,    ///< 节目类型不支持
    EM_EW_MSG_VOD_FRONTTS_STOP,  ///< 前端停播，无码流
    EM_EW_MSG_VOD_FRONTTS_RUN,   ///< 前端恢复播放
    EM_EW_MSG_VOD_PLAYSTATE_READY = VOD_MSG_START + 0x1000 , ///< 播放预备,EPG中点击VOD播放链接后若需要产生应用控制的播放界面会产生该消息.p0=PlayURL(const char*),时移播放不会产生该消息
    EM_EW_MSG_VOD_PLAYSTATE_PLAYFAILED,    ///< 播放失败,p0=PlayId(int)   
    EM_EW_MSG_VOD_CARD_VALID = VOD_MSG_START + 0x1100 ,   ///< 卡有效，插入的卡被正确读取才是有效的
    EM_EW_MSG_VOD_CARD_INVALID,   ///< 卡无效，卡拔出，插入错误卡
    EM_EW_MSG_VOD_CARD_PREPARING  ///<卡已经插入，但是还没有获取到卡号，适用于异步获取卡号，并且需要时间较长的CA，如irdeto
}
CSEWVODMsgType_E;



/**
@brief 定义服务器类型
@see CSEWVODOpenHomePage
    */
typedef enum
{
    EM_EW_UNDEFINED,                    ///< 未定义
    EM_EW_VOD_EPG,                       ///< VOD IEPG服务器
    EM_EW_VOD_RTV_URL,                   ///< 精彩回放访问URL
    EM_EW_VOD_FAVORITE_URL,              ///< 收藏服务访问URL
    EM_EW_VOD_PORTAL_URL,                ///< 主页访问URL
    EM_EW_VOD_UPGRADE_URL,               ///< IP访问升级URL
    EM_EW_VOD_SHORTCUT_URL               ///<  快捷键URL
} CSEWVODServerType_E;

/**
@brief 百汇200 VOD Event信息数据结构

存储一个Event
*/
typedef struct
{
    char aEventName[64];                  ///< 节目名称,如新闻联播等.如果没有节目名信息则返回.编码方式由前端决定，请与前端确认。
    int nStartTime;                       ///< 节目开始时间,时移以CSEWVODGetSoftTime为基准定位,点播节目为0.
    int nEndTime;                         ///< 节目结束时间,时移以CSEWVODGetSoftTime为基准定位,点播节目为时长.
} CSEWVODEventInfo_S;

/**
@brief 百汇200 VOD EPG信息数据结构

存储多个Event
@note nEventTotalCnt为实际的节目个数，nEventAvailCnt为有效的节目个数，小于等于nEventTotalCnt，
大于等于nEventArrayLen的值。当实际个数小于应用设置的nEventArrayLen值时，nEventTotalCnt等于nEventAvailCnt，
都等于实际个数，nEventArrayLen值不变；当实际个数大于应用设置的nEventArrayLen值时，nEventTotalCnt为实际个数，
nEventAvailCnt等于nEventArrayLen，为应用设置的值。
@code
应用使用事例代码:
CSEWVODEpgInfo_S *s_pEPGInfo;
//CSEWVODEventInfo_S为变长数组，其中EVENTCOUNT为应用需要的节目个数，即数组的长度

if (s_pEPGInfo == NULL)
     s_pEPGInfo = (CSEWVODEpgInfo_S *)CSMalloc(sizeof(CSEWVODEpgInfo_S)+sizeof(CSEWVODEventInfo_S)*(EVENTCOUNT-1));

if(s_pEPGInfo != NULL)
{
	s_pEPGInfo->nEventArrayLen = EVENTCOUNT;
	s_pEPGInfo->nEventTotalCnt = 0;
	s_pEPGInfo->nEventAvailCnt = 0;
}
@endcode
@see CSEWVODGetEpgEvent
*/
typedef struct
{
    int nEventTotalCnt;  ///< EPG中Event的总个数
    int nEventAvailCnt;  ///< EPG中有效的Event个数
    int nEventArrayLen;  ///< EPG中asEvents数组的长度，由应用指定
    CSEWVODEventInfo_S asEvents[1];     ///< CSEWVODEventInfo_S的数组,变长数组。应用根据需要申请asEvents数组的大小和设置nEventArrayLen的值
} CSEWVODEpgInfo_S;


/**
@brief 百汇200 VOD Correlate节目信息数据结构

存储一个关联Event的URL和其Event信息
@see CSEWVODGetCorrelateEventByIndex
*/
typedef struct
{
    char aCorrelateUrl[1024];     ///< 存储节目的URL
    CSEWVODEventInfo_S sEventInfo;	   ///< 节目的基本信息，包括其节目名称，开始和结束时间
} CSEWVODCorrelateEvtInfo_S;


/**
@brief 注册VOD消息回调函数

注册后百汇200 VOD相关的消息会通过该接口通知到应用.
@param[in] pUserData 用户数据,会在回调中原样返回
@return 成功返回CSUDI_TRUE,失败返回CSUDI_TRUE
@note 回调函数的dwMsg参数在CSEWVODMsgType_E中取值
*/
CSUDI_BOOL CSEWVODRegisterCallback(CSEWCallback_F fnCallback, void* pUserData);




/**
@brief VOD快速登陆接口

仅完成登录VOD过程中鉴权等必要操作，一般来说完成后可进入iEPG、可获得VOD信息等。
完整的登录过程还包括获取区域码，在成功获取区域码之前，点播操作会失败。
@return 成功返回CSUDI_TRUE,失败返回CSUDI_TRUE
*/
CSUDI_BOOL CSEWVODFastLogin(void);
/**
@brief VOD登陆-获取区域码接口

成功鉴权后使用该接口获取区域码，之后方能进行点播操作。
完成的登录过程包括鉴权和获取区域码两个步骤。
@note 未获得区域码时该接口会去前端获取区域码，已获取区域码后该接口仅会返回缓存的区域码，若需要重新获取需先Logout。
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWVODGetAreaCode(void);

/**
    @brief VOD退出登录接口

    用于需要退出VOD。调用该接口后，可以清空保存的VOD的认证信息。
    @return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
    @note 当调用CSEWVODFastLogin()登录成功后，需要更换服务器或者重新登录时，可调用该CSEWVODLogOut()接口，清空登录信息
    */
CSUDI_BOOL CSEWVODLogOut(void);



/**
@brief 按URL播放一个VOD节目

目前支持的URL类型包括:
 - 来自EPG页面链接形式的URL,这种形式的URL一般由百汇200自行从浏览器提取并自动开始播放
 - 直播节目URL,用于直播回放类节目的播放.其形式为:"dvb://networkid.tsid.serviceid/starttime",所有数据均采用16进制,不需要0x标记,如dvb://a.8.f/49D02FEB
 其中starttime表示节目的开始时间,该时间从UDI的CSEWVODGetSoftTime接口获取,若需从当前时间开始播放可省略starttime参数.
@param[in] nPlayId 播放Id,其值由用户自行设定,唯一标识一路VOD的播放业务,对于目前仅支持单路播放的VOD系统,简单取DEFAULT_PLAY_ID即可.
@param[in] pUrl 目标URL
@param[in] nLength URL长度
@param[in] nTimeout 超时时间,单位ms,建议超时时间大于2000ms,超过该时间仍无法连接服务器会返回失败
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@note nPlayId实际上用于标识支持多路VOD播放情况下的不同路播放,百汇不关心其取值,
但nPlayId相同的后一次Play操作会覆盖前一次;而nPlayId不同的两次Play操作不会相互影响.
如果新的nPlayId与正在播放的nPlayId不同,VOD系统又无法分配新的播放通道,本函数会返回失败.
*/
CSUDI_BOOL CSEWVODPlayStream(int nPlayId, const char* pUrl, int nLength, int nTimeout);

/**
@brief 停止播放一个VOD节目

停止当前的VOD播放,并断开服务器连接
@param[in] nPlayId 播放Id,必须与播放时的一致
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@note 本操作会释放该nPlayId对应的资源,即成功调用本函数后,即使在仅支持一路播放的VOD系统上,也可以用新的nPlayId进行其它播放
*/
CSUDI_BOOL CSEWVODStopPlay(int nPlayId);

/**
@brief 设置一个VOD节目的播放速度

一般用于快退快进等操作
@param[in] nPlayId 播放Id,必须与播放时的一致
@param[in] nRate 播放速度
@note 输入速率1000时为1倍速前进,2000为两倍速前进,为500表示慢进,小于0表示后退
@note 一般服务器支持2,4,8,16倍数播放,如果传入的速率不支持,会自动选择最接近的速率
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWVODSetPlayRate(int nPlayId, int nRate);

/**
@brief 暂停播放一个VOD节目

@param[in] nPlayId 播放Id,必须与播放时的一致
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWVODPause(int nPlayId);

/**
@brief 恢复一个VOD节目正常播放

用于从快退快进暂停等状态恢复到正常播放状态
@param[in] nPlayId 播放Id,必须与播放时的一致
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
*/
CSUDI_BOOL CSEWVODResume(int nPlayId);


/**
@brief 设置一个VOD节目播放的时间

直接跳转到指定时间播放,主要用于VOD节目的直接定位操作
@param[in] nPlayId 播放Id,必须与播放时的一致
@param[in] nTime 播放时间,单位秒
@note 时移节目的时间以CSEWVODGetSoftTime为基准定位,点播节目时间从0开始计算
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@see CSEWVODTell
*/
CSUDI_BOOL CSEWVODSeek(int nPlayId, int nTime);

/**
@brief 获取一个VOD节目的当前播放时间

与CSEWVODSetTime的时间想对应,主要用于绘制当前播放进度,时间点等
@param[in] nPlayId 播放Id,必须与播放时的一致
@param[out] pnTime 播放时间,单位秒
@note 时移节目的时间以CSEWVODGetSoftTime为基准定位,点播节目时间从0开始计算
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@see CSEWVODSeek
*/
CSUDI_BOOL CSEWVODTell(int nPlayId, int* pnTime);

/**
@brief 获取一个VOD节目的EPG信息

用于描述VOD节目的开始时间,结束时间,节目内容等EPG信息
@param[in] nPlayId 播放Id,必须与播放时的一致
@param[out] psEpg 存储节目Event信息的内存
@note 点播节目一般只有1个Event,用于指示节目总长度
@return 成功返回CSUDI_TRUE,失败或不再有更早的Event返回CSUDI_FALSE
@note 每一次获取EPG信息,都可能会触发与服务器的交互,并更新EPG信息.
*/
CSUDI_BOOL CSEWVODGetEpgEvent(int nPlayId, CSEWVODEpgInfo_S *psEpg);

/**
@brief由URL 获取一个VOD节目的EPG信息

用于描述VOD节目的开始时间,结束时间,节目内容等EPG信息
@param[in] pUrl 目标URL,其规则与CSEWVODPlayStream接口的URL一致
@param[out] psEpg 存储节目Event信息的内存
@note 点播节目一般只有1个Event,用于指示节目总长度
@return 成功返回CSUDI_TRUE,失败或不再有更早的Event返回CSUDI_FALSE
@note 每一次获取EPG信息,都可能会触发与服务器的交互,并更新EPG信息.
*/
CSUDI_BOOL CSEWVODGetEpgEventByURL(const char* pUrl, CSEWVODEpgInfo_S *ptEvent);

/**
@brief由URL和索引获取指定的关联节目信息

用于描述关联节目的开始时间,结束时间,节目的URl等信息
@param[in] pUrl 目标URL,其规则与CSEWVODPlayStream接口的URL一致
@param[in] nIndex 目标索引,为需要获得的关联节目的序号
@param[out] psCorrelateEvent 存储指定URL、指定索引的关联节目
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@note 每一次获取关联节目信息,都可能会触发与服务器的交互,并更新关联信息.
@note 本版本中该关联节目存在问题，待修改
*/
CSUDI_BOOL CSEWVODGetCorrelateEventByIndex(const char* pUrl, int nIndex, CSEWVODCorrelateEvtInfo_S *psCorrelateEvent);


/**
@brief由配置列表的类型获得其Value信息

用于描述配置信息的配置项和其对应的值
@param[in] pcParaKey 配置的关键字pcParaKey 取值见注释
@param[out] pcParaValue 对应配置类型的值
@param[in] nParaValueLen 返回的pcParaVale的长度
@return 成功返回TRUE,失败或者没有对应的配置类型，或配置类型信息不存在返回FALSE
@note  pcParaKey的可取的值如下:
@ note  "iEPG_URL"      	  	   表示  iEPG访问url：http://domain:port domain即可以是IPV4地址，也可以是域名格式
@ note  "NTP_SERVER"   	   	   表示 时钟同步服务器地址
@ note   "RTV_URL"             	   表示  精彩回放业务访问url
@ note    "FAVORITE_URL"   	   表示 收藏服务访问url
@ note    "SEARCH_URL" 		   表示 搜索服务访问url
@ note    "PORTAL_URL"  		   表示主页访问url
@ note   "UPGRADE_URL"	    	   表示IP方式升级服务器URL
@ note   "SHORTCUT_URL"         表示快捷键URL例：http:// domain:port/shortcut?domain即可以是IPV4地址，也可以是域名格式。
@ note   "TIMESHIFT_PROVIDER"  表示扩展字段，标识时移业务功能库提供方。如果没有牵涉到第三方时移业务库，则无需配置。需要根据实际项目环境决定是否配置改参数。
@ note  "TTV_SKIPTIME" 		表示时移业务滚动条跳动时间段。但时移业务获取不到节目单，遥控器上按【上页】【下页】时，每按一次时间条按TTV_SKIPTIME配置值为单位进行跳动。单位为分钟。
@ note  "TTV_MAXSILENTTIME" 表示  时移业务最大静默期。如果时移业务连续播放超过最大静默期且中间没有任何操作，则STB提示用户是否继续回放，如果提示后，仍无人操作，则自动退出直播回放进入直播状态。单位为小时
@ note  "BTV_PERIODTIME"    表示 回放业务的最大回放有效期。从当前时间算起，最多只能回放多少时间前的节目（早于有效期前的内容不能再回放）。单位为小时
@推荐使用
*/
CSUDI_BOOL CSEWVODGetParameterInfobyKey(char * pcParaKey, char *pcParaValue, int nParaValueLen);
/**
@brief获取认证过程中的错误码

@param[out] nErrorCode 错误信息码
@return 成功返回TRUE,失败返回FALSE
@note  错误信息码对应的错误信息列表:
- 0000：激活成功 登录成功
- 0602：用户参数不能为空
- 0611：登录参数不能为空
- 0612：用户签名不能为空
- 0613：激活参数不能为空
- 0614：用户别名不能为空
- 0616：用户的标识id不能为空
- 0617：用户标识id不是数字
- 0619：硬件提供厂商不能为空
- 0622：用户的接口版本不能为空
- 0623：接口版本错误
- 0633：激活失败
- 0650：终端用户登录失败
- 0661：指定终端用户不存在
- 0662：指定用户在数据库中不存在
- 0665：用户最新版本值不存在
- 0668：终端用户还没有激活，请先激活
- 0669：输入密码错误
- 0671：检查签名失败
- 0676：版本信息不能为空
- 0678：Stb厂商提供者不能为空
- 0690：3.1认证关键字配置错误
- 0691：用户不唯一
- 0692：必需有在sic,stb,alies,mac字段中取其中一个作为身份标识
*/
CSUDI_BOOL CSEWVODGetErrorInfo(int *nErrorCode);

/**
@brief 快速识别一个URL是否为一个支持的VOD节目

主要用于判断一个节目是否支持时移,少量情况下可用于点播节目VOD的合法性验证(不推荐使用).
@param[in] pUrl 目标URL,其规则与CSEWVODPlayStream接口的URL一致
@param[in] nLength URL长度
@return 支持返回CSUDI_TRUE;不支持或者出错返回CSUDI_FALSE
@note 该接口只进行简单检查,返回TRUE并不能100%保证能播放
@see CSEWVODPlayStream
*/
CSUDI_BOOL CSEWVODIdentifyStream(const char* pUrl, int nLength);


/**
@brief VOD登陆接口
用于进行初始化成功后,正式使用任何VOD功能前的准备工作.
一般来说会在该过程中完成与服务器的认证交互工作.
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@deprecated 推荐使用CSEWVODFastLogin 和CSEWVODGetAreaCode来实现此功能
*/
CSUDI_BOOL CSEWVODLogin(void);

/**
@brief 打开VOD相关主页

用百汇200浏览器打开VOD相关页面首页
@param[in] eServerType 服务器类型
@return 成功返回CSUDI_TRUE,失败返回CSUDI_FALSE
@deprecated 推荐使用CSEWVODGetParameterInfobyKey(),如果使用，必须先调用CSEWExternAddVOD
*/
CSUDI_BOOL CSEWVODOpenHomePage(CSEWVODServerType_E eServerType);

/**
@brief由配置列表的类型获得其Value信息

用于描述配置信息的配置项和其对应的值
@param[in] eParaKey 配置信息的类型，参见CSEWVODPARAType_E
@param[in] nParaValueLen 返回的pcParaVale的长度
@param[out] pcParaValue 对应配置类型的值
@return 成功返回CSUDI_TRUE,失败或者没有对应的配置类型，或配置类型信息不存在返回CSUDI_FALSE
@note 当CSEWVODServerType_E的类型是EM_EW_VOD_EPG，不会返回pcParaValue值，而是直接打开页面，与CSEWVODOpenHomePage()的功能相同
@deprecated 该接口不推荐使用，推荐使用CSEWVODGetParameterInfobyKey
*/
CSUDI_BOOL CSEWVODGetParameterInfo(CSEWVODServerType_E eParaKey, char *pcParaValue, int nParaValueLen);


/**
@brief 获取当前系统时间

@return  成功返回自1970年1月1日凌晨起至现在的总秒数,失败返回0
@note 在未设置时间之前,认为开机时间为1970年1月1日凌晨
@note 该时间与VOD服务器保持同步，所有与VOD相关的时间均使用该时间。
*/
int  CSEWVODGetSoftTime(void);


/**
@brief 快速识别一个URL是否为一个已经订购的时移节目

主要用于判断一个节目是否已经订购格式为"dvb://networkid.tsid.serviceid"
@param[in] pcUrl 目标URL,其规则与CSEWVODPlayStream接口的URL一致
@param[in] nLength URL长度
@return 已经订购返回CSUDI_TRUE;未订购或者出错返回CSUDI_FALSE
@see CSEWVODPlayStream
*/
CSUDI_BOOL CSEWVODIsOrderd(const char* pcUrl, int nLength);
#ifdef __cplusplus
}
#endif

/** @} */

#endif//_CS_EW_VOD_H_

