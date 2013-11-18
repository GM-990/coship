#ifndef NVODInterface_H
#define NVODInterface_H

/* {{{USR
NVOD接口
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
	EXT_NVOD_BEGIN,    //数据开始分析前的准备工作 
	EXT_NVOD_ANALYLSE,   //数据处理
	EXT_NVOD_END,       //数据处理完成后，把结果返回给NVOD模块由于保存
	EXT_NVOD_FREEDATA    //删除数据

}NVODExtCallbackType;

typedef enum _NVODExtANALYLSERet
{
    EXT_NVOD_ANALYLSE_NOTPARSE,      //用户未处理
    EXT_NVOD_ANALYLSE_SUCCEED,       //用户处理成功
    EXT_NVOD_ANALYLSE_FAILED        //用户尝试处理，但申请空间时失败
}NVODExtANALYLSERet;

typedef void ( *NVODExt ) ( NVODExtCallbackType enmType, HCSHANDLE* pParam );
//end NVOD Ext interface


typedef struct _ReferenceService
{
	//原始网络ID
	WORD m_wOrigianlNetworkID;

	//传输流ID
	WORD m_wTsID;

	//该Reference Service的Service ID
	WORD m_wServiceID;

	//该Reference Service的service name
	BYTE m_pServiceName[ NVOD_SERVICE_NAME ];

	//编码方式
	BYTE m_ucLanguage;
}ReferenceService;

typedef struct _TimeShiftedService
{
	//原始网络ID
	WORD m_wOriginalNetworkID;

	//传输流ID
	WORD m_wTsID;

	//该Service的Service ID
	WORD m_wServiceID;

	WORD m_wRefServiceID;
}TimeShiftedService;

typedef struct _ReferenceEvent
{
	//父母指导级别
	BYTE m_ucParental;

	//节目分类1
	BYTE m_ucContentLevel2;

	//节目分类2
	BYTE m_ucContentLevel1;

	//节目名称
	BYTE m_pEventName[ NVOD_MAX_EVENT_NAME ];

	//节目简介
	BYTE m_pTextDescription[ NVOD_MAX_DESCRIPTION ];

	//Event ID
	WORD m_wEventID;

	//节目的片长
	CSUDIPLUSOSTIME_S m_oDuration;

	WORD m_OriginalNetworkID;

	WORD m_wTsID;

	WORD m_wServiceID;

	//编码方式
	BYTE m_ucLanguage;

	//扩展字段
	HCSHANDLE m_hExtInfo;
}ReferenceEvent;

typedef struct _TimeShiftedEvent
{
	//该时间偏移节目的开始时间
	CSUDIPLUSOSSysTime_S m_oStartTime;

	//该时间偏移节目的event ID
	WORD m_wEventID;

	//该时间偏移节目的结束时间
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
	NVOD_FINISH,                   //表示退出NVOD搜索或搜索完成pParam等于NVOD频点的个数，如果NVOD频点的个数等于0，表示搜索失败，如果大于零表示搜索正常完成。
	NVOD_TMSHFTEVENT_COMPLETE,     //表示某个ReferenceEvent下的TimeShiftEvent搜完，pParam等于ReferenceEvent 索引值的地址（int*）
	NVOD_REFEVENT_COMPLETE,        //表示某ReferenceService下的ReferenceEvent搜完，pParam等于NULL
	NVOD_SERVICE_COMPLETE          //表示某个ReferenceService下的TimeShiftServic搜完，pParam等于ReferenceService索引值的地址

}NVODCallbackType;

typedef enum _NVODLevelType
{
	NVOD_CONTENT_LEVEL,
	NVOD_USER_LEVLE
	
}NVODLevelType;

typedef void( *NVODCallBack ) ( NVODCallbackType enmType, void* pParam );


/* {{{USR
申请NVOD用到的资源，返回NVOD资源句柄，这是使用NVOD模块的第一步。如果参数hSection、hMemoryPool和pCallBack中任意一个无效（如为NULL）则申请资源句柄失败，返回NULL。
   }}}USR */
HCSHANDLE CSUSPInitializeNVOD( HCSHANDLE hSection, HCSHANDLE hMemoryPool, NVODCallBack pCallBack, float nTimeOffset );

/* {{{USR
当NVOD需要退出NVOD模块或不再使用NVOD模块时，调用此函数释放NVOD资源。
该函数首先停止NVOD搜索，然后释放NVOD数据所占资源；最后释放NVOD句柄. 
如果参数hNVOD为NULL或非法指针，返回FALSE;
如果释放资源失败返回FALSE.
   }}}USR */
BOOL CSUSPFreeNVOD( HCSHANDLE hNVOD );

/* {{{USR
删除NVOD数据， 但并不释放资源；
当NVOD正在搜索时，先停止搜索，然后删除数据。
如果参数hNVOD为NULL或非法指针，函数返回FALSE。
   }}}USR */
BOOL CSUSPDeleteAllNVODData( HCSHANDLE hNVOD );

/* {{{USR
当NVOD资源初始化后，则调用此函数启动NVOD数据的搜索。注意：在通过上述接口获取NVOD数据时，要先调用此函数接收NVOD数据。
如果参数hNVOD为NULL或非法指针，立即返回FALSE；
hDeliverList 和 hDMContainer 不能同时为null;
如果 hDeliverList 和 hDMContainer 都为 null 或无效时，收索当前频点下收取NVOD数据
如果 hDeliverList 为 null 或无效时， 从数据库中收取NVOD数据;
如果 hDMContainer 为 null 或无效时， 从频点下收取NVOD数据.
   }}}USR */
BOOL CSUSPSearchNvodInfo( HCSHANDLE hNVOD, HCSHANDLE hDeliverList, int nListCount, HCSHANDLE hDMContainer, HCSHANDLE hSource, BOOL bIsNVODFreq );

/* {{{USR
当需要停止NVOD数据的搜索时，调用此函数停止NVOD搜索。
如果参数hNVOD为NULL或非法指针，返回FALSE，表示停止NVOD搜索失败。 
   }}}USR */
BOOL CSUSPStopNVOD( HCSHANDLE hNVOD );

/* {{{USR
当用户注册的回调函数返回NVOD_SERVICE_COMPLETE时，表示某个频点下或所有的Service搜完，用户可以调用此函数获取当前已经搜索到的 Reference Service 的总数目；如果回调函数没有返回此类型时，用户调用此接口得到的结果可能不完全。
如果参数hNVOD为NULL或非法指针，返回-1，表示获取失败。
   }}}USR */
int CSUSPNVODGetRefSrvcNum( HCSHANDLE hNVOD );

/* {{{USR
当用户注册的回调函数返回NVOD_SERVICE_COMPLETE时，表示某个频点下或所有的Service搜完，用户可以调用此函数获取指定Reference Service的信息， nRefServiceIndex的取值范围应该在Reference Service 的总数目之内。
如果参数hNVOD和pRefService为NULL或非法指针、nRefServiceIndex输入索引不在有效范围之内，则返回FALSE，表示获取失败。 
   }}}USR */
BOOL CSUSPNVODGetRefSrvcInfo( HCSHANDLE hNVOD, int nRefServiceIndex, ReferenceService * pRefService );

/* {{{USR
当用户注册的回调函数返回NVOD_SERVICE_COMPLETE时，表示某个频点下或所有的Service搜完，用户可以调用此函数获取指定Reference Service下TimeShiftedService的个数。如果回调函数没有返回此类型时，用户调用此接口得到的结果可能不完全。nRefServiceIndex 的取值范围应该在Reference Service 的总数目之内。
如果nRefServiceIndex不存在或不在有效范围之内，返回0；
如果参数hNVOD为NULL或非法指针，返回-1，表示获取失败。
   }}}USR */
int CSUSPNVODGetTimeShiftedSrvcNum( HCSHANDLE hNVOD, int nRefServiceIndex );

/* {{{USR
当用户注册的回调函数返回NVOD_SERVICE_COMPLETE时，表示某个频点下或所有的Service搜完，用户可以调用此函数获取指定Reference Service下指定TimeShiftedService的信息。nRefServiceIndex、nTimeShiftedIndex的取值范围应该分别在Reference Service和相应timeshifted serviece 的总数目之内。
如果参数hNVOD、pTimeShiftedService为NULL或非法指针，返回FALSE；
如果nRefServiceIndex和nTimeShiftedIndex的输入索引不存在，则返回FALSE，表示获取失败。
   }}}USR */
BOOL CSUSPNVODGetTimeShiftedSrvcInfo ( HCSHANDLE hNVOD, int nRefServiceIndex, int nTimeShiftedIndex, TimeShiftedService * pTimeShiftedService );

/* {{{USR
当用户注册的回调函数返回NVOD_REFEVENT_COMPLETE时，用户可以调用此函数获取当前已经搜索到的 Reference Event 的总数目，如果回调函数没有返回此类型时，用户调用此接口得到的结果可能不完全。
如果参数hNVOD为NULL或非法指针,则返回-1。
   }}}USR */
int CSUSPNVODGetRefEvtNum( HCSHANDLE hNVOD );

/* {{{USR
当用户注册的回调函数返回NVOD_REFEVENT_COMPLETE时，用户可以调用此函数获取指定Reference Event的信息，nRefEventIndex的取值范围应该在Reference Event的总数目之内。
如果参数hNVOD、pReferenceEvent为NULL或非法指针则返回FALSE，表示获取失败;
如果nRefEventIndex不存在，则返回FALSE，表示获取失败。
   }}}USR */
BOOL CSUSPNVODGetRefEvtInfo( HCSHANDLE hNVOD, int nRefEventIndex, ReferenceEvent * pReferenceEvent );

/* {{{USR
当用户注册的回调函数返回NVOD_REFEVENT_COMPLETE时，用户可以调用此函数获取指定ReferenceService下并且当前已经搜索到的 Reference Event 的总数目，如果回调函数没有返回此类型时，用户调用此接口得到的结果可能不完全。nRefServiceIndex的取值范围应该在Reference Service 的总数目之内。
如果nRefServiceIndex不存在或不在有效范围之内，返回0；
如果参数hNVOD为NULL或非法指针,则返回-1。
   }}}USR */
int CSUSPNVODGetRefEvtNumByRefSrvc( HCSHANDLE hNVOD, int nRefServiceIndex );

/* {{{USR
当用户注册的回调函数返回NVOD_REFEVENT_COMPLETE时，用户可以调用此函数获取指定ReferenceService下指定Reference Event的信息，nRefServiceIndex的取值范围应该在Reference Service 的总数目之内。nRefEventIndex的取值范围应该在Reference Event的总数目之内。
如果参数hNVOD、pReferenceEvent为NULL或非法指针则返回FALSE，表示获取失败;
如果nRefServiceIndex不存在或不在有效范围之内，则返回FALSE，表示获取失败；
如果nRefEventIndex不存在，则返回FALSE，表示获取失败。
   }}}USR */
BOOL CSUSPNVODGetRefEvtInfoByRefSrvc( HCSHANDLE hNVOD, int nRefServiceIndex, int nRefEventIndex, ReferenceEvent * pReferenceEvent );

/* {{{USR
当用户注册的回调函数返回 NVOD_TMSHFTEVENT_COMPLETE 时，用户可以调用此函数获取指定Reference Event下TimeShiftedEvent的数目，如果回调函数没有返回此类型时，用户调用此接口得到的结果可能不完全。nRefEventIndex的取值范围应该在Reference Event 的总数目之内。
如果参数hNVOD为NULL或非法指针，返回-1，表示获取失败。
如果nRefEventIndex输入索引不存在，返回0；
   }}}USR */
int CSUSPNVODGetTimeShiftedEvtNum( HCSHANDLE hNVOD, int nRefEventIndex );

/* {{{USR
当用户注册的回调函数返回 NVOD_TMSHFTEVENT_COMPLETE 时，用户可以调用此函数获取指定 Reference Event 下指定 TimeShiftedEvent 的信息。nRefEventIndex 的取值范围应该在Reference Event 的总数目之内，nTimeSftIndex 的取值范围应该在指定 Reference Event 下的 TimeShiftedEvent 的总数目之内。
如果参数hNVOD、pTimeShiftedEvent为NULL或非法指针， 返回FALSE，表示获取失败；
如果nRefEventIndex和nTimeSftIndex不存在，则返回FALSE，表示获取失败。
   }}}USR */
BOOL CSUSPNVODGetTimeShiftedEvtInfo( HCSHANDLE hNVOD, int nRefEventIndex, int nTimeSftIndex, TimeShiftedEvent * pTimeShiftedEvent );

/* {{{USR
当某个 ReferenceEvent 下的 TimeShiftEvent 搜到后，用户可以调用此函数获取播放该TimeShiftedEvent 节目的所需信息。nRefEventIndex 的取值范围应该在 Reference Event 的总数目之内，nTimeSftIndex 的取值范围应该在指定 Reference Event 下的 TimeShiftedEvent 的总数目之内。 
如果参数hNVOD为NULL或非法指针，参数pwOrgNetworkID、pwTsID和pwServiceID为NULL，则返回FALSE，表示获取失败；
如果nRefEventIndex 和 nTimeSftIndex 不存在，则返回FALSE，表示获取失败。
   }}}USR */
BOOL CSUSPNVODGetTimeShiftedPlayInfo( HCSHANDLE hNVOD, int nRefEventIndex, int nTimeSftIndex, WORD * pwNetworkID, WORD * pwTsID, WORD * pwServiceID );

/* {{{USR
当某个ReferenceEvent下的TimeShiftEvent搜完后，用户可以调用此函数获取指定Reference Event的最近已开始和即将开始的TimeShiftedEvent信息和索引值。nRefEventIndex 的取值范围应该在 Reference Event 的总数目之内。
如果参数hNVOD为NULL或非法指针，或nRefEventIndex的不存在，则返回FALSE， 表示获取失败；
如果参数pRecentIndex、pRecent、pFutureIndex和pFuture为NULL，则返回FALSE。
   }}}USR */
BOOL CSUSPNVODGetRcntFutTimeShiftedEvt ( HCSHANDLE hNVOD, int nRefEventIndex, int * pRecentIndex, TimeShiftedEvent * pRecent, int * pFutureIndex, TimeShiftedEvent * pFuture );

/* {{{USR
当用户注册的回调函数返回 NVOD_TMSHFTEVENT_COMPLETE 时，用户可以调用此函数获取指定Reference Event下某个时间段（日期和时间）的TimeShiftedEvent的起始句柄，nRefEventIndex的取值范围应该在Reference Event 的总数目之内。
如果参数hNVOD为NULL或非法指针,或nRefEventIndex不存在，则返回NULL，获取失败;
如果参数pStartTSYSTEMTIME或pEndTSYSTEMTIME为NULL，则返回NULL，获取失败。
   }}}USR */
HCSHANDLE CSUSPNVODGetDateTimeEvtHandle( HCSHANDLE hNVOD, int nRefEventIndex, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );

/* {{{USR
当用户注册的回调函数返回 NVOD_TMSHFTEVENT_COMPLETE 时，用户可以调用此函数获取指定Reference Event下某个时间段（日期和时间）的TimeShiftedEvent的数目，如果回调函数没有返回此类型时，用户调用此接口得到的结果可能不完全。nRefEventIndex的取值范围应该在Reference Event 的总数目之内。
如果参数hNVOD为NULL或非法指针，参数pStartTSYSTEMTIME和pEndTSYSTEMTIME为NULL，则返回-1，表示获取失败;
如果nRefEventIndex不存在，返回0。
   }}}USR */
int CSUSPNVODGetDateTimeEvtNum( HCSHANDLE hNVOD, int nRefEventIndex, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );

/* {{{USR
当用户注册的回调函数返回 NVOD_TMSHFTEVENT_COMPLETE 时，用户可以调用此函数获取指定Reference Event下某个时间段（日期和时间）的指定TimeShiftedEvent的信息，注意：参数hGetData应该从接口CSUSPNVODGetDateTimeEvtHandle获得，参数nTimeSftIndex的范围应该从接口CSUSPNVODGetDateTimeEvtNum获得。
如果参数hNVOD为NULL或非法指针，nTimeSftIndex的输入索引不存在，参数hGetData和pTimeShiftedEvent为NULL，则返回FALSE，表示获取失败。
   }}}USR */
BOOL CSUSPNVODGetDateTimeEvtInfo( HCSHANDLE hNVOD, HCSHANDLE hGetData, int nTimeSftIndex, TimeShiftedEvent * pTimeShiftedEvent );

/* {{{USR
当用户注册的回调函数返回NVOD_REFEVENT_COMPLETE时，用户可以调用此函数获取当前已经搜索到的并且是指定节目类型的Reference Event总数目。如果回调函数没有返回此类型时，用户调用此接口得到的结果可能不完全。
如果参数hNVOD为NULL或非法指针，参数nContentType1和nContentType2值的范围不符合标准，则返回-1，表示获取失败。
   }}}USR */
int CSUSPNVODGetTypeRefEvtNum( HCSHANDLE hNVOD, int nContentLevel1, int nContentLevel2 );

/* {{{USR
当用户注册的回调函数返回NVOD_REFEVENT_COMPLETE时，用户可以调用此函数获取当前已经搜索到的并且是指定节目类型的指定Reference Event信息。参数nRefEventIndex值的范围应该从接口CSUSPNVODGetTypeRefEvtNum获得。
如果参数hNVOD为NULL或非法指针，nTimeSftIndex的输入索引不存在，参数nContentType1和nContentType2值的范围不符合标准，参数pReferenceEvent为NULL，则返回FALSE，表示获取失败。
   }}}USR */
BOOL CSUSPNVODGetTypeRefEvtInfo( HCSHANDLE hNVOD, int nRefEventIndex, int nContentLevel1, int nContentLevel2, ReferenceEvent * pReferenceEvent );

/* {{{USR
当用户注册的回调函数返回NVOD_FINISH时，用户可以调用此函数获取当前已经搜索到的NVOD频点的个数。
如果参数hNVOD为NULL或非法指针，则返回-1，表示获取失败。
return : Int, 返回频点的数目，返回-1表示获取失败，0表示没有频点信息。
   }}}USR */
int CSUSPNVODGetFreqDataNum( HCSHANDLE hNVOD );

/* {{{USR
当用户注册的回调函数返回NVOD_FINISH时，用户可以调用此函数获取当前已经搜索到指定索引的NVOD频点信息。如果参数hNVOD为NULL或非法指针，nFreqIndex的取值范围不合理，则返回FALSE，表示获取频点失败。 
return: BOOL TRUE表示获取NVOD频点成功，FALSE表示获取NVOD频点失败
   }}}USR */
BOOL CSUSPNVODGetFreqData( HCSHANDLE hNVOD, int nFreqIndex, NVODDeliver * pNVODDeliver );

/* {{{USR
当NVOD资源初始化后，则调用此函数启动NVOD数据的搜索。注意：在通过上述接口获取NVOD数据时，要先调用此函数接收NVOD数据。
如果参数hNVOD为NULL或非法指针，立即返回FALSE；
hDeliverList 和 hDMContainer 不能同时为null;
如果 hDeliverList 和 hDMContainer 都为 null 或无效时，收索当前频点下收取NVOD数据
如果 hDeliverList 为 null 或无效时， 从数据库中收取NVOD数据;
如果 hDMContainer 为 null 或无效时， 从频点下收取NVOD数据.
   }}}USR */
BOOL CSUSPSearchNvodInfobyOther( HCSHANDLE hNVOD, HCSHANDLE hDeliverList, int nListCount, HCSHANDLE hDMContainer, HCSHANDLE hSource, BOOL bIsNVODFreq, BOOL bIsFindOther );

/* {{{USR
改变NVOD模块中的时区偏移值。
   }}}USR */
BOOL CSUSPNVODChangeTimeZone( HCSHANDLE hNVOD, float fTimeOffset );

/* {{{USR
CSUSPNVODRegisterUserListener
   当用户需要使用没有包含在NVOD基本结构的数据时，调用此函数把用户自定义的处理扩展数据的类对象注册到NVOD模块中，
   NVOD模块把包含扩展数据的描述符传给此对象，以便让用户得到想要的数据。
   如果参数hNVOD、hExtProcess为NULL或非法指针则返回FALSE，表示注册失败;
   }}}USR */
BOOL CSUSPNVODRegisterUserListener( HCSHANDLE hNVOD, HCSHANDLE hExtProcess );

/* {{{USR
设置只存储某一时间段的TimeshiftEvent数据
   }}}USR */
BOOL CSUSPNVODSetEventDuration( HCSHANDLE hNVOD, CSUDIPLUSOSSysTime_S * pStartTime, CSUDIPLUSOSSysTime_S * pEndTime );

/* {{{USR
设置关于NVOD搜索的相关参数。例如是否强制指定不依赖于数据管理搜索。通知NVOD模块码流中NIT表是否有Servcie_List_Descriptor
描述符。此接口需要在NVOD搜索前调用。
 参数：
 bSearchFlag： 等于TRUE, 表示强制指定不依赖于数据管理搜索。
               等于FALSE, 表示强制指定依赖于数据管理搜索。
 bIsSearchList：等于TRUE, 表示在NIT表中存在Servcie_List_Descriptor描述符。
                等于FALSE, 表示在NIT表中不存在Servcie_List_Descriptor描述符。
                默认为TRUE。
   }}}USR */
BOOL CSUSPNVODSetSearchFlag( HCSHANDLE hNVOD, BOOL bSearchFlag, BOOL bIsSearchList );

/* {{{USR
设置码流中EIT表发送的特征。此接口需要在NVOD搜索前调用。
 参数：
   bIsScheduleToSimple：
           如果为TRUE,表示前端把TimeShiftEvent数据在TableID 等于0x4e或0x4f的表中发送。
           如果为FALSE,表示前端把TimeShiftEvent数据在TableID 等于0x50-0x5f或0x60-0x6f的表中发送。
           NVOD模块中默认为FALSE。
   }}}USR */
BOOL CSUSPNVODSetStreamType( HCSHANDLE hNVOD, BOOL bIsScheduleToSimple, void * nParam );

/* {{{USR
设置NVOD搜索的超时时间。由于NVOD模块对用户传的时间做了处理，所以真正的超时时间不一定和用户所设置的时间一致。
 参数：
   nTimeOut： 搜索的超时时间，以毫秒为单位
   }}}USR */
BOOL CSUSPNVODSetTimeOut( HCSHANDLE hNVOD, int nTimeOut, void * nParam );

/* {{{USR
设置不锁频的NVOD搜索。使用此种方式的原因：由于进入NVOD界面马上需要播放节目，也需要NVOD搜索，但是如果这两种情况锁的频点不同，
就会造成问题，所以提供这种功能在播放的当前频点搜索NVOD数据，这种功能也可以作为实时监视数据的方案（在NVOD界面中利用Timer持续调用搜索）。
   }}}USR */
BOOL CSUSPNVODSetIsLockFreq( HCSHANDLE hNVOD, BOOL bIsLock );

/* {{{USR

   }}}USR */
BOOL CSUSPNVODSetFreqData( HCSHANDLE hNVOD, DWORD dwFreqency, WORD wOriNetworkID, WORD wTSID, void * pParam );

/* {{{USR
设置NVOD搜索线程的优先级和堆栈大小。默认的优先级是17*7， 线程堆栈大小是1024*10 B.
   }}}USR */
BOOL CSUSPNVODSetThreadParam( HCSHANDLE hNVOD, DWORD dwPrioriy, DWORD dwStackCount, void * pParam );

/* {{{USR
设置是否把没有时移事件的参考事件返回给用户。
 参数：
  bIsDisplay：为TRUE，表示把没有时移事件的参考事件返回给用户。
             为FALSE，表示不把没有时移事件的参考事件返回给用户。
   }}}USR */
BOOL CSUSPNVODSetIsDisplayBadRefEvt( HCSHANDLE hNVOD, BOOL bIsDisplay );

/* {{{USR
设置在搜索完成并且搜全时，是否删除前次搜到而本次没有搜到的数据。
 参数
  bIsDeleteData：为TRUE，表示删除老的数据。
             为FALSE，表示不删除老的数据。
   }}}USR */
BOOL CSUSPNVODIsDeleteOldData( HCSHANDLE hNVOD, BOOL bIsDeleteData );

/* {{{USR
删除指定时间点以前的所有时移事件（TimeShiftEvent）。当某个RefEvent的所有时移事件
播放完毕后，需要在界面上不显示此RefEvent，用户可以调用此接口删除过期时移事件，此
时获取RefEvent数据时就不会得到RefEvent。
 参数
    hNVOD：  NVOD模块句炳。
    pCurSYSTEMTIME：用户指定的时间点。
   }}}USR */
BOOL CSUSPNVODDeleteOldTimeShtEvent( HCSHANDLE hNVOD, CSUDIPLUSOSSysTime_S * pCurSYSTEMTIME );

/* {{{USR
当NOVD模块向数据管理添加节目时，需要先得到频点信息，此接口用于指定频点的关键字。关键字
有两种选择：（1）OriginalNetworkID和TSID (2)频率。
 参数 
    hNVOD：  NVOD模块句炳。
    bIsFreqency ：等于TRUE,表示以频率为关键字；等于FALSE,表示以 
                OriginalNetworkID和TSID为关键字。
   }}}USR */
BOOL CSUSPNVODSetUniqueFreqFlag( HCSHANDLE hNVOD, BOOL bIsFreqency );

/* {{{USR
把搜索到的ReferenceEvent按照他的第一个TimeShiftedEvent的starttime的先后进行排序
   }}}USR */
BOOL CSUSPNVODRefEventSortByStartTime( HCSHANDLE hNVOD );

/* {{{USR
设置是否需要NIT表搜索到的Service的功能
   }}}USR */
BOOL CSUSPNVODIsNeedServiceByNIT( HCSHANDLE hNVOD, BOOL IsNeedSerByNIT );

/* {{{USR
设置是否添加判断Section搜索完全的功能
   }}}USR */
BOOL CSUSPNVODIsJudgeSectionComplete( HCSHANDLE hNVOD, BOOL IsNeedJudge );

/* {{{USR
把搜索到的ReferenceService 进行排序，排序索引为其ServiceID,排序方式为升序。
}}}USR */
BOOL CSUSPNVODRefServiceSortByServiceID( HCSHANDLE hNVOD );

/* {{{USR
设置是否显示没有对应ReferenceEvent的ReferenceService
参数
hNVOD：  NVOD模块句炳。
bIsDisplay：TRUE:显示
            FALSE：不显示（过滤掉没有对应ReferenceEvent的ReferenceService）
}}}USR */
BOOL CSUSPNVODSetIsDisplayBadRefSrvc(HCSHANDLE hNVOD, BOOL bIsDisplay);

/* {{{USR
得到NVOD模块的版本号信息。
   }}}USR */
int CSUSPNVODGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
用户设定搜索EIT表时的PID
参数
    hNVOD：  NVOD模块句炳。
    dwPID：  EIT表的PID，默认为0x0012;
}}}USR */
BOOL CSUSPNVODSetPID( HCSHANDLE hNVOD, DWORD dwPID );
/* {{{USR
向NVOD模块内部申请空间以保存相关信息：如扩展描述符的信息。
CSUSPNVODMallocMemory

Parameter:  
    [in] hNVOD    : 资源句柄
    [in] nCount ：表示需申请的空间大小。 

RETURN VALUE  
    NULL表示申请失败
    非NULL 表示申请成功。
}}}USR */
PBYTE CSUSPNVODMallocMemory( HCSHANDLE hNVOD, int nCount );

/* {{{USR
释放向NVOD模块内部申请的空间：如扩展描述符的信息。
CSUSPNVODFreeMemory

Parameter:  
    [in] hNVOD    : 资源句柄
    [in] pvMemoryAddr ：内存指针。 

Return Values
    1）如果成功返回TRUE
    2）如果失败返回FALSE
}}}USR */
BOOL CSUSPNVODFreeMemory( HCSHANDLE hNVOD, PVOID pvMemoryAddr );


/* {{{USR
设置是否搜索没有节目名称的参考事件
参数
    hNVOD：  NVOD模块句炳。
    bIsDisplay：TRUE:   搜索
                FALSE:  不搜索（NVOD默认为该状态）
}}}USR */
BOOL CSUSPNVODIsSearchNoNameRefEvt( HCSHANDLE hNVOD, BOOL bIsSearch );

/* {{{USR
设置是否搜索Schedule EIT表，如果确定时移事件也全部发送在P/F EIT表中，则不需要搜索Schudele EIT表，否则请搜索该表
参数
    hNVOD：  NVOD模块句炳。
    bIsDisplay：TRUE:   搜索Schedule EIT表（NVOD默认为该状态）
                FALSE:  不搜索Schedule EIT表
}}}USR */
BOOL CSUSPNVODIsSearchScheduleEIT( HCSHANDLE hNVOD, BOOL bIsSearch );

/* {{{USR
重新设置时移事件的持续时间，即：将对应ReferenceEvent中的持续时间赋值到时移事件中。
此接口在时移事件中的持续时间发送不正确的情况下调用，调用的时机为：搜索完成之后，获取数据之前。
参数
    hNVOD：  NVOD模块句炳。

}}}USR */
BOOL CSUSPNVODResetTimeShiftedEvtDuration( HCSHANDLE hNVOD );

/* {{{USR
得到NVOD使用内存的数量的最大值和最小值。
   }}}USR */
BOOL CSUSPNVODGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR
设置DemuxID。
}}}USR */
BOOL CSUSPNVODSetDemuxID(HCSHANDLE hNVOD, DWORD dwDemuxID);

/* {{{USR
设置使用的分类参数

}}}USR */
BOOL CSUSPNVODSetLevelType( HCSHANDLE hNVOD, NVODLevelType emLevelType );

#ifdef  __cplusplus
}
#endif

#endif /* NVODInterface_H */
