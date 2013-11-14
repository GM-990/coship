#ifndef _TMPLAYERINCLUDE_H_
#define _TMPLAYERINCLUDE_H_
/* Linux Includes*/
#include <stdio.h>
#include <signal.h>
#include <kal.h>

/* Edwards/Pipeline Includes*/
#include "pipeline.h"
#include "stbcfg.h"
#include "kal.h"
#include "presentation.h"
#include "pr_audio.h"

/*Coship-Udi includes*/
#include "udi2_os.h"
#include "udiplus_debug.h"
#include "udi2_audio.h"
#include "udi2_aout.h"
#include "udi2_vout.h"
#include "udi2_screen.h"
#include "udi2_inject.h"
#include "udi2_osg.h"
#include "udi2_video.h"
#include "udi2_player.h"
#include "udi2_toolset.h"


#define MAX_CALLBK_FUNCS 32
#define MAX_PLAYER_HANDLES 5

typedef struct CSTMPLAYCALLBACK
{
	CSUDIPLAYERCallback_F Func;
	CSUDIPlayerEventType_E eEvent;
	void * pvUserData;
       bool bInuse;
}CSTMPLAYCALLBACK;
typedef enum CSTMPLAYER_STATE
{
	E_PLAYER_INIT,
	E_PLAYER_CLOSE,
	E_PLAYER_OPEN,
	E_PLAYER_READY,
	E_PLAYER_START,
	E_PLAYER_STOP,
	E_PLAYER_PAUSE
}CSTMPLAYER_STATE;
typedef  struct CS_TM_Player_SubSystem
{
   int8 DemuxIndx;
   int8 VideoIndx;
   int8 AudioIndx;
   u_int32 uVidPID;
   u_int32 uAudPID;
   u_int32 uPcrPID;
   u_int32 uSubTitlePID;
   u_int32 uTeletextPID;
   CSUDIPLAYERType_E PlayerType;
   CNXT_VIDEO_FORMAT VideoFormat;
   CNXT_VIDEO_FORMAT CurVideoFormat;
   u_int32 StreamType;
   u_int32 CurStreamType;
   CSTMPLAYCALLBACK PlayCallbackData[MAX_CALLBK_FUNCS];
   bool bPlayerInuse;
   bool bRadioStream;
   bool bVideoOnlyStream;
   bool bStreamModified;
   bool bIframeInject;
   bool bTrickMode;
   CSTMPLAYER_STATE ePlayerState;
   u_int8 nStreamCnt;
   CSUDIStreamInfo_S sStreamInfo[5];
   CSUDIPlaybackParam_S sPlaybackParams;
   PIPE_PIPELINE_OBJECT     *pPipeObj;
   PIPE_TUNER_OBJECT        *pTunerObj;
   PIPE_DEMUX_OBJECT        *pDemuxObj;/*MAX_DEMUX_OBJECTS*/
   PIPE_AUDIO_OBJECT        *pAudioObj;
   PIPE_VIDEO_OBJECT        *pVideoObj;
   u_int8   nMixerId;
   //tbd: add 线程保护mutex
 }CS_TM_Player_SubSystem;

typedef struct CS_CNXT_Player_Config
{   
   CS_TM_Player_SubSystem hPlayerHandle[MAX_PLAYER_HANDLES];
   u_int32 count;  
   //tbd: add 线程保护mutex
}CS_CNXT_Player_Config;

typedef struct UDI_PLAYER_HNDL
{
	int uPlayerindx;
	bool bPlayerOpen;
	int uStreamCount;
}UDI_PLAYER_HNDL;

CNXT_STATUS CS_TM_Player_Init(void);

#endif
