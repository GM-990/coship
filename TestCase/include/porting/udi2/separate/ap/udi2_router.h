/**@defgroup  ��ģ���UDI2RT����ӿں����ݽṹ�����˶���
@brief UDI2RTģ���Ӧ��ͷ�ļ�<udi2_router.h>,������router���ò����ӿ�

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _UDI2_ROUTER_H_
#define _UDI2_ROUTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_error.h"

#define CSUDI_APRT_IP_LEN (16)				///<IP��ַ��󳤶�


enum
{
	CSUDIRT_ERROR_BAD_PARAMETER = 129 << 16, 			///<��������
	CSUDIRT_ERROR_NOT_SET_WAN_LAN,					///<δ����WAN�ں�LAN��
	CSUDIRT_ERROR_UNKNOW_SERVICE,					///<δ֪����
	CSUDIRT_ERROR_EXCEED_MAX_NUMBER,				///<����������
	CSUDIRT_ERROR_SERVICE_IS_STOPED				///��ǰ�����ڹر�״̬
};

typedef enum{
	EM_RT_SERVICETYPE_DNS_PROXY, 	///< DNS������� ʹ��LAN������DNS����,pvStartParamΪCSUDIRTDnsServers_S*���ͣ�ָ��dns��������ַ
	EM_RT_SERVICETYPE_IGMP_PROXY, 	///< IGMP�����������ΪWAN�ڣ�����ΪLAN�ڣ�pvStartParam��δ����
	EM_RT_SERVICETYPE_UPNP,			///< UPNP��������ΪWAN�ڣ�����ΪLAN��
	EM_RT_SERVICETYPE_MAX
}CSUDIRTServiceType_E;

typedef struct{
	char acFirstDNS[CSUDI_APRT_IP_LEN]; 	 		///< IP��ַ�����ַ�����ʾδ���ø�IP��ַ
	char acSecondDNS[CSUDI_APRT_IP_LEN]; 	 		///< IP��ַ�����ַ�����ʾδ���ø�IP��ַ
}CSUDIRTDnsServers_S;


/**
@brief ����wan�ں�lan�ڵ�����

@param[in] szWan wan������
@param[in] szLan lan������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSUDIRTSetInterfaces(const char* szWan,const char* szLan);

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
CSUDI_Error_Code CSUDIRTGetInterfaces(char* pcWan,int nWanLen,char* pcLan,int nLanLen);

/**
@brief ����LAN��IP��ַ

@param[in] szLan LAN������
@param[in] szIp  IP��ַ
@param[in] szMask  ��������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSUDIRTSetLanIp(const char* szLan,const char* szIp,const char* szMask);

/**
@brief ����ָ���������

@param[in] eService ��Ҫ�����������������
@param[in] pvUserdata ���������������������������Ͷ���˵����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSUDIRTStartService(CSUDIRTServiceType_E eService,void* pvStartParam);

/**
@brief ָֹͣ���������

@param[in] eService ��Ҫֹͣ�������������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSUDIRTStopService(CSUDIRTServiceType_E eService);

/**
@brief ��ȡָ����������״̬

@param[in] eService ָ���������
@param[out] pbIsRunning ���ظ���������״̬,TRUEΪ����״̬,FALSEΪδ����״̬
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSUDIRTGetServiceStatus(CSUDIRTServiceType_E eService,CSUDI_BOOL *pbIsRunning);

/**
@brief ִ��iptable�������

@param[in] pszcmd iptable��������ַ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code  CSUDIRTExecuteIptableCmd(const char *pszcmd);

#ifdef __cplusplus
}
#endif
#endif /* __CSLANPLUS_H__ */


