/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_video.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_video.h"
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

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOAddCallback(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent,void * pvUserData )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

 /**
@brief ɾ��ָ���¼����͵Ļص�����

@param[in] nVideoIndex ��Ƶ������������
@param[in] fnVideoCallback �ص�����ָ�룬���CSUDIVIDEOCallback_F����
@param[in] eEvent �ص��������ͣ����CSUDIVIDEOEventType_E
@param[in] pvUserData ͬCSUDIVIDEOAddCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ������CSUDIVIDEO_ERROR_CALLBACK_NOT_EXIST
@note �����ڻص�������ɾ���ص�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEODelCallback(int nVideoIndex, CSUDIVIDEOCallback_F fnVideoCallback,CSUDIVIDEOEventType_E eEvent, void * pvUserData )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	 
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	 
	return Retcode;
}

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

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetVBIEnable(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL bEnable)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡVBI��������teletext\close caption\Macrovision ����״̬

@param[in] nVideoIndex ��Ƶ������������
@param[in] eVideoVBIType VBI�������ͣ�ÿ��ֻ�ܲ�ѯһ������
@param[out] pbEnable ,eVideoVBIType��Ӧ�Ĺ��ܿ���״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetVBIEnable(int nVideoIndex, CSUDIVIDEOVBIType_E eVideoVBIType, CSUDI_BOOL* pbEnable)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡVideo Decoder�豸������

@param[out] pnVideoDecCount ϵͳ�� ����Video Decoder�豸�ĸ���
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ�����������pnVideoDecCount�����ݲ�ȷ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetCount(int * pnVideoDecCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief ��ȡָ��Video Decoder�豸��������Ϣ

���ۿ��л��߱�ռ��
@param[in] nVideoIndex Video Decoder�豸������
@param[out] psCapabilityInfo Video Decoder�豸������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ�����������psCapabilityInfo�ṹ�����ݲ�ȷ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetCapability(int nVideoIndex, CSUDIVIDEOCapability_S * psCapabilityInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**@brief ��Ƶ��ͼ(����)�ӿڣ�����ǰ������ʾ����Ƶͼ��Ĳ���(��ȫ��)��ȡ�������Ŀ��ͼ���ָ������

@param[in] nVideoIndex  Video Decoder�豸������
@param[in] psSrcRect ���봰�ڴ�С����ʾҪ��ȡ��ǰ��Ƶͼ�����һ���֣�ΪCSUDI_NULL��ʾ����ͼ��ע�ⲻ�ܳ�����ǰ��Ƶͼ���С��
@param[in] psDstRect ������ڴ�С����ʾ�����hSurface����һ���֣�ΪCSUDI_NULL��ʾ����hSurface��ע�ⲻ�ܳ���hSurface��С��
@param[out] phSurface ��Ƶ��ͼ�������hSurface��ָ�����򣬸�Surface�ǵ���OSG�ӿڴ����Ļ������ֱ�ӻ�ȡ�Դ棻
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����hSurface�ṹ������δ���塣
@note �ڽ�ȡI֡ͼƬʱ��ͨ������CSUDIVIDEOGetStreamStatus��ȡ��ǰI֡��Ϣ������ͼ������Ϣ��
@note �����Ʒ����Ҫ��Ƶ���չ��ܻ���Ӳ����֧�֣��ýӿڿ��Բ�ʵ�֡���ʱ�践��CSUDIVIDEO_ERROR_FEATURE_NOT_SUPPORTED��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSnapshot(int nVideoIndex, const CSUDIWinRect_S * psSrcRect, const CSUDIWinRect_S * psDstRect, CSUDI_HANDLE* phSurface)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ����������״̬��Ϣ

�統ǰ���ŵ���Ƶ��С��
@param[in]  nVideoIndex Video Decoder�豸��Դ������
@param[out] psStreamStatus ����IFRAME��Ϣ ��CSUDIVIDStreamStatus_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psStreamStatus�ṹ������δ����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetStreamStatus(int nVideoIndex,CSUDIVIDEOStreamStatus_S * psStreamStatus)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ʾ/������Ƶ�����������

@param[in] nVideoIndex  ��Ƶ������������
@param[in] bIsShow ΪCSUDI_TRUE���ʾ��ʾ�ô���;ΪCSUDI_FALSE���ʾ���ظô���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ 
@note ϵͳ��ʼ��ʱӦΪ��ʾ״̬

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOShow(int nVideoIndex, CSUDI_BOOL bIsShow)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

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

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetAspectRatio(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E eAspectRatio)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief��ȡָ����Ƶ�������Ļ��߱�

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[out] peAspectRatio ��Ƶ����ӿڵĿ�߱ȣ���4:3 �� 16:9
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���ӿ����ڻ�ȡ��ǰ���õ���Ļ��߱�״̬��������õ���AUTO���򷵻�AUTO��
���������CSUDIVIDEOGetStreamStatus�ӿڻ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetAspectRatio(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOAspectRatioType_E *peAspectRatio)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

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

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetMatchMethod(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E eMatchMethod)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief��ȡָ����Ƶ����ӿڵ���Ӧģʽ

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice ָ����Ƶ�����Ļ
@param[out] peMatchMethod ָ����Ƶ����˿ڵ���Ӧģʽ����PAN_SCAN,LETTER_BOX,��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetMatchMethod(int nVideoIndex,CSUDISCREENType_E eScreenDevice, CSUDIVIDEOMatchMethod_E *peMatchMethod)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ����ֹͣ��Ƶ��ģʽ

����Ŀ�л�ʱ����Ƶ�л�Ч�����羲֡������
@param[in] nVideoIndex  Video Decoder�豸��Դ������
@param[in] eStopMode ֹͣģʽ��������ο�CSUDIVIDEOStopMode_E��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E eStopMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief��ȡֹͣ��Ƶ��ģʽ

����Ŀ�л�ʱ����Ƶ�л�Ч�����羲֡������
@param[in] nVideoIndex  Video Decoder�豸��Դ������
@param[out] peStopMode ֹͣģʽ��������ο�CSUDIVIDEOStopMode_E��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetStopMode(int nVideoIndex, CSUDIVIDEOStopMode_E *peStopMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ����ָ����Ƶ��������Ĵ��ڴ�С��ʵ�����Ź���

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ���ڴ�С
@param[in] pstRect ���ڵĴ�С�Լ�����λ�ã��ο�CSUDIWinRect_S�ṹ�����pstRectΪNULL������ʾΪȫ����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note pstRectΪ����뵱ǰ��Ƶ�ֱ��ʵľ���,����Ϣ���Դ�CSUDISCREENGetResolution�ӿڻ�ȡ
@note �����ǲü�����Ϊ���������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * pstRect )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡָ����Ƶ��������Ĵ��ڴ�С

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ���ڴ�С
@param[out] pstRect ���ڵĴ�С�Լ�����λ�ã��ο�CSUDIWinRect_S�ṹ��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetWindowSize(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * pstRect )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

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

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOSetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, const CSUDIWinRect_S * psRect)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief ��ȡ���ڼ���λ�úͳߴ�

@param[in] nVideoIndex ��Ƶ������������
@param[in] eScreenDevice �������ĸ���Ļ�ϵ���Ƶ��������
@param[out] psRect ���е��������CSUDIWinRect_S���塣
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIVIDEOGetWinClip(int nVideoIndex, CSUDISCREENType_E eScreenDevice, CSUDIWinRect_S * psRect)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

