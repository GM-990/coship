/**@defgroup VIDEO VIDEO  ������ƽ̨�޹ص���Ƶ�����ӿ�
@brief VIDEOģ���Ӧͷ�ļ�<udi2_video.h>��������Ƶ���������Լ����ƽӿ� ��

һ�����Բ�����Ҫ������������Ƶ����������Ϊһ��Video����������pipϵͳ��������������������ڷ�pipϵͳ�У���ʹMpeg2��Mpeg4�������������ڹ�����Ҳ�������һ��Video��������

I֡�Ľ���ͨ������һ��inject��һ��player��Ȼ�����ݴ�injectע�룬��player���в���, - �ٽ���Snapshot�����������С��Ϣ��ͨ��GetStreamStatus.

Video��������������0��ʼ,һ����˵0��ʾ����������������������Ƶ���ǵ�������������Ƶ�����棬Ȼ��ͬ��ϵ�Screen�ϣ���������Hide��

����֮�⣬��Ƶ�ڽ���ʱ�������һЩ�����ϵı仯�������������š��ı�ֱ��ʡ����Ӻڱߵȵȡ���Щ������Ƶ��������ɡ�

@version 2.0.4 2009/09/24 �������òü��Ⱦ��εĲο�ϵ˵��
@version 2.0.3 2009/09/15 ������Ļ��Ӧģʽ��ͼʾ˵��
@version 2.0.2 2009/08/27 ȥ���Բ���jpgͼƬ��֧��
@version 2.0.2 2009/08/25 ����Video������򼰲���jpg�ȹ���˵��
@version 2.0.1 2009/08/16 ����
@{
*/
#ifndef _UDI2_VIDEO_H_
#define _UDI2_VIDEO_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_screen.h"
#include "udi2_osg.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief ��Ƶ�豸������ش������ֵ*/
enum
{
	CSUDIVIDEO_ERROR_BAD_PARAMETER = CSUDI_VIDEO_ERROR_BASE,  ///< ��������
	CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED,					///< ��֧�ֲ���
	CSUDIVIDEO_ERROR_UNKNOWN_ERROR,					   		///< һ�����
	CSUDIVIDEO_ERROR_NO_MEMORY,					    		///< �޿����ڴ�
	CSUDIVIDEO_ERROR_INVALID_DEVICE_ID,					    ///< �Ƿ��豸ID
	CSUDIVIDEO_ERROR_CALLBACK_EXIST,         					///< Ҫע��Ļص��Ѿ�����
	CSUDIVIDEO_ERROR_CALLBACK_NOT_EXIST 					///< Ҫɾ���Ļص�������
};

/**@brief VIDEO�豸������Ϣ
@note �����У������������ΪEM_UDI_VID_STREAM_INVALID�����ʾ֧�ֵ���Ƶ�����Ѿ��������������ݽ������ԡ�
*/
typedef struct
{
	  CSUDIVIDStreamType_E   m_eStreamType[EM_UDI_VID_STREAMTYPE_NUM];	   ///< �豸֧�ֵ���Ƶ������ʽ
} CSUDIVIDEOCapability_S;

/**@brief ��Ƶֹͣģʽ*/
typedef enum
{	
	EM_UDIVIDEO_STOPMODE_BLACK,	///< ���� 
	EM_UDIVIDEO_STOPMODE_FREEZE	///< ��֡ 
	
}CSUDIVIDEOStopMode_E;

/**@brief ��Ƶ������Ϣ*/
typedef struct 
{
	CSUDIVIDStreamType_E 	m_eVidStreamType;		///< ��Ƶ������
	int 			                	m_nSourceWidth;		///< Դ��Ƶ�����
	int 			                	m_nSourceHight;		    	///< Դ��Ƶ���߶�
	int 			                	m_nFrameRate;		    	///< Դ��Ƶ������
	CSUDI_BOOL	                	m_bInterlaced;		    	///< CSUDI_TRUE��ʾ����;CSUDI_FALSE��ʾ����
	int			                	m_dwFrameCount;		///< ��Ƶ�Ѳ��ŵ���֡��
}CSUDIVIDEOStreamStatus_S;

/**@brief ��Ƶ��߱�*/
typedef enum 
{
	EM_UDIVIDEO_ASPECT_RATIO_UNKNOWN,  
	EM_UDIVIDEO_ASPECT_RATIO_4_3,	        ///< 4��3
	EM_UDIVIDEO_ASPECT_RATIO_16_9,	        ///< 16��9 
	EM_UDIVIDEO_ASPECT_RATIO_AUTO		 ///<  �Զ�ѡ�� 
}CSUDIVIDEOAspectRatioType_E;

/**
@brief ��Ƶ�����߱�ת������,��ο���Ƶ�����߱�ת��ʾ��ͼ
@see CSUDIVIDEOSetMatchMethod
*/
typedef enum 
{
	EM_UDIVIDEO_MATCH_METHOD_LETTER_BOX,	///<  Letter Box 
	EM_UDIVIDEO_MATCH_METHOD_PAN_SCAN,	///<  Pan Scan 
	EM_UDIVIDEO_MATCH_METHOD_COMBINED,	///<  ComBined 
	EM_UDIVIDEO_MATCH_METHOD_IGNORE,	      ///< Ignore 
	EM_UDIVIDEO_MATCH_METHOD_NUM                 ///< ���ޡ�
}CSUDIVIDEOMatchMethod_E;

/**@brief VBI ��������*/
typedef enum 
{   
    EM_UDIVIDEO_VBI_TELTEXT=(1<<0),   ///< teltext
    EM_UDIVIDEO_VBI_CLOSECAPTION=(1<<1),///<VBI CLOSE CAPTION
    EM_UDIVIDEO_VBI_MACROVISION=(1<<2), ///< Macrovision
    EM_UDIVIDEO_VBI_CGMS=(1<<3) 		   ///< CGMS
}CSUDIVIDEOVBIType_E;

/**@brief ��Ƶ�¼�ö�ٶ���*/
typedef enum 
{	
	EM_UDIVIDEO_USERDATA,		    	///< ��Ƶ������user data�¼����ص���pvEventDataָ��CSUDIVIDEOUserData_S�ṹ��
	EM_UDIVIDEO_MAXEVENTTYPE		///< �߽�ֵ
}CSUDIVIDEOEventType_E; 

/**@brief video�е�user data���ݽṹ����*/
typedef struct
{
	void *			m_pVideoUserdata;				///< UserData����Buffer��ַ
	unsigned int		m_uLength;						///< UserData����
	CSUDI_UINT64 	m_u64Pts;						///< PTS 
	CSUDI_BOOL		m_bIsRegistered;					///< ��ʾ�Ƿ�Ϊ��ע���UserData��ΪTRUE��ṹ���Ա(m_ucCountryCode, m_ucCountryCodeExtensionByte & m_uProviderCode)��Ч
	CSUDI_UINT8		m_ucCountryCode;				///< Country code following ITU-T recommendation T.35
	CSUDI_UINT8		m_ucCountryCodeExtensionByte;	///< Country code extension byte. ע�⣺����m_ucCountryCodeΪ0xFF��Ч
	CSUDI_UINT16	m_uProviderCode;				///< manufacturers code
	unsigned int		m_adwReserved[4];				///< �����ֶ�
} CSUDIVIDEOUserData_S;

 /**
@brief video�ص������Ķ���

@param[in] nVideoIndex ��Ƶ��������������
@param[in] eEvent �ص���Ϣ���ͣ����CSUDIVIDEOEventType_E���壻
@param[in] pvEventData�ص���Ϣ���ݣ����CSUDIVIDEOEventType_E��ö�پ��嶨�壻
@param[in] pvUserData  �û���������
*/
typedef  void ( *CSUDIVIDEOCallback_F  )(int nVideoIndex, CSUDIVIDEOEventType_E eEvent, void * pvEventData, void * pvUserData);

 /**
@brief  ע��ָ���¼����͵Ļص�����

@param[in] nVideoIndex ��Ƶ������������
@param[in] fnVideoCallback �ص�����ָ�룬���CSUDIVIDEOCallback_F����
@param[in] eEvent �ص��������ͣ����CSUDIVIDEOEventType_E
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����֧��ע��Ļص���������Ϊ32���������
@note ������ע��EM_UDIVIDEO_MAXEVENTTYPE���͵Ļص�
@note �ص�����ָ��+�ص���������+�û�����Ψһ��ʶһ���ص�,������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIVIDEO_ERROR_CALLBACK_EXIST
*/
CSUDI_Error_Code CSUDIVIDEOAddCallback(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent,void * pvUserData );

 /**
@brief ɾ��ָ���¼����͵Ļص�����

@param[in] nVideoIndex ��Ƶ������������
@param[in] fnVideoCallback �ص�����ָ�룬���CSUDIVIDEOCallback_F����
@param[in] eEvent �ص��������ͣ����CSUDIVIDEOEventType_E
@param[in] pvUserData ͬCSUDIVIDEOAddCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ������CSUDIVIDEO_ERROR_CALLBACK_NOT_EXIST
@note �����ڻص�������ɾ���ص�
*/
CSUDI_Error_Code CSUDIVIDEODelCallback(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent, void * pvUserData );
 
/**
@brief ����VBI��ʽ��teletext\close caption\Macrovision ���ܿ���

@param[in] nVideoIndex ��Ƶ������������
@param[in] eVideoVBIType VBI�������ͣ�������Ͽ���
@param[in] bEnable �򿪻�ر�eVideoVBIType��Ӧ�Ĺ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ϵͳ����ʱ���й���Ĭ�Ϲرգ�ֻ�е��ñ��ӿڲ��ܴ򿪻�رն�Ӧ���ܣ�����һ��������Ч
@note ������ΪEM_UDIVIDEO_VBI_TELTEXTʱ����Ҫͬplayer�ӿ�CSUDIPLAYERSetStream���ʹ��,eg:   
@note ����VBI teltext ����
@note  sStreamInfo.m_eContentType=EM_UDI_CONTENT_TELTEXT;
@note  sStreamInfo.m_nPid=teltext_pid;
@note  CSUDIPLAYERSetStream(hPlayer,&sStreamInfo,nStreamCnt,CSUDI_NULL);
@note  CSUDIVIDEOSetVBIEnable(nVideoIndex,EM_UDIVIDEO_VBI_TELTEXT, CSUDI_TRUE);
@note  ������������Ҫ����PID,��ֻ�����CSUDIVIDEOSetVBIEnable�ӿ�
*/
CSUDI_Error_Code CSUDIVIDEOSetVBIEnable(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL bEnable);

/**
@brief ��ȡVBI��������teletext\close caption\Macrovision ����״̬

@param[in] nVideoIndex ��Ƶ������������
@param[in] eVideoVBIType VBI�������ͣ�ÿ��ֻ�ܲ�ѯһ������
@param[out] pbEnable ,eVideoVBIType��Ӧ�Ĺ��ܿ���״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVIDEOGetVBIEnable(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL* pbEnable);

/**
@brief ��ȡVideo Decoder�豸������

@param[out] pnVideoDecCount ϵͳ�� ����Video Decoder�豸�ĸ���
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ�����������pnVideoDecCount�����ݲ�ȷ��
*/
CSUDI_Error_Code CSUDIVIDEOGetCount(int * pnVideoDecCount);

/**
@brief ��ȡָ��Video Decoder�豸��������Ϣ

���ۿ��л��߱�ռ��
@param[in] nVideoIndex Video Decoder�豸������
@param[out] psCapabilityInfo Video Decoder�豸������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ�����������psCapabilityInfo�ṹ�����ݲ�ȷ��
*/
CSUDI_Error_Code CSUDIVIDEOGetCapability(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo);

/**@brief ��Ƶ��ͼ(����)�ӿڣ�����ǰ������ʾ����Ƶͼ��Ĳ���(��ȫ��)��ȡ�������Ŀ��ͼ���ָ������

@param[in] nVideoIndex  Video Decoder�豸������
@param[in] psSrcRect ���봰�ڴ�С����ʾҪ��ȡ��ǰ��Ƶͼ�����һ���֣�ΪCSUDI_NULL��ʾ����ͼ��ע�ⲻ�ܳ�����ǰ��Ƶͼ���С��
@param[in] psDstRect ������ڴ�С����ʾ�����hSurface����һ���֣�ΪCSUDI_NULL��ʾ����hSurface��ע�ⲻ�ܳ���hSurface��С��
@param[out] phSurface ��Ƶ��ͼ�������hSurface��ָ�����򣬸�Surface�ǵ���OSG�ӿڴ����Ļ������ֱ�ӻ�ȡ�Դ棻
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����hSurface�ṹ������δ���塣
@note �ڽ�ȡI֡ͼƬʱ��ͨ������CSUDIVIDEOGetStreamStatus��ȡ��ǰI֡��Ϣ������ͼ������Ϣ��
@note �����Ʒ����Ҫ��Ƶ���չ��ܻ���Ӳ����֧�֣��ýӿڿ��Բ�ʵ�֡���ʱ�践��CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED��
*/
CSUDI_Error_Code CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect, CSUDI_HANDLE* phSurface);

/**
@brief ��ȡ����������״̬��Ϣ

�統ǰ���ŵ���Ƶ��С��
@param[in]  nVideoIndex Video Decoder�豸��Դ������
@param[out] psStreamStatus ����IFRAME��Ϣ ��CSUDIVIDStreamStatus_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psStreamStatus�ṹ������δ����
*/
CSUDI_Error_Code CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus);

/**
@brief ��ʾ/������Ƶ�����������

@param[in] nVideoIndex  ��Ƶ������������
@param[in] bIsShow ΪCSUDI_TRUE���ʾ��ʾ�ô���;ΪCSUDI_FALSE���ʾ���ظô���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ 
@note ϵͳ��ʼ��ʱӦΪ��ʾ״̬
*/
CSUDI_Error_Code CSUDIVIDEOShow(int nVideoIndex, CSUDI_BOOL bIsShow);

/**
@brief ����ָ����Ƶ�������Ļ��߱�

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[in] eAspectRatio ��Ƶ����ӿڵĿ�߱ȣ���4:3 �� 16:9
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note auto��ʾ���ݽ�Ŀ�Ŀ�߱��Զ�������Ļ��߱ȣ�Ҳ������߱�Ч�������ײ���������
@note ����ָ����Ƶ����ӿ��������Ŀ�߱ȣ�Ŀǰֻ֧��16:9/4:3/auto��ģʽ��
����Ч�������������ŵĽ�Ŀ�Ŀ�߱��Լ�ָ����Ƶ����ӿڵ���Ӧģʽ(MatchMethod)������
@note �˽ӿ����õ���ʾЧ��ֻ����Ƶȫ����ʾʱ��Ч������ƵС������ʾʱ�������ã������۴˽ӿ�������ã���ƵС������ʾʱʼ��������С���ڵ�
*/
CSUDI_Error_Code CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio);

/**
@brief��ȡָ����Ƶ�������Ļ��߱�

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[out] peAspectRatio ��Ƶ����ӿڵĿ�߱ȣ���4:3 �� 16:9
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���ӿ����ڻ�ȡ��ǰ���õ���Ļ��߱�״̬��������õ���AUTO���򷵻�AUTO��
���������CSUDIVIDEOGetStreamStatus�ӿڻ��
*/
CSUDI_Error_Code CSUDIVIDEOGetAspectRatio(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E *peAspectRatio);


/**
@brief����ָ����Ƶ����ӿڵ���Ӧģʽ

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[in] eMatchMethod ָ����Ƶ����˿ڵ���Ӧģʽ����PAN_SCAN,LETTER_BOX,��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����ָ����Ƶ����˿ڵ���Ļ��Ӧģʽ��
����Ч�������������ŵĽ�Ŀ�Ŀ�߱��Լ�ָ����Ƶ����ӿڵĻ����(AspectRatio)�����أ���ο���ͼ��
@image HTML VIDAspectRatioType.png
@note �˽ӿ����õ���ʾЧ��ֻ����Ƶȫ����ʾʱ��Ч������ƵС������ʾʱ�������ã������۴˽ӿ�������ã���ƵС������ʾʱʼ��������С���ڵ�
*/
CSUDI_Error_Code CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E eMatchMethod);

/**
@brief��ȡָ����Ƶ����ӿڵ���Ӧģʽ

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[out] peMatchMethod ָ����Ƶ����˿ڵ���Ӧģʽ����PAN_SCAN,LETTER_BOX,��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVIDEOGetMatchMethod(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E *peMatchMethod);

/**
@brief ����ֹͣ��Ƶ��ģʽ

����Ŀ�л�ʱ����Ƶ�л�Ч�����羲֡������
@param[in] nVideoIndex  Video Decoder�豸��Դ������
@param[in] eStopMode ֹͣģʽ��������ο�CSUDIVIDEOStopMode_E��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode);

/**
@brief��ȡֹͣ��Ƶ��ģʽ

����Ŀ�л�ʱ����Ƶ�л�Ч�����羲֡������
@param[in] nVideoIndex  Video Decoder�豸��Դ������
@param[out] peStopMode ֹͣģʽ��������ο�CSUDIVIDEOStopMode_E��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVIDEOGetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E *peStopMode);

/**
@brief ����ָ����Ƶ��������Ĵ��ڴ�С��ʵ�����Ź���

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ���ڴ�С
@param[in] pstRect ���ڵĴ�С�Լ�����λ�ã��ο�CSUDIWinRect_S�ṹ�����pstRectΪNULL������ʾΪȫ����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note pstRectΪ����뵱ǰ��Ƶ�ֱ��ʵľ���,����Ϣ���Դ�CSUDISCREENGetResolution�ӿڻ�ȡ
@note �����ǲü�����Ϊ���������
*/
CSUDI_Error_Code CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect );

/**
@brief ��ȡָ����Ƶ��������Ĵ��ڴ�С

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ���ڴ�С
@param[out] pstRect ���ڵĴ�С�Լ�����λ�ã��ο�CSUDIWinRect_S�ṹ��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVIDEOGetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * pstRect );

/**
@brief ���ô��ڼ���λ�úͳߴ�

���ú�������Ƶ���ڽ���ʾ�����������Ƶ������ʵ�־ֲ��Ŵ���С�Ȳ�����
�����ú��ƶ�����������Screen����ʾ(���û�����ù�CSUDIVIDEOSetWindowSize��Ϊȫ����ʾ)
@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ��������
@param[in] psRect ָ�����е��������CSUDIWinRect_S���塣
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���psRectΪCSUDI_NULL�����ʾȫ����ʾ
@note pstRectΪ����뵱ǰ��Ƶ�ֱ��ʵľ���,����Ϣ���Դ�CSUDISCREENGetResolution�ӿڻ�ȡ
*/
CSUDI_Error_Code CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect);

/**
@brief ��ȡ���ڼ���λ�úͳߴ�

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ��������
@param[out] psRect ���е��������CSUDIWinRect_S���塣
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIVIDEOGetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * psRect);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif
