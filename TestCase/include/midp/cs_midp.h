  /******************************************************************************
 FileName:  cs_midp.h
 Copyright (c) 2008， coship。
 
 Version:       Ver 1.0
 Author:        ybq (yinbenqing@coship.com)
 
 File Description:
         Support MIDP Interface
 
 Modify History:
 Version     Date        Author          Description
 --------    ----------  --------        ------------
 1.0         2008-1203  ybq             Create
 
 *******************************************************************************/
 
#ifndef CSMIDP_H
#define CSMIDP_H
 
#ifdef __cplusplus
 extern "C" {
#endif
 
 
 /******************************************************************************
  *                                 Header File Include                         *
  ******************************************************************************/
#include "udiplus_typedef.h"
 
 /******************************************************************************
  *                                 Macro/Define/Structus                       *
  ******************************************************************************/
 /**
 * 定义MIDP退出的键值
 */
#define CSMIDPEXIT_KEYCODE 0xF1F2

 /**
 * 以下为midp 规范中定义的键
 */
typedef enum {
    MIDP_KEY_0        = '0',
    MIDP_KEY_1        = '1',
    MIDP_KEY_2        = '2',
    MIDP_KEY_3        = '3',
    MIDP_KEY_4        = '4',
    MIDP_KEY_5        = '5',
    MIDP_KEY_6        = '6',
    MIDP_KEY_7        = '7',
    MIDP_KEY_8        = '8',
    MIDP_KEY_9        = '9',
    MIDP_KEY_ASTERISK = '*',
    MIDP_KEY_POUND    = '#',

    MIDP_KEY_UP       = -1,
    MIDP_KEY_DOWN     = -2,
    MIDP_KEY_LEFT     = -3,
    MIDP_KEY_RIGHT    = -4,
    MIDP_KEY_SELECT   = -5, //对应手机上确认键

    MIDP_KEY_SOFT1    = -6, //对应手机上左软件
    MIDP_KEY_SOFT2    = -7, //对应手机上右软件
    MIDP_KEY_CLEAR    = -8, //对应手机上返回键

    MIDP_KEY_POWER    = -12,  //对应手机上挂机键，此键会导致midp 强制退出，慎用

    MIDP_KEY_INVALID  = 0 //无效键，用于取消注册某个按键
    
} CSMidpKeyType_E;

 /*
 以下为midp 支持的选项类型
 特别注意，以后扩展属性时在后面添加，不可改变原有属性
 -----------------------------------------------------------------------------------------------
EM_MIDP_KEY_MODE: 
    wParam 传传具体的值，
    EM_MIDP_KEYMODE_UDI-表示 让外部传给midp的键值 (也就是本地键值)
                                        直接透传给java层应用，而不用sun约定俗成的键值；极
                                        个别项目才会这样使用，如:topway；一般情况下，请不要调用此函数
    EM_MIDP_KEYMODE_DEFAULT-表示按默认处理
    lParam 保留不用
------------------------------------------------------------------------------------------------
EM_MIDP_FONT_SIZE:
    设置字体大小；
    wParam传CSMidpFontSize_E  中定义的值
    lParam传该字体对应的实际大小，当传0时恢复默认；
 -----------------------------------------------------------------------------------------------
EM_MIDP_STORAGE_HOME:
    设置MIDP永久性数据存储所需的根目录；
    wParam传入表示路径的字符串
    如:CSMidpSetOption(EM_MIDP_STORAGE_HOME, (void *)"/workdir/mnt/hd/HDD9/midpHome", NULL);
 -----------------------------------------------------------------------------------------------
 EM_MIDP_HEAP_SIZE:
    设置MIDP堆内存池大小；
    wParam堆内存池大小，单位字节
    如:CSMidpSetOption(EM_MIDP_HEAP_SIZE, (void *)0x200000, NULL);
 -----------------------------------------------------------------------------------------------
 */
typedef enum {
    EM_MIDP_KEY_MODE = 1,    
    EM_MIDP_FONT_SIZE = 2,
    EM_MIDP_STORAGE_HOME = 3,
    EM_MIDP_HEAP_SIZE = 4
} CSMidpOption_E;

typedef enum {
    EM_MIDP_KEYMODE_DEFAULT = 0, 
    EM_MIDP_KEYMODE_UDI    
} CSMidpKeyMode_E;

 /**
 * 以下为midp 字体大小
 */
typedef enum {
    EM_MIDP_FONT_SIZE_SMALL,    
    EM_MIDP_FONT_SIZE_MEDIUM,
    EM_MIDP_FONT_SIZE_LARGE    
} CSMidpFontSize_E;

 typedef struct _MidpInitParam
 {
     int nClientId;                  //GrOpen返回的ClientID
     int nParentWinId;          //midp 窗口的父窗口ID, 小于等于0时，认为是根Window
     int nX;
     int nY; 
     int width;
     int height;                     //midp窗口的位置和大小,位置相对于其父窗口
 
     int nEvtFromFlag;     // midp系统接收事件的方式
                                      //0 -midp auto get event form microwin
                                     // 1 caller post event to midp by CSMIDPHandEvent
 }CSMIDPInitParams_S;

 typedef enum 
{
	EM_MIDP_DVB_WAIT = 1,
	EM_MIDP_DVB_READY,
	EM_MIDP_DVB_FAILE
}CSMidpDvbState_E;

/**MIDP游戏状态枚举*/
typedef enum {
	EM_MIDP_ENTER,        //进入MIDP游戏
	EM_MIDP_EXIT 		   //退出MIDP游戏
} CSMIDPState_E;

typedef struct _MidpDvbProtocolReg
{
	int (*DvbInit)(void); 
	int (*DvbOpen)(const char * pszUrl, const char * pcMode, BOOL bNeedUpdate, DWORD dwUpdateInterval); //pszUrl: "dvb://207.2a.1069"; mode: "rb" ；bNeedUpdate-表示是否更新；dwUpdateInterval-表示检测更新的间隔时间
	int (*DvbRecv)(int nIndex, void *pBuffer, unsigned int size);
	int (*DvbClose)(int nIndex); //return CSMidpDvbState_E
	int (*DvbSelect)(int nIndex, int nTimeout); //nTimeout:ms; return value:CSMidpDvbState_E
	int (*DvbAvailable)(int nIndex);
	int (*DvbDestroy)(void);
}CSMidpDvbProtocolReg_S;

typedef int (*CSPlatformRequest_F)(char *pcUrl);


/**
@brief 获取MIDP状态回调函数定义
@param[in] eType详见CSMIDPState_E说明
@param[in] pvUserData 用户数据
@param[in] pMidpData 返回MIDP游戏的名称和jar的URL。格式如下："{midpname:value1,midpjar:value2}"
*/
typedef int (*CSMIDPStateCallback_F) (CSMIDPState_E eType, char* pMidpData, void* pvUserData);

/**
@brief 注册获取MIDP游戏状态的回调。
@param[in] fnCallback 指向回调函数的函数指针
@param[in] pvUserData 用户数据，可以为NULL
@note  可以允许注册多个回调
*/
BOOL CSMIDPAddStateCallback (CSMIDPStateCallback_F fnCallback ,void* pvUserData);

/**
@brief 删除获取MIDP游戏状态的回调。
@param[in] fnCallback 指向回调函数的函数指针
*/
BOOL CSMIDPDelStateCallback (CSMIDPStateCallback_F fnCallback, void* pvUserData);

 /******************************************************************************
  * FUNCTION:    CSMIDPInit
  * 
  * OVERVIEW:
  *        根据pMIDPInitParams初始化midp系统，
  *         创建midp窗口，
  *         设置事件来源，
  *         初始化midp   系统用到的参数，如，gc、font等。
  *   
  * Input:
  *     pMIDPInitParams:初始化参数，详见CSMIDPInitParams_S 定义
  * 
  * Output:
  *     无
  *
  * Return Value:
  *     midp窗口的windowID
  * 
  *Other:
  *     在启动midp前调用
  * *****************************************************************************/
  
  int CSMIDPInit(CSMIDPInitParams_S *pMIDPInitParams);
  
  
  /*=========================================================================
  * FUNCTION:      CSMIDPStart
  * OVERVIEW:      The main method. There are three modes that MIDP can
  *                be run:
  *
  *                   - TCK       driven by loading descriptors from
  *                                a url
  *                   - debug     classpath and MIDlet class on the
  *                                command line
  *                   - resident  install, remove, list, run apps
  * Input:
  *   parameters:  argc:  The total number of arguments
  *                argv:  An array of 'C' strings containing the arguments
  *   Return Value:
  *          int:   The status of the last VM invocation.
  *=======================================================================*/
 
 int CSMIDPStart(int argc, char* argv[]);
 
 /*=========================================================================
  * FUNCTION:      CSMIDPHandEvent
  * OVERVIEW:      如果设置了调用这发消息给midp，则通过此接口发送消息给midp
                            包括退出MIDP的调用,CSMIDPEXIT_KEYCODE这个键值同步退出MIDP
  * 
  *  
  * Input
  *   parameters:  
  *                   dwKeyType: 按键类型，GR_EVENT_TYPE_KEY_UP = 9  GR_EVENT_TYPE_KEY_DOWN = 8
  *                   dwKeyCode  对应键值,基于microwin和GTools的键值，CSMIDPEXIT_KEYCODE使MIDP退出
  *                  
  *   returns:     
  *              成功为0，失败返回-1，不处理返回-2
  *=======================================================================*/
 
 int CSMIDPHandEvent(DWORD dwKeyType, DWORD dwKeyCode);
 
  /*=========================================================================
  * FUNCTION:      CSMIDPGetStatus
  * OVERVIEW:     得到KVM+MIDP的运行状态
  * 
  *  
  * Input
  *   parameters:  
  *                  无
  *                  
  *   returns:     
  *            1: 运行状态
  *            0: 未运行状态
  *=======================================================================*/
 
 int CSMIDPGetStatus();

/*=========================================================================
 * FUNCTION:      CSMIDPExit
 * OVERVIEW:      退出KVM+MIDP应用，一般情况下不使用，应用本身退出后，虚拟机会退出
 *                      用于强行退出KVM时使用
 * 
 *  
 * Input
 *   parameters:  
 *          无
 *                  
 *   returns:     
 *              成功为0，否则其他值
 *=======================================================================*/

int CSMIDPExit();


  /********************************************************************************
 Function Name:    CSMIDPGetVersion
 
 Description:
         Function Description.  获取当前MIDP 的版本号
 
 Input:
         int   nSize       申请版本号存储长度                                                     
 
 Output:
        char*  pcVersion   MIDP 的版本号 
 
 Return Value:
     0 : 返回版本号长度;
     -1 : 返回默认长度;
 
 Other:
 *********************************************************************************/
 int CSMIDPGetVersion( char * pcVersion, int nSize );

  /********************************************************************************
 Function Name:    CSMidpRegDvbProtocol
 
 Description:
         Function Description. 
         已过时，为兼容老项目保留
         注册dvb 协议依赖的OC数据接收接口
         注意使用该接口默认用dvb://作为协议头
 
 Input:
         CSMidpDvbProtocolReg_S *pDvbProtocol      dvb协议注册函数列表                                 
 
 Output:
        无 
 
 Return Value:
     成功为0，否则其他值
 
 Other:
 *********************************************************************************/
int CSMidpRegDvbProtocol(CSMidpDvbProtocolReg_S *pDvbProtocol);

/********************************************************************************
 Function Name:    CSMidpRegUserProtocol
 
 Description:
         Function Description. 注册用户定义的各种协议依赖的数据接收接口
 
 Input:
  	   char *pUserProtocolHead  			    注册的协议头,目前只支持dvb://和pushfile://两种
         CSMidpDvbProtocolReg_S *pDvbProtocol      dvb协议注册函数列表                                 
 
 Output:
        无 
 
 Return Value:
     成功为0，否则其他值
 
 Other:
 *********************************************************************************/
int CSMidpRegUserProtocol(const char *pUserProtocolHead, CSMidpDvbProtocolReg_S *fnUserCallBack);

 /********************************************************************************
 Function Name:    CSMidpRegPlatformRequest
 
 Description:
         Function Description. 注册platformRequest回调函数；当midp应用中
         调用MIDlet.resumeRequest 时，midp将url透传给浏览器或用户
         来决定如何处理。
 
 Input:
         fnCSPlatformRequest fun      处理platformRequest url的回调函数    
         BOOL bExitMidp         midp抛出url之后是否主动退出,默认为退出
         void *pUserData        用户私有参数，保留以后使用
         
 Output:
        无 
 
 Return Value:
     成功为0，否则其他值
 
 Other:
 *********************************************************************************/
int CSMidpRegPlatformRequest(CSPlatformRequest_F fnCallback, BOOL bExit, void *pUserData);

 /********************************************************************************
 Function Name:    CSMidpRegKeyMap
 
 Description:
         Function Description. 注册midp特殊的键值映射，用户注册后将改变midp中相关的默认键值映射；
				用户注册后，midp优先从用户注册的映射中匹配键值；
				如果需要取消之前注册的键值，nMidpKey 注册为MIDP_KEY_INVALID 即可；
 
 Input:
         int nCoshipUdiKey      对应coship的udi定义的键值    
         CSMidpKeyType_E nMidpKey           对应midp规范中的键值
         
 Output:
        无 
 
 Return Value:
     成功为0，否则其他值
 
 Other:
 *********************************************************************************/
int CSMidpRegKeyMap(int nCoshipUdiKey, CSMidpKeyType_E nMidpKey);

/********************************************************************************
 Function Name:    CSMidpSetOption
 
 Description:
         Function Description. 用于设置属性
         
 Input:
         无
         
 Output:
        无 
 
 Return Value:
     成功为0，否则其他值
 
 Other:
 *********************************************************************************/
int CSMidpSetOption(CSMidpOption_E eOption, void *wParam, void *lParam);


/********************************************************************************
 Function Name:    CSMidpPause
 
 Description:
         Function Description. 用于暂停midp线程,同时会备份缓存和显存，
         resume时恢复
         
 Input:
         nX			备份矩形区域的起点x坐标
         nY			备份矩形区域的起点y坐标
         nW		备份矩形区域的宽度
         nH		       备份矩形区域的高度
         参数传入全0的话会备份全屏，缺点是浪费内存
 Output:
        无 
 
 Return Value:
     成功为0，否则其他值
 
 Other:
 *********************************************************************************/
int  CSMidpPause(int nX, int nY, int nW, int nH);

/********************************************************************************
 Function Name:    CSMidpResume
 
 Description:
         Function Description. 用于恢复midp线程，同时会恢复缓存和显存，
         必须和CSMidpPause配合使用
         
 Input:
         无
         
 Output:
        无 
 
 Return Value:
     成功为0，否则其他值
 
 Other:
 *********************************************************************************/
int  CSMidpResume(void);

/********************************************************************************
 Function Name:    CSMidpSetOption
 
 Description:
         Function Description. 用于midp播放视频接口初始化,注册播放需要的接口,解除和
         	mediaplayer的耦合关系，不调用则表示MIDP不支持视频播放。
         
 Input:
         无
         
 Output:
        无 
 
 Return Value:
     成功为0，否则其他值
 
 Other:
 *********************************************************************************/
int CSMidpVideoInit(void);
  
#ifdef __cplusplus
 }
#endif
#endif
 

