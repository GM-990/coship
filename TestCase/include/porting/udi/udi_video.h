/**@defgroup CSVideo video �ṩͳһ����Ƶ������ؽӿ�
@brief �ṩͳһ����Ƶ������ؽӿ�

@version 0.3 2009/04/27 ����CSVID_TIMING_MODE_1080P_50HZ
@version 0.2 2008/10/30 �����ϸ����������ʣ��ĳ�ʼ���ӿ�
@version 0.1 2008/10/23 ��ʼ�汾
@{
*/

#ifndef _UDI_SYSVIDEO_H_
#define _UDI_SYSVIDEO_H_

#include "udi_typedef.h"
#include "udi_public.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ������Ƶ���������ٶ�*/
#define CS_VID_NORMAL_SPEED 		(1000)

/**@brief ��Ƶ��߱�*/
typedef enum _CSVIDAspectRatioType
{
	CSVID_ASPECT_RATIO_UNKNOWN,
	CSVID_ASPECT_RATIO_4TO3,	/**< 4��3*/
	CSVID_ASPECT_RATIO_16TO9,	/**< 16��9*/
	CSVID_ASPECT_RATIO_AUTO		/**< �Զ�ѡ��*/
}CSVIDAspectRatioType;

/**@brief ��Ƶ�����߱�ת������
@image HTML VIDAspectRatioType.png
*/
typedef enum _CSVIDMatchMethod
{
	CSVID_MATCH_METHOD_LETTER_BOX,	/**< Letter Box*/
	CSVID_MATCH_METHOD_PAN_SCAN,	/**< Pan Scan*/
	CSVID_MATCH_METHOD_COMBINED,	/**< ComBined*/
	CSVID_MATCH_METHOD_IGNORE,	    /**< Ignore*/
	CSVID_MATCH_METHOD_NUM
}CSVIDMatchMethod;

/**@brief ��Ƶ�������ʽ����ע��������ʽ(TS��ES��AVI��ASF������)*/
typedef enum _CSVIDStreamType
{
	CSVID_STREAM_MPEG1,			/**< MPEG-1 Video (ISO/IEC 11172-2) */
	CSVID_STREAM_MPEG2,			/**< MPEG-2 Video (ISO/IEC 13818-2) */
	CSVID_STREAM_MPEG4,			/**< MPEG-4 Part 2 Video */
	CSVID_STREAM_H264,			/**< H.264 (ITU-T) or ISO/IEC 14496-10/MPEG-4 AVC */
	CSVID_STREAM_H263,			/**< H.263 Video. The value of the enum is not based on PSI standards. */
	CSVID_STREAM_VC1,			/**< VC-1 Advanced Profile */
	CSVID_STREAM_VC1_SM,		/**< VC-1 Simple&Main Profile */
	CSVID_STREAM_DIVX_311,		/**< DivX 3.11 coded video */
  	CSVID_STREAMTYPE_NUM
}CSVIDStreamType;

/**@brief ��Ƶ����ӿ�*/
typedef enum _CSVIDOutputDevice
{
	CSVID_OUTPUT_CVBS	=0x00000001,	/**< video cvbs BNC ����ӿ�*/
	CSVID_OUTPUT_YC		=0x00000002,	/**< S��������ӿ�*/
	CSVID_OUTPUT_RGB	=0x00000004,	/**< video R,G, B, component output interface*/
	CSVID_OUTPUT_YUV	=0x00000008,	/**< video Y, Cb, Cr output interface*/
	CSVID_OUTPUT_RF		=0x00000010,	/**< ����*/
	CSVID_OUTPUT_SCART	=0x00000020,	/**< ����*/
	CSVID_OUTPUT_HDMI	=0x00000040,	/**< hdmi �ӿ�*/
	CSVID_OUTPUT_DVO	=0x00000080	    /**< ������Ƶ���*/
}CSVIDOutputDevice;

/**@brief ��Ƶ����ֱ���*/
typedef enum _CSVIDTimingMode
{
	CSVID_TIMING_MODE_SLAVE,
	CSVID_TIMING_MODE_NTSC,  		/**< SD: CSMIX_ENCODING_MODE_NTSCM*/ 
	CSVID_TIMING_MODE_PAL, 			/**< SD: CSMIX_ENCODING_MODE_PALBDGHI*/
	CSVID_TIMING_MODE_576P, 		/**< Australian mode*/
	CSVID_TIMING_MODE_720P,  		/**< HD: EIA770.3 #1 P60 = SMPTE 296M #1 P60*/
	CSVID_TIMING_MODE_720P_50HZ,
	CSVID_TIMING_MODE_1080I,  		/**< HD: EIA770.3 #3 I60 = SMPTE274M #4 I60*/ 		
	CSVID_TIMING_MODE_1080I_50HZ,	/**< HD:  SMPTE 274M   I50*/
	CSVID_TIMING_MODE_AUTO,			/**< new add, ref 7710. zzp, 2006.9.18*/
	CSVID_TIMING_MODE_NTSC_443,		/**< NTSC M mode but with 4.43361875MHz color subcarrier frequency.*/
	CSVID_TIMING_MODE_NTSC_JAPAN,	/**< Japan NTSC, no pedestal level*/
	CSVID_TIMING_MODE_PAL_M,        /**< PAL Brazil*/
	CSVID_TIMING_MODE_PAL_N,        /**< PAL_N*/
	CSVID_TIMING_MODE_PAL_NC,       /**< PAL_N, Argentina (PAL N mode but with 3.58205625 MHz color subcarrier frequency and no pedestal (= 0 IRE blanking setup))*/
	CSVID_TIMING_MODE_PAL_B,		/**< Australia*/
	CSVID_TIMING_MODE_PAL_B1,		/**< Hungary*/
	CSVID_TIMING_MODE_PAL_D,		/**< China ==CSVID_TIMING_MODE_PAL*/
	CSVID_TIMING_MODE_PAL_D1,		/**< Poland*/
	CSVID_TIMING_MODE_PAL_G,		/**< Europe. Same as CSVID_TIMING_MODE_PAL.*/
	CSVID_TIMING_MODE_PAL_H,		/**< Europe*/
	CSVID_TIMING_MODE_PAL_K,		/**< Europe*/
	CSVID_TIMING_MODE_PAL_I,		/**< U.K.*/
	CSVID_TIMING_MODE_SECAM,        /**< SECAM III B6*/
	CSVID_TIMING_MODE_480P,         /**< NTSC Progressive (27Mhz)*/
	CSVID_TIMING_MODE_1080P,        /**< HD 1080 Progressive, 60Hz*/
	CSVID_TIMING_MODE_1080P_24HZ,	/**< HD 1080 Progressive, 24Hz*/
	CSVID_TIMING_MODE_1080P_25HZ,	/**< HD 1080 Progressive, 25Hz*/
	CSVID_TIMING_MODE_1080P_30HZ,	/**< HD 1080 Progressive, 30Hz*/
	CSVID_TIMING_MODE_1250I_50HZ,   /**< HD 1250 Interlaced, 50Hz*/
	CSVID_TIMING_MODE_720P_24HZ,    /**< HD 720p 24Hz*/
	CSVID_TIMING_MODE_VESA,			/**< PC monitor. Requires width, height and refresh rate parameters*/
	CSVID_TIMING_MODE_1080P_50HZ, /**< HD 1080 Progressive, 50Hz*/
	CSVID_TIMING_MODE_NUM
} CSVIDTimingMode;

/**@brief ��Ƶ�����ö��*/
typedef enum _CSVIDWindow
{
	CSVID_WINDOW_VIDEO,			/**< ��Ƶ����*/
	CSVID_WINDOW_OSD,			/**< osd ����*/
	CSVID_WINDOW_BACKGROUND,	/**< ��������*/
	CSVID_WINDOW_ALLWINDOW
}CSVIDWindow;


/**@brief ��Ƶ��ʾģʽ*/
typedef enum _CSSYSVideoShowType
{
	CS_MODE_NORMAL = 0,	/**< ���ڲ��ŵ�ǰ��Ŀ*/
	CS_MODE_BLACK,  	/**< ����*/
	CS_MODE_FREEZE		/**< ��֡*/
}CSSYSVideoShowType;

/**@brief ��Ƶֹͣģʽ*/
typedef enum _CSVIDStopMode	
{	
	CSVID_STOPMODE_BLACK,	/**< ����*/
	CSVID_STOPMODE_FREEZE,	/**< ��֡*/	
	CSVID_STOPMODE_FIFO	    /**< ���뵭��*/
}CSVIDStopMode;

/**@brief ��Ƶ�¼�*/
typedef enum _CSSYSVidEvent_E
{
	EM_VID_NO_EVT	=0x00000000,			    /**< û���κ��·���*/
	EM_VID_SOURCE_CHANGED_EVT=0x00000001,		/**< ��ƵԴ�仯*/
	EM_VID_ASPACTRADIO_CHANGED_EVT=0x00000002,	/**< ��߱�ת�����Ʊ仯*/
	EM_VID_TIMINGMODE_CHANGED_EVT=0x00000004,	/**< ����ֱ��ʱ仯*/
	EM_VID_OVERFLOW_EVT=0x00000008,			    /**< �������*/
	EM_VID_UNDERFLOW_EVT=0x00000010,		    /**< �������ݺľ�����û�����ݲ���*/
	EM_VID_PAKET_START =  0x000000020,		    /**< �����ע��*/
	EM_VID_PAKET_STOP = 0x000000040,		    /**< �����ע��*/
	EM_VID_ALL_EVT=0x0000007f			        /**< �¼�����*/
}CSSYSVidEvent_E;

/**@brief ��Ƶ״̬*/
typedef struct _CSSYSVIDStatus_S
{
	BOOL 			m_bWindowEnable;	    /**< ��Ƶ��ʾ�����Ƿ��*/	
	CSVIDStreamType		m_eVidStreamType;	/**< ��Ƶ������*/
	int 			m_nSourceWidth;		    /**< Դ��Ƶ�����*/
	int 			m_nSourceHight;		    /**< Դ��Ƶ���߶�*/
	int 			m_nFrameRate;		    /**< Դ��Ƶ������*/
	BOOL			m_bInterlaced;		    /**< TRUE��ʾ����;FALSE��ʾ����*/
	
	CSVIDAspectRatioType 	m_eAspectRatio;		/**< ��Ƶ��߱�*/
	CSVIDMatchMethod 	m_eConversion;		    /**< ��Ƶ�����߱�ת������*/
	CSVIDTimingMode		m_eTimingModeSD;	    /**< �����豸��Ƶ����ֱ���*/
	CSVIDTimingMode		m_eTimingModeHD;	    /**< �����豸��Ƶ����ֱ���*/
	DWORD			m_dwFrameCount;		        /**< ��Ƶ�Ѳ��ŵ���֡��*/
	DWORD 			m_dwReserved1;		        /**< ��������������0*/
	DWORD			m_dwReserved2;		        /**< ��������������0*/
}CSSYSVIDStatus_S;

/**@brief ��Ƶ������Ϣ*/
typedef struct _CSSYSVIDInputInfo
{
	CSVIDStreamType	StreamType;		    /**< ��Ƶ������*/
	int 			nFrameRate;		    /**< ��Ƶ����: 25/30,0��ʾδ֪*/
	int 			nWidth;			    /**< ��Ƶ���:702/720/1920,0��ʾδ֪*/
	int 			nHight;			    /**< ��Ƶ�߶�: 480/576/1080,0��ʾδ֪*/
	CSVIDAspectRatioType 	AspectRatio;/**< ��Ƶ��߱�: 4:3/16:9,0��ʾδ֪*/
	int 			        nPID;		/**< ��Ƶ��PID*/
	CSSYSServiceType	    ServiceType;/**< ��Ƶ�������ͣ�PES or ES*/							
	int                     nFirstSpeed;/**< �������ŵĳ�ʼ�ٶ�, CS_VID_NORMAL_SPEED Ϊ�����ٶ�*/
}CSSYSVIDInputInfo;	

/**@brief ��Ƶ�������*/
typedef struct _CSWINRECT
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
}CSWINRECT;

/**@brief ��Ƶ����Ϣ*/
typedef struct _CSVIDWindowInfo
{
	CSVIDWindow		nWindowIndex;	/**< ��Ƶ���ڲ�*/
 	BOOL 			nShow;			/**< ��Ƶ��ʾ״̬*/
	BOOL 			nFullSize;		/**< �Ƿ�ȫ��*/
	CSWINRECT*		pstRect;		/**< ��ʾ����*/
	int 			nalpha;			/**< ��Ƶ���͸���ȣ���Χ����*/
}CSVIDWindowInfo;

/**@brief ��Ƶ�����Ϣ*/
typedef struct _CSSYSTVOutputInfo
{
	CSVIDOutputDevice	OutputDevice;		/**< ��Ƶ����豸*/
	CSVIDTimingMode		Resolution;		    /**< ��Ƶ����ֱ���*/
	CSVIDWindowInfo*	pstWindowInfo;		/**< ��Ƶ����Ϣ*/
	int 			    nBrightness;		/**< ����*/
	int 			    nContrast;		    /**< �Աȶ�*/
	int 			    nSaturation;		/**< ���Ͷ�*/
	int 			    nPositionH;		    /**< tv output PositionH*/
	int 			    nPositionY;		    /**< tv output PositionV*/
}CSSYSTVOutputInfo;

/**
@brief ��Ƶ��ʾ�¼��ص�����

�ú�����Ҫ����֪ͨ��Ƶ���Ź����е��¼���Ŀǰ����֪ͨCSSYSEVENT_FRONTTS_RUN��CSSYSEVENT_FRONTTS_STOP��
����ͣ����⹦�ܣ�ͣ��������ƵͬʱΪ׼����ʱ���ܵ���
ֻ�ڲ���ʱ��Ч,�������������Stopֹͣ����,������CSSYSEVENT_FRONTTS_STOP��Ϣ
@param[in] enmEventMsg �������¼�
@param[in] pvParam �û����ݣ�ע��ʱת�������
*/
typedef void (*CSSYSShowVideoCallback)(CSSYSEventMsg enmEventMsg, PVOID pvUserData );

/**
@brief ע����Ƶ��ʾ�¼��ص�����

�����ڷ�����ʾ��ص��¼�ʱ֪ͨ���ڣ���ǰ��ͣ����
@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������������ο��豸�����ӿ��ĵ����塣
@param[in] enmEventMsg ע�����Ϣ��Ŀǰ��֧��CSSYS_MSG_FRONT_TS
@param[in] pfnCallback ע��Ļص�������
@param[in] pvUserData �û��Զ�������ݡ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@see CSSYSShowVideoByEventMsg CSSYSShowVideoSetEventMask CSSYSShowVideoGetEventMask
*/
BOOL CSSYSVIDRegisterVideoShowCallback(HCSHANDLE hHandle,  CSSYSRegisterMsg enmEventMsg,CSSYSShowVideoCallback pfnCallback, PVOID pvUserData);

/**
@brief ֪ͨ�ײ㣬Ӧ����������Ƶ��ʾ���ܣ�����������廹��������ʾ����Ӧ���¼�����

���ϲ��⵽ĳ���¼���(ͨ��CSSYSShowVideoCallback�ص�����CA����Ƶͷ������ģ��)���ñ��������ڿ�����Ƶ��״̬
��ģ����Ҫ���Ƹ��ֲ�ͬ��Ϣ�����ȼ���״̬
@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@param[in] enmEventMsg ��Ӧ���¼�����
@param[in] enmVideoShowType �������¼�ʱ�Ĵ���ʽ���������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE.
@see CSSYSVIDRegisterVideoShowCallback CSSYSShowVideoSetEventMask CSSYSShowVideoGetEventMask
*/
BOOL CSSYSShowVideoByEventMsg( HCSHANDLE hHandle, CSSYSEventMsg enmEventMsg, CSSYSVideoShowType enmVideoShowType );

/**
@brief  ���ö�video������������õ��¼���Mask

���ڿ���ģ�鵱ǰ��ĳ���¼�����Ӧ�����ĳЩ�������ĳ��������棬����Ҫ��ӦĳЩ��Ϣ
@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@param[in] nEvent ��Ӧ���¼����ͣ���CSSYSEventMsg�е�ȡֵ
@param[in] dwMask �¼���Mask������˵�����ԣ�������¼��Ƿ���Ч�ȣ�ΪCSSYSEventMask
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ʹ�øýӿڿ���ʵ�ֽ��û�����һ��Event���Ͷ�video���صĿ��ƣ�Ҳ�������ø�Event�����Ƿ��Ŀ��ء�
һ��Event���ͽ���֮���������Ը�Event����CSSYSShowVideoByEventMsg
��Щ�¼���״̬��Ȼ�ᱻ��¼������������Ƶ�Ŀ��ؿ��ơ�
����һ���¼����û�����ʹ�ã���Ƶ�Ŀ���״̬�����������ı䣬
ֱ��CSSYSShowVideoByEventMsg�����á�

@code 
//����CSSYSEVENT_SIGNAL_LOCK��video�Ŀ��ƣ�
DWORD dwMask = CSSYSShowVideoGetEventMask(CSSYSGetNexyHandle(0), CSSYSEVENT_SIGNAL_LOCK);
dwMask &= ~CSSYSEVENT_MASK_ACTIVE;
CSSYSShowVideoSetEventMask(CSSYSGetNextHandle(0), CSSYSEVENT_SIGNAL_LOCK, dwMask);

//����CSSYSEVENT_SIGNAL_LOCK��video�Ŀ��ƣ�
DWORD dwMask = CSSYSShowVideoGetEventMask(CSSYSGetNexyHandle(0), CSSYSEVENT_SIGNAL_LOCK);
dwMask |= CSSYSEVENT_MASK_ACTIVE;
CSSYSShowVideoSetEventMask(CSSYSGetNextHandle(0), CSSYSEVENT_SIGNAL_LOCK, dwMask);
@endcode
@see CSSYSVIDRegisterVideoShowCallback CSSYSShowVideoByEventMsg CSSYSShowVideoGetEventMask
*/
BOOL CSSYSShowVideoSetEventMask(HCSHANDLE hHandle, int nEvent, DWORD dwMask); 

/**
@brief  ��ȡ��video������������õ��¼���Mask

@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@param[in] nEvent ��Ӧ���¼����ͣ���CSSYSEventMsg�е�ȡֵ
@return �¼���Mask������˵�����ԣ�������¼��Ƿ���Ч�ȣ�ΪCSSYSEventMask
@see CSSYSVIDRegisterVideoShowCallback CSSYSShowVideoByEventMsg CSSYSShowVideoSetEventMask
*/
DWORD CSSYSShowVideoGetEventMask(HCSHANDLE hHandle, int nEvent); 

/**
@brief ���ý�Ŀvideo/pcr��Ϣ

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] pstVIDInfo �����Ŀ����Ƶ��Ϣ
@param[in] pstPCRInfo �����Ŀ��PCR��Ϣ�������PCR��Ϣ�������գ�PCRPID�Ƿ��൱����PCR���Կ���������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���û�����video/pcr��Ŀ��Ϣ�������������ͣ���ʽ��pid�ȣ����������������Ƶ���ͣ�����Ĭ�����Ͳ�����	
*/
BOOL CSSYSVIDSetInput(HCSHANDLE hHandle, CSSYSVIDInputInfo *pstVIDInfo,CSSYSPCRInputInfo*pstPCRInfo,HCSHANDLE hReserved );

/**
@brief ������Ƶ����

@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ����CSVIDSetInput()�����ú������õ���Ƶpid����Ƶ��������Բ���������Ƶ�Ĳ���.
@note ��Stop״̬���ٴε���Play����TRUE���������¿�ʼ����,�����õ�PidҲ����Ч
*/
BOOL CSSYSVIDPlay(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief ֹͣ��Ƶ����

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ֹͣ��Ƶ���ţ���CSVIDPlay�ɶ�ʹ�á�
@note Stop״̬���ٴε���Stop����TRUE
*/
BOOL CSSYSVIDStop(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief ��ͣ��Ƶ����

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��ͣ��hHandle��ص���Ƶ�Ĳ��ţ������������ʾ��ֻ�����ڷ�Stop״̬�µ��ò���Ч,�ּ���Pauseʱ�ٴε��÷���TRUE
@note �����ʵʱ��������֡,�ָ�����Ҫԭ�ؼ�������;�����ע�벥������ͣ������ԭ�ػָ�
@note VODϵͳһ�����ǰ�˽�����ͣ���ţ��������������ñ�ϵ�нӿ�
*/
BOOL CSSYSVIDPause(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief �ָ���ͣģʽ�����Ŵ���ͣ�̿�ʼ�����Ƶ��

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ָ���ͣģʽ;��CSSYSVIDPause�ɶ�ʹ��
@note ��Resume״̬���ٴ�Resume�Է���TRUE
*/
BOOL CSSYSVIDResume(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief ����ָ�������Ļ��߱�

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] enmAspectRatio ��Ƶ����˿ڵĿ�߱ȣ���4:3 �� 16:9
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �Զ���ʾ���ݽ�Ŀ�Ŀ�߱��Զ�������Ļ��߱ȣ�Ҳ������߱�Ч��������ʾ�豸����
@note ����ָ�������Ļ��߱ȣ�Ŀǰֻ֧��16:9/4:3������Ч����Ҫ���������ŵĽ�Ŀ�Ŀ�߱ȣ�
�Լ����õ���Ӧģʽ���Լ����ӻ��Ƿ�������Ӧ������ȷ����Ŀǰֻ֧��video�㡣
@note һ����˵,���ӻ�������������Ļ��߱Ȳ�һ�µĽ�ĿԴʱֻ��������ģʽ���д���
����˼·���ɵõ��������Ӧ�е���ʾЧ��,��Ҳ�����������е�������,�����ע������Ч��
*/
BOOL CSSYSVIDSetAspectRatio(HCSHANDLE hHandle, CSVIDAspectRatioType enmAspectRatio, HCSHANDLE hReserved );

/**
@brief ����ֹͣ��Ƶ��ģʽ

����̨ʱ����Ƶ�л�Ч�����羲֡������
@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] eStopMode ֹͣģʽ
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSYSVIDSetStopMode(HCSHANDLE hHandle, CSVIDStopMode eStopMode);

/**
@brief ����ָ�������Ļ����Ӧģʽ

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] MatchMethod ��Ƶ����˿ڵ���Ӧģʽ����PAN_SCAN,LETTER_BOX,��
@param[in] hReserved  ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ����ָ�������Ļ����Ӧģʽ������Ч����Ҫ���������ŵĽ�Ŀ�Ŀ�߱ȣ�
�Լ����õ���Ӧģʽ���Լ����ӻ��Ƿ�������Ӧ������ȷ����Ŀǰֻ֧��video�㡣
@see CSVIDMatchMethod
*/
BOOL CSSYSVIDSetMatchMethod(HCSHANDLE hHandle, CSVIDMatchMethod MatchMethod, HCSHANDLE hReserved);

/**
@brief ��������ֱ��ʼ�ˢ��Ƶ��

����ָ��������������ֱ���
@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] enmResolution ϵͳ����ֱ���, 720*480,720*576, 1920*1080 etc
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��������豸(�ӿ�)һ������
@note �������õ�����������ķֱ���,����OSD�ȣ����ǽ�������Ƶ
@note ���ƽ̨֧��OSD�ȵ��������뽫OSD���ŵ��÷ֱ�����;���򰴾��д���
@note ���ر�ע����Ƶ����ԭʼˢ��Ƶ�ʲ��ſ��ܻ�����쳣,�����������һ����˵���鰴��Ƶ��Ŀ��ֵ��������
*/
BOOL CSSYSVIDSetResolution(HCSHANDLE hHandle, CSVIDTimingMode enmResolution, HCSHANDLE hReserved );

/**
@brief ����ָ����Ĵ��ڴ�С��ʵ�����Ź���

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] enmWindow ָ�����������ͼ�β�
@param[in] pstRect ���ڵĴ�С�Լ�����λ�ã��ο�CSWINRECT�ṹ��
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���pstRectΪNULL������ʾΪȫ����Ŀǰֻ֧��video��
*/
BOOL CSSYSVIDSetWindowSize(HCSHANDLE hHandle, CSVIDWindow enmWindow, CSWINRECT* pstRect, HCSHANDLE hReserved );

/**
@brief ��ʾ/������Ƶ�����

���û��������㣬osd�㣬video�㣬iframe�㡣�ýӿ�ʵ�ֶ�ָ����Ŀɼ��벻�ɼ���
����Ŀǰ�����Ϊosd���ϣ�video��Σ�iframe����ײ㣬����osd�ᵲסvideo��video�ᵲסiframe
@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] enmWindow ָ�����������ͼ�β�
@param[in] bIsShow TRUE:��ʾ�ô���;FALSE:���ظô���
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note 
*/
BOOL CSSYSVIDShow(HCSHANDLE hHandle, CSVIDWindow enmWindow, BOOL bIsShow, HCSHANDLE hReserved );

/**
@brief ����ָ�����͸����

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] enmWindow ָ�����������ͼ�β�
@param[in] nAlpha ���ڵ�͸�������ԣ�ȡֵ[0��100];
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ����ָ�����͸���ȣ���ΧΪ0-100������0Ϊ��ȫ͸����100Ϊ��͸��
*/
BOOL CSSYSVIDSetAlpha(HCSHANDLE hHandle,CSVIDWindow enmWindow, int nAlpha, HCSHANDLE hReserved );

/**
@brief ����ָ���������

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] nBrightness ��Ƶ��������ȣ�ȡֵ[0��100]
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ����ָ��������ȣ�0Ϊȫ�ڣ�100Ϊ������ߡ�Ŀǰֻ֧��video�㡣
@note ���ȵ�Ĭ��ֵΪ50,����ֵ����Ϊ50ʱ,�������Ŀ���ֳ����Ч��
*/
BOOL CSSYSVIDSetBrightness(HCSHANDLE hHandle, int nBrightness, HCSHANDLE hReserved );

/**
@brief ����ָ����ĶԱȶ�

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] nContrast ��Ƶ����ĶԱȶȣ�ȡֵ[0��100]��
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ����ָ����ĶԱȶȣ�0Ϊȫ�ڣ�100Ϊ�Աȶ���ߡ�Ŀǰֻ֧��video�㡣
@note �Աȶȵ�Ĭ��ֵΪ50,����ֵ����Ϊ50ʱ,�������Ŀ���ֳ����Ч��
*/
BOOL CSSYSVIDSetContrast(HCSHANDLE hHandle, int nContrast, HCSHANDLE hReserved );

/**
@brief ��ȡ��ǰ��Ƶ״̬

@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@param[out] pstVIDStatus ָ��洢��ǰ��Ƶ״̬���ڴ�ռ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSYSVIDGetStatus(HCSHANDLE hHandle,CSSYSVIDStatus_S* pstVIDStatus);	

/**
@brief ����ָ����ı��Ͷ�

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] nSaturation ��Ƶ����˿ڵı��Ͷȣ�ȡֵ[0��100]��
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ����ָ����ı��Ͷȣ�0Ϊȫ�ڣ�100Ϊ���Ͷ���ߡ�Ŀǰֻ֧��video��
@note ���Ͷȵ�Ĭ��ֵΪ50,����ֵ����Ϊ50ʱ,�������Ŀ���ֳ����Ч��
*/
BOOL CSSYSVIDSetSaturation(HCSHANDLE hHandle, int nSaturation, HCSHANDLE hReserved );

/**
@brief ���ô��ڼ���λ�úͳߴ�

���ú����ʾ�������Ƶ������ʵ�־ֲ��Ŵ���С�Ȳ���
@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] psRect ָ�����е�����
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSSYSVIDSetWinClip(HCSHANDLE hHandle, const CSWINRECT psRect);

/*------------------���½ӿ�ʵ����PVRδ���ã�������ý�岥��ȴ�����õ�--------------*/

/**
@brief ������,����,��λģʽ

Ϊ��������ˡ���λ����׼����������������λ�ӿ�ǰ����
@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ�һ�����ڴ�Demuxע��������Կ��Ʋ����ٶȵ�
*/
BOOL CSSYSVIDTrickModeStart(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief �˳����,����,��λģʽ

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ�һ�����ڴ�Demuxע��������Կ��Ʋ����ٶȵ�
*/
BOOL CSSYSVIDTrickModeStop(HCSHANDLE hHandle, HCSHANDLE hReserved );

/**
@brief ����Video�Ĳ����ٶ�

���ڱ���ý�岥��ʱ�����������ٶ�֮��
@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] nSpeed ���ŵ��ٶ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �����ٶ�:1000Ϊ���ٲ���,2000��ʾ2���٣�500��ʾ�������0Ϊpause,����Ϊ���򲥷�,����-1000��ʾ����һ����
@note ��������PVR��Ӧ��(������);������ƽ̨��֧�������ʱ��δ���(������)
@note VODϵͳһ�����ǰ�˽��в����ٶȵ����ã��������������ñ�ϵ�нӿ�
@note �ýӿ�һ�����ڴ�Demuxע��������Կ��Ʋ����ٶ�
*/
BOOL CSSYSVIDSetSpeed(HCSHANDLE hHandle, int nSpeed);

#ifdef  __cplusplus
}
#endif
/** @} */
#endif


