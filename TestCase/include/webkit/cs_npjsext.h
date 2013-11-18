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
relativeURL: URL相对路径，unicode码，
relativeURLLen: URL相对路径unicode字符个数
outURL: 返回绝对路径存放地址，输出参数，unicode码
outURLLen: 输入输出参数，输入outURL的地址空间长度，输出返回绝对URL路径字符个数
***/
void JSNpMakeURL(NPP np, unsigned short *relativeURL, int relativeURLLen, unsigned short *outURL, int *outURLLen);

/***
np: NPP对象，通过此对象获取NPP对象所在的webview
void* :返回NPP对象对应的webview的地址，只能在jsext中使用
***/
void * getJsWebView(NPP np);

/***
unsigned int: 当前webview的句柄，用来获取当前的mainframe
void :返回void* ,实际是NPP对象，只能在jsext中使用
***/
void *getMainFrameNppByWebView(unsigned int hWebview);
#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif
