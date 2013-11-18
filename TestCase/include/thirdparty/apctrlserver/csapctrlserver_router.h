/**@defgroup  ��ģ���UDI2RT����ӿں����ݽṹ�����˶���
@brief UDI2RTģ���Ӧ��ͷ�ļ�<udi2_router.h>,������router���ò����ӿ�

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _CS_APCTRL_SERVER_ROUTER_H_
#define _CS_APCTRL_SERVER_ROUTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_error.h"

enum
{
	CSAPCTRLSRT_ERROR_BAD_PARAMETER = 2 << 16, 			///<��������
	CSAPCTRLSRT_ERROR_UNKNOW_SERVICE,					///<δ֪����
	CSAPCTRLSRT_ERROR_EXCEED_MAX_NUMBER,				///<����������
	CSAPCTRLSRT_ERROR_SERVICE_IS_STOPED				///��ǰ�����ڹر�״̬
};

typedef enum{
	EM_APCTRLSRT_SERVICETYPE_DNS_PROXY, 	///< DNS������� ʹ��LAN������DNS����
	EM_APCTRLSRT_SERVICETYPE_IGMP_PROXY, 	///< IGMP�����������ΪWAN�ڣ�����ΪLAN��
	EM_APCTRLSRT_SERVICETYPE_UPNP, 			///< UPNP��������ΪWAN�ڣ�����ΪLAN��
}CSAPCTRLSRTServiceType_E;

/**
@brief ����wan�ں�lan�ڵ�����

@param[in] szWan wan������
@param[in] szLan lan������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLSRTSetInterfaces(const char* szWan,const char* szLan);

/**
@brief ��ȡwan�ں�lan�ڵ�����

@param[in] pcWan : wan������
@param[in] nWanLen :pcWan���鳤��
@param[in] pcLan  :Lan������
@param[in] nLanLen :pcLan���鳤��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLSRTGetInterfaces(char* pcWan,int nWanLen,char* pcLan,int nLanLen);

/**
@brief ����LAN��IP��ַ

@param[in] szLan LAN������
@param[in] szIp  IP��ַ
@param[in] szMask  ��������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLSRTSetLanIp(const char* szLan,const char* szIp,const char* szMask);

/**
@brief ����ָ���������

@param[in] eService ��Ҫ�����������������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLSRTStartService(CSAPCTRLSRTServiceType_E eService);

/**
@brief ָֹͣ���������

@param[in] eService ��Ҫֹͣ�������������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLSRTStopService(CSAPCTRLSRTServiceType_E eService);

/**
@brief ��ȡָ����������״̬

@param[in] eService ָ���������
@param[out] pbIsRunning ���ظ���������״̬,TRUEΪ����״̬,FALSEΪδ����״̬
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLSRTGetServiceStatus(CSAPCTRLSRTServiceType_E eService,CSUDI_BOOL *pbIsRunning);

/**
@brief ִ��iptable�������

@param[in] pszcmd iptable��������ַ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code  CSAPCTRLSRTExecuteIptableCmd(const char *pszcmd);

#ifdef __cplusplus
}
#endif
#endif /* __CSLANPLUS_H__ */


