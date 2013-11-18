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

#define CSASYN_DIRECTORY "/JSFiledownload"
#define CSDATA_RESTOREFILE   ("AllData")     //NGB 此宏定义用以识别是否是恢复到出厂设置

#define ASYNFILE_MSG_START 10150

typedef enum CSAsynFileMessageType_E
{
	EM_ASYNFILE_MSG_START=ASYNFILE_MSG_START,
    EM_ASYNFILE_MSG_OK,                  /**/
	EM_ASYNFILE_MSG_NOJOB,
	EM_ASYNFILE_MSG_FAIL,
	EM_ASYNFILE_MSG_TIMEOUT,
	EM_ASYNFILE_ISEXIST_FILE_OK,
	EM_ASYNFILE_ISEXIST_FILE_NOJOB,
	EM_ASYNFILE_ISEXIST_FILE_TIMEOUT,
	EM_ASYNFILE_COPY_DIRECTORY_SUCORFAIL,
	EM_ASYNFILE_MOVE_DIRECTORY_SUCORFAIL,
	EM_ASYNFILE_DEL_DIRECTORY_SUCORFAIL,	
	EM_ASYNFILE_MSG_SAVE_DATA,
	EM_ASYNFILE_MSG_REMOVE_DATA,
	EM_ASYNFILE_MSG_REVERT_DATA,
	EM_ASYNFILE_MSG_RESTORE_TO_DEFAULT,
	EM_ASYNFILE_MSG_DEVICE_UNINSTALL,
	EM_ASYNFILE_COPY_FILE_SUCORFAIL,
	EM_ASYNFILE_MOVE_FILE_SUCORFAIL,
	EM_ASYNFILE_DEL_FILE_SUCORFAIL	
}CSAsynFileMessageType_E;

typedef enum FileType_E
{
	INVALID_REMOTE_FILE,
	EXIST_REMOTE_OC_FILE = 1,
	DOWNLOAD_REMOTE_OC_FILE,
	EXSIT_REMOTE_HTTP_FILE,
	DOWNLOAD_REMOTE_HTTP_FILE
}FileType_E;

/**
@brief 	SaveBootScreen的回调通知函数
@input:nMsg 对应CSAsynFileMessageType_E里面的定义
@input:dwPara1 
@input:dwPara2 
*/
typedef void (*FOnCSAsynFileCallback)(DWORD dwUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);

/**
@brief 	添加回调
@input:hDownLoad  调用CSDownLoadGetHandle创建的句柄
@input:dwUserData
@input:callback 
@return 成功返回TRUE,失败则返回FALSE
*/
BOOL CSAsynFileAddCallback(HCSHANDLE hAsynFile, DWORD dwUserData, FOnCSAsynFileCallback callback);

/**
@brief 	创建异步文件处理句柄
@return 成功返回创建句柄,失败则返回0
 */
HCSHANDLE CSAsynFileGetHandle();

/**
@brief 	通过HTTP方式异步下载文件
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcUrl
@input:nTimeout 
@input:eFileType
@return 
 */
DWORD CSAsynFileHttpDownLoad(HCSHANDLE hAsynFile, char *pcUrl, int nTimeout,FileType_E eFileType );

/**
@brief 	判断url指定的文件是否存在
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcUrl
@input:nTimeout 
@input:eFileType
@return 
@note url指向的是一个http文件链接；该接口是异步的
 */
DWORD CSAsynFileHttpIsExist(HCSHANDLE hAsynFile, char *pcUrl, int nTimeout,FileType_E eFileType );

/**
@brief 	通过OC方式异步下载文件
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcUrl
@input:nTimeout 
@input:eFileType
@return 
 */
DWORD CSAsynFileOCDownLoad(HCSHANDLE hAsynFile, char *pcUrl, int nTimeout,FileType_E eFileType );

/**
@brief 	判断url指定的文件是否存在
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcUrl
@input:nTimeout 
@input:eFileType
@return 
@note url指向的是一个oc文件链接(dvb://)；该接口是异步的
 */
DWORD CSAsynFileOCIsExist(HCSHANDLE hAsynFile, char *pcUrl, int nTimeout,FileType_E eFileType );

/**
@brief 	
@input:nMaskID
@input:pcFilePath 
@input:nLen
@return 
 */
BOOL  CSAsynFileGetFilePathByMaskId(int nMaskID, char *pcFilePath, int nLen);

/**
@brief 	拷贝文件夹
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcSrcUrl
@input:nSrcLen
@input:pcDesUrl
@input:nDesLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynFileCopyDir(HCSHANDLE hAsynFile, char *pcSrcUrl, int nSrcLen, char *pcDesUrl, int nDesLen);

/**
@brief 	移动文件夹
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcSrcUrl
@input:nSrcLen
@input:pcDesUrl
@input:nDesLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynFileMoveDir(HCSHANDLE hAsynFile, char *pcSrcUrl, int nSrcLen, char *pcDesUrl, int nDesLen);

/**
@brief 	删除文件夹
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcUrl
@input:nSrcLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynFileDeleteDir(HCSHANDLE hAsynFile, char *pcUrl, int nLen);

/**
@brief 	判断这个路径的下载是否完成
@input:pcFilePath 文件路径
@return 
@note  该接口是同步的
 */
BOOL CSAsynFileIsCompleteByFilePath(char *pcFilePath);

/**
@brief 	删除文件夹
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcUrl
@input:nSrcLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbSaveEnv(HCSHANDLE hAsynFile, const char *pcFileName, int nLen);

/**
@brief 	删除所有的环境变量
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcFileName
@input:nLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbRemoveAllEnv(HCSHANDLE hAsynFile, const char *pcFileName, int nLen);

/**
@brief 	还原环境变量
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcFileName
@input:nLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbRevertEnv(HCSHANDLE hAsynFile, const char *pcFileName, int nLen);

/**
@brief 	卸载设备
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcFileName
@input:nLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbUninstallStorage(HCSHANDLE hAsynFile, const char *pcFileName, int nLen);

/**
@brief 	将pcSrc指定的文件复制到pcDes路径，不删除源文件。
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcSrc
@input:nSrcLen
@input:pcDes
@input:nDesLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbCopyFile(HCSHANDLE hAsynFile, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);

/**
@brief 	将pcSrc指定的文件复制到pcDes路径，删除源文件
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcSrc
@input:nSrcLen
@input:pcDes
@input:nDesLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbMoveFile(HCSHANDLE hAsynFile, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);

/**
@brief 	删除一个本地文件
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcFileName
@input:nLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbDelFile(HCSHANDLE hAsynFile, char *pcFileName, int nLen);

/**
@brief 	将pcSrc指定的目录及目录下的所有内容复制到pcDes路径，不删除源目录及其内容
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcSrc
@input:nSrcLen
@input:pcDes
@input:nDesLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbCopyDir(HCSHANDLE hAsynFile, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);

/**
@brief 	将pcSrc指定的目录及目录下的所有内容复制到pcDes路径，且删除源目录及其内容
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcSrc
@input:nSrcLen
@input:pcDes
@input:nDesLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbMoveDir(HCSHANDLE hAsynFile, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);

/**
@brief 	删除一个目录及其所有内容
@input:hAsynFile  调用CSAsynFileGetHandle创建的句柄
@input:pcFileName
@input:nLen
@return 
@note  该接口是异步的
 */
DWORD CSAsynNgbDelDir(HCSHANDLE hAsynFile, char *pcFileName, int nLen);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* !_CSZIPUTIL_H_ */ 
