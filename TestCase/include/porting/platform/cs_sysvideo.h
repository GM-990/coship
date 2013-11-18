/** @addtogroup CSVideo
@{
*/

#ifndef _CS_SYSVIDEO_H_
#define _CS_SYSVIDEO_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "udi_video.h"
#include "udi_osg.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
@name ��Ƶ����ֱ����ض���
@deprecated ���鶨���Ϊ���ּ��ݶ��������Ƽ�ֱ��ʹ��CSVIDTimingMode
@{*/ 
#define CSVID_TIMING_MODE_480I59940_13500		CSVID_TIMING_MODE_NTSC
#define CSVID_TIMING_MODE_576I50000_13500		CSVID_TIMING_MODE_PAL
#define CSVID_TIMING_MODE_576P50000_27000		CSVID_TIMING_MODE_576P
#define CSVID_TIMING_MODE_720P60000_74250		CSVID_TIMING_MODE_720P
#define CSVID_TIMING_MODE_720P50000_74250		CSVID_TIMING_MODE_720P_50HZ
#define CSVID_TIMING_MODE_1080I60000_74250		CSVID_TIMING_MODE_1080I
#define CSVID_TIMING_MODE_1080I50000_74250_1 		CSVID_TIMING_MODE_1080I_50HZ
/*@}*/

/**@brief �ض�����Ƶ�����߱�ת������
@deprecated �������Ϊ���ּ��ݶ��������Ƽ�ֱ��ʹ��CSVIDMatchMethod
*/
#define CSVIDDisplayARConversionType  			CSVIDMatchMethod

/**@brief ��Ƶ���ģʽ*/
typedef enum _CSVIDOutputMode
{
	CSVID_OUTPUT_SD_MODE 	=0x00000001,	/**< SD mode. 576i/480i*/
	CSVID_OUTPUT_ED_MODE	=0x00000002,	/**< ED mode. 576p/480p*/
	CSVID_OUTPUT_HD_MODE	=0x00000004	    /**< HD mode. 1080p/1080i/720p etc.*/
}CSVIDOutputMode;

/**@brief ��Ƶ����˿ڵ���ɫģʽ */
typedef enum
{
	CSVID_COLOR_MODE_YUV,	/**< YUVģʽ */
	CSVID_COLOR_MODE_RGB, 	/**< RGBģʽ */
	CSVID_COLOR_MODE_NUM	 /**< ģʽ���� */
}CSSYSVIDOutputColorMode;

/**@brief �����ע��*/
typedef struct _CSOnVideoNeedData_S
{ 
	int     nJumpTimeMs;		/**< �����ע��*/
	UINT    uNeedDataSize; 		/**< �����ע��*/
	void *  pUserData;		    /**< �����ע��*/
}CSOnVideoNeedData_S; 

/**@brief Image Quality Improvement  ͼ��������������; ע�� ��ST7111  ST7105 ��Ч*/
typedef enum _CSVIDIQILevel
{
	CSVID_IQI_LEVEL_DISABLE,			/**< Disable  Level */
	CSVID_IQI_LEVEL_LOW,				/**< Low	Level  */
	CSVID_IQI_LEVEL_MID,				/**< Middle 	Level  */
	CSVID_IQI_LEVEL_HIGH,				/**< High 	Level  */
	CSVID_IQI_LEVEL_MANUAL,			/**< Manual Level */
	CSVID_IQI_LEVEL_NUM
}CSVIDIQILevel;

/**@brief Image Quality Improvement  ͼ��������������; ע�� ��ST7111  ST7105 ��Ч*/
typedef enum _CSVIDIQIMode
{
	CSVID_IQI_MODE_LTI,					/**< LTI Mode ˲ʱ������ǿ 	*/
	CSVID_IQI_MODE_CTI,					/**< CTI Mode ˲ʱɫ����ǿ	*/
	CSVID_IQI_MODE_PEAKING,				/**< Peaking Mode ���ȷ廯	*/
	CSVID_IQI_MODE_LUMA_ENHANCER,		/**< Luma 	Mode  ������ǿ	*/
	CSVID_IQI_MODE_DEMO,				/**< Demo ������IQI ���IQI��ͼ����죬�ֱ�Ϊ��ߣ��ұߣ��м�ͼ����������	*/
	CSVID_IQI_MODE_ZOOM_FILTERSET,		/**< ͼ��ƽ����*/
	CSVID_IQI_MODE_NUM
}CSVIDIQIMode;

/**
@brief ��Ƶ�����¼��ص�����

���ײ㲥����Ƶʱ������Ӧ�¼�����øú���֪ͨ����Ҫ���������������Ƶ���ƹ���
�ýӿ�Ŀǰ��δʵ�֣��Ƽ�����������CSSYSShowVideoCallback�޷�����ʱʹ�ñ��ӿڽ�����չ���������CSSYSShowVideoCallback
@param[in] dwEvent ��������Ƶ�¼���ΪCSSYSVidEvent_E�е�����ֵ
@param[in] pvParam ��Ӧ�Ļص����ݽṹ�ṹ,Ŀǰ��ΪNULL
@param[in] pvUserData �û�����,ע��ʱ����
*/
typedef void (*FVideoCallback)(DWORD dwEvent,PVOID pvParam,PVOID pvUserData);

/**
@brief ע����Ƶ�¼��ص�����

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] fnCallback �ص�����������Ϊ��
@param[in] pvUserData �û��Զ������ݣ�����fnCallback��������ʱ��pvUserData���ظ��û�
@note �ɱ���ε��ã������һ�ε��ý��Ϊ׼��fnCallbackΪNULLΪ��ע��
@see CSSYSVIDSetEventMask CSSYSVIDGetEventMask
*/
BOOL CSSYSVIDRegisterEvent(HCSHANDLE hHandle,FVideoCallback fnCallback,PVOID pvUserData);

/**
@brief  ������Ƶ�¼���Ч��

����ǰ�Ƿ��עĳ����Ƶ�¼�
@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@param[in] dwEventMask ���õ�ǰvideoģ��Ҫ��ص��¼�,ΪCSSYSVidEvent_E��ö�ٱ�������ϻ�ֵ
@note ����˵��:���dwEventMask=EM_VID_SOURCE_CHANGED_EVT|EM_VID_OVERFLOW_EVT
��ô������ĿԴ�����仯����video buffer ovferlow��CSSYSVIDRegisterEvent����ע��Ļص������ᱻ����
@see CSSYSVIDRegisterEvent CSSYSVIDGetEventMask
*/
BOOL CSSYSVIDSetEventMask(HCSHANDLE hHandle,DWORD dwEventMask);

/**
@brief ��ȡ��Ƶ�¼���Ч��

����ȡCSSYSVIDSetEventMask�������������
@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@param[out] pdwEventMask ��ǰvideoģ���ص��¼���ΪCSSYSVidEvent_E��ö�ٱ�������ϻ�ֵ
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@see CSSYSVIDRegisterEvent CSSYSVIDSetEventMask
*/
BOOL CSSYSVIDGetEventMask(HCSHANDLE hHandle,DWORD* pdwEventMask);

/**
@brief ��֡�¼��ص�

������֡�����ʱ���øú���֪ͨ
@param[in] hHandle �ײ��豸�����������Ӧ��ͨ����CSRMConnect()�������
@param[in] pUserData �û��Զ������
@deprecated �ѱ�FVideoCallback�ص���������ٽ���ʹ��
*/
typedef void(*FOnVidFrameCome)(HCSHANDLE hHandle, PVOID pUserData);

/**
@brief ע����֡�¼��ص�

@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������������ο��豸�����ӿ��ĵ����塣
@param[in] fnCallback �ص�����
@param[in] pUserData �û��Զ������
@deprecated �ѱ�FVideoCallback�ص���������ٽ���ʹ��
*/
void CSSYSVIDRegNewFrameCome(HCSHANDLE hHandle, FOnVidFrameCome fnCallback, PVOID pUserData);

/**
@brief �ж��豸(��)�Ƿ�֧��H.264��MPEG-4��ʽ�ĸ�����Ƶ����

@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@return ֧�ַ���TRUE,��֧�ַ���FALSE
@note ����豸handle�Ƿ�������FALSE(һ���Ƿ����豸���ܽ���H.264)
*/
BOOL CSSYSVIDIsSupportHD_H264(HCSHANDLE hHandle);

/**
@brief �ж��豸(��)�Ƿ�֧��MPEG-2��ʽ�ĸ�����Ƶ����

@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@return ֧�ַ���TRUE,��֧�ַ���FALSE
@note ����豸handle�Ƿ�������FALSE(һ���Ƿ����豸���ܽ���H.264)
*/
BOOL CSSYSVIDIsSupportHD(HCSHANDLE hHandle);

/**
@brief ���video picture buffer�����һ֡

@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@param[in] hUserData ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@deprecated ��ʹ��CSSYSVIDClearDecodeBuffer
*/
BOOL CSSYSVIDClearLastPicture(HCSHANDLE hHandle, HCSHANDLE hUserData );

/**
@brief ������Ƶ����ֱ����Զ�ѡ��

��CSSYSVIDSetResolution���õ�ǰ�ֱ���Ϊ�Զ�ʱ���Զ��л���������ָ���ķֱ���
@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] enmDefaultPal ����⵽��ƵΪPAL��ʱ���л���������ָ���ķֱ��ʣ�������һ��PAL�ֱ�����PALN,PALD��
@param[in] enmDefaultNtsc ����⵽��ƵΪNTSC��ʱ���л���������ָ���ķֱ��ʣ�������һ��NTSC�ֱ�����NTSCM,NTSC_443,NTSC_J��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@see CSSYSVIDSetResolution
*/
BOOL CSSYSVIDSetDefaultAutoResolution(HCSHANDLE hHandle, CSVIDTimingMode enmDefaultPal, CSVIDTimingMode enmDefaultNtsc);

/**
@brief ˮƽ�ƶ���Ƶ

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] nXoffset ˮƽƫ����, ȡֵ��Χ[-50, 50], Ĭ��ֵΪ0;������ʾ����,������ʾ����
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ���Ҫ��������Ƶ����΢����ʹ�ò�ͬ������������һ��
*/
BOOL CSSYSVIDSetPositionH(HCSHANDLE hHandle, int  nXoffset, HCSHANDLE hReserved );
 
/**
@brief ��ֱ�ƶ���Ƶ

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] nYoffset ��ֱƫ����, ȡֵ��Χ[-50, 50], Ĭ��ֵΪ0;������ʾ����,������ʾ����
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ���Ҫ��������Ƶ����΢����ʹ�ò�ͬ������������һ��
*/
BOOL CSSYSVIDSetPositionV(HCSHANDLE hHandle, int  nYoffset, HCSHANDLE hReserved );

/**
@brief ������Ƶ����豸

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] dwOutputDevice ��Ƶ����˿ڣ�ΪCSVIDOutputDevice�и������
@param[in] hReserved ����������Ŀǰ����ΪNULL
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���û�������Ƶ����豸,δ���õ��豸����������Ƶ�����
@note ���dwOutputDevice�д����κβ�֧�ֻ��߲���ͬʱ֧�ֵ��豸,
�򷵻�FALSE,���Ҳ������κ�����(����dwOutputDevice��ָ����֧�ֵ��豸)
@note ����һ��ƽ̨�����ԵĽӿ�,һ����˵ֻ��Ҫ�ڿ���ʱ����һ��
*/
BOOL CSSYSVIDSetOutputDevice(HCSHANDLE hHandle, DWORD dwOutputDevice,HCSHANDLE hReserved );

/**
@brief ������Ƶ����˿ڵ���ɫģʽ

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] eDevice ��Ƶ����豸
@param[in] eMode ��ɫģʽ��RGB����YUV
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note һ��ֻ֧��CSVID_OUTPUT_RGB/CSVID_OUTPUT_YUV/CSVID_OUTPUT_HDMI��
����CSVID_OUTPUT_RGB/CSVID_OUTPUT_YUV����ָ���������
����������Ч
*/
BOOL CSSYSVIDSetOutputDeviceColorMode(HCSHANDLE hHandle, CSVIDOutputDevice eDevice, CSSYSVIDOutputColorMode eMode);

/*----------------------------------ESע��ӿ�----------------------------------*/
/**
@brief VideoDecoder��Ҫ���ݵĻص����� 

ESע��ص�
@param[in] hHandle �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������
@param[in] pvParam ��Ӧ�Ļص����ݽṹ�ṹ
@note �ýӿ�ʵ������Ƶ����Trick Modeʱ���õĺ���������֧�ֻطź�ʱ�ơ�ʵʱ�����Ŵ˺�����Ч��
@note pvParamָ��һ��CSOnVideoNeedData_S�ṹ��
����nJumpTimeMs��ָ��һ��ע�������β���뱾��ע������ͷ�м��ʱ������
���ڻطŵ�Trick�������ø����ݾ�����ȡ���ݵ�λ�ã���ΪֵΪ0��ʾ��������ע��
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
*/
typedef void(*FOnVideoNeedData)(HCSHANDLE hHandle,  PVOID pvParam);

/**
@brief ��һ��ES��Ƶע��ͨ��

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] pstESBufferInfo ESע���Buffer����ص���Ϣ��������ƵBuffer�ĵ�ַ�ʹ�С��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ��ʼ��ESע������Ҫ��Buffer����ص���Ϣ��������buffer����Сͨ����ʼ���������룬Ĭ����Ƶ��СΪ2��
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
*/
BOOL CSVIDESInit(HCSHANDLE hHandle, CSESBufferInfo* pstESBufferInfo);

/**
@brief �ر�һ��ESע��ͨ����������Դ

��ֹES��ע�룬��������Դ
@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
*/
BOOL CSVIDESTerm(HCSHANDLE hHandle);

/**
@brief ���ESע��VideoBuffer�Ŀ��пռ�Ĵ�С

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[out] pnBufSize ESע������VideoBuffer�Ĵ�С��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���ESע��VideoBuffer�Ŀ��пռ�Ĵ�С
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
*/
BOOL CSESGetVideoBufferFreeSize(HCSHANDLE hHandle, int * pnBufSize);

/**
@brief ���Video ESע��Buffer����ʼ��ַ�ʹ�С

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] nBufSize Ҫ��ȡ��VideoBuffer�Ĵ�С
@param[out] ppucBuffer ESע�������VideoBuffer�ĵ�ַ
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���ESע��VideoBuffer�Ŀ��пռ�Ĵ�С
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
*/
BOOL CSESGetAvailVideoBuffer(HCSHANDLE  hHandle, int nBufSize, PBYTE *ppucBuffer);

/**
@brief ���ESע�������VideoBuffe

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ���ESע�������VideoBuffer
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
*/
BOOL CSESVideoBufferReset(HCSHANDLE hHandle);

/**
@brief ��Videobuffer��д������

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] pucBuffer Ҫ����ESע���video���ݵĴ�ŵĵ�ַ��
@param[in] nBufSize Ҫ����ESע���video���ݵĴ�С��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
@note ��Videobuffer��д�����ݡ�
*/
BOOL CSESVideoBufferPut(HCSHANDLE hHandle,  PBYTE pucBuffer, int nBufSize);

/**
@brief ����Video����ע���ͬ����Ϣ

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] nFirstInjectSize ��һ�ν���ESע���video���ݵĴ�С
@param[in] nFirstMilliSeconds ��һ�ν���video����ESע���ʱ��
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
@note ��Videobuffer��д�����ݡ�
*/
BOOL CSESSetVideoBufferSyncInfo(HCSHANDLE  hHandle,  int  nFirstInjectSize, int  nFirstMilliSeconds);

/**
@brief ���ע���AV ES��PTS��Ϣ

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[out] syncInfo ���صĵİ���ES����PTS���ݡ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ú����ӿ�����IPTV�ģţ���ע��ͬ������
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
*/
BOOL CSESGetVideoCurrentPTS(HCSHANDLE hHandle, CSESSyncInfoEx* syncInfo);

/**
@brief ����ES��ע�����Ƶ�������ģУԣ�

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] un64PTS �����õ���clock_t���͵���Ƶ�Уԣӡ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ú����ӿ�����IPTV�ģţ���ע��ͬ������
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
*/
BOOL CSESSetVideoPTSEx(HCSHANDLE hHandle, U64CLock_t un64PTS);

/**
@brief ��֪��Ƶ����������ע���ES����ͷ���ϴ�ע�������β�Ƿ��ǲ�����ע��

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] bDiscontinuity TRUE��������FALSE����
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ�ʵ������Ƶ����Trick Modeʱ���õĺ���������֧�ֻطź�ʱ�ơ�ʵʱ�����Ŵ˺�����Ч
@deprecated ��Ϊ���ּ��ݶ����������Ƽ�����Ƶ�ӱ�ͨ��ע�룬Ҳ���Ƽ�ʹ�ô˸�ʽ��Ƶ
*/
BOOL CSSYSVIDDiscontinuity(HCSHANDLE hHandle, BOOL bDiscontinuity);

/**
@brief �ڽ��յ�FOnVideoNeedData�ص�ע�����ݺ󣬵��øú�����֪��Ƶ������ESע�����

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ�ʵ������Ƶ����Trick Modeʱ���õĺ���������֧�ֻطź�ʱ�ơ�ʵʱ�����Ŵ˺�����Ч��
*/
BOOL CSSYSVIDInjectComplete(HCSHANDLE hHandle);

/**
@brief ����ES����ע�����Ƶ�ص�����

@param[in] hHandle ��Ƶ�������豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] pfnCallback �ϲ�ע������ڻ��decoder ��Ϣ�ص�����ָ�롣
@param[in] pvUserData �ṩ���ص������Ĳ���ָ�롣
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ûص���������IPTVӦ�ã���Ҫ����֪ͨ STB IP�˿ͻ���ES��Decoder������״̬��
	���������磬����ȣ��Ա�IP�ͻ�����ǰ��IPTV����������Э��������ǰ��IP���ݷ��͵����ʣ��ﵽ�����Ĳ���Ч����
*/
BOOL  CSESSetVideoDataInjectCallback(HCSHANDLE hHandle, CSESDataInjectCallback pfnCallback, PVOID pvUserData);

/**
@brief ע��VideoDecoder��Ҫ���ݵĻص�

@param[in] hHandle  �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������������ο��豸�����ӿ��ĵ����塣
@param[in] fnCallback �ص�
@param[in] pUserData �û��Զ������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ýӿ�ʵ������Ƶ����Trick Modeʱ���õĺ���������֧�ֻطź�ʱ�ơ�ʵʱ�����Ŵ˺�����Ч��
*/
void CSSYSVIDRegNeedDataCallback(HCSHANDLE hHandle, FOnVideoNeedData fnCallback, PVOID pUserData);

/**@brief ���Video Decode Buffer�е�����

���øýӿں���û���µ���Ƶͼ��������֮ǰ����ʹvideo��ʾ�Ǵ򿪵ģ�����ҲӦ���Ǻ���/��֡Ч��\n
��Ҫ���ڽ����Ƶ���жϺ�ָ�������£�δ����I֮֡ǰ���������ж�ǰ���һ��I֡���и������룬�������������������
���������һ֡���������������
- ���ܽ�Ŀ�ε����ܿ�
- �ε��ź��ߵ����video���澲ֹ���쳣�¼���
- ����ֹͣ��ǰ����Ƶ��Ŀ��
@param[in] hHandle[in]: �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������������ο��豸�����ӿ��ĵ�����
@param[in] hReserved[in]: �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�������ΪNULL
@return �����ɹ�����TRUE������ʧ�ܷ���FALSE
*/
BOOL CSSYSVIDClearDecodeBuffer(HCSHANDLE hHandle, HCSHANDLE hReserved);

/**@brief ������Ƶ��ͼ��������ǿ(IQI)������Ŀǰ��ST7111��ST7105ƽ̨֧�ָ�����
@param[in] hHandle[in]: �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������������ο��豸�����ӿ��ĵ�����
@param[in] enmIQIMode[in]:    CSVID_IQI_MODE_DEMO Ϊ�ͻ���ʾģʽ����ģʽ�򿪺�enmIQILevel �ֱ��Ӧ�������ߣ��ұߣ��м�Ĵ�IQI�������Ը�����Ŀ����
@param[in] enmIQILevel[in]: �ò�������IQI ��LEVEL,��4���ȼ���
@param[in] dwReserve[in]: �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�������Ϊ0
@return �����ɹ�����TRUE������ʧ�ܷ���FALSE
*/
BOOL CSSYSVIDSetIQIMode(HCSHANDLE hHandle, CSVIDIQIMode enmIQIMode,CSVIDIQILevel enmIQILevel, DWORD dwReserve);

/**@brief ��Ƶ��ͼ(����)�ӿڣ�����ǰ������ʾ����Ƶͼ��Ĳ���(��ȫ��)��ȡ�������Ŀ��ͼ���ָ������
@param[in] hHandle[in]: �ײ���Ƶ�豸�����������Ӧ��ͨ����CSRMConnect() �������������ο��豸�����ӿ��ĵ�����
@param[in] pstInputRect[in]:	���봰�ڴ�С����ʾҪ��ȡ��ǰ��Ƶͼ�����һ���֣�ΪNULL��ʾ����ͼ��ע�ⲻ�ܳ�����ǰ��Ƶͼ���С��
@param[in] pstOutputRect[in]:	������ڴ�С����ʾ�����pstDestImage����һ���֣�ΪNULL��ʾ����pstDestImage��ע�ⲻ�ܳ���pstDestImage��С��
@param[in] pstDestImage[in]: ��Ƶ��ͼ�������pstDestImage��ָ������Ŀǰֻ֧��RGB888 24λɫ��ʽ��CSGraphPixmap::imagebitsָ���buffer���û����䣬4�ֽڶ��룬����CSGraphPixmap�ṹ�嶨���Ҫ��һ��
@return �����ɹ�����TRUE������ʧ�ܷ���FALSE
*/
BOOL CSSYSVIDSnapshot(HCSHANDLE hHandle, const CSWINRECT* pstInputRect, const CSWINRECT* pstOutputRect, CSGraphPixmap * pstDestImage);

#ifdef  __cplusplus
}
#endif
/** @} */
#endif


