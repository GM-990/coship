/**@defgroup COSHIPLOAD COSHIPLOAD 提供coship规范loader接口
@brief COSHIPBOOT模块对应头文件<csbl_coshipload.h>，定义了coship规范loader所有的功能及配置接口。

@version 1.0.0 2011/3/11  
@version 1.0.1 2011/4/21   对应bl_ver:2.2.0.0
@{
*/
#ifndef _CSBL_COSHIPLOAD_H_
#define _CSBL_COSHIPLOAD_H_

#ifdef  __cplusplus
extern "C" {
#endif

/**
@brief 从界面获取qam升级参数

@param[in] Param1 无效参数，可传NULL。
@param[in] Param2 无效参数，可传NULL。
@return 成功返回SUCCESS
*/
int CSBLGetUpgParaFromGuiQam(PVOID Param1, PVOID Param2);
/**
@brief 从界面获取cofdm升级参数

@param[in] Param1 无效参数，可传NULL。
@param[in] Param2 无效参数，可传NULL。
@return 成功返回SUCCESS
*/
int CSBLGetUpgParaFromGuiCofdm(PVOID Param1, PVOID Param2);
/**
@brief 从界面获取qpsk升级参数

@param[in] Param1 无效参数，可传NULL。
@param[in] Param2 无效参数，可传NULL。
@return 成功返回SUCCESS
*/
int CSBLGetUpgParaFromGuiQpsk(PVOID Param1, PVOID Param2);

/**
@brief tuner升级函数

@param[in] Param1 无效参数，可传NULL。
@param[in] Param2 无效参数，可传NULL。
@return 成功返回SUCCESS
@note qam、cofdm、qpsk均使用该函数升级
*/
int CSBLUpgradeTuner(PVOID Param1, PVOID Param2);

/**
@brief usb升级函数

@param[in] Param1 无效参数，可传NULL。
@param[in] Param2 无效参数，可传NULL。
@return 成功返回SUCCESS
@note usb升级从界面获取参数的函数暂未实现，在配置时可以传NULL
*/
int CSBLUpgradeUsb(PVOID Param1, PVOID Param2);

/**
@brief 旧版ip升级函数

@param[in] Param1 无效参数，可传NULL。
@param[in] Param2 无效参数，可传NULL。
@return 成功返回SUCCESS
@note 从界面获取参数的函数暂未实现，在配置时可以传NULL
*/
int CSBLUpgradeIpOld(PVOID Param1, PVOID Param2);

/**
@brief ip升级函数

@param[in] Param1 无效参数，可传NULL。
@param[in] Param2 无效参数，可传NULL。
@return 成功返回SUCCESS
@note 从界面获取参数的函数暂未实现，在配置时可以传NULL
*/
int CSBLUpgradeIp(PVOID Param1, PVOID Param2);


/**
@brief Coship规范loader入口函数

@return 如果成功函数不会返回;如果失败返回FAILURE
*/
int CSBLEntryCoshipLoad(void);



/**
@brief 数字太和规范loader入口函数

@return 如果成功函数不会返回;如果失败返回FAILURE
*/
int CSBLEntryUnitendLoad(void);


/*
功能：完成对下载数据的签字与解签字功能
输入参数：
Param1 ：保存OTA下载后签字数据信息的地址,默认类型为CSBLDataPos_S*
Param2 ：保存解签字后数据信息的地址,默认类型为CSBLDataPos_S*

返回值：
-2：参数错误；
-1：校验失败，文件错误；
0 ：数据校验成功。

*/


int CSBLDownLoadVerify(PVOID Param1, PVOID Param2);


#ifdef  __cplusplus
}
#endif

#endif



