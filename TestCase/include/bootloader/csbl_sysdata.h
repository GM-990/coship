#ifndef _CS_SYS_DATA_H_
#define _CS_SYS_DATA_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "udi2_error.h"

#define M_SYS_DATA_VER ("00.00.01")

#define M_TAG_LENGTH (16)
#define M_NAME_LENGTH_IN_DATA (16)


typedef struct
{
	char m_aTag[M_TAG_LENGTH]; //标志
	unsigned int m_dwCrc32;//对数据部分的校验,即m_pData指向的内容,长如为m_dwTotLength
	unsigned int m_dwTotLength;
	unsigned int m_dwTotCnt; //数据总数
	unsigned int m_dwVer;
	unsigned int m_dwHeadCrc32; //本数据之前的数据头的校验,
							//即m_aTag,m_dwCrc32,m_dwTotLength,m_dwTotCnt数据的校验.
							//注意:先做m_dwCrc32校验,再做m_dwHeadCrc32校验
	unsigned char * m_pData; //数据内容
} CSBLInfoInSysData_S;




//============parameter===============
#define M_NEW_SYS_DATA_TAG "NEW_SYSDATA_TAG"


typedef CSBLInfoInSysData_S CSBLSysData_S; 

typedef struct
{
	char m_szName[M_NAME_LENGTH_IN_DATA];
	unsigned int m_dwLength;
	unsigned char * m_pData;
} CSBLSysDataItem_S;


CSUDI_Error_Code CSBLSysDataInit(unsigned char * pBuf);
CSUDI_Error_Code CSBLGetSysData(CSBLSysData_S * psSysData);
CSUDI_Error_Code CSBLGetSysDataItem(char * szName, CSBLSysDataItem_S * psSysDataItem);


#ifdef	__cplusplus
}
#endif

#endif

