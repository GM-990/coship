/****************************************************************************/
/*                   CONEXANT PROPRIETARY AND CONFIDENTIAL                  */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Conexant Systems India Pvt Ltd. 2004-2008                */
/*                                Hyderabad, India                                */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        NXP_cs_clocktimer.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 * Author:          Santhosh Reddy K
 *
 ****************************************************************************/
/* $Id: nxp_cs_pcm.h 110991 2009-08-18 05:40:16Z adonia $
 ****************************************************************************/
#ifndef _PCMINCLUDE_H_
#define _PCMINCLUDE_H_

//#include "basetype.h"

extern bool CSPcmClip_Init(int nAudIndx);
extern bool CSPcmInject_Open(CS_CNXT_Inject_SubSystem *pInjectHandle, int nAudIndx);
extern bool CSPcmInject_Close(CS_CNXT_Inject_SubSystem *pInjectHandle);
extern bool CSPcmInject_Start(CS_CNXT_Inject_SubSystem *pInjectHandle);
extern bool CSPcmInject_Stop(CS_CNXT_Inject_SubSystem *pInjectHandle);
extern bool CSPcmInject_GetFreeBuffer(CS_CNXT_Inject_SubSystem *pInjectHandle, void ** ppvBuffer, unsigned  * puLength );
extern bool CSPcmInject_WriteComplete(CS_CNXT_Inject_SubSystem *pInjectHandle, unsigned uWrittenSize);

extern bool CSPcmPlay_Pause(int nAudIndx);
extern bool CSPcmPlay_Resume(int nAudIndx);
extern bool CSPcmPlay_Stop(int nAudIndx, int nMixerId);
extern bool CSPcmPlay_Start(int nAudIndx, u_int8 *nMixerId);
extern bool CSPcmPlaySetEndian(CS_CNXT_Inject_SubSystem *pInjectHandle, CSUDIINJECTERPcmEndian_E *pEnmEndian);
extern bool CSPcmPlaySetPCMParams(CS_CNXT_Inject_SubSystem *pInjectHandle, CSUDIINJECTERPcmStartParam_S* psPcmStart);
extern bool CSPcmPlayGetEndian(CS_CNXT_Inject_SubSystem *pInjectHandle, CSUDIINJECTERPcmEndian_E *pEnmEndian);
extern bool CSPcmPlayGetPCMParams(CS_CNXT_Inject_SubSystem *pInjectHandle, CSUDIINJECTERPcmStartParam_S* psPcmStart);
extern bool CSPcmInject_Flush(CS_CNXT_Inject_SubSystem *pInjectHandle);
extern bool CSPcmInjectClearBuffer(CS_CNXT_Inject_SubSystem *pInjectHandle);
extern PIPE_PIPELINE_OBJECT* cs_tm_get_live_pipe(int nAudIndx);
extern bool CSPCMPlay_SetVolume(int nAudIndx, int volume);
extern bool CSPCMPlay_Mute(int nAudIndx, bool bMute);
extern bool CSPCMPlay_SetChannel(int nAudIndx ,CSUDIAUDIOChannelType_E eChannelType);



//extern bool CSPcmPlayClearBuffer(int nAudIndx);


#endif


