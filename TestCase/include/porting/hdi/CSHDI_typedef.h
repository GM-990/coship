#ifndef _CSHDI_TYPEDEF_H_
#define _CSHDI_TYPEDEF_H_

#include "cs_hdicommon_plus.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief �����ֲ��ƽ̨�ӿڵİ汾��Ϣ

@param[out] pcVersionInfo ��ʾ�ַ�������󳤶�
@param[in] nSize ��ڲ�������ֵΪ����CSCreateTimer����ʱ��pvArg������
@return ��ǰ�汾�ĳ���,ʧ�ܷ���0
@note �����ʽΪ��
- "CSHDI-R-���汾.�Ӱ汾.�Ķ����� "; Release Version 
- "CSHDI-D-���汾.�Ӱ汾.�Ķ�����";  Debug Version
- "CSHDI-T-���汾.�Ӱ汾.�Ķ�����";  Test Version 
*/
int CSHDIGetVersion( char* pcVersionInfo, int nSize);

#ifdef __cplusplus
}
#endif

#endif
