/******************************************************************************
FileName:  ca_inject.c
Copyright (c) 2007， Shen Zhen Coship Electronics CO.,LTD。

Version:       Ver 1.0
Author:        liuzhenghua

File Description:
        File Description, Please add you description here.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2008-3-11  liuzhenghua      Create
*******************************************************************************/

/******************************************************************************
 *                                 Head file                                 *
 ******************************************************************************/
#ifndef CSCAINJECT_H
#define CSCAINJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "mmcp_os.h"

typedef enum _CaInjectCatCachePolicy
{
	CAT_CACHE_ONLY_CURRENT_TS = 0,		//只缓存当前频点的cat
	CAT_CACHE_ALL_TS		//缓存所有频点的cat
}CaInjectCatCachePolicy_E;

/*表注入策略，应用可将除CSCAINJECT_ENABLE_ALL_TABLE_INJECT之外的选项结合起来使用，控制表的注入*/
#define CSCAINJECT_ENABLE_ALL_TABLE_INJECT		(0)		///<允许注入所有的表，如果用户不设置，则为默认
#define CSCAINJECT_DISABLE_NIT_INJECT			(1<<0)	///<禁止注入NIT表
#define CSCAINJECT_DISABLE_BAT_INJECT			(1<<1)	///<禁止注入BAT表


/******************************************************************************
Function Name:    FOnCAHaveMsgToUser

Description:
      注册CA模块的回调函数定义，通过本函数可以向应用发送CA_ALARM、CA_INFO
      CA_COMMAND、CA_STATUS消息。
Input:
	DWORD nUserData,用户参数
	int nMsg,       消息类型，包括CA_ALARM、CA_INFO、CA_COMMAND、CA_STATUS
	DWORD dwPara1,  nMsg消息中的子消息类型
	DWORD dwPara2，消息中携带的参数
Output:
    	无
Return Value:
    	无
Other:
*******************************************************************************/
typedef int (*FOnCAHaveMsgToUser)(DWORD nUserData,int nMsg,DWORD dwPara1,DWORD dwPara2);
/******************************************************************************
Function Name:    CSRegisterCACallback

Description:
      注册CA模块的回调函数，通过本函数可以向应用发送CA_ALARM、CA_INFO
      CA_COMMAND、CA_STATUS消息。
Input:
	DWORD nUserData,用户参数
	FOnCAHaveMsgToUser hcbMsg 用户要注册的回调函数
Output:
    	无
Return Value:
    	无
Other:
*******************************************************************************/
void CSRegisterCACallback(DWORD nUserData,FOnCAHaveMsgToUser fnCallback);

/******************************************************************************
Function Name:    CSInitCA

Description:
      初始化CA模块，本接口只调用一次
	  向SI_cahce注册CA所需表的回调函数
Input:
	无
Output:
    	无

Return Value:
    	CA模块句柄
Other:
*******************************************************************************/
HCSHANDLE CSInitCA();

/******************************************************************************
Function Name:    CSDestoryCA

Description:
      销毁CA模块，本接口只调用一次

Input:
	hCAhandle CSInitCA返回的句柄
Output:
    无

Return Value:
    	无
Other:
*******************************************************************************/
void CSDestroyCA(HCSHANDLE hCAhandle);
/******************************************************************************
Function Name:    CSStartCA

Description:
      启动CA，在每次启动节目后启动CA

Input:
	
Output:
    无

Return Value:
    	成功返回TRUE，失败返回FALSE
Other:
*******************************************************************************/
BOOL CSStartCA( HCSHANDLE hCAhandle, HCSHANDLE hPath, int nOriNetworkID, int nTSID, int nServiceID, int nVideoPID, int nAudioPID  , BOOL bRestartCA);
/******************************************************************************
Function Name:    CSStopCA

Description:
      停止CA，在每次结束节目后停止CA

		a。 nChangePlay = 0:
		在stopca之后开始播放，之后pmt表回调肯定会到来。
		再次调用CSStartCA时，bPmtCome = FALSE;//等到pmt表到来才注入

		b nChangePlay = 1:
		在播放过程中pmt表相对之前的发生了变化。在stopca前后发生过pmt表版本号变化。
		再次调用CSStartCA时，bPmtCome = s_bPmtChanged; //只有发生过pmt表版本号变化才注入

		c. nChangePlay = 2：
		在播放过程时，发现pmt表中的音视频id同dm中保存的不一致，重新启动ca。之后除非pmt表发生版本号变化，否则不会再次请求。
		再次调用CSStartCA时，不改变bPmtCome的状态。因为pmt表不会再次到来了。

Input:
	
Output:
    无

Return Value:
    	成功返回TRUE，失败返回FALSE
Other:
*******************************************************************************/
BOOL CSStopCA( HCSHANDLE hCAhandle, int nVideoPID, int nAudioPID ,int nChangePlay);
/******************************************************************************
@deprecated 	过时的接口，目前没有项目要用

Function Name:    CSEnableCaInject

Description:
	停止cat表的向ca模块的注入
Input:
	void
Output:

Return Value:
	成功：TRUE
	失败：FALSE

Other:
*******************************************************************************/
BOOL CSEnableCaInject( );

/******************************************************************************
@deprecated 	过时的接口，目前没有项目要用
Function Name:    CSDisableCaInject

Description:
	开启cat表向ca模块的注入
Input:
	void
Output:

Return Value:
	成功：非0
	失败：0

Other:
*******************************************************************************/
BOOL CSDisableCaInject();

//通知cainject频点切换，需要清空上次存储的CAT表。
void CSCaInjectChangeFreq();

/******************************************************************************
Function Name:    CSCaInjectSetCatCachePolicy

Description:
	设置catinject采用的cat表缓存策略
Input:
	eCachePolicy, 缓存策略。建议空闲内存大时设置为CAT_CACHE_ALL_TS，能明显提高跨频点换台速度。否则设置为CAT_CACHE_ONLY_CURRENT_TS。
		默认策略为CAT_CACHE_ONLY_CURRENT_TS。
Output:

Return Value:
	成功：TRUE
	失败：FALSE

Other:
	该函数必须在CSInitCA之前调用。否则无效
*******************************************************************************/
BOOL CSCaInjectSetCatCachePolicy(CaInjectCatCachePolicy_E eCachePolicy);
	
/******************************************************************************
Function Name:	  CSCaInjectSetInjectPolicy

Description:
	设置catinject注入表给ca模块时的策略。目前可设置nit,bat的策略
Input:
	nInjectPolicy,表注入策略，默认为全部允许注入(CSCAINJECT_ENABLE_ALL_TABLE_INJECT)。 允许打开和关闭nit和bat注入
Output:

Return Value:
	成功：TRUE
	失败：FALSE

Other:
	该函数可以随时打开和关闭，不是线程安全的。建议用户在系统启动初始化ca后调用。
	因为cat,pmt，这两种表的注入流程对任意ca都是一样的，因此不允许用户设置。设置的nit和bat表的注入策略只对sicache/cainject有影响，
	用户仍然可以在其他地方自由注入表。
	需求来源：因为有的ca（如数码视讯），要求只注入主频点的nit表，提供此接口，可关闭cainject的nit表注入，而由shell注入
*******************************************************************************/
BOOL CSCaInjectSetInjectPolicy(int nInjectPolicy);


#ifdef __cplusplus
}
#endif

#endif

