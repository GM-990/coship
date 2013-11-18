/**@defgroup CSINPUT input  提供输入设备交互接口
@brief 本模块提供嵌入式设备的交互式相关输入输出接口,包括按键的输入和简单的信息显示

@version 0.2 2008/10/29 添加部分详细描述
@version 0.1 2008/10/23 初始版本
@{
*/

#include "udi_hditypedef.h"
#include "udi2_typedef.h"
#include "udi2_input.h"

#ifndef _UDI_INPUT_H_
#define _UDI_INPUT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**@brief 前面板输入类型*/
#define PI_EV_INPUT_FRONTBOARD	    0x3002
/**@brief 遥控器输入类型*/		
#define PI_EV_INPUT_REMOTE_CTRL	    0x3001	
/**@brief 键盘输入类型*/	
#define PI_EV_INPUT_KEYBOARD		0x3003	
/**@brief 鼠标输入类型*/	
#define PI_EV_INPUT_MOUSE			0x3004  	

/**@ 以下定义表示是否有功能键触发和触发类型（按下还是松开触发）*/
#define KEY_SHIFT					0x0001
#define KEY_CTRL					0x0002
#define KEY_ALT						0x0004

#define KEY_PRESS					EM_UDIINPUT_KEY_PRESS
#define KEY_RELEASE				    EM_UDIINPUT_KEY_RELEASE

/**@brief 以下定义鼠标事件类型*/
#define MOUSE_LEFT					0x0001
#define MOUSE_MIDDLE				0x0002
#define MOUSE_RIGHT				    0x0004
#define MOUSE_PRESS				    0x0100
#define MOUSE_RELEASE				0x0200
#define MOUSE_MOVE					0x0400

/**@brief 以下定义遥控器和键盘的键值*/
#define CS_VK_CHANNEL_UP		    CSUDI_VK_CHANNEL_UP
#define CS_VK_CHANNEL_DOWN	        CSUDI_VK_CHANNEL_DOWN
#define CS_VK_VOLUME_UP			    CSUDI_VK_VOLUME_UP
#define CS_VK_VOLUME_DOWN		    CSUDI_VK_VOLUME_DOWN
#define CS_VK_OK					CSUDI_VK_OK
#define CS_VK_CANCEL				0X03    ///返回

#define CS_VK_MENU					CSUDI_VK_MENU
#define CS_VK_MUTE					CSUDI_VK_MUTE
#define CS_VK_INFO					CSUDI_VK_INFO
#define CS_VK_GUIDE				    CSUDI_VK_GUIDE
#define CS_VK_COLORED_KEY_0	        CSUDI_VK_COLORED_KEY_0  ///Red
#define CS_VK_COLORED_KEY_1	        CSUDI_VK_COLORED_KEY_1  ///green
#define CS_VK_COLORED_KEY_2	        CSUDI_VK_COLORED_KEY_2  ///yellow
#define CS_VK_COLORED_KEY_3	        CSUDI_VK_COLORED_KEY_3  ///blue
#define CS_VK_AUDIO					CSUDI_VK_AUDIO
#define CS_VK_LAN					0x0198

#define CS_VK_TOGGLE                CSUDI_VK_TOGGLE
#define CS_VK_PROG_LIST             CSUDI_VK_PROG_LIST
#define CS_VK_TV_RADIO              CSUDI_VK_TV_RADIO
#define CS_VK_CARD_INFO             CSUDI_VK_CARD_INFO
#define CS_VK_MAIL_INFO             CSUDI_VK_MAIL_INFO
#define CS_VK_VOD                   CSUDI_VK_VOD
#define CS_VK_SYSSET				CSUDI_VK_SYSSET

#define CS_VK_ENTER					'\n'
#define CS_VK_BACK_SPACE		    '\b'
#define CS_VK_TAB					CSUDI_VK_TAB
#define CS_VK_CLEAR					0x0C
#define CS_VK_SHIFT					0x10
#define CS_VK_CONTROL				0x11
#define CS_VK_ALT					0x12

#define CS_VK_CAPS_LOCK			    0x14
#define CS_VK_ESCAPE				CSUDI_VK_EXIT    ///退出
#define CS_VK_SPACE					0x20
#define CS_VK_PAGE_UP				CSUDI_VK_PAGE_UP
#define CS_VK_PAGE_DOWN			    CSUDI_VK_PAGE_DOWN
#define CS_VK_END					0x23
#define CS_VK_HOME					0x24
#define CS_VK_LEFT					CSUDI_VK_LEFT
#define CS_VK_UP					CSUDI_VK_UP
#define CS_VK_RIGHT					CSUDI_VK_RIGHT
#define CS_VK_DOWN					CSUDI_VK_DOWN
#define CS_VK_COMMA					0x2C
#define CS_VK_PERIOD				0x2E
#define CS_VK_SLASH					0x2F
#define CS_VK_0						CSUDI_VK_0
#define CS_VK_1						CSUDI_VK_1
#define CS_VK_2						CSUDI_VK_2
#define CS_VK_3						CSUDI_VK_3
#define CS_VK_4						CSUDI_VK_4
#define CS_VK_5						CSUDI_VK_5
#define CS_VK_6						CSUDI_VK_6
#define CS_VK_7						CSUDI_VK_7
#define CS_VK_8						CSUDI_VK_8
#define CS_VK_9						CSUDI_VK_9
#define CS_VK_POWER					CSUDI_VK_POWER
#define CS_VK_SEMICOLON			    0x3B
#define CS_VK_EQUALS				0x3D
#define CS_VK_A						0x41
#define CS_VK_B						0x42
#define CS_VK_C						0x43
#define CS_VK_D						0x44
#define CS_VK_E						0x45
#define CS_VK_F						0x46
#define CS_VK_G						0x47
#define CS_VK_H						0x48
#define CS_VK_I						0x49
#define CS_VK_J						0x4A
#define CS_VK_K						0x4B
#define CS_VK_L						0x4C
#define CS_VK_M						0x4D
#define CS_VK_N						0x4E
#define CS_VK_O						0x4F
#define CS_VK_P						0x50
#define CS_VK_Q						0x51
#define CS_VK_R						0x52
#define CS_VK_S						0x53
#define CS_VK_T						0x54
#define CS_VK_U						0x55
#define CS_VK_V						0x56
#define CS_VK_W						0x57
#define CS_VK_X						0x58
#define CS_VK_Y						0x59
#define CS_VK_Z						0x5A
#define CS_VK_OPEN_BRACKET	        0x5B
#define CS_VK_BACK_SLASH		    0x5C
#define CS_VK_CLOSE_BRACKET	        0x5D
#define CS_VK_NUMPAD0				0x60
#define CS_VK_NUMPAD1				0x61
#define CS_VK_NUMPAD2				0x62
#define CS_VK_NUMPAD3				0x63
#define CS_VK_NUMPAD4				0x64
#define CS_VK_NUMPAD5				0x65
#define CS_VK_NUMPAD6				0x66
#define CS_VK_NUMPAD7				0x67
#define CS_VK_NUMPAD8				0x68
#define CS_VK_NUMPAD9				0x69
#define CS_VK_MULTIPLY			    0x6A
#define CS_VK_ADD					0x6B
#define CS_VK_SEPARATER			    0x6C
#define CS_VK_SUBTRACT			    0x6D
#define CS_VK_DECIMAL				0x6E
#define CS_VK_DIVIDE				0x6F
#define CS_VK_F1					0x70
#define CS_VK_F2					0x71
#define CS_VK_F3					0x72
#define CS_VK_F4					0x73
#define CS_VK_F5					0x74
#define CS_VK_F6					0x75
#define CS_VK_F7					0x76
#define CS_VK_F8					0x77
#define CS_VK_F9					0x78
#define CS_VK_F10					0x79
#define CS_VK_F11					0x7A
#define CS_VK_F12					0x7B
#define CS_VK_DELETE				0x7F


/**@brief 以下定义浏览器的传入值 */
/*************************************************************/
#define CS_VK_REFRESH               CSUDI_VK_REFRESH       	/**<  刷新网页 */
#define CS_VK_FORWARD               CSUDI_VK_FORWARD		/**<  前进（网页） */
#define CS_VK_BACK                  CSUDI_VK_BACK		/**<  后退（网页） */
#define CS_VK_BROSER                CSUDI_VK_BROWSER		/**<  启动浏览器 */
#define CS_VK_INPUT                 CSUDI_VK_INPUT		/**<  启动T9输入法 */
/*************************************************************/
#define CS_VK_NUM_LOCK			    0x90
#define CS_VK_SCROLL_LOCK		    0x91
#define CS_VK_PRINTSCREEN		    0x9A
#define CS_VK_INSERT				0x9B
#define CS_VK_UNDEFINED			    0x0

#define CS_VK_TELETEXT     	        0x1CB
#define CS_VK_SUBTITLE     	        0x1CC

/**@brief 以下定义PVR使用的键值*/
#define CS_VK_RECORD       	        CSUDI_VK_RECORD
#define CS_VK_HDD_LIST	 		    0xf11
#define CS_VK_PIP 					0xf12
#define CS_VK_SWAP 					0xf13
#define CS_VK_ASSIST 				0xf14
#define CS_VK_PLAY 					CSUDI_VK_PLAY
#define CS_VK_STOP 					CSUDI_VK_STOP
#define CS_VK_PAUSE 				CSUDI_VK_PAUSE
#define CS_VK_FAV 					CSUDI_VK_FAV
#define CS_VK_RW   					CSUDI_VK_RW
#define CS_VK_FF   					CSUDI_VK_FF
#define CS_VK_SF   					0xf23
#define CS_VK_RECALL   			    0xf24
#define CS_VK_TIMER_REC			    0xf25

#define CS_VK_POUND					CSUDI_VK_POUND			/**< Constant for the "#" key*/
#define CS_VK_ASTERISK			    CSUDI_VK_ASTERISK			/**< Constant for the "*" key*/
#define CS_VK_SEEK                  CSUDI_VK_SEEK

/**@brief 以下定义一些快捷键*/
#define CS_VK_GAME 					0xf29			/**< 游戏*/
#define CS_VK_ADS 					0xf2A			/**< 广告*/
#define CS_VK_NVOD					0xf2B			/**< NVOD*/

#define CS_VK_STOCK        0xf2C
#define CS_VK_PAYOUT      0xf2D
#define CS_VK_PORTAL	0xf2E

#define CS_VK_SERV                    (CSUDI_VK_SERV)
#define CS_VK_TV                    (CSUDI_VK_TV)
#define CS_VK_HELP                  (CSUDI_VK_HELP)


/**@brief 输入信息结果类型 */
typedef struct _CSInputInfo_S
{
	int			nType;			/**< 输入设备来源，PI_EV_INPUT_FRONTBOARD,PI_EV_INPUT_REMOTE_CTRL,PI_EV_INPUT_KEYBOARD or PI_EV_INPUT_MOUSE*/
	int			nStatus;		/**< 输入按键状态 ,KEY_SHIFT	,KEY_CTRL,KEY_ALT,KEY_PRESS or KEY_RELEASE*/
	int			nKeyCode;		/**< 输入键码 */
}CSInputInfo_S;	

/**@brief struct of the second parameter of DebugCallback */
typedef  void ( *CSInputCallback )(CSInputInfo_S);

/**
@brief 注册输入设备按键回调函数

任何输入设备产生有效的按键信息后，通过注册的回调函数通知应用层
@param[in] pInputCallback 指向回调函数的函数指针，用户通过回调函数得按键来源、状态和功能码
@return HCSHANDLE 注册成功;NULL：注册失败
@note
*/
HCSHANDLE CSInputAddCallback( CSInputCallback pInputCallback );


/**
@brief 删除输入设备按键回调函数

@param[in] hHandle 调用CSInputAddCallback得到的句柄
@return TRUE：删除成功;FALSE：删除失败
@note
*/
BOOL CSInputDelCallback( HCSHANDLE hHandle);

#ifdef __cplusplus
}
#endif

/** @} */

#endif   

