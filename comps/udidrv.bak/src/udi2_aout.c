/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_aout.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
		 This module is used to control RCA, SPDIF, HDMI audio output, such as: 
			 audio output mute;
			 audio output unmute;
			 audio output passthrough mode;
			 audio output decoded mode;
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_aout.h"
#include "generic_include.h"

#define  MODULE_NAME   "CS_AOUT"

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

bool IsHdmiSupportEncData(void)
{
    PIPE_VP_DEVICE_HDMI_DEV_CAPS hdmiCaps;  
    bool bHDMIAC3support = FALSE;
    int index;
    cnxt_kal_memset(&hdmiCaps,0,sizeof(hdmiCaps));
#if 0    //commented by frank.zhou
    if((gTmVpDeviceObj.pHDDevice != NULL) &&
       (gTmVpDeviceObj.pHDDevice->hdmi_get_device_capabilities(gTmVpDeviceObj.pHDDevice,&hdmiCaps)==CNXT_STATUS_OK)
       )
#else
	if(gTmVpDeviceObj.pHDDevice != NULL)
#endif
    {
        //if(hdmiCaps.audioCaps.nAudioDescriptors) //commented by frank.zhou
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

//frank.zhou---------------------------------------------------------------------------------------------------
/**
@brief 设置指定音频输出通道为静音

@param[in] eAudioDevice  音频 输出通道，具体请参见CSUDIAOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户设定的指定音频通道上实现静音功能
@note 可以多次调用，但只有第一次有效
*/
CSUDI_Error_Code CSUDIAOUTMute(CSUDIAOUTDevices_E eAudioDevice)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 取消指定音频输出通道的静音状态

@param[in] eAudioDevice 音频输出通道，具体请参见CSUDIAOUTDevices_E
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户设定的指定音频设备上实现取消静音的功能
@note 在非mute状态下调用此接口无效
@note 可以多次调用，但只有第一次有效
*/
CSUDI_Error_Code CSUDIAOUTUnmute(CSUDIAOUTDevices_E eAudioDevice)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置指定输出通道的输出模式

@param[in] eAudioDevice 音频输出通道，具体请参见CSUDIAOUTDevices_E
@param[in] eMode 设备的输出模式，具体请参见CSUDIAOUTMode_E。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 缺省情况下，所有音频输出通道为解码后输出模式，并且左右声道只能是解码后输出模式。
@note 如果当前处于播放状态，调用该接口马上生效，可以允许声音短暂的停止
*/
CSUDI_Error_Code CSUDIAOUTSetOPMode(CSUDIAOUTDevices_E eAudioDevice,CSUDIAOUTMode_E eMode)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
