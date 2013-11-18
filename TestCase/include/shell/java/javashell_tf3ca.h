
#ifndef SHELL_TF3CA_H
#define SHELL_TF3CA_H

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "thirdparty/ca/tongfang3/CS_TFCA_Interface.h"

typedef struct _Tf3CaFunc
{
	int (*fnGetEmailTotalNum)(BYTE* pcEmailNum, BYTE* pcEmptyNum);
	void (*fnDeleteMail)(int index);
	WORD (*fnGetEmailTitle)(INT nIndex, T_CaEmailHead *pstEmail);
	WORD (*fnGetEmailContent)(INT nIndex, T_CaEmailContent* pstEmailContent);
	void (*fnSortEmail)(T_CaEmailSortStyle eSortStyle);
	int (*fnRestart)(void);	
	int (*fnGetTFLibVersion)(BYTE* pacVerBuf,  DWORD dwBufSize);
	WORD (*fnIsPinLocked)(BYTE* pbPinLock);
	WORD (*fnChangePin)(DWORD dwOldPin, DWORD dwNewPin);
	WORD (*fnGetParentRating)(BYTE* pucRating);
	WORD (*fnSetParentRating)(DWORD dwPinCode, BYTE ucRating);
	WORD (*fnGetWorkTime)(T_WorkingTime* pstStartTime, T_WorkingTime* pstEndTime);
	WORD (*fnSetWorkTime)(DWORD dwPinCode,T_WorkingTime* pstStartTime, T_WorkingTime* pstEndTime);
	WORD (*fnGetOperatorIds)(T_CaOperatorId* pstOperatorsId);
	WORD (*fnGetOperatorInfo)(WORD wOperatorsId, T_CaOperatorInfo* pstOperatorInfo);
	WORD (*fnGetACList)(WORD wOperatorsId, T_CaOperatorAcList* pstAcList);
	WORD (*fnGetServiceEntitles)(WORD wOperatorsId, T_CaServiceEntitles* pstServiceEntitles);
	WORD (*fnGetSlotIDs)(WORD wOperatorsId, T_CaSlotId* pstSlotId);
	WORD (*fnGetSlotInfo)(WORD wOperatorsId, BYTE bySlotID, T_CaSlotInfo* pstSlotInfo);
	WORD (*fnGetIPPVProgram)(WORD wOperatorsId, BYTE bySlotID, T_CaIppvProgInfo* pstIppvInfo);
	WORD (*fnIPPVBuyConfirm)(BYTE cBuyFlag,WORD wEcmPid, DWORD dwPinCode, T_CaIppvPrice* pstPrice);
	WORD (*fnDeleteIPPVProgram)(WORD wOperatorId, BYTE wSlotId, WORD	wProductId, WORD wExpireDate);
	void (*fnChangeTFDate)(WORD wdate, T_CaDate *pstDate);
	int (*fnStartStockPlay)(T_CaFtaService* pstServiceInfo);
	int (*fnStopStockPlay)(T_CaFtaService* pstServiceInfo);
	WORD (*fnGetDetitleInfo)(WORD wOperatorsId,T_DetitleInfo* pstDetileInfo);
	int (*fnDeleteDetitleInfo)(WORD wOperatorsId,DWORD dwDetileNum);
	WORD (*fnGetEntitleInfo)(WORD wOperatorsId,T_EntitleInfo* pstEntileInfo);
	int (*fnHaveNewDetitle)(void);
	WORD (*fnGetPairCardInfo)(WORD wOperatorsId,T_CaChildInfo* pstChildInfo);
	WORD (*fnSetParentPair)(WORD wOperatorsId, BOOL bIsChild, E_PairStatus* eStatus);
	void (*fnClearParentPairStatus)(void);
	DWORD (*fnGetFlashAddress)(DWORD dwSize);
	DWORD (*fnGetBackupFlashAddress)(DWORD dwSize);
	int (*fnGetSmartCardId)(char* pcCardId);
	WORD (*fnIsPaired)(T_PairList * pstPairList);
}Tf3CaFunc_S;

typedef struct
{
	DWORD m_FLSADDR_CA;
	DWORD m_FLSADDR_CABACK;
}Tf3CaConfig_S;

//void registMonitorCat();

void  RegTf3CaFunc(Tf3CaFunc_S tf3cafunc);

void Tf3CaConfigInit(Tf3CaConfig_S caAddrConfig );

#endif


