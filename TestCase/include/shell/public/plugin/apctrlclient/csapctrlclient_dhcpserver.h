/**@defgroup  
@brief 
1����ģ�鶨���ip��ַ��������"192.168.1.1"��ASII���ַ�����ʽ��ʾ��������󳤶�Ϊ16���ֽ�(���һ���ֽ�Ϊ\0)
2����ģ�鶨���ip��ַ��Χ��������"192.168.1.1~255"���ַ�����ʽ��ʾ��������󳤶�Ϊ20(���һ���ֽ�Ϊ\0)
2����ģ��ʹ�õ�MAC��ַ��������{0x00,0x0e,0x09,0x11,0x12,0xe1}���޷���BYTE���鷽ʽ��ʾ������ĳ�����6���ֽ�

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _CS_APCTRL_CLIENT_DHCPSERVER_H_
#define _CS_APCTRL_CLIENT_DHCPSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csapctrlclient.h"

/**@brief DHCP Server�����ַ��Ϣ����*/
typedef struct{
	char aucIPStart[CSAPCTRLC_IP_LEN]; 	///<��ַ�ؿ�ʼIP��ַ
	char aucIPEnd[CSAPCTRLC_IP_LEN];  		///<��ַ�ؽ���IP��ַ,(��ʼ��ַ&mask) < (������ַ&mask)
	char aucMask[CSAPCTRLC_IP_LEN]; 		///<DHCP����IP��ַ����������,�ַ�����"192.168.1.1"
	char aucGatway[CSAPCTRLC_IP_LEN]; 	///<Ĭ������,�ַ�����"192.168.1.1",Ҫ����ʼ��ַ,������ַ,������ͬһ������
	unsigned int   uLeaseTime;  				///<��ַ��Լʱ��,��λΪ��
}CSAPCTRLCDHCPSConfig_S;

/**@brief ��̬���õ�IP��ַ��MAC��ַ��*/
typedef struct{
	char aucIP[CSAPCTRLC_IP_LEN]; 			///<IP��ַ,�ַ�����"192.168.1.1"
	unsigned char aucMac[CSAPCTRLC_MAC_LEN]; 		///<MAC��ַ,��ֵ��{0x00,0x01,0x01,0x01,0x01,0x01}
}CSAPCTRLCDHCPSStaticIpCfg;

/**@brief DHCPServer�Ѿ�����ĵ�ַ��Ϣ*/
typedef struct{
	char aucIP[CSAPCTRLC_IP_LEN]; 			///<IP��ַ,�ַ�����"192.168.1.1"
	unsigned char aucMac[CSAPCTRLC_MAC_LEN];		 ///<MAC��ַ,��ֵ��{0x00,0x01,0x01,0x01,0x01,0x01}
	unsigned int 	nLeftTime;  					///<����client��Լ��ַ��Ч�ڵ�ʣ��ʱ��
}CSAPCTRLCDHCPSLeaseInfo;

/**
@brief ����DHCP������

@param[in] szIfName :�����豸����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLCDHCPSStart(const char* szIfName);

/**
@brief ֹͣDHCP������

@param[in] szIfName :�����豸����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLCDHCPSStop(const char* szIfName);

/**
@brief DHCP�����Ƿ�������

@param[in] szIfName :�����豸����
@param[out] pbOutIsRunning DHCP�����Ƿ�������
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLCDHCPSIsRunning(const char* szIfName,CSUDI_BOOL* pbOutIsRunning);


/**
@brief ����DHCP����������

@param[in] szIfName :�����豸����
@param[in] psDHCPCfg ָ�����ò�����ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLCDHCPSConfig(const char* szIfName,const CSAPCTRLCDHCPSConfig_S *psDHCPCfg);

/**
@brief ��ȡDHCP�����������ò���

@param[in] szIfName :�����豸����
@param[out] psOutDHCPCfg DHCP�������ĵ�ַ�غ����ز���
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSAPCTRLCDHCPSGetConfig(const char* szIfName,CSAPCTRLCDHCPSConfig_S *psOutDHCPCfg);

/**
@brief ��DHCP��������Ӿ�̬���õ�IP��ַ��MAC��ַ��,��������ӵ�MAC DHCPʱ�����ָ����IP��ַ

@param[in] szIfName :�����豸����
@param[in] psItem ָ��MAC��IP��ַ�Խṹ�����ָ��
@param[in] nCount ָ��pstItemָ���ڴ������CSAPCTRLCDHCPSStaticIpCfg�ṹ����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
@note �ú�������ͬʱ��Ӷ����̬����,���������nCount����ָ��
*/
CSUDI_Error_Code  CSAPCTRLCDHCPSAddStaticIPCfg(const char* szIfName,const CSAPCTRLCDHCPSStaticIpCfg *psItem,int nCount);

/**
@brief ��DHCP������ɾ����̬���õ�IP��ַ��MAC��ַ��

@param[in] szIfName :�����豸����
@param[in] ptItem ָ��MAC��IP��ַ�Խṹ�����ָ��
@param[in] nCount ָ��pstItemָ���ڴ������CSAPCTRLCDHCPSStaticIpCfg�ṹ����
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
@note �ú�������ͬʱɾ�������̬����,���������nCount����ָ��
*/
CSUDI_Error_Code  CSAPCTRLCDHCPSDelStaticIPCfg(const char* szIfName,const CSAPCTRLCDHCPSStaticIpCfg *pstItem,int nCount);

/**
@brief ��ȡDHCP�����������ò���

@param[in] szIfName :�����豸����
@param[out] psItem ��̬���õĻ�����Ϣ
@param[in/out] pnStaticCount ����������� ,���봫�������С�����ʵ�ʻ�ȡ���ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code  CSAPCTRLCDHCPSGetStaticIPCfgs(const char* szIfName,CSAPCTRLCDHCPSStaticIpCfg *psItem,int *pnStaticCount);

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
CSUDI_Error_Code  CSAPCTRLCDHCPSGetLeaseInfo(const char* szIfName,CSAPCTRLCDHCPSLeaseInfo *pstLeaseInfo,int *pnCount);

#ifdef __cplusplus
}
#endif

#endif


