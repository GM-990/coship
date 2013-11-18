/**@defgroup CS_ZipUtil模块是kernel对zlib库接口的封装层，以便对外提供更好的可用性。
@brief zip/jar文件读取支持模块
 
@version 1.0 2010/04/27 Initial Version
@version 1.1 2010/06/02 comment修改为中文
@{
*/

#ifndef _CSFILEDOWNLOAD_H_
#define _CSFILEDOWNLOAD_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"
#define CSDOWNLOAD_DIRECTORY "/JSFiledownload"
#define DOWNLOAD_MSG_START 10150

#define CSDATA_RESTOREFILE   ("AllData")     //NGB 此宏定义用以识别是否是恢复到出厂设置

typedef enum CSDownLoadMessageType_E
{
	EM_DOWNLOAD_MSG_START=DOWNLOAD_MSG_START,
    EM_DOWNLOAD_MSG_OK,                  /**/
	EM_DOWNLOAD_MSG_NOJOB,
	EM_DOWNLOAD_MSG_FAIL,
	EM_DOWNLOAD_MSG_TIMEOUT,
	EM_DOWNLOAD_ISEXIST_FILE_OK,
	EM_DOWNLOAD_ISEXIST_FILE_NOJOB,
	EM_DOWNLOAD_ISEXIST_FILE_TIMEOUT,
	EM_DOWNLOAD_COPY_DIRECTORY_SUCORFAIL,
	EM_DOWNLOAD_MOVE_DIRECTORY_SUCORFAIL,
	EM_DOWNLOAD_DEL_DIRECTORY_SUCORFAIL,
	EM_DOWNLOAD_SAVE_PIC_SUCCESS = 10602,
	EM_DOWNLOAD_SAVE_PIC_FAILURE,	
	
	EM_DOWNLOAD_MSG_SAVE_DATA,
	EM_DOWNLOAD_MSG_REMOVE_DATA,
	EM_DOWNLOAD_MSG_REVERT_DATA,
	EM_DOWNLOAD_MSG_RESTORE_TO_DEFAULT,
	EM_DOWNLOAD_MSG_DEVICE_UNINSTALL,
	EM_DOWNLOAD_COPY_FILE_SUCORFAIL,
	EM_DOWNLOAD_MOVE_FILE_SUCORFAIL,
	EM_DOWNLOAD_DEL_FILE_SUCORFAIL	
}CSDownLoadMessageType_E;

typedef enum FileType_E
{
	INVALID_REMOTE_FILE,
	EXIST_REMOTE_OC_FILE = 1,
	DOWNLOAD_REMOTE_OC_FILE,
	EXSIT_REMOTE_HTTP_FILE,
	DOWNLOAD_REMOTE_HTTP_FILE
}FileType_E;

/*
输出参数:nMsg对应CSDownLoadMessageType_E中定义的类型

*/
typedef void (*FOnCSDownloadCallback)(DWORD dwUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);

/*

*/
BOOL CSDownLoadAddCallback(HCSHANDLE hDownLoad, DWORD dwUserData, FOnCSDownloadCallback callback);

/*

*/
HCSHANDLE CSDownLoadGetHandle();
DWORD CSDownLoadHttpDownLoad (HCSHANDLE hDownLoad, char *pcUrl, int nTimeout,FileType_E eFileType );
DWORD CSDownLoadHttpIsExist (HCSHANDLE hDownLoad, char *pcUrl, int nTimeout,FileType_E eFileType );
DWORD CSDownLoadOCDownLoad(HCSHANDLE hDownLoad, char *pcUrl, int nTimeout,FileType_E eFileType );
DWORD CSDownLoadOCIsExist(HCSHANDLE hDownLoad, char *pcUrl, int nTimeout,FileType_E eFileType );
BOOL  CSDownLoadGetFilePathByMaskId(int nMaskID, char *pcFilePath, int nLen);
DWORD CSDownLoadSaveBootPic(HCSHANDLE hDownLoad, char *pcUrl, int nLen);
BOOL  CSDownLoadIsCompleteByFilePath(char *pcFilePath);

DWORD CSAsynCopyDirectory(HCSHANDLE hDownLoad, char *pcSrcUrl, int nSrcLen, char *pcDesUrl, int nDesLen);
DWORD CSAsynLoadMoveDirectory(HCSHANDLE hDownLoad, char *pcSrcUrl, int nSrcLen, char *pcDesUrl, int nDesLen);
DWORD CSAsynLoadDeleteDirectory(HCSHANDLE hDownLoad, char *pcUrl, int nLen);

//NGB相关的异步函数
DWORD CSAsynNgbSaveEnv(HCSHANDLE hDownLoad, const char *pcFileName, int nLen);
DWORD CSAsynNgbRemoveAllEnv(HCSHANDLE hDownLoad, const char *pcFileName, int nLen);
DWORD CSAsynNgbRevertEnv(HCSHANDLE hDownLoad, const char *pcFileName, int nLen);
DWORD CSAsynNgbUninstallStorage(HCSHANDLE hDownLoad, const char *pcFileName, int nLen);

DWORD CSAsynNgbCopyFile(HCSHANDLE hDownLoad, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);
DWORD CSAsynNgbMoveFile(HCSHANDLE hDownLoad, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);
DWORD CSAsynNgbDelFile(HCSHANDLE hDownLoad, char *pcFileName, int nLen);
DWORD CSAsynNgbCopyDir(HCSHANDLE hDownLoad, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);
DWORD CSAsynNgbMoveDir(HCSHANDLE hDownLoad, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);
DWORD CSAsynNgbDelDir(HCSHANDLE hDownLoad, char *pcFileName, int nLen);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* !_CSZIPUTIL_H_ */ 
