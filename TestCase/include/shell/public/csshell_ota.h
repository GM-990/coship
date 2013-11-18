#ifndef CSSHELL_OTA_H
#define CSSHELL_OTA_H
#include "mmcp_typedef.h"

#include "CSUSPDMCommonType.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define OTA_DELIVER_COUNT				(0X8)

/**
@brief ota类型枚举
@note 这里只是定了几种常见的ota标准类型，并不梦满足所有项目需求。项目可以定义自己的ota类型。
项目定义ota类型时一定要注意:
1.对于ota类型的解释，一定要和java层保持一致
2.自定义ota类型值一定要大于EM_CSSHELL_OTA_TYPE_USERSTART
@note 这里的ota类型与java类OTAType有区别。java类OTAType命名并不准确，由于历史原因保留
*/
typedef enum
{
    EM_CSSHELL_OTA_TYPE_NDS = 1,
    EM_CSSHELL_OTA_TYPE_IRDETO,
    EM_CSSHELL_OTA_TYPE_TF,
    EM_CSSHELL_OTA_TYPE_COSHIP,
    EM_CSSHELL_OTA_TYPE_OTHER,
    EM_CSSHELL_OTA_TYPE_COMVENIENT_CA,
    EM_CSSHELL_OTA_TYPE_FTASAT,
    EM_CSSHELL_OTA_TYPE_FTATER,

    EM_CSSHELL_OTA_TYPE_USERSTART = 0x100
}CSShellOtaType_E;

/**
@brief ota消息意义定义
@note 底层ota与java层ota进行通信，依靠这些ota消息。
@note 当底层C代码检测到ota升级信息时，通过调用MMCP_EvtBroadcastEvent，广播类型为以下枚举值的MMCP_EvtEventEntry消息，将这些消息传递给java层。
@code 检测到了强制升级的ota信息时，需要广播消息通知java应用:

	MMCP_EvtEventEntry sEventEntry = {0};

	sEventEntry.type = EVT_OTA_EVENT;
	sEventEntry.code = EM_CSSHELL_OTA_MESSAGE_FORCE;
	sEventEntry.data = (void  *)0;	//该参数自己定义，java层获取到之后按照，案中案项目解析
	nRet = MMCP_EvtBroadcastEvent(&sEventEntry, NULL);

*/
typedef enum
{
    EM_CSSHELL_OTA_MESSAGE_FORCE = 0,
    EM_CSSHELL_OTA_MESSAGE_NORMAL ,
    EM_CSSHELL_OTA_MESSAGE_REFRESHLOADER,
    EM_CSSHELL_OTA_MESSAGE_CANCEL,
    EM_CSSHELL_OTA_MESSAGE_MANUAL
} CSShellOtaMessageType_E;

/**
@brief Comvenient CA OTA 升级所需参数
@aDelivers 升级频点列表
@nDeliverCount 升级频点的个数
@bDownUpgrade 是否支持低版本升级
*/
typedef struct _CSSHELLOTA_ComvenientCAStartParam_S
{
    Deliver aDelivers[OTA_DELIVER_COUNT];
    int nDeliverCount;
    BOOL bDownUpgrade;
} CSSHELLOTA_ComvenientCAStartParam_S;

/**
@brief FTA的卫星OTA 升级所需参数
@nSatIndex 卫星索引
@nTPIndex  TP索引
@nDownLoadPID 升级所需的DownLoadPID
@nTableID 升级所需的TableID
*/
typedef struct _CSSHELLOTA_FtaSatStartParam_S
{
    int nSatIndex;
    int nTPIndex;
    int nDownLoadPID;
    int nTableID;
} CSSHELLOTA_FtaSatStartParam_S;

/**
@brief FTA的地面OTA 升级所需参数
@aDelivers 升级频点列表
@nDeliverCount  升级频点的个数
@nDownLoadPID 升级所需的DownLoadPID
@nTableID 升级所需的TableID
*/
typedef struct _CSSHELLOTA_FtaTerStartParam_S
{
    Deliver aDelivers[OTA_DELIVER_COUNT];
    int nDeliverCount;
    int nDownLoadPID;
    int nTableID;
} CSSHELLOTA_FtaTerStartParam_S;
/*
*@brief ota初始化参数,定义了一系列函数指针，这些函数指针由应用的ota实现(C代码)，然后注册给shell。每个函数指针对应OTAInvoke中一个接口
@see OTAInvoke*/
typedef struct _CSShellOtaInitParam
{
	int m_nSize;	///<本结构大小，一定等于sizeof(CSShellOtaInitParam_S)
	CSShellOtaType_E m_eType;	//ota的类型
	/**
	@brief 开始启用ota升级功能,与OTAInvoke.start对应.
	@note 调用该接口后，仅可检测码流中的升级信息。检测到升级信息之后要发消息通知给应用。在下次启动时由loader实施应用写入flash的升级。
	@note loader下载升级应用并写入flash实现升级的过程不是该接口所关心的。
	*/
	void (* m_fnStart)();

	/**
	@brief 暂停ota升级,与OTAInvoke.userwait对应
	*/
	void (* m_fnUserWait)();

	/**
	@brief 设置待机状态,与OTAInvoke.setStandByStatus对应
	@param bStandby 待机：1  不待机：0
	@return 成功：0 失败：-1
	*/
	int (* m_fnSetStandbyStatus)(BOOL bStandby);

	/**
	@brief 获取loader状态,与OTAInvoke.getLoaderStatus对应
	@return loader的状态。并不是所有ota都关心loader状态，如果不关心可以直接返回-1，也可以注册时设置m_fnGetLoaderStatus为NULL；
	如果关心的话，状态值代表的意义由具体ota决定。
	@note 注意状态值的含义，即一个具体整数代表什么状态，要与OTAInvoke.getLoaderStatus返回值保持一致
	*/
	int (* m_fnGetLoaderStatus)();

	/**
	@brief 进入手动OTA升级,与OTAInvoke.enterManualOTA对应
	*/
	void (* m_fnEnterManualOta)(int nFreq,int nSymbol,int nModulation);

	/**
	@brief 获取OTA名称,与OTAInvoke.getOTAName对应
	*/
	const char* (* m_fnGetOtaName)();

	/**
	@brief判断是否有升级信息,与OTAInvoke.hasUpdateInfo对应
	@return 有升级：TRUE   无升级：FALSE
	*/
	BOOL (*m_fnHasUpdateInfo)();

	/**
	@brief获取待机状态
	@return 待机状态：TRUE 非待机状态：FALSE
	@note nds需求，在假待机状态也能ota，ota完毕后重启，由于之前是待机状态，重启之后也要立刻转为待机状态。应用上需要获取待机状态才能进行此操作。
	@note 只对假待机有效。除了不支持真待机的盒子外，nds还要求，应用上检测到升级标志并提示后，如果用户按下取消键，之后用户再按下待机键，也要假待机。
	*/
	BOOL (* m_fnGetStandbyStatus)();

	 /**
	 @brief启动USB升级
	 @return 启动成功：TRUE 启动失败返回：FALSE
	 @pcPathName:表示待升级的文件所在的路径和名称
	 */
	BOOL (* m_fnStartUSBUpgrade)(const char *pcPathName);

	 /**
	 @brief启动USB下载
	 @return 启动成功返回：TRUE 启动失败返回：FALSE
	 */
	BOOL (*m_fnStartUSBDownLoad)();
	/**
	@brief 进入手强制进入Loader进行OTA升级
	*/
	
	void (* m_fnEnterForceOta)(int nFreq,int nSymbol,int nModulation, int nPid, int nTableId);
	/**
	@brief 设置启动参数
	@param pParam
					OTAType为EM_CSSHELL_OTA_TYPE_COMVENIENT_CA 时，pParam为CSSHELLOTA_ComvenientCAStartParam_S*
					OTAType为EM_CSSHELL_OTA_TYPE_FTASAT 时，pParam为CSSHELLOTA_FtaSatStartParam_S*
					OTAType为EM_CSSHELL_OTA_TYPE_FTATER 时，pParam为CSSHELLOTA_FtaTerStartParam_S*
	*/
	BOOL (* m_fnSetStartParam)(void* pParam);

 	/**
     @brief启动OTA下载
     @return 启动成功返回：TRUE 启动失败返回：FALSE
     @note 中间件修改flash中的Booter数据，将该方法输入的参数信息写入flash供loader模块使用。
     */
	BOOL (*m_fnStartGDOTA)(int nFrequency,int nSymbolRate, int nModulation,DWORD nServiceID, int nManu_code, int nMajor_hw_version,int nMinor_hw_version,int nHw_batch_no);

	/**
     @brief中间件通知机顶盒修改flash中的Booter数据，写入完成后，中间件无需重启机顶盒。待机顶盒下次启动时，直接进入loader模块进行OTA升级。
     @return 启动成功返回：TRUE 启动失败返回：FALSE
     @note 
     */
	BOOL (*m_fnStartLater)(void);
	
	/**
     @brief启动网络升级
     @param pcURL: 升级软件的URL地址 nURLLen: URL字符串长度
     @return 启动成功返回：TRUE 启动失败返回：FALSE
     @note 
     */
	BOOL (*m_fnStartInternetUpgrade)(char *pcURL, int nURLLen);

}CSShellOtaInitParam_S;


/**
@brief 设置ota升级接口
@param psParam ota升级shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSShellOtaSetParam(CSShellOtaInitParam_S * psParam);


#ifdef __cplusplus
}
#endif

#endif

