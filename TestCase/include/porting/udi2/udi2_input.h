/**@defgroup INPUT INPUT 提供输入设备交互接口
@brief INPUT模块对应头文件<udi2_input.h>，定义了所有输入设备的控制接口。现对该模块
进行如下说明：

- 无论哪个模块的按键（遥控器、前面板等）均通过此模块通知到Eastwin。
- 遥控器的实际按键与功能键的对应关系根据项目的不同由运营商、COSHIP共同确定。例如：
某项目需要在按F1键时调出输入法，则需要在收到F1键时发送CS_VK_INPUT键值。
- 不要求所有按键都支持EM_UDIINPUT_KEY_REPEAT消息，具体哪些按键要求支持根据项目的不同由运营商、COSHIP共同确定，
要求支持EM_UDIINPUT_KEY_REPEAT消息的按键在被长时间按住不放时，要求按下时发送EM_UDIINPUT_KEY_PRESS消息、不放开过程中持续发送EM_UDIINPUT_KEY_REPEAT消息、放开时发送EM_UDIINPUT_KEY_RELEASE消息
不要求支持EM_UDIINPUT_KEY_REPEAT消息的按键在被长时间按住不放时，要求按下时发送EM_UDIINPUT_KEY_PRESS消息、放开时发送EM_UDIINPUT_KEY_RELEASE消息。
- 该模块未定义的键值一般会透传到应用层，如JavaScript或者Java应用中。但Eastwin内部
会有部分按键与逻辑相对应，如CSUDI_VK_OK对应进入超链接等，而不是完全透传。所以不可
假设Eastwin仅仅只做透传，而自行改变这些键值。

@version 2.0.2 2009/8/25 调整定义顺序后的版本
@version 2.0.1 2009/8/24 第一次综合评审后修改的版本
@version 2.0.0 2009/8/16  
@{
*/

#ifndef _UDI2_INPUT_H_
#define _UDI2_INPUT_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

//遥控器，前面板按键
#define CSUDI_VK_POWER					0xffff ///< 待机键
#define CSUDI_VK_0						0x30	///< 数字键0
#define CSUDI_VK_1						0x31	///< 数字键1
#define CSUDI_VK_2						0x32	///< 数字键2
#define CSUDI_VK_3						0x33	///< 数字键3
#define CSUDI_VK_4						0x34	///< 数字键4
#define CSUDI_VK_5						0x35	///< 数字键5
#define CSUDI_VK_6						0x36	///< 数字键6
#define CSUDI_VK_7						0x37	///< 数字键7
#define CSUDI_VK_8						0x38	///< 数字键8
#define CSUDI_VK_9						0x39	///< 数字键9
#define CSUDI_VK_OK			       	 	0x0D   ///< 确认键
#define CSUDI_VK_TOGGLE                	0x0280 ///< 返回键
#define CSUDI_VK_EXIT					0x1B   ///< 退出键
#define CSUDI_VK_PAGE_UP				0x21   ///< 向上翻页
#define CSUDI_VK_PAGE_DOWN			    0x22   ///< 向下翻页
#define CSUDI_VK_LEFT					0x25   ///< 向左键 
#define CSUDI_VK_UP						0x26   ///< 向上键
#define CSUDI_VK_RIGHT					0x27   ///< 向右键
#define CSUDI_VK_DOWN					0x28   ///< 向下键
#define CSUDI_VK_CHANNEL_UP				0x01AB	///< 频道增加键
#define CSUDI_VK_CHANNEL_DOWN			0x01AC  ///< 频道减少键
#define CSUDI_VK_VOLUME_UP				0x01BF  ///< 音量增加键
#define CSUDI_VK_VOLUME_DOWN			0x01C0	///< 音量减少键
#define CSUDI_VK_MENU					0X01D4  ///< 菜单键
#define CSUDI_VK_MUTE					0x01C1	///< 静音键
#define CSUDI_VK_GUIDE					0x01CA	///< 节目指南键
#define CSUDI_VK_AUDIO					0x0197  ///< 声道键
#define CSUDI_VK_COLORED_KEY_0	        0X0193  ///< 红色键
#define CSUDI_VK_COLORED_KEY_1	        0X0194  ///< 绿色键
#define CSUDI_VK_COLORED_KEY_2	        0X0195  ///< 黄色键
#define CSUDI_VK_COLORED_KEY_3	        0X0196  ///< 蓝色键

#define CSUDI_VK_PROG_LIST             0x0281 ///< 频道列表键
#define CSUDI_VK_TV_RADIO              0x0282 ///< 电视/音频广播键
#define CSUDI_VK_CARD_INFO             0x0283 ///< 卡信息键
#define CSUDI_VK_MAIL_INFO             0x0284 ///< 邮件键
#define CSUDI_VK_VOD                   0x0285 ///< 点播键
#define CSUDI_VK_SYSSET					0x0286 ///< 系统设置键
#define CSUDI_VK_HELP					0x1E3   ///< 帮助键
#define CSUDI_VK_SERV					0x01D0  ///< 资讯键
#define CSUDI_VK_FAV					0x01E0	///< 喜爱键
#define CSUDI_VK_TV						0x01E1	///< 电视键
#define CSUDI_VK_RADIO					0x01E2  ///< 广播键
#define CSUDI_VK_INFO					0x01C9  ///< 信息键
#define CSUDI_VK_LANGUAGE				0x0198  ///< 中/英文切换键
#define CSUDI_VK_INPUT                 	0x84	///<  输入法按键

#define CSUDI_VK_REFRESH               0x80    ///<  刷新网页 
#define CSUDI_VK_FORWARD               0x81	///<  前进（历史网页） 
#define CSUDI_VK_BACK                  0x82	///<  后退（历史网页） 
#define CSUDI_VK_BROWSER                0x83	///<  启动浏览器 
#define CSUDI_VK_RECORD       	        0xf10	///< 录制键
#define CSUDI_VK_PLAY 					0xf16	///< 播放
#define CSUDI_VK_STOP 					0xf17	///< 停止播放
#define CSUDI_VK_PAUSE 					0xf18   ///< 暂停
#define CSUDI_VK_RW   					0xf21	///< 快退
#define CSUDI_VK_FF   					0xf22	///< 快进
#define CSUDI_VK_SF   					0xf23	///< 慢进
#define CSUDI_VK_SB   					0xf24	///< 慢退

#define CSUDI_VK_POUND					0x23	///< "#"键
#define CSUDI_VK_ASTERISK			    0x2A	///< "*"键
#define CSUDI_VK_SEEK                  	0xf28   ///< 定位键
#define CSUDI_VK_TAB					0x09	///< Tab键

#define CSUDI_VK_END            		0x23	///< END键
#define CSUDI_VK_HOME           		0x24 	///< Home键
#define CSUDI_VK_INSERT         		0x9B	///< Insert键
#define CSUDI_VK_DELETE        			0x7F	///< Delete键

#define CSUDI_VK_TELETEXT				0x01CB	///<字幕键
#define CSUDI_VK_SUBTITLE				0x01CC	///<subtitle键

#define CSUDI_VK_RECALL					0xf24	///<Recall键
#define CSUDI_VK_TIMER_REC				0xf25	///<显示预订列表

#define CSUDI_VK_GAME					0xf29	///<游戏快捷键
#define CSUDI_VK_ADS					0xf2A	///<广告快捷键
#define CSUDI_VK_NVOD					0xf2B	///<NVOD快捷键
#define CSUDI_VK_STOCK					0xf2C	///<股票快捷键
#define CSUDI_VK_PAYOUT					0xf2D	///<支付快捷键
#define CSUDI_VK_PORTAL					0xf2E	///<Portal快捷键
		
#define CSUDI_VK_LBUTTON				0x01	///<鼠标左键
#define CSUDI_VK_RBUTTON				0x02	///<鼠标右键
#define CSUDI_VK_CANCEL					0x03	///<取消键（Ctrl-Break）
#define CSUDI_VK_MBUTTON				0x04	///<鼠标中键
#define CSUDI_VK_CLEAR					0x0C	///<Num Lock关闭时的数字键盘5
#define CSUDI_VK_SHIFT					0x10	///<Shift键
#define CSUDI_VK_CONTROL				0x11	///<Ctrl键
#define CSUDI_VK_ALT					0x12	///<Alt键
#define CSUDI_VK_CAPITAL				0x14	///<Caps lock键
#define CSUDI_VK_SPACE					0x20	///<空格键
#define CSUDI_VK_SELECT					0x29	///<Select键
#define CSUDI_VK_PRINT					0x2A	///<Print键
#define CSUDI_VK_EXECUTE				0x2B	///<Execute键
#define CSUDI_VK_SNAPSHOT				0x2C	///<Print Screen键
#define CSUDI_VK_A						0x41	///<字母键A
#define CSUDI_VK_B						0x42	///<字母键B
#define CSUDI_VK_C						0x43	///<字母键C
#define CSUDI_VK_D						0x44	///<字母键D
#define CSUDI_VK_E						0x45	///<字母键E
#define CSUDI_VK_F						0x46	///<字母键F
#define CSUDI_VK_G						0x47	///<字母键G
#define CSUDI_VK_H						0x48	///<字母键H
#define CSUDI_VK_I						0x49	///<字母键I
#define CSUDI_VK_J						0x4A	///<字母键J
#define CSUDI_VK_K						0x4B	///<字母键K
#define CSUDI_VK_L						0x4C	///<字母键L
#define CSUDI_VK_M						0x4D	///<字母键M
#define CSUDI_VK_N						0x4E	///<字母键N
#define CSUDI_VK_O						0x4F	///<字母键O
#define CSUDI_VK_P						0x50	///<字母键P
#define CSUDI_VK_Q						0x51	///<字母键Q
#define CSUDI_VK_R						0x52	///<字母键R
#define CSUDI_VK_S						0x53	///<字母键S
#define CSUDI_VK_T						0x54	///<字母键T
#define CSUDI_VK_U						0x55	///<字母键U
#define CSUDI_VK_V						0x56	///<字母键V
#define CSUDI_VK_W						0x57	///<字母键W
#define CSUDI_VK_X						0x58	///<字母键X
#define CSUDI_VK_Y						0x59	///<字母键Y
#define CSUDI_VK_Z						0x5A	///<字母键Z
#define CSUDI_VK_LWIN					0x6B	///<左窗口
#define CSUDI_VK_RWIN					0x7C	///<右窗口
#define CSUDI_VK_NUMPAD0				0x60	///<小键盘0键
#define CSUDI_VK_NUMPAD1				0x61	///<小键盘1键
#define CSUDI_VK_NUMPAD2				0x62	///<小键盘2键
#define CSUDI_VK_NUMPAD3				0x63	///<小键盘3键
#define CSUDI_VK_NUMPAD4				0x64	///<小键盘4键
#define CSUDI_VK_NUMPAD5				0x65	///<小键盘5键
#define CSUDI_VK_NUMPAD6				0x66	///<小键盘6键
#define CSUDI_VK_NUMPAD7				0x67	///<小键盘7键
#define CSUDI_VK_NUMPAD8				0x68	///<小键盘8键
#define CSUDI_VK_NUMPAD9				0x69	///<小键盘9键
#define CSUDI_VK_MULTIPLY				0x6A	///<小键盘*
#define CSUDI_VK_ADD					0x6B	///<小键盘+
#define CSUDI_VK_SEPARATER				0x6C	///<ENTER键
#define CSUDI_VK_SUBTRACT				0x6D	///<小键盘-
#define CSUDI_VK_DECIMAL				0x6E	///<小键盘.
#define CSUDI_VK_DIVIDE					0x6F	///<小键盘/
#define CSUDI_VK_F1						0x70	///<F1键
#define CSUDI_VK_F2						0x71	///<F2键
#define CSUDI_VK_F3						0x72	///<F3键
#define CSUDI_VK_F4						0x73	///<F4键
#define CSUDI_VK_F5						0x74	///<F5键
#define CSUDI_VK_F6						0x75	///<F6键
#define CSUDI_VK_F7						0x76	///<F7键
#define CSUDI_VK_F8						0x77	///<F8键
#define CSUDI_VK_F9						0x78	///<F9键
#define CSUDI_VK_F10					0x79	///<F10键
#define CSUDI_VK_F11					0x7A	///<F11键
#define CSUDI_VK_F12					0x7B	///<F12键
#define CSUDI_VK_F13					0x7C	///<F13键
#define CSUDI_VK_F14					0x7D	///<F14键
#define CSUDI_VK_F15					0x7E	///<F15键
#define CSUDI_VK_F16					0x7F	///<F16键
#define CSUDI_VK_NUM_LOCK				0x90	///<num lock键
#define CSUDI_VK_SCROLL_LOCK			0x91	///<scroll lock键
#define CSUDI_VK_OEM_1					0xBA	///<"；" or "："
#define CSUDI_VK_OEM_PLUS				0xBB	///<"=" or "+"
#define CSUDI_VK_OEM_COMMA				0xBC	///<"," or "<"
#define CSUDI_VK_OEM_MINUS				0xBD	///<"_" or "-"
#define CSUDI_VK_OEM_PERIOD				0xBE	///<"." or ">"
#define CSUDI_VK_OEM_2					0xBF	///<"/" or "?"
#define CSUDI_VK_OEM_3					0xC0	///<"~" or "`"
#define CSUDI_VK_OEM_4					0xDB	///<"[" or "{"
#define CSUDI_VK_OEM_5					0xDC	///<"\" or "|"
#define CSUDI_VK_OEM_6					0xDD	///<"]" or "}"
#define CSUDI_VK_OEM_7					0xDE	///<"'" or """

#define CSUDI_VK_PRIOR					CSUDI_VK_PAGE_UP	///<向上翻页键
#define CSUDI_VK_NEXT						CSUDI_VK_PAGE_DOWN	///<向下翻页键
#define CSUDI_VK_RETURN					CSUDI_VK_OK			///<回车键
#define CSUDI_VK_ESCAPE					CSUDI_VK_EXIT		///<ESC键(返回)

#define CSUDI_VK_HDD_LIST					0xf11	///<硬盘文件列表键
#define CSUDI_VK_PIP							0xf12	///<PIP
#define CSUDI_VK_SWAP							0xf13	///<交换窗口
#define CSUDI_VK_ASSIST						0xf14	///<快捷菜单键/助手键/魔键

#define CSUDI_VK_INSERT_WIN				0x2D	///<Insert键,为了不与awt中该按键值定义冲突，windows中另定义的按键加上下标 _WIN 标志
#define CSUDI_VK_DELETE_WIN				0x2E	///<Delete键,windows中使用。
#define CSUDI_VK_BACK_WIN					0x08	///<back space键

/** 很多项目有特殊的按键，对于第三方集成来说需要增加到udi中 */
#define CSUDI_VK_INTERACTION 0xAAA				///< 交互键
#define CSUDI_VK_TIMESHIFT   0xAAB				///< 时移键
#define CSUDI_VK_SEARCH      0xAAC				///< 搜索
#define CSUDI_VK_ALL      0xAAD					///< 全部键

#define CSUDI_VK_REPLAY         0xAAE               //回看当前节目键
#define CSUDI_VK_HDTV           0xAAF				//高清节目键
#define CSUDI_VK_PPV_CALL       0xAB0				//PPV节目键
#define CSUDI_VK_PRE_DAY_EPG    0xAB1				//前一天的EPG信息键
#define CSUDI_VK_NEXT_DAY_EPG   0xAB2				//后一天的EPG信息键
#define CSUDI_VK_MOSAIC         0xAB3				//mosaic键


/**@brief INPUT 设备相关错误代码值*/
enum
{
	CSUDIINPUT_ERROR_BAD_PARAMETER = CSUDI_INPUT_ERROR_BASE,	///< 参数错误
	CSUDIINPUT_ERROR_FEATURE_NOT_SUPPORTED,						///< 操作不支持 
	CSUDIINPUT_ERROR_ALREADY_ADDED,                     ///< 回调函数已经注册
	CSUDIINPUT_ERROR_CALLBACK_FULL,                     ///< 注册的回调函数已经达到最大值
	CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST                   ///< 要删除的回调没有被注册
};

/**@brief 输入按键状态类型枚举定义 */
typedef enum
{
	EM_UDIINPUT_KEY_PRESS     = 0x0100,    ///< 按键按下
	EM_UDIINPUT_KEY_RELEASE   = 0x0200,    ///< 按键释放
	EM_UDIINPUT_KEY_REPEAT    = 0x0400    ///< 按键长按
}CSUDIInputKeyStatus_E;

/**@brief 输入设备来源类型枚举定义 */
typedef enum
{
	EM_UDIINPUT_FRONTPANEL,  ///< 前面板输入类型
	EM_UDIINPUT_REMOTECTRL,  ///< 遥控器输入类型
	EM_UDIINPUT_KEYBOARD,    ///< 键盘输入类型
	EM_UDIINPUT_MOUSE,        ///< 鼠标输入类型
	EM_UDIINPUT_GENERALCODES,  ///< 普通字节流，目前可用于刷卡遥控器
	EM_UDIINPUT_JOYSTICK    ///<游戏手柄
}CSUDIInputType_E;

/**@brief 输入设备事件类型枚举定义 , 对应  CSUDIInputEvent_S.m_uEeventType*/
typedef enum
{
	EM_UDIINPUT_EV_SYN=0x00,  ///< 同步事件
	EM_UDIINPUT_EV_KEY=0x01,  ///< 按键,CSUDIInputInfo_S.m_nKeyCode字节1和0为按键值
	EM_UDIINPUT_EV_REL=0x02,    ///< 相对坐标,Relative
	EM_UDIINPUT_EV_ABS=0x03,        ///< 绝对坐标,Absolute
	EM_UDIINPUT_EV_MSC=0x04,  ///< 其他Misc
	EM_UDIINPUT_EV_LED=0x11,  ///< LED灯,CSUDIInputInfo_S.m_nKeyCode字节1为CSUDIInputLEDType_E,字节0为LED灯状态CSUDIInputLEDStatus_E
	EM_UDIINPUT_EV_SND=0x12,    ///< 声音
	EM_UDIINPUT_EV_REP=0x14,        ///< repeat
	EM_UDIINPUT_EV_FF=0x15,  ///< ForceFeedback
	EM_UDIINPUT_EV_PWR=0x16,        ///< Power
	EM_UDIINPUT_EV_FF_STATUS=0x17,  ///< ForceFeedbackStatus
	EM_UDIINPUT_EV_MAX=0x1f  ///<事件最大值
}CSUDIInputEventType_E;


/**@brief mouse keypad joystick Synchronization events定义，对应  CSUDIInputEvent_S.m_uCode*/
#define CSUDIINPUT_SYN_REPORT		0
#define CSUDIINPUT_SYN_CONFIG		1

 /**@brief mouse keypad joystick Keys and buttons 定义 ，对应  CSUDIInputEvent_S.m_uCode*/
#define CSUDIINPUT_KEY_RESERVED		0
#define CSUDIINPUT_KEY_ESC			1
#define CSUDIINPUT_KEY_1			2
#define CSUDIINPUT_KEY_2			3
#define CSUDIINPUT_KEY_3			4
#define CSUDIINPUT_KEY_4			5
#define CSUDIINPUT_KEY_5			6
#define CSUDIINPUT_KEY_6			7
#define CSUDIINPUT_KEY_7			8
#define CSUDIINPUT_KEY_8			9
#define CSUDIINPUT_KEY_9			10
#define CSUDIINPUT_KEY_0			11
#define CSUDIINPUT_KEY_MINUS		12
#define CSUDIINPUT_KEY_EQUAL		13
#define CSUDIINPUT_KEY_BACKSPACE		14
#define CSUDIINPUT_KEY_TAB			15
#define CSUDIINPUT_KEY_Q			16
#define CSUDIINPUT_KEY_W			17
#define CSUDIINPUT_KEY_E			18
#define CSUDIINPUT_KEY_R			19
#define CSUDIINPUT_KEY_T			20
#define CSUDIINPUT_KEY_Y			21
#define CSUDIINPUT_KEY_U			22
#define CSUDIINPUT_KEY_I			23
#define CSUDIINPUT_KEY_O			24
#define CSUDIINPUT_KEY_P			25
#define CSUDIINPUT_KEY_LEFTBRACE		26
#define CSUDIINPUT_KEY_RIGHTBRACE		27
#define CSUDIINPUT_KEY_ENTER		28
#define CSUDIINPUT_KEY_LEFTCTRL		29
#define CSUDIINPUT_KEY_A			30
#define CSUDIINPUT_KEY_S			31
#define CSUDIINPUT_KEY_D			32
#define CSUDIINPUT_KEY_F			33
#define CSUDIINPUT_KEY_G			34
#define CSUDIINPUT_KEY_H			35
#define CSUDIINPUT_KEY_J			36
#define CSUDIINPUT_KEY_K			37
#define CSUDIINPUT_KEY_L			38
#define CSUDIINPUT_KEY_SEMICOLON		39
#define CSUDIINPUT_KEY_APOSTROPHE		40
#define CSUDIINPUT_KEY_GRAVE		41
#define CSUDIINPUT_KEY_LEFTSHIFT		42
#define CSUDIINPUT_KEY_BACKSLASH		43
#define CSUDIINPUT_KEY_Z			44
#define CSUDIINPUT_KEY_X			45
#define CSUDIINPUT_KEY_C			46
#define CSUDIINPUT_KEY_V			47
#define CSUDIINPUT_KEY_B			48
#define CSUDIINPUT_KEY_N			49
#define CSUDIINPUT_KEY_M			50
#define CSUDIINPUT_KEY_COMMA		51
#define CSUDIINPUT_KEY_DOT			52
#define CSUDIINPUT_KEY_SLASH		53
#define CSUDIINPUT_KEY_RIGHTSHIFT		54
#define CSUDIINPUT_KEY_KPASTERISK		55
#define CSUDIINPUT_KEY_LEFTALT		56
#define CSUDIINPUT_KEY_SPACE		57
#define CSUDIINPUT_KEY_CAPSLOCK		58
#define CSUDIINPUT_KEY_F1			59
#define CSUDIINPUT_KEY_F2			60
#define CSUDIINPUT_KEY_F3			61
#define CSUDIINPUT_KEY_F4			62
#define CSUDIINPUT_KEY_F5			63
#define CSUDIINPUT_KEY_F6			64
#define CSUDIINPUT_KEY_F7			65
#define CSUDIINPUT_KEY_F8			66
#define CSUDIINPUT_KEY_F9			67
#define CSUDIINPUT_KEY_F10			68
#define CSUDIINPUT_KEY_NUMLOCK		69
#define CSUDIINPUT_KEY_SCROLLLOCK		70
#define CSUDIINPUT_KEY_KP7			71
#define CSUDIINPUT_KEY_KP8			72
#define CSUDIINPUT_KEY_KP9			73
#define CSUDIINPUT_KEY_KPMINUS		74
#define CSUDIINPUT_KEY_KP4			75
#define CSUDIINPUT_KEY_KP5			76
#define CSUDIINPUT_KEY_KP6			77
#define CSUDIINPUT_KEY_KPPLUS		78
#define CSUDIINPUT_KEY_KP1			79
#define CSUDIINPUT_KEY_KP2			80
#define CSUDIINPUT_KEY_KP3			81
#define CSUDIINPUT_KEY_KP0			82
#define CSUDIINPUT_KEY_KPDOT		83

#define CSUDIINPUT_KEY_ZENKAKUHANKAKU	85
#define CSUDIINPUT_KEY_102ND		86
#define CSUDIINPUT_KEY_F11			87
#define CSUDIINPUT_KEY_F12			88
#define CSUDIINPUT_KEY_RO			89
#define CSUDIINPUT_KEY_KATAKANA		90
#define CSUDIINPUT_KEY_HIRAGANA		91
#define CSUDIINPUT_KEY_HENKAN		92
#define CSUDIINPUT_KEY_KATAKANAHIRAGANA	93
#define CSUDIINPUT_KEY_MUHENKAN		94
#define CSUDIINPUT_KEY_KPJPCOMMA		95
#define CSUDIINPUT_KEY_KPENTER		96
#define CSUDIINPUT_KEY_RIGHTCTRL		97
#define CSUDIINPUT_KEY_KPSLASH		98
#define CSUDIINPUT_KEY_SYSRQ		99
#define CSUDIINPUT_KEY_RIGHTALT		100
#define CSUDIINPUT_KEY_LINEFEED		101
#define CSUDIINPUT_KEY_HOME		102
#define CSUDIINPUT_KEY_UP			103
#define CSUDIINPUT_KEY_PAGEUP		104
#define CSUDIINPUT_KEY_LEFT		105
#define CSUDIINPUT_KEY_RIGHT		106
#define CSUDIINPUT_KEY_END			107
#define CSUDIINPUT_KEY_DOWN		108
#define CSUDIINPUT_KEY_PAGEDOWN		109
#define CSUDIINPUT_KEY_INSERT		110
#define CSUDIINPUT_KEY_DELETE		111
#define CSUDIINPUT_KEY_MACRO		112
#define CSUDIINPUT_KEY_MUTE		113
#define CSUDIINPUT_KEY_VOLUMEDOWN		114
#define CSUDIINPUT_KEY_VOLUMEUP		115
#define CSUDIINPUT_KEY_POWER		116
#define CSUDIINPUT_KEY_KPEQUAL		117
#define CSUDIINPUT_KEY_KPPLUSMINUS		118
#define CSUDIINPUT_KEY_PAUSE		119

#define CSUDIINPUT_KEY_KPCOMMA		121
#define CSUDIINPUT_KEY_HANGUEL		122
#define CSUDIINPUT_KEY_HANJA		123
#define CSUDIINPUT_KEY_YEN			124
#define CSUDIINPUT_KEY_LEFTMETA		125
#define CSUDIINPUT_KEY_RIGHTMETA		126
#define CSUDIINPUT_KEY_COMPOSE		127

#define CSUDIINPUT_KEY_STOP		128
#define CSUDIINPUT_KEY_AGAIN		129
#define CSUDIINPUT_KEY_PROPS		130
#define CSUDIINPUT_KEY_UNDO		131
#define CSUDIINPUT_KEY_FRONT		132
#define CSUDIINPUT_KEY_COPY		133
#define CSUDIINPUT_KEY_OPEN		134
#define CSUDIINPUT_KEY_PASTE		135
#define CSUDIINPUT_KEY_FIND		136
#define CSUDIINPUT_KEY_CUT			137
#define CSUDIINPUT_KEY_HELP		138
#define CSUDIINPUT_KEY_MENU		139
#define CSUDIINPUT_KEY_CALC		140
#define CSUDIINPUT_KEY_SETUP		141
#define CSUDIINPUT_KEY_SLEEP		142
#define CSUDIINPUT_KEY_WAKEUP		143
#define CSUDIINPUT_KEY_FILE		144
#define CSUDIINPUT_KEY_SENDFILE		145
#define CSUDIINPUT_KEY_DELETEFILE		146
#define CSUDIINPUT_KEY_XFER		147
#define CSUDIINPUT_KEY_PROG1		148
#define CSUDIINPUT_KEY_PROG2		149
#define CSUDIINPUT_KEY_WWW			150
#define CSUDIINPUT_KEY_MSDOS		151
#define CSUDIINPUT_KEY_COFFEE		152
#define CSUDIINPUT_KEY_DIRECTION		153
#define CSUDIINPUT_KEY_CYCLEWINDOWS	154
#define CSUDIINPUT_KEY_MAIL		155
#define CSUDIINPUT_KEY_BOOKMARKS		156
#define CSUDIINPUT_KEY_COMPUTER		157
#define CSUDIINPUT_KEY_BACK		158
#define CSUDIINPUT_KEY_FORWARD		159
#define CSUDIINPUT_KEY_CLOSECD		160
#define CSUDIINPUT_KEY_EJECTCD		161
#define CSUDIINPUT_KEY_EJECTCLOSECD	162
#define CSUDIINPUT_KEY_NEXTSONG		163
#define CSUDIINPUT_KEY_PLAYPAUSE		164
#define CSUDIINPUT_KEY_PREVIOUSSONG	165
#define CSUDIINPUT_KEY_STOPCD		166
#define CSUDIINPUT_KEY_RECORD		167
#define CSUDIINPUT_KEY_REWIND		168
#define CSUDIINPUT_KEY_PHONE		169
#define CSUDIINPUT_KEY_ISO			170
#define CSUDIINPUT_KEY_CONFIG		171
#define CSUDIINPUT_KEY_HOMEPAGE		172
#define CSUDIINPUT_KEY_REFRESH		173
#define CSUDIINPUT_KEY_EXIT		174
#define CSUDIINPUT_KEY_MOVE		175
#define CSUDIINPUT_KEY_EDIT		176
#define CSUDIINPUT_KEY_SCROLLUP		177
#define CSUDIINPUT_KEY_SCROLLDOWN		178
#define CSUDIINPUT_KEY_KPLEFTPAREN		179
#define CSUDIINPUT_KEY_KPRIGHTPAREN	180

#define CSUDIINPUT_KEY_F13			183
#define CSUDIINPUT_KEY_F14			184
#define CSUDIINPUT_KEY_F15			185
#define CSUDIINPUT_KEY_F16			186
#define CSUDIINPUT_KEY_F17			187
#define CSUDIINPUT_KEY_F18			188
#define CSUDIINPUT_KEY_F19			189
#define CSUDIINPUT_KEY_F20			190
#define CSUDIINPUT_KEY_F21			191
#define CSUDIINPUT_KEY_F22			192
#define CSUDIINPUT_KEY_F23			193
#define CSUDIINPUT_KEY_F24			194

#define CSUDIINPUT_KEY_PLAYCD		200
#define CSUDIINPUT_KEY_PAUSECD		201
#define CSUDIINPUT_KEY_PROG3		202
#define CSUDIINPUT_KEY_PROG4		203
#define CSUDIINPUT_KEY_SUSPEND		205
#define CSUDIINPUT_KEY_CLOSE		206
#define CSUDIINPUT_KEY_PLAY		207
#define CSUDIINPUT_KEY_FASTFORWARD		208
#define CSUDIINPUT_KEY_BASSBOOST		209
#define CSUDIINPUT_KEY_PRINT		210
#define CSUDIINPUT_KEY_HP			211
#define CSUDIINPUT_KEY_CAMERA		212
#define CSUDIINPUT_KEY_SOUND		213
#define CSUDIINPUT_KEY_QUESTION		214
#define CSUDIINPUT_KEY_EMAIL		215
#define CSUDIINPUT_KEY_CHAT		216
#define CSUDIINPUT_KEY_SEARCH		217
#define CSUDIINPUT_KEY_CONNECT		218
#define CSUDIINPUT_KEY_FINANCE		219
#define CSUDIINPUT_KEY_SPORT		220
#define CSUDIINPUT_KEY_SHOP		221
#define CSUDIINPUT_KEY_ALTERASE		222
#define CSUDIINPUT_KEY_CANCEL		223
#define CSUDIINPUT_KEY_BRIGHTNESSDOWN	224
#define CSUDIINPUT_KEY_BRIGHTNESSUP	225
#define CSUDIINPUT_KEY_MEDIA		226

#define CSUDIINPUT_KEY_SWITCHVIDEOMODE	227
#define CSUDIINPUT_KEY_KBDILLUMTOGGLE	228
#define CSUDIINPUT_KEY_KBDILLUMDOWN	229
#define CSUDIINPUT_KEY_KBDILLUMUP		230

#define CSUDIINPUT_KEY_UNKNOWN		240

#define CSUDIINPUT_BTN_MISC		0x100
#define CSUDIINPUT_BTN_0			0x100
#define CSUDIINPUT_BTN_1			0x101
#define CSUDIINPUT_BTN_2			0x102
#define CSUDIINPUT_BTN_3			0x103
#define CSUDIINPUT_BTN_4			0x104
#define CSUDIINPUT_BTN_5			0x105
#define CSUDIINPUT_BTN_6			0x106
#define CSUDIINPUT_BTN_7			0x107
#define CSUDIINPUT_BTN_8			0x108
#define CSUDIINPUT_BTN_9			0x109

#define CSUDIINPUT_BTN_MOUSE		0x110
#define CSUDIINPUT_BTN_LEFT		0x110
#define CSUDIINPUT_BTN_RIGHT		0x111
#define CSUDIINPUT_BTN_MIDDLE		0x112
#define CSUDIINPUT_BTN_SIDE		0x113
#define CSUDIINPUT_BTN_EXTRA		0x114
#define CSUDIINPUT_BTN_FORWARD		0x115
#define CSUDIINPUT_BTN_BACK		0x116
#define CSUDIINPUT_BTN_TASK		0x117

#define CSUDIINPUT_BTN_JOYSTICK		0x120
#define CSUDIINPUT_BTN_TRIGGER		0x120
#define CSUDIINPUT_BTN_THUMB		0x121
#define CSUDIINPUT_BTN_THUMB2		0x122
#define CSUDIINPUT_BTN_TOP		0x123
#define CSUDIINPUT_BTN_TOP2		0x124
#define CSUDIINPUT_BTN_PINKIE		0x125
#define CSUDIINPUT_BTN_BASE		0x126
#define CSUDIINPUT_BTN_BASE2		0x127
#define CSUDIINPUT_BTN_BASE3		0x128
#define CSUDIINPUT_BTN_BASE4		0x129
#define CSUDIINPUT_BTN_BASE5		0x12a
#define CSUDIINPUT_BTN_BASE6		0x12b
#define CSUDIINPUT_BTN_DEAD		0x12f

#define CSUDIINPUT_BTN_GAMEPAD		0x130
#define CSUDIINPUT_BTN_A		0x130
#define CSUDIINPUT_BTN_B		0x131
#define CSUDIINPUT_BTN_C		0x132
#define CSUDIINPUT_BTN_X		0x133
#define CSUDIINPUT_BTN_Y		0x134
#define CSUDIINPUT_BTN_Z		0x135
#define CSUDIINPUT_BTN_TL		0x136
#define CSUDIINPUT_BTN_TR		0x137
#define CSUDIINPUT_BTN_TL2		0x138
#define CSUDIINPUT_BTN_TR2		0x139
#define CSUDIINPUT_BTN_SELECT		0x13a
#define CSUDIINPUT_BTN_START		0x13b
#define CSUDIINPUT_BTN_MODE		0x13c
#define CSUDIINPUT_BTN_THUMBL		0x13d
#define CSUDIINPUT_BTN_THUMBR		0x13e

#define CSUDIINPUT_BTN_DIGI		0x140
#define CSUDIINPUT_BTN_TOOL_PEN		0x140
#define CSUDIINPUT_BTN_TOOL_RUBBER	0x141
#define CSUDIINPUT_BTN_TOOL_BRUSH	0x142
#define CSUDIINPUT_BTN_TOOL_PENCIL	0x143
#define CSUDIINPUT_BTN_TOOL_AIRBRUSH	0x144
#define CSUDIINPUT_BTN_TOOL_FINGER	0x145
#define CSUDIINPUT_BTN_TOOL_MOUSE	0x146
#define CSUDIINPUT_BTN_TOOL_LENS	0x147
#define CSUDIINPUT_BTN_TOUCH		0x14a
#define CSUDIINPUT_BTN_STYLUS		0x14b
#define CSUDIINPUT_BTN_STYLUS2		0x14c
#define CSUDIINPUT_BTN_TOOL_DOUBLETAP	0x14d
#define CSUDIINPUT_BTN_TOOL_TRIPLETAP	0x14e

#define CSUDIINPUT_BTN_WHEEL		0x150
#define CSUDIINPUT_BTN_GEAR_DOWN	0x150
#define CSUDIINPUT_BTN_GEAR_UP		0x151

#define CSUDIINPUT_KEY_OK		0x160
#define CSUDIINPUT_KEY_SELECT 		0x161
#define CSUDIINPUT_KEY_GOTO		0x162
#define CSUDIINPUT_KEY_CLEAR		0x163
#define CSUDIINPUT_KEY_POWER2		0x164
#define CSUDIINPUT_KEY_OPTION		0x165
#define CSUDIINPUT_KEY_INFO		0x166
#define CSUDIINPUT_KEY_TIME		0x167
#define CSUDIINPUT_KEY_VENDOR		0x168
#define CSUDIINPUT_KEY_ARCHIVE		0x169
#define CSUDIINPUT_KEY_PROGRAM		0x16a
#define CSUDIINPUT_KEY_CHANNEL		0x16b
#define CSUDIINPUT_KEY_FAVORITES	0x16c
#define CSUDIINPUT_KEY_EPG		0x16d
#define CSUDIINPUT_KEY_PVR		0x16e
#define CSUDIINPUT_KEY_MHP		0x16f
#define CSUDIINPUT_KEY_LANGUAGE		0x170
#define CSUDIINPUT_KEY_TITLE		0x171
#define CSUDIINPUT_KEY_SUBTITLE		0x172
#define CSUDIINPUT_KEY_ANGLE		0x173
#define CSUDIINPUT_KEY_ZOOM		0x174
#define CSUDIINPUT_KEY_MODE		0x175
#define CSUDIINPUT_KEY_KEYBOARD		0x176
#define CSUDIINPUT_KEY_SCREEN		0x177
#define CSUDIINPUT_KEY_PC		0x178
#define CSUDIINPUT_KEY_TV		0x179
#define CSUDIINPUT_KEY_TV2		0x17a
#define CSUDIINPUT_KEY_VCR		0x17b
#define CSUDIINPUT_KEY_VCR2		0x17c
#define CSUDIINPUT_KEY_SAT		0x17d
#define CSUDIINPUT_KEY_SAT2		0x17e
#define CSUDIINPUT_KEY_CD		0x17f
#define CSUDIINPUT_KEY_TAPE		0x180
#define CSUDIINPUT_KEY_RADIO		0x181
#define CSUDIINPUT_KEY_TUNER		0x182
#define CSUDIINPUT_KEY_PLAYER		0x183
#define CSUDIINPUT_KEY_TEXT		0x184
#define CSUDIINPUT_KEY_DVD		0x185
#define CSUDIINPUT_KEY_AUX		0x186
#define CSUDIINPUT_KEY_MP3		0x187
#define CSUDIINPUT_KEY_AUDIO		0x188
#define CSUDIINPUT_KEY_VIDEO		0x189
#define CSUDIINPUT_KEY_DIRECTORY	0x18a
#define CSUDIINPUT_KEY_LIST		0x18b
#define CSUDIINPUT_KEY_MEMO		0x18c
#define CSUDIINPUT_KEY_CALENDAR		0x18d
#define CSUDIINPUT_KEY_RED		0x18e
#define CSUDIINPUT_KEY_GREEN		0x18f
#define CSUDIINPUT_KEY_YELLOW		0x190
#define CSUDIINPUT_KEY_BLUE		0x191
#define CSUDIINPUT_KEY_CHANNELUP	0x192
#define CSUDIINPUT_KEY_CHANNELDOWN	0x193
#define CSUDIINPUT_KEY_FIRST		0x194
#define CSUDIINPUT_KEY_LAST		0x195
#define CSUDIINPUT_KEY_AB		0x196
#define CSUDIINPUT_KEY_NEXT		0x197
#define CSUDIINPUT_KEY_RESTART		0x198
#define CSUDIINPUT_KEY_SLOW		0x199
#define CSUDIINPUT_KEY_SHUFFLE		0x19a
#define CSUDIINPUT_KEY_BREAK		0x19b
#define CSUDIINPUT_KEY_PREVIOUS		0x19c
#define CSUDIINPUT_KEY_DIGITS		0x19d
#define CSUDIINPUT_KEY_TEEN		0x19e
#define CSUDIINPUT_KEY_TWEN		0x19f

#define CSUDIINPUT_KEY_DEL_EOL		0x1c0
#define CSUDIINPUT_KEY_DEL_EOS		0x1c1
#define CSUDIINPUT_KEY_INS_LINE		0x1c2
#define CSUDIINPUT_KEY_DEL_LINE		0x1c3

#define CSUDIINPUT_KEY_FN		0x1d0
#define CSUDIINPUT_KEY_FN_ESC		0x1d1
#define CSUDIINPUT_KEY_FN_F1		0x1d2
#define CSUDIINPUT_KEY_FN_F2		0x1d3
#define CSUDIINPUT_KEY_FN_F3		0x1d4
#define CSUDIINPUT_KEY_FN_F4		0x1d5
#define CSUDIINPUT_KEY_FN_F5		0x1d6
#define CSUDIINPUT_KEY_FN_F6		0x1d7
#define CSUDIINPUT_KEY_FN_F7		0x1d8
#define CSUDIINPUT_KEY_FN_F8		0x1d9
#define CSUDIINPUT_KEY_FN_F9		0x1da
#define CSUDIINPUT_KEY_FN_F10		0x1db
#define CSUDIINPUT_KEY_FN_F11		0x1dc
#define CSUDIINPUT_KEY_FN_F12		0x1dd
#define CSUDIINPUT_KEY_FN_1		0x1de
#define CSUDIINPUT_KEY_FN_2		0x1df
#define CSUDIINPUT_KEY_FN_D		0x1e0
#define CSUDIINPUT_KEY_FN_E		0x1e1
#define CSUDIINPUT_KEY_FN_F		0x1e2
#define CSUDIINPUT_KEY_FN_S		0x1e3
#define CSUDIINPUT_KEY_FN_B		0x1e4

#define CSUDIINPUT_KEY_MAX		0x1ff


 /**@brief mouse keypad joystick Relative axes 定义  ，对应  CSUDIInputEvent_S.m_uCode*/
#define CSUDIINPUT_REL_X		0x00
#define CSUDIINPUT_REL_Y		0x01
#define CSUDIINPUT_REL_Z		0x02
#define CSUDIINPUT_REL_RX		0x03
#define CSUDIINPUT_REL_RY		0x04
#define CSUDIINPUT_REL_RZ		0x05
#define CSUDIINPUT_REL_HWHEEL		0x06
#define CSUDIINPUT_REL_DIAL		0x07
#define CSUDIINPUT_REL_WHEEL		0x08
#define CSUDIINPUT_REL_MISC		0x09
#define CSUDIINPUT_REL_MAX		0x0f

 /**@brief mouse keypad joystick Absolute axes 定义 ，对应  CSUDIInputEvent_S.m_uCode*/
#define CSUDIINPUT_ABS_X		0x00
#define CSUDIINPUT_ABS_Y		0x01
#define CSUDIINPUT_ABS_Z		0x02
#define CSUDIINPUT_ABS_RX		0x03
#define CSUDIINPUT_ABS_RY		0x04
#define CSUDIINPUT_ABS_RZ		0x05
#define CSUDIINPUT_ABS_THROTTLE		0x06
#define CSUDIINPUT_ABS_RUDDER		0x07
#define CSUDIINPUT_ABS_WHEEL		0x08
#define CSUDIINPUT_ABS_GAS		0x09
#define CSUDIINPUT_ABS_BRAKE		0x0a
#define CSUDIINPUT_ABS_HAT0X		0x10
#define CSUDIINPUT_ABS_HAT0Y		0x11
#define CSUDIINPUT_ABS_HAT1X		0x12
#define CSUDIINPUT_ABS_HAT1Y		0x13
#define CSUDIINPUT_ABS_HAT2X		0x14
#define CSUDIINPUT_ABS_HAT2Y		0x15
#define CSUDIINPUT_ABS_HAT3X		0x16
#define CSUDIINPUT_ABS_HAT3Y		0x17
#define CSUDIINPUT_ABS_PRESSURE		0x18
#define CSUDIINPUT_ABS_DISTANCE		0x19
#define CSUDIINPUT_ABS_TILT_X		0x1a
#define CSUDIINPUT_ABS_TILT_Y		0x1b
#define CSUDIINPUT_ABS_TOOL_WIDTH	0x1c
#define CSUDIINPUT_ABS_VOLUME		0x20
#define CSUDIINPUT_ABS_MISC		0x28
#define CSUDIINPUT_ABS_MAX		0x3f


/**@brief mouse keypad joystick Misc events定义 ，对应  CSUDIInputEvent_S.m_uCode*/
#define CSUDIINPUT_MSC_SERIAL		0x00
#define CSUDIINPUT_MSC_PULSELED		0x01
#define CSUDIINPUT_MSC_GESTURE		0x02
#define CSUDIINPUT_MSC_RAW		0x03
#define CSUDIINPUT_MSC_SCAN		0x04
#define CSUDIINPUT_MSC_MAX		0x07

   
 /**@brief mouse keypad joystick LEDs定义，对应  CSUDIInputEvent_S.m_uCode */
#define CSUDIINPUT_LED_NUML		0x00
#define CSUDIINPUT_LED_CAPSL		0x01
#define CSUDIINPUT_LED_SCROLLL		0x02
#define CSUDIINPUT_LED_COMPOSE		0x03
#define CSUDIINPUT_LED_KANA		0x04
#define CSUDIINPUT_LED_SLEEP		0x05
#define CSUDIINPUT_LED_SUSPEND		0x06
#define CSUDIINPUT_LED_MUTE		0x07
#define CSUDIINPUT_LED_MISC		0x08
#define CSUDIINPUT_LED_MAIL		0x09
#define CSUDIINPUT_LED_CHARGING		0x0a
#define CSUDIINPUT_LED_MAX		0x0f

 /**@brief mouse keypad joystick Autorepeat values定义，对应  CSUDIInputEvent_S.m_uCode */
#define CSUDIINPUT_REP_DELAY		0x00
#define CSUDIINPUT_REP_PERIOD		0x01
#define CSUDIINPUT_REP_MAX		0x01

/**@brief mouse keypad joystick Sounds定义，对应  CSUDIInputEvent_S.m_uCode */
#define CSUDIINPUT_SND_CLICK		0x00
#define CSUDIINPUT_SND_BELL		0x01
#define CSUDIINPUT_SND_TONE		0x02
#define CSUDIINPUT_SND_MAX		0x07


 /**@brief 输入设备LED灯状态枚举定义 */
 typedef enum
 {
     EM_UDIINPUT_LED_OFF=0x00,  ///< LED 灭
     EM_UDIINPUT_LED_ON=0x01   ///< LED 亮
 } CSUDIInputLEDStatus_E ;

 /**@brief 键盘鼠标游戏手柄时间定义 */
 typedef struct 
 {
     unsigned long    tv_sec;
     unsigned long    tv_usec;
 }CSUDIInputTimeval_S;

 /**@brief 键盘鼠标游戏手柄数据类型定义 */
typedef struct 
{
    unsigned int m_dwDeviceIndex;   ///<事件发生的设备编号，用于区分同一类型的多个设备
    CSUDIInputTimeval_S m_stTime;      ///<事件发生的时间
    unsigned short m_uEeventType;      ///< 事件类型，对应与CSUDIInputEventType_E
    unsigned short m_uCode;      ///< 事件对应的数据，根据m_uEeventType的值，对应与CSUDIINPUT_SYN_*,
                                                //CSUDIINPUT_KEY_*,CSUDIINPUT_REL_*,CSUDIINPUT_ABS_*,
                                                //CSUDIINPUT_MSC_*,CSUDIINPUT_LED_*,CSUDIINPUT_REP_*和CSUDIINPUT_SND_*
    signed int m_nValue;///<事件携带的状态数据. 根据m_uEeventType的不同而有所差异，
                                    //对于EM_UDIINPUT_EV_SYN ,reserved
                                    //对于EM_UDIINPUT_EV_KEY,0 for KEY release, 1 for keypress and 2 for autorepeat.
                                    //对于EM_UDIINPUT_EV_REL,相对 偏移值
                                    //对于EM_UDIINPUT_EV_ABS,绝对位置 
                                    //对于EM_UDIINPUT_EV_LED，led灯状态，对应CSUDIInputLEDStatus_E
}CSUDIInputEvent_S ;

/**@brief 普通字节流数据类型定义 */
typedef struct
{
    unsigned char*    m_pucData;        ///< 数据指针,具体数据格式参考第三方规范文档
    int               m_nDataLen;             ///< 数据长度
}CSUDIGeneralCodesInputData_S;

/**@brief 输入信息结果类型 */
typedef struct  
{
	CSUDIInputType_E 	m_eType;		///<  输入设备来源，详见CSUDIInputType_E
	CSUDIInputKeyStatus_E 		m_eStatus;		///< 输入按键状态,详见CSUDIInputKeyStatus_E
	int			m_nKeyCode;		///< 输入键码,仅m_eType为EM_UDIINPUT_FRONTPANEL或EM_UDIINPUT_REMOTECTRL时关注此值，其他情况请置为0
	void*       m_ExtInputData;   ///<扩展数据，根据m_eType的不同而类型不同。目前是在刷卡遥控器时转换为CSUDIGeneralCodesInputData_S*类型，
	                                            //当m_eType为EM_UDIINPUT_KEYBOARD,   EM_UDIINPUT_MOUSE,   EM_UDIINPUT_JOYSTICK   时转换为CSUDIInputEvent_S*类型
							  //不排除以后会有其它新类型定义.当未用到此数据时，请置为NULL
}CSUDIInputInfo_S;	

/**@brief 输入设备回调函数定义

@param[in] psInputInfo 详见CSUDIInputInfo_S说明
@param[in] pvUserData 用户数据
*/
typedef  void ( *CSUDIInputCallback_F )(CSUDIInputInfo_S * psInputInfo ,void * pvUserData);

/**
@brief 注册输入设备按键回调函数

任何输入设备产生有效的按键信息后，通过注册的回调函数通知应用层\n
@param[in] fnInputCallback 指向回调函数的函数指针，用户通过回调函数得到按键来源、状态和功能码
@param[in] pvUserData 用户数据，可以为CSUDI_NULL
@return 注册成功则返回CSUDI_SUCCESS ;注册失败则返回相关错误代码
@note 
- 支持注册的回调个数至少需支持8个，最大不限
- fnInputCallback和pvUserData唯一标识一个回调类型，当两次注册它们完全一样时，第二次将返回CSUDIINPUT_ERROR_ALREADY_ADDED错误

@note Booter版回调函数可以在中断中调用，上层会按中断使用原则使用回调
*/
CSUDI_Error_Code CSUDIINPUTAddCallback( CSUDIInputCallback_F fnInputCallback ,void * pvUserData);


/**
@brief 删除输入设备按键回调函数

@param[in] fnInputCallback 指向回调函数的函数指针
@param[in] pvUserData 用户数据
@return 删除成功则返回CSUDI_SUCCESS;删除失败则返回相关错误代码
@note 如果要删除一个不存在的回调，则返回CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST
@note fnInputCallback和pvUserData唯一标识一个回调类型

@note Booter版回调函数可以在中断中调用，上层会按中断使用原则使用回调
*/
CSUDI_Error_Code CSUDIINPUTDelCallback(  CSUDIInputCallback_F fnInputCallback ,void * pvUserData);


#ifdef __cplusplus
}
#endif

/** @} */

#endif   

