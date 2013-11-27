/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_uart.c
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

#include "udi2_uart.h"
/**
@brief 获取该平台支持的串口个数
@param[out] pdwUartDeviceCount  该平台支持的串口个数
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIUartGetCount(unsigned int* pdwUartDeviceCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 获取指定串口的属性
@param[in] nUartIndex 串口索引值
@param[out] psUartAttribute  保存串口属性的地址，具体结构参看CSUDIUartAttribute_S数据结构。
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIUartGetAttribute(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 设置指定串口的属性
@param[in] nUartIndex 串口索引值
@param[in] psUartAttribute  串口属性的地址，具体结构参看CSUDIUartAttribute_S数据结构。
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIUartSetAttribute(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 从uart接口读取最多uLength字节数据
@param[in] nUartIndex 要操作的串口索引。
@param[in] pucBuffer 用来存放读到数据的缓存。
@param[in] uLength 缓冲区长度。
@param[in] nTimeout 超时的时间，两个字符之间的超时时间，单位毫秒。连续nTimeout未收到数据表示超时。
@return 返回实际读到的数据的长度，0表示没有读到任何数据。
@note 当发生超时或者缓存区被填满之后，该接口返回。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
int CSUDIUartRead(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief 从uart接口发送最多uLength字节数据

@param[in] nUartIndex 要操作的串口索引。
@param[in] pucBuffer 存放要发送数据的缓存。
@param[in] uLength 要发送数据的长度。
@param[in] nTimeout 超时的时间，两个字符之间的超时时间，单位毫秒。连续nTimeout未收到数据表示超时。
@return 返回实际发送的数据的长度，0表示没有发出任何数据。
@note 当发生超时或者缓存区数据全部被送出后，该接口返回。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
int CSUDIUartWrite(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 清除已经从uart收到，尚存在缓冲区中没有被CSUDIUartRead读走的数据

@param[in] nUartIndex 要操作的串口索引。

@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIUartClear(int nUartIndex)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

