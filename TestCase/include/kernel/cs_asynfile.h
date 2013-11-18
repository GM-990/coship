/**@defgroup CS_ZipUtilģ����kernel��zlib��ӿڵķ�װ�㣬�Ա�����ṩ���õĿ����ԡ�
@brief zip/jar�ļ���ȡ֧��ģ��
 
@version 1.0 2010/04/27 Initial Version
@version 1.1 2010/06/02 comment�޸�Ϊ����
@{
*/

#ifndef _CSFILEDOWNLOAD_H_
#define _CSFILEDOWNLOAD_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "udiplus_typedef.h"

#define CSASYN_DIRECTORY "/JSFiledownload"
#define CSDATA_RESTOREFILE   ("AllData")     //NGB �˺궨������ʶ���Ƿ��ǻָ�����������

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
@brief 	SaveBootScreen�Ļص�֪ͨ����
@input:nMsg ��ӦCSAsynFileMessageType_E����Ķ���
@input:dwPara1 
@input:dwPara2 
*/
typedef void (*FOnCSAsynFileCallback)(DWORD dwUserData, int nMsg, DWORD dwPara1, DWORD dwPara2);

/**
@brief 	��ӻص�
@input:hDownLoad  ����CSDownLoadGetHandle�����ľ��
@input:dwUserData
@input:callback 
@return �ɹ�����TRUE,ʧ���򷵻�FALSE
*/
BOOL CSAsynFileAddCallback(HCSHANDLE hAsynFile, DWORD dwUserData, FOnCSAsynFileCallback callback);

/**
@brief 	�����첽�ļ�������
@return �ɹ����ش������,ʧ���򷵻�0
 */
HCSHANDLE CSAsynFileGetHandle();

/**
@brief 	ͨ��HTTP��ʽ�첽�����ļ�
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcUrl
@input:nTimeout 
@input:eFileType
@return 
 */
DWORD CSAsynFileHttpDownLoad(HCSHANDLE hAsynFile, char *pcUrl, int nTimeout,FileType_E eFileType );

/**
@brief 	�ж�urlָ�����ļ��Ƿ����
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcUrl
@input:nTimeout 
@input:eFileType
@return 
@note urlָ�����һ��http�ļ����ӣ��ýӿ����첽��
 */
DWORD CSAsynFileHttpIsExist(HCSHANDLE hAsynFile, char *pcUrl, int nTimeout,FileType_E eFileType );

/**
@brief 	ͨ��OC��ʽ�첽�����ļ�
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcUrl
@input:nTimeout 
@input:eFileType
@return 
 */
DWORD CSAsynFileOCDownLoad(HCSHANDLE hAsynFile, char *pcUrl, int nTimeout,FileType_E eFileType );

/**
@brief 	�ж�urlָ�����ļ��Ƿ����
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcUrl
@input:nTimeout 
@input:eFileType
@return 
@note urlָ�����һ��oc�ļ�����(dvb://)���ýӿ����첽��
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
@brief 	�����ļ���
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcSrcUrl
@input:nSrcLen
@input:pcDesUrl
@input:nDesLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynFileCopyDir(HCSHANDLE hAsynFile, char *pcSrcUrl, int nSrcLen, char *pcDesUrl, int nDesLen);

/**
@brief 	�ƶ��ļ���
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcSrcUrl
@input:nSrcLen
@input:pcDesUrl
@input:nDesLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynFileMoveDir(HCSHANDLE hAsynFile, char *pcSrcUrl, int nSrcLen, char *pcDesUrl, int nDesLen);

/**
@brief 	ɾ���ļ���
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcUrl
@input:nSrcLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynFileDeleteDir(HCSHANDLE hAsynFile, char *pcUrl, int nLen);

/**
@brief 	�ж����·���������Ƿ����
@input:pcFilePath �ļ�·��
@return 
@note  �ýӿ���ͬ����
 */
BOOL CSAsynFileIsCompleteByFilePath(char *pcFilePath);

/**
@brief 	ɾ���ļ���
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcUrl
@input:nSrcLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbSaveEnv(HCSHANDLE hAsynFile, const char *pcFileName, int nLen);

/**
@brief 	ɾ�����еĻ�������
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcFileName
@input:nLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbRemoveAllEnv(HCSHANDLE hAsynFile, const char *pcFileName, int nLen);

/**
@brief 	��ԭ��������
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcFileName
@input:nLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbRevertEnv(HCSHANDLE hAsynFile, const char *pcFileName, int nLen);

/**
@brief 	ж���豸
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcFileName
@input:nLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbUninstallStorage(HCSHANDLE hAsynFile, const char *pcFileName, int nLen);

/**
@brief 	��pcSrcָ�����ļ����Ƶ�pcDes·������ɾ��Դ�ļ���
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcSrc
@input:nSrcLen
@input:pcDes
@input:nDesLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbCopyFile(HCSHANDLE hAsynFile, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);

/**
@brief 	��pcSrcָ�����ļ����Ƶ�pcDes·����ɾ��Դ�ļ�
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcSrc
@input:nSrcLen
@input:pcDes
@input:nDesLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbMoveFile(HCSHANDLE hAsynFile, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);

/**
@brief 	ɾ��һ�������ļ�
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcFileName
@input:nLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbDelFile(HCSHANDLE hAsynFile, char *pcFileName, int nLen);

/**
@brief 	��pcSrcָ����Ŀ¼��Ŀ¼�µ��������ݸ��Ƶ�pcDes·������ɾ��ԴĿ¼��������
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcSrc
@input:nSrcLen
@input:pcDes
@input:nDesLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbCopyDir(HCSHANDLE hAsynFile, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);

/**
@brief 	��pcSrcָ����Ŀ¼��Ŀ¼�µ��������ݸ��Ƶ�pcDes·������ɾ��ԴĿ¼��������
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcSrc
@input:nSrcLen
@input:pcDes
@input:nDesLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbMoveDir(HCSHANDLE hAsynFile, char *pcSrc, int nSrcLen, char *pcDes, int nDesLen);

/**
@brief 	ɾ��һ��Ŀ¼������������
@input:hAsynFile  ����CSAsynFileGetHandle�����ľ��
@input:pcFileName
@input:nLen
@return 
@note  �ýӿ����첽��
 */
DWORD CSAsynNgbDelDir(HCSHANDLE hAsynFile, char *pcFileName, int nLen);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif /* !_CSZIPUTIL_H_ */ 
