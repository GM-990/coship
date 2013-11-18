#ifndef _CS_NP_JSEXT_H
#define _CS_NP_JSEXT_H

#include "npruntime.h"
#include "npfunctions.h"

#ifdef __cplusplus
extern "C" {
#endif

bool addJavaScriptObject(char * name,NPClass * obj);

NPObject *JSCreateScriptObject(NPP np);
void JSDelScriptObject(NPObject *obj);

void getNPNBrowserFuncs(NPNetscapeFuncs *browserFuncs);

/***
relativeURL: URL���·����unicode�룬
relativeURLLen: URL���·��unicode�ַ�����
outURL: ���ؾ���·����ŵ�ַ�����������unicode��
outURLLen: �����������������outURL�ĵ�ַ�ռ䳤�ȣ�������ؾ���URL·���ַ�����
***/
void JSNpMakeURL(NPP np, unsigned short *relativeURL, int relativeURLLen, unsigned short *outURL, int *outURLLen);

/***
np: NPP����ͨ���˶����ȡNPP�������ڵ�webview
void* :����NPP�����Ӧ��webview�ĵ�ַ��ֻ����jsext��ʹ��
***/
void * getJsWebView(NPP np);

/***
unsigned int: ��ǰwebview�ľ����������ȡ��ǰ��mainframe
void :����void* ,ʵ����NPP����ֻ����jsext��ʹ��
***/
void *getMainFrameNppByWebView(unsigned int hWebview);
#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif
