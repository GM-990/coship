#ifndef EPGReceiverInterface_H
#define EPGReceiverInterface_H

#include "mmcp_typedef.h"
#include "mmcp_debug.h"

#ifdef  __cplusplus
extern "C" {
#endif

//搜索策略
#define	EPG_SERVICE_DETECTIVE			0x08      //设置自动监视当前Sercice
#define	EPG_SEARCH_ORINETWORK			0x04      //Network级搜索
#define	EPG_SEARCH_TS					0x02      //TS级搜索
#define	EPG_SEARCH_SERVICE				0x01      //Service级搜索

//存储策略
#define	EPG_SAVE_ALL					0x80      //存储所有数据
#define	EPG_SAVE_ORINETWORK				0x40      //存储当前Network数据
#define	EPG_SAVE_TS						0x20      //存储当前TS数据
#define	EPG_SAVE_SERVICE				0x10      //存储当前Service数据

//注册回调函数的类型（REG）
//当前流的PresentEPG搜到
#define EPG_REG_CURSERVICE_SIMPLE_PRESENT_FINISHED	(0x00000001)
//当前流的FollowingEPG搜到
#define	EPG_REG_CURSERVICE_SIMPLE_FOLLOW_FINISHED	(0x00000002)
//当前流的PresentEPG改变
#define	EPG_REG_CURSERVICE_SIMPLE_PRESENT_CHANGED	(0x00000004)
//当前流的FollowingEPG改变
#define	EPG_REG_CURSERVICE_SIMPLE_FOLLOW_CHANGED	(0x00000008)
//请求的P/F EPG搜全
#define	EPG_REG_ALLSERVICE_SIMPLE_FINISHED			(0x00000010)
//PG信息	
#define	EPG_REG_CURSERVICE_SIMPLE_PRESENT_PGCHANGED	(0x00000020)
//一个Service的ScheduleEPG搜全
#define	EPG_REG_CURSERVICE_SCHEDLE_FINISHED			(0x00000040)
//请求的Schedule EPG搜全
#define	EPG_REG_ALLSERVICE_SCHEDLE_FINISHED			(0x00000080)
#define	EPG_REG_CUREVENT_ALLDATA					(0x00000100)
#define	EPG_REG_CUREVENT_FREEPRIVATEDATA			(0x00000200)
//内存不足
#define	EPG_REG_MEMORY_FULL							(0x00000400)
//搜到一个Schedule EPG的Section
#define	EPG_REG_EIT_REC 							(0x00000800)

#define EPG_REG_SCHEDULE_CONTENTTYPE                (0x00001000)
#define EPG_REG_SCHEDULE_EIT_VERSION_CHANGED 	(0x00002000)

//wangshazhou 2006.4.9
//删除过期EPG
#define EPG_REG_DELETE_EPGHANDLE                    (0x00002000)
//P/F EPG因超时而停止搜索的回调
#define EPG_REG_SIMPLE_TIMEOUT                      (0x00004000)
//Schedule EPG因超时而停止搜索的回调
#define EPG_REG_SCHEDULE_TIMEOUT                    (0x00008000)

//Simple EPG分类回调
#define EPG_REG_SIMPLE_CONTENTTYPE                  (0x00010000)

//设置Event的数据类型
//设置搜索节目简介
#define	EPG_EVENT_DESDATA							(0x00000001)
//设置搜索用户私有数据
#define	EPG_EVENT_PRIVATEDATA						(0x00000002)

//设置只存储当前Service的节目简介
#define	EPG_NEEDSERVICE_DEC							(0x00000004)
//设置只存储当前TS的节目简介
#define	EPG_NEEDTS_DEC							(0x00000008)

//设置保存描述符
#define EPG_NEEDSAVE_PRIVATEDATA						(0x00000010)

#define EPG_DEC_SHORT        0x01  //从short 描述符中得简介
#define EPG_DEC_EXTEND       0x02  //从extend 描述符中得简介

#define EPG_TIMEOUT_BY_REPEAT   0x10   //判断重复包的超时
#define EPG_TIMEOUT_DIRECT      0x20   //直接超时

//回调类型（与上面的“注册回调函数的类型”相对应）
typedef enum _EPGCallbackType
{
	EPG_CURSERVICE_SIMPLE_PRESENT_FINISHED,
	EPG_CURSERVICE_SIMPLE_FOLLOW_FINISHED,
	EPG_CURSERVICE_SIMPLE_PRESENT_CHANGED,   //EPGUserParam
	EPG_CURSERVICE_SIMPLE_FOLLOW_CHANGED,	 //EPGUserParam
	EPG_ALLSERVICE_SIMPLE_FINISHED,  //NULL
	
	EPG_CURSERVICE_SIMPLE_PRESENT_PGCHANGED,     //EPGPGParam

	EPG_CURSERVICE_SCHEDLE_FINISHED,//EPGUserParam
	EPG_ALLSERVICE_SCHEDLE_FINISHED,//NULL

	EPG_ANALYSE_PRIVATEDATA,//EPGEventData
	EPG_FREE_PRIVATEDATA,

	EPG_MEMORY_FULL,

	EPG_EIT_REC,

	EPG_SCHEDULE_CONTENTTYPE,

	EPG_DELETE_EPGHANDLE,
	EPG_SIMPLE_TIMEOUT,
	EPG_SCHEDULE_TIMEOUT,

	EPG_GET_PRIVATEDATA,

	EPG_SIMPLE_CONTENTTYPE,
	EPG_SCHEDULE_EIT_VERSION_CHANGED
}EPGCallbackType;

typedef struct _ServiceSourceID
{
	WORD m_wOriginalNetworkID;
	WORD m_wTsID;
	WORD m_wServiceID;
}ServiceSourceID;

typedef struct _PGParam
{
	ServiceSourceID m_ServiceSourceID;

	int m_nPGRate;
}EPGPGParam;

typedef struct _EPGUserParam
{
	ServiceSourceID m_ServiceSourceID;

	void* m_pUserData;
}EPGUserParam;

typedef struct _EPGEventData
{
	PBYTE m_pEventData;
	int	  m_nEnentDataLen;

	HCSHANDLE m_hUserData;
}EPGEventData;

typedef struct _EPGContentTypeData
{
	WORD m_wOriginalNetworkID;
	WORD m_wTsID;
	WORD m_wServiceID;
	BYTE m_ucContentLevel1;
	BYTE m_ucContentLevel2;
	DWORD m_dwUserData;
}EPGContentTypeData;

//用户声明的回调函数的指针
//enmType 等于 SERVICE_SUCCESS时 ：表示一个节目搜索完成，pParam为ServiceParam类型的结构体，包括NetworkID，TSID和ServiceID。
//        等于 PG_INFO时 ：表示当前Event包含PG信息，pParam为PG信息级别和所在的NetworkID,TSID,ServiceID值。
//                         当一个频点搜完，调用此函数。
typedef void*(*EPGCallBack)(EPGCallbackType enmType, void* pParam);

/* {{{USR
CreateEPG
  创建EPG资源句柄
HCSHANDLE CreateEPG( HCSHANDLE hSection, HCSHANDLE hMemoryPool, EPGCallBack pCallBack, DWORD dwRegCallbackType, DWORD dwEventSimpleDataType, DWORD dwEventScheduleDataType );

Parameters：
  hSection
     [in]启动section模块的句柄
  hMemoryPool
     [in]MemoryPool模块的内存池句柄
  dwRegCallbackType 
     [in]用户注册的回调函数
  dwRegCallbackType 
     [in]此参数用于注册用户需要的回调函数类型，此参数由接口定义的宏指定。
  dwEventSimpleDataType 
     [in]表示是否获取Present 和Following EPG的节目描述信息，此参数由接口定义的宏指定。
  dwEventScheduleDataType 
     [in] 表示是否获取Schedule EPG的节目描述信息，此参数由接口定义的宏指定。

Return Values
  返回EPG资源句柄，如果返回NULL，表示创建失败
   }}}USR */
HCSHANDLE CreateEPG( HCSHANDLE hSection, HCSHANDLE hMemoryPool, EPGCallBack pCallBack, DWORD dwRegCallbackType, DWORD dwEventSimpleDataType, DWORD dwEventScheduleDataType );

/* {{{USR
FreeEPG
  释放EPG资源句柄
  BOOL FreeEPG( HCSHANDLE hEPG );

Parameters：
  hEPG
   [in]EPG资源句柄 

Return Values
  BOOL TRUE表示成功
       FALSE表示失败
   }}}USR */
BOOL FreeEPG( HCSHANDLE hEPG );

/* {{{USR
StartRecEPG
  启动指定搜索策略和存储策略的Present/Following EPG 和 Schedule EPG 信息接收,
  接收策略和存储策略由函数SetEPGRecPolicy 和 SetEPGSavePolicy分别指定。
StartRecEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID );
    Parameters：
      hEPG
        [in]EPG资源句柄 
      wOriginalNetworkID
        [in]指定Network的ID值
      wTsID
        [in]指定TS的ID值  
      wServiceID
        [in]指定Service的ID值

    Return Values
        BOOL TRUE表示成功
        FALSE表示失败

   }}}USR */
BOOL StartRecEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID );

/* {{{USR
BOOL StartRecEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID );
    启动指定搜索策略和存储策略的Present/Following EPG 和 Schedule EPG 信息接收，
    接收策略和存储策略由函数SetEPGRecPolicy 和 SetEPGSavePolicy分别指定。此接口与StartRecEPG的区别在于多了一个dwUserOriginalNetworkID参数。给网络添加了一个标示符，用于处理在多网络条件下，wOriginalNetworkID不准确的情况。
  
  Parameter:  
    hEPG
      [in]EPG资源句柄
    wOriginalNetworkID
      [in]指定Network的ID值
    wTsID
      [in]指定TS的ID值
    wServiceID
      [in]指定Service的ID值
    dwUserOriginalNetworkID
      [in]用户指定的网络的标示符

  RETURN VALUE  
      BOOL TRUE表示启动接收成功
      FALSE表示启动接收失败。

   }}}USR */
BOOL StartRecEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID );

/* {{{USR

   }}}USR */
BOOL StartRecEPGWithUserData( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, void * pUserData );

/* {{{USR
设置EPG的搜索策略
   参数：
     nSimpleEPG：设置P/F EPG的搜索策略
	 nSckeduleEPG：设置Schedule EPG的搜索策略
   }}}USR */
BOOL SetEPGRecPolicy( HCSHANDLE hEPG, int nSimpleEPG, int nSckeduleEPG );

/* {{{USR
设置EPG的存储策略
   参数：
     nSimpleEPG：设置P/F EPG的存储策略
	 nSckeduleEPG：设置Schedule EPG的存储策略
   }}}USR */
BOOL SetEPGSavePolicy( HCSHANDLE hEPG, int nSimpleEPG, int nScheduleEPG );

/* {{{USR
StopRecEPG
 停止所有EPG信息的接收
 BOOL StopRecEPG( HCSHANDLE hEPG );

Parameters：
  hEPG
   [in]EPG资源句柄

Return Values
  BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL StopRecEPG( HCSHANDLE hEPG );

/* {{{USR
得到EPG模块的版本号信息。
   }}}USR */
int CSUSPEPGGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
得到EPG使用内存数量的最大值和最小值。
   }}}USR */
BOOL CSUSPEPGGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR
当需要搜索EPG节目简介时，设置节目简介的类型
  参数：
   nEPGDecDataType：节目简介的类型，使用宏EPG_DEC_SHORT，EPG_DEC_EXTEND。
                    如果两种类型都要，那么把两个宏或操作
   }}}USR */
BOOL SetEPGDecDataType( HCSHANDLE hEPG, int nEPGDecDataType );

/* {{{USR
设置EPG搜索用的DumaxID值
   }}}USR */
BOOL SetEPGDemaxID( HCSHANDLE hEPG, int nDemaxID );

/* {{{USR
由于在启动EPG的搜索时，当传入的参数与前一次传入的参数相同时（依赖于搜索级别），
那么这次不会再次启动EPG的搜索，这时模块对输入的参数做了对比检查。
但是有时用户不需要此项功能，用户希望每次启动EPG搜索时都能启动EPG搜索，
这时就可以调用此接口实现此功能。
  参数：
   bSimpleCheck：表示是否设置SimpleEPG的参数检查
   bScheduleCheck：表示是否设置Schedule EPG的参数检查
   }}}USR */
BOOL SetEPGCheckParam( HCSHANDLE hEPG, BOOL bSimpleCheck, BOOL bScheduleCheck );

/* {{{USR
  启动Service级别的EPG搜索。不管用户设置什么级别的搜索，调用此接口只执行Service
级的搜索。接口的bSimpleOnlyServ参数指定Simple EPG是否强制执行Service级搜索，
参数bScheduleOnlyServ指定Schedule EPG是否强制执行Service级搜索。
   }}}USR */
BOOL StartRecEPGbyService( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, void * pUserData, BOOL bSimpleOnlyServ, BOOL bScheduleOnlyServ );

/* {{{USR
设置EPG的超时时间和是否启动“判断搜索完成”的功能。如果不设置超时（设置为-1），
那么表示永久超时。
 参数：
  nSimpleTime： Simple EPG的超时时间（毫秒）
  bIsSimpleAll：等于True表示对Simple EPG启动“判断搜索完成”的功能，
                  等于False表示不启动。
  nScheduleTime：Schedule EPG的超时时间（毫秒）
  bIsScheduleAll：等于True表示对Schedule EPG启动“判断搜索完成”的功能，
                 等于False表示不启动。
   }}}USR */
BOOL SetEPGTimeOut( HCSHANDLE hEPG, int nSimpleTime, BOOL bIsSimpleAll, int nScheduleTime, BOOL bIsScheduleAll );

/* {{{USR
设置是否删除不正确的EPG数据，例如一个Service的EPG时间是12：00——13：00，当又搜索到一个
EPG的时间是11：00——14：00或者12：10——12：40，那么就认为搜到的数据是错的，不加入到EPG数据库中。
 参数：
  nIsDelBadData： 为TRUE,表示删除不正确的EPG数据
                  为FALSE,表示不删除不正确的EPG数据

return BOOL
   }}}USR */
BOOL CSUSPEPGSetDelBadData( HCSHANDLE hEPG, int nIsDelBadData );

/* {{{USR
设置DemuxID
   }}}USR */
BOOL SetEPGDemuxID( HCSHANDLE hEPG, int nDemuxID );

/* {{{USR
设置超时类型，可以选择直接超时或者判断重复包超时(即有重复包时重新计时)
   }}}USR */
BOOL EPGSetTimeoutType( HCSHANDLE hEPG, int nTimeoutType );

/* {{{USR
设置在EPG搜索时是否以SectionNum来判断一个段是否重复。此接口是为了兼容一些错误码流，
例如同一个Servcie的相同TableID和SectionNum的段可能有两个。
   }}}USR */
BOOL CSUSPEPGIsNeedSecNumWithSearch( HCSHANDLE hEPG, BOOL bIsNeedSecNum );

/* {{{USR
设置在EPG搜索中，是否需要更新已经存在的EPG数据。
   }}}USR */
BOOL CSUSPEPGIsNeedUpdateExistEPG( HCSHANDLE hEPG, BOOL bIsSimpleEPGUpdate, BOOL bIsScheduleEPGUpdate );

/* {{{USR
向EPG模块内部申请空间以保存相关信息：如扩展描述符的信息。
CSUSPEPGMallocMemory

Parameter:  
    [in] EPG    : 资源句柄
    [in] nCount ：表示需申请的空间大小。 

RETURN VALUE  
    NULL表示申请失败，非NULL 表示申请成功。
}}}USR */
PVOID CSUSPEPGMallocMemory( HCSHANDLE hEPG, int nCount );

/* {{{USR
释放先前向EPG模块内部申请空间,如扩展描述符的信息。
CSUSPEPGFreeMemory

Parameter:  
    [in] hEPG         : EPG资源句柄
    [in] pvMemoryAddr ：内存指针。 
RETURN VALUE  
    1）如果成功返回TRUE
    2）如果失败返回FALSE
}}}USR */
BOOL CSUSPEPGFreeMemory( HCSHANDLE hEPG, PVOID pvMemoryAddr );

/* {{{USR
设置是否对EIT的版本号的变化进行忽略，请在EPG模块初始化后，启动EPG搜索前调用该函数
CSUSPEPGSetIsIgnoreEitVersionChange
Parameter:  
	[in] hEPG			: EPG资源句柄
	[in] bSimple		：是否忽略P/F EIT表版本号的变化。 （TRUE：表示忽略，FALSE：表示不忽略）
	[in] bSchedule		：是否忽略Schedule EIT表版本号的变化。（TRUE：表示忽略，FALSE：表示不忽略） 
RETURN VALUE  
	1）如果成功返回TRUE
	2）如果失败返回FALSE

}}}USR */
BOOL CSUSPEPGSetIsIgnoreEitVersionChange( HCSHANDLE hEPG, BOOL bSimple, BOOL bSchedule );


/* {{{USR
设置解析“家长分级描述符”时的国家代码，请在EPG模块初始化后，启动EPG搜索前调用该函数
CSUSPEPGSetCountryCode
Parameter:  
	[in] hEPG				: EPG资源句柄
	[in] dwCountryCode		：国家代码（英国由3字符代码“GBR”表示，编码为：“0100 0111 0100 0010 0101 0010”）

	
Remark：
国家代码  country_code：
	24位字段，按照ISO 3166用3字符代码指明国家。
	每个字符根据GB/T 15273.1-1994编码为8位，并依次插入24位字段。
	假设3个字符代表了一个900至999的数字，那么country_code指定了一组ETSI定义的国家。其分配见ETR 162
	例如：英国由3字符代码“GBR”表示，编码为：“0100 0111 0100 0010 0101 0010”。

RETURN VALUE  
	1）设置成功返回TRUE
	2）设置失败返回FALSE
}}}USR */
BOOL CSUSPEPGSetCountryCode( HCSHANDLE hEPG, DWORD dwCountryCode );

BOOL CSUSPEPGSetTunerDemuxPath( HCSHANDLE hEPG, HCSHANDLE hRCPath );

typedef struct _EPGTagNeedSaveInfo
{
	int m_nTagIDCount;              //Tag ID实际个数
	BYTE m_ucTagID[8];  		    //要保存的descriptor的TagID
} EPGTagNeedSaveInfo;

/* 
设置需要保存的描述符信息
CSUSPEPGSetNeedSaveTagInfo
Parameter:  
	[in] hEPG			: EPG资源句柄
	[in] psSimpleTagInfo		：要保存的simple EPG的描述符信息
	[in] psScheduleTagInfo		：要保存的schedule EPG的描述符信息
RETURN VALUE  
	1）如果成功返回TRUE
	2）如果失败返回FALSE
Remark:
	此接口需与EPG_NEEDSAVE_PRIVATEDATA选项一起使用，
	即实现保存描述符的功能需要设置EPG_NEEDSAVE_PRIVATEDATA选项，
	并在EPG初始化之后，	开始搜索EPG信息之前调用CSUSPEPGSetNeedSaveTagInfo
*/
BOOL CSUSPEPGSetNeedSaveTagInfo(HCSHANDLE hEPG, EPGTagNeedSaveInfo * psSimpleTagInfo, EPGTagNeedSaveInfo * psScheduleTagInfo);

/* {{{USR
设置Schedule EIT 版本号发生变化时是否删除原有数据，请在EPG模块初始化后，启动EPG搜索前调用该函数
CSUSPEPGSetIsDeleteOldDataWhenEitVerChange
Parameter:  
	[in] hEPG			: EPG资源句柄
	[in] bSchedule	: 是否删除原有数据。（TRUE：表示删除，FALSE：表示不删除，默认情况不删除） 
RETURN VALUE  
	1）如果成功返回TRUE
	2）如果失败返回FALSE

}}}USR */
BOOL CSUSPEPGSetIsDeleteOldDataWhenEitVerChange( HCSHANDLE hEPG, BOOL bSchedule );

#ifdef  __cplusplus
}
#endif
// }}}USR

#endif /* EPGReceiverInterface_H */
