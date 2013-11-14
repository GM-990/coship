/**@defgroup RECORD  RECORD 定义了PVR 功能相关的接口
@brief 本模块主要定义了PVR录制相关的接口

录制分为三种:节目录制、时移录制、ts流录制

- 节目录制:
录制一路节目的音视频节目数据至指定文件。
只允许录制一路节目的音视频、pcr数据，不允许将多个节目的pid数据同时录制。
节目录制的文件回放时可以进行trickmode操作

- 时移录制:
时移录制是属于节目录制的一种特例，时移录制的录制文件是循环文件
循环文件:大小固定，文件创建时，即占据了固定大小的磁盘空间，当写文件大小超过固定时，自动绕回从开头写，开头的数据会被自动覆盖
当时移录制绕回后，录制时间信息中的开始时间为绕回后覆盖的长度。
时移录制的停止可以调用CSUDIRECStop或者调用CSUDIRECStopTshift
调用CSUDIRECStopTshift时可以将时移录制的循环文件转换成普通文件，用于普通的文件回放。

-ts流录制
录制一路ts流，允许录制多路节目的数据，允许录制si表格
ts流录制时，不会生成相应的索引文件，回放ts流时，一般不支持trickmode操作。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

 @version 2.1.1 2010/11/1 启动录制参数增加加密类型，追加录制，时移文件大小，增加tshift转普通文件，pvr录制文件切割接口  
 @version 2.0.1 2009/8/10  
 @{
*/
#ifndef _UDI2_RECORD_H_
#define _UDI2_RECORD_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C"
{
#endif 

#define CSUDI_REC_MAX_PID_COUNT 32

/**@brief 录制相关错误代码值*/
enum
{
	CSUDIREC_ERROR_BAD_PARAMETER = CSUDI_RECORD_ERROR_BASE,			///< 参数错误
	CSUDIREC_ERROR_FEATURE_NOT_SUPPORTED,							///< 操作不支持
	CSUDIREC_ERROR_UNKNOWN_ERROR,									///< 一般错误
	CSUDIREC_ERROR_NO_MEMORY,										///< 无可用内存
	CSUDIREC_ERROR_INVALID_DEVICE_ID,								///< 非法的设备ID值
	CSUDIREC_ERROR_INVALID_HANDLE,									///< 非法的句柄
	CSUDIREC_ERROR_RECORD_FULL,										///< 超过允许的录制个数
	CSUDIREC_ERROR_ALREADY_STARTED,									///< 该录制已经启动
	CSUDIREC_ERROR_ALREADY_STOPPED,									///< 该录制已经停止
	CSUDIREC_ERROR_NOT_STARTED,										///< 该录制还没有启动
	CSUDIREC_ERROR_INVALID_PIDS,									///< 非法PID值,包括无PID值
	CSUDIREC_ERROR_TIME_FAILED,										///< 获取录制时间失败
	CSUDIREC_ERROR_NO_CALLBACK,								 	///< 没有相应的回调函数可以删除
	CSUDIREC_ERROR_ALREADY_ADDED								///< 该回调已经添加过
};

/**@brief 录制类型*/
typedef enum
{
	EM_UDIRECORD_TYPE_CHNL = 0,           ///<    录制一路节目，保存的文件形式为线性文件
	EM_UDIRECORD_TYPE_TSHIFT,              ///<    时移录制,保存的文件形式为循环文件，回放时可以进行trickmode操作
	EM_UDIRECORD_TYPE_TS                      ///<    ts流录制（ts流中包含多路节目）
}CSUDIRecType_E;

/**@brief 录制回调信息*/
typedef enum
{
	EM_UDIRECORD_EVT_DISKFULL,      ///<   存储设备没有空间消息
	EM_UDIRECORD_EVT_UNKNOWN,       ///<   未知消息类型
	EM_UDIRECORD_EVT_WRITEFAIL,     ///<   录制写文件失败
	EM_UDIRECORD_EVT_MAX_NUM        ///<   无效边界值
}CSUDIRecEvent_E;

/**@brief 录制通道设备资源数据结构定义*/
typedef struct
{
	int m_nDemux;		///< Demux设备的Index，-1代表不存在Demux设备
}CSUDIRecChnl_S;

/**@brief 录制时间信息*/
typedef struct {
	int       m_nStartTime;   		///<  录制开始的绝对时间，对于非时移录类型时，该时间始终为0
	int       m_nValidDuration;   	///<  录制有效时间，即录制可播放的时长
} CSUDIRecTimeInfo_S;

/**@brief 码流加密类型，一般用于PVR*/
typedef enum
{
	EM_UDI_ENCRYPT_TYPE_FREE=0,		///< 清流，不加密
	EM_UDI_ENCRYPT_TYPE_CA,			///< ca加密流
	EM_UDI_ENCRYPT_TYPE_REENCRYPT	///< 二次加密流(先ca解绕成清流，再平台二次加密)
}CSUDIStreamEncryptType_E;

/**@brief 录制stream加密数据结构定义*/
typedef struct{
	CSUDIStreamEncryptType_E m_eEncryptType;				///<加密方式，详见CSUDIRecEncryptType_E说明
	CSUDI_UINT8 m_ucEncryptKey[CSUDI_MAX_SECURITYKEY_LEN]; 	///<加密解密Key,如果是录制就是加密Key;如果是播放就是解密Key
	int m_nEncryptKeyLength; 								///<密钥长度
} CSUDIStreamEncryptParam_S;

/**@brief 启动录制信息*/
typedef struct {
	CSUDIRecType_E		m_eRecType;								///< 该路录制的类型，详见CSUDIRecType_E说明
	char                m_szFileName[CSUDI_MAX_FILE_NAME_LEN];  ///< 保存录制结果的文件名，只支持ASCII编码格式
	int					m_nCount;								///< 录制的pid个数
	CSUDIStreamInfo_S	m_psPids[CSUDI_REC_MAX_PID_COUNT];    	///< 需要录制的节目pids 详见CSUDIStreamInfo_S说明
	CSUDI_BOOL			m_bAppend;								///追加录制，m_bAppend=false时，如果录制目标文件存在时，会先将目标文件清除后，再从头写文件
																///<m_bAppend=true时，如果录制目标文件存在时，不删除现有文件，在现有文件末尾追加写
	unsigned int		m_nMaxSize;								///< 录制最大长度(单位为MB)，仅当m_eRecType==EM_UDIRECORD_TYPE_TSHIFT时有效
    CSUDIStreamEncryptParam_S m_stEncryptParam; 				///<参见CSUDIStreamEncryptParam_S
} CSUDIRecStartParam_S;

/**@brief 录制回调函数定义

@param[in] hRecHandle  该路录制句柄
@param[in] eEvent 录制消息类型，详见CSUDIRecEvent_E
@param[in] pvUserData 用户数据
@return  必须返回0
@note 由hHandle和pvUserData唯一区分每个回调函数
*/
typedef int (*CSUDIRECCallback_F)(CSUDI_HANDLE hRecHandle,const CSUDIRecEvent_E eEvent, void* pvEventData,void * pvUserData);

/**
@brief 注册录制回调函数

@param[in] hRecHandle 录制句柄，由CSUDIRECOpen()获得
@param[in] fnRecordCallback 录制回调函数，详见CSUDIRECCallback_F说明
@param[in] pvUserData  用户数据，此数据会原封不动返回给用户
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 至少支持注册的回调函数个数为32个，最大不限
@note 回调函数指针+回调函数类型+用户数据唯一标识一个回调,当两次注册它们完全一样时，第二次将返回CSUDIREC_ERROR_ALREADY_ADDED
@note 当注册的函数已经注册过将返回CSUDIREC_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECAddCallback(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback, void * pvUserData);

/**
@brief 删除录制回调函数

@param[in] hRecHandle 录制句柄，由CSUDIRECOpen()获得
@param[in] fnRecordCallback 录制回调函数，详见CSUDIRECCallback_F说明
@param[in] pvUserData  用户数据，此数据会原封不动返回给用户
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIREC_ERROR_NO_CALLBACK
@note 允许在回调函数中删除回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECDelCallback(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback,void * pvUserData);

/**
@brief 创建一个录制器

@param[in] psChnl   录制通道的数据结构。
@param[out] phRecHandle 打开录制的句柄。
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECOpen(const CSUDIRecChnl_S * psChnl, CSUDI_HANDLE * phRecHandle);

/**
@brief 获取录制支持的加密类型

@param[in] hRecHandle 录制句柄，由CSUDIRECOpen()获得
@param[out] peOutTypeArray   该路录制支持的加密方式(可能支持多种)，详见CSUDIRecEncryptType_E定义
@param[in]  nArraySize 传入peOutTypeArray数组的长度
@param[out]  pnActualCnt 该路录制支持的加密方式种数
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECGetSupportEncryptTypes(CSUDI_HANDLE hRecHandle,CSUDIStreamEncryptType_E* peOutTypeArray,int nArraySize,int* pnActualCnt);

/**
@brief 销毁一个录制器

@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 销毁一个录制器；与CSUDIRECOpen成对使用。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECClose(CSUDI_HANDLE hRecHandle);

/**
@brief 启动录制

@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@param[in] psStartParam  启动录制参数详见CSUDIRecStartParam_S说明
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 由参数psStartParam指定需要录制的类型以及录制的节目
@note 调用此接口之前，需要调用CSUDIDEMUXConnectTuner连接hRecHandle对应的demux和tuner设备，并使用对应的tuner已经锁频成功。
@note 如果该录制所用的demux已经连接tuner，则此时不需要再次调用CSUDIDEMUXConnectTuner进行demux与tuner的连接
@see CSUDIDEMUXConnectTuner

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStart(CSUDI_HANDLE  hRecHandle,const CSUDIRecStartParam_S * psStartParam);

/**
@brief 停止录制

@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStop(CSUDI_HANDLE  hRecHandle);

/**
@brief 暂停录制
@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECPause(CSUDI_HANDLE  hRecHandle);

/**
@brief 恢复录制
@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 恢复录制后新录制的内容追加到暂停前的录制文件中

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECResume(CSUDI_HANDLE  hRecHandle);


/**
@brief 停止时移录制

@param[in] hRecHandle 正在时移录制的录制器句柄，由函数CSUDIRECOpen()获得
@param[in] bSwitchToFile 是否将时移录制的循环文件转换成普通文件
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStopTshift(CSUDI_HANDLE  hRecHandle,CSUDI_BOOL bSwitchToFile);

/**
@brief 获取录制时间信息

@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@param[out] psTimeInfo 录制的时间信息，详见CSUDIRecTimeInfo_S定义
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECGetRecTime(CSUDI_HANDLE  hRecHandle, CSUDIRecTimeInfo_S * psTimeInfo);

/**
@brief 录制文件切割

@param[in] szFileName 录制生成的文件
@param[in] unStartPos 剪切开始位置(单位为s)
@param[in] unEndPos 剪切的结束位置(单位为s)
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 切割操作需要将参数的时间位置nstartPos和nEndPos转换成文件中的物理位置，并将该段数据删除掉，切割后的文件可播放长度比切割前减少了(nEndPos-nStartPos)

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECFileCut(const char* szFileName,unsigned int unStartPos,unsigned int unEndPos);

#ifdef __cplusplus
}
#endif
/**@}*/
#endif
