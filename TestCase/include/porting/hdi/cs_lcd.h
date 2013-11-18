/**@defgroup CSLCD[CSLCD]同洲项目专用LCD驱动接口
@brief 本模块主要定义了同洲项目专用平台无关LCD驱动接口
@brief LCD的意思是液晶显示器- Liquid Crystal Display 的简称 。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.3 2008/11/5  第一次细化版本
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_LCD_H_
#define _CS_LCD_H_
#include "cs_typedef.h"
#include "cs_assert.h"
#include "CSHDI_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief LCD模块GPIO配置参数定义*/
typedef struct
{
	BYTE uclcd_db0;				/**< GPIO config of data0 in Data bus*/
	BYTE uclcd_db1;				/**< GPIO config of data1 in Data bus*/
	BYTE uclcd_db2;				/**< GPIO config of data2 in Data bus*/
	BYTE uclcd_db3;				/**< GPIO config of data3 in Data bus*/
	BYTE uclcd_db4;				/**< GPIO config of data4 in Data bus*/
	BYTE uclcd_db5;				/**< GPIO config of data5 in Data bus*/
	BYTE uclcd_db6;				/**< GPIO config of data6 in Data bus*/
	BYTE uclcd_db7;				/**< GPIO config of data7 in Data bus*/

	BYTE uclcd_r_w;				/**< GPIO of Read/Write*/
	BYTE uclcd_enalble;		/**< GPIO of LCD Enable*/
	BYTE uclcd_rs;				/**< GPIO of Registor Select*/
	BYTE ucpower_on_off;	/**< GPIO of LCD power*/
	
}CSLCD_PIO_CFG_T;


/**@brief LCD模块数据总线宽度定义*/
typedef enum
{
	MODE_8BIT,		/**< Data bus is 8 bits*/
	MODE_4BIT			/**< Data bus is 4 bits*/
}CSLCD_BUS_MODE_T;

/**@brief LCD模块驱动的芯片类型定义*/
typedef enum
{
	KS0070B,
	LCD_MAX_CHIP_TYPE
}CSLCD_CHIP_TYPE_T;


/**@brief LCD模块数据移动方向定义*/
typedef enum
{
	SHIFT_LEFT, 	/**< 左移*/
	SHIFT_RIGHT, 	/**< 右移*/
	SHIFT_UP, 		/**< 上移*/
	SHIFT_DOWN 		/**< 下移*/
}CSLCD_SHIFT_DIRECT_T;


/**@brief LCD模块的光标状态定义*/
typedef enum
{
	CURSOR_OFF,					/**< cursor is not displayed*/
	CURSOR_ON_NOBLINK,	/**< cursor is displayed and without blink*/
	CURSOR_ON_BLINK			/**< cursor is displayed and blink*/
	
}CSLCD_CURSOR_STATUS_T;


/**@brief LCD模块的自定义字符类型*/
typedef enum
{
	CHAR_RECEIVED,			/**< 接收字符*/
	ARROW_UP,						/**< 向上的箭头*/
	ARROW_DOWN,					/**< 向下的箭头*/
	ARROW_LEFT,					/**< 向左的箭头*/
	ARROW_RIGHT					/**< 向右的箭头*/
}CSLCD_CUSTOM_CHARACTER_T;


/**@brief 模拟LCD 的视频标准*/
typedef enum
{
	EM_ALCD_NTSC,				/**< N制*/
	EM_ALCD_PAL,				/**< P制*/
	EM_ALCD_SECAM, 			/**< 需添加说明*/
	EM_ALCD_NTSC_4,			/**< 需添加说明*/
	EM_ALCD_PAL_M, 			/**< 需添加说明*/
	EM_ALCD_PAL_CN,  		/**< 自动检测*/
	EM_ALCD_END		 			/**<  上限 */
}CSALCDVideoSTD_E;


/**@brief LCD的显示模式16:9或4:3*/
typedef enum
{
	EM_ALCD_16_9,				/**< 16:9*/
	EM_ALCD_4_3					/**< 4:3*/
}CSALCDDispalyMode_E;

/**@brief LCD 的背光控制*/
typedef enum
{
	EM_ALCD_ON,				/**< LCD 背光开*/
	EM_ALCD_OFF 			/**< LCD背光关*/
}CSALCDSwitch_E;


/**@brief LCD 可调节参数*/
typedef enum
{
	EM_ALCD_BRIGHTNESS,			 	/**< 亮度 		 0-100*/
	EM_ALCD_CONTRAST,	       	/**< 对比度 	 0-100*/
	EM_ALCD_SHARPNESS,	    	/**< 锐力度 	 0-10*/
	EM_ALCD_SAT,             	/**< 饱和度		 0-100*/
	EM_ALCD_BACKGROUND,				/**< 背光调节  0-10*/
	EM_ALCD_DAC,              /**< DAC调节*/
	EM_ALCD_VCOMA, 	      		/**< VCOMA调节 0-32*/
	EM_ALCD_VCOMD             /**< VOMD调节  0-32*/
}CSALCDPARAM_E;


/**@brief LCD OSD显示位置定义*/
typedef enum
{
	EM_ALCD_TOP_RIGHT,			/**< 右上角*/
	EM_ALCD_MIDDLE,					/**< 中间*/
	EM_ALCD_POSITION_MAX		/**< 需添加说明*/
}CSALCD_OSD_POSITION_E;


/**@brief LCD 驱动BACKGROUP定义*/
typedef enum
{
	EM_ALCD_BLUE,  							/**< 显示蓝屏*/
	EM_ALCD_BLACK,   						/**< 显示黑屏*/  
	EM_ALCD_CLOSE_BACKGROUP,  	/**< 关背景*/
	EM_ALCD_MAX_BACKGROUP_TYPE 	/**<  上限 */
}CSALCD_BACKGROUP_E;


/**@brief 模拟LCD 驱动CVBS输入通道定义*/
typedef enum
{
	EM_ALCD_CVBS, 							/**< 选择CVBS输入通道*/
	EM_ALCD_YC, 								/**< 选择YC输入通道。*/
	EM_ALCD_CHANNEL_MAX_NUM 		/**<  上限 */
}CSALCD_SourceType_E;


/**@brief 模拟LCD的初始化参数*/
typedef struct
{
	DWORD							m_dwChipReset;	/**< dwChipReset为T118芯片的RESET引脚的GPIO索引。*/
	CSALCDVideoSTD_E 	m_eVideoStd;		/**< m_eVideoStd为输入信号制式，设置为EM_ALCD_AUTO表示自动检测*/
}CSALCDInitCFG_T;

/** 
@brief LCD驱动初始化。

@param[in] enmLCDType LCD芯片类型，目前只能是KS0070B
@param[in] enmBusMode LCD模块数据总线宽度
@param[in] LCDPIOConfig 用于控制的GPIO的配置信息，详见关于结构体CSLCD_PIO_CFG_T的描述。
@return CSHDI_SUCCESS表示初始化成功，CSHDI_FAILURE表示初始化失败。 
*/
CSHDI_Error_t CSLCDInit( CSLCD_CHIP_TYPE_T enmLCDType, CSLCD_BUS_MODE_T enmBusMode, CSLCD_PIO_CFG_T LCDPIOConfig);

/** 
@brief 控制电源开关，用于控制背景灯

@param[in] enmLCDPower CSHDI_OFF：关  CSHDI_ON：开
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSLCDPower( CSHDISwitch enmLCDPower);

/** 
@brief 清屏

@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSLCDDisplayClear(void);

/** 
@brief 从指定的起始位置显示字符串（不能跨行）

@param[in] nRowNo 起始位置的行号
@param[in] nColumnNo 起始位置列号
@param[in] pucStringToDisplay 存放要显示字符的缓存的首地址
@param[in] nLength 要显示字符的长度
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSLCDDisplay(int nRowNo, int nColumnNo, BYTE *pucStringToDisplay,int nLength);

/** 
@brief 移动光标

@param[in] enmDirect 光标移动方向,SHIFT_LEFT:左移,SHIFT_RIGHT:右移,SHIFT_UP:上移,SHIFT_DOWN:下移
@param[in] nTimes 光标移动次数
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSLCDCursorShift(CSLCD_SHIFT_DIRECT_T enmDirect, int nTimes) ;

/** 
@brief 移动屏幕所有显示字符和光标

@param[in] enmDirect 光标移动方向,HIFT_LEFT:左移,SHIFT_RIGHT:右移
@param[in] nTimes 移动次数
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSLCDDisplayShift(CSLCD_SHIFT_DIRECT_T enmDirect, int nTimes);

/** 
@brief 移动光标到屏幕左上角

@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSLCDCursorReturnHome(void);

/** 
@brief 设置光标显示方式

@param[in] enmStatus 显示方式,CURSOR_OFF:光标不显示,CURSOR_ON_NOBLINK:光标显示但不闪烁,CURSOR_ON_BLINK:光标显示并闪烁
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSLCDCursorStatusSet( CSLCD_CURSOR_STATUS_T enmStatus);

/** 
@brief 以当前光标所在位置为起始位置显示字符串

@param[in] pucStringToDisplay 存储要显示字符的缓存的首地址
@param[in] nLength 显示字符的长度
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSLCDDisplayCurrentPos( BYTE *pucStringToDisplay,int nLength);

/** 
@brief 指定光标的位置

@param[in] nRowNo 所在行
@param[in] nColumnNo 所在列
@return CSHDI_SUCCESS表示设置成功，CSHDI_FAILURE表示设置失败。
*/
CSHDI_Error_t CSLCDSetPos(int nRowNo,int nColumnNo);

/** 
@brief 指定光标的位置

@param[in] enmStatus CSHDI_OFF：关，CSHDI_ON：开
*/
void BackGroundLightCtr( CSHDISwitch enmStatus);

/** 
@brief 显示特定字符

@param[in] code 特定字符，如箭头向左等
@return 显示成功则返回CSHDI_SUCCESS，否则返回CSHDI_FAILURE
*/
CSHDI_Error_t CSLCDDisplayCustomCharacter( CSLCD_CUSTOM_CHARACTER_T code);

/** 
@brief 获取LCD驱动版本号

@param[in]   nSize  版本号字符长度
@param[out] pchVer 版本号地址
@return 获取到版本号的长度。
*/
int CSLCDGetVersion( PCHAR pchVer, int nSize );

/** 
@brief 对LCD控制芯片T118初始化，并设置屏的grmma值等参数。

@param[in] eI2CIndex LCD控制芯片T118所在的I2C索引。
@param[in] nPriority ALCD模块线程的优先级，该线程用于检测LCD是否有信号和NSCT制和PAL的自动检测。
@param[in] psInitConfig  用于配置LCD初始化参数的指针。
@return CSHDI_FAILURE：初始化失败。CSHDI_SUCCESS：初始化成功。
*/
CSHDI_Error_t CSALCDInitI2C( CSHDII2CIndex eI2CIndex, int nPriority, CSALCDInitCFG_T *psInitConfig);

/** 
@brief 选择LCD输入通道。

@param[in] eSourceType EM_ALCD_CVBS：选择CVBS输入通道。EM_ALCD_YC：选择YC输入通道。
@return CSHDI_SUCCESS：成功。CSHDI_FAILURE：失败
*/
CSHDI_Error_t CSALCDSourceSelect(CSALCD_SourceType_E eSourceType);

/** 
@brief 控制LCD背光。

@param[in] eLCDBGSwitch EM_ALCD_ON：LCD背光开。EM_ALCD_OFF：LCD背光关。
@return CSHDI_SUCCESS：成功，CSHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDCntBackLight( CSALCDSwitch_E eLCDBGSwitch);

/** 
@brief 在LCD上显示或清除字体。

@param[in] eLCDDISSwitch 显示或清除OSD显示，EM_ALCD_ON表示显示；EM_ALCD_OFF表示清除。
@param[in] eOSDPosition EM_ALCD_TOP_RIGHT表示在右上角显示；EM_ALCD_MIDDLE表示在屏幕中间显示。
@param[in] pucOSDText 显示的字符串，目前显示的字符串很有限。
@return CSHDI_SUCCESS：成功。CSHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDCntDisplay( CSALCDSwitch_E eLCDDISSwitch, CSALCD_OSD_POSITION_E eOSDPosition, PBYTE pucOSDText);

/** 
@brief 清除OSD显示。

@return CSHDI_SUCCESS：成功。CSHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDClearOSD(void);

/** 
@brief 控制LCD的背景。

@param[in] eBackGroupType EM_ALCD_BLUE：显示蓝屏，EM_ALCD_BLACK：显示黑屏，EM_ALCD_CLOSE_BACKGROUP：关背景
@return CSHDI_SUCCESS：成功。CSHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDCntBackGround( CSALCD_BACKGROUP_E eBackGroupType);

/** 
@brief 设置LCD参数。

@param[in] eLCDParam
- EM_ALCD_BRIGHTNESS：表示设置亮度
- EM_ALCD_CONTRAST：表示设置对比度
- EM_ALCD_SHARPNESS：表示设置锐利度
- EM_ALCD_SAT：表示设置灰度
- EM_ALCD_BACKGROUND：表示设置背光
- EM_ALCD_DAC：DAC调节
- EM_ALCD_VCOMA：VCOMA调节
- EM_ALCD_VCOMD：VCOMD调节
@param[in] nValue
@return CSHDI_SUCCESS：成功。CSHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDSetCntParam( CSALCDPARAM_E eLCDParam, int nValue);

/** 
@brief 获取LCD的参数。

@param[in] eLCDParam LCD参数类型
@param[in] pnValue LCD对应参数值的指针。
@return CSHDI_SUCCESS：成功,CSHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDGetCntParam( CSALCDPARAM_E eLCDParam, int *pnValue);

/** 
@brief 检查LCD是否有信号输入。

@param[in] pbSignal 获取是否有信号的值的地址，TRUE表示有信号，FLASE表示无信号。
@return CSHDI_SUCCESS：成功。CSHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDDetectSignal (BOOL *pbSignal);

/** 
@brief 获取视频输入标准。

@param[in] peVideoType 存放地址，详见CSALCDVideoSTD_E定义。
@return CSHDI_SUCCESS：成功，CSHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDDetectSignalStd(CSALCDVideoSTD_E *peVideoType);

/** 
@brief 设置信号输入标准。

@param[in] eVideoType 信号输入标准，详见CSALCDVideoSTD_E定义。
@return CSHDI_SUCCESS：成功，CSHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDSetSignalStd(CSALCDVideoSTD_E eVideoType);

/** 
@brief 注册回调函数，驱动回调当前是否有信号的信息。

@param[in] fnCallBackFun 回调函数的地址
@return CSHDI_SUCCESS：成功。SHDI_FAILURE：失败。
*/
CSHDI_Error_t CSALCDRegisterKeySendCallback( CSHDICallback fnCallBackFun);

/** 
@brief 获取模拟LCD驱动版本号

@param[in] nSize  版本号字符长度
@param[out] pchVer 版本号地址
@return 获取到版本号的长度。
*/
int CSALCDGetVersion( PCHAR pchVer, int nSize );

#ifdef __cplusplus
}
#endif

/** @} */

#endif

