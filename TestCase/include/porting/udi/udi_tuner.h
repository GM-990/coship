/**@defgroup CSTUNER tuner 定义了统一的tuner接口操作函数
@brief 本模块主要定义了统一的tuner接口操作函数
@brief 统一的tuner接口支持DVB-C标准(QAM调制方式),DVB-S/DVB-S2标准(QPSK调制方式),DVB-T标准(COFDM和DMB-TH调制方式),以及手机电视CMMB标准的tuner

@version 0.1 2008/10/23 初始版本
@{
*/

#ifndef _UDI_TUNER_H_
#define _UDI_TUNER_H_

#include "udi_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif
	

/**@brief QPSK驱动TS流(传输流)时钟极性定义*/
typedef enum
{
	LOW_TO_HIGH,	/**< 上升延*/
	HIGH_TO_LOW	    /**< 下降延*/
}T_ClockPolarity;

/**@brief QPSK驱动Diseqc1.0端口定义*/
typedef enum  
{
	DISEQC10_OFF,	/**< diseqc1.0通道选择关闭*/
	PORT_4_A,		/**< diseqc1.0四选一开关的A通道*/
	PORT_4_B,		/**< diseqc1.0四选一开关的B通道*/
	PORT_4_C,		/**< diseqc1.0四选一开关的C通道*/
	PORT_4_D,		/**< diseqc1.0四选一开关的D通道*/
	PORT_2_A,		/**< diseqc1.0二选一开关的A通道*/
	PORT_2_B		/**< diseqc1.0二选一开关的B通道 */
}Switch_Port;

/**@brief 地面tuner驱动通道Band定义*/
typedef enum 
{
	BAND_LOW,		/**< 低波段*/
	BAND_HIGH		/**< 高波段*/
}Channel_Band;

/**@brief QPSK驱动信号极化方向定义*/
typedef enum 
{
	POLARISATION_H,		/**< 线极化方向-水平极化*/
	POLARISATION_V,		/**< 线极化方向-垂直极化*/
	POLARISATION_L,     /**< 圆极化方向-左旋极化*/
	POLARISATION_R      /**< 圆极化方向-右旋极化*/
}Channel_Polarisation;

/**@brief QPSK驱动Diseqc1.0中的Tone burst(多选一通道选择 的级联模式,与多选一组合可以支持2*N个通道选择)定义*/
typedef enum 
{
	TONEBURST_NONE,		/**< Tone burst选择关闭*/
	TONEBURST_A,		/**< Tone burst选择A通道*/
	TONEBURST_B			/**< Tone burst选择B通道*/
}Switch_ToneBurst;

/**@brief QPSK驱动Diseqc中的通道重复选择定义,DiSEqC Repeat(指令重复)是一项很实用的功能,让每次指令重复发送多次,可以保证切换可靠。不过,重复太多也没意思,换星操作会迟钝的*/
typedef enum 
{
	NOREPEAT,		/**< 没有DiSEqC Repeat */
	ONCEREPEAT,		/**< 需要一次DiSEqC Repeat*/
	TWICEREPEAT		/**< 需要两次DiSEqC Repeat*/
}Repeat_Time;

/**@brief QPSK驱动Diseqc1.2中天线转动方向定义*/
typedef enum 
{
	EASTERN,	/**< 向东转动*/
	WESTERN		/**< 向西转动*/
}RotateDirection;
	
/**@brief QPSK驱动LNB(LNB又叫高频头（Low Noise Block）即低噪声下变频器，\n
	其功能是将由馈源传送的卫星信号经过放大和下变频，把Ku或C波\n
	段信号变成L波段，经同轴电缆传送给卫星接收机。 \n
	它是由微波低噪声放大器，微波混频器，第一本振和\n
	第一中频前置放大器组成，一般分Ｃ波段用的C头,和偏馈使用\n
	的KU头,LNB上都会有探针，电路对这个探针检测到的卫星下行信\n
	号进行低噪声放大和下变频处理，产生950~2150MHZ带宽的第一中频\n
	信号经过馈线输送数字调谐解调器)类型定义，Universal LNB是双本振\n
	通用LNB*/
typedef enum 
{
	SINGLE,					/**< 单本振LNB */
	UNIVERSAL,				/**< 双本振通用LNB*/
	UNIVERSAL_NegLogic		/**< 需要添加说明 */
}LNBType;

/**@brief QPSK驱动12伏电压选择的LNB电压定义*/
typedef enum 
{
	SWITCH_0V,		/**< 0伏特电压选择 */
	SWITCH_12V		/**< 12伏特电压选择*/
}Switch0_12V;

/**@brief QPSK驱动中的FEC也叫前向纠错(Forward Error Correction)，\n
是利用数据进行传输冗长信息的方法,当传输中出现错误\n
,将允许接收器再建数据。以7/8为例,其实际意义是,在一个\n
TS流中,只有7/8的内容是装有节目内容的PES流,而另外的1/8内\n
容,则是用来保护数据流不发生变异的纠错码。还用上面的\n
例子做比喻,如果整个节目的符码率是毛重的话,则7/8的节目\n
内容好比是净重,而1/8的纠错码就是包装箱的重量。*/
typedef enum
{
	FEC_AUTO,		/**< FEC方式自动选择 */
	FEC1_2,			/**< 1/2方式的FEC*/
	FEC2_3,			/**< 2/3方式的FEC*/
	FEC3_4,			/**< 3/4方式的FEC*/
	FEC5_6,			/**< 5/6方式的FEC */
	FEC7_8,			/**< 7/8方式的FEC*/
	FEC8_9,			/**< 8/9方式的FEC*/
	FEC3_5,			/**< 3/5方式的FEC */
	FEC4_5,			/**< 4/5方式的FEC*/
	FEC9_10,		/**< 9/10方式的FEC*/
	FEC6_7			/**< 6/7方式的FEC */
}CSQPSK_FECRates;

/**@brief DVB-S2中的导频开关定义*/
typedef enum
{
	PILOT_OFF,		/**< 导频打开*/
    	PILOT_ON		/**< 导频关闭*/
}CSQPSK_PIILOT;

/**@brief DVB-S/S2中常用的调制方式定义*/
typedef enum
{
	CS_AUTO_PSK,
	CS_QPSK,	/**< Quadrature Phase Shift Keying */
	CS_8PSK		/**< Eight Phase Shift Keying*/
}CSQPSK_MODULATION;

/**@brief 卫星数字电视的标准定义*/
typedef enum
{
	DVBS_STANDARD,		/**< DVB-S标准信号 */
	DVBS2_STANDARD,		/**< DVB-S2标准信号*/
	DVBS_AUTO_STANDARD    /** <DVB-S和DVB-S2自动选择>*/
}CSQPSK_STANDARD;

/**@brief  Diseqc1.1中的可选通道选择,类似于Diseqc1.0中的Tone burst*/
typedef enum
{
	OPTION_NONE,		/**< 没有可选通道 */
	OPTION_A,			/**< 选择可选通道A */
	OPTION_B			/**< 选择可选通道B */
}CSQPSK_OPTION;

/**@brief QPSK驱动中的LNB电压状态定义*/
typedef enum
{
	LNB_OFF,		/**< LNB电压关闭*/
	LNB_AUTO,		/**< LNB电压选择自动 */
	LNB_14V,		/**< LNB电压选择14伏特*/
	LNB_18V		    /**< LNB电压选择18伏特*/
}CSQPSK_LNB_Status;

/**@brief 卫星电视tuner的信号数据结构(同时满足DVB-S/S2)*/
typedef struct _CSTUNSatelliteDeliver_S
{
	DWORD	m_dwFrequency;						/**< 频率，单位MHZ*/
	DWORD	m_dwSymbolRate_24			:24;	/**< 符号率，单位KHZ*/
	DWORD	m_dwFECInner_4				:4;		/**< CSQPSK_FECRates类型枚举*/
	DWORD	m_dwPolarization_2			:2;		/**< Channel_Polarisation类型枚举*/
	DWORD	m_ucPilot;                          /**< CSQPSK_PIILOT类型枚举--> add for s2*/
	DWORD	m_ucSatCRNo;                        /**< 多户联用的信道路由器(SatCR)的通道选择*/
	DWORD	m_ucSatCRLNBNo;                     /**< 多户联用的信道路由器(SatCR)的LNB选择*/
	DWORD	m_dwOptions                 :2;     /**<CSQPSK_OPTION类型枚举*/
	DWORD	m_dwModulations             :2;		/**< CSQPSK_MODULATION类型枚举*/
	DWORD  	m_dwstandards               :1;		/**< CSQPSK_STANDARD类型枚举*/
	DWORD	m_dwreserved                :2;		/**< 保留使用*/
}CSTUNSatelliteDeliver_S;


/**@brief QAM_驱动接口返回值定义*/
typedef enum
{
	CS_NOT_DEFINED,	    /**< 未定义的QAM星座，一般情况下标示无效参数*/
	CS_QAM16,			/**< 16QAM星座*/
	CS_QAM32,			/**< 32QAM星座*/
	CS_QAM64,			/**< 64QAM星座*/
	CS_QAM128,			/**< 128QAM星座*/
	CS_QAM256			/**< 256QAM星座 */
}T_QAMMode;

/**@brief 有线数字电视(DVB-C)tuner的信号参数数据结构*/
typedef struct _CSTUNCableDeliver_S
{
	DWORD m_dwFrequency;					/**< 频率，单位KHZ*/
	DWORD m_dwSymbolRate_24				:24;/**< 符号率，单位KHZ*/
	DWORD m_dwModulation_8				:8;	/**< T_QAMMode类型枚举*/
}CSTUNCableDeliver_S;


/**@brief cofdm 信号带宽定义*/
typedef enum _T_BANDWIDTH							
{
	BAND_8M,    /**<带宽8M*/
	BAND_7M,    /**<带宽7M*/
	BAND_6M     /**<带宽6M*/
}T_BANDWIDTH;

/**@brief 地面数字电视(DVB-T)tuner的信号参数数据结构)*/
typedef struct _CSTUNTerrestrialDeliver_S
{
	DWORD m_dwFrequency;					 /**< 频率，单位KHZ*/
	DWORD m_dwBanwith_3					:3;	 /**< T_BANDWIDTH类型枚举*/
	DWORD m_dwFlag_2					:2;		/**< DVB-T/DVB-T2标志:00-T;01-T2;10,11保留*/
	DWORD m_dwPlpID_8					:8;     /**< T2存储PLPID*/
	DWORD m_dwReserved_19				:19;  	/**< 保留使用*/
}CSTUNTerrestrialDeliver_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner中DiSEqC1.0标准的参数*/
typedef struct _CSTUNDiSEqC1020_S
{
	DWORD m_dwToneburst					:2;		/**< Switch_ToneBurst类型枚举*/
	DWORD m_dwCommitted					:4;		/**< Switch_Port类型枚举*/
	DWORD m_dwReserved					:26;	/**< 保留使用*/
}CSTUNDiSEqC1020_S;


/**@brief 卫星数字电视(DVB-S/S2)tuner中DiSEqC1.1标准的参数*/
typedef struct _CSTUNDiSEqC1121_S
{
	DWORD m_dwUncommitted					:4;	/**< 参见DiSEqC1.1标准*/
	DWORD m_dwCommitted					:4;		/**< 参见DiSEqC1.1标准*/
	DWORD m_dwRepeatedTimes					:2;	/**< 重复次数Repeat_Time类型枚举*/
	DWORD m_dwReserved					:22;	/**< 保留使用*/
}CSTUNDiSEqC1121_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner中DiSEqC1.2标准的参数*/
typedef struct _CSTUNDiSEqC1222_S
{
	DWORD m_dwUsals						:2;		/**< 需转位置，位置值取自m_dwIndex 2 -根据用户设置的经纬度进行相应转锅操作*/
	DWORD m_dwIndex						:5;		/**< 需转到的位置值。*/
	DWORD m_dwReserved					:25;	/**< 保留使用*/
}CSTUNDiSEqC1222_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner的LNB参数数据结构*/
typedef struct _CSTUNLNB_S
{
	DWORD m_dwLowFrequency;				    /**< low LNB frequency in Universal Mod or 
													LNB frequency in Single Mod [unit:KHz]*/
	DWORD m_dwHighFrequency;				/**< high LNB frequency in Universal Mod [unit:KHz]*/
	DWORD m_dwCenterFrequency;			    /**< middle LNB frequency in Universal Mod [unit:KHz]*/
	DWORD m_dwLnbType					:4;	/**< 高频头类型LNBType类型枚举*/
	DWORD m_dwLnbPower					:2;	/**< CSHDISwitch类型枚举*/
	DWORD m_dwLnbVoltage				:2;	/**< CSQPSK_LNB_Status类型枚举*/
	DWORD m_dwReserved					:24;/**< 保留使用*/
}CSTUNLNB_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner的卫星位置参数结构(同时满足DVB-S/S2)*/
typedef struct _CSTUNSatellitePar_S
{
	DWORD m_dwOrbitalPosition			:16;	/**< 卫星所在的经度位置*/
	DWORD m_dwWestEastFlag				:1;		/**< 卫星所在的位置的东西方标志：0 -EASTERN, 1 - WESTERN*/
	DWORD m_dwReserved					:15;	/**< 保留使用*/
}CSTUNSatellitePar_S;

/**@brief 卫星数字电视(DVB-S/S2)tuner的天线参数据结构(同时满足DVB-S/S2)*/
typedef struct _CSTUNAntennaPar_S
{
	DWORD m_dwSwitch22k					:1;		/**< CSHDISwitch类型枚*/
	DWORD m_dwSwith12v					:1;		/**< Switch0_12V类型枚举*/
	DWORD m_dwTunerMask					:4;		/**< 0x01 - 代表1号tuner，0x02 - 代表2号tuner*/
	DWORD m_dwDisEqCType				:1;		/**< 需添加注释*/
	DWORD m_dwAntennaKey				:8;		/**< 需添加注释*/
	DWORD m_dwSatCrNum_3				:3;		/**< 需添加注释*/
	DWORD m_dwLnbNum_3					:3;		/**< 需添加注释*/
	DWORD m_dwReserved					:11;	/**< 保留使用*/
}CSTUNAntennaPar_S;

/**@brief tuner模块统一的信号频率、符号率偏移数据结构*/
typedef struct _CSTUNTransponderOffset_S
{
	DWORD m_dwOffsetFrequency_23		:23;	/**< 频率的偏移值，单位KHZ*/
	DWORD m_dwOffsetSymbolRate_8		:8;		/**< 符号率的偏移值，单位KHZ*/
	DWORD m_dwIQ_1						:1;		/**< 0-Normal; 1-Invers*/
}CSTUNTransponderOffset_S;


/**@brief 卫星电视tuner的带disqec信息的信号数据结构(同时满足DVB-S/S2)*/
typedef struct _CSTUNSRCSatelliteDeliver_S
{
	CSTUNSatelliteDeliver_S m_SatelliteDeliver;		/**< 卫星机频点信息结构体*/
	CSTUNLNB_S				m_LNB;					/**< 高频头信息结构体*/
	CSTUNDiSEqC1020_S		m_DiSEqC1020;           /**< diseq1.0信息结构体*/
	CSTUNDiSEqC1121_S		m_DiSEqC1121;           /**< diseq1.1信息结构体*/
	CSTUNDiSEqC1222_S		m_DiSEqC1222;           /**< diseq1.2信息结构体*/
	CSTUNSatellitePar_S		m_SatellitePar;			/**< 卫星参数信息结构体*/
	CSTUNAntennaPar_S		m_AntennaPar;           /**< 天线参数信息结构体*/
	CSTUNTransponderOffset_S	m_TransponderOffset;/**< 频点偏移值信息结构体*/
}CSTUNSRCSatelliteDeliver_S;

/**@brief tuner模块统一信号参数定义*/
typedef union _CSTUNSRCDeliver_U
{
	CSTUNCableDeliver_S		    m_CableDeliver;             /**< 有线机频点信息结构体*/
	CSTUNSRCSatelliteDeliver_S	m_SourceSatelliteDeliver;	/**< 见上面定义*/
	CSTUNTerrestrialDeliver_S	m_TerrestrialDeliver;       /**< 地面机频点信息结构体*/
}CSTUNSRCDeliver_U;



/**@brief tuner回调函数类型定义，有异议*/
typedef enum
{
	CTUN_CALLBACK_QPSK=2,		/**<  回调中的信号参数是QPSK调制方式的*/
	CTUN_CALLBACK_COFDM,	    /**< 回调中的信号参数是COFDM调制方式的*/
	CTUN_CALLBACK_QAM		    /**<  回调中的信号参数是QAM调制方式的*/
}CSTUNCallbackType_E;

/**@brief tuner回调消息类型定义*/
typedef enum
{
      TUN_SIGNAL_CONNECTED,          	/**< 频点锁定成功*/
      TUN_SIGNAL_DROPPED,             	/**< 取消锁频时发送此消息*/
      TUN_SIGNAL_LOST,                	/**< 由锁定转为失锁时发送此消息*/
      TUN_SIGNAL_SEARCHING,           	/**< 正在锁定频点，如果锁定失败会一直发送该消息*/
      TUN_SIGNAL_NOSIGNAL				/**< 无信号,模块会在确定锁定失败时发送一次该消息*/
}CSTUNCallbackEvents_E;

/**@brief tuner回调消息数据结构定义*/
typedef struct _CSTUNCallbackMessage_S
{
 CSTUNCallbackEvents_E m_SourceCallbackEvent;   /*见前面定义*/
 DWORD dwFrequency;                             /**< 回调消息对应的频点，单位KHZ*/
 DWORD dwSymbolRate;                            /**< 回调消息对应的符号率，单位Kbps*/
 DWORD dwBandWidth;                             /**< 回调消息对应的波段，实际对应HDI中
 										的T_BANDWIDTH枚举类型，值含义：0----8M, 1----7M, 2----6M。*/
 DWORD dwQAMMode;                               /**< 回调消息对应的调制方式，实际对应HDI中的T_QAMMode枚举类型，值含义：
 										0----Not Define, 1----16QAM, 2----32QAM, 3----64QAM, 4----128QAM, 5----256QAM。*/
 DWORD dwTunerIndex;                            /**< 回调消息对应的Tuner索引号，实际对应HDI中的CSHDITunerIndex枚举类型，
 										值含义：0----tuner 1, 1----tuner 2, 2----最大tuner数。*/
 void *pvUserData;                             /**< 用户在注册回调函数时传入的指针*/
}CSTUNCallbackMessage_S;

/**@brief tuner事件回调函数原型定义*/
typedef  void ( *CSTUNCallback  )( CSTUNCallbackType_E nSourceCallbackType, CSTUNCallbackMessage_S* pSourceCallbackMessage);

/**@brief tuner回调消息数据结构定义*/
typedef struct _CSTUNSignalInfo_S
{
	int 		nQuality;	/**< 信号质量（0～100）*/
	int		nStrength;	    /**< 信号强度（0 ～100）*/
	int		nErrorRate;	    /**< 错误率，1*107　个码元中错误码元的个数．(除以107就得到误码率）*/
	int		nCurrentFreq;	/**< 用来保存获取的信号频率*/
	int		nReserved[3];	/**< 保留参数*/
}CSTUNSignalInfo_S;

/**
@brief 根据指定的频点进行锁频操作.

@param[in] hHandle 用于锁频的Tuner设备句柄，由资源关联文档中定义的CSRMAlloc（）或 CSRMConnect()函数返回。具体参考资源关联接口设计文档。
@param[in] pDeliver 用户存放频点信息的数据块的地址
@return 如果成功返回TRUE,如果失败返回FALSE
*/
BOOL CSTunerConnectSource(HCSHANDLE hHandle, const CSTUNSRCDeliver_U* pDeliver);

 /**
@brief  注册指定的回调函数

@param[in] hHandle 用于锁频的Tuner设备句柄，由资源关联文档中定义的CSRMAlloc（）或 CSRMConnect()函数返回。具体参考资源关联接口设计文档。
@param[in] callback 回调函数句柄
@param[in] pData 用户传入的自定义指针
@return 如果成功返回TRUE,如果失败返回FALSE
@note
- 回调函数的最大注册个数为20个，超过20个将返回FALSE
- 注册时的以tuner号与callback为键值。
*/
BOOL CSTunerAddCallback( HCSHANDLE hHandle, CSTUNCallback  callback, void * pData );

 /**
@brief 删除指定的回调函数的注册。

@param[in] hHandle 用于锁频的Tuner设备句柄，由资源关联文档中定义的CSRMAlloc（）或 CSRMConnect()函数返回。具体参考资源关联接口设计文档。
@param[in] callback 回调函数句柄
@return 如果该回调函数根本就没有注册，将返回FALSE
*/
BOOL CSTunerDelCallback( HCSHANDLE hHandle, CSTUNCallback  callback );

 /**
@brief 取得当前正在锁定的信号信息

@param[in] hHandle 用于锁频的Tuner设备句柄，由资源关联文档中定义的CSRMAlloc（）或 CSRMConnect()函数返回。具体参考资源关联接口设计文档。
@param[out] pSignalInfo 用来保存获取的信号信息
@return 如果信号锁定，应该返回TRUE,否则FALSE
*/
BOOL CSTunerGetSignalInfo( HCSHANDLE hHandle, CSTUNSignalInfo_S *pSignalInfo);

/**
@brief 获取一组PlpID,PlpID为0~255的整数

@param[in] hHandle 用于锁频的Tuner设备句柄，由资源关联文档中定义的CSRMAlloc()或 CSRMConnect()函数返回。具体参考资源关联接口设计文档。
@param[out] pbPlpIds 用于返PLP ID的数组，数组大小应该至少256，否则会由于可用的PLP ID数量超过数组大小而返回错误
@param[in] nPlpIdsCount pbPlpIds指向的数组大小
@param[out] pnRetIdCount 用于返回实际保存到pbPlpIds中的PlpID个数
@return 
- 如果成功返回TRUE。
- 如果失败返回FALSE
@note 本接口为DVB-T2专用。
*/
BOOL CSTunerT2GetPlpInfo(HCSHANDLE hHandle, PBYTE pbPlpIds, int nPlpIdsCount, int* pnRetIdCount);

#ifdef __cplusplus
}
#endif

/** @} */

#endif 
