/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_IRDETO_Loader.h  
* Author:      wansuanlin
* Date:        Match 2006 
* Description: this file provide loader function
*            
*****************************************************************************************************************************/
#include "mmcp_typedef.h"

/****************************************************************************************************************************
* 			                                           Revision History                                                     *
*****************************************************************************************************************************
*   Revision  : V1.0
*   Programmer: Wan suanlin
*   Create    : 2006-03-17
*****************************************************************************************************************************/
#ifndef _CS_IRDETO_LOADER_H_
#define _CS_IRDETO_LOADER_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _CpcbInfo
{
    BYTE      m_cLoadNum;  /*当前下载号*/
    DWORD     m_dwBin2irdVer;
    DWORD     m_dwBin2irdSerialNum;
    BYTE      m_acSignTimeDate[5];
    BYTE      m_acDownloadTimeDate[5];
    WORD      m_wDownloadDuration;
    DWORD     m_dwImageSize;
}T_CpcbInfo;

typedef struct _BbcbInfo
{
    WORD      m_wCRC;
    BYTE      m_cManufactureCode;  /*厂商代号*/                  
    BYTE      m_cHW_Version;       /*硬件版本号*/
    BYTE      m_cVerMajor;
    BYTE      m_cVerMinor;              
    BYTE      m_acSerialNumber[4];  
    BYTE      m_acRandomNumber[4];   
	BYTE      m_acRev[9];
}T_BbcbInfo; 

typedef struct _KeyStatusEx
{
    BYTE      m_cKeyStatus;
    BYTE      m_cLastImage;
    WORD      m_wSysID;
    WORD      m_wKeyVer;
    WORD      m_wVariant; /*市场号*/
    WORD      m_wSigVer;
    DWORD     m_dwRandomNum;
}T_KeyStatusEx;

typedef struct _LoaderIrdInfo
{
    WORD      m_wManufacturerCode;  /*厂商代号*/
    WORD      m_wHardwareCode;      /*硬件版本号*/
    WORD      m_wVariant;           /*市场号，由Irdeto分配*/
    WORD      m_wLoadNum;           /*当前下载号*/
}T_LoaderIrdInfo;

typedef struct
{
   WORD u16SysID;               /* System ID                                 */
   WORD s16SysIDValid;          /* 0=u16SysID valid, -1=u16SysID invalid     */
   WORD u16KeyVer;              /* Key version                               */
   WORD s16KeyVerValid;         /* 0=u16KeyVer valid, -1=u16KeyVer invalid   */
   WORD u16SigVer;              /* Signature version                         */
   WORD s16SigVerValid;         /* 0=u16SigVer valid, -1=u16SigVer invalid   */
   WORD u16Variant;             /* Operator variant                          */
   WORD s16VariantValid;        /* 0=u16Variant valid, -1=u16Variant invalid */
   BYTE   u8BlockFailMask;      /* Bit mask of failed Irdeto NVRAM partition */
                                /* blocks (block0 = bit0)                    */
} IHL_STATUS, *IHL_PSTATUS;

typedef enum _CaFtaType
{
	E_CA_FTA_SAT   = 0x43,
	E_CA_FTA_CABLE = 0x44,
	E_CA_FTA_TER   = 0x5A,
	E_CA_FTA_MAX
}T_CaFtaType;


//IRDStatus
#define   CA_MAX_SIZE_IRD_SERIALNUMBER      (32)
#define	CA_MAX_SIZE_SC_STATUS               (32)
#define	CA_MAX_SIZE_IRD_VER                 (64)
#define	CA_MAX_SIZE_FIRMWARE                (64)
#define	CA_MAX_SIZE_CAM_VER                 (64)
#define	CA_MAX_SIZE_SC_VER                  (32)
#define	CA_MAX_SIZE_LOASER                  (64)
#define	CA_MAX_SIZE_SW_VER                  (64)
#define CA_MAX_SIZE_REV                     (32) //增加的

typedef struct _CaIrdStatus
{
	BYTE	m_szSerialNumber[CA_MAX_SIZE_IRD_SERIALNUMBER];
	BYTE	m_szRandomNumber[CA_MAX_SIZE_IRD_SERIALNUMBER]; //Random NO.
	BYTE	m_szScStatus[CA_MAX_SIZE_SC_STATUS];
	BYTE	m_szIrdVer[CA_MAX_SIZE_IRD_VER];
	BYTE	m_szFirmware[CA_MAX_SIZE_FIRMWARE];
	BYTE	m_szCamVer[CA_MAX_SIZE_CAM_VER];
	BYTE	m_szScVer[CA_MAX_SIZE_SC_VER];			//no used
	BYTE	m_szLoader[CA_MAX_SIZE_LOASER];
	BYTE    m_szRev[CA_MAX_SIZE_REV];             //增加的
	//BYTE	m_szIRDSW[CA_MAX_SIZE_SW_VER];
	//BYTE	m_szMiddleSW[CA_MAX_SIZE_SW_VER];	
	//BYTE	m_szAPPSW[CA_MAX_SIZE_SW_VER];
	//BYTE	m_szDRIVERSW[CA_MAX_SIZE_SW_VER];
}T_CaIrdStatus;

//LoaderStatus
#define	CA_MAX_SIZE_MANUFACTURE_ID          (8)
#define	CA_MAX_SIZE_HW_VER                  (8)
#define	CA_MAX_SIZE_LD_SEQNO                (8)
#define	CA_MAX_SIZE_KEY_SEQNO               (8)
#define	CA_MAX_SIZE_VARIANT                 (8)
#define	CA_MAX_SIZE_KEY_SYSID               (8)
#define	CA_MAX_SIZE_KEYSTATUS               (8)
#define	CA_MAX_SIZE_KEY_VER                 (8)
#define	CA_MAX_SIZE_BIN2IRD_VER             (16)
#define	CA_MAX_SIZE_BIN2IRD_SER             (16)
#define	CA_MAX_SIZE_SIGN_DATE               (16)
#define	CA_MAX_SIZE_DL_DATE                 (16)
#define	CA_MAX_SIZE_SIGN_TIME               (16)
#define	CA_MAX_SIZE_DL_TIME                 (16)
#define	CA_MAX_SIZE_DL_DUR                  (16)
#define	CA_MAX_SIZE_DL_IMAGE                (16)

typedef struct _CaLoaderStatus
{
	BYTE	m_szSerialNumber[CA_MAX_SIZE_IRD_SERIALNUMBER];
	BYTE	m_szLoader[CA_MAX_SIZE_LOASER];
	BYTE	m_szManuID[CA_MAX_SIZE_MANUFACTURE_ID];
	BYTE	m_szHWVer[CA_MAX_SIZE_HW_VER];
	BYTE	m_szLDSeqNo[CA_MAX_SIZE_LD_SEQNO];
	BYTE	m_szKEYSeqNo[CA_MAX_SIZE_KEY_SEQNO];
	BYTE	m_szVARIANT[CA_MAX_SIZE_VARIANT];
	BYTE	m_szKEYSysId[CA_MAX_SIZE_KEY_SYSID];
	BYTE	m_szKEYStatus[CA_MAX_SIZE_KEYSTATUS];
	BYTE	m_szKEYVer[CA_MAX_SIZE_KEY_VER];
	BYTE	m_szBIN2IRDVer[CA_MAX_SIZE_BIN2IRD_VER];
	BYTE	m_szBIN2IRDSer[CA_MAX_SIZE_BIN2IRD_SER];
	BYTE	m_szSignDate[CA_MAX_SIZE_SIGN_DATE];
	BYTE	m_szDLDate[CA_MAX_SIZE_DL_DATE];
	BYTE	m_szSignTime[CA_MAX_SIZE_SIGN_TIME];
	BYTE	m_szDLTime[CA_MAX_SIZE_DL_TIME];
	BYTE	m_szDLDur[CA_MAX_SIZE_DL_DUR];
	BYTE	m_szImageSize[CA_MAX_SIZE_DL_IMAGE];
}T_CaLoaderStatus;


typedef struct _QpskSwitch
{
	BYTE	m_cSwitch0_12V;     //SWITCH_0V | SWITCH_12V
	BYTE	m_c22K;             //SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBPower;        //SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBType;         //SINGLE | UNIVERSAL
	DWORD	m_dwLNBLowFreq;     //low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]
	DWORD	m_dwLNBMidFreq;     //middle LNB frequency in Universal Mod  [unit:KHz]
	DWORD	m_dwLNBHighFreq;    //high LNB frequency in Universal Mod   [unit:KHz]
	BYTE	m_cDiSEqC10;        //DiSEqC1.0开关	( PORT_A | PORT_B | PORT_C | PORT_D )
	BYTE	m_cDiSEqC11;        //DiSEqC1.0 ( 0-16 and 0 for not used )
	BYTE	m_cToneBurst;       //Tone Burst开关 ( TONEBURST_NONE / TONEBURST_A / TONEBURST_B )
	BYTE	m_cDiSEqCRepeat;    //重复次数 ( NoRepeat / OnceRepeat / TwiceRepeat )
	DWORD	m_dwFreqencyOffset; //channel frequency offset in KHz
	BYTE	m_cIQ;              //0-Normal; 1-Invers
}T_QpskSwitch;

typedef struct _SatelliteSystem
{
	DWORD      m_dwFrequency;
	WORD        m_wPrbitalPosition;
	BYTE          m_cWestEastFlag;
	BYTE          m_cPolarization;
	BYTE          m_cModulation;
	DWORD      m_dwSymbolRate;
	BYTE       m_cFecInner;
	T_QpskSwitch  m_stQpskSwitch;
}T_SatelliteSystem;

typedef struct _TerrestrialSystem
{
	DWORD   m_dwCentreFrequency;
	BYTE       m_cBandWidth;
	BYTE       m_cConstellation;
	BYTE       m_cHierarchyInfo;
	BYTE       m_cCodeRateHpStream;
	BYTE       m_cCodeRateLpStream;
	BYTE       m_cGuardInterval;
	BYTE       m_cTransmissionMode;
	BYTE       m_cOtherFrequencyFlag;	
}T_TerrestrialSystem;


typedef struct _CableSystem
{
	DWORD      m_dwFrequency;
	BYTE          m_cFecOuter;
	BYTE          m_cModulation;
	DWORD      m_dwSymbolRate;
	BYTE       m_cFecInner;
}T_CableSystem;

typedef union _DeliverySystem 
{
   	T_CableSystem              m_stCableStytle;
	T_SatelliteSystem           m_stSatelliteStytle;
	T_TerrestrialSystem        m_stTerrestrialStytle;
}T_DeliverySystem;


typedef struct _OtaCfg     // OTA升级参数
{
	WORD    m_wOtaPid;
	BYTE    m_nOtaTableId;
	INT     m_nOtaTime;
	
	T_CaFtaType         m_eType; 
	T_DeliverySystem    m_stDlySys;
}T_OtaCfg;

INT CS_CA_Loader_Init(void);
INT CS_CA_Loader_GetCPCB(T_CpcbInfo*  pstBuf );
INT CS_CA_Loader_GetBBCB(T_BbcbInfo*  pstBuf);
INT CS_CA_Loader_GetKeyStatus(T_KeyStatusEx*  pstKeyStatus);
INT CS_CA_Loader_SaveOtaInfo(T_OtaCfg*  pstOtaInfo);
BOOL CS_CA_Loader_ReadIrdCfg(T_LoaderIrdInfo*  pIrdCfg);
BOOL CS_CA_Loader_GetIrdStatus(T_CaIrdStatus* pstIrdStatus);
BOOL CS_CA_GetLdrStatus(T_CaLoaderStatus* pstLoaderStatus);

WORD I_HLSUP_GetStatusInfo (IHL_PSTATUS pstStatus);

void LFromMJDUTC(BYTE* MjdUtc,BYTE* year,BYTE* month,BYTE* day,BYTE* hour,BYTE* min,BYTE* second);
int  IMin(int x1,int x2);

#ifdef  __cplusplus
}
#endif

#endif //_CS_IRDETO_LOADER_H_



