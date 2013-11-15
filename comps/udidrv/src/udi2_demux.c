/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_demux.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------

 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_demux.h"
#include "generic_include.h"

#define MODULE_NAME ("CS_DEMUX")

extern CNXT_SEM_ID  gSectionSem;
extern CS_TM_PIPE_OBJECTS gTmPipeObject;
extern u_int32 guSectionFilterCount[MAX_NUM_TSI_DEMUX+1];

static int gTunerConnectedPipeID = CSUDIDEMUX_NOT_CONNECTED_ID;
/**
@brief ��ȡDemux�豸������

@param[out] pnDemuxCount ϵͳ�д���demux�豸�ĸ���
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����pnDemuxCount������δ����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetCount(int * pnDemuxCount)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	if(pnDemuxCount == NULL)
	{
		return CSUDIDEMUX_ERROR_BAD_PARAMETER;
	}
	*pnDemuxCount = MAX_DEMUX_OBJECTS;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ��ȡָ��Demux�豸������

���ۿ��л��߱�ռ��
@param[in] nDemuxIndex demux�豸������
@param[out] psCapabilityInfo demux�豸������Ϣ�����CSUDIDEMUXCapability_S����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����psCapabilityInfo������δ����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetCapability(int nDemuxIndex,CSUDIDEMUXCapability_S * psCapabilityInfo)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	if((psCapabilityInfo == NULL)||((nDemuxIndex >= MAX_DEMUX_OBJECTS)||(nDemuxIndex < 0)))
    {
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }

    if (nDemuxIndex == 0 || nDemuxIndex == 1)
    {
		psCapabilityInfo->m_dwWorkTypeMask = (EM_UDI_DEMUX_PLAY   | \
		                                     EM_UDI_DEMUX_RECORD | \
		                                     EM_UDI_DEMUX_DMA);
		psCapabilityInfo->m_dwDMAConnectedTunerId  = gTunerConnectedPipeID;
		psCapabilityInfo->m_nFilterNum = 26;//(MAX_SECTION_FILTERS/MAX_NUM_TSI_DEMUX);
		psCapabilityInfo->m_nkeyNum    = 32; /*Changed Once descrambler implemented */
		psCapabilityInfo->m_dwDMAConnectSupportTunersMask = 1;
		psCapabilityInfo->m_dwConTypeMask            = 0;
    }
    else if (nDemuxIndex == 2)
    {
        psCapabilityInfo->m_dwWorkTypeMask = ( EM_UDI_DEMUX_INJECT | \
                                               EM_UDI_DEMUX_PLAY);
        psCapabilityInfo->m_dwDMAConnectedTunerId  = CSUDIDEMUX_NOT_CONNECTED_ID;
        psCapabilityInfo->m_nFilterNum = 12;//0;
        psCapabilityInfo->m_nkeyNum    = 0;
        psCapabilityInfo->m_dwDMAConnectSupportTunersMask = 0;
        psCapabilityInfo->m_dwConTypeMask          = EM_UDI_DEMUX_INJECT_TS;
    }


    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "FilterNum %d KeyNum %d WorkTye %d ConType %d DMAConnected %d \n", 
    psCapabilityInfo->m_nFilterNum,psCapabilityInfo->m_nkeyNum, psCapabilityInfo->m_dwWorkTypeMask, 
    psCapabilityInfo->m_dwConTypeMask, psCapabilityInfo->m_dwDMAConnectedTunerId);
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief ��ָ��demux����ָ��tuner

@param[in] dwTunerId  ��ָ��demux���ӵ�tuner�豸Id
@param[in] nDemuxIndex  ��ָ��tuner���ӵ�demux������
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note 
	- ������Ҫtuner��demux�豸�ķ���������֮ǰ��������øýӿڽ���tuner��demux������
	- ������ڹ���״̬��demux����ֱ����¼�ƹ��ܣ�������ֱ��֮����ȥ����¼��ʱ�����Բ��õ��ô˽ӿ��ٴ�����tuner��demux���������Ʒ����մ˹涨��
@note ������Tuner��Demux��Ĭ�����ӹ���Ϊ0-0��1-1�Դ����ơ���ˣ��뾡������Demux����������ǰ�档
@note һ��Tuner���ԺͶ��Demux����,�����¼�߲������;��һ��Demuxֻ��ͬʱ����һ��Tuner
@note ��ͬʱ��һ��Demux���ӵ����Tuner�ϣ�����CSUDIDEMUX_ERROR_DEVICE_BUSY
@note Demux��������Tuner�Ͽ����ӣ��������ڴ���Inject����һ��Demux����ͬʱӵ������Դ�豸
@note ͬ������Injectռ�õ�Demux�豸������Tuner���ӣ���������½�����CSUDIDEMUX_ERROR_DEVICE_BUSY
@note �������ӵ�Tuner��Demux�޷����ӣ����罫����Tuner�벻֧��Live��Inject��Demux���ӣ�����CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED
@note ֻ��DMA���͵�Tuner����Ҫʹ�øýӿڽ�������,�����뷵��CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED
@note ����ӦTuner��Demux�������ӣ�����CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED����������ǻ���ĳЩƽ̨�����Ի��߼�ʵ�ֶ������
@note �ݲ�����DMA���͵Ŀ��ƶ�Tuner�豸

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXConnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
	
    CSUDI_Error_Code retCode = CSUDI_FAILURE;
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	int       Index          = 0;
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "START: CSUDIDEMUXConnectTuner Demux%d <-> Tuner%d\n", nDemuxIndex, dwTunerId);

    if((nDemuxIndex >= MAX_NUM_TSI_DEMUX )||(nDemuxIndex < 0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXConnectTuner Wrong DmxID %d\n", nDemuxIndex);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }
    if((dwTunerId >= MAX_TUNER_OBJECTS)||(dwTunerId < 0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXConnectTuner Wrong TunerID %d\n", dwTunerId);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }

    printf("\ndwTunerId(%d)::nDemuxIndex(%d)\n",dwTunerId,nDemuxIndex);

    #if 0
    if (gTunerConnectedPipeID == dwTunerId)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXConnectTuner Tuner Already Connected with PIPE(%d)\n", gTunerConnectedPipeID);
        return CSUDI_SUCCESS;
    }
    #endif
    
    /* Add tuner device According to Demux Number,Note: we Have only one tuner Device*/
    Index = nDemuxIndex;

    do
    {
        printf("Add Tuner in PIPE No(%d)\n",Index);
        if ( gTmPipeObject.hPipeObj[Index]->add_device(gTmPipeObject.hPipeObj[Index], (PIPE_OBJ_HANDLE)gTmPipeObject.hTunerObj[0])!= PIPE_STATUS_OK )
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Cannot Add Tuner To pipeline\n" );
        }

      gTunerConnectedPipeID = 0;
      retCode = CSUDI_SUCCESS;

    }while(0);
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;
}

/**
@brief �Ͽ�ָ��demux��ָ��tuner������

@param[in] dwTunerId ��ָ��demux���ӵ�tuner�豸Id
@param[in] nDemuxIndex ��ָ��tuner���ӵ�demux������
@return �ɹ��Ͽ��򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXDisconnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	CSUDI_Error_Code retCode = CSUDI_FAILURE;
	int Index = 0;
    CSDEBUG(MODULE_NAME, DEBUG_LEVEL, "START: CSUDIDEMUXDisconnectTuner Demux%d <-> Tuner%d\n", nDemuxIndex, dwTunerId);

    if((nDemuxIndex >= MAX_NUM_TSI_DEMUX )||(nDemuxIndex < 0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXDisconnectTuner Wrong DmxID %d\n", nDemuxIndex);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }
    if((dwTunerId >= MAX_TUNER_OBJECTS)||(dwTunerId < 0))
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXDisconnectTuner Wrong TunerID %d\n", dwTunerId);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }

    #if 0
    if (gTunerConnectedPipeID != dwTunerId)
    {
        CSDEBUG(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXDisconnectTuner Wrong PipeID %d\n", gTunerConnectedPipeID);
        return CSUDI_SUCCESS;
    }
    #endif
    
    /* Remove tuner device According to Demux Number,Note: we Have only one tuner Device*/
    Index = nDemuxIndex;

    do
    {
        printf("Remove Tuner(%d) From PIPE No(%d)\n",dwTunerId,Index);
        
        if ( gTmPipeObject.hPipeObj[Index]->delete_device(gTmPipeObject.hPipeObj[Index], (PIPE_OBJ_HANDLE)gTmPipeObject.hTunerObj[0])!= PIPE_STATUS_OK )
        {
            CSDEBUG(MODULE_NAME,ERROR_LEVEL,"Cannot Delete Tuner to pipeline\n" );
        }
        
        gTunerConnectedPipeID = CSUDIDEMUX_NOT_CONNECTED_ID;
        retCode = CSUDI_SUCCESS;
        
    }while(0);
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ��ȡָ��Demux�豸�Ŀ���Filter����

@param[in] nDemuxIndex demux�豸������
@param[out] pnFreeFilterCount ��ǰ���ڵĿ���filter����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����pnFreeFilterCount������δ����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetFreeFilterCount(int nDemuxIndex, int * pnFreeFilterCount)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;
	if((nDemuxIndex >= MAX_DEMUX_OBJECTS)||(nDemuxIndex < 0)||(pnFreeFilterCount == NULL))
	{
	   CSDebug(MODULE_NAME, ERROR_LEVEL, "FAIL: CSUDIDEMUXGetFreeFilterCount Wrong DmxID %d\n", nDemuxIndex);
	   return CSUDIDEMUX_ERROR_BAD_PARAMETER;
	}

	if ( cnxt_kal_sem_get(gSectionSem,CNXT_KAL_WAIT_FOREVER) != CNXT_STATUS_OK)
	{
	   CSDebug(MODULE_NAME, ERROR_LEVEL, "Demux Device is busy ....\n");
	   return CSUDISECTION_ERROR_UNKNOWN_ERROR;
	}

	do 
	{
	   if(nDemuxIndex < MAX_NUM_TSI_DEMUX)
	   {
	       *pnFreeFilterCount = ( 26 -guSectionFilterCount[nDemuxIndex]);
	   }
	   else if(nDemuxIndex == 2)
	   {
	        *pnFreeFilterCount = ( 12 -guSectionFilterCount[nDemuxIndex]);
	   }
	   else
	   {
	       *pnFreeFilterCount = 0;
	   }
	   
	}while(0);

	cnxt_kal_sem_put(gSectionSem);
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

/**
@brief ��ȡָ��Demux�豸��ǰts������

��ȡ�Ѿ�ע��(��TunerӲע��)��TS��������
@param[in] nDemuxIndex demux�豸������
@param[out] puTsPacketCount ��ŵ�ǰdumux��ts������
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����puTsPacketCount������δ����
@note ���ӿ���Ҫ��CSUDIPLAYERGetPacketCount�ӿ����ʹ���ж�ǰ�������Ƿ�ͣ��
@note ���ӿڽ�Ҫ��ӶԴ�Tuner����������Ч���Ƿ���Ի�ȡ��ע���TS������Ҫ��UDI2.0��ʵ��Ҳ����Ҫ����رܣ�Eastwin������ʹ�øýӿڻ�ȡ��ע���TS����Ϣ��
@note �ڲ���ƽ̨�ϣ��ýӿ��޷�ʵ�֡���ʱ�뷵��CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED�� Eastwin�����ڲ�֧�ֵ�ƽ̨��ʹ�ô˽ӿ��ж�����ͣ����
@deprecated �˽ӿ��м��δʹ�ã��ɲ�ʵ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
		but Entropic Pipeline Manager doesn't support to get the TS packet count,
		so we always reture 0 of TS packet count.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIDEMUXGetTsPacketCount(int nDemuxIndex, unsigned int * puTsPacketCount)
{
    UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	if((nDemuxIndex >= MAX_DEMUX_OBJECTS)||(nDemuxIndex < 0)||(puTsPacketCount == NULL))
    {
        CSDebug(MODULE_NAME, DEBUG_LEVEL, "FAIL: CSUDIDEMUXGetTsPacketCount Wrong DmxID %d\n", nDemuxIndex);
        return CSUDIDEMUX_ERROR_BAD_PARAMETER;
    }

    /* No Driver API is available in PIPILINE to get TS Packet Count*/
    *puTsPacketCount = 0;
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

