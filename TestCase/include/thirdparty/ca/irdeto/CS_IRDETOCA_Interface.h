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
	BYTE    	m_cInfoNo;     	//特殊信息号
	INT     	m_nOvertime;  	//超时限制
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
	CA_TXT_MAIL         	= 0,//Text -- Mailbox,	邮件消息
	CA_TXT_ANNOUNCE     	= 1,//Text -- Announcement,广播通知
	CA_TXT_CLUB         	= 2 //Club number,会员号
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

typedef struct _CaMsg		// 邮件属性
{
	BYTE    	m_cType;     			//T_TxtType// 0: Text_Mail  1: Text_Announcement 
	BYTE    	m_cClass;    			//T_TxtShowModel// 0: Normal     1: Time              2: ForcedDisplay
	BYTE    	m_cPriority;
	DWORD   	m_dwLen;
	T_CaDateTime  m_stDeleteTime; 	//在TIMED邮件中存在
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
	WORD   m_wDisplayMethod;//界面覆盖值(20-100)
	WORD   m_wDuration;
	DWORD  m_dwLen;
	BYTE   m_acData[CA_MAX_SIZE_TXT_MAIL];
}T_CaTms;

typedef struct _CaMail		//邮件结构体
{	
	BOOL    m_bReadFlag;          	//已读标志
	BOOL    m_bLockFlag;	      	 //邮件锁定标志
	T_CaDateTime  m_stArriveTime; //邮件到达的时间
	T_CaMsg m_stMsg;
}T_CaMail;

typedef struct _OtaStatus  //OTA升级状态信息
{
	BYTE    m_cStep;	
	BYTE    m_cTimeLeft;
}T_OtaStatus;

typedef enum _OtaError     //OTA升级错误信息
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
	BYTE    m_cFlag;       		//可用标志：1：可用；0：不可用
	BYTE    m_acCountry[3]; 	//国家可用代码
}T_FtaCountry;

typedef struct _FtaCountryList
{
	WORD    		m_wCountryNum;    //结构中国家代码个数
	T_FtaCountry  	m_stCountryList[CA_MAX_COUNTRY_NUM]; //存放的国家代码
}T_FtaCountryList;

typedef void (*T_CBPvrKey)(T_PvrKey* pstPvrKey);


/**************************************************************************
*Function Name: CS_CA_PVR_StartSrvRecord
*  启动一个节目录制通道
*
*Parameters 
*	nServiceID [in]:
*		节目号
*	nVideoPID[in]:
*		视频PID
*	nAudioPID[in]:
*		音频PID
*	cDemux[in]:
*		通道号（目前之支持二路通道）
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_PVR_StartSrvRecord(int nServiceID,int nVideoPID,int nAudioPID,BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_PVR_StopSrvRecord
*  停止一个节目录制通道
*Parameters 
*	nServiceID [in]:
*		节目号
*	nVideoPID[in]:
*		视频PID
*	nAudioPID[in]:
*		音频PID
*	cDemux[in]:
*		通道号（目前之支持二路通道）
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_PVR_StopSrvRecord(int nServiceID,int nVideoPID,int nAudioPID,BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_PVR_RegisterKeyCallback
*  注册保存KEY回掉
*Parameters 
*	hCBPvrKey[in]:
*	      回调函数
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_PVR_RegisterKeyCallback(T_CBPvrKey hCBPvrKey);

/**************************************************************************
*Function Name: CS_CA_PVR_StartPvrEsPlay
*  启动播放录制节目
*
*Parameters 
*	wPid [in]:
*		节目PID
*	cDemux[in]:
*		通道号（目前之支持二路通道）
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT  CS_CA_PVR_StartPvrEsPlay(WORD wPid, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_PVR_StopPvrEsPlay
*  停止播放录制节目
*
*Parameters 
*	wPid [in]:
*		节目PID
*	cDemux[in]:
*		通道号（目前之支持二路通道）
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT  CS_CA_PVR_StopPvrEsPlay(WORD wPid, BYTE cDemux);

/**************************************************************************
*Function Name: CS_CA_PVR_SetEvenKey
*  启动一个节目录制通道
*
*Parameters 
*	wPid [in]:
*		节目PID
*	cDemux[in]:
*		通道号（目前之支持二路通道）
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT  CS_CA_PVR_SetEvenKey(WORD wPid, BYTE cDemux, BYTE acKey[8]);

/**************************************************************************
*Function Name: CS_CA_PVR_SetOddKey
*  启动一个节目录制通道
*
*Parameters 
*	wPid [in]:
*		节目PID
*	cDemux[in]:
*		通道号（目前之支持二路通道）
*		
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT  CS_CA_PVR_SetOddKey(WORD wPid, BYTE cDemux, BYTE acKey[8]);

/**************************************************************************
*Function Name:CS_CA_GetCaStatus
* 获取CA状态信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/


INT CS_CA_GetCaStatus(void);

/**************************************************************************
*Function Name:CS_CA_StopGetCaStatus
*停止获取IRD(机顶盒)配置信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_StopGetCaStatus(void);

/**************************************************************************
*Function Name:CS_CA_GetLoaderStatus
* 获取LDR状态信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_GetLoaderStatus(void);

/**************************************************************************
*Function Name:CS_CA_GetIrdStatus
* 获取IRD(机顶盒)配置信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_GetIrdStatus(void);

/**************************************************************************
*Function Name:CS_CA_TXT_Add
* 保存一封邮件
*
*Parameters 
*	pstTxt[in]:邮件内容
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Add(T_CaMail* pstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_Del
* 删除某一封邮件
*
*Parameters 
*	nIndex[in]:邮件下标，从0开始
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Del(DWORD dwIndex);

/**************************************************************************
*Function Name:CS_CA_TXT_DelAll
* 删除所有保存过的邮件
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_DelAll(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetNum
* 获取总邮件数
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetNum(void);

/**************************************************************************
*Function Name:CS_CA_TXT_GetInfo
* 获取具体某一封邮件的内容
*
*Parameters 
*	nIndex[in]:邮件下标，从0开始
*	ppstTxt[out]:返回邮件的存储地址
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_GetInfo(DWORD dwIndex, T_CaMail** ppstTxt);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNew
* 查询某一类邮件是否有未读邮件
*
*Parameters 
*	eType[in]:邮件类型
*
*Return Values: 
*	1: TRUE 
*	0: FALSE
**************************************************************************/
BOOL CS_CA_TXT_HaveNew(T_TxtType eType);

/**************************************************************************
*Function Name:CS_CA_TXT_HaveNewMailNum
* 获取未读邮件个数
*
*Parameters 
*	无
*
*Return Values: 
*	返回未读邮件个数
**************************************************************************/
INT CS_CA_TXT_HaveNewMailNum();

/**************************************************************************
*Function Name:CS_CA_TXT_Save
* 将所有邮件保存到flash中，一般是关机或断电前进行的操作
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_Save(void);

/**************************************************************************
*Function Name:CS_CA_Txt_SetReadFlag
* 设置邮件是否已读标志
*
*Parameters 
*	nIndex[in]:邮件存储的下标，从0开始
*	bRead[in]:已读(TRUE),未读(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT CS_CA_TXT_SetReadFlag(DWORD dwIndex, BOOL bRead);

/**************************************************************************
*Function Name:CS_CA_TXT_SetLockFalg
* 设置邮件是否锁定
*
*Parameters 
*	nIndex[in]:邮件存储的下标，从0开始
*	bLockFlag[in]:已读(TRUE),未读(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_SetLockFalg(DWORD dwIndex, BOOL bLockFlag);

/**************************************************************************
*Function Name:CS_CA_TXT_GetLockFalg
* 得到指定邮件的锁定状态
*
*Parameters 
*	nIndex[in]:邮件存储的下标，从0开始
*	pbLockFlag[out]:已读(TRUE),未读(FALSE)
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
INT  CS_CA_TXT_GetLockFalg(DWORD dwIndex, BOOL* pbLockFlag);

/**************************************************************************
*Function Name:CS_CA_OTA_GetTunerParm
* 获取升级时所需的参数
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
void CS_CA_OTA_GetTunerParm(void);

/**************************************************************************
*Function Name:CS_CA_OTA_GetUpdateInfo
* 获取升级过程中界面需提示的信息
*
*Parameters 
*	无
*
*Return Values: 
*	0 : SUCCESS 
*	-1: FAILURE
**************************************************************************/
void CS_CA_OTA_GetUpdateInfo(void);

/**************************************************************************
*Function Name:CS_CA_OTA_SaveCfg
* 保存升级后的信息
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
*  设置区域受限：解析NIT表，填充结构体
*
*Parameters 
*	pstCountypList[in]:国家代码个数和国家代码号
*
*Return Values: 
*		0 : SUCCESS 
*		-1: FAILURE
**************************************************************************/
INT CS_CA_SetAreaBlock (T_FtaCountryList* pstCountypList);

/**************************************************************************
*Function Name: CS_CA_GetRegionInfo
*  获取智能卡内部的Region 值和SubRegion 值
*  输入参数:   cSector
*  智能卡内部cSector 号，目前仅支持1
*  输出参数:  pcRegion ， pcSubRegion
*  用于获取智能卡内部的Region 和SubRegion 号
*  返回值
*   0:   SUCCESS 请求成功
* -1:   FAILURE   请求失败
**************************************************************************/
INT CS_CA_GetRegionInfo(BYTE cSector, BYTE* pcRegion, BYTE* pcSubRegion);

/**************************************************************************
*Function Name: CS_CA_GetFTAType
*  获取机网络类型
*  输入参数:   无
*  返回值 ：网络类型
**************************************************************************/
extern T_CaFtaType  CS_CA_GetFTAType(void);

/**************************************************************************
*Function Name: CS_CA_GetFlashAddress
*  获取CA所需Flash地址
*  输入参数:   无
*  返回值 ：应用分配的Flash空间首地址
**************************************************************************/

extern DWORD  CS_CA_GetFlashAddress(DWORD dwFlashSize);

#ifdef  __cplusplus
}
#endif

#endif //_CS_IRDETO_INTERFACE_H_

