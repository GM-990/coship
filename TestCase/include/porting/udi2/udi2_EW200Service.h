/**@defgroup EW200S [EW200Only]EW200 Service �ṩEW200ר�õ�Ӧ�ò����ӿڣ��Լ�EW210��EW510Ҫ�õ��Ĳ�����֤��ؽӿ�
@brief EW200���ڱ�������DVBӦ�ÿ��Ʋ��֣���๤����ҪӦ�ò㸨����ɣ�����Ը�Ƶͷ��Դ��ռ�ã����ݹ����ѯ����Ŀ���ŵ�
��ģ�鼴����˲��ֽӿ�

@note ��ģ��ӿ�����Eastwin����ϵ�в�Ʒ�Ĳ���UDI�ӿڻ��������ظ�������ݾ��弯�ɵĲ�Ʒ����ʹ���ĸ�ģ��
@note �ϲ�����Ҫ�����ݾ���ʹ�õĲ�Ʒ����ʹ����Щ�ӿڣ�ԭ���ϣ��ϲ����EW200�����ýӿڼ�ר��ģ�飬����ģ������õ��ñ�ģ��ӿ�

@note EW200��Ʒ��ʵ�ֱ�ģ�����нӿڣ�EW210��EW510ֻ��Ҫʵ��CSUDIEW200SReadClientInfo��CSUDIEW200SWriteClientInfo������ֻ��Ҫ֧��CSUDIEW200SClientInfoType_E��EM_UDIEW200S_CARDID~EM_UDIEW200S_VOLUME���Ҫ������֤
@note ��ģ�鶨��Ľӿڸ�ƫ��Ӧ�ò㣬������Ӧ�ò�ʵ��

@version 2.0.0 2009/09/09 ����
@{
*/

#ifndef _UDI2_EW200_SERVICE_H_
#define _UDI2_EW200_SERVICE_H_

#include "udi2_typedef.h"
#include "udi2_tuner.h"
#include "udi2_public.h"
#include "udi2_error.h"
#ifdef __cplusplus
extern "C" {
#endif

/**@brief EW200 Service��ش������ֵ*/
typedef enum
{
	CSUDIEW200S_ERROR_BAD_PARAMETER = CSUDI_EW200S_ERROR_BASE, ///< ��������
	CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED,                   ///< ������֧��
	CSUDIEW200S_ERROR_UNKNOWN_ERROR,		           ///< һ�����
	CSUDIEW200S_ERROR_NO_MEMORY,			           ///< �޿����ڴ�
	CSUDIEW200S_ERROR_NO_CALLBACK,				 ///< û����Ӧ�Ļص���������ɾ��
	CSUDIEW200S_ERROR_ALREADY_ADDED,			///< �ûص��Ѿ���ӹ�
	CSUDIEW200S_ERROR_TRICKMODE_STATE_ERROR   ///< TRICKMODE״̬���󣬽������Ѿ���TrickModeʱ�ٴν���TrickMode�����Ѿ��ڷ�TrickModeʱ�ٴ��˳�TrickMode
}CSUDIEW200S_ERRORCODE_E;


#define CSUDIEW200S_VIDEOCOUNT_PER_PROG 	(30)    ///< ÿ����Ŀ����Ƶ��
#define CSUDIEW200S_AUDIOCOUNT_PER_PROG 	(30)	///< ÿ����Ŀ����Ƶ��
#define CSUDIEW200S_SUBTITLECOUNT_PER_PROG 	(8)	///< ÿ����Ŀ��subtitile��
#define CSUDIEW200S_TELETEXTCOUNT_PER_PROG 	(1)	///< ÿ����Ŀ��teletext��
#define CSUDIEW200S_SUBTITLE_PER_STREAM         (20)	///< ÿ������subtitile��
#define CSUDIEW200S_TELETEXT_PER_STREAM         (20)	///< ÿ������teletext��

/**
@brief ����EW200 Serviceͨ����Ϣ�����ṹ

�������������Ϣ����ȷ��,
����Ϣ�����л���:���=����(����)����ʽ��������
�����ر�˵��,����ָ��ָ�����������Ϣ���غ�����Ч.
*/
typedef struct
{
	void* pPara0;                         ///< ����0,���p0
	void* pPara1;                         ///< ����1,���p1
	void* pPara2;                         ///< ����2,���p2
	void* pPara3;                         ///< ����3,���p3
} CSUDIEW200SParam_S;

/**

@brief ����EW200 Service��Ϣ֪ͨ�ص�����

EW200 Service��ͨ���ûص���ȡ�ײ㲥����ص�һЩ״̬����Ϣ
@param[in] dwMsg ��Ϣ����,����ȡCSEWVODMsgType_E�е�EM_EW_MSG_VOD_CONNECT_SUCCESS��EM_EW_MSG_VOD_CONNECT_FAILED��EM_EW_MSG_VOD_NO_PROGRAME��EM_EW_MSG_VOD_NOT_SUPPORT��EM_EW_MSG_VOD_FRONTTS_STOP,EM_EW_MSG_VOD_FRONTTS_RUN,
@param[in] psParam ��Ϣ����,����������ͼ���Ϣ���Ͷ���
@param[in] pUserData �û�����,���ڻص�ע��ʱ����Ӧ����
@return �ɹ��������Ϣ����1,���򷵻�0
@note �����ر�˵��,MMCP�ᾡ��ӻص������з���
*/
typedef int (*CSUDIEW200SCallback_F)(unsigned int dwMsg, CSUDIEW200SParam_S *psParam, void* pUserData);


/**@brief EW200 ��������Դ�����ṹ*/
typedef struct
{
	CSUDI_UINT32 m_dwTunerId;		///< EW200ʹ�õ�Tuner�豸Id
	int m_nDemuxID;				///< EW200ʹ�õ�Demux�豸Id
	CSUDITunerSRCDeliver_U m_punDeliver;	///< EW200ʹ�õ�Ƶ��
}CSUDIEW200SSourceParam_S;

/**
@brief ����VOD����Ҫ���ն��ṩ����Ϣ����

������Ϣ���������ϸ���������ͼ�����ö����
@note CITV��֤��ص��ֶ���ϸ��ʽ������Ӫ��ָ��,JSExtern����Ϣ��������ҳ���м����Ӧ�ù�ͬ����
@note ����[w]��ʶ���ֶλ����CSUDIEW200SWriteClientInfo����,����[r]��ʶ���ֶλ����CSUDIEW200SReadClientInfo����
*/
typedef enum
{
	//CITV��֤��ص�ö��ֵ
	EM_UDIEW200S_CARDID,          ///< [r]���ܿ�����,char*
	EM_UDIEW200S_SN,              ///< [r]���к�,char*
	EM_UDIEW200S_PROVIDER,        ///< [r]�ն����쳧��,char*
	EM_UDIEW200S_ALIAS,           ///< [r]�ն˱���,char*
	EM_UDIEW200S_MODULE,          ///< [r]�ն��ͺ�,char*
	EM_UDIEW200S_LOCAL_IP,        ///< [r]����IP��ַ,char*,��ȡʱ��ʽΪ"192.168.1.1"	
	EM_UDIEW200S_LOCAL_MAC,       ///< [r]����Mac��ַ,char*,��"00AABBCCDDEE"
	EM_UDIEW200S_CITV_PSD,        ///< [r]CITV VOD�û�����,char*
	EM_UDIEW200S_CITV_USERID,     ///< [w][r]CITV VOD�û���,��һ�λ�ȡ��ȫ0,ͨ����֤��CITV3�ᷴ����ֵ,�뽫��洢��ROM��,�Ա��´�ʹ��,char*
	EM_UDIEW200S_CITV_TOKEN,      ///< [w][r]CITV ����,��һ�λ�ȡ��ȫ0,ͨ����֤��CITV3�ᷴ����ֵ,�뽫��洢��ROM��,�Ա��´�ʹ��,char*
	EM_UDIEW200S_CITV_SERVERIP,   ///< [r]CITV ��֤������IP��ַ,char*
	EM_UDIEW200S_CITV_SERVERPORT, ///< [r]CITV ��֤�������˿ں�,int,��8080
	EM_UDIEW200S_CITV_VIDEOTYPE,  ///< [r]֧������Ƶ�����ʽMPEG-2����:"MPEG-2-SD" ;MPEG-2����:"MPEG-2-HD";H.264����: "H.264-SD";H.264����: "H.264-HD" ; ȱʡΪMPEG-2-SD;ͬһ��ʽ�������¼��ݱ��壬���Բ���������;�ն˿ɽ����ʽ�������ж�����á�/���ָ���
	EM_UDIEW200S_CITV_INMUSICSHOW,///< [w][r]�Ƿ��������ģʽ,"TRUE":����ģʽ "FALSE":������ģʽ.EW200���յ�"ITVRTSPURL"�ؼ���ʱ,��������ģʽ;���յ�"ITVControl"�ؼ���,ֵΪ"ITVStop"ʱ�˳�����ģʽ
	EM_UDIEW200S_CITV_BROWSERVER, ///< [r]������İ汾�ţ�Ӧ�ø���������ǰ��Э��
	EM_UDIEW200S_ISMUTE,          ///< [r][w]����״̬,CSUDI_BOOL,CSUDI_TRUE��ʾ��ǰΪ����״̬,CSUDI_FALSE��ʾΪ�Ǿ���״̬
	EM_UDIEW200S_VOLUME,          ///< [r][w]����,int,[0,31],0Ϊ����,31���

	EM_UDIEW200S_AUDIOCHANNEL,    ///< [w]����,CSUDIEW200SAUDChannelType_E
	EM_UDIEW200S_AUDIOPID,        ///< [r][w]��Ƶpid,int,
	EM_UDIEW200S_VIDEOALPHA,      ///< [w] ���ڵ�͸���ȣ�int,[0,100],0Ϊ��ȫ͸����100Ϊ��͸��
	EM_UDIEW200S_BRIGHTNESS,      ///< [w]��Ƶ���ȣ�int,[0,100],0Ϊȫ�ڣ�100Ϊ�������
	EM_UDIEW200S_CONTRAST,        ///< [w]��Ƶ�Աȶȣ�int,[0,100],0Ϊȫ�ڣ�100Ϊ�Աȶ����
	EM_UDIEW200S_SATURATION,      ///< [w]��Ƶ���Ͷȣ�int,[0,100],0Ϊȫ�ڣ�100Ϊ���Ͷ����
	EM_UDIEW200S_ASPECTRATIO,     ///< [w]�����߱�,CSUDIEW200SAspectRatio_E
	EM_UDIEW200S_MATCHMETHOD,     ///< [r][w]���ڵ�pan scan�ȸ�ʽ,CSUDIEW200SMatchMethod_E
	EM_UDIEW200S_VIDEOFORMAT,     ///< [w]��Ƶ�����ʽ��CSUDIEW200SVdisVideoFormat_E	
	EM_UDIEW200S_STOPMODE,        ///< [r][w]��̨ʱ��Ƶ�Ĵ�������:CSUDIEW200SStopMode_E
	EM_UDIEW200S_PROGRAMEINFO ,    ///< [r] ��Ŀ��Ϣ,CSUDIEW200SProgrameInfo_S
	EM_UDIEW200S_MIDDLEWAREVER,  //�м���汾����󳤶�32�ֽڡ�
	EM_UDIEW200S_SOFTWAREVER,   //�����������󳤶�16�ֽ�
	EM_UDIEW200S_TVCODE,         //���Ӻ���󳤶�16�ֽڡ�
	EM_UDIEW200S_RESOLUTION,	 //��ȡ��Ƶ�ֱ���, int��0-EM_ITV_RESOLUTION_720_576, 1-EM_ITV_RESOLUTION_1280_720
	EW_UDIEW200S_LOGINKEY       //[r]��������,char *
}CSUDIEW200SClientInfoType_E;

/**@brief ��̨ʱ��Ƶ�Ĵ������� */
typedef enum 
{	
	EM_UDIEW200S_STOPMODE_BLACK,		///< ����
	EM_UDIEW200S_STOPMODE_FREEZE,		///< ��֡	
	EM_UDIEW200S_STOPMODE_FIFO		///< ���뵭��
}CSUDIEW200SStopMode_E;

/**@brief �������� */
typedef enum 
{
  	EM_UDIEW200S_AUD_CHANNEL_STEREO,     ///< ������
  	EM_UDIEW200S_AUD_CHANNEL_LEFT,       ///< ������    
  	EM_UDIEW200S_AUD_CHANNEL_RIGHT,      ///< ������ 
  	EM_UDIEW200S_AUD_CHANNEL_MIXED_MONO  ///< �����
}CSUDIEW200SAUDChannelType_E;	

/**@brief ���ڸ�ʽ */
typedef enum 
{
	EM_UDIEW200S_LETTER_BOX,	///< Letter Box
	EM_UDIEW200S_PAN_SCAN,	        ///< Pan Scan
	EM_UDIEW200S_COMBINED,	        ///< ComBined
	EM_UDIEW200S_IGNORE,		///< Ignore
	EM_UDIEW200S_NUM			///< Number, not use
}CSUDIEW200SMatchMethod_E;

/**@brief ��Ƶ���� */
typedef enum 
{
	EM_UDIEW200S_VIDEO_FMT_NTSC = 0,          ///< 480i, NSTC-M for North America 
	EM_UDIEW200S_VIDEO_FMT_NTSC_J,            ///< 480i (Japan) 
	EM_UDIEW200S_VIDEO_FMT_PAL_B,             ///< Australia 
	EM_UDIEW200S_VIDEO_FMT_PAL_B1,            ///< Hungary
	EM_UDIEW200S_VIDEO_FMT_PAL_D,             ///< China 
	EM_UDIEW200S_VIDEO_FMT_PAL_D1,            ///< Poland 
	EM_UDIEW200S_VIDEO_FMT_PAL_G,             ///< Europe 
	EM_UDIEW200S_VIDEO_FMT_PAL_H,             ///< Europe 
	EM_UDIEW200S_VIDEO_FMT_PAL_K,             ///< Europe 
	EM_UDIEW200S_VIDEO_FMT_PAL_I,             ///< U.K. 
	EM_UDIEW200S_VIDEO_FMT_PAL_M,             ///< 525-lines (Brazil)
	EM_UDIEW200S_VIDEO_FMT_PAL_N,             ///< Jamaica, Uruguay 
	EM_UDIEW200S_VIDEO_FMT_PAL_NC,            ///< N combination (Argentina) 
	EM_UDIEW200S_VIDEO_FMT_SECAM,             ///< LDK/SECAM (France,Russia) 
	EM_UDIEW200S_VIDEO_FMT_1080i,               ///< HD 1080i 
	EM_UDIEW200S_VIDEO_FMT_1080p,             ///< HD 1080p 60/59.94Hz, SMPTE 274M-1998
	EM_UDIEW200S_VIDEO_FMT_720p,               ///< HD 720p 
	EM_UDIEW200S_VIDEO_FMT_480p,               ///< HD 480p 
	EM_UDIEW200S_VIDEO_FMT_1080i_50Hz,        ///< HD 1080i 50Hz, 1125 sample per line, SMPTE 274M 
	EM_UDIEW200S_VIDEO_FMT_1080p_24Hz,        ///< HD 1080p 24Hz, 2750 sample per line, SMPTE 274M-1998 
	EM_UDIEW200S_VIDEO_FMT_1080p_25Hz,        ///<HD 1080p 25Hz, 2640 sample per line, SMPTE 274M-1998 
	EM_UDIEW200S_VIDEO_FMT_1080p_30Hz,       ///< HD 1080p 30Hz, 2200 sample per line, SMPTE 274M-1998 
	EM_UDIEW200S_VIDEO_FMT_1250i_50Hz,       ///< HD 1250i 50Hz, another 1080i_50hz standard SMPTE 295M 
	EM_UDIEW200S_VIDEO_FMT_720p_24Hz,        ///< HD 720p 23.976/24Hz, 750 line, SMPTE 296M 
	EM_UDIEW200S_VIDEO_FMT_720p_50Hz,         ///< HD 720p 50Hz (Australia) 
	EM_UDIEW200S_VIDEO_FMT_576p_50Hz,         ///< HD 576p 50Hz (Australia)
	EM_UDIEW200S_VIDEO_FMT_CUSTOM_1440x240p_60Hz,  ///< 240p 60Hz 7411 custom format. 
	EM_UDIEW200S_VIDEO_FMT_CUSTOM_1440x288p_50Hz,  ///< 288p 50Hz 7411 custom format. 
	EM_UDIEW200S_VIDEO_FMT_CUSTOM_1366x768p,       ///< Custom 1366x768 mode 
	EM_UDIEW200S_VIDEO_FMT_CUSTOM_1366x768p_50Hz, ///< Custom 1366x768 50Hz mode 
	EM_UDIEW200S_VIDEO_FMT_DVI_640x480p,       ///< DVI Safe mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_800x600p,       ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1024x768p,     ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1280x768p,      ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1280x720p_50Hz, ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1280x720p,      ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_DVI_1280x720p_ReducedBlank, ///< DVI VESA mode for computer monitors 
	EM_UDIEW200S_VIDEO_FMT_MaxCount            ///< Counter. Do not use! 
}CSUDIEW200SVdisVideoFormat_E;

/**@brief ���������߱� */
typedef enum 
{
	EM_UDIEW200S_WIN_ASPECT_RATIO_4_3 = 0,   ///< 4��3 
	EM_UDIEW200S_WIN_ASPECT_RATIO_16_9       ///< 16��9 
}CSUDIEW200SAspectRatio_E;

/**@brief ��Ƶ���� */
typedef struct 
{
	CSUDI_UINT16  wVideoPID;              ///< ��ƵPID
	CSUDI_UINT8   ucVideoFormat;        ///< ��Ƶ��ʽ
}CSUDIEW200SVideoParams_S;

/**@brief ��Ƶ���� */
typedef struct 
{
	CSUDI_UINT16  wAudioPID;                ///< ��ƵPID
	CSUDI_UINT8    ucAudioFormat;           ///< ��Ƶ��ʽ
	CSUDI_UINT8    ucLanguageCode[4];       ///< ����������,�ַ���Chinese��English�� ��
}CSUDIEW200SAudioParams_S; 

/**@brief ��Ļʵ�� */
typedef struct 
{
	CSUDI_UINT8     aucLanguage[3];         ///< ISO639 �ַ����Ա���
	CSUDI_UINT8     ucSubtitleType;         ///< ��Ļ����
	CSUDI_UINT16   wCompPageID;		///< ҳ��ʶ�������
	CSUDI_UINT16   wAnciPageID;		///< ����ҳ���ʶ
}CSUDIEW200SSubtitleEntity_S;

/**@brief ͼ�ĵ���ʵ�� */
typedef struct 
{
	CSUDI_UINT8    aucLanguage[3];         ///< ISO639 �ַ����Ա���
	CSUDI_UINT8    ucTeletextType;	       ///< ͼ�ĵ�������
	CSUDI_UINT8    ucTeleMagNum;	       ///< ͼ�ĵ�����־����
	CSUDI_UINT8    ucTelePageNum;          ///< ͼ�ĵ���ҳ��
}CSUDIEW200STeletextEntity_S;

/**@brief ��Ļ���� */
typedef struct 
{
	CSUDI_UINT8    ucStreamType;           ///< ����������
	CSUDI_UINT16    wSubtitlePID;          ///< subtitile PID
	CSUDI_UINT8    ucDescriptorCount;      ///< subtitile����
	CSUDIEW200SSubtitleEntity_S sDescriptor[CSUDIEW200S_SUBTITLE_PER_STREAM]; ///< subtile����
}CSUDIEW200SSubtitileParams_S;

/**@brief ͼ�ĵ��Ӳ��� */
typedef struct 
{
	CSUDI_UINT8    ucStreamType;    ///< ����������
	CSUDI_UINT16  wTeletextPID;      ///< ͼ�ĵ���PID
	CSUDI_UINT8    ucDescriptorCount; ///< ͼ�ĵ��Ӹ���
	CSUDIEW200STeletextEntity_S sDescriptor[CSUDIEW200S_TELETEXT_PER_STREAM];  ///< ͼ�ĵ�������
}CSUDIEW200STeletextParams_S;

/**@brief ��Ŀ��Ϣ */
typedef struct 
{
	CSUDI_UINT16	wPCRPID;              ///< PCR PID
	CSUDI_UINT16	wVideoCount;          ///< ��Ƶ��Ŀ
	CSUDI_UINT16    wVideoIndex;          ///< ��Ƶ����
	CSUDIEW200SVideoParams_S	asVideo[CSUDIEW200S_VIDEOCOUNT_PER_PROG]; ///< һ����Ŀ�е�������Ƶ��Ϣ
	CSUDI_UINT16	wAudioCount;     ///< ��Ƶ��Ŀ
	CSUDI_UINT16 wAudioIndex;        ///< ��Ƶ����
	CSUDIEW200SAudioParams_S	asAudio[CSUDIEW200S_AUDIOCOUNT_PER_PROG];///< һ����Ŀ�е�������Ƶ��Ϣ
	CSUDI_UINT16	wSubtitleCount;  ///< subtitle��Ŀ
	CSUDI_UINT16 wSubtitleIndex;	///< subtitle����
	CSUDIEW200SSubtitileParams_S asSubtitle[CSUDIEW200S_SUBTITLECOUNT_PER_PROG];///< һ����Ŀ�е�����subtitle��Ϣ
	CSUDI_UINT16	wTeletextCount;  ///< ͼ�ĵ�����Ŀ
	CSUDI_UINT16 wTeletextIndex;	 ///< ͼ�ĵ�������
	CSUDIEW200STeletextParams_S asTeletext[CSUDIEW200S_TELETEXTCOUNT_PER_PROG];///< һ����Ŀ�е�����ͼ�ĵ�����Ϣ
}CSUDIEW200SProgrameInfo_S;


/**
@brief ��ȡ�ն���Ϣ�ӿ�

Eastwin200��ͨ���ýӿڻ�ȡ�ն˵����к�,���ܿ��ŵ���Ϣ,���ȡ��ҳ�����õ���Ϣ
@param[in] eType ��Ϣ����
@param[in] value eType���Ͷ�Ӧ��ֵ
@param[in] length valueֵ�ĳ���
@return ����ʵ����Ϣ�ĳ���,������-1
@note ��Ҫ���õ���Ϣ��CSUDIEW200SClientInfoType_Eö���е�[r]��ʶ
@note ֧��Coship����Enreach����ʱ��Ҫʵ�ָýӿ�

@code
//�ο�ʵ��(δ���ǻ�����������쳣)
//ע��:�������������value��ʾ�����ͼ��������в�ͬ���밴�ο�����ʵ��/ʹ�ü��ɡ�
if(eType == EM_UDIEW200S_AUDIOCHANNEL)
{
	//EM_UDIEW200S_AUDIOCHANNEL��Ӧ����������int��
	//valueΪint�ı������䳤��Ӧ��Ϊsizeof(int)
	
	value = EM_UDIEW200S_AUD_CHANNEL_STEREO;
	CSUDIEW200SReadClientInfo(eType , (void*)&value, sizeof(int));

}
else if(eType == EM_UDIEW200S_SN)
{
	//EM_UDIEW200S_SN��Ӧ����������CSUDI_UINT8����
	//valueһ��ָ��CSUDI_UINT8���飬�䳤��Ϊlength
	//��valueָ���buffer�л�ȡ��Ӧ����
	value[32];
	CSUDIEW200SReadClientInfo(eType , (void*)value, sizeof(value));

}
@endcode
*/
int CSUDIEW200SReadClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length);

/**
@brief �����ն���Ϣ�ӿ�

Eastwin200��ͨ���ýӿڽ�һЩ�û���Ϣ���õ��ն����ô洢����Ӧ���Լ��洢����
@param[in] eType ��Ϣ����
@param[in] value eType���Ͷ�Ӧ��ֵ
@param[in] length valueֵ�ĳ���
@return ����ʵ�ʴ洢��Ϣ�ĳ���,������-1
@note ��Ҫ���õ���Ϣ��CSUDIEW200SClientInfoType_Eö���е�[w]��ʶ
@note ֧��Coship����Enreach����ʱ��Ҫʵ�ָýӿ�
@note �ο�ʵ��ͬCSUDIEW200SReadClientInfo
*/
int CSUDIEW200SWriteClientInfo(CSUDIEW200SClientInfoType_E eType, void* value, int length);

/**
@brief ͨ��DVB3Ҫ�ز�ѯ����Դ��Ϣ

������ȡ��3Ҫ�ض�Ӧ��Ϣʱ����ʹ�õ�TunerId,DemuxId,��Ƶ��
@param[in] nOrNetId org network id
@param[in] nTsId ts id
@param[in] nServiceId service id
@param[out] psSourceParam ���ڴ洢���������Դ��Ϣ
@return ����ɹ�����CSUDI_SUCCESS, ʧ�ܷ��ش������
@note EW200ͨ���ýӿڲ�ѯһ��Service�Ķ�Ӧ��Ƶ�㼰��Ƶͷ��Ϣ����ƽ̨�ж����Ƶͷ������ӿڵ�ʵ��Ҫ�ر�С�ģ���Ϊ��Ӧ�����ݿ����ڲ�ͬ�ĸ�Ƶͷ�ϣ�����򵥲�ѯ���ݹ�����
*/
CSUDI_Error_Code CSUDIEW200SGetTuneParam(int nOrNetId,int nTsId,int nServiceId,CSUDIEW200SSourceParam_S * psSourceParam);

/**
@brief ��ȡIPQam����Ϣ�ӿ�

������VODģ��
��֤ģ��ͨ�������ն�ʵ�ֵĸýӿڵõ�IPQam������Ϣ
- CITV3.x�汾��VODĿǰȡ����ָ��Section�ĵ�4��5�ֽ�(��data[3],data[4])
@param[in] punDeliverList �����Ϣ����Ƶ���б�����
@param[in] nDeliverCount pDeliverList�����е�Ƶ�����
@param[in] nPid IPQam����Ϣ����Pid
@param[in] ucTableId IPQam����Ϣ����TableId
@param[out] pIpQamDataBuffer ���ڴ��IPQam����Ϣ���ڴ�ָ��
@param[in] nBufferLength pIpQamDataBuffer�ĳ���
@param[in] pvReserved �����ֶ�,Ŀǰ����ΪNULL
@return �ɹ���ȡ����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSUDIEW200SGetIPQamDomainInfo(const CSUDITunerSRCDeliver_U* punDeliverList,
										int nDeliverCount,
										int nPid,
										unsigned char ucTableId,
										char* pIpQamDataBuffer,
										int nBufferLength,
										void* pvReserved
									   );
									   
/**
@brief ���Ž�Ŀ�ӿ�

�����ƶ�URL�ĸ�ʽ���в���

@param[in] pcURL ����URL
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@code ����URL�ĸ�ʽ�����¼��֣�Ӧ�ÿ��Ը��ݲ�ͬ��URL��ʽ���в��Ŵ���
(1)dvb://OriginalNetworkId.TS_Id.ServiceId  ����ο������ֵ����м�������淶����3���ָ�¼I��1.2.1С�ڡ�
		OriginalNetworkId: original_network_id
		TS_Id: transport_stream_id
		ServiceId: service_id
		����: dvb://1.2.5(ʮ������)
(2)dvbc://Frequency.SymbolRate.Modulation.ServiceId
		Frequency:Ƶ��.��λMHz
		SymbolRate:�����ʣ���λ��Kbps
		Modulation:����ģʽ
		ServiceId:��ĿId
		����: deliver://490000000:6875:64:10(ServiceIdΪʮ������)
(3)avpid://pcrPid.emm.videoPid.audioPid.audio_ecm_pid.video_ecm_pid(ʮ������)	
@endcode
*/
void CSUDIEW200SPlayStream(char* pcURL, int nPlayId);

/**
@brief ֹͣ��Ŀ���Žӿ�

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
*/
void CSUDIEW200SStopPlay(int nPlayId);

/**
@brief ������Ƶ���Ŵ��ڴ�С�ӿ�

@param[in] psRect ��Ƶ���δ�С,ΪNULL��ʾȫ������
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
*/
void CSUDIEW200SSetPlayRect(CSUDIWinRect_S* psRect,int nPlayId);

/**
@brief �ָ���ͣģʽ 

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���

@return 
- �ɹ�����CSUDI_SUCCESS��
- ʧ�ܷ�������
��֧�ִ˲�������CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
@note �ָ���ͣģʽ;��CSUDIEW200SPause�ɶ�ʹ�á�
*/
CSUDI_Error_Code CSUDIEW200SResume(int nPlayId);

/**
@brief ��ͣ��hPlayer��ص�һ���������Ĺ���

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���

@return 
- �ɹ�����CSUDI_SUCCESS;
- ʧ�ܷ�������
��֧�ִ˲�������CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
@note ��ͣ��hPlayer��ص�һ���������Ĺ�����ֻ�����ڲ���ĳһ��Ŀ��������ֻ����Ƶ������Ƶ��ʱ���ò���Ч��
*/
CSUDI_Error_Code CSUDIEW200SPause(int nPlayId);

/**
@brief �����˳�TrickMode

֪ͨ�������������Ƶ���ܻᰴ�������ٶȽ����������
������VOD����ʱ�Ŀ��˿���ȣ����������Tuner����������Ϊ�������ٶȣ�
ƽ̨������׼���Է�ֹ���Ƚ׶μ����Ž׶ε������ˡ�
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@param[in] bTrickMode ΪCSUDI_TRUE��ʾ����TrickMode,ΪCSUDI_FALSE��ʾ�˳�TrickMode
@return 
- �ɹ�����CSUDI_SUCCESS��
��֧�ִ˲�������CSUDIEW200S_ERROR_FEATURE_NOT_SUPPORTED
�Ѿ����ڴ�״̬����CSUDIEW200S_ERROR_TRICKMODE_STATE_ERROR
*/
CSUDI_Error_Code CSUDIEW200STrickMode(int nPlayId,CSUDI_BOOL bTrickMode);


/**
@brief ע��UDI����Ϣ�ص�����

ע���UDI����ص���Ϣ��ͨ���ýӿ�֪ͨ��Ӧ��
@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@param[in] pUserData �û�����,���ڻص���ԭ������
@return �ɹ�����CSUDI_SUCCESS,ʧ�ܷ��ش�������CSUDIEW200S_ERRORCODE_E����
@note
- �ص�������ע���������֧��5�������Ӧ��ע��ص�����������֧�ָ���������CSUDIEW200S_ERROR_NO_MEMORY
- һ��nPlayId,fncallback,pUserdata��ͬ����һ���ص���������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIEW200S_ERROR_ALREADY_ADDED
*/
CSUDI_Error_Code CSUDIEW200SAddCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata);

 /**
@brief ɾ��UDI��ص�������ע��

@param[in] nPlayId �μ�Eastwin200VOD�е�nPlayId����,��֧�ֶ�Demux,��Tuner,Pip�ȹ��ܵĻ�������ֲ������øò������Ӧ��ȷ��ʹ���ĸ��豸����������ɺ��Ըò���
@param[in] fnTunerCallback �ص��������
@param[in] pUserData ͬCSUDIEW200SAddCallback�е�pUserData
@return�ɹ�����CSUDI_SUCCESS ʧ�ܴ��󷵻ؼ�CSUDIEW200S_ERRORCODE_E����
*/
CSUDI_Error_Code CSUDIEW200SDelCallback(int nPlayId,CSUDIEW200SCallback_F fncallback,void* pUserdata);

#ifdef __cplusplus
}
#endif

/** @} */

#endif   


