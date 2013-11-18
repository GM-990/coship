/*
	��������ļ�����ϸ˵������μ������ӿ��ĵ�
*/

#ifndef ServiceManagerInterface_H
#define ServiceManagerInterface_H

#include "mmcp_typedef.h"
#include "SICommonType.h"

#ifdef __cplusplus
extern "C" 
{
#endif


/*
 *	����ѡ��壬�����ⲿ�ӿ�
 */
#define SM_EPG						0x01		//����EPG
#define SM_UPDATE					0x02		//�����Զ�����
#define SM_TELETEXT					0x04		//����Teletext
#define SM_SUBTITLING				0x08		//����subtitling
#define SM_CA						0x10		//����CA
#define SM_PG_ANY					0x20		//����PG����(��Ӧ���е�PG��Ϣ)
#define SM_PG_ONCE					0x40		//����PG����(��Ӧ��һ��PG��Ϣ)
#define SM_SERVICE_LOCK				0x80		//������Ŀ��������

#define SM_START_PAT_FOR_PMT        0x80000     //��̨��ʱ��������PAT������PMT��PID

#define SM_ONLY_MONITOR_TABLE       0x40000     //����ʱֻ����PAT��BAT��SDT��NIT��CAT
												//���������������������������������Ա�
												//�ֲ��䣬��������

#define SM_BAT_ONLY_MONITOR			0x1000		//Ƶ���л�������BAT���� 
#define SM_NIT_ONLY_MONITOR			0x400		//Ƶ���л�������NIT����
#define SM_SDT_ONLY_MONITOR			0x800		//Ƶ���л�������SDT����

#define SM_CAT_SEARCH_AND_MONITOR	0x200		//Ƶ���л�����CAT����Ȼ����������
#define SM_BAT_SEARCH_AND_MONITOR	0x100		//Ƶ���л�����BAT����Ȼ����������
#define SM_NIT_SEARCH_AND_MONITOR	0x2000		//Ƶ���л�����NIT����Ȼ����������
#define SM_SDT_SEARCH_AND_MONITOR	0x4000		//Ƶ���л�����SDT����Ȼ����������

#define SM_SDT_SEARCH_EVERY_SERVICE 0x8000		//ÿ�λ�̨������һ��ActSDT����Ȼ������
#define SM_PAT_ONLY_MONITOR			0x10000


//���º궨��Ϊ������ǰ�Ľӿڼ��ݶ�����������
//����������ѡ������µĶ�����ʽ
#define SM_BAT_SENSITIVE			0x100		//Ƶ���л�����BAT����Ȼ����������
#define SM_CAT						0x200		//Ƶ���л�����CAT����Ȼ����������
#define SM_CAT_SENSITIVE			0x200		//Ƶ���л�����CAT����Ȼ����������
#define SM_NIT_SENSITIVE			0x400		//Ƶ���л�������NIT����
#define SM_SDT_SENSITIVE			0x800		//Ƶ���л�������SDT����

/*
 *	��������ɲ��Ժ궨�壬�����ⲿ�ӿ�
 */
#define USPSM_COMPLETE_SECTION_NUM 1			  //����SectionNum�ж�Section�Ƿ���ȫ
#define USPSM_COMPLETE_RECYCLE_RET 2			  //���ݰ��ظ��ж�Section�Ƿ���ȫ

typedef enum _ServiceCallbackMessageType
{
  USP_SMR_SERVICE_LOCK
  , USP_SMR_SERVICE_NOLOCK
  , USP_SMR_NO_SIGNAL
  , USP_SMR_SIGNAL_CONNECTED
  , USP_SMR_SIGNAL_LOST
  , USP_SMR_PG_MESSAGE
  , USP_SMR_NO_PG_MESSAGE
  , USP_SMR_PMT_VER_CHANGED
  , USP_SMR_SDT_VER_CHANGED
  , USP_SMR_NIT_VER_CHANGED
  , USP_SMR_PMT_COMPLETE
  , USP_SMR_CAT_COMPLETE
  , USP_SMR_DM_CHANGE
  , USP_SMR_SIGNAL_SEARCHING
  , USP_SMR_NIT_COMPLETE
  , USP_SMR_SDT_COMPLETE
  , USP_SMR_BAT_COMPLETE
  , USP_SMR_RUNNING_STATUS
  , USP_SMR_STARTPLAY_DONE
  , USP_SMR_STOPPLAY_DONE
  , USP_SMR_SOURCE_CHANGE
  , USP_SMR_START_LOCK_SOURCE
  , USP_SMR_PAT_DATA_AVAIL
  , USP_SMR_PAT_DATA_COMPLETE
  , USP_SMR_CANNOTGET_PMT//������PAT����ҵ�PMT��Ӧ��pid������pmt���ǳ�ʱ
  , USP_SMR_NO_CUR_SERVICEID//PMT����º�ɾ���˵�ǰ��Ŀ��service_id
  , USP_SMR_GET_AVAIL_APID//PMT�����кϷ�����Ƶpid
  , USP_SMR_GET_INVALI_APID//PMT����û�кϷ�����Ƶpid
  , USP_SMR_GET_AVAIL_VPID//PMT�����кϷ�����Ƶpid
  , USP_SMR_GET_INVALI_VPID//PMT����û�кϷ�����Ƶpid
  , USP_SMR_PMT_READY
  , USP_SMR_DISABLEAV_FOR_PG
  , USP_SMR_PMT_PID_CHANGE
}ServiceCallbackMessageType;

typedef struct 
{
	unsigned int unAudioPID			:13;	//audio pid
	unsigned int unAudioSMType		:3;		//1: 13818-3audio or 11172audio; 2:AC3Audio
	unsigned int unAudioType		:8;		//audio type: elementary stream type: see 13818-1 Table 2-36
	unsigned int unCanUsedFlag		:1;		//1: audio pid can be used; 0: audio pid can't be used
	unsigned int unLanguageIndex	:7;		//��μ�����SM֧�ֵ����Զ���
}CSUSPSMAudioInfo;		//���ڶ�����֧��

typedef struct _SMData		//SM�������û���һ�λ�����
{
	int m_nType;			//����������

	BYTE* m_pucBuf;			//�������׵�ַ
	int m_nBufLen;			//����������
}SMData;

typedef enum _CSUSPSMServicePlayType
{
	SM_UNKNOW,      //δ��������
	SM_DVB,		    //��ͨDVBҵ��
	SM_RADIO_PAGE	//�㲥ҳ���ţ�ֻ�ı���Ƶ�����ı���Ƶ
}CSUSPSMServicePlayType;

typedef struct _PlayDevice
{
	HCSHANDLE hSourceHandle;//���Ź�����������Ƶ�ľ��
	DWORD     dwDemuxID;//���Ź����и��ֱ������õ�demuxid
}CSUSPSMPlayDevice;

#define  USPSM_MAX_AUDIO_NUM 64		//֧�ֵ����������

#define AUDIO_LANG_UNKNOW			0		//unknown language 
#define AUDIO_LANG_ENG              1       //"English"   Ӣ��
#define AUDIO_LANG_FIN              2       //"Finnish"   ������  
#define AUDIO_LANG_FRAFRE           3       //"French"    ���� 
#define AUDIO_LANG_DEUGER			4       //"German"    ���� 
#define AUDIO_LANG_ELLGRE           5		//"Greek"     ϣ����  
#define AUDIO_LANG_HUN              6		//"Hungarian" ��������
#define AUDIO_LANG_ITA              7       //"Italian"	  �������
#define AUDIO_LANG_NOR              8		//"Norwegian" Ų����
#define AUDIO_LANG_POL              9		//"Polish"    ������      
#define AUDIO_LANG_POR              10		//"Portuguese" ��������
#define AUDIO_LANG_RONRUM           11      //"Romanian"   ����������
#define AUDIO_LANG_RUS              12		//"Russian"    ����
#define AUDIO_LANG_SLV              13		//"Slovenian"  ˹����������
#define AUDIO_LANG_ESLSPA           14		//"Spanish"    ��������   
#define AUDIO_LANG_SVESWE           15      //"Swedish"    �����
#define AUDIO_LANG_TUR              16		//"Turkish"    �������� 
#define AUDIO_LANG_ARA              17		//"Arabic"     ��������
#define AUDIO_LANG_CHIZHO           18      //"Chinese"    �й��� 
#define AUDIO_LANG_CESCZE           19      //"Czech"      �ݿ���  
#define AUDIO_LANG_DAN              20		//"Danish"     ������ 
#define AUDIO_LANG_DUTNLA           21      //"Dutch"      ������

typedef void (*ServiceManagerCallback)(ServiceCallbackMessageType type,void* pParam);

/************************************************************************
                    ���º������ڴ���ServiceManager                  
************************************************************************/

//�������������ڴ���ServiceManager���, ����ServiceManager��������;
//��������������Ӧ�ó����������Ӧ�ñ��ִ���.
HCSHANDLE CSUSPSMCreateServiceManager( HCSHANDLE hDemux, HCSHANDLE hSource, HCSHANDLE hEpg, HCSHANDLE hDataContainer, ServiceManagerCallback fnCallback, HCSHANDLE hUserData );
HCSHANDLE CSUSPSMCreateServiceManagerEx( HCSHANDLE hDemux, HCSHANDLE hMemoryPool, HCSHANDLE hSource, HCSHANDLE hEpg, HCSHANDLE hDataContainer, ServiceManagerCallback fnCallback, int nStackSize, int nPriority, HCSHANDLE hUserData );

//�����������񣬰���CAT��BAT��SDT��NIT
BOOL CSUSPSMStartMonitorThread( HCSHANDLE hServiceManager, DWORD dwOption );
BOOL CSUSPSMStartMonitorThreadEx( HCSHANDLE hServiceManager, int nStackSize, int nPriority, DWORD dwOption );

//�趨NIT��BAT����ʱ��NetworkID��BouquetID.����������Ӧ����������������ĺ�������֮ǰ����
BOOL CSUSPSMSetNetworkID( HCSHANDLE hServiceManager, WORD wNetworkID, BOOL bNetworkIDMask );
BOOL CSUSPSMSetBouquetID( HCSHANDLE hServiceManager, WORD wBouquetID, BOOL bBouquetIDMask );

//�趨�Ƿ���SM�ڲ���NIT/PAT/SDT/BAT����ʱ�Զ������µİ汾���趨�����ݹ���ģ��
//		Ĭ�ϣ��Զ��趨��dwStorageType = 0; ��dwStorageType = 1,��ô�ڲ����Զ��趨;����ֵ�޶���
BOOL CSUSPSMSetTableVersionStorageType( HCSHANDLE hServiceManager, DWORD dwStorageType );

// �趨��Ƶ���ģʽ���˲����ᱻ���͵�SM����CSUSPSMSPlayStream�еĽṹ����
//
//	dwAudioOutputMode����ֵ�ö�����μ�csuspsmsupportinterface.h�й���audiooutputmode�Ķ��壺
//		#define SMS_AUIDO_OUTPUTMODE_UNKNOWN		0
//		#define SMS_AUIDO_OUTPUTMODE_PCM			1
//		#define SMS_AUIDO_OUTPUTMODE_DIGITAL		2
BOOL CSUSPSMSetAudioOutputMode( HCSHANDLE hServiceManager, DWORD dwAudioOutputMode );

//	�趨��̨������Ƶ�л����ͣ���ͳ�����������壬���뵭����
//		����û�������������������������Ϊ0,��Ϊ��ͳģʽ;����Ϊָ��ģʽ,��Ҫƽ̨����Ӧ�汾֧��
//
//	dwServiceSwitchType��ֵ��μ�csuspsmsupportinterface.h�й��ڻ�̨ʱ����Ƶ�Ĵ������͵Ķ��壺
//	#define SMS_SER_SWITCH_TYPE_UNKNOWN				0	//��̨ʱ����Ƶ�Ĵ������ͣ�δ����
//	#define SMS_SER_SWITCH_TYPE_SCREEN_BLACK		1	//��̨ʱ����Ƶ�Ĵ������ͣ�����
//	#define SMS_SER_SWITCH_TYPE_SCREEN_IFRAME		2	//��̨ʱ����Ƶ�Ĵ������ͣ�����
//	#define SMS_SER_SWITCH_TYPE_SCREEN_FADE_IN_OUT	3	//��̨ʱ����Ƶ�Ĵ������ͣ����뵭��
BOOL CSUSPSMSetServiceSwitchType( HCSHANDLE hServiceManager, DWORD dwServiceSwitchType );

//	�趨�Ƿ���PMT������ɺ��Զ���������Ƶ�Ľ���������Ĭ��״̬�£�Ҳ��������ӿڲ������ã����߲��Ա��趨ΪTRUE����
//	SM�ڲ����Զ����½����������������PMT��Ҫ���µĻ���������û��Ѳ����趨ΪFALSE����ôSM��PMT�����ɹ����ٽ�
//	�н��������Զ����¶������������ɻ�����CA���׳�PMTԭʼ���ݣ����������ȶ�����
BOOL CSUSPSMSetAutoUpdatePolicyForPMT( HCSHANDLE hServiceManager, BOOL bAuotUpdatePolicyForPMT );

//�趨BAT��������жϲ���
BOOL CSUSPSMSetBATCompletePolicy( HCSHANDLE hServiceManager, int nBATCompletePolicy );
//�趨NIT��������жϲ���
BOOL CSUSPSMSetNITCompletePolicy( HCSHANDLE hServiceManager, int nNITCompletePolicy );
//�趨Other SDT��������жϲ���
BOOL CSUSPSMSetOtherSDTCompletePolicy( HCSHANDLE hServiceManager, int nOtherSDTCompletePolicy );
//�趨��Ƶ�ɹ�֮�󣬽���������֮ǰ��SM���������ʱ�䡣Ĭ��Ϊ0��������msΪ��λ��
BOOL CSUSPSMSetSleepTimeAtPointA( HCSHANDLE hServiceManager, DWORD dwSleepTime );
//�趨SM��������������Ҫ�ı�����֮��SM������Ĺ���ʱ�䡣Ĭ��Ϊ0��������msΪ��λ��
BOOL CSUSPSMSetSleepTimeAtPointB( HCSHANDLE hServiceManager, DWORD dwSleepTime );
//�����������ServiceManagerʵ�壬�ڳ���������ڲ��ܱ�����
BOOL CSUSPSMDeleteServiceManager( HCSHANDLE hServiceManager );
//�ڴ���SM����ã�֧�ֶ�ʵ��������SM�����Tuner����
BOOL CSUSPSMSetSourceHandle( HCSHANDLE hServiceManager, HCSHANDLE hSource );
//����BAT��ȫ����
BOOL CSUSPSMSetBATSectionCompleteRepeatTimes( HCSHANDLE hServiceManager, int nRepeatTimes );

/************************************************************************
	 ���º������ڲ��ţ�ֹͣ����һ����Ŀ������ر�����Ƶ����� 
************************************************************************/

//����һ����Ŀ
BOOL CSUSPSMPlayService( HCSHANDLE hServiceManager, HCSHANDLE hService, DWORD dwOption );

//ֹͣ�������ڲ��ŵĽ�Ŀ
BOOL CSUSPSMStopPlay( HCSHANDLE hServiceManager );
//ֹͣ�������ڲ��ŵĽ�Ŀ-ͬ����ʽ
BOOL CSUSPSMStopPlaySync( HCSHANDLE hServiceManager );

//������ֹͣ����Ƶ���
BOOL CSUSPSMEnableAV( HCSHANDLE hServiceManager );
BOOL CSUSPSMDisableAV( HCSHANDLE hServiceManager );
BOOL CSUSPSMDisableAudio( HCSHANDLE hServiceManager );
BOOL CSUSPSMDisableVideo( HCSHANDLE hServiceManager );
BOOL CSUSPSMEnableAudio( HCSHANDLE hServiceManager );
BOOL CSUSPSMEnableVideo( HCSHANDLE hServiceManager );

//ֹͣ������Ƶ�������������Ƶ�������ϸ���ӿ��ĵ���
BOOL CSUSPSMMute( HCSHANDLE hServiceManager );
BOOL CSUSPSMUnMute( HCSHANDLE hServiceManager );

//�����Ŀ����PG��
BOOL CSUSPSMUnLockService( HCSHANDLE hServiceManager );
BOOL CSUSPSMUnPGService( HCSHANDLE hServiceManager );

//��ȡ���ڲ��Ž�Ŀ�ľ��
HCSHANDLE CSUSPSMGetCurPlayingServiceHandle( HCSHANDLE hServiceManager );

//��PMT��������ڽ�����֮ǰ
BOOL CSUSPSMEarlyRequestPMT( HCSHANDLE hServiceManager, BOOL bEarlyRequest );

//���ݽ�Ŀ�����Ͳ��Ž�Ŀ
BOOL CSUSPSMPlayServiceByType( HCSHANDLE hServiceManager, HCSHANDLE hService, CSUSPSMServicePlayType enmServiceType, DWORD dwOption );

/************************************************************************
		���º������ڶ����ڲ��ŵĽ�Ŀ���д���						
		��ı���Ƶ����ȡTeletext��                               
************************************************************************/

// ���º���ֻ����PMT������ɺ����

// �õ���ǰ��Ŀ��PMTԭʼ���ݣ�������������ȡ�洢��SM�е�PMTԭʼ���ݵ��׵�ַ�ͳ��ȣ���ǧ��Ҫ���ĵ�ַ�е����ݣ�
const BYTE * CSUSPSMGetPMTDataBuf( HCSHANDLE hServiceManager );
int CSUSPSMGetPMTDataBufLen( HCSHANDLE hServiceManager );

//  �ı��Ŀ����ƵPID������������Ŀǰ���������ĸ�����Ϊ��Ƶ���ԣ�����û��趨ΪNULL����SM��������Ƶ�������ͣ�
BOOL CSUSPSMChangeAudio( HCSHANDLE hServiceManager, DWORD dwAudioPID, int nType, const char * pcLanguageCode );
//  ����ͬ�ϣ��������һ����������Ƶ���Զ�Ӧ��Index����������ļ�����ĺ궨��
BOOL CSUSPSMChangeAudioEx( HCSHANDLE hServiceManager, DWORD dwAudioPID, int nType, int nLanguageIndex );

//	��ȡ��Ŀ�Ĳ�ͬ�����������ԡ�
//	nIndex: ������������벻ͬ��nIndex��ȡ����ͬ�������Ե���Ƶ���ԡ�nIndex��ֵ�� AUDIO_LANG_...��ȷ����//			
//	pAudioInfo: �����������Ŷ�Ӧ��PIDֵ
BOOL CSUSPSMGetAudioInfo( HCSHANDLE hServiceManager, int nIndex, CSUSPSMAudioInfo * pAudioInfo );

//��ȡ���ڲ��Ž�Ŀ�ĵ�һ��AudioPID��������AC3��
HCSHANDLE CSUSPSMGetFirstAudioPID( HCSHANDLE hServiceManager, WORD * pwAudioPID, BYTE * pucStreamType, int * pnLanguageIndex );
//��ȡ���ڲ��Ž�Ŀ����һ��AudioPID��������AC3��
HCSHANDLE CSUSPSMGetNextAudioPID( HCSHANDLE hServiceManager, HCSHANDLE hPre, WORD * pwAudioPID, BYTE * pucStreamType, int * pnLanguageIndex );

//��ȡ���ڲ��Ž�Ŀ��Audio�ܸ�����������AC3��
int CSUSPSMGetAllAudioCount( HCSHANDLE hServiceManager );
//��ȡ���ڲ��Ž�Ŀ��ָ����nIndex��Audio��������AC3��
BOOL CSUSPSMFindIndexAudioPID( HCSHANDLE hServiceManager, int nIndex, WORD * pwAudioPID, BYTE * pucStreamType, int * pnLanguageIndex );

//����������֪��Audio ElemType�����磬03 ��04 ��Audio Stream��AC3 Audio
int CSUSPSMGetAllTypeAudioCount( HCSHANDLE hServiceManager );
//����������֪��Audio ElemType�����磬03 ��04 ��Audio Stream��AC3 Audio
BOOL CSUSPSMFindIndexTypeAudioPID( HCSHANDLE hServiceManager, int nIndex, CSUSPSMAudioInfo * pstAudioInfo );

//�����Ƿ��޶�CAT��PMT����׳�����bCATPMTSeqSensitive��ΪTRUE���׳�CAT���׳�PMT����ΪFALSE��û�д����޶�
BOOL CSUSPSMSetCATPMTSeqSensitive( HCSHANDLE hServiceManager, BOOL bCATPMTSeqSensitive );
//����CAT��PMT����׳������趨
BOOL CSUSPSMGetCATPMTSeqSensitive( HCSHANDLE hServiceManager );

//��ȡ���ڲ��Ž�Ŀ��AC3 PID����û��AC3 PIDʱ������ 0��
WORD CSUSPSMGetAC3PID( HCSHANDLE hServiceManager );
//�趨���ڲ��Ž�Ŀ��AC3 PID�������������AC3������
BOOL CSUSPSMSetAC3Decoder( HCSHANDLE hServiceManager, WORD wAC3PID );

//��ȡ���ڲ��Ž�Ŀ�ĵ�һ��Teletest
HCSHANDLE CSUSPSMGetFirstTeletext( HCSHANDLE hServiceManager, SITeletext * pSITeletext );
//��ȡ���ڲ��Ž�Ŀ����һ��Teletest
HCSHANDLE CSUSPSMGetNextTeletext( HCSHANDLE hServiceManager, HCSHANDLE hPre, SITeletext * pSITeletext );
//��ȡ���ڲ��Ž�Ŀ�ĵ�һ��Subtitle
HCSHANDLE CSUSPSMGetFirstSubtitle( HCSHANDLE hServiceManager, SISubtitle * pSISubtile );
//��ȡ���ڲ��Ž�Ŀ����һ��Subtitle
HCSHANDLE CSUSPSMGetNextSubtitle( HCSHANDLE hServiceManager, HCSHANDLE hPre, SISubtitle * pSISubtile );

//ע��EPG�����������SM���е���;ɾ��EPG������ע��EPG���
BOOL CSUSPSMRegesterEPG( HCSHANDLE hServiceManager, HCSHANDLE hEPG );
//ע��EPG������������û�ɾ��EPG���֮ǰ����SM�б����EPG���ע��
BOOL CSUSPSMUnRegesterEPG( HCSHANDLE hServiceManager );

// ��ȡServiceManager�İ汾��
int CSUSPSMGetVersionInfo( char * pcVersionInfo, int nSize );

BOOL CSUSPSMSetBATUserBouquetIDFilter( HCSHANDLE hServiceManager, BOOL bUserSetBouquetIDSwitch, WORD ucBouquetArray[], int nArrayLen );
//��ʱ����PMT�����ö�ʱʱ�䣬dwInterval�ĵ�λΪms
BOOL CSUSPSMSetRequestPMTInterval( HCSHANDLE hServiceManager, DWORD dwInterval, BOOL bTimeRequestPMT );
//����CA��PGȫ��״̬�����ȼ������bCAPriority��ΪTrue������PGȫ��״̬����CA����CA��Ϣ�׳�
//Ĭ��ΪFALSE
BOOL CSUSPSMSetCAPriorToPG( HCSHANDLE hServiceManager, BOOL bCAPriority );
/******************************************************************************
Function Name:    CSUSPSMRegesterAudioType

Description:��SMע����Ƶ����

Input:
hServiceManager: SM���
pucAudioStreamTypes : ע�����ƵStreamType����0x03����Ϊ���Ȳ�����16���������׵�ַ
pucAudioSMSTypes : 	��ƵStreamType����Ӧ��ƽ̨�������壨���ļ�CSUSPSMSupportedInterface.h�У�����Ƶ���ͣ���SMS_AUIDO_TYPE_NORMAL��SMS_AUIDO_TYPE_MPEG1�ȵȣ���
Ϊ���Ȳ�����16���������׵�ַ�����е�Ԫ����pucAudioStreamTypes��Ԫ��Ҫһһ��Ӧ����pucAudioStreamTypes�ĳ�������ȣ�
wMapCount :	����2 

Output:

Return Value: ע��ɹ�TRUE;
ע��ʧ��FALSE;

Remark:���pucAudioStreamTypes�е�Ԫ�ص�ֵ���ظ��ģ�����FALSE
ע�������streamtype����Ϊ0�����򷵻�FALSE��Ӧ����SMĬ�ϵ�����������ʾ��
�������FALSE����ʹ��SM�ڲ�Ĭ�ϵ�streamtype�������ǣ�
0x03��SMS_AUIDO_TYPE_NORMAL��SMS_AUIDO_TYPE_MPEG1��
0x04��SMS_AUIDO_TYPE_NORMAL��SMS_AUIDO_TYPE_MPEG2��
0x81��SMS_AUIDO_TYPE_AC3��
*******************************************************************************/
BOOL CSUSPSMRegesterAudioType( HCSHANDLE hServiceManager, BYTE * pucAudioStreamType, BYTE * pucAudioSMSType, WORD wMapCount );
/******************************************************************************
Function Name:    CSUSPSMRegesterVideoType

Description:��SMע����Ƶ����

Input:
hServiceManager: SM���
pucAudioStreamTypes : ע�����ƵStreamType����0x01����Ϊ���Ȳ�����16���������׵�ַ
pucAudioSMSTypes : 	��ƵStreamType����Ӧ��ƽ̨�������壨���ļ�CSUSPSMSupportedInterface.h�У�����Ƶ���ͣ���SMS_VIDEO_TYPE_MPEG2�ȵȣ���
Ϊ���Ȳ�����16���������׵�ַ�����е�Ԫ����pucVideoStreamTypes��Ԫ��Ҫһһ��Ӧ����pucVideoStreamTypes�ĳ���Ҳ����ȣ�
wMapCount : ��Ҫע���SMʵ�ʵ���Ƶ���͸��� 

Output:

Return Value: ע��ɹ�TRUE;
ע��ʧ��FALSE;

Remark:	1�����pucVideoStreamTypes�е�Ԫ�ص�ֵ���ظ��ģ�����FALSE
2��ע�������streamtype����Ϊ0�����򷵻�FALSE��Ӧ����SMĬ�ϵ�����������ʾ��
3���������FALSE����ʹ��SM�ڲ�Ĭ�ϵ�streamtype�������ǣ�
0x01��SMS_AUIDO_TYPE_MPEG2��
0x02��SMS_AUIDO_TYPE_MPEG2��
*******************************************************************************/
BOOL CSUSPSMRegesterVideoType( HCSHANDLE hServiceManager, BYTE * pucVideoStreamType, BYTE * pucVideoSMSType, WORD wMapCount );

BOOL CSUSPSMPlayByDevice( HCSHANDLE hServiceManager, HCSHANDLE hService, CSUSPSMServicePlayType enmServiceType, CSUSPSMPlayDevice sPlayDevice, DWORD dwOption );
#ifdef __cplusplus
}
#endif


#endif /* ServiceManagerInterface_H */
