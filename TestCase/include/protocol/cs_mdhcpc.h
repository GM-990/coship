/**@defgroup MDHCP ��ģ���DHCP����ӿں����ݽṹ�����˶���
@brief MDHCPģ���Ӧ��ͷ�ļ�<cs_mdhcpc.h>,������DHCP���ò����ӿڣ�֧��DHCP RFC2131,
RFC2132��RFC1533��׼

1�����ӿ�֧�ֶ�ʵ���������豸������ΪΨһ��ʶ��
2��CSMDHCPInit�����ᴴ��һ��ʵ�������еĽӿڵĵ��ñ�����CSMDHCPInit����֮��
3����ģ���ʵ������RFC2131��RFC1533��RFC2132����ȡ��IP��ַ���������������
4����ģ����������option֮�⣬��֧�ּ��������optionѡ��:

        ������           ����                                                     ��;      
        
        60                      Class-identifier                                         �������նˡ������˫��У�顢DHCP����������������
        43                      Vendor specific information                         �·�ĳ����������ַ,���������ʵ����
        151-155              private                                                     Coship��׼���������Ӧ�����ط�������ַ���������ڹ㶫ʡ��������
        125                     Vendor-Identifying Vendor-Specific.             �ն˶Է������˽���У��
����ͻ���Ҫ֧�������������⼸��optionѡ��������CSMDHCPSetSendOption�����ã������Ҫ�ͻ���Ҳ��֤������Ҫ
����CSMDHCPSetRecvOption�����ÿͻ���У���ֶ�

5������option���ձ���Щoption����Ҫ�û����ã�DHCP ģ�鷢���ͻ������Щoption

        ������     			      ����                                                           
		1			 		      ����
		3						����
		6						DNS������
		51						DHCP������Ϣ
		53						DHCP������ IP��ַ
		55						DHCP ����ѡ���б�
5���û����Զ���ȡip��ַ�л�����̬����IP��ַ���������CSMDHCPStop��ֹͣDHCP�������þ�̬IP��ַ
6���û��Ӿ�̬IP��ַ�л����Զ���ȡIP,ֱ�ӵ���CSMDHCPStart����ȡIP��ַ

@version 2.0.1 2011/11/14 
@{
*/

#ifndef __MDHCPC_H_INCLUDED__
#define __MDHCPC_H_INCLUDED__

#include "mmcp_typedef.h"
#include "csshell_mdhcpc.h"

#if defined(__cplusplus) || defined(__CPLUSPLUS)
extern "C" {
#endif 

/**
@brief ��ʼ��DHCP

@param[in] pcEthName:�����豸����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
@note �ýӿڲ�������dhcp�������ǳ�ʼ������
*/
int  CSMDHCPInit(char * pcEthName,CSMDHCPInitPara_S *psInitPara);

/**
@brief �����Ƿ����DHCP��ȡ��DNS��gateway��Ĭ��Ϊ���á�

@param[in] pcEthName : ��Ӧ���õ���������
@param[in] bEnableDNS : �����Ƿ����DHCP�����������DNS��TRUE��ʾ���ã�FALSE��ʾ������
@param[in] bEnableGW : �����Ƿ����DHCP�����������gateway,TRUE��ʾ���ã�FALSE��ʾ������
@return 
- ����ɹ�����CSUDI_SUCCESS
- ���ʧ�ܷ���CSUDI_FAILURE
@note �˽ӿ���CSMDHCPInit֮��CSMDHCPStart֮ǰ����
*/
int CSMDHCPDnsGwSet(char * pcEthName, BOOL bEnableDNS, BOOL bEnableGW);


/**
@brief ����DHCP

@param[in] pcEthName:�����豸����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
@note ���ӿ��첽ִ�У����سɹ�����ʾ�����ѽ��գ�����ʾ����ִ�н����
*/
int  CSMDHCPStart(char * pcEthName);

/**
@brief ֹͣDHCP

@param[in] pcEthName:�����豸����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
@note ���ӿ��첽ִ�У����سɹ�����ʾ�����ѽ��գ�����ʾ����ִ�н����
*/
int  CSMDHCPStop(char * pcEthName);

/**
@brief ����DHCP optionѡ��,����֮�󣬷����ͻ���������õ�ѡ��

@param[in] pcEthName:�����豸����
@param[in] nOptCode:Optionѡ��Ĳ�����,�����뷶Χ0-255���������������ĺ�����μ�RFC2132
@param[in] pOptVal:Optionѡ���ֵ
@param[in] nOptLen:Optionѡ��ĳ���
@return 
- �ɹ�����CSUDI_SUCCESS��
- ʧ�ܷ���CSUDI_FAILURE
*/
int CSMDHCPSetSendOption(char * pcEthName,  int nOptCode, void *pOptVal,int nOptLen);

/**
@brief ����DHCP optionѡ�����֮�󣬿ͻ��˽����DHCP������Ӧ������ݰ�����Ӧ��У��

@param[in] pcEthName:�����豸����
@param[in] nOptCode:Optionѡ��Ĳ�����,�����뷶Χ0-255���������������ĺ�����μ�RFC2132
@param[in] pOptVal:Optionѡ���ֵ
@param[in] nOptLen:Optionѡ��ĳ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
*/
int CSMDHCPSetRecvOption(char * pcEthName,  int nOptCode, void *pOptVal,int nOptLen);

/**
@brief ��ȡDHCP optionѡ��

@param[in] pcEthName:�����豸����
@param[in] nOptCode:Optionѡ��Ĳ�����,�����뷶Χ0-255���������������ĺ�����μ�RFC2132
@param[out] pOptVal:������Ż�ȡ��option ѡ���ֵ��buffer�ĳ��Ȳ���С��256
@param[out] nOptLen:pOptVal��ָ��Ĵ洢�ռ�Ĵ�С
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
*/
int CSMDHCPGetOption(char * pcEthName,  int nOptCode,void *pOptVal, int nOptLen);

/**
@brief ����DHCP���ĵķ������

@param[in] pcEthName �����豸����
@param[in] ePacketType: �����ͣ����CSMDHCPPacketType_E ����
@param[in] fnGetSendInterval: ����ָ�룬��ȡ����ʱ�������ú������û�ʵ�֣�����ֵ��λ:��(s),��������ã�������Ĭ��ֵ���з���
@return
- ����ɹ�����0��
- ���ʧ�ܷ���-1
@note  �����ã�DHCPDISCOVER���Ľ�����Ĭ��ʱ������Ĭ��ʱ����[1,2,4,8,16,32,64,128,256],ѭ��
*/
int CSMDHCPSetPacketInterval(char * pcEthName,CSMDHCPPacketType_E ePacketType, CSOnMDHCPGetSendInterval_F fnGetSendInterval);

/**
@brief ����DHCP��Լģʽ

@param[in] pcEthName �����豸����
@param[in] nSendTime: ����RENEW���ĵ�ʱ�䣬��Լ����ֱȣ�����:������75%��Լ����Ҫ��7500
@param[in] eSendType: ����ģʽ�����CSMDHCPPacketMode_E����
@return
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
@note  ����RFC2131��׼��DHCPģ����50%��Լ���͵�������Լ����Լ���ɹ�����87.5%���͹㲥����Լ��\n
��Լ���ɹ����ȴ���Լ����ʱ�����·���DHCPDISCOVER ����
@note  ������Ŀ�ڱ�׼�Ļ����ϼ�����˽�еķ�����ʽ��Ŀǰ����������
@note  ��������:50%��Լ���ɹ���ʱ��������75%��Լ��ʱ���͵�������Լ
@note �Ϻ���������:50%��Լ���ɹ���ʱ��������68.75%���͵�������Լ��87.5%��Լ���ɹ���ʱ��������93.75%���͹㲥����Լ
*/
int CSMDHCPAddRenewPacketSendMode(char * pcEthName,int nSendTime,CSMDHCPPacketMode_E eSendType); 

/**
@brief  ��ȡDHCP�������������ֶ�

@param[in] pcEthName �����豸����
@param[out] pucServerHostName: �û����õķ������������ֶε�ֵ�����Ȳ�����64�ֽ�
@param[in] dwLen pucServerHostName��ָ��Ĵ洢�ռ�Ĵ�С
@return 
- �ɹ�����CSUDI_SUCCESS
- ʧ�ܷ���CSUDI_FAILURE
@note:�еľַ���VOD�������ĵ�ַ��������ֶ����·�������Ӧ����Ҫ���øýӿڻ�ȡvod�������ĵ�ַȥ�㲥
*/
int CSMDHCPGetServerHostName(char * pcEthName,BYTE *pucServerHostName,DWORD dwLen);

/**
@brief ���DHCP֪ͨ�ص�����

@param[in] fnCallback �ص�����,��DHCP״̬�����仯ʱ���øú���
@param[in] pvUserData �û�����,����Ϊ�ص�������pvUserData��������
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
@note ��ӻص�ǰ������DHCP״̬�仯���Ჹ��֪ͨ
*/
int CSMDHCPAddCallback(char * pcEthName,CSOnMDHCPChange_F fnCallback,void* pvUserData);

/**
@brief ɾ��DHCP�ص�����

@param[in] fnCallback �ص�����,֮ǰע��ĺ���
@param[in] pvUserData �û�����,������ע��ʱ��һ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ���CSUDI_FAILURE
@note fnCallback��pUserDataһ��Ψһȷ��һ���ص�����
*/
int CSMDHCPRemoveCallback(char * pcEthName,CSOnMDHCPChange_F fnCallback);

#if defined(__cplusplus) || defined(__CPLUSPLUS)
}
#endif

#endif 

