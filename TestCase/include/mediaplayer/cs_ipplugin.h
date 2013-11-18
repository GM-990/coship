/******************************************************************************
FileName:  cs_ipplugin.h
Copyright (c) 2009�� Shen Zhen Coship Electronics CO.,LTD.

Version:       Ver 1.0
Author:        
File Description:
        1�����ļ��涨�˲���ı�׼�ӿںͲ������Ľӿ�
        2���������ʵ�ִ��������١������ݡ�������ѯ����ǰý���ѯ���������������������Բ��ṩ
        ���ĳ���������ṩӦ���ܹ���������ѯ�����л�֪
Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
*******************************************************************************/
/*
��ͻ����

�ص�����FOnCSIPPluginCallback
����ֵCSIPPluginRet_E
��������ͳһ����
ý������
�ص���Ϣ����
*/

#ifndef _CS_IPPLUGIN_H_
#define _CS_IPPLUGIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "cs_player_message.h"

/**
@brief  macro
*/
#define  CS_IPPLUGIN_NAME_LEN    64         
#define  CS_IPPLUGIN_DESCRIPTION_LEN   256
#define  CS_IPPLUGIN_PROTOCOL_NAME_LEN   64

/**
@brief  ���INVOKE������������ö��
*/
typedef enum _CSIPPlugin_Invoke_CommandType_E
{
	EM_CSIPPLUG_INVOKE_COMMAND_SAVE_BOOKMARK,
	EM_CSIPPLUG_INVOKE_COMMAND_GET_STARTTIME,
	EM_CSIPPLUG_INVOKE_COMMAND_DISABLE_BREAKPOINT_TOSEE
}CSIPPlugin_Invoke_CommandType_E;

/**
@brief  �������ö��
*/
typedef  enum  _CSIPPluginCapability_E
{
	EM_CSIPPLUG_CAPABILITY_PAUSE,
	EM_CSIPPLUG_CAPABILITY_RESUME,
	EM_CSIPPLUG_CAPABILITY_SETRATE,
	EM_CSIPPLUG_CAPABILITY_GETRATE,
	EM_CSIPPLUG_CAPABILITY_SETTIME,
	EM_CSIPPLUG_CAPABILITY_GETTIME,
	EM_CSIPPLUG_CAPABILITY_GETDURATION,
	EM_CSIPPLUG_CAPABILITY_GETSENDDECODERELEN,  
	EM_CSIPPLUG_CAPABILITY_INVOKE,
	EM_CSIPPLUG_CAPABILITY_SETDURATION,
	EM_CSIPPLUG_CAPABILITY_SETDMXCHECKBUFFERINGLEN,
	EM_CSIPPLUG_CAPABILITY_GETDMXBUFFERINGPERCENT,
	EM_CSIPPLUG_CAPABILITY_GETSTARTCLOCK
} CSIPPluginCapability_E;

/**
@brief  ý������
*/
typedef enum  _CSIPPluginMediaType_E
{
    EM_PUMP_SOURCE_TYPE_TS = 0,
	EM_PUMP_SOURCE_TYPE_ES,  	  
	EM_PUMP_SOURCE_TYPE_PCM,
	EM_PUMP_SOURCE_TYPE_MP3,
	EM_PUMP_SOURCE_TYPE_DIR, //yzh, test
	EM_PUMP_SOURCE_TYPE_UNKNOWN
}CSIPPluginMediaType_E;

/**
@brief  ��������
*/
typedef enum _CSEncodeType_E
{    
	EM_ENCODE_H264,
    EM_ENCODE_MPEG1,
    EM_ENCODE_MPEG2,
    EM_ENCODE_UNKNOWN
}CSEncodeType_E;

/**
@brief  TS��Ϣ
*/
typedef struct  _CSPSIInfo_S
{
    int nAudPid;
    int nAudStreamType;  //���Ͳο�udi
    int nVidPid;
    int nVidStreamType;   //���Ͳο�udi
    int nPcrPid;
    int nProgramId;
}CSPSIInfo_S;

/**
@brief  ����ֵ����
*/
typedef enum  _CSIPPluginRet_E
{
    EM_PUMP_SUCCESS = 0, 
    EM_PUMP_FAILURE,
    EM_PUMP_TIMEOUT,
    EM_PUMP_INVALPARA,
    EM_PUMP_NOMEM,
    EM_PUMP_URL_ERROR, 
    EM_PUMP_NOT_SUPPORT,
    EM_PUMP_SOCKET_ERROR ,
    EM_PUMP_RESOURCES_FAILURE,
    EM_PUMP_DEMUX_ERROR,
    EM_PUMP_END_OF_STREAM
} CSIPPluginRet_E;
/**
@brief  ����ֵ����
*/
typedef enum  _CSIPPluginNameType_E
{
    EM_PUMP_Plugin_NOT_SET = 0, 
    EM_PUMP_Plugin_CITV,
    EM_PUMP_Plugin_NGOD,
    EM_PUMP_Plugin_SEARCH_VOD,
} CSIPPluginNameType_E;


/**
@brief �ص���������
@param[in] nUserData �û�����
@param[in] nMsg ��Ϣ����,����μ�ö������CSIPPUMPMsg_E(cs_player_message.h)
@param[in] nPara1 ��Ϣ����1
@param[in] nPara2 ��Ϣ����2
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ����˵��:
		1.������Ϣ����Ϊ����յ��ķ�����������֪ͨ��Ҳ��Ҫͨ���ص���֮������:
			EM_CSIPPUMP_MSG_SERVER_START_STREAM:���˵�ͷ������Ҫ����
			EM_CSIPPUMP_MSG_SERVER_END_STREAM :�����β������Ҫ����
			EM_CSIPPUMP_MSG_SERVER_OPTIONS:������Ҫ����
			EM_CSIPPUMP_MSG_SERVER_REDIRECT:��Ҫ�ض��򣬲���Ҫ����
		2.������������������У��κ����ڷ�����ԭ�������ʧ�ܣ���Ӧͨ���ص���֮������:
			EM_CSIPPUMP_MSG_SERVER_ERROR���Ҳ���1��ʾ����ԭ�򣬲���2Ԥ��
		3.�еķ�������Ҫ������ж�ʱ����������Ϣ��������Ϣ�Ĵ���һ��Ϊ����ڲ���������Ҫ�ⲿ֪��	
*/
typedef int (*FOnCSIPPluginCallback) (int  nUserData,int  nMsg,int nPara1,int nPara2);

/**
@brief   �������
@param[in]  pszURL  ͳһ��Դ��λ����������Э������ͺ���Դ��ַ
@param[in]  dwTimeout ���糬ʱʱ�䣬��λΪms
@param[in]  fnCallback ��Ҫע��Ļص�����
@param[in]  nUserData  �ص�ʱ�ش����û�����
@param[out] phIPClient ������
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note ����������ʵ�֣���Ϊͬ����������������������������Ӳ����󲥷ŵĹ���
	  �˺������ý����Ժ�Ӧ�ÿ���������������Ƶ
@note ����URL��ʽ����
     "dvb://490000000:6875:64:8"
     "igmp://224.0.0.10:1234"
     "file://image.mp3"
     "http://video/haha.ts"
     "rtsp://192.168.101.9:554/Video/h264/v.mpg"
@note �˺�����Ҫ�ص�����Ϣ:
	1)���ʹ��IPQAM��ʽ(��Ҫ��Ƶ)�㲥���ڻ�ȡ���㲥��Ŀ��Ƶ����Ϣ��
	�ص�EM_CSIPPUMP_MSG_CATVINFO_TOMP��Ϣ������1Ϊ��Ŀ��serviceid������2ΪCSUDITunerCableDeliver_S���͵Ľṹ��ָ�룬���н�ĿƵ����Ϣ
	2)���ʹ��IPTV��ʽ,����Ҫ����EM_CSIPPUMP_MSG_CATVINFO_TOMP��Ϣ
	3)����㲥���������������У����ڷ���������Ĵ����ʧ��
	�ص�EM_CSIPPUMP_MSG_SERVER_ERROR������1Ϊ�������������ֵ������2ΪԤ��
*/
typedef  int(*FonIPPluginCreate)(const char *pszURL,  DWORD  dwTimeout,  FOnCSIPPluginCallback  fnCallback, int  nUserData, HCSHANDLE  *phIPClient);

/**
@brief   ���ٲ��
@param[in] hIPClient ������
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note  ����������ʵ�֣���Ϊͬ������������֪ͨ�������ն��˳����Լ���Դ�ͷ�
*/
typedef  int(*FonIPPluginDestroy) (HCSHANDLE hIPClient);

/**
@brief   �ж�����֧��
@param[in] hIPClient ������
@param[out] eCapability  Ҫ�жϵ�����������μ�ö������CSIPPluginCapability_E
@return  ֧�ַ���TRUE����֧�ַ���FALSE
@note  ͨ��ʹ��ĳ������֮ǰ��Ҫ�ж��Ƿ�֧�֣�����ĳ�������˵��ĳ�������Ƿ�
         ֧�ֲ���һ�ɲ���ģ��п��ܸ������ǰ��״̬�й�
@note  ������ÿ�����������ʵ�֣��Թ���ѯĳ�������Ƿ�֧��
@note  ���ĳ��������֧�֣����Բ�ʵ�ֶ�Ӧ�ķ����������ڱ������п��Բ�ѯ��
@note  ����������ʵ��
*/
typedef  BOOL(*FonIPPluginCheckCapability)( HCSHANDLE hIPClient , CSIPPluginCapability_E eCapability);

/**
@brief  ��ȡ��ǰý������
@param[in] hIPClient ������
@param[out] peMediaType ý������
@param[out] pParam  ���ز������ڴ����ⲿ���䣬��������Ŀ��Դ��գ�����֧�ִ���
		   ��peMediaType = EM_PUMP_SOURCE_TYPE_TSʱpParam��ʾCSPSIInfo_S��Ϣ
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ����������ʵ��
*/ 
typedef  int(*FonIPPluginGetCurMediaType)( HCSHANDLE hIPClient, CSIPPluginMediaType_E *peMediaType, void* pParam);

/**
@brief   ��ȡ����
@param[in] hIPClient ������
@param[out] pcBuf ���������ݣ��ڴ����ⲿ����
@param[in/out] pdwLen �����������������������ʱ��ʾpcBuf���ڴ�����С,�����ʱ��ʾʵ�ֶ���������
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ����������ʵ�֣�����ֻ֧��IPQAM��ʽ�㲥�Ĳ��������ֱ�ӷ��ء������IPTV��ʽ������Ҫʵ�ִ�����������ݵĹ���
@note    ��ȡ�ɹ�ʱ��ʵ�ʶ��������ݿ��Ա�����С��
		 �˺�����pcBuf�е������Ǵ�TS���ݣ�������յ��������ض���װ����Ҫȥ����װ��
		 �������TCP��ʽ����Ҫע��������ݵ������ԣ���Ҫ����ý��ճ�ʱ����������С��ָ�����ȵ������
		 �˺������յ����ݽ���datadriver�Ժ�datadriver�ۻ����ݵ�һ�������Ժ󣬾ͻ�ע��demux���в��š�
*/ 
typedef  int(*FonIPPluginReadStream)(HCSHANDLE  hIPClient, char *pcBuf, DWORD *pdwLen);

/**
@brief   ��ͣ
@param[in] hIPClient ������
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ������ͬ�������첽����������ʵ���б���ͨ���ص�������ִ�н����֮������.
		 �����ͬ����ʽ�����ڴ˺����н���Ӧ��������첽��ʽ����Ҫÿ���̶�ʱ��ȥ����Ӧ��
		 �õ��������ɹ�Ӧ���: �ڻص������з���EM_CSIPPUMP_MSG_PAUSE_SUCC��Ϣ
         ����Ӧ��ʧ�ܺ��ڻص������з���EM_CSIPPUMP_MSG_SERVER_ERROR��Ϣ���Ҳ���1��ʾ����ԭ�򣬲���2Ԥ��

@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginPause) (HCSHANDLE hIPClient);

/**
@brief   �ָ�
@param[in] hIPClient ������
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ������ͬ�������첽����������ʵ���б���ͨ���ص�������ִ�н����֮������
		 �����ͬ����ʽ�����ڴ˺����н���Ӧ��������첽��ʽ����Ҫÿ���̶�ʱ��ȥ����Ӧ��
         ִ�гɹ��ڻص������з���EM_CSIPPUMP_MSG_RESUME_SUCC��Ϣ
         ִ��ʧ���ڻص������з���EM_CSIPPUMP_MSG_SERVER_ERROR��Ϣ���Ҳ���1��ʾ����ԭ�򣬲���2Ԥ��
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginResume) (HCSHANDLE hIPClient);

/**
@brief   ��������
@param[in] hIPClient ������
@param[in] nRate  Ҫ���õĲ����ٶȡ�Ϊ��֧�������Ҳ�ʹ�ø������������������׼����
         �������µĹ�ʽ: nRate = RealRate * 1000
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ������ͬ�������첽����������ʵ���б���ͨ���ص�������ִ�н����֮������
		 �����ͬ����ʽ�����ڴ˺����н���Ӧ��������첽��ʽ����Ҫÿ���̶�ʱ��ȥ����Ӧ��
         ִ�гɹ��ڻص������з���EM_CSIPPUMP_MSG_SETRATE_SUCC��Ϣ���Ҳ���1��ʾ��������Ӧ��ʵ�ʱ��ʣ���1000��2000��
         ִ��ʧ���ڻص������з���EM_CSIPPUMP_MSG_SERVER_ERROR��Ϣ���Ҳ���1��ʾ����ԭ�򣬲���2Ԥ��
@note    ��������1000ʱΪ1�����������ţ�2000Ϊ�����ٿ����4000Ϊ�ı��ٿ����Ϊ-2000��ʾ�����ٿ��ˡ�
         ���С��0˵��Ϊ���ˡ�
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginSetRate) (HCSHANDLE hIPClient, int nRate);

/**
@brief   �������
@param[in] hIPClient ������
@param[out] pnRate  ��õĲ����ٶȡ�Ϊ�������ñ���һ�£�ʵ���ٶ�Ҫ����1000
         �������µĹ�ʽ: RealRate = nRate��1000(����������)
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    �õ�����1000ʱΪ1�����������ţ�2000Ϊ�����ٿ����4000Ϊ�ı��ٿ����Ϊ-2000��ʾ�����ٿ��ˡ�
         ���С��0˵��Ϊ���ˡ�
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginGetRate) (HCSHANDLE hIPClient, int *pnRate);

/**
@brief   ���ò��ſ�ʼʱ��
@param[in] hIPClient ������
@param[in] nTime  ����ڽ�Ŀ��ʼʱ��Ĳ���ʱ��㣬��λΪ�롣����NPTʱ�䣬��Ŀ��ʼʱ��Ϊ0s
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ������ͬ�������첽����������ʵ���б���ͨ���ص�������ִ�н����֮������
		 �����ͬ����ʽ�����ڴ˺����н���Ӧ��������첽��ʽ����Ҫÿ���̶�ʱ��ȥ����Ӧ��
         ִ�гɹ��ڻص������з���EM_CSIPPUMP_MSG_SETTIME_SUCC��Ϣ
         ִ��ʧ���ڻص������з���EM_CSIPPUMP_MSG_SERVER_ERROR��Ϣ���Ҳ���1��ʾ����ԭ�򣬲���2Ԥ��
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginSetTime) (HCSHANDLE hIPClient, int nTime);

/**
@brief   ��ȡ��ǰ����ʱ��
@param[in] hIPClient ������
@param[out] pnTime  ��ǰý�岥��ʱ�䣬����ڽ�Ŀ��ʼ��ʱ�䣬��λΪ��
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
@note    ͨ���������������Լ���Ĳ�����ȡ��
		����һ���㲥��Ŀ����ʼʱ��Ϊ0��һ���ٲ�����10s��Ȼ�������ٿ����5s��
		��˽ӿ�Ӧ�÷��ص�*pnTime = 10 + 2*5 = 20s
*/
typedef  int(*FonIPPluginGetTime) (HCSHANDLE hIPClient, int *pnTime);

/**
@brief   ��ȡ��Ŀʱ����
@param[in] hIPClient ������
@param[out] pdwTime  ��ǰý����ʱ�䣬��λΪ��
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
		һ������£�����ʽ����Ե㲥�;��ʻط�ʵ�֣�ʱ���޷���ȡ����ʱ��
*/
typedef  int(*FonIPPluginGetDuration) (HCSHANDLE hIPClient, DWORD *pdwTime);

/**
@brief   ����ʱ�䳤��
@param[in] hIPClient ������
@param[in] dwTime  ��ǰý��ʱ�䣬��λΪ��
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginSetDuration) (HCSHANDLE hIPClient, DWORD dwTime);

/**
@brief   ����DMX�����С
@param[in] hIPClient ������
@param[in] dwSize  ����DMX �������Ĵ�С
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginSetDMXBufferingCheckMaxSize) (HCSHANDLE hIPClient, DWORD dwSize);

/**
@brief   ����DMX������
@param[in] hIPClient ������
@param[out] pnPercent  ���������DMX����������������ռ�ٷֱ�
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginGetDMXBufferingPercent) (HCSHANDLE hIPClient, int * pnPercent);

/**
@brief   ��ȡһ��д��decoder��demux�е�������ݳ���
@param[in] hIPClient ������
@param[out] pdwLen  һ��ע�����ѳ���
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ÿ��ý��һ��ע�����Ѵ�С���ܲ�һ�������֪��ý��ĸ�ʽ��Ϣ���Ӷ������ṩ���ⲿ
@note    ���͵�Ӧ����TS���ĸ����Ŀ�ͱ����Ŀһ��ע�����Ѵ�С����һ��
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginGetSendDecoderLen) (HCSHANDLE hIPClient, DWORD *pdwLen);

/**
@brief   ��������ܷ���
@param[in] hIPClient ������
@param[in] nCommand  Ҫִ�е����ÿ���������һ�����ɲ���Լ�ȥ���壬�����ǲ���ڲ������ö����
@param[in] pInputPara  ����������ڴ����ⲿ���䣬�˴����Ͳ����壬�ɲ���Լ�ȥ���壬�����ǲ���Լ�����Ľṹ��
@param[out] pOutputPara  ����������ڴ����ⲿ���䣬�˴����Ͳ����壬�ɲ���Լ�ȥ���壬�����ǲ���Լ�����Ľṹ��
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ����������ʵ��ÿ��������еĹ��ܣ�ÿ�������������Щ�����������һ���Ĺ���
@note    ������������δ��ʾ���壬Ŀ���ǽ���ÿ������Լ�ȥ����
@note    ÿ�����������ͨ���÷�����������չ�Լ������⹦�ܣ�ֻҪʹ����Щ���⹦�ܵĿͻ�֪��
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginInvoke) (HCSHANDLE hIPClient, int  nCommand, void *pInputPara, void *pOutputPara);

/**
@brief   ���ò��ſ�ʼʱ��
@param[in] hIPClient ������
@param[in] nTime  ����ڽ�Ŀ��ʼʱ��Ĳ���ʱ��㣬��λΪ�롣��UTCʱ��ת������ֵ
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note    ������ͬ�������첽����������ʵ���б���ͨ���ص�������ִ�н����֮������
		 �����ͬ����ʽ�����ڴ˺����н���Ӧ��������첽��ʽ����Ҫÿ���̶�ʱ��ȥ����Ӧ��
         ִ�гɹ��ڻص������з���EM_CSIPPUMP_MSG_SETTIME_SUCC��Ϣ
         ִ��ʧ���ڻص������з���EM_CSIPPUMP_MSG_SERVER_ERROR��Ϣ���Ҳ���1��ʾ����ԭ�򣬲���2Ԥ��
@note    ���������Բ�ʵ�֣���������ͨ��m_fnCheckCapability��ѯ��
*/
typedef  int(*FonIPPluginSetUTCTime) (HCSHANDLE hIPClient, DWORD nTime);
/**
@brief  ���������Ϣ
@note   һ���������������һЩ��Ϣ
@note   m_fnCreate��m_fnDestory��FonIPPluginReadStream��m_fnCheckCapability��m_fnGetCurMediaType��ÿ���������ʵ�ֵģ�
        �������������ѡ��ʵ��
@note   ͨ��m_fnCheckCapability����ÿ����������Բ�ѯ��ĳ�������Ƿ�֧��
@note   ���ĳ��������֧�֣����Բ�ʵ�ֶ�Ӧ�ķ����������ڱ������п��Բ�ѯ��
*/
typedef struct _CSIPPluginInfo_S 
{
	char   m_szName[CS_IPPLUGIN_NAME_LEN];                 //������֣�����Ψһ��ʶ�˲����������������ֲ���һ������'\0'��β
	char   m_szDescription[CS_IPPLUGIN_DESCRIPTION_LEN];  //�����������Ϣ����'\0'��β
	char   m_szType[CS_IPPLUGIN_PROTOCOL_NAME_LEN];      //���֧�ֵ�Э�����ͣ���'\0'��β
	FonIPPluginCreate            m_fnCreate;
	FonIPPluginDestroy          m_fnDestory;
	FonIPPluginReadStream    m_fnReadStream;
	FonIPPluginCheckCapability        m_fnCheckCapability;
	FonIPPluginGetCurMediaType     m_fnGetCurMediaType;
	//�Դ�Ϊ����ǰ��ķ�������ʵ�֣�����ķ�����ѡ��ʵ��
	FonIPPluginPause        m_fnPause;
	FonIPPluginResume     m_fnResume;
	FonIPPluginSetRate     m_fnSetRate;
	FonIPPluginGetRate     m_fnGetRate;
	FonIPPluginSetTime     m_fnSetTime;
	FonIPPluginGetTime     m_fnGetTime;
	FonIPPluginGetDuration            m_fnGetDuration;
	FonIPPluginGetSendDecoderLen    m_fnGetSendDecoderLen;
	FonIPPluginInvoke        m_fnInvoke;
	FonIPPluginSetDuration            m_fnSetDuration;
	FonIPPluginSetDMXBufferingCheckMaxSize m_fnSetDMXBufferingCheckMaxSize;
	FonIPPluginGetDMXBufferingPercent m_fnGetDMXBufferingPercent;
	FonIPPluginSetUTCTime     m_fnSetUTCTime;
} CSIPPluginInfo_S;

/**
@brief   ��ȡ�����������Ϣ
@param[out] psPluginInfo �������Ϣ�ṹ�壬�ڴ����ⲿ����
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note  ��������ÿһ���������ʵ�ֵĶ��⹫��������ͨ��������Ӧ�ÿ��Ի�ò����������Ϣ
@note  ���������⣬�������������Ӧ��ȫ����˽�еģ�Ӧ������Ϊstatic����
@note  �ⲿ��������Ϣ�Ľṹ���ڴ棬�������ڲ�ȥ���ýṹ��
*/
typedef int (*FOnCSIPGetPluginInfo) (CSIPPluginInfo_S  *psPluginInfo);


/**
@brief   ע��һ�����
@param[in] fnIPGetPluginInfo �����¶�Ļ�ȡ��������Ϣ�ķ���
@return  �ɹ�����EM_PUMP_SUCCESS������Ϊʧ�ܣ�����μ�ö����CSIPPluginRet_E
@note  �������ṩ��Ӧ��ʹ�ã�Ӧ��ͨ���÷���ע��һ�����
@note  ͬһ�����ֻ��ע��һ�Σ��ظ�ע��᷵��ʧ��
*/
int  CSIPPMRegister(FOnCSIPGetPluginInfo  fnIPGetPluginInfo);

/**
@brief   ����һ�����
@param[in] pcType Ҫ����Ĳ����Э�����ͣ���"rtsp"��"http"��
@param[in] pUserData�û�˽�����ݣ�����IPUDP����ѡ������ʱ����������������ֵ㲥���͵�rtsp�����IPDUP���͵�rtsp�������IPUDP���Ϳ��Դ���
@return  �ɹ�����һ���������Ϣ�ṹ�壬ʧ�ܷ���NULL
@note    �����ʹ����ͨ���÷�����ȡָ��Э��Ĳ��
*/
CSIPPluginInfo_S *  CSIPPMRequest(char *pcType, void *pUserData);

/**
@brief   ���õ㲥���������
@param[in] ePluginType �㲥�����������
@return  �ɹ�����TRUE��ʧ�ܷ���FALSE
@note    �˽ӿ���������һ���汾ͬʱע�����㲥�����ͨ����CFGý�岥�ų�ʼ�����ò��������ʶ����
*/
BOOL CSIPPMSetVodPluginType(CSIPPluginNameType_E ePluginType);

#ifdef __cplusplus
}
#endif

#endif  

