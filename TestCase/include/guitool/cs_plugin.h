#ifndef CSPLUGIN_H_
#define CSPLUGIN_H_

#define TYPE_MAX_LENGTH 50

#include "GToolsPlugin.h"

typedef GTOOLSPlugin* (*CreatPluginInstance) (GTPluginCreateData * pluginInfo);

typedef struct _CSGTMimeClassInfo
{
    char *		type;			//  类型名
    char *		desc;       		//  描述       
    char *		suffixes;    		//   扩展名
   CreatPluginInstance fPluginCreateFun;
	bool isSupportFocus;    // true 插件支持焦点， false 插件不支持焦点 
}CSGTMimeClassInfo;

int CSGTPluginRegister(CSGTMimeClassInfo pluginMimeTypes[] , unsigned int counts);

#endif
