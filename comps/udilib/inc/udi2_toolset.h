/**@defgroup TOOLSET TOOLSET����ƽ̨�޹صĹ��߼��ӿ�
@brief Toolsetģ���Ӧͷ�ļ�<udi2_toolset.h>�������˰������кš�ϵͳ���������Լ�
OTA���ƵȻ������ۺϽӿڡ���������Ҫ�������кŲ��֡������������֡�\n

@par ���кŲ���
���кŲ��ְ�����ȡоƬChipID����Ʒ���к������ӿڡ�\n
 	1��оƬChipIDΪоƬΨһ��ʶ�����ʽ��������оƬ����������ѡоƬȷʵ��֧�ֻ�
 	ȡоƬChipID��������COSHIP�������ȷ��ȷʵ�޷�ʵ����ɷ���
 	CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED��\n
 	2����Ʒ���к�Ϊ����������ǰ��COSHIP�����Ӫ�̸����Ļ����������е�Eastwin���
 	��Ψһ��ʶ�ţ�ÿ̨�����е����кŶ���Ψһ�ġ����������10W̨����Eastwin�����
 	�����У�COSHIP���ṩ10W�����кţ�ÿ̨��������������ʱ����һ�����кţ���ͨ��
 	��ȡ��Ʒ���кŽӿ��ṩ��Eastwin�����\n

@par ������������
�����������ְ����رջ����С������������Լ����������ӿڡ�\n
���ر�ע����Ǵ����ӿڿ������������Ҳ�����Ǽٴ�����Eastwin����ٴ����Ĳ���˳��
��ͬ��\n
 	1������Ǽٴ����������ӿڻ����Ϸ��أ�Eastwin�������״̬�������������Ҫ�˳�
 	�� ��״̬���ٴ���CSUDI_FALSEΪ�������ô����ӿڡ�\n 
 	2�������������������ӿڷ���ʱ�����������Ѿ�������UDI��ʵ�ֻᴦ���ѵĹ��̣�
 	������Eastwin��˵��������̽�����ȫ͸���ģ����������о�������������̴��ڡ���
 	������״̬������״̬�������ڴ����ݣ��豸�����ӵȣ������ֲ��䡣

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.3 2009/09/01 ȡ����ʼ���ӿ�,����OTA�ӿ�
@version 2.0.2 2009/08/25 �ѳ�ʼ���ӿڷֿ���ȫ���ͻ�����ʼ
@version 2.0.1 2009/08/16
@{
*/
#ifndef _UDI2_TOOLSET_H_
#define _UDI2_TOOLSET_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDI_CHIPID_MAX_LENGTH   (32)    ///< ��оƬ��ID����󳤶�
#define CSUDI_SERIAL_MAX_LENGTH   (64)    ///< ϵͳ���кŵ���󳤶�
#define CSUDI_KEY_MAX_LENGTH  	  (512)   ///< У����Կ����󳤶�

/**@brief ���߼�ģ����ش������ֵ��*/
enum
{
	CSUDITOOLSET_ERROR_BAD_PARAMETER = CSUDI_TOOLSET_ERROR_BASE,///< �������
	CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED     				///< ��֧�ֲ���
};

/**@brief ��оƬ��ID��Ϣ��*/
typedef struct 
{
	int 	m_nLength;						///< ��оƬ��ID�ĳ��ȣ��䳤�Ȳ�����CSUDI_CHIPID_MAX_LENGTH
	char    m_szChipID[(CSUDI_CHIPID_MAX_LENGTH+1+3) & (~3)];	///< ��оƬ��ID���������и߼���ȫ����оƬ�У�����Ψһ��ID�š�	
}CSUDICHIPID_S;

/**@brief ���������кź�Keyֵ��

�����к���������ǰ��ͬ�������Ӫ�̸���������Ҫ������STB��
*/
typedef struct 
{
	char m_szSerial[(CSUDI_SERIAL_MAX_LENGTH+1+3) & (~3)];	///< ϵͳ���кű�ʶ��һ̨����ֻ����һ��Ψһ��ʶ������У��
	char m_szPairsKey[(CSUDI_KEY_MAX_LENGTH+1+3) & (~3)];	///< ���к�����룬һ̨����ֻ����һ��Ψһ�ļ��ܣ�����У��
}CSUDISerialKey_S;

/**@brief ��������ö��*/
typedef enum
{
	EM_UDITOOLSET_EXECTYPE_UNKONWN,         ///< �߽�ֵ
	EM_UDITOOLSET_EXECTYPE_APP,             ///< ����Ŀ��ΪӦ��
	EM_UDITOOLSET_EXECTYPE_LOADER           ///< ����Ŀ��ΪLoader
}CSUDIToolsetExecType_E;

/**@brief ������Ϣ�ṹ*/
typedef struct
{
	CSUDIToolsetExecType_E  m_eType;        ///< ��������
	void*                   m_pCodeAddr;    ///< ������������ڴ��׵�ַ��Flash�׵�ַ
	int                     m_nCodeLength;	///< ����������γ���
}CSUDIToolsetExecInfo_S;

/**@brief ���ô�������ö��*/
typedef enum
{
	EM_SYS_STANDBY_ATTR_INFO,         ///< ���û����д�����ǰ�����ʾֵ����Ӧ��pvValue ΪCSUDIPanelLightValue_S ���͵�ָ�룬��δ���ô����ԣ�Ĭ��ǰ��岻�����⴦��
	EM_SYS_STANDBY_ATTR_ALL,          ///< ���û���������ģ�����, ������Χ�豸����WIFI��CM���豸, pvValueΪNULL��
	EM_SYS_STANDBY_ATTR_WIFI,         ///< ���û�����WIFIģ�����, pvValueΪNULL��
	EM_SYS_STANDBY_ATTR_STB           ///< ���û���������ģ�����, pvValueΪNULL��
}CSUDISYSStandbyAttribute_E;

/**
@brief ��ȡ��оƬ��ID��

@param[out] psChipID  ���ڴ�Ż�ȡ����Chip ID���䳤��,��μ�CSUDICHIPID_S��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note оƬ����֧�ֻ�ȡChipId���������оƬ�����CoshipЭ�̴���,Э�̺���ȷ��ȷʵ�޷�ʵ���򷵻�CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED��
*/
CSUDI_Error_Code CSUDIGetChipID(CSUDICHIPID_S * psChipID);


/**
@brief ��ȡĿ����������кź�Key��

@param[out] psSerialKey ���ڴ�Ż�ȡ�������кź�Key�����кź�Key������0��β���ַ�������μ�CSUDISerialKey_S��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIGetSerialKey(CSUDISerialKey_S * psSerialKey);

/**
@brief ����Ŀ����������кź�Key��

@param[in] psSerialKey ���ڴ�����õ����кź�Key����μ�CSUDISerialKey_S��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �ýӿ�Ŀǰ��ʹ��m_szPairsKey ��
*/
CSUDI_Error_Code CSUDISetSerialKey(const CSUDISerialKey_S * psSerialKey);

/**
@brief ������Ŀ�������

@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDISYSReboot(void);

/**
@brief ʵ��ϵͳ�Ĺػ���

@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 
- ִ�гɹ����ú������Բ����ء�
- ����֧�ָù��ܿɷ���CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED��
*/
CSUDI_Error_Code CSUDISYSHalt(void);	

/**
@brief ���ô������ԡ�
@param[in] eAttribute ΪCSUDISYSStandbyAttribute_E ���ͣ���ʾ��Ҫ���õĴ�����������
@param[in] pValue Ϊ����ָ�����ͣ��������������eAttribute ȷ�����ɲο�CSUDISYSStandbyAttribute_E ע��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note
-�����õĴ�������ΪEM_SYS_STANDBY_ATTR_INFO �����ɳ����������Ŀ��������������Ƿ�仯��( ��ʱ��)
*/
CSUDI_Error_Code CSUDISYSSetStandbyAttribute(CSUDISYSStandbyAttribute_E eAttribute, void* pValue);

/**
@brief ʹϵͳ�������״̬�����˳�����״̬��

@param[in] bStandby ΪCSUDI_TRUEʱ��ʾ�������״̬;ΪCSUDI_FALSEʱ��ʾ�˳�����״̬��
@param[in] nStandbyTime ��������ʱ�䣬��λΪ�롣��nStandbyTime����Զ����ѣ�Ϊ0��ʾ���Զ����ѡ����������ʱ��Ч���ٴ���ʱ���Ըò�����
@param[out] pbReal ΪCSUDI_TRUEʱ��ʾ�������;ΪCSUDI_FALSEʱ��ʾ�Ǽٴ���״̬��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 
- ��Ϊ�ٴ�������ýӿ����Ϸ��أ�����*pbReal����ΪCSUDI_FALSE�������������Ҫ�˳�����״̬���ٴ���bStandby=CSUDI_FALSE���ñ��ӿڡ�\n
- ��Ϊ���������ýӿ����˳�����״̬ʱ���أ�����*pbReal����ΪCSUDI_TRUE��������²���Ҫ����bStandby=CSUDI_FALSE���ñ��ӿڣ����˳�����״̬ʱ����״̬�������ڴ����ݣ��豸�����ӵȣ����ֲ��䡣
- ������ٴ����������ȹر�����Ƶ���š�
*/
CSUDI_Error_Code CSUDISYSStandby(CSUDI_BOOL bStandby,int nStandbyTime,CSUDI_BOOL *pbReal);

/**
@brief ���������ӿڣ�Booter��ר�ã�����������ʵ��

��������������õ�ָ��λ�ú�ʹ�øýӿ�ʹ�ó�����ת���˵�ַ��
@param[in] psExecInfo �洢�������������Ϣ
@return �ýӿڵ��ú�ֱ�ӽ��뱻��������������������践�ء�
@note ����������ο������ڴ��У�Ҳ������Flash�ϣ���������Ĳ��죬ѡ��ʵ�ּ��ɡ�
*/
void CSUDIToolsetExec(CSUDIToolsetExecInfo_S *psExecInfo);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif


