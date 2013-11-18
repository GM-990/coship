#ifndef _CS_SYS_BOOTPARAM_H_
#define _CS_SYS_BOOTPARAM_H_

#ifdef	__cplusplus
extern "C" {
#endif


#include "csbl_sysdata.h"

#define M_SYS_BOOTPARAM_VER ("00.00.01")


//=========boot parameter=====
#define M_SYS_BOOTPARAM_TAG "BOOT_PARAM_TAGS"


typedef CSBLInfoInSysData_S CSBLSysBootParam_S;

typedef struct
{
	char m_acName[M_NAME_LENGTH_IN_DATA];
	unsigned int m_dwLength;
	unsigned char * m_pData;
} CSBLSysBootParamItem_S;


CSUDI_Error_Code CSBLSysBootParamInit(PBYTE pBuf);
CSUDI_Error_Code CSBLGetSysBootParam(CSBLSysBootParam_S *psBootParam);
CSUDI_Error_Code CSBLGetSysBootParamItem(char * pcName, CSBLSysBootParamItem_S *psBootItem);




#ifdef	__cplusplus
}
#endif

#endif

