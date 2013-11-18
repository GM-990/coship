/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_IRDETO_Interface.h  
* Author:      Xiejianghua
* Date:        Mar 2006
* Description: IRDETO CA interface
*            
*****************************************************************************************************************************/
#include "mmcp_typedef.h"
#include "CS_IRDETOCA_Loader.h"

/****************************************************************************************************************************
* 			                Revision History                                                                                *
*****************************************************************************************************************************
* - Revision  : 1.0  2006/3/22
*   Programmer: Xie jianghua
*   Create
* - Revision  : 1.1  2007/4/12
*   Modify by : Wan suanlin
* - Revision  : 1.2  2007/12/12  
*   Modify by : Pengshungquan
* - Revision  : 1.3  2008/04/21  
*   Modify by : Pengshungquan
*****************************************************************************************************************************/
#ifndef _CS_IRDETO_INTERFACE_H_
#define _CS_IRDETO_INTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

//~ MESSAGE
typedef enum _CaEventGroup
{
	CA_ALARM,
	CA_INFO,
	CA_COMMAND,
	CA_STATUS
}T_CaEventGroup;

typedef enum _CaStatus
{
	CA_STATUS_OTA_OK,                   //dwParm2 = (DWORD)&T_OtaCfg
	CA_STATUS_OTA_STATUS,              	//dwParm2 = (DWORD)&T_OtaStatus
	CA_STATUS_OTA_FAILURE               //dwParm2 = (DWORD)&E_OtaError
}T_CaStatus;

typedef enum _CaInfo
{
	CA_INFO_CA_STATUS,                 	//dwParm2 = (DWORD)&T_CaCaStatus
	CA_INFO_IRD_STATUS,                	//dwParm2 = (DWORD)&T_CaIrdStatus
	CA_INFO_LOADER_STATUS,             	//dwParm2 = (DWORD)&T_CaLoaderStatus
	CA_INFO_MAIL,                  		//dwParm2 = (DWORD)&T_CaMsg
	CA_INFO_TMS                         //dwParm2 = (DWORD)&T_CaTms
}T_CaInfo;

typedef enum _CaCommand
{
	CA_COMMAND_FINGER_PRINT,              //dwParm2 = (DWORD)&T_CaFingerPrint
	CA_COMMAND_HOMECHANNEL_SET,           //dwParm2 = (DWORD)&T_DeliverySystem
	CA_COMMAND_HOMECHANNEL_SCAN,          //dwParm2 = (DWORD)&T_DeliverySystem(Seacher CA download home channel)
	CA_COMMAND_APP_HOMECHANNEL_SCAN,      //dwParm2 = 0(Seacher App default home channel)
	CA_COMMAND_RESET_CHANNEL,             //dwParm2 = (DWORD)BouquetID
	CA_COMMAND_RECONNECT_CURRENT_SERVICE, //dwParm2 = 0
	CA_COMMAND_REBOOT,                    //dwParm2 = 0
	CA_COMMAND_TUNE_SERVICE,              //dwParm2 = (DWORD)&T_TuneToSvc
	CA_COMMAND_CHANGE_PGPIN,              //dwParm2 = (DWORD)&T_CaPin
	CA_COMMAND_CHANGE_INSTALLPIN,         //dwParm2 = (DWORD)&T_CaPin
	CA_COMMAND_EMM_OTA,                   //dwParm2 = (DWORD)&T_CaEmmOta
	CA_COMMAND_OTA_NEXT,                  //dwParm2 = (DWORD)&T_OtaCfg
	CA_COMMAND_OTA_DETECT                 //dwParm2 = TRUE/FALSE
}T_CaCommand;

typedef enum _CaAlarm
{
	 CA_ALARM_ERROR_E00  = 0,   /* Free To Air */
	 CA_ALARM_ERROR_E04  = 4,   /* No Smart Card */    
	 CA_ALARM_ERROR_E05  = 5,   /* Unknown Smart Card */
	 //
	 CA_ALARM_ERROR_E06  = 6,   /* Smart Card failure */
	 CA_ALARM_ERROR_E07  = 7,   /* Checking Smart Card */
	 CA_ALARM_ERROR_E08  = 8,   /* Smart Card failure */
	 CA_ALARM_ERROR_E09  = 9,   /* CA Module failure */
	 CA_ALARM_ERROR_E10  = 10,  /* PMT format error */
	 //
	 CA_ALARM_ERROR_E11  = 11,  /* Updating CAM EEPROM */
	 CA_ALARM_ERROR_E12  = 12,  /* CA Module failure */
	 CA_ALARM_ERROR_E13  = 13,  /* CA Module failure */
	 CA_ALARM_ERROR_E14  = 14,  /* No ECM */
	 CA_ALARM_ERROR_E15  = 15,  /* Currently Scrambled */
	 //
	 CA_ALARM_ERROR_E16  = 16,  /* Currently scrambled */
	 CA_ALARM_ERROR_E17  = 17,  /* Currently scrambled */
	 CA_ALARM_ERROR_E18  = 18,  /* Currently scrambled */
	 CA_ALARM_ERROR_E19  = 19,  /* Currently scrambled */
	 CA_ALARM_ERROR_E20  = 20,  /* Currently scrambled */
	 //
	 CA_ALARM_ERROR_E21  = 21,  /* Currently scrambled */
	 CA_ALARM_ERROR_E22  = 22,  /* Event Purchased */
	 CA_ALARM_ERROR_E23  = 23,  /* Preview Period */ 
	 CA_ALARM_ERROR_E24  = 24,  /* Scrambled IPPVevent */
	 CA_ALARM_ERROR_E25  = 25,  /* Already purchased */
	 //
	 CA_ALARM_ERROR_E26  = 26,  /* Event Recorded */ 
	 CA_ALARM_ERROR_E27  = 27,  /* Purchase denied */
	 CA_ALARM_ERROR_E28  = 28,  /* Credit limit */
	 CA_ALARM_ERROR_E29  = 29,  /* Descrambled */
	 CA_ALARM_ERROR_E30  = 30,  /* Currently Scrambled */
	 //
	 CA_ALARM_ERROR_E31  = 31,  /* Currently Scrambled */
	 CA_ALARM_ERROR_E32  = 32,  /* Currently Scrambled */
	 CA_ALARM_ERROR_E33  = 33,  /* Invalid Smartcard */
	 CA_ALARM_ERROR_E34  = 34,  /* Currently Scrambled */
	 CA_ALARM_ERROR_E35  = 35,  /* Currently Scrambled */
	 //
	 CA_ALARM_ERROR_E36  = 36,  /* Smart Card Not Compatible */ 
	 CA_ALARM_ERROR_E37  = 37,  /* Service Unknown */
	 CA_ALARM_ERROR_E38  = 38,  /* Service is not running */
	 CA_ALARM_ERROR_E39  = 39,  /* Locating Service */
	 CA_ALARM_ERROR_E40  = 40,  /* Decoder Memory Full */
	 //
	 CA_ALARM_ERROR_E41  = 41,  /* Service is not currently available */
	 CA_ALARM_ERROR_E42  = 42,  /* Parental Control Lock */
	 CA_ALARM_ERROR_E43  = 43,  /* Not allowed in this country */ 
	 CA_ALARM_ERROR_E44  = 44,  /* No Event Information */
	 CA_ALARM_ERROR_E45  = 45,  /* Service not allowed */
	 //
	 CA_ALARM_ERROR_E46  = 46,  /* Reading Satellite Information */ 
	 CA_ALARM_ERROR_E47  = 47,  /* Bouquet Block */
	 CA_ALARM_ERROR_E48  = 48,  /* No Signal */
	 CA_ALARM_ERROR_E49  = 49,  /* LNB Overload */
	 CA_ALARM_ERROR_E50  = 50,  /* No Services available */
	 //
	 CA_ALARM_ERROR_E52  = 52,  /* Searching for signal */
	 CA_ALARM_ERROR_E53  = 53,  /* Incorrect Pin */ 
	 CA_ALARM_ERROR_E54  = 54,  /* IPPV OK */
	 //
	 CA_ALARM_ERROR_E55  = 55,  /* Please stay tuned */
	 //
	 CA_ALARM_ERROR_E56  = 56,  /* CAM not compatible */ 
	 CA_ALARM_ERROR_E57  = 57,  /* Unknown Channel */
	 CA_ALARM_ERROR_E58  = 58,  /* channel not currently available */
	 //
	 CA_ALARM_ERROR_E64  = 64,  /* Invalid tuning parameters */
	 CA_ALARM_ERROR_E66  = 66,  /* Me Block*/
	 CA_ALARM_ERROR_E70  = 70,  /* Tune to valid service*/
	 //
	 CA_ALARM_ERROR_E80  = 80,  /*E80 Not receive the de-tweak keys*/
	 CA_ALARM_ERROR_E81  = 81,  /*E81 Not have the correct key to the decrypt the received de-tweak keys*/
	 //
	 CA_ALARM_ERROR_E82  = 82,  /*E82 No CA Regional Info*/
	 CA_ALARM_ERROR_E83  = 83,  /*E83 Regional Blocking*/

	 //USER DEFINED
	 CA_ALARM_ERROR_E129 = 129, /*Sign OK <-->E48*/
	 CA_ALARM_ERROR_E130 = 130, /*Service <--->E38*/
	 CA_ALARM_ERROR_E131 = 131, /*NO EIT <--->E42*/
	 CA_ALARM_ERROR_E132 = 132, /*COUNTRY<--->E43*/
	 CA_ALARM_ERROR_E133 = 133, /*FTA is not authorised*/
	 CA_ALARM_ERROR_E134 = 134, /*FTA Limited*/
	 CA_ALARM_ERROR_E135 = 135, /*FTA Limited--->E134,clr password*/

	 CA_ALARM_ERROR_E136 = 136, /*Irdeto CA*/
	 CA_ALARM_ERROR_E137 = 137, /*smc ok*/
	 CA_ALARM_ERROR_E255 = 255, /*Clear all message*/
	 //
	 CA_ALARM_ERROR_MAX
 }T_CaAlarm;

//FINGER PRINT
#define CA_MAX_SIZE_FINGERPRINT      (100)
typedef struct _CaFingerPrin  //FINGER PRINT
{
	BYTE    	m_cInfoNo;     	//������Ϣ��
	INT     	m_nOvertime;  	//��ʱ����
	BYTE    	m_acMsg[CA_MAX_SIZE_FINGERPRINT];
}T_CaFingerPrint;

//TUNE
typedef struct  _TuneToSvc
{
	WORD    m_wNetworkId;
	WORD    m_wTranStreamId;
	WORD    m_wServiceId;
}T_TuneToSvc;

//PIN
typedef struct  _CaPin
{
	BYTE*   m_pcPin;
	BYTE    m_cPinLen;
}T_CaPin;

//Emm ota struct
typedef struct _CaEmmOta
{
	BOOL    m_bDownLoadAllowed;
	BOOL    m_bForced;
}T_CaEmmOta;

//CAStatus
#define CA_MAX_SIZE_SC_VER              (32)
#define CA_MAX_SIZE_CAM_VER             (64)
#define CA_MAX_SIZE_SC_SERIALNUMBER     (32)
#define CA_MAX_SIZE_SC_NAT              (4)
#define CA_MAX_SIZE_SC_TYPE             (8)
#define CA_MAX_SIZE_SERVICEINFO         (64)
#define CA_MAX_SIZE_EMM                 (64)
#define CA_MAX_SIZE_ECM                 (64)
#define CA_MAX_SIZE_SC_STATE            (8)
#define CA_MAX_SIZE_EMM_PID             (16)
#define CA_MAX_SIZE_SRV_HANDLE          (16)
#define CA_MAX_SIZE_GLOBAL_STATE        (16)
#define CA_MAX_SIZE_STREAM_STATE        (16)

typedef struct  _CaCaStatus
{
	BYTE    m_szScVer[CA_MAX_SIZE_SC_VER];
	BYTE    m_szScSerialNumber[CA_MAX_SIZE_SC_SERIALNUMBER];
	BYTE    m_szScNat[CA_MAX_SIZE_SC_NAT];
	BYTE    m_szScType[CA_MAX_SIZE_SC_TYPE];
	BYTE    m_szScState[CA_MAX_SIZE_SC_STATE];
	BYTE    m_szCamVer[CA_MAX_SIZE_CAM_VER];
	DWORD   m_dwSrvErrNo;
	BYTE    m_szEmmPid[CA_MAX_SIZE_EMM_PID];		
	BYTE    m_szServiceHandle[CA_MAX_SIZE_SRV_HANDLE];
	BYTE    m_szGloableState[CA_MAX_SIZE_GLOBAL_STATE];
	BYTE    m_szStreamState[CA_MAX_SIZE_STREAM_STATE];
	BYTE    m_szEMM[CA_MAX_SIZE_EMM];
	BYTE    m_szECM[CA_MAX_SIZE_ECM];
}T_CaCaStatus;

typedef enum _TxtType
{
	CA_TXT_MAIL         	= 0,//Text -- Mailbox,	�ʼ���Ϣ
	CA_TXT_ANNOUNCE     	= 1,//Text -- Announcement,�㲥֪ͨ
	CA_TXT_CLUB         	= 2 //Club number,��Ա��
}T_TxtType;

typedef enum _TxtShowModel
{
	CA_TXT_SHOW_NORMAL		= 0x00,		//Normal
	CA_TXT_SHOW_TIMED			= 0x01,		//Timed(automatic erase after date/time)
	CA_TXT_SHOW_FORCED		= 0x02		//Forced Display
}T_TxtShowModel;

// MAIL 
#define CA_MAX_SIZE_TXT_MAIL       (256)
typedef struct _CaDateTime
{
	DWORD   m_dwYear;
	BYTE    m_cMonty;
	BYTE    m_cDay;
	BYTE    m_cHour;
	BYTE    m_cMinute;
}T_CaDateTime;

typedef struct _CaMsg		// �ʼ�����
{
	BYTE    	m_cType;     			//T_TxtType// 0: Text_Mail  1: Text_Announcement 
	BYTE    	m_cClass;    			//T_TxtShowModel// 0: Normal     1: Time              2: ForcedDisplay
	BYTE    	m_cPriority;
	DWORD   	m_dwLen;
	T_CaDateTime  m_stDeleteTime; 	//��TIMED�ʼ��д���
	BYTE    m_szContent[CA_MAX_SIZE_TXT_MAIL];
}T_CaMsg;

//TMS
typedef enum _TMSTYPE
{
	CA_TMS_NOMAL,
	CA_TMS_FORCE_TEXT,
	CA_TMS_FINGERPRINT,
	CA_TMS_MAX
}TMS_TYPE_E;

typedef struct _CaTms
{
	TMS_TYPE_E   m_enType;
	BOOL   m_bFlash;
	BOOL   m_bBanner;
	WORD   m_wDisplayMethod;//���渲��ֵ(20-100)
	WORD   m_wDuration;
	DWORD  m_dwLen;
	BYTE   m_acData[CA_MAX_SIZE_TXT_MAIL];
}T_CaTms;

typedef struct _CaMail		//�ʼ��ṹ��
{	
	BOOL    m_bReadFlag;          	//�Ѷ���־
	BOOL    m_bLockFlag;	      	 //�ʼ�������־
	T_CaDateTime  m_stArriveTime; //�ʼ������ʱ��
	T_CaMsg m_stMsg;
}T_CaMail;

typedef struct _OtaStatus  //OTA����״̬��Ϣ
{
	BYTE    m_cStep;	
	BYTE    m_cTimeLeft;
}T_OtaStatus;

typedef enum _OtaError     //OTA����������Ϣ
{
	CA_OTA_ERROR_1,
	CA_OTA_ERROR_2
}E_OtaError;

typedef struct _CaFtaServiceEx
{
	WORD    m_wDemuxID;
	WORD    m_wOrginalNetworkID;
	WORD    m_wNetworkID;
	WORD    m_wTsID;
	WORD    m_wServiceID;
	WORD    m_wVideoPid;
	WORD    m_wAudioPid;
}T_CaFtaServiceEx;

typedef struct _PvrKey
{
	WORD    	m_wPid;
	BYTE   	m_cDemux;
	BOOL    	m_bEkey;
	BYTE    	m_acKey[8];
}T_PvrKey;

#define CA_MAX_COUNTRY_NUM         (20)

typedef struct _FtaCountry
{
	BYTE    m_cFlag;       		//���ñ�־��1�����ã�0��������
	BYTE    m_acCountry[3]; 	//���ҿ��ô���
}T_FtaCountry;

typedef struct _FtaCountryList
{
	WORD    		m_wCountryNum;    //�ṹ�й��Ҵ������
	T_FtaCountry  	m_stCountryList[CA_MAX_COUNTRY_NUM]; //��ŵĹ��Ҵ���
}T_FtaCountryList;

typedef void (*T_CBPvrKey)(T_PvrKey* pstPvrKey);


/**************************************************************************
*Function Name: CS_CA_PVR_StartSrvRecord
*  ����һ����Ŀ¼��ͨ��
*
*Parameters 
*	nServiceID [in]:
*		��Ŀ��
*	nVideoPID[in]:
*		��ƵPID
*	nAudioPID[in]:
*		��ƵPID
*	cDemux[in]:
*		ͨ���ţ�Ŀǰ֧֮�ֶ�·ͨ����
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_PVR_StartSrvRecord(int nServiceID,int nVideoPID,int nAudioPID,BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_PVR_StopSrvRecord
*  ֹͣһ����Ŀ¼��ͨ��
*Parameters 
*	nServiceID [in]:
*		��Ŀ��
*	nVideoPID[in]:
*		��ƵPID
*	nAudioPID[in]:
*		��ƵPID
*	cDemux[in]:
*		ͨ���ţ�Ŀǰ֧֮�ֶ�·ͨ����
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_PVR_StopSrvRecord(int nServiceID,int nVideoPID,int nAudioPID,BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_PVR_RegisterKeyCallback
*  ע�ᱣ��KEY�ص�
*Parameters 
*	hCBPvrKey[in]:
*	      �ص�����
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_PVR_RegisterKeyCallback(T_CBPvrKey hCBPvrKey);

/**************************************************************************
*Function Name: CS_CA_PVR_StartPvrEsPlay
*  ��������¼�ƽ�Ŀ
*
*Parameters 
*	wPid [in]:
*		��ĿPID
*	cDemux[in]:
*		ͨ���ţ�Ŀǰ֧֮�ֶ�·ͨ����
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT  CS_CA_PVR_StartPvrEsPlay(WORD wPid, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_PVR_StopPvrEsPlay
*  ֹͣ����¼�ƽ�Ŀ
*
*Parameters 
*	wPid [in]:
*		��ĿPID
*	cDemux[in]:
*		ͨ���ţ�Ŀǰ֧֮�ֶ�·ͨ����
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT  CS_CA_PVR_StopPvrEsPlay(WORD wPid, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_PVR_SetEvenKey
*  ����һ����Ŀ¼��ͨ��
*
*Parameters 
*	wPid [in]:
*		��ĿPID
*	cDemux[in]:
*		ͨ���ţ�Ŀǰ֧֮�ֶ�·ͨ����
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT  CS_CA_PVR_SetEvenKey(WORD wPid, BYTE cDemux, BYTE acKey[8]);

/**************************************************************************
*Function Name: CS_CA_PVR_SetOddKey
*  ����һ����Ŀ¼��ͨ��
*
*Parameters 
*	wPid [in]:
*		��ĿPID
*	cDemux[in]:
*		ͨ���ţ�Ŀǰ֧֮�ֶ�·ͨ����
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT  CS_CA_PVR_SetOddKey(WORD wPid, BYTE cDemux, BYTE acKey[8]);

/**************************************************************************
*Function Name:CS_CA_GetCaStatus
* ��ȡCA״̬��Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/


INT CS_CA_GetCaStatus(void);

/**************************************************************************
*Function Name:CS_CA_StopGetCaStatus
*ֹͣ��ȡIRD(������)������Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_StopGetCaStatus(void);

/**************************************************************************
*Function Name:CS_CA_GetLoaderStatus
* ��ȡLDR״̬��Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_GetLoaderStatus(void);

/**************************************************************************
*Function Name:CS_CA_GetIrdStatus
* ��ȡIRD(������)������Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_GetIrdStatus(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Add
* ����һ���ʼ�
*
*Parameters 
*	pstTxt[in]:�ʼ�����
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Add(T_CaMail* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Del
* ɾ��ĳһ���ʼ�
*
*Parameters 
*	nIndex[in]:�ʼ��±꣬��0��ʼ
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Del(DWORD dwIndex);

/**************************************************************************
*Function Name:CS_CA_TXT_DelAll
* ɾ�����б�������ʼ�
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_DelAll(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetNum
* ��ȡ���ʼ���
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* ��ȡ����ĳһ���ʼ�������
*
*Parameters 
*	nIndex[in]:�ʼ��±꣬��0��ʼ
*	ppstTxt[out]:�����ʼ��Ĵ洢��ַ
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(DWORD dwIndex, T_CaMail** ppstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNew
* ��ѯĳһ���ʼ��Ƿ���δ���ʼ�
*
*Parameters 
*	eType[in]:�ʼ�����
*
*Return Values: 
*	1: TRUE 
*	0: FALSE
**************************************************************************/
BOOL CS_CA_TXT_HaveNew(T_TxtType eType);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* ��ȡδ���ʼ�����
*
*Parameters 
*	��
*
*Return Values: 
*	����δ���ʼ�����
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum();

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* �������ʼ����浽flash�У�һ���ǹػ���ϵ�ǰ���еĲ���
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_Txt_SetReadFlag
* �����ʼ��Ƿ��Ѷ���־
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	bRead[in]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_SetLockFalg
* �����ʼ��Ƿ�����
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	bLockFlag[in]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_SetLockFalg(DWORD dwIndex, BOOL bLockFlag);

/**************************************************************************
*Function Name:CS_CA_TXT_GetLockFalg
* �õ�ָ���ʼ�������״̬
*
*Parameters 
*	nIndex[in]:�ʼ��洢���±꣬��0��ʼ
*	pbLockFlag[out]:�Ѷ�(TRUE),δ��(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_GetLockFalg(DWORD dwIndex, BOOL* pbLockFlag);

/**************************************************************************
*Function Name:CS_CA_OTA_GetTunerParm
* ��ȡ����ʱ����Ĳ���
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
void CS_CA_OTA_GetTunerParm(void);

/**************************************************************************
*Function Name:CS_CA_OTA_GetUpdateInfo
* ��ȡ���������н�������ʾ����Ϣ
*
*Parameters 
*	��
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
void CS_CA_OTA_GetUpdateInfo(void);

/**************************************************************************
*Function Name:CS_CA_OTA_SaveCfg
* �������������Ϣ
*
*Parameters 
*	pstOtaCfg[in]
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_OTA_SaveCfg(T_OtaCfg* pstOtaCfg);

/**************************************************************************
*Function Name: CS_CA_SetAreaBlock
*  �����������ޣ�����NIT�����ṹ��
*
*Parameters 
*	pstCountypList[in]:���Ҵ�������͹��Ҵ����
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_SetAreaBlock (T_FtaCountryList* pstCountypList);

/**************************************************************************
*Function Name: CS_CA_GetRegionInfo
*  ��ȡ���ܿ��ڲ���Region ֵ��SubRegion ֵ
*  �������:   cSector
*  ���ܿ��ڲ�cSector �ţ�Ŀǰ��֧��1
*  �������:  pcRegion �� pcSubRegion
*  ���ڻ�ȡ���ܿ��ڲ���Region ��SubRegion ��
*  ����ֵ
*   0:   SUCCESS ����ɹ�
* -1:   FAILURE   ����ʧ��
**************************************************************************/
INT CS_CA_GetRegionInfo(BYTE cSector, BYTE* pcRegion, BYTE* pcSubRegion);

/**************************************************************************
*Function Name: CS_CA_GetFTAType
*  ��ȡ����������
*  �������:   ��
*  ����ֵ ����������
**************************************************************************/
extern T_CaFtaType  CS_CA_GetFTAType(void);

/**************************************************************************
*Function Name: CS_CA_GetFlashAddress
*  ��ȡCA����Flash��ַ
*  �������:   ��
*  ����ֵ ��Ӧ�÷����Flash�ռ��׵�ַ
**************************************************************************/

extern DWORD  CS_CA_GetFlashAddress(DWORD dwFlashSize);

#ifdef  __cplusplus
}
#endif

#endif //_CS_IRDETO_INTERFACE_H_

