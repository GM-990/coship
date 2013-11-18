#ifndef __SHOWPCBITMPA_H__
#define __SHOWPCBITMPA_H__

#ifdef  __cplusplus
extern "C" {
#endif

GUIDLL_API BOOL HardSetWnd(HCSWND hwnd);
GUIDLL_API BOOL ShowGUIBitmap(CSRECT* pCSRECT);

GUIDLL_API BOOL HardShowCSBITMAP(HCSDC hHardOption,CSRECT* pCSRECT);
GUIDLL_API HCSBITMAP CreateBITMAP(HCSDC hDC,PBYTE pbPaletteColor,DWORD dwPaletteCount,BYTE* pbData,DWORD dwDataLen,CSRECT* pCSRECT);

GUIDLL_API BOOL AddHardInputEventMessageQueueMsg(DWORD);
GUIDLL_API BOOL AddHardInputEventQueueMsg(CSMSG* msg);

int GetLanguageCount();
int GetLanguageCode(int nIndex, char* szText, int nSize);
int SetStrLanguageCode(char* szText);

#ifdef  __cplusplus
}
#endif

#endif

