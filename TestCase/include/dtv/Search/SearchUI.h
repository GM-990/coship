#ifndef SearchUI_H
#define SearchUI_H

#ifdef PRAGMA
#pragma interface "SearchUI.h"
#endif
#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h"

/* {{{USR

   }}}USR */

// {{{USR
#ifdef __cplusplus
extern "C"
{
#endif

//增强型搜索的几种类型的宏定义
//有些增强型类型不能同时使用。
// SEARCH_ON_FRIST_NIT_CLEAR_DELIVERS与SEARCH_NIT_MAIN_CUSTOM，
//SEARCH_NIT_GET_TAG和SEARCH_NIT_GET_TAG_ONCE不能同时使用
#define SEARCH_BAT_BOUQUETID_TRANSPONDER      (0x00000001)     				//用BAT表进行要搜索的频点过滤,并且不做BAT节目过滤, 根据bouquet_id。
#define SEARCH_BAT_BOUQUETID_SERVICE          (0x00000002)     				//用BAT进行搜索出来频点及节目的过滤，根据bouquet_id。
#define SEARCH_BAT_GET_TAG                    (0x00000004)     				//进行BAT搜索加节目时返回相关的BAT Tag。
#define SEARCH_BAT_GET_SECTION                (0x00000008)     				//进行BAT搜索时返回相关的BAT Section。
#define SEARCH_EASY_INSTALL                   (0x00000010)     				//进行简易搜索，由Search决定锁频所用的diseqc端口。
#define SEARCH_NIT_GET_TAG                    (0x00000020)     				//进行NITPATPMT或NITSDT搜索加节目时取得NIT Tag并做返回给用
                                                               				//户,默认请求所有传入频点和新搜索频点的TAG
#define SEARCH_SDT_GET_TAG                    (0x00000040)     				//进行NITPATPMT搜索加节目时取得SDT Tag并返回给用户。
#define SEARCH_SERVICE_SAVE_CUSTOM            (0x00000100)     				//由用户进行自定义的存储。
#define ONLY_SEARCH_EXTERNAL_FREQ_NIT_TAG     (0x00000200)     				//请求NIT tag时，定义该宏表示"只搜索传入频点的NIT tag",该宏只在和SEARCH_NIT_GET_TAG
                                                               				//同时使用才有效，如果传入参数没有定义SEARCH_NIT_GET_TAG，该宏无效.
#define SEARCH_NIT_SYNCHRONOUS                (0x00000400)     				//NIT同步搜索，该搜索是对NIT搜索的优化，目前仅对卫星机搜索有效

#define ADVANCED_EASYINSTALL_SEARCH           (0x00000800)     				//简易搜索时如果用户已经配置端口，则不再进行自动配置端口
#define ADVACNED_EASYINSTALL_SEARCH           (ADVANCED_EASYINSTALL_SEARCH) //旧的错误拼写

#define SEARCH_ON_FRIST_NIT_CLEAR_DELIVERS	  (0x00001000)					//NITPATPMT搜索方式，当搜到第一个NIT频点时，清空Deliver list，与SEARCH_NIT_MAIN_CUSTOM不能同时使用
#define SEARCH_NIT_MAIN_CUSTOM				  (0x00002000)			//多中心频点搜索，在多个中心频点中搜索,直到找到的频点中发送的NIT表符合要求，如果找到，则以此NIT表为入口进行NITPATPMT搜索，与SEARCH_ON_FRIST_NIT_CLEAR_DELIVERS不能同时使用
//定义搜索时要求的匹配信息
#define SEARCH_PAT_TSID_MATCH					(0x00004000)			//搜索PAT表时要求PAT中的TS_ID同NIT表中的描述相匹配
#define SEARCH_SDT_TSID_MATCH					(0x00008000)			//搜索SDT表时要求SDT表中的TS_ID同NIT表中的描述相匹配	
#define SEARCH_SDT_ORINETWORKID_MATCH			(0x00010000)			//搜索SDT表时要求SDT表中的Ori_Network_ID同NIT表中的描述相匹配
#define SEARCH_BAT_ALL						  (0x00020000)					//搜索每一个频点的BAT表。默认情况下只搜索主频点的BAT表
#define SEARCH_NIT_GET_TAG_ONCE				(0x00040000)				//仅搜索NIT表的指定描述符一次，搜到就不再搜索
#define SEARCH_NIT_THROW_TAG_IF_HAVE			(0x00080000)				//只要在NIT表中发现有指定描述符就抛出，而不管该描述符的属性是否扩展到正在搜索的节目，适用于NIT表有多个Section且其中一级描述符不一致的情况,
																		//只有在找不到跟节目相关的描述符的情况下才执行此策略，由于指定的是描述符抛出策略而不是搜索策略，所以至少需要设置SEARCH_NIT_GET_TAG和SEARCH_GET_TAG_ONCE两种策略中的一种此策略才起作用

#define QUICK_SEARCH_EXT	(0x00100000)					//扩展NITSDT搜索，此策略在搜索完毕NIT表后同时请求所有SDT other表，速度更快
#define SEARCH_IP_SERVICE			(0x00200000)					//IP搜索
#define SEARCH_NIT_OTHER	(0x00400000)		//NIT搜索时搜索NIT Other表
#define SEARCH_SAVE_TS_WITH_NOSERVICE	(0x00800000)			//搜索完毕后保存没有节目的频点，默认情况下是不保存的
#define QUICK_SEARCH_ONLY_SDT   (0x01000000)    //扩展NITSDT搜索，此策略搜索SDT_Actrual表
#define QUICK_SEARCH_SDT        (0x02000000)    //扩展NITSDT搜索，此策略搜索SDT_Actual表和SDT_other表

#define QUICK_SEARCH_SDT_EXT_TIMEOUT   (0x04000000)       //扩展NITSDT搜索，此策略在搜索完毕NIT表后同时请求所有SDT other表，速度更快，搜索与QUICK_SEARCH_EXT相同，SDT表的判全采用SDT 超时
#define QUICK_SEARCH_SDT_TIMEOUT       (0x08000000)        //扩展NITSDT搜索，此策略搜索SDT_Actual表和SDT_other表， 搜索与QUICK_SEARCH_SDT相同，SDT other表的判全采用SDT表的超时

typedef enum callbackType
{
	SEARCH_NIT_FREQLIST,			//当前频点的NIT表搜索完成
	SEARCH_FREQ_START,				//开始搜索当前频点
	SEARCH_FREQ_FINISHED,			//当前频点的节目搜索完成
	SEARCH_FREQ_NITSDT,				//当前频点的NITSDT搜索完成(快速搜索当前网络)
	SEARCH_FINISHED,				//搜索完成

	SIGNAL_CONNECTED,				//频点锁定成功
	SIGNAL_SEARCHING,				//正在锁定频点
	SIGNAL_LOST,					//频点锁定失败
	SIGNAL_DROPPED,					//由频点锁定成功转为锁定失败

	SOURCE_PARAM_CHANGED,			//频点参数改变
	SOURCE_SIMILARLY_PARAM,			//频点参数相同
	SOURCE_FAILD,					//不能获取Tuner
	NIT_SEARCH_FINISHED,			//所有频点的NIT 搜索完成
	START_TRANSPONDER_SEARCH,		//开始搜索频点,目前只在卫星搜索中用到

    SERVICE_ISNEEDSAVE,             //是否需要保存该节目

	SEARCH_BAT_SECTION_COMING,      //BAT搜索时有Section数据到来
	SEARCH_BAT_FREQLIST,            //当前频点的BAT表搜索完成
	SEARCH_BAT_FINISHED,            //所有频点的BAT 搜索完成
	CAT_SECTION_COMMING,            //抛出CAT表

	SEARCH_PMT_SERVICE_TYPE,		//抛出SearchPMTServiceType用于设置节目类型,参数为SearchPMTServiceType
	NIT_ISCUSTOM,					//是否为自定义主频点?抛出该频点的网络信息让应用判断,用于SEARCH_NIT_MAIN_CUSTOM
	NIT_SEARCH_NEW_DELIVER,			//在带NIT的搜索中，每在nit表中新解析到一个tsloop后抛出其中内容，抛出内容为指向SITsInfo结构的指针

	SEARCH_NITSDT_TIMEOUT,		//快速搜索时请求nitsdt表超时,在用search模块完成sau(节目自动更新)功能时，会根据此回调判断是否需要保存更新到的数据
	SEARCH_PATPMT_TIMEOUT,		//patpmt搜索时patpmtsdt表超时，用法同上
	SEARCH_SERVICEINFO_EXT    //SIServiceInfoExt对应的数据
}CallBackType;

typedef enum _DATAPROCESSSTRATEGY
{
	SEARCH_PROCESS_DATA_DEFAULT = 0, //默认数据处理策略
	SEARCH_PROCESS_DATA_SKIP, 		//跳过对该频点数据的处理
	SEARCH_PROCESS_DATA_STOP		//停止对所有数据的处理，会导致停止搜索
}DATAPROCESSSTRATEGY;

//为每个表设置搜索超时时间，可根据实际情况进行扩展
//设创建搜索句柄是传入的超时时间为M秒，则PAT超时时间为1/5M，PMT超时时间为M-2秒
//SDT超时时间为M，这些表暂时不提供单独设置
typedef enum _SEARCHTABLETYPE
{
	USP_SEARCH_NIT = 0,  //搜索NIT
	USP_SEARCH_BAT,  //搜索BAT
	USP_SEARCH_CAT   //搜索CAT
}SEARCHTABLETYPE;

typedef enum _DELIVERSTATE
{
	SEARCH_DELIVER_RESET,			//初始状态
	SEARCH_DELIVER_NIT,				//当前频点NIT表搜索
	SEARCH_DELIVER_SERVICE,			//当前频点节目搜索完成
	SEARCH_DELIVER_NOSIGN,			//当前频点没有信号
	SEARCH_DELIVER_SKIP,			//当前频点取消搜索
	SEARCH_DELIVER_TIMEOUT			//当前频点搜索节目超时。只有在频点patpmtsdt表超时时才会记录为超时，nit,cat,bat表超时仍然设为SEARCH_DELIVER_SERVICE状态
}DELIVERSTATE;

typedef struct _SEARCHNITLIST
{
	int m_nDeliverIndex;			//当前NIT频点的索引(以创建SearchUI时输入的频点参数的顺序为准)
    int m_nNITCount;				//当前NIT表中包含的频点数(新搜索到的频点，如果频点已包含在频点列表中，则此频点不重复添加）
	int m_nNITStartIndex;			//当前频点NIT表中包含的开始频点在新的频点表中的索引
}SEARCHNITLIST,*PSEARCHNITLIST;

typedef struct _SEARCHFREQLIST
{
	int m_nDeliverIndex;			//当前频点的索引(以所有搜索频点的顺序为准)
    int m_nVideoCount;				//当前频点中所包含的Video节目数
	int m_nRadioCount;				//当前频点中所包含的Radio节目数
	int m_nVidoeStartIndex;			//当前频点中的第一个Video节目在Video队列中的索引
	int m_nRadioStartIndex;			//当前频点中的第一个Radio节目在Radio队列中的索引
}SEARCHFREQLIST,*PSEARCHFREQLIST;

typedef struct _SEARCHFREQLISTEx
{
	SEARCHFREQLIST m_FreqList;
	int m_ServiceCount;			//当前频点中所包含的Service数
	int m_ServiceStartIndex;		//当前频点中的第一个Service 节目在队列中的索引
}SEARCHFREQLISTEx,*PSEARCHFREQLISTEx;

typedef enum 
{
	SEARCH_UI_TYPE_SATELLITE,       //搜索卫星下的所有频点，不带NIT
	SEARCH_UI_TYPE_SATELLITE_NIT,   //搜索卫星下的所有频点，带NIT
	SEARCH_UI_TYPE_TRANSPONDER,     //搜索频点，不带NIT
	SEARCH_UI_TYPE_TRANSPONDER_NIT, //搜索频点，带NIT
    SEARCH_UI_TYPE_BLINDSCAN,        //盲扫搜索
    SEARCH_UI_TYPE_NITSDT
} SearchType;

typedef struct _SERVICEINFOFORCLIENT
{
	HCSHANDLE m_hServiceData;		//节目在数据管理中的存放句柄
    void *m_pvReserved;				//预留数据
} SERVICEINFOFORCLIENT;

//当请求BAT Tag或BouquetID信息时的请求信息
typedef struct _BATRequestInfo
{
	BYTE m_ucTagID[8];  		    //BAT TagID
	int m_nTagIDCount;              //Tag ID实际个数
	WORD m_wBouquetID[32];       	//BouquetID
	int m_nBouquetIDCount;          //BouquetID实际个数
} BATRequestInfo;

//当请求Tag(私有描述符)时的请求信息
typedef struct _SearchTagRequestInfo
{
	BYTE m_ucTagID[8];  		    //本次请求TAG的TagID
	int m_nTagIDCount;              //Tag ID实际个数
} SearchTagRequestInfo;

//简易搜索时的请求信息。
typedef struct _EasyInstallRequestInfo
{
   BOOL m_bScanAll;              //是否扫描所有频点。TRUE扫描所有频点直到找到第一个能锁住的为止，FALSE只扫描第一个，如不能锁住则退出搜索，如能锁住则继续往下搜索其他频点。
   BOOL m_bModifyDiseqcInfo;     //锁住频点后修改卫星的Diseqc参数，TRUE－修改，FALSE－不修改。
   DWORD m_dwDiseqcType;         //0-diseqc1.0, 1-diseqc1.1, 2-diseqc1.2
} EasyInstallRequestInfo;
//盲扫的请求信息。
typedef struct _BlindScanRequestInfo
{
    DWORD  dwStartFreq;           //开始频点频率
    DWORD  dwEndFreq;             //结束频点频率
    DWORD  dwStepLength;       	  //步长        
    DWORD  dwSymbolRate[8];       //符号率的数组
    DWORD  dwSymbolRateCount;  	  //符号率有效值个数
    DWORD  dwPolarization[2];     //极化方式
    DWORD  dwPolarizationCount;   //极化方式有效个数
    DWORD  dwFEC_inner;           //内部前向纠错方案
    HCSHANDLE hSatellite;         //要搜索的卫星句柄
    HCSHANDLE hAntenna;           //要搜索的天线句柄。
    BOOL    m_bScanAll;           //是否扫描所有频点。TRUE扫描所有频点并找到所有能锁//住的频点，FALSE只找到第一个能锁住的频点
} BlindScanRequestInfo;

//增强搜索时的总体请求信息
typedef struct _SearchExRequestInfo
{
	BATRequestInfo m_BATRequestInfo;   //BAT请求信息
	SearchTagRequestInfo m_NITTagRequestInfo;   //NIT Tag请求信息。
	SearchTagRequestInfo m_SDTTagRequestInfo;   //SDT Tag请求信息。
} SearchExRequestInfo;

//回调时返回BAT的信息的结构体
typedef struct _BATInfo
{
	WORD m_wBouquetID;                //对应请求的BouquetID
	BYTE *m_pucBATInfo;               //bat信息字符串
	DWORD m_dwBATInfoLength;          //bat信息的总长度
    BYTE *m_pucBouquetName;           //BouquetName
    DWORD m_dwBouquetNameLength;      //BouquetName的长度
} BATInfo;

//回调时返回的Tag(私有描述符)信息的结构体,但在返回CAT表时是整个表的信息
typedef struct _SearchPrivateInfo
{
   BYTE *m_pucPrivateInfo;                //信息字符串.对于cat表，m_pucPrivateInfo是一个cat section的内容。
   										//对于nit，sdt，bat表，m_pucPrivateInfo由多个描述符串联而成，m_dwPrivateInfoLength是所有描述符长度加起来的总长度。
   										//格式如下:||des1.tag|des1.dataLen|des1.data||des2.tag|des2.dataLen|des2.data||...||desn.tag|desn.dataLen|desn.data
   											//
   DWORD m_dwPrivateInfoLength;           //m_pucPrivateInfo信息的总长度。
} SearchPrivateInfo;

//增强搜索时的总体回应信息
typedef struct _SearchExResponseInfo
{
	int m_nDeliverIndex;	  		                 //当前频点的索引(以所有搜索频点的顺序为准)
	BATInfo m_BATResponseInfo;                       //保留域
    SearchPrivateInfo m_NITResponseInfo;             //NIT Tag回应信息。返回的是一个频点的信息。
	SearchPrivateInfo m_SDTResponseInfo;             //SDT Tag回应信息。返回的是一个频点的信息。
} SearchExResponseInfo;

//自定义存储节目时的节目相关信息结构体
typedef struct _SearchServiceHandleInfo{
	HCSHANDLE m_hService;           //还未真正加入到数据管理中的节目句柄，须用CSUSPDMAddTailDataHandle才能真正加入　
	WORD m_wServiceID;              //所属的节目ID
	BYTE m_ucServiceType;           //节目类型
	int m_nDeliverIndex;            //所属的搜索频点的索引号
	HCSHANDLE m_nDeliverHandle;     //该节目所属频点在数据管理中的句柄
} SearchServiceHandleInfo;


typedef struct _SearchPMTServiceType
{
	BYTE m_ucStreamType;		//当前PMT循环的StreamType字段
	WORD m_wElementaryPid;		//当前PMT循环的elementary_PID字段
	PBYTE m_pDescriptors;		//当前PMT第一批及当前循环中启动搜索时，指定关心的描述符
	BYTE m_ucDescriptorsLen;	//描述符长度
	HCSHANDLE hService;			//对于的Service句柄
}SearchPMTServiceType;

/*
void(*SearchCallBack)
	搜索回调涵数原型

	int(*SearchCallBack)(
		CallBackType enmType,
		 void* pParam
	};	

Parameters
	
	enmType
		[out]消息类型
			1)enmType=SEARCH_NIT_FREQLIST 表示当前频点的NIT表搜索完成
					pParam为指向SEARCHNITLIST的指针
					指向SEARCHNITLIST的指针中的m_nDeliverIndex代表当前NIT频点的索引(以创建SearchUI时输入的频点参数的顺序为准)
					指向SEARCHNITLIST的指针中的m_nNITCount代表当前频点NIT表中的频点个数
					指向SEARCHNITLIST的指针中的m_nNITStartIndex代表当前频点NIT表中包含的开始频点在新的频点表中的索引

			2)enmType=SEARCH_FREQ_FINISHED 表示当前频点的节目搜索完成
					pParam为指向SEARCHFREQLIST的指针
					指向SEARCHFREQLIST的指针中的m_nDeliverIndex代表当前频点的索引(以所有搜索频点的顺序为准)
					指向SEARCHFREQLIST的指针中的m_nVideoCount代表当前频点中所包含的Video节目数
					指向SEARCHFREQLIST的指针中的m_nRadioCount代表当前频点中所包含的Radio节目数
					指向SEARCHFREQLIST的指针中的m_nVidoeStartIndex代表当前频点中的第一个Video节目在Video队列中的索引
					指向SEARCHFREQLIST的指针中的m_nRadioStartIndex代表当前频点中的第一个Radio节目在Radio队列中的索引

			3)enmType=SEARCH_FREQ_NITSDT 表示当前频点的NITSDT快速节目搜索完成
					pParam为指向SEARCHFREQLIST的指针
					指向SEARCHFREQLIST的指针中的m_nDeliverIndex代表当前频点的索引(以所有搜索频点的顺序为准)
					指向SEARCHFREQLIST的指针中的m_nVideoCount代表当前频点中所包含的Video节目数
					指向SEARCHFREQLIST的指针中的m_nRadioCount代表当前频点中所包含的Radio节目数
					指向SEARCHFREQLIST的指针中的m_nVidoeStartIndex代表当前频点中的第一个Video节目在Video队列中的索引
					指向SEARCHFREQLIST的指针中的m_nRadioStartIndex代表当前频点中的第一个Radio节目在Radio队列中的索引

					注:在发送该消息之前将会发送	SEARCH_NIT_FREQLIST 消息

			4)enmType=SEARCH_FINISHED 表示搜索完成
					pParam = NULL
					
			5)enmType=SERVICE_ISNEEDSAVE 表示是否需要保存该节目。
					pParam为指向SERVICEINFOFORCLIENT的指针。
					指向SERVICEINFOFORCLIENT的指针中的m_hServiceData为节目在数据管理中的句柄值，用户据此从数据管理中取出对应数据进行判断是否需要存储，如果需要存储则返回0,不存储则返回非零值。
					指向SERVICEINFOFORCLIENT的指针的m_pvReserved为预留指针，是指向SearchExResponseInfo的指针。
					
			6)enmType = SEARCH_PMT_SERVICE_TYPE 表示用户是否需要根据PMT信息指定PID类型并指定
					pParam指向SearchPMTServiceType
					返回非0表示已接收该循环，无需search再分析，不影响后续循环

			7)enmType = NIT_ISCUSTOM, 询问该频点是否是符合要求的自定义频点
					pParam为指向SINetworkInfoExt的指针。
					用户对NIT表进行判断,如果是自定义的NIT表,则返回0,否则请返回1
			8)NIT_SEARCH_FINISHED,	频点搜索完成。pParam为NULL
					用户可在此回调消息中添加自定义的频点。
	pParam
		[out]返回的实际参数(与消息类型密切相关)

Return Values
		除非有特殊说明，否则请返回0
*/
typedef int(*SearchCallBack)(CallBackType enmType, void* pParam);
// }}}USR

/* {{{USR
CreateSearch
    申请第一个句柄资源（创建Searching类的句柄，这是启动Search模块的第一步）

	HCSHANDLE CreateSearch( void );

Parameters	
        无

Return Values
		1）如果成功返回申请的资源句柄
		2）如果失败返回为NULL

   }}}USR */
HCSHANDLE CreateSearch( void );

/* {{{USR
CloseSearch
    释放第一个申请的句柄资源(CreateSearch的返回句柄)

	BOOL CloseSearch( HCSHANDLE hSearch );

Parameters	
        hSearch
           [in]第一个申请的句柄资源

Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

   }}}USR */
BOOL CloseSearch( HCSHANDLE hSearch );

/* {{{USR
CreateNITPATPMTSearchUI
    申请提供NITPATPMT搜索方式的句柄资源（这是启动Search模块的第二步）

	HCSHANDLE CreateNITPATPMTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, 
                   int nDeliverListCount, SearchCallBack callback, int mTimeOut );

Parameters	
        hSearch
           [in]由第一步创建的搜索句柄
        pDeliverList
           [in]频点数组的首地址        
        nDeliverListCount
           [in]频点的个数
        callback 
           [in]回调函数
        mTimeOut
           [in]超时时间

Return Values
		1）如果成功返回申请的资源句柄
		2）如果失败返回为NULL
        
        当hSearch为NULL或pDeliverList为NULL或nDeliverListCount小于等于零时，创建会失败。

   }}}USR */
HCSHANDLE CreateNITPATPMTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, int nDeliverListCount, SearchCallBack callback, int mTimeOut );

/* {{{USR
CreatePATPMTSearchUI
    申请提供PATPMT搜索方式的句柄资源（这是启动Search模块的第二步）

	HCSHANDLE CreatePATPMTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, 
                   int nDeliverListCount, SearchCallBack callback, int mTimeOut );

Parameters	
        hSearch
           [in]由第一步创建的搜索句柄
        pDeliverList
           [in]频点数组的首地址        
        nDeliverListCount
           [in]频点的个数
        callback 
           [in]回调函数
        mTimeOut
           [in]超时时间

Return Values
		1）如果成功返回申请的资源句柄
		2）如果失败返回为NULL

        当hSearch为NULL或pDeliverList为NULL或nDeliverListCount小于等于零时，创建会失败。
   }}}USR */
HCSHANDLE CreatePATPMTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, int nDeliverListCount, SearchCallBack callback, int mTimeOut );

/* {{{USR
CreateNITSDTSearchUI
    申请提供NITSDT搜索方式的句柄资源（这是启动Search模块的第二步）

	HCSHANDLE CreateNITSDTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, 
                   int nDeliverListCount, SearchCallBack callback, int mTimeOut );

Parameters	
        hSearch
           [in]由第一步创建的搜索句柄
        pDeliverList
           [in]频点数组的首地址        
        nDeliverListCount
           [in]频点的个数
        callback 
           [in]回调函数
        mTimeOut
           [in]超时时间

Return Values
		1）如果成功返回申请的资源句柄
		2）如果失败返回为NULL

        当hSearch为NULL或pDeliverList为NULL或nDeliverListCount小于等于零时，创建会失败。

   }}}USR */
HCSHANDLE CreateNITSDTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, int nDeliverListCount, SearchCallBack callback, int mTimeOut );

/* {{{USR
CreateSearchUI
    申请提供卫星搜索方式的句柄资源（这是启动Search模块的第二步）

	HCSHANDLE CreateSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, 
                int nDeliverListCount, SearchCallBack callback, int mTimeOut, SearchType enmSearchType ) ;

Parameters	
        hSearch
           [in]由第一步创建的搜索句柄
        pDeliverList
           [in]频点数组的首地址        
        nDeliverListCount
           [in]频点的个数
        callback 
           [in]回调函数
        mTimeOut
           [in]超时时间
        enmSearchType 
           [in]搜索类型

Return Values
		1）如果成功返回申请的资源句柄
		2）如果失败返回为NULL

        当hSearch为NULL或pDeliverList为NULL或nDeliverListCount小于等于零时，创建会失败。

   }}}USR */
HCSHANDLE CreateSearchUI( HCSHANDLE hSearch, HCSHANDLE * phSearchDataList, int nSearchDataCount, SearchCallBack callback, int mTimeOut, SearchType enmSearchType );

/* {{{USR
CloseSearchUI
    释放第二步申请的句柄资源

	BOOL CloseSearchUI( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源

Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

   }}}USR */
BOOL CloseSearchUI( HCSHANDLE hSearchUI );

/* {{{USR
StartSearch
    开始Search模块的搜索（这是启动Search模块的第三步）

	BOOL StartSearch( HCSHANDLE hSearchUI, HCSHANDLE hDataContainer, HCSHANDLE hSource );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源
        hDataContainer
           [in]数据容器的句柄
        hSource
           [in]用于锁频的Source的句柄

Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

   }}}USR */
BOOL StartSearch( HCSHANDLE hSearchUI, HCSHANDLE hDataContainer, HCSHANDLE hSource );

/* {{{USR
StopSearch
    终止搜索，退出搜索过程

	BOOL StopSearch( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源

Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

   }}}USR */
BOOL StopSearch( HCSHANDLE hSearchUI );

/* {{{USR
SkipSearch
    跳过当前搜索的频点，搜索下一个频点的节目或频点（带NIT搜索）

	BOOL SkipSearch( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源

Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

   }}}USR */
BOOL SkipSearch( HCSHANDLE hSearchUI );

/* {{{USR
GetAllVideoProgNum
    得到当前搜索到的Video节目数量。

	int GetAllVideoProgNum( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源

Return Values
		返回当前搜索到的Video节目数量

   }}}USR */
int GetAllVideoProgNum( HCSHANDLE hSearchUI );

/* {{{USR
GetAllRadioProgNum
    得到当前搜索到的所有Radio节目的数量。

	int GetAllRadioProgNum( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源

Return Values
		返回当前搜索到的所有Radio节目的数量

   }}}USR */
int GetAllRadioProgNum( HCSHANDLE hSearchUI );

/* {{{USR
GetSearchDeliver
    得到用于搜索的频点。

	BOOL GetSearchDeliver( HCSHANDLE hSearchUI, int nIndex, Deliver * pDeliver );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源
        nIndex
           [in]频点的序号
        pDeliver
           [out]用户存放频点信息的数据块的地址
Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

        当hSearchUI为NULL或nIndex小于零或pDeliver为NULL时，获取会失败。

   }}}USR */
BOOL GetSearchDeliver( HCSHANDLE hSearchUI, int nIndex, Deliver * pDeliver );

/* {{{USR
GetSearchDeliverHandle
    得到用于搜索的频点在数据管理中的句柄。

	BOOL GetSearchDeliverHandle( HCSHANDLE hSearchUI, int nIndex, HCSHANDLE * pDeliverHandle );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源
        nIndex
           [in]频点的序号
        pDeliverHandle
           [out]用户存放频点句柄的指针
Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

        当hSearchUI为NULL或nIndex小于零或pDeliverHandle为NULL时，获取会失败。

   }}}USR */
BOOL GetSearchDeliverHandle( HCSHANDLE hSearchUI, int nIndex, HCSHANDLE * pDeliverHandle );

/* {{{USR
GetSearchState
    得到频点的状态。

	BOOL GetSearchState( HCSHANDLE hSearchUI, int nIndex, int * pnState );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源
        nIndex
           [in]频点的序号
        pnState
           [out]存放状态值的指针
Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

        当hSearchUI为NULL或nIndex小于零或pnState为NULL时，获取会失败。

   }}}USR */
BOOL GetSearchState( HCSHANDLE hSearchUI, int nIndex, int * pnState );

/* {{{USR
GetSearchServerDataHandle
    得到Service在数据管理中的句柄。

	BOOL GetSearchServerDataHandle( HCSHANDLE hSearchUI, int nIndex, int nServiceType, 
                HCSHANDLE * phServerData );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源
        nIndex
           [in]节目在本次搜索结果中的序号
        nServiceType
           [in]Service类型，TV or Radio。
        phServerData
           [out]存放service的句柄指针
Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE
		
		不可用于自定义存储方式。自定义存储请参考GetSearchServiceHandleInfo().
        当hSearchUI为NULL或nIndex小于零或phServerData为NULL时，获取会失败。

   }}}USR */
BOOL GetSearchServerDataHandle( HCSHANDLE hSearchUI, int nIndex, int nServiceType, HCSHANDLE * phServerData );

/* {{{USR
CSUSPSearchGetVersionInfo
    获取模块版本信息， 包括版本号、最后修订日期。用户可以选择性地调用该方法获取USP或者其子模块的版本信息。

	 int CSUSPSearchGetVersionInfo( char * pcVersionInfo, int nSize );

Parameters
       pcVersionInfo
       [out]存放版本信息的缓冲区首地址，输出参数,存放版本信息。格式如下：
            “Module Name: EPG\nCurrent Version: 1.2\nRevision Date: 2005-5-11”
       nSize
       [in]    缓冲区的长度

Return Values
		1)	如果第一个参数pcVersionInfo为空，返回版本信息的实际长度。建议用户输入第二个参数为：版本信息的实际长度+       
1。 
       2)	如果第一个参数pcVersionInfo不为空，且函数调用成功，返回版本信息的实际长度；
       3)	如果失败，返回0。

       利用第一种返回值的情况，用户可测试该函数，并为第二个参数,nSize,提供参考。
       如果用户输入的第二个参数小于版本信息的实际长度，尾部多余的字节被截掉。
   }}}USR */
int CSUSPSearchGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
CSUSPSearchGetMemInfo
    获取模块要求的内存空间的最小值和最大值。当该模块要求一个固定空间时，*pnMin 和*pnMax相等；有的模块至少需要*pnMin bytes才能正常运行；在分配*pnMax空间时，可获得更好的性能。

	 BOOL CSUSPSearchGetMemInfo( int * pnMin, int * pnMax );

Parameters
       pnMin
       [out]模块所需最小内存 (in bytes)
       pnMax
       [out]模块所需最大内存(in bytes)

Return Values
       1)成功，返回TRUE;
       2)失败，返回 FALSE

Remarks
       1）	当模块要求固定内存时，返回的最大内存和最小内存相等。；
       2）	当模块要求最小内存为*pnMin， 而在*pnMin 和*pnMax之间可获得更好的性能或更多功能时，最大内存和最小内存不等。
       3）	当pnMin =NULL 或 pnMax = NULL，函数调用失败。
       4） 该方法对内存的合理使用，硬件的配置具有参考价值。    

   }}}USR */
BOOL CSUSPSearchGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR
GetServiceNum
    根据类型得到当前搜索到的该类型的节目数量。

	int GetServiceNum( HCSHANDLE hSearchUI, int nServiceType );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源
        nServiceType
           [in]Service类型，TV(0x01) or Radio(0x02) 或其他类型

Return Values
		返回当前搜索到的节目数量
		
Remark
    当nServiceType为0xffffffff时获取所有类型节目的总个数


   }}}USR */
int GetServiceNum( HCSHANDLE hSearchUI, int nServiceType );

/* {{{USR
StartSearchEx
    开始Search模块的增强搜索（这是启动Search模块的第三步）

	BOOL StartSearchEx( HCSHANDLE hSearchUI, HCSHANDLE hDataContainer, HCSHANDLE hSource,  int nThreadStackSize, int nSearchServiceType, void *pvParam );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源
        hDataContainer
           [in]数据容器的句柄
        hSource
           [in]用于锁频的Source的句柄
		nThreadStackSize
		   [in]搜索线程堆栈大小，默认的为8192*2 Bytes字节大小，该值必须大于零，小于等于零被忽略，用默认大小。
		nSearchServiceType
		   [in]要搜索的类型，宏定义见头文件中增强型搜索的定义，类型之间可用或的关系。
	    pvParam
		   [in]为了传各种不同类型的参数用的void类型指针。
           当为SEARCH_BAT_BOUQUETID_TRANSPONDER、SEARCH_BAT_BOUQUETID_SERVICE、SEARCH_BAT_GET_TAG、SEARCH_NIT_GET_TAG、SEARCH_SDT_GET_TAG搜索类型时，pvParam为指向SearchExRequestInfo结构体的指针。
		   当为SEARCH_EASY_INSTALL搜索类型时，pvParam为指向EasyInstallRequestInfo结构体的指针。
		   当为SEARCH_SERVICE_SAVE_CUSTOM搜索类型时，pvParam指针可指向NULL，不传入任何数据。

Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

   }}}USR */
BOOL StartSearchEx( HCSHANDLE hSearchUI, HCSHANDLE hDataContainer, HCSHANDLE hSource, int nThreadStackSize, int nSearchServiceType, void * pvParam );

/* {{{USR
GetSearchServiceHandleInfo
    得到Service在数据管理中的信息。

	BOOL GetSearchServiceHandleInfo( HCSHANDLE hSearchUI, int nIndex, BYTE ucServiceType, SearchServiceHandleInfo * pSearchServiceHandleInfo );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源
        nIndex
           [in]节目在本次搜索结果中的序号
        ucServiceType
           [in]Service类型，TV(0x01) or Radio(0x02)。 
        pSearchServiceHandleInfo
           [out]存放SearchServiceHandleInfo结构体的指针
Return Values
		1）如果成功返回TRUE
		2）如果失败返回为FALSE

        当hSearchUI为NULL或nIndex小于零或pSearchServiceHandleInfo为NULL时，获取会失败。

   }}}USR */
BOOL GetSearchServiceHandleInfo( HCSHANDLE hSearchUI, int nIndex, BYTE ucServiceType, SearchServiceHandleInfo * pSearchServiceHandleInfo );

/* {{{USR
CSUSPSearchGetNITVersion
    得到NIT版本。

	BYTE CSUSPSearchGetNITVersion( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]第二步所创建的搜索资源

Return Values
	本函数返回值为0xff时表示失败，其他小于等于0x1f的值为成功。
NOTE
	本函数常被误解，其返回的版本号是最后搜索过的NIT表的版本号，而不一定是当前频点所在的NIT表的版本号。
	如果需要得到每个频点中的NIT表的版本号，增强搜索类型请包含SEARCH_NIT_GET_TAG。
	要得到当前频点所在的网络的NIT表的版本号，请调用DM接口CSUSPDMGetTPNITVersion();

   }}}USR */
BYTE CSUSPSearchGetNITVersion( HCSHANDLE hSearchUI );

/* {{{USR
GetServiceFirstBATInfo
	获取Service所属第一个Bouquet信息(一个ServiceID可属于多个BouquetID,
	也可属于一个bouquetID下多个不同的transport_stream_id+original_network_id).

	HCSHANDLE GetServiceFirstBATInfo( HCSHANDLE hSearchUI, WORD wServiceID, BATInfo * pBatInfo, WORD * pwTsID, WORD * pwOriNetID );

Parameters
		HCSHANDLE hSearchUI 	
			[in]搜索窗口句柄
		WORD wServiceID     	
			[in]Service标识
		BATInfo * pBatInfo  	
			[out]Service所属第一个Bouquet信息
		WORD    * pwTsID       	
			[out]Service所属的transport_stream_id(如果用户确保所有serviceID在所
		         有ts_id中都唯一，可以不关心此域，传入NULL即可)
        WORD    * pwOriNetID  	
        	[out]Service所属的original_network_dd(如果用户确保所有serviceID在所
		         有ts_id中都唯一，可以不关心此域，传入NULL即可)	
Return Values
		NULL 	表示搜索完毕，未找到相应Bouquet信息
		非NULL	表示找到相应Bouquet信息，搜索没有结束.返回搜索句柄
   }}}USR */
HCSHANDLE GetServiceFirstBATInfo( HCSHANDLE hSearchUI, WORD wServiceID, BATInfo * pBatInfo, WORD * pwTsID, WORD * pwOriNetID );

/* {{{USR
GetServiceNextBATInfo
	获取Service所属第一个Bouquet信息(一个ServiceID可属于多
	个BouquetID,也可属于一个bouquetID下多个不同的transport_stream_id+original_network_id).

	HCSHANDLE GetServiceNextBATInfo( HCSHANDLE hSearchUI, HCSHANDLE hBATHandle, WORD wServiceID, BATInfo * pBatInfo, WORD * pwTsID, WORD * pwOriNetID );

Parameters
		HCSHANDLE hSearchUI  	
			[in]搜索窗口句柄
		HCSHANDLE hBATHandle 	
			[in]上一次调用GetServiceFirstBATInfo或GetServiceNextBATInfo返回的搜索句柄	
		WORD wServiceID      	
			[in]Service标识
		BATInfo * pBatInfo  	
			[out]Service所属下一个Bouquet信息
		WORD    * pwTsID       	
			[out]Service所属的transport_stream_id(如果用户确保所有serviceID在所
		         有ts_id中都唯一，可以不关心此域，传入NULL即可)
        WORD    * pwOriNetID   	
        	[out]Service所属的original_network_dd(如果用户确保所有serviceID在所
		         有ts_id中都唯一，可以不关心此域，传入NULL即可)		                    
Return Values
		NULL 	表示搜索完毕，未找到相应Bouquet信息
		非NULL	表示找到相应Bouquet信息，搜索没有结束.返回搜索句柄

   }}}USR */
HCSHANDLE GetServiceNextBATInfo( HCSHANDLE hSearchUI, HCSHANDLE hBATHandle, WORD wServiceID, BATInfo * pBatInfo, WORD * pwTsID, WORD * pwOriNetID );

/* {{{USR
CSUSPSearchSetPmtRequestTagParam
	设置请求pmt表描述符参数

	BOOL CSUSPSearchSetPmtRequestTagParam( HCSHANDLE hSearchUI, BYTE * pucTagID, DWORD dwTagCnt );


Parameters
		HCSHANDLE hSearchUI  	
			[in]搜索窗口句柄
		BYTE * pucTagID 	
			[in]设置请求pmt表描述符tagID
		DWORD dwTagCnt      	
			[in]请求的有效的tag数
Return Values
		TRUE 	设置参数成功
		FALSE	设置参数失败
   }}}USR */
BOOL CSUSPSearchSetPmtRequestTagParam( HCSHANDLE hSearchUI, BYTE * pucTagID, DWORD dwTagCnt );

/* {{{USR
CSUSPSearchGetCurServicePmtFirstTagInfo
	在ISNEEDSAVE回调中，获取当前节目的pmt tag描述符数据
	WARNING: 该函数必须只能在ISNEEDSAVE回调中调用才能确保正确性

	BOOL CSUSPSearchGetCurServicePmtFirstTagInfo( HCSHANDLE hSearchUI, DWORD * pdwBufAddr, DWORD * pdwBufLen );


Parameters
		HCSHANDLE hSearchUI  	
			[in]搜索窗口句柄
		DWORD * pdwBufAddr	
			[out]得到tag buf地址
		DWORD * pdwBufLen
			[out]得到tag buf的长度
Return Values
		TRUE 	得到pmt tag数据
		FALSE	未得到pmt tag 数据

   }}}USR */
BOOL CSUSPSearchGetCurServicePmtFirstTagInfo( HCSHANDLE hSearchUI, DWORD * pdwBufAddr, DWORD * pdwBufLen );

/* {{{USR
CSUSPSearchGetCurServiceFirstElementInfo
	在SERVICE_ISNEEDSAVE回调中调用该函数提取PMT表ELEMENT的相关信息
	HCSHANDLE CSUSPSearchGetCurServiceFirstElementInfo( HCSHANDLE hSearchUI, DWORD * pdwStreamType, DWORD * pdwElementPid, DWORD * pdwBufAddr, DWORD * pdwBufLen );

Parameters
		HCSHANDLE hSearchUI  	
			[in]搜索窗口句柄
		DWORD * pdwStreamType
			[out]传出stream_type 值
		DWORD * pdwElementPid
			[out]传出pid值
		DWORD * pdwBufAddr
			[out]传出该element中包含的请求的tag buf的地址
        DWORD * pdwBufLen
        	[out]传出该element中包含的请求的tag buf的长度
Return Values
		NULL 	表示搜索完毕
		非NULL	表示搜索成功，没有搜索完毕，返回搜索句柄
   }}}USR */
HCSHANDLE CSUSPSearchGetCurServiceFirstElementInfo( HCSHANDLE hSearchUI, DWORD * pdwStreamType, DWORD * pdwElementPid, DWORD * pdwBufAddr, DWORD * pdwBufLen );

/* {{{USR
CSUSPSearchGetCurServiceNextElementInfo
	在SERVICE_ISNEEDSAVE回调中调用该函数提取PMT表ELEMENT的相关信息
	HCSHANDLE CSUSPSearchGetCurServiceNextElementInfo( HCSHANDLE hSearchUI, HCSHANDLE hPreSearch, DWORD * pdwStreamType, DWORD * pdwElementPid, DWORD * pdwBufAddr, DWORD * pdwBufLen );

Parameters
		HCSHANDLE hSearchUI  	
			[in]搜索窗口句柄
		HCSHANDLE hPreSearch
			[in]上一次调用CSUSPSearchGetCurServiceFirstElementInfo 或 CSUSPSearchGetCurServiceNextElementInfo返回的搜索句柄
		DWORD * pdwStreamType
			[out]传出stream_type 值
		DWORD * pdwElementPid
			[out]传出pid值
		DWORD * pdwBufAddr
			[out]传出该element中包含的请求的tag buf的地址
        DWORD * pdwBufLen
        	[out]传出该element中包含的请求的tag buf的长度
Return Values
		NULL 	表示搜索完毕
		非NULL	表示搜索成功，没有搜索完毕，返回搜索句柄
   }}}USR */
HCSHANDLE CSUSPSearchGetCurServiceNextElementInfo( HCSHANDLE hSearchUI, HCSHANDLE hPreSearch, DWORD * pdwStreamType, DWORD * pdwElementPid, DWORD * pdwBufAddr, DWORD * pdwBufLen );

/* {{{USR
CSUSPSearchSetValidBouquetIDRange
	设置BAT 合法的bouquetID区间[wMinBouquetID,wMaxBouquetID],闭合区间(包括wMinBouquetID和wMaxBouquetID)
	如果不调用该函数，search默认的合法区间为[0,0xFFFF]
	
	BOOL CSUSPSearchSetValidBouquetIDRange( HCSHANDLE hSearchUI, WORD wMinBouquetID, WORD wMaxBouquetID );

Parameters
		HCSHANDLE hSearchUI  	
			[in]搜索窗口句柄
		WORD wMinBouquetID	
			[in]最小的合法bouquetID值
		WORD wMaxBouquetID
			[in]最大值的合法bouquetID值
Return Values
		TRUE 	设置成功
		FALSE	设置失败
   }}}USR */
BOOL CSUSPSearchSetValidBouquetIDRange( HCSHANDLE hSearchUI, WORD wMinBouquetID, WORD wMaxBouquetID );

/* {{{USR
CSUSPSearchRequestCATTable
	在启动搜索前调用该函数 search将抛出CAT表原始数据	
	void CSUSPSearchRequestCATTable( HCSHANDLE hSearchUI )

Parameters
		HCSHANDLE hSearchUI  	
			[in]搜索窗口句柄
Return Values
		无
   }}}USR */
void CSUSPSearchRequestCATTable( HCSHANDLE hSearchUI );

/* {{{USR
CSUSPSearchRequestPMTTable
	在启动搜索前调用该函数 search将支持在SERVICE_ISNEEDSAVE回调中提取PMT表原始数据	
	void CSUSPSearchRequestPMTTable( HCSHANDLE hSearchUI )

Parameters
		HCSHANDLE hSearchUI  	
			[in]搜索窗口句柄
Return Values
		无
   }}}USR */
void CSUSPSearchRequestPMTTable( HCSHANDLE hSearchUI );

/* {{{USR
CSUSPSearchGetCurServicePmtSection
	SERVICE_ISNEEDSAVE回调中调用该函数提取当前节目的PMT表原始数据,需要在搜索之前调用CSUSPSearchRequestPMTTable	
	BOOL CSUSPSearchGetCurServicePmtSection( HCSHANDLE hSearchUI, DWORD * pdwBufAddr, DWORD * pdwBufLen );

Parameters
		HCSHANDLE hSearchUI  	
			[in]搜索窗口句柄
		DWORD * pdwBufAddr
			[out]传出存储pmt原始数据的buf地址
		DWORD * pdwBufLen
			[out]传出存储pmt原始数据的buf的长度
Return Values
		TURE  提取成功
		FALSE 提取失败
   }}}USR */
BOOL CSUSPSearchGetCurServicePmtSection( HCSHANDLE hSearchUI, DWORD * pdwBufAddr, DWORD * pdwBufLen );

/* {{{USR
	设置BAT搜索的BouquetID的搜索周期，即搜索到几次BouquetID循环任务搜索完成，默认为1个循环。
   }}}USR */
BOOL CSUSPSearchSetBouquetIDComingCnt( HCSHANDLE hSearchUI, int nMaxCnt );

/* {{{USR
	是否强制搜索BAT，在强制状态下，即使指定了BouquetID，依然搜索所有Bouquet
   }}}USR */
BOOL CSUSPSearchSetBatForceAutoSearch( HCSHANDLE hSearchUI, BOOL bForeceAutoSearch );

/* {{{USR
对CSUSPSearchSetBouquetIDComingCnt的扩展，如果搜索到nNormalComingCnt次BouquetID循环，BouquetID个数依然没有达到nLessBouquetIDCnt，则BAT搜索的BouquetID的搜索周期扩展至nAbnormalComingCnt
   }}}USR */
BOOL CSUSPSearchSetBouquetIDComingCntAutoExtern( HCSHANDLE hSearchUI, int nNormalComingCnt, int nLessBouquetIDCnt, int nAbnormalComingCnt );

/* {{{USR
调用此函数设置SDT的搜全策略为搜全所有PAT中描述的Service，该接口暂时对卫星机无效
   }}}USR */
void CSUSPSearchSetSdtCompleteByPat( HCSHANDLE hSearchUI );

/* {{{USR
获取某个频点下搜到的NIT中的network_id（如果没搜到返回0xffff)
   }}}USR */
BOOL GetSearchDeliverNitNetworkID( HCSHANDLE hSearchUI, int nIndex, WORD * pwNetWorkID );

/* {{{USR
在SERVICE_ISNEEDSAVE回调中得到当前节目的EIT_Schedule_Flag信息
   }}}USR */
BOOL CSUSPSearchGetCurServiceEITScheduleFlag( HCSHANDLE hSearchUI, BYTE * pbEITScheduleFlag );

/* {{{USR
在SERVICE_ISNEEDSAVE回调中得到当前节目的EIT_PF_Flag信息
   }}}USR */
BOOL CSUSPSearchGetCurServiceEITPFFlag( HCSHANDLE hSearchUI, BYTE * pbEITPFFlag );

/* {{{USR
在SERVICE_ISNEEDSAVE回调中得到当前节目的ServiceID
   }}}USR */
BOOL CSUSPSearchGetCurServiceID( HCSHANDLE hSearchUI, WORD * pwServiceID );

/* {{{USR
NITPATPMT搜索中，搜索NIT表完毕后在频点列表中添加频点，这样可以搜索这些频点中的节目。在NITPATPMT搜索情况下，如果NIT表中缺少某些频点的描述子，建议在回调消息NIT_SEARCH_FINISHED中调用此函数。如果频点表中已经包含要加入的频点，则添加失败返回FALSE。添加成功则返回TRUE。

   }}}USR */
BOOL CSUSPSearchAddDeliver( HCSHANDLE hSearchUI, Deliver * pDeliver );

/* {{{USR
在扩展NITSDT搜索中，如果sdt other表的发送不规则，可以调用CSUSPSearchSetSdtOtherComingCnt来设置同一个sdtother表的最大重复次数，即，当有某个sdt other表请求到nMaxCnt+1次后，认为搜索完毕。默认是1。
   }}}USR */
BOOL CSUSPSearchSetSdtOtherComingCnt( HCSHANDLE hSearchUI, int nMaxCnt );

/*设置要搜索时所用的Tuner和Demux设备的句柄*/
BOOL CSUSPSearchSetTunerDemuxPath(HCSHANDLE hSearchUI, HCSHANDLE hRCPath);

//设置search模块保存的音视频类型是否支持mediaplayer直接使用，默认为FALSE
//FALSE,按照cs_sysav.h中定义存储音视频类型，且音频类型为2个bit。这是之前SM所要求的.
//TRUE, 按照cs_sysaudio.h和cs_sysvidio.h中定义存储音视频类型，且音频类型为8个bit。这是mediaplayer要求的。
void CSUSPSearchSetSupportMPlayer( BOOL bSupportMplayer);

/**
@brief 设置节目名称数据中码表类型标志位的处理类型，开始搜索之前调用

@param[in] hSearchUI  搜索窗口句柄
@param[in] bIsGetOriginal  是否保存原始码表类型标志位
TRUE:保存原始码表类型标志位
FALSE:为USP_CODING_UNICODE或USP_CODING_ASC，默认为该种状态
@return 成功返回TRUE；失败则返回FALSE
*/
BOOL CSUSPSearchSetServiceNameCCTableIndexFlag(HCSHANDLE hSearchUI, BOOL bIsGetOriginal);

/**
@brief 分别设置搜索各个表的超时时间，在开始搜索前调用有效，如果不设置则以创建搜索时
设置的总的超时时间为准

@param[in] hSearchUI  搜索窗口句柄
@param[in] emTableType 搜索表的类型
@param[in] nTimeout 搜索表的超时时间
@return 设置成功返回TRUE；失败则返回FALSE
*/
BOOL CSUSPSearchSetSearchTableTimeout(HCSHANDLE hSearchUI, SEARCHTABLETYPE emTableType, int nTimeout);

/**
@brief 设置当存在多个NIT子表时是否需要搜索多个NIT子表

@param[in] TRUE表示需要存在多个NIT子表时需要逐一搜索，返回FALSE时表示不需要搜索
@return 无
*/
void CSUSPSearchNeedSearchMultiNIT(BOOL bIsNeedSearchMultiNIT);

/**
@brief 获取当前搜索的nit版本号

@param[in]无
@return 当前频点解析出的nit版本号
*/
BYTE CSUSPSearchGetNitVersion();

/**
@brief 获取当前搜索的BAT 版本号

@param[in]无
@return 当前频点解析出的BAT 版本号
*/
BYTE CSUSPSearchGetBatVersion();

/**
@brief 表示pat的tsid无效，以sdt的tsid为准
@param[in]hSearchUI  搜索窗口句柄
@return 设置成功返回TRUE；失败则返回FALSE
*/
BOOL CSUSPSearchSetPATTsidInvalid(HCSHANDLE hSearchUI); 
// {{{USR
#ifdef __cplusplus
}
#endif
// }}}USR

#endif /* SearchUI_H */
