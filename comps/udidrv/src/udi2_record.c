/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_record.c
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

#include "udi2_record.h"
/**
@brief 注册录制回调函数

@param[in] hRecHandle 录制句柄，由CSUDIRECOpen()获得
@param[in] fnRecordCallback 录制回调函数，详见CSUDIRECCallback_F说明
@param[in] pvUserData  用户数据，此数据会原封不动返回给用户
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 至少支持注册的回调函数个数为32个，最大不限
@note 回调函数指针+回调函数类型+用户数据唯一标识一个回调,当两次注册它们完全一样时，第二次将返回CSUDIREC_ERROR_ALREADY_ADDED
@note 当注册的函数已经注册过将返回CSUDIREC_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECAddCallback(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback, void * pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 删除录制回调函数

@param[in] hRecHandle 录制句柄，由CSUDIRECOpen()获得
@param[in] fnRecordCallback 录制回调函数，详见CSUDIRECCallback_F说明
@param[in] pvUserData  用户数据，此数据会原封不动返回给用户
@return 成功返回CSUDI_SUCCESS；如果该回调函数根本就没有注册，将返回CSUDIREC_ERROR_NO_CALLBACK
@note 允许在回调函数中删除回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECDelCallback(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback,void * pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 创建一个录制器

@param[in] psChnl   录制通道的数据结构。
@param[out] phRecHandle 打开录制的句柄。
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECOpen(const CSUDIRecChnl_S * psChnl, CSUDI_HANDLE * phRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取录制支持的加密类型

@param[in] hRecHandle 录制句柄，由CSUDIRECOpen()获得
@param[out] peOutTypeArray   该路录制支持的加密方式(可能支持多种)，详见CSUDIRecEncryptType_E定义
@param[in]  nArraySize 传入peOutTypeArray数组的长度
@param[out]  pnActualCnt 该路录制支持的加密方式种数
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECGetSupportEncryptTypes(CSUDI_HANDLE hRecHandle,CSUDIStreamEncryptType_E* peOutTypeArray,int nArraySize,int* pnActualCnt)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 销毁一个录制器

@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 销毁一个录制器；与CSUDIRECOpen成对使用。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECClose(CSUDI_HANDLE hRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 启动录制

@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@param[in] psStartParam  启动录制参数详见CSUDIRecStartParam_S说明
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 由参数psStartParam指定需要录制的类型以及录制的节目
@note 调用此接口之前，需要调用CSUDIDEMUXConnectTuner连接hRecHandle对应的demux和tuner设备，并使用对应的tuner已经锁频成功。
@note 如果该录制所用的demux已经连接tuner，则此时不需要再次调用CSUDIDEMUXConnectTuner进行demux与tuner的连接
@see CSUDIDEMUXConnectTuner

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStart(CSUDI_HANDLE  hRecHandle,const CSUDIRecStartParam_S * psStartParam)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 停止录制

@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStop(CSUDI_HANDLE  hRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 暂停录制
@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECPause(CSUDI_HANDLE  hRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 恢复录制
@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 恢复录制后新录制的内容追加到暂停前的录制文件中

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECResume(CSUDI_HANDLE  hRecHandle)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 停止时移录制

@param[in] hRecHandle 正在时移录制的录制器句柄，由函数CSUDIRECOpen()获得
@param[in] bSwitchToFile 是否将时移录制的循环文件转换成普通文件
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStopTshift(CSUDI_HANDLE  hRecHandle,CSUDI_BOOL bSwitchToFile)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取录制时间信息

@param[in] hRecHandle 录制器句柄，由函数CSUDIRECOpen()获得
@param[out] psTimeInfo 录制的时间信息，详见CSUDIRecTimeInfo_S定义
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECGetRecTime(CSUDI_HANDLE  hRecHandle, CSUDIRecTimeInfo_S * psTimeInfo)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 录制文件切割

@param[in] szFileName 录制生成的文件
@param[in] unStartPos 剪切开始位置(单位为s)
@param[in] unEndPos 剪切的结束位置(单位为s)
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 切割操作需要将参数的时间位置nstartPos和nEndPos转换成文件中的物理位置，并将该段数据删除掉，切割后的文件可播放长度比切割前减少了(nEndPos-nStartPos)

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECFileCut(const char* szFileName,unsigned int unStartPos,unsigned int unEndPos)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

