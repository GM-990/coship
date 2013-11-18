#ifndef _CSPLUGIN_H_
#define _CSPLUGIN_H_


#include "npapi.h"

#include "npfunctions.h"

#ifndef QWEBKIT_EXPORT
#define QWEBKIT_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _CSPluginMimeTypeInfo
{
	char *	type;			//  类型名
	char *	desc;       		//  描述       
	char *	suffixes;    		//   扩展名
	bool isSupportFocus;    // true 插件支持焦点， false 插件不支持焦点 
}CSPluginMimeTypeInfo;

typedef struct _CSPluginsLib
{
	NP_GetEntryPointsFuncPtr             NP_GetEntryPoints;
	NP_InitializeFuncPtr                        NP_Initialize;
	NPP_ShutdownProcPtr						NP_Shutdown;
	char identification[32];                 
	CSPluginMimeTypeInfo                         mimeClassInfo;
}CSPluginsLib;

QWEBKIT_EXPORT int CSPluginInfoRegister(CSPluginsLib * Plugin);

void setPluginStaticPath(char *pah);

#ifdef __cplusplus
}
#endif

#endif
