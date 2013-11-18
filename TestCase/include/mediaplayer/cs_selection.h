/******************************************************************************
FileName:  SelectionInterface.h
Copyright (c) 2007， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        pengyunjuan (pengyunjuan@coship.com)

File Description:
The interface of selection module.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2007-08-1  pengyunjuan       Create

*******************************************************************************/

#ifndef SELECTIONINTERFACE_H
#define SELECTIONINTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

    /******************************************************************************
    *                                 Header File Include                         *
    ******************************************************************************/
#include "mmcp_debug.h"
#include "mmcp_typedef.h"
#include "mmcp_tuner.h"
#include "cs_player_message.h"
    /******************************************************************************
    *                                 Macro/Define/Structus                       *
    ******************************************************************************/

#define   SEL_SERVICE_LOCK		0x01//当前节目为加了节目锁的节目，节目有效
#define   SEL_CAT               0x02//加载并侦听CAT，频点有效
#define   SEL_NIT				0x04//加载并侦听NIT，频点有效
#define   SEL_BAT				0x08//加载并侦听BAT，频点有效
#define   SEL_SDT				0x10//加载并侦听SDT，频点有效

#define   SEL_STOP_MONITOR_ALL 0 //停止播放时停止所有表的监听
#define   SEL_STOP_WITHOUT_CAT_MONITOR 0x1		//停止播放的时候不停止cat表监听

    typedef enum
    {
        EM_SEL_IPTV,             //IPTV MODE
        EM_SEL_IPQAM, 	       //IPQAM MODE
        EM_SEL_IPUDP_ES, 	       //IPQAM DUP传送ES类型
        EM_SEL_UNKNOWN    //未知类型
    }CSSELTransMode_E;

/* STB状态枚举 */
typedef enum{
	EM_SEL_STATUS_DVB,      //进入DVB
	EM_SEL_STATUS_BROWSE,   //进入浏览器
	EM_SEL_STATUS_VOD,      //进入VOD
	EM_SEL_STATUS_INVALID   
 }CSSELNrcStatus_E;

    /*为支持消息的透传功能，回调消息重新规划.
        修改原则:共有的消息定义在头文件中，私有的消息(比如特定服务器上的特有消息)透传
         
        IPPUMP:0x0~0xfff
        MEDIAPLAYER:0x1000~0x1fff
        SELECTION:0x2000~0x2fff
         
        */
    /******************************************************************************
    *                                 Global Function Declare                     *
    ******************************************************************************/
    /******************************************************************************
    Function Name:    FOnCSSELCallback
     
    Description:
    	用户回调,selection 把消息返回给用户
     
    Input:
    	dwUserData: 用户在创建selection时传入的用户数据
    	nMsg : 消息类型
    	dwPara1 : 	参数1 
    	dwPara2 :	参数2 
     
    Output:
     
    Return Value:
     
    Other:
    *******************************************************************************/
    typedef void (*FOnCSSELCallback)(DWORD dwUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);

/******************************************************************************
Function Name:    FOnCSEOCSwitch
 
Description:
	EOC回调, 通知NRC库， STB业务切换
 
Input:
	eMode: STB将切换至何种状态,取值参见CSSELNrcStatus_E
	   
Output:
 
Return Value:
    成功返回TRUE，失败返回FALSE
Other:
*******************************************************************************/
typedef BOOL (*FOnCSEOCSwitch)(CSSELNrcStatus_E eMode);

    /******************************************************************************
    Function Name:    CSSELSetTransMode
     
    Description:
    	用户设置机顶盒音视频传输模式:EM_SEL_IPTV或EM_SEL_IPQAM、EM_SEL_IPUDP_ES
     
    Input:
    	hSelection: selection句柄
    	eTranMode : CSSELTransMode_E类型枚举变量，见CSSELTransMode_E定义
     
    Output:
     
    Return Value:
       设置成功返回TRUE,否则返回FALSE
    Other:
      默认方式为EM_SEL_IPQAM,
      CSSELCreate之后紧接着调用，  在进入点播或时移之前要保证已经设置好
    *******************************************************************************/
    BOOL CSSELSetTransMode (HCSHANDLE hSelection, CSSELTransMode_E eTranMode);


    /******************************************************************************
    Function Name:    CSSELGetTransMode
     
    Description:
    	用户获取机顶盒音视频传输模式:EM_SEL_IPTV、EM_SEL_IPQAM、M_SEL_IPUDP_ES、EM_SEL_UNKNOWN
     
    Input:
    	hSelection: selection句柄
    	eTranMode : CSSELTransMode_E类型枚举变量，见CSSELTransMode_E定义
     
    Output:
     
    Return Value:
         返回获取到的传输模式，
    Other:
    如果Locator中带有streamChannel字段，则此函数得到的结果以此字段
    表明的模式为准。
    比如:
    应用开始设置了IPTV:CSSELSetTransMode(hSelection,EM_SEL_IPTV);
    后来播放时Locator为rtsp://10.10.99.10/ddd.ts&streamChannel=IPQAM
    之后调用此接口得到的结果为EM_SEL_IPQAM
    *******************************************************************************/
    CSSELTransMode_E CSSELGetTransMode (HCSHANDLE hSelection);


    /******************************************************************************
        Function Name:    CSSELCreate
         
        Description:
        	创建selection模块
         
        Input:
        	fnCallback : 注册的回调函数
        	dwUserData :  用户数据，回调函数中dwPara2的返回值
         
        Output:
         
        Return Value:
        	返回创建的selection模块句柄
         
        Other:
        *******************************************************************************/
    HCSHANDLE CSSELCreate (FOnCSSELCallback	fnCallback,
                           DWORD	dwUserData);

    /******************************************************************************
    Function Name:    CSSELDestroy
     
    Description:
    	销毁selection模块，释放相关资源
     
    Input:
    	hSelection : selection句柄
     
    Output:
     
    Return Value:
    	成功：TRUE
    	失败：FALSE
     
    Other:
    *******************************************************************************/
    BOOL CSSELDestroy(HCSHANDLE hSelection);

    /******************************************************************************
    Function Name:    CSSELSelectByLocator
     
    Description:
    	根据service的Locator进行业务选择
     
    Input:
    	hSelection : selection句柄
    	pszLocator : Locator字符串
    	dwOption :	播放选项，可以为NULL，也可以为头文件中define的选项 
     
    Output:
     
    Return Value:
    	成功：TRUE
    	失败：FALSE
     
    Other:
    合法Locator格式有以下几种:
	1、MP3 边下边播：http://a/xxx.mp3l；
	2、VOD：rtsp://10.10.99.10/ddd.ts；
	3、TV Channel：支持下列URL格式：
	1）dvb://OriginalNetworkID.TS_ID.ServiceID.ComponentID，具体参考《数字电视中间件技术规范》第3部分附录I的1.2.1小节。
	
	2)component://Frequency.SymbolRate.Modulation.pcrpid.vpid.vtype.apid.atype
	备注：dvb三要素格式中数字为16进制,component中均为10进制
			Frequency:单位Hz
			SymbolRate:单位kbps
			Modulation:16,32,64,128,256
	4、文件：file:///文件路径
	5、IFrame:  iframe://addr.nlen  （此种格式只对UDI2.0有效）
	addr为IFrame数据在内存地址，nLen为数据长度，两者均为十进制
    
    *******************************************************************************/
    BOOL CSSELSelectByLocator (HCSHANDLE hSelection,
                               const char *pszLocator,
                               DWORD	dwOption);

    /******************************************************************************
    Function Name:    CSSELSelectByChannel
     
    Description:
    	根据service的句柄进行业务选择，仅针对DVB业务
     
    Input:
    	hSelection : selection句柄
    	dwChannel : 需要播放的业务句柄即ServiceHandle
    	dwOption :	播放选项，可以为NULL，也可以为头文件中define的选项 
     
    Output:
     
    Return Value:
    	成功：TRUE
    	失败：FALSE
     
    Other:
    *******************************************************************************/
    BOOL CSSELSelectByChannel (HCSHANDLE hSelection,
                               DWORD dwChannel,
                               DWORD	dwOption);

    /******************************************************************************
    Function Name:    CSSELStop
     
    Description:
    	停止CSSELSelectByLocator/CSSELSelectByLocator启动的业务
     
    Input:
    	hSelection : selection句柄 
     
    Output:
     
    Return Value:
    	成功：TRUE
    	失败：FALSE
     
    Other:
    *******************************************************************************/
    BOOL CSSELStop (HCSHANDLE hSelection, DWORD dwStopFlag);


    /******************************************************************************
    Function Name:    CSSELGetMeidaPlayer
     
    Description:
    	获得由selection创建的MediaPlayer的句柄
     
    Input:
    	hSelection : selection句柄 
     
    Output:
    	phPlayerHandle：MediaPlayer句柄
     
    Return Value:
    	成功：TRUE
    	失败：FALSE
     
    Other:
    *******************************************************************************/
    BOOL CSSELGetMeidaPlayer (HCSHANDLE hSelection,
                              HCSHANDLE *phPlayerHandle);

    /******************************************************************************
    Function Name:    CSSELUnlock
     
    Description:
    	加锁节目用来解锁
     
    Input:
    	hSelection : selection句柄 
     
    Output:
    	成功：TRUE
    	失败：FALSE
     
    Return Value:
     
    Other:
    *******************************************************************************/
    BOOL CSSELUnlock (HCSHANDLE hSelection);
    /******************************************************************************
    Function Name:    CSSELGetDevicePath
     
    Description:
    	根据selection的句柄获取正在使用的demux设备(UDI1.0返回demux设备句柄,UDI2.0返回demux设备索引)
     
    Input:
    	hSelection:selection句柄
    Output:
    	
    Return Value:
    	成功：返回decoder句柄
    	失败：返回0
    Other:
    *******************************************************************************/
    int CSSELGetDevicePath(HCSHANDLE hSelection);
    /******************************************************************************
    Function Name:    CSSELGetTunnerPath
     
    Description:
    	根据selection的句柄获取正在使用的tunner句柄,
     
    Input:
    	hSelection:selection句柄
    Output:
    	
    Return Value:
    	成功：返回tunner句柄
    	失败：返回0
    Other:
    *******************************************************************************/
    int CSSELGetTunnerPath(HCSHANDLE hSelection);
    /******************************************************************************
    Function Name:    CSSELGetUsedSelections
     
    Description:
    	获取正在使用的selection,
     
    Input:
     
    Output:
     
     
    Return Value:
    	返回一个selection 数组，一般取数组的第一个元素
    Other:
    *******************************************************************************/
    HCSHANDLE* CSSELGetUsedSelections(void);
    /******************************************************************************
    Function Name:    CSSELSelectByFrequency
     
    Description:
    	根据Frequency，SymbolRate，Modulation，ServiceId播放
     
    Input:
    	hSelection : selection句柄 
    	pszLocator:  频点、符号率、调制方式、节目号构成的locator
    	格式有以下四种
    	1) dvb://Frequency.SymbolRate.Modulation.ServiceID
    	2) deliver://Frequency.SymbolRate.Modulation.ServiceID
    	3) dvb://Frequency:SymbolRate:Modulation:ServiceID
    	4) deliver://Frequency:SymbolRate:Modulation:ServiceID
		Frequency:频率.单位Hz
		SymbolRate:符号率
		Modulation:调制模式.
		ServiceId:节目ID
		如: deliver://490000000.6875.64.10

		备注：以上四种locator中出现数字均为10进制
    Output:
    	成功：TRUE
    	失败：FALSE
     
    Return Value:
     
    Other:
    *******************************************************************************/
    BOOL CSSELSelectByFrequency(HCSHANDLE hSelection, char *pszLocator);
    /******************************************************************************
    Function Name:    CSSELGetServiceHandle
     
    Description:
    	获取当前节目的service句柄，只适应于DVB节目。
    Input:
    	hSelection : selection句柄 
    Output:
     
    Return Value:
    	成功：非0
    	失败：0
     
    Other:
    *******************************************************************************/
    HCSHANDLE CSSELGetServiceHandle(HCSHANDLE hSelection);

    BOOL CSSELEnableCatMonitor(HCSHANDLE hSelection);

    BOOL CSSELDisableCatMonitor(HCSHANDLE hSelection);
    /******************************************************************************
    Function Name:    CSSELAddCallback
     
    Description:
    	向selection注册一个callback
    Input:
    	hSelection : selection句柄 
    	callback:要注册的callback
    Output:
     
    Return Value:
    	成功：非0
    	失败：0
     
    Other:可注册多个callback
    *******************************************************************************/
    BOOL CSSELAddCallback(HCSHANDLE hSelection, DWORD dwUserData, FOnCSSELCallback callback);
    /******************************************************************************
    Function Name:    CSSELRemoveCallback
     
    Description:
    	移除一个callback
    Input:
    	hSelection : selection句柄 
    	callback:要移除的callback
    Output:
     
    Return Value:
    	成功：非0
    	失败：0
     
    Other:
    *******************************************************************************/

    BOOL CSSELRemoveCallback(HCSHANDLE hSelection, DWORD dwUserData, FOnCSSELCallback callback);

    /******************************************************************************
    Function Name:    CSSELGetCurrentLocator
     
    Description:
    	获取当前节目的url
    Input:
    	hSelection : selection句柄 
    	nLen:pcLocator所指向的存储空间的大小
    Output:
    	pcLocator:用于存放当前节目的url.
    Return Value:
    	成功：非0
    	失败：0
     
    Other:
    *******************************************************************************/
    BOOL CSSELGetCurrentLocator(HCSHANDLE hSelection, char *pcLocator, int nLen);
    /**********************************************************************************************
    Function Name:    CSSELSetPMTMonitorPolicy
     
    Description:
    	设置pmt监听策略
    Input:
    	hSelection : selection句柄 
	    dwPeriod:监听周期，单位ms
	    dwCount:监听次数，由于点播节目pmt信息一般都不会实时更新，先做成在播放已个节目的前期进行监听
			当dwCount = 0xffffffff时表示一直监听，没有次数限制
    Output:
    
    Return Value:
    	成功：非0
    	失败：0
     
    Other:
    目的:		解决武汉点播黑屏问题
    问题原因:	华为ipqam对数据进行了缓存，如果先播一个mpg2的节目，接着播放h264的节目，刚好
    这个两个节目由被分配到同一频点、同一通道上，就会造成终端锁频成功后，解析pmt表，解析出来
    视频格式是mpg2的节目，而后来实际码流是h264的,pmt的版本号又没有发生改变，导致终端解码失败。
    解决方案: 监听版本号到不到预期效果，采用每隔一定时间主动请求pmt的策略。
    **********************************************************************************************/
    BOOL CSSELSetPMTMonitorPolicy(HCSHANDLE hSelection, DWORD dwPeriod, DWORD dwCount);
    /**********************************************************************************************
    Function Name:    CSSELSetPATMonitor
     
    Description:
    	设置是否要对pat进行监听。
    Input:
    	hSelection : selection句柄 
	bMonitor: TRUE监听PAT,  FALSE不监听 PAT
    Output:
    
    Return Value:
    	成功：非0
    	失败：0
     
    Other:
    目的:		天津项目前端pat pmt一起发生改变时当前节目黑屏，无提示。
    备注:		为了不影响其他项目，我们把pat监听做成可配置的选项，默认还是
    				不监听的，如果有监听的需求，请调用此接口进行设置.
    **********************************************************************************************/
    BOOL CSSELSetPATMonitor(HCSHANDLE hSelection,BOOL bMonitor  );

	/**********************************************************************************************
    Function Name:    CSSELSetSiTableMonitor
     
    Description:
    	设置是否要对SI中相关的表进行监听。
    Input:
    	hSelection : selection句柄 
		dwOption :	需要监听的表，不可以为NULL，必须为头文件中define的选项,
					如果设置为: SEL_BAT|SEL_SDT，表示监听BAT和SDT
    Output:
    
    Return Value:
    	成功：非0
    	失败：0
     
    Other:
    目的:		为满足irdeto CA的其中一个alarm消息，需要解析SDT表的需求。
    备注:		为了不影响其他CA和项目，当不需要解析和监听SDT表，并注入CA的功能时，所以我们把监听
    			SI的相关表做成可配置的选项，默认是不监听的，如果有监听指定表的需求，请调用此接口进行设置。
    			目前仅实现监听SDT表，并向各个CA注入
    **********************************************************************************************/
    BOOL CSSELSetSiTableMonitor(HCSHANDLE hSelection,DWORD	dwOption);



/******************************************************************************
Function Name:    CSSELRegisterEOCCallback

Description:
	 向selection模块注册EOC相关函数.

Input:
	hSelection: selection句柄 
	fnEOCSwitchCallback: 注册的EOC回调, 通知NRC库， STB业务切换

Output:
Return Value:
	 成功返回TRUE，失败返回FALSE

Other:
	主要针对需要使用EOC模块的项目，需要自行注册相应接口，默认是不注册的
*******************************************************************************/
BOOL CSSELRegisterEOCCallback(HCSHANDLE hSelection,FOnCSEOCSwitch fnEOCSwitchCallback);

/**********************************************************************************************
Function Name:    CSSELSetServiceLockCtr
 
Description:
	设置播放器是否要对节目锁进行音视频控制
Input:
	hSelection : selection句柄 
	bServiceLockCtr: 默认值为TRUE,TRUE表示播放器对节目锁进行音视频控制，FALSE表示不进行控制
Output:

Return Value:
	成功：TRUE
	失败：FALSE
 
Other:
目的:		广东项目需求，播放器不对节目锁进行音视频控制，将控制放在应用上实现
**********************************************************************************************/
BOOL CSSELSetServiceLockCtr(HCSHANDLE hSelection, BOOL bServiceLockCtr);	

/**********************************************************************************************
Function Name:    CSSELSetNgodSaFreq
 
Description:
	设置NGOD双向认证互动频点给播放器
Input:
	CSUDITunerCableDeliver_S : 双向认证互动频点列表
       nFreqCount: 双向互动频点个数
Output:

Return Value:
	成功：TRUE
	失败：FALSE
 
**********************************************************************************************/

BOOL CSSELSetNgodSaFreq (CSUDITunerCableDeliver_S *pstTunerCableDeliver , int nFreqCount);

/**********************************************************************************************
    Function Name:    CSSELSetIsCHKServiceIdchanged
     
    Description:
    	设置PAT表变化时，是否去校验其中的节目ID发生了变化
    Input:
    	hSelection : selection句柄 
		bServiceIDChangeCHK 是否校验serviceID在PAT更新时发生了变化，默认值为FALSE
    Output:
    
    Return Value:
    	成功：TRUE
    	失败：FALSE
     
   Other:
    目的:		天津项目前端pat更新时有时会改变serviceID，需要校验后更新，请调用此接口进行设置.
**********************************************************************************************/
BOOL CSSELSetIsCHKServiceIdchanged(HCSHANDLE hSelection, BOOL bServiceIDChangeCHK );

/**********************************************************************************************
    Function Name:    CSSELSetStackSize
     
    Description:
    	设置mediaplayer线程堆栈大小，必须在CSSELCreate函数调用之前调用该接口才有效，主要用于解决适配播放中异步操作时mediaplayer线程过小问题
    Input:
    	nStackSize 设置的线程堆栈大小，一般要大于 32 * 1024
    Output:
    
    Return Value:
    	none
**********************************************************************************************/
void CSSELSetStackSize(int nStackSize);
#ifdef __cplusplus
}
#endif

#endif
