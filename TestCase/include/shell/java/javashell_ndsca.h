
#ifndef SHELL_NDSCA_H
#define SHELL_NDSCA_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "CS_NDSCA_Interface.h"


typedef struct _NdsCaFunc
{
	int (*fnGetCoreVer)(char* pcVer, INT* pnLen);
	int (*fnGetSmcInfo)();
	int (*fnGetParentRating) (DWORD dwPin);
	int (*fnGetCurrentLanguage)(void);
	int (*fnGetPinLockStatus)(void);
	int (*fnGetCaDiagData)(void);
	int (*fnGetClearMode)(void);
	int (*fnGetBouquetID)(DWORD* pdwBouquetId);
	int (*fnGetIdentifyNum)(DWORD *pdwNum); 
	int (*fnGetStbLabelInfo)(T_CaStbLabelInfo* pstCaStbLabelInfo);
	int (*fnGetCaSpecificInfo)(HCSHANDLE hService, CASpecificInfo_S* pstCaInfo);
	int (*fnChangeParentRating)(DWORD dwPin, BYTE cParentRatingValue);
	int (*fnChangeStandbyStatus)(BYTE cStandbyStaus);
	int (*fnChangeCurrentLanguage)(E_CaLanguage eLan);
	DWORD (*fnCancelOSD)(void);
	int (*fnGetMailCount)(void);
	int (*fnGetNewMailCout)(void);
	int (*fnGetMailInfo)(int nIndex, T_CaMail* pstTxt);
	int (*fnDeleteMail)(int index) ;	
	int (*fnDeleteAllMail)(void) ;
	int (*fnSaveMail)(void) ;
	int (*fnSetMailReadFlag)(int index, BOOL bFlag) ;	
	int (*fnGetSmartCardID)(char* pacCardIdStr, int* pnLen);
}NdsCaFunc_S;
typedef struct
{
	DWORD m_dwEmail;
	DWORD m_dwCAEEProm;
	DWORD m_dwBCA;
	DWORD m_dwFlashHead;	
	DWORD m_dwCustomerID;
}NdsCaConfig_S;


void registMonitorCat();

void  RegNdsCaFunc(NdsCaFunc_S ndscafunc);

void NdsCaConfigInit(NdsCaConfig_S caAddrConfig );

#endif


