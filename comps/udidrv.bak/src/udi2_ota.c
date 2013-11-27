/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_ota.c
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

#include "udi2_ota.h"

/**
@brief 启动wifi升级
@param[out] pbWifiUpdate: CSUDI_TRUE:表示需要进行wifi升级，CSUDI_FALSE:表示不需要进行wifi升级
@return 成功返回CSUDI_SUCCESS，失败CSUDI_FAILURE
@note:只有调用此接口后中间件才能接收到wifi升级消息CSUDIOTAWIFIMsg_E
@note:使用场景，中间件启动后调用此接口，通过pbWifiUpdate判断wifi是否需要升级。
     若需要升级，启动后台新线程实现与wifi模块交互,更新wifi软件和配置文件。
     并在更新过程中将更新相关消息通过注册的回调函数抛给中间件。
*/
CSUDI_Error_Code CSUDIOTAWIFIStart(CSUDI_BOOL *pbWifiUpdate)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 注册wifi升级回调函数
@param[in] fnWifiCallback: 回调函数指针，详见：CSUDIOTAWIFICakllback_F
@param[in] pvUserData: 用户传入自定义指针，可以为NULL
@return 成功返回CSUDI_SUCCESS，失败CSUDI_FAILURE
@note:
*/
CSUDI_Error_Code CSUDIOTAWIFIAddCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 删除wifi升级回调函数
@param[in] fnWifiCallback: 回调函数指针，详见：CSUDIOTAWIFICakllback_F
@param[in] pvUserData: 用户传入自定义指针，可以为NULL
@return 成功返回CSUDI_SUCCESS，失败CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIOTAWIFIDelCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**@brief 获取OTA信息接口

@param[in] eType 信息类型，详见CSUDIOTAInfoType_E
@param[out] value 指向存储获取信息的内存地址
@param[in] length value值的长度
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note MMCP会根据运营商选择的OTA方式，调用本接口获取特定类型的OTA信息。

@code
//参考实现(未考虑缓冲区溢出等异常)
//注意:以下两种情况，value表示的类型及含义略有不同，请按参考代码实现/使用即可。
if(eType == EM_UDIOTA_NOR_MANUFACTURER_CODE)
{
	//EM_UDIOTA_NOR_MANUFACTURER_CODE对应数据类型是int的
	//value为一个指向int的指针，其长度应该为sizeof(int)
	//把对应值存储到value指向的buffer中
	*value = (int)100;
}
else if(eType == EM_UDIOTA_NDS_BCA)
{
	//EM_UDIOTA_NDS_BCA对应数据类型是CSUDI_UINT8数组
	//value一个指向CSUDI_UINT8数组，其长度为length
	//把对应的数据存储到value指向的buffer中
	memcpy(value,s_aBCAData,sizeof(s_aBCAData));
}
@endcode
*/
CSUDI_Error_Code CSUDIOTAGetInfo(CSUDIOTAInfoType_E eType, void* value, int length)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**@brief 设置OTA信息接口

@param[in] eType 信息类型，详见CSUDIOTAInfoType_E
@param[in] value 指向存储获取信息的内存地址
@param[in] length value值的长度
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note MMCP会根据运营商选择的OTA方式，调用本接口设置特定类型的OTA信息。该信息的时效性由具体选项决定。

@code
//参考实现(未考虑缓冲区溢出等异常)
//注意:以下两种情况，value表示的类型及含义略有不同，请按参考代码实现/使用即可。
if(eType == EM_UDIOTA_NOR_MANUFACTURER_CODE)
{
	//EM_UDIOTA_NOR_MANUFACTURER_CODE对应数据类型是int的
	//value为一个指向int的指针，其长度应该为sizeof(int)
	//从value指向的buffer中获取该值
	int manu_code = *(int*)value;
}
else if(eType == EM_UDIOTA_NDS_BCA)
{
	//EM_UDIOTA_NDS_BCA对应数据类型是CSUDI_UINT8数组
	//value一个指向CSUDI_UINT8数组，其长度为length
	//从value指向的buffer中获取对应内容
	memcpy(s_aBCAData,value,length);
}
@endcode
*/
CSUDI_Error_Code CSUDIOTASetInfo(CSUDIOTAInfoType_E eType, void* value, int length)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**@brief 设置OTA信息.

普通OTA模式会通过该接口通知Loader需要启动OTA，并将必要信息存储，之后MMCP会随时重启终端，下次启动时直接进入Loader。
@param[in] pUrl 升级信息，描述升级的位置，如频点,IP地址等。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note NDS OTA模式时该接口无效，MMCP会自行完成NDS数据结构如FDCA等的修改，并在必要时重启，Booter需根据NDS规则在启动时判断这些结构
@note 目前支持的URL有：
- dvb://networkid.tsid.serviceid，如dvb://1000.100.10。
- dvbc://qamfreq.SymbolRate.Modulation，如dvbc://363000.6875.64
- http://ipurl/，如http://www.coship.com/otafile.bin。
- tftp://ipurl/，如tftp://ftp.coship.com/otafile.bin。
- wifi://upgrade?ipurl/, 如wifi://upgrade?http://www.downloadWifi.com/wifi.bin?ver=4.1.0.20&upgradeType=1
其中所有整数数据均为10进制。
@note URL后面还可能附带一些可选的升级参数，如dvbc://363000.6875.64?pid=100&tableid=254目前需支持的参数有：
- pid，表示升级数据所在pid，10进制整数，如pid=100。
- tableid，表示升级数据所在TableId,10进制整数，如tableid=254。
- md5,表示升级数据的md5校验值,10进制，如md5=2b166501e6b8a40911975c54d3c4297b。
- version,表示升级后的软件版本，10进制，如version=100。
- type，表示升级类型，字符串，如type=auto,type=manual。
@note 具体需要支持哪些类型的URL需根据项目及CA需求确定。
@note pUrl为CSUDI_NULL表示取消升级。
@note 该接口仅仅是设置OTA信息，完毕后并不执行重启动作。重启动作由MMCP应用显式调用
*/
CSUDI_Error_Code CSUDIOTASetOta(const char* pUrl)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

