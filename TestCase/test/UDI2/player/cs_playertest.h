#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cs_testkit.h"
#include "udi2_typedef.h"
#include "udi2_input.h"
#include "udi2_error.h"
#include "udi2_player.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  ERROR_CONNECT_DEMUX   0x01 
#define  ERROR_TUNER  0x02

/*
*  测试用例中CSUDIPLAYERSetStream用到的各种类型的PID，
*  可以相或使用，如(EM_STREAM_VIDEO|EM_STREAM_AUDIO)同时设置音视频的pid
*/
typedef enum
{
	EM_STREAM_VIDEO = 1<<0,   //视频
	EM_STREAM_AUDIO = 1<<1,	  //音频
	EM_STREAM_PCR   = 1<<2,   //pcr
	EM_STREAM_SUBTITLE = 1<<3, //subtitle
	EM_STREAM_TELETEXT = 1<<4  //teletext
}PlayerTestStreamType_E;

typedef struct _UserData_s
{
	CSUDI_HANDLE * phSem;
	CSUDI_BOOL     bTestPass;
	int				nId;
}UserData_S;

void initPlayerCfg();

int PLAYER_searchDemuxID_WithIndex(int nDemxType,int index);
int PLAYER_searchDemuxID_Y(int nDemxType);
int PLAYER_searchDemuxID_N(int nDemxType);

int PLAYER_searchAudioID_WithIndex(int nAudioType,int index);
int PLAYER_searchAudioID_Y(int nAudioType);

int PLAYER_searchVideoID_WithIndex(int nVideoType,int index);
int PLAYER_searchVideoID_Y(int nVideoType);

int PLAYER_searchAudioID_N(unsigned int *pnAudioType);               
int PLAYER_searchVideoID_N(unsigned int *pnVideoType);          
void fnPlayerCallback(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData);
void fnPlayerCallback1(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData);
void fnPlayerCallback2(CSUDI_HANDLE hPlayer, CSUDIPlayerEventType_E eEvent, void *pvUserData);
CSUDI_HANDLE   PLAYER_openPlayer(int uDemuxType,int eAudStreamType,int eVidStreamType,int ePlayerType );
CSUDI_HANDLE  secondOpenPlayer(int uDemuxType,int eAudStreamType,int eVidStreamType,int ePlayerType,CSUDI_Error_Code * pnOpenError);
int  connectTuner(int nDumuxID,int nStreamIndex);
int PLAYER_searchVideoID_Y_Y(int support1,int support2);            
void initAVInfo(int nAudioId, int nVideoId);

CSUDI_Error_Code PLAYER_openPlayerWithType(unsigned int dwStreamType,int uDemuxType, int eAudStreamType, int eVidStreamType, int ePlayerType,  CSUDI_HANDLE *phPlayer);
void  PLAYER_iInitCallbackPara();
CSUDI_BOOL PLAYER_iIsSourceEventCallback(int nWaitTime);
CSUDI_BOOL PLAYER_iIsAudioSupportDecodeType(int nStreamType);
CSUDI_BOOL PLAYER_iIsVideoSupportDecodeType(int nStreamType);
#ifdef __cplusplus
}
#endif

#endif
