#ifndef _UDI2_PLAYER_H_PROXY
#define _UDI2_PLAYER_H_PROXY

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_record.h"
#include "udi2_error.h"
#include "../udi2/udi2_player.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSUDIPLAYERProbe(format, ...) CSUDIPLAYERProbe_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERProbe_PROXY(const char * pcURL, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERAddPlayerCallback(format, ...) CSUDIPLAYERAddPlayerCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERAddPlayerCallback_PROXY(CSUDI_HANDLE hPlayer,CSUDIPLAYERCallback_F fnPlayerCallback,CSUDIPlayerEventType_E eEvent,void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDIPLAYERDelPlayerCallback(format, ...) CSUDIPLAYERDelPlayerCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERDelPlayerCallback_PROXY(CSUDI_HANDLE hPlayer,CSUDIPLAYERCallback_F fnPlayerCallback,CSUDIPlayerEventType_E eEvent, void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDIPLAYERAddFilePlayerCallback(format, ...) CSUDIPLAYERAddFilePlayerCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERAddFilePlayerCallback_PROXY(CSUDI_HANDLE hPlayer, CSUDIFilePLAYERCallback_F fnPlayerCallback, void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDIPLAYERDelFilePlayerCallback(format, ...) CSUDIPLAYERDelFilePlayerCallback_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERDelFilePlayerCallback_PROXY(CSUDI_HANDLE hPlayer, CSUDIFilePLAYERCallback_F fnPlayerCallback, void * pvUserData , const char *pcFuncName, const int nLine);

#define CSUDIPLAYEROpen(format, ...) CSUDIPLAYEROpen_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYEROpen_PROXY(const CSUDIPlayerChnl_S * psPlayerChnl, CSUDIPLAYERType_E ePlayerType, CSUDI_HANDLE * phPlayer, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERClose(format, ...) CSUDIPLAYERClose_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERClose_PROXY(CSUDI_HANDLE hPlayer, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERSetStream(format, ...) CSUDIPLAYERSetStream_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERSetStream_PROXY(CSUDI_HANDLE hPlayer, const CSUDIStreamInfo_S * psStreamInfo, int nStreamCnt,const CSUDIPlaybackParam_S * psPlaybackParam, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERGetStream(format, ...) CSUDIPLAYERGetStream_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERGetStream_PROXY(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S * psStreamInfo,int * pnStreamCnt, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERGetPlaybackParam(format, ...) CSUDIPLAYERGetPlaybackParam_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERGetPlaybackParam_PROXY(CSUDI_HANDLE hPlayer, CSUDIPlaybackParam_S * psPlaybackParam, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERModifyStream(format, ...) CSUDIPLAYERModifyStream_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERModifyStream_PROXY(CSUDI_HANDLE hPlayer, CSUDIStreamInfo_S * psStreamInfo,int nStreamCnt, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERStart(format, ...) CSUDIPLAYERStart_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERStart_PROXY(CSUDI_HANDLE hPlayer, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERStop(format, ...) CSUDIPLAYERStop_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERStop_PROXY(CSUDI_HANDLE hPlayer, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERPause(format, ...) CSUDIPLAYERPause_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERPause_PROXY(CSUDI_HANDLE hPlayer, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERResume(format, ...) CSUDIPLAYERResume_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERResume_PROXY(CSUDI_HANDLE hPlayer, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERClear(format, ...) CSUDIPLAYERClear_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERClear_PROXY(CSUDI_HANDLE hPlayer, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERSetSpeed(format, ...) CSUDIPLAYERSetSpeed_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERSetSpeed_PROXY(CSUDI_HANDLE  hPlayer, CSUDIPlayerSpeed_E eSpeed, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERGetSpeed(format, ...) CSUDIPLAYERGetSpeed_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERGetSpeed_PROXY(CSUDI_HANDLE  hPlayer, CSUDIPlayerSpeed_E *peSpeed, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERSeek(format, ...) CSUDIPLAYERSeek_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERSeek_PROXY(CSUDI_HANDLE  hPlayer,const int nPosInSec, const CSUDIPlayPosition_E ePlayPosFlag, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERGetCurPosInSec(format, ...) CSUDIPLAYERGetCurPosInSec_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERGetCurPosInSec_PROXY(CSUDI_HANDLE  hPlayer, int * pnPosInSec, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERGetDuration(format, ...) CSUDIPLAYERGetDuration_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERGetDuration_PROXY(CSUDI_HANDLE  hPlayer, int *pnSeconds, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERGetCurPTS(format, ...) CSUDIPLAYERGetCurPTS_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERGetCurPTS_PROXY(CSUDI_HANDLE  hPlayer, CSUDIContentType_E eContentType, CSUDI_UINT64 *psPTS, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERGetPacketCount(format, ...) CSUDIPLAYERGetPacketCount_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERGetPacketCount_PROXY(CSUDI_HANDLE  hPlayer, CSUDIContentType_E eContentType, unsigned int * puPacketCnt, const char *pcFuncName, const int nLine);

#define CSUDIPLAYERTrickMode(format, ...) CSUDIPLAYERTrickMode_PROXY(format, ##__VA_ARGS__, __FUNCTION__, __LINE__)
CSUDI_Error_Code CSUDIPLAYERTrickMode_PROXY(CSUDI_HANDLE hPlayer,CSUDI_BOOL bTrickMode, const char *pcFuncName, const int nLine);

#ifdef __cplusplus
}
#endif

#endif
