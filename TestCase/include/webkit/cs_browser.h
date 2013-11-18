/**@defgroup WebKit Browser定义浏览器与中间件各模块交互接口

@brief Browser模块对应头文件cs_browser.h，定义了浏览器与中间件各模块交互接口，
 定义了浏览器创建窗口、打开网页，操作网页等

@note 

@version 1.0.0 2012/02/01 初稿
@{
*/

#ifndef _CS_BROWSER_H
#define _CS_BROWSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**@brief 设置浏览器通用属性
   通过CSBrwSetWebAttribute函数设置属性，pvAttr具体属性值，根据实际属性定义
*/
typedef enum _CSBrwAttr_E
{
    EM_BRW_ATTR_PROGRESS_INFO,            ///< 设置进度条属性, pvAttr为CSBrwProgressInfo_S类型地址, 整个浏览器全局有效
    EM_BRW_ATTR_DISABLE_DRAW_FOCUS,       ///< 浏览器执行焦点算法并执行焦点操作，但不画焦点框，默认画焦点，pvAttr is int, 1 禁止，0 允许，当前WebView全局有效
    EM_BRW_ATTR_DISABLE_FOCUS,            ///< 浏览器完全禁止执行默认焦点算法操作，pvAttr is int, 1 禁止，0 允许，当前WebView全局有效
    EM_BRW_ATTR_HOTKEY_INFO,              ///< 设置浏览器热键处理，网页没有捕获的按键如果设置了热键执行热键操作，支持多注册，pvAttr是CSBrwHotKey_S地址, 可支持单独WebView设置
    EM_BRW_ATTR_CACHE_SIZE,               ///< 设置浏览器缓存大小, pvAttr is int, 单位是字节, 默认大小8M，浏览器全局有效
    EM_BRW_ATTR_ENCODE_TYPE,              ///< 设置浏览器默认编码方式，格式为ISO 字符集，默认为gb2312, pvAttr是字符串，如"gbk" "gb2312" "utf-8"，浏览器全局有效
    EM_BRW_ATTR_DISABLE_KEEP_ALIVE,       ///< 设置浏览器禁止长连接方式，pvAttr is int, 1 禁止，0 允许，默认允许，全局浏览器有效
    EM_BRW_ATTR_HTTP401_ACCESS_INFO,      ///< 设置浏览器http 401对话框的默认用户名和密码，整个浏览器有效,详细可见CSBrwHttp401Info_S说明
    EM_BRW_ATTR_DST_OFFSET,               ///< 设置浏览器夏令时偏移时间秒数，默认的是0s，整个浏览器有效
    EM_BRW_ATTR_UTC_OFFSET,               ///< 设置浏览器时区偏移标准时间秋秒数，默认是8*3600s，整个浏览器有效
    EM_BRW_ATTR_ENABLE_SIMULATE_MOUSE,     ///< 设置浏览器设置焦点的同时是否需要模拟触发鼠标事件，CSBrwSetWebAttribute第一个参数传空时整个浏览器有效， pvAttr is bool，默认是ture，即允许触发 
    EM_BRW_ATTR_HISTORY_CAPACITY,		   ///< 设置浏览器历史记录保存的总个数， pvAttr is int, 默认保存100个, 整个浏览器有效
    EM_BRW_ATTR_SET_SUPPORTEXCEPTION,      ///< 设置浏览器是否抛异常，pvAttr is int, 0表示不抛，1表示抛，默认抛异常，整个浏览器有效
    EM_BRW_ATTR_WEBVIEW_ATTR,             ///< 设置浏览器page页面的属性只用于设置page的name,page的打开主面，还用于获取新建page的属性，包括page的名称，状态，具体可见CSBrwWebViewAttr_S, 需要创建page成功后才可用
    EM_BRW_ATTR_INPUTBOX_SIZE,			  ///< 设置输入框的大小,pvAttr is int , 默认输入框大小为20，应用可以根据需要用方法设置输入框的大小
    EM_BRW_ATTR_SET_SUPPORTSMARTEPG,	  ///< 设置是否支持smartEpg功能,pvAttr is int, 1 为支持，0为不支持，默认不支持，全局浏览器有效
    EM_BRW_ATTR_ERROR_PAGE_PATH           ///< 设置浏览器错误页面地址,pvAttr is char*, 长度不超作1024,默认地址为"/root/notfound.html"
}CSBrwAttr_E;

typedef enum _CSBrwListenerEvtType_E
{
    EM_BRW_LINSTENER_LOAD_FINISH,         ///< 监听浏览器下载完成
    EM_BRW_LINSTENER_URL_USERAGENT,       ///< 监听浏览器的user agent
    EM_BRW_LINSTENER_EVENT_TYPE           ///< 监听浏览器的事件类型，详细可见CSBrwEventType_E，在回调CSBrwListener_F中pvPara0  传事件CSBrwEvent_S的地址
}CSBrwListenerEvtType_E;


/**@brief 浏览器事件类型，键盘、鼠标、JS消息*/
typedef enum _CSBrwEventType_E
{	
    EM_BRW_EVNET_TYPE_KEYBODRD = 1,
    EM_BRW_EVNET_TYPE_MOUSE,
    EM_BRW_EVNET_TYPE_PANEL,
    EM_BRW_EVNET_TYPE_JS_MSG,
    EM_BRW_EVNET_TYPE_JS_APP_MSG,
    EM_BRW_EVNET_TYPE_JS_JSON_CHAR
}CSBrwEventType_E;

typedef enum _CSBrwAction_E
{
    EM_BRW_ACTION_GOBACK,        ///< 浏览器执行返回操作
    EM_BRW_ACTION_REPAINT,       ///< 可设置区域，pvUserData为CSUDIOSGRect_S地址，全屏刷新pvUserData为NULL
    EM_BRW_ACTION_RELOAD,        ///< 浏览器执行重新加载刷新操作
    EM_BRW_ACTION_CLEAR_PAGE     ///< 浏览器执行清屏工作，应用在调用 CSBrwSetVisible为false前需要执行该操作
}CSBrwAction_E;

typedef enum _CSBrwWebViewState_E
{
	EM_BRW_WEBVIEW_NONE = 0,    ///< page 不存在
	EM_BRW_WEBVIEW_VISIBLE= 1,  ///< page处于可见状态，即处于焦点状态
	EM_BRW_WEBVIEW_HIDE = 2     ///< pgae处于隐藏状态
}CSBrwWebViewState_E;

typedef struct  _CSBrwEvent_S
{
    unsigned int m_dwPara0;  ///< 事件类型 CSBrwEventType_E
    unsigned int m_dwPara1;  ///< 如果是键盘事件，则此参数传按键状态值 CSUDIInputKeyStatus_E
    unsigned int m_dwPara2;  ///< 如果是键盘事件为键值，如果是JS事件为JS自定义值
    unsigned int m_dwPara3;  ///< 如果CSBrwEventType_E为EM_BRW_EVNET_TYPE_JS_APP_MSG则值为usrInt类型
}CSBrwEvent_S;

/**@brief 设置浏览器进度条的相关信息
    默认启动时间2s，持续时间30s，位置在窗口中间
   工程中从browser600param.properties配制文件中读取参数值，
   配制文件可以不同时设置四个参数,如果不设置值传-1
*/
typedef struct _CSBrwProgressInfo_S
{
    int m_nPosX;             ///< 设置进度条显示位置的横坐标
    int m_nPosY;             ///< 设置进度条显示位置的纵坐标
    int m_nStartTime;        ///< 设置启动进度条的时间
    int m_nDuration;         ///< 设置进度条持续时间
    char *m_pcResPath;       ///< 下载进度条资源存放目录路径，有个性化项目设置提供资源文件
    int m_nResPathLen;       ///< 下载进度条资源存放目录路径长度
}CSBrwProgressInfo_S;

typedef struct _CSBrwHotKey_S
{
    int m_nKeyCode;     ///< 热键键值
    char *m_pcUrl;      ///< 热键键值对应的URL, 编码为本地码，本地码类型根据kernel中定义的MMCP_GetDefaultCodePage函数获取，如果传入为空表示清除对应keycode快捷键
    int m_nUrlLen;      ///< URL长度，如果传入的长度为0表示清除对应keycode快捷键
}CSBrwHotKey_S;

/**
@brief 设置浏览器http 401对话框的默认用户名和密码
 工程中从browser600param.properties配制文件中读取参数值, 如果不用可以放空.
*/
typedef struct _CSBrwHttp401Info_S
{
    char m_caUsername[16];   ///< 默认用户名
    char m_caPassword[16];   ///< 默认密码
}CSBrwHttp401Info_S;

/**
@brief  浏览器page的信息

*/
typedef struct  _CSBrwWebViewAttr_S
{
    CSBrwWebViewState_E m_state;  ///< page的状态信息，CSBrwEventType_E
    char m_caPageName[16];  ///< 创建page 的名称，方便识别，主page取名为"main_page"
    HCSHANDLE m_hPageHandle;  ///< 创建page的handle，即调用CSBrwCreateWebView创建page的返回值
    char m_caIndexUrl[1024];     ///< 创建page的主页
    BOOL m_bFoucs;              ///< 创建的page是否支持焦点
}CSBrwWebViewAttr_S;


/**@brief 浏览器监听函数
@param[in] emAttr CSBrwListenerAttr_E类型，具体功能见CSBrwListenerAttr_E说明
@param[in] pvPara0  pvPara1是根据emAttr类型指定具体形式
@param[in] pvUserData 用户数据
@return 0-用户已处理了该类型，不执行其他监听器; 
        -1-用户未处理监听类型，需要继续执行其他监听器; 
		1-用户处理了监听类型，并要求继续执行其他监听器
*/
typedef int (*CSBrwListener_F)(CSBrwListenerEvtType_E emType, void *pvPara0, void *pvPara1, void *pvUserData);


/**@brief 浏览器初始化接口

@param[in] nArgc 是argv命令行的参数个数,至少大于0
@param[in] pcArgv 是命令行参数，必须带一个或多个命令
@return 成功返回TRUE；失败则返回FALSE
@note 初始化浏览器。
@code
//参考实现
void main(int argc, char **argv)
{
    CSBrwInit(argc, argv);
}

ew600后端驱动为DFB，则命令行启动命令:
./main.out -qws -display directfb --dfb:bg-color=00000000,module-dir=/usr/local/lib/directfb-1.4-0
@endcode
*/
BOOL CSBrwInit(int nArgc, char *pcArgv[]);

/**@brief 浏览器接口
@return 无
@note 启动浏览器，该接口阻塞式。
@code
//参考实现
void main(int argc, char **argv)
{
    CSBrwInit(argc, argv);
	do something...
	CSBrwRun();
}
@endcode
*/
void CSBrwRun(void);

/**@brief 设置浏览器或指定WebView通用属性
@param[in] hWebView 浏览器webview句柄, NULL对所有WebView全局有效
@param[in] emAttr pvAttr 说明详见CSBrwAttr_E定义
@return 无

@note 可以在CSBrwInit之前调用，nWebView传入默认参数NULL。
*/
void CSBrwSetAttribute(HCSHANDLE hWebView, CSBrwAttr_E emAttr, void *pvAttr);

/**@brief 设置浏览器或指定WebView通用属性
@param[in] hWebView 浏览器webview句柄, NULL对所有WebView全局有效
@param[in] emAttr pvAttr 说明详见CSBrwAttr_E定义
@return 具体属性

@note 可以在CSBrwInit之前调用。
*/
void* CSBrwGetAttribute(HCSHANDLE nWebView, CSBrwAttr_E emAttr);

/**@brief 创建一个新的浏览器WebView，多Page时使用
@param[in] nX 浏览器初始x方向位置
@param[in] nY 浏览器初始y方向位置
@param[in] nW 浏览器初始宽度
@param[in] nH 浏览器初始高度
@return WebView句柄，失败返回NULL

@note 初始化CSBrwInit之后才能调用，该函数在多Page时才有用。
@note nX,nY, nW,nH设置都为0，则创建全屏的Page。
@note 创建出来的WebView处于后台,需要调用CSBrwSetVisible(webview, TRUE)将webview抬升并获得焦点。
@code
//参考实现
HCSHANDLE createWebPage(void)
{
    // 后台创建WebPage
	HCSHANDLE webview = CSBrwCreateWebView(0, 0, 0, 0);
    
	// 抬升webview并获取焦点
	if (webview)
	   CSBrwSetVisible(webview, TRUE);

	return webview;
}
@endcode
*/
HCSHANDLE CSBrwCreateWebView(int nX, int nY, int nW, int nH);

/**@brief 销毁指定浏览器WebView
@param[in] hWebView 浏览器webview句柄
@return 无

@note 初始化CSBrwInit之后才能调用，该函数在多Page时才有用。
@note 销毁之后，临近的上一个WebView获取焦点。
*/
void CSBrwDestroyWebView(HCSHANDLE hWebView);

/**@brief 获取当前焦点浏览器WebView句柄
@return 当前焦点Webview浏览器句柄

@note 初始化CSBrwInit之后才能调用。
*/
HCSHANDLE CSBrwGetCurWebView(void);

/**@brief 获取当前浏览器所有WebView个数
@return 当前所有Webview个数,个数>=1有效

@note 初始化CSBrwInit之后才能调用。
*/
int CSBrwGetWebViewCount(void);

/**@brief 根据指定位置获取WebView句柄
@return 指定位置的webview的句柄

@note 初始化CSBrwInit之后才能调用。
@code
//参考实现
void getWebPageWithPos(void)
{
    // 获取所有WebView个数
	int viewCount = CSBrwGetWebViewCount();
    
	// 获取指定位置的WebView
	for(int i = 0; i < viewCount; i++)
	   HCSHANDLE view = CSBrwGetWebViewByIndex(i);
}
@endcode
*/
HCSHANDLE CSBrwGetWebViewByIndex(int nIndex);

/**@brief 显示/隐藏指定WebView
@param[in] nWebView 浏览器webview句柄
@param[in] bVisible TRUE激活Webview并显示，FALSE隐藏Webview
@return 无

@note 初始化CSBrwInit之后才能调用。
@code
//参考实现
HCSHANDLE createWebPage(void)
{
    // 后台创建WebPage
	HCSHANDLE webview = CSBrwCreateWebView(0, 0, 0, 0);
    
	// 抬升webview
	if (webview)
	   CSBrwSetVisible(webview, TRUE);

	return webview;
}
@endcode
*/
void CSBrwSetVisible(HCSHANDLE hWebView, BOOL bVisible);

/**@brief 重新指定WebView位置大小
@param[in] hWebView 浏览器webview句柄, 传入NULL为当前焦点WebView
@param[in] nWidth 浏览器宽度大小
@param[in] nHeight move 浏览器高度大小
@return 无

@note 初始化CSBrwInit之后才能调用。
@code
//参考实现
HCSHANDLE createWebPage(void)
{
    // 后台创建WebPage
	HCSHANDLE webview = CSBrwCreateWebView(0, 0, 0, 0);
    
	// 重置指定WebView窗口位置大小
	if (webview) 
	   CSBrwResizeWebView(webview, 100, 100);

	return webview;
}
@endcode
*/
void CSBrwResizeWebView(HCSHANDLE hWebView, int nWidth, int nHeight);

/**@brief 重新指定WebView位置横纵坐标
@param[in] hWebView 浏览器webview句柄, 传入NULL为当前焦点WebView
@param[in] nPosX move x方向的坐标，-1时不做移动
@param[in] nPosY move y方向的坐标，-1时不做移动
@return 无

@note 初始化CSBrwInit之后才能调用。
@code
//参考实现
HCSHANDLE createWebPage(void)
{
// 后台创建WebPage
HCSHANDLE webview = CSBrwCreateWebView(0, 0, 0, 0);

// 重置指定WebView窗口位置大小
if (webview) 
CSBrwResizeWebView(webview, 200, 100);

return webview;
}
@endcode
*/
void CSBrwMoveToWebView(HCSHANDLE hWebView, int nPosX, int nPosY);

/**@brief 在指定WebView中打开网页链接地址
@param[in] hWebView 浏览器webview句柄, 传入NULL为当前激活WebView
@param[in] pcURL 打开链接地址URL
@param[in] nURLLen 打开链接地址URL的字符串长度
@return 无

@note 初始化CSBrwInit之后才能调用。
@note URL编码格式是本地码，本地码类型根据kernel中定义的MMCP_GetDefaultCodePage函数获取。
*/
void CSBrwOpenURL(HCSHANDLE hWebView, const char *pcURL, int nURLLen);

/**@brief 向指定WebView中发送消息
@param[in] hWebView 浏览器webview句柄, 传入NULL为当前焦点WebView
@param[in] sEvent 事件消息，类型说明详见CSBrwEvent_S
@return 

@note 初始化CSBrwInit之后才能调用。
*/
int CSBrwPostEvent(HCSHANDLE hWebView, CSBrwEvent_S sEvent);

/**@brief 从指定WebView中获取当前打开的页面的URL
@param[in] hWebView 浏览器webview句柄, 传入NULL为当前焦点WebView
@param[out] pcURL 输入存放当前URL的地址，输出当前URL内容
@param[in] nUrlLen 输入存放当前URL的地址的长度
@return 无

@note 初始化CSBrwInit之后才能调用。
*/
void CSBrwGetCurURL(HCSHANDLE hWebView, char *pcURL, int nUrlLen);

/**@brief 执行浏览器操作
@param[in] hWebView 浏览器webview句柄, NULL对当前焦点WebView有效
@param[in] emAction pvUserData说明详见CSBrwAction_E定义
@return 无

@note 初始化CSBrwInit之后才能调用。
*/
void CSBrwDoAction(HCSHANDLE hWebView, CSBrwAction_E emAction, void *pvUserData);

/**@brief 添加浏览器监听器
@param[in] fnListener 监听器实现函数
@param[in] pvUserData 用户数据
@return 无

@note 可在CSBrwInit初始化之前调用
@note 支持多注册
*/
void CSBrwAddListener(CSBrwListener_F fnListener, void *pvUserData);

/**@brief 删除浏览器监听器
@param[in] fnListener 监听器实现函数
@return 无

@note 可在CSBrwInit初始化之前调用
*/
void CSBrwRemoveListener(CSBrwListener_F fnListener);


#ifdef __cplusplus
}
#endif
/**@}*/
#endif
