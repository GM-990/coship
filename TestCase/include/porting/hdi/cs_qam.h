/**@defgroup CSQAM [CSQAM]同洲项目专用QAM tuner 接口
@brief 本模块主要定义了同洲项目专用平台无关QAM驱动接口
@brief QAM: Quadrature Amplitude Modulation(正交调幅) 正交调幅(QAM)是信令信息被携带在相位中和调制载波振幅的一类调制。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _UDI_QAM_H_
#define _UDI_QAM_H_

#include "CSHDI_typedef.h"
#include "udi_tuner.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief QAM_驱动接口返回值定义*/
typedef enum
{
	CSQAM_SUCCESS,			/**< 成功*/
	CSQAM_FAILURE,				/**< 失败*/
	CSQAM_HAS_INITIALIZED,	/**< 已经初始化 */
	CSQAM_INVALID_PARAM, 		/**< 无效参数*/
	CSQAM_DEVICE_UNKNOWN,	/**< 未知设备,目前没有用*/
	CSQAM_I2C_ERROR,			/**< I2C错误,目前没有用*/
	CSQAM_GPIO_ERROR,			/**< GPIO错误,目前没有用 */
	CSQAM_SYSTEM_ERROR,		/**< 系统错误,目前没有用 */
	CSQAM_DROPPED,			/**< 信号丢失,目前没有用*/
	CSQAM_STOP,				/**< 停止锁频,目前没有用 */
	CSQAM_AGC_NOT_LOCK,		/**< AGC锁不住 */
	CSQAM_NOT_INITIALIZED,	/**< 未初始化 */
	CSQAM_LEVEL_TOO_HIGH,	/**< 输入电平太高,*/
	CSQAM_LEVEL_TOO_LOW		/**< 输入电平太低*/
}CSQAM_Error_t; 


/**@brief 频道参数结构体*/
typedef struct 		
{
  DWORD		dwFrequencyKHz;				/**<	频率 ( unit: KHz )  频率范围: ( 48000 - 866000 )*/
  DWORD		dwSymbolRateKHz;			/**< 符号率 ( unit: Kbps )*/  
  T_QAMMode	bQAMMode;				/**< 0 Not Define  QAM星座 ( 0 Not Define   1 - 16QAM	  2 - 32QAM	2 - 64QAM	  3- 128QAM	4 - 256QAM)*/
  int		nFrequencyOffsetKHz;		/**< 频偏(unit:KHz),没有用到 */
  int		nSymbolRateOffset;			/**< 符号率偏移(unit:Kbps),没有用到 */
  BYTE		bIQ;					/**< 0表示频谱不反转,1表示频谱反转 */
} T_QAMParameters;

/**@brief QAM_驱动接口返回值定义*/
typedef enum
{
	QAM_QUICKLOCK,		/**< 没有用到*/
	QAM_SEARCHING			/**< 没有用到 */
}T_QAMSearchingMode;

/**@brief QAM_驱动接口返回值定义*/
typedef enum _DVBMCNSMode_t
{
	DVBMCNS_DVB,						/**< DVB 模式*/
	DVBMCNS_MCNS						/**< ANNEX_B模式*/
}DVBMCNSMode_t;

/**@brief 回调函数 返回值*/
typedef enum								
{
	QAM_CALLBACK_SIGNAL_CONNECTED,        /**< 	信号锁定通知   */
	QAM_CALLBACK_SIGNAL_DROPPED,          /**<	放弃搜台通知*/
	QAM_CALLBACK_SIGNAL_LOST,             /**< 信号丢失通知*/
	QAM_CALLBACK_SIGNAL_SEARCHING	      /**< 信号未锁定通知*/
}T_QAMCallbackEvents;
 
/**@brief QAM_驱动接口返回值定义*/
typedef struct 
{
	T_QAMCallbackEvents dwNIMEvent;		/**< 回调函数的事件 */
	DWORD 	dwFrequency;				/**< 回调的频率(unit:KHz) */
	DWORD 	dwSymbolRate;				/**< 回调的符号率:(unit:Kbps) */	
	T_QAMMode dwQAMMode;				/**< 0 Not Define  QAM星座 ( 0 Not Define   1 - 16QAM	  2 - 32QAM	2 - 64QAM	  3- 128QAM	4 - 256QAM)*/
	CSHDITunerIndex bTunerIndex;		/**< 回调的tuner索引,从0开始 */
}T_QAMCallbackMessage;


/**@brief QAM_同频干扰模式定义*/
typedef enum
{
	EM_DIGITAL_INTERFERE_GB,   /**< 同频国标要求:小于27 */
	EM_DIGITAL_INTERFERE_8,    /**< 同频特殊要求:小于8，如武汉 */
	EM_DIGITAL_INTERFERE_MAX
}CSQAMDigitalInterfere_E;

/**
@brief 用来设置当前输入信号的带宽，在初始化之前调用，如果不调用此接口，默认为8M，主要适用于那些带宽可适配的高频头，如TDA18252

@param[in] bTunerIndex   tuner索引,该接口中tuner索引为demodulator所在的I2C索引
@param[in] bBandWidth  当前输入信号的带宽，6M或者8M
@return    CSQAM_SUCCESS 带宽设置成功  CSQAM_FAILURE带宽设置失败
*/
CSQAM_Error_t CSQAMSetBandWidth(CSHDITunerIndex bTunerIndex, T_BANDWIDTH eBandWidth); 

/**
@brief QAM模块初始化

@param[in] bTunerIndex   tuner索引,该接口中tuner索引为demodulator所在的I2C索引
@param[in] nPriority     线程的优先级, 0---255
@param[in] TSOutMode     TS流输出方式,TS_PARALLEL:并行输出 TS_SERIAL:串行输出
@param[in] nResetPinNum  硬件重置对应的GPIO索引,没有则用0xff
@return    CSQAM_SUCCESS 初始化成功  CSQAM_FAILURE初始化失败
*/
CSQAM_Error_t CSQAMInitialize( CSHDITunerIndex bTunerIndex, int nPriority, CSHDITsMode TSOutMode,  int nResetPinNum ); 

/**
@brief QAM模块初始化,是CSQAMInitialize的扩展函数

@param[in] bTunerIndex tuner索引号,Tuner_0 0号tuner, Tuner_1 1号tuner,从Tuner_0开始编号
@param[in] eI2cIndex tuner对应的I2C物理索引
@param[in] nPriority 线程优先级, 0--255
@param[in] TSOutMode TS流输出方式,TS_PARALLEL:并行输出 TS_SERIAL:串行输出
@param[in] nResetPinNum  硬件重置对应的GPIO索引,没有则用0xff
@return    CSQAM_SUCCESS 初始化成功  CSQAM_FAILURE初始化失败
*/
CSQAM_Error_t CSQAMInitializeEx( CSHDITunerIndex bTunerIndex,CSHDII2CIndex eI2cIndex, int nPriority, CSHDITsMode TSOutMode,  int nResetPinNum ); 

/**
@brief 注册用于提示用户高频头运行状态的回调函数,必须在初始化后调用

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[in] function 注册的回调函数,不能为空
@return    无
*/
void CSQAMRegisterCallback( CSHDITunerIndex bTunerIndex, CSHDICallback function );

/**
@brief 注销用于提示用户高频头运行状态的回调函数

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[in] function 建议function为空,否则断言提示,但不影响函数功能
@return    无
*/
void CSQAMUnRegisterCallback( CSHDITunerIndex bTunerIndex, CSHDICallback function );

/**
@brief 请求对指定的频点进行锁频,调用前请设置正确的频点,调用该函数后会自动对此频点进行锁频,失锁后也可以自动重锁直至锁频成功

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[in] signalPtr 频点信息,具体请参照T_QAMParameters描述
@return    CSQAM_SUCCESS 发出锁频请求成功 CSQAM_FAILURE发出锁频请求失败 
@	   CSQAM_INVALID_PARAM 输入参数错误 CSQAM_NOT_INITIALIZED未初始化
*/
CSQAM_Error_t CSQAMConnectSignal( CSHDITunerIndex bTunerIndex, T_QAMParameters *signalPtr );

/**
@brief 请求对指定的频点进行锁频,调用前请设置正确的频点,调用该函数后会自动对此频点进行锁频,失锁后也可以自动重锁直至锁频成功

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[in] signalPtr 频点信息,具体请参照T_QAMParameters描述
@param[in] bFlag 暂不用
@return    CSQAM_SUCCESS 发出锁频请求成功 CSQAM_FAILURE发出锁频请求失败 
@	   CSQAM_INVALID_PARAM 输入参数错误 CSQAM_NOT_INITIALIZED未初始化
*/
CSQAM_Error_t CSQAMConnectSignal_EXT( CSHDITunerIndex bTunerIndex, T_QAMParameters *signalPtr,T_QAMSearchingMode bFlag );

/**
@brief 获取当前高频头的锁频状态

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[out] pnLockStatus 1表示锁频 0表示失锁
@return    CSQAM_SUCCESS 获取锁频状态成功 CSQAM_FAILURE获取锁频状态失败 
@	   CSQAM_INVALID_PARAM 输入参数错误 CSQAM_NOT_INITIALIZED未初始化
*/
CSQAM_Error_t CSQAMCheckNIMLock(CSHDITunerIndex bTunerIndex, int *pnLockStatus);

/**
@brief 获取指定tuner的信号质量,在CSQAMConnectSignal后调用

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[out] pnSignalQuality  返回的是0--100的整数,对于计算Quality的公式并没有统一的标准，
@                            但是可以提供参考值，如64QAM, 小于18dB为0，大于35dB为100，中间为线性；
@			     256QAM，小于26dB为0，大于38dB为100，中间线性。另外，在锁不住的情况下，信号质量为0。
@return    CSQAM_SUCCESS 获取信号质量成功 CSQAM_FAILURE获取信号质量失败 
@	   CSQAM_INVALID_PARAM 输入参数错误 CSQAM_NOT_INITIALIZED未初始化
*/
CSQAM_Error_t CSQAMQuerySignalQuality( CSHDITunerIndex bTunerIndex, int *pnSignalQuality );	

/**
@brief 获取指定tuner的信号强度,在CSQAMConnectSignal后调用

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[out] pnSignalStrength 返回的是0---100的值。对于SignalStrengt的计算目前没有统一的标准, 
@			     但是可以提供参考值, 如64QAM，小于32dBuV可以认为信号强度为0，大于120dBuV可以认为信号强度为100，中间线性;
@			     对于256QAM，小于36dB可以认为信号强度为0，大于115dB可以认为信号强度为100，中间线性;
@			     在锁不住的情况下，仍然可能有信号强度。
@return    CSQAM_SUCCESS 获取信号强度成功 CSQAM_FAILURE获取信号强度失败 
@	   CSQAM_INVALID_PARAM 输入参数错误 CSQAM_NOT_INITIALIZED未初始化
*/
CSQAM_Error_t CSQAMQuerySignalStrength( CSHDITunerIndex bTunerIndex, int *pnSignalStrength );	

/**
@brief 获取指定tuner的误码率, 在CSQAMConnectSignal后调用

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[out] pdwErrorRate 返回的是R-S纠错后的错误的比特数(Bit error count), 锁不住时为10000000。
@return    CSQAM_SUCCESS 获取误码率成功 CSQAM_FAILURE获取误码率失败 
@	   CSQAM_INVALID_PARAM 输入参数错误 CSQAM_NOT_INITIALIZED未初始化
*/
CSQAM_Error_t CSQAMQueryErrorRate(CSHDITunerIndex bTunerIndex, DWORD *pdwErrorRate);		

/**
@brief 获取指定tuner的信号电平强度, 在CSQAMConnectSignal后调用

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[out] pnSignalLevel 返回的是输入信号的电平强度,范围30-114dBuV(unit:dBuV)
@return    CSQAM_SUCCESS 获取信号电平成功 CSQAM_FAILURE获取信号电平失败 
@	   CSQAM_INVALID_PARAM 输入参数错误 CSQAM_NOT_INITIALIZED未初始化
*/
CSQAM_Error_t CSQAMQuerySignalLeveldBuV(CSHDITunerIndex bTunerIndex, int *pnSignalLevel);

/**
@brief 获取指定tuner的载噪比(C/N值), 在CSQAMConnectSignal后调用

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[in] pnSignalCN  返回的值是信号的载噪比(unit:dB),如36.1dB返回的是36dB
@return    CSQAM_SUCCESS 获取信号载噪比成功 CSQAM_FAILURE获取信号载噪比失败 
@	   CSQAM_INVALID_PARAM 输入参数错误 CSQAM_NOT_INITIALIZED未初始化
*/
CSQAM_Error_t CSQAMQuerySignalCNdB(CSHDITunerIndex bTunerIndex, int *pnSignalCN);

/**
@brief 获取指定tuner的偏移参数

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[in] ptrQamChannel  具体参照T_QAMParameters描述,驱动中并没有保存频偏和符号率偏移值,所以nFrequencyOffsetKHz和nSymbolRateOffset赋值为0
@return    CSQAM_SUCCESS 获取偏移参数成功 CSQAM_FAILURE获取偏移参数失败 
@	   CSQAM_INVALID_PARAM 输入参数错误 
*/
CSQAM_Error_t CSQAMGetOffsetPAR(CSHDITunerIndex bTunerIndex, T_QAMParameters *ptrQamChannel);

/**
@brief 新增接口，请在QAM初始化前调用，用于选择不同的服务类型,不调用此函数默认的服务类型为DVBMCNS_DVB模式

@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[in] bModeDvbMcns  具体参照DVBMCNSMode_t说明 
@return    CSQAM_SUCCESS 设置服务类型成功, CSQAM_FAILURE 设置服务类型失败
@	   CSQAM_INVALID_PARAM 输入参数错误 CSQAM_HAS_INITIALIZED已经初始化
*/
CSQAM_Error_t CSQAMSetDVBMCNSMode( CSHDITunerIndex bTunerIndex, DVBMCNSMode_t bModeDvbMcns );

/**
@brief 获取模块版本信息，显示格式为:
  [模块名]-[模块类型]-[版本号],如CS_QAM-T-01.00.00, CS_QAM表示模块名,T表示test版本,01.00.00为版本号

@param[out] pchVer 存放版本信息的缓冲区首地址
@param[in] nSize  缓冲区长度
@return 
   1> 如果第一个参数pchVer为空，返回版本信息的实际长度
   2> 如果第一个参数不为空，且函数调用成功，则返回实
        际获取的版本信息长度
   3> 如果失败，返回0

*/
int CSQAMGetVersion( PCHAR pchVer, int nSize );


/**
@brief 用于注册画点函数，用于在屏幕上画出星座图,调用CSQAMShowConstellation显示

@note  画点函数的函数原型为: void SetPixel(int x, int y, DWORD color);
 	其中，x, y为屏幕坐标，color与透明度和RGB颜色的关系为:
 	color=(alpha<<24)|(B<<16)|(G<<8)|R. 以720x576作为参考
@return 无
*/
void CSQAMRegisterFuncSetPixel( void (*function)(int, int, DWORD)  );

/**
@brief 用于注册文本显示函数，用于在屏幕上显示误码率,信号质量等信息。调用CSQAMShowConstellation显示
@note 文本显示函数的函数原型为: void DrawText(char * strMsg, int xStart, int yStart,  DWORD color);
 	其中，xStart, yStart为显示起始坐标，color与透明度和RGB颜色的关系为:
 	color=(alpha<<24)|(B<<16)|(G<<8)|R.
@return 无
*/

void CSQAMRegisterFuncDrawText( void (*function)(char *, int, int,  DWORD)  )	;

/**
@brief 用于在屏幕上画出星座图，目前只支持64QAM，画点
	使用CSQAMRegisterFuncSetPixel函数注册的画点函数。
	绘制过程为，函数被调用后，首先将屏幕OSD层清调，
	然后获取信号IQ值并在屏幕上打点。因此需要重复调用
	该接口已获取实时的星座图。
	显示的内容包括:信号质量(0-100);CN值(unit:dB);信号强度(0-100);误码比特数
	
@param[in] bTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@return 无
*/
void CSQAMShowConstellation( CSHDITunerIndex bTunerIndex );

/**
@brief 该接口设置锁频时的频偏和符号率偏移参数，偏移策略为:
		首先以中心频率和中心符号率进行锁频，如果锁频失败，
		则以设定的步长和偏移次数，进行左右交迭偏移。
		注意设置的偏移次数是左右总的偏移次数。偏移顺序是先左后右。
		该函数可以在任何时候调用。
		如果用户不调用该函数，QAM库会以默认的参数进行锁频控制。
		以下是各高频头的默认控制参数:
		STV0297/STV0297J:
			nFreqSearchTime: 1
			nFreqStepKHz: 500
			nSymSearchTime: 0
			nSRateStepKSps: 2
		TDA10021/TDA10023:
			nFreqSearchTime: 0
			nFreqStepKHz: 500
			nSymSearchTime: 0
			nSRateStepKSps: 2

@param[in] nFreqSearchTime 设置锁频时左右频偏的次数
@param[in] nFreqStepKHz 设置锁频的频偏步长，以KHz为单位
@param[in] nSymSearchTime 设置锁频时左右符号率偏移的次数
@param[in] nSRateStepKSps 设置锁频时符号率偏移的步长，以KSps为单位
@return    CSQAM_SUCCESS 设置频偏参数成功, CSQAM_FAILURE 设置频偏参数失败
@	   CSQAM_INVALID_PARAM 输入参数错误 
*/
CSQAM_Error_t CSQAMSetSearchingPar(int nFreqSearchTime, int nFreqStepKHz, int nSymSearchTime, int nSRateStepKSps);

/**
@brief 用于计算 MT2060的第一中频 

@param[in] eTunerIndex   tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[in] pQAMParameters  输入频点信息,具体请参照T_QAMParameters说明
@param[in] dwIFout  tuner的中频值, Output IF(unit:KHz)(Range 0~60000)
@param[out] dwFIF1  返回的第一中频的值, MT2060 First IF(unit:Hz)(Range 1160,000,000~1270,000,000)
@param[out] dwPMax  返回的输入电平值(unit:*100dB), Input Power Level	(less than 0x1400->Input Level is low)(larger than 0x1900->Input Level is high)
@return 
-CSQAM_SUCCESS - 计算中频值成功   
-CSQAM_FAILURE	 - 计算中频值失败
-CSQAM_INVALID_PARAM	- 参数错误

*/
CSQAM_Error_t CSQAMLocateIF(CSHDITunerIndex eTunerIndex, T_QAMParameters* pQAMParameters, DWORD dwIFout, DWORD* dwFIF1, DWORD* dwPMax);

/**
@brief This function Get  the value of MT2060's First IF from App
@	该函数仅用于Silicon tuner MT2060的传递第一中频。
@	在之前已成功校验过第一中频之后，并将数据存入FLASH中；此后开机时无需验证参数，只需调用此函数将已经存储的数据传给MT2060


@param[in] eTunerIndex tuner索引 Tuner_0 0号tuner, Tuner_1 1号tuner
@param[in] dwFreqIF1 成功校验存于ROM中的第一中频值 MT2060 First IF from App read in flash(Hz)(Range 1160,000,000~1270,000,000)
@return  
-CSQAM_SUCCESS	- 获取成功
-CSQAM_FAILURE	 - 	获取失败
-CSQAM_INVALID_PARAM	- 参数错误 

*/
CSQAM_Error_t CSQAMGetIF(CSHDITunerIndex eTunerIndex, DWORD dwFreqIF1);



/**
@brief 
@	该函数设置同频干扰指标，默认为国标模式
@	该函数在初始化之前调用，不能重复调用
@param[in] eMode 同频干扰模式
@return  
-CSQAM_SUCCESS	- 设置成功
-CSQAM_FAILURE	 - 	设置失败
-CSQAM_INVALID_PARAM	- 参数错误 

*/
CSQAM_Error_t CSQAMSetDigitalInterfereMode(CSQAMDigitalInterfere_E eMode);

#ifdef __cplusplus
}
#endif
/** @} */
#endif
