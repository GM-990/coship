/**@defgroup  ��ģ���UDI2AP����ӿں����ݽṹ�����˶���
@brief UDI2APģ���Ӧ��ͷ�ļ�<udi2_ap.h>,������AP���ò����ӿ�
1����ģ��ʹ�õ�ip��ַ��ʾ��ʽ�ǵ��ip��ַ��������󳤶�Ϊ16���ֽ���:"192.168.1.1" ���ֱ�ʾ��ʽ
2����ģ��ʹ�õ�MAC��ַ������ĳ�����6���ֽ���:{0x00,0x0e,0x09,0x11,0x12,0xe1}

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _UDI2_AP_H_
#define _UDI2_AP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_typedef.h"
#include "udi2_error.h"

#define CSUDI_IP_LEN (16)				///<IP��ַ��󳤶�
#define CSUDI_MAC_LEN (6)				///<MAC��ַ����

#define CSUDIAP_SSID_NAME_LEN (32 + 1)			///<SSID��󳤶�
#define CSUDIAP_PASSWORD_LEN (32)			///<��������������󳤶�

/**@brief  AP��������ö�ٶ���*/
enum
{    
	CSUDIAP_ERROR_BAD_PARAMETER = 128 << 16, 		///<��������
	CSUDIAP_ERROR_SET_PARAMETER,					///<���ò���	ʧ��
	CSUDIAP_ERROR_GET_PARAMETER,					///<��ȡ����ʧ��
	CSUDIAP_ERROR_SSID_NOT_EXIST,					///<SSID������
	CSUDIAP_ERROR_EXCEED_MAX_NUMBER				///�������֧�ָ���
};

/**@brief  �����ź�Ƶ��ö�ٶ���*/
typedef enum{
	EM_AP_CHANNEL_1_2412M,
	EM_AP_CHANNEL_2_2417M,
	EM_AP_CHANNEL_3_2422M,
	EM_AP_CHANNEL_4_2427M,
	EM_AP_CHANNEL_5_2432M,
	EM_AP_CHANNEL_6_2437M,
	EM_AP_CHANNEL_7_2442M,
	EM_AP_CHANNEL_8_2447M,
	EM_AP_CHANNEL_9_2452M,
	EM_AP_CHANNEL_10_2457M,
	EM_AP_CHANNEL_11_2462M,
	EM_AP_CHANNEL_12_2467M,
	EM_AP_CHANNEL_13_2472M,
	EM_AP_CHANNEL_14_2484M,
	EM_AP_CHANNEL_AUTO
}CSUDIAPFreqChannel_E; 

/**@brief  WIFI����ģʽö�ٶ���*/
typedef enum{
	EM_AP_WIFI_MODE_11B = 1,			///< ��ģʽ����֧�� 802.11b��׼
	EM_AP_WIFI_MODE_11G,				///< ��ģʽ����֧�� 802.11g��׼
	EM_AP_WIFI_MODE_11N,				///< ��ģʽ����֧�� 802.11n��׼
	EM_AP_WIFI_MODE_11BG,			///< ��ģʽ֧�� 802.11b  &  802.11g��׼
	EM_AP_WIFI_MODE_11BGN, 			///< ��ģʽ֧�� 802.11b  &  802.11g & 802.11n��׼
	EM_AP_WIFI_MODE_11GN				///< ��ģʽ֧�� 802.11g & 802.11n��׼
}CSUDIAPWIFIMode_E; 

/**@brief BG����ģʽö�ٶ���*/
typedef enum{
	EM_AP_BGPROTECT_MODE_AUTO,   			///<�Զ�
	EM_AP_BGPROTECT_MODE_OPEN,				///<��
	EM_AP_BGPROTECT_MODE_CLOSE				///<��
}CSUDIAPBGProtectMode_E;

/**@brief ������֤��ʽö�ٶ���*/
typedef enum{
	EM_AP_AUTH_OPEN, 				///<����ʽ��ֻ������CSUDISS_ENCRYP_WEP���ܷ�ʽ
	EM_AP_AUTH_SHARE,  			///<����ʽ,ֻ������CSUDISS_ENCRYP_WEP���ܷ�ʽ
	EM_AP_AUTH_AUTO,  			///<����Ӧ,ֻ������CSUDISS_ENCRYP_WEP���ܷ�ʽ
	EM_AP_AUTH_WPAPSK, 			///<WPAPSK��֤��ʽ
	EM_AP_AUTH_WPA2PSK,			///<WPA2PSK��֤��ʽ
	EM_AP_AUTH_WPAPSK_WPA2PSK,  ///<WPAPSK & WPA2PSK��֤��ʽ
}CSUDIAPAuthMode_E; 

/**@brief ���߼��ܷ�ʽö�ٶ���*/
typedef enum{
	EM_AP_ENCRYP_WEP,			///WEP���ܷ�ʽ
	EM_AP_ENCRYP_TKIP,			///TKIP���ܷ�ʽ
	EM_AP_ENCRYP_AES,				///AES���ܷ�ʽ
	EM_AP_ENCRYP_TKIP_AES,		///TKIP&AES��ϼ��ܷ�ʽ
	EM_AP_ENCRYP_NONE 			///������
}CSUDIAPEncrypType_E; 

/**@brief SSID �ṹ�� ����*/
typedef struct{
	int nIndex;     						///<SSID�������ţ�nIndex=0��SSIDΪ��SSID������ʹ��״̬
	char szSSID[CSUDIAP_SSID_NAME_LEN];  	///<SSID����
	CSUDI_BOOL bIsEnable;				 ///ʹ�ܱ�־����ʾ��SSID�Ƿ���Ա�����
	CSUDIAPWIFIMode_E eWifiMode;					///<WIFI����ģʽ
	CSUDIAPAuthMode_E eAuthMode; 				///<��֤��ʽ�����eEncrypType == EM_AP_NONE,�ó�Ա������
	CSUDIAPEncrypType_E eEncrypType;				///<AP ���ܷ�ʽ
	char szPassWord[CSUDIAP_PASSWORD_LEN]; 		//���eEncrypType == EM_AP_NONE,�ó�Ա������
	unsigned int uUpdateTime; 						///<���eEncrypType == EM_AP_NONE,�ó�Ա������
}CSUDIAPSSID_S; 

/**@brief AP�����������¼��Ϣ*/
typedef struct{
	unsigned char aucMac[CSUDI_MAC_LEN];  		///< MAC��ַ
	unsigned int uSpeed;  					///< ��������,��λMbps
	unsigned	int uSignalStrength; 				///< �ź�ǿ��
}CSUDIAPStationInfo_S;

/**@brief AP�߼�������Ϣ*/
typedef struct{
	CSUDIAPBGProtectMode_E eBGMode;		///<BG����ģʽ
	int nBeaconInterval;   				///<�ű���20 ~ 999
	int nDivideLimit;  					///<�ָ����256 ~ 2346
	int nTranferLimit;  				///<��������ָ����1 ~ 2347
	int nFreqPower;  					///<���书��0 ~ 100
	CSUDI_BOOL bShortStartCode;  	///<�Ƿ����ö�ǰ����
	CSUDI_BOOL bShortClashSlot;  		///<�Ƿ���������ײ��
	CSUDI_BOOL bTransportBurst;  		///<�Ƿ����ô���ͻ��
	CSUDI_BOOL bDataPacketGrab;  	///<�Ƿ��������ݰ��ۺ�
}CSUDIAPAdvancedInfo_S;

/**@brief AP������Ϣ*/
typedef struct{
	CSUDI_BOOL bSignalEnable;  		///<	�Ƿ�������źţ�TRUE���������źţ�FALSE�ر������ź�
	CSUDIAPFreqChannel_E eChannel;	///<Ƶ�������CSUDIAPFreqChannel_E����
	CSUDI_BOOL bSSIDSeparateEnable;	///<�򿪻�ر�AP����
}CSUDIAPConfig_S;

/**
@brief �򿪻��߹ر�AP�������ź�

@param[in] pszApName ��Ҫ�ر��źŵ�AP�ӿ���
@param[in] bEnable ȡֵTRUEʱ�������ź�;����ر������ź�
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSUDIAPSetSignal(const char* szApName,CSUDI_BOOL bEnable);

/**
@brief ����AP�ŵ�

@param[in] pszApName AP�ӿ���
@param[in] enChannel ���õ��ŵ�
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSUDIAPSetFreqChannel(const char* szApName,CSUDIAPFreqChannel_E eChannel);

/**
@brief �򿪻��߹ر�AP���룬����SSID����ʱ��ͨ����ͬSSID�����station���ܻ�ͨ

@param[in] pszApName AP�ӿ���
@param[in] bEnable ȡֵTRUE ʱʹ��SSID����;����ر�SSID����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSUDIAPSetSSIDSeparate(const char* szApName,CSUDI_BOOL bEnable);

/**
@brief ��ȡAP������Ϣ

@param[in] szApName:AP����
@param[out] pstOutConfig:���AP������Ϣ
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSUDIAPGetConfig(const char* szApName,CSUDIAPConfig_S* pstOutConfig);

/**
@brief ��ȡAP֧�ֵ����SSID����

@param[in] pszApName AP�ӿ���
@param[out] pnCount �����ȡ�������SSID������ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSUDIAPGetSSIDCapbility(const char* szApName,int *pnMaxCount);

/**
@brief ��ȡ�û����õ����е�SSID

@param[in] pszApName AP�ӿ���
@param[out] psOutSSIDs��������SSID��Ϣ
@param[in/out] pnCount SSID����,���봫�������С�����ʵ�ʻ�ȡ���ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSUDIAPGetSSIDs(const char* szApName,CSUDIAPSSID_S* psOutSsids,int* pnCount);

/**
@brief ����SSID

@param[in] pszApName AP�ӿ���
@param[int] pahOutSsids ����Ҫ���õ�����SSID����Ϣ
@param[int] nCount ��Ҫ���õ�SSID����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSUDIAPSetSSIDs(const char* szApName,const CSUDIAPSSID_S* psSsids,int nCount);

/**
@brief ��ȡ����AP��������Ϣ

@param[in] szSsid:ssid
@param[out] psOutArray:��������������Ϣ
@param[in/out] pnCount:ʵ�ʽ������������,���봫�������С�����ʵ�ʻ�ȡ���ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSUDIAPGetStationsInfo(const char* szSsid,CSUDIAPStationInfo_S* psOutArray,int* pnCount);

/**
@brief ����ָ��AP�ĸ߼�����

@param[in] pszApName AP�ӿ���
@param[out] psInfo ���ò���ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSUDIAPSetAdvancedInfo(const char* szApName,const CSUDIAPAdvancedInfo_S *psInfo);

/**
@brief ��ȡָ��AP�߼��������û�����

@param[in] pszApName AP�ӿ���
@param[out] psOutInfo ���ڱ����ȡ��Ϣ��ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
@note ����û�����û�����ù��򷵻�Ĭ������
*/
CSUDI_Error_Code CSUDIAPGetAdvancedInfo(const char* szApName,CSUDIAPAdvancedInfo_S *psOutInfo);

#ifdef __cplusplus
}
#endif

/** @} */

#endif


