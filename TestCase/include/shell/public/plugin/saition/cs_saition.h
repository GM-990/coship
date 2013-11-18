#ifndef _CS_SAITIONCOSHIP_H_
#define _CS_SAITIONCOSHIP_H_

#include "csshell_saition.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
@brief 	启动甩信功能前的初始化工作
@param[in]  eDeviceType: 甩信模块使用的传输设备,具体定义见CSSaitionTransDevType_E
@param[in] pvParam: 初始化时需要传入的参数，对于EM_SAITION_TANSDEVTYPE_BLUETOOTH传输设备，此
                                参数类型为CSSaitionBTHInfo_S*类型，表示蓝牙sdp注册信息；对于 EM_SAITION_TANSDEVTYPE_ETHERNET
                                传输设 备，此参数类型为*UINT型，表示机顶盒服务端端口号,
@return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
@note 
-参数pvParam为空时，函数返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionInit(CSSaitionTransDevType_E eDeviceType, void *pvParam);

/**
@brief 	启动甩信服务, 在CSSaitionInit之后调用
@param[in] uPort: 甩信服务端端口，此端口在带甩信cfg 的工程里面是从saitionconfig.properties配置文件中读取
@return  成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
@note 
- 如果需要启动甩信服务，需先调用CSSaitionInit函数后，再调用此函数就可以了。
*/
CSUDI_Error_Code CSSaitionStart();

/**
@brief 	停止甩信服务
@return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
@note 
- 
*/
CSUDI_Error_Code CSSaitionStop();

/**
@brief 	设置甩信生成URL, 此URL和甩信中的"拉"功能有关
@param[char*] pcBuildUrl: 甩信生成URL, pcBuildUrl必须是以'\0'结尾的ASCII编码字符串, 且字符串中间不能含有'\0\.
					   且pcBuildUrl的长度需小于(1024)， 否则不能将甩信生成URL正确设置到C模块里面
@return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
@note 
-参数pcBuildUrl为空时，函数返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionSetBuildURL(const char *pcBuildUrl);

/**
@brief 	获取甩信生成URL
@param[out] pcBuildUrl:  存放获取的甩信生成URL缓存
@param[in] nUrlLen:  pcBuildUrl缓存长度
@return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
@note 
-参数pcBuildUrl为空时函数返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionGetBuildURL(char* pcBuildUrl, int nUrlLen);

/**
@brief 	设置甩信中心URL, 此URL和甩信的"甩" 功能有关
@param[in] pcHomeUrl: 甩信中心URL, pcHomeUrl必须是以'\0'结尾的ASCII编码字符串, 且字符串中间不能含有'\0\.
				     且pcHomeUrl的长度需小于(1024)， 否则不能将甩信中心URL正确设置到C模块里面
@return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
@note 
-参数pcHomeUrl为空时函数返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionSetHomeURL(const char *pcHomeUrl);

/**
@brief 	获取甩信中心URL
@param[out] pcHomeUrl:  存放获取的甩信中心URL缓存
@param[in] nUrlLen:  pcHomeUrl缓存长度
@return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
@note 
-参数pcHomeurl为空时函数返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionGetHomeURL(char* pcHomeUrl, int nUrlLen);

/**
@brief 	此接口为了实现机顶盒通知手机执行某个过程。由其它模块调用将pcContent
		设置到甩信C模块，内容需双方协商决定，本接口只负责透传
@param[char*] pcContent:业务数据的请求参数, pcContent必须是以'\0'结尾的ASCII编码字符串, 且字符串中间不能含有'\0\.
					 且pcContent的长度需小于(2*1024)， 否则不能将内容正确设置到C模块里面,
@return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
@note 调用完此接口后，会通过异步发送数据的方式将此内容透传给手机。
-参数pcContent为空时函数返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionSendContent(const char *pcContent);

/**
@brief  向甩信模块注册一个回调
@param[in] fnOnSaitionEventCallBack:  要添加的回调
@param[in] dwUserData: 保存用户数据信息
@return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE	
*/
CSUDI_Error_Code CSSaitionAddCallback(CSOnSaitionEventCallBack_F fnOnSaitionEventCallBack, DWORD dwUserData);

/**
@brief  从甩信模块移掉一个callback
@param[in] fnOnSaitionEventCallBack:  要删除的回调
@param[in] dwUserData: 保存用户数据信息
@return   成功返回CSUDI_SUCCESS,失败返回CSUDI_FAILURE
*/
CSUDI_Error_Code CSSaitionRemoveCallback(CSOnSaitionEventCallBack_F fnOnSaitionEventCallBack, DWORD dwUserData);

#ifdef __cplusplus
}
#endif
/** @} */
#endif  



