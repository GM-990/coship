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

/**@brief  �豸��Ϣ�ṹ�� */
typedef struct 
{
    char acName[MAX_NAME_LEN];                      ///<�豸��ASCII��
    char acUUID[MAX_UUID_LEN];                          ///<�豸ID  �豸Ψһ��ʶASCII��
}CSDLNADeviceInfo_S;

/**@brief  �������� */
typedef enum
{
    EM_DLNA_MEDIASERVER = 0,                        ///<MediaServer����
    EM_DLNA_MEDIARENDER                               ///<MediaRender����
}CSDLNADeviceType_E;

/**@brief  �ص���Ϣ���� */
typedef enum
{
    EM_DLNA_MEDIASERVER_CHANGE = 0,              ///<Media���ͷ����б仯
    EM_DLNA_MEDIARENDER_CHANGE                     ///<Render���ͷ����б仯
}CSDLNAMsgType_E;

/**@brief ���ṹ���ڴ洢�ʹ����ļ�Ŀ¼��Ϣ.�����ļ�id���ļ����ͣ��ļ����ƺ��ļ�·��*/
typedef struct 
{
    BOOL	bIsDir;										///<�ļ����� TRUE��ʾ�ļ��У�FALSE��ʾ�ļ�
    //int 	nFileId;									///<�ļ�Ψһ��ʾ�������ļ���fileIdΪ0
    char  	acFileId[MAX_FILEID_LEN];
    int         nFileType;                                                                              ///<�ļ�����0:��Ƶ�ļ���1:��Ƶ�ļ���2:ͼƬ�ļ���3:����
    char 	acFileName[MAX_FILE_LEN];					///<�ļ�����, UTF-8����
    char	acFileMetadata[MAX_FILEMETADATA_LEN];		///<�ļ�Ԫ������Ϣ
    char	acFilePath[MAX_FILE_PATH];					///<�ļ�����·��
    char acFileUrl[MAX_URL_LEN];

}CSDLNAFileInfo_S;

typedef struct
{
    int nFiletype;
    char acFileName[MAX_FILE_LEN];
    char acFileUrl[MAX_URL_LEN];
}CSDLNAMetaData_S;

/**
@brief ����һ��Controller
- �ɹ�����Controller�����
- ʧ�ܷ���NULL��
*/
HCSHANDLE CSDLNAMediaCtrlCreate();

/**
@brief ����һ��Controller
@param[in] hController Controller�ľ��
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaCtrlDestroy(HCSHANDLE hController);

/**
@brief ����Controller
@param[in] hController Controller���
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaCtrlStart(HCSHANDLE hController);

/**
@brief ֹͣController
@param[in] hController Controller���
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaCtrlStop(HCSHANDLE hController);

/**
@brief    �ص������������µ��豸���룬�������豸ֹͣʱ���ᴥ���ûص���Ϣ��
�û��յ��ûص���Ϣ�����CSDLNAMediaCtrlGetDeviceCount��CSDLNAMediaCtrlGetDeviceInfo����ȡ�豸��Ϣ��
@param[in] hController  Controller���
@param[in] eType ��Ϣ����
@param[in] pvPara ��Ϣֵ
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return �ޡ�
*/
typedef int (*CSOnDLNACtrlDeviceChange_F)(HCSHANDLE hController, CSDLNAMsgType_E eType, void * pvPara,  void * pvUserData);

/**
@brief	 ע��֪ͨ�ص�������
@param[in] fnCb �ص�������
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaCtrlAddCallback(CSOnDLNACtrlDeviceChange_F  fnCb, void* pvUserData);

/**
@brief	ɾ���ص�������
@param[in] fnCb �ص�������
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaCtrlRemoveCallback(CSOnDLNACtrlDeviceChange_F  fnCb, void* pvUserData);

/**
@brief ��ȡ��ǰ����ĳһ�����豸����
@param[in] hController ��ǰController���
@param[out] eType �豸����
@return 
- �ɹ������豸������
- ʧ�ܷ���-1��
*/
int CSDLNAMediaCtrlGetDeviceCount(HCSHANDLE hController, CSDLNADeviceType_E eType);

/**
@brief ����������ȡ�豸��Ϣ,����ֵҪС��CSDLNACtrlGetDeviceCount�ķ���ֵ
@param[in] hController ��ǰController���
@param[in] eType �豸����
@param[in] nIndex �豸������
@param[out] psRendererInfo �������е�Renderer��Ϣ
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
int CSDLNAMediaCtrlGetDeviceInfo(HCSHANDLE hController, CSDLNADeviceType_E eType, int nIndex, CSDLNADeviceInfo_S *psDevInfo);

/**
@brief �ýӿڻ�ȡĳһ���ļ���Ŀ¼���ļ�����
@param[in] pcUUID���豸uuid,ΪASCII�ַ���,���豸��Ψһ��ʶ��
@param[in] nFileId���ļ�id�ţ�Ϊ0��ʾȡ��Ŀ¼�������ļ�
@return 
- �ɹ�����ĳһ���ļ���Ŀ¼���ļ�������
- ʧ�ܷ���-1��
*/
//int CSDLNAMediaCtrlGetFileCount(char*pcUUID, int nFileId);
int CSDLNAMediaCtrlGetFileCount(char*pcUUID, char  *pcFileId);

/**
@brief �ýӿڻ�ȡĳһ���ļ���Ŀ¼���ļ���Ϣ
@param[in] pcUUID���豸uuid,ΪASCII�ַ���
@param[in] nFileId���ļ�id�ţ�Ϊ0��ʾȡ��Ŀ¼�������ļ����������ļ�ʱ�����ϴ�����ȡ���ļ���nFileId,���ṹ��CSDLNAItem_S�е�nFiledId
@param[in] nFileIndex Ϊ��0��count������ֵ
@param[in] psFileInfo�������洢����ǰ�ļ���������ΪnFileIndex ���ļ���Ϣ
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
//int CSDLNAMediaCtrlGetFile(char*pcUUID, int nFileId, int nFileIndex, CSDLNAFileInfo_S *psFileInfo);
int CSDLNAMediaCtrlGetFile(char*pcUUID, char  *pcFileId, int nFileIndex, CSDLNAFileInfo_S *psFileInfo);

/**
@brief �ýӿڻ�ȡָ���ļ�ID���ļ�URL
@param[in] pcUUID���豸uuid,ΪASCII�ַ���
@param[in] nFileId���ļ�id�ţ�����CSDLNAMediaCtrlGetFile����ȡ����FileId
@param[out] pcFileURL���ļ����ڵ�URL
@param[in] nLength:pcFileURL����
@return 
- �ɹ�����SUCCESS��
- ʧ�ܷ���FAILURE��
*/
//int CSDLNAMediaCtrlGetFileURL(char*pcUUID, int nFileId, char *pcFileURL, int nLength);
int CSDLNAMediaCtrlGetFileURL(char*pcUUID, char * pcFileId, char *pcFileURL, int nLength);

#ifdef __cplusplus
}
#endif

#endif

