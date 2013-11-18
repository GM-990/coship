#ifndef _CS_SYS_PARTITION_H_
#define _CS_SYS_PARTITION_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "csbl_sysdata.h"

#define M_SYS_PARTITION_VER ("00.00.01")


//============partition===========
#define M_SYS_PARTITION_TAG "PARTITION__TAGS"


typedef CSBLInfoInSysData_S CSBLSysPartition_S;


typedef struct
{
	char m_acName[M_NAME_LENGTH_IN_DATA]; //��������
	unsigned int m_dwOffset; //����������ƫ�Ƶ�ַ
	unsigned int m_dwSize; //���������������С
	unsigned int m_dwSpareSize; //�����������С
} CSBLSysPartitionItem_S;


CSUDI_Error_Code CSBLSysPartitionInit(PBYTE pBuf);

CSUDI_Error_Code CSBLGetSysPartition(CSBLSysPartition_S * psSysPar);
CSUDI_Error_Code CSBLGetSysPartitionItem(char * pcName, CSBLSysPartitionItem_S * psSysParItem);




#ifdef	__cplusplus
}
#endif

#endif

