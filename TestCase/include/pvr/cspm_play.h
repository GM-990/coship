/******************************************************************************
FileName:  cspm_play.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

�ļ�����:
������PVR����(����PVRֱ��,TShift����,PVR�ط�)����ؽӿ�
ģ����PVRPLAY

�޸ļ�¼:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      �޽�           Create

*******************************************************************************/
#ifndef CSPM_PLAY_H
#define CSPM_PLAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "cspm_notifymgr.h"

/*!��Ч����λ��*/
#define  CSPVRPLAY_INVALID_TIME 0xffffffff
/*!��Ч�Ĳ����ٶ�*/
#define CSPVRPLAY_INVALID_SPEED 0xffffffff
/*!���������ٶ�*/
#define CSPVRPLAY_NORMAL_SPEED 1000

/*!��ĿPid����*/
typedef struct _CSPVRPLAYChnlParam_S
{
	/*!��Ƶpid*/
	WORD 	wVideoPid;
	/*!��Ƶ���ͣ�ʹ��porting��cs_sysvideo.h�е�CSVIDStreamType����*/
	BYTE   	ucVideoType;
	/*!��Ƶpid*/
	WORD	wAudioPid;
	/*!��Ƶ���ͣ�ʹ��porting��cs_sysaudio.h�е�CSAUDStreamType����*/
	BYTE   	ucAudioType;
	/*!pcr pid*/
	WORD	wPcrPid;	
	/*!teletext pid*/
	WORD	wTeletextPid;	
	/*!closed caption pid*/
	WORD	wClosedCaptionPid;
	/*!subtitile pid*/
	WORD 	wSubtitlePid;
}CSPVRPLAYChnlParam_S;

/*!��������*/
typedef enum _CSPVRPLAYType_E
{
	/*!ʵʱ��ֱ��*/
	EM_PVRPLAY_PLAYTYPE_LIVE=0,
	/*!PVR�ļ��ط�*/
	EM_PVRPLAY_PLAYTYPE_FILE,	
	/*!TShift����*/
	EM_PVRPLAY_PLAYTYPE_TSHIFT,
	/*!δ֪����*/
	EM_PVRPLAY_PLAYTYPE_UNKOWN
}CSPVRPLAYType_E;

/*!
	��ȡϵͳ֧�ֵĲ��ž������
\return ϵͳ֧�ֵĲ��ž������(pvrϵͳ��ʼ��ʱ�û����õĸ���)
*/
DWORD CSPVRPLAYGetSupportPlayCount(void);
/*!
	��ȡ���ž��
@param[in] nIdx ���ž��������
\return �ɹ���ȡʱ���ز��ž��,���򷵻�NULL
*/
HCSHANDLE CSPVRPLAYGetPlayHandle(int nIdx);

/*!
	��ʼ����Ŀpid����
@param[out] psParam �����ʼ����¼����������
\note ���ȵ��ñ�������ʼ��CSPVRPLAYChnlParam_S�ṹ��,
\note �ٶԸ���Ȥ������и�ֵ,�ٵ���CSPVRPLAYStartLive����ֱ��
*/
void CSPVRPLAYGetDefaultChnlParam(CSPVRPLAYChnlParam_S *psParam);

/*!
	����ʵʱ��ֱ��
@param[in]  hPVRPlayHandle Pvr���ž��
@param[in]  hPath ����ʹ���豸��·��
@param[in]  szDvbLocator ����dvb ��Ŀ��locator
\return �ɹ�����TRUE,���򷵻�FALSE
\note ���ϵͳ����TShift����ģʽ��EM_PVRPLAY_AWAYS, ������ֱ��ʱ,���Զ�����TShift¼��
\note �ú�������������ʱ�첽������
\note �ڲ��Ź�����,����ɾ��hPath
*/
BOOL CSPVRPLAYStartLive(HCSHANDLE hPVRPlayHandle, HCSHANDLE hPath, const char* szDvbLocator);

/*!
	����PVR�ļ��ط�
@param[in]  hPVRPlayHandle Pvr���ž��
@param[in]  hPath ����ʹ���豸��·��
@param[in]  hPvrFile ����CSPVRFOpen�õ���Pvr�ļ����
\return �ɹ�����TRUE,���򷵻�FALSE
\note �����������������첽��
\note �ڲ��Ź�����,����ɾ��hPath����hPvrStream
*/
BOOL CSPVRPLAYStartFile(HCSHANDLE hPVRPlayHandle,HCSHANDLE hPath,HCSHANDLE hPvrStream);

/*!
	��ȡ���ڲ��Ž�Ŀ��pid
@param[in]  hPVRPlayHandle Pvr���ž��
@param[out] psChnlPid  Ƶ��pid��Ϣ
\return �ɹ�����TRUE,���򷵻�FALSE
\note 
*/
BOOL CSPVRPLAYGetChnlPid(HCSHANDLE hPVRPlayHandle, CSPVRPLAYChnlParam_S *psChnlPid);

/*!
	�޸����ڲ��Ž�Ŀ��pid
@param[in]  hPVRPlayHandle Pvr���ž��
@param[out] psChnlPid  Ƶ����pid��Ϣ
\return �ɹ�����TRUE,���򷵻�FALSE
\note ͬ������,������Ч
*/
BOOL CSPVRPLAYModifyPid(HCSHANDLE hPVRPlayHandle,const CSPVRPLAYChnlParam_S *psChnlPid);

/*!
	��ͣ����(��TShift����,ֱ��,�ļ��طž�����)
@param[in]  hPVRPlayHandle Pvr���ž��
\return �ɹ�����TRUE,���򷵻�FALSE
\note ���ϵͳ����TShift����ģʽ��EM_PVRPLAY_START_WHEN_PAUSE����ֱ����ͣʱ���Զ�����TShift¼��
\note ���ϵͳ����TShift����ģʽ����EM_PVRPLAY_NEVER_START����ֱ������ͣʱ�����Զ��л�ΪTShift����
\note ͬ������,������Ч
*/
BOOL CSPVRPLAYPause(HCSHANDLE hPVRPlayHandle);

/*!
	�ָ�����(��TShift����,ֱ��,�ļ��ط�����)
@param[in]  hPVRPlayHandle Pvr���ž��
\return �ɹ�����TRUE,���򷵻�FALSE
\note ͬ������,������Ч
\note ��ǰ������ʲô�ٶȲ��ţ�Resume�ɹ������Ϊ���ٲ���
*/
BOOL CSPVRPLAYResume(HCSHANDLE hPVRPlayHandle);
/*!
	ֹͣ����(��TShift����,ֱ��,�ļ��طž�����)
@param[in]  hPVRPlayHandle Pvr���ž��
\return �ɹ�����TRUE,���򷵻�FALSE
\note ͬ������,������Ч
\note ֹͣ����ʱ,������ɾ��hPath(Pvrmgrģ���ڲ�����ɾ��hPath)
\note �����ֹͣ�ļ��ط�,������ɾ��hPvrStream(PvrMgrģ���ڲ�����ɾ��PvrStream)
*/
BOOL CSPVRPLAYStop(HCSHANDLE hPVRPlayHandle);

/*!
	���²���(��TShift����,ֱ��,�ļ��طž�����)
@param[in]  hPVRPlayHandle Pvr���ž��
\return �ɹ�����TRUE,���򷵻�FALSE
\note ͬ������,������Ч
\note ����TShift���ţ�Restart���Զ��л�Ϊֱ��									
*/
BOOL CSPVRPLAYRestart(HCSHANDLE hPVRPlayHandle);
/*!
	���ò���λ��(��TShift����,�ļ��ط�����)
@param[in]  hPVRPlayHandle Pvr���ž��
@param[in] dwTimeSec �����õĲ���λ��(��׼λ��Ϊ��ʼλ��,��λΪ��)
\return �ɹ�����TRUE,���򷵻�FALSE
\note ֱ��ʱ�޷����ò���λ��
\note ͬ������,������Ч
\note ���ò���λ�óɹ��󣬲����ٶȱ���Ϊ����
*/
BOOL CSPVRPLAYSeek(HCSHANDLE hPVRPlayHandle,DWORD dwTimeSec);

/*!
	��ȡ��ǰ����λ��(��TShift����,�ļ��ط�����)
@param[in]  hPVRPlayHandle Pvr���ž��
\return  ��ȡ�ɹ����ص�ǰ����λ��(��λΪ��), ʧ��ʱ����CSPVRPLAY_INVALID_TIME
*/
DWORD CSPVRPLAYGetCurTimeSec(HCSHANDLE hPVRPlayHandle);

/*!
	���ò����ٶ�(��TShift����,ֱ��,�ļ��طž�����)
@param[in]  hPVRPlayHandle Pvr���ž��
@param[in]  nSpeed �����õĲ����ٶ�
return �Ƿ�ɹ����ò����ٶ�
\note CSPVRPLAY_NORMAL_SPEEDΪ���ٲ���,
\note ֧�ֵĿ��˲����ٶ���: (-16, -8,-6,-4,-2,-1)*CSPVRPLAY_NORMAL_SPEED
\note ֧�ֵ����˲����ٶ���: (-1/8, -1/4,-1/2)*CSPVRPLAY_NORMAL_SPEED
\note ֧�ֵ����������ٶ���: (1/8,  1/4, 1/2)*CSPVRPLAY_NORMAL_SPEED
\note ֧�ֵĿ�������ٶ���: (2,4,6,8,16,32)*CSPVRPLAY_NORMAL_SPEED
\note ֱ��ʱ,���ϵͳ����TShift����ģʽ��EM_PVRPLAY_AWAYS,������С��CSPVRPLAY_NORMAL_SPEED���ٶ�.
\npte ֱ��ʱ�����ٶȳɹ���,���Զ��л�ΪTShift����
*/
BOOL CSPVRPLAYSetSpeed(HCSHANDLE hPVRPlayHandle, int nSpeed);

/*!
	��ȡ��ǰ�����ٶ�
@param[in]  hPVRPlayHandle Pvr���ž��
\return ��ȡ�ɹ����ص�ǰ�����ٶȣ�ʧ��ʱ����CSPVRPLAY_INVALID_SPEED
\note ���ص��ٶ�����CSPVRPLAY_NORMAL_SPEEDΪ��׼
\note ���ظ�����ʾ���ڻ���
\note ��TShift����,ֱ��,�ļ��طž�����
*/
int CSPVRPLAYGetSpeed(HCSHANDLE hPVRPlayHandle);

/*!
	��ȡ��ǰ��������
@param[in]  hPVRPlayHandle Pvr���ž��
\return ��ǰ��������
\note ��TShift����,ֱ��,�ļ��طž�����
*/
CSPVRPLAYType_E CSPVRPLAYGetPlayType(HCSHANDLE hPVRPlayHandle);

/*!
	��ȡ���ڻطŵ�PVR�ļ���
@param[in]  hPVRPlayHandle Pvr���ž��
@param[out]  phOutPvrStream ���ڻطŵ�PVR�ļ������
\return �ɹ�����TRUE
\note ����hPVRPlayHandle���ڲ�������ΪPVR�ļ��ط�ʱ�����ӿ����á�
*/
BOOL  CSPVRPLAYGetPlayingStream(HCSHANDLE hPVRPlayHandle,HCSHANDLE* phOutPvrStream);

/*!
	��ȡ��ǰ�ɲ��ų���
@param[in]  hPVRPlayHandle Pvr���ž��
\return �ɲ��ų���,��λΪ��
\note �����ǰ������ֱ������δ�������ţ��򷵻�CSPVRPLAY_INVALID_TIME
\note ��TShift����,�ļ��ط�����
*/
DWORD CSPVRPLAYGetDurationSec(HCSHANDLE hPVRPlayHandle);

/*! ���ŵ�֪ͨ���Ͷ���*/
typedef enum _CSPVRPLAYNotifyType_E
{
	/*!
		ֱ����TShift�����л�֪ͨ
		\note	dwNotifyType:	EM_PVRPLAY_TSHIFT_SWITCH 
		\note	pNotifyData:	�л���Ĳ������ͣ���CSPVRPLAYType_E�Ķ���
		\note	hSender: 		hPvrPlayHandle���ž��
	*/
	EM_PVRPLAY_TSHIFT_SWITCH = 0,
	/*!
		�����ٶȱ仯֪ͨ
		\note	dwNotifyType:	EM_PVRPLAY_SPEED_CHANGE 
		\note	pNotifyData:	�仯��Ĳ����ٶ�
		\note	hSender: 		hPvrPlayHandle���ž��
	*/
	EM_PVRPLAY_SPEED_CHANGE,
	/*!
		���ŵ��ļ�ĩβ֪ͨ
		\note	dwNotifyType:	EM_PVRPLAY_END_OF_FILE 
		\note	pNotifyData:	δ����
		\note	hSender: 		hPvrPlayHandle���ž��
	*/
	EM_PVRPLAY_END_OF_FILE,	
	/*!
		���ŵ��ļ���ͷ֪ͨ
		\note	dwNotifyType:	EM_PVRPLAY_BEGIN_OF_FILE 
		\note	pNotifyData:	δ����
		\note	hSender: 		hPvrPlayHandle���ž��
	*/
	EM_PVRPLAY_BEGIN_OF_FILE,	
	/*!
		����ģ��֪ͨ����ĸ���
	*/
	EM_PVRPLAY_NOTIFY_TYPE_COUNT
}CSPVRPLAYNotifyType_E;

/*!
	��ȡ��Ϣ֪ͨ�������ľ��
\return ��Ϣ֪ͨ�������ľ��
*/
HCSHANDLE CSPVRPLAYGetNotifyMgr(void);

/*!
	����PVR PLAY�Ƿ�֧��Teletext 
@param[in]  bSupport: PVR PLAY�Ƿ�֧��Teletext
\note ��֧�֣�����ʱ��Ҫ��Teletext��PID ͨ��CSUDIPLAYERSetStream���ø�����
\note VBI Teletext ��Ҫͨ��CSUDIPLAYERSetStream��PID���ø�������OSD Teletext����Ҫ
*/
void CSPVRPLAYSetSupportTtxCc(BOOL bSupport);

/*!
	��ȡPVR PLAY�Ƿ�֧��Teletext 
\return ֧�ַ���TRUE, ���򷵻�FALSE
*/
BOOL CSPVRPLAYGetSupportTtxCc();

/*!
	����PVR PLAY�Ƿ�֧��Subtitle
@param[in]  bSupport: PVR PLAY�Ƿ�֧��Subtitle
\note ��֧�֣�����ʱ��Ҫ��Subtitle��PID ͨ��CSUDIPLAYERSetStream���ø�����
*/
void CSPVRPLAYSetSupportSubtitle(BOOL bSupport);

/*!
	����PVR PLAY�Ƿ�֧��Subtitle 
\return ֧�ַ���TRUE, ���򷵻�FALSE
*/
BOOL CSPVRPLAYGetSupportSubtitle();



#ifdef MMCP_UDI2
/*!
	��MPͨ��CSUDIPLAYEROpen�Ĳ���handle���ݸ�pvr��udi2toudi1��adapter�㣬�����������ֲ���
@param[in]  hPVRPlayHandle Pvr���ž��
@param[out] hPlayer  MP��Open�Ĳ���handle
\return 
\note ͬ������,������Ч
*/
void CSPVRPLAYSetPlayerHandle(int hPVRPlayHandle, CSUDI_HANDLE hPlayer);

/*!
	��Pvr���ž����Ӧ��ʱ�Ʋ��ž�����ظ�MP
@param[in]  hPVRPlayHandle Pvr���ž��
@param[out] MP�Ĳ���handle
\return �ɹ����غϷ���ʱ�Ʋ��ž����ʧ�ܷ���CSUDI_NULL
\note ͬ������,������Ч
*/
CSUDI_HANDLE CSPVRPLAYGetTShiftPlayerHandle(int hPVRPlayHandle);
#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

