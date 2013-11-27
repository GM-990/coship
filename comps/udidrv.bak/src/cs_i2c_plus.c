/****************************************************************************/
/*
 * Filename:        cs_i2c_plus.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 -------------------------------------------------------------------------------
 ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
 -------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_i2c_plus.h"

/**
@brief  初始化I2C 模块 

@param[in] enmIndex I2C bus index
@param[in] enmSpeed: I2C bus speed
@return 见CSI2C_Error_t 的定义
@note  必须首先调用该模块
*/
CSI2C_Error_t CSI2CInitialize (CSHDII2CIndex enmIndex, CSI2C_SPEED_t enmSpeed)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}



/**
@brief  打开某条i2c 总线上的一个设备，返回一个句柄，通过该句柄与i2c 设备通讯.

@param[in] enmIndex I2C bus index
@param[in] wSlaveAddress I2C device slave address
@param[out] phI2CHandle   I2C handle
@return 见CSI2C_Error_t 的定义
@note  在读和写操作前必须调用该模块
*/
CSI2C_Error_t CSI2COpen(CSHDII2CIndex enmIndex, WORD wSlaveAddress, HCSHANDLE *phI2CHandle)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief   关闭一个I2C句柄

@param[in] hI2CHandle  I2C 句柄
@return 见CSI2C_Error_t 的定义	
@note  此功能是用来关闭一个开放的设备，并释放资源。 
*/
CSI2C_Error_t CSI2CClose(HCSHANDLE hI2CHandle)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  向指定的i2c 设备写指定长度的数据，并产生停止信号 。

@param[in] hI2CHandle I2C 句柄
@param[in] pucBuffer 数据缓冲区
@param[in] dwDataLength  写入数据长度
@return 见CSI2C_Error_t 的定义		
@note 
*/
CSI2C_Error_t CSI2CWriteWithStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  从指定的I2C 设备读取指定长度的数据，并产生停止信号 。

@param[in] hI2CHandle I2C handle
@param[in] dwDataLength Data length to read
@param[out]  pucBuffer Data buffer
@return 见CSI2C_Error_t 的定义	
@note 
*/
CSI2C_Error_t CSI2CReadWithStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  向指定的i2c 设备写指定长度的数据，但不产生停止信号 。

@param[in] hI2CHandle I2C handle
@param[in] pucBuffer  Data buffer
@param[in] dwDataLength: Data length to write
@return 见CSI2C_Error_t 的定义	
@note 
*/
CSI2C_Error_t CSI2CWriteWithoutStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  从指定的I2C 设备读取指定长度的数据，但不产生停止信号 。

@param[in] hI2CHandle I2C handle
@param[in] dwDataLength  Data length to read
@param[out]  pucBuffer Data buffer
@return 见CSI2C_Error_t 的定义
@note 
*/
CSI2C_Error_t CSI2CReadWithoutStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**	   
@brief  该接口从指定的I2C设备读取指定长度的数据，该函数包含两个操作:写要读的寄存器地址+读寄存器数据
@param[in] hI2CHandle I2C handle
@param[in/out]  psReadInfo 读操作信息，详见CSI2CReadInfo_S定义
@return 见CSI2C_Error_t 的定义
@note 
*/
CSI2C_Error_t CSI2CReadExt(HCSHANDLE hI2CHandle, CSI2CReadInfo_S *psReadInfo)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  发出访问指定I2C 总线的请求 。

@param[in] enmIndex I2C index
@param[in] dwMilliseconds  Timeout in ms
@return 见CSI2C_Error_t 的定义	
@note 
*/
CSI2C_Error_t CSI2CRequestBus(CSHDII2CIndex enmIndex, DWORD dwMilliseconds)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  释放指定I2C 总线的访问

@param[in] enmIndex  I2C总线索引
@return   见CSI2C_Error_t 的定义	
@note 
*/
CSI2C_Error_t CSI2CReleaseBus(CSHDII2CIndex enmIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief  

@param[in]  nSize 缓冲区长度
@param[out]  pcVer  存放版本信息的缓冲区首地址
@return  1> 如果第一个参数pchVer为空，返回版本信息的实际长度
  	     2> 如果第一个参数不为空，且函数调用成功，则返回实
  	     际获取的版本信息长度
  	     3> 如果失败，返回0	
@note 
*/
int CSI2CGetVersion(PCHAR pcVer, int nSize)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSI2C_Error_t Retcode = CSI2C_SUCCESS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

