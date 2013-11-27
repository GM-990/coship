/****************************************************************************/
/*                   Trident Multimedia Technologies (Shanghai) Co, LTD                                   */
/*                        SOFTWARE FILE/MODULE HEADER                                                        */
/*                 Copyright Trident Multimedia Technologies (Shanghai) Co, LTD 2004-200       */
/*                            All Rights Reserved                                                                         */
/****************************************************************************/
/*
 * Filename:        udi2_audio.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 * Author:          Alfred Chen 2010.11
 *
 ****************************************************************************/
#include "generic_include.h"
#include "udi2_aout.h"

extern CS_TM_AP_DEVICE_OBJECTS gTmApDeviceObj;
extern CNXT_SEM_ID gUdi2AudioSem[2];
//extern CS_TM_AP_DEVICE_OBJECTS gTmApDeviceObj;
extern CS_TM_PIPE_OBJECTS gTmPipeObject;
CSUDI_BOOL g_bHdmiMuteInOpMode = CSUDI_FALSE;

#define  MODULE_NAME   "CS_AOUT"
#define AV_LEVEL  DEBUG_LEVEL
#if 0
#define INVALID_PCM_STREAM_HANDLE   0
#define PCM_BUF_SIZE    (2 * ENCODED_PCM_BUFFER_SIZE * 1024) /*2*96*1024 Bye*/

#define IS_VALID_PCM_INDEX(nAudIndx)  \
	if(((nAudIndx)< 1)||((nAudIndx)> PCM_INJECT_INDEX)) \
	{  \
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"input index:%d PCM must use index %d\n", \
			nAudIndx,PCM_INJECT_INDEX); \
			return FALSE; \
	}
#endif
bool bIsEncDataRequired(PIPE_AUDIO_FORMAT AudioFormat)
{
    bool bEncodedDataOnSPDIF=FALSE,bEncodedDataOnHDMI=FALSE;	    
	int 	chain=0;

    if((gTmPipeObject.TridCoshipSetup[chain].eBypassAoutDevice&EM_UDIAOUT_SPDIF))
    {
        /*
            * Decide what to send on SPDIF and HDMI digital audio ports.
            * This section assumes SPDIF and HDMI ports are present only for the main output.
            */
        if ((AudioFormat == PIPE_AUDIO_FMT_DOLBY_DIGITAL) || 
            (AudioFormat == PIPE_AUDIO_FMT_DD_PLUS) ||
            (AudioFormat == PIPE_AUDIO_FMT_AAC)    ||
            (AudioFormat == PIPE_AUDIO_FMT_AAC_PLUS) )
        {
           bEncodedDataOnSPDIF = TRUE;
        }
        
    }
    
    if((gTmPipeObject.TridCoshipSetup[chain].eBypassAoutDevice&EM_UDIAOUT_HDMI))
    {
        
        if ((AudioFormat == PIPE_AUDIO_FMT_DOLBY_DIGITAL) || 
            (AudioFormat == PIPE_AUDIO_FMT_DD_PLUS) ||
            (AudioFormat == PIPE_AUDIO_FMT_AAC)    ||
            (AudioFormat == PIPE_AUDIO_FMT_AAC_PLUS) )
        {
            bEncodedDataOnHDMI = TRUE;
        }
    }

    if(bEncodedDataOnHDMI || bEncodedDataOnSPDIF)
    {
       return TRUE;
    }
    return FALSE;

}
bool IsHdmiSupportEncData(void)
{
    PIPE_VP_DEVICE_HDMI_DEV_CAPS  hdmiCaps;  
	PIPE_VP_DEVICE_HDMI_GET_PARAM_ATTRIB hdmiAttrib;
    bool bHDMIAC3support = FALSE;
    int index;
    cnxt_kal_memset(&hdmiCaps,0,sizeof(hdmiCaps));
    
    if(gTmVpDeviceObj.pHDDevice != NULL)      
    {
        if(hdmiCaps.uNumAudDescr)
        {
            PIPE_VP_DEVICE_HDMI_AUDIO_CAPABILITIES *pAudioCaps;

            for (index=0; index<hdmiCaps.uNumAudDescr; index++)
            {
                pAudioCaps = &hdmiCaps.audioCaps[index];
                switch(pAudioCaps->Format)
                {
                    
                    case HDMI_AUDIO_FORMAT_LPCM:
                        break;
                    case HDMI_AUDIO_FORMAT_AC3:
                        bHDMIAC3support = TRUE;
                        break;
                    case HDMI_AUDIO_FORMAT_MPEG1:
                        break;
                    case HDMI_AUDIO_FORMAT_MP3:
                        break;
                    case HDMI_AUDIO_FORMAT_MPEG2:
                        break;
                    case HDMI_AUDIO_FORMAT_AAC:
                        break;
                    case HDMI_AUDIO_FORMAT_DTS:
                    case HDMI_AUDIO_FORMAT_ATRAC:
                    default:
                        break;
                }
            }
        }
    }
    else
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL,"hdmi_get_device_capabilities error\n ");
    }

    return bHDMIAC3support;
}
CSUDI_Error_Code TM_AOUTSetMute(CSUDIAOUTDevices_E eAudioDevice,CSUDI_BOOL bMute)
{
    
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	    
    PIPE_AUDIO_PRESENTATION_OUTPUT_ATTRIB     AudioPre_OutputAttr;
    PIPE_STATUS Status;            
    PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObj = gTmApDeviceObj.pAudioPresentation[0];
    
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s eAudioDevice[%d] mute %d\n",__FUNCTION__,eAudioDevice,bMute);
    do
    {
        if((eAudioDevice<=0)||(eAudioDevice>(EM_UDIAOUT_RCA|EM_UDIAOUT_SPDIF|EM_UDIAOUT_HDMI)))
        {
            Retcode = CSUDIAOUT_ERROR_BAD_PARAMETER;
            break;
        }

        if((eAudioDevice&EM_UDIAOUT_RCA) == EM_UDIAOUT_RCA)
        {
            cnxt_kal_memset(&AudioPre_OutputAttr, 0, sizeof(AudioPre_OutputAttr));
            Status = pAPMObj->get_output_attrib(pAPMObj,PIPE_AUDIO_RCA_OUTPUT_0,&AudioPre_OutputAttr);
            if(Status != PIPE_STATUS_OK)                
            {
                Retcode = CSUDI_FAILURE;
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
                break;            
            }
            AudioPre_OutputAttr.Output.bMute = bMute;
            Status = pAPMObj->set_output_attrib(pAPMObj,PIPE_AUDIO_RCA_OUTPUT_0,&AudioPre_OutputAttr);
            if(Status != PIPE_STATUS_OK)                
            {
                Retcode = CSUDI_FAILURE;
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
                break;            
            }
        }
        if((eAudioDevice&EM_UDIAOUT_SPDIF) == EM_UDIAOUT_SPDIF)
        {
            cnxt_kal_memset(&AudioPre_OutputAttr, 0, sizeof(AudioPre_OutputAttr));
            Status = pAPMObj->get_output_attrib(pAPMObj,PIPE_AUDIO_SPDIF_OUTPUT_0,&AudioPre_OutputAttr);
            if(Status != PIPE_STATUS_OK)                
            {
                Retcode = CSUDI_FAILURE;
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Function :%s line: %d error\n",__FUNCTION__,__LINE__);
                break;            
            }
            AudioPre_OutputAttr.Output.bMute = bMute;
            Status = pAPMObj->set_output_attrib(pAPMObj,PIPE_AUDIO_SPDIF_OUTPUT_0,&AudioPre_OutputAttr);
            if(Status != PIPE_STATUS_OK)                
            {
                Retcode = CSUDI_FAILURE;
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Function :%s line: %d error\n",__FUNCTION__,__LINE__);
                break;            
            }
            
        }
        if((eAudioDevice&EM_UDIAOUT_HDMI) == EM_UDIAOUT_HDMI)
        {
            cnxt_kal_memset(&AudioPre_OutputAttr, 0, sizeof(AudioPre_OutputAttr));
            Status = pAPMObj->get_output_attrib(pAPMObj,PIPE_AUDIO_HDMI_OUTPUT_0,&AudioPre_OutputAttr);
            if(Status != PIPE_STATUS_OK)                
            {
                Retcode = CSUDI_FAILURE;
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Function :%s line: %d error\n",__FUNCTION__,__LINE__);
                break;            
            }
            AudioPre_OutputAttr.Output.bMute = bMute;
            Status = pAPMObj->set_output_attrib(pAPMObj,PIPE_AUDIO_HDMI_OUTPUT_0,&AudioPre_OutputAttr);
            if(Status != PIPE_STATUS_OK)                
            {
                Retcode = CSUDI_FAILURE;
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Function :%s line: %d error\n",__FUNCTION__,__LINE__);
                break;            
            }
        }

    }while(0);
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit Function :%s Retcode[%d]\n",__FUNCTION__,Retcode);
	return Retcode;
}

CSUDI_Error_Code CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice)
{
    int chain=0;
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	              
    CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;
    
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s eAudioDevice[%d]\n",__FUNCTION__,eAudioDevice);
    Cnxt_Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER);    
    do
    {
        if((eAudioDevice<=0)||(eAudioDevice>(EM_UDIAOUT_RCA|EM_UDIAOUT_SPDIF|EM_UDIAOUT_HDMI)))
        {
            Retcode = CSUDIAOUT_ERROR_BAD_PARAMETER;
            break;
        }
        
        if(CSUDI_SUCCESS != TM_AOUTSetMute(eAudioDevice,CSUDI_TRUE))
        {
            Retcode = CSUDI_FAILURE;
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
            break; 
        }
        
        if((eAudioDevice&EM_UDIAOUT_RCA) == EM_UDIAOUT_RCA)
        {
            gTmPipeObject.TridCoshipSetup[chain].AoutRCAmute  = 1;
        }
        if((eAudioDevice&EM_UDIAOUT_SPDIF) == EM_UDIAOUT_SPDIF)
        {
            gTmPipeObject.TridCoshipSetup[chain].Aoutspdifmute = 1; 
        }
        if((eAudioDevice&EM_UDIAOUT_HDMI) == EM_UDIAOUT_HDMI)
        {
            gTmPipeObject.TridCoshipSetup[chain].AoutHDMImute = 1;
        }
    }while(0);
    Cnxt_Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit Function :%s Retcode[%d]\n",__FUNCTION__,Retcode);
	return Retcode;
}


CSUDI_Error_Code CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice)
{
    
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	    
	int 	chain=0;        
    CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;

    
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s eAudioDevice[%d]\n",__FUNCTION__,eAudioDevice);
    Cnxt_Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER);    

    do
    {
        if((eAudioDevice<=0)||(eAudioDevice>(EM_UDIAOUT_RCA|EM_UDIAOUT_SPDIF|EM_UDIAOUT_HDMI)))
        {
            Retcode = CSUDIAOUT_ERROR_BAD_PARAMETER;
            break;
        }
        if(((eAudioDevice&EM_UDIAOUT_HDMI) == EM_UDIAOUT_HDMI) && g_bHdmiMuteInOpMode)
        {
            break;//if hdmi is set bypass and mute,here we won't allow app to set hdmi unmute
        }
        if(CSUDI_SUCCESS != TM_AOUTSetMute(eAudioDevice,CSUDI_FALSE))
        {
            Retcode = CSUDI_FAILURE;
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
            break; 
        }
        
        if((eAudioDevice&EM_UDIAOUT_RCA) == EM_UDIAOUT_RCA)
        {
            gTmPipeObject.TridCoshipSetup[chain].AoutRCAmute  = 0;
        }
        if((eAudioDevice&EM_UDIAOUT_SPDIF) == EM_UDIAOUT_SPDIF)
        {
            gTmPipeObject.TridCoshipSetup[chain].Aoutspdifmute = 0; 
        }
        if((eAudioDevice&EM_UDIAOUT_HDMI) == EM_UDIAOUT_HDMI)
        {
            gTmPipeObject.TridCoshipSetup[chain].AoutHDMImute = 0;
        }

    }while(0);
    Cnxt_Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit Function :%s \n",__FUNCTION__);
	return Retcode;
}
extern void audio_player_restart(void);
CSUDI_Error_Code CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	    
	int 	chain=0;
    CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;
           
    CSUDIAOUTMode_E ePreAoutMode;    
    CSUDIAOUTDevices_E ePreDecAoutDevice;
    CSUDIAOUTDevices_E ePreBypassAoutDevice;
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s eAudioDevice[%d]eMode[%d]\n",__FUNCTION__,eAudioDevice,eMode);
    Cnxt_Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER);    
    do
    {
        if(( eMode < EM_UDIAOUT_DECODE) || (eMode > EM_UDIAOUT_BYPASS ))
        {
            Retcode = CSUDIAOUT_ERROR_BAD_PARAMETER;
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
            break;                        
        }
        if((eAudioDevice<EM_UDIAOUT_RCA)||(eAudioDevice>(EM_UDIAOUT_RCA|EM_UDIAOUT_SPDIF|EM_UDIAOUT_HDMI)))
        {
            Retcode = CSUDIAOUT_ERROR_BAD_PARAMETER;
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
            break;                                    
        }
        if(((eAudioDevice&EM_UDIAOUT_RCA)==EM_UDIAOUT_RCA)&&(eMode == EM_UDIAOUT_BYPASS))
        {
            Retcode = CSUDIAOUT_ERROR_FEATURE_NOT_SUPPORTED;
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Into Function :%s line: %d error\n",__FUNCTION__,__LINE__);
            break;                                    
            
        }
        ePreAoutMode = gTmPipeObject.TridCoshipSetup[chain].eAoutMode;
        ePreDecAoutDevice = gTmPipeObject.TridCoshipSetup[chain].eDecAoutDevice;
        ePreBypassAoutDevice = gTmPipeObject.TridCoshipSetup[chain].eBypassAoutDevice;
        if(eMode == EM_UDIAOUT_DECODE)
        {
            gTmPipeObject.TridCoshipSetup[chain].eAoutMode = eMode;
            gTmPipeObject.TridCoshipSetup[chain].eDecAoutDevice |= eAudioDevice;            
            gTmPipeObject.TridCoshipSetup[chain].eBypassAoutDevice &= (~eAudioDevice);
        }
        else if(eMode == EM_UDIAOUT_BYPASS)
        {
            gTmPipeObject.TridCoshipSetup[chain].eAoutMode = eMode;
            gTmPipeObject.TridCoshipSetup[chain].eBypassAoutDevice |= eAudioDevice;
            gTmPipeObject.TridCoshipSetup[chain].eDecAoutDevice &= (~eAudioDevice);
        }

        if((ePreAoutMode == gTmPipeObject.TridCoshipSetup[chain].eAoutMode)&&
            (ePreDecAoutDevice == gTmPipeObject.TridCoshipSetup[chain].eDecAoutDevice)&&
            (ePreBypassAoutDevice == gTmPipeObject.TridCoshipSetup[chain].eBypassAoutDevice))
        {
            
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s the setting is as same as previous setting, ingore it!\n",__FUNCTION__);            
        }
        else
        {
            CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s call audio_player_restart!\n",__FUNCTION__);
            audio_player_restart();

        }
        
    }while(0);
    
    Cnxt_Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Exit Function :%s Retcode[%d]\n",__FUNCTION__,Retcode);
	return Retcode;
}

