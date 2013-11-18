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
	char m_aTag[M_TAG_LENGTH]; //��־
	unsigned int m_dwCrc32;//�����ݲ��ֵ�У��,��m_pDataָ�������,����Ϊm_dwTotLength
	unsigned int m_dwTotLength;
	unsigned int m_dwTotCnt; //��������
	unsigned int m_dwVer;
	unsigned int m_dwHeadCrc32; //������֮ǰ������ͷ��У��,
							//��m_aTag,m_dwCrc32,m_dwTotLength,m_dwTotCnt���ݵ�У��.
							//ע��:����m_dwCrc32У��,����m_dwHeadCrc32У��
	unsigned char * m_pData; //��������
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

