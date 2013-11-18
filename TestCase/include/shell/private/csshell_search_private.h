
#ifndef CSSHELL_SEARCH_PRIVATE_H
#define CSSHELL_SEARCH_PRIVATE_H

#include "csshell_search.h"
#include "CSUSPDMCommonType.h"
#include "SearchUI.h"
#ifdef __cplusplus
extern "C"
{
#endif
	/**
	@brief 获取设置的搜索工厂
	@param 无
	@note 该接口被多次调用时，返回的是最后一次设置的搜索工厂
	*/
    CSShellSearchCreate_F CSShellSearchGetFactory();

	/**
	@brief 创建地面或者有线搜索ui
	@param eSearchType, 搜索的类型
	@param eMode, 搜索的模式
	@param psDeliver,搜索时传入的主频点
	@param nDeliverCount,主频点个数
	@param callback, search的回调
	@param nTimeout, 搜索超时，即搜索一个频点所花时间最大值
	@return 成功返回创建的searchUI,失败返回NULL
	@note callback实现在dtvm native中，在dtvm中调用CSShellSearchCreateUI。
	*/
	HCSHANDLE CSShellSearchCreateUI(CSShellSearchType_E eSearchType, CSShellSearchMode_E eMode, Deliver * psDeliver, int nDeliverCount, SearchCallBack callback, int nTimeout);

	/**
	@brief 创建卫星搜索ui
	@param eSearchType, 卫星搜索的类型。卫星搜索类型直接使用searchIU.h中定义的，
	@param eMode, 搜索的模式
	@param phData,搜索时传入的数据句柄。这些句柄可能是卫星句柄，也可能是频点句柄。search内部可以根据searchType来区分
	@param nDataCount, 传入的数据句柄个数
	@param callback, search的回调
	@param nTimeout,搜索超时。即搜索一个频点所花时间最大值
	@return 成功返回创建的searchUI,失败返回NULL
	@note callback实现在dtvm native中，在dtvm中调用CSShellSearchCreateSatelliteUI。
	*/
	HCSHANDLE CSShellSearchCreateSatelliteUI(CSShellSearchType_E eSearchType, CSShellSearchMode_E eMode, HCSHANDLE * phData, int nDataCount, SearchCallBack callback, int nTimeout);


	/**
	@brief 关闭搜索ui
	@param 无
	@return 成功返回TRUE,失败返回FALSE
	*/
	BOOL CSShellSearchCloseUI();


#ifdef __cplusplus
}
#endif
/** @} */
#endif   
