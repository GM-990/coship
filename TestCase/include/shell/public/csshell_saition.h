#ifndef _CSSHELL_SAITION_H_
#define _CSSHELL_SAITION_H_

#include "mmcp_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define SAITION_BTH_SERVICE_LEN    32
#define SAITION_URL_LEN            1024

/**@defgroup SAITION SAITION��ģ���˦�Ŷ���ӿں����ݽṹ�����˶���
@brief  

@ 1 ���ӿ������ж�����ַ�����ΪASCII�ַ�����


@ 2 ˦��ģ��֧���û���չ�Զ���ʽ�����ݽ�����

@ 2.1�ֻ������Զ���������������ʱ��˦��ģ��ͨ��EM_SAITION_MTOSNOTIFY��Ϣ֪ͨӦ�ã���Ϣ����Ϊ�ַ�������ʽΪ��"{Socket:12,Command:5,Paramlen:123,Param:'xxxx'}" , ���У�
- SocketȡֵΪ��ֵ�ͣ���Ϣͨ��socket�š�\n
- CommandȡֵΪ��ֵ�ͣ��ֻ����͵��û��Զ��������ţ������˦����Ԫ��ӿ��ĵ�.doc����IF4-15: ���ܽӿڡ��ж��塣\n
- ParamlenȡֵΪ��ֵ�ͣ��ֻ����͵��û��Զ�������������ȣ������˦����Ԫ��ӿ��ĵ�.doc����IF4-15: ���ܽӿڡ��ж��塣\n
- ParamȡֵΪ�ַ�����ʽ���ֻ����͵��û��Զ�����������������˦����Ԫ��ӿ��ĵ�.doc����IF4-15: ���ܽӿڡ��ж��塣\n

@ 2.2 ���������ֻ������Զ�������ʱ��ͨ�����ýӿ�m_fnSaitionSendContent�����Զ�������Զ��彻������Ϊjson�ַ�������ʽΪ��\n 
"{Socket:12,Command:5,Paramlen:123,Param:'xxxx'}", ���У�
- SocketȡֵΪ��ֵ�ͣ���������Ŀ���ֻ�����Ϣͨ��socket�ţ�ȡ��EM_SAITION_MTOSNOTIFY��Ϣ�����е�socket��
- CommandȡֵΪ��ֵ�ͣ��跢�͸��ֻ����û��Զ���������룬�����˦����Ԫ��ӿ��ĵ�.doc����IF4-16: ���ܽӿڡ��ж��塣
- ParamlenȡֵΪ��ֵ�ͣ��跢�͸��ֻ����û��Զ�������������ȣ������˦����Ԫ��ӿ��ĵ�.doc����IF4-15: ���ܽӿڡ��ж��塣
- ParamȡֵΪ�ַ�����ʽ���跢�͸��ֻ����û��Զ�����������������˦����Ԫ��ӿ��ĵ�.doc����IF4-16: ���ܽӿڡ��ж��塣

@ 3 ��������Ӧ�÷���EM_SAITION_SETTICKET��Ϣʱ����Ϣ����Ϊjson�ַ���,��ʽΪ��"{TicketID:%d,TicketToken:'%s',ProductID:%d,ContentID:%d,PlayModel:%d,UID:'%s'}", ���У�
- TicketIDȡֵΪ��ֵ�ͣ������˦����Ԫ��ӿ��ĵ�.doc����IF4-1:��������ӿڣ�ticket�����ж��塣
- TicketTokenȡֵΪ�ַ�����ʽ�������˦����Ԫ��ӿ��ĵ�.doc����IF4-1:��������ӿڣ�ticket�����ж��塣
- ProductIDȡֵΪ��ֵ�ͣ������˦����Ԫ��ӿ��ĵ�.doc����IF4-1:��������ӿڣ�ticket�����ж��塣
- ContentIDȡֵΪ��ֵ�ͣ������˦����Ԫ��ӿ��ĵ�.doc����IF4-1:��������ӿڣ�ticket�����ж��塣
- PlayModelȡֵΪΪ��ֵ�ͣ������˦����Ԫ��ӿ��ĵ�.doc����IF4-1:��������ӿڣ�ticket�����ж��塣
- UIDȡֵΪ�ַ�����ʽ�������˦����Ԫ��ӿ��ĵ�.doc����IF4-1:��������ӿڣ�ticket�����ж��塣
@{
*/

/**
@brief ˦��ģ��ʹ�õĴ����豸
*/
typedef enum  
{
    EM_SAITION_TANSDEVTYPE_UNDEF = 0,
    EM_SAITION_TANSDEVTYPE_BLUETOOTH , 
    EM_SAITION_TANSDEVTYPE_ETHERNET , 
    EM_SAITION_TANSDEVTYPE_COM
} CSSaitionTransDevType_E;

/**
@brief ˦����Ϣ�ص�����
*/
typedef enum {
	EM_SAITION_CONNECT, ///<�ֻ��ͻ��������ӳɹ�
	EM_SAITION_DISCONNECT ,	///<�ֻ��ͻ����жϿ�����
	EM_SAITION_SETURL,   ///< 2 �м�������ŵ�URL����Ӧ�ã����͸�Ӧ�õ���Ϣ������һ��CSSaitionPlayURL_S �ṹ�����͵ı���	
	EM_SAITION_SETVOLUME ,     ///< 3 �ֻ�ң�������û����в�������,���͸�Ӧ�õ���Ϣ����������ֵ������Ϊint
	EM_SAITION_SETPROCESS,    ///<4�ֻ����û����в��Ž���,���͸�Ӧ�õ���Ϣ�����ǲ��Ž���ֵ��������int,ֵΪ��ǰ����ռ�ܽ��ȵİٷֱ�
	EM_SAITION_SETTICKET,   ///< 5�м�������ŵ�ticket����Ӧ��,���͸�Ӧ�õ���Ϣ������һ���ַ�������Ӧ����ticket����,���ĵ���ͷע��3
	EM_SAITION_MTOSNOTIFY  ///< 6�Զ�������֪ͨ��MOBILE->STB���ص�����Ϣ�������ֻ����������ַ��������ĵ���ͷע��2.1
}CSSaitionEvent_E;

/**
@brief ˦��ģ�鷵��ֵ
*/
typedef enum  
{
    EM_SAITION_SUCCESS = 0, 
    EM_SAITION_FAILURE,
    EM_SAITION_TIMEOUT,
    EM_SAITION_INVALPARA,
    EM_SAITION_SOCKET_ERROR 
} CSSaitionRet_E;

/**
@brief CSSaitionPlayURL_S  ˦�Ų���url��������Ƭ��URL����ƬURL
*/
typedef struct {
	char m_acPreviewURL[SAITION_URL_LEN]; /*Ƭ��url*/
	char m_acPlayURL[SAITION_URL_LEN];    /*��Ƭurl*/
}CSSaitionPlayURL_S ;

/**
@brief CSSaitionBTHInfo_S ע��sdp����ʱ��Ҫ��һЩ��Ϣ
*/
typedef struct {
	const char m_acSerialID[SAITION_BTH_SERVICE_LEN]; /*���к�, ����˦����˵�������к�����������׼Э���� �����˵�˦����
						   �кţ���ƽ̨�޹�,Ψһ��ʶ˦��, ��ֵΪ��{ 0x03, 0xDA, 0x49, 0x59, 0x8A, 0xBA, 0x43, 
						   0xBA, 0xB3, 0xD4, 0xC2, 0x77, 0x6B, 0xB0, 0xBB, 0x26} */
	const char m_acServiceName[SAITION_BTH_SERVICE_LEN];     /*����������sdp����������޹�*/
	const char m_acServiceDsc[SAITION_BTH_SERVICE_LEN];      /* ��������������sdp����������޹�*/
	const char m_acServiceProv[SAITION_BTH_SERVICE_LEN];     /*�����ṩ��������sdp����������޹�*/
	UINT m_uPort;     /* �������������˿�, �������ڵ��ֻ��ͻ��˰汾����ֵΪ28,�˶˿��ڴ�˦��cfg �Ĺ��������Ǵ�saitionconfig.properties�����ļ��ж�ȡ*/
}CSSaitionBTHInfo_S;

/**
@brief ����CSSaitionAddCallback��CSSaitionRemoveCallback��ʱ��, ��Ҫ��������͵ĺ���ָ��
@param[in] eMsg:��ʾ��ǰ�յ�����Ϣ����
@param[in] dwUserData:���������û�����
@param[in] dwPara: ��ʾ�յ�����Ϣ��������Ӧ�Ĳ������ݵ�ַ
*/
typedef void (*CSOnSaitionEventCallBack_F)(CSSaitionEvent_E eMsg, void *pvPara, DWORD dwUserData);

typedef struct 
{
    int m_nSize;	///<���ṹ��С��һ������sizeof(CSShellSaitionInitParam_S)

    /**
    @brief 	����˦�Ź���ǰ�ĳ�ʼ������
    @param[in]  eDeviceType: ˦��ģ��ʹ�õĴ����豸,���嶨���CSSaitionTransDevType_E
    @param[in] pvParam: ��ʼ��ʱ��Ҫ����Ĳ���������EM_SAITION_TANSDEVTYPE_BLUETOOTH�����豸����
                ��������ΪCSSaitionBTHInfo_S*���ͣ���ʾ����sdpע����Ϣ������ EM_SAITION_TANSDEVTYPE_ETHERNET
                ������ �����˲�������Ϊ*UINT�ͣ���ʾ�����з���˶˿ں�,
    @return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
    @note 
    --����pvParamΪ��ʱ����������CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionInit)(CSSaitionTransDevType_E eDeviceType, void *pvParam);

    /**
    @brief 	����˦�ŷ���, ��CSSaitionInit֮�����
    @param[in] uPort: ˦�ŷ���˶˿ڣ��˶˿��ڴ�˦��cfg �Ĺ��������Ǵ�saitionconfig.properties�����ļ��ж�ȡ
    @return  �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
    @note 
    - �����Ҫ����˦�ŷ������ȵ���CSSaitionInit�������ٵ��ô˺����Ϳ����ˡ�
    */
    CSUDI_Error_Code (* m_fnSaitionStart)();

    /**
    @brief 	ֹͣ˦�ŷ���
    @return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
    @note 
    -
    */
    CSUDI_Error_Code (* m_fnSaitionStop)();

    /**
    @brief 	����˦������URL, ��URL��˦���е�"��"�����й�
    @param[char*] pcBuildUrl: ˦������URL, pcBuildUrl��������'\0'��β��ASCII�����ַ���, ���ַ����м䲻�ܺ���'\0\.
    	   ��pcBuildUrl�ĳ�����С��(1024)�� �����ܽ�˦������URL��ȷ���õ�Cģ������
    @return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
    @note 
    -����pcBuildUrlΪ��ʱ����������CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionSetBuildURL)(const char *pcBuildUrl);

    /**
    @brief 	��ȡ˦������URL
    @param[out] pcBuildUrl:  ��Ż�ȡ��˦������URL����
    @param[in] nUrlLen:  pcBuildUrl���泤��
    @return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
    @note 
    -����pcBuildUrlΪ��ʱ��������CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionGetBuildURL)(char* pcBuildUrl, int nUrlLen);

    /**
    @brief 	����˦������URL, ��URL��˦�ŵ�"˦" �����й�
    @param[in] pcHomeUrl: ˦������URL, pcHomeUrl��������'\0'��β��ASCII�����ַ���, ���ַ����м䲻�ܺ���'\0\.
    	     ��pcHomeUrl�ĳ�����С��(1024)�� �����ܽ�˦������URL��ȷ���õ�Cģ������
    @return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
    @note 
    -����pcHomeUrlΪ��ʱ��������CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionSetHomeURL)(const char *pcHomeUrl);

    /**
    @brief 	��ȡ˦������URL
    @param[out] pcHomeUrl:  ��Ż�ȡ��˦������URL����
    @param[in] nUrlLen:  pcHomeUrl���泤��
    @return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
    @note 
    -����pcHomeurlΪ��ʱ��������CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionGetHomeURL)(char* pcHomeUrl, int nUrlLen);

    /**
    @brief 	�˽ӿ�Ϊ��ʵ�ֻ�����֪ͨ�ֻ�ִ��ĳ�����̡�������ģ����ý�pcContent
    ���õ�˦��Cģ�飬������˫��Э�̾��������ӿ�ֻ����͸��,������ĵ���ͷע��2.2
    @param[char*] pcContent:ҵ�����ݵ��������, pcContent��������'\0'��β��ASCII�����ַ���, ���ַ����м䲻�ܺ���'\0\.
    	 ��pcContent�ĳ�����С��(2*1024)�� �����ܽ�������ȷ���õ�Cģ������,
    @return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
    @note ������˽ӿں󣬻�ͨ���첽�������ݵķ�ʽ��������͸�����ֻ���
    -����pcContentΪ��ʱ��������CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionSendContent)(const char *pcContent);

    /**
    @brief  ��˦��ģ��ע��һ���ص�
    @param[in] fnOnSaitionEventCallBack:  Ҫ��ӵĻص�
    @param[in] dwUserData: �����û�������Ϣ
    @return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE	
    */
    CSUDI_Error_Code (* m_fnSaitionAddListener)(CSOnSaitionEventCallBack_F fnOnSaitionEventCallBack, DWORD dwUserData);

    /**
    @brief  ��˦��ģ���Ƶ�һ��callback
    @param[in] fnOnSaitionEventCallBack:  Ҫɾ���Ļص�
    @param[in] dwUserData: �����û�������Ϣ
    @return   �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
    */
    CSUDI_Error_Code (* m_fnSaitionRemoveListener)(CSOnSaitionEventCallBack_F fnOnSaitionEventCallBack, DWORD dwUserData);
}CSShellSaitionInitParam_S;

/**
@brief ���� ˦�� �ӿڲ���
@param psParam  ˦��  Shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellSaitionSetParam(CSShellSaitionInitParam_S *psParam);

/**
@brief ��ȡ ��ȡ �ӿڲ���
@param ��
@�ɹ�����CSShellSaitionInitParam_S,ʧ�ܷ���NULL
*/
const CSShellSaitionInitParam_S* CSShellSaitionGetParam();

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



