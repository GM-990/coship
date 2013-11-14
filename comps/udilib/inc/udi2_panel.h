/**@defgroup PANEL PANEL 定义了前面板显示相关接口

@brief PANEL模块对应头文件<udi2_panel.h>，包括了终端LED及VFD类面板的显示部分接口。

本模块把面板显示类型划分成文本灯、控制灯、组合灯三种显示类型，以灯名唯一标识一个灯。灯名中已经包括数字电视机顶盒终端常见的所有显示灯功能，如有遗漏可与COSHIP协商增加。

文本灯是指可以随意改变文本显示的内容(英文和数字)的面板显示灯，包括其上的分隔符如"："等，例如7段管，米字管。下图即为一个文本灯的示例。

@image HTML udi_panel_text.png

文本灯的属性仅包括其能显示的字符数，显示值包括:
- 文本模式：即直接输入显示的文本，当输入的文本字符数多余文本子灯数量时，忽略超过的部分，但未超出的部分仍需要正常显示；
- 时间模式：即输入当前完整的时间，由驱动实现自行根据面板情况决定如何显示这个时间，如何闪烁间隔符。

文本灯的名字用于表示文本灯的主次关系，其中TEXT0表示主灯、TEXT1次之，TEXT2再次，以此类推。一般来说，应用上若需要在面板上显示当前的主状态，会获取TEXT0文本灯，如果能成功获取则根据该文本灯的长度，将内容显示在上面，否则跳过显示步骤。

控制灯是指只能显示固定图标的面板显示灯，例如邮件指示灯，信号指示灯等。下图即为一些控制灯的示例。

@image HTML udi_panel_ctrl.png

控制灯的属性包括了其显示能力，显示能力包括是否具有 - 灭、亮、闪烁、以第二种颜色亮、以第二种颜色闪烁、两种颜色交替闪烁等几种能力的组合。显示值则是将其状态设置为以上几种状态之一。其中灭、亮两种状态为基本功能，必须支持。

控制灯的名字用于表示控制灯的意义，如信号灯一般用来表示信号的锁定与否。一般来说应用上在锁频成功后会从本模块获取信号控制灯，如果能获取到则将其点亮，否则不做任何操作。其它控制灯亦然。

组合灯是指多个相关灯的灯组，如工程机上的信号强度灯组，VFD上常见的圆形灯组等。这些灯往往可以组合在一起，表示一些特殊的意义，如一个数字，动态Loading图标等等。下图即为一个组合灯的示例。

@image HTML udi_panel_suit.png

组合灯的属性包括该组灯包含的子灯数量，上图左边所示的组合灯就包括8个子灯，子灯必须是可以单独控制状态的。其显示值则是从低到高，第n位表示n号子灯的亮灭状态，0表示熄灭；1表示点亮。其中子灯的编号规则由移植层决定，但须遵循如果用组灯表示数字时，应该是1号灯单独亮表示1，1、2号灯亮表示2，1、2、3号灯亮表示3，以此类推。以上图右边的组合灯为例则其编号顺序应该是从左到右依次递增。

终端所配置面板，不一定要支持所有署名显示灯，若不支持或者不存在该类型灯时，只需要在GetCapabilityByName时返回CSUDIPANEL_ERROR_NOT_SUPPORTED即可。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.2 2009/09/02 增加控制灯双色交替闪烁属性
@version 2.0.1 2009/08/26 修改面板显示类型命名方式，认为每种显示类型都是灯的一种
@version 2.0.1 2009/08/24 第一次综合评审后修改的版本
@version 2.0.0 2009/07/23 初稿
@{
*/
#ifndef _UDI2_PANEL_H_
#define _UDI2_PANEL_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief PANEL 设备相关错误代码值*/
enum
{
	CSUDIPANEL_ERROR_BAD_PARAMETER = CSUDI_PANEL_ERROR_BASE,  ///< 参数错误 
	CSUDIPANEL_ERROR_FEATURE_NOT_SUPPORTED,					  ///< 操作不支持
	CSUDIPANEL_ERROR_UNKNOWN_ERROR,					  ///< 一般错误
	CSUDIPANEL_ERROR_INVALID_DEVICE_ID						  ///< 非法设备ID
};


/**@brief 面板显示类型枚举定义

@note 文本灯显示类型主要指可以随意改变文本显示的内容(英文和数字)，例如7段管,米字管等
@note 控制符灯显示类型主要指固定图标显示类型,如前面板上的邮件指示灯,信号指示灯等
@note 组合灯显示类型主要表示多个相关灯的灯组,如工程机上的信号强度灯组,VFD上常见的圆形灯组
*/
typedef enum
{
	EM_UDIPANEL_CTRLSTART,       	///< 控制符灯显示类型起始值,不作为类型标识
	
	EM_UDIPANEL_MAIL,     	        ///< Mail灯
	EM_UDIPANEL_SIGNAL,  	        ///< Signal灯
	EM_UDIPANEL_AUDIO, 	       		///< Audio灯
	EM_UDIPANEL_PLAY, 				///< 播放 
	EM_UDIPANEL_PAUSE, 	        	///< 暂停 
	EM_UDIPANEL_STOP, 				///< 停止 
	EM_UDIPANEL_HDTV,				///< HDTV 
	EM_UDIPANEL_BACKWARD,           ///< 快退
	EM_UDIPANEL_FORWARD, 	        ///< 快进 
	EM_UDIPANEL_BALL, 				///< 圆
	EM_UDIPANEL_DUBY, 				///< 杜比 
	EM_UDIPANEL_PREVIOUS, 	        ///< 前一个
	EM_UDIPANEL_NEXT, 				///< 后一个
	EM_UDIPANEL_IR, 				///< 遥控器
	EM_UDIPANEL_USB, 				///< USB
	EM_UDIPANEL_N,					///< N
	EM_UDIPANEL_VOD, 				///< VOD
	EM_UDIPANEL_POWER, 	        	///< 电源
	EM_UDIPANEL_TV, 				///< 电视
	EM_UDIPANEL_NETWORK, 	        ///< 网络
	EM_UDIPANEL_RADIO, 	        	///< 广播
	EM_UDIPANEL_CLOCK,				///< 时钟
	EM_UDIPANEL_MODEM,	        	///< MODEM 
	EM_UDIPANEL_REC,				///< 录制 
	EM_UDIPANEL_UP,					///< 上
	EM_UDIPANEL_DOWN,				///< 下
	EM_UDIPANEL_LEFT,				///< 左
	EM_UDIPANEL_RIGHT,				///< 右			
	
	EM_UDIPANEL_CTRLALL,       		///< 控制符灯显示类型结束值
	
	EM_UDIPANEL_TEXTSTART ,      ///< 文本灯显示类型起始值,不作为类型标识,TEXT0-TEXT4按由主到次顺序排列
	
	EM_UDIPANEL_TEXT0,	    	///< 第一行文本
	EM_UDIPANEL_TEXT1,          ///< 第二行文本
	EM_UDIPANEL_TEXT2,	    	///< 第三行文本	
	EM_UDIPANEL_TEXT3,          ///< 第四行文本
	EM_UDIPANEL_TEXT4,          ///< 第五行文本
	
	EM_UDIPANEL_TEXTALL,        ///< 文本灯显示类型结束值
	
	EM_UDIPANEL_SUITSTART ,      ///< 组合灯显示类型起始值,不作为类型标识
	
	EM_UDIPANEL_SUIT0,          ///< 第一个组合灯
	EM_UDIPANEL_SUIT1,          ///< 第二个组合灯
	EM_UDIPANEL_SUIT2,          ///< 第三个组合灯
	
	EM_UDIPANEL_SUITALL,	    ///< 组合灯显示类型结束值

	EM_UDIPANEL_EXT_CTRL_START = 0x1000,  ///< 扩展控制符灯显示类型起始值,不作为类型标识，今后若需增加控制灯则在此后添加
	EM_UDIPANEL_CMRS,					///< CM 状态:  收发
	EM_UDIPANEL_CMONLINE,			///< CM 状态:  在线
	EM_UDIPANEL_CMUPLINK,			///< CM 状态:  上行
	EM_UDIPANEL_CMDOWNLINK,		///< CM 状态:  下行
	EM_UDIPANEL_EXT_CTRL_END,

	EM_UDIPANEL_EXT_TEXT_START  =0x2000,  ///< 扩展文本灯显示类型起始值,不作为类型标识，今后若需增加文本灯则在此后添加
	EM_UDIPANEL_EXT_TEXT_END  ,
	
	EM_UDIPANEL_EXT_SUIT_START = 0x3000,  ///< 组合文本灯显示类型起始值,不作为类型标识，今后若需增加组合灯则在此后添加
	EM_UDIPANEL_EXT_SUIT_END ,

	EM_UDIPANEL_MAX             ///< 边界值	
}CSUDIPanelLightName_E;

#define CSUDI_PANEL_IS_TEXT(name)	(  ( (name) > EM_UDIPANEL_TEXTSTART &&  (name) < EM_UDIPANEL_TEXTALL ) || ( (name) > EM_UDIPANEL_EXT_TEXT_START &&  (name) < EM_UDIPANEL_EXT_TEXT_END )  ) ///< 判断name是否是文本灯显示类型
#define CSUDI_PANEL_IS_CTRL(name)   (  (  (name) > EM_UDIPANEL_CTRLSTART &&  (name) < EM_UDIPANEL_CTRLALL ) || (  (name) > EM_UDIPANEL_EXT_CTRL_START &&  (name) < EM_UDIPANEL_EXT_CTRL_END ) ) ///< 判断name是否是控制符灯显示类型
#define CSUDI_PANEL_IS_SUIT(name)   (  (  (name) > EM_UDIPANEL_SUITSTART &&  (name) < EM_UDIPANEL_SUITALL ) || (  (name) > EM_UDIPANEL_EXT_SUIT_START &&  (name) < EM_UDIPANEL_EXT_SUIT_END ) )///< 判断name是否是组合灯显示类型

/**@brief 文本灯显示类型能力信息 */
typedef struct
{
	int             m_nTextCount;                 ///< 该行文本支持显示的字符个数	
}CSUDIPanelTextLightAttr_S;

/**@brief 组合灯显示类型能力信息

@note 获取组合中灯管的个数，便于对每个灯管的状态进行操作
*/
typedef struct
{	
	int m_nSuitCount;                 ///< 组合灯子灯数量
}CSUDIPanelSuitLightAttr_S;

/**@brief 控制符灯显示类型功能枚举定义 
@note 其中点亮和熄灭是基本功能必须支持
*/
typedef enum
{
	EM_UDIPANEL_CTRL_OFF		 = 1 << 0,		///< 该控制灯可以熄灭
	EM_UDIPANEL_CTRL_ON			 = 1 << 1,		///< 该控制灯可以点亮,如果支持color2，使用默认颜色显示
	EM_UDIPANEL_CTRL_FLASH		 = 1 << 2,		///< 该控制灯可以闪烁，默认颜色的闪烁
	EM_UDIPANEL_CTRL_COLOR2		 = 1 << 3,		///< 该控制灯支持第二种颜色
	EM_UDIPANEL_CTRL_COLOR2FLASH = 1 << 4,		///< 该控制灯支持以第二种颜色闪烁
	EM_UDIPANEL_CTRL_COLORSWITCH = 1 << 5     ///< 控制灯支持两种颜色交替闪烁
}CSUDIPanelCtrlLightFun_E;

/**@brief 控制符灯显示类型能力信息*/
typedef struct
{
	CSUDI_UINT32	m_dwCtrFunMask;   ///< 相应位为1表示具有该功能值,取值为CSUDIPanelCtrlLightFun_E中各值相或, 例如(EM_UDIPANEL_FLASH |EM_UDIPANEL_COLOR2)
}CSUDIPanelCtrlLightAttr_S;

/**@brief 设备能力信息*/
typedef struct
{
	CSUDIPanelLightName_E m_eLightName;         ///< 显示灯名称，主要用于确认灯类型，详见CSUDIPanelLightName_E
	union
	{
		CSUDIPanelTextLightAttr_S	m_sText;    ///< 文本灯显示类型
		CSUDIPanelSuitLightAttr_S	m_sSuit;	///< 组合灯显示类型
		CSUDIPanelCtrlLightAttr_S	m_sCtrl;	///< 控制符灯显示类型
	}m_uProperty;                               ///< 显示类型能力联合结构体,根据m_eLightName字段的类型取对应显示设备类型描述字段
}CSUDIPanelLightCapability_S;

/**@brief 文本灯显示类型枚举*/
typedef enum
{
	EM_UDIPANEL_TEXT_STRING,			///< 字符串类型
	EM_UDIPANEL_TEXT_TIME			///< 时间类型
}CSUDIPanelTextType_E;

/**@brief 文本灯显示类型的显示值*/
typedef struct
{
	CSUDIPanelTextType_E  m_eDisplayType;	///< 显示类型
	char				m_szStr[32];        /**< 若m_eDisplayType为文本类型则为显示字符串,字符串必须以0结尾,对应字节为空格表示熄灭该字符;若字符数量超过总子灯数，忽略超过部分
												 若m_eDisplayType为时间类型则内容为yyyymmddhhmmss格式的时间,具体显示方式及间隔符号的闪烁方式由移植层自行根据面板情况确定 */
    int 				m_nStrCount;        ///< 要显示的字符个数，最大不超过32个字符
}CSUDIPanelTextValue_S;

/**@brief 组合灯显示类型的显示值

@note 默认一个组合最多包含32个灯
*/
typedef struct
{
	CSUDI_UINT32	m_dwSuitValueMask;         ///< 从低到高第n位的值表示组合中第n个灯的状态,0表示熄灭,1表示点亮	
}CSUDIPanelSuitValue_S;


/**@brief 控制符灯显示类型的显示值*/
typedef struct
{
	CSUDIPanelCtrlLightFun_E	m_eCtrlValue;	///< 表示该控制灯的显示状态
}CSUDIPanelCtrlValue_S;

/**@brief 显示类型的显示值 */
typedef struct
{
	CSUDIPanelLightName_E m_eLightName;         ///< 显示灯名称，详见CSUDIPanelLightName_E
	union
	{
		CSUDIPanelTextValue_S		m_sTextValue;
		CSUDIPanelSuitValue_S		m_sSuitValue;
		CSUDIPanelCtrlValue_S		m_sCtrlValue;
	}m_uValue;                         ///< 显示类型显示值联合结构体,根据m_eLightName字段的类型取对应显示设备类型描述字段	
}CSUDIPanelLightValule_S;

/**
@brief 得到指定功能灯的能力信息

@param[in] nPanelIndex 显示设备的索引，默认从0开始索,直到没有更多的面板
@param[in] eName  显示设备类型名 
@param[out] psLightCapablity  保存读出的指定显示类型的属性结构信息的指针
@return CSUDI_SUCCESS表示成功,错误设备号返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID,不存在对应灯返回CSUDIPANEL_ERROR_NOT_SUPPORTED
@note eName不允许指定为EM_UDIPANEL_CTRLSTART、EM_UDIPANEL_CTRLALL、EM_UDIPANEL_TEXTSTART、
EM_UDIPANEL_TEXTALL、EM_UDIPANEL_SUITSTART、EM_UDIPANEL_SUITALL、EM_UDIPANEL_MAX枚举类型，否则返回CSUDIPANEL_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIPanelGetCapabilityByName(int nPanelIndex,CSUDIPanelLightName_E eName,CSUDIPanelLightCapability_S * psLightCapablity);

/**
@brief 设置指定显示类型的显示内容
 
@param[in] nPanelIndex  显示设备的索引，默认从0 开始索引,直到没有更多的面板
@param[in] psValue 指定灯的显示值
@note 该接口调用成功后，设置的内容立即在面板上显示
@return CSUDI_SUCCESS 表示成功,错误设备号返回CSUDIPANEL_ERROR_INVALID_DEVICE_ID,不存在对应灯返回CSUDIPANEL_ERROR_NOT_SUPPORTED
@note psValue->m_eLightName不允许指定为EM_UDIPANEL_CTRLSTART、EM_UDIPANEL_CTRLALL、EM_UDIPANEL_TEXTSTART、
EM_UDIPANEL_TEXTALL、EM_UDIPANEL_SUITSTART、EM_UDIPANEL_SUITALL、EM_UDIPANEL_MAX枚举类型，否则返回CSUDIPANEL_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIPanelDisplay(int nPanelIndex,const CSUDIPanelLightValule_S * psValue);

/**
@brief 清除面板显示信息
@param[in] nPanelIndex  显示设备的索引，默认从0 开始索引,直到没有更多的面板
@param[in] eName   选择要清除的显示类型，详见CSUDIPanelLightName_E
@note 
    - 如果eName类型为EM_UDIPANEL_TEXTALL，则所有TEXT显示类型都会熄灭
    - 如果eName类型为EM_UDIPANEL_CTRLALL，则所有CTRL显示类型都会熄灭
    - 如果eName类型为EM_UDIPANEL_SUITALL，则所有SUIT显示类型都会熄灭
    - 如果eName类型为EM_UDIPANEL_MAX，则所有显示类型都会熄灭
@return CSUDI_SUCCESS 表示成功，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIPanelClear(int nPanelIndex,CSUDIPanelLightName_E eName);

#ifdef __cplusplus
}
#endif

/** @} */

#endif



