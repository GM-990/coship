/*****************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*                 All rights reserved.
* Filename:    CS_CAM_Interface.h  
* Author:       Xiejianghua
* Date:          2006-10 
* Description: MediaGuard CA Interface
*            
******************************************************************************/
#include "udiplus_typedef.h"

/*****************************************************************************
* 			                Revision History                                                                          *
******************************************************************************
* - Revision 1.0  2006/10/31
*   Programmer:Xiejianghua
*   Create.

******************************************************************************/
#ifndef _CS_CAM_INTERFACE_H_
#define _CS_CAM_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define	MAX_OPERATOR_NUM         (64)
#define MAX_MAIL_DATA_LENGTH     (256)

typedef enum _CaMessage
{
	CA_ALARM,
	CA_COMMAND,
	CA_STATUS,
	CA_INFO,
	CA_MMI
}T_CaMessage;

typedef enum _CaAlarm
{
	CA_ALARM_PROGRAM_ACCESS_OK,        /*0��Ŀ��������*/
	CA_ALARM_PROGRAM_PREVIEW,          /*1��ĿԤ��*/
	CA_ALARM_PROGRAM_EXPIRED,          /*2��Ŀ��Ȩ����*/
	CA_ALARM_NO_CARD,                  /*3û�����ܿ�*/
	CA_ALARM_ERROR_CARD,               /*4��������ܿ�*/
	CA_ALARM_CARD_OK,                  /*5��ȷ�����ܿ�*/
	CA_ALARM_UNKNOWN_OPERATOR,         /*6δ֪��Ӫ��*/
	CA_ALARM_UNKNOWN_OFFER,            /*7δ֪�����ṩ��*/
	CA_ALARM_GEOGRAPHICAL_BLACKOUT,    /*8�������*/   
	CA_ALARM_RATING_LIMITED,           /*9��ĸ������*/
	CA_ALARM_NO_CREDIT,                /*10����*/
	CA_ALARM_NO_RIGHTS,                /*11 ��Ŀû����ȨdwParm2 ȷ������
												dwParm2 = 0:A/V ��û��Ȩ��
												dwParm2 = 1:��Ƶû��Ȩ��
												dwParm2 = 2:��Ƶû��Ȩ
										*/
	CA_ALARM_NOT_PAIRED,               /*12����δ���*/
	CA_ALARM_FREE_PROGRAME,            /*13����*/
	CA_ALARM_OTHER_CA_ENCRYPT,         /*14��MG CA���Ž�Ŀ*/
	CA_ALARM_MG_CA_ENCRYPT,            /*15MG CA ���Ž�Ŀ*/
	CA_ALARM_PAIRED,                   /*16�������*/	
	CA_ALARM_MAX
}T_CaAlarm;

typedef enum _CaCommand
{
	CA_COMMAND_INHIBIT_STB,
	CA_COMMAND_SHOW_TEXT,
	CA_COMMAND_NEW_EMAIL,				//dwParm2 = (DWORD)&T_CaEmail
	CA_COMMAND_OTA,						//dwparm2 = (DWORD)&T_CAOtaParameter
	CA_COMMAND_RESETPIN,
	CA_COMMAND_MAX
}T_CaCommand;

typedef enum _Castatus
{
	CA_STATUS_SRV_FREE,	              //��ѽ�Ŀ
	CA_STATUS_SRV_OTHER,              //����CA�����Ŀ
	CA_STATUS_CARD_IN,
	CA_STATUS_CARD_OUT,
	CA_STATUS_SRV_RIGHT,             //NVOD�ȷ�����Ȩ��
	CA_STATUS_SRV_NO_RIGHT,          //NVOD�ȷ���ûȨ��
	CA_STATUS_PAIRED,
	CA_STATUS_NOT_PAIRED,
     	CA_STATUS_MAX
}T_CaStatus;

typedef enum _CaInfo
{
	CA_INFO_CA_LIB_VERSION,				//dwparm2 = (DWORD)&T_CaLibVersion
	CA_INFO_CARD_ID,						//dwparm2 = (DWORD)dwCardId
	CA_INFO_RATING,						//dwparm2 = (DWORD)dwRating
	CA_INFO_OPERATOR_INFO,				//dwparm2 = (DWORD)&T_CaOperatorInfoArray
     	CA_INFO_MAX
}T_CaInfo;

typedef enum _CaMmi
{
	CA_MMI_PIN_OK,						//Check Pin ==> PIN OK
	CA_MMI_PIN_ERROR,					//Chcek Pin ==> PIN Error

	CA_MMI_CHANGE_PIN_SUCCESS,			//Change Pin SUCCESS
	CA_MMI_CHANGE_PIN_FAIL,				//Change Pin Fail

	CA_MMI_ENABLE_RATING_SUCCESS,		//Enable Rating check SUCCESS
	CA_MMI_ENABLE_RATING_FAIL,			//Enable Rating check FAIL

	CA_MMI_DISABLE_RATING_SUCCESS,		//Disable Rating check SUCCESS
	CA_MMI_DISABLE_RATING_FAIL,			//Disable Rating check FAIL

	CA_MMI_SET_RATING_SUCCESS,			//Set Rating SUCCESS
	CA_MMI_SET_RATING_FAIL				//Set Rating FAIL

}T_CaMmi;

typedef struct
{
	DWORD	m_dwYear;
	DWORD	m_dwMonth;
	DWORD	m_dwDay;
	DWORD	m_dwHour;
	DWORD	m_dwMinute;
	DWORD	m_dwSecond;
}T_CaDate;

typedef struct _CaLibVersion					/*MediaGuard LIB �汾*/
{
	DWORD	m_dwCoreLibVersionLength;
	DWORD	m_dwPairingLibVersionLength;
	BYTE	m_acCoreLibVersion[15];
	BYTE	m_acPairingLibVersion[15];
}T_CaLibVersion;

typedef struct _CaOperatorInfo
{
	BYTE		m_acOperatorName[16];
	BYTE		m_acOffers[8];
	DWORD		m_dwOperatorId;
	DWORD		m_dwGeographicalId;
	T_CaDate	m_stDate;     					/* �ꡢ�¡�����Ч*/
	DWORD       m_dwDate; //add at 2008-04-03
}T_CaOperatorInfo;

typedef struct _CaOperatorInfoArray			/*������Ӫ����Ϣ*/
{
	DWORD			m_dwOperatorNum;		/*��Ӫ����Ϣ����*/
	T_CaOperatorInfo	m_stOperatorInfo[MAX_OPERATOR_NUM];
}T_CaOperatorInfoArray;

typedef struct _CaOtaParameter		/*OTA ��������*/
{
	BYTE	m_cNewSwVerMajor;
	BYTE	m_cNewSwVerMinor;
	BYTE	m_cModulation;			/*0:undefine;  1:16QAM;  2: 32QAM;  3: 64QAM;  4:128QAM;  5: 256QAM*/
	WORD	m_wDownLoadPid;
	WORD	m_wSymbRate;
	WORD     m_wVersionNum;			/*CDT ��汾��*/
	DWORD	m_dwBackUpFrequency;
	DWORD	m_dwMainFrequency;
}T_CAOtaParameter;

typedef enum _CaEmailSort				/*�ʼ�����ķ�ʽ*/
{
	CA_EMAIL_TIME_DESC,					/*����ʱ��������ں���*/	
	CA_EMAIL_READSTATUS_DESC,			/*�Ѷ������ں���*/
	CA_EMAIL_COMSTATUS_DESC,			/*�Ѷ����Ҵ���ʱ��������ں���*/
	
	CA_EMAIL_SORT_MAX
}T_CaEmailSortStyle;

typedef struct							/*�ʼ�*/
{
	BOOL  			m_bNewEmail;
	BYTE			m_cEmailId;
	BYTE			m_cTitleLen;
	BYTE			m_cSenderLen;
	BYTE			m_cContentLen; 	
	BYTE			m_acEmailData[MAX_MAIL_DATA_LENGTH];	
	/*�ʼ���ʽΪ Title  0 Sender 0 Content �������ַ�����0 ���
	   ��m_cTitleLen = 0;  ��m_acEmailData ΪSender 0 Content      */
	T_CaDate		m_stDate;	
}T_CaEmail;


/**************************************************************************
* Function Name: CS_CA_Restart
*   ��������CA����
*
*Parameters	
*		
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_Restart(void);

/**************************************************************************
* Function Name: CS_CA_ResetAudioPid
*   ����������ƵPID
*
*Parameters	
*		wAudioPid
*			��ƵPID
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_ResetAudioPid(WORD		wAudioPid);


/**************************************************************************
* Function Name: CS_CA_MMI_ChangePin
*   �޸����ܿ�����
*
*Parameters	
*		dwOldPin[in] 
*			������
*       	dwNewPin[in] 
*			������
*Return Values:SUCCESS
*	�����ͨ��CA_MMI_CHANGE_PIN_SUCCESS��CA_MMI_CHANGE_PIN_FAIL�ϱ�
**************************************************************************/
INT CS_CA_MMI_ChangePin(DWORD dwOldPin, DWORD dwNewPin);

/**************************************************************************
* Function Name: CS_CA_MMI_CheckPin
*   У�����ܿ�����
*
*Parameters	
*		dwPinCode[in] 
*			ҪУ�������
*Return Values:SUCCESS
*	�����ͨ��CA_MMI_PIN_OK��CA_MMI_PIN_ERROR�ϱ�
**************************************************************************/
INT CS_CA_MMI_CheckPin(DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestCaLibVersion
*   �����ȡMediaGuard CA��汾
*
*Parameters	
*
*Return Values:SUCCESS
*	���ݻ�ͨ��CA_INFO_CA_LIB_VERSION
**************************************************************************/
INT CS_CA_MMI_RequestCaLibVersion(void);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestPairStatus
*   �����ȡ�������״̬
*
*Parameters	
*
*Return Values:SUCCESS
*	���ݻ�ͨ��CA_INFO_PAIR_STATUS
**************************************************************************/
INT CS_CA_MMI_RequestPairStatus(void);

/**************************************************************************
* Function Name: CS_CA_MMI_EnableRatingLock
*   �򿪸�ĸ������
*
*Parameters	
*		dwPinCode[in] 
*			���ܿ�����
*Return Values:SUCCESS
*	�����ͨ��CA_MMI_ENABLE_RATING_SUCCESS��CA_MMI_ENABLE_RATING_FAIL�ϱ�
**************************************************************************/
INT CS_CA_MMI_EnableRatingLock(DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_MMI_DisableRatingLock
*  �رո�ĸ������
*
*Parameters	
*		dwPinCode[in] 
*			���ܿ�����
*Return Values:SUCCESS
*	�����ͨ��CA_MMI_DISABLE_RATING_SUCCESS��CA_MMI_DISABLE_RATING_FAIL�ϱ�
**************************************************************************/
INT CS_CA_MMI_DisableRatingLock(DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_MMI_SetRating
*  ���ø�ĸ����
*
*Parameters	
*		dwRating[in] 
*			Ҫ���õĸ�ĸ��
*		dwPinCode[in] 
*			���ܿ�����
*Return Values:SUCCESS
*	�����ͨ��CA_MMI_SET_RATING_SUCCESS��CA_MMI_SET_RATING_FAIL�ϱ�
**************************************************************************/
INT CS_CA_MMI_SetRating(DWORD dwRating, DWORD dwPinCode);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestRating
*   �����ȡ��ĸ����
*
*Parameters	
*
*Return Values:SUCCESS
*	���ݻ�ͨ��CA_INFO_RATING
**************************************************************************/
INT CS_CA_MMI_RequestRating(void);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestCardId
*   �����ȡ���ܿ���
*
*Parameters	
*
*Return Values:SUCCESS
*	���ݻ�ͨ��CA_INFO_CARD_ID
**************************************************************************/
INT CS_CA_MMI_RequestCardId(void);

/**************************************************************************
* Function Name: CS_CA_MMI_RequestOperatorInfo
*   �����ȡ��Ӫ����Ϣ
*
*Parameters	
*
*Return Values:SUCCESS
*	���ݻ�ͨ��CA_INFO_OPERATOR_INFO
**************************************************************************/
INT CS_CA_MMI_RequestOperatorInfo(void);

/**************************************************************************
*Function Name:CS_CA_MMI_GetSmcStatus
* ��ȡ���ܿ���ǰ״̬
*
*Parameters 
*	pcSmcStatus[In/Out]:�洢���ܿ�״̬���������ֵΪ0����OUT��Ϊ1��IN
*
*Return Values: 
*	SUCCESS ��ȡ���ܿ���ǰ״̬�ɹ� 
*	FAILURE �����������
**************************************************************************/
INT CS_CA_MMI_GetSmcStatus(BYTE* pcSmcStatus);

/**************************************************************************
* Function Name: CS_CA_SetSortEmailStyle
*   �����ʼ��������ɾ�����ͣ���CA��ʼ��ʱ����
*   ����ȱʡֵ(δͨ���ú�������)Ϊ: ����ʱ��������ں���
*   ������ʱɾ������ȱʡֵ(δͨ���ú�������):ɾ������ʱ������ʼ�
*
*Parameters	
*	eEmailSortStyle[In]:�ʼ�����������
*	eEmailDeleteStyle[In]:ɾ���ʼ�������
*
*Return Values:
*		SUCCESS:�����������ͳɹ�
*		FAILURE:����Ĳ�������ȷ
**************************************************************************/
INT CS_CA_SetSortEmailStyle(T_CaEmailSortStyle eEmailSortStyle, T_CaEmailSortStyle eEmailDeleteStyle);

/**************************************************************************
* Function Name: CS_CA_GetEmailTotalNum
*   ��ȡ�ʼ�����
*
*Parameters	
*
*Return Values:��ǰ�ʼ�����
**************************************************************************/
INT CS_CA_GetEmailTotalNum(void);

/**************************************************************************
* Function Name: CS_CA_GetEmailInfo
*   ��ȡָ������ʼ�����
*
*Parameters	
*		dwIndex[in]
*			�ʼ����(��1��ʼ)
*		pstMsg[out]
*			�ʼ�����
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_GetEmailInfo(DWORD dwIndex/*��1��ʼ*/, T_CaEmail* pstMsg);

/**************************************************************************
* Function Name: CS_CA_SetEmailReadFlag
*   ����ָ���ʼ�Ϊ�Ѷ�
*
*Parameters	
*		dwIndex[in]
*			�ʼ����(��1��ʼ)
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_SetEmailReadFlag(DWORD dwIndex/*��1��ʼ*/);

/**************************************************************************
* Function Name: CS_CA_SetEmailReadFlagEx
*   ��չCS_CA_SetEmailReadFlag ����������EmailId ����
*   ָ���ʼ�Ϊ�Ѷ�
*
*Parameters	
*		cEmailId[in]
*			�ʼ�Id
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_SetEmailReadFlagEx(BYTE  cEmailId);

/**************************************************************************
* Function Name: CS_CA_DelEmail
*   ɾ���ʼ�
*
*Parameters	
*		dwIndex[in]
*			�ʼ����(=0:ɾ�������ʼ���>0:ɾ����Ӧ����ʼ�)
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_DelEmail(DWORD dwIndex/*��1��ʼ��0: ɾ��ȫ��*/);

/**************************************************************************
* Function Name: CS_CA_SaveEmailChange
*   ������ʼ����޸�
*
*Parameters	
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
**************************************************************************/
INT CS_CA_SaveEmailChange(void);

/**************************************************************************
* Function Name: CS_CA_CheckServiceRight
*   ��ȡAPP����Service ��Ȩ��( �軪��������)
*
*Parameters	
*Return Values
**************************************************************************/
BOOL CS_CA_CheckServiceRight(WORD wServiceId, DWORD dwPid);

/**************************************************************************
* Function Name: CS_CA_StartCdtMonitor
*   ��ʼCDT ����( �軪��������)
*
*Parameters	
*		dwPid[in]
*			CDT���Pid
*		wVersionNum[in]
*			CDT��İ汾��*			
*Return Values
*		SUCCESS. 
**************************************************************************/
INT CS_CA_StartCdtMonitor(DWORD dwPid, WORD wVersionNum);/* wVersionNUm = 0xFF ʱ���жϰ汾��*/

/**************************************************************************
* Function Name: CS_CA_StartSmtMonitor
*   ��ʼSMT ����( �軪��������)
*
*Parameters	
*		dwPid[in]
*			SMT���Pid
*		wVersionNum[in]
*			SMT��İ汾��
*Return Values
*		SUCCESS. 
**************************************************************************/
INT CS_CA_StartSmtMonitor(DWORD dwPid, WORD wVersionNum);/* wVersionNUm = 0xFF ʱ���жϰ汾��*/

/**************************************************************************
* Function Name: CS_CA_StopCdtMonitor
* ֹͣCDT ����( �軪��������)
*
*Parameters	

*Return Values
*		SUCCESS. 
**************************************************************************/
INT CS_CA_StopCdtMonitor(void);

/**************************************************************************
* Function Name: CS_CA_StopSmtMonitor
* ֹͣSMT  ����( �軪��������)
*
*Parameters	

*Return Values
*		SUCCESS. 
**************************************************************************/
INT CS_CA_StopSmtMonitor(void);

/**************************************************************************
* Function Name: CS_CA_GetFlashAddress
*   CA��ȡFLASH�ռ��ַ(��APPʵ��)
*
*Parameters	
*Return Values
*		�����CA��FLASH�ռ���ʼ��ַ,��СΪ5K
**************************************************************************/
extern DWORD CS_CA_GetFlashAddress(void);

/**************************************************************************
* Function Name: CS_CA_GetCALibFlashAddress
*   CA��ȡFLASH�ռ��ַ(��APPʵ��)
*
*Parameters	
*Return Values
*		�����CA Lib��FLASH�ռ���ʼ��ַ,��СΪ8K
**************************************************************************/
extern DWORD CS_CA_GetCALibFlashAddress(void);

/**************************************************************************
* Function Name: CS_CA_EmailReadFlash
*   CA ��FLASH (��APPʵ��) ( �軪��������)
*
*Parameters	
*		dwStartAddress[in]
*				FLASH ����ʼ��ַ
*		pcData[in]
*				�������ݵĵ�ַ
*		dwSize[in] 
*				�������ݵĳ���
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
*		
**************************************************************************/
extern INT CS_CA_EmailReadFlash(DWORD dwStartAddress, BYTE* pcData, DWORD dwSize);

/**************************************************************************
* Function Name: CS_CA_EmailWriteFlash
*   CA дFLASH (��APPʵ��) ( �軪��������)
*
*Parameters	
*		dwStartAddress[in]
*				FLASH ����ʼ��ַ
*		pcData[in]
*				д�����ݵĵ�ַ
*		dwSize[in] 
*				д�����ݵĳ���
*Return Values
*		SUCCESS	�ɹ�
*		FAILURE		ʧ��
*
**************************************************************************/
extern INT CS_CA_EmailWriteFlash(DWORD dwStartAddress, BYTE* pcData, DWORD dwSize);

typedef struct _StbInfo
{
	BYTE	m_cManufacturorId;
	BYTE	m_cHardwareId;
	BYTE	m_cSWVerMajor;
	BYTE	m_cSWVerMinor;
	BYTE	m_acSN[8];
	
}T_StbInfo;

/**************************************************************************
* Function Name: CS_CA_GetSTBInfo
*   CA��ȡ�����������Ϣ(��APPʵ��)
*
*Parameters	
*		pStbInfo[out]
*			��������Ϣ
*Return Values
*		SUCCESS. 
**************************************************************************/
extern INT CS_CA_GetSTBInfo(T_StbInfo* pStbInfo);

/**************************************************************************
* Function Name: CS_CA_GetCurrentDateTime
*   CA��ȡ��ǰ����ʱ��(��APPʵ��)
*
*Parameters	
*		pDatetime[out]
*			����ʱ��
*Return Values
*		SUCCESS. 
**************************************************************************/
extern INT CS_CA_GetCurrentDateTime(T_CaDate* pDatetime);

/**************************************************************************
* Function Name: CS_CA_TunerChangeNotify
*   ֪ͨCA Tuner״̬�����ı�
*Parameters	
*		eCaTunerStatus[in]
*			Tuner��״̬�仯
*Return Values
*		FAILURE
*		SUCCESS. 
**************************************************************************/
typedef enum _CaTunerStatus
{
	CA_TUNER_CONNECTED,
	CA_TUNER_DISCONNECTED,
	CA_TUNER_CHANGED,

	CA_TUNER_MAX
}T_CaTunerStatus;

INT CS_CA_TunerChangeNotify(T_CaTunerStatus eCaTunerStatus);


#ifdef  __cplusplus
}
#endif

#endif 

