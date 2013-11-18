/**@defgroup CSPANEL panel
@brief 面板显示、按键相关接口

@version 0.1 2008/10/23 初始版本
@{
*/

#ifndef _CS_PANEL_H_
#define _CS_PANEL_H_

#include "CSHDI_typedef.h"
#include "udi_input.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
**                          Data structures		                                      
**********************************************************************/
/**@brief 面板类型定义*/
typedef enum _CSPanelCntTye_t
{
	CSPANEL_CNT_164_LED,      	/**< 有数码管，用164控制*/
	CSPANEL_CNT_164_LOCK,    	/**< 没有数码管，有LED，用164控制*/
	CSPANEL_CNT_164_NO_LED,		/**< 没有数码管，用164控制*/
	CSPANEL_CNT_GPIO_NO_LED,    /**< 使用GPIO，在新开发项目中建议使用选项CSPANEL_CNT_GPIO_MATRIX*/
	CSPANEL_CNT_MCU,			/**< 使用单片机，真待机*/
	CSPANEL_CNT_GPIO_MATRIX,	/**< 由行列gpio组成按键扫描矩阵*/
	CSPANEL_CNT_SPI,            /**< 使用pt6964,具有锁存功能,采用SPI协议>*/
	CSPANEL_CNT_ALL_GPIO,       /**< 使用GPIO，没有164，有led，led由gpio控制>*/
	CSPANEL_MAX_TYPE
}CSPanelCntTye_t;

/**@brief 板按LED和按键扫描的IO口配置*/
typedef struct _CSPanelPinCfg_t
{
	BYTE m_pucComPinGroup[6]; 	/**< 普通面板: 设置面板位选线的IO口资源，参考硬件设计*/	
	                            /**<CSPANEL_CNT_SPI:                              >*/
	                            /**0: m_nGPIOSTB ; 1:(上、下、左、右、menu、ok)中断; 2:power中断;3:power 中断触发方式标志位，若为 GPIO_LOW_TO_HIGH模式则设为0xfe >*/ 
								/**<CSPANEL_CNT_MCU:*/
								/**0:若待机后要求显示时间则赋初值'y'或者'Y'*/
	                            /**其余待定 >*/
								/**重点关注: 不使用的pin配置,请赋初值为0xff >*/
								
	BYTE m_pucSegPinGroup[2];	/**< 普通面板: 设置面板74HC164芯片的Data和Clock对应的IO口，m_pnSegPinGroup[0]对应Data线，m_pnSegPinGroup[1]对应Clock线*/
	                            /**<CSPANEL_CNT_SPI: 0:m_nGPIOData; 1:m_nGPIOCLK >*/  
								/**重点关注: 不使用的pin配置,请赋初值为0xff >*/

	BYTE m_ucKeyScanPin; 		/**< 普通面板: 1.假待机面板此参数为按键扫描对应的IO口.2.真待机面板此参数为主芯片与单片机通信使用的串口索引*/
	                            /**<CSPANEL_CNT_SPI: 留用 >*/
}CSPanelPinCfg_t;

typedef struct _CSLedPinCfg_t
{	
	/**
	- m_LedSegPinGroup[0]---- A
	- m_LedSegPinGroup[1]---- B
	- m_LedSegPinGroup[2]---- C
	- m_LedSegPinGroup[3]---- D
	- m_LedSegPinGroup[4]---- E
	- m_LedSegPinGroup[5]---- F
	- m_LedSegPinGroup[6]---- G
	- m_LedSegPinGroup[7]---- DP
	- m_LedComPinGroup[0]---- LED1
	- m_LedComPinGroup[1]---- LED2
	- m_LedComPinGroup[2]---- LED_COL
	- m_LedComPinGroup[3]---- LED3
	- m_LedComPinGroup[4]---- LED4
	- m_ucLedStandbyPin  ---- STANDBY
	- 重点关注: 不使用的pin配置,请赋初值为0xff
	*/
	BYTE m_LedSegPinGroup[8];
	BYTE m_LedComPinGroup[7];
	BYTE m_ucLedStandbyPin;
}CSLedPinCfg_t;

/**@brief 检测面板按键的GPIO配置，对应于选项CSPANEL_CNT_GPIO_NO_LED*/
typedef struct _CSPanelKeyCfg_t
{
	/**
	- pKeyPinGroup[0]---- CH-UP按键
	- pKeyPinGroup[1]---- CH-DOWN按键
	- pKeyPinGroup[2]---- POWER按键
	- pKeyPinGroup[3]---- ENTER按键
	- pKeyPinGroup[4]---- CH-LEFT按键
	- pKeyPinGroup[5]---- CH-RIGHT按键
	- pKeyPinGroup[6]---- MUTE按键
	- pKeyPinGroup[7]---- 上、下、左、右、ok、menu按键的中断口
	- 重点关注: 不使用的pin配置,请赋初值为0xff
	*/
	BYTE m_aucKeyPinGroup[8];
	DWORD m_dwKeyNum;
}CSPanelKeyPinCfg_t;


/**@brief 对应于选项CSPANEL_CNT_GPIO_MATRIX*/
typedef struct _CSPanelKeyMatrixCfg_S
{
	BYTE  *m_pucRowGpioIndex;	/**< 指向行GPIO的索引,从行0开始*/
	BYTE  *m_pucColGpioIndex;	/**< 指向列GPIO的索引,从列0开始*/
	DWORD m_dwRowNum;	/**< 矩阵的行数*/
	DWORD m_dwColNum;	/**< 矩阵的列数*/
}CSPanelKeyMatrixCfg_S;


/**@brief 静态面板配置参数*/
typedef struct _CSStaticPanelPinCfg_S    
{
	BYTE m_ucPCLK;       		 /**< 74595 SH CLK引脚*/
	BYTE m_ucPSTB;       		 /**< 74595 OE和ST CLK引脚 （通常这两个引脚连在一起）*/
	BYTE m_ucPDATA;      		 /**< 74595 串行输入引脚*/
	BYTE m_ucSENSE0;     		 /**< 按键地端引脚*/
	BYTE m_ucSENSE1;     		 /**< 按键地端引脚*/
	BYTE m_ucSENSE2;     		 /**< 按键地端引脚*/
	BYTE m_ucSCAN0;      		 /**< 检查按键引脚，按键未按下时，处于上拉状态*/
	BYTE m_ucSCAN1;      		 /**< 检查按键引脚，按键未按下时，处于上拉状态*/	
}CSStaticPanelCfg_S;

/**@brief 面板显示的时间参数*/
typedef struct _CSPanelTimeCfg_t
{
	BYTE m_ucHour;   		 /**< 设置面板时间显示的小时*/
	BYTE m_ucMinute; 		 /**< 设置面板时间显示的分钟*/
}CSPanelTimeCfg_t;

/**@brief 面板上LED显示状态定义*/
typedef enum _CSPanelLEDStatus_t
{
	CSPANEL_LED_OFF,			/**<  LED灯灭*/
	CSPANEL_LED_ON,			/**<  LED灯亮	*/
	CSPANEL_LED_FL			/**<  LED灯闪烁*/
}CSPanelLEDStatus_t;

/**@brief 面板上发送按键时的按键状态定义*/
typedef enum _CSPanelKeyStatus_t
{
	CSPANEL_KEY_PRESS,		/**<  表示按键按下*/
	CSPANEL_KEY_REPEAT,		/**<  表示按键重复*/
	CSPANEL_KEY_RELEASE		/**<  表示按键弹起*/
}CSPanelKeyStatus_t;

typedef struct _CSPanelKeyParam_t
{
	CSPanelKeyStatus_t	m_enmPanelKeyStatus;  	/**<  按键状态，表示按键按下或弹起*/
	BYTE			m_ucPanelKeyCode;			/**<  按键的键值*/
}CSPanelKeyParam_t;

typedef struct _CSPanelDecode_S
{
	int m_nKeyValue;	/**< 转换之后的按键键值*/
	int m_nLogicValue;	/**< 原始按键值*/
	BOOL m_bKeyRepeat;	/**< 是否支持repeat*/
}CSPanelDecode_S;

typedef enum _CSPanelLEDType_t
{
	CSPANEL_LED_MAIL,     	/**< 控制面板Mail灯*/
	CSPANEL_LED_SIGNAL,  	/**<  控制面板 Signal灯*/
	CSPANEL_LED_POWER,  	/**< 控制面板Power灯*/
	CSPANEL_LED_AUDIO, 	    /**< 控制面板Audio灯*/
	CSPANEL_LED_CMRS,   	/**< Cable Modem 收发状态灯*/
	CSPANEL_LED_CMONLINE,   /**< Cable Modem 上线指示灯*/
	CSPANEL_LED_CMUPLINK,	/**< Cable Modem 上行状态指示灯*/
	CSPANEL_LED_CMDOWNLINK,  /**< Cable Modem 下行状态指示灯*/
	CSPANEL_LED_CLOCK    	/**< 控制面板时间点灯，即数字中间的":" */  
}CSPanelLEDType_t;

/**@deprecated CSPANEL_LED_EXT0已被CSPANEL_LED_CMRS取代，保留在此仅为保持兼容，新的版本将会彻底移除 */
#define CSPANEL_LED_EXT0 CSPANEL_LED_CMRS

/**@deprecated CSPANEL_LED_EXT1已被CSPANEL_LED_CMONLINE取代，保留在此仅为保持兼容，新的版本将会彻底移除 */
#define CSPANEL_LED_EXT1 CSPANEL_LED_CMONLINE

/**@deprecated CSPANEL_LED_EXT2已被CSPANEL_LED_CMUPLINK取代，保留在此仅为保持兼容，新的版本将会彻底移除 */
#define CSPANEL_LED_EXT2 CSPANEL_LED_CMUPLINK

/**@deprecated CSPANEL_LED_EXT3已被CSPANEL_LED_CMDOWNLINK取代，保留在此仅为保持兼容，新的版本将会彻底移除 */
#define CSPANEL_LED_EXT3 CSPANEL_LED_CMDOWNLINK

typedef enum _CSPanelBootType_E
{
	CSPANEL_BOOT_BY_KEY,   		/**< 按下遥控器或前面板按键启动*/
	CSPANEL_BOOT_BY_TIME,      		/**< 定时启动*/
	CSPANEL_BOOT_BY_POWER_ON,		/**<机顶盒上电启动*/
	CSPANEL_BOOT_TYPE_NOT_DEF  	/**< 未定义的启动*/
}CSPanelBootType_E;

typedef struct _CSPanelIR_S
{
	DWORD	 	m_dwIRProtocol;  /**< 遥控器协议,使用CSIRProtocol_t枚举类型*/
	DWORD		m_dwUserID;		/**< 用户码*/
	DWORD		m_dwScanKeyCode;	/**< 扫描码*/
	DWORD		m_dwLogicKeyCode;	/**< reserved*/
}CSPanelIRSpecialKey_S;

/**
@brief 面板显示Hook
@param[in] penmLEDType    显示的面板灯枚举
@param[in] penmLEDStatus  显示的面板灯状态
@param[in] pbSkipDefault 是否忽略默认操作，TRUE表示只执行Hook函数定义的操作，FALSE表示同时执行CSPanelSetDisplayLED默认操作
@return CSUDI_SUCCESS 表示成功，失败则返回错误代码值
*/
typedef CSHDI_Error_t (*CSPanelDisplayLedHookFunc_F)(CSPanelLEDType_t* eLedType, CSPanelLEDStatus_t* eLedStatus, BOOL *pbSkipDefault);

/**
@brief 设置前面板的种类

具体请参照CSPanelCntTye_t的定义，在初始化之前调用
@param[in] enmPanelCntType 面板种类，参见CSPanelCntTye_t
@return CSHDI_SUCCESS表示设置面板种类成功，CSHDI_FAILURE表示设置面板种类失败
@note 不调用该函数，默认值为CSPANEL_CNT_164_LED
*/
CSHDI_Error_t CSPanelSetType(CSPanelCntTye_t enmPanelCntType);

/**
@brief 设置面板的中断优先级

@param[in] nIntLevel 面板中断优先级，取值范围为0-255
@return CSHDI_SUCCESS表示面板中断级别设置成功，CSHDI_FAILURE表示面板面板中断级别设置失败
*/
CSHDI_Error_t CSPanelSetInterruptLevel( int nIntLevel );

/**
@brief 全gpio面板初始化，此种面板的按键和数码管全部为gpio直连方式

@param[in] nIntLevel 面板中断优先级，取值范围为0-255
@param[in] pKeyPinGroup 检测面板按键的GPIO配置
@param[in] pLedPinGroup led数码管的管脚配置
@return CSHDI_SUCCESS表示初始化成功，CSHDI_FAILURE表示面板初始化失败。
*/
CSHDI_Error_t CSPanelAllGPIOInitialize(int nPriority, CSPanelKeyPinCfg_t *psKeyPinGroup, CSLedPinCfg_t * psLedPinGroup);

/**
@brief 面板初始化

配置面板任务（LSI平台）或中断（ST平台）优先级和IO口资源
@param[in] nPriority 面板任务（LSI平台）或中断（ST平台）优先级，取值范围为0-255
@param[in] pPinGroup 面板IO口资源分配
@return CSHDI_SUCCESS表示面板初始化成功，CSHDI_FAILURE表示面板初始化失败
@note 任务优先级为0-255，值越大优先级越高。推荐优先级为200-230。如果优先级太低可能会出现闪烁的问题
*/
CSHDI_Error_t CSPanelInitialize( int nPriority, CSPanelPinCfg_t *pPinGroup );

/**
@brief 面板初始化，对应于选项CSPANEL_CNT_GPIO_NO_LED的初始化函数

适用于用GPIO检测按键，没有164，没有数码管的面板
@param[in] nPriority 面板任务（LSI平台）或中断（ST平台）优先级，取值范围为0-255
@param[in] pKeyPinGroup 检测面板按键的GPIO配置
@return CSHDI_SUCCESS表示面板初始化成功，CSHDI_FAILURE表示面板初始化失败
@note 任务优先级为0-255，值越大优先级越高。推荐优先级为200-230。如果优先级太低可能会出现闪烁的问题
*/
CSHDI_Error_t CSPanelInitialize_GPIO( int nPriority, CSPanelKeyPinCfg_t *pKeyPinGroup );


/**
@brief 面板初始化，对应于选项CSPANEL_CNT_GPIO_MATRIX的初始化函数

适用于用GPIO检测按键，没有164，没有数码管的面板
@param[in] nPriority 面板任务（LSI平台）或中断（ST平台）优先级，取值范围为0-255
@param[in] pKeyPinGroup 检测面板按键的GPIO配置
@return CSHDI_SUCCESS表示面板初始化成功，CSHDI_FAILURE表示面板初始化失败
@note 任务优先级为0-255，值越大优先级越高。推荐优先级为200-230。如果优先级太低可能会出现闪烁的问题
*/
CSHDI_Error_t CSPanelInitializeGpioMatrix( int nPriority, CSPanelKeyMatrixCfg_S *pKeyPinGroup );


/**
@brief 面板初始化

面板初始化，可设置任务的栈大小，适合于不使用GPIO配置的面板
@param[in] nPriority 面板任务（LSI平台）或中断（ST平台）优先级，取值范围为0-255
@param[in] nStackSize 面板任务的栈大小
@param[in] pPinGroup 面板IO口资源分配
@return CSHDI_SUCCESS表示面板初始化成功，CSHDI_FAILURE表示面板初始化失败
@note 任务优先级为0-255，值越大优先级越高。推荐优先级为200-230。如果优先级太低可能会出现闪烁的问题
*/
CSHDI_Error_t CSPanelInitialize_Ext( int nPriority,int nStackSize,CSPanelPinCfg_t *pPinGroup );

/**
@brief 面板初始化

可设置任务的栈大小，适合于使用GPIO检测按键，不适用164的面板
@param[in] nPriority 面板任务（LSI平台）或中断（ST平台）优先级，取值范围为0-255
@param[in] nStackSize 面板任务的栈大小
@param[in] pKeyPinGroup 面板按键IO口资源分配
@return CSHDI_SUCCESS表示面板初始化成功，CSHDI_FAILURE表示面板初始化失败
@note 任务优先级为0-255，值越大优先级越高。推荐优先级为200-230。如果优先级太低可能会出现闪烁的问题
*/
CSHDI_Error_t CSPanelInitialize_Ext_GPIO( int nPriority,int nStackSize,CSPanelKeyPinCfg_t *pKeyPinGroup);

/**
@brief 面板初始化

可设置任务的栈大小，适用于静态面板
@param[in] nPriority 面板任务（LSI平台）或中断（ST平台）优先级，取值范围为0-255
@param[in] psPinGroup 面板按键IO口资源分配
@return CSHDI_SUCCESS表示面板初始化成功，CSHDI_FAILURE表示面板初始化失败
@note 任务优先级为0-255，值越大优先级越高。推荐优先级为200-230。如果优先级太低可能会出现闪烁的问题
*/
CSHDI_Error_t CSStaticPanelInitialize( int nPriority, CSStaticPanelCfg_S *psPinGroup);

/**
@brief 面板初始化

使用pt6964芯片，具有锁存功能，锁存按键值
@param[in] nPriority 面板任务（LSI平台）或中断（ST平台）优先级，取值范围为0-255
@param[in] psPinGroup 面板按键IO口资源分配
@return CSHDI_SUCCESS表示面板初始化成功，CSHDI_FAILURE表示面板初始化失败
*/
CSHDI_Error_t CSPanelSPIInit( int nPriority, CSPanelPinCfg_t *pPinGroup );

/**
@brief 注册面板按键回调函数

面板驱动检测到按键按下或弹起时通过注册的回调函数通知应用层
@param[in] function 回调函数，用户通过回调函数得到按键来源、状态和键值
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelRegisterKeySendCallback( CSHDICallback function );

/**
@brief 数码管显示，可以控制四个七段管和两个圆点的显示

@param[in] pucSegVal
- pucSegVal[0]：左起第一个七段管的段选
- pucSegVal[1]：左起第二个七段管的段选
- pucSegVal[2]：圆点的段选
- pucSegVal[3]：左起第三个七段管的段选
- pucSegVal[4]：左起第四个七段管的段选
@param[in] ucSize 要显示的七段管个数，nSize最大为5，若nSize小于5，其余的数码将无显示
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelDisplaySegment( BYTE *pucSegVal, BYTE ucSize );

/**
@brief 设置面板按键是否可重复

@param[in] ucSingleKeyValue 要设置的按键的键值
@param[in] bISRepeat 
- FALSE：按键不可重复，只在按键按下和弹起时产生回调，该状态是按键的默认状态
- TRUE：按键可重复，在按键按下不放时，连续产生按键回调
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelSetSingleKeyRepeat( BYTE ucSingleKeyValue,BOOL bISRepeat );

/**
@brief 面板扫描

用于无操作系统平台的数码管显示和按键扫描。
用户需要在调用该函数前设置要显示的内容，并在调用该函数时传递扫描次数和每次扫描的延时时间。
@param[in] dwScanTime 面板扫描次数，用于控制数码管显示和按键扫描的持续时间
@param[in] dwDelayTime 用于控制每次扫描的延时时间，单位是1个时钟周期
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelScan (DWORD dwScanTime, DWORD dwDelayTime);

/**
@brief 用于无操作系统的单个数码管显示

@param[in] ucSegValue 数码管的段选
@param[in] ucComIndex 数码管位选，0－左起第一个七段管，1－左起第二个七段管，2－圆点，3－左起第三个七段管，4－左起第四个七段管
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelSetSingleSegment( BYTE ucSegValue, BYTE ucComIndex );

/**
@brief 用于无操作系统的数码管显示相同内容

@param[in] ucSegValue 数码管的段选
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelSetAllSegment( BYTE ucSegValue );

/**
@brief 设置冒号的闪烁频率
@param[in] nTimeDiv 设置的值越小，闪烁越快，值越大，闪烁越慢
@return CSHDI_SUCCESS表示显示成功，CSHDI_FAILURE表示显示失败。
*/
CSHDI_Error_t	CSPanelConfigDisplayTimeDiv(int nTimeDiv);

/**
@brief 设置LED灯的闪烁频率

@param[in] nLEDDiv 设置的值越小，闪烁越快，值越大，闪烁越慢
@return CSHDI_SUCCESS表示显示成功，CSHDI_FAILURE表示显示失败。
*/
CSHDI_Error_t	CSPanelConfigLEDFlashDiv(int nLEDDiv);

/**
@brief 获取面板库的版本号

@param[out] pchVer 用来存放版本信息
@param[in] nSize pchVer的长度
@return 版本信息的实际长度
@note This function is used to get version information.Display format is:\n
"[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
*/
int CSPANELGetVersion( PCHAR pchVer, int nSize );

/**
@brief 显示时间

@param[in] pCSTime 时间
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelDisplayTime( CSPanelTimeCfg_t *pCSTime );

/**
@brief 显示信号强度

@param[in] ucStrength 表示信号强度，取值范围是0-10
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelDisplaySignalStrength( BYTE ucStrength );

/**
@brief 控制面板LED灯显示

@param[in] enmLEDType 选择要显示的LED灯
@param[in] enmLEDStatus LED灯的显示状态
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelDisplayLED( CSPanelLEDType_t enmLEDType, CSPanelLEDStatus_t enmLEDStatus );

/**
@brief 清除所有前面板7段管显示

调用后7段管熄灭，但不影响Mail,Siganl,Power,Aduio等
*/
void CSPanelClear( void );

/**
@brief 加入前面板按键的实际值和逻辑值之间的关系

@param[in] psKeyList 加入实际值和逻辑值对应关系的指针。详见CSPanelDecode_S定义。
@param[in] nKeyNumber 前面板按键个数，该参数标明psKeyList数组的长度。
@return CSHDI_SUCCESS表示显示成功，CSHDI_FAILURE表示显示失败。
*/
CSHDI_Error_t  CSPanelAddEquipment( const CSPanelDecode_S *psKeyList, int nKeyNumber);
	
/**
@brief 数码管字符显示。

对有操作系统的平台，数码管显示在调用该函数后自动更新；
无操作系统的平台需要在调用该函数后调用CSPanelScan（）来更新数码管显示。
@param[in] pcText 要在数码管显示的字符串，必须是数码管可显示的字符，否则数码管将无显示。最多可显示四个字符。
@param[in] ucSize 要显示的字符数，最大为4，若nSize小于4，字符左对齐显示
@return CSHDI_SUCCESS表示成功，CSHDI_FAILURE表示失败
*/
CSHDI_Error_t CSPanelDisplayText( char *pcText, BYTE ucSize );


/**
@brief 设置机顶盒待机时间，当设置的时间到达，机顶盒自动启动

@param[in] psStandTime 待机时间，目前只支持小时，分钟。如设置3小时，20分钟，则关机3小时20分钟后，机顶盒启动。
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSPanelSetStandbyTime (const CSPanelTimeCfg_t *psStandTime);


/**
@brief 机顶盒待机，调用该函数以后，单片机立即切断主板电源。
@param[in] 待机后可响应的遥控器按键列表及遥控器数目
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSPanelStandby(const CSPanelIRSpecialKey_S *psIRSpecialKeyList,BYTE ucSize);



/**
@brief设置单片机管脚电平，用于通过单片机控制的面板

@param[in] ucPinIndex 要设置电平的单片机管脚的索引,51单片机的管脚均能表示为Pm.n的形式,脚Pm.n对应的索引号为m*8+n,如P1.6管脚的索引为1*8+6=14
@param[in] ucLevel 要设置成的电平值,0或1
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSPanelSetPinLevel(BYTE ucPinIndex,  BYTE ucLevel);


/**
@brief获取启动方式类型

@param[out] peBootType 启动方式存放地址
@return CSHDI_SUCCESS表示获取成功，CSHDI_FAILURE表示获取失败。
*/
CSHDI_Error_t CSPanelGetBootType(CSPanelBootType_E  *peBootType);

/**
@brief 面板注册Hook函数
@param[in] fnHook   要注册的Hook函数 
@return CSUDI_SUCCESS 表示注册成功，失败则注册失败
*/
CSHDI_Error_t CSPanelSetDisplayLedHook(CSPanelDisplayLedHookFunc_F fnHook);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
