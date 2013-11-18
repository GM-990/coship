#ifndef CS_NDSVOD_H
#define CS_NDSVOD_H
#include "mmcp_typedef.h"
#include "udi2_tuner.h"

#include <stdio.h>


#ifdef __cplusplus
extern "C"
{
#endif

/**@brief 音频错误代码值*/
typedef enum
{
	CSNDSVOD_MSG_SUCCESS = 1,    ///< 获取成功
	CSNDSVOD_MSG_TIMEOUT,		 ///< 超时
	CSNDSVOD_MSG_ERROR 			 ///< 失败
}CSVODMSGType_E;

/**
@brief 获取NDSVOD句柄
@return 成功返回句柄；失败则返回-1
*/
CSUDI_HANDLE  CSNDSVODGetHandle();
/**
@brief     获取VOD Baker  频点的回调函数
@param[in] hVod  NDS VOD句柄
@param[in] eType 事件类型  见CSVODMSGType_E
@param[in] psCableDeliver  Baker  频点信息 见CSUDITunerCableDeliver_S
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note   CSVODMSGType_E 为 CSNDSVOD_MSG_SUCCESS 时 ，psCableDeliver 为  CSUDITunerCableDeliver_S结构指针
        CSVODMSGType_E 为 CSNDSVOD_MSG_TIMEOUT 、 CSNDSVOD_MSG_ERROR时 ， psCableDeliver  为NULL.   
*/
typedef int (*CSNDSVODBakerCallback_F)(CSUDI_HANDLE hVod,
	CSVODMSGType_E  eType,
	const CSUDITunerCableDeliver_S * psCableDeliver,
	void * pvUserData);

/**
@brief      获取VOD Service Group 的回调函数
@param[in] hVod  voip句柄
@param[in] eType 事件类型  见CSVODMSGType_E
@param[in] nServiceGroup  VOD Service Group
@param[in] pvUserData 用户自定义数据，注册回调函数时传入的
@return  成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note   CSVODMSGType_E 为 CSNDSVOD_MSG_SUCCESS 时 ，nServiceGroup 为 VOD Service Group ID.
        CSVODMSGType_E 为 CSNDSVOD_MSG_TIMEOUT 、 CSNDSVOD_MSG_ERROR时 ，nServiceGroup 为-1.
*/
typedef int (*CSNDSVODSGCallback_F)(CSUDI_HANDLE hVod,
	CSVODMSGType_E  eType,
	int  nServiceGroup,
	void * pvUserData);
/**
@brief  请求VOD Baker  频点， 异步接口。
@param[in] hVod  voip句柄
@param[in] fnBakerCallback  回调函数
@param[in] pvUserData 用户自定义数据
@return 成功返回TRUE；失败则返回FALSE
*/
BOOL CSNDSVODRequestQAMBarker(CSUDI_HANDLE  hVod, CSNDSVODBakerCallback_F fnBakerCallback ,void *  pvUserData);

/**
@brief  请求ServiceGroup ， 异步接口。
@param[in] hVod  voip句柄
@param[in] fnSGCallback  回调函数
@param[in] pvUserData 用户自定义数据
@return 成功返回TRUE；失败则返回FALSE
*/
BOOL  CSNDSVODRequestServiceGroup(CSUDI_HANDLE  hVod, CSNDSVODSGCallback_F fnSGCallback ,void *  pvUserData);
/**
@brief  中止 请求 , 可以中止请求ServiceGroup 或 Baker频点
@param[in] hVod  voip句柄
@param[in] fnSGCallback  回调函数
@param[in] pvUserData 用户自定义数据
@return 成功返回SUCCESS；失败则返回FALSE
@note 可以中止请求ServiceGroup 或 Baker频点，正常流程不需要调用。
*/
BOOL CSNDSVODCancel (CSUDI_HANDLE  hVod);


#ifdef __cplusplus
}
#endif

#endif


