/**@defgroup PublicShellSearch PublicShellSearch 定义搜索相关的对外接口及方法
@brief 本模块用于应用处理私有搜索流程，相关流程须尽量与SVS保持一致

@note 应用需在相关回调中调用DTV(USP)-Search的相关接口进行搜索，流程与SVS近似

@version 2.0.0 2009/12/11 初稿
@{
*/
#ifndef CSSHELL_SEARCH_H
#define CSSHELL_SEARCH_H

#include "mmcp_typedef.h"
#include "SearchUI.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSSHELL_SEARCH_MAX_TAG_COUNT    (8) ///< 每次搜索可请求的某种类型描述符的最大个数
#define CSSHELL_SEARCH_MAX_BOUQUETID_COUNT    (32)  ///每次搜索可设置的用于BAT过滤的BOUQUETID的最大个数

/**@brief 搜索存储选择。搜索方式和搜索模式可以确定搜索存储策略。所以，搜索时方式和模式是由应用决定，而存储方式则由前二者决定*/
typedef enum
{
    EM_CSSHELL_SEARCH_SAVE_OKEND,  ///< 搜索成功结束才保存
    EM_CSSHELL_SEARCH_SAVE_ANY     ///< 任何情况下都保存
}CSShellSearchSaveMode_E;

/**
@brief 搜索描述符的模式
@note 这些模式目前仅对nit和bat生效。对sdt actual不起作用，因为每个频点的sdt actual表都不同，必须都搜索	
*/
typedef enum
{
	EM_CSSHELL_SEARCH_TAG_DEFAULT,		///<默认。对于nit来说，是每个频点都搜索；对于bat来说，是搜索到一次就不再搜索
	EM_CSSHELL_SEARCH_TAG_ONCE,			///<搜索到一次就不再搜索
	EM_CSSHELL_SEARCH_TAG_EVERY_DELIVER,	///<每个频点都搜索
	EM_CSSHELL_SEARCH_TAG_EXTERNAL_DELIVER	///<只搜索传入频点的描述符，只对nit 描述符请求有效
}CSShellSearchTagMode_E;

/**
@brief 搜索方式,与java 类SearchManager中的搜索类型对应.对于搜索工厂来时，搜索方式和模式是非常必要的。
搜索方式，是从搜索总过程对表的请求策略来说的。搜索模式，则是从搜索总过应用对回调的节目数据的处理策略来说的
@see CSShellSearchMode_E
*/
typedef enum
{
	EM_CSSHELL_SEARCH_TYPE_PATPMT = 0,   ///< 单频点，列表，全频段搜索
	EM_CSSHELL_SEARCH_TYPE_NITSDT =1,  
	EM_CSSHELL_SEARCH_TYPE_NITPATPMT = 2,   ///< 网络搜索
	EM_CSSHELL_SEARCH_TYPE_SATELLITE = 3,   ///卫星搜索不带NIT
	EM_CSSHELL_SEARCH_TYPE_SATELLITE_NIT=4,   //卫星搜索带NIT
	EM_CSSHELL_SEARCH_TYPE_TRANSPONDER = 5,   ///频点搜索不带NIT
	EM_CSSHELL_SEARCH_TYPE_TRANSPONDER_NIT= 6,  //频点搜索带NIT
	EM_CSSHELL_SEARCH_TYPE_NITSDT_EX = 7,
	EM_CSSHELL_SEARCH_TYPE_SDT = 8
 
///< 快速搜索以来NIT_Other,SDT_other
}CSShellSearchType_E;

/**@brief 搜索模式*/
typedef enum
{
    EM_CSSHELL_SEARCH_MODE_NORMAL,      ///< 常规搜索，即一般情况下的从界面进入的搜索模式，默认为此模式
    EM_CSSHELL_SEARCH_MODE_NO_FILTER,     ///< 无过滤搜索方式，一般用于后门搜索，通过Java接口setFiltratorFlag可以启动该搜索模式
    EM_CSSHELL_SEARCH_MODE_SAU			///<sau搜索模式.这里sau模式是一个比较笼统的概念，sau模式是否过滤节目等由具体项目需求决定，
}CSShellSearchMode_E;

/**
@brief 搜索的私有数据解析回调
@param[in] hServiceNode DTV(USP)-Search buffer中的“当前待保存Service”
@param[in] pPrivateData 私有数据，格式由USP-Search定义。对应于SearchPrivateInfo结构中的m_pucPrivateInfo
@param[in] nDataLen 私有数据长度
@param[in] pUserData 用户数据，其值等于对应ShellCommonSearch_S结构中的m_pUserData字段，该字段由搜索工厂设置
@return TRUE表示过滤掉该Service, FALSE表示保留该Service
@see SearchPrivateInfo
*/
typedef BOOL (* OnCSShellSearchPrivateParse_F) (HCSHANDLE hServiceNode, PBYTE pbPrivateData, int nDataLen,void* pvUserData);

/**
@brief 将搜索到的Service列表以用户自己方式添加到数据容器
@param[in] phServiceHandle Service列表首址
@param[in] nHandleCount Service个数
@param[in] pUserData 用户数据，其值等于对应ShellCommonSearch_S结构中的m_pUserData字段，该字段由搜索工厂设置
@return TRUE表示有实际添加(即本函数返回后数据容器内容有变);FALSE表示没有实际添加有效数据
@note 本函数负责释放加入数据容器失败的ServiceHandle
*/
typedef BOOL (* OnCSShellSearchAddContainer_F) (HCSHANDLE *phServiceHandle, int nHandleCount,void* pvUserData);


/**
@brief搜索开始之前的回调，此时searchui句柄已经创建。但是应用上可能还需要设置自己的一些东西
@param[in] 无
@return TRUE表时用户设置自己的搜索参数成功，FALSE表示失败
@note 之所以提供该接口，是因为Search需求太多，要求的设置项在CSShellCommonSearch_S中无法完全包含。如果用户有自己额外的需求，则可以调用搜索的接口自己设置
*/
typedef BOOL (* OnCSShellSearchStart_F) (void);

/**
@brief用户主动排序的回调，当应用主动调用java的排序接口时被调用，这样可以满足用户选择是否重新排序的需求
@param[in] 无
*/
typedef void (* OnCSShellSearchSort_F) (void);

/**
@brief应用获取是否搜索出新节目回调
@param[in] 无
*/
typedef BOOL (* CSShellSearchHaveNewService_F) (void);


/**
本结构定义了搜索时用到的信息。其中的回调属于dtv search的间接回调。其直接回调在dtvm native中实现。dtvm native通过接口CSShellSearchGetFactory
，最后可获取CSShellCommonSearch_S,可得到间接回调。然后，在native中将无法处理的信息转发给间接回调。间接回调由shell实现。
*/
typedef struct _CSShellCommonSearch
{
    int                    m_nSize;                    ///< 本结构体的大小，必须赋值为sizeof(CommonSearch_S)
	void*                  m_pvUserData;                            ///< 用户数据，此参数将随着本结构中的函数指针一起传递给函数实现
	
    //NIT
    int			           m_nNITTagCount;                         ///< 需关注的NIT私有描述符个数
    BYTE		           m_abNITTags[CSSHELL_SEARCH_MAX_TAG_COUNT]; ///< 需关注的NIT私有描述符Tags
    OnCSShellSearchPrivateParse_F       m_fnNITParseCallback;                    ///< NIT描述符处理回调

    //SDT
    int			           m_nSDTTagCount;                         ///< 需关注的SDT私有描述符个数
    BYTE		           m_abSDTTags[CSSHELL_SEARCH_MAX_TAG_COUNT]; ///< 需关注的SDT私有描述符Tags
    OnCSShellSearchPrivateParse_F        m_fnSDTParseCallback;                    ///< SDT描述符处理回调

    //BAT
    WORD		           m_wMinBouquetID;                        ///< BAT过滤最小BouquetID,必须设置，如果不关注，该项可设置为0
    WORD                   m_wMaxBouquetID;                        ///< BAT过滤最大BouquetID,必须设置，如果不关注，该项可设置为0xffff
    BOOL                   m_bBouquetFilter;                       ///< 是否要启动BAT过滤
    int			           m_nBATTagCount;                         ///< 需关注的BAT私有描述符个数
    BYTE		           m_abBATTags[CSSHELL_SEARCH_MAX_TAG_COUNT]; ///< 需关注的BAT私有描述符Tags
    OnCSShellSearchPrivateParse_F         m_fnBATParseCallback;                    ///< BAT描述符处理回调

    //PMT
    OnCSShellSearchPrivateParse_F         m_fnPMTParseCallback;                    ///< PMT处理回调函数

    //CAT
    OnCSShellSearchPrivateParse_F         m_fnCATParseCallback;                    ///< CAT处理回调函数

    OnCSShellSearchAddContainer_F         m_fnAddContainer;                        ///< 以用户自定义方式添加Service到数据管理的函数
    
    int                    				m_nTimeOut;                             ///< @deprecated, 目前未使用，单个频点搜索超时以java或js应用设置的为准 
    CSShellSearchSaveMode_E  			m_eSaveMode;                            ///< 搜索存储方案

	OnCSShellSearchStart_F				m_fnBeforStart;		///<搜索开始之前，该回调将被执行

	CSShellSearchTagMode_E				m_eNitTagSearchMode;	///<搜索nit表描述符时，是每个频点都搜索，还是搜索到之后就不再搜索,默认为每个频点都搜索
	CSShellSearchTagMode_E				m_eBatTagSearchMode;	///<搜索bat表描述符时，是每个频点都搜索，还是搜索到之后就不再搜索，默认为搜索到之后就不再搜索
	SearchCallBack						m_fnDtvSearchCallback;	///<原始的dtv search回调，请慎用该回调，建议只有对dtv search非常熟悉的情况下才可使用。
																///<如果注册，该回调在其他所有回调发出之前先发出。请谨慎确定返回值，返回值非0的情况下，searchmanager不会再针对此回调做任何处理，直接返回。
	int m_nBouquetIDCnt;                  ///需要请求的BouquetID的个数，如果关注m_wMinBouquetID和m_wMaxBouquetID并给这两个变量进行了赋值，则需要将m_nBouquetIDCnt设置为0，
									///这种情况则采用BouquetID区间的方式进行BAT过滤
	WORD m_awBouquetID[CSSHELL_SEARCH_MAX_BOUQUETID_COUNT];   ///需要请求的BouquetID的值，必须和m_nBouquetIDCnt对应使用，如果m_nBouquetIDCnt赋值为0，则该数组不起作用，如果该数组和m_nBouquetIDCnt都进行了赋值
	                                                                                                             ///则不关注m_wMinBouquetID和m_wMaxBouquetID，配置时请注意，并严格按要求配置!!!!!!!!
	OnCSShellSearchSort_F					m_fnSortCallback;		///<应用主动排序回调函数			
	CSShellSearchHaveNewService_F		m_fnHaveNewService;		///<应用获取是否有新节目回调	
	BOOL 								m_bInitMainCustom; ///<设置为TRUE，则自动搜索策略加入SEARCH_NIT_MAIN_CUSTOM类型，用户自定义
	BOOL                                m_bNetworkidFilter;      ///<设置是否指定networkid来进行搜索,TRUE表示指定，FALSE表示不指定
	DWORD								m_dwNetworkId;      ///<设置指定的networkid                                 
}CSShellCommonSearch_S;

/**
@brief 创建搜索实例接口定义
@param[in] eSearchType 搜索方式
@param[in] eMode 搜索模式
@return 返回适用于该方式、该模式的搜索对象；返回NULL按默认流程处理。
*/
typedef const CSShellCommonSearch_S* (* CSShellSearchCreate_F )(CSShellSearchType_E eSearchType, CSShellSearchMode_E eMode);

/**
@brief 设置search工厂
@param[in] pFactory search工厂，Shell会通过该接口创建搜索相关参数及函数
@return 成功返回TRUE,失败返回FALSE
*/
BOOL CSShellSearchSetFactory(CSShellSearchCreate_F pfnFactory);

/**
@brief 获取当前搜索的句柄
@param 无
@return 如果在搜索过程中，则返回搜索句柄，否则返回NULL
@note 该函数一般由search的回调调用,包括直接回调和间接回调
*/
HCSHANDLE CSShellSearchGetUI();

#ifdef __cplusplus
}
#endif
/** @} */
#endif   
