#ifndef _CS_RECORD_TEST_H_
#define _CS_RECORD_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_typedef.h"
#include "udiplus_os.h"
#include "udiplus_debug.h"
#include "udi2_record.h"
#include "udi2_demux.h"
#include "udi2_public.h"
#include "udi2_audio.h"
#include "udi2_video.h"
#include "udi2_player.h"
#include "udi2_fs.h"
#include "../cs_udi2testcase.h"

#define UDI2_TUNER0_ID 0
#define SHORT_REC_TIME (10*1000) //(ms)
#define MIDDLE_REC_TIME (30*1000) //(ms)
#define LONG_REC_TIME (120*1000) //(ms)
#define TSHIFT_MAX_SIZE 50 //(MB)

typedef enum
{
	EM_RECORD_H264_HD,
	EM_RECORD_H264_MPEG1,
	EM_RECORD_MAX
}RecordTestServiceIndex_E;

/**@brief 测试用到的码流信息*/
typedef struct 
{	
	const char*				m_pcTsFilename;		///<码流文件名称
	const char*				m_pcServiceDescript;	///<码流描述
	int						m_nVideoPid;           	///< 视频PID，-1表示不存在
	CSUDIVIDStreamType_E 	m_eVidStreamType;	///<视频类型
	int						m_nAudioPid;           	///< 音频PID ，-1表示不存在
	CSUDIAUDStreamType_E	m_eAudStreamType;	///<音频类型
	int						m_nPcrPid;			///< PCR类型PID，-1表示不存在
	int 				 		m_nSubPid;			///<SUBTITLE类型的PID，-1表示不存在
	int 				 		m_nTelPid;			///<TELETEXT类型的PID，-1表示不存在
}RecordTestServiceInfo_S;

/**@brief 查找一个支持录制的demux

@return  成功返回支持录制的demuxId, 失败返回-1
*/
int FindDemuxSupportRec(void);

/**@brief 查找所有支持录制的demux

@param[out] pOutDemuxArray  所有支持录制的demuxId数组
@param[in] nArraySize 传入pOutDemuxArray数组的长度
@param[out] pnActualCnt 支持录制的demux的实际个数
@return  成功返回CSUDI_TRUE, 失败返回CSUDI_FALSE
*/
CSUDI_BOOL FindAllDemuxSupportRec(int* pOutDemuxArray, int nArraySize, int* pnActualCnt);

/**@brief 查找一个不支持录制的demux

@return  成功返回不支持录制的demuxId, 失败返回-1
*/
int FindDemuxNotSupportRec(void);

/**@brief 查找一个支持PVR录制文件、时移文件播放的demux

@return  成功返回支持PVR录制文件、时移文件播放的demuxId, 失败返回-1
*/
int FindDemuxSupportPvrPlay(void);

/**@brief 查找一个支持直播播放的demux

@return  成功返回支持直播播放的demuxId, 失败返回-1
*/
int FindDemuxSupportLivePlay(void);

/**@brief 查找支持某种音频类型的音频解码器

@param[in] eAudioType 音频类型, 参见CSUDIAUDStreamType_E
@return  成功返回支持该种音频类型的AudioId, 失败返回-1
*/
int FindAudioDecoder(CSUDIAUDStreamType_E eAudioType);

/**@brief 查找支持某种视频类型的视频解码器

@param[in] eVideoType 视频类型, 参见CSUDIVIDStreamType_E
@return  成功返回支持该种视频类型的VideoId, 失败返回-1
*/
int FindVideoDecoder(CSUDIVIDStreamType_E eVideoType);

/**@brief 开始直播

@param[in] psPlayerChnl 播放通道资源信息
@param[in] psAudioStream 音频流信息
@param[in] psVideoStream 视频流信息
@return  成功返回播放器句柄, 失败返回CSUDI_NULL
*/
CSUDI_HANDLE LivePlayStart(CSUDIPlayerChnl_S* psPlayerChnl, CSUDIStreamInfo_S* psAudioStream, CSUDIStreamInfo_S* psVideoStream);

/**@brief 停止直播

@param[in] hPlayer 播放器句柄
@param[in] nDemuxId 用于播放的demuxId
@return  成功返回CSUDI_TRUE, 失败返回CSUDI_FALSE
*/
CSUDI_BOOL LivePlayStop(CSUDI_HANDLE hPlayer, int nDemuxId);

/**@brief 获取录制文件的总大小

@param[in] pcFileName 录制文件名称(可能是文件或文件夹)
@return  成功返回大于0的文件总大小, 失败返回0
@note 如果pcFileName为文件夹, 则认为该文件夹下面全部为文件没有文件夹, 以后遇到第二种情况再优化该接口的实现
*/
unsigned long GetRecFileSize(const char* pcFileName);

/**@brief 检查录制结果(录制文件是否存在、录制的文件能否播放)

@param[in] sRecStartPar 录制启动参数信息
@return  成功返回CSUDI_TRUE, 失败返回CSUDI_FALSE
@note 只适用于单路节目, 即TS录制的文件不适合用本接口检查
*/
CSUDI_BOOL CheckRecResult(CSUDIRecStartParam_S sRecStartPar);

/**@brief 清除文件(夹)

@param[in] pcFileName 文件(夹)名称
@note 如果pcFileName为文件夹, 则认为该文件夹下面全部为文件没有文件夹, 以后遇到第二种情况再优化该接口的实现
*/
void ClearFile(const char* pcFileName);

/**@brief 准备测试用的文件

@param[in] eRecType 录制类型
@param[in] pcFileName 测试文件名称
@param[in] psServiceInfo 录制节目信息
@return 如果文件已经存在直接返回CSUDI_TRUE; 如果文件不存在重新录制, 录制成功返回CSUDI_TRUE, 否则返回CSUDI_FALSE
*/
CSUDI_BOOL PreparePlayTestFile(CSUDIRecType_E eRecType, const char* pcFileName, RecordTestServiceInfo_S* psServiceInfo);


#ifdef __cplusplus
}
#endif

#endif/*_CS_RECORD_TEST_H_*/ 

