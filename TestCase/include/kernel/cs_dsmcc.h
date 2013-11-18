#ifndef _CS_DSMCC_H_
#define _CS_DSMCC_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "mmcp_os.h"

/*BIOP�������Ͷ���*/
#define CSDSM_OBJ_TYPE_UNKOWN 			0
#define CSDSM_OBJ_TYPE_SERVICE_GATEWAY 	1
#define CSDSM_OBJ_TYPE_DIRECTORY 			2
#define CSDSM_OBJ_TYPE_FILE 				3
#define CSDSM_OBJ_TYPE_STREAM 				4
#define CSDSM_OBJ_TYPE_STREAM_EVENT 		5

#define CSDSM_OBJ_LOCATION_NATIVE 			0
#define CSDSM_OBJ_LOCATION_FOREIGN 		1


#define CSDSM_SEEK_SET	0	/* Seek from beginning of file.  */
#define CSDSM_SEEK_CUR 	1	/* Seek from current position.  */
#define CSDSM_SEEK_END  	2 	/* Set file pointer to EOF plus "offset" */


/********** enum **********/
/*��������*/
typedef enum _CSDSM_ErrorCode_E{
	CSDSM_ERROR_NO_ERROR = 				  0,			//û�д���
	CSDSM_ERROR_INVALID_PARAM = 			-1,			//��������
	CSDSM_ERROR_NO_SUCH_OBJ = 			-2,			//���󲻴���
	CSDSM_ERROR_OBJ_NOT_LOADED = 		-3,			//����δ����
	CSDSM_ERROR_OBJ_LOADING =			-4,			//������������
	CSDSM_ERROR_OBJ_LOCATION_FOREIGN = 	-5,			//��������һ���ֲ���
	CSDSM_ERROR_ISNOT_STREAM_EVENT = 	-7,			//�����¼�
	CSDSM_ERROR_NO_FREE_GATEWAY = 		-8,			//û�п��е�sg
	CSDSM_ERROR_NO_MEMORY = 				-9,			//�ڴ治��
	CSDSM_ERROR_NO_SUCH_EVENT = 			-10,			//û�и��¼�
	CSDSM_ERROR_MUTEX_ERROR = 			-11,			//�ź�������ʧ��
	CSDSM_ERROR_TIMER_ERROR = 			-12,			//��ʱ������
	CSDSM_ERROR_EVENT_ERROR = 			-13,		//�����¼�����
	CSDSM_ERROR_OBJ_NOT_RECEIVED = 		-15,			//δ�յ��ö������Ϣ
	CSDSM_ERROR_OBJ_CHANGED	 =	 		-16			//δ�յ��ö������Ϣ	
}CSDSM_ErrorCode_E;

/*�ص���������Ϣ����*/
typedef enum _CSDSMCallbackType_E
{
	EM_DSM_OBJECT_VERSION_CHANGED = 	0,
	EM_DSM_STREAM_EVENT_ARRIVIED =	1,
	EM_DSM_ATTACH_SUCCESS = 			2,/* pucPrivateData = NULL*/
	EM_DSM_ATTACH_FAILED = 			3,/* pucPrivateData = NULL*/
	EM_DSM_OBJECT_LOAD_SUCCESS = 		4,
	EM_DSM_OBJECT_LOAD_FAILED = 		5
}CSDSMCallbackType_E;

/*�û�������������*/
typedef enum _CSDSMLoadType_E
{
	EM_DSM_NOT_PRELOAD = 				0, /*��Ԥȡ����*/
	EM_DSM_PRELOAD_WITH_CANCEL = 	1,/*Ԥȡ���󣬿ɱ�����ȡ��*/
	EM_DSM_PRELOAD_NO_CANCEL = 		2 /*Ԥȡ���󣬲��ܱ�ȡ��*/	
}CSDSMLoadType_E;
//����������
typedef enum _CSDSMUpdateType_E
{
	EM_DSM_MONITOR_UPDATE =			0,/*ʵʱ������*/
	EM_DSM_IGNORE_UPDATE =			1/*���ȴӻ����ȡ���ݣ����Ը���*/
}CSDSMUpdateType_E;

typedef enum _CSDSMConfigParamType_E
{
	EM_DSM_NOT_FREE_MAIN_BUF = 0,/*������DDB���ͷ�����õ�1.1M�ڴ棬Ĭ�ϲ��ͷ�����1.1M�ڴ�*/
	EM_DSM_FREE_MAIN_BUF	         = 1,/*������DDB���ͷ�����õ�1.1M�ڴ�*/
	EM_DSM_REQUEST_INTERVAL   = 2, /*���÷��ͽ���Ŀ¼�������ʱ������Ĭ��Ϊ10ms*/
	EM_DSM_MEMORY_VECTOR       =3,/*ʹ���ڴ�������ʽ�����ڴ�*/
	EM_DSM_MEMORY_MALLOC       =4,/*ʹ��MMCP_MemMalloc()��ʽ�����ڴ�,Ĭ�Ϸ�ʽ*/
	EM_DSM_UPDATE_FILE_COUNT =5,/*���ü�⵽���º�����ص��ļ�����*/
	EM_DSM_DII_REPEAT_COUNT    =6,/*�ж�DII�Ƿ�������ظ�����*/
	EM_DSM_FILENAME_STRCASE     =7,/*�������ļ����Ĵ�Сд��Ĭ�ϲ�����*/
	EM_DSM_FILENAME_NOT_STRCASE     =8/*�����ļ����Ĵ�Сд��Ĭ�ϲ�����*/
}CSDSMConfigParamType_E;

/********** struct **********/
/*����ʶ��*/
typedef struct  _Stream_Taps {
    WORD	m_wId;/*reserved for future*/
    WORD 	m_wUse;/*0x0B-NPT,0x0C-str&ste des,0x0D-ste des,0x0E-str des,0x18-ES stream,0x19-BIOP_PROGRAM_USE*/
    WORD 	m_wAssociationTag;
    BYTE 	m_ucSelectorLen;/*reserved for future*/
} CSDSMStreamTaps_t;

/*������*/
typedef struct _Stream_object{
    DWORD 			m_dwDurationS;/*stream duration seconds*/
    DWORD 			m_dwDurationMS;/*stream duration microseconds*/
    BYTE 				m_ucAudio;/*if stream is audio then m_ucAudio!=0*/
    BYTE 				m_ucVideo;/*if stream is video then m_ucVideo!=0*/
    BYTE 				m_ucData;/*others m_ucData!=0*/
    BYTE 				m_ucTapsCount;/*the count of taps associated with this stream*/
    CSDSMStreamTaps_t *m_pStreamTapList;/*this memory size is m_ucTapsCount*sizeof(CSDSMStreamTaps_t)*/
} CSDSMStreamObject_t;

/*���¼�����*/
typedef struct _Streamevent_object {
    CSDSMStreamObject_t m_StreamObject;
    DWORD 			 m_dwEventListBytes;/*memory size of m_pucEventList*/
    BYTE *				 m_pucEventList;/*eventNames_count-2 bytes,following eventName_lenght-1 bytes,eventName_data-eventName_lenght bytes,......*/
    BYTE 				 m_ucEventIdCount;/*this field indentifies the size of m_pwEventIdArray*/
    WORD *			 m_pwEventIdArray;/*this memory size is m_ucEventIdCount*sizeof(WORD)*/
} CSDSMStreamEventObject_t;

/*����ҵ������*/
typedef struct _Nsap 
{
    BYTE 		m_ucNameCount;
    char *		m_pcPathName;
    DWORD	m_dwCrsId;
    WORD 		m_wTsId;
    WORD 		m_wOrgNetId;
    WORD 		m_wServiceId;
} CSDSMNsap_t;

/*����ṹ*/
typedef struct _CSDSMOcObject_S {
    BYTE 			m_ucObjType;		       /*object type: file/dir/str/ste/srg,�μ�BIOP�������Ͷ���*/
    BYTE 			m_ucObjLocation;		/*native/foreign/other*/
    DWORD 		m_ucObjKey;		/*own objkey*/
    DWORD 		m_ucParentObjKey;		/*parent objkey*/
    WORD 			m_wModuleId;		/*own module_id*/
    WORD 			m_wParentModuleId;		/*parent moduleid*/
    char 			*m_pcObjName;			/*object name*/
    char 			*m_pcObjContent;			/*object content pointer*/
    DWORD 		m_dwContentSize;			/*size of object content*/
    void 			*m_pStreamData;			/*only for stream and stream event           */
    WORD 			m_wDataPid; 		/*ES pid*/
    DWORD  		m_dwTransactionId;	/* it is also the group id of this directory */
    CSUDIPLUS_TIME_T m_tAccessTime;		/*accesstime;*/
    BYTE 			m_ucVersion;
    CSDSMNsap_t* 	m_pNsap;
} CSDSMOcObject_t;

typedef struct _CSDSMCarouselInfo_S
{
	WORD 	m_wOrgNetId;/*ԭʼ����ID*/
	WORD	m_wTsId;/*������ID*/
	WORD 	m_wServiceId;/*��Ŀ��*/
	BYTE   	m_ucComponentTag;/*��m_wCrsEsPid��Ӧ��ComponentTag*/
	DWORD  	m_dwTransactionId;/*����ID,used for session integrity and error processing*/
	DWORD  	m_dwTimeoutDsi;/*DSI��Ϣװ�زο���ʱ��0Ϊ�޲ο�*/
	DWORD  	m_dwTimeoutDii;/*DII��Ϣװ�زο���ʱ��0Ϊ�޲ο�*/
	WORD  	m_wCrsEsPid;/*DSI��Ϣ���ڵĻ�������Pid*/
	WORD  	m_wBlockSize;/*the size in bytes of every block in the module 
	                                      (except for the last block which may bethe same or smaller). 
	                                      This is equivalent to blockSize in the DII.*/
	BYTE  	m_ucTypeId;/*equals 2, indicates two layers carousel*/
	DWORD 	m_dwCrsId;/*identifies the object carousel 
						with the corresponding carouselId, with the 
						carousel_identifier_descriptor carrying the 
						broadcaster's OrgID in the most significant 24 msbs 
						if carousel is sharable across multiple transport streams
						or between 0 - 255 otherwise*/
	BYTE 	m_ucModuleVersion;/*the version number of the module containing
								the service gateway. This is equivalent to
								moduleVersion in the DII.*/
	WORD 	m_wModuleId;/*identifier of the module in the carousel. 
						This is equivalent to moduleId in the DII*/
	WORD 	m_wModuleSize;/*the size of the module in bytes.
							This is equivalent to moduleSize in the DII*/
	BYTE     	m_ucIsCompressed;/*If Compressed or not*/
	DWORD 	m_dwOriginalSize;/*the size of the data (in bytes) carried by the 
								module before it was compressed. It is
								equivalent to original_size carried in the 
								compressed_module_descriptor in the DII*/
	DWORD 	m_dwObjectKey;/**/
}CSDSMCarouselInfo_t;

/*���ص���Ϣ����ΪEM_DSM_OBJECT_VERSION_CHANGED, EM_DSM_OBJECT_LOAD_SUCCESS,
EM_DSM_OBJECT_LOAD_FAILED ʱ����Ϣ���ݽṹ��*/
typedef struct _CSDSMObjectMsgInfo_S
{
	int nValue;
	char *pcObjAbsolutePath;
}CSDSMObjectMsgInfo_t;

/*���ص���Ϣ����ΪEM_DSM_STREAM_EVENT_ARRIVIEDʱ����Ϣ���ݽṹ��*/
typedef struct _DSMStreamEvent_S
{
	DWORD	dwNptSecs;
	DWORD 	dwNptMicroSecs;
	int 		nEventId;
	int 		nPrvDataLen;
	BYTE * 	pucPrivateData;
}DSMStreamEvent_t;

/*֪ͨ�û�����*/
typedef int (*CSDSMCallback) (
 HCSHANDLE 			hCrsHandle,
 CSDSMCallbackType_E 	eMsg, /*ֵΪenum���� */
 void * 				pUserData,/*������û�����*/
 void *				pMsgData/*����nMsg��ֵת������Ӧ�����ݽṹ*/
);/* �û�����Ļص�����*/

/* Service Gateway APIs */

/**
* ��ʼ��DSMģ��
* dwMaxCacheSizeΪ�����OC���ڴ��С����λΪKBYTE
* �ɹ�����0��ʧ�ܷ���-1
*/
CSDSM_ErrorCode_E CSDSMInit(DWORD dwMaxCacheSize);

/**
* ��һ���ֲ��������ļ�ϵͳ
@param [in]
@param [in] 
@param [out] pHandle �ֲ����ļ�ϵͳ�ɹ���������ֲ����
\retval NULL
* �ɹ�����0��ʧ�ܷ���-1
*/
CSDSM_ErrorCode_E CSDSMAttachGateway(DWORD dwDeviceId, char *pcMoutedDirName, CSDSMCarouselInfo_t * pCrsInfo, BOOL bAuth, int nCheckPeriod, /*out*/HCSHANDLE * pCrsHandle);
/**
* �洢���ʿ�ѡ���CSDSMAttachGateway
@param [in] szStoragePath �ֲ�����ʵ�ʵĴ洢���ļ�ϵͳ�е�λ�ã�NULL��ʾ�洢���ڴ���
*/
CSDSM_ErrorCode_E CSDSMAttachGatewayEx(DWORD dwDeviceId, char *pcMoutedDirName,char * szStoragePath, CSDSMCarouselInfo_t * pCrsInfo, BOOL bAuth, int nCheckPeriod, HCSHANDLE * pCrsHandle);
//�Ͽ�һ���ֲ����ļ�ϵͳ֮��Ĺ���
CSDSM_ErrorCode_E CSDSMDetachGateway (HCSHANDLE hCrsHandle, BOOL bClearCache);

//��һ���ֲ��в���һ������������ҳɹ�����õ��ö������Ϣ
CSDSM_ErrorCode_E CSDSMFindFileObject(char *pcDirName, char *pcFileName,  /*out*/CSDSMOcObject_t ** ppFileObject);

//����һ������
CSDSM_ErrorCode_E CSDSMLoadObject(char *pcDirName, char *pcFileName,/*out*/CSDSMOcObject_t ** ppLiteOptionsObject);

//Ԥ����һ������
//eLoadTypeΪԤ��������
CSDSM_ErrorCode_E CSDSMPreLoadObject(char *pcDirName, char *pcFileName,/*out*/CSDSMOcObject_t ** ppLiteOptionsObject, CSDSMLoadType_E eLoadType);

//ȡ�����ض���
CSDSM_ErrorCode_E CSDSMAbortGatewayAttach(HCSHANDLE hCrsHandle);

//ж��һ������
CSDSM_ErrorCode_E CSDSMUnloadObject(char *pcDirName, char *pcFileName);

//ע��ص�����������callbackeId��������һ������ɹ������¼����ﶼ��ص�֪ͨ
DWORD CSDSMRegisterCallback( CSDSMCallbackType_E  eMsg,CSDSMCallback fnCallback,void* pUserData);

//ע���ص�����
CSDSM_ErrorCode_E CSDSMUnregisterCallback(/*in*/ DWORD dwCallbackId);

//����һ������
CSDSM_ErrorCode_E CSDSMAddMonitorObject(char *pcDirName, char *pcFileName,  void *pUserData);

//ȡ����������
CSDSM_ErrorCode_E CSDSMDelMonitorObject(char *pcDirName, char *pcFileName);

//���һ�������Ƿ��Ѿ�����
CSDSM_ErrorCode_E CSDSMIsObjectExist(WORD wOrgNetId, WORD wTsId, WORD wServiceId, WORD wComponentTag, char *pcFilePath);

//��ȡĳ���ֲ�������ļ�ϵͳ�Ĺ��ص㣬�������ֲ��ĸ�Ŀ¼
CSDSM_ErrorCode_E CSDSMGetMountPointByHandle(/*in*/HCSHANDLE hCrsHandle, /*out*/char **ppcMountPoint);

//���ݹ��ص��ȡ��Ӧ�ֲ��ľ�������ָ���Ĺ��ص㲻���ڣ�����NULL
CSDSM_ErrorCode_E CSDSMGetHandleByMountPoint(char *pcMountPoint,HCSHANDLE*pHandle);

//������Ҫ�ػ�ȡ��Ӧ�ֲ��ľ�����������Ҫ��ָ����ֲ������ڻ���δ���ļ�ϵͳ����������NULL
CSDSM_ErrorCode_E CSDSMGetHandleByFactors(WORD wOnId,WORD wTsId, WORD wServiceId,WORD wComTag,HCSHANDLE*pHandle);

//�������¼��ĵ���
CSDSM_ErrorCode_E CSDSMAddMonitorEvent(char *pcDirName, char *pcFileName, int nEventId, void *pUserData);

//ȡ���������¼��ĵ���
CSDSM_ErrorCode_E CSDSMDelMonitorEvent(char *pcDirName, char *pcFileName, int nEventId);

//��ȡassociation_tag������������Ҫ��
CSDSM_ErrorCode_E CSDSMGetDeferredAssociationTagInfo(/*in*/HCSHANDLE hCrsHandle, /*in*/WORD wAssociationTag, /*out*/WORD *pwTsId, /*out*/WORD *pwOrgNetId, /*out*/WORD *pwServiceId);

//������Ҫ�غ�carouselId��ȡ�ֲ��Ļ�����Ϣ
//carouselId��componetnTag����ΪOR��ϵ���ȿ���carouselId����Ϊ0xffff�ſ���componetnTag�������߶�Ϊ0~����ȡ��service�еĵ�һ���ֲ�
CSDSM_ErrorCode_E CSDSMGetCarouselInfo(DWORD dwDeviceId, WORD wOrgNetId, WORD wTsId, WORD wServiceId, DWORD wCrsId, BYTE ucCmptTag, /*out*/CSDSMCarouselInfo_t* psCrsInfo);

/** Get VerifyFlag by Handle */
  CSDSM_ErrorCode_E CSDSMGetVerifyFlagByHandle(HCSHANDLE handle, int *flag);

//��szPath����Ӧ���ļ�
HCSHANDLE CSDSMOpen(HCSHANDLE hCrsl, char * szPath);

// ��CSDSMOpen ���򿪵��ļ���ָ����ǰ�Ķ�/дλ��
int CSDSMSeek(HCSHANDLE hFileObj, int nOffset, int nOrigin);

//��ȡhFileObjָ����ļ�
int CSDSMRead(HCSHANDLE hFileObj, char *pcBuf, UINT uSize);

//�ر�hFileObjָ����ļ�
int CSDSMClose(HCSHANDLE hFileObj);

//��ȡhFileObjָ���ļ��Ĵ�С
int CSDSMFileSize(HCSHANDLE hFileObj);

//ȡ������Ԥȡ����
CSDSM_ErrorCode_E CSDSMCancelPreLoad(HCSHANDLE hCrsHandle);

//���ø��ֲ��Ƿ�ʵʱ������
//eUpdateTypeΪ��������,nIntervalΪ���¼������λ���롣
CSDSM_ErrorCode_E CSDSMSetUpdate(HCSHANDLE hCrsHandle, CSDSMUpdateType_E eUpdateType, int nInterval);

/**
@brief ����OC�����ò���

@param[in] eConfigParamType ��������
@param[in] nValue ������ΪEM_DSM_FREE_MAIN_BUF��EM_DSM_NOT_FREE_MAIN_BUF��EM_DSM_FILENAME_STRCASE��EM_DSM_FILENAME_NOT_STRCASEʱ�����Ĵ���;
			������ΪEM_DSM_REQUEST_INTERVALʱnValue���Ժ���Ϊ��λ��ʱ����.
@return CSDSM_ERROR_NO_ERROR��ʾ���óɹ�������Ϊʧ��
*/
CSDSM_ErrorCode_E CSDSMSetConfigParam(CSDSMConfigParamType_E eConfigParamType, int nValue);

/**
@brief �����ֲ��µ������ļ�

@param[in] pcPath ����·��
@param[in] pcPath �ַ���pcPath�ĳ���
@return TRUE��ʾ����ɹ���FALSEΪʧ��
*/
BOOL CSDSMSaveFiles(char *pcPath, int nLen);

/**
@brief ָֹͣ�����ݹ㲥�е���������

@param[in] hCrsHandle ָ��Ҫֹͣ��������ݹ㲥
@return TRUE��ʾ�ɹ���FALSEΪʧ��
@note 1���ú������سɹ����ֲ�hCrsHandle�е��������������޷�����,
         �ѷ��������󶼻ᱻȡ������Ҫ�ָ����ֲ��������µ���CSDSMAttachGateway��
      2�����ֲ�δ����(attach)�ɹ�����ú��������ʧ�ܡ�
         ���ڸú�������ʧ��ʱж���ֲ����´�ʹ�ø��ֲ�ʱ���¹���(attach)��
*/
BOOL CSDSMStopLoad(HCSHANDLE hCrsHandle);


#ifdef __cplusplus
}
#endif

#endif /* _DSMOC_SG_H */

