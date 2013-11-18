#ifndef CSSHELL_TR069_H
#define CSSHELL_TR069_H

#include "mmcp_typedef.h"
#include "cs_tr069.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief TR069����ö��
@note ����ֻ�Ƕ��˼��ֳ�����TR069 ��׼���ͣ���������������Ŀ������Ŀ���Զ����Լ���TR069���͡�
��Ŀ����TR069����ʱһ��Ҫע��:
1.�Զ���TR069����ֵһ��Ҫ����EM_CSSHELL_TR069_TYPE_USERSTART
*/
typedef enum
{
	EM_CSSHELL_TR069_TYPE_COSHIP = 1,
	EM_CSSHELL_TR069_TYPE_SHTEL,
	EM_CSSHELL_TR069_TYPE_HENAN,
	EM_CSSHELL_TR069_TYPE_OTHER,
	EM_CSSHELL_TR069_TYPE_USERSTART = 0x100
}CSShellTR069Type_E;


/*
*@brief TR069��ʼ������,������һϵ�к���ָ�룬��Щ����ָ����Ӧ�û���shell   ��TR069ʵ��(C����)��Ȼ��ע���shell��
*/
typedef struct _CSShellTR069InitParam
{
	int m_nSize;	///<���ṹ��С��һ������sizeof(CSShellTR069InitParam_S)
	CSShellTR069Type_E m_eType;	//TR069������
	/**
	@brief ǰ������ϵͳ��URL 
	*/
	char *m_pcUrl;
			/**
	@brief ǰ������ϵͳ���û���
	*/
	char *m_pcUserName;
					/**
	@brief ǰ������ϵͳ������
	*/
	char *m_pcPassWord;
					
	/**
	@brief �����й�������ģ��(�ṹ������)
	*/
	CSTR069_Item_S *m_psItem;
	
	/**
	@brief �����й�������ģ�������ݵĸ���
	*/
	int m_nItemLen;

	/**
	@brief ����inform��Ϣ��(�ַ���ָ������)
	*/
	char **m_ppcInform;
	
	/**
	@brief ����inform��Ϣ������Ϣ����
	*/
	int m_nInformNum;

	/**
	@brief inform��Ϣ����DeviceId��Ϣ (�ַ���ָ������)
	*/
	char **m_ppcInformDeviceId;
	
	/**
	@brief inform��Ϣ����DeviceId��Ϣ����
	*/
	int m_nInformDeviceIdNum;

	/**
	@brief Period  inform��Ϣ�� (�ַ���ָ������)
	*/
	char **m_ppcInformPeriod;
	
	/**
	@brief Period  inform��Ϣ����Period��Ϣ����
	*/
	int m_nInformPeriodNum;
	
	/**
	@brief Trape  inform��Ϣ�� (�ַ���ָ������)
	*/
	char **m_ppcInformTrape;

	/**
	@brief Trape  inform��Ϣ����Trape��Ϣ����
	*/
	int m_nInformTrapeNum;
	
     	/**
     	@brief ���ñ����������ģ�͵Ĳ����Ļص�����
     	@param pcParaName �������ƣ�pcParaValue ����ֵ��nParaLen ����ֵ����;unUserData�û�������
     	*/
	int (* m_fnSetPara)(char* pcParaName,char *pcParaValue,int nParaLen,unsigned int unUserData);

     	/**
     	@brief ��ȡ�����������ģ�͵Ĳ����Ļص�����
     	@param pcParaName �������ƣ�pcParaValue ����ֵ��nParaLen ����ֵ����(����/���);unUserData�û�������
     	*/
	int (* m_fnGetPara)(char* pcParaName,char *pcParaValue,int *pnParaLen,unsigned int unUserData);

	/**
	@brief Trape  �û�����
	*/
	unsigned int m_unUserData;
}CSShellTR069InitParam_S;


/**
@brief ����tr069 �����ӿ�
@param psParam tr069 shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellTR069SetParam(CSShellTR069InitParam_S * psParam);


#ifdef __cplusplus
}
#endif

#endif

