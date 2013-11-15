/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_audio.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
		 This module is used to get the audio decoders' properities supported by platform:
			 get the number of audio decoders;
			 get the capabilities of audio decoders;
			 get the status of audio decoders;
		 and do some audio decoder output control, such as:
			 audio decoding output stereo mode;
			 audio decoding output mute/unmute;
			 audio decoding output volume control;
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"
#include "udiplus_debug.h"

#include "udi2_audio.h"
#include "generic_include.h"

#define  MODULE_NAME   "CS_AUDIO"
#define AV_LEVEL  DEBUG_LEVEL
#define CS_MAX_AUD_MOD_NUM  MAX_AUDIO_OBJECTS
#define CS_MAX_VOLUME   31
#define CS_MIN_VOLUME   0

extern CS_TM_AP_DEVICE_OBJECTS gTmApDeviceObj;
CNXT_SEM_ID gUdi2AudioSem[CS_TRID_MAX_AUDIO];
bool g_bMuteFlag;
CSUDI_Volume_Cfg_S g_sVlmCfg = {0}; 


CNXT_STATUS CS_TRID_Audio_init(void)
{
	CNXT_STATUS Retcode = CNXT_STATUS_OK;    
	int 	chain=0;    
	char sem_name[20];
	do
	{
		/* create AP device */
		gTmApDeviceObj.pAudioPresentation[chain] = pipe_audio_presentation_open();
		if ( gTmApDeviceObj.pAudioPresentation[chain] == NULL )
		{
			pipe_audio_presentation_close ( gTmApDeviceObj.pAudioPresentation[chain]);
			gTmApDeviceObj.pAudioPresentation[chain] = NULL;
			CSDEBUG(MODULE_NAME,ERROR_LEVEL, "Failed create audio presentation \n" );
			Retcode = CNXT_STATUS_FAILURE;
			break;
		}
		else
		{
            for(chain=0;chain<CS_TRID_MAX_AUDIO;chain++)
            {
    			cnxt_kal_memset(&sem_name[0],0, 20);
    			sprintf(sem_name,"Udi2AudioSem%d",chain);                
    			Retcode=cnxt_kal_sem_create(1, sem_name, &(gUdi2AudioSem[chain]));
    			if(CNXT_STATUS_OK!=Retcode)
    			{
    				CSDEBUG(MODULE_NAME, ERROR_LEVEL, "sem_create  failed\n");
    				Retcode = CNXT_STATUS_FAILURE;          
    				break;          
    			}
            }
		}
		g_bMuteFlag = 0;
	}while(0);
  

	gTmPipeObject.TridCoshipSetup[0].eChannelType = EM_UDIAUDIO_CHANNEL_STEREO;
    gTmPipeObject.TridCoshipSetup[0].eAoutMode = EM_UDIAOUT_DECODE;
	return Retcode;
}

BOOL CSUDIAUDIOTerm()
{
	PIPE_STATUS enRet;

	//printf("start %s\n",__FUNCTION__);
	
	enRet = pipe_audio_presentation_close ( gTmApDeviceObj.pAudioPresentation[0]);
	if (enRet)
	{
		CSDEBUG(MODULE_NAME,ERROR_LEVEL,"fail to call pipe_audio_presentation_close %d\n",__LINE__);
		return FALSE;
	}

	//printf("exit %s\n",__FUNCTION__);

	return TRUE;
}

//frank.zhou---------------------------------------------------------------------------------------------------
/**
@brief 获取Audio Decoder设备的总数
@param[out] pnAudioDecCount 系统中 存在Audio Decoder设备的个数
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pnAudioDecCount的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOGetCount(int * pnAudioDecCount)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int 	chain=0;	
	Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER);  
	if(Retcode != CNXT_STATUS_OK)
	{
		UDIDRV_LOGI("%s KAL_SEM_GET FAILED %d \n", __FUNCTION__, Retcode);
		return CSUDI_FAILURE;
	}

	do
	{
		if(!pnAudioDecCount)
		{
			Retcode = CSUDIAUDIO_ERROR_BAD_PARAMETER;
			CSDEBUG(MODULE_NAME, ERROR_LEVEL, "[CS_PF] BAD parameter.\n");
			break;
		}
		*pnAudioDecCount =  CS_TRID_MAX_AUDIO;
		CSDEBUG(MODULE_NAME, AV_LEVEL, "pnAudioDecCount[%d]\n", *pnAudioDecCount);
		Retcode=CSUDI_SUCCESS;
	}while(0);

	Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_put FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief 获取指定Audio Decoder设备的能力

无论该设备是空闲或者被占用
@param[in] nAudioIndex Audio Decoder设备的索引
@param[out] psCapabilityInfo Audio Decoder设备能力信息,详情请参见CSUDIAUDIOCapability_S
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psCapabilityInfo结构的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOGetCapability(int nAudioIndex,CSUDIAUDIOCapability_S * psCapabilityInfo)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
	int 	chain=0;
	u_int32	i=0;
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	chain = nAudioIndex;
	CSDEBUG(MODULE_NAME, AV_LEVEL, "[CS_PF] %s begin.nAudioIndex=%d\n", __FUNCTION__,nAudioIndex);
	Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER); 
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}

	do
	{
		CSASSERT( nAudioIndex != -1 && nAudioIndex<CS_MAX_AUD_MOD_NUM);
		CSASSERT( psCapabilityInfo );
		
		if(nAudioIndex < 0|| nAudioIndex>=CS_MAX_AUD_MOD_NUM || !psCapabilityInfo)
		{
			Retcode = CSUDIAUDIO_ERROR_BAD_PARAMETER;
			break;
		}
		
		i=0;
		if (nAudioIndex == 0)
		{
			psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_AC3;
			psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_DTS;
			psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_MPEG1;
			psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_MPEG2;
	        //psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_PCM;
	        //psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_LPCM;
	        psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_MP3;
	        psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_MPEG_AAC,     ///<  Advanced audio coding. Part of MPEG-4  
	        psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_HE_AAC,       ///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE)  
	        psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_RAW_AAC,      
	        psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_AAC_PLUS_ADTS, ///<   AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format)  
	        psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_AAC_PLUS_LOAS, ///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream)  
	        psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_AC3_PLUS,     ///<   Dolby Digital Plus (AC3+ or DDP) audio 
	        psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_WMA_STD,     ///<     WMA Standard  
	//        psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_DTS_HD,       ///<   Digital Surround sound, HD 
	        psCapabilityInfo->m_sSupportDecode[i]=EM_UDI_AUD_STREAM_UNKNOWN;
	        
	        i=0;
	        psCapabilityInfo->m_sSupportByPass[i++]=EM_UDI_AUD_STREAM_AC3;
	        psCapabilityInfo->m_sSupportByPass[i++]=EM_UDI_AUD_STREAM_DTS;
	        psCapabilityInfo->m_sSupportByPass[i++]=EM_UDI_AUD_STREAM_MPEG_AAC,     ///<  Advanced audio coding. Part of MPEG-4  
	        psCapabilityInfo->m_sSupportByPass[i++]=EM_UDI_AUD_STREAM_HE_AAC,       ///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE)  
	        psCapabilityInfo->m_sSupportByPass[i++]=EM_UDI_AUD_STREAM_RAW_AAC,      
	        psCapabilityInfo->m_sSupportByPass[i++]=EM_UDI_AUD_STREAM_AAC_PLUS_ADTS, ///<   AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with ADTS (Audio Data Transport Format)  
	        psCapabilityInfo->m_sSupportByPass[i++]=EM_UDI_AUD_STREAM_AAC_PLUS_LOAS, ///<  AAC plus SBR. aka MPEG-4 High Efficiency (AAC-HE), with LOAS (Low Overhead Audio Stream)  
	        psCapabilityInfo->m_sSupportByPass[i++]=EM_UDI_AUD_STREAM_AC3_PLUS,     ///<   Dolby Digital Plus (AC3+ or DDP) audio 
	        psCapabilityInfo->m_sSupportByPass[i++]=EM_UDI_AUD_STREAM_DTS_HD, 
	        psCapabilityInfo->m_sSupportByPass[i]=EM_UDI_AUD_STREAM_UNKNOWN;
	    }
        else if (nAudioIndex == 1)
        {
            psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_PCM;
            psCapabilityInfo->m_sSupportDecode[i++]=EM_UDI_AUD_STREAM_LPCM;
            psCapabilityInfo->m_sSupportDecode[i]=EM_UDI_AUD_STREAM_UNKNOWN;

            i=0;
            psCapabilityInfo->m_sSupportByPass[i]=EM_UDI_AUD_STREAM_UNKNOWN;
        }
        Retcode=CSUDI_SUCCESS;
	}while(0);
	Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_put FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


/**
@brief 获取指定Audio Decode设备的状态信息

@param[in] nAudioIndex Audio Decoder设备的索引
@param[out] psStatus 用于存放Audio Decoder 状态信息的数据结构地址指针
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值，且psStatus结构的内容未定义

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOGetStatus(int nAudioIndex,CSUDIAUDIOStatus_S * psStatus)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	PIPE_AUDIO_ATTRIB AudioAttrib;
	int 	chain=0;
	PIPE_STATUS Status;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s\n",__FUNCTION__); 
	Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER);	
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}

	do
	{
		if((nAudioIndex < 0)||(nAudioIndex>=CS_TRID_MAX_AUDIO) || ( psStatus == NULL))
		{
			Retcode = CSUDIAUDIO_ERROR_BAD_PARAMETER;
			break;
		}
		chain = nAudioIndex;		
		Status = gTmPipeObject.hAudioObj[chain]->get_attrib(gTmPipeObject.hAudioObj[chain],&AudioAttrib);
		if(Status != PIPE_STATUS_OK)				
		{
			Retcode = CSUDI_FAILURE;
			break;			  
		}
		psStatus->m_nChannels = 2;
		switch(AudioAttrib.AudioCfg.Format)
		{
		case PIPE_AUDIO_FMT_AUTO:		   /* auto-detect the audio format */
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_MPEG2;	///<  音频流类型 
			if(1)//Attribs.StreamAttribs.MPEGAttribs.StereoMode <= DUAL_CHANNEL)
			{
				psStatus->m_nChannels =2;///<  通道数:1-mono,2-stereo 
			}
			else
			{
				psStatus->m_nChannels =1;///<  通道数:1-mono,2-stereo 
			}
			break;
		case PIPE_AUDIO_FMT_MPEG:			   /* MPEG-1 Layers I and II */
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_MPEG1;	///<  音频流类型 
			if(1)//Attribs.StreamAttribs.MPEGAttribs.StereoMode <= DUAL_CHANNEL)
			{
				psStatus->m_nChannels =2;///<  通道数:1-mono,2-stereo 
			}
			else
			{
				psStatus->m_nChannels =1;///<  通道数:1-mono,2-stereo 
			}
			break;
		case PIPE_AUDIO_FMT_MP3:			   /* MPEG-1 Layer III */
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_MP3;  ///<	音频流类型 
			break;
		case PIPE_AUDIO_FMT_DOLBY_DIGITAL:	   /* Dolby Digital, i.e. AC-3 */
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_AC3;  ///<	音频流类型 
			break;
		case PIPE_AUDIO_FMT_AAC:			   /* AAC using Low Complexity Profile */
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_MPEG_AAC;  ///<  音频流类型 
			break;
		case PIPE_AUDIO_FMT_AAC_PLUS:		   /* MPEG-4 High Efficiency (aacPlus), i.e. AAC with SBR */
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_HE_AAC;  ///<  音频流类型 
			break;
		case PIPE_AUDIO_FMT_DD_PLUS:		   /* Dolby Digital Plus. */
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_AC3_PLUS;  ///<  音频流类型 
			break;
		case PIPE_AUDIO_FMT_DTS:			   /* Digital Theater Systems */
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_DTS_HD;  ///<  音频流类型 
			break;
		case PIPE_AUDIO_FMT_WMA:			   /* WMA Format */
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_WMA_STD;  ///<	音频流类型 
			break;
		case PIPE_AUDIO_FMT_AVS:			   /* AVS Format*/
		case PIPE_AUDIO_FMT_MLP:			   /* Meridian Lossless Packing (DVD-Audio) */
		default:
			psStatus->m_eStreamType = EM_UDI_AUD_STREAM_UNKNOWN;
			break;
		}

		//commented by frank.zhou
		//need to fix in future
		//psStatus->m_nSampleRate = AudioAttrib.AudioCfg.SampleRate;	///<  音频采样率 
		
		psStatus->m_nBitWidth = AudioAttrib.AudioCfg.FormatCfg.RealAudioCfg.uNumSamples;
		psStatus->m_uFrameCount = 0;




		psStatus->m_eStreamType = EM_UDI_AUD_STREAM_AC3;
		
		
	}while(0);
	Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_put FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


/**
@brief 设置指定音频解码器的声道

@param[in] nAudioIndex Audio Decoder设备的索引
@param[in] eChannelType 用户需要输出的声道, 如LEFT, RIGHT, STEREO
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户指定的音频解码器上，切换模拟音频输出的声道
@note 目前只支持RCA通道的设置

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOSetChannel(int nAudioIndex,CSUDIAUDIOChannelType_E eChannelType)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	    
	int 	chain=0;    
	PIPE_STATUS Status;            
	CNXT_STATUS Cnxt_Retcode = CNXT_STATUS_OK;
	chain = nAudioIndex;
	PIPE_AUDIO_PRESENTATION_INPUT_ATTRIB inputAttr;
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s\n",__FUNCTION__);
	Cnxt_Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER); 
	if(Cnxt_Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Cnxt_Retcode);
		return CSUDI_FAILURE;
	}

	do
	{
		if((nAudioIndex < 0)||(nAudioIndex>=CS_TRID_MAX_AUDIO))
		{
			Retcode = CSUDIAUDIO_ERROR_BAD_PARAMETER;
			break;
		}
        
        if((nAudioIndex == 1)||/*For index 1&2 we support PCM play*/
            (nAudioIndex == 2))
        {
            CSPCMPlay_SetChannel(nAudioIndex,eChannelType);     
        }
        else
        {
    		chain = nAudioIndex;
    		gTmPipeObject.TridCoshipSetup[chain].eChannelType = eChannelType;
    		Status = gTmApDeviceObj.pAudioPresentation[chain]->get_input_attrib(gTmApDeviceObj.pAudioPresentation[chain],PIPE_AUDIO_PLAY_STREAM_0,&inputAttr);
    		if(Status != PIPE_STATUS_OK)                
    		{
    			Retcode = CSUDI_SUCCESS;
    			CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Warning.......line[%d] Status=%d\n",__LINE__,Status);
    			break;            
    		}
    		
    		switch(eChannelType)
    		{
    		case EM_UDIAUDIO_CHANNEL_STEREO:
    			inputAttr.InputCfg.LRMap = PIPE_AUDIO_PRESENTATION_LL_RR;
    			//AudioPresentationCfg.Input[0].LRMap = PIPE_AUDIO_PRESENTATION_LL_RR;
    			break;
    		case EM_UDIAUDIO_CHANNEL_LEFT:
    			inputAttr.InputCfg.LRMap = PIPE_AUDIO_PRESENTATION_LL_LR;
    			//AudioPresentationCfg.Input[0].LRMap = PIPE_AUDIO_PRESENTATION_LL_LR;
    			break;
    		case EM_UDIAUDIO_CHANNEL_RIGHT:
    			inputAttr.InputCfg.LRMap = PIPE_AUDIO_PRESENTATION_RL_RR;
    			//AudioPresentationCfg.Input[0].LRMap = PIPE_AUDIO_PRESENTATION_RL_RR;
    			break;
    		case EM_UDIAUDIO_CHANNEL_MIXED_MONO:
    			//inputAttr.InputCfg.LRMap = PIPE_AUDIO_PRESENTATION_RL_LR;
    			//AudioPresentationCfg.Input[0].LRMap = PIPE_AUDIO_PRESENTATION_RL_LR;
    			break;
    		default:
    			break;
    		}    
    		if(eChannelType == EM_UDIAUDIO_CHANNEL_MIXED_MONO)
    		{
    			PIPE_AUDIO_ATTRIB audioAttr;
                
    			cnxt_kal_memset(&audioAttr, 0, sizeof(audioAttr));
                
    			gTmPipeObject.hAudioObj[chain]->get_attrib(gTmPipeObject.hAudioObj[chain],&audioAttr);

                audioAttr.AudioCfg.DualMono = PIPE_AUDIO_DUAL_MONO_MIXEDMONO;

    			gTmPipeObject.hAudioObj[chain]->config(gTmPipeObject.hAudioObj[chain],&audioAttr.AudioCfg);
    			if(Status != PIPE_STATUS_OK)                
    			{
    				Retcode = CSUDI_FAILURE;
    				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Warning.......line[%d]Status=%d\n",__LINE__,Status);
    				break;            
    			}
    			
    		}
    		else
    		{
    			Status = gTmApDeviceObj.pAudioPresentation[chain]->set_input_attrib(gTmApDeviceObj.pAudioPresentation[chain],PIPE_AUDIO_PLAY_STREAM_0,&inputAttr);
    			if(Status != PIPE_STATUS_OK)                
    			{
    				Retcode = CSUDI_FAILURE;
    				CSDEBUG(MODULE_NAME, ERROR_LEVEL,"Warning.......line[%d]Status=%d\n",__LINE__,Status);
    				break;            
    			}
    		}
        }
		
	}while(0);
	Cnxt_Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
	if(Cnxt_Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_put FAILED %d \n", Cnxt_Retcode);
		return CSUDI_FAILURE;
	}
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief 设置指定音频解码器为静音

@param[in] nAudioIndex  Audio Decoder设备的索引
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户指定音频解码器上实现静音功能
@note 可以多次调用，但只有第一次有效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOMute(int nAudioIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	PIPE_STATUS Status;
	int 	chain=0;
	chain = nAudioIndex;  
    
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s nAudioIndex[%d]\n",__FUNCTION__,nAudioIndex);
	
	Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER);    
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_GET FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}

    do
	{
		if((nAudioIndex < 0)||(nAudioIndex>=CS_TRID_MAX_AUDIO))
		{
			Retcode = CSUDIAUDIO_ERROR_BAD_PARAMETER;
			break;
		}
        if((nAudioIndex == 1)||/*For index 1&2 we support PCM play*/
            (nAudioIndex == 2))
        {
            CSPCMPlay_Mute(nAudioIndex,TRUE);     
        }
        else
        {
            gTmPipeObject.TridCoshipSetup[chain].AudioMute = 1;
            Status = gTmPipeObject.hAudioObj[chain]->mute(gTmPipeObject.hAudioObj[chain], TRUE);
            if(Status != PIPE_STATUS_OK)
            {
                CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Fail :%s line: %d error\n",__FUNCTION__,__LINE__);
                Retcode = CSUDI_FAILURE;
                break;
            }
        }
	}while(0);
	Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_put FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


/**
@brief 取消指定音频解码器的静音状态

@param[in] nAudioIndex  Audio Decoder设备的索引
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 在用户指定音频解码器上实现取消静音的功能
@note 在非mute状态下调用此接口无效
@note 可以多次调用，但只有第一次有效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOUnmute(int nAudioIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int 	chain=0;
	PIPE_STATUS Status;
	chain = nAudioIndex;	
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s, nAudioIndex :%d\n",__FUNCTION__, nAudioIndex);
	
	Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER);	 
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_put FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}

	do
	{
		if((nAudioIndex < 0)||(nAudioIndex>=CS_TRID_MAX_AUDIO))
		{
			Retcode = CSUDIAUDIO_ERROR_BAD_PARAMETER;
			break;
		}
		if((nAudioIndex == 1)||/*For index 1&2 we support PCM play*/
			(nAudioIndex == 2))
		{
			CSPCMPlay_Mute(nAudioIndex,FALSE);	   
		}
		else
		{
			gTmPipeObject.TridCoshipSetup[chain].AudioMute = 0;
			Status = gTmPipeObject.hAudioObj[chain]->mute(gTmPipeObject.hAudioObj[chain], FALSE);
			if(Status != PIPE_STATUS_OK)
			{
				 CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"Fail:%s line: %d error\n",__FUNCTION__,__LINE__);
				 Retcode = CSUDI_FAILURE;
				 break;
			}
		}

	}while(0);
	Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_put FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}

/**
@brief 设置指定音频解码器的音量

@param[in] nAudioIndex  Audio Decoder设备的索引
@param[in] nVolume 用户需要输出的音量值，按[0,CSUDI_MAX_VOLUME]阶音量计算,0为无声,CSUDI_MAX_VOLUME最大
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 所有输出设备上统一设置音量
@note 音量超过有效范围返回错误,并不生效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIAUDIOSetVolume(int nAudioIndex,int nVolume)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	
	int 	chain=0;	
	PIPE_STATUS Status=PIPE_STATUS_OK;			  
	PIPE_AUDIO_PRESENTATION_OBJECT *pAPMObj = gTmApDeviceObj.pAudioPresentation[0];
	PIPE_AUDIO_PRESENTATION_ATTRIB AudioAttrib;    
	int32 volumeTemp;
	chain = nAudioIndex;
	
	CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"In Function :%s nAudioIndex[%d]nVolume[%d]\n",__FUNCTION__,nAudioIndex,nVolume);
	Retcode = cnxt_kal_sem_get(gUdi2AudioSem[chain], CNXT_KAL_WAIT_FOREVER);	 
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_get FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}

	do
	{
		if((nAudioIndex < 0)||(nAudioIndex>=CS_TRID_MAX_AUDIO) || ( nVolume < 0 || nVolume >= 32 ))
		{
			Retcode = CSUDIAUDIO_ERROR_BAD_PARAMETER;
			break;
		}
		if(nVolume == 0)
		{
			volumeTemp = -120000;  //-120 dB 
		}
		else
		{
		/*lichanghua, note:6000是nxp的最大音量值,但是在我们的硬件上用该值音频就会
		失真,所以将最大值减小,该值能保证音频不失真以及幅度的指标*/
			volumeTemp = -12000 + (12190*nVolume)/CSUDI_MAX_VOLUME;
			//volumeTemp = -12000 + (18000 * nVolume)/CSUDI_MAX_VOLUME;
		}
		
/*<!-- Add by 903126,2011-2-19 17:06:10: middleware will set the last volume 
befor stop player,but udi2 will do apdetach in player stop which reset the 
volume config*/
		g_sVlmCfg.nVolumeLeft = g_sVlmCfg.nVolumeRight = volumeTemp;
 /*End by --!>*/
		
		if((nAudioIndex == 1)/*For index 1 we support PCM play*/)
		{
			CSPCMPlay_SetVolume(nAudioIndex, volumeTemp);	  
		}
		else
		{
#if 0 /*<!-- by 903126 2011-3-31 15:03:45 */
			Status = gTmApDeviceObj.pAudioPresentation[chain]->get_attrib(gTmApDeviceObj.pAudioPresentation[chain],&AudioAttrib);
			if(Status != PIPE_STATUS_OK)				
			{
				Retcode = CSUDI_FAILURE;
				CSDEBUG(MODULE_NAME,ERROR_LEVEL,"failed.......line[%d] Status=%d\n",__LINE__,Status);
				break;
			} 
			AudioAttrib.Cfg.Input[0].nVolumeLeft = volumeTemp;
			AudioAttrib.Cfg.Input[0].nVolumeRight= volumeTemp;
			Status = gTmApDeviceObj.pAudioPresentation[chain]->config(gTmApDeviceObj.pAudioPresentation[chain],&AudioAttrib.Cfg);
			if(Status != PIPE_STATUS_OK)				
			{
				Retcode = CSUDI_FAILURE;
				CSDEBUG(MODULE_NAME,ERROR_LEVEL,"failed.......line[%d]Status=%d\n",__LINE__,Status);
				break;			  
			}
#else       
			int i =0;
			PIPE_AUDIO_PRESENTATION_INPUT_ATTRIB inputAtr;
			Status = pAPMObj->get_attrib(pAPMObj,&AudioAttrib);
			if(Status != PIPE_STATUS_OK)				
			{
			   Retcode = CSUDI_FAILURE;
			   CSDEBUG(MODULE_NAME,ERROR_LEVEL,"failed.......line[%d] Status=%d\n",__LINE__,Status);
			   break;
			} 
			for(i = 0;i< AudioAttrib.Cfg.uNumOfInput;i++)
			{
				Status = pAPMObj->get_input_attrib(pAPMObj,AudioAttrib.Cfg.Input[i].uStreamNum,&inputAtr);
				if(Status != PIPE_STATUS_OK)				
				{
					Retcode = CSUDI_FAILURE;
					CSDEBUG(MODULE_NAME,ERROR_LEVEL,"failed.......line[%d] Status=%d\n",__LINE__,Status);
					break;
				}
				
				inputAtr.InputCfg.nVolumeLeft = volumeTemp;
				inputAtr.InputCfg.nVolumeRight = volumeTemp;
				
				Status = pAPMObj->set_input_attrib(pAPMObj,AudioAttrib.Cfg.Input[i].uStreamNum,&inputAtr);
				if(Status != PIPE_STATUS_OK)				
				{
					Retcode = CSUDI_FAILURE;
					CSDEBUG(MODULE_NAME,ERROR_LEVEL,"failed.......line[%d] Status=%d\n",__LINE__,Status);
					break;
				}
			}

#endif /* 0  end by 903126--!>*/
		}
		
	}while(0);
	Retcode = cnxt_kal_sem_put(gUdi2AudioSem[chain]);
	if(Retcode != CNXT_STATUS_OK)
	{
		CSDEBUG(MODULE_NAME, ERROR_LEVEL,"KAL_SEM_put FAILED %d \n", Retcode);
		return CSUDI_FAILURE;
	}
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
    return Retcode;
}


