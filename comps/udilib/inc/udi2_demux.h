/**@defgroup DEMUX DEMUX ������ƽ̨�޹صĽ⸴���豸�����ӿ�
@brief DEMUXģ���Ӧͷ�ļ�<udi2_demux.h>��������Demux�豸������״̬�������ӿڡ�

�ܶ��������Ľ⸴���豸����Ϊһ��Demux����Ӳ����Demux����һ�¼��ɡ�������Demux��������������ǰ�档

Demux��ʹ��ǰ������Tuner�������ӣ���������û�е����κ����ӶϿ��ӿ�֮ǰ��Tuner0��Demux0���ӣ�Tuner1��Demux1���ӣ���֤����û�����ģ���MMCP��CloudTV ���м��ƽ̨ϵ�в�Ʒ�У�����Ҳ����������������Ҫ����Demux��������ǰ���ԭ��֮һ��

Demux���������Ƿ����Inject��Record������һ��Demux�Ƿ���Խ��б�¼�߲�������ֻ�ܽ�����ͨ�Ĳ��ţ�����ʱ����������һ��Demuxȥ����¼�ơ�

��������������Χ�ڣ�һ��Demux�������������ͬʱ��������������������ͬʱ����������һ��Demux����¼������Ҳ�в��������������������ͬʱ¼����һ·TS���еĲ�ͬ��Ŀ����һ��Demux��ʹͬʱӵ��DMA��Inject��������Ҳ����ͬʱ����Tuner��ȡ�����ִ�Inject��ȡ���ݡ���֮����������������Դͬʱ����ͬһ��demux

��ģ�黹�������ڻ�ȡ�Ѵ�Tunerע���TS�������������ڼ��ǰ������ͣ��������pidͣ������������pidͣ������Ϣ���Դ�Player��á��Ƿ���Ի�ȡ��ע���TS������Ҫ��UDI��ʵ��Ҳ����Ҫ����رܣ�MMCP��CloudTV ���м��ƽ̨����ʹ�øýӿڻ�ȡ��ע���TS����Ϣ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.4 2009/09/14 ���Tuner,Demux����״̬��ѯ���ܣ����Tuner��Demux�������ӵĴ������˵��
@version 2.0.3 2009/09/08 ���EM_UDI_DEMUX_DMA�Խ�������������ݻ�ȡ�����ֿ�����Ϊ���Ǳ������Ƕ�����
@version 2.0.2 2009/08/26 �����Ƿ�֧������ע�����������
@version 2.0.2 2009/08/26 ȥ��demux������Ϣ�е�ͨ�����Ͷ˿����������Ƿ�֧��ֱ��������
@version 2.0.2 2009/08/25 ������Demux�豸��������˵��
@version 2.0.1 2009/08/17 ����
@{
*/

#ifndef _UDI2_DEMUX_H_
#define _UDI2_DEMUX_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDIDEMUX_NOT_CONNECTED_ID			(0xFFFFFFFF)			///< δ���κ�Demux���ӵ�TunerId

/**@brief DEMUX �豸��ش������ֵ*/
enum
{
	CSUDIDEMUX_ERROR_BAD_PARAMETER = CSUDI_DEMUX_ERROR_BASE,		///< �������
	CSUDIDEMUX_ERROR_FEATURE_NOT_SUPPORTED,							///< ��֧�ִ˲���
	CSUDIDEMUX_ERROR_UNKNOWN_ERROR,									///< һ�����
	CSUDIDEMUX_ERROR_DEVICE_BUSY,									///< �豸æ,����Demux�Ѿ����ӹ���
	CSUDIDEMUX_ERROR_NO_MEMORY									///< �޿����ڴ�
};

/**@brief DEMUX֧�ֵ�����ע������

@note ������ν�ġ�ע�롱ָ���ݽ��룬�����ܷ�ʽ����������ͨ��Tuner DMA�����ģ����Ǵ�Buffer��ע��ġ�
*/
typedef enum
{
	EM_UDI_DEMUX_INJECT_AVI 	= 1 << 0,       ///< ֧��AVI��������ע��
	EM_UDI_DEMUX_INJECT_RM		= 1 << 1,		///< ֧��RM��������ע��
	EM_UDI_DEMUX_INJECT_RMVB 	= 1 << 2,		///< ֧��RMVB���͵�����ע��
	EM_UDI_DEMUX_INJECT_TS      = 1 << 3        ///< ֧��TS���͵�����ע��
	
}CSUDIDEMUXContainerType_E;

/**@brief DEMUX֧�ֹ�������*/
typedef enum
{
	EM_UDI_DEMUX_PLAY 	  		= 1 << 0,	 ///< ֧�ֲ���
	EM_UDI_DEMUX_RECORD   		= 1 << 1,    ///< ֧��¼��
	EM_UDI_DEMUX_DMA			= 1 << 2,	 ///< ֧����DMA��ʽ��ȡTuner�����ݣ���Tuner��Demux����ͨ��Ӳ����ʽֱ�����ӣ�����Demuxͨ��CSUDIDEMUXConnectTuner��Tuner���Ӻ������κβ������ɻ�ȡTuner�����ݣ�����������Դ�Tuner��ȡ���ݵ�Demux����ӵ���������
	EM_UDI_DEMUX_INJECT 		= 1 << 3     ///< �ط����ݵ�ע�룬����PVR���͵�TSע�룬�Լ�IPͨ�����͵�TS������ע�룬��ע��Tuner��ʽ������ע���	
}CSUDIDEMUXWorkType_E;

/**@brief DEMUX�豸������Ϣ*/
typedef struct  
{
	int 			m_nFilterNum;                      ///< ���������� 
	int 			m_nkeyNum;                         ///< ��Կ��(descramble)����
	CSUDI_UINT32	m_dwWorkTypeMask;                  ///< ֧�ֵ�TSԴע�����ͣ�������CSUDIDEMUXWorkType_E�е�(EM_UDI_DEMUX_PLAY | EM_UDI_DEMUX_INJECT | EM_UDI_DEMUX_RECORD ) �ĸ������
	CSUDI_UINT32    m_dwConTypeMask;				   ///< ֧�ֵ���������ע�����ͣ�������CSUDIDEMUXContainerType_E�еĸ������
	CSUDI_UINT32	m_dwDMAConnectedTunerId;		   ///< ��ǰ���Demux�����ϵ�DMA TunerId���������DMA Demux����DMA Demuxδ���κ�Tuner���������ֵΪCSUDIDEMUX_NOT_CONNECTED_ID
	CSUDI_UINT32	m_dwDMAConnectSupportTunersMask;   ///< �������Demux���ӵ�TunerId�б����е�nλ��ʾTunerIdΪn��Tuner�������Demux�������ݲ����ǿ��ƶ�Tuner��DemuxӲ���ӵ����
} CSUDIDEMUXCapability_S;

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
CSUDI_Error_Code CSUDIDEMUXGetCount(int * pnDemuxCount);

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
CSUDI_Error_Code CSUDIDEMUXGetCapability(int nDemuxIndex,CSUDIDEMUXCapability_S * psCapabilityInfo);

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
CSUDI_Error_Code CSUDIDEMUXConnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex);

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
CSUDI_Error_Code CSUDIDEMUXDisconnectTuner(CSUDI_UINT32 dwTunerId, int nDemuxIndex);

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
CSUDI_Error_Code CSUDIDEMUXGetFreeFilterCount(int nDemuxIndex, int * pnFreeFilterCount);

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
CSUDI_Error_Code CSUDIDEMUXGetTsPacketCount(int nDemuxIndex, unsigned int * puTsPacketCount);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif

