#ifndef _CS_SYS_FLASH_H_
#define _CS_SYS_FLASH_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "csbl_sysdata.h"


#define M_SYS_FLASH_VER ("00.00.01")

//================flash========
#define M_SYS_FLASH_TAG "FLASH_INFO_TAGS"

typedef enum
{
	EM_CSBL_FLASH_ID_0,
	EM_CSBL_FLASH_ID_1,

	
	EM_CSBL_FLASH_ID_UNKWOWN	
} CSBLSysFlashId_E;

typedef CSBLInfoInSysData_S CSBLSysFlash_S;

typedef struct
{
	unsigned int m_dwId;
	unsigned int m_dwAddr;
	unsigned int m_dwSize;
} CSBLSysFlashItem_S;


CSUDI_Error_Code SysFlashInit(PBYTE pBuf);
CSUDI_Error_Code GetSysFlash(CSBLSysFlash_S * psSysFlash);
CSUDI_Error_Code GetSysFlashItem(unsigned int dwId, CSBLSysFlashItem_S * psFlash);



#ifdef	__cplusplus
}
#endif

#endif

