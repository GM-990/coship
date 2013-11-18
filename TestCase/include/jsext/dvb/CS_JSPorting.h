
#ifndef _CS_JSPorting_H_
#define _CS_JSPorting_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h"
#include "cs_mediaplayer.h"

#ifdef MMCP_UDI2
#include "udi2_vout.h"
#endif

typedef void (*CSJSSearchCallback_F)(DWORD dwMessageType, const char* caJsonStr, int nLength);

typedef struct _JSSearchDataInfo_S{
	void* m_pUserData;
	CSJSSearchCallback_F m_fnSearchCallback;
}JSSearchDataInfo_S;

//CSGTIoctlWrite时传入的key值

//与此key值对应的value无意义
#define CS_JS_RESTORE_FACTORY_DEFAULT           "JS.RestoreFactoryDefault" 

//与此key值对应的value无意义
#define CS_JS_STANDBY                                           "JS.Standby"

//与此key值对应的value格式为"{appName:namevalue},{args:argsvalue}"
#define CS_JS_STARTAPP                                          "JS.StartApp"

//与此key值对应的value值为节目句柄,使用时可将char*类型转换为(HCSHANDLE)
#define CS_JS_SET_SERVICE_VOLUME                                    "JS.SetServiceVolume"

//与此key值对应的value无意义
#define CS_JS_RESTORE_FACTORY_DEFAULT           "JS.RestoreFactoryDefault" 

//与此key值对应的value无意义
#define CS_NGB_JS_RESTORE_FACTORY_DEFAULT		"JS.NGB.RestoreFactoryDefault" 


//节目类型的宏定义
#define SERVICE_LIST_TYPE_SERVICE       0
#define SERVICE_LIST_TYPE_FAV           1
#define SERVICE_LIST_TYPE_BAT           2
#define SERVICE_LIST_TYPE_USEDEF        3
#define SERVICE_LIST_TYPE_HIDEATTR      4
#define SERVICE_LIST_TYPE_SAT      5
#define SERVICE_LIST_TYPE_FTA      6
#define SERVICE_LIST_TYPE_SCR      7
#define SERVICE_LIST_TYPE_SIMILAR_NAME      8
#define SERVICE_LIST_TYPE_TP     9

#define FILESTORAGE_MAX_DIR_LENGTH   256

#define DM_STATES_MAX_CALLBACK_COUNT  10

typedef enum _CSJSMediaplayerVolumeMode_E    //兼容天威音量
{
	EM_JSMP_VOLUMEMODE_0_31,                 //系统默认0-31
	EM_JSMP_VOLUMEMODE_0_100                 //天威0-100
}CSJSMediaplayerVolumeMode_E;
typedef enum _CS_JSDATATYPE_E
{
    EM_JS_DATA_SYSPAR,		//注册系统参数相关的对象，现在不使用
    EM_JS_DATA_VOLCHA,         //节目音量、声道
    EM_JS_DATA_COMDATA,        //其他数据，比如预订数据、定时开关机数据等
    EM_JS_DATA_NUMBER
}CS_JSDATATYPE_E;

/**
@brief DM状态枚举
*/
typedef enum _CS_DM_STATES_E
{
    EM_DM_STATE_SEARCH_FINISHED,
    EM_DM_STATE_SAVE_TO_FLASH,
    EM_DM_STATE_REVERT_FROM_FLASH,
    EM_DM_STATE_RESTORE_FACTORY_DEFAULT
}CS_DM_STATES_E;

/**
@brief DM状态变化回调函数
@param [in] dwUserData 用户自定义数据，由应用注册回调函数时传下来
@param [in] eStatus DM状态
*/
typedef void (*CSDmStatesChangeCallback_F)(DWORD dwUserData, CS_DM_STATES_E eStatus);


/**
@brief 网卡ip模式回调函数
@param [in] pUserData 用户自定义数据，由应用注册回调函数时传下来
@param [in] nEthernetIndex 用户自定义数据，由应用注册回调函数时传下来
@param [out] pcIpMode 字符串 "DHCP" 或者"MANUAL_CONFIG"
*/
typedef void (*CSEthernetModeCallback_F)(void *pUserData, int nEthernetIndex, char *pcIpMode);

typedef struct _CSDMStatesCallbackPair_S
{
	DWORD				dwUserData;
	CSDmStatesChangeCallback_F	fnCallback;
}CSDMStatesCallbackPair_S;

typedef struct _CSJSSearchParam_S
{
	int nBouquetID; 		//搜索过滤区域码
	int nDescriptorType; 	//搜索指定私有描述符类型
	int nDescriptorTag;		//搜索指定私有描述符tag值
	int anType[16];			//搜索过程中需要创建channel对象的节目类型
	int nTypeCount;			//搜索过程中创建channel对象的节目类型个数
	int nSearchMode;		//搜索模式(自动搜索、手动搜索等)
}CSJSSearchParam_S;

typedef struct
{
	DWORD m_dwFileNumber; ///< 文件编号，以数字命名
    DWORD m_dwFormat;     ///< gif, bmp, jpg, mpg。mpg特指I帧格式。
    DWORD m_dwXPos;         ///< x坐标
    DWORD m_dwYPos;			///< y坐标
    DWORD m_dwWidth;   ///< 宽度
    DWORD m_dwHeight;    ///< 高度
    DWORD m_dwDuration; ///< 持续时间，单位是秒，为0表示一直显示。
    DWORD m_dwVersion; ///< 版本
	int   m_nDataLen;    ///< 对应的数据长度
	BYTE *m_pucBuffer;	///< 对应的数据内容
}CSSkyworthADInfo;


/*==========================================================
函数原型:CSJSPortingRegisterFileDir()
输入参数:
    eDataType: 见上面CS_JSDATATYPE_E定义
    pcDir: 对应于eDataType类型数据的文件的保存目录
    nLength: pcDir的长度，长度不能超过FILESTORAGE_MAX_DIR_LENGTH
输出参数:
    无
返回值:  
    成功 0
    失败 -1
功能 :  
    注册要按文件保存的数据类型的文件目录
注意事项: 
    调用了此方法说明保存这些参数都要用文件系统的方式来保存，注意，此方法暂不支持与CSJSPortingRegisterFlashAddr同时使用的情况
    两者只能选其一
=========================================================== */
int CSJSPortingRegisterFileDir(CS_JSDATATYPE_E eDataType, const char* pcDir, int nLength);


/*==========================================================
函数原型:CSJSPortingInit()
输入参数:
    无
输出参数:
    无
返回值:  
    成功 0
    失败 -1
功能 :  
    初始化JS native
注意事项: 
    此方法必须调用
=========================================================== */
int CSJSPortingInit(void);

/******************************************************************************
Function Name:    JSProcessCAMessage
	
Description:
	处理CA消息
	处理的消息内容包括在C应用中无法处理的消息对象传递给页面等消息
Input:
	无
Output:
    None

Return Value:
	成功返回0，失败-1；
Other:
    处理的CA消息比如在算通CA中，获取智能卡信息是通过消息异步的方式抛给页面，
    而在C应用中，无法把智能卡信息这个js对象发送给页面，此时就要调用此方法来处理
*******************************************************************************/
int JSProcessCAMessage( DWORD dwType, DWORD wParam, DWORD lParam );

/******************************************************************************
Description:
	判断指定节目是否属于指定的用户分组
Input:
	hService : 要判断的节目的句柄
    nUserGroupID : 用户自定义分组的ID（从1开始，表示第几个用户分组）
Output:
    None

Return Value:
	如果指定的节目属于此分组， 则返回TRUE,否则返回FALSE
Other:
    无
*******************************************************************************/
typedef BOOL (*CSJSServiceCanMatch_F)(HCSHANDLE hService, int nUserGroupID);

/******************************************************************************
Description:
	注册判断指定节目是否属于指定的用户分组的回调
Input:
	pfnMatchCallback : 回调函数指针
Output:
    None

Return Value:
	成功返回0，失败返回其他值
Other:
    无
*******************************************************************************/
int CSJSRegisterUserGroupCallback(CSJSServiceCanMatch_F pfnMatchCallback);

/******************************************************************************
Description:
	排序规则回调类型
Input:
	hServiceLhs : 节目句柄
	hServiceRhs : 节目句柄
	nCompareUserData : 通过JS应用透传下来的值，可以表示指定的排序规则
Output:
    None

Return Value:
	如果前一个节目应该排在后一个节目之后，则返回大于0的值
	否则，返回小于0的值
Other:
    无
*******************************************************************************/
typedef int (*CSJSCompareService) ( HCSHANDLE hServiceLhs, HCSHANDLE hServiceRhs, int nCompareUserData );

/******************************************************************************
Description:
	注册用户自定义排序规则回调
Input:
	pfnCompareRule : 回调函数指针
Output:
    None

Return Value:
	成功返回0，失败返回其他值
Other:
    无
*******************************************************************************/
int CSJSRegisterCompareRule(CSJSCompareService pfnCompareRule);


/******************************************************************************
Description:
	根据节目分组类型和值重新建立节目列表,对于用户自定义分组，当节目分组发生改变时，应该调用此接口。
Input:
	nListType:节目类型，取值为节目类型的宏定义，例如喜爱为:SERVICE_LIST_TYPE_FAV
	nValue:节目类型对应的值，参考数字电视标准。例如:TV对应的nValue取1,RADIO取2等等.
Output:
    None

Return Value:
	成功返回0，失败返回其他值
Other:
    当nListType和nValue都传入-1，表示重新构造所有的节目列表
*******************************************************************************/
int CSJSRebuildServiceList(int nListType, int nValue);

/******************************************************************************
Description:
	设置Schedule epg搜索模式，true表示只要一次搜索Schedule epg完成后，后续只是检测Schedule epg，换台不再启动搜索
       项目中不掉用该函数，默认Schedule epg搜索模式为bOnlyOnce = false

Input:
    bOnlyOnce:是否只搜索一次schedule EPG
Output:
    None

Return Value:
    无
*******************************************************************************/
void CSJSEPGSetScheduleEpgSearchMode(BOOL bOnlyOnce);

/**
@brief 添加DM状态变化回调
@param [in] dwUserData 用户自定义数据
@param [in] fncallback DM状态变化回调
@return 成功返回TRUE, 失败返回FALSE
@note 以fncallback和dwUserData为关键字判断是否重复，不允许重复的注册
*/
BOOL CSJSDmStatesAddCallback(DWORD dwUserData, CSDmStatesChangeCallback_F fncallback);

/**
@brief 删除DM状态变化回调
@param [in] dwUserData 用户自定义数据
@param [in] fncallback DM状态变化回调
@return 成功返回TRUE, 失败返回FALSE
@以fncallback为关键字删除已注册的函数
*/
BOOL CSJSDmStatesRemoveCallback(DWORD dwUserData, CSDmStatesChangeCallback_F fncallback);

/******************************************************************************
Description:
	恢复出厂设置

Input:
    None
    
Output:
    None

Return Value:
    无
*******************************************************************************/
void CSJSRestoreFactoryDefault(void);
/********************************************************************************
Description: 此接口是为JS Mediaplayer对象 setVolume 方法设置，天威接口定义setVolume 传入的值为0-100
      新接口定义的传入值的范围为0-31.现有的项目按新接口定义的不用调用此接口，中间件默认的为0-31；
      如果是天威项目，需要在工程中调用此接口CSJSMediaplayerSetVolumeMode(EM_JSMP_VOLUMEMODE_0_100)
Input:
      eMode: EM_JSMP_VOLUMEMODE_0_31 表示传入的音量值范围为0-31
  	  EM_JSMP_VOLUMEMODE_0_100 表示传入的音量值范围为0-100
Output:
      无
Return Value:
      无
      
**********************************************************************************/
void CSJSMediaplayerSetVolumeMode(CSJSMediaplayerVolumeMode_E eMode);

/******************************************************************************
Description:
	此接口是为锁频成功后自动更新，若上一次自动更新未完成则继续进行更新，否则开启NIT监听
Input:
	puDeliver当前锁频的频点参数
Output:
    None

Return Value:
	None
*******************************************************************************/
void CSJSStartUpdate(Deliver *puDeliver);

/******************************************************************************
Description:
	此接口在锁频前调用，主要用于停止nit监听，且自动更新未完成时停止本次更新
Input:
	None
Output:
    None

Return Value:
	None
*******************************************************************************/
void CSJSStopUpdate();

/******************************************************************************
Description:
	获取搜索参数，参数具体值CSSearchParam_S中定义
Input:
	None
Output:
    pstSearchParam 搜索参数结构体
    
Return Value:
	None
*******************************************************************************/
 void CSJSGetSearchParam(CSJSSearchParam_S *pstSearchParam);
 
 /******************************************************************************
Description:
	设置搜索过程中是否发送JSCALL_POP_SERVICE消息
Input:
	bIsSend:为TRUE表示发送，为FALSE表示不发送。默认不发送
Output:
    无
    
Return Value:
	None
*******************************************************************************/
 void CSJSSetSearchIsSendPopServiceMsg(BOOL bIsSend);

/******************************************************************************
Description:
	设置算通马赛克解析Logic Screen的坐标时，是以PMT表为准还是以SDT表为准
	注:当坐标只在一个表中有描述时，当以这个表为准，此设置接口无效
Input:
	nFlag:
		0表示Logic Screen的坐标是以PMT表为准
		1表示Logic Screen的坐标是以SDT表为准
Output:
    无
    
Return Value:
	None
*******************************************************************************/
 void CSJSSTMosaicSetParseFlagForLScreen(int nFlag);

/******************************************************************************
Description:
    设置哈雷马赛克的区域码

Input:
    nAreaCode:区域码

Output:
    None

Return Value:
    无
*******************************************************************************/
void CSJSHmMosaicSetAreaCode(int nAreaCode); 

/******************************************************************************
Description:
    获取创维广告的属性信息，这个接口由项目的实现，当项目要使用这个功能的时候，需要注册void CSJSRegisteSkyworthAD(void);这个接口

Input:
nTypeIndex：广告类型
nOrgNetworkID：节目的原始网络ID
nTsID:节目的TSID
nServiceID：节目的ServiceID
Output:
    pstADInfo：创维广告的属性信息

Return Value:
    成功返回TRUE,失败返回FALSE
*******************************************************************************/
BOOL CSJSGetSkyworthADInfoByService(int nTypeIndex, int nOrgNetworkID,int nTsID, int nServiceID ,CSSkyworthADInfo *pstADInfo);

/******************************************************************************
Description:
    NGB开机初始化音频连接音频设备个数
Input:
    nAreaCode:区域码

Output:
    None

Return Value:
    无
*******************************************************************************/
BOOL CSJSAddAudioOutputInterface(CSMPAUDOutputDevice_E  eDeviceType,char *pcAudioDeviceName, BOOL bOpenStatus);


#ifdef MMCP_UDI2
/******************************************************************************
Description:
    NGB开机初始化音频连接音频视频个数

Input:
    eDeviceType:视频输出设备类型
    pcVideoDeviceName:视频设备名称
    bOpenStatus:

Output:
    None

Return Value:
    无
*******************************************************************************/
BOOL CSJSAddVideoOutputInterface(CSUDIVOUTDevices_E  eDeviceType, char *pcVideoDeviceName, BOOL bOpenStatus);
#endif

/******************************************************************************
Description:
    注册获取网卡模式

Input:
    fnCallback:注册的回调函数
    pUserData:用户数据

Output:
    None

Return Value:
    无
*******************************************************************************/
BOOL CSJSRegisteGetEthernetMode(CSEthernetModeCallback_F fnCallback, void *pUserData);

void CSJSAPPSyncDMtoFlash(void);
int CSJSStopSearch(void);
BOOL CSJSAddSearchCallback(CSJSSearchCallback_F fnSearchCallback, void* pUserData);
BOOL CSJSRemoveSearchCallback(CSJSSearchCallback_F fnSearchCallback);
int CSJSStartCableSearch(int nSearchType, Deliver *pDeliver, int nDeliverCount);

#ifdef __cplusplus
}
#endif

#endif

