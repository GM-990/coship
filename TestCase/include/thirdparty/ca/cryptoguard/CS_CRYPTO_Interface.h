/****************************************************************************************************************************
* Notice:         Copyright Shenzhen Coship Electronics Co., Ltd., 
*                    All rights reserved.
* Filename:     CS_CRYPT_Interface.h  
* Author:        caoxiaojing
* Date:           June 2012
* Description: Cryptguard CA interface
*            
*****************************************************************************************************************************/
#ifndef _CS_CRYPT_INTERFACE_H_
#define _CS_CRYPT_INTERFACE_H_

#include "udiplus_typedef.h"
#include "cs_cainterface.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum _CAMessage
{
	CA_ALARM,
	CA_COMMAND,
	CA_INFO,
	CA_STATUS
}CAMessage_E;

typedef enum _CAAlarm
{
	CA_ALARM_CANCEL,                

	CA_ALARM_SMC_OUT, 			      
	CA_ALARM_SMC_ERR, 			      

	CA_ALARM_NO_RIGHT, //��Ȩ��
	CA_ALARM_SRV_OTHER,//�������Ž�Ŀ		
	CA_ALARM_CHAL_LOCKED,//��Ŀ������
	CA_ALARM_NO_PAIR,//������ƥ��
	CA_ALARM_BLACK_OUT,//value rules,��ʾ"This service is blocked"
	
	CA_ALARM_MAX	
}CAAlarm_E;

typedef enum  _CACommand
{
	CA_COMMAND_PINCHECK,//Ӧ���յ�����Ϣ����Ҫ����PIN��У�������PIN������CSCACheckPinCode()
	CA_COMMAND_FINGERPRINT,//����Ϣ��������dwParm2 =(DWORD)&CSCAFingerPrint_S
	CA_COMMAND_OSD,//����Ϣ��������dwParm2 =(DWORD)&CAOSDMsg_S
	
	CA_COMMAND_MAX
}CACommand_E;

typedef enum  _CAInfo
{
	CA_INFO_SUBSCRIPTION_STATUS,//Subscription status,dwParm2 =(DWORD)&CAStatus_S
	CA_INFO_PPV_STATUS,//Pay_Per_View status,dwParm2 =(DWORD)&CAStatus_S
	CA_INFO_MAX
}CAInfo_E;

typedef enum _CAStatus
{
	CA_STATUS_SMC_IN, 	
	CA_STATUS_SMC_OUT,    
	CA_STATUS_SMC_OK,
	CA_STATUS_SMC_ERR, 

	CA_STATUS_PIN_RIGHT,//PIN�����
	CA_STATUS_PIN_ERROR,//PIN����ȷ

	CA_STATUS_DSM_OK,//���ųɹ�

	CA_STATUS_MAX
}CAStatus_E;

//�յ���Ҫ�ر�����Ƶ��ALARM��Ϣʱ�ж���Ҫ�رյ�����Ƶ���ͣ���dwParm3����
typedef enum _CAAVStop
{
	CA_STOP_AV,      ///�ر�����Ƶ
	CA_STOP_AUDIO,   ///�ر���Ƶ
	CA_STOP_VIDEO,    ///�ر���Ƶ

	CA_STOP_MAX
}CAAVStop_E;

typedef struct  _CATime
{
	WORD   m_wYear;
	BYTE   m_ucMonth;
	BYTE   m_ucDay;
	BYTE   m_ucHour;
	BYTE   m_ucMinute;
}CATime_S;

typedef struct _CAFingerprint 
{
	BYTE   m_ucFPIRD; ///// 0: No IRD number display  // 1: Fingerprint IRD number display
	BYTE   m_ucFPCardNum;// 0: No cardnumber display // 1: Fingerprint Cardnumber display
	BYTE   m_ucFPRandom;// 0: Fingerprint placement is controlled by X and Y.// 1: Fingerprint placement should be randomized on each display.
	BYTE   m_ucFPRepeat;// 0: No repeat  // ����ֵx: Repeat fingerprint displaying every x minutes
	BYTE   m_ucLocationX; // 0-15: Placement X cordinate    
    	BYTE   m_ucLocationY; // 0-15: Placement Y cordinate
	WORD   m_wDuration; // 0-65535: Duration in ms for display of fingerprint.
	BYTE   m_ucFPColor;// 0: Random fingerprint color. // 1: Fingerprint color Grey.// 2: Fingerprint color Black.// 3: Fingerprint color White.
						// 4: Fingerprint color Blue.// 5: Fingerprint color Green.// 6: Fingerprint color Red.// 7: Fingerprint color Yellow.
}CAFingerPrint_S;

typedef struct _CASMCInfo
{
	DWORD m_dwCardMainNumber;//Card main identification number
	DWORD m_dwCardAltNumber;//Alternative card  identification number
	WORD m_wCASystemID;//CA System ID
}CASMCInfo_S;

#define CA_MAX_OSD_LEN 255
typedef struct _CAOSDMsg
{
	BYTE  m_ucDisplayTime;//��λ��,0ʱһֱ��ʾ
	BYTE  m_aucMsg[CA_MAX_OSD_LEN + 1];//�ַ���
} CAOSDMsg_S;

#define MAX_STATUS_LEN (1024)
typedef struct _CAStatusMsg
{
	WORD  m_wLen;
	BYTE  m_aucStatus[MAX_STATUS_LEN + 1];//�ַ���
} CAStatusMsg_S;//subscription status and ppv view status


#define STB_KEY_LEN 16
typedef struct _CASTBKeyInfo
{
	DWORD m_dwSTBID;
	BYTE m_aucSysGlobalKey[STB_KEY_LEN];
	BYTE m_aucManGlobalKey[STB_KEY_LEN];
	BYTE m_aucStbGroupKey[STB_KEY_LEN];
	BYTE m_aucStbUniqueKey[STB_KEY_LEN];
}CASTBKeyInfo_S;

/**************************************************************************
*Function Name: CSCASMCGetInfo
*
* ��ѯ���ܿ���Ϣ
*
*Parameters:
*	pstSMCInfo[out]:�����ѯ�������ܿ���Ϣ
*   		           
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCASMCGetInfo(CASMCInfo_S* pstSMCInfo);

/**************************************************************************
*Function Name: CSCACheckPinCode
*
* У�����ܿ�PIN��
*
*Parameters:
*	pucPinCode[in]:�����PIN��
*	ucLen[in]:�����PIN��ĳ���		
*
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCACheckPinCode(BYTE *pucPinCode, BYTE ucLen);

/**************************************************************************
*Function Name: CSCAGetProtocolVersion
*
* ��ѯcard  Protocol Version
*
*Parameters:
*	pwPrtlVer[out]:�������ܿ� Protocol Version
*   		           
*Return Values:
*	 0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCAGetProtocolVersion(WORD *pwPrtlVer);

/**************************************************************************
*Function Name: CSCAGetSubscriptionStatus
*
* ��ѯSubscription״̬���յ���ϢCA_INFO_SUBSCRIPTION_STATUS�󽫲����е��ַ�����ʾ�ڽ�����
*
*
*Parameters:
*	void
*   		           
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCAGetSubscriptionStatus(void);

/**************************************************************************
*Function Name: CSCAGetPPVStatus
*
* ��ѯPPV״̬���յ���ϢCA_INFO_PPV_STATUS�󽫲����е��ַ�����ʾ�ڽ�����
*
*
*Parameters:
*	void
*   		           
*Return Values:
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CSCAGetPPVStatus(void);

/**************************************************************************
*Function Name:CSCAGetFlashAddress
*
* ��ȡ����������Flash ��ַ��Ӧ��ʵ��
*
*Parameters: 
*	 dwSize[In]: �����������Ĵ�С
*
*Return Values: 
*	һ�������������ʼ��ַ
**************************************************************************/
extern DWORD CSCAGetFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetBackupFlashAddress
*
*  ��ȡ������������Flash ��ַ(����������д��һ��������ͬһ��Block)��Ӧ��ʵ��
*
*Parameters: 
*	 dwSize[In]: �����������Ĵ�С
*
*Return Values: 
*	һ�������������ʼ��ַ
**************************************************************************/
extern DWORD CSCAGetBackupFlashAddress(DWORD dwSize);

/**************************************************************************
*Function Name:CSCAGetStbKeyInfo
*
* ��ȡ��ǰ�����е�ID���ĸ�Key��Ϣ,ÿ��������Ψһ,Ӧ��ʵ��
*
*Parameters: 
*	psSTBKeyInfo [out]:�����������Ϣ�Ľṹ���ַ
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
extern INT CSCAGetStbKeyInfo(CASTBKeyInfo_S* psSTBKeyInfo);

#ifdef  __cplusplus
}
#endif
#endif //_CS_CRYPT_INTERFACE_H_

