/**@defgroup  ��ģ���UDI2RT����ӿں����ݽṹ�����˶���
@brief UDI2RTģ���Ӧ��ͷ�ļ�<udi2_router.h>,������router���ò����ӿ�

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _CS_APCTRL_CLIENT_ROUTER_H_
#define _CS_APCTRL_CLIENT_ROUTER_H_

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	CSAPCTRLCRT_ERROR_BAD_PARAMETER = 130 << 16, 			///<��������
	CSAPCTRLCRT_ERROR_UNKNOW_SERVICE,					///<δ֪����
	CSAPCTRLCRT_ERROR_EXCEED_MAX_NUMBER,				///<����������
	CSAPCTRLCRT_ERROR_SERVICE_IS_STOPED				///��ǰ�����ڹر�״̬
};

typedef enum{
	EM_APCTRLCRT_SERVICETYPE_DNS_PROXY, 	///< DNS������� ʹ��LAN������DNS����
	EM_APCTRLCRT_SERVICETYPE_IGMP_PROXY, 	///< IGMP�����������ΪWAN�ڣ�����ΪLAN��
	EM_APCTRLCRT_SERVICETYPE_UPNP 			///< UPNP��������ΪWAN�ڣ�����ΪLAN��
}CSAPCTRLCRTServiceType_E;

/**
@brief ����wan�ں�lan�ڵ�����

@param[in] szWan wan������
@param[in] szLan lan������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLCRTSetInterfaces(const char* szWan,const char* szLan);

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
CSUDI_Error_Code CSAPCTRLCRTGetInterfaces(char* pcWan,int nWanLen,char* pcLan,int nLanLen);

/**
@brief ����LAN��IP��ַ

@param[in] szLan LAN������
@param[in] szIp  IP��ַ
@param[in] szMask  ��������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLCRTSetLanIp(const char* szLan,const char* szIp,const char* szMask);

/**
@brief ����ָ���������

@param[in] eService ��Ҫ�����������������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLCRTStartService(CSAPCTRLCRTServiceType_E eService);

/**
@brief ָֹͣ���������

@param[in] eService ��Ҫֹͣ�������������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLCRTStopService(CSAPCTRLCRTServiceType_E eService);

/**
@brief ��ȡָ����������״̬

@param[in] eService ָ���������
@param[out] pbIsRunning ���ظ���������״̬,TRUEΪ����״̬,FALSEΪδ����״̬
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLCRTGetServiceStatus(CSAPCTRLCRTServiceType_E eService,CSUDI_BOOL *pbIsRunning);

/**
@brief ִ��iptable�������

@param[in] pszcmd iptable��������ַ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code  CSAPCTRLCRTExecuteIptableCmd(const char *pszcmd);

/**
@brief ����DNS��������ַ

@param[in] szFirstDnsServerIp ��ѡDNS��������ַ������ӦΪ�ַ�����ʽ��:"192.168.1.1"
@param[in] szSecondDnsServerIp ��ѡDNS��������ַ������ӦΪ�ַ�����ʽ��:"192.168.1.1"
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
@note
1��szFirstDnsServerIp����ΪNULL
2��szSecondDnsServerIpΪNULLʱ����ʾδ���ñ�ѡDNS��������ַ
*/
CSUDI_Error_Code CSAPCTRLRTSetDnsServer(const char* szFirstDnsServerIp,const char* szSecondDnsServerIp);

/**
@brief ����wan�ڵ�ַ

@param[in] szWanIp wan�ڵ�ַ������ӦΪ�ַ�����ʽ��:"192.168.1.1"
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��� �������
*/
CSUDI_Error_Code CSAPCTRLRTSetWanIp(const char* szWanIp);


#ifdef __cplusplus
}
#endif
#endif /* __CSLANPLUS_H__ */


