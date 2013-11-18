/****************************************************************************
*                                                                           *
* winuser.h -- USER procedure declarations, constant definitions and macros *
*                                                                           *
* CSCopyright (c) Coship Corporation. All rights reserved.                          *
*                                                                           *
****************************************************************************/




#ifndef _CS_WINUSER_
#define _CS_WINUSER_


//
// CSDefine API decoration for direct importing of DLL references.
//

#if !defined(_USER32_)
#undef WINUSERAPI
#define WINUSERAPI GUIDLL_API //DECLSPEC_IMPORT
#else
#undef WINUSERAPI
#define WINUSERAPI GUIDLL_API //DECLSPEC_IMPORT
#endif


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef WINVER
#define WINVER  0x0500      /* version 5.0 */
#endif /* !WINVER */

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x0500		/* version 5.0 */
#endif /* !WINVER */
//#include <stdarg.h>

#ifndef NOUSER

typedef HCSHANDLE HCSDWP;
typedef VOID MENUTEMPLATEA;
typedef MENUTEMPLATEA MENUTEMPLATE;
typedef PVOID LPMENUTEMPLATEA;
typedef LPMENUTEMPLATEA LPMENUTEMPLATE;

typedef LRESULT (CALLBACK* CSWNDPROC)(HCSWND, UINT, WPARAM, LPARAM);

#ifdef STRICT

typedef INT_PTR (CALLBACK* CSDLGPROC)(HCSWND, UINT, WPARAM, LPARAM);
typedef VOID (CALLBACK* CSTIMERPROC)(HCSWND, UINT, UINT_PTR, DWORD);
typedef BOOL (CALLBACK* CSGRAYSTRINGPROC)(HCSDC, LPARAM, int);
typedef BOOL (CALLBACK* CSWNDENUMPROC)(HCSWND, LPARAM);
typedef LRESULT (CALLBACK* CSHOOKPROC)(int code, WPARAM wParam, LPARAM lParam);
typedef VOID (CALLBACK* CSSENDASYNCPROC)(HCSWND, UINT, ULONG_PTR, LRESULT);

typedef BOOL (CALLBACK* CSPROPENUMPROCA)(HCSWND, LPCTSTR, HCSHANDLE);
typedef BOOL (CALLBACK* CSPROPENUMPROCW)(HCSWND, LPCWSTR, HCSHANDLE);

typedef BOOL (CALLBACK* CSPROPENUMPROCEXA)(HCSWND, LPTSTR, HCSHANDLE, ULONG_PTR);
typedef BOOL (CALLBACK* CSPROPENUMPROCEXW)(HCSWND, LPWSTR, HCSHANDLE, ULONG_PTR);

typedef int (CALLBACK* CSEDITWORDBREAKPROCA)(LPTSTR lpch, int ichCurrent, int cch, int code);
typedef int (CALLBACK* CSEDITWORDBREAKPROCW)(LPWSTR lpch, int ichCurrent, int cch, int code);

#if(WINVER >= 0x0400)
typedef BOOL (CALLBACK* CSDRAWSTATEPROC)(HCSDC hdc, LPARAM lData, WPARAM wData, int cx, int cy);
#endif /* WINVER >= 0x0400 */
#else /* !STRICT */

typedef FARPROC CSDLGPROC;
typedef FARPROC CSTIMERPROC;
typedef FARPROC CSGRAYSTRINGPROC;
typedef FARPROC CSWNDENUMPROC;
typedef FARPROC CSHOOKPROC;
typedef FARPROC CSSENDASYNCPROC;

typedef FARPROC CSEDITWORDBREAKPROCA;

typedef FARPROC CSPROPENUMPROCA;

typedef FARPROC CSPROPENUMPROCEXA;

#if(WINVER >= 0x0400)
typedef FARPROC CSDRAWSTATEPROC;
#endif /* WINVER >= 0x0400 */
#endif /* !STRICT */

typedef CSPROPENUMPROCA        CSPROPENUMPROC;
typedef CSPROPENUMPROCEXA      CSPROPENUMPROCEX;
typedef CSEDITWORDBREAKPROCA   CSEDITWORDBREAKPROC;

#ifdef STRICT

typedef BOOL (CALLBACK* CSNAMEENUMPROCA)(LPTSTR, LPARAM);
typedef BOOL (CALLBACK* CSNAMEENUMPROCW)(LPWSTR, LPARAM);

typedef CSNAMEENUMPROCA   CSWINSTAENUMPROCA;
typedef CSNAMEENUMPROCA   CSDESKTOPENUMPROCA;


#else /* !STRICT */

typedef FARPROC CSNAMEENUMPROCA;
typedef FARPROC CSWINSTAENUMPROCA;
typedef FARPROC CSDESKTOPENUMPROCA;


#endif /* !STRICT */

typedef CSWINSTAENUMPROCA     CSWINSTAENUMPROC;
typedef CSDESKTOPENUMPROCA    CSDESKTOPENUMPROC;


#define IS_INTRESOURCE(_r) (((ULONG_PTR)(_r) >> 16) == 0)
#undef MAKEINTRESOURCE
#define MAKEINTRESOURCE(i) (LPTSTR)((ULONG_PTR)((WORD)(i)))


#ifndef NORESOURCE

/*
 * Predefined Resource Types
 */
#define RT_CURSOR           MAKEINTRESOURCE(1)
#define RT_BITMAP           MAKEINTRESOURCE(2)
#define RT_ICON             MAKEINTRESOURCE(3)
#define RT_MENU             MAKEINTRESOURCE(4)
#define RT_DIALOG           MAKEINTRESOURCE(5)
#define RT_STRING           MAKEINTRESOURCE(6)
#define RT_FONTDIR          MAKEINTRESOURCE(7)
#define RT_FONT             MAKEINTRESOURCE(8)
#define RT_ACCELERATOR      MAKEINTRESOURCE(9)
#define RT_RCDATA           MAKEINTRESOURCE(10)
#define RT_MESSAGETABLE     MAKEINTRESOURCE(11)

#define DIFFERENCE     11
#define RT_GROUP_CURSOR MAKEINTRESOURCE((ULONG_PTR)RT_CURSOR + DIFFERENCE)
#define RT_GROUP_ICON   MAKEINTRESOURCE((ULONG_PTR)RT_ICON + DIFFERENCE)
#define RT_VERSION      MAKEINTRESOURCE(16)
#define RT_DLGINCLUDE   MAKEINTRESOURCE(17)
#if(WINVER >= 0x0400)
#define RT_PLUGPLAY     MAKEINTRESOURCE(19)
#define RT_VXD          MAKEINTRESOURCE(20)
#define RT_ANICURSOR    MAKEINTRESOURCE(21)
#define RT_ANIICON      MAKEINTRESOURCE(22)
#endif /* WINVER >= 0x0400 */
#define RT_HTML         MAKEINTRESOURCE(23)
#ifdef RC_INVOKED
#define RT_MANIFEST                        24
#define CREATEPROCESS_MANIFEST_RESOURCE_ID  1
#define ISOLATIONAWARE_MANIFEST_RESOURCE_ID 2
#define ISOLATIONAWARE_NOSTATICIMPORT_MANIFEST_RESOURCE_ID 3
#define MINIMUM_RESERVED_MANIFEST_RESOURCE_ID 1   /* inclusive */
#define MAXIMUM_RESERVED_MANIFEST_RESOURCE_ID 16  /* inclusive */
#else  /* RC_INVOKED */
#define RT_MANIFEST                        MAKEINTRESOURCE(24)
#define CREATEPROCESS_MANIFEST_RESOURCE_ID MAKEINTRESOURCE( 1)
#define ISOLATIONAWARE_MANIFEST_RESOURCE_ID MAKEINTRESOURCE(2)
#define ISOLATIONAWARE_NOSTATICIMPORT_MANIFEST_RESOURCE_ID MAKEINTRESOURCE(3)
#define MINIMUM_RESERVED_MANIFEST_RESOURCE_ID MAKEINTRESOURCE( 1 /*inclusive*/)
#define MAXIMUM_RESERVED_MANIFEST_RESOURCE_ID MAKEINTRESOURCE(16 /*inclusive*/)
#endif /* RC_INVOKED */


#endif /* !NORESOURCE */


WINUSERAPI
int
WINAPIV
CSwsprintf(
    OUT LPTSTR,
    IN LPCTSTR,
    ...);

WINUSERAPI
int
CS_wcslen(
		  const char* pchwStr);

WINUSERAPI
char* 
CS_wcscpy(
		  char* pchwDestStr,
		  const char* pchwSrcStr);
WINUSERAPI
int
CS_wcscmp(
   const char *string1,
   const char *string2);

WINUSERAPI
char* 
CS_wcsncpy(
		   char* pchwDestStr,
		   const char* pchwSrcStr,
		   int nCount);

WINUSERAPI
char* CS_wcscat(char* pchwDestStr,
				const char* pchwSrcStr);

WINUSERAPI
char* CS_wcsncat(char* pchwDestStr,const char* pchwSrcStr,int nCount);

WINUSERAPI
int
WINAPI
CSMultiByteToWideChar(
    IN UINT     CodePage,
    IN DWORD    dwFlags,
    IN LPCSTR   lpMultiByteStr,
    IN int      cbMultiByte,
    OUT LPWSTR  lpWideCharStr,
    IN int      cchWideChar);
    
    
WINUSERAPI
int
WINAPI
CSSetDefaultLanguageCode(
    int     nCode);
       

WINUSERAPI
double 
WINAPI
CS_wtof(
		const char* string
		);

WINUSERAPI
int 
WINAPI
CS_wtoi(
		const char* string
		);

WINUSERAPI
long 
WINAPI
CS_wtol(
		const char* string
		);
WINUSERAPI
long 
WINAPI
CS_wcstol(
		  const char* nptr,
		  char** endptr,
		  int base);




/*
 * SPI_SETDESKWALLPAPER defined constants
 */
#define SETWALLPAPER_DEFAULT    ((LPWSTR)-1)

#ifndef NOSCROLL

/*
 * Scroll Bar Constants
 */
#define SB_HORZ             0
#define SB_VERT             1
#define SB_CTL              2
#define SB_BOTH             3

/*
 * Scroll Bar Commands
 */
#define SB_LINEUP           0
#define SB_LINELEFT         0
#define SB_LINEDOWN         1
#define SB_LINERIGHT        1
#define SB_PAGEUP           2
#define SB_PAGELEFT         2
#define SB_PAGEDOWN         3
#define SB_PAGERIGHT        3
#define SB_THUMBPOSITION    4
#define SB_THUMBTRACK       5
#define SB_TOP              6
#define SB_LEFT             6
#define SB_BOTTOM           7
#define SB_RIGHT            7
#define SB_ENDSCROLL        8

#endif /* !NOSCROLL */

#ifndef NOSHOWWINDOW


/*
 * CSShowWindow() Commands
 */
#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
#define SW_SHOWDEFAULT      10
#define SW_FORCEMINIMIZE    11
#define SW_MAX              11

/*
 * Old CSShowWindow() Commands
 */
#define HIDE_WINDOW         0
#define SHOW_OPENWINDOW     1
#define SHOW_ICONWINDOW     2
#define SHOW_FULLSCREEN     3
#define SHOW_OPENNOACTIVATE 4

/*
 * Identifiers for the WM_SHOWWINDOW message
 */
#define SW_PARENTCLOSING    1
#define SW_OTHERZOOM        2
#define SW_PARENTOPENING    3
#define SW_OTHERUNZOOM      4


#endif /* !NOSHOWWINDOW */

#if(WINVER >= 0x0500)
/*
 * AnimateWindow() Commands
 */
#define AW_HOR_POSITIVE             0x00000001
#define AW_HOR_NEGATIVE             0x00000002
#define AW_VER_POSITIVE             0x00000004
#define AW_VER_NEGATIVE             0x00000008
#define AW_CENTER                   0x00000010
#define AW_HIDE                     0x00010000
#define AW_ACTIVATE                 0x00020000
#define AW_SLIDE                    0x00040000
#define AW_BLEND                    0x00080000

#endif /* WINVER >= 0x0500 */


/*
 * WM_KEYUP/DOWN/CHAR HIWORD(lParam) flags
 */
#define KF_EXTENDED       0x0100
#define KF_DLGMODE        0x0800
#define KF_MENUMODE       0x1000
#define KF_ALTDOWN        0x2000
#define KF_REPEAT         0x4000
#define KF_UP             0x8000

#ifndef NOVIRTUALKEYCODES


/*
 * Virtual Keys, Standard CSSet
 */
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#if(_WIN32_WINNT >= 0x0500)
#define VK_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define VK_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0x07 : unassigned
 */

#define VK_BACK           0x08
#define VK_TAB            0x09

/*
 * 0x0A - 0x0B : reserved
 */

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */

#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D

/*
 * 0x5E : reserved
 */

#define VK_SLEEP          0x5F

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

/*
 * 0x88 - 0x8F : unassigned
 */

#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91

/*
 * NEC PC-9800 kbd definitions
 */
#define VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad

/*
 * Fujitsu/OASYS kbd definitions
 */
#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to CSGetAsyncKeyState() and CSGetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5

#if(_WIN32_WINNT >= 0x0500)
#define VK_BROWSER_BACK        0xA6
#define VK_BROWSER_FORWARD     0xA7
#define VK_BROWSER_REFRESH     0xA8
#define VK_BROWSER_STOP        0xA9
#define VK_BROWSER_SEARCH      0xAA
#define VK_BROWSER_FAVORITES   0xAB
#define VK_BROWSER_HOME        0xAC

#define VK_VOLUME_MUTE         0xAD
#define VK_VOLUME_DOWN         0xAE
#define VK_VOLUME_UP           0xAF
#define VK_MEDIA_NEXT_TRACK    0xB0
#define VK_MEDIA_PREV_TRACK    0xB1
#define VK_MEDIA_STOP          0xB2
#define VK_MEDIA_PLAY_PAUSE    0xB3
#define VK_LAUNCH_MAIL         0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1         0xB6
#define VK_LAUNCH_APP2         0xB7

#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xB8 - 0xB9 : reserved
 */

#define VK_OEM_1          0xBA   // ';:' for US
#define VK_OEM_PLUS       0xBB   // '+' any country
#define VK_OEM_COMMA      0xBC   // ',' any country
#define VK_OEM_MINUS      0xBD   // '-' any country
#define VK_OEM_PERIOD     0xBE   // '.' any country
#define VK_OEM_2          0xBF   // '/?' for US
#define VK_OEM_3          0xC0   // '`~' for US

/*
 * 0xC1 - 0xD7 : reserved
 */

/*
 * 0xD8 - 0xDA : unassigned
 */

#define VK_OEM_4          0xDB  //  '[{' for US
#define VK_OEM_5          0xDC  //  '\|' for US
#define VK_OEM_6          0xDD  //  ']}' for US
#define VK_OEM_7          0xDE  //  ''"' for US
#define VK_OEM_8          0xDF

/*
 * 0xE0 : reserved
 */

/*
 * Various extended or enhanced keyboards
 */
#define VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define VK_ICO_HELP       0xE3  //  Help key on ICO
#define VK_ICO_00         0xE4  //  00 key on ICO

#if(WINVER >= 0x0400)
#define VK_PROCESSKEY     0xE5
#endif /* WINVER >= 0x0400 */

#define VK_ICO_CLEAR      0xE6


#if(_WIN32_WINNT >= 0x0500)
#define VK_PACKET         0xE7
#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xE8 : unassigned
 */

/*
 * Nokia/Ericsson definitions
 */
#define VK_OEM_RESET      0xE9
#define VK_OEM_JUMP       0xEA
#define VK_OEM_PA1        0xEB
#define VK_OEM_PA2        0xEC
#define VK_OEM_PA3        0xED
#define VK_OEM_WSCTRL     0xEE
#define VK_OEM_CUSEL      0xEF
#define VK_OEM_ATTN       0xF0
#define VK_OEM_FINISH     0xF1
#define VK_OEM_COPY       0xF2
#define VK_OEM_AUTO       0xF3
#define VK_OEM_ENLW       0xF4
#define VK_OEM_BACKTAB    0xF5

#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE

/*
 * 0xFF : reserved
 */


#endif /* !NOVIRTUALKEYCODES */

#ifndef NOWH

/*
 * CSSetWindowsHook() codes
 */
#define WH_MIN              (-1)
#define WH_MSGFILTER        (-1)
#define WH_JOURNALRECORD    0
#define WH_JOURNALPLAYBACK  1
#define WH_KEYBOARD         2
#define WH_GETMESSAGE       3
#define WH_CALLWNDPROC      4
#define WH_CBT              5
#define WH_SYSMSGFILTER     6
#define WH_MOUSE            7
#if defined(_WIN32_WINDOWS)
#define WH_HARDWARE         8
#endif
#define WH_DEBUG            9
#define WH_SHELL           10
#define WH_FOREGROUNDIDLE  11
#if(WINVER >= 0x0400)
#define WH_CALLWNDPROCRET  12
#endif /* WINVER >= 0x0400 */

#if (_WIN32_WINNT >= 0x0400)
#define WH_KEYBOARD_LL     13
#define WH_MOUSE_LL        14
#endif // (_WIN32_WINNT >= 0x0400)


#if(WINVER >= 0x0400)
#undef WH_MAX
#if (_WIN32_WINNT >= 0x0400)
#define WH_MAX             14
#else
#define WH_MAX             12
#endif // (_WIN32_WINNT >= 0x0400)
#else
#define WH_MAX             11
#endif

#define WH_MINHOOK         WH_MIN
#define WH_MAXHOOK         WH_MAX

/*
 * Hook Codes
 */
#define HC_ACTION           0
#define HC_GETNEXT          1
#define HC_SKIP             2
#define HC_NOREMOVE         3
#define HC_NOREM            HC_NOREMOVE
#define HC_SYSMODALON       4
#define HC_SYSMODALOFF      5

/*
 * CBT Hook Codes
 */
#define HCBT_MOVESIZE       0
#define HCBT_MINMAX         1
#define HCBT_QS             2
#define HCBT_CREATEWND      3
#define HCBT_DESTROYWND     4
#define HCBT_ACTIVATE       5
#define HCBT_CLICKSKIPPED   6
#define HCBT_KEYSKIPPED     7
#define HCBT_SYSCOMMAND     8
#define HCBT_SETFOCUS       9

/*
 * HCBT_CREATEWND parameters pointed to by lParam
 */
typedef struct tagCSCBT_CREATEWNDA
{
    struct tagCSCREATESTRUCTA *lpcs;
    HCSWND           hwndInsertAfter;
} CSCBT_CREATEWNDA, *LPCSCBT_CREATEWNDA;
/*
 * HCBT_CREATEWND parameters pointed to by lParam
 */
typedef CSCBT_CREATEWNDA CSCBT_CREATEWND;
typedef LPCSCBT_CREATEWNDA LPCSCBT_CREATEWND;

/*
 * HCBT_ACTIVATE structure pointed to by lParam
 */
typedef struct tagCSCBTACTIVATESTRUCT
{
    BOOL    fMouse;
    HCSWND    hWndActive;
} CSCBTACTIVATESTRUCT, *LPCSCBTACTIVATESTRUCT;

#if(_WIN32_WINNT >= 0x0501)
/*
 * CSWTSSESSION_NOTIFICATION struct pointed by lParam, for WM_WTSSESSION_CHANGE
 */
typedef struct tagCSWTSSESSION_NOTIFICATION
{
    DWORD cbSize;
    DWORD dwSessionId;

} CSWTSSESSION_NOTIFICATION, *PCSWTSSESSION_NOTIFICATION;

/*
 * codes passed in WPARAM for WM_WTSSESSION_CHANGE
 */

#define WTS_CONSOLE_CONNECT                0x1
#define WTS_CONSOLE_DISCONNECT             0x2
#define WTS_REMOTE_CONNECT                 0x3
#define WTS_REMOTE_DISCONNECT              0x4
#define WTS_SESSION_LOGON                  0x5
#define WTS_SESSION_LOGOFF                 0x6
#define WTS_SESSION_LOCK                   0x7
#define WTS_SESSION_UNLOCK                 0x8

#endif /* _WIN32_WINNT >= 0x0501 */

/*
 * WH_MSGFILTER Filter Proc Codes
 */
#define MSGF_DIALOGBOX      0
#define MSGF_MESSAGEBOX     1
#define MSGF_MENU           2
#define MSGF_SCROLLBAR      5
#define MSGF_NEXTWINDOW     6
#define MSGF_MAX            8                       // unused
#define MSGF_USER           4096

/*
 * Shell support
 */
#define HSHELL_WINDOWCREATED        1
#define HSHELL_WINDOWDESTROYED      2
#define HSHELL_ACTIVATESHELLWINDOW  3

#if(WINVER >= 0x0400)
#define HSHELL_WINDOWACTIVATED      4
#define HSHELL_GETMINRECT           5
#define HSHELL_REDRAW               6
#define HSHELL_TASKMAN              7
#define HSHELL_LANGUAGE             8
#endif /* WINVER >= 0x0400 */
#if(_WIN32_WINNT >= 0x0500)
#define HSHELL_ACCESSIBILITYSTATE   11
#define HSHELL_APPCOMMAND           12
#endif /* _WIN32_WINNT >= 0x0500 */

#if(_WIN32_WINNT >= 0x0501)
#define HSHELL_WINDOWREPLACED       13
#endif /* _WIN32_WINNT >= 0x0501 */


#if(_WIN32_WINNT >= 0x0500)
/* wparam for HSHELL_ACCESSIBILITYSTATE */
#define    ACCESS_STICKYKEYS            0x0001
#define    ACCESS_FILTERKEYS            0x0002
#define    ACCESS_MOUSEKEYS             0x0003

/* cmd for HSHELL_APPCOMMAND and WM_APPCOMMAND */
#define APPCOMMAND_BROWSER_BACKWARD       1
#define APPCOMMAND_BROWSER_FORWARD        2
#define APPCOMMAND_BROWSER_REFRESH        3
#define APPCOMMAND_BROWSER_STOP           4
#define APPCOMMAND_BROWSER_SEARCH         5
#define APPCOMMAND_BROWSER_FAVORITES      6
#define APPCOMMAND_BROWSER_HOME           7
#define APPCOMMAND_VOLUME_MUTE            8
#define APPCOMMAND_VOLUME_DOWN            9
#define APPCOMMAND_VOLUME_UP              10
#define APPCOMMAND_MEDIA_NEXTTRACK        11
#define APPCOMMAND_MEDIA_PREVIOUSTRACK    12
#define APPCOMMAND_MEDIA_STOP             13
#define APPCOMMAND_MEDIA_PLAY_PAUSE       14
#define APPCOMMAND_LAUNCH_MAIL            15
#define APPCOMMAND_LAUNCH_MEDIA_SELECT    16
#define APPCOMMAND_LAUNCH_APP1            17
#define APPCOMMAND_LAUNCH_APP2            18
#define APPCOMMAND_BASS_DOWN              19
#define APPCOMMAND_BASS_BOOST             20
#define APPCOMMAND_BASS_UP                21
#define APPCOMMAND_TREBLE_DOWN            22
#define APPCOMMAND_TREBLE_UP              23
#if(_WIN32_WINNT >= 0x0501)
#define APPCOMMAND_MICROPHONE_VOLUME_MUTE 24
#define APPCOMMAND_MICROPHONE_VOLUME_DOWN 25
#define APPCOMMAND_MICROPHONE_VOLUME_UP   26
#define APPCOMMAND_HELP                   27
#define APPCOMMAND_FIND                   28
#define APPCOMMAND_NEW                    29
#define APPCOMMAND_OPEN                   30
#define APPCOMMAND_CLOSE                  31
#define APPCOMMAND_SAVE                   32
#define APPCOMMAND_PRINT                  33
#define APPCOMMAND_UNDO                   34
#define APPCOMMAND_REDO                   35
#define APPCOMMAND_COPY                   36
#define APPCOMMAND_CUT                    37
#define APPCOMMAND_PASTE                  38
#define APPCOMMAND_REPLY_TO_MAIL          39
#define APPCOMMAND_FORWARD_MAIL           40
#define APPCOMMAND_SEND_MAIL              41
#define APPCOMMAND_SPELL_CHECK            42
#define APPCOMMAND_DICTATE_OR_COMMAND_CONTROL_TOGGLE    43
#define APPCOMMAND_MIC_ON_OFF_TOGGLE      44
#define APPCOMMAND_CORRECTION_LIST        45
#endif /* _WIN32_WINNT >= 0x0501 */

#define FAPPCOMMAND_MOUSE 0x8000
#define FAPPCOMMAND_KEY   0
#define FAPPCOMMAND_OEM   0x1000
#define FAPPCOMMAND_MASK  0xF000

#define GET_APPCOMMAND_LPARAM(lParam) ((short)(HIWORD(lParam) & ~FAPPCOMMAND_MASK))
#define GET_DEVICE_LPARAM(lParam)     ((WORD)(HIWORD(lParam) & FAPPCOMMAND_MASK))
#define GET_MOUSEORKEY_LPARAM         GET_DEVICE_LPARAM
#define GET_FLAGS_LPARAM(lParam)      (LOWORD(lParam))
#define GET_KEYSTATE_LPARAM(lParam)   GET_FLAGS_LPARAM(lParam)
#endif /* _WIN32_WINNT >= 0x0500 */


/*
 * Message Structure used in Journaling
 */
typedef struct tagCSEVENTMSG {
    UINT    message;
    UINT    paramL;
    UINT    paramH;
    DWORD    time;
    HCSWND     hwnd;
} CSEVENTMSG, *PCSEVENTMSGMSG, NEAR *NPCSEVENTMSGMSG, FAR *LPCSEVENTMSGMSG;

typedef struct tagCSEVENTMSG *PCSEVENTMSG, NEAR *NPCSEVENTMSG, FAR *LPCSEVENTMSG;

/*
 * Message structure used by WH_CALLWNDPROC
 */
typedef struct tagCSCWPSTRUCT {
    LPARAM  lParam;
    WPARAM  wParam;
    UINT    message;
    HCSWND    hwnd;
} CSCWPSTRUCT, *PCSCWPSTRUCT, NEAR *NPCSCWPSTRUCT, FAR *LPCSCWPSTRUCT;

#if(WINVER >= 0x0400)
/*
 * Message structure used by WH_CALLWNDPROCRET
 */
typedef struct tagCSCWPRETSTRUCT {
    LRESULT lResult;
    LPARAM  lParam;
    WPARAM  wParam;
    UINT    message;
    HCSWND    hwnd;
} CSCWPRETSTRUCT, *PCSCWPRETSTRUCT, NEAR *NPCSCWPRETSTRUCT, FAR *LPCSCWPRETSTRUCT;

#endif /* WINVER >= 0x0400 */

#if (_WIN32_WINNT >= 0x0400)

/*
 * Low level hook flags
 */

#define LLKHF_EXTENDED       (KF_EXTENDED >> 8)
#define LLKHF_INJECTED       0x00000010
#define LLKHF_ALTDOWN        (KF_ALTDOWN >> 8)
#define LLKHF_UP             (KF_UP >> 8)

#define LLMHF_INJECTED       0x00000001

/*
 * Structure used by WH_KEYBOARD_LL
 */
typedef struct tagCSKBDLLHOOKSTRUCT {
    DWORD   vkCode;
    DWORD   scanCode;
    DWORD   flags;
    DWORD   time;
    ULONG_PTR dwExtraInfo;
} CSKBDLLHOOKSTRUCT, FAR *LPCSKBDLLHOOKSTRUCT, *PCSKBDLLHOOKSTRUCT;

/*
 * Structure used by WH_MOUSE_LL
 */
typedef struct tagCSMSLLHOOKSTRUCT {
    CSPOINT   pt;
    DWORD   mouseData;
    DWORD   flags;
    DWORD   time;
    ULONG_PTR dwExtraInfo;
} CSMSLLHOOKSTRUCT, FAR *LPCSMSLLHOOKSTRUCT, *PCSMSLLHOOKSTRUCT;

#endif // (_WIN32_WINNT >= 0x0400)

/*
 * Structure used by WH_DEBUG
 */
typedef struct tagCSDEBUGHOOKINFO
{
    DWORD   idThread;
    DWORD   idThreadInstaller;
    LPARAM  lParam;
    WPARAM  wParam;
    int     code;
} CSDEBUGHOOKINFO, *PCSDEBUGHOOKINFO, NEAR *NPCSDEBUGHOOKINFO, FAR* LPCSDEBUGHOOKINFO;

/*
 * Structure used by WH_MOUSE
 */
typedef struct tagCSMOUSEHOOKSTRUCT {
    CSPOINT   pt;
    HCSWND    hwnd;
    UINT    wHitTestCode;
    ULONG_PTR dwExtraInfo;
} CSMOUSEHOOKSTRUCT, FAR *LPCSMOUSEHOOKSTRUCT, *PCSMOUSEHOOKSTRUCT;

#if(_WIN32_WINNT >= 0x0500)
#ifdef __cplusplus
typedef struct tagCSMOUSEHOOKSTRUCTEX : public tagCSMOUSEHOOKSTRUCT
{
    DWORD   mouseData;
} CSMOUSEHOOKSTRUCTEX, *LPCSMOUSEHOOKSTRUCTEX, *PCSMOUSEHOOKSTRUCTEX;
#else // ndef __cplusplus
typedef struct tagCSMOUSEHOOKSTRUCTEX
{
    CSMOUSEHOOKSTRUCT dummy;
    DWORD   mouseData;
} CSMOUSEHOOKSTRUCTEX, *LPCSMOUSEHOOKSTRUCTEX, *PCSMOUSEHOOKSTRUCTEX;
#endif
#endif /* _WIN32_WINNT >= 0x0500 */

#if(WINVER >= 0x0400)
/*
 * Structure used by WH_HARDWARE
 */
typedef struct tagCSHARDWAREHOOKSTRUCT {
    HCSWND    hwnd;
    UINT    message;
    WPARAM  wParam;
    LPARAM  lParam;
} CSHARDWAREHOOKSTRUCT, FAR *LPCSHARDWAREHOOKSTRUCT, *PCSHARDWAREHOOKSTRUCT;
#endif /* WINVER >= 0x0400 */
#endif /* !NOWH */

/*
 * Keyboard Layout API
 */
#define HKL_PREV            0
#define HKL_NEXT            1


#define KLF_ACTIVATE        0x00000001
#define KLF_SUBSTITUTE_OK   0x00000002
#define KLF_REORDER         0x00000008
#if(WINVER >= 0x0400)
#define KLF_REPLACELANG     0x00000010
#define KLF_NOTELLSHELL     0x00000080
#endif /* WINVER >= 0x0400 */
#define KLF_SETFORPROCESS   0x00000100
#if(_WIN32_WINNT >= 0x0500)
#define KLF_SHIFTLOCK       0x00010000
#define KLF_RESET           0x40000000
#endif /* _WIN32_WINNT >= 0x0500 */


#if(WINVER >= 0x0500)
/*
 * Bits in wParam of WM_INPUTLANGCHANGEREQUEST message
 */
#define INPUTLANGCHANGE_SYSCHARSET 0x0001
#define INPUTLANGCHANGE_FORWARD    0x0002
#define INPUTLANGCHANGE_BACKWARD   0x0004
#endif /* WINVER >= 0x0500 */

/*
 * Size of KeyboardLayoutName (number of characters), including nul terminator
 */
#define KL_NAMELENGTH       9

WINUSERAPI
HCSKL
WINAPI
CSLoadKeyboardLayout(
    IN LPCTSTR pwszKLID,
    IN UINT Flags);


#if(WINVER >= 0x0400)
WINUSERAPI
HCSKL
WINAPI
CSActivateKeyboardLayout(
    IN HCSKL hkl,
    IN UINT Flags);
#else
WINUSERAPI
BOOL
WINAPI
CSActivateKeyboardLayout(
    IN HCSKL hkl,
    IN UINT Flags);
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0400)
WINUSERAPI
int
WINAPI
CSToUnicodeEx(
    IN UINT wVirtKey,
    IN UINT wScanCode,
    IN CONST BYTE *lpKeyState,
    OUT LPWSTR pwszBuff,
    IN int cchBuff,
    IN UINT wFlags,
    IN HCSKL dwhkl);
#endif /* WINVER >= 0x0400 */

WINUSERAPI
BOOL
WINAPI
CSUnloadKeyboardLayout(
    IN HCSKL hkl);

WINUSERAPI
BOOL
WINAPI
CSGetKeyboardLayoutName(
    OUT LPTSTR pwszKLID);


#if(WINVER >= 0x0400)
WINUSERAPI
int
WINAPI
CSGetKeyboardLayoutList(
        IN int nBuff,
        OUT HCSKL FAR *lpList);

WINUSERAPI
HCSKL
WINAPI
CSGetKeyboardLayout(
    IN DWORD idThread
);
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)

typedef struct tagCSMOUSEMOVEPOINT {
    int   x;
    int   y;
    DWORD time;
    ULONG_PTR dwExtraInfo;
} CSMOUSEMOVEPOINT, *PCSMOUSEMOVEPOINT, FAR* LPCSMOUSEMOVEPOINT;

/*
 * Values for resolution parameter of CSGetMouseMovePointsEx
 */
#define GMMP_USE_DISPLAY_POINTS          1
#define GMMP_USE_HIGH_RESOLUTION_POINTS  2

WINUSERAPI
int
WINAPI
CSGetMouseMovePointsEx(
    IN UINT             cbSize,
    IN LPCSMOUSEMOVEPOINT lppt,
    IN LPCSMOUSEMOVEPOINT lpptBuf,
    IN int              nBufPoints,
    IN DWORD            resolution
);

#endif /* WINVER >= 0x0500 */

#ifndef NODESKTOP
/*
 * Desktop-specific access flags
 */
#define DESKTOP_READOBJECTS         0x0001L
#define DESKTOP_CREATEWINDOW        0x0002L
#define DESKTOP_CREATEMENU          0x0004L
#define DESKTOP_HOOKCONTROL         0x0008L
#define DESKTOP_JOURNALRECORD       0x0010L
#define DESKTOP_JOURNALPLAYBACK     0x0020L
#define DESKTOP_ENUMERATE           0x0040L
#define DESKTOP_WRITEOBJECTS        0x0080L
#define DESKTOP_SWITCHDESKTOP       0x0100L

/*
 * Desktop-specific control flags
 */
#define DF_ALLOWOTHERACCOUNTHOOK    0x0001L

#ifdef _WINGDI_
#ifndef NOGDI

WINUSERAPI
HCSDESK
WINAPI
CSCreateDesktop(
    IN LPCTSTR lpszDesktop,
    IN LPCTSTR lpszDevice,
    IN LPCSDEVMODEA pDevmode,
    IN DWORD dwFlags,
    IN ACCESS_MASK dwDesiredAccess,
    IN LPCSSECURITY_ATTRIBUTES lpsa);

#endif /* NOGDI */
#endif /* _WINGDI_ */

WINUSERAPI
HCSDESK
WINAPI
CSOpenDesktop(
    IN LPCTSTR lpszDesktop,
    IN DWORD dwFlags,
    IN BOOL fInherit,
    IN ACCESS_MASK dwDesiredAccess);

WINUSERAPI
HCSDESK
WINAPI
CSOpenInputDesktop(
    IN DWORD dwFlags,
    IN BOOL fInherit,
    IN ACCESS_MASK dwDesiredAccess);

WINUSERAPI
BOOL
WINAPI
CSEnumDesktops(
    IN HCSWINSTA hwinsta,
    IN CSDESKTOPENUMPROCA lpEnumFunc,
    IN LPARAM lParam);


WINUSERAPI
BOOL
WINAPI
CSEnumDesktopWindows(
    IN HCSDESK hDesktop,
    IN CSWNDENUMPROC lpfn,
    IN LPARAM lParam);

WINUSERAPI
BOOL
WINAPI
CSSwitchDesktop(
    IN HCSDESK hDesktop);

WINUSERAPI
BOOL
WINAPI
CSSetThreadDesktop(
    IN HCSDESK hDesktop);

WINUSERAPI
BOOL
WINAPI
CSCloseDesktop(
    IN HCSDESK hDesktop);

WINUSERAPI
HCSDESK
WINAPI
CSGetThreadDesktop(
    IN DWORD dwThreadId);
#endif  /* !NODESKTOP */

#ifndef NOWINDOWSTATION
/*
 * Windowstation-specific access flags
 */
#define WINSTA_ENUMDESKTOPS         0x0001L
#define WINSTA_READATTRIBUTES       0x0002L
#define WINSTA_ACCESSCLIPBOARD      0x0004L
#define WINSTA_CREATEDESKTOP        0x0008L
#define WINSTA_WRITEATTRIBUTES      0x0010L
#define WINSTA_ACCESSGLOBALATOMS    0x0020L
#define WINSTA_EXITWINDOWS          0x0040L
#define WINSTA_ENUMERATE            0x0100L
#define WINSTA_READSCREEN           0x0200L

/*
 * Windowstation-specific attribute flags
 */
#define WSF_VISIBLE                 0x0001L

WINUSERAPI
HCSWINSTA
WINAPI
CSCreateWindowStation(
    IN LPCTSTR              lpwinsta,
    IN DWORD                 dwReserved,
    IN ACCESS_MASK           dwDesiredAccess,
    IN LPCSSECURITY_ATTRIBUTES lpsa);

WINUSERAPI
HCSWINSTA
WINAPI
CSOpenWindowStation(
    IN LPCTSTR lpszWinSta,
    IN BOOL fInherit,
    IN ACCESS_MASK dwDesiredAccess);

WINUSERAPI
BOOL
WINAPI
CSEnumWindowStations(
    IN CSWINSTAENUMPROCA lpEnumFunc,
    IN LPARAM lParam);



WINUSERAPI
BOOL
WINAPI
CSCloseWindowStation(
    IN HCSWINSTA hWinSta);

WINUSERAPI
BOOL
WINAPI
CSSetProcessWindowStation(
    IN HCSWINSTA hWinSta);

WINUSERAPI
HCSWINSTA
WINAPI
CSGetProcessWindowStation(
    void);
#endif  /* !NOWINDOWSTATION */
/*
#ifndef NOSECURITY

WINUSERAPI
BOOL
WINAPI
CSSetUserObjectSecurity(
    IN HCSHANDLE hObj,
    IN PSECURITY_INFORMATION pSIRequested,
    IN PSECURITY_DESCRIPTOR pSID);

WINUSERAPI
BOOL
WINAPI
CSGetUserObjectSecurity(
    IN HCSHANDLE hObj,
    IN PSECURITY_INFORMATION pSIRequested,
    IN OUT PSECURITY_DESCRIPTOR pSID,
    IN DWORD nLength,
    OUT LPDWORD lpnLengthNeeded);

#define UOI_FLAGS       1
#define UOI_NAME        2
#define UOI_TYPE        3
#define UOI_USER_SID    4

typedef struct tagCSUSEROBJECTFLAGS {
    BOOL fInherit;
    BOOL fReserved;
    DWORD dwFlags;
} CSUSEROBJECTFLAGS, *PCSUSEROBJECTFLAGS;

WINUSERAPI
BOOL
WINAPI
CSGetUserObjectInformation(
    IN HCSHANDLE hObj,
    IN int nIndex,
    OUT PVOID pvInfo,
    IN DWORD nLength,
    OUT LPDWORD lpnLengthNeeded);

WINUSERAPI
BOOL
WINAPI
CSSetUserObjectInformation(
    IN HCSHANDLE hObj,
    IN int nIndex,
    IN PVOID pvInfo,
    IN DWORD nLength);

#endif   //!NOSECURITY 
*/
#if(WINVER >= 0x0400)
typedef struct tagCSWNDCLASSEXA {
    UINT        cbSize;
    /* Win 3.x */
    UINT        style;
    CSWNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HCSINSTANCE   hInstance;
    HCSICON       hIcon;
    HCSCURSOR     hCursor;
    HCSBRUSH      hbrBackground;
    LPCTSTR      lpszMenuName;
    LPCTSTR      lpszClassName;
    /* Win 4.0 */
    HCSICON       hIconSm;
} CSWNDCLASSEXA, *PCSWNDCLASSEXA, NEAR *NPCSWNDCLASSEXA, FAR *LPCSWNDCLASSEXA;

typedef CSWNDCLASSEXA CSWNDCLASSEX;
typedef PCSWNDCLASSEXA PCSWNDCLASSEX;
typedef NPCSWNDCLASSEXA NPCSWNDCLASSEX;
typedef LPCSWNDCLASSEXA LPCSWNDCLASSEX;
#endif /* WINVER >= 0x0400 */

typedef struct tagCSWNDCLASSA {
    UINT        style;
    CSWNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HCSINSTANCE   hInstance;
    HCSICON       hIcon;
    HCSCURSOR     hCursor;
    HCSBRUSH      hbrBackground;
    LPCTSTR      lpszMenuName;
    LPCTSTR      lpszClassName;
} CSWNDCLASSA, *PCSWNDCLASSA, NEAR *NPCSWNDCLASSA, FAR *LPCSWNDCLASSA;

typedef CSWNDCLASSA CSWNDCLASS;
typedef PCSWNDCLASSA PCSWNDCLASS;
typedef NPCSWNDCLASSA NPCSWNDCLASS;
typedef LPCSWNDCLASSA LPCSWNDCLASS;


#ifndef NOMSG

/*
 * Message structure
 */
typedef struct tagCSMSG {
    HCSWND        hwnd;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
    DWORD       time;
    CSPOINT       pt;
#ifdef _MAC
    DWORD       lPrivate;
#endif
} CSMSG, *PCSMSG, NEAR *NPCSMSG, FAR *LPCSMSG;

#define POINTSTOPOINT(pt, pts)                          \
        { (pt).x = (LONG)(SHORT)LOWORD(*(LONG*)&pts);   \
          (pt).y = (LONG)(SHORT)HIWORD(*(LONG*)&pts); }

#define POINTTOPOINTS(pt)      (MAKELONG((short)((pt).x), (short)((pt).y)))
#define MAKEWPARAM(l, h)      ((WPARAM)(DWORD)MAKELONG(l, h))
#define MAKELPARAM(l, h)      ((LPARAM)(DWORD)MAKELONG(l, h))
#define MAKELRESULT(l, h)     ((LRESULT)(DWORD)MAKELONG(l, h))


#endif /* !NOMSG */

#ifndef NOWINOFFSETS

/*
 * Window field offsets for CSGetWindowLong()
 */
#define GWL_WNDPROC         (-4)
#define GWL_HINSTANCE       (-6)
#define GWL_HWNDPARENT      (-8)
#define GWL_STYLE           (-16)
#define GWL_EXSTYLE         (-20)
#define GWL_USERDATA        (-21)
#define GWL_ID              (-12)

#ifdef _WIN64

#undef GWL_WNDPROC
#undef GWL_HINSTANCE
#undef GWL_HWNDPARENT
#undef GWL_USERDATA

#endif /* _WIN64 */

#define GWLP_WNDPROC        (-4)
#define GWLP_HINSTANCE      (-6)
#define GWLP_HWNDPARENT     (-8)
#define GWLP_USERDATA       (-21)
#define GWLP_ID             (-12)

/*
 * Class field offsets for CSGetClassLong()
 */
#define GCL_MENUNAME        (-8)
#define GCL_HBRBACKGROUND   (-10)
#define GCL_HCURSOR         (-12)
#define GCL_HICON           (-14)
#define GCL_HMODULE         (-16)
#define GCL_CBWNDEXTRA      (-18)
#define GCL_CBCLSEXTRA      (-20)
#define GCL_WNDPROC         (-24)
#define GCL_STYLE           (-26)
#define GCW_ATOM            (-32)

#if(WINVER >= 0x0400)
#define GCL_HICONSM         (-34)
#endif /* WINVER >= 0x0400 */

#ifdef _WIN64

#undef GCL_MENUNAME
#undef GCL_HBRBACKGROUND
#undef GCL_HCURSOR
#undef GCL_HICON
#undef GCL_HMODULE
#undef GCL_WNDPROC
#undef GCL_HICONSM

#endif /* _WIN64 */

#define GCLP_MENUNAME       (-8)
#define GCLP_HBRBACKGROUND  (-10)
#define GCLP_HCURSOR        (-12)
#define GCLP_HICON          (-14)
#define GCLP_HMODULE        (-16)
#define GCLP_WNDPROC        (-24)
#define GCLP_HICONSM        (-34)

#endif /* !NOWINOFFSETS */

#ifndef NOWINMESSAGES


/*
 * Window Messages
 */

#define WM_NULL                         0x0000
#define WM_CREATE                       0x0001
#define WM_DESTROY                      0x0002
#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005

#define WM_ACTIVATE                     0x0006
/*
 * WM_ACTIVATE state values
 */
#define     WA_INACTIVE     0
#define     WA_ACTIVE       1
#define     WA_CLICKACTIVE  2

#define WM_SETFOCUS                     0x0007
#define WM_KILLFOCUS                    0x0008
#define WM_ENABLE                       0x000A
#define WM_SETREDRAW                    0x000B
#define WM_SETTEXT                      0x000C
#define WM_GETTEXT                      0x000D
#define WM_GETTEXTLENGTH                0x000E
#define WM_PAINT                        0x000F
#define WM_CLOSE                        0x0010
#ifndef _WIN32_WCE
#define WM_QUERYENDSESSION              0x0011
#define WM_QUERYOPEN                    0x0013
#define WM_ENDSESSION                   0x0016
#endif
#define WM_QUIT                         0x0012
#define WM_ERASEBKGND                   0x0014
#define WM_SYSCOLORCHANGE               0x0015
#define WM_SHOWWINDOW                   0x0018
#define WM_WININICHANGE                 0x001A
#if(WINVER >= 0x0400)
#define WM_SETTINGCHANGE                WM_WININICHANGE
#endif /* WINVER >= 0x0400 */


#define WM_DEVMODECHANGE                0x001B
#define WM_ACTIVATEAPP                  0x001C
#define WM_FONTCHANGE                   0x001D
#define WM_TIMECHANGE                   0x001E
#define WM_CANCELMODE                   0x001F
#define WM_SETCURSOR                    0x0020
#define WM_MOUSEACTIVATE                0x0021
#define WM_CHILDACTIVATE                0x0022
#define WM_QUEUESYNC                    0x0023

#define WM_GETMINMAXINFO                0x0024
/*
 * Struct pointed to by WM_GETCSMINMAXINFO lParam
 */
typedef struct tagCSMINMAXINFO {
    CSPOINT ptReserved;
    CSPOINT ptMaxSize;
    CSPOINT ptMaxPosition;
    CSPOINT ptMinTrackSize;
    CSPOINT ptMaxTrackSize;
} CSMINMAXINFO, *PCSMINMAXINFO, *LPCSMINMAXINFO;

#define WM_PAINTICON                    0x0026
#define WM_ICONERASEBKGND               0x0027
#define WM_NEXTDLGCTL                   0x0028
#define WM_SPOOLERSTATUS                0x002A
#define WM_DRAWITEM                     0x002B
#define WM_MEASUREITEM                  0x002C
#define WM_DELETEITEM                   0x002D
#define WM_VKEYTOITEM                   0x002E
#define WM_CHARTOITEM                   0x002F
#define WM_SETFONT                      0x0030
#define WM_GETFONT                      0x0031
#define WM_SETHOTKEY                    0x0032
#define WM_GETHOTKEY                    0x0033
#define WM_QUERYDRAGICON                0x0037
#define WM_COMPAREITEM                  0x0039
#if(WINVER >= 0x0500)
#ifndef _WIN32_WCE
#define WM_GETOBJECT                    0x003D
#endif
#endif /* WINVER >= 0x0500 */
#define WM_COMPACTING                   0x0041
#define WM_COMMNOTIFY                   0x0044  /* no longer suported */
#define WM_WINDOWPOSCHANGING            0x0046
#define WM_WINDOWPOSCHANGED             0x0047
/*******************************************/
//  WM_DRAWMENUFRAME:
//  it same as the WM_DRAWMENUITEM,but is only send when the popop menu has ;
//  first enter,you should paint your menu frame at here;
/***************************************** */

#define WM_DRAWMENUFRAME				0x34000000  
/*******************************************/
//  WM_MEASUREMENUFRAME:
//  the member CtlID of struc MEASUREITEMSTRUCT means :the x offset;
//  the member CtlType of struc MEASUREITEMSTRUCT means : the y offset;
/***************************************** */
#define WM_MEASUREMENUFRAME				0x35000000
/*******************************************/
//  WM_MENUREINIT:
//  when the menu should be redisplay when it has all dispeared,
//  the gui will send this message to notify the parent window to reinit the menu
/***************************************** */
#define WM_MENUREINIT					0x36000000
/*******************************************/
//  WM_MENUDESTROY:
//  when the menu will all dispeared(it means destroied)
//  the gui will send this message to notify the parent window
/***************************************** */

#define WM_MENUDESTROY					0x37000000
#define WM_POWER                        0x0048
/*
 * wParam for WM_POWER window message and DRV_POWER driver notification
 */
#define PWR_OK              1
#define PWR_FAIL            (-1)
#define PWR_SUSPENDREQUEST  1
#define PWR_SUSPENDRESUME   2
#define PWR_CRITICALRESUME  3

#define WM_COPYDATA                     0x004A
#define WM_CANCELJOURNAL                0x004B


/*
 * lParam of WM_COPYDATA message points to...
 */
typedef struct tagCSCOPYDATASTRUCT {
    ULONG_PTR dwData;
    DWORD cbData;
    PVOID lpData;
} CSCOPYDATASTRUCT, *PCSCOPYDATASTRUCT;

#if(WINVER >= 0x0400)
typedef struct tagCSMDINEXTMENU
{
    HCSMENU   hmenuIn;
    HCSMENU   hmenuNext;
    HCSWND    hwndNext;
} CSMDINEXTMENU, * PCSMDINEXTMENU, FAR * LPCSMDINEXTMENU;
#endif /* WINVER >= 0x0400 */


#if(WINVER >= 0x0400)
#define WM_NOTIFY                       0x004E
#define WM_INPUTLANGCHANGEREQUEST       0x0050
#define WM_INPUTLANGCHANGE              0x0051
#define WM_TCARD                        0x0052
#define WM_HELP                         0x0053
#define WM_USERCHANGED                  0x0054
#define WM_NOTIFYFORMAT                 0x0055

#define NFR_ANSI                             1
#define NFR_UNICODE                          2
#define NF_QUERY                             3
#define NF_REQUERY                           4

#define WM_CONTEXTMENU                  0x007B
#define WM_STYLECHANGING                0x007C
#define WM_STYLECHANGED                 0x007D
#define WM_DISPLAYCHANGE                0x007E
#define WM_GETICON                      0x007F
#define WM_SETICON                      0x0080
#endif /* WINVER >= 0x0400 */

#define WM_NCCREATE                     0x0081
#define WM_NCDESTROY                    0x0082
#define WM_NCCALCSIZE                   0x0083
#define WM_NCHITTEST                    0x0084
#define WM_NCPAINT                      0x0085
#define WM_NCACTIVATE                   0x0086
#define WM_GETDLGCODE                   0x0087
#ifndef _WIN32_WCE
#define WM_SYNCPAINT                    0x0088
#endif
#define WM_NCMOUSEMOVE                  0x00A0
#define WM_NCLBUTTONDOWN                0x00A1
#define WM_NCLBUTTONUP                  0x00A2
#define WM_NCLBUTTONDBLCLK              0x00A3
#define WM_NCRBUTTONDOWN                0x00A4
#define WM_NCRBUTTONUP                  0x00A5
#define WM_NCRBUTTONDBLCLK              0x00A6
#define WM_NCMBUTTONDOWN                0x00A7
#define WM_NCMBUTTONUP                  0x00A8
#define WM_NCMBUTTONDBLCLK              0x00A9



#if(_WIN32_WINNT >= 0x0500)
#define WM_NCXBUTTONDOWN                0x00AB
#define WM_NCXBUTTONUP                  0x00AC
#define WM_NCXBUTTONDBLCLK              0x00AD
#endif /* _WIN32_WINNT >= 0x0500 */


#if(_WIN32_WINNT >= 0x0501)
#define WM_INPUT                        0x00FF
#endif /* _WIN32_WINNT >= 0x0501 */

#define WM_KEYFIRST                     0x0100
#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101
#define WM_CHAR                         0x0102
#define WM_DEADCHAR                     0x0103
#define WM_SYSKEYDOWN                   0x0104
#define WM_SYSKEYUP                     0x0105
#define WM_SYSCHAR                      0x0106
#define WM_SYSDEADCHAR                  0x0107
#if(_WIN32_WINNT >= 0x0501)
#define WM_UNICHAR                      0x0109
#define WM_KEYLAST                      0x0109
#define UNICODE_NOCHAR                  0xFFFF
#else
#define WM_KEYLAST                      0x0108
#endif /* _WIN32_WINNT >= 0x0501 */

#if(WINVER >= 0x0400)
#define WM_IME_STARTCOMPOSITION         0x010D
#define WM_IME_ENDCOMPOSITION           0x010E
#define WM_IME_COMPOSITION              0x010F
#define WM_IME_KEYLAST                  0x010F
#endif /* WINVER >= 0x0400 */

#define WM_INITDIALOG                   0x0110
#define WM_COMMAND                      0x0111
#define WM_SYSCOMMAND                   0x0112
#define WM_TIMER                        0x0113
#define WM_HSCROLL                      0x0114
#define WM_VSCROLL                      0x0115
#define WM_INITMENU                     0x0116
#define WM_INITMENUPOPUP                0x0117
#define WM_MENUSELECT                   0x011F
#define WM_MENUCHAR                     0x0120
#define WM_ENTERIDLE                    0x0121
#if(WINVER >= 0x0500)
#ifndef _WIN32_WCE
#define WM_MENURBUTTONUP                0x0122
#define WM_MENUDRAG                     0x0123
#define WM_MENUGETOBJECT                0x0124
#define WM_UNINITMENUPOPUP              0x0125
#define WM_MENUCOMMAND                  0x0126

#ifndef _WIN32_WCE
#if(_WIN32_WINNT >= 0x0500)
#define WM_CHANGEUISTATE                0x0127
#define WM_UPDATEUISTATE                0x0128
#define WM_QUERYUISTATE                 0x0129

/*
 * LOWORD(wParam) values in WM_*UISTATE*
 */
#define UIS_SET                         1
#define UIS_CLEAR                       2
#define UIS_INITIALIZE                  3

/*
 * HIWORD(wParam) values in WM_*UISTATE*
 */
#define UISF_HIDEFOCUS                  0x1
#define UISF_HIDEACCEL                  0x2
#if(_WIN32_WINNT >= 0x0501)
#define UISF_ACTIVE                     0x4
#endif /* _WIN32_WINNT >= 0x0501 */
#endif /* _WIN32_WINNT >= 0x0500 */
#endif

#endif
#endif /* WINVER >= 0x0500 */

#define WM_CTLCOLORMSGBOX               0x0132
#define WM_CTLCOLOREDIT                 0x0133
#define WM_CTLCOLORLISTBOX              0x0134
#define WM_CTLCOLORBTN                  0x0135
#define WM_CTLCOLORDLG                  0x0136
#define WM_CTLCOLORSCROLLBAR            0x0137
#define WM_CTLCOLORSTATIC               0x0138
#define WM_CTLCOLORMENU					0x0139

#define WM_MOUSEFIRST                   0x0200
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
#define WM_MOUSEWHEEL                   0x020A
#endif
#if (_WIN32_WINNT >= 0x0500)
#define WM_XBUTTONDOWN                  0x020B
#define WM_XBUTTONUP                    0x020C
#define WM_XBUTTONDBLCLK                0x020D
#endif
#undef WM_MOUSELAST
#if (_WIN32_WINNT >= 0x0500)
#define WM_MOUSELAST                    0x020D
#elif (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
#undef WM_MOUSELAST
#define WM_MOUSELAST                    0x020A
#else
#undef WM_MOUSELAST
#define WM_MOUSELAST                    0x0209
#endif /* (_WIN32_WINNT >= 0x0500) */


#if(_WIN32_WINNT >= 0x0400)
/* Value for rolling one detent */
#define WHEEL_DELTA                     120
#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))

/* CSSetting to scroll one page for SPI_GET/SETWHEELSCROLLLINES */
#define WHEEL_PAGESCROLL                (UINT_MAX)
#endif /* _WIN32_WINNT >= 0x0400 */

#if(_WIN32_WINNT >= 0x0500)
#define GET_KEYSTATE_WPARAM(wParam)     (LOWORD(wParam))
#define GET_NCHITTEST_WPARAM(wParam)    ((short)LOWORD(wParam))
#define GET_XBUTTON_WPARAM(wParam)      (HIWORD(wParam))

/* XButton values are WORD flags */
#define XBUTTON1      0x0001
#define XBUTTON2      0x0002
/* Were there to be an XBUTTON3, its value would be 0x0004 */
#endif /* _WIN32_WINNT >= 0x0500 */

#define WM_PARENTNOTIFY                 0x0210
#define WM_ENTERMENULOOP                0x0211
#define WM_EXITMENULOOP                 0x0212

#if(WINVER >= 0x0400)
#define WM_NEXTMENU                     0x0213
#define WM_SIZING                       0x0214
#define WM_CAPTURECHANGED               0x0215
#define WM_MOVING                       0x0216
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0400)


#define WM_POWERBROADCAST               0x0218

#ifndef _WIN32_WCE
#define PBT_APMQUERYSUSPEND             0x0000
#define PBT_APMQUERYSTANDBY             0x0001

#define PBT_APMQUERYSUSPENDFAILED       0x0002
#define PBT_APMQUERYSTANDBYFAILED       0x0003

#define PBT_APMSUSPEND                  0x0004
#define PBT_APMSTANDBY                  0x0005

#define PBT_APMRESUMECRITICAL           0x0006
#define PBT_APMRESUMESUSPEND            0x0007
#define PBT_APMRESUMESTANDBY            0x0008

#define PBTF_APMRESUMEFROMFAILURE       0x00000001

#define PBT_APMBATTERYLOW               0x0009
#define PBT_APMPOWERSTATUSCHANGE        0x000A

#define PBT_APMOEMEVENT                 0x000B
#define PBT_APMRESUMEAUTOMATIC          0x0012
#endif

#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0400)
#define WM_DEVICECHANGE                 0x0219
#endif /* WINVER >= 0x0400 */

#define WM_MDICREATE                    0x0220
#define WM_MDIDESTROY                   0x0221
#define WM_MDIACTIVATE                  0x0222
#define WM_MDIRESTORE                   0x0223
#define WM_MDINEXT                      0x0224
#define WM_MDIMAXIMIZE                  0x0225
#define WM_MDITILE                      0x0226
#define WM_MDICASCADE                   0x0227
#define WM_MDIICONARRANGE               0x0228
#define WM_MDIGETACTIVE                 0x0229


#define WM_MDISETMENU                   0x0230
#define WM_ENTERSIZEMOVE                0x0231
#define WM_EXITSIZEMOVE                 0x0232
#define WM_DROPFILES                    0x0233
#define WM_MDIREFRESHCSMENU               0x0234


#if(WINVER >= 0x0400)
#define WM_IME_SETCONTEXT               0x0281
#define WM_IME_NOTIFY                   0x0282
#define WM_IME_CONTROL                  0x0283
#define WM_IME_COMPOSITIONFULL          0x0284
#define WM_IME_SELECT                   0x0285
#define WM_IME_CHAR                     0x0286
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
#define WM_IME_REQUEST                  0x0288
#endif /* WINVER >= 0x0500 */
#if(WINVER >= 0x0400)
#define WM_IME_KEYDOWN                  0x0290
#define WM_IME_KEYUP                    0x0291
#endif /* WINVER >= 0x0400 */

#if((_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500))
#define WM_MOUSEHOVER                   0x02A1
#define WM_MOUSELEAVE                   0x02A3
#endif
#if(WINVER >= 0x0500)
#define WM_NCMOUSEHOVER                 0x02A0
#define WM_NCMOUSELEAVE                 0x02A2
#endif /* WINVER >= 0x0500 */

#if(_WIN32_WINNT >= 0x0501)
#define WM_WTSSESSION_CHANGE            0x02B1

#define WM_TABLET_FIRST                 0x02c0
#define WM_TABLET_LAST                  0x02df
#endif /* _WIN32_WINNT >= 0x0501 */

#define WM_CUT                          0x0300
#define WM_COPY                         0x0301
#define WM_PASTE                        0x0302
#define WM_CLEAR                        0x0303
#define WM_UNDO                         0x0304
#define WM_RENDERFORMAT                 0x0305
#define WM_RENDERALLFORMATS             0x0306
#define WM_DESTROYCLIPBOARD             0x0307
#define WM_DRAWCLIPBOARD                0x0308
#define WM_PAINTCLIPBOARD               0x0309
#define WM_VSCROLLCLIPBOARD             0x030A
#define WM_SIZECLIPBOARD                0x030B
#define WM_ASKCBFORMATNAME              0x030C
#define WM_CHANGECBCHAIN                0x030D
#define WM_HSCROLLCLIPBOARD             0x030E
#define WM_QUERYNEWPALETTE              0x030F
#define WM_PALETTEISCHANGING            0x0310
#define WM_PALETTECHANGED               0x0311
#define WM_HOTKEY                       0x0312

#if(WINVER >= 0x0400)
#define WM_PRINT                        0x0317
#define WM_PRINTCLIENT                  0x0318
#endif /* WINVER >= 0x0400 */

#if(_WIN32_WINNT >= 0x0500)
#define WM_APPCOMMAND                   0x0319
#endif /* _WIN32_WINNT >= 0x0500 */

#if(_WIN32_WINNT >= 0x0501)
#define WM_THEMECHANGED                 0x031A
#endif /* _WIN32_WINNT >= 0x0501 */


#if(WINVER >= 0x0400)

#define WM_HANDHELDFIRST                0x0358
#define WM_HANDHELDLAST                 0x035F

#define WM_AFXFIRST                     0x0360
#define WM_AFXLAST                      0x037F
#endif /* WINVER >= 0x0400 */

#define WM_PENWINFIRST                  0x0380
#define WM_PENWINLAST                   0x038F


#if(WINVER >= 0x0400)
#define WM_APP                          0x8000
#endif /* WINVER >= 0x0400 */


/*
 * NOTE: All Message Numbers below 0x0400 are RESERVED.
 *
 * Private Window Messages Start Here:
 */
#define WM_USER                         0x0400

#if(WINVER >= 0x0400)

/*  wParam for WM_SIZING message  */
#define WMSZ_LEFT           1
#define WMSZ_RIGHT          2
#define WMSZ_TOP            3
#define WMSZ_TOPLEFT        4
#define WMSZ_TOPRIGHT       5
#define WMSZ_BOTTOM         6
#define WMSZ_BOTTOMLEFT     7
#define WMSZ_BOTTOMRIGHT    8
#endif /* WINVER >= 0x0400 */

#ifndef NONCMESSAGES

/*
 * WM_NCHITTEST and CSMOUSEHOOKSTRUCT Mouse Position Codes
 */
#define HTERROR             (-2)
#define HTTRANSPARENT       (-1)
#define HTNOWHERE           0
#define HTCLIENT            1
#define HTCAPTION           2
#define HTSYSMENU           3
#define HTGROWBOX           4
#define HTSIZE              HTGROWBOX
#define HTMENU              5
#define HTHSCROLL           6
#define HTVSCROLL           7
#define HTMINBUTTON         8
#define HTMAXBUTTON         9
#define HTLEFT              10
#define HTRIGHT             11
#define HTTOP               12
#define HTTOPLEFT           13
#define HTTOPRIGHT          14
#define HTBOTTOM            15
#define HTBOTTOMLEFT        16
#define HTBOTTOMRIGHT       17
#define HTBORDER            18
#define HTREDUCE            HTMINBUTTON
#define HTZOOM              HTMAXBUTTON
#define HTSIZEFIRST         HTLEFT
#define HTSIZELAST          HTBOTTOMRIGHT
#if(WINVER >= 0x0400)
#define HTOBJECT            19
#define HTCLOSE             20
#define HTHELP              21
#endif /* WINVER >= 0x0400 */


/*
 * CSSendMessageTimeout values
 */
#define SMTO_NORMAL         0x0000
#define SMTO_BLOCK          0x0001
#define SMTO_ABORTIFHUNG    0x0002
#if(WINVER >= 0x0500)
#define SMTO_NOTIMEOUTIFNOTHUNG 0x0008
#endif /* WINVER >= 0x0500 */
#endif /* !NONCMESSAGES */

/*
 * WM_MOUSEACTIVATE Return Codes
 */
#define MA_ACTIVATE         1
#define MA_ACTIVATEANDEAT   2
#define MA_NOACTIVATE       3
#define MA_NOACTIVATEANDEAT 4

/*
 * WM_SETICON / WM_GETICON Type Codes
 */
#define ICON_SMALL          0
#define ICON_BIG            1
#if(_WIN32_WINNT >= 0x0501)
#define ICON_SMALL2         2
#endif /* _WIN32_WINNT >= 0x0501 */


WINUSERAPI
UINT
WINAPI
CSRegisterWindowMessage(
    IN LPCTSTR lpString);



/*
 * WM_SIZE message wParam values
 */
#define SIZE_RESTORED       0
#define SIZE_MINIMIZED      1
#define SIZE_MAXIMIZED      2
#define SIZE_MAXSHOW        3
#define SIZE_MAXHIDE        4

/*
 * Obsolete constant names
 */
#define SIZENORMAL          SIZE_RESTORED
#define SIZEICONIC          SIZE_MINIMIZED
#define SIZEFULLSCREEN      SIZE_MAXIMIZED
#define SIZEZOOMSHOW        SIZE_MAXSHOW
#define SIZEZOOMHIDE        SIZE_MAXHIDE

/*
 * WM_WINDOWPOSCHANGING/CHANGED struct pointed to by lParam
 */
typedef struct tagCSWINDOWPOS {
    HCSWND    hwnd;
    HCSWND    hwndInsertAfter;
    int     x;
    int     y;
    int     cx;
    int     cy;
    UINT    flags;
} CSWINDOWPOS, *LPCSWINDOWPOS, *PCSWINDOWPOS;

/*
 * WM_NCCALCSIZE parameter structure
 */
typedef struct tagCSNCCALCSIZE_PARAMS {
    CSRECT       rgrc[3];
    PCSWINDOWPOS lppos;
} CSNCCALCSIZE_PARAMS, *LPCSNCCALCSIZE_PARAMS;

/*
 * WM_NCCALCSIZE "window valid rect" return values
 */
#define WVR_ALIGNTOP        0x0010
#define WVR_ALIGNLEFT       0x0020
#define WVR_ALIGNBOTTOM     0x0040
#define WVR_ALIGNRIGHT      0x0080
#define WVR_HREDRAW         0x0100
#define WVR_VREDRAW         0x0200
#define WVR_REDRAW         (WVR_HREDRAW | \
                            WVR_VREDRAW)
#define WVR_VALIDRECTS      0x0400


#ifndef NOKEYSTATES

/*
 * Key State Masks for Mouse Messages
 */
#define MK_LBUTTON          0x0001
#define MK_RBUTTON          0x0002
#define MK_SHIFT            0x0004
#define MK_CONTROL          0x0008
#define MK_MBUTTON          0x0010
#if(_WIN32_WINNT >= 0x0500)
#define MK_XBUTTON1         0x0020
#define MK_XBUTTON2         0x0040
#endif /* _WIN32_WINNT >= 0x0500 */

#endif /* !NOKEYSTATES */


#if(_WIN32_WINNT >= 0x0400)
#ifndef NOCSTRACKMOUSEEVENT

#define TME_HOVER       0x00000001
#define TME_LEAVE       0x00000002
#if(WINVER >= 0x0500)
#define TME_NONCLIENT   0x00000010
#endif /* WINVER >= 0x0500 */
#define TME_QUERY       0x40000000
#define TME_CANCEL      0x80000000


#define HOVER_DEFAULT   0xFFFFFFFF
#endif /* _WIN32_WINNT >= 0x0400 */

#if(_WIN32_WINNT >= 0x0400)
typedef struct tagCSTRACKMOUSEEVENT {
    DWORD cbSize;
    DWORD dwFlags;
    HCSWND  hwndTrack;
    DWORD dwHoverTime;
} CSTRACKMOUSEEVENT, *LPCSTRACKMOUSEEVENT;

WINUSERAPI
BOOL
WINAPI
CSTrackMouseEvent(
    IN OUT LPCSTRACKMOUSEEVENT lpEventTrack);
#endif /* _WIN32_WINNT >= 0x0400 */

#if(_WIN32_WINNT >= 0x0400)

#endif /* !NOCSTRACKMOUSEEVENT */
#endif /* _WIN32_WINNT >= 0x0400 */


#endif /* !NOWINMESSAGES */

#ifndef NOWINSTYLES


/*
 * Window Styles
 */
#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L

#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L


#define WS_TILED            WS_OVERLAPPED
#define WS_ICONIC           WS_MINIMIZE
#define WS_SIZEBOX          WS_THICKFRAME
#define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW

/*
 * Common Window Styles
 */
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)

#define WS_POPUPWINDOW      (WS_POPUP          | \
                             WS_BORDER         | \
                             WS_SYSMENU)

#define WS_CHILDWINDOW      (WS_CHILD)

/*
 * Extended Window Styles
 */
#define WS_EX_DLGMODALFRAME     0x00000001L
#define WS_EX_NOPARENTNOTIFY    0x00000004L
#define WS_EX_TOPMOST           0x00000008L
#define WS_EX_ACCEPTFILES       0x00000010L
#define WS_EX_TRANSPARENT       0x00000020L
#if(WINVER >= 0x0400)
#define WS_EX_MDICHILD          0x00000040L
#define WS_EX_TOOLWINDOW        0x00000080L
#define WS_EX_WINDOWEDGE        0x00000100L
#define WS_EX_CLIENTEDGE        0x00000200L
#define WS_EX_CONTEXTHELP       0x00000400L

#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0400)

#define WS_EX_RIGHT             0x00001000L
#define WS_EX_LEFT              0x00000000L
#define WS_EX_RTLREADING        0x00002000L
#define WS_EX_LTRREADING        0x00000000L
#define WS_EX_LEFTSCROLLBAR     0x00004000L
#define WS_EX_RIGHTSCROLLBAR    0x00000000L

#define WS_EX_CONTROLPARENT     0x00010000L
#define WS_EX_STATICEDGE        0x00020000L
#define WS_EX_APPWINDOW         0x00040000L


#define WS_EX_OVERLAPPEDWINDOW  (WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)
#define WS_EX_PALETTEWINDOW     (WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)

#endif /* WINVER >= 0x0400 */

#if(_WIN32_WINNT >= 0x0500)
#define WS_EX_LAYERED           0x00080000

#endif /* _WIN32_WINNT >= 0x0500 */


#if(WINVER >= 0x0500)
#define WS_EX_NOINHERITLAYOUT   0x00100000L // Disable inheritence of mirroring by children
#define WS_EX_LAYOUTRTL         0x00400000L // Right to left mirroring
#endif /* WINVER >= 0x0500 */

#if(_WIN32_WINNT >= 0x0501)
#define WS_EX_COMPOSITED        0x02000000L
#endif /* _WIN32_WINNT >= 0x0501 */
#if(_WIN32_WINNT >= 0x0500)
#define WS_EX_NOACTIVATE        0x08000000L
#endif /* _WIN32_WINNT >= 0x0500 */


/*
 * Class styles
 */
#define CS_VREDRAW          0x0001
#define CS_HREDRAW          0x0002
#define CS_DBLCLKS          0x0008
#define CS_OWNDC            0x0020
#define CS_CLASSDC          0x0040
#define CS_PARENTDC         0x0080
#define CS_NOCLOSE          0x0200
#define CS_SAVEBITS         0x0800
#define CS_BYTEALIGNCLIENT  0x1000
#define CS_BYTEALIGNWINDOW  0x2000
#define CS_GLOBALCLASS      0x4000

#define CS_IME              0x00010000
#if(_WIN32_WINNT >= 0x0501)
#define CS_DROPSHADOW       0x00020000
#endif /* _WIN32_WINNT >= 0x0501 */



#endif /* !NOWINSTYLES */
#if(WINVER >= 0x0400)
/* WM_PRINT flags */
#define PRF_CHECKVISIBLE    0x00000001L
#define PRF_NONCLIENT       0x00000002L
#define PRF_CLIENT          0x00000004L
#define PRF_ERASEBKGND      0x00000008L
#define PRF_CHILDREN        0x00000010L
#define PRF_OWNED           0x00000020L

/* 3D border styles */
#define BDR_RAISEDOUTER 0x0001
#define BDR_SUNKENOUTER 0x0002
#define BDR_RAISEDINNER 0x0004
#define BDR_SUNKENINNER 0x0008

#define BDR_OUTER       (BDR_RAISEDOUTER | BDR_SUNKENOUTER)
#define BDR_INNER       (BDR_RAISEDINNER | BDR_SUNKENINNER)
#define BDR_RAISED      (BDR_RAISEDOUTER | BDR_RAISEDINNER)
#define BDR_SUNKEN      (BDR_SUNKENOUTER | BDR_SUNKENINNER)


#define EDGE_RAISED     (BDR_RAISEDOUTER | BDR_RAISEDINNER)
#define EDGE_SUNKEN     (BDR_SUNKENOUTER | BDR_SUNKENINNER)
#define EDGE_ETCHED     (BDR_SUNKENOUTER | BDR_RAISEDINNER)
#define EDGE_BUMP       (BDR_RAISEDOUTER | BDR_SUNKENINNER)

/* Border flags */
#define BF_LEFT         0x0001
#define BF_TOP          0x0002
#define BF_RIGHT        0x0004
#define BF_BOTTOM       0x0008

#define BF_TOPLEFT      (BF_TOP | BF_LEFT)
#define BF_TOPRIGHT     (BF_TOP | BF_RIGHT)
#define BF_BOTTOMLEFT   (BF_BOTTOM | BF_LEFT)
#define BF_BOTTOMRIGHT  (BF_BOTTOM | BF_RIGHT)
#define BF_RECT         (BF_LEFT | BF_TOP | BF_RIGHT | BF_BOTTOM)

#define BF_DIAGONAL     0x0010

// For diagonal lines, the BF_RECT flags specify the end point of the
// vector bounded by the rectangle parameter.
#define BF_DIAGONAL_ENDTOPRIGHT     (BF_DIAGONAL | BF_TOP | BF_RIGHT)
#define BF_DIAGONAL_ENDTOPLEFT      (BF_DIAGONAL | BF_TOP | BF_LEFT)
#define BF_DIAGONAL_ENDBOTTOMLEFT   (BF_DIAGONAL | BF_BOTTOM | BF_LEFT)
#define BF_DIAGONAL_ENDBOTTOMRIGHT  (BF_DIAGONAL | BF_BOTTOM | BF_RIGHT)


#define BF_MIDDLE       0x0800  /* Fill in the middle */
#define BF_SOFT         0x1000  /* For softer buttons */
#define BF_ADJUST       0x2000  /* Calculate the space left over */
#define BF_FLAT         0x4000  /* For flat rather than 3D borders */
#define BF_MONO         0x8000  /* For monochrome borders */


WINUSERAPI
BOOL
WINAPI
CSDrawEdge(
    IN HCSDC hdc,
    IN OUT LPCSRECT qrc,
    IN UINT edge,
    IN UINT grfFlags);

/* flags for CSDrawFrameControl */

#define DFC_CAPTION             1
#define DFC_MENU                2
#define DFC_SCROLL              3
#define DFC_BUTTON              4
#if(WINVER >= 0x0500)
#define DFC_POPUPMENU           5
#endif /* WINVER >= 0x0500 */

#define DFCS_CAPTIONCLOSE       0x0000
#define DFCS_CAPTIONMIN         0x0001
#define DFCS_CAPTIONMAX         0x0002
#define DFCS_CAPTIONRESTORE     0x0003
#define DFCS_CAPTIONHELP        0x0004

#define DFCS_MENUARROW          0x0000
#define DFCS_MENUCHECK          0x0001
#define DFCS_MENUBULLET         0x0002
#define DFCS_MENUARROWRIGHT     0x0004
#define DFCS_SCROLLUP           0x0000
#define DFCS_SCROLLDOWN         0x0001
#define DFCS_SCROLLLEFT         0x0002
#define DFCS_SCROLLRIGHT        0x0003
#define DFCS_SCROLLCOMBOBOX     0x0005
#define DFCS_SCROLLSIZEGRIP     0x0008
#define DFCS_SCROLLSIZEGRIPRIGHT 0x0010

#define DFCS_BUTTONCHECK        0x0000
#define DFCS_BUTTONRADIOIMAGE   0x0001
#define DFCS_BUTTONRADIOMASK    0x0002
#define DFCS_BUTTONRADIO        0x0004
#define DFCS_BUTTON3STATE       0x0008
#define DFCS_BUTTONPUSH         0x0010

#define DFCS_INACTIVE           0x0100
#define DFCS_PUSHED             0x0200
#define DFCS_CHECKED            0x0400

#if(WINVER >= 0x0500)
#define DFCS_TRANSPARENT        0x0800
#define DFCS_HOT                0x1000
#endif /* WINVER >= 0x0500 */

#define DFCS_ADJUSTRECT         0x2000
#define DFCS_FLAT               0x4000
#define DFCS_MONO               0x8000

WINUSERAPI
BOOL
WINAPI
CSDrawFrameControl(
    IN HCSDC,
    IN OUT LPCSRECT,
    IN UINT,
    IN UINT);


/* flags for CSDrawCaption */
#define DC_ACTIVE           0x0001
#define DC_SMALLCAP         0x0002
#define DC_ICON             0x0004
#define DC_TEXT             0x0008
#define DC_INBUTTON         0x0010
#if(WINVER >= 0x0500)
#define DC_GRADIENT         0x0020
#endif /* WINVER >= 0x0500 */
#if(_WIN32_WINNT >= 0x0501)
#define DC_BUTTONS          0x1000
#endif /* _WIN32_WINNT >= 0x0501 */

WINUSERAPI
BOOL
WINAPI
CSDrawCaption(IN HCSWND, IN HCSDC, IN CONST CSRECT *, IN UINT);


#define IDANI_OPEN          1
#define IDANI_CAPTION       3

WINUSERAPI
BOOL
WINAPI
CSDrawAnimatedRects(
    IN HCSWND hwnd,
    IN int idAni,
    IN CONST CSRECT * lprcFrom,
    IN CONST CSRECT * lprcTo);

#endif /* WINVER >= 0x0400 */

#ifndef NOCLIPBOARD


/*
 * Predefined Clipboard Formats
 */
#define CF_TEXT             1
#define CF_BITMAP           2
#define CF_METAFILEPICT     3
#define CF_SYLK             4
#define CF_DIF              5
#define CF_TIFF             6
#define CF_OEMTEXT          7
#define CF_DIB              8
#define CF_PALETTE          9
#define CF_PENDATA          10
#define CF_RIFF             11
#define CF_WAVE             12
#define CF_UNICODETEXT      13
#define CF_ENHMETAFILE      14
#if(WINVER >= 0x0400)
#define CF_HDROP            15
#define CF_LOCALE           16
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
#define CF_DIBV5            17
#endif /* WINVER >= 0x0500 */

#if(WINVER >= 0x0500)
#define CF_MAX              18
#elif(WINVER >= 0x0400)
#define CF_MAX              17
#else
#define CF_MAX              15
#endif

#define CF_OWNERDISPLAY     0x0080
#define CF_DSPTEXT          0x0081
#define CF_DSPBITMAP        0x0082
#define CF_DSPMETAFILEPICT  0x0083
#define CF_DSPENHMETAFILE   0x008E

/*
 * "Private" formats don't get GlobalFree()'d
 */
#define CF_PRIVATEFIRST     0x0200
#define CF_PRIVATELAST      0x02FF

/*
 * "GDIOBJ" formats do get CSDeleteObject()'d
 */
#define CF_GDIOBJFIRST      0x0300
#define CF_GDIOBJLAST       0x03FF


#endif /* !NOCLIPBOARD */

/*
 * CSDefines for the fVirt field of the Accelerator table structure.
 */
#define FVIRTKEY  TRUE          /* Assumed to be == TRUE */
#define FNOINVERT 0x02
#define FSHIFT    0x04
#define FCONTROL  0x08
#define FALT      0x10

typedef struct tagCSACCEL {
#ifndef _MAC
    BYTE   fVirt;               /* Also called the flags field */
    WORD   key;
    WORD   cmd;
#else
    WORD   fVirt;               /* Also called the flags field */
    WORD   key;
    DWORD  cmd;
#endif
} CSACCEL, *LPCSACCEL;

typedef struct tagCSPAINTSTRUCT {
    HCSDC         hdc;
    BOOL        fErase;
    CSRECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} CSPAINTSTRUCT, *PCSPAINTSTRUCT, *NPCSPAINTSTRUCT, *LPCSPAINTSTRUCT;

typedef struct tagCSCREATESTRUCTA {
    LPVOID      lpCreateParams;
    HCSINSTANCE   hInstance;
    HCSMENU       hMenu;
    HCSWND        hwndParent;
    int         cy;
    int         cx;
    int         y;
    int         x;
    LONG        style;
    LPCTSTR      lpszName;
    LPCTSTR      lpszClass;
    DWORD       dwExStyle;
} CSCREATESTRUCTA, *LPCSCREATESTRUCTA;

typedef CSCREATESTRUCTA CSCREATESTRUCT;
typedef LPCSCREATESTRUCTA LPCSCREATESTRUCT;

typedef struct tagCSWINDOWPLACEMENT {
    UINT  length;
    UINT  flags;
    UINT  showCmd;
    CSPOINT ptMinPosition;
    CSPOINT ptMaxPosition;
    CSRECT  rcNormalPosition;
#ifdef _MAC
    CSRECT  rcDevice;
#endif
} CSWINDOWPLACEMENT;
typedef CSWINDOWPLACEMENT *PCSWINDOWPLACEMENT, *LPCSWINDOWPLACEMENT;

#define WPF_SETMINPOSITION          0x0001
#define WPF_RESTORETOMAXIMIZED      0x0002
#if(_WIN32_WINNT >= 0x0500)
#define WPF_ASYNCCSWINDOWPLACEMENT    0x0004
#endif /* _WIN32_WINNT >= 0x0500 */

#if(WINVER >= 0x0400)
typedef struct tagCSNMHDR
{
    HCSWND      hwndFrom;
    UINT_PTR  idFrom;
    UINT      code;         // NM_ code
}   CSNMHDR;
typedef CSNMHDR FAR * LPCSNMHDR;

typedef struct tagCSSTYLESTRUCT
{
    DWORD   styleOld;
    DWORD   styleNew;
} CSSTYLESTRUCT, * LPCSSTYLESTRUCT;
#endif /* WINVER >= 0x0400 */


/*
 * Owner draw control types
 */
#define ODT_MENU        1
#define ODT_LISTBOX     2
#define ODT_COMBOBOX    3
#define ODT_BUTTON      4
#if(WINVER >= 0x0400)
#define ODT_STATIC      5
#endif /* WINVER >= 0x0400 */

/*
 * Owner draw actions
 */
#define ODA_DRAWENTIRE  0x0001
#define ODA_SELECT      0x0002
#define ODA_FOCUS       0x0004

/*
 * Owner draw state
 */
#define ODS_SELECTED    0x0001
#define ODS_GRAYED      0x0002
#define ODS_DISABLED    0x0004
#define ODS_CHECKED     0x0008
#define ODS_FOCUS       0x0010
#if(WINVER >= 0x0400)
#define ODS_DEFAULT         0x0020
#define ODS_COMBOBOXEDIT    0x1000
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
#define ODS_HOTLIGHT        0x0040
#define ODS_INACTIVE        0x0080
#if(_WIN32_WINNT >= 0x0500)
#define ODS_NOACCEL         0x0100
#define ODS_NOFOCUSRECT     0x0200
#endif /* _WIN32_WINNT >= 0x0500 */
#endif /* WINVER >= 0x0500 */

/*
 * CSMEASUREITEMSTRUCT for ownerdraw
 */
typedef struct tagCSMEASUREITEMSTRUCT {
    UINT       CtlType;
    UINT       CtlID;
    UINT       itemID;
    UINT       itemWidth;
    UINT       itemHeight;
    ULONG_PTR  itemData;
} CSMEASUREITEMSTRUCT, NEAR *PCSMEASUREITEMSTRUCT, FAR *LPCSMEASUREITEMSTRUCT;


/*
 * CSDRAWITEMSTRUCT for ownerdraw
 */
typedef struct tagCSDRAWITEMSTRUCT {
    UINT        CtlType;
    UINT        CtlID;
    UINT        itemID;
    UINT        itemAction;
    UINT        itemState;
    HCSWND        hwndItem;
    HCSDC         hDC;
    CSRECT        rcItem;
    ULONG_PTR   itemData;
} CSDRAWITEMSTRUCT, NEAR *PCSDRAWITEMSTRUCT, FAR *LPCSDRAWITEMSTRUCT;

/*
 * CSDELETEITEMSTRUCT for ownerdraw
 */
typedef struct tagCSDELETEITEMSTRUCT {
    UINT       CtlType;
    UINT       CtlID;
    UINT       itemID;
    HCSWND       hwndItem;
    ULONG_PTR  itemData;
} CSDELETEITEMSTRUCT, NEAR *PCSDELETEITEMSTRUCT, FAR *LPCSDELETEITEMSTRUCT;

/*
 * COMPAREITEMSTUCT for ownerdraw sorting
 */
typedef struct tagCSCOMPAREITEMSTRUCT {
    UINT        CtlType;
    UINT        CtlID;
    HCSWND        hwndItem;
    UINT        itemID1;
    ULONG_PTR   itemData1;
    UINT        itemID2;
    ULONG_PTR   itemData2;
    DWORD       dwLocaleId;
} CSCOMPAREITEMSTRUCT, NEAR *PCSCOMPAREITEMSTRUCT, FAR *LPCSCOMPAREITEMSTRUCT;

#ifndef NOMSG

/*
 * Message Function Templates
 */

WINUSERAPI
BOOL
WINAPI
CSGetMessage(
    OUT LPCSMSG lpMsg,
    IN HCSWND hWnd,
    IN UINT wMsgFilterMin,
    IN UINT wMsgFilterMax);


WINUSERAPI
BOOL
WINAPI
CSTranslateMessage(
    IN CONST CSMSG *lpMsg);

WINUSERAPI
LRESULT
WINAPI
CSDispatchMessage(
    IN CONST CSMSG *lpMsg);

WINUSERAPI
BOOL
WINAPI
CSSetMessageQueue(
    IN int cMessagesMax);

WINUSERAPI
BOOL
WINAPI
CSPeekMessage(
    OUT LPCSMSG lpMsg,
    IN HCSWND hWnd,
    IN UINT wMsgFilterMin,
    IN UINT wMsgFilterMax,
    IN UINT wRemoveMsg);


/*
 * CSPeekMessage() Options
 */
#define PM_NOREMOVE         0x0000
#define PM_REMOVE           0x0001
#define PM_NOYIELD          0x0002
#if(WINVER >= 0x0500)
#define PM_QS_INPUT         (QS_INPUT << 16)
#define PM_QS_POSTMESSAGE   ((QS_POSTMESSAGE | QS_HOTKEY | QS_TIMER) << 16)
#define PM_QS_PAINT         (QS_PAINT << 16)
#define PM_QS_SENDMESSAGE   (QS_SENDMESSAGE << 16)
#endif /* WINVER >= 0x0500 */


#endif /* !NOMSG */

WINUSERAPI
BOOL
WINAPI
CSRegisterHotKey(
    IN HCSWND hWnd,
    IN int id,
    IN UINT fsModifiers,
    IN UINT vk);

WINUSERAPI
BOOL
WINAPI
CSUnregisterHotKey(
    IN HCSWND hWnd,
    IN int id);

#define MOD_ALT         0x0001
#define MOD_CONTROL     0x0002
#define MOD_SHIFT       0x0004
#define MOD_WIN         0x0008


#define IDHOT_SNAPWINDOW        (-1)    /* SHIFT-PRINTSCRN  */
#define IDHOT_SNAPDESKTOP       (-2)    /* PRINTSCRN        */

#ifdef WIN_INTERNAL
    #ifndef LSTRING
    #define NOLSTRING
    #endif /* LSTRING */
    #ifndef LFILEIO
    #define NOLFILEIO
    #endif /* LFILEIO */
#endif /* WIN_INTERNAL */

#if(WINVER >= 0x0400)

#define ENDSESSION_LOGOFF    0x80000000
#endif /* WINVER >= 0x0400 */

#define EWX_LOGOFF          0
#define EWX_SHUTDOWN        0x00000001
#define EWX_REBOOT          0x00000002
#define EWX_FORCE           0x00000004
#define EWX_POWEROFF        0x00000008
#if(_WIN32_WINNT >= 0x0500)
#define EWX_FORCEIFHUNG     0x00000010
#endif /* _WIN32_WINNT >= 0x0500 */


#define CSExitWindows(dwReserved, Code) CSExitWindowsEx(EWX_LOGOFF, 0xFFFFFFFF)

WINUSERAPI
BOOL
WINAPI
CSExitWindowsEx(
    IN UINT uFlags,
    IN DWORD dwReserved);

WINUSERAPI
BOOL
WINAPI
CSSwapMouseButton(
    IN BOOL fSwap);

WINUSERAPI
DWORD
WINAPI
CSGetMessagePos(
    void);

WINUSERAPI
LONG
WINAPI
CSGetMessageTime(
    void);

WINUSERAPI
LPARAM
WINAPI
CSGetMessageExtraInfo(
    void);

#if(WINVER >= 0x0400)
WINUSERAPI
LPARAM
WINAPI
CSSetMessageExtraInfo(
    IN LPARAM lParam);
#endif /* WINVER >= 0x0400 */

WINUSERAPI
LRESULT
WINAPI
CSSendMessage(
    IN HCSWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

WINUSERAPI
LRESULT
WINAPI
CSSendMessageTimeout(
    IN HCSWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam,
    IN UINT fuFlags,
    IN UINT uTimeout,
    OUT PDWORD_PTR lpdwResult);


WINUSERAPI
BOOL
WINAPI
CSSendNotifyMessage(
    IN HCSWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);


WINUSERAPI
BOOL
WINAPI
CSSendMessageCallback(
    IN HCSWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam,
    IN CSSENDASYNCPROC lpResultCallBack,
    IN ULONG_PTR dwData);

#if(_WIN32_WINNT >= 0x0501)
typedef struct {
    UINT  cbSize;
    HCSDESK hdesk;
    HCSWND  hwnd;
    CSLUID  luid;
} CSBSMINFO, *PCSBSMINFO;

WINUSERAPI
long
WINAPI
CSBroadcastSystemMessageEx(
    IN DWORD,
    IN LPDWORD,
    IN UINT,
    IN WPARAM,
    IN LPARAM,
    OUT PCSBSMINFO);
#endif /* _WIN32_WINNT >= 0x0501 */

#if(WINVER >= 0x0400)

#if defined(_WIN32_WINNT)
WINUSERAPI
long
WINAPI
CSBroadcastSystemMessage(
    IN DWORD,
    IN LPDWORD,
    IN UINT,
    IN WPARAM,
    IN LPARAM);

#elif defined(_WIN32_WINDOWS)
// The Win95 version isn't A/W decorated
WINUSERAPI
long
WINAPI
CSBroadcastSystemMessage(
    IN DWORD,
    IN LPDWORD,
    IN UINT,
    IN WPARAM,
    IN LPARAM);

#endif

//CSBroadcast Special Message Recipient list
#define BSM_ALLCOMPONENTS       0x00000000
#define BSM_VXDS                0x00000001
#define BSM_NETDRIVER           0x00000002
#define BSM_INSTALLABLEDRIVERS  0x00000004
#define BSM_APPLICATIONS        0x00000008
#define BSM_ALLDESKTOPS         0x00000010

//CSBroadcast Special Message Flags
#define BSF_QUERY               0x00000001
#define BSF_IGNORECURRENTTASK   0x00000002
#define BSF_FLUSHDISK           0x00000004
#define BSF_NOHANG              0x00000008
#define BSF_POSTMESSAGE         0x00000010
#define BSF_FORCEIFHUNG         0x00000020
#define BSF_NOTIMEOUTIFNOTHUNG  0x00000040
#if(_WIN32_WINNT >= 0x0500)
#define BSF_ALLOWSFW            0x00000080
#define BSF_SENDNOTIFYMESSAGE   0x00000100
#endif /* _WIN32_WINNT >= 0x0500 */
#if(_WIN32_WINNT >= 0x0501)
#define BSF_RETURNHDESK         0x00000200
#define BSF_LUID                0x00000400
#endif /* _WIN32_WINNT >= 0x0501 */

#define BROADCAST_QUERY_DENY         0x424D5144  // Return this value to deny a query.
#endif /* WINVER >= 0x0400 */

// CSRegisterDeviceNotification

#if(WINVER >= 0x0500)
typedef  PVOID           HDEVNOTIFY;
typedef  HDEVNOTIFY     *PHDEVNOTIFY;

#define DEVICE_NOTIFY_WINDOW_HCSHANDLE          0x00000000
#define DEVICE_NOTIFY_SERVICE_HCSHANDLE         0x00000001
#if(_WIN32_WINNT >= 0x0501)
#define DEVICE_NOTIFY_ALL_INTERFACE_CLASSES  0x00000004
#endif /* _WIN32_WINNT >= 0x0501 */

WINUSERAPI
HDEVNOTIFY
WINAPI
CSRegisterDeviceNotification(
    IN HCSHANDLE hRecipient,
    IN LPVOID NotificationFilter,
    IN DWORD Flags
    );

WINUSERAPI
BOOL
WINAPI
CSUnregisterDeviceNotification(
    IN HDEVNOTIFY Handle
    );
#endif /* WINVER >= 0x0500 */


WINUSERAPI
BOOL
WINAPI
CSPostMessage(
    IN HCSWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

WINUSERAPI
BOOL
WINAPI
CSPostThreadMessage(
    IN DWORD idThread,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

#define CSPostAppMessage(idThread, wMsg, wParam, lParam)\
        CSPostThreadMessage((DWORD)idThread, wMsg, wParam, lParam)

/*
 * Special HCSWND value for use with CSPostMessage() and CSSendMessage()
 */
#define HCSWND_BROADCAST  ((HCSWND)0xffff)

#if(WINVER >= 0x0500)
#define HCSWND_MESSAGE     ((HCSWND)-3)
#endif /* WINVER >= 0x0500 */

WINUSERAPI
BOOL
WINAPI
CSAttachThreadInput(
    IN DWORD idAttach,
    IN DWORD idAttachTo,
    IN BOOL fAttach);


WINUSERAPI
BOOL
WINAPI
CSReplyMessage(
    IN LRESULT lResult);

WINUSERAPI
BOOL
WINAPI
CSWaitMessage(
    void);


WINUSERAPI
DWORD
WINAPI
CSWaitForInputIdle(
    IN HCSHANDLE hProcess,
    IN DWORD dwMilliseconds);

WINUSERAPI
#ifndef _MAC
LRESULT
WINAPI
#else
LRESULT
CALLBACK
#endif
CSDefWindowProc(
    IN HCSWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);


WINUSERAPI
void
WINAPI
CSPostQuitMessage(
    IN int nExitCode);

#ifdef STRICT

WINUSERAPI
LRESULT
WINAPI
CSCallWindowProc(
    IN CSWNDPROC lpPrevWndFunc,
    IN HCSWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

#else /* !STRICT */

WINUSERAPI
LRESULT
WINAPI
CSCallWindowProc(
    IN FARPROC lpPrevWndFunc,
    IN HCSWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);


#endif /* !STRICT */

WINUSERAPI
BOOL
WINAPI
CSInSendMessage(
    void);

#if(WINVER >= 0x0500)
WINUSERAPI
DWORD
WINAPI
CSInSendMessageEx(
    IN LPVOID lpReserved);

/*
 * CSInSendMessageEx return value
 */
#define ISMEX_NOSEND      0x00000000
#define ISMEX_SEND        0x00000001
#define ISMEX_NOTIFY      0x00000002
#define ISMEX_CALLBACK    0x00000004
#define ISMEX_REPLIED     0x00000008
#endif /* WINVER >= 0x0500 */

WINUSERAPI
UINT
WINAPI
CSGetDoubleClickTime(
    void);

WINUSERAPI
BOOL
WINAPI
CSSetDoubleClickTime(
    IN UINT);

WINUSERAPI
ATOM
WINAPI
CSRegisterClass(
    IN CONST CSWNDCLASSA *lpWndClass);

WINUSERAPI
BOOL
WINAPI
CSUnregisterClass(
    IN LPCTSTR lpClassName,
    IN HCSINSTANCE hInstance);

WINUSERAPI
BOOL
WINAPI
CSGetClassInfo(
    IN HCSINSTANCE hInstance,
    IN LPCTSTR lpClassName,
    OUT LPCSWNDCLASSA lpWndClass);

#if(WINVER >= 0x0400)
WINUSERAPI
ATOM
WINAPI
CSRegisterClassEx(
    IN CONST CSWNDCLASSEXA *);

WINUSERAPI
BOOL
WINAPI
CSGetClassInfoEx(
    IN HCSINSTANCE,
    IN LPCTSTR,
    OUT LPCSWNDCLASSEXA);

#endif /* WINVER >= 0x0400 */

#define CW_USEDEFAULT       ((int)0x80000000)

/*
 * Special value for CSCreateWindow, et al.
 */
#define HCSWND_DESKTOP        ((HCSWND)0)

#if(_WIN32_WINNT >= 0x0501)
typedef BYTE (WINAPI * CSPREGISTERCLASSNAMEW)(LPCWSTR);
#endif /* _WIN32_WINNT >= 0x0501 */

WINUSERAPI
HCSWND
WINAPI
CSCreateWindowEx(
    IN DWORD dwExStyle,
    IN LPCTSTR lpClassName,
    IN LPCTSTR lpWindowName,
    IN DWORD dwStyle,
    IN int X,
    IN int Y,
    IN int nWidth,
    IN int nHeight,
    IN HCSWND hWndParent,
    IN HCSMENU hMenu,
    IN HCSINSTANCE hInstance,
    IN LPVOID lpParam);


#define CSCreateWindow(lpClassName, lpWindowName, dwStyle, x, y,\
nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)\
CSCreateWindowEx(0L, lpClassName, lpWindowName, dwStyle, x, y,\
nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)


WINUSERAPI
BOOL
WINAPI
CSIsWindow(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSIsMenu(
    IN HCSMENU hMenu);

WINUSERAPI
BOOL
WINAPI
CSIsChild(
    IN HCSWND hWndParent,
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSDestroyWindow(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSShowWindow(
    IN HCSWND hWnd,
    IN int nCmdShow);

#if(WINVER >= 0x0500)
WINUSERAPI
BOOL
WINAPI
CSAnimateWindow(
    IN HCSWND hWnd,
    IN DWORD dwTime,
    IN DWORD dwFlags);
#endif /* WINVER >= 0x0500 */

#if(_WIN32_WINNT >= 0x0500)
#if defined(_WINGDI_) && !defined (NOGDI)

WINUSERAPI
BOOL
WINAPI
CSUpdateLayeredWindow(
    HCSWND hWnd,
    HCSDC hdcDst,
    CSPOINT *pptDst,
    SIZE *psize,
    HCSDC hdcSrc,
    CSPOINT *pptSrc,
    COLORREF crKey,
    BLENDFUNCTION *pblend,
    DWORD dwFlags);
#endif

#if(_WIN32_WINNT >= 0x0501)
WINUSERAPI
BOOL
WINAPI
CSGetLayeredWindowAttributes(
    HCSWND hwnd,
    COLORREF *pcrKey,
    BYTE *pbAlpha,
    DWORD *pdwFlags);

#define PW_CLIENTONLY           0x00000001


WINUSERAPI
BOOL
WINAPI
CSPrintWindow(
    IN HCSWND hwnd,
    IN HCSDC hdcBlt,
    IN UINT nFlags);

#endif /* _WIN32_WINNT >= 0x0501 */

WINUSERAPI
BOOL
WINAPI
CSSetLayeredWindowAttributes(
    HCSWND hwnd,
    COLORREF crKey,
    BYTE bAlpha,
    DWORD dwFlags);

#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002


#define ULW_COLORKEY            0x00000001
#define ULW_ALPHA               0x00000002
#define ULW_OPAQUE              0x00000004


#endif /* _WIN32_WINNT >= 0x0500 */

#if(WINVER >= 0x0400)
WINUSERAPI
BOOL
WINAPI
CSShowWindowAsync(
    IN HCSWND hWnd,
    IN int nCmdShow);
#endif /* WINVER >= 0x0400 */

WINUSERAPI
BOOL
WINAPI
CSFlashWindow(
    IN HCSWND hWnd,
    IN BOOL bInvert);

#if(WINVER >= 0x0500)
typedef struct {
    UINT  cbSize;
    HCSWND  hwnd;
    DWORD dwFlags;
    UINT  uCount;
    DWORD dwTimeout;
} CSFLASHWINFO, *PCSFLASHWINFO;

WINUSERAPI
BOOL
WINAPI
CSFlashWindowEx(
    PCSFLASHWINFO pfwi);

#define FLASHW_STOP         0
#define FLASHW_CAPTION      0x00000001
#define FLASHW_TRAY         0x00000002
#define FLASHW_ALL          (FLASHW_CAPTION | FLASHW_TRAY)
#define FLASHW_TIMER        0x00000004
#define FLASHW_TIMERNOFG    0x0000000C

#endif /* WINVER >= 0x0500 */

WINUSERAPI
BOOL
WINAPI
CSShowOwnedPopups(
    IN HCSWND hWnd,
    IN BOOL fShow);

WINUSERAPI
BOOL
WINAPI
CSOpenIcon(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSCloseWindow(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSMoveWindow(
    IN HCSWND hWnd,
    IN int X,
    IN int Y,
    IN int nWidth,
    IN int nHeight,
    IN BOOL bRepaint);

WINUSERAPI
BOOL
WINAPI
CSSetWindowPos(
    IN HCSWND hWnd,
    IN HCSWND hWndInsertAfter,
    IN int X,
    IN int Y,
    IN int cx,
    IN int cy,
    IN UINT uFlags);

WINUSERAPI
BOOL
WINAPI
CSGetWindowPlacement(
    IN HCSWND hWnd,
    OUT CSWINDOWPLACEMENT *lpwndpl);

WINUSERAPI
BOOL
WINAPI
CSSetWindowPlacement(
    IN HCSWND hWnd,
    IN CONST CSWINDOWPLACEMENT *lpwndpl);


#ifndef NODEFERWINDOWPOS

WINUSERAPI
HCSDWP
WINAPI
CSBeginDeferWindowPos(
    IN int nNumWindows);

WINUSERAPI
HCSDWP
WINAPI
CSDeferWindowPos(
    IN HCSDWP hWinPosInfo,
    IN HCSWND hWnd,
    IN HCSWND hWndInsertAfter,
    IN int x,
    IN int y,
    IN int cx,
    IN int cy,
    IN UINT uFlags);

WINUSERAPI
BOOL
WINAPI
CSEndDeferWindowPos(
    IN HCSDWP hWinPosInfo);

#endif /* !NODEFERWINDOWPOS */

WINUSERAPI
BOOL
WINAPI
CSIsWindowVisible(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSIsIconic(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSAnyPopup(
    void);

WINUSERAPI
BOOL
WINAPI
CSBringWindowToTop(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSIsZoomed(
    IN HCSWND hWnd);

/*
 * CSSetWindowPos Flags
 */
#define SWP_NOSIZE          0x0001
#define SWP_NOMOVE          0x0002
#define SWP_NOZORDER        0x0004
#define SWP_NOREDRAW        0x0008
#define SWP_NOACTIVATE      0x0010
#define SWP_FRAMECHANGED    0x0020  /* The frame changed: send WM_NCCALCSIZE */
#define SWP_SHOWWINDOW      0x0040
#define SWP_HIDEWINDOW      0x0080
#define SWP_NOCOPYBITS      0x0100
#define SWP_NOOWNERZORDER   0x0200  /* Don't do owner Z ordering */
#define SWP_NOSENDCHANGING  0x0400  /* Don't send WM_WINDOWPOSCHANGING */
#define SWP_REDRAW			0x0800

#define SWP_DRAWFRAME       SWP_FRAMECHANGED
#define SWP_NOREPOSITION    SWP_NOOWNERZORDER

#if(WINVER >= 0x0400)
#define SWP_DEFERERASE      0x2000
#define SWP_ASYNCWINDOWPOS  0x4000
#endif /* WINVER >= 0x0400 */

#undef HWND_TOP
#define HWND_TOP        ((HCSWND)0)
#undef HWND_BOTTOM
#define HWND_BOTTOM     ((HCSWND)1)
#undef HWND_TOPMOST
#define HWND_TOPMOST    ((HCSWND)-1)
#undef HWND_NOTOPMOST
#define HWND_NOTOPMOST  ((HCSWND)-2)

#ifndef NOCTLMGR

/*
 * WARNING:
 * The following structures must NOT be DWORD padded because they are
 * followed by strings, etc that do not have to be DWORD aligned.
 */
//#include <pshpack2.h>

/*
 * original NT 32 bit dialog template:
 */
typedef struct {
    DWORD style;
    DWORD dwExtendedStyle;
    WORD cdit;
    short x;
    short y;
    short cx;
    short cy;
} CSDLGTEMPLATE;
typedef CSDLGTEMPLATE *LPCSDLGTEMPLATEA;

typedef LPCSDLGTEMPLATEA LPCSDLGTEMPLATE;
typedef CONST CSDLGTEMPLATE *LPCCSDLGTEMPLATEA;
typedef LPCCSDLGTEMPLATEA LPCCSDLGTEMPLATE;

/*
 * 32 bit Dialog item template.
 */
typedef struct {
    DWORD style;
    DWORD dwExtendedStyle;
    short x;
    short y;
    short cx;
    short cy;
    WORD id;
} CSDLGITEMTEMPLATE;
typedef CSDLGITEMTEMPLATE *PCSDLGITEMTEMPLATEA;

typedef PCSDLGITEMTEMPLATEA PCSDLGITEMTEMPLATE;
typedef CSDLGITEMTEMPLATE *LPCSDLGITEMTEMPLATEA;
typedef LPCSDLGITEMTEMPLATEA LPCSDLGITEMTEMPLATE;


//#include <poppack.h> 

WINUSERAPI
HCSWND
WINAPI
CSCreateDialogParam(
    IN HCSINSTANCE hInstance,
    IN LPCTSTR lpTemplateName,
    IN HCSWND hWndParent,
    IN CSDLGPROC lpDialogFunc,
    IN LPARAM dwInitParam);

WINUSERAPI
HCSWND
WINAPI
CSCreateDialogIndirectParam(
    IN HCSINSTANCE hInstance,
    IN LPCCSDLGTEMPLATEA lpTemplate,
    IN HCSWND hWndParent,
    IN CSDLGPROC lpDialogFunc,
    IN LPARAM dwInitParam);
/////////////////////////////////////////////////y_junjie modified
WINUSERAPI
HCSWND
WINAPI
CSCreateDialog(
  HCSINSTANCE hInstance,  // handle to module
  LPCTSTR lpTemplate,   // dialog box template name
  HCSWND hWndParent,      // handle to owner window
  CSDLGPROC lpDialogFunc  // dialog box procedure
);
WINUSERAPI
HCSWND
WINAPI
CSCreateDialogIndirect(
  HCSINSTANCE hInstance,        // handle to module
  LPCCSDLGTEMPLATE lpTemplate,  // dialog box template
  HCSWND hWndParent,            // handle to owner window
  CSDLGPROC lpDialogFunc        // dialog box procedure
);
/////////////////////////////////////////////////y_junjie modified


WINUSERAPI
INT_PTR
WINAPI
CSDialogBoxParam(
    IN HCSINSTANCE hInstance,
    IN LPCTSTR lpTemplateName,
    IN HCSWND hWndParent,
    IN CSDLGPROC lpDialogFunc,
    IN LPARAM dwInitParam);

WINUSERAPI
INT_PTR
WINAPI
CSDialogBoxIndirectParam(
    IN HCSINSTANCE hInstance,
    IN LPCCSDLGTEMPLATEA hDialogTemplate,
    IN HCSWND hWndParent,
    IN CSDLGPROC lpDialogFunc,
    IN LPARAM dwInitParam);


//////////////////////////////////////////////////y_junjie
WINUSERAPI
INT_PTR 
WINAPI
CSDialogBox(
  HCSINSTANCE hInstance,  // handle to module
  LPCTSTR lpTemplate,   // dialog box template
  HCSWND hWndParent,      // handle to owner window
  CSDLGPROC lpDialogFunc  // dialog box procedure
);
WINUSERAPI
INT_PTR 
WINAPI
CSDialogBoxIndirect(
  HCSINSTANCE hInstance,        // handle to module
  LPCSDLGTEMPLATE lpTemplate,  // dialog box template
  HCSWND hWndParent,            // handle to owner window
  CSDLGPROC lpDialogFunc        // dialog box procedure
);

//////////////////////////////////////////////////////////


WINUSERAPI
BOOL
WINAPI
CSEndDialog(
    IN HCSWND hDlg,
    IN INT_PTR nResult);

WINUSERAPI
HCSWND
WINAPI
CSGetDlgItem(
    IN HCSWND hDlg,
    IN int nIDDlgItem);

WINUSERAPI
BOOL
WINAPI
CSSetDlgItemInt(
    IN HCSWND hDlg,
    IN int nIDDlgItem,
    IN UINT uValue,
    IN BOOL bSigned);

WINUSERAPI
UINT
WINAPI
CSGetDlgItemInt(
    IN HCSWND hDlg,
    IN int nIDDlgItem,
    OUT BOOL *lpTranslated,
    IN BOOL bSigned);

WINUSERAPI
BOOL
WINAPI
CSSetDlgItemText(
    IN HCSWND hDlg,
    IN int nIDDlgItem,
    IN LPCTSTR lpString);

WINUSERAPI
UINT
WINAPI
CSGetDlgItemText(
    IN HCSWND hDlg,
    IN int nIDDlgItem,
    OUT LPTSTR lpString,
    IN int nMaxCount);


WINUSERAPI
BOOL
WINAPI
CSCheckDlgButton(
    IN HCSWND hDlg,
    IN int nIDButton,
    IN UINT uCheck);

WINUSERAPI
BOOL
WINAPI
CSCheckRadioButton(
    IN HCSWND hDlg,
    IN int nIDFirstButton,
    IN int nIDLastButton,
    IN int nIDCheckButton);

WINUSERAPI
UINT
WINAPI
CSIsDlgButtonChecked(
    IN HCSWND hDlg,
    IN int nIDButton);

WINUSERAPI
LRESULT
WINAPI
CSSendDlgItemMessage(
    IN HCSWND hDlg,
    IN int nIDDlgItem,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

WINUSERAPI
HCSWND
WINAPI
CSGetNextDlgGroupItem(
    IN HCSWND hDlg,
    IN HCSWND hCtl,
    IN BOOL bPrevious);

WINUSERAPI
HCSWND
WINAPI
CSGetNextDlgTabItem(
    IN HCSWND hDlg,
    IN HCSWND hCtl,
    IN BOOL bPrevious);

WINUSERAPI
int
WINAPI
CSGetDlgCtrlID(
    IN HCSWND hWnd);

WINUSERAPI
long
WINAPI
CSGetDialogBaseUnits(void);

WINUSERAPI
#ifndef _MAC
LRESULT
WINAPI
#else
LRESULT
CALLBACK
#endif
CSDefDlgProc(
    IN HCSWND hDlg,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

/*
 * Window extra byted needed for private dialog classes.
 */
#ifndef _MAC
#define DLGWINDOWEXTRA 30
#else
#define DLGWINDOWEXTRA 48
#endif

#endif /* !NOCTLMGR */

#ifndef NOMSG

WINUSERAPI
BOOL
WINAPI
CSCallMsgFilter(
    IN LPCSMSG lpMsg,
    IN int nCode);


#endif /* !NOMSG */

#ifndef NOCLIPBOARD

/*
 * Clipboard Manager Functions
 */

WINUSERAPI
BOOL
WINAPI
CSOpenClipboard(
    IN HCSWND hWndNewOwner);

WINUSERAPI
BOOL
WINAPI
CSCloseClipboard(
    void);


#if(WINVER >= 0x0500)

WINUSERAPI
DWORD
WINAPI
CSGetClipboardSequenceNumber(
    void);

#endif /* WINVER >= 0x0500 */

WINUSERAPI
HCSWND
WINAPI
CSGetClipboardOwner(
    void);

WINUSERAPI
HCSWND
WINAPI
CSSetClipboardViewer(
    IN HCSWND hWndNewViewer);

WINUSERAPI
HCSWND
WINAPI
CSGetClipboardViewer(
    void);

WINUSERAPI
BOOL
WINAPI
CSChangeClipboardChain(
    IN HCSWND hWndRemove,
    IN HCSWND hWndNewNext);

WINUSERAPI
HCSHANDLE
WINAPI
CSSetClipboardData(
    IN UINT uFormat,
    IN HCSHANDLE hMem);

WINUSERAPI
HCSHANDLE
WINAPI
CSGetClipboardData(
    IN UINT uFormat);

WINUSERAPI
UINT
WINAPI
CSRegisterClipboardFormat(
    IN LPCTSTR lpszFormat);


WINUSERAPI
int
WINAPI
CSCountClipboardFormats(
    void);

WINUSERAPI
UINT
WINAPI
CSEnumClipboardFormats(
    IN UINT format);

WINUSERAPI
int
WINAPI
CSGetClipboardFormatName(
    IN UINT format,
    OUT LPTSTR lpszFormatName,
    IN int cchMaxCount);

WINUSERAPI
BOOL
WINAPI
CSEmptyClipboard(
    void);

WINUSERAPI
BOOL
WINAPI
CSIsClipboardFormatAvailable(
    IN UINT format);

WINUSERAPI
int
WINAPI
CSGetPriorityClipboardFormat(
    OUT UINT *paFormatPriorityList,
    IN int cFormats);

WINUSERAPI
HCSWND
WINAPI
CSGetOpenClipboardWindow(
    void);

#endif /* !NOCLIPBOARD */

/*
 * Character Translation Routines
 */

WINUSERAPI
BOOL
WINAPI
CSCharToOem(
    IN LPCTSTR lpszSrc,
    OUT LPTSTR lpszDst);


WINUSERAPI
BOOL
WINAPI
CSOemToChar(
    IN LPCTSTR lpszSrc,
    OUT LPTSTR lpszDst);


WINUSERAPI
BOOL
WINAPI
CSCharToOemBuff(
    IN LPCTSTR lpszSrc,
    OUT LPTSTR lpszDst,
    IN DWORD cchDstLength);

WINUSERAPI
BOOL
WINAPI
CSOemToCharBuff(
    IN LPCTSTR lpszSrc,
    OUT LPTSTR lpszDst,
    IN DWORD cchDstLength);


WINUSERAPI
LPTSTR
WINAPI
CSCharUpper(
    IN OUT LPTSTR lpsz);


WINUSERAPI
DWORD
WINAPI
CSCharUpperBuff(
    IN OUT LPTSTR lpsz,
    IN DWORD cchLength);


WINUSERAPI
LPTSTR
WINAPI
CSCharLower(
    IN OUT LPTSTR lpsz);

WINUSERAPI
DWORD
WINAPI
CSCharLowerBuff(
    IN OUT LPTSTR lpsz,
    IN DWORD cchLength);


WINUSERAPI
LPTSTR
WINAPI
CSCharNext(
    IN LPCTSTR lpsz);

WINUSERAPI
LPTSTR
WINAPI
CSCharPrev(
    IN LPCTSTR lpszStart,
    IN LPCTSTR lpszCurrent);


#if(WINVER >= 0x0400)
WINUSERAPI
LPTSTR
WINAPI
CSCharNextExA(
     IN WORD CodePage,
     IN LPCTSTR lpCurrentChar,
     IN DWORD dwFlags);

WINUSERAPI
LPTSTR
WINAPI
CSCharPrevExA(
     IN WORD CodePage,
     IN LPCTSTR lpStart,
     IN LPCTSTR lpCurrentChar,
     IN DWORD dwFlags);
#endif /* WINVER >= 0x0400 */

/*
 * Compatibility defines for character translation routines
 */
#define CSAnsiToOem CSCharToOemA
#define CSOemToAnsi CSOemToCharA
#define CSAnsiToOemBuff CSCharToOemBuffA
#define CSOemToAnsiBuff CSOemToCharBuffA
#define CSAnsiUpper CSCharUpperA
#define CSAnsiUpperBuff CSCharUpperBuffA
#define CSAnsiLower CSCharLowerA
#define CSAnsiLowerBuff CSCharLowerBuffA
#define CSAnsiNext CSCharNextA
#define CSAnsiPrev CSCharPrevA

#ifndef  NOLANGUAGE
/*
 * Language dependent Routines
 */

WINUSERAPI
BOOL
WINAPI
CSIsCharAlpha(
    IN CHAR ch);

WINUSERAPI
BOOL
WINAPI
CSIsCharAlphaNumeric(
    IN CHAR ch);


WINUSERAPI
BOOL
WINAPI
CSIsCharUpper(
    IN CHAR ch);


WINUSERAPI
BOOL
WINAPI
CSIsCharLower(
    IN CHAR ch);


#endif  /* !NOLANGUAGE */

WINUSERAPI
HCSWND
WINAPI
CSSetFocus(
    IN HCSWND hWnd);

WINUSERAPI
HCSWND
WINAPI
CSGetActiveWindow(
    void);

WINUSERAPI
HCSWND
WINAPI
CSGetFocus(
    void);

WINUSERAPI
UINT
WINAPI
CSGetKBCodePage(
    void);

WINUSERAPI
SHORT
WINAPI
CSGetKeyState(
    IN int nVirtKey);

WINUSERAPI
SHORT
WINAPI
CSGetAsyncKeyState(
    IN int vKey);

WINUSERAPI
BOOL
WINAPI
CSGetKeyboardState(
    OUT PBYTE lpKeyState);

WINUSERAPI
BOOL
WINAPI
CSSetKeyboardState(
    IN LPBYTE lpKeyState);

WINUSERAPI
int
WINAPI
CSGetKeyNameText(
    IN LONG lParam,
    OUT LPTSTR lpString,
    IN int nSize
    );


WINUSERAPI
int
WINAPI
CSGetKeyboardType(
    IN int nTypeFlag);

WINUSERAPI
int
WINAPI
CSToAscii(
    IN UINT uVirtKey,
    IN UINT uScanCode,
    IN CONST BYTE *lpKeyState,
    OUT LPWORD lpChar,
    IN UINT uFlags);

#if(WINVER >= 0x0400)
WINUSERAPI
int
WINAPI
CSToAsciiEx(
    IN UINT uVirtKey,
    IN UINT uScanCode,
    IN CONST BYTE *lpKeyState,
    OUT LPWORD lpChar,
    IN UINT uFlags,
    IN HCSKL dwhkl);
#endif /* WINVER >= 0x0400 */

WINUSERAPI
int
WINAPI
CSToUnicode(
    IN UINT wVirtKey,
    IN UINT wScanCode,
    IN CONST BYTE *lpKeyState,
    OUT LPWSTR pwszBuff,
    IN int cchBuff,
    IN UINT wFlags);

WINUSERAPI
DWORD
WINAPI
CSOemKeyScan(
    IN WORD wOemChar);

WINUSERAPI
SHORT
WINAPI
CSVkKeyScan(
    IN CHAR ch);


#if(WINVER >= 0x0400)
WINUSERAPI
SHORT
WINAPI
CSVkKeyScanEx(
    IN CHAR  ch,
    IN HCSKL   dwhkl);

#endif /* WINVER >= 0x0400 */
#define KEYEVENTF_EXTENDEDKEY 0x0001
#define KEYEVENTF_KEYUP       0x0002
#if(_WIN32_WINNT >= 0x0500)
#define KEYEVENTF_UNICODE     0x0004
#define KEYEVENTF_SCANCODE    0x0008
#endif /* _WIN32_WINNT >= 0x0500 */

WINUSERAPI
void
WINAPI
CSkeybd_event(
    IN BYTE bVk,
    IN BYTE bScan,
    IN DWORD dwFlags,
    IN ULONG_PTR dwExtraInfo);

#define MOUSEEVENTF_MOVE        0x0001 /* mouse move */
#define MOUSEEVENTF_LEFTDOWN    0x0002 /* left button down */
#define MOUSEEVENTF_LEFTUP      0x0004 /* left button up */
#define MOUSEEVENTF_RIGHTDOWN   0x0008 /* right button down */
#define MOUSEEVENTF_RIGHTUP     0x0010 /* right button up */
#define MOUSEEVENTF_MIDDLEDOWN  0x0020 /* middle button down */
#define MOUSEEVENTF_MIDDLEUP    0x0040 /* middle button up */
#define MOUSEEVENTF_XDOWN       0x0080 /* x button down */
#define MOUSEEVENTF_XUP         0x0100 /* x button down */
#define MOUSEEVENTF_WHEEL       0x0800 /* wheel button rolled */
#define MOUSEEVENTF_VIRTUALDESK 0x4000 /* map to entire virtual desktop */
#define MOUSEEVENTF_ABSOLUTE    0x8000 /* absolute move */



WINUSERAPI
void
WINAPI
CSmouse_event(
    IN DWORD dwFlags,
    IN DWORD dx,
    IN DWORD dy,
    IN DWORD dwData,
    IN ULONG_PTR dwExtraInfo);

#if (_WIN32_WINNT > 0x0400)

typedef struct tagCSMOUSEINPUT {
    LONG    dx;
    LONG    dy;
    DWORD   mouseData;
    DWORD   dwFlags;
    DWORD   time;
    ULONG_PTR dwExtraInfo;
} CSMOUSEINPUT, *PCSMOUSEINPUT, FAR* LPCSMOUSEINPUT;

typedef struct tagCSKEYBDINPUT {
    WORD    wVk;
    WORD    wScan;
    DWORD   dwFlags;
    DWORD   time;
    ULONG_PTR dwExtraInfo;
} CSKEYBDINPUT, *PCSKEYBDINPUT, FAR* LPCSKEYBDINPUT;

typedef struct tagCSHARDWAREINPUT {
    DWORD   uMsg;
    WORD    wParamL;
    WORD    wParamH;
} CSHARDWAREINPUT, *PCSHARDWAREINPUT, FAR* LPCSHARDWAREINPUT;

#define INPUT_MOUSE     0
#define INPUT_KEYBOARD  1
#define INPUT_HARDWARE  2

typedef struct tagCSINPUT {
    DWORD   type;

    union
    {
        CSMOUSEINPUT      mi;
        CSKEYBDINPUT      ki;
        CSHARDWAREINPUT   hi;
    }u;
} CSINPUT, *PCSINPUT, FAR* LPCSINPUT;

WINUSERAPI
UINT
WINAPI
CSSendInput(
    IN UINT    cInputs,     // number of input in the array
    IN LPCSINPUT pInputs,     // array of inputs
    IN int     cbSize);     // sizeof(CSINPUT)

#endif // (_WIN32_WINNT > 0x0400)

#if(_WIN32_WINNT >= 0x0500)
typedef struct tagCSLASTINPUTINFO {
    UINT cbSize;
    DWORD dwTime;
} CSLASTINPUTINFO, * PCSLASTINPUTINFO;

WINUSERAPI
BOOL
WINAPI
CSGetLastInputInfo(
    OUT PCSLASTINPUTINFO plii);
#endif /* _WIN32_WINNT >= 0x0500 */

WINUSERAPI
UINT
WINAPI
CSMapVirtualKey(
    IN UINT uCode,
    IN UINT uMapType);


#if(WINVER >= 0x0400)
WINUSERAPI
UINT
WINAPI
CSMapVirtualKeyEx(
    IN UINT uCode,
    IN UINT uMapType,
    IN HCSKL dwhkl);

#endif /* WINVER >= 0x0400 */

WINUSERAPI
BOOL
WINAPI
CSGetInputState(
    void);

WINUSERAPI
DWORD
WINAPI
CSGetQueueStatus(
    IN UINT flags);


WINUSERAPI
HCSWND
WINAPI
CSGetCapture(
    void);

WINUSERAPI
HCSWND
WINAPI
CSSetCapture(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSModifyCapture(
    void);

WINUSERAPI
DWORD
WINAPI
CSMsgWaitForMultipleObjects(
    IN DWORD nCount,
    IN CONST HCSHANDLE *pHandles,
    IN BOOL fWaitAll,
    IN DWORD dwMilliseconds,
    IN DWORD dwWakeMask);

WINUSERAPI
DWORD
WINAPI
CSMsgWaitForMultipleObjectsEx(
    IN DWORD nCount,
    IN CONST HCSHANDLE *pHandles,
    IN DWORD dwMilliseconds,
    IN DWORD dwWakeMask,
    IN DWORD dwFlags);


#define MWMO_WAITALL        0x0001
#define MWMO_ALERTABLE      0x0002
#define MWMO_INPUTAVAILABLE 0x0004

/*
 * Queue status flags for CSGetQueueStatus() and MsgWaitForMultipleObjects()
 */
#define QS_KEY              0x0001
#define QS_MOUSEMOVE        0x0002
#define QS_MOUSEBUTTON      0x0004
#define QS_POSTMESSAGE      0x0008
#define QS_TIMER            0x0010
#define QS_PAINT            0x0020
#define QS_SENDMESSAGE      0x0040
#define QS_HOTKEY           0x0080
#define QS_ALLPOSTMESSAGE   0x0100
#if(_WIN32_WINNT >= 0x0501)
#define QS_RAWINPUT         0x0400
#endif /* _WIN32_WINNT >= 0x0501 */

#define QS_MOUSE           (QS_MOUSEMOVE     | \
                            QS_MOUSEBUTTON)

#if (_WIN32_WINNT >= 0x0501)
#define QS_INPUT           (QS_MOUSE         | \
                            QS_KEY           | \
                            QS_RAWINPUT)
#else
#define QS_INPUT           (QS_MOUSE         | \
                            QS_KEY)
#endif // (_WIN32_WINNT >= 0x0501)

#define QS_ALLEVENTS       (QS_INPUT         | \
                            QS_POSTMESSAGE   | \
                            QS_TIMER         | \
                            QS_PAINT         | \
                            QS_HOTKEY)

#define QS_ALLINPUT        (QS_INPUT         | \
                            QS_POSTMESSAGE   | \
                            QS_TIMER         | \
                            QS_PAINT         | \
                            QS_HOTKEY        | \
                            QS_SENDMESSAGE)


/*
 * Windows Functions
 */

WINUSERAPI
UINT_PTR
WINAPI
CSSetTimer(
    IN HCSWND hWnd,
    IN UINT_PTR nIDEvent,
    IN UINT uElapse,
    IN CSTIMERPROC lpTimerFunc);

WINUSERAPI
BOOL
WINAPI
CSKillTimer(
    IN HCSWND hWnd,
    IN UINT_PTR uIDEvent);

WINUSERAPI
BOOL
WINAPI
CSIsWindowUnicode(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSEnableWindow(
    IN HCSWND hWnd,
    IN BOOL bEnable);

WINUSERAPI
BOOL
WINAPI
CSIsWindowEnabled(
    IN HCSWND hWnd);

WINUSERAPI
HCSACCEL
WINAPI
CSLoadAccelerators(
    IN HCSINSTANCE hInstance,
    IN LPCTSTR lpTableName);

WINUSERAPI
HCSACCEL
WINAPI
CSCreateAcceleratorTable(
    IN LPCSACCEL, IN int);


WINUSERAPI
BOOL
WINAPI
CSDestroyAcceleratorTable(
    IN HCSACCEL hAccel);

WINUSERAPI
int
WINAPI
CSCopyAcceleratorTable(
    IN HCSACCEL hAccelSrc,
    OUT LPCSACCEL lpAccelDst,
    IN int cAccelEntries);


#ifndef NOMSG

WINUSERAPI
int
WINAPI
CSTranslateAccelerator(
    IN HCSWND hWnd,
    IN HCSACCEL hAccTable,
    IN LPCSMSG lpMsg);


#endif /* !NOMSG */

#ifndef NOSYSMETRICS

/*
 * CSGetSystemMetrics() codes
 */

#define SM_CXSCREEN             0
#define SM_CYSCREEN             1
#define SM_CXVSCROLL            2
#define SM_CYHSCROLL            3
#define SM_CYCAPTION            4
#define SM_CXBORDER             5
#define SM_CYBORDER             6
#define SM_CXDLGFRAME           7
#define SM_CYDLGFRAME           8
#define SM_CYVTHUMB             9
#define SM_CXHTHUMB             10
#define SM_CXICON               11
#define SM_CYICON               12
#define SM_CXCURSOR             13
#define SM_CYCURSOR             14
#define SM_CYMENU               15
#define SM_CXFULLSCREEN         16
#define SM_CYFULLSCREEN         17
#define SM_CYKANJIWINDOW        18
#define SM_MOUSEPRESENT         19
#define SM_CYVSCROLL            20
#define SM_CXHSCROLL            21
#define SM_DEBUG                22
#define SM_SWAPBUTTON           23
#define SM_RESERVED1            24
#define SM_RESERVED2            25
#define SM_RESERVED3            26
#define SM_RESERVED4            27
#define SM_CXMIN                28
#define SM_CYMIN                29
#define SM_CXSIZE               30
#define SM_CYSIZE               31
#define SM_CXFRAME              32
#define SM_CYFRAME              33
#define SM_CXMINTRACK           34
#define SM_CYMINTRACK           35
#define SM_CXDOUBLECLK          36
#define SM_CYDOUBLECLK          37
#define SM_CXICONSPACING        38
#define SM_CYICONSPACING        39
#define SM_MENUDROPALIGNMENT    40
#define SM_PENWINDOWS           41
#define SM_DBCSENABLED          42
#define SM_CMOUSEBUTTONS        43

#if(WINVER >= 0x0400)
#define SM_CXFIXEDFRAME           SM_CXDLGFRAME  /* ;win40 name change */
#define SM_CYFIXEDFRAME           SM_CYDLGFRAME  /* ;win40 name change */
#define SM_CXSIZEFRAME            SM_CXFRAME     /* ;win40 name change */
#define SM_CYSIZEFRAME            SM_CYFRAME     /* ;win40 name change */

#define SM_SECURE               44
#define SM_CXEDGE               45
#define SM_CYEDGE               46
#define SM_CXMINSPACING         47
#define SM_CYMINSPACING         48
#define SM_CXSMICON             49
#define SM_CYSMICON             50
#define SM_CYSMCAPTION          51
#define SM_CXSMSIZE             52
#define SM_CYSMSIZE             53
#define SM_CXMENUSIZE           54
#define SM_CYMENUSIZE           55
#define SM_ARRANGE              56
#define SM_CXMINIMIZED          57
#define SM_CYMINIMIZED          58
#define SM_CXMAXTRACK           59
#define SM_CYMAXTRACK           60
#define SM_CXMAXIMIZED          61
#define SM_CYMAXIMIZED          62
#define SM_NETWORK              63
#define SM_CLEANBOOT            67
#define SM_CXDRAG               68
#define SM_CYDRAG               69
#endif /* WINVER >= 0x0400 */
#define SM_SHOWSOUNDS           70
#if(WINVER >= 0x0400)
#define SM_CXMENUCHECK          71   /* Use instead of CSGetMenuCheckMarkDimensions()! */
#define SM_CYMENUCHECK          72
#define SM_SLOWMACHINE          73
#define SM_MIDEASTENABLED       74
#endif /* WINVER >= 0x0400 */

#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
#define SM_MOUSEWHEELPRESENT    75
#endif
#if(WINVER >= 0x0500)
#define SM_XVIRTUALSCREEN       76
#define SM_YVIRTUALSCREEN       77
#define SM_CXVIRTUALSCREEN      78
#define SM_CYVIRTUALSCREEN      79
#define SM_CMONITORS            80
#define SM_SAMEDISPLAYFORMAT    81
#endif /* WINVER >= 0x0500 */
#if(_WIN32_WINNT >= 0x0500)
#define SM_IMMENABLED           82
#endif /* _WIN32_WINNT >= 0x0500 */
#if(_WIN32_WINNT >= 0x0501)
#define SM_CXFOCUSBORDER        83
#define SM_CYFOCUSBORDER        84
#endif /* _WIN32_WINNT >= 0x0501 */

#if (WINVER < 0x0500) && (!defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0400))
#define SM_CMETRICS             76
#else
#define SM_CMETRICS             86
#endif

#if(WINVER >= 0x0500)
#define SM_REMOTESESSION        0x1000


#if(_WIN32_WINNT >= 0x0501)
#define SM_SHUTTINGDOWN         0x2000
#endif /* _WIN32_WINNT >= 0x0501 */

#endif /* WINVER >= 0x0500 */


WINUSERAPI
int
WINAPI
CSGetSystemMetrics(
    IN int nIndex);


#endif /* !NOSYSMETRICS */

#ifndef NOMENUS

WINUSERAPI
HCSMENU
WINAPI
CSLoadMenu(
    IN HCSINSTANCE hInstance,
    IN LPCTSTR lpMenuName);

WINUSERAPI
HCSMENU
WINAPI
CSLoadMenuIndirect(
    IN CONST MENUTEMPLATEA *lpMenuTemplate);


WINUSERAPI
HCSMENU
WINAPI
CSGetMenu(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSSetMenu(
    IN HCSWND hWnd,
    IN HCSMENU hMenu);

WINUSERAPI
BOOL
WINAPI
CSChangeMenu(
    IN HCSMENU hMenu,
    IN UINT cmd,
    IN LPCTSTR lpszNewItem,
    IN UINT cmdInsert,
    IN UINT flags);

WINUSERAPI
BOOL
WINAPI
CSHiliteMenuItem(
    IN HCSWND hWnd,
    IN HCSMENU hMenu,
    IN UINT uIDHiliteItem,
    IN UINT uHilite);

WINUSERAPI
int
WINAPI
CSGetMenuString(
    IN HCSMENU hMenu,
    IN UINT uIDItem,
    OUT LPTSTR lpString,
    IN int nMaxCount,
    IN UINT uFlag);

WINUSERAPI
UINT
WINAPI
CSGetMenuState(
    IN HCSMENU hMenu,
    IN UINT uId,
    IN UINT uFlags);

WINUSERAPI
BOOL
WINAPI
CSDrawMenuBar(
    IN HCSWND hWnd);

#if(_WIN32_WINNT >= 0x0501)
#define PMB_ACTIVE      0x00000001

#endif /* _WIN32_WINNT >= 0x0501 */


WINUSERAPI
HCSMENU
WINAPI
CSGetSystemMenu(
    IN HCSWND hWnd,
    IN BOOL bRevert);


WINUSERAPI
HCSMENU
WINAPI
CSCreateMenu(
    void);

WINUSERAPI
HCSMENU
WINAPI
CSCreatePopupMenu(
    void);

WINUSERAPI
BOOL
WINAPI
CSDestroyMenu(
    IN HCSMENU hMenu);

WINUSERAPI
DWORD
WINAPI
CSCheckMenuItem(
    IN HCSMENU hMenu,
    IN UINT uIDCheckItem,
    IN UINT uCheck);

WINUSERAPI
BOOL
WINAPI
CSEnableMenuItem(
    IN HCSMENU hMenu,
    IN UINT uIDEnableItem,
    IN UINT uEnable);

WINUSERAPI
HCSMENU
WINAPI
CSGetSubMenu(
    IN HCSMENU hMenu,
    IN int nPos);

WINUSERAPI
UINT
WINAPI
CSGetMenuItemID(
    IN HCSMENU hMenu,
    IN int nPos);

WINUSERAPI
int
WINAPI
CSGetMenuItemCount(
    IN HCSMENU hMenu);

WINUSERAPI
BOOL
WINAPI
CSInsertMenu(
    IN HCSMENU hMenu,
    IN UINT uPosition,
    IN UINT uFlags,
    IN UINT_PTR uIDNewItem,
    IN LPCTSTR lpNewItem
    );

WINUSERAPI
BOOL
WINAPI
CSAppendMenu(
    IN HCSMENU hMenu,
    IN UINT uFlags,
    IN UINT_PTR uIDNewItem,
    IN LPCTSTR lpNewItem
    );

WINUSERAPI
BOOL
WINAPI
CSModifyMenu(
    IN HCSMENU hMnu,
    IN UINT uPosition,
    IN UINT uFlags,
    IN UINT_PTR uIDNewItem,
    IN LPCTSTR lpNewItem
    );


WINUSERAPI
BOOL
WINAPI CSRemoveMenu(
    IN HCSMENU hMenu,
    IN UINT uPosition,
    IN UINT uFlags);

WINUSERAPI
BOOL
WINAPI
CSDeleteMenu(
    IN HCSMENU hMenu,
    IN UINT uPosition,
    IN UINT uFlags);

WINUSERAPI
BOOL
WINAPI
CSSetMenuItemBitmaps(
    IN HCSMENU hMenu,
    IN UINT uPosition,
    IN UINT uFlags,
    IN HCSBITMAP hBitmapUnchecked,
    IN HCSBITMAP hBitmapChecked);

WINUSERAPI
LONG
WINAPI
CSGetMenuCheckMarkDimensions(
    void);

WINUSERAPI
BOOL
WINAPI
CSTrackPopupMenu(
    IN HCSMENU hMenu,
    IN UINT uFlags,
    IN int x,
    IN int y,
    IN int nReserved,
    IN HCSWND hWnd,
    IN CONST CSRECT *prcRect);

#if(WINVER >= 0x0400)
/* return codes for WM_MENUCHAR */
#define MNC_IGNORE  0
#define MNC_CLOSE   1
#define MNC_EXECUTE 2
#define MNC_SELECT  3

typedef struct tagCSTPMPARAMS
{
    UINT    cbSize;     /* Size of structure */
    CSRECT    rcExclude;  /* Screen coordinates of rectangle to exclude when positioning */
}   CSTPMPARAMS;
typedef CSTPMPARAMS FAR *LPCSTPMPARAMS;

WINUSERAPI
BOOL
WINAPI
CSTrackPopupMenuEx(
    IN HCSMENU,
    IN UINT,
    IN int,
    IN int,
    IN HCSWND,
    IN LPCSTPMPARAMS);
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)

#define MNS_NOCHECK         0x80000000
#define MNS_MODELESS        0x40000000
#define MNS_DRAGDROP        0x20000000
#define MNS_AUTODISMISS     0x10000000
#define MNS_NOTIFYBYPOS     0x08000000
#define MNS_CHECKORBMP      0x04000000

#define MIM_MAXHEIGHT               0x00000001
#define MIM_BACKGROUND              0x00000002
#define MIM_HELPID                  0x00000004
#define MIM_MENUDATA                0x00000008
#define MIM_STYLE                   0x00000010
#define MIM_APPLYTOSUBMENUS         0x80000000

typedef struct tagCSMENUINFO
{
    DWORD   cbSize;
    DWORD   fMask;
    DWORD   dwStyle;
    UINT    cyMax;
    HCSBRUSH  hbrBack;
    DWORD   dwContextHelpID;
    ULONG_PTR dwMenuData;
}   CSMENUINFO, FAR *LPCSMENUINFO;
typedef CSMENUINFO CONST FAR *LPCCSMENUINFO;

WINUSERAPI
BOOL
WINAPI
CSGetMenuInfo(
    IN HCSMENU,
    OUT LPCCSMENUINFO);

WINUSERAPI
BOOL
WINAPI
CSSetMenuInfo(
    IN HCSMENU,
    IN LPCCSMENUINFO);

WINUSERAPI
BOOL
WINAPI
CSEndMenu(
        void);

/*
 * WM_MENUDRAG return values.
 */
#define MND_CONTINUE       0
#define MND_ENDMENU        1

typedef struct tagCSMENUGETOBJECTINFO
{
    DWORD dwFlags;
    UINT uPos;
    HCSMENU hmenu;
    PVOID riid;
    PVOID pvObj;
} CSMENUGETOBJECTINFO, * PCSMENUGETOBJECTINFO;

/*
 * CSMENUGETOBJECTINFO dwFlags values
 */
#define MNGOF_TOPGAP         0x00000001
#define MNGOF_BOTTOMGAP      0x00000002

/*
 * WM_MENUGETOBJECT return values
 */
#define MNGO_NOINTERFACE     0x00000000
#define MNGO_NOERROR         0x00000001
#endif /* WINVER >= 0x0500 */

#if(WINVER >= 0x0400)
#define MIIM_STATE       0x00000001
#define MIIM_ID          0x00000002
#define MIIM_SUBMENU     0x00000004
#define MIIM_CHECKMARKS  0x00000008
#define MIIM_TYPE        0x00000010
#define MIIM_DATA        0x00000020
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)
#define MIIM_STRING      0x00000040
#define MIIM_BITMAP      0x00000080
#define MIIM_FTYPE       0x00000100
/*
#define HBMMENU_CALLBACK            ((HCSBITMAP) -1)
#define HBMMENU_SYSTEM              ((HCSBITMAP)  1)
#define HBMMENU_MBAR_RESTORE        ((HCSBITMAP)  2)
#define HBMMENU_MBAR_MINIMIZE       ((HCSBITMAP)  3)
#define HBMMENU_MBAR_CLOSE          ((HCSBITMAP)  5)
#define HBMMENU_MBAR_CLOSE_D        ((HCSBITMAP)  6)
#define HBMMENU_MBAR_MINIMIZE_D     ((HCSBITMAP)  7)
#define HBMMENU_POPUP_CLOSE         ((HCSBITMAP)  8)
#define HBMMENU_POPUP_RESTORE       ((HCSBITMAP)  9)
#define HBMMENU_POPUP_MAXIMIZE      ((HCSBITMAP) 10)
#define HBMMENU_POPUP_MINIMIZE      ((HCSBITMAP) 11)
*/
#endif /* WINVER >= 0x0500 */

#if(WINVER >= 0x0400)
typedef struct tagCSMENUITEMINFOA
{
    UINT     cbSize;
    UINT     fMask;
    UINT     fType;         // used if MIIM_TYPE (4.0) or MIIM_FTYPE (>4.0)
    UINT     fState;        // used if MIIM_STATE
    UINT     wID;           // used if MIIM_ID
    HCSMENU    hSubMenu;      // used if MIIM_SUBMENU
    HCSBITMAP  hbmpChecked;   // used if MIIM_CHECKMARKS
    HCSBITMAP  hbmpUnchecked; // used if MIIM_CHECKMARKS
    ULONG_PTR dwItemData;   // used if MIIM_DATA
    LPTSTR    dwTypeData;    // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
    UINT     cch;           // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
#if(WINVER >= 0x0500)
    HCSBITMAP  hbmpItem;      // used if MIIM_BITMAP
#endif /* WINVER >= 0x0500 */
}   CSMENUITEMINFOA, FAR *LPCSMENUITEMINFOA;

typedef CSMENUITEMINFOA CSMENUITEMINFO;
typedef LPCSMENUITEMINFOA LPCSMENUITEMINFO;
typedef CSMENUITEMINFOA CONST FAR *LPCCSMENUITEMINFOA;
typedef LPCCSMENUITEMINFOA LPCCSMENUITEMINFO;


WINUSERAPI
BOOL
WINAPI
CSInsertMenuItem(
    IN HCSMENU,
    IN UINT,
    IN BOOL,
    IN LPCCSMENUITEMINFOA
    );


WINUSERAPI
BOOL
WINAPI
CSGetMenuItemInfo(
    IN HCSMENU,
    IN UINT,
    IN BOOL,
    IN OUT LPCSMENUITEMINFOA
    );

WINUSERAPI
BOOL
WINAPI
CSSetMenuItemInfo(
    IN HCSMENU,
    IN UINT,
    IN BOOL,
    IN LPCSMENUITEMINFOA
    );


#define GMDI_USEDISABLED    0x0001L
#define GMDI_GOINTOPOPUPS   0x0002L

WINUSERAPI UINT    WINAPI CSGetMenuDefaultItem( IN HCSMENU hMenu, IN UINT fByPos, IN UINT gmdiFlags);
WINUSERAPI BOOL    WINAPI CSSetMenuDefaultItem( IN HCSMENU hMenu, IN UINT uItem,  IN UINT fByPos);

WINUSERAPI BOOL    WINAPI CSGetMenuItemRect( IN HCSWND hWnd, IN HCSMENU hMenu, IN UINT uItem, OUT LPCSRECT lprcItem);
WINUSERAPI int     WINAPI CSMenuItemFromPoint( IN HCSWND hWnd, IN HCSMENU hMenu, IN CSPOINT ptScreen);
#endif /* WINVER >= 0x0400 */

/*
 * Flags for CSTrackPopupMenu
 */
#define TPM_LEFTBUTTON  0x0000L
#define TPM_RIGHTBUTTON 0x0002L
#define TPM_LEFTALIGN   0x0000L
#define TPM_CENTERALIGN 0x0004L
#define TPM_RIGHTALIGN  0x0008L
#if(WINVER >= 0x0400)
#define TPM_TOPALIGN        0x0000L
#define TPM_VCENTERALIGN    0x0010L
#define TPM_BOTTOMALIGN     0x0020L

#define TPM_HORIZONTAL      0x0000L     /* Horz alignment matters more */
#define TPM_VERTICAL        0x0040L     /* Vert alignment matters more */
#define TPM_NONOTIFY        0x0080L     /* Don't send any notification msgs */
#define TPM_RETURNCMD       0x0100L
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
#define TPM_RECURSE         0x0001L
#define TPM_HORPOSANIMATION 0x0400L
#define TPM_HORNEGANIMATION 0x0800L
#define TPM_VERPOSANIMATION 0x1000L
#define TPM_VERNEGANIMATION 0x2000L
#if(_WIN32_WINNT >= 0x0500)
#define TPM_NOANIMATION     0x4000L
#endif /* _WIN32_WINNT >= 0x0500 */
#if(_WIN32_WINNT >= 0x0501)
#define TPM_LAYOUTRTL       0x8000L
#endif /* _WIN32_WINNT >= 0x0501 */
#endif /* WINVER >= 0x0500 */


#endif /* !NOMENUS */


#if(WINVER >= 0x0400)
//
// CSDrag-and-drop support
// Obsolete - use OLE instead
//
typedef struct tagCSDROPSTRUCT
{
    HCSWND    hwndSource;
    HCSWND    hwndSink;
    DWORD   wFmt;
    ULONG_PTR dwData;
    CSPOINT   ptDrop;
    DWORD   dwControlData;
} CSDROPSTRUCT, *PCSDROPSTRUCT, *LPCSDROPSTRUCT;

#define DOF_EXECUTABLE      0x8001      // wFmt flags
#define DOF_DOCUMENT        0x8002
#define DOF_DIRECTORY       0x8003
#define DOF_MULTIPLE        0x8004
#define DOF_PROGMAN         0x0001
#define DOF_SHELLDATA       0x0002

#define DO_DROPFILE         0x454C4946L
#define DO_PRINTFILE        0x544E5250L

WINUSERAPI
DWORD
WINAPI
CSDragObject(
    IN HCSWND,
    IN HCSWND,
    IN UINT,
    IN ULONG_PTR,
    IN HCSCURSOR);

WINUSERAPI
BOOL
WINAPI
CSDragDetect(
    IN HCSWND,
    IN CSPOINT);
#endif /* WINVER >= 0x0400 */

WINUSERAPI
BOOL
WINAPI
CSDrawIcon(
    IN HCSDC hDC,
    IN int X,
    IN int Y,
    IN HCSICON hIcon);

#ifndef NODRAWTEXT

/*
 * CSDrawText() Format Flags
 */
#define DT_TOP                      0x00000000
#define DT_LEFT                     0x00000000
#define DT_CENTER                   0x00000001
#define DT_RIGHT                    0x00000002
#define DT_VCENTER                  0x00000004
#define DT_BOTTOM                   0x00000008
#define DT_WORDBREAK                0x00000010
#define DT_SINGLELINE               0x00000020
#define DT_EXPANDTABS               0x00000040
#define DT_TABSTOP                  0x00000080
#define DT_NOCLIP                   0x00000100
#define DT_EXTERNALLEADING          0x00000200
#define DT_CALCRECT                 0x00000400
#define DT_NOPREFIX                 0x00000800
#define DT_INTERNAL                 0x00001000

#if(WINVER >= 0x0400)
#define DT_EDITCONTROL              0x00002000
#define DT_PATH_ELLIPSIS            0x00004000
#define DT_END_ELLIPSIS             0x00008000
#define DT_MODIFYSTRING             0x00010000
#define DT_RTLREADING               0x00020000
#define DT_WORD_ELLIPSIS            0x00040000
#if(WINVER >= 0x0500)
#define DT_NOFULLWIDTHCHARBREAK     0x00080000
#if(_WIN32_WINNT >= 0x0500)
#define DT_HIDEPREFIX               0x00100000
#define DT_PREFIXONLY               0x00200000
#endif /* _WIN32_WINNT >= 0x0500 */
#endif /* WINVER >= 0x0500 */

typedef struct tagCSDRAWTEXTPARAMS
{
    UINT    cbSize;
    int     iTabLength;
    int     iLeftMargin;
    int     iRightMargin;
    UINT    uiLengthDrawn;
} CSDRAWTEXTPARAMS, FAR *LPCSDRAWTEXTPARAMS;
#endif /* WINVER >= 0x0400 */


WINUSERAPI
int
WINAPI
CSDrawText(
    IN HCSDC hDC,
    IN LPCTSTR lpString,
    IN int nCount,
    IN OUT LPCSRECT lpRect,
    IN UINT uFormat);



#if(WINVER >= 0x0400)
WINUSERAPI
int
WINAPI
CSDrawTextEx(
    IN HCSDC,
    IN OUT LPTSTR,
    IN int,
    IN OUT LPCSRECT,
    IN UINT,
    IN LPCSDRAWTEXTPARAMS);

#endif /* WINVER >= 0x0400 */

#endif /* !NODRAWTEXT */

WINUSERAPI
BOOL
WINAPI
CSGrayString(
    IN HCSDC hDC,
    IN HCSBRUSH hBrush,
    IN CSGRAYSTRINGPROC lpOutputFunc,
    IN LPARAM lpData,
    IN int nCount,
    IN int X,
    IN int Y,
    IN int nWidth,
    IN int nHeight);

#if(WINVER >= 0x0400)
/* Monolithic state-drawing routine */
/* Image type */
#define DST_COMPLEX     0x0000
#define DST_TEXT        0x0001
#define DST_PREFIXTEXT  0x0002
#define DST_ICON        0x0003
#define DST_BITMAP      0x0004

/* State type */
#define DSS_NORMAL      0x0000
#define DSS_UNION       0x0010  /* CSGray string appearance */
#define DSS_DISABLED    0x0020
#define DSS_MONO        0x0080
#if(_WIN32_WINNT >= 0x0500)
#define DSS_HIDEPREFIX  0x0200
#define DSS_PREFIXONLY  0x0400
#endif /* _WIN32_WINNT >= 0x0500 */
#define DSS_RIGHT       0x8000

WINUSERAPI
BOOL
WINAPI
CSDrawState(
    IN HCSDC,
    IN HCSBRUSH,
    IN CSDRAWSTATEPROC,
    IN LPARAM,
    IN WPARAM,
    IN int,
    IN int,
    IN int,
    IN int,
    IN UINT);

#endif /* WINVER >= 0x0400 */

WINUSERAPI
LONG
WINAPI
CSTabbedTextOut(
    IN HCSDC hDC,
    IN int X,
    IN int Y,
    IN LPCTSTR lpString,
    IN int nCount,
    IN int nTabPositions,
    IN CONST LPINT lpnTabStopPositions,
    IN int nTabOrigin);


WINUSERAPI
DWORD
WINAPI
CSGetTabbedTextExtent(
    IN HCSDC hDC,
    IN LPCTSTR lpString,
    IN int nCount,
    IN int nTabPositions,
    IN CONST INT *lpnTabStopPositions);

WINUSERAPI
BOOL
WINAPI
CSUpdateWindow(
    IN HCSWND hWnd);

WINUSERAPI
HCSWND
WINAPI
CSSetActiveWindow(
    IN HCSWND hWnd);

WINUSERAPI
HCSWND
WINAPI
CSGetForegroundWindow(
    void);

#if(WINVER >= 0x0400)
WINUSERAPI
BOOL
WINAPI
CSPaintDesktop(
    IN HCSDC hdc);

#endif /* WINVER >= 0x0400 */

WINUSERAPI
BOOL
WINAPI
CSSetForegroundWindow(
    IN HCSWND hWnd);

#if(_WIN32_WINNT >= 0x0500)
WINUSERAPI
BOOL
WINAPI
CSAllowSetForegroundWindow(
    DWORD dwProcessId);

#define ASFW_ANY    ((DWORD)-1)

WINUSERAPI
BOOL
WINAPI
CSLockSetForegroundWindow(
    UINT uLockCode);

#define LSFW_LOCK       1
#define LSFW_UNLOCK     2

#endif /* _WIN32_WINNT >= 0x0500 */

WINUSERAPI
HCSWND
WINAPI
CSWindowFromDC(
    IN HCSDC hDC);

WINUSERAPI
HCSDC
WINAPI
CSGetDC(
    IN HCSWND hWnd);

WINUSERAPI
HCSDC
WINAPI
CSGetDCEx(
    IN HCSWND hWnd,
    IN HCSRGN hrgnClip,
    IN DWORD flags);

/*
 * CSGetDCEx() flags
 */
#define DCX_WINDOW           0x00000001L
#define DCX_CACHE            0x00000002L
#define DCX_NORESETATTRS     0x00000004L
#define DCX_CLIPCHILDREN     0x00000008L
#define DCX_CLIPSIBLINGS     0x00000010L
#define DCX_PARENTCLIP       0x00000020L
#define DCX_EXCLUDERGN       0x00000040L
#define DCX_INTERSECTRGN     0x00000080L
#define DCX_EXCLUDEUPDATE    0x00000100L
#define DCX_INTERSECTUPDATE  0x00000200L
#define DCX_LOCKWINDOWUPDATE 0x00000400L

#define DCX_VALIDATE         0x00200000L

WINUSERAPI
HCSDC
WINAPI
CSGetWindowDC(
    IN HCSWND hWnd);

WINUSERAPI
int
WINAPI
CSReleaseDC(
    IN HCSWND hWnd,
    IN HCSDC hDC);

WINUSERAPI
HCSDC
WINAPI
CSBeginPaint(
    IN HCSWND hWnd,
    OUT LPCSPAINTSTRUCT lpPaint);

WINUSERAPI
BOOL
WINAPI
CSEndPaint(
    IN HCSWND hWnd,
    IN CONST CSPAINTSTRUCT *lpPaint);

WINUSERAPI
BOOL
WINAPI
CSGetUpdateRect(
    IN HCSWND hWnd,
    OUT LPCSRECT lpRect,
    IN BOOL bErase);

WINUSERAPI
int
WINAPI
CSGetUpdateRgn(
    IN HCSWND hWnd,
    IN HCSRGN hRgn,
    IN BOOL bErase);

WINUSERAPI
int
WINAPI
CSSetWindowRgn(
    IN HCSWND hWnd,
    IN HCSRGN hRgn,
    IN BOOL bRedraw);


WINUSERAPI
int
WINAPI
CSGetWindowRgn(
    IN HCSWND hWnd,
    IN HCSRGN hRgn);

#if(_WIN32_WINNT >= 0x0501)

WINUSERAPI
int
WINAPI
CSGetWindowRgnBox(
    IN HCSWND hWnd,
    OUT LPCSRECT lprc);

#endif /* _WIN32_WINNT >= 0x0501 */

WINUSERAPI
int
WINAPI
CSExcludeUpdateRgn(
    IN HCSDC hDC,
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSInvalidateRect(
    IN HCSWND hWnd,
    IN CONST CSRECT *lpRect,
    IN BOOL bErase);

WINUSERAPI
BOOL
WINAPI
CSValidateRect(
    IN HCSWND hWnd,
    IN CONST CSRECT *lpRect);

WINUSERAPI
BOOL
WINAPI
CSInvalidateRgn(
    IN HCSWND hWnd,
    IN HCSRGN hRgn,
    IN BOOL bErase);

WINUSERAPI
BOOL
WINAPI
CSValidateRgn(
    IN HCSWND hWnd,
    IN HCSRGN hRgn);


WINUSERAPI
BOOL
WINAPI
CSRedrawWindow(
    IN HCSWND hWnd,
    IN CONST CSRECT *lprcUpdate,
    IN HCSRGN hrgnUpdate,
    IN UINT flags);

/*
 * RedrawWindow() flags
 */
#define RDW_INVALIDATE          0x0001
#define RDW_INTERNALPAINT       0x0002
#define RDW_ERASE               0x0004

#define RDW_VALIDATE            0x0008
#define RDW_NOINTERNALPAINT     0x0010
#define RDW_NOERASE             0x0020

#define RDW_NOCHILDREN          0x0040
#define RDW_ALLCHILDREN         0x0080

#define RDW_UPDATENOW           0x0100
#define RDW_ERASENOW            0x0200

#define RDW_FRAME               0x0400
#define RDW_NOFRAME             0x0800


/*
 * LockWindowUpdate API
 */

WINUSERAPI
BOOL
WINAPI
CSLockWindowUpdate(
    IN HCSWND hWndLock);

WINUSERAPI
BOOL
WINAPI
CSScrollWindow(
    IN HCSWND hWnd,
    IN int XAmount,
    IN int YAmount,
    IN CONST CSRECT *lpRect,
    IN CONST CSRECT *lpClipRect);

WINUSERAPI
BOOL
WINAPI
CSScrollDC(
    IN HCSDC hDC,
    IN int dx,
    IN int dy,
    IN CONST CSRECT *lprcScroll,
    IN CONST CSRECT *lprcClip,
    IN HCSRGN hrgnUpdate,
    OUT LPCSRECT lprcUpdate);

WINUSERAPI
int
WINAPI
CSScrollWindowEx(
    IN HCSWND hWnd,
    IN int dx,
    IN int dy,
    IN CONST CSRECT *prcScroll,
    IN CONST CSRECT *prcClip,
    IN HCSRGN hrgnUpdate,
    OUT LPCSRECT prcUpdate,
    IN UINT flags);

#define SW_SCROLLCHILDREN   0x0001  /* Scroll children within *lprcScroll. */
#define SW_INVALIDATE       0x0002  /* Invalidate after scrolling */
#define SW_ERASE            0x0004  /* If SW_INVALIDATE, don't send WM_ERASEBACKGROUND */
#if(WINVER >= 0x0500)
#define SW_SMOOTHSCROLL     0x0010  /* Use smooth scrolling */
#endif /* WINVER >= 0x0500 */

#ifndef NOSCROLL

WINUSERAPI
int
WINAPI
CSSetScrollPos(
    IN HCSWND hWnd,
    IN int nBar,
    IN int nPos,
    IN BOOL bRedraw);

WINUSERAPI
int
WINAPI
CSGetScrollPos(
    IN HCSWND hWnd,
    IN int nBar);

WINUSERAPI
BOOL
WINAPI
CSSetScrollRange(
    IN HCSWND hWnd,
    IN int nBar,
    IN int nMinPos,
    IN int nMaxPos,
    IN BOOL bRedraw);

WINUSERAPI
BOOL
WINAPI
CSGetScrollRange(
    IN HCSWND hWnd,
    IN int nBar,
    OUT LPINT lpMinPos,
    OUT LPINT lpMaxPos);

WINUSERAPI
BOOL
WINAPI
CSShowScrollBar(
    IN HCSWND hWnd,
    IN int wBar,
    IN BOOL bShow);

WINUSERAPI
BOOL
WINAPI
CSEnableScrollBar(
    IN HCSWND hWnd,
    IN UINT wSBflags,
    IN UINT wArrows);


/*
 * CSEnableScrollBar() flags
 */
#define ESB_ENABLE_BOTH     0x0000
#define ESB_DISABLE_BOTH    0x0003

#define ESB_DISABLE_LEFT    0x0001
#define ESB_DISABLE_RIGHT   0x0002

#define ESB_DISABLE_UP      0x0001
#define ESB_DISABLE_DOWN    0x0002

#define ESB_DISABLE_LTUP    ESB_DISABLE_LEFT
#define ESB_DISABLE_RTDN    ESB_DISABLE_RIGHT


#endif  /* !NOSCROLL */

WINUSERAPI
BOOL
WINAPI
CSSetProp(
    IN HCSWND hWnd,
    IN LPCTSTR lpString,
    IN HCSHANDLE hData);


WINUSERAPI
HCSHANDLE
WINAPI
CSGetProp(
    IN HCSWND hWnd,
    IN LPCTSTR lpString);


WINUSERAPI
HCSHANDLE
WINAPI
CSRemoveProp(
    IN HCSWND hWnd,
    IN LPCTSTR lpString);

WINUSERAPI
int
WINAPI
CSEnumPropsEx(
    IN HCSWND hWnd,
    IN CSPROPENUMPROCEXA lpEnumFunc,
    IN LPARAM lParam);

WINUSERAPI
int
WINAPI
CSEnumProps(
    IN HCSWND hWnd,
    IN CSPROPENUMPROCA lpEnumFunc);

WINUSERAPI
BOOL
WINAPI
CSSetWindowText(
    IN HCSWND hWnd,
    IN LPCTSTR lpString);


WINUSERAPI
int
WINAPI
CSGetWindowText(
    IN HCSWND hWnd,
    OUT LPTSTR lpString,
    IN int nMaxCount);


WINUSERAPI
int
WINAPI
CSGetWindowTextLength(
    IN HCSWND hWnd);


WINUSERAPI
BOOL
WINAPI
CSGetClientRect(
    IN HCSWND hWnd,
    OUT LPCSRECT lpRect);

WINUSERAPI
BOOL
WINAPI
CSGetWindowRect(
    IN HCSWND hWnd,
    OUT LPCSRECT lpRect);

WINUSERAPI
BOOL
WINAPI
CSAdjustWindowRect(
    IN OUT LPCSRECT lpRect,
    IN DWORD dwStyle,
    IN BOOL bMenu);

WINUSERAPI
BOOL
WINAPI
CSAdjustWindowRectEx(
    IN OUT LPCSRECT lpRect,
    IN DWORD dwStyle,
    IN BOOL bMenu,
    IN DWORD dwExStyle);


#if(WINVER >= 0x0400)
#define HELPINFO_WINDOW    0x0001
#define HELPINFO_MENUITEM  0x0002
typedef struct tagCSHELPINFO      /* Structure pointed to by lParam of WM_HELP */
{
    UINT    cbSize;             /* Size in bytes of this struct  */
    int     iContextType;       /* Either HELPINFO_WINDOW or HELPINFO_MENUITEM */
    int     iCtrlId;            /* Control Id or a Menu item Id. */
    HCSHANDLE  hItemHandle;        /* hWnd of control or hMenu.     */
    DWORD_PTR dwContextId;      /* Context Id associated with this item */
    CSPOINT   MousePos;           /* Mouse Position in screen co-ordinates */
}  CSHELPINFO, FAR *LPCSHELPINFO;

WINUSERAPI
BOOL
WINAPI
CSSetWindowContextHelpId(
    IN HCSWND,
    IN DWORD);

WINUSERAPI
DWORD
WINAPI
CSGetWindowContextHelpId(
    IN HCSWND);

WINUSERAPI
BOOL
WINAPI
CSSetMenuContextHelpId(
    IN HCSMENU,
    IN DWORD);

WINUSERAPI
DWORD
WINAPI
CSGetMenuContextHelpId(
    IN HCSMENU);

#endif /* WINVER >= 0x0400 */


#ifndef NOMB

/*
 * MessageBox() Flags
 */
#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#if(WINVER >= 0x0500)
#define MB_CANCELTRYCONTINUE        0x00000006L
#endif /* WINVER >= 0x0500 */


#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L

#if(WINVER >= 0x0400)
#define MB_USERICON                 0x00000080L
#define MB_ICONWARNING              MB_ICONEXCLAMATION
#define MB_ICONERROR                MB_ICONHAND
#endif /* WINVER >= 0x0400 */

#define MB_ICONINFORMATION          MB_ICONASTERISK
#define MB_ICONSTOP                 MB_ICONHAND

#define MB_DEFBUTTON1               0x00000000L
#define MB_DEFBUTTON2               0x00000100L
#define MB_DEFBUTTON3               0x00000200L
#if(WINVER >= 0x0400)
#define MB_DEFBUTTON4               0x00000300L
#endif /* WINVER >= 0x0400 */

#define MB_APPLMODAL                0x00000000L
#define MB_SYSTEMMODAL              0x00001000L
#define MB_TASKMODAL                0x00002000L
#if(WINVER >= 0x0400)
#define MB_HELP                     0x00004000L // Help Button
#endif /* WINVER >= 0x0400 */

#define MB_NOFOCUS                  0x00008000L
#define MB_SETFOREGROUND            0x00010000L
#define MB_DEFAULT_DESKTOP_ONLY     0x00020000L

#if(WINVER >= 0x0400)
#define MB_TOPMOST                  0x00040000L
#define MB_RIGHT                    0x00080000L
#define MB_RTLREADING               0x00100000L


#endif /* WINVER >= 0x0400 */

#ifdef _WIN32_WINNT
#if (_WIN32_WINNT >= 0x0400)
#define MB_SERVICE_NOTIFICATION          0x00200000L
#else
#define MB_SERVICE_NOTIFICATION          0x00040000L
#endif
#define MB_SERVICE_NOTIFICATION_NT3X     0x00040000L
#endif

#define MB_TYPEMASK                 0x0000000FL
#define MB_ICONMASK                 0x000000F0L
#define MB_DEFMASK                  0x00000F00L
#define MB_MODEMASK                 0x00003000L
#define MB_MISCMASK                 0x0000C000L

WINUSERAPI
int
WINAPI
CSMessageBox(
    IN HCSWND hWnd,
    IN LPCTSTR lpText,
    IN LPCTSTR lpCaption,
    IN UINT uType);

WINUSERAPI
int
WINAPI
CSMessageBoxEx(
    IN HCSWND hWnd,
    IN LPCTSTR lpText,
    IN LPCTSTR lpCaption,
    IN UINT uType,
    IN WORD wLanguageId);

#if(WINVER >= 0x0400)

typedef void (CALLBACK *CSMSGBOXCALLBACK)(LPCSHELPINFO lpHelpInfo);

typedef struct tagCSMSGBOXPARAMSA
{
    UINT        cbSize;
    HCSWND        hwndOwner;
    HCSINSTANCE   hInstance;
    LPCTSTR      lpszText;
    LPCTSTR      lpszCaption;
    DWORD       dwStyle;
    LPCTSTR      lpszIcon;
    DWORD_PTR   dwContextHelpId;
    CSMSGBOXCALLBACK      lpfnMsgBoxCallback;
    DWORD       dwLanguageId;
} CSMSGBOXPARAMSA, *PCSMSGBOXPARAMSA, *LPCSMSGBOXPARAMSA;

typedef CSMSGBOXPARAMSA CSMSGBOXPARAMS;
typedef PCSMSGBOXPARAMSA PCSMSGBOXPARAMS;
typedef LPCSMSGBOXPARAMSA LPCSMSGBOXPARAMS;

WINUSERAPI
int
WINAPI
CSMessageBoxIndirect(
    IN CONST LPCSMSGBOXPARAMS );

#endif /* WINVER >= 0x0400 */


WINUSERAPI
BOOL
WINAPI
CSMessageBeep(
    IN UINT uType);

#endif /* !NOMB */

WINUSERAPI
int
WINAPI
CSShowCursor(
    IN BOOL bShow);

WINUSERAPI
BOOL
WINAPI
CSSetCursorPos(
    IN int X,
    IN int Y);

WINUSERAPI
HCSCURSOR
WINAPI
CSSetCursor(
    IN HCSCURSOR hCursor);

WINUSERAPI
BOOL
WINAPI
CSGetCursorPos(
    OUT LPCSPOINT lpPoint);

WINUSERAPI
BOOL
WINAPI
CSClipCursor(
    IN CONST CSRECT *lpRect);

WINUSERAPI
BOOL
WINAPI
CSGetClipCursor(
    OUT LPCSRECT lpRect);

WINUSERAPI
HCSCURSOR
WINAPI
CSGetCursor(
    void);

WINUSERAPI
BOOL
WINAPI
CSCreateCaret(
    IN HCSWND hWnd,
    IN HCSBITMAP hBitmap,
    IN int nWidth,
    IN int nHeight);

WINUSERAPI
UINT
WINAPI
CSGetCaretBlinkTime(
    void);

WINUSERAPI
BOOL
WINAPI
CSCSSetCaretBlinkTime(
    IN UINT uMSeconds);

WINUSERAPI
BOOL
WINAPI
CSDestroyCaret(
    void);

WINUSERAPI
BOOL
WINAPI
CSHideCaret(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSShowCaret(
    IN HCSWND hWnd);

WINUSERAPI
BOOL
WINAPI
CSCSSetCaretPos(
    IN int X,
    IN int Y);

WINUSERAPI
BOOL
WINAPI
CSGetCaretPos(
    OUT LPCSPOINT lpPoint);

WINUSERAPI
BOOL
WINAPI
CSClientToScreen(
    IN HCSWND hWnd,
    IN OUT LPCSPOINT lpPoint);

WINUSERAPI
BOOL
WINAPI
CSScreenToClient(
    IN HCSWND hWnd,
    IN OUT LPCSPOINT lpPoint);

WINUSERAPI
int
WINAPI
CSMapWindowPoints(
    IN HCSWND hWndFrom,
    IN HCSWND hWndTo,
    IN OUT LPCSPOINT lpPoints,
    IN UINT cPoints);

WINUSERAPI
HCSWND
WINAPI
CSWindowFromPoint(
    IN CSPOINT Point);

WINUSERAPI
HCSWND
WINAPI
CSChildWindowFromPoint(
    IN HCSWND hWndParent,
    IN CSPOINT Point);

#if(WINVER >= 0x0400)
#define CWP_ALL             0x0000
#define CWP_SKIPINVISIBLE   0x0001
#define CWP_SKIPDISABLED    0x0002
#define CWP_SKIPTRANSPARENT 0x0004

WINUSERAPI HCSWND    WINAPI CSChildWindowFromPointEx( IN HCSWND, IN CSPOINT, IN UINT);
#endif /* WINVER >= 0x0400 */

#ifndef NOCOLOR

/*
 * Color Types
 */
#define CTLCOLOR_MSGBOX         0
#define CTLCOLOR_EDIT           1
#define CTLCOLOR_LISTBOX        2
#define CTLCOLOR_BTN            3
#define CTLCOLOR_DLG            4
#define CTLCOLOR_SCROLLBAR      5
#define CTLCOLOR_STATIC         6
#define CTLCOLOR_MAX            7

#define COLOR_SCROLLBAR         0
#define COLOR_BACKGROUND        1
#define COLOR_ACTIVECAPTION     2
#define COLOR_INACTIVECAPTION   3
#define COLOR_MENU              4
#define COLOR_WINDOW            5
#define COLOR_WINDOWFRAME       6
#define COLOR_MENUTEXT          7
#define COLOR_WINDOWTEXT        8
#define COLOR_CAPTIONTEXT       9
#define COLOR_ACTIVEBORDER      10
#define COLOR_INACTIVEBORDER    11
#define COLOR_APPWORKSPACE      12
#define COLOR_HIGHLIGHT         13
#define COLOR_HIGHLIGHTTEXT     14
#define COLOR_BTNFACE           15
#define COLOR_BTNSHADOW         16
#define COLOR_GRAYTEXT          17
#define COLOR_BTNTEXT           18
#define COLOR_INACTIVECAPTIONTEXT 19
#define COLOR_BTNHIGHLIGHT      20

#if(WINVER >= 0x0400)
#define COLOR_3DDKSHADOW        21
#define COLOR_3DLIGHT           22
#define COLOR_INFOTEXT          23
#define COLOR_INFOBK            24
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)
#define COLOR_HOTLIGHT          26
#define COLOR_GRADIENTACTIVECAPTION 27
#define COLOR_GRADIENTINACTIVECAPTION 28
#if(WINVER >= 0x0501)
#define COLOR_MENUHILIGHT       29
#define COLOR_MENUBAR           30
#endif /* WINVER >= 0x0501 */
#endif /* WINVER >= 0x0500 */

#if(WINVER >= 0x0400)
#define COLOR_DESKTOP           COLOR_BACKGROUND
#define COLOR_3DFACE            COLOR_BTNFACE
#define COLOR_3DSHADOW          COLOR_BTNSHADOW
#define COLOR_3DHIGHLIGHT       COLOR_BTNHIGHLIGHT
#define COLOR_3DHILIGHT         COLOR_BTNHIGHLIGHT
#define COLOR_BTNHILIGHT        COLOR_BTNHIGHLIGHT
#endif /* WINVER >= 0x0400 */


WINUSERAPI
DWORD
WINAPI
CSGetSysColor(
    IN int nIndex);

#if(WINVER >= 0x0400)
WINUSERAPI
HCSBRUSH
WINAPI
CSGetSysColorBrush(
    IN int nIndex);


#endif /* WINVER >= 0x0400 */

WINUSERAPI
BOOL
WINAPI
CSSetSysColors(
    IN int cElements,
    IN CONST INT * lpaElements,
    IN CONST COLORREF * lpaRgbValues);

#endif /* !NOCOLOR */

WINUSERAPI
BOOL
WINAPI
CSDrawFocusRect(
    IN HCSDC hDC,
    IN CONST CSRECT * lprc);

WINUSERAPI
int
WINAPI
CSFillRect(
    IN HCSDC hDC,
    IN CONST CSRECT *lprc,
    IN HCSBRUSH hbr);

WINUSERAPI
int
WINAPI
CSFrameRect(
    IN HCSDC hDC,
    IN CONST CSRECT *lprc,
    IN HCSBRUSH hbr);

WINUSERAPI
BOOL
WINAPI
CSInvertRect(
    IN HCSDC hDC,
    IN CONST CSRECT *lprc);

WINUSERAPI
BOOL
WINAPI
CSSetRect(
    OUT LPCSRECT lprc,
    IN int xLeft,
    IN int yTop,
    IN int xRight,
    IN int yBottom);

WINUSERAPI
BOOL
WINAPI
CSSetRectEmpty(
    OUT LPCSRECT lprc);

WINUSERAPI
BOOL
WINAPI
CSCopyRect(
    OUT LPCSRECT lprcDst,
    IN CONST CSRECT *lprcSrc);

WINUSERAPI
BOOL
WINAPI
CSInflateRect(
    IN OUT LPCSRECT lprc,
    IN int dx,
    IN int dy);

WINUSERAPI
BOOL
WINAPI
CSIntersectRect(
    OUT LPCSRECT lprcDst,
    IN CONST CSRECT *lprcSrc1,
    IN CONST CSRECT *lprcSrc2);

WINUSERAPI
BOOL
WINAPI
CSUnionRect(
    OUT LPCSRECT lprcDst,
    IN CONST CSRECT *lprcSrc1,
    IN CONST CSRECT *lprcSrc2);

WINUSERAPI
BOOL
WINAPI
CSSubtractRect(
    OUT LPCSRECT lprcDst,
    IN CONST CSRECT *lprcSrc1,
    IN CONST CSRECT *lprcSrc2);

WINUSERAPI
BOOL
WINAPI
CSOffsetRect(
    IN OUT LPCSRECT lprc,
    IN int dx,
    IN int dy);

WINUSERAPI
BOOL
WINAPI
CSIsRectEmpty(
    IN CONST CSRECT *lprc);

WINUSERAPI
BOOL
WINAPI
CSEqualRect(
    IN CONST CSRECT *lprc1,
    IN CONST CSRECT *lprc2);

WINUSERAPI
BOOL
WINAPI
CSPtInRect(
    IN CONST CSRECT *lprc,
    IN CSPOINT pt);

#ifndef NOWINOFFSETS

WINUSERAPI
WORD
WINAPI
CSGetWindowWord(
    IN HCSWND hWnd,
    IN int nIndex);

WINUSERAPI
WORD
WINAPI
CSSetWindowWord(
    IN HCSWND hWnd,
    IN int nIndex,
    IN WORD wNewWord);

WINUSERAPI
LONG
WINAPI
CSGetWindowLong(
    IN HCSWND hWnd,
    IN int nIndex);


WINUSERAPI
LONG
WINAPI
CSSetWindowLong(
    IN HCSWND hWnd,
    IN int nIndex,
    IN LONG dwNewLong);

#ifdef _WIN64

WINUSERAPI
LONG_PTR
WINAPI
CSGetWindowLongPtr(
    HCSWND hWnd,
    int nIndex);


WINUSERAPI
LONG_PTR
WINAPI
CSSetWindowLongPtr(
    HCSWND hWnd,
    int nIndex,
    LONG_PTR dwNewLong);


#else  /* _WIN64 */

#define CSGetWindowLongPtrA   CSGetWindowLongA
#define CSGetWindowLongPtr  CSGetWindowLongPtrA

#define CSSetWindowLongPtrA   CSSetWindowLongA
#define CSSetWindowLongPtr  CSSetWindowLongPtrA

#endif /* _WIN64 */

WINUSERAPI
WORD
WINAPI
CSGetClassWord(
    IN HCSWND hWnd,
    IN int nIndex);

WINUSERAPI
WORD
WINAPI
CSSetClassWord(
    IN HCSWND hWnd,
    IN int nIndex,
    IN WORD wNewWord);

WINUSERAPI
DWORD
WINAPI
CSGetClassLong(
    IN HCSWND hWnd,
    IN int nIndex);

WINUSERAPI
DWORD
WINAPI
CSSetClassLong(
    IN HCSWND hWnd,
    IN int nIndex,
    IN LONG dwNewLong);


#ifdef _WIN64

WINUSERAPI
ULONG_PTR
WINAPI
CSGetClassLongPtr(
    IN HCSWND hWnd,
    IN int nIndex);


WINUSERAPI
ULONG_PTR
WINAPI
CSSetClassLongPtr(
    IN HCSWND hWnd,
    IN int nIndex,
    IN LONG_PTR dwNewLong);


#else  /* _WIN64 */

#define CSGetClassLongPtrA    CSGetClassLongA
#define CSGetClassLongPtr  CSGetClassLongPtrA

#define CSSetClassLongPtrA    CSSetClassLongA
#define CSSetClassLongPtr  CSSetClassLongPtrA

#endif /* _WIN64 */

#endif /* !NOWINOFFSETS */

#if(WINVER >= 0x0500)
WINUSERAPI
BOOL
WINAPI
CSGetProcessDefaultLayout(
    OUT DWORD *pdwDefaultLayout);

WINUSERAPI
BOOL
WINAPI
CSSetProcessDefaultLayout(
    IN DWORD dwDefaultLayout);
#endif /* WINVER >= 0x0500 */

WINUSERAPI
HCSWND
WINAPI
CSGetDesktopWindow(
    void);


WINUSERAPI
HCSWND
WINAPI
CSGetParent(
    IN HCSWND hWnd);

WINUSERAPI
HCSWND
WINAPI
CSSetParent(
    IN HCSWND hWndChild,
    IN HCSWND hWndNewParent);

WINUSERAPI
BOOL
WINAPI
CSEnumChildWindows(
    IN HCSWND hWndParent,
    IN CSWNDENUMPROC lpEnumFunc,
    IN LPARAM lParam);

WINUSERAPI
HCSWND
WINAPI
CSFindWindow(
    IN LPCTSTR lpClassName,
    IN LPCTSTR lpWindowName);


#if(WINVER >= 0x0400)
WINUSERAPI HCSWND    WINAPI CSFindWindowEx( IN HCSWND, IN HCSWND, IN LPCTSTR, IN LPCTSTR);
#endif /* WINVER >= 0x0400 */


WINUSERAPI
BOOL
WINAPI
CSEnumWindows(
    IN CSWNDENUMPROC lpEnumFunc,
    IN LPARAM lParam);

WINUSERAPI
BOOL
WINAPI
CSEnumThreadWindows(
    IN DWORD dwThreadId,
    IN CSWNDENUMPROC lpfn,
    IN LPARAM lParam);

#define CSEnumTaskWindows(hTask, lpfn, lParam) CSEnumThreadWindows(HandleToUlong(hTask), lpfn, lParam)

WINUSERAPI
int
WINAPI
CSGetClassName(
    IN HCSWND hWnd,
    OUT LPTSTR lpClassName,
    IN int nMaxCount);


WINUSERAPI
HCSWND
WINAPI
CSGetTopWindow(
    IN HCSWND hWnd);

#define CSGetNextWindow(hWnd, wCmd) CSGetWindow(hWnd, wCmd)
#define CSGetSysModalWindow() (NULL)
#define CSSetSysModalWindow(hWnd) (NULL)

WINUSERAPI
DWORD
WINAPI
CSGetWindowThreadProcessId(
    IN HCSWND hWnd,
    OUT LPDWORD lpdwProcessId);

#if(_WIN32_WINNT >= 0x0501)
WINUSERAPI
BOOL
WINAPI
CSIsGUIThread(
    BOOL bConvert);

#endif /* _WIN32_WINNT >= 0x0501 */

#define CSGetWindowTask(hWnd) \
        ((HCSHANDLE)(DWORD_PTR)CSGetWindowThreadProcessId(hWnd, NULL))

WINUSERAPI
HCSWND
WINAPI
CSGetLastActivePopup(
    IN HCSWND hWnd);

/*
 * CSGetWindow() Constants
 */
#define GW_HWNDFIRST        0
#define GW_HWNDLAST         1
#define GW_HWNDNEXT         2
#define GW_HWNDPREV         3
#define GW_OWNER            4
#define GW_CHILD            5
#if(WINVER <= 0x0400)
#define GW_MAX              5
#else
#define GW_ENABLEDPOPUP     6
#define GW_MAX              6
#endif

WINUSERAPI
HCSWND
WINAPI
CSGetWindow(
    IN HCSWND hWnd,
    IN UINT uCmd);



#ifndef NOWH

#ifdef STRICT

WINUSERAPI
HCSHOOK
WINAPI
CSSetWindowsHook(
    IN int nFilterType,
    IN CSHOOKPROC pfnFilterProc);


#else /* !STRICT */

WINUSERAPI
CSHOOKPROC
WINAPI
CSSetWindowsHook(
    IN int nFilterType,
    IN CSHOOKPROC pfnFilterProc);

#endif /* !STRICT */

WINUSERAPI
BOOL
WINAPI
CSUnhookWindowsHook(
    IN int nCode,
    IN CSHOOKPROC pfnFilterProc);

WINUSERAPI
HCSHOOK
WINAPI
CSSetWindowsHookEx(
    IN int idHook,
    IN CSHOOKPROC lpfn,
    IN HCSINSTANCE hmod,
    IN DWORD dwThreadId);


WINUSERAPI
BOOL
WINAPI
CSUnhookWindowsHookEx(
    IN HCSHOOK hhk);

WINUSERAPI
LRESULT
WINAPI
CSCallNextHookEx(
    IN HCSHOOK hhk,
    IN int nCode,
    IN WPARAM wParam,
    IN LPARAM lParam);

/*
 * Macros for source-level compatibility with old functions.
 */
#ifdef STRICT
#define CSDefHookProc(nCode, wParam, lParam, phhk)\
        CSCallNextHookEx(*phhk, nCode, wParam, lParam)
#else
#define CSDefHookProc(nCode, wParam, lParam, phhk)\
        CSCallNextHookEx((HCSHOOK)*phhk, nCode, wParam, lParam)
#endif /* STRICT */
#endif /* !NOWH */

#ifndef NOMENUS


/* ;win40  -- A lot of MF_* flags have been renamed as MFT_* and MFS_* flags */
/*
 * Menu flags for Add/Check/CSEnableMenuItem()
 */
#define MF_INSERT           0x00000000L
#define MF_CHANGE           0x00000080L
#define MF_APPEND           0x00000100L
#define MF_DELETE           0x00000200L
#define MF_REMOVE           0x00001000L

#define MF_BYCOMMAND        0x00000000L
#define MF_BYPOSITION       0x00000400L

#define MF_SEPARATOR        0x00000800L

#define MF_ENABLED          0x00000000L
#define MF_GRAYED           0x00000001L
#define MF_DISABLED         0x00000002L

#define MF_UNCHECKED        0x00000000L
#define MF_CHECKED          0x00000008L
#define MF_USECHECKBITMAPS  0x00000200L

#define MF_STRING           0x00000000L
#define MF_BITMAP           0x00000004L
#define MF_OWNERDRAW        0x00000100L

#define MF_POPUP            0x00000010L
#define MF_MENUBARBREAK     0x00000020L
#define MF_MENUBREAK        0x00000040L

#define MF_UNHILITE         0x00000000L
#define MF_HILITE           0x00000080L

#if(WINVER >= 0x0400)
#define MF_DEFAULT          0x00001000L
#endif /* WINVER >= 0x0400 */
#define MF_SYSMENU          0x00002000L
#define MF_HELP             0x00004000L
#if(WINVER >= 0x0400)
#define MF_RIGHTJUSTIFY     0x00004000L
#endif /* WINVER >= 0x0400 */

#define MF_MOUSESELECT      0x00008000L
#if(WINVER >= 0x0400)
#define MF_END              0x00000080L  /* Obsolete -- only used by old RES files */
#endif /* WINVER >= 0x0400 */


#if(WINVER >= 0x0400)
#define MFT_STRING          MF_STRING
#define MFT_BITMAP          MF_BITMAP
#define MFT_MENUBARBREAK    MF_MENUBARBREAK
#define MFT_MENUBREAK       MF_MENUBREAK
#define MFT_OWNERDRAW       MF_OWNERDRAW
#define MFT_RADIOCHECK      0x00000200L
#define MFT_SEPARATOR       MF_SEPARATOR
#define MFT_RIGHTORDER      0x00002000L
#define MFT_RIGHTJUSTIFY    MF_RIGHTJUSTIFY

/* Menu flags for Add/Check/CSEnableMenuItem() */
#define MFS_GRAYED          0x00000003L
#define MFS_DISABLED        MFS_GRAYED
#define MFS_CHECKED         MF_CHECKED
#define MFS_HILITE          MF_HILITE
#define MFS_ENABLED         MF_ENABLED
#define MFS_UNCHECKED       MF_UNCHECKED
#define MFS_UNHILITE        MF_UNHILITE
#define MFS_DEFAULT         MF_DEFAULT
#endif /* WINVER >= 0x0400 */


#if(WINVER >= 0x0400)

WINUSERAPI
BOOL
WINAPI
CSCheckMenuRadioItem(
    IN HCSMENU,
    IN UINT,
    IN UINT,
    IN UINT,
    IN UINT);
#endif /* WINVER >= 0x0400 */

/*
 * Menu item resource format
 */
typedef struct {
    WORD versionNumber;
    WORD offset;
} CSMENUITEMTEMPLATEHEADER, *PCSMENUITEMTEMPLATEHEADER;

typedef struct {        // version 0
    WORD mtOption;
    WORD mtID;
    WCHAR mtString[1];
} CSMENUITEMTEMPLATE, *PCSMENUITEMTEMPLATE;
#define MF_END             0x00000080L

#endif /* !NOMENUS */

#ifndef NOSYSCOMMANDS

/*
 * System Menu Command Values
 */
#define SC_SIZE         0xF000
#define SC_MOVE         0xF010
#define SC_MINIMIZE     0xF020
#define SC_MAXIMIZE     0xF030
#define SC_NEXTWINDOW   0xF040
#define SC_PREVWINDOW   0xF050
#define SC_CLOSE        0xF060
#define SC_VSCROLL      0xF070
#define SC_HSCROLL      0xF080
#define SC_MOUSEMENU    0xF090
#define SC_KEYMENU      0xF100
#define SC_ARRANGE      0xF110
#define SC_RESTORE      0xF120
#define SC_TASKLIST     0xF130
#define SC_SCREENSAVE   0xF140
#define SC_HOTKEY       0xF150
#if(WINVER >= 0x0400)
#define SC_DEFAULT      0xF160
#define SC_MONITORPOWER 0xF170
#define SC_CONTEXTHELP  0xF180
#define SC_SEPARATOR    0xF00F
#endif /* WINVER >= 0x0400 */

/*
 * Obsolete names
 */
#define SC_ICON         SC_MINIMIZE
#define SC_ZOOM         SC_MAXIMIZE

#endif /* !NOSYSCOMMANDS */

/*
 * Resource CSLoading Routines
 */

WINUSERAPI
HCSBITMAP
WINAPI
CSLoadBitmap(
    IN HCSINSTANCE hInstance,
    IN LPCTSTR lpBitmapName);


WINUSERAPI
HCSCURSOR
WINAPI
CSLoadCursor(
    IN HCSINSTANCE hInstance,
    IN LPCTSTR lpCursorName);


WINUSERAPI
HCSCURSOR
WINAPI
CSLoadCursorFromFile(
    IN LPCTSTR lpFileName);


WINUSERAPI
HCSCURSOR
WINAPI
CSCreateCursor(
    IN HCSINSTANCE hInst,
    IN int xHotSpot,
    IN int yHotSpot,
    IN int nWidth,
    IN int nHeight,
    IN CONST void *pvANDPlane,
    IN CONST void *pvXORPlane);

WINUSERAPI
BOOL
WINAPI
CSDestroyCursor(
    IN HCSCURSOR hCursor);

#ifndef _MAC
#define CSCopyCursor(pcur) ((HCSCURSOR)CSCopyIcon((HCSICON)(pcur)))
#else
WINUSERAPI
HCSCURSOR
WINAPI
CSCopyCursor(
    IN HCSCURSOR hCursor);
#endif

/*
 * Standard Cursor IDs
 */
#define IDC_ARROW           MAKEINTRESOURCE(32512)
#define IDC_IBEAM           MAKEINTRESOURCE(32513)
#define IDC_WAIT            MAKEINTRESOURCE(32514)
#define IDC_CROSS           MAKEINTRESOURCE(32515)
#define IDC_UPARROW         MAKEINTRESOURCE(32516)
#define IDC_SIZE            MAKEINTRESOURCE(32640)  /* OBSOLETE: use IDC_SIZEALL */
#define IDC_ICON            MAKEINTRESOURCE(32641)  /* OBSOLETE: use IDC_ARROW */
#define IDC_SIZENWSE        MAKEINTRESOURCE(32642)
#define IDC_SIZENESW        MAKEINTRESOURCE(32643)
#define IDC_SIZEWE          MAKEINTRESOURCE(32644)
#define IDC_SIZENS          MAKEINTRESOURCE(32645)
#define IDC_SIZEALL         MAKEINTRESOURCE(32646)
#define IDC_NO              MAKEINTRESOURCE(32648) /*not in win3.1 */
#if(WINVER >= 0x0500)
#define IDC_HAND            MAKEINTRESOURCE(32649)
#endif /* WINVER >= 0x0500 */
#define IDC_APPSTARTING     MAKEINTRESOURCE(32650) /*not in win3.1 */
#if(WINVER >= 0x0400)
#define IDC_HELP            MAKEINTRESOURCE(32651)
#endif /* WINVER >= 0x0400 */

WINUSERAPI
BOOL
WINAPI
CSSetSystemCursor(
    IN HCSCURSOR hcur,
    IN DWORD   id);

typedef struct _CSICONINFO {
    BOOL    fIcon;
    DWORD   xHotspot;
    DWORD   yHotspot;
    HCSBITMAP hbmMask;
    HCSBITMAP hbmColor;
} CSICONINFO;
typedef CSICONINFO *PCSICONINFO;

WINUSERAPI
HCSICON
WINAPI
CSLoadIcon(
    IN HCSINSTANCE hInstance,
    IN LPCTSTR lpIconName);


WINUSERAPI
HCSICON
WINAPI
CSCreateIcon(
    IN HCSINSTANCE hInstance,
    IN int nWidth,
    IN int nHeight,
    IN BYTE cPlanes,
    IN BYTE cBitsPixel,
    IN CONST BYTE *lpbANDbits,
    IN CONST BYTE *lpbXORbits);

WINUSERAPI
BOOL
WINAPI
CSDestroyIcon(
    IN HCSICON hIcon);

WINUSERAPI
int
WINAPI
CSLookupIconIdFromDirectory(
    IN PBYTE presbits,
    IN BOOL fIcon);

#if(WINVER >= 0x0400)
WINUSERAPI
int
WINAPI
CSLookupIconIdFromDirectoryEx(
    IN PBYTE presbits,
    IN BOOL  fIcon,
    IN int   cxDesired,
    IN int   cyDesired,
    IN UINT  Flags);
#endif /* WINVER >= 0x0400 */

WINUSERAPI
HCSICON
WINAPI
CSCreateIconFromResource(
    IN PBYTE presbits,
    IN DWORD dwResSize,
    IN BOOL fIcon,
    IN DWORD dwVer);

#if(WINVER >= 0x0400)
WINUSERAPI
HCSICON
WINAPI
CSCreateIconFromResourceEx(
    IN PBYTE presbits,
    IN DWORD dwResSize,
    IN BOOL  fIcon,
    IN DWORD dwVer,
    IN int   cxDesired,
    IN int   cyDesired,
    IN UINT  Flags);

/* Icon/Cursor header */
typedef struct tagCSCURSORSHAPE
{
    int     xHotSpot;
    int     yHotSpot;
    int     cx;
    int     cy;
    int     cbWidth;
    BYTE    Planes;
    BYTE    BitsPixel;
} CSCURSORSHAPE, FAR *LPCSCURSORSHAPE;
#endif /* WINVER >= 0x0400 */

#define IMAGE_BITMAP        0
#define IMAGE_ICON          1
#define IMAGE_CURSOR        2
#if(WINVER >= 0x0400)
#define IMAGE_ENHMETAFILE   3

#define LR_DEFAULTCOLOR     0x0000
#define LR_MONOCHROME       0x0001
#define LR_COLOR            0x0002
#define LR_COPYRETURNORG    0x0004
#define LR_COPYDELETEORG    0x0008
#define LR_LOADFROMFILE     0x0010
#define LR_LOADTRANSPARENT  0x0020
#define LR_DEFAULTSIZE      0x0040
#define LR_VGACOLOR         0x0080
#define LR_LOADMAP3DCOLORS  0x1000
#define LR_CREATEDIBSECTION 0x2000
#define LR_COPYFROMRESOURCE 0x4000
#define LR_SHARED           0x8000

WINUSERAPI
HCSHANDLE
WINAPI
CSLoadImage(
    IN HCSINSTANCE,
    IN LPCTSTR,
    IN UINT,
    IN int,
    IN int,
    IN UINT);


WINUSERAPI
HCSHANDLE
WINAPI
CSCopyImage(
    IN HCSHANDLE,
    IN UINT,
    IN int,
    IN int,
    IN UINT);

#define DI_MASK         0x0001
#define DI_IMAGE        0x0002
#define DI_NORMAL       0x0003
#define DI_COMPAT       0x0004
#define DI_DEFAULTSIZE  0x0008
#if(_WIN32_WINNT >= 0x0501)
#define DI_NOMIRROR     0x0010
#endif /* _WIN32_WINNT >= 0x0501 */

WINUSERAPI BOOL WINAPI CSDrawIconEx( IN HCSDC hdc, IN int xLeft, IN int yTop,
              IN HCSICON hIcon, IN int cxWidth, IN int cyWidth,
              IN UINT istepIfAniCur, IN HCSBRUSH hbrFlickerFreeDraw, IN UINT diFlags);
#endif /* WINVER >= 0x0400 */

WINUSERAPI
HCSICON
WINAPI
CSCreateIconIndirect(
    IN PCSICONINFO piconinfo);

WINUSERAPI
HCSICON
WINAPI
CSCopyIcon(
    IN HCSICON hIcon);

WINUSERAPI
BOOL
WINAPI
CSGetIconInfo(
    IN HCSICON hIcon,
    OUT PCSICONINFO piconinfo);

#if(WINVER >= 0x0400)
#define RES_ICON    1
#define RES_CURSOR  2
#endif /* WINVER >= 0x0400 */

#ifdef OEMRESOURCE


/*
 * OEM Resource Ordinal Numbers
 */
#define OBM_CLOSE           32754
#define OBM_UPARROW         32753
#define OBM_DNARROW         32752
#define OBM_RGARROW         32751
#define OBM_LFARROW         32750
#define OBM_REDUCE          32749
#define OBM_ZOOM            32748
#define OBM_RESTORE         32747
#define OBM_REDUCED         32746
#define OBM_ZOOMD           32745
#define OBM_RESTORED        32744
#define OBM_UPARROWD        32743
#define OBM_DNARROWD        32742
#define OBM_RGARROWD        32741
#define OBM_LFARROWD        32740
#define OBM_MNARROW         32739
#define OBM_COMBO           32738
#define OBM_UPARROWI        32737
#define OBM_DNARROWI        32736
#define OBM_RGARROWI        32735
#define OBM_LFARROWI        32734

#define OBM_OLD_CLOSE       32767
#define OBM_SIZE            32766
#define OBM_OLD_UPARROW     32765
#define OBM_OLD_DNARROW     32764
#define OBM_OLD_RGARROW     32763
#define OBM_OLD_LFARROW     32762
#define OBM_BTSIZE          32761
#define OBM_CHECK           32760
#define OBM_CHECKBOXES      32759
#define OBM_BTNCORNERS      32758
#define OBM_OLD_REDUCE      32757
#define OBM_OLD_ZOOM        32756
#define OBM_OLD_RESTORE     32755


#define OCR_NORMAL          32512
#define OCR_IBEAM           32513
#define OCR_WAIT            32514
#define OCR_CROSS           32515
#define OCR_UP              32516
#define OCR_SIZE            32640   /* OBSOLETE: use OCR_SIZEALL */
#define OCR_ICON            32641   /* OBSOLETE: use OCR_NORMAL */
#define OCR_SIZENWSE        32642
#define OCR_SIZENESW        32643
#define OCR_SIZEWE          32644
#define OCR_SIZENS          32645
#define OCR_SIZEALL         32646
#define OCR_ICOCUR          32647   /* OBSOLETE: use OIC_WINLOGO */
#define OCR_NO              32648
#if(WINVER >= 0x0500)
#define OCR_HAND            32649
#endif /* WINVER >= 0x0500 */
#if(WINVER >= 0x0400)
#define OCR_APPSTARTING     32650
#endif /* WINVER >= 0x0400 */


#define OIC_SAMPLE          32512
#define OIC_HAND            32513
#define OIC_QUES            32514
#define OIC_BANG            32515
#define OIC_NOTE            32516
#if(WINVER >= 0x0400)
#define OIC_WINLOGO         32517
#define OIC_WARNING         OIC_BANG
#define OIC_ERROR           OIC_HAND
#define OIC_INFORMATION     OIC_NOTE
#endif /* WINVER >= 0x0400 */



#endif /* OEMRESOURCE */

#define ORD_LANGDRIVER    1     /* The ordinal number for the entry point of
                                ** language drivers.
                                */

#ifndef NOICONS

/*
 * Standard Icon IDs
 */
#ifdef RC_INVOKED
#define IDI_APPLICATION     32512
#define IDI_HAND            32513
#define IDI_QUESTION        32514
#define IDI_EXCLAMATION     32515
#define IDI_ASTERISK        32516
#if(WINVER >= 0x0400)
#define IDI_WINLOGO         32517
#endif /* WINVER >= 0x0400 */
#else
#define IDI_APPLICATION     MAKEINTRESOURCE(32512)
#define IDI_HAND            MAKEINTRESOURCE(32513)
#define IDI_QUESTION        MAKEINTRESOURCE(32514)
#define IDI_EXCLAMATION     MAKEINTRESOURCE(32515)
#define IDI_ASTERISK        MAKEINTRESOURCE(32516)
#if(WINVER >= 0x0400)
#define IDI_WINLOGO         MAKEINTRESOURCE(32517)
#endif /* WINVER >= 0x0400 */
#endif /* RC_INVOKED */

#if(WINVER >= 0x0400)
#define IDI_WARNING     IDI_EXCLAMATION
#define IDI_ERROR       IDI_HAND
#define IDI_INFORMATION IDI_ASTERISK
#endif /* WINVER >= 0x0400 */


#endif /* !NOICONS */

WINUSERAPI
int
WINAPI
CSLoadString(
    IN HCSINSTANCE hInstance,
    IN UINT uID,
    OUT LPTSTR lpBuffer,
    IN int nBufferMax);


/*
 * Dialog Box Command IDs
 */
#define IDOK                1
#define IDCANCEL            2
#define IDABORT             3
#define IDRETRY             4
#define IDIGNORE            5
#define IDYES               6
#define IDNO                7
#if(WINVER >= 0x0400)
#define IDCLOSE         8
#define IDHELP          9
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)
#define IDTRYAGAIN      10
#define IDCONTINUE      11
#endif /* WINVER >= 0x0500 */

#if(WINVER >= 0x0501)
#ifndef IDTIMEOUT
#define IDTIMEOUT 32000
#endif
#endif /* WINVER >= 0x0501 */


#ifndef NOCTLMGR

/*
 * Control Manager Structures and Definitions
 */

#ifndef NOWINSTYLES


/*
 * Edit Control Styles
 */
#define ES_LEFT             0x0000L
#define ES_CENTER           0x0001L
#define ES_RIGHT            0x0002L
#define ES_MULTILINE        0x0004L
#define ES_UPPERCASE        0x0008L
#define ES_LOWERCASE        0x0010L
#define ES_PASSWORD         0x0020L
#define ES_AUTOVSCROLL      0x0040L
#define ES_AUTOHSCROLL      0x0080L
#define ES_NOHIDESEL        0x0100L
#define ES_OEMCONVERT       0x0400L
#define ES_READONLY         0x0800L
#define ES_WANTRETURN       0x1000L
#if(WINVER >= 0x0400)
#define ES_NUMBER           0x2000L
#endif /* WINVER >= 0x0400 */


#endif /* !NOWINSTYLES */

/*
 * Edit Control Notification Codes
 */
#define EN_SETFOCUS         0x0100
#define EN_KILLFOCUS        0x0200
#define EN_CHANGE           0x0300
#define EN_UPDATE           0x0400
#define EN_ERRSPACE         0x0500
#define EN_MAXTEXT          0x0501
#define EN_HSCROLL          0x0601
#define EN_VSCROLL          0x0602

#if(_WIN32_WINNT >= 0x0500)
#define EN_ALIGN_LTR_EC     0x0700
#define EN_ALIGN_RTL_EC     0x0701
#endif /* _WIN32_WINNT >= 0x0500 */

#if(WINVER >= 0x0400)
/* Edit control EM_SETMARGIN parameters */
#define EC_LEFTMARGIN       0x0001
#define EC_RIGHTMARGIN      0x0002
#define EC_USEFONTINFO      0xffff
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)
/* wParam of EM_GET/SETIMESTATUS  */
#define EMSIS_COMPOSITIONSTRING        0x0001

/* lParam for EMSIS_COMPOSITIONSTRING  */
#define EIMES_GETCOMPSTRATONCE         0x0001
#define EIMES_CANCELCOMPSTRINFOCUS     0x0002
#define EIMES_COMPLETECOMPSTRKILLFOCUS 0x0004
#endif /* WINVER >= 0x0500 */

#ifndef NOWINMESSAGES


/*
 * Edit Control CSMessages
 */
#define EM_GETSEL               0x00B0
#define EM_SETSEL               0x00B1
#define EM_GETRECT              0x00B2
#define EM_SETRECT              0x00B3
#define EM_SETRECTNP            0x00B4
#define EM_SCROLL               0x00B5
#define EM_LINESCROLL           0x00B6
#define EM_SCROLLCARET          0x00B7
#define EM_GETMODIFY            0x00B8
#define EM_SETMODIFY            0x00B9
#define EM_GETLINECOUNT         0x00BA
#define EM_LINEINDEX            0x00BB
#define EM_SETHANDLE            0x00BC
#define EM_GETHANDLE            0x00BD
#define EM_GETTHUMB             0x00BE
#define EM_LINELENGTH           0x00C1
#define EM_REPLACESEL           0x00C2
#define EM_GETLINE              0x00C4
#define EM_LIMITTEXT            0x00C5
#define EM_CANUNDO              0x00C6
#define EM_UNDO                 0x00C7
#define EM_FMTLINES             0x00C8
#define EM_LINEFROMCHAR         0x00C9
#define EM_SETTABSTOPS          0x00CB
#define EM_SETPASSWORDCHAR      0x00CC
#define EM_EMPTYUNDOBUFFER      0x00CD
#define EM_GETFIRSTVISIBLELINE  0x00CE
#define EM_SETREADONLY          0x00CF
#define EM_SETWORDBREAKPROC     0x00D0
#define EM_GETWORDBREAKPROC     0x00D1
#define EM_GETPASSWORDCHAR      0x00D2
#if(WINVER >= 0x0400)
#define EM_SETMARGINS           0x00D3
#define EM_GETMARGINS           0x00D4
#define EM_SETLIMITTEXT         EM_LIMITTEXT   /* ;win40 Name change */
#define EM_GETLIMITTEXT         0x00D5
#define EM_POSFROMCHAR          0x00D6
#define EM_CHARFROMPOS          0x00D7
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)
#define EM_SETIMESTATUS         0x00D8
#define EM_GETIMESTATUS         0x00D9
#endif /* WINVER >= 0x0500 */


#endif /* !NOWINMESSAGES */

/*
 * EDITWORDBREAKPROC code values
 */
#define WB_LEFT            0
#define WB_RIGHT           1
#define WB_ISDELIMITER     2


/*
 * Button Control Styles
 */
#define BS_PUSHBUTTON       0x00000000L
#define BS_DEFPUSHBUTTON    0x00000001L
#define BS_CHECKBOX         0x00000002L
#define BS_AUTOCHECKBOX     0x00000003L
#define BS_RADIOBUTTON      0x00000004L
#define BS_3STATE           0x00000005L
#define BS_AUTO3STATE       0x00000006L
#define BS_GROUPBOX         0x00000007L
#define BS_USERBUTTON       0x00000008L
#define BS_AUTORADIOBUTTON  0x00000009L
#define BS_OWNERDRAW        0x0000000BL
#define BS_LEFTTEXT         0x00000020L
#if(WINVER >= 0x0400)
#define BS_TEXT             0x00000000L
#define BS_ICON             0x00000040L
#define BS_BITMAP           0x00000080L
#define BS_LEFT             0x00000100L
#define BS_RIGHT            0x00000200L
#define BS_CENTER           0x00000300L
#define BS_TOP              0x00000400L
#define BS_BOTTOM           0x00000800L
#define BS_VCENTER          0x00000C00L
#define BS_PUSHLIKE         0x00001000L
#define BS_MULTILINE        0x00002000L
#define BS_NOTIFY           0x00004000L
#define BS_FLAT             0x00008000L
#define BS_RIGHTBUTTON      BS_LEFTTEXT
#endif /* WINVER >= 0x0400 */

/*
 * User Button Notification Codes
 */
#define BN_CLICKED          0
#define BN_PAINT            1
#define BN_HILITE           2
#define BN_UNHILITE         3
#define BN_DISABLE          4
#define BN_DOUBLECLICKED    5
#if(WINVER >= 0x0400)
#define BN_PUSHED           BN_HILITE
#define BN_UNPUSHED         BN_UNHILITE
#define BN_DBLCLK           BN_DOUBLECLICKED
#define BN_SETFOCUS         6
#define BN_KILLFOCUS        7
#endif /* WINVER >= 0x0400 */

/*
 * Button Control CSMessages
 */
#define BM_GETCHECK        0x00F0
#define BM_SETCHECK        0x00F1
#define BM_GETSTATE        0x00F2
#define BM_SETSTATE        0x00F3
#define BM_SETSTYLE        0x00F4
#if(WINVER >= 0x0400)
#define BM_CLICK           0x00F5
#define BM_GETIMAGE        0x00F6
#define BM_SETIMAGE        0x00F7

#define BST_UNCHECKED      0x0000
#define BST_CHECKED        0x0001
#define BST_INDETERMINATE  0x0002
#define BST_PUSHED         0x0004
#define BST_FOCUS          0x0008
#endif /* WINVER >= 0x0400 */

/*
 * Static Control Constants
 */
#define SS_LEFT             0x00000000L
#define SS_CENTER           0x00000001L
#define SS_RIGHT            0x00000002L
#define SS_ICON             0x00000003L
#define SS_BLACKRECT        0x00000004L
#define SS_GRAYRECT         0x00000005L
#define SS_WHITERECT        0x00000006L
#define SS_BLACKFRAME       0x00000007L
#define SS_GRAYFRAME        0x00000008L
#define SS_WHITEFRAME       0x00000009L
#define SS_USERITEM         0x0000000AL
#define SS_SIMPLE           0x0000000BL
#define SS_LEFTNOWORDWRAP   0x0000000CL
#if(WINVER >= 0x0400)
#define SS_OWNERDRAW        0x0000000DL
#define SS_BITMAP           0x0000000EL
#define SS_ENHMETAFILE      0x0000000FL
#define SS_ETCHEDHORZ       0x00000010L
#define SS_ETCHEDVERT       0x00000011L
#define SS_ETCHEDFRAME      0x00000012L
#define SS_TYPEMASK         0x0000001FL
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0501)
#define SS_REALSIZECONTROL  0x00000040L
#endif /* WINVER >= 0x0501 */
#define SS_NOPREFIX         0x00000080L /* Don't do "&" character translation */
#if(WINVER >= 0x0400)
#define SS_NOTIFY           0x00000100L
#define SS_CENTERIMAGE      0x00000200L
#define SS_RIGHTJUST        0x00000400L
#define SS_REALSIZEIMAGE    0x00000800L
#define SS_SUNKEN           0x00001000L
#define SS_ENDELLIPSIS      0x00004000L
#define SS_PATHELLIPSIS     0x00008000L
#define SS_WORDELLIPSIS     0x0000C000L
#define SS_ELLIPSISMASK     0x0000C000L
#endif /* WINVER >= 0x0400 */



#ifndef NOWINMESSAGES
/*
 * Static Control Mesages
 */
#define STM_SETICON         0x0170
#define STM_GETICON         0x0171
#if(WINVER >= 0x0400)
#define STM_SETIMAGE        0x0172
#define STM_GETIMAGE        0x0173
#define STN_CLICKED         0
#define STN_DBLCLK          1
#define STN_ENABLE          2
#define STN_DISABLE         3
#endif /* WINVER >= 0x0400 */
#define STM_MSGMAX          0x0174
#endif /* !NOWINMESSAGES */
#define STM_SETTXTFORMAT  0x0175 //added by C.Xia,2007.4.23. for static text format.

/*
 * Dialog window class
 */
#define WC_DIALOG       (MAKEINTATOM(0x8002))

/*
 * CSGet/CSSetWindowWord/Long offsets for use with WC_DIALOG windows
 */
#define DWL_MSGRESULT   0
#define DWL_DLGPROC     4
#define DWL_USER        8

#ifdef _WIN64

#undef DWL_MSGRESULT
#undef DWL_DLGPROC
#undef DWL_USER

#endif /* _WIN64 */

#define DWLP_MSGRESULT  0
#define DWLP_DLGPROC    DWLP_MSGRESULT + sizeof(LRESULT)
#undef DWLP_USER
#define DWLP_USER       DWLP_DLGPROC + sizeof(CSDLGPROC)

/*
 * Dialog Manager Routines
 */

#ifndef NOMSG

WINUSERAPI
BOOL
WINAPI
CSIsDialogMessage(
    IN HCSWND hDlg,
    IN LPCSMSG lpMsg);


#endif /* !NOMSG */

WINUSERAPI
BOOL
WINAPI
CSMapDialogRect(
    IN HCSWND hDlg,
    IN OUT LPCSRECT lpRect);

WINUSERAPI
int
WINAPI
CSDlgDirList(
    IN HCSWND hDlg,
    IN OUT LPTSTR lpPathSpec,
    IN int nIDListBox,
    IN int nIDStaticPath,
    IN UINT uFileType);

/*
 * DlgDirList, DlgDirListComboBox flags values
 */
#define DDL_READWRITE       0x0000
#define DDL_READONLY        0x0001
#define DDL_HIDDEN          0x0002
#define DDL_SYSTEM          0x0004
#define DDL_DIRECTORY       0x0010
#define DDL_ARCHIVE         0x0020

#define DDL_POSTMSGS        0x2000
#define DDL_DRIVES          0x4000
#define DDL_EXCLUSIVE       0x8000

WINUSERAPI
BOOL
WINAPI
CSDlgDirSelectEx(
    IN HCSWND hDlg,
    OUT LPTSTR lpString,
    IN int nCount,
    IN int nIDListBox);

WINUSERAPI
int
WINAPI
CSDlgDirListComboBox(
    IN HCSWND hDlg,
    IN OUT LPTSTR lpPathSpec,
    IN int nIDComboBox,
    IN int nIDStaticPath,
    IN UINT uFiletype);

WINUSERAPI
BOOL
WINAPI
CSDlgDirSelectComboBoxEx(
    IN HCSWND hDlg,
    OUT LPTSTR lpString,
    IN int nCount,
    IN int nIDComboBox);




/*
 * Dialog Styles
 */
#define DS_ABSALIGN         0x01L
#define DS_SYSMODAL         0x02L
#define DS_LOCALEDIT        0x20L   /* Edit items get Local storage. */
#define DS_SETFONT          0x40L   /* User specified font for Dlg controls */
#define DS_MODALFRAME       0x80L   /* Can be combined with WS_CAPTION  */
#define DS_NOIDLEMSG        0x100L  /* WM_ENTERIDLE message will not be sent */
#define DS_SETFOREGROUND    0x200L  /* not in win3.1 */


#if(WINVER >= 0x0400)
#define DS_3DLOOK           0x0004L
#define DS_FIXEDSYS         0x0008L
#define DS_NOFAILCREATE     0x0010L
#define DS_CONTROL          0x0400L
#define DS_CENTER           0x0800L
#define DS_CENTERMOUSE      0x1000L
#define DS_CONTEXTHELP      0x2000L

#define DS_SHELLFONT        (DS_SETFONT | DS_FIXEDSYS)
#endif /* WINVER >= 0x0400 */


#define DM_GETDEFID         (WM_USER+0)
#define DM_SETDEFID         (WM_USER+1)

#if(WINVER >= 0x0400)
#define DM_REPOSITION       (WM_USER+2)
#endif /* WINVER >= 0x0400 */
/*
 * Returned in HIWORD() of DM_GETDEFID result if msg is supported
 */
#define DC_HASDEFID         0x534B

/*
 * Dialog Codes
 */
#define DLGC_WANTARROWS     0x0001      /* Control wants arrow keys         */
#define DLGC_WANTTAB        0x0002      /* Control wants tab keys           */
#define DLGC_WANTALLKEYS    0x0004      /* Control wants all keys           */
#define DLGC_WANTMESSAGE    0x0004      /* Pass message to control          */
#define DLGC_HASSETSEL      0x0008      /* Understands EM_SETSEL message    */
#define DLGC_DEFPUSHBUTTON  0x0010      /* Default pushbutton               */
#define DLGC_UNDEFPUSHBUTTON 0x0020     /* Non-default pushbutton           */
#define DLGC_RADIOBUTTON    0x0040      /* Radio button                     */
#define DLGC_WANTCHARS      0x0080      /* Want WM_CHAR messages            */
#define DLGC_STATIC         0x0100      /* Static item: don't include       */
#define DLGC_BUTTON         0x2000      /* Button item: can be checked      */

//	Add By hxc 2003.3.14
#define DLGC_WANTLEFTARROWS     0x0200      /* Control wants arrow keys         */
#define DLGC_WANTRIGHTARROWS    0x0400      /* Control wants arrow keys         */
#define DLGC_WANTDOWNARROWS     0x0800      /* Control wants arrow keys         */
#define DLGC_WANTUPARROWS       0x1000      /* Control wants arrow keys         */
#define DLGC_WANTALLARROWS	DLGC_WANTLEFTARROWS|DLGC_WANTRIGHTARROWS|DLGC_WANTDOWNARROWS|DLGC_WANTUPARROWS

//	Add By hxc 2003.3.14

#define LB_CTLCODE          0L

/*
 * Listbox Return Values
 */
#define LB_OKAY             0
#define LB_ERR              (-1)
#define LB_ERRSPACE         (-2)

/*
**  The idStaticPath parameter to CSDlgDirList can have the following values
**  ORed if the list box should show other details of the files along with
**  the name of the files;
*/
                                  /* all other details also will be returned */


/*
 * Listbox Notification Codes
 */
#define LBN_ERRSPACE        (-2)
#define LBN_SELCHANGE       1
#define LBN_DBLCLK          2
#define LBN_SELCANCEL       3
#define LBN_SETFOCUS        4
#define LBN_KILLFOCUS       5



#ifndef NOWINMESSAGES

/*
 * Listbox messages
 */
#define LB_ADDSTRING            0x0180
#define LB_INSERTSTRING         0x0181
#define LB_DELETESTRING         0x0182
#define LB_SELITEMRANGEEX       0x0183
#define LB_RESETCONTENT         0x0184
#define LB_SETSEL               0x0185
#define LB_SETCURSEL            0x0186
#define LB_GETSEL               0x0187
#define LB_GETCURSEL            0x0188
#define LB_GETTEXT              0x0189
#define LB_GETTEXTLEN           0x018A
#define LB_GETCOUNT             0x018B
#define LB_SELECTSTRING         0x018C
#define LB_DIR                  0x018D
#define LB_GETTOPINDEX          0x018E
#define LB_FINDSTRING           0x018F
#define LB_GETSELCOUNT          0x0190
#define LB_GETSELITEMS          0x0191
#define LB_SETTABSTOPS          0x0192
#define LB_GETHORIZONTALEXTENT  0x0193
#define LB_SETHORIZONTALEXTENT  0x0194
#define LB_SETCOLUMNWIDTH       0x0195
#define LB_ADDFILE              0x0196
#define LB_SETTOPINDEX          0x0197
#define LB_GETITEMRECT          0x0198
#define LB_GETITEMDATA          0x0199
#define LB_SETITEMDATA          0x019A
#define LB_SELITEMRANGE         0x019B
#define LB_SETANCHORINDEX       0x019C
#define LB_GETANCHORINDEX       0x019D
#define LB_SETCARETINDEX        0x019E
#define LB_GETCARETINDEX        0x019F
#define LB_SETITEMHEIGHT        0x01A0
#define LB_GETITEMHEIGHT        0x01A1
#define LB_FINDSTRINGEXACT      0x01A2
#define LB_SETLOCALE            0x01A5
#define LB_GETLOCALE            0x01A6
#define LB_SETCOUNT             0x01A7
#if(WINVER >= 0x0400)
#define LB_INITSTORAGE          0x01A8
#define LB_ITEMFROMPOINT        0x01A9
#endif /* WINVER >= 0x0400 */
#if(_WIN32_WCE >= 0x0400)
#define LB_MULTIPLEADDSTRING    0x01B1
#endif


#if(_WIN32_WINNT >= 0x0501)
#define LB_GETLISTBOXINFO       0x01B2
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0501)
#define LB_MSGMAX               0x01B3
#elif(_WIN32_WCE >= 0x0400)
#undef LB_MSGMAX
#define LB_MSGMAX               0x01B1
#elif(WINVER >= 0x0400)
#define LB_MSGMAX               0x01B0
#else
#define LB_MSGMAX               0x01A8
#endif

#endif /* !NOWINMESSAGES */

#ifndef NOWINSTYLES


/*
 * Listbox Styles
 */
#define LBS_NOTIFY            0x0001L
#define LBS_SORT              0x0002L
#define LBS_NOREDRAW          0x0004L
#define LBS_MULTIPLESEL       0x0008L
#define LBS_OWNERDRAWFIXED    0x0010L
#define LBS_OWNERDRAWVARIABLE 0x0020L
#define LBS_HASSTRINGS        0x0040L
#define LBS_USETABSTOPS       0x0080L
#define LBS_NOINTEGRALHEIGHT  0x0100L
#define LBS_MULTICOLUMN       0x0200L
#define LBS_WANTKEYBOARDINPUT 0x0400L
#define LBS_EXTENDEDSEL       0x0800L
#define LBS_DISABLENOSCROLL   0x1000L
#define LBS_NODATA            0x2000L
#if(WINVER >= 0x0400)
#define LBS_NOSEL             0x4000L
#endif /* WINVER >= 0x0400 */
#define LBS_STANDARD          (LBS_NOTIFY | LBS_SORT | WS_VSCROLL | WS_BORDER)


#endif /* !NOWINSTYLES */


/*
 * Combo Box return Values
 */
#define CB_OKAY             0
#define CB_ERR              (-1)
#define CB_ERRSPACE         (-2)


/*
 * Combo Box Notification Codes
 */
#define CBN_ERRSPACE        (-1)
#define CBN_SELCHANGE       1
#define CBN_DBLCLK          2
#define CBN_SETFOCUS        3
#define CBN_KILLFOCUS       4
#define CBN_EDITCHANGE      5
#define CBN_EDITUPDATE      6
#define CBN_DROPDOWN        7
#define CBN_CLOSEUP         8
#define CBN_SELENDOK        9
#define CBN_SELENDCANCEL    10

#ifndef NOWINSTYLES

/*
 * Combo Box styles
 */
#define CBS_SIMPLE            0x0001L
#define CBS_DROPDOWN          0x0002L
#define CBS_DROPDOWNLIST      0x0003L
#define CBS_OWNERDRAWFIXED    0x0010L
#define CBS_OWNERDRAWVARIABLE 0x0020L
#define CBS_AUTOHSCROLL       0x0040L
#define CBS_OEMCONVERT        0x0080L
#define CBS_SORT              0x0100L
#define CBS_HASSTRINGS        0x0200L
#define CBS_NOINTEGRALHEIGHT  0x0400L
#define CBS_DISABLENOSCROLL   0x0800L
#if(WINVER >= 0x0400)
#define CBS_UPPERCASE           0x2000L
#define CBS_LOWERCASE           0x4000L
#endif /* WINVER >= 0x0400 */

#endif  /* !NOWINSTYLES */


/*
 * Combo Box messages
 */
#ifndef NOWINMESSAGES
#define CB_GETEDITSEL               0x0140
#define CB_LIMITTEXT                0x0141
#define CB_SETEDITSEL               0x0142
#define CB_ADDSTRING                0x0143
#define CB_DELETESTRING             0x0144
#define CB_DIR                      0x0145
#define CB_GETCOUNT                 0x0146
#define CB_GETCURSEL                0x0147
#define CB_GETLBTEXT                0x0148
#define CB_GETLBTEXTLEN             0x0149
#define CB_INSERTSTRING             0x014A
#define CB_RESETCONTENT             0x014B
#define CB_FINDSTRING               0x014C
#define CB_SELECTSTRING             0x014D
#define CB_SETCURSEL                0x014E
#define CB_SHOWDROPDOWN             0x014F
#define CB_GETITEMDATA              0x0150
#define CB_SETITEMDATA              0x0151
#define CB_GETDROPPEDCONTROLRECT    0x0152
#define CB_SETITEMHEIGHT            0x0153
#define CB_GETITEMHEIGHT            0x0154
#define CB_SETEXTENDEDUI            0x0155
#define CB_GETEXTENDEDUI            0x0156
#define CB_GETDROPPEDSTATE          0x0157
#define CB_FINDSTRINGEXACT          0x0158
#define CB_SETLOCALE                0x0159
#define CB_GETLOCALE                0x015A
#if(WINVER >= 0x0400)
#define CB_GETTOPINDEX              0x015b
#define CB_SETTOPINDEX              0x015c
#define CB_GETHORIZONTALEXTENT      0x015d
#define CB_SETHORIZONTALEXTENT      0x015e
#define CB_GETDROPPEDWIDTH          0x015f
#define CB_SETDROPPEDWIDTH          0x0160
#define CB_INITSTORAGE              0x0161
#if(_WIN32_WCE >= 0x0400)
#define CB_MULTIPLEADDSTRING        0x0163
#endif
#endif /* WINVER >= 0x0400 */

//#if(_WIN32_WINNT >= 0x0501) //y_junjie comment;
#define CB_GETCOMBOBOXINFO          0x0164
//#endif 

#if(_WIN32_WINNT >= 0x0501)
#define CB_MSGMAX                   0x0165
#elif(_WIN32_WCE >= 0x0400)
#undef CB_MSGMAX
#define CB_MSGMAX                   0x0163
#elif(WINVER >= 0x0400)
#define CB_MSGMAX                   0x0162
#else
#define CB_MSGMAX                   0x015B
#endif
#endif  /* !NOWINMESSAGES */



#ifndef NOWINSTYLES


/*
 * Scroll Bar Styles
 */
#define SBS_HORZ                    0x0000L
#define SBS_VERT                    0x0001L
#define SBS_TOPALIGN                0x0002L
#define SBS_LEFTALIGN               0x0002L
#define SBS_BOTTOMALIGN             0x0004L
#define SBS_RIGHTALIGN              0x0004L
#define SBS_SIZEBOXTOPLEFTALIGN     0x0002L
#define SBS_SIZEBOXBOTTOMRIGHTALIGN 0x0004L
#define SBS_SIZEBOX                 0x0008L
#if(WINVER >= 0x0400)
#define SBS_SIZEGRIP                0x0010L
#endif /* WINVER >= 0x0400 */


#endif /* !NOWINSTYLES */

/*
 * Scroll bar messages
 */
#ifndef NOWINMESSAGES
#define SBM_SETPOS                  0x00E0 /*not in win3.1 */
#define SBM_GETPOS                  0x00E1 /*not in win3.1 */
#define SBM_SETRANGE                0x00E2 /*not in win3.1 */
#define SBM_SETRANGEREDRAW          0x00E6 /*not in win3.1 */
#define SBM_GETRANGE                0x00E3 /*not in win3.1 */
#define SBM_ENABLE_ARROWS           0x00E4 /*not in win3.1 */
#if(WINVER >= 0x0400)
#define SBM_SETSCROLLINFO           0x00E9
#define SBM_GETSCROLLINFO           0x00EA
#endif /* WINVER >= 0x0400 */

#if(_WIN32_WINNT >= 0x0501)
#define SBM_GETSCROLLBARINFO        0x00EB
#endif /* _WIN32_WINNT >= 0x0501 */

#if(WINVER >= 0x0400)
#define SIF_RANGE           0x0001
#define SIF_PAGE            0x0002
#define SIF_POS             0x0004
#define SIF_DISABLENOSCROLL 0x0008
#define SIF_TRACKPOS        0x0010
#define SIF_ALL             (SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS)

typedef struct tagCSSCROLLINFO
{
    UINT    cbSize;
    UINT    fMask;
    int     nMin;
    int     nMax;
    UINT    nPage;
    int     nPos;
    int     nTrackPos;
}   CSSCROLLINFO, FAR *LPCSSCROLLINFO;
typedef CSSCROLLINFO CONST FAR *LPCCSSCROLLINFO;

WINUSERAPI int     WINAPI CSSetScrollInfo(IN HCSWND, IN int, IN LPCCSSCROLLINFO, IN BOOL);
WINUSERAPI BOOL    WINAPI CSGetScrollInfo(IN HCSWND, IN int, IN OUT LPCSSCROLLINFO);

#endif /* WINVER >= 0x0400 */
#endif /* !NOWINMESSAGES */
#endif /* !NOCTLMGR */

#ifndef NOMDI

/*
 * MDI client style bits
 */
#define MDIS_ALLCHILDSTYLES    0x0001

/*
 * wParam Flags for WM_MDITILE and WM_MDICASCADE messages.
 */
#define MDITILE_VERTICAL       0x0000 /*not in win3.1 */
#define MDITILE_HORIZONTAL     0x0001 /*not in win3.1 */
#define MDITILE_SKIPDISABLED   0x0002 /*not in win3.1 */
#if(_WIN32_WINNT >= 0x0500)
#define MDITILE_ZORDER         0x0004
#endif /* _WIN32_WINNT >= 0x0500 */

typedef struct tagCSMDICREATESTRUCTA {
    LPCTSTR   szClass;
    LPCTSTR   szTitle;
    HCSHANDLE hOwner;
    int x;
    int y;
    int cx;
    int cy;
    DWORD style;
    LPARAM lParam;        /* app-defined stuff */
} CSMDICREATESTRUCTA, *LPCSMDICREATESTRUCTA;

typedef CSMDICREATESTRUCTA CSMDICREATESTRUCT;
typedef LPCSMDICREATESTRUCTA LPCSMDICREATESTRUCT;

typedef struct tagCSCLIENTCREATESTRUCT {
    HCSHANDLE hWindowMenu;
    UINT idFirstChild;
} CSCLIENTCREATESTRUCT, *LPCSCLIENTCREATESTRUCT;

WINUSERAPI
LRESULT
WINAPI
CSDefFrameProc(
    IN HCSWND hWnd,
    IN HCSWND hWndMDIClient,
    IN UINT uMsg,
    IN WPARAM wParam,
    IN LPARAM lParam);


WINUSERAPI
#ifndef _MAC
LRESULT
WINAPI
#else
LRESULT
CALLBACK
#endif
CSDefMDIChildProc(
    IN HCSWND hWnd,
    IN UINT uMsg,
    IN WPARAM wParam,
    IN LPARAM lParam);

#ifndef NOMSG

WINUSERAPI
BOOL
WINAPI
CSTranslateMDISysAccel(
    IN HCSWND hWndClient,
    IN LPCSMSG lpMsg);

#endif /* !NOMSG */

WINUSERAPI
UINT
WINAPI
CSArrangeIconicWindows(
    IN HCSWND hWnd);

WINUSERAPI
HCSWND
WINAPI
CSCreateMDIWindow(
    IN LPCTSTR lpClassName,
    IN LPCTSTR lpWindowName,
    IN DWORD dwStyle,
    IN int X,
    IN int Y,
    IN int nWidth,
    IN int nHeight,
    IN HCSWND hWndParent,
    IN HCSINSTANCE hInstance,
    IN LPARAM lParam
    );


#if(WINVER >= 0x0400)
WINUSERAPI WORD    WINAPI CSTileWindows( IN HCSWND hwndParent, IN UINT wHow, IN CONST CSRECT * lpRect, IN UINT cKids, IN const HCSWND FAR * lpKids);
WINUSERAPI WORD    WINAPI CSCascadeWindows( IN HCSWND hwndParent, IN UINT wHow, IN CONST CSRECT * lpRect, IN UINT cKids,  IN const HCSWND FAR * lpKids);
#endif /* WINVER >= 0x0400 */
#endif /* !NOMDI */

#endif /* !NOUSER */

/****** Help support ********************************************************/

#ifndef NOHELP

typedef DWORD HELPPOLY;
typedef struct tagCSMULTIKEYHELPA {
#ifndef _MAC
    DWORD  mkSize;
#else
    WORD   mkSize;
#endif
    CHAR   mkKeylist;
    CHAR   szKeyphrase[1];
} CSMULTIKEYHELPA, *PCSMULTIKEYHELPA, *LPCSMULTIKEYHELPA;

typedef CSMULTIKEYHELPA CSMULTIKEYHELP;
typedef PCSMULTIKEYHELPA PCSMULTIKEYHELP;
typedef LPCSMULTIKEYHELPA LPCSMULTIKEYHELP;

typedef struct tagCSHELPWININFOA {
    int  wStructSize;
    int  x;
    int  y;
    int  dx;
    int  dy;
    int  wMax;
    CHAR   rgchMember[2];
} CSHELPWININFOA, *PCSHELPWININFOA, *LPCSHELPWININFOA;

typedef CSHELPWININFOA CSHELPWININFO;
typedef PCSHELPWININFOA PCSHELPWININFO;
typedef LPCSHELPWININFOA LPCSHELPWININFO;


/*
 * Commands to pass to WinHelp()
 */
#define HELP_CONTEXT      0x0001L  /* Display topic in ulTopic */
#define HELP_QUIT         0x0002L  /* Terminate help */
#define HELP_INDEX        0x0003L  /* Display index */
#define HELP_CONTENTS     0x0003L
#define HELP_HELPONHELP   0x0004L  /* Display help on using help */
#define HELP_SETINDEX     0x0005L  /* CSSet current Index for multi index help */
#define HELP_SETCONTENTS  0x0005L
#define HELP_CONTEXTPOPUP 0x0008L
#define HELP_FORCEFILE    0x0009L
#define HELP_KEY          0x0101L  /* Display topic for keyword in offabData */
#define HELP_COMMAND      0x0102L
#define HELP_PARTIALKEY   0x0105L
#define HELP_MULTIKEY     0x0201L
#define HELP_SETWINPOS    0x0203L
#if(WINVER >= 0x0400)
#define HELP_CONTEXTMENU  0x000a
#define HELP_FINDER       0x000b
#define HELP_WM_HELP      0x000c
#define HELP_SETPOPUP_POS 0x000d

#define HELP_TCARD              0x8000
#define HELP_TCARD_DATA         0x0010
#define HELP_TCARD_OTHER_CALLER 0x0011

// These are in winhelp.h in Win95.
#define IDH_NO_HELP                     28440
#define IDH_MISSING_CONTEXT             28441 // Control doesn't have matching help context
#define IDH_GENERIC_HELP_BUTTON         28442 // Property sheet help button
#define IDH_OK                          28443
#define IDH_CANCEL                      28444
#define IDH_HELP                        28445

#endif /* WINVER >= 0x0400 */



WINUSERAPI
BOOL
WINAPI
CSWinHelp(
    IN HCSWND hWndMain,
    IN LPCTSTR lpszHelp,
    IN UINT uCommand,
    IN ULONG_PTR dwData
    );

#endif /* !NOHELP */

#if(WINVER >= 0x0500)

#define GR_GDIOBJECTS     0       /* Count of GDI objects */
#define GR_USEROBJECTS    1       /* Count of USER objects */

WINUSERAPI
DWORD
WINAPI
CSGetGuiResources(
    IN HCSHANDLE hProcess,
    IN DWORD uiFlags);

#endif /* WINVER >= 0x0500 */


#ifndef NOSYSPARAMSINFO

/*
 * Parameter for SystemParametersInfo()
 */

#define SPI_GETBEEP                 0x0001
#define SPI_SETBEEP                 0x0002
#define SPI_GETMOUSE                0x0003
#define SPI_SETMOUSE                0x0004
#define SPI_GETBORDER               0x0005
#define SPI_SETBORDER               0x0006
#define SPI_GETKEYBOARDSPEED        0x000A
#define SPI_SETKEYBOARDSPEED        0x000B
#define SPI_LANGDRIVER              0x000C
#define SPI_ICONHORIZONTALSPACING   0x000D
#define SPI_GETSCREENSAVETIMEOUT    0x000E
#define SPI_SETSCREENSAVETIMEOUT    0x000F
#define SPI_GETSCREENSAVEACTIVE     0x0010
#define SPI_SETSCREENSAVEACTIVE     0x0011
#define SPI_GETGRIDGRANULARITY      0x0012
#define SPI_SETGRIDGRANULARITY      0x0013
#define SPI_SETDESKWALLPAPER        0x0014
#define SPI_SETDESKPATTERN          0x0015
#define SPI_GETKEYBOARDDELAY        0x0016
#define SPI_SETKEYBOARDDELAY        0x0017
#define SPI_ICONVERTICALSPACING     0x0018
#define SPI_GETICONTITLEWRAP        0x0019
#define SPI_SETICONTITLEWRAP        0x001A
#define SPI_GETMENUDROPALIGNMENT    0x001B
#define SPI_SETMENUDROPALIGNMENT    0x001C
#define SPI_SETDOUBLECLKWIDTH       0x001D
#define SPI_SETDOUBLECLKHEIGHT      0x001E
#define SPI_GETICONTITLELOGFONT     0x001F
#define SPI_SETDOUBLECLICKTIME      0x0020
#define SPI_SETMOUSEBUTTONSWAP      0x0021
#define SPI_SETICONTITLELOGFONT     0x0022
#define SPI_GETFASTTASKSWITCH       0x0023
#define SPI_SETFASTTASKSWITCH       0x0024
#if(WINVER >= 0x0400)
#define SPI_SETDRAGFULLWINDOWS      0x0025
#define SPI_GETDRAGFULLWINDOWS      0x0026
#define SPI_GETNONCLIENTMETRICS     0x0029
#define SPI_SETNONCLIENTMETRICS     0x002A
#define SPI_GETCSMINIMIZEDMETRICS     0x002B
#define SPI_SETCSMINIMIZEDMETRICS     0x002C
#define SPI_GETICONMETRICS          0x002D
#define SPI_SETICONMETRICS          0x002E
#define SPI_SETWORKAREA             0x002F
#define SPI_GETWORKAREA             0x0030
#define SPI_SETPENWINDOWS           0x0031

#define SPI_GETHIGHCONTRAST         0x0042
#define SPI_SETHIGHCONTRAST         0x0043
#define SPI_GETKEYBOARDPREF         0x0044
#define SPI_SETKEYBOARDPREF         0x0045
#define SPI_GETSCREENREADER         0x0046
#define SPI_SETSCREENREADER         0x0047
#define SPI_GETANIMATION            0x0048
#define SPI_SETANIMATION            0x0049
#define SPI_GETFONTSMOOTHING        0x004A
#define SPI_SETFONTSMOOTHING        0x004B
#define SPI_SETDRAGWIDTH            0x004C
#define SPI_SETDRAGHEIGHT           0x004D
#define SPI_SETHANDHELD             0x004E
#define SPI_GETLOWPOWERTIMEOUT      0x004F
#define SPI_GETPOWEROFFTIMEOUT      0x0050
#define SPI_SETLOWPOWERTIMEOUT      0x0051
#define SPI_SETPOWEROFFTIMEOUT      0x0052
#define SPI_GETLOWPOWERACTIVE       0x0053
#define SPI_GETPOWEROFFACTIVE       0x0054
#define SPI_SETLOWPOWERACTIVE       0x0055
#define SPI_SETPOWEROFFACTIVE       0x0056
#define SPI_SETCURSORS              0x0057
#define SPI_SETICONS                0x0058
#define SPI_GETDEFAULTINPUTLANG     0x0059
#define SPI_SETDEFAULTINPUTLANG     0x005A
#define SPI_SETLANGTOGGLE           0x005B
#define SPI_GETWINDOWSEXTENSION     0x005C
#define SPI_SETMOUSETRAILS          0x005D
#define SPI_GETMOUSETRAILS          0x005E
#define SPI_SETSCREENSAVERRUNNING   0x0061
#define SPI_SCREENSAVERRUNNING     SPI_SETSCREENSAVERRUNNING
#endif /* WINVER >= 0x0400 */
#define SPI_GETFILTERKEYS          0x0032
#define SPI_SETFILTERKEYS          0x0033
#define SPI_GETTOGGLEKEYS          0x0034
#define SPI_SETTOGGLEKEYS          0x0035
#define SPI_GETMOUSEKEYS           0x0036
#define SPI_SETMOUSEKEYS           0x0037
#define SPI_GETSHOWSOUNDS          0x0038
#define SPI_SETSHOWSOUNDS          0x0039
#define SPI_GETSTICKYKEYS          0x003A
#define SPI_SETSTICKYKEYS          0x003B
#define SPI_GETACCESSTIMEOUT       0x003C
#define SPI_SETACCESSTIMEOUT       0x003D
#if(WINVER >= 0x0400)
#define SPI_GETSERIALKEYS          0x003E
#define SPI_SETSERIALKEYS          0x003F
#endif /* WINVER >= 0x0400 */
#define SPI_GETSOUNDSENTRY         0x0040
#define SPI_SETSOUNDSENTRY         0x0041
#if(_WIN32_WINNT >= 0x0400)
#define SPI_GETSNAPTODEFBUTTON     0x005F
#define SPI_SETSNAPTODEFBUTTON     0x0060
#endif /* _WIN32_WINNT >= 0x0400 */
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
#define SPI_GETMOUSEHOVERWIDTH     0x0062
#define SPI_SETMOUSEHOVERWIDTH     0x0063
#define SPI_GETMOUSEHOVERHEIGHT    0x0064
#define SPI_SETMOUSEHOVERHEIGHT    0x0065
#define SPI_GETMOUSEHOVERTIME      0x0066
#define SPI_SETMOUSEHOVERTIME      0x0067
#define SPI_GETWHEELSCROLLLINES    0x0068
#define SPI_SETWHEELSCROLLLINES    0x0069
#define SPI_GETMENUSHOWDELAY       0x006A
#define SPI_SETMENUSHOWDELAY       0x006B


#define SPI_GETSHOWIMEUI          0x006E
#define SPI_SETSHOWIMEUI          0x006F
#endif


#if(WINVER >= 0x0500)
#define SPI_GETMOUSESPEED         0x0070
#define SPI_SETMOUSESPEED         0x0071
#define SPI_GETSCREENSAVERRUNNING 0x0072
#define SPI_GETDESKWALLPAPER      0x0073
#endif /* WINVER >= 0x0500 */


#if(WINVER >= 0x0500)
#define SPI_GETACTIVEWINDOWTRACKING         0x1000
#define SPI_SETACTIVEWINDOWTRACKING         0x1001
#define SPI_GETMENUANIMATION                0x1002
#define SPI_SETMENUANIMATION                0x1003
#define SPI_GETCOMBOBOXANIMATION            0x1004
#define SPI_SETCOMBOBOXANIMATION            0x1005
#define SPI_GETLISTBOXSMOOTHSCROLLING       0x1006
#define SPI_SETLISTBOXSMOOTHSCROLLING       0x1007
#define SPI_GETGRADIENTCAPTIONS             0x1008
#define SPI_SETGRADIENTCAPTIONS             0x1009
#define SPI_GETKEYBOARDCUES                 0x100A
#define SPI_SETKEYBOARDCUES                 0x100B
#define SPI_GETMENUUNDERLINES               SPI_GETKEYBOARDCUES
#define SPI_SETMENUUNDERLINES               SPI_SETKEYBOARDCUES
#define SPI_GETACTIVEWNDTRKZORDER           0x100C
#define SPI_SETACTIVEWNDTRKZORDER           0x100D
#define SPI_GETHOTTRACKING                  0x100E
#define SPI_SETHOTTRACKING                  0x100F
#define SPI_GETMENUFADE                     0x1012
#define SPI_SETMENUFADE                     0x1013
#define SPI_GETSELECTIONFADE                0x1014
#define SPI_SETSELECTIONFADE                0x1015
#define SPI_GETTOOLTIPANIMATION             0x1016
#define SPI_SETTOOLTIPANIMATION             0x1017
#define SPI_GETTOOLTIPFADE                  0x1018
#define SPI_SETTOOLTIPFADE                  0x1019
#define SPI_GETCURSORSHADOW                 0x101A
#define SPI_SETCURSORSHADOW                 0x101B
#if(_WIN32_WINNT >= 0x0501)
#define SPI_GETMOUSESONAR                   0x101C
#define SPI_SETMOUSESONAR                   0x101D
#define SPI_GETMOUSECLICKLOCK               0x101E
#define SPI_SETMOUSECLICKLOCK               0x101F
#define SPI_GETMOUSEVANISH                  0x1020
#define SPI_SETMOUSEVANISH                  0x1021
#define SPI_GETFLATMENU                     0x1022
#define SPI_SETFLATMENU                     0x1023
#define SPI_GETDROPSHADOW                   0x1024
#define SPI_SETDROPSHADOW                   0x1025
#endif /* _WIN32_WINNT >= 0x0501 */

#define SPI_GETUIEFFECTS                    0x103E
#define SPI_SETUIEFFECTS                    0x103F

#define SPI_GETFOREGROUNDLOCKTIMEOUT        0x2000
#define SPI_SETFOREGROUNDLOCKTIMEOUT        0x2001
#define SPI_GETACTIVEWNDTRKTIMEOUT          0x2002
#define SPI_SETACTIVEWNDTRKTIMEOUT          0x2003
#define SPI_GETFOREGROUNDFLASHCOUNT         0x2004
#define SPI_SETFOREGROUNDFLASHCOUNT         0x2005
#define SPI_GETCARETWIDTH                   0x2006
#define SPI_SETCARETWIDTH                   0x2007

#if(_WIN32_WINNT >= 0x0501)
#define SPI_GETMOUSECLICKLOCKTIME           0x2008
#define SPI_SETMOUSECLICKLOCKTIME           0x2009
#define SPI_GETFONTSMOOTHINGTYPE            0x200A
#define SPI_SETFONTSMOOTHINGTYPE            0x200B

/* constants for SPI_GETFONTSMOOTHINGTYPE and SPI_SETFONTSMOOTHINGTYPE: */
#define FE_FONTSMOOTHINGSTANDARD            0x0001
#define FE_FONTSMOOTHINGCLEARTYPE           0x0002
#define FE_FONTSMOOTHINGDOCKING             0x8000

#define SPI_GETFONTSMOOTHINGCONTRAST           0x200C
#define SPI_SETFONTSMOOTHINGCONTRAST           0x200D

#define SPI_GETFOCUSBORDERWIDTH             0x200E
#define SPI_SETFOCUSBORDERWIDTH             0x200F
#define SPI_GETFOCUSBORDERHEIGHT            0x2010
#define SPI_SETFOCUSBORDERHEIGHT            0x2011
#endif /* _WIN32_WINNT >= 0x0501 */

#endif /* WINVER >= 0x0500 */

/*
 * Flags
 */
#define SPIF_UPDATEINIFILE    0x0001
#define SPIF_SENDWININICHANGE 0x0002
#define SPIF_SENDCHANGE       SPIF_SENDWININICHANGE


#define METRICS_USEDEFAULT -1
#ifdef _WINGDI_
#ifndef NOGDI
typedef struct tagCSNONCLIENTMETRICSA
{
    UINT    cbSize;
    int     iBorderWidth;
    int     iScrollWidth;
    int     iScrollHeight;
    int     iCaptionWidth;
    int     iCaptionHeight;
    LOGFONTA lfCaptionFont;
    int     iSmCaptionWidth;
    int     iSmCaptionHeight;
    LOGFONTA lfSmCaptionFont;
    int     iMenuWidth;
    int     iMenuHeight;
    LOGFONTA lfMenuFont;
    LOGFONTA lfStatusFont;
    LOGFONTA lfMessageFont;
}   CSNONCLIENTMETRICSA, *PCSNONCLIENTMETRICSA, FAR* LPCSNONCLIENTMETRICSA;

typedef CSNONCLIENTMETRICSA CSNONCLIENTMETRICS;
typedef PCSNONCLIENTMETRICSA PCSNONCLIENTMETRICS;
typedef LPCSNONCLIENTMETRICSA LPCSNONCLIENTMETRICS;
#endif /* NOGDI */
#endif /* _WINGDI_ */

#define ARW_BOTTOMLEFT              0x0000L
#define ARW_BOTTOMRIGHT             0x0001L
#define ARW_TOPLEFT                 0x0002L
#define ARW_TOPRIGHT                0x0003L
#define ARW_STARTMASK               0x0003L
#define ARW_STARTRIGHT              0x0001L
#define ARW_STARTTOP                0x0002L

#define ARW_LEFT                    0x0000L
#define ARW_RIGHT                   0x0000L
#define ARW_UP                      0x0004L
#define ARW_DOWN                    0x0004L
#define ARW_HIDE                    0x0008L

typedef struct tagCSMINIMIZEDMETRICS
{
    UINT    cbSize;
    int     iWidth;
    int     iHorzGap;
    int     iVertGap;
    int     iArrange;
}   CSMINIMIZEDMETRICS, *PCSMINIMIZEDMETRICS, *LPCSMINIMIZEDMETRICS;

#ifdef _WINGDI_
#ifndef NOGDI
typedef struct tagCSICONMETRICSA
{
    UINT    cbSize;
    int     iHorzSpacing;
    int     iVertSpacing;
    int     iTitleWrap;
    LOGFONTA lfFont;
}   CSICONMETRICSA, *PCSICONMETRICSA, *LPCSICONMETRICSA;

typedef CSICONMETRICSA CSICONMETRICS;
typedef PCSICONMETRICSA PCSICONMETRICS;
typedef LPCSICONMETRICSA LPCSICONMETRICS;
#endif /* NOGDI */
#endif /* _WINGDI_ */

typedef struct tagCSANIMATIONINFO
{
    UINT    cbSize;
    int     iMinAnimate;
}   CSANIMATIONINFO, *LPCSANIMATIONINFO;

typedef struct tagCSSERIALKEYSA
{
    UINT    cbSize;
    DWORD   dwFlags;
    LPTSTR     lpszActivePort;
    LPTSTR     lpszPort;
    UINT    iBaudRate;
    UINT    iPortState;
    UINT    iActive;
}   CSSERIALKEYSA, *LPCSSERIALKEYSA;

typedef CSSERIALKEYSA CSSERIALKEYS;
typedef LPCSSERIALKEYSA LPCSSERIALKEYS;

/* flags for SERIALKEYS dwFlags field */
#define SERKF_SERIALKEYSON  0x00000001
#define SERKF_AVAILABLE     0x00000002
#define SERKF_INDICATOR     0x00000004


typedef struct tagCSHIGHCONTRASTA
{
    UINT    cbSize;
    DWORD   dwFlags;
    LPTSTR   lpszDefaultScheme;
}   CSHIGHCONTRASTA, *LPCSHIGHCONTRASTA;

typedef CSHIGHCONTRASTA CSHIGHCONTRAST;
typedef LPCSHIGHCONTRASTA LPCSHIGHCONTRAST;

/* flags for HIGHCONTRAST dwFlags field */
#define HCF_HIGHCONTRASTON  0x00000001
#define HCF_AVAILABLE       0x00000002
#define HCF_HOTKEYACTIVE    0x00000004
#define HCF_CONFIRMHOTKEY   0x00000008
#define HCF_HOTKEYSOUND     0x00000010
#define HCF_INDICATOR       0x00000020
#define HCF_HOTKEYAVAILABLE 0x00000040

/* Flags for CSChangeDisplayCSSettings */
#define CDS_UPDATEREGISTRY  0x00000001
#define CDS_TEST            0x00000002
#define CDS_FULLSCREEN      0x00000004
#define CDS_GLOBAL          0x00000008
#define CDS_SET_PRIMARY     0x00000010
#define CDS_VIDEOPARAMETERS 0x00000020
#define CDS_RESET           0x40000000
#define CDS_NORESET         0x10000000

//#include <tvout.h>

/* Return values for CSChangeDisplayCSSettings */
#define DISP_CHANGE_SUCCESSFUL       0
#define DISP_CHANGE_RESTART          1
#define DISP_CHANGE_FAILED          -1
#define DISP_CHANGE_BADMODE         -2
#define DISP_CHANGE_NOTUPDATED      -3
#define DISP_CHANGE_BADFLAGS        -4
#define DISP_CHANGE_BADPARAM        -5
#if(_WIN32_WINNT >= 0x0501)
#define DISP_CHANGE_BADDUALVIEW     -6
#endif /* _WIN32_WINNT >= 0x0501 */

#ifdef _WINGDI_
#ifndef NOGDI

WINUSERAPI
LONG
WINAPI
CSChangeDisplaySettings(
    IN LPCSDEVMODEA  lpDevMode,
    IN DWORD       dwFlags);

WINUSERAPI
LONG
WINAPI
CSChangeDisplaySettingsEx(
    IN LPCTSTR    lpszDeviceName,
    IN LPCSDEVMODEA  lpDevMode,
    IN HCSWND        hwnd,
    IN DWORD       dwflags,
    IN LPVOID      lParam);


#define ENUM_CURRENT_SETTINGS       ((DWORD)-1)
#define ENUM_REGISTRY_SETTINGS      ((DWORD)-2)

WINUSERAPI
BOOL
WINAPI
CSEnumDisplaySettings(
    IN LPCTSTR lpszDeviceName,
    IN DWORD iModeNum,
    OUT LPCSDEVMODEA lpDevMode);

#if(WINVER >= 0x0500)

WINUSERAPI
BOOL
WINAPI
CSEnumDisplaySettingsEx(
    IN LPCTSTR lpszDeviceName,
    IN DWORD iModeNum,
    OUT LPCSDEVMODEA lpDevMode,
    IN DWORD dwFlags);


/* Flags for CSEnumDisplaySettingsEx */
#define EDS_RAWMODE                   0x00000002

WINUSERAPI
BOOL
WINAPI
CSEnumDisplayDevices(
    IN LPCTSTR lpDevice,
    IN DWORD iDevNum,
    OUT PDISPLAY_DEVICEA lpDisplayDevice,
    IN DWORD dwFlags);

#endif /* WINVER >= 0x0500 */

#endif /* NOGDI */
#endif /* _WINGDI_ */


WINUSERAPI
BOOL
WINAPI
CSSystemParametersInfo(
    IN UINT uiAction,
    IN UINT uiParam,
    IN OUT PVOID pvParam,
    IN UINT fWinIni);

#endif  /* !NOSYSPARAMSINFO  */

/*
 * Accessibility support
 */
typedef struct tagCSFILTERKEYS
{
    UINT  cbSize;
    DWORD dwFlags;
    DWORD iWaitMSec;            // Acceptance Delay
    DWORD iDelayMSec;           // Delay Until Repeat
    DWORD iRepeatMSec;          // Repeat Rate
    DWORD iBounceMSec;          // Debounce Time
} CSFILTERKEYS, *LPCSFILTERKEYS;

/*
 * CSFILTERKEYS dwFlags field
 */
#define FKF_FILTERKEYSON    0x00000001
#define FKF_AVAILABLE       0x00000002
#define FKF_HOTKEYACTIVE    0x00000004
#define FKF_CONFIRMHOTKEY   0x00000008
#define FKF_HOTKEYSOUND     0x00000010
#define FKF_INDICATOR       0x00000020
#define FKF_CLICKON         0x00000040

typedef struct tagCSSTICKYKEYS
{
    UINT  cbSize;
    DWORD dwFlags;
} CSSTICKYKEYS, *LPCSSTICKYKEYS;

/*
 * CSSTICKYKEYS dwFlags field
 */
#define SKF_STICKYKEYSON    0x00000001
#define SKF_AVAILABLE       0x00000002
#define SKF_HOTKEYACTIVE    0x00000004
#define SKF_CONFIRMHOTKEY   0x00000008
#define SKF_HOTKEYSOUND     0x00000010
#define SKF_INDICATOR       0x00000020
#define SKF_AUDIBLEFEEDBACK 0x00000040
#define SKF_TRISTATE        0x00000080
#define SKF_TWOKEYSOFF      0x00000100
#if(_WIN32_WINNT >= 0x0500)
#define SKF_LALTLATCHED       0x10000000
#define SKF_LCTLLATCHED       0x04000000
#define SKF_LSHIFTLATCHED     0x01000000
#define SKF_RALTLATCHED       0x20000000
#define SKF_RCTLLATCHED       0x08000000
#define SKF_RSHIFTLATCHED     0x02000000
#define SKF_LWINLATCHED       0x40000000
#define SKF_RWINLATCHED       0x80000000
#define SKF_LALTLOCKED        0x00100000
#define SKF_LCTLLOCKED        0x00040000
#define SKF_LSHIFTLOCKED      0x00010000
#define SKF_RALTLOCKED        0x00200000
#define SKF_RCTLLOCKED        0x00080000
#define SKF_RSHIFTLOCKED      0x00020000
#define SKF_LWINLOCKED        0x00400000
#define SKF_RWINLOCKED        0x00800000
#endif /* _WIN32_WINNT >= 0x0500 */

typedef struct tagCSMOUSEKEYS
{
    UINT cbSize;
    DWORD dwFlags;
    DWORD iMaxSpeed;
    DWORD iTimeToMaxSpeed;
    DWORD iCtrlSpeed;
    DWORD dwReserved1;
    DWORD dwReserved2;
} CSMOUSEKEYS, *LPCSMOUSEKEYS;

/*
 * CSMOUSEKEYS dwFlags field
 */
#define MKF_MOUSEKEYSON     0x00000001
#define MKF_AVAILABLE       0x00000002
#define MKF_HOTKEYACTIVE    0x00000004
#define MKF_CONFIRMHOTKEY   0x00000008
#define MKF_HOTKEYSOUND     0x00000010
#define MKF_INDICATOR       0x00000020
#define MKF_MODIFIERS       0x00000040
#define MKF_REPLACENUMBERS  0x00000080
#if(_WIN32_WINNT >= 0x0500)
#define MKF_LEFTBUTTONSEL   0x10000000
#define MKF_RIGHTBUTTONSEL  0x20000000
#define MKF_LEFTBUTTONDOWN  0x01000000
#define MKF_RIGHTBUTTONDOWN 0x02000000
#define MKF_MOUSEMODE       0x80000000
#endif /* _WIN32_WINNT >= 0x0500 */

typedef struct tagCSACCESSTIMEOUT
{
    UINT  cbSize;
    DWORD dwFlags;
    DWORD iTimeOutMSec;
} CSACCESSTIMEOUT, *LPCSACCESSTIMEOUT;

/*
 * CSACCESSTIMEOUT dwFlags field
 */
#define ATF_TIMEOUTON       0x00000001
#define ATF_ONOFFFEEDBACK   0x00000002

/* values for SOUNDSENTRY iFSGrafEffect field */
#define SSGF_NONE       0
#define SSGF_DISPLAY    3

/* values for SOUNDSENTRY iFSTextEffect field */
#define SSTF_NONE       0
#define SSTF_CHARS      1
#define SSTF_BORDER     2
#define SSTF_DISPLAY    3

/* values for SOUNDSENTRY iWindowsEffect field */
#define SSWF_NONE     0
#define SSWF_TITLE    1
#define SSWF_WINDOW   2
#define SSWF_DISPLAY  3
#define SSWF_CUSTOM   4

typedef struct tagCSSOUNDSENTRYA
{
    UINT cbSize;
    DWORD dwFlags;
    DWORD iFSTextEffect;
    DWORD iFSTextEffectMSec;
    DWORD iFSTextEffectColorBits;
    DWORD iFSGrafEffect;
    DWORD iFSGrafEffectMSec;
    DWORD iFSGrafEffectColor;
    DWORD iWindowsEffect;
    DWORD iWindowsEffectMSec;
    LPTSTR   lpszWindowsEffectDLL;
    DWORD iWindowsEffectOrdinal;
} CSSOUNDSENTRYA, *LPCSSOUNDSENTRYA;

typedef CSSOUNDSENTRYA CSSOUNDSENTRY;
typedef LPCSSOUNDSENTRYA LPCSSOUNDSENTRY;

/*
 * SOUNDSENTRY dwFlags field
 */
#define SSF_SOUNDSENTRYON   0x00000001
#define SSF_AVAILABLE       0x00000002
#define SSF_INDICATOR       0x00000004

typedef struct tagCSTOGGLEKEYS
{
    UINT cbSize;
    DWORD dwFlags;
} CSTOGGLEKEYS, *LPCSTOGGLEKEYS;

/*
 * CSTOGGLEKEYS dwFlags field
 */
#define TKF_TOGGLEKEYSON    0x00000001
#define TKF_AVAILABLE       0x00000002
#define TKF_HOTKEYACTIVE    0x00000004
#define TKF_CONFIRMHOTKEY   0x00000008
#define TKF_HOTKEYSOUND     0x00000010
#define TKF_INDICATOR       0x00000020

/*
 * Set debug level
 */

WINUSERAPI
void
WINAPI
CSSetDebugErrorLevel(
    IN DWORD dwLevel
    );

/*
 * CSSetLastErrorEx() types.
 */

#define SLE_ERROR       0x00000001
#define SLE_MINORERROR  0x00000002
#define SLE_WARNING     0x00000003

WINUSERAPI
void
WINAPI
CSSetLastErrorEx(
    IN DWORD dwErrCode,
    IN DWORD dwType
    );


#if(WINVER >= 0x0500)

/*
 * Multimonitor API.
 */

#define MONITOR_DEFAULTTONULL       0x00000000
#define MONITOR_DEFAULTTOPRIMARY    0x00000001
#define MONITOR_DEFAULTTONEAREST    0x00000002

WINUSERAPI
HCSMONITOR
WINAPI
CSMonitorFromPoint(
    IN CSPOINT pt,
    IN DWORD dwFlags);

WINUSERAPI
HCSMONITOR
WINAPI
CSMonitorFromRect(
    IN LPCSRECT lprc,
    IN DWORD dwFlags);

WINUSERAPI
HCSMONITOR
WINAPI
CSMonitorFromWindow( IN HCSWND hwnd, IN DWORD dwFlags);

#define MONITORINFOF_PRIMARY        0x00000001

#ifndef CCHDEVICENAME
#define CCHDEVICENAME 32
#endif

typedef struct tagCSMONITORINFO
{
    DWORD   cbSize;
    CSRECT    rcMonitor;
    CSRECT    rcWork;
    DWORD   dwFlags;
} CSMONITORINFO, *LPCSMONITORINFO;

#ifdef __cplusplus
typedef struct tagCSMONITORINFOEXA : public tagCSMONITORINFO
{
    CHAR        szDevice[CCHDEVICENAME];
} CSMONITORINFOEXA, *LPCSMONITORINFOEXA;

typedef CSMONITORINFOEXA CSMONITORINFOEX;
typedef LPCSMONITORINFOEXA LPCSMONITORINFOEX;
#else // ndef __cplusplus
typedef struct tagCSMONITORINFOEXA
{
    CSMONITORINFO u;
    CHAR        szDevice[CCHDEVICENAME];
} CSMONITORINFOEXA, *LPCSMONITORINFOEXA;

typedef CSMONITORINFOEXA CSMONITORINFOEX;
typedef LPCSMONITORINFOEXA LPCSMONITORINFOEX;
#endif

WINUSERAPI BOOL WINAPI CSGetMonitorInfo( IN HCSMONITOR hMonitor, OUT LPCSMONITORINFO lpmi);

typedef BOOL (CALLBACK* CSMONITORENUMPROC)(HCSMONITOR, HCSDC, LPCSRECT, LPARAM);

WINUSERAPI
BOOL
WINAPI
CSEnumDisplayMonitors(
    IN HCSDC             hdc,
    IN LPCSRECT         lprcClip,
    IN CSMONITORENUMPROC lpfnEnum,
    IN LPARAM          dwData);


#ifndef NOWINABLE

/*
 * WinEvents - Active Accessibility hooks
 */

WINUSERAPI
void
WINAPI
CSNotifyWinEvent(
    IN DWORD event,
    IN HCSWND  hwnd,
    IN LONG  idObject,
    IN LONG  idChild);

typedef void (CALLBACK* CSWINEVENTPROC)(
    HCSWINEVENTHOOK hWinEventHook,
    DWORD         event,
    HCSWND          hwnd,
    LONG          idObject,
    LONG          idChild,
    DWORD         idEventThread,
    DWORD         dwmsEventTime);

WINUSERAPI
HCSWINEVENTHOOK
WINAPI
CSSetWinEventHook(
    IN DWORD        eventMin,
    IN DWORD        eventMax,
    IN HCSMODULE      hmodWinEventProc,
    IN CSWINEVENTPROC pfnWinEventProc,
    IN DWORD        idProcess,
    IN DWORD        idThread,
    IN DWORD        dwFlags);

#if(_WIN32_WINNT >= 0x0501)
WINUSERAPI
BOOL
WINAPI
CSIsWinEventHookInstalled(
    IN DWORD event);
#endif /* _WIN32_WINNT >= 0x0501 */

/*
 * dwFlags for CSSetWinEventHook
 */
#define WINEVENT_OUTOFCONTEXT   0x0000  // Events are ASYNC
#define WINEVENT_SKIPOWNTHREAD  0x0001  // Don't call back for events on installer's thread
#define WINEVENT_SKIPOWNPROCESS 0x0002  // Don't call back for events on installer's process
#define WINEVENT_INCONTEXT      0x0004  // Events are SYNC, this causes your dll to be injected into every process

WINUSERAPI
BOOL
WINAPI
CSUnhookWinEvent(
    IN HCSWINEVENTHOOK hWinEventHook);

/*
 * idObject values for WinEventProc and NotifyWinEvent
 */

/*
 * hwnd + idObject can be used with OLEACC.DLL's OleCSGetObjectFromWindow()
 * to get an interface pointer to the container.  indexChild is the item
 * within the container in question.  CSSetup a VARIANT with vt VT_I4 and
 * lVal the indexChild and pass that in to all methods.  Then you
 * are raring to go.
 */


/*
 * Common object IDs (cookies, only for sending WM_GETOBJECT to get at the
 * thing in question).  Positive IDs are reserved for apps (app specific),
 * negative IDs are system things and are global, 0 means "just little old
 * me".
 */
#define     CHILDID_SELF        0
#define     INDEXID_OBJECT      0
#define     INDEXID_CONTAINER   0

/*
 * Reserved IDs for system objects
 */
#define     OBJID_WINDOW        ((LONG)0x00000000)
#define     OBJID_SYSMENU       ((LONG)0xFFFFFFFF)
#define     OBJID_TITLEBAR      ((LONG)0xFFFFFFFE)
#define     OBJID_MENU          ((LONG)0xFFFFFFFD)
#define     OBJID_CLIENT        ((LONG)0xFFFFFFFC)
#define     OBJID_VSCROLL       ((LONG)0xFFFFFFFB)
#define     OBJID_HSCROLL       ((LONG)0xFFFFFFFA)
#define     OBJID_SIZEGRIP      ((LONG)0xFFFFFFF9)
#define     OBJID_CARET         ((LONG)0xFFFFFFF8)
#define     OBJID_CURSOR        ((LONG)0xFFFFFFF7)
#define     OBJID_ALERT         ((LONG)0xFFFFFFF6)
#define     OBJID_SOUND         ((LONG)0xFFFFFFF5)
#define     OBJID_QUERYCLASSNAMEIDX ((LONG)0xFFFFFFF4)
#define     OBJID_NATIVEOM      ((LONG)0xFFFFFFF0)

/*
 * EVENT DEFINITION
 */
#define EVENT_MIN           0x00000001
#define EVENT_MAX           0x7FFFFFFF


/*
 *  EVENT_SYSTEM_SOUND
 *  Sent when a sound is played.  Currently nothing is generating this, we
 *  this event when a system sound (for menus, etc) is played.  Apps
 *  generate this, if accessible, when a private sound is played.  For
 *  example, if Mail plays a "New Mail" sound.
 *
 *  CSSystem Sounds:
 *  (Generated by PlaySoundEvent in USER itself)
 *      hwnd            is NULL
 *      idObject        is OBJID_SOUND
 *      idChild         is sound child ID if one
 *  App Sounds:
 *  (PlaySoundEvent won't generate notification; up to app)
 *      hwnd + idObject gets interface pointer to Sound object
 *      idChild identifies the sound in question
 *  are going to be cleaning up the SOUNDSENTRY feature in the control panel
 *  and will use this at that time.  Applications implementing WinEvents
 *  are perfectly welcome to use it.  Clients of IAccessible* will simply
 *  turn around and get back a non-visual object that describes the sound.
 */
#define EVENT_SYSTEM_SOUND              0x0001

/*
 * EVENT_SYSTEM_ALERT
 * CSSystem Alerts:
 * (Generated by MessageBox() calls for example)
 *      hwnd            is hwndMessageBox
 *      idObject        is OBJID_ALERT
 * App Alerts:
 * (Generated whenever)
 *      hwnd+idObject gets interface pointer to Alert
 */
#define EVENT_SYSTEM_ALERT              0x0002

/*
 * EVENT_SYSTEM_FOREGROUND
 * Sent when the foreground (active) window changes, even if it is changing
 * to another window in the same thread as the previous one.
 *      hwnd            is hwndNewForeground
 *      idObject        is OBJID_WINDOW
 *      idChild    is INDEXID_OBJECT
 */
#define EVENT_SYSTEM_FOREGROUND         0x0003

/*
 * Menu
 *      hwnd            is window (top level window or popup menu window)
 *      idObject        is ID of control (OBJID_MENU, OBJID_SYSMENU, OBJID_SELF for popup)
 *      idChild         is CHILDID_SELF
 *
 * EVENT_SYSTEM_MENUSTART
 * EVENT_SYSTEM_MENUEND
 * For MENUSTART, hwnd+idObject+idChild refers to the control with the menu bar,
 *  or the control bringing up the context menu.
 *
 * Sent when entering into and leaving from menu mode (system, app bar, and
 * track popups).
 */
#define EVENT_SYSTEM_MENUSTART          0x0004
#define EVENT_SYSTEM_MENUEND            0x0005

/*
 * EVENT_SYSTEM_MENUPOPUPSTART
 * EVENT_SYSTEM_MENUPOPUPEND
 * Sent when a menu popup comes up and just before it is taken down.  Note
 * that for a call to TrackPopupMenu(), a client will see EVENT_SYSTEM_MENUSTART
 * followed almost immediately by EVENT_SYSTEM_MENUPOPUPSTART for the popup
 * being shown.
 *
 * For MENUPOPUP, hwnd+idObject+idChild refers to the NEW popup coming up, not the
 * parent item which is hierarchical.  You can get the parent menu/popup by
 * asking for the accParent object.
 */
#define EVENT_SYSTEM_MENUPOPUPSTART     0x0006
#define EVENT_SYSTEM_MENUPOPUPEND       0x0007


/*
 * EVENT_SYSTEM_CAPTURESTART
 * EVENT_SYSTEM_CAPTUREEND
 * Sent when a window takes the capture and releases the capture.
 */
#define EVENT_SYSTEM_CAPTURESTART       0x0008
#define EVENT_SYSTEM_CAPTUREEND         0x0009

/*
 * CSMove Size
 * EVENT_SYSTEM_MOVESIZESTART
 * EVENT_SYSTEM_MOVESIZEEND
 * Sent when a window enters and leaves move-size dragging mode.
 */
#define EVENT_SYSTEM_MOVESIZESTART      0x000A
#define EVENT_SYSTEM_MOVESIZEEND        0x000B

/*
 * Context Help
 * EVENT_SYSTEM_CONTEXTHELPSTART
 * EVENT_SYSTEM_CONTEXTHELPEND
 * Sent when a window enters and leaves context sensitive help mode.
 */
#define EVENT_SYSTEM_CONTEXTHELPSTART   0x000C
#define EVENT_SYSTEM_CONTEXTHELPEND     0x000D

/*
 * CSDrag & Drop
 * EVENT_SYSTEM_DRAGDROPSTART
 * EVENT_SYSTEM_DRAGDROPEND
 * CSSend the START notification just before going into drag&drop loop.  CSSend
 * the END notification just after canceling out.
 * Note that it is up to apps and OLE to generate this, since the system
 * doesn't know.  Like EVENT_SYSTEM_SOUND, it will be a while before this
 * is prevalent.
 */
#define EVENT_SYSTEM_DRAGDROPSTART      0x000E
#define EVENT_SYSTEM_DRAGDROPEND        0x000F

/*
 * Dialog
 * CSSend the START notification right after the dialog is completely
 *  initialized and visible.  CSSend the END right before the dialog
 *  is hidden and goes away.
 * EVENT_SYSTEM_DIALOGSTART
 * EVENT_SYSTEM_DIALOGEND
 */
#define EVENT_SYSTEM_DIALOGSTART        0x0010
#define EVENT_SYSTEM_DIALOGEND          0x0011

/*
 * EVENT_SYSTEM_SCROLLING
 * EVENT_SYSTEM_SCROLLINGSTART
 * EVENT_SYSTEM_SCROLLINGEND
 * Sent when beginning and ending the tracking of a scrollbar in a window,
 * and also for scrollbar controls.
 */
#define EVENT_SYSTEM_SCROLLINGSTART     0x0012
#define EVENT_SYSTEM_SCROLLINGEND       0x0013

/*
 * Alt-Tab Window
 * CSSend the START notification right after the switch window is initialized
 * and visible.  CSSend the END right before it is hidden and goes away.
 * EVENT_SYSTEM_SWITCHSTART
 * EVENT_SYSTEM_SWITCHEND
 */
#define EVENT_SYSTEM_SWITCHSTART        0x0014
#define EVENT_SYSTEM_SWITCHEND          0x0015

/*
 * EVENT_SYSTEM_MINIMIZESTART
 * EVENT_SYSTEM_MINIMIZEEND
 * Sent when a window minimizes and just before it restores.
 */
#define EVENT_SYSTEM_MINIMIZESTART      0x0016
#define EVENT_SYSTEM_MINIMIZEEND        0x0017


#if(_WIN32_WINNT >= 0x0501)
#define EVENT_CONSOLE_CARET             0x4001
#define EVENT_CONSOLE_UPDATE_REGION     0x4002
#define EVENT_CONSOLE_UPDATE_SIMPLE     0x4003
#define EVENT_CONSOLE_UPDATE_SCROLL     0x4004
#define EVENT_CONSOLE_LAYOUT            0x4005
#define EVENT_CONSOLE_START_APPLICATION 0x4006
#define EVENT_CONSOLE_END_APPLICATION   0x4007

/*
 * Flags for EVENT_CONSOLE_START/END_APPLICATION.
 */
#define CONSOLE_APPLICATION_16BIT       0x0001

/*
 * Flags for EVENT_CONSOLE_CARET
 */
#define CONSOLE_CARET_SELECTION         0x0001
#define CONSOLE_CARET_VISIBLE           0x0002
#endif /* _WIN32_WINNT >= 0x0501 */

/*
 * Object events
 *
 * The system AND apps generate these.  The system generates these for
 * real windows.  Apps generate these for objects within their window which
 * act like a separate control, e.g. an item in a list view.
 *
 * When the system generate them, dwParam2 is always WMOBJID_SELF.  When
 * apps generate them, apps put the has-meaning-to-the-app-only ID value
 * in dwParam2.
 * For all events, if you want detailed accessibility information, callers
 * should
 *      * CSCall AccessibleObjectFromWindow() with the hwnd, idObject parameters
 *          of the event, and IID_IAccessible as the REFIID, to get back an
 *          IAccessible* to talk to
 *      * Initialize and fill in a VARIANT as VT_I4 with lVal the idChild
 *          parameter of the event.
 *      * If idChild isn't zero, call get_accChild() in the container to see
 *          if the child is an object in its own right.  If so, you will get
 *          back an ICSDispatch* object for the child.  You should release the
 *          parent, and call QueryInterface() on the child object to get its
 *          IAccessible*.  Then you talk directly to the child.  Otherwise,
 *          if get_accChild() returns you nothing, you should continue to
 *          use the child VARIANT.  You will ask the container for the properties
 *          of the child identified by the VARIANT.  In other words, the
 *          child in this case is accessible but not a full-blown object.
 *          Like a button on a titlebar which is 'small' and has no children.
 */

/*
 * For all EVENT_OBJECT events,
 *      hwnd is the dude to CSSend the WM_GETOBJECT message to (unless NULL,
 *          see above for system things)
 *      idObject is the ID of the object that can resolve any queries a
 *          client might have.  It's a way to deal with windowless controls,
 *          controls that are just drawn on the screen in some larger parent
 *          window (like SDM), or standard frame elements of a window.
 *      idChild is the piece inside of the object that is affected.  This
 *          allows clients to access things that are too small to have full
 *          blown objects in their own right.  Like the thumb of a scrollbar.
 *          The hwnd/idObject pair gets you to the container, the dude you
 *          probably want to talk to most of the time anyway.  The idChild
 *          can then be passed into the acc properties to get the name/value
 *          of it as needed.
 *
 * Example #1:
 *      CSSystem propagating a listbox selection change
 *      EVENT_OBJECT_SELECTION
 *          hwnd == listbox hwnd
 *          idObject == OBJID_WINDOW
 *          idChild == new selected item, or CHILDID_SELF if
 *              nothing now selected within container.
 *      Word '97 propagating a listbox selection change
 *          hwnd == SDM window
 *          idObject == SDM ID to get at listbox 'control'
 *          idChild == new selected item, or CHILDID_SELF if
 *              nothing
 *
 * Example #2:
 *      CSSystem propagating a menu item selection on the menu bar
 *      EVENT_OBJECT_SELECTION
 *          hwnd == top level window
 *          idObject == OBJID_MENU
 *          idChild == ID of child menu bar item selected
 *
 * Example #3:
 *      System propagating a dropdown coming off of said menu bar item
 *      EVENT_OBJECT_CREATE
 *          hwnd == popup item
 *          idObject == OBJID_WINDOW
 *          idChild == CHILDID_SELF
 *
 * Example #4:
 *
 * For EVENT_OBJECT_REORDER, the object referred to by hwnd/idObject is the
 * PARENT container in which the zorder is occurring.  This is because if
 * one child is zordering, all of them are changing their relative zorder.
 */
#define EVENT_OBJECT_CREATE                 0x8000  // hwnd + ID + idChild is created item
#define EVENT_OBJECT_DESTROY                0x8001  // hwnd + ID + idChild is destroyed item
#define EVENT_OBJECT_SHOW                   0x8002  // hwnd + ID + idChild is shown item
#define EVENT_OBJECT_HIDE                   0x8003  // hwnd + ID + idChild is hidden item
#define EVENT_OBJECT_REORDER                0x8004  // hwnd + ID + idChild is parent of zordering children
/*
 * NOTE:
 * Minimize the number of notifications!
 *
 * When you are hiding a parent object, obviously all child objects are no
 * longer visible on screen.  They still have the same "visible" status,
 * but are not truly visible.  Hence do not send HIDE notifications for the
 * children also.  One implies all.  The same goes for SHOW.
 */


#define EVENT_OBJECT_FOCUS                  0x8005  // hwnd + ID + idChild is focused item
#define EVENT_OBJECT_SELECTION              0x8006  // hwnd + ID + idChild is selected item (if only one), or idChild is OBJID_WINDOW if complex
#define EVENT_OBJECT_SELECTIONADD           0x8007  // hwnd + ID + idChild is item added
#define EVENT_OBJECT_SELECTIONREMOVE        0x8008  // hwnd + ID + idChild is item removed
#define EVENT_OBJECT_SELECTIONWITHIN        0x8009  // hwnd + ID + idChild is parent of changed selected items

/*
 * NOTES:
 * There is only one "focused" child item in a parent.  This is the place
 * keystrokes are going at a given moment.  Hence only send a notification
 * about where the NEW focus is going.  A NEW item getting the focus already
 * implies that the OLD item is losing it.
 *
 * SELECTION however can be multiple.  Hence the different SELECTION
 * notifications.  Here's when to use each:
 *
 * (1) CSSend a SELECTION notification in the simple single selection
 *     case (like the focus) when the item with the selection is
 *     merely moving to a different item within a container.  hwnd + ID
 *     is the container control, idChildItem is the new child with the
 *     selection.
 *
 * (2) CSSend a SELECTIONADD notification when a new item has simply been added
 *     to the selection within a container.  This is appropriate when the
 *     number of newly selected items is very small.  hwnd + ID is the
 *     container control, idChildItem is the new child added to the selection.
 *
 * (3) CSSend a SELECTIONREMOVE notification when a new item has simply been
 *     removed from the selection within a container.  This is appropriate
 *     when the number of newly selected items is very small, just like
 *     SELECTIONADD.  hwnd + ID is the container control, idChildItem is the
 *     new child removed from the selection.
 *
 * (4) CSSend a SELECTIONWITHIN notification when the selected items within a
 *     control have changed substantially.  Rather than propagate a large
 *     number of changes to reflect removal for some items, addition of
 *     others, just tell somebody who cares that a lot happened.  It will
 *     be faster an easier for somebody watching to just turn around and
 *     query the container control what the new bunch of selected items
 *     are.
 */

#define EVENT_OBJECT_STATECHANGE            0x800A  // hwnd + ID + idChild is item w/ state change
/*
 * Examples of when to send an EVENT_OBJECT_STATECHANGE include
 *      * It is being enabled/disabled (USER does for windows)
 *      * It is being pressed/released (USER does for buttons)
 *      * It is being checked/unchecked (USER does for radio/check buttons)
 */
#define EVENT_OBJECT_LOCATIONCHANGE         0x800B  // hwnd + ID + idChild is moved/sized item

/*
 * Note:
 * A LOCATIONCHANGE is not sent for every child object when the parent
 * changes shape/moves.  CSSend one notification for the topmost object
 * that is changing.  For example, if the user resizes a top level window,
 * USER will generate a LOCATIONCHANGE for it, but not for the menu bar,
 * title bar, scrollbars, etc.  that are also changing shape/moving.
 *
 * In other words, it only generates LOCATIONCHANGE notifications for
 * real windows that are moving/sizing.  It will not generate a LOCATIONCHANGE
 * for every non-floating child window when the parent moves (the children are
 * logically moving also on screen, but not relative to the parent).
 *
 * Now, if the app itself resizes child windows as a result of being
 * sized, USER will generate LOCATIONCHANGEs for those dudes also because
 * it doesn't know better.
 *
 * Note also that USER will generate LOCATIONCHANGE notifications for two
 * non-window sys objects:
 *      (1) System caret
 *      (2) Cursor
 */

#define EVENT_OBJECT_NAMECHANGE             0x800C  // hwnd + ID + idChild is item w/ name change
#define EVENT_OBJECT_DESCRIPTIONCHANGE      0x800D  // hwnd + ID + idChild is item w/ desc change
#define EVENT_OBJECT_VALUECHANGE            0x800E  // hwnd + ID + idChild is item w/ value change
#define EVENT_OBJECT_PARENTCHANGE           0x800F  // hwnd + ID + idChild is item w/ new parent
#define EVENT_OBJECT_HELPCHANGE             0x8010  // hwnd + ID + idChild is item w/ help change
#define EVENT_OBJECT_DEFACTIONCHANGE        0x8011  // hwnd + ID + idChild is item w/ def action change
#define EVENT_OBJECT_ACCELERATORCHANGE      0x8012  // hwnd + ID + idChild is item w/ keybd accel change

/*
 * Child IDs
 */

/*
 * System Sounds (idChild of system SOUND notification)
 */
#define SOUND_SYSTEM_STARTUP            1
#define SOUND_SYSTEM_SHUTDOWN           2
#define SOUND_SYSTEM_BEEP               3
#define SOUND_SYSTEM_ERROR              4
#define SOUND_SYSTEM_QUESTION           5
#define SOUND_SYSTEM_WARNING            6
#define SOUND_SYSTEM_INFORMATION        7
#define SOUND_SYSTEM_MAXIMIZE           8
#define SOUND_SYSTEM_MINIMIZE           9
#define SOUND_SYSTEM_RESTOREUP          10
#define SOUND_SYSTEM_RESTOREDOWN        11
#define SOUND_SYSTEM_APPSTART           12
#define SOUND_SYSTEM_FAULT              13
#define SOUND_SYSTEM_APPEND             14
#define SOUND_SYSTEM_MENUCOMMAND        15
#define SOUND_SYSTEM_MENUPOPUP          16
#define CSOUND_SYSTEM                   16

/*
 * System Alerts (indexChild of system ALERT notification)
 */
#define ALERT_SYSTEM_INFORMATIONAL      1       // MB_INFORMATION
#define ALERT_SYSTEM_WARNING            2       // MB_WARNING
#define ALERT_SYSTEM_ERROR              3       // MB_ERROR
#define ALERT_SYSTEM_QUERY              4       // MB_QUESTION
#define ALERT_SYSTEM_CRITICAL           5       // HardSysErrBox
#define CALERT_SYSTEM                   6

typedef struct tagCSGUITHREADINFO
{
    DWORD   cbSize;
    DWORD   flags;
    HCSWND    hwndActive;
    HCSWND    hwndFocus;
    HCSWND    hwndCapture;
    HCSWND    hwndMenuOwner;
    HCSWND    hwndMoveSize;
    HCSWND    hwndCaret;
    CSRECT    rcCaret;
} CSGUITHREADINFO, *PCSGUITHREADINFO, FAR * LPCSGUITHREADINFO;

#define GUI_CARETBLINKING   0x00000001
#define GUI_INMOVESIZE      0x00000002
#define GUI_INMENUMODE      0x00000004
#define GUI_SYSTEMMENUMODE  0x00000008
#define GUI_POPUPMENUMODE   0x00000010
#if(_WIN32_WINNT >= 0x0501)
#define GUI_16BITTASK       0x00000020
#endif /* _WIN32_WINNT >= 0x0501 */

WINUSERAPI
BOOL
WINAPI
CSGetGUIThreadInfo(
    IN DWORD idThread,
    OUT PCSGUITHREADINFO pgui);

WINUSERAPI
UINT
WINAPI
CSGetWindowModuleFileName(
    IN HCSWND     hwnd,
    OUT LPTSTR pszFileName,
    IN UINT     cchFileNameMax);

#ifndef NO_STATE_FLAGS
#define STATE_SYSTEM_UNAVAILABLE        0x00000001  // Disabled
#define STATE_SYSTEM_SELECTED           0x00000002
#define STATE_SYSTEM_FOCUSED            0x00000004
#define STATE_SYSTEM_PRESSED            0x00000008
#define STATE_SYSTEM_CHECKED            0x00000010
#define STATE_SYSTEM_MIXED              0x00000020  // 3-state checkbox or toolbar button
#define STATE_SYSTEM_INDETERMINATE      STATE_SYSTEM_MIXED
#define STATE_SYSTEM_READONLY           0x00000040
#define STATE_SYSTEM_HOTTRACKED         0x00000080
#define STATE_SYSTEM_DEFAULT            0x00000100
#define STATE_SYSTEM_EXPANDED           0x00000200
#define STATE_SYSTEM_COLLAPSED          0x00000400
#define STATE_SYSTEM_BUSY               0x00000800
#define STATE_SYSTEM_FLOATING           0x00001000  // Children "owned" not "contained" by parent
#define STATE_SYSTEM_MARQUEED           0x00002000
#define STATE_SYSTEM_ANIMATED           0x00004000
#define STATE_SYSTEM_INVISIBLE          0x00008000
#define STATE_SYSTEM_OFFSCREEN          0x00010000
#define STATE_SYSTEM_SIZEABLE           0x00020000
#define STATE_SYSTEM_MOVEABLE           0x00040000
#define STATE_SYSTEM_SELFVOICING        0x00080000
#define STATE_SYSTEM_FOCUSABLE          0x00100000
#define STATE_SYSTEM_SELECTABLE         0x00200000
#define STATE_SYSTEM_LINKED             0x00400000
#define STATE_SYSTEM_TRAVERSED          0x00800000
#define STATE_SYSTEM_MULTISELECTABLE    0x01000000  // Supports multiple selection
#define STATE_SYSTEM_EXTSELECTABLE      0x02000000  // Supports extended selection
#define STATE_SYSTEM_ALERT_LOW          0x04000000  // This information is of low priority
#define STATE_SYSTEM_ALERT_MEDIUM       0x08000000  // This information is of medium priority
#define STATE_SYSTEM_ALERT_HIGH         0x10000000  // This information is of high priority
#define STATE_SYSTEM_PROTECTED          0x20000000  // access to this is restricted
#define STATE_SYSTEM_VALID              0x3FFFFFFF
#endif

#define CCHILDREN_TITLEBAR              5
#define CCHILDREN_SCROLLBAR             5

/*
 * Information about the global cursor.
 */
typedef struct tagCSCURSORINFO
{
    DWORD   cbSize;
    DWORD   flags;
    HCSCURSOR hCursor;
    CSPOINT   ptScreenPos;
} CSCURSORINFO, *PCSCURSORINFO, *LPCSCURSORINFO;

#define CURSOR_SHOWING     0x00000001

WINUSERAPI
BOOL
WINAPI
CSGetCursorInfo(
    OUT PCSCURSORINFO pci
);

/*
 * Window information snapshot
 */
typedef struct tagCSWINDOWINFO
{
    DWORD cbSize;
    CSRECT  rcWindow;
    CSRECT  rcClient;
    DWORD dwStyle;
    DWORD dwExStyle;
    DWORD dwWindowStatus;
    UINT  cxWindowBorders;
    UINT  cyWindowBorders;
    ATOM  atomWindowType;
    WORD  wCreatorVersion;
} CSWINDOWINFO, *PCSWINDOWINFO, *LPCSWINDOWINFO;

#define WS_ACTIVECAPTION    0x0001

WINUSERAPI
BOOL
WINAPI
CSGetWindowInfo(
    IN HCSWND hwnd,
    OUT PCSWINDOWINFO pwi
);

/*
 * Titlebar information.
 */
typedef struct tagCSTITLEBARINFO
{
    DWORD cbSize;
    CSRECT  rcTitleBar;
    DWORD rgstate[CCHILDREN_TITLEBAR+1];
} CSTITLEBARINFO, *PCSTITLEBARINFO, *LPCSTITLEBARINFO;

WINUSERAPI
BOOL
WINAPI
CSGetTitleBarInfo(
    IN HCSWND hwnd,
    OUT PCSTITLEBARINFO pti
);

/*
 * Menubar information
 */
typedef struct tagCSMENUBARINFO
{
    DWORD cbSize;
    CSRECT  rcBar;          // rect of bar, popup, item
    HCSMENU hMenu;          // real menu handle of bar, popup
    HCSWND  hwndMenu;       // hwnd of item submenu if one
    BOOL  fBarFocused:1;  // bar, popup has the focus
    BOOL  fFocused:1;     // item has the focus
} CSMENUBARINFO, *PCSMENUBARINFO, *LPCSMENUBARINFO;

WINUSERAPI
BOOL
WINAPI
CSGetMenuBarInfo(
    IN HCSWND hwnd,
    IN LONG idObject,
    IN LONG idItem,
    OUT PCSMENUBARINFO pmbi
);

/*
 * Scrollbar information
 */
typedef struct tagCSSCROLLBARINFO
{
    DWORD cbSize;
    CSRECT  rcScrollBar;
    int   dxyLineButton;
    int   xyThumbTop;
    int   xyThumbBottom;
    int   reserved;
    DWORD rgstate[CCHILDREN_SCROLLBAR+1];
} CSSCROLLBARINFO, *PCSSCROLLBARINFO, *LPCSSCROLLBARINFO;

WINUSERAPI
BOOL
WINAPI
CSGetScrollBarInfo(
    IN HCSWND hwnd,
    IN LONG idObject,
    OUT PCSSCROLLBARINFO psbi
);

/*
 * Combobox information
 */
typedef struct tagCSCOMBOBOXINFO
{
    DWORD cbSize;
    CSRECT  rcItem;
    CSRECT  rcButton;
    DWORD stateButton;
    HCSWND  hwndCombo;
    HCSWND  hwndItem;
    HCSWND  hwndList;
} CSCOMBOBOXINFO, *PCSCOMBOBOXINFO, *LPCSCOMBOBOXINFO;

WINUSERAPI
BOOL
WINAPI
CSGetComboBoxInfo(
    IN HCSWND hwndCombo,
    OUT PCSCOMBOBOXINFO pcbi
);

/*
 * The "real" ancestor window
 */
#define     GA_PARENT       1
#define     GA_ROOT         2
#define     GA_ROOTOWNER    3

WINUSERAPI
HCSWND
WINAPI
CSGetAncestor(
    IN HCSWND hwnd,
    IN UINT gaFlags
);


/*
 * This gets the REAL child window at the point.  If it is in the dead
 * space of a group box, it will try a sibling behind it.  But static
 * fields will get returned.  In other words, it is kind of a cross between
 * ChildWindowFromPointEx and WindowFromPoint.
 */
WINUSERAPI
HCSWND
WINAPI
CSRealChildWindowFromPoint(
    IN HCSWND hwndParent,
    IN CSPOINT ptParentClientCoords
);


/*
 * This gets the name of the window TYPE, not class.  This allows us to
 * recognize ThunderButton32 et al.
 */
WINUSERAPI
UINT
WINAPI
CSRealGetWindowClass(
    IN HCSWND  hwnd,
    OUT LPTSTR pszType,
    IN UINT  cchType
);
/*
 * This gets the name of the window TYPE, not class.  This allows us to
 * recognize ThunderButton32 et al.
 */

/*
 * Alt-Tab CSSwitch window information.
 */
typedef struct tagCSALTTABINFO
{
    DWORD cbSize;
    int   cItems;
    int   cColumns;
    int   cRows;
    int   iColFocus;
    int   iRowFocus;
    int   cxItem;
    int   cyItem;
    CSPOINT ptStart;
} CSALTTABINFO, *PCSALTTABINFO, *LPCSALTTABINFO;

WINUSERAPI
BOOL
WINAPI
CSGetAltTabInfo(
    IN HCSWND hwnd,
    IN int iItem,
    OUT PCSALTTABINFO pati,
    OUT LPTSTR pszItemText,
    IN UINT cchItemText
);

/*
 * Listbox information.
 * Returns the number of items per row.
 */
WINUSERAPI
DWORD
WINAPI
CSGetListBoxInfo(
    IN HCSWND hwnd
);

#endif /* NOWINABLE */
#endif /* WINVER >= 0x0500 */


#if(_WIN32_WINNT >= 0x0500)
WINUSERAPI
BOOL
WINAPI
CSLockWorkStation(
    void);
#endif /* _WIN32_WINNT >= 0x0500 */

#if(_WIN32_WINNT >= 0x0500)

WINUSERAPI
BOOL
WINAPI
CSUserHandleGrantAccess(
    HCSHANDLE hUserHandle,
    HCSHANDLE hJob,
    BOOL   bGrant);

#endif /* _WIN32_WINNT >= 0x0500 */

#if(_WIN32_WINNT >= 0x0501)

/*
 * Raw Input Messages.
 */

DECLARE_HANDLE(HCSRAWINPUT);

/*
 * WM_INPUT wParam
 */

/*
 * Use this macro to get the input code from wParam.
 */
#define GET_RAWINPUT_CODE_WPARAM(wParam)    ((wParam) & 0xff)

/*
 * The input is in the regular message flow,
 * the app is required to call DefWindowProc
 * so that the system can perform clean ups.
 */
#define RIM_INPUT       0

/*
 * The input is sink only. The app is expected
 * to behave nicely, honoring RIM_FOREGROUND etc.
 */
#define RIM_INPUTSINK   1


/*
 * Raw Input data header
 */
typedef struct tagCSRAWINPUTHEADER {
    DWORD dwType;
    DWORD dwSize;
    HCSHANDLE hDevice;
    WPARAM wParam;
} CSRAWINPUTHEADER, *PCSRAWINPUTHEADER, *LPCSRAWINPUTHEADER;

/*
 * Type of the raw input
 */
#define RIM_TYPEMOUSE       0
#define RIM_TYPEKEYBOARD    1
#define RIM_TYPEHID         2

/*
 * Raw format of the mouse input
 */
typedef struct tagCSRAWMOUSE {
    /*
     * Indicator flags.
     */
    USHORT usFlags;

    /*
     * The transition state of the mouse buttons.
     */
    union {
        ULONG ulButtons;
        struct  {
            USHORT  usButtonFlags;
            USHORT  usButtonData;
        }u;
    }u1;


    /*
     * The raw state of the mouse buttons.
     */
    ULONG ulRawButtons;

    /*
     * The signed relative or absolute motion in the X direction.
     */
    LONG lLastX;

    /*
     * The signed relative or absolute motion in the Y direction.
     */
    LONG lLastY;

    /*
     * Device-specific additional information for the event.
     */
    ULONG ulExtraInformation;

} CSRAWMOUSE, *PCSRAWMOUSE, *LPCSRAWMOUSE;

/*
 * Define the mouse button state indicators.
 */

#define RI_MOUSE_LEFT_BUTTON_DOWN   0x0001  // Left Button changed to down.
#define RI_MOUSE_LEFT_BUTTON_UP     0x0002  // Left Button changed to up.
#define RI_MOUSE_RIGHT_BUTTON_DOWN  0x0004  // Right Button changed to down.
#define RI_MOUSE_RIGHT_BUTTON_UP    0x0008  // Right Button changed to up.
#define RI_MOUSE_MIDDLE_BUTTON_DOWN 0x0010  // Middle Button changed to down.
#define RI_MOUSE_MIDDLE_BUTTON_UP   0x0020  // Middle Button changed to up.

#define RI_MOUSE_BUTTON_1_DOWN      RI_MOUSE_LEFT_BUTTON_DOWN
#define RI_MOUSE_BUTTON_1_UP        RI_MOUSE_LEFT_BUTTON_UP
#define RI_MOUSE_BUTTON_2_DOWN      RI_MOUSE_RIGHT_BUTTON_DOWN
#define RI_MOUSE_BUTTON_2_UP        RI_MOUSE_RIGHT_BUTTON_UP
#define RI_MOUSE_BUTTON_3_DOWN      RI_MOUSE_MIDDLE_BUTTON_DOWN
#define RI_MOUSE_BUTTON_3_UP        RI_MOUSE_MIDDLE_BUTTON_UP

#define RI_MOUSE_BUTTON_4_DOWN      0x0040
#define RI_MOUSE_BUTTON_4_UP        0x0080
#define RI_MOUSE_BUTTON_5_DOWN      0x0100
#define RI_MOUSE_BUTTON_5_UP        0x0200

/*
 * If usButtonFlags has RI_MOUSE_WHEEL, the wheel delta is stored in usButtonData.
 * Take it as a signed value.
 */
#define RI_MOUSE_WHEEL              0x0400

/*
 * Define the mouse indicator flags.
 */
#define MOUSE_MOVE_RELATIVE         0
#define MOUSE_MOVE_ABSOLUTE         1
#define MOUSE_VIRTUAL_DESKTOP    0x02  // the coordinates are mapped to the virtual desktop
#define MOUSE_ATTRIBUTES_CHANGED 0x04  // requery for mouse attributes


/*
 * Raw format of the keyboard input
 */
typedef struct tagCSRAWKEYBOARD {
    /*
     * The "make" scan code (key depression).
     */
    USHORT MakeCode;

    /*
     * The flags field indicates a "break" (key release) and other
     * miscellaneous scan code information defined in ntddkbd.h.
     */
    USHORT Flags;

    USHORT Reserved;

    /*
     * Windows message compatible information
     */
    USHORT VKey;
    UINT   Message;

    /*
     * Device-specific additional information for the event.
     */
    ULONG ExtraInformation;


} CSRAWKEYBOARD, *PCSRAWKEYBOARD, *LPCSRAWKEYBOARD;


/*
 * Define the keyboard overrun MakeCode.
 */

#define KEYBOARD_OVERRUN_MAKE_CODE    0xFF

/*
 * Define the keyboard input data Flags.
 */
#define RI_KEY_MAKE             0
#define RI_KEY_BREAK            1
#define RI_KEY_E0               2
#define RI_KEY_E1               4
#define RI_KEY_TERMSRV_SET_LED  8
#define RI_KEY_TERMSRV_SHADOW   0x10


/*
 * Raw format of the input from Human Input Devices
 */
typedef struct tagCSRAWHID {
    DWORD dwSizeHid;    // byte size of each report
    DWORD dwCount;      // number of input packed
    BYTE bRawData[1];
} CSRAWHID, *PCSRAWHID, *LPCSRAWHID;

/*
 * CSRAWINPUT data structure.
 */
typedef struct tagCSRAWINPUT {
    CSRAWINPUTHEADER header;
    union {
        CSRAWMOUSE    mouse;
        CSRAWKEYBOARD keyboard;
        CSRAWHID      hid;
    } data;
} CSRAWINPUT, *PCSRAWINPUT, *LPCSRAWINPUT;

#ifdef _WIN64
#define RAWINPUT_ALIGN(x)   (((x) + sizeof(QWORD) - 1) & ~(sizeof(QWORD) - 1))
#else   // _WIN64
#define RAWINPUT_ALIGN(x)   (((x) + sizeof(DWORD) - 1) & ~(sizeof(DWORD) - 1))
#endif  // _WIN64
#undef NEXTRAWINPUTBLOCK
#define NEXTRAWINPUTBLOCK(ptr) ((PCSRAWINPUT)RAWINPUT_ALIGN((ULONG_PTR)((PBYTE)(ptr) + (ptr)->header.dwSize)))

/*
 * Flags for CSGetRawInputData
 */

#define RID_INPUT               0x10000003
#define RID_HEADER              0x10000005

WINUSERAPI
UINT
WINAPI
CSGetRawInputData(
    IN HCSRAWINPUT    hRawInput,
    IN UINT         uiCommand,
    OUT LPVOID      pData,
    IN OUT PUINT    pcbSize,
    IN UINT         cbSizeHeader);

/*
 * Raw Input Device Information
 */
#define RIDI_PREPARSEDDATA      0x20000005
#define RIDI_DEVICENAME         0x20000007  // the return valus is the character length, not the byte size
#define RIDI_DEVICEINFO         0x2000000b

typedef struct tagCSRID_DEVICE_INFO_MOUSE {
    DWORD dwId;
    DWORD dwNumberOfButtons;
    DWORD dwSampleRate;
} CSRID_DEVICE_INFO_MOUSE, *PCSRID_DEVICE_INFO_MOUSE;

typedef struct tagCSRID_DEVICE_INFO_KEYBOARD {
    DWORD dwType;
    DWORD dwSubType;
    DWORD dwKeyboardMode;
    DWORD dwNumberOfFunctionKeys;
    DWORD dwNumberOfIndicators;
    DWORD dwNumberOfKeysTotal;
} CSRID_DEVICE_INFO_KEYBOARD, *PCSRID_DEVICE_INFO_KEYBOARD;

typedef struct tagCSRID_DEVICE_INFO_HID {
    DWORD dwVendorId;
    DWORD dwProductId;
    DWORD dwVersionNumber;

    /*
     * Top level collection UsagePage and Usage
     */
    USHORT usUsagePage;
    USHORT usUsage;
} CSRID_DEVICE_INFO_HID, *PCSRID_DEVICE_INFO_HID;

typedef struct tagCSRID_DEVICE_INFO {
    DWORD cbSize;
    DWORD dwType;
    union {
        CSRID_DEVICE_INFO_MOUSE mouse;
        CSRID_DEVICE_INFO_KEYBOARD keyboard;
        CSRID_DEVICE_INFO_HID hid;
    }u;
} CSRID_DEVICE_INFO, *PCSRID_DEVICE_INFO, *LPCSRID_DEVICE_INFO;

WINUSERAPI
UINT
WINAPI
CSGetRawInputDeviceInfo(
    IN HCSHANDLE hDevice,
    IN UINT uiCommand,
    OUT LPVOID pData,
    IN OUT PUINT pcbSize);


/*
 * Raw Input Bulk Read: CSGetRawInputBuffer
 */
WINUSERAPI
UINT
WINAPI
CSGetRawInputBuffer(
    OUT PCSRAWINPUT   pData,
    IN OUT PUINT    pcbSize,
    IN UINT         cbSizeHeader);

/*
 * Raw Input request APCSIs
 */
typedef struct tagCSRAWINPUTDEVICE {
    USHORT usUsagePage; // Toplevel collection UsagePage
    USHORT usUsage;     // Toplevel collection Usage
    DWORD dwFlags;
    HCSWND hwndTarget;    // Target hwnd. NULL = follows keyboard focus
} CSRAWINPUTDEVICE, *PCSRAWINPUTDEVICE, *LPCSRAWINPUTDEVICE;

typedef CONST CSRAWINPUTDEVICE* PCCSRAWINPUTDEVICE;

#define RIDEV_REMOVE            0x00000001
#define RIDEV_EXCLUDE           0x00000010
#define RIDEV_PAGEONLY          0x00000020
#define RIDEV_NOLEGACY          0x00000030
#define RIDEV_CAPTUREMOUSE      0x00000200  // effective when mouse nolegacy is specified, otherwise it would be an error
#define RIDEV_NOHOTKEYS         0x00000200  // effective for keyboard.
#define RIDEV_EXMODEMASK        0x000000F0

#define RIDEV_EXMODE(mode)  ((mode) & RIDEV_EXMODEMASK)

WINUSERAPI
BOOL
WINAPI
CSRegisterRawInputDevices(
    IN PCCSRAWINPUTDEVICE pRawInputDevices,
    IN UINT uiNumDevices,
    IN UINT cbSize);

WINUSERAPI
UINT
WINAPI
CSGetRegisteredRawInputDevices(
    OUT PCSRAWINPUTDEVICE pRawInputDevices,
    IN OUT PUINT puiNumDevices,
    IN UINT cbSize);


typedef struct tagCSRAWINPUTDEVICELIST {
    HCSHANDLE hDevice;
    DWORD dwType;
} CSRAWINPUTDEVICELIST, *PCSRAWINPUTDEVICELIST;

WINUSERAPI
UINT
WINAPI
CSGetRawInputDeviceList(
    OUT PCSRAWINPUTDEVICELIST pRawInputDeviceList,
    IN OUT PUINT puiNumDevices,
    IN UINT cbSize);


WINUSERAPI
LRESULT
WINAPI
CSDefRawInputProc(
    IN PCSRAWINPUT* paRawInput,
    IN INT nInput,
    IN UINT cbSizeHeader);


#endif /* _WIN32_WINNT >= 0x0501 */




#if !defined(RC_INVOKED) /* RC complains about long symbols in #ifs */
#if ISOLATION_AWARE_ENABLED
#include "winuser.inl"
#endif /* ISOLATION_AWARE_ENABLED */
#endif /* RC */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* !_WINUSER_ */





