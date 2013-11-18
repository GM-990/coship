/****************************************************************************
*                                                                           *
* windef.h -- Basic Windows Type Definitions                                *
*                                                                           *
* Copyright (c) Coship Corporation. All rights reserved.                 *
*                                                                           *
****************************************************************************/


#ifndef _CS_WINDEF_
#define _CS_WINDEF_
//////////////////////////////////////////// y_junjie added;
#undef DECLARE_HANDLE
#define DECLARE_HANDLE(name) typedef DWORD name
//////////////////////////////////////////////////
#ifndef NO_STRICT
#ifndef STRICT
#define STRICT 1
#endif
#endif /* NO_STRICT */

// Win32 defines _WIN32 automatically,
// but Macintosh doesn't, so if we are using
// Win32 Functions, we must do it here

#ifndef _WIN32
#define _WIN32
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif /* WINVER */

/*
 * BASETYPES is defined in ntdef.h if these types are already defined
 */

#define MAX_PATH          260


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifndef QS_QUIT
#define QS_QUIT				0x0800
#endif

#ifndef ZERO
#define ZERO 0
#endif

#ifndef BS_TYPEMASK
#define BS_TYPEMASK			0x0f
#endif


#undef far
#undef near
#undef pascal

#define far
#define near
#if (!defined(_MAC)) && ((_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED))
#define pascal __stdcall
#else
#define pascal
#endif

#if defined(DOSWIN32) || defined(_MAC)
#define cdecl _cdecl
#ifndef CDECL
#define CDECL _cdecl
#endif
#else
#define cdecl
#ifndef CDECL
#define CDECL
#endif
#endif

#ifdef _MAC
#define CALLBACK    PASCAL
#define WINAPI      CDECL
#define WINAPIV     CDECL
#define APIENTRY    WINAPI
#define APIPRIVATE  CDECL
#ifdef _68K_
#define PASCAL      __pascal
#else
#define PASCAL
#endif
#elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define CALLBACK    __stdcall
#define WINAPI      __stdcall
#define WINAPIV     __cdecl
#define APIENTRY    WINAPI
#define APIPRIVATE  __stdcall
#define PASCAL      __stdcall
#else
#define CALLBACK
#define WINAPI
#define WINAPIV
#define APIENTRY    WINAPI
#define APIPRIVATE
#define PASCAL      pascal
#endif

#undef FAR
#undef  NEAR
#define FAR                 
#define NEAR                

#undef CONST
#define CONST               const

#ifndef WIN32
typedef  unsigned short		WCHAR;
typedef  WCHAR				*PWSTR;
typedef  WCHAR				*LPWSTR;
typedef  CONST WCHAR		*LPCWSTR;  
typedef  CONST CHAR			*LPCSTR, *PCSTR;
typedef  LPCSTR 			PCTSTR, LPCTSTR;
//
// ANSI (Multi-byte Character) types
//
typedef CHAR *LPCH, *PCH;

typedef CONST CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR;
typedef CHAR *LPSTR, *PSTR;

typedef  char				TCHAR;
typedef  TCHAR 				*LPTSTR;

typedef  LONG				INT_PTR;
typedef  DWORD				UINT_PTR;
typedef  DWORD 				LONG_PTR;
typedef  DWORD 				ULONG_PTR;    

typedef ULONG_PTR DWORD_PTR,*PDWORD_PTR;
typedef ULONG_PTR SIZE_T,	*PSIZE_T;
typedef LONG_PTR SSIZE_T,	*PSSIZE_T;

typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;


typedef WORD				LANGID;      
typedef LONG				SCODE;
typedef DWORD				ACCESS_MASK;
typedef ACCESS_MASK			*PACCESS_MASK;
#endif

typedef struct _CSLUID {
	DWORD LowPart;
	LONG HighPart;
} CSLUID, *PCSLUID;
#ifndef _CSSYSTEMTIME_
#define _CSSYSTEMTIME_
typedef struct _CSSYSTEMTIME
    {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
    } 	CSSYSTEMTIME;

typedef struct _CSSYSTEMTIME *PCSSYSTEMTIME;

typedef struct _CSSYSTEMTIME *LPCSSYSTEMTIME;

#endif // !_SYSTEMTIME
#ifndef _CSSECURITY_ATTRIBUTES_
#define _CSSECURITY_ATTRIBUTES_
typedef struct _CSSECURITY_ATTRIBUTES
    {
    DWORD nLength;
    /* [size_is] */ LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
    } 	CSSECURITY_ATTRIBUTES;

typedef struct _CSSECURITY_ATTRIBUTES *PCSSECURITY_ATTRIBUTES;

typedef struct _CSSECURITY_ATTRIBUTES *LPCSSECURITY_ATTRIBUTES;

#endif // !_SECURITY_ATTRIBUTES_

//typedef DWORD HCSHANDLE ;
typedef DWORD HWNDCLASS;
typedef DWORD HSURFACE;
typedef DWORD HGDIOBJECT;
typedef	PVOID SEC_THREAD_START;
typedef DWORD HDCPALETTE;
typedef DWORD HCSWNDZNUM;
typedef DWORD HCLIPCSRECT;
typedef DWORD HBUTTON;
typedef DWORD HSTATIC;
typedef DWORD HDIALOG;
typedef DWORD HCSPALETTEENTRY;
typedef DWORD HPALETTEORDER;
typedef DWORD HPROGRESS;
typedef DWORD HSCROLLBAR;
typedef DWORD HEDIT;
typedef DWORD HLINE;
typedef DWORD HBUF;
typedef DWORD HPATHLIST; 
typedef DWORD HPATHPAR;
typedef DWORD HPATHOBJ;
typedef DWORD HCLIPOBJ;
typedef DWORD HCSRECTTYPE;
typedef DWORD HCLIPLIST;
typedef DWORD HCSICONIMAGEDATA;
typedef DWORD HONESTACKMEMORY;
typedef DWORD HCSGIF89ACFG;
typedef DWORD DHSURF;  
typedef DWORD DHPDEV;

#ifndef NT_INCLUDED
//#include <winnt.h>
#endif /* NT_INCLUDED */

/* Types use for passing & returning polymorphic values */

#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */

#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((DWORD_PTR)(w) >> 8))


#ifndef WIN_INTERNAL
DECLARE_HANDLE            (HCSWND);
DECLARE_HANDLE            (HCSHOOK);
#ifdef WINABLE
DECLARE_HANDLE            (HCSEVENT);
#endif
#endif

typedef WORD                ATOM;

typedef HCSHANDLE NEAR         *SPCSHANDLE;
typedef HCSHANDLE FAR          *LPCSHANDLE;
typedef HCSHANDLE              HCSGLOBAL;
typedef HCSHANDLE              HCSLOCAL;
typedef HCSHANDLE              GLOBALHCSANDLE;
typedef HCSHANDLE              LOCALHCSANDLE;
#ifndef _MAC
#ifdef _WIN64
typedef INT_PTR (FAR WINAPI *FARPROC)();
typedef INT_PTR (NEAR WINAPI *NEARPROC)();
typedef INT_PTR (WINAPI *PROC)();
#else
typedef int (FAR WINAPI *FARPROC)(void);
typedef int (NEAR WINAPI *NEARPROC)(void);
typedef int (WINAPI *PROC)(void);
#endif  // _WIN64
#else
typedef int (CALLBACK *FARPROC)();
typedef int (CALLBACK *NEARPROC)();
typedef int (CALLBACK *PROC)();
#endif

#if !defined(_MAC) || !defined(GDI_INTERNAL)
#ifdef STRICT
//typedef void NEAR* HCSGDIOBJ;
typedef DWORD HCSGDIOBJ;
typedef DWORD HCSOBJECT;//should remove
#else
DECLARE_HANDLE(HCSGDIOBJ);
DECLARE_HANDLE(HCSOBJECT);  //should remove
#endif
#endif

DECLARE_HANDLE(HCSKEY);
typedef HCSKEY *PCSHKEY;

#if !defined(_MAC) || !defined(WIN_INTERNAL)
DECLARE_HANDLE(HCSACCEL);
#endif
#if !defined(_MAC) || !defined(GDI_INTERNAL)
DECLARE_HANDLE(HCSBITMAP);
DECLARE_HANDLE(HCSBRUSH);
#endif
#if(WINVER >= 0x0400)
DECLARE_HANDLE(HCSCOLORSPACE);
#endif /* WINVER >= 0x0400 */
#if !defined(_MAC) || !defined(GDI_INTERNAL)
DECLARE_HANDLE(HCSDC);
#endif
DECLARE_HANDLE(HCSGLRC);          // OpenGL
DECLARE_HANDLE(HCSDESK);
DECLARE_HANDLE(HCSENHMETAFILE);
#if !defined(_MAC) || !defined(GDI_INTERNAL)
DECLARE_HANDLE(HCSFONT);
#endif
DECLARE_HANDLE(HCSICON);
#if !defined(_MAC) || !defined(WIN_INTERNAL)
DECLARE_HANDLE(HCSMENU);
#endif
DECLARE_HANDLE(HCSMETAFILE);
DECLARE_HANDLE(HCSINSTANCE);
typedef HCSINSTANCE HCSMODULE;      /* HMODULEs can be used in place of HINSTANCEs */
#if !defined(_MAC) || !defined(GDI_INTERNAL)
DECLARE_HANDLE(HCSPALETTE);
DECLARE_HANDLE(HCSPEN);
#endif
DECLARE_HANDLE(HCSRGN);
DECLARE_HANDLE(HCSRSRC);
DECLARE_HANDLE(HCSSTR);
DECLARE_HANDLE(HCSTASK);
DECLARE_HANDLE(HCSWINSTA);
DECLARE_HANDLE(HCSKL);

#if(WINVER >= 0x0500)
#ifndef _MAC
DECLARE_HANDLE(HCSMONITOR);
DECLARE_HANDLE(HCSWINEVENTHOOK);
#endif
DECLARE_HANDLE(HCSUMPD);
#endif /* WINVER >= 0x0500 */

#ifndef _MAC
typedef int HFILE;
typedef HCSICON HCSCURSOR;      /* HICONs & HCURSORs are polymorphic */
#else
typedef short HCSFILE;
DECLARE_HANDLE(HCSCURSOR);    /* HICONs & HCURSORs are not polymorphic */
#endif

typedef DWORD   COLORREF;
typedef DWORD   *LPCOLORREF;

#define HFILE_ERROR ((HFILE)-1)

typedef struct tagCSRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} CSRECT, *PCSRECT, NEAR *NPCSRECT, FAR *LPCSRECT;

typedef const CSRECT FAR* LPCCSRECT;

typedef struct _CSRECTL       /* rcl */
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} CSRECTL, *PCSRECTL, *LPCSRECTL;

typedef const CSRECTL FAR* LPCCSRECTL;

typedef struct tagCSPOINT
{
    LONG  x;
    LONG  y;
} CSPOINT, *PCSPOINT, NEAR *NPCSPOINT, FAR *LPCSPOINT;

typedef struct _CSPOINTL      /* ptl  */
{
    LONG  x;
    LONG  y;
} CSPOINTL, *PCSPOINTL;

typedef struct tagCSSIZE
{
    LONG        cx;
    LONG        cy;
} CSSIZE, *PCSSIZE, *LPCSSIZE;

typedef CSSIZE               CSSIZEL;
typedef CSSIZE               *PCSSIZEL, *LPCSSIZEL;

typedef struct tagCSPOINTS
{
#ifndef _MAC
    SHORT   x;
    SHORT   y;
#else
    SHORT   y;
    SHORT   x;
#endif
} CSPOINTS, *PCSPOINTS, *LPCSPOINTS;

/* mode selections for the device mode function */
#define DM_UPDATE           1
#define DM_COPY             2
#define DM_PROMPT           4
#define DM_MODIFY           8

#define DM_IN_BUFFER        DM_MODIFY
#define DM_IN_PROMPT        DM_PROMPT
#define DM_OUT_BUFFER       DM_COPY
#define DM_OUT_DEFAULT      DM_UPDATE

/* device capabilities indices */
#define DC_FIELDS           1
#define DC_PAPERS           2
#define DC_PAPERCSSIZE        3
#define DC_MINEXTENT        4
#define DC_MAXEXTENT        5
#define DC_BINS             6
#define DC_DUPLEX           7
#define DC_CSSIZE             8
#define DC_EXTRA            9
#define DC_VERSION          10
#define DC_DRIVER           11
#define DC_BINNAMES         12
#define DC_ENUMRESOLUTIONS  13
#define DC_FILEDEPENDENCIES 14
#define DC_TRUETYPE         15
#define DC_PAPERNAMES       16
#define DC_ORIENTATION      17
#define DC_COPIES           18

#ifdef __cplusplus
}
#endif

#endif /* _WINDEF_ */

