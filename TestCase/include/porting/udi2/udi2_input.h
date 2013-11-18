/**@defgroup INPUT INPUT �ṩ�����豸�����ӿ�
@brief INPUTģ���Ӧͷ�ļ�<udi2_input.h>�����������������豸�Ŀ��ƽӿڡ��ֶԸ�ģ��
��������˵����

- �����ĸ�ģ��İ�����ң������ǰ���ȣ���ͨ����ģ��֪ͨ��Eastwin��
- ң������ʵ�ʰ����빦�ܼ��Ķ�Ӧ��ϵ������Ŀ�Ĳ�ͬ����Ӫ�̡�COSHIP��ͬȷ�������磺
ĳ��Ŀ��Ҫ�ڰ�F1��ʱ�������뷨������Ҫ���յ�F1��ʱ����CS_VK_INPUT��ֵ��
- ��Ҫ�����а�����֧��EM_UDIINPUT_KEY_REPEAT��Ϣ��������Щ����Ҫ��֧�ָ�����Ŀ�Ĳ�ͬ����Ӫ�̡�COSHIP��ͬȷ����
Ҫ��֧��EM_UDIINPUT_KEY_REPEAT��Ϣ�İ����ڱ���ʱ�䰴ס����ʱ��Ҫ����ʱ����EM_UDIINPUT_KEY_PRESS��Ϣ�����ſ������г�������EM_UDIINPUT_KEY_REPEAT��Ϣ���ſ�ʱ����EM_UDIINPUT_KEY_RELEASE��Ϣ
��Ҫ��֧��EM_UDIINPUT_KEY_REPEAT��Ϣ�İ����ڱ���ʱ�䰴ס����ʱ��Ҫ����ʱ����EM_UDIINPUT_KEY_PRESS��Ϣ���ſ�ʱ����EM_UDIINPUT_KEY_RELEASE��Ϣ��
- ��ģ��δ����ļ�ֵһ���͸����Ӧ�ò㣬��JavaScript����JavaӦ���С���Eastwin�ڲ�
���в��ְ������߼����Ӧ����CSUDI_VK_OK��Ӧ���볬���ӵȣ���������ȫ͸�������Բ���
����Eastwin����ֻ��͸���������иı���Щ��ֵ��

@version 2.0.2 2009/8/25 ��������˳���İ汾
@version 2.0.1 2009/8/24 ��һ���ۺ�������޸ĵİ汾
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

//ң������ǰ��尴��
#define CSUDI_VK_POWER					0xffff ///< ������
#define CSUDI_VK_0						0x30	///< ���ּ�0
#define CSUDI_VK_1						0x31	///< ���ּ�1
#define CSUDI_VK_2						0x32	///< ���ּ�2
#define CSUDI_VK_3						0x33	///< ���ּ�3
#define CSUDI_VK_4						0x34	///< ���ּ�4
#define CSUDI_VK_5						0x35	///< ���ּ�5
#define CSUDI_VK_6						0x36	///< ���ּ�6
#define CSUDI_VK_7						0x37	///< ���ּ�7
#define CSUDI_VK_8						0x38	///< ���ּ�8
#define CSUDI_VK_9						0x39	///< ���ּ�9
#define CSUDI_VK_OK			       	 	0x0D   ///< ȷ�ϼ�
#define CSUDI_VK_TOGGLE                	0x0280 ///< ���ؼ�
#define CSUDI_VK_EXIT					0x1B   ///< �˳���
#define CSUDI_VK_PAGE_UP				0x21   ///< ���Ϸ�ҳ
#define CSUDI_VK_PAGE_DOWN			    0x22   ///< ���·�ҳ
#define CSUDI_VK_LEFT					0x25   ///< ����� 
#define CSUDI_VK_UP						0x26   ///< ���ϼ�
#define CSUDI_VK_RIGHT					0x27   ///< ���Ҽ�
#define CSUDI_VK_DOWN					0x28   ///< ���¼�
#define CSUDI_VK_CHANNEL_UP				0x01AB	///< Ƶ�����Ӽ�
#define CSUDI_VK_CHANNEL_DOWN			0x01AC  ///< Ƶ�����ټ�
#define CSUDI_VK_VOLUME_UP				0x01BF  ///< �������Ӽ�
#define CSUDI_VK_VOLUME_DOWN			0x01C0	///< �������ټ�
#define CSUDI_VK_MENU					0X01D4  ///< �˵���
#define CSUDI_VK_MUTE					0x01C1	///< ������
#define CSUDI_VK_GUIDE					0x01CA	///< ��Ŀָ�ϼ�
#define CSUDI_VK_AUDIO					0x0197  ///< ������
#define CSUDI_VK_COLORED_KEY_0	        0X0193  ///< ��ɫ��
#define CSUDI_VK_COLORED_KEY_1	        0X0194  ///< ��ɫ��
#define CSUDI_VK_COLORED_KEY_2	        0X0195  ///< ��ɫ��
#define CSUDI_VK_COLORED_KEY_3	        0X0196  ///< ��ɫ��

#define CSUDI_VK_PROG_LIST             0x0281 ///< Ƶ���б��
#define CSUDI_VK_TV_RADIO              0x0282 ///< ����/��Ƶ�㲥��
#define CSUDI_VK_CARD_INFO             0x0283 ///< ����Ϣ��
#define CSUDI_VK_MAIL_INFO             0x0284 ///< �ʼ���
#define CSUDI_VK_VOD                   0x0285 ///< �㲥��
#define CSUDI_VK_SYSSET					0x0286 ///< ϵͳ���ü�
#define CSUDI_VK_HELP					0x1E3   ///< ������
#define CSUDI_VK_SERV					0x01D0  ///< ��Ѷ��
#define CSUDI_VK_FAV					0x01E0	///< ϲ����
#define CSUDI_VK_TV						0x01E1	///< ���Ӽ�
#define CSUDI_VK_RADIO					0x01E2  ///< �㲥��
#define CSUDI_VK_INFO					0x01C9  ///< ��Ϣ��
#define CSUDI_VK_LANGUAGE				0x0198  ///< ��/Ӣ���л���
#define CSUDI_VK_INPUT                 	0x84	///<  ���뷨����

#define CSUDI_VK_REFRESH               0x80    ///<  ˢ����ҳ 
#define CSUDI_VK_FORWARD               0x81	///<  ǰ������ʷ��ҳ�� 
#define CSUDI_VK_BACK                  0x82	///<  ���ˣ���ʷ��ҳ�� 
#define CSUDI_VK_BROWSER                0x83	///<  ��������� 
#define CSUDI_VK_RECORD       	        0xf10	///< ¼�Ƽ�
#define CSUDI_VK_PLAY 					0xf16	///< ����
#define CSUDI_VK_STOP 					0xf17	///< ֹͣ����
#define CSUDI_VK_PAUSE 					0xf18   ///< ��ͣ
#define CSUDI_VK_RW   					0xf21	///< ����
#define CSUDI_VK_FF   					0xf22	///< ���
#define CSUDI_VK_SF   					0xf23	///< ����
#define CSUDI_VK_SB   					0xf24	///< ����

#define CSUDI_VK_POUND					0x23	///< "#"��
#define CSUDI_VK_ASTERISK			    0x2A	///< "*"��
#define CSUDI_VK_SEEK                  	0xf28   ///< ��λ��
#define CSUDI_VK_TAB					0x09	///< Tab��

#define CSUDI_VK_END            		0x23	///< END��
#define CSUDI_VK_HOME           		0x24 	///< Home��
#define CSUDI_VK_INSERT         		0x9B	///< Insert��
#define CSUDI_VK_DELETE        			0x7F	///< Delete��

#define CSUDI_VK_TELETEXT				0x01CB	///<��Ļ��
#define CSUDI_VK_SUBTITLE				0x01CC	///<subtitle��

#define CSUDI_VK_RECALL					0xf24	///<Recall��
#define CSUDI_VK_TIMER_REC				0xf25	///<��ʾԤ���б�

#define CSUDI_VK_GAME					0xf29	///<��Ϸ��ݼ�
#define CSUDI_VK_ADS					0xf2A	///<����ݼ�
#define CSUDI_VK_NVOD					0xf2B	///<NVOD��ݼ�
#define CSUDI_VK_STOCK					0xf2C	///<��Ʊ��ݼ�
#define CSUDI_VK_PAYOUT					0xf2D	///<֧����ݼ�
#define CSUDI_VK_PORTAL					0xf2E	///<Portal��ݼ�
		
#define CSUDI_VK_LBUTTON				0x01	///<������
#define CSUDI_VK_RBUTTON				0x02	///<����Ҽ�
#define CSUDI_VK_CANCEL					0x03	///<ȡ������Ctrl-Break��
#define CSUDI_VK_MBUTTON				0x04	///<����м�
#define CSUDI_VK_CLEAR					0x0C	///<Num Lock�ر�ʱ�����ּ���5
#define CSUDI_VK_SHIFT					0x10	///<Shift��
#define CSUDI_VK_CONTROL				0x11	///<Ctrl��
#define CSUDI_VK_ALT					0x12	///<Alt��
#define CSUDI_VK_CAPITAL				0x14	///<Caps lock��
#define CSUDI_VK_SPACE					0x20	///<�ո��
#define CSUDI_VK_SELECT					0x29	///<Select��
#define CSUDI_VK_PRINT					0x2A	///<Print��
#define CSUDI_VK_EXECUTE				0x2B	///<Execute��
#define CSUDI_VK_SNAPSHOT				0x2C	///<Print Screen��
#define CSUDI_VK_A						0x41	///<��ĸ��A
#define CSUDI_VK_B						0x42	///<��ĸ��B
#define CSUDI_VK_C						0x43	///<��ĸ��C
#define CSUDI_VK_D						0x44	///<��ĸ��D
#define CSUDI_VK_E						0x45	///<��ĸ��E
#define CSUDI_VK_F						0x46	///<��ĸ��F
#define CSUDI_VK_G						0x47	///<��ĸ��G
#define CSUDI_VK_H						0x48	///<��ĸ��H
#define CSUDI_VK_I						0x49	///<��ĸ��I
#define CSUDI_VK_J						0x4A	///<��ĸ��J
#define CSUDI_VK_K						0x4B	///<��ĸ��K
#define CSUDI_VK_L						0x4C	///<��ĸ��L
#define CSUDI_VK_M						0x4D	///<��ĸ��M
#define CSUDI_VK_N						0x4E	///<��ĸ��N
#define CSUDI_VK_O						0x4F	///<��ĸ��O
#define CSUDI_VK_P						0x50	///<��ĸ��P
#define CSUDI_VK_Q						0x51	///<��ĸ��Q
#define CSUDI_VK_R						0x52	///<��ĸ��R
#define CSUDI_VK_S						0x53	///<��ĸ��S
#define CSUDI_VK_T						0x54	///<��ĸ��T
#define CSUDI_VK_U						0x55	///<��ĸ��U
#define CSUDI_VK_V						0x56	///<��ĸ��V
#define CSUDI_VK_W						0x57	///<��ĸ��W
#define CSUDI_VK_X						0x58	///<��ĸ��X
#define CSUDI_VK_Y						0x59	///<��ĸ��Y
#define CSUDI_VK_Z						0x5A	///<��ĸ��Z
#define CSUDI_VK_LWIN					0x6B	///<�󴰿�
#define CSUDI_VK_RWIN					0x7C	///<�Ҵ���
#define CSUDI_VK_NUMPAD0				0x60	///<С����0��
#define CSUDI_VK_NUMPAD1				0x61	///<С����1��
#define CSUDI_VK_NUMPAD2				0x62	///<С����2��
#define CSUDI_VK_NUMPAD3				0x63	///<С����3��
#define CSUDI_VK_NUMPAD4				0x64	///<С����4��
#define CSUDI_VK_NUMPAD5				0x65	///<С����5��
#define CSUDI_VK_NUMPAD6				0x66	///<С����6��
#define CSUDI_VK_NUMPAD7				0x67	///<С����7��
#define CSUDI_VK_NUMPAD8				0x68	///<С����8��
#define CSUDI_VK_NUMPAD9				0x69	///<С����9��
#define CSUDI_VK_MULTIPLY				0x6A	///<С����*
#define CSUDI_VK_ADD					0x6B	///<С����+
#define CSUDI_VK_SEPARATER				0x6C	///<ENTER��
#define CSUDI_VK_SUBTRACT				0x6D	///<С����-
#define CSUDI_VK_DECIMAL				0x6E	///<С����.
#define CSUDI_VK_DIVIDE					0x6F	///<С����/
#define CSUDI_VK_F1						0x70	///<F1��
#define CSUDI_VK_F2						0x71	///<F2��
#define CSUDI_VK_F3						0x72	///<F3��
#define CSUDI_VK_F4						0x73	///<F4��
#define CSUDI_VK_F5						0x74	///<F5��
#define CSUDI_VK_F6						0x75	///<F6��
#define CSUDI_VK_F7						0x76	///<F7��
#define CSUDI_VK_F8						0x77	///<F8��
#define CSUDI_VK_F9						0x78	///<F9��
#define CSUDI_VK_F10					0x79	///<F10��
#define CSUDI_VK_F11					0x7A	///<F11��
#define CSUDI_VK_F12					0x7B	///<F12��
#define CSUDI_VK_F13					0x7C	///<F13��
#define CSUDI_VK_F14					0x7D	///<F14��
#define CSUDI_VK_F15					0x7E	///<F15��
#define CSUDI_VK_F16					0x7F	///<F16��
#define CSUDI_VK_NUM_LOCK				0x90	///<num lock��
#define CSUDI_VK_SCROLL_LOCK			0x91	///<scroll lock��
#define CSUDI_VK_OEM_1					0xBA	///<"��" or "��"
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

#define CSUDI_VK_PRIOR					CSUDI_VK_PAGE_UP	///<���Ϸ�ҳ��
#define CSUDI_VK_NEXT						CSUDI_VK_PAGE_DOWN	///<���·�ҳ��
#define CSUDI_VK_RETURN					CSUDI_VK_OK			///<�س���
#define CSUDI_VK_ESCAPE					CSUDI_VK_EXIT		///<ESC��(����)

#define CSUDI_VK_HDD_LIST					0xf11	///<Ӳ���ļ��б��
#define CSUDI_VK_PIP							0xf12	///<PIP
#define CSUDI_VK_SWAP							0xf13	///<��������
#define CSUDI_VK_ASSIST						0xf14	///<��ݲ˵���/���ּ�/ħ��

#define CSUDI_VK_INSERT_WIN				0x2D	///<Insert��,Ϊ�˲���awt�иð���ֵ�����ͻ��windows������İ��������±� _WIN ��־
#define CSUDI_VK_DELETE_WIN				0x2E	///<Delete��,windows��ʹ�á�
#define CSUDI_VK_BACK_WIN					0x08	///<back space��

/** �ܶ���Ŀ������İ��������ڵ�����������˵��Ҫ���ӵ�udi�� */
#define CSUDI_VK_INTERACTION 0xAAA				///< ������
#define CSUDI_VK_TIMESHIFT   0xAAB				///< ʱ�Ƽ�
#define CSUDI_VK_SEARCH      0xAAC				///< ����
#define CSUDI_VK_ALL      0xAAD					///< ȫ����

#define CSUDI_VK_REPLAY         0xAAE               //�ؿ���ǰ��Ŀ��
#define CSUDI_VK_HDTV           0xAAF				//�����Ŀ��
#define CSUDI_VK_PPV_CALL       0xAB0				//PPV��Ŀ��
#define CSUDI_VK_PRE_DAY_EPG    0xAB1				//ǰһ���EPG��Ϣ��
#define CSUDI_VK_NEXT_DAY_EPG   0xAB2				//��һ���EPG��Ϣ��
#define CSUDI_VK_MOSAIC         0xAB3				//mosaic��


/**@brief INPUT �豸��ش������ֵ*/
enum
{
	CSUDIINPUT_ERROR_BAD_PARAMETER = CSUDI_INPUT_ERROR_BASE,	///< ��������
	CSUDIINPUT_ERROR_FEATURE_NOT_SUPPORTED,						///< ������֧�� 
	CSUDIINPUT_ERROR_ALREADY_ADDED,                     ///< �ص������Ѿ�ע��
	CSUDIINPUT_ERROR_CALLBACK_FULL,                     ///< ע��Ļص������Ѿ��ﵽ���ֵ
	CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST                   ///< Ҫɾ���Ļص�û�б�ע��
};

/**@brief ���밴��״̬����ö�ٶ��� */
typedef enum
{
	EM_UDIINPUT_KEY_PRESS     = 0x0100,    ///< ��������
	EM_UDIINPUT_KEY_RELEASE   = 0x0200,    ///< �����ͷ�
	EM_UDIINPUT_KEY_REPEAT    = 0x0400    ///< ��������
}CSUDIInputKeyStatus_E;

/**@brief �����豸��Դ����ö�ٶ��� */
typedef enum
{
	EM_UDIINPUT_FRONTPANEL,  ///< ǰ�����������
	EM_UDIINPUT_REMOTECTRL,  ///< ң������������
	EM_UDIINPUT_KEYBOARD,    ///< ������������
	EM_UDIINPUT_MOUSE,        ///< �����������
	EM_UDIINPUT_GENERALCODES,  ///< ��ͨ�ֽ�����Ŀǰ������ˢ��ң����
	EM_UDIINPUT_JOYSTICK    ///<��Ϸ�ֱ�
}CSUDIInputType_E;

/**@brief �����豸�¼�����ö�ٶ��� , ��Ӧ  CSUDIInputEvent_S.m_uEeventType*/
typedef enum
{
	EM_UDIINPUT_EV_SYN=0x00,  ///< ͬ���¼�
	EM_UDIINPUT_EV_KEY=0x01,  ///< ����,CSUDIInputInfo_S.m_nKeyCode�ֽ�1��0Ϊ����ֵ
	EM_UDIINPUT_EV_REL=0x02,    ///< �������,Relative
	EM_UDIINPUT_EV_ABS=0x03,        ///< ��������,Absolute
	EM_UDIINPUT_EV_MSC=0x04,  ///< ����Misc
	EM_UDIINPUT_EV_LED=0x11,  ///< LED��,CSUDIInputInfo_S.m_nKeyCode�ֽ�1ΪCSUDIInputLEDType_E,�ֽ�0ΪLED��״̬CSUDIInputLEDStatus_E
	EM_UDIINPUT_EV_SND=0x12,    ///< ����
	EM_UDIINPUT_EV_REP=0x14,        ///< repeat
	EM_UDIINPUT_EV_FF=0x15,  ///< ForceFeedback
	EM_UDIINPUT_EV_PWR=0x16,        ///< Power
	EM_UDIINPUT_EV_FF_STATUS=0x17,  ///< ForceFeedbackStatus
	EM_UDIINPUT_EV_MAX=0x1f  ///<�¼����ֵ
}CSUDIInputEventType_E;


/**@brief mouse keypad joystick Synchronization events���壬��Ӧ  CSUDIInputEvent_S.m_uCode*/
#define CSUDIINPUT_SYN_REPORT		0
#define CSUDIINPUT_SYN_CONFIG		1

 /**@brief mouse keypad joystick Keys and buttons ���� ����Ӧ  CSUDIInputEvent_S.m_uCode*/
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


 /**@brief mouse keypad joystick Relative axes ����  ����Ӧ  CSUDIInputEvent_S.m_uCode*/
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

 /**@brief mouse keypad joystick Absolute axes ���� ����Ӧ  CSUDIInputEvent_S.m_uCode*/
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


/**@brief mouse keypad joystick Misc events���� ����Ӧ  CSUDIInputEvent_S.m_uCode*/
#define CSUDIINPUT_MSC_SERIAL		0x00
#define CSUDIINPUT_MSC_PULSELED		0x01
#define CSUDIINPUT_MSC_GESTURE		0x02
#define CSUDIINPUT_MSC_RAW		0x03
#define CSUDIINPUT_MSC_SCAN		0x04
#define CSUDIINPUT_MSC_MAX		0x07

   
 /**@brief mouse keypad joystick LEDs���壬��Ӧ  CSUDIInputEvent_S.m_uCode */
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

 /**@brief mouse keypad joystick Autorepeat values���壬��Ӧ  CSUDIInputEvent_S.m_uCode */
#define CSUDIINPUT_REP_DELAY		0x00
#define CSUDIINPUT_REP_PERIOD		0x01
#define CSUDIINPUT_REP_MAX		0x01

/**@brief mouse keypad joystick Sounds���壬��Ӧ  CSUDIInputEvent_S.m_uCode */
#define CSUDIINPUT_SND_CLICK		0x00
#define CSUDIINPUT_SND_BELL		0x01
#define CSUDIINPUT_SND_TONE		0x02
#define CSUDIINPUT_SND_MAX		0x07


 /**@brief �����豸LED��״̬ö�ٶ��� */
 typedef enum
 {
     EM_UDIINPUT_LED_OFF=0x00,  ///< LED ��
     EM_UDIINPUT_LED_ON=0x01   ///< LED ��
 } CSUDIInputLEDStatus_E ;

 /**@brief ���������Ϸ�ֱ�ʱ�䶨�� */
 typedef struct 
 {
     unsigned long    tv_sec;
     unsigned long    tv_usec;
 }CSUDIInputTimeval_S;

 /**@brief ���������Ϸ�ֱ��������Ͷ��� */
typedef struct 
{
    unsigned int m_dwDeviceIndex;   ///<�¼��������豸��ţ���������ͬһ���͵Ķ���豸
    CSUDIInputTimeval_S m_stTime;      ///<�¼�������ʱ��
    unsigned short m_uEeventType;      ///< �¼����ͣ���Ӧ��CSUDIInputEventType_E
    unsigned short m_uCode;      ///< �¼���Ӧ�����ݣ�����m_uEeventType��ֵ����Ӧ��CSUDIINPUT_SYN_*,
                                                //CSUDIINPUT_KEY_*,CSUDIINPUT_REL_*,CSUDIINPUT_ABS_*,
                                                //CSUDIINPUT_MSC_*,CSUDIINPUT_LED_*,CSUDIINPUT_REP_*��CSUDIINPUT_SND_*
    signed int m_nValue;///<�¼�Я����״̬����. ����m_uEeventType�Ĳ�ͬ���������죬
                                    //����EM_UDIINPUT_EV_SYN ,reserved
                                    //����EM_UDIINPUT_EV_KEY,0 for KEY release, 1 for keypress and 2 for autorepeat.
                                    //����EM_UDIINPUT_EV_REL,��� ƫ��ֵ
                                    //����EM_UDIINPUT_EV_ABS,����λ�� 
                                    //����EM_UDIINPUT_EV_LED��led��״̬����ӦCSUDIInputLEDStatus_E
}CSUDIInputEvent_S ;

/**@brief ��ͨ�ֽ����������Ͷ��� */
typedef struct
{
    unsigned char*    m_pucData;        ///< ����ָ��,�������ݸ�ʽ�ο��������淶�ĵ�
    int               m_nDataLen;             ///< ���ݳ���
}CSUDIGeneralCodesInputData_S;

/**@brief ������Ϣ������� */
typedef struct  
{
	CSUDIInputType_E 	m_eType;		///<  �����豸��Դ�����CSUDIInputType_E
	CSUDIInputKeyStatus_E 		m_eStatus;		///< ���밴��״̬,���CSUDIInputKeyStatus_E
	int			m_nKeyCode;		///< �������,��m_eTypeΪEM_UDIINPUT_FRONTPANEL��EM_UDIINPUT_REMOTECTRLʱ��ע��ֵ�������������Ϊ0
	void*       m_ExtInputData;   ///<��չ���ݣ�����m_eType�Ĳ�ͬ�����Ͳ�ͬ��Ŀǰ����ˢ��ң����ʱת��ΪCSUDIGeneralCodesInputData_S*���ͣ�
	                                            //��m_eTypeΪEM_UDIINPUT_KEYBOARD,   EM_UDIINPUT_MOUSE,   EM_UDIINPUT_JOYSTICK   ʱת��ΪCSUDIInputEvent_S*����
							  //���ų��Ժ�������������Ͷ���.��δ�õ�������ʱ������ΪNULL
}CSUDIInputInfo_S;	

/**@brief �����豸�ص���������

@param[in] psInputInfo ���CSUDIInputInfo_S˵��
@param[in] pvUserData �û�����
*/
typedef  void ( *CSUDIInputCallback_F )(CSUDIInputInfo_S * psInputInfo ,void * pvUserData);

/**
@brief ע�������豸�����ص�����

�κ������豸������Ч�İ�����Ϣ��ͨ��ע��Ļص�����֪ͨӦ�ò�\n
@param[in] fnInputCallback ָ��ص������ĺ���ָ�룬�û�ͨ���ص������õ�������Դ��״̬�͹�����
@param[in] pvUserData �û����ݣ�����ΪCSUDI_NULL
@return ע��ɹ��򷵻�CSUDI_SUCCESS ;ע��ʧ���򷵻���ش������
@note 
- ֧��ע��Ļص�����������֧��8���������
- fnInputCallback��pvUserDataΨһ��ʶһ���ص����ͣ�������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIINPUT_ERROR_ALREADY_ADDED����

@note Booter��ص������������ж��е��ã��ϲ�ᰴ�ж�ʹ��ԭ��ʹ�ûص�
*/
CSUDI_Error_Code CSUDIINPUTAddCallback( CSUDIInputCallback_F fnInputCallback ,void * pvUserData);


/**
@brief ɾ�������豸�����ص�����

@param[in] fnInputCallback ָ��ص������ĺ���ָ��
@param[in] pvUserData �û�����
@return ɾ���ɹ��򷵻�CSUDI_SUCCESS;ɾ��ʧ���򷵻���ش������
@note ���Ҫɾ��һ�������ڵĻص����򷵻�CSUDIINPUT_ERROR_CALLBACK_NOT_EXIST
@note fnInputCallback��pvUserDataΨһ��ʶһ���ص�����

@note Booter��ص������������ж��е��ã��ϲ�ᰴ�ж�ʹ��ԭ��ʹ�ûص�
*/
CSUDI_Error_Code CSUDIINPUTDelCallback(  CSUDIInputCallback_F fnInputCallback ,void * pvUserData);


#ifdef __cplusplus
}
#endif

/** @} */

#endif   

