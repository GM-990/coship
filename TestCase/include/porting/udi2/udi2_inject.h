/**@defgroup INJECTER INJECTER ��������������ע����ؽӿ�
@brief ��ģ����Ҫ�����˶����˸�������ע�����ؽӿڡ�
-ע�벥�ű��뱣֤������ӿڵ���˳�������������
1.��ʼע�벥�ţ� CSUDIINJECTEROpen -> CSUDIPLAYEROpen -> CSUDIINJECTERStart -> CSUDIPLAYERStart
2.ֹͣע�벥�ţ� CSUDIPLAYERStop ->	CSUDIINJECTERStop -> CSUDIPLAYERClose -> CSUDIINJECTERClose
��ϸ�����μ�PLAYERģ�顣


@version 2.0.2 2010/11/24 ����ע�벥��˳������ע��
@version 2.0.1 2009/08/15 
@{
*/
#ifndef _UDI2_INJECTER_H_
#define _UDI2_INJECTER_H_

#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif 

/**@brief ����ע����ش������ֵ*/
enum
{
	CSUDIINJECTER_ERROR_BAD_PARAMETER = CSUDI_INJECT_ERROR_BASE,  ///< �������
	CSUDIINJECTER_ERROR_FEATURE_NOT_SUPPORTED,                    ///< ��֧�ֲ���
	CSUDIINJECTER_ERROR_UNKNOWN_ERROR,							  ///< һ����� 
	CSUDIINJECTER_ERROR_NO_MEMORY,								  ///< �޿����ڴ�
	CSUDIINJECTER_ERROR_INVALID_DEVICE_ID,						  ///< �Ƿ��豸ID
	CSUDIINJECTER_ERROR_INVALID_HANDLE,							  ///< �Ƿ����	
	CSUDIINJECTER_ERROR_INVALID_STATUS,							  ///< �Ƿ�״̬
	CSUDIINJECTER_ERROR_DEVICE_BUSY,							  ///< �豸��æ
	CSUDIINJECTER_ERROR_DEVICE_DISORDER,						 ///<�豸���󣬱���˵ע��TS��ʱ��demux=-1,�����ǶԵģ����߼�����
	CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE,	///<ע��������ע�����ݲ���
	CSUDIINJECTER_ERROR_ALREADY_STARTED,								///< ��ע���Ѿ�����
	CSUDIINJECTER_ERROR_ALREADY_STOPPED,								///< ��ע���Ѿ�ֹͣ
	CSUDIINJECTER_ERROR_NOT_STARTED									///< ��ע���Ѿ�ֹͣ

};

/**@brief Injecterö�����͵Ķ���*/
typedef enum 
{
	EM_INJECTER_PCM, 	      ///<  PCMע�벥���� 
	EM_INJECTER_ES,		      ///<  ESע�벥���� 
	EM_INJECTER_PES,	      ///<  PESע�벥���� 
	EM_INJECTER_TS,		      ///<  TSע�벥���� 
	EM_INJECTER_AVI,	      ///<  AVIע�벥���� 
	EM_INJECTER_RM,	          ///<  RMע�벥���� 
	EM_INJECTER_RMVB,	      ///<  RMVBע�벥���� 
	EM_INJECTER_PS,			  ///<  PSע�벥���� 
	EM_INJECTER_MAX               ///<���� ��
}CSUDIINJECTERType_E;


/**@brief Injecter ע���ý�����ݵ�ö�ٶ���*/
typedef enum 
{
	EM_UDIINJECTER_CONTENT_DEFAULT,		///<  Injecterע������ݣ�defaultΪȱʡ���� ��ָTS�Ȳ���Ҫ��û�а취�ֱ������Ƶ����Ƶ����ͼ����Ļ��Ϣ
	EM_UDIINJECTER_CONTENT_AUDIO,		///<   Injecterע�������Ϊ��Ƶ����
	EM_UDIINJECTER_CONTENT_VIDEO,		///<   Injecterע�������Ϊ��Ƶ���� 
	EM_UDIINJECTER_CONTENT_SUBTITLE,		///<   Injecterע�������Ϊsubtitle���� 
	EM_UDIINJECTER_CONTENT_TELETEXT,	///<   Injecterע�������Ϊteletext���� 
	EM_UDIINJECTER_CONTENT_IFRAME,	///<   Injecterע�������Ϊ��ƵIFrame���� 
	EM_UDIINJECTER_CONTENT_MAX             ///< ���ޡ�
}CSUDIINJECTERContentType_E;


/**@brief ����PCM���ݲ����Ľṹ����*/
typedef struct
{
	unsigned int m_uBitsSample;	///< ����λ��:8,16,32
	unsigned int m_uSampleRate;	///< ������:32000,44100,48000 
	unsigned int m_uChannels;  	///< ͨ����:1-mono,2-stereo 
}CSUDIINJECTERPcmStartParam_S;


/**@brief ƽ̨֧�ֵ�PCM�ֽ������*/
typedef enum 
{
	EM_UDIINJECTER_ERROR_ENDIAN,             ///<  ���� ��
	EM_UDIINJECTER_LITTLE_ENDIAN,		///< С���ֽ��� 
	EM_UDIINJECTER_BIG_ENDIAN			///< ����ֽ��� 
	
}CSUDIINJECTERPcmEndian_E;


/**@brief ͬ����ʽѡ���ö�����Ͷ���*/
typedef enum 
{
	EM_UDIINJECTER_SYNC_PCR,			///<PCRͬ��ģʽ
	EM_UDIINJECTER_SYNC_AUDIO_FIRST,	///<��ƵPTSΪʱ�ӻ�׼��ͬ��ģʽ
	EM_UDIINJECTER_SYNC_VIDEO_FIRST,	///<��ƵPTSΪʱ�ӻ�׼��ͬ��ģʽ
	EM_UDIINJECTER_SYNC_NONE			///<����Ҫͬ��
}CSUDISYNCMode_E;

/**@brief Injecter ͬ�������ṹ����*/
typedef struct
{

	CSUDI_HANDLE m_hSyncHandle;	    ///< ��Ϊͬ����׼��ע�������þ����CSUDIINJECTEROpen�õ�
	CSUDISYNCMode_E m_eSyncMode;	///< ָ��ͬ��ģʽ��ͬ��ģʽ�����ݣ���μ�CSUDISYNCMode_E
	
}CSUDIPESSYNCMode_S;


/**@brief Injecter Buffer�����ṹ����*/
typedef struct
{
	int     m_nFIFOBufferSize;		///< Injecter��Ŀ�껺��������������������ȣ������ֵΪ0���ʾӦ�ò����ĳ��ȣ����������о���
}CSUDIINJECTERBuffer_S;

/**@brief Injecter Buffer״̬��Ϣ�ṹ����*/
typedef struct
{
	int 	  m_nAvailableDataSize;			///< Injecter�Ѿ�ע�뵽Ŀ�껺������������������У�����δ�����ĵ������ݳ���(��λ�ֽ�)��������ݳ�ʼֵΪ0���ܹ���ע�롢��������б���ѯ
}CSUDIINJECTERStatus_S;

/**@brief Injecter ���Բ������͵�ö�ٶ���*/
typedef enum
{
	EM_UDIINJECTER_STATUS,		   ///< ��ȡINJECTER����״̬��Ϣ ����ο�CSUDIINJECTERStatus_S����ֵΪֻ��
	EM_UDIINJECTER_BUFFER,    	   ///< Injecter������buffer��Ϣ��������μ�CSUDIINJECTERBuffer_S����ֵΪֻ��
	EM_UDIINJECTER_CALLBACK,	   ///< ����Injecter�ص��������ݲ���ʵ��
	EM_UDIINJECTER_PES_SYNC,	   ///< ���ע����֮���ͬ����Ϣ��������μ�CSUDIPESSYNCMode_S����ֵ�ɶ�д	
	EM_UDIINJECTER_PCM_PARAMS,     ///< PCMע�벥��������������������μ�CSUDIINJECTERPcmStartParam_S����ֵ�ɶ�д
	EM_UDIINJECTER_PCM_ENDIAN,     ///< PCM����ע�����ݵĴ�С���趨��������μ�CSUDIINJECTERPcmEndian_E����Ĭ��ֵΪƽ̨�Ƽ�ֵ����ֵ�ɶ�д
	EM_UDIINJECTER_IFRAME_PARAMS   ///< IFrame���ݲ�����������μ�CSUDIINJECTERIFrameParam_S����ֵ�ɶ�д
}CSUDIINJECTERAttributeType_E;

/**@brief Injecter ע��ͨ������Դ���ݽṹ����*/
typedef struct
{
	int m_nDemux;		    ///< Injecter�õ���demux ID��Ϣ����TS��ע�������£���Ҫָ��demux ID��-1������Чֵ�����û���õ�Demux����ò�����Ϊ-1
	int m_nVideoDecoder;        ///< Injecter�õ�����Ƶ������ID��Ϣ��-1������Чֵ�����û���õ�Video decoder����ò�����Ϊ-1
	int m_nAudioDecoder;        ///< Injecter�õ�����Ƶ������ID��Ϣ��-1������Чֵ�����û���õ�Audio decoder����ò�����Ϊ-1
	
}CSUDIINJECTERChnl_S;

/**@brief Injecter ע��IFrame���ݲ��� */
typedef struct
{
	const char *m_pcIFrameData;		    ///< I֡���ݵĵ�ַ
	int m_nDataLen;        							///< I֡���ݵĵ�ַ�ĳ���
}CSUDIINJECTERIFrameParam_S;


/**@brief Injecter ע��ͨ�����ͽṹ����*/
typedef struct 
{
 	CSUDIINJECTERType_E 		m_eInjecterType;  ///< Injecterʵ�����ͣ����CSUDIINJECTERType_E
	CSUDIINJECTERContentType_E 	m_eContentType;  ///< Injecter ע���ý����������
}CSUDIINJECTEROpenParam_S;

/**
@brief ����һ��INJECTERע��ʵ��,ΪINJECTERע������׼��
@param[in] psInjecterChnl ����ע��ͨ�����ݽṹָ�롣
@param[in] psOpenParams ��Ҫ������INJECTERʵ�������ͣ���Ҫ����PCM,TS,PES,ES�ȣ�������μ�CSUDIINJECTERType_E
@param[out] phINJECTER ���δ򿪵�INJECTERע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note psInjecterChnl�ṹ���е��豸��������Ϊ��Чֵ(-1),������豸����Ҫ�򲻴���
@note Demux��������Tuner�Ͽ����ӣ��������ڴ���Inject����һ��Demux����ͬʱӵ������Դ�豸
@note ����Inject�뾡��ʹ�����ٵ��豸���磺
- ע��TS����ʱ��ע������ֻ����EM_UDIINJECTER_CONTENT_DEFAULT�����򷵻�CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- ע��PCM��ʱ��ע������ֻ����EM_UDIINJECTER_CONTENT_AUDIO�����򷵻�CSUDIINJECTER_ERROR_STREAMTYPE_NOT_MATCH_INJECTERTYPE
- ע��TS����ʱ��ע��ͨ��ֻ��ѡDemuxID������ƵID��Ϊ-1�����򷵻�CSUDIINJECTER_ERROR_DEVICE_DISORDER
- ע��PCM��ʱ��ע��ͨ��ֻ��ѡAudioID����ƵID��DemuxID��Ϊ-1�����򷵻�CSUDIINJECTER_ERROR_DEVICE_DISORDER
- ע��I֡��ʱ��ע��ͨ��ֻ��ѡVideoID,��ƵID��DemuxID��Ϊ-1�����򷵻�CSUDIINJECTER_ERROR_DEVICE_DISORDER
- AVI,RM,RMVB, ���漰���Ŷ�ý���ļ����ŵ�ע��ɲ�֧�֡����оƬ�����Զ�ý���ļ������и��õ�API�����֪ͬ�ޣ�
	ͬ�޿ɶԴ�API����������������ʣ����װ�ϲ�ӿ�ֱ�ӵ�����API.�Ͳ�����ʵ�ֵײ��ע��֧�֡�
*/
CSUDI_Error_Code  CSUDIINJECTEROpen(const CSUDIINJECTERChnl_S * psInjecterChnl, const CSUDIINJECTEROpenParam_S * psOpenParams, CSUDI_HANDLE * phINJECTER);

/**
@brief �ر�һ��INJECTERע��ʵ��,�ͷ������Դ
@param[in] hINJECTER INJECTERע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code  CSUDIINJECTERClose(CSUDI_HANDLE hINJECTER);


/**
@brief ����INJECTERע��ʵ����������Ϣ������������INJECTERʵ�������ԡ���ز���������Դ����Ϣ��
@param[in] hINJECTER ע��ʵ���������CSUDIINJECTEROpen����
@param[in] eAttrType ����Ҫ�������������ͣ���μ�CSUDIINJECTERAttributeType_E����ϸ˵��
@param[in] pvData ������������ݽṹָ�룬������μ�CSUDIINJECTERAttributeType_E����ϸ˵����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����Ѿ�����CSUDIINJECTERStart��������ע�룬�������ٵ��ô˽ӿڶԻص������������á�
@note �����֧�����ûص����������ô˽ӿڽ����ز�֧�ֲ���
*/
CSUDI_Error_Code CSUDIINJECTERSetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, const void * pvData);


/**
@brief ��ȡINJECTERע��ʵ���ĸ�������Ϣ��
@param[in] hINJECTER ע��ʵ���������CSUDIINJECTEROpen����
@param[in] eAttrType ����Ҫ�������������ͣ���μ�CSUDIINJECTERAttributeType_E����ϸ˵��
@param[out] pvData ������������ݽṹָ�룬������μ�CSUDIINJECTERAttributeType_E����ϸ˵����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIINJECTERGetAttribute(CSUDI_HANDLE hINJECTER, CSUDIINJECTERAttributeType_E eAttrType, void * pvData);

/**
@brief  ��ʼһ��INJECTER ���� ע��
@param[in] hINJECTER INJECTERע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  ���ע��ʵ���Ѿ���ʼ���򷵻�CSUDIREC_ERROR_ALREADY_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStart(CSUDI_HANDLE hINJECTER);


/**
@brief ֹͣһ��INJECTER ����ע��
@param[in] hINJECTER INJECTERע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  ���ע��ʵ���Ѿ���ֹͣ���򷵻�CSUDIREC_ERROR_ALREADY_STOPPED
@note  ���ע��ʵ��û���������򷵻�CSUDIREC_ERROR_NOT_STARTED
*/
CSUDI_Error_Code CSUDIINJECTERStop(CSUDI_HANDLE hINJECTER);

/**
@brief�ȴ�Inecter�Ѿ�ע�뵽Ŀ�껺�����е����ݲ������

�������������һֱ�ȵ������е����ݲ�������˲ŷ���CSUDI_SUCCESS�����²�������ʱ����CSUDI_FAILURE
һ�����ڴ���������ȫ��ע����Ϻ󣬵ȴ����Ž���
@param[in] hINJECTER INJECTER����ע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  
*/
CSUDI_Error_Code CSUDIINJECTERFlush(CSUDI_HANDLE hINJECTER);

/**
@brief ���Inecter�Ѿ�ע�뵽Ŀ�껺�����еģ���û�в������ȫ������

��Ҫ���ڿ�������ˡ���λ�Ȳ���ʱ�����ע������ݣ��ѷ�ֹ��Щ�������ӳ�
@param[in] hINJECTER INJECTER����ע��ʵ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  
*/
CSUDI_Error_Code CSUDIINJECTERClear(CSUDI_HANDLE hINJECTER);

/**
@brief ��ȡ�´�ע�뵽InjecterĿ�껺�����еĿ���Buffer��ַָ�����������Buffer�Ĵ�С��
@param[in] hINJECTER INJECTER����ע��ʵ�����
@param[out] ppvBuffer ���ػ�ȡ�����ڴ��ַָ��
@param[out] puLength ���ػ�ȡ�����ڴ泤��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note  ���buffer�Ѿ�������ȥ���ô˽ӿ�Ӧ�÷���ʧ�ܣ���puLength����Ϊ0
@note  �ú���������CSUDIINJECTERWriteComplete���ʹ�ã��Ըú����ĵ���Ҫ�����̰߳�ȫ
@code 
	char  	*pcBuf = NULL; 
	unsigned int unBufSize = 0;
	...
	if( (CSUDIINJECTERGetFreeBuffer(hInject, (void *)&pcBuf, &unBufSize) == CSUDI_SUCCESS) && (unBufSize > 0))
	{
		//��buffer��д������,  �˴�����unWriteLen <= unBufSize
		memcpy(pcBuf, pcDataTobeWrite, unWriteLen );		
		CSUDIINJECTERWriteComplete(hInject, unWriteLen);
	}
	...
@endcode
*/
CSUDI_Error_Code CSUDIINJECTERGetFreeBuffer(CSUDI_HANDLE hINJECTER, void ** ppvBuffer, unsigned  int * puLength );

/**
@brief ֪ͨ����ע�������
@param[in] hINJECTER INJECTER����ע��ʵ�����
@param[in] uWrittenSize �ѿ������ݵĴ�С
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���buffer����ΪnLength, ע�볤�ȴ���nLength,��ýӿڷ���CSUDIINJECTER_ERROR_NO_MEMORY
@note  �ú���������CSUDIINJECTERGetFreeBuffer���ʹ�ã��Ըú����ĵ���Ҫ�����̰߳�ȫ
*/
CSUDI_Error_Code CSUDIINJECTERWriteComplete(CSUDI_HANDLE hINJECTER, unsigned int uWrittenSize);



#ifdef  __cplusplus
}
#endif
/**@}*/
#endif
