
#ifndef __CS_DEFAULTGUIOBJECT_H
#define __CS_DEFAULTGUIOBJECT_H

#ifdef  __cplusplus
extern "C" {
#endif

typedef BOOL ReadCharOutlineDataFUN (DWORD hFont,WORD wStrID,PBYTE pbData,DWORD dwDataLen,int nDataWidth,int nOff,int Bits, int nValue,int* pnActiveWidth,int* pnDataType);
typedef BOOL ExchangeExternInfoFUN( DWORD hFont,int nX, int nY,int nDelta, int nShowBuffWidth, int nBits, COLORREF clrText, COLORREF clrBackGND,PBYTE pCorlor, PBYTE pLev, PBYTE pbData );


typedef struct tagExternFontOption
{
	DWORD hFont;
	int   m_nFontWidth;
	int   m_nFontHeight;
	char  m_pchFontName[32];
	BOOL (* ReadCharOutlineData)( DWORD hFont,WORD wStrID,PBYTE pbData,DWORD dwDataLen,int nDataWidth,int nOff,int Bits, int nValue,int* pnActiveWidth,int* pnDataType);
	BOOL (* ExchangeExternInfo)( DWORD hFont,int nX, int nY,int nDelta, int nShowBuffWidth, int nBits, COLORREF clrText,COLORREF clrBackGND,PBYTE pCorlor, PBYTE pLev, PBYTE pbData );

}EXTERNFONTOPTION;

GUIDLL_API
BOOL SetDefaultSystemLargeFont(HCSFONT hCSFONT);

GUIDLL_API
BOOL SetDefaultSystemSmallFont(HCSFONT hCSFONT);

GUIDLL_API
BOOL CSCreateExternFont(EXTERNFONTOPTION *pEXTERNFONTOPTION);

GUIDLL_API
BOOL CSSetFontCharInterSpaceAndLineSpace( HCSFONT hFont, int nCharInterSpace, int nLineSpace );
#ifdef  __cplusplus
}
#endif

#endif
