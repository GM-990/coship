/**@defgroup SECTION SECTION ���ݹ�����ؽӿڶ���
@brief Sectionģ���Ӧͷ�ļ�<udi2_section.h>�������ȡDVB PSI��SI��PES��RAW���ݵĽӿڡ�

��ģ����FILTER������Ϊ���Ƶ�λ��ÿ��һ��Demux������һ�������������о�����Demux��
�ҷ��Ϲ������������ݶ���ͨ��Allocateʱ���õĻص����������������еĻص�������ͬһ
���߳��в�����\n
���������������ڷ�ΪReady��Running��Cancel����״̬��������ͨ����ģ���ṩ�ļ�����
������Щ״̬���л������Ӧ��ϵ����ͼ��ʾ��
- �������ڹ���EM_UDI_REQ_SECTION_DATA���͵�����ʱ���������ڶ����ֽ�Data[1]�͵���
���ֽ�Data[2]�����ݣ���Filter[0]��Data[0]��Ӧ��Filter[1]��Data[3]��Ӧ��Filter[2]
��Data[4]��Ӧ��Filter[3]��Data[5]���������ơ���ΪData[1]��Data[2]�������ֽ���
Section�ĳ��ȣ����Բ����й��ˡ�
- �������ڹ����������͵�����ʱ����������CSUDIFILTERMatchMask_S������Ч��
@image HTML udi_section_state.png

@brief �����ƽ̨�ϣ���ع��ܶ���һ��ͨ���ĸ����һ��Demux�Ͽ��ԹҶ��ͨ������Ӧ
һ��PID����һ��ͨ�����ٹҶ��Filter����ͼ����������ģ�͡���������£���ģ�����
עFilter����ʵ���账���UDI��ͨ�ù�����ģ�ͼ��ת������������һ����Filterʱ����ʵ��
���ֶ�Ӧ��ͨ��δ����������Ҫ���ȴ���֮�����Ѿ����������������Filter���ɡ�ͬ������
Ҫɾ��һ��Filterʱ��ʵ�����ע����ͨ������û�������κ�Filter����ͬʱ�ر�ͨ������֮��
����ͨ����
@image HTML udi_section_channel.png

@note
- ��ΪNDS CAʹ��ICAMģ���ȡSection���ݣ�������Ҫ�ر�ע��ICAMģ����Sectionģ��֮���
Э������ֹ��Դ���ڳ�ͻ��
- ��ͬ�Ĺ�����֮�䣬���ܴ��ڲ������ݵ��ص�����������£�����Ӧ֪ͨ�����й���������ʱ
��Ӧ�Ļص������������ǽ���֪ͨ��һ����

@version 2.0.4 2009/09/15 ȥ��CSUDISECTIONCallbackData_S�ṹ���ж����m_pvAddData
@version 2.0.3 2009/09/14 �����ڷǷ�״̬���ýӿڷ���ֵ�ľ���˵��
@version 2.0.2 2009/08/25 �����˹�����ͨ����˵��,�����˹�����״̬ת��˵��
@version 2.0.1 2009/08/17 ����
@{
*/

#ifndef _UDI2_SECTION_H_
#define _UDI2_SECTION_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif
 

/**@brief �⸴����ش������ֵ*/
enum 
{
	CSUDISECTION_ERROR_BAD_PARAMETER = CSUDI_SECTION_ERROR_BASE,      ///< ��������
	CSUDISECTION_ERROR_FEATURE_NOT_SUPPORTED,	///< ������֧��
	CSUDISECTION_ERROR_UNKNOWN_ERROR,			///< һ�����
	CSUDISECTION_ERROR_NO_MEMORY,				///< �޿����ڴ�
	CSUDISECTION_ERROR_INVALID_HANDLE,			///< �Ƿ����
	CSUDISECTION_ERROR_INVALID_STATUS,  		///< ��Ч״̬��������û��stopʱ����modify
	CSUDISECTION_ERROR_NO_FREE_FILTER			///< û�п��õ�filter
};


/**@brief filter���ȣ�������section�е�length�ֶ�*/
#define CSUDISECTION_FILTER_LEN 16

/**@brief Section�ص���������ö��*/
typedef enum
{
	EM_UDI_REQ_SECTION_DATA,		///< ��ʾһ��PSI��SI���ݰ�
	EM_UDI_REQ_PES_DATA,			///< ��ʾһ��PES���ݰ�(������Ƶ��Ҫ����������)
	EM_UDI_REQ_RAW_DATA			///< ��ʾһ��RAW���ݰ�
}CSUDISECTIONRequestType_E;


/**@brief Section�ص����ݽṹ*/
typedef struct 
{
	unsigned char* m_pucData;		///< Section����ָ��
	int            	  m_nDataLen;			///< Section���ݳ���
}CSUDISECTIONCallbackData_S;

/**@brief Filter��Ϣ���ݽṹ

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
@note ��i��0��CSUDISECTION_FILTER_LEN �� 1�����й�������������ʱ������filter�Ĺ�������������section�Żᱻ����
@note ��Ϊһ����˵�����ǲ����section length�ֶ������˴����������ǵĽӿ�Ҫ�󲻹���section�ĵ�2��3�����ֽ����ݡ������¶�Ӧ��ϵ��
- Data[0]    ����   MatchMask[0] 
- Data[1]    ����   ��
- Data[2]    ����   ��
- Data[3]    ����   MatchMask[1] 
- Data[4]    ����   MatchMask[2]
- Data[5]    ����   MatchMask[3]
- ����
@note �������������ֽڣ�ʵ��Filter��ȿ�ΪCSUDISECTION_FILTER_LEN+1�ֽڣ���ʵ��ֻ�ܱ�֤ǰ16�ֽڵĹ��ˣ�����CSUDISECTION_FILTER_LEN-2��CSUDISECTION_FILTER_LEN-1�ֽڿ�����Ч

@note Filterֻ��EM_UDI_REQ_SECTION_DATA��Ч������������Ч
*/
typedef struct 
{
    unsigned char match[CSUDISECTION_FILTER_LEN];	    ///< ƥ���ֶ�
    unsigned char mask[CSUDISECTION_FILTER_LEN];	    ///< ��ע�ֶ�
    unsigned char negate[CSUDISECTION_FILTER_LEN];	    ///< ȡ���ֶ�
}CSUDIFILTERMatchMask_S;


/**@brief Filter��Ϣ������Ϣ������Ҫ��������ݵ�����*/
typedef struct 
{	   
    CSUDISECTIONRequestType_E		m_eReqType;	    ///< ָ���������ͣ���μ�CSUDISECTIONRequestType_E
    int 							m_nDemuxID;	    ///<ָ��Demux Index
    int                          	             m_nPID;		    ///< ָ���������ݵ�pid
    CSUDI_BOOL                               m_bCRCCheck;	    ///< �Ƿ����CRC��飬ע�⣺���ڴ���������(PSI/SI/˽������)�����ֵ�����ر�׼�ĵ�������ȷû��CRC�ֶε������Ҫ���м�����ø���ΪCSUDI_FALSE������ʵ����Ҳ�����ע����ΪCSUDI_TRUE����µ�ʵ�ֻ��ƣ�
    CSUDIFILTERMatchMask_S     		 m_sMMFilter;	    ///< Filter ��Ϣ
} CSUDISECTIONRequestInfo_S;



/**
@brief Demux����ص�����

����������ݵ���ʱ�������ص�
@param[in] eType �ص����ͣ�������μ�CSUDISECTIONRequestType_E
@param[in] pvParam �ص����ݣ�Ŀǰ�����еĻص����;�ΪΪָ��һ��CSUDISECTIONCallbackData_S���͵�ָ��
@param[in] pvUserData �û�����
*/
typedef void ( *CSUDISECTIONCallback_F ) ( CSUDISECTIONRequestType_E eType, void * pvParam ,void * pvUserData);


/**
@brief ����Filter��������

����������PSI��SI��Ҳ�����ڹ���PES��RAW������,��Ҫ��CSUDIFILTERStart���ʹ��
@param[in] psRequestInfo Filter������������μ�CSUDISECTIONRequestInfo_S
@param[in] fnSectionCallback �ص����������з������������ݵ���ʱ�����ñ�����֪ͨ�����лص�������ͬһ�߳��ҽ��鲻Ҫ�ڵ����߳���ִ��̫�������������μ�CSUDISECTIONCallback_F
@param[in] pvUserData �û����ݣ�����ΪCSUDI_NULL
@param[out] phFilterHandle	���ڽ��շ��ص�Filter���������ֵΪ0��Ϊ�Ƿ�
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note 
- ��ϸ�Ĺ��˹�����ο�CSUDISECTIONRequestInfo_S�ṹ�Ķ���
- ��Ӧ���豸�����CSUDISECTIONRequestInfo_S�ṹ��
- CSUDIFILTERAlloc�ɹ��󣬵���CSUDIFILTERStart��ʼ��������
- CSUDIFILTERAlloc���Զ�ε��ã�ֱ��Filterͨ����ȫ��������Ϊֹ
*/
CSUDI_Error_Code CSUDIFILTERAllocate(const CSUDISECTIONRequestInfo_S * psRequestInfo, CSUDISECTIONCallback_F fnSectionCallback, const void * pvUserData ,CSUDI_HANDLE * phFilterHandle);

/**
@brief ����Filter��������

@param[in] hFilterHandle ��CSUDIFILTERAllocate����õ���filterͨ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note
- Start�ɹ��󣬼���ʼ���ա�������Ӧ����
- ��һ�ε��ú��ٴ�(���)���ý�����ʧ��
*/    
CSUDI_Error_Code CSUDIFILTERStart(CSUDI_HANDLE hFilterHandle);

/**
@brief ֹͣFilter��������,�������ͷ�Filter��Դ

��ֹͣ�����ͷ���Դ
@param[in] hFilterHandle ��CSUDIFILTERAllocate����õ���filterͨ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note
- Stop�ɹ��󣬼�ֹͣ���ա�������Ӧ����
- ��ͼȥֹͣһ��δ������Filter����CSUDISECTION_ERROR_INVALID_STATUS
- ��һ�ε��ú��ٴ�(���)���ý�����ʧ��
*/
CSUDI_Error_Code CSUDIFILTERStop(CSUDI_HANDLE hFilterHandle );

/**
@brief ֹͣFilter��������,���ͷ�Filter��Դ

@param[in] hFilterHandle ��CSUDIFILTERAllocate����õ���filterͨ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note
- Free�ɹ��󣬼�ֹͣ���ա�������Ӧ����
- ��һ�ε��ú��ٴ�(���)���ý�����ʧ��
*/
CSUDI_Error_Code CSUDIFILTERFree(CSUDI_HANDLE hFilterHandle);

/**
@brief �޸�Fillter����

@param[in] hFilterHandle ��CSUDIFILTERAllocate����õ���filterͨ�����
@param[in] psMMFilter Filter ������� CSUDIFILTERMatchMask_S����,��ΪCSUDI_NULL��ʾ�����Ĵ���
@param[in] bCRCCheck �Ƿ���CRC
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note
- ������Stop״̬�²����޸�filter���������򷵻�CSUDISECTION_ERROR_INVALID_STATUS
- ��һ�ε��ú��ٴ�(���)���ý�����ʧ��
*/
CSUDI_Error_Code CSUDIFILTERModify(CSUDI_HANDLE hFilterHandle, const CSUDIFILTERMatchMask_S * psMMFilter,  CSUDI_BOOL bCRCCheck);

/**
@brief ����Fillter�ĵײ�buffer��С

��Ҫ������Ҫ���˴�������,�ϲ���ܻ�������ȡ�ߵ����
@param[in] hFilterHandle ��CSUDIFILTERAlloc����Ĺ��������
@param[in] nBufSize Ҫ���õĵײ�buffer�Ĵ�С������Ϊ2��n�η�����С1K�����ֵ���ݸ���ƽ̨���첻ͬ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ����Fillter�ĵײ�buffer��Сʱ������Fillter����stop״̬�����򷵻�CSUDISECTION_ERROR_INVALID_STATUS	
*/
CSUDI_Error_Code CSUDIFILTERSetBufSize(CSUDI_HANDLE hFilterHandle,  int nBufSize);


#ifdef  __cplusplus
}
#endif

/** @} */

#endif


