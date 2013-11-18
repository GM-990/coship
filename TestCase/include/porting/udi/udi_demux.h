/**@defgroup CSDEMUX demux �⸴�ü�����ע��
@brief �⸴�ü�����ע��

������������ļ����ӿڵĹ�ϵ�뿴��ͼ:
@image HTML demuxstate.png

@version 0.3 2008/11/27 
- ����CSDMXStart�ص�ʱ������ϸ˵��
- ����Filterֻ��֤16�ֽ���ȵ�����

@version 0.2 2008/10/29 �����ϸ����
@version 0.1 2008/10/17 ��ʼ�汾
@{
*/

#ifndef _UDI_DEMUX_H_
#define _UDI_DEMUX_H_

#include "udi_typedef.h"
#include "udi_public.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief filter���ȣ�������section�е�length�ֶ�*/
#define FILTER_LEN 16

/**@brief Demux�ص���������ö��*/
typedef enum _USPSectionDemuxCallbackType
{
	USP_SECTION_DEMUX_DATA,         /**< ��ʾһ��PSI��SI���ݰ�*/
	USP_PES_DEMUX_DATA,             /**< ��ʾһ��PES���ݰ�*/
	USP_RAW_DEMUX_DATA              /**< ��ʾһ��RAW���ݰ�*/
}USPSectionDemuxCallbackType;

/**@brief Demux�ص����ݽṹ*/
typedef struct _USPSectionDemuxCallbackData
{
	PBYTE m_pucData;		/**< Section����ָ��*/
	int m_nDataLen;			/**< Section���ݳ���*/
	PVOID m_pvAddData;		/**< �û����ݣ���USPSectionDemuxRequest�ȵ���ʱ��pvAppData����һ��*/
}USPSectionDemuxCallbackData;

/**@brief ������������ȼ��������ȼ���section��ȵ����ȼ���section��ǰ�ַ�����Demuxģ������Ч*/
typedef enum  
{
    SECTION_PRIORITY_HIGH,		/**< �����ȼ�*/
    SECTION_PRIORITY_MIDDLE,	/**< �����ȼ�*/  
    SECTION_PRIORITY_LOW,		/**< �����ȼ�*/
    NUM_SECTION_PRIORITIES		/**< ������ö�ٵ����һλ����ʾSection���ȼ��ĸ��������ɵ����ȼ���*/
} SectionPriority ;

/**
@brief Filter��Ϣ���ݽṹ

�ṩ��Ϣ�������ݵĹ��ˣ�����˹���Ϊ:
@code
IF ngate[i] == 0
{
	IF data[i] & mask[i] == match[i] & mask[i]
	{
		data[i] can be passed with the filter.
	}
	ELSE
	{
		Drop the data
	}
}
ELSE
{
	IF data[i] & mask[i] != match[i] & mask[i]
	{
		data[i] can be passed with the filter.
	}
ELSE
	{
		Drop the data
	}
}
@endcode
@note ��i��0��FILTER_LEN �� 1�����й�������������ʱ������filter�Ĺ�������������section�Żᱻ����
@note ��Ϊһ����˵�����ǲ����section length�ֶ������˴����������ǵĽӿ�Ҫ�󲻹���section�ĵ�2��3�����ֽ����ݡ������¶�Ӧ��ϵ��
- Data[0]    ����   MatchMask[0] 
- Data[1]    ����   ��
- Data[2]    ����   ��
- Data[3]    ����   MatchMask[1] 
- Data[4]    ����   MatchMask[2]
- Data[5]    ����   MatchMask[3]
- ����
@note �������������ֽڣ�ʵ��Filter��ȿ�ΪFILTER_LEN+1�ֽڣ���ʵ��ֻ�ܱ�֤ǰ16�ֽڵĹ��ˣ�����FILTER_LEN-2��FILTER_LEN-1�ֽڿ�����Ч
*/
typedef struct _MatchMask_S
{
    BYTE match[FILTER_LEN];	    /**< ƥ���ֶ�*/
    BYTE mask[FILTER_LEN];	    /**< ��ע�ֶ�*/
    BYTE negate[FILTER_LEN];	/**< ȡ���ֶ�*/
}MatchMask;

/**@brief Filter��Ϣ������Ϣ������Ҫ��������ݵ�����*/
typedef struct _RequestInfo_S
{	    
    DWORD dwPID;		        /**< �������ݵ�pid*/
    DWORD dwDemuxID;		    /**< �����������ݵ�Demux�豸���*/
    long lTimeout;		        /**< ��ʱʱ�䣬�������ʱ��ͻᷢ�ͳ�ʱ��Ϣ������Demux�ӿ���Ч*/
    void* pvAppData;		    /**< �û����ݣ�������Sectionʱ��������Ҳ��ԭ�ⲻ���ķ��ظ��û�������Demux�ӿ���Ч*/	
    BOOL bCRCCheck;		        /**< �Ƿ����CRC���*/
    SectionPriority enmPriority;/**< ����������ȼ� ������Demux�ӿ���Ч*/
    MatchMask MMFilter;		    /**< Filter ��Ϣ*/
	int m_nPES;					/**< SectionRequestType�ж������ֵ������Demux�ӿ���Ч*/
	int m_nTaskIndex;           /**< ���������̣߳�����Demux�ӿ���Ч*/
} RequestInfo;

/**@brief ���ݴ����ʽ��ͬCSAUDStreamType��CSVIDStreamType���������ݱ����ʽ����ͬһ���*/
typedef enum _CSSYSTransportType_E
{
	EM_TRANSPORT_TYPE_UNKNOW,	/**< unknown or not supported stream format */
	EM_TRANSPORT_TYPE_ES,		/**< Elementary stream */
	EM_TRANSPORT_TYPE_PES,		/**< Packetized elementary stream */
	EM_TRANSPORT_TYPE_TS,		/**< Transport stream */
	EM_TRANSPORT_TYPE_DSS_ES,
	EM_TRANSPORT_TYPE_DSS_PES,
	EM_TRANSPORT_TYPE_VOB,		/**< video object, used with DVD */
	EM_TRANSPORT_TYPE_ASF,		/**< Advanced Systems Format */
	EM_TRANSPORT_TYPE_AVI,		/**< Audio Video Interleave */
	EM_TRANSPORT_TYPE_MP4,	   	 /**< MP4 (MPEG-4 Part12) container */
	EM_TRANSPORT_TYPE_FLV		/**< Flash video container */
} CSSYSTransportType_E;

/**@brief  Descrambler �������Ͷ��壬���ڸı��Լ����ü�������**/
typedef enum _CSDescramblerType_E
{
    CS_DESCRAMBLER_TYPE_DVB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_ECB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_CBC_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_ECB_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_CBC_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_OFB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DES_CTS_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_FASTI_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_ECB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_CBC_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_ECB_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_CBC_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_OFB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_MULTI2_CTS_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_ECB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CBC_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_ECB_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CBC_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_OFB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CTS_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CTS_SCHNEIER_DESCRAMBLER,    
    CS_DESCRAMBLER_TYPE_AES_NSA_MDD_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_NSA_MDI_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_IPTV_CSA_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_DVB_EAVS_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_ECB_LR_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_AES_CBC_LR_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_ECB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_CBC_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_ECB_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_CBC_DVS042_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_OFB_DESCRAMBLER,
    CS_DESCRAMBLER_TYPE_3DES_CTS_DESCRAMBLER
} CSDescramblerType_E;


/**@brief ֻ�е�CSSYSTransportType_E ֵΪES��PES����������ʱ,��ö����Ч,����ָ������Ƶ��������Ƶ��*/
typedef enum _CSSYSStreamType_E
{
	EM_STREAM_TYPE_UNKNOW,	/**< unknown or not supported stream type */
	EM_STREAM_TYPE_VIDEO,
	EM_STREAM_TYPE_AUDIO
} CSSYSStreamType_E;


/**@brief ��һ������ע��ʱ����Ĳ�������Ϊ����Ԥ����Ŀǰ��Ч*/
typedef struct _CSDMXOpenParams_S
{
	DWORD m_dwBufferSize; 	/**< ע�뻺�����Ĵ�С��Ŀǰ����Ϊ0*/
	DWORD m_dwAlignment; 	/**< ע�뻺�������䷽ʽ����ʾΪ2^m_dwAlignment����2��ʾ2^2=4�ֽڶ���;3��ʾ2^3=8�ֽڶ��룻
					            0��ʾʹ��Ĭ��ֵ��Ŀǰ��Ч������Ϊ0*/
	CSSYSTransportType_E m_dwTransportType; /**< ���ݴ����ʽ*/
	DWORD m_dwResvered0;	/**< ��CSSYSTransportType_E ֵΪES��PES����������ʱ���ò�������ָ��������.
								 ���嶨��μ�CSSYSStreamType_E�������������Ϊ0*/
	DWORD m_dwResvered1;	/**< ��������������Ϊ0*/
 }CSDMXOpenParams_S;

/**
@brief ����ע��ص�����

@param[in] pContext �û����ݣ���������ע������������m_pCallbackContext��
*/
typedef void (*CSDMXCallback)(void *pContext);

/**@brief ����ע���������Ų���*/
typedef struct _CSDMXStartParams_S 
{
	CSDMXCallback m_fnWriteCallback;/**< ��ע�뻺�������������Խ���ע��ʱ��ʹ�øûص�����֪ͨ�ϲ����ע���������
						                    ���Ƽ�ֱ���ڸûص��л�ȡBuffer����ע�룬����ܻ�����Ӱ�첥�ŵ�Ч��
						                �����ڸûص���֪ͨ�����߳̽���ע��*/
	void *m_pCallbackContext; 	    /**< �û����ݣ��ص������Ĳ���*/	
	DWORD m_dwResvered0;		    /**< ��������������Ϊ0*/
	DWORD m_dwResvered1;		    /**< ��������������Ϊ0*/
	DWORD m_dwResvered2;		    /**< ��������������Ϊ0*/
} CSDMXStartParams_S;

/**
@brief Demux����ص�����

����������ݵ���ʱ�������ص�
@param[in] enmType �ص�����
@param[in] pParam �ص�����,Ŀǰ�����еĻص����;�ΪΪָ��һ��USPSectionDemuxCallbackData���͵�ָ��
*/
typedef void( *USPSectionDemuxCallback ) ( USPSectionDemuxCallbackType enmType, void* pParam );

/*-------------���ݻ�ȡ�ӿ�----------------*/

/**
@brief ��ȡָ��Demux�豸��Filter����

���ۿ��л��߱�ռ��
@param[in] hHandle �豸���豸���������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@return ����Filter����,������-1
*/
int USPSectionGetDemuxFilterCount( HCSHANDLE   hHandle );

/**
@brief ��ȡָ��Demux�豸�Ŀ���Filter����

@param[in] hHandle �豸���豸���������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@return ���ؿ��е�Filter����,������-1
*/
int USPSectionGetDemuxFreeFilterCount( HCSHANDLE   hHandle );

/**
@brief ����Filter��������

����������PSI��SI��Ҳ�����ڹ���PES��RAW������
���ǽ�֧��PSI��SI�İ汾
@param[in] pRequestInfo Filter����
@param[in] pDmxCallback �ص����������з������������ݵ���ʱ�����ñ�����֪ͨ�����лص�������ͬһ�߳��Ҳ��ڵ����̷߳���
@param[in] pvAppData �û����ݣ����ڻص�ʱ�����ص����ݵ�m_pvAddData�ֶ�
@return ���ص�������ľ�����������ʧ���򷵻�-1(0����Ч��)
@note ������������ӿ�֮��Ĺ�ϵ��ο����µġ���ϸ����������
@note RequestInfo��Ҳ��һ��pvAppData�ֶΣ����ϲ�ģ��ʹ�õģ��������
@note ��ϸ�Ĺ��˹�����ο�RequestInfo�ṹ�Ķ���
@note ��Ӧ���豸�����RequestInfo�ṹ��
@note Request�ɹ��󣬼���ʼ���ܡ�������Ӧ����
*/
int USPSectionDemuxRequest( RequestInfo *pRequestInfo, USPSectionDemuxCallback pDmxCallback, PVOID pvAppData );

/**
@brief ����Filter��������

����������Section��Ҳ�����ڹ���PES��RAW������
����֧��PES�İ汾
@param[in] bRequestPES ����PES���ݱ��,TRUE��ʾ����PES��FALSEʱ����USPSectionDemuxRequest
@param[in] pRequestInfo Filter����
@param[in] pDmxCallback �ص����������з������������ݵ���ʱ�����ñ�����֪ͨ�����лص�������ͬһ�߳��Ҳ��ڵ����̷߳���
@param[in] pvAppData �û����ݣ����ڻص�ʱ�����ص����ݵ�m_pvAddData�ֶ�
@return ���ص�������ľ�����������ʧ���򷵻�-1
@note ������������ӿ�֮��Ĺ�ϵ��ο����µġ���ϸ����������
@note RequestInfo��Ҳ��һ��pvAppData�ֶΣ����ϲ�ģ��ʹ�õģ��������
@note ��ϸ�Ĺ��˹�����ο�RequestInfo�ṹ�Ķ���
@note ��Ӧ���豸�����RequestInfo�ṹ��
@note Request�ɹ��󣬼���ʼ���ܡ�������Ӧ����
*/
int USPSectionDemuxRequestEx( BOOL bRequestPES, RequestInfo *pRequestInfo, USPSectionDemuxCallback pDmxCallback, PVOID pvAppData );

/**
@brief ����Filter��������

����������Դ֮ǰ�Ѿ������
@param[in] hHandle ����������,USPSectionDemuxRequest��USPSectionDemuxAllocateSectionFilter�ķ���ֵ
@param[in] nFilter Filter���(USPSectionDemuxRequest���ص���Ч���)
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ������������ӿ�֮��Ĺ�ϵ��ο����µġ���ϸ����������
@note Start�ɹ��󣬼���ʼ���ܡ�������Ӧ����
*/    
BOOL USPSectionDemuxStart( HCSHANDLE hHandle, int nFilter );

/**
@brief ֹͣFilter��������,�������ͷ�Filter��Դ

��ֹͣ�����ͷ���Դ
@param[in] hHandle ����������,USPSectionDemuxRequest��USPSectionDemuxAllocateSectionFilter�ķ���ֵ
@param[in] nFilter Filter���(USPSectionDemuxRequest���ص���Ч���)
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ������������ӿ�֮��Ĺ�ϵ��ο����µġ���ϸ����������
@note Stop�ɹ��󣬼�ֹͣ���ܡ�������Ӧ����
*/
BOOL USPSectionDemuxStop(HCSHANDLE   hHandle, int nFilter );

/**
@brief ֹͣFilter��������,���ͷ�Filter��Դ

ֹͣ�����ͷ���Դ
@param[in] hHandle ����������,USPSectionDemuxRequest��USPSectionDemuxAllocateSectionFilter�ķ���ֵ
@param[in] nFilter Filter���(USPSectionDemuxRequest���ص���Ч���)
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note ������������ӿ�֮��Ĺ�ϵ��ο����µġ���ϸ����������
@note Cancel�ɹ��󣬼�ֹͣ���ܡ�������Ӧ����
*/
BOOL USPSectionDemuxCancel(HCSHANDLE   hHandle, int nFilter );

/**
@brief ����Filter��Դ������Fillter������������ʼ��ȡ�������

��������Դ��������
@param[in] bRequestPES bRequestPES ,TRUE��ʾ����PES��FALSEʱ����PSI��SI
@param[in] pRequestInfo Filter����
@param[in] pDmxCallback �ص����������з������������ݵ���ʱ�����ñ�����֪ͨ�����лص�������ͬһ�߳��Ҳ��ڵ����̷߳���
@param[in] pvAppData �û����ݣ����ڻص�ʱ�����ص����ݵ�m_pvAddData�ֶ�
@return ���ص�ǰ����ľ�����������ʧ���򷵻�-1
@note ������������ӿ�֮��Ĺ�ϵ��ο����µġ���ϸ����������
@note RequestInfo��Ҳ��һ��pvAppData�ֶΣ����ϲ�ģ��ʹ�õģ��������
@note ��ϸ�Ĺ��˹�����ο�RequestInfo�ṹ�Ķ���
@note ��Ӧ���豸�����RequestInfo�ṹ��
@note Alloca�ɹ��󣬲�����ʼ���ܡ�������Ӧ����
*/
int USPSectionDemuxAllocateSectionFilter( BOOL bRequestPES, RequestInfo *pRequestInfo, USPSectionDemuxCallback pDmxCallback, PVOID pvAppData );

/**
@brief �޸�Fillter����

������Stop״̬�²��ܵ���
@param[in] hHandle ����������,USPSectionDemuxRequest��USPSectionDemuxAllocateSectionFilter�ķ���ֵ
@param[in] nFilter Filter���(USPSectionDemuxRequest���ص���Ч���)
@param[in] pRequestInfo Filter���� 
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note �豸�����hHandle����Ϊ׼������RequestInfo�еľ������
@note RequestInfo��Ҳ��һ��pvAppData�ֶΣ����ϲ�ģ��ʹ�õģ�������ᣬҲ�������޷��ı�pvAppData
*/
BOOL USPSectionDemuxModifySectionFilter(HCSHANDLE hHandle, int nFilter, RequestInfo *pRequestInfo);

/**
@brief ����Fillter�ĵײ�buffer��С

��Ҫ������Ҫ���˴�������,�ϲ���ܻ�������ȡ�ߵ����
@param[in] hHandle �豸���豸���������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[in] nFilter Filte�����ʹ������Filterʱ�ķ�����USPSectionDemuxAllocateSectionFilter
@param[in] nBufSize Ҫ���õĵײ�buffer�Ĵ�С������Ϊ2��n�η�����С1K�����ֵ���ݸ���ƽ̨���첻ͬ
@return ����ɹ��򷵻� TRUE,ʧ���򷵻� FALSE.
@note �޸�Filter size��filter�账��Stop��pause��״̬�����޸�filter��	
*/
BOOL USPSectionDemuxSetFilterBufSize(HCSHANDLE hHandle,int nFilter,int nBufSize);

/*-------------����ע��ӿ�----------------*/

/**
@brief ��һ������ע��ͨ��

���ӿ����ڲ��ű��ء�IP�ȷ�Tunerͨ����ȡ������Ƶ����\n
�������ϲ�ֱ��ע�뵽�ײ㣬�ɵײ������Ӧ�Ľ⸴��(��ѡ)������(��ѡ)������Ȳ���
@param[in] dwPath �豸�����������������Ŵ�ע�����ݵ�������Ҫ���豸�������ﲢ��һ����Ҫ��飬ȱ���豸���ں���������ʧ��
@param[in] pOpenParams ����ע��ͨ�������������CSDMXOpenParams_S�ṹ���壬Ŀǰ�ò�����Ч�����봫NULL���������ݾ�Ϊָ��ֵ
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
@note ����ͨ�������ã������󣬲������Ͽ�ʼ���ţ���Ҫ�ϲ�Ը��豸���ϵ������豸���в�������������
*/
BOOL CSDMXOpen(DWORD dwPath,const CSDMXOpenParams_S* pOpenParams);

/**
@brief �ر�һ������ע��ͨ��

�ͷ�Demux��Դ��������Դ�������豸�����У��ɹ����豸������������������
@param[in] dwPath �豸�������ͬCSDMXOpen�ĸò���
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note ͨ���رպ�����Ҫ�ϲ���豸���ϵ������豸������Ӧ�Ĵ�����رյ�
*/
BOOL CSDMXClose(DWORD dwPath);

/**
@brief ��ʼ����ע��

���ӿڳɹ����غ󣬼��ɿ�ʼ��������ע��
@param[in] dwPath �豸�������ͬCSDMXOpen�ĸò���
@param[in] pStartParams �������Ʋ���
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note Start�󲢲������̲����ص�֪ͨ��д������������д��һ�����ݺ���п����յ�֪ͨ
@note ͬʱ��ĳ��֪ͨ��������������ԭ��δ�ɹ�д�룬Ҳ�����ٴβ���֪ͨ�����ϲ����н��лָ�����
*/
BOOL CSDMXStart(DWORD dwPath,CSDMXStartParams_S* pStartParams);

/**
@brief ֹͣ����ע��

@param[in] dwPath �豸�������ͬCSDMXOpen�ĸò���
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
*/
BOOL CSDMXStop(DWORD dwPath);

/**
@brief ����ע�븴λ

��λ����ע�룬�����ע�������
@param[in] dwPath �豸�������ͬCSDMXOpen�ĸò���
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note ����ע������쳣�����������Ҫ������ע������ʱ���и�λ
*/
BOOL CSDMXReset(DWORD dwPath);

/**
@brief �������ע�뻺�������ڴ�ͻ��������еĴ�С

��ȡ����ע��Ļ����������ϲ�ʹ�á���IPTV�н�socket ���յ�����ֱ��д��Demux�Ļ������С�
@param[in] dwPath �豸�������ͬCSDMXOpen�ĸò���
@param[out] ppBuffer ���ػ�������д���ݵ�ַ
@param[out] pSize ���ػ��������еĴ�С
@return �õ�����������TRUE;���򷵻�FALSE
@note ������CSDMXWriteComplete�ɶԵ��ã�����CSDMXGetFreeBufferû�еõ��κλ�����
@note �ýӿ��ǲ������ģ��������ʱû�п��ÿռ���*ppBuffer ��ֵ������NULL������*pSize =0
@note ��TSע��ģ���п���ռ�ʱm_fnWriteCallback������������
@note ���Ƽ���֪ͨ�ص���ֱ�ӵ��øú�������ע�룬�Ƽ��ڻص��з���Ϣ֪ͨ������߳̿�ʼע��
*/
BOOL  CSDMXGetFreeBuffer(DWORD dwPath, void **ppBuffer, int*pSize);

/**
@brief ֪ͨ�ײ㱾��ע�����д������ݺ����������ݴ�С�����޸�Demux������ָ��

@param[in] dwPath �豸�������ͬCSDMXOpen�ĸò���
@param[out] nSkipSize ����ע��������ԵĻ��������ݴ�С���������ϴλ�ȡ���Ļ������ڴ��ǰnSkipSize�ֽڵ�����
@param[out] nUsedSize ����ע���ʵ�����ݵĴ�С����ע�����Ч���ݵĴ�С�������ݴ�����ϴλ�ȡ�Ļ�������ʼ��ַƫ��nSkipSize�ֽڵ�λ��
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note ������CSDMXGetFreeBuffer�ɶԵ��ã�����CSDMXGetFreeBufferû�еõ��κλ�����
@note 0 <= nSkipSize+nUsedSize < CSDMXGetFreeBuffer����pSize
*/
BOOL CSDMXWriteComplete(DWORD dwPath, int nSkipSize, int  nUsedSize);

/*-------------���Žӿ�----------------*/

/**
@brief ����һ�����ŵ�ͨ��ID

@param[out] pdwDescrambleID ����Ľ���ͨ��ID
@return ����ɹ�����0;����ʧ��-1
*/
int CSDSMAllocateDescramble(DWORD *pdwDescrambleID);

/**
@brief ����һ�����ŵ�ͨ��ID

��ָ����Demux�豸�Ϸ���
@param[in] hHandle Demux�豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�������
@param[out] pdwDescrambleID ����Ľ���ͨ��ID
@return ����ɹ�����0;����ʧ��-1
*/
int CSDSMAllocateDescrambleEX(HCSHANDLE hHandle ,DWORD *pdwDescrambleID);

/**
@brief �ͷŷ���Ľ��ŵ�ͨ��ID

@param[in] dwDescrambleID ����ͨ��ID,CSDSMAllocateDescramble���������
@return �ɹ� 0;ʧ�� -1
*/ 
int CSDSMFreeDescramble(DWORD dwDescrambleID );

/**
@brief Ϊ���ŵ�ͨ��IDָ�����˵�PID

@param[in] dwDescrambleID ����ͨ��ID,CSDSMAllocateDescramble���������
@param[in] wPID ����PID
@return �ɹ� 0;ʧ�� -1
@note ���ý���PIDǰ��Ҫ���ȴ򿪸�PIDͨ��(�������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ)�ǿ��Խ��ܵģ�ʹ��������Ҫע���˳������
*/ 
int CSDSMSetDescramblePID(DWORD dwDescrambleID,WORD wPID );

/**
@brief ��ս���ͨ�����˵�PID

@param[in] dwDescrambleID ����ͨ��ID,CSDSMAllocateDescramble���������
@param[in] wPID ����PID
@return �ɹ� 0;ʧ�� -1
*/ 
int CSDSMClearDescramblePID(DWORD dwDescrambleID,WORD wPID );

/**
@brief ���ý���ͨ������Keyֵ

@param[in] dwDescrambleID ����ͨ��ID,CSDSMAllocateDescramble���������
@param[in] pbOddKey ���ŵ���KEY
@param[in] bOddLength Key����
@return �ɹ� 0;ʧ�� -1
@note ��keyǰ��Ҫ���ȴ򿪸�PIDͨ��(�������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ)�ǿ��Խ��ܵ�
*/ 
int CSDSMDescrambleSetOddKeyValue(DWORD dwDescrambleID, BYTE* pbOddKey, BYTE bOddLength);

/**
@brief ���ý���ͨ����żKeyֵ

@param[in] dwDescrambleID ����ͨ��ID,CSDSMAllocateDescramble���������
@param[in] pbEvenKey ���ŵ�żKEY
@param[in] bEvenLength Key����
@return �ɹ� 0;ʧ�� -1
@note ��keyǰ��Ҫ���ȴ򿪸�PIDͨ��(�������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ)�ǿ��Խ��ܵ�
*/ 
int CSDSMDescrambleSetEvenKeyValue(DWORD  dwDescrambleID, BYTE *pbEvenKey, BYTE bEvenLength);


/**
@brief ���ý���ͨ���Ľ�������

@param[in] dwDescrambleID ����ͨ��ID,CSDSMAllocateDescramble���������
@param[in] eDescrambleType ������������
@return �ɹ� 0;ʧ�� -1
@note �������ô˽ӿڣ�Ĭ�������Descramble TypeΪDVB
*/ 
int CSDSMSetDescrambleType(DWORD dwDescrambleID , CSDescramblerType_E  eDescrambleType);

/**
@brief ����System Key,��Multi2���ŵ�ʱ��Ҫ���ô˽ӿڡ�SystemKey���Կ�����һ��Demux��һ��ȫ��������ͨ���޹ء�

@param[in] hDmxHandle Demux�豸�����������Ӧ��ͨ����CSRMAlloc()��CSRMConnect()�����������CSDSMAllocateDescrambleEX�ĵ�һ��������һ�µġ�
@param[in] pbSystemKey SystemKey�ĵ�ַ������ΪMulti2��׼�涨��32B
@return �ɹ� 0;ʧ�� -1
@note �ýӿڿ������κν�����������ǰ���ñ�����
	���һ��Demux�ж���������������н�������SystemKey��һ���ģ�
	���������ֻ���һ��CSDSMDescrambleSetSystemKey��
*/ 
int CSDSMDescrambleSetSystemKey(HCSHANDLE  hDmxHandle, BYTE * pbSystemKey);

#ifdef  __cplusplus
}
#endif

/** @} */

#endif

