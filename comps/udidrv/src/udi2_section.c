/****************************************************************************/
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_section.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_section.h"
#include "generic_include.h"

#define MODULE_NAME "CS_SECTION"

static SECTION_REQUEST           gSectionTable[MAX_SECTION_FILTERS];
static PIPE_DEMUX_FILTER_OBJECT *gDmxFltObj[MAX_SECTION_FILTERS] = {NULL, };
static CNXT_SEM_ID  gSectionSem;
u_int32     guSectionFilterCount[MAX_NUM_TSI_DEMUX+1]={0};
u_int8      *gCbufSection;

u_int32 cntDmxFilterInInjectMode = 0;
u_int32 uGlobaltag = 0; 

extern CNXT_STATUS cnxt_cbuf_read_data_mmapped_dest(
                            CNXT_CBUF_HANDLE hHandle,
                            void *pData,
                            u_int32 uNumBytes,
                            u_int32 *pNumBytesRead);

extern CS_TM_PIPE_OBJECTS gTmPipeObject;

extern CNXT_QUEUE_ID injectDataQueue;

extern bool bPlayerStarted;

/**
@brief 请求Filter过滤数据

常用来过滤PSI、SI，也可用于过滤PES、RAW等数据,需要与CSUDIFILTERStart配合使用
@param[in] psRequestInfo Filter参数，详情请参见CSUDISECTIONRequestInfo_S
@param[in] fnSectionCallback 回调函数，当有符合条件的数据到来时，调用本函数通知，所有回调必须在同一线程且建议不要在调用线程中执行太多操作，详情请参见CSUDISECTIONCallback_F
@param[in] pvUserData 用户数据，可以为CSUDI_NULL
@param[out] phFilterHandle	用于接收返回的Filter句柄，返回值为0认为非法
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 
- 详细的过滤规则，请参考CSUDISECTIONRequestInfo_S结构的定义
- 对应的设备句柄在CSUDISECTIONRequestInfo_S结构中
- CSUDIFILTERAlloc成功后，调用CSUDIFILTERStart开始接收数据
- CSUDIFILTERAlloc可以多次调用，直到Filter通道数全部分配完为止

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERAllocate(const CSUDISECTIONRequestInfo_S * psRequestInfo, CSUDISECTIONCallback_F fnSectionCallback, const void * pvUserData ,CSUDI_HANDLE * phFilterHandle)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int FilterID;
    u_int8 i = 0;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    if(psRequestInfo == NULL)
    {
       return CSUDISECTION_ERROR_BAD_PARAMETER;
    }
    if((fnSectionCallback == NULL)||(phFilterHandle == NULL))
    {
       CSDebug(MODULE_NAME, ERROR_LEVEL, "NULL HANDLES \n");
        return CSUDISECTION_ERROR_INVALID_HANDLE;
    }
    if(psRequestInfo->m_nDemuxID >= MAX_DEMUX_OBJECTS || psRequestInfo->m_nDemuxID < 0)
    {
       CSDebug(MODULE_NAME, ERROR_LEVEL, "dwDemuxID is greater than max dmx handles \n");
       return CSUDISECTION_ERROR_BAD_PARAMETER;
    }
    if(psRequestInfo->m_nPID> 0x1FFF)
    {
       CSDebug(MODULE_NAME, ERROR_LEVEL, "Setting Invalid PID 0x%x \n", psRequestInfo->m_nPID);
       return CSUDISECTION_ERROR_BAD_PARAMETER;
    }
    if(psRequestInfo->m_nDemuxID < MAX_NUM_TSI_DEMUX)
    {
        if  (guSectionFilterCount[psRequestInfo->m_nDemuxID] >= 26)
        {
            CSDebug(MODULE_NAME, ERROR_LEVEL, "No Free Filter Available\n");
            return CSUDISECTION_ERROR_NO_FREE_FILTER;
        }
    }
    else if(psRequestInfo->m_nDemuxID == 2)
    {
        if  (guSectionFilterCount[psRequestInfo->m_nDemuxID] >= 12)
        {
            CSDebug(MODULE_NAME, ERROR_LEVEL, "No Free Filter Available\n");
            return CSUDISECTION_ERROR_NO_FREE_FILTER;
        }
        
    }

    if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Demux Device is busy ....\n");
        return CSUDISECTION_ERROR_UNKNOWN_ERROR;
    }

    
    do
    {
        FilterID = get_free_Filter_tableID();
        if(FilterID < 0)
        {
           retCode = CSUDI_FAILURE;
           break;
        }

        gSectionTable[FilterID].InUseFlag              = TRUE;
        gSectionTable[FilterID].uDemuxId               = psRequestInfo->m_nDemuxID;
        gSectionTable[FilterID].fnSectionCallback      = fnSectionCallback;
        gSectionTable[FilterID].pvUserData             = pvUserData;
		gSectionTable[FilterID].pTag = uGlobaltag;
        gSectionTable[FilterID].PID                    = psRequestInfo->m_nPID;
        gSectionTable[FilterID].DmxFilterCfg.uFilterId = FilterID;
        gSectionTable[FilterID].DmxFilterCfg.uPid      = psRequestInfo->m_nPID;
	/*lch note:驱动中CBUF的分配时按照PID 通道,同一个PID下挂多个filter的话
	会公用通道的CBUF,每个通道对应两个CBUF,一个是硬件放数据的channel
	buffer ,另一个是中间buffer :notify buffer,这两个CBUF的大小一样都是由下面的
	大小控制,所以打开一个PID 的filter对应的空间是下面的大小x 2 .驱动中
	分配通道buffer从KALCSSGEN段分配,详情请查看internal_pl_demux_filter_hw_config*/
        gSectionTable[FilterID].DmxFilterCfg.uBufSize  = 256 * 1024;

        if (psRequestInfo->m_eReqType == EM_UDI_REQ_SECTION_DATA)
        {
        
        #if 0
           gSectionTable[FilterID].InUseFlag = TRUE;
           gSectionTable[FilterID].uDemuxId  = psRequestInfo->m_nDemuxID;
           gSectionTable[FilterID].fnSectionCallback = fnSectionCallback;
           gSectionTable[FilterID].pvUserData = pvUserData;
           gSectionTable[FilterID].PID        = psRequestInfo->m_nPID;
           gSectionTable[FilterID].DmxFilterCfg.Type = PIPE_DEMUX_FILTER_SECTION_TYPE;
           gSectionTable[FilterID].DmxFilterCfg.uFilterId = FilterID;
           gSectionTable[FilterID].DmxFilterCfg.uPid  = psRequestInfo->m_nPID;
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uLength = 12;
           gSectionTable[FilterID].DmxFilterCfg.uBufSize = 128 * 1024;
        #endif
        
           gSectionTable[FilterID].DmxFilterCfg.Type = PIPE_DEMUX_FILTER_SECTION_TYPE;
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uLength = 12;

           if (psRequestInfo->m_bCRCCheck == CSUDI_TRUE)
           {
              gSectionTable[FilterID].DmxFilterCfg.bPassCrcErrorData = FALSE;
           }
           else 
           {
              gSectionTable[FilterID].DmxFilterCfg.bPassCrcErrorData = TRUE;
           }
           
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[0]  = \
               psRequestInfo->m_sMMFilter.match[0];
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[0]   = \
               psRequestInfo->m_sMMFilter.mask[0];
           if(psRequestInfo->m_sMMFilter.negate[0]!=0x0)
           {
             gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[0] = 0xFF;
           }
           else
           {
             gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[0] = 0;
           }
           
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[1]  = 0;
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[1]   = 0;
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[1] = 0;
   
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[2]  = 0;
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[2]   = 0;
           gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[2] = 0;
		   
           //gSectionTable[FilterID].DmxFilterCfg.pTag = uGlobaltag; //commented by frank.zhou
		   
		   uGlobaltag++;
   
           for (i = 3; i< PIPE_DEMUX_MAX_FILTER_LEN; i ++ )
           {
             gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[i] = \
               psRequestInfo->m_sMMFilter.match[i-2];
   
             gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[i] = \
               psRequestInfo->m_sMMFilter.mask[i-2];
             
             /* Edwards Compare Negative mask Bit Wise and UDI Using Byte Wise*/
             if(psRequestInfo->m_sMMFilter.negate[i-2]!=0x0)
             {
               gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[i] = 0xFF;
             }
             else
             {
               gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[i] = 0;
             }
           }
        }
        else if (psRequestInfo->m_eReqType == EM_UDI_REQ_PES_DATA)
        {
           gSectionTable[FilterID].DmxFilterCfg.Type     = PIPE_DEMUX_FILTER_PES_TYPE;

           printf("Set PES PID(0x%X) \n ",gSectionTable[FilterID].PID);
        }
        
        guSectionFilterCount[psRequestInfo->m_nDemuxID]++;
        
        *phFilterHandle = (CSUDI_HANDLE)&(gSectionTable[FilterID].DmxFilterCfg.uFilterId);

         gSectionTable[FilterID].state = SF_READY;
         
         retCode = CSUDI_SUCCESS;
        
    }while(0);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

/**
@brief 启动Filter过滤数据

@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- Start成功后，即开始接收、过滤相应数据
- 第一次调用后，再次(多次)调用将返回失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/    
CSUDI_Error_Code CSUDIFILTERStart(CSUDI_HANDLE hFilterHandle)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int FilterID;
    int *FtId;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    if(hFilterHandle == NULL)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIFILTERStart: NULL hFilterHandle\n");
        return CSUDISECTION_ERROR_INVALID_HANDLE;
    }
    FtId = (int *)hFilterHandle;
    FilterID = *FtId;
    if( (gSectionTable[FilterID].InUseFlag == FALSE) || 
          (gSectionTable[FilterID].state == SF_FILTERING))
    {
       CSDebug(MODULE_NAME, ERROR_LEVEL," Invalid Filter State =%d\n",gSectionTable[FilterID].state);
       return CSUDISECTION_ERROR_INVALID_STATUS;
    }

    if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Demux Device is busy ....\n");
        return CSUDISECTION_ERROR_UNKNOWN_ERROR;
    }

    //printf("Filter(%d) PvtData(%d) Start ......\n",FilterID,gSectionTable[FilterID].pvUserData);

    do
    {
       int     PmStatus;
       u_int32 uEventMask;
       u_int8  DemuxID = gSectionTable[FilterID].uDemuxId;
       PIPE_DEMUX_FILTER_CFG      dmxFilterCfg;

       cnxt_kal_memcpy(&dmxFilterCfg, &(gSectionTable[FilterID].DmxFilterCfg), sizeof(PIPE_DEMUX_FILTER_CFG));

       gDmxFltObj[FilterID] = PIPE_OBJ_CREATE ( demux_filter );
       if ( gDmxFltObj[FilterID] == NULL )
       {
          CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to create Filter\n");
          retCode = CSUDI_FAILURE;
          break;
       }

       #if 0
       print_filter_config(dmxFilterCfg);
       #endif

       PmStatus = gDmxFltObj[FilterID]->config( gDmxFltObj[FilterID], &dmxFilterCfg);
       if ( PmStatus != PIPE_STATUS_OK )
       {
          CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to Config Filter\n");
          retCode = CSUDI_FAILURE;
          break;
       }
       
       /* Subscribe events */
       uEventMask = ( 1 << ( PIPE_EVENT_DEMUX_FILTER_DATA & 0xffffff ) )
                      | ( 1 << ( PIPE_EVENT_DEMUX_FILTER_BUF_OVERFLOW & 0xffffff ) )
                      | ( 1 << ( PIPE_EVENT_DEMUX_FILTER_ERROR & 0xffffff ) );

       PmStatus = gDmxFltObj[FilterID]->subscribe_event( gDmxFltObj[FilterID], uEventMask );
       if ( PmStatus != PIPE_STATUS_OK )
       {
          CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to Set Event For Filter\n");
          retCode = CSUDI_FAILURE;
          break;
       }
       
       /*  Add Filter to demux */
       PmStatus = gTmPipeObject.hDemuxObj[DemuxID]->add_filter( gTmPipeObject.hDemuxObj[DemuxID], gDmxFltObj[FilterID] );
       if ( PmStatus != PIPE_STATUS_OK )
       {
          CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to Add Filter with Demux\n");
          retCode = CSUDI_FAILURE;
          break;
       }

       /* Start acquire */
       PmStatus = gDmxFltObj[FilterID]->filter_acquire_ctrl( gDmxFltObj[FilterID], TRUE );
       if ( PmStatus != PIPE_STATUS_OK )
       {
          CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to Start Filter\n");
          retCode = CSUDI_FAILURE;
          break;
       }
       gSectionTable[FilterID].state = SF_FILTERING;
       retCode = CSUDI_SUCCESS;
		if (2 == DemuxID)
		{
		   	u_int32 uMsg[4];
			CNXT_STATUS Status = CNXT_STATUS_OK;
			
			if (bPlayerStarted != TRUE)
			{					
				if(injectDataQueue == 0x0ffff)
				{	
					printf("injectDataQueue doesn't exist(0x0ffff), create it in CSUDIFILTERStart.\n");				
					Status = cnxt_kal_qu_create(100, NULL, &injectDataQueue);
					if(Status != CNXT_STATUS_OK)
					{
					   printf("Queue creation failed in CSUDIFILTERStart...\n" );
					   break;
					}
				}

				printf("Play module is stopped, so send request to injectDataQueue once.\n");
				cnxt_kal_qu_send(injectDataQueue, uMsg);
			}

		  	cntDmxFilterInInjectMode ++;			
		}
        
    }while(0);

    cnxt_kal_sem_put(gSectionSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


static CSUDI_Error_Code CSUDIFILTERStop_Priv(CSUDI_HANDLE hFilterHandle )
{
    int FilterID;
    int *FtId;
    int     PmStatus;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    if(hFilterHandle == NULL)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIFILTERStart: NULL hFilterHandle\n");
        return CSUDISECTION_ERROR_INVALID_HANDLE;
    }

    FtId = (int *)hFilterHandle;
    FilterID = *FtId;
    
    if((gSectionTable[FilterID].InUseFlag == FALSE) ||
            (gSectionTable[FilterID].state != SF_FILTERING))
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIFILTERStart: Invalid State\n");
        return CSUDISECTION_ERROR_INVALID_STATUS;
    }

    //printf("Filter(%d) PvtData(%d) Stop ......\n",FilterID,gSectionTable[FilterID].pvUserData);

    do
    {
       u_int8  DemuxID = gSectionTable[FilterID].uDemuxId;
       /* Stop acquire */
       PmStatus =  gDmxFltObj[FilterID]->filter_acquire_ctrl(  gDmxFltObj[FilterID], FALSE );
       if ( PmStatus != PIPE_STATUS_OK )
       {
          CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to Stop Filter\n");
          retCode = CSUDI_FAILURE;
          break;
       }

       /* Remove Filter from Demux */
       PmStatus =  gTmPipeObject.hDemuxObj[DemuxID]->remove_filter( gTmPipeObject.hDemuxObj[DemuxID], gDmxFltObj[FilterID] );
       if ( PmStatus != PIPE_STATUS_OK )
       {
          CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to remove Filter from demux\n");
          retCode = CSUDI_FAILURE;
          break;
       }

       PmStatus = PIPE_OBJ_DESTROY (demux_filter, gDmxFltObj[FilterID]);
       if ( PmStatus != PIPE_STATUS_OK )
       {
          CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to delete Filter\n");
          retCode = CSUDI_FAILURE;
          break;
       }

       gSectionTable[FilterID].state = SF_STOP;
       gDmxFltObj[FilterID] = NULL;

       retCode = CSUDI_SUCCESS;
       if (2 == DemuxID)
       {
		   CNXT_STATUS Status = CNXT_STATUS_OK;
			
           cntDmxFilterInInjectMode --;
		   
		   if (bPlayerStarted != TRUE)
		   {
		       if (0 == cntDmxFilterInInjectMode)
		       {
				   if(injectDataQueue != 0x0ffff)
				   {	   
					   printf("Not in play start mode, delete query in CSUDIFILTERStop.\n");
					   cnxt_kal_qu_destroy(injectDataQueue);
					   injectDataQueue = 0x0ffff;
				   }
		       }
		   }
       }
       
    }while(0);

    return retCode;
}


/**
@brief 停止Filter过滤数据,但并不释放Filter资源

仅停止，不释放资源
@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- Stop成功后，即停止接收、过滤相应数据
- 试图去停止一个未启动的Filter返回CSUDISECTION_ERROR_INVALID_STATUS
- 第一次调用后，再次(多次)调用将返回失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERStop(CSUDI_HANDLE hFilterHandle )
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CSUDI_Error_Code enRet;
	
	 if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
	{
		CSDebug(MODULE_NAME, ERROR_LEVEL, "Demux Device is busy ....\n");
		return CSUDISECTION_ERROR_UNKNOWN_ERROR;
	}

	enRet = CSUDIFILTERStop_Priv(hFilterHandle);
	 
	cnxt_kal_sem_put(gSectionSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

static CSUDI_Error_Code CSUDIFILTERFree_Priv(CSUDI_HANDLE hFilterHandle)
{
    int FilterID;
    u_int8 DemuxID;
    int *FtId;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    if(hFilterHandle == NULL)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIFILTERStart: NULL hFilterHandle\n");
        return CSUDISECTION_ERROR_INVALID_HANDLE;
    }	
    FtId = (int *)hFilterHandle;
    FilterID = *FtId;
    
    DemuxID = gSectionTable[FilterID].uDemuxId;

    /* Before Freeing Filter we must Stop Section Filter*/
    if (gSectionTable[FilterID].state == SF_FILTERING)
    {
       CSUDIFILTERStop_Priv(hFilterHandle);
    }

    //printf("Filter(%d) PvtData(%d) Free ......\n",FilterID,gSectionTable[FilterID].pvUserData);

    do
    {
        gSectionTable[FilterID].InUseFlag         = FALSE;
        gSectionTable[FilterID].PID               = ~0;
        gSectionTable[FilterID].nBufSize          = 0;
        gSectionTable[FilterID].fnSectionCallback = NULL;
        cnxt_kal_memset((void *)&(gSectionTable[FilterID].DmxFilterCfg),0, sizeof(PIPE_DEMUX_FILTER_CFG));

        if(guSectionFilterCount[DemuxID] != 0)
        {
            guSectionFilterCount[DemuxID]--;
        }
        
        gSectionTable[FilterID].state = SF_INIT;
        retCode = CSUDI_SUCCESS;
        
    }while(0);

    return retCode;
}

/**
@brief 停止Filter接收数据,并释放Filter资源

@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- Free成功后，即停止接收、过滤相应数据
- 第一次调用后，再次(多次)调用将返回失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERFree(CSUDI_HANDLE hFilterHandle)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	
	 if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
	{
		CSDebug(MODULE_NAME, ERROR_LEVEL, "Demux Device is busy ....\n");
		return CSUDISECTION_ERROR_UNKNOWN_ERROR;
	}

	Retcode= CSUDIFILTERFree_Priv(hFilterHandle);
	 
	cnxt_kal_sem_put(gSectionSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 修改Fillter设置

@param[in] hFilterHandle 由CSUDIFILTERAllocate分配得到的filter通道句柄
@param[in] psMMFilter Filter 参数详见 CSUDIFILTERMatchMask_S定义,若为CSUDI_NULL表示不更改此项
@param[in] bCRCCheck 是否检测CRC
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note
- 必须在Stop状态下才能修改filter参数，否则返回CSUDISECTION_ERROR_INVALID_STATUS
- 第一次调用后，再次(多次)调用将返回失败

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERModify(CSUDI_HANDLE hFilterHandle, const CSUDIFILTERMatchMask_S * psMMFilter,  CSUDI_BOOL bCRCCheck)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int FilterID;
    int *FtId;
    u_int8 i=0;

    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    if(hFilterHandle == NULL)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIFILTERModify: NULL hFilterHandle\n");
        return CSUDISECTION_ERROR_INVALID_HANDLE;
    }
    
    FtId = (int *)hFilterHandle;
    FilterID = *FtId;

    if(gSectionTable[FilterID].state == SF_FILTERING)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIFILTERModify: Invalid State\n");
        return CSUDISECTION_ERROR_INVALID_STATUS;
    }
    
    if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Demux Device is busy ....\n");
        return CSUDISECTION_ERROR_UNKNOWN_ERROR;
    }

    //printf("Filter(%d) PvtData(%d) Modify ......\n",FilterID,gSectionTable[FilterID].pvUserData);

    do
    {
        if (psMMFilter != NULL)
        {
        
        gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[0]  = \
            psMMFilter->match[0];
        gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[0]   = \
            psMMFilter->mask[0];
        if(psMMFilter->negate[0]!=0x0)
        {
          gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[0] = 0xFF;
        }
        else
        {
          gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[0] = 0;
        }
        
        gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[1]  = 0;
        gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[1]   = 0;
        gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[1] = 0;

        gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[2]  = 0;
        gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[2]   = 0;
        gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[2] = 0;

          for (i = 3; i<PIPE_DEMUX_MAX_FILTER_LEN ; i ++ )
          {
             gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[i] = \
               psMMFilter->match[i-2];
   
             gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[i] = \
               psMMFilter->mask[i-2];
             /* Edwards Compare Negative mask Bit Wise and UDI Using Byte Wise*/
             if (psMMFilter->negate[i-2] != 0)
             {
               gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[i] = 0xFF;
             }
             else
             {
                gSectionTable[FilterID].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[i] = 0;
             }
           }
        }
        else
        {
           CSDebug(MODULE_NAME, DEBUG_LEVEL, "psMMFilter is NULL ....\n");
        }
        
        if (bCRCCheck == CSUDI_TRUE)
        {
           gSectionTable[FilterID].DmxFilterCfg.bPassCrcErrorData = FALSE;
        }
        else 
        {
           gSectionTable[FilterID].DmxFilterCfg.bPassCrcErrorData = TRUE;
        }
        
        //print_filter_config(gSectionTable[FilterID].DmxFilterCfg);

        retCode = CSUDI_SUCCESS;
        
    }while(0);

    cnxt_kal_sem_put(gSectionSem);
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}


/**
@brief 设置Fillter的底层buffer大小

主要用于需要过滤大量数据,上层可能会来不及取走的情况
@param[in] hFilterHandle 由CSUDIFILTERAlloc分配的过滤器句柄
@param[in] nBufSize 要设置的底层buffer的大小，必须为2的n次方，最小1K，最大值根据各个平台差异不同
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值
@note 设置Fillter的底层buffer大小时必须让Fillter处于stop状态，否则返回CSUDISECTION_ERROR_INVALID_STATUS	

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFILTERSetBufSize(CSUDI_HANDLE hFilterHandle,  int nBufSize)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int FilterID;
    int *FtId;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;

    if(hFilterHandle == NULL)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIFILTERSetBufSize: NULL hFilterHandle\n");
        return CSUDISECTION_ERROR_INVALID_HANDLE;
    }

    FtId = (int *)hFilterHandle;
    FilterID = *FtId;

    if(gSectionTable[FilterID].state == SF_FILTERING)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL,"CSUDIFILTERSetBufSize: Invalid State\n");
        return CSUDISECTION_ERROR_INVALID_STATUS;
    }
    
    if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
    {
        CSDebug(MODULE_NAME, ERROR_LEVEL, "Demux Device is busy ....\n");
        return CSUDISECTION_ERROR_UNKNOWN_ERROR;
    }
    
    do 
    {
        /* Filter Config will allocate Memory in PIPELINE Manager */
        gSectionTable[FilterID].nBufSize = nBufSize;

        retCode = CSUDI_SUCCESS;

    }while(0);
    
    cnxt_kal_sem_put(gSectionSem);
    retCode = CSUDI_SUCCESS;
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);	
	return Retcode;
}

//frank.zhou following functions are for internal usage
CSUDI_Error_Code tm_section_init(void)
{
    u_int8       uIndex=0;
    CNXT_STATUS  Result = CNXT_STATUS_OK;
    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    
    for(uIndex = 0; uIndex< MAX_SECTION_FILTERS ; uIndex++)
    {
       gSectionTable[uIndex].InUseFlag         = FALSE;
       gSectionTable[uIndex].uDemuxId          = ~0;
       gSectionTable[uIndex].PID               = ~0;
       gSectionTable[uIndex].nBufSize          = 0;
       gSectionTable[uIndex].fnSectionCallback = NULL;
       gSectionTable[uIndex].hCbuf             = 0;
       gSectionTable[uIndex].state             = SF_INIT;
       cnxt_kal_memset((void *)&(gSectionTable[uIndex].DmxFilterCfg),0, sizeof(PIPE_DEMUX_FILTER_CFG));
    }

    do
    {
       Result = cnxt_kal_mem_malloc ( CNXT_POOL_ID_DEFAULT_UNCACHED,
                                      DEMUX_SECTION_MAX_SIZE,
                                      CNXT_MEM_DEFAULT,
                                      (void**)&gCbufSection );
       if ( Result != CNXT_STATUS_OK )
       {
           CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to Allocate Memory\n");
           retCode = CSUDI_FAILURE;
           break;
       }
       
       Result = cnxt_kal_sem_create(1, NULL, &gSectionSem);
       if(CNXT_STATUS_OK != Result)
       {
            CSDebug(MODULE_NAME, ERROR_LEVEL," Failed to Create Semaphore\n");
            retCode = CSUDI_FAILURE;
            break;
       }
       
       retCode = CSUDI_SUCCESS;
       
   }while(0);
    
   guSectionFilterCount[0] = 0;
   guSectionFilterCount[1] = 0;
   guSectionFilterCount[2] = 0;
    
    return retCode;
}

BOOL CSUDISECTIONTerm()
{
	u_int32       uIndex=0;
	u_int32 	    u32FilterHnd;

	printf("start %s\n",__FUNCTION__);

	/*lichanghua:在term的时候拿到锁就不再释放了,这样可以防止
	释放掉所有filter资源后其他线程又申请新的filter
	*/
	if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
	{
		CSDebug(MODULE_NAME, ERROR_LEVEL, "Demux Device is busy ....\n");
		return FALSE;
	}
	
	for(uIndex = 0; uIndex< MAX_SECTION_FILTERS ; uIndex++)
	{
		if (gSectionTable[uIndex].InUseFlag)
		{
			u32FilterHnd = uIndex;
			printf("start free filter:%d\n",uIndex);
			/*注意这里直接用index的地址能工作,但是比较丑陋*/
			if (CSUDI_SUCCESS != CSUDIFILTERFree_Priv(&u32FilterHnd))
			{
				CSDEBUG(MODULE_NAME,ERROR_LEVEL,"fail to CSUDIFILTERFree %d\n",__LINE__);
				break;
			}
		}
	}

	cnxt_kal_mem_free(gCbufSection);

	printf("exit %s\n",__FUNCTION__);

	return (uIndex==MAX_SECTION_FILTERS);
}

int get_free_Filter_tableID(void)
{
       u_int8 uIndex=0;

    for(uIndex = 0; uIndex< MAX_SECTION_FILTERS ; uIndex++)
    {
       if (gSectionTable[uIndex].InUseFlag == FALSE)
       {
          return uIndex;
       }
    }

    return -1;
}

void print_filter_config(PIPE_DEMUX_FILTER_CFG dmxFilterCfg)
{
 int i = 0;

 printf("Filter Type     = %d\n",dmxFilterCfg.Type );
 printf("Filter Id       = %d\n",dmxFilterCfg.uFilterId );
 printf("Filter uBufSize = %d\n",dmxFilterCfg.uBufSize );
 printf("Filter uPid     = %d\n",dmxFilterCfg.uPid );
 printf("CRC Check       = %d \n",dmxFilterCfg.bPassCrcErrorData);
 
 printf("\nFilterMatch=");
  for (i = 0; i< PIPE_DEMUX_MAX_FILTER_LEN ; i ++ )
  {
    printf("[0x%X] ", dmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[i]);
  }

  printf("\nFilterMask=");
  for (i = 0; i< PIPE_DEMUX_MAX_FILTER_LEN ; i ++ )
  {
    printf("[0x%X] ", dmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[i]);
  }

  printf("\nNegativeMask=");
  for (i = 0; i< PIPE_DEMUX_MAX_FILTER_LEN ; i ++ )
  {
    printf("[0x%X] ", dmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[i]);
  }

  printf("\n");

  return;
}

static CSUDI_Error_Code Soft_Section_Filtering(u_int32 FltId,u_int8 *gSectionData)
{
    u_int8 i;
    u_int8 Old_Bytes,New_Bytes;
    bool   bMatchFailed = FALSE;
    for(i = 12; i < PIPE_DEMUX_MAX_FILTER_LEN; i++)
    {
       Old_Bytes = gSectionTable[FltId].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMatch[i] & \
                   gSectionTable[FltId].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[i];

       New_Bytes = gSectionData[i] & \
                   gSectionTable[FltId].DmxFilterCfg.FilterCfg.SectionFilter.uFilterMask[i];

       if(gSectionTable[FltId].DmxFilterCfg.FilterCfg.SectionFilter.uNegativeMask[i])
       {
         if(Old_Bytes == New_Bytes)
         {
            bMatchFailed = TRUE;
            break;
         }
       }
       else
       {
          if(Old_Bytes != New_Bytes)
          {
            bMatchFailed = TRUE;
            break;
          }
       }
    }
    
    if (bMatchFailed != FALSE)
        return CSUDI_FAILURE;
    
    return CSUDI_SUCCESS;
}

void Section_filter_event_notifier(struct pipe_pipeline_obj *pPipe,
                                   void                     *pUserData,
                                   PIPE_NOTIFY_EVENT         Event,
                                   void                     *pData,
                                   void                     *pTag )
{

  PIPE_DEMUX_FILTER_DATA    *pFilterData = NULL;
  CNXT_STATUS                Status      = CNXT_STATUS_OK;
  u_int32                    TempFltID;
  CSUDISECTIONCallbackData_S Section_Data;
  u_int32                    uBytesRead;
  
  if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
  {
      CSDebug(MODULE_NAME, ERROR_LEVEL, "Filter Device is busy ....\n");
      return;
  }
  switch(Event) 
  {
     case PIPE_EVENT_DEMUX_SYNC_ACQUIRED:
            break;
     case PIPE_EVENT_DEMUX_SYNC_LOST:
            break;
     case PIPE_EVENT_DEMUX_HDR_ERROR:
            break;
     case PIPE_EVENT_DEMUX_FILTER_BUF_OVERFLOW:
            break;
     case PIPE_EVENT_DEMUX_FILTER_ERROR:
            break;
     case PIPE_EVENT_DEMUX_FILTER_DATA_PES:
          pFilterData = (PIPE_DEMUX_FILTER_DATA*)pData;

#if 0
          if (pFilterData->uLength >= DEMUX_SECTION_MAX_SIZE)
          {
             printf("Large DATA Chunk Size(%d).....\n",pFilterData->uLength);
             break;
          }
#endif

          for ( TempFltID = 0; TempFltID < MAX_SECTION_FILTERS; TempFltID++ )
          {
             if ( gDmxFltObj[TempFltID] == pFilterData->pDemuxFilter[0] )
             {
                break;
             }
          }
          if(TempFltID >= MAX_SECTION_FILTERS)
          {
             printf("No PES Filter Found....\n");
             break;
          }

		  if( gSectionTable[TempFltID].InUseFlag == FALSE )
		  {
			break;
		  }
		  
          Status = cnxt_cbuf_read_data_mmapped_dest(pFilterData->hCbuf,
                                                    gCbufSection,
                                                    pFilterData->uLength,
                                                    &uBytesRead);
          if(Status != CNXT_STATUS_OK)
          {
              //printf(" Failed to Read PES Data From Cbuf Status(%d)\n",Status);
              //printf("uBytesRead(%d) uLength(%d)\n",uBytesRead,pFilterData->uLength);
              break;
          }


          if ( (gSectionTable[TempFltID].InUseFlag != TRUE) || 
               (gSectionTable[TempFltID].state != SF_FILTERING) )
          {
             printf("Filter is Not in Use FltId=%d, [%d][%d]\n",
			 	TempFltID, gSectionTable[TempFltID].InUseFlag, gSectionTable[TempFltID].state);
             break;
          }
          if(gSectionTable[TempFltID].fnSectionCallback != NULL)
          {
              Section_Data.m_nDataLen = pFilterData->uLength;
              Section_Data.m_pucData  = gCbufSection;
  
              //printf("PES Filter(%d) PvtData(%d) Notify ......\n",TempFltID,gSectionTable[TempFltID].pvUserData);
              /* Call Back Function */
              gSectionTable[TempFltID].fnSectionCallback(
                                        EM_UDI_REQ_PES_DATA ,
                                        &Section_Data,
                                        gSectionTable[TempFltID].pvUserData );
              break;
          }
          else
          {
             printf("No Call Back function found \n");
          }
          
          break;

     case PIPE_EVENT_DEMUX_FILTER_DATA:
             
             pFilterData = (PIPE_DEMUX_FILTER_DATA*)pData;

			 int i = 0;

             Status = cnxt_cbuf_read_data_mmapped_dest(pFilterData->hCbuf,
                                                       gCbufSection,
                                                       pFilterData->uLength,
                                                       &uBytesRead);
             if(Status != CNXT_STATUS_OK)
             {
                 CSDebug(MODULE_NAME, INFO_LEVEL, " Failed to Read From Cbuf %x, status %d\n",pFilterData->hCbuf,Status);
                 break;
             }
            
			 for ( i = 0; i < pFilterData->uNumOfFilters; i++ )
			 {
				for ( TempFltID = 0; TempFltID < MAX_SECTION_FILTERS; TempFltID++ )
				{
				  if ( gDmxFltObj[TempFltID] == pFilterData->pDemuxFilter[i] )
				  {
					 break;
				  }
				} 

                if( TempFltID>= MAX_SECTION_FILTERS)
                    continue;

				#if 0 //commented by frank.zhou
				if((gSectionTable[TempFltID].InUseFlag == FALSE)||(gSectionTable[TempFltID].pTag != pFilterData->pTag[i]))
				{
					continue;
				}
				else
				{
					break;
				}
				#endif
			 }

			 if((i >= pFilterData->uNumOfFilters)||(gSectionTable[TempFltID].InUseFlag==FALSE))
			 {
				 break;
			 }
			 

#if 0
             int k;
             printf("\nSection Data(%d)Bytes::",pFilterData->uLength);
             for (k = 0; k< 5; k ++ )
             {
               printf("[0x%X] ", gCbufSection[k]);
             }
             printf("\n");
#endif

             for ( i = 0; i < pFilterData->uNumOfFilters; i++ )
             {
                for ( TempFltID = 0; TempFltID < MAX_SECTION_FILTERS; TempFltID++ )
                {
                  if ( gDmxFltObj[TempFltID] == pFilterData->pDemuxFilter[i] )
                  {
                     break;
                  }
                }   
            #if 0 //commented by frank.zhou
                if ((gSectionTable[TempFltID].InUseFlag != TRUE)||(gSectionTable[TempFltID].pTag != pFilterData->pTag[i]) )
                {
                   printf("Filter is Not in Use FltId=%d,%08X,%08X,%08X\n",TempFltID,
				   	gSectionTable[TempFltID].InUseFlag,
				   	gSectionTable[TempFltID].pTag,
				   	pFilterData->pTag[i]);
                   continue;
                }
			#endif
                
                if(gSectionTable[TempFltID].state != SF_FILTERING)
                {
                   printf(" Notify : Incorrect Filter State FltId=%d\n ",TempFltID);
                   continue;
                }
                
                /* 12 Byte Filtering is Done by H/W rest 4Bytes need to check Here */
                if(  Soft_Section_Filtering( TempFltID ,gCbufSection) != CSUDI_SUCCESS)
                {
                   //printf(" Notify : Sotware Section Filtering Failed\n" );
                   continue;
                }
                
                if(gSectionTable[TempFltID].fnSectionCallback != NULL)
                {
                    Section_Data.m_nDataLen = pFilterData->uLength;
                    Section_Data.m_pucData  = gCbufSection;

                    //printf("Filter(%d) PvtData(%d) Notify ......\n",TempFltID,gSectionTable[TempFltID].pvUserData);
                    /* Call Back Function */
                    gSectionTable[TempFltID].fnSectionCallback(
                                              EM_UDI_REQ_SECTION_DATA ,
                                              &Section_Data,
                                              gSectionTable[TempFltID].pvUserData );
                }    
             }

         break;

         default:
             break;
     }
  
  cnxt_kal_sem_put(gSectionSem);
  
}
