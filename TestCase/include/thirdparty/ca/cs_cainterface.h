/***************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    cs_cacommand_Interface.h  
* Author:      Wansuanlin
* Date:        Match 2008
* Description: CA command interface
*            
****************************************************************************************************/
#include "mmcp_typedef.h"

#ifndef __CS_CAINTERFACE_H__
#define __CS_CAINTERFACE_H__

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef FAILURE
	#define FAILURE      (-1)
#endif

#ifndef SUCCESS
	#define SUCCESS      (0)
#endif


#ifndef TRUE
	#define TRUE         (1)
#endif

#ifndef FALSE
	#define FALSE        (0)
#endif

typedef enum _CaTraceFlow
{
	CA_FLOW_NONE = 0x00,
	CA_FLOW_INIT = 0x01,
	CA_FLOW_PLAY = 0x02,
	CA_FLOW_UI   = 0x04,
	CA_FLOW_MSG  = 0x08,
	CA_FLOW_ECM  = 0x10,
	CA_FLOW_EMM  = 0x20,
	CA_FLOW_SMC  = 0x40,
	CA_FLOW_KEY  = 0x80,	
    CA_FLOW_TEST= 0x100
}T_CaTraceFlow;

typedef struct _CaFtaService
{
	WORD    m_wDemuxID;          //DMX ID��
	WORD    m_wOrginalNetworkID; //ԭʼ����ID��
	WORD    m_wTsID;             //��ID��
	WORD    m_wServiceID;        //��ĿID��
	WORD    m_wVideoPid;         //��ƵPID
	WORD    m_wAudioPid;         //��ƵPID
	DWORD   m_dwUserData;        //Ԥ��
}T_CaFtaService;

typedef void (*T_CBMsg)(DWORD dwType, DWORD dwParm1, DWORD dwParm2);
typedef INT (*T_CBGetSTBId)(BYTE * pucSTBId,BYTE *pucSTBIdLen);

//�߼���ȫ����
typedef enum _CaSecuredType
{
    EM_CA_SECURED_NACHIPSET_NASECURED, //�Ǹ߼���ȫоƬ�������߼���ȫ����
    EM_CA_SECURED_CHIPSET_SECURED,     //�߼���ȫоƬ�����߼���ȫ����
    EM_CA_SECURED_CHIPSET_NASECURED,   //�߼���ȫоƬ�������߼���ȫ����
    EM_CA_SECURED_MAX
}E_CaSecuredType;

typedef enum _SecurityPolicy
{
	EM_NON_ADVANCE_SECURITY = 0,
	EM_CONAX_VERIFIER_MODE,  
	EM_CONAX_CHIPSET_PAIRING, 
	EM_NAGRA_ST_POLICY ,
	EM_NAGRA_PAIRING, 
    EM_CONAX_CHIPSET_PAIRING_WITH_LP, 
	EM_CA_SECURITY_POLICY_MAX

}E_SecurityPolicy;

typedef enum _PlatType
{
	EM_ST5107 = 0,
	EM_ST7100,  
 	EM_CONEXANT,
	EM_BCM7309,
	EM_BCM740x,
	EM_ST5197,
	EM_MAX
}E_PlatType;

//Verimatrix��ʼ�����ò���
#define MAX_VMX_STRING_lEN 256

typedef enum _SupportEncAlg
{
    emEncAlg_null = 0,
    emEncAlg_RC4 = 1, 
    emEncAlg_AESECBT = 2,
    emEncAlg_proprietary1 = 3,
    emEncAlg_AESECBL = 4,
    emEncAlg_DVBCSA = 5,
    emEncAlg_proprietary2 = 6,
    emEncAlg_AESCBC1 = 7
}SupportEncAlg_E;

#define  MAX_ALGPRITHN_COUNT 10

typedef struct _VMInitParm_S
{
	CHAR			m_acCompany[MAX_VMX_STRING_lEN];		//client register company name
	CHAR			m_acServer[MAX_VMX_STRING_lEN];			//vcas server ip address
	CHAR			m_acVKSServer[MAX_VMX_STRING_lEN];		//VKS Server ip address and port ep, 10.9.8.1:12579
	DWORD			m_dwPort;								//vcas server ip port
	BOOL			m_bEnableIPv6;							//enable ipv6
	DWORD			m_dwTimeout;							//setting specifies the timeout (in seconds) for connect operations.
	CHAR			m_acCertStorePath[MAX_VMX_STRING_lEN];
	CHAR			m_acRootCert[MAX_VMX_STRING_lEN];		//setting specifies the name and location of the ��rootcert.pem�� file
	DWORD			m_dwErrorLevel; 						//range 1-5
	BOOL			m_bEnableLogging;						//setting causes logging to the error log to be disabled. FALSE is disable
	BOOL			m_bEnableHDDecrypt;						//enable hardware decrypt or not

	/*cxj add begin*/
	INT				m_nAlgthSupport;						//֧�ֵĽ����㷨����
	SupportEncAlg_E m_aeAlgSpt[MAX_ALGPRITHN_COUNT];		//֧�ֵĽ����㷨
	/*cxj add end*/
}VMInitParm_S;

//CA���ýṹ
typedef struct _CaConfig
{
	//viaccess caר��
	BOOL     			m_bCamlock;                 //TRUE:����camlock ����,FALSE:�ر�
	BOOL     			m_bIsDrvTest;               //TRUE:�������� ,FALSE:��֤����
	BOOL     			m_bIsPVR;                   //TRUE:PVR����,FALSE:����һ·��Ŀ����
	//conaxר��
	BOOL     			m_bIsIPPV;                  // TRUE:�����ش�����,FALSE:�ر�
	E_PlatType        	m_emPlatType;      			//ƽ̨���ͣ���������chipset pairing mech ����
	//conax & nagra
	E_SecurityPolicy  	m_emPolicy;	     			//�߼���ȫ���õĲ���

	//ͨ��
	E_CaSecuredType   	m_emSecured;       			//�߼���ȫ����
	DWORD    			m_dwCaDataStartAddr;        //  CA flash address, 1 block
	DWORD   			m_dwCaPrivateDataStartAddr; // CA backup flash address,1 block
	DWORD    			m_dwCaOTPAddr;              //����ҪOTP����ʼ��ַ
	BOOL     			m_bIsReverseCard;           //TRUE:��ʾ���򿨰壬Ĭ��FALSE:���򿨰�
	DWORD    			m_dwPrivate;                //Ԥ��
		//Irdeto ר��//re-move in the end of the structure
	BOOL	 			m_bIsFTABlock;    			//�Ƿ���FTABlock����,TRUE:����,FLASH:�ر�
	DWORD	 			m_dwOwnerID;				//IrdetoΪ��ǰ��Ŀ�����ר��OwnerID
	BOOL	 			m_bIsEOF;    				//�Ƿ���EOF����,TRUE:����,FLASH:�ر�
	BOOL	 			m_bIsSOL;					//�Ƿ���SOL����,TRUE:����,FLASH:�ر�

	/*��ȡ�������ڲ����кţ�������Ե�ʱ����õ�,pucSTBIdLenΪ�������������
	�����ʱ��ΪSTBId��buffer��С�������ʱ��ΪSTBId��ʵ�ʳ���*/
	T_CBGetSTBId 		m_stGetSTBId;

	DWORD 				dwSTBSoftVer;				//����汾��
	DWORD 				dwSTBHardVer;				//Ӳ���汾��
	DWORD 				dwManufacturerCode;			//���̴���
	VMInitParm_S     	m_sConfigParm;				//cxj add for Verimatrix init param
}CaConfig_S;

/**************************************************************************
*Function Name: CSCAConfig
*   ���ó�ʼ��CAģ���������
*
*Parameters	
*	pstCaConfig[in]:
*		---m_bCamlock   �൱���Ƿ�������Ӫ���뿨��Թ��ܣ�������viaccess ca
*		---m_bIsDrvTest ������������֤���Դ���ʽ��ͬ��������viaccess/mediaguard/nagra ca
*		---m_bIsPVR     �Ƿ���Ҫ֧�ֶ�·ͬʱ���ţ�������viaccess ca�����Ҫ����viaccess��չ��Ŀ
*                          Ҫ��viaccess�ṩ��Ӧ���Ƿ�Ҫ֧�ֶ�·���ŵ��ں˿�
*		---m_bIsIPPV    �Ƿ����ش����ܣ�FALSE��ʾ���ã�������conax ca
*		---m_emPlatType ����оƬ�ͺţ���������chipset pairing mech�������Ǹ߼���ȫоƬ��������
*			   chipset pairing mech���ܿ��Բ������������
*		---m_emPolicy   �߼���ȫ���ԣ�������conax/nagra ca
*		---m_emSecured  оƬ���ͣ����Բ���
*		---m_dwCaDataStartAddr   �洢CA���ݵ�Flash��ʼ��ַ������NDS CA�Ǵ�E2PROM��ʼ��ַ
*		---m_dwCaPrivateDataStartAddr  �洢�ʼ������ݵ�Flash��ʼ��ַ������NDS CA�Ǵ�E2PROM��ʼ��ַ
*		---m_dwCaOTPAddr   OTP��ʼ��ַ������Nagra ca��һ������ҪOTP��OTP��ֻ�ܶ�������д
*		---m_bIsReverseCard �����������Ƿ���Ĭ�����Ϊ���򿨰壬�����Ŀ��Ҫ�ر�ע��
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
*
*Descriptiont:
*	����֮ǰ�ѳ�������Ŀ�Ͱ���֮ǰ���������������¿�����Ŀ��ϣ�����ڳ�ʼ��CA֮ǰ���ô˺��������ղ�����
*   �����������ݸ�CAģ��
*
**************************************************************************/
INT CSCAConfig(CaConfig_S *pstCaConfig);

/**************************************************************************
*Function Name: CS_CA_Init
*   ��ʼ��CAģ��
*
*Parameters	
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_Init(void);

/**************************************************************************
*Function Name: CS_CA_RegisterMsgCallback
*    CA��Ϣ����ע��ص�����
*
*Parameters	
*	hcbMsg [in]:
*		T_CBMsg���ͻص�����.
*		����dwType ΪCA��Ϣ����(CA_COMMAND��CA_ALARM)
*		����dwParm1ΪCA��Ϣ����(��CA_COMMAND_PPV_REQUEST)
*		����dwParm2Ϊ��Ϣ�������ݽṹ��ĵ�ַ
*
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
*
* ��ע: �˺�����ҪCS_CA_Init��ʼ��֮ǰ����
**************************************************************************/
INT CS_CA_RegisterMsgCallback(T_CBMsg hcbMsg);
/**************************************************************************
*Function Name: CS_CA_GetCoreVersion
*   ��ȡCA�ں˿�汾��Ϣ
*
*Parameters
*   pacVerBuf[out]:ָ��洢CA�ں˰汾��Ϣ�Ŀռ�
*   dwBufSize[in]:����һ����󳤶�ֵ.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetCoreVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name: CS_CA_GetVersion
*   ��ȡCA�汾��Ϣ
*
*Parameters
*   pacVerBuf[out]:ָ��洢CA�汾��Ϣ�Ŀռ�
*   dwBufSize[in]:����һ����󳤶�ֵ.
*		           
*Return Values
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_GetVersion(BYTE* pacVerBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name:  CS_CA_StartSrvPlay
*  ����һ��CA����ͨ��
*
*Parameters
*	pstService [in]:
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_StartSrvPlay(T_CaFtaService* pstService);

/**************************************************************************
*Function Name: CS_CA_StopSrvPlay
*  ֹͣһ��CA����ͨ��
*
*Parameters 
*	pstService [in]:
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_StopSrvPlay(T_CaFtaService* pstService);

/**************************************************************************
*Function Name: CS_CA_NewBatSectionNotify
*  �м���ṩһ���ӿڹ�Ӧע��ĳ��ָ����BouquetId(����:IRDETO CAҪ3622���BouquetId)
*  
*  ֪ͨCAģ��BAT�������:
*  (1)��BAT�汾�����仯;
*  (2)�л�Ƶ��;
*
*Parameters 
*	pcSectionData[in]:
*		BAT���ݻ�������ַ 
*	nLen [in]:
*		BAT���ݳ���
*	cDemux[in]:
*		ͨ����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewBatSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewNitSectionNotify
*  ֪ͨCAģ��NIT�������:
*  (1)��NIT�汾�����仯;
*  (2)�л�Ƶ��;
*
*Parameters 
*	pcSectionData[in]:
*		NIT���ݻ�������ַ 
*	nLen [in]:
*		NIT���ݳ���
*	cDemux[in]:
*		ͨ����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewNitSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewPmtSectionNotify
*  ֪ͨCA��PMT����.
*
*Parameters 
*	pcSectionData[in]:
*		PMT���ݻ�������ַ 
*	nLen [in]:
*		PMT���ݳ���
*	cDemux[in]:
*		ͨ����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewPmtSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewCatSectionNotify
*  ֪ͨCA��CAT����
*
*Parameters 
*	pcSectionData[in]:
*		CAT���ݻ�������ַ 
*     nLen [in]:
*		CAT���ݳ���
*	cDemux[in]:
*		ͨ����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewCatSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_NewSdtSectionNotify
*  ֪ͨCA��SDT����.
*
*Parameters 
*	pcSectionData[in]:
*		SDT���ݻ�������ַ 
*	nLen [in]:
*		SDT���ݳ���
*	cDemux[in]:
*		ͨ����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_NewSdtSectionNotify(BYTE* pcSectionData, INT nLen, BYTE cDemux);

/**************************************************************************
* Function Name: CSCANotifyFrequencyChange
* 	֪ͨCAƵ�㷢���ı�
* Parameters	
*             ��
* Return Values
*			  ��
**************************************************************************/
void CSCANotifyFrequencyChange(void);

/**************************************************************************
*Function Name: CS_CA_SetDebugMode
*  ���ô�ӡģʽ,����Ӧ�õ���
*
*Parameters 
*	dwMode[in]:
*		T_CaTraceFlow�����е�һ�ֻ����
*       ���磺���dwMode = CA_FLOW_SMC�����ӡ�����ܿ���ص���Ϣ
**************************************************************************/
void CS_CA_SetDebugMode(T_CaTraceFlow dwMode);

#ifdef  __cplusplus
}
#endif

#endif //__CS_CAINTERFACE_H__

