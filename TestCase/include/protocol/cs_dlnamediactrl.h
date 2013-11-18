#ifndef _CS_DLNA_MEDIACTRL_H_
#define _CS_DLNA_MEDIACTRL_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

#define MAX_NAME_LEN                          256
#define MAX_UUID_LEN                            64
#define MAX_FILEID_LEN                            256
#define MAX_FILE_LEN                              256
#define MAX_FILE_PATH 	                     256
#define MAX_FILEMETADATA_LEN        256
#define MAX_URL_LEN                                1024

/**@brief  设备信息结构体 */
typedef struct 
{
    char acName[MAX_NAME_LEN];                      ///<设备名ASCII码
    char acUUID[MAX_UUID_LEN];                          ///<设备ID  设备唯一标识ASCII码
}CSDLNADeviceInfo_S;

/**@brief  服务类型 */
typedef enum
{
    EM_DLNA_MEDIASERVER = 0,                        ///<MediaServer类型
    EM_DLNA_MEDIARENDER                               ///<MediaRender类型
}CSDLNADeviceType_E;

/**@brief  回调消息类型 */
typedef enum
{
    EM_DLNA_MEDIASERVER_CHANGE = 0,              ///<Media类型服务有变化
    EM_DLNA_MEDIARENDER_CHANGE                     ///<Render类型服务有变化
}CSDLNAMsgType_E;

/**@brief 本结构用于存储和传输文件目录信息.包括文件id，文件类型，文件名称和文件路径*/
typedef struct 
{
    BOOL	bIsDir;										///<文件类型 TRUE表示文件夹，FALSE表示文件
    //int 	nFileId;									///<文件唯一标示符，根文件的fileId为0
    char  	acFileId[MAX_FILEID_LEN];
    int         nFileType;                                                                              ///<文件类型0:视频文件；1:音频文件；2:图片文件；3:其他
    char 	acFileName[MAX_FILE_LEN];					///<文件名称, UTF-8编码
    char	acFileMetadata[MAX_FILEMETADATA_LEN];		///<文件元数据信息
    char	acFilePath[MAX_FILE_PATH];					///<文件所在路径
    char acFileUrl[MAX_URL_LEN];

}CSDLNAFileInfo_S;

typedef struct
{
    int nFiletype;
    char acFileName[MAX_FILE_LEN];
    char acFileUrl[MAX_URL_LEN];
}CSDLNAMetaData_S;

/**
@brief 创建一个Controller
- 成功返回Controller句柄。
- 失败返回NULL。
*/
HCSHANDLE CSDLNAMediaCtrlCreate();

/**
@brief 销毁一个Controller
@param[in] hController Controller的句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaCtrlDestroy(HCSHANDLE hController);

/**
@brief 启动Controller
@param[in] hController Controller句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaCtrlStart(HCSHANDLE hController);

/**
@brief 停止Controller
@param[in] hController Controller句柄
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaCtrlStop(HCSHANDLE hController);

/**
@brief    回调函数，当有新的设备加入，或者有设备停止时，会触发该回调信息，
用户收到该回调消息后调用CSDLNAMediaCtrlGetDeviceCount和CSDLNAMediaCtrlGetDeviceInfo来获取设备信息。
@param[in] hController  Controller句柄
@param[in] eType 消息类型
@param[in] pvPara 消息值
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 无。
*/
typedef int (*CSOnDLNACtrlDeviceChange_F)(HCSHANDLE hController, CSDLNAMsgType_E eType, void * pvPara,  void * pvUserData);

/**
@brief	 注册通知回调函数。
@param[in] fnCb 回调处理函数
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaCtrlAddCallback(CSOnDLNACtrlDeviceChange_F  fnCb, void* pvUserData);

/**
@brief	删除回调函数。
@param[in] fnCb 回调处理函数
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaCtrlRemoveCallback(CSOnDLNACtrlDeviceChange_F  fnCb, void* pvUserData);

/**
@brief 获取当前网络某一类型设备总数
@param[in] hController 当前Controller句柄
@param[out] eType 设备类型
@return 
- 成功返回设备总数。
- 失败返回-1。
*/
int CSDLNAMediaCtrlGetDeviceCount(HCSHANDLE hController, CSDLNADeviceType_E eType);

/**
@brief 根据索引获取设备信息,索引值要小于CSDLNACtrlGetDeviceCount的返回值
@param[in] hController 当前Controller句柄
@param[in] eType 设备类型
@param[in] nIndex 设备索引号
@param[out] psRendererInfo 返回所有的Renderer信息
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
int CSDLNAMediaCtrlGetDeviceInfo(HCSHANDLE hController, CSDLNADeviceType_E eType, int nIndex, CSDLNADeviceInfo_S *psDevInfo);

/**
@brief 该接口获取某一级文件夹目录下文件个数
@param[in] pcUUID：设备uuid,为ASCII字符串,是设备的唯一标识符
@param[in] nFileId：文件id号，为0表示取根目录下所有文件
@return 
- 成功返回某一级文件夹目录下文件个数。
- 失败返回-1。
*/
//int CSDLNAMediaCtrlGetFileCount(char*pcUUID, int nFileId);
int CSDLNAMediaCtrlGetFileCount(char*pcUUID, char  *pcFileId);

/**
@brief 该接口获取某一级文件夹目录下文件信息
@param[in] pcUUID：设备uuid,为ASCII字符串
@param[in] nFileId：文件id号，为0表示取根目录下所有文件，遍历子文件时传递上次所获取的文件的nFileId,即结构体CSDLNAItem_S中的nFiledId
@param[in] nFileIndex 为从0到count的索引值
@param[in] psFileInfo：用来存储所获当前文件夹下索引为nFileIndex 的文件信息
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
//int CSDLNAMediaCtrlGetFile(char*pcUUID, int nFileId, int nFileIndex, CSDLNAFileInfo_S *psFileInfo);
int CSDLNAMediaCtrlGetFile(char*pcUUID, char  *pcFileId, int nFileIndex, CSDLNAFileInfo_S *psFileInfo);

/**
@brief 该接口获取指定文件ID的文件URL
@param[in] pcUUID：设备uuid,为ASCII字符串
@param[in] nFileId：文件id号，根据CSDLNAMediaCtrlGetFile来获取到的FileId
@param[out] pcFileURL：文件所在的URL
@param[in] nLength:pcFileURL长度
@return 
- 成功返回SUCCESS。
- 失败返回FAILURE。
*/
//int CSDLNAMediaCtrlGetFileURL(char*pcUUID, int nFileId, char *pcFileURL, int nLength);
int CSDLNAMediaCtrlGetFileURL(char*pcUUID, char * pcFileId, char *pcFileURL, int nLength);

#ifdef __cplusplus
}
#endif

#endif

