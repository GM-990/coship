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
	//价格
	BYTE m_wPrice[ PRICELEN ];
	
	//节目介绍
	BYTE m_cExtDescriptor[ NVODEXTINFOLEN ];

}NVODExtInfo;



//创建NVOD扩展句柄
HCSHANDLE CreateNVODExtProcess( HCSHANDLE hMemoryPool );

//销毁NVOD扩展句柄
BOOL FreeNVODExtProcess( HCSHANDLE pNVODExtProcess );

//把NVOD扩展句柄注册到NVOD模块中  hNVOD为NVOD模块句柄， hExtProcess为NVOD扩展句柄。
BOOL CSUSPNVODRegisterUserListener( HCSHANDLE hNVOD, HCSHANDLE hExtProcess );


#ifdef  __cplusplus
}
#endif


#endif /* NVODExtendInterface_H */


