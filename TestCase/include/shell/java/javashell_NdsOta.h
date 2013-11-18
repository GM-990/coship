
#include "mmcp_typedef.h"


typedef struct
{
	DWORD m_dwBCA;
	DWORD m_dwFDCA;
	DWORD m_dwCSData;
	DWORD m_dwFlashHead;
	DWORD m_dwOTA;
}NdsOTAConfig_S;

void javashell_setStandbyStatus(BOOL bStatus);
BOOL javashell_getStandbyStatus();

int NdsGetSTBSWVer();

void NdsOTAInit( NdsOTAConfig_S otaConfig);


