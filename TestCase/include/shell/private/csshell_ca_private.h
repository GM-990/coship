/**@defgroup PublicShellCA PublicShellCA ����ShellCA��������ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_CA_PRIVATE_H
#define CSSHELL_CA_PRIVATE_H

#include "mmcp_typedef.h"
#include "csshell_ca.h"
#include "cs_mediaplayer.h"

#ifdef __cplusplus
extern "C"
{
#endif
typedef struct _CSShellCaInitParam
{  
	int                   m_nSize;    ///< ���ṹ��Ĵ�С��
		CSShellCaType_E         m_eCaType;       ///< CA���ͣ�

	CSShellCaGetSmartCardID_F m_fnGetSmartCardID;   ///< ��ȡ���ܿ����Žӿ�			
}CSShellCaInitParam_S;

/*ca ��״̬�ı�ö��*/
typedef enum _CSShellCaCardChangedEvent
{
	EM_CSSHELL_CARD_VALID = 0,	///<����Ч������Ŀ�����ȷ��ȡ������Ч��
	EM_CSSHELL_CARD_INVALID,	///<����Ч�����γ����������
	EM_CSSHELL_CARD_PREPARING ///<���Ѿ����룬���ǻ�û�л�ȡ�����ţ��������첽��ȡ���ţ�������Ҫʱ��ϳ���CA����irdeto
}CSShellCaCardChangedEvent_E;

	/*���������ÿ��caע��8����ȡ��������Ϣ�Ļص�*/
#define CSSHELL_CA_MAX_CALLBACK_COUNT		(8)
	
typedef enum
{
	EM_CSSHELL_CA_REG_CALLBACK_SUCCESS = 0,	///< ע��ص������ɹ�
	EM_CSSHELL_CA_REG_CALLBACK_REPEAT , ///< �ظ�ע��ص�����������userDataΪ�ؼ����ж�
	EM_CSSHELL_CA_REG_CALLBACK_OVERFLOW, ///< ע��Ļص������Ѿ��ﵽ���������޷�ע��ɹ�		
	EM_CSSHELL_CA_DEL_CALLBACK_SUCCESS,	///< ɾ���ص������ɹ�
	EM_CSSHELL_CA_DEL_CALLBACK_FAILURE	///< ɾ���ص�����ʧ��
}CSShellCaRegCallbackErrorCode_E;


typedef DWORD (*CSShellGetFlashAddr_F)(DWORD dwAddress); ///<CA�����ӿڣ�Ϊtf3��st��������
typedef DWORD (*CSShellGetBakFlashAddr_F)(DWORD dwBakAddress);///<CA�����ӿڣ�Ϊtf3��dvt��������
typedef DWORD (*CSShellGetSoftwareVersion_F)(char* chSWVision, int nsizeSW); ///<CA�����ӿڣ�Ϊst��stcti��������
typedef DWORD (*CSShellGetMailFlashAddress_F)(void);///<CA�����ӿڣ�Ϊst��stcti��������
typedef DWORD (*CSShellGetHardwareVersion_F)(char* chHWVision, int nsizeHW); ///<CA�����ӿڣ�Ϊst��stcti��������

/**
@brief ����ca ���������Ľӿ�
@note ����ֻ�г��˸���ca����Ҫʵ�ֵĹ����ӿڡ���������NDS CA��Ҫʵ�ֶ�TF3 CA����Ҫʵ�ֵ������ӿڣ������csshelL_ndsca.c��ʵ�֣�
Ϊʲô��������? ca���������ӿڣ������ÿ��ca shell�ж�ʵ�֣�������ض�������⡣ֻ����ע�᷽ʽʵ�֡��ǹ��������ӿ��򲻴��ڴ�����
*/
typedef struct _CSShellCaDependence
{  
	CSShellGetFlashAddr_F	m_fnGetFlashAddress;///<flash��ַ
	CSShellGetBakFlashAddr_F m_fnGetBakFlashAddress;///<flash�ı��ݵ�ַ
	CSShellGetSoftwareVersion_F m_fnGetSoftwareVersion;
	CSShellGetHardwareVersion_F m_fnGetHardwareVersion;
	CSShellGetMailFlashAddress_F m_fnGetMailFlashAddress;
}CSShellCaDependence_S;

/**
@brief ���ö�Ӧca�����Ľӿ�
@param psDependence �����ӿ�
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ú����ɾ���ca�������ã�ָ���Լ���Ҫ���õĽӿڡ���C��CA���������Ľӿ�ʱ�����Ե��õ�ָ��ca�������ӿڡ�
	���Ӹýӿڵ�Ŀ�����ڱ������CA�������Ľӿ���Ϊͬ��ͬ�ε��µ��ض���
*/
BOOL CSShellCaSetCommonDependence(CSShellCaDependence_S* psDependence);

/**
@brief ����ca����
@param psParam a����
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ú����ɾ���ca�е� :CSShell***CaSetParam���ã�psParamҲ�����䴫���������������Ӧ�õ��õġ�
	���ù�ϵ: app-->CSShell***CaSetParam-->CSShellCaSetParam
	����caģ��ֻ�����¼psParam��ֵ�������追����ֵ*psParam
*/
BOOL CSShellCaSetParam(CSShellCaInitParam_S* psParam);

/**
@brief ��ȡca����
@param ��
@return ca����
@note �ú�����dtvmx native���á�ca��ʼ��ʱ���ȵ���CSShellCaGetParam��ȡca������Ȼ����ݸò���������type�ж��Ƿ�ΪҪ��ʼ����ca������ǣ����ʼ��
	���ù�ϵ: init***Ca(java)-->CSShellCaGetParam-->if(ca������ȷ)-->��ʼ��***ca
*/	
const CSShellCaInitParam_S* CSShellCaGetParam();

/**
@brief ����ca����
@param Ca����
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ú����ɾ���ca�е� :CSShell***CaSetConfig���ã�psConfigҲ�����䴫�������
	���caģ��ֻ�����¼psConfig��ֵ�������追����ֵ*psConfig
*/
BOOL CSShellCaSetConfig(void* pvConfig);

/**
@brief ��ȡca����
@param ��
@return ca����
@note �ú�����dtvmx native���á�ca��ʼ��ʱ���ɵ���CSShellCaGetConfig��ȡCa����.���ز���Ϊvoid*���ͣ�Ҫת��Ϊ��ȷ��ca����
	���ù�ϵ: init***Ca(java)-->CSShellCaSetConfig-->if(ca������ȷ)-->��ʼ��***ca
*/		
const void* CSShellCaGetConfig();

/**
@brief	���ܿ�״̬�仯�ص�����ԭ�͡�
@param  dwUserData:�û�����
@param  eCardStatus:smartcard��״̬
@param  dwPara1:״̬����1��һ�����ڴ�����
@param   dwPara2:״̬����2��Ԥ��
@return  �ɹ�:0,ʧ��:����
@note �ýӿ������ca�޹ء���ÿ��ca������֧���ڲ�ο�ʱ�����ûص���
*******************************************************************************/

typedef int (*OnCSShellCaCardChanged_F)(DWORD nUserData,CSShellCaCardChangedEvent_E eCardStatus,DWORD dwPara1,DWORD dwPara2);


/******************************************************************************
Function Name:    CSShellCaAddCardChangedCallback
	
Description:
	����smc״̬�仯������
Input:
	dwUserData:�û�����
	fnCallback:�ص�����
Output:
    None

Return Value:
    ���� CSShellCaRegCallbackErrorCode_E
Other:
	�ú���֧�ֶ��ע�ᡣ���ǲ�֧���ظ�ע��(��userDataΪ�ؼ���)��
*******************************************************************************/
CSShellCaRegCallbackErrorCode_E CSShellCaAddCardChangedCallback(DWORD dwUserData,OnCSShellCaCardChanged_F fnCallback);
/******************************************************************************
Function Name:    CSShellCaRemoveCardChangedCallback
	
Description:
	ɾ��smc״̬�仯������
Input:
	dwUserData:�û�����
	fnCallback:�ص�����
Output:
    None

Return Value:
   ���� CSShellCaRegCallbackErrorCode_E
Other
	:
*******************************************************************************/
CSShellCaRegCallbackErrorCode_E CSShellCaRemoveCardChangedCallback(DWORD dwUserData,OnCSShellCaCardChanged_F fnCallback);

/******************************************************************************
Function Name:    CSShellCaGetCardChangedCallback
	
Description:
	����index��ȡע���smc�����仯�ص�
Input:
	nIndex:ָ��������ֵ	
Output:
    pdwUserData:�����Ӧ�����Ļص����û�����
    pfnCallback:�����Ӧ�����Ļص��Ļص�����

Return Value:
    �ɹ�����true��ʧ�ܷ���false
Other:
*******************************************************************************/
void CSShellCaProcessCallback(CSShellCaCardChangedEvent_E eCardStatus, int nPara1, int nPara2);
	
/******************************************************************************
Function Name:    CSShellCaGetCardChangedCallback
	
Description:
	����index��ȡע���smc�����仯�ص�
Input:
	nIndex:ָ��������ֵ	
Output:
    pdwUserData:�����Ӧ�����Ļص����û�����
    pfnCallback:�����Ӧ�����Ļص��Ļص�����

Return Value:
    �ɹ�����true��ʧ�ܷ���false
Other:
*******************************************************************************/
BOOL CSShellCaGetCardChangedCallback(int nIndex, DWORD * pdwUserData, OnCSShellCaCardChanged_F * pfnCallback);

/******************************************************************************
@brief ����ca���ò���
@param pvConfig ca����
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ú����ɾ���ca�е� :CSShell***CaSetConfig���ã�psConfigҲ�����䴫���������������Ӧ�õ��õġ�
	���ù�ϵ: app-->CSShell***CaSetConfig-->CSShellCaSetConfig
	����caģ��ֻ�����¼psConfig��ֵ�������追����ֵ*pvConfig
*/
BOOL CSShellCaSetConfig(void* pvConfig);

/******************************************************************************
@brief ��ȡca��״̬
@param ��
@return ����ca����״̬
@note ϵͳ����ʱ������ca�ṩ�Ļ�ȡ����ʼ״̬�ķ�����һ�¡����ǣ����ṩ�˻�ȡ��״̬�ķ���������ca��ȡ����״̬��֪ͨ����ca�ӿ�CSShellCaProcessCallback
	��¼��ca��״̬��������Ӧ�ÿ��Բ���ע����ca����ѯ����״̬
*******************************************************************************/
CSShellCaCardChangedEvent_E CSShellCaGetCardStatus(void);

/******************************************************************************
@brief ��ȡ��ǰ��mediaplayer���
@param ��
@return Mediaplayer���
*******************************************************************************/
HCSHANDLE CSShellCaGetMPHandle();

/******************************************************************************
@brief �����ܿ������ø���������:SmartCard.serialNumber
@param pcSmcId,���ܿ���
@return �ɹ����÷���TRUE,���򷵻�FALSE
@note ����ʱ����˿����е���Ч�ո񣬽��������ִ���
*******************************************************************************/
BOOL CSShellCaSetSmcIdToEnv(char* pcSmcId, int nMaxLen);


/*******************************************************************************
@brief ��ȡdtvm ca native�رջ������Ƶ��Ȩ������
@param ��
@return ����dtvm ca native�ر�����ƵΪTRUE,����ΪFALSE
@note �ýӿ���Ϊ�˼���������Ŀ֮ǰ��dtvm ca native�п�������Ƶ����ӣ�Ĭ��Ϊ FALSE
@note ������������Ŀʹ�ã���ΪTRUE;������Ŀ���鲻Ҫ���øýӿ�
@note Ŀǰ�����ý�����dtvm nds ca native�е��ã�����ca��Ҫ����
*******************************************************************************/
BOOL CSShellCaGetControlAV();


#ifdef __cplusplus
}
#endif
/** @} */
#endif   



