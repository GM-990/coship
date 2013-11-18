#ifndef NVODInterface_H
#define NVODInterface_H

/* {{{USR
NVOD�ӿ�
   }}}USR */

#include "mmcp_typedef.h"
#include "mmcp_debug.h"
#include "mmcp_os.h"
#include "CSUSPDMCommonType.h"
#include "CSUSPCommonType.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define  NVOD_SERVICE_NAME     (64)
#define  NVOD_MAX_EVENT_NAME    (64)
#define  NVOD_MAX_DESCRIPTION     (256)

//NVOD Ext interface
typedef enum _NVODExtCallbackType
{
	EXT_NVOD_BEGIN,    //���ݿ�ʼ����ǰ��׼������ 
	EXT_NVOD_ANALYLSE,   //���ݴ���
	EXT_NVOD_END,       //���ݴ�����ɺ󣬰ѽ�����ظ�NVODģ�����ڱ���
	EXT_NVOD_FREEDATA    //ɾ������

}NVODExtCallbackType;

typedef enum _NVODExtANALYLSERet
{
    EXT_NVOD_ANALYLSE_NOTPARSE,      //�û�δ����
    EXT_NVOD_ANALYLSE_SUCCEED,       //�û�����ɹ�
    EXT_NVOD_ANALYLSE_FAILED        //�û����Դ���������ռ�ʱʧ��
}NVODExtANALYLSERet;

typedef void ( *NVODExt ) ( NVODExtCallbackType enmType, HCSHANDLE* pParam );
//end NVOD Ext interface


typedef struct _ReferenceService
{
	//ԭʼ����ID
	WORD m_wOrigianlNetworkID;

	//������ID
	WORD m_wTsID;

	//��Reference Service��Service ID
	WORD m_wServiceID;

	//��Reference Service��service name
	BYTE m_pServiceName[ NVOD_SERVICE_NAME ];

	//���뷽ʽ
	BYTE m_ucLanguage;
}ReferenceService;

typedef struct _TimeShiftedService
{
	//ԭʼ����ID
	WORD m_wOriginalNetworkID;

	//������ID
	WORD m_wTsID;

	//��Service��Service ID
	WORD m_wServiceID;

	WORD m_wRefServiceID;
}TimeShiftedService;

typedef struct _ReferenceEvent
{
	//��ĸָ������
	BYTE m_ucParental;

	//��Ŀ����1
	BYTE m_ucContentLevel2;

	//��Ŀ����2
	BYTE m_ucContentLevel1;

	//��Ŀ����
	BYTE m_pEventName[ NVOD_MAX_EVENT_NAME ];

	//��Ŀ���
	BYTE m_pTextDescription[ NVOD_MAX_DESCRIPTION ];

	//Event ID
	WORD m_wEventID;

	//��Ŀ��Ƭ��
	CSUDIPLUSOSTIME_S m_oDuration;

	WORD m_OriginalNetworkID;

	WORD m_wTsID;

	WORD m_wServiceID;

	//���뷽ʽ
	BYTE m_ucLanguage;

	//��չ�ֶ�
	HCSHANDLE m_hExtInfo;
}ReferenceEvent;

typedef struct _TimeShiftedEvent
{
	//��ʱ��ƫ�ƽ�Ŀ�Ŀ�ʼʱ��
	CSUDIPLUSOSSysTime_S m_oStartTime;

	//��ʱ��ƫ�ƽ�Ŀ��event ID
	WORD m_wEventID;

	//��ʱ��ƫ�ƽ�Ŀ�Ľ���ʱ��
	CSUDIPLUSOSSysTime_S m_oEndTime;

	WORD m_OriginalNetworkID;

	WORD m_wTsID;

	WORD m_wServiceID;
}TimeShiftedEvent;

typedef struct _NVODDeliver
{
	WORD m_wOriginalNetworkID;

	WORD m_wTSID;

	Deliver m_tDeliver;
}NVODDeliver;

typedef enum _NVODCallbackType
{
	NVOD_FINISH,                   //��ʾ�˳�NVOD�������������pParam����NVODƵ��ĸ��������NVODƵ��ĸ�������0����ʾ����ʧ�ܣ�����������ʾ����������ɡ�
	NVOD_TMSHFTEVENT_COMPLETE,     //��ʾĳ��ReferenceEvent�µ�TimeShiftEvent���꣬pParam����ReferenceEvent ����ֵ�ĵ�ַ��int*��
	NVOD_REFEVENT_COMPLETE,        //��ʾĳReferenceService�µ�ReferenceEvent���꣬pParam����NULL
	NVOD_SERVICE_COMPLETE          //��ʾĳ��ReferenceService�µ�TimeShiftServic���꣬pParam����ReferenceService����ֵ�ĵ�ַ

}NVODCallbackType;

typedef enum _NVODLevelType
{
	NVOD_CONTENT_LEVEL,
	NVOD_USER_LEVLE
	
}NVODLevelType;

typedef void( *NVODCallBack ) ( NVODCallbackType enmType, void* pParam );


/* {{{USR
����NVOD�õ�����Դ������NVOD��Դ���������ʹ��NVODģ��ĵ�һ�����������hSection��hMemoryPool��pCallBack������һ����Ч����ΪNULL����������Դ���ʧ�ܣ�����NULL��
   }}}USR */
HCSHANDLE CSUSPInitializeNVOD( HCSHANDLE hSection, HCSHANDLE hMemoryPool, NVODCallBack pCallBack, float nTimeOffset );

/* {{{USR
��NVOD��Ҫ�˳�NVODģ�����ʹ��NVODģ��ʱ�����ô˺����ͷ�NVOD��Դ��
�ú�������ֹͣNVOD������Ȼ���ͷ�NVOD������ռ��Դ������ͷ�NVOD���. 
�������hNVODΪNULL��Ƿ�ָ�룬����FALSE;
����ͷ���Դʧ�ܷ���FALSE.
   }}}USR */
BOOL CSUSPFreeNVOD( HCSHANDLE hNVOD );

/* {{{USR
ɾ��NVOD���ݣ� �������ͷ���Դ��
��NVOD��������ʱ����ֹͣ������Ȼ��ɾ�����ݡ�
�������hNVODΪNULL��Ƿ�ָ�룬��������FALSE��
   }}}USR */
BOOL CSUSPDeleteAllNVODData( HCSHANDLE hNVOD );

/* {{{USR
��NVOD��Դ��ʼ��������ô˺�������NVOD���ݵ�������ע�⣺��ͨ�������ӿڻ�ȡNVOD����ʱ��Ҫ�ȵ��ô˺�������NVOD���ݡ�
�������hNVODΪNULL��Ƿ�ָ�룬��������FALSE��
hDeliverList �� hDMContainer ����ͬʱΪnull;
��� hDeliverList �� hDMContainer ��Ϊ null ����Чʱ��������ǰƵ������ȡNVOD����
��� hDeliverList Ϊ null ����Чʱ�� �����ݿ�����ȡNVOD����;
��� hDMContainer Ϊ null ����Чʱ�� ��Ƶ������ȡNVOD����.
   }}}USR */
BOOL CSUSPSearchNvodInfo( HCSHANDLE hNVOD, HCSHANDLE hDeliverList, int nListCount, HCSHANDLE hDMContainer, HCSHANDLE hSource, BOOL bIsNVODFreq );

/* {{{USR
����ҪֹͣNVOD���ݵ�����ʱ�����ô˺���ֹͣNVOD������
�������hNVODΪNULL��Ƿ�ָ�룬����FALSE����ʾֹͣNVOD����ʧ�ܡ� 
   }}}USR */
BOOL CSUSPStopNVOD( HCSHANDLE hNVOD );

/* {{{USR
���û�ע��Ļص���������NVOD_SERVICE_COMPLETEʱ����ʾĳ��Ƶ���»����е�Service���꣬�û����Ե��ô˺�����ȡ��ǰ�Ѿ��������� Reference Service ������Ŀ������ص�����û�з��ش�����ʱ���û����ô˽ӿڵõ��Ľ�����ܲ���ȫ��
�������hNVODΪNULL��Ƿ�ָ�룬����-1����ʾ��ȡʧ�ܡ�
   }}}USR */
int CSUSPNVODGetRefSrvcNum( HCSHANDLE hNVOD );

/* {{{USR
���û�ע��Ļص���������NVOD_SERVICE_COMPLETEʱ����ʾĳ��Ƶ���»����е�Service���꣬�û����Ե��ô˺�����ȡָ��Reference Service����Ϣ�� nRefServiceIndex��ȡֵ��ΧӦ����Reference Service ������Ŀ֮�ڡ�
�������hNVOD��pRefServiceΪNULL��Ƿ�ָ�롢nRefServiceIndex��������������Ч��Χ֮�ڣ��򷵻�FALSE����ʾ��ȡʧ�ܡ� 
   }}}USR */
BOOL CSUSPNVODGetRefSrvcInfo( HCSHANDLE hNVOD, int nRefServiceIndex, ReferenceService * pRefService );

/* {{{USR
���û�ע��Ļص���������NVOD_SERVICE_COMPLETEʱ����ʾĳ��Ƶ���»����е�Service���꣬�û����Ե��ô˺�����ȡָ��Reference Service��TimeShiftedService�ĸ���������ص�����û�з��ش�����ʱ���û����ô˽ӿڵõ��Ľ�����ܲ���ȫ��nRefServiceIndex ��ȡֵ��ΧӦ����Reference Service ������Ŀ֮�ڡ�
���nRefServiceIndex�����ڻ�����Ч��Χ֮�ڣ�����0��
�������hNVODΪNULL��Ƿ�ָ�룬����-1����ʾ��ȡʧ�ܡ�
   }}}USR */
int CSUSPNVODGetTimeShiftedSrvcNum( HCSHANDLE hNVOD, int nRefServiceIndex );

/* {{{USR
���û�ע��Ļص���������NVOD_SERVICE_COMPLETEʱ����ʾĳ��Ƶ���»����е�Service���꣬�û����Ե��ô˺�����ȡָ��Reference Service��ָ��TimeShiftedService����Ϣ��nRefServiceIndex��nTimeShiftedIndex��ȡֵ��ΧӦ�÷ֱ���Reference Service����Ӧtimeshifted serviece ������Ŀ֮�ڡ�
�������hNVOD��pTimeShiftedServiceΪNULL��Ƿ�ָ�룬����FALSE��
���nRefServiceIndex��nTimeShiftedIndex���������������ڣ��򷵻�FALSE����ʾ��ȡʧ�ܡ�
   }}}USR */
BOOL CSUSPNVODGetTimeShiftedSrvcInfo ( HCSHANDLE hNVOD, int nRefServiceIndex, int nTimeShiftedIndex, TimeShiftedService * pTimeShiftedService );

/* {{{USR
���û�ע��Ļص���������NVOD_REFEVENT_COMPLETEʱ���û����Ե��ô˺�����ȡ��ǰ�Ѿ��������� Reference Event ������Ŀ������ص�����û�з��ش�����ʱ���û����ô˽ӿڵõ��Ľ�����ܲ���ȫ��
�������hNVODΪNULL��Ƿ�ָ��,�򷵻�-1��
   }}}USR */
int CSUSPNVODGetRefEvtNum( HCSHANDLE hNVOD );

/* {{{USR
���û�ע��Ļص���������NVOD_REFEVENT_COMPLETEʱ���û����Ե��ô˺�����ȡָ��Reference Event����Ϣ��nRefEventIndex��ȡֵ��ΧӦ����Reference Event������Ŀ֮�ڡ�
�������hNVOD��pReferenceEventΪNULL��Ƿ�ָ���򷵻�FALSE����ʾ��ȡʧ��;
���nRefEventIndex�����ڣ��򷵻�FALSE����ʾ��ȡʧ�ܡ�
   }}}USR */
BOOL CSUSPNVODGetRefEvtInfo( HCSHANDLE hNVOD, int nRefEventIndex, ReferenceEvent * pReferenceEvent );

/* {{{USR
���û�ע��Ļص���������NVOD_REFEVENT_COMPLETEʱ���û����Ե��ô˺�����ȡָ��ReferenceService�²��ҵ�ǰ�Ѿ��������� Reference Event ������Ŀ������ص�����û�з��ش�����ʱ���û����ô˽ӿڵõ��Ľ�����ܲ���ȫ��nRefServiceIndex��ȡֵ��ΧӦ����Reference Service ������Ŀ֮�ڡ�
���nRefServiceIndex�����ڻ�����Ч��Χ֮�ڣ�����0��
�������hNVODΪNULL��Ƿ�ָ��,�򷵻�-1��
   }}}USR */
int CSUSPNVODGetRefEvtNumByRefSrvc( HCSHANDLE hNVOD, int nRefServiceIndex );

/* {{{USR
���û�ע��Ļص���������NVOD_REFEVENT_COMPLETEʱ���û����Ե��ô˺�����ȡָ��ReferenceService��ָ��Reference Event����Ϣ��nRefServiceIndex��ȡֵ��ΧӦ����Reference Service ������Ŀ֮�ڡ�nRefEventIndex��ȡֵ��ΧӦ����Reference Event������Ŀ֮�ڡ�
�������hNVOD��pReferenceEventΪNULL��Ƿ�ָ���򷵻�FALSE����ʾ��ȡʧ��;
���nRefServiceIndex�����ڻ�����Ч��Χ֮�ڣ��򷵻�FALSE����ʾ��ȡʧ�ܣ�
���nRefEventIndex�����ڣ��򷵻�FALSE����ʾ��ȡʧ�ܡ�
   }}}USR */
BOOL CSUSPNVODGetRefEvtInfoByRefSrvc( HCSHANDLE hNVOD, int nRefServiceIndex, int nRefEventIndex, ReferenceEvent * pReferenceEvent );

/* {{{USR
���û�ע��Ļص��������� NVOD_TMSHFTEVENT_COMPLETE ʱ���û����Ե��ô˺�����ȡָ��Reference Event��TimeShiftedEvent����Ŀ������ص�����û�з��ش�����ʱ���û����ô˽ӿڵõ��Ľ�����ܲ���ȫ��nRefEventIndex��ȡֵ��ΧӦ����Reference Event ������Ŀ֮�ڡ�
�������hNVODΪNULL��Ƿ�ָ�룬����-1����ʾ��ȡʧ�ܡ�
���nRefEventIndex�������������ڣ�����0��
   }}}USR */
int CSUSPNVODGetTimeShiftedEvtNum( HCSHANDLE hNVOD, int nRefEventIndex );

/* {{{USR
���û�ע��Ļص��������� NVOD_TMSHFTEVENT_COMPLETE ʱ���û����Ե��ô˺�����ȡָ�� Reference Event ��ָ�� TimeShiftedEvent ����Ϣ��nRefEventIndex ��ȡֵ��ΧӦ����Reference Event ������Ŀ֮�ڣ�nTimeSftIndex ��ȡֵ��ΧӦ����ָ�� Reference Event �µ� TimeShiftedEvent ������Ŀ֮�ڡ�
�������hNVOD��pTimeShiftedEventΪNULL��Ƿ�ָ�룬 ����FALSE����ʾ��ȡʧ�ܣ�
���nRefEventIndex��nTimeSftIndex�����ڣ��򷵻�FALSE����ʾ��ȡʧ�ܡ�
   }}}USR */
BOOL CSUSPNVODGetTimeShiftedEvtInfo( HCSHANDLE hNVOD, int nRefEventIndex, int nTimeSftIndex, TimeShiftedEvent * pTimeShiftedEvent );

/* {{{USR
��ĳ�� ReferenceEvent �µ� TimeShiftEvent �ѵ����û����Ե��ô˺�����ȡ���Ÿ�TimeShiftedEvent ��Ŀ��������Ϣ��nRefEventIndex ��ȡֵ��ΧӦ���� Reference Event ������Ŀ֮�ڣ�nTimeSftIndex ��ȡֵ��ΧӦ����ָ�� Reference Event �µ� TimeShiftedEvent ������Ŀ֮�ڡ� 
�������hNVODΪNULL��Ƿ�ָ�룬����pwOrgNetworkID��pwTsID��pwServiceIDΪNULL���򷵻�FALSE����ʾ��ȡʧ�ܣ�
���nRefEventIndex �� nTimeSftIndex �����ڣ��򷵻�FALSE����ʾ��ȡʧ�ܡ�
   }}}USR */
BOOL CSUSPNVODGetTimeShiftedPlayInfo( HCSHANDLE hNVOD, int nRefEventIndex, int nTimeSftIndex, WORD * pwNetworkID, WORD * pwTsID, WORD * pwServiceID );

/* {{{USR
��ĳ��ReferenceEvent�µ�TimeShiftEvent������û����Ե��ô˺�����ȡָ��Reference Event������ѿ�ʼ�ͼ�����ʼ��TimeShiftedEvent��Ϣ������ֵ��nRefEventIndex ��ȡֵ��ΧӦ���� Reference Event ������Ŀ֮�ڡ�
�������hNVODΪNULL��Ƿ�ָ�룬��nRefEventIndex�Ĳ����ڣ��򷵻�FALSE�� ��ʾ��ȡʧ�ܣ�
�������pRecentIndex��pRecent��pFutureIndex��pFutureΪNULL���򷵻�FALSE��
   }}}USR */
BOOL CSUSPNVODGetRcntFutTimeShiftedEvt ( HCSHANDLE hNVOD, int nRefEventIndex, int * pRecentIndex, TimeShiftedEvent * pRecent, int * pFutureIndex, TimeShiftedEvent * pFuture );

/* {{{USR
���û�ע��Ļص��������� NVOD_TMSHFTEVENT_COMPLETE ʱ���û����Ե��ô˺�����ȡָ��Reference Event��ĳ��ʱ��Σ����ں�ʱ�䣩��TimeShiftedEvent����ʼ�����nRefEventIndex��ȡֵ��ΧӦ����Reference Event ������Ŀ֮�ڡ�
�������hNVODΪNULL��Ƿ�ָ��,��nRefEventIndex�����ڣ��򷵻�NULL����ȡʧ��;
�������pStartTSYSTEMTIME��pEndTSYSTEMTIMEΪNULL���򷵻�NULL����ȡʧ�ܡ�
   }}}USR */
HCSHANDLE CSUSPNVODGetDateTimeEvtHandle( HCSHANDLE hNVOD, int nRefEventIndex, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );

/* {{{USR
���û�ע��Ļص��������� NVOD_TMSHFTEVENT_COMPLETE ʱ���û����Ե��ô˺�����ȡָ��Reference Event��ĳ��ʱ��Σ����ں�ʱ�䣩��TimeShiftedEvent����Ŀ������ص�����û�з��ش�����ʱ���û����ô˽ӿڵõ��Ľ�����ܲ���ȫ��nRefEventIndex��ȡֵ��ΧӦ����Reference Event ������Ŀ֮�ڡ�
�������hNVODΪNULL��Ƿ�ָ�룬����pStartTSYSTEMTIME��pEndTSYSTEMTIMEΪNULL���򷵻�-1����ʾ��ȡʧ��;
���nRefEventIndex�����ڣ�����0��
   }}}USR */
int CSUSPNVODGetDateTimeEvtNum( HCSHANDLE hNVOD, int nRefEventIndex, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );

/* {{{USR
���û�ע��Ļص��������� NVOD_TMSHFTEVENT_COMPLETE ʱ���û����Ե��ô˺�����ȡָ��Reference Event��ĳ��ʱ��Σ����ں�ʱ�䣩��ָ��TimeShiftedEvent����Ϣ��ע�⣺����hGetDataӦ�ôӽӿ�CSUSPNVODGetDateTimeEvtHandle��ã�����nTimeSftIndex�ķ�ΧӦ�ôӽӿ�CSUSPNVODGetDateTimeEvtNum��á�
�������hNVODΪNULL��Ƿ�ָ�룬nTimeSftIndex���������������ڣ�����hGetData��pTimeShiftedEventΪNULL���򷵻�FALSE����ʾ��ȡʧ�ܡ�
   }}}USR */
BOOL CSUSPNVODGetDateTimeEvtInfo( HCSHANDLE hNVOD, HCSHANDLE hGetData, int nTimeSftIndex, TimeShiftedEvent * pTimeShiftedEvent );

/* {{{USR
���û�ע��Ļص���������NVOD_REFEVENT_COMPLETEʱ���û����Ե��ô˺�����ȡ��ǰ�Ѿ��������Ĳ�����ָ����Ŀ���͵�Reference Event����Ŀ������ص�����û�з��ش�����ʱ���û����ô˽ӿڵõ��Ľ�����ܲ���ȫ��
�������hNVODΪNULL��Ƿ�ָ�룬����nContentType1��nContentType2ֵ�ķ�Χ�����ϱ�׼���򷵻�-1����ʾ��ȡʧ�ܡ�
   }}}USR */
int CSUSPNVODGetTypeRefEvtNum( HCSHANDLE hNVOD, int nContentLevel1, int nContentLevel2 );

/* {{{USR
���û�ע��Ļص���������NVOD_REFEVENT_COMPLETEʱ���û����Ե��ô˺�����ȡ��ǰ�Ѿ��������Ĳ�����ָ����Ŀ���͵�ָ��Reference Event��Ϣ������nRefEventIndexֵ�ķ�ΧӦ�ôӽӿ�CSUSPNVODGetTypeRefEvtNum��á�
�������hNVODΪNULL��Ƿ�ָ�룬nTimeSftIndex���������������ڣ�����nContentType1��nContentType2ֵ�ķ�Χ�����ϱ�׼������pReferenceEventΪNULL���򷵻�FALSE����ʾ��ȡʧ�ܡ�
   }}}USR */
BOOL CSUSPNVODGetTypeRefEvtInfo( HCSHANDLE hNVOD, int nRefEventIndex, int nContentLevel1, int nContentLevel2, ReferenceEvent * pReferenceEvent );

/* {{{USR
���û�ע��Ļص���������NVOD_FINISHʱ���û����Ե��ô˺�����ȡ��ǰ�Ѿ���������NVODƵ��ĸ�����
�������hNVODΪNULL��Ƿ�ָ�룬�򷵻�-1����ʾ��ȡʧ�ܡ�
return : Int, ����Ƶ�����Ŀ������-1��ʾ��ȡʧ�ܣ�0��ʾû��Ƶ����Ϣ��
   }}}USR */
int CSUSPNVODGetFreqDataNum( HCSHANDLE hNVOD );

/* {{{USR
���û�ע��Ļص���������NVOD_FINISHʱ���û����Ե��ô˺�����ȡ��ǰ�Ѿ�������ָ��������NVODƵ����Ϣ���������hNVODΪNULL��Ƿ�ָ�룬nFreqIndex��ȡֵ��Χ�������򷵻�FALSE����ʾ��ȡƵ��ʧ�ܡ� 
return: BOOL TRUE��ʾ��ȡNVODƵ��ɹ���FALSE��ʾ��ȡNVODƵ��ʧ��
   }}}USR */
BOOL CSUSPNVODGetFreqData( HCSHANDLE hNVOD, int nFreqIndex, NVODDeliver * pNVODDeliver );

/* {{{USR
��NVOD��Դ��ʼ��������ô˺�������NVOD���ݵ�������ע�⣺��ͨ�������ӿڻ�ȡNVOD����ʱ��Ҫ�ȵ��ô˺�������NVOD���ݡ�
�������hNVODΪNULL��Ƿ�ָ�룬��������FALSE��
hDeliverList �� hDMContainer ����ͬʱΪnull;
��� hDeliverList �� hDMContainer ��Ϊ null ����Чʱ��������ǰƵ������ȡNVOD����
��� hDeliverList Ϊ null ����Чʱ�� �����ݿ�����ȡNVOD����;
��� hDMContainer Ϊ null ����Чʱ�� ��Ƶ������ȡNVOD����.
   }}}USR */
BOOL CSUSPSearchNvodInfobyOther( HCSHANDLE hNVOD, HCSHANDLE hDeliverList, int nListCount, HCSHANDLE hDMContainer, HCSHANDLE hSource, BOOL bIsNVODFreq, BOOL bIsFindOther );

/* {{{USR
�ı�NVODģ���е�ʱ��ƫ��ֵ��
   }}}USR */
BOOL CSUSPNVODChangeTimeZone( HCSHANDLE hNVOD, float fTimeOffset );

/* {{{USR
CSUSPNVODRegisterUserListener
   ���û���Ҫʹ��û�а�����NVOD�����ṹ������ʱ�����ô˺������û��Զ���Ĵ�����չ���ݵ������ע�ᵽNVODģ���У�
   NVODģ��Ѱ�����չ���ݵ������������˶����Ա����û��õ���Ҫ�����ݡ�
   �������hNVOD��hExtProcessΪNULL��Ƿ�ָ���򷵻�FALSE����ʾע��ʧ��;
   }}}USR */
BOOL CSUSPNVODRegisterUserListener( HCSHANDLE hNVOD, HCSHANDLE hExtProcess );

/* {{{USR
����ֻ�洢ĳһʱ��ε�TimeshiftEvent����
   }}}USR */
BOOL CSUSPNVODSetEventDuration( HCSHANDLE hNVOD, CSUDIPLUSOSSysTime_S * pStartTime, CSUDIPLUSOSSysTime_S * pEndTime );

/* {{{USR
���ù���NVOD��������ز����������Ƿ�ǿ��ָ�������������ݹ���������֪ͨNVODģ��������NIT���Ƿ���Servcie_List_Descriptor
���������˽ӿ���Ҫ��NVOD����ǰ���á�
 ������
 bSearchFlag�� ����TRUE, ��ʾǿ��ָ�������������ݹ���������
               ����FALSE, ��ʾǿ��ָ�����������ݹ���������
 bIsSearchList������TRUE, ��ʾ��NIT���д���Servcie_List_Descriptor��������
                ����FALSE, ��ʾ��NIT���в�����Servcie_List_Descriptor��������
                Ĭ��ΪTRUE��
   }}}USR */
BOOL CSUSPNVODSetSearchFlag( HCSHANDLE hNVOD, BOOL bSearchFlag, BOOL bIsSearchList );

/* {{{USR
����������EIT���͵��������˽ӿ���Ҫ��NVOD����ǰ���á�
 ������
   bIsScheduleToSimple��
           ���ΪTRUE,��ʾǰ�˰�TimeShiftEvent������TableID ����0x4e��0x4f�ı��з��͡�
           ���ΪFALSE,��ʾǰ�˰�TimeShiftEvent������TableID ����0x50-0x5f��0x60-0x6f�ı��з��͡�
           NVODģ����Ĭ��ΪFALSE��
   }}}USR */
BOOL CSUSPNVODSetStreamType( HCSHANDLE hNVOD, BOOL bIsScheduleToSimple, void * nParam );

/* {{{USR
����NVOD�����ĳ�ʱʱ�䡣����NVODģ����û�����ʱ�����˴������������ĳ�ʱʱ�䲻һ�����û������õ�ʱ��һ�¡�
 ������
   nTimeOut�� �����ĳ�ʱʱ�䣬�Ժ���Ϊ��λ
   }}}USR */
BOOL CSUSPNVODSetTimeOut( HCSHANDLE hNVOD, int nTimeOut, void * nParam );

/* {{{USR
���ò���Ƶ��NVOD������ʹ�ô��ַ�ʽ��ԭ�����ڽ���NVOD����������Ҫ���Ž�Ŀ��Ҳ��ҪNVOD��������������������������Ƶ�㲻ͬ��
�ͻ�������⣬�����ṩ���ֹ����ڲ��ŵĵ�ǰƵ������NVOD���ݣ����ֹ���Ҳ������Ϊʵʱ�������ݵķ�������NVOD����������Timer����������������
   }}}USR */
BOOL CSUSPNVODSetIsLockFreq( HCSHANDLE hNVOD, BOOL bIsLock );

/* {{{USR

   }}}USR */
BOOL CSUSPNVODSetFreqData( HCSHANDLE hNVOD, DWORD dwFreqency, WORD wOriNetworkID, WORD wTSID, void * pParam );

/* {{{USR
����NVOD�����̵߳����ȼ��Ͷ�ջ��С��Ĭ�ϵ����ȼ���17*7�� �̶߳�ջ��С��1024*10 B.
   }}}USR */
BOOL CSUSPNVODSetThreadParam( HCSHANDLE hNVOD, DWORD dwPrioriy, DWORD dwStackCount, void * pParam );

/* {{{USR
�����Ƿ��û��ʱ���¼��Ĳο��¼����ظ��û���
 ������
  bIsDisplay��ΪTRUE����ʾ��û��ʱ���¼��Ĳο��¼����ظ��û���
             ΪFALSE����ʾ����û��ʱ���¼��Ĳο��¼����ظ��û���
   }}}USR */
BOOL CSUSPNVODSetIsDisplayBadRefEvt( HCSHANDLE hNVOD, BOOL bIsDisplay );

/* {{{USR
������������ɲ�����ȫʱ���Ƿ�ɾ��ǰ���ѵ�������û���ѵ������ݡ�
 ����
  bIsDeleteData��ΪTRUE����ʾɾ���ϵ����ݡ�
             ΪFALSE����ʾ��ɾ���ϵ����ݡ�
   }}}USR */
BOOL CSUSPNVODIsDeleteOldData( HCSHANDLE hNVOD, BOOL bIsDeleteData );

/* {{{USR
ɾ��ָ��ʱ�����ǰ������ʱ���¼���TimeShiftEvent������ĳ��RefEvent������ʱ���¼�
������Ϻ���Ҫ�ڽ����ϲ���ʾ��RefEvent���û����Ե��ô˽ӿ�ɾ������ʱ���¼�����
ʱ��ȡRefEvent����ʱ�Ͳ���õ�RefEvent��
 ����
    hNVOD��  NVODģ������
    pCurSYSTEMTIME���û�ָ����ʱ��㡣
   }}}USR */
BOOL CSUSPNVODDeleteOldTimeShtEvent( HCSHANDLE hNVOD, CSUDIPLUSOSSysTime_S * pCurSYSTEMTIME );

/* {{{USR
��NOVDģ�������ݹ�����ӽ�Ŀʱ����Ҫ�ȵõ�Ƶ����Ϣ���˽ӿ�����ָ��Ƶ��Ĺؼ��֡��ؼ���
������ѡ�񣺣�1��OriginalNetworkID��TSID (2)Ƶ�ʡ�
 ���� 
    hNVOD��  NVODģ������
    bIsFreqency ������TRUE,��ʾ��Ƶ��Ϊ�ؼ��֣�����FALSE,��ʾ�� 
                OriginalNetworkID��TSIDΪ�ؼ��֡�
   }}}USR */
BOOL CSUSPNVODSetUniqueFreqFlag( HCSHANDLE hNVOD, BOOL bIsFreqency );

/* {{{USR
����������ReferenceEvent�������ĵ�һ��TimeShiftedEvent��starttime���Ⱥ��������
   }}}USR */
BOOL CSUSPNVODRefEventSortByStartTime( HCSHANDLE hNVOD );

/* {{{USR
�����Ƿ���ҪNIT����������Service�Ĺ���
   }}}USR */
BOOL CSUSPNVODIsNeedServiceByNIT( HCSHANDLE hNVOD, BOOL IsNeedSerByNIT );

/* {{{USR
�����Ƿ�����ж�Section������ȫ�Ĺ���
   }}}USR */
BOOL CSUSPNVODIsJudgeSectionComplete( HCSHANDLE hNVOD, BOOL IsNeedJudge );

/* {{{USR
����������ReferenceService ����������������Ϊ��ServiceID,����ʽΪ����
}}}USR */
BOOL CSUSPNVODRefServiceSortByServiceID( HCSHANDLE hNVOD );

/* {{{USR
�����Ƿ���ʾû�ж�ӦReferenceEvent��ReferenceService
����
hNVOD��  NVODģ������
bIsDisplay��TRUE:��ʾ
            FALSE������ʾ�����˵�û�ж�ӦReferenceEvent��ReferenceService��
}}}USR */
BOOL CSUSPNVODSetIsDisplayBadRefSrvc(HCSHANDLE hNVOD, BOOL bIsDisplay);

/* {{{USR
�õ�NVODģ��İ汾����Ϣ��
   }}}USR */
int CSUSPNVODGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
�û��趨����EIT��ʱ��PID
����
    hNVOD��  NVODģ������
    dwPID��  EIT���PID��Ĭ��Ϊ0x0012;
}}}USR */
BOOL CSUSPNVODSetPID( HCSHANDLE hNVOD, DWORD dwPID );
/* {{{USR
��NVODģ���ڲ�����ռ��Ա��������Ϣ������չ����������Ϣ��
CSUSPNVODMallocMemory

Parameter:  
    [in] hNVOD    : ��Դ���
    [in] nCount ����ʾ������Ŀռ��С�� 

RETURN VALUE  
    NULL��ʾ����ʧ��
    ��NULL ��ʾ����ɹ���
}}}USR */
PBYTE CSUSPNVODMallocMemory( HCSHANDLE hNVOD, int nCount );

/* {{{USR
�ͷ���NVODģ���ڲ�����Ŀռ䣺����չ����������Ϣ��
CSUSPNVODFreeMemory

Parameter:  
    [in] hNVOD    : ��Դ���
    [in] pvMemoryAddr ���ڴ�ָ�롣 

Return Values
    1������ɹ�����TRUE
    2�����ʧ�ܷ���FALSE
}}}USR */
BOOL CSUSPNVODFreeMemory( HCSHANDLE hNVOD, PVOID pvMemoryAddr );


/* {{{USR
�����Ƿ�����û�н�Ŀ���ƵĲο��¼�
����
    hNVOD��  NVODģ������
    bIsDisplay��TRUE:   ����
                FALSE:  ��������NVODĬ��Ϊ��״̬��
}}}USR */
BOOL CSUSPNVODIsSearchNoNameRefEvt( HCSHANDLE hNVOD, BOOL bIsSearch );

/* {{{USR
�����Ƿ�����Schedule EIT�����ȷ��ʱ���¼�Ҳȫ��������P/F EIT���У�����Ҫ����Schudele EIT�������������ñ�
����
    hNVOD��  NVODģ������
    bIsDisplay��TRUE:   ����Schedule EIT��NVODĬ��Ϊ��״̬��
                FALSE:  ������Schedule EIT��
}}}USR */
BOOL CSUSPNVODIsSearchScheduleEIT( HCSHANDLE hNVOD, BOOL bIsSearch );

/* {{{USR
��������ʱ���¼��ĳ���ʱ�䣬��������ӦReferenceEvent�еĳ���ʱ�丳ֵ��ʱ���¼��С�
�˽ӿ���ʱ���¼��еĳ���ʱ�䷢�Ͳ���ȷ������µ��ã����õ�ʱ��Ϊ���������֮�󣬻�ȡ����֮ǰ��
����
    hNVOD��  NVODģ������

}}}USR */
BOOL CSUSPNVODResetTimeShiftedEvtDuration( HCSHANDLE hNVOD );

/* {{{USR
�õ�NVODʹ���ڴ�����������ֵ����Сֵ��
   }}}USR */
BOOL CSUSPNVODGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR
����DemuxID��
}}}USR */
BOOL CSUSPNVODSetDemuxID(HCSHANDLE hNVOD, DWORD dwDemuxID);

/* {{{USR
����ʹ�õķ������

}}}USR */
BOOL CSUSPNVODSetLevelType( HCSHANDLE hNVOD, NVODLevelType emLevelType );

#ifdef  __cplusplus
}
#endif

#endif /* NVODInterface_H */
