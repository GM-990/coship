#ifndef JAVASHELL_STANDBY_H
#define JAVASHELL_STANDBY_H

#ifdef __cplusplus
extern "C" {
#endif


#include "mmcp_typedef.h"

/**待机状态枚举*/
typedef enum _CSStandbyState_E {
	EM_Standby_IN,       //进入待机状态
	EM_Standby_OUT      //退出待机状态
} CSStandbyState_E;


/**
@brief 获取待机状态回调函数定义
@param[in] state 详见CSStandbyState_E说明
@param[in] pvUserData 用户数据
*/
typedef void(*CSShellStandbyCallback_F) (CSStandbyState_E state, void* pUserData);

/**
@brief 注册获取待机状态的回调函数。
@param[in] fnCallback 指向回调函数的函数指针
@param[in] pvUserData 用户数据，可以为NULL
@note  可以允许注册多个回调
*/
DWORD CSShellAddStandbyCallback (CSShellStandbyCallback_F fnCallback ,void* pUserData);

/**
@brief 删除获取待机状态的回调函数。
@param[in] fnCallback 指向回调函数的函数指针
*/
void CSShellDelStandbyCallback(CSShellStandbyCallback_F fnCallback);


/******************************************************************************
Function Name:    CSShellStandby
	
Description:
	进行待机操作
	
Input:
	standby:待机模式参数   1：真待机   0：假待机

Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
BOOL CSShellStandby(BOOL standby);

/******************************************************************************
Function Name:    CSShellSetStandbyMode
	
Description:
	设置待机模式
	
Input:
	standby:待机模式参数   1：真待机   0：假待机

Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
void CSShellSetStandbyMode(int model);

/******************************************************************************
Function Name:    CSShellGetStandbyMode
	
Description:
	获取待机模式
	
Input:
	None		

Output:
   	standby:待机模式   1：真待机   0：假待机

Return Value:
    None
Other:
*******************************************************************************/
BOOL  CSShellGetStandbyMode();


/******************************************************************************
Function Name:    CSShellSetParamForRealStandby
	
Description:
	设置真待机模式的参数
	
Input:
	nGPIO:结束待机信号的GPIO口
	lowVol:待机所需方波低电平时长
	highVol:待机所需方波高电平时长
	
Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
void CSShellSetParamForRealStandby(int nGPIO,int lowVol,int highVol );

/******************************************************************************
Function Name:    CSShellReboot
	
Description:
	重启
	
Input:
    None	
	
Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
void  CSShellReboot();

#ifdef __cplusplus
}
#endif

#endif

