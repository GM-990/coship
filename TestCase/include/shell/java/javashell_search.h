#ifndef _SEARCH_INIT_PARAM_H_
#define _SEARCH_INIT_PARAM_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "CSUSPDMCommonType.h"
#include "mmcp_tuner.h"

//搜索存储选择
enum
{
	SEARCH_SAVE_OKEND,  //搜索成功结束才保存
	SEARCH_SAVE_ANY     //任何情况下都保存
};

//界面4种搜索方式
enum
{
	SEARCH_SINGLE,      //单频点
	SEARCH_NIT,         //NIT网络搜索
	SEARCH_LIST,        //列表搜索:
	SEARCH_FAST        //SDT快速搜索 
};

//搜索某个表中最大可请求的私有描述符个数
#define  MAX_SEARCH_PRVTAG              (8)
void javashell_searchGetDefaultDeliver(CSUDITunerCableDeliver_S *sDeliver);

//搜索的私有数据解析回调:
//参数pDatahServiceNode: USP-Search buffer中的“当前待保存Service”
//参数pPrivateData: 私有数据，格式由USP-Search定义
//参数nDataLen:     私有数据长度
//返回值: TRUE表示过滤掉该Service, FALSE表示保留该Service
typedef BOOL (* javashell_searchPrivateParse) (HCSHANDLE hServiceNode, PBYTE pPrivateData, int nDataLen);

//将搜索到的Service列表以用户自己方式添加到数据容器
//参数phServiceHandle: Service列表首址
//参数nHandleCount:    Service个数
//返回值: TRUE表示有实际添加(即本函数返回后数据容器内容有变)
//        FALSE表示没有实际添加有效数据
//注意: 本函数负责释放加入数据容器失败的ServiceHandle
typedef BOOL (* javashell_searchAddContainer) (HCSHANDLE *phServiceHandle, int nHandleCount);


//将搜索到的NIT上抛给用户决定是否以这个NIT为入口开始搜索
//参数pNITNode: SINetworkInfo 的指针， 用户可以通过这些信息来判断NIT表是否符合要求
//返回值: 1 表示不考虑该NIT，继续搜索别的NIT
//        0 表示以这个NIT为入口开始搜索
//注意: 本函数主要是针对需要自定义搜索NIT的情况
//typedef int (*FOnCheckNit) (SINetworkNode *pNITNode);


//搜索请求的私有数据
typedef struct _PrivateRequest
{
	//NIT
	int			  m_nNITTagCount;
	BYTE		  m_bNITTags[MAX_SEARCH_PRVTAG];
	javashell_searchPrivateParse  m_pNITParseCallback;

	//SDT
	int			  m_nSDTTagCount;
	BYTE		  m_bSDTTags[MAX_SEARCH_PRVTAG];
	javashell_searchPrivateParse  m_pSDTParseCallback;

	//BAT
	WORD		  m_wMinBouquetID;
	WORD          m_wMaxBouquetID;
	BOOL          m_bBouquetFilter;
	BOOL          m_bBouquetSearch;
	int			  m_nBATTagCount;
	BYTE		  m_bBATTags[MAX_SEARCH_PRVTAG];	
	javashell_searchPrivateParse  m_pBATParseCallback;

	//PMT
	javashell_searchPrivateParse  m_pPMTParseCallback;

	//CAT
	javashell_searchPrivateParse  m_pCATParseCallback;

}JAVASHELLPRIVATEREQUEST;


//NIT搜索时自定义的一些需求
#if 0
typedef struct _SearchNitCustom
{
    BOOL        m_bCustomNitSearch;         //下面的几个选项是否有效
    FOnCheckNit m_pCheckNit;                //搜索NIT时候的确认NIT是否有效的函数

    BOOL        m_bSearchPatTsIdMatch;      //是否进入频点搜索时候，要检测PAT的TsID是否与NIT的一致， 不一致则舍弃该频点 
    BOOL        m_bSearchSdtTsIdMatch;      //是否进入频点搜索时候，要检测SDT的TsID是否与NIT的一致， 不一致则舍弃该频点
    BOOL        m_bSearchSdtOriNetWorkMatch;//是否进入频点搜索的时候， 要检测SDT的OriNetWorkID是否与NIT的一致， 不一致则舍弃该频点

}SEARCHNITCUSTOM;
#endif


//搜索模块初始化需要设定的参数集
typedef struct _SearchInitParam
{
	//搜索请求私有数据
	JAVASHELLPRIVATEREQUEST   m_stPrivateRequest;

	//以用户自定义方式添加Service到数据管理
	javashell_searchAddContainer     m_pAddContainer;

	//单个频点超时时间:xxx_mili_sec
	int              m_nTimeOut;

	//搜索存储方案选择
	int              m_nSaveMode;

	//是否需要显示Service,即接收WM_POP_SERVICE(或SVSCALL_POP_SERVICE)
	BOOL             m_bPopService;

  //  SEARCHNITCUSTOM  m_stSearchNitCustom;

}JAVASHELLSEARCHINITPARAM,*LPJAVASHELLSEARCHINITPARAM;

/*========================================================
//函数原型:SearchInit()
//输入参数:无
//输出参数:无
//返回值:  无
//功能:    配置搜索初始化参数,不应该放在此头文件中-----tfl
=========================================================*/
//void SearchInit(void);

/*==========================================================
//函数原型:SVSInitSearch()
//输入参数:pInitParam 用户初始化参数
//输出参数:无
//返回值: void
//功能 :按具体运营商需求进行功能选择，初始化频道搜索功能
//注意事项:开机初始化调用一次 在USPInit中通过 SearchInit 调用
=========================================================== */
BOOL javashell_InitSearchParam(LPJAVASHELLSEARCHINITPARAM pInitParam);

/*========================================================
//搜索节目排序回调
//输入参数:无
//输出参数:无
//返回值:  无
//功能:    对节目进行排序
=========================================================*/
typedef void (*javashell_SearchSortServiceCallback)(void);

/*========================================================
//搜索保存节目回调
//输入参数:type      	节目类型
		   pParam 		返回参数
//输出参数:无
//返回值:  1  	 不保存当前节目
		   0     保存当前节目
//功能:    用于保存/过滤节目
=========================================================*/
typedef int (*javashell_SearchSaveServiceCallback)(CSUSPDMServiceType type, void* pParam);

/*========================================================
//搜索后门过滤回调
//输入参数:bIsFiltrateSearchData    TRUE 过滤 FALSE 不过滤
//输出参数:无
//返回值:  无
//功能:    设置搜索过滤标志
=========================================================*/
typedef void (*javashell_SetSearchFiltrationTagCallBack)(BOOL bIsFiltrateSearchData);

/*========================================================
//搜索设置手动搜索标志回调
//输入参数:bIsManulSearch    TRUE 手动搜索 FALSE 不是手动搜索
//输出参数:无
//返回值:  无
//功能:    设置搜索手动搜索标志
=========================================================*/
typedef void (*javashell_SetManulSearchTagCallBack)(BOOL bIsManulSearch);

/*========================================================
//记录搜索到的节目三要素回调
//输入参数:serviceHandle  节目句柄
//输入参数:isClearRecordInfo  TRUE:清掉以前记录的数据 并且参数serviceHandle无效   FALSE:记录数据
//输出参数:无
//返回值:  无
//功能:    记录搜索到的节目中第一个电视和广播节目的三要素
=========================================================*/
typedef void (*javashell_RecordSearchInfoCallBack)(HCSHANDLE serviceHandle, BOOL isClearRecordInfo);

/*========================================================
//获取搜索过程中记录的电视或广播的三要素
//输入参数: type  0:TV 1:RADIO
//输入参数: elementIndex  0:获取onid 1:获取tsid 2:获取serviceid
//输出参数:无
//返回值:  保存的要素值(onid or tsid or serviceid)
//功能:    获取搜索过程中记录的电视或广播的三要素
=========================================================*/
typedef int (*javashell_GetSearchRecordInfoCallBack)(int type, int elementIndex);

/*========================================================
//函数原型:RegisterSearchSaveCallBack()
//输入参数:searchSaveCallback 回调函数的实现
//输出参数:无
//返回值:  无
//功能:    注册搜索回调函数的实现(属于可配置项)
=========================================================*/
void javashell_RegisterSearchSaveCallBack(javashell_SearchSaveServiceCallback searchSaveCallback);

/*========================================================
//函数原型:RegisterSearchSortServiceCallBack()
//输入参数:searchSortServiceCallBack 回调函数的实现
//输出参数:无
//返回值:  无
//功能:    注册搜索回调函数的实现(属于可配置项)
=========================================================*/
void javashell_RegisterSearchSortServiceCallBack(javashell_SearchSortServiceCallback searchSortServiceCallBack);

/*========================================================
//函数原型:RegisterSetManulSearchTagCallBack()
//输入参数:setMaunulTag 回调函数的实现
//输出参数:无
//返回值:  无
//功能:    注册设置手动搜索标志的实现(属于可配置项)
=========================================================*/
void javashell_RegisterSetManulSearchTagCallBack(javashell_SetManulSearchTagCallBack setMaunulTag);

/*========================================================
//函数原型:RegisterSetSearchFiltrationTagCallBack()
//输入参数:setFiltrationTag 回调函数的实现
//输出参数:无
//返回值:  无
//功能:    注册设置后门过滤标志的实现(属于可配置项)
=========================================================*/
void javashell_RegisterSetSearchFiltrationTagCallBack(javashell_SetSearchFiltrationTagCallBack setFiltrationTag);

/*========================================================
//函数原型:RegisterRecordSearchInfoCallBack()
//输入参数:recordSearchInfo 回调函数的实现
//输出参数:无
//返回值:  无
//功能:    注册记录搜索到的节目三要素的实现(属于可配置项)
=========================================================*/
void javashell_RegisterRecordSearchInfoCallBack(javashell_RecordSearchInfoCallBack recordSearchInfo);

/*========================================================
//函数原型:RegisterGetSearchRecordInfoCallBack()
//输入参数:getSearchRecordInfo 回调函数的实现
//输出参数:无
//返回值:  无
//功能:    注册获取搜索到的节目三要素的实现(属于可配置项)
=========================================================*/
void javashell_RegisterGetSearchRecordInfoCallBack(javashell_GetSearchRecordInfoCallBack getSearchRecordInfo);
//暂时没用用到
HCSHANDLE  javashell_GetSearchUIHandle( void);

#ifdef __cplusplus
}
#endif

#endif

