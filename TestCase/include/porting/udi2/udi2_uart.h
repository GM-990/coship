/**@defgroup UART UART定义通用异步接收发送相关接口(一般来说是串口)

@brief UART模块对应头文件<udi2_uart.h>，定义了通用异步接收发送数据的接口，
一般来说，就是串口的数据接收及发送，但不排除其他接口的可能性。\n

@version 2.0.0 2009/04/01 初稿
@{
*/
#ifndef _UDI2_UART_H_
#define _UDI2_UART_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief UART相关错误代码值*/
enum
{
	CSUDIUART_ERROR_BAD_PARAMETER = CSUDI_UART_ERROR_BASE,	///< 参数错误
	CSUDIUART_ERROR_FEATURE_NOT_SUPPORTED,						      ///< 操作不支持 
	CSUDIUART_ERROR_TIMEOUT                                 ///< 超时
};

/**@brief 奇偶校验属性枚举值 */
typedef enum {
	EM_UDIURART_PARITY_NONE,		///< 不校验
	EM_UDIURART_PARITY_EVEN,		///< 偶数校验
	EM_UDIURART_PARITY_ODD,			///< 奇数校验
	EM_UDIURART_PARITY_NUM
} CSUDIUartParity_E;


/**@brief 串口属性结构*/
typedef struct
{
	int m_nBaudrate;		///< 波特率,常见的有115200,38400 等
	int m_nDataBits;		///< 数据位,常见的5,6,7,8 四种情况
	int m_nStopBits;		///< 停止位,常见的1, 1.5,2 三种情况
	CSUDIUartParity_E m_eParity;		///< 奇偶校验,详见CSUDIUartParity_E定义
}CSUDIUartAttribute_S;	

#define CSUDI_DEFAULT_UART_INDEX	(-1)	///< 默认的串口索引，目前仅使用默认串口

/**
@brief 获取该平台支持的串口个数
@param[out] pdwUartDeviceCount  该平台支持的串口个数
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIUartGetCount(unsigned int* pdwUartDeviceCount);

/**
@brief 获取指定串口的属性
@param[in] nUartIndex 串口索引值
@param[out] psUartAttribute  保存串口属性的地址，具体结构参看CSUDIUartAttribute_S数据结构。
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIUartGetAttribute(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute);

/**
@brief 设置指定串口的属性
@param[in] nUartIndex 串口索引值
@param[in] psUartAttribute  串口属性的地址，具体结构参看CSUDIUartAttribute_S数据结构。
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIUartSetAttribute(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute);

/**
@brief 从uart接口读取最多uLength字节数据
@param[in] nUartIndex 要操作的串口索引。
@param[in] pucBuffer 用来存放读到数据的缓存。
@param[in] uLength 缓冲区长度。
@param[in] nTimeout 超时的时间，两个字符之间的超时时间，单位毫秒。连续nTimeout未收到数据表示超时。
@return 返回实际读到的数据的长度，0表示没有读到任何数据。
@note 当发生超时或者缓存区被填满之后，该接口返回。
*/
int CSUDIUartRead(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout);

/**
@brief 从uart接口发送最多uLength字节数据

@param[in] nUartIndex 要操作的串口索引。
@param[in] pucBuffer 存放要发送数据的缓存。
@param[in] uLength 要发送数据的长度。
@param[in] nTimeout 超时的时间，两个字符之间的超时时间，单位毫秒。连续nTimeout未收到数据表示超时。
@return 返回实际发送的数据的长度，0表示没有发出任何数据。
@note 当发生超时或者缓存区数据全部被送出后，该接口返回。
*/
int CSUDIUartWrite(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout);

/**
@brief 清除已经从uart收到，尚存在缓冲区中没有被CSUDIUartRead读走的数据

@param[in] nUartIndex 要操作的串口索引。

@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIUartClear(int nUartIndex);


#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

