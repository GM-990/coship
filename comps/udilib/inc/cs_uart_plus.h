/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

/**@defgroup CSUART[CSUART]同洲项目专用UART驱动接口
@brief 本模块主要定义了同洲项目专用平台无关UART驱动接口
@brief UART (Universal Asynchronous Receiver/Transmitter) 通用异步收发器。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。
 
@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_UART_PLUS_H_
#define _CS_UART_PLUS_H_

#include  "udiplus_typedef.h"
#include  "cs_hdicommon_plus.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief 串口参数类型*/
typedef enum
{
	UART_STOP_BIT_NUM, 	/**< Type:T_UartStopBitNum */
	UART_DATA_LENGTH, 	/**< Type:T_UartDataLength*/
	UART_PARITY, 		/**<  Type:T_UartParityType */
	UART_BAUD_RATE 		/**<  Type:DWORD */
}T_UartParamType;

/**@brief 串口参数中的停止位定义*/
typedef enum
{ 
	UART_ONE_STOP_BIT, 	/**<1位停止位*/
	UART_TWO_STOP_BIT	/**< 2位停止位*/
}T_UartStopBitNum;


/**@brief 串口参数中的数据长度定义*/
typedef enum
{
	UART_7_BIT_DATA,	/**< 数据位为7bit*/
	UART_8_BIT_DATA	/**< 数据位为8bit*/
}T_UartDataLength;


/**@brief 串口参数中的奇偶校验定义*/
typedef enum
{
	UART_NO_PARITY,		/**< 无校验*/
	UART_ODD_PARITY,		/**< 奇校验*/
	UART_EVEN_PARITY		/**< 偶校验*/
}T_UartParityType;


/**@brief 串口参数中的工作模式定义,调用CSSetUartMode使用,CSSetUartMode已从头文件中删除*/
typedef enum
{
	UART_PRINT_MODE,		/**< 需添加说明*/
	UART_DATA_MODE,		/**< 需添加说明*/
	UART_OTHER_MODE		/**< 需添加说明*/
}T_UartModeType;


/**
@brief 对第nUartIndex路串口进行初始化。在使用串口前必须对该路串口进行初始化，如果用户要设置common clock的值则必须在初始化串口前调用函数CSHDIUartSetCustomParam。

@param[in] nUartIndex  要初始化串口的索引。对于不同的平台，串口的总数不一样，用作串口功能的ASC也不同，需视具体硬件设计而定，在新的硬件平台上进行开发时，请向硬件工程时师或者向驱动工程师询问具体的取值。
@param[in] dwBaud 串口的传输速率。一般为115200bps
@return
- CSHDI_ERROR_BAD_PARAMETER：输入参数不合法。
- CSHDI_ERROR_ALREADY_INITIALIZED：该路串口在之前已经被初始化过了。
- CSHDI_FAILURE：串口初始化失败。
- CSHDI_SUCCESS：串口初始化成功。
@note 串口初始化需在gpio初始化之后。
*/
CSHDI_Error_t CSHDIUartInitial  ( int  nUartIndex, UINT dwBaud);


/**
@brief 设置默认的串口。

@param[in] nUartIndex 要设置成默认串口的串口索引。
@return 如果成功则返回用户设定的串口索引，如果不成功，则返回用户先前设定的默认串口的索引，如果用户先前没有设置过默认的串口，则返回驱动初始化时默认的串口索引(2)。
@note 设置默认串口需在读写操作前。
*/
int CSHDIUartSetDefault ( int  nUartIndex);

/**
@brief 从串口nUartIndex读取u32Length个数据，读取的数据存放到 pucBuffer，超时时间为u32TimeOut。

@param[in] nUartIndex 要操作的串口索引。
@param[in] pucBuffer 用于存放读取到数据的缓存。
@param[in] u32Length 要读取数据的长度。
@param[in] u32TimeOut 超时时间，两个字符之间的超时时间。
@return 返回实际读到的数据的长度。
*/
ULONG  CSHDIUartReadFromWithTimeout ( int  nUartIndex ,PBYTE pucBuffer, ULONG u32Length, ULONG u32TimeOut);

/**
@brief 向串口nUartIndex发送u32Length个数据，要发送的数据存放在 pucBuffer中，超时时间为u32TimeOut。

@param[in] nUartIndex 要操作的串口索引。
@param[in] pucBuffer 存放要发送数据的缓存。
@param[in] u32Length 要发送数据的长度。
@param[in] u32TimeOut 超时的时间，两个字符之间的超时时间。
@return 返回实际发送的数据的长度。
*/
ULONG  CSHDIUartWriteToWithTimeout( int  nUartIndex ,PBYTE pucBuffer, ULONG u32Length, ULONG u32TimeOut);


/**
@brief 设置指定串口的参数，具体见T_UartParamType定义

@param[in] nUartIndex 串口索引
@param[in] enmType 参数类型
@param[in] pVaule 参数内容的首地址
@return 
	CSHDI_SUCCESS:设置成功
	CSHDI_FAILURE:设置失败
	CSHDI_ERROR_BAD_PARAMETER: 参数错误
*/
CSHDI_Error_t CSHDIUartSetParam(int  nUartIndex, T_UartParamType enmType, void *pVaule);

/**
@brief 获取指定串口的参数

@param[in] nUartIndex 串口索引
@param[in] enmType 参数类型
@param[out] pVaule  存放参数值的首地址
@return  
	CSHDI_SUCCESS:设置成功
	CSHDI_FAILURE:设置失败
	CSHDI_ERROR_BAD_PARAMETER: 参数错误
*/
CSHDI_Error_t CSHDIUartGetParam(int  nUartIndex, T_UartParamType enmType, void *pVaule);


/**
@brief 关闭指定串口,关闭后必须通过调用初始化接口方可恢复串口工作。

@param[in] nUartIndex 串口索引
@return
	CSHDI_SUCCESS: 关闭成功
	CSHDI_FAILURE:  关闭失败
	CSHDI_ERROR_BAD_PARAMETER: 参数错误
*/
CSHDI_Error_t CSHDIUartTerm(int nUartIndex);

/**
@brief 获取UART模块版本信息，显示格式为:
  [模块名]-[模块类型]-[版本号],如UART_OS_T_01.00.00, UART表示模块名,OS表示+os驱动，NOOS表示-os驱动，T表示test版本,01.00.00为版本号

@param[out] pchVer 存放版本信息的缓冲区首地址
@param[in] nSize  缓冲区长度
@return 
   1> 如果第一个参数pchVer为空，返回版本信息的实际长度
   2> 如果第一个参数不为空，且函数调用成功，则返回实
        际获取的版本信息长度
   3> 如果失败，返回0
*/

int CSUARTGetVersion( PCHAR pucVer, int nSize );


#ifdef __cplusplus
}
#endif

/** @} */

#endif



