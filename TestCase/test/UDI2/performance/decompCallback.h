//decompCallback.h

#ifndef _DECOMP_CALLBACK_H_
#define _DECOMP_CALLBACK_H_

#include "cs_typedef.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef enum _DecompressCallBackType
{
	DECOMPRESS_START,//Decompress start,param NULL		
	DECOMPRESS_TIMESOFCALLBACK,//decompressing ,param:times of callback
	DECOMPRESS_END//Decompress end,param NULL
}DecompressCallBackType;

typedef  void(*DecompressCallBack) (DecompressCallBackType enmType,  PVOID pParam);

VOID BLRegisterDecompressCallback(DWORD dwCallBackCount, DecompressCallBack fnDecompCallbck);
VOID BLDecompressCallback(DecompressCallBackType enmType,  PVOID pParam);

#ifdef __cplusplus
}
#endif

#endif
