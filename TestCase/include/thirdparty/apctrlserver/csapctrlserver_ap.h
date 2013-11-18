/**@defgroup  
@brief 
1����ģ�鶨���ip��ַ��������"192.168.1.1"��ASII���ַ�����ʽ��ʾ��������󳤶�Ϊ16���ֽ�(���һ���ֽ�Ϊ\0)
2����ģ�鶨���ip��ַ��Χ��������"192.168.1.1~255"���ַ�����ʽ��ʾ��������󳤶�Ϊ20(���һ���ֽ�Ϊ\0)
2����ģ��ʹ�õ�MAC��ַ��������{0x00,0x0e,0x09,0x11,0x12,0xe1}���޷���BYTE���鷽ʽ��ʾ������ĳ�����6���ֽ�

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _CS_APCTRL_SERVER_AP_H_
#define _CS_APCTRL_SERVER_AP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csapctrlserver.h"

#define CSAPCTRLSAP_SSID_NAME_LEN (32)			///<SSID��󳤶�
#define CSAPCTRLSAP_PASSWORD_LEN (32)			///<��������������󳤶�

/**@brief  AP��������ö�ٶ���*/
enum
{    
	CSAPCTRLSAP_ERROR_BAD_PARAMETER = 128 << 16, 		///<��������
	CSAPCTRLSAP_ERROR_SET_PARAMETER,					///<���ò���	ʧ��
	CSAPCTRLSAP_ERROR_GET_PARAMETER,					///<��ȡ����ʧ��
	CSAPCTRLSAP_ERROR_SSID_NOT_EXIST,					///<SSID������
	CSAPCTRLSAP_ERROR_EXCEED_MAX_NUMBER				///�������֧�ָ���
};

/**@brief  �����ź�Ƶ��ö�ٶ���*/
typedef enum{
	EM_APCTRLSAP_CHANNEL_1_2412M,
	EM_APCTRLSAP_CHANNEL_2_2417M,
	EM_APCTRLSAP_CHANNEL_3_2422M,
	EM_APCTRLSAP_CHANNEL_4_2427M,
	EM_APCTRLSAP_CHANNEL_5_2432M,
	EM_APCTRLSAP_CHANNEL_6_2437M,
	EM_APCTRLSAP_CHANNEL_7_2442M,
	EM_APCTRLSAP_CHANNEL_8_2447M,
	EM_APCTRLSAP_CHANNEL_9_2452M,
	EM_APCTRLSAP_CHANNEL_10_2457M,
	EM_APCTRLSAP_CHANNEL_11_2462M,
	EM_APCTRLSAP_CHANNEL_12_2467M,
	EM_APCTRLSAP_CHANNEL_13_2472M,
	EM_APCTRLSAP_CHANNEL_14_2484M,
	EM_APCTRLSAP_CHANNEL_AUTO
}CSAPCTRLSAPFreqChannel_E; 

/**@brief  WIFI����ģʽö�ٶ���*/
typedef enum{
	EM_APCTRLSAP_WIFI_MODE_11B = 1,			///< ��ģʽ����֧�� 802.11b��׼
	EM_APCTRLSAP_WIFI_MODE_11G,				///< ��ģʽ����֧�� 802.11g��׼
	EM_APCTRLSAP_WIFI_MODE_11N,				///< ��ģʽ����֧�� 802.11n��׼
	EM_APCTRLSAP_WIFI_MODE_11BG,			///< ��ģʽ֧�� 802.11b  &  802.11g��׼
	EM_APCTRLSAP_WIFI_MODE_11BGN 			///< ��ģʽ֧�� 802.11b  &  802.11g & 802.11n��׼
}CSAPCTRLSAPWIFIMode_E; 

/**@brief BG����ģʽö�ٶ���*/
typedef enum{
	EM_APCTRLSAP_BGPROTECT_MODE_AUTO,   			///<�Զ�
	EM_APCTRLSAP_BGPROTECT_MODE_OPEN,				///<��
	EM_APCTRLSAP_BGPROTECT_MODE_CLOSE				///<��
}CSAPCTRLSAPBGProtectMode_E;

/**@brief ������֤��ʽö�ٶ���*/
typedef enum{
	EM_APCTRLSAP_AUTH_OPEN, 				///<����ʽ��ֻ������CSUDISS_ENCRYP_WEP���ܷ�ʽ
	EM_APCTRLSAP_AUTH_SHARE,  			///<����ʽ,ֻ������CSUDISS_ENCRYP_WEP���ܷ�ʽ
	EM_APCTRLSAP_AUTH_AUTO,  			///<����Ӧ,ֻ������CSUDISS_ENCRYP_WEP���ܷ�ʽ
	EM_APCTRLSAP_AUTH_WPAPSK, 			///<WPAPSK��֤��ʽ
	EM_APCTRLSAP_AUTH_WPA2PSK,			///<WPA2PSK��֤��ʽ
	EM_APCTRLSAP_AUTH_WPAPSK_WPA2PSK,  ///<WPAPSK & WPA2PSK��֤��ʽ
}CSAPCTRLSAPAuthMode_E; 

/**@brief ���߼��ܷ�ʽö�ٶ���*/
typedef enum{
	EM_APCTRLSAP_ENCRYP_WEP,			///WEP���ܷ�ʽ
	EM_APCTRLSAP_ENCRYP_TKIP,			///TKIP���ܷ�ʽ
	EM_APCTRLSAP_ENCRYP_AES,				///AES���ܷ�ʽ
	EM_APCTRLSAP_ENCRYP_TKIP_AES,		///TKIP&AES��ϼ��ܷ�ʽ
	EM_APCTRLSAP_ENCRYP_NONE 			///������
}CSAPCTRLSAPEncrypType_E; 

/**@brief SSID �ṹ�� ����*/
typedef struct{
	int nIndex;     						///<SSID�������ţ�nIndex=0��SSIDΪ��SSID������ʹ��״̬
	char szSSID[CSAPCTRLSAP_SSID_NAME_LEN];  	///<SSID����
	CSUDI_BOOL bIsEnable;				 ///ʹ�ܱ�־����ʾ��SSID�Ƿ���Ա�����
}CSAPCTRLSAPSSID_S; 

/**@brief SSID ������Ϣ����*/
typedef struct{
	CSAPCTRLSAPWIFIMode_E   eMode;					///<WIFI����ģʽ
	CSAPCTRLSAPAuthMode_E   eAuthMode; 				///<��֤��ʽ�����eEncrypType == EM_APCTRLSAP_NONE,�ó�Ա������
	CSAPCTRLSAPEncrypType_E eEncrypType;				///<AP ���ܷ�ʽ
	char szPassWord[CSAPCTRLSAP_PASSWORD_LEN]; 		//���eEncrypType == EM_APCTRLSAP_NONE,�ó�Ա������
	unsigned int uUpdateTime; 						///<���eEncrypType == EM_APCTRLSAP_NONE,�ó�Ա������
}CSAPCTRLSAPSSIDConfig_S;

/**@brief AP�����������¼��Ϣ*/
typedef struct{
	char aucIp[CSAPCTRLS_IP_LEN]; 	 		///< IP��ַ,
	unsigned char aucMac[CSAPCTRLS_MAC_LEN];  		///< MAC��ַ,
	unsigned int 	uSpeed;  					///< ��������,��λMbps
	CSAPCTRLSAPWIFIMode_E eMode;				///< ����ģʽ
	unsigned	int 	uSignalStrength; 				///< �ź�ǿ��
}CSAPCTRLSAPStationInfo_S;

/**@brief AP�߼�������Ϣ*/
typedef struct{
	CSAPCTRLSAPBGProtectMode_E eBGMode;		///<BG����ģʽ
	int nBeaconInterval;   				///<�ű���20 ~ 999
	int nDivideLimit;  					///<�ָ����256 ~ 2346
	int nTranferLimit;  				///<��������ָ����1 ~ 2347
	int nFreqPower;  					///<���书��0 ~ 100
	CSUDI_BOOL bShortStartCode;  	///<�Ƿ����ö�ǰ����
	CSUDI_BOOL bShortClashSlot;  		///<�Ƿ���������ײ��
	CSUDI_BOOL bTransportBurst;  		///<�Ƿ����ô���ͻ��
	CSUDI_BOOL bDataPacketGrab;  	///<�Ƿ��������ݰ��ۺ�
}CSAPCTRLSAPAdvancedInfo_S;

/**@brief AP������Ϣ*/
typedef struct{
	CSUDI_BOOL bSignalEnable;  		///<	�Ƿ�������źţ�TRUE���������źţ�FALSE�ر������ź�
	CSAPCTRLSAPFreqChannel_E eChannel;	///<Ƶ�������CSAPCTRLSAPFreqChannel_E����
	CSUDI_BOOL bSSIDSeparateEnable;	///<�򿪻�ر�AP����
}CSAPCTRLSAPConfig_S;

/**
@brief �򿪻��߹ر�AP�������ź�

@param[in] pszApName ��Ҫ�ر��źŵ�AP�ӿ���
@param[in] bEnable ȡֵTRUEʱ�������ź�;����ر������ź�
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPSetSignal(const char* szApName,CSUDI_BOOL bEnable);

/**
@brief ����AP�ŵ�

@param[in] pszApName AP�ӿ���
@param[in] enChannel ���õ��ŵ�
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPSetFreqChannel(const char* szApName,CSAPCTRLSAPFreqChannel_E eChannel);

/**
@brief �򿪻��߹ر�AP���룬����SSID����ʱ��ͨ����ͬSSID�����station���ܻ�ͨ

@param[in] pszApName AP�ӿ���
@param[in] bEnable ȡֵTRUE ʱʹ��SSID����;����ر�SSID����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPSetSSIDSeparate(const char* szApName,CSUDI_BOOL bEnable);

/**
@brief ��ȡAP������Ϣ

@param[in] szApName:AP����
@param[out] pstOutConfig:���AP������Ϣ
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPGetConfig(const char* szApName,CSAPCTRLSAPConfig_S* pstOutConfig);

/**
@brief ��ȡAP֧�ֵ����SSID����

@param[in] pszApName AP�ӿ���
@param[out] pnCount �����ȡ�������SSID������ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPGetSSIDCapbility(const char* szApName,int *pnMaxCount);

/**
@brief ��ȡ�û����õ����е�SSID

@param[in] pszApName AP�ӿ���
@param[out] psOutSSIDs��������SSID��Ϣ
@param[in/out] pnCount SSID����,���봫�������С�����ʵ�ʻ�ȡ���ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPGetSSIDs(const char* szApName,CSAPCTRLSAPSSID_S* psOutSsids,int* pnCount);

/**
@brief ����SSID

@param[in] pszApName AP�ӿ���
@param[int] pahOutSsids ����Ҫ���õ�����SSID����Ϣ
@param[int] nCount ��Ҫ���õ�SSID����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPSetSSIDs(const char* szApName,const CSAPCTRLSAPSSID_S* psOutSsids,int nCount);

/**
@brief ����SSID������Ϣ

@param[in] szSsid :ssid
@param[in] psConfig:ssid������Ϣ  
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPSetSSIDConfig(const char* szSsid,const CSAPCTRLSAPSSIDConfig_S* psConfig);

/**
@brief ��ȡSSID������Ϣ

@param[in] szSsid:ssid
@param[out] psOutConfig ����ssid������Ϣ
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPGetSSIDConfig(const char* szSsid,CSAPCTRLSAPSSIDConfig_S* psOutConfig);

/**
@brief ��ȡ����AP��������Ϣ

@param[in] szSsid:ssid
@param[out] psOutArray:��������������Ϣ
@param[in/out] pnCount:ʵ�ʽ������������,���봫�������С�����ʵ�ʻ�ȡ���ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPGetStationsInfo(const char* szSsid,CSAPCTRLSAPStationInfo_S* psOutArray,int* pnCount);

/**
@brief ����ָ��AP�ĸ߼�����

@param[in] pszApName AP�ӿ���
@param[in] psInfo ���ò���ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLSAPSetAdvancedInfo(const char* szApName,const CSAPCTRLSAPAdvancedInfo_S *psInfo);

/**
@brief ��ȡָ��AP�߼��������û�����

@param[in] pszApName AP�ӿ���
@param[out] psOutInfo ���ڱ����ȡ��Ϣ��ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
@note ����û�����û�����ù��򷵻�Ĭ������
*/
CSUDI_Error_Code CSAPCTRLSAPGetAdvancedInfo(const char* szApName,CSAPCTRLSAPAdvancedInfo_S *psOutInfo);

#ifdef __cplusplus
}
#endif

/** @} */

#endif


