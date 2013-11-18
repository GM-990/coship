/**@defgroup CSVFD[CSVFD]同洲项目专用VFD驱动接口
@brief 本模块主要定义了同洲项目专用平台无关VFD驱动接口
@brief 真空荧光显示屏VFD(Vacuum Fluorescent Display)。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_VFD_H_
#define _CS_VFD_H_


#include "cs_typedef.h"
#include "cs_assert.h"
#include "CSHDI_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief VFD显示动作定义*/
typedef enum
{
	VFD_LED_PLAY, 		/**< 播放 */ 
	VFD_LED_PAUSE, 	/**<暂停 */ 
	VFD_LED_STOP, 		/**< 停止 */ 
	VFD_LED_HDTV,		/**< 播放 */ 
	VFD_LED_BACKWARD, /**< HDTV */ 
	VFD_LED_FORWARD, 	/**< 快进 */ 
	VFD_LED_BALL, 		/**< 圆*/ 
	VFD_LED_DUBY, 		/**< 杜比 */ 
	VFD_LED_PREVIOUS, 	/**< 前一个*/ 
	VFD_LED_NEXT, 		/**< 后一个*/ 
	VFD_LED_IR, 		/**< 遥控器*/ 
	VFD_LED_USB, 		/**< USB*/ 
	VFD_LED_N,			/**< N*/ 
	VFD_LED_VOD, 		/**< VOD */
	VFD_LED_POWER, 	/**< 电源*/ 
	VFD_LED_TV, 		/**< 电视*/ 
	VFD_LED_NETWORK, 	/**< 网络*/ 
	VFD_LED_RADIO, 	/**< 广播*/ 
	VFD_LED_CLOCK,		/**< 时钟*/ 
	VFD_LED_MODEM,	/**< MODEM */ 
	VFD_LED_REC		/**< 录制 */ 
}T_CS_VFD_LEDS;
	

/**@brief VFD显示模式定义*/
typedef enum
{
	VFD_DIGIT_4_SEG_24,	/**< 需要添加说明 */
	VFD_DIGIT_5_SEG_23,	/**< 需要添加说明 */
	VFD_DIGIT_6_SEG_22,	/**< 需要添加说明 */
	VFD_DIGIT_7_SEG_21,	/**< 需要添加说明 */
	VFD_DIGIT_8_SEG_20,	/**< 需要添加说明 */
	VFD_DIGIT_9_SEG_19,	/**< 需要添加说明 */
	VFD_DIGIT_10_SEG_18,	/**< 需要添加说明 */
	VFD_DIGIT_11_SEG_17,	/**< 需要添加说明 */
	VFD_DIGIT_12_SEG_16	/**< 需要添加说明 */
}T_CS_VFD_DIS_MODE;

/**@brief VFD显示亮度定义*/
typedef enum
{
	VFD_WIDTH_1_16, 		/**< 最弱 */
	VFD_WIDTH_2_16, 		/**< 需要添加说明 */
	VFD_WIDTH_4_16, 		/**< 需要添加说明 */
	VFD_WIDTH_10_16, 		/**< 需要添加说明 */
	VFD_WIDTH_11_16, 		/**< 需要添加说明 */
	VFD_WIDTH_12_16, 		/**< 需要添加说明 */
	VFD_WIDTH_13_16, 		/**< 需要添加说明 */
	VFD_WIDTH_14_16 		/**< 最强 */
}T_CS_VFD_DIMMING; 

/**@brief VFD显示开关定义*/
typedef enum
{
	VFD_LED_OFF,		/**< VFD显示关闭*/
	VFD_LED_ON			/**< VFD显示打开*/
}T_CS_VFD_LED_STATUS;

/**@brief VFD显示开关定义*/
typedef enum
{
	VFD_COL_ON,		/**< 需要添加说明 */
	VFD_COL_FLS,		/**< 需要添加说明 */
	VFD_COL_OFF		/**< 需要添加说明 */
}T_CS_VFD_COL_STATUS;

/**@brief VFD 按键状态定义*/
typedef enum 
{
	CSVFDPANEL_KEY_PRESS, 		/**< 表示按键按下 */
	CSVFDPANEL_KEY_REPEAT, 		/**< 表示按键重复 */
	CSVFDPANEL_KEY_RELEASE 		/**< 表示按键弹起 */
}T_CS_VFDPanel_Key_Status; 		

/**@brief VFD 显示时钟的两点方法定义*/
typedef enum
{
	TWO_POINT_FIRST, /**< 左边的两点 */
	TWO_POINT_SECOND /**< 右边的两点 */
}T_CS_VFDPanel_Two_Points;

/**@brief VFD 设备启动方法定义*/
typedef enum
{
	BOOT_BY_POWER,   		/**< 按下遥控器或前面板的开机键启动*/
	BOOT_BY_TIME,      		/**< 定时启动*/
	BOOT_BY_RESTART,		/**< 断电重启导致的启动*/
	BOOT_TYPE_NOT_DEF	/**< 未定义的启动方式*/
}CS_VFD_BOOT_TYPE_E;

/**@brief VFD 按键回调参数定义*/
typedef struct 
{
	T_CS_VFDPanel_Key_Status enmVFDPanelKeyStatus; 	/**< 按键状态 */
	BYTE ucVFDPanelKeyCode;						/**< 按键键值 */
}T_CS_VFDPanel_KeyParam; 				

/**@brief VFD 按键初始化参数定义*/
typedef struct
{
	int nGpioSTB; 		/**< 使能控制脚 */
	int nGpioDIn; 		/**< 数据输入脚，机顶盒通过该引脚从PT6315 读数据*/
	int nGpioDout; 		/**< 数据输出脚，机顶盒通过该引脚向PT6315写数据 */
	int nGpioClk; 		/**< 时钟 */
	T_CS_VFD_DIS_MODE enmDisplayMode; 	/**< VFD的显示模式，一般为VFD_DIGIT_10_SEG_18 */
	T_CS_VFD_DIMMING enmDimming;			/**< 需要添加说明 */
	BOOL bKeyScan; 		/**< 表示是否需要支持键盘扫描,TRUE:支持,FALSE:不支持 */
}T_CS_VFDPanel_InitParam; 


/**@brief VFD 按键定义*/
typedef struct _CSVFDKeyDef_t
{
	BYTE	m_ucKeyValue;		/**< 需要添加说明 */
	BOOL	m_bRepeatEnable;	/**< 需要添加说明 */
}CSVFDKeyDef_t;


/**@brief VFD 显示时间定义*/
typedef struct _CSVFDTimeCfg_S
{
	WORD m_wYear;			/**< 需要添加说明 */
	BYTE m_ucMonth;		/**< 需要添加说明 */
	BYTE m_ucDay;			/**< 需要添加说明 */
	BYTE m_ucHour;			/**< 需要添加说明 */
	BYTE m_ucMinute;		/**< 需要添加说明 */
	BYTE m_ucSecond;		/**< 需要添加说明 */
}CSVFDTimeCfg_S;

/**@brief VFD 控制模式定义*/
typedef enum
{
	CS_VFD_GPIO,		/**< 需要添加说明 */
	CS_VFD_SCM			/**< 需要添加说明 */
}CSVFDMode;

typedef struct _CSVFDIRSpecialKey_S
{
	DWORD	 	m_dwIRProtocol;  /**< 遥控器协议,使用CSIRProtocol_t枚举类型*/
	DWORD		m_dwUserID;		/**< 用户码*/
	DWORD		m_dwScanKeyCode;	/**< 扫描码*/
	DWORD		m_dwLogicKeyCode;	/**< reserved*/
}CSVFDIRSpecialKey_S;

/**
@brief VFD初始化，配置VFD初始化时的IO配置信息和PT6315的配置信息。

@param[in] stVFDInitparam VFD IO口资源分配和PT6315配置信息，详见T_CS_VFDPanel_InitParam结构定义
@return CSHDI_SUCCESS表示VFD初始化成功，CSHDI_FAILURE表示VFD初始化失败
*/
CSHDI_Error_t CSVFDInit(T_CS_VFDPanel_InitParam stVFDInitparam);

/**
@brief VFD字符显示。对有操作系统的平台，数码管显示在调用该函数后自动更新；无操作系统的平台需要在调用该函数后调用CSVFDScan（）来更新VFD显示。

@param[in] pucData 要在VFD中显示的字符串，必须是VFD可显示的字符集内，否则VFD只显示在字符集范围内的字符，最多可显示八个字符。
@param[in] nLength 要显示的字符数，最大为8，若nLength小于8，字符左对齐显示。
@return CSHDI_SUCCESS表示显示成功，CSHDI_FAILURE表示显示失败
*/
CSHDI_Error_t CSVFDDisplayText(BYTE *pucData, int nLength);

/**
@brief 显示时间

@param[in] nhour 显示小时，最大值为99小时。若nhour小于0，则不显示小时。
@param[in] nminute 显示分钟，最大值为99分钟。若nminute小于0，则不显示分钟。
@param[in] nSecond 显示秒，最大值为99秒。若nSecond小于0，则不显示秒。
@return CSHDI_SUCCESS表示显示成功，CSHDI_FAILURE表示显示失败
*/
CSHDI_Error_t CSVFDDisplayTime(int nhour, int nminute, int nSecond);

/**
@brief 设置主芯片与单片机通信的串口索引

@param[in] nUartIndex 主芯片与单片机通信的串口号，默认值为0，该函数必须在初始化前
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSVFDSetUartIndex (int nUartIndex);

/**
@brief 设置机顶盒待机时间，当设置的时间到达，机顶盒自动启动

@param[in] psVFDTimeCfg 待机时间，目前只支持小时，分钟，秒。如设置3小时，20分钟，20秒，则3:20:20过去后，机顶盒启动。
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。当发送成功并且收到单片机应答以后，返回成功，否则返回失败。
*/
CSHDI_Error_t CSVFDSetStandbyTime (CSVFDTimeCfg_S *psVFDTimeCfg);

/**
@brief 机顶盒待机，调用该函数以后，单片机立即切断主板电源。

@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSVFDStandby(void);

/**
@brief获取启动方式类型

@param[out] peBootType 启动方式存放地址
@return CSHDI_SUCCESS表示获取成功，CSHDI_FAILURE表示获取失败。
*/
CSHDI_Error_t CSVFDGetBootType(CS_VFD_BOOT_TYPE_E *peBootType);

/**
@brief 控制VFD中LED灯的显示，详见T_CS_VFD_LEDS类型。

@param[in] enmLED  要显示的LED灯
@param[in] enmStatus LED灯的亮灭情况，详见T_CS_VFD_LED_STATUS
@return CSHDI_SUCCESS表示显示成功，CSHDI_FAILURE表示显示失败。
*/
CSHDI_Error_t CSVFDDisplayLED(T_CS_VFD_LEDS enmLED, T_CS_VFD_LED_STATUS enmStatus);

/**
@brief 控制VFD中两个冒号的亮灭情况和闪烁频率。

@param[in] enmPointType 表示冒号类型，前面冒号或者后面冒号。
@param[in] enmColStatus 表示冒号显示状态，亮、灭或闪烁。
@param[in] nFlsTimeDiv 表示冒号闪烁的频率，设置1表示30ms亮或灭一次。
@return CSHDI_SUCCESS表示显示成功，CSHDI_FAILURE表示显示失败。
*/
CSHDI_Error_t CSVFDConfigDisplayTimeDiv(T_CS_VFDPanel_Two_Points enmPointType, 
                                                                               T_CS_VFD_COL_STATUS enmColStatus, int nFlsTimeDiv);

/**
@brief 注册VFD面板按键回调函数。VFD驱动检测到按键按下或弹起时通过注册的回调函数通知应用层。

@param[in] pfunCallBack 回调函数，用户通过回调函数得到按键来源、状态和键值。
@return CSHDI_SUCCESS表示注册成功，CSHDI_FAILURE表示注册失败。
*/
CSHDI_Error_t CSVFDPanelRegisterKeySendCallback(CSHDICallback pfunCallBack);

/**
@brief 设置VFD面板按键是否可重复。

@param[in] ucSingleKeyValue 要设置的按键的键值
@param[in] bISRepeat FALSE：按键不可重复，只在按键按下和弹起时产生回调，该状态是按键的默认状态。TRUE：按键可重复，在按键按下不放时，连续产生按键回调
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSVFDSetSingleKeyRepeat( BYTE ucSingleKeyValue, BOOL bISRepeat );

/**
@brief 设置VFD初始化时创建任务的优先级和栈大小

@param[in] nPriority  VFD初始化创建任务的优先级
@param[in] nStackSize VFD初始化创建任务的栈大小
@return None
*/
void CSVFDSetTaskParam(int nPriority, int nStackSize);

/**
@brief 设置VFD的显示亮度

@param[in] eDimming VFD的亮度，详见T_CS_VFD_DIMMING定义
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSVFDSetDimming(T_CS_VFD_DIMMING eDimming);

/**
@brief VFD面板扫描，用于无操作系统平台的VFD显示和按键扫描。用户需要在调用该函数前设置要显示的内容，并在调用该函数时传递扫描次数和每次扫描的延时时间。

@param[in] dwScanTime 面板扫描次数，用于控制VFD显示和按键扫描的持续时间
@param[in] dwDelayTime 用于控制每次扫描的延时时间。
@return CSHDI_SUCCESS表示扫描成功，CSHDI_FAILURE表示扫描失败。
*/
CSHDI_Error_t CSVFDScan (DWORD dwScanTime,DWORD dwDelayTime);

/**
@brief获取版本信息。

@param[in] nSize pchVer的长度
@param[out] pcVer 用来存放版本信息
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
int CSVFDGetVersion(PCHAR pcVer, int nSize);

/**
@brief 选择VFD控制模式

@param[in] eVfdMode VFD控制模式
@return CSHDI_SUCCESS表示选择成功，CSHDI_FAILURE表示选择失败。
*/
CSHDI_Error_t CSVFDSeleteMode(CSVFDMode eVfdMode);


#ifdef __cplusplus
}
#endif

/** @} */

#endif






