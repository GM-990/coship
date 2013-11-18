#ifndef __STARTUP_H
#define __STARTUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CS_Windows.h"

typedef int (/*CALLBACK*/* GUIMAIN)(HCSINSTANCE, HCSINSTANCE, char*, int);
typedef DWORD (/*CALLBACK*/* GUICALL)(CSRECT *pRect);
typedef void* (*GUIMALLOC)(size_t nSize);
typedef void  (*GUIFREE)(void* p);
GUIDLL_API BOOL InitNewCOSHIPGUI(PBYTE pbGUIPool,DWORD dwGUIPoolLen,PDWORD pdwRCData,PDWORD pdwRCDataLen,PDWORD pdwDataType,DWORD dwDataCount);

GUIDLL_API int StartCOSHIGUIApp(GUIMAIN COSHIPGUIMain);
GUIDLL_API int StartCOSHIGUIAppEx(GUIMAIN COSHIPGUIMain,int nPriority,int nStackSize);
GUIDLL_API void GetGUIVersion( char* pszVer, int nVerLen );

GUIDLL_API BOOL AddHardInputEventQueueMsg(CSMSG* pCSMSG);//old interface
GUIDLL_API BOOL AddHardInputEventMessageQueueMsg(DWORD dwKeyCode);

GUIDLL_API BOOL  GetScreenHardProperty(PBYTE *pbShowAddr ,DWORD* pdwWidth,DWORD* pdwHeight,DWORD* pdwColorDepth);
GUIDLL_API BOOL  SetScreenHardProperty(PBYTE pbShowAddr ,DWORD dwWidth,DWORD dwHeight,DWORD dwColorDepth);
GUIDLL_API BOOL  SetScreenPixelFormat(DWORD dwPixelFormat);
GUIDLL_API BOOL  GetScreenPixelFormat(DWORD* pdwPixelFormat);
GUIDLL_API BOOL  SetScreenChangeNotify(GUICALL pfnCall);

GUIDLL_API BOOL CSGUIInit(PBYTE pbGUIPool,DWORD dwGUIPoolLen,PDWORD pdwRCData,PDWORD pdwRCDataLen,PDWORD pdwDataType,DWORD dwDataCount);
GUIDLL_API BOOL CSGUIStart(GUIMAIN COSHIPGUIMain,int nPriority,int nStackSize);
GUIDLL_API BOOL CSGUISetReplaceCode(DWORD dwReplace);

#ifdef __cplusplus
}
#endif

#endif
