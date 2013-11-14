/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_section.c
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

#include "udi2_section.h"


/**
@brief 请求Filter过滤数据

常用来过滤PSI、SI，也可用于过滤PES、RAW等数据,需要与CSUDIFILTERStart配合使用
@param[in] psRequestInfo Filter参数，详情请参见CSUDISECTIONRequestInfo_S
@param[in] fnSectionCallback 回调函数，当有符合条件的数据到来时，调用本函数通知，所有回调必须在同一线程且建议不要在调用线程中执行太多操作，详情请参见CSUDISECTIONCallback_F
@param[in] pvUserData 用户数据，可以为CSUDI_NULL
@param[out] phFilterHandle	用于接收返回的Filter句柄，返回值为0认为非法
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 
- 详细的过滤规则，请参考CSUDISECTIONRequestInfo_S结构的定义
- 对应的设备句柄在CSUDISECTIONRequestInfo_S结构中
- CSUDIFILTERAlloc成功后，调用CSUDIFILTERStart开始接收数据
- CSUDIFILTERAlloc可以多次调用，直到Filter通道数全部分配完为止

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERAllocate(const CSUDISECTIONRequestInfo_S * psRequestInfo, CSUDISECTIONCallback_F fnSectionCallback, const void * pvUserData ,CSUDI_HANDLE * phFilterHandle)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief 启动Filter过滤数据

@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- Start成功后，即开始接收、过滤相应数据
- 第一次调用后，再次(多次)调用将返回失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/    
CSUDI_Error_Code CSUDIFILTERStart(CSUDI_HANDLE hFilterHandle)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}




/**
@brief 停止Filter过滤数据,但并不释放Filter资源

仅停止，不释放资源
@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- Stop成功后，即停止接收、过滤相应数据
- 试图去停止一个未启动的Filter返回CSUDISECTION_ERROR_INVALID_STATUS
- 第一次调用后，再次(多次)调用将返回失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERStop(CSUDI_HANDLE hFilterHandle )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief 停止Filter接收数据,并释放Filter资源

@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- Free成功后，即停止接收、过滤相应数据
- 第一次调用后，再次(多次)调用将返回失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERFree(CSUDI_HANDLE hFilterHandle)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 修改Fillter设置

@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@param[in] psMMFilter Filter 参数详见 CSUDIFILTERMatchMask_S定义,若为CSUDI_NULL表示不更改此项
@param[in] bCRCCheck 是否检测CRC
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- 必须在Stop状态下才能修改filter参数，否则返回CSUDISECTION_ERROR_INVALID_STATUS
- 第一次调用后，再次(多次)调用将返回失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERModify(CSUDI_HANDLE hFilterHandle, const CSUDIFILTERMatchMask_S * psMMFilter,  CSUDI_BOOL bCRCCheck)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 设置Fillter的底层buffer大小

主要用于需要过滤大量数据,上层可能会来不及取走的情况
@param[in] hFilterHandle 由CSUDIFILTERAlloc分配的过滤器句柄
@param[in] nBufSize 要设置的底层buffer的大小，必须为2的n次方，最小1K，最大值根据各个平台差异不同
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置Fillter的底层buffer大小时必须让Fillter处于stop状态，否则返回CSUDISECTION_ERROR_INVALID_STATUS	

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERSetBufSize(CSUDI_HANDLE hFilterHandle,  int nBufSize)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


