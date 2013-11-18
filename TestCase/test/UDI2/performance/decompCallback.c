//decompCallback.c

#include "decompCallback.h"
#include "cs_assert.h"

DWORD g_dwCallbackCount = 0;
DecompressCallBack g_fnDecompCallback=NULL;

VOID BLRegisterDecompressCallback(DWORD dwCallBackCount, DecompressCallBack fnDecompCallbck)
{
	g_dwCallbackCount = dwCallBackCount;
	g_fnDecompCallback = fnDecompCallbck;
}

VOID BLDecompressCallback(DecompressCallBackType enmType,  PVOID pParam)
{	
	if(DECOMPRESS_START == enmType)
	{
		CSTRACE(INFO_LEVEL,"[BLDecompressCallback]DECOMPRESS_START\r\n");
	}
	else if(DECOMPRESS_END == enmType)
	{
		CSTRACE(INFO_LEVEL,"[BLDecompressCallback]DECOMPRESS_END\r\n");
	}
	else
	{
		CSTRACE(INFO_LEVEL,"[BLDecompressCallback]enmType = %d, pParam = %d\r\n", enmType, *(int*)pParam);
	}
}
