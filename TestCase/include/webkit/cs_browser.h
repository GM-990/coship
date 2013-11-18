/**@defgroup WebKit Browser������������м����ģ�齻���ӿ�

@brief Browserģ���Ӧͷ�ļ�cs_browser.h����������������м����ģ�齻���ӿڣ�
 ������������������ڡ�����ҳ��������ҳ��

@note 

@version 1.0.0 2012/02/01 ����
@{
*/

#ifndef _CS_BROWSER_H
#define _CS_BROWSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**@brief ���������ͨ������
   ͨ��CSBrwSetWebAttribute�����������ԣ�pvAttr��������ֵ������ʵ�����Զ���
*/
typedef enum _CSBrwAttr_E
{
    EM_BRW_ATTR_PROGRESS_INFO,            ///< ���ý���������, pvAttrΪCSBrwProgressInfo_S���͵�ַ, ���������ȫ����Ч
    EM_BRW_ATTR_DISABLE_DRAW_FOCUS,       ///< �����ִ�н����㷨��ִ�н�������������������Ĭ�ϻ����㣬pvAttr is int, 1 ��ֹ��0 ������ǰWebViewȫ����Ч
    EM_BRW_ATTR_DISABLE_FOCUS,            ///< �������ȫ��ִֹ��Ĭ�Ͻ����㷨������pvAttr is int, 1 ��ֹ��0 ������ǰWebViewȫ����Ч
    EM_BRW_ATTR_HOTKEY_INFO,              ///< ����������ȼ�������ҳû�в���İ�������������ȼ�ִ���ȼ�������֧�ֶ�ע�ᣬpvAttr��CSBrwHotKey_S��ַ, ��֧�ֵ���WebView����
    EM_BRW_ATTR_CACHE_SIZE,               ///< ��������������С, pvAttr is int, ��λ���ֽ�, Ĭ�ϴ�С8M�������ȫ����Ч
    EM_BRW_ATTR_ENCODE_TYPE,              ///< ���������Ĭ�ϱ��뷽ʽ����ʽΪISO �ַ�����Ĭ��Ϊgb2312, pvAttr���ַ�������"gbk" "gb2312" "utf-8"�������ȫ����Ч
    EM_BRW_ATTR_DISABLE_KEEP_ALIVE,       ///< �����������ֹ�����ӷ�ʽ��pvAttr is int, 1 ��ֹ��0 ����Ĭ������ȫ���������Ч
    EM_BRW_ATTR_HTTP401_ACCESS_INFO,      ///< ���������http 401�Ի����Ĭ���û��������룬�����������Ч,��ϸ�ɼ�CSBrwHttp401Info_S˵��
    EM_BRW_ATTR_DST_OFFSET,               ///< �������������ʱƫ��ʱ��������Ĭ�ϵ���0s�������������Ч
    EM_BRW_ATTR_UTC_OFFSET,               ///< ���������ʱ��ƫ�Ʊ�׼ʱ����������Ĭ����8*3600s�������������Ч
    EM_BRW_ATTR_ENABLE_SIMULATE_MOUSE,     ///< ������������ý����ͬʱ�Ƿ���Ҫģ�ⴥ������¼���CSBrwSetWebAttribute��һ����������ʱ�����������Ч�� pvAttr is bool��Ĭ����ture���������� 
    EM_BRW_ATTR_HISTORY_CAPACITY,		   ///< �����������ʷ��¼������ܸ����� pvAttr is int, Ĭ�ϱ���100��, �����������Ч
    EM_BRW_ATTR_SET_SUPPORTEXCEPTION,      ///< ����������Ƿ����쳣��pvAttr is int, 0��ʾ���ף�1��ʾ�ף�Ĭ�����쳣�������������Ч
    EM_BRW_ATTR_WEBVIEW_ATTR,             ///< ���������pageҳ�������ֻ��������page��name,page�Ĵ����棬�����ڻ�ȡ�½�page�����ԣ�����page�����ƣ�״̬������ɼ�CSBrwWebViewAttr_S, ��Ҫ����page�ɹ���ſ���
    EM_BRW_ATTR_INPUTBOX_SIZE,			  ///< ���������Ĵ�С,pvAttr is int , Ĭ��������СΪ20��Ӧ�ÿ��Ը�����Ҫ�÷������������Ĵ�С
    EM_BRW_ATTR_SET_SUPPORTSMARTEPG,	  ///< �����Ƿ�֧��smartEpg����,pvAttr is int, 1 Ϊ֧�֣�0Ϊ��֧�֣�Ĭ�ϲ�֧�֣�ȫ���������Ч
    EM_BRW_ATTR_ERROR_PAGE_PATH           ///< �������������ҳ���ַ,pvAttr is char*, ���Ȳ�����1024,Ĭ�ϵ�ַΪ"/root/notfound.html"
}CSBrwAttr_E;

typedef enum _CSBrwListenerEvtType_E
{
    EM_BRW_LINSTENER_LOAD_FINISH,         ///< ����������������
    EM_BRW_LINSTENER_URL_USERAGENT,       ///< �����������user agent
    EM_BRW_LINSTENER_EVENT_TYPE           ///< ������������¼����ͣ���ϸ�ɼ�CSBrwEventType_E���ڻص�CSBrwListener_F��pvPara0  ���¼�CSBrwEvent_S�ĵ�ַ
}CSBrwListenerEvtType_E;


/**@brief ������¼����ͣ����̡���ꡢJS��Ϣ*/
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
    EM_BRW_ACTION_GOBACK,        ///< �����ִ�з��ز���
    EM_BRW_ACTION_REPAINT,       ///< ����������pvUserDataΪCSUDIOSGRect_S��ַ��ȫ��ˢ��pvUserDataΪNULL
    EM_BRW_ACTION_RELOAD,        ///< �����ִ�����¼���ˢ�²���
    EM_BRW_ACTION_CLEAR_PAGE     ///< �����ִ������������Ӧ���ڵ��� CSBrwSetVisibleΪfalseǰ��Ҫִ�иò���
}CSBrwAction_E;

typedef enum _CSBrwWebViewState_E
{
	EM_BRW_WEBVIEW_NONE = 0,    ///< page ������
	EM_BRW_WEBVIEW_VISIBLE= 1,  ///< page���ڿɼ�״̬�������ڽ���״̬
	EM_BRW_WEBVIEW_HIDE = 2     ///< pgae��������״̬
}CSBrwWebViewState_E;

typedef struct  _CSBrwEvent_S
{
    unsigned int m_dwPara0;  ///< �¼����� CSBrwEventType_E
    unsigned int m_dwPara1;  ///< ����Ǽ����¼�����˲���������״ֵ̬ CSUDIInputKeyStatus_E
    unsigned int m_dwPara2;  ///< ����Ǽ����¼�Ϊ��ֵ�������JS�¼�ΪJS�Զ���ֵ
    unsigned int m_dwPara3;  ///< ���CSBrwEventType_EΪEM_BRW_EVNET_TYPE_JS_APP_MSG��ֵΪusrInt����
}CSBrwEvent_S;

/**@brief ����������������������Ϣ
    Ĭ������ʱ��2s������ʱ��30s��λ���ڴ����м�
   �����д�browser600param.properties�����ļ��ж�ȡ����ֵ��
   �����ļ����Բ�ͬʱ�����ĸ�����,���������ֵ��-1
*/
typedef struct _CSBrwProgressInfo_S
{
    int m_nPosX;             ///< ���ý�������ʾλ�õĺ�����
    int m_nPosY;             ///< ���ý�������ʾλ�õ�������
    int m_nStartTime;        ///< ����������������ʱ��
    int m_nDuration;         ///< ���ý���������ʱ��
    char *m_pcResPath;       ///< ���ؽ�������Դ���Ŀ¼·�����и��Ի���Ŀ�����ṩ��Դ�ļ�
    int m_nResPathLen;       ///< ���ؽ�������Դ���Ŀ¼·������
}CSBrwProgressInfo_S;

typedef struct _CSBrwHotKey_S
{
    int m_nKeyCode;     ///< �ȼ���ֵ
    char *m_pcUrl;      ///< �ȼ���ֵ��Ӧ��URL, ����Ϊ�����룬���������͸���kernel�ж����MMCP_GetDefaultCodePage������ȡ���������Ϊ�ձ�ʾ�����Ӧkeycode��ݼ�
    int m_nUrlLen;      ///< URL���ȣ��������ĳ���Ϊ0��ʾ�����Ӧkeycode��ݼ�
}CSBrwHotKey_S;

/**
@brief ���������http 401�Ի����Ĭ���û���������
 �����д�browser600param.properties�����ļ��ж�ȡ����ֵ, ������ÿ��Էſ�.
*/
typedef struct _CSBrwHttp401Info_S
{
    char m_caUsername[16];   ///< Ĭ���û���
    char m_caPassword[16];   ///< Ĭ������
}CSBrwHttp401Info_S;

/**
@brief  �����page����Ϣ

*/
typedef struct  _CSBrwWebViewAttr_S
{
    CSBrwWebViewState_E m_state;  ///< page��״̬��Ϣ��CSBrwEventType_E
    char m_caPageName[16];  ///< ����page �����ƣ�����ʶ����pageȡ��Ϊ"main_page"
    HCSHANDLE m_hPageHandle;  ///< ����page��handle��������CSBrwCreateWebView����page�ķ���ֵ
    char m_caIndexUrl[1024];     ///< ����page����ҳ
    BOOL m_bFoucs;              ///< ������page�Ƿ�֧�ֽ���
}CSBrwWebViewAttr_S;


/**@brief �������������
@param[in] emAttr CSBrwListenerAttr_E���ͣ����幦�ܼ�CSBrwListenerAttr_E˵��
@param[in] pvPara0  pvPara1�Ǹ���emAttr����ָ��������ʽ
@param[in] pvUserData �û�����
@return 0-�û��Ѵ����˸����ͣ���ִ������������; 
        -1-�û�δ����������ͣ���Ҫ����ִ������������; 
		1-�û������˼������ͣ���Ҫ�����ִ������������
*/
typedef int (*CSBrwListener_F)(CSBrwListenerEvtType_E emType, void *pvPara0, void *pvPara1, void *pvUserData);


/**@brief �������ʼ���ӿ�

@param[in] nArgc ��argv�����еĲ�������,���ٴ���0
@param[in] pcArgv �������в����������һ����������
@return �ɹ�����TRUE��ʧ���򷵻�FALSE
@note ��ʼ���������
@code
//�ο�ʵ��
void main(int argc, char **argv)
{
    CSBrwInit(argc, argv);
}

ew600�������ΪDFB������������������:
./main.out -qws -display directfb --dfb:bg-color=00000000,module-dir=/usr/local/lib/directfb-1.4-0
@endcode
*/
BOOL CSBrwInit(int nArgc, char *pcArgv[]);

/**@brief ������ӿ�
@return ��
@note ������������ýӿ�����ʽ��
@code
//�ο�ʵ��
void main(int argc, char **argv)
{
    CSBrwInit(argc, argv);
	do something...
	CSBrwRun();
}
@endcode
*/
void CSBrwRun(void);

/**@brief �����������ָ��WebViewͨ������
@param[in] hWebView �����webview���, NULL������WebViewȫ����Ч
@param[in] emAttr pvAttr ˵�����CSBrwAttr_E����
@return ��

@note ������CSBrwInit֮ǰ���ã�nWebView����Ĭ�ϲ���NULL��
*/
void CSBrwSetAttribute(HCSHANDLE hWebView, CSBrwAttr_E emAttr, void *pvAttr);

/**@brief �����������ָ��WebViewͨ������
@param[in] hWebView �����webview���, NULL������WebViewȫ����Ч
@param[in] emAttr pvAttr ˵�����CSBrwAttr_E����
@return ��������

@note ������CSBrwInit֮ǰ���á�
*/
void* CSBrwGetAttribute(HCSHANDLE nWebView, CSBrwAttr_E emAttr);

/**@brief ����һ���µ������WebView����Pageʱʹ��
@param[in] nX �������ʼx����λ��
@param[in] nY �������ʼy����λ��
@param[in] nW �������ʼ���
@param[in] nH �������ʼ�߶�
@return WebView�����ʧ�ܷ���NULL

@note ��ʼ��CSBrwInit֮����ܵ��ã��ú����ڶ�Pageʱ�����á�
@note nX,nY, nW,nH���ö�Ϊ0���򴴽�ȫ����Page��
@note ����������WebView���ں�̨,��Ҫ����CSBrwSetVisible(webview, TRUE)��webview̧������ý��㡣
@code
//�ο�ʵ��
HCSHANDLE createWebPage(void)
{
    // ��̨����WebPage
	HCSHANDLE webview = CSBrwCreateWebView(0, 0, 0, 0);
    
	// ̧��webview����ȡ����
	if (webview)
	   CSBrwSetVisible(webview, TRUE);

	return webview;
}
@endcode
*/
HCSHANDLE CSBrwCreateWebView(int nX, int nY, int nW, int nH);

/**@brief ����ָ�������WebView
@param[in] hWebView �����webview���
@return ��

@note ��ʼ��CSBrwInit֮����ܵ��ã��ú����ڶ�Pageʱ�����á�
@note ����֮���ٽ�����һ��WebView��ȡ���㡣
*/
void CSBrwDestroyWebView(HCSHANDLE hWebView);

/**@brief ��ȡ��ǰ���������WebView���
@return ��ǰ����Webview��������

@note ��ʼ��CSBrwInit֮����ܵ��á�
*/
HCSHANDLE CSBrwGetCurWebView(void);

/**@brief ��ȡ��ǰ���������WebView����
@return ��ǰ����Webview����,����>=1��Ч

@note ��ʼ��CSBrwInit֮����ܵ��á�
*/
int CSBrwGetWebViewCount(void);

/**@brief ����ָ��λ�û�ȡWebView���
@return ָ��λ�õ�webview�ľ��

@note ��ʼ��CSBrwInit֮����ܵ��á�
@code
//�ο�ʵ��
void getWebPageWithPos(void)
{
    // ��ȡ����WebView����
	int viewCount = CSBrwGetWebViewCount();
    
	// ��ȡָ��λ�õ�WebView
	for(int i = 0; i < viewCount; i++)
	   HCSHANDLE view = CSBrwGetWebViewByIndex(i);
}
@endcode
*/
HCSHANDLE CSBrwGetWebViewByIndex(int nIndex);

/**@brief ��ʾ/����ָ��WebView
@param[in] nWebView �����webview���
@param[in] bVisible TRUE����Webview����ʾ��FALSE����Webview
@return ��

@note ��ʼ��CSBrwInit֮����ܵ��á�
@code
//�ο�ʵ��
HCSHANDLE createWebPage(void)
{
    // ��̨����WebPage
	HCSHANDLE webview = CSBrwCreateWebView(0, 0, 0, 0);
    
	// ̧��webview
	if (webview)
	   CSBrwSetVisible(webview, TRUE);

	return webview;
}
@endcode
*/
void CSBrwSetVisible(HCSHANDLE hWebView, BOOL bVisible);

/**@brief ����ָ��WebViewλ�ô�С
@param[in] hWebView �����webview���, ����NULLΪ��ǰ����WebView
@param[in] nWidth �������ȴ�С
@param[in] nHeight move ������߶ȴ�С
@return ��

@note ��ʼ��CSBrwInit֮����ܵ��á�
@code
//�ο�ʵ��
HCSHANDLE createWebPage(void)
{
    // ��̨����WebPage
	HCSHANDLE webview = CSBrwCreateWebView(0, 0, 0, 0);
    
	// ����ָ��WebView����λ�ô�С
	if (webview) 
	   CSBrwResizeWebView(webview, 100, 100);

	return webview;
}
@endcode
*/
void CSBrwResizeWebView(HCSHANDLE hWebView, int nWidth, int nHeight);

/**@brief ����ָ��WebViewλ�ú�������
@param[in] hWebView �����webview���, ����NULLΪ��ǰ����WebView
@param[in] nPosX move x��������꣬-1ʱ�����ƶ�
@param[in] nPosY move y��������꣬-1ʱ�����ƶ�
@return ��

@note ��ʼ��CSBrwInit֮����ܵ��á�
@code
//�ο�ʵ��
HCSHANDLE createWebPage(void)
{
// ��̨����WebPage
HCSHANDLE webview = CSBrwCreateWebView(0, 0, 0, 0);

// ����ָ��WebView����λ�ô�С
if (webview) 
CSBrwResizeWebView(webview, 200, 100);

return webview;
}
@endcode
*/
void CSBrwMoveToWebView(HCSHANDLE hWebView, int nPosX, int nPosY);

/**@brief ��ָ��WebView�д���ҳ���ӵ�ַ
@param[in] hWebView �����webview���, ����NULLΪ��ǰ����WebView
@param[in] pcURL �����ӵ�ַURL
@param[in] nURLLen �����ӵ�ַURL���ַ�������
@return ��

@note ��ʼ��CSBrwInit֮����ܵ��á�
@note URL�����ʽ�Ǳ����룬���������͸���kernel�ж����MMCP_GetDefaultCodePage������ȡ��
*/
void CSBrwOpenURL(HCSHANDLE hWebView, const char *pcURL, int nURLLen);

/**@brief ��ָ��WebView�з�����Ϣ
@param[in] hWebView �����webview���, ����NULLΪ��ǰ����WebView
@param[in] sEvent �¼���Ϣ������˵�����CSBrwEvent_S
@return 

@note ��ʼ��CSBrwInit֮����ܵ��á�
*/
int CSBrwPostEvent(HCSHANDLE hWebView, CSBrwEvent_S sEvent);

/**@brief ��ָ��WebView�л�ȡ��ǰ�򿪵�ҳ���URL
@param[in] hWebView �����webview���, ����NULLΪ��ǰ����WebView
@param[out] pcURL �����ŵ�ǰURL�ĵ�ַ�������ǰURL����
@param[in] nUrlLen �����ŵ�ǰURL�ĵ�ַ�ĳ���
@return ��

@note ��ʼ��CSBrwInit֮����ܵ��á�
*/
void CSBrwGetCurURL(HCSHANDLE hWebView, char *pcURL, int nUrlLen);

/**@brief ִ�����������
@param[in] hWebView �����webview���, NULL�Ե�ǰ����WebView��Ч
@param[in] emAction pvUserData˵�����CSBrwAction_E����
@return ��

@note ��ʼ��CSBrwInit֮����ܵ��á�
*/
void CSBrwDoAction(HCSHANDLE hWebView, CSBrwAction_E emAction, void *pvUserData);

/**@brief ��������������
@param[in] fnListener ������ʵ�ֺ���
@param[in] pvUserData �û�����
@return ��

@note ����CSBrwInit��ʼ��֮ǰ����
@note ֧�ֶ�ע��
*/
void CSBrwAddListener(CSBrwListener_F fnListener, void *pvUserData);

/**@brief ɾ�������������
@param[in] fnListener ������ʵ�ֺ���
@return ��

@note ����CSBrwInit��ʼ��֮ǰ����
*/
void CSBrwRemoveListener(CSBrwListener_F fnListener);


#ifdef __cplusplus
}
#endif
/**@}*/
#endif
