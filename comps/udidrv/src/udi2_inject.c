/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_inject.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_inject.h"
#include "generic_include.h"


CNXT_QUEUE_ID injectDataQueue=0x0ffff;

#ifdef DUAL_VIDEO_SURFACE
CNXT_IMAGE_HANDLE gStillImageHandle=NULL;
bool bInjectPlayIframe = FALSE;
bool bFinishedIframeDec = FALSE;
#else
CNXT_IMAGE_HANDLE gStillImageHandle;
#endif


#ifdef DUAL_VIDEO_SURFACE
extern void cs_tm_notify_player(CSUDIPlayerEventType_E UDIEvent);
void cs_tm_playback_notify( PIPE_PIPELINE_OBJECT *pPipeline,
							void                     *pUserData,
							PIPE_NOTIFY_EVENT         Event,
							void                     *pData,
							void                     *pTag )

{
	u_int32 uMsg[4];

	printf("\n-----DUAL_VIDEO_SURFACE----[%lx]----------\n\n",Event);

	switch(Event)
	{		
		case PIPE_EVENT_PIPELINE_REQ_CONT_BLK_FROM_T:
			cnxt_kal_qu_send(injectDataQueue, uMsg);
                    break;
		case PIPE_EVENT_PIPELINE_REQ_NEXT_CONT_BLK:							
			cnxt_kal_qu_send(injectDataQueue, uMsg);
			break;
		case PIPE_EVENT_PIPELINE_MEDIA_DATA_CONSUMED:
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_PIPELINE_MEDIA_DATA_CONSUMED \n");	         
			break;
		case PIPE_EVENT_VIDEO_DECODE_COMPLETE:       
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"[%s]PIPE_EVENT_VIDEO_DECODE_COMPLETE \n",__FUNCTION__);			
			bFinishedIframeDec = TRUE;
			cs_tm_display_update();		
            cs_tm_notify_player(EM_UDIPLAYER_VIDEO_FRAME_COMING);	
			break;
		case PIPE_EVENT_PIPELINE_STATE_CHANGED:
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_PIPELINE_STATE_CHANGED \n");
			break;
		case PIPE_EVENT_VIDEO_STARTED :          
			break;
		case PIPE_EVENT_VIDEO_DISPLAY_UPDATE :		
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"[%s]PIPE_EVENT_VIDEO_DISPLAY_UPDATE \n",__FUNCTION__);	
			cs_tm_display_update();		
            cs_tm_notify_player(EM_UDIPLAYER_VIDEO_FRAME_COMING);	
			break;
		case PIPE_EVENT_VIDEO_PLAYING :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_PLAYING \n");
			break;
		case PIPE_EVENT_VIDEO_BUFFER_EMPTY:
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_BUFFER_EMPTY \n");
			break;
		case PIPE_EVENT_AUDIO_STOPPED :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STOPPED \n");
			break;
		case PIPE_EVENT_AUDIO_STARTED :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STARTED \n");
			break;
		case PIPE_EVENT_AUDIO_STREAM_LOST :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STREAM_LOST \n");
			break;
		case PIPE_EVENT_AUDIO_SYNC_ACQ :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_ACQ \n");
			break;
		case PIPE_EVENT_AUDIO_SYNC_LOST :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_LOST \n");
			break;
		case PIPE_EVENT_AUDIO_BUFFER_EMPTY :
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_BUFFER_EMPTY \n");
			break;
		case PIPE_EVENT_VIDEO_STREAM_CHANGE:
			//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_STREAM_CHANGE \n");
			break;
		case PIPE_EVENT_DEMUX_FILTER_DATA:
			Section_filter_event_notifier(pPipeline,pUserData,Event,pData,pTag);
			break;
		default:
			break;
	}
}
#else
void cs_tm_playback_notify( PIPE_PIPELINE_OBJECT *pPipeline,
							void                     *pUserData,
							PIPE_NOTIFY_EVENT         Event,
							void                     *pData,
							void                     *pTag )

{
    CSUDIPlayerEventType_E UDIEvent;
	u_int32 uMsg[4];
	//CS_CNXT_Inject_SubSystem *pInjectHandle = (CS_CNXT_Inject_SubSystem *)pUserData;

	printf("\n---NO --DUAL_VIDEO_SURFACE----[%lx]----------\n\n",Event);

	switch(Event)
	{
	
	case PIPE_EVENT_PIPELINE_REQ_CONT_BLK_FROM_T:
	case PIPE_EVENT_PIPELINE_REQ_NEXT_CONT_BLK:
		cnxt_kal_qu_send(injectDataQueue, uMsg);
		break;
	case PIPE_EVENT_PIPELINE_MEDIA_DATA_CONSUMED:
		break;
	case PIPE_EVENT_VIDEO_DECODE_COMPLETE:
		// 2011-01-13 TerenceZhang begin:removed here to let the application know that the decoding complete.
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_VIDEO_DECODE_COMPLETE \n");	   
		UDIEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING;
		cs_tm_notify_player(UDIEvent);	
		// 2011-01-13 TerenceZhang end		
		break;
	case PIPE_EVENT_PIPELINE_STATE_CHANGED:
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL," PIPE_EVENT_PIPELINE_STATE_CHANGED \n");
		break;

	case PIPE_EVENT_VIDEO_STARTED :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_STARTED \n");       
        UDIEvent = EM_UDIPLAYER_VIDEO_FRAME_COMING;
		//printf("notify user msg:%d from playback\n",UDIEvent);
		/*lichanghua cancel :去掉解决IPTV TS注入闪上一帧问题*/
		//cs_tm_notify_player(UDIEvent);
		break;
	case PIPE_EVENT_VIDEO_DISPLAY_UPDATE :
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_DISPLAY_UPDATE \n"); 
		cs_tm_notify_player(UDIEvent);	
		break;
	case PIPE_EVENT_VIDEO_PLAYING :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_PLAYING \n");
		break;
	case PIPE_EVENT_VIDEO_BUFFER_EMPTY:
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_VIDEO_BUFFER_EMPTY \n");
		break;
	case PIPE_EVENT_AUDIO_STOPPED :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STOPPED \n");
		break;
	case PIPE_EVENT_AUDIO_STARTED :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STARTED \n");
		break;
	case PIPE_EVENT_AUDIO_STREAM_LOST :
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_STREAM_LOST \n");
		break;
	case PIPE_EVENT_AUDIO_SYNC_ACQ :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_ACQ \n");
		break;
	case PIPE_EVENT_AUDIO_SYNC_LOST :
		CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_SYNC_LOST \n");
		break;
	case PIPE_EVENT_AUDIO_BUFFER_EMPTY :
		//CSDEBUG(MODULE_NAME, DEBUG_LEVEL,"PIPE_EVENT_AUDIO_BUFFER_EMPTY \n");
		break;
	case PIPE_EVENT_VIDEO_STREAM_CHANGE:
		{
#if 0            
			PIPE_VIDEO_ATTRIB   Attrib;
			printf("PIPE_EVENT_VIDEO_STREAM_CHANGE");
			gTridPipeObject.hVideoObj[i]->get_attrib(gTridPipeObject.hVideoObj[i],&Attrib);               
			printf("Attrib[0].PicInfo.AspectRatio=%d\r\n", Attrib.PicInfo.AspectRatio );
			gTridPipeObject.TridVideoSubSystem[i].PictureInfo.AspectRatio = Attrib.PicInfo.AspectRatio;
#endif
		}
		break;
#if 0        
	case PIPE_EVENT_DEMUX_SYNC_ACQUIRED:
	case PIPE_EVENT_DEMUX_SYNC_LOST:
	case PIPE_EVENT_DEMUX_HDR_ERROR:
	case PIPE_EVENT_DEMUX_FILTER_DATA:
	case PIPE_EVENT_DEMUX_FILTER_BUF_OVERFLOW:
	case PIPE_EVENT_DEMUX_FILTER_ERROR:
		
            //printf("PIPE_EVENT_DEMUX_FILTER \n");
		Section_filter_event_notifier(pPipe,pUserData,Event,pData,pTag);
#endif            
	case PIPE_EVENT_DEMUX_FILTER_DATA:
		Section_filter_event_notifier(pPipeline,pUserData,Event,pData,pTag);
		break;


	default:
		break;
	}
        //cs_tm_notify_player(UDIEvent);
}
#endif

//frank.zhou---------------------------------------------------------------------------------------------------
/**
@brief 创建一个INJECTER注入实例,为INJECTER注入做好准备
@param[in] psInjecterChnl 数据注入通道数据结构指针。
@param[in] psOpenParams 将要创建的INJECTER实例的类型，主要包括PCM,TS,PES,ES等，详情请参见CSUDIINJECTERType_E
@param[out] phINJECTER 本次打开的INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note psInjecterChnl结构体中的设备索引可以为无效值(-1),代表该设备不需要或不存在
@note Demux必须先于Tuner断开连接，才能用于创建Inject，即一个Demux不能同时拥有两个源设备
@note 创建Inject须尽量使用最少的设备，如：
- 注入TS流的时候，注入内容只能是EM_UDIINJECTER_CONTENT_DEFAULT，否则返回CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- 注入PCM的时候，注入内容只能是EM_UDIINJECTER_CONTENT_AUDIO，否则返回CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- 注入TS流的时候，注入通道只能选DemuxID，音视频ID都为-1，否则返回CSUDIINJECTER_ERROR_DEVICE_DISORDER
- 注入PCM的时候，注入通道只能选AudioID，视频ID与DemuxID都为-1，否则返回CSUDIINJECTER_ERROR_DEVICE_DISORDER
- 注入I帧的时候，注入通道只能选VideoID,音频ID与DemuxID都为-1，否则返回CSUDIINJECTER_ERROR_DEVICE_DISORDER
- AVI,RM,RMVB, 等涉及播放多媒体文件播放的注入可不支持。如果芯片驱动对多媒体文件播放有更好的API，请告知同洲，
	同洲可对此API进行评估，如果合适，则封装上层接口直接调用其API.就不用再实现底层的注入支持。
*/
CSUDI_Error_Code  CSUDIINJECTEROpen(const CSUDIINJECTERChnl_S * psInjecterChnl, const CSUDIINJECTEROpenParam_S * psOpenParams, CSUDI_HANDLE * phINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 关闭一个INJECTER注入实例,释放相关资源
@param[in] hINJECTER INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code  CSUDIINJECTERClose(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 设置INJECTER注入实例的属性信息，本函数调整INJECTER实例的属性、相关参数、数据源等信息。
@param[in] hINJECTER 注入实例句柄，由CSUDIINJECTEROpen返回
@param[in] eAttrType ，将要调整的属性类型，请参见CSUDIINJECTERAttributeType_E的详细说明
@param[in] pvData 具体的属性数据结构指针，内容请参见CSUDIINJECTERAttributeType_E的详细说明。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 如果已经调用CSUDIINJECTERStart启动数据注入，不允许再调用此接口对回调函数进行设置。
@note 如果不支持设置回调函数，调用此接口将返回不支持操作
*/
CSUDI_Error_Code CSUDIINJECTERSetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, const void * pvData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 获取INJECTER注入实例的各属性信息。
@param[in] hINJECTER 注入实例句柄，由CSUDIINJECTEROpen返回
@param[in] eAttrType ，将要调整的属性类型，请参见CSUDIINJECTERAttributeType_E的详细说明
@param[out] pvData 具体的属性数据结构指针，内容请参见CSUDIINJECTERAttributeType_E的详细说明。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
*/
CSUDI_Error_Code CSUDIINJECTERGetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, void * pvData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief  开始一个INJECTER 数据 注入
@param[in] hINJECTER INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  如果注入实例已经开始，则返回CSUDIREC_ERROR_ALREADY_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStart(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief 停止一个INJECTER 数据注入
@param[in] hINJECTER INJECTER注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  如果注入实例已经被停止，则返回CSUDIREC_ERROR_ALREADY_STOPPED
@note  如果注入实例没有启动，则返回CSUDIREC_ERROR_NOT_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStop(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief等待Inecter已经注入到目标缓存区中的数据播放完成

本函数会堵塞，一直等到缓存中的数据播放完成了才返回CSUDI_SUCCESS，或下层驱动超时返回CSUDI_FAILURE
一般用于待播放数据全部注入完毕后，等待播放结束
@param[in] hINJECTER INJECTER数据注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  
*/
CSUDI_Error_Code CSUDIINJECTERFlush(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 清除Inecter已经注入到目标缓存区中的，尚没有播放完的全部数据

主要用于快进、快退、定位等操作时清空已注入的数据，已防止这些操作的延迟
@param[in] hINJECTER INJECTER数据注入实例句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  
*/
CSUDI_Error_Code CSUDIINJECTERClear(CSUDI_HANDLE hINJECTER)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 获取下次注入到Injecter目标缓冲区中的空闲Buffer地址指针和连续空闲Buffer的大小。
@param[in] hINJECTER INJECTER数据注入实例句柄
@param[out] ppvBuffer 返回获取到的内存地址指针
@param[out] puLength 返回获取到的内存长度
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note  如果buffer已经满，再去调用此接口应该返回失败，且puLength长度为0
@note  该函数必须与CSUDIINJECTERWriteComplete配对使用，对该函数的调用要考虑线程安全
@code 
	char  	*pcBuf = NULL; 
	unsigned int unBufSize = 0;
	...
	if( (CSUDIINJECTERGetFreeBuffer(hInject, (void *)&pcBuf, &unBufSize) == CSUDI_SUCCESS) && (unBufSize > 0))
	{
		//往buffer中写入内容,  此处假设unWriteLen <= unBufSize
		memcpy(pcBuf, pcDataTobeWrite, unWriteLen );		
		CSUDIINJECTERWriteComplete(hInject, unWriteLen);
	}
	...
@endcode
*/
CSUDI_Error_Code CSUDIINJECTERGetFreeBuffer(CSUDI_HANDLE hINJECTER, void ** ppvBuffer, unsigned  int * puLength )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief 通知本次注入的数据
@param[in] hINJECTER INJECTER数据注入实例句柄
@param[in] uWrittenSize 已拷贝数据的大小
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 如果buffer长度为nLength, 注入长度大于nLength,则该接口返回CSUDIINJECTER_ERROR_NO_MEMORY
@note  该函数必须与CSUDIINJECTERGetFreeBuffer配对使用，对该函数的调用要考虑线程安全
*/
CSUDI_Error_Code CSUDIINJECTERWriteComplete(CSUDI_HANDLE hINJECTER, unsigned int uWrittenSize)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

