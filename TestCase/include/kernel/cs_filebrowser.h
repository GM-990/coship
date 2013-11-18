/******************************************************************************
FileName:  cs_filebrowser.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

�ļ�����:


�޸ļ�¼:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      �޽�           Create

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
	EM_FILEBR_UPDATE = 0/*�ļ�����������ˣ��·��ִ洢�豸���ߴ洢�豸���Ƴ�ʱ֪ͨ*/
}FILEBRNotifyType_E;

typedef void (*CSFILEBRNotify_F)(HCSHANDLE hBrowser, FILEBRNotifyType_E eType, DWORD dwNotifyData, void* pUserData);

/*ģ���ʼ������*/
BOOL CSFILEBRInit(void);

/*!
	�����ļ���������
\note��������ļ������ʱ,ϵͳ���д洢�豸,���Զ����Ĭ�Ϸ���·�����ļ�
*/
HCSHANDLE CSFILEBRCreate(CSFILEBRNotify_F fnCb, void* pUserData);
/*!
	��ȡ�ļ�������ĵ�ǰ�ļ���·��
*/
const char* CSFILEBRGetCurDir(HCSHANDLE hBrowser);

/*!
	���������ļ�ʶ����
\note ���������ļ�ʶ���Ǻ�,�ļ�������Զ��������ϵͳĬ�Ϸ���·��
*/
BOOL CSFILEBRSetMark(HCSHANDLE hBrowser, const char* szMark);

BOOL CSFILEBRSetMarks(HCSHANDLE hBrowser,const char** szMark, DWORD nMarkCount);

/*!
	�����һ��Ŀ¼
\note �����ǰĿ¼�Ǹ�Ŀ¼,�򱾺�������ʧ��
*/
BOOL CSFILEBRBrowseParentDir(HCSHANDLE hBrowser);

/*!
	�жϵ�ǰ�������·���Ƿ��Ǹ�Ŀ¼
*/
BOOL CSFILEBRIsRootDir(HCSHANDLE hBrowser);

/*!
	��ȡ��ǰ·�������ļ��еĸ���
\note 
*/
DWORD CSFILEBRGetSubDirCount(HCSHANDLE hBrowser);

/*!
	��ȡ��ǰ·�������ļ��е�ȫ·����
*/
const char* CSFILEBRGetSubDirFullPath(HCSHANDLE hBrowser,int nIdx);

/*!
	��ȡ��ǰ·���ºϷ����ļ�����
\noteֻ�к�׺����Mark��ƥ����ļ����ǺϷ���
*/
DWORD CSFILEBRGetFileCount(HCSHANDLE hBrowser);

/*!
	��ȡ��ǰ·���ºϷ����ļ���ȫ·����
\note
*/
const char* CSFILEBRGetFileFullPath(HCSHANDLE hBrowser,int nIdx);

/*!
	�������ָ����·��
\note
*/
BOOL CSFILEBRBrowse(HCSHANDLE hBrowser,const char *szDir, BOOL bRecursive);

#ifdef __cplusplus
}
#endif

#endif
