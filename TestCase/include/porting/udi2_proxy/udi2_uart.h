#ifndef _UDI2_UART_H_PROXY
#define _UDI2_UART_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_uart.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIUartGetCount(format, ...) CSUDIUartGetCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIUartGetCount_PROXY(unsigned int* pdwUartDeviceCount, const char *pcFuncName, const int nLine);

#define CSUDIUartGetAttribute(format, ...) CSUDIUartGetAttribute_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIUartGetAttribute_PROXY(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute, const char *pcFuncName, const int nLine);

#define CSUDIUartSetAttribute(format, ...) CSUDIUartSetAttribute_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIUartSetAttribute_PROXY(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute, const char *pcFuncName, const int nLine);

#define CSUDIUartRead(format, ...) CSUDIUartRead_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIUartRead_PROXY(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout, const char *pcFuncName, const int nLine);

#define CSUDIUartWrite(format, ...) CSUDIUartWrite_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
int CSUDIUartWrite_PROXY(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout, const char *pcFuncName, const int nLine);

#define CSUDIUartClear(format, ...) CSUDIUartClear_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIUartClear_PROXY(int nUartIndex, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
