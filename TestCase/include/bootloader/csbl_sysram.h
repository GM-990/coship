#ifndef _CS_SYS_RAM_H_
#define _CS_SYS_RAM_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "csbl_sysdata.h"


#define M_SYS_RAM_VER ("00.00.01")


//============Ram=============
#define M_SYS_RAM_TAG "RAM__INFO__TAGS"

typedef enum
{
	EM_CSBL_RAM_ID_0,
	EM_CSBL_RAM_ID_1,
	EM_CSBL_RAM_ID_2,
	EM_CSBL_RAM_ID_3,
	

	EM_CSBL_RAM_ID_UNKWOWN	
} CSBLSysRamId_E;

typedef CSBLInfoInSysData_S CSBLSysRam_S;

typedef struct
{
	unsigned int m_dwId;
	unsigned int m_dwAddr;
	unsigned int m_dwSize;
} CSBLSysRamItem_S;



CSUDI_Error_Code SysRamInit(PBYTE pBuf);
CSUDI_Error_Code GetSysRam(CSBLSysRam_S * psSysRam);
CSUDI_Error_Code GetSysRamItem(unsigned int dwId, CSBLSysRamItem_S * psRamItem);





#ifdef	__cplusplus
}
#endif

#endif

