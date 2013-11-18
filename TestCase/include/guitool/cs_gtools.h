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

/*�����������Ӧ�õ���Ϣ����*/
typedef enum 
{
	EM_BRW_OC,
	EM_BRW_NETWORK,
	EM_BRW_SYSTEM,
	EM_BRW_KEYCODETRANS,  // ҳ�沶���ֵ������Ŀ��Ҫ����ת����dwParm1�����Ĭ�ϼ�ֵ���������dwParm2ת����ļ�ֵ(int) �������
	EM_BRW_WINDOW_CLOSE,  // ͨ��Window.close() �˳������
	EM_BRW_SYSTEM_OPENURL,	// ֪ͨӦ�����������ҳ���
	EM_BRW_SYSTEM_GOBACK, // ֪ͨӦ��������������˳ɹ�
	EM_BRW_SYSTEM_GOFORWARD,//  ֪ͨӦ�����������ǰ���ɹ�
	EM_BRW_SYSTEM_REPAINT, // ֪ͨӦ�������REPAINT �ɹ�
	EM_BRW_SYSTEM_REFRESH, // ֪ͨӦ�������ˢ��ҳ��ɹ�
	EM_BRW_SYSTEM_EXIT,	    // ֪ͨӦ��������˳��Ĺ��ܴ�����Ϣ,���������ͣ��ο��ص�"fnBrowserfun" ע��
	EM_BRW_SYSTEM_NOHISTORY, // ֪ͨӦ������Ѿ��ص���ҳ
	EM_BRW_JS_APP_MSG,       //֪ͨӦ�ã�ת��dvb��ϵͳ��Ϣ
	EM_BRW_SYSTEM_lOADCOMPETED, // ֪ͨ�������ǰ��ҳ��Դ������� 
	EM_BRW_FILE_URL_TO_LOAD,     // ֪ͨӦ�ã��׳�������fileЭ��򿪵�url.ͨ��wParam2�׳���url,url��ascii����.fnBrowserfun�ص������ķ���ֵ��int���ͣ�����ֵΪ1:��ֹ���ʸ�url ��0:����ֹ, Ĭ����0							 
	EM_BRW_KEYBOARDEVENT_PROCESS  //֪ͨӦ���ڴ��������Ϣ
}eBrwMsgType;

typedef enum
{        
    EM_BRW_FILTER_INPUTBOX,               // ����򲶻񰴼�        
    EM_BRW_FILTER_INPUTMETHOD,            // ���뷨���񰴼�          
    EM_BRW_FILTER_WEBPAGE,                // ҳ�沶�񰴼�������, ʹ��e.preventDefault();
    EM_BRW_NOTHING_FILTER                 // ҳ�沶�񰴼�ʲô��û����
}eBrwProcessType;

typedef enum
{
	EM_CABLE_LOST = 1, //Cable �ź��жϣ���ҪDVBConnectSourceCallBack_Fʵ����ȷ������
	EM_CABLE_RESUME,  //Cable �źŻָ�����ҪDVBConnectSourceCallBack_Fʵ����ȷ������
	EM_OPEN_DVB_FAIL,	//��dvb����ʧ��
	EM_OPEN_HOMEPAGE_FAIL, //��dvb��ҳʧ��
	EM_OPEN_DVB_TIMEOUT,  //��dvb���ӳ�ʱ
	EM_AIT_VER_CHANGE		// AIT�汾�仯
}eOCMsgBoxType; //according to EM_BRW_OC

typedef enum
{
	EM_SOCKET_CONNECT_TIMEOUT    = 1001 , /**socket���ӳ�ʱ*/
	EM_COULDNT_RESOLVE_HOST      = 2001 , /**�޷���������*/	
	EM_NO_DNS_SERVER             = 2002 , /**�Ҳ���DNS������*/	
	EM_NO_PROXY_SERVER           = 3001 , /**���Ӳ���PROXY������*/
	EM_PROXY_USER_PSD_WRONG      = 3002 , /**PROXY�û����������*/ 
	EM_HTTP_STATUS_CODE_100      = 4100 , /**HTTP״̬��100*/
	EM_HTTP_STATUS_CODE_101      = 4101 , /**HTTP״̬��101*/
	EM_HTTP_STATUS_CODE_201      = 4201 , /**HTTP״̬��201*/
	EM_HTTP_STATUS_CODE_202      = 4202 , /**HTTP״̬��202*/
	EM_HTTP_STATUS_CODE_203      = 4203 , /**HTTP״̬��203*/
	EM_HTTP_STATUS_CODE_204      = 4204 , /**HTTP״̬��204*/
	EM_HTTP_STATUS_CODE_205      = 4205 , /**HTTP״̬��205*/
	EM_HTTP_STATUS_CODE_206      = 4206 , /**HTTP״̬��206*/
	EM_HTTP_STATUS_CODE_300      = 4300 , /**HTTP״̬��300*/
	EM_HTTP_STATUS_CODE_301      = 4301 , /**HTTP״̬��301*/
	EM_HTTP_STATUS_CODE_303      = 4303 , /**HTTP״̬��303*/
	EM_HTTP_STATUS_CODE_304      = 4304 , /**HTTP״̬��304*/
	EM_HTTP_STATUS_CODE_307      = 4307 , /**HTTP״̬��307*/
	EM_HTTP_STATUS_CODE_400      = 4400 , /**HTTP״̬��400*/
	EM_HTTP_STATUS_CODE_401      = 4401 , /**HTTP״̬��401*/
	EM_HTTP_STATUS_CODE_402      = 4402 , /**HTTP״̬��402*/
	EM_HTTP_STATUS_CODE_403      = 4403 , /**HTTP״̬��403*/
	EM_HTTP_STATUS_CODE_404      = 4404 , /**HTTP״̬��404*/
	EM_HTTP_STATUS_CODE_405      = 4405 , /**HTTP״̬��405*/
	EM_HTTP_STATUS_CODE_406      = 4406 , /**HTTP״̬��406*/
	EM_HTTP_STATUS_CODE_407      = 4407 , /**HTTP״̬��407*/
	EM_HTTP_STATUS_CODE_408      = 4408 , /**HTTP״̬��408*/
	EM_HTTP_STATUS_CODE_409      = 4409 , /**HTTP״̬��409*/
	EM_HTTP_STATUS_CODE_410      = 4410 , /**HTTP״̬��410*/
	EM_HTTP_STATUS_CODE_411      = 4411 , /**HTTP״̬��411*/
	EM_HTTP_STATUS_CODE_412      = 4412 , /**HTTP״̬��412*/
	EM_HTTP_STATUS_CODE_413      = 4413 , /**HTTP״̬��413*/
	EM_HTTP_STATUS_CODE_414      = 4414 , /**HTTP״̬��414*/
	EM_HTTP_STATUS_CODE_415      = 4415 , /**HTTP״̬��415*/
	EM_HTTP_STATUS_CODE_416      = 4416 , /**HTTP״̬��416*/
	EM_HTTP_STATUS_CODE_417      = 4417 , /**HTTP״̬��417*/
	EM_HTTP_STATUS_CODE_500      = 4500 , /**HTTP״̬��500*/
	EM_HTTP_STATUS_CODE_501      = 4501 , /**HTTP״̬��501*/
	EM_HTTP_STATUS_CODE_502      = 4502 , /**HTTP״̬��502*/
	EM_HTTP_STATUS_CODE_503      = 4503 , /**HTTP״̬��503*/
	EM_HTTP_STATUS_CODE_504      = 4504 , /**HTTP״̬��504*/
	EM_HTTP_STATUS_CODE_505      = 4505   /**HTTP״̬��505*/
}eNetworkMsgType;//according to EM_BRW_NETWORK

typedef enum
{
	EM_NOMEMORY,
	EM_PARSERERROR,
	EM_NOTINIT
}eSystemMsgType;//according to EM_BRW_SYSTEM

typedef struct _CSBRWErrorPageInfo
{
  int   nErrorPageState;     /** Ӧ���Ƿ�򿪹�����ҳ�� */   
  const char* pzURL;               /** ���ִ����URL��ַ */
}CSBRWErrorPageInfo_S;

typedef enum _CSBRWErrorPageState
{
    EM_ERROR_PAGE_UNOPEN    = -1,  /**û�д򿪴���ҳ��  */
    EM_ERROR_PAGE_OPENED    =  4   /**�ѽ����˴���ҳ��*/
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
	GT_RET_FAILURE    = -1,  /**<guitool ����ӿڵ���ʧ��*/
	GT_RET_SUCCESS    =  0,  /**< guitool ����ӿڵ��óɹ�*/
	GT_RET_DEFAULT    =  1, /**< messagebox �ص�������չ*/
	GT_RET_STATE_ERR  =  2,/**<  �����״̬���� */
	GT_RET_NO_MEMORY  =  3,   /**<  ������ڴ治�� */
	GT_RET_MEMORY_LOCK = 4 /**< �����ڴ�û���ͷ�*/
}CSGTRet_E;

typedef enum _CSGTState_E   
{
	GT_STATE_FRONT   = 0, /**< ���������ǰ̨״̬ */
	GT_STATE_BACK    = 1, /**<  ��������ں�̨״̬ */
	GT_STATE_STANDBY = 2  /**< ������ͷ�������ҳ������ �������ʼ��ʱ�����ڴ��״̬ */
}CSGTState_E;

typedef struct _CSGTBrwInfo  
{
	int nMemorySize;       /**< guitool�ڴ�ش�С*/
	int nFreeMemorySize;   /**< guitoolʣ������ڴ��С*/
	int nCacheBufferSize;  /**< guitool �����С*/
	CSGTState_E eGTState;  /**< guitool ��ǰ״̬*/ 
	BOOL bIsFocusOnInput;  /**< guitool�Ľ����Ƿ�Ϊinput�����*/
}CSGTBrwInfo_S;


typedef struct _CSGTMixMemoryCfg
{
	int nBrwMemoryPoolSize;  /** < ����������ڴ�ش�С */
	int nBrwMaxMallocSize;   /** < ������ڴ�ʹ�÷�ֵ,
								   С�ڵ��ڸ�ֵʹ��������ڴ���ڴ�,����ʹ��ϵͳ�ڴ�.
								  */
}CSGTMixMemoryCfg_S;

typedef struct _CSGTLocalCacheCfg
{
	int nType;  /** 1: file sys; 2: eeprom */
	void* pParam;   /** 1: ��ű���cookie���ļ�����·�����ļ���, char*; 2: e2prom addr, unsigned int */
	int nLen;		/* eeprom�Ļ�������ֶα�ʾeeprom�Ĵ�С; �ļ��Ļ�����ʾ���������ļ���С >0 */
}CSGTLocalCacheCfg_S;

typedef  enum  _CSGTMsgType_E
{
	BRW_EVENT_TYPE_KEYBOARD  	= 0xB001, /**< �����¼�*/
	BRW_EVENT_TYPE_MOUSE     	= 0xB002, /**< ����¼�*/
	/**@brief mmcp ר����Ϣ����ͨ�� */
	BRW_EVENT_TYPE_SYSTEM			= 0xB003, /**< ϵͳ�����¼�*/
	BRW_EVENT_TYPE_PAINT			= 0xB004,  /**< �����л���֪ͨ��������Ƶ���Ϣ*/
	BRW_EVENT_TYPE_JSMSG			= 0xB005,	  /** JSMsg���� */
	BRW_EVENT_TYPE_JS_APP_MSG            = 0xB006      /**����ϵͳ�ڲ�����Ϣ������Ƶ�ɹ�����̨��ϡ������ź��жϵ�*/
}CSGTMsgType_E;

/**@brief 
Browser ���ܼ���Ϣ 
���������֧�ֿ�ݼ�����
��: ǰ��, ���� ,ˢ��,ֹͣ����,������ҳ
�����ʷ��¼,���»���ҳ���
Ϊ��Ӧ�ò���÷���,Ӧ�ò�ֻ�����
�ӿ�,����Ҫ����¼�����

�����ö��Ӧ�ò㲻��Ҫ���
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
/**@brief app send msg_repaint to Coship Browser, Ӧ��֪ͨ��������»��ƴ��ڣ�����Ҫ������������*/
	BRW_SYSTEM_TYPE_REPAINT 	=	0x04, 
/**@brief app send msg_clearHistory to Coship Browser, �����ʷ��¼ */
	BRW_SYSTEM_TYPE_CLEARHISTORY	=	0x05,
/**@brief app send msg_setHomePage to Coship Browser ,������ҳ�湦�� */
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
	GT_BUTTON_ENTER = 1,  	/**<���������OK �¼�>*/
	GT_BUTTON_CANCEL,		/**<���������CANCEL �¼�>*/
	GT_BUTTON_TIMEOUT	/**<���������ʱ �¼�>*/
}CSGTMsgActionType_E;

typedef enum _CSGTMsgBoxType_E
{
	GT_BUTTON_NULL 		= 0,		/**<��������������ͣ���button>*/
	GT_BUTTON_OK 			= 1,		/**<��������������ͣ�ok button>*/
	GT_BUTTON_OKCANCEL	= 2		/**<��������������ͣ�Cancel button>*/
}CSGTMsgBoxType_E;


typedef enum _CSGTMsgBoxAlign_E
{
	GT_MSG_ALIGN_LEFT		= 0,    /**<������������������>*/
	GT_MSG_ALIGN_CENTER	= 1,
	GT_MSG_ALIGN_RIGHT	= 2
}CSGTMsgBoxAlign_E;

// �ص�fnBrowserfun ����ΪEM_BRW_SYSTEM_EXIT�� dwParm1����ȡֵ
typedef enum _CSGTExitType_E
{
	EM_GT_EXIT_NORMAL_TYPE	= 0,    //������Ѿ��˳������ڴ򿪺�����ҳ״̬
	EM_GT_EXIT_PREVENT_TYPE	= 1,    //�����û���˳���Ҳ���򿪺�����ҳ������ҳ��ֹ�˳�
	EM_GT_EXIT_NO_PROCESS_TYPE	= 2  // �����û���˳���Ҳû�б���ҳ��ֹ�˳�, ����EM_GT_OPT_BRW_QUIT_FLAGѡ��Ϊ0����������˳������ظ�����
}CSGTExitType_E;

typedef enum _GTUserRenderAndLayout_E 
{
	EM_GT_URAL_WIDTH_CONTAIN_BORDER = 1, // CSS  .tdx {width: 80px; border-top: 2px solid #FFFFFF} ;width contain the border ,������Ŀ
	EM_GT_URAL_USE_CALC_TABLE_HEIGHT, // ��tableָ���߶ȴ��ڼ���߶�ʱʹ�ü���߶ȣ�eg: <table height="60"><tr><td height="50"></td></tr></table> then table height is 50 ������Ŀ
	EM_GT_URAL_ABSOLUTEPOSITION_NESTING, // Ƕ�װ������Զ�λʱ������iPanel�Կ�ʼ����Ϊ������λ��ʼ�㣬<div id="Layer6" style="position:absolute; left:207px; top:112px; width:430px; height:367px; z-index:6"><span style="position:absolute; left:5px; top:18px; width:200px; height:150px; z-index:7"></span>
												// </div> span������div��Ϊ��ʼ�㣬�������
	EM_GT_URAL_GB2312TOGBK,	// meta�еı��뷽ʽǿ����GB2312תΪGBK ��������
	EM_GT_URAL_CALC_TABLE_WIDTH, // ���ݻ���table����㷨, ̨�ݻ���ʹ��
	EM_GT_URAL_IFRAME_PERCENTHEIGHT,       //  iframe�ĸ߶�Ϊ�ٷֱ�ʱ����Ը߶�������������Ϊ����iframe�ڵ�Ĵ��ڸ߶� ������Ŀ	
	EM_GT_JSC_UNINIT_VALUE,   //�Ƿ�֧��δ��ʼ���ı������ã����ø�ö�پ�ֱ�ӷ���NULL
	EM_GT_JSC_CHECK_SCRIPT_STRING, //�Ƿ���Ҫ���script string���﷨��Ŀǰ�Ǽ��������Ƿ�ƥ��
	EM_GT_URAL_IGNORE_LINEHEIGHT,   //�Ƿ�֧��CSS�е�LineHeight����	������ʡ��
	EM_GT_URAL_DISABLE_JSKEYDOWNEVENT,  //�Ƿ�֧��jskeydown�¼�,������Ŀ
	EM_GT_URAL_FOCUSON_SUB_HREF,		//�����Ƿ��ڳ������ӵ��ӽڵ��ϣ�����ʡ��/����ϵ��
	EM_GT_URAL_ENABLE_PRELOAD,          	// �Ƿ�֧����ҳԤȡ����
	EM_GT_CACHE_JS_SYNTAX_TREE,		// �Ƿ�֧�ֻ���js �﷨��
	EM_GT_MIDP_AUTO_CENTER_DISPLAY,		// Ĭ������midp���������ʾ
	EM_GT_URAL_JS_CALC_TABLE_HEIGHT  // �Ƿ�֧�ֽ���ʡ�����߶ȼ��㣬 ����ʡ��, true �ǽ���ʡ�����⴦��Ĭ��Ϊfalse
}GTUserRenderAndLayout_E;

typedef enum _CSGTOption_E
{
	EM_GT_OPT_CABLEMODEM 			= 1 ,  // ����CM״̬��pvValueֵ1��cable modem��0��cable modem
	EM_GT_OPT_VOLUMECONTROL 		= 2,	 // �����Ƿ����������Ĭ����������״̬��pvValue is int.  0: control��1: no control
	EM_GT_OPT_SD_MUTE_POSITION_X 	= 3,  // ���ñ��徲��ͼ��xλ��,   pvValue is int. 
	EM_GT_OPT_SD_MUTE_POSITION_Y 	= 4,	 // ���ñ��徲��ͼ��yλ�ã�pvValue is int. 
	EM_GT_OPT_SD_VOLUME_POSITION_X = 5,	 // ���ñ�������ͼ��x λ�ã�pvValue is int.
	EM_GT_OPT_SD_VOLUME_POSITION_Y = 6,	 // ���ñ�������ͼ��yλ��.   pvValue is int.
	EM_GT_OPT_HD_MUTE_POSITION_X 	= 7,	 // ���ø��徲��ͼ��xλ��.�ppvValue is int.
	EM_GT_OPT_HD_MUTE_POSITION_Y 	= 8,	 // ���ø��徲��ͼ��yλ��.  pvValue is int.
	EM_GT_OPT_HD_VOLUME_POSITION_X = 9,	 // ���ø�������ͼ��xλ��.  pvValue is int. 
	EM_GT_OPT_HD_VOLUME_POSITION_Y = 10, // ���ø�������ͼ��yλ��. pvValue is int.
	EM_GT_OPT_EXIT_TO_HOMEPAGE		= 11,  // ������������˳������˳�homePage, pvValue �� CSGTOptionExitToHomePage_S �ṹ��ַ
	EM_GT_OPT_OC_PROGRESS_TIMEOUT = 12,  // ����DVB��Դ���س�ʱʱ�䣬����ﵽ����ʱ�䲢���Ѿ�������3/4���򲻿����������ͼ����, pvValue is int.��λms 
	EM_GT_OPT_RESOLUTION_CHANGE_SUPPORT = 13, //�����Ƿ�֧�ֱַ����л���pvValue is int,  0:no support ,1:support(default value)
	EM_GT_OPT_PROGRESS_TIME_DELAY	       	= 14,   // �����������ҳ����ӳ�ʱ��
	EM_GT_OPT_ALWAYS_CACHE_JS_SYNTAX_TREE = 15,   //  ����������Ƿ����û����﷨����pvValue is int 0:needn't cache 1: cache js syntax tree
	EM_GT_OPT_ALWAYS_CACHE_IMAGE = 16,  //  ����������Ƿ����û���ĳЩͼƬ pvValue is int 0:needn't cache 1: cache image
	EM_GT_OPT_ALLOWED_ALWAYS_CACHE_JS_SYNTAXTREE_NUM = 17,   //  ������������û����﷨��������pvValue is int  always cached js syntax tree numble default is 3
	EM_GT_OPT_ALLOWED_ALWAYS_CACHE_IMAGE_NUM = 18,  //  ������������û���ͼƬ������pvValue is int ,  always cached image numble default is 2
	EM_GT_OPT_HTMLPARSE_FORCE_USE_HTML4COMPAT = 19,  //  ���������HTML�����Ƿ�ǿ��ʹ��Html4 Compat���ͣ�pvValue is int ,  1 ʹ�ã�Ĭ�ϲ�ʹ����ҳ�����
	EM_GT_OPT_SET_MIX_MEMORY_MODE = 20,  //  ���������ʹ�û���ڴ�ģʽ��ֻ�����������ʼ��֮ǰ����, pvValue Ϊ CSGTMixMemory_S����ڴ���Ϣ��ַ
	EM_GT_OPT_HOMEPAGE			= 21,    //  �����������ݼ�������ҳ, pvValue �� CSGTOptionExitToHomePage_S �ṹ��ַ
	EM_GT_OPT_READDATA_TIMEOUT_MS = 22,   // ��������������ȡ���ݳ�ʱʱ�� ��λms,ȫ����Ч��pvValue is unsigned int
	EM_GT_OPT_MIN_VOLUME          = 23,    //�����������С����ֵ,Ŀǰ��֧����СֵΪ0
	EM_GT_OPT_MAX_VOLUME          = 24,    //����������������ֵ
	EM_GT_OPT_FAIL_ON_ERROR       = 25,    //  ����������Ƿ�״̬��>400����Ӧ����������
	EM_GT_OPT_ENABLE_LOCAL_COOKIE = 26,	//���������֧�ֱ���cookie,pvValueֵΪ��CSGTLocalCacheCfg_S*���ͣ�
	EM_GT_OPT_UTCOFFSET           = 27,     //���������ʱ��, ʱ��Ϊdouble���ͣ�pvValue Ϊ����ʱ�������ĵ�ַ, Ĭ��ʱ���� 8.0
	EM_GT_OPT_DSTOFFSET           = 28, 	//��������ʱ����  pvValue is int 0:no support DSTOFFSET ,1: support DSTOFFSET,default pvValue is 0
	EM_GT_OPT_SD_RESOLUTION_WIDTH = 29,	//�������������ֱ��ʵĿ�ȣ�pvValue is int��Ĭ��720�������ã�����CSGTWebInit֮ǰ���ã���CSGTWebInit��ʼ�����Ǳ���ķֱ��ʣ�Ҫ��EM_GT_OPT_SD_RESOLUTION_WIDTH>=g_arglist.nScreenWidth(g_arglist��CSGTWebInit�Ĳ���������ΪCSBrwArgList_S)
	EM_GT_OPT_SD_RESOLUTION_HEIGHT = 30,	//�������������ֱ��ʵĸ߶ȣ�pvValue is int��Ĭ��576�������ã�����CSGTWebInit֮ǰ���ã���CSGTWebInit��ʼ�����Ǳ���ķֱ��ʣ�Ҫ��EM_GT_OPT_SD_RESOLUTION_HEIGHT>=g_arglist.nScreenHeight(g_arglist��CSGTWebInit�Ĳ���������ΪCSBrwArgList_S)
	EM_GT_OPT_INNERTEXTTOINNERHTML = 31,    //�����Ƿ�ʹ��ElementInnerTextת����ElementInnerHTML pvValue is int, 0:innerText��ת����innerHtml ,1: innerTextת����innerHtml,default pvValue is 0
	EM_GT_OPT_SUPPORT_JSEXCEPTION = 32,   //�������������JS�Ƿ��׳��쳣��pvValueֵTRUE�׳��쳣��FALSE���׳��쳣
	EM_GT_OPT_MSGBOX_KEYCODE = 33,		 // ���õ�����ʾ��ļ�ֵ pvValue is int, keypress��������͵ļ�ֵ�����ü�ֵһ�£��������ʾ��ӿ�
	EM_GT_OPT_METACHARSET_FIRST_HTTPCHARSET = 34, // ���������charset���ȼ�, pvValue is int , ��Ϊ1����ҳmeta Charset����HTTP Charset,Ĭ��HTTP Charset������ҳmeta Charset
	EM_GT_OPT_INPUTBOX_SIZE = 35,      // ���������Ĵ�С,pvValue is int , Ĭ��������СΪ10��Ӧ�ÿ��Ը�����Ҫ�÷������������Ĵ�С
	EM_GT_OPT_USE_IECompat = 36,    // �����Ƿ�ʹ��IEģʽ��pvValue is bool ,Ĭ��Ϊfalse����Ӧ������Ϊtureʱ����ʾΪIEģʽ��JS getYear��ȡ��ݼ�������1900
	EM_GT_OPT_BRW_MUTE_LOGO = 37,       //�����Ƿ���ʾ�����Ĭ�Ͼ���ͼ�꣬pvValue is int, 0Ϊ����ʾ��1Ϊ��ʾ��Ĭ��Ϊ1
	EM_GT_OPT_ISNBSPEQUALTOSPACE = 38,   // ����&nbsp�Ƿ��ÿո��ʾ���ո�Ŀ�ȱ�&nbsp�Ŀ�2px���ң�pvValue is int, pvValueΪ1��ʾʹ�ÿո��ʾ&nbsp, Ĭ��Ϊ0, 0��ʾ&nbsp���ÿո��ʾ
	EM_GT_OPT_BRW_QUIT_FLAG = 39,         // ������ҳ����ֹҲû�������˳���homepage��������£��Ƿ��˳�������ı�־����pvValue is int, ȡֵ��0�����˳��������1���˳��������Ĭ�ϣ�1 
	EM_GT_OPT_BRW_DEFAULT_IME = 40,         // ���������Ĭ�����뷨��pvValue is int, ȡֵ��0�����ģ�1�������ַ���2. Ӣ����ĸ3.����� Ĭ�ϣ�0
	EM_GT_OPT_PROGRESS_INFO = 41,       // ����ע���������������Ϣ��pvValue�ǽṹ��CSGTProgressInfo_S��ַ
	EM_GT_OPT_PROGRESS_REPEAT_TIME = 42,     // ������תȦ�ٶȣ�pvValueΪdouble���͵ı�����ַ��Ĭ��ֵ������СֵΪ0.1(��������0.1 0.2 ... 1.0)����ֵԽ��תȦԽ����
	EM_GT_OPT_PROGRESS_POS = 43, // ��������ʾλ�ã�pvValue�ǽṹ��CSGTProgressPos_S��ַ 
	EM_GT_OPT_BRW_KEY_EVENT_TYPE = 44,        // ���ð����¼� event.type��ȡֵ�� Ĭ��Ϊ-1��pvValue is int, ȡֵ -1�� event.type���ձ�׼���巵�ظ���ҳ�� ȡֵ ����0����������ֵ���ظ���ҳ
	EM_GT_OPT_CACHE_MAXAGE = 45,           // ���÷�����û�����������cacheʧЧʱ��ʱĬ��ֵ�� pvValue��double ָ�룬 ��λΪ�룬��������10*60����10����,Ĭ��ֵΪ20����
	EM_GT_OPT_CONNECT_FORBID_REUSE = 46,           // ����HTTP �����Ƿ��ֹ���ã�pvValue is int, ȡֵ��0��������, 1 ��ֹ���ã�Ĭ��Ϊ������
	EM_GT_OPT_ISUSEPIPELINE = 47, 	//�����Ƿ�ʹ��pipeline�ķ�ʽ������Դ��0Ϊ��ʹ�ã�1Ϊʹ�ã�Ĭ�ϲ�ʹ��
	EM_GT_OPT_CSS_PRIORITY = 48,     // �����û��Զ���css�������ȼ���Ĭ��false��ʾ�����洦��ture��ʾcheck����û��Զ������ȼ�
	EM_GT_OPT_TABLE_BACKGROUND_REPEAT = 49, // ����table��ǩ�ı���ͼƬƽ�̷�ʽ��Ĭ��Ϊfalse��ʾƽ�̣�ture��ʾ��ƽ�̣���������ͼƬλ��Ϊ����
	EM_GT_OPT_HUNAN = 50,     // ���ú�����ĿһЩ����ҳ�洦��������Ĭ����false��ʾ�رա���Ҫ��td����img��Ŵ�table;���ϸ�ģʽ������table�����padding��border;������ˢ������;
	EM_GT_OPT_USE_STATIC_MEM = 51,     // ���������ʹ���ⲿ���õľ�̬�ڴ� , pvValue is int�� Ĭ��Ϊ0 ��ʹ���ⲿ�ڴ棬1Ϊʹ���ⲿ�ڴ�
	EM_GT_OPT_STATIC_MEM_ADDR  = 52,     // ���������ʹ���ⲿ���õľ�̬�ڴ�ĵ�ַ��pvValue is void *, ��ַ�ڴ��׵�ַ��Ĭ��Ϊ��
	EM_GT_OPT_STATIC_MEM_SIZE = 53,   // ���������ʹ���ⲿ���õľ�̬�ڴ�Ĵ�С��pvValue is int, �ⲿ�ڴ��ַ�ռ䳤�ȣ�Ĭ��Ϊ0
	EM_GT_OPT_SHOW_TRANSPRENT_BMP = 54,   // ����͸����BMPͼƬ�Ƿ���ʾ��pvValue is int, 0Ϊ����ʾ��1Ϊ��ʾ��Ĭ��Ϊ0
	EM_GT_OPT_RETURN0_STOP_DEFAULT = 55,		// ����return 0ʱ�Ƿ���ֹ�����Ĭ�ϴ���,pvValue is int, 1��ʾĬ����ֹ�˳�����0��ʾ����ֹ��Ĭ��Ϊ1��
	EM_GT_OPT_SHOW_DEFAULT_IMAGE = 56,	// ���õ�ͼƬû���سɹ�ʱ���Ƿ���ʾ��ʾĬ��ͼƬ��pvValue is int, 1��ʾ��ʾ��0��ʾ����ʾ��Ĭ��Ϊ1��
	EM_GT_OPT_URL_DONOT_ALWAYS_UTF8_ENCODER = 57,  // ����URL������ʹ��UTF-8���ͣ�����ҳ��ָ�����뷽ʽ����URL���� pvValue is int, ȡֵ��0������ʹ��UTF-8����, 1 ������ʹ��UTF-8���룬Ĭ��Ϊ0
	EM_GT_OPT_PAINT_AFTER_ALLCOMPLETED = 58, // �����Ƿ����е�frame����������ʾҳ�棬pvValue int ,0Ϊ����frame��������ʾ��1Ϊ����frame����������ʾ��Ĭ��Ϊ0(Ŀǰ��Ϊ����ʹ��)
	EM_GT_OPT_IMAGE_SCALE_SIZE = 59,       // ����ͼƬ�������ŵĿ�ߣ�pvValue�ǽṹ��CSGTImgScalSize_S��ַ, Ĭ����ͼƬԭʼ��С
	EM_GT_OPT_CUSTOMIZE_INPUTKEY = 60,      //  �����û��Զ������뷨����������pvValue is int,��ʾ����İ�����ֵ��Ĭ��Ϊ-1
	EM_GT_OPT_DISABLE_SETDRAWFOCUSRING = 61    //����ҳ��setDrawFocusRing(0)����д����Ч��pvValue is int, 1��ʾ��Ч��0��ʾ��Ч��Ĭ��Ϊ0
}CSGTOption_E;

/**
�������������
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
@ʹ�ô˽ӿڣ�������������ܰ����˳��Ƿ���������ҳ�Ĺ���
@���� 	:  ��������˳�������������homepage ,����ҳ�˳������
@������	:  ��������˳�
*/
typedef struct _CSGTOptionExitToHomePage_S
{
	int		 nKeyCode;				// ���ô�����ѡ��ܵİ�����ֵ
	char	 	*pcUrl;					 // ���ô�����ѡ��ܵ�url
	int		 nLen;					// ���������������ҳurl�ĳ���
}CSGTOptionExitToHomePage_S; 

/**
@brief ��������������Ի���Ļص�
@param[in] int nId �����������ID, e_ActionType eType ������Ķ���, void *pUserData ����չ����
*/
typedef void (*MsgBoxActionCallBack_F)(CSGTMsgActionType_E enType, void *pvData, void *pvUserData); 

/**
@brief ��ȡ����ϵͳ�ڲ�����Ϣ������Ƶ�ɹ�����̨��ϡ������ź��жϵȣ��Ļص�����
��ϵͳ��Ϣ��Ҫ3������ type which modifiers
1�����modifiers��Ϊ�գ������·�����Ϣ
CSBRWEvent eEvent;
eEvent.m_dwPara0 = nId; //��idΨһ��־һ����Ϣ�����ݴ�id�ӻص������ȡ��Ϣ����which modifiers��ֵ
eEvent.m_dwPara1 = BRW_EVENT_TYPE_JS_APP_MSG; //����Ϣ����
eEvent.m_dwPara2 = 2; //��Ϣ���� type��ֵ
eEvent.m_dwPara3 = (DWORD)(JsMsgInfoCallBack); //�ص�����ָ��
eEvent.m_dwPara0 = 0;

2�����modifiersΪ�գ������·�����Ϣ
CSBRWEvent eEvent;
eEvent.m_dwPara0 = nWhich; //��Ϣ����which��ֵ
eEvent.m_dwPara1 = BRW_EVENT_TYPE_JS_APP_MSG; //����Ϣ����
eEvent.m_dwPara2 = 2; //��Ϣ���� type��ֵ
eEvent.m_dwPara3 = 0; //����Ҫ�ص�
eEvent.m_dwPara0 = 0;

@param[in]       nId           ��Ϣ��־
@param[out]      pnWhich       ��Ϣ����which��ֵ
@param[out]      pcModifiers   ��Ϣ����modifiers��ֵ���������ͱ���Ϊunicode
@param[int��out] pnModifiersLen   in: pcModifiers��ָ���ڴ���ֽڳ��� out����Ϣ����modifiers��unicode�ַ�����
@param           pvUserData    �û�����
*/
typedef void (*JsMsgInfoCallBack_F)(int nId, int *pnWhich, char *pcModifiers, int *pnModifiersLen, void *pvUserData); 

typedef struct _CSGTsMsgBoxConfig_S
{
	DWORD			dwMsgBoxId;    //�û���ʾ��id
	DWORD			dwTimeOut; //ms 0 ����ʱ
	char 			*pcBoxAption; //��ʾ���������
	char				*pcBoxContent;  //��ʾ������
	int				nBoxAptionLen;	//��ʾ�����ĳ���
	int 				nBoxContentLen; // ��ʾ�����ݵĳ���
	void 			*pUserdata;  //�û����ݣ���m_fnMsgBoxCB�д��ظ��û�
	CSBRWRect_S	sRect;		// Msg ��ʾλ��
	CSGTMsgBoxType_E 	emMsgType;   //type 0 - 2, button numble
	CSGTMsgBoxAlign_E    emAlign;		// ���뷽ʽ��left = 0; right = 1;center = 2;
	MsgBoxActionCallBack_F fnMsgBoxCallback;
	
}CSGTsMsgBoxConfig_S;


typedef struct _CSGTsMsgBoxInfo_S
{
	DWORD			dwMsgBoxId;    //�û���ʾ��id
	DWORD			dwTimeOut; //ms 0 ����ʱ
	char 			pcBoxAption[64]; //��ʾ���������
	char			pcBoxContent[256];  //��ʾ������
	int				nBoxAptionLen;	//��ʾ�����ĳ���
	int 			nBoxContentLen; // ��ʾ�����ݵĳ���
	void 			*pUserdata;  //�û����ݣ���m_fnMsgBoxCB�д��ظ��û�
	CSBRWRect_S		sRect;		// Msg ��ʾλ��
	CSGTMsgBoxType_E 	emMsgType;   //type 0 - 2, button numble
	CSGTMsgBoxAlign_E    emAlign;		// ���뷽ʽ��left = 0; right = 1;center = 2;
	MsgBoxActionCallBack_F fnMsgBoxCallback;

}CSGTsMsgBoxInfo_S;

/**@brief �¼��Ķ���,��keydown,keyup,keypress,
һ��������Ҫ������������,
��Ϊ�������Ҫ�����Ķ���  
*/
typedef  enum  _CSBRWKeyStatus_E
{
	BRW_EVENT_TYPE_KEY_DOWN	= 8,  /**< ���̰����¼�*/
	BRW_EVENT_TYPE_KEY_UP		= 9, /**< �����ɿ��¼�*/
	BRW_EVENT_TYPE_KEY_PRESS	= 10 /**< press �¼�,������Լ�������,�������Ҫ�����¼�����*/
} CSBRWKeyStatus_E;

/**@brief ���������Ӧ�õ���Ϣ����ΪEM_BRW_KEYBOARDEVENT_PROCESSʱ��
               ������׸��������������Ϣ�Ľṹ��
m_processType��m_keyStatus�μ�eBrwProcessType��CSBRWKeyStatus_E�Ķ���
��m_processTypeΪEM_BRW_INPUTBOXʱ��m_x��m_y����ֵ
*/
typedef struct _CSBRWEventProcess_S
{
	eBrwProcessType m_processType;    //�������������Ϣ������	
	CSBRWKeyStatus_E m_keyStatus;     //������¼��Ķ���	
	int m_keycode;                    //������ֵ
	int m_x;				          //������x����	
	int m_y;				          //���뿨��y����
}CSBRWEventProcess_S;

/*@brief �¼����Ͷ��壺
	1.BRW_EVENT_TYPE_KEYBOARD  	= 0xB001 	// �����¼�
	m_dwPara0:  keycode
	m_dwPara1:	CSGTMsgType_E����
	m_dwPara2:	��������
	m_dwPara3��	δʹ��
	m_dwPara4�� δʹ��
	
	2.BRW_EVENT_TYPE_MOUSE     	= 0xB002	// ����¼�
	m_dwPara0:  δʹ��
	m_dwPara1:	CSGTMsgType_E����
	m_dwPara2:	���������
	m_dwPara3��	x,yλ������
	m_dwPara4�� δʹ��
	
	3.BRW_EVENT_TYPE_SYSTEM			= 0xB003  // ϵͳ�����¼�
	m_dwPara0:  ������Ϣ ���ο�CSGTMsgSystemType_E��
	m_dwPara1:	CSGTMsgType_E����
	m_dwPara2:	��չ����
	m_dwPara3��	δʹ��
	m_dwPara4�� δʹ��
	
	4.BRW_EVENT_TYPE_PAINT			= 0xB004  // �����л���֪ͨ��������Ƶ���Ϣ
	m_dwPara0:  ������Ϣ ���ο�CSGTMsgSystemType_E��
	m_dwPara1:	CSGTMsgType_E����
	m_dwPara2:	δʹ��
	m_dwPara3��	x,yλ������
	m_dwPara4�� w,h����16λ����16λ��
	
	5.BRW_EVENT_TYPE_JSMSG		= 0xB005	// JSMSG 
	m_dwPara0:  keycode 
	m_dwPara1:	CSGTMsgType_E����
	m_dwPara2:	��������
	m_dwPara3��	��չ����
	m_dwPara4�� δʹ��
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
	BOOL		isBrwCreateWnd;   /**< �Ƿ����������������TRUE:  ����������� FALSE: Ӧ��ֻ��Ҫ����ǰ��������*/
	int    		nBufferSize;    		/**<�ⲿ������ڴ�صĴ�С*/
	int			nScreenWidth;		/**<��ʾ����ֱ���*/
	int			nScreenHeight;   	/**<��ʾ����ֱ���*/
	int			nCacheBufferSize;	/**<����������С>*/
	/** @brief
	��Ҫ�������������ʱ����Ҫ������Ĳ���*/
	int 			nX;			      	/**< ��ʼ�����ڵ�Xλ��(�����ʾ�豸)*/
	int 			nY;			      	 /**< ��ʼ�����ڵ�Yλ��(�����ʾ�豸) */
	int 			nWidth;           	 /**< ��ʼ�����ڵĿ�� */
	int 			nHeight;	      		 /**< ��ʼ�����ڵĸ߶� */

} CSBrwArgList_S;

//zmr add -s
typedef struct _CSGTFontMap  {	
	int nStartSize;    //����ӳ�����俪ʼλ��  	
	int nEndSize;	   //����ӳ���������λ��
	int nLibFont;	   //��ӳ�䵽������
}CSGTFontMap_S;
//zmr add -e

typedef struct _CSGTProgPicInfo_S
{
	int nPerFrameW; //������ͼƬÿһ֡��ȣ���ÿһ֡�Ŀ�ȶ�һ��ʱ��ֱ������һ��ȫ�ּ��ɣ�Ŀǰ�ݲ�֧��
	int nPerFrameH; //������ͼƬÿһ֡�߶ȣ���ÿһ֡�ĸ߶ȶ�һ��ʱ��ֱ������һ��ȫ�ּ��ɣ�Ŀǰ�ݲ�֧��
	int pnBufferSize; //������ÿ֡bufferSzie
	unsigned char*  pcBuffer; //������ÿ֡��buffer��ַ
}CSGTProgPicInfo_S;

typedef struct _CSGTProgRect_S
{
	int nWidth; //������ͼƬȫ�ֿ�,��������ÿһ֡��ȣ��������ȡ��ֵ
	int nHeight; //������ͼƬȫ�ָ�,��������ÿһ֡�߶ȣ��������ȡ��ֵ
}CSGTProgRect_S;

typedef struct _CSGTProgressPos_S
{
	int nSDX;  //����������Xλ��
	int nSDY;  //����������Yλ��
	int nHDX;  //����������Xλ��
	int nHDY;  //����������Yλ�� 
}CSGTProgressPos_S;

/**
@brief Ӧ��ע��Ļ�ȡ������ÿ֡��Ϣ�Ļص�����
@param[in] CSGTProgPicInfo_S ������ÿ֡��Ϣ
*/
typedef void (*fnGetProgIcon_F)(int nFrameIndex, CSGTProgPicInfo_S * pSProgressIcon, void * pvUserData);

typedef struct _CSGTProgressInfo_S{
	int nFrameCount;                // ������֡�� ��Ĭ��ֵΪ8�����ֵΪ12
	CSGTProgRect_S  SProgressPicRect;  // ��������ʾ��λ�ã���С 
	fnGetProgIcon_F  fnProgfun;    // Ӧ��ע��Ļ�ȡ������ÿ֡��Ϣ�Ļص�����
}CSGTProgressInfo_S;

/*url����*/
typedef struct _CSGTUrlList
{
	char * url;	
	struct _CSGTUrlList * next;
}CSGTUrlList_S;

/*ͼƬ���ųߴ�*/
typedef struct _CSGTImgScalSize_S
{
	int nWidth;	
	int nHeight;
}CSGTImgScalSize_S;

/**
@brief �������Ӧ�õĻص����������塣
@param[in] dwType:   ��Ϣ�����ͣ�Ŀǰ�����࣬OC , Network ,System
@param[in] dwParm1:  �������µ�������
@param[in] dwParm2�� �û��Զ������������ʹ�ýṹ��ָ�봫�ݲ�����
@param[in] userdata: 
����ֵ: ����dwType��dwParm1���з��أ�����Ϣ������ΪNetworkʱ��������Ҫ�򿪵Ĵ���ҳ�棬����ֱ�ӷ��ؿա�
������� dwType  ��EM_BRW_SYSTEM_EXIT ʱ,  dwParm1 Ϊ0����ʾ�˳������������ֵΪ�˳�����������ҳ���ƣ�Ӧ�ò���Ҫ����
*/
typedef  char *(*fnBrowserfun)(eBrwMsgType dwType, int dwParm1, void* dwParm2 , void * pvUserData);

/**
@brief Э��ص����塣
1��Ӧ�ÿ���ͨ��EM_GT_OPT_MSGBOX_KEYCODE�Լ����ð����˳���������·��͸����������ϢCSBRWEvent�е�m_dwPara3Ϊ�գ�m_dwPara4�����û�Id
���m_dwPara4����Ϊ0����EM_GT_OPT_MSGBOX_KEYCODE���õļ�ֵΪ�û�id���ݸ�dwParm1��
2������Ӧ��Ҳ����ֱ�ӷ���Ϣ����BRW_SYSTEM_TYPE_SHOW_MSGBOX����ʵ�ֵ���������CSBRWEvent�е�m_dwPara3Ϊ�գ�m_dwPara4Ϊ�û�Id�������ͨ��
CSOnGTGetMsgBoxInfo_F�����ص���ȡ��ʾ����Ϣ������dwParm1����ΪCSBRWEvent�ص�m_dwPara4�û�id��

@param[in] dwParm1 : ���û���ʾ��ID��
@param[in] dwParm2 : ��ȡCSGTsMsgBoxInfo_S�ṹ������ݡ�
@param[in] pvUserData :�û�����
@return void��
@note �ޡ�
*/
typedef void(*CSOnGTGetMsgBoxInfo_F)(int dwParm1, void* dwParm2 , void * pvUserData);

/**
@brief ҳ����Ӧ�갴�����ٽ���ֵ����APP ����
@param[in] nKeyCode:  ����APP ����ļ�ֵ
@param[in] dwParm2:   δ����
@param[in] userdata:  �û����ݣ�ע��ʱ�趨
����ֵ: void
*/
typedef void(*fnAppEventProcessFun)(unsigned int nKeyCode, void* dwParm2 , void * pvUserData);

/**
@brief Э��ص����塣

@param[in] url Э��ͷ eg : " rtsp://" ʵʱ��Э�飬 �����û���
@param[in] nUrlLen unicode �����url��ռ���ڴ��ֽڳ��ȡ�
@return �޷���ֵ��
@note �ޡ�
*/
typedef void* (* UserProtocolCallBack)(char * pcUrl, int nUrlLen);


/**
@brief �˳�������ص�
@param[in] enType �ص�����: �Ƿ��˳������
*/

typedef void* (* UserDialogCallBack)(CSGTRet_E enType);

/**
@brief ����ص�������ָ�롣

@param[in] char ��ʾ��Ϣ��
@return �޷���ֵ��
@note �ṩ��GUITOOL �Զ�������ʹ�á�
*/
typedef void (*fnNextURLCALLBACK)(const char * pcInfo); // �ص�����ָ��


/**
@brief ��ʼ��GUITOOL��

@param[in] psBrwArgList  ��ʼ������ָ�롣
@return �ɹ�ʱ����GT_RET_SUCCESS,ʧ�ܷ���GT_RET_FAILURE��
@note int    		       nBufferSize;    	�ⲿ������ڴ�صĴ�С��
@note int				nScreenWidth;			��ʾ����ֱ��ʡ�
@note int				nScreenHeight;   	��ʾ����ֱ��ʣ���Ҫ�������������ʱ����Ҫ������Ĳ�����
@note int 			nX;			      		��ʼ�����ڵ�Xλ��(�����ʾ�豸)��
@note int 			nY;			      	  ��ʼ�����ڵ�Yλ��(�����ʾ�豸) 
@note int 			nWidth;           ��ʼ�����ڵĿ�ȡ�
@note int 			nHeight;	      	��ʼ�����ڵĸ߶� ��
@note BOOL			isBrwCreateWnd;   	�Ƿ���������������ڣ�TRUE:  ����������� FALSE: Ӧ��ֻ��Ҫ����ǰ����������
*/
WEBKIT_API CSGTRet_E  CSGTWebInit(CSBrwArgList_S *psBrwArgList);

/**
@brief  ��ȡ�������ǰurl
@note char*			pcCurUrl: �������������������ǰ���ʵ�url��
@note int			nLen:  ���������pcCurUrl�ĳ��ȣ�
@return     SUCCESS :    GT_RET_SUCCESS;    FAILURE :    GT_RET_FALIURE; 
@note �ޡ�

*/
WEBKIT_API CSGTRet_E  CSGTGetCurrentUrl(char *pcCurUrl,int nLen);
/**
@brief  �˳��ر�GUITOOL ,���ٴ���,�ͷ���Դ���ú����������������  ��ʼ��֮����ò���Ч���������������������������ڴ棬
                                ��û���ͷţ���˺������÷���ʧ�ܡ�

@return     SUCCESS :    GT_RET_SUCCESS;    FAILURE :    GT_RET_MEMORY_LOCK; //����Ĺ����ڴ�û���ͷš�
@note �ޡ�

*/
WEBKIT_API CSGTRet_E  CSGTWebDestroy() ;


/**
@brief Ӧ��������������¼���Ϣ��

@param[in] sEvent ������Ϣ��
@return �ɹ�ʱ����GT_RET_SUCCESS,ʧ�ܷ���GT_RET_FAILURE��
@note �ο�����:
@note ������Ϣ:
@note CSBRWEvent sEvent;
@note sEvent.m_dwPara0 = nKeyCode; // ��ֵ
@note sEvent.m_dwPara1 = nEventType; // ��������
@note sEvent.m_dwPara2 = nEventAction; // ��������
@note sEvent.m_dwPara3 = 0;
@note sEvent.m_dwPara4 = 0;
@note CSGTMsgProc(sEvent);

@note ������Ϣ:
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
@brief ��̬�ı�������Ĵ���,ֻ����Ӧ�ô�����������ڵ�ʱ����Ҫʹ��(��hyperText ��ʹ��),������ʱ����Ҫ.
@param[in] GR_RECT rect ��ȡ��ǰ������Ĵ��ڷֱ��ʡ�
@return �ɹ�ʱ����GT_RET_SUCCESS,ʧ�ܷ���GT_RET_FAILURE��
@note! the type of rect is GR_RECT ,we use void * to decouple
*/
WEBKIT_API CSGTRet_E CSGTCurrentWinSize(CSBRWRect_S *psRect);    

/**
@brief ��̬�ı�������Ĵ���,ֻ����Ӧ�ó�ʼ������Ҫ���ñ�����ҳ��Ҫ�Ĵ��ڴ�С
@param[in] int nSDWidth, int nSDHeight Ӧ�����õĴ��ڴ�С��
@return �ɹ�ʱ����GT_RET_SUCCESS,ʧ�ܷ���GT_RET_FAILURE��
@note! the type of rect is GR_RECT ,we use void * to decouple
*/
WEBKIT_API CSGTRet_E CSGTSetSDDefaultWinSize(int nSDWidth, int nSDHeight);


/**
@param[in] hClient ������ķֱ��ʾ����
@param[in] hWindow ������Ĵ��ڡ�
@param[in]  int nScreenWidth ��ʼ���ֱ��ʵĿ�
@param[in]  int nScreenHeight ��ʼ���ֱ��ʵĸ�
@return �ɹ�ʱ����GT_RET_SUCCESS,ʧ�ܷ���GT_RET_FAILURE��
*/
WEBKIT_API CSGTRet_E  CSGTChangeWin(HCSHANDLE hClient, HCSHANDLE hWindow, int nScreenWidth, int nScreenHeight); 

/**
@brief �ṩ�Զ�������ʹ�õĻص�����һ����ַ������

@param[in] msg ��ʾ��Ϣ��
@param[in] callback �ص�����ָ�롣
@return �޷���ֵ��
@note �ο�:
@note void* testCSGTRegisterCallback()
@note {
@note      CSGTRegisterCallback( "GuiTool Test!", openNextUrl );
@note  }
*/
WEBKIT_API void CSGTRegisterCallback( const char* pcMsg, fnNextURLCALLBACK fnCallback );

/**
@brief �������������ʡ�ڴ�״̬���ͷ���ҳ����Դ��
                                �����������ʼ��ʱ�����һЩ��Դ���ú�����������
                                ������Ѿ���ʼ��֮����ò���Ч��

@return     SUCCESS :    GT_RET_SUCCESS;    FAILURE :    GT_RET_STATE_ERR��
@note �ޡ�
*/
WEBKIT_API CSGTRet_E CSGTReleaseWebContent();

/**
@brief �ṩ���������homePage �Ĺ���
								�ú�����������
                                ������Ѿ���ʼ��֮����ò���Ч��
                                
@param[in] char* pUrl:     ����Ϊ��ҳ��url,�ַ�������С�� 1024byte
@param[in] int nKeyCode: ���ÿ�ݰ���(��ҳ�İ���)��ֵ,�벻Ҫ���������κΰ�����ֵ��ͬ

@return     SUCCESS :    GT_RET_SUCCESS;    FAILURE :    GT_RET_STATE_ERR��
@note �ޡ�
*/
WEBKIT_API CSGTRet_E CSGTSetHomePage(char *pcUrl, int nKeyCode);


/**
@brief ��ȡ����������Ϣ��

@param[in] pBrwInfo ��ȡ�����������Ϣ��
@return     SUCCESS :     GT_RET_SUCCESS;    FAILURE :     GT_RET_FAILURE��
@note �ޡ�
*/
CSGTRet_E CSGTGetBrwInfo(CSGTBrwInfo_S * psBrwInfo);


/**
@brief ����guitool�е��ڴ档
@param[in] nSize ������ڴ��С����λΪ�ֽڡ�
@return SUCCESS������һָ�롣    FAILURE: NULL  ʧ��ԭ���ж���(1)�����״̬��ΪGT_STATE_BACK��GT_STATE_STANDBY
			          (2) ������ڴ治�㣬���ܷ���������nSize�ڴ��С��
@note �ú���ֻ���������״̬ΪGT_STATE_BACK��GT_STATE_STANDBYʱ���ò���Ч��
@note ���������������ǰ̨֮ǰ�ͷŴ�������ڴ棬
@note ����������ܿ��������ڴ治������²���������ʾ��
*/
void *CSGTMalloc(int nSize);



/**
@brief �ͷŴ�guitool�з�����ڴ档

@param[in] ptr ָ����CSGTMalloc�����ص��ڴ�ָ�롣
@return  SUCCESS :     GT_RET_SUCCESS;    FAILURE :     GT_RET_STATE_ERR  �����״̬��ΪGT_STATE_BACK��GT_STATE_STANDBY��
@note �ú���ֻ���������״̬ΪGT_STATE_BACK��GT_STATE_STANDBY  ʱ���ò���Ч��
*/
CSGTRet_E CSGTFree(void * pvPtr);


/**
@brief ע�� �û�Э��ص���

@param[in] pUserProtocolHead  ע���Э��ͷ ��
@param[in] userCallBack ע��Ļص����� ��
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE��
@note �ޡ�
*/
WEBKIT_API CSGTRet_E CSGTSetUserProtocol(const char *pUserProtocolHead, 
                                                        UserProtocolCallBack fnUserCallBack);



/**
@brief �û��Ƿ�GTOOL���ڵ�ǰ���ڡ�

@param[in] bUsedGTool  TRUE,   �û��ǽ�GTOOL���ڵ�ǰ����FALSE�� �û��Ƿ�GTOOL���ء�
@return     Hidden :     GT_RET_SUCCESS;    Show   :     GT_RET_FAILURE;
@note �ޡ�
*/
WEBKIT_API CSGTRet_E  CSGTWindowVisible(BOOL  bUsedGTool);


/**
@brief ��һ����ҳ ��

@param[in] url  ���Է��ʵ���ҳ url ( ascall ��)��
@param[in] nURLLen url �ĳ��ȡ�
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE��
@note �ޡ�
*/
WEBKIT_API CSGTRet_E  CSGTOpenURL(char *pcUrl,int nUrlLen);


/**
@brief Ԥ���ش�һ����ҳ�������������Paint  ���� ��

@param[in] url  ���Է��ʵ���ҳ url ( ascall ��)��
@param[in] nURLLen url �ĳ��ȡ�
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE��
@note �ޡ�
*/
WEBKIT_API CSGTRet_E   CSGTPreLoadURL(char *pcUrl, int nLen);

/**
@brief ��һ��html �ı��ļ� ��

@param[in] pData html �ı��ļ�(  ascall ��)��
@param[in] nDataLengh �ļ���·�� �����ȡ�
@return �޷���ֵ��
@note �ޡ�
*/
WEBKIT_API CSGTRet_E CSGTOpenHtmlContent(char *pcData,int nDataLengh);

/**
@brief �ṩ������˳�ע�ᡣ

@param[in] statusmessages 		��ʾ״̬��Ϣ��
@param[in] contentmessages 	��ʾ״̬������Ϣ��
@param[in] DialagCallback		 ע��Ļص����� ��
@return �޷���ֵ��
*/
WEBKIT_API CSGTRet_E CSGTShowAlert(char* pcStatusMessages , char* pcContentMessages, UserDialogCallBack fnDialagCallback);

/**
@brief ��ȡ��ǰGTOOL �İ汾��

@param[out] pcVersion GUITOOL �İ汾�� ��

@param[in] nSize ����汾�Ŵ洢����        
@return SUCCESS : ���ذ汾�ų���;    FAILURE : ����Ĭ�ϳ��ȡ�
@note �ޡ�
*/
WEBKIT_API int CSGTGetVersion( char * pcVersion, int nSize );

/**
@brief �������ݡ�

@param[in] pucData ����Buffer��ָ�롣
@param[in] nLen ��������Buffer�ĳ��ȡ�
@param[in] szName ���ص����ݵ����֣�������׺��
@return �޷���ֵ��
@note �˺������ص�Buffer���û��Լ������������档��������᳤�ñ��ָ����ݡ�
*/
typedef void(*CSOnGTDownloadData_F)(char * pucData, int nLen, const char* szName);


/**
@brief �������ݡ�

@param[in] szUrl ���������ݵ�URL���ַ�����
@param[in] callback �û��Զ���ص�������
@return ����ɹ�����GT_RET_SUCCESS�����ʧ�ܷ���GT_RET_FAILURE��
@note �ޡ�
*/
WEBKIT_API CSGTRet_E CSGTDownLoad(const char* szUrl, CSOnGTDownloadData_F fnCallBack);


/**
@brief  ͨ�����������JavaӦ��

@param[in] JadURL JavaApp��Jad��URL ��
@param[in] JarURL JavaApp��Jar����URL ��
@param[in] pParams JaraApp������������ ���磨ServerIP��UserID �ȣ���������Ϊ������һ���ַ�����
	                 "ServerIP=17.30.19.102 UserID=901351 ..." , һ�Բ��������ÿո�ֿ�����=��ǰ�����пո�
@param[in] x JavaApp���ڵ�λ�ú����ꡣ
@param[in] y JavaApp���ڵ�λ�������ꡣ
@param[in] width JavaApp���ڵĿ�ȡ�
@param[in] height JavaApp���ڵĵĸ߶ȡ�
@return �ɹ�����0��ʧ�ܷ���-1 ��
@note �ޡ�
*/
WEBKIT_API int CSGTStartJavaApp(const char *pcJadURL, const char *pcJarURL, const char *pcParams, int nX, int nY, int nWidth, int nHeight);


/**
@brief  �õ�GT������JavaӦ�õ�״̬������js��չ�ĺ�Plugin��ʽ��Ӧ��

@param[in] ��
@return TRUE Ӧ��������,FALSE Ӧ��δ����
@note ��
*/
WEBKIT_API int CSGTGetJavaAppStatus();


/**
@brief  �˳�ͨ�������������Ӧ��(������JS��չ��Plugin��ʽ��Ӧ��)

@param[in] ��
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSGTExitJavaApp();

/**
@brief  ע���������Ӧ�õĻص�������

@param[in] fun      �������Ӧ�õĻص�������
@param[in] userdata �û�����
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSRegisterBrwCallback(fnBrowserfun fnFun, void *pvUserData);

/**
@brief  ע��Ӧ���е���Ļص�������

@param[in] fnFun : �������Ӧ�õĻص�������
@param[in] pvUserData:  �û�����
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSGTRegisterMsgBoxCallback(CSOnGTGetMsgBoxInfo_F fnFun, void *pvUserData);

/**
@brief  ���ü�������Ŀ����

@param[in] GTUserRenderAndLayout: ������Ŀ��������
@param[in] bSwitch: true ����false�أ�Ĭ�Ϲ�
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
@brief  �û���ֵ����ҳ��ֵת����������ҳʹ�õļ�ֵ��UDI����ļ�ֵ��һ�µ����

@param[in] pnKeyTableAddr ��ֵ��Ӧ��
@param[in] nFirstDimension ��һά���鳤��
@return ����ɹ�����GT_RET_SUCCESS�����ʧ�ܷ���GT_RET_FAILURE
@note ?
����:
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
@brief  �ṩ���û������ֿ�ӳ���

@param[in]   Font[]  ����ֿ���Ϣ��Ľṹ������
@param[in]   num �ṹ����������Ľṹ��ĸ���
@return SUCCESS :  0
@note ?
*/
WEBKIT_API void CSGTSetFontLib(CSGTFontMap_S saFont[], int nNum);

/**
@brief  ע��ҳ�洦�����󽫼�ֵ����APP ����ĺ���

@param[in] fun  APP ����ü�ֵ�Ľӿ�    
@param[in] userdata �û�����
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSRegisterAppEventProcessCallback(fnAppEventProcessFun fnAppEvtPrcsFun, void *pvUserData);

/**
@brief  ����2D Effect�����С
@param[in] nCacheMemorySize ����2D ����Ĵ�С
@return 
@note ?
*/
WEBKIT_API int CSJSEffectSetCacheSize(int nCacheMemorySize);

/**
@brief ioctlRead /ioctlWrite�ӿ�����

IoctlRead
    keyValue:			    
        "GTGetMaxVol"    ��ȡ�����������ʡ�ԣ�Ĭ��ֵ31
        "GTGetMinVol"     ��ȡ��С��������ʡ�ԣ�Ĭ��ֵ0
        "GTGetVolTimeOut"    ��ȡ��������ʱʱ�䣬��ʡ�ԣ���Ĭ��ֵ3000ms
        "GTGetVolColor"         ��ȡ���������ı���ɫ����ʡ�ԣ���Ĭ��ֵ0xFFFFFFFF
        "GTGetMuteState"     ��ȡ����״̬
        "GTGetVolume"          ��ȡ����ֵ

IoctlWrite
    keyValue: 
        "GTSetMute"        ���þ���״̬
        "GTSetVolume"    ��������ֵ
return value:
    return 0��ʾ��Դ����key�����ɹ���
    return -1��ʾ�����key�����Լ����ĵģ�û�в���
*/
typedef int (*CSGTPortingIoctlRead_F)(const char * pcKey, int nKeyLen, char * pcReadBuf, int nBufLen);
typedef int (*CSGTPortingIoctlWrite_F)(const char * pcKey, int nKeyLen, char * pcValue, int nValueLen);

/**
@brief ע��ioctl������Ŀǰ��shellע��
Ŀǰ���֧��ע��5��,ע�ⲻ֧��ע��Ϊ�գ�
fRead fWrite���붼��Ϊ�գ�������Ҫ�û�����ע��պ�����
return value:
    return 0 ��ʾע��ɹ�
    return -1��ʾע��ʧ��
*/
int CSGTRegisterIoctlFun(CSGTPortingIoctlRead_F fnRead, CSGTPortingIoctlWrite_F fnWrite);

/**
@brief io Read, Ŀǰ��jsextʹ��
return value:
    return 0��ʾ��Դ����key�����ɹ���
    return -1��ʾ�����keyû�д���
*/
int CSGTIoctlRead(const char * pcKey, int nKeyLen, char *pcReadBuf, int nBufLen);

/**
@brief io write, Ŀǰ��jsextʹ��
return value:
    return 0��ʾ��Դ����key�����ɹ���
    return -1��ʾ�����keyû�д���
*/
int CSGTIoctlWrite(const char * pcKey, int nKeyLen, char * pcValue, int nValueLen);

/**
@brief  �����Ƿ������������ʾҳ��

@param[in]   bAllowPaint  TRUEΪ�����������ʾҳ�棬FALSEΪ�������������ʾҳ��
@return 
@note Ĭ�������������ʾҳ��
*/
WEBKIT_API void CSGTAllowPaint(BOOL bAllowPaint);

/**
@brief set proxy, ���ô��������
proxy: �����������ַ,�64�ֽ�
proxyPort: ����������˿�
userName: ���ʴ�����������û������32�ֽڡ������û�������ʹ��NULL
password: ���ʴ�������������룬�32�ֽڡ��������룬��ʹ��NULL
proxyType���������������,Ĭ��CSGTPROXY_HTTP
typedef enum _CSGTProxyType{
	CSGTPROXY_HTTP = 0, 
	CSGTPROXY_SOCKS4 = 4, 
	CSGTPROXY_SOCKS5 = 5,
	CSGTPROXY_SOCKS4A = 6, 
	CSGTPROXY_SOCKS5_HOSTNAME = 7 
} CSGTProxyType_E;
return value:
    return TRUE ��ʾ�ɹ���
    return FALSE ��ʾû�д���
*/
WEBKIT_API BOOL CSGTSetProxy(const char * pcProxy, long lProxyPort, CSGTProxyType_E enmProxyType, const char * pcUserName, const char * pcPassword);

/**
@brief set proxy open or close, ������������أ���CSGTSetProxy��Ĭ����true����û��CSGTSetProxy����Ĭ����false��
bUseProxy: 
	TRUE: ʹ���Ѿ����õĴ��������������Դ
	FALSE: ��ʹ�ô��������������Դ
return value:
	return TRUE ��ʾ���óɹ�
	return FALSE ��ʾû�д���
*/
WEBKIT_API BOOL CSGTSetUseProxy(BOOL bUseProxy);



/**
@brief app����ǰҳ���url
@param[in] s_url:   ��ǰҳ���url����
����ֵ: void
*/
typedef void(*fnAppDealUrlListFun)(CSGTUrlList_S * s_url);

/**
@brief  ע��app��ȡ��ǰҳ��Ԫ�����е�url
@param[in] fun  APP ����CSGTUrlList_S�Ľӿ�    
@param[in] userdata �û�����
@return SUCCESS :  0
@note ?
*/
WEBKIT_API int CSRegisterGetUrlListCallback(fnAppDealUrlListFun dealTime, void *pvUserData);
#ifdef __cplusplus
}
#endif
#endif

/** @} */



