/**@defgroup GUITOOL 
@brief 
@note Copyright (c) 2008 Coship
@version 1.0  2008-03-04  hjl             Create
@{
*/

#ifndef CSGTOOLS_H
#define CSGTOOLS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WEBKIT_API 
#define WEBKIT_API
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/
#include "mmcp_typedef.h"
/******************************************************************************
 *                                 Macro/Define/Structus                       *
 ******************************************************************************/

/*浏览器发动给应用的消息类型*/
typedef enum 
{
	EM_BRW_OC,
	EM_BRW_NETWORK,
	EM_BRW_SYSTEM,
	EM_BRW_KEYCODETRANS,  // 页面捕获键值根据项目需要进行转化，dwParm1浏览器默认键值输入参数，dwParm2转化后的键值(int) 输出参数
	EM_BRW_WINDOW_CLOSE,  // 通过Window.close() 退出浏览器
	EM_BRW_SYSTEM_OPENURL,	// 通知应用浏览器打开网页完成
	EM_BRW_SYSTEM_GOBACK, // 通知应用浏览器导航后退成功
	EM_BRW_SYSTEM_GOFORWARD,//  通知应用浏览器导航前进成功
	EM_BRW_SYSTEM_REPAINT, // 通知应用浏览器REPAINT 成功
	EM_BRW_SYSTEM_REFRESH, // 通知应用浏览器刷新页面成功
	EM_BRW_SYSTEM_EXIT,	    // 通知应用浏览器退出的功能处理消息,分两种类型，参考回调"fnBrowserfun" 注释
	EM_BRW_SYSTEM_NOHISTORY, // 通知应用浏览已经回到首页
	EM_BRW_JS_APP_MSG,       //通知应用，转发dvb的系统消息
	EM_BRW_SYSTEM_lOADCOMPETED, // 通知浏览器当前网页资源下载完毕 
	EM_BRW_FILE_URL_TO_LOAD,     // 通知应用，抛出即将用file协议打开的url.通过wParam2抛出该url,url是ascii类型.fnBrowserfun回调函数的返回值是int类型，返回值为1:禁止访问该url ，0:不禁止, 默认是0							 
	EM_BRW_KEYBOARDEVENT_PROCESS  //通知应用在处理键盘消息
}eBrwMsgType;

typedef enum
{        
    EM_BRW_FILTER_INPUTBOX,               // 输入框捕获按键        
    EM_BRW_FILTER_INPUTMETHOD,            // 输入法捕获按键          
    EM_BRW_FILTER_WEBPAGE,                // 页面捕获按键并处理, 使用e.preventDefault();
    EM_BRW_NOTHING_FILTER                 // 页面捕获按键什么都没处理
}eBrwProcessType;

typedef enum
{
	EM_CABLE_LOST = 1, //Cable 信号中断，需要DVBConnectSourceCallBack_F实现正确的类型
	EM_CABLE_RESUME,  //Cable 信号恢复，需要DVBConnectSourceCallBack_F实现正确的类型
	EM_OPEN_DVB_FAIL,	//打开dvb链接失败
	EM_OPEN_HOMEPAGE_FAIL, //打开dvb首页失败
	EM_OPEN_DVB_TIMEOUT,  //打开dvb链接超时
	EM_AIT_VER_CHANGE		// AIT版本变化
}eOCMsgBoxType; //according to EM_BRW_OC

typedef enum
{
	EM_SOCKET_CONNECT_TIMEOUT    = 1001 , /**socket连接超时*/
	EM_COULDNT_RESOLVE_HOST      = 2001 , /**无法解析域名*/	
	EM_NO_DNS_SERVER             = 2002 , /**找不到DNS服务器*/	
	EM_NO_PROXY_SERVER           = 3001 , /**连接不到PROXY服务器*/
	EM_PROXY_USER_PSD_WRONG      = 3002 , /**PROXY用户名密码错误*/ 
	EM_HTTP_STATUS_CODE_100      = 4100 , /**HTTP状态码100*/
	EM_HTTP_STATUS_CODE_101      = 4101 , /**HTTP状态码101*/
	EM_HTTP_STATUS_CODE_201      = 4201 , /**HTTP状态码201*/
	EM_HTTP_STATUS_CODE_202      = 4202 , /**HTTP状态码202*/
	EM_HTTP_STATUS_CODE_203      = 4203 , /**HTTP状态码203*/
	EM_HTTP_STATUS_CODE_204      = 4204 , /**HTTP状态码204*/
	EM_HTTP_STATUS_CODE_205      = 4205 , /**HTTP状态码205*/
	EM_HTTP_STATUS_CODE_206      = 4206 , /**HTTP状态码206*/
	EM_HTTP_STATUS_CODE_300      = 4300 , /**HTTP状态码300*/
	EM_HTTP_STATUS_CODE_301      = 4301 , /**HTTP状态码301*/
	EM_HTTP_STATUS_CODE_303      = 4303 , /**HTTP状态码303*/
	EM_HTTP_STATUS_CODE_304      = 4304 , /**HTTP状态码304*/
	EM_HTTP_STATUS_CODE_307      = 4307 , /**HTTP状态码307*/
	EM_HTTP_STATUS_CODE_400      = 4400 , /**HTTP状态码400*/
	EM_HTTP_STATUS_CODE_401      = 4401 , /**HTTP状态码401*/
	EM_HTTP_STATUS_CODE_402      = 4402 , /**HTTP状态码402*/
	EM_HTTP_STATUS_CODE_403      = 4403 , /**HTTP状态码403*/
	EM_HTTP_STATUS_CODE_404      = 4404 , /**HTTP状态码404*/
	EM_HTTP_STATUS_CODE_405      = 4405 , /**HTTP状态码405*/
	EM_HTTP_STATUS_CODE_406      = 4406 , /**HTTP状态码406*/
	EM_HTTP_STATUS_CODE_407      = 4407 , /**HTTP状态码407*/
	EM_HTTP_STATUS_CODE_408      = 4408 , /**HTTP状态码408*/
	EM_HTTP_STATUS_CODE_409      = 4409 , /**HTTP状态码409*/
	EM_HTTP_STATUS_CODE_410      = 4410 , /**HTTP状态码410*/
	EM_HTTP_STATUS_CODE_411      = 4411 , /**HTTP状态码411*/
	EM_HTTP_STATUS_CODE_412      = 4412 , /**HTTP状态码412*/
	EM_HTTP_STATUS_CODE_413      = 4413 , /**HTTP状态码413*/
	EM_HTTP_STATUS_CODE_414      = 4414 , /**HTTP状态码414*/
	EM_HTTP_STATUS_CODE_415      = 4415 , /**HTTP状态码415*/
	EM_HTTP_STATUS_CODE_416      = 4416 , /**HTTP状态码416*/
	EM_HTTP_STATUS_CODE_417      = 4417 , /**HTTP状态码417*/
	EM_HTTP_STATUS_CODE_500      = 4500 , /**HTTP状态码500*/
	EM_HTTP_STATUS_CODE_501      = 4501 , /**HTTP状态码501*/
	EM_HTTP_STATUS_CODE_502      = 4502 , /**HTTP状态码502*/
	EM_HTTP_STATUS_CODE_503      = 4503 , /**HTTP状态码503*/
	EM_HTTP_STATUS_CODE_504      = 4504 , /**HTTP状态码504*/
	EM_HTTP_STATUS_CODE_505      = 4505   /**HTTP状态码505*/
}eNetworkMsgType;//according to EM_BRW_NETWORK

typedef enum
{
	EM_NOMEMORY,
	EM_PARSERERROR,
	EM_NOTINIT
}eSystemMsgType;//according to EM_BRW_SYSTEM

typedef struct _CSBRWErrorPageInfo
{
  int   nErrorPageState;     /** 应用是否打开过错误页面 */   
  const char* pzURL;               /** 出现错误的URL地址 */
}CSBRWErrorPageInfo_S;

typedef enum _CSBRWErrorPageState
{
    EM_ERROR_PAGE_UNOPEN    = -1,  /**没有打开错误页面  */
    EM_ERROR_PAGE_OPENED    =  4   /**已近打开了错误页面*/
}CSBRWErrorPageState;

typedef struct _CSBRWRect_S
{
  	int nX; 
  	int nY;
  	int nWidth;
  	int nHeight;
}CSBRWRect_S;

typedef enum _CSGTRet_E
{	
	GT_RET_FAILURE    = -1,  /**<guitool 对外接口调用失败*/
	GT_RET_SUCCESS    =  0,  /**< guitool 对外接口调用成功*/
	GT_RET_DEFAULT    =  1, /**< messagebox 回调类型扩展*/
	GT_RET_STATE_ERR  =  2,/**<  浏览器状态出错 */
	GT_RET_NO_MEMORY  =  3,   /**<  浏览器内存不足 */
	GT_RET_MEMORY_LOCK = 4 /**< 共享内存没有释放*/
}CSGTRet_E;

typedef enum _CSGTState_E   
{
	GT_STATE_FRONT   = 0, /**< 浏览器置于前台状态 */
	GT_STATE_BACK    = 1, /**<  浏览器置于后台状态 */
	GT_STATE_STANDBY = 2  /**< 浏览器释放所有网页但保留 浏览器初始化时必须内存的状态 */
}CSGTState_E;

typedef struct _CSGTBrwInfo  
{
	int nMemorySize;       /**< guitool内存池大小*/
	int nFreeMemorySize;   /**< guitool剩余可用内存大小*/
	int nCacheBufferSize;  /**< guitool 缓存大小*/
	CSGTState_E eGTState;  /**< guitool 当前状态*/ 
	BOOL bIsFocusOnInput;  /**< guitool的焦点是否为input输入框*/
}CSGTBrwInfo_S;


typedef struct _CSGTMixMemoryCfg
{
	int nBrwMemoryPoolSize;  /** < 浏览器自身内存池大小 */
	int nBrwMaxMallocSize;   /** < 浏览器内存使用阀值,
								   小于等于该值使用浏览器内存池内存,否则使用系统内存.
								  */
}CSGTMixMemoryCfg_S;

typedef struct _CSGTLocalCacheCfg
{
	int nType;  /** 1: file sys; 2: eeprom */
	void* pParam;   /** 1: 存放本地cookie的文件完整路径和文件名, char*; 2: e2prom addr, unsigned int */
	int nLen;		/* eeprom的话，这个字段表示eeprom的大小; 文件的话，表示允许的最大文件大小 >0 */
}CSGTLocalCacheCfg_S;

typedef  enum  _CSGTMsgType_E
{
	BRW_EVENT_TYPE_KEYBOARD  	= 0xB001, /**< 键盘事件*/
	BRW_EVENT_TYPE_MOUSE     	= 0xB002, /**< 鼠标事件*/
	/**@brief mmcp 专用消息类型通道 */
	BRW_EVENT_TYPE_SYSTEM			= 0xB003, /**< 系统命令事件*/
	BRW_EVENT_TYPE_PAINT			= 0xB004,  /**< 窗口切换，通知浏览器绘制的消息*/
	BRW_EVENT_TYPE_JSMSG			= 0xB005,	  /** JSMsg类型 */
	BRW_EVENT_TYPE_JS_APP_MSG            = 0xB006      /**来自系统内部的消息，如锁频成功、搜台完毕、网络信号中断等*/
}CSGTMsgType_E;

/**@brief 
Browser 功能键消息 
由于浏览器支持快捷键功能
如: 前进, 后退 ,刷新,停止下载,设置主页
清除历史记录,重新绘制页面等
为了应用层调用方便,应用层只需调用
接口,不需要理会事件类型

下面的枚举应用层不需要理会
*/

typedef  enum  _CSGTMsgSystemType_E
{
/**@brief app send msg_goback to Coship Browser  */
	BRW_SYSTEM_TYPE_GOBACK		=	0x00,          
/**@brief app send msg_goforward to Coship Browser  */
	BRW_SYSTEM_TYPE_GOFORWARD 	=	0x01,
/**@brief app send msg_stoploading to Coship Browser  */
	BRW_SYSTEM_TYPE_STOPLOADING 	=	0x02,
/**@brief app send msg_refresh to Coship Browser  */
	BRW_SYSTEM_TYPE_REFRESH 	=	0x03,
/**@brief app send msg_repaint to Coship Browser, 应用通知浏览器重新绘制窗口，不需要重新现在数据*/
	BRW_SYSTEM_TYPE_REPAINT 	=	0x04, 
/**@brief app send msg_clearHistory to Coship Browser, 清除历史记录 */
	BRW_SYSTEM_TYPE_CLEARHISTORY	=	0x05,
/**@brief app send msg_setHomePage to Coship Browser ,设置主页面功能 */
	BRW_SYSTEM_TYPE_SETHOMEPAGE	=	0x06, 
/**@brief app send msg_openurl to Coship Browser  */
	BRW_SYSTEM_TYPE_OPENURL		= 	0x07,
/**@brief app send show_msg_msgboxl to Coship Browser  */
	BRW_SYSTEM_TYPE_SHOW_MSGBOX	= 	0x08,
/**@brief app send hide_msg_msgboxl to Coship Browser  */
	BRW_SYSTEM_TYPE_HIDE_MSGBOX	= 	0x09,
	BRW_SYSTEM_TYPE_CLEARCACHE		= 	0x0a,
	BRW_SYSTEM_TYPE_EXIT_WINCLOSE	= 	0x0b,
	BRW_SYSTEM_TYPE_DVB             =   0x0c,
	BRW_SYSTEM_TYPE_IME             =   0x0d
} CSGTMsgSystemType_E;


typedef enum _CSGTMsgActionType_E
{
	GT_BUTTON_ENTER = 1,  	/**<浏览器处理OK 事件>*/
	GT_BUTTON_CANCEL,		/**<浏览器处理CANCEL 事件>*/
	GT_BUTTON_TIMEOUT	/**<浏览器处理超时 事件>*/
}CSGTMsgActionType_E;

typedef enum _CSGTMsgBoxType_E
{
	GT_BUTTON_NULL 		= 0,		/**<浏览器弹出框类型，无button>*/
	GT_BUTTON_OK 			= 1,		/**<浏览器弹出框类型，ok button>*/
	GT_BUTTON_OKCANCEL	= 2		/**<浏览器弹出框类型，Cancel button>*/
}CSGTMsgBoxType_E;


typedef enum _CSGTMsgBoxAlign_E
{
	GT_MSG_ALIGN_LEFT		= 0,    /**<浏览器弹出框对齐类型>*/
	GT_MSG_ALIGN_CENTER	= 1,
	GT_MSG_ALIGN_RIGHT	= 2
}CSGTMsgBoxAlign_E;

// 回调fnBrowserfun 类型为EM_BRW_SYSTEM_EXIT的 dwParm1参数取值
typedef enum _CSGTExitType_E
{
	EM_GT_EXIT_NORMAL_TYPE	= 0,    //浏览器已经退出，处于打开黑屏网页状态
	EM_GT_EXIT_PREVENT_TYPE	= 1,    //浏览器没有退出，也不打开黑屏网页，被网页阻止退出
	EM_GT_EXIT_NO_PROCESS_TYPE	= 2  // 浏览器没有退出，也没有被网页阻止退出, 设置EM_GT_OPT_BRW_QUIT_FLAG选项为0，浏览器按退出键返回该类型
}CSGTExitType_E;

typedef enum _GTUserRenderAndLayout_E 
{
	EM_GT_URAL_WIDTH_CONTAIN_BORDER = 1, // CSS  .tdx {width: 80px; border-top: 2px solid #FFFFFF} ;width contain the border ,江苏项目
	EM_GT_URAL_USE_CALC_TABLE_HEIGHT, // 当table指定高度大于计算高度时使用计算高度，eg: <table height="60"><tr><td height="50"></td></tr></table> then table height is 50 江苏项目
	EM_GT_URAL_ABSOLUTEPOSITION_NESTING, // 嵌套包含绝对定位时，兼容iPanel以开始点作为决定定位起始点，<div id="Layer6" style="position:absolute; left:207px; top:112px; width:430px; height:367px; z-index:6"><span style="position:absolute; left:5px; top:18px; width:200px; height:150px; z-index:7"></span>
												// </div> span将不已div作为起始点，齐齐哈尔
	EM_GT_URAL_GB2312TOGBK,	// meta中的编码方式强制由GB2312转为GBK ，哈尔滨
	EM_GT_URAL_CALC_TABLE_WIDTH, // 兼容华数table宽高算法, 台州华数使用
	EM_GT_URAL_IFRAME_PERCENTHEIGHT,       //  iframe的高度为百分比时，相对高度由其包含块调整为包含iframe节点的窗口高度 江苏项目	
	EM_GT_JSC_UNINIT_VALUE,   //是否支持未初始化的变量引用，设置该枚举就直接返回NULL
	EM_GT_JSC_CHECK_SCRIPT_STRING, //是否需要检查script string的语法，目前是检查大括号是否匹配
	EM_GT_URAL_IGNORE_LINEHEIGHT,   //是否支持CSS中的LineHeight属性	，江苏省网
	EM_GT_URAL_DISABLE_JSKEYDOWNEVENT,  //是否支持jskeydown事件,广州项目
	EM_GT_URAL_FOCUSON_SUB_HREF,		//焦点是否画在超级链接的子节点上，江苏省网/电信系列
	EM_GT_URAL_ENABLE_PRELOAD,          	// 是否支持网页预取功能
	EM_GT_CACHE_JS_SYNTAX_TREE,		// 是否支持缓存js 语法树
	EM_GT_MIDP_AUTO_CENTER_DISPLAY,		// 默认设置midp插件居中显示
	EM_GT_URAL_JS_CALC_TABLE_HEIGHT  // 是否支持江苏省网表格高度计算， 江苏省网, true 是江苏省网特殊处理，默认为false
}GTUserRenderAndLayout_E;

typedef enum _CSGTOption_E
{
	EM_GT_OPT_CABLEMODEM 			= 1 ,  // 设置CM状态，pvValue值1有cable modem，0无cable modem
	EM_GT_OPT_VOLUMECONTROL 		= 2,	 // 设置是否启动浏览器默认音量控制状态，pvValue is int.  0: control，1: no control
	EM_GT_OPT_SD_MUTE_POSITION_X 	= 3,  // 设置标清静音图标x位置,   pvValue is int. 
	EM_GT_OPT_SD_MUTE_POSITION_Y 	= 4,	 // 设置标清静音图标y位置，pvValue is int. 
	EM_GT_OPT_SD_VOLUME_POSITION_X = 5,	 // 设置标清音量图标x 位置，pvValue is int.
	EM_GT_OPT_SD_VOLUME_POSITION_Y = 6,	 // 设置标清音量图标y位置.   pvValue is int.
	EM_GT_OPT_HD_MUTE_POSITION_X 	= 7,	 // 设置高清静音图标x位置.pvValue is int.
	EM_GT_OPT_HD_MUTE_POSITION_Y 	= 8,	 // 设置高清静音图标y位置.  pvValue is int.
	EM_GT_OPT_HD_VOLUME_POSITION_X = 9,	 // 设置高清音量图标x位置.  pvValue is int. 
	EM_GT_OPT_HD_VOLUME_POSITION_Y = 10, // 设置高清音量图标y位置. pvValue is int.
	EM_GT_OPT_EXIT_TO_HOMEPAGE		= 11,  // 设置浏览器按退出按键退出homePage, pvValue 是 CSGTOptionExitToHomePage_S 结构地址
	EM_GT_OPT_OC_PROGRESS_TIMEOUT = 12,  // 设置DVB资源下载超时时间，如果达到设置时间并且已经下载了3/4，则不控制浏览器画图操作, pvValue is int.单位ms 
	EM_GT_OPT_RESOLUTION_CHANGE_SUPPORT = 13, //设置是否支持分辨率切换，pvValue is int,  0:no support ,1:support(default value)
	EM_GT_OPT_PROGRESS_TIME_DELAY	       	= 14,   // 设置浏览器打开页面的延迟时间
	EM_GT_OPT_ALWAYS_CACHE_JS_SYNTAX_TREE = 15,   //  设置浏览器是否永久缓存语法树，pvValue is int 0:needn't cache 1: cache js syntax tree
	EM_GT_OPT_ALWAYS_CACHE_IMAGE = 16,  //  设置浏览器是否永久缓存某些图片 pvValue is int 0:needn't cache 1: cache image
	EM_GT_OPT_ALLOWED_ALWAYS_CACHE_JS_SYNTAXTREE_NUM = 17,   //  设置浏览器永久缓存语法树个数，pvValue is int  always cached js syntax tree numble default is 3
	EM_GT_OPT_ALLOWED_ALWAYS_CACHE_IMAGE_NUM = 18,  //  设置浏览器永久缓存图片个数，pvValue is int ,  always cached image numble default is 2
	EM_GT_OPT_HTMLPARSE_FORCE_USE_HTML4COMPAT = 19,  //  设置浏览器HTML解析是否强制使用Html4 Compat类型，pvValue is int ,  1 使用，默认不使用由页面控制
	EM_GT_OPT_SET_MIX_MEMORY_MODE = 20,  //  设置浏览器使用混合内存模式，只能在浏览器初始化之前设置, pvValue 为 CSGTMixMemory_S混合内存信息地址
	EM_GT_OPT_HOMEPAGE			= 21,    //  设置浏览器快捷键进入主页, pvValue 是 CSGTOptionExitToHomePage_S 结构地址
	EM_GT_OPT_READDATA_TIMEOUT_MS = 22,   // 设置浏览器请求读取数据超时时间 单位ms,全局有效，pvValue is unsigned int
	EM_GT_OPT_MIN_VOLUME          = 23,    //设置浏览器最小音量值,目前仅支持最小值为0
	EM_GT_OPT_MAX_VOLUME          = 24,    //设置浏览器最大音量值
	EM_GT_OPT_FAIL_ON_ERROR       = 25,    //  设置浏览器是否将状态码>400的响应当作错误处理
	EM_GT_OPT_ENABLE_LOCAL_COOKIE = 26,	//设置浏览器支持本地cookie,pvValue值为，CSGTLocalCacheCfg_S*类型；
	EM_GT_OPT_UTCOFFSET           = 27,     //设置浏览器时区, 时区为double类型，pvValue 为传入时区变量的地址, 默认时区是 8.0
	EM_GT_OPT_DSTOFFSET           = 28, 	//设置夏令时开关  pvValue is int 0:no support DSTOFFSET ,1: support DSTOFFSET,default pvValue is 0
	EM_GT_OPT_SD_RESOLUTION_WIDTH = 29,	//设置浏览器标清分辨率的宽度，pvValue is int；默认720；若设置，请在CSGTWebInit之前设置，若CSGTWebInit初始化的是标清的分辨率，要求EM_GT_OPT_SD_RESOLUTION_WIDTH>=g_arglist.nScreenWidth(g_arglist是CSGTWebInit的参数，类型为CSBrwArgList_S)
	EM_GT_OPT_SD_RESOLUTION_HEIGHT = 30,	//设置浏览器标清分辨率的高度，pvValue is int；默认576；若设置，请在CSGTWebInit之前设置，若CSGTWebInit初始化的是标清的分辨率，要求EM_GT_OPT_SD_RESOLUTION_HEIGHT>=g_arglist.nScreenHeight(g_arglist是CSGTWebInit的参数，类型为CSBrwArgList_S)
	EM_GT_OPT_INNERTEXTTOINNERHTML = 31,    //控制是否使用ElementInnerText转换成ElementInnerHTML pvValue is int, 0:innerText不转换成innerHtml ,1: innerText转换成innerHtml,default pvValue is 0
	EM_GT_OPT_SUPPORT_JSEXCEPTION = 32,   //控制浏览器对于JS是否抛出异常，pvValue值TRUE抛出异常，FALSE不抛出异常
	EM_GT_OPT_MSGBOX_KEYCODE = 33,		 // 设置弹出提示框的键值 pvValue is int, keypress中如果发送的键值和设置键值一致，则调用提示框接口
	EM_GT_OPT_METACHARSET_FIRST_HTTPCHARSET = 34, // 设置浏览器charset优先级, pvValue is int , 设为1，网页meta Charset大于HTTP Charset,默认HTTP Charset大于网页meta Charset
	EM_GT_OPT_INPUTBOX_SIZE = 35,      // 设置输入框的大小,pvValue is int , 默认输入框大小为10，应用可以根据需要用方法设置输入框的大小
	EM_GT_OPT_USE_IECompat = 36,    // 设置是否使用IE模式，pvValue is bool ,默认为false，当应用设置为ture时，表示为IE模式，JS getYear获取年份计算会加上1900
	EM_GT_OPT_BRW_MUTE_LOGO = 37,       //设置是否显示浏览器默认静音图标，pvValue is int, 0为不显示，1为显示，默认为1
	EM_GT_OPT_ISNBSPEQUALTOSPACE = 38,   // 设置&nbsp是否用空格表示，空格的宽度比&nbsp的宽2px左右，pvValue is int, pvValue为1表示使用空格表示&nbsp, 默认为0, 0表示&nbsp不用空格表示
	EM_GT_OPT_BRW_QUIT_FLAG = 39,         // 设置网页不阻止也没有设置退出到homepage键的情况下，是否退出浏览器的标志，，pvValue is int, 取值：0，不退出浏览器；1，退出浏览器；默认：1 
	EM_GT_OPT_BRW_DEFAULT_IME = 40,         // 设置浏览器默认输入法，pvValue is int, 取值：0，中文；1，特殊字符；2. 英文字母3.软键盘 默认：0
	EM_GT_OPT_PROGRESS_INFO = 41,       // 设置注册浏览器进度条信息，pvValue是结构体CSGTProgressInfo_S地址
	EM_GT_OPT_PROGRESS_REPEAT_TIME = 42,     // 进度条转圈速度，pvValue为double类型的变量地址，默认值并且最小值为0.1(建议设置0.1 0.2 ... 1.0)，数值越大转圈越慢，
	EM_GT_OPT_PROGRESS_POS = 43, // 进度条显示位置，pvValue是结构体CSGTProgressPos_S地址 
	EM_GT_OPT_BRW_KEY_EVENT_TYPE = 44,        // 设置按键事件 event.type的取值， 默认为-1，pvValue is int, 取值 -1： event.type按照标准定义返回给网页； 取值 大于0：按照设置值返回给网页
	EM_GT_OPT_CACHE_MAXAGE = 45,           // 设置服务器没有设置浏览器cache失效时间时默认值， pvValue是double 指针， 单位为秒，设置如下10*60代表10分钟,默认值为20分钟
	EM_GT_OPT_CONNECT_FORBID_REUSE = 46,           // 设置HTTP 连接是否禁止复用，pvValue is int, 取值：0，允许复用, 1 禁止复用，默认为允许复用
	EM_GT_OPT_ISUSEPIPELINE = 47, 	//设置是否使用pipeline的方式下载资源，0为不使用，1为使用，默认不使用
	EM_GT_OPT_CSS_PRIORITY = 48,     // 设置用户自定义css属性优先级，默认false表示按常规处理，ture表示check检查用户自定义优先级
	EM_GT_OPT_TABLE_BACKGROUND_REPEAT = 49, // 设置table标签的背景图片平铺方式，默认为false表示平铺，ture表示不平铺，并且设置图片位置为居中
	EM_GT_OPT_HUNAN = 50,     // 设置湖南项目一些特殊页面处理的配制项，默认是false表示关闭。主要是td中有img会撑大table;在严格模式解析下table会加上padding与border;扩大了刷新区域;
	EM_GT_OPT_USE_STATIC_MEM = 51,     // 设置浏览器使用外部配置的静态内存 , pvValue is int， 默认为0 不使用外部内存，1为使用外部内存
	EM_GT_OPT_STATIC_MEM_ADDR  = 52,     // 设置浏览器使用外部配置的静态内存的地址，pvValue is void *, 地址内存首地址，默认为空
	EM_GT_OPT_STATIC_MEM_SIZE = 53,   // 设置浏览器使用外部配置的静态内存的大小，pvValue is int, 外部内存地址空间长度，默认为0
	EM_GT_OPT_SHOW_TRANSPRENT_BMP = 54,   // 设置透明的BMP图片是否显示，pvValue is int, 0为不显示，1为显示，默认为0
	EM_GT_OPT_RETURN0_STOP_DEFAULT = 55,		// 配置return 0时是否阻止浏览器默认处理,pvValue is int, 1表示默认阻止退出键，0表示不阻止，默认为1。
	EM_GT_OPT_SHOW_DEFAULT_IMAGE = 56,	// 设置当图片没下载成功时，是否显示显示默认图片，pvValue is int, 1表示显示，0表示不显示，默认为1。
	EM_GT_OPT_URL_DONOT_ALWAYS_UTF8_ENCODER = 57,  // 设置URL不总是使用UTF-8发送，按照页面指定编码方式进行URL编码 pvValue is int, 取值：0，总是使用UTF-8编码, 1 不总是使用UTF-8编码，默认为0
	EM_GT_OPT_PAINT_AFTER_ALLCOMPLETED = 58, // 设置是否所有的frame下载完后才显示页面，pvValue int ,0为部分frame下载完显示，1为所有frame下载完后才显示，默认为0(目前仅为吉林使用)
	EM_GT_OPT_IMAGE_SCALE_SIZE = 59,       // 设置图片解码缩放的宽高，pvValue是结构体CSGTImgScalSize_S地址, 默认是图片原始大小
	EM_GT_OPT_CUSTOMIZE_INPUTKEY = 60,      //  设置用户自定义输入法启动按键，pvValue is int,表示定义的按键键值，默认为-1
	EM_GT_OPT_DISABLE_SETDRAWFOCUSRING = 61    //设置页面setDrawFocusRing(0)这种写法无效，pvValue is int, 1表示无效，0表示有效，默认为0
}CSGTOption_E;

/**
代理服务器类型
*/
typedef enum _CSGTProxyType{
	CSGTPROXY_HTTP = 0,   /* added in 7.10 */
	CSGTPROXY_SOCKS4 = 4, /* support added in 7.15.2, enum existed already
							in 7.10 */ 
	CSGTPROXY_SOCKS5 = 5, /* added in 7.10 */
	CSGTPROXY_SOCKS4A = 6, /* added in 7.18.0 */
	CSGTPROXY_SOCKS5_HOSTNAME = 7 /* Use the SOCKS5 protocol but pass along the
									host name rather than the IP address. added
									in 7.18.0 */
} CSGTProxyType_E; /* this enum was added in 7.10 */

/*
@使用此接口，设置浏览器功能按键退出是否有退至主页的功能
@设置 	:  浏览器的退出按键采用退至homepage ,在主页退出浏览器
@不设置	:  浏览器的退出
*/
typedef struct _CSGTOptionExitToHomePage_S
{
	int		 nKeyCode;				// 设置触发可选项功能的按键键值
	char	 	*pcUrl;					 // 设置触发可选项功能的url
	int		 nLen;					// 设置浏览器退至主页url的长度
}CSGTOptionExitToHomePage_S; 

/**
@brief 设置浏览器弹出对话框的回调
@param[in] int nId 弹出框的类型ID, e_ActionType eType 弹出框的动作, void *pUserData 可扩展参数
*/
typedef void (*MsgBoxActionCallBack_F)(CSGTMsgActionType_E enType, void *pvData, void *pvUserData); 

/**
@brief 获取来自系统内部的消息（如锁频成功、搜台完毕、网络信号中断等）的回调函数
此系统消息需要3个参数 type which modifiers
1、如果modifiers不为空，则如下发送消息
CSBRWEvent eEvent;
eEvent.m_dwPara0 = nId; //此id唯一标志一个消息，根据此id从回调里面获取消息参数which modifiers的值
eEvent.m_dwPara1 = BRW_EVENT_TYPE_JS_APP_MSG; //此消息类型
eEvent.m_dwPara2 = 2; //消息参数 type的值
eEvent.m_dwPara3 = (DWORD)(JsMsgInfoCallBack); //回调函数指针
eEvent.m_dwPara0 = 0;

2、如果modifiers为空，则如下发送消息
CSBRWEvent eEvent;
eEvent.m_dwPara0 = nWhich; //消息参数which的值
eEvent.m_dwPara1 = BRW_EVENT_TYPE_JS_APP_MSG; //此消息类型
eEvent.m_dwPara2 = 2; //消息参数 type的值
eEvent.m_dwPara3 = 0; //不需要回调
eEvent.m_dwPara0 = 0;

@param[in]       nId           消息标志
@param[out]      pnWhich       消息参数which的值
@param[out]      pcModifiers   消息参数modifiers的值，编码类型必须为unicode
@param[int，out] pnModifiersLen   in: pcModifiers所指向内存的字节长度 out：消息参数modifiers的unicode字符个数
@param           pvUserData    用户数据
*/
typedef void (*JsMsgInfoCallBack_F)(int nId, int *pnWhich, char *pcModifiers, int *pnModifiersLen, void *pvUserData); 

typedef struct _CSGTsMsgBoxConfig_S
{
	DWORD			dwMsgBoxId;    //用户提示框id
	DWORD			dwTimeOut; //ms 0 不超时
	char 			*pcBoxAption; //提示框标题内容
	char				*pcBoxContent;  //提示框内容
	int				nBoxAptionLen;	//提示框标题的长度
	int 				nBoxContentLen; // 提示框内容的长度
	void 			*pUserdata;  //用户数据，在m_fnMsgBoxCB中传回给用户
	CSBRWRect_S	sRect;		// Msg 显示位置
	CSGTMsgBoxType_E 	emMsgType;   //type 0 - 2, button numble
	CSGTMsgBoxAlign_E    emAlign;		// 对齐方式，left = 0; right = 1;center = 2;
	MsgBoxActionCallBack_F fnMsgBoxCallback;
	
}CSGTsMsgBoxConfig_S;


typedef struct _CSGTsMsgBoxInfo_S
{
	DWORD			dwMsgBoxId;    //用户提示框id
	DWORD			dwTimeOut; //ms 0 不超时
	char 			pcBoxAption[64]; //提示框标题内容
	char			pcBoxContent[256];  //提示框内容
	int				nBoxAptionLen;	//提示框标题的长度
	int 			nBoxContentLen; // 提示框内容的长度
	void 			*pUserdata;  //用户数据，在m_fnMsgBoxCB中传回给用户
	CSBRWRect_S		sRect;		// Msg 显示位置
	CSGTMsgBoxType_E 	emMsgType;   //type 0 - 2, button numble
	CSGTMsgBoxAlign_E    emAlign;		// 对齐方式，left = 0; right = 1;center = 2;
	MsgBoxActionCallBack_F fnMsgBoxCallback;

}CSGTsMsgBoxInfo_S;

/**@brief 事件的动作,如keydown,keyup,keypress,
一个按键需要产生三个动作,
因为浏览器需要按键的动作  
*/
typedef  enum  _CSBRWKeyStatus_E
{
	BRW_EVENT_TYPE_KEY_DOWN	= 8,  /**< 键盘按下事件*/
	BRW_EVENT_TYPE_KEY_UP		= 9, /**< 键盘松开事件*/
	BRW_EVENT_TYPE_KEY_PRESS	= 10 /**< press 事件,浏览器自己产生的,浏览器需要这类事件动作*/
} CSBRWKeyStatus_E;

/**@brief 浏览器发给应用的消息类型为EM_BRW_KEYBOARDEVENT_PROCESS时，
               浏览器抛给外面监听按键信息的结构体
m_processType、m_keyStatus参见eBrwProcessType、CSBRWKeyStatus_E的定义
当m_processType为EM_BRW_INPUTBOX时，m_x、m_y才有值
*/
typedef struct _CSBRWEventProcess_S
{
	eBrwProcessType m_processType;    //浏览器处理按键消息的类型	
	CSBRWKeyStatus_E m_keyStatus;     //浏览器事件的动作	
	int m_keycode;                    //处理按键值
	int m_x;				          //输入框的x坐标	
	int m_y;				          //输入卡的y坐标
}CSBRWEventProcess_S;

/*@brief 事件类型定义：
	1.BRW_EVENT_TYPE_KEYBOARD  	= 0xB001 	// 键盘事件
	m_dwPara0:  keycode
	m_dwPara1:	CSGTMsgType_E类型
	m_dwPara2:	按键动作
	m_dwPara3：	未使用
	m_dwPara4： 未使用
	
	2.BRW_EVENT_TYPE_MOUSE     	= 0xB002	// 鼠标事件
	m_dwPara0:  未使用
	m_dwPara1:	CSGTMsgType_E类型
	m_dwPara2:	鼠标点击动作
	m_dwPara3：	x,y位置坐标
	m_dwPara4： 未使用
	
	3.BRW_EVENT_TYPE_SYSTEM			= 0xB003  // 系统命令事件
	m_dwPara0:  功能消息 （参看CSGTMsgSystemType_E）
	m_dwPara1:	CSGTMsgType_E类型
	m_dwPara2:	扩展参数
	m_dwPara3：	未使用
	m_dwPara4： 未使用
	
	4.BRW_EVENT_TYPE_PAINT			= 0xB004  // 窗口切换，通知浏览器绘制的消息
	m_dwPara0:  功能消息 （参看CSGTMsgSystemType_E）
	m_dwPara1:	CSGTMsgType_E类型
	m_dwPara2:	未使用
	m_dwPara3：	x,y位置坐标
	m_dwPara4： w,h（高16位，低16位）
	
	5.BRW_EVENT_TYPE_JSMSG		= 0xB005	// JSMSG 
	m_dwPara0:  keycode 
	m_dwPara1:	CSGTMsgType_E类型
	m_dwPara2:	按键动作
	m_dwPara3：	扩展参数
	m_dwPara4： 未使用
*/

typedef struct  _CSBRWEvent
{
	DWORD m_dwPara0;	
  DWORD m_dwPara1;
	DWORD m_dwPara2;
	DWORD m_dwPara3;
	DWORD m_dwPara4; 
}CSBRWEvent;

typedef struct _CSBrwArgList
{
	BOOL		isBrwCreateWnd;   /**< 是否是浏览器创建窗口TRUE:  浏览器创建， FALSE: 应用只需要传送前三个变量*/
	int    		nBufferSize;    		/**<外部传入的内存池的大小*/
	int			nScreenWidth;		/**<显示区域分辨率*/
	int			nScreenHeight;   	/**<显示区域分辨率*/
	int			nCacheBufferSize;	/**<浏览器缓存大小>*/
	/** @brief
	需要浏览器创建窗口时，需要传下面的参数*/
	int 			nX;			      	/**< 初始化窗口的X位置(相对显示设备)*/
	int 			nY;			      	 /**< 初始化窗口的Y位置(相对显示设备) */
	int 			nWidth;           	 /**< 初始化窗口的宽度 */
	int 			nHeight;	      		 /**< 初始化窗口的高度 */

} CSBrwArgList_S;

//zmr add -s
typedef struct _CSGTFontMap  {	
	int nStartSize;    //字体映射区间开始位置  	
	int nEndSize;	   //字体映射区间结束位置
	int nLibFont;	   //将映射到的字体
}CSGTFontMap_S;
//zmr add -e

typedef struct _CSGTProgPicInfo_S
{
	int nPerFrameW; //进度条图片每一帧宽度，若每一帧的宽度都一样时可直接设置一个全局即可，目前暂不支持
	int nPerFrameH; //进度条图片每一帧高度，若每一帧的高度都一样时可直接设置一个全局即可，目前暂不支持
	int pnBufferSize; //进度条每帧bufferSzie
	unsigned char*  pcBuffer; //进度条每帧的buffer地址
}CSGTProgPicInfo_S;

typedef struct _CSGTProgRect_S
{
	int nWidth; //进度条图片全局宽,若不设置每一帧宽度，则浏览器取该值
	int nHeight; //进度条图片全局高,若不设置每一帧高度，则浏览器取该值
}CSGTProgRect_S;

typedef struct _CSGTProgressPos_S
{
	int nSDX;  //进度条标清X位置
	int nSDY;  //进度条标清Y位置
	int nHDX;  //进度条高清X位置
	int nHDY;  //进度条高清Y位置 
}CSGTProgressPos_S;

/**
@brief 应用注册的获取进度条每帧信息的回调函数
@param[in] CSGTProgPicInfo_S 进度条每帧信息
*/
typedef void (*fnGetProgIcon_F)(int nFrameIndex, CSGTProgPicInfo_S * pSProgressIcon, void * pvUserData);

typedef struct _CSGTProgressInfo_S{
	int nFrameCount;                // 进度条帧数 ，默认值为8，最大值为12
	CSGTProgRect_S  SProgressPicRect;  // 进度条显示的位置，大小 
	fnGetProgIcon_F  fnProgfun;    // 应用注册的获取进度条每帧信息的回调函数
}CSGTProgressInfo_S;

/*url链表*/
typedef struct _CSGTUrlList
{
	char * url;	
	struct _CSGTUrlList * next;
}CSGTUrlList_S;

/*图片缩放尺寸*/
typedef struct _CSGTImgScalSize_S
{
	int nWidth;	
	int nHeight;
}CSGTImgScalSize_S;

/**
@brief 浏览器给应用的回调处理函数定义。
@param[in] dwType:   消息主类型，目前分三类，OC , Network ,System
@param[in] dwParm1:  主类型下的子类型
@param[in] dwParm2： 用户自定义参数，可以使用结构体指针传递参数。
@param[in] userdata: 
返回值: 根据dwType和dwParm1进行返回，如消息主类型为Network时，返回需要打开的错误页面，否则直接返回空。
如果返回 dwType  是EM_BRW_SYSTEM_EXIT 时,  dwParm1 为0，表示退出浏览器，其他值为退出按键交由网页控制，应用不需要处理
*/
typedef  char *(*fnBrowserfun)(eBrwMsgType dwType, int dwParm1, void* dwParm2 , void * pvUserData);

/**
@brief 协议回调定义。
1、应用可以通过EM_GT_OPT_MSGBOX_KEYCODE自己设置按键退出，此情况下发送给浏览器的消息CSBRWEvent中的m_dwPara3为空，m_dwPara4保存用户Id
如果m_dwPara4参数为0，则EM_GT_OPT_MSGBOX_KEYCODE设置的键值为用户id传递给dwParm1。
2、另外应用也可以直接发消息调用BRW_SYSTEM_TYPE_SHOW_MSGBOX方法实现弹出框。设置CSBRWEvent中的m_dwPara3为空，m_dwPara4为用户Id，浏览器通过
CSOnGTGetMsgBoxInfo_F函数回调获取提示框信息，其中dwParm1参数为CSBRWEvent重的m_dwPara4用户id。

@param[in] dwParm1 : 传用户提示框ID。
@param[in] dwParm2 : 获取CSGTsMsgBoxInfo_S结构体的内容。
@param[in] pvUserData :用户数据
@return void。
@note 无。
*/
typedef void(*CSOnGTGetMsgBoxInfo_F)(int dwParm1, void* dwParm2 , void * pvUserData);

/**
@brief 页面响应完按键后再将键值传给APP 处理
@param[in] nKeyCode:  传给APP 处理的键值
@param[in] dwParm2:   未定义
@param[in] userdata:  用户数据，注册时设定
返回值: void
*/
typedef void(*fnAppEventProcessFun)(unsigned int nKeyCode, void* dwParm2 , void * pvUserData);

/**
@brief 协议回调定义。

@param[in] url 协议头 eg : " rtsp://" 实时流协议， 依据用户。
@param[in] nUrlLen unicode 编码的url所占的内存字节长度。
@return 无返回值。
@note 无。
*/
typedef void* (* UserProtocolCallBack)(char * pcUrl, int nUrlLen);


/**
@brief 退出浏览器回调
@param[in] enType 回调动作: 是否退出浏览器
*/

typedef void* (* UserDialogCallBack)(CSGTRet_E enType);

/**
@brief 定义回调函数的指针。

@param[in] char 提示信息。
@return 无返回值。
@note 提供给GUITOOL 自动化测试使用。
*/
typedef void (*fnNextURLCALLBACK)(const char * pcInfo); // 回调函数指针


/**
@brief 初始化GUITOOL。

@param[in] psBrwArgList  初始化函数指针。
@return 成功时返回GT_RET_SUCCESS,失败返回GT_RET_FAILURE。
@note int    		       nBufferSize;    	外部传入的内存池的大小。
@note int				nScreenWidth;			显示区域分辨率。
@note int				nScreenHeight;   	显示区域分辨率，需要浏览器创建窗口时，需要传下面的参数。
@note int 			nX;			      		初始化窗口的X位置(相对显示设备)。
@note int 			nY;			      	  初始化窗口的Y位置(相对显示设备) 
@note int 			nWidth;           初始化窗口的宽度。
@note int 			nHeight;	      	初始化窗口的高度 。
@note BOOL			isBrwCreateWnd;   	是否是浏览器创建窗口，TRUE:  浏览器创建， FALSE: 应用只需要传送前三个变量。
*/
WEBKIT_API CSGTRet_E  CSGTWebInit(CSBrwArgList_S *psBrwArgList);

/**
@brief  获取浏览器当前url
@note char*			pcCurUrl: 输出参数，存放浏览器当前访问的url；
@note int			nLen:  输入参数，pcCurUrl的长度；
@return     SUCCESS :    GT_RET_SUCCESS;    FAILURE :    GT_RET_FALIURE; 
@note 无。

*/
WEBKIT_API CSGTRet_E  CSGTGetCurrentUrl(char *pcCurUrl,int nLen);
/**
@brief  退出关闭GUITOOL ,销毁窗口,释放资源，该函数必须是在浏览器  初始化之后调用才有效。如果有其他程序共享了浏览器到内存，
                                但没有释放，则此函数调用返回失败。

@return     SUCCESS :    GT_RET_SUCCESS;    FAILURE :    GT_RET_MEMORY_LOCK; //申请的共享内存没有释放。
@note 无。

*/
WEBKIT_API CSGTRet_E  CSGTWebDestroy() ;


/**
@brief 应用向浏览器发送事件消息。

@param[in] sEvent 按键消息。
@return 成功时返回GT_RET_SUCCESS,失败返回GT_RET_FAILURE。
@note 参考例子:
@note 按键消息:
@note CSBRWEvent sEvent;
@note sEvent.m_dwPara0 = nKeyCode; // 键值
@note sEvent.m_dwPara1 = nEventType; // 按键类型
@note sEvent.m_dwPara2 = nEventAction; // 按键动作
@note sEvent.m_dwPara3 = 0;
@note sEvent.m_dwPara4 = 0;
@note CSGTMsgProc(sEvent);

@note 功能消息:
@note CSBRWEvent sEvent;
@note sEvent.m_dwPara0 = BRW_SYSTEM_TYPE_GOBACK;
@note sEvent.m_dwPara1 = BRW_EVENT_TYPE_SYSTEM;
@note sEvent.m_dwPara2 = 0;
@note sEvent.m_dwPara3 = 0;
@note sEvent.m_dwPara4 = 0;
@note CSGTMsgProc(sEvent);
*/
WEBKIT_API CSGTRet_E  CSGTMsgProc(CSBRWEvent sEvent);

/**
@brief 动态改变浏览器的窗口,只有在应用创建浏览器窗口的时候需要使用(在hyperText 中使用),其他的时候不需要.
@param[in] GR_RECT rect 获取当前浏览器的窗口分辨率。
@return 成功时返回GT_RET_SUCCESS,失败返回GT_RET_FAILURE。
@note! the type of rect is GR_RECT ,we use void * to decouple
*/
WEBKIT_API CSGTRet_E CSGTCurrentWinSize(CSBRWRect_S *psRect);    

/**
@brief 动态改变浏览器的窗口,只有在应用初始化是需要设置标清网页需要的窗口大小
@param[in] int nSDWidth, int nSDHeight 应用设置的窗口大小。
@return 成功时返回GT_RET_SUCCESS,失败返回GT_RET_FAILURE。
@note! the type of rect is GR_RECT ,we use void * to decouple
*/
WEBKIT_API CSGTRet_E CSGTSetSDDefaultWinSize(int nSDWidth, int nSDHeight);


/**
@param[in] hClient 浏览器的分辨率句柄。
@param[in] hWindow 浏览器的窗口。
@param[in]  int nScreenWidth 初始化分辨率的宽。
@param[in]  int nScreenHeight 初始化分辨率的高
@return 成功时返回GT_RET_SUCCESS,失败返回GT_RET_FAILURE。
*/
WEBKIT_API CSGTRet_E  CSGTChangeWin(HCSHANDLE hClient, HCSHANDLE hWindow, int nScreenWidth, int nScreenHeight); 

/**
@brief 提供自动化测试使用的回调打开下一条网址函数。

@param[in] msg 提示信息。
@param[in] callback 回调函数指针。
@return 无返回值。
@note 参考:
@note void* testCSGTRegisterCallback()
@note {
@note      CSGTRegisterCallback( "GuiTool Test!", openNextUrl );
@note  }
*/
WEBKIT_API void CSGTRegisterCallback( const char* pcMsg, fnNextURLCALLBACK fnCallback );

/**
@brief 把浏览器置于最省内存状态，释放网页的资源，
                                保留浏览器初始化时必须的一些资源。该函数必须是在
                                浏览器已经初始化之后调用才有效。

@return     SUCCESS :    GT_RET_SUCCESS;    FAILURE :    GT_RET_STATE_ERR。
@note 无。
*/
WEBKIT_API CSGTRet_E CSGTReleaseWebContent();

/**
@brief 提供设置浏览器homePage 的功能
								该函数必须是在
                                浏览器已经初始化之后调用才有效。
                                
@param[in] char* pUrl:     设置为主页的url,字符串长度小于 1024byte
@param[in] int nKeyCode: 设置快捷按键(主页的按键)键值,请不要与其他的任何按键键值相同

@return     SUCCESS :    GT_RET_SUCCESS;    FAILURE :    GT_RET_STATE_ERR。
@note 无。
*/
WEBKIT_API CSGTRet_E CSGTSetHomePage(char *pcUrl, int nKeyCode);


/**
@brief 获取浏览器相关信息。

@param[in] pBrwInfo 获取到的浏览器信息。
@return     SUCCESS :     GT_RET_SUCCESS;    FAILURE :     GT_RET_FAILURE。
@note 无。
*/
CSGTRet_E CSGTGetBrwInfo(CSGTBrwInfo_S * psBrwInfo);


/**
@brief 申请guitool中的内存。
@param[in] nSize 申请的内存大小，单位为字节。
@return SUCCESS：返回一指针。    FAILURE: NULL  失败原因有二：(1)浏览器状态不为GT_STATE_BACK或GT_STATE_STANDBY
			          (2) 浏览器内存不足，不能分配连续的nSize内存大小。
@note 该函数只能在浏览器状态为GT_STATE_BACK或GT_STATE_STANDBY时调用才有效。
@note 建议在浏览器置于前台之前释放此申请的内存，
@note 否则浏览器很可能由于内存不足而导致不能正常显示。
*/
void *CSGTMalloc(int nSize);



/**
@brief 释放从guitool中分配的内存。

@param[in] ptr 指向由CSGTMalloc所返回的内存指针。
@return  SUCCESS :     GT_RET_SUCCESS;    FAILURE :     GT_RET_STATE_ERR  浏览器状态不为GT_STATE_BACK或GT_STATE_STANDBY。
@note 该函数只能在浏览器状态为GT_STATE_BACK或GT_STATE_STANDBY  时调用才有效。
*/
CSGTRet_E CSGTFree(void * pvPtr);


/**
@brief 注册 用户协议回调。

@param[in] pUserProtocolHead  注册的协议头 。
@param[in] userCallBack 注册的回调函数 。
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE。
@note 无。
*/
WEBKIT_API CSGTRet_E CSGTSetUserProtocol(const char *pUserProtocolHead, 
                                                        UserProtocolCallBack fnUserCallBack);



/**
@brief 用户是否将GTOOL至于当前窗口。

@param[in] bUsedGTool  TRUE,   用户是将GTOOL至于当前窗口FALSE， 用户是否将GTOOL隐藏。
@return     Hidden :     GT_RET_SUCCESS;    Show   :     GT_RET_FAILURE;
@note 无。
*/
WEBKIT_API CSGTRet_E  CSGTWindowVisible(BOOL  bUsedGTool);


/**
@brief 打开一个网页 。

@param[in] url  可以访问的网页 url ( ascall 码)。
@param[in] nURLLen url 的长度。
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE。
@note 无。
*/
WEBKIT_API CSGTRet_E  CSGTOpenURL(char *pcUrl,int nUrlLen);


/**
@brief 预加载打开一个网页，浏览器不进行Paint  操作 。

@param[in] url  可以访问的网页 url ( ascall 码)。
@param[in] nURLLen url 的长度。
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE。
@note 无。
*/
WEBKIT_API CSGTRet_E   CSGTPreLoadURL(char *pcUrl, int nLen);

/**
@brief 打开一个html 文本文件 。

@param[in] pData html 文本文件(  ascall 码)。
@param[in] nDataLengh 文件的路径 名长度。
@return 无返回值。
@note 无。
*/
WEBKIT_API CSGTRet_E CSGTOpenHtmlContent(char *pcData,int nDataLengh);

/**
@brief 提供浏览器退出注册。

@param[in] statusmessages 		提示状态信息。
@param[in] contentmessages 	提示状态内容信息。
@param[in] DialagCallback		 注册的回调函数 。
@return 无返回值。
*/
WEBKIT_API CSGTRet_E CSGTShowAlert(char* pcStatusMessages , char* pcContentMessages, UserDialogCallBack fnDialagCallback);

/**
@brief 获取当前GTOOL 的版本号

@param[out] pcVersion GUITOOL 的版本号 。

@param[in] nSize 申请版本号存储长度        
@return SUCCESS : 返回版本号长度;    FAILURE : 返回默认长度。
@note 无。
*/
WEBKIT_API int CSGTGetVersion( char * pcVersion, int nSize );

/**
@brief 下载数据。

@param[in] pucData 数据Buffer的指针。
@param[in] nLen 传回数据Buffer的长度。
@param[in] szName 返回的数据的名字，包含后缀。
@return 无返回值。
@note 此函数返回的Buffer需用户自己拷贝出来保存。浏览器不会长久保持该数据。
*/
typedef void(*CSOnGTDownloadData_F)(char * pucData, int nLen, const char* szName);


/**
@brief 下载数据。

@param[in] szUrl 需下载数据的URL的字符串。
@param[in] callback 用户自定义回调函数。
@return 如果成功返回GT_RET_SUCCESS，如果失败返回GT_RET_FAILURE。
@note 无。
*/
WEBKIT_API CSGTRet_E CSGTDownLoad(const char* szUrl, CSOnGTDownloadData_F fnCallBack);


/**
@brief  通过浏览器启动Java应用

@param[in] JadURL JavaApp的Jad的URL 。
@param[in] JarURL JavaApp的Jar包的URL 。
@param[in] pParams JaraApp的启动参数， 例如（ServerIP，UserID 等，），描述为这样的一个字符串：
	                 "ServerIP=17.30.19.102 UserID=901351 ..." , 一对参数间利用空格分开，“=”前后不能有空格。
@param[in] x JavaApp窗口的位置横坐标。
@param[in] y JavaApp窗口的位置纵坐标。
@param[in] width JavaApp窗口的宽度。
@param[in] height JavaApp窗口的的高度。
@return 成功返回0，失败返回-1 。
@note 无。
*/
WEBKIT_API int CSGTStartJavaApp(const char *pcJadURL, const char *pcJarURL, const char *pcParams, int nX, int nY, int nWidth, int nHeight);


/**
@brief  得到GT启动的Java应用的状态，包括js扩展的和Plugin方式的应用

@param[in] 无
@return TRUE 应用在运行,FALSE 应用未运行
@note 无
*/
WEBKIT_API int CSGTGetJavaAppStatus();


/**
@brief  退出通过浏览器启动的应用(包括邋JS扩展和Plugin方式的应用)

@param[in] 无
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSGTExitJavaApp();

/**
@brief  注册浏览器给应用的回调处理函数

@param[in] fun      浏览器给应用的回调处理函数
@param[in] userdata 用户数据
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSRegisterBrwCallback(fnBrowserfun fnFun, void *pvUserData);

/**
@brief  注册应用中弹框的回调处理函数

@param[in] fnFun : 浏览器给应用的回调处理函数
@param[in] pvUserData:  用户数据
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSGTRegisterMsgBoxCallback(CSOnGTGetMsgBoxInfo_F fnFun, void *pvUserData);

/**
@brief  设置兼容性项目开关

@param[in] GTUserRenderAndLayout: 根据项目功能类型
@param[in] bSwitch: true 开，false关，默认关
@return
@note ?
*/
WEBKIT_API  BOOL CSGTSetUserRenderAndLayout(GTUserRenderAndLayout_E enmType, BOOL bSwitch);

/**
@brief  

@param[in] 
@param[in] 
if enmOption is EM_GT_OPT_CABLEMODEM, the pvValue is a int
@return 
@note ?
*/
WEBKIT_API void CSGTSetOpt(HCSHANDLE hHandle, CSGTOption_E enmOption, void *pvValue);

/**
@brief  用户键值与网页键值转换表，用于网页使用的键值与UDI定义的键值不一致的情况

@param[in] pnKeyTableAddr 键值对应表
@param[in] nFirstDimension 第一维数组长度
@return 如果成功返回GT_RET_SUCCESS，如果失败返回GT_RET_FAILURE
@note ?
例子:
static int nMapKey[][2] = {
       {CSUDI_VK_UP, PAGE_UP},
       {CSUDI_VK_DOWN, PAGE_DOWN}, 
       {CSUDI_VK_LEFT, PAGE_LEFT}, 
       {CSUDI_VK_RIGHT, PAGE_RIGHT}
}; 

CSGTMapKeyTable(nMapKey, sizeof(nMapKey)/sizeof(nMapKey[0][0])/2);
*/
WEBKIT_API int CSGTMapKeyTable(int(* pnKeyTableAddr)[2], int nFirstDimension);

/**
@brief  提供给用户设置字库映射表

@param[in]   Font[]  存放字库信息表的结构体数组
@param[in]   num 结构体数组包含的结构体的个数
@return SUCCESS :  0
@note ?
*/
WEBKIT_API void CSGTSetFontLib(CSGTFontMap_S saFont[], int nNum);

/**
@brief  注册页面处理按键后将键值传给APP 处理的函数

@param[in] fun  APP 处理该键值的接口    
@param[in] userdata 用户数据
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSRegisterAppEventProcessCallback(fnAppEventProcessFun fnAppEvtPrcsFun, void *pvUserData);

/**
@brief  设置2D Effect缓存大小
@param[in] nCacheMemorySize 设置2D 缓存的大小
@return 
@note ?
*/
WEBKIT_API int CSJSEffectSetCacheSize(int nCacheMemorySize);

/**
@brief ioctlRead /ioctlWrite接口声明

IoctlRead
    keyValue:			    
        "GTGetMaxVol"    获取最大音量，可省略，默认值31
        "GTGetMinVol"     获取最小音量，可省略，默认值0
        "GTGetVolTimeOut"    获取音量条超时时间，可省略，有默认值3000ms
        "GTGetVolColor"         获取音量条上文本颜色，可省略，有默认值0xFFFFFFFF
        "GTGetMuteState"     获取静音状态
        "GTGetVolume"          获取音量值

IoctlWrite
    keyValue: 
        "GTSetMute"        设置静音状态
        "GTSetVolume"    设置音量值
return value:
    return 0表示针对传入的key操作成功，
    return -1表示传入的key不是自己关心的，没有操作
*/
typedef int (*CSGTPortingIoctlRead_F)(const char * pcKey, int nKeyLen, char * pcReadBuf, int nBufLen);
typedef int (*CSGTPortingIoctlWrite_F)(const char * pcKey, int nKeyLen, char * pcValue, int nValueLen);

/**
@brief 注册ioctl函数，目前由shell注册
目前最多支持注册5对,注意不支持注册为空，
fRead fWrite必须都不为空，如有需要用户可以注册空函数。
return value:
    return 0 表示注册成功
    return -1表示注册失败
*/
int CSGTRegisterIoctlFun(CSGTPortingIoctlRead_F fnRead, CSGTPortingIoctlWrite_F fnWrite);

/**
@brief io Read, 目前供jsext使用
return value:
    return 0表示针对传入的key操作成功，
    return -1表示传入的key没有处理
*/
int CSGTIoctlRead(const char * pcKey, int nKeyLen, char *pcReadBuf, int nBufLen);

/**
@brief io write, 目前供jsext使用
return value:
    return 0表示针对传入的key操作成功，
    return -1表示传入的key没有处理
*/
int CSGTIoctlWrite(const char * pcKey, int nKeyLen, char * pcValue, int nValueLen);

/**
@brief  设置是否允许浏览器显示页面

@param[in]   bAllowPaint  TRUE为允许浏览器显示页面，FALSE为不允许浏览器显示页面
@return 
@note 默认允许浏览器显示页面
*/
WEBKIT_API void CSGTAllowPaint(BOOL bAllowPaint);

/**
@brief set proxy, 设置代理服务器
proxy: 代理服务器地址,最长64字节
proxyPort: 代理服务器端口
userName: 访问代理服务器的用户名，最长32字节。若无用户名，请使用NULL
password: 访问代理服务器的密码，最长32字节。若无密码，请使用NULL
proxyType：代理服务器类型,默认CSGTPROXY_HTTP
typedef enum _CSGTProxyType{
	CSGTPROXY_HTTP = 0, 
	CSGTPROXY_SOCKS4 = 4, 
	CSGTPROXY_SOCKS5 = 5,
	CSGTPROXY_SOCKS4A = 6, 
	CSGTPROXY_SOCKS5_HOSTNAME = 7 
} CSGTProxyType_E;
return value:
    return TRUE 表示成功，
    return FALSE 表示没有处理
*/
WEBKIT_API BOOL CSGTSetProxy(const char * pcProxy, long lProxyPort, CSGTProxyType_E enmProxyType, const char * pcUserName, const char * pcPassword);

/**
@brief set proxy open or close, 代理服务器开关，若CSGTSetProxy后默认是true；若没有CSGTSetProxy，则默认是false。
bUseProxy: 
	TRUE: 使用已经设置的代理服务器访问资源
	FALSE: 不使用代理服务器访问资源
return value:
	return TRUE 表示设置成功
	return FALSE 表示没有处理
*/
WEBKIT_API BOOL CSGTSetUseProxy(BOOL bUseProxy);



/**
@brief app处理当前页面的url
@param[in] s_url:   当前页面的url链表
返回值: void
*/
typedef void(*fnAppDealUrlListFun)(CSGTUrlList_S * s_url);

/**
@brief  注册app获取当前页面元素所有的url
@param[in] fun  APP 处理CSGTUrlList_S的接口    
@param[in] userdata 用户数据
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSRegisterGetUrlListCallback(fnAppDealUrlListFun dealTime, void *pvUserData);
#ifdef __cplusplus
}
#endif
#endif

/** @} */



