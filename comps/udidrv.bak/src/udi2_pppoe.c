/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_pppoe.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_pppoe.h"
/**
@brief 初始化pppoe模块

@param[in] psInitPara 初始化时需要传入的参数,为CSUDIPPPOEInitPara_S类型,堆栈大小和优先级传0的话我们的模块内部会取默认值
@return CSUDI_SUCCESS表示初始化成功，其他表示失败，失败类型见错误码值
@note  ACMAC和会话id从flash中读出(每次连接成功，要将这两个值保存在flash中)，第一次ACMAC传全FF,sessionid传0
*/
CSUDI_Error_Code  CSUDIPPPOEInit(CSUDIPPPOEInitPara_S *psInitPara)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 销毁pppoe模块

@param[in] nFlag kill掉pppd进程所采用的方式，gentle或者rude方式,0为gentle方式，1为rude方式，0表示杀掉该进程即可，1表示杀掉该进程及所有的子进程
@return CSUDI_SUCCESS表示销毁成功，其他表示失败，失败类型见错误码值
@note 该函数销毁了所有的pppoe的资源，如果要重新连接，需要重新初始化后再建立连接
*/
CSUDI_Error_Code  CSUDIPPPOECleanup (int nFlag)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 发起连接

@param[in] pcUserName 用户名
@param[in] pcPasswd 密码
@param[in] pcServiceName pppoe服务名称
@param[in] pcEthName 物理网卡名称，如"eth0"
@param[in] nTimeout 连接超时时间，单位:秒
@return CSUDI_SUCCESS表示连接成功，其他表示失败，失败类型见错误码值
@note 连接成功之后，获取到的ip地址、子网掩码和网关、dns等信息会以回调的形式通知给用户
*/
CSUDI_Error_Code  CSUDIPPPOEConnect(const char *pcUserName, const char *pcPasswd, char *pcServiceName, char *pcEthName, int nTimeout)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 断开连接

@param[in] nTimeout 断开连接的超时时间，单位:秒
@return CSUDI_SUCCESS表示断开连接成功，其他表示失败，失败类型见错误码值
@note 需要断开pppoe连接，可以调用该函数，以后再建立连接就直接调用CSUDIPPPOEConnect
*/
CSUDI_Error_Code  CSUDIPPPOEDisconnect(int nTimeout )
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获取pppoe服务名称

@param[out] pcSevicename 用来保存获取的pppoe服务名称
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
*/
CSUDI_Error_Code  CSUDIPPPOEGetServiceName( char *pcSevicename)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获取pppoe状态

@param[out] pnState 用来保存获取的pppoe的状态,状态的取值为CSUDIPPPOEState_E中定义的几种值
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
*/
CSUDI_Error_Code  CSUDIPPPOEGetState(int *pnState)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获取AC的MAC地址

@param[out] pcMAC 用来保存获取的AC的MAC地址
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
@note ac的mac地址的长度为6个字节，传入的buffer的空间要大于6个字节
*/
CSUDI_Error_Code  CSUDIPPPOEGetACMacAddr(char *pcMAC)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获取pppoe会话id

@param[out] pusSessionID 用来保存获取pppoe的会话id
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
@note 每次连接成功都会产生一个会话id，保存该会话id，下次连接的时候直接传入，会加快连接速度
*/
CSUDI_Error_Code  CSUDIPPPOEGetSessionID(unsigned short *pusSessionID)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获取pppoe版本号

@param[out] pcVer 用来保存获取pppoe的版本号
@return CSUDI_SUCCESS表示获取成功，其他表示失败，失败类型见错误码值
*/
CSUDI_Error_Code  CSUDIPPPOEGetVersion( char *pcVer)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 添加一个pppoe回调

@param[in] pvUserData 保存用户数据信息,将作为回调函数的dwUserData参数返回
@param[in] fnCallback 要添加的回调，当pppoe状态发生变化的时候会调用该函数
@return CSUDI_SUCCESS表示添加回调成功，其他表示失败，失败类型见错误码值
@note  可以添加多个回调 ,要求 pfCallback和dwUserData都相同的时候才不会添加进去，否则都添加
@note pppoe模块支持注册最小8个回调函数
*/
CSUDI_Error_Code CSUDIPPPOEAddCallback(CSUDIPPPOECallback_F fnCallback,void *pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 移除一个回调

@param[in] pvUserData 用户数据信息
@param[in] fnCallback 要移除的回调
@return CSUDI_SUCCESS表示移除回调成功，其他表示失败，失败类型见错误码值
@note  当存在和pfCallback和dwUserData均相同的时候才会移除,两个参数都匹配时才移除
*/
CSUDI_Error_Code CSUDIPPPOERemoveCallback(CSUDIPPPOECallback_F fnCallback,void *pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

