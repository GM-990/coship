/**@defgroup PLAYER  PLAYER ������ƽ̨�޹ص�����Ƶ���ſ��ƽӿ�
@brief PLAYER��INJECT��RECORD����ģ��ֱ��Ӧͷ�ļ�<udi2_player.h>��<udi2_inject.h>��<udi2_record.h>���������ö����豸��ͬ�ṩ���š�ע�롢¼�Ʒ���ĳ�����Щ����֮�䣬ͨ���豸id���й���������һ��ע�����͵Ĳ��ź�һ��TS�����͵�ע�����Ҫ�����������һ��TS��ע�벥�Ź�����Ҳֻ�������������������������������������߻�ʹ��ͬһ��Demux��ͨ�����Demux��������MMCP��CloudTV ���м��ƽ̨������ǹ������� ��

���潫ͨ��һЩ�����ĳ�������������ģ���ʹ�÷�ʽ�����������������ģ���ʵ�֡�

- DVBֱ��
ѡȡһ��ӵ��EM_UDI_DEMUX_PLAY��EM_UDI_DEMUX_DMA������Demux��
���Ŵ��ڶ�Ӧ�Ľ�����(�����ڲ���Ϊ0��pip����Ϊ1)��
���õ�Audio������(һ�����PCM������������ֻ����һ��)��
����һ��EM_UDIPLAYER_LIVE���͵�Player���в��š�

- I֡����
ѡȡһ��ӵ��EM_UDI_VID_STREAM_MPEG2������Video��������ѡ����Ϊ-1)
��������ΪEM_INJECTER_ESע������ΪEM_UDIINJECTER_CONTENT_IFRAME��Inject
ͨ������EM_UDIINJECTER_IFRAME_PARAMS��ʽ������ע��
Ȼ���ø�Video����Player���в���

- TSע�루IPTV��
ѡȡһ��ӵ��EM_UDI_DEMUX_INJECT��EM_UDI_DEMUX_PLAY������Demux������Inject��
ѡȡ��Demux����ӦVideo��Audio(�μ�DVBֱ��)������һ��EM_UDIPLAYER_INJECT�͵�Player���в��š�

- PCMע�룺
ѡȡһ��Audio(һ����˵�����һ��ֻ�ܽ�PCM�ģ�����֧�ֻ�����ֻ��Ψһһ��Audio)������Inject��
��ͬһ��Audio����һ��EM_UDIPLAYER_INJECT�͵�Player���в��š�

- ESע��
�ֱ���һ��Audio��Video��������Inject���������Ǵ���һ��Player���в��š�

- ����AVI�ļ�ע�벥��
ѡȡһ����֧��EM_UDI_DEMUX_INJECT_AVI������ӵ��EM_UDI_DEMUX_INJECT��EM_UDI_DEMUX_PLAY
������Demux������Inject�����������ȡ������ע�뵽��Inject�С�
ѡȡ��Demux����ӦVideo��Audio����һ��EM_UDIPLAYER_INJECT����Player������CSUDIPLAYERSetStream
ΪUNKNOWN������Ƶ�������ûطŲ������в��š�

- ����AVI�ļ�ע�벥��
ѡȡһ����֧��EM_UDI_DEMUX_INJECT_AVI������ӵ��EM_UDI_DEMUX_INJECT��EM_UDI_DEMUX_PLAY
������Demux��ѡȡ��Demux����ӦVideo��Audio����һ��EM_UDIPLAYER_FILE���͵�Player����������Inject��
����CSUDIPLAYERSetStreamΪUNKNOWN������Ƶ�������ûطŲ���Ϊ��ӦAVI�ļ���UDI_FS·�����в��š�

-UDI �ļ����ţ����������ļ��������ļ���

(1)�м�����ȵ���CSUDIPLAYERProbe�ӿڣ�����URL����Ƿ�֧�ָø�ʽ��
(2)��֧�֣�����CSUDIPLAYEROpen(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer)�ӿ�ʱ��
psPlayerChnl���ա�ePlayerType��EM_UDIPLAYER_FILE��
(3)����CSUDIPLAYERSetStream(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam)
�ӿ�ʱ��psStreamInfo��nStreamCnt��0��psPlaybackParam->m_szFileName����URL��file��http����
(4)����CSUDIPLAYERStart�ӿڲ��š�
(5)Pause Resume Seek GetCurrentTime�ȿ��ƽӿ������ж�����ͬ��
(6)�ļ�����ʹ�õĻص����ͺ��¼����ͷֱ�ΪCSUDIFilePLAYERCallback_F��CSUDIFilePlayerEventType_E
(7)����CSUDIPLAYERGetFileInfo �ӿڻ�ȡ���ŵ��ļ���Ϣ
(8)����CSUDIPLAYERSetFilePlayStream �ӿ������ļ����ŵ����ԣ�����programid, videoid, audioid, subtitleid

-PVR ¼���ļ�����
(1)ѡȡһ��֧��EM_UDI_DEMUX_INJECT_TS������ӵ��EM_UDI_DEMUX_PLAY��EM_UDI_DEMUX_INJECT������Demux��
(2)����CSUDIPLAYEROpen(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer)�ӿ�ʱ��
psPlayerChnl����ѡ����Demux����Ӧ��Audio��Video��ePlayerType��EM_UDIPLAYER_PVR��
(3)����CSUDIPLAYERSetStream(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam)
�ӿ�ʱ��psStreamInfo��������Ƶ�����Ϣ��nStreamCnt�����Ӧ��count��psPlaybackParam->m_szFileName����PVR¼���ļ�·����
(4)����CSUDIPLAYERStart�ӿڲ��š�
(5)Pause Resume Seek GetCurrentTime�ȿ��ƽӿ������ж�����ͬ��

-PVR ¼��ʱ�Ʋ���
(1)ѡȡһ��֧��EM_UDI_DEMUX_INJECT_TS������ӵ��EM_UDI_DEMUX_PLAY��EM_UDI_DEMUX_INJECT������Demux��
(2)����CSUDIPLAYEROpen(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer)�ӿ�ʱ��
psPlayerChnl����ѡ����Demux����Ӧ��Audio��Video��ePlayerType��EM_UDIPLAYER_TSHIFT��
(3)����CSUDIPLAYERSetStream(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam)
�ӿ�ʱ��psStreamInfo��������Ƶ�����Ϣ��nStreamCnt�����Ӧ��count��psPlaybackParam->m_szFileName����PVRʱ���ļ�·����
(4)����CSUDIPLAYERStart�ӿڲ��š�
(5)Pause Resume Seek GetCurrentTime�ȿ��ƽӿ������ж�����ͬ��


@note �������ر�ע��:
-ע�벥�ű��뱣֤������ӿڵ���˳�������������
1.��ʼע�벥�ţ� CSUDIINJECTEROpen -> CSUDIPLAYEROpen -> CSUDIINJECTERStart -> CSUDIPLAYERStart
2.ֹͣע�벥�ţ� CSUDIPLAYERStop ->	CSUDIINJECTERStop -> CSUDIPLAYERClose -> CSUDIINJECTERClose
- ����PlayerʱӦ�ž����������豸���粥��I֡ʱ���ô���Audio
- �ش�ı�������Open������һ�������Open�ӿ��У���
- ���Ըı�ֻ��Set���ɣ�����һ�������Set�ӿ��С�
- ԭ���ϲ�����ͨ��Set�ӿڸı�������ͣ��粻����ͨ��Set�ӿڽ�һ��PCMע���Inject�ı�ΪTS��Inject����Ϊ�������漰���豸��ȫ��ͬ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.3 2010/11/24 ����ע�벥��˳������ע��
@version 2.0.2 2010/11/1 ���Żص�֪ͨ����������EM_UDIPLAYER_END_OF_STREAM��EM_UDIPLAYER_BEGIN_OF_STREAM
@version 2.0.2 2009/8/26 ���Ӵ������ע�ͺ���ƵԴ�ı��¼�����
@version 2.0.1 2009/8/17
@{
*/

#ifndef _UDI2_PLAYER_H_
#define _UDI2_PLAYER_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_record.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define EM_UDIFILEPLAYER_ERR_BASE        (1<<16) ///<�ļ����Ŵ������ͻ���ֵ
#define EM_UDIFILEPLAYER_STATE_BASE    (2<<16)	///<�ļ�����״̬���ͻ���ֵ

/**@brief ������ش������ֵ*/
enum
{
	CSUDIPLAYER_ERROR_BAD_PARAMETER = CSUDI_PLAYER_ERROR_BASE, ///< ��������
	CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED,  ///< ������֧��
	CSUDIPLAYER_ERROR_UNKNOWN_ERROR,		  ///< һ�����
	CSUDIPLAYER_ERROR_NO_MEMORY,			  ///< �޿����ڴ�
	CSUDIPLAYER_ERROR_INVALID_DEVICE_ID,	  ///< �Ƿ����豸ID��
	CSUDIPLAYER_ERROR_INVALID_HANDLE,		  ///< �Ƿ����
	CSUDIPLAYER_ERROR_INVALID_STATUS,	      ///< �Ƿ�״̬������������û��stopʱ����ModifyStream��������stopʱ����pause
	CSUDIPLAYER_ERROR_DEVICE_BUSY,			  ///< �豸��æ
	CSUDIPLAYER_ERROR_DEVICE_DISORDER,        ///< ������豸ID�������˴�����豸ID
	CSUDIPLAYER_ERROR_CALLBACK_FULL,          ///<ע��Ļص��Ѿ��ﵽ���ֵ
	CSUDIPLAYER_ERROR_CALLBACK_EXIST,         ///< Ҫע��Ļص��Ѿ�����
	CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST,     ///< Ҫɾ���Ļص�������
	CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER,    ///< ����Ƶ���ͻ��ң�����˵��������Ƶ��������Ƶ
	CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR   ///< TRICKMODE״̬���󣬽������Ѿ���TrickModeʱ�ٴν���TrickMode�����Ѿ��ڷ�TrickModeʱ�ٴ��˳�TrickMode
};

/**@brief ��������ö�ٶ���*/
typedef enum 
{
	EM_UDIPLAYER_SPEED_NORMAL= 0,          	///< 1
	EM_UDIPLAYER_SPEED_FASTREWIND_32,	   	///< -32		
	EM_UDIPLAYER_SPEED_FASTREWIND_16,		///< -16	
	EM_UDIPLAYER_SPEED_FASTREWIND_8,		///< -8
	EM_UDIPLAYER_SPEED_FASTREWIND_4,		///< -4 	
	EM_UDIPLAYER_SPEED_FASTREWIND_2,		///< -2	
	EM_UDIPLAYER_SPEED_SLOWREWIND_2,	   	///< -1/2		
	EM_UDIPLAYER_SPEED_SLOWREWIND_4,	   	///< -1/4		
  	EM_UDIPLAYER_SPEED_SLOWREWIND_8,	   	///< -1/8		
  	EM_UDIPLAYER_SPEED_SLOWFORWARD_2,		///< 1/2
  	EM_UDIPLAYER_SPEED_SLOWFORWARD_4,		///< 1/4
  	EM_UDIPLAYER_SPEED_SLOWFORWARD_8,	 	///< 1/8
	EM_UDIPLAYER_SPEED_FASTFORWARD_2, 		///< 2
	EM_UDIPLAYER_SPEED_FASTFORWARD_4,     	///< 4
	EM_UDIPLAYER_SPEED_FASTFORWARD_8,		///< 8
	EM_UDIPLAYER_SPEED_FASTFORWARD_16, 	  	///< 16	
  	EM_UDIPLAYER_SPEED_FASTFORWARD_32,		///< 32	
	EM_UDIPLAYER_SPEED_MAX
}CSUDIPlayerSpeed_E;	

/**@brief ����ģʽö�ٶ���*/
typedef enum
{
	EM_UDIPLAYER_LIVE,			///< ֱ��ģʽ
	EM_UDIPLAYER_FILE,			///< �ļ�����ģʽ�����������ļ��������ļ�
	EM_UDIPLAYER_TSHIFT,		///< PVR¼��ʱ�Ʋ���ģʽ
	EM_UDIPLAYER_INJECT,		///< ����ע�벥��ģʽ
	EM_UDIPLAYER_PVR,			///< PVR¼�Ʋ���ģʽ
	EM_UDIPLAYER_MAXPLAYERTYPE  ///< �������ֵ����������Ϊ������
}CSUDIPLAYERType_E;

/**@brief �طŲ���ģʽ(EM_UDIPLAYER_FILE��EM_UDIPLAYER_PVR��EM_UDIPLAYER_TSHIFT)�µĲ������ݽṹ*/
typedef struct 
{
	int                         m_nSecondPos;    ///< �طŵ���ʼλ��
	CSUDIPlayerSpeed_E 			m_eSpeed;           ///< �طŵ��ٶ�
	char                        m_szFileName[CSUDI_MAX_URL_LEN];     	///< �طŵ����ļ���·��,ֻ֧��ASCII�����ʽ
    CSUDIStreamEncryptParam_S   m_stEncryptParam; ///<�μ�CSUDIStreamEncryptParam_S
}CSUDIPlaybackParam_S;

/**@brief ����ͨ���豸��Դ���ݽṹ����*/
typedef struct 
{
	int m_nDemux;			///< Demux�豸��Index��CSUDI_INVALID_INDEX��������Demux�豸
	int m_nVideoDecoder;	///< ��Ƶ���������豸Index��CSUDI_INVALID_INDEX����������Ƶ������
	int m_nAudioDecoder;	///< ��Ƶ���������豸Index��CSUDI_INVALID_INDEX����������Ƶ������
}CSUDIPlayerChnl_S;

/**@brief �ط�ƫ����ʼλ��ö�ٶ���*/
typedef enum
{
	EM_UDIPLAYER_POSITION_FROM_HEAD = 0,  		///< ���ļ�ͷ��ʼ����ƫ��
	EM_UDIPLAYER_POSITION_FROM_CURRENT,		///< �ӵ�ǰλ�ü���ƫ��
	EM_UDIPLAYER_POSITION_FROM_END			///< ���ļ�β����ƫ��
}CSUDIPlayPosition_E;

/**@brief �Ǳ����ļ�����ʱ������Ƶ�����¼�ö�ٶ���*/
typedef enum 
{
	
	EM_UDIPLAYER_VIDEO_FRAME_COMING,		///< video��֡�¼� ���¼�Ӧ�ñ�֤����Ƶ��������������һֱ���Լ������������ǲ��Ž�Ŀ��ʼ����ʱֻ������1��
	EM_UDIPLAYER_AUDIO_FRAME_COMING,		///< audio��֡�¼�
	
	EM_UDIPLAYER_VIDEO_UNDERFLOW,		    ///< ��Ƶ�����������¼�
	EM_UDIPLAYER_AUDIO_UNDERFLOW,		    ///< ��Ƶ�����������¼�  
	
	EM_UDIPLAYER_VIDEO_OVERFLOW,		    ///< ��Ƶ�����������¼�
	EM_UDIPLAYER_AUDIO_OVERFLOW,		    ///< ��Ƶ�����������¼�

	EM_UDIPLAYER_VIDEO_SOURCE_CHANGE,		///< ��ƵԴ�ı��¼�������width,height,framerate�����ı�
	EM_UDIPLAYER_AUDIO_SOURCE_CHANGE,		///< ��ƵԴ�ı��¼�������codec�ı�
	
	EM_UDIPLAYER_END_OF_STREAM,				///< �������ļ�ĩβ����������ģʽΪEM_UDIPLAYER_FILE��EM_UDIPLAYER_PVR����EM_UDIPLAYER_TSHIFTʱ�Ż��и��¼�����
	EM_UDIPLAYER_BEGIN_OF_STREAM,			///< �������ļ���ͷ, ��������ģʽΪEM_UDIPLAYER_FILE��EM_UDIPLAYER_PVR����EM_UDIPLAYER_TSHIFT�ҿ��˻�����ʱ�Ż��и��¼�����

	EM_UDIPLAYER_MAXEVENTTYPE				///< �߽�ֵ
}CSUDIPlayerEventType_E; 

/**@brief �����ļ����ţ�����Ƶ�����¼�ö�ٶ��壬ͨ��CSUDIFilePLAYERCallback_F �ص�������ֵ*/
typedef enum 
{
	
	EM_UDIFILEPLAYER_VIDEO_FRAME_COMING,		///< video��֡�¼� ���¼�Ӧ�ñ�֤����Ƶ��������������һֱ���Լ������������ǲ��Ž�Ŀ��ʼ����ʱֻ������1��
	EM_UDIFILEPLAYER_AUDIO_FRAME_COMING,		///< audio��֡�¼�
	
	EM_UDIFILEPLAYER_VIDEO_UNDERFLOW,		    ///< ��Ƶ�����������¼�
	EM_UDIFILEPLAYER_AUDIO_UNDERFLOW,		    ///< ��Ƶ�����������¼�  
	
	EM_UDIFILEPLAYER_VIDEO_OVERFLOW,		    ///< ��Ƶ�����������¼�
	EM_UDIFILEPLAYER_AUDIO_OVERFLOW,		    ///< ��Ƶ�����������¼�

	EM_UDIFILEPLAYER_VIDEO_SOURCE_CHANGE,		///< ��ƵԴ�ı��¼�������width,height,framerate�����ı�
	EM_UDIFILEPLAYER_AUDIO_SOURCE_CHANGE,		///< ��ƵԴ�ı��¼�������codec�ı�
	
	EM_UDIFILEPLAYER_END_OF_STREAM,				///< �������ļ�ĩβ����������ģʽΪEM_UDIPLAYER_FILEʱ�Ż��и��¼�����
	EM_UDIFILEPLAYER_BEGIN_OF_STREAM,			///< �������ļ���ͷ, ��������ģʽΪEM_UDIPLAYER_FILE�ҿ��˻�����ʱ�Ż��и��¼�����

	EM_UDIFILEPLAYER_SHOW_SUBTITLE,             ///< ��ʾ��Ļ��pvEventDataΪconst char* ��ʾҪ��ʾ����ĻASCII������ַ���
	EM_UDIFILEPLAYER_CLEAR_SUBTITLE,            ///< �����ǰ��ʾ����Ļ

	EM_UDIFILEPLAYER_ERR_ABORTED = EM_UDIFILEPLAYER_ERR_BASE,                           ///< �û���ֹ��������ֹ�ɹ���Ϣ
	EM_UDIFILEPLAYER_ERR_DECODE,                                ///< ����������
	EM_UDIFILEPLAYER_ERR_FORMAT,                                ///< ý���ļ������ڻ��ļ���ʽ����
	EM_UDIFILEPLAYER_ERR_SEEK,                         ///< ���Ŷ�λ����ʱ����һ����Ϣ
	EM_UDIFILEPLAYER_ERR_PAUSE,                      ///< ��ͣ����ʧ��ʱ����һ����Ϣ
	EM_UDIFILEPLAYER_ERR_RESUME,                   ///< ״̬�ָ�ʧ��ʱ����һ����Ϣ
	EM_UDIFILEPLAYER_ERR_SETSPEED,                ///< ��������ʧ��ʱ����һ����Ϣ
	EM_UDIFILEPLAYER_ERR_NETWORK,                               ///< �����쳣��m_pvDataΪint��ֵΪHTTP ��׼error code, ����http����Ϊ 200, pvEventDataֵΪ200��
	EM_UDIFILEPLAYER_ERR_TIMEOUT,                    ///< ��ȡ���ݳ�ʱ 

	EM_UDIPFILELAYER_STATE_LOADING = EM_UDIFILEPLAYER_STATE_BASE,                             ///< ���濪ʼ,ÿ�ν��뻺��״̬��Ҫ��һ��
	EM_UDIPFILELAYER_STATE_LOADED,            ///< �������,���㹻���ݿ�ʼ����
	EM_UDIPFILELAYER_STATE_HAVEMETADATA,                        ///< �ܹ���ȡһЩ������Ϣ����Ƭ��
	EM_UDIPFILELAYER_STATE_ENOUGHDATA_FORPLAY,            ///< ���㹻�����ݽ��в��ţ�����п�����˶�����
	EM_UDIPFILELAYER_STATE_DURATIONCHANGE,                       ///< ��Ŀ��ʱ�������仯����Ҫ����������ʱ��������ͨ��CSUDIPLAYERGetDuration ��ȡ�µ���ʱ��
	EM_UDIPFILELAYER_STATE_RATECHANGE,                           ///< ��Ŀ�ܲ��ű��������仯������ͨ��CSUDIPLAYERGetSpeed ��ȡ�µĲ�������

	EM_UDIFILEPLAYER_STATE_LOADING_PROGRESS,	///< �ȴ����ţ��������ݵĽ���ֵ������ֵ�ͻ�ֵ��������m_pvDataΪint, �ٷ��ƣ����绺�嵽25%,  pvEventDataֵΪ25��
	EM_UDIFILEPLAYER_STATE_LOADING_DOWNLOADRATE,	///< ��������, pvEventDataΪint, ��λ:kbytes/s, �������������ٶ�25kbits/s, pvDataֵΪ25��
	
	EM_UDIFILEPLAYER_STATE_STOP,                 ///< ֹͣ�����������ɹ�
	EM_UDIFILEPLAYER_STATE_START,                ///< ���������������ɹ�
	EM_UDIFILEPLAYER_STATE_PAUSE,                ///< ��ͣ�����������ɹ�
	EM_UDIFILEPLAYER_SEEK_FINISH,                 ///< ѡʱ���Ŷ�λ�ɹ�

	EM_UDIFILEPLAYER_MAXEVENTTYPE				///< �߽�ֵ
}CSUDIFilePlayerEventType_E; 

/**@brief ����ͬ��ģʽ������*/
typedef enum
{
	EM_UDIPLAYER_AVSYNC_PCR_MASTER,			///< ��PCR��Ϊͬ��ģʽ (Ĭ��״̬)
	EM_UDIPLAYER_AVSYNC_AUDIO_MASTER,		///< ��AUDIO PTS ��Ϊͬ��ģʽ
	EM_UDIPLAYER_AVSYNC_VIDEO_MASTER,		///< ��VIDEO PTS��Ϊͬ��ģʽ
	EM_UDIPLAYER_AVSYNC_MAX_MODE 			///< �������ֵ����������Ϊ������
}CSUDIPLAYERAVSyncMode_E;

#define CSUDI_PLAYER_MAX_FILE_NAME_LEN         (512)///<�ļ����Ƴ���
#define CSUDI_PLAYER_TITLE_MAX_LEN             (512)///<�ļ����⡢���ߵ�����ַ���
#define CSUDI_PLAYER_TIME_LEN                  (8)///<����ֽ���
#define CSUDI_PLAYER_MAX_PROGRAM_NUM           (5)///<����Ŀ��������tsý���ļ�����Ŀ�����������5�����򲥷������ֻ�ܴ洢����ĳ5����Ŀ��Ϣ�����������е�ĳһ��
#define CSUDI_PLAYER_MAX_STREAM_NUM            (5)///< ������ 
#define CSUDI_PLAYER_MAX_LANG_NUM              (5)///<֧�ֵ���������������
#define CSUDI_PLAYER_LANG_LEN                  (4)///<�����ַ���
#define CSUDI_PLAYER_SUB_TITLE_LEN             (8)///<��Ļ���ⳤ��
 
/**@brief �ļ������ͣ����ŵ��ļ����� */
typedef enum
{
	 EM_UDIFILEPLAYER_STREAM_ES = 0x0,	 ///< es���ļ�
	 EM_UDIFILEPLAYER_STREAM_TS, 		 ///< ts���ļ�
	 EM_UDIFILEPLAYER_STREAM_NORMAL, 	 ///< ��ͨ�ļ�����idx,lrc,srt����Ļ�ļ�
	 EM_UDIFILEPLAYER_STREAM_NET,		 ///< �������ļ�
	 EM_UDIFILEPLAYER_STREAM_END
} CSUDIPlayerStreamType_E;

/**@brief ��Ƶ����Ϣ */
typedef struct
{
	 CSUDI_INT32 m_n32StreamIndex;		   ///< ������������ts������ֵΪ��pid
	 CSUDI_UINT32 m_u32Format;			   ///< ��Ƶ�����ʽ��ֵ����ο�::CSUDIVIDStreamType_E ,����ʵ�ֵ�ʱ��Ҫע��ת��
	 CSUDI_UINT16 m_u16Width;			   ///< ��ȣ���λ����
	 CSUDI_UINT16 m_u16Height;			   ///< �߶ȣ���λ����
	 CSUDI_UINT16 m_u16FpsInteger;		   ///< ֡�ʣ���������
	 CSUDI_UINT16 m_u16FpsDecimal;		   ///< ֡�ʣ�С������
	 CSUDI_UINT32 m_u32Bps; 			   ///< ��Ƶ���ʣ�bits/s
	 CSUDI_UINT32 m_u32ExtradataSize;	   ///< ��չ���ݳ���
	 CSUDI_UINT8  *m_pu8Extradata;		   ///< ��չ����
} CSUDIPlayerVidInfo_S;

/**@brief ��Ƶ����Ϣ */
typedef struct
{
	 CSUDI_INT32 m_n32StreamIndex;					  ///< ������������ts������ֵΪ��pid���Ƿ�ֵΪ::HI_FORMAT_INVALID_STREAM_ID
	 CSUDI_UINT32 m_u32Format;						  ///< ��Ƶ�����ʽ��ֵ����ο�::CSUDIAUDStreamType_E ,����ʵ�ֵ�ʱ��Ҫע��ת��
	 CSUDI_UINT32 m_u32Version; 					  ///< ��Ƶ����汾������wma���룬0x160(WMAV1), 0x161 (WMAV2)
	 CSUDI_UINT32 m_u32SampleRate;					  ///< 8000,11025,441000,...
	 CSUDI_UINT16 m_u16BitPerSample;				  ///< ��Ƶÿ����������ռ�ı�������8bit,16bit
	 CSUDI_UINT16 m_u16Channels;					  ///< ������, 1 or 2
	 CSUDI_UINT32 m_u32BlockAlign;					  ///< packet�������ֽ���
	 CSUDI_UINT32 m_u32Bps; 						  ///< ��Ƶ���ʣ�bits/s
	 CSUDI_BOOL m_bBigEndian;					      ///< ��С�ˣ���pcm��ʽ��Ч
	 char m_acAudLang[CSUDI_PLAYER_LANG_LEN];         ///< ��Ƶ������
	 CSUDI_UINT32 m_u32ExtradataSize;				  ///< ��չ���ݳ���
	 CSUDI_UINT8  *m_pu8Extradata;					  ///< ��չ����
} CSUDIPlayerAudInfo_S;

/**@brief ��Ļ�������� */
typedef enum
{
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_TEXT = 0x0,	  ///< �ַ���
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_BMP, 		  ///< bmpͼƬ
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_ASS, 		  ///< ass��Ļ
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_HDMV_PGS,	  ///< pgs��Ļ
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_BMP_SUB, 	  ///< sub��Ļ
	 EM_UDIFILEPLAYER_SUBTITLE_DATA_END
} CSUDIPlayerSubtitleDataType_E;


/**@brief ��Ļ��Ϣ */
typedef struct
{
	 CSUDI_INT32  m_n32StreamIndex; 						    ///< ������������ts������ֵΪ��pid
	 CSUDI_BOOL m_bExtSubTitle; 							    ///< �Ƿ�Ϊ�����Ļ
	 CSUDIPlayerSubtitleDataType_E m_eSubtitileType;	        ///< ��Ļ��ʽ����bmp,string
	 char m_acSubTitleName[CSUDI_PLAYER_SUB_TITLE_LEN];         ///< ��Ļ����
	 CSUDI_UINT16  m_u16OriginalFrameWidth; 					///< ԭʼͼ����
	 CSUDI_UINT16  m_u16OriginalFrameHeight;					///< ԭʼͼ��߶�
	 CSUDI_UINT16  m_u16HorScale;								///< ˮƽ���űȣ�0-100
	 CSUDI_UINT16  m_u16VerScale;								///< ��ֱ���űȣ�0-100
	 CSUDI_UINT32  m_u32ExtradataSize;							///< ��չ���ݳ���
	 CSUDI_UINT8   *m_pu8Extradata; 							///< ��չ����
} CSUDIPlayerSubtitleInfo_S;


/**@brief ��Ŀ��Ϣ������ļ��ȴ�������ĻҲ��������Ļ����������Ļ��Ϣ׷����������Ļ��
 ע: ���ý���ļ�������Ƶ�����轫videstreamindex����ΪHI_FORMAT_INVALID_STREAM_ID */
typedef struct
{
	 CSUDIPlayerVidInfo_S m_stVidStream;								        ///< ��Ƶ����Ϣ
	 CSUDI_UINT32 m_u32AudStreamNum;											///< ��Ƶ������
	 CSUDIPlayerAudInfo_S m_astAudStream[CSUDI_PLAYER_MAX_STREAM_NUM];		    ///< ��Ƶ����Ϣ
	 CSUDI_UINT32 m_u32SubTitleNum; 											///< ��Ļ����
	 CSUDIPlayerSubtitleInfo_S m_astSubTitle[CSUDI_PLAYER_MAX_LANG_NUM];	    ///< ��Ļ��Ϣ
} CSUDIPlayerProgramInfo_S;

/**@brief ý���ļ���Ϣ */
typedef struct
{
	 CSUDIPlayerStreamType_E m_eStreamType; 			        ///< �ļ�������
	 CSUDI_INT64  m_n64FileSize;								///< �ļ���С����λ�ֽ�
	 CSUDI_INT64  m_n64StartTime;								///< �ļ�������ʼʱ�䣬��λms
	 CSUDI_INT64  m_n64Duration;								///< �ļ���ʱ������λms
	 CSUDI_UINT32  m_u32Bps;									///< �ļ����ʣ�bits/s
	 char m_acFileName[CSUDI_PLAYER_MAX_FILE_NAME_LEN];         ///< �ļ�����
	 char m_acAlbum[CSUDI_PLAYER_TITLE_MAX_LEN];		        ///< ר��
	 char m_aczTitle[CSUDI_PLAYER_TITLE_MAX_LEN];		        ///< ����
	 char m_acArtist[CSUDI_PLAYER_TITLE_MAX_LEN];		        ///< �����ң�����
	 char m_acGenre[CSUDI_PLAYER_TITLE_MAX_LEN];		        ///< ���
	 char m_acComments[CSUDI_PLAYER_TITLE_MAX_LEN]; 	        ///< ��ע
	 char m_acTime[CSUDI_PLAYER_TIME_LEN];				        ///< �������
	 CSUDI_UINT32  m_u32ProgramNum; 							///< ʵ�ʽ�Ŀ����
	 CSUDIPlayerProgramInfo_S m_astProgramInfo[CSUDI_PLAYER_MAX_PROGRAM_NUM];	 ///< ��Ŀ��Ϣ
} CSUDIPlayerFileInfo_S;

/**@brief ����Ƶ����ĻID�л����� */
typedef struct
{
    CSUDI_UINT32 m_u32ProgramId;   ///< ��Ŀid��ֵΪ::CSUDIPlayerFileInfo_S�ṹm_astProgramInfo�����±�
    CSUDI_UINT32 m_u32VStreamId;   ///< ��Ƶ��id��ֵ��m_u32ProgramId���
    CSUDI_UINT32 m_u32AStreamId;   ///< ��Ƶ��id��ֵΪ::CSUDIPlayerProgramInfo_S�ṹm_astAudStream�����±�
    CSUDI_UINT32 m_u32SubTitleId;  ///< ��Ļid��ֵΪ::CSUDIPlayerProgramInfo_S�ṹm_astSubTitle�����±�
} CSUDIPlayerFileStreamId_S;

 /**
@brief ���Żص������Ķ���

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[in] eEvent �ص���Ϣ���ͣ����CSUDIPlayerEventType_E����
@param[in] pvUserData  �û���������
*/
typedef  void ( *CSUDIPLAYERCallback_F  )(CSUDI_HANDLE hPlayer,CSUDIPlayerEventType_E eEvent,void * pvUserData);

  /**
@brief �ļ�����״̬�ص���������

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[in] ePlayerEvent �ص���Ϣ���ͣ����CSUDIFilePlayerEventType_E����
@param[in] pvEventData  �ص����ݣ�����CSUDIFilePlayerEventType_E���Ͳ�ͬ��pvEventData����ĸ������Ͷ���ͬ�����CSUDIFilePlayerEventType_E���Ͷ���
@param[in] pvUserData  �û�����
*/
typedef  void ( *CSUDIFilePLAYERCallback_F  )(CSUDI_HANDLE hPlayer, CSUDIFilePlayerEventType_E ePlayerEvent, void *pvEventData, void * pvUserData);

  
/**
@brief UDI Player�Ƿ�֧�ֲ��Ŵ����URLָ����ý���ļ�.
 
@param[in] pcURL ��Ҫ̽���ý���ļ�URL.
@return ֧�ַ���CSUDI_SUCCESS, ��֧�ַ���CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED.
@note �ο�udi2_player.h�ļ��жԵ�����ý�岥����������ص�����.
@note �ýӿ���Ҫ���оƬ�ܹ�ֱ��֧�ֱ���ý�岥�ţ��粻֧����ֱ�ӷ���CSUDIPLAYER_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/	
CSUDI_Error_Code CSUDIPLAYERProbe(const char * pcURL);

 /**
@brief  ע��ָ���¼����͵Ļص�����

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[in] fnPlayerCallback �ص�����ָ�룬���CSUDIPLAYERCallback_F����
@param[in] eEvent �ص��������ͣ����CSUDIPlayerEventType_E
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����֧��ע��Ļص���������Ϊ32���������
@note ������ע��EM_UDIPLAYER_MAXEVENTTYPE���͵Ļص�
@note �ص�����ָ��+�ص���������+�û�����Ψһ��ʶһ���ص�,������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIPLAYER_ERROR_CALLBACK_EXIST

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERAddPlayerCallback(CSUDI_HANDLE hPlayer,CSUDIPLAYERCallback_F fnPlayerCallback,CSUDIPlayerEventType_E eEvent,void * pvUserData );

 /**
@brief ɾ��ָ���¼����͵Ļص�����
@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[in] fnPlayerCallback �ص�����ָ�룬���CSUDIPLAYERCallback_F����
@param[in] eEvent �ص��������ͣ����CSUDIPlayerEventType_E
@param[in] pvUserData ͬCSUDIPLAYERAddPLAYERCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ������CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
@note �����ڻص�������ɾ���ص�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERDelPlayerCallback(CSUDI_HANDLE hPlayer,CSUDIPLAYERCallback_F fnPlayerCallback,CSUDIPlayerEventType_E eEvent, void * pvUserData );

 /**
@brief  ע���ļ������¼����͵Ļص�����

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[in] fnPlayerCallback �ص�����ָ�룬���CSUDIFilePLAYERCallback_F����
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����֧��ע��Ļص���������Ϊ32���������
@note �ص�����ָ��+�û�����Ψһ��ʶһ���ص�,������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIPLAYER_ERROR_CALLBACK_EXIST
@note ֻ����ePlayerTypeΪEM_UDIPLAYER_FILE��hPlayerʱ������ע��˻ص�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERAddFilePlayerCallback(CSUDI_HANDLE hPlayer, CSUDIFilePLAYERCallback_F fnPlayerCallback, void * pvUserData );

 /**
@brief ɾ���ļ������¼����͵Ļص�����

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[in] fnPlayerCallback �ص�����ָ�룬���CSUDIFilePLAYERCallback_F����
@param[in] pvUserData ͬCSUDIPLAYERAddFilePlayerCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ������CSUDIPLAYER_ERROR_CALLBACK_NOT_EXIST
@note �����ڻص�������ɾ���ص�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERDelFilePlayerCallback(CSUDI_HANDLE hPlayer, CSUDIFilePLAYERCallback_F fnPlayerCallback, void * pvUserData );

/**
@brief  ��ȡ���ŵ��ļ���Ϣ

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[out] pstFileInfo ��ȡ���ļ���Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetFileInfo(CSUDI_HANDLE hPlayer, CSUDIPlayerFileInfo_S * pstFileInfo);

/**
@brief  ���ò��ŵ��ļ���Ϣ�������õ��ļ��������Բ���

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[in] pstFileStreamId ��Ҫ���õ��ļ�������Ϣ
@note ͨ��CSUDIPLAYERGetFilePlayInfo ��ȡ��Ҫ���õ���Ϣ������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSetFilePlayStream(CSUDI_HANDLE hPlayer, CSUDIPlayerFileStreamId_S * pstFileStreamId);
 
/**
@brief ����һ��������

@param[in] psPlayerChnl   ������ͨ�������ݽṹָ�룬��μ�CSUDIPlayerChnl_S��
@param[in] ePlayerType   �������������ͣ���μ�CSUDIPLAYERType_E��
@param[out] phPlayer  �����ɹ������ɵĲ��������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note psPlayerChnl �еĳ�Ա�豸��Դ��������ΪCSUDI_INVALID_INDEX��CSUDI_INVALID_INDEX������豸��Դ�����ڻ��߲���������Ҫ���豸ʵ�ֲ��Ų�����
@note ����PlayerʱӦ�ž����������豸���粥��I֡ʱ���ô���Audio
@note �����������������ֱ������һ��Ҫָ��demux ID,������Ƶ����ͬʱΪ-1�����򷵻�CSUDIPLAYER_ERROR_DEVICE_DISORDER
@note �����ʱ����Ƶ���������ڽ���I֡����(����CSUDIOSGDecodeImage�ӿڵ�ʹ��)�����ұ�������Ҫ�����õ��ý���������ýӿ�Ӧ���еȴ�ֱ��I֡�������Ϊֹ
@note �����ֱ����ʽ��Player������Demuxδ���κ�Tuner����,���ܴ����ɹ��������޷����ų��κ�����
@note �����Inject�Ȳ���ҪTuner����ʽ��Player������Demux�Ѿ�������Tuner���򷵻�CSUDIPLAYER_ERROR_DEVICE_BUSY
@see CSUDIOSGDecodeImage

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYEROpen(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer);

/**
@brief ����һ��������

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����һ������������CSUDIPLAYEROpen�ɶ�ʹ�á�
@note һ�������������ٺ��������Դ��һ����������(��ò����������Ļص�����)��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERClose(CSUDI_HANDLE hPlayer);

/**
@brief �û�����Audio/video/pcr/sub/tt��Ŀ��Ϣ

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[in] psStreamInfo �����Ŀ��Ϣ������ṹ�ο�CSUDIStreamInfo_S ���ݽṹ��
@param[in] nStreamCnt Ҫ���õĽ�Ŀ��Ϣ����
@param[in] psPlaybackParam �ļ��ط�/ʱ�Ʋ��Ų�����Ϣ���ò�������ΪCSUDI_NULL,��ʾ����ֱ��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��ͼ������������Ƶ��Ϣʱ����CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER,������ͬʱ������������ƵPID 
@note ���û�����Audio/video/pcr/sub/tt��Ŀ��Ϣ.������μ�CSUDIStreamInfo_S�Ķ��� 
@note ��ͼ���ý�������֧�ֵ�����Ƶ����ʱ����CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER
@note �ýӿ����õ���Ƶ��ʽ�����ģʽ�޹أ��統ǰ����Aout�豸��ΪDECODERģʽ������ǰPlayer�е�Audio�豸
��֧��AC3��Ƶ��BYPASSģʽ��ʹ�ñ��ӿ�����һ��AC3��Ƶʱ���Է��سɹ�����ʵ�������û��������
@note ���Ų����в����ò���Ҫ������PID����һ����AV��Player�����Ź㲥ʱ��������ƵPID
@note �ýӿڼ���ؽӿ��������PID��׼�����жϣ���0,0x10�ȱ�����PSIռ�õ�PID��Ϊ�Ϸ���PID��ʵ��������PID���޶�Ӧ����Ƶ����ʱ���޷����ųɹ����ɡ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSetStream(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam);

/**
@brief �û��õ�Audio/video/pcr/sub/tt��Ŀ��Ϣ

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[out] psStreamInfo �����Ŀ��Ϣ�ĵ�ַ������ṹ�ο�CSUDIStreamInfo_S���ݽṹ��
@param[in,out] pnStreamCnt ��ȡ�Ľ�Ŀ��Ϣ����.�������ʱ����psStreamInfo������Ԫ�ظ������������ΪpsStreamInfo��Ч��ĿԪ�ظ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note 
- �˽ӿ��ڴ�����������Ϳ���ʹ�ã������û�����ý�Ŀ���򷵻�nStreamCnt =0����psStreamInfo�����ݲ��ܹ����ı�
- ʹ�ô˽ӿڻ�ȡ��������Ϣ��CSUDIPLAYERSetStream���õ���Ϣ���Ӧ�������StreamInfo������봫�㹻��(����>=5)����������޷�ȡ����Ҫ����Ϣ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetStream(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S * psStreamInfo,int * pnStreamCnt);

/**
@brief ��ȡ�طŲ�����Ϣ

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[out] psPlaybackParam ����ط���Ϣ������ṹ�ο�CSUDIPlaybackParam_S���ݽṹ��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetPlaybackParam(CSUDI_HANDLE hPlayer, CSUDIPlaybackParam_S * psPlaybackParam);

/**
@brief �û��޸�Audio/video/pcr/sub/tt��Ŀ��Ϣ

@param[in] hPlayer �������������CSUDIPLAYEROpen()���
@param[in] psStreamInfo ���޸ĵĽ�Ŀ��Ϣ�����������μ�CSUDIStreamInfo_S���ݽṹ��
@param[in] nStreamCnt  �޸ĵĽ�Ŀ��Ϣ����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��ͼ������������Ƶ��Ϣʱ����CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER  ,������ͬʱ������������ƵPID  
@note ��ͼ���ý�������֧�ֵ�����Ƶ����ʱ����CSUDIPLAYER_ERROR_STREAMTYPE_DISORDER    
@note �޸Ľ�Ŀ��Ϣ�Ὣǰһ�ε�����ȫ�����ǣ�������ñ��ӿ�ǰ��ͨ��CSUDIPLAYERGetPlaybackParam��ȡ��ǰ״̬���ٽ����޸ġ�
@note �޸ĺ�Ķ������޸�ǰ�Ķ�������һ�¡�����޸�ǰ���ڲ���״̬�����޸ĺ�Ҳ���ڲ���״̬��
@note �ýӿ����õ���Ƶ��ʽ�����ģʽ�޹أ��統ǰ����Aout�豸��ΪDECODERģʽ������ǰPlayer�е�Audio�豸
��֧��AC3��Ƶ��BYPASSģʽ��ʹ�ñ��ӿ�����һ��AC3��Ƶʱ���Է��سɹ�����ʵ�������û��������
@todo ״̬��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERModifyStream(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S * psStreamInfo,int nStreamCnt);

/**
@brief ����һ���������Ĺ��� 

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����CSUDIPLAYERSetInputStream �Ⱥ������õ�����Ϣ�Լ�������ز�������һ��ý������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERStart(CSUDI_HANDLE hPlayer);

/**
@brief ֹͣһ���������Ĺ���

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ֹͣһ���������Ĺ�������CSUDIPLAYERStart�ɶ�ʹ�á�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERStop(CSUDI_HANDLE hPlayer);

/**
@brief ��ͣ��hPlayer��ص�һ���������Ĺ���

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@return 
      - �ɹ�����CSUDI_SUCCESS��
      - ʧ���򷵻ش������ֵ��
      - ��֧�ִ˲�������CSUDI_ERROR_PLAYER_NOTSUPPORT
@note ��ͣ��hPlayer��ص�һ���������Ĺ�����ֻ�����ڲ���ĳһ��Ŀ��������ֻ����Ƶ������Ƶ��ʱ���ò���Ч��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERPause(CSUDI_HANDLE hPlayer);

/**
@brief �ָ���ͣģʽ 

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@return 
- �ɹ�����CSUDI_SUCCESS��
- ʧ���򷵻ش������ֵ��
- ��֧�ִ˲�������CSUDI_ERROR_PLAYER_NOTSUPPORT
@note �ָ���ͣģʽ;��CSUDIPLAYERPause�ɶ�ʹ�á�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERResume(CSUDI_HANDLE hPlayer);

/**
@brief ����������л��������

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@return 
- �ɹ�����CSUDI_SUCCESS��
- ʧ���򷵻ش������ֵ��
- ��֧�ִ˲�������CSUDI_ERROR_PLAYER_NOTSUPPORT
@note ���ӿڲ��ı�Player״̬��������������ݣ������ڲ���״̬���������ݻ����¿�ʼ����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERClear(CSUDI_HANDLE hPlayer);

/**
@brief ���ò�������

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[in] eSpeed Ҫ���õ�����ģʽ,���CSUDIPlayerSpeed_E����
@return 
- �ɹ�����CSUDI_SUCCESS��
- ʧ���򷵻ش������ֵ��
- �ýӿڶ�ֱ����Ŀ��Ч������CSUDI_ERROR_PLAYER_NOTSUPPORT����֧�ִ˲�����
@note �ò����޸ĺ�������Ч

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSetSpeed(CSUDI_HANDLE  hPlayer, CSUDIPlayerSpeed_E eSpeed);

/**
@brief �õ���������

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[out] peSpeed Ҫ�õ�������ģʽ,���CSUDIPlayerSpeed_E����
@return 
- �ɹ�����CSUDI_SUCCESS��
- ʧ���򷵻ش������ֵ��
- �ýӿڶ�ֱ����Ŀ��Ч������CSUDI_ERROR_PLAYER_NOTSUPPORT����֧�ִ˲�����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetSpeed(CSUDI_HANDLE  hPlayer, CSUDIPlayerSpeed_E *peSpeed);

/**
@brief ���ò���λ��

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[in] nPosInSec ƫ����
@param[in] ePlayPosFlag ƫ�Ƶ����λ��
@return 	
         - �ɹ�����CSUDI_SUCCESS��
         - ʧ���򷵻ش������ֵ��
         - ��֧�ִ˲����򷵻�CSUDI_ERROR_PLAYER_NOTSUPPORT
@note ֻ�лط��ļ���Tshift���ſ������ò���λ�ã�Tshift���ŵ���ʼλ��Ϊ��Ч�ɲ��ŵ���ʼλ�ã�������ͼ�CSUDIPLAYERGetCurPosInSec��ע�ͣ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERSeek(CSUDI_HANDLE  hPlayer,const int nPosInSec, const CSUDIPlayPosition_E ePlayPosFlag);

/**
@brief ��ȡ�ط��ļ�/Tshift���ŵĵ�ǰ����λ�õľ���ʱ����

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[out] pnPosInSec  ��ŵ�ǰ����λ��(��λ:��)
@return  
         - �ɹ�����CSUDI_SUCCESS��
         - ʧ���򷵻ش������ֵ��
         - ��֧�ִ˲����򷵻�CSUDI_ERROR_PLAYER_NOTSUPPORT
@note �ڷ�"�ط��ļ�/Tshift����"�������,�ýӿڵ���Ϊδ����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetCurPosInSec(CSUDI_HANDLE  hPlayer, int * pnPosInSec);

/**
@brief ��ȡ��ǰ���ڲ��ű����ļ��Ľ�Ŀ��ʱ��

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[out] pnSeconds ��Ŀ��ʱ��,��λ��
@return  
         - �ɹ�����CSUDI_SUCCESS��
         - ʧ���򷵻ش������ֵ��
         - ��֧�ִ˲����򷵻�CSUDI_ERROR_PLAYER_NOTSUPPORT
         
@note ���ӿ�ֻ���UDI������Ƶ�ļ���Ч,����ʵʱ����������֧�ֵ���Ƶ�ļ�pdwSecondsΪ0������ֵΪCSUDI_ERROR_PLAYER_NOTSUPPORT
      ������ͨ��TS�ļ����������Injectע�����̣�Ҳ���Բ��÷��ؽ�Ŀʱ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetDuration(CSUDI_HANDLE  hPlayer, int *pnSeconds);

/**
@brief ��ȡ��ǰ���ڲ���(��ʾ)������PTS��Ϣ��

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[in] eContentType ������ ���CSUDIContentType_E��ض���
@param[out] psPTS ���ڴ�ŵ�ǰ���ڲ���(��ʾ)������PTSֵ,��ֵ33bit��
@return  
         - �ɹ�����CSUDI_SUCCESS��
         - ʧ���򷵻ش������ֵ��
         - ��֧�ִ˲����򷵻�CSUDI_ERROR_PLAYER_NOTSUPPORT
@note �����PCR��ΪSTC

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetCurPTS(CSUDI_HANDLE  hPlayer, CSUDIContentType_E eContentType, CSUDI_UINT64 *psPTS);

/**
@brief ��ȡ��ǰ���ڲ��Ž�Ŀָ�������͵�packet������

@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[in] eContentType ��ǰ���ڲ��ŵ����������CSUDIContentType_E
@param[out] puPacketCnt ���packet������,ָ����PID���͵İ�����
@return  
         - �ɹ�����CSUDI_SUCCESS��
         - ʧ���򷵻ش������ֵ��
         - ��֧�ִ˲����򷵻�CSUDI_ERROR_PLAYER_NOTSUPPORT
         
@note ���ӿ���Ҫ�����ж�ǰ�������Ƿ�ͣ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERGetPacketCount(CSUDI_HANDLE  hPlayer, CSUDIContentType_E eContentType, unsigned int * puPacketCnt);

/**
@brief �����˳�TrickMode

֪ͨ�������������Ƶ���ܻᰴ�������ٶȽ����������
������VOD����ʱ�Ŀ��˿���ȣ����������Tuner����������Ϊ�������ٶȣ�
ƽ̨������׼���Է�ֹ���Ƚ׶μ����Ž׶ε������ˡ�
@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[in] bTrickMode ΪCSUDI_TRUE��ʾ����TrickMode,ΪCSUDI_FALSE��ʾ�˳�TrickMode
@return
- �ɹ�����CSUDI_SUCCESS��
- ʧ���򷵻ش������ֵ��
- �Ѿ���TrickModeʱ�ٴν���TrickMode�����Ѿ��ڷ�TrickModeʱ�ٴ��˳�TrickMode����CSUDIPLAYER_ERROR_TRICKMODE_STATE_ERROR
@note �ط��ļ���Tshift�������ڿ��������ڵײ㴦����˲���Ҫ���ñ��ӿڡ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERTrickMode(CSUDI_HANDLE hPlayer,CSUDI_BOOL bTrickMode);

/**
@brief �����˳��������ģʽ
@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[in] bQuickOutputMode ΪCSUDI_TRUE��ʾ����QuickOutputMode,ΪCSUDI_FALSE��ʾ�˳�QuickOutputMode
@return
	- �ɹ�����CSUDI_SUCCESS��
	- ʧ���򷵻�CSUDI_FAILURE��
@note ����������ģʽ����������Ҫ�����⴦��
          ����: 1���رո����㷨
                2�������ʱ���ж���
                3��������˳�����
@note �ýӿ���Ҫ���ڶԵ���ʱ�����ر�ߵ�Ӧ�ó���
          ����: 1����������Ϸ������Ƶҵ��
@note �ýӿ���Ҫ�ڲ�����������״̬����
          ����: 1��open֮��start֮ǰ
                2��stop֮��close֮ǰ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIPLAYERQuickOutputMode(CSUDI_HANDLE hPlayer,CSUDI_BOOL bQuickOutputMode);

/**
@brief ����player������Ƶͬ��ģʽ
@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[in] eAVSyncMode Ϊ��Ҫ���õ�ͬ��ģʽ����CSUDIPLAYERAVSyncMode_E
@return
	- �ɹ�����CSUDI_SUCCESS��
	- ʧ���򷵻�CSUDI_FAILURE��
@note �����ʹ�øýӿڣ���ôĬ���ǲ���PCRͬ��ģʽ

@note �ýӿ���Ҫ�ڲ�����������״̬����
          ����: 1��open֮��start֮ǰ
                2��stop֮��close֮ǰ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code  CSUDIPLAYERSetAVSyncMode(CSUDI_HANDLE hPlayer,CSUDIPLAYERAVSyncMode_E eAVSyncMode);

/**
@brief ��ȡplayer������Ƶͬ��ģʽ
@param[in] hPlayer ������������ɺ���CSUDIPLAYEROpen()���
@param[out] peAVSyncMode Ϊ����ͬ��ģʽ��ָ�룬��CSUDIPLAYERAVSyncMode_E
@return
	- �ɹ�����CSUDI_SUCCESS��
	- ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code  CSUDIPLAYERGetAVSyncMode(CSUDI_HANDLE hPlayer,CSUDIPLAYERAVSyncMode_E *peAVSyncMode);


#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

