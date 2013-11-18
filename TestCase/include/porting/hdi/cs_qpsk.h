/**@defgroup CSQPSK[CSQPSK]同洲项目专用Hardware QPSK porting
@brief 本模块主要定义了同洲项目专用平台无关QPSK驱动接口
@brief QPSK: Quadrature Phase Shift Keying(正交相移键控) 正交相移键控(QPSK)是一个通过转换或调制来传达数据的调制方法,基准信号(载波)的定相。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.3 2008/11/5  第一次细化版本
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_QPSK_H_
#define _CS_QPSK_H_

#include "cs_typedef.h"
#include "CSHDI_typedef.h"
#include "udi_tuner.h"

 
#ifdef __cplusplus
extern "C" {
#endif

/**@brief  save the Allocated user band parameters in unicable auto-installation*/
typedef struct  
{
	DWORD	m_dwUBNumber:8;				/*Allocated user band number*/
	DWORD	m_dwReserved:24;					/*reserved for future use*/
	DWORD 	m_dwCentralFreq;                			/*Allocated user band centre frequency,KHz*/
} CSQPSKUNICABLEParam_S;

/**@brief  unicable type */
typedef enum 
{
	EM_CSQPSK_UNICABLE_STANDARD,			/*standard unicable device whose commands are defined in CENELEC Standard EN50494*/
	EM_CSQPSK_UNICABLE_PRIME481, 			/*Extension of CENELEC Standard, such as Prime 4*8+1 unicable switch*/
	EM_CSQPSK_UNICABLE_MTI_DONGLE, 		/*nonstandard unicable device which has private commands, such as MTI dongle*/
	EM_CSQPSK_UNICABLE_RESERVED			/*reserved for future use*/
}CSQPSKUNICABLETYPE_E;

/**@brief  unicable type */
typedef enum 
{
	EM_CSQPSK_UNICABLE_UNDEFINED,		/*invalide user band */
	EM_CSQPSK_UNICABLE_1,					/*unicable user band 1*/
	EM_CSQPSK_UNICABLE_2,					/*unicable user band 2*/
	EM_CSQPSK_UNICABLE_3,					/*unicable user band 3*/
	EM_CSQPSK_UNICABLE_4,					/*unicable user band 4*/
	EM_CSQPSK_UNICABLE_5,					/*unicable user band 5*/
	EM_CSQPSK_UNICABLE_6,					/*unicable user band 6*/
	EM_CSQPSK_UNICABLE_7,					/*unicable user band 7*/
	EM_CSQPSK_UNICABLE_8,					/*unicable user band 8*/
	EM_CSQPSK_UNICABLE_MAX				/*reserved for future use*/
}CSQPSKUNICABLEINDEX_E;

/**@brief QPSK驱动接口返回值定义*/
typedef enum
{
	CSQPSK_SUCCESS,						/**< 操作成功 */
	CSQPSK_FAILURE,						/**< 操作失败*/
	CSQPSK_HAS_INITIALIZED,		/**< 模块已经被成功初始化*/
	CSQPSK_INVALID_PARAM, 		/**< 无效参数 */
	CSQPSK_DEVICE_UNKNOWN,		/**< 未知设备 */
	CSQPSK_I2C_ERROR,					/**< I2C访问错误 */
	CSQPSK_GPIO_ERROR,				/**< GPIO访问错误 */
	CSQPSK_SYSTEM_ERROR,			/**< 系统错误 */
	CSQPSK_DROPPED,						/**< 信号丢失 */
	CSQPSK_NOT_INITIALIZED		/**< 未初始化 */
}CSQPSK_Error_t; 

/**@brief QPSK驱动回调时间类型定义*/
typedef enum
{
	QPSK_CALLBACK_SIGNAL_CONNECTED,		/**< 信号锁定 */
	QPSK_CALLBACK_SIGNAL_DROPPED, 		/**< 人为信号丢失 */
	QPSK_CALLBACK_SIGNAL_LOST,				/**< 信号丢失 */
	QPSK_CALLBACK_SIGNAL_SEARCHING,		/**< 搜索信号 */
	QPSK_CALLBACK_STS_COMPLETE,	 			/**< 需要添加说明 */	
	QPSK_CALLBACK_STS_DROPPED		 			/**< 需要添加说明 */	
}T_QPSKCallbackEvents;

/**@brief 需要添加说明*/
typedef struct 	
{
	BYTE	ucSwitch0_12V;			/**< SWITCH_0V | SWITCH_12V*/
	BYTE	ucPolar;						/**< POLARISATION_V | POLARISATION_H*/
	BYTE	uc22K;							/**< CSHDI_ON | CSHDI_OFF*/
	BYTE	ucLNBPower;					/**< CSHDI_ON | CSHDI_OFF*/
	BYTE	ucLNBType;					/**< SINGLE | UNIVERSAL*/
	DWORD	dwLNBLowFreq;				/**< low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]*/
	DWORD	dwLNBMidFreq;				/**< middle LNB frequency in Universal Mod  [unit:KHz]*/
	DWORD	dwLNBHighFreq; 			/**< high LNB frequency in Universal Mod   [unit:KHz]*/
	BYTE	ucDiSEqC10;					/**< DiSEqC1.0开关	( PORT_4_A/PORT_4_B/PORT_4_C/PORT_4_D )*/
	BYTE	ucDiSEqC11;					/**< DiSEqC1.0 ( 0-16 and 0 for not used )*/
	BYTE	ucToneBurst;				/**< Tone Burst开关	( TONEBURST_NONE / TONEBURST_A / TONEBURST_B )*/
	BYTE	ucDiSEqCRepeat;			/**< 重复次数	( NoRepeat / OnceRepeat / TwiceRepeat )*/
}T_QPSKSateInfo;

/**@brief 需要添加说明*/
typedef struct 	
{
	BYTE	bSwitch0_12V;			/**< SWITCH_0V | SWITCH_12V*/
	BYTE	bPolar;						/**< POLARISATION_H | POLARISATION_V*/
	BYTE	b22K;							/**< CSHDI_ON | CSHDI_OFF*/
	BYTE	bLNBPower;				/**< CSHDI_ON | CSHDI_OFF*/
	BYTE	bLNBType;					/**< SINGLE | UNIVERSAL*/
	DWORD	dwLNBLowFreq;			/**< low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz] */
	DWORD	dwLNBMidFreq;			/**< middle LNB frequency in Universal Mod  [unit:KHz] */
	DWORD	dwLNBHighFreq; 		/**< high LNB frequency in Universal Mod   [unit:KHz] */
	BYTE	bDiSEqC10;				/**< DiSEqC1.0开关	( PORT_4_A/PORT_4_B/PORT_4_C/PORT_4_D ) */
	BYTE	bDiSEqC11;				/**< DiSEqC1.0 ( 0-16 and 0 for not used ) */
	BYTE	bToneBurst;				/**< Tone Burst开关	( TONEBURST_NONE / TONEBURST_A / TONEBURST_B ) */
	BYTE	bDiSEqCRepeat;		/**< 重复次数	( NoRepeat / OnceRepeat / TwiceRepeat ) */

	DWORD	dwFreqency;				/**< channel frequency in KHz */
	int		nFreqencyOffset;	/**< channel frequency offset in KHz */
	DWORD	dwSymbolRate;			/**< channel symbol rate in KBps */
	BYTE	bIQ;							/**< 0-Normal; 1-Invers */
	BYTE	bFECRates;				/**< FEC Rates(FEC_AUTO/FEC1_2/FEC2_3/FEC3_4/FEC5_6/FEC7_8) */
}CSQPSKSignalParameter;

/**@brief 需要添加说明*/
typedef struct 	
{
	BYTE	bSwitch0_12V;			/**< SWITCH_0V | SWITCH_12V*/
	BYTE	bPolar;						/**< POLARISATION_H | POLARISATION_V*/
	BYTE	b22K;							/**< CSHDI_ON | CSHDI_OFF*/
	BYTE	bLNBPower;				/**< CSHDI_ON | CSHDI_OFF*/
	BYTE	bLNBType;					/**< SINGLE | UNIVERSAL*/
	DWORD	dwLNBLowFreq;			/**< low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz] */
	DWORD	dwLNBMidFreq;			/**< middle LNB frequency in Universal Mod  [unit:KHz] */
	DWORD	dwLNBHighFreq; 		/**< high LNB frequency in Universal Mod   [unit:KHz] */
	BYTE	bDiSEqC10;				/**< DiSEqC1.0开关	( PORT_4_A/PORT_4_B/PORT_4_C/PORT_4_D ) */
	BYTE	bDiSEqC11;				/**< DiSEqC1.0 ( 0-16 and 0 for not used ) */
	BYTE	bToneBurst;				/**< Tone Burst开关	( TONEBURST_NONE / TONEBURST_A / TONEBURST_B ) */
	BYTE	bDiSEqCRepeat;		/**< 重复次数	( NoRepeat / OnceRepeat / TwiceRepeat ) */

	DWORD	dwFreqency;				/**< channel frequency in KHz */
	int		nFreqencyOffset;	/**< channel frequency offset in KHz */
	DWORD	dwSymbolRate;			/**< channel symbol rate in KBps */
	BYTE	bIQ;							/**< 0-Normal; 1-Invers */
	BYTE	bFECRates;				/**< FEC Rates(FEC_AUTO/FEC1_2/FEC2_3/FEC3_4/FEC5_6/FEC7_8) */
	
	BYTE 	ucPilot;					/**< 需要添加说明 */
	BYTE 	ucSatCRNo;				/**< Unicable 序号: (1 ~8) . 0为不可用, Unicable 最多输出8 个user band*/
	BYTE 	ucUnicableType;			/**< Unicable类型: EM_CSQPSK_UNICABLE_STANDARD/EM_CSQPSK_UNICABLE_PRIME481/EM_CSQPSK_UNICABLE_MTI_DONGLE ..... */
	DWORD	m_dwOption:2;			/**< Unicable 接入卫星序号,一个unicable最多支持2颗卫星输入:(OPTION_A/OPTION_B)*/
	DWORD	m_dwModulation:2;	/**< 需要添加说明 */
	DWORD  	m_dwstandard:2;	/**<DVBS|DVBS2|DVBAUTO   原来是DWORD  m_dwstandard:1;  */
	DWORD	dwreserved : 26;	/**<reserved for future use*/	
}CSQPSKSignalParameterS2;

/**@brief 需要添加说明*/
typedef struct 	
{
	BYTE	bSwitch0_12V;			/**< SWITCH_0V | SWITCH_12V*/
	BYTE	bPolar;						/**< POLARISATION_V | POLARISATION_H*/
	BYTE	b22K;							/**< CSHDI_ON | CSHDI_OFF*/
	BYTE	bLNBPower;				/**< CSHDI_ON | CSHDI_OFF*/
	BYTE	bLNBType;					/**< SINGLE | UNIVERSAL*/
	DWORD	dwLNBLowFreq;			/**< low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]*/
	DWORD	dwLNBMidFreq;			/**< middle LNB frequency in Universal Mod  [unit:KHz]*/
	DWORD	dwLNBHighFreq; 		/**< high LNB frequency in Universal Mod   [unit:KHz]*/
	BYTE	bDiSEqC10;				/**< DiSEqC1.0开关	( PORT_4_A/PORT_4_B/PORT_4_C/PORT_4_D )*/
	BYTE	bDiSEqC11;				/**< DiSEqC1.0 ( 0-16 and 0 for not used )*/
	BYTE	bToneBurst;				/**< Tone Burst开关	( TONEBURST_NONE / TONEBURST_A / TONEBURST_B )*/
	BYTE	bDiSEqCRepeat;		/**< 重复次数	( NoRepeat / OnceRepeat / TwiceRepeat )*/
	
	DWORD	dwFreqency;				/**< channel frequency in KHz*/
	int		nFreqencyOffset;	/**< channel frequency offset in KHz*/
	DWORD	dwSymbolRate;			/**< channel symbol rate in KBps*/
	BYTE	bIQ;							/**< 0-Normal; 1-Invers*/
	BYTE	bFECRates;				/**< FEC Rates(FEC_AUTO/FEC1_2/FEC2_3/FEC3_4/FEC5_6/FEC7_8)*/
	BYTE	ucSaTCRNum;				/**< 需要添加说明 */
	BYTE	ucLNBNum;					/**< 需要添加说明 */
	
}CSQPSKSaTCRSignalParameter;

/**@brief 回调消息*/
typedef struct _QPSKCallbackMessage
{
	T_QPSKCallbackEvents dwNIMEvent;		/**< 回调事件 */
	DWORD	dwFrequency;									/**< 频率 */
	DWORD	dwSymbolRate;									/**< 符号率 */
	CSHDITunerIndex bTunerIndex;				/**< Tuner索引 */
}QPSKCallbackMessage;



/**
@brief Initialize all supported QPSK tuner driver.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
 	nResetGpioIndex: The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
@note This routine must be called first
 	此函数目前调用了
 	CSQPSKStv6110Init();
 	CSQPSKStv0194Init();
 	CSQPSKA1108Init();
 	CSQPSKStv0165Init();
 	CSQPSKZL313nit();
 	的自动查找和初始化
*/
CSQPSK_Error_t CSQPSKInitialize(CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex);



/**
@brief Initialize all supported QPSK tuner driver.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
 @param[in] eI2cIndex index of I2C
 I2C_0: I2C 0 I2C_1: I2C 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
 	nResetGpioIndex: The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
*/
CSQPSK_Error_t CSQPSKInitializeEx(CSHDITunerIndex enmTunerIndex, CSHDII2CIndex eI2cIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex);


/**
@brief user call this function to registe a callback function ,and the driver will use this function notice 
 	the user the result of a lock operation. 

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] pfunQPSKCallBack the function user want to regist.

*/
CSQPSK_Error_t CSQPSKRegisterCallback(CSHDITunerIndex enmTunerIndex, CSHDICallback pfunQPSKCallBack);

/**
@brief user call this function to unregiste a callback function ,and the driver will use this function notice 
 	the user the result of a lock operation. 

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] pfunQPSKCallBack the function user want to regist.

*/
CSQPSK_Error_t CSQPSKUnRegisterCallback( CSHDITunerIndex enmTunerIndex, CSHDICallback pfunQPSKCallBack );


/**
@brief Config the Gpio used to control the switch between 13V and 18V  

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nGpioIndex the index of the Gpio that used to control the switch between 13V and 18V 
 		can be set to 0~0xff,and the 0xff means not used.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Config successfully.
 	CSQPSK_FAILURE:
 		Config failed.
	CSQPSK_GPIO_ERROR:
		Something wrong when call functions of Gpio Module.
*/
CSQPSK_Error_t CSQPSKConfigPin13_18V (CSHDITunerIndex enmTunerIndex, int nGpioIndex);

/**
@brief Change the lnb power between 13V and 18V 

@param[in] bTunerIndex index of tuner Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nSwitchStatus When POLARISATION_H change lnb power to 18V,and POLARISATION_V to 13V
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Config successfully.
 	CSQPSK_FAILURE:
 		Config failed.
*/
CSQPSK_Error_t CSQPSKSwitch13_18V ( CSHDITunerIndex bTunerIndex, Channel_Polarisation nSwitchStatus );

/**
@brief Change the lnb power between 13V and 18V  

@param[in] bTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] n22KStatus When CSHDI_OFF close 22K output, and CSHDI_ON open it
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Config successfully.
 	CSQPSK_FAILURE:
 		Config failed.
*/
CSQPSK_Error_t CSQPSKSwitch22K ( CSHDITunerIndex bTunerIndex, CSHDISwitch n22KStatus );

/**
@brief Config the Gpio used to control the switch the LNB Power 

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nGpioIndex the index of the Gpio that used to control the switch switch the LNB Power
 		can be set to 0~0xff,and the 0xff means not used.
@return  Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Config successfully.
 	CSQPSK_FAILURE:
 		Config failed.
	CSQPSK_GPIO_ERROR:
		Something wrong when call functions of Gpio Module.
*/
CSQPSK_Error_t CSQPSKConfigPinLNBPower (CSHDITunerIndex enmTunerIndex, int nGpioIndex);

/**
@brief Config the Gpio used to control the switch the LNB Power  

@param[in] bTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nSwitchStatus when LNB_OFF close lnb power
				LNB_AUTO set to auto and have no afflunce to other APIs 
				LNB_14V lnb power allways output 14V
				LNB_18V lnb power allways output 18V
@return  Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Config successfully.
 	CSQPSK_FAILURE:
 		Config failed.
	CSQPSK_GPIO_ERROR:
		Something wrong when call functions of Gpio Module.
*/
CSQPSK_Error_t CSQPSKSwitchLNBPower ( CSHDITunerIndex bTunerIndex, CSQPSK_LNB_Status nSwitchStatus );

/**
@brief Config the Gpio used to control the switch the 0_12V   

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] ucGpioIndex  the index of the Gpio that used to control the switch switch the 0_12V
 		can be set to 0~0xff,and the 0xff means not used.
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Config successfully.
 	CSQPSK_FAILURE:
 		Config failed.
	CSQPSK_GPIO_ERROR:
		Something wrong when call functions of Gpio Module.
*/
CSQPSK_Error_t CSQPSKConfigPin0_12V (CSHDITunerIndex enmTunerIndex, BYTE ucGpioIndex);

/**
@brief Switch 0 or 12V power supply   

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] enmSwitchStatus when LNB_OFF close lnb power
				SWITCH_0V set to 0V 
				SWITCH_12V set to 12V 
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Config successfully.
 	CSQPSK_FAILURE:
 		Config failed.
	CSQPSK_GPIO_ERROR:
		Something wrong when call functions of Gpio Module.
*/

CSQPSK_Error_t CSQPSKSwitch0_12V(CSHDITunerIndex enmTunerIndex, Switch0_12V enmSwitchStatus);

/**
@brief Send a request to lock a specified frequency point.  

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] pstSignalParam Point to the struct that store the information of frequency point
 		see to the struct "CSQPSKSignalParameter". 
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Send locking request successfully.
 	CSQPSK_FAILURE:
 		Send locking request failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
*/
CSQPSK_Error_t CSQPSKConnectSignal(CSHDITunerIndex enmTunerIndex, CSQPSKSignalParameter *pstSignalParam);

/**
@brief Send a request to lock a specified frequency point.  

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] pstSignalParam Point to the struct that store the information of frequency point
 		see to the struct "CSQPSKSignalParameter".
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Send locking request successfully.
 	CSQPSK_FAILURE:
 		Send locking request failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
*/
CSQPSK_Error_t CSQPSKConnectSignalS2(CSHDITunerIndex enmTunerIndex, CSQPSKSignalParameterS2 *pstSignalParam);

/**
@brief Get the lock status.  

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[out] pnLockStatus The status of tuner,locked or unlock.
 		TRUE: locked
 		FALSE: unlock
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
 	CSQPSK_DROPPED:
 		Tuner is unknown.
*/
CSQPSK_Error_t CSQPSKCheckNIMLock(CSHDITunerIndex enmTunerIndex, int *pnLockStatus);

/**
@brief Get the signal quality. 

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[out] pnSignalQuality The value of signal quality.
 		The value can be 0~100
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
 	CSQPSK_DROPPED:
 		Tuner is unknown.
*/
CSQPSK_Error_t CSQPSKQuerySignalQuality(CSHDITunerIndex enmTunerIndex, int *pnSignalQuality);	

/**
@brief Get the signal strength.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[out] pnSignalStrength The value of signal strength.
 		The value can be 0~100
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
 	CSQPSK_DROPPED:
 		Tuner is unknown.
*/
CSQPSK_Error_t CSQPSKQuerySignalStrength(CSHDITunerIndex enmTunerIndex, int *pnSignalStrength);	

/**
@brief Get the real RF level of the input signal, using dBm as measuring unit .

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[out] pnSignalRFLevel The value of signal RF level.

@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
 	CSQPSK_DROPPED:
 		Tuner is unknown.
*/
CSQPSK_Error_t CSQPSKQuerySignalLeveldBm(CSHDITunerIndex enmTunerIndex, int *pnSignalRFLevel);	

/**
@brief Get the real C/N level of the input signal, using dB as measuring unit .

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[out] pnSignalCN The value of signal C/N.
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
 	CSQPSK_DROPPED:
 		Tuner is unknown.
*/
CSQPSK_Error_t CSQPSKQuerySignalCNdB(CSHDITunerIndex enmTunerIndex, int *pnSignalCN);	

/**
@brief Get the Count of error byte.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[out] pdwErrorCount The value of error bit.
 		The value can be 0~10000000, pdwErrorCount/10000000 is the error rate.
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
 	CSQPSK_DROPPED:
 		Tuner is unknown.
*/
CSQPSK_Error_t CSQPSKQueryErrorRate(CSHDITunerIndex enmTunerIndex, DWORD *pdwErrorCount);		

/**
@brief Get the offset of frequency.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1

 		
@return The value of the frequency offset.
*/
int CSQPSKQueryFrequencyOffset (CSHDITunerIndex enmTunerIndex);

/**
@brief Make the status of tuner is unlock.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1

 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
*/
CSQPSK_Error_t CSQPSKDropSignal(CSHDITunerIndex enmTunerIndex);

/**
@brief Set the polarity of tuner clock.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] enmClockPol polarity fo tuner clock
 		see to enumerate "T_ClockPolarity"
 		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
*/
CSQPSK_Error_t CSQPSKSetClockPolarity(CSHDITunerIndex enmTunerIndex, T_ClockPolarity enmClockPol );

/**
@brief Set the polarity of tuner clock.

@param[in] nSize The size of Version Buffer.
@param[out] pcVer The top address of Version Buffer
 		
@return  <1>If [pcVer] is NULL,return the actual length of
	 	version information,including "\0".
	 <2>If [pcVer] is not NULL,and the call is success,
	 	return the length of version information returned.
	 <3>If the call is failure,return 0.	
@note This function is used to get version information.
     Display format is:
 	"[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
*/
int CSQPSKGetVersion(PCHAR pcVer, int nSize);

/**
@brief set the center frequency responding to the user band. 

@param[in] nSaTCRNum index of SaTCR
		(1~ 8): see CSQPSKUNICABLEINDEX_E. 

@param[in] nBPF centry frequency
		(950~2150): IF frequency, such as 1284, 1400, 1516, 1632, 1748, 1864, 1980, 2096MHz
		these values are provided by the unicable device vendor.

@return none
*/
void CSQPSKSetSaTCRBPF(int nSaTCRNum, int nBPF);

/**
@brief STB send assignment command. 
@param[in] bTunerIndex index of tuner
 		Tuner_0: tuner 0
 		Tuner_1: tuner 1
 		
@param[in] nSaTCRNum index of SaTCR
		(1~ 8): see CSQPSKUNICABLEINDEX_E. 
		
@param[in] dwCenterFreq  the center frequency of user band
		 
@param[out] pucScrNum
		
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
*/
CSQPSK_Error_t CSQPSKUnicableUBxAssignment (CSHDITunerIndex eTunerIndex, DWORD dwCenterFreq, BYTE * pucScrNum);

/**
@brief STB send accept command which will make the MDU identify the specified UB as an occupied UB.

@param[in] bTunerIndex index of tuner
 		Tuner_0: tuner 0
 		Tuner_1: tuner 1
 		
@param[in] nSaTCRNum index of SaTCR
		(1~ 8): see CSQPSKUNICABLEINDEX_E. 

@param[out] none

@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
*/
CSQPSK_Error_t CSQPSKUnicableUBxAccept (CSHDITunerIndex eTunerIndex,BYTE ucScrNum);

/**
@brief Deallocate an assigned UB.

@param[in] bTunerIndex index of tuner
 		Tuner_0: tuner 0
 		Tuner_1: tuner 1
 		
@param[in] nSaTCRNum index of SaTCR
		(1~ 8): see CSQPSKUNICABLEINDEX_E. 

@param[out] none

@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
*/
CSQPSK_Error_t CSQPSKUnicableUBxDeAllocation (CSHDITunerIndex eTunerIndex, BYTE ucScrNum);

/**
@brief find a free UB with auto detecting CW.

@param[in] bTunerIndex index of tuner
 		Tuner_0: tuner 0
 		Tuner_1: tuner 1
 		
@param[in] eUnicableType  unicable device type
		see to the enum "CSQPSKUNICABLETYPE_E".

@param[out] psUnicableParam store the detected user band parameters,
		see to the struct "CSQPSKUNICABLEParam_S".

@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
*/
CSQPSK_Error_t CSQPSKUnicableAutoInstall(CSHDITunerIndex eTunerIndex, CSQPSKUNICABLETYPE_E eUnicableType, CSQPSKUNICABLEParam_S * psUnicableParam);

/**
@brief handle the traffic collison when traffic collision has been detected in unicable system. 

@param[in] bTunerIndex  
		Tuner_0: tuner 0
 		Tuner_1: tuner 1

 @return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Operate successfully.
 	CSQPSK_FAILURE:
 		Operate failed.
*/
CSQPSK_Error_t CSQPSKHandleTrafficCollison( CSHDITunerIndex bTunerIndex );

/**
@brief Send a request to lock a specified frequency point.

@param[in] bTunerIndex index of tuner
 		Tuner_0: tuner 0
 		Tuner_1: tuner 1
@param[in] signalPtr Point to the struct that store the information of frequency point
 		see to the struct "CSQPSKSignalParameter".
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		Send locking request successfully.
 	CSQPSK_FAILURE:
 		Send locking request failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.	
*/
CSQPSK_Error_t CSQPSKSaTCRConnectSignal( CSHDITunerIndex bTunerIndex, CSQPSKSaTCRSignalParameter *signalPtr);

/**
@brief Initialize supported QPSK Stv6110 tuner driver.

@param[in] enmTunerIndex index of tuner
 		Tuner_0: tuner 0
 		Tuner_1: tuner 1
@param[in] nPriority no use
@param[in] TSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
@param[in] nResetGpioIndex 
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
@note 
 	此函数只适合于DVB-S1/DVB-S2标准的stv6110(stv0903chip sharp tuner 0169)tuner 初始化
 	调用此函数前必须调用CSQPSKInitializeEx
 	CSQPSKStv6110Init()支持的tuner:demodulator芯片是stv0903,tuner芯片是stb6110,外观文字写有F7HZ1169
*/
CSQPSK_Error_t CSQPSKStv6110Init( CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode TSOutMode, int nResetGpioIndex );

/**
@brief Initialize all supported QPSK tuner driver.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
@note 
 	此函数只适合于DVB-S1/DVB-S2标准的(sharp tuner 0165)tuner 初始化
 	调用此函数前必须调用CSQPSKInitializeEx
 	CSQPSKStv0165Init()支持的tuner:demodulator芯片是CX24116,tuner芯片还没搞清楚,外观文字写有F7HZ1165
*/
CSQPSK_Error_t CSQPSKA1108Init( CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex );

/**
@brief Initialize supported QPSK Stv0194 tuner driver.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
 	nResetGpioIndex: The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
@note 
 	此函数只适合于DVB-S1/DVB-S2标准的(sharp tuner 0165)tuner 初始化
 	调用此函数前必须调用CSQPSKInitializeEx
 	CSQPSKStv0165Init()支持的tuner:demodulator芯片是CX24116,tuner芯片还没搞清楚,外观文字写有F7HZ1165
*/
CSQPSK_Error_t CSQPSKStv0194Init( CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex );

/**
@brief Initialize supported QPSK Ct202 tuner driver.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
 	nResetGpioIndex: The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
@note 
 	需要添加描述
*/
CSQPSK_Error_t CSQPSKCt202Init( CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex );

/**
@brief Initialize supported QPSK Stv0165 tuner driver.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
 	nResetGpioIndex: The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
@note 
 	需要添加描述
*/
CSQPSK_Error_t CSQPSKStv0165Init( CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex );
/**
@brief Initialize supported QPSK ZL313 tuner driver.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
 	nResetGpioIndex: The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
@note  	此函数只适合于DVB-S1/DVB-S2标准的(sharp tuner 0165)tuner 初始化
 	调用此函数前必须调用CSQPSKInitializeEx
 	CSQPSKStv0165Init()支持的tuner:demodulator芯片是CX24116,tuner芯片还没搞清楚,外观文字写有F7HZ1165
*/
CSQPSK_Error_t CSQPSKZL313Init( CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex );

/**
@brief Initialize supported QPSK Stv7395 tuner driver.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
 	nResetGpioIndex: The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
 @note  CSQPSKStv7395Init()支持的tuner:demodulator芯片是stv0288,tuner芯片是IX2470,外观文字写有F7ZV7395
*/
CSQPSK_Error_t CSQPSKStv7395Init( CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex );
/**
@brief Initialize supported QPSK Stx7111  tuner driver.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
 	nResetGpioIndex: The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
 @note  CSQPSKStx7111Init()支持的tuner:demodulator 是Stx7111内部集成的双demodulator ,tuner芯片是sharp7306,外观文字写有S7VZ7306.
*/
CSQPSK_Error_t CSQPSKStx7111Init( CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex );

/**
@brief Initialize the internal QPSK demodulator of STi5189.

@param[in] enmTunerIndex index of tuner
 Tuner_0: tuner 0 Tuner_1: tuner 1
@param[in] nPriority the priority of qpsk task,can be set to 0~255
@param[in] enmTSOutMode TS Mode
 		TS_PARALLEL: TS output mode is paraller 
 		TS_SERIAL: TS output mode is paraller 
 	nResetGpioIndex: The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.

@param[in] nResetGpioIndex The index of Gpio that be used to reset the tuner when initilize
 		can be set to 0~0xff,and the 0xff means no reset pin.
@return Operation Return Status	 
 	CSQPSK_SUCCESS:
 		QPSK module initialized successfully.
 	CSQPSK_HAS_INITIALIZED:
 		QPSK module was initialized at befor.
 	CSQPSK_FAILURE:
 		QPSK module initilize failed.
 	CSQPSK_SYSTEM_ERROR:
 		Some thing wrong when call OS functions.
 	CSQPSK_DEVICE_UNKNOWN:
 		tuner is unknown;
 @note  CSQPSKSTV5189Init()支持的demod 是Sti5189 内部集成的demodulator ;支持的tuner类型是sharp0502, 或则stv6110.
*/
CSQPSK_Error_t CSQPSKSTV5189Init( CSHDITunerIndex enmTunerIndex, int nPriority, CSHDITsMode enmTSOutMode, int nResetGpioIndex );


#ifdef __cplusplus
}
#endif
/** @} */
#endif

