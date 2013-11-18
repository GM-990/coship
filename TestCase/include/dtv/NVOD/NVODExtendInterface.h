#ifndef NVODExtendInterface_H
#define NVODExtendInterface_H


#include "mmcp_typedef.h"
#include "mmcp_debug.h"


#ifdef  __cplusplus
extern "C" {
#endif

#define NVODEXTINFOLEN  (1024)
#define PRICELEN        (16)

typedef struct _NVODExtInfo
{
	//�۸�
	BYTE m_wPrice[ PRICELEN ];
	
	//��Ŀ����
	BYTE m_cExtDescriptor[ NVODEXTINFOLEN ];

}NVODExtInfo;



//����NVOD��չ���
HCSHANDLE CreateNVODExtProcess( HCSHANDLE hMemoryPool );

//����NVOD��չ���
BOOL FreeNVODExtProcess( HCSHANDLE pNVODExtProcess );

//��NVOD��չ���ע�ᵽNVODģ����  hNVODΪNVODģ������ hExtProcessΪNVOD��չ�����
BOOL CSUSPNVODRegisterUserListener( HCSHANDLE hNVOD, HCSHANDLE hExtProcess );


#ifdef  __cplusplus
}
#endif


#endif /* NVODExtendInterface_H */


