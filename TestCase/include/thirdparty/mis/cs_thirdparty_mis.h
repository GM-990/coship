#ifndef _CS_THIRDPARTY_MIS_H
#define _CS_THIRDPARTY_MIS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "udiplus_typedef.h"
#include "udi2_input.h"

typedef enum _CSMISCallbackMessageType_E
{
	MIS_SHOW_ADV = 0				// 显示泡泡广告
	, MIS_HIDE_ADV					// 隐藏泡泡广告
}CSMISCallbackMessageType_E;		// Mis发送回调类型

typedef enum _CSMISAdvType_E
{
	MIS_GENERAL_ADV = 0				// 普通泡泡广告
	, MIS_CAPTION_ADV				// 字幕泡泡广告
}CSMISAdvType_E;					// 广告类型 

typedef enum _CSMISEpgAdvType_E
{
	MIS_CHANNELBAR_EPGADV = 0		// 换台广告位(channelbar)
	, MIS_VOLBAR_EPGADV				// 音量条广告位(volbar)
	, MIS_OKLIST_EPGADV				// 节目索引广告位(oklist)
	, MIS_PUB_CHANNEL_EPGADV		// 公共频道广告位
	, MIS_FEE_CHANNEL_EPGADV		// 付费频道广告位
	, MIS_NVOD_THEATRA_EPGADV		// 点播剧院广告位
	, MIS_BROADCAST_EPGADV			// 音乐广播广告位
	, MIS_VOD_CHANNEL_EPGADV		// vod播放器广告位
	, MIS_VOD_PAUSE_EPGADV			// vod暂停广告位
	, MIS_MENU_EPGADV				// 主菜单广告位
	, MIS_UNDEFINE_EPGADV			// 未定义的广告位
}CSMISEpgAdvType_E;				// 旗帜广告类型



typedef struct _CSMISAdvInfo_S {
	int nIndex;			// 旗帜广告类型
	int nWidth;			// 该类型广告所对应的宽，应参考局方规范
	int nHeight;		// 该类型广告所对应的高，应参考局方规范
} CSMISAdvInfo_S;

typedef struct _CSMISAdvConfig_S {
	int nAdvCount;		// 旗帜广告个数
	CSMISAdvInfo_S * psAdvConfigInfo;	// 旗帜广告配置信息
} CSMISAdvConfig_S;

/**
@brief  获取系统菜单透明度
@ 
*/
typedef int (*CSMISGetAlpha_F)(void);

typedef struct  _CSMISConfig_S
{	
	int nMisFreq;   			// 如加广告开机默认频点，频率(mHz),例如:443 mHz；如不关注该项，则设置为-1
	int nMisSymb;	 			// 默认符号率，符号率 (Ks/s)，如6875Ks/s；如不关注该项，则设置为-1
	int nMisModu;	 			// 默认模式，(1: 16Qam; 2: 32Qam; 3: 64Qam; 4: 128Qam; 5: 256Qam)；如不关注该项，则设置为-1
	int nMisOpenPid ;			// 默认开机应用PID；如不关注该项，则设置为-1
	int nMisNormalPid ; 		// 普通应用PID；如不关注该项，则设置为-1
	int nMisMoveInterval;		// 字幕刷新频率，取值范围-10~3。默认0,代表每40毫秒刷一帧。如加2.0库设置该项无效；如不关注该项，则设置为0
	int nMisMovePixel; 			// 字幕每帧移动像素数，取值范围-2~3。默认0，代表每帧移动三个像素；如不关注该项，则设置为0
	int nMisDefaultFromFreq; 	// 如加广告默认跳转频点,release库不关注该项，仅供debug库调试使用
	int nMisSwitchToFreq;		// 指定如加广告跳转的频点,release库不关注该项,仅供debug库调试使用
	int nMisShowIframeTime; 	// 如加广告I帧显示时长，单位：秒；如不关注该项，则设置为-1
	CSMISGetAlpha_F m_fnMisAlpha;	// 获取系统菜单透明度；如不关注该项，则设置为NULL
} CSMISConfig_S;  				



/**
@brief  获取保存数据的空间地址
@param  pcAddress:  用来保存数据空间的地址
@param  pnSize：数据大小不小于128字节
@return 0:表示设置成功；-1:表示设置失败
*/
typedef int (*CSMISNvramInfo_F)(char **pcAddress, int *pnSize);

/**
@brief  读取数据内容
@param  nFlashAddress:  读取数据起始地址
@param  pcBuffAddress：保存起始数据地址
@param  nBytes ：读取数据字节数
@return 0:表示设置成功；-1:表示设置失败
*/
typedef int (*CSMISNvramRead_F)(unsigned int nFlashAddress, char * pcBuffAddress, int nBytes);

/**
@brief  将数据写入保存数据的地址和DSM-CC缓冲区
@param  nFlashAddress:  保存数据的起始地址
@param  pcBuffAddress：写入数据块的起始地址
@param  nLength ：想要写入的字节数
@return 0:表示设置成功；-1:表示设置失败
*/
typedef int (*CSMISNvramBurn_F)(unsigned int nFlashAddress, const char * pcBuffAddress, int nLength);


typedef struct _CSMISNvramFunc_S 
{
	CSMISNvramInfo_F  m_fnInfo;
	CSMISNvramRead_F  m_fnRead;
	CSMISNvramBurn_F  m_fnBurn;
} CSMISNvramFunc_S;

typedef struct _CSMISFsPath_S
{
    char* psFsPath;
} CSMISFsPath_S;

typedef enum _CSMISOption_E
{
	EM_MIS_OPT_ADV_CONFIG = 1,	// 设置旗帜广告配置信息，pvValue是结构体CSMISAdvConfig_S地址
	EM_MIS_OPT_NVRAM_REG_FUNC = 2,  // 设置回调函数，pvValue是结构体CSMISNvramFunc_S地址
	EM_MIS_OPT_MIS_CONFIG = 3,	// 设置如加广告配置参数, pvValue是结构体CSMISConfig_S 的地址
	EM_MIS_OPT_FS_PATH = 4 // 设置如加开机图片保存路径（格式:"/workdir/app/JFFS/rojia"）
} CSMISOption_E;


/**
@brief  设置各个广告位相关配置信息

@param[in] eOption:  配置参数 结构体 CSMISOption_E 中有定义
@return TURE:表示设置成功；FALSE:表示设置失败
@note 该接口一般需要在调用如加广告初始化函数CSMISInit()之前调用。
*/
BOOL CSMISSetOption(CSMISOption_E eOption, void * pvValue);

/**
@brief 初始化如加广告系统

@return TRUE:初始化成功; FALSE:初始化失败
@note 该接口一般在系统初始化时调用
*/
BOOL CSMISInit(void);

/**
@brief 启动如加开机应用，该函数为阻塞模式，显示完开机应用后才可进行后续流程

@return TRUE:启动开机应用成功; FALSE:启动开机应用失败
@note 该接口一般在系统初始化时就需要调用，前置条件是CSMISInit已经调用过
*/
BOOL CSMISStartBootApp(void);

/**
@brief 分发遥控器按键给如加广告系统

@param[in] eKeyState:按键消息状态，如按下、弹起，具体参考参见udi2_inpu.h中CSUDIInputKeyStatus_E定义
@param[in] nMsg:按键值，参见udi2_inpu.h定义
@return TURE:表示如加广告系统独占按键，该按键不能再发送给中间件；FALSE:表示如加广告系统不处理此按键，该按键按正常流程处理，即发给中间件
@note 该接口一般在遥控器按键事件上抛给中间件的地方调用，流程参考如下：
-   if ( CS_MIS_DispatchInput(keystatus, psInputInfo->m_nKeyCode) == FALSE ) //如加不处理此按键消息
-		{
-			nRet = MMCP_EvtSendEvent(0, &event, NULL);
-		}
*/
BOOL CSMISDispatchInput(CSUDIInputKeyStatus_E eKeyState, int nMsg);

/**
@brief 设置存放默认广告图片的路径

@param[in] pcPathName:默认图片的路径
@return TURE:成功；FALSE:失败
@note 该接口一般在如加广告系统初始化之后立即调用;一般将该目录设置为I帧所在的目录
*/
BOOL CSMISSetAdImagePath(const char * pcPathName);

/**
@brief 	用户回调,mis把消息返回给用户

@param[in] enMsg:消息类型
@param[in] dwPara:参数
@return: 
@note:
enMsg为MIS_SHOW_ADV或MIS_HIDE_ADV时，dwPara为广告类型，详见枚举类型
*/
typedef void (* FOnCSMISCallback)(CSMISCallbackMessageType_E enMsg, DWORD dwPara);

/**
@brief 	注册回调函数

@param[in] callback:用户回调函数
@return TURE:成功；FALSE:失败
@note:
*/
BOOL CSMISRigisterCallback(FOnCSMISCallback callback);

/**
@brief 	获取旗帜广告id

@param[in] enType: 旗帜广告类型
@param[out] puId:旗帜广告id
@return TURE :成功；FALSE :失败
@note:
*/
BOOL CSMISGetEpgAdvId(CSMISEpgAdvType_E enType, UINT * puId);

#ifdef __cplusplus
}
#endif

#endif

