/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_toolset.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_toolset.h"
/**
@brief 获取主芯片的ID。

@param[out] psChipID  用于存放获取到的Chip ID及其长度,请参见CSUDICHIPID_S。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 芯片若不支持获取ChipId该项需根据芯片情况与Coship协商处理,协商后若确认确实无法实现则返回CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED。
*/
CSUDI_Error_Code CSUDIGetChipID(CSUDICHIPID_S * psChipID)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}



/**
@brief 获取目标机器的序列号和Key。

@param[out] psSerialKey 用于存放获取到的序列号和Key，序列号和Key均是以0结尾的字符串，请参见CSUDISerialKey_S。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIGetSerialKey(CSUDISerialKey_S * psSerialKey)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 设置目标机器的序列号和Key。

@param[in] psSerialKey 用于存放设置的序列号和Key，请参见CSUDISerialKey_S。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 该接口目前仅使用m_szPairsKey 域。
*/
CSUDI_Error_Code CSUDISetSerialKey(const CSUDISerialKey_S * psSerialKey)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 软重启目标机器。

@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDISYSReboot(void)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 实现系统的关机。

@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 
- 执行成功，该函数可以不返回。
- 若不支持该功能可返回CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED。
*/
CSUDI_Error_Code CSUDISYSHalt(void)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief 设置待机属性。
@param[in] eAttribute 为CSUDISYSStandbyAttribute_E 类型，表示需要设置的待机属性类型
@param[in] pValue 为任意指针类型，具体类型需根据eAttribute 确定，可参考CSUDISYSStandbyAttribute_E 注释
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note
-若设置的待机属性为EM_SYS_STANDBY_ATTR_INFO ，集成厂家需根据项目需求决定待机后是否变化。( 如时间)
*/
CSUDI_Error_Code CSUDISYSSetStandbyAttribute(CSUDISYSStandbyAttribute_E eAttribute, void* pValue)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 使系统进入待机状态或者退出待机状态。

@param[in] bStandby 为CSUDI_TRUE时表示进入待机状态;为CSUDI_FALSE时表示退出待机状态。
@param[in] nStandbyTime 待机持续时间，单位为秒。即nStandbyTime秒后自动唤醒，为0表示不自动唤醒。仅当真待机时有效，假待机时忽略该参数。
@param[out] pbReal 为CSUDI_TRUE时表示是真待机;为CSUDI_FALSE时表示是假待机状态。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 
- 若为假待机，则该接口马上返回，并且*pbReal被置为CSUDI_FALSE，此情况下若需要退出待机状态需再次以bStandby=CSUDI_FALSE调用本接口。\n
- 若为真待机，则该接口在退出待机状态时返回，并且*pbReal被置为CSUDI_TRUE，此情况下不需要再以bStandby=CSUDI_FALSE调用本接口，且退出待机状态时所有状态（包括内存数据，设备的连接等）保持不变。
- 无论真假待机均会事先关闭音视频播放。
*/
CSUDI_Error_Code CSUDISYSStandby(CSUDI_BOOL bStandby,int nStandbyTime,CSUDI_BOOL *pbReal)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief 程序引导接口，Booter版专用，其它版无需实现

将被引导代码放置到指定位置后，使用该接口使得程序跳转到此地址。
@param[in] psExecInfo 存储被引导代码段信息
@return 该接口调用后直接进入被引导代码的主函数，无需返回。
@note 被引导代码段可能在内存中，也可能在Flash上，根据需求的差异，选择实现即可。
*/
void CSUDIToolsetExec(CSUDIToolsetExecInfo_S *psExecInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

