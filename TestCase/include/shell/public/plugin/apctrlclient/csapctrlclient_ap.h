/**@defgroup  
@brief 
1����ģ�鶨���ip��ַ��������"192.168.1.1"��ASII���ַ�����ʽ��ʾ��������󳤶�Ϊ16���ֽ�(���һ���ֽ�Ϊ\0)
2����ģ�鶨���ip��ַ��Χ��������"192.168.1.1~255"���ַ�����ʽ��ʾ��������󳤶�Ϊ20(���һ���ֽ�Ϊ\0)
2����ģ��ʹ�õ�MAC��ַ��������{0x00,0x0e,0x09,0x11,0x12,0xe1}���޷���BYTE���鷽ʽ��ʾ������ĳ�����6���ֽ�

@version 2.0.1 2011/11/14 
@{
*/
#ifndef _CS_APCTRL_CLIENT_AP_H_
#define _CS_APCTRL_CLIENT_AP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csapctrlclient.h"

#define CSAPCTRLCAP_SSID_NAME_LEN (32)			///<SSID��󳤶�
#define CSAPCTRLCAP_PASSWORD_LEN (32)			///<��������������󳤶�

/**@brief  AP��������ö�ٶ���*/
enum
{    
	CSAPCTRLCAP_ERROR_BAD_PARAMETER = 128 << 16, 		///<��������
	CSAPCTRLCAP_ERROR_SET_PARAMETER,					///<���ò���	ʧ��
	CSAPCTRLCAP_ERROR_GET_PARAMETER,					///<��ȡ����ʧ��
	CSAPCTRLCAP_ERROR_SSID_NOT_EXIST,					///<SSID������
	CSAPCTRLCAP_ERROR_EXCEED_MAX_NUMBER				///�������֧�ָ���
};

/**@brief  �����ź�Ƶ��ö�ٶ���*/
typedef enum{
	EM_APCTRLCAP_CHANNEL_1_2412M,
	EM_APCTRLCAP_CHANNEL_2_2417M,
	EM_APCTRLCAP_CHANNEL_3_2422M,
	EM_APCTRLCAP_CHANNEL_4_2427M,
	EM_APCTRLCAP_CHANNEL_5_2432M,
	EM_APCTRLCAP_CHANNEL_6_2437M,
	EM_APCTRLCAP_CHANNEL_7_2442M,
	EM_APCTRLCAP_CHANNEL_8_2447M,
	EM_APCTRLCAP_CHANNEL_9_2452M,
	EM_APCTRLCAP_CHANNEL_10_2457M,
	EM_APCTRLCAP_CHANNEL_11_2462M,
	EM_APCTRLCAP_CHANNEL_12_2467M,
	EM_APCTRLCAP_CHANNEL_13_2472M,
	EM_APCTRLCAP_CHANNEL_14_2484M,
	EM_APCTRLCAP_CHANNEL_AUTO
}CSAPCTRLCAPFreqChannel_E; 

/**@brief  WIFI����ģʽö�ٶ���*/
typedef enum{
	EM_APCTRLCAP_WIFI_MODE_11B = 1,			///< ��ģʽ����֧�� 802.11b��׼
	EM_APCTRLCAP_WIFI_MODE_11G,				///< ��ģʽ����֧�� 802.11g��׼
	EM_APCTRLCAP_WIFI_MODE_11N,				///< ��ģʽ����֧�� 802.11n��׼
	EM_APCTRLCAP_WIFI_MODE_11BG,			///< ��ģʽ֧�� 802.11b  &  802.11g��׼
	EM_APCTRLCAP_WIFI_MODE_11BGN 			///< ��ģʽ֧�� 802.11b  &  802.11g & 802.11n��׼
}CSAPCTRLCAPWIFIMode_E; 

/**@brief BG����ģʽö�ٶ���*/
typedef enum{
	EM_APCTRLCAP_BGPROTECT_MODE_AUTO,   			///<�Զ�
	EM_APCTRLCAP_BGPROTECT_MODE_OPEN,				///<��
	EM_APCTRLCAP_BGPROTECT_MODE_CLOSE				///<��
}CSAPCTRLCAPBGProtectMode_E;

/**@brief ������֤��ʽö�ٶ���*/
typedef enum{
	EM_APCTRLCAP_AUTH_OPEN, 				///<����ʽ��ֻ������CSUDISS_ENCRYP_WEP���ܷ�ʽ
	EM_APCTRLCAP_AUTH_SHARE,  			///<����ʽ,ֻ������CSUDISS_ENCRYP_WEP���ܷ�ʽ
	EM_APCTRLCAP_AUTH_AUTO,  			///<����Ӧ,ֻ������CSUDISS_ENCRYP_WEP���ܷ�ʽ
	EM_APCTRLCAP_AUTH_WPAPSK, 			///<WPAPSK��֤��ʽ
	EM_APCTRLCAP_AUTH_WPA2PSK,			///<WPA2PSK��֤��ʽ
	EM_APCTRLCAP_AUTH_WPAPSK_WPA2PSK  ///<WPAPSK & WPA2PSK��֤��ʽ
}CSAPCTRLCAPAuthMode_E; 

/**@brief ���߼��ܷ�ʽö�ٶ���*/
typedef enum{
	EM_APCTRLCAP_ENCRYP_WEP,			///WEP���ܷ�ʽ
	EM_APCTRLCAP_ENCRYP_TKIP,			///TKIP���ܷ�ʽ
	EM_APCTRLCAP_ENCRYP_AES,				///AES���ܷ�ʽ
	EM_APCTRLCAP_ENCRYP_TKIP_AES,		///TKIP&AES��ϼ��ܷ�ʽ
	EM_APCTRLCAP_ENCRYP_NONE 			///������
}CSAPCTRLCAPEncrypType_E; 

/**@brief SSID �ṹ�� ����*/
typedef struct{
	int nIndex;     						///<SSID�������ţ�nIndex=0��SSIDΪ��SSID������ʹ��״̬
	char szSSID[CSAPCTRLCAP_SSID_NAME_LEN];  	///<SSID����
	CSUDI_BOOL bIsEnable;				 ///ʹ�ܱ�־����ʾ��SSID�Ƿ���Ա�����
}CSAPCTRLCAPSSID_S; 

/**@brief SSID ������Ϣ����*/
typedef struct{
	CSAPCTRLCAPWIFIMode_E   eMode;					///<WIFI����ģʽ
	CSAPCTRLCAPAuthMode_E   eAuthMode; 				///<��֤��ʽ�����eEncrypType == EM_APCTRLCAP_NONE,�ó�Ա������
	CSAPCTRLCAPEncrypType_E eEncrypType;				///<AP ���ܷ�ʽ
	char szPassWord[CSAPCTRLCAP_PASSWORD_LEN]; 		//���eEncrypType == EM_APCTRLCAP_NONE,�ó�Ա������
	unsigned int uUpdateTime; 						///<���eEncrypType == EM_APCTRLCAP_NONE,�ó�Ա������
}CSAPCTRLCAPSSIDConfig_S;

/**@brief AP�����������¼��Ϣ*/
typedef struct{
	char aucIp[CSAPCTRLC_IP_LEN]; 	 		///< IP��ַ,
	unsigned char aucMac[CSAPCTRLC_MAC_LEN];  		///< MAC��ַ,
	unsigned int 	uSpeed;  					///< ��������,��λMbps
	CSAPCTRLCAPWIFIMode_E eMode;				///< ����ģʽ
	unsigned	int 	uSignalStrength; 				///< �ź�ǿ��
}CSAPCTRLCAPStationInfo_S;

/**@brief AP�߼�������Ϣ*/
typedef struct{
	CSAPCTRLCAPBGProtectMode_E eBGMode;		///<BG����ģʽ
	int nBeaconInterval;   				///<�ű���20 ~ 999
	int nDivideLimit;  					///<�ָ����256 ~ 2346
	int nTranferLimit;  				///<��������ָ����1 ~ 2347
	int nFreqPower;  					///<���书��0 ~ 100
	CSUDI_BOOL bShortStartCode;  	///<�Ƿ����ö�ǰ����
	CSUDI_BOOL bShortClashSlot;  		///<�Ƿ���������ײ��
	CSUDI_BOOL bTransportBurst;  		///<�Ƿ����ô���ͻ��
	CSUDI_BOOL bDataPacketGrab;  	///<�Ƿ��������ݰ��ۺ�
}CSAPCTRLCAPAdvancedInfo_S;

/**@brief AP������Ϣ*/
typedef struct{
	CSUDI_BOOL bSignalEnable;  		///<	�Ƿ�������źţ�TRUE���������źţ�FALSE�ر������ź�
	CSAPCTRLCAPFreqChannel_E eChannel;	///<Ƶ�������CSAPCTRLCAPFreqChannel_E����
	CSUDI_BOOL bSSIDSeparateEnable;	///<�򿪻�ر�AP����
}CSAPCTRLCAPConfig_S;

/**
@brief �򿪻��߹ر�AP�������ź�

@param[in] pszApName ��Ҫ�ر��źŵ�AP�ӿ���
@param[in] bEnable ȡֵTRUEʱ�������ź�;����ر������ź�
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPSetSignal(const char* szApName,CSUDI_BOOL bEnable);

/**
@brief ����AP�ŵ�

@param[in] pszApName AP�ӿ���
@param[in] enChannel ���õ��ŵ�
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPSetFreqChannel(const char* szApName,CSAPCTRLCAPFreqChannel_E eChannel);

/**
@brief �򿪻��߹ر�AP���룬����SSID����ʱ��ͨ����ͬSSID�����station���ܻ�ͨ

@param[in] pszApName AP�ӿ���
@param[in] bEnable ȡֵTRUE ʱʹ��SSID����;����ر�SSID����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPSetSSIDSeparate(const char* szApName,CSUDI_BOOL bEnable);

/**
@brief ��ȡAP������Ϣ

@param[in] szApName:AP����
@param[out] pstOutConfig:���AP������Ϣ
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPGetConfig(const char* szApName,CSAPCTRLCAPConfig_S* pstOutConfig);

/**
@brief ��ȡAP֧�ֵ����SSID����

@param[in] pszApName AP�ӿ���
@param[out] pnCount �����ȡ�������SSID������ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPGetSSIDCapbility(const char* szApName,int *pnMaxCount);

/**
@brief ��ȡ�û����õ����е�SSID

@param[in] pszApName AP�ӿ���
@param[out] psOutSSIDs��������SSID��Ϣ
@param[in/out] pnCount SSID����,���봫�������С�����ʵ�ʻ�ȡ���ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPGetSSIDs(const char* szApName,CSAPCTRLCAPSSID_S* psOutSsids,int* pnCount);

/**
@brief ����SSID

@param[in] pszApName AP�ӿ���
@param[int] pahOutSsids ����Ҫ���õ�����SSID����Ϣ
@param[int] nCount ��Ҫ���õ�SSID����
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPSetSSIDs(const char* szApName,const CSAPCTRLCAPSSID_S* psOutSsids,int nCount);

/**
@brief ����SSID������Ϣ

@param[in] szSsid :ssid
@param[in] psConfig:ssid������Ϣ  
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPSetSSIDConfig(const char* szSsid,const CSAPCTRLCAPSSIDConfig_S* psConfig);

/**
@brief ��ȡSSID������Ϣ

@param[in] szSsid:ssid
@param[out] psOutConfig ����ssid������Ϣ
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPGetSSIDConfig(const char* szSsid,CSAPCTRLCAPSSIDConfig_S* psOutConfig);

/**
@brief ��ȡ����AP��������Ϣ

@param[in] szSsid:ssid
@param[out] psOutArray:��������������Ϣ
@param[in/out] pnCount:ʵ�ʽ������������,���봫�������С�����ʵ�ʻ�ȡ���ĸ���
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPGetStationsInfo(const char* szSsid,CSAPCTRLCAPStationInfo_S* psOutArray,int* pnCount);

/**
@brief ����ָ��AP�ĸ߼�����

@param[in] pszApName AP�ӿ���
@param[out] psInfo ���ò���ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
*/
CSUDI_Error_Code CSAPCTRLCAPSetAdvancedInfo(const char* szApName,const CSAPCTRLCAPAdvancedInfo_S *psInfo);

/**
@brief ��ȡָ��AP�߼��������û�����

@param[in] pszApName AP�ӿ���
@param[out] psOutInfo ���ڱ����ȡ��Ϣ��ָ��
@return 
- ����ɹ�����CSUDI_SUCCESS��
- ���ʧ�ܷ��ش�����
@note ����û�����û�����ù��򷵻�Ĭ������
*/
CSUDI_Error_Code CSAPCTRLCAPGetAdvancedInfo(const char* szApName,CSAPCTRLCAPAdvancedInfo_S *psOutInfo);

#ifdef __cplusplus
}
#endif

/** @} */

#endif


