/**@defgroup 通用异步接收发送相关接口(一般来说是串口)，csshell_uart，模块简称：CSSHELLUART
	
模块简称UART
@{
*/
#ifndef _CSSHELL_UART_H_
#define _CSSHELL_UART_H_

#include "udi2_uart.h"
#include "mmcp_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct 
{
	int m_nSize;	///<本结构大小，一定等于sizeof(CSShellUARTInitParam_S)

	/**
	@brief 获取该平台支持的串口个数
	@param[out] pdwUartDeviceCount  该平台支持的串口个数
	@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
	*/
	CSUDI_Error_Code (* m_fnGetCount)(unsigned int* pdwUartDeviceCount);
	
	/**
	@brief 获取指定串口的属性
	@param[in] nUartIndex 串口索引值
	@param[out] psUartAttribute  保存串口属性的地址，具体结构参看CSUDIUartAttribute_S数据结构。
	@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
	*/
	CSUDI_Error_Code (* m_fnGetAttribute)(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute);
	
	/**
	@brief 设置指定串口的属性
	@param[in] nUartIndex 串口索引值
	@param[in] psUartAttribute  串口属性的地址，具体结构参看CSUDIUartAttribute_S数据结构。
	@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值
	*/
	CSUDI_Error_Code (* m_fnSetAttribute)(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute);

	/**
	@brief 从uart接口读取最多uLength字节数据
	@param[in] nUartIndex 要操作的串口索引。
	@param[in] pucBuffer 用来存放读到数据的缓存。
	@param[in] uLength 缓冲区长度。
	@param[in] nTimeout 超时的时间，两个字符之间的超时时间，单位毫秒。连续nTimeout未收到数据表示超时。
	@return 返回实际读到的数据的长度，0表示没有读到任何数据。
	@note 当发生超时或者缓存区被填满之后，该接口返回。
	*/
	int (* m_fnRead)(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout);
	
	/**
	@brief 从uart接口发送最多uLength字节数据
	@param[in] nUartIndex 要操作的串口索引。
	@param[in] pucBuffer 存放要发送数据的缓存。
	@param[in] uLength 要发送数据的长度。
	@param[in] nTimeout 超时的时间，两个字符之间的超时时间，单位毫秒。连续nTimeout未收到数据表示超时。
	@return 返回实际发送的数据的长度，0表示没有发出任何数据。
	@note 当发生超时或者缓存区数据全部被送出后，该接口返回。
	*/
	int (* m_fnWrite)(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout);
	
	/**
	@brief 清除已经从uart收到，尚存在缓冲区中没有被CSUDIUartRead读走的数据
	@param[in] nUartIndex 要操作的串口索引。
	@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
	*/
	CSUDI_Error_Code (* m_fnClear)(int nUartIndex);

}CSShellUARTInitParam_S;

/**
@brief 设置 UART 接口参数
@param psParam  UART  Shell层要实现的接口
@成功返回TRUE,失败返回FALSE
*/
BOOL CSShellUARTSetParam(CSShellUARTInitParam_S * psParam);

/**
@brief 获取 UART 接口参数
@param 无
@成功返回CSShellUARTInitParam_S指针,失败返回NULL
*/
const CSShellUARTInitParam_S* CSShellUARTGetParam();

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



