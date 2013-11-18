/**@defgroup CSSYSAV [CSSYSAV]ͬ����Ŀר��AV�����ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨��AV�����ӿ�
@version 0.1 2008/10/23 ��ʾ�汾
@{
*/
#ifndef _CS_SYSAV_H_
#define _CS_SYSAV_H_

#include "cs_typedef.h"
#include "cs_syspublic.h"
#include "cs_sysvideo.h"
#include "cs_sysaudio.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define SMS_AUIDO_TYPE_UNKNOWN									(0) 	/**< ��Ƶ���ͣ�δ֪����*/
#define SMS_AUIDO_TYPE_NORMAL										(1)	  /**< ��Ƶ���ͣ���ͨ,0x03/0x04*/
#define SMS_AUIDO_TYPE_AC3											(2)	  /**< ��Ƶ���ͣ�AC3*/

#define SMS_AUIDO_TYPE_DTS        							(3) 	/**< ��Ƶ���ͣ�DTS */
#define SMS_AUIDO_TYPE_MPEG1     								(4) 	/**< ��Ƶ���ͣ�MPEG1*/
#define SMS_AUIDO_TYPE_MPEG2      							(5)		/**< ��Ƶ���ͣ�MPEG2*/
#define SMS_AUIDO_TYPE_CDDA       							(6) 	/**< ��Ƶ���ͣ�CDDA*/
#define SMS_AUIDO_TYPE_PCM       	 							(7) 	/**< ��Ƶ���ͣ�PCM*/
#define SMS_AUIDO_TYPE_LPCM       							(8) 	/**< ��Ƶ���ͣ�LPCM*/
#define SMS_AUIDO_TYPE_PINK_NOISE 							(9) 	/**< ��Ƶ���ͣ�PINK_NOISE*/
#define SMS_AUIDO_TYPE_MP3        							(10) 	/**< ��Ƶ���ͣ�MP3 */
#define SMS_AUIDO_TYPE_MLP        							(11) 	/**< ��Ƶ���ͣ�MLP */
#define SMS_AUIDO_TYPE_BEEP_TONE  							(12) 	/**< ��Ƶ���ͣ�EEP_TONE*/
#define SMS_AUIDO_TYPE_MPEG_AAC   							(13)	/**< ��Ƶ���ͣ�Advanced audio coding. Part of MPEG-4 */
#define SMS_AUIDO_TYPE_NULL       							(14)
#define SMS_AUIDO_TYPE_MPEG_AAC_PLUS  					(15) 	/**< ��Ƶ���ͣ�AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE) */
#define SMS_AUIDO_TYPE_MPEG_AAC_PLUS_ADTS			 	(16) 	/**< ��Ƶ���ͣ�AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format) */
#define SMS_AUIDO_TYPE_MPEG_AAC_PLUS_LOAS 			(17) 	/**< ��Ƶ���ͣ�AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream) */
#define SMS_AUIDO_TYPE_AC3_PLUS   							(18) 	/**< ��Ƶ���ͣ�Dolby Digital Plus (AC3+ or DDP) audio */
#define SMS_AUIDO_TYPE_HE_AAC   								(19) 	/**< ��Ҫ���˵��*/
#define SMS_AUIDO_TYPE_RAW_AAC   								(20) 	/**< ��Ҫ���˵��*/
#define SMS_AUIDO_TYPE_DRA        							(21) 	/**< ��Ƶ���ͣ�DRA */

#define SMS_VIDEO_TYPE_UNKNOWN									(0)		/**< ��Ƶ���ͣ�δ֪����*/

#define SMS_VIDEO_TYPE_MPEG2  									(0) 	/**< ��Ƶ���ͣ�MPEG2*/
#define SMS_VIDEO_TYPE_H264    									(1) 	/**< ��Ƶ���ͣ�H264*/
#define SMS_VIDEO_TYPE_MPEG4 										(2) 	/**< ��Ƶ���ͣ�MPEG4*/
#define SMS_VIDEO_TYPE_MPEG1          					(3) 	/**< ��Ƶ���ͣ�MPEG1*/
#define SMS_VIDEO_TYPE_H263            					(4) 	/**< ��Ƶ���ͣ�H263*/
#define SMS_VIDEO_TYPE_NULL       							(500)	/**< ��Ƶ���ͣ�δ֪����*/

#define SMS_AUIDO_OUTPUTMODE_UNKNOWN						(0)		/**< ��Ƶ������ͣ�δ֪*/
#define SMS_AUIDO_OUTPUTMODE_PCM								(1)		/**< ��Ƶ������ͣ�PCM*/
#define SMS_AUIDO_OUTPUTMODE_DIGITAL						(2)		/**< ��Ƶ������ͣ�Digital*/

#define SMS_VIDEO_OUTPUTMODE_UNKNOWN						(0)		/**< ��Ƶ������ͣ�δ֪*/

#define SMS_SER_SWITCH_TYPE_UNKNOWN							(0)		/**< ��̨ʱ����Ƶ�Ĵ������ͣ�δ����*/
#define SMS_SER_SWITCH_TYPE_SCREEN_BLACK				(1)		/**< ��̨ʱ����Ƶ�Ĵ������ͣ�����*/
#define SMS_SER_SWITCH_TYPE_SCREEN_IFRAME				(2)		/**< ��̨ʱ����Ƶ�Ĵ������ͣ�����*/
#define SMS_SER_SWITCH_TYPE_SCREEN_FADE_IN_OUT	(3)		/**< ��̨ʱ����Ƶ�Ĵ������ͣ����뵭��*/

typedef struct _SMSAVStreamType
{
	DWORD			m_dwAudioType:8;					/**< SMS_AUIDO_TYPE_UNKNOWN SMS_AUIDO_TYPE_NORMAL SMS_AUIDO_TYPE_AC3*/
	DWORD			m_dwVideoType:8;					/**< SMS_VIDEO_TYPE_UNKNOWN*/
	DWORD			m_dwAudioOutMode:8;				/**< SMS_AUIDO_OUTPUTMODE_UNKNOWN SMS_AUIDO_OUTPUTMODE_PCM SMS_AUIDO_TYPE_AC3*/
	DWORD			m_dwVideoOutMode:8;				/**< SMS_VIDEO_OUTPUTMODE_UNKNOWN*/

	DWORD			m_dwCode:8;								/**< ���Ϊ1����˵�����ڽ��������ΪSMSAVStreamType������δ����*/
	DWORD			m_dwServiceSwitchType:8;	/**< ��̨ʱ����Ƶ�Ĵ�������:���� ���� ���뵭��*/
	DWORD					:16;									/**< ����*/
}SMSAVStreamType;
/**@brief ���϶���������������Ƶ����ʱ���������ͣ���Ҫ���ں��� CSUSPSMSPlayStream*/
typedef enum _CSSYSAVSync_E
{
	EM_AV_SYNC_NONE = 0, 					/**< û������ͬ��*/
	EM_AV_SYNC_PCR,		 						/**< ����Ƶ��PCRͬ��*/
	EM_AV_SYNC_VID,  							/**< ����Ƶ����Ƶͬ�� */
	EM_AV_SYNC_AUD 								/**< ����Ƶ����Ƶͬ��,�ݲ�֧��*/
}CSSYSAVSync_E;

/**
@brief �û�����Audio/video/pcr��Ŀ��Ϣ

@param[in] hHandle �豸ͨ�������ͨ��CSRMConnect()����õ����豸�����
@param[in] pstAUDInfo �����Ŀ��Audio��Ϣ������ṹ�ο�CSSYSAUDInputInfo���ݽṹ��
@param[in] pstVIDInfo �����Ŀ��Video��Ϣ������ṹ�ο�CSSYSVIDInputInfo���ݽṹ��
@param[in] pstPCRInfo pstPCRInfo�����Ŀ��PCR��Ϣ���ο�3.2.1 PCR Input Info���ݽṹ
@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�Ĭ��ֵ��ΪNULL��
@return  ��������ɹ��򷵻�TRUE���������ʧ���򷵻�FALSE
@note���û�����Audio/video/pcr��Ŀ��Ϣ�������������ͣ���ʽ��pid�ȣ����������������Ƶ���ͣ���Ƶ���ͣ�����Ĭ�����Ͳ�����
*/
BOOL CSSYSSMSetInputStream(HCSHANDLE hHandle, CSSYSAUDInputInfo *pstAUDInfo, 
			   CSSYSVIDInputInfo *pstVIDInfo,CSSYSPCRInputInfo* pstPCRInfo,HCSHANDLE hUserData );


/**
@brief ��������Ƶ�Ĳ��� 

@param[in] hHandle ͨ������CSSYSChipInit�������صõ�
@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�Ĭ��ֵ��ΪNULL
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE
@note ����CSSYSSMSetInputStream���������ú�����������Ƶpid���Լ�����Ƶ��������Բ�����������Ƶ�Ĳ��ţ����û��������Ƶpid����ֻ�������������ֻ֮����Ƶ���
*/
BOOL CSSYSSMPlay(HCSHANDLE hHandle, HCSHANDLE hUserData);


/**
@brief ֹͣ����Ƶ�Ĳ���

@param[in] hHandle ͨ������CSSYSChipInit�������صõ���
@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�Ĭ��ֵ��ΪNULL��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@note ֹͣ����Ƶ�Ĳ��ţ���CSSYSSMPlay�ɶ�ʹ�á�
*/
BOOL CSSYSSMStop(HCSHANDLE hHandle, HCSHANDLE hUserData);


/**
@brief ��ͣ��hHandle��ص�����Ƶ�Ĳ���

@param[in] hHandle ͨ������CSSYSChipInit�������صõ�
@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�Ĭ��ֵ��ΪNULL��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@note ��ͣ��hHandle��ص�����Ƶ�Ĳ��ţ�ֻ�����ڲ���ĳһ��Ŀ��������ֻ����Ƶ������Ƶ��ʱ���ò���Ч��
*/
BOOL CSSYSSMPause(HCSHANDLE hHandle, HCSHANDLE hUserData);



/**
@brief �ָ���ͣģʽ 

@param[in] hHandle ͨ������CSSYSChipInit�������صõ�
@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�Ĭ��ֵ��ΪNULL��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@note �ָ���ͣģʽ;��CSSYSSMPause�ɶ�ʹ�á�
*/
BOOL CSSYSSMResume(HCSHANDLE hHandle, HCSHANDLE hUserData);

/**
@brief ������,����,��λģʽ

@param[in] hHandle ͨ������CSSYSChipInit�������صõ�
@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�Ĭ��ֵ��ΪNULL��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE  
@note ������,����,��λģʽ��
*/
BOOL CSSYSSMTrickModeStart(HCSHANDLE hHandle,HCSHANDLE hUserData);


/**
@brief �˳����,����,��λģʽ

@param[in] hHandle ͨ������CSSYSChipInit�������صõ�
@param[in] hUserData �ò����������Ա��պ�Ĺ�����չ��Ŀǰ��ʹ�ã�Ĭ��ֵ��ΪNULL��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@note �˳����,����,��λģʽ��
*/
BOOL CSSYSSMTrickModeStop(HCSHANDLE  hHandle, HCSHANDLE  hUserData);


/**
@brief ES��ע���ʼ��

@param[in] hHandle ͨ������CSSYSChipInit�������صõ� hHandleͨ������CSSYSChipInit�������صõ�
@param[in] pstESBufferInfo ע���Buffer����ص���Ϣ����������ƵBuffer�ĵ�ַ�ʹ�С�ȡ��ο�ES  Buffer Info ���ݽṹ�� 
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@note ��ֹES��ע�룬������buffer��
*/
BOOL CSESInit(HCSHANDLE  hHandle, CSESBufferInfo* pstESBufferInfo);


/**
@brief ��ֹES��ע�룬������buffer

@param[in] hHandle ͨ������CSSYSChipInit�������صõ�
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@note ��ֹES��ע�룬������buffer��
*/
BOOL CSESTerm(HCSHANDLE hHandle);


/**
@brief ���ע���AV ES��PTS��Ϣ 

@param[in] hHandle �豸ͨ�������ͨ��CSRMConnect()����õ����豸���
@param[out] syncInfo ���صĵİ��� ES����PTS���ݡ�
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
*/
BOOL CSESGetCurrentPTSEx(HCSHANDLE hHandle, CSESSyncInfoEx* syncInfo);
/**
@brief  ����AVͬ��

@param[in] hPcrDmx - ����PCR���õ�Demux�豸Handle�� EM_AV_SYNC_PCRģʽ��Ч
@param[in] wPcrPid - PCR PID, EM_AV_SYNC_PCRģʽ��Ч
@param[in] hVid  - Video�豸���
@param[in] hAud  - Audio�豸���
@param[in] eSync  - ͬ����ʽ
@return  ����ɹ�����ͬ����������ʧ�ܷ���NULL��
*/
HCSHANDLE  CSSYSAVSyncStart (HCSHANDLE hPcrDmx,WORD wPcrPid, HCSHANDLE hVid, HCSHANDLE hAud,CSSYSAVSync_E eSync);

/**
@brief ֹͣͬ����ֹͣ��hAvSync��Ч��ͬһ��hAvSyncֻ��Stopһ��

@param[in]  hAvSync  - CSAVSyncStart���ص�ͬ�����
@return  ����ɹ�����ͬ����������ʧ�ܷ���NULL��
*/
BOOL  CSSYSAVSyncStop (HCSHANDLE hAvSync);

/**
@brief ������Ƶͬ��ģʽ�����´β���ʱ��Ч

@param[in] hHandle  -ͨ������CSSYSChipInit�������صõ���
@param[in] eMode  -ͬ��ģʽ
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@note ���´β���ʱ��Ч
*/
BOOL CSSYSAVSetSyncMode(HCSHANDLE hHandle, CSSYSSync_E eMode);


/**
@brief ��ȡ����Ƶͬ��ģʽ 

@param[in] hHandle -ͨ������CSSYSChipInit�������صõ���
@param[out] peMode -ͬ��ģʽ
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE  
*/

BOOL CSSYSAVGetSyncMode(HCSHANDLE hHandle, CSSYSSync_E* peMode);
/**
@brief ��������Ƶ������,����SM 01.05.15 �����Ժ�汾��hUserData�����ͳ� &SMSAVStreamType

@param[in] hDemux �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����Demux�豸�����š�
						���ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] wVideoPID  ������������ƵPID����Χ0-0x1ffe
@param[in] wAudioPID  ������������ƵPID����Χ0-0x1ffe
@param[in] wPCRPID  ����������PCR PID����Χ0-0x1ffe
@param[in] hUserData  �û��Զ������ݣ� ĿǰUSP�Ըò�����ʹ��Ϊָ��SMSAVStreamType��ָ�롣
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺���ֻ������������������Ƶ�Ĵ򿪹ر�Ӧ����˺����޹أ�Ҫ��˺�������ǰ�󲻸ı�����Ƶ���״̬��
 ����ʹwVideoPID wAudioPID wPCRPID��Ϊ0��Ȼ����TRUE
*/
BOOL CSUSPSMSPlayStream( HCSHANDLE hDemux, WORD wVideoPID, WORD wAudioPID, WORD wPCRPID, HCSHANDLE hUserData );

/**
@brief ֹͣ����Ƶ������

@param[in] hDemux �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] hUserData  �û��Զ������ݣ�ĿǰUSP�Ըò�����ʹ��Ϊָ��SMSAVStreamType��ָ��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺�������ֹͣ����Ƶ������������Ƶ�Ĵ򿪹ر�Ӧ����˺����޹أ�Ҫ��˺�������ǰ�󲻸ı�����Ƶ���״̬��
*/
BOOL CSUSPSMSStopPlay(HCSHANDLE hDemux, HCSHANDLE hUserData);

/**
@brief ����Ƶ���

@param[in] hDemux   �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] hUserData  �û��Զ������ݣ�����
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺���ֻ������Ƶ����Ĵ򿪣���������������ر��޹ء�
*/
BOOL CSUSPSMSEnableAudio( HCSHANDLE hDemux, HCSHANDLE hUserData );
/**
@brief �ر���Ƶ���

@param[in] hDemux  �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] hUserData  �û��Զ������ݣ�����
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺���ֻ������Ƶ����Ĺرգ���������������ر��޹�.
*/
BOOL CSUSPSMSDisableAudio( HCSHANDLE hDemux, HCSHANDLE hUserData );

/**
@brief ����Ƶ���

@param[in] hDemux   �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] hUserData  �û��Զ������ݣ�����
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺���ֻ������Ƶ����Ĵ򿪣���������������ر��޹ء�
*/
BOOL CSUSPSMSEnableVideo( HCSHANDLE hDemux, HCSHANDLE hUserData );

/**
@brief �ر���Ƶ���

@�ر���Ƶ������ýӿ�ֻ�ṩ��UPS ��SMSģ��ʹ�á�
 �����ǰϵͳģʽΪPVRģʽ����ֱ�ӷ���TRUE����ǰϵͳģʽΪPVRģʽͨ��ƽ̨CSSYSAdvancedParamEx�ӿڽ��г�ʼ�����ã�
 Ҳ����ͨ��CSSYSSetPvrSupport�ӿ��޸�
@param[in] hDemux  �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] hUserData  �û��Զ������ݣ�����
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺�����������Ƶ����Ĺرգ���������������ر��޹�
*/
BOOL CSUSPSMSDisableVideo( HCSHANDLE hDemux, HCSHANDLE hUserData );

/**
@brief ����5�����������趨����Ƶ������Audio��Video PID,�����漰����Ƶ����Ĵ򿪺͹ر�
�趨��������Audio PID
@�ýӿ�ֻ�ṩ��UPS ��SMSģ��ʹ�á������ǰϵͳģʽΪPVRģʽ����ֱ�ӷ���TRUE��
 ��ǰϵͳģʽΪPVRģʽͨ��ƽ̨CSSYSAdvancedParamEx�ӿڽ��г�ʼ�����ã�Ҳ����ͨ��CSSYSSetPvrSupport�ӿ��޸ġ�
@param[in] hDemux  �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] dwAudioPID   ��ƵPID
@param[in] hUserData  �û��Զ������ݣ�����
@param[in] nType   Audio ����
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺������������趨��������Audio PID�������漰����Ƶ����Ĵ򿪺͹رա�
 �˺����ĵ��÷����ڽ���������֮�󣬷�����Ҫ�ı���ƵPID�ĵط�������PMT�汾�ű仯����
*/
BOOL CSUSPSMSSetAudioPID( HCSHANDLE hDemux, DWORD dwAudioPID, int nType, HCSHANDLE hUserData );
/**
@brief �趨��������Audio PID

@�ýӿ�ֻ�ṩ��USP ��SMSģ��ʹ�ã������ǰϵͳģʽΪPVRģʽ����ֱ�ӷ���TRUE��
 ��ǰϵͳģʽΪPVRģʽͨ��ƽ̨CSSYSAdvancedParamEx�ӿڽ��г�ʼ�����ã�Ҳ����ͨ��CSSYSSetPvrSupport�ӿ��޸ġ�
@param[in] hDemux  �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] wAudioPID   ��ƵPID
@param[in] hUserData   �û��Զ������ݣ�����
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺������������趨��������Audio PID�������漰����Ƶ����Ĵ򿪺͹رա�
 �˺����ĵ��÷����ڽ���������֮�󣬷�����Ҫ����AudioPID�ĵط�������PMT�汾�ű仯����
*/
BOOL CSUSPSMSResetAudioPID( HCSHANDLE hDemux, WORD wAudioPID, HCSHANDLE hUserData );

/**
@brief �趨��������Video PID

@�ýӿ�ֻ�ṩ��UPS ��SMSģ��ʹ�ã������ǰϵͳģʽΪPVRģʽ����ֱ�ӷ���TRUE��
 ��ǰϵͳģʽΪPVRģʽͨ��ƽ̨CSSYSAdvancedParamEx�ӿڽ��г�ʼ�����ã�Ҳ����ͨ��CSSYSSetPvrSupport�ӿ��޸ġ�
@param[in] hDemux  �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] wVideoPID   ��ƵPID
@param[in] hUserData   �û��Զ������ݣ�����
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺������������趨��������Video PID�������漰����Ƶ����Ĵ򿪺͹رա�
 �˺����ĵ��÷����ڽ���������֮�󣬷�����Ҫ�ı���ƵPID�ĵط�������PMT�汾�ű仯����
*/
BOOL CSUSPSMSResetVideoPID( HCSHANDLE hDemux, WORD wVideoPID, HCSHANDLE hUserData );

/**
@brief �趨��������PCR PID

@�ýӿ�ֻ�ṩ��UPS ��SMSģ��ʹ�á������ǰϵͳģʽΪPVRģʽ����ֱ�ӷ���TRUE��
 ��ǰϵͳģʽΪPVRģʽͨ��ƽ̨CSSYSAdvancedParamEx�ӿڽ��г�ʼ�����ã�Ҳ����ͨ��CSSYSSetPvrSupport�ӿ��޸ġ�
@param[in] hDemux   �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] wPCRPID   PCR PID
@param[in] hUserData   �û��Զ������ݣ�����
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺������������趨��������PCR PID�������漰����Ƶ����Ĵ򿪺͹رա�
 �˺����ĵ��÷����ڽ���������֮�󣬷�����Ҫ�ı�PCR PID�ĵط�������PMT�汾�ŷ����仯����
*/
BOOL CSUSPSMSResetPCRPID(HCSHANDLE hDemux, WORD wPCRPID, HCSHANDLE hUserData );

/**
@brief �趨��������AC3 Audio PID

@�ýӿ�ֻ�ṩ��UPS ��SMSģ��ʹ�á������ǰϵͳģʽΪPVRģʽ����ֱ�ӷ���TRUE��
��ǰϵͳģʽΪPVRģʽͨ��ƽ̨CSSYSAdvancedParamEx�ӿڽ��г�ʼ�����ã�Ҳ����ͨ��CSSYSSetPvrSupport�ӿ��޸ġ�
@param[in] hDemux   �豸������ò�����Զ�Demux�豸�����ֱ�Ӷ�Ӧ�ײ�ɷ����
									Demux�豸�����š����ڵ�Demux�豸���ò��������壬ֱ�Ӹ�0.
@param[in] wAC3PID   AC3PID
@param[in] hUserData   �û��Զ������ݣ�����
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
@�˺������������趨��������AC3 PID�������漰����Ƶ����Ĵ򿪺͹رա�
 �˺����ĵ��÷����ڽ���������֮�󣬷�����Ҫ�ı��л���ƵPIDΪAC3PID�ĵط�������PMT������ɣ���
*/
BOOL CSUSPSMSResetAC3PID( HCSHANDLE hDemux, WORD wAC3PID, HCSHANDLE hUserData );

/**
@brief ����CA

@param[in] hDemux    ��Ҫ���˵��
@param[in] nOriNetworkID   ��Ҫ���˵��
@param[in] nTSID   ��Ҫ���˵��
@param[in] nServiceID   ��Ҫ���˵��
@param[in] nVideoPID   ��Ҫ���˵��
@param[in] nAudioPID   ��Ҫ���˵��
@param[in] hUserData   ��Ҫ���˵��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
*/
BOOL CSUSPSMSStartCA(HCSHANDLE hDemux, int nOriNetworkID, int nTSID, int nServiceID, int nVideoPID, int nAudioPID, HCSHANDLE hUserData );

/**
@brief ֹͣCA

@param[in] hDemux    ��Ҫ���˵��
@param[in] nVideoPID   ��Ҫ���˵��
@param[in] nAudioPID   ��Ҫ���˵��
@param[in]  hUserData  ��Ҫ���˵��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE 
*/
BOOL CSUSPSMSStopCA( HCSHANDLE hDemux, int nVideoPID, int nAudioPID, HCSHANDLE hUserData );

/**
@brief ֹͣCA��չ�ӿ�,֧�ֶ�·���

@param[in] hDemux    ��Ҫ���˵��
@param[in] nOriNetworkID   ��Ҫ���˵��
@param[in] nTSID   ��Ҫ���˵��
@param[in]  nServiceID  ��Ҫ���˵��
@param[in]  nVideoPID  ��Ҫ���˵��
@param[in]  nAudioPID  ��Ҫ���˵��
@param[in]  hUserData   ��Ҫ���˵��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE
*/
BOOL CSUSPSMSStopCAEx( HCSHANDLE hDemux, int nOriNetworkID, int nTSID, int nServiceID, int nVideoPID, int nAudioPID, HCSHANDLE hUserData );
/**
@brief �׳�PMTԭʼ����

@param[in] hDemux    ��Ҫ���˵��
@param[in] pbPMTData   ��Ҫ���˵��
@param[in] nDataLength   ��Ҫ���˵��
@param[in]  hUserData  ��Ҫ���˵��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE
*/
BOOL CSUSPSMSInjectPMT( HCSHANDLE hDemux, PBYTE pbPMTData, int nDataLength, HCSHANDLE hUserData );

/**
@brief �׳�CATԭʼ����

@param[in] hDemux    ��Ҫ���˵��
@param[in] pbCATData   ��Ҫ���˵��
@param[in] nDataLength   ��Ҫ���˵��
@param[in]  hUserData  ��Ҫ���˵��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE
*/
BOOL CSUSPSMSInjectCAT( HCSHANDLE hDemux, PBYTE pbCATData, int nDataLength, HCSHANDLE hUserData );


/**
@brief �׳�BATԭʼ����

@param[in] hDemux ��Ҫ���˵��
@param[in] pbBATData ��Ҫ���˵��
@param[in] nDataLength   ��Ҫ���˵��
@param[in] nSectionEventType: 		1: section available		2: section complete
@param[in] hUserData ��Ҫ���˵��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE
*/
BOOL CSUSPSMSInjectBAT( HCSHANDLE hDemux, PBYTE pbBATData, int nDataLength, int nSectionEventType, HCSHANDLE hUserData );

/**
@brief �׳�NITԭʼ����

@param[in] hDemux ��Ҫ���˵��
@param[in] pbNITData ��Ҫ���˵��
@param[in] nDataLength   ��Ҫ���˵��
@param[in] nSectionEventType: 		1: section available		2: section complete
@param[in] hUserData ��Ҫ���˵��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE
*/
BOOL CSUSPSMSInjectNIT( HCSHANDLE hDemux, PBYTE pbNITData, int nDataLength, int nSectionEventType, HCSHANDLE hUserData );

/**
@brief �׳�SDTԭʼ����

@param[in] hDemux ��Ҫ���˵��
@param[in] pbSDTData ��Ҫ���˵��
@param[in] nDataLength   ��Ҫ���˵��
@param[in] nSectionEventType 1: section available		2: section complete
@param[in] hUserData ��Ҫ���˵��
@return  ����ɹ��򷵻�ΪTRUE�����ʧ���򷵻�ΪFALSE
*/
BOOL CSUSPSMSInjectSDT( HCSHANDLE hDemux, PBYTE pbSDTData, int nDataLength, int nSectionEventType, HCSHANDLE hUserData );

/**
@brief get user defined pg_rate

@return ��Ҫ���˵��
*/
int CSUSPSMSGetUserPGRate(void);


typedef  void ( *CSPlatAvStandInit)(HCSHANDLE);

#if 0 /*removed CSAvStandInit*/
/*CSAvStandInitͨ��CSSYSSetCfg()����CSPlatAvStandInit���͵Ľӿڣ���ֲ�����
�����÷�Ϊ:
��CSSYSChipBoot()֮ǰ����
	CSSYSSetCfg(CSCFG_SET_CSAVSTANDINIT_API, CSDEVICE_IGNORE, CSAvStandInit, 0);
*/

BOOL CSAvStandInit(HCSHANDLE hUserData);

/********************************************************************************
�ӿ����ƣ� int CSAvStandGetVersion( char* pcVersionInfo, int nSize)
�ӿ���;����ȡ��ģ��İ汾��Ϣ
                          �˺�����ֲ��ֻ�ṩ���룬�����ӵ������Ŀ���ȥ
����˵����
*********************************************************************************/
int CSAvStandGetVersion( char* pcVersionInfo, int nSize);

#endif

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

