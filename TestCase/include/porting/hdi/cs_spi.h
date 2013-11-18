/**@defgroup CSSPI[CSSPI]同洲项目专用SPI 驱动接口
@version 0.1 
@{
*/

#ifndef _CS_SPI_H_
#define _CS_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include  "CSHDI_typedef.h"


/**@brief SPI 模块接口的返回值定义*/
typedef enum
{
	CSSPI_SUCCESS,					/**< 成功*/
	CSSPI_FAILURE,					/**< 失败*/
	CSSPI_BUSY,						/**< 繁忙*/
	CSSPI_NOACK,					/**< 无确认*/
	CSSPI_NOT_SUPPORT,				/**< 不支持*/
	CSSPI_TIMOUT,					/**< 超时*/
	CSSPI_BAD_PARAM,				/**< 错误参数*/
	CSSPI_MAX_HANDLE,				/**< 需添加注释*/
	CSSPI_ALREADY_INIT,				/**< 已经初始化*/
	CSSPI_NOT_INIT					/**< 未初始化*/
}CSSPIError;

/**@brief SPI 总线的时钟频率定义*/
typedef enum
{
	CSSPI_SPEED_LOW,				/**< SPI总线的时钟频率等于40KHz*/
	CSSPI_SPEED_NORMAL,			/**< SPI总线的时钟频率等于100KHz*/
	CSSPI_SPEED_HIGHT				/**< SPI总线的时钟频率等于10MHz*/
}CSSPISpeed;

/**@brief SPI 总线的引脚定义*/
typedef struct
{
	int 	m_nCSIndex;					/**< SPI总线的片选对应的IO 口*/
	int 	m_nCLKIndex;				/**< SPI总线的时钟对应的IO 口*/
	int 	m_nDataInIndex;				/**< SPI总线的数据输入对应的IO 口*/
	int 	m_nDataOutIndex;				/**< SPI总线的数据输出对应的IO 口*/
	int 	m_nReserved;	
}CSSPIPinIndex;

/**@brief SPI 总线的参数定义*/
typedef struct
{
	 BOOL       m_bMSBFirst;		/*设置大端/小端模式.If set to TRUE MSB is sent first else LSB is sent.*/
   	 BOOL       m_bClkPhase;		/*对时钟相位的设置规定了处理器在何时发送和接收数据,If set to TRUE pulse will be in the first half cycle else in the 2nd half cycle.*/
   	 BOOL       m_bPolarity;		/*对时钟极性的设置决定了静止时SPI串行时钟的有效状态 ,If set to TRUE clock idles at logic 1 else at logic 0.*//*相位和极性是为了适应不同外设和线路的需要，因为有些外设在时钟信号的上升沿锁存数据，而有些外设则在时钟信号的下降沿锁存数据。*/
   	 DWORD   m_dwDataWidth;	/*设置数据位宽.Set the width of Tx and Rx buffer for data transfer.*/
    	 DWORD   m_dwBaudRate;		/*设置波特率.Baud rate of the SPI link.*/
    	 DWORD   m_dwGlitchWidth;	/*Value in microseconds specifying the maximum width of noise pulses to be suppressed. User can disable this option by writing '0' .*/
}CSSPIParams;


/**
@brief  初始化的SPI 模块 

@param[in] enmIndex SPI bus index
@param[in] nChipSelectIndex: SPI chipSelect GPIO index 
@param[in] enmSpeed: SPI bus speed
@return 见CSSPIError 的定义
@note  必须首先调用该模块
*/
CSSPIError CSSPIInitialize(CSHDISPIIndex enmIndex, CSSPIPinIndex sCSSPIPinIndex, CSSPISpeed enmSpeed);


/**
@brief   打开一个SPI句柄

@param[in] hSPIHandle  SPI 句柄
@return 见CSSPIError 的定义	
@note  此功能是用来打开一个开放的设备，并输出对应句柄。 
*/
CSSPIError CSSPIOpen( CSHDISPIIndex enmIndex, HCSHANDLE *phSPIHandle );


/**
@brief   关闭一个SPI句柄

@param[in] hSPIHandle  SPI 句柄
@return 见CSSPIError 的定义	
@note  此功能是用来关闭一个开放的设备，并释放资源。 
*/
CSSPIError CSSPIClose( HCSHANDLE hSPIHandle );


/**
@brief  Wirte data to SPI device 

@param[in] hSPIHandle SPI 句柄
@param[in] pwBuffer 数据缓冲区
@param[in] dwDataLength  写入数据长度
@return 见CSSPIError 的定义		
@note 
*/
CSSPIError CSSPIWriteData( HCSHANDLE hSPIHandle, WORD *pwBuffer,DWORD dwDataLength );


/**
@brief  Read data  from SPI device 

@param[in] hSPIHandle SPI handle
@param[in] dwDataLength Data length to read
@param[out]  pwBuffer Data buffer
@return 见CSSPIError 的定义	
@note 
*/
CSSPIError CSSPIReadData( HCSHANDLE hSPIHandle, WORD *pwBuffer,DWORD dwDataLength );


/**
@brief  set params to the SPI bus

@param[in] hSPIHandle SPI handle
@param[in] pSetParams  Data buffer
@return 见CSSPIError 的定义	
@note 
*/
CSSPIError CSSPISetParams( HCSHANDLE hSPIHandle, CSSPIParams  *pSetParams);


/**
@brief  get current params from the SPI bus

@param[in] hSPIHandle SPI handle
@param[out]  pGetParams Data buffer
@return 见CSSPIError 的定义
@note 
*/
CSSPIError CSSPIGetParams( HCSHANDLE hSPIHandle, CSSPIParams *pGetParams);


/**
@brief  puts an inited spi device into a stable shut down state and deletes its associated control structures.

@param[in] hSPIHandle SPI handle
@param[in] flag used to terminate the SPI bus forcibly
@return 见CSSPIError 的定义
@note 
*/
CSSPIError CSSPITerm(CSHDISPIIndex enmIndex, BOOL bForceTerm);


/**
@brief  Request SPI bus

@param[in] enmIndex SPI index
@param[in] dwMilliseconds  Timeout in ms
@return 见CSSPIError 的定义	
@note 
*/
CSSPIError CSSPIRequestBus(CSHDISPIIndex enmIndex, DWORD dwMilliseconds);


/**
@brief  释放SPI总线的访问

@param[in] enmIndex  SPI总线索引
@return   见CSSPIError 的定义	
@note 
*/
CSSPIError CSSPIReleaseBus(CSHDISPIIndex enmIndex);


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
int CSSPIGetVersion(PCHAR pcVer, int nSize);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

