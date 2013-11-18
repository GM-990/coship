/**@defgroup SHELLMDHCP ��ģ���DHCP����ӿں����ݽṹ�����˶���
@brief MDHCPģ���Ӧ��ͷ�ļ�<csshell_mdhcpc.h>,������DHCP���ò����ӿڣ�֧��DHCP RFC2131,
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
@version 2.0.2 2011/12/05
@{
*/
#ifndef __SHELL_MDHCPC_H_INCLUDED__
#define __SHELL_MDHCPC_H_INCLUDED__

#include "mmcp_typedef.h"

#if defined(__cplusplus) || defined(__CPLUSPLUS)
extern "C" {
#endif 

#define MDHCP_ETH_NAME_LEN 32
#define MDHCP_SERVER_HOSTNAME_LEN 64

/**@brief  MDHCPģ����Ϣ״̬ö�����Ͷ���*/
typedef enum 
{
    EM_MDHCP_UP = 0,			///<  ��ȡ��ip��ַ
    EM_MDHCP_DOWN,                	///< ��������down��״̬ 
    EM_MDHCP_INFORM,                 ///< ֪ͨ��Ϣ
    EM_MDHCP_RENEW,                  ///<���ڸ���״̬
    EM_MDHCP_RENEWED,              ///<���³ɹ�
    EM_MDHCP_RELEASED,             ///<IP��ַ���ͷ�
    EM_MDHCP_FAILED                  ///<��ȡip��ַʧ��
}CSMDHCPEvent_E;

/**@brief  MDHCP�������Ͷ���*/
typedef enum
{
    EM_MDHCP_DISCOVER = 0,		     ///<  DHCPDISCOVER����
    EM_MDHCP_REQUEST = 1              ///< DHCPREQUEST����
}CSMDHCPPacketType_E;

/**@brief  MDHCPģ������DHCP�շ���ģʽ*/
typedef enum
{
    EM_MDHCP_UNICAST = 0,		///<����ʱ����ʾ�Ե�����ʽ���ͣ��հ�ʱ��ָ���������Ե�����ʽӦ�𣬴�ʱIP�����Ŀ��IP��ַ����ָ�ɸ��ͻ��ĵ�ַ����Ϊ�ͻ�û�л�ȡ��IP��ַ֮ǰ������֪������IP��ַ�������ܻᶪ���������
    EM_MDHCP_BROADCAST = 1		///<����ʱ����ʾ�Թ㲥��ʽ���ͣ��հ�ʱ��ָ���������Թ㲥��ʽӦ��ÿһ������������ղ������������
} CSMDHCPPacketMode_E;

/**@brief  MDHCPģ��ص���Ϣ�ṹ�嶨��*/
typedef struct  
{
    unsigned int      uServerIp;            ///<DHCP server��IP��ַ,�����ֽ���
    unsigned int   	uClientIp;              ///<������ͻ��˵� IP��ַ,�����ֽ��� 
    unsigned int      uClientMast;          ///<�ͻ��˵���������,�����ֽ��� 
    unsigned int      uClientGateWay;   ///<�ͻ��˵�����,�����ֽ��� 
    unsigned int      uDns1;                 ///<��DHCP�·���DNS ��������ַ,�����ֽ���
    unsigned int      uDns2;                 ///<��DHCP�·���DNS ��������ַ,�����ֽ���
    unsigned long  	ulLease;                ///< ������ͻ��˵���Լʱ�� ,�����ֽ���
} CSMDHCPClientInfo_S;

/**@brief  MDHCPģ���ʼ����Ϣ�ṹ�嶨��*/
typedef struct  
{
	CSMDHCPPacketMode_E eServerAnswerType;  ///<���÷�������Ӧ��ģʽ,�㲥���ǵ���ģʽ
	BOOL bIsEnableDNS;						///<�����Ƿ����DHCP�����������DNS��TRUE��ʾ���ã�FALSE��ʾ������
	BOOL bIsEnableGW;						///<�����Ƿ����DHCP�����������gateway,TRUE��ʾ���ã�FALSE��ʾ������
	char szServerHostName[MDHCP_SERVER_HOSTNAME_LEN];	///< "\0"������ASCII�ַ���������serverhostname�ֶ�,����RFC2131��׼�����ֶ��ɷ�������д������û����ô��ֶΣ����ڿͻ����յ�DHCPOFFERʱ����Ƚϴ��ֶΣ�����������õ���ͬ����ܸ�offer�������򲻽��ܸ�offer����������������
} CSMDHCPInitPara_S;

/**
@brief MDHCP��ȡ����ʱ���������û�ʵ��

@param[in] nIndex ��������
@note ���ݴ����index,��ȡ����ʱ����
*/
typedef unsigned int (*CSOnMDHCPGetSendInterval_F)(unsigned int nIndex);

/**
@brief MDHCPģ���¼��ص�����ԭ�Ͷ���

@param[in] pcEthName �����豸����
@param[in] eEvent �ص���Ϣ�����CSMDHCPEvent_E����
@param[in] sClientInfo �ص���Ϣ���ݣ����CSMDHCPClientInfo_S���塣
@param[in] pvUserData �û��Զ�������
*/
typedef void  (*CSOnMDHCPChange_F)(CSMDHCPEvent_E eEvent, CSMDHCPClientInfo_S *sClientInfo, void * pvUserData);

typedef struct 
{

	int m_nSize;		///<���ṹ��С��һ������sizeof(CSDHCPInitParam_S)

	/**
	@brief ��ʼ��DHCP

	@param[in] pcEthName:�����豸����
	@return 
	- ����ɹ�����0��
	- ���ʧ�ܷ���-1
	@note �ýӿڲ�������dhcp�������ǳ�ʼ������
	*/
	int (* m_fnDHCPInit)(char * pcEthName,CSMDHCPInitPara_S *sInitPara);

	/**
	@brief ����DHCP

	@param[in] pcEthName:�����豸����
	@return 
	- ����ɹ�����0��
	- ���ʧ�ܷ���-1
	@note ���ӿ��첽ִ�У����سɹ�����ʾ�����ѽ��գ�����ʾ����ִ�н����
	*/
	int (* m_fnDHCPStart)(char* pcEthname);

	/**
	@brief ֹͣDHCP

	@param[in] pcEthName:�����豸����
	@return 
	- ����ɹ�����0��
	- ���ʧ�ܷ���-1
	@note ���ӿ��첽ִ�У����سɹ�����ʾ�����ѽ��գ�����ʾ����ִ�н����
	*/
	int (* m_fnDHCPStop)(char* pcEthname);

	/**
	@brief ����DHCP optionѡ��,����֮�󣬷����ͻ���������õ�ѡ��

	@param[in] pcEthName:�����豸����
	@param[in] nOptCode:Optionѡ��Ĳ�����,�����뷶Χ0-255���������������ĺ�����μ�RFC2132
	@param[in] pOptVal:Optionѡ���ֵ
	@param[in] nOptLen:Optionѡ��ĳ���
	@return 
	- �ɹ�����0��
	- ʧ�ܷ���-1
	*/
	int (* m_fnDHCPSetSendOption)(char * pcEthName,  int nOptCode, void *pOptVal,int nOptLen);

	/**
	@brief ����DHCP optionѡ�����֮�󣬿ͻ��˽����DHCP������Ӧ������ݰ�����Ӧ��У��

	@param[in] pcEthName:�����豸����
	@param[in] nOptCode:Optionѡ��Ĳ�����,�����뷶Χ0-255���������������ĺ�����μ�RFC2132
	@param[in] pOptVal:Optionѡ���ֵ
	@param[in] nOptLen:Optionѡ��ĳ���
	@return 
	- ����ɹ�����0��
	- ���ʧ�ܷ���-1
	*/
	int (* m_fnDHCPSetRecvOption)(char * pcEthName,  int nOptCode, void *pOptVal,int nOptLen);

	/**
	@brief ��ȡDHCP optionѡ��

	@param[in] pcEthName:�����豸����
	@param[in] nOptCode:Optionѡ��Ĳ�����,�����뷶Χ0-255���������������ĺ�����μ�RFC2132
	@param[out] pOptVal:������Ż�ȡ��option ѡ���ֵ��buffer�ĳ��Ȳ���С��256
	@param[out] nOptLen:pOptVal��ָ��Ĵ洢�ռ�Ĵ�С
	@return 
	- ����ɹ�����0��
	- ���ʧ�ܷ���-1
	*/
	int (* m_fnDHCPGetOption)(char * pcEthName,  int nOptCode,void *pOptVal, int nOptLen);

	/**
	@brief ����DHCP���ĵķ���ʱ�����Լ���������

	@param[in] pcEthName �����豸����
	@param[in] ePacketType: �����ͣ����CSMDHCPPacketType_E ����
	@param[in] fnGetSendInterval: ����ָ�룬��ȡ����ʱ�������ú������û�ʵ�֣�����ֵ��λ:��(s),��������ã�������Ĭ��ֵ���з���
	@return
	- ����ɹ�����0��
	- ���ʧ�ܷ���-1
	@note  �����ã�DHCPDISCOVER���Ľ�����Ĭ��ʱ������Ĭ��ʱ����[1,2,4,8,16,32,64,128,256],ѭ��
	*/
	int  (*m_fnDHCPSetPacketInterval)(char * pcEthName,CSMDHCPPacketType_E ePacketType,CSOnMDHCPGetSendInterval_F fnGetSendInterval);

	/**
	@brief ����DHCP��Լģʽ

	@param[in] pcEthName �����豸����
	@param[in] nSendTime: ����RENEW���ĵ�ʱ�䣬��Լ����ֱȣ�����:������75%��Լ����Ҫ��7500
	@param[in] eSendType: ����ģʽ�����CSMDHCPPacketMode_E����
	@return
	- ����ɹ�����0��
	- ���ʧ�ܷ���-1
	@note  ����RFC2131��׼��DHCPģ����50%��Լ���͵�������Լ����Լ���ɹ�����87.5%���͹㲥����Լ��\n
	��Լ���ɹ����ȴ���Լ����ʱ�����·���DHCPDISCOVER ����
	@note  ������Ŀ�ڱ�׼�Ļ����ϼ�����˽�еķ�����ʽ��Ŀǰ����������
	@note  ��������:50%��Լ���ɹ���ʱ��������75%��Լ��ʱ���͵�������Լ
	@note �Ϻ���������:50%��Լ���ɹ���ʱ��������68.75%���͵�������Լ��87.5%��Լ���ɹ���ʱ��������93.75%���͹㲥����Լ
	*/
	int (*m_fnDHCPAddRenewPacketSendMode)(char * pcEthName,int nSendTime,CSMDHCPPacketMode_E eSendType);

	/**
	@brief  ��ȡDHCP�������������ֶ�

	@param[in] pcEthName �����豸����
	@param[out] pucServerHostName: �û����õķ������������ֶε�ֵ�����Ȳ�����64�ֽ�
	@param[in] dwLen pucServerHostName��ָ��Ĵ洢�ռ�Ĵ�С
	@return 
	- �ɹ�����0
	- ʧ�ܷ���-1
	@note:�еľַ���VOD�������ĵ�ַ��������ֶ����·�������Ӧ����Ҫ���øýӿڻ�ȡvod�������ĵ�ַȥ�㲥
	*/
	int (* m_fnDHCPGetServerHostName)(char * pcEthName,BYTE *pucServerHostName,DWORD dwLen );

	/**
	@brief ���DHCP֪ͨ�ص�����

	@param[in] fnCallback �ص�����,��DHCP״̬�����仯ʱ���øú���
	@param[in] pvUserData �û�����,����Ϊ�ص�������pvUserData��������
	@return 
	- ����ɹ�����0��
	- ���ʧ�ܷ���-1
	@note ��ӻص�ǰ������DHCP״̬�仯���Ჹ��֪ͨ
	*/
	int (* m_fnAddDHCPListener)(char * pcEthName,CSOnMDHCPChange_F pCallback,void* pvUserData);

	/**
	@brief ɾ��DHCP֪ͨ�ص�����

	@param[in] fnCallback �ص�����,��DHCP״̬�����仯ʱ���øú���
	@param[in] pvUserData �û�����,����Ϊ�ص�������pvUserData��������
	@return 
	- ����ɹ�����0��
	- ���ʧ�ܷ���-1
	*/
	int (* m_fnRemoveDHCPListener)(char * pcEthName,CSOnMDHCPChange_F pCallback);
}CSMDHCPInitParam_S;

/**
@brief  DHCP ע��ӿ�
@param psParam  DHCP  Shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSMDHCPSetParam(CSMDHCPInitParam_S * psParam);

/**
@brief ��ȡ ��ȡ �ӿڲ���
@param ��
@�ɹ�����CSMDHCPInitParam_S,ʧ�ܷ���NULL
*/
const CSMDHCPInitParam_S* CSMDHCPGetParam();


#if defined(__cplusplus) || defined(__CPLUSPLUS)
}
#endif

#endif 

