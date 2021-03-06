/****************************************************************************/
/*
 * Filename:        tm_include.h
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 * Author:          Trident Design-In Team
 *
 ****************************************************************************/

#ifndef _TMINJECTINCLUDE_H_
#define _TMINJECTINCLUDE_H_

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
#include "tm_player.h"

#define MAX_INJECT_HANDLES 8
#define MAX_PLAYBACK_HANDLES 1

typedef struct UDI_INJECT_HNDL
{
	int uIndx;
	bool bInjecterOpen;
}UDI_INJECT_HNDL;

typedef enum CSTMINJECT_STATE
{
	E_INJECT_INIT,
	E_INJECT_CLOSE,
	E_INJECT_OPEN,
	E_INJECT_START,
	E_INJECT_STOP
}CSTMINJECT_STATE;

typedef  struct CS_CNXT_Inject_SubSystem
{
    u_int32       uIndex;
	bool          bInUse;
	int			  nDmxIndx;
	int			  nVidIndx;
	int			  nAudIndx;
	CSUDIINJECTERType_E 	eInjecterType;
	CSUDIINJECTERContentType_E eContentType;
   	CSTMINJECT_STATE 		eInjecterState;
	//CSUDIINJECTERReqDataCallback_F InjectCallbkFunc;
    CSUDIINJECTERPcmStartParam_S psPcmStartParams;
    CSUDIINJECTERPcmEndian_E pEnmEndian;
    CSUDIPESSYNCMode_S psSyncmode;
    CSUDIINJECTERBuffer_S psBuffer;
    CSUDIINJECTERStatus_S psStatus;
    CSUDIINJECTERIFrameParam_S iFrameParams;
   	PIPE_PIPELINE_OBJECT     *pInjectPipeObj;
    PIPE_DEMUX_OBJECT        *pDmxObject;
	PIPE_AUDIO_OBJECT	     *pAudObject;	
	PIPE_VIDEO_OBJECT 	     *pVidObject;
	u_int8 *pTsBuff;
	bool 	bTsBuffInUse;
	u_int32 uTsBufLen;
    CNXT_CBUF_HANDLE hPCMClipcbuf;
    PIPE_AUDIO_MIXER_ID uMixerId;
 }CS_CNXT_Inject_SubSystem;

typedef struct CS_CNXT_Inject_Config
{
    CS_CNXT_Inject_SubSystem hInjectHandle[MAX_INJECT_HANDLES];
    u_int32 count;
}CS_CNXT_Inject_Config;

extern CNXT_STATUS cs_tm_inject_init(void);
extern PIPE_PIPELINE_OBJECT * cs_tm_get_injecter_pipe(int dmxIndx, CSUDIINJECTERType_E eInjectType);
extern bool CS_start_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem);
extern bool CS_stop_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem);
extern bool CS_Open_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem, int nVidIndex);
extern bool CS_Close_Iframe_decode(CS_TM_Player_SubSystem *pPlayerSubsystem);

#endif


