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
    
    CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
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
    UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
    return Retcode;

}

