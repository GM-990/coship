
#ifndef __CS_HOTVKEY_H
#define __CS_HOTVKEY_H

#ifdef  __cplusplus
extern "C" {
#endif
GUIDLL_API
BOOL AddHOTKEY(HCSWND hParentCSWND,DWORD dwVCode,HCSWND hCSWND);

GUIDLL_API
BOOL DeleteHOTKEY(HCSWND hCSWND,DWORD dwVCode);

GUIDLL_API
BOOL SetDLGParse(HCSWND hCSWND,int nVCode,BOOL bActive);

GUIDLL_API 
int GetFontModelDataFromGUI( HCSFONT hFont, LPCTSTR pcInputStr, int nInputStrLen, PBYTE pbDataBuf, int nDataBufCount, int nOneLinesBytes, int Bits, int nValue );

GUIDLL_API
int GetMultiLineFontModelData(HCSFONT hFont, LPCTSTR pcInputStr, int nInputStrLen, PBYTE pbDataBuf, int nDataBufLen, DWORD dwFormat, int nLineSpace, int nInterCharSpace, int nOffSetX, int nOffSetY, CSSIZE* pCSSIZE);

GUIDLL_API
HCSHANDLE GetScreenPalette(void);

GUIDLL_API
int MatchColor(HCSHANDLE hDCPALETTE,COLORREF dwCOLOR);

#ifdef  __cplusplus
}
#endif

#endif
