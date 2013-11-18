/**@defgroup RM resource manager �豸����
@brief �豸����

�ṩ�������豸�Ĺ��ܲ�ѯ�����ӵȹ��ܡ�\n
������豸֮�����������������м�㼰Ӧ�ò�ģ��ֱ�Ӱ�ҵ���ע�豸��ʹ�ã��������ע������豸���豸��Ĺ���\n
�����ϲ�Ը�ģ��ʹ���豸���й���������ģ��ͬʱʹ����ɵĳ�ͻ
@version 0.2 2008/10/28 �����ϸ����������ʣ��ĳ�ʼ���ӿ�
@version 0.1 2008/10/21 ��ʼ�汾
@{
*/

#ifndef _UDI_RM_H_
#define _UDI_RM_H_
#include "udi_typedef.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**@brief Demux�豸֧��ֱ��*/
#define   DEMUX_SUPPORT_LIVE   				 (0x01)

/**@brief Demux�豸֧��¼��*/
#define   DEMUX_SUPPORT_REC  				 (0x02)

/**@brief Demux�豸֧�ֻط�*/
#define   DEMUX_SUPPORT_PLAYBACK   			 (0x04)	

typedef DWORD HPATH;

/**@brief ֧�ֵ��豸����ö��*/
typedef enum
{
	EM_RESOURCE_TUNER = 0x10,       /**< ��Ƶͷ*/
	EM_RESOURCE_DEMUX,		        /**< �⸴����*/
	EM_RESOURCE_VIDEO,              /**< ��Ƶ������*/
	EM_RESOURCE_AUDIO,              /**< ��Ƶ������*/
	EM_RESOURCE_VIDLAYER,		    /**< ��Ƶ�����*/
	EM_RESOURCE_LAN,		        /**< ����*/
	EM_RESOURCE_UNDEF
}CSRMResource_E;  

/**@brief �豸��ʶ�ṹ����Ψһ��ʶһ���豸*/
typedef struct _RM_Dev_Req_
{
      CSRMResource_E m_eResType;    /**< �豸����*/
      int            m_nIndex;   	/**< �豸��������0��ʼ����*/
}CSRMDeviceReq_S;

/**@brief ��Ƶͷ����ö��*/
typedef enum
{
	EM_TUNER_CAB,			  /**< ���߸�Ƶͷ*/
	EM_TUNER_SAT,			  /**< ���Ǹ�Ƶͷ*/
	EM_TUNER_SAT2,     		  /**< ����S2��Ƶͷ*/
	EM_TUNER_TER,			  /**< �����Ƶͷ*/
	EM_TUNER_UNDEF
}CSRMTunerType_E;

/**@brief ��Ƶͷ������Ϣ*/
typedef  struct  _Tuner_config
{  
    CSRMTunerType_E 	m_emTunerType;	/**< ��Ƶͷ����*/
}CSRMTunerConfig_S;

/**@brief Demux֧��ͨ������ö�٣�����ָʾ��ͨ�����͸��ݾ�����ò�ͬ*/
typedef enum
{
	EM_DEMUX_CHANNELCOUNT_0 = 0x0, 		/**< 0��ͨ��*/
	EM_DEMUX_CHANNELCOUNT_1 = 0x1, 		/**< 1��ͨ��*/
	EM_DEMUX_CHANNELCOUNT_N = 0x7fffffff 	/**< δ֪��ͨ��*/
}CSRMDemuxChannel_E;

/**@brief Demux������Ϣ*/
typedef   struct  _Demux_config
{ 
	int				m_nSupportType;                     /**< DEMUX_SUPPORT_LIVE|DEMUX_SUPPORT_REC|DEMUX_SUPPORT_PLAYBACK�����*/
	CSRMDemuxChannel_E		m_emLiveChannel;            /**< Demux֧�ֵ���ֱ��ͨ���ĸ�����Ҳ����demux���������ٸ�decoder(A+V��һ��)*/
	CSRMDemuxChannel_E		m_emRecordChannel;          /**< Demux֧��¼��ͨ���ĸ�����Ҳ����demux�����������·ts��[��HDD]*/
	CSRMDemuxChannel_E		m_emPlaybackChannel;        /**< Demux֧�ֵ��ӻطŵĸ�����Ҳ����demux���Խ��ն���·ts����ע��[From FILE or IP]*/
    DWORD	 	        	m_nTunerLinkMark;	        /**< ��Demux��������Tuner�ļ�¼��
        								                    �����Demux�������ӵ���N��Tuner,��m_nTunerMark�Ķ�Ӧ��Nλ��1,�����������Nλ��0��
        								                    �����Demux�������ӵ��κ�Tuner����������λȫΪ0��*/
}CSRMDemuxConfig_S;

/**@brief ��Ƶ����������ö��*/
typedef  enum    _VID_Dec_Type_
{
	EM_DECODER_OUTPUT_HD,       /**< ��Ƶ��������֧�ָ������������*/
	EM_DECODER_OUTPUT_SD,       /**< ��Ƶ��������֧�ֱ������������*/
   	EM_DECODER_OUTPUT_HD_SD,    /**< ��Ƶ������֧�ָߡ��������������*/
	EM_DECODER_OUTPUT_UNDEF
} CSRMVIDDecType_E;

/**@brief ��Ƶ������������Ϣ*/
typedef  struct  _video_config
{  
    CSRMVIDDecType_E 	m_emDecoderType;  /**< ��Ƶ����������*/
}CSRMVideoConfig_S;

/**@brief ��Ƶ���������ö��*/
typedef  enum    VID_Layer_Type_
{
	EM_LAYTER_HD,       /**< ��Ƶ�����֧�ָ���(HD)��ʽ���루��:1024��768�ȣ�*/
	EM_LAYTER_SD,       /**< ��Ƶ�����֧�ֱ���(SD)��ʽ���� ���磺720��576�ȣ�*/
	EM_LAYTER_UNDEF
} CSRMVIDLayerTpye_E;

/**@brief ��Ƶ�����������Ϣ*/
typedef  struct  _video_Layer_config
{  
    CSRMVIDLayerTpye_E 		m_emVIDLayer;	/**< ��Ƶ���������*/
}CSRMVIDLayerConfig_S;

/**
@brief ����ָ�����豸

һ���豸���ͷ�ǰ��ֻ������һ��
@param[in] stDevice Ҫ������豸�ı�ʶ
@param[out] phDevice �豸���
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note ���ĳ��ƽ̨��n����Ƶͷ����������������0,1...n-1
*/
BOOL CSRMAlloc(const CSRMDeviceReq_S  stDevice, HCSHANDLE* const phDevice);

/**
@brief �ͷŷ�����豸���

һ���ѱ�������豸�����ͷź�ɱ��ٴ�����
@param[in] phDevice Ҫ�ͷŵ��豸���
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note �������ͷ���CSRMAlloc������豸������ͷųɹ��󣬸�Handle�����������ø��豸���κι��ܺ�����
�ϲ����Ҫʹ���豸���������µ���CSRMAlloc����豸�����
*/
BOOL CSRMFree(const HCSHANDLE  phDevice);
 
/**
@brief ������豸���һ���豸��

���豸������Ϊͳһ�ľ�������������豸���ϵ��豸\n
���罫һ��Tunerһ��Demuxһ��Decoder�����������γ�һ��path��
Ҫʹ����Щ�豸ʱʹ�����path��Ϊ��ʾ������������ؽӿ�\n
@param[in] hDevices Ҫ����豸�����豸�������,��Щ�豸�����ΪCSRMAlloc�����
@param[in] nDeviceNums �豸�������豸������
@return �豸���������ӳɹ����򷵻�һ���豸��·�����������־�����豸�������򷵻�NULL
@note ��������豸�߼��ϻ����������޷����ӣ��򷵻�NULL
@note �豸�����ڸú������غ���ͷţ�����ʵ�ֻ�������������е���Ϣ
@note ������Connect�γ�hPath���豸���������������Free����������Free���ٲ�����hPath�������Ϊ��δ�����
@note ����ȱ��ĳЩΨһ�豸���豸�����Ƿ��������������δ����ġ���ֻ��һ����Ƶ�����ʱ���豸����û��EM_RESOURCE_VIDLAYER�豸
*/
HPATH CSRMConnect(const HCSHANDLE hDevices[], int nDeviceNums);

/**
@brief �Ͽ������ӵ��豸��

��ͨ��CSRMConnect�������豸���Ͽ����Ͽ���hPath���������Ч
@param[in] hPath Ҫ�Ͽ����豸�������
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note ��������£��Ͽ��豸���󣬲���Free�豸���ϵ��豸
*/
BOOL CSRMDisconnect(const HPATH hPath);

/**
@brief �����豸��������豸����Ӧ���͵��豸���豸����

@param[in] hHandle �豸��������豸�����
@param[in] emType �豸����
@return �����ɹ����ض�Ӧ����������Ч����-1
@note �ýӿ���Ҫ�����豸����ģ����豸״̬��ѯ
@note hHandle�������豸Ҳ�������豸��
- ���handel��һ���豸�������emType�����Ǹ��豸�����ͣ����򷵻�ʧ��(-1)�����������emType������У��
- ���handle��һ���豸������������б������һ��emType���͵��豸�����򷵻�ʧ��(-1)�����������emType����ѡ���豸
*/
int CSRMGetIndex(const HCSHANDLE hHandle, CSRMResource_E  emType);

/**
@brief ��ѯָ�����豸�Ŀ���״̬

���һ���豸ͨ��CSRMAlloc�ӿڷ����ȥ������û��ͨ��CSRMFree�ӿ��ͷţ���Ϊ�ǿ���״̬������Ϊ����״̬
@param[in] stDevice ����ѯ�豸�ı�ʶ
@return Ŀ����з���TRUE�����򷵻�FALSE
@note �ýӿ���Ҫ�����豸����ģ����豸״̬��ѯ
@note ���ڲ����ڵ��豸����FALSE
*/
BOOL CSRMIsFree(const CSRMDeviceReq_S stDevice);

/**
@brief ��ѯָ�����͵��豸������

��ѯ�豸ĳ�����͵��豸�������������Ƿ���л����ѱ�����
@param[in] emType ����ѯ���豸����
@return �������豸����������ƽ̨û�и��豸�򷵻�0��δ֪���豸�����򷵻�-1
@note �ýӿ���Ҫ�����豸����ģ����豸״̬��ѯ
*/
int CSRMGetDeviceCounts(CSRMResource_E emType);

/**
@brief ��ѯָ��Demux�豸������

@param[in] nDmxIndex ����ѯdemux�豸����
@param[out] pstDemuxInfo �洢�����demux������Ϣ������ΪNULL
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note �ýӿ���Ҫ�����豸����ģ����豸״̬��ѯ
*/
BOOL CSRMGetDemuxConfig(int nDmxIndex,CSRMDemuxConfig_S* pstDemuxInfo);

/**
@brief ��ѯָ����Ƶͷ�豸������

@param[in] nTunerIndex ����ѯTuner�豸����
@param[out] pstTunerInfo �洢�����Tuner������Ϣ������ΪNULL
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note �ýӿ���Ҫ�����豸����ģ����豸״̬��ѯ
*/
BOOL CSRMGetTunerConfig(int nTunerIndex,CSRMTunerConfig_S* pstTunerInfo);

/**
@brief ��ѯָ����Ƶ�������豸������

@param[in] nVideoIndex ����ѯ��Ƶ�������豸����
@param[out] pstVideoInfo �洢�������Ƶ������������Ϣ������ΪNULL
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note �ýӿ���Ҫ�����豸����ģ����豸״̬��ѯ
*/
BOOL CSRMGetVideoConfig(int nVideoIndex,CSRMVideoConfig_S* pstVideoInfo);

/**
@brief ��ѯָ����Ƶ������豸������

@param[in] nVIDLayerIndex ����ѯ��Ƶ������豸����
@param[out] pstVIDLayerInfo �洢�������Ƶ�����������Ϣ������ΪNULL
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note �ýӿ���Ҫ�����豸����ģ����豸״̬��ѯ
*/
BOOL CSRMGetVIDLayerConfig(int nVIDLayerIndex,CSRMVIDLayerConfig_S *pstVIDLayerInfo);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
