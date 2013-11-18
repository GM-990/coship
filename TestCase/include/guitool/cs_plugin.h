#ifndef CSPLUGIN_H_
#define CSPLUGIN_H_

#define TYPE_MAX_LENGTH 50

#include "GToolsPlugin.h"

typedef GTOOLSPlugin* (*CreatPluginInstance) (GTPluginCreateData * pluginInfo);

typedef struct _CSGTMimeClassInfo
{
    char *		type;			//  ������
    char *		desc;       		//  ����       
    char *		suffixes;    		//   ��չ��
   CreatPluginInstance fPluginCreateFun;
	bool isSupportFocus;    // true ���֧�ֽ��㣬 false �����֧�ֽ��� 
}CSGTMimeClassInfo;

int CSGTPluginRegister(CSGTMimeClassInfo pluginMimeTypes[] , unsigned int counts);

#endif
