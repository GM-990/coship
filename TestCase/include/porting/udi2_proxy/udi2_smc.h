#ifndef _UDI2_SMC_H_PROXY
#define _UDI2_SMC_H_PROXY

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "../udi2/udi2_smc.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDISMCGetCount(format, ...) CSUDISMCGetCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCGetCount_PROXY(int * pnSMCCount, const char *pcFuncName, const int nLine);

#define CSUDISMCOpen(format, ...) CSUDISMCOpen_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCOpen_PROXY(int nCardIndex, CSUDISMCCallback fnSMCCallback, const char *pcFuncName, const int nLine);

#define CSUDISMCClose(format, ...) CSUDISMCClose_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCClose_PROXY(int nCardIndex, const char *pcFuncName, const int nLine);

#define CSUDISMCReset(format, ...) CSUDISMCReset_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCReset_PROXY(int nCardIndex, unsigned char * pucAtr, int * pnAtrLength, CSUDI_BOOL bColdRst, const char *pcFuncName, const int nLine);

#define CSUDISMCSetParams(format, ...) CSUDISMCSetParams_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCSetParams_PROXY(int nCardIndex,const CSUDISMCParam_S * psSMCParams, const char *pcFuncName, const int nLine);

#define CSUDISMCGetParams(format, ...) CSUDISMCGetParams_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCGetParams_PROXY(int nCardIndex,CSUDISMCParam_S * psSMCParams, const char *pcFuncName, const int nLine);

#define CSUDISMCDataExchange(format, ...) CSUDISMCDataExchange_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCDataExchange_PROXY(int             nCardIndex, 
									unsigned char  *pucWriteData,
	                                int             nNumberToWrite,
	                                unsigned char  *pcResponseData,
	                                int            *pnNumberRead,
	                                unsigned char  *pucStatusWord, const char *pcFuncName, const int nLine);

#define CSUDISMCSend(format, ...) CSUDISMCSend_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCSend_PROXY(int             nCardIndex,
				     unsigned char  *pucWriteBuf,
				     int             nNumberToWrite,
				     int            *pnNumberWrite,
				     int             nTimeout, const char *pcFuncName, const int nLine);

#define CSUDISMCReceive(format, ...) CSUDISMCReceive_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCReceive_PROXY(int             nCardIndex,
				        unsigned char  *pucReadBuf,
				        int             nNumberToRead,
				        int            *pnNumberRead,
				        int             nTimeout, const char *pcFuncName, const int nLine);

#define CSUDISMCSetPPS(format, ...) CSUDISMCSetPPS_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCSetPPS_PROXY(int             nCardIndex,
	                    unsigned char  *pucWriteData,
                           int             nNumberToWriteLen,
	                    unsigned char  *pucResponseData,
                           int            *pnResponseDataLen, const char *pcFuncName, const int nLine);

#define CSUDISMCDetect(format, ...) CSUDISMCDetect_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCDetect_PROXY(int nCardIndex, const char *pcFuncName, const int nLine);

#define CSUDISMCActive(format, ...) CSUDISMCActive_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCActive_PROXY(int nCardIndex, const char *pcFuncName, const int nLine);

#define CSUDISMCDeactive(format, ...) CSUDISMCDeactive_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDISMCDeactive_PROXY(int nCardIndex, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
