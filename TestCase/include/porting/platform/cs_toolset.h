/** @addtogroup CSTOOLSET
@{
*/
#ifndef _CS_TOOLSET_H_
#define _CS_TOOLSET_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "cs_sysvideo.h"
#include "cs_sysaudio.h"
#include "udi_toolset.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 配置系统内存分配，针对ST71XX 和ST5197系列 的OS21操作系统*/
typedef struct _CSSYSAdvancedParam
{
	BOOL				m_bSupportHDDecoder;		/**< 配置是否支持MPEG-2 MP@HL*/
	BOOL				m_bDualOSDBuffer;			/**<单缓存,双缓存，默认为双缓存(指同时支持单缓存，双缓存的环境)	*/

	DWORD				m_dwMemoryStartAddr;		/**< 配置物理内存起始地址*/
	DWORD				m_dwMemoryTotalSize;		/**< 物理内存的大小*/

	DWORD*				m_pdwMemCacheStartAddr;		/**< Ignore,No used*/
	DWORD*				m_pdwMemCacheEndAddr;		/**< Ignore,No used*/
	DWORD*				m_pdwMemCacheCount;			/**< Ignore,No used*/
	
	DWORD*				m_pdwAVMemPartitionStartAddr;	/**< Ignore,No used*/
	DWORD*				m_pdwAVMemPartitionEndAddr;		/**< Ignore,No used*/
	DWORD*				m_pdwAVMemPartitionCount;		/**< Ignore,No used*/
	BOOL 				m_bSupportHDDecoder_H264;		/**< 配置是否支持 AVC HP@L4.1*/
} CSSYSAdvancedParam;

/**@brief 配置系统内存分配，针对ST71XX 和ST5197系列 的OS21操作系统*/
typedef struct _CSSYSAdvancedParamEx
{
	CSSYSAdvancedParam  m_stSysAdvancedParam;	/**<CSSYSAdvancedParam */
	BOOL  				m_bPvrSupport;     		/**<PVR application support*/
	BOOL  				m_bPipSupport;     		/**<PIP application support*/
	BYTE  	       		m_ucPtiNumber;     		/**<multi pti support*/
	DWORD				m_dwReserve;			/**<0---不支持NDS CA,1---支持NDS CA*/
 } CSSYSAdvancedParamEx;
 
/**@brief 配置系统内存分配，针对ST71XX 和ST5197系列 的OS21操作系统*/
typedef struct _CSBootParamEx
{
	BOOL				m_bDualOSDBuffer;		/**<单缓存,双缓存，默认为双缓存(指同时支持单缓存，双缓存的环境)	*/
	DWORD				m_dwColorSize;			/**<8位色，16位色，默认为8位色	*/
	DWORD				m_dwMemoryTotalSize;	/**<默认为8m内存*/
}CSBootParamEx;


/********************系统初始化和配置***********************/
/**
@brief 移植层配置接口，调用时机由各eCfgType决定,具体见CSSYSCfg_E定义

该接口可被调用多次，同一类型的配置以最后一次的调用为准
@param[in] eCfgType 配置类型,具体见CSSYSCfg_E定义;
@param[in] dwDevice 所要配置的设备ID,见cs_device.h,如果该eCfgType无对应的设备时,传入CSDEVICE_IGNORE;
@param[in] dwValue  配置参数,具体含义由eCfgType决定;
@param[in] dwRserved 保留参数,目前不使用，必须为NULL。
@return 配置成功返回TRUE ，配置失败返回FALSE
*/
BOOL CSSYSSetCfg(CSSYSCfg_E eCfgType, DWORD dwDevice, DWORD dwValue,DWORD dwRserved);

/**
@brief 移植层初始化接口，实现系统的分区划分，并启动操作系统，系统启动时第一个被调用

@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，必须NULL。
@return 配置成功返回TRUE ，配置失败返回FALSE
*/
BOOL CSSYSChipBoot(HCSHANDLE hUserData);

/**
@brief 移植层系统初始化接口

该接口只可被调用一次，必须在CSSYSChipBoot或CSSYSChipBootEX后调用
@param[in] pstSysInfo 由应用配置的STB的系统信息，具体结构参看CSSYSInfo的数据结构。
@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，必须为NULL。
@return 设备链句柄
@note 
- 初始化机顶盒设备链，根据芯片的不同特性分配clkrv,dma, audio, video, osd, pti etc.的资源；
	并将把底层设备链支持的STB的服务类型（DVB 或 IPTV），音频、视屏输入格式配置；
	音、视频输出格式配置。通过该函数接口通知底层保存下来。在I2c,uart,gpio等设备初
	始化之后调用.用户不需要关心各设备的初始化顺序。对于参数pstSysInfo，上层应用
	如果不确认具体参数内容，可将该参数置NULL，移植组此时采用预置的缺省值，
	移植层要保证对NULL输入的处理
*/
HCSHANDLE CSSYSChipInit(CSSYSInfo* pstSysInfo, HCSHANDLE hUserData);

/**
@brief 移植层系统初始化接口，CSSYSChipInit的扩展

该接口只可被调用一次，必须在CSSYSChipBoot或CSSYSChipBootEX后调用
@param[in] pstSysInfo 由应用配置的STB的系统信息，具体结构参看CSSYSInfo的数据结构。
@param[in] hUserData 该参数可以为NULL也可以为CSSYSAdvancedParamEx
@return 设备链句柄
@note 
- 初始化机顶盒设备链，根据芯片的不同特性分配clkrv,dma, audio, video, osd, pti etc.的资源；
	并将把底层设备链支持的STB的服务类型（DVB 或 IPTV），音频、视屏输入格式配置；
	音、视频输出格式配置。通过该函数接口通知底层保存下来。在I2c,uart,gpio等设备初
	始化之后调用.用户不需要关心各设备的初始化顺序。对于参数pstSysInfo，上层应用
	如果不确认具体参数内容，可将该参数置NULL，移植组此时采用预置的缺省值，
	移植层要保证对NULL输入的处理
*/
HCSHANDLE CSSYSChipInitEx(CSSYSInfo* pstSysInfo, HCSHANDLE hUserData);

/**
@brief 设置驱动是否支持PVR
@param[in] bSupport  TRUE表示支持,FALSE表示不支持
@return 配置成功返回TRUE ，配置失败返回FALSE
*/
BOOL CSSYSSetPvrSupport(BOOL bSupport);

/**
@brief 自定义CSSYSEventMsg事件及优先级

当CSSYSEventMsg定义的事件或者优先级不满足需求时,使用该接口进行自定义注册
事件优先级按其在数组中的先后顺序由高到低
如果CSSYSEventMsg定义的事件及优先级均满足要求时,无需调用本接口
仅在平台相关初始化完毕以后调用一次，传入要注册的各种事件类型及相关属性
@param[in] hHandle 需自定义事件及优先级的音视频设备或设备链句柄
@param[in] pstItems 事件类型(项)的数组起始地址，
@param[in] nCount 数组元素个数
*/
BOOL CSSYSInitEventStatusTable(HCSHANDLE hHandle, CSSYSEventItem * pstItems,  int nCount);

/**
@brief 获得传入设备句柄在设备句柄链的下一个合法设备句柄

@param[in] hInstance NULL 或者 CSSYSChipInit所创建的句柄
@return 设备链句柄
@note 如果传入为NULL，则返回设备句柄链的头句柄。如果没有建立句柄链，则返回NULL
@note 如果传入一个合法句柄，如果句柄链有下一个合法句柄，则返回该句柄；否者返回NULL。
@note 对RM支持的情况有用
*/
HCSHANDLE CSSYSGetNextHandle(HCSHANDLE hInstance);

/**
@brief 获得移植层平台接口的版本信息

包括ES,以及video，audio的各种信息
@param[out] pcVersionInfo 系统当前的移植层平台接口的版本信息
@param[in] nSize 显示字符串的最大长度
@return 成功:当前版本的长度;失败:0
@note 输出格式为以下几种形式之一：
- "PLAT-R-主版本.子版本.改动次数 ";Release Version 
- "PLAT--主版本.子版本.改动次数";  Debug Version
- "PLAT--主版本.子版本.改动次数";  Test Version 
*/
int  CSSYSGetPlatformVersion( char* pcVersionInfo, int nSize);

/**
@brief 获得当前芯片的版本驱动信息

包括ES,以及video，audio的各种信息
@param[out] pcVersionInfo 系统当前的芯片的版本信息
@param[in] nSize 当前芯片版本号的长度
@return 成功:当前版本的长度;失败:0
@note 由于平台提供商底层驱动模块不断有更新，平台需要记录驱动模块的更新状态，提供这个接口来获得系统当前的芯片的版本号修改情况
*/
int  CSSYSGetChipVersion( char* pcVersionInfo, int nSize);

/*-------------下面定义只是为了兼容之前驱动平台,不需要实现-------------*/
/**
@brief 移植层初始化接口，CSSYSChipBoot的扩展接口，ST系列 OS21版本需要实现，其它方案实现为桩函数即可
@param[in] pBootParam 配置参数
@param[in] hUserData 该参数保留，以便日后的功能扩展，目前不使用，必须NULL。
@return 配置成功返回TRUE ，配置失败返回FALSE
*/
BOOL CSSYSChipBootEx(CSBootParamEx* pBootParam,HCSHANDLE hUserData);

/**
@brief Enable WatchDog，不需要实现
@note 不需要实现
*/
void CSWatchDogEnable( void );

/**
@brief 	Disable WatchDog，不需要实现
@note 不需要实现
*/
void CSWatchDogDisable( void );

/**
@brief 	Set WatchDog Reset Time in Seconds，不需要实现
@note 不需要实现
*/
void CSWatchDogSetResetTime( DWORD dwResetTimeSec );

/**
@brief 获得传入设备句柄在设备句柄链的下一个合法设备句柄，不需要实现

@param[in] hHandle 设备通道句柄，系统初始化得到；
@param[in] bToBeLimited 是否限制cpu的带宽
@param[in] hUserData 保留参数，必须为NULL
@return 配置成功返回TRUE ，配置失败返回FALSE
@note 不需要实现
*/
BOOL CSSYSLimitBandWidthOfCPU(HCSHANDLE hHandle,BOOL bToBeLimited,HCSHANDLE hUserData);

/**
@brief 	针对ST710X，驱动播放mp3的功能需要调用，不需要实现
@note 不需要实现
*/
BOOL CSSYSInitSoftMp3(HCSHANDLE hUserData);

/**
@brief 芯片待机的钩子函数定义
@param[in] bStandbyStatus		TRUE:进入待机状态;FALSE:退出待机状态
@param[in] pstStandbyParams	 见CSSYSStandbyParams_S定义
@param[out] pbSkipDefault 是否忽略默认操作，TRUE表示只执行Hook函数定义的操作，FALSE表示同时执行CSSYSChipStandby原有操作，默认为TRUE
@return TRUE 表示钩子函数执行成功，FALSE表示钩子函数执行失败
*/
typedef BOOL (*CSChipStandbyHookFunc_F)(BOOL bStandbyStatus, const CSSYSStandbyParams_S* pstStandbyParams, BOOL *pbSkipDefault);

/**
@brief 设置芯片待机的钩子函数

@param[in] fnHook 钩子函数，见CSChipStandbyHookFunc_F定义
@return 如果调用成功，返回调用TRUE；如果调用失败则返回FALSE
@note fnHook可以为NULL，表示取消钩子函数
*/
BOOL CSSYSSetChipStandbyHook(CSChipStandbyHookFunc_F fnHook); 


/**
@brief 获取主芯片的ID
@param[out] pdwChipID 主芯片的id。
@return 如果调用成功，返回调用TRUE；如果调用失败则返回FALSE
*/
BOOL CSSYSGetChipID (DWORD* pdwChipID);

/**
@brief 获取主芯片的ID
@param[out] pdwChipID 主芯片的id。
@param[out] pdwChipIDEx    对于高级安全需要更多位的主芯片的id。
@return 如果调用成功，返回调用TRUE；如果调用失败则返回FALSE
*/
BOOL CSSYSGetChipIDEx(DWORD* pdwChipID,DWORD* pdwChipIDEx);

#ifdef  __cplusplus
}
#endif

#endif

/** @} */
