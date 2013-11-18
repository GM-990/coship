/**@defgroup PublicShellCA PublicShellCA 定义ShellCA公共对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_CA_PRIVATE_H
#define CSSHELL_CA_PRIVATE_H

#include "mmcp_typedef.h"
#include "csshell_ca.h"
#include "cs_mediaplayer.h"

#ifdef __cplusplus
extern "C"
{
#endif
typedef struct _CSShellCaInitParam
{  
	int                   m_nSize;    ///< 本结构体的大小，
		CSShellCaType_E         m_eCaType;       ///< CA类型，

	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< 获取智能卡卡号接口			
}CSShellCaInitParam_S;

/*ca 卡状态改变枚举*/
typedef enum _CSShellCaCardChangedEvent
{
	EM_CSSHELL_CARD_VALID = 0,	///<卡有效，插入的卡被正确读取才是有效的
	EM_CSSHELL_CARD_INVALID,	///<卡无效，卡拔出，插入错误卡
	EM_CSSHELL_CARD_PREPARING ///<卡已经插入，但是还没有获取到卡号，适用于异步获取卡号，并且需要时间较长的CA，如irdeto
}CSShellCaCardChangedEvent_E;

	/*最多允许向每个ca注册8个获取卡交换信息的回调*/
#define CSSHELL_CA_MAX_CALLBACK_COUNT		(8)
	
typedef enum
{
	EM_CSSHELL_CA_REG_CALLBACK_SUCCESS = 0,	///< 注册回调函数成功
	EM_CSSHELL_CA_REG_CALLBACK_REPEAT , ///< 重复注册回调函数，根据userData为关键字判断
	EM_CSSHELL_CA_REG_CALLBACK_OVERFLOW, ///< 注册的回调函数已经达到最大个数，无法注册成功		
	EM_CSSHELL_CA_DEL_CALLBACK_SUCCESS,	///< 删除回调函数成功
	EM_CSSHELL_CA_DEL_CALLBACK_FAILURE	///< 删除回调函数失败
}CSShellCaRegCallbackErrorCode_E;


typedef DWORD (*CSShellGetFlashAddr_F)(DWORD dwAddress); ///<CA依赖接口，为tf3、st公共依赖
typedef DWORD (*CSShellGetBakFlashAddr_F)(DWORD dwBakAddress);///<CA依赖接口，为tf3、dvt公共依赖
typedef DWORD (*CSShellGetSoftwareVersion_F)(char* chSWVision, int nsizeSW); ///<CA依赖接口，为st、stcti公共依赖
typedef DWORD (*CSShellGetMailFlashAddress_F)(void);///<CA依赖接口，为st、stcti公共依赖
typedef DWORD (*CSShellGetHardwareVersion_F)(char* chHWVision, int nsizeHW); ///<CA依赖接口，为st、stcti公共依赖

/**
@brief 各个ca 公共依赖的接口
@note 这里只列出了各个ca都需要实现的公共接口。对于类似NDS CA需要实现而TF3 CA不需要实现的依赖接口，则放在csshelL_ndsca.c中实现；
为什么这样做呢? ca公共依赖接口，如果在每个ca shell中都实现，则会有重定义的问题。只能以注册方式实现。非公共依赖接口则不存在此问题
*/
typedef struct _CSShellCaDependence
{  
	CSShellGetFlashAddr_F	m_fnGetFlashAddress;///<flash地址
	CSShellGetBakFlashAddr_F m_fnGetBakFlashAddress;///<flash的备份地址
	CSShellGetSoftwareVersion_F m_fnGetSoftwareVersion;
	CSShellGetHardwareVersion_F m_fnGetHardwareVersion;
	CSShellGetMailFlashAddress_F m_fnGetMailFlashAddress;
}CSShellCaDependence_S;

/**
@brief 设置对应ca依赖的接口
@param psDependence 依赖接口
@return 成功返回TRUE,失败返回FALSE
@note 该函数由具体ca进行设置，指定自己需要调用的接口。当C层CA调用依赖的接口时，可以调用到指定ca的依赖接口。
	增加该接口的目的在于避免各个CA中依赖的接口因为同名同形导致的重定义
*/
BOOL CSShellCaSetCommonDependence(CSShellCaDependence_S* psDependence);

/**
@brief 设置ca参数
@param psParam a参数
@return 成功返回TRUE,失败返回FALSE
@note 该函数由具体ca中的 :CSShell***CaSetParam调用，psParam也就是其传入参数。而不是由应用调用的。
	调用关系: app-->CSShell***CaSetParam-->CSShellCaSetParam
	公共ca模块只负责记录psParam的值即可无需拷贝赋值*psParam
*/
BOOL CSShellCaSetParam(CSShellCaInitParam_S* psParam);

/**
@brief 获取ca参数
@param 无
@return ca参数
@note 该函数由dtvmx native调用。ca初始化时，先调用CSShellCaGetParam获取ca参数，然后根据该参数中类型type判断是否为要初始化的ca，如果是，则初始化
	调用关系: init***Ca(java)-->CSShellCaGetParam-->if(ca类型正确)-->初始化***ca
*/	
const CSShellCaInitParam_S* CSShellCaGetParam();

/**
@brief 设置ca配置
@param Ca配置
@return 成功返回TRUE,失败返回FALSE
@note 该函数由具体ca中的 :CSShell***CaSetConfig调用，psConfig也就是其传入参数。
	瞔a模块只负责记录psConfig的值即可无需拷贝赋值*psConfig
*/
BOOL CSShellCaSetConfig(void* pvConfig);

/**
@brief 获取ca配置
@param 无
@return ca配置
@note 该函数由dtvmx native调用。ca初始化时，可调用CSShellCaGetConfig获取Ca配置.返回参数为void*类型，要转化为正确的ca参数
	调用关系: init***Ca(java)-->CSShellCaSetConfig-->if(ca类型正确)-->初始化***ca
*/		
const void* CSShellCaGetConfig();

/**
@brief	智能卡状态变化回调函数原型。
@param  dwUserData:用户数据
@param  eCardStatus:smartcard的状态
@param  dwPara1:状态参数1，一般用于传卡号
@param   dwPara2:状态参数2，预留
@return  成功:0,失败:其他
@note 该接口与具体ca无关。但每个ca都必须支持在插拔卡时发出该回调。
*******************************************************************************/

typedef int (*OnCSShellCaCardChanged_F)(DWORD nUserData,CSShellCaCardChangedEvent_E eCardStatus,DWORD dwPara1,DWORD dwPara2);


/******************************************************************************
Function Name:    CSShellCaAddCardChangedCallback
	
Description:
	增加smc状态变化的侦听
Input:
	dwUserData:用户数据
	fnCallback:回调函数
Output:
    None

Return Value:
    返回 CSShellCaRegCallbackErrorCode_E
Other:
	该函数支持多次注册。但是不支持重复注册(以userData为关键字)。
*******************************************************************************/
CSShellCaRegCallbackErrorCode_E CSShellCaAddCardChangedCallback(DWORD dwUserData,OnCSShellCaCardChanged_F fnCallback);
/******************************************************************************
Function Name:    CSShellCaRemoveCardChangedCallback
	
Description:
	删除smc状态变化的侦听
Input:
	dwUserData:用户数据
	fnCallback:回调函数
Output:
    None

Return Value:
   返回 CSShellCaRegCallbackErrorCode_E
Other
	:
*******************************************************************************/
CSShellCaRegCallbackErrorCode_E CSShellCaRemoveCardChangedCallback(DWORD dwUserData,OnCSShellCaCardChanged_F fnCallback);

/******************************************************************************
Function Name:    CSShellCaGetCardChangedCallback
	
Description:
	根据index获取注册的smc监听变化回调
Input:
	nIndex:指定的索引值	
Output:
    pdwUserData:输出对应索引的回调的用户数据
    pfnCallback:输出对应索引的回调的回调函数

Return Value:
    成功返回true，失败返回false
Other:
*******************************************************************************/
void CSShellCaProcessCallback(CSShellCaCardChangedEvent_E eCardStatus, int nPara1, int nPara2);
	
/******************************************************************************
Function Name:    CSShellCaGetCardChangedCallback
	
Description:
	根据index获取注册的smc监听变化回调
Input:
	nIndex:指定的索引值	
Output:
    pdwUserData:输出对应索引的回调的用户数据
    pfnCallback:输出对应索引的回调的回调函数

Return Value:
    成功返回true，失败返回false
Other:
*******************************************************************************/
BOOL CSShellCaGetCardChangedCallback(int nIndex, DWORD * pdwUserData, OnCSShellCaCardChanged_F * pfnCallback);

/******************************************************************************
@brief 设置ca配置参数
@param pvConfig ca参数
@return 成功返回TRUE,失败返回FALSE
@note 该函数由具体ca中的 :CSShell***CaSetConfig调用，psConfig也就是其传入参数。而不是由应用调用的。
	调用关系: app-->CSShell***CaSetConfig-->CSShellCaSetConfig
	公共ca模块只负责记录psConfig的值即可无需拷贝赋值*pvConfig
*/
BOOL CSShellCaSetConfig(void* pvConfig);

/******************************************************************************
@brief 获取ca卡状态
@param 无
@return 返回ca卡的状态
@note 系统引导时，各个ca提供的获取卡初始状态的方法不一致。但是，都提供了获取卡状态的方法。具体ca获取到卡状态后，通知公共ca接口CSShellCaProcessCallback
	记录下ca卡状态。这样，应用可以不关注具体ca而查询卡的状态
*******************************************************************************/
CSShellCaCardChangedEvent_E CSShellCaGetCardStatus(void);

/******************************************************************************
@brief 获取当前的mediaplayer句柄
@param 无
@return Mediaplayer句柄
*******************************************************************************/
HCSHANDLE CSShellCaGetMPHandle();

/******************************************************************************
@brief 将智能卡号设置给环境变量:SmartCard.serialNumber
@param pcSmcId,智能卡号
@return 成功设置返回TRUE,否则返回FALSE
@note 设置时会过滤卡号中的无效空格，仅保留数字代码
*******************************************************************************/
BOOL CSShellCaSetSmcIdToEnv(char* pcSmcId, int nMaxLen);


/*******************************************************************************
@brief 获取dtvm ca native关闭或打开音视频的权限设置
@param 无
@return 允许dtvm ca native关闭音视频为TRUE,否则为FALSE
@note 该接口是为了兼容天威项目之前在dtvm ca native中控制音视频而添加；默认为 FALSE
@note 仅建议天威项目使用，设为TRUE;其他项目建议不要调用该接口
@note 目前该设置仅能在dtvm nds ca native中调用，其他ca不要调用
*******************************************************************************/
BOOL CSShellCaGetControlAV();


#ifdef __cplusplus
}
#endif
/** @} */
#endif   



