#ifndef _GRAINPUT_H_
#define _GRAINPUT_H_

#define GRABUTTON_L	04
#define GRABUTTON_M	02
#define GRABUTTON_R	01

typedef unsigned int    GRAKEYMOD;
typedef unsigned short  GRAKEY;
typedef unsigned char   GRASCANCODE;

#define GRAKEY_MAX			CSUDIINPUT_KEY_MAX

#define GRAKEY_UNKNOWN		0
#define GRA_CHAR_UNDEFINED	0xFFFE
#include "cs_input.h"

// [[
/**@ 以下定义表示是否有功能键触发和触发类型（按下还是松开触发）*/
#define KEY_SHIFT					0x0001
#define KEY_CTRL					0x0002
#define KEY_ALT						0x0004

/**@brief 以下定义鼠标事件类型*/
#define MOUSE_LEFT					0x0001
#define MOUSE_MIDDLE				0x0002
#define MOUSE_RIGHT				0x0004
#define MOUSE_PRESS				0x0100
#define MOUSE_RELEASE				0x0200
#define MOUSE_MOVE					0x0400
#define MOUSE_WHEEL			0x0800
// ]] end 

#define GRAKEY_NONASCII_MASK	0xFF00

/**@brief 以下定义键盘事件键值*/
/***************正常键值************/
#define GRAKEY_GRAVE			'`'
#define GRAKEY_1					'1'
#define GRAKEY_2					'2'
#define GRAKEY_3					'3'
#define GRAKEY_4					'4'
#define GRAKEY_5					'5'
#define GRAKEY_6					'6'
#define GRAKEY_7					'7'
#define GRAKEY_8					'8'
#define GRAKEY_9					'9'
#define GRAKEY_0					'0'
#define GRAKEY_MINUS			'-'
#define GRAKEY_EQUAL			'='

#define GRAKEY_q					'q'
#define GRAKEY_w				'w'
#define GRAKEY_e					'e'
#define GRAKEY_r					'r'
#define GRAKEY_t					't'
#define GRAKEY_y					'y'
#define GRAKEY_u					'u'
#define GRAKEY_i					'i'
#define GRAKEY_o					'o'
#define GRAKEY_p					'p'
#define GRAKEY_LEFTBRACKET		'['
#define GRAKEY_RIGHTBRACKET	']'
#define GRAKEY_ENTER			'\r'

#define GRAKEY_a					'a'
#define GRAKEY_s					's'
#define GRAKEY_d					'd'
#define GRAKEY_f					'f'
#define GRAKEY_g					'g'
#define GRAKEY_h					'h'
#define GRAKEY_j					'j'
#define GRAKEY_k					'k'
#define GRAKEY_l					'l'
#define GRAKEY_SEMICOLON		';'
#define GRAKEY_APOSTROPHE		'\''
#define GRAKEY_BACKSLASH		'\\'

#define GRAKEY_z					'z'
#define GRAKEY_x					'x'
#define GRAKEY_c					'c'
#define GRAKEY_v					'v'
#define GRAKEY_b					'b'
#define GRAKEY_n					'n'
#define GRAKEY_m				'm'
#define GRAKEY_COMMA			','
#define GRAKEY_PERIOD			'.'
#define GRAKEY_SLASH			'/'

#define GRAKEY_SPACE			0x0020

#define	GRAKEY_TAB			    0x0009
#define	GRAKEY_SHIFT	        0x0010
#define	GRAKEY_CONTROL	        0x0011
#define	GRAKEY_ALT	            0x0012
#define	GRAKEY_CAPS_LOCK	    0x0014
#define	GRAKEY_ESCAPE	        0x001B

#define GRAKEY_LEFT             0x0025
#define GRAKEY_UP               0x0026          
#define GRAKEY_RIGHT            0x0027
#define GRAKEY_DOWN             0x0028
#define GRAKEY_BACKSPACE        0x0280

/***************shift 状态下键值************/
#define GRAKEY_TILDE			'~'
#define GRAKEY_EXCLAM			'!'
#define GRAKEY_AT				'@'
#define GRAKEY_NUMBERSIGN		'#'
#define GRAKEY_DOLLAR			'$'
#define GRAKEY_PERCENT			'%'
#define GRAKEY_CARET			'^'
#define GRAKEY_AMPERSAND		'&'
#define GRAKEY_ASTERISK			'*'
#define GRAKEY_LEFTPARENTHESIS		'('
#define GRAKEY_RIGHTPARENTHESIS	')'
#define GRAKEY_UNDERSCORE		'_'
#define GRAKEY_PLUS				'+'

#define GRAKEY_Q				'Q'
#define GRAKEY_W				'W'
#define GRAKEY_E					'E'
#define GRAKEY_R					'R'
#define GRAKEY_T					'T'
#define GRAKEY_Y					'Y'
#define GRAKEY_U					'U'
#define GRAKEY_I					'I'
#define GRAKEY_P					'P'
#define GRAKEY_O				'O'
#define GRAKEY_LEFTBRACE		'{'
#define GRAKEY_RIGHTBRACE		'}'

#define GRAKEY_A					'A'
#define GRAKEY_S					'S'
#define GRAKEY_D					'D'
#define GRAKEY_F					'F'
#define GRAKEY_G					'G'
#define GRAKEY_H					'H'
#define GRAKEY_J					'J'
#define GRAKEY_K					'K'
#define GRAKEY_L					'L'
#define GRAKEY_COLON			':'
#define GRAKEY_QUOTEDBL		'\"'
#define GRAKEY_VERTICALBAR		'|'

#define GRAKEY_Z					'Z'
#define GRAKEY_X					'X'
#define GRAKEY_C					'C'
#define GRAKEY_V					'V'
#define GRAKEY_B					'B'
#define GRAKEY_N					'N'
#define GRAKEY_M				'M'
#define GRAKEY_LESS				'<'
#define GRAKEY_GREATER			'>'
#define GRAKEY_QUESTION		'?'
/********************************************/
/**@brief 以下定义小键盘事件键值*/
#define GRAKEY_KP_DIVIDE 		GRAKEY_SLASH
#define GRAKEY_KP_MULTIPLY		GRAKEY_ASTERISK
#define GRAKEY_KP_MINUS		GRAKEY_MINUS

#define GRAKEY_KP_7				GRAKEY_7
#define GRAKEY_KP_8				GRAKEY_8
#define GRAKEY_KP_9				GRAKEY_9
#define GRAKEY_KP_PLUS			GRAKEY_PLUS

#define GRAKEY_KP_4				GRAKEY_4
#define GRAKEY_KP_5				GRAKEY_5
#define GRAKEY_KP_6				GRAKEY_6

#define GRAKEY_KP_1				GRAKEY_1
#define GRAKEY_KP_2				GRAKEY_2
#define GRAKEY_KP_3				GRAKEY_3
#define GRAKEY_KP_ENTER			GRAKEY_ENTER

#define GRAKEY_KP_0				GRAKEY_0
#define GRAKEY_KP_PERIOD		GRAKEY_PERIOD

/*********************************************************/
#define	GRA_VK_UNDEFINED	GRAKEY_UNKNOWN

#define	GRA_VK_BACK_SPACE	0x0080
#define	GRA_VK_TAB			0x0009
#define	GRA_VK_ENTER	0x000D
#define	GRA_VK_CLEAR	0x000C
#define	GRA_VK_SHIFT	0x0010
#define	GRA_VK_CONTROL	0x0011
#define	GRA_VK_ALT	0x0012
#define	GRA_VK_PAUSE	0x0013
#define	GRA_VK_CAPS_LOCK	0x0014
#define	GRA_VK_ESCAPE	0x001B
#define	GRA_VK_SPACE	0x0020
#define	GRA_VK_PAGE_UP	0x0021
#define	GRA_VK_PAGE_DOWN	0x0022
#define	GRA_VK_END	0x0023
#define	GRA_VK_HOME	0x0024
#define	GRA_VK_LEFT	0x0025
#define	GRA_VK_UP	0x0026
#define	GRA_VK_RIGHT	0x0027
#define	GRA_VK_DOWN	0x0028
#define	GRA_VK_COMMA	0x002C
#define	GRA_VK_MINUS	0x002D
#define	GRA_VK_PERIOD	0x002E
#define	GRA_VK_SLASH	0x002F
#define	GRA_VK_0	0x0030
#define	GRA_VK_1	0x0031
#define	GRA_VK_2	0x0032
#define	GRA_VK_3	0x0033
#define	GRA_VK_4	0x0034
#define	GRA_VK_5	0x0035
#define	GRA_VK_6	0x0036
#define	GRA_VK_7	0x0037
#define	GRA_VK_8	0x0038
#define	GRA_VK_9	0x0039
#define	GRA_VK_SEMICOLON	0x003B
#define	GRA_VK_EQUALS	0x003D
#define	GRA_VK_A	0x0041
#define	GRA_VK_B	0x0042
#define	GRA_VK_C	0x0043
#define	GRA_VK_D	0x0044
#define	GRA_VK_E	0x0045
#define	GRA_VK_F	0x0046
#define	GRA_VK_G	0x0047
#define	GRA_VK_H	0x0048
#define	GRA_VK_I	0x0049
#define	GRA_VK_J	0x004A
#define	GRA_VK_K	0x004B
#define	GRA_VK_L	0x004C
#define	GRA_VK_M	0x004D
#define	GRA_VK_N	0x004E
#define	GRA_VK_O	0x004F
#define	GRA_VK_P	0x0050
#define	GRA_VK_Q	0x0051
#define	GRA_VK_R	0x0052
#define	GRA_VK_S	0x0053
#define	GRA_VK_T	0x0054
#define	GRA_VK_U	0x0055
#define	GRA_VK_V	0x0056
#define	GRA_VK_W	0x0057
#define	GRA_VK_X	0x0058
#define	GRA_VK_Y	0x0059
#define	GRA_VK_Z	0x005A
#define	GRA_VK_OPEN_BRACKET	0x005B
#define	GRA_VK_BACK_SLASH	0x005C
#define	GRA_VK_CLOSE_BRACKET	0x005D
#define	GRA_VK_NUMPAD0	0x0060
#define	GRA_VK_NUMPAD1	0x0061
#define	GRA_VK_NUMPAD2	0x0062
#define	GRA_VK_NUMPAD3	0x0063
#define	GRA_VK_NUMPAD4	0x0064
#define	GRA_VK_NUMPAD5	0x0065
#define	GRA_VK_NUMPAD6	0x0066
#define	GRA_VK_NUMPAD7	0x0067
#define	GRA_VK_NUMPAD8	0x0068
#define	GRA_VK_NUMPAD9	0x0069
#define	GRA_VK_MULTIPLY	0x006A
#define	GRA_VK_ADD	0x006B
#define	GRA_VK_SEPARATER	0x006C
#define	GRA_VK_SUBTRACT	0x006D
#define	GRA_VK_DECIMAL	0x006E
#define	GRA_VK_DIVIDE	0x006F
#define	GRA_VK_F1	0x0070
#define	GRA_VK_F2	0x0071
#define	GRA_VK_F3	0x0072
#define	GRA_VK_F4	0x0073
#define	GRA_VK_F5	0x0074
#define	GRA_VK_F6	0x0075
#define	GRA_VK_F7	0x0076
#define	GRA_VK_F8	0x0077
#define	GRA_VK_F9	0x0078
#define	GRA_VK_F10	0x0079
#define	GRA_VK_F11	0x007A
#define	GRA_VK_F12	0x007B
#define	GRA_VK_DELETE	0x007F
#define	GRA_VK_DEAD_GRAVE	0x0080
#define	GRA_VK_DEAD_ACUTE	0x0081
#define	GRA_VK_DEAD_CIRCUMFLEX	0x0082
#define	GRA_VK_DEAD_TILDE	0x0083
#define	GRA_VK_DEAD_MACRON	0x0084
#define	GRA_VK_DEAD_BREVE	0x0085
#define	GRA_VK_DEAD_ABOVEDOT	0x0086
#define	GRA_VK_DEAD_DIAERESIS	0x0087
#define	GRA_VK_DEAD_ABOVERING	0x0088
#define	GRA_VK_DEAD_DOUBLEACUTE	0x0089
#define	GRA_VK_DEAD_CARON	0x008A
#define	GRA_VK_DEAD_CEDILLA	0x008B
#define	GRA_VK_DEAD_OGONEK	0x008C
#define	GRA_VK_DEAD_IOTA	0x008D
#define	GRA_VK_DEAD_VOICED_SOUND	0x008E
#define	GRA_VK_DEAD_SEMIVOICED_SOUND	0x008F
#define	GRA_VK_NUM_LOCK	0x0090
#define	GRA_VK_SCROLL_LOCK	0x0091
#define	GRA_VK_AMPERSAND	0x0096
#define	GRA_VK_ASTERISK	0x0097
#define	GRA_VK_QUOTEDBL	0x0098
#define	GRA_VK_LESS	0x0099
#define	GRA_VK_PRINTSCREEN	0x009A
#define	GRA_VK_INSERT	0x009B
#define	GRA_VK_HELP	0x009C
#define	GRA_VK_META	0x009D
#define	GRA_VK_GREATER	0x00A0
#define	GRA_VK_BRACELEFT	0x00A1
#define	GRA_VK_BRACERIGHT	0x00A2
#define	GRA_VK_BACK_QUOTE	0x00C0
#define	GRA_VK_QUOTE	0x00DE
#define	GRA_VK_KP_UP	0x00E0
#define	GRA_VK_KP_DOWN	0x00E1
#define	GRA_VK_KP_LEFT	0x00E2
#define	GRA_VK_KP_RIGHT	0x00E3
#define	GRA_VK_ALPHANUMERIC	0x00F0
#define	GRA_VK_AT	0x0200
#define	GRA_VK_COLON	0x0201
#define	GRA_VK_CIRCUMFLEX	0x0202
#define	GRA_VK_DOLLAR	0x0203
#define	GRA_VK_EURO_SIGN	0x0204
#define	GRA_VK_EXCLAMATION_MARK	0x0205
#define	GRA_VK_INVERTED_EXCLAMATION_MARK	0x0206
#define	GRA_VK_LEFT_PARENTHESIS	0x0207
#define	GRA_VK_NUMBER_SIGN	0x0208
#define	GRA_VK_PLUS	0x0209
#define	GRA_VK_RIGHT_PARENTHESIS	0x020A
#define	GRA_VK_UNDERSCORE	0x020B
#define	GRA_VK_WINDOWS	0x020C
#define	GRA_VK_CONTEXT_MENU	0x020D
#define	GRA_VK_COMPOSE	0xFF20
#define	GRA_VK_BEGIN	0xFF58
#define	GRA_VK_ALT_GRAPH	0xFF7E

/*********************************************************/
/**@brief 以下定义遥控器按键事件键值*/
/* Handheld function keys*/
#define GRAKEY_RC_RECORD			CSUDI_VK_RECORD
#define GRAKEY_RC_PLAY				CSUDI_VK_PLAY
#define GRAKEY_RC_STOP				CSUDI_VK_STOP
#define GRAKEY_RC_PAUSE			CSUDI_VK_PAUSE
#define GRAKEY_RC_RW				CSUDI_VK_RW
#define GRAKEY_RC_FF				CSUDI_VK_FF
#define GRAKEY_RC_VOD				CSUDI_VK_VOD
#define GRAKEY_RC_FAV				CSUDI_VK_FAV
#define GRAKEY_RC_SERV				CSUDI_VK_SERV
#define GRAKEY_RC_SYSSET			CSUDI_VK_SYSSET
#define GRAKEY_RC_HELP				CSUDI_VK_HELP

/* RC function keys */
#define GRAKEY_RC_CHANNEL_UP		CSUDI_VK_CHANNEL_UP
#define GRAKEY_RC_CHANNEL_DOWN	CSUDI_VK_CHANNEL_DOWN
#define GRAKEY_RC_VOLUME_UP		CSUDI_VK_VOLUME_UP
#define GRAKEY_RC_VOLUME_DOWN	CSUDI_VK_VOLUME_DOWN
#define GRAKEY_RC_TV_RADIO			CSUDI_VK_TV_RADIO

#define GRAKEY_RC_OK				CSUDI_VK_OK
#define GRAKEY_RC_CANCEL			CSUDI_VK_TOGGLE
#define GRAKEY_RC_INFO				CSUDI_VK_INFO
#define GRAKEY_RC_MENU				CSUDI_VK_MENU
#define GRAKEY_RC_EXIT				CSUDI_VK_EXIT

#define GRAKEY_RC_REFRESH			CSUDI_VK_REFRESH		 /* Refresh the Page */
#define GRAKEY_RC_FORWARD			CSUDI_VK_FORWARD		/* Forward of History */
#define GRAKEY_RC_BACK				CSUDI_VK_BACK		/* Back of History */
#define GRAKEY_RC_INPUT				CSUDI_VK_INPUT		/* Boot the T9 input module */

#define GRAKEY_RC_MUTE				CSUDI_VK_MUTE
#define GRAKEY_RC_GUIDE				CSUDI_VK_GUIDE
#define GRAKEY_RC_AUDIO			CSUDI_VK_AUDIO

#define GRAKEY_RC_COLORED_KEY_0	CSUDI_VK_COLORED_KEY_0
#define GRAKEY_RC_COLORED_KEY_1	CSUDI_VK_COLORED_KEY_1
#define GRAKEY_RC_COLORED_KEY_2	CSUDI_VK_COLORED_KEY_2
#define GRAKEY_RC_COLORED_KEY_3	CSUDI_VK_COLORED_KEY_3

/****************************************************************/
/* Keyboard state modifiers*/
#define GRAKMOD_NONE  		0x0000
#define GRAKMOD_LSHIFT		0x0001
#define GRAKMOD_RSHIFT		0x0002
#define GRAKMOD_LCTRL 		0x0040
#define GRAKMOD_RCTRL 		0x0080
#define GRAKMOD_LALT  		0x0100
#define GRAKMOD_RALT  		0x0200
#define GRAKMOD_LMETA 		0x0400		/* Windows key*/
#define GRAKMOD_RMETA 		0x0800		/* Windows key*/
#define GRAKMOD_NUM   		0x1000
#define GRAKMOD_CAPS  		0x2000
#define GRAKMOD_ALTGR 		0x4000
#define GRAKMOD_SCR		0x8000

#define GRAKMOD_CTRL	(GRAKMOD_LCTRL|GRAKMOD_RCTRL)
#define GRAKMOD_SHIFT	(GRAKMOD_LSHIFT|GRAKMOD_RSHIFT)
#define GRAKMOD_ALT	(GRAKMOD_LALT|GRAKMOD_RALT)
#define GRAKMOD_META	(GRAKMOD_LMETA|GRAKMOD_RMETA)

#define GRAKINFO_LED_MASK	(1 << 0)
#define GRAKINFO_LED_MODE_MASK	(1 << 1)

#define GRAKINFO_LED_CAP		(1 << 0)
#define GRAKINFO_LED_NUM		(1 << 1)
#define GRAKINFO_LED_SCR		(1 << 2)

#define GRAKINFO_LED_MODE_ON	(1 << 3)
#define GRAKINFO_LED_MODE_OFF	(1 << 4)

#endif

