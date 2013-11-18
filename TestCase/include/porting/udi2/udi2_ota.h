/**@defgroup OTA OTA �����м����Loader�����ӿ�

@brief OTAģ���Ӧͷ�ļ�udi2_ota.h���������м����Loader�����Ľӿڣ�
�м��ͨ����ģ���ȡOTA��Ϣ��Ӳ���汾�ţ�����ID�ȣ���֪Loader��Ҫ������Ϣ��

@note ���ڱ�ģ����Ӧ�ò㼰BooterLoader�����ϴ󣬽������Ӧ�ò����Booterloader��ʵ��

@note wifi����������������wifi����ģ�����ء�wifi����ģ�����
����wifi����ģ����������Ŀ��ʹ�õ�OTA�������ƣ���ҪOTA��������
�漰���Ľӿ�:
CSUDI_Error_Code CSUDIOTAGetInfo(CSUDIOTAInfoType_E eType, void* value, int length);
CSUDI_Error_Code CSUDIOTASetInfo(CSUDIOTAInfoType_E eType, void* value, int length);
CSUDI_Error_Code CSUDIOTASetOta(const char* pUrl);

wifi����ģ�����
�漰���Ľӿ�:
CSUDI_Error_Code CSUDIOTAWIFIStart(void *pvUserData);
CSUDI_Error_Code CSUDIOTAWIFIAddCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData);
CSUDI_Error_Code CSUDIOTAWIFIDelCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData);
����ֻ�е��ýӿ�CSUDIOTAWIFIStart���м�����ܽ��յ�wifi������ϢCSUDIOTAWIFIMsg_E

@version 2.0.0 2009/11/27 ����
@{
*/

#ifndef _UDI2_OTA_H_
#define _UDI2_OTA_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief OTA�������ֵ*/
enum
{
	CSUDIOTA_ERROR_BAD_PARAMETER = CSUDI_OTA_ERROR_BASE,  ///< �������
	CSUDIOTA_ERROR_FEATURE_NOT_SUPPORTED,				  ///< ��֧�ִ˲���
	CSUDIOTA_ERROR_MEMORY_NOT_ENOUGH                      ///< �ڴ治��
};

/**@brief OTA�����Ϣ����ö��

�����˸���OTA��ʽ���ȡ����Ϣ��������Ŀ�Ĳ�ͬ��ֻ��ʵ��ĳһ�ּ��ɡ�
��CA����������һ��ʵ��NOR�ࣻ��ΪNDSCA����Ҫʵ��NDS�ࡣ
����ʹ������OTA��ʽ����Ŀ��صģ����ڼ����м��ʱ������Ӫ��Ҫ��������ã�
�м����������ô�UDI��ȡ������͵�OTA��Ϣ��
@note ˵����[R]�������ɶ�������ͨ��CSUDIOTAGetInfo�ӿڶ�ȡ����Ϣ;[W]��ʾ�����д������ͨ��CSUDIOTASetInfo�ӿ�д��;[RW]��ʾ��д
@note NDS BCA,FDCA,FLASHHEAD�ṹ���京��ȣ���ο�NDS����ĵ���
*/
typedef enum
{
	EM_UDIOTA_NDS_BEGIN = 2,            ///< []NDS OTA�ϱ߽�ֵ������ʹ��
	EM_UDIOTA_NDS_BCA,		            ///< [R]NDS BCA���ݣ�CSUDI_UINT8*
	EM_UDIOTA_NDS_FDCA,                 ///< [RW]NDS FDCA���ݣ�ʹ�øò�������д����ʱ��������ô洢�Ա��´�����ʱ�����Ƿ����Loader��CSUDI_UINT8*
	EM_UDIOTA_NDS_FLASHHEAD,            ///< [R]NDS FlashHead���ݣ�CSUDI_UINT8*
	EM_UDIOTA_NDS_STANDBY_INFO,         ///< [RW]NDS Loader����״̬λ,Loader���м��ֱ����ͨ���ýӿڽ�����ǰ�Ƿ�Ϊ����״̬,��������NDS������OTA������,CSUDI_BOOL
	EM_UDIOTA_NDS_END = 0x100,          ///< []NDS OTA�±߽�ֵ������ʹ��

	EM_UDIOTA_NOR_BEGIN = 0x101,        ///< []��ͨOTA�ϱ߽�ֵ������ʹ��
	EM_UDIOTA_NOR_MANUFACTURER_CODE,    ///< [R]��ͨOTA�����̴���,int
	EM_UDIOTA_NOR_HARDWARE_CODE,        ///< [R]��ͨOTA��Ӳ���汾��,int
	EM_UDIOTA_NOR_SOFTWARE_VERSION,     ///< [R]��ͨOTA������汾��,int
	EM_UDIOTA_NOR_HARDWARE_BATCHNO,		///< [R]��ͨOTA��Ӳ�����κ�,int
	EM_UDIOTA_NOR_END = 0x200,     		///< []��ͨOTA�±߽�ֵ������ʹ��

	EM_UDIOTA_IRDETO_BEGIN = 0x201,		///< []Irdeto OTA�ϱ߽�ֵ������ʹ��
	EM_UDIOTA_IRDETO_CPCB,				/// [R] Irdeto loader ��CPCB�ṹ�ֶΣ� CSUDI_INT8*
	EM_UDIOTA_IRDETO_BBCB,				/// [R] Irdeto loader ��PBCB�ṹ�ֶΣ� CSUDI_INT8*
	EM_UDIOTA_IRDETO_KEYSTATUS,			/// [R] Irdeto loader ��KEYSTATUS�ṹ�ֶΣ� CSUDI_INT8*
	EM_UDIOTA_IRDETO_END = 0x300		///< []Irdeto OTA�±߽�ֵ������ʹ��
	
}CSUDIOTAInfoType_E;

typedef enum
{
	EM_UDIOTA_WIFI_SOFTWARE_START,  ///< WIFIģ��������ʼ��δ���壬�ݷ���CSUDI_NULL
	EM_UDIOTA_WIFI_SOFTWARE_FINISH, ///< WIFIģ������������δ���壬�ݷ���CSUDI_NULL
	EM_UDIOTA_WIFI_SOFTWARE_FAILED, ///< WIFIģ������ʧ�ܣ�pMsgDataΪCSUDIOTAWIFIErrorType_E����
	EM_UDIOTA_WIFI_CONFIG_START, ///< WIFI���ñ���¿�ʼ��δ���壬�ݷ���CSUDI_NULL
	EM_UDIOTA_WIFI_CONFIG_FINISH, ///< WIFI���ñ���½�����δ���壬�ݷ���CSUDI_NULL
	EM_UDIOTA_WIFI_CONFIG_FAILED, ///< WIFI���ñ����ʧ�ܣ�pMsgDataΪCSUDIOTAWIFIErrorType_E����
	EM_UDIOTA_WIFI_ALL_FINISH, ///< WIFI������ɣ������ɹ���ʧ�ܶ�Ҫ������Ϣ��δ���壬�ݷ���CSUDI_NULL
	EM_UDIOTA_WIFI_MAX ///< WIFI��Ϣ���ͱ߽�ֵ
}CSUDIOTAWIFIMsg_E; 

typedef enum
{
	EM_UDIOTA_WIFI_ERROR_GET_URL_FAIL,  ///<��ȡURLʧ��
	EM_UDIOTA_WIFI_ERROR_URL_ERROR,  ///<URL����
	EM_UDIOTA_WIFI_ERROR_HTTP_OPEN_FAIL,  ///<��http������ʧ��
	EM_UDIOTA_WIFI_ERROR_HTTP_LOAD_FAIL, ///< HTTP���ش���
	EM_UDIOTA_WIFI_ERROR_SHA256_CHECK_FAIL, ///< ��������sha256У��ʧ��
	EM_UDIOTA_WIFI_ERROR_GETVERSION_FAIL, ///< ��ȡwifi�汾ʧ��
	EM_UDIOTA_WIFI_ERROR_VERSION_CHECK_FAIL, ///< wifi�汾���ʧ�ܻ���ͨ���汾��ⲻ��Ҫ����
	EM_UDIOTA_WIFI_ERROR_READ_CONFIG_FAIL,  ///<��ȡwifi�����ļ�ʧ��
	EM_UDIOTA_WIFI_ERROR_SET_RUN_TYPE_FAIL,  ///<����wifi Ϊrun type ʧ��
	EM_UDIOTA_WIFI_ERROR_SET_BACKUP_TYPE_FAIL,  ///<����wifiΪBACKUP  type ʧ��
	EM_UDIOTA_WIFI_ERROR_SET_CONFIG_FAIL,  ///<����wifi�����ļ�ʧ��
	EM_UDIOTA_WIFI_ERROR_CHECK_CONFIG_FAIL,  ///<���wifi����ʧ��
	EM_UDIOTA_WIFI_ERROR_SEND_FAIL, ///< ��wifi��������ʧ��
	EM_UDIOTA_WIFI_ERROR_MAX ///< wifi������Ϣ�߽�ֵ
}CSUDIOTAWIFIErrorType_E; 


/**
@brief WIFI�����ص�����
@param[in] eMsg:���CSUDIOTAWIFIMsg_E˵��
@param[in] pvMsgData:���ݲ�ͬ����Ϣ���ͣ��ص��������Ϣ�����CSUDIOTAWIFIMsg_E˵��
@param[in] pvUserData: �û������Զ���ָ��
@note:
*/
typedef void (*CSUDIOTAWIFICakllback_F)(CSUDIOTAWIFIMsg_E eMsg, void *pvMsgData, void *pvUserData);

/**
@brief ����wifi����
@param[out] pbWifiUpdate: CSUDI_TRUE:��ʾ��Ҫ����wifi������CSUDI_FALSE:��ʾ����Ҫ����wifi����
@return �ɹ�����CSUDI_SUCCESS��ʧ��CSUDI_FAILURE
@note:ֻ�е��ô˽ӿں��м�����ܽ��յ�wifi������ϢCSUDIOTAWIFIMsg_E
@note:ʹ�ó������м����������ô˽ӿڣ�ͨ��pbWifiUpdate�ж�wifi�Ƿ���Ҫ������
     ����Ҫ������������̨���߳�ʵ����wifiģ�齻��,����wifi����������ļ���
     ���ڸ��¹����н����������Ϣͨ��ע��Ļص������׸��м����
*/
CSUDI_Error_Code CSUDIOTAWIFIStart(CSUDI_BOOL *pbWifiUpdate);

/**
@brief ע��wifi�����ص�����
@param[in] fnWifiCallback: �ص�����ָ�룬�����CSUDIOTAWIFICakllback_F
@param[in] pvUserData: �û������Զ���ָ�룬����ΪNULL
@return �ɹ�����CSUDI_SUCCESS��ʧ��CSUDI_FAILURE
@note:
*/
CSUDI_Error_Code CSUDIOTAWIFIAddCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData);

/**
@brief ɾ��wifi�����ص�����
@param[in] fnWifiCallback: �ص�����ָ�룬�����CSUDIOTAWIFICakllback_F
@param[in] pvUserData: �û������Զ���ָ�룬����ΪNULL
@return �ɹ�����CSUDI_SUCCESS��ʧ��CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIOTAWIFIDelCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData);

/**@brief ��ȡOTA��Ϣ�ӿ�

@param[in] eType ��Ϣ���ͣ����CSUDIOTAInfoType_E
@param[out] value ָ��洢��ȡ��Ϣ���ڴ��ַ
@param[in] length valueֵ�ĳ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note MMCP�������Ӫ��ѡ���OTA��ʽ�����ñ��ӿڻ�ȡ�ض����͵�OTA��Ϣ��

@code
//�ο�ʵ��(δ���ǻ�����������쳣)
//ע��:�������������value��ʾ�����ͼ��������в�ͬ���밴�ο�����ʵ��/ʹ�ü��ɡ�
if(eType == EM_UDIOTA_NOR_MANUFACTURER_CODE)
{
	//EM_UDIOTA_NOR_MANUFACTURER_CODE��Ӧ����������int��
	//valueΪһ��ָ��int��ָ�룬�䳤��Ӧ��Ϊsizeof(int)
	//�Ѷ�Ӧֵ�洢��valueָ���buffer��
	*value = (int)100;
}
else if(eType == EM_UDIOTA_NDS_BCA)
{
	//EM_UDIOTA_NDS_BCA��Ӧ����������CSUDI_UINT8����
	//valueһ��ָ��CSUDI_UINT8���飬�䳤��Ϊlength
	//�Ѷ�Ӧ�����ݴ洢��valueָ���buffer��
	memcpy(value,s_aBCAData,sizeof(s_aBCAData));
}
@endcode
*/
CSUDI_Error_Code CSUDIOTAGetInfo(CSUDIOTAInfoType_E eType, void* value, int length);

/**@brief ����OTA��Ϣ�ӿ�

@param[in] eType ��Ϣ���ͣ����CSUDIOTAInfoType_E
@param[in] value ָ��洢��ȡ��Ϣ���ڴ��ַ
@param[in] length valueֵ�ĳ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note MMCP�������Ӫ��ѡ���OTA��ʽ�����ñ��ӿ������ض����͵�OTA��Ϣ������Ϣ��ʱЧ���ɾ���ѡ�������

@code
//�ο�ʵ��(δ���ǻ�����������쳣)
//ע��:�������������value��ʾ�����ͼ��������в�ͬ���밴�ο�����ʵ��/ʹ�ü��ɡ�
if(eType == EM_UDIOTA_NOR_MANUFACTURER_CODE)
{
	//EM_UDIOTA_NOR_MANUFACTURER_CODE��Ӧ����������int��
	//valueΪһ��ָ��int��ָ�룬�䳤��Ӧ��Ϊsizeof(int)
	//��valueָ���buffer�л�ȡ��ֵ
	int manu_code = *(int*)value;
}
else if(eType == EM_UDIOTA_NDS_BCA)
{
	//EM_UDIOTA_NDS_BCA��Ӧ����������CSUDI_UINT8����
	//valueһ��ָ��CSUDI_UINT8���飬�䳤��Ϊlength
	//��valueָ���buffer�л�ȡ��Ӧ����
	memcpy(s_aBCAData,value,length);
}
@endcode
*/
CSUDI_Error_Code CSUDIOTASetInfo(CSUDIOTAInfoType_E eType, void* value, int length);

/**@brief ����OTA��Ϣ.

��ͨOTAģʽ��ͨ���ýӿ�֪ͨLoader��Ҫ����OTA��������Ҫ��Ϣ�洢��֮��MMCP����ʱ�����նˣ��´�����ʱֱ�ӽ���Loader��
@param[in] pUrl ������Ϣ������������λ�ã���Ƶ��,IP��ַ�ȡ�
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note NDS OTAģʽʱ�ýӿ���Ч��MMCP���������NDS���ݽṹ��FDCA�ȵ��޸ģ����ڱ�Ҫʱ������Booter�����NDS����������ʱ�ж���Щ�ṹ
@note Ŀǰ֧�ֵ�URL�У�
- dvb://networkid.tsid.serviceid����dvb://1000.100.10��
- dvbc://qamfreq.SymbolRate.Modulation����dvbc://363000.6875.64
- http://ipurl/����http://www.coship.com/otafile.bin��
- tftp://ipurl/����tftp://ftp.coship.com/otafile.bin��
- wifi://upgrade?ipurl/, ��wifi://upgrade?http://www.downloadWifi.com/wifi.bin?ver=4.1.0.20&upgradeType=1
���������������ݾ�Ϊ10���ơ�
@note URL���滹���ܸ���һЩ��ѡ��������������dvbc://363000.6875.64?pid=100&tableid=254Ŀǰ��֧�ֵĲ����У�
- pid����ʾ������������pid��10������������pid=100��
- tableid����ʾ������������TableId,10������������tableid=254��
- md5,��ʾ�������ݵ�md5У��ֵ,10���ƣ���md5=2b166501e6b8a40911975c54d3c4297b��
- version,��ʾ�����������汾��10���ƣ���version=100��
- type����ʾ�������ͣ��ַ�������type=auto,type=manual��
@note ������Ҫ֧����Щ���͵�URL�������Ŀ��CA����ȷ����
@note pUrlΪCSUDI_NULL��ʾȡ��������
@note �ýӿڽ���������OTA��Ϣ����Ϻ󲢲�ִ����������������������MMCPӦ����ʽ����
*/
CSUDI_Error_Code CSUDIOTASetOta(const char* pUrl);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

