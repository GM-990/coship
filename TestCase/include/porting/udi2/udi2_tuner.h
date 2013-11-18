/**@defgroup TUNER TUNER 定义了统一的tuner接口操作函数
@brief Tuner模块对应头文件<udi2_tuner.h>，定义了Tuner的常用操作接口。支持DVB-C标
准(QAM调制方式),DVB-S/DVB-S2标准(QPSK,8PSK调制方式),DVB-T标准(COFDM和DMB-TH调制方式)。

Tuner分为固定Tuner和可移动Tuner两种，通过GetAllDeviceId可获得每个Tuner的Id，其中固
定Tuner的ID从0开始，依次累加；可移动Tuner的ID规则见RDI模块描述。

可移动Tuner在被发现后，为Init状态；固定Tuner在初始化后均为Init状态。
Tuner经过一些列的接口调用会产生状态的变化，在这些变化的过程中，回调消息应按下图所
示的情况发生。其中：\n
- Init 指tuner初始化完成之后还没有进行锁频之前的状态。
- Locked 指tuner进行锁频操作，并且锁频成功之后的状态。
- Unlocked 指tuner没有锁频成功的状态。
- Init -> Locked 是指tuner进行第一次锁频操作，锁频成功之后发送一次CONNECTED消息
- Init -> Unlocked 是指tuner进行第一次锁频操作，锁频失败之后，不断尝试进行锁频
操作，期间会不断发送SEARCHING消息直到锁频成功。searching消息发送的频率是每1s一次。
- Unlocked -> Locked 是指tuner从锁频失败状态之后再次锁频成功，成功之后发送CONNECTED
消息。同时停止发送searching消息
- Locked->Unlocked 是指tuner锁频失败后，发送一次LOST消息，并不断进行锁频操作，期间
会发送SEARCHING消息直到锁频成功。\n

@section Tuner状态改变产生回调图
@image HTML udi_tuner_event.jpg

从数据的传输角度本模块又将Tuner分为两类：
- 一种为DMA类型的Tuner，即固定Tuner，通过该Tuner锁频后的TS数据会直接DMA到与其链
接的Demux或者其它设备上。
- 一种为SOFT类型的Tuner，即可移动Tuner，通过该Tuner锁频后的TS数据需要通过IORead
接口读到Buff上，并由Eastwin决定其后续走向，如注入到Demux中进行播放，或者存储到
硬盘等。

部分高频头会支持PID过滤功能，这种高频头常见于一些可移动设备如DVB-TH、CMMB
等。当高频头的m_bPIDFilterSupport属性为CSUDI_TRUE时，该功能有效，这种情况下锁定
频点后，高频头不会将任何数据传送给其后续处理模块，必须设置哪些Pid（或者时隙）
的数据会通过后，方有数据通过，其实质相当于给了Tuner一个简单的Demux功能。如果
系统中不存在这样的Tuner，可不实现此接口，直接返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
即可。

@version 2.2.0 2011/12/31 修改CSUDITunerTerrestrialDeliver_S结构体，增加T2相关接口
@version 2.1.2 2009/09/15 修改Tuner事件回调参数类型，修正一些编码规范问题
@version 2.1.1 2009/09/11 修改获取Tuner信号情况接口，该接口现在可选择的获取部分信息而不是一次性全部获取
@version 2.1.0 2009/09/08 增加可移动Tuner设备相关定义
@version 2.0.2 2009/08/28 增加注册回调函数时指定tuner设备号号的参数
@version 2.0.1 2009/08/25 第一次综合评审修改后的版本
@{
*/

#ifndef _UDI2_TUNER_H_
#define _UDI2_TUNER_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_rdi.h"
#ifdef __cplusplus
extern "C" 
{
#endif
	
/**@brief tuner模块相关错误代码值*/
enum
{
	CSUDITUNER_ERROR_BAD_PARAMETER = CSUDI_TUNER_ERROR_BASE,	///< 参数错误
	CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED,						///< 不支持此操作
	CSUDITUNER_ERROR_UNKNOWN,									///< 一般错误
	CSUDITUNER_ERROR_NO_MEMORY,									///< 无内存可用错误
	CSUDITUNER_ERROR_INVALID_DEVICE_ID,							///< 非法设备ID
	CSUDITUNER_ERROR_I2C_ERROR,								    ///< I2C总线错误
	CSUDITUNER_ERROR_NO_CALLBACK,                               ///< 没有相应的回调函数可以删除
	CSUDITUNER_ERROR_NO_DATA,									///< 没有数据可以读取
	CSUDITUNER_ERROR_ALREADY_ADDED								///< 该回调已经添加过
};

/**@brief QAM 星座类型枚举定义*/
typedef enum
{
	EM_UDITUNER_NOT_DEFINED,	///< 未定义的QAM星座，一般情况下标是无效参数
	EM_UDITUNER_QAM16,			///< 16QAM星座
	EM_UDITUNER_QAM32,			///< 32QAM星座
	EM_UDITUNER_QAM64,			///< 64QAM星座
	EM_UDITUNER_QAM128,			///< 128QAM星座
	EM_UDITUNER_QAM256			///< 256QAM星座 
}CSUDITunerQAMMode_E;

/**@brief 有线数字电视(DVB-C)tuner的信号参数数据结构*/
typedef struct 
{
	unsigned int m_uFrequency;					///< 频率，单位KHz
	unsigned int m_uSymbolRate_24			:24;    	///< 符号率，单位Kbps
	unsigned int m_uModulation_8			:8;	 ///< CSUDITunerQAMMode_E类型枚举
}CSUDITunerCableDeliver_S;

/**@brief tuner模块开关类型枚举定义*/
typedef enum
{
	EM_UDITUNER_SWITCH_OFF,	///< 开关关闭
	EM_UDITUNER_SWITCH_ON	///< 开关打开
}CSUDITunerSwitchStatus_E;


/**@brief cofdm 信号带宽类型枚举定义*/
typedef enum 							
{
	EM_UDITUNER_BAND_8M,    ///<带宽8M 
	EM_UDITUNER_BAND_7M,    ///<带宽7M
	EM_UDITUNER_BAND_6M     ///<带宽6M
}CSUDITunerBanuidth_E;

/**@brief 地面数字电视(DVB-T)tuner的信号参数数据结构*/
typedef struct 
{
	unsigned int m_uFrequency;					          ///< 频率，单位KHz
	unsigned int m_uBanuidth_3				:3;	 	///< CSUDITunerBanuidth_E类型枚举
	unsigned int m_uFlag_2					:2;		///DVB-T/DVB-T2标志:00-T;01-T2;10,11保留
	unsigned int m_uPlpID_8					:8;     ///T2存储PLPID
	unsigned int m_uReserved_19				:19;  	///< 保留使用 
}CSUDITunerTerrestrialDeliver_S;

/**@brief QPSK驱动Diseqc1.0端口类型枚举定义*/
typedef enum  
{
	EM_UDITUNER_DISEQC10_OFF,	///< diseqc1.0通道选择关闭
	EM_UDITUNER_PORT_4_A,		///< diseqc1.0四选一开关的A通道
	EM_UDITUNER_PORT_4_B,		///< diseqc1.0四选一开关的B通道
	EM_UDITUNER_PORT_4_C,		///< diseqc1.0四选一开关的C通道
	EM_UDITUNER_PORT_4_D,		///< diseqc1.0四选一开关的D通道
	EM_UDITUNER_PORT_2_A,		///< diseqc1.0二选一开关的A通道
	EM_UDITUNER_PORT_2_B		///< diseqc1.0二选一开关的B通道
}CSUDITunerSwitchDisqec10Port_E;  

/**@brief QPSK驱动信号极化方向枚举定义*/
typedef enum 
{
	EM_UDITUNER_POLARISATION_H,		///< 线极化方向-水平极化
	EM_UDITUNER_POLARISATION_V,		///< 线极化方向-垂直极化
	EM_UDITUNER_POLARISATION_L,     	///< 圆极化方向-左旋极化
	EM_UDITUNER_POLARISATION_R      	///< 圆极化方向-右旋极化
}CSUDITunerChannelPolar_E;

/**@brief QPSK驱动Tone burst 定义*/
typedef enum 
{
	EM_UDITUNER_TONEBURST_NONE,		///< Tone burst选择关闭
	EM_UDITUNER_TONEBURST_A,		///< Tone burst选择A通道
	EM_UDITUNER_TONEBURST_B			///< Tone burst选择B通道
}CSUDITunerSwitchToneBurst_E;

/**@brief QPSK驱动Diseqc命令重复次数定义*/
typedef enum 
{
	EM_UDITUNER_NOREPEAT,		///< 没有DiSEqC Repeat 
	EM_UDITUNER_ONCEREPEAT,		///< 需要一次DiSEqC Repeat
	EM_UDITUNER_TWICEREPEAT		///< 需要两次DiSEqC Repeat
}CSUDITunerRepeatTime_E;

/**@brief QPSK驱动Diseqc1.2中天线转动方向定义*/
typedef enum 
{
	EM_UDITUNER_EASTERN,		///< 向东转动
	EM_UDITUNER_WESTERN		///< 向西转动
}CSUDITunerRotateDirection_E;
	
/**@brief QPSK驱动LNB类型定义*/
typedef enum 
{
	EM_UDITUNER_SINGLE,				///< 单本振LNB 
	EM_UDITUNER_UNIVERSAL,				///< 双本振通用LNB:  高本振22K打开，低本振22K关闭
	EM_UDITUNER_UNIVERSAL_NEGLOGIC			///< 双本振LNB: 高本振22K关闭，低本振22K打开
}CSUDITunerLNBType_E;

/**@brief QPSK驱动12伏电压选择的LNB电压定义*/
typedef enum 
{
	EM_UDITUNER_SWITCH_0V,		///< 0伏特电压选择 
	EM_UDITUNER_SWITCH_12V		///< 12伏特电压选择
}CSUDITunerSwitch012V_E;

/**@brief QPSK驱动中的FEC的类型定义*/
typedef enum
{
	EM_UDITUNER_FEC_AUTO,			///< FEC方式自动选择 
	EM_UDITUNER_FEC1_2,			///< 1/2方式的FEC
	EM_UDITUNER_FEC2_3,			///< 2/3方式的FEC
	EM_UDITUNER_FEC3_4,			///< 3/4方式的FEC
	EM_UDITUNER_FEC5_6,			///< 5/6方式的FEC 
	EM_UDITUNER_FEC7_8,			///< 7/8方式的FEC
	EM_UDITUNER_FEC8_9,			///< 8/9方式的FEC
	EM_UDITUNER_FEC3_5,			///< 3/5方式的FEC 
	EM_UDITUNER_FEC4_5,			///< 4/5方式的FEC
	EM_UDITUNER_FEC9_10,		     	///< 9/10方式的FEC
	EM_UDITUNER_FEC6_7			///< 6/7方式的FEC 
}CSUDITunerQPSKFECRates_E;

/**@brief DVB-S/S2中常用的调制方式定义*/
typedef enum
{
	EM_UDITUNER_AUTO_PSK,		///< QPSK和8PSK自动识别方式
	EM_UDITUNER_QPSK,	        ///< QPSK调制方式
	EM_UDITUNER_8PSK		///< 8PSK调制方式
}CSUDITunerQPSKModulation_E;

/**@brief 卫星数字电视的标准定义*/
typedef enum
{
	EM_UDITUNER_DVBS_STANDARD,		///< DVB-S标准信号 
	EM_UDITUNER_DVBS2_STANDARD,		///< DVB-S2标准信号
	EM_UDITUNER_DVBS_AUTO_STANDARD  	///< DVB-S和DVB-S2自动识别
}CSUDITunerQPSKStandard_E;


/**@brief QPSK驱动Unicable中的卫星序号选择(一个Unicable LNB 同时最多支持2个卫星, 卫星A和卫星B)*/
typedef enum
{
	EM_UDITUNER_OPTION_NONE,		///< 没有可选卫星 
	EM_UDITUNER_OPTION_A,			///< Unicable选择卫星A
	EM_UDITUNER_OPTION_B			///< Unicable选择卫星B
}CSUDITunerQPSKOption_E;

/**@brief QPSK驱动Unicable中的卫星天线类型选择，一个Unicable LNB 最多支持8种类型*/
typedef enum 
{
	EM_UDITUNER_UNICABLE_NONE,			///< Unicable LNB 未定义
	EM_UDITUNER_UNICABLE_A_LB_VP,			///< Unicable LNB 选择:satellite A, Low Band, Vertical Polarization
	EM_UDITUNER_UNICABLE_A_HB_VP,			///< Unicable LNB 选择:satellite A, High Band, Vertical Polarization
	EM_UDITUNER_UNICABLE_A_LB_HP,			///< Unicable LNB 选择:satellite A, Low Band, Horizontal Polarization
	EM_UDITUNER_UNICABLE_A_HB_HP,			///< Unicable LNB 选择:satellite A, High  Band, Horizontal Polarization
	EM_UDITUNER_UNICABLE_B_LB_VP,			///< Unicable LNB 选择:satellite B, Low Band,  Vertical Polarization
	EM_UDITUNER_UNICABLE_B_HB_VP,			///< Unicable LNB 选择:satellite B, High  Band, Vertical Polarization
	EM_UDITUNER_UNICABLE_B_LB_HP,			///< Unicable LNB 选择:satellite B, Low Band, Horizontal Polarization
	EM_UDITUNER_UNICABLE_B_HB_HP			///< Unicable LNB 选择:satellite B, High  Band, Horizontal Polarization
}CSUDITunerQPSKUnicableLNBType_E;

/**@brief QPSK驱动中的LNB电压状态定义*/
typedef enum
{
	EM_UDITUNER_LNB_OFF,		///< LNB电压关闭
	EM_UDITUNER_LNB_AUTO,		///< LNB电压选择自动 
	EM_UDITUNER_LNB_14V,		///< LNB电压选择14伏特
	EM_UDITUNER_LNB_18V		///< LNB电压选择18伏特
}CSUDITunerQPSKLNBStatus_E;

/**@brief QPSK驱动中用于DVB-S2中的导频开关定义*/
typedef enum
{
	EM_UDITUNER_PILOT_OFF,		///< 导频打开 
    	EM_UDITUNER_PILOT_ON		///< 导频关闭 
}CSUDITunerQPSKPilot_E;

/**@brief 卫星电视tuner的信号数据结构(同时满足DVB-S/S2)*/
typedef struct 
{
	unsigned int	        m_uFrequency;					///< 频率，单位KHz
	unsigned int	m_uSymbolRate_24			:24;	///< 符号率，单位Kbps
	unsigned int	m_uFECInner_4				:4;	///< CSUDITunerQPSKFECRates_E 类型枚举
	unsigned int	m_uPolar_4				:4;	///< CSUDITunerChannelPolar_E 类型枚举
	unsigned int	        m_uPilot;                          		///< CSUDITunerQPSKPilot_E 类型枚举,DVB_S2信号专用
	unsigned int	m_uModulations_2             :2;			///< CSUDITunerQPSKModulation_E 类型枚举
	unsigned int    m_uStandards_2               :2;			///< CSUDITunerQPSKStandard_E 类型枚举 
	unsigned int	m_uReserved_28               :28;		///< 保留使用
}CSUDITunerSatelliteDeliver_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner中DiSEqC1.0标准的参数*/
typedef struct 
{
	unsigned int m_uCommitted					:4;		///< CSUDITunerSwitchDisqec10Port_E 类型枚举
	unsigned int m_uToneburst					:2;		///< CSUDITunerSwitchToneBurst_E 类型枚举
	unsigned int m_uRepeatedTimes				:2;		///< 重复次数CSUDITunerRepeatTime_E 类型枚举
	unsigned int m_uReserved					       :24;		///< 保留使用
}CSUDITunerDiSEqC1020_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner中DiSEqC1.1标准的参数*/
typedef struct 
{
	unsigned int m_uCommitted					:4;	///< DiSEqC1.0端口,见CSUDITunerSwitchDisqec10Port_E 类型枚举
	unsigned int m_uUncommitted					:4;	///< DiSEqC1.1端口, 1----16
	unsigned int m_uToneburst					:2;     ///< CSUDITunerSwitchToneBurst_E 类型枚举
	unsigned int m_uRepeatedTimes					:2;	///< 重复次数CSUDITunerRepeatTime_E 类型枚举
	unsigned int m_uReserved					:20;	///< 保留使用
}CSUDITunerDiSEqC1121_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner中DiSEqC1.2标准的参数*/
typedef struct 
{
	unsigned int m_uUsals						:2;		///< 需转位置，位置值取自m_uIndex 2 -根据用户设置的经纬度进行相应转锅操作
	unsigned int m_uIndex						:5;		///< 需转到的位置值。
	unsigned int m_uReserved					:25;		///< 保留使用
}CSUDITunerDiSEqC1222_S;


/**@brief 卫星数字电视(DVB-S/S2)tuner的卫星位置参数结构(同时满足DVB-S/S2)*/
typedef struct 
{
	double m_dLocalLongitude;					///< 本地的经度位置(经度值*10,和DTV中定义一致)
	double m_dLatitude;						///< 本地的纬度位置(纬度值*10,和DTV中定义一致)
	double m_dSatLongitude;						///< 卫星所在的经度位置(经度值*10,和DTV中定义一致)
	unsigned int m_uWestEastFlag			:1;		///< 卫星所在的位置的东西方标志：0 -EM_UDITUNER_EASTERN, 1 - EM_UDITUNER_WESTERN
	unsigned int m_uReserved			:31;		///< 保留使用
}CSUDITunerSatellitePar_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner的LNB参数数据结构*/
typedef struct 
{
	unsigned int m_uLowFrequency;				///< low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]
	unsigned int m_uHighFrequency;				///< high LNB frequency in Universal Mod [unit:KHz]
	unsigned int m_uCenterFrequency;			///< middle LNB frequency in Universal Mod [unit:KHz]
	unsigned int m_uSatCRNo			:16;    ///< 多户联用的信道路由器(SatCR)的通道选择, 0---7
	unsigned int m_uSatCRLNBNo			:16;	///< 多户联用的信道路由器(SatCR)的LNB选择, CSUDITunerQPSKUnicableLNBType_E枚举类型定义
	unsigned int m_uLnbType			:4;	///< 高频头类型CSUDITunerLNBType_E 类型枚举
	unsigned int m_uLnbPower			:2;	///< CSUDITunerSwitchStatus_E 类型枚举
	unsigned int m_uOptions                 	:2;     ///<多户联用的信道路由器(SatCR)的卫星选择, CSUDITunerQPSKOption_E枚举类型定义
	unsigned int m_uReserved		 	:24;	///< 保留使用
}CSUDITunerLNB_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner的天线参数据结构(同时满足DVB-S/S2)*/
typedef struct 
{
	unsigned int m_uSwitch22k					:1;		///< CSUDITunerSwitchStatus_E 类型枚举
	unsigned int m_uSwith12v					:1;		///< CSUDITunerSwitch012V_E类型枚举
	unsigned int m_uDisEqCType					:1;		///< 扩展使用
	unsigned int m_uAntennaKey					:8;		///< 扩展使用
	unsigned int m_uReserved					:21;	        ///< 保留使用
}CSUDITunerAntennaPar_S;

/**@brief tuner模块统一的信号频率、符号率偏移数据结构*/
typedef struct 
{
	unsigned int m_uOffsetFrequency_23		:23;	///< 频率的偏移值，单位KHz
	unsigned int m_uOffsetSymbolRate_8		:8;	///< 符号率的偏移值，单位Kbps
	unsigned int m_uIQ_1				:1;	///< 0-Normal; 1-Inverse
}CSUDITunerTransponderOffset_S;

/**@brief 卫星电视tuner的带disqec信息的信号数据结构(同时满足DVB-S/S2)*/
typedef struct 
{
	CSUDITunerSatelliteDeliver_S m_sSatelliteDeliver;	///< 卫星机频点信息结构体
	CSUDITunerLNB_S			m_sLNB;			///< 高频头信息结构体
	CSUDITunerDiSEqC1020_S		m_sDiSEqC1020;           ///< diseq1.0信息结构体
	CSUDITunerDiSEqC1121_S		m_sDiSEqC1121;           ///< diseq1.1信息结构体
	CSUDITunerDiSEqC1222_S		m_sDiSEqC1222;           ///< diseq1.2信息结构体
	CSUDITunerSatellitePar_S	m_sSatellitePar;	///< 卫星参数信息结构体
	CSUDITunerAntennaPar_S		m_sAntennaPar;           ///< 天线参数信息结构体
	CSUDITunerTransponderOffset_S	m_sTransponderOffset;	///< 频点偏移值信息结构体
}CSUDITunerSRCSatelliteDeliver_S;


/**@brief tuner模块统一信号参数定义*/
typedef union 
{
	CSUDITunerCableDeliver_S	m_sCableDeliver;                ///< 有线机频点信息结构体
	CSUDITunerSRCSatelliteDeliver_S	m_sSourceSatelliteDeliver;	///< 卫星机频点信息结构体
	CSUDITunerTerrestrialDeliver_S	m_sTerrestrialDeliver;          ///< 地面机频点信息结构体
}CSUDITunerSRCDeliver_U;

/**@brief tuner 类型定义*/
typedef enum
{
	EM_UDITUNER_TYPE_UNKNOWN,
	EM_UDITUNER_TYPE_QAM,		///<  有线tuner类型,主要采用QAM调制方式
	EM_UDITUNER_TYPE_COFDM,	   	///<  地面tuner类型,主要采用COFDM调制方式
	EM_UDITUNER_TYPE_QPSK		///<  卫星tuner类型,主要采用QPSK,8PSK调制方式
}CSUDITunerType_E;

/**@brief tuner数据传输类型定义*/
typedef enum
{
	EM_UDITUNER_DATATRANSFER_UNKNOWN,	///< 非法值
	EM_UDITUNER_DATATRANSFER_DMA,		///< DMA数据传输模式高频头，即数据以DMA形式直接传输到Demux，这种情况下需要使用DMA类型的Demux进行后处理
	EM_UDITUNER_DATATRANSFER_SOFT		///< 软注入数据传输模式的高频头，即数据以软件形式注入到Demux，这种情况下需要使用INJECT类型的Demux进行后处理，并自行将数据注入到Demux
}CSUDITunerDataTransferType_E;

/**@brief Tuner信号信息类型枚举定义 */
typedef enum
{
	EM_UDITUNER_SIGNAL_QUALITY      = 1 << 0,       ///<信号质量
	EM_UDITUNER_SIGNAL_STRENGTH     = 1 << 1,       ///< 信号强度，等级
	EM_UDITUNER_ERROR_RATE          = 1 << 2,       ///< 误码率
	EM_UDITUNER_SIGNAL_LEVEL        = 1 << 3,       ///< 信号输入电平
	EM_UDITUNER_SIGNAL_CN           = 1 << 4,       ///<信号载噪比
	EM_UDITUNER_CUR_FREQU           = 1 << 5        ///<信号频率
}CSUDITunerSignalInfoType_E;

/**@brief tuner设备能力信息定义*/
typedef struct
{
	CSUDITunerType_E m_eType;	   ///< Tuner类型
	CSUDITunerDataTransferType_E m_eTransferType;	///< Tuner数据传输模式
	CSUDI_BOOL m_bPIDFilterSupport;		///< 是否支持PID过滤，部分高频头可支持仅将部分PID的数据输出，以减少带宽，对软注入形式的高频头，DVB-TH，CMMB等形式的高频头特别有用
}CSUDITunerInfo_S;

/**@brief tuner回调消息类型定义

Tuner锁频状态变化引起的事件请参考下图:
@note 
	- Init 指tuner初始化完成之后还没有进行锁频之前的状态
	- Locked 指tuner进行锁频操作，并且锁频成功之后的状态
	- Unlocked 指tuner没有锁频成功的状态
	- Init -> Locked 是指tuner进行第一次锁频操作，锁频成功之后发送CONNECTED消息
	- Init -> Unlocked 是指tuner进行第一次锁频操作，锁频失败之后，不断尝试进行锁频操作，期间会发送SEARCHING消息直到锁频成功
	- Unlocked -> Locked 是指tuner从锁频失败状态之后再次锁频成功，成功之后发送CONNECTED消息
	- Locked->Unlocked 是指tuner锁频失败后，发送一次LOST消息，并不断进行锁频操作，期间会发送SEARCHING消息直到锁频成功
@image HTML udi_tuner_event.jpg
*/
typedef enum
{
      EM_UDITUNER_SIGNAL_CONNECTED,   ///< 频点锁定成功
      EM_UDITUNER_SIGNAL_LOST,        ///< 由锁定转为失锁时发送此消息
      EM_UDITUNER_SIGNAL_SEARCHING    ///< 此消息表示锁频失败，但会继续尝试锁频，直到锁频成功。正常情况下在两个能锁定的频点间切换，不需要发此消息。
}CSUDITunerCallbackEvent_E;

/**@brief tuner回调消息数据结构定义*/
typedef struct 
{  										
	 unsigned int  m_dwTunerId;                    ///< 回调消息对应的Tuner设备号
	 CSUDITunerCallbackEvent_E m_eEvent;      		///< 见前面CSUDITunerCallbackEvents_E定义 
	 unsigned int  m_uFrequency;                    ///< 回调消息对应的频点，单位KHz
	 unsigned int  m_uSymbolRate;                   ///< 回调消息对应的符号率，单位Kbps
	 unsigned int  m_uBandWidth;                    ///< 回调消息对应的波段，见CSUDITunerBanuidth_E定义
	 unsigned int  m_uQAMMode;                      ///< 回调消息对应的调制方式，见CSUDITunerQAMMode_E定义
	 void *        m_pvUserData;                    ///< 用户在注册回调函数时传入的指针
}CSUDITunerCallbackMessage_S;

/**
@brief tuner事件回调函数原型定义

@param[in] eType 产生回调的高频头类型
@param[in] psSourceCallbackMessage 回调消息数据，详见CSUDITunerCallbackMessage_S定义。
*/
typedef  void ( *CSUDITunerCallback  )( CSUDITunerType_E eType, CSUDITunerCallbackMessage_S* psMessage);

/**@brief tuner信号信息数据结构定义*/
typedef struct 
{
	int 		 m_nQuality;	    ///< 信号质量（0～100）
	int		     m_nStrength;	    ///< 信号强度（0 ～100）
	int		     m_nErrorRate;	    ///< 误码率，1*10^7个码元中错误码元的个数．(除以10^7就得到误码率）,失锁时，误码率为1*10^7，即100%误码。	
	unsigned int m_nCurrentFreq;	///< 用来保存获取的信号频率,unit[KHz]
	double 		 m_dSignalLevel;    ///< 信号电平,精确到小数点后一位,unit(QAM: dBuv; COFDM: dBm; QPSK:dBm)	
	double 		 m_dSignalCN;	    ///< 载噪比,精确到小数点后一位, unit[dB]
	int		     m_nReserved[3];	///< 保留参数
}CSUDITunerSignalInfo_S;

/**
@brief 根据指定的频点进行锁频操作.

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[in] punDeliver 用户存放频点信息的数据块的地址。
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回错误代码。\n
-- CSUDITUNER_ERROR_BAD_PARAMETER：参数错误。\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID：非法ID。\n
-- CSUDITUNER_ERROR_I2C_ERR：I2C总线错误。\n
@note 本接口异步执行，返回成功仅表示命令已接收，不表示命令执行结果。
@note 本接口返回后，如果本次锁的频点与上次不同，则必须马上停止掉上个频点的demux数据接收
*/
CSUDI_Error_Code CSUDITunerConnect(CSUDI_UINT32 dwTunerId, const CSUDITunerSRCDeliver_U* punDeliver);

 /**
@brief 注册Tuner锁频信息回调

@param[in] dwTunerId tuner设备号。
@param[in] fnTunerCallback 回调函数句柄。
@param[in] pvUserData 用户传入的自定义指针,用来通知驱动返回给用户的数据信息。
@return 如果成功返回CSUDI_SUCCESS, 失败返回错误代码
@note
- 回调函数的注册个数至少支持8个，如果应用注册回调个数超过所支持个数将返回CSUDITUNER_ERROR_NO_MEMORY
- 一组tuner id,callback,userdata共同决定一个Tuner锁频信息回调，当两次注册它们完全一样时，第二次将返回CSUDITUNER_ERROR_ALREADY_ADDED
*/
CSUDI_Error_Code CSUDITunerAddCallback(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData );

 /**
@brief 删除指定的锁频回调函数的注册

@param[in] dwTunerId tuner设备号。
@param[in] fnTunerCallback 回调函数句柄。
@param[in] pvUserData 同CSUDITunerAddCallback的pvUserData
@return成功返回CSUDI_SUCCESS 如果该回调函数根本就没有注册，将返回CSUDI_FAILURE
@note 对于可移动的Tuner，当其被拔出后，仍需要主动删除其对应的回调注册，否则仍会占用部分资源（主要是内存）
*/
CSUDI_Error_Code CSUDITunerDelCallback(CSUDI_UINT32 dwTunerId,CSUDITunerCallback  fnTunerCallback, void *pvUserData);

 /**
@brief 取得当前正在锁定的信号信息

@param[in] dwTunerId 用于锁频的Tuner设备的设备号
@param[in] u32SignalInfoMask 需获取的信号信息类型,取值为CSUDITunerSignalInfoType_E中的枚举值或枚举中任意值相或，对应位为1则返回该信息；为0则不返回，此时对应结构的值是不确定的。
@param[out] psSignalInfo 用来保存获取的信号信息。
@return 
- 成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
- 如果失败返回如下错误代码:\n
-- CSUDITUNER_ERROR_BAD_PARAMETER ：参数错误。\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID：非法设备ID。\n 
-- CSUDITUNER_ERROR_I2C_ERROR：I2C总线错误。\n 
@note 获取所有这些信息可能会涉及一些不同的Tuner操作，为提高效率，请尽量仅获取需关注的信息，而不是全部。因为大多数情况下，需要的信息越多，该接口需要的时间越长
@note 该接口一定要迅速返回，不必获取非常精确的信息。不建议在该接口中直接调用驱动阻塞接口查询。建议在udi层定时查询，将所需的信息先取出来，调用本接口时直接获取即可。
*/
CSUDI_Error_Code CSUDITunerGetSignalInfo( CSUDI_UINT32 dwTunerId, CSUDI_UINT32 u32SignalInfoMask, CSUDITunerSignalInfo_S *psSignalInfo);

 /**
@brief 取得Tuner设备的能力信息

@param[in] dwTunerId 用于标记Tuner设备的设备号
@param[out] psTunerInfo 用来保存获取的Tuner能力信息
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDITunerGetDeviceInfo( CSUDI_UINT32 dwTunerId, CSUDITunerInfo_S *psTunerInfo);

/**
@brief 获取所有设备的Id

包括可移动的Tuner设备和固定的设备
@param[out] pdwIds 用于存储所有存在的设备的Id
@param[in] nMaxIdCnt pdwIds数组的大小,即最多能返回多少设备Id
@param[out] pnActIdCnt 实际的设备ID数
@return 成功返回CSUDI_SUCCESS，失败返回错误代码值
@note
- 为保持兼容，所有非可移动设备的设备号仍从0开始索引
- 允许pdwIds为CSUDI_NULL，这种情况下表示应用仅关注有多少个tuner设备，但不关心它们具体的Id
*/
CSUDI_Error_Code CSUDITunerGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt);

/**
@brief 增加Tuner硬过滤Pid

- 对于m_bPIDFilterSupport为CSUDI_TRUE的Tuner，必须通过该接口增加硬过滤Pid后，方可获取数据。
- 对于m_bPIDFilterSupport为CSUDI_FALSE的Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED，并且不需要调用该接口即可获取所有数据
@param[in] dwTunerId Tuner设备号
@param[in] nPID 需添加过滤的数据Pid，添加后该Pid的数据可通过进入对应的后处理模块（一般为Demux）
@return 成功返回CSUDI_SUCCESS，失败返回错误代码值
@note 初始状态下，Tuner不通过任何Pid的数据
*/
CSUDI_Error_Code CSUDITunerAddPID (CSUDI_UINT32 dwTunerId ,int nPID);

/**
@brief 删除Tuner硬过滤Pid

- 对于m_bPIDFilterSupport为CSUDI_TRUE的Tuner，可以通过该接口删除CSUDITunerAddPID接口增加的硬过滤Pid，阻止获取对应PID的数据。
- 对于m_bPIDFilterSupport为CSUDI_FALSE的Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED，并且不需要调用该接口即可获取所有数据
@param[in] dwTunerId Tuner设备号
@param[in] nPID 需删除过滤的数据Pid，删除后该Pid的数据不可通过进入对应的后处理模块（一般为Demux）
@return 成功返回CSUDI_SUCCESS，失败返回错误代码值
@note nPID为~0表示删除该Tuner下的所有pid过滤，即此后无法通过该Tuner获取任何数据（除非再次调用CSUDITunerAddPID）
@note 初始状态下，Tuner不通过任何Pid的数据
@see CSUDITunerAddPID
*/
CSUDI_Error_Code CSUDITunerRemovePID (CSUDI_UINT32 dwTunerId ,int nPID);

/*-----------------以下接口定义可移动Tuner的相关接口---------------------*/
/**
@brief 添加设备通知回调函数

详见RDI模块CSUDIRDIAddCallback_F接口定义，仅用于处理“TUNER”类型的可移动设备
@param[in] fnCallback 回调函数,当设备状态发生变化时调用该函数
@param[in] pvUserData 用户数据,将作为回调函数的pvUserData参数返回
@return 成功添加返回CSUDI_SUCCESS,失败返回错误代码值
@note 添加回调前产生的设备变化信息,不会补充通知.即若开机时这些设备就存在,调用本函数时不会得到通知.
@note 若平台不需要支持任何可移动Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDITunerRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief 删除设备通知回调函数

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“TUNER”类型的可移动设备
@param[in] fnCallback 回调函数,之前注册的函数
@param[in] pvUserData 用户数据,必须与注册时的一致
@return 成功删除返回CSUDI_SUCCESS,失败返回错误代码值
@note fnCallback和pUserData一起唯一确定一个回调函数
@note 若平台不需要支持任何可移动Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDITunerRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief 安全移除可移动Tuner设备

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“TUNER”类型的可移动设备
@param[in] dwTunerId 目标设备ID
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note
- 只能删除可移动设备，试图删除其它设备类型则返回CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED，通过设备的设备Id可判断设备类型，删除设备后不可再通过该设备Id获取设备信息。用户调用该接口会触发EM_UDIRDI_PLUGOUT_SAFE类型的PLUGOUT事件。 
- 若平台不需要支持任何可移动Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDITunerRDIRemoveDev(CSUDI_UINT32 dwTunerId);

/**
@brief 从可移动Tuner读取数据

成功锁频并设置过滤PID(如果需要)后，可通过该接口读取到TS流数据
@param[in] dwTunerId 目标设备Id
@param[in] pvData 指向读入数据的Buffer
@param[in] nBuffLength pvData的长度
@param[out] pnReadLength 实际读到的数据长度
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 接口应立刻返回，若不能读到任何数据，则*pnReadLength==0，但仍返回成功
@note 详见RDI模块中CSUDIRDIIORead_F原形
@note 若平台不需要支持任何可移动Tuner，该接口返回CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDITunerIORead(CSUDI_UINT32 dwTunerId,void* pvData,int nBuffLength,int * pnReadLength);


/**
@brief 获取一组PlpID,PlpID为0~255的整数

@param[in] dwTunerId 用于锁频的Tuner设备的设备号。
@param[out] pbPlpIds 用于返PLP ID的数组，数组大小应该至少256，否则会由于可用的PLP ID数量超过数组大小而返回错误
@param[in] nPlpIdsCount pbPlpIds指向的数组大小
@param[out] pnRetIdCount 用于返回实际保存到pbPlpIds中的PlpID个数
@return 
- 如果成功返回CSUDI_SUCCESS。
- 如果失败返回错误代码。\n
-- CSUDITUNER_ERROR_BAD_PARAMETER：参数错误。\n
-- CSUDITUNER_ERROR_INVALID_DEVICE_ID：非法ID。\n
-- CSUDITUNER_ERROR_I2C_ERR：I2C总线错误。\n
@note 本接口为DVB-T2专用。
*/
CSUDI_Error_Code CSUDITunerT2GetPlpInfo(CSUDI_UINT32 dwTunerId, CSUDI_UINT8* pbPlpIds, int nPlpIdsCount, int* pnRetIdCount);

#ifdef __cplusplus
}
#endif

/** @} */

#endif 

