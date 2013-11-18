
#ifndef __CS_GUIINFO_H
#define __CS_GUIINFO_H

#ifdef  __cplusplus
extern "C" {
#endif

GUIDLL_API int CSGUIGetCurCSWNDObjectCount( void );
GUIDLL_API int CSGUIGetCurCSDCObjectCount( void );
GUIDLL_API int CSGUIGetCurCSPENObjectCount( void );
GUIDLL_API int CSGUIGetCurCSBRUSHObjectCount( void );
GUIDLL_API int CSGUIGetCurCSBITMAPObjectCount( void );
GUIDLL_API int CSGUIGetCurCSPALETTEObjectCount( void );
GUIDLL_API int CSGUIGetCurCSFONTObjectCount( void );
GUIDLL_API int CSGUIGetCurFreeMemorySize( void );

#ifdef  __cplusplus
}
#endif

#endif
