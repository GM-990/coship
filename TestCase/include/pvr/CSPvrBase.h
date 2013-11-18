#ifndef _CS_PVRBASE_H_
#define _CS_PVRBASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmcp_typedef.h"
//#include <CSUSPCommonType.h>
#include <mmcp_os.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define PVR_INVALID_SECOND_LENGTH (-1)

#define PVR_INVALID_PID (0x1fff)
#define PVR_MAX_PIDS_COUNT 16
#define PVR_PIDTYPE(PID) (PID>>13)
#define PVR_PIDVALUE(PID) (PID & 0x1fff)
#define PVR_COMPOSE_PID(PID, TYPE) ((WORD) ((PID & 0x1fff) | (TYPE << 13)) )
#define PVR_PID_IS_VALID(PID) ( (PVR_PIDVALUE(PID)!=0) && (PVR_PIDVALUE(PID)!=0x1fff) ) /*PAT请勿用此宏*/
#define PVR_PID_NOT_VALID(PID) (!PVR_PID_IS_VALID(PID)) /*PAT请勿用此宏*/

#define PVR_MAX_CRYPT_KEY_SIZE (128)

#define PVR_AUD_PRE 0x0100
#define PVR_AUD_MP1 0
#define PVR_AUD_MP2 1
#define PVR_AUD_MP4 2
#define PVR_AUD_MP3  3
#define PVR_AUD_AC3 4
#define PVR_AUD_HE_AAC  5
#define PVR_AUD_MPG_AAC 6     /*Mpeg_AAC*/
#define PVR_AUD_AC3_PLUS 7  

#define PVR_VID_PRE 0x0200
#define PVR_VID_MP1 1
#define PVR_VID_MP2 0
#define PVR_VID_MP4 2
#define PVR_VID_H264 3


#define AUD_TYPE_MP1 (PVR_AUD_PRE | PVR_AUD_MP1)
#define AUD_TYPE_MP2 (PVR_AUD_PRE | PVR_AUD_MP2)
#define AUD_TYPE_MP3 (PVR_AUD_PRE | PVR_AUD_MP3)
#define AUD_TYPE_MP4 (PVR_AUD_PRE | PVR_AUD_MP4)
#define AUD_TYPE_AC3 (PVR_AUD_PRE | PVR_AUD_AC3)
#define AUD_TYPE_HE_AAC (PVR_AUD_PRE | PVR_AUD_HE_AAC)
#define AUD_TYPE_MPG_AAC (PVR_AUD_PRE | PVR_AUD_MPG_AAC) 
#define AUD_TYPE_AC3_PLUS (PVR_AUD_PRE | PVR_AUD_AC3_PLUS) 


#define VID_TYPE_MP1 (PVR_VID_PRE | PVR_VID_MP1)
#define VID_TYPE_MP2 (PVR_VID_PRE | PVR_VID_MP2)
#define VID_TYPE_MP4 (PVR_VID_PRE | PVR_VID_MP4)
#define VID_TYPE_H264 (PVR_VID_PRE | PVR_VID_H264)

#define PVR_OTHER_PRE 0x0300
#define PCR_TYPE (PVR_OTHER_PRE | 1)
#define TTX_TYPE (PVR_OTHER_PRE | 2)
#define SUB_TYPE (PVR_OTHER_PRE | 3)
#define CC_TYPE  (PVR_OTHER_PRE | 4)

#define PVR_TABLE_PRE 0x0400
#define PAT_TYPE (PVR_TABLE_PRE | 1)
#define CAT_TYPE (PVR_TABLE_PRE | 2)
#define PMT_TYPE (PVR_TABLE_PRE | 3)
#define SDT_TYPE (PVR_TABLE_PRE | 4)
#define EIT_TYPE (PVR_TABLE_PRE | 5)
#define NIT_TYPE (PVR_TABLE_PRE | 6)

#define NOT_SUPPORTED_TYPE 0xFFFF    

typedef struct 
{
	WORD awPid[PVR_MAX_PIDS_COUNT];
	WORD awType[PVR_MAX_PIDS_COUNT];
	BYTE ucCount;
}SPvrBasePidData;



typedef HCSHANDLE (*CSOnPvrBaseOpenFile_F)(const char *szFileName);
typedef int (*CSOnPvrBaseWriteFile_F)(HCSHANDLE hFile, BYTE *pbuf, int nBufSize);
typedef int (*CSOnPvrBaseReadFile_F)(HCSHANDLE hFile, BYTE *pbuf, int nBufSize);
typedef BOOL (*CSOnPvrBaseCloseFile_F)(HCSHANDLE hFile);

typedef struct
{
	CSOnPvrBaseOpenFile_F m_fnOnOpenFile;
	CSOnPvrBaseWriteFile_F m_fnOnWriteFile;
	CSOnPvrBaseReadFile_F m_fnOnReadFile;
	CSOnPvrBaseCloseFile_F m_fnOnCloseFile;
}CSPvrBaseFileIo_S; /*added by wanglin, 2007.9.26 */
/*removed by luojinhua 2006-12-15
typedef int(*FPvrBaseNotify)(void *pUserData);*/

typedef struct _CSPvrBaseInitParam_S
{
	int nMaxRecordCount;
	int nMaxPlayCount;
}CSPvrBaseInitParam_S;

/*!
功能描述：
	初始化。

参数：
	无

返回值
	成功初始化返回TRUE。
	否则返回FALSE。

备注：
	这个函数必须在Record和Play模块初始化之前被调用
*/
BOOL CSPvrBase_Init(const CSPvrBaseInitParam_S* sInitParam);

/*!
功能描述：
	锁定Tuner。

参数：
	nDemux
		[in]	Demux的索引号
	nTuner
		[in]	Tuner号

返回值
	成功返回TRUE。
	否则返回FALSE。

备注：
	无
*/
BOOL CSPvrBase_ConnectDemuxAndTuner(const int nDemux, const int nTuner);

/*!
功能描述：
	获取是否已初始化。

参数：
	无

返回值
	是否已初始化

备注：
	无
*/
BOOL CSPvrBase_HaveInited(void);


/*[[[add by luojinhua 2007-08-04*/
/*!
功能描述：
	获取用于TS注入的Demux索引号

参数：
	无

返回值
	<0   , 表示失败
	>= 0, 返回用于TS注入的Demux索引

备注：
	对于SWTS Demux,可以通过DMA的方式将内存缓冲区的数据注入到Demux中。
	此接口保留，以后获取注入Demux将从RM中获取。
*/
int CSPvrBase_GetSWTSDemuxIdx(void);



/*!
功能描述：
	注册网络PVR的文件操作。
	
参数：
	sFileIo : 对于文件操作的函数集合结构。

返回值
	无

备注：
	用户可以自己实现文件的操作，以便抽象存储访问。

*/
void CSPvrBase_SetIpPumpFileIo(CSPvrBaseFileIo_S sFileIo);


/*!
功能描述：
	设置PVR模块缺省的加密key
	
参数：
	pucCryptKey : 加密key的首地址
	uKeySize      :  key的长度

返回值
	成功返回TRUE；失败返回FALSE

备注：
	无
*/
BOOL CSPvrBase_SetDefaultCryptKey(const BYTE * pucCryptKey, UINT uKeySize);

/*!
功能描述：
	获取PVR模块缺省的加密key
	
参数：
	pucCryptKey : 缓冲区的首地址
	uKeySize      :  要求获取key的长度

返回值
	实际返回的key的长度

备注：
	无
*/
UINT CSPvrBase_GetDefaultCryptKet(BYTE * pucCryptKey, UINT uKeySize);

/*!
功能描述：
	打开或关闭PvrEngine模块的Trace打印信息
	
参数：
	bEnable  : 决定是否打开PvrEngine的Trace信息。
			  TRUE则打开，FALSE则关闭。
	
返回值
	无

备注：
	无
*/
void CSPvrBase_EnableTraceInfo(BOOL bEnable);

/*!
功能描述：
	注册获取设备路径(path) 的回调函数
	
参数：
	fnCb  : 获取设备路径的回调函数
	pUserData : 用户自定义数据
	
返回值
	无

备注：
	无
*/
typedef HCSHANDLE (*PvrBaseGetPath_F)(int nWorkId,BOOL bIsPlay,int nDemux, void *pUserData);
void CSPvrBase_RegGetPathCb(PvrBaseGetPath_F fnCb, void *pUserData);


/*!
功能描述：
	注册转换路径的回调函数
	
参数：
	fnCb  : 转换路径的回调函数
	
返回值
	无

备注：
	pvrEngine的调用者传递给给pvrEngine的路径通常pvrEngine不能识别；
	pvrEngine使用的是驱动能识别的路径，因此需要一个转换
*/
typedef int (*PvrPathConvert_F)(const char * szCsfsPath,int nDriverNameLen,char *szDriverPath);
void CSPvrBase_RegPathConvertCb(PvrPathConvert_F fnCb);
/*!
功能描述：
	获取pvrEngine的版本信息
	
参数：
	无
	
返回值
	pvrEngine的版本信息

备注：
	无
*/
char * CSPvrBase_GetVersionInfo(void);
#ifdef __cplusplus
}
#endif

#endif
