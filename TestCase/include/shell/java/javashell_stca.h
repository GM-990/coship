
#ifndef SHELL_STCA_H
#define SHELL_STCA_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "thirdparty/ca/suantong/CS_SUANTONGCA_Interface.h"

typedef struct _STCaFunc
{
	int (*fngetCoreVersion)(char* pcVer, INT* pnLen);
	int (*fncheckPinCode)(DWORD dwPin);
	int (*fnmodifyPinCode)(DWORD dwOldPin, DWORD dwNewPin);
	int (*fnppvConfirm)(DWORD dwIppvNo, DWORD dwPin, BOOL bIsPpvp);
	int (*fnppvCancel)(DWORD dwIppvNo, DWORD dwPin);
	int (*fnrequestSmcInfo)();
	int (*fnrequestPPIDInfo)();
	int (*fnrequestDetailPPIDInfo)(DWORD dwPin, DWORD dwPpId);
	int (*fnrequestRecordSrvAllowedFlag)();
	int (*fnmodifyParentRating)(DWORD dwPin, BYTE cParentRatingValue);
	int (*fnrequestCurrentParentLevel)();
	int (*fngetCurrentProgramNo)();
	int (*fncheckIsNewPPV)(T_PpvInfo * pstOldPPV, T_PpvInfo * pstNewPPV);
	int (*fnsetUserViewOpImpl)(CaUserView_S stUserView,DWORD dwPin);
	void (*fnsetFactoryDefault)(void);
	int (*fngetTSAntiMoveCtrlCode)(BYTE* pcAntiMoveCtrl);
	int (*fnaddMailImpl)(T_CaMail* pstTxt);
	int (*fndeleteMail)(DWORD dwIndex);
	int (*fndeleteAllMail)(void);
	int (*fngetMailNumber)(void);
	int (*fngetMailInfoByIndex)(INT nIndex, T_CaMail* pstTxt);
	int (*fnsaveFlash)(void);
	int (*fnsetReadFlag)(INT nIndex, BOOL bRead);
	int (*fngetNoReadMailNumber)(void);
	int (*fnsetRepeatCheck)(int isCheck);
}STCaFunc_S;

typedef struct
{
	DWORD m_CA_FLSADDR;
	DWORD m_CAMail_FLSADDR;
}STCaConfig_S;

void  RegSTCaFunc(STCaFunc_S stcafunc);

void STCaConfigInit(STCaConfig_S caAddrConfig);

#endif


