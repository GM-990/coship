#ifndef _CS_QUICK_SEARCH_H_
#define _CS_QUICK_SEARCH_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"


typedef enum _CSQSMsgType_E
{	
	EM_MSG_QUICKSEARCH_START = 0x1000000,
	EM_MSG_QUICKSEARCH_NIT_COMPLETE, //表示NIT搜索完成
	EM_MSG_QUICKSEARCH_NIT_TIMEOUT, //表示NIT搜索超时
	EM_MSG_QUICKSEARCH_SEARCH_FINISH, //表示搜索完成
	EM_MSG_QUICKSEARCH_TUNE_LOST //表示锁频失败
}CSQSMsgType_E;

/**
@brief 添加快速搜索的回调函数

@param[in] nMsg 当收到EM_MSG_QUICKSEARCH_END消息，表示搜索完成。
@param[in] dwUserData 用户参数
@return 
@note 
*/
typedef void (*CSOnQuickSearchCallback_F)(int nMsg, DWORD dwUserData);

/**
@brief 是否需要把CSQuickSearchAddCallback增加的回调add成功的回调函数

@param[in] nMsg 当收到EM_MSG_QUICKSEARCH_END消息，表示搜索完成。
@param[in] dwUserData 用户参数
@return 返回TRUE，则CSQuickSearchAddCallback回调添加失败，如果本函数返回FALSE,则继续添加CSQuickSearchAddCallback的回调.只能注册一个回调函数
@note 
*/
typedef BOOL (*CSOnSearchIsNeedCallback_F)(DWORD dwUserData);

/**
@brief 进行快速搜索的初始化，在这个函数中注册tuner的回调

@return 成功返回TRUE，失败返回FALSE
*/
BOOL CSQuicksearchInit();

/**
@brief 添加快速搜索的回调函数

@param[in] fnInfoCallback 回调函数
@param[in] dwUserData1 用户参数1，跟fnInfoCallback匹配
@param[in] fnInfoCallback 回调函数
@param[in] dwUserData2 用户参数,跟fnInfoCallback匹配
@return 成功返回TRUE，失败返回FALSE
@note 
- 在添加回调的时候，会先调用fnInfoCallback这个的回调，
- 如果fnInfoCallback回调返回成功，则CSQuickSearchAddCallback添加的fnCallback添加不进去，如果fnInfoCallback回调返回失败，则CSQuickSearchAddCallback可以添加成功
*/
BOOL CSQuickSearchAddCallback(CSOnQuickSearchCallback_F fnCallback, DWORD dwUserData1, CSOnSearchIsNeedCallback_F fnInfoCallback, DWORD dwUserData2);

/**
@brief 停止搜索并且设置qs模块功能失效
@param[in] 
@return 成功返回TRUE，失败返回FALSE
@note 
*/
BOOL CSQuickSearchDisable();

/**
@brief 设置qs功能生效
@param[in] 
@return 成功返回TRUE，失败返回FALSE
@note 
*/
BOOL CSQuickSearchEnable();

/**
@brief 删除CSQuickSearchAddCallback添加的回调函数

@param[in] fnInfoCallback 回调函数
@return 成功返回TRUE，失败返回FALSE
@note 
*/
BOOL CSQuickSearchRemoveCallback(CSOnQuickSearchCallback_F fnCallback);

/**
@brief 控制后台更新

@param[in] bIsUpdate:控制后台更新的时候是否要更新节目属性;TRUE，表示需要更新，FALSE表示不更新
@return 
@note 
*/
BOOL CSQuickSearchSetIsUpdateServiceProperty(BOOL bIsUpdate);


#ifdef	__cplusplus
}
#endif

#endif 

