/******************************************************************************
FileName:	cs_workermanager.h
Copyright (c) 2007， Shen Zhen Coship Electronics CO.,LTD.。

Version:	Ver 1.0    
Author:		zhudengyu
Author:		pengyunjuan
File Description:
	基于系统移植层的ResourceManager，对Tuner，AV，Demux等可共享的资源进行统一的冲突检测、
	分配管理，避免上层模块各自为政，造成资源独占，同时提高使用的方便性。

Modify History:
	Version		Date		Author		Description
	--------	----------	--------	------------
	1.0			2007-8-13	zhudengyu		Create
	1.0         2007-8-13   pengyunjuan     Create
	1.2			2008-1-30	zhaodemin	Modify
*******************************************************************************/


#ifndef _CS_WORKERMANAGER_H_
#define _CS_WORKERMANAGER_H_

#include "mmcp_typedef.h"
#include "mmcp_tuner.h"

#ifdef MMCP_UDI2
#include "udi2_rdi.h"
#else
#include "cs_rm.h"
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

/******data sturct*********/
typedef enum _CSWMRequestType_E
{
	EM_WM_TUNER,                                           //锁频；
	EM_WM_TUNER_DMX,                                //锁频，请求demux数据，
	EM_WM_TUNER_DMX_REC,                     //锁频，请求demux数据，并进行录制
	EM_WM_TUNER_DMX_PLAY,		//锁频，请求demux数据，并进行播放
	EM_WM_DMX_DEC_AVOUT,                       //播放IP或FILE数据，数据格式为TS
	EM_WM_DEC_AVOUT,                                   //播放IP或FILE数据，数据格式为ES/PES;
	EM_WM_DEC_AOUT_PCM,                                   //播放PCM数据
	EM_WM_TUNER_DMX_AOUT,		//锁频，请求demux数据，并进行音频播放
	EM_WM_DEC_VOUT                         //播放I帧数据
} CSWMRequestType_E;

/*
	请求资源时设置的优先级
*/
typedef enum _CSWMPRIORITY_E
{
	EM_WM_PRIORITY_1 = 1,
	EM_WM_PRIORITY_2 = 2,
	EM_WM_PRIORITY_3 = 3,
	EM_WM_PRIORITY_4 = 4,
	EM_WM_PRIORITY_5 = 5,
	EM_WM_PRIORITY_6 = 6,
	EM_WM_PRIORITY_7 = 7,
	EM_WM_PRIORITY_8 = 8,
	EM_WM_PRIORITY_9 = 9,
	EM_WM_PRIORITY_10 = 10,
	EM_WM_PRIORITY_11 = 11
}CSWMPRIORITY_E;

typedef struct _CSWMRequestInfo_S
{
	CSUDITunerSRCDeliver_U		unDeliver;		//请求的Tuner要锁定的频点,如果不关注具体频点，可以全部赋0
	CSUDITunerType_E		emTunerType;			//要使用 Tuner的类型
	DWORD dwTunerMask;		//可供选择的Tuner掩码
	DWORD dwServiceUniTag;		//可用serviceid，用来指定demux要传输的serviceId，如果不关心此值，需要传0
	CSWMPRIORITY_E         emRequestPriority;		//请求的优先级
	int         nVIndex;                         //video decoder index，此值也是窗口index
} CSWMRequestInfo_S;

typedef enum _CSWMDeviceType_E
{
    EM_WM_DEVICE_TUNER, 	/**< 高频头*/
    EM_WM_DEVICE_DEMUX,              /**< 解复用器*/
    EM_WM_DEVICE_VIDEO,              /**< 视频解码器*/
    EM_WM_DEVICE_AUDIO,              /**< 音频解码器*/
    EM_WM_DEVICE_UNDEF
}CSWMDeviceType_E;

typedef struct _CSWMDeviceInfo_S
{
    CSWMDeviceType_E eDeviceType;           //eDeviceType：设备的类型
    HCSHANDLE nDeviceHandle;                                          //nDeviceId：设备的句柄
}CSWMDeviceInfo_S;

typedef enum _CSWMEvent_E
{
    EM_WM_EVENT_UNKNOWN = 0,    ///未知设备事件,非法值
    EM_WM_EVENT_PLUGOUT = 1,     //设备移除,pEventData为CSWMDeviceInfo_S,该消息通知返回后将不再可通过该设备链句柄获取设备信息
    EM_WM_EVENT_PLUGIN = 2,  //设备插入,pEventData为CSWMDeviceInfo_S
    EM_WM_EVENT_GRABBED = 3,     //设备要被其他高优先级的任务抢占，应用收到此消息后，需要在回调中同步的将自己的path释放掉,pEventData为被抢的设备的CSWMDeviceInfo_S
    EM_WM_EVENT_PATH_AFTER_CREATE = 10,//有新的path被创建了，pEventData值为新创建的path的句柄hDevicePath
    EM_WM_EVENT_PATH_AFTER_DESTROY = 20 //有path被释放了，pEventData值暂时无定义，不允许被使用
}CSWMEvent_E;

/*****************************************************
定义设备通知函数原形。
当有设备状态发生变化(插入、拔出等)时,WordManager模块通过该类型的回调函数通知用户
    hDevicePath：从WorkManager中申请到的设备链路的句柄,其值与调用CSWMAddCallback时传入的hDevicePath值相同
    eEvent: 事件名称,详细定义见CSWMEvent_E说明
    pvEventData: 与事件关联的数据，详细定义见CSWMEvent_E说明
    pvUserData: 用户数据,调用CSWMAddCallback_F注册该回调时传入的pvUserData
*****************************************************/
typedef void(*CSWMCallback_F)(HCSHANDLE hDevicePath, CSWMEvent_E eEvent, const void * pvEventData, void * pvUserData);

#define CS_WM_PRIORITY_HIGHEST         90          //表示最高优先级
#define CS_WM_PRIORITY_HIGHER         70          //表示较高优先级
#define CS_WM_PRIORITY_NORMAL       50          //表示正常优先级
#define CS_WM_PRIORITY_LOWER          30          //表示较低优先级
#define CS_WM_PRIORITY_LOWEST          10          //表示最低优先级

/******************************************************************************
Function Name:    CSWMInitStyleSimple

Description:
    初始化WorkManager的工作方式为简单工作模式
    WorkManager在此模式不使用其设备管理功能
    此时，再调用WorkManager的其他接口获取设备相关的内容时，
    WorkManager只会返回唯一的一条有效path,其所有设备ID均为0

Input:

Output:

Return Value:
    成功返回TRUE，失败返回FALSE；

Other:
    注意一点，如果调用了此方法，则不允许再调用CSWMInit方法
*******************************************************************************/
BOOL CSWMInitStyleSimple(void);

/******************************************************************************
Function Name:    CSWMInit

Description:
	初始化WorkManager模块。

Input:

Output:

Return Value:
	成功返回TRUE，失败返回FALSE；

Other:
	如果该模块已经初始化过，也返回TRUE；
*******************************************************************************/
BOOL CSWMInit(void);

/******************************************************************************
Function Name:    CSWMGetDefaultRequestInfo

Description:
	初始化CSWMRequestInfo_S结构体。

Input:
	psRequestInfo:外部声明的CSWMRequestInfo_S结构体实例地址
Output:


Return Value:
	成功返回TRUE，失败返回FALSE；

Other:

*******************************************************************************/
BOOL CSWMGetDefaultRequestInfo(CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMTryRequest

Description:
	尝试请求分配一条设备链。

Input:
	enmRequestType：请求的设备链的用途；
	psRequestInfo：请求设备链时的信息；
Output:


Return Value:
	如果可以请求到设备链则返回TRUE，否则返回FALSE；

Other:
*******************************************************************************/
BOOL CSWMTryRequest(CSWMRequestType_E enmRequestType, CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMRequest

Description:
	请求分配一条设备链。

Input:
	enmRequestType：请求的设备链的用途；
	psRequestInfo：请求设备链时的信息；

Return Value:
	如果可以请求到设备链则返回设备链的句柄，否则返回NULL；
	当请求类型为EM_WM_TUNER的时候，此设备链中连接了tuner
    当请求类型为EM_WM_TUNER_DMX时，此设备链中连接了tuner+demux
    当请求类型为EM_WM_TUNER_DMX_REC时，此设备链中连接了tuner+demux
    当请求类型为EM_WM_TUNER_DMX_PLAY时，此设备链中连接了tuner+demux+a+v
    当请求类型为EM_WM_DEMUX时，此设备链中只包含demux
    当请求类型为EM_WM_DMX_DEC_AVOUT时，此设备链中连接了demux+a+v
    当请求类型为EM_WM_DEC_AVOUT时，此设备链中连接了a+v


Other:
*******************************************************************************/
HCSHANDLE CSWMRequest(CSWMRequestType_E eRequestType, CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMRequestByName

Description:
	请求分配一条设备链。

Input:
	enmRequestType：请求的设备链的用途；
	psRequestInfo：请求设备链时的信息；
	pcModuleName:调用此接口的模块名；

Return Value:
	如果可以请求到设备链则返回设备链的句柄，否则返回NULL；
	当请求类型为EM_WM_TUNER的时候，此设备链中连接了tuner
    当请求类型为EM_WM_TUNER_DMX时，此设备链中连接了tuner+demux
    当请求类型为EM_WM_TUNER_DMX_REC时，此设备链中连接了tuner+demux
    当请求类型为EM_WM_TUNER_DMX_PLAY时，此设备链中连接了tuner+demux+a+v
    当请求类型为EM_WM_DEMUX时，此设备链中只包含demux
    当请求类型为EM_WM_DMX_DEC_AVOUT时，此设备链中连接了demux+a+v
    当请求类型为EM_WM_DEC_AVOUT时，此设备链中连接了a+v


Other:
*******************************************************************************/
/*模块名                   优先级
   dtvmx.ads                   EM_WM_PRIORITY_6
   dtvmx.davic                 EM_WM_PRIORITY_7
   dtvmx.dtv                   EM_WM_PRIORITY_8/EM_WM_PRIORITY_10/EM_WM_PRIORITY_6
   mp                          EM_WM_PRIORITY_5
   mp.ffmpeg                   EM_WM_PRIORITY_5
   mp.sa                       EM_WM_PRIORITY_6
   cfg.nvod                    EM_WM_PRIORITY_6
   cfg.ota                     EM_WM_PRIORITY_6/EM_WM_PRIORITY_11/EM_WM_PRIORITY_10
   cfg.TuneMainFreq            EM_WM_PRIORITY_5
   cfg.citv                    EM_WM_PRIORITY_6
   shell.ota                   EM_WM_PRIORITY_6
   shell.ngodsa                EM_WM_PRIORITY_5 /EM_WM_PRIORITY_4
   kernel.occtrl               EM_WM_PRIORITY_9
   jsext                       EM_WM_PRIORITY_11
*/ 

HCSHANDLE CSWMRequestByName(const char * pcModuleName, CSWMRequestType_E eRequestType, CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMGetRequestParam

Description:
	根据path获取申请此path时的请求参数

Input:
        hDevicePath : 设备链路句柄

Output :
	peRequestType：设备链的用途；
	psRequestInfo：设备链的信息

Return Value:
	申请此path时的请求参数

Other:
*******************************************************************************/
BOOL CSWMGetRequestParam(HCSHANDLE hDevicePath, CSWMRequestType_E *peRequestType, CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMCancel

Description:
	停止掉对一条设备链的使用。

Input:
	handle：要停掉的设备链的句柄。
Output:


Return Value:
	成功返回TRUE，失败返回FALSE；

Other:
*******************************************************************************/
BOOL CSWMCancel(HCSHANDLE hDevicePath);

/******************************************************************************
Function Name:    CSWMGetDeviceId

Description:
	根据WorkManager模块中的设备链路句柄 获取指定设备类型的ID
Input:
	hDevicePath：从WorkManager中申请到的设备链路的句柄
	eDeviceType：设备类型
Output:

Return Value:
	返回指定类型的设备的ID，如果指定类型的设备中此链路中不存在，则返回-1
	如果是tuner设备，则返回tuner_id，如果是Demux或者decoder，则返回设备的索引

Other:
*******************************************************************************/
int CSWMGetDeviceId(HCSHANDLE hDevicePath, CSWMDeviceType_E  eDeviceType);

/******************************************************************************
 * Function Name:    CSWMGetDeviceCount
 * 
 * Description:
 * 通过设备类型获取WorkManager中记录的设备的总个数
 * 
 * Input:
 * emDeviceType: 设备类型
 * 
 * Output:
 * 无
 * 
 * Return Value:
 * 返回指定的设备类型在WorkManager中记录的个数
 * 
 * Other:
*******************************************************************************/
int CSWMGetDeviceCount(CSWMDeviceType_E emDeviceType);

/******************************************************************************
Function Name:    CSWMAddCallback

Description:
    添加设备通知回调函数
Input:
    hDevicePath: 此参数推荐传0，所有事件都会被通知此回调，如果传的是指定申请的path，则只会接收到与此path相关的事件
    fnCallback：回调函数,当设备状态发生变化时调用该函数
    pvUserData：用户数据,将作为回调函数的pvUserData参数返回

Output:

Return Value:
    成功返回TRUE，失败返回FALSE

Other:
    添加回调前产生的设备变化信息,不会补充通知.即若开机时这些设备就存在,调用本函数时不会得到通知
*******************************************************************************/
BOOL CSWMAddCallback(HCSHANDLE hDevicePath, CSWMCallback_F fnCallback, const void * pvUserData);

/******************************************************************************
Function Name:    CSWMRemoveCallback

Description:
    删除设备通知回调函数
Input:
    hDevicePath: 调用CSWMAddCallback注册时传入的hDevicePath
    fnCallback：回调函数,之前注册的函数
    pvUserData：用户数据,必须与注册时的一致

Output:

Return Value:
    成功返回TRUE，失败返回FALSE

Other:
    fnCallback和pvUserData一起唯一确定一个回调函数
*******************************************************************************/
BOOL CSWMRemoveCallback(HCSHANDLE hDevicePath, CSWMCallback_F fnCallback, const void * pvUserData);

#ifdef __cplusplus
}
#endif

#endif

