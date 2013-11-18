/**@defgroup CSCOFDM [CSCOFDM] DVB-T标准的 编码正交频分复用(coded orthogonal frequency division multiplexing)驱动，
@brief 定义cofdm驱动的锁频和取得信号信息等。
 
@version 0.3 2008/11/5  第一次细化版本
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/


#ifndef _CS_COFDM_H_
#define _CS_COFDM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cs_typedef.h"
#include "CSHDI_typedef.h"
#include "udi_tuner.h"

/**@brief cofdm 接口返回值定义*/
typedef enum
{
	CSCOFDM_SUCCESS,						/**< 操作成功*/
	CSCOFDM_FAILURE,						/**< 操作失败*/
	CSCOFDM_HAS_INITIALIZED,		/**< 模块已经被初始化过一次*/
	CSCOFDM_INVALID_PARAM, 			/**< 参数错误*/
	CSCOFDM_DEVICE_UNKNOWN,			/**< 未知设备，比如无效的CSHDITunerIndex*/
	CSCOFDM_I2C_ERROR,					/**< 操作时I2C访问出错*/
	CSCOFDM_GPIO_ERROR,					/**< 操作时gpio访问出错*/
	CSCOFDM_SYSTEM_ERROR,				/**< 未知的系统错误*/
	CSCOFDM_DROPPED,						/**< 信号丢失*/
	CSCOFDM_STOP,								/**< 搜索停止*/
	CSCOFDM_NOT_INITIALIZED			/**< 对应的CSHDITunerIndex未初始化*/
}CSCOFDM_Error_t; 

/**@brief cofdm 信号传输模式枚举*/
typedef enum _T_TransMode
{
	FFT_2K,		 			/**< FFT 2K搜索模式*/
	FFT_8K,		  		/**< FFT 8K搜索模式*/
	FFT_AUTO  			/**< FFT 自动搜索*/
}T_TransMode;

/**@brief cofdm 信号Guard模式枚举*/
typedef enum _T_GuardMode
{
	GUARD_1_32,						/**< GUARD 模式为1/32*/
	GUARD_1_16,						/**< GUARD 模式为1/16*/
	GUARD_1_8,						/**< GUARD 模式为1/8*/
	GUARD_1_4,						/**< GUARD 模式为1/4*/
	GUARD_AUTO   					/**< GUARD 自动搜索*/
}T_GuardMode;

/**@brief cofdm 信号I/Q模式枚举*/
typedef enum _T_IQMode
{
	IQ_NORMAL,   					/**< 模式为 I/Q*/
	IQ_CHANGED, 	 				/**< 模式为 I/-Q*/
	IQ_AUTO     					/**< I/Q自动搜索*/
}T_IQMode;

/**@brief cofdm 信号CodeRate枚举*/
typedef enum _T_CodeRate
{
	CODERATE_1_2, 				/**< CodeRate为1/2*/
	CODERATE_2_3, 				/**< CodeRate为2/3*/
	CODERATE_3_4, 				/**< CodeRate为/3/4*/
	CODERATE_5_6, 				/**< CodeRate为5/6*/
	CODERATE_7_8, 				/**< CodeRate为7/8*/
	CODERATE_AUTO 				/**< CODERATE自动搜索*/
}T_CodeRate;

/**@brief cofdm 信号星座图Constellation枚举*/
typedef enum _T_Constellation
{
	QPSK_MODE,   					/**< 星座图为QPSK模式*/
	QAM16_MODE,  					/**< 星座图为QAM16模式*/
	QAM64_MODE,  					/**< 星座图QAM64模式*/
	MODULATION_AUTO	 			/**< 星座图自动搜索*/
}T_Constellation;

/**@brief cofdm 驱动回调事件类型枚举*/
typedef enum							
{
  COFDM_CALLBACK_SIGNAL_CONNECTED,         	/**< 信号锁定*/
  COFDM_CALLBACK_SIGNAL_DROPPED,          	/**< 人为使信号丢失*/
  COFDM_CALLBACK_SIGNAL_LOST,             	/**< 信号丢失*/
  COFDM_CALLBACK_SIGNAL_SEARCHING						/**< 正在搜索*/	
}T_COFDMCallbackEvents;

/**@brief Heirarchy枚举*/
typedef enum _T_Heirarchy
{
	ALPHA_NONE,									/**< 待补充*/	
	ALPHA_1,										/**< 待补充*/	
	ALPHA_2,										/**< 待补充*/	
	ALPHA_4,										/**< 待补充*/	
	ALPHA_AUTO									/**< 待补充*/	
}T_Heirarchy;

/**@brief 优先级枚举*/
typedef enum _T_HPLP
{
	PRIORITY_HIGH,  	/**< 高优先*/
	PRIORITY_LOW,   	/**< 低优先*/
	PRIORITY_AUTO  		/**< 自动搜索*/
}T_HPLP;

/**@brief 搜索类型枚举*/
typedef enum _T_SeachType
{
	PLAY_SIGNAL, 			/**< 播放模式搜索*/
	SCAN_SIGNAL  			/**< 自动模式搜索*/
}T_SearchType;


/**@brief 无线信号调制方式枚举*/
typedef enum
{
	EM_DMB_TH_MODE_AUTO, 					/**< 自动模式*/
	EM_DMB_TH_MODE_4QAM, 					/**< 4QAM*/
	EM_DMB_TH_MODE_4QAM_NR, 			/**< 4QAM_NR*/
	EM_DMB_TH_MODE_16QAM, 				/**< 16QAM*/
	EM_DMB_TH_MODE_32QAM, 				/**< 32QAM*/
	EM_DMB_TH_MODE_64QAM 					/**< 64QAM*/
}DMB_TH_MODE_E;

/**@brief DMB载波方式枚举*/
typedef enum
{
	EM_DMB_TH_SM_AUTO,    				/**<	自动*/
	EM_DMB_TH_SM_SINGLE, 					/**<	单载波*/
	EM_DMB_TH_SM_MULTI  					/**<	多载波*/
}DMB_TH_SM_E;

/**@brief DMB PN方式枚举*/
typedef enum
{
	EM_DMB_TH_PN_AUTO,    				/**<	自动*/
	EM_DMB_TH_PN_420,    					/**<	420*/
	EM_DMB_TH_PN_595,    					/**<	595*/
	EM_DMB_TH_PN_945    					/**<	945*/
}DMB_TH_PN_E;

/**@brief DMB PNFLIP方式枚举*/
typedef enum
{
	EM_DMB_TH_PNFLIP_AUTO,    			/**<  自动*/
	EM_DMB_TH_PNFLIP_VPN,    				/**<	VPN */
	EM_DMB_TH_PNFLIP_CPN    				/**<	CPN */
}DMB_TH_PNFLIP_E;

/**@brief 频道信息参数结构体*/
typedef struct _DMB_TH_Para_S
{
	DWORD dwDMB_TH_Mode_3		:3;  			/**<  待补充*/
	DWORD	dwDMB_TH_SM_2			:2;  			/**<  待补充*/
	DWORD	dwDMB_TH_PN_2			:2;  			/**<  待补充*/
	DWORD	dwDMB_TH_PNFLIP_2	:2;  			/**<  待补充*/
	DWORD	dwReserved				:23;  		/**<  待补充*/
}DMB_TH_Para_S;

/**@brief 频道参数扩展结构*/
typedef struct _COFDM_ParamExt_S
{
	T_BANDWIDTH bBandWidth ;  					/**<  带宽*/
	DWORD dwFrequency ;  								/**<  频率(kHz)*/
	T_TransMode bTransMode ;  					/**<  传输模式*/
	T_GuardMode bGuard ;  							/**<  Guard模式*/
	T_IQMode	bIQ;  										/**<  IQ模式*/
	T_Constellation bConstellation ;  	/**<  星座图*/
	T_CodeRate bHPCode ;  							/**<  高位码率*/
	T_CodeRate bLPCode ;  							/**<  低位码率*/
	T_Heirarchy bHeirarchy ;  					/**<  bHeirarchy参数*/
	T_HPLP bHPLP;  											/**<  优先级*/ 
	DMB_TH_Para_S sDMBTHPara;  					/**<  国标参数*/
}COFDM_ParamExt_S;

/**@brief 频道参数结构*/
typedef struct _T_COFDM_Param
{
	T_BANDWIDTH bBandWidth ;  					/**<  带宽*/
	DWORD dwFrequency ; 								/**<  频率*/
	T_TransMode bTransMode ;  					/**<  传输模式*/
	T_GuardMode bGuard ;  							/**<  Guard模式*/
	T_IQMode	bIQ;  										/**<  IQ模式*/
	T_Constellation bConstellation ; 		/**<  星座图*/
	T_CodeRate bHPCode ;  							/**<  高位码率*/
	T_CodeRate bLPCode ;  							/**<  低位码率*/
	T_Heirarchy bHeirarchy ;  					/**<  T_Heirarchy参数*/ 
	T_HPLP bHPLP;  											/**<  优先级*/ 
}T_COFDM_Param;

/**@brief 回调参数结构*/
typedef struct _COFDMCallbackMessage
{
	T_COFDMCallbackEvents dwNIMEvent;							/**<  回调事件*/
	DWORD dwFrequency;														/**<  频率*/
	DWORD dwBandWidth;														/**<  带宽*/
	CSHDITunerIndex bTunerIndex;									/**<  tuner索引*/
}COFDMCallbackMessage;


/**
@brief 初始化COFDM前端

@param[in] enmTunerIndex 要初始化tuner的索引:Tuner_0表示0号tuner；Tuner_1表示1号tuner
@param[in] nPriority 所需要的优先级。
@param[in] enmTSOutputMode 传输流输出接口模式(串行/并行): TS_PARALLEL表示并行; TS_SERIAL表示串行
@param[in] nResetPinNum 连接解调芯片硬复位的GPIO的引脚index.若不希望硬复位，则设为0xff
@return CSCOFDM_SUCCESS 表示前端初始化成功，CSCOFDM_FAILURE表示初始化失败。
@note 该函数必须先于其它函数调用，且对于每个tuner只能在系统复位时调用１次，否则会造成不可预知的错误。
*/
CSCOFDM_Error_t CSCOFDMInitialize(CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutputMode, int nResetPinNum);


/**
@brief 初始化COFDM前端

@param[in] enmTunerIndex 要初始化tuner的索引:Tuner_0表示0号tuner；Tuner_1表示1号tuner
@param[in] eI2cIndex tuner所在的I2C索引。
@param[in] nPriority 所需要的优先级。
@param[in] enmTSOutputMode 传输流输出接口模式(串行/并行): TS_PARALLEL表示并行; TS_SERIAL表示串行
@param[in] nResetPinNum 连接解调芯片硬复位的GPIO的引脚index.若不希望硬复位，则设为0xff
@return CSCOFDM_SUCCESS 表示前端初始化成功，CSCOFDM_FAILURE表示初始化失败。
@note 该函数必须先于其它函数调用，且对于每个tuner只能在系统复位时调用１次，否则会造成不可预知的错误。
*/
CSCOFDM_Error_t CSCOFDMInitializeEx(CSHDITunerIndex enmTunerIndex, CSHDII2CIndex eI2cIndex, int nPriority, CSHDITsMode enmTSOutputMode, int nResetPinNum);

/**
@brief 注册回调函数，用来传送状态通知

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[in] pfunCOFDMCallback 注册的回调函数
@note 对于每个tuner该函数必须紧跟着各自的CSCOFDMInitialize 调用，必须在其它函数之前调用．否则不能正确返回通知
*/
void CSCOFDMRegisterCallback( CSHDITunerIndex enmTunerIndex, CSHDICallback pfunCOFDMCallback) ;

/**
@brief 设置信号参数,搜索信号,To send a request to lock a specified frequency point.

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[in] pstSignal 信号参数结构指针（参见结构说明中T_COFDM_Param的说明）
@return CSCOFDM_SUCCESS表示设置参数成功，CSCOFDM_FAILURE表示设置参数失败
*/
CSCOFDM_Error_t CSCOFDMConnectSignal(CSHDITunerIndex enmTunerIndex, T_COFDM_Param *pstSignal);

 /**
@brief 查询信号质量。

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[in] pnSignalQuality 信号质量,范围为0到100,值越大,质量越高
@return CSCOFDM_SUCCESS表示查询质量成功，CSCOFDM_FAILURE表示查询质量失败。
*/
CSCOFDM_Error_t CSCOFDMQuerySignalQuality(CSHDITunerIndex enmTunerIndex,int *pnSignalQuality);

 /**
@brief 查询信号强度

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[out] pnSignalStrength 信号强度，范围为0到100,值越大，信号越强
@return CSCOFDM_SUCCESS表示查询信号强度成功，CSCOFDM_FAILURE表示查询信号强度失败。
*/
CSCOFDM_Error_t CSCOFDMQuerySignalStrength(CSHDITunerIndex enmTunerIndex, int *pnSignalStrength);
 
 /**
@brief 查询信号电平

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[out] pnSignalRFLevel 信号电平大小
@return CSCOFDM_SUCCESS表示查询成功，CSCOFDM_FAILURE表示查询失败。
*/
 CSCOFDM_Error_t CSCOFDMQuerySignalLeveldBm(CSHDITunerIndex enmTunerIndex, int *pnSignalRFLevel);	

 
 /**
@brief 查询信号载噪比(C/N)

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[out] pnSignalCN 信号载噪比大小
@return CSCOFDM_SUCCESS表示查询成功，CSCOFDM_FAILURE表示查询失败。
*/
 CSCOFDM_Error_t CSCOFDMQuerySignalCNdB(CSHDITunerIndex enmTunerIndex, int *pnSignalCN);

/** 
@brief 查询信号误码率

@param[in]  enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[out] pdwBER_1E7 1*10^-7个码元中错误码元的个数．(除以10^-7就得到误码率)
@return CSCOFDM_SUCCESS表示查询误码率成功，CSCOFDM_FAILURE表示查询误码率失败。
*/
CSCOFDM_Error_t CSCOFDMQueryErrorRate(CSHDITunerIndex enmTunerIndex, DWORD *pdwBER_1E7);

/**
@brief 放弃当前搜索信号，直至有新的锁频请求

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@return CSCOFDM_SUCCESS表示设置成功，CSCOFDM_FAILURE表示设置失败。
*/
CSCOFDM_Error_t CSCOFDMDropSignal(CSHDITunerIndex enmTunerIndex);

/**
@brief 查询信号锁定状况

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[out] pnLockStatus 锁定状态，TRUE为锁定，FALSE为未锁定。
@return CSCOFDM_SUCCESS表示查询成功，CSCOFDM_FAILURE表示查询失败。
*/
CSCOFDM_Error_t CSCOFDMCheckNIMLock(CSHDITunerIndex enmTunerIndex, int *pnLockStatus);

/**
@brief 获取正确的信号参数

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[out] pstTPSParam 信号参数结构（参见结构说明中T_COFDM_Param的说明）
@return CSCOFDM_SUCCESS表示获取信号参数成功，CSCOFDM_FAILURE表示获取信号参数失败。
*/
CSCOFDM_Error_t CSCOFDMGetTPSParam(CSHDITunerIndex enmTunerIndex, T_COFDM_Param *pstTPSParam);

/**
@brief 配置LNB电源开关的GPIO口

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[in] nGPIOIndex 控制LNB电源开关的GPIO口的索引
@return CSCOFDM_SUCCESS表示配置成功，CSCOFDM_FAILURE表示配置失败。
*/
CSCOFDM_Error_t CSCOFDMConfigPinLNBPower (CSHDITunerIndex enmTunerIndex, int nGPIOIndex);

/**
@brief  配置LNB电源开关的GPIO口

@param[in] enmTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[in] enmSwitchStatus CSHDI_OFF 表示关闭LNB电源;CSHDI_ON表示打开LNB电源
@return CSCOFDM_SUCCESS表示控制LNB电源成功，CSCOFDM_FAILURE表示控制LNB电源失败。
*/
CSCOFDM_Error_t CSCOFDMSwitchLNBPower (CSHDITunerIndex enmTunerIndex, CSHDISwitch enmSwitchStatus);

 /**
@brief 设置信号参数,搜索信号,To send a request to lock a specified frequency point.

@param[in] eTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[in] psCOFDMSignalParam 信号参数结构指针（参见结构说明中COFDM_ParamExt_S的说明）
@return CSCOFDM_SUCCESS表示设置参数成功，CSCOFDM_FAILURE表示设置参数失败
*/
CSCOFDM_Error_t CSCOFDMConnectSignalExt(CSHDITunerIndex eTunerIndex, COFDM_ParamExt_S *psCOFDMSignalParam);

/**
@brief 获取正确的信号参数

@param[in] eTunerIndex tuner的索引:Tuner_0表示0号tuner; Tuner_1表示1号tuner
@param[out] psCOFDMSignalParam 信号参数结构（参见结构说明中COFDM_ParamExt_S的说明）
@return CSCOFDM_SUCCESS表示获取信号参数成功，CSCOFDM_FAILURE表示获取信号参数失败。
*/
CSCOFDM_Error_t CSCOFDMGetTPSParamExt(CSHDITunerIndex eTunerIndex, COFDM_ParamExt_S *psCOFDMSignalParam);

/*
@brief 获取当前驱动库版本号

@param[in]nSize The size of Version Buffer. Must be less than 256.
@param[out]pchVer The top address of Version Buffer
@return 
- 1>If [pchVer] is NULL,return the actual length of version information,including '\0'.
- 2>If [pchVer] is not NULL,and the call is success,return the length of version information returned.
- 3>If the call is failure,return 0.   
*/
int CSCOFDMGetVersion(PCHAR pcVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif


