#ifndef _CS_DSMCC_H_
#define _CS_DSMCC_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "mmcp_os.h"

/*BIOP对象类型定义*/
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
/*错误类型*/
typedef enum _CSDSM_ErrorCode_E{
	CSDSM_ERROR_NO_ERROR = 				  0,			//没有错误
	CSDSM_ERROR_INVALID_PARAM = 			-1,			//参数错误
	CSDSM_ERROR_NO_SUCH_OBJ = 			-2,			//对象不存在
	CSDSM_ERROR_OBJ_NOT_LOADED = 		-3,			//对象未下载
	CSDSM_ERROR_OBJ_LOADING =			-4,			//对象正在下载
	CSDSM_ERROR_OBJ_LOCATION_FOREIGN = 	-5,			//对象在另一个轮播中
	CSDSM_ERROR_ISNOT_STREAM_EVENT = 	-7,			//非流事件
	CSDSM_ERROR_NO_FREE_GATEWAY = 		-8,			//没有空闲的sg
	CSDSM_ERROR_NO_MEMORY = 				-9,			//内存不足
	CSDSM_ERROR_NO_SUCH_EVENT = 			-10,			//没有该事件
	CSDSM_ERROR_MUTEX_ERROR = 			-11,			//信号量操作失败
	CSDSM_ERROR_TIMER_ERROR = 			-12,			//计时器错误
	CSDSM_ERROR_EVENT_ERROR = 			-13,		//发送事件错误
	CSDSM_ERROR_OBJ_NOT_RECEIVED = 		-15,			//未收到该对象的信息
	CSDSM_ERROR_OBJ_CHANGED	 =	 		-16			//未收到该对象的信息	
}CSDSM_ErrorCode_E;

/*回调函数的消息类型*/
typedef enum _CSDSMCallbackType_E
{
	EM_DSM_OBJECT_VERSION_CHANGED = 	0,
	EM_DSM_STREAM_EVENT_ARRIVIED =	1,
	EM_DSM_ATTACH_SUCCESS = 			2,/* pucPrivateData = NULL*/
	EM_DSM_ATTACH_FAILED = 			3,/* pucPrivateData = NULL*/
	EM_DSM_OBJECT_LOAD_SUCCESS = 		4,
	EM_DSM_OBJECT_LOAD_FAILED = 		5
}CSDSMCallbackType_E;

/*用户下载请求类型*/
typedef enum _CSDSMLoadType_E
{
	EM_DSM_NOT_PRELOAD = 				0, /*非预取请求*/
	EM_DSM_PRELOAD_WITH_CANCEL = 	1,/*预取请求，可被主动取消*/
	EM_DSM_PRELOAD_NO_CANCEL = 		2 /*预取请求，不能被取消*/	
}CSDSMLoadType_E;
//监测更新类型
typedef enum _CSDSMUpdateType_E
{
	EM_DSM_MONITOR_UPDATE =			0,/*实时监测更新*/
	EM_DSM_IGNORE_UPDATE =			1/*优先从缓存读取数据，忽略更新*/
}CSDSMUpdateType_E;

typedef enum _CSDSMConfigParamType_E
{
	EM_DSM_NOT_FREE_MAIN_BUF = 0,/*解析完DDB后不释放组表用的1.1M内存，默认不释放组表的1.1M内存*/
	EM_DSM_FREE_MAIN_BUF	         = 1,/*解析完DDB后释放组表用的1.1M内存*/
	EM_DSM_REQUEST_INTERVAL   = 2, /*设置发送建立目录树请求的时间间隔，默认为10ms*/
	EM_DSM_MEMORY_VECTOR       =3,/*使用内存向量方式分配内存*/
	EM_DSM_MEMORY_MALLOC       =4,/*使用MMCP_MemMalloc()方式分配内存,默认方式*/
	EM_DSM_UPDATE_FILE_COUNT =5,/*设置检测到更新后需加载的文件个数*/
	EM_DSM_DII_REPEAT_COUNT    =6,/*判断DII是否收齐的重复门限*/
	EM_DSM_FILENAME_STRCASE     =7,/*不区分文件名的大小写，默认不区分*/
	EM_DSM_FILENAME_NOT_STRCASE     =8/*区分文件名的大小写，默认不区分*/
}CSDSMConfigParamType_E;

/********** struct **********/
/*流标识符*/
typedef struct  _Stream_Taps {
    WORD	m_wId;/*reserved for future*/
    WORD 	m_wUse;/*0x0B-NPT,0x0C-str&ste des,0x0D-ste des,0x0E-str des,0x18-ES stream,0x19-BIOP_PROGRAM_USE*/
    WORD 	m_wAssociationTag;
    BYTE 	m_ucSelectorLen;/*reserved for future*/
} CSDSMStreamTaps_t;

/*流对象*/
typedef struct _Stream_object{
    DWORD 			m_dwDurationS;/*stream duration seconds*/
    DWORD 			m_dwDurationMS;/*stream duration microseconds*/
    BYTE 				m_ucAudio;/*if stream is audio then m_ucAudio!=0*/
    BYTE 				m_ucVideo;/*if stream is video then m_ucVideo!=0*/
    BYTE 				m_ucData;/*others m_ucData!=0*/
    BYTE 				m_ucTapsCount;/*the count of taps associated with this stream*/
    CSDSMStreamTaps_t *m_pStreamTapList;/*this memory size is m_ucTapsCount*sizeof(CSDSMStreamTaps_t)*/
} CSDSMStreamObject_t;

/*流事件对象*/
typedef struct _Streamevent_object {
    CSDSMStreamObject_t m_StreamObject;
    DWORD 			 m_dwEventListBytes;/*memory size of m_pucEventList*/
    BYTE *				 m_pucEventList;/*eventNames_count-2 bytes,following eventName_lenght-1 bytes,eventName_data-eventName_lenght bytes,......*/
    BYTE 				 m_ucEventIdCount;/*this field indentifies the size of m_pwEventIdArray*/
    WORD *			 m_pwEventIdArray;/*this memory size is m_ucEventIdCount*sizeof(WORD)*/
} CSDSMStreamEventObject_t;

/*网络业务接入点*/
typedef struct _Nsap 
{
    BYTE 		m_ucNameCount;
    char *		m_pcPathName;
    DWORD	m_dwCrsId;
    WORD 		m_wTsId;
    WORD 		m_wOrgNetId;
    WORD 		m_wServiceId;
} CSDSMNsap_t;

/*对象结构*/
typedef struct _CSDSMOcObject_S {
    BYTE 			m_ucObjType;		       /*object type: file/dir/str/ste/srg,参见BIOP对象类型定义*/
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
	WORD 	m_wOrgNetId;/*原始网络ID*/
	WORD	m_wTsId;/*传输流ID*/
	WORD 	m_wServiceId;/*节目号*/
	BYTE   	m_ucComponentTag;/*与m_wCrsEsPid对应的ComponentTag*/
	DWORD  	m_dwTransactionId;/*事务ID,used for session integrity and error processing*/
	DWORD  	m_dwTimeoutDsi;/*DSI信息装载参考超时，0为无参考*/
	DWORD  	m_dwTimeoutDii;/*DII信息装载参考超时，0为无参考*/
	WORD  	m_wCrsEsPid;/*DSI信息所在的基本流的Pid*/
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

/*当回调消息类型为EM_DSM_OBJECT_VERSION_CHANGED, EM_DSM_OBJECT_LOAD_SUCCESS,
EM_DSM_OBJECT_LOAD_FAILED 时的消息内容结构体*/
typedef struct _CSDSMObjectMsgInfo_S
{
	int nValue;
	char *pcObjAbsolutePath;
}CSDSMObjectMsgInfo_t;

/*当回调消息类型为EM_DSM_STREAM_EVENT_ARRIVIED时的消息内容结构体*/
typedef struct _DSMStreamEvent_S
{
	DWORD	dwNptSecs;
	DWORD 	dwNptMicroSecs;
	int 		nEventId;
	int 		nPrvDataLen;
	BYTE * 	pucPrivateData;
}DSMStreamEvent_t;

/*通知用户函数*/
typedef int (*CSDSMCallback) (
 HCSHANDLE 			hCrsHandle,
 CSDSMCallbackType_E 	eMsg, /*值为enum类型 */
 void * 				pUserData,/*传入的用户数据*/
 void *				pMsgData/*根据nMsg的值转换成相应的数据结构*/
);/* 用户定义的回调函数*/

/* Service Gateway APIs */

/**
* 初始化DSM模块
* dwMaxCacheSize为分配给OC的内存大小，单位为KBYTE
* 成功返回0，失败返回-1
*/
CSDSM_ErrorCode_E CSDSMInit(DWORD dwMaxCacheSize);

/**
* 把一个轮播关联到文件系统
@param [in]
@param [in] 
@param [out] pHandle 轮播和文件系统成功关联后的轮播句柄
\retval NULL
* 成功返回0，失败返回-1
*/
CSDSM_ErrorCode_E CSDSMAttachGateway(DWORD dwDeviceId, char *pcMoutedDirName, CSDSMCarouselInfo_t * pCrsInfo, BOOL bAuth, int nCheckPeriod, /*out*/HCSHANDLE * pCrsHandle);
/**
* 存储介质可选择的CSDSMAttachGateway
@param [in] szStoragePath 轮播数据实际的存储在文件系统中的位置，NULL表示存储在内存中
*/
CSDSM_ErrorCode_E CSDSMAttachGatewayEx(DWORD dwDeviceId, char *pcMoutedDirName,char * szStoragePath, CSDSMCarouselInfo_t * pCrsInfo, BOOL bAuth, int nCheckPeriod, HCSHANDLE * pCrsHandle);
//断开一个轮播与文件系统之间的关联
CSDSM_ErrorCode_E CSDSMDetachGateway (HCSHANDLE hCrsHandle, BOOL bClearCache);

//在一个轮播中查找一个对象，如果查找成功，会得到该对象的信息
CSDSM_ErrorCode_E CSDSMFindFileObject(char *pcDirName, char *pcFileName,  /*out*/CSDSMOcObject_t ** ppFileObject);

//加载一个对象
CSDSM_ErrorCode_E CSDSMLoadObject(char *pcDirName, char *pcFileName,/*out*/CSDSMOcObject_t ** ppLiteOptionsObject);

//预加载一个对象
//eLoadType为预加载类型
CSDSM_ErrorCode_E CSDSMPreLoadObject(char *pcDirName, char *pcFileName,/*out*/CSDSMOcObject_t ** ppLiteOptionsObject, CSDSMLoadType_E eLoadType);

//取消加载对象
CSDSM_ErrorCode_E CSDSMAbortGatewayAttach(HCSHANDLE hCrsHandle);

//卸载一个对象
CSDSM_ErrorCode_E CSDSMUnloadObject(char *pcDirName, char *pcFileName);

//注册回调函数，返回callbackeId，当加载一个对象成功和流事件到达都会回调通知
DWORD CSDSMRegisterCallback( CSDSMCallbackType_E  eMsg,CSDSMCallback fnCallback,void* pUserData);

//注销回调函数
CSDSM_ErrorCode_E CSDSMUnregisterCallback(/*in*/ DWORD dwCallbackId);

//监听一个对象
CSDSM_ErrorCode_E CSDSMAddMonitorObject(char *pcDirName, char *pcFileName,  void *pUserData);

//取消监听对象
CSDSM_ErrorCode_E CSDSMDelMonitorObject(char *pcDirName, char *pcFileName);

//检测一个对象是否已经加载
CSDSM_ErrorCode_E CSDSMIsObjectExist(WORD wOrgNetId, WORD wTsId, WORD wServiceId, WORD wComponentTag, char *pcFilePath);

//获取某个轮播句柄在文件系统的挂载点，即保存轮播的根目录
CSDSM_ErrorCode_E CSDSMGetMountPointByHandle(/*in*/HCSHANDLE hCrsHandle, /*out*/char **ppcMountPoint);

//根据挂载点获取对应轮播的句柄，如果指定的挂载点不存在，返回NULL
CSDSM_ErrorCode_E CSDSMGetHandleByMountPoint(char *pcMountPoint,HCSHANDLE*pHandle);

//根据四要素获取对应轮播的句柄，如果该三要素指向的轮播不存在或尚未与文件系统关联，返回NULL
CSDSM_ErrorCode_E CSDSMGetHandleByFactors(WORD wOnId,WORD wTsId, WORD wServiceId,WORD wComTag,HCSHANDLE*pHandle);

//监听流事件的到达
CSDSM_ErrorCode_E CSDSMAddMonitorEvent(char *pcDirName, char *pcFileName, int nEventId, void *pUserData);

//取消监听流事件的到达
CSDSM_ErrorCode_E CSDSMDelMonitorEvent(char *pcDirName, char *pcFileName, int nEventId);

//获取association_tag关联的流的三要素
CSDSM_ErrorCode_E CSDSMGetDeferredAssociationTagInfo(/*in*/HCSHANDLE hCrsHandle, /*in*/WORD wAssociationTag, /*out*/WORD *pwTsId, /*out*/WORD *pwOrgNetId, /*out*/WORD *pwServiceId);

//根据三要素和carouselId获取轮播的基本信息
//carouselId和componetnTag两者为OR关系，先考虑carouselId，其为0xffff才考虑componetnTag，若两者都为0~，则取该service中的第一个轮播
CSDSM_ErrorCode_E CSDSMGetCarouselInfo(DWORD dwDeviceId, WORD wOrgNetId, WORD wTsId, WORD wServiceId, DWORD wCrsId, BYTE ucCmptTag, /*out*/CSDSMCarouselInfo_t* psCrsInfo);

/** Get VerifyFlag by Handle */
  CSDSM_ErrorCode_E CSDSMGetVerifyFlagByHandle(HCSHANDLE handle, int *flag);

//打开szPath所对应的文件
HCSHANDLE CSDSMOpen(HCSHANDLE hCrsl, char * szPath);

// 在CSDSMOpen 语句打开的文件中指定当前的读/写位置
int CSDSMSeek(HCSHANDLE hFileObj, int nOffset, int nOrigin);

//读取hFileObj指向的文件
int CSDSMRead(HCSHANDLE hFileObj, char *pcBuf, UINT uSize);

//关闭hFileObj指向的文件
int CSDSMClose(HCSHANDLE hFileObj);

//获取hFileObj指向文件的大小
int CSDSMFileSize(HCSHANDLE hFileObj);

//取消所有预取请求
CSDSM_ErrorCode_E CSDSMCancelPreLoad(HCSHANDLE hCrsHandle);

//设置该轮播是否实时监测更新
//eUpdateType为更新类型,nInterval为更新间隔，单位毫秒。
CSDSM_ErrorCode_E CSDSMSetUpdate(HCSHANDLE hCrsHandle, CSDSMUpdateType_E eUpdateType, int nInterval);

/**
@brief 设置OC的配置参数

@param[in] eConfigParamType 配置类型
@param[in] nValue 当类型为EM_DSM_FREE_MAIN_BUF、EM_DSM_NOT_FREE_MAIN_BUF、EM_DSM_FILENAME_STRCASE、EM_DSM_FILENAME_NOT_STRCASE时不关心此项;
			当类型为EM_DSM_REQUEST_INTERVAL时nValue是以毫秒为单位的时间间隔.
@return CSDSM_ERROR_NO_ERROR表示配置成功，其他为失败
*/
CSDSM_ErrorCode_E CSDSMSetConfigParam(CSDSMConfigParamType_E eConfigParamType, int nValue);

/**
@brief 保存轮播下的所有文件

@param[in] pcPath 保存路径
@param[in] pcPath 字符串pcPath的长度
@return TRUE表示保存成功，FALSE为失败
*/
BOOL CSDSMSaveFiles(char *pcPath, int nLen);

/**
@brief 停止指定数据广播中的所有请求

@param[in] hCrsHandle 指定要停止请求的数据广播
@return TRUE表示成功，FALSE为失败
@note 1、该函数返回成功后，轮播hCrsHandle中的所有数据请求都无法发出,
         已发出的请求都会被取消。若要恢复该轮播，请重新调用CSDSMAttachGateway。
      2、若轮播未关联(attach)成功，则该函数会调用失败。
         请在该函数返回失败时卸载轮播，下次使用该轮播时重新关联(attach)。
*/
BOOL CSDSMStopLoad(HCSHANDLE hCrsHandle);


#ifdef __cplusplus
}
#endif

#endif /* _DSMOC_SG_H */

