/**@defgroup PUBLIC PUBLIC  ����������Ƶ�Լ�ͼƬ������ؽӿ��õ��Ĺ�������
@brief Publicģ���Ӧͷ�ļ�udi2_public.h������������Ƶ�Լ�ͼƬ������ؽӿ��õ��Ĺ�������

����Ƶģ����豸,���ܵĳ�����ο���ͼ(�������߲���Ϊδ�����ģ��):
@image HTML udi_av.png

- ���е�һ��(player,inject,record)Ϊ��Ϸ�������ṩ��һЩ���ö����豸��ɵķ���
- �ڶ���(demux,video,audio)Ϊ�豸�����ṩ��һЩ�豸�����������ĳ���
- �����ż�����Ϊ��������ĳ����ṩ�����ö�Ӧ�豸�Ķ�����ɵĹ��ܡ�

@version 2.0.4 2009/11/04 ȥ������Ƶ���ݴ����ʽ����Դ,injectģ���Ѿ�����
@version 2.0.3 2009/08/28 �����˾�������ṹ��
@version 2.0.2 2009/08/25 �����˶���������Ƶģ��ĳ���˵��
@version 2.0.1 2009/08/17 ����

@{
*/
#ifndef _UDI2_PUBLIC_H_
#define _UDI2_PUBLIC_H_


#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDI_MAX_FILE_NAME_LEN (256)       ///< �ļ������Ƶ���󳤶�

#define CSUDI_MAX_URL_LEN (1024)       ///< URL ��󳤶�

#define CSUDI_MAX_SECURITYKEY_LEN (128)  ///< ������볤�� ��λ:BYTES,����ָ��AED,DES������Կ����

#define CSUDI_INVALID_PID (0xe000)          ///< �Ƿ�������ƵPIDֵ

#define CSUDI_INVALID_INDEX (-1)   	        ///< �Ƿ����豸id


/**@brief ��Ƶ�������ʽ����ע��������ʽ(TS��AVI��ASF������)*/
typedef enum 
{
	EM_UDI_AUD_STREAM_UNKNOWN,      ///< δ֪���ͣ���������£�����Ӧ����ʶ����Ҫ���ڽ�AVI�������е�����Ƶ
	EM_UDI_AUD_STREAM_AC3,			///< Dolby Digital AC3 audio  
	EM_UDI_AUD_STREAM_DTS,			///< Digital Surround sound 
	EM_UDI_AUD_STREAM_MPEG1,		///< MPEG1/2, layer 1/2. This does not support layer 3 (mp3) 
	EM_UDI_AUD_STREAM_MPEG2,		///< MPEG1/2, layer 1/2. This does not support layer 3 (mp3) 
	EM_UDI_AUD_STREAM_CDDA,			///< 1979�꣬Philips��Sony��˾�������Ͽ���CD-DA Compact Disc-Digital Audio,���ܹ���������Ƶ����׼ 
	EM_UDI_AUD_STREAM_PCM,			///< PCM data
	EM_UDI_AUD_STREAM_LPCM,		
  	EM_UDI_AUD_STREAM_MP3,			///<  MPEG1/2, layer 3 
  	EM_UDI_AUD_STREAM_MPEG_AAC,		///<  Advanced audio coding. Part of MPEG-4  
  	EM_UDI_AUD_STREAM_HE_AAC,		///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE)  
  	EM_UDI_AUD_STREAM_RAW_AAC,	    
	EM_UDI_AUD_STREAM_AAC_PLUS_ADTS, ///<   AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format)  
	EM_UDI_AUD_STREAM_AAC_PLUS_LOAS, ///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream)  
	EM_UDI_AUD_STREAM_AC3_PLUS,		///<   Dolby Digital Plus (AC3+ or DDP) audio 
	EM_UDI_AUD_STREAM_DTS_HD,		///<   Digital Surround sound, HD 
  	EM_UDI_AUD_STREAM_WMA_STD,		///<   WMA Standard  
  	EM_UDI_AUD_STREAM_WMA_PRO,		///<   WMA Professional 
  	EM_UDI_AUD_STREAM_DRA,			///< DRA1
  	EM_UDI_AUD_STREAMTYPE_NUM				
}CSUDIAUDStreamType_E;							

/**@brief ��Ƶ�������ʽ����ע��������ʽ(TS��ES��AVI��ASF������)*/
typedef enum 
{
	EM_UDI_VID_STREAM_UNKNOWN,      ///< δ֪���ͣ���������£�����Ӧ����ʶ����Ҫ���ڽ�AVI�������е�����Ƶ
	EM_UDI_VID_STREAM_MPEG1,		///< MPEG-1 Video (ISO/IEC 11172-2) 
	EM_UDI_VID_STREAM_MPEG2,		///< MPEG-2 Video (ISO/IEC 13818-2)  
	EM_UDI_VID_STREAM_MPEG4,		///< MPEG-4 Part 2 Video  
	EM_UDI_VID_STREAM_H264,			///< H.264 (ITU-T) or ISO/IEC 14496-10/MPEG-4 AVC 
	EM_UDI_VID_STREAM_H263,			///< H.263 Video. The value of the enum is not based on PSI standards 
	EM_UDI_VID_STREAM_VC1,			///< VC-1 Advanced Profile  
	EM_UDI_VID_STREAM_VC1_SM,		///< VC-1 Simple&Main Profile 
	EM_UDI_VID_STREAM_DIVX_311,		///< DivX 3.11 coded video 
  	EM_UDI_VID_STREAMTYPE_NUM
}CSUDIVIDStreamType_E;


/**@brief ������*/
typedef enum 
{
	EM_UDI_CONTENT_VIDEO = 0,	///< ��Ƶ����	 						
	EM_UDI_CONTENT_AUDIO,  	    ///< ��Ƶ����						
	EM_UDI_CONTENT_SUBTITLE,    ///< ��Ļ����
	EM_UDI_CONTENT_TELTEXT, 	///< ͼ�ĵ�������
	EM_UDI_CONTENT_PCR 		///< PCR����	
}CSUDIContentType_E;

/**@brief ������Ϣ����*/
typedef struct
{
	int                         m_nPid;                        ///< ��������PID.pidֵ�ķ�Χ��ѭ13818-1�����ǣ������stream������ע�벥��ģʽ��ʹ��ʱ���븳ֵm_nPid=-1
	CSUDIContentType_E       	m_eContentType;  	           ///< �����ͣ���CSUDIContentType_E����
	union 
	{
		CSUDIVIDStreamType_E m_eVideoType; ///< ��Ƶ������
		CSUDIAUDStreamType_E m_eAudioType; ///< ��Ƶ������
	} m_uStreamType;   	///< ���ݵ������ͣ����������֣����Ҹ���m_eContentType������ȷ��ʹ������������������-CSUDIVIDStreamType_E m_eVideoType����Ƶ�����͡�-CSUDIAUDStreamType_E m_eAudioType����Ƶ�����͡�
	
}CSUDIStreamInfo_S;

/**@brief ��������ṹ���� */
typedef struct 
{
	int m_nX;                ///< �����������ϽǺ����� 
	int m_nY;                ///< �����������Ͻ������� 
	int m_nWidth;            ///< ���������� > 0
	int m_nHeight;           ///< ��������߶� > 0
} CSUDIWinRect_S;


#ifdef  __cplusplus
}
#endif
/** @} */
#endif


