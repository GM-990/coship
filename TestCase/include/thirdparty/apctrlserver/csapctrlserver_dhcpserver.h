/**@defgroup  
@brief 
1����ģ�鶨���ip��ַ��������"192.168.1.1"��ASII���ַ�����ʽ��ʾ��������󳤶�Ϊ16���ֽ�(���һ���ֽ�Ϊ\0)
2����ģ�鶨���ip��ַ��Χ��������"192.168.1.1~255"���ַ�����ʽ��ʾ��������󳤶�Ϊ20(���һ���ֽ�Ϊ\0)
2����ģ��ʹ�õ�MAC��ַ��������{0x00,0x0e,0x09,0x11,0x12,0xe1}���޷���BYTE���鷽ʽ��ʾ������ĳ�����6���ֽ�

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _CS_APCTRL_SERVER_DHCPSERVER_H_
#define _CS_APCTRL_SERVER_DHCPSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csapctrlserver.h"

/**@brief DHCP Server�����ַ��Ϣ����*/
typedef struct{
	char aucIPStart[CSAPCTRLS_IP_LEN]; 	///<��ַ�ؿ�ʼIP��ַ
	char aucIPEnd[CSAPCTRLS_IP_LEN];  		///<��ַ�ؽ���IP��ַ,(��ʼ��ַ&mask) < (������ַ&mask)
	char aucMask[CSAPCTRLS_IP_LEN]; 		///<DHCP����IP��ַ����������,�ַ�����"192.168.1.1"
	char aucGatway[CSAPCTRLS_IP_LEN]; 	///<Ĭ������,�ַ�����"192.168.1.1",Ҫ����ʼ��ַ,������ַ,������ͬһ������
	unsigned int   uLeaseTime;  				///<��ַ��Լʱ��,��λΪ��
}CSAPCTRLSDHCPSConfig_S;

/**@brief ��̬���õ�IP��ַ��MAC��ַ��*/
typedef struct{
	char aucIP[CSAPCTRLS_IP_LEN]; 			///<IP��ַ,�ַ�����"192.168.1.1"
	unsigned char aucMac[CSAPCTRLS_MAC_LEN]; 		///<MAC��ַ,��ֵ��{0x00,0x01,0x01,0x01,0x01,0x01}
}CSAPCTRLSDHCPSStaticIpCfg;

/**@brief DHCPServer�Ѿ�����ĵ�ַ��Ϣ*/
typedef struct{
	char aucIP[CSAPCTRLS_IP_LEN]; 			///<IP��ַ,�ַ�����"192.168.1.1"
	unsigned char aucMac[CSAPCTRLS_MAC_LEN];		 ///<MAC��ַ,��ֵ��{0x00,0x01,0x01,0x01,0x01,0x01}
	unsigned int 	nLeftTime;  					///<����client��Լ��ַ��Ч�ڵ�ʣ��ʱ��
}CSAPCTRLSDHCPSLeaseInfo;

/**
@brief ����DHCP������

@param[in] szIfName :�����豸����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSStart(const char* szIfName);

/**
@brief ֹͣDHCP������

@param[in] szIfName :�����豸����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSStop(const char* szIfName);

/**
@brief DHCP�����Ƿ�������

@param[in] szIfName :�����豸����
@param[out] pbOutIsRunning DHCP�����Ƿ�������
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSIsRunning(const char* szIfName,CSUDI_BOOL* pbOutIsRunning);

/**
@brief ����DHCP����������

@param[in] szIfName :�����豸����
@param[in] psDHCPCfg ָ�����ò�����ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSConfig(const char* szIfName,const CSAPCTRLSDHCPSConfig_S *psDHCPCfg);

/**
@brief ��ȡDHCP�����������ò���

@param[in] szIfName :�����豸����
@param[out] psOutDHCPCfg DHCP�������ĵ�ַ�غ����ز���
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLSDHCPSGetConfig(const char* szIfName,CSAPCTRLSDHCPSConfig_S *psOutDHCPCfg);

/**
@brief ��DHCP��������Ӿ�̬���õ�IP��ַ��MAC��ַ��,��������ӵ�MAC DHCPʱ�����ָ����IP��ַ

@param[in] szIfName :�����豸����
@param[in] psItem ָ��MAC��IP��ַ�Խṹ�����ָ��
@param[in] nCount ָ��pstItemָ���ڴ������CSAPCTRLSDHCPSStaticIpCfg�ṹ����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
@note �ú�������ͬʱ��Ӷ����̬����,���������nCount����ָ��
*/
CSUDI_Error_Code  CSAPCTRLSDHCPSAddStaticIPCfg(const char* szIfName,const CSAPCTRLSDHCPSStaticIpCfg *psItem,int nCount);

/**
@brief ��DHCP������ɾ����̬���õ�IP��ַ��MAC��ַ��

@param[in] szIfName :�����豸����
@param[in] ptItem ָ��MAC��IP��ַ�Խṹ�����ָ��
@param[in] nCount ָ��pstItemָ���ڴ������CSAPCTRLSDHCPSStaticIpCfg�ṹ����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
@note �ú�������ͬʱɾ�������̬����,���������nCount����ָ��
*/
CSUDI_Error_Code  CSAPCTRLSDHCPSDelStaticIPCfg(const char* szIfName,const CSAPCTRLSDHCPSStaticIpCfg *pstItem,int nCount);

/**
@brief ��ȡDHCP�����������ò���

@param[in] szIfName :�����豸����
@param[out] psItem ��̬���õĻ�����Ϣ
@param[in/out] pnStaticCount ����������� ,���봫�������С�����ʵ�ʻ�ȡ���ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code  CSAPCTRLSDHCPSGetStaticIPCfgs(const char* szIfName,CSAPCTRLSDHCPSStaticIpCfg *psItem,int *pnStaticCount);

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
CSUDI_Error_Code  CSAPCTRLSDHCPSGetLeaseInfo(const char* szIfName,CSAPCTRLSDHCPSLeaseInfo *pstLeaseInfo,int *pnCount);

#ifdef __cplusplus
}
#endif

#endif


