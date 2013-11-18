/**@defgroup CSAudio audio �ṩͳһ����Ƶ��ؽӿ�,����Ƶ����,��Ƶ������Ե����ü�������Ƶ�¼������Ȳ���
@brief �ṩͳһ����Ƶ��ؽӿ�,����Ƶ����,��Ƶ������Ե����ü�������Ƶ�¼������Ȳ���

@version 0.2 2008/11/04 �����ϸ����
@version 0.1 2008/10/21 ��ʼ�汾
@{
*/
#ifndef _UDI_SYSAUDIO_H_
#define _UDI_SYSAUDIO_H_

#include "udi_typedef.h"
#include "udi_public.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ��Ƶ�������ʽ����ע��������ʽ(TS��AVI��ASF������)*/
typedef enum _CSAUDStreamType
{
	CSAUD_STREAM_AC3,			/**< Dolby Digital AC3 audio */
	CSAUD_STREAM_DTS,			/**< Digital Surround sound. */
	CSAUD_STREAM_MPEG1,			/**< MPEG1/2, layer 1/2. This does not support layer 3 (mp3). */
	CSAUD_STREAM_MPEG2,			/**< MPEG1/2, layer 1/2. This does not support layer 3 (mp3). */
	CSAUD_STREAM_CDDA,			/**< 1979�꣬Philips��Sony��˾�������Ͽ���CD-DA Compact Disc-Digital Audio,���ܹ���������Ƶ����׼*/
	CSAUD_STREAM_PCM,			
	CSAUD_STREAM_LPCM,			
  	CSAUD_STREAM_MP3,			/**< MPEG1/2, layer 3. */
  	CSAUD_STREAM_MPEG_AAC,		/**< Advanced audio coding. Part of MPEG-4 */
  	CSAUD_STREAM_HE_AAC,		/**< AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE) */
  	CSAUD_STREAM_RAW_AAC,	
	CSAUD_STREAM_AAC_PLUS_ADTS, /**< AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format) */
	CSAUD_STREAM_AAC_PLUS_LOAS, /**< AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream) */
	CSAUD_STREAM_AC3_PLUS,		/**< Dolby Digital Plus (AC3+ or DDP) audio */
	CSAUD_STREAM_DTS_HD,		/**< Digital Surround sound, HD */
  	CSAUD_STREAM_WMA_STD,		/**< WMA Standard */
  	CSAUD_STREAM_WMA_PRO,		/**< WMA Professional */ 
 	CSAUD_STREAM_DRA,           /**< Digital Rise Audio */
  	CSAUD_STREAMTYPE_NUM	
}CSAUDStreamType;

/**@brief ��Ƶ�������*/
typedef enum _CSAUDStreamPacketType
{
	CSAUD_STREAM_PACKET_TYPE_PES,
	CSAUD_STREAM_PACKET_TYPE_ES,
  	CSAUD_STREAM_PACKET_TYPE_NUM
}CSAUDStreamPacketType;

/**@brief ��Ƶ������Ϣ*/
typedef struct _CSSYSAUDInputInfo
{
	CSAUDStreamType     StreamType;	    /**< ������:Mpeg2/Mp3/AAC/AC3*/
	int 			    nSampleRate;	/**< ��Ƶ������:44100, 48000, 32000 etc.,0��ʾδ֪*/
	int 			    nPID;		    /**< ��������PID*/
	CSSYSServiceType    ServiceType;    /**< ���ݴ������:PES or ES*/
	int 			    nBitWidth;	    /**< ����λ����8��16,0��ʾδ֪*/
	int 			    nChannel;	    /**< ͨ����:1-mono,2-stereo,0��ʾδ֪*/
}CSSYSAUDInputInfo;


/**@brief ������Ƶ����ӿ�����*/
typedef enum _CSAUDOutputDevice
{
	CSAUD_OUTPUT_RCA	=0x00000001,	/**< ������Ƶ������������ӿ�*/
	CSAUD_OUTPUT_SPDIF	=0x00000002,	/**< ������Ƶ�ӿ�*/
	CSAUD_OUTPUT_RF		=0x00000004,	/**< RF�������*/
	CSAUD_OUTPUT_SCART	=0x00000008,	/**< scart�ӿ�*/
	CSAUD_OUTPUT_HDMI  	=0x00000010	    /**< hdmi �ӿ�*/
}CSAUDOutputDevice;	

/**@brief ����*/
typedef enum _CSAUDChannelType
{
  	CSAUD_CHANNEL_STEREO,			/**< ������*/
  	CSAUD_CHANNEL_LEFT,			    /**< ������*/
  	CSAUD_CHANNEL_RIGHT,			/**< ������*/
  	CSAUD_CHANNEL_MIXED_MONO		/**< �����*/
}CSAUDChannelType;	
 
/**@brief ��Ƶ״̬�ṹ�� */
typedef struct _CSSYSAUDStatus_S
{
	BOOL 			m_bMute;		/**< ������ʱΪtrue*/
	CSAUDStreamType 	m_eAudStreamType;	/**< ��Ƶ������*/
	CSAUDChannelType 	m_eChannelType;		/**< ����*/
	int 			m_nSampleRate;		/**< ��Ƶ������*/
	int 			m_nBitWidth;		/**< ��Ƶ����λ��*/
	DWORD			m_dwFrameCount;		/**< ������֡��*/
	DWORD 			m_dwReserved1;		/**< ��������������0*/
	DWORD			m_dwReserved2;		/**< ��������������0*/
}CSSYSAUDStatus_S;

/**@brief PCMע����������*/
typedef struct _CSPcmStartParam_S
{
	unsigned uBitsSample;	/**< ����λ��:8 or 16*/
	unsigned uSampleRate;	/**< ������:32000,44100,48000*/
	unsigned uChannels;  	/**< ͨ����:1-mono,2-stereo*/
}CSPcmStartParam_S;

/**@brief PCM�ֽ������*/
typedef enum _CSPcmEndian_e
{
	CSPCM_ERROR_ENDIAN,
	CSPCM_DEFAULT_ENDIAN,	/**< Ĭ���ֽ��򣬼�ƽ̨�Ƽ�ʹ�õ��ֽ���*/
	CSPCM_LITTLE_ENDIAN,	/**< С���ֽ���*/
	CSPCM_BIG_ENDIAN	/**< ����ֽ���*/
}CSPcmEndian;

/**@brief PCMע��״̬*/
typedef struct _CSPcmStatusParam_S
{	
	DWORD m_dwQueuedByte;        /**< ע��ռ���ʣ���δ���ŵ������ֽ���*/
	DWORD m_ulFifoSize;          /**< ע��ռ��ܴ�С*/
}CSPcmStatusParam_S;

/**@brief �����Ƶ������Ϣ�ṹ��*/
typedef struct _CSSYSAUDOutputInfo
{
	CSAUDOutputDevice	OutputDevice;	/**< ����ӿ�: AV/spdif/hdmi*/
	BOOL 			    bMute;		    /**< ����Ƿ��� */
	int 			    nVolume;	    /**< �������, Ԥ��  32 */
	CSAUDChannelType	ChannelType;	/**< ������� */
}CSSYSAUDOutputInfo;

/**
@brief �û�����Audio/pcr�Ƚ�Ŀ��Ϣ

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] pstAUDInfo �����Ŀ����Ƶ��Ϣ
@param[in] pstPCRInfo �����Ŀ��PCR��Ϣ�������PCR��Ϣ�������գ�PCRPID�Ƿ��൱����PCR���Կ���������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��������ʱ����������Ƶ�Ľ���
*/
BOOL CSSYSAUDSetInput(HCSHANDLE hHandle, CSSYSAUDInputInfo *pstAUDInfo, CSSYSPCRInputInfo* pstPCRInfo,HCSHANDLE hReserved );

/**
@brief ���õ�ǰ����Ƶ���ݵĴ������

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] ePacketType ����Ƶ���ݴ�����ͣ���ES��PES
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��CSSYSAUDSetInput֮������������֮ǰ���ã���������ʱ��Ч�������������Ĭ��ΪPES��ʽ
*/
BOOL CSSYSAUDSetStreamPacketType(HCSHANDLE hHandle, CSAUDStreamPacketType ePacketType);

/**
@brief ������Ƶ����豸��������ݸ�ʽ��ѹ�����ѹ����ʽ��

��Ҫ��������AC3����Ƶ��ʽ�Ƿ�Ҫ�ڻ���������н��룬����ֱ�������������豸�Ի�ø��õ�Ч��
@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] eDevice ��Ƶ����˿ڣ���ΪCSAUDOutputDevice�и������
@param[in] bCompressed �Ƿ�Ϊѹ����ʽ
@note ���dwOutputDevice�д����κβ�֧�ֻ��߲���ͬʱ֧�ֵ��豸,
�򷵻�FALSE,���Ҳ������κ�����(����eDevice��ָ����֧�ֵ��豸)
@note ��ѹ����ʽʵ���Ͼ���ԭʼ��PCM��Ƶ��ʽ���������κ���ʽ����Ϊ��ѹ����ʽ(��AC3)
@note ��Ϊ�豸һ�㲻�߱����빦�ܣ�����ֻ��ѡ�������߲����룬������ѡ�����ض��ĸ�ʽ���
*/
BOOL CSSYSAUDSetOutputDeviceDataFormat(HCSHANDLE hHandle, CSAUDOutputDevice eDevice, BOOL bCompressed);

/**
@brief ������Ƶ�Ľ��롢���ź����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ����CSSYSAUDsudInput()�����ú���������Ƶpid���Լ���Ƶ��������Բ���������Ƶ�Ĳ���
@note ��Stop״̬���ٴε���Play����TRUE���������¿�ʼ����,�����õ�PidҲ����Ч
*/
BOOL CSSYSAUDPlay(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief ֹͣ��Ƶ�Ĳ��źͽ���

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note Stop״̬���ٴε���Stop����TRUE
*/
BOOL CSSYSAUDStop(HCSHANDLE hHandle,HCSHANDLE hReserved);

/**
@brief ��ͣ��Ƶ�Ĳ��źͽ���

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��ͣ��hHandle��ص���Ƶ�Ĳ��š�ֻ�����ڲ�����Ƶʱ���ò���Ч,�ּ���Pauseʱ�ٴε��÷���TRUE
*/
BOOL CSSYSAUDPause(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief �ָ�����ͣ����Ƶ

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ָ�����ͣ����Ƶ����CSSYSAUDPause�ɶ�ʹ��
@note ��Resume״̬���ٴ�Resume�Է���TRUE
*/
BOOL CSSYSAUDResume(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief ��Ƶ�豸����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���û��趨��ָ����Ƶ�豸��ʵ�־�������
@note ���Զ�ε��ã���ֻ��Unmute��ĵ�һ����Ч
*/
BOOL CSSYSAUDMute(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief ȡ����Ƶ�豸����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] hReserved  ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���û��趨��ָ����Ƶ�豸��ʵ��ȡ�������Ĺ���
@note ���Զ�ε��ã���ֻ��Mute��ĵ�һ����Ч
@note ��ȻӦ���ϴ���ʹ�ã����������������Զ�Unmute
*/
BOOL CSSYSAUDUnmute( HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief ������Ƶ����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] nVolume �û���Ҫ���������ֵ����[0,31]����������,0Ϊ����,31���
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��������豸��ͳһ��������
@note ����������Ч��Χ���ش���,������Ч
*/
BOOL CSSYSAUDSetVolume(HCSHANDLE hHandle, int nVolume, HCSHANDLE hReserved);

/**
@brief ������Ƶ����

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] enmChannelType �û���Ҫ���������, ��LEFT, RIGHT, STEREO
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���û�ָ������Ƶ�豸�ϣ��л�ģ����Ƶ���������
*/
BOOL CSSYSAUDSetChannel(HCSHANDLE hHandle,CSAUDChannelType enmChannelType,HCSHANDLE hReserved);

/**
@brief ��ǰϵͳ��Ƶ��״̬

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[out] pstAUDStatus ָ��CSSYSAUDStatus_S �ṹ��ָ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSYSAUDGetStatus(HCSHANDLE hHandle,CSSYSAUDStatus_S *pstAUDStatus);

/*---------------PCMע����ؽӿ�---------------*/
/**
@brief ��ʼ��PCMע��

ΪPCM��������׼��
@return PCM���ž��
@note �ݲ�֧��ѡ���豸
*/
HCSHANDLE CSPcmPlay_Open(void);

/**
@brief ��ʼ����PCM��Ƶ

@param[in] hPcm PCM���ž������CSPcmPlay_Open����
@param[in] psPcmStart ����ѡ�������ָʾ��ע���PCM���ݵ�����
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL CSPcmPlay_Start(HCSHANDLE hPcm, CSPcmStartParam_S * psPcmStart);

/**
@brief ��ȡPCM��������Buffer

��ȡ���ڴ�Ŵ�����PCM�������ݵĵ��ڴ棬���ȡ�ɹ���Ὣ������Ҫ���ŵ�PCM���ݿ���������
�����ʾ��������������ʱ�������������
�ϲ��費�ϵĵ��øýӿڻ�ȡBuffer���Ա㼰ʱ������������
@param[in] hPcm PCM���ž��
@param[out] ppvBuffer ���ػ�ȡ�����ڴ��ַָ��
@param[out] puLength ���ػ�ȡ�����ڴ泤��
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL CSPcmPlay_GetFreeBuffer(HCSHANDLE hPcm,void ** ppvBuffer, unsigned  * puLength );

/**
@brief ֪ͨ����ע�������

ͨ��CSPcmPlay_GetFreeBuffer��ȡ���ڴ�󣬽����������ݿ��������к�ʹ�ñ�����֪ͨ�ײ�ģ�����ݿ�����ɼ������˶���
���������ú󣬴˴ο��������ݼ������ڲ�����
@param[in] hPcm PCM���ž��
@param[in] uWrittenSize �ѿ������ݵĴ�С
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL CSPcmPlay_WriteComplete(HCSHANDLE hPcm, unsigned uWrittenSize);

/**
@brief ֹͣ����

@param[in] hPcm PCM���ž��
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL CSPcmPlay_Stop(HCSHANDLE hPcm);

/**
   @brief ��ͣ����PCM��Ƶ

   @param[in]hPcm PCM ���ž������CSPcmPlay_Open����
   @return �ɹ�����TRUE��ʧ�ܷ���FALSE
   @note ��Pause״̬���ٴ�Pause��Ȼ����TRUE
*/
BOOL CSPcmPlay_Pause(HCSHANDLE hPcm);

/**
   @brief �ָ��Ѿ���ͣ����PCM��Ƶ

   @param[in]hPcm PCM ���ž������CSPcmPlay_Open����
   @return �ɹ�����TRUE��ʧ�ܷ���FALSE
   @note ��Resume״̬���ٴ�Resume��Ȼ����TRUE
*/
BOOL CSPcmPlay_Resume(HCSHANDLE hPcm);

/**
@brief �ر�PCM����

�ͷ������Դ
@param[in] hPcm PCM���ž��
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL CSPcmPlay_Close(HCSHANDLE hPcm);

/**
@brief �������е�pcm���ݲ�����ȫ

�������������һֱ�ȵ������е����ݲ�������˲ŷ���
һ�����ڴ���������ȫ��ע����Ϻ󣬵ȴ����Ž���
@param[in] hPcm PCM���ž��
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL CSPcmPlay_Flush(HCSHANDLE hPcm);

/**
@brief ���ò�����PCM�ֽ���

�ظ����ý����ز���Ч
@param[in] hPcm PCM���ž��
@param[in] enmEndian Ŀ���ֽ���
- PCM_LITTLE_ENDIAN ����ʹ��С�ֽ���
	- ��ƽ̨��֧��PCM_BIG_ENDIAN�򷵻�PCM_BIG_ENDIAN
	- ��ƽ̨֧��PCM_LITTLE_ENDIAN���򷵻�PCM_LITTLE_ENDIAN
- PCM_BIG_ENDIAN ����ʹ�ô��ֽ���
	- ��ƽ̨��֧��PCM_LITTLE_ENDIAN�򷵻�PCM_LITTLE_ENDIAN
	- ��ƽ̨֧��PCM_BIG_ENDIAN���򷵻�PCM_BIG_ENDIAN
- PCM_DEFAULT_ENDIAN ����ƽ̨�Ƽ�ʹ�õ��ֽ�����Ч�ʸ��ߵ��ֽ���
@return ����ʵ���ֽ���״̬
@note ��Ϊƽ̨��һ��֧�������ֽ�������ʵ�ʷ��ص��ֽ��������õ��ֽ���һ����ͬ
*/
CSPcmEndian CSPcmPlaySetEndian(HCSHANDLE hPcm, CSPcmEndian enmEndian);

/**
@brief ��ȡPCMע���״̬��Ϣ

���ڻ�ȡ"��ȷ"��ע��������λ�ã���Ϊ���ݴ�ע�뵽���Ż���ҪһЩʱ�䣬��Щ���ݾ��������������ʱ���
@param[in] hPcm PCM���ž��
@param[out] psPcmStatus ָ��CSPcmStatusParam_S�ṹ��ָ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ʵ�������ڸ�ģ����²�ģ��������л��棬���ʱ��Ҳ���ܱ�֤100%��ȷ�����������Ѿ�����
*/
BOOL CSPcmPlayGetStatus(HCSHANDLE hPcm, CSPcmStatusParam_S *psPcmStatus);

/**
@brief �����ע�������PCM����

��Ҫ���ڿ�������ˡ���λ�Ȳ���ʱ�����ע������ݣ��ѷ�ֹ��Щ�������ӳ�
@param[in] hPcm PCM���ž��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSPcmPlayClearBuffer(HCSHANDLE hPcm);



/*------------------���½ӿ�ʵ����PVRδ���ã�������ý�岥��ȴ�����õ�--------------*/

/**
@brief ��Ƶģ�������,����,��λģʽ

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()������� 
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSYSAUDTrickModeStart(HCSHANDLE hHandle,HCSHANDLE hReserved);

/**
@brief �˳���Ƶ�Ŀ��,����,��λģʽ

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()������� 
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSYSAUDTrickModeStop(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**
@brief ����Audio�Ĳ����ٶ�

@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] nSpeed  	���ŵ��ٶ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ�ʵ������Ƶ����Trick Modeʱ���õĺ���������֧�ֻطź�ʱ�ơ�ʵʱ�����Ŵ˺�����Ч��
@note 1000Ϊ���ٲ���,2000��ʾ2���٣�500��ʾ�������0Ϊpause,����Ϊ���򲥷�,����-1000��ʾ����һ���١�
*/
BOOL CSSYSAUDSetSpeed(HCSHANDLE hHandle, int nSpeed);



#ifdef  __cplusplus
}
#endif

/** @} */

#endif


