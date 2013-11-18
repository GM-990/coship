/******************************************************************************
FileName:  cs_filebrowser.h
Copyright (c) 2008， Shen Zhen Coship Electronics CO.,LTD.。

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

文件描述:


修改记录:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      罗金华           Create

*******************************************************************************/
#ifndef _CS_FILE_BROWSER_H_
#define _CS_FILE_BROWSER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"

typedef enum _FILEBRNotifyType_E
{
	EM_FILEBR_UPDATE = 0/*文件浏览器更新了，新发现存储设备或者存储设备被移除时通知*/
}FILEBRNotifyType_E;

typedef void (*CSFILEBRNotify_F)(HCSHANDLE hBrowser, FILEBRNotifyType_E eType, DWORD dwNotifyData, void* pUserData);

/*模块初始化函数*/
BOOL CSFILEBRInit(void);

/*!
	创建文件浏览器句柄
\note如果创建文件浏览器时,系统已有存储设备,则自动浏览默认分区路径的文件
*/
HCSHANDLE CSFILEBRCreate(CSFILEBRNotify_F fnCb, void* pUserData);
/*!
	获取文件浏览器的当前文件夹路径
*/
const char* CSFILEBRGetCurDir(HCSHANDLE hBrowser);

/*!
	重新设置文件识别标记
\note 重新设置文件识别标记后,文件浏览器自动重新浏览系统默认分区路径
*/
BOOL CSFILEBRSetMark(HCSHANDLE hBrowser, const char* szMark);

BOOL CSFILEBRSetMarks(HCSHANDLE hBrowser,const char** szMark, DWORD nMarkCount);

/*!
	浏览上一级目录
\note 如果当前目录是跟目录,则本函数返回失败
*/
BOOL CSFILEBRBrowseParentDir(HCSHANDLE hBrowser);

/*!
	判断当前浏览器的路径是否是根目录
*/
BOOL CSFILEBRIsRootDir(HCSHANDLE hBrowser);

/*!
	获取当前路径下子文件夹的个数
\note 
*/
DWORD CSFILEBRGetSubDirCount(HCSHANDLE hBrowser);

/*!
	获取当前路径下子文件夹的全路径名
*/
const char* CSFILEBRGetSubDirFullPath(HCSHANDLE hBrowser,int nIdx);

/*!
	获取当前路径下合法子文件个数
\note只有后缀名与Mark相匹配的文件才是合法的
*/
DWORD CSFILEBRGetFileCount(HCSHANDLE hBrowser);

/*!
	获取当前路径下合法子文件的全路径名
\note
*/
const char* CSFILEBRGetFileFullPath(HCSHANDLE hBrowser,int nIdx);

/*!
	重新浏览指定的路径
\note
*/
BOOL CSFILEBRBrowse(HCSHANDLE hBrowser,const char *szDir, BOOL bRecursive);

#ifdef __cplusplus
}
#endif

#endif
