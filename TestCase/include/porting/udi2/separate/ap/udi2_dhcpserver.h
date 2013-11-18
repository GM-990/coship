/**@defgroup  ��ģ���UDI2DHCPSERVER����ӿں����ݽṹ�����˶���
@brief UDI2DHCPSERVERģ���Ӧ��ͷ�ļ�<udi2_dhcpserver.h>,������DHCPSERVER���ò����ӿ�
1����ģ��ʹ�õ�ip��ַ��ʾ��ʽ�ǵ��ip��ַ��������󳤶�Ϊ16���ֽ���:"192.168.1.1" ���ֱ�ʾ��ʽ
2����ģ��ʹ�õ�MAC��ַ������ĳ�����6���ֽ���:{0x00,0x0e,0x09,0x11,0x12,0xe1}

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _UDI2_DHCPSERVER_H_
#define _UDI2_DHCPSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_error.h"

#define CSUDI_IP_LEN (16)				///<IP��ַ��󳤶�
#define CSUDI_MAC_LEN (6)				///<MAC��ַ����

/**@brief  DHCP server��������ö�ٶ���*/
enum
{    
	CSUDIDHCPS_ERROR_BAD_PARAMETER = 130 << 16, 		///<������
	CSUDIDHCPS_ERROR_NOT_CONFIG,  ///δ���л���DHCP server���þ�����DHCP server
	CSUDIDHCPS_ERROR_MORE_THAN_MAX ///���ó�������֧�ֵ����ֵ
};
/**@brief DHCP Server�����ַ��Ϣ����*/
typedef struct{
	char acIPStart[CSUDI_IP_LEN]; 	///<��ַ�ؿ�ʼIP��ַ
	char acIPEnd[CSUDI_IP_LEN];  		///<��ַ�ؽ���IP��ַ,(��ʼ��ַ&mask) < (������ַ&mask)
	char acMask[CSUDI_IP_LEN]; 		///<DHCP����IP��ַ����������,�ַ�����"192.168.1.1"
	char acGatway[CSUDI_IP_LEN]; 	///<Ĭ������,�ַ�����"192.168.1.1",Ҫ����ʼ��ַ,������ַ,������ͬһ������
	char acFirstDNS[CSUDI_IP_LEN]; ///��ѡDNS������IP��ַ,�ַ�����ʽ,��ѡDNS��������
	char acSecondDNS[CSUDI_IP_LEN]; ///����DNS������IP��ַ,�ַ�����ʽ,��ѡDNS���Բ�����,������ʱ���մ�
	unsigned int   uLeaseTime;  				///<��ַ��Լʱ��,��λΪ��
}CSUDIDHCPSConfig_S;

/**@brief ��̬���õ�IP��ַ��MAC��ַ��*/
typedef struct{
	char acIP[CSUDI_IP_LEN]; 			///<IP��ַ,�ַ�����"192.168.1.1"
	unsigned char aucMac[CSUDI_MAC_LEN]; 		///<MAC��ַ,��ֵ��{0x00,0x01,0x01,0x01,0x01,0x01}
}CSUDIDHCPSStaticIpCfg;

/**@brief DHCPServer�Ѿ�����ĵ�ַ��Ϣ*/
typedef struct{
	char acIP[CSUDI_IP_LEN]; 			///<IP��ַ,�ַ�����"192.168.1.1"
	unsigned char aucMac[CSUDI_MAC_LEN];		 ///<MAC��ַ,��ֵ��{0x00,0x01,0x01,0x01,0x01,0x01}
	unsigned int 	nLeftTime;  					///<����client��Լ��ַ��Ч�ڵ�ʣ��ʱ��
}CSUDIDHCPSLeaseInfo;

/**
@brief ����DHCP������

@param[in] szIfName :�����豸����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIDHCPSStart(const char* szIfName);

/**
@brief ֹͣDHCP������

@param[in] szIfName :�����豸����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIDHCPSStop(const char* szIfName);

/**
@brief ����DHCP����������

@param[in] szIfName :�����豸����
@param[in] psDHCPCfg ָ�����ò�����ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIDHCPSConfig(const char* szIfName,const CSUDIDHCPSConfig_S *psDHCPCfg);

/**
@brief ��ȡDHCP�����������ò���

@param[in] szIfName :�����豸����
@param[out] psOutDHCPCfg DHCP�������ĵ�ַ�غ����ز���
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIDHCPSGetConfig(const char* szIfName,CSUDIDHCPSConfig_S *psOutDHCPCfg);

/**
@brief ��DHCP��������Ӿ�̬���õ�IP��ַ��MAC��ַ��,��������ӵ�MAC DHCPʱ�����ָ����IP��ַ

@param[in] szIfName :�����豸����
@param[in] psItem ָ��MAC��IP��ַ�Խṹ�����ָ��
@param[in] nCount ָ��pstItemָ���ڴ������CSUDIDHCPSStaticIpCfg�ṹ����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
@note �ú�������ͬʱ��Ӷ����̬����,���������nCount����ָ��
@note �����ӵ�IP���ڵ�ַ�ط�Χ���򣬸õ�ַӳ�䲻���ڷ����ʱ����Ч
*/
CSUDI_Error_Code  CSUDIDHCPSAddStaticIPCfg(const char* szIfName,const CSUDIDHCPSStaticIpCfg *psItem,int nCount);

/**
@brief ��DHCP������ɾ����̬���õ�IP��ַ��MAC��ַ��

@param[in] szIfName :�����豸����
@param[in] ptItem ָ��MAC��IP��ַ�Խṹ�����ָ��
@param[in] nCount ָ��pstItemָ���ڴ������CSUDIDHCPSStaticIpCfg�ṹ����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
@note �ú�������ͬʱɾ�������̬����,���������nCount����ָ��
*/
CSUDI_Error_Code  CSUDIDHCPSDelStaticIPCfg(const char* szIfName,const CSUDIDHCPSStaticIpCfg *pstItem,int nCount);

/**
@brief ��ȡDHCP�����������ò���

@param[in] szIfName :�����豸����
@param[out] psItem ��̬���õĻ�����Ϣ
@param[in/out] pnStaticCount ����������� ,���봫�������С�����ʵ�ʻ�ȡ���ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code  CSUDIDHCPSGetStaticIPCfgs(const char* szIfName,CSUDIDHCPSStaticIpCfg *psItem,int *pnStaticCount);

/**
@brief ��ȡDHCP���������ѷ����ַ��Ϣ

@param[in] szIfName :�����豸����
@param[out] pstLeaseInfo �û�����ı����ѷ����ַ��Ϣ��ָ�룬���֧�ַ���CSNET_MAX_DHCPS_LEASE_COUNT ��
@param[in/out] pnCount ��Ϊ�������ʱ��ָ�뱣�����psLeaseInfo����Ĵ�С,��Ϊ�������ʱ����ʵ�ʻ�ȡ�ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
@note ���øú���ʱpnCount��ָ�Ķ���һ��Ҫ����ֵ,�������н������Ԥ��
*/
CSUDI_Error_Code  CSUDIDHCPSGetLeaseInfo(const char* szIfName,CSUDIDHCPSLeaseInfo *pstLeaseInfo,int *pnCount);

#ifdef __cplusplus
}
#endif

#endif


