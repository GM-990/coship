/**@defgroup RECORD  RECORD ������PVR ������صĽӿ�
@brief ��ģ����Ҫ������PVR¼����صĽӿ�

¼�Ʒ�Ϊ����:��Ŀ¼�ơ�ʱ��¼�ơ�ts��¼��

- ��Ŀ¼��:
¼��һ·��Ŀ������Ƶ��Ŀ������ָ���ļ���
ֻ����¼��һ·��Ŀ������Ƶ��pcr���ݣ������������Ŀ��pid����ͬʱ¼�ơ�
��Ŀ¼�Ƶ��ļ��ط�ʱ���Խ���trickmode����

- ʱ��¼��:
ʱ��¼�������ڽ�Ŀ¼�Ƶ�һ��������ʱ��¼�Ƶ�¼���ļ���ѭ���ļ�
ѭ���ļ�:��С�̶����ļ�����ʱ����ռ���˹̶���С�Ĵ��̿ռ䣬��д�ļ���С�����̶�ʱ���Զ��ƻشӿ�ͷд����ͷ�����ݻᱻ�Զ�����
��ʱ��¼���ƻغ�¼��ʱ����Ϣ�еĿ�ʼʱ��Ϊ�ƻغ󸲸ǵĳ��ȡ�
ʱ��¼�Ƶ�ֹͣ���Ե���CSUDIRECStop���ߵ���CSUDIRECStopTshift
����CSUDIRECStopTshiftʱ���Խ�ʱ��¼�Ƶ�ѭ���ļ�ת������ͨ�ļ���������ͨ���ļ��طš�

-ts��¼��
¼��һ·ts��������¼�ƶ�·��Ŀ�����ݣ�����¼��si���
ts��¼��ʱ������������Ӧ�������ļ����ط�ts��ʱ��һ�㲻֧��trickmode������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

 @version 2.1.1 2010/11/1 ����¼�Ʋ������Ӽ������ͣ�׷��¼�ƣ�ʱ���ļ���С������tshiftת��ͨ�ļ���pvr¼���ļ��и�ӿ�  
 @version 2.0.1 2009/8/10  
 @{
*/
#ifndef _UDI2_RECORD_H_
#define _UDI2_RECORD_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C"
{
#endif 

#define CSUDI_REC_MAX_PID_COUNT 32

/**@brief ¼����ش������ֵ*/
enum
{
	CSUDIREC_ERROR_BAD_PARAMETER = CSUDI_RECORD_ERROR_BASE,			///< ��������
	CSUDIREC_ERROR_FEATURE_NOT_SUPPORTED,							///< ������֧��
	CSUDIREC_ERROR_UNKNOWN_ERROR,									///< һ�����
	CSUDIREC_ERROR_NO_MEMORY,										///< �޿����ڴ�
	CSUDIREC_ERROR_INVALID_DEVICE_ID,								///< �Ƿ����豸IDֵ
	CSUDIREC_ERROR_INVALID_HANDLE,									///< �Ƿ��ľ��
	CSUDIREC_ERROR_RECORD_FULL,										///< ���������¼�Ƹ���
	CSUDIREC_ERROR_ALREADY_STARTED,									///< ��¼���Ѿ�����
	CSUDIREC_ERROR_ALREADY_STOPPED,									///< ��¼���Ѿ�ֹͣ
	CSUDIREC_ERROR_NOT_STARTED,										///< ��¼�ƻ�û������
	CSUDIREC_ERROR_INVALID_PIDS,									///< �Ƿ�PIDֵ,������PIDֵ
	CSUDIREC_ERROR_TIME_FAILED,										///< ��ȡ¼��ʱ��ʧ��
	CSUDIREC_ERROR_NO_CALLBACK,								 	///< û����Ӧ�Ļص���������ɾ��
	CSUDIREC_ERROR_ALREADY_ADDED								///< �ûص��Ѿ���ӹ�
};

/**@brief ¼������*/
typedef enum
{
	EM_UDIRECORD_TYPE_CHNL = 0,           ///<    ¼��һ·��Ŀ��������ļ���ʽΪ�����ļ�
	EM_UDIRECORD_TYPE_TSHIFT,              ///<    ʱ��¼��,������ļ���ʽΪѭ���ļ����ط�ʱ���Խ���trickmode����
	EM_UDIRECORD_TYPE_TS                      ///<    ts��¼�ƣ�ts���а�����·��Ŀ��
}CSUDIRecType_E;

/**@brief ¼�ƻص���Ϣ*/
typedef enum
{
	EM_UDIRECORD_EVT_DISKFULL,      ///<   �洢�豸û�пռ���Ϣ
	EM_UDIRECORD_EVT_UNKNOWN,       ///<   δ֪��Ϣ����
	EM_UDIRECORD_EVT_WRITEFAIL,     ///<   ¼��д�ļ�ʧ��
	EM_UDIRECORD_EVT_MAX_NUM        ///<   ��Ч�߽�ֵ
}CSUDIRecEvent_E;

/**@brief ¼��ͨ���豸��Դ���ݽṹ����*/
typedef struct
{
	int m_nDemux;		///< Demux�豸��Index��-1��������Demux�豸
}CSUDIRecChnl_S;

/**@brief ¼��ʱ����Ϣ*/
typedef struct {
	int       m_nStartTime;   		///<  ¼�ƿ�ʼ�ľ���ʱ�䣬���ڷ�ʱ��¼����ʱ����ʱ��ʼ��Ϊ0
	int       m_nValidDuration;   	///<  ¼����Чʱ�䣬��¼�ƿɲ��ŵ�ʱ��
} CSUDIRecTimeInfo_S;

/**@brief �����������ͣ�һ������PVR*/
typedef enum
{
	EM_UDI_ENCRYPT_TYPE_FREE=0,		///< ������������
	EM_UDI_ENCRYPT_TYPE_CA,			///< ca������
	EM_UDI_ENCRYPT_TYPE_REENCRYPT	///< ���μ�����(��ca���Ƴ���������ƽ̨���μ���)
}CSUDIStreamEncryptType_E;

/**@brief ¼��stream�������ݽṹ����*/
typedef struct{
	CSUDIStreamEncryptType_E m_eEncryptType;				///<���ܷ�ʽ�����CSUDIRecEncryptType_E˵��
	CSUDI_UINT8 m_ucEncryptKey[CSUDI_MAX_SECURITYKEY_LEN]; 	///<���ܽ���Key,�����¼�ƾ��Ǽ���Key;����ǲ��ž��ǽ���Key
	int m_nEncryptKeyLength; 								///<��Կ����
} CSUDIStreamEncryptParam_S;

/**@brief ����¼����Ϣ*/
typedef struct {
	CSUDIRecType_E		m_eRecType;								///< ��·¼�Ƶ����ͣ����CSUDIRecType_E˵��
	char                m_szFileName[CSUDI_MAX_FILE_NAME_LEN];  ///< ����¼�ƽ�����ļ�����ֻ֧��ASCII�����ʽ
	int					m_nCount;								///< ¼�Ƶ�pid����
	CSUDIStreamInfo_S	m_psPids[CSUDI_REC_MAX_PID_COUNT];    	///< ��Ҫ¼�ƵĽ�Ŀpids ���CSUDIStreamInfo_S˵��
	CSUDI_BOOL			m_bAppend;								///׷��¼�ƣ�m_bAppend=falseʱ�����¼��Ŀ���ļ�����ʱ�����Ƚ�Ŀ���ļ�������ٴ�ͷд�ļ�
																///<m_bAppend=trueʱ�����¼��Ŀ���ļ�����ʱ����ɾ�������ļ����������ļ�ĩβ׷��д
	unsigned int		m_nMaxSize;								///< ¼����󳤶�(��λΪMB)������m_eRecType==EM_UDIRECORD_TYPE_TSHIFTʱ��Ч
    CSUDIStreamEncryptParam_S m_stEncryptParam; 				///<�μ�CSUDIStreamEncryptParam_S
} CSUDIRecStartParam_S;

/**@brief ¼�ƻص���������

@param[in] hRecHandle  ��·¼�ƾ��
@param[in] eEvent ¼����Ϣ���ͣ����CSUDIRecEvent_E
@param[in] pvUserData �û�����
@return  ���뷵��0
@note ��hHandle��pvUserDataΨһ����ÿ���ص�����
*/
typedef int (*CSUDIRECCallback_F)(CSUDI_HANDLE hRecHandle,const CSUDIRecEvent_E eEvent, void* pvEventData,void * pvUserData);

/**
@brief ע��¼�ƻص�����

@param[in] hRecHandle ¼�ƾ������CSUDIRECOpen()���
@param[in] fnRecordCallback ¼�ƻص����������CSUDIRECCallback_F˵��
@param[in] pvUserData  �û����ݣ������ݻ�ԭ�ⲻ�����ظ��û�
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ����֧��ע��Ļص���������Ϊ32���������
@note �ص�����ָ��+�ص���������+�û�����Ψһ��ʶһ���ص�,������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIREC_ERROR_ALREADY_ADDED
@note ��ע��ĺ����Ѿ�ע���������CSUDIREC_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECAddCallback(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback, void * pvUserData);

/**
@brief ɾ��¼�ƻص�����

@param[in] hRecHandle ¼�ƾ������CSUDIRECOpen()���
@param[in] fnRecordCallback ¼�ƻص����������CSUDIRECCallback_F˵��
@param[in] pvUserData  �û����ݣ������ݻ�ԭ�ⲻ�����ظ��û�
@return �ɹ�����CSUDI_SUCCESS������ûص�����������û��ע�ᣬ������CSUDIREC_ERROR_NO_CALLBACK
@note �����ڻص�������ɾ���ص�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECDelCallback(CSUDI_HANDLE hRecHandle,CSUDIRECCallback_F fnRecordCallback,void * pvUserData);

/**
@brief ����һ��¼����

@param[in] psChnl   ¼��ͨ�������ݽṹ��
@param[out] phRecHandle ��¼�Ƶľ����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECOpen(const CSUDIRecChnl_S * psChnl, CSUDI_HANDLE * phRecHandle);

/**
@brief ��ȡ¼��֧�ֵļ�������

@param[in] hRecHandle ¼�ƾ������CSUDIRECOpen()���
@param[out] peOutTypeArray   ��·¼��֧�ֵļ��ܷ�ʽ(����֧�ֶ���)�����CSUDIRecEncryptType_E����
@param[in]  nArraySize ����peOutTypeArray����ĳ���
@param[out]  pnActualCnt ��·¼��֧�ֵļ��ܷ�ʽ����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECGetSupportEncryptTypes(CSUDI_HANDLE hRecHandle,CSUDIStreamEncryptType_E* peOutTypeArray,int nArraySize,int* pnActualCnt);

/**
@brief ����һ��¼����

@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note ����һ��¼��������CSUDIRECOpen�ɶ�ʹ�á�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECClose(CSUDI_HANDLE hRecHandle);

/**
@brief ����¼��

@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@param[in] psStartParam  ����¼�Ʋ������CSUDIRecStartParam_S˵��
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note �ɲ���psStartParamָ����Ҫ¼�Ƶ������Լ�¼�ƵĽ�Ŀ
@note ���ô˽ӿ�֮ǰ����Ҫ����CSUDIDEMUXConnectTuner����hRecHandle��Ӧ��demux��tuner�豸����ʹ�ö�Ӧ��tuner�Ѿ���Ƶ�ɹ���
@note �����¼�����õ�demux�Ѿ�����tuner�����ʱ����Ҫ�ٴε���CSUDIDEMUXConnectTuner����demux��tuner������
@see CSUDIDEMUXConnectTuner

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStart(CSUDI_HANDLE  hRecHandle,const CSUDIRecStartParam_S * psStartParam);

/**
@brief ֹͣ¼��

@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStop(CSUDI_HANDLE  hRecHandle);

/**
@brief ��ͣ¼��
@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECPause(CSUDI_HANDLE  hRecHandle);

/**
@brief �ָ�¼��
@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note �ָ�¼�ƺ���¼�Ƶ�����׷�ӵ���ͣǰ��¼���ļ���

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECResume(CSUDI_HANDLE  hRecHandle);


/**
@brief ֹͣʱ��¼��

@param[in] hRecHandle ����ʱ��¼�Ƶ�¼����������ɺ���CSUDIRECOpen()���
@param[in] bSwitchToFile �Ƿ�ʱ��¼�Ƶ�ѭ���ļ�ת������ͨ�ļ�
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECStopTshift(CSUDI_HANDLE  hRecHandle,CSUDI_BOOL bSwitchToFile);

/**
@brief ��ȡ¼��ʱ����Ϣ

@param[in] hRecHandle ¼����������ɺ���CSUDIRECOpen()���
@param[out] psTimeInfo ¼�Ƶ�ʱ����Ϣ�����CSUDIRecTimeInfo_S����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECGetRecTime(CSUDI_HANDLE  hRecHandle, CSUDIRecTimeInfo_S * psTimeInfo);

/**
@brief ¼���ļ��и�

@param[in] szFileName ¼�����ɵ��ļ�
@param[in] unStartPos ���п�ʼλ��(��λΪs)
@param[in] unEndPos ���еĽ���λ��(��λΪs)
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note �и������Ҫ��������ʱ��λ��nstartPos��nEndPosת�����ļ��е�����λ�ã������ö�����ɾ�������и����ļ��ɲ��ų��ȱ��и�ǰ������(nEndPos-nStartPos)

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIRECFileCut(const char* szFileName,unsigned int unStartPos,unsigned int unEndPos);

#ifdef __cplusplus
}
#endif
/**@}*/
#endif
