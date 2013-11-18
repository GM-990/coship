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

/**@brief �����õ���������Ϣ*/
typedef struct 
{	
	const char*				m_pcTsFilename;		///<�����ļ�����
	const char*				m_pcServiceDescript;	///<��������
	int						m_nVideoPid;           	///< ��ƵPID��-1��ʾ������
	CSUDIVIDStreamType_E 	m_eVidStreamType;	///<��Ƶ����
	int						m_nAudioPid;           	///< ��ƵPID ��-1��ʾ������
	CSUDIAUDStreamType_E	m_eAudStreamType;	///<��Ƶ����
	int						m_nPcrPid;			///< PCR����PID��-1��ʾ������
	int 				 		m_nSubPid;			///<SUBTITLE���͵�PID��-1��ʾ������
	int 				 		m_nTelPid;			///<TELETEXT���͵�PID��-1��ʾ������
}RecordTestServiceInfo_S;

/**@brief ����һ��֧��¼�Ƶ�demux

@return  �ɹ�����֧��¼�Ƶ�demuxId, ʧ�ܷ���-1
*/
int FindDemuxSupportRec(void);

/**@brief ��������֧��¼�Ƶ�demux

@param[out] pOutDemuxArray  ����֧��¼�Ƶ�demuxId����
@param[in] nArraySize ����pOutDemuxArray����ĳ���
@param[out] pnActualCnt ֧��¼�Ƶ�demux��ʵ�ʸ���
@return  �ɹ�����CSUDI_TRUE, ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL FindAllDemuxSupportRec(int* pOutDemuxArray, int nArraySize, int* pnActualCnt);

/**@brief ����һ����֧��¼�Ƶ�demux

@return  �ɹ����ز�֧��¼�Ƶ�demuxId, ʧ�ܷ���-1
*/
int FindDemuxNotSupportRec(void);

/**@brief ����һ��֧��PVR¼���ļ���ʱ���ļ����ŵ�demux

@return  �ɹ�����֧��PVR¼���ļ���ʱ���ļ����ŵ�demuxId, ʧ�ܷ���-1
*/
int FindDemuxSupportPvrPlay(void);

/**@brief ����һ��֧��ֱ�����ŵ�demux

@return  �ɹ�����֧��ֱ�����ŵ�demuxId, ʧ�ܷ���-1
*/
int FindDemuxSupportLivePlay(void);

/**@brief ����֧��ĳ����Ƶ���͵���Ƶ������

@param[in] eAudioType ��Ƶ����, �μ�CSUDIAUDStreamType_E
@return  �ɹ�����֧�ָ�����Ƶ���͵�AudioId, ʧ�ܷ���-1
*/
int FindAudioDecoder(CSUDIAUDStreamType_E eAudioType);

/**@brief ����֧��ĳ����Ƶ���͵���Ƶ������

@param[in] eVideoType ��Ƶ����, �μ�CSUDIVIDStreamType_E
@return  �ɹ�����֧�ָ�����Ƶ���͵�VideoId, ʧ�ܷ���-1
*/
int FindVideoDecoder(CSUDIVIDStreamType_E eVideoType);

/**@brief ��ʼֱ��

@param[in] psPlayerChnl ����ͨ����Դ��Ϣ
@param[in] psAudioStream ��Ƶ����Ϣ
@param[in] psVideoStream ��Ƶ����Ϣ
@return  �ɹ����ز��������, ʧ�ܷ���CSUDI_NULL
*/
CSUDI_HANDLE LivePlayStart(CSUDIPlayerChnl_S* psPlayerChnl, CSUDIStreamInfo_S* psAudioStream, CSUDIStreamInfo_S* psVideoStream);

/**@brief ֱֹͣ��

@param[in] hPlayer ���������
@param[in] nDemuxId ���ڲ��ŵ�demuxId
@return  �ɹ�����CSUDI_TRUE, ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL LivePlayStop(CSUDI_HANDLE hPlayer, int nDemuxId);

/**@brief ��ȡ¼���ļ����ܴ�С

@param[in] pcFileName ¼���ļ�����(�������ļ����ļ���)
@return  �ɹ����ش���0���ļ��ܴ�С, ʧ�ܷ���0
@note ���pcFileNameΪ�ļ���, ����Ϊ���ļ�������ȫ��Ϊ�ļ�û���ļ���, �Ժ������ڶ���������Ż��ýӿڵ�ʵ��
*/
unsigned long GetRecFileSize(const char* pcFileName);

/**@brief ���¼�ƽ��(¼���ļ��Ƿ���ڡ�¼�Ƶ��ļ��ܷ񲥷�)

@param[in] sRecStartPar ¼������������Ϣ
@return  �ɹ�����CSUDI_TRUE, ʧ�ܷ���CSUDI_FALSE
@note ֻ�����ڵ�·��Ŀ, ��TS¼�Ƶ��ļ����ʺ��ñ��ӿڼ��
*/
CSUDI_BOOL CheckRecResult(CSUDIRecStartParam_S sRecStartPar);

/**@brief ����ļ�(��)

@param[in] pcFileName �ļ�(��)����
@note ���pcFileNameΪ�ļ���, ����Ϊ���ļ�������ȫ��Ϊ�ļ�û���ļ���, �Ժ������ڶ���������Ż��ýӿڵ�ʵ��
*/
void ClearFile(const char* pcFileName);

/**@brief ׼�������õ��ļ�

@param[in] eRecType ¼������
@param[in] pcFileName �����ļ�����
@param[in] psServiceInfo ¼�ƽ�Ŀ��Ϣ
@return ����ļ��Ѿ�����ֱ�ӷ���CSUDI_TRUE; ����ļ�����������¼��, ¼�Ƴɹ�����CSUDI_TRUE, ���򷵻�CSUDI_FALSE
*/
CSUDI_BOOL PreparePlayTestFile(CSUDIRecType_E eRecType, const char* pcFileName, RecordTestServiceInfo_S* psServiceInfo);


#ifdef __cplusplus
}
#endif

#endif/*_CS_RECORD_TEST_H_*/ 

