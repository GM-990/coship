/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

/**@defgroup CSI2C[CSI2C]同洲项目专用I2C驱动接口
@brief 本模块主要定义了同洲项目专用I2C驱动接口
@brief I2C(Inter-Integrated Circuit)总线是一种由PHILIPS公司开发的两线式串行总线，用于连接微控制器及其外围设备。I2C总线产生于在80年代，最初为音频和视频设备开发，如今主要在服务器管理中使用，其中包括单个组件状态的通信。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.3 2008/11/5  第一次细化版本
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_I2C_PLUS_H_
#define _CS_I2C_PLUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include  "udiplus_typedef.h"
#include  "cs_hdicommon_plus.h"

/**@brief I2C模块接口的返回值定义*/
typedef enum
{
	CSI2C_SUCCESS,						/**< 成功*/
	CSI2C_FAILURE,						/**< 失败*/
	CSI2C_BUSY,							/**< I2C 总线繁忙*/
	CSI2C_NOACK,						/**< 没有应答*/
	CSI2C_NOT_SUPPORT,					/**< 不支持*/
	CSI2C_TIMOUT,						/**<操作 超时*/
	CSI2C_BAD_PARAM,					/**< 错误参数*/
	CSI2C_MAX_HANDLE,					/**< 暂时没使用*/
	CSI2C_ALREADY_INIT,					/**< 已经初始化*/
	CSI2C_NOT_INIT						/**< 未初始化*/
}CSI2C_Error_t;


/**@brief I2C总线的时钟频率定义*/
typedef enum
{
	CSI2C_SPEED_LOW,				/**< I2C总线的时钟频率等于40KHz*/
	CSI2C_SPEED_NORMAL,			/**< I2C总线的时钟频率等于100KHz*/
	CSI2C_SPEED_HIGHT,				/**< I2C总线的时钟频率等于400KHz*/
	CSI2C_SPEED_MIDDLE				/**< I2C总线的时钟频率等于300KHz*/
}CSI2C_SPEED_t;

/**@brief I2C读接口参数定义*/
typedef struct
{
	BYTE *pucRegAddrBuf;       /**< 存放读寄存器地址buffer*/
	DWORD dwAddrLen;	   /**< 寄存器地址buffer长度(字节数),即pucRegAddrBuf有效数据长度*/ 
	BYTE *pucRdBuf;		   /**<存放寄存器数据buffer,该buffer至少包含dwDataLength指定的大小内存*/
	DWORD dwDataLength;	   /**< 要读取寄存器数据的长度(字节数)*/ 
	BOOL bWrtStop;	   	   /**< 写操作是否包含停止位*/ 
	BOOL bRdStop;	       /**< 读操作是否包含停止位*/ 
}CSI2CReadInfo_S;

/**
@brief  初始化I2C 模块 

@param[in] enmIndex I2C bus index
@param[in] enmSpeed: I2C bus speed
@return 见CSI2C_Error_t 的定义
@note  必须首先调用该模块
*/
CSI2C_Error_t CSI2CInitialize (CSHDII2CIndex enmIndex, CSI2C_SPEED_t enmSpeed);



/**
@brief  打开某条i2c 总线上的一个设备，返回一个句柄，通过该句柄与i2c 设备通讯.

@param[in] enmIndex I2C bus index
@param[in] wSlaveAddress I2C device slave address
@param[out] phI2CHandle   I2C handle
@return 见CSI2C_Error_t 的定义
@note  在读和写操作前必须调用该模块
*/
CSI2C_Error_t CSI2COpen(CSHDII2CIndex enmIndex, WORD wSlaveAddress, HCSHANDLE *phI2CHandle);

/**
@brief   关闭一个I2C句柄

@param[in] hI2CHandle  I2C 句柄
@return 见CSI2C_Error_t 的定义	
@note  此功能是用来关闭一个开放的设备，并释放资源。 
*/
CSI2C_Error_t CSI2CClose(HCSHANDLE hI2CHandle);

/**
@brief  向指定的i2c 设备写指定长度的数据，并产生停止信号 。

@param[in] hI2CHandle I2C 句柄
@param[in] pucBuffer 数据缓冲区
@param[in] dwDataLength  写入数据长度
@return 见CSI2C_Error_t 的定义		
@note 
*/
CSI2C_Error_t CSI2CWriteWithStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength);

/**
@brief  从指定的I2C 设备读取指定长度的数据，并产生停止信号 。

@param[in] hI2CHandle I2C handle
@param[in] dwDataLength Data length to read
@param[out]  pucBuffer Data buffer
@return 见CSI2C_Error_t 的定义	
@note 
*/
CSI2C_Error_t CSI2CReadWithStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength);

/**
@brief  向指定的i2c 设备写指定长度的数据，但不产生停止信号 。

@param[in] hI2CHandle I2C handle
@param[in] pucBuffer  Data buffer
@param[in] dwDataLength: Data length to write
@return 见CSI2C_Error_t 的定义	
@note 
*/
CSI2C_Error_t CSI2CWriteWithoutStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength);

/**
@brief  从指定的I2C 设备读取指定长度的数据，但不产生停止信号 。

@param[in] hI2CHandle I2C handle
@param[in] dwDataLength  Data length to read
@param[out]  pucBuffer Data buffer
@return 见CSI2C_Error_t 的定义
@note 
*/
CSI2C_Error_t CSI2CReadWithoutStop(HCSHANDLE hI2CHandle, BYTE *pucBuffer, DWORD dwDataLength);

/**	   
@brief  该接口从指定的I2C设备读取指定长度的数据，该函数包含两个操作:写要读的寄存器地址+读寄存器数据
@param[in] hI2CHandle I2C handle
@param[in/out]  psReadInfo 读操作信息，详见CSI2CReadInfo_S定义
@return 见CSI2C_Error_t 的定义
@note 
*/
CSI2C_Error_t CSI2CReadExt(HCSHANDLE hI2CHandle, CSI2CReadInfo_S *psReadInfo);

/**
@brief  发出访问指定I2C 总线的请求 。

@param[in] enmIndex I2C index
@param[in] dwMilliseconds  Timeout in ms
@return 见CSI2C_Error_t 的定义	
@note 
*/
CSI2C_Error_t CSI2CRequestBus(CSHDII2CIndex enmIndex, DWORD dwMilliseconds);

/**
@brief  释放指定I2C 总线的访问

@param[in] enmIndex  I2C总线索引
@return   见CSI2C_Error_t 的定义	
@note 
*/
CSI2C_Error_t CSI2CReleaseBus(CSHDII2CIndex enmIndex);

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
int CSI2CGetVersion(PCHAR pcVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

