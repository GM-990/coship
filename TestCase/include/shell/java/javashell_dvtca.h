
#ifndef SHELL_DVTCA_H
#define SHELL_DVTCA_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "CS_SHIXUNCA_Interface.h"

typedef struct _DvtCaFunc
{
	int (*fnGetCoreVer)(char* pcVer, INT* pnLen);
	int (*fnTxtAdd)(T_CaMail* pstTxt);
	int (*fnTxtGetInfo)(int nIndex, T_CaMail* pstTxt);
	int (*fnTxtDel)(int nIndex);
	int (*fnTxtDelAll)(void);
	int (*fnTxtGetNum)(void);
	int (*fnTxtSave)(void);
	int (*fnTxtSetReadFlag)(int nIndex, BOOL bRead);
	int (*fnTxtHaveNewMailNum)(void);
	int (*fnMMICheckPinCode)(DWORD dwPin);
	int (*fnMMIModifyPinCode)(DWORD dwOldPin, DWORD dwNewPin);
	int (*fnMMIRequestBookPpvMenu)(void);
	int (*fnMMIRequestViewedPpvManu)(void);
	int (*fnMMIPpvBook)(T_IppDetailInfo* pstIppInfo);
	int (*fnMMIPpvCancel)(WORD wEcmPid);
	int (*fnMMIRequestPPIDMoneyInfo)(WORD wPPID);
	int (*fnMMIRequestWorkTime)(void);	
	int (*fnMMIModifyWorkTime)(DWORD dwPin, T_WorkTime* pstWorkTime);
	int (*fnMMIequestSmcInfo)(void);
	int (*fnMMIRequestPPIDInfo)(void);
	int (*fnMMIRequestDetailPPIDInfo)(DWORD dwPin, DWORD dwPpId);
	int (*fnMMINotifyCaShowOSDOver)(DWORD dwDuration);
	int (*fnMMIRequestParentRating)(void);
	int (*fnMMIModifyParentRating)(DWORD dwPin, BYTE cParentRatingValue);
	int (*fnMMIRequestPairInfo)(void);
	int (*fnMMIPairing)(T_PairInfo* pstPairInfo);	
	BOOL (*fnMMIIsPinLock)(void);
	int (*fnMMILockPin)(void);
	BOOL (*fnMMIIsPinPass)(void);
	int (*fnMMIGetSmcStatuss)(E_SmsStatus* pcSmcStatus);	
	int (*fnstartBoardCast)(BYTE* pcPmtData, INT nPmtLen, BYTE* pcCatData, INT nCatLen, WORD wTsId, WORD wOrgNetId);	
	int (*fnsetFakeFlag)(BYTE cFakeFlag);
	BOOL (*fnstopBoardCast)(void);
	int (*fnMMIRequestAreaBlockInfo)(void);
	int (*fnsetLanguageMode)(E_LanguageType eLanguageType);	
	int (*fnprocAreaLockCatInfo)(BYTE *pcData, INT nLen, BYTE cDemux);
	int (*fnMMIGetMotherInfo)(void);
	int (*fnSetLockFrequency)(int nFre);
	void (*m_fnCACASDebugMsgAdd)(BOOL bOpen);//CSCACASDebugMsgAdd
	int (*m_fnPDSDProc)(DWORD dwPDSD);///CS_CA_PDSDProc
	int (*fnGetCaSpecificInfo)(HCSHANDLE hService, CASpecificInfo_S* pstCaInfo);
	int (*m_fnConfirmFreePreview)(BOOL bConfirmed);///CS_CA_MMI_ConfirmFreePreview
}DvtCaFunc_S;

typedef struct
{
	DWORD m_FLSADDR_CA;	
	DWORD m_BackupFLSADDR_CA;	
}DVTCaConfig_S;


void DVTCaConfigInit(DVTCaConfig_S caAddrConfig );

void RegDvtCaFunc(DvtCaFunc_S);

#endif

